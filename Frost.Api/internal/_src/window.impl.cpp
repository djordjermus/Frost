#include "../window.impl.hpp"
#include "../sysmap.impl.hpp"
// TODO: #include "../logging_messages.hpp"
#include "../../keycode.api.hpp"
#include "../../event_system.api.hpp"
#include "../../logging.api.hpp"
#if defined(TARGET_BUILD_PLATFORM_WINDOWS)
#include <winnt.h>
#include <WindowsX.h>
#include <hidusage.h>
#include <iostream>
static const wchar_t* err_window_creation_failed = L"Window creation failed! ";
static const u64 err_window_creation_failed_length = ::wcslen(err_window_creation_failed);
const wchar_t* kc_to_wstr(frost::api::keycode keycode);

WORD frost::impl::window::_window_class = frost::impl::window::get_window_atom();
static inline frost::api::window_state fold_state(frost::api::window_state state);
static inline int state_to_int(frost::api::window_state state);
static inline frost::api::window_state int_to_state(int state);
static inline frost::impl::window* get_hwnd_data(HWND hwnd) { return reinterpret_cast<frost::impl::window*>(::GetWindowLongPtrW(hwnd, GWLP_USERDATA)); }

frost::impl::window::~window()
{
	::DestroyWindow(_hwnd);
}

bool frost::impl::window::is_enabled() const { return (_flags & _flag_enabled) != 0; }
bool frost::impl::window::is_active() const { return (_flags & _flag_active) != 0; }
bool frost::impl::window::is_focused() const { return (_flags & _flag_focused) != 0; }

frost::api::window_state frost::impl::window::get_state() const { return _state; }

point2d<i32> frost::impl::window::get_position() const { return { _rect.left, _rect.top }; }
size2d<i32> frost::impl::window::get_size() const { return { _rect.right - _rect.left, _rect.bottom - _rect.top }; }
frost::api::window_procedure_sig frost::impl::window::get_procedure() const { return _procedure; }
void* frost::impl::window::get_data() const { return _data; }

void frost::impl::window::update_state()
{
	WINDOWPLACEMENT wp{};
	::GetWindowPlacement(_hwnd, &wp);
	frost::api::window_state new_state = int_to_state(wp.showCmd);

	if (_state != new_state)
	{
		_state = new_state;
		if (_procedure)
		{
			frost::api::window_event e = {};
			e.target	= this;
			e.type		= frost::api::window_event::event_type::state_change;
			_procedure(&e);
		}
	}
}
void frost::impl::window::update_rect()
{
	RECT new_rect = {};
	::GetWindowRect(_hwnd, &new_rect);
	_rect = new_rect;
}
void frost::impl::window::update_last_cursor_position(LPARAM l)
{
	_last_cursor_position.x = GET_X_LPARAM(l);
	_last_cursor_position.y = GET_Y_LPARAM(l);
}

void frost::impl::window::set_key_state(u64 index)
{
	_keystates[index / 64] |= (1ull << (index % 64));
}
void frost::impl::window::reset_key_state(u64 index)
{
	_keystates[index / 64] &= ~(1ull << (index % 64));
}
bool frost::impl::window::get_key_state(u64 index)
{
	return (_keystates[index / 64] >> (index % 64)) & 1ull;
}



ATOM frost::impl::window::get_window_atom()
{
	static ATOM _atom = 0;
	if (_atom == 0)
	{
		::WNDCLASSEXW wc = {};
		wc.cbSize			= sizeof(wc);
		wc.lpszClassName	= L"FROST_API_WINDOW_CLASS_99999";
		wc.cbWndExtra		= sizeof(void*);
		wc.style			= CS_DBLCLKS | CS_VREDRAW | CS_HREDRAW;
		wc.hbrBackground	= nullptr;
		wc.lpfnWndProc		= window_procedure;

		_atom = ::RegisterClassExW(&wc);
	}

	return _atom;
}

LRESULT frost::impl::window::wm_input(HWND hwnd, UINT msg, WPARAM w, LPARAM l)
{
	auto data = ::get_hwnd_data(hwnd);

	RAWINPUT rid = {};
	UINT rid_size = sizeof(rid);
	UINT i = ::GetRawInputData(reinterpret_cast<HRAWINPUT>(l), RID_INPUT, &rid, &rid_size, sizeof(RAWINPUTHEADER));

	if (rid.header.dwType == RIM_TYPEMOUSE)
	{
		auto& mouse = rid.data.mouse;

		if ((mouse.usFlags & MOUSE_MOVE_RELATIVE) == MOUSE_MOVE_RELATIVE &&
			(rid.data.mouse.lLastX != 0 || rid.data.mouse.lLastY != 0))
		{
			if (data->_procedure != nullptr)
			{
				frost::api::window_event e;
				e.target = data;
				e.type = frost::api::window_event::event_type::mouse_move;
				e.mouse_move.delta = { rid.data.mouse.lLastX, rid.data.mouse.lLastY };
				data->_procedure(&e);
			}
		}

		/* BUTTON EVENTS */
		bool down = false;
		api::keycode button = sysmap::system_button_event_to_internal_keycode(mouse.usButtonFlags, &down);

		if (button == api::keycode::null) { /* DO NOTHING */ }
		else if (down)
		{

			bool repeat = data->get_key_state(static_cast<u64>(button));
			data->set_key_state(static_cast<u64>(button));

			if (data->_procedure != nullptr)
			{
				frost::api::window_event e;
				e.target = data;
				e.type = frost::api::window_event::event_type::key_down;
				e.key_down.key = button;
				e.key_down.repeat = repeat;
				e.key_down.text = L"";
				data->_procedure(&e);
			}
		}
		else if (!down)
		{
			data->reset_key_state(static_cast<u64>(button));

			if (data->_procedure != nullptr)
			{
				frost::api::window_event e;
				e.target = data;
				e.type = frost::api::window_event::event_type::key_up;
				e.key_up.key = button;
				e.key_up.text = L"";
				data->_procedure(&e);
			}
		}
		

		/* SCROLLING EVENTS */
		if ((mouse.usButtonFlags & RI_MOUSE_WHEEL) != 0)
		{
			if (data->_procedure != nullptr)
			{
				frost::api::window_event e;
				e.target = data;
				e.type = frost::api::window_event::event_type::mouse_scroll;
				e.mouse_scroll.delta = { 0, ((i16)(u16)mouse.usButtonData) / WHEEL_DELTA };
				data->_procedure(&e);
			}
		}

		if ((mouse.usButtonFlags & RI_MOUSE_HWHEEL) != 0)
		{
			if (data->_procedure != nullptr)
			{
				frost::api::window_event e;
				e.target = data;
				e.type = frost::api::window_event::event_type::mouse_scroll;
				e.mouse_scroll.delta = { ((i16)(u16)mouse.usButtonData) / WHEEL_DELTA, 0 };
				data->_procedure(&e);
			}
		}
	}
	else if (rid.header.dwType == RIM_TYPEKEYBOARD)
	{
		auto& keyboard = rid.data.keyboard;

		if (keyboard.MakeCode == KEYBOARD_OVERRUN_MAKE_CODE)
			return ::DefWindowProcW(hwnd, msg, w, l);

		// ignore double WM_INPUT from PrtSc & Pause
		if (keyboard.MakeCode == 55 || keyboard.MakeCode == 69)
			return ::DefWindowProcW(hwnd, msg, w, l);
		
		frost::api::window_event e;
		e.target = data;
		e.type = (keyboard.Flags & RI_KEY_BREAK) == RI_KEY_BREAK ?
			frost::api::window_event::event_type::key_up :
			frost::api::window_event::event_type::key_down;
		 
		u8 syskey = sysmap::compose_syskey(keyboard.MakeCode, keyboard.Flags);
		api::keycode keycode = sysmap::syskey_to_keycode(syskey);

		wchar_t chars[8];
		sysmap::syskey_to_wchar(syskey, chars, 8);
		
		if (e.type == frost::api::window_event::event_type::key_down)
		{
			bool repeat = data->get_key_state(syskey);
			data->set_key_state(syskey);
			e.key_down.key = (frost::api::keycode)syskey;
			e.key_down.text = chars;
			e.key_down.repeat = repeat;
		}
		else if (e.type == frost::api::window_event::event_type::key_up)
		{
			data->reset_key_state(syskey);
			e.key_up.key = (frost::api::keycode)syskey;
			e.key_up.text = chars;
		}

		// SCAN CODES SHOULD BE MAPPED TO INTERNAL KEY REPRESENTATIONS
		auto proc = data->_procedure;
		proc(&e);
	}
	else if (rid.header.dwType == RIM_TYPEHID)
	{
		/* ... */
	}

	return ::DefWindowProcW(hwnd, msg, w, l);
}

LRESULT frost::impl::window::wm_double_click(HWND hwnd, UINT msg, WPARAM w, LPARAM l)
{
	auto data = ::get_hwnd_data(hwnd);
	data->update_last_cursor_position(l);
	if (data->_procedure)
	{
		frost::api::window_event e;
		e.target = data;
		e.type = frost::api::window_event::event_type::double_click;
		e.double_click.x = GET_X_LPARAM(l);
		e.double_click.y = GET_Y_LPARAM(l);
		switch (msg)
		{
		case WM_LBUTTONDBLCLK:
			e.double_click.key = frost::api::keycode::mouse_left;
			break;
		case WM_RBUTTONDBLCLK:
			e.double_click.key = frost::api::keycode::mouse_right;
			break;
		case WM_MBUTTONDBLCLK:
			e.double_click.key = frost::api::keycode::mouse_middle;
			break;
		case WM_XBUTTONDBLCLK:
			if (l == 0x0001)
				e.double_click.key = frost::api::keycode::mouse_x1;
			else if (l == 0x0002)
				e.double_click.key = frost::api::keycode::mouse_x2;
			else
				e.double_click.key = frost::api::keycode::null;
			break;
		default:
			e.double_click.key = frost::api::keycode::null;
		}
		data->_procedure(&e);
	}

	return ::DefWindowProcW(hwnd, msg, w, l);
}

LRESULT frost::impl::window::wm_mouse_move(HWND hwnd, UINT msg, WPARAM w, LPARAM l)
{
	auto data = ::get_hwnd_data(hwnd);
	data->update_last_cursor_position(l);

	if ((data->_flags & frost::impl::window::_flag_cursor_inside) != 0)
	{	// CURSOR MOVE
		if (data->_procedure)
		{
			frost::api::window_event e;
			e.target = data;
			e.type = frost::api::window_event::event_type::cursor_move;
			e.cursor_move.position = data->_last_cursor_position;
			data->_procedure(&e);
		}
	}
	else
	{	// CURSOR ENTER
		data->_flags |= frost::impl::window::_flag_cursor_inside;
		if (data->_procedure)
		{
			frost::api::window_event e;
			e.target = data;
			e.type = frost::api::window_event::event_type::cursor_enter;
			e.cursor_enter.position = data->_last_cursor_position;
			data->_procedure(&e);
		}
		::TrackMouseEvent(&data->_track_mouse_event);
	}
	return ::DefWindowProcW(hwnd, msg, w, l);
}
LRESULT frost::impl::window::wm_mouse_leave(HWND hwnd, UINT msg, WPARAM w, LPARAM l)
{
	auto data = ::get_hwnd_data(hwnd);
	data->_flags &= ~frost::impl::window::_flag_cursor_inside;

	if (data->_procedure)
	{
		frost::api::window_event e;
		e.target = data;
		e.type = frost::api::window_event::event_type::cursor_leave;
		e.cursor_leave.last_position = data->_last_cursor_position;
		data->_procedure(&e);
	}
	return ::DefWindowProcW(hwnd, msg, w, l);
}

LRESULT frost::impl::window::wm_move(HWND hwnd, UINT msg, WPARAM w, LPARAM l)
{
	auto data = ::get_hwnd_data(hwnd);
	data->update_rect();
	data->update_state();
	if (data->_procedure)
	{
		frost::api::window_event e;
		e.type			= frost::api::window_event::event_type::move;
		e.target		= data;
		e.position.x	= static_cast<WORD>(l & 0xFFFF);
		e.position.y	= static_cast<WORD>((l >> sizeof(WORD)) & 0xFFFF);
		data->_procedure(&e);
	}
	return ::DefWindowProcW(hwnd, msg, w, l);
}
LRESULT frost::impl::window::wm_size(HWND hwnd, UINT msg, WPARAM w, LPARAM l)
{
	auto data = ::get_hwnd_data(hwnd);
	data->update_rect();
	data->update_state();
	if (data->_procedure)
	{
		frost::api::window_event e;
		e.type			= frost::api::window_event::event_type::resize;
		e.target		= data;
		e.resize.width	= static_cast<WORD>(l & 0xFFFF);
		e.resize.height	= static_cast<WORD>((l >> sizeof(WORD)) & 0xFFFF);
		data->_procedure(&e);
	}
	return ::DefWindowProcW(hwnd, msg, w, l);
}

LRESULT frost::impl::window::wm_enable(HWND hwnd, UINT msg, WPARAM w, LPARAM l)
{
	auto data = ::get_hwnd_data(hwnd);
	if (w == TRUE)
	{
		data->_flags = data->_flags | window::_flag_enabled;

		if (data->_procedure)
		{
			frost::api::window_event e;
			e.target = data;
			e.type = frost::api::window_event::event_type::enable;
			data->_procedure(&e);
		}
	}
	else if (w == FALSE)
	{
		data->_flags = data->_flags & ~window::_flag_enabled;

		if (data->_procedure)
		{
			frost::api::window_event e;
			e.target = data;
			e.type = frost::api::window_event::event_type::disable;
			data->_procedure(&e);
		}
	}
	return ::DefWindowProcW(hwnd, msg, w, l);
}
LRESULT frost::impl::window::wm_activate(HWND hwnd, UINT msg, WPARAM w, LPARAM l)
{
	auto data = ::get_hwnd_data(hwnd);
	if (w == WA_INACTIVE)
	{
		data->_flags = data->_flags | window::_flag_active;

		if (data->_procedure)
		{
			frost::api::window_event e;
			e.target = data;
			e.type = frost::api::window_event::event_type::activate;
			data->_procedure(&e);
		}
	}
	else if (w == WA_ACTIVE || w == WA_CLICKACTIVE)
	{
		data->_flags = data->_flags & ~window::_flag_active;

		if (data->_procedure)
		{
			frost::api::window_event e;
			e.target = data;
			e.type = frost::api::window_event::event_type::deactivate;
			data->_procedure(&e);
		}
	}
	return ::DefWindowProcW(hwnd, msg, w, l);
}
LRESULT frost::impl::window::wm_set_focus(HWND hwnd, UINT msg, WPARAM w, LPARAM l)
{
	auto data = ::get_hwnd_data(hwnd);
	data->_flags = data->_flags | window::_flag_focused;

	if (data->_procedure)
	{
		frost::api::window_event e;
		e.target = data;
		e.type = frost::api::window_event::event_type::gain_focus;
		data->_procedure(&e);
	}
	return ::DefWindowProcW(hwnd, msg, w, l);
}
LRESULT frost::impl::window::wm_kill_focus(HWND hwnd, UINT msg, WPARAM w, LPARAM l)
{
	auto data = ::get_hwnd_data(hwnd);
	data->_flags = data->_flags & ~window::_flag_focused;

	if (data->_procedure)
	{
		frost::api::window_event e;
		e.target = data;
		e.type = frost::api::window_event::event_type::lose_focus;
		data->_procedure(&e);
	}
	return ::DefWindowProcW(hwnd, msg, w, l);
}

LRESULT frost::impl::window::wm_paint(HWND hwnd, UINT msg, WPARAM w, LPARAM l)
{
	auto data = ::get_hwnd_data(hwnd);

	RECT rect = {};
	::GetWindowRect(hwnd, &rect);
	rect = { 0, 0, rect.right - rect.left, rect.bottom - rect.top };

	auto hdc = ::GetDC(hwnd);
	auto br = ::CreateSolidBrush(RGB(255, 0, 255));
	::FillRect(hdc, &rect, br);

	::ReleaseDC(hwnd, hdc);
	::DeleteObject(br);
	return ::DefWindowProcW(hwnd, msg, w, l);
}

LRESULT frost::impl::window::wm_nc_calc_size(HWND hwnd, UINT msg, WPARAM w, LPARAM l)
{
	if (static_cast<bool>(w))
		return 0;
	return ::DefWindowProcW(hwnd, msg, w, l);
}
LRESULT frost::impl::window::wm_nc_hit_test(HWND hwnd, UINT msg, WPARAM w, LPARAM l)
{
	auto data = ::get_hwnd_data(hwnd);
	WORD x = GET_X_LPARAM(l);
	WORD y = GET_Y_LPARAM(l);
	constexpr LONG edge_margin = 5;
	
	data->update_rect();
	point2d<i32> top_left = { data->_rect.left, data->_rect.top };
	point2d<i32> bottom_right = { data->_rect.right, data->_rect.bottom };
	
	// EDGE DETECTION
	if (x >= top_left.x && x < (top_left.x + edge_margin))
	{
		if (y >= top_left.y && y < (top_left.y + edge_margin))
			return HTTOPLEFT;
		else if (y < bottom_right.y && y >= (bottom_right.y - edge_margin))
			return HTBOTTOMLEFT;
		else
			return HTLEFT;
	}
	else if (x < bottom_right.x && x >= (bottom_right.x - edge_margin))
	{
		if (y >= top_left.y && y < (top_left.y + edge_margin))
			return HTTOPRIGHT;
		else if (y < bottom_right.y && y >= (bottom_right.y - edge_margin))
			return HTBOTTOMRIGHT;
		else
			return HTRIGHT;
	}
	else
	{
		if (y >= top_left.y && y < (top_left.y + edge_margin))
			return HTTOP;
		else if (y < bottom_right.y && y >= (bottom_right.y - edge_margin))
			return HTBOTTOM;
		else
			return HTCLIENT;
	}
	// 
	// if (y > top_left.y && y < (top_left.y + 20))
	// 	return HTCAPTION;
	// else if ((x >= top_left.x && x < bottom_right.x) && (y >= top_left.y && y < bottom_right.y))
	//else
	//	return HTNOWHERE;
}
LRESULT frost::impl::window::wm_nc_activate(HWND hwnd, UINT msg, WPARAM w, LPARAM l)
{
	return 0; // PREVENT BORDER FROM BEING DRAWN
}

LRESULT frost::impl::window::wm_create(HWND hwnd, UINT msg, WPARAM w, LPARAM l)
{
	::SetWindowLongPtrW(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(*(void**)l));
	auto data = ::get_hwnd_data(hwnd);
	// data->acquire_reference();
	if (data->_procedure)
	{
		frost::api::window_event e;
		e.target = data;
		e.type = frost::api::window_event::event_type::create;
		data->_procedure(&e);
	}
	return ::DefWindowProcW(hwnd, msg, w, l);
}
LRESULT frost::impl::window::wm_close(HWND hwnd, UINT msg, WPARAM w, LPARAM l)
{
	auto data = ::get_hwnd_data(hwnd);
	if (data->_procedure)
	{
		frost::api::window_event e;
		e.target = data;
		e.type = frost::api::window_event::event_type::close;
		data->_procedure(&e);
	}
	return 0;
}
LRESULT frost::impl::window::wm_destroy(HWND hwnd, UINT msg, WPARAM w, LPARAM l)
{
	auto data = ::get_hwnd_data(hwnd);
	if (data->_procedure)
	{
		frost::api::window_event e;
		e.target = data;
		e.type = frost::api::window_event::event_type::destroy;
		data->_procedure(&e);
	}
	// data->release_reference();
	return ::DefWindowProcW(hwnd, msg, w, l);
}

LRESULT frost::impl::window::wm_input_language_changed(HWND hwnd, UINT msg, WPARAM w, LPARAM l)
{
	auto data = ::get_hwnd_data(hwnd);
	data->_hkl = ::GetKeyboardLayout(data->_thread_id);
	return ::DefWindowProcW(hwnd, msg, w, l);
}

LRESULT frost::impl::window::window_procedure(HWND hwnd, UINT msg, WPARAM w, LPARAM l)
{
	switch (msg)
	{
	case WM_INPUT:
		return wm_input(hwnd, msg, w, l);

	case WM_MOUSEMOVE:
		return wm_mouse_move(hwnd, msg, w, l);
	case WM_MOUSELEAVE:
		return wm_mouse_leave(hwnd, msg, w, l);

	case WM_MOVE:
		return wm_move(hwnd, msg, w, l);
	case WM_SIZE:
		return wm_size(hwnd, msg, w, l);

	case WM_ENABLE:
		return wm_enable(hwnd, msg, w, l);
	case WM_ACTIVATE:
		return wm_activate(hwnd, msg, w, l);
	case WM_SETFOCUS:
		return wm_set_focus(hwnd, msg, w, l);
	case WM_KILLFOCUS:
		return wm_kill_focus(hwnd, msg, w, l);


	case WM_PAINT:
		return wm_paint(hwnd, msg, w, l);

	case WM_NCHITTEST:
		return wm_nc_hit_test(hwnd, msg, w, l);
	case WM_NCCALCSIZE:
		return wm_nc_calc_size(hwnd, msg, w, l);
	case WM_NCACTIVATE:
		return wm_nc_activate(hwnd, msg, w, l);

	case WM_CREATE:
		return wm_create(hwnd, msg, w, l);
	case WM_CLOSE:
		return wm_close(hwnd, msg, w, l);
	case WM_DESTROY:
		return wm_destroy(hwnd, msg, w, l);

	case WM_INPUTLANGCHANGE:
		return wm_input_language_changed(hwnd, msg, w, l);
	}
	return ::DefWindowProcW(hwnd, msg, w, l);
}



frost::impl::window* frost::impl::window::create(const frost::api::window_description* description)
{
	auto* result = new window();
	result->_last_cursor_position = {};
	result->_flags		= _flag_enabled | _flag_active | _flag_focused;
	result->_state		= fold_state(description->state);
	result->_procedure	= description->procedure;
	result->_data		= description->data;
	result->_thread_id	= ::GetCurrentThreadId();
	result->_hkl		= ::GetKeyboardLayout(result->_thread_id);
	result->_rect		= {
		description->position.x,
		description->position.y,
		description->position.x + description->size.width,
		description->position.y + description->size.height
	};

	/* CREATE WINDOW */
	result->_hwnd = ::CreateWindowExW(
		WS_EX_LAYERED,
		(LPCWSTR)_window_class,
		nullptr, WS_CLIPCHILDREN | WS_SIZEBOX,
		description->position.x,
		description->position.y,
		description->size.width,
		description->size.height,
		nullptr,
		nullptr,
		nullptr,
		reinterpret_cast<void*>(result));

	if (result->_hwnd == nullptr)
	{	// ERROR
		result->acquire_reference();
		result->release_reference();
		// TODO: log_message(msg_window_creation_failed);
		return nullptr;
	}

	/* TRACK MOUSE LEAVING CLIENT AREA */
	result->_track_mouse_event.cbSize		= sizeof(_track_mouse_event);
	result->_track_mouse_event.hwndTrack	= result->_hwnd;
	result->_track_mouse_event.dwHoverTime	= 0;
	result->_track_mouse_event.dwFlags		= TME_LEAVE;

	/* INPUT HOOKS */
	RAWINPUTDEVICE rid[2] = {};
	for (auto& device : rid)
	{
		device.hwndTarget	= result->_hwnd;
		device.dwFlags		= 0x00;
		device.usUsagePage	= HID_USAGE_PAGE_GENERIC;
	}
	rid[0].usUsage = HID_USAGE_GENERIC_MOUSE;
	rid[1].usUsage = HID_USAGE_GENERIC_KEYBOARD;

	if (!::RegisterRawInputDevices(rid, sizeof(rid) / sizeof(*rid), sizeof(*rid)))
	{
		::DestroyWindow(result->_hwnd);
		result->_hwnd = nullptr;
		result->acquire_reference();
		result->release_reference();
		// TODO: log_message(msg_window_input_initialization_failed);
		return nullptr;
	}

	::SetLayeredWindowAttributes(result->_hwnd, 0, 255, 0);
	::ShowWindow(result->_hwnd, state_to_int(result->_state));

	return result;
}
void frost::impl::window::pump_messages()
{
	MSG msg{};
	while (::GetMessageW(&msg, _hwnd, 0, 0))
	{
		::TranslateMessage(&msg);
		::DispatchMessageW(&msg);
	}
}


static inline frost::api::window_state fold_state(frost::api::window_state  state)
{
	switch (state)
	{
	case frost::api::window_state::hidden:
	case frost::api::window_state::minimized:
	case frost::api::window_state::normal:
	case frost::api::window_state::maximized:
		return state;
	default:
		return frost::api::window_state::invalid;
	}
}
static inline int state_to_int(frost::api::window_state  state)
{
	switch (state)
	{
	case frost::api::window_state::hidden:
		return SW_HIDE;
	case frost::api::window_state::minimized:
		return SW_SHOWMINIMIZED;
	case frost::api::window_state::normal:
		return SW_SHOWNORMAL;
	case frost::api::window_state::maximized:
		return SW_SHOWMAXIMIZED;
	case frost::api::window_state::invalid:
	default:
		return -1;
	}
}
static inline frost::api::window_state  int_to_state(int state)
{
	switch (state)
	{
	case SW_HIDE:
		return frost::api::window_state::hidden;
	case SW_SHOWMINIMIZED:
		return frost::api::window_state::minimized;
	case SW_SHOWNORMAL:
		return frost::api::window_state::normal;
	case SW_SHOWMAXIMIZED:
		return frost::api::window_state::maximized;
	default:
		return frost::api::window_state::invalid;
	}
}
#else
static_assert("PLATFORM NOT SUPPORTED!")
#endif


const wchar_t* kc_to_wstr(frost::api::keycode keycode)
{
	switch (keycode)
	{
		case frost::api::keycode::null				: return L"null";
		case frost::api::keycode::mouse_left		: return L"mouse_left";
		case frost::api::keycode::mouse_right		: return L"mouse_right";
		case frost::api::keycode::mouse_middle		: return L"mouse_middle";
		case frost::api::keycode::mouse_x1			: return L"mouse_x1";
		case frost::api::keycode::mouse_x2			: return L"mouse_x2";
		case frost::api::keycode::escape			: return L"escape";
		case frost::api::keycode::an_1				: return L"an_1";
		case frost::api::keycode::an_2				: return L"an_2";
		case frost::api::keycode::an_3				: return L"an_3";
		case frost::api::keycode::an_4				: return L"an_4";
		case frost::api::keycode::an_5				: return L"an_5";
		case frost::api::keycode::an_6				: return L"an_6";
		case frost::api::keycode::an_7				: return L"an_7";
		case frost::api::keycode::an_8				: return L"an_8";
		case frost::api::keycode::an_9				: return L"an_9";
		case frost::api::keycode::an_0				: return L"an_0";
		case frost::api::keycode::dash				: return L"dash";
		case frost::api::keycode::equals			: return L"equals";
		case frost::api::keycode::backspace			: return L"backspace";
		case frost::api::keycode::tilde				: return L"tilde";
		case frost::api::keycode::q					: return L"q";
		case frost::api::keycode::w					: return L"w";
		case frost::api::keycode::e					: return L"e";
		case frost::api::keycode::r					: return L"r";
		case frost::api::keycode::t					: return L"t";
		case frost::api::keycode::y					: return L"y";
		case frost::api::keycode::u					: return L"u";
		case frost::api::keycode::i					: return L"i";
		case frost::api::keycode::o					: return L"o";
		case frost::api::keycode::p					: return L"p";
		case frost::api::keycode::bracket_open		: return L"bracket_open";
		case frost::api::keycode::bracket_close		: return L"bracket_close";
		case frost::api::keycode::a					: return L"a";
		case frost::api::keycode::s					: return L"s";
		case frost::api::keycode::d					: return L"d";
		case frost::api::keycode::f					: return L"f";
		case frost::api::keycode::g					: return L"g";
		case frost::api::keycode::h					: return L"h";
		case frost::api::keycode::j					: return L"j";
		case frost::api::keycode::k					: return L"k";
		case frost::api::keycode::l					: return L"l";
		case frost::api::keycode::semicolon			: return L"semicolon";
		case frost::api::keycode::quote				: return L"quote";
		case frost::api::keycode::z					: return L"z";
		case frost::api::keycode::x					: return L"x";
		case frost::api::keycode::c					: return L"c";
		case frost::api::keycode::backslash			: return L"backslash";
		case frost::api::keycode::v					: return L"v";
		case frost::api::keycode::b					: return L"b";
		case frost::api::keycode::n					: return L"n";
		case frost::api::keycode::m					: return L"m";
		case frost::api::keycode::comma				: return L"comma";
		case frost::api::keycode::period			: return L"period";
		case frost::api::keycode::slash				: return L"slash";
		case frost::api::keycode::f1				: return L"f1";
		case frost::api::keycode::f2				: return L"f2";
		case frost::api::keycode::f3				: return L"f3";
		case frost::api::keycode::f4				: return L"f4";
		case frost::api::keycode::f5				: return L"f5";
		case frost::api::keycode::f6				: return L"f6";
		case frost::api::keycode::f7				: return L"f7";
		case frost::api::keycode::f8				: return L"f8";
		case frost::api::keycode::f9				: return L"f9";
		case frost::api::keycode::f10				: return L"f10";
		case frost::api::keycode::f11				: return L"f11";
		case frost::api::keycode::f12				: return L"f12";
		case frost::api::keycode::tab				: return L"tab";
		case frost::api::keycode::capslock			: return L"capslock";
		case frost::api::keycode::left_shift		: return L"left_shift";
		case frost::api::keycode::right_shift		: return L"right_shift";
		case frost::api::keycode::left_ctrl			: return L"left_ctrl";
		case frost::api::keycode::right_ctrl		: return L"right_ctrl";
		case frost::api::keycode::left_win			: return L"left_win";
		case frost::api::keycode::right_win			: return L"right_win";
		case frost::api::keycode::left_alt			: return L"left_alt";
		case frost::api::keycode::right_alt			: return L"right_alt";
		case frost::api::keycode::space				: return L"space";
		case frost::api::keycode::enter				: return L"enter";
		case frost::api::keycode::left_arrow		: return L"left_arrow";
		case frost::api::keycode::up_arrow			: return L"up_arrow";
		case frost::api::keycode::right_arrow		: return L"right_arrow";
		case frost::api::keycode::down_arrow		: return L"down_arrow";
		case frost::api::keycode::numpad_0			: return L"numpad_0";
		case frost::api::keycode::numpad_1			: return L"numpad_1";
		case frost::api::keycode::numpad_2			: return L"numpad_2";
		case frost::api::keycode::numpad_3			: return L"numpad_3";
		case frost::api::keycode::numpad_4			: return L"numpad_4";
		case frost::api::keycode::numpad_5			: return L"numpad_5";
		case frost::api::keycode::numpad_6			: return L"numpad_6";
		case frost::api::keycode::numpad_7			: return L"numpad_7";
		case frost::api::keycode::numpad_8			: return L"numpad_8";
		case frost::api::keycode::numpad_9			: return L"numpad_9";
		case frost::api::keycode::numpad_numlock	: return L"numpad_numlock";
		case frost::api::keycode::numpad_divide		: return L"numpad_divide";
		case frost::api::keycode::numpad_multiply	: return L"numpad_multiply";
		case frost::api::keycode::numpad_minus		: return L"numpad_minus";
		case frost::api::keycode::numpad_plus		: return L"numpad_plus";
		case frost::api::keycode::numpad_enter		: return L"numpad_enter";
		case frost::api::keycode::numpad_separator	: return L"numpad_separator";
		case frost::api::keycode::print_screen		: return L"print_screen";
		case frost::api::keycode::scroll_lock		: return L"scroll_lock";

		case frost::api::keycode::insert			: return L"insert";
		case frost::api::keycode::home				: return L"home";
		case frost::api::keycode::page_up			: return L"page_up";
		case frost::api::keycode::del				: return L"del";
		case frost::api::keycode::end				: return L"end";
		case frost::api::keycode::page_down			: return L"page_down";
		case frost::api::keycode::pause				: return L"pause";
		default:
			return L"<UNKNOWN>";
	}
}