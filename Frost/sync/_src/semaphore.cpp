#include "../semaphore.hpp"
#include "Frost.Api/semaphore.api.hpp"

#if defined(TARGET_BUILD_PLATFORM_WINDOWS)
#include <Windows.h>
namespace frost::sync
{
	semaphore::semaphore() :
		pimpl_crtp<semaphore>(nullptr) {}
	semaphore::semaphore(pimpl_t<semaphore> pimpl) :
		pimpl_crtp<semaphore>(pimpl) {}
	semaphore::semaphore(i32 count, i32 max) :
		pimpl_crtp<semaphore>((semaphore*)api::semaphore::create(count, max)) {}
	semaphore::semaphore(semaphore& move) :
		pimpl_crtp<semaphore>(move) {}
	semaphore::semaphore(semaphore&& move) noexcept :
		pimpl_crtp<semaphore>(move) {}
	semaphore::~semaphore()
	{
		frost::api::semaphore::destroy((frost::api::semaphore*)semaphore::get_pimpl());
	}

	bool semaphore::acquire() const
	{
		return api::semaphore::acquire((frost::api::semaphore*)get_pimpl());
	}
	bool semaphore::try_acquire() const
	{
		return api::semaphore::try_acquire((frost::api::semaphore*)get_pimpl());
	}
	bool semaphore::release() const
	{
		return api::semaphore::release((frost::api::semaphore*)get_pimpl());
	}
	bool semaphore::is_valid() const
	{
		return get_pimpl() != nullptr;
	}
	sync_object semaphore::get_sync_object() const
	{
		auto result = sync_object();
		result.swap_pimpl((pimpl_t<sync_object>)api::semaphore::get_sync_object((frost::api::semaphore*)get_pimpl()));
		return result;
	}
}
#else
static_assert("PLATFORM NOT SUPPORTED!")
#endif