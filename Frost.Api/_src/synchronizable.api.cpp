#include "../internal/synchronizable.impl.hpp"

#define TO_IMPL(x) (static_cast<frost::impl::synchronizable*>(x))
#define TO_IMPL_EVENT(x) (static_cast<frost::impl::synchronizable_event*>(x))
#define TO_IMPL_LIST(x) (reinterpret_cast<frost::impl::synchronizable* const*>(x))

FROST_API bool _stdcall frost_api_synchronizable_wait(frost::api::resource* target)
{
	return TO_IMPL(target)->wait();
}
FROST_API bool _stdcall frost_api_synchronizable_try_wait(frost::api::resource* target)
{
	return TO_IMPL(target)->try_wait();
}
FROST_API bool _stdcall frost_api_synchronizable_signal(frost::api::resource* target)
{
	return TO_IMPL(target)->signal();
}

FROST_API bool _stdcall frost_api_synchronizable_event_reset(frost::api::resource* target)
{
	return TO_IMPL_EVENT(target)->reset();
}



FROST_API i32 _stdcall frost_api_synchronizable_wait_one(frost::api::resource* const* target_list, i32 count)
{
	return frost::impl::synchronizable::wait_one(TO_IMPL_LIST(target_list), count);
}
FROST_API bool _stdcall frost_api_synchronizable_wait_all(frost::api::resource* const* target_list, i32 count)
{
	return frost::impl::synchronizable::wait_all(TO_IMPL_LIST(target_list), count);
}
FROST_API i32  _stdcall frost_api_synchronizable_try_wait_one(frost::api::resource* const* target_list, i32 count)
{
	return frost::impl::synchronizable::try_wait_one(TO_IMPL_LIST(target_list), count);
}
FROST_API bool _stdcall frost_api_synchronizable_try_wait_all(frost::api::resource* const* target_list, i32 count)
{
	return frost::impl::synchronizable::try_wait_all(TO_IMPL_LIST(target_list), count);
}



FROST_API frost::api::resource* _stdcall frost_api_synchronizable_create_mutex(bool initial_owner)
{
	return new frost::impl::synchronizable_mutex(initial_owner);
}
FROST_API frost::api::resource* _stdcall frost_api_synchronizable_create_semaphore(i32 count, i32 max)
{
	return new frost::impl::synchronizable_semaphore(count, max);
}
FROST_API frost::api::resource* _stdcall frost_api_synchronizable_create_event()
{
	return new frost::impl::synchronizable_event();
}
FROST_API bool _stdcall frost_api_resource_is_synchronizable(frost::api::resource* target)
{
	return dynamic_cast<frost::impl::synchronizable*>(target) != nullptr;
}
