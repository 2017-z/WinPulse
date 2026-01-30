#pragma once
#include "collectors/i_collector.h"
#include "models/metrics.h"
#include <windows.h>

namespace WinPulse::Collectors {

    class MemCollector : public ICollector {
    public:
        [[nodiscard]] std::string getName() const override { return "Memory"; }
        void collect() override;
        [[nodiscard]] std::string getFormattedOutput() const override;

    private:
        Models::MemMetric m_data{};
    };
} // namespace WinPulse::Collectors