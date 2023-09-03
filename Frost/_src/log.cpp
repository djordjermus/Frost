#include <typeinfo>
#include "Frost.Api/logging.api.hpp"
#include "Frost.Api/event_system.api.hpp"
#include "../log.hpp"

#if defined(_WIN32) || defined(_WIN64)
#include <Windows.h>
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

		logging_log_verbose(message_template.data(), message_template.length(), params, lengths, parameters.size(), layers);

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

		logging_log_debug(message_template.data(), message_template.length(), params, lengths, parameters.size(), layers);

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

		logging_log_info(message_template.data(), message_template.length(), params, lengths, parameters.size(), layers);

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

		logging_log_warning(message_template.data(), message_template.length(), params, lengths, parameters.size(), layers);

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

		logging_log_error(message_template.data(), message_template.length(), params, lengths, parameters.size(), layers);

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

		logging_log_critical(message_template.data(), message_template.length(), params, lengths, parameters.size(), layers);

		if ((sizeof(wchar_t*) * parameters.size()) > _ALLOCA_S_THRESHOLD)
			delete params;
		if ((sizeof(u64) * parameters.size()) > _ALLOCA_S_THRESHOLD)
			delete lengths;
	}

	void log::hook_relay()
	{
		event_system_subscribe_relay(relay);
	}

	void log::relay(u64 tag, u64 layer, void* p_data)
	{
		static u64 log_tag = logging_get_log_event_tag();
		if (tag != log_tag)
			return;

		log_event* raw = reinterpret_cast<log_event*>(p_data);
		event_data e = event_data(
			raw->message_template,
			raw->message, raw->parameters,
			raw->parameter_count,
			raw->timestamp,
			raw->thread_id,
			raw->level);

		event_system_emit(typeid(event_data).hash_code(), layer, &e);
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

}
#endif
