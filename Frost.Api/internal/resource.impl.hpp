#include "../resource.api.hpp"
#include <atomic>
#pragma once
namespace frost::api 
{
	class resource
	{
	private:
		mutable std::atomic<u64> _reference_count;

	protected:
		virtual ~resource() = default;

	public:
		u64 get_reference_count() const;
		void acquire_reference() const;
		void release_reference() const;
	};
}
