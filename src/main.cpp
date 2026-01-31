#include "core/engine.h"
#include "core/config.h"
#include "collectors/cpu_collector.h"
#include "collectors/mem_collector.h"
#include "collectors/process_collector.h"
#include "collectors/disk_collector.h"
#include "core/analyzer.h"
#include <memory>
#include <iostream>
#include <windows.h>
#include <thread>
#include <chrono>


// 注册一个全局的Handlr，避免程序被ctrl+c强制kill时无法正常退出
static WinPulse::Core::Engine* g_engine = nullptr;

// 信号处理
BOOL WINAPI ConsoleHandler(DWORD signal) {
    if (signal == CTRL_C_EVENT || signal == CTRL_CLOSE_EVENT) {
        std::cout << "\n[Info] Stopping WinPulse safely..." << std::endl;
        if (g_engine) {
            g_engine->stop();
        }
        return TRUE;
    }
    return FALSE;
}
int main(int argc, char* argv[]) {
    using namespace WinPulse;


    // 注册信号处理
    if (!SetConsoleCtrlHandler(ConsoleHandler, TRUE)) {
        std::cerr << "Error: Could not set control handler" << std::endl;
        return 1;
    }
    // 1. 解析配置
    Core::Settings settings;
    if (!Core::Config::parse(argc, argv, settings)) {
        return 0; // 如果用户输了 --help，解析返回 false，直接退出
    }

    // 2. 初始化引擎
    Core::Engine engine;

    g_engine = &engine;// quanju指针
    engine.configure(settings.intervalMs, settings.durationSec);

    // 3. 组装采集器 (将配置参数注入)
    engine.addCollector(std::make_unique<Collectors::CpuCollector>());
    engine.addCollector(std::make_unique<Collectors::MemCollector>());
    
    // 注入 topN 参数
    engine.addCollector(std::make_unique<Collectors::ProcessCollector>(settings.topN));
    
    // 注入 磁盘过滤 参数
    engine.addCollector(std::make_unique<Collectors::DiskCollector>(settings.diskFilter));

    std::cout << ">>> Config: Interval=" << settings.intervalMs 
              << "ms, Top=" << settings.topN 
              << ", DiskFilter='" << settings.diskFilter << "'" << std::endl;

    // 4. 运行
    engine.run();

    if (!engine.getLogPath().empty()) {
        // 等待文件锁被释放，这是一个保险措施
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        Core::LogAnalyzer::analyze(engine.getLogPath());
    }

    g_engine = nullptr; // 清理全局指针

    std::cout << "\nPress Enter to exit..." << std::endl;
    std::cin.get();

    return 0;
}