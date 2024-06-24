#include "Frost.Api/include.hpp"
#include "Frost.Api/ref.hpp"
#include <iostream>
#include <thread>
#include <fstream>
#include <vector>

void _stdcall fn(frost::api::window_event* target);
ref window;
ref msg;
static void _stdcall log_handler(void* p_data)
{
	frost::api::log_event_data* e = reinterpret_cast<frost::api::log_event_data*>(p_data);
	std::wcout << e->message << L"\n";
}
int main()
{
	frost::api::log_parameter parameters[] =
	{
		{ L"Greeting", L"Hello", 8, 5 },
		{ L"Subject", L"World", 7, 5 }
	};
	frost_api_event_system_subscribe(frost_api_logging_get_log_event_tag(), 1, log_handler);
	while (true)
	frost_api_logging_log(1, L"{@Greeting}, {@Subject}", 24, parameters, 2, frost::api::log_level::fatal);


	frost::api::window_description desc = frost::api::window_description();
	desc.procedure = fn;
	window = frost_api_window_create(&desc);
	msg = frost_api_thread_message_create();

	while (frost_api_window_get_state(window) != frost::api::window_state::hidden)
	{
		frost_api_synchronizable_wait(msg);
		frost_api_thread_message_dispatch(msg);
	}
}

void _stdcall fn(frost::api::window_event* e)
{
	if (e->type == frost::api::window_event::event_type::key_down && e->key_down.key == frost::api::keycode::escape)
		frost_api_window_set_state(e->target, frost::api::window_state::hidden);
	frost_api_logging_log(0x00, L"Hello, World!", 13, nullptr, 0,frost::api::log_level::error);
}