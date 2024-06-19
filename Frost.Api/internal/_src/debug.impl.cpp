#include <string>
#include <exception>
#include <stdio.h>
#include "../debug.impl.hpp"
#include "../../include.hpp"
void frost::impl::debug::log_resource_creation(const frost::api::object* target)
{
#if FROST_API_DEBUG_LOG_RESOURCE_CREATION == 1
	constexpr u64 param_count = 1;
	constexpr u64 address_size = sizeof(void*) * (8 / 4) + 1;

	static const wchar_t _template[] = L"creating resource at 0x{@Address}.";
	static const u64 _template_length = sizeof(_template) / sizeof(*_template) - 1;

	static const wchar_t _exception_template[] = L"logging of resource creation threw exception! message:\n{@ExceptionMessage}.";
	static const u64 _exception_template_length = sizeof(_exception_template) / sizeof(*_exception_template) - 1;

	static const wchar_t _error_template[] = L"logging of resource creation uncaught error!\n";
	static const u64 _error_template_length = sizeof(_error_template) / sizeof(*_error_template) - 1;

	static const wchar_t _prop_address[] = L"Address";
	static const u64 _prop_address_length = sizeof(_prop_address) / sizeof(*_prop_address) - 1;
	try
	{
		// ADDRESS
		wchar_t address[address_size] = {};
		swprintf_s(address, address_size, L"%p", target);

		// DEBUG LOG
		frost::api::log_parameter params[1] =
		{
			{ _prop_address, address, _prop_address_length,address_size - 1 }
		};
		frost_api_logging_log(
			frost_api_event_system_get_api_broadcast_layer(),
			_template,
			_template_length,
			params, 
			sizeof(params) / sizeof(*params),
			frost::api::log_level::trace);
	}
	catch (const std::exception& e)
	{
		static const wchar_t _exc_message[] = L"ExceptionMessage";
		static const u64 _exc_message_length = sizeof(_exc_message) / sizeof(*_exc_message) - 1;

		const char* c_what = e.what();
		u64 what_length = strlen(c_what) + 1;
		std::wstring what = std::wstring(c_what, c_what + what_length);
		const wchar_t* wcs_what = what.c_str();
		frost::api::log_parameter params[1] =
		{
			{ _exc_message, wcs_what, _exc_message_length, what_length}
		};

		frost_api_logging_log(
			frost_api_event_system_get_api_broadcast_layer(),
			_exception_template,
			_exception_template_length,
			params,
			sizeof(params) / sizeof(*params),
			frost::api::log_level::error);
	}
	catch (...)
	{
		frost_api_logging_log(
			frost_api_event_system_get_api_broadcast_layer(),
			_error_template,
			_error_template_length,
			nullptr,
			0,
			frost::api::log_level::error);
	}
#endif
}
void frost::impl::debug::log_resource_destruction(const frost::api::object* target)
{
#if FROST_API_DEBUG_LOG_RESOURCE_DESTRUCTION == 1
	constexpr u64 param_count = 1;
	constexpr u64 address_size = sizeof(void*) * (8 / 4) + 1;

	static const wchar_t _template[] = L"destroying resource at 0x{@Address}.";
	static const u64 _template_length = sizeof(_template) / sizeof(*_template) - 1;

	static const wchar_t _exception_template[] = L"logging of resource destruction threw exception! message:\n{@ExceptionMessage}.";
	static const u64 _exception_template_length = sizeof(_exception_template) / sizeof(*_exception_template) - 1;

	static const wchar_t _error_template[] = L"logging of resource destruction uncaught error!";
	static const u64 _error_template_length = sizeof(_error_template) / sizeof(*_error_template - 1);

	static const wchar_t _prop_address[] = L"Address";
	static const u64 _prop_address_length = sizeof(_prop_address) / sizeof(*_prop_address) - 1;
	try
	{
		// ADDRESS
		wchar_t address[address_size] = {};
		swprintf_s(address, address_size, L"%p", target);

		// DEBUG LOG
		frost::api::log_parameter params[1] =
		{
			{ _prop_address, address, _prop_address_length,address_size - 1 }
		};
		frost_api_logging_log(
			frost_api_event_system_get_api_broadcast_layer(),
			_template,
			_template_length,
			params,
			sizeof(params) / sizeof(*params),
			frost::api::log_level::trace);
	}
	catch (const std::exception& e)
	{
		static const wchar_t _exc_message[] = L"ExceptionMessage";
		static const u64 _exc_message_length = sizeof(_exc_message) / sizeof(*_exc_message) - 1;

		const char* c_what = e.what();
		u64 what_length = strlen(c_what) + 1;
		std::wstring what = std::wstring(c_what, c_what + what_length);
		const wchar_t* wcs_what = what.c_str();
		frost::api::log_parameter params[1] =
		{
			{ _exc_message, wcs_what, _exc_message_length, what_length}
		};

		frost_api_logging_log(
			frost_api_event_system_get_api_broadcast_layer(),
			_exception_template,
			_exception_template_length,
			params,
			sizeof(params) / sizeof(*params),
			frost::api::log_level::error);
	}
	catch (...)
	{
		frost_api_logging_log(
			frost_api_event_system_get_api_broadcast_layer(),
			_error_template,
			_error_template_length,
			nullptr,
			0,
			frost::api::log_level::error);
	}
#endif
}