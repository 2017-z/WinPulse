#include "core/engine.h"
#include "collectors/cpu_collector.h"
#include "collectors/mem_collector.h"
#include "collectors/process_collector.h"
#include "collectors/disk_collector.h"
#include <memory>

int main() {
    using namespace WinPulse;

    // 1. 创建引擎
    Core::Engine engine;

    // 2. 注入依赖 (Dependency Injection)
    engine.addCollector(std::make_unique<Collectors::CpuCollector>());
    engine.addCollector(std::make_unique<Collectors::MemCollector>());

    // 先试试输出前三名，观察一下日志形态
    engine.addCollector(std::make_unique<Collectors::ProcessCollector>(3));

    // 添加磁盘采集器
    engine.addCollector(std::make_unique<Collectors::DiskCollector>());

    // 3. 运行
    engine.run();

    return 0;
}