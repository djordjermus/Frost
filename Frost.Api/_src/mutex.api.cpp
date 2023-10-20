#include "../mutex.api.hpp"
#include "../internal/mutex.impl.hpp"
frost::api::mutex* frost::api::mutex::create(bool initial_owner)
{
	return frost::impl::mutex::create(initial_owner);
}
