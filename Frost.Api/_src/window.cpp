#include "_internal/sysmap.impl.hpp"
#include "object.hpp"
#include <windowsx.h>
#include <hidusage.h>
using namespace frost::api;
using namespace frost::impl;
#define WND(x) ((window*)(x))
static bool is_direct_invoke_required(window* target);
static inline void update_rect(window* target);
static inline void update_state(window* target);
static inline void update_last_cursor_position(window* target, LPARAM l);

static inline window_state fold_state(window_state state);
static inline int state_to_int(window_state state);
static inline window_state int_to_state(int state);

static inline void set_key_state(window* target, u64 index);
static inline void reset_key_state(window* target, u64 index);
static inline bool get_key_state(window* target, u64 index);

static inline ATOM get_window_atom();
static inline window* get_hwnd_data(HWND hwnd) { return reinterpret_cast<window*>(::GetWindowLongPtrW(hwnd, GWLP_USERDATA)); }


static LRESULT CALLBACK window_procedure(HWND hwnd, UINT msg, WPARAM w, LPARAM l);

static LRESULT wm_input(HWND hwnd, UINT msg, WPARAM w, LPARAM l);

static LRESULT wm_double_click(HWND hwnd, UINT msg, WPARAM w, LPARAM l);

static LRESULT wm_mouse_move(HWND hwnd, UINT msg, WPARAM w, LPARAM l);
static LRESULT wm_mouse_leave(HWND hwnd, UINT msg, WPARAM w, LPARAM l);

static LRESULT wm_move(HWND hwnd, UINT msg, WPARAM w, LPARAM l);
static LRESULT wm_size(HWND hwnd, UINT msg, WPARAM w, LPARAM l);

static LRESULT wm_enable(HWND hwnd, UINT msg, WPARAM w, LPARAM l);
static LRESULT wm_activate(HWND hwnd, UINT msg, WPARAM w, LPARAM l);
static LRESULT wm_set_focus(HWND hwnd, UINT msg, WPARAM w, LPARAM l);
static LRESULT wm_kill_focus(HWND hwnd, UINT msg, WPARAM w, LPARAM l);

static LRESULT wm_paint(HWND hwnd, UINT msg, WPARAM w, LPARAM l);

static LRESULT wm_nc_calc_size(HWND hwnd, UINT msg, WPARAM w, LPARAM l);
static LRESULT wm_nc_hit_test(HWND hwnd, UINT msg, WPARAM w, LPARAM l);
static LRESULT wm_nc_activate(HWND hwnd, UINT msg, WPARAM w, LPARAM l);

static LRESULT wm_create(HWND hwnd, UINT msg, WPARAM w, LPARAM l);
static LRESULT wm_close(HWND hwnd, UINT msg, WPARAM w, LPARAM l);
static LRESULT wm_destroy(HWND hwnd, UINT msg, WPARAM w, LPARAM l);

static LRESULT wm_input_language_changed(HWND hwnd, UINT msg, WPARAM w, LPARAM l);

class execute_deferred_data
{
public:
	window* target;
	void* p_argument;
};

WORD window::_window_class = get_window_atom();

FROST_API bool _stdcall frost_api_window_is_enabled(object* target)
{
	switch (target->type)
	{
	case object_type::window:
		return (((window*)target)->_flags & window::_flag_enabled) != 0;
	default:
		return false;
	}
}
FROST_API bool _stdcall frost_api_window_is_active(object* target)
{
	switch (target->type)
	{
	case object_type::window:
		return (((window*)target)->_flags & window::_flag_active) != 0;
	default:
		return false;
	}
}
FROST_API bool _stdcall frost_api_window_is_focused(object* target)
{
	switch (target->type)
	{
	case object_type::window:
		return (((window*)target)->_flags & window::_flag_focused) != 0;
	default:
		return false;
	}
}

FROST_API window_state _stdcall frost_api_window_get_state(object* target)
{
	return ((window*)target)->_state;
}

FROST_API bool _stdcall frost_api_window_get_key_state(object* target, keycode key)
{
	switch (target->type)
	{
	case object_type::window:
		return get_key_state((window*)target, static_cast<u64>(key));
	default:
		return false;
	}
}

FROST_API void _stdcall frost_api_window_get_position(object* target, point2d<i32>* output)
{
	switch (target->type)
	{
	case object_type::window:
		output->x = ((window*)target)->_rect.left;
		output->y = ((window*)target)->_rect.top;
		break;
	default:
		break;
	}
}
FROST_API void _stdcall frost_api_window_get_size(object* target, size2d<i32>* output)
{
	switch (target->type)
	{
	case object_type::window:
		output->width = ((window*)target)->_rect.right - ((window*)target)->_rect.left;
		output->height = ((window*)target)->_rect.bottom - ((window*)target)->_rect.top;
		break;
	default:
		break;
	}
}

FROST_API void _stdcall frost_api_window_set_enabled(object* target, bool enabled)
{
	bool flag = false;
	switch (target->type)
	{
		case object_type::window:
		flag = frost_api_window_is_enabled(target);
		if ((flag && enabled) || !(flag || enabled))
			return;
		if (is_direct_invoke_required(WND(target)))
		{
			::EnableWindow((HWND)WND(target)->handle, static_cast<BOOL>(enabled));
			return;
		}
		else
		{
			execute_deferred_data data;
			data.target = WND(target);
			data.p_argument = &enabled;
			frost_api_thread_message_send(
				WND(target)->_thread.get(),
				[](void* arg) {
					auto& edd = *reinterpret_cast<execute_deferred_data*>(arg);
					frost_api_window_set_enabled(edd.target, *reinterpret_cast<bool*>(edd.p_argument));
				},
				&data);
		}
		break;
		default:
			break;
	}
}
FROST_API void _stdcall frost_api_window_set_active(object* target, bool active)
{
	bool flag = false;
	switch (target->type)
	{
	case object_type::window:
		flag = frost_api_window_is_active(target);
		if ((flag && active) || !(flag || active))
			return;
		if (is_direct_invoke_required(WND(target)))
		{
			if (flag && !active)
				::SetActiveWindow(nullptr);
			else if (!flag && active)
				::SetActiveWindow((HWND)WND(target)->handle);
			return;
		}
		else
		{
			execute_deferred_data data;
			data.target = WND(target);
			data.p_argument = &active;
			frost_api_thread_message_send(
				WND(target)->_thread.get(),
				[](void* arg) {
					auto& edd = *reinterpret_cast<execute_deferred_data*>(arg);
					frost_api_window_set_active(edd.target, *reinterpret_cast<bool*>(edd.p_argument));
				},
				&data);
		}
		break;
	default:
		break;
	}
}
FROST_API void _stdcall frost_api_window_set_focused(object* target, bool focus)
{
	bool flag = false;
	switch (target->type)
	{
	case object_type::window:
		flag = frost_api_window_is_focused(target);
		if ((flag && focus) || !(flag || focus))
			return;
		if (is_direct_invoke_required(WND(target)))
		{
			if (flag && !focus)
				::SetFocus(nullptr);
			else if (!flag && focus)
				::SetFocus((HWND)WND(target)->handle);
			return;
		}
		else
		{
			execute_deferred_data data;
			data.target = WND(target);
			data.p_argument = &focus;
			frost_api_thread_message_send(
				WND(target)->_thread.get(),
				[](void* arg) {
					auto& edd = *reinterpret_cast<execute_deferred_data*>(arg);
					frost_api_window_set_enabled(edd.target, *reinterpret_cast<bool*>(edd.p_argument));
				},
				&data);
		}
		break;
	default:
		break;
	}
}

FROST_API void _stdcall frost_api_window_set_state(object* target, window_state state)
{
	switch (target->type)
	{
	case object_type::window:
		if (is_direct_invoke_required(WND(target)))
		{
			::ShowWindow((HWND)WND(target)->handle, state_to_int(state));
			update_state(WND(target));
		}
		else
		{
			execute_deferred_data data;
			data.target = WND(target);
			data.p_argument = &state;
			frost_api_thread_message_send(
				WND(target)->_thread.get(),
				[](void* arg) {
					auto& edd = *reinterpret_cast<execute_deferred_data*>(arg);
					frost_api_window_set_state(edd.target, (*reinterpret_cast<window_state*>(edd.p_argument)));
				},
				&data);
		}
	default:
		break;
	}
}

FROST_API void _stdcall frost_api_window_set_position(object* target, point2d<i32> position)
{
	switch (target->type)
	{
	case object_type::window:
		if (is_direct_invoke_required(WND(target)))
		{
			::SetWindowPos((HWND)WND(target)->handle, nullptr, position.x, position.y, 0, 0, SWP_NOSIZE);
		}
		else
		{
			execute_deferred_data data;
			data.target = WND(target);
			data.p_argument = &position;
			frost_api_thread_message_send(
				WND(target)->_thread.get(),
				[](void* arg) {
					auto& edd = *reinterpret_cast<execute_deferred_data*>(arg);
					frost_api_window_set_position(edd.target, *reinterpret_cast<point2d<i32>*>(edd.p_argument));
				},
				&data);
		}
	default:
		break;
	}
}
FROST_API void _stdcall frost_api_window_set_size(object* target, size2d<i32> size)
{
	switch (target->type)
	{
	case object_type::window:
		if (is_direct_invoke_required(WND(target)))
		{
			::SetWindowPos((HWND)WND(target)->handle, nullptr, 0, 0, size.width, size.height, SWP_NOMOVE);
		}
		else
		{
			execute_deferred_data data;
			data.target = WND(target);
			data.p_argument = &size;
			frost_api_thread_message_send(
				WND(target)->_thread.get(),
				[](void* arg) {
					auto& edd = *reinterpret_cast<execute_deferred_data*>(arg);
					frost_api_window_set_size(edd.target, *reinterpret_cast<size2d<i32>*>(edd.p_argument));
				},
				&data);
		}
	default:
		break;
	}
}

FROST_API window_procedure_sig _stdcall frost_api_window_get_procedure(object* target)
{
	switch (target->type)
	{
	case object_type::window:
		return WND(target)->_procedure;
	default:
		return nullptr;
	}
}
FROST_API void* _stdcall frost_api_window_get_data(object* target)
{
	switch (target->type)
	{
	case object_type::window:
		return WND(target)->_data;
	default:
		return nullptr;
	}
}

FROST_API void _stdcall frost_api_window_set_procedure(object* target, window_procedure_sig procedure)
{
	switch (target->type)
	{
	case object_type::window:
		WND(target)->_procedure = procedure;
		break;
	default:
		break;
	}
}
FROST_API void _stdcall frost_api_window_set_data(object* target, void* data)
{
	switch (target->type)
	{
	case object_type::window:
		WND(target)->_data = data;
		break;
	default:
		break;
	}
}

FROST_API object* _stdcall frost_api_window_create(const window_description* description)
{
	auto hwnd = ::CreateWindowExW(
		WS_EX_LAYERED,
		(LPCWSTR)window::_window_class,
		nullptr,
		WS_CLIPCHILDREN | WS_SIZEBOX,
		description->position.x,
		description->position.y,
		description->size.width,
		description->size.height,
		nullptr,
		nullptr,
		nullptr,
		const_cast<void*>(reinterpret_cast<const void*>(description)));

	if (hwnd == nullptr)
		return nullptr;

	LONG_PTR ptr = ::GetWindowLongPtrW(hwnd, GWLP_USERDATA);
	return reinterpret_cast<window*>(ptr);
}

static bool is_direct_invoke_required(window* target)
{
	return target->_thread_id == ::GetCurrentThreadId();
}
static inline window_state fold_state(window_state  state)
{
	switch (state)
	{
	case window_state::hidden:
	case window_state::minimized:
	case window_state::normal:
	case window_state::maximized:
		return state;
	default:
		return window_state::invalid;
	}
}
static inline int state_to_int(window_state  state)
{
	switch (state)
	{
	case window_state::hidden:
		return SW_HIDE;
	case window_state::minimized:
		return SW_SHOWMINIMIZED;
	case window_state::normal:
		return SW_SHOWNORMAL;
	case window_state::maximized:
		return SW_SHOWMAXIMIZED;
	case window_state::invalid:
	default:
		return -1;
	}
}
static inline window_state  int_to_state(int state)
{
	switch (state)
	{
	case SW_HIDE:
		return window_state::hidden;
	case SW_SHOWMINIMIZED:
		return window_state::minimized;
	case SW_SHOWNORMAL:
		return window_state::normal;
	case SW_SHOWMAXIMIZED:
		return window_state::maximized;
	default:
		return window_state::invalid;
	}
}
static inline void update_state(window* target)
{
	window_state new_state;
	HWND hwnd = (HWND)target->handle;
	bool visible = (GetWindowLongW(hwnd, GWL_STYLE) & WS_VISIBLE) != 0;
	if (visible)
	{
		WINDOWPLACEMENT wp{};
		::GetWindowPlacement((HWND)target->handle, &wp);
		new_state = int_to_state(wp.showCmd);
	}
	else
	{
		new_state = window_state::hidden;
	}


	if (target->_state != new_state)
	{
		target->_state = new_state;
		if (target->_procedure)
		{
			window_event e = {};
			e.target = target;
			e.type = window_event::event_type::state_change;
			target->_procedure(&e);
		}
	}
}

static inline void update_last_cursor_position(window* target, LPARAM l)
{
	target->_last_cursor_position.x = GET_X_LPARAM(l);
	target->_last_cursor_position.y = GET_Y_LPARAM(l);
}

static inline ATOM get_window_atom()
{
	static ATOM _atom = 0;
	if (_atom == 0)
	{
		::WNDCLASSEXW wc = {};
		wc.cbSize = sizeof(wc);
		wc.lpszClassName = L"FROST_API_WINDOW_CLASS_99999";
		wc.cbWndExtra = sizeof(void*);
		wc.style = CS_DBLCLKS | CS_VREDRAW | CS_HREDRAW;
		wc.hbrBackground = nullptr;
		wc.lpfnWndProc = window_procedure;

		_atom = ::RegisterClassExW(&wc);
	}

	return _atom;
}

static inline void set_key_state(window* target, u64 index)
{
	target->_keystates[index / 64] |= (1ull << (index % 64));
}
static inline void reset_key_state(window* target, u64 index)
{
	target->_keystates[index / 64] &= ~(1ull << (index % 64));
}
static inline bool get_key_state(window* target, u64 index)
{
	return (target->_keystates[index / 64] >> (index % 64)) & 1ull;
}

static LRESULT window_procedure(HWND hwnd, UINT msg, WPARAM w, LPARAM l)
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

static inline void update_rect(window* target)
{
	RECT new_rect = {};
	::GetWindowRect((HWND)target->handle, &new_rect);
	target->_rect = new_rect;
}

LRESULT wm_input(HWND hwnd, UINT msg, WPARAM w, LPARAM l)
{
	auto data = get_hwnd_data(hwnd);

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
				window_event e;
				e.target = data;
				e.type = window_event::event_type::mouse_move;
				e.mouse_move.delta = { rid.data.mouse.lLastX, rid.data.mouse.lLastY };
				data->_procedure(&e);
			}
		}

		/* BUTTON EVENTS */
		bool down = false;
		keycode button = frost::impl::sysmap::system_button_event_to_internal_keycode(mouse.usButtonFlags, &down);

		if (button == keycode::null) { /* DO NOTHING */ }
		else if (down)
		{
			bool repeat = get_key_state(data, static_cast<u64>(button));
			set_key_state(data, static_cast<u64>(button));

			if (data->_procedure != nullptr)
			{
				window_event e;
				e.target = data;
				e.type = window_event::event_type::key_down;
				e.key_down.key = button;
				e.key_down.repeat = repeat;
				e.key_down.text = L"";
				data->_procedure(&e);
			}
		}
		else if (!down)
		{
			reset_key_state(data, static_cast<u64>(button));

			if (data->_procedure != nullptr)
			{
				window_event e;
				e.target = data;
				e.type = window_event::event_type::key_up;
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
				window_event e;
				e.target = data;
				e.type = window_event::event_type::mouse_scroll;
				e.mouse_scroll.delta = { 0, ((i16)(u16)mouse.usButtonData) / WHEEL_DELTA };
				data->_procedure(&e);
			}
		}

		if ((mouse.usButtonFlags & RI_MOUSE_HWHEEL) != 0)
		{
			if (data->_procedure != nullptr)
			{
				window_event e;
				e.target = data;
				e.type = window_event::event_type::mouse_scroll;
				e.mouse_scroll.delta = { ((i16)(u16)mouse.usButtonData) / WHEEL_DELTA, 0 };
				data->_procedure(&e);
			}
		}
	}
	else if (rid.header.dwType == RIM_TYPEKEYBOARD)
	{
		auto& keyboard = rid.data.keyboard;

		// ignore overrun makecode
		if (keyboard.MakeCode == KEYBOARD_OVERRUN_MAKE_CODE)
			return ::DefWindowProcW(hwnd, msg, w, l);

		// ignore double WM_INPUTs, let left shift pass
		if (keyboard.MakeCode == 42 && keyboard.VKey != VK_SHIFT)
			return ::DefWindowProcW(hwnd, msg, w, l);

		// ignore double WM_INPUTs, let control keys pass
		if (keyboard.MakeCode == 29 && keyboard.VKey != VK_CONTROL && keyboard.VKey != VK_PAUSE)
			return ::DefWindowProcW(hwnd, msg, w, l);

		// ignore second VK_PAUSE input message
		if (keyboard.VKey == 255)
			return ::DefWindowProcW(hwnd, msg, w, l);

		window_event e;
		e.target = data;
		e.type = (keyboard.Flags & RI_KEY_BREAK) == RI_KEY_BREAK ?
			window_event::event_type::key_up :
			window_event::event_type::key_down;

		u8 syskey = frost::impl::sysmap::compose_syskey(keyboard.MakeCode, keyboard.Flags);
		keycode keycode = frost::impl::sysmap::syskey_to_keycode(syskey);

		wchar_t chars[8];
		frost::impl::sysmap::syskey_to_wcs(syskey, chars, 8, false);

		if (e.type == window_event::event_type::key_down)
		{
			bool repeat = get_key_state(data, syskey);
			set_key_state(data, syskey);
			e.key_down.key = keycode;
			e.key_down.text = chars;
			e.key_down.repeat = repeat;
		}
		else if (e.type == window_event::event_type::key_up)
		{
			reset_key_state(data, syskey);
			e.key_up.key = keycode;
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

LRESULT wm_double_click(HWND hwnd, UINT msg, WPARAM w, LPARAM l)
{
	auto data = get_hwnd_data(hwnd);
	update_last_cursor_position(data, l);
	if (data->_procedure)
	{
		window_event e;
		e.target = data;
		e.type = window_event::event_type::double_click;
		e.double_click.x = GET_X_LPARAM(l);
		e.double_click.y = GET_Y_LPARAM(l);
		switch (msg)
		{
		case WM_LBUTTONDBLCLK:
			e.double_click.key = keycode::mouse_left;
			break;
		case WM_RBUTTONDBLCLK:
			e.double_click.key = keycode::mouse_right;
			break;
		case WM_MBUTTONDBLCLK:
			e.double_click.key = keycode::mouse_middle;
			break;
		case WM_XBUTTONDBLCLK:
			if (l == 0x0001)
				e.double_click.key = keycode::mouse_x1;
			else if (l == 0x0002)
				e.double_click.key = keycode::mouse_x2;
			else
				e.double_click.key = keycode::null;
			break;
		default:
			e.double_click.key = keycode::null;
		}
		data->_procedure(&e);
	}

	return ::DefWindowProcW(hwnd, msg, w, l);
}

LRESULT wm_mouse_move(HWND hwnd, UINT msg, WPARAM w, LPARAM l)
{
	auto data = get_hwnd_data(hwnd);
	update_last_cursor_position(data, l);

	if ((data->_flags & window::_flag_cursor_inside) != 0)
	{	// CURSOR MOVE
		if (data->_procedure)
		{
			window_event e;
			e.target = data;
			e.type = window_event::event_type::cursor_move;
			e.cursor_move.position = data->_last_cursor_position;
			data->_procedure(&e);
		}
	}
	else
	{	// CURSOR ENTER
		data->_flags |= window::_flag_cursor_inside;
		if (data->_procedure)
		{
			window_event e;
			e.target = data;
			e.type = window_event::event_type::cursor_enter;
			e.cursor_enter.position = data->_last_cursor_position;
			data->_procedure(&e);
		}
		::TrackMouseEvent(&data->_track_mouse_event);
	}
	return ::DefWindowProcW(hwnd, msg, w, l);
}
LRESULT wm_mouse_leave(HWND hwnd, UINT msg, WPARAM w, LPARAM l)
{
	auto data = get_hwnd_data(hwnd);
	data->_flags &= ~window::_flag_cursor_inside;

	if (data->_procedure)
	{
		window_event e;
		e.target = data;
		e.type = window_event::event_type::cursor_leave;
		e.cursor_leave.last_position = data->_last_cursor_position;
		data->_procedure(&e);
	}
	return ::DefWindowProcW(hwnd, msg, w, l);
}

LRESULT wm_move(HWND hwnd, UINT msg, WPARAM w, LPARAM l)
{
	auto data = get_hwnd_data(hwnd);
	update_rect(data);
	update_state(data);
	if (data->_procedure)
	{
		window_event e;
		e.type = window_event::event_type::move;
		e.target = data;
		e.position.x = static_cast<WORD>(l & 0xFFFF);
		e.position.y = static_cast<WORD>((l >> (sizeof(WORD) * 8)) & 0xFFFF);
		data->_procedure(&e);
	}
	return ::DefWindowProcW(hwnd, msg, w, l);
}
LRESULT wm_size(HWND hwnd, UINT msg, WPARAM w, LPARAM l)
{
	auto data = get_hwnd_data(hwnd);
	update_rect(data);
	update_state(data);
	if (data->_procedure)
	{
		window_event e;
		e.type = window_event::event_type::resize;
		e.target = data;
		e.resize.width = static_cast<WORD>(l & 0xFFFF);
		e.resize.height = static_cast<WORD>((l >> (sizeof(WORD) * 8)) & 0xFFFF);
		data->_procedure(&e);
	}
	return ::DefWindowProcW(hwnd, msg, w, l);
}

LRESULT wm_enable(HWND hwnd, UINT msg, WPARAM w, LPARAM l)
{
	auto data = get_hwnd_data(hwnd);
	if (w == TRUE)
	{
		data->_flags = data->_flags | window::_flag_enabled;

		if (data->_procedure)
		{
			window_event e;
			e.target = data;
			e.type = window_event::event_type::enable;
			data->_procedure(&e);
		}
	}
	else if (w == FALSE)
	{
		data->_flags = data->_flags & ~window::_flag_enabled;

		if (data->_procedure)
		{
			window_event e;
			e.target = data;
			e.type = window_event::event_type::disable;
			data->_procedure(&e);
		}
	}
	return ::DefWindowProcW(hwnd, msg, w, l);
}
LRESULT wm_activate(HWND hwnd, UINT msg, WPARAM w, LPARAM l)
{
	auto data = get_hwnd_data(hwnd);
	if (w == WA_INACTIVE)
	{
		data->_flags = data->_flags | window::_flag_active;

		if (data->_procedure)
		{
			window_event e;
			e.target = data;
			e.type = window_event::event_type::activate;
			data->_procedure(&e);
		}
	}
	else if (w == WA_ACTIVE || w == WA_CLICKACTIVE)
	{
		data->_flags = data->_flags & ~window::_flag_active;

		if (data->_procedure)
		{
			window_event e;
			e.target = data;
			e.type = window_event::event_type::deactivate;
			data->_procedure(&e);
		}
	}
	return ::DefWindowProcW(hwnd, msg, w, l);
}
LRESULT wm_set_focus(HWND hwnd, UINT msg, WPARAM w, LPARAM l)
{
	auto data = get_hwnd_data(hwnd);
	data->_flags = data->_flags | window::_flag_focused;

	if (data->_procedure)
	{
		window_event e;
		e.target = data;
		e.type = window_event::event_type::gain_focus;
		data->_procedure(&e);
	}
	return ::DefWindowProcW(hwnd, msg, w, l);
}
LRESULT wm_kill_focus(HWND hwnd, UINT msg, WPARAM w, LPARAM l)
{
	auto data = get_hwnd_data(hwnd);
	data->_flags = data->_flags & ~window::_flag_focused;

	if (data->_procedure)
	{
		window_event e;
		e.target = data;
		e.type = window_event::event_type::lose_focus;
		data->_procedure(&e);
	}
	return ::DefWindowProcW(hwnd, msg, w, l);
}

LRESULT wm_paint(HWND hwnd, UINT msg, WPARAM w, LPARAM l)
{
	auto data = get_hwnd_data(hwnd);

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

LRESULT wm_nc_calc_size(HWND hwnd, UINT msg, WPARAM w, LPARAM l)
{
	if (static_cast<bool>(w))
		return 0;
	return ::DefWindowProcW(hwnd, msg, w, l);
}
LRESULT wm_nc_hit_test(HWND hwnd, UINT msg, WPARAM w, LPARAM l)
{
	auto data = get_hwnd_data(hwnd);
	WORD x = GET_X_LPARAM(l);
	WORD y = GET_Y_LPARAM(l);
	constexpr LONG edge_margin = 5;

	update_rect(data);
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
LRESULT wm_nc_activate(HWND hwnd, UINT msg, WPARAM w, LPARAM l)
{

	return ::DefWindowProcW(hwnd, msg, w, l);; // PREVENT BORDER FROM BEING DRAWN
}

LRESULT wm_create(HWND hwnd, UINT msg, WPARAM w, LPARAM l)
{
	window_description* desc = *reinterpret_cast<window_description**>(l);
	window* result = new window();
	result->type = object_type::window;
	result->handle = hwnd;

	::SetWindowLongPtrW(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(result));
	result->_last_cursor_position = {};
	result->_flags = window::_flag_enabled | window::_flag_active | window::_flag_focused;
	result->_state = fold_state(desc->state);
	result->_procedure = desc->procedure;
	result->_data = desc->data;
	result->_thread = frost_api_thread_get_current();
	result->_thread_id = ::GetCurrentThreadId();
	result->_hkl = ::GetKeyboardLayout(result->_thread_id);
	result->_rect = {
		desc->position.x,
		desc->position.y,
		desc->position.x + desc->size.width,
		desc->position.y + desc->size.height
	};

	/* TRACK MOUSE LEAVING CLIENT AREA */
	result->_track_mouse_event.cbSize = sizeof(window::_track_mouse_event);
	result->_track_mouse_event.hwndTrack = hwnd;
	result->_track_mouse_event.dwHoverTime = 0;
	result->_track_mouse_event.dwFlags = TME_LEAVE;

	/* INPUT HOOKS */
	RAWINPUTDEVICE rid[2] = {};
	for (auto& device : rid)
	{
		device.hwndTarget = hwnd;
		device.dwFlags = 0x00;
		device.usUsagePage = HID_USAGE_PAGE_GENERIC;
	}
	rid[0].usUsage = HID_USAGE_GENERIC_MOUSE;
	rid[1].usUsage = HID_USAGE_GENERIC_KEYBOARD;

	if (!::RegisterRawInputDevices(rid, sizeof(rid) / sizeof(*rid), sizeof(*rid)))
	{
		::DestroyWindow(hwnd);
		frost_api_object_acquire_reference(result);
		frost_api_object_release_reference(result);
		return ::DefWindowProcW(hwnd, msg, w, l);
	}

	/* LAYERED WINDOW SETUP */
	::SetLayeredWindowAttributes(hwnd, 0, 255, 0);
	::ShowWindow(hwnd, state_to_int(result->_state));
	auto data = get_hwnd_data(hwnd);

	/* EMIT EVENT */
	if (data->_procedure)
	{
		window_event e;
		e.target = data;
		e.type = window_event::event_type::create;
		data->_procedure(&e);
	}
	return ::DefWindowProcW(hwnd, msg, w, l);
}
LRESULT wm_close(HWND hwnd, UINT msg, WPARAM w, LPARAM l)
{
	auto data = get_hwnd_data(hwnd);
	if (data->_procedure)
	{
		window_event e;
		e.target = data;
		e.type = window_event::event_type::close;
		data->_procedure(&e);
	}
	return 0;
}
LRESULT wm_destroy(HWND hwnd, UINT msg, WPARAM w, LPARAM l)
{
	auto data = get_hwnd_data(hwnd);
	if (data->_procedure)
	{
		window_event e;
		e.target = data;
		e.type = window_event::event_type::destroy;
		data->_procedure(&e);
	}
	// data->release_reference();
	return ::DefWindowProcW(hwnd, msg, w, l);
}

LRESULT wm_input_language_changed(HWND hwnd, UINT msg, WPARAM w, LPARAM l)
{
	auto data = get_hwnd_data(hwnd);
	data->_hkl = ::GetKeyboardLayout(::GetCurrentThreadId());
	return ::DefWindowProcW(hwnd, msg, w, l);
}
