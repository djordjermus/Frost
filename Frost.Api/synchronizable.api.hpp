#include "resource.api.hpp"
#pragma once
namespace frost::api { class synchronizable; }

extern "C"
{
	FROST_API bool _stdcall synchronizable_lock(frost::api::synchronizable* target);
	FROST_API bool _stdcall synchronizable_try_lock(frost::api::synchronizable* target);
	FROST_API bool _stdcall synchronizable_unlock(frost::api::synchronizable* target);
	FROST_API void* _stdcall synchronizable_get_internal_handle(frost::api::synchronizable* target);

	FROST_API i32  _stdcall synchronizable_lock_one(frost::api::synchronizable* const* target_list, i32 count);
	FROST_API bool _stdcall synchronizable_lock_all(frost::api::synchronizable* const* target_list, i32 count);
	FROST_API i32  _stdcall synchronizable_try_lock_one(frost::api::synchronizable* const* target_list, i32 count);
	FROST_API bool _stdcall synchronizable_try_lock_all(frost::api::synchronizable* const* target_list, i32 count);

	FROST_API frost::api::synchronizable* _stdcall synchronizable_create_mutex(bool initial_owner);
	FROST_API frost::api::synchronizable* _stdcall synchronizable_create_semaphore(i32 count, i32 max);
}

namespace frost::api
{
	class synchronizable : public resource
	{
	public:
		virtual bool lock() = 0;
		virtual bool try_lock() = 0;
		virtual bool unlock() = 0;
		virtual void* get_internal_handle() = 0;

		static i32  _stdcall lock_one(synchronizable* const* target_list, i32 count);
		static bool _stdcall lock_all(synchronizable* const* target_list, i32 count);
		static i32  _stdcall try_lock_one(synchronizable* const* target_list, i32 count);
		static bool _stdcall try_lock_all(synchronizable* const* target_list, i32 count);

		static synchronizable* create_mutex(bool initial_owner);
		static synchronizable* create_semaphore(i32 count, i32 max);
	};
}
