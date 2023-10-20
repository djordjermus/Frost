#include "primitives.hpp"
#include "macro.hpp"
#pragma once
namespace frost::api
{
	class object
	{
	public:
		virtual ~object() = default;
		virtual u64 get_reference_count() const = 0;
		virtual void acquire_reference() const = 0;
		virtual void release_reference() const = 0;

	private:
		static FROST_API u64 _stdcall get_reference_count(const object* target);
		static FROST_API void _stdcall acquire_reference(const object* target);
		static FROST_API void _stdcall release_reference(const object* target);
	};
}