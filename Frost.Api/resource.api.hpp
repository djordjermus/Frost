#include "primitives.hpp"
#include "macro.hpp"
#pragma once
namespace frost::api { class resource; }

extern "C"
{
	FROST_API u64  _stdcall resource_get_reference_count(const frost::api::resource* target);
	FROST_API u64  _stdcall resource_get_type_hash(const frost::api::resource* target);
	FROST_API void _stdcall resource_acquire_reference(const frost::api::resource* target);
	FROST_API void _stdcall resource_release_reference(const frost::api::resource* target);
}
