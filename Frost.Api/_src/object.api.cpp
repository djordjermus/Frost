#include <atomic>
#include "../object.api.hpp"

u64 _stdcall frost::api::object::get_reference_count(object* target)
{
	return target->_reference_count;
}
void _stdcall frost::api::object::acquire_reference(object* target)
{
	reinterpret_cast<std::atomic<u64>&>(target->_reference_count).fetch_add(1);

}
void _stdcall frost::api::object::release_reference(object* target)
{
	if (reinterpret_cast<std::atomic<u64>&>(target->_reference_count).fetch_sub(1) == 1)
		delete target;
}