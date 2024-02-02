#include "../internal/thread.impl.hpp"
#include "../thread.api.hpp"
#include "../ref.hpp"
#define TO_IMPL(x) static_cast<frost::impl::thread*>(x)
#define TO_IMPL_MSG(x) static_cast<frost::impl::thread::message*>(x)

FROST_API frost::api::resource* _stdcall frost_api_thread_create(void(_stdcall* procedure)(void*), void* argument)
{
	return new frost::impl::thread(procedure, argument);
}
FROST_API frost::api::resource* _stdcall frost_api_thread_get_current()
{
	return frost::impl::thread_reference::get_current();
}
FROST_API u64 _stdcall frost_api_thread_get_current_id()
{
	return frost::impl::thread::get_current_id();
}
FROST_API u64 _stdcall frost_api_thread_get_id(frost::api::resource* thread)
{
	return TO_IMPL(thread)->get_id();
}

FROST_API frost::api::resource* _stdcall frost_api_thread_message_create()
{
	return frost::impl::thread::message::create();
}
FROST_API void _stdcall frost_api_thread_message_wait(frost::api::resource* message)
{
	TO_IMPL_MSG(message)->wait();
}
FROST_API void _stdcall frost_api_thread_message_peek(frost::api::resource* message)
{
	TO_IMPL_MSG(message)->peek();
}
FROST_API void _stdcall frost_api_thread_message_dispatch(frost::api::resource* message)
{
	TO_IMPL_MSG(message)->dispatch();
}
FROST_API void _stdcall frost_api_thread_message_discard(frost::api::resource* message)
{
	TO_IMPL_MSG(message)->discard();
}

FROST_API bool _stdcall frost_api_thread_message_send(
	frost::api::resource* thread,
	void(_stdcall* procedure)(void*),
	void* argument)
{
	auto sync = new frost::impl::synchronizable_event();
	sync->acquire_reference();
	bool success = frost::impl::thread::message::send(
		static_cast<frost::impl::thread_reference*>(thread),
		sync,
		procedure,
		argument);
	
	if (success)
	{
		frost::impl::synchronizable* arr[2] = { sync, static_cast<frost::impl::synchronizable*>(thread)};
		auto x = frost::impl::synchronizable::wait_one(arr, 2);
	
		sync->release_reference();
		return x == 0;
	}
	else
	{
		auto x = ::GetLastError();
		sync->release_reference();
		return success;
	}
}
FROST_API bool _stdcall frost_api_thread_message_send_async(
	frost::api::resource* thread,
	frost::api::resource* sync,
	void(_stdcall* procedure)(void*),
	void* argument)
{
	return frost::impl::thread::message::send(static_cast<frost::impl::thread_reference*>(thread), static_cast<frost::impl::synchronizable*>(sync), procedure, argument);
}
