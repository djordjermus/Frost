#include "../clock.hpp"

#if defined(TARGET_BUILD_PLATFORM_WINDOWS)
#include <Windows.h>

static inline u64 read_frequency();
static inline u64 read_timestamp();

namespace frost
{
	u64 clock::frequency()
	{
		static u64 result = read_frequency();
		return result;
	}
	f64 clock::period()
	{
		static f64 result = 1.0 / read_frequency();
		return result;
	}
	u64 clock::timestamp()
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