#include "example/src/pch.h"
#include "example/include/window_example/monitor_mouse.h"

const size_t &example::ExampleMouseMonitorID()
{
    static size_t id = wb::IDFactory::CreateMonitorID();
    return id;
}

namespace example
{
    WB_REGISTER_MONITOR(ExampleMouseMonitorID, wb::DefaultMouseMonitorFactoryID());
}