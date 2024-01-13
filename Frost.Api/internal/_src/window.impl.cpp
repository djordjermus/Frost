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

namespace frost::impl
{
	class window::execute_deferred_data
	{
	public:
		void (*procedure)(window* target, void* p_argument);
		window* target;
		void* p_argument;
	};
	static const wchar_t* err_window_creation_failed = L"Window creation failed! ";
	static const u64 err_window_creation_failed_length = ::wcslen(err_window_creation_failed);

	WORD frost::impl::window::_window_class = frost::impl::window::get_window_atom();
	static inline frost::api::window_state fold_state(frost::api::window_state state);
	static inline int state_to_int(frost::api::window_state state);
	static inline frost::api::window_state int_to_state(int state);
	static inline frost::impl::window* get_hwnd_data(HWND hwnd) { return reinterpret_cast<frost::impl::window*>(::GetWindowLongPtrW(hwnd, GWLP_USERDATA)); }
	window::window(HWND hwnd) :
		synchronizable(hwnd) {}
	window::~window()
	{
		::DestroyWindow(get_hwnd());
	}

	bool window::is_enabled() const
	{
		return (_flags & _flag_enabled) != 0;
	}
	bool window::is_active() const
	{
		return (_flags & _flag_active) != 0;
	}
	bool window::is_focused() const
	{
		return (_flags & _flag_focused) != 0;
	}

	frost::api::window_state window::get_state() const
	{
		return _state;
	}

	point2d<i32> window::get_position() const
	{
		return { _rect.left, _rect.top };
	}
	size2d<i32> window::get_size() const
	{
		return { _rect.right - _rect.left, _rect.bottom - _rect.top };
	}
	frost::api::window_procedure_sig window::get_procedure() const
	{
		return _procedure;
	}
	void* window::get_data() const
	{
		return _data;
	}

	void window::set_enabled(bool enabled)
	{
		bool flag = is_enabled();
		if ((flag && enabled) || !(flag || enabled))
			return;

		if (is_direct_invoke_required())
		{
			::EnableWindow(get_hwnd(), static_cast<BOOL>(enabled));
			return;
		}
		else
		{
			execute_deferred_data data;
			data.procedure	= [](window* target, void* arg) { target->set_enabled(*reinterpret_cast<bool*>(arg)); };
			data.target		= this;
			data.p_argument	= &enabled;
			execute_deferred(&data);
		}
	}
	void window::set_active(bool active)
	{
#pragma warning(push)
#pragma warning(disable: 6387)
		bool flag = is_active();
		if ((flag && active) || !(flag || active))
			return;

		if (is_direct_invoke_required())
		{
			if (flag && !active)
				::SetActiveWindow(nullptr);
			else if (!flag && active)
				::SetActiveWindow(get_hwnd());
			return;
		}
		else
		{
			execute_deferred_data data;
			data.procedure	= [](window* target, void* arg) { target->set_active(*reinterpret_cast<bool*>(arg)); };
			data.target		= this;
			data.p_argument	= &active;
			execute_deferred(&data);
		}
#pragma warning(pop)
	}
	void window::set_focus(bool focus)
	{
		bool flag = is_focused();
		if ((flag && focus) || !(flag || focus))
			return;

		if (is_direct_invoke_required())
		{
			if (flag && !focus)
				::SetFocus(nullptr);
			else if (!flag && focus)
				::SetFocus(get_hwnd());
			return;
		}
		else
		{
			execute_deferred_data data;
			data.procedure	= [](window* target, void* arg) { target->set_focus(*reinterpret_cast<bool*>(arg)); };
			data.target		= this;
			data.p_argument	= &focus;
			execute_deferred(&data);
		}
		return; // TODO DEFERRED
	}

	void window::set_state(api::window_state state)
	{
		if (is_direct_invoke_required())
		{
			::ShowWindow(get_hwnd(), state_to_int(state));
		}
		else
		{
			execute_deferred_data data;
			data.procedure	= [](window* target, void* arg) { target->set_state(*reinterpret_cast<api::window_state*>(arg)); };
			data.target		= this;
			data.p_argument	= &state;
			execute_deferred(&data);
		}
	}

	void window::set_position(point2d<i32> position)
	{
		if (is_direct_invoke_required())
		{
			::SetWindowPos(get_hwnd(), nullptr, position.x, position.y, 0, 0, SWP_NOSIZE);
		}
		else
		{
			execute_deferred_data data;
			data.procedure		= [](window* target, void* arg) { target->set_position(*reinterpret_cast<point2d<i32>*>(arg)); };
			data.target			= this;
			data.p_argument		= &position;
			execute_deferred(&data);
		}
	}
	void window::set_size(size2d<i32> size)
	{
		if (is_direct_invoke_required())
		{
			::SetWindowPos(get_hwnd(), nullptr, 0, 0, size.width , size.height, SWP_NOMOVE);
		}
		else
		{
			execute_deferred_data data;
			data.procedure	= [](window* target, void* arg) { target->set_size(*reinterpret_cast<size2d<i32>*>(arg)); };
			data.target		= this;
			data.p_argument	= &size;
			execute_deferred(&data);
		}
	}
	void window::set_procedure(api::window_procedure_sig procedure)
	{
		_procedure = procedure;
	}
	void window::set_data(void* data)
	{
		_data = data;
	}

	bool window::signal() const
	{
		return true;
		/* DO NOTHING */
	}
	HWND window::get_hwnd() const
	{
		return reinterpret_cast<HWND>(get_system_handle());
	}
	bool window::is_direct_invoke_required() const
	{
		return _thread_id == ::GetCurrentThreadId();
	}
	bool window::execute_deferred(execute_deferred_data* data, bool wait)
	{
		if (!is_direct_invoke_required())
			return false;

		if (wait)
		{
			HANDLE e = ::CreateEventW(nullptr, false, false,	nullptr);
			if (e == nullptr)
				return false;

			BOOL result = ::PostThreadMessageW(_thread_id, modify_deferred_msg, reinterpret_cast<WPARAM>(data), reinterpret_cast<LPARAM>(e));
			if (result != TRUE)
			{
				::CloseHandle(e);
				return false;
			}

			if (::WaitForSingleObject(e, ~0ul) != WAIT_OBJECT_0)
			{
				::CloseHandle(e);
				return false;
			}
			else
			{
				::CloseHandle(e);
				return true;
			}
		}
		else
		{
			execute_deferred_data* delete_me = new execute_deferred_data();
			*delete_me = *data;
			BOOL result = ::PostThreadMessageW(_thread_id, modify_deferred_msg, reinterpret_cast<WPARAM>(data), 0);
			if (result != TRUE)
			{
				delete delete_me;
				return false;
			}
			return true;
		}
	}
	void window::update_state()
	{
		WINDOWPLACEMENT wp{};
		::GetWindowPlacement(get_hwnd(), &wp);
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
	void window::update_rect()
	{
		RECT new_rect = {};
		::GetWindowRect(get_hwnd(), &new_rect);
		_rect = new_rect;
	}
	void window::update_last_cursor_position(LPARAM l)
	{
		_last_cursor_position.x = GET_X_LPARAM(l);
		_last_cursor_position.y = GET_Y_LPARAM(l);
	}

	void window::set_key_state(u64 index)
	{
		_keystates[index / 64] |= (1ull << (index % 64));
	}
	void window::reset_key_state(u64 index)
	{
		_keystates[index / 64] &= ~(1ull << (index % 64));
	}
	bool window::get_key_state(u64 index)
	{
		return (_keystates[index / 64] >> (index % 64)) & 1ull;
	}


	ATOM window::get_window_atom()
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

	LRESULT window::wm_input(HWND hwnd, UINT msg, WPARAM w, LPARAM l)
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

			frost::api::window_event e;
			e.target = data;
			e.type = (keyboard.Flags & RI_KEY_BREAK) == RI_KEY_BREAK ?
				frost::api::window_event::event_type::key_up :
				frost::api::window_event::event_type::key_down;
		 
			u8 syskey = sysmap::compose_syskey(keyboard.MakeCode, keyboard.Flags);
			api::keycode keycode = sysmap::syskey_to_keycode(syskey);

			wchar_t chars[8];
			sysmap::syskey_to_wcs(syskey, chars, 8, false);
		
			if (e.type == frost::api::window_event::event_type::key_down)
			{
				bool repeat = data->get_key_state(syskey);
				data->set_key_state(syskey);
				e.key_down.key = keycode;
				e.key_down.text = chars;
				e.key_down.repeat = repeat;
			}
			else if (e.type == frost::api::window_event::event_type::key_up)
			{
				data->reset_key_state(syskey);
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

	LRESULT window::wm_double_click(HWND hwnd, UINT msg, WPARAM w, LPARAM l)
	{
		auto data = get_hwnd_data(hwnd);
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

	LRESULT window::wm_mouse_move(HWND hwnd, UINT msg, WPARAM w, LPARAM l)
	{
		auto data = get_hwnd_data(hwnd);
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
	LRESULT window::wm_mouse_leave(HWND hwnd, UINT msg, WPARAM w, LPARAM l)
	{
		auto data = get_hwnd_data(hwnd);
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

	LRESULT window::wm_move(HWND hwnd, UINT msg, WPARAM w, LPARAM l)
	{
		auto data = get_hwnd_data(hwnd);
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
	LRESULT window::wm_size(HWND hwnd, UINT msg, WPARAM w, LPARAM l)
	{
		auto data = get_hwnd_data(hwnd);
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

	LRESULT window::wm_enable(HWND hwnd, UINT msg, WPARAM w, LPARAM l)
	{
		auto data = get_hwnd_data(hwnd);
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
	LRESULT window::wm_activate(HWND hwnd, UINT msg, WPARAM w, LPARAM l)
	{
		auto data = get_hwnd_data(hwnd);
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
	LRESULT window::wm_set_focus(HWND hwnd, UINT msg, WPARAM w, LPARAM l)
	{
		auto data = get_hwnd_data(hwnd);
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
	LRESULT window::wm_kill_focus(HWND hwnd, UINT msg, WPARAM w, LPARAM l)
	{
		auto data = get_hwnd_data(hwnd);
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

	LRESULT window::wm_paint(HWND hwnd, UINT msg, WPARAM w, LPARAM l)
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

	LRESULT window::wm_nc_calc_size(HWND hwnd, UINT msg, WPARAM w, LPARAM l)
	{
		if (static_cast<bool>(w))
			return 0;
		return ::DefWindowProcW(hwnd, msg, w, l);
	}
	LRESULT window::wm_nc_hit_test(HWND hwnd, UINT msg, WPARAM w, LPARAM l)
	{
		auto data = get_hwnd_data(hwnd);
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
	LRESULT window::wm_nc_activate(HWND hwnd, UINT msg, WPARAM w, LPARAM l)
	{
		return 0; // PREVENT BORDER FROM BEING DRAWN
	}

	LRESULT window::wm_create(HWND hwnd, UINT msg, WPARAM w, LPARAM l)
	{
		frost::api::window_description* desc = *reinterpret_cast<frost::api::window_description**>(l);
		window* result					= new window(hwnd);
		::SetWindowLongPtrW(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(result));
		result->_last_cursor_position	= {};
		result->_flags					= _flag_enabled | _flag_active | _flag_focused;
		result->_state					= fold_state(desc->state);
		result->_procedure				= desc->procedure;
		result->_data					= desc->data;
		result->_thread_id				= ::GetCurrentThreadId();
		result->_hkl					= ::GetKeyboardLayout(result->_thread_id);
		result->_rect = {
			desc->position.x,
			desc->position.y,
			desc->position.x + desc->size.width,
			desc->position.y + desc->size.height
		};

		/* TRACK MOUSE LEAVING CLIENT AREA */
		result->_track_mouse_event.cbSize = sizeof(_track_mouse_event);
		result->_track_mouse_event.hwndTrack = result->get_hwnd();
		result->_track_mouse_event.dwHoverTime = 0;
		result->_track_mouse_event.dwFlags = TME_LEAVE;

		/* INPUT HOOKS */
		RAWINPUTDEVICE rid[2] = {};
		for (auto& device : rid)
		{
			device.hwndTarget = result->get_hwnd();
			device.dwFlags = 0x00;
			device.usUsagePage = HID_USAGE_PAGE_GENERIC;
		}
		rid[0].usUsage = HID_USAGE_GENERIC_MOUSE;
		rid[1].usUsage = HID_USAGE_GENERIC_KEYBOARD;

		if (!::RegisterRawInputDevices(rid, sizeof(rid) / sizeof(*rid), sizeof(*rid)))
		{
			::DestroyWindow(result->get_hwnd());
			result->acquire_reference();
			result->release_reference();
			return ::DefWindowProcW(hwnd, msg, w, l);
		}

		/* LAYERED WINDOW SETUP */
		::SetLayeredWindowAttributes(result->get_hwnd(), 0, 255, 0);
		::ShowWindow(result->get_hwnd(), state_to_int(result->_state));
		auto data = get_hwnd_data(hwnd);
		
		/* EMIT EVENT */
		if (data->_procedure)
		{
			frost::api::window_event e;
			e.target = data;
			e.type = frost::api::window_event::event_type::create;
			data->_procedure(&e);
		}
		return ::DefWindowProcW(hwnd, msg, w, l);
	}
	LRESULT window::wm_close(HWND hwnd, UINT msg, WPARAM w, LPARAM l)
	{
		auto data = get_hwnd_data(hwnd);
		if (data->_procedure)
		{
			frost::api::window_event e;
			e.target = data;
			e.type = frost::api::window_event::event_type::close;
			data->_procedure(&e);
		}
		return 0;
	}
	LRESULT window::wm_destroy(HWND hwnd, UINT msg, WPARAM w, LPARAM l)
	{
		auto data = get_hwnd_data(hwnd);
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

	LRESULT window::wm_input_language_changed(HWND hwnd, UINT msg, WPARAM w, LPARAM l)
	{
		auto data = get_hwnd_data(hwnd);
		data->_hkl = ::GetKeyboardLayout(data->_thread_id);
		return ::DefWindowProcW(hwnd, msg, w, l);
	}

	LRESULT window::window_procedure(HWND hwnd, UINT msg, WPARAM w, LPARAM l)
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

	frost::impl::window* window::create(const frost::api::window_description* description)
	{
		auto hwnd = ::CreateWindowExW(
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
			const_cast<void*>(reinterpret_cast<const void*>(description)));

		if (hwnd == nullptr)
			return nullptr;
		
		LONG_PTR ptr = ::GetWindowLongPtrW(hwnd, GWLP_USERDATA);
		return reinterpret_cast<frost::impl::window*>(ptr);
	}
	void window::pump_messages()
	{
		MSG msg{};
		while (::GetMessageW(&msg, get_hwnd(), 0, 0))
		{
			if (msg.message == modify_deferred_msg)
			{
				execute_deferred_data* data = reinterpret_cast<execute_deferred_data*>(msg.wParam);
				HANDLE e = reinterpret_cast<HANDLE>(msg.lParam);

				data->procedure(data->target, data->p_argument);
				if (e != nullptr)
					SetEvent(e);	// Synchronous event
				else
					delete data;	// Fire and forget
				continue;
			}
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
}

#else
static_assert("PLATFORM NOT SUPPORTED!")
#endif
