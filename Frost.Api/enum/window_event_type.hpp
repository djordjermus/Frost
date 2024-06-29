#include "../primitives.hpp"
#pragma once
namespace frost::api
{
	enum class event_type : u64
	{
		invalid			= 0x00,
			
		enable			= 0x01,
		disable			= 0x02,
			
		activate		= 0x03,
		deactivate		= 0x04,
			
		gain_focus		= 0x05,
		lose_focus		= 0x06,
			
		state_change	= 0x07,
			
		move			= 0x08,
		resize			= 0x09,
			
		key_down		= 0x0A,
		key_up			= 0x0B,
		double_click	= 0x0C,
							   
		cursor_enter	= 0x0D,
		cursor_move		= 0x0E,
		cursor_leave	= 0x0F,
		mouse_move		= 0x10,
		mouse_scroll	= 0x11,
			
		create			= 0x12,
		close			= 0x13,
		destroy			= 0x14,
	};
}
