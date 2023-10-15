#include "primitives.hpp"
#include "macro.hpp"
#pragma once
namespace frost::api
{
	class FROST_API random final
	{
	public:
		STATIC_CLASS(random);

		static u64 _stdcall generate(u64* p_seed);
		static u64 _stdcall generate_range(u64* p_seed, u64 min, u64 range);
		static f64 _stdcall generate_range_double(u64* p_seed, f64 min, f64 range);
	};
}