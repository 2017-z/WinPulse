#include "collectors/process_collector.h"
#include <windows.h>
#include <tlhelp32.h>
#include <psapi.h>
#include <algorithm>
#include <sstream>
#include <iomanip>
#include <iostream>

namespace WinPulse::Collectors {

    ProcessCollector::ProcessCollector(int topN) : m_topN(topN) {}

    std::string ProcessCollector::wideToString(const std::wstring& wstr) const {
        if (wstr.empty()) return std::string();
        int size_needed = WideCharToMultiByte(CP_ACP, 0, &wstr[0], (int)wstr.size(), NULL, 0, NULL, NULL);
        std::string strTo(size_needed, 0);
        WideCharToMultiByte(CP_ACP, 0, &wstr[0], (int)wstr.size(), &strTo[0], size_needed, NULL, NULL);
        return strTo;
    }

    void ProcessCollector::collect() {
        HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
        if (hSnapshot == INVALID_HANDLE_VALUE) {
            std::cerr << "[Error] Failed to snapshot processes. Error: " << GetLastError() << std::endl;
            return;
        }

        PROCESSENTRY32W pe32;
        pe32.dwSize = sizeof(PROCESSENTRY32W);

        if (!Process32FirstW(hSnapshot, &pe32)) {
            CloseHandle(hSnapshot);
            return;
        }

        std::vector<ProcessInfo> allProcs;
        // 增加容量，预留 1024 个位置，覆盖绝大多数桌面场景，避免内存重分配
        allProcs.reserve(1024); 

        do {
            // 过滤掉 PID 0 (Idle) 和 PID 4 (System)，它们无法读取且无意义
            if (pe32.th32ProcessID == 0 || pe32.th32ProcessID == 4) {
                continue;
            }

            HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, pe32.th32ProcessID);
            
            if (hProcess) {
                PROCESS_MEMORY_COUNTERS pmc;
                if (GetProcessMemoryInfo(hProcess, &pmc, sizeof(pmc))) {
                    ProcessInfo info;
                    info.name = wideToString(pe32.szExeFile);
                    info.memoryUsageBytes = pmc.WorkingSetSize;
                    allProcs.push_back(info);
                }
                CloseHandle(hProcess);
            } else {
                // 捕获并打印失败原因
                // 注意：只打印到 cerr，避免污染主日志文件
                // for 后面如果可能存在的Contributors（虽然不会有人在意叭：P）：
                // 如果你在更改这个项目，那么你应该仅在调试时开启此输出，否则屏幕会被“拒绝访问”刷屏
                /* DWORD err = GetLastError();
                // 错误码 5 = ERROR_ACCESS_DENIED (权限不足)
                if (err == ERROR_ACCESS_DENIED) {
                     // 很多杀毒软件或系统服务会拒绝访问，这是正常的
                     // std::cerr << "[Access Denied] PID: " << pe32.th32ProcessID << " Name: " << wideToString(pe32.szExeFile) << std::endl;
                } else {
                     std::cerr << "[OpenProcess Failed] PID: " << pe32.th32ProcessID 
                               << " Error: " << err << std::endl;
                }
                */
            }

        } while (Process32NextW(hSnapshot, &pe32));

        CloseHandle(hSnapshot);

        std::sort(allProcs.begin(), allProcs.end(), [](const ProcessInfo& a, const ProcessInfo& b) {
            return a.memoryUsageBytes > b.memoryUsageBytes;
        });

        m_topProcesses.clear();
        for (int i = 0; i < m_topN && i < allProcs.size(); ++i) {
            m_topProcesses.push_back(allProcs[i]);
        }
    }

    std::string ProcessCollector::getFormattedOutput() const {
        std::ostringstream oss;
        oss << "Top" << m_topN << " Mem: ";
        
        if (m_topProcesses.empty()) {
            return "Top Mem: (No Data / Access Denied)";
        }

        for (size_t i = 0; i < m_topProcesses.size(); ++i) {
            const auto& p = m_topProcesses[i];
            double memMB = p.memoryUsageBytes / (1024.0 * 1024.0);
            
            oss << p.name << "(" << std::fixed << std::setprecision(1) << memMB << "M)";
            
            if (i < m_topProcesses.size() - 1) {
                oss << ", ";
            }
        }
        return oss.str();
    }
}