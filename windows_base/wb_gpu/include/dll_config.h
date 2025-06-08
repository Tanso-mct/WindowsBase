#pragma once

#define BUILDING_WB_GPU_DLL
#ifdef BUILDING_WB_GPU_DLL
#define WB_GPU_API __declspec(dllexport)
#else
#define WB_GPU_API __declspec(dllimport)
#endif