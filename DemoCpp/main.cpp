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

void thread_proc(void*);
ref mainThread;
ref e;
ref th;

int main()
{
	// frost_api_event_system_subscribe(frost_api_logging_get_log_event_tag(), ~0ull, [](void* arg)
	// {
	// 	log_event* e = (log_event*)arg;
	// 	std::wcout << e->message << '\n';
	// });
	// const wchar_t* params[] = { L"World", L"Hello"};
	// const u64 lengths[] = { 5, 5 };
	// 
	// while (true)
	// 	frost_api_logging_info(L"{1}, {0}!", wcslen(L"{1}, {0}!"), params, lengths, 2, 1);

	mainThread = frost_api_thread_get_current();
	th = frost_api_thread_create(thread_proc, nullptr);
	ref message = frost_api_thread_message_create();
	while (true)
	{
		frost_api_thread_message_wait(message);
		frost_api_thread_message_dispatch(message);
	}
}

void thread_proc(void*)
{
	while (true)
		frost_api_thread_message_send(
			mainThread.get(), [](void*)
			{ },
			nullptr);
}
