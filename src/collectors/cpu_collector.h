#pragma once
#include "collectors/i_collector.h"
#include <windows.h>

namespace WinPulse::Collectors {

    class CpuCollector : public ICollector {
    public:
        CpuCollector();
        
        [[nodiscard]] std::string getName() const override { return "CPU"; }
        void collect() override;
        [[nodiscard]] std::string getFormattedOutput() const override;

    private:
        double m_usagePercent = 0.0;
        
        // 记录上一时刻的时间片
        ULONGLONG m_lastIdle = 0;
        ULONGLONG m_lastKernel = 0;
        ULONGLONG m_lastUser = 0;

        // 辅助转换函数
        [[nodiscard]] ULONGLONG fileTimeToInt64(const FILETIME& ft) const;
    };

}