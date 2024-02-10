#include <math.h>
#include "../random.api.hpp"

static constexpr u64 x = 12;
static constexpr u64 y = 25;
static constexpr u64 z = 27;
static constexpr u64 m = 2685821657736338717ULL;

FROST_API u64 _stdcall frost_api_random_generate(u64* p_seed)
{
	u64 ret = *p_seed;
	ret ^= ret << x;
	ret ^= ret >> y;
	ret ^= ret << z;
	*p_seed = ret * m;

	return ret;
}
FROST_API u64 _stdcall frost_api_random_generate_range(u64* p_seed, u64 min, u64 range)
{
	u64 ret = *p_seed;
	ret ^= ret << x;
	ret ^= ret >> y;
	ret ^= ret << z;
	*p_seed = ret * m;

	return min + ret % range;
}
FROST_API f64 _stdcall frost_api_random_generate_range_double(u64* p_seed, f64 min, f64 range)
{
	u64 ret = *p_seed;
	ret ^= ret << x;
	ret ^= ret >> y;
	ret ^= ret << z;
	*p_seed = ret * m;
	return min + fmod(ret * 0.0000000000000001, 1.0f) * range;
}