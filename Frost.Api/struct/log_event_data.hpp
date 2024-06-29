#include "../primitives.hpp"
#include "log_parameter.hpp"
#include "../enum/log_level.hpp"
#pragma once
namespace frost::api
{
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