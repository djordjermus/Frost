#include "../clock.api.hpp"

#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
static inline u64 read_frequency();
static inline u64 read_timestamp();
extern "C" 
{
	u64 _stdcall clock_get_frequency()
	{
		static u64 result = read_frequency();
		return result;
	}
	f64 _stdcall clock_get_period()
	{
		static f64 result = 1.0 / read_frequency();
		return result;
	}
	u64 _stdcall clock_get_timestamp()
	{
		LARGE_INTEGER ret;
		::QueryPerformanceCounter(&ret);
		return static_cast<u64>(ret.QuadPart);
	}
}

static inline u64 read_frequency()
{
	LARGE_INTEGER ret;
	::QueryPerformanceFrequency(&ret);
	return static_cast<u64>(ret.QuadPart);
}

static inline u64 read_timestamp()
{
	LARGE_INTEGER ret;
	::QueryPerformanceCounter(&ret);
	return static_cast<u64>(ret.QuadPart);
}
#else
static_assert("PLATFORM NOT SUPPORTED!")
#endif