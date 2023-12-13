#include "../internal/synchronizable.impl.hpp"

#define TO_IMPL(x) (static_cast<frost::impl::synchronizable*>(x))
#define TO_IMPL_EVENT(x) (static_cast<frost::impl::synchronizable_event*>(x))
#define TO_IMPL_LIST(x) (reinterpret_cast<frost::impl::synchronizable* const*>(x))

bool synchronizable_wait(frost::api::resource* target)
{
	return TO_IMPL(target)->wait();
}
bool synchronizable_try_wait(frost::api::resource* target)
{
	return TO_IMPL(target)->try_wait();
}
bool synchronizable_signal(frost::api::resource* target)
{
	return TO_IMPL(target)->signal();
}
void* synchronizable_get_internal_handle(frost::api::resource* target)
{
	return TO_IMPL(target)->get_system_handle();
}

FROST_API bool _stdcall synchronizable_event_reset(frost::api::resource* target)
{
	return TO_IMPL_EVENT(target)->reset();
}



i32  _stdcall synchronizable_wait_one(frost::api::resource* const* target_list, i32 count)
{
	return frost::impl::synchronizable::wait_one(TO_IMPL_LIST(target_list), count);
}
bool _stdcall synchronizable_wait_all(frost::api::resource* const* target_list, i32 count)
{
	return frost::impl::synchronizable::wait_all(TO_IMPL_LIST(target_list), count);
}
i32  _stdcall synchronizable_try_wait_one(frost::api::resource* const* target_list, i32 count)
{
	return frost::impl::synchronizable::try_wait_one(TO_IMPL_LIST(target_list), count);
}
bool _stdcall synchronizable_try_wait_all(frost::api::resource* const* target_list, i32 count)
{
	return frost::impl::synchronizable::try_wait_all(TO_IMPL_LIST(target_list), count);
}



frost::api::resource* synchronizable_create_mutex(bool initial_owner)
{
	return new frost::impl::synchronizable_mutex(initial_owner);
}
frost::api::resource* synchronizable_create_semaphore(i32 count, i32 max)
{
	return new frost::impl::synchronizable_semaphore(count, max);
}
FROST_API frost::api::resource* _stdcall synchronizable_create_event()
{
	return new frost::impl::synchronizable_event();
}
bool resource_is_synchronizable(frost::api::resource* target)
{
	return dynamic_cast<frost::impl::synchronizable*>(target) != nullptr;
}
