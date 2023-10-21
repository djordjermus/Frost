#include "primitives.hpp"
#include "macro.hpp"
#pragma once

extern "C"
{
	typedef void(_stdcall* event_system_handler)(void* p_data);
	typedef void(_stdcall* event_system_relay)(u64 tag, u64 layer, void* p_data);

	FROST_API void _stdcall event_system_emit(u64 tag, u64 layer, void* p_data);

	FROST_API void _stdcall event_system_subscribe(u64 tag, u64 activation_layers, event_system_handler handler);
	FROST_API void _stdcall event_system_unsubscribe(u64 tag, u64 activation_layers, event_system_handler handler);

	FROST_API void _stdcall event_system_subscribe_relay(event_system_relay relay);
	FROST_API void _stdcall event_system_unsubscribe_relay(event_system_relay relay);
}
