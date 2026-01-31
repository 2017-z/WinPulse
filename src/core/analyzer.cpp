#include "core/analyzer.h"
#include <iostream>
#include <fstream>
#include <regex>
#include <iomanip>
#include <sstream> 
namespace WinPulse::Core {

    void LogAnalyzer::analyze(const std::string& logPath) {
        std::cout << "\nAnalyzing session log..." << std::endl;
        SessionSummary summary = parseLog(logPath);
        saveAndPrintReport(summary, logPath);
    }

    SessionSummary LogAnalyzer::parseLog(const std::string& logPath) {
        std::ifstream file(logPath);
        SessionSummary sum{};
        if (!file.is_open()) return sum;
        
        std::string line;
        std::regex cpuRegex(R"(CPU:\s+([\d\.]+)%)");
        std::regex memRegex(R"(Memory:\s+([\d\.]+)%)");
        std::regex diskRegex(R"(Disk\([^)]+\):\s+R\s+([\d\.]+)K\s+W\s+([\d\.]+)K)");

        while (std::getline(file, line)) {
            if (line.find("CPU:") == std::string::npos) continue;
            sum.sampleCount++;
            std::smatch match;

            if (std::regex_search(line, match, cpuRegex)) {
                double val = std::stod(match[1]);
                sum.avgCpu += val;
                if (val > sum.maxCpu) sum.maxCpu = val;
            }
            if (std::regex_search(line, match, memRegex)) {
                double val = std::stod(match[1]);
                sum.avgMemPercent += val;
                if (val > sum.maxMemPercent) sum.maxMemPercent = val;
            }
            auto diskBegin = std::sregex_iterator(line.begin(), line.end(), diskRegex);
            auto diskEnd = std::sregex_iterator();
            for (std::sregex_iterator i = diskBegin; i != diskEnd; ++i) {
                sum.totalReadMB += (std::stod((*i)[1]) / 1024.0);
                sum.totalWriteMB += (std::stod((*i)[2]) / 1024.0);
            }
        }
        if (sum.sampleCount > 0) {
            sum.avgCpu /= sum.sampleCount;
            sum.avgMemPercent /= sum.sampleCount;
        }
        return sum;
    }

    void LogAnalyzer::saveAndPrintReport(const SessionSummary& s, const std::string& logPath) {
        if (s.sampleCount == 0) {
            std::cout << "[Analyzer] No valid data found in log." << std::endl;
            return;
        }

        // 1. 使用 stringstream 构建报告内容
        std::stringstream ss;
        ss << "\n"; 
        ss << "============================================" << std::endl;
        ss << "         SESSION ANALYSIS REPORT            " << std::endl;
        ss << "============================================" << std::endl;
        ss << " Samples       : " << s.sampleCount << std::endl;
        ss << "--------------------------------------------" << std::endl;
        ss << " [CPU Usage]" << std::endl;
        ss << "   Average     : " << std::fixed << std::setprecision(2) << s.avgCpu << "%" << std::endl;
        ss << "   Peak        : " << s.maxCpu << "%" << std::endl;
        ss << "--------------------------------------------" << std::endl;
        ss << " [Memory Usage]" << std::endl;
        ss << "   Average     : " << s.avgMemPercent << "%" << std::endl;
        ss << "   Peak        : " << s.maxMemPercent << "%" << std::endl;
        ss << "--------------------------------------------" << std::endl;
        ss << " [Disk I/O Total]" << std::endl;
        ss << "   Total Read  : " << s.totalReadMB << " MB" << std::endl;
        ss << "   Total Write : " << s.totalWriteMB << " MB" << std::endl;
        ss << "============================================" << std::endl;

        std::string reportStr = ss.str();

        // 2. 输出到控制台
        std::cout << reportStr;

        // 3. 追加到日志文件 (Append Mode)
        // 使用 std::ios::app 模式打开，确保写在文件末尾
        std::ofstream logFile(logPath, std::ios::app);
        if (logFile.is_open()) {
            logFile << reportStr;
            // std::cout << "(Report appended to log file)" << std::endl;
        } else {
            std::cerr << "[Analyzer] Warning: Failed to append report to log file." << std::endl;
        }
    }
} // namespace WinPulse::Core