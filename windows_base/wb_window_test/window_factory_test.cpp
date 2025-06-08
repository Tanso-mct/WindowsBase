#include "pch.h"

#include "wb_window/include/window_factory.h"
#pragma comment(lib, "wb_window.lib")

TEST(WindowFacadeFactory, MainWindowFacade)
{
    std::unique_ptr<wb::IWindowFacadeFactory> factory = std::make_unique<wb::MainWindowFacadeFactory>();
    std::unique_ptr<wb::IWindowFacade> window = factory->Create();
    EXPECT_NE(window.get(), nullptr);
}

TEST(WindowEventFactory, MainWindowEvent)
{
    std::unique_ptr<wb::IWindowEventFactory> factory = std::make_unique<wb::MainWindowEventFactory>();
    std::unique_ptr<wb::IWindowEvent> windowEvent = factory->Create();
    EXPECT_NE(windowEvent.get(), nullptr);
}