#include "Frost.Api/event_system.api.hpp"
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
		event_system_emit(tag, layers, event_data);
	}

	std::map<u64, layer_collection> event_system::init_event_system()
	{
		event_system_subscribe_relay(relay);
		event_system_subscribe_relay(log::relay);
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
}