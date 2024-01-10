#include "../thread.impl.hpp"
#if defined(TARGET_BUILD_PLATFORM_WINDOWS)
namespace frost::impl
{
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
		synchronizable(create_thread(new thread_startup_info(procedure, argument), &_id)) {}
	bool thread::signal() const { return true; /* DO NOTHING */ }

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
		try
		{
			tsi->procedure(tsi->argument);
		}
		catch (...) { }
		try
		{
			delete tsi;
		}
		catch (...) {}
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
		::PeekMessageW(nullptr, nullptr, 0, 0, PM_NOREMOVE);
		auto ret = new message();
		return ret;
	}

	bool thread::message::send(
		u64 thread,
		impl::synchronizable* sync,
		void(_stdcall* procedure)(void* argument),
		void* argument)
	{

		auto* info = new procedure_message_info();
		info->procedure = procedure;
		info->argument = argument;

		if (sync != nullptr)
			sync->acquire_reference();

		BOOL result = ::PostThreadMessageW(
			static_cast<DWORD>(thread),
			msg_execute_procedure,
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
		try
		{
			if (_msg.message == msg_execute_procedure)
			{
				execute_procedure();
				delete_info();
				signal();
			}
			else
			{
				::TranslateMessage(&_msg);
				::DispatchMessageW(&_msg);
			}
		}
		catch (...) { /* SUPPRESS */ }
		clear_state();
	}

	void thread::message::discard()
	{
		if (_msg.message == msg_execute_procedure)
		{
			delete_info();
			signal();
		}
		clear_state();
	}

	void thread::message::clear_state()
	{
		_msg = {};
	}

	void thread::message::delete_info()
	{
		if (_msg.wParam == 0)
			return;

		try
		{
			auto* info = reinterpret_cast<procedure_message_info*>(_msg.wParam);
			delete info;
		}
		catch (...) { /* SUPPRESS */ }
	}

	void thread::message::execute_procedure()
	{
		if (_msg.wParam == 0)
			return;

		try
		{
			auto* info = reinterpret_cast<procedure_message_info*>(_msg.wParam);
			info->procedure(info->argument);
		}
		catch (...) { /* SUPPRESS */ }
	}
	void thread::message::signal()
	{
		if (_msg.lParam == 0)
			return;

		auto sync = reinterpret_cast<frost::impl::synchronizable*>(_msg.lParam);
		try
		{
			sync->signal();
		}
		catch (...) { /* SUPPRESS */ }
		try
		{
			sync->release_reference();
		}
		catch (...) { /* SUPPRESS */ }
	}
}
#else
static_assert("PLATFORM NOT SUPPORTED!")
#endif
