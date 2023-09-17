#include "Frost.Api/sync.api.hpp"
#include "../mutex.hpp"
namespace frost::sync
{
	mutex::mutex(bool auto_acquire) :
		pimpl_crtp<mutex>(sync_create_mutex(auto_acquire)) {}
	mutex::mutex(mutex& move) :
		pimpl_crtp<mutex>(move) {}
	mutex::mutex(mutex&& move) noexcept :
		pimpl_crtp<mutex>(move) {}
	mutex::~mutex()
	{
		sync_destroy_mutex(reinterpret_cast<frost_mutex*>(get_pimpl()));
	}

	bool mutex::acquire() const
	{
		return sync_acquire_mutex(reinterpret_cast<frost_mutex*>(get_pimpl()));
	}
	bool mutex::try_acquire() const
	{
		return sync_try_acquire_mutex(reinterpret_cast<frost_mutex*>(get_pimpl()));
	}
	bool mutex::release() const
	{
		return sync_release_mutex(reinterpret_cast<frost_mutex*>(get_pimpl()));
	}
	bool mutex::is_valid() const
	{
		return get_pimpl() != nullptr;
	}
	sync_object mutex::get_sync_object() const
	{
		auto result = sync_object();
		result.swap_pimpl(reinterpret_cast<frost::pimpl_t<sync_object>>(
			sync_get_sync_object_from_mutex(reinterpret_cast<frost_mutex*>(get_pimpl()))));
		return result;
	}
}