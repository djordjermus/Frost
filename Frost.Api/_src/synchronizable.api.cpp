#include "../synchronizable.api.hpp"

bool frost::api::synchronizable::lock(synchronizable* target) { return target->lock(); }
bool frost::api::synchronizable::try_lock(synchronizable* target) { return target->try_lock(); }
bool frost::api::synchronizable::unlock(synchronizable* target) { return target->unlock(); }
void* frost::api::synchronizable::get_internal_handle(synchronizable* target) { return target->get_internal_handle(); }

#if defined(TARGET_BUILD_PLATFORM_WINDOWS)
#include <memory>
#include <Windows.h>
//frost::api::sync_object* _stdcall frost::api::sync_object::create_mutex(bool initial_owner)
//{
//	return frost::api::mutex::create(initial_owner);
//}
//frost::api::sync_object* _stdcall frost::api::sync_object::create_semaphore(i32 count, i32 max)
//{
//	return frost::api::semaphore::create(count, max);
//}

i32  frost::api::synchronizable::lock_one(frost::api::synchronizable*const* target_list, i32 count)
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
bool frost::api::synchronizable::lock_all(frost::api::synchronizable*const* target_list, i32 count)
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
i32  frost::api::synchronizable::try_lock_one(frost::api::synchronizable*const* target_list, i32 count)
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
bool frost::api::synchronizable::try_lock_all(frost::api::synchronizable*const* target_list, i32 count)
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
