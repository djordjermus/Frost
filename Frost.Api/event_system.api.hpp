#include "macro.hpp"
#include "primitives.hpp"
#pragma once
extern "C"
{
	typedef void(_stdcall *finalizer_sig)(void* p_data);
	typedef void(_stdcall *handler_sig)(void* p_data);
	typedef void(_stdcall *relay_sig)(u64 tag, u64 layer, void* p_data);

	FROST_API void _stdcall event_system_emit(u64 tag, u64 layer, void* p_data);

	FROST_API void _stdcall event_system_subscribe(u64 tag, u64 activation_layers, handler_sig handler);
	FROST_API void _stdcall event_system_unsubscribe(u64 tag, u64 activation_layers, handler_sig handler);

	FROST_API void _stdcall event_system_subscribe_relay(relay_sig relay);
	FROST_API void _stdcall event_system_unsubscribe_relay(relay_sig relay);
}