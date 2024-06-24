using Frost.Interoperability;
using Frost.Utilities;

namespace Frost.Models;

/// <summary>
/// Semantic version model - Major, Minor, Patch, Decoration
/// </summary>
public sealed class Version
{
	public ushort Major { get; set; }
	public ushort Minor { get; set; }
	public ushort BuildNumber { get; set; }
	public ushort Revision { get; set; }



	public static Version GetApiVersion()
	{
		FrostApi.Version.GetApiVersion(out FrostApi.Version.FrostVersion version);

		return new Version()
		{
			Major = version.major,
			Minor = version.minor,
			BuildNumber = version.build,
			Revision = version.revision
		};
	}
}
