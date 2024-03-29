#include "primitives.hpp"
#include "macro.hpp"
#pragma once
extern "C"
{
	FROST_API void _stdcall frost_api_color_rgba8_to_rgba32(const u32 input, f32* output);
	FROST_API void _stdcall frost_api_color_rgba32_to_rgba8(const f32* input, u32* output);

	FROST_API void _stdcall frost_api_color_rgba32_to_hsva(const f32* input, f32* output);
	FROST_API void _stdcall frost_api_color_rgba32_to_hsla(const f32* input, f32* output);
	FROST_API void _stdcall frost_api_color_rgba32_to_cmyk(const f32* input, f32* output);

	FROST_API void _stdcall frost_api_color_hsva_to_rgba32(const f32* input, f32* output);
	FROST_API void _stdcall frost_api_color_hsla_to_rgba32(const f32* input, f32* output);
	FROST_API void _stdcall frost_api_color_cmyk_to_rgba32(const f32* input, f32* output);
}
