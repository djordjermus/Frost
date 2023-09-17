#include "Frost.Api/sync.api.hpp"
#include "../semaphore.hpp"
namespace frost::sync
{
	semaphore::semaphore(i32 count, i32 max) :
		pimpl_crtp<semaphore>(sync_create_semaphore(count, max)) {}
	semaphore::semaphore(semaphore& move) :
		pimpl_crtp<semaphore>(move) {}
	semaphore::semaphore(semaphore&& move) noexcept :
		pimpl_crtp<semaphore>(move) {}
	semaphore::~semaphore()
	{
		sync_destroy_semaphore(reinterpret_cast<frost_semaphore*>(get_pimpl()));
	}

	bool semaphore::acquire() const
	{
		return sync_acquire_semaphore(reinterpret_cast<frost_semaphore*>(get_pimpl()));
	}
	bool semaphore::try_acquire() const
	{
		return sync_try_acquire_semaphore(reinterpret_cast<frost_semaphore*>(get_pimpl()));
	}
	bool semaphore::release() const
	{
		return sync_release_semaphore(reinterpret_cast<frost_semaphore*>(get_pimpl()));
	}
	bool semaphore::is_valid() const
	{
		return get_pimpl() != nullptr;
	}
	sync_object semaphore::get_sync_object() const
	{
		auto result = sync_object();
		result.swap_pimpl(reinterpret_cast<frost::pimpl_t<sync_object>>(
			sync_get_sync_object_from_semaphore(reinterpret_cast<frost_semaphore*>(get_pimpl()))));
		return result;
	}
}