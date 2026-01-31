#pragma once
#include <string>
#include <vector>

namespace WinPulse::Core {

    struct SessionSummary {
        double avgCpu = 0.0;
        double maxCpu = 0.0;
        double avgMemPercent = 0.0;
        double maxMemPercent = 0.0;
        double totalReadMB = 0.0;
        double totalWriteMB = 0.0;
        int sampleCount = 0;
    };

    class LogAnalyzer {
    public:
        static void analyze(const std::string& logPath);

    private:
        static SessionSummary parseLog(const std::string& logPath);
        
        // 新增增了 logPath 参数，用于写入文件
        static void saveAndPrintReport(const SessionSummary& summary, const std::string& logPath);
    };
} // namespace WinPulse::Core