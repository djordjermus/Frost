#include "resource.api.hpp"
#pragma once
extern "C"
{
	FROST_API bool _stdcall synchronizable_lock(frost::api::resource* target);
	FROST_API bool _stdcall synchronizable_try_lock(frost::api::resource* target);
	FROST_API bool _stdcall synchronizable_unlock(frost::api::resource* target);
	FROST_API void* _stdcall synchronizable_get_internal_handle(frost::api::resource* target);

	FROST_API i32  _stdcall synchronizable_lock_one(frost::api::resource* const* target_list, i32 count);
	FROST_API bool _stdcall synchronizable_lock_all(frost::api::resource* const* target_list, i32 count);
	FROST_API i32  _stdcall synchronizable_try_lock_one(frost::api::resource* const* target_list, i32 count);
	FROST_API bool _stdcall synchronizable_try_lock_all(frost::api::resource* const* target_list, i32 count);

	FROST_API frost::api::resource* _stdcall synchronizable_create_mutex(bool initial_owner);
	FROST_API frost::api::resource* _stdcall synchronizable_create_semaphore(i32 count, i32 max);

	FROST_API bool _stdcall resource_is_synchronizable(frost::api::resource* target);
}
