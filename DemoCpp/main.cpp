#include "Frost/sync/mutex.hpp"
#include "Frost/sync/semaphore.hpp"
#include <iostream>

int main()
{
	frost::sync::semaphore mx1(2, 4);
	frost::sync::mutex mx2(false);

	std::vector<frost::sync::sync_object> s;
	s.emplace_back(mx1.get_sync_object());
	s.emplace_back(mx2.get_sync_object());
	auto result = frost::sync::sync_object::try_acquire_all(s);
	result = frost::sync::sync_object::acquire_all(s);
	result = frost::sync::sync_object::try_acquire_all(s);
	int i = frost::sync::sync_object::try_acquire_one(s);
	i = frost::sync::sync_object::acquire_one(s);
}
