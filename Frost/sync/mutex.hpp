#include "sync_object.hpp"
#include "../pimpl.hpp"
#pragma once
namespace frost::sync
{
	class mutex final : public frost::pimpl_crtp<mutex>
	{
	public:
		mutex(bool auto_acquire);
		mutex(mutex& move);
		mutex(mutex&& move) noexcept;
		mutex& operator=(mutex& move) = delete;
		mutex& operator=(mutex&& move) = delete;
		~mutex();

		bool acquire() const;
		bool try_acquire() const;
		bool release() const;
		bool is_valid() const;
		sync_object get_sync_object() const;
	};
}