#include "primitives.hpp"
#include "macro.hpp"

#include "enum/keycode.hpp"
#include "enum/log_level.hpp"
#include "enum/object_type.hpp"
#include "enum/window_state.hpp"
#include "enum/window_event_type.hpp"

#include "function/event_system_handler.hpp"
#include "function/event_system_relay.hpp"
#include "function/window_procedure.hpp"

#include "struct/point.hpp"
#include "struct/size.hpp"
#include "struct/version.hpp"
#include "struct/log_parameter.hpp"
#include "struct/log_event_data.hpp"
#include "struct/window_description.hpp"
#include "struct/window_event.hpp"

#pragma once


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
	FROST_API u64  _stdcall frost_api_object_get_reference_count(frost::api::object* target);
	FROST_API void _stdcall frost_api_object_acquire_reference(frost::api::object* target);
	FROST_API void _stdcall frost_api_object_release_reference(frost::api::object* target);
}

extern "C" // VERSIONING API 
{
	FROST_API void _stdcall frost_api_version_get_api_version(frost::api::version* output);
}

extern "C" // SYNCHRONIZABLE API
{
	FROST_API bool _stdcall  frost_api_synchronizable_wait(frost::api::object* target);
	FROST_API bool _stdcall  frost_api_synchronizable_try_wait(frost::api::object* target);
	FROST_API bool _stdcall  frost_api_synchronizable_signal(frost::api::object* target);

	FROST_API bool _stdcall frost_api_synchronizable_event_reset(frost::api::object* target);

	FROST_API i32  _stdcall frost_api_synchronizable_wait_one(frost::api::object* const* target_list, i32 count);
	FROST_API bool _stdcall frost_api_synchronizable_wait_all(frost::api::object* const* target_list, i32 count);
	FROST_API i32  _stdcall frost_api_synchronizable_try_wait_one(frost::api::object* const* target_list, i32 count);
	FROST_API bool _stdcall frost_api_synchronizable_try_wait_all(frost::api::object* const* target_list, i32 count);

	FROST_API frost::api::object* _stdcall frost_api_synchronizable_create_mutex(bool initial_owner);
	FROST_API frost::api::object* _stdcall frost_api_synchronizable_create_semaphore(i32 count, i32 max);
	FROST_API frost::api::object* _stdcall frost_api_synchronizable_create_event();
}

extern "C" // SYSTEM RESOURCE API
{
	FROST_API void* _stdcall frost_api_system_handle_host_get_system_handle(frost::api::object* target);
}

extern "C" // THREADING API
{
	FROST_API frost::api::object* _stdcall frost_api_thread_create(void(_stdcall* procedure)(void*), void* argument);
	FROST_API frost::api::object* _stdcall frost_api_thread_get_current();

	FROST_API frost::api::object* _stdcall frost_api_thread_message_create();
	FROST_API void _stdcall frost_api_thread_message_dispatch(frost::api::object* message);
	FROST_API void _stdcall frost_api_thread_message_discard(frost::api::object* message);

	FROST_API bool _stdcall frost_api_thread_message_send(
		frost::api::object* thread,
		void(_stdcall* procedure)(void*),
		void* argument);
	FROST_API bool _stdcall frost_api_thread_message_send_async(
		frost::api::object* thread,
		frost::api::object* sync,
		void(_stdcall* procedure)(void*),
		void* argument);
}

extern "C" // WINDOW API
{
	FROST_API bool _stdcall frost_api_window_is_enabled(frost::api::object* target);
	FROST_API bool _stdcall frost_api_window_is_active(frost::api::object* target);
	FROST_API bool _stdcall frost_api_window_is_focused(frost::api::object* target);
	
	FROST_API frost::api::window_state _stdcall frost_api_window_get_state(frost::api::object* target);

	FROST_API bool _stdcall frost_api_window_get_key_state(frost::api::object* target, frost::api::keycode key);
	
	FROST_API void _stdcall frost_api_window_get_position(frost::api::object* target, frost::api::point2d<i32>* output);
	FROST_API void _stdcall frost_api_window_get_size(frost::api::object* target, frost::api::size2d<i32>* output);
	
	FROST_API frost::api::window_procedure_sig _stdcall frost_api_window_get_procedure(frost::api::object* target);
	FROST_API void* _stdcall frost_api_window_get_data(frost::api::object* target);



	FROST_API void _stdcall frost_api_window_set_enabled(frost::api::object* target, bool enabled);
	FROST_API void _stdcall frost_api_window_set_active(frost::api::object* target, bool active);
	FROST_API void _stdcall frost_api_window_set_focused(frost::api::object* target, bool focused);
	
	FROST_API void _stdcall frost_api_window_set_state(frost::api::object* target, frost::api::window_state state);
	
	FROST_API void _stdcall frost_api_window_set_position(frost::api::object* target, frost::api::point2d<i32> position);
	FROST_API void _stdcall frost_api_window_set_size(frost::api::object* target, frost::api::size2d<i32> size);
	FROST_API void _stdcall frost_api_window_set_procedure(frost::api::object* target, frost::api::window_procedure_sig procedure);
	FROST_API void _stdcall frost_api_window_set_data(frost::api::object* target, void* data);



	FROST_API frost::api::object* _stdcall frost_api_window_create(const frost::api::window_description* description);
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

extern "C" // BLOB API
{
	FROST_API const void* _stdcall frost_api_blob_get_pointer(frost::api::object* target);
}
