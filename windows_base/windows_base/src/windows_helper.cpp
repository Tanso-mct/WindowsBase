#include "windows_base/src/pch.h"
#include "windows_base/include/windows_helper.h"

#include "windows_base/include/console_log.h"
#include "windows_base/include/error_handler.h"

WINDOWS_BASE_API void wb::CreateWindowWB
(
    HWND &hWnd, DWORD windowStyle, LPCWSTR windowName, 
    const UINT &posX, const UINT &posY, const UINT &width, const UINT &height, 
    HWND hWndParent, WNDCLASSEX &wc
){
    if (!RegisterClassEx(&wc))
    {
        std::string err = wb::CreateErrorMessage
        (
            __FILE__, __LINE__, __FUNCTION__,
            {"Failed to register window class."}
        );

        wb::ConsoleLogErr(err);
        wb::ErrorNotify("WINDOWS_BASE", err);
        wb::ThrowRuntimeError(err);
    }

    hWnd = CreateWindow
    (
        wc.lpszClassName,
        windowName, windowStyle,
        posX,posY, width,height,
        hWndParent,
        nullptr,
        wc.hInstance,
        nullptr
    );

    if (!hWnd)
    {
        std::string err = wb::CreateErrorMessage
        (
            __FILE__, __LINE__, __FUNCTION__,
            {"Failed to create window."}
        );

        wb::ConsoleLogErr(err);
        wb::ErrorNotify("WINDOWS_BASE", err);
        wb::ThrowRuntimeError(err);
    }
}

WINDOWS_BASE_API void wb::ShowWindowWB(HWND &hWnd)
{
    if (!hWnd)
    {
        std::string err = wb::CreateErrorMessage
        (
            __FILE__, __LINE__, __FUNCTION__,
            {"Invalid window handle. Cannot show window."}
        );

        wb::ConsoleLogErr(err);
        wb::ErrorNotify("WINDOWS_BASE", err);
        wb::ThrowRuntimeError(err);
    }

    ShowWindow(hWnd, SW_SHOW);
    UpdateWindow(hWnd);
}

WINDOWS_BASE_API void wb::CreateStandardConsole()
{
    // If the console window already exists, do nothing
    if (GetConsoleWindow() != NULL) return;

    // Allocate a new console for the application
    AllocConsole();
    freopen_s(reinterpret_cast<FILE**>(stdout), "CONOUT$", "w", stdout);
    freopen_s(reinterpret_cast<FILE**>(stderr), "CONOUT$", "w", stderr);

    // Sync C++ streams with the console
    std::ios::sync_with_stdio(true);
    std::wcout.clear();
    std::cout.clear();
    std::wcerr.clear();
    std::cerr.clear();
    std::wclog.clear();
    std::clog.clear();

    // Get the standard output handle
    HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hStdout == INVALID_HANDLE_VALUE)
    {
        std::string err = wb::CreateErrorMessage
        (
            __FILE__, __LINE__, __FUNCTION__,
            {"Failed to get standard output handle."}
        );

        wb::ConsoleLogErr(err);
        wb::ErrorNotify("WINDOWS_BASE", err);
        wb::ThrowRuntimeError(err);
    }

    // Get the current console mode
    DWORD mode = 0;
    if (!GetConsoleMode(hStdout, &mode))
    {
        std::string err = wb::CreateErrorMessage
        (
            __FILE__, __LINE__, __FUNCTION__,
            {"Failed to get console mode."}
        );

        wb::ConsoleLogErr(err);
        wb::ErrorNotify("WINDOWS_BASE", err);
        wb::ThrowRuntimeError(err);
    }

    // ANSIシーケンス対応を有効化
    mode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    if (!SetConsoleMode(hStdout, mode))
    {
        std::string err = wb::CreateErrorMessage
        (
            __FILE__, __LINE__, __FUNCTION__,
            {"Failed to set console mode."}
        );

        wb::ConsoleLogErr(err);
        wb::ErrorNotify("WINDOWS_BASE", err);
        wb::ThrowRuntimeError(err);
    }
}

WINDOWS_BASE_API void wb::GetWindowSize(HWND hWnd, UINT &width, UINT &height)
{
    MONITORINFO mi = {0};
    HMONITOR hMonitor = MonitorFromWindow(hWnd, MONITOR_DEFAULTTONEAREST);
    mi.cbSize = sizeof(mi);

    if (GetMonitorInfo(hMonitor, &mi))
    {
        width = mi.rcMonitor.right - mi.rcMonitor.left;
        height = mi.rcMonitor.bottom - mi.rcMonitor.top;
    } 
    else 
    {
        std::string err = wb::CreateErrorMessage
        (
            __FILE__, __LINE__, __FUNCTION__,
            {"Failed to get monitor info."}
        );

        wb::ConsoleLogErr(err);
        wb::ErrorNotify("WINDOWS_BASE", err);
        wb::ThrowRuntimeError(err);
    }
}