#include "Frost.Api/primitives.hpp"
#pragma once
namespace frost
{
	class random final
	{
		u64 _seed;

	public:

		random(u64 seed);
		random();

		u64 next_u64();
		u64 next_u64(u64 minimum, u64 range);
		i64 next_i64();
		i64 next_i64(i64 minimum, u64 range);
		f64 next_f64(f64 minimum, f64 range);
	};
}