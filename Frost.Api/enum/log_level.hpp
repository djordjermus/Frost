#include "../primitives.hpp"
#pragma once
namespace frost::api
{
	enum class log_level : u8
	{
		trace		= 1,
		debug		= 2,
		information	= 4,
		warning		= 8,
		error		= 16,
		fatal		= 32,
	};
}