#include "object.hpp"
#include "../ref.hpp"
using namespace frost::api;
static constexpr u64 msg_execute_procedure_asynchronous = 0xBFFF;

static thread_reference* create_local_thread_reference();
static u32 _stdcall thread_procedure(void* info);
thread_local ref current_thread_reference = create_local_thread_reference();

struct thread_startup_info
{
	void(_stdcall* procedure)(void* argument);
	void* argument;
	inline thread_startup_info(void(_stdcall* procedure)(void* argument), void* argument)
	{
		this->procedure = procedure;
		this->argument = argument;
	}
};

struct procedure_message_info
{
	void(_stdcall* procedure)(void* argument);
	void* argument;
};


FROST_API object* _stdcall frost_api_thread_create(void(_stdcall* procedure)(void*), void* argument)
{
	auto result = new thread();
	result->type = object_type::thread;
	DWORD id = 0;
	auto info = new thread_startup_info(procedure, argument);
	result->handle = ::CreateThread(nullptr, 0, thread_procedure, info, 0, &id);
	result->thread_id = id;
	if (result->handle == nullptr)
		delete info;

	return result;
}
FROST_API object* _stdcall frost_api_thread_get_current()
{
	return current_thread_reference.get();
}

FROST_API object* _stdcall frost_api_thread_message_create()
{
	auto result = new thread_message();
	result->type = object_type::thread_message;
	result->message = {};
	return result;
}
FROST_API void _stdcall frost_api_thread_message_dispatch(object* message)
{
	if (message->type != object_type::thread_message)
		return;
	auto _msg = ((thread_message*)message)->message;

	if (_msg.message == msg_execute_procedure_asynchronous)
	{
		if (_msg.wParam != 0)
		{
			try
			{
				auto* info = reinterpret_cast<procedure_message_info*>(_msg.wParam);
				if (info->procedure)
					info->procedure(info->argument);
			}
			catch (...) { /* ... */ }
		}

		if (_msg.lParam != 0)
		{
			auto sync = reinterpret_cast<object*>(_msg.lParam);
			try
			{
				frost_api_synchronizable_signal(sync);
			}
			catch (...) { /* ... */ }
			try
			{
				frost_api_object_release_reference(sync);
			}
			catch (...) { /* ... */ }
		}
	}
	else
	{
		::TranslateMessage(&_msg);
		::DispatchMessageW(&_msg);
	}
	((thread_message*)message)->message = {};
}
FROST_API void _stdcall frost_api_thread_message_discard(object* message)
{
	if (message->type != object_type::thread_message)
		return;
	auto _msg = ((thread_message*)message)->message;

	if (_msg.message == msg_execute_procedure_asynchronous)
	{
		if (_msg.wParam != 0)
		{
			auto* info = reinterpret_cast<procedure_message_info*>(_msg.wParam);
			delete info;
		}
		if (_msg.lParam != 0)
		{
			auto sync = reinterpret_cast<object*>(_msg.lParam);
			try
			{
				frost_api_synchronizable_signal(sync);
			}
			catch (...) { /* ... */ }
			try
			{
				frost_api_object_release_reference(sync);
			}
			catch (...) { /* ... */ }
		}
	}
	_msg = {};
}

FROST_API bool _stdcall frost_api_thread_message_send(
	object* thread,
	void(_stdcall* procedure)(void*),
	void* argument)
{
	switch (thread->type)
	{
	case object_type::thread_reference:
	case object_type::thread:
		break;
	default:
		return false;
	}

	auto handle = ((frost::api::thread*)thread)->handle;
	if (handle == nullptr)
		return false;

	auto id = ::GetThreadId(handle);
	if (id == 0)
		return false;

	auto info = procedure_message_info();
	info.procedure = procedure;
	info.argument = argument;

	auto sync = frost_api_synchronizable_create_event();
	frost_api_object_acquire_reference(sync);

	BOOL result = ::PostThreadMessageW(
		id,
		msg_execute_procedure_asynchronous,
		reinterpret_cast<WPARAM>(&info),
		reinterpret_cast<LPARAM>(sync));
	if (!!result)
	{
		object* syncs[] = { sync, thread };
		auto ix = frost_api_synchronizable_wait_one(syncs, 2);
		if (ix != 0)
		{
			frost_api_object_release_reference(sync);
			return false;
		}
		return true;
	}
	else
	{
		frost_api_object_release_reference(sync);
		return false;
	}
}
FROST_API bool _stdcall frost_api_thread_message_send_async(
	object* thread,
	object* sync,
	void(_stdcall* procedure)(void*),
	void* argument)
{
	switch (thread->type)
	{
	case object_type::thread_reference:
	case object_type::thread:
		break;
	default:
		return false;
	}

	switch (sync->type)
	{
	case object_type::sync_mutex:
	case object_type::sync_semaphore:
	case object_type::sync_event:
		break;
	default:
		return false;
	}

	auto* info = new procedure_message_info();
	info->procedure = procedure;
	info->argument = argument;

	auto handle = ((frost::api::thread*)thread)->handle;
	if (handle == nullptr)
		return false;

	auto id = ::GetThreadId(handle);
	if (id == 0)
		return false;

	BOOL result = ::PostThreadMessageW(
		id,
		msg_execute_procedure_asynchronous,
		reinterpret_cast<WPARAM>(info),
		reinterpret_cast<LPARAM>(sync));

	return !!result;
}
static thread_reference* create_local_thread_reference()
{
	auto result = new thread_reference();
	auto thread_id		= ::GetCurrentThreadId();
	result->type		= object_type::thread_reference;
	result->handle		= ::OpenThread(THREAD_ALL_ACCESS, FALSE, ::GetCurrentThreadId());
	result->thread_id	= thread_id;

	return result;
}

static u32 _stdcall thread_procedure(void* info)
{
	thread_startup_info* tsi = reinterpret_cast<thread_startup_info*>(info);
	tsi->procedure(tsi->argument);
	delete tsi;
	return 0;
}
