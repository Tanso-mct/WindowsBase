#pragma once

#include <Windows.h>

static LRESULT CALLBACK WindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
    case WM_PAINT:
        // WM_PAINTでDefWindowProcを返すと更新が止まるので、WM_PAINTは0を返す。
        break;

    default:
        return DefWindowProc(hWnd, msg, wParam, lParam);
    }

    return 0;
}