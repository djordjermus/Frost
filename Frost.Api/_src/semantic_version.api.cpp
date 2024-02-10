#include "../semantic_version.api.hpp"
FROST_API bool _stdcall frost_api_semantic_version_check_compatibility(semantic_version version, semantic_version target)
{
	return
		(version.major_version == target.major_version) &&
		(
			(version.minor_version > target.minor_version) ||
			((version.minor_version == target.minor_version) && (version.patch >= target.patch))
		);
}
FROST_API void _stdcall frost_api_semantic_version_get_api_version(semantic_version* output)
{
	output->major_version	= 0;
	output->minor_version	= 12;
	output->patch			= 0;
	output->decoration		= L"alpha";
}