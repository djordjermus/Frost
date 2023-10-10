#include "primitives.hpp"
#include "macro.hpp"
#include <utility>
#include <vector>
#include <map>
#pragma once
namespace frost
{
	class event_system final
	{
	public:
		template<class T>
		using handler = void(_stdcall*)(T* event_data);
		template<class T>
		static void subscribe(u64 activation_layers, handler<T> procedure)
		{
			return subscribe_tagged(
				typeid(T).hash_code(), 
				activation_layers, 
				reinterpret_cast<handler<void>>(procedure));
		}

		template<class T>
		static void emit(u64 layers, T* event_data)
		{
			return emit_tagged(
				typeid(T).hash_code(),
				layers,
				event_data);
		}

		static void subscribe_tagged(u64 tag, u64 activation_layers, handler<void> procedure);
 
		static void emit_tagged(u64 tag, u64 layers, void* event_data);

	private:
		using handler_collection = std::vector<event_system::handler<void>>;
		using layer_collection = std::vector<std::pair<u64, handler_collection>>;

		event_system() = delete;
		event_system(event_system&) = delete;
		event_system(event_system&&) = delete;
		~event_system() = delete;

		static std::map<u64, layer_collection> init_event_system();
		static void relay(u64 tag, u64 layer, void* event_data);

		static std::map<u64, layer_collection> _handlers;
	};
}