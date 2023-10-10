#include "../semantic_version.api.hpp"
bool frost::api::semantic_version::check_compatibility(semantic_version version, semantic_version target)
{
	return
		(version.major_version == target.major_version) &&
		(
			(version.minor_version > target.minor_version) ||
			((version.minor_version == target.minor_version) && (version.patch >= target.patch))
		);
}
void frost::api::semantic_version::get_api_version(semantic_version* output)
{
	output->major_version	= 0;
	output->minor_version	= 12;
	output->patch			= 0;
	output->decoration		= L"alpha";
}