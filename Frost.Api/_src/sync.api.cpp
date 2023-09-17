#include "../sync.api.hpp"
#if defined(_WIN32) || defined(_WIN64)
#include <Windows.h>
extern "C"
{
	FROST_API frost_mutex* _stdcall sync_create_mutex(bool initial_owner)
	{
		return reinterpret_cast<frost_mutex*>(::CreateMutexW(nullptr, static_cast<BOOL>(initial_owner), nullptr));
	}

	FROST_API bool _stdcall sync_acquire_mutex(frost_mutex* p_impl)
	{
		return ::WaitForSingleObjectEx(p_impl, ~0ul, FALSE) == WAIT_OBJECT_0;
	}

	FROST_API bool _stdcall sync_try_acquire_mutex(frost_mutex* p_impl)
	{
		return ::WaitForSingleObjectEx(p_impl, 0ul, FALSE) == WAIT_OBJECT_0;
	}

	FROST_API bool _stdcall sync_release_mutex(frost_mutex* p_impl)
	{
		return ::ReleaseMutex(p_impl);
	}

	FROST_API frost_sync_object* _stdcall sync_get_sync_object_from_mutex(frost_mutex* p_impl)
	{
		return reinterpret_cast<frost_sync_object*>(p_impl);
	}

	FROST_API bool _stdcall sync_destroy_mutex(frost_mutex* p_impl)
	{
		return ::CloseHandle(p_impl) == TRUE;
	}



	FROST_API frost_semaphore* _stdcall sync_create_semaphore(i32 count, i32 max)
	{
		return reinterpret_cast<frost_semaphore*>(::CreateSemaphoreW(nullptr, count, max, nullptr));
	}

	FROST_API bool _stdcall sync_acquire_semaphore(frost_semaphore* p_impl)
	{
		return ::WaitForSingleObjectEx(p_impl, ~0ul, FALSE) == WAIT_OBJECT_0;
	}

	FROST_API bool _stdcall sync_try_acquire_semaphore(frost_semaphore* p_impl)
	{
		return ::WaitForSingleObjectEx(p_impl, 0ul, FALSE) == WAIT_OBJECT_0;
	}

	FROST_API bool _stdcall sync_release_semaphore(frost_semaphore* p_impl)
	{
		return ::ReleaseSemaphore(p_impl, 1, nullptr) == TRUE;
	}

	FROST_API frost_sync_object* _stdcall sync_get_sync_object_from_semaphore(frost_semaphore* p_impl)
	{
		return reinterpret_cast<frost_sync_object*>(p_impl);
	}

	FROST_API bool _stdcall sync_destroy_semaphore(frost_semaphore* p_impl)
	{
		return ::CloseHandle(p_impl) == TRUE;
	}



	FROST_API i32 sync_acquire_one(frost_sync_object** sync_objects, i32 count)
	{
		if (count > MAXIMUM_WAIT_OBJECTS)
			return false;

		auto result = ::WaitForMultipleObjects(count, reinterpret_cast<HANDLE*>(sync_objects), FALSE, ~0ul);
		if ((result >= WAIT_OBJECT_0) && (result < (WAIT_OBJECT_0 + count)))
			return result - WAIT_OBJECT_0;
		else
			return -1;
	}

	FROST_API bool sync_acquire_all(frost_sync_object** sync_objects, i32 count)
	{
		if (count > MAXIMUM_WAIT_OBJECTS)
			return false;

		auto result = ::WaitForMultipleObjects(count, reinterpret_cast<HANDLE*>(sync_objects), TRUE, ~0ul);
		return (result >= WAIT_OBJECT_0) && (result < (WAIT_OBJECT_0 + count));
	}

	FROST_API i32 sync_try_acquire_one(frost_sync_object** sync_objects, i32 count)
	{
		if (count > MAXIMUM_WAIT_OBJECTS)
			return false;

		auto result = ::WaitForMultipleObjects(count, reinterpret_cast<HANDLE*>(sync_objects), FALSE, 0ul);
		if ((result >= WAIT_OBJECT_0) && (result < (WAIT_OBJECT_0 + count)))
			return result - WAIT_OBJECT_0;
		else
			return -1;
	}

	FROST_API bool sync_try_acquire_all(frost_sync_object** sync_objects, i32 count)
	{
		if (count > MAXIMUM_WAIT_OBJECTS)
			return false;

		auto result = ::WaitForMultipleObjects(count, reinterpret_cast<HANDLE*>(sync_objects), TRUE, 0ul);
		return (result >= WAIT_OBJECT_0) && (result < (WAIT_OBJECT_0 + count));
	}
}
#endif
