#include "../sync_object.api.hpp"
#pragma once
#if defined(TARGET_BUILD_PLATFORM_WINDOWS)
#include <Windows.h>
namespace frost::api
{
	class semaphore : public sync_object
	{
		HANDLE _handle;

	protected:
		semaphore(HANDLE handle);
		~semaphore();
		void* _stdcall get_system_handle() override;

	public:
		static semaphore* create(i32 count, i32 max);
		bool _stdcall lock() override;
		bool _stdcall try_lock() override;
		bool _stdcall unlock() override;
	};
}
#else
static_assert("PLATFORM NOT SUPPORTED!")
#endif