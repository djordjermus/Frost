#include "Frost.Api/clock.api.hpp"
#include "../clock.hpp"

namespace frost
{
	u64 clock::get_frequency()
	{
		return api::clock::get_frequency();
	}
	f64 clock::get_period()
	{
		return api::clock::get_period();
	}
	u64 clock::get_timestamp()
	{
		return api::clock::get_timestamp();
	}
}
