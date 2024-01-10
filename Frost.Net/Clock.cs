using Frost.Net.Interoperability;

namespace Frost.Net;

public static class Clock
{
	/// <summary>
	/// Returns the frequency of the clock (ticks per second)
	/// </summary>
	public static ulong Frequency => FrostApi.Clock.GetFrequency();

	/// <summary>
	/// Returns the period of the clock (seconds per clock tick)
	/// </summary>
	public static double Period => FrostApi.Clock.GetPeriod();

	/// <summary>
	/// Returns current timestamp (ticks)
	/// </summary>
	public static ulong Timestamp => FrostApi.Clock.GetTimestamp();
}
