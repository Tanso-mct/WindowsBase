#pragma once

#define BUILDING_WB_FILE_DLL
#ifdef BUILDING_WB_FILE_DLL
#define WB_FILE_API __declspec(dllexport)
#else
#define WB_FILE_API __declspec(dllimport)
#endif