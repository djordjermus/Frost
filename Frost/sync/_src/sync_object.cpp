#include "Frost.Api/sync_object.api.hpp"
#include "../sync_object.hpp"

#if defined(TARGET_BUILD_PLATFORM_WINDOWS)
#include <Windows.h>
#include <iostream>
namespace frost::sync
{
	sync_object::sync_object() :
		pimpl_crtp<sync_object>() {}
	sync_object::sync_object(pimpl_t<sync_object> pimpl) :
		pimpl_crtp<sync_object>(pimpl) {}
	sync_object::sync_object(const sync_object& copy) :
		pimpl_crtp<sync_object>(copy.get_pimpl()) {}
	sync_object::sync_object(const sync_object&& copy) noexcept :
		pimpl_crtp<sync_object>(copy.get_pimpl()) {}

	bool sync_object::is_valid() const
	{
		return get_pimpl() != nullptr;
	}

	i32 sync_object::acquire_one(const std::vector<sync_object>& synchronization_objects)
	{
		return api::sync_object::acquire_one(
			reinterpret_cast<const frost::api::sync_object**>(const_cast<sync_object*>(synchronization_objects.data())),
			static_cast<i32>(synchronization_objects.size()));
	}

	bool sync_object::acquire_all(const std::vector<sync_object>& synchronization_objects)
	{
		return api::sync_object::acquire_all(
			reinterpret_cast<const frost::api::sync_object**>(const_cast<sync_object*>(synchronization_objects.data())),
			static_cast<i32>(synchronization_objects.size()));
	}

	i32 sync_object::try_acquire_one(const std::vector<sync_object>& synchronization_objects)
	{
		return api::sync_object::try_acquire_one(
			reinterpret_cast<const frost::api::sync_object**>(const_cast<sync_object*>(synchronization_objects.data())),
			static_cast<i32>(synchronization_objects.size()));
	}

	bool sync_object::try_acquire_all(const std::vector<sync_object>& synchronization_objects)
	{
		return api::sync_object::try_acquire_all(
			reinterpret_cast<const frost::api::sync_object**>(const_cast<sync_object*>(synchronization_objects.data())),
			static_cast<i32>(synchronization_objects.size()));
	}
}
#else
static_assert("PLATFORM NOT SUPPORTED!")
#endif