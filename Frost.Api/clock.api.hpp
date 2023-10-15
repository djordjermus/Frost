#include "primitives.hpp"
#include "macro.hpp"
#pragma once
namespace frost::api
{
	class FROST_API clock final
	{
	public:
		STATIC_CLASS(clock);

		static u64 _stdcall get_frequency();
		static f64 _stdcall get_period();
		static u64 _stdcall get_timestamp();
	};
}