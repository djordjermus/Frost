#pragma once
#if defined(FROST_API_BUILD_DLL)
#define FROST_API __declspec(dllexport) _stdcall
#else
#define FROST_API __declspec(dllimport) _stdcall
#endif

//
// PLATFORM SWITCH
//
#if defined(_WIN32) || defined(_WIN64)
	#define TARGET_BUILD_PLATFORM_WINDOWS
#endif
