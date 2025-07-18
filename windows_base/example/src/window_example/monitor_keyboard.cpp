#include "example/src/pch.h"
#include "example/include/window_example/monitor_keyboard.h"

const size_t &example::ExampleKeyboardMonitorID()
{
    static size_t id = wb::IDFactory::CreateMonitorID();
    return id;
}

namespace example
{
    WB_REGISTER_MONITOR(ExampleKeyboardMonitorID, wb::DefaultKeyboardMonitorFactoryID());
}