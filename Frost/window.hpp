#include <string>
#include "macro.hpp"
#include "primitives.hpp"
#include "pimpl.hpp"
#pragma once
namespace frost
{
	class window final : public frost::pimpl_crtp<window>
	{
	public:
		enum class state : u8;
		enum class key : u8;
		struct description;
		class event_data;

		class api final
		{
		public:
			STATIC_CLASS(api);

			class modification_context;

			struct window_description;
			class window_event_data;

			using window_procedure_sig	= void(_stdcall*)(window_event_data* context);
			using window_modify_sig		= void(_stdcall*)(modification_context* context);



			static pimpl_t<window> FROST_API create(const window_description* description);

			static window_procedure_sig FROST_API get_procedure(pimpl_t<window> target);
			static bool FROST_API get_key_state(pimpl_t<window> target, u8 keycode);

			static bool FROST_API is_enabled(pimpl_t<window> target);
			static bool FROST_API is_active(pimpl_t<window> target);
			static bool FROST_API is_focused(pimpl_t<window> target);

			static bool FROST_API is_key_color_used(pimpl_t<window> target);
			static bool FROST_API get_key_color(pimpl_t<window> target);
			static f32 FROST_API get_opacity(pimpl_t<window> target);

			static u8 FROST_API get_state(pimpl_t<window> target);

			static i32  FROST_API get_x(pimpl_t<window> target);
			static i32  FROST_API get_y(pimpl_t<window> target);
			static i32  FROST_API get_width(pimpl_t<window> target);
			static i32  FROST_API get_height(pimpl_t<window> target);
	
			static i32  FROST_API get_caption_length(pimpl_t<window> target);
			static void FROST_API get_caption(pimpl_t<window> target, wchar_t* caption, i32 max_write);

			static void FROST_API set_procedure(pimpl_t<window> target, window_procedure_sig procedure);
			
			static void FROST_API destroy(pimpl_t<window> p_impl);



			static void FROST_API modify(pimpl_t<window> target, window_modify_sig modify_fn);
	
			static pimpl_t<window> FROST_API get_modification_target(modification_context* context);
			static void FROST_API set_enabled(modification_context* context, bool enabled);
			static void FROST_API set_active(modification_context* context, bool active);
			static void FROST_API set_focused(modification_context* context, bool focused);

			static void FROST_API set_key_color_used(modification_context* context, bool use_key);
			static void FROST_API set_key_color(modification_context* context, u32 rgba);
			static void FROST_API set_opacity(modification_context* context, f32 opacity);

			static void FROST_API set_state(modification_context* context, u8 state);

			static void FROST_API set_position(modification_context* context, i32 x, i32 y);
			static void FROST_API set_size(modification_context* context, i32 width, i32 height);

			static void FROST_API set_client_position(modification_context* context, i32 x, i32 y);
			static void FROST_API set_client_size(modification_context* context, i32 width, i32 height);

			static void FROST_API set_caption(modification_context* context, const wchar_t* caption);
	
			static void FROST_API prepare_message_queue();
			static void FROST_API message_pump();

			static constexpr u8 state_invalid	= 0xFF;
			static constexpr u8 state_hidden	= 0;
			static constexpr u8 state_minimized	= 1;
			static constexpr u8 state_normal	= 2;
			static constexpr u8 state_maximized	= 3;

			struct window_description final
			{
				i32 x					=  200;
				i32 y					=  200;
				i32 width				= 1280;
				i32 height				=  720;

				u8 state				= state_normal;

				const wchar_t* caption			= L"Frost application | Frost.Api v0.8.0a";

				window_procedure_sig procedure	= nullptr;
			};

			class window_event_data final
			{
			public:
				u64 type;
				pimpl_t<window> target;
				union
				{
					struct {} create;
					struct {} close;
					struct {} destroy;

					struct { bool enabled; } enable;
					struct { bool activated; } activate;
					struct { bool focused; } focus;
			
					struct { i32 x; i32 y; } move;
					struct { i32 width; i32 height; } resize;

					struct { wchar_t character; u8 key; } key_down;
					struct { wchar_t character; u8 key; } key_up;
					struct { f32 x; f32 y; u8 key; } double_click;

			
					struct { f32 x; f32 y; } cursor_enter;
					struct { f32 x; f32 y; } cursor_move;
					struct { f32 x; f32 y; } cursor_leave;
					struct { i32 delta_x; i32 delta_y; } mouse_move;
					struct { i32 delta; } scroll;
				};


			};
			static constexpr i32	event_type_create			= 1;
			static constexpr i32	event_type_close			= 2;
			static constexpr i32	event_type_destroy			= 3;

			static constexpr i32	event_type_enable			= 4;
			static constexpr i32	event_type_activate			= 5;
			static constexpr i32	event_type_focus			= 6;

			static constexpr i32	event_type_move				= 7;
			static constexpr i32	event_type_resize			= 8;

			static constexpr i32	event_type_key_down			= 9;
			static constexpr i32	event_type_key_up			= 10;
			static constexpr i32	event_type_double_click		= 11;

			static constexpr i32	event_type_cursor_enter		= 12;
			static constexpr i32	event_type_cursor_move		= 13;
			static constexpr i32	event_type_cursor_leave		= 14;
			static constexpr i32	event_type_mouse_move		= 16;
			static constexpr i32	event_type_cursor_scroll	= 17;

			static constexpr i32	keycode_null				= 0x00;

			static constexpr u8		keycode_mouse_left			= 0xe0;
			static constexpr u8		keycode_mouse_right			= 0xe1;
			static constexpr u8		keycode_mouse_middle		= 0xe2;
			static constexpr u8		keycode_mouse_x1			= 0xe3;
			static constexpr u8		keycode_mouse_x2			= 0xe4;

			static constexpr u8		keycode_escape				= 0x01;
			static constexpr u8		keycode_an_1				= 0x02;
			static constexpr u8		keycode_an_2				= 0x03;
			static constexpr u8		keycode_an_3				= 0x04;
			static constexpr u8		keycode_an_4				= 0x05;
			static constexpr u8		keycode_an_5				= 0x06;
			static constexpr u8		keycode_an_6				= 0x07;
			static constexpr u8		keycode_an_7				= 0x08;
			static constexpr u8		keycode_an_8				= 0x09;
			static constexpr u8		keycode_an_9				= 0x0a;
			static constexpr u8		keycode_an_0				= 0x0b;
			static constexpr u8		keycode_dash				= 0x0c;
			static constexpr u8		keycode_equals				= 0x0d;
			static constexpr u8		keycode_backspace			= 0x0e;

			static constexpr u8		keycode_tilde				= 0x29;
			static constexpr u8		keycode_q					= 0x10;
			static constexpr u8		keycode_w					= 0x11;
			static constexpr u8		keycode_e					= 0x12;
			static constexpr u8		keycode_r					= 0x13;
			static constexpr u8		keycode_t					= 0x14;
			static constexpr u8		keycode_y					= 0x15;
			static constexpr u8		keycode_u					= 0x16;
			static constexpr u8		keycode_i					= 0x17;
			static constexpr u8		keycode_o					= 0x18;
			static constexpr u8		keycode_p					= 0x19;
			static constexpr u8		keycode_bracket_open		= 0x1a;
			static constexpr u8		keycode_bracket_close		= 0x1b;
			static constexpr u8		keycode_a					= 0x1e;
			static constexpr u8		keycode_s					= 0x1f;
			static constexpr u8		keycode_d					= 0x20;
			static constexpr u8		keycode_f					= 0x21;
			static constexpr u8		keycode_g					= 0x22;
			static constexpr u8		keycode_h					= 0x23;
			static constexpr u8		keycode_j					= 0x24;
			static constexpr u8		keycode_k					= 0x25;
			static constexpr u8		keycode_l					= 0x26;
			static constexpr u8		keycode_semicolon			= 0x27;
			static constexpr u8		keycode_quote				= 0x28;
			static constexpr u8		keycode_z					= 0x2c;
			static constexpr u8		keycode_x					= 0x2d;
			static constexpr u8		keycode_c					= 0x2e;
			static constexpr u8		keycode_backslash			= 0x2b;
			static constexpr u8		keycode_v					= 0x2f;
			static constexpr u8		keycode_b					= 0x30;
			static constexpr u8		keycode_n					= 0x31;
			static constexpr u8		keycode_m					= 0x32;
			static constexpr u8		keycode_comma				= 0x33;
			static constexpr u8		keycode_period				= 0x34;
			static constexpr u8		keycode_slash				= 0x35;

			static constexpr u8		keycode_f1					= 0x3b;
			static constexpr u8		keycode_f2					= 0x3c;
			static constexpr u8		keycode_f3					= 0x3d;
			static constexpr u8		keycode_f4					= 0x3e;
			static constexpr u8		keycode_f5					= 0x3f;
			static constexpr u8		keycode_f6					= 0x40;
			static constexpr u8		keycode_f7					= 0x41;
			static constexpr u8		keycode_f8					= 0x42;
			static constexpr u8		keycode_f9					= 0x43;
			static constexpr u8		keycode_f10					= 0x44;
			static constexpr u8		keycode_f11					= 0x57;
			static constexpr u8		keycode_f12					= 0x58;

			static constexpr u8		keycode_tab					= 0x0F;
			static constexpr u8		keycode_capslock			= 0x3a;
			static constexpr u8		keycode_left_shift			= 0x2a;
			static constexpr u8		keycode_right_shift			= 0x36;
			static constexpr u8		keycode_left_ctrl			= 0x1d;
			static constexpr u8		keycode_right_ctrl			= 0x9d;
			static constexpr u8		keycode_left_win			= 0xdb;
			static constexpr u8		keycode_right_win			= 0xdd;
			static constexpr u8		keycode_left_alt			= 0x38;
			static constexpr u8		keycode_right_alt			= 0xb8;

			static constexpr u8		keycode_space				= 0x39;
			static constexpr u8		keycode_enter				= 0x1c;

			static constexpr u8		keycode_left_arrow			= 0xcb;
			static constexpr u8		keycode_up_arrow			= 0xc8;
			static constexpr u8		keycode_right_arrow			= 0xcd;
			static constexpr u8		keycode_down_arrow			= 0xd0;

			static constexpr u8		keycode_numpad_0			= 0x52;
			static constexpr u8		keycode_numpad_1			= 0x4f;
			static constexpr u8		keycode_numpad_2			= 0x50;
			static constexpr u8		keycode_numpad_3			= 0x51;
			static constexpr u8		keycode_numpad_4			= 0x4b;
			static constexpr u8		keycode_numpad_5			= 0x4c;
			static constexpr u8		keycode_numpad_6			= 0x4d;
			static constexpr u8		keycode_numpad_7			= 0x47;
			static constexpr u8		keycode_numpad_8			= 0x48;
			static constexpr u8		keycode_numpad_9			= 0x49;
			static constexpr u8		keycode_numpad_numlock		= 0x45;
			static constexpr u8		keycode_numpad_divide		= 0xb5;
			static constexpr u8		keycode_numpad_multiply		= 0x37;
			static constexpr u8		keycode_numpad_minus		= 0x4a;
			static constexpr u8		keycode_numpad_plus			= 0x4e;
			static constexpr u8		keycode_numpad_enter		= 0x9c;
			static constexpr u8		keycode_numpad_separator	= 0x53;

			static constexpr u8		keycode_print_screen		= 0xaa;
			static constexpr u8		keycode_scroll_lock			= 0x46;
			static constexpr u8		keycode_pause				= 0x9d;

			static constexpr u8		keycode_insert				= 0xd2;
			static constexpr u8		keycode_home				= 0xc7;
			static constexpr u8		keycode_page_up				= 0xc9;
			static constexpr u8		keycode_delete				= 0xd3;
			static constexpr u8		keycode_end					= 0xcf;
			static constexpr u8		keycode_page_down			= 0xd1;
		};

		enum class state : u8
		{
			invalid		= api::state_invalid,
			hidden		= api::state_hidden,
			minimized	= api::state_minimized,
			normal		= api::state_normal,
			maximized	= api::state_maximized,
		};
		
		enum class key : u8
		{
			null				=	api::keycode_null,
			
			mouse_left			=	api::keycode_mouse_left,
			mouse_right			=	api::keycode_mouse_right,
			mouse_middle		=	api::keycode_mouse_middle,
			mouse_x1			=	api::keycode_mouse_x1,
			mouse_x2			=	api::keycode_mouse_x2,
			
			escape				=	api::keycode_escape,
			an_1				=	api::keycode_an_1,
			an_2				=	api::keycode_an_2,
			an_3				=	api::keycode_an_3,
			an_4				=	api::keycode_an_4,
			an_5				=	api::keycode_an_5,
			an_6				=	api::keycode_an_6,
			an_7				=	api::keycode_an_7,
			an_8				=	api::keycode_an_8,
			an_9				=	api::keycode_an_9,
			an_0				=	api::keycode_an_0,
			dash				=	api::keycode_dash,
			equals				=	api::keycode_equals,
			backspace			=	api::keycode_backspace,
			
			tilde				=	api::keycode_tilde,
			q					=	api::keycode_q,
			w					=	api::keycode_w,
			e					=	api::keycode_e,
			r					=	api::keycode_r,
			t					=	api::keycode_t,
			y					=	api::keycode_y,
			u					=	api::keycode_u,
			i					=	api::keycode_i,
			o					=	api::keycode_o,
			p					=	api::keycode_p,
			bracket_open		=	api::keycode_bracket_open,
			bracket_close		=	api::keycode_bracket_close,
			a					=	api::keycode_a,
			s					=	api::keycode_s,
			d					=	api::keycode_d,
			f					=	api::keycode_f,
			g					=	api::keycode_g,
			h					=	api::keycode_h,
			j					=	api::keycode_j,
			k					=	api::keycode_k,
			l					=	api::keycode_l,
			semicolon			=	api::keycode_semicolon,
			quote				=	api::keycode_quote,
			z					=	api::keycode_z,
			x					=	api::keycode_x,
			c					=	api::keycode_c,
			backslash			=	api::keycode_backslash,
			v					=	api::keycode_v,
			b					=	api::keycode_b,
			n					=	api::keycode_n,
			m					=	api::keycode_m,
			comma				=	api::keycode_comma,
			period				=	api::keycode_period,
			slash				=	api::keycode_slash,
			
			f1					=	api::keycode_f1,
			f2					=	api::keycode_f2,
			f3					=	api::keycode_f3,
			f4					=	api::keycode_f4,
			f5					=	api::keycode_f5,
			f6					=	api::keycode_f6,
			f7					=	api::keycode_f7,
			f8					=	api::keycode_f8,
			f9					=	api::keycode_f9,
			f10					=	api::keycode_f10,
			f11					=	api::keycode_f11,
			f12					=	api::keycode_f12,
			
			tab					=	api::keycode_tab,
			capslock			=	api::keycode_capslock,
			left_shift			=	api::keycode_left_shift,
			right_shift			=	api::keycode_right_shift,
			left_ctrl			=	api::keycode_left_ctrl,
			right_ctrl			=	api::keycode_right_ctrl,
			left_win			=	api::keycode_left_win,
			right_win			=	api::keycode_right_win,
			left_alt			=	api::keycode_left_alt,
			right_alt			=	api::keycode_right_alt,
			
			space				=	api::keycode_space,
			enter				=	api::keycode_enter,
			
			left_arrow			=	api::keycode_left_arrow,
			up_arrow			=	api::keycode_up_arrow,
			right_arrow			=	api::keycode_right_arrow,
			down_arrow			=	api::keycode_down_arrow,
			
			numpad_0			=	api::keycode_numpad_0,
			numpad_1			=	api::keycode_numpad_1,
			numpad_2			=	api::keycode_numpad_2,
			numpad_3			=	api::keycode_numpad_3,
			numpad_4			=	api::keycode_numpad_4,
			numpad_5			=	api::keycode_numpad_5,
			numpad_6			=	api::keycode_numpad_6,
			numpad_7			=	api::keycode_numpad_7,
			numpad_8			=	api::keycode_numpad_8,
			numpad_9			=	api::keycode_numpad_9,
			numpad_numlock		=	api::keycode_numpad_numlock,
			numpad_divide		=	api::keycode_numpad_divide,
			numpad_multiply		=	api::keycode_numpad_multiply,
			numpad_minus		=	api::keycode_numpad_minus,
			numpad_plus			=	api::keycode_numpad_plus,
			numpad_enter		=	api::keycode_numpad_enter,
			numpad_separator	=	api::keycode_numpad_separator,
			
			print_screen		=	api::keycode_print_screen,
			scroll_lock			=	api::keycode_scroll_lock,
			pause				=	api::keycode_pause,
			
			insert				=	api::keycode_insert,
			home				=	api::keycode_home,
			page_up				=	api::keycode_page_up,
			del					=	api::keycode_delete,
			end					=	api::keycode_end,
			page_down			=	api::keycode_page_down,
		};
		
		struct description final
		{
		public:
			i32 x					=	200;
			i32 y					=	200;
			i32 width				=	1280;
			i32 height				=	720;

			state state				= state::normal;

			std::wstring caption	= L"Frost application | Frost.Api v0.8.0a";

			api::window_procedure_sig procedure	= nullptr; // TODO REPLACE
		};
	};
}
