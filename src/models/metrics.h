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

} // namespace WinPulse::Models