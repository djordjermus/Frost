#pragma once

//
// PLATFORM SWITCH
//
#if defined(_WIN32) || defined(_WIN64)
	#define TARGET_BUILD_PLATFORM_WINDOWS
#endif

//
// STATIC_CLASS
//
#define STATIC_CLASS(CLASS)\
CLASS() = delete;\
CLASS(CLASS&) = delete;\
CLASS(CLASS&&) = delete;\
~CLASS() = delete;
