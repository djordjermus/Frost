#include "../semaphore.impl.hpp"

#if defined(TARGET_BUILD_PLATFORM_WINDOWS)

frost::impl::semaphore::semaphore(HANDLE handle) :
	_handle(handle) {}
frost::impl::semaphore::~semaphore()
{
	::CloseHandle(_handle);
}
frost::impl::semaphore* frost::impl::semaphore::create(i32 count, i32 max)
{
	return new semaphore(::CreateSemaphoreW(nullptr, count, max, nullptr));
}

AUTO_RESOURCE_INTERFACE_IMPL(frost::impl::semaphore);

void* frost::impl::semaphore::get_internal_handle()
{
	return _handle;
}
bool frost::impl::semaphore::lock()
{
	return ::WaitForSingleObjectEx(_handle, ~0ul, FALSE) == WAIT_OBJECT_0;
}
bool frost::impl::semaphore::try_lock()
{
	return ::WaitForSingleObjectEx(_handle, 0ul, FALSE) == WAIT_OBJECT_0;
}
bool frost::impl::semaphore::unlock()
{
	return ::ReleaseSemaphore(_handle, 1, nullptr);
}
#else
static_assert("PLATFORM NOT SUPPORTED!")
#endif
