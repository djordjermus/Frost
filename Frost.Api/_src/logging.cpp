#include <typeinfo>
#include <vector>
#include <string>
#include "../include.hpp"

static u64 read_placeholder(
	const wchar_t* start,
	u64 length);

static const frost::api::log_parameter* match_parameter(
	const wchar_t* placeholder,
	u64 placeholder_length,
	const frost::api::log_parameter* parameters,
	u64 parameter_count);

static bool match_parameter_name(
	const wchar_t* placeholder,
	const frost::api::log_parameter* parameter);

FROST_API u64 _stdcall frost_api_logging_render_message(
	const wchar_t* message_template,
	u64 message_template_length,
	const frost::api::log_parameter* parameters,
	u64 parameter_count,
	wchar_t* output,
	u64 output_length)
{
	u64 message_length = 0;
	for (u64 sampler = 0; sampler < message_template_length; sampler++)
	{
		auto read = message_template + sampler;
		auto left = message_template_length - sampler;
		if (sampler < message_template_length - 3)
		{
			u64 param_placeholder_len = read_placeholder(read, left);

			if (param_placeholder_len > 0)
			{
				const frost::api::log_parameter* parameter = match_parameter(
					read,
					param_placeholder_len,
					parameters, 
					parameter_count);

				if (parameter != nullptr)
				{
					for (u64 copy = 0; (message_length + copy < output_length) && (copy < parameter->value_length); copy++)
						output[message_length + copy] = parameter->value[copy];
					
					message_length += parameter->value_length;
					sampler += param_placeholder_len;
				}
				else
				{
					if (output_length > message_length)
						output[message_length] = *read;
					message_length++;
				}
			}
			else
			{	// Directly copy character
				if (output_length > message_length)
					output[message_length] = *read;
				message_length++;
			}
		}
		else
		{	// Directly copy character
			if (output_length > message_length)
				output[message_length] = *read;
			message_length++;
		}
	}
	if (output_length > message_length )
		output[message_length] = L'\0';
	return message_length;
}

#if defined(TARGET_BUILD_PLATFORM_WINDOWS) // Needed for _malloca and _freea
FROST_API void _stdcall frost_api_logging_log(
	u64 activation_layers,
	const wchar_t* message_template,
	u64 message_template_length,
	const frost::api::log_parameter* parameters,
	u64 parameter_count,
	frost::api::log_level level)
{
	try
	{
		frost::api::log_event_data data = {};
		data.message_template			= message_template;
		data.message_template_length	= message_template_length;
		data.message_length = frost_api_logging_render_message(
			message_template,
			message_template_length,
			parameters,
			parameter_count,
			nullptr,
			0);

		wchar_t* message = reinterpret_cast<wchar_t*>(_malloca((data.message_length + 1) * sizeof(wchar_t)));
		data.message = message;
		data.message_length = frost_api_logging_render_message(
			message_template,
			message_template_length,
			parameters,
			parameter_count,
			message,
			data.message_length + 1);

		data.parameters					= parameters;
		data.parameter_count			= parameter_count;
		data.level						= level;

		frost_api_event_system_emit(typeid(frost::api::log_event_data).hash_code(), activation_layers, &data);
		_freea(message);
	}
	catch (...) { /*SILENTLY SUPPRESS*/ }
}
#endif

static u64 read_placeholder(const wchar_t* start, u64 length)
{
	if (length < 4) // at least "{@a}"
		return 0;

	if (start[0] != L'{' || start[1] != L'@' || !iswalpha(start[2]))
		return 0;

	for (u64 i = 3; i < length; i++)
	{
		if (start[i] == L'}')
			return i;
		else if (!iswalnum(start[i]))
			return 0;
	}
	return 0;
}

static const frost::api::log_parameter* match_parameter(
	const wchar_t* placeholder,
	u64 placeholder_length,
	const frost::api::log_parameter* parameters,
	u64 parameter_count)
{
	// Adjust for placeholder braces
	const wchar_t* placeholder_name = placeholder + 2;
	u64 placeholder_name_length = placeholder_length - 2;

	for (u64 i = 0; i < parameter_count; i++)
	{
		const frost::api::log_parameter* param = &parameters[i];
		
		if (param->name_length == placeholder_name_length)
		{
			if (match_parameter_name(placeholder_name, param))
				return param;
		}
	}
	return nullptr;
}

static bool match_parameter_name(
	const wchar_t* placeholder,
	const frost::api::log_parameter* parameter)
{
	for (u64 i = 0; i < parameter->name_length; i++)
	{
		if (placeholder[i] != parameter->name[i])
			return false;
	}
	return true;
}



FROST_API u64 _stdcall frost_api_logging_get_log_event_tag()
{
	return typeid(frost::api::log_event_data).hash_code();
}


