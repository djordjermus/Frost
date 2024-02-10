#pragma once
#if defined(FROST_API_BUILD_DLL)
#define FROST_API __declspec(dllexport)
#else
#define FROST_API __declspec(dllimport)
#endif



/*
 * PLATFORM SWITCH
 */
#if defined(_WIN32) || defined(_WIN64)
#define TARGET_BUILD_PLATFORM_WINDOWS
#endif



/*
 * DEBUG
 */
#if _DEBUG
#define FROST_API_DEBUG_LOG_RESOURCE_CREATION 1
#define FROST_API_DEBUG_LOG_RESOURCE_DESTRUCTION 1
#endif



/*
 * UTILITY
 */

#define STATIC_CLASS(CLASS)\
CLASS() = delete;\
CLASS(CLASS&) = delete;\
CLASS(CLASS&&) noexcept = delete;\
CLASS(CLASS&&) noexcept = delete;\
~CLASS() = delete;

#define NO_COPY_SEMANTICS(CLASS)\
CLASS(CLASS&) = delete;\
CLASS& operator=(CLASS&) = delete;

#define NO_MOVE_SEMANTICS(CLASS)\
CLASS(CLASS&&) noexcept = delete;\
CLASS& operator=(CLASS&&) noexcept = delete;