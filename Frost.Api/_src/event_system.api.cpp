#include "../event_system.api.hpp"
#include <vector>
#include <map>
#pragma once

using  handler_group = std::vector<handler_sig>;
static std::vector<relay_sig> _relays;
static std::map<u64, std::map<u64, handler_group>> _handlers;

static inline void emit_to_handlers(u64 tag, u64 layer, void* p_data);

extern "C"
{
	FROST_API void _stdcall event_system_emit(u64 tag, u64 layer, void* p_data)
	{
		// Emit to handlers
		emit_to_handlers(tag, layer, p_data);

		// Emit to relays
		for (auto relay : _relays)
			relay(tag, layer, p_data);
	}

	FROST_API void _stdcall event_system_subscribe(u64 tag, u64 activation_layers, handler_sig handler)
	{
		_handlers[tag][activation_layers].emplace_back(handler);
	}
	FROST_API void _stdcall event_system_unsubscribe(u64 tag, u64 activation_layers, handler_sig handler)
	{
		auto layer_group = std::find(_handlers.begin(), _handlers.end(), tag);
		if (layer_group == _handlers.end())
			return;

		for (auto it = layer_group->second.begin(), jt = layer_group->second.end(); it != jt; it++)
		{
			if (it->first != activation_layers)
				continue;

			auto handler_it = std::find(it->second.begin(), it->second.end(), handler);
			if (handler_it == it->second.end())
				break;
			else
				it->second.erase(handler_it);
			break;
		}
	}

	FROST_API void _stdcall event_system_subscribe_relay(relay_sig relay)
	{
		_relays.emplace_back(relay);
	}
	FROST_API void _stdcall event_system_unsubscribe_relay(relay_sig relay)
	{
		auto it = std::find(_relays.begin(), _relays.end(), relay);
		if (it != _relays.end())
			_relays.erase(it);
	}
}

static inline void emit_to_handlers(u64 tag, u64 layer, void* p_data)
{
	auto layer_group = std::find(_handlers.begin(), _handlers.end(), tag);
	if (layer_group == _handlers.end())
		return;
	
	for (auto elem : layer_group->second)
	{
		if ((elem.first & layer) == 0)
			continue;

		for (auto handler : elem.second)
			handler(p_data);
	}
}
