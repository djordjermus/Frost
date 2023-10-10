#include <math.h>
#include "Frost.Api/random.api.hpp"
#include "Frost.Api/clock.api.hpp"
#include "../random.hpp"
namespace frost
{
	random::random(u64 seed) : _seed(seed) {}
	random::random() : _seed(frost::api::clock::get_timestamp()) {}

	u64 random::next_u64()
	{
		return api::random::generate(&_seed);
	}
	u64 random::next_u64(u64 minimum, u64 range)
	{
		return api::random::generate_range(&_seed, minimum, range);
	}
	i64 random::next_i64()
	{
		return static_cast<i64>(api::random::generate(&_seed));
	}
	i64 random::next_i64(i64 minimum, u64 range)
	{
		return static_cast<i64>(api::random::generate_range(&_seed, static_cast<u64>(minimum), range));
	}
	f64 random::next_f64(f64 minimum, f64 range)
	{
		return api::random::generate_range_double(&_seed, minimum, range);
	}
}