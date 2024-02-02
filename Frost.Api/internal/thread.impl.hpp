#include "synchronizable.impl.hpp"
#pragma once
#if defined(TARGET_BUILD_PLATFORM_WINDOWS)
#include <windows.h>
namespace frost::impl
{
	class thread_reference : public synchronizable
	{
		u64 _thread_id;
	protected:
		~thread_reference() = default;

	public:
		thread_reference(HANDLE thread_handle);
		bool signal() const override;

		static thread_reference* get_current();
	};

	class thread : public thread_reference
	{
	protected:
		~thread() override;

	public:
		thread(void(_stdcall* procedure)(void*), void* argument);

		u64 get_id() const;
		static u64 get_current_id();



		class message final : public frost::api::resource
		{
		public:
			static constexpr u64 msg_execute_procedure = 0xBFFF;
			static message* create();

			static bool send(
				frost::impl::thread_reference* thread,
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
			void execute_procedure() const;
			void signal() const;
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
