#include "Frost/sync/mutex.hpp"
#include <iostream>
int main()
{
	frost::sync::mutex mx(false);
	frost::sync::mutex mx2(false);

	std::vector<frost::sync::sync_object> s;
	s.emplace_back(mx.get_sync_object());
	s.emplace_back(mx2.get_sync_object());
	auto result = frost::sync::sync_object::acquire_all(s);
}
