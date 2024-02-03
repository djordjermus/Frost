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

ref wnd;
ref msg;
ref thread;
void window_proc(frost::api::window_event* e)
{
	auto x = frost_api_thread_get_current();

	if (e->type == frost::api::window_event::event_type::key_down)
		std::wcout << L"DOWN: " << frost_api_keycode_get_name(e->key_down.key) << " | " << e->key_down.text << L" : " << e->key_down.repeat << L'\n';
	if (e->type == frost::api::window_event::event_type::key_up)
		std::wcout << L"UP:   " << frost_api_keycode_get_name(e->key_up.key) << " | " << e->key_up.text << L'\n';
}
void thread_procedure(void* unused)
{
	std::this_thread::sleep_for(std::chrono::milliseconds(5000));
	frost_api_window_set_size(wnd, { 100, 100 });
}

int main()
{
	frost::api::window_description desc = {};
	desc.procedure = window_proc;

	wnd = frost_api_window_create(&desc);
	msg = frost_api_thread_message_create();
	thread = frost_api_thread_create(thread_procedure, nullptr);
	while (frost_api_window_get_state(wnd) != frost::api::window_state::hidden)
	{
		frost_api_thread_message_wait(msg);
		frost_api_thread_message_dispatch(msg);
	}
	frost_api_synchronizable_wait(thread);
}
