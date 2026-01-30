#include "collectors/disk_collector.h"
#include <sstream>
#include <iomanip>
#include <iostream>

#pragma comment(lib, "pdh.lib") // 我提供了 MSVC 的一种链接方式，但 CMake 中实际已经配置，作为冗余备用013026-z

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
        // 1. 打开一个查询句柄
        if (PdhOpenQueryA(NULL, 0, &m_queryHandle) != ERROR_SUCCESS) {
            std::cerr << "[DiskCollector] Failed to open PDH query." << std::endl;
            return;
        }

        // 2. 添加计数器
        // 发现英文系统与中文系统的计数器名称居然不同，保持一下一致性013026-z
        PDH_STATUS s1 = PdhAddEnglishCounterA(m_queryHandle, "\\PhysicalDisk(_Total)\\Disk Read Bytes/sec", 0, &m_readCounter);
        PDH_STATUS s2 = PdhAddEnglishCounterA(m_queryHandle, "\\PhysicalDisk(_Total)\\Disk Write Bytes/sec", 0, &m_writeCounter);

        if (s1 != ERROR_SUCCESS || s2 != ERROR_SUCCESS) {
            std::cerr << "[DiskCollector] Failed to add counters. Error code: " << std::hex << (s1 != ERROR_SUCCESS ? s1 : s2) << std::endl;
            // 资源清理
            PdhCloseQuery(m_queryHandle);
            m_queryHandle = NULL;
            return;
        }

        // 3. 第一次采集（建立基准值，通常没有数据）
        PdhCollectQueryData(m_queryHandle);
        m_isInitialized = true;
    }

    void DiskCollector::collect() {
        if (!m_isInitialized || !m_queryHandle) return;

        // 再次采集，PDH 会自动计算两次采集之间的时间差和增量
        if (PdhCollectQueryData(m_queryHandle) != ERROR_SUCCESS) return;

        PDH_FMT_COUNTERVALUE readVal, writeVal;

        // 获取格式化后的 double 值 (PDH_FMT_DOUBLE)
        if (PdhGetFormattedCounterValue(m_readCounter, PDH_FMT_DOUBLE, NULL, &readVal) == ERROR_SUCCESS) {
            m_data.readKBps = readVal.doubleValue / 1024.0; // Bytes -> KB
        }

        if (PdhGetFormattedCounterValue(m_writeCounter, PDH_FMT_DOUBLE, NULL, &writeVal) == ERROR_SUCCESS) {
            m_data.writeKBps = writeVal.doubleValue / 1024.0;
        }
    }

    std::string DiskCollector::getFormattedOutput() const {
        if (!m_isInitialized) return "Disk: (Init Error)";

        std::ostringstream oss;
        oss << std::fixed << std::setprecision(1);
        oss << "Disk: R " << std::setw(5) << m_data.readKBps << "K/s | "
            << "W " << std::setw(5) << m_data.writeKBps << "K/s";
        return oss.str();
    }
}