#include "../../include.hpp"
#include "../../ref.hpp"
#include <atomic>
#pragma once
namespace frost::api
{
	struct object
	{
	public:
		std::atomic<u64> reference_count;
		object_type type;
	};
}

/* OBJECTS WITH PLATFORM AGNOSTIC IMPLEMENTATION */
namespace frost::impl {
	struct blob final : public api::object
	{
	public:
		void* data;
		u64 byte_size;
	};

	struct blob_inlined final : public api::object
	{
	public:
		u64 byte_size;
	};
}

/* OBJECTS WITH PLATFORM SPECIFIC IMPLEMENTATION */
#if defined(TARGET_BUILD_PLATFORM_WINDOWS)
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include "windows.h"
#include <d3d12.h>
#include <dxgi1_6.h>
#pragma comment(lib, "d3d12")
#pragma comment(lib, "dxgi")
namespace frost::impl
{
	struct system_handle_host : public api::object
	{
	public:
		HANDLE handle;
	};

	struct sync_mutex final : public system_handle_host {};
	struct sync_semaphore final : public system_handle_host {};
	struct sync_event final : public system_handle_host {};

	struct thread_reference : public system_handle_host
	{
	public:
		u64 thread_id;
	};

	struct thread final : public thread_reference {};

	struct thread_message final : public api::object
	{
	public:
		MSG message;
	};

	struct graphics_root;
	struct window : public system_handle_host
	{
	public:
		static WORD _window_class;

		u64 _keystates[4] = {};
		RECT _rect = {};
		TRACKMOUSEEVENT _track_mouse_event = {};
		frost::api::point2d<i32> _last_cursor_position = {};
		HKL _hkl = nullptr;
		u64 _flags = 0;

		frost::api::window_procedure_sig _procedure = nullptr;
		void* _data = nullptr;

		frost::impl::thread_reference* _thread = nullptr;
		DWORD _thread_id = 0;

		struct
		{
			graphics_root* root;
			frost::api::size2d<u32> buffer_size;
			u32 buffer_count;
			IDXGISwapChain1* swapchain;
		} graphics;

		frost::api::window_state _state = frost::api::window_state::invalid;

		constexpr static u64 _flag_enabled = (1ull << 0);
		constexpr static u64 _flag_active = (1ull << 1);
		constexpr static u64 _flag_focused = (1ull << 2);
		constexpr static u64 _flag_cursor_inside = (1ull << 3);
	};

	struct graphics_root : public api::object
	{
	public:
		static const frost::api::graphics_api preset_apis;

		frost::api::graphics_api target;

		IDXGIFactory1* factory;
		IDXGIAdapter1* adapter;

		ID3D12Device* device;
		ID3D12CommandQueue* queue;
		ID3D12CommandAllocator* allocator;
	};
}
#else
static_assert("PLATFORM NOT SUPPORTED!" == nullptr);
#endif
