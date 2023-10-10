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
		
		rgba8(u8 r, u8 g, u8 b, u8 a);
		rgba8(u32 value);
		rgba8(rgba32 rgba);
		rgba8(hsva hsva);
		rgba8(hsla hsla);
		rgba8(cmyk cmyk);
	};

	class rgba32 final
	{
	public:
		float r, g, b, a;

		rgba32(u32 value);
		rgba32(f32 r, f32 g, f32 b, f32 a);
		rgba32(rgba8 rgba);
		rgba32(hsva hsva);
		rgba32(hsla hsla);
		rgba32(cmyk cmyk);
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