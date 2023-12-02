#include "../internal/window.impl.hpp"

#define TO_IMPL(x) (static_cast<frost::impl::window*>(x))

bool _stdcall window_is_enabled(frost::api::resource* target)
{
	return TO_IMPL(target)->is_enabled();
}
bool _stdcall window_is_active(frost::api::resource* target)
{
	return TO_IMPL(target)->is_active();
}
bool _stdcall window_is_focused(frost::api::resource* target)
{
	return TO_IMPL(target)->is_focused();
}

frost::api::window_state _stdcall window_get_state(frost::api::resource* target)
{
	return TO_IMPL(target)->get_state();
}

bool _stdcall window_get_key_state(frost::api::resource* target, frost::api::keycode key)
{
	return TO_IMPL(target)->get_key_state((u64)key);
}

void _stdcall window_get_position(frost::api::resource* target, point2d<i32>* output)
{
	*output = TO_IMPL(target)->get_position();
}
void _stdcall window_get_size(frost::api::resource* target, size2d<i32>* output)
{
	*output = TO_IMPL(target)->get_size();
}

frost::api::window_procedure_sig _stdcall window_get_procedure(frost::api::resource* target)
{
	return TO_IMPL(target)->get_procedure();
}
void* _stdcall window_get_data(frost::api::resource* target)
{
	return TO_IMPL(target)->get_data();
}



void _stdcall window_pump_messages(frost::api::resource* target)
{
	TO_IMPL(target)->pump_messages();
}

frost::api::resource* _stdcall window_create(const frost::api::window_description* description)
{
	return frost::impl::window::create(description);
}
bool _stdcall resource_is_window(frost::api::resource* target)
{
	return dynamic_cast<frost::impl::window*>(target) != nullptr;
}