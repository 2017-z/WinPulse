#include "utils/time_utils.h"
#include <chrono>
#include <sstream>
#include <iomanip>

namespace WinPulse::Utils {
    std::string GetCurrentTimestamp() {
        auto now = std::chrono::system_clock::now();
        auto time = std::chrono::system_clock::to_time_t(now);
        std::tm tm{};
        localtime_s(&tm, &time);
        
        std::ostringstream oss;
        oss << std::put_time(&tm, "%Y-%m-%d %H:%M:%S");
        return oss.str();
    }
} // namespace WinPulse::Utils