#include "../../ref.hpp"
#include "../thread.impl.hpp"
#if defined(TARGET_BUILD_PLATFORM_WINDOWS)
namespace frost::impl
{
	thread_reference::thread_reference(HANDLE thread_handle) : 
		synchronizable(thread_handle),
		_thread_id(::GetThreadId(thread_handle)) {}

	bool thread_reference::signal() const { return true; /* DO NOTHING */ }

	thread_reference* thread_reference::get_current()
	{
		thread_local ref thread_ref = new thread_reference(::OpenThread(THREAD_ALL_ACCESS, FALSE, ::GetCurrentThreadId()));
		return static_cast<thread_reference*>(thread_ref.get());
	}



	struct thread::thread_startup_info
	{
		void(_stdcall*procedure)(void* argument);
		void* argument;
		inline thread_startup_info(void(_stdcall* procedure)(void* argument), void* argument)
		{
			this->procedure = procedure;
			this->argument = argument;
		}
	};
	thread::~thread()
	{
		wait();
	}
	thread::thread(void(_stdcall* procedure)(void*), void* argument) :
		thread_reference(create_thread(new thread_startup_info(procedure, argument), &_id)) {}

	u64 thread::get_id() const { return _id; }
	u64 thread::get_current_id() { return ::GetCurrentThreadId(); }



	HANDLE thread::create_thread(thread_startup_info* info, DWORD* out_id)
	{
		HANDLE ret = ::CreateThread(nullptr, 0, thread_procedure, info, 0, out_id);
		return ret;
	}
	u32 _stdcall thread::thread_procedure(void* info)
	{
		thread_startup_info* tsi = reinterpret_cast<thread_startup_info*>(info);
		tsi->procedure(tsi->argument);
		delete tsi;
		
		return 0;
	}

	/*
	 * MESSAGE
	 */

	struct procedure_message_info
	{
		void(_stdcall* procedure)(void* argument);
		void* argument;
	};
	thread::message* thread::message::create()
	{
		MSG temp = {};
		::PeekMessageW(&temp, nullptr, 0, 0, PM_NOREMOVE);
		auto ret = new message();
		return ret;
	}

	bool thread::message::send(
		frost::impl::thread_reference* thread,
		void(_stdcall* procedure)(void* argument),
		void* argument)
	{
		auto handle = thread->get_system_handle();
		if (handle == nullptr)
			return false;

		auto id = ::GetThreadId(handle);
		if (id == 0)
			return false;

		auto info = procedure_message_info();
		info.procedure = procedure;
		info.argument = argument;

		auto sync = new frost::impl::synchronizable_event();
		sync->acquire_reference();

		BOOL result = ::PostThreadMessageW(
			id,
			msg_execute_procedure_asynchronous,
			reinterpret_cast<WPARAM>(&info),
			reinterpret_cast<LPARAM>(sync));
		if (!!result)
		{
			frost::impl::synchronizable* syncs[] = { sync, thread };
			auto ix = frost::impl::synchronizable::wait_one(syncs, 2);
			if (ix != 0)
			{
				sync->release_reference();
				return false;
			}
			return true;
		}
		else
		{
			sync->release_reference();
			return false;
		}
	}

	bool thread::message::send_async(
		frost::impl::thread_reference* thread,
		impl::synchronizable* sync,
		void(_stdcall* procedure)(void* argument),
		void* argument)
	{
		auto* info = new procedure_message_info();
		info->procedure = procedure;
		info->argument = argument;

		auto handle = thread->get_system_handle();
		if (handle == nullptr)
			return false;

		auto id = ::GetThreadId(handle);
		if (id == 0)
			return false;

		BOOL result = ::PostThreadMessageW(
			id,
			msg_execute_procedure_asynchronous,
			reinterpret_cast<WPARAM>(info),
			reinterpret_cast<LPARAM>(sync));

		return !!result;
	}

	void thread::message::wait()
	{
		::GetMessageW(&_msg, nullptr, 0, 0);
	}
	void thread::message::peek()
	{
		::PeekMessageW(&_msg, nullptr, 0, 0, PM_REMOVE);
	}



	void thread::message::dispatch()
	{
		if (_msg.message == msg_execute_procedure_asynchronous)
		{
			if (_msg.wParam != 0)
			{
				try
				{
					auto* info = reinterpret_cast<procedure_message_info*>(_msg.wParam);
					if (info->procedure)
						info->procedure(info->argument);
				}
				catch (...) { /* ... */ }
			}

			if (_msg.lParam != 0)
			{
				auto sync = reinterpret_cast<frost::impl::synchronizable*>(_msg.lParam);
				try
				{
					sync->signal();
				}
				catch (...) { /* ... */ }
				try
				{
					sync->release_reference();
				}
				catch (...) { /* ... */ }
			}
		}
		else
		{
			::TranslateMessage(&_msg);
			::DispatchMessageW(&_msg);
		}
		_msg = {};
	}

	void thread::message::discard()
	{
		if (_msg.message == msg_execute_procedure_asynchronous)
		{
			if (_msg.wParam != 0)
			{
				auto* info = reinterpret_cast<procedure_message_info*>(_msg.wParam);
				delete info;
			}
			if (_msg.lParam != 0)
			{
				reinterpret_cast<frost::impl::synchronizable*>(_msg.lParam)->signal();
			}
		}
		_msg = {};
	}
}
#else
static_assert("PLATFORM NOT SUPPORTED!")
#endif
