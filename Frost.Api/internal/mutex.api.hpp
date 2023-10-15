#include "../sync_object.api.hpp"
#pragma once
#if defined(TARGET_BUILD_PLATFORM_WINDOWS)
#include <Windows.h>
namespace frost::api
{
	class mutex : public sync_object
	{
		HANDLE _handle;

	protected:
		mutex(HANDLE handle);
		~mutex();
		void* _stdcall get_system_handle() override;

	public:
		static mutex* create(bool initial_owner);
		bool _stdcall lock() override;
		bool _stdcall try_lock() override;
		bool _stdcall unlock() override;
	};
}
#else
static_assert("PLATFORM NOT SUPPORTED!")
#endif