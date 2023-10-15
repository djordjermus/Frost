#include "object.api.hpp"
#pragma once
namespace frost::api
{
	class sync_object : public object
	{
	public:
		virtual bool _stdcall lock() = 0;
		virtual bool _stdcall try_lock() = 0;
		virtual bool _stdcall unlock() = 0;

	protected:
		virtual void* _stdcall get_system_handle() = 0;



		/* * * * * * * * * */
		/* STATIC METHODS */
		/* * * * * * * * */
	public:
		static FROST_API sync_object* _stdcall create_mutex(bool initial_owner);
		static FROST_API sync_object* _stdcall create_semaphore(i32 count, i32 max);

		static FROST_API bool _stdcall lock(sync_object* target);
		static FROST_API bool _stdcall try_lock(sync_object* target);
		static FROST_API bool _stdcall unlock(sync_object* target);

		static FROST_API i32  _stdcall lock_one(sync_object*const* synchronization_objects, i32 count);
		static FROST_API bool _stdcall lock_all(sync_object*const* synchronization_objects, i32 count);
		static FROST_API i32  _stdcall try_lock_one(sync_object*const* synchronization_objects, i32 count);
		static FROST_API bool _stdcall try_lock_all(sync_object*const* synchronization_objects, i32 count);

	protected:
		static FROST_API void* _stdcall get_system_handle(sync_object* target);
	};
}
