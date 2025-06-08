#pragma once

#define BUILDING_WB_UTILITY_DLL
#ifdef BUILDING_WB_UTILITY_DLL
#define WB_UTILITY_API __declspec(dllexport)
#else
#define WB_UTILITY_API __declspec(dllimport)
#endif