#include "../macro.hpp"
#include "../pimpl.hpp"
#include "sync_object.hpp"

#pragma once
namespace frost::sync
{
	class mutex final : public frost::pimpl_crtp<mutex>
	{
	public:
		FROST_API mutex(bool auto_acquire);
		FROST_API mutex(mutex& move);
		FROST_API mutex(mutex&& move) noexcept;
		mutex& operator=(mutex& move) = delete;
		mutex& operator=(mutex&& move) = delete;
		FROST_API ~mutex();

		bool FROST_API acquire() const;
		bool FROST_API try_acquire() const;
		bool FROST_API release() const;
		bool FROST_API is_valid() const;
		sync_object FROST_API get_sync_object() const;


		class api final
		{
		public:
			STATIC_CLASS(api);

			static pimpl_t<mutex> FROST_API create_mutex(bool initial_owner);
			static bool FROST_API acquire_mutex(pimpl_t<mutex> p_impl);
			static bool FROST_API try_acquire_mutex(pimpl_t<mutex> p_impl);
			static bool FROST_API release_mutex(pimpl_t<mutex> p_impl);
			static pimpl_t<sync_object> FROST_API get_sync_object_from_mutex(pimpl_t<mutex> p_impl);
			static bool FROST_API destroy_mutex(pimpl_t<mutex> p_impl);
		};
	};
}