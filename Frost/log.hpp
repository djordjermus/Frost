#include "macro.hpp"
#include "primitives.hpp"
#include <vector>
#include <string>
#pragma once
namespace frost
{
	class log final
	{
	public:
		enum class level : u8;
		struct event_data;

		static void verbose(u64 layers, const std::wstring& message_template, const std::vector<std::wstring> parameters);
		static void debug(u64 layers, const std::wstring& message_template, const std::vector<std::wstring> parameters);
		static void info(u64 layers, const std::wstring& message_template, const std::vector<std::wstring> parameters);
		static void warning(u64 layers, const std::wstring& message_template, const std::vector<std::wstring> parameters);
		static void error(u64 layers, const std::wstring& message_template, const std::vector<std::wstring> parameters);
		static void critical(u64 layers, const std::wstring& message_template, const std::vector<std::wstring> parameters);

		static void hook_relay();

		struct event_data final
		{
		public:
			event_data(
				const wchar_t* message_template,
				const wchar_t* message,
				const wchar_t** parameters,
				u64 parameter_count,
				u64 timestamp,
				u64 thread_id,
				u8 level);

			const std::wstring message_template;
			const std::wstring message;
			const std::vector<std::wstring> parameters;
			const u64 timestamp;
			const u64 thread_id;
			const level level;
		};

		enum class level : u8
		{
			verbose	= 1,
			debug	= 2,
			info	= 4,
			warning	= 8,
			error	= 16,
			fatal	= 32,
		};

		static void relay(u64 tag, u64 layer, void* p_data);

		class api final
		{
		public:
			STATIC_CLASS(api);

			struct log_event;

			static u64 FROST_API logging_get_log_event_tag();

			static void FROST_API logging_log_verbose(
				const wchar_t* message_template,
				u64 template_length,
				const wchar_t** params,
				const u64* param_lengths,
				u64 param_count,
				u64 activation_layers);

			static void FROST_API logging_log_debug(
				const wchar_t* message_template,
				u64 template_length,
				const wchar_t** params,
				const u64* param_lengths,
				u64 param_count,
				u64 activation_layers);

			static void FROST_API logging_log_info(
				const wchar_t* message_template,
				u64 template_length,
				const wchar_t** params,
				const u64* param_lengths,
				u64 param_count,
				u64 activation_layers);

			static void FROST_API logging_log_warning(
				const wchar_t* message_template,
				u64 template_length,
				const wchar_t** params,
				const u64* param_lengths,
				u64 param_count,
				u64 activation_layers);

			static void FROST_API logging_log_error(
				const wchar_t* message_template,
				u64 template_length,
				const wchar_t** params,
				const u64* param_lengths,
				u64 param_count,
				u64 activation_layers);

			static void FROST_API logging_log_critical(
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

	private:

		log() = delete;
		log(log&) = delete;
		log(log&&) = delete;
		~log() = delete;
	};
}