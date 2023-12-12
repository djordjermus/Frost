#include "../keycode.api.hpp"
#include "../internal/sysmap.impl.hpp"
FROST_API bool _stdcall keycode_to_wcs(
	frost::api::keycode keycode,
	wchar_t* output,
	u64 output_length,
	bool normalize_case)
{
	return frost::impl::sysmap::keycode_to_wcs(keycode, output, output_length, normalize_case);
}

FROST_API const char* _stdcall keycode_get_name(frost::api::keycode keycode)
{
	switch (keycode)
	{
		case frost::api::keycode::null:				return "null";
		case frost::api::keycode::mouse_left:		return "mouse_left";
		case frost::api::keycode::mouse_right:		return "mouse_right";
		case frost::api::keycode::mouse_middle:		return "mouse_middle";
		case frost::api::keycode::mouse_x1:			return "mouse_x1";
		case frost::api::keycode::mouse_x2:			return "mouse_x2";
		case frost::api::keycode::escape:			return "escape";
		case frost::api::keycode::an_1:				return "an_1";
		case frost::api::keycode::an_2:				return "an_2";
		case frost::api::keycode::an_3:				return "an_3";
		case frost::api::keycode::an_4:				return "an_4";
		case frost::api::keycode::an_5:				return "an_5";
		case frost::api::keycode::an_6:				return "an_6";
		case frost::api::keycode::an_7:				return "an_7";
		case frost::api::keycode::an_8:				return "an_8";
		case frost::api::keycode::an_9:				return "an_9";
		case frost::api::keycode::an_0:				return "an_0";
		case frost::api::keycode::dash:				return "dash";
		case frost::api::keycode::equals:			return "equals";
		case frost::api::keycode::backspace:		return "backspace";
		case frost::api::keycode::tilde:			return "tilde";
		case frost::api::keycode::q:				return "q";
		case frost::api::keycode::w:				return "w";
		case frost::api::keycode::e:				return "e";
		case frost::api::keycode::r:				return "r";
		case frost::api::keycode::t:				return "t";
		case frost::api::keycode::y:				return "y";
		case frost::api::keycode::u:				return "u";
		case frost::api::keycode::i:				return "i";
		case frost::api::keycode::o:				return "o";
		case frost::api::keycode::p:				return "p";
		case frost::api::keycode::bracket_open:		return "bracket_open";
		case frost::api::keycode::bracket_close:	return "bracket_close";
		case frost::api::keycode::a:				return "a";
		case frost::api::keycode::s:				return "s";
		case frost::api::keycode::d:				return "d";
		case frost::api::keycode::f:				return "f";
		case frost::api::keycode::g:				return "g";
		case frost::api::keycode::h:				return "h";
		case frost::api::keycode::j:				return "j";
		case frost::api::keycode::k:				return "k";
		case frost::api::keycode::l:				return "l";
		case frost::api::keycode::semicolon:		return "semicolon";
		case frost::api::keycode::quote:			return "quote";
		case frost::api::keycode::z:				return "z";
		case frost::api::keycode::x:				return "x";
		case frost::api::keycode::c:				return "c";
		case frost::api::keycode::backslash:		return "backslash";
		case frost::api::keycode::v:				return "v";
		case frost::api::keycode::b:				return "b";
		case frost::api::keycode::n:				return "n";
		case frost::api::keycode::m:				return "m";
		case frost::api::keycode::comma:			return "comma";
		case frost::api::keycode::period:			return "period";
		case frost::api::keycode::slash:			return "slash";
		case frost::api::keycode::f1:				return "f1";
		case frost::api::keycode::f2:				return "f2";
		case frost::api::keycode::f3:				return "f3";
		case frost::api::keycode::f4:				return "f4";
		case frost::api::keycode::f5:				return "f5";
		case frost::api::keycode::f6:				return "f6";
		case frost::api::keycode::f7:				return "f7";
		case frost::api::keycode::f8:				return "f8";
		case frost::api::keycode::f9:				return "f9";
		case frost::api::keycode::f10:				return "f10";
		case frost::api::keycode::f11:				return "f11";
		case frost::api::keycode::f12:				return "f12";
		case frost::api::keycode::tab:				return "tab";
		case frost::api::keycode::capslock:			return "capslock";
		case frost::api::keycode::left_shift:		return "left_shift";
		case frost::api::keycode::right_shift:		return "right_shift";
		case frost::api::keycode::left_ctrl:		return "left_ctrl";
		case frost::api::keycode::right_ctrl:		return "right_ctrl";
		case frost::api::keycode::left_win:			return "left_win";
		case frost::api::keycode::right_win:		return "right_win";
		case frost::api::keycode::left_alt:			return "left_alt";
		case frost::api::keycode::right_alt:		return "right_alt";
		case frost::api::keycode::space:			return "space";
		case frost::api::keycode::enter:			return "enter";
		case frost::api::keycode::left_arrow:		return "left_arrow";
		case frost::api::keycode::up_arrow:			return "up_arrow";
		case frost::api::keycode::right_arrow:		return "right_arrow";
		case frost::api::keycode::down_arrow:		return "down_arrow";
		case frost::api::keycode::numpad_0:			return "numpad_0";
		case frost::api::keycode::numpad_1:			return "numpad_1";
		case frost::api::keycode::numpad_2:			return "numpad_2";
		case frost::api::keycode::numpad_3:			return "numpad_3";
		case frost::api::keycode::numpad_4:			return "numpad_4";
		case frost::api::keycode::numpad_5:			return "numpad_5";
		case frost::api::keycode::numpad_6:			return "numpad_6";
		case frost::api::keycode::numpad_7:			return "numpad_7";
		case frost::api::keycode::numpad_8:			return "numpad_8";
		case frost::api::keycode::numpad_9:			return "numpad_9";
		case frost::api::keycode::numpad_numlock:	return "numpad_numlock";
		case frost::api::keycode::numpad_divide:	return "numpad_divide";
		case frost::api::keycode::numpad_multiply:	return "numpad_multiply";
		case frost::api::keycode::numpad_minus:		return "numpad_minus";
		case frost::api::keycode::numpad_plus:		return "numpad_plus";
		case frost::api::keycode::numpad_enter:		return "numpad_enter";
		case frost::api::keycode::numpad_separator:	return "numpad_separator";
		case frost::api::keycode::print_screen:		return "print_screen";
		case frost::api::keycode::scroll_lock:		return "scroll_lock";
		case frost::api::keycode::pause:			return "pause";
		case frost::api::keycode::insert:			return "insert";
		case frost::api::keycode::home:				return "home";
		case frost::api::keycode::page_up:			return "page_up";
		case frost::api::keycode::del:				return "del";
		case frost::api::keycode::end:				return "end";
		case frost::api::keycode::page_down:		return "page_down";
	}
	return "N/A";
}