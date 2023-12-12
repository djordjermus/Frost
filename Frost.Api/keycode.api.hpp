#include "primitives.hpp"
#include "macro.hpp"
#pragma once
namespace frost::api
{
	enum class keycode : u8;
}

extern "C"
{
	FROST_API bool _stdcall keycode_to_wcs(
		frost::api::keycode keycode,
		wchar_t* output,
		u64 output_length,
		bool normalize_case);

	FROST_API const wchar_t* _stdcall keycode_get_name(frost::api::keycode keycode);
}

namespace frost::api
{
	enum class keycode : u8
	{
		null				= 0x00,

		mouse_left			= 0xe0,
		mouse_right			= 0xe1,
		mouse_middle		= 0xe2,
		mouse_x1			= 0xe3,
		mouse_x2			= 0xe4,

		escape				= 0x01,
		an_1				= 0x02,
		an_2				= 0x03,
		an_3				= 0x04,
		an_4				= 0x05,
		an_5				= 0x06,
		an_6				= 0x07,
		an_7				= 0x08,
		an_8				= 0x09,
		an_9				= 0x0a,
		an_0				= 0x0b,
		dash				= 0x0c,
		equals				= 0x0d,
		backspace			= 0x0e,

		tilde				= 0x29,
		q					= 0x10,
		w					= 0x11,
		e					= 0x12,
		r					= 0x13,
		t					= 0x14,
		y					= 0x15,
		u					= 0x16,
		i					= 0x17,
		o					= 0x18,
		p					= 0x19,
		bracket_open		= 0x1a,
		bracket_close		= 0x1b,
		a					= 0x1e,
		s					= 0x1f,
		d					= 0x20,
		f					= 0x21,
		g					= 0x22,
		h					= 0x23,
		j					= 0x24,
		k					= 0x25,
		l					= 0x26,
		semicolon			= 0x27,
		quote				= 0x28,
		z					= 0x2c,
		x					= 0x2d,
		c					= 0x2e,
		backslash			= 0x2b,
		v					= 0x2f,
		b					= 0x30,
		n					= 0x31,
		m					= 0x32,
		comma				= 0x33,
		period				= 0x34,
		slash				= 0x35,

		f1					= 0x3b,
		f2					= 0x3c,
		f3					= 0x3d,
		f4					= 0x3e,
		f5					= 0x3f,
		f6					= 0x40,
		f7					= 0x41,
		f8					= 0x42,
		f9					= 0x43,
		f10					= 0x44,
		f11					= 0x57,
		f12					= 0x58,

		tab					= 0x0F,
		capslock			= 0x3a,
		left_shift			= 0x2a,
		right_shift			= 0x36,
		left_ctrl			= 0x1d,
		right_ctrl			= 0x76,
		left_win			= 0xB4,
		right_win			= 0xB6,
		left_alt			= 0x38,
		right_alt			= 0x91,

		space				= 0x39,
		enter				= 0x1c,

		left_arrow			= 0xA4,
		up_arrow			= 0xA1,
		right_arrow			= 0xA6,
		down_arrow			= 0xA9,

		numpad_0			= 0x52,
		numpad_1			= 0x4f,
		numpad_2			= 0x50,
		numpad_3			= 0x51,
		numpad_4			= 0x4b,
		numpad_5			= 0x4c,
		numpad_6			= 0x4d,
		numpad_7			= 0x47,
		numpad_8			= 0x48,
		numpad_9			= 0x49,
		numpad_numlock		= 0x45,
		numpad_divide		= 0x8E,
		numpad_multiply		= 0x37,
		numpad_minus		= 0x4a,
		numpad_plus			= 0x4e,
		numpad_enter		= 0x75,
		numpad_separator	= 0x53,

		print_screen		= 0x83,
		scroll_lock			= 0x46,
		pause				= 0xD3,

		insert				= 0xAB,
		home				= 0xA0,
		page_up				= 0xA2,
		del					= 0xAC,
		end					= 0xA8,
		page_down			= 0xAA,
	};
}