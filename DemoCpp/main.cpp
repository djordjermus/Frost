#include "Frost.Api/keycode.api.hpp"
#include "Frost.Api/window.api.hpp"
#include "Frost.Api/synchronizable.api.hpp"
#include "Frost.Api/resource.api.hpp"
#include "Frost.Api/thread.api.hpp"
#include "Frost.Api/ref.hpp"
#include "Frost.Api/event_system.api.hpp"
#include "Frost.Api/logging.api.hpp"
#include <iostream>
#include <thread>
#include <fstream>
#include <vector>

void _stdcall fn(frost::api::window_event* target);
ref window;
ref msg;

int main()
{
	frost::api::window_description desc = {};
	desc.procedure = fn;
	window = frost_api_window_create(&desc);
	msg = frost_api_thread_message_create();
	while (true)
	{
		frost_api_thread_message_wait(msg);
		frost_api_thread_message_dispatch(msg);
	}

}

void _stdcall fn(frost::api::window_event* e)
{
	if (e->type == frost::api::window_event::event_type::key_down)
		int i = 0;
}