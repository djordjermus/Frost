#include "resource.impl.hpp"
#if defined(TARGET_BUILD_PLATFORM_WINDOWS)
#include <Windows.h>
#pragma once
namespace frost::impl
{
	class system_resource : public api::resource
	{
	protected:
		HANDLE _system_handle;
		system_resource(HANDLE handle);
		~system_resource();
		
	public:
		HANDLE get_system_handle() const;
	};
}
#endif