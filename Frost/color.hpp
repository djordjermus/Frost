#include "macro.hpp"
#include "primitives.hpp"
#pragma once
namespace frost
{
	class hsva;
	class hsla;
	class cmyk;
	class rgba8;
	class rgba32;

	class rgba8 final
	{
	public:
		union
		{
			u32 value;
			struct { u8 r, g, b, a; };
		};
		
		FROST_API rgba8(u8 r, u8 g, u8 b, u8 a);
		FROST_API rgba8(u32 value);
		FROST_API rgba8(rgba32 rgba);
		FROST_API rgba8(hsva hsva);
		FROST_API rgba8(hsla hsla);
		FROST_API rgba8(cmyk cmyk);
		static void FROST_API color_rgba8_to_rgba32(const u32 input, f32* output);
	};

	class rgba32 final
	{
	public:
		float r, g, b, a;

		FROST_API rgba32(u32 value);
		FROST_API rgba32(f32 r, f32 g, f32 b, f32 a);
		FROST_API rgba32(rgba8 rgba);
		FROST_API rgba32(hsva hsva);
		FROST_API rgba32(hsla hsla);
		FROST_API rgba32(cmyk cmyk);

		static void FROST_API color_rgba32_to_rgba8(const f32* input, u32* output);

		static void FROST_API color_rgba32_to_hsva(const f32* input, f32* output);
		static void FROST_API color_rgba32_to_hsla(const f32* input, f32* output);
		static void FROST_API color_rgba32_to_cmyk(const f32* input, f32* output);

		static void FROST_API color_hsva_to_rgba32(const f32* input, f32* output);
		static void FROST_API color_hsla_to_rgba32(const f32* input, f32* output);
		static void FROST_API color_cmyk_to_rgba32(const f32* input, f32* output);
	};



	class hsva final
	{
	public:
		float h;
		float s;
		float v;
		float a;
	};
	class hsla final
	{
	public:
		float h;
		float s;
		float l;
		float a;
	};
	class cmyk final
	{
	public:
		float c;
		float m;
		float y;
		float k;
	};
}