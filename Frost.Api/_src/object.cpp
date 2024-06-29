#include "object.hpp"
#include "_internal/debug.impl.hpp"
#include <typeinfo>
using namespace frost::api;
using namespace frost::impl;

FROST_API u64 _stdcall frost_api_object_get_reference_count(object* target)
{
	return target->reference_count;
}
FROST_API void _stdcall frost_api_object_acquire_reference(object* target)
{
	target->reference_count.fetch_add(1);
}
FROST_API void _stdcall frost_api_object_release_reference(object* target)
{
	if (target->reference_count.fetch_sub(1) != 1)
		return; // Did not reach zero

	switch (target->type)
	{
	case object_type::sync_mutex:
	case object_type::sync_semaphore:
	case object_type::sync_event:
	case object_type::thread_reference:
		::CloseHandle(((system_handle_host*)target)->handle);
		break;

	case object_type::thread:
		frost_api_synchronizable_wait(((system_handle_host*)target));
		::CloseHandle(((system_handle_host*)target)->handle);
		break;
	case object_type::window:
		::DestroyWindow((HWND)((window*)target)->handle);
		break;
	default:
		frost::impl::debug::log_object_invalid_type(target);
		break;
	}

	frost::impl::debug::log_object_destruction(target);
	delete target;
}
