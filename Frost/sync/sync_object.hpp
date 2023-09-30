#include "../macro.hpp"
#include "../primitives.hpp"
#include "../pimpl.hpp"
#include <vector>
#pragma once
namespace frost::sync
{
	class sync_object final : public frost::pimpl_crtp<sync_object>
	{
	public:
		FROST_API sync_object();
		FROST_API sync_object(pimpl_crtp<sync_object> pimpl);
		FROST_API sync_object(const sync_object& copy);
		FROST_API sync_object(const sync_object&& copy) noexcept;
		~sync_object() = default;

		static i32  FROST_API acquire_one(const std::vector<sync_object>& synchronization_objects);
		static bool FROST_API acquire_all(const std::vector<sync_object>& synchronization_objects);
		static i32  FROST_API try_acquire_one(const std::vector<sync_object>& synchronization_objects);
		static bool FROST_API try_acquire_all(const std::vector<sync_object>& synchronization_objects);

		class api final
		{
		public:
			STATIC_CLASS(api);

			static i32  FROST_API acquire_one(const pimpl_t<sync_object>* synchronization_objects, i32 count);
			static bool FROST_API acquire_all(const pimpl_t<sync_object>* synchronization_objects, i32 count);
			static i32  FROST_API try_acquire_one(const pimpl_t<sync_object>* synchronization_objects, i32 count);
			static bool FROST_API try_acquire_all(const pimpl_t<sync_object>* synchronization_objects, i32 count);
		};
	};
}