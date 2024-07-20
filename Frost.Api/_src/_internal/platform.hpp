#pragma once
#include "../../macro.hpp"

#if defined(TARGET_BUILD_PLATFORM_WINDOWS)
	#define WIN32_LEAN_AND_MEAN
	#include "windows.h"
	#include <d3d12.h>
	#include <dxgi1_6.h>
	#pragma comment(lib, "d3d12")
	#pragma comment(lib, "dxgi")
#else
	static_assert("PLATFORM NOT SUPPORTED!" == nullptr);
#endif