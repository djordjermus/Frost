using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;

namespace Frost.Net
{
	internal static class FrostApi
	{
		public const string frostApiPath = "Frost.Api.dll";

		public static class Clock
		{
			[MethodImpl(MethodImplOptions.AggressiveInlining)]
			[DllImport(
				dllName: frostApiPath,
				CallingConvention = CallingConvention.StdCall,
				EntryPoint = "clock_get_frequency")]
			public static extern ulong GetFrequency();


			[MethodImpl(MethodImplOptions.AggressiveInlining)]
			[DllImport(
				dllName: frostApiPath,
				CallingConvention = CallingConvention.StdCall,
				EntryPoint = "clock_get_period")]
			public static extern double GetPeriod();


			[MethodImpl(MethodImplOptions.AggressiveInlining)]
			[DllImport(
				dllName: frostApiPath,
				CallingConvention = CallingConvention.StdCall,
				EntryPoint = "clock_get_timestamp")]
			public static extern ulong GetTimestamp();
		}
	}
}
