#pragma once

#include "wb_utility/include/dll_config.h"

#include <Windows.h>

namespace wb
{
    WB_UTILITY_API void CreateWindowWB
    (
        HWND& hWnd, DWORD windowStyle, LPCWSTR windowName, 
        const UINT& posX, const UINT& posY, const UINT& width, const UINT& height,
        HWND hWndParent, WNDCLASSEX& wc
    );

    WB_UTILITY_API void ShowWindowWB(HWND& hWnd);

    WB_UTILITY_API void GetCurrentScreenSize(HWND hWnd, UINT& width, UINT& height);

    WB_UTILITY_API void CreateStandardConsole();
}