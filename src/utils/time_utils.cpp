#include "utils/time_utils.h"
#include <chrono>
#include <sstream>
#include <iomanip>

namespace WinPulse::Utils {
    
    // 内部复用逻辑，通过 pattern 区分格式
    static std::string getFormattedTime(const char* pattern) {
        auto now = std::chrono::system_clock::now();
        auto time = std::chrono::system_clock::to_time_t(now);
        std::tm tm{};
        localtime_s(&tm, &time);
        
        std::ostringstream oss;
        oss << std::put_time(&tm, pattern);
        return oss.str();
    }

    std::string GetCurrentTimestamp() {
        return getFormattedTime("%Y-%m-%d %H:%M:%S");
    }

    std::string GetFileSafeTimestamp() {
        // Windows 文件名不支持冒号，改用连字符
        return getFormattedTime("%Y-%m-%d_%H-%M-%S");
    }
} // namespace WinPulse::Utils