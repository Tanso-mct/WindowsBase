#pragma once

#define BUILDING_WB_MONITOR_DLL
#ifdef BUILDING_WB_MONITOR_DLL
#define WB_MONITOR_API __declspec(dllexport)
#else
#define WB_MONITOR_API __declspec(dllimport)
#endif