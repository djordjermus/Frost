#include "primitives.hpp"
#include "macro.hpp"
#pragma once
namespace frost::api
{
	class sync_object final
	{
	public:
		static i32  FROST_API acquire_one(const sync_object** synchronization_objects, i32 count);
		static bool FROST_API acquire_all(const sync_object** synchronization_objects, i32 count);
		static i32  FROST_API try_acquire_one(const sync_object** synchronization_objects, i32 count);
		static bool FROST_API try_acquire_all(const sync_object** synchronization_objects, i32 count);
	};
}
