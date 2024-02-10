#include "primitives.hpp"
#include "macro.hpp"
#pragma once
namespace frost::api { class resource; }

extern "C"
{
	FROST_API u64  _stdcall frost_api_resource_get_reference_count(frost::api::resource* target);
	FROST_API void _stdcall frost_api_resource_acquire_reference(frost::api::resource* target);
	FROST_API void _stdcall frost_api_resource_release_reference(frost::api::resource* target);
}
