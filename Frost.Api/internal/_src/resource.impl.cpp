#include <typeinfo>
#include <string>
#include <exception>
#include <stdio.h>
#include "../resource.impl.hpp"
#include "../debug.impl.hpp"
frost::api::resource::resource() : _reference_count()
{
	frost::impl::debug::log_resource_creation(this);
}
frost::api::resource::~resource()
{
	frost::impl::debug::log_resource_destruction(this, typeid(*this));
}

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
		delete this;
}
