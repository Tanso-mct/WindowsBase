#pragma once

#include "wb_monitor/include/dll_config.h"

#include "wb_interface/monitor.h"

namespace wb
{
    class WB_MONITOR_API KeyboardMonitorFactory : public IMonitorFactory
    {
    public:
        virtual ~KeyboardMonitorFactory() = default;
        std::unique_ptr<IMonitor> Create() override;
    };

    class WB_MONITOR_API MouseMonitorFactory : public IMonitorFactory
    {
    public:
        virtual ~MouseMonitorFactory() = default;
        std::unique_ptr<IMonitor> Create() override;
    };
    
} // namespace wb