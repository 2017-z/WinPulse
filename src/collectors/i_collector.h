#pragma once
#include <string>

namespace WinPulse::Collectors {

    // 抽象基类 (Interface)
    class ICollector {
    public:
        virtual ~ICollector() = default;

        // 获取采集器名称
        [[nodiscard]] virtual std::string getName() const = 0;

        // 执行采集逻辑，更新内部状态
        virtual void collect() = 0;

        // 获取格式化后的日志字符串
        [[nodiscard]] virtual std::string getFormattedOutput() const = 0;
    };

} // namespace WinPulse::Collectors