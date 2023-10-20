#include "object.impl.hpp"
#include "../semaphore.api.hpp"
#pragma once
#if defined(TARGET_BUILD_PLATFORM_WINDOWS)
#include <Windows.h>
namespace frost::impl
{
	class semaphore : public object, public api::semaphore
	{
		HANDLE _handle;

	protected:
		semaphore(HANDLE handle);
		~semaphore();
		void* _stdcall get_internal_handle() override;

	public:
		AUTO_OBJECT_INTERFACE_DECL();

		static semaphore* create(i32 count, i32 max);
		bool _stdcall lock() override;
		bool _stdcall try_lock() override;
		bool _stdcall unlock() override;
	};
}
#else
static_assert("PLATFORM NOT SUPPORTED!")
#endif