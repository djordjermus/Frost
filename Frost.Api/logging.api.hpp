#include "macro.hpp"
#include "primitives.hpp"
#pragma once
extern "C"
{
	struct log_event;

	FROST_API u64 _stdcall logging_get_log_event_tag();

	FROST_API void _stdcall logging_log_verbose(
		const wchar_t* message_template,
		u64 template_length,
		const wchar_t** params,
		const u64* param_lengths,
		u64 param_count,
		u64 activation_layers);

	FROST_API void _stdcall logging_log_debug(
		const wchar_t* message_template,
		u64 template_length,
		const wchar_t** params,
		const u64* param_lengths,
		u64 param_count,
		u64 activation_layers);

	FROST_API void _stdcall logging_log_info(
		const wchar_t* message_template,
		u64 template_length,
		const wchar_t** params,
		const u64* param_lengths,
		u64 param_count,
		u64 activation_layers);

	FROST_API void _stdcall logging_log_warning(
		const wchar_t* message_template,
		u64 template_length,
		const wchar_t** params,
		const u64* param_lengths,
		u64 param_count,
		u64 activation_layers);

	FROST_API void _stdcall logging_log_error(
		const wchar_t* message_template,
		u64 template_length,
		const wchar_t** params,
		const u64* param_lengths,
		u64 param_count,
		u64 activation_layers);

	FROST_API void _stdcall logging_log_critical(
		const wchar_t* message_template,
		u64 template_length,
		const wchar_t** params,
		const u64* param_lengths,
		u64 param_count,
		u64 activation_layers);

	struct log_event final
	{
	public:
		const wchar_t*  message_template;
		u64				template_length;

		const wchar_t*  message;
		u64				message_length;

		const wchar_t** parameters;
		const u64*		parameter_lengths;
		u64				parameter_count;

		u64				timestamp;
		u64				thread_id;
		u8				level;
	};
}