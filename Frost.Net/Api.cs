using Frost.Interoperability;
using Frost.Utilities;

namespace Frost.Models;

/// <summary>
/// Semantic version model - Major, Minor, Patch, Decoration
/// </summary>
public static class Api
{
	public static Version GetApiVersion()
	{
		FrostApi.Version.GetApiVersion(out FrostApi.Version.FrostVersion version);

		return new Version(version.major, version.minor, version.build, version.revision);
	}
}
