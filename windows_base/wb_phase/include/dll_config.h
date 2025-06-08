#pragma once

#define BUILDING_WB_PHASE_DLL
#ifdef BUILDING_WB_PHASE_DLL
#define WB_PHASE_API __declspec(dllexport)
#else
#define WB_PHASE_API __declspec(dllimport)
#endif