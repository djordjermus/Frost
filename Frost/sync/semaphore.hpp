#include "sync_object.hpp"
#include "../pimpl.hpp"
#pragma once
namespace frost::sync
{
	class semaphore final : public frost::pimpl_crtp<semaphore>
	{
	public:
		semaphore(i32 count, i32 maximum);
		semaphore(semaphore& move);
		semaphore(semaphore&& move) noexcept;
		semaphore& operator=(semaphore& move) = delete;
		semaphore& operator=(semaphore&& move) = delete;
		~semaphore();

		bool acquire() const;
		bool try_acquire() const;
		bool release() const;
		bool is_valid() const;
		sync_object get_sync_object() const;
	};
}