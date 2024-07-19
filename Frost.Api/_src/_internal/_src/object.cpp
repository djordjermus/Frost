#include "../object.hpp"
#include "../debug.impl.hpp"
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


#if defined(TARGET_BUILD_PLATFORM_WINDOWS)
static void destroy_window(frost::impl::window* target);
static void destroy_graphics_root(frost::impl::graphics_root* target);
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
		destroy_window(((window*)target));
		break;
	case object_type::data_blob:
		delete ((blob*)target)->data;
		break;
	case object_type::data_blob_inlined:
		break;
	case object_type::graphics_root:
		destroy_graphics_root((graphics_root*)target);
		break;
	default:
		frost::impl::debug::log_object_invalid_type(target);
		break;
	}

	frost::impl::debug::log_object_destruction(target);
	free(target);
}

static void destroy_window(frost::impl::window* target)
{
	::DestroyWindow((HWND)target->handle);
	if (target->graphics.swapchain != nullptr)
		target->graphics.swapchain->Release();
	if (target->_thread != nullptr)
		frost_api_object_release_reference(target->_thread);
	if (target->graphics.root != nullptr)
		frost_api_object_release_reference(target->graphics.root);
}

static void destroy_graphics_root(frost::impl::graphics_root* target)
{
	if (target->factory)
		target->factory->Release();

	if (target->adapter)
		target->adapter->Release();

	if (target->device)
		target->device->Release();

	if (target->queue)
		target->queue->Release();

	if (target->allocator)
		target->allocator->Release();
}

#else
static_assert("PLATFORM NOT SUPPORTED!" == nullptr);
#endif
