#include "object.api.hpp"
#pragma once
namespace frost::api
{
	class synchronizable : public object
	{
	public:
		virtual bool lock() = 0;
		virtual bool try_lock() = 0;
		virtual bool unlock() = 0;

		static FROST_API bool _stdcall lock(synchronizable* target);
		static FROST_API bool _stdcall try_lock(synchronizable* target);
		static FROST_API bool _stdcall unlock(synchronizable* target);

		static FROST_API i32  _stdcall lock_one(synchronizable* const* target_list, i32 count);
		static FROST_API bool _stdcall lock_all(synchronizable* const* target_list, i32 count);
		static FROST_API i32  _stdcall try_lock_one(synchronizable* const* target_list, i32 count);
		static FROST_API bool _stdcall try_lock_all(synchronizable* const* target_list, i32 count);

	protected:
		virtual void* get_internal_handle() = 0;

		static FROST_API void* _stdcall get_internal_handle(synchronizable* target);
	};
}
