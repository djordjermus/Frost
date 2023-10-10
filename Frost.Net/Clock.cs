using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;

namespace Frost.Net
{
	public static class Clock
	{
		public static ulong Frequency => Interop.GetFrequency();
		public static double Period => Interop.GetPeriod();
		public static ulong Timestamp => Interop.GetTimestamp();



		internal static class Interop
		{
			[MethodImpl(MethodImplOptions.AggressiveInlining)]
			[DllImport(
				dllName: Settings.frostApiPath,
				CallingConvention = CallingConvention.StdCall,
				EntryPoint = "?get_frequency@clock@api@frost@@SA_KXZ")]
			public static extern ulong GetFrequency();


			[MethodImpl(MethodImplOptions.AggressiveInlining)]
			[DllImport(
				dllName: Settings.frostApiPath,
				CallingConvention = CallingConvention.StdCall,
				EntryPoint = "?get_period@clock@api@frost@@SANXZ")]
			public static extern double GetPeriod();


			[MethodImpl(MethodImplOptions.AggressiveInlining)]
			[DllImport(
				dllName: Settings.frostApiPath,
				CallingConvention = CallingConvention.StdCall,
				EntryPoint = "?get_timestamp@clock@api@frost@@SA_KXZ")]
			public static extern ulong GetTimestamp();

		}
	}
}
