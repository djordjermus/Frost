#include <math.h>
#include "Frost.Api/color.api.hpp"
#include "../color.hpp"
namespace frost
{
#define DEG60 (1.0f / 6.0f)
#pragma warning(push)
#pragma warning (disable: 26495)
	rgba8::rgba8(u8 r, u8 g, u8 b, u8 a)
	{
		this->r = r;
		this->g = g;
		this->b = b;
		this->a = a;
	}
	rgba8::rgba8(u32 value)
	{
		this->value = value;
	}
	rgba8::rgba8(rgba32 rgba)
	{
		api::color::rgba32_to_rgba8(&rgba.r, &this->value);
	}
	rgba8::rgba8(hsva hsva)
	{
		float output[4];
		api::color::hsva_to_rgba32(&hsva.h, output);
		api::color::rgba32_to_rgba8(output, &value);
	}
	rgba8::rgba8(hsla hsla)
	{
		float output[4];
		api::color::hsla_to_rgba32(&hsla.h, output);
		api::color::rgba32_to_rgba8(output, &value);
	}			
	rgba8::rgba8(cmyk cmyk)
	{
		float output[4];
		api::color::cmyk_to_rgba32(&cmyk.c, output);
		api::color::rgba32_to_rgba8(output, &value);
	}



	rgba32::rgba32(u32 value)
	{
		api::color::rgba8_to_rgba32(value, &this->r);
	}
	rgba32::rgba32(f32 r, f32 g, f32 b, f32 a)
	{
		this->r = r;
		this->g = g;
		this->b = b;
		this->a = a;
	}
	rgba32::rgba32(rgba8 rgba)
	{
		api::color::rgba8_to_rgba32(rgba.value, &this->r);
	}
	rgba32::rgba32(hsva hsva)
	{
		api::color::hsva_to_rgba32(&hsva.h, &this->r);
	}
	rgba32::rgba32(hsla hsla)
	{
		api::color::hsla_to_rgba32(&hsla.h, &this->r);
	}
	rgba32::rgba32(cmyk cmyk)
	{
		api::color::cmyk_to_rgba32(&cmyk.c, &this->r);
	}
#pragma warning(pop)
}