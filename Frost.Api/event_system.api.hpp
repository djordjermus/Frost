#include "primitives.hpp"
#include "macro.hpp"
#pragma once
namespace frost::api
{
	class FROST_API event_system final
	{
	public:
		STATIC_CLASS(event_system);

		typedef void(_stdcall* handler)(void* p_data);
		typedef void(_stdcall* relay)(u64 tag, u64 layer, void* p_data);

		static void _stdcall emit(u64 tag, u64 layer, void* p_data);

		static void _stdcall subscribe(u64 tag, u64 activation_layers, handler handler);
		static void _stdcall unsubscribe(u64 tag, u64 activation_layers, handler handler);

		static void _stdcall subscribe_relay(relay relay);
		static void _stdcall unsubscribe_relay(relay relay);
	};
}
