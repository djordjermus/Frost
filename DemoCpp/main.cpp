#include "Frost/window.hpp"
#include "Frost/sync/semaphore.hpp"
#include "Frost/sync/sync_object.hpp"
#include <iostream>
void window_proc(frost::window::api::window_event_data* e);
void hide_window(frost::window::api::modification_context* ctx);
void toggle_max_window(frost::window::api::modification_context* ctx);
int main()
{
	frost::window::api::window_description desc = {};
	
	desc.procedure = window_proc;
	auto wnd = frost::window::api::create(&desc);

	while (frost::window::api::get_state(wnd) != frost::window::api::state_hidden)
		frost::window::api::message_pump();
	frost::window::api::destroy(wnd);
}
void window_proc(frost::window::api::window_event_data* e)
{
	if (e->type == frost::window::api::event_type_close)
		frost::window::api::modify(e->target, hide_window);
	else if (e->type == frost::window::api::event_type_double_click)
		frost::window::api::modify(e->target, toggle_max_window);
}
void hide_window(frost::window::api::modification_context* ctx)
{
	frost::window::api::set_state(ctx, frost::window::api::state_hidden);
}
void toggle_max_window(frost::window::api::modification_context* ctx)
{
	if (frost::window::api::get_state(frost::window::api::get_modification_target(ctx)) == frost::window::api::state_maximized)
		frost::window::api::set_state(ctx, frost::window::api::state_normal);
	else
		frost::window::api::set_state(ctx, frost::window::api::state_maximized);
}
