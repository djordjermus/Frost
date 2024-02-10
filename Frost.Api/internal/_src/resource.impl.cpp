#include <typeinfo>
#include <string>
#include <exception>
#include <stdio.h>
#include "../resource.impl.hpp"
#include "../debug.impl.hpp"
namespace frost::api
{
	resource::~resource()
	{
		frost::impl::debug::log_resource_destruction(this, typeid(*this));
	}

	resource::resource() : _reference_count()
	{
		frost::impl::debug::log_resource_creation(this);
	}

	u64 resource::get_reference_count() const
	{
		return this->_reference_count;
	}
	void resource::acquire_reference() const
	{
		this->_reference_count.fetch_add(1); 
	}
	void resource::release_reference() const
	{
		if (_reference_count.fetch_sub(1) == 1)
			delete this;
	}
}
