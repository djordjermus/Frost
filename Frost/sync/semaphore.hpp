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
		FROST_API semaphore(pimpl_crtp<semaphore> pimpl);
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

			static pimpl_crtp<semaphore> FROST_API create(i32 count, i32 max);
			static bool FROST_API acquire(pimpl_crtp<semaphore> p_impl);
			static bool FROST_API try_acquire(pimpl_crtp<semaphore> p_impl);
			static bool FROST_API release(pimpl_crtp<semaphore> p_impl);
			static pimpl_crtp<sync_object> FROST_API get_sync_object(pimpl_crtp<semaphore> p_impl);
			static bool FROST_API destroy(pimpl_crtp<semaphore> p_impl);
		};
	};
}