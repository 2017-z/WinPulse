#include "core/engine.h"
#include "collectors/cpu_collector.h"
#include "collectors/mem_collector.h"
#include <memory>

int main() {
    using namespace WinPulse;

    // 1. 创建引擎
    Core::Engine engine;

    // 2. 注入依赖 (Dependency Injection)
    engine.addCollector(std::make_unique<Collectors::CpuCollector>());
    engine.addCollector(std::make_unique<Collectors::MemCollector>());

    // 3. 运行
    engine.run();

    return 0;
}