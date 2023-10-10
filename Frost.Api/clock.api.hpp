#include "primitives.hpp"
#include "macro.hpp"
#pragma once
namespace frost::api
{
	class clock final
	{
	public:
		STATIC_CLASS(clock);

		static u64 FROST_API get_frequency();
		static f64 FROST_API get_period();
		static u64 FROST_API get_timestamp();
	};
}