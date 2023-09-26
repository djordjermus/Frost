#include <math.h>
#include "../random.hpp"
#include "../clock.hpp"
namespace frost
{
	random::random(u64 seed) : _seed(seed) {}
	random::random() : _seed(clock::timestamp()) {}

	u64 random::next_u64()
	{
		return generate(&_seed);
	}
	u64 random::next_u64(u64 minimum, u64 range)
	{
		return generate_range(&_seed, minimum, range);
	}
	i64 random::next_i64()
	{
		return static_cast<i64>(generate(&_seed));
	}
	i64 random::next_i64(i64 minimum, u64 range)
	{
		return static_cast<i64>(generate_range(&_seed, static_cast<u64>(minimum), range));
	}
	f64 random::next_f64(f64 minimum, f64 range)
	{
		return generate_range_double(&_seed, minimum, range);
	}

	static constexpr u64 x = 12;
	static constexpr u64 y = 25;
	static constexpr u64 z = 27;
	static constexpr u64 m = 2685821657736338717ULL;

	u64 random::generate(u64* p_seed)
	{
		u64 ret = *p_seed;
		ret ^= ret << x;
		ret ^= ret >> y;
		ret ^= ret << z;
		*p_seed = ret * m;

		return ret;
	}
	u64 random::generate_range(u64* p_seed, u64 min, u64 range)
	{
		u64 ret = *p_seed;
		ret ^= ret << x;
		ret ^= ret >> y;
		ret ^= ret << z;
		*p_seed = ret * m;

		return min + ret % range;
	}
	f64 random::generate_range_double(u64* p_seed, f64 min, f64 range)
	{
		u64 ret = *p_seed;
		ret ^= ret << x;
		ret ^= ret >> y;
		ret ^= ret << z;
		*p_seed = ret * m;
		return min + fmod(ret * 0.0000000000000001, 1.0f) * range;
	}
}