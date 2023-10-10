#include "primitives.hpp"
#include "macro.hpp"
#pragma once
namespace frost::api
{
	class sync_object;
	class semaphore final
	{
	public:

		static handle<semaphore> FROST_API create(i32 count, i32 max);
		static bool FROST_API acquire(semaphore* p_impl);
		static bool FROST_API try_acquire(semaphore* p_impl);
		static bool FROST_API release(semaphore* p_impl);
		static handle<sync_object> FROST_API get_sync_object(semaphore* p_impl);
		static bool FROST_API destroy(semaphore* p_impl);
	};
}