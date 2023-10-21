#include "primitives.hpp"
#include "macro.hpp"
#pragma once

extern "C"
{
	struct semantic_version final
	{
		u16 major_version;
		u16 minor_version;
		u32 patch;
		const wchar_t* decoration;
	};

	FROST_API bool _stdcall semantic_version_check_compatibility(semantic_version version, semantic_version target);
	FROST_API void _stdcall semantic_version_get_api_version(semantic_version* output);
}
