#include "primitives.hpp"
#include "macro.hpp"
#pragma once
namespace frost::api
{
	class FROST_API semantic_version final
	{
	public:
		u16 major_version;
		u16 minor_version;
		u32 patch;
		const wchar_t* decoration;

		static bool _stdcall check_compatibility(semantic_version version, semantic_version target);
		static void _stdcall get_api_version(semantic_version* output);
	};
}