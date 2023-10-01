#include "../sync_object.hpp"

#if defined(TARGET_BUILD_PLATFORM_WINDOWS)
#include <Windows.h>
#include <iostream>
namespace frost::sync
{
	sync_object::sync_object() :
		pimpl_crtp<sync_object>(nullptr) {}
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
		return api::acquire_one(
			reinterpret_cast<const pimpl_t<sync_object>*>(synchronization_objects.data()),
			static_cast<i32>(synchronization_objects.size()));
	}

	bool sync_object::acquire_all(const std::vector<sync_object>& synchronization_objects)
	{
		return api::acquire_all(
			reinterpret_cast<const pimpl_t<sync_object>*>(synchronization_objects.data()),
			static_cast<i32>(synchronization_objects.size()));
	}

	i32 sync_object::try_acquire_one(const std::vector<sync_object>& synchronization_objects)
	{
		return api::try_acquire_one(
			reinterpret_cast<const pimpl_t<sync_object>*>(synchronization_objects.data()),
			static_cast<i32>(synchronization_objects.size()));
	}

	bool sync_object::try_acquire_all(const std::vector<sync_object>& synchronization_objects)
	{
		return api::try_acquire_all(
			reinterpret_cast<const pimpl_t<sync_object>*>(synchronization_objects.data()),
			static_cast<i32>(synchronization_objects.size()));
	}



	i32 sync_object::api::acquire_one(const pimpl_t<sync_object>* synchronization_objects, i32 count)
	{
		if (count > MAXIMUM_WAIT_OBJECTS)
			return false;

		auto* data = const_cast<pimpl_t<sync_object>*>(synchronization_objects);
		auto result = ::WaitForMultipleObjects(static_cast<DWORD>(count), reinterpret_cast<HANDLE*>(data), FALSE, ~0u);
		if ((result >= WAIT_OBJECT_0) && (result < (WAIT_OBJECT_0 + count)))
			return result - WAIT_OBJECT_0;
		else
			return -1;
	}

	bool sync_object::api::acquire_all(const pimpl_t<sync_object>* synchronization_objects, i32 count)
	{
		if (count > MAXIMUM_WAIT_OBJECTS)
			return false;
		
		auto* data = const_cast<pimpl_t<sync_object>*>(synchronization_objects);
		auto result = ::WaitForMultipleObjects(static_cast<DWORD>(count), reinterpret_cast<HANDLE*>(data), TRUE, ~0u);

		return (result >= WAIT_OBJECT_0) && (result < (WAIT_OBJECT_0 + count));
	}

	i32 sync_object::api::try_acquire_one(const pimpl_t<sync_object>* synchronization_objects, i32 count)
	{
		if (count > MAXIMUM_WAIT_OBJECTS)
			return false;

		auto* data = const_cast<pimpl_t<sync_object>*>(synchronization_objects);
		auto result = ::WaitForMultipleObjects(static_cast<DWORD>(count), reinterpret_cast<HANDLE*>(data), FALSE, 0u);
		if ((result >= WAIT_OBJECT_0) && (result < (WAIT_OBJECT_0 + count)))
			return result - WAIT_OBJECT_0;
		else
			return -1;
		
	}

	bool sync_object::api::try_acquire_all(const pimpl_t<sync_object>* synchronization_objects, i32 count)
	{
		if (count > MAXIMUM_WAIT_OBJECTS)
			return false;

		auto* data = const_cast<pimpl_t<sync_object>*>(synchronization_objects);
		auto result = ::WaitForMultipleObjects(static_cast<DWORD>(count), reinterpret_cast<HANDLE*>(data), TRUE, 0u);

		return (result >= WAIT_OBJECT_0) && (result < (WAIT_OBJECT_0 + count));
	}
}
#else
static_assert("PLATFORM NOT SUPPORTED!")
#endif