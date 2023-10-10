#include <typeinfo>
#include "Frost.Api/event_system.api.hpp"
#include "Frost.Api/logging.api.hpp"
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
#pragma warning(push)
#pragma warning(disable: 6011)
			params[i] = parameters[i].data();
			lengths[i] = parameters[i].size();
#pragma warning(pop)
		}

		api::logging::verbose(message_template.data(), message_template.length(), params, lengths, parameters.size(), layers);

		if ((sizeof(wchar_t*) * parameters.size()) > _ALLOCA_S_THRESHOLD)
			delete params;
		if ((sizeof(u64) * parameters.size()) > _ALLOCA_S_THRESHOLD)
			delete lengths;
	}

	void log::debug(u64 layers, const std::wstring& message_template, const std::vector<std::wstring> parameters)
	{
#pragma warning(push)
#pragma warning(disable: 6011)
		auto params = (const wchar_t**)_malloca(sizeof(wchar_t*) * parameters.size());
		auto lengths = (u64*)_malloca(sizeof(u64) * parameters.size());
		for (u64 i = 0; i < parameters.size(); i++)
		{
			params[i] = parameters[i].data();
			lengths[i] = parameters[i].size();
#pragma warning(pop)
		}

		api::logging::debug(message_template.data(), message_template.length(), params, lengths, parameters.size(), layers);

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
#pragma warning(push)
#pragma warning(disable: 6011)
			params[i] = parameters[i].data();
			lengths[i] = parameters[i].size();
#pragma warning(pop)
		}

		api::logging::info(message_template.data(), message_template.length(), params, lengths, parameters.size(), layers);

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
#pragma warning(push)
#pragma warning(disable: 6011)
			params[i] = parameters[i].data();
			lengths[i] = parameters[i].size();
#pragma warning(pop)
		}

		api::logging::warning(message_template.data(), message_template.length(), params, lengths, parameters.size(), layers);

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
#pragma warning(push)
#pragma warning(disable: 6011)
			params[i] = parameters[i].data();
			lengths[i] = parameters[i].size();
#pragma warning(pop)
		}

		api::logging::error(message_template.data(), message_template.length(), params, lengths, parameters.size(), layers);

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
#pragma warning(push)
#pragma warning(disable: 6011)
			params[i] = parameters[i].data();
			lengths[i] = parameters[i].size();
#pragma warning(pop)
		}

		api::logging::critical(message_template.data(), message_template.length(), params, lengths, parameters.size(), layers);

		if ((sizeof(wchar_t*) * parameters.size()) > _ALLOCA_S_THRESHOLD)
			delete params;
		if ((sizeof(u64) * parameters.size()) > _ALLOCA_S_THRESHOLD)
			delete lengths;
	}

	void log::hook_relay()
	{
		api::event_system::subscribe_relay(relay);
	}

	void log::relay(u64 tag, u64 layer, void* p_data)
	{
		static u64 log_tag = api::logging::get_log_event_tag();
		if (tag != log_tag)
			return;

		api::logging::log_event* raw = reinterpret_cast<api::logging::log_event*>(p_data);
		event_data e = event_data(
			raw->message_template,
			raw->message, raw->parameters,
			raw->parameter_count,
			raw->timestamp,
			raw->thread_id,
			raw->level);

		api::event_system::emit(typeid(event_data).hash_code(), layer, &e);
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
