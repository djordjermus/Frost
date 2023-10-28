#include "resource.impl.hpp"
#include "../synchronizable.api.hpp"
#pragma once
#if defined(TARGET_BUILD_PLATFORM_WINDOWS)
#include <Windows.h>
namespace frost::impl
{
	class mutex : public resource, public api::synchronizable
	{
		HANDLE _handle;

	protected:
		mutex(HANDLE handle);
		~mutex();
		void* _stdcall get_internal_handle() override;

	public:
		AUTO_RESOURCE_INTERFACE_DECL();

		static mutex* create(bool initial_owner);
		bool _stdcall lock() override;
		bool _stdcall try_lock() override;
		bool _stdcall unlock() override;
	};
}
#else
static_assert("PLATFORM NOT SUPPORTED!")
#endif