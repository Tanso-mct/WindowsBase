#pragma once

#define BUILDING_WB_GAME_OBJECT_DLL
#ifdef BUILDING_WB_GAME_OBJECT_DLL
#define WB_GAME_OBJECT_API __declspec(dllexport)
#else
#define WB_GAME_OBJECT_API __declspec(dllimport)
#endif