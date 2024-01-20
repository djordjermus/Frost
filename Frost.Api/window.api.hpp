#include "keycode.api.hpp"
#include "resource.api.hpp"
#include "composites.hpp"
#pragma once
namespace frost::api
{
	enum class window_state : u8;
	struct window_description;
	struct window_event;
	using window_procedure_sig = void(_stdcall*)(window_event* target);
}
extern "C"
{
	FROST_API bool _stdcall frost_api_window_is_enabled(frost::api::resource* target);
	FROST_API bool _stdcall frost_api_window_is_active(frost::api::resource* target);
	FROST_API bool _stdcall frost_api_window_is_focused(frost::api::resource* target);
	
	FROST_API frost::api::window_state _stdcall frost_api_window_get_state(frost::api::resource* target);

	FROST_API bool _stdcall frost_api_window_get_key_state(frost::api::resource* target, frost::api::keycode key);
	
	FROST_API void _stdcall frost_api_window_get_position(frost::api::resource* target, point2d<i32>* output);
	FROST_API void _stdcall frost_api_window_get_size(frost::api::resource* target, size2d<i32>* output);
	
	FROST_API frost::api::window_procedure_sig _stdcall frost_api_window_get_procedure(frost::api::resource* target);
	FROST_API void* _stdcall frost_api_window_get_data(frost::api::resource* target);



	FROST_API void _stdcall frost_api_window_set_enabled(frost::api::resource* target, bool enabled);
	FROST_API void _stdcall frost_api_window_set_active(frost::api::resource* target, bool active);
	FROST_API void _stdcall frost_api_window_set_focused(frost::api::resource* target, bool focused);
	
	FROST_API void _stdcall frost_api_window_set_state(frost::api::resource* target, frost::api::window_state state);
	
	FROST_API void _stdcall frost_api_window_set_position(frost::api::resource* target, point2d<i32> position);
	FROST_API void _stdcall frost_api_window_set_size(frost::api::resource* target, size2d<i32> size);
	FROST_API void _stdcall frost_api_window_set_procedure(frost::api::resource* target, frost::api::window_procedure_sig procedure);
	FROST_API void _stdcall frost_api_window_set_data(frost::api::resource* target, void* data);



	FROST_API void _stdcall frost_api_window_pump_messages(frost::api::resource* target);

	FROST_API frost::api::resource* _stdcall frost_api_window_create(const frost::api::window_description* description);
	FROST_API bool _stdcall frost_api_resource_is_window(frost::api::resource* target);
}

namespace frost::api
{
	enum class window_state : u8
	{
		invalid		= 0xFF,
		hidden		= 0x00,
		minimized	= 0x01,
		normal		= 0x02,
		maximized	= 0x03
	};

	struct window_description
	{
	public:
		point2d<i32> position = { 200, 200 };
		size2d<i32> size = { 1280, 720 };

		window_procedure_sig procedure = nullptr;
		void* data = nullptr;

		window_state state = window_state::normal;
	};

	struct window_event
	{
	public:
		enum class event_type : u64;

		event_type type;
		resource* target;

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
			mouse_move		= 0x10, // TODO
			mouse_scroll	= 0x11, // TODO
			
			create			= 0x12,
			close			= 0x13,
			destroy			= 0x14,
		};
	};
	using window_procedure_sig = void(_stdcall*)(window_event* target);
}