#include "Frost.Api/keycode.api.hpp"
#include "Frost.Api/window.api.hpp"
#include "Frost.Api/synchronizable.api.hpp"
#include "Frost.Api/resource.api.hpp"
#include "Frost.Api/ref.hpp"
#include <iostream>
#include <thread>
#include <fstream>
#include <vector>
void test_synchronizable();

frost::api::resource* wnd;

void window_proc(frost::api::window_event* e)
{
	if (e->type == frost::api::window_event::event_type::key_down)
		std::wcout << L"DOWN: " << keycode_get_name(e->key_down.key) << " | " << e->key_down.text << L" : " << e->key_down.repeat << L'\n';
	if (e->type == frost::api::window_event::event_type::key_up)
		std::wcout << L"UP:   " << keycode_get_name(e->key_up.key) << " | " << e->key_up.text << L'\n';
}
void thread_procedure()
{
	std::this_thread::sleep_for(std::chrono::milliseconds(5000));
	window_set_position(wnd, { 500, 500 });
	window_set_size(wnd, { 200, 200 });
}
auto e = ::synchronizable_create_event();

int main()
{
	std::thread th([]() {
		std::this_thread::sleep_for(std::chrono::seconds(10));
		synchronizable_unlock(e);
		std::this_thread::sleep_for(std::chrono::seconds(10));
		synchronizable_unlock(e);
		});
	synchronizable_lock(e);
	std::cout << "1st\n";
	synchronizable_event_reset(e);
	synchronizable_lock(e);
	std::cout << "2nd\n";
	th.join();
}

void test_synchronizable()
{
	ref mx = synchronizable_create_mutex(false);
	ref sf = synchronizable_create_semaphore(2, 2);
	auto m2 = mx;
	auto m3 = mx;
	auto s2 = sf;
	bool b;
	i32 i;
	b = synchronizable_lock(mx.get());		// TRUE
	b = synchronizable_lock(sf.get());		// TRUE
	b = synchronizable_lock(sf.get());		// TRUE
	b = synchronizable_try_lock(sf.get());	// FALSE
	b = synchronizable_unlock(mx.get());	// TRUE
	b = synchronizable_unlock(mx.get());	// FALSE
	b = synchronizable_unlock(sf.get());	// TRUE
	b = synchronizable_unlock(sf.get());	// TRUE
	b = synchronizable_unlock(sf.get());	// FALSE

	std::vector<ref> vec;
	vec.push_back(sf.get());
	vec.push_back(mx.get());

	b = synchronizable_lock_all(vec.data()->get_array(), vec.size());		// TRUE
	b = synchronizable_lock_all(vec.data()->get_array(), vec.size());		// TRUE
	b = synchronizable_try_lock_all(vec.data()->get_array(), vec.size());	// FALSE
	i = synchronizable_lock_one(vec.data()->get_array(), vec.size());		// 1
	i = synchronizable_try_lock_one(vec.data()->get_array(), vec.size());	// 1
}
