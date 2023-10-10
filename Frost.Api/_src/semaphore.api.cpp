#include "../semaphore.api.hpp"

#if defined(TARGET_BUILD_PLATFORM_WINDOWS)
#include <Windows.h>

frost::api::semaphore* frost::api::semaphore::create(i32 count, i32 max)
{
	return reinterpret_cast<semaphore*>(::CreateSemaphoreW(nullptr, count, max, nullptr));
}
bool frost::api::semaphore::acquire(semaphore* p_impl)
{
	return ::WaitForSingleObjectEx(p_impl, ~0ul, FALSE) == WAIT_OBJECT_0;
}
bool frost::api::semaphore::try_acquire(semaphore* p_impl)
{
	return ::WaitForSingleObjectEx(p_impl, 0ul, FALSE) == WAIT_OBJECT_0;
}
bool frost::api::semaphore::release(semaphore* p_impl)
{
	return ::ReleaseSemaphore(p_impl, 1, nullptr) == TRUE;
}
frost::api::sync_object* frost::api::semaphore::get_sync_object(semaphore* p_impl)
{
	return reinterpret_cast<sync_object*>(p_impl);
}
bool frost::api::semaphore::destroy(semaphore* p_impl)
{
	return ::CloseHandle(p_impl) == TRUE;
}

#else
static_assert("PLATFORM NOT SUPPORTED!")
#endif
