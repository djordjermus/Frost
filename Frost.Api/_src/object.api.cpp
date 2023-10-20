#include <atomic>
#include "../object.api.hpp"

u64 _stdcall frost::api::object::get_reference_count(const frost::api::object* target)
{
	return target->get_reference_count();
}
void _stdcall frost::api::object::acquire_reference(const frost::api::object* target)
{
	return target->acquire_reference();
}
void _stdcall frost::api::object::release_reference(const frost::api::object* target)
{
	return target->release_reference();
}
