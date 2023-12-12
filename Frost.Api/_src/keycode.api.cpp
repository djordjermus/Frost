#include "../keycode.api.hpp"
#include "../internal/sysmap.impl.hpp"
FROST_API u64 _stdcall keycode_to_wcs(
	frost::api::keycode keycode,
	wchar_t* output,
	u64 output_length,
	bool normalize_case)
{
	return frost::impl::sysmap::keycode_to_wcs(keycode, output, output_length, normalize_case);
}

FROST_API const wchar_t* _stdcall keycode_get_name(frost::api::keycode keycode)
{
	switch (keycode)
	{
		case frost::api::keycode::null:				return L"null";
		case frost::api::keycode::mouse_left:		return L"mouse_left";
		case frost::api::keycode::mouse_right:		return L"mouse_right";
		case frost::api::keycode::mouse_middle:		return L"mouse_middle";
		case frost::api::keycode::mouse_x1:			return L"mouse_x1";
		case frost::api::keycode::mouse_x2:			return L"mouse_x2";
		case frost::api::keycode::escape:			return L"escape";
		case frost::api::keycode::an_1:				return L"an_1";
		case frost::api::keycode::an_2:				return L"an_2";
		case frost::api::keycode::an_3:				return L"an_3";
		case frost::api::keycode::an_4:				return L"an_4";
		case frost::api::keycode::an_5:				return L"an_5";
		case frost::api::keycode::an_6:				return L"an_6";
		case frost::api::keycode::an_7:				return L"an_7";
		case frost::api::keycode::an_8:				return L"an_8";
		case frost::api::keycode::an_9:				return L"an_9";
		case frost::api::keycode::an_0:				return L"an_0";
		case frost::api::keycode::dash:				return L"dash";
		case frost::api::keycode::equals:			return L"equals";
		case frost::api::keycode::backspace:		return L"backspace";
		case frost::api::keycode::tilde:			return L"tilde";
		case frost::api::keycode::q:				return L"q";
		case frost::api::keycode::w:				return L"w";
		case frost::api::keycode::e:				return L"e";
		case frost::api::keycode::r:				return L"r";
		case frost::api::keycode::t:				return L"t";
		case frost::api::keycode::y:				return L"y";
		case frost::api::keycode::u:				return L"u";
		case frost::api::keycode::i:				return L"i";
		case frost::api::keycode::o:				return L"o";
		case frost::api::keycode::p:				return L"p";
		case frost::api::keycode::bracket_open:		return L"bracket_open";
		case frost::api::keycode::bracket_close:	return L"bracket_close";
		case frost::api::keycode::a:				return L"a";
		case frost::api::keycode::s:				return L"s";
		case frost::api::keycode::d:				return L"d";
		case frost::api::keycode::f:				return L"f";
		case frost::api::keycode::g:				return L"g";
		case frost::api::keycode::h:				return L"h";
		case frost::api::keycode::j:				return L"j";
		case frost::api::keycode::k:				return L"k";
		case frost::api::keycode::l:				return L"l";
		case frost::api::keycode::semicolon:		return L"semicolon";
		case frost::api::keycode::quote:			return L"quote";
		case frost::api::keycode::z:				return L"z";
		case frost::api::keycode::x:				return L"x";
		case frost::api::keycode::c:				return L"c";
		case frost::api::keycode::backslash:		return L"backslash";
		case frost::api::keycode::v:				return L"v";
		case frost::api::keycode::b:				return L"b";
		case frost::api::keycode::n:				return L"n";
		case frost::api::keycode::m:				return L"m";
		case frost::api::keycode::comma:			return L"comma";
		case frost::api::keycode::period:			return L"period";
		case frost::api::keycode::slash:			return L"slash";
		case frost::api::keycode::f1:				return L"f1";
		case frost::api::keycode::f2:				return L"f2";
		case frost::api::keycode::f3:				return L"f3";
		case frost::api::keycode::f4:				return L"f4";
		case frost::api::keycode::f5:				return L"f5";
		case frost::api::keycode::f6:				return L"f6";
		case frost::api::keycode::f7:				return L"f7";
		case frost::api::keycode::f8:				return L"f8";
		case frost::api::keycode::f9:				return L"f9";
		case frost::api::keycode::f10:				return L"f10";
		case frost::api::keycode::f11:				return L"f11";
		case frost::api::keycode::f12:				return L"f12";
		case frost::api::keycode::tab:				return L"tab";
		case frost::api::keycode::capslock:			return L"capslock";
		case frost::api::keycode::left_shift:		return L"left_shift";
		case frost::api::keycode::right_shift:		return L"right_shift";
		case frost::api::keycode::left_ctrl:		return L"left_ctrl";
		case frost::api::keycode::right_ctrl:		return L"right_ctrl";
		case frost::api::keycode::left_win:			return L"left_win";
		case frost::api::keycode::right_win:		return L"right_win";
		case frost::api::keycode::left_alt:			return L"left_alt";
		case frost::api::keycode::right_alt:		return L"right_alt";
		case frost::api::keycode::space:			return L"space";
		case frost::api::keycode::enter:			return L"enter";
		case frost::api::keycode::left_arrow:		return L"left_arrow";
		case frost::api::keycode::up_arrow:			return L"up_arrow";
		case frost::api::keycode::right_arrow:		return L"right_arrow";
		case frost::api::keycode::down_arrow:		return L"down_arrow";
		case frost::api::keycode::numpad_0:			return L"numpad_0";
		case frost::api::keycode::numpad_1:			return L"numpad_1";
		case frost::api::keycode::numpad_2:			return L"numpad_2";
		case frost::api::keycode::numpad_3:			return L"numpad_3";
		case frost::api::keycode::numpad_4:			return L"numpad_4";
		case frost::api::keycode::numpad_5:			return L"numpad_5";
		case frost::api::keycode::numpad_6:			return L"numpad_6";
		case frost::api::keycode::numpad_7:			return L"numpad_7";
		case frost::api::keycode::numpad_8:			return L"numpad_8";
		case frost::api::keycode::numpad_9:			return L"numpad_9";
		case frost::api::keycode::numpad_numlock:	return L"numpad_numlock";
		case frost::api::keycode::numpad_divide:	return L"numpad_divide";
		case frost::api::keycode::numpad_multiply:	return L"numpad_multiply";
		case frost::api::keycode::numpad_minus:		return L"numpad_minus";
		case frost::api::keycode::numpad_plus:		return L"numpad_plus";
		case frost::api::keycode::numpad_enter:		return L"numpad_enter";
		case frost::api::keycode::numpad_separator:	return L"numpad_separator";
		case frost::api::keycode::print_screen:		return L"print_screen";
		case frost::api::keycode::scroll_lock:		return L"scroll_lock";
		case frost::api::keycode::pause:			return L"pause";
		case frost::api::keycode::insert:			return L"insert";
		case frost::api::keycode::home:				return L"home";
		case frost::api::keycode::page_up:			return L"page_up";
		case frost::api::keycode::del:				return L"del";
		case frost::api::keycode::end:				return L"end";
		case frost::api::keycode::page_down:		return L"page_down";
		default:									return L"N/A";
	}
}