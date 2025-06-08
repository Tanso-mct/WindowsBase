#pragma once

#define BUILDING_WB_ASSET_DLL
#ifdef BUILDING_WB_ASSET_DLL
#define WB_ASSET_API __declspec(dllexport)
#else
#define WB_ASSET_API __declspec(dllimport)
#endif