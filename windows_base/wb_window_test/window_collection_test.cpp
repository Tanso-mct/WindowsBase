#include "pch.h"

#include "wb_window/include/window_collection.h"
#pragma comment(lib, "wb_window.lib")

TEST(WindowFacadeCollection, GetFactory)
{
    wb::WindowFacadeCollection collection;
    std::unique_ptr<wb::IWindowFacadeFactory> &factory = collection.GetFactory(wb::WindowID::Main);
    EXPECT_NE(factory.get(), nullptr);
}

TEST(WindowEventCollection, GetFactory)
{
    wb::WindowEventCollection collection;
    std::unique_ptr<wb::IWindowEventFactory> &factory = collection.GetFactory(wb::WindowID::Main);
    EXPECT_NE(factory.get(), nullptr);
}