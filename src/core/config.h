#pragma once
#include <string>
#include <iostream>
#include <vector>

namespace WinPulse::Core {

    struct Settings {
        // 采样间隔 (毫秒)，默认 1000ms
        int intervalMs = 1000;
        
        // 自动停止时间 (秒)，0 代表无限运行
        int durationSec = 0;
        
        // Top N 进程数
        int topN = 5;
        
        // 磁盘过滤器 (空代表监控所有，输入 "C:" 则只监控 C 盘)
        std::string diskFilter = "";

        // TODO: 未来在这里添加 bool enableGpu;
    };

    class Config {
    public:
        // 解析命令行参数，返回是否继续运行 (如果是 --help 则返回 false)
        static bool parse(int argc, char* argv[], Settings& outSettings);
        
        static void printHelp();
    };
} // namespace WinPulse::Core