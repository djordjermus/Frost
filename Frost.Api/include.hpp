#pragma once
#if defined(FROST_API_BUILD_DLL)
#define FROST_API __declspec(dllexport)
#else
#define FROST_API __declspec(dllimport)
#endif

/*
 * PLATFORM SWITCH
 */
#if defined(_WIN32) || defined(_WIN64)
#define TARGET_BUILD_PLATFORM_WINDOWS
#endif

/*
 * DEBUG
 */
#if _DEBUG
#define FROST_API_DEBUG_LOG_RESOURCE_CREATION 1
#define FROST_API_DEBUG_LOG_RESOURCE_DESTRUCTION 1
#endif

/*
 * UTILITY
 */

#define STATIC_CLASS(CLASS)\
CLASS() = delete;\
CLASS(CLASS&) = delete;\
CLASS(CLASS&&) noexcept = delete;\
CLASS(CLASS&&) noexcept = delete;\
~CLASS() = delete;

#define NO_COPY_SEMANTICS(CLASS)\
CLASS(CLASS&) = delete;\
CLASS& operator=(CLASS&) = delete;

#define NO_MOVE_SEMANTICS(CLASS)\
CLASS(CLASS&&) noexcept = delete;\
CLASS& operator=(CLASS&&) noexcept = delete;

using u8 = unsigned char;
using i8 = signed char;

using u16 = unsigned short;
using i16 = signed short;

using u32 = unsigned long;
using i32 = signed long;

using u64 = unsigned long long;
using i64 = signed long long;

using f32 = float;
using f64 = double;

template<class T = void>
using handle = T*;

/* ASSERTIONS */

static_assert(sizeof(u8) == 1, "u8 must be an unsigned 8bit integer!");
static_assert(sizeof(i8) == 1, "i8 must be an signed 8bit integer!");

static_assert(sizeof(u16) == 2, "u16 must be an unsigned 16bit integer!");
static_assert(sizeof(i16) == 2, "i16 must be an signed 16bit integer!");

static_assert(sizeof(u32) == 4, "u32 must be an unsigned 32bit integer!");
static_assert(sizeof(i32) == 4, "i32 must be an signed 32bit integer!");

static_assert(sizeof(u64) == 8, "u64 must be an unsigned 64bit integer!");
static_assert(sizeof(i64) == 8, "i64 must be an signed 64bit integer!");

static_assert(sizeof(f32) == 4, "f32 must be an 32bit floating point number!");
static_assert(sizeof(f64) == 8, "f64 must be an 64bit floating point number!");

namespace frost::api
{
	/* FORWARD DECLARATION */

	enum class keycode : u8;
	enum class window_state : u8;
	enum class log_level : u8;

	struct window_description;
	struct window_event;

	class resource;



	/* ENUM DEFINITIONS */

	enum class keycode : u8
	{
		null				= 0x00,

		mouse_left			= 0xe0,
		mouse_right			= 0xe1,
		mouse_middle		= 0xe2,
		mouse_x1			= 0xe3,
		mouse_x2			= 0xe4,

		escape				= 0x01,
		an_1				= 0x02,
		an_2				= 0x03,
		an_3				= 0x04,
		an_4				= 0x05,
		an_5				= 0x06,
		an_6				= 0x07,
		an_7				= 0x08,
		an_8				= 0x09,
		an_9				= 0x0a,
		an_0				= 0x0b,
		dash				= 0x0c,
		equals				= 0x0d,
		backspace			= 0x0e,

		tilde				= 0x29,
		q					= 0x10,
		w					= 0x11,
		e					= 0x12,
		r					= 0x13,
		t					= 0x14,
		y					= 0x15,
		u					= 0x16,
		i					= 0x17,
		o					= 0x18,
		p					= 0x19,
		bracket_open		= 0x1a,
		bracket_close		= 0x1b,
		a					= 0x1e,
		s					= 0x1f,
		d					= 0x20,
		f					= 0x21,
		g					= 0x22,
		h					= 0x23,
		j					= 0x24,
		k					= 0x25,
		l					= 0x26,
		semicolon			= 0x27,
		quote				= 0x28,
		z					= 0x2c,
		x					= 0x2d,
		c					= 0x2e,
		backslash			= 0x2b,
		v					= 0x2f,
		b					= 0x30,
		n					= 0x31,
		m					= 0x32,
		comma				= 0x33,
		period				= 0x34,
		slash				= 0x35,

		f1					= 0x3b,
		f2					= 0x3c,
		f3					= 0x3d,
		f4					= 0x3e,
		f5					= 0x3f,
		f6					= 0x40,
		f7					= 0x41,
		f8					= 0x42,
		f9					= 0x43,
		f10					= 0x44,
		f11					= 0x57,
		f12					= 0x58,

		tab					= 0x0F,
		capslock			= 0x3a,
		left_shift			= 0x2a,
		right_shift			= 0x36,
		left_ctrl			= 0x1d,
		right_ctrl			= 0x76,
		left_win			= 0xB4,
		right_win			= 0xB6,
		left_alt			= 0x38,
		right_alt			= 0x91,

		space				= 0x39,
		enter				= 0x1c,

		left_arrow			= 0xA4,
		up_arrow			= 0xA1,
		right_arrow			= 0xA6,
		down_arrow			= 0xA9,

		numpad_0			= 0x52,
		numpad_1			= 0x4f,
		numpad_2			= 0x50,
		numpad_3			= 0x51,
		numpad_4			= 0x4b,
		numpad_5			= 0x4c,
		numpad_6			= 0x4d,
		numpad_7			= 0x47,
		numpad_8			= 0x48,
		numpad_9			= 0x49,
		numpad_numlock		= 0x45,
		numpad_divide		= 0x8E,
		numpad_multiply		= 0x37,
		numpad_minus		= 0x4a,
		numpad_plus			= 0x4e,
		numpad_enter		= 0x75,
		numpad_separator	= 0x53,

		print_screen		= 0x83,
		scroll_lock			= 0x46,
		pause				= 0xD3,

		insert				= 0xAB,
		home				= 0xA0,
		page_up				= 0xA2,
		del					= 0xAC,
		end					= 0xA8,
		page_down			= 0xAA,
	};

	enum class window_state : u8
	{
		invalid		= 0xFF,
		hidden		= 0x00,
		minimized	= 0x01,
		normal		= 0x02,
		maximized	= 0x03
	};

	enum class log_level : u8
	{
		trace		= 1,
		debug		= 2,
		information	= 4,
		warning		= 8,
		error		= 16,
		fatal		= 32,
	};
	


	/* FUNCTION POINTER DEFINITIONS */

	typedef void(_stdcall* event_system_handler)(void* p_data);

	typedef void(_stdcall* event_system_relay)(u64 tag, u64 layer, void* p_data);

	using window_procedure_sig = void(_stdcall*)(window_event* target);

	/* STRUCT DEFINITIONS */

	template<class T>
	struct point2d
	{
	public:
		T x;
		T y;
	};

	template<class T>
	struct size2d
	{
	public:
		T width;
		T height;
	};

	struct window_description final
	{
	public:
		point2d<i32> position = { 200, 200 };
		size2d<i32> size = { 1280, 720 };

		window_procedure_sig procedure = nullptr;
		void* data = nullptr;

		window_state state = window_state::normal;
	};

	struct window_event final
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
			mouse_move		= 0x10,
			mouse_scroll	= 0x11,
			
			create			= 0x12,
			close			= 0x13,
			destroy			= 0x14,
		};
	};

	struct semantic_version final
	{
		u16 major_version;
		u16 minor_version;
		u32 patch;
		const wchar_t* decoration;
	};


	class log_parameter final
	{
	public:
		const wchar_t* name;
		const wchar_t* value;
		u64 name_length;
		u64 value_length;
	};

	class log_event_data final
	{
	public:
		const wchar_t* message_template;
		u64 message_template_length;
		const wchar_t* message;
		u64 message_length;

		const frost::api::log_parameter* parameters;
		u64 parameter_count;
		log_level level;
	};
}

extern "C" // CLOCK API
{
	FROST_API u64 _stdcall frost_api_clock_get_frequency();
	FROST_API f64 _stdcall frost_api_clock_get_period();
	FROST_API u64 _stdcall frost_api_clock_get_timestamp();
}

extern "C" // COLOR API
{
	FROST_API void _stdcall frost_api_color_rgba8_to_rgba32(const u32 input, f32* output);
	FROST_API void _stdcall frost_api_color_rgba32_to_rgba8(const f32* input, u32* output);

	FROST_API void _stdcall frost_api_color_rgba32_to_hsva(const f32* input, f32* output);
	FROST_API void _stdcall frost_api_color_rgba32_to_hsla(const f32* input, f32* output);
	FROST_API void _stdcall frost_api_color_rgba32_to_cmyk(const f32* input, f32* output);

	FROST_API void _stdcall frost_api_color_hsva_to_rgba32(const f32* input, f32* output);
	FROST_API void _stdcall frost_api_color_hsla_to_rgba32(const f32* input, f32* output);
	FROST_API void _stdcall frost_api_color_cmyk_to_rgba32(const f32* input, f32* output);
}

extern "C" // EVENT SYSTEM API
{
	FROST_API u64 _stdcall frost_api_event_system_get_api_broadcast_layer();
	FROST_API void _stdcall frost_api_event_system_emit(u64 tag, u64 layer, void* p_data);

	FROST_API void _stdcall frost_api_event_system_subscribe(u64 tag, u64 activation_layers, frost::api::event_system_handler handler);
	FROST_API void _stdcall frost_api_event_system_unsubscribe(u64 tag, u64 activation_layers, frost::api::event_system_handler handler);

	FROST_API void _stdcall frost_api_event_system_subscribe_relay(frost::api::event_system_relay relay);
	FROST_API void _stdcall frost_api_event_system_unsubscribe_relay(frost::api::event_system_relay relay);
}

extern "C" // LOGGING API
{
		/// <summary>
	/// Returns the event tag associated with log events
	/// </summary>
	/// <returns></returns>
	FROST_API u64 _stdcall frost_api_logging_get_log_event_tag();

	/// <summary>
	/// Attempts to renders the message to output. 
	/// Function will terminate the output message with '\0' if there is enough space.
	/// Parameters in the message template are in format {@MyParameter}.
	/// First character of the parameter name must be a letter, rest can be alphanumeric characters.
	/// </summary>
	/// <param name="message_template">Message template with parameter placeholders such as {@MyParameter1}.</param>
	/// <param name="message_template_length">Length of the message template, '\0' not included.</param>
	/// <param name="parameters">Array of parameter definitions.</param>
	/// <param name="parameter_count">Number of parameter definitions in the array.</param>
	/// <param name="output">pointer to the output buffer. may be set to null, if output_length is zero.</param>
	/// <param name="output_length">Length in characters of the output buffer.
	/// Zero if only the message length is needed, and rendering is not.</param>
	/// <returns>Number of characters needed to fully render the message (null terminator not included).</returns>
	FROST_API u64 _stdcall frost_api_logging_render_message(
		const wchar_t* message_template,
		u64 message_template_length,
		const frost::api::log_parameter* parameters,
		u64 parameter_count,
		wchar_t* output,
		u64 output_length);

	/// <summary>
	/// Emits an frost::api::log_event_data event, created with given parameters.
	/// Function will terminate the log event data message with '\0' only if there is enough space.
	/// Parameters in the message template are in format {@MyParameter}.
	/// First character of the parameter name must be a letter, rest can be alphanumeric characters.
	/// </summary>
	/// <param name="activation_layers">Layers on which event will be emitted on.</param>
	/// <param name="message_template">Message template with parameter placeholders such as {@MyParameter1}.</param>
	/// <param name="message_template_length">Length of the message template, '\0' not included.</param>
	/// <param name="parameters">Array of parameter definitions.</param>
	/// <param name="parameter_count">Number of parameter definitions in the array.</param>
	/// <param name="level">Log level of the emmited event.</param>
	FROST_API void _stdcall frost_api_logging_log(
		u64 activation_layers,
		const wchar_t* message_template,
		u64 message_template_length,
		const frost::api::log_parameter* parameters,
		u64 parameter_count,
		frost::api::log_level level);
}

extern "C" // RANDOM API
{
	FROST_API u64 _stdcall frost_api_random_generate(u64* p_seed);
	FROST_API u64 _stdcall frost_api_random_generate_range(u64* p_seed, u64 min, u64 range);
	FROST_API f64 _stdcall frost_api_random_generate_range_double(u64* p_seed, f64 min, f64 range);
}

extern "C" // RESOURCE API
{
	FROST_API u64  _stdcall frost_api_resource_get_reference_count(frost::api::resource* target);
	FROST_API void _stdcall frost_api_resource_acquire_reference(frost::api::resource* target);
	FROST_API void _stdcall frost_api_resource_release_reference(frost::api::resource* target);
}

extern "C" // VERSIONING API 
{
	FROST_API bool _stdcall frost_api_semantic_version_check_compatibility(frost::api::semantic_version version, frost::api::semantic_version target);
	FROST_API void _stdcall frost_api_semantic_version_get_api_version(frost::api::semantic_version* output);
}

extern "C" // SYNCHRONIZABLE API
{
	FROST_API bool _stdcall  frost_api_synchronizable_wait(frost::api::resource* target);
	FROST_API bool _stdcall  frost_api_synchronizable_try_wait(frost::api::resource* target);
	FROST_API bool _stdcall  frost_api_synchronizable_signal(frost::api::resource* target);

	FROST_API bool _stdcall frost_api_synchronizable_event_reset(frost::api::resource* target);

	FROST_API i32  _stdcall frost_api_synchronizable_wait_one(frost::api::resource* const* target_list, i32 count);
	FROST_API bool _stdcall frost_api_synchronizable_wait_all(frost::api::resource* const* target_list, i32 count);
	FROST_API i32  _stdcall frost_api_synchronizable_try_wait_one(frost::api::resource* const* target_list, i32 count);
	FROST_API bool _stdcall frost_api_synchronizable_try_wait_all(frost::api::resource* const* target_list, i32 count);

	FROST_API frost::api::resource* _stdcall frost_api_synchronizable_create_mutex(bool initial_owner);
	FROST_API frost::api::resource* _stdcall frost_api_synchronizable_create_semaphore(i32 count, i32 max);
	FROST_API frost::api::resource* _stdcall frost_api_synchronizable_create_event();

	FROST_API bool _stdcall frost_api_resource_is_synchronizable(frost::api::resource* target);
}

extern "C" // SYSTEM RESOURCE API
{
	FROST_API void* _stdcall frost_api_system_resource_get_system_handle(frost::api::resource* target);
}

extern "C" // THREADING API
{
	FROST_API frost::api::resource* _stdcall frost_api_thread_create(void(_stdcall* procedure)(void*), void* argument);
	FROST_API frost::api::resource* _stdcall frost_api_thread_get_current();

	FROST_API frost::api::resource* _stdcall frost_api_thread_message_create();
	FROST_API void _stdcall frost_api_thread_message_wait(frost::api::resource* message);
	FROST_API void _stdcall frost_api_thread_message_peek(frost::api::resource* message);
	FROST_API void _stdcall frost_api_thread_message_dispatch(frost::api::resource* message);
	FROST_API void _stdcall frost_api_thread_message_discard(frost::api::resource* message);

	FROST_API bool _stdcall frost_api_thread_message_send(
		frost::api::resource* thread,
		void(_stdcall* procedure)(void*),
		void* argument);
	FROST_API bool _stdcall frost_api_thread_message_send_async(
		frost::api::resource* thread,
		frost::api::resource* sync,
		void(_stdcall* procedure)(void*),
		void* argument);
}

extern "C" // WINDOW API
{
	FROST_API bool _stdcall frost_api_window_is_enabled(frost::api::resource* target);
	FROST_API bool _stdcall frost_api_window_is_active(frost::api::resource* target);
	FROST_API bool _stdcall frost_api_window_is_focused(frost::api::resource* target);
	
	FROST_API frost::api::window_state _stdcall frost_api_window_get_state(frost::api::resource* target);

	FROST_API bool _stdcall frost_api_window_get_key_state(frost::api::resource* target, frost::api::keycode key);
	
	FROST_API void _stdcall frost_api_window_get_position(frost::api::resource* target, frost::api::point2d<i32>* output);
	FROST_API void _stdcall frost_api_window_get_size(frost::api::resource* target, frost::api::size2d<i32>* output);
	
	FROST_API frost::api::window_procedure_sig _stdcall frost_api_window_get_procedure(frost::api::resource* target);
	FROST_API void* _stdcall frost_api_window_get_data(frost::api::resource* target);



	FROST_API void _stdcall frost_api_window_set_enabled(frost::api::resource* target, bool enabled);
	FROST_API void _stdcall frost_api_window_set_active(frost::api::resource* target, bool active);
	FROST_API void _stdcall frost_api_window_set_focused(frost::api::resource* target, bool focused);
	
	FROST_API void _stdcall frost_api_window_set_state(frost::api::resource* target, frost::api::window_state state);
	
	FROST_API void _stdcall frost_api_window_set_position(frost::api::resource* target, frost::api::point2d<i32> position);
	FROST_API void _stdcall frost_api_window_set_size(frost::api::resource* target, frost::api::size2d<i32> size);
	FROST_API void _stdcall frost_api_window_set_procedure(frost::api::resource* target, frost::api::window_procedure_sig procedure);
	FROST_API void _stdcall frost_api_window_set_data(frost::api::resource* target, void* data);



	FROST_API frost::api::resource* _stdcall frost_api_window_create(const frost::api::window_description* description);
	FROST_API bool _stdcall frost_api_resource_is_window(frost::api::resource* target);
}

extern "C" // KEYCODE API
{
	FROST_API u64 _stdcall frost_api_keycode_to_wcs(
		frost::api::keycode keycode,
		wchar_t* output,
		u64 output_length,
		bool normalize_case);

	FROST_API const wchar_t* _stdcall frost_api_keycode_get_name(frost::api::keycode keycode);
}

extern "C"
{

}
