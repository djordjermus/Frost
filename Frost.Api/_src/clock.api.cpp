#include "../clock.api.hpp"


#if defined(TARGET_BUILD_PLATFORM_WINDOWS)
#include <Windows.h>
static inline u64 query_frequency();
static u64 _cached_frequency = query_frequency();
static f64 _cached_period    = 1.0 / query_frequency();

FROST_API u64 _stdcall frost_api_clock_get_frequency()
{
	return _cached_frequency;
}

FROST_API f64 _stdcall frost_api_clock_get_period()
{
	return _cached_period;
}

FROST_API u64 _stdcall frost_api_clock_get_timestamp()
{
	LARGE_INTEGER output;
	::QueryPerformanceCounter(&output);
	return static_cast<u64>(output.QuadPart);
}



static inline u64 query_frequency()
{
	LARGE_INTEGER output;
	::QueryPerformanceFrequency(&output);
	return static_cast<u64>(output.QuadPart);

}
#else
static_assert("INVALID PLATFORM");
#endif