#include "../clock.hpp"
#include "Frost.Api/clock.api.hpp"
namespace frost
{
	u64 clock::frequency()
	{
		return clock_get_frequency();
	}
	f64 clock::period()
	{
		return clock_get_period();
	}
	u64 clock::timestamp()
	{
		return clock_get_timestamp();
	}
}