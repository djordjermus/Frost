#include "primitives.hpp"
#include "macro.hpp"
#pragma once
extern "C"
{
	FROST_API u64 _stdcall frost_api_random_generate(u64* p_seed);
	FROST_API u64 _stdcall frost_api_random_generate_range(u64* p_seed, u64 min, u64 range);
	FROST_API f64 _stdcall frost_api_random_generate_range_double(u64* p_seed, f64 min, f64 range);
}
