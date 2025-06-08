#include "wb_window/src/pch.h"
#include "wb_window/include/window_collection.h"

#include "wb_window/include/window_factory.h"

#include "wb_utility/include/console_log.h"
#include "wb_utility/include/error_handler.h"
#pragma comment(lib, "wb_utility.lib")

wb::WindowFacadeCollection::WindowFacadeCollection()
{
    // 各種ウィンドウファサードファクトリを登録
    windowFacadeFactories_[WindowID::Main] = std::make_unique<wb::MainWindowFacadeFactory>();
}

std::unique_ptr<wb::IWindowFacadeFactory> &wb::WindowFacadeCollection::GetFactory(WindowID windowID)
{
    if (windowFacadeFactories_.find(windowID) == windowFacadeFactories_.end())
    {
        std::string err = wb::ConsoleLogErr
        (
            __FILE__, __LINE__, __FUNCTION__,
            {"指定されたWindowIDのファーサードファクトリが存在しません。"}
        );
        wb::ErrorNotify("WINDOW_COLLECTION", err);
        wb::QuitProgram();
        return empty_;
    }

    return windowFacadeFactories_[windowID];
}

wb::WindowEventCollection::WindowEventCollection()
{
    // 各種ウィンドウイベントファクトリを登録
    windowEventFactories_[WindowID::Main] = std::make_unique<wb::MainWindowEventFactory>();
}

std::unique_ptr<wb::IWindowEventFactory> &wb::WindowEventCollection::GetFactory(WindowID windowID)
{
    if (windowEventFactories_.find(windowID) == windowEventFactories_.end())
    {
        std::string err = wb::ConsoleLogErr
        (
            __FILE__, __LINE__, __FUNCTION__,
            {"指定されたWindowIDのイベントファクトリが存在しません。"}
        );
        wb::ErrorNotify("WINDOW_COLLECTION", err);
        wb::QuitProgram();
        return empty_;
    }

    return windowEventFactories_[windowID];
}
