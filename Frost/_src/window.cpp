#include "../window.hpp"
#include "../color.hpp"

#if defined(TARGET_BUILD_PLATFORM_WINDOWS)
#include <Windows.h>

namespace frost
{
	static const u32 wm_procedure = 0xBFFF;
	static inline void null_procedure(window::api::window_event_data* e) { /* DO NOTHING*/ }
	static inline pimpl_t<window> get_data(HWND hwnd) { return reinterpret_cast<pimpl_t<window>>(::GetWindowLongPtrW(hwnd, GWLP_USERDATA)); }
	static inline void update_rects(pimpl_t<window> target);
	static inline void update_state(pimpl_t<window> target);
	static inline void update_last_cursor_pos(pimpl_t<window> target, LPARAM l);
	static inline void update_lwa(pimpl_t<window> target, u32* key, f32 opacity);
	static inline void set_key_state(pimpl_t<window> target, u8 key);
	static inline void reset_key_state(pimpl_t<window> target, u8 key);
	static inline bool get_key_state(pimpl_t<window> target, u8 key);
	static inline void wm_window_modify(window::api::window_modification_context* ctx);

	static int state_to_int(window::api::window_state state);
	static window::api::window_state int_to_state(int state);

	static ATOM get_window_atom();
	static LRESULT window_procedure(HWND, UINT, WPARAM, LPARAM);
	
	template<>
	class impl_t<window> final
	{
	public:
		HWND  hwnd = {};
		void* argument = {};
		window::api::window_procedure_sig procedure = {};
		RECT  client_rect = {};
		TRACKMOUSEEVENT track_mouse_event = {};
		POINT last_cursor_position = {};
		u64   flags = {};
		float opacity = {};
		u32   key = {};
		DWORD thread_id = {};

		window::api::window_state state = {};

		u8 byte_state[256 / 8] = {};

		static u64 flag_enabled;
		static u64 flag_activated;
		static u64 flag_focused;
		static u64 flag_use_key;
		static u64 flag_cursor_inside;
	};

	class window::api::window_modification_context final
	{
	public:
		pimpl_t<window> target;
		void* argument;
		window::api::window_modify_sig procedure;
	};

	u64 impl_t<window>::flag_enabled = (1 << 0);
	u64 impl_t<window>::flag_activated = (1 << 1);
	u64 impl_t<window>::flag_focused = (1 << 2);
	u64 impl_t<window>::flag_use_key = (1 << 3);
	u64 impl_t<window>::flag_cursor_inside = (1 << 4);

	pimpl_t<window> window::api::create(const window_description* description)
	{
		pimpl_t<window> data = new impl_t<window>();
		data->state = description->state;
		data->opacity = 1.0f;
		data->key = 0;
		data->flags = impl_t<window>::flag_enabled | impl_t<window>::flag_activated | impl_t<window>::flag_focused;
		data->thread_id = ::GetCurrentThreadId();
		data->procedure = description->procedure;
		if (data->procedure == nullptr)
			data->procedure = null_procedure;
		data->hwnd = ::CreateWindowExW(
			WS_EX_LAYERED,
			(LPCWSTR)get_window_atom(),
			description->caption,
			0ul,
			description->x,
			description->y,
			description->width,
			description->height,
			nullptr,
			nullptr,
			nullptr,
			data);

		data->track_mouse_event.cbSize		= sizeof(data->track_mouse_event);
		data->track_mouse_event.hwndTrack	= data->hwnd;
		data->track_mouse_event.dwHoverTime	= 0;
		data->track_mouse_event.dwFlags		= TME_LEAVE;

		::SetWindowLongW(data->hwnd, GWL_STYLE, 0l);
		::SetWindowPos(data->hwnd, nullptr, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE | SWP_NOACTIVATE | SWP_NOREPOSITION | SWP_ASYNCWINDOWPOS | SWP_NOZORDER);
		::SetLayeredWindowAttributes(data->hwnd, 0, 255, 0);

		::RAWINPUTDEVICE rid[2];
		for (auto& raw : rid)
		{
			raw.hwndTarget = data->hwnd;
			raw.dwFlags = 0x00;
			raw.usUsagePage = 0x01; // Generic desktop controlls
		}
		rid[0].usUsage = 0x02;	// MOUSE
		rid[1].usUsage = 0x06;	// KEYBOARD

		::RegisterRawInputDevices(rid, sizeof(rid) / sizeof(*rid), sizeof(*rid));

		::ShowWindow(data->hwnd, state_to_int(description->state));
		::InvalidateRect(data->hwnd, nullptr, TRUE);
		return data;
	}

	window::api::window_procedure_sig window::api::get_procedure(pimpl_t<window> target)
	{
		return target->procedure;
	}
	bool window::api::get_key_state(pimpl_t<window> target, u8 keycode)
	{
		return frost::get_key_state(target, keycode);
	}

	bool window::api::is_enabled(pimpl_t<window> target)
	{
		return (target->flags & impl_t<window>::flag_enabled) != 0;
	}
	bool window::api::is_active(pimpl_t<window> target)
	{
		return (target->flags & impl_t<window>::flag_activated) != 0;
	}
	bool window::api::is_focused(pimpl_t<window> target)
	{
		return (target->flags & impl_t<window>::flag_focused) != 0;
	}

	bool window::api::is_key_color_used(pimpl_t<window> target)
	{
		return (target->flags & impl_t<window>::flag_use_key) != 0;
	}
	bool window::api::get_key_color(pimpl_t<window> target)
	{
		u32 rgba;
		rgba8::api::abgr8_to_rgba8(target->key, &rgba);
		return rgba;
	}
	f32 window::api::get_opacity(pimpl_t<window> target)
	{
		return target->opacity;
	}

	window::api::window_state window::api::get_state(pimpl_t<window> target)
	{
		return target->state;
	}

	i32 window::api::get_x(pimpl_t<window> target)
	{
		return target->client_rect.left;
	}
	i32 window::api::get_y(pimpl_t<window> target)
	{
		return target->client_rect.top;
	}
	i32 window::api::get_width(pimpl_t<window> target)
	{
		return target->client_rect.right - target->client_rect.left;
	}
	i32 window::api::get_height(pimpl_t<window> target)
	{
		return target->client_rect.bottom - target->client_rect.top;
	}

	i32 window::api::get_caption_length(pimpl_t<window> target)
	{
		return ::GetWindowTextW(target->hwnd, nullptr, 0);
	}
	void window::api::get_caption(pimpl_t<window> target, wchar_t* caption, i32 max_write)
	{
		::GetWindowTextW(target->hwnd, caption, max_write);
	}

	void window::api::destroy(pimpl_t<window> p_impl)
	{
		::DestroyWindow(p_impl->hwnd);
	}

	void window::api::modify(pimpl_t<window> target, window::api::window_modify_sig modify_fn, void* argument)
	{
		if (target->thread_id == ::GetCurrentThreadId())
		{	// Same thread - modify at once
			window_modification_context ctx;
			ctx.argument = argument;
			ctx.target = target;
			ctx.procedure = modify_fn;
			modify_fn(&ctx);
		}
		else
		{	// Different thread - post thread message to modify
			auto* ctx = new window_modification_context();
			ctx->target = target;
			ctx->argument = argument;
			ctx->procedure = modify_fn;
			::PostThreadMessageW(
				target->thread_id,
				wm_procedure,
				reinterpret_cast<WPARAM>(ctx),
				reinterpret_cast<LPARAM>(wm_window_modify));
		}

	}

	void window::api::set_procedure(pimpl_t<window> target, window::api::window_procedure_sig procedure)
	{
		target->procedure = procedure;
	}
	void window::api::set_enabled(window_modification_context* target, bool enabled)
	{
		::EnableWindow(target->target->hwnd, enabled ? TRUE : FALSE);
	}
	void window::api::set_active(window_modification_context* target, bool active)
	{
		if (active && ::GetActiveWindow() != target->target->hwnd)
			::SetActiveWindow(target->target->hwnd);
		else if (!active && ::GetActiveWindow() == target->target->hwnd)
			::SetActiveWindow(NULL);
	}
	void window::api::set_focused(window_modification_context* target, bool focused)
	{
		if (focused && ::GetFocus() != target->target->hwnd)
			::SetFocus(target->target->hwnd);
		else if (!focused && ::GetFocus() == target->target->hwnd)
			::SetFocus(NULL);
	}

	void window::api::set_key_color_used(window_modification_context* target, bool use_key)
	{
		update_lwa(
			target->target,
			&target->target->key,
			target->target->opacity);
	}
	void window::api::set_key_color(window_modification_context* target, u32 rgba)
	{
		u32 abgr = RGB(255, 0, 255);
		rgba8::api::rgba8_to_abgr8(rgba, &abgr);
		update_lwa(
			target->target,
			&abgr,
			target->target->opacity);
	}
	void window::api::set_opacity(window_modification_context* target, f32 opacity)
	{
		update_lwa(
			target->target,
			is_key_color_used(target->target) ? &target->target->key : nullptr,
			opacity);
	}

	void window::api::set_state(window_modification_context* target, window_state state)
	{
		::ShowWindow(target->target->hwnd, state_to_int(state));
		target->target->state = state;
	}

	void window::api::set_position(window_modification_context* target, i32 x, i32 y)
	{
		::SetWindowPos(
			target->target->hwnd,
			nullptr,
			x, y, 0, 0,
			SWP_NOSIZE);
	}
	void window::api::set_size(window_modification_context* target, i32 width, i32 height)
	{
		::SetWindowPos(
			target->target->hwnd,
			nullptr,
			0, 0, width, height,
			SWP_NOMOVE);
	}

	void window::api::set_client_position(window_modification_context* target, i32 x, i32 y)
	{
		RECT rect = { x, y, 0, 0 };
		::AdjustWindowRectEx(
			&rect,
			::GetWindowLongW(target->target->hwnd, GWL_STYLE),
			FALSE,
			::GetWindowLongW(target->target->hwnd, GWL_EXSTYLE));

		::SetWindowPos(
			target->target->hwnd,
			nullptr,
			rect.left, rect.top,
			0, 0,
			SWP_NOSIZE);
	}
	void window::api::set_client_size(window_modification_context* target, i32 width, i32 height)
	{
		RECT rect = { 0, 0, width, height };
		::AdjustWindowRectEx(
			&rect,
			::GetWindowLongW(target->target->hwnd, GWL_STYLE),
			FALSE,
			::GetWindowLongW(target->target->hwnd, GWL_EXSTYLE));

		::SetWindowPos(
			target->target->hwnd,
			nullptr,
			0, 0,
			rect.right - rect.left, rect.bottom - rect.top,
			SWP_NOMOVE);
	}

	void window::api::set_caption(window_modification_context* target, const wchar_t* caption)
	{
		::SetWindowTextW(target->target->hwnd, caption);
	}

	void window::api::prepare_message_queue()
	{
		MSG msg;
		::PeekMessageW(&msg, nullptr, 0, 0, PM_NOREMOVE);
	}
	void window::api::message_pump()
	{
		MSG msg = {};
		::GetMessageW(&msg, nullptr, 0, 0);
		if (msg.message == wm_procedure)
			reinterpret_cast<void(*)(void*)>(msg.lParam)(reinterpret_cast<void*>(msg.wParam));
		::TranslateMessage(&msg);
		::DispatchMessageW(&msg);
	}



	static inline void update_rects(pimpl_t<window> target)
	{
		::GetClientRect(target->hwnd, &target->client_rect);
	}
	static inline void update_state(pimpl_t<window> target)
	{
		WINDOWPLACEMENT wp{};
		::GetWindowPlacement(target->hwnd, &wp);
		target->state = int_to_state(wp.showCmd);
	}
	static inline void update_last_cursor_pos(pimpl_t<window> target, LPARAM l)
	{
		target->last_cursor_position.x = static_cast<WORD>(l & 0xFFFF);
		target->last_cursor_position.y = static_cast<WORD>((l >> sizeof(WORD)) & 0xFFFF);
	}
	static inline void update_lwa(pimpl_t<window> target, u32* key, f32 opacity)
	{
		bool use_key = key != nullptr;
		bool use_opacity = opacity < 1.0f;

		target->flags = use_key ?
			target->flags | impl_t<window>::flag_use_key :
			target->flags & ~impl_t<window>::flag_use_key;
		target->opacity = opacity;
		target->key = use_key ? *key : target->key;

		u8 opacity_value = static_cast<u8>((opacity > 1.0f ? 1.0f : opacity < 0.0f ? 0.0f : opacity) * 255);
		::SetLayeredWindowAttributes(
			target->hwnd,
			target->key,
			opacity_value,
			(((target->flags & impl_t<window>::flag_use_key) != 0) ? LWA_COLORKEY : 0) |
			(use_opacity ? LWA_ALPHA : 0));
	}

	static inline void set_key_state(pimpl_t<window> target, u8 key)
	{
		u64 index = key / 8;
		u64 offset = key % 8;
		target->byte_state[index] |= (1 << offset);
	}
	static inline void reset_key_state(pimpl_t<window> target, u8 key)
	{
		u64 index = key / 8;
		u64 offset = key % 8;
		target->byte_state[index] &= ~(1 << offset);
	}
	static inline bool get_key_state(pimpl_t<window> target, u8 key)
	{
		u64 index = key / 8;
		u64 offset = key % 8;
		return (target->byte_state[index] & (1 << offset)) != 0;
	}

	static int state_to_int(window::api::window_state state)
	{
		switch (state)
		{
		case window::api::window_state::hidden:		return SW_HIDE;
		case window::api::window_state::minimized:	return SW_SHOWMINIMIZED;
		case window::api::window_state::normal:		return SW_SHOWNORMAL;
		case window::api::window_state::maximized:	return SW_SHOWMAXIMIZED;
		default:
			return -1;
		}
	}
	static window::api::window_state int_to_state(int state)
	{
		switch (state)
		{
		case SW_HIDE:			return window::api::window_state::hidden;
		case SW_SHOWMINIMIZED:	return window::api::window_state::minimized;
		case SW_SHOWNORMAL:		return window::api::window_state::normal;
		case SW_SHOWMAXIMIZED:	return window::api::window_state::maximized;
		default:
			return window::api::window_state::invalid;
		}
	}

	static ATOM get_window_atom()
	{
		static ATOM _atom = 0;
		if (_atom == 0)
		{
			::WNDCLASSEXW wc = {};
			wc.cbSize = sizeof(wc);
			wc.lpszClassName = L"FROST_API_WINDOW_CLASS_99999";
			wc.cbWndExtra = sizeof(void*);
			wc.style = CS_DBLCLKS | CS_OWNDC | CS_VREDRAW | CS_HREDRAW;
			wc.hbrBackground = nullptr;
			wc.lpfnWndProc = window_procedure;

			_atom = ::RegisterClassExW(&wc);
		}

		return _atom;
	}

	static inline void wm_window_modify(window::api::window_modification_context* ctx)
	{
		ctx->procedure(ctx);
		delete ctx;
	}

	static LRESULT wm_input(HWND hwnd, UINT msg, WPARAM w, LPARAM l)
	{
		auto data = get_data(hwnd);

		if (data->procedure == nullptr)
			return ::DefWindowProcW(hwnd, msg, w, l);

		RAWINPUT rid = {};
		UINT rid_size = sizeof(rid);
		UINT i = ::GetRawInputData(reinterpret_cast<HRAWINPUT>(l), RID_INPUT, &rid, &rid_size, sizeof(RAWINPUTHEADER));
		if (rid.header.dwType == RIM_TYPEMOUSE)
		{
			auto& mouse = rid.data.mouse;

			if ((mouse.usFlags & MOUSE_MOVE_RELATIVE) == MOUSE_MOVE_RELATIVE &&
				(rid.data.mouse.lLastX != 0 || rid.data.mouse.lLastY != 0))
			{
				window::api::window_event_data e;
				e.target = data;
				e.type = window::api::window_event_data::type_mouse_move;
				e.mouse_move.delta_x = rid.data.mouse.lLastX;
				e.mouse_move.delta_y = rid.data.mouse.lLastY;
				data->procedure(&e);
			}

			static u16 buttons[] =
			{
				window::api::keycode_mouse_left,
				window::api::keycode_mouse_right,
				window::api::keycode_mouse_middle,
				window::api::keycode_mouse_x1,
				window::api::keycode_mouse_x2,
			};
			static u16 down_flags[] =
			{
				RI_MOUSE_BUTTON_1_DOWN,
				RI_MOUSE_BUTTON_2_DOWN,
				RI_MOUSE_BUTTON_3_DOWN,
				RI_MOUSE_BUTTON_4_DOWN,
				RI_MOUSE_BUTTON_5_DOWN
			};
			static u16 up_flags[] =
			{
				RI_MOUSE_BUTTON_1_UP,
				RI_MOUSE_BUTTON_2_UP,
				RI_MOUSE_BUTTON_3_UP,
				RI_MOUSE_BUTTON_4_UP,
				RI_MOUSE_BUTTON_5_UP
			};

			for (u32 i = 0; i < (sizeof(down_flags) / sizeof(*down_flags)); i++)
			{
				if ((mouse.usButtonFlags & down_flags[i]) != 0)
				{
					set_key_state(data, static_cast<u8>(buttons[i]));

					window::api::window_event_data e;
					e.target = data;
					e.type = window::api::window_event_data::type_key_down;
					e.key_down.key = static_cast<u8>(buttons[i]);
					e.key_down.character = L'\0';
					data->procedure(&e);
				}
				if ((mouse.usButtonFlags & up_flags[i]) != 0)
				{
					reset_key_state(data, static_cast<u8>(buttons[i]));

					window::api::window_event_data e;
					e.target = data;
					e.type = window::api::window_event_data::type_key_up;
					e.key_up.key = static_cast<u8>(buttons[i]);
					e.key_down.character = L'\0';
					data->procedure(&e);
				}
			}

			if ((mouse.usButtonFlags & RI_MOUSE_WHEEL) != 0)
			{
				window::api::window_event_data e;
				e.target = data;
				e.type = window::api::window_event_data::type_cursor_scroll;
				e.scroll.delta = ((i16)(u16)mouse.usButtonData) / WHEEL_DELTA;
				data->procedure(&e);
			}

			if ((mouse.usButtonFlags & RI_MOUSE_HWHEEL) != 0)
			{
				window::api::window_event_data e;
				e.target = data;
				e.type = window::api::window_event_data::type_cursor_scroll;
				e.scroll.delta = ((i16)(u16)mouse.usButtonData) / WHEEL_DELTA;
				data->procedure(&e);
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

			u16 scanCode = keyboard.MakeCode;
			scanCode |= (keyboard.Flags & RI_KEY_E0) ? 0xe000 : 0;
			scanCode |= (keyboard.Flags & RI_KEY_E1) ? 0xe100 : 0;

			window::api::window_event_data e;
			e.target = data;
			e.type = (keyboard.Flags & RI_KEY_BREAK) == RI_KEY_BREAK ?
				window::api::window_event_data::type_key_up :
				window::api::window_event_data::type_key_down;

			u8 keycode = static_cast<u8>(keyboard.MakeCode);
			if ((keyboard.Flags & RI_KEY_E0) || (keyboard.Flags & RI_KEY_E1))
				keycode |= 0x80;

			auto hkl = ::GetKeyboardLayout(::GetCurrentThreadId());
			if (e.type == window::api::window_event_data::type_key_down)
			{
				set_key_state(data, keycode);
				e.key_down.key = keycode;
				e.key_down.character = ::MapVirtualKeyExW(keyboard.VKey, MAPVK_VK_TO_CHAR, hkl);
			}
			else if (e.type == window::api::window_event_data::type_key_up)
			{
				reset_key_state(data, keycode);
				e.key_up.key = keycode;
				e.key_down.character = ::MapVirtualKeyExW(keyboard.VKey, MAPVK_VK_TO_CHAR, hkl);
			}

			// SCAN CODES SHOULD BE MAPPED TO INTERNAL KEY REPRESENTATIONS
			data->procedure(&e);
		}
		else if (rid.header.dwType == RIM_TYPEHID)
		{
			/* ... */
		}
		return ::DefWindowProcW(hwnd, msg, w, l);
	}

	static LRESULT wm_double_click(HWND hwnd, UINT msg, WPARAM w, LPARAM l)
	{
		auto data = get_data(hwnd);
		update_last_cursor_pos(data, l);

		if (data->procedure)
		{
			window::api::window_event_data e;
			e.target = data;
			e.type = window::api::window_event_data::type_double_click;
			e.double_click.x = data->last_cursor_position.x / (data->client_rect.right - data->client_rect.left);
			e.double_click.y = data->last_cursor_position.y / (data->client_rect.bottom - data->client_rect.top);
			switch (msg)
			{
			case WM_LBUTTONDBLCLK:
				e.double_click.key = window::api::keycode_mouse_left;
				break;
			case WM_RBUTTONDBLCLK:
				e.double_click.key = window::api::keycode_mouse_right;
				break;
			case WM_MBUTTONDBLCLK:
				e.double_click.key = window::api::keycode_mouse_middle;
				break;
			case WM_XBUTTONDBLCLK:
				if (l == 0x0001)
					e.double_click.key = window::api::keycode_mouse_x1;
				else if (l == 0x0002)
					e.double_click.key = window::api::keycode_mouse_x2;
				else
					e.double_click.key = 0;
				break;
			default:
				e.double_click.key = 0;
			}
			data->procedure(&e);
		}

		return ::DefWindowProcW(hwnd, msg, w, l);
	}

	static LRESULT wm_cursor_move(HWND hwnd, UINT msg, WPARAM w, LPARAM l)
	{
		auto data = get_data(hwnd);
		update_last_cursor_pos(data, l);

		if ((data->flags & impl_t<window>::flag_cursor_inside) != 0)
		{	// MOVE
			if (data->procedure)
			{
				window::api::window_event_data e;
				e.target = data;
				e.type = window::api::window_event_data::type_cursor_move;
				e.cursor_move.x = data->last_cursor_position.x / (data->client_rect.right - data->client_rect.left);;
				e.cursor_move.y = data->last_cursor_position.y / (data->client_rect.bottom - data->client_rect.top);;
				data->procedure(&e);
			}
		}
		else
		{	// ENTER
			if (data->procedure)
			{
				window::api::window_event_data e;
				e.target = data;
				e.type = window::api::window_event_data::type_cursor_enter;
				e.cursor_enter.x = data->last_cursor_position.x / (data->client_rect.right - data->client_rect.left);;
				e.cursor_enter.y = data->last_cursor_position.y / (data->client_rect.bottom - data->client_rect.top);;
				data->procedure(&e);
			}
			data->flags |= impl_t<window>::flag_cursor_inside;
			::TrackMouseEvent(&data->track_mouse_event);
		}

		return ::DefWindowProcW(hwnd, msg, w, l);
	}

	static LRESULT wm_cursor_leave(HWND hwnd, UINT msg, WPARAM w, LPARAM l)
	{
		auto data = get_data(hwnd);
		data->flags &= ~impl_t<window>::flag_cursor_inside;

		if (data->procedure)
		{
			window::api::window_event_data e;
			e.target = data;
			e.type = window::api::window_event_data::type_cursor_leave;
			e.cursor_leave.x = data->last_cursor_position.x / (data->client_rect.right - data->client_rect.left);;
			e.cursor_leave.y = data->last_cursor_position.y / (data->client_rect.bottom - data->client_rect.top);;
			data->procedure(&e);
		}
		return ::DefWindowProcW(hwnd, msg, w, l);
	}

	static LRESULT wm_move(HWND hwnd, UINT msg, WPARAM w, LPARAM l)
	{
		auto data = get_data(hwnd);
		update_rects(data);
		update_state(data);
		if (data->procedure)
		{
			window::api::window_event_data e;
			e.target = data;
			e.type = window::api::window_event_data::type_move;
			e.move.x = static_cast<WORD>(l & 0xFFFF);
			e.move.y = static_cast<WORD>((l >> sizeof(WORD)) & 0xFFFF);
			data->procedure(&e);
		}
		return ::DefWindowProcW(hwnd, msg, w, l);
	}

	static LRESULT wm_resize(HWND hwnd, UINT msg, WPARAM w, LPARAM l)
	{
		auto data = get_data(hwnd);
		update_rects(data);
		update_state(data);
		if (data->procedure)
		{
			window::api::window_event_data e;
			e.target = data;
			e.type = window::api::window_event_data::type_resize;
			e.resize.width = static_cast<WORD>(l & 0xFFFF);
			e.resize.height = static_cast<WORD>((l >> sizeof(WORD)) & 0xFFFF);
			data->procedure(&e);
		}
		return ::DefWindowProcW(hwnd, msg, w, l);
	}

	static LRESULT wm_enable(HWND hwnd, UINT msg, WPARAM w, LPARAM l)
	{
		auto data = get_data(hwnd);
		if (w == TRUE)
		{
			data->flags = data->flags | impl_t<window>::flag_enabled;

			if (data->procedure)
			{
				window::api::window_event_data e;
				e.target = data;
				e.type = window::api::window_event_data::type_enable;
				e.enable.enabled = true;
				data->procedure(&e);
			}
		}
		else if (w == FALSE)
		{
			data->flags = data->flags & ~impl_t<window>::flag_enabled;

			if (data->procedure)
			{
				window::api::window_event_data e;
				e.target = data;
				e.type = window::api::window_event_data::type_enable;
				e.enable.enabled = true;
				data->procedure(&e);
			}
		}
		return ::DefWindowProcW(hwnd, msg, w, l);
	}

	static LRESULT wm_activate(HWND hwnd, UINT msg, WPARAM w, LPARAM l)
	{
		auto data = get_data(hwnd);
		if (w == WA_INACTIVE)
		{
			data->flags = data->flags | impl_t<window>::flag_activated;

			if (data->procedure)
			{
				window::api::window_event_data e;
				e.target = data;
				e.type = window::api::window_event_data::type_activate;
				e.activate.activated = false;
				data->procedure(&e);
			}
		}
		else if (w == WA_ACTIVE || w == WA_CLICKACTIVE)
		{
			data->flags = data->flags & ~impl_t<window>::flag_activated;

			if (data->procedure)
			{
				window::api::window_event_data e;
				e.target = data;
				e.type = window::api::window_event_data::type_activate;
				e.activate.activated = true;
				data->procedure(&e);
			}
		}
		return ::DefWindowProcW(hwnd, msg, w, l);
	}

	static LRESULT wm_focus(HWND hwnd, UINT msg, WPARAM w, LPARAM l)
	{
		auto data = get_data(hwnd);
		if (msg == WM_SETFOCUS)
		{
			data->flags = data->flags | impl_t<window>::flag_focused;

			if (data->procedure)
			{
				window::api::window_event_data e;
				e.target = data;
				e.type = window::api::window_event_data::type_focus;
				e.focus.focused = true;
				data->procedure(&e);
			}
		}
		else if (msg == WM_KILLFOCUS)
		{
			data->flags = data->flags & ~impl_t<window>::flag_focused;

			if (data->procedure)
			{
				window::api::window_event_data e;
				e.target = data;
				e.type = window::api::window_event_data::type_focus;
				e.focus.focused = true;
				data->procedure(&e);
			}
		}

		return ::DefWindowProcW(hwnd, msg, w, l);
	}

	static LRESULT wm_create(HWND hwnd, UINT msg, WPARAM w, LPARAM l)
	{
		::SetWindowLongPtrW(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(*reinterpret_cast<pimpl_t<window>*>(l)));
		auto data = get_data(hwnd);
		if (data->procedure)
		{
			window::api::window_event_data e;
			e.target = data;
			e.type = window::api::window_event_data::type_create;
			data->procedure(&e);
		}
		return ::DefWindowProcW(hwnd, msg, w, l);
	}

	static LRESULT wm_close(HWND hwnd, UINT msg, WPARAM w, LPARAM l)
	{
		auto data = get_data(hwnd);
		if (data->procedure)
		{
			window::api::window_event_data e;
			e.target = data;
			e.type = window::api::window_event_data::type_close;
			data->procedure(&e);
		}
		return 0;
	}

	static LRESULT wm_destroy(HWND hwnd, UINT msg, WPARAM w, LPARAM l)
	{
		auto data = get_data(hwnd);
		if (data->procedure)
		{
			window::api::window_event_data e;
			e.target = data;
			e.type = window::api::window_event_data::type_destroy;
			data->procedure(&e);
		}
		delete data;
		return ::DefWindowProcW(hwnd, msg, w, l);
	}

	static LRESULT wm_nccalcsize(HWND hwnd, UINT msg, WPARAM w, LPARAM l)
	{
		if (static_cast<bool>(w))
			return 0;
		return DefWindowProc(hwnd, msg, w, l);
	}
	static LRESULT wm_nchittest(HWND hwnd, UINT msg, WPARAM w, LPARAM l)
	{
		return HTCAPTION;
	}
	
	static LRESULT window_procedure(HWND hwnd, UINT msg, WPARAM w, LPARAM l)
	{
		switch (msg)
		{
		case WM_INPUT:
			return wm_input(hwnd, msg, w, l);
		case WM_NCHITTEST:
			return wm_nchittest(hwnd, msg, w, l);
		case WM_LBUTTONDBLCLK:
		case WM_RBUTTONDBLCLK:
		case WM_MBUTTONDBLCLK:
		case WM_XBUTTONDBLCLK:
			return wm_double_click(hwnd, msg, w, l);

		case WM_MOUSEMOVE:
			return wm_cursor_move(hwnd, msg, w, l);
		case WM_MOUSELEAVE:
			return wm_cursor_leave(hwnd, msg, w, l);

		case WM_MOVE:
			return wm_move(hwnd, msg, w, l);
		case WM_SIZE:
			return wm_resize(hwnd, msg, w, l);

		case WM_ENABLE:
			return wm_enable(hwnd, msg, w, l);
		case WM_ACTIVATE:
			return wm_activate(hwnd, msg, w, l);
		case WM_SETFOCUS:
		case WM_KILLFOCUS:
			return wm_focus(hwnd, msg, w, l);

		case WM_CREATE:
			return wm_create(hwnd, msg, w, l);
		case WM_NCCALCSIZE:
			return wm_nccalcsize(hwnd, msg, w, l);
		case WM_CLOSE:
			return wm_close(hwnd, msg, w, l);
		case WM_DESTROY:
			return wm_destroy(hwnd, msg, w, l);
		}
		return ::DefWindowProcW(hwnd, msg, w, l);
	}
}
#endif
