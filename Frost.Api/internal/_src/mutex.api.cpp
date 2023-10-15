#include "../mutex.api.hpp"

#if defined(TARGET_BUILD_PLATFORM_WINDOWS)

frost::api::mutex::mutex(HANDLE handle) :
	_handle(handle) {}
frost::api::mutex::~mutex()
{
	::CloseHandle(_handle);
}
frost::api::mutex* frost::api::mutex::create(bool initial_owner)
{
	return new mutex(::CreateMutexW(nullptr, static_cast<BOOL>(initial_owner), nullptr));
}



void* frost::api::mutex::get_system_handle()
{
	return _handle;
}
bool frost::api::mutex::lock()
{
	return ::WaitForSingleObjectEx(_handle, ~0ul, FALSE) == WAIT_OBJECT_0;
}
bool frost::api::mutex::try_lock()
{
	return ::WaitForSingleObjectEx(_handle, 0ul, FALSE) == WAIT_OBJECT_0;
}
bool frost::api::mutex::unlock()
{
	return ::ReleaseMutex(_handle);
}
#else
static_assert("PLATFORM NOT SUPPORTED!")
#endif
