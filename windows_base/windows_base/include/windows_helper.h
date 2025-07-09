#pragma once

#include "windows_base/include/dll_config.h"

#include <Windows.h>

namespace wb
{
    WINDOWS_BASE_API void CreateWindowWB
    (
        HWND& hWnd, DWORD windowStyle, LPCWSTR windowName, 
        const UINT& posX, const UINT& posY, const UINT& width, const UINT& height,
        HWND hWndParent, WNDCLASSEX& wc
    );
    WINDOWS_BASE_API void ShowWindowWB(HWND& hWnd);
    WINDOWS_BASE_API void CreateStandardConsole();

    WINDOWS_BASE_API void GetWindowSize(HWND hWnd, UINT& width, UINT& height);

}