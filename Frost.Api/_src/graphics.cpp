#include "../include.hpp"
#include "_internal/object.hpp"
const frost::api::graphics_backend frost::impl::graphics_root::preset_apis = (frost::api::graphics_backend)(
	(u32)(frost::api::graphics_backend::d3d12) |
	(u32)(frost::api::graphics_backend::d3d12));

FROST_API frost::api::graphics_backend frost_api_graphics_get_preset_apis()
{
	return frost::impl::graphics_root::preset_apis;
}
FROST_API frost::api::object* frost_api_graphics_create_root(frost::api::graphics_backend selected_apis)
{
	HRESULT hr = S_OK;
	UINT factory_flags = 0;
#if _DEBUG
	{ /* D3D12 DEBUG */
		ID3D12Debug* debug;
		if ((hr = D3D12GetDebugInterface(IID_PPV_ARGS(&debug))) != S_OK)
		{
			debug->EnableDebugLayer();
			debug->Release();
			factory_flags |= DXGI_CREATE_FACTORY_DEBUG;
		}
	}
#endif
	auto* result = new frost::impl::graphics_root();
	result->type = frost::api::object_type::graphics_root;
	
	/* DXGI FACTORY */
	if ((hr = CreateDXGIFactory2(factory_flags, IID_PPV_ARGS(&result->factory))) != S_OK)
	{
		frost_api_object_acquire_reference(result);
		frost_api_object_release_reference(result);
		return nullptr;
	}

	/* DXGI ADAPTER */
	IDXGIFactory7* factory7 = nullptr;
	IDXGIAdapter1* adapter = nullptr;
	if ((hr = result->factory->QueryInterface(IID_PPV_ARGS(&factory7))) == S_OK)
	{
		for (UINT adapter_index = 0;
			(hr = factory7->EnumAdapterByGpuPreference(adapter_index, DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE, IID_PPV_ARGS(&adapter))) == S_OK;
			adapter_index++)
		{

			DXGI_ADAPTER_DESC1 desc;
			adapter->GetDesc1(&desc);
			if (desc.Flags & DXGI_ADAPTER_FLAG::DXGI_ADAPTER_FLAG_SOFTWARE)
			{
				adapter->Release();
				continue;
			}
			else
			{
				result->adapter = adapter;
				break;
			}
		}
	}
	factory7->Release();
	/* D3D12 DEVICE */
	if ((hr = D3D12CreateDevice(result->adapter, D3D_FEATURE_LEVEL_12_1, IID_PPV_ARGS(&result->device))) != S_OK)
	{
		frost_api_object_acquire_reference(result);
		frost_api_object_release_reference(result);
		return nullptr;
	}

	/* D3D12 COMMAND QUEUE */
	D3D12_COMMAND_QUEUE_DESC queueDesc = {};
	queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAGS::D3D12_COMMAND_QUEUE_FLAG_NONE;
	queueDesc.Type = D3D12_COMMAND_LIST_TYPE::D3D12_COMMAND_LIST_TYPE_DIRECT;

	if ((hr = result->device->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&result->queue))) != S_OK)
	{
		frost_api_object_acquire_reference(result);
		frost_api_object_release_reference(result);
		return nullptr;
	}

	/* D3D12 COMMAND ALLOCATOR */
	if ((hr = result->device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&result->allocator))) != S_OK)
	{
		frost_api_object_acquire_reference(result);
		frost_api_object_release_reference(result);
		return nullptr;
	}

	result->rtv_descriptor_handle_increment_size = result->device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

	D3D12_INPUT_ELEMENT_DESC inputElementDescs[] =
	{
		// SEMANTIC, SEMANTIC INDEX, FORMAT, INPUT SLOT, ALIGNED OFFSET, CLASSIFICATION, INSTANCE STEPRATE
		{ "POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT,		0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "NORMAL",		0, DXGI_FORMAT_R32G32B32_FLOAT,		0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "TANGENT",	0, DXGI_FORMAT_R32G32B32_FLOAT,		0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD0",	0, DXGI_FORMAT_R32G32_FLOAT,		0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD1",	0, DXGI_FORMAT_R32G32_FLOAT,		0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD2",	0, DXGI_FORMAT_R32G32_FLOAT,		0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD3",	0, DXGI_FORMAT_R32G32_FLOAT,		0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "COLOR",		0, DXGI_FORMAT_R32G32B32A32_FLOAT,	0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }
	};

	return result;
}

FROST_API frost::api::object* frost_api_graphics_command_create(frost::api::object* root)
{
	if (root->type != frost::api::object_type::graphics_root)
		return nullptr;
	auto result = new frost::impl::graphics_command();
	result->type = frost::api::object_type::graphics_command;
	result->root = static_cast<frost::impl::graphics_root*>(root);
	result->command = nullptr;

	ID3D12GraphicsCommandList* commandList = nullptr;
	result->allocator = result->root->allocator;
	result->root->device->CreateCommandList(
		0,
		D3D12_COMMAND_LIST_TYPE::D3D12_COMMAND_LIST_TYPE_DIRECT,
		result->root->allocator,
		nullptr,
		IID_PPV_ARGS(&commandList));
	result->command = commandList; // TODO: Check command list is null
	frost_api_object_acquire_reference(root);

	return result;
}

FROST_API void frost_api_graphics_command_clear_target(frost::api::object* command, frost::api::object* target, const float* rgba)
{
	if (command->type != frost::api::object_type::graphics_command)
		return;
	if (target->type != frost::api::object_type::graphics_texture)
		return;

	D3D12_RESOURCE_BARRIER barrier = {};
	barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	barrier.Flags = D3D12_RESOURCE_BARRIER_FLAGS::D3D12_RESOURCE_BARRIER_FLAG_NONE;
	barrier.Transition.pResource = static_cast<frost::impl::graphics_texture*>(target)->resource;
	barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;
	barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;
	barrier.Transition.Subresource = 0xFFFF'FFFF;

	static_cast<frost::impl::graphics_command*>(command)->command->ResourceBarrier(1, &barrier);

	D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = static_cast<frost::impl::graphics_texture*>(target)->heap->GetCPUDescriptorHandleForHeapStart();
	rtvHandle.ptr += static_cast<frost::impl::graphics_texture*>(target)->heap_offset;
	static_cast<frost::impl::graphics_command*>(command)->command->ClearRenderTargetView(rtvHandle, rgba, 0, nullptr);

	barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
	barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;
	static_cast<frost::impl::graphics_command*>(command)->command->ResourceBarrier(1, &barrier);
}

FROST_API void frost_api_graphics_command_reset(frost::api::object* command)
{
	if (command->type != frost::api::object_type::graphics_command)
		return;

	auto cmd = static_cast<frost::impl::graphics_command*>(command);
	cmd->command->Reset(cmd->root->allocator, nullptr);
}

FROST_API void frost_api_graphics_command_close(frost::api::object* command)
{
	if (command->type != frost::api::object_type::graphics_command)
		return;

	auto cmd = static_cast<frost::impl::graphics_command*>(command);
	cmd->command->Close();
}

FROST_API void frost_api_graphics_command_execute(frost::api::object* command)
{
	if (command->type != frost::api::object_type::graphics_command)
		return;

	auto cmd = static_cast<frost::impl::graphics_command*>(command);
	ID3D12CommandList* ppCommandLists[] = { cmd->command };
	cmd->root->queue->ExecuteCommandLists(_countof(ppCommandLists), ppCommandLists);
}


