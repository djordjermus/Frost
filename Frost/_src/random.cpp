#include "../random.hpp"
#include "Frost.Api/clock.api.hpp"
#include "Frost.Api/random.api.hpp"
namespace frost
{
	random::random(u64 seed) : _seed(seed) {}
	random::random() : _seed(clock_get_timestamp()) {}

	u64 random::next_u64()
	{
		return random_generate(&_seed);
	}
	u64 random::next_u64(u64 minimum, u64 range)
	{
		return random_generate_range(&_seed, minimum, range);
	}
	i64 random::next_i64()
	{
		return static_cast<i64>(random_generate(&_seed));
	}
	i64 random::next_i64(i64 minimum, u64 range)
	{
		return static_cast<i64>(random_generate_range(&_seed, static_cast<u64>(minimum), range));
	}
	f64 random::next_f64(f64 minimum, f64 range)
	{
		return random_generate_range_double(&_seed, minimum, range);
	}
}