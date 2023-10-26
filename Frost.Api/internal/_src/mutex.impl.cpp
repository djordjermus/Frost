#include "../mutex.impl.hpp"

#if defined(TARGET_BUILD_PLATFORM_WINDOWS)

frost::impl::mutex::mutex(HANDLE handle) :
	_handle(handle) {}
frost::impl::mutex::~mutex()
{
	::CloseHandle(_handle);
}
frost::impl::mutex* frost::impl::mutex::create(bool initial_owner)
{
	return new mutex(::CreateMutexW(nullptr, static_cast<BOOL>(initial_owner), nullptr));
}

AUTO_RESOURCE_INTERFACE_IMPL(frost::impl::mutex);

void* frost::impl::mutex::get_internal_handle()
{
	return _handle;
}
bool frost::impl::mutex::lock()
{
	return ::WaitForSingleObjectEx(_handle, ~0ul, FALSE) == WAIT_OBJECT_0;
}
bool frost::impl::mutex::try_lock()
{
	return ::WaitForSingleObjectEx(_handle, 0ul, FALSE) == WAIT_OBJECT_0;
}
bool frost::impl::mutex::unlock()
{
	return ::ReleaseMutex(_handle);
}
#else
static_assert("PLATFORM NOT SUPPORTED!")
#endif
