#include "Frost.Api/keycode.api.hpp"
#include "Frost.Api/window.api.hpp"
#include "Frost.Api/synchronizable.api.hpp"
#include "Frost.Api/resource.api.hpp"
#include "Frost.Api/thread.api.hpp"
#include "Frost.Api/ref.hpp"
#include <iostream>
#include <thread>
#include <fstream>
#include <vector>
void test_synchronizable();

frost::api::resource* thread;
frost::api::resource* wnd;

void window_proc(frost::api::window_event* e)
{
	auto x = frost_api_thread_get_current();

	if (e->type == frost::api::window_event::event_type::key_down)
		std::wcout << L"DOWN: " << frost_api_keycode_get_name(e->key_down.key) << " | " << e->key_down.text << L" : " << e->key_down.repeat << L'\n';
	if (e->type == frost::api::window_event::event_type::key_up)
		std::wcout << L"UP:   " << frost_api_keycode_get_name(e->key_up.key) << " | " << e->key_up.text << L'\n';
}
void thread_procedure()
{
	std::this_thread::sleep_for(std::chrono::milliseconds(5000));
	frost_api_window_set_position(wnd, { 500, 500 });
	frost_api_window_set_size(wnd, { 200, 200 });
}
ref e = ::frost_api_synchronizable_create_event();
int i = 0;
int main()
{
	ref thread = frost_api_thread_create([](void* ptr)
		{
			ref msg = frost_api_thread_message_create();
			frost_api_synchronizable_signal(e);
			for (int i = 0; i < 100; i++)
			{
				frost_api_thread_message_wait(msg.get());
				frost_api_thread_message_dispatch(msg.get());
			}
		},
		nullptr);
	frost_api_synchronizable_wait(e);

	while (true)
	{
		bool success = frost_api_thread_message_send(thread, [](void* ptr) { i++; }, nullptr);
		if (!success)
			break;
	}
	std::cout << i;
}

void test_synchronizable()
{
	ref mx = frost_api_synchronizable_create_mutex(false);
	ref sf = frost_api_synchronizable_create_semaphore(2, 2);
	auto m2 = mx;
	auto m3 = mx;
	auto s2 = sf;
	bool b;
	i32 i;
	b = frost_api_synchronizable_wait(mx.get());		// TRUE
	b = frost_api_synchronizable_wait(sf.get());		// TRUE
	b = frost_api_synchronizable_wait(sf.get());		// TRUE
	b = frost_api_synchronizable_try_wait(sf.get());	// FALSE
	b = frost_api_synchronizable_signal(mx.get());	// TRUE
	b = frost_api_synchronizable_signal(mx.get());	// FALSE
	b = frost_api_synchronizable_signal(sf.get());	// TRUE
	b = frost_api_synchronizable_signal(sf.get());	// TRUE
	b = frost_api_synchronizable_signal(sf.get());	// FALSE

	std::vector<ref> vec;
	vec.push_back(sf.get());
	vec.push_back(mx.get());

	b = frost_api_synchronizable_wait_all(vec.data()->get_array(), vec.size());		// TRUE
	b = frost_api_synchronizable_wait_all(vec.data()->get_array(), vec.size());		// TRUE
	b = frost_api_synchronizable_try_wait_all(vec.data()->get_array(), vec.size());	// FALSE
	i = frost_api_synchronizable_wait_one(vec.data()->get_array(), vec.size());		// 1
	i = frost_api_synchronizable_try_wait_one(vec.data()->get_array(), vec.size());	// 1
}
