#include "core/engine.h"
#include "utils/time_utils.h"
#include <iostream>
#include <filesystem>
#include <thread>
#include <chrono>

namespace WinPulse::Core {

    Engine::Engine() : m_running(true) {
        initWorkspace();
    }

    Engine::~Engine() {
        // RAII: 即使发生异常，析构函数也能保证文件流被关闭
        if (m_logFile.is_open()) {
            m_logFile << "=== Stopped " << Utils::GetCurrentTimestamp() << " ===" << std::endl;
        }
        // m_logFile 的析构函数会自动调用 close()
    }

    void Engine::initWorkspace() {
        if (!std::filesystem::exists(m_logDir)) {
            std::filesystem::create_directory(m_logDir);
        }

        // 1. 获取文件名安全的时间戳
        std::string timeStr = Utils::GetFileSafeTimestamp();
        
        // 2. 拼接完整路径: logs/winpulse_2026-01-30_22-10-00.log
        m_logPath = m_logDir + "/winpulse_" + timeStr + ".log";

        // 3. 打开文件
        m_logFile.open(m_logPath); // 默认就是覆盖模式，因为文件名是新的

        if (!m_logFile) {
            std::cerr << "Fatal Error: Failed to open log file: " << m_logPath << std::endl;
            exit(1);
        }

        m_logFile << "=== WinPulse Session Started at " << Utils::GetCurrentTimestamp() << " ===" << std::endl;
    }
    

    void Engine::addCollector(std::unique_ptr<Collectors::ICollector> collector) {
        m_collectors.push_back(std::move(collector));
    }

    void Engine::run() {
        std::cout << "=== WinPulse MVP v0.1 (Refactored) ===" << std::endl;
        std::cout << "Monitoring initialized..." << std::endl;
        std::cout << "Log: " << m_logPath << std::endl;
        std::cout << "Press Ctrl+C to stop\n" << std::endl;

        // 预热延时，让 CPU 计算有初始差值
        std::this_thread::sleep_for(std::chrono::milliseconds(100));

        while (m_running) {
            tick();
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    }

    void Engine::tick() {
        std::string logLine = "[" + Utils::GetCurrentTimestamp() + "] ";

        for (size_t i = 0; i < m_collectors.size(); ++i) {
            auto& c = m_collectors[i];
            
            c->collect(); // 1. 采集数据
            logLine += c->getFormattedOutput(); // 2. 获取格式化输出

            // 添加分隔符
            if (i < m_collectors.size() - 1) {
                logLine += " | ";
            }
        }

        // 双重输出：控制台 + 文件
        std::cout << logLine << std::endl;
        if (m_logFile.is_open()) {
            m_logFile << logLine << std::endl;
            m_logFile.flush();
        }
    }
}