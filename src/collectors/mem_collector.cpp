#include "collectors/mem_collector.h"
#include <sstream>
#include <iomanip>
#include <psapi.h> // Ensure explicit include

namespace WinPulse::Collectors {

    void MemCollector::collect() {
        MEMORYSTATUSEX mem = { sizeof(mem) };
        if (GlobalMemoryStatusEx(&mem)) {
            m_data.usagePercent = static_cast<double>(mem.dwMemoryLoad);
            m_data.totalMB = mem.ullTotalPhys / (1024 * 1024);
            m_data.availMB = mem.ullAvailPhys / (1024 * 1024);
            m_data.usedMB = m_data.totalMB - m_data.availMB;
        }
    }

    std::string MemCollector::getFormattedOutput() const {
        std::ostringstream oss;
        oss << std::fixed << std::setprecision(2);
        oss << "Memory: " << std::setw(5) << m_data.usagePercent << "% "
            << "(" << m_data.usedMB << "/" << m_data.totalMB << " MB)";
        return oss.str();
    }
}