#include "../synchronizable.api.hpp"
#include "resource.impl.hpp"
#pragma once
#if defined(TARGET_BUILD_PLATFORM_WINDOWS)
#include <windows.h>
namespace frost::impl
{
	class synchronizable : public api::resource
	{
	protected:
		HANDLE _handle;
		synchronizable(HANDLE handle);
		~synchronizable() override;

	public:
		HANDLE get_system_handle() const;
		bool lock() const;
		bool try_lock() const;
		virtual bool unlock() const = 0;

		static i32 lock_one(synchronizable* const* target_list, i32 count);
		static bool lock_all(synchronizable* const* target_list, i32 count);
		static i32 try_lock_one(synchronizable* const* target_list, i32 count);
		static bool try_lock_all(synchronizable* const* target_list, i32 count);
		
		static api::resource* create_mutex(bool initial_owner);
		static api::resource* create_semaphore(i32 count, i32 maximum);
	};
}
#else
static_assert("PLATFORM NOT SUPPORTED!")
#endif