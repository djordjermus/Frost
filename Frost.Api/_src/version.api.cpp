#include "../version.api.hpp"
extern "C"
{
	FROST_API frost_semantic_version _stdcall version_get_api_version()
	{
		frost_semantic_version result;
		result.major = 0;
		result.minor = 8;
		result.patch = 0;
		result.decoration = L"alpha";
		return result;
	}
	FROST_API bool _stdcall version_check_compatibility(frost_semantic_version version, frost_semantic_version target)
	{
		return 
			(version.major == target.major) &&
			(
				 (version.minor  > target.minor) ||
				((version.minor == target.minor) && (version.patch >= target.patch))
			);
	}
}