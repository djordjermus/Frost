#include "../mutex.hpp"

#if defined(TARGET_BUILD_PLATFORM_WINDOWS)
#include <Windows.h>
namespace frost::sync
{
	mutex::mutex() :
		pimpl_crtp<mutex>(nullptr) {}
	mutex::mutex(pimpl_t<mutex> pimpl) :
		pimpl_crtp<mutex>(pimpl) {}
	mutex::mutex(bool auto_acquire) :
		pimpl_crtp<mutex>(api::create_mutex(auto_acquire)) {}
	mutex::mutex(mutex& move) :
		pimpl_crtp<mutex>(move) {}
	mutex::mutex(mutex&& move) noexcept :
		pimpl_crtp<mutex>(move) {}
	mutex::~mutex()
	{
		api::destroy_mutex(get_pimpl());
	}

	bool mutex::acquire() const
	{
		return api::acquire_mutex(get_pimpl());
	}
	bool mutex::try_acquire() const
	{
		return api::try_acquire_mutex(get_pimpl());
	}
	bool mutex::release() const
	{
		return api::release_mutex(get_pimpl());
	}
	bool mutex::is_valid() const
	{
		return get_pimpl() != nullptr;
	}
	sync_object mutex::get_sync_object() const
	{
		auto result = sync_object();
		result.swap_pimpl(api::get_sync_object_from_mutex(get_pimpl()));
		return result;
	}



	pimpl_t<mutex> mutex::api::create_mutex(bool initial_owner)
	{
		return reinterpret_cast<pimpl_t<mutex>>(::CreateMutexW(nullptr, static_cast<BOOL>(initial_owner), nullptr));
	}
	bool mutex::api::acquire_mutex(pimpl_t<mutex> p_impl)
	{
		return ::WaitForSingleObjectEx(p_impl, ~0ul, FALSE) == WAIT_OBJECT_0;
	}
	bool mutex::api::try_acquire_mutex(pimpl_t<mutex> p_impl)
	{
		return ::WaitForSingleObjectEx(p_impl, 0ul, FALSE) == WAIT_OBJECT_0;
	}
	bool mutex::api::release_mutex(pimpl_t<mutex> p_impl)
	{
		return ::ReleaseMutex(p_impl);
	}
	pimpl_t<sync_object> mutex::api::get_sync_object_from_mutex(pimpl_t<mutex> p_impl)
	{
		return reinterpret_cast<pimpl_t<sync_object>>(p_impl);
	}
	bool mutex::api::destroy_mutex(pimpl_t<mutex> p_impl)
	{
		return ::CloseHandle(p_impl) == TRUE;
	}
}
#else
static_assert("PLATFORM NOT SUPPORTED!")
#endif