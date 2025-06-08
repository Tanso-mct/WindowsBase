#include "wb_monitor/src/pch.h"
#include "wb_monitor/include/monitor_collection.h"

#include "wb_monitor/include/monitor_factory.h"

#include "wb_utility/include/console_log.h"
#include "wb_utility/include/error_handler.h"
#pragma comment(lib, "wb_utility.lib")

wb::MonitorCollection::MonitorCollection()
{
    // 各種モニターファクトリを登録
    monitorFactories_[MonitorID::MainWindowKeyboard] = std::make_unique<wb::KeyboardMonitorFactory>();
    monitorFactories_[MonitorID::MainWindowMouse] = std::make_unique<wb::MouseMonitorFactory>();
}

std::unique_ptr<wb::IMonitorFactory> &wb::MonitorCollection::GetFactory(MonitorID monitorID)
{
    if (monitorFactories_.find(monitorID) == monitorFactories_.end())
    {
        std::string err = wb::ConsoleLogErr
        (
            __FILE__, __LINE__, __FUNCTION__,
            {"指定されたMonitorIDのファクトリが存在しません。"}
        );
        wb::ErrorNotify("MONITOR_COLLECTION", err);
        wb::QuitProgram();
        return empty_;
    }

    return monitorFactories_[monitorID];
}