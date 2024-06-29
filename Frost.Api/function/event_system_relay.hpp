#include "../primitives.hpp"
#pragma once
namespace frost::api
{
	typedef void(_stdcall* event_system_relay)(u64 tag, u64 layer, void* p_data);
}