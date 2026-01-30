#include "core/config.h"
#include <cstring>
#include <cstdlib> // for atoi

namespace WinPulse::Core {

    void Config::printHelp() {
        std::cout << "Usage: WinPulse.exe [options]\n"
                  << "Options:\n"
                  << "  -i, --interval <ms>   Sampling interval in milliseconds (Default: 1000)\n"
                  << "  -d, --duration <sec>  Run for N seconds then exit (Default: 0 = infinite)\n"
                  << "  -n, --top <num>       Number of top memory processes to show (Default: 5)\n"
                  << "  --disk <name>         Filter disk by name (e.g., 'C:', 'Total')\n"
                  << "  -h, --help            Show this help message\n"
                  << std::endl;
    }

    bool Config::parse(int argc, char* argv[], Settings& outSettings) {
        for (int i = 1; i < argc; ++i) {
            std::string arg = argv[i];

            if (arg == "-h" || arg == "--help") {
                printHelp();
                return false; // 指示调用者不需要继续运行程序
            }
            else if (arg == "-i" || arg == "--interval") {
                if (i + 1 < argc) {
                    outSettings.intervalMs = std::atoi(argv[++i]);
                    if (outSettings.intervalMs < 100) outSettings.intervalMs = 100; // 最小限制
                }
            }
            else if (arg == "-d" || arg == "--duration") {
                if (i + 1 < argc) {
                    outSettings.durationSec = std::atoi(argv[++i]);
                }
            }
            else if (arg == "-n" || arg == "--top") {
                if (i + 1 < argc) {
                    outSettings.topN = std::atoi(argv[++i]);
                }
            }
            else if (arg == "--disk") {
                if (i + 1 < argc) {
                    outSettings.diskFilter = argv[++i];
                }
            }
            // 未来扩展：
            // else if (arg == "--gpu") { outSettings.enableGpu = true; }
        }
        return true;
    }
}
//加入了一个轻量级的解析器，具有很强的可扩展性
//013026-z