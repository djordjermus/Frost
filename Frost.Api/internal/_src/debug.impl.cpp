#include <string>
#include <exception>
#include <stdio.h>
#include "../debug.impl.hpp"
#include "../../event_system.api.hpp"
#include "../../logging.api.hpp"
void frost::impl::debug::log_resource_creation(const frost::api::resource* target)
{
#if FROST_API_DEBUG_LOG_RESOURCE_CREATION == 1
	constexpr u64 param_count = 1;
	constexpr u64 address_length = 17;

	static const wchar_t* _template = L"creating resource at 0x{0}";
	static const u64 _template_length = wcslen(_template);

	static const wchar_t* _exception_template = L"logging of resource creation threw exception! message:\n{0}";
	static const u64 _exception_template_length = wcslen(_exception_template);

	static const wchar_t* _error_template = L"logging of resource creation uncaught error!\n";
	static const u64 _error_template_length = wcslen(_error_template);
	try
	{
		// ADDRESS
		wchar_t address[address_length] = {};
		swprintf_s(address, address_length, L"%p", target);

		// DEBUG LOG
		const wchar_t* params[param_count] = { address };
		const u64 param_lengths[param_count] = { address_length };
		logging_debug(_template, _template_length, params, param_lengths, param_count, event_system_get_api_broadcast_layer());
	}
	catch (const std::exception& e)
	{
		const char* c_what = e.what();
		u64 what_length = strlen(c_what) + 1;
		std::wstring what = std::wstring(c_what, c_what + what_length);
		const wchar_t* wcs_what = what.c_str();
		logging_error(_exception_template, _exception_template_length, &wcs_what, &what_length, 1, event_system_get_api_broadcast_layer());
	}
	catch (...)
	{
		logging_error(_error_template, _error_template_length, nullptr, nullptr, 0, event_system_get_api_broadcast_layer());
	}
#endif
}
void frost::impl::debug::log_resource_destruction(const frost::api::resource* target, const std::type_info& type)
{
#if FROST_API_DEBUG_LOG_RESOURCE_DESTRUCTION == 1
	constexpr u64 param_count = 2;
	constexpr u64 address_length = 17;
	constexpr u64 name_max_length = 121;

	static const wchar_t* _template = L"destroying resource at 0x{0} of concrete type \"{1}\"";
	static const u64 _template_length = wcslen(_template);

	static const wchar_t* _exception_template = L"logging of resource destruction threw exception! message:\n{0}";
	static const u64 _exception_template_length = wcslen(_exception_template);

	static const wchar_t* _error_template = L"logging of resource destruction uncaught error!\n";
	static const u64 _error_template_length = wcslen(_error_template);
	try
	{
		// ADDRESS
		wchar_t address[address_length]	= {};
		swprintf_s(address, address_length, L"%p", target);
		
		// TYPE NAME
		wchar_t name[name_max_length] = {};
		wchar_t* write = name;
		const char* read = typeid(*target).name();
		while (write != (name + name_max_length - 1) && *read != '\0')
		{
			*write = *read;
			write++;
			read++;
		}

		// DEBUG LOG
		const wchar_t* params[param_count]		= { address, name };
		const u64 param_lengths[param_count]	= { address_length, (u64)(write - name) };
		logging_debug(_template, _template_length, params, param_lengths, param_count, event_system_get_api_broadcast_layer());
	}
	catch (const std::exception& e)
	{
		const char* c_what = e.what();
		u64 what_length = strlen(c_what) + 1;
		std::wstring what = std::wstring(c_what, c_what + what_length);
		const wchar_t* wcs_what = what.c_str();
		logging_error(_exception_template, _exception_template_length, &wcs_what, &what_length, 1, event_system_get_api_broadcast_layer());
	}
	catch (...)
	{
		logging_error(_error_template, _error_template_length, nullptr, nullptr, 0, event_system_get_api_broadcast_layer());
	}
#endif
}