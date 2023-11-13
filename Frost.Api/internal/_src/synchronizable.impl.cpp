#include "../synchronizable.impl.hpp"
#if defined(TARGET_BUILD_PLATFORM_WINDOWS)
#include <memory>


/*
 * MUTEX
 */
class synchronizable_mutex : public frost::impl::synchronizable
{
public:
	synchronizable_mutex(bool initial_owner);
	bool unlock() const override;
};

synchronizable_mutex::synchronizable_mutex(bool initial_owner) :
	frost::impl::synchronizable(::CreateMutexW(nullptr, initial_owner, nullptr)) {}

bool synchronizable_mutex::unlock() const { return ::ReleaseMutex(_handle); }



/*
 * SEMAPHORE
 */
class synchronizable_semaphore : public frost::impl::synchronizable
{
public:
	synchronizable_semaphore(i32 count, i32 maximum);
	bool unlock() const override;
};

synchronizable_semaphore::synchronizable_semaphore(i32 count, i32 maximum) :
	frost::impl::synchronizable(::CreateSemaphoreW(nullptr, count, maximum, nullptr)) {}

bool synchronizable_semaphore::unlock() const { return ::ReleaseSemaphore(_handle, 1, nullptr); }



/*
 * SYNCHRONIZABLE
 */
frost::impl::synchronizable::synchronizable(HANDLE handle) :
	_handle(handle) {}
frost::impl::synchronizable::~synchronizable()
{
	::CloseHandle(_handle);
}

HANDLE frost::impl::synchronizable::get_system_handle() const
{
	return _handle;
}

bool frost::impl::synchronizable::lock() const
{
	return ::WaitForSingleObjectEx(_handle, ~0ul, FALSE) == WAIT_OBJECT_0;
}



frost::api::resource* frost::impl::synchronizable::create_semaphore(i32 count, i32 maximum)
{
	return new synchronizable_semaphore(count, maximum);
}

i32  frost::impl::synchronizable::lock_one(frost::impl::synchronizable* const* target_list, i32 count)
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
bool frost::impl::synchronizable::lock_all(frost::impl::synchronizable* const* target_list, i32 count)
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
i32  frost::impl::synchronizable::try_lock_one(frost::impl::synchronizable* const* target_list, i32 count)
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
bool frost::impl::synchronizable::try_lock_all(frost::impl::synchronizable* const* target_list, i32 count)
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



bool frost::impl::synchronizable::try_lock() const
{
	return ::WaitForSingleObjectEx(_handle, 0ul, FALSE) == WAIT_OBJECT_0;
}

frost::api::resource* frost::impl::synchronizable::create_mutex(bool initial_owner)
{
	return new synchronizable_mutex(initial_owner);
}

#else
static_assert("PLATFORM NOT SUPPORTED!")
#endif