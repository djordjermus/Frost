#include "primitives.hpp"
#include "macro.hpp"
#pragma once
namespace frost::api
{
	enum class log_level : u8
	{
		trace		= 1,
		debug		= 2,
		information	= 4,
		warning		= 8,
		error		= 16,
		fatal		= 32,
	};

	class log_parameter
	{
	public:
		const wchar_t* name;
		const wchar_t* value;
		u64 name_length;
		u64 value_length;
	};

	class log_event_data
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
extern "C"
{
	struct log_event;


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
