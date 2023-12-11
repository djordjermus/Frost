#include "../sysmap.impl.hpp"
#if defined (TARGET_BUILD_PLATFORM_WINDOWS)
namespace frost::impl
{
	sysmap sysmap::_instance = sysmap();
	u8 sysmap::compose_syskey(u16 make_code, u16 flags)
	{
		u8 syskey = static_cast<u8>(make_code);
		if ((flags & RI_KEY_E0))
			syskey += 0x59;
		else if ((flags & RI_KEY_E1))
			syskey += 0xB6;
		return syskey;
	}
	u8 sysmap::keycode_to_syskey(api::keycode internal_keycode)
	{
		return _instance._keycode_map[(u64)internal_keycode].second;
	}
	api::keycode sysmap::syskey_to_keycode(u8 system_keycode)
	{
		return _instance._keycode_map[system_keycode].first;
	}

	api::keycode sysmap::system_button_event_to_internal_keycode(u16 event_value, bool* out_down)
	{
		for (u64 i = 0; i < 10; i++)
		{
			if ((event_value >> i) & 1)
			{
				*out_down = !(i % 2);
				return _instance._button_event_keycode_map[i];
			}
		}
		return api::keycode::null;
	}

	bool sysmap::keycode_to_wchar(api::keycode internal_keycode, wchar_t* output, u64 output_length)
	{
		u8 system_keycode = _instance._keycode_map[(u64)internal_keycode].second;
		return syskey_to_wchar(system_keycode, output, output_length);
	}
	bool sysmap::syskey_to_wchar(u8 system_keycode, wchar_t* output, u64 output_length)
	{
		HKL hkl = ::GetKeyboardLayout(::GetCurrentThreadId());
		u8 virtual_key = ::MapVirtualKeyExW(system_keycode, MAPVK_VSC_TO_VK, hkl);

		u8 keyboard_state[256] = {};
		if (::GetKeyboardState(keyboard_state) != TRUE) 
			return false;

		u16 make_code = 0;
		u16 flags = 0;
		decompose_syskey(system_keycode, &make_code, &flags);

		if (::ToUnicodeEx(virtual_key, compose_scancode(make_code, flags), keyboard_state, output, output_length, 0, hkl) <= 0)
			return true;
		else
			return false;
	}

	void sysmap::decompose_syskey(u8 syskey, u16* out_make_code, u16* out_flags)
	{
		if (syskey >= 0x75 && syskey <= 0xB6)
		{
			*out_make_code = syskey - 0x59;
			*out_flags = RI_KEY_E0;
		}
		else if (syskey == 0xD3)
		{
			*out_make_code = syskey - 0xB6;
			*out_flags = RI_KEY_E0;
		}
		else
		{
			*out_make_code = syskey;
			*out_flags = RI_KEY_MAKE;
		}
	}
	u16 sysmap::compose_scancode(u16 make_code, u16 flags)
	{
		u16 scan_code = make_code;
		scan_code |= (flags & RI_KEY_E0) ? 0xe000 : 0;
		scan_code |= (flags & RI_KEY_E1) ? 0xe100 : 0;
		return scan_code;
	}

	
	
	sysmap::sysmap() :
		_keycode_map(), _button_event_keycode_map()
	{
		_button_event_keycode_map[0] = api::keycode::mouse_left;
		_button_event_keycode_map[1] = api::keycode::mouse_left;
		_button_event_keycode_map[2] = api::keycode::mouse_right;
		_button_event_keycode_map[3] = api::keycode::mouse_right;
		_button_event_keycode_map[4] = api::keycode::mouse_middle;
		_button_event_keycode_map[5] = api::keycode::mouse_middle;
		_button_event_keycode_map[6] = api::keycode::mouse_x1;
		_button_event_keycode_map[7] = api::keycode::mouse_x1;
		_button_event_keycode_map[8] = api::keycode::mouse_x2;
		_button_event_keycode_map[9] = api::keycode::mouse_x2;

		_keycode_map[0x00]	= { api::keycode::null,				0x00 };
		_keycode_map[0x01]	= { api::keycode::escape,			0x01 };
		_keycode_map[0x02]	= { api::keycode::an_1,				0x02 };
		_keycode_map[0x03]	= { api::keycode::an_2,				0x03 };
		_keycode_map[0x04]	= { api::keycode::an_3,				0x04 };
		_keycode_map[0x05]	= { api::keycode::an_4,				0x05 };
		_keycode_map[0x06]	= { api::keycode::an_5,				0x06 };
		_keycode_map[0x07]	= { api::keycode::an_6,				0x07 };
		_keycode_map[0x08]	= { api::keycode::an_7,				0x08 };
		_keycode_map[0x09]	= { api::keycode::an_8,				0x09 };
		_keycode_map[0x0A]	= { api::keycode::an_9,				0x0A };
		_keycode_map[0x0B]	= { api::keycode::an_0,				0x0B };
		_keycode_map[0x0C]	= { api::keycode::dash,				0x0C };
		_keycode_map[0x0D]	= { api::keycode::equals,			0x0D };
		_keycode_map[0x0E]	= { api::keycode::backspace,		0x0E };
		_keycode_map[0x0F]	= { api::keycode::tab,				0x0F };
		_keycode_map[0x10]	= { api::keycode::q,				0x10 };
		_keycode_map[0x11]	= { api::keycode::w,				0x11 };
		_keycode_map[0x12]	= { api::keycode::e,				0x12 };
		_keycode_map[0x13]	= { api::keycode::r,				0x13 };
		_keycode_map[0x14]	= { api::keycode::t,				0x14 };
		_keycode_map[0x15]	= { api::keycode::y,				0x15 };
		_keycode_map[0x16]	= { api::keycode::u,				0x16 };
		_keycode_map[0x17]	= { api::keycode::i,				0x17 };
		_keycode_map[0x18]	= { api::keycode::o,				0x18 };
		_keycode_map[0x19]	= { api::keycode::p,				0x19 };
		_keycode_map[0x1A]	= { api::keycode::bracket_open,		0x1A };
		_keycode_map[0x1B]	= { api::keycode::bracket_close,	0x1B };
		_keycode_map[0x1C]	= { api::keycode::enter,			0x1C };
		_keycode_map[0x1D]	= { api::keycode::left_ctrl,		0x1D };
		_keycode_map[0x1E]	= { api::keycode::a,				0x1E };
		_keycode_map[0x1F]	= { api::keycode::s,				0x1F };
		_keycode_map[0x20]	= { api::keycode::d,				0x20 };
		_keycode_map[0x21]	= { api::keycode::f,				0x21 };
		_keycode_map[0x22]	= { api::keycode::g,				0x22 };
		_keycode_map[0x23]	= { api::keycode::h,				0x23 };
		_keycode_map[0x24]	= { api::keycode::j,				0x24 };
		_keycode_map[0x25]	= { api::keycode::k,				0x25 };
		_keycode_map[0x26]	= { api::keycode::l,				0x26 };
		_keycode_map[0x27]	= { api::keycode::semicolon,		0x27 };
		_keycode_map[0x28]	= { api::keycode::quote,			0x28 };
		_keycode_map[0x29]	= { api::keycode::tilde,			0x29 };
		_keycode_map[0x2A]	= { api::keycode::left_shift,		0x2A };
		_keycode_map[0x2B]	= { api::keycode::backslash,		0x2B };
		_keycode_map[0x2C]	= { api::keycode::z,				0x2C };
		_keycode_map[0x2D]	= { api::keycode::x,				0x2D };
		_keycode_map[0x2E]	= { api::keycode::c,				0x2E };
		_keycode_map[0x2F]	= { api::keycode::v,				0x2F };
		_keycode_map[0x30]	= { api::keycode::b,				0x30 };
		_keycode_map[0x31]	= { api::keycode::n,				0x31 };
		_keycode_map[0x32]	= { api::keycode::m,				0x32 };
		_keycode_map[0x33]	= { api::keycode::comma,			0x33 };
		_keycode_map[0x34]	= { api::keycode::period,			0x34 };
		_keycode_map[0x35]	= { api::keycode::slash,			0x35 };
		_keycode_map[0x36]	= { api::keycode::right_shift,		0x36 };
		_keycode_map[0x38]	= { api::keycode::left_alt,			0x38 };
		_keycode_map[0x39]	= { api::keycode::space,			0x39 };
		_keycode_map[0x3A]	= { api::keycode::capslock,			0x3A };
		_keycode_map[0x3B]	= { api::keycode::f1,				0x3B };
		_keycode_map[0x3C]	= { api::keycode::f2,				0x3C };
		_keycode_map[0x3D]	= { api::keycode::f3,				0x3D };
		_keycode_map[0x3E]	= { api::keycode::f4,				0x3E };
		_keycode_map[0x3F]	= { api::keycode::f5,				0x3F };
		_keycode_map[0x40]	= { api::keycode::f6,				0x40 };
		_keycode_map[0x41]	= { api::keycode::f7,				0x41 };
		_keycode_map[0x42]	= { api::keycode::f8,				0x42 };
		_keycode_map[0x43]	= { api::keycode::f9,				0x43 };
		_keycode_map[0x44]	= { api::keycode::f10,				0x44 };
		_keycode_map[0x46]	= { api::keycode::scroll_lock,		0x46 };
		_keycode_map[0x47]	= { api::keycode::numpad_7,			0x47 };
		_keycode_map[0x48]	= { api::keycode::numpad_8,			0x48 };
		_keycode_map[0x49]	= { api::keycode::numpad_9,			0x49 };
		_keycode_map[0x4A]	= { api::keycode::numpad_minus,		0x4A };
		_keycode_map[0x4B]	= { api::keycode::numpad_4,			0x4B };
		_keycode_map[0x4C]	= { api::keycode::numpad_5,			0x4C };
		_keycode_map[0x4D]	= { api::keycode::numpad_6,			0x4D };
		_keycode_map[0x4E]	= { api::keycode::numpad_plus,		0x4E };
		_keycode_map[0x4F]	= { api::keycode::numpad_1,			0x4F };
		_keycode_map[0x50]	= { api::keycode::numpad_2,			0x50 };
		_keycode_map[0x51]	= { api::keycode::numpad_3,			0x51 };
		_keycode_map[0x52]	= { api::keycode::numpad_0,			0x52 };
		_keycode_map[0x53]	= { api::keycode::numpad_separator, 0x53 };
		_keycode_map[0x57]	= { api::keycode::f11,				0x57 };
		_keycode_map[0x58]	= { api::keycode::f12,				0x58 };


		
		_keycode_map[0x75]	= { api::keycode::numpad_enter,		0x75 };
		_keycode_map[0x76]	= { api::keycode::right_ctrl,		0x76 };
		_keycode_map[0x83]	= { api::keycode::print_screen,		0x83 };
		_keycode_map[0x8E]	= { api::keycode::numpad_divide,	0x8E };
		_keycode_map[0x91]	= { api::keycode::right_alt,		0x91 };
		_keycode_map[0xA0]	= { api::keycode::home,				0xA0 };
		_keycode_map[0xA1]	= { api::keycode::up_arrow,			0xA1 };
		_keycode_map[0xA2]	= { api::keycode::page_up,			0xA2 };
		_keycode_map[0xA4]	= { api::keycode::left_arrow,		0xA4 };
		_keycode_map[0xA6]	= { api::keycode::right_arrow,		0xA6 };
		_keycode_map[0xA8]	= { api::keycode::end,				0xA8 };
		_keycode_map[0xA9]	= { api::keycode::down_arrow,		0xA9 };
		_keycode_map[0xAA]	= { api::keycode::page_down,		0xAA };
		_keycode_map[0xAC]	= { api::keycode::del,				0xAC };
		_keycode_map[0xAB]	= { api::keycode::insert,			0xAB };
		_keycode_map[0xB4]	= { api::keycode::left_win,			0xB4 };
		_keycode_map[0xB6]	= { api::keycode::right_win,		0xB6 };

		_keycode_map[0xD3]	= { api::keycode::pause,			0xD3 };
	}
}

#else
static_assert("PLATFORM NOT SUPPORTED")
#endif