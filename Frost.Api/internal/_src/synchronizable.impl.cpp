#include "../synchronizable.impl.hpp"
#if defined(TARGET_BUILD_PLATFORM_WINDOWS)
#include <memory>


namespace frost::impl
{
	/*
	 * MUTEX
	 */
	synchronizable_mutex::synchronizable_mutex(bool initial_owner) :
		synchronizable(::CreateMutexW(nullptr, initial_owner, nullptr)) {}

	bool synchronizable_mutex::signal() const { return ::ReleaseMutex(_system_handle); }

	synchronizable_semaphore::synchronizable_semaphore(i32 count, i32 maximum) :
		synchronizable(::CreateSemaphoreW(nullptr, count, maximum, nullptr)) {}

	bool synchronizable_semaphore::signal() const { return ::ReleaseSemaphore(_system_handle, 1, nullptr); }

	/* 
	 * EVENT
	 */
	synchronizable_event::synchronizable_event() : 
		synchronizable(::CreateEventW(nullptr, TRUE, FALSE, nullptr)){}

	bool synchronizable_event::signal() const
	{
		return ::SetEvent(_system_handle);
	}
	bool synchronizable_event::reset() const
	{
		return ::ResetEvent(_system_handle);
	}


	/*
	 * SYNCHRONIZABLE
	 */

	synchronizable::synchronizable(HANDLE handle) :
		system_resource(handle) {}

	

	bool synchronizable::wait() const
	{
		return ::WaitForSingleObjectEx(_system_handle, ~0ul, FALSE) == WAIT_OBJECT_0;
	}



	i32  synchronizable::wait_one(synchronizable* const* target_list, i32 count)
	{
		if (count > MAXIMUM_WAIT_OBJECTS)
			return false;

		auto* data = reinterpret_cast<HANDLE*>(_malloca(sizeof(HANDLE) * count));
		if (data == nullptr)
			return false;

		for (i32 i = 0; i < count; i++)
			data[i] = target_list[i]->get_system_handle();

		auto result = ::WaitForMultipleObjects(static_cast<DWORD>(count), reinterpret_cast<HANDLE*>(data), FALSE, ~0u);

		if ((sizeof(HANDLE) * count) > _ALLOCA_S_THRESHOLD)
			free(data);

		if ((result >= WAIT_OBJECT_0) && (result < (WAIT_OBJECT_0 + count)))
			return result - WAIT_OBJECT_0;
		else
			return -1;
	}
	bool synchronizable::wait_all(synchronizable* const* target_list, i32 count)
	{
		if (count > MAXIMUM_WAIT_OBJECTS)
			return false;

		auto* data = reinterpret_cast<HANDLE*>(_malloca(sizeof(HANDLE) * count));
		if (data == nullptr)
			return false;

		for (i32 i = 0; i < count; i++)
			data[i] = target_list[i]->get_system_handle();

		auto result = ::WaitForMultipleObjects(static_cast<DWORD>(count), reinterpret_cast<HANDLE*>(data), TRUE, ~0u);

		if ((sizeof(HANDLE) * count) > _ALLOCA_S_THRESHOLD)
			free(data);

		return (result >= WAIT_OBJECT_0) && (result < (WAIT_OBJECT_0 + count));
	}
	i32  synchronizable::try_wait_one(synchronizable* const* target_list, i32 count)
	{
		if (count > MAXIMUM_WAIT_OBJECTS)
			return false;

		auto* data = reinterpret_cast<HANDLE*>(_malloca(sizeof(HANDLE) * count));
		if (data == nullptr)
			return false;

		for (i32 i = 0; i < count; i++)
			data[i] = target_list[i]->get_system_handle();

		auto result = ::WaitForMultipleObjects(static_cast<DWORD>(count), reinterpret_cast<HANDLE*>(data), FALSE, 0u);

		if ((sizeof(HANDLE) * count) > _ALLOCA_S_THRESHOLD)
			free(data);

		if ((result >= WAIT_OBJECT_0) && (result < (WAIT_OBJECT_0 + count)))
			return result - WAIT_OBJECT_0;
		else
			return -1;
	}
	bool synchronizable::try_wait_all(synchronizable* const* target_list, i32 count)
	{
		if (count > MAXIMUM_WAIT_OBJECTS)
			return false;

		auto* data = reinterpret_cast<HANDLE*>(_malloca(sizeof(HANDLE) * count));
		if (data == nullptr)
			return false;

		for (i32 i = 0; i < count; i++)
			data[i] = target_list[i]->get_system_handle();

		auto result = ::WaitForMultipleObjects(static_cast<DWORD>(count), reinterpret_cast<HANDLE*>(data), TRUE, 0u);

		if ((sizeof(HANDLE) * count) > _ALLOCA_S_THRESHOLD)
			free(data);

		return (result >= WAIT_OBJECT_0) && (result < (WAIT_OBJECT_0 + count));
	}



	bool synchronizable::try_wait() const
	{
		return ::WaitForSingleObjectEx(_system_handle, 0ul, FALSE) == WAIT_OBJECT_0;
	}
}

#else
static_assert("PLATFORM NOT SUPPORTED!")
#endif