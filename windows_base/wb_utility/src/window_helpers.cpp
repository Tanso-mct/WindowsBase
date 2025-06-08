#include "wb_utility/src/pch.h"
#include "wb_utility/include/window_helpers.h"

#include "wb_utility/include/console_log.h"
#include "wb_utility/include/error_handler.h"

WB_UTILITY_API void wb::CreateWindowWB
(
    HWND &hWnd, DWORD windowStyle, LPCWSTR windowName, 
    const UINT &posX, const UINT &posY, const UINT &width, const UINT &height, 
    HWND hWndParent, WNDCLASSEX &wc
){
    if (!RegisterClassEx(&wc))
    {
        std::string err = wb::ConsoleLogErr
        (
            __FILE__, __LINE__, __FUNCTION__,
            {"Windowクラスの登録に失敗しました。"}
        );
        wb::ErrorNotify("WINDOW_HELPERS", err);
        wb::QuitProgram();
    }

    hWnd = CreateWindow
    (
        wc.lpszClassName,
        windowName,
        windowStyle,
        posX,
        posY,
        width,
        height,
        hWndParent,
        nullptr,
        wc.hInstance,
        nullptr
    );

    if (!hWnd)
    {
        std::string err = wb::ConsoleLogErr
        (
            __FILE__, __LINE__, __FUNCTION__,
            {"ウィンドウの作成に失敗しました。"}
        );
        wb::ErrorNotify("WINDOW_HELPERS", err);
        wb::QuitProgram();
    }
}

WB_UTILITY_API void wb::ShowWindowWB(HWND &hWnd)
{
    if (!hWnd)
    {
        std::string err = wb::ConsoleLogErr
        (
            __FILE__, __LINE__, __FUNCTION__,
            {"ウィンドウハンドルが無効です。"}
        );
        wb::ErrorNotify("WINDOW_HELPERS", err);
        wb::QuitProgram();
    }

    ShowWindow(hWnd, SW_SHOW);
    UpdateWindow(hWnd);
}

WB_UTILITY_API void wb::GetCurrentScreenSize(HWND hWnd, UINT &width, UINT &height)
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
        std::string err = wb::ConsoleLogErr
        (
            __FILE__, __LINE__, __FUNCTION__,
            {"モニター情報の取得に失敗しました。"}
        );
        wb::ErrorNotify("WINDOW_HELPERS", err);
        wb::QuitProgram();
    }
}

WB_UTILITY_API void wb::CreateStandardConsole()
{
    // すでにコンソールが存在する場合は何もしない
    if (GetConsoleWindow() != NULL) return;

    // コンソールを割り当てる
    AllocConsole();
    freopen_s(reinterpret_cast<FILE**>(stdout), "CONOUT$", "w", stdout);
    freopen_s(reinterpret_cast<FILE**>(stderr), "CONOUT$", "w", stderr);

    // C++ストリームも同期
    std::ios::sync_with_stdio(true);
    std::wcout.clear();
    std::cout.clear();
    std::wcerr.clear();
    std::cerr.clear();
    std::wclog.clear();
    std::clog.clear();

    // 標準出力のハンドルを取得
    HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hStdout == INVALID_HANDLE_VALUE)
    {
        std::string err = wb::ConsoleLogErr
        (
            __FILE__, __LINE__, __FUNCTION__,
            {"標準出力のハンドルを取得できませんでした。"}
        );
        wb::ErrorNotify("INTEGRATION_TEST", err);
        wb::QuitProgram();
    }

    // 現在のモードを取得
    DWORD mode = 0;
    if (!GetConsoleMode(hStdout, &mode))
    {
        std::string err = wb::ConsoleLogErr
        (
            __FILE__, __LINE__, __FUNCTION__,
            {"GetConsoleModeの呼び出しに失敗しました。"}
        );
        wb::ErrorNotify("INTEGRATION_TEST", err);
        wb::QuitProgram();
    }

    // ANSIシーケンス対応を有効化
    mode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    if (!SetConsoleMode(hStdout, mode))
    {
        std::string err = wb::ConsoleLogErr
        (
            __FILE__, __LINE__, __FUNCTION__,
            {"SetConsoleModeの呼び出しに失敗しました。"}
        );
        wb::ErrorNotify("INTEGRATION_TEST", err);
        wb::QuitProgram();
    }
}