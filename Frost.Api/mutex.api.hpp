#include "primitives.hpp"
#include "macro.hpp"
#pragma once
namespace frost::api
{
	class sync_object;
	class mutex final
	{
	public:

		static handle<mutex> FROST_API create(bool initial_owner);
		static bool FROST_API acquire(mutex* p_impl);
		static bool FROST_API try_acquire(mutex* p_impl);
		static bool FROST_API release(mutex* p_impl);
		static handle<sync_object> FROST_API get_sync_object(mutex* p_impl);
		static bool FROST_API destroy(mutex* p_impl);
	};
}