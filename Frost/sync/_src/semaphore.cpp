#include "../semaphore.hpp"

#if defined(TARGET_BUILD_PLATFORM_WINDOWS)
#include <Windows.h>
namespace frost::sync
{
	semaphore::semaphore() :
		pimpl_crtp<semaphore>() {}
	semaphore::semaphore(pimpl_t<semaphore> pimpl) :
		pimpl_crtp<semaphore>(pimpl) {}
	semaphore::semaphore(i32 count, i32 max) :
		pimpl_crtp<semaphore>(api::create(count, max)) {}
	semaphore::semaphore(semaphore& move) :
		pimpl_crtp<semaphore>(move) {}
	semaphore::semaphore(semaphore&& move) noexcept :
		pimpl_crtp<semaphore>(move) {}
	semaphore::~semaphore()
	{
		api::destroy(get_pimpl());
	}

	bool semaphore::acquire() const
	{
		return api::acquire(get_pimpl());
	}
	bool semaphore::try_acquire() const
	{
		return api::try_acquire(get_pimpl());
	}
	bool semaphore::release() const
	{
		return api::release(get_pimpl());
	}
	bool semaphore::is_valid() const
	{
		return get_pimpl() != nullptr;
	}
	sync_object semaphore::get_sync_object() const
	{
		auto result = sync_object();
		result.swap_pimpl(api::get_sync_object(get_pimpl()));
		return result;
	}

	pimpl_t<semaphore> semaphore::api::create(i32 count, i32 max)
	{
		return reinterpret_cast<pimpl_t<semaphore>>(::CreateSemaphoreW(nullptr, count, max, nullptr));
	}
	bool semaphore::api::acquire(pimpl_t<semaphore> p_impl)
	{
		return ::WaitForSingleObjectEx(p_impl, ~0ul, FALSE) == WAIT_OBJECT_0;
	}
	bool semaphore::api::try_acquire(pimpl_t<semaphore> p_impl)
	{
		return ::WaitForSingleObjectEx(p_impl, 0ul, FALSE) == WAIT_OBJECT_0;
	}
	bool semaphore::api::release(pimpl_t<semaphore> p_impl)
	{
		return ::ReleaseSemaphore(p_impl, 1, nullptr) == TRUE;
	}
	pimpl_t<sync_object> semaphore::api::get_sync_object(pimpl_t<semaphore> p_impl)
	{
		return reinterpret_cast<pimpl_t<sync_object>>(p_impl);
	}
	bool semaphore::api::destroy(pimpl_t<semaphore> p_impl)
	{
		return ::CloseHandle(p_impl) == TRUE;
	}
}
#else
static_assert("PLATFORM NOT SUPPORTED!")
#endif