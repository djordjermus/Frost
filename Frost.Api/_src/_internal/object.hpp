#include "../../include.hpp"
#include "../../ref.hpp"
#include "platform.hpp"
#include "../../enum/object_type.hpp"
#include <atomic>
#pragma once
namespace frost::api
{
	struct object
	{
	public:
		inline object(api::object_type type) :
			reference_count(), type(type) {}

		std::atomic<u64> reference_count;
		api::object_type type;
	};
}

/* OBJECTS WITH PLATFORM AGNOSTIC IMPLEMENTATION */
namespace frost::impl {
	struct blob final : public api::object
	{
	public:
		inline blob(void* data, u64 byte_size) :
			object(api::object_type::data_blob), data(data), byte_size(byte_size) {}

		void* data;
		u64 byte_size;
	};



	struct blob_inlined final : public api::object
	{
	public:
		inline blob_inlined(u64 byte_size) :
			object(api::object_type::data_blob_inlined), byte_size(byte_size) {}

		u64 byte_size;
	};
}



/* OBJECTS WITH PLATFORM SPECIFIC IMPLEMENTATION */
#if defined(TARGET_BUILD_PLATFORM_WINDOWS)
namespace frost::impl {
	struct system_handle_host : public api::object
	{
	public:
		inline system_handle_host(api::object_type type, HANDLE handle) :
			object(type), handle(handle) {}
		HANDLE handle;
	};



	struct sync_mutex final : public system_handle_host
	{
	public:
		inline sync_mutex(HANDLE handle) :
			system_handle_host(api::object_type::sync_mutex, handle) {}
	};



	struct sync_semaphore final : public system_handle_host
	{
	public:
		inline sync_semaphore(HANDLE handle) :
			system_handle_host(api::object_type::sync_semaphore, handle) {}
	};



	struct sync_event final : public system_handle_host
	{
	public:
		inline sync_event(HANDLE handle) :
			system_handle_host(api::object_type::sync_event, handle) {}
	};



	struct thread_reference : public system_handle_host
	{
	public:
		inline thread_reference(HANDLE handle) :
			system_handle_host(api::object_type::thread_reference, handle) {}
		inline thread_reference(api::object_type type, HANDLE handle) :
			system_handle_host(type, handle) {}

		u64 thread_id;
	};



	struct thread final : public thread_reference
	{
	public:
		inline thread(HANDLE handle) :
			thread_reference(api::object_type::thread, handle) {}
	};



	struct thread_message final : public api::object
	{
	public:
		inline thread_message() :
			object(api::object_type::thread_message), message() {}

		MSG message;
	};



	struct graphics_root;
	struct graphics_texture;

	struct window : public system_handle_host
	{
	public:
		inline window() :
			system_handle_host(api::object_type::window, nullptr) {}

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
			IDXGISwapChain3* swapchain;
			ID3D12DescriptorHeap* descriptor_heap;
			graphics_texture* back_buffers[16];

			frost::api::size2d<u32> frame_size;
			u32 buffer_count;
			u32 buffer_index;
			u32 descriptor_heap_handle_size;
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
		inline graphics_root() :
			object(api::object_type::graphics_root) {}

		static const frost::api::graphics_backend preset_apis;

		frost::api::graphics_backend target;

		IDXGIFactory1* factory;
		IDXGIAdapter1* adapter;

		ID3D12Device* device;
		ID3D12CommandQueue* queue;
		ID3D12CommandAllocator* allocator;

		u64 rtv_descriptor_handle_increment_size;
	};



	struct graphics_command : public api::object
	{
	public:
		inline graphics_command() :
			object(api::object_type::graphics_command) {}

		graphics_root* root;
		ID3D12CommandAllocator* allocator;
		ID3D12GraphicsCommandList* command;
	};



	struct graphics_texture : public api::object
	{
	public:
		inline graphics_texture() :
			object(api::object_type::graphics_texture) {}

		graphics_root* root;
		ID3D12Resource* resource;
		ID3D12DescriptorHeap* heap;
		u64 heap_offset;
	};
}
#else
#endif
