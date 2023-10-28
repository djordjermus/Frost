#include "../synchronizable.api.hpp"
#include "../internal/mutex.impl.hpp"
#include "../internal/semaphore.impl.hpp"

bool synchronizable_lock(frost::api::synchronizable* target) { return target->lock(); }
bool synchronizable_try_lock(frost::api::synchronizable* target) { return target->try_lock(); }
bool synchronizable_unlock(frost::api::synchronizable* target) { return target->unlock(); }
void* synchronizable_get_internal_handle(frost::api::synchronizable* target) { return target->get_internal_handle(); }

i32  _stdcall synchronizable_lock_one(frost::api::synchronizable* const* target_list, i32 count)
{
	return frost::api::synchronizable::lock_one(target_list, count);
}
bool _stdcall synchronizable_lock_all(frost::api::synchronizable* const* target_list, i32 count)
{
	return frost::api::synchronizable::lock_all(target_list, count);
}
i32  _stdcall synchronizable_try_lock_one(frost::api::synchronizable* const* target_list, i32 count)
{
	return frost::api::synchronizable::try_lock_one(target_list, count);
}
bool _stdcall synchronizable_try_lock_all(frost::api::synchronizable* const* target_list, i32 count)
{
	return frost::api::synchronizable::try_lock_all(target_list, count);
}

frost::api::synchronizable* synchronizable_create_mutex(bool initial_owner)
{
	return frost::api::synchronizable::create_mutex(initial_owner);
}
frost::api::synchronizable* synchronizable_create_semaphore(i32 count, i32 max)
{
	return frost::api::synchronizable::create_semaphore(count, max);
}

#if defined(TARGET_BUILD_PLATFORM_WINDOWS)
#include <memory>
#include <Windows.h>

i32  frost::api::synchronizable::lock_one(frost::api::synchronizable* const* target_list, i32 count)
{
	if (count > MAXIMUM_WAIT_OBJECTS)
		return false;

	auto* data = reinterpret_cast<HANDLE*>(_malloca(sizeof(HANDLE) * count));
	if (data == nullptr)
		return false;

	for (i32 i = 0; i < count; i++)
		data[i] = target_list[i]->get_internal_handle();

	auto result = ::WaitForMultipleObjects(static_cast<DWORD>(count), reinterpret_cast<HANDLE*>(data), FALSE, ~0u);

	if ((sizeof(HANDLE) * count) > _ALLOCA_S_THRESHOLD)
		free(data);

	if ((result >= WAIT_OBJECT_0) && (result < (WAIT_OBJECT_0 + count)))
		return result - WAIT_OBJECT_0;
	else
		return -1;
}
bool frost::api::synchronizable::lock_all(frost::api::synchronizable* const* target_list, i32 count)
{
	if (count > MAXIMUM_WAIT_OBJECTS)
		return false;

	auto* data = reinterpret_cast<HANDLE*>(_malloca(sizeof(HANDLE) * count));
	if (data == nullptr)
		return false;

	for (i32 i = 0; i < count; i++)
		data[i] = target_list[i]->get_internal_handle();

	auto result = ::WaitForMultipleObjects(static_cast<DWORD>(count), reinterpret_cast<HANDLE*>(data), TRUE, ~0u);

	if ((sizeof(HANDLE) * count) > _ALLOCA_S_THRESHOLD)
		free(data);

	return (result >= WAIT_OBJECT_0) && (result < (WAIT_OBJECT_0 + count));
}
i32  frost::api::synchronizable::try_lock_one(frost::api::synchronizable* const* target_list, i32 count)
{
	if (count > MAXIMUM_WAIT_OBJECTS)
		return false;

	auto* data = reinterpret_cast<HANDLE*>(_malloca(sizeof(HANDLE) * count));
	if (data == nullptr)
		return false;

	for (i32 i = 0; i < count; i++)
		data[i] = target_list[i]->get_internal_handle();

	auto result = ::WaitForMultipleObjects(static_cast<DWORD>(count), reinterpret_cast<HANDLE*>(data), FALSE, 0u);

	if ((sizeof(HANDLE) * count) > _ALLOCA_S_THRESHOLD)
		free(data);

	if ((result >= WAIT_OBJECT_0) && (result < (WAIT_OBJECT_0 + count)))
		return result - WAIT_OBJECT_0;
	else
		return -1;
}
bool frost::api::synchronizable::try_lock_all(frost::api::synchronizable* const* target_list, i32 count)
{
	if (count > MAXIMUM_WAIT_OBJECTS)
		return false;

	auto* data = reinterpret_cast<HANDLE*>(_malloca(sizeof(HANDLE) * count));
	if (data == nullptr)
		return false;

	for (i32 i = 0; i < count; i++)
		data[i] = target_list[i]->get_internal_handle();

	auto result = ::WaitForMultipleObjects(static_cast<DWORD>(count), reinterpret_cast<HANDLE*>(data), TRUE, 0u);

	if ((sizeof(HANDLE) * count) > _ALLOCA_S_THRESHOLD)
		free(data);

	return (result >= WAIT_OBJECT_0) && (result < (WAIT_OBJECT_0 + count));
}

#else
static_assert("PLATFORM NOT SUPPORTED!")
#endif

frost::api::synchronizable* frost::api::synchronizable::create_mutex(bool initial_owner)
{
	return frost::impl::mutex::create(initial_owner);
}
frost::api::synchronizable* frost::api::synchronizable::create_semaphore(i32 count, i32 max)
{
	return frost::impl::semaphore::create(count, max);
}