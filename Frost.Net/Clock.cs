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
				EntryPoint = "?frequency@api@clock@frost@@SA_KXZ")]
			public static extern ulong GetFrequency();


			[MethodImpl(MethodImplOptions.AggressiveInlining)]
			[DllImport(
				dllName: Settings.frostApiPath,
				CallingConvention = CallingConvention.StdCall,
				EntryPoint = "?period@api@clock@frost@@SANXZ")]
			public static extern double GetPeriod();


			[MethodImpl(MethodImplOptions.AggressiveInlining)]
			[DllImport(
				dllName: Settings.frostApiPath,
				CallingConvention = CallingConvention.StdCall,
				EntryPoint = "?timestamp@api@clock@frost@@SA_KXZ")]
			public static extern ulong GetTimestamp();

		}
	}
}
