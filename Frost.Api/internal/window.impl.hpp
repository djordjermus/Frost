#include "resource.impl.hpp"
#include "../window.api.hpp"
#pragma once
#if defined(TARGET_BUILD_PLATFORM_WINDOWS)
#include <windows.h>
namespace frost::impl
{
	class window final : public api::resource
	{
	private:
		window() = default;
		~window();

		static WORD _window_class;
		u64 _keystates[4] = {};
		RECT _rect = {};
		TRACKMOUSEEVENT _track_mouse_event = {};
		point2d<i32> _last_cursor_position = {};
		HWND _hwnd = nullptr;
		HKL _hkl = nullptr;
		u64 _flags = 0;

		frost::api::window_procedure_sig _procedure = nullptr;
		void* _data = nullptr;

		DWORD _thread_id = 0;

		frost::api::window_state _state = frost::api::window_state::invalid;
		
		constexpr static u64 _flag_enabled			= (1ull << 0);
		constexpr static u64 _flag_active			= (1ull << 1);
		constexpr static u64 _flag_focused			= (1ull << 2);
		constexpr static u64 _flag_cursor_inside	= (1ull << 3);

		void update_state();
		void update_rect();
		void update_last_cursor_position(LPARAM l);

		void set_key_state(u64 index);
		void reset_key_state(u64 index);

		static ATOM get_window_atom();

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

		static LRESULT window_procedure(HWND hwnd, UINT msg, WPARAM w, LPARAM l);

	public:

		bool is_enabled() const;
		bool is_active() const;
		bool is_focused() const;

		api::window_state get_state() const;
		bool get_key_state(u64 index);

		point2d<i32> get_position() const;
		size2d<i32> get_size() const;
		api::window_procedure_sig get_procedure() const;
		void* get_data() const;

		static window* create(const frost::api::window_description* description);
		void pump_messages();
	};
}
#else
static_assert("PLATFORM NOT SUPPORTED!")
#endif