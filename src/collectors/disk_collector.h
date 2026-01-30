#pragma once
#include "collectors/i_collector.h"
#include "models/metrics.h"
#include <windows.h>
#include <pdh.h>
#include <pdhmsg.h>

namespace WinPulse::Collectors {

    class DiskCollector : public ICollector {
    public:
        DiskCollector();
        ~DiskCollector() override; // 需要析构函数释放 PDH 句柄

        [[nodiscard]] std::string getName() const override { return "Disk"; }
        void collect() override;
        [[nodiscard]] std::string getFormattedOutput() const override;

    private:
        Models::DiskMetric m_data{};

        // PDH 相关的句柄
        PDH_HQUERY m_queryHandle = NULL;
        PDH_HCOUNTER m_readCounter = NULL;
        PDH_HCOUNTER m_writeCounter = NULL;
        
        bool m_isInitialized = false;

        // 内部初始化函数
        void initPdh();
    };
} // namespace WinPulse::Collectors