#include "Frost.Api/sync_object.api.hpp"
#include "Frost.Api/object.api.hpp"
#include "Frost.Api/ref.hpp"
#include <iostream>
#include <vector>
void test_sync_object();
int main()
{
}

void test_sync_object()
{
	

	ref<frost::api::sync_object> mx = frost::api::sync_object::create_mutex(false);
	ref<frost::api::sync_object> sf = frost::api::sync_object::create_semaphore(2, 2);
	sf = frost::api::sync_object::create_mutex(false);
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

	std::vector<ref<frost::api::sync_object>> vec;
	vec.push_back(sf.get());
	vec.push_back(mx.get());

	b = frost::api::sync_object::lock_all(vec.data()->get_array(), vec.size());		// TRUE
	b = frost::api::sync_object::lock_all(vec.data()->get_array(), vec.size());		// TRUaE
	b = frost::api::sync_object::try_lock_all(vec.data()->get_array(), vec.size());	// FALSE
	i = frost::api::sync_object::lock_one(vec.data()->get_array(), vec.size());		// 1
	i = frost::api::sync_object::try_lock_one(vec.data()->get_array(), vec.size());	// 1
}