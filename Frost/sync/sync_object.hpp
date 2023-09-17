#include "Frost.Api/primitives.hpp"
#include "../pimpl.hpp"
#include <vector>
#pragma once
namespace frost::sync
{
	class sync_object final : public frost::pimpl_crtp<sync_object>
	{
	public:
		sync_object();
		sync_object(const sync_object& copy);
		sync_object(const sync_object&& copy);
		~sync_object() = default;
		static i32 acquire_one(const std::vector<sync_object>& synchronization_objects);
		static bool acquire_all(const std::vector<sync_object>& synchronization_objects);
		static i32 try_acquire_one(const std::vector<sync_object>& synchronization_objects);
		static bool try_acquire_all(const std::vector<sync_object>& synchronization_objects);
	};
}