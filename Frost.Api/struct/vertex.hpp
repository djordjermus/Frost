#include "../primitives.hpp"
#pragma once
namespace frost::api
{
	class vertex final
	{
	public:
		f32 position[4];
		f32 tangent[4];
		f32 normal[3];
		f32 uv1[2];
		f32 uv2[2];
		f32 uv3[2];
		f32 uv4[2];
		u8 color[4];
	};
}