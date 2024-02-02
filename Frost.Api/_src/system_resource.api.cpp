#include "../internal/system_resource.impl.hpp"
#include "../system_resource.api.hpp"
FROST_API void* _stdcall frost_api_system_resource_get_system_handle(frost::api::resource* target)
{
	return static_cast<frost::impl::system_resource*>(target)->get_system_handle();
}