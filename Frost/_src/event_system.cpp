#include "../event_system.hpp"
#include "../log.hpp"
#include <utility>
#include <vector>
#include <map>
namespace frost
{
	using handler_collection = std::vector<event_system::handler<void>>;
	using layer_collection   = std::vector<std::pair<u64, handler_collection>>;
	std::map<u64, layer_collection> event_system::_handlers = init_event_system();


	void event_system::subscribe_tagged(u64 tag, u64 activation_layers, handler<void> procedure)
	{
		auto layers_it = _handlers.find(tag);
		if (layers_it == _handlers.end())
		{
		_handlers[tag];
		layers_it = _handlers.find(tag);
		  
		if (layers_it == _handlers.end())
		  	return; // FAIL
		}
		
		bool add = false;
		for (auto& handlers : layers_it->second)
		{
		 	if (handlers.first != activation_layers)
		 		continue;
		 	
			handlers.second.emplace_back(procedure);
			add = true;
			break;
		}
		
		if (!add)
		{
			auto& pair = layers_it->second.emplace_back();
			pair.first = activation_layers;
			pair.second = std::vector<handler<void>>();
			pair.second.emplace_back(procedure);
		}
	}

	void event_system::emit_tagged(u64 tag, u64 layers, void* event_data)
	{
		api::event_system_emit(tag, layers, event_data);
	}

	std::map<u64, layer_collection> event_system::init_event_system()
	{
		api::event_system_subscribe_relay(relay);
		api::event_system_subscribe_relay(log::relay);
		return std::map<u64, layer_collection>();
	}

	void event_system::relay(u64 tag, u64 layer, void* event_data)
	{
		auto layers_it = _handlers.find(tag);
		
		if (layers_it == _handlers.end())
			return;
		
		for (auto& it : layers_it->second)
		{
			if (it.first & layer)
			{
				for (auto handler : it.second)
					handler(event_data);
			}
		}
	}





	using  handler_group = std::vector<event_system::api::handler_sig>;
	static std::vector<event_system::api::relay_sig> relays;
	static std::map<u64, std::map<u64, handler_group>> handlers;

	static inline void emit_to_handlers(u64 tag, u64 layer, void* p_data);
	void event_system::api::event_system_emit(u64 tag, u64 layer, void* p_data)
	{
		// Emit to handlers
		emit_to_handlers(tag, layer, p_data);

		// Emit to relays
		for (auto relay : relays)
			relay(tag, layer, p_data);
	}

	void event_system::api::event_system_subscribe(u64 tag, u64 activation_layers, handler_sig handler)
	{
		handlers[tag][activation_layers].emplace_back(handler);
	}
	void event_system::api::event_system_unsubscribe(u64 tag, u64 activation_layers, handler_sig handler)
	{
		auto layer_group = _handlers.find(tag);
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

	void event_system::api::event_system_subscribe_relay(relay_sig relay)
	{
		relays.emplace_back(relay);
	}
	void event_system::api::event_system_unsubscribe_relay(relay_sig relay)
	{
		auto it = std::find(relays.begin(), relays.end(), relay);
		if (it != relays.end())
			relays.erase(it);
	}

	static inline void emit_to_handlers(u64 tag, u64 layer, void* p_data)
	{
		auto layer_group = handlers.find(tag);
		if (layer_group == handlers.end())
			return;

		for (auto elem : layer_group->second)
		{
			if ((elem.first & layer) == 0)
				continue;

			for (auto handler : elem.second)
				handler(p_data);
		}
	}
}