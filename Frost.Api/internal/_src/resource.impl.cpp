#include <typeinfo>
#include <string>
#include <exception>
#include <stdio.h>
#include "../resource.impl.hpp"
#include "../../event_system.api.hpp"
#include "../../logging.api.hpp"
#include <iostream>
frost::api::resource::resource() : _reference_count()
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
		swprintf_s(address, address_length, L"%p", this);

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
frost::api::resource::~resource()
{
#if FROST_API_DEBUG_LOG_RESOURCE_DESTRUCTION == 1
	constexpr u64 param_count = 2;
	constexpr u64 address_length = 17;
	constexpr u64 name_max_length = 121;

	static const wchar_t* _template = L"deleting resource at 0x{0} of concrete type \"{1}\"";
	static const u64 _template_length = wcslen(_template);

	static const wchar_t* _exception_template = L"logging of resource deletion threw exception! message:\n{0}";
	static const u64 _exception_template_length = wcslen(_exception_template);

	static const wchar_t* _error_template = L"logging of resource deletion uncaught error!\n";
	static const u64 _error_template_length = wcslen(_error_template);
	try
	{
		// ADDRESS
		wchar_t address[address_length]	= {};
		swprintf_s(address, address_length, L"%p", this);
		
		// TYPE NAME
		wchar_t name[name_max_length] = {};
		wchar_t* write = name;
		const char* read = typeid(*this).name();
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

u64 frost::api::resource::get_reference_count() const
{
	return this->_reference_count;
}
void frost::api::resource::acquire_reference() const
{
	this->_reference_count.fetch_add(1); 
}
void frost::api	::resource::release_reference() const
{
	if (_reference_count.fetch_sub(1) == 1)
		delete this;
}
