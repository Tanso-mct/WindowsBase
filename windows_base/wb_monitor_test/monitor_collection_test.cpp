#include "pch.h"

#include "wb_monitor/include/monitor_collection.h"
#pragma comment(lib, "wb_monitor.lib")

TEST(MonitorCollection, GetFactory)
{
    wb::MonitorCollection monitorCollection;

    // キーボードモニターファクトリの取得
    std::unique_ptr<wb::IMonitorFactory> &keyboardFactory = monitorCollection.GetFactory(wb::MonitorID::MainWindowKeyboard);
    EXPECT_NE(keyboardFactory, nullptr);

    // マウスモニターファクトリの取得
    std::unique_ptr<wb::IMonitorFactory> &mouseFactory = monitorCollection.GetFactory(wb::MonitorID::MainWindowMouse);
    EXPECT_NE(mouseFactory, nullptr);
}