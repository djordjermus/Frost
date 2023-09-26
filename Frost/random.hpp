#include "macro.hpp"
#include "primitives.hpp"
#pragma once
namespace frost
{
	class random final
	{
		u64 _seed;

	public:

		FROST_API random(u64 seed);
		FROST_API random();

		u64 FROST_API next_u64();
		u64 FROST_API next_u64(u64 minimum, u64 range);
		i64 FROST_API next_i64();
		i64 FROST_API next_i64(i64 minimum, u64 range);
		f64 FROST_API next_f64(f64 minimum, f64 range);

		static u64 FROST_API random_generate(u64* p_seed);
		static u64 FROST_API random_generate_range(u64* p_seed, u64 min, u64 range);
		static f64 FROST_API random_generate_range_double(u64* p_seed, f64 min, f64 range);
	};
}