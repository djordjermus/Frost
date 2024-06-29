#include "../primitives.hpp"
#pragma once
namespace frost::api
{
	enum class window_state : u8
	{
		invalid		= 0xFF,
		hidden		= 0x00,
		minimized	= 0x01,
		normal		= 0x02,
		maximized	= 0x03
	};
}
