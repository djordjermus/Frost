#include "Frost.Api/sync.api.hpp"
#include "../sync_object.hpp"
namespace frost::sync
{
	sync_object::sync_object() :
		pimpl_crtp<sync_object>(nullptr) {}
	sync_object::sync_object(const sync_object& copy) :
		pimpl_crtp<sync_object>(copy.get_pimpl()) {}
	sync_object::sync_object(const sync_object&& copy) :
		pimpl_crtp<sync_object>(copy.get_pimpl()) {}

	i32 sync_object::acquire_one(const std::vector<sync_object>& synchronization_objects)
	{
		auto ptr = reinterpret_cast<frost_sync_object* const*>(&(*synchronization_objects.cbegin()));
		return sync_acquire_one(const_cast<frost_sync_object**>(ptr), static_cast<i32>(synchronization_objects.size()));
	}

	bool  sync_object::acquire_all(const std::vector<sync_object>& synchronization_objects)
	{
		auto ptr = reinterpret_cast<frost_sync_object* const*>(&(*synchronization_objects.cbegin()));
		return sync_acquire_all(const_cast<frost_sync_object**>(ptr), static_cast<i32>(synchronization_objects.size()));
	}

	i32 sync_object::try_acquire_one(const std::vector<sync_object>& synchronization_objects)
	{
		auto ptr = reinterpret_cast<frost_sync_object* const*>(&(*synchronization_objects.cbegin()));
		return sync_try_acquire_one(const_cast<frost_sync_object**>(ptr), static_cast<i32>(synchronization_objects.size()));
	}

	bool  sync_object::try_acquire_all(const std::vector<sync_object>& synchronization_objects)
	{
		auto ptr = reinterpret_cast<frost_sync_object* const*>(&(*synchronization_objects.cbegin()));
		return sync_try_acquire_all(const_cast<frost_sync_object**>(ptr), static_cast<i32>(synchronization_objects.size()));
	}
}