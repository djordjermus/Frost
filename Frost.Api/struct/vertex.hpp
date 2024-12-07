#include "../primitives.hpp"
#pragma once
namespace frost::api
{
	class vertex final
	{
	public:
		f32 position[3];	// POSITION
		f32 normal[3];		// NORMAL
		f32 tangent[3];		// TANGENT
		f32 uv1[2];			// TEXCOORD0
		f32 uv2[2];			// TEXCOORD1
		f32 uv3[2];			// TEXCOORD2
		f32 uv4[2];			// TEXCOORD3
		f32 color[4];		// COLOR
	};
}