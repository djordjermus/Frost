#include "macro.hpp"
#include "primitives.hpp"
#pragma once
namespace frost
{
	class semantic_version final
	{
	public:
		semantic_version() = default;
		semantic_version(u16 major, u16 minor, u32 patch, const wchar_t* decoration);

		bool check_compatibility(semantic_version target);
		static semantic_version get_api_version();
		
		u16 major_version;
		u16 minor_version;
		u32 patch;
		const wchar_t* decoration;
	};
}