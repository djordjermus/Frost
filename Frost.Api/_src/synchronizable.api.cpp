#include "../synchronizable.api.hpp"
#include "../internal/synchronizable.impl.hpp"

#if defined(TARGET_BUILD_PLATFORM_WINDOWS)
#include <memory>
#include <Windows.h>
#define TO_IMPL(x) (static_cast<frost::impl::synchronizable*>(x))
#define TO_IMPL_LIST(x) (reinterpret_cast<frost::impl::synchronizable* const*>(x))

bool synchronizable_lock(frost::api::resource* target) { return TO_IMPL(target)->lock(); }
bool synchronizable_try_lock(frost::api::resource* target) { return TO_IMPL(target)->try_lock(); }
bool synchronizable_unlock(frost::api::resource* target) { return TO_IMPL(target)->unlock(); }
void* synchronizable_get_internal_handle(frost::api::resource* target) { return TO_IMPL(target)->get_system_handle(); }



i32  _stdcall synchronizable_lock_one(frost::api::resource* const* target_list, i32 count)
{
	return frost::impl::synchronizable::lock_one(TO_IMPL_LIST(target_list), count);
}
bool _stdcall synchronizable_lock_all(frost::api::resource* const* target_list, i32 count)
{
	return frost::impl::synchronizable::lock_all(TO_IMPL_LIST(target_list), count);
}
i32  _stdcall synchronizable_try_lock_one(frost::api::resource* const* target_list, i32 count)
{
	return frost::impl::synchronizable::try_lock_one(TO_IMPL_LIST(target_list), count);
}
bool _stdcall synchronizable_try_lock_all(frost::api::resource* const* target_list, i32 count)
{
	return frost::impl::synchronizable::try_lock_all(TO_IMPL_LIST(target_list), count);
}



frost::api::resource* synchronizable_create_mutex(bool initial_owner)
{
	return frost::impl::synchronizable::create_mutex(initial_owner);
}
frost::api::resource* synchronizable_create_semaphore(i32 count, i32 max)
{
	return frost::impl::synchronizable::create_semaphore(count, max);
}

#else
static_assert("PLATFORM NOT SUPPORTED!")
#endif
