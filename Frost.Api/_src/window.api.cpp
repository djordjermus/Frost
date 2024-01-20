#include "../internal/window.impl.hpp"

#define TO_IMPL(x) (static_cast<frost::impl::window*>(x))

FROST_API bool _stdcall frost_api_window_is_enabled(frost::api::resource* target)
{
	return TO_IMPL(target)->is_enabled();
}
FROST_API bool _stdcall frost_api_window_is_active(frost::api::resource* target)
{
	return TO_IMPL(target)->is_active();
}
FROST_API bool _stdcall frost_api_window_is_focused(frost::api::resource* target)
{
	return TO_IMPL(target)->is_focused();
}

FROST_API frost::api::window_state _stdcall frost_api_window_get_state(frost::api::resource* target)
{
	return TO_IMPL(target)->get_state();
}

FROST_API bool _stdcall frost_api_window_get_key_state(frost::api::resource* target, frost::api::keycode key)
{
	return TO_IMPL(target)->get_key_state((u64)key);
}

FROST_API void _stdcall frost_api_window_get_position(frost::api::resource* target, point2d<i32>* output)
{
	*output = TO_IMPL(target)->get_position();
}
FROST_API void _stdcall frost_api_window_get_size(frost::api::resource* target, size2d<i32>* output)
{
	*output = TO_IMPL(target)->get_size();
}

FROST_API void _stdcall frost_api_window_set_enabled(frost::api::resource* target, bool enabled)
{
	TO_IMPL(target)->set_enabled(enabled);
}
FROST_API void _stdcall frost_api_window_set_active(frost::api::resource* target, bool active)
{
	TO_IMPL(target)->set_active(active);
}
FROST_API void _stdcall frost_api_window_set_focused(frost::api::resource* target, bool focused)
{
	TO_IMPL(target)->set_focus(focused);
}

FROST_API void _stdcall frost_api_window_set_state(frost::api::resource* target, frost::api::window_state state)
{
	TO_IMPL(target)->set_state(state);
}

FROST_API void _stdcall frost_api_window_set_position(frost::api::resource* target, point2d<i32> position)
{
	TO_IMPL(target)->set_position(position);
}
FROST_API void _stdcall frost_api_window_set_size(frost::api::resource* target, size2d<i32> size)
{
	TO_IMPL(target)->set_size(size);
}

FROST_API frost::api::window_procedure_sig _stdcall frost_api_window_get_procedure(frost::api::resource* target)
{
	return TO_IMPL(target)->get_procedure();
}
FROST_API void* _stdcall frost_api_window_get_data(frost::api::resource* target)
{
	return TO_IMPL(target)->get_data();
}

FROST_API void _stdcall frost_api_window_set_procedure(frost::api::resource* target, frost::api::window_procedure_sig procedure)
{
	TO_IMPL(target)->set_procedure(procedure);
}
FROST_API void _stdcall frost_api_window_set_data(frost::api::resource* target, void* data)
{
	TO_IMPL(target)->set_data(data);
}

FROST_API void _stdcall frost_api_window_pump_messages(frost::api::resource* target)
{
	TO_IMPL(target)->pump_messages();
}

FROST_API frost::api::resource* _stdcall frost_api_window_create(const frost::api::window_description* description)
{
	return frost::impl::window::create(description);
}
FROST_API bool _stdcall frost_api_resource_is_window(frost::api::resource* target)
{
	return dynamic_cast<frost::impl::window*>(target) != nullptr;
}