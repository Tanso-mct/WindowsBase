#pragma once

#define BUILDING_WB_COMPONENT_DLL
#ifdef BUILDING_WB_COMPONENT_DLL
#define WB_COMPONENT_API __declspec(dllexport)
#else
#define WB_COMPONENT_API __declspec(dllimport)
#endif