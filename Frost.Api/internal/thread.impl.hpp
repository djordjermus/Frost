#include "synchronizable.impl.hpp"
#pragma once
#if defined(TARGET_BUILD_PLATFORM_WINDOWS)
#include <windows.h>
namespace frost::impl
{
	class thread final : public frost::impl::synchronizable
	{
		~thread() override;
	public:
		thread(void(_stdcall* procedure)(void*), void* argument);
		bool signal() const override;

		u64 get_id() const;
		static u64 get_current_id();
		class message final : public frost::api::resource
		{
		public:
			static constexpr u64 msg_execute_procedure = 0xBFFF;
			static message* create();

			static bool send(
				u64 thread,
				impl::synchronizable* sync,
				void (_stdcall*procedure)(void* argument),
				void* argument);

			void wait();
			void peek();
			void dispatch();
			void discard();

		private:
			void clear_state();
			void delete_info();
			void execute_procedure();
			void signal();
			MSG _msg = {};
		};

	private:
		struct thread_startup_info;
		static HANDLE create_thread(thread_startup_info* info, DWORD* out_id);
		static u32 _stdcall thread_procedure(void* info);
		DWORD _id;
	};
}
#else
static_assert("PLATFORM NOT SUPPORTED!")
#endif
