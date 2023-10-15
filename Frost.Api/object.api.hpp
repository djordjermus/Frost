#include "primitives.hpp"
#include "macro.hpp"
#pragma once
namespace frost::api
{
	class FROST_API object
	{
	private:
		u64 _reference_count = 0;
	protected:
		virtual ~object() = default;

	public:
		inline u64  get_reference_count() { return get_reference_count(this); }
		inline void acquire_reference() { return acquire_reference(this); }
		inline void release_reference() { return release_reference(this); }

		static u64 _stdcall get_reference_count(object* target);
		static void _stdcall acquire_reference(object* target);
		static void _stdcall release_reference(object* target);
	};
}