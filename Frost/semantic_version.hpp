#include "macro.hpp"
#include "primitives.hpp"
#pragma once
namespace frost
{
	class semantic_version final
	{
	public:
		FROST_API semantic_version() = default;
		FROST_API semantic_version(u16 major, u16 minor, u32 patch, const wchar_t* decoration);

		bool FROST_API check_compatibility(semantic_version target);
		static semantic_version FROST_API get_api_version();
		
		u16 major_version;
		u16 minor_version;
		u32 patch;
		const wchar_t* decoration;



		class api final
		{
		public:
			api() = delete;
			api(api&) = delete;
			api(api&&) = delete;
			~api() = delete;

			static bool FROST_API check_compatibility(semantic_version version, semantic_version target);
			static void FROST_API get_api_version(semantic_version* output);
		};
	};
}