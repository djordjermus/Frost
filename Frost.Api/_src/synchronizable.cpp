#include <memory>
#include "object.hpp"
using namespace frost::api;
using namespace frost::impl;

FROST_API bool _stdcall frost_api_synchronizable_wait(object* target)
{
	switch (target->type)
	{
	case object_type::sync_mutex:
	case object_type::sync_semaphore:
	case object_type::sync_event:
	case object_type::thread_reference:
	case object_type::thread:
	case object_type::window:
		return ::WaitForSingleObject(((system_handle_host*)target)->handle, ~0ul) == WAIT_OBJECT_0;
	case object_type::thread_message:
		return ::GetMessageW(&((thread_message*)target)->message, nullptr, 0, 0) != -1;
	default:
		return false;
	}
}

FROST_API bool _stdcall frost_api_synchronizable_try_wait(object* target)
{
	switch (target->type)
	{
	case object_type::sync_mutex:
	case object_type::sync_semaphore:
	case object_type::sync_event:
	case object_type::thread_reference:
	case object_type::thread:
	case object_type::window:
		return ::WaitForSingleObject(((system_handle_host*)target)->handle, 0ul) == WAIT_OBJECT_0;
	case object_type::thread_message:
		return ::PeekMessageW(&((thread_message*)target)->message, nullptr, 0, 0, PM_REMOVE) != 0;
	default:
		return false;
	}
}

FROST_API bool _stdcall frost_api_synchronizable_signal(object* target)
{
	switch (target->type)
	{
	case object_type::sync_mutex:
		return !!::ReleaseMutex(((system_handle_host*)target)->handle);
	case object_type::sync_semaphore:
		return !!::ReleaseSemaphore(((system_handle_host*)target)->handle, 1, nullptr);
	case object_type::sync_event:
		return !!::SetEvent(((system_handle_host*)target)->handle);
	default:
		return false;
	}
}

FROST_API bool _stdcall frost_api_synchronizable_event_reset(object* target)
{
	switch (target->type)
	{
	case object_type::sync_event:
		return !!::ResetEvent(((system_handle_host*)target)->handle);
	default:
		return false;
	}
}



FROST_API i32 _stdcall frost_api_synchronizable_wait_one(object* const* target_list, i32 count)
{
	if (count > MAXIMUM_WAIT_OBJECTS)
		return false;

	auto* data = reinterpret_cast<HANDLE*>(_malloca(sizeof(HANDLE) * count));
	if (data == nullptr)
		return false;

	for (i32 i = 0; i < count; i++)
	{
		switch (target_list[i]->type)
		{
		case object_type::sync_mutex:
		case object_type::sync_semaphore:
		case object_type::sync_event:
		case object_type::thread_reference:
		case object_type::thread:
		case object_type::window:
			data[i] = ((system_handle_host*)target_list[i])->handle;
			break;

		default:
			_freea(data);
			return false;
		}
	}

	auto result = ::WaitForMultipleObjects(static_cast<DWORD>(count), reinterpret_cast<HANDLE*>(data), FALSE, ~0u);

	_freea(data);

	if ((result >= WAIT_OBJECT_0) && (result < (WAIT_OBJECT_0 + count)))
		return result - WAIT_OBJECT_0;
	else
		return -1;
}

FROST_API bool _stdcall frost_api_synchronizable_wait_all(object* const* target_list, i32 count)
{
	if (count > MAXIMUM_WAIT_OBJECTS)
		return false;

	auto* data = reinterpret_cast<HANDLE*>(_malloca(sizeof(HANDLE) * count));
	if (data == nullptr)
		return false;

	for (i32 i = 0; i < count; i++)
	{
		switch (target_list[i]->type)
		{
		case object_type::sync_mutex:
		case object_type::sync_semaphore:
		case object_type::sync_event:
		case object_type::thread_reference:
		case object_type::thread:
		case object_type::window:
			data[i] = ((system_handle_host*)target_list[i])->handle;
			break;

		default:
			_freea(data);
			return false;
		}
	}

	auto result = ::WaitForMultipleObjects(static_cast<DWORD>(count), reinterpret_cast<HANDLE*>(data), TRUE, ~0u);

	_freea(data);

	return (result >= WAIT_OBJECT_0) && (result < (WAIT_OBJECT_0 + count));
}

FROST_API i32  _stdcall frost_api_synchronizable_try_wait_one(object* const* target_list, i32 count)
{
	if (count > MAXIMUM_WAIT_OBJECTS)
		return false;

	auto* data = reinterpret_cast<HANDLE*>(_malloca(sizeof(HANDLE) * count));
	if (data == nullptr)
		return false;

	for (i32 i = 0; i < count; i++)
	{
		switch (target_list[i]->type)
		{
		case object_type::sync_mutex:
		case object_type::sync_semaphore:
		case object_type::sync_event:
		case object_type::thread_reference:
		case object_type::thread:
		case object_type::window:
			data[i] = ((system_handle_host*)target_list[i])->handle;
			break;

		default:
			_freea(data);
			return false;
		}
	}

	auto result = ::WaitForMultipleObjects(static_cast<DWORD>(count), reinterpret_cast<HANDLE*>(data), FALSE, 0u);

	_freea(data);

	if ((result >= WAIT_OBJECT_0) && (result < (WAIT_OBJECT_0 + count)))
		return result - WAIT_OBJECT_0;
	else
		return -1;
}

FROST_API bool _stdcall frost_api_synchronizable_try_wait_all(object* const* target_list, i32 count)
{
	if (count > MAXIMUM_WAIT_OBJECTS)
		return false;

	auto* data = reinterpret_cast<HANDLE*>(_malloca(sizeof(HANDLE) * count));
	if (data == nullptr)
		return false;

	for (i32 i = 0; i < count; i++)
	{
		switch (target_list[i]->type)
		{
		case object_type::sync_mutex:
		case object_type::sync_semaphore:
		case object_type::sync_event:
		case object_type::thread_reference:
		case object_type::thread:
		case object_type::window:
			data[i] = ((system_handle_host*)target_list[i])->handle;
			break;

		default:
			_freea(data);
			return false;
		}
	}

	auto result = ::WaitForMultipleObjects(static_cast<DWORD>(count), reinterpret_cast<HANDLE*>(data), TRUE, 0u);

	_freea(data);

	return (result >= WAIT_OBJECT_0) && (result < (WAIT_OBJECT_0 + count));
}



FROST_API object* _stdcall frost_api_synchronizable_create_mutex(bool initial_owner)
{
	auto result = new sync_mutex();
	result->type = object_type::sync_mutex;
	result->handle = ::CreateMutexW(nullptr, initial_owner, nullptr);
	return result;
}

FROST_API object* _stdcall frost_api_synchronizable_create_semaphore(i32 count, i32 max)
{
	auto result = new sync_semaphore();
	result->type = object_type::sync_semaphore;
	result->handle = ::CreateSemaphoreW(nullptr, count, max, nullptr);
	return result;
}

FROST_API object* _stdcall frost_api_synchronizable_create_event()
{
	auto result = new sync_event();
	result->type = object_type::sync_event;
	result->handle = ::CreateEventW(nullptr, TRUE, FALSE, nullptr);
	return result;
}
