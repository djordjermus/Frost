#include "../mutex.api.hpp"

#if defined(TARGET_BUILD_PLATFORM_WINDOWS)
#include <Windows.h>

frost::api::mutex* frost::api::mutex::create(bool initial_owner)
{
	return reinterpret_cast<frost::api::mutex*>(::CreateMutexW(nullptr, static_cast<BOOL>(initial_owner), nullptr));
}
bool frost::api::mutex::acquire(frost::api::mutex* p_impl)
{
	return ::WaitForSingleObjectEx(p_impl, ~0ul, FALSE) == WAIT_OBJECT_0;
}
bool frost::api::mutex::try_acquire(frost::api::mutex* p_impl)
{
	return ::WaitForSingleObjectEx(p_impl, 0ul, FALSE) == WAIT_OBJECT_0;
}
bool frost::api::mutex::release(frost::api::mutex* p_impl)
{
	return ::ReleaseMutex(p_impl);
}
frost::api::sync_object* frost::api::mutex::get_sync_object(frost::api::mutex* p_impl)
{
	return reinterpret_cast<frost::api::sync_object*>(p_impl);
}
bool frost::api::mutex::destroy(frost::api::mutex* p_impl)
{
	return ::CloseHandle(p_impl) == TRUE;
}

#else
static_assert("PLATFORM NOT SUPPORTED!")
#endif
