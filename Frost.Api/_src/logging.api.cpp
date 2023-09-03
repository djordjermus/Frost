#include "../logging.api.hpp"
#include "../event_system.api.hpp"
#include <cctype>
#include <wchar.h>
#include <typeinfo>

static void log_internal(
	const wchar_t* message_template,
	u64 template_length,
	const wchar_t** params,
	const u64* param_lengths,
	u64 param_count,
	u64 activation_layers,
	u8 level);

static u64 get_thread_id();
static u64 get_timestamp();
static u64 get_param_index(const wchar_t* marker, u64 max_read, u64* consumed);
static u64 render_message(
	const wchar_t* message_template,
	u64 template_length,
	const wchar_t** parameters,
	const u64* parameter_lengths,
	u64 parameter_count,
	wchar_t* message,
	u64 message_length);
static u64 render_message_v2(
	const wchar_t* message_template,
	u64 template_length,
	const wchar_t** parameters,
	const u64* parameter_lengths,
	u64 parameter_count,
	wchar_t* message,
	u64 message_length);

extern "C"
{
	FROST_API u64 _stdcall logging_get_log_event_tag()
	{
		return typeid(log_event).hash_code();
	}

	FROST_API void _stdcall logging_log_verbose(
		const wchar_t* message_template,
		u64 template_length,
		const wchar_t** params,
		const u64* param_lengths,
		u64 param_count,
		u64 activation_layers)
	{
		log_internal(message_template, template_length, params, param_lengths, param_count, activation_layers, 1);
	}

	FROST_API void _stdcall logging_log_debug(
		const wchar_t* message_template,
		u64 template_length,
		const wchar_t** params,
		const u64* param_lengths,
		u64 param_count,
		u64 activation_layers)
	{
		log_internal(message_template, template_length, params, param_lengths, param_count, activation_layers, 2);
	}

	FROST_API void _stdcall logging_log_info(
		const wchar_t* message_template,
		u64 template_length,
		const wchar_t** params,
		const u64* param_lengths,
		u64 param_count,
		u64 activation_layers)
	{
		log_internal(message_template, template_length, params, param_lengths, param_count, activation_layers, 4);
	}

	FROST_API void _stdcall logging_log_warning(
		const wchar_t* message_template,
		u64 template_length,
		const wchar_t** params,
		const u64* param_lengths,
		u64 param_count,
		u64 activation_layers)
	{
		log_internal(message_template, template_length, params, param_lengths, param_count, activation_layers, 8);
	}

	FROST_API void _stdcall logging_log_error(
		const wchar_t* message_template,
		u64 template_length,
		const wchar_t** params,
		const u64* param_lengths,
		u64 param_count,
		u64 activation_layers)
	{
		log_internal(message_template, template_length, params, param_lengths, param_count, activation_layers, 16);
	}

	FROST_API void _stdcall logging_log_critical(
		const wchar_t* message_template,
		u64 template_length,
		const wchar_t** params,
		const u64* param_lengths,
		u64 param_count,
		u64 activation_layers)
	{
		log_internal(message_template, template_length, params, param_lengths, param_count, activation_layers, 32);
	}
}
#if defined(_WIN32) || defined(_WIN64)
#include <Windows.h>
#include <iostream>
static void log_internal(
	const wchar_t* message_template,
	u64 template_length,
	const wchar_t** params,
	const u64* param_lengths,
	u64 param_count,
	u64 activation_layers,
	u8 level)
{
	log_event e;
	e.message_template	= message_template;
	e.template_length	= template_length;

	e.message_length = render_message_v2(message_template, template_length, params, param_lengths, param_count, nullptr, 0);
	auto message = reinterpret_cast<wchar_t*>(_malloca((e.message_length + 1) * sizeof(wchar_t)));
	render_message_v2(message_template, template_length, params, param_lengths, param_count, message, e.message_length + 1);
	e.message = message;

	e.parameters		= params;
	e.parameter_lengths = param_lengths;
	e.parameter_count	= param_count;

	e.timestamp			= get_timestamp();
	e.thread_id			= get_thread_id();
	e.level				= level;

	event_system_emit(typeid(log_event).hash_code(), activation_layers, &e);

	if (e.message_length > _ALLOCA_S_THRESHOLD)
		delete message;
}


static u64 get_thread_id()
{
	return static_cast<u64>(::GetCurrentThreadId());
}

static u64 get_timestamp()
{
	::FILETIME t;
	GetSystemTimePreciseAsFileTime(&t);
	return t.dwLowDateTime | (static_cast<u64>(t.dwHighDateTime) << 32);
}

static u64 get_param_index(const wchar_t* start, u64 length, u64* consumed)
{
	if (length == 0)
	{
		*consumed = 0;
		return ~0ull;
	}
	if (start[0] != L'{')
	{
		*consumed = 1;
		return ~0ull;
	}

	for (int i = 1; i < length; i++)
	{
		if (std::isdigit(start[i]))
		{
			continue;
		}
		else if (start[i] == L'}')
		{
			*consumed = i + 1;
			return i > 1 ? _wtoi(&start[1]) : -1;
		}
		else
		{
			*consumed = i + 1;
			return ~0ull;
		}
	}
	*consumed = length;
	return ~0ull;
}

static u64 render_message(
	const wchar_t* message_template,
	u64 template_length,
	const wchar_t** parameters,
	const u64* parameter_lengths,
	u64 parameter_count,
	wchar_t* buffer,
	u64 buffer_size)
{
	u64 acc = template_length;
	u64 write_index = 0;
	for (u64 i = 0; i < template_length; i++)
	{
		u64 consumed = 0;
		u64 param_index = get_param_index(&message_template[i], template_length - i, &consumed);
		if (param_index == ~0ull || param_index >= parameter_count)
		{
			if (buffer != nullptr)
			{
				for (u64 read = i; read < (i + consumed) && write_index <= buffer_size; read++)
				{
					buffer[write_index++] = message_template[read];
					if (write_index > buffer_size)
						break;
				}
			}
			
			i += consumed - 1;
		}
		else
		{
			if (buffer != nullptr)
			{
				for (u64 read = 0; read < parameter_lengths[param_index] && write_index < buffer_size; read++)
				{
					buffer[write_index++] = parameters[param_index][read];
					if (write_index > buffer_size)
						break;
				}
			}
			
			acc = acc - consumed + parameter_lengths[param_index];
			i += consumed - 1;
		}
	}
	if (buffer && buffer_size > 0) 
		buffer[buffer_size] = L'\0';
	return acc;
}

static u64 render_message_v2(
	const wchar_t* message_template,
	u64 template_length,
	const wchar_t** parameters,
	const u64* parameter_lengths,
	u64 parameter_count,
	wchar_t* buffer,
	u64 buffer_size)
{
	u64 read = 0;
	u64 write = 0;
	u64 required_size = template_length;
	for (u64 i = 0; i < template_length; i)
	{
		u64 consumed = 0;
		u64 param_index = get_param_index(&message_template[i], template_length - i, &consumed);
		if (param_index == ~0ull || param_index >= parameter_count)
		{	// Invalid index, print normally
			i += consumed;
			if (buffer == nullptr)
				continue;

			for (u64 j = 0; j < consumed; j++)
				buffer[write++] = message_template[i - consumed + j];
		}
		else
		{	// Valid index, print parameter
			required_size = required_size - consumed + parameter_lengths[param_index];
			i += consumed;

			if (buffer == nullptr)
				continue;
			for (u64 j = 0; j < parameter_lengths[param_index]; j++)
			{
				buffer[write++] = parameters[param_index][j];
			}
		}
		
	}
	if (buffer != nullptr)
		buffer[write++] = L'\0';
	return required_size;
}
#endif