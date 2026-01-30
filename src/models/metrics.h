#pragma once
#include <windows.h>

namespace WinPulse::Models {

    struct CpuMetric {
        double usagePercent;
    };

    struct MemMetric {
        double usagePercent;
        size_t totalMB;
        size_t usedMB;
        size_t availMB;
    };

    struct DiskMetric {
        double readKBps;  // 每秒读取 (KB)
        double writeKBps; // 每秒写入 (KB)
    };

} // namespace WinPulse::Models