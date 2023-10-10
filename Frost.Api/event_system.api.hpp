#include "primitives.hpp"
#include "macro.hpp"
#pragma once
namespace frost::api
{
	class event_system final
	{
	public:
		STATIC_CLASS(event_system);

		typedef void(_stdcall* handler)(void* p_data);
		typedef void(_stdcall* relay)(u64 tag, u64 layer, void* p_data);

		static void FROST_API emit(u64 tag, u64 layer, void* p_data);

		static void FROST_API subscribe(u64 tag, u64 activation_layers, handler handler);
		static void FROST_API unsubscribe(u64 tag, u64 activation_layers, handler handler);

		static void FROST_API subscribe_relay(relay relay);
		static void FROST_API unsubscribe_relay(relay relay);
	};
}
