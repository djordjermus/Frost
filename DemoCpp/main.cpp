#include "Frost/window.hpp"
#include "Frost/sync/semaphore.hpp"
#include <iostream>
void window_proc(frost::window::api::window_event_data* e);
void hide_window(frost::window::api::window_modification_context* ctx);
int main()
{
	frost::window::api::window_description desc = {};
	desc.state = frost::window::api::window_state::maximized;
	desc.procedure = window_proc;
	auto wnd = frost::window::api::create(&desc);

	while (frost::window::api::get_state(wnd) != frost::window::api::window_state::hidden)
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
