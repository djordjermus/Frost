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
}
int main()
{
	frost::api::window_description desc = frost::api::window_description();
	desc.procedure = fn;
	window = frost_api_window_create(&desc);
	msg = frost_api_thread_message_create();

	while (frost_api_window_get_state(window) != frost::api::window_state::hidden)
	{
		frost_api_thread_message_wait(msg);
		frost_api_thread_message_dispatch(msg);
	}
}

void _stdcall fn(frost::api::window_event* e)
{
	if (e->type == frost::api::window_event::event_type::key_down && e->key_down.key == frost::api::keycode::escape)
		frost_api_window_set_state(e->target, frost::api::window_state::hidden);
}