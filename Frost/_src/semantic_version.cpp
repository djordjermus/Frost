#include "../semantic_version.hpp"
namespace frost
{
	semantic_version::semantic_version(u16 major, u16 minor, u32 patch, const wchar_t* decoration) :
		major_version(major), minor_version(minor), patch(patch), decoration(decoration) {}
	
	bool semantic_version::check_compatibility(semantic_version target)
	{
		return api::check_compatibility(*this, target);
	}

	semantic_version semantic_version::get_api_version()
	{
		semantic_version result = {};
		api::get_api_version(&result);
		return result;
	}



	bool semantic_version::api::check_compatibility(semantic_version version, semantic_version target)
	{
		return
			(version.major_version == target.major_version) &&
			(
				(version.minor_version > target.minor_version) ||
				((version.minor_version == target.minor_version) && (version.patch >= target.patch))
			);
	}
	void semantic_version::api::get_api_version(semantic_version* output)
	{
		output->major_version	= 0;
		output->minor_version	= 8;
		output->patch			= 0;
		output->decoration		= L"alpha";
	}
}