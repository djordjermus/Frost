#include "../system_resource.impl.hpp"
#if defined(TARGET_BUILD_PLATFORM_WINDOWS)
#pragma once
namespace frost::impl
{
	system_resource::system_resource(HANDLE handle) :
		_system_handle(handle) {}
	system_resource::~system_resource()
	{
		::CloseHandle(_system_handle);
	}
	HANDLE system_resource::get_system_handle() const
	{
		return _system_handle;
	}
}
#else
static_assert("PLATFORM NOT SUPPORTED!");
#endif
