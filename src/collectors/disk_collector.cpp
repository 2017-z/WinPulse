#include "collectors/disk_collector.h"
#include <sstream>
#include <iomanip>
#include <iostream>
#include <vector>
#include <pdhmsg.h> 

// 链接 pdh.lib
#pragma comment(lib, "pdh.lib")

namespace WinPulse::Collectors {

    DiskCollector::DiskCollector() {
        initPdh();
    }

    DiskCollector::~DiskCollector() {
        if (m_queryHandle) {
            PdhCloseQuery(m_queryHandle);
        }
    }

    void DiskCollector::initPdh() {
        if (PdhOpenQueryA(NULL, 0, &m_queryHandle) != ERROR_SUCCESS) {
            std::cerr << "[DiskCollector] Failed to open PDH query." << std::endl;
            return;
        }

        // 1. 自动发现并添加计数器
        discoverDisks();

        // 2. 只有找到了磁盘才算初始化成功
        if (!m_disks.empty()) {
            PdhCollectQueryData(m_queryHandle); // 第一次采集建立基准
            m_isInitialized = true;
        } else {
             std::cerr << "[DiskCollector] No physical disks found via PDH." << std::endl;
        }
    }

    void DiskCollector::discoverDisks() {
        const char* wildcardPath = "\\PhysicalDisk(*)\\Disk Read Bytes/sec";
        
        PDH_STATUS status;
        DWORD bufferSize = 0;

        // 第一步：获取缓冲区大小
        status = PdhExpandWildCardPathA(NULL, wildcardPath, NULL, &bufferSize, 0);
        
        // 注意：这里即便失败，通常也会返回 bufferSize，所以我们要综合判断
        if (status != PDH_MORE_DATA && status != ERROR_SUCCESS) {
            // 如果连 buffer 大小都取不到，说明可能路径不对或权限不足
            // 某些系统第一次调用可能返回 ERROR_SUCCESS 但 bufferSize 为 0
            if (bufferSize == 0) {
                std::cerr << "[DiskCollector] Failed to estimate buffer. Status: " << std::hex << status << std::endl;
                return;
            }
        }

        // 第二步：分配内存并获取路径列表
        std::vector<char> buffer(bufferSize + 1); // 多留一个字节防溢出
        status = PdhExpandWildCardPathA(NULL, wildcardPath, buffer.data(), &bufferSize, 0);
        
        if (status != ERROR_SUCCESS) {
             std::cerr << "[DiskCollector] Failed to expand wildcard path. Status: " << std::hex << status << std::endl;
             return;
        }

        char* currentPath = buffer.data();
        while (*currentPath) {
            std::string fullPath(currentPath);
            
            // fullPath 类似: \PhysicalDisk(0 C:)\Disk Read Bytes/sec
            size_t start = fullPath.find('(');
            size_t end = fullPath.find(')');
            
            if (start != std::string::npos && end != std::string::npos && end > start) {
                std::string instanceName = fullPath.substr(start + 1, end - start - 1);

                // 过滤掉 _Total
                if (instanceName != "_Total") {
                    DiskUnit unit;
                    unit.name = instanceName; 

                    // 构造精确路径 (使用 English Counter)
                    std::string readPath = "\\PhysicalDisk(" + instanceName + ")\\Disk Read Bytes/sec";
                    std::string writePath = "\\PhysicalDisk(" + instanceName + ")\\Disk Write Bytes/sec";

                    PDH_STATUS s1 = PdhAddEnglishCounterA(m_queryHandle, readPath.c_str(), 0, &unit.hRead);
                    PDH_STATUS s2 = PdhAddEnglishCounterA(m_queryHandle, writePath.c_str(), 0, &unit.hWrite);

                    if (s1 == ERROR_SUCCESS && s2 == ERROR_SUCCESS) {
                        m_disks.push_back(unit);
                    }
                }
            }
            
            // 移动到下一个字符串（以 \0 分隔）
            currentPath += fullPath.length() + 1;
        }
    }

    void DiskCollector::collect() {
        if (!m_isInitialized || !m_queryHandle) return;

        if (PdhCollectQueryData(m_queryHandle) != ERROR_SUCCESS) return;

        for (auto& disk : m_disks) {
            PDH_FMT_COUNTERVALUE readVal, writeVal;

            if (PdhGetFormattedCounterValue(disk.hRead, PDH_FMT_DOUBLE, NULL, &readVal) == ERROR_SUCCESS) {
                disk.readKBps = readVal.doubleValue / 1024.0;
            } else {
                disk.readKBps = 0;
            }

            if (PdhGetFormattedCounterValue(disk.hWrite, PDH_FMT_DOUBLE, NULL, &writeVal) == ERROR_SUCCESS) {
                disk.writeKBps = writeVal.doubleValue / 1024.0;
            } else {
                disk.writeKBps = 0;
            }
        }
    }

    std::string DiskCollector::getFormattedOutput() const {
        if (!m_isInitialized) return "Disk: (Init Error)";
        if (m_disks.empty()) return "Disk: (No Disks)";

        std::ostringstream oss;
        oss << std::fixed << std::setprecision(1);

        for (size_t i = 0; i < m_disks.size(); ++i) {
            const auto& disk = m_disks[i];
            
            oss << "Disk(" << disk.name << "): "
                << "R " << std::setw(5) << disk.readKBps << "K "
                << "W " << std::setw(5) << disk.writeKBps << "K";

            if (i < m_disks.size() - 1) {
                oss << " | ";
            }
        }
        return oss.str();
    }
}