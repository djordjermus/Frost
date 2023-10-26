#include "Frost.Api/mutex.api.hpp"
#include "Frost.Api/semaphore.api.hpp"
#include "Frost.Api/resource.api.hpp"
#include "Frost.Api/ref.hpp"
#include <iostream>
#include <vector>
void test_synchronizable();
int main()
{
	test_synchronizable();
}

void test_synchronizable()
{
	ref<frost::api::synchronizable> mx = frost::api::mutex::create(false);
	ref<frost::api::synchronizable> sf = frost::api::semaphore::create(2, 2);
	auto m2 = mx;
	auto m3 = mx;
	auto s2 = sf;
	bool b;
	i32 i;
	b = mx->lock();		// TRUE
	b = sf->lock();		// TRUE
	b = sf->lock();		// TRUE
	b = sf->try_lock();	// FALSE
	b = mx->unlock();	// TRUE
	b = mx->unlock();	// FALSE
	b = sf->unlock();	// TRUE
	b = sf->unlock();	// TRUE
	b = sf->unlock();	// FALSE

	std::vector<ref<frost::api::synchronizable>> vec;
	vec.push_back(sf.get());
	vec.push_back(mx.get());

	b = frost::api::synchronizable::lock_all(vec.data()->get_array(), vec.size());		// TRUE
	b = frost::api::synchronizable::lock_all(vec.data()->get_array(), vec.size());		// TRUE
	b = frost::api::synchronizable::try_lock_all(vec.data()->get_array(), vec.size());	// FALSE
	i = frost::api::synchronizable::lock_one(vec.data()->get_array(), vec.size());		// 1
	i = frost::api::synchronizable::try_lock_one(vec.data()->get_array(), vec.size());	// 1
}