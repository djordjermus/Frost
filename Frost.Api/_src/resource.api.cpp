#include "../internal/resource.impl.hpp"
#include <typeinfo>
FROST_API u64 _stdcall frost_api_resource_get_reference_count(const frost::api::resource* target)
{
	return target->get_reference_count();
}
FROST_API void _stdcall frost_api_resource_acquire_reference(const frost::api::resource* target)
{
	return target->acquire_reference();
}
FROST_API void _stdcall frost_api_resource_release_reference(const frost::api::resource* target)
{
	return target->release_reference();
}
