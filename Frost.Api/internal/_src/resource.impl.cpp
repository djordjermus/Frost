#include "../resource.impl.hpp"
#include "../../event_system.api.hpp"
#include "../../logging.api.hpp"
u64 frost::api::resource::get_reference_count() const
{
	return this->_reference_count;
}
void frost::api::resource::acquire_reference() const
{
	this->_reference_count.fetch_add(1); 
}
void frost::api	::resource::release_reference() const
{
	if (_reference_count.fetch_sub(1) == 1)
	{
		delete this;
	}
}
