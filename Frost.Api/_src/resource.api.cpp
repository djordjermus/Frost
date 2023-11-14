#include "../internal/resource.impl.hpp"
#include <typeinfo>
u64 resource_get_reference_count(const frost::api::resource* target)
{
	return target->get_reference_count();
}
void resource_acquire_reference(const frost::api::resource* target)
{
	return target->acquire_reference();
}
void resource_release_reference(const frost::api::resource* target)
{
	return target->release_reference();
}
