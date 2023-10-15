#include "../semaphore.api.hpp"

#if defined(TARGET_BUILD_PLATFORM_WINDOWS)

frost::api::semaphore::semaphore(HANDLE handle) :
	_handle(handle) {}
frost::api::semaphore::~semaphore()
{
	::CloseHandle(_handle);
}
frost::api::semaphore* frost::api::semaphore::create(i32 count, i32 max)
{
	return new semaphore(::CreateSemaphoreW(nullptr, count, max, nullptr));
}



void* frost::api::semaphore::get_system_handle()
{
	return _handle;
}
bool frost::api::semaphore::lock()
{
	return ::WaitForSingleObjectEx(_handle, ~0ul, FALSE) == WAIT_OBJECT_0;
}
bool frost::api::semaphore::try_lock()
{
	return ::WaitForSingleObjectEx(_handle, 0ul, FALSE) == WAIT_OBJECT_0;
}
bool frost::api::semaphore::unlock()
{
	return ::ReleaseSemaphore(_handle, 1, nullptr);
}
#else
static_assert("PLATFORM NOT SUPPORTED!")
#endif
