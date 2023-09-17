#include "macro.hpp"
#include "primitives.hpp"
#pragma once
extern "C"
{
	struct frost_semantic_version;
	FROST_API frost_semantic_version _stdcall version_get_api_version();
	FROST_API bool _stdcall version_check_compatibility(frost_semantic_version version, frost_semantic_version target);

	struct frost_semantic_version final
	{
	public:
		u16 major;
		u16 minor;
		u32 patch;
		const wchar_t* decoration;
	};
}