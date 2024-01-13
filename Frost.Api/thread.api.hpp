#include "resource.api.hpp"
#pragma once
extern "C"
{
	FROST_API frost::api::resource* _stdcall frost_api_thread_create(void(_stdcall* procedure)(void*), void* argument);
	FROST_API u64 _stdcall frost_api_thread_get_current_id();
	FROST_API u64 _stdcall frost_api_thread_get_id(frost::api::resource* thread);

	FROST_API frost::api::resource* _stdcall frost_api_thread_message_create();
	FROST_API void _stdcall frost_api_thread_message_wait(frost::api::resource* message);
	FROST_API void _stdcall frost_api_thread_message_peek(frost::api::resource* message);
	FROST_API void _stdcall frost_api_thread_message_dispatch(frost::api::resource* message);
	FROST_API void _stdcall frost_api_thread_message_discard(frost::api::resource* message);

	FROST_API bool _stdcall frost_api_thread_message_send(
		u64 thread,
		void(_stdcall* procedure)(void*),
		void* argument);
	FROST_API bool _stdcall frost_api_thread_message_send_async(
		frost::api::resource* sync_event,
		u64 thread,
		void(_stdcall* procedure)(void*),
		void* argument);
}