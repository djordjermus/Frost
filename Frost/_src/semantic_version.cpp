#include "../semantic_version.hpp"
#include "Frost.Api/version.api.hpp"
namespace frost
{
	semantic_version::semantic_version(u16 major, u16 minor, u32 patch, const wchar_t* decoration) :
		major_version(major), minor_version(minor), patch(patch), decoration(decoration) {}

	bool semantic_version::check_compatibility(semantic_version target)
	{
		frost_semantic_version target_version;
		frost_semantic_version current_version;

		current_version.major		= this->major_version;
		current_version.minor		= this->minor_version;
		current_version.patch		= this->patch;
		current_version.decoration	= this->decoration;

		target_version.major		= target.major_version;
		target_version.minor		= target.minor_version;
		target_version.patch		= target.patch;
		target_version.decoration	= target.decoration;
		return ::version_check_compatibility(current_version, target_version);
	}

	semantic_version semantic_version::get_api_version()
	{
		auto version = version_get_api_version();
		return semantic_version(
			version.major,
			version.minor,
			version.patch,
			version.decoration);
	}
}