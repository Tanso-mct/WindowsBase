#include "pch.h"

#include "windows_base/include/windows_helper.h"
#pragma comment(lib, "windows_base.lib")

TEST(WindowsHelper, CreateAndShowWindow)
{
    HWND hWnd = nullptr;
    WNDCLASSEX wc = { sizeof(WNDCLASSEX) };
    wc.lpszClassName = L"TestWindowClass";
    wc.hInstance = GetModuleHandle(nullptr);
    wc.lpfnWndProc = DefWindowProc;
    wc.style = CS_HREDRAW | CS_VREDRAW;

    wb::CreateWindowWB(hWnd, WS_OVERLAPPEDWINDOW, L"Test Window", 100, 100, 800, 600, nullptr, wc);
    EXPECT_NE(hWnd, nullptr);

    wb::ShowWindowWB(hWnd);

    DestroyWindow(hWnd);
}

TEST(WindowsHelper, CreateStandardConsole)
{
    wb::CreateStandardConsole();
}

TEST(WindowsHelper, GetMonitorSize)
{
    HWND hWnd = GetDesktopWindow();
    UINT width = 0;
    UINT height = 0;
    wb::GetMonitorSize(hWnd, width, height);
    
    EXPECT_GT(width, 0);
    EXPECT_GT(height, 0);
}

TEST(WindowsHelper, LoadFileData)
{
    fpos_t size = 0;
    std::string_view filePath = "resources/sample.txt";
    std::unique_ptr<unsigned char []> data = wb::LoadFileData(filePath, size);

    EXPECT_NE(data, nullptr);
    EXPECT_GT(size, 0);
}

