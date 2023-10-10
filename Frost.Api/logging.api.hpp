#include "primitives.hpp"
#include "macro.hpp"
#pragma once
namespace frost::api
{
	class logging final
	{
	public:
		STATIC_CLASS(logging);

		struct log_event;

		static u64 FROST_API get_log_event_tag();

		static void FROST_API verbose(
			const wchar_t* message_template,
			u64 template_length,
			const wchar_t** params,
			const u64* param_lengths,
			u64 param_count,
			u64 activation_layers);

		static void FROST_API debug(
			const wchar_t* message_template,
			u64 template_length,
			const wchar_t** params,
			const u64* param_lengths,
			u64 param_count,
			u64 activation_layers);

		static void FROST_API info(
			const wchar_t* message_template,
			u64 template_length,
			const wchar_t** params,
			const u64* param_lengths,
			u64 param_count,
			u64 activation_layers);

		static void FROST_API warning(
			const wchar_t* message_template,
			u64 template_length,
			const wchar_t** params,
			const u64* param_lengths,
			u64 param_count,
			u64 activation_layers);

		static void FROST_API error(
			const wchar_t* message_template,
			u64 template_length,
			const wchar_t** params,
			const u64* param_lengths,
			u64 param_count,
			u64 activation_layers);

		static void FROST_API critical(
			const wchar_t* message_template,
			u64 template_length,
			const wchar_t** params,
			const u64* param_lengths,
			u64 param_count,
			u64 activation_layers);

		struct log_event final
		{
		public:
			const wchar_t*	message_template;
			u64				template_length;

			const wchar_t*	message;
			u64				message_length;

			const wchar_t** parameters;
			const u64*		parameter_lengths;
			u64				parameter_count;

			u64				timestamp;
			u64				thread_id;
			u8				level;
		};
	};
}