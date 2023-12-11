#include "Frost.Api/keycode.api.hpp"
#include "Frost.Api/window.api.hpp"
#include "Frost.Api/synchronizable.api.hpp"
#include "Frost.Api/resource.api.hpp"
#include "Frost.Api/ref.hpp"
#include <iostream>
#include <fstream>
#include <vector>
void test_synchronizable();
const wchar_t* kc_to_wstr(frost::api::keycode keycode);
void window_proc(frost::api::window_event* e)
{
	if (e->type == frost::api::window_event::event_type::key_down)
		std::wcout << L"DOWN: " << e->key_down.text << L" : " << e->key_down.repeat << L'\n';
	if (e->type == frost::api::window_event::event_type::key_up)
		std::wcout << L"UP:   " << e->key_down.text << L'\n';
}
int main()
{
	wchar_t text[16] = {};
	keycode_to_wcs(frost::api::keycode::bracket_open, text, 16, true);
	keycode_to_wcs(frost::api::keycode::bracket_close, text, 16, true);

	frost::api::window_description desc;
	auto ref = window_proc;
	desc.procedure = window_proc;
	auto wnd = window_create(&desc);
	resource_acquire_reference(wnd);
	window_pump_messages(wnd);
	resource_release_reference(wnd);
}

void test_synchronizable()
{
	ref mx = synchronizable_create_mutex(false);
	ref sf = synchronizable_create_semaphore(2, 2);
	auto m2 = mx;
	auto m3 = mx;
	auto s2 = sf;
	bool b;
	i32 i;
	b = synchronizable_lock(mx.get());		// TRUE
	b = synchronizable_lock(sf.get());		// TRUE
	b = synchronizable_lock(sf.get());		// TRUE
	b = synchronizable_try_lock(sf.get());	// FALSE
	b = synchronizable_unlock(mx.get());	// TRUE
	b = synchronizable_unlock(mx.get());	// FALSE
	b = synchronizable_unlock(sf.get());	// TRUE
	b = synchronizable_unlock(sf.get());	// TRUE
	b = synchronizable_unlock(sf.get());	// FALSE

	std::vector<ref> vec;
	vec.push_back(sf.get());
	vec.push_back(mx.get());

	b = synchronizable_lock_all(vec.data()->get_array(), vec.size());		// TRUE
	b = synchronizable_lock_all(vec.data()->get_array(), vec.size());		// TRUE
	b = synchronizable_try_lock_all(vec.data()->get_array(), vec.size());	// FALSE
	i = synchronizable_lock_one(vec.data()->get_array(), vec.size());		// 1
	i = synchronizable_try_lock_one(vec.data()->get_array(), vec.size());	// 1
}
