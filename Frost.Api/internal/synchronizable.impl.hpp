#include "../synchronizable.api.hpp"
#include "system_resource.impl.hpp"
#pragma once
#if defined(TARGET_BUILD_PLATFORM_WINDOWS)
#include <windows.h>
namespace frost::impl
{
	class synchronizable : public system_resource
	{
	protected:
		synchronizable(HANDLE handle);
		~synchronizable() override;
	public:
		bool wait() const;
		bool try_wait() const;
		virtual bool signal() const = 0;

		static i32 wait_one(synchronizable* const* target_list, i32 count);
		static bool wait_all(synchronizable* const* target_list, i32 count);
		static i32 try_wait_one(synchronizable* const* target_list, i32 count);
		static bool try_wait_all(synchronizable* const* target_list, i32 count);
	};

	class synchronizable_mutex : public frost::impl::synchronizable
	{
	public:
		synchronizable_mutex(bool initial_owner);

		bool signal() const override;
	};

	class synchronizable_semaphore : public frost::impl::synchronizable
	{
	public:
		synchronizable_semaphore(i32 count, i32 maximum);

		bool signal() const override;
	};

	class synchronizable_event : public synchronizable
	{
	public:
		synchronizable_event();

		bool signal() const override;
		bool reset() const;
	};
}
#else
static_assert("PLATFORM NOT SUPPORTED!")
#endif