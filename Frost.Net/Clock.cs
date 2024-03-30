using Frost.Interoperability;
using System.Runtime.CompilerServices;

namespace Frost;

public static class Clock
{
	/// <summary>
	/// Returns the frequency of the clock (ticks per second)
	/// </summary>
	public static ulong Frequency
	{
		[MethodImpl(MethodImplOptions.AggressiveInlining)]
		get => FrostApi.Clock.GetFrequency();
	}

	/// <summary>
	/// Returns the period of the clock (seconds per clock tick)
	/// </summary>
	public static double Period
	{
		[MethodImpl(MethodImplOptions.AggressiveInlining)]
		get => FrostApi.Clock.GetPeriod();
	}

	/// <summary>
	/// Returns current timestamp (ticks)
	/// </summary>
	public static ulong Timestamp
	{
		[MethodImpl(MethodImplOptions.AggressiveInlining)]
		get => FrostApi.Clock.GetTimestamp();
	}
}
