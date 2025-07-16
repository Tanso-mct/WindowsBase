#pragma once

#ifdef BUILDING_WINDOWS_BASE_DLL
#define WINDOWS_BASE_API __declspec(dllexport)
#else
#define WINDOWS_BASE_API __declspec(dllimport)
#endif