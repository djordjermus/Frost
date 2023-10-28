#include "primitives.hpp"
#include "macro.hpp"
#pragma once
namespace frost::api
{
	class resource
	{
	public:
		virtual ~resource() = default;
		virtual u64 get_reference_count() const = 0;
		virtual void acquire_reference() const = 0;
		virtual void release_reference() const = 0;

	private:
	};
}

extern "C"
{
	FROST_API u64 _stdcall  resource_get_reference_count(const frost::api::resource* target);
	FROST_API void _stdcall resource_acquire_reference(const  frost::api::resource* target);
	FROST_API void _stdcall resource_release_reference(const  frost::api::resource* target);
}
