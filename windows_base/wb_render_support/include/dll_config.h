#pragma once

#define BUILDING_WB_RENDER_SUPPORT_DLL
#ifdef BUILDING_WB_RENDER_SUPPORT_DLL
#define WB_RENDER_SUPPORT_API __declspec(dllexport)
#else
#define WB_RENDER_SUPPORT_API __declspec(dllimport)
#endif