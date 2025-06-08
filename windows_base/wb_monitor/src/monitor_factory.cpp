#include "wb_monitor/src/pch.h"
#include "wb_monitor/include/monitor_factory.h"

#include "wb_monitor/include/keyboard_monitor.h"
#include "wb_monitor/include/keyboard_context.h"
#include "wb_monitor/include/key_code_table.h"

#include "wb_monitor/include/mouse_monitor.h"
#include "wb_monitor/include/mouse_context.h"
#include "wb_monitor/include/mouse_code_table.h"
#include "wb_monitor/include/mouse_input_type_table.h"

#include "wb_utility/include/console_log.h"
#include "wb_utility/include/error_handler.h"
#pragma comment(lib, "wb_utility.lib")

std::unique_ptr<wb::IMonitor> wb::KeyboardMonitorFactory::Create()
{
    std::unique_ptr<wb::IKeyboardMonitor> monitor = std::make_unique<wb::KeyboardMonitor>();

    {
        std::unique_ptr<wb::IKeyboardContext> context = std::make_unique<wb::KeyboardContext>();
        monitor->SetUpContext(std::move(context));
    }

    {
        std::unique_ptr<wb::IKeyCodeTable> keyCodeTable = std::make_unique<wb::KeyCodeTable>();
        monitor->SetUpCodeTable(std::move(keyCodeTable));
    }

    if (!monitor->IsSetUp())
    {
        std::string err = wb::ConsoleLogErr
        (
            __FILE__, __LINE__, __FUNCTION__,
            {"KeyboardMonitorのセットアップが完了していません。"}
        );
        wb::ErrorNotify("MONITOR_FACTORy", err);
        wb::QuitProgram();
        return nullptr;
    }

    return monitor;
}

std::unique_ptr<wb::IMonitor> wb::MouseMonitorFactory::Create()
{
    std::unique_ptr<wb::IMouseMonitor> monitor = std::make_unique<wb::MouseMonitor>();

    {
        std::unique_ptr<wb::IMouseContext> context = std::make_unique<wb::MouseContext>();
        monitor->SetUpContext(std::move(context));
    }

    {
        std::unique_ptr<wb::IMouseCodeTable> mouseCodeTable = std::make_unique<wb::MouseCodeTable>();
        monitor->SetUpCodeTable(std::move(mouseCodeTable));
    }

    {
        std::unique_ptr<wb::IInputTypeTable> inputTypeTable = std::make_unique<wb::MouseInputTypeTable>();
        monitor->SetUpInputTypeTable(std::move(inputTypeTable));
    }

    if (!monitor->IsSetUp())
    {
        std::string err = wb::ConsoleLogErr
        (
            __FILE__, __LINE__, __FUNCTION__,
            {"MouseMonitorのセットアップが完了していません。"}
        );
        wb::ErrorNotify("MONITOR_FACTORY", err);
        wb::QuitProgram();
        return nullptr;
    }

    return monitor;
}

