#include <vector>
#include <map>
#include "../event_system.api.hpp"
using handler_group		= std::vector<frost::api::event_system::handler>;
using handler_collection	= std::vector<frost::api::event_system::handler>;
using layer_collection		= std::vector<std::pair<u64, handler_collection>>;

static std::vector<frost::api::event_system::relay> relays;
static std::map<u64, std::map<u64, handler_group>> handlers;
static std::map<u64, layer_collection> _handlers;

static inline void emit_to_handlers(u64 tag, u64 layer, void* p_data);
void frost::api::event_system::emit(u64 tag, u64 layer, void* p_data)
{
	// Emit to handlers
	emit_to_handlers(tag, layer, p_data);

	// Emit to relays
	for (auto relay : relays)
		relay(tag, layer, p_data);
}

void frost::api::event_system::subscribe(u64 tag, u64 activation_layers, handler handler)
{
	handlers[tag][activation_layers].emplace_back(handler);
}
void frost::api::event_system::unsubscribe(u64 tag, u64 activation_layers, handler handler)
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

void frost::api::event_system::subscribe_relay(relay relay)
{
	relays.emplace_back(relay);
}
void frost::api::event_system::unsubscribe_relay(relay relay)
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