#include "macro.hpp"
#include "Frost.Api/primitives.hpp"
#pragma once
namespace frost
{
	class clock final
	{
	public:
		STATIC_CLASS(clock);

		static u64 get_frequency();
		static f64 get_period();
		static u64 get_timestamp();
	};
}