using Frost.Net.Interoperability;
using Frost.Net.Utilities;

namespace Frost.Net.Models;

/// <summary>
/// Semantic version model - Major, Minor, Patch, Decoration
/// </summary>
public sealed class SemanticVersion
{
	public ushort Major { get; set; }
	public ushort Minor { get; set; }
	public uint   Patch { get; set; }
	public string Decoration { get; set; } = string.Empty;



	public bool CheckCompatibility(SemanticVersion target)
	{
		var current_fsv = new FrostApi.SemanticVersion.FrostSemanticVersion()
		{
			major = Major,
			minor = Minor,
			patch = Patch,
			decoration = IntPtr.Zero,
		};

		var target_fsv = new FrostApi.SemanticVersion.FrostSemanticVersion()
		{
			major = target.Major,
			minor = target.Minor,
			patch = target.Patch,
			decoration = IntPtr.Zero,
		};

		return FrostApi.SemanticVersion.CheckCompatibility(current_fsv, target_fsv);
	}



	public static SemanticVersion GetApiVersion()
	{
		FrostApi.SemanticVersion.GetApiVersion(out FrostApi.SemanticVersion.FrostSemanticVersion version);

		return new SemanticVersion()
		{
			Major = version.major,
			Minor = version.minor,
			Patch = version.patch,
			Decoration = Unmanaged.StringFromUnmanagedWstr(version.decoration)
		};
	}
}
