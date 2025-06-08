#include "pch.h"

#include "wb_utility/include/window_helpers.h"
#pragma comment(lib, "wb_utility.lib")

#include "wb_utility_test/window_proc.h"

TEST(WindowHelpers, CreateAndShowWindow)
{
    HWND hWnd = nullptr;
    WNDCLASSEX wc = { sizeof(WNDCLASSEX) };
    wc.lpszClassName = L"CreateAndShowWindow";
    wc.hInstance = GetModuleHandle(nullptr);
    wc.lpfnWndProc = WindowProc;
    
    wb::CreateWindowWB(hWnd, WS_OVERLAPPEDWINDOW | WS_VISIBLE, L"CreateAndShowWindow", 100, 100, 800, 600, nullptr, wc);
    
    EXPECT_NE(hWnd, nullptr);
    EXPECT_TRUE(IsWindow(hWnd));
    
    ShowWindow(hWnd, SW_SHOW);
    EXPECT_TRUE(IsWindowVisible(hWnd));

    // Windowを破棄
    DestroyWindow(hWnd);
}

TEST(WindowHelpers, GetCurrentScreenSize)
{
    HWND hWnd = nullptr;
    WNDCLASSEX wc = { sizeof(WNDCLASSEX) };
    wc.lpszClassName = L"GetCurrentScreenSize";
    wc.hInstance = GetModuleHandle(nullptr);
    wc.lpfnWndProc = WindowProc;
    
    wb::CreateWindowWB(hWnd, WS_OVERLAPPEDWINDOW | WS_VISIBLE, L"GetCurrentScreenSize", 100, 100, 800, 600, nullptr, wc);
    
    EXPECT_NE(hWnd, nullptr);
    EXPECT_TRUE(IsWindow(hWnd));
    
    ShowWindow(hWnd, SW_SHOW);
    EXPECT_TRUE(IsWindowVisible(hWnd));

    UINT width = 0;
    UINT height = 0;
    
    wb::GetCurrentScreenSize(hWnd, width, height);
    std::cout << "Current Screen Size: " << width << "x" << height << std::endl;

    EXPECT_NE(width, 0);
    EXPECT_NE(height, 0);

    // Windowを破棄
    DestroyWindow(hWnd);
}

TEST(WindowHelpers, CreateStandardConsole)
{
    wb::CreateStandardConsole();
    
    // 標準出力にメッセージを出力
    std::cout << "Standard console created successfully." << std::endl;
}