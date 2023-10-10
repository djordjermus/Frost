#include "primitives.hpp"
#include "macro.hpp"
#pragma once
namespace frost::api
{
	class random final
	{
	public:
		STATIC_CLASS(random);

		static u64 FROST_API generate(u64* p_seed);
		static u64 FROST_API generate_range(u64* p_seed, u64 min, u64 range);
		static f64 FROST_API generate_range_double(u64* p_seed, f64 min, f64 range);
	};
}