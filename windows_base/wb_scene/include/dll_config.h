#pragma once

#define BUILDING_WB_SCENE_DLL
#ifdef BUILDING_WB_SCENE_DLL
#define WB_SCENE_API __declspec(dllexport)
#else
#define WB_SCENE_API __declspec(dllimport)
#endif