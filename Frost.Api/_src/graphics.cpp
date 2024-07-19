#include "../include.hpp"
#include "_internal/object.hpp"
const frost::api::graphics_api frost::impl::graphics_root::preset_apis = (frost::api::graphics_api)(
	(u32)(frost::api::graphics_api::d3d12) |
	(u32)(frost::api::graphics_api::d3d12));

FROST_API frost::api::graphics_api frost_api_graphics_get_preset_apis()
{
	return frost::impl::graphics_root::preset_apis;
}
FROST_API frost::api::object* frost_api_graphics_create_root(frost::api::graphics_api selected_apis)
{
    HRESULT hr = S_OK;
    UINT factory_flags = 0;
    { /* D3D12 DEBUG */
#if _DEBUG
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

	return result;
}


