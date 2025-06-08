#include "pch.h"

#include "wb_window/include/window_context.h"
#pragma comment(lib, "wb_window.lib")

#include <memory>

TEST(WindowContext, SetValue)
{
    std::unique_ptr<wb::IWindowContext> windowContext = std::make_unique<wb::WindowContext>();

    // Set values
    windowContext->Handle() = reinterpret_cast<HWND>(0x1234);
    windowContext->Instance() = GetModuleHandle(nullptr);
    windowContext->ParentHandle() = reinterpret_cast<HWND>(0x5678);

    windowContext->Name() = L"Test Window";
    windowContext->PosX() = CW_USEDEFAULT;
    windowContext->PosY() = CW_USEDEFAULT;
    windowContext->Width() = 800;
    windowContext->Height() = 600;
    windowContext->ClientWidth() = 780;
    windowContext->ClientHeight() = 580;

    windowContext->IsCreated() = true;
    windowContext->IsFocus() = true;
    windowContext->IsMaximized() = false;
    windowContext->IsMinimized() = false;

    // Verify values
    EXPECT_EQ(windowContext->Handle(), reinterpret_cast<HWND>(0x1234));
    EXPECT_EQ(windowContext->Instance(), GetModuleHandle(nullptr));
    EXPECT_EQ(windowContext->ParentHandle(), reinterpret_cast<HWND>(0x5678));

    EXPECT_EQ(windowContext->Name(), L"Test Window");
    EXPECT_EQ(windowContext->PosX(), CW_USEDEFAULT);
    EXPECT_EQ(windowContext->PosY(), CW_USEDEFAULT);
    EXPECT_EQ(windowContext->Width(), 800);
    EXPECT_EQ(windowContext->Height(), 600);
    EXPECT_EQ(windowContext->ClientWidth(), 780);
    EXPECT_EQ(windowContext->ClientHeight(), 580);

    EXPECT_TRUE(windowContext->IsCreated());
    EXPECT_TRUE(windowContext->IsFocus());
    EXPECT_FALSE(windowContext->IsMaximized());
    EXPECT_FALSE(windowContext->IsMinimized());
}
