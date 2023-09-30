#include <typeinfo>
#include "../event_system.hpp"
#include "../log.hpp"

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

namespace frost
{
	void log::verbose(u64 layers, const std::wstring& message_template, const std::vector<std::wstring> parameters)
	{
		auto params = (const wchar_t**)_malloca(sizeof(wchar_t*) * parameters.size());
		auto lengths = (u64*)_malloca(sizeof(u64) * parameters.size());
		for (u64 i = 0; i < parameters.size(); i++)
		{
			params[i] = parameters[i].data();
			lengths[i] = parameters[i].size();
		}

		api::logging_log_verbose(message_template.data(), message_template.length(), params, lengths, parameters.size(), layers);

		if ((sizeof(wchar_t*) * parameters.size()) > _ALLOCA_S_THRESHOLD)
			delete params;
		if ((sizeof(u64) * parameters.size()) > _ALLOCA_S_THRESHOLD)
			delete lengths;
	}

	void log::debug(u64 layers, const std::wstring& message_template, const std::vector<std::wstring> parameters)
	{
		auto params = (const wchar_t**)_malloca(sizeof(wchar_t*) * parameters.size());
		auto lengths = (u64*)_malloca(sizeof(u64) * parameters.size());
		for (u64 i = 0; i < parameters.size(); i++)
		{
			params[i] = parameters[i].data();
			lengths[i] = parameters[i].size();
		}

		api::logging_log_debug(message_template.data(), message_template.length(), params, lengths, parameters.size(), layers);

		if ((sizeof(wchar_t*) * parameters.size()) > _ALLOCA_S_THRESHOLD)
			delete params;
		if ((sizeof(u64) * parameters.size()) > _ALLOCA_S_THRESHOLD)
			delete lengths;
	}

	void log::info(u64 layers, const std::wstring& message_template, const std::vector<std::wstring> parameters)
	{
		auto params = (const wchar_t**)_malloca(sizeof(wchar_t*) * parameters.size());
		auto lengths = (u64*)_malloca(sizeof(u64) * parameters.size());
		for (u64 i = 0; i < parameters.size(); i++)
		{
			params[i] = parameters[i].data();
			lengths[i] = parameters[i].size();
		}

		api::logging_log_info(message_template.data(), message_template.length(), params, lengths, parameters.size(), layers);

		if ((sizeof(wchar_t*) * parameters.size()) > _ALLOCA_S_THRESHOLD)
			delete params;
		if ((sizeof(u64) * parameters.size()) > _ALLOCA_S_THRESHOLD)
			delete lengths;
	}

	void log::warning(u64 layers, const std::wstring& message_template, const std::vector<std::wstring> parameters)
	{
		auto params = (const wchar_t**)_malloca(sizeof(wchar_t*) * parameters.size());
		auto lengths = (u64*)_malloca(sizeof(u64) * parameters.size());
		for (u64 i = 0; i < parameters.size(); i++)
		{
			params[i] = parameters[i].data();
			lengths[i] = parameters[i].size();
		}

		api::logging_log_warning(message_template.data(), message_template.length(), params, lengths, parameters.size(), layers);

		if ((sizeof(wchar_t*) * parameters.size()) > _ALLOCA_S_THRESHOLD)
			delete params;
		if ((sizeof(u64) * parameters.size()) > _ALLOCA_S_THRESHOLD)
			delete lengths;
	}

	void log::error(u64 layers, const std::wstring& message_template, const std::vector<std::wstring> parameters)
	{
		auto params = (const wchar_t**)_malloca(sizeof(wchar_t*) * parameters.size());
		auto lengths = (u64*)_malloca(sizeof(u64) * parameters.size());
		for (u64 i = 0; i < parameters.size(); i++)
		{
			params[i] = parameters[i].data();
			lengths[i] = parameters[i].size();
		}

		api::logging_log_error(message_template.data(), message_template.length(), params, lengths, parameters.size(), layers);

		if ((sizeof(wchar_t*) * parameters.size()) > _ALLOCA_S_THRESHOLD)
			delete params;
		if ((sizeof(u64) * parameters.size()) > _ALLOCA_S_THRESHOLD)
			delete lengths;
	}

	void log::critical(u64 layers, const std::wstring& message_template, const std::vector<std::wstring> parameters)
	{
		auto params = (const wchar_t**)_malloca(sizeof(wchar_t*) * parameters.size());
		auto lengths = (u64*)_malloca(sizeof(u64) * parameters.size());
		for (u64 i = 0; i < parameters.size(); i++)
		{
			params[i] = parameters[i].data();
			lengths[i] = parameters[i].size();
		}

		api::logging_log_critical(message_template.data(), message_template.length(), params, lengths, parameters.size(), layers);

		if ((sizeof(wchar_t*) * parameters.size()) > _ALLOCA_S_THRESHOLD)
			delete params;
		if ((sizeof(u64) * parameters.size()) > _ALLOCA_S_THRESHOLD)
			delete lengths;
	}

	void log::hook_relay()
	{
		event_system::api::event_system_subscribe_relay(relay);
	}

	void log::relay(u64 tag, u64 layer, void* p_data)
	{
		static u64 log_tag = api::logging_get_log_event_tag();
		if (tag != log_tag)
			return;

		log::api::log_event* raw = reinterpret_cast<log::api::log_event*>(p_data);
		event_data e = event_data(
			raw->message_template,
			raw->message, raw->parameters,
			raw->parameter_count,
			raw->timestamp,
			raw->thread_id,
			raw->level);

		event_system::api::event_system_emit(typeid(event_data).hash_code(), layer, &e);
	}
	static std::vector<std::wstring> params_to_vec(const wchar_t** params, u64 count);
	log::event_data::event_data(
		const wchar_t* message_template,
		const wchar_t* message,
		const wchar_t** parameters,
		u64 parameter_count,
		u64 timestamp,
		u64 thread_id,
		u8 level) :
		message_template(message_template),
		message(message),
		parameters(params_to_vec(parameters, parameter_count)),
		timestamp(timestamp),
		thread_id(thread_id),
		level((log::level)level) { }

	static std::vector<std::wstring> params_to_vec(const wchar_t** params, u64 count)
	{
		std::vector<std::wstring> result;
		for (u64 i = 0; i < count; i++)
			result.emplace_back(params[i]);

		return result;
	}



	u64 log::api::logging_get_log_event_tag()
	{
		return typeid(log::api::log_event).hash_code();
	}

	void log::api::logging_log_verbose(
		const wchar_t* message_template,
		u64 template_length,
		const wchar_t** params,
		const u64* param_lengths,
		u64 param_count,
		u64 activation_layers)
	{
		log_internal(message_template, template_length, params, param_lengths, param_count, activation_layers, 1);
	}

	void log::api::logging_log_debug(
		const wchar_t* message_template,
		u64 template_length,
		const wchar_t** params,
		const u64* param_lengths,
		u64 param_count,
		u64 activation_layers)
	{
		log_internal(message_template, template_length, params, param_lengths, param_count, activation_layers, 2);
	}

	void log::api::logging_log_info(
		const wchar_t* message_template,
		u64 template_length,
		const wchar_t** params,
		const u64* param_lengths,
		u64 param_count,
		u64 activation_layers)
	{
		log_internal(message_template, template_length, params, param_lengths, param_count, activation_layers, 4);
	}

	void log::api::logging_log_warning(
		const wchar_t* message_template,
		u64 template_length,
		const wchar_t** params,
		const u64* param_lengths,
		u64 param_count,
		u64 activation_layers)
	{
		log_internal(message_template, template_length, params, param_lengths, param_count, activation_layers, 8);
	}

	void log::api::logging_log_error(
		const wchar_t* message_template,
		u64 template_length,
		const wchar_t** params,
		const u64* param_lengths,
		u64 param_count,
		u64 activation_layers)
	{
		log_internal(message_template, template_length, params, param_lengths, param_count, activation_layers, 16);
	}

	void log::api::logging_log_critical(
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

#if defined(TARGET_BUILD_PLATFORM_WINDOWS)
#include <Windows.h>

static void log_internal(
	const wchar_t* message_template,
	u64 template_length,
	const wchar_t** params,
	const u64* param_lengths,
	u64 param_count,
	u64 activation_layers,
	u8 level)
{
	frost::log::api::log_event e;
	e.message_template = message_template;
	e.template_length = template_length;

	e.message_length = render_message_v2(message_template, template_length, params, param_lengths, param_count, nullptr, 0);
	auto message = reinterpret_cast<wchar_t*>(_malloca((e.message_length + 1) * sizeof(wchar_t)));
	render_message_v2(message_template, template_length, params, param_lengths, param_count, message, e.message_length + 1);
	e.message = message;

	e.parameters = params;
	e.parameter_lengths = param_lengths;
	e.parameter_count = param_count;

	e.timestamp = get_timestamp();
	e.thread_id = get_thread_id();
	e.level = level;

	frost::event_system::api::event_system_emit(typeid(frost::log::api::log_event).hash_code(), activation_layers, &e);

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
