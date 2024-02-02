#include "resource.api.hpp"
#pragma once
extern "C"
{
	FROST_API bool _stdcall  frost_api_synchronizable_wait(frost::api::resource* target);
	FROST_API bool _stdcall  frost_api_synchronizable_try_wait(frost::api::resource* target);
	FROST_API bool _stdcall  frost_api_synchronizable_signal(frost::api::resource* target);

	FROST_API bool _stdcall frost_api_synchronizable_event_reset(frost::api::resource* target);

	FROST_API i32  _stdcall frost_api_synchronizable_wait_one(frost::api::resource* const* target_list, i32 count);
	FROST_API bool _stdcall frost_api_synchronizable_wait_all(frost::api::resource* const* target_list, i32 count);
	FROST_API i32  _stdcall frost_api_synchronizable_try_wait_one(frost::api::resource* const* target_list, i32 count);
	FROST_API bool _stdcall frost_api_synchronizable_try_wait_all(frost::api::resource* const* target_list, i32 count);

	FROST_API frost::api::resource* _stdcall frost_api_synchronizable_create_mutex(bool initial_owner);
	FROST_API frost::api::resource* _stdcall frost_api_synchronizable_create_semaphore(i32 count, i32 max);
	FROST_API frost::api::resource* _stdcall frost_api_synchronizable_create_event();

	FROST_API bool _stdcall frost_api_resource_is_synchronizable(frost::api::resource* target);
}
