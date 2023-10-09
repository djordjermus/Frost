#include "../mutex.hpp"

#if defined(TARGET_BUILD_PLATFORM_WINDOWS)
#include <Windows.h>
namespace frost::sync
{
	mutex::mutex() :
		pimpl_crtp<mutex>() {}
	mutex::mutex(pimpl_t<mutex> pimpl) :
		pimpl_crtp<mutex>(pimpl) {}
	mutex::mutex(bool auto_acquire) :
		pimpl_crtp<mutex>(api::create(auto_acquire)) {}
	mutex::mutex(mutex& move) :
		pimpl_crtp<mutex>(move) {}
	mutex::mutex(mutex&& move) noexcept :
		pimpl_crtp<mutex>(move) {}
	mutex::~mutex()
	{
		api::destroy(get_pimpl());
	}

	bool mutex::acquire() const
	{
		return api::acquire(get_pimpl());
	}
	bool mutex::try_acquire() const
	{
		return api::try_acquire(get_pimpl());
	}
	bool mutex::release() const
	{
		return api::release(get_pimpl());
	}
	bool mutex::is_valid() const
	{
		return get_pimpl() != nullptr;
	}
	sync_object mutex::get_sync_object() const
	{
		auto result = sync_object();
		result.swap_pimpl(api::get_sync_object(get_pimpl()));
		return result;
	}



	pimpl_t<mutex> mutex::api::create(bool initial_owner)
	{
		return reinterpret_cast<pimpl_t<mutex>>(::CreateMutexW(nullptr, static_cast<BOOL>(initial_owner), nullptr));
	}
	bool mutex::api::acquire(pimpl_t<mutex> p_impl)
	{
		return ::WaitForSingleObjectEx(p_impl, ~0ul, FALSE) == WAIT_OBJECT_0;
	}
	bool mutex::api::try_acquire(pimpl_t<mutex> p_impl)
	{
		return ::WaitForSingleObjectEx(p_impl, 0ul, FALSE) == WAIT_OBJECT_0;
	}
	bool mutex::api::release(pimpl_t<mutex> p_impl)
	{
		return ::ReleaseMutex(p_impl);
	}
	pimpl_t<sync_object> mutex::api::get_sync_object(pimpl_t<mutex> p_impl)
	{
		return reinterpret_cast<pimpl_t<sync_object>>(p_impl);
	}
	bool mutex::api::destroy(pimpl_t<mutex> p_impl)
	{
		return ::CloseHandle(p_impl) == TRUE;
	}
}
#else
static_assert("PLATFORM NOT SUPPORTED!")
#endif