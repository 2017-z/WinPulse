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

        // 注册采集器 (依赖注入)
        void addCollector(std::unique_ptr<Collectors::ICollector> collector);
        
        // 启动主循环
        void run();

    private:
        void initWorkspace();
        void tick();

        bool m_running;
        std::vector<std::unique_ptr<Collectors::ICollector>> m_collectors;
        
        std::ofstream m_logFile;
        const std::string m_logDir = "logs";
        std::string m_logPath;    
    };

}