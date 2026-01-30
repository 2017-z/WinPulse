#pragma once
#include <string>

namespace WinPulse::Utils {
    // 获取当前时间戳字符串
    [[nodiscard]] std::string GetCurrentTimestamp();
    // 一个用于日志文件名的时间戳格式
    [[nodiscard]] std::string GetFileSafeTimestamp();
} // namespace WinPulse::Utils