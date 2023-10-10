#include "../semantic_version.hpp"
#include "Frost.Api/semantic_version.api.hpp"
namespace frost
{
	semantic_version::semantic_version(u16 major, u16 minor, u32 patch, const wchar_t* decoration) :
		major_version(major), minor_version(minor), patch(patch), decoration(decoration) {}
	
	bool semantic_version::check_compatibility(semantic_version target)
	{
		return api::semantic_version::check_compatibility(
			reinterpret_cast<api::semantic_version&>(*this),
			reinterpret_cast<api::semantic_version&>(target));
	}

	semantic_version semantic_version::get_api_version()
	{
		semantic_version result = {};
		api::semantic_version::get_api_version(reinterpret_cast<api::semantic_version*>(&result));
		return result;
	}
}