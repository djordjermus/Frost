#include "primitives.hpp"
#include "macro.hpp"
#pragma once
namespace frost::api
{
	class FROST_API sync_object final
	{
	public:
		static i32  _stdcall acquire_one(const sync_object** synchronization_objects, i32 count);
		static bool _stdcall acquire_all(const sync_object** synchronization_objects, i32 count);
		static i32  _stdcall try_acquire_one(const sync_object** synchronization_objects, i32 count);
		static bool _stdcall try_acquire_all(const sync_object** synchronization_objects, i32 count);
	};
}
