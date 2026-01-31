#pragma once
#include <vector>
#include <memory>
#include <fstream>
#include "collectors/i_collector.h"

namespace WinPulse::Core {

    class Engine {
    public:
        Engine();
        ~Engine(); // RAII: 析构时自动关闭文件

        void configure(int intervalMs, int durationSec);

        // 注册采集器 (依赖注入)
        void addCollector(std::unique_ptr<Collectors::ICollector> collector);
        
        // 启动主循环
        void run();

        // 获取当前日志路径
        [[nodiscard]] std::string getLogPath() const { return m_logPath; }

    private:
        void initWorkspace();
        void tick();

        bool m_running;
        std::vector<std::unique_ptr<Collectors::ICollector>> m_collectors;
        
        std::ofstream m_logFile;
        const std::string m_logDir = "logs";
        std::string m_logPath;    

        int m_intervalMs = 1000;
        int m_durationSec = 0;
    };

}