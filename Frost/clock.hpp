#include "macro.hpp"
#include "primitives.hpp"
#pragma once
namespace frost
{
	class clock final
	{
		clock() = delete;
		~clock() = delete;

	public:
		static u64 FROST_API frequency();
		static f64 FROST_API period();
		static u64 FROST_API timestamp();
	};
}