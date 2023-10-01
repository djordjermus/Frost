#include "Frost/window.hpp"
#include "Frost/sync/semaphore.hpp"
#include "Frost/sync/sync_object.hpp"
#include <iostream>
void window_proc(frost::window::api::window_event_data* e);
void hide_window(frost::window::api::window_modification_context* ctx);
int main()
{
	frost::window::api::window_description desc = {};
	desc.procedure = window_proc;
	auto wnd = frost::window::api::create(&desc);

	while (true)
		frost::window::api::message_pump();
}
void window_proc(frost::window::api::window_event_data* e)
{
	if (e->type == frost::window::api::window_event_data::type_close)
		frost::window::api::modify(e->target, hide_window, nullptr);
		
}
void hide_window(frost::window::api::window_modification_context* ctx)
{
	frost::window::api::set_state(ctx, frost::window::api::window_state::hidden);
}
