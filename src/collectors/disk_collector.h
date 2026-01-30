#pragma once
#include "collectors/i_collector.h"
#include <windows.h>
#include <pdh.h>
#include <vector>
#include <string>

namespace WinPulse::Collectors {

    // 定义单个磁盘的上下文
    struct DiskUnit {
        std::string name;           // 磁盘名称，如 "0 C:"
        PDH_HCOUNTER hRead = NULL;  // 读计数器句柄
        PDH_HCOUNTER hWrite = NULL; // 写计数器句柄
        double readKBps = 0.0;      // 当前读速率
        double writeKBps = 0.0;     // 当前写速率
    };

    class DiskCollector : public ICollector {
    public:
        // DiskCollector();
        // ~DiskCollector() override;

        //增加了filter参数
        explicit DiskCollector(const std::string& filter = ""); 
        ~DiskCollector() override;

        [[nodiscard]] std::string getName() const override { return "Disk"; }
        void collect() override;
        [[nodiscard]] std::string getFormattedOutput() const override;

    private:
        PDH_HQUERY m_queryHandle = NULL;
        bool m_isInitialized = false;

        // 存储所有探测到的物理磁盘
        std::vector<DiskUnit> m_disks;

        void initPdh();
        
        // 新增辅助函数：利用通配符自动发现磁盘
        void discoverDisks();

        std::string m_filter;
    };
}