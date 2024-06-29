#include "../primitives.hpp"
#include "../enum/keycode.hpp"
#include "point.hpp"
#pragma once
namespace frost::api
{
	struct object;
	struct window_event final
	{
	public:
		enum class event_type : u64;

		event_type type;
		object* target;

		union
		{
			struct { i32 x; i32 y; } position;
			struct { i32 width; i32 height; } resize;
			struct { i32 x; i32 y; keycode key; } double_click;
			struct { point2d<i32> position; } cursor_enter;
			struct { point2d<i32> position; } cursor_move;
			struct { point2d<i32> last_position; } cursor_leave;
			struct { point2d<i32> delta; } mouse_move;
			struct { keycode key; const wchar_t* text; bool repeat; } key_down;
			struct { keycode key; const wchar_t* text; } key_up;
			struct { point2d<i32> delta; } mouse_scroll;
		};

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
	};
}
