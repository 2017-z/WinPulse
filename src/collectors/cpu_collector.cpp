#include "collectors/cpu_collector.h"
#include <sstream>
#include <iomanip>

namespace WinPulse::Collectors {

    CpuCollector::CpuCollector() {
        // 初始化时先执行一次，要建立基准值，否则第一次输出为0
        collect(); 
    }

    ULONGLONG CpuCollector::fileTimeToInt64(const FILETIME& ft) const {
        return ((ULONGLONG)ft.dwHighDateTime << 32) | ft.dwLowDateTime;
    }

    void CpuCollector::collect() {
        FILETIME idle, kernel, user;
        if (!GetSystemTimes(&idle, &kernel, &user)) return;

        ULONGLONG i = fileTimeToInt64(idle);
        ULONGLONG k = fileTimeToInt64(kernel);
        ULONGLONG u = fileTimeToInt64(user);

        if (m_lastIdle == 0) {
            m_lastIdle = i; m_lastKernel = k; m_lastUser = u;
            m_usagePercent = 0.0;
            return;
        }

        ULONGLONG idleDiff = i - m_lastIdle;
        ULONGLONG kernelDiff = k - m_lastKernel;
        ULONGLONG userDiff = u - m_lastUser;
        ULONGLONG totalDiff = kernelDiff + userDiff;

        m_lastIdle = i; m_lastKernel = k; m_lastUser = u;

        if (totalDiff > 0) {
            m_usagePercent = 100.0 * (1.0 - (double)idleDiff / totalDiff);
        } else {
            m_usagePercent = 0.0;
        }
    }

    std::string CpuCollector::getFormattedOutput() const {
        std::ostringstream oss;
        oss << std::fixed << std::setprecision(2);
        oss << "CPU: " << std::setw(5) << m_usagePercent << "%";
        return oss.str();
    }
}