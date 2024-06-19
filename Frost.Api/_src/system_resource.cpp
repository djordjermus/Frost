#include "object.hpp"
using namespace frost::api;

FROST_API void* _stdcall frost_api_system_handle_host_get_system_handle(frost::api::object* target)
{
	switch (target->type)
	{
	case object_type::sync_mutex:
	case object_type::sync_semaphore:
	case object_type::sync_event:
	case object_type::thread_reference:
	case object_type::thread:
	case object_type::window:
		return ((system_handle_host*)target)->handle;

	default:
		return nullptr;
	}
}