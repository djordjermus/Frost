#include "../semaphore.api.hpp"
#include "../internal/semaphore.impl.hpp"
frost::api::semaphore* frost::api::semaphore::create(i32 count, i32 max)
{
	return frost::impl::semaphore::create(count, max);
}
