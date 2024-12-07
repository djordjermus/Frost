#include "../primitives.hpp"
#pragma once
namespace frost::api
{
	enum class graphics_backend : u32
	{
		none		= 0x0000'0000,
		d3d12		= 0x0000'0001,

		/*
		d3d11		= 0x0000'0002,
		d3d10		= 0x0000'0004,
		d3d9		= 0x0000'0008,

		vulkan		= 0x0001'0000,
		opengl21	= 0x0002'0000,
		opengl31	= 0x0004'0000,
		opengles2	= 0x0008'0000,
		opengles31	= 0x0010'0000,
		webgl10		= 0x0020'0000,
		webgl20		= 0x0040'0000,

		metal		= 0x8000'0000,
		*/
	};
}