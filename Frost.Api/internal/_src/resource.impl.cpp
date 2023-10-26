#include "../resource.impl.hpp"
u64 frost::impl::resource::impl_get_reference_count() const
{
	return this->_reference_count;
}
void frost::impl::resource::impl_acquire_reference() const
{
	this->_reference_count.fetch_add(1); 
}
void frost::impl::resource::impl_release_reference() const
{
	if (_reference_count.fetch_sub(1) == 1)
		delete this;
}