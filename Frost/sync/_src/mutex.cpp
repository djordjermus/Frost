#include "../mutex.hpp"
#include "Frost.Api/mutex.api.hpp"

#if defined(TARGET_BUILD_PLATFORM_WINDOWS)
#include <Windows.h>
namespace frost::sync
{
	mutex::mutex() :
		pimpl_crtp<mutex>(nullptr) {}
	mutex::mutex(pimpl_t<mutex> pimpl) :
		pimpl_crtp<mutex>(pimpl) {}
	mutex::mutex(bool auto_acquire) :
		pimpl_crtp<mutex>(api::mutex::create(auto_acquire)) {}
	mutex::mutex(mutex& move) :
		pimpl_crtp<mutex>(move) {}
	mutex::mutex(mutex&& move) noexcept :
		pimpl_crtp<mutex>(move) {}
	mutex::~mutex()
	{
		api::mutex::destroy((frost::api::mutex*)get_pimpl());
	}

	bool mutex::acquire() const
	{
		return api::mutex::acquire((frost::api::mutex*)get_pimpl());
	}
	bool mutex::try_acquire() const
	{
		return api::mutex::try_acquire((frost::api::mutex*)get_pimpl());
	}
	bool mutex::release() const
	{
		return api::mutex::release((frost::api::mutex*)get_pimpl());
	}
	bool mutex::is_valid() const
	{
		return get_pimpl() != nullptr;
	}
	sync_object mutex::get_sync_object() const
	{
		auto result = sync_object();
		result.swap_pimpl((pimpl_t<sync_object>)frost::api::mutex::get_sync_object((frost::api::mutex*)get_pimpl()));
		return result;
	}


}
#else
static_assert("PLATFORM NOT SUPPORTED!")
#endif