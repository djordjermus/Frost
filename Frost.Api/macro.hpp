#pragma once
#if defined(FROST_API_BUILD_DLL)
#define FROST_API __declspec(dllexport)
#else
#define FROST_API __declspec(dllimport)
#endif

//
// PLATFORM SWITCH
//
#if defined(_WIN32) || defined(_WIN64)
#define TARGET_BUILD_PLATFORM_WINDOWS
#endif


/*
 * UTILITY
 */

#define STATIC_CLASS(CLASS)\
CLASS() = delete;\
CLASS(CLASS&) = delete;\
CLASS(CLASS&&) = delete;\
~CLASS() = delete;
