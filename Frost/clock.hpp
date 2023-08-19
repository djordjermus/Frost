#include "Frost.Api/primitives.hpp"
#pragma once
namespace frost
{
	class clock final
	{
		clock() = delete;
		~clock() = delete;

	public:
		static u64 frequency();
		static f64 period();
		static u64 timestamp();
	};
}