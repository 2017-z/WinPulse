#include "core/engine.h"
#include "core/config.h"
#include "collectors/cpu_collector.h"
#include "collectors/mem_collector.h"
#include "collectors/process_collector.h"
#include "collectors/disk_collector.h"
#include <memory>
#include <iostream>

int main(int argc, char* argv[]) {
    using namespace WinPulse;

    // 1. 解析配置
    Core::Settings settings;
    if (!Core::Config::parse(argc, argv, settings)) {
        return 0; // 如果用户输了 --help，解析返回 false，直接退出
    }

    // 2. 初始化引擎
    Core::Engine engine;
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

    return 0;
}