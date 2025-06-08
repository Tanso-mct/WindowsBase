#pragma once

#include "wb_monitor/include/dll_config.h"

#include "wb_interface/monitor.h"

#include <unordered_map>
#include <memory>

namespace wb
{
    class WB_MONITOR_API MonitorCollection
    {
    private:
        std::unordered_map<MonitorID, std::unique_ptr<IMonitorFactory>> monitorFactories_;
        std::unique_ptr<IMonitorFactory> empty_ = nullptr;

    public:
        MonitorCollection();
        ~MonitorCollection() = default;

        std::unique_ptr<IMonitorFactory> &GetFactory(MonitorID monitorID);
    };


} // namespace wb