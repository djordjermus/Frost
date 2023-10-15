#include "primitives.hpp"
#include "macro.hpp"
#pragma once
namespace frost::api
{
	class sync_object;
	class FROST_API semaphore final
	{
	public:

		static handle<semaphore> _stdcall create(i32 count, i32 max);
		static bool _stdcall acquire(semaphore* p_impl);
		static bool _stdcall try_acquire(semaphore* p_impl);
		static bool _stdcall release(semaphore* p_impl);
		static handle<sync_object> _stdcall get_sync_object(semaphore* p_impl);
		static bool _stdcall destroy(semaphore* p_impl);
	};
}