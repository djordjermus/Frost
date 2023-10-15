#include "primitives.hpp"
#include "macro.hpp"
#pragma once
namespace frost::api
{
	class FROST_API color final
	{
	public:
		STATIC_CLASS(color);

		static void _stdcall rgba8_to_rgba32(const u32 input, f32* output);
		static void _stdcall rgba32_to_rgba8(const f32* input, u32* output);

		static void _stdcall rgba32_to_hsva(const f32* input, f32* output);
		static void _stdcall rgba32_to_hsla(const f32* input, f32* output);
		static void _stdcall rgba32_to_cmyk(const f32* input, f32* output);

		static void _stdcall hsva_to_rgba32(const f32* input, f32* output);
		static void _stdcall hsla_to_rgba32(const f32* input, f32* output);
		static void _stdcall cmyk_to_rgba32(const f32* input, f32* output);
	};
}