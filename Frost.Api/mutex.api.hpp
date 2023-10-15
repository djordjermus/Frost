#include "primitives.hpp"
#include "macro.hpp"
#pragma once
namespace frost::api
{
	class sync_object;
	class FROST_API mutex final
	{
	public:

		static handle<mutex> _stdcall create(bool initial_owner);
		static bool _stdcall acquire(mutex* p_impl);
		static bool _stdcall try_acquire(mutex* p_impl);
		static bool _stdcall release(mutex* p_impl);
		static handle<sync_object> _stdcall get_sync_object(mutex* p_impl);
		static bool _stdcall destroy(mutex* p_impl);
	};
}