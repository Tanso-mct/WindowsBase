#include "pch.h"

#include "wb_monitor/include/monitor_factory.h"
#pragma comment(lib, "wb_monitor.lib")

TEST(MonitorFactory, CreateKeyboardMonitor)
{
    wb::KeyboardMonitorFactory factory;
    std::unique_ptr<wb::IMonitor> monitor = factory.Create();

    EXPECT_NE(monitor, nullptr);
}

TEST(MonitorFactory, CreateMouseMonitor)
{
    wb::MouseMonitorFactory factory;
    std::unique_ptr<wb::IMonitor> monitor = factory.Create();

    EXPECT_NE(monitor, nullptr);
}