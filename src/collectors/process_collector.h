#pragma once
#include "collectors/i_collector.h"
#include <vector>
#include <string>

namespace WinPulse::Collectors {

    // 内部使用的简单结构体，用于排序
    struct ProcessInfo {
        std::string name;
        size_t memoryUsageBytes;
    };

    class ProcessCollector : public ICollector {
    public:
        // 使用topN 参数决定监控前几名，后续优化可以传入参数
        explicit ProcessCollector(int topN = 5);

        [[nodiscard]] std::string getName() const override { return "TopProcess"; }
        void collect() override;
        [[nodiscard]] std::string getFormattedOutput() const override;

    private:
        int m_topN;
        std::vector<ProcessInfo> m_topProcesses;

        // 辅助函数：将宽字符(wstring)转换为普通字符串(string)
        // Windows API 返回的是 wchar_t，我们需要转换才能打印
        std::string wideToString(const std::wstring& wstr) const;
    };

}