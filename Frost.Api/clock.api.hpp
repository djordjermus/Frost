#include "primitives.hpp"
#include "macro.hpp"
#pragma once
extern "C"
{
	FROST_API u64 _stdcall frost_api_clock_get_frequency();
	FROST_API f64 _stdcall frost_api_clock_get_period();
	FROST_API u64 _stdcall frost_api_clock_get_timestamp();
}
