#include "../resource.api.hpp"
#include <atomic>
#pragma once
namespace frost::impl
{
	class resource
	{
	private:
		mutable std::atomic<u64> _reference_count;
	protected:
		virtual ~resource() = default;
		u64 impl_get_reference_count() const;
		void impl_acquire_reference() const;
		void impl_release_reference() const;
	};
}

#define AUTO_RESOURCE_INTERFACE_DECL()\
	u64 get_reference_count() const override;\
	void acquire_reference() const override;\
	void release_reference() const override;\

#define AUTO_RESOURCE_INTERFACE_IMPL(T)\
	u64 T::get_reference_count() const { return this->impl::resource::impl_get_reference_count(); }\
	void T::acquire_reference() const { return this->impl::resource::impl_acquire_reference(); }\
	void T::release_reference() const { return this->impl::resource::impl_release_reference(); }
