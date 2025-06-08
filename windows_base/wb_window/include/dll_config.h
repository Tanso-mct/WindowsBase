#pragma once

#define BUILDING_WB_WINDOW_DLL
#ifdef BUILDING_WB_WINDOW_DLL
#define WB_WINDOW_API __declspec(dllexport)
#else
#define WB_WINDOW_API __declspec(dllimport)
#endif