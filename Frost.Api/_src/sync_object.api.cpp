#include "../sync_object.api.hpp"

#if defined(TARGET_BUILD_PLATFORM_WINDOWS)
#include <Windows.h>

i32  frost::api::sync_object::acquire_one(const frost::api::sync_object** synchronization_objects, i32 count)
{
		if (count > MAXIMUM_WAIT_OBJECTS)
		return false;

	auto* data = const_cast<frost::api::sync_object**>(synchronization_objects);
	auto result = ::WaitForMultipleObjects(static_cast<DWORD>(count), reinterpret_cast<HANDLE*>(data), FALSE, ~0u);
	if ((result >= WAIT_OBJECT_0) && (result < (WAIT_OBJECT_0 + count)))
		return result - WAIT_OBJECT_0;
	else
		return -1;
}
bool frost::api::sync_object::acquire_all(const frost::api::sync_object** synchronization_objects, i32 count)
{
	if (count > MAXIMUM_WAIT_OBJECTS)
		return false;

	auto* data = const_cast<frost::api::sync_object**>(synchronization_objects);
	auto result = ::WaitForMultipleObjects(static_cast<DWORD>(count), reinterpret_cast<HANDLE*>(data), TRUE, ~0u);

	return (result >= WAIT_OBJECT_0) && (result < (WAIT_OBJECT_0 + count));
}
i32  frost::api::sync_object::try_acquire_one(const frost::api::sync_object** synchronization_objects, i32 count)
{
	if (count > MAXIMUM_WAIT_OBJECTS)
		return false;

	auto* data = const_cast<frost::api::sync_object**>(synchronization_objects);
	auto result = ::WaitForMultipleObjects(static_cast<DWORD>(count), reinterpret_cast<HANDLE*>(data), FALSE, 0u);

	if ((result >= WAIT_OBJECT_0) && (result < (WAIT_OBJECT_0 + count)))
		return result - WAIT_OBJECT_0;
	else
		return -1;
}
bool frost::api::sync_object::try_acquire_all(const frost::api::sync_object** synchronization_objects, i32 count)
{
	if (count > MAXIMUM_WAIT_OBJECTS)
		return false;

	auto* data = const_cast<frost::api::sync_object**>(synchronization_objects);
	auto result = ::WaitForMultipleObjects(static_cast<DWORD>(count), reinterpret_cast<HANDLE*>(data), TRUE, 0u);

	return (result >= WAIT_OBJECT_0) && (result < (WAIT_OBJECT_0 + count));
}

#else
static_assert("PLATFORM NOT SUPPORTED!")
#endif
