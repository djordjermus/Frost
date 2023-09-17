#include "primitives.hpp"
#include "macro.hpp"
#pragma once

extern "C"
{
	class frost_sync_object;
	class frost_mutex;
	class frost_semaphore;

	FROST_API frost_mutex* _stdcall sync_create_mutex(bool initial_owner);
	FROST_API bool _stdcall sync_acquire_mutex(frost_mutex* p_impl);
	FROST_API bool _stdcall sync_try_acquire_mutex(frost_mutex* p_impl);
	FROST_API bool _stdcall sync_release_mutex(frost_mutex* p_impl);
	FROST_API frost_sync_object* _stdcall sync_get_sync_object_from_mutex(frost_mutex* p_impl);
	FROST_API bool _stdcall sync_destroy_mutex(frost_mutex* p_impl);

	FROST_API frost_semaphore* _stdcall sync_create_semaphore(i32 count, i32 max);
	FROST_API bool _stdcall sync_acquire_semaphore(frost_semaphore* p_impl);
	FROST_API bool _stdcall sync_try_acquire_semaphore(frost_semaphore* p_impl);
	FROST_API bool _stdcall sync_release_semaphore(frost_semaphore* p_impl);
	FROST_API frost_sync_object* _stdcall sync_get_sync_object_from_semaphore(frost_semaphore* p_impl);
	FROST_API bool _stdcall sync_destroy_semaphore(frost_semaphore* p_impl);

	FROST_API i32 sync_acquire_one(frost_sync_object** sync_objects, i32 count);
	FROST_API bool sync_acquire_all(frost_sync_object** sync_objects, i32 count);
	FROST_API i32 sync_try_acquire_one(frost_sync_object** sync_objects, i32 count);
	FROST_API bool sync_try_acquire_all(frost_sync_object** sync_objects, i32 count);
}
