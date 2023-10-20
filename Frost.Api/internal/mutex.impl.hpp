#include "object.impl.hpp"
#include "../mutex.api.hpp"
#pragma once
#if defined(TARGET_BUILD_PLATFORM_WINDOWS)
#include <Windows.h>
namespace frost::impl
{
	class mutex : public object, public api::mutex
	{
		HANDLE _handle;

	protected:
		mutex(HANDLE handle);
		~mutex();
		void* _stdcall get_internal_handle() override;

	public:
		AUTO_OBJECT_INTERFACE_DECL();

		static mutex* create(bool initial_owner);
		bool _stdcall lock() override;
		bool _stdcall try_lock() override;
		bool _stdcall unlock() override;
	};
}
#else
static_assert("PLATFORM NOT SUPPORTED!")
#endif