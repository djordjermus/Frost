using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;

namespace Frost.Net
{
	public static class Clock
	{
		public static ulong Frequency => FrostApi.Clock.GetFrequency();
		public static double Period => FrostApi.Clock.GetPeriod();
		public static ulong Timestamp => FrostApi.Clock.GetTimestamp();
	}
}
