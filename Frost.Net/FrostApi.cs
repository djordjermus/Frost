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

		public static class Color
		{
			[MethodImpl(MethodImplOptions.AggressiveInlining)]
            [DllImport(
                dllName: Settings.frostApiPath,
                CallingConvention = CallingConvention.StdCall,
                EntryPoint = "color_rgba8_to_rgba32")]
            public extern static void Rgba8ToRgba32(int input, IntPtr output);

            [MethodImpl(MethodImplOptions.AggressiveInlining)]
            [DllImport(
                dllName: Settings.frostApiPath,
                CallingConvention = CallingConvention.StdCall,
                EntryPoint = "color_rgba32_to_rgba8")]
            public extern static void Rgba32ToRgba8(IntPtr input, IntPtr output);



            [MethodImpl(MethodImplOptions.AggressiveInlining)]
            [DllImport(
                dllName: Settings.frostApiPath,
                CallingConvention = CallingConvention.StdCall,
                EntryPoint = "color_rgba32_to_hsva")]
            public extern static void Rgba32ToHsva(IntPtr input, IntPtr output);

            [MethodImpl(MethodImplOptions.AggressiveInlining)]
            [DllImport(
                dllName: Settings.frostApiPath,
                CallingConvention = CallingConvention.StdCall,
                EntryPoint = "color_rgba32_to_hsla")]
            public extern static void Rgba32ToHsla(IntPtr input, IntPtr output);

            [MethodImpl(MethodImplOptions.AggressiveInlining)]
            [DllImport(
                dllName: Settings.frostApiPath,
                CallingConvention = CallingConvention.StdCall,
                EntryPoint = "color_rgba32_to_cmyk")]
            public extern static void Rgba32ToCmyk(IntPtr input, IntPtr output);



            [MethodImpl(MethodImplOptions.AggressiveInlining)]
            [DllImport(
                dllName: Settings.frostApiPath,
                CallingConvention = CallingConvention.StdCall,
                EntryPoint = "color_hsva_to_rgba32")]
            public extern static void HsvaToRgba32(IntPtr input, IntPtr output);

            [MethodImpl(MethodImplOptions.AggressiveInlining)]
            [DllImport(
                dllName: Settings.frostApiPath,
                CallingConvention = CallingConvention.StdCall,
                EntryPoint = "color_hsla_to_rgba32")]
            public extern static void HslaToRgba32(IntPtr input, IntPtr output);

            [MethodImpl(MethodImplOptions.AggressiveInlining)]
            [DllImport(
                dllName: Settings.frostApiPath,
                CallingConvention = CallingConvention.StdCall,
                EntryPoint = "color_cmyk_to_rgba32")]
            public extern static void CmykToRgba32(IntPtr input, IntPtr output);
		}

        public static class Random
        {
			[MethodImpl(MethodImplOptions.AggressiveInlining)]
			[DllImport(
	            Settings.frostApiPath,
	            CallingConvention = CallingConvention.StdCall,
	            EntryPoint = "random_generate")]
			public static extern ulong Generate(IntPtr pSeed);

			[MethodImpl(MethodImplOptions.AggressiveInlining)]
			[DllImport(
				Settings.frostApiPath,
				CallingConvention = CallingConvention.StdCall,
				EntryPoint = "random_generate_range")]
			public static extern ulong GenerateRange(IntPtr pSeed, ulong min, ulong range);

			[MethodImpl(MethodImplOptions.AggressiveInlining)]
			[DllImport(
				Settings.frostApiPath,
				CallingConvention = CallingConvention.StdCall,
				EntryPoint = "random_generate_range_double")]
			public static extern double GenerateF64(IntPtr pSeed, double min, double range);
		}

        public static class SemanticVersion
        {
        	[StructLayout(LayoutKind.Sequential)]
			public struct FrostSemanticVersion
			{
				public ushort major;
				public ushort minor;
				public uint   patch;
				public IntPtr decoration;
			}
			[MethodImpl(MethodImplOptions.AggressiveInlining)]
			[DllImport(
				dllName: Settings.frostApiPath,
				CallingConvention = CallingConvention.StdCall,
				EntryPoint = "semantic_version_get_api_version")]
			public static extern void GetApiVersion(out FrostSemanticVersion output);

			[MethodImpl(MethodImplOptions.AggressiveInlining)]
			[DllImport(
				dllName: Settings.frostApiPath,
				CallingConvention = CallingConvention.StdCall,
				EntryPoint = "semantic_version_check_compatibility")]
			public static extern bool CheckCompatibility(
				FrostSemanticVersion version, 
				FrostSemanticVersion target);
        }
	}
}
