#include "../macro.hpp"
#include "../pimpl.hpp"
#include "sync_object.hpp"

#pragma once
namespace frost::sync
{
	class semaphore final : public frost::pimpl_crtp<semaphore>
	{
	public:
		FROST_API semaphore();
		FROST_API semaphore(pimpl_t<semaphore> pimpl);
		FROST_API semaphore(i32 count, i32 maximum);
		FROST_API semaphore(semaphore& move);
		FROST_API semaphore(semaphore&& move) noexcept;
		semaphore& operator=(semaphore& move) = delete;
		semaphore& operator=(semaphore&& move) = delete;
		FROST_API ~semaphore();

		bool FROST_API acquire() const;
		bool FROST_API try_acquire() const;
		bool FROST_API release() const;
		bool FROST_API is_valid() const;
		sync_object FROST_API get_sync_object() const;


		class api final
		{
		public:
			STATIC_CLASS(api);

			static pimpl_t<semaphore> FROST_API create_semaphore(i32 count, i32 max);
			static bool FROST_API acquire_semaphore(pimpl_t<semaphore> p_impl);
			static bool FROST_API try_acquire_semaphore(pimpl_t<semaphore> p_impl);
			static bool FROST_API release_semaphore(pimpl_t<semaphore> p_impl);
			static pimpl_t<sync_object> FROST_API get_sync_object_from_semaphore(pimpl_t<semaphore> p_impl);
			static bool FROST_API destroy_semaphore(pimpl_t<semaphore> p_impl);
		};
	};
}