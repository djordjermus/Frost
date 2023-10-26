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

		public static class EventSystem
		{
			public struct RawLogEvent
			{
				public RawLogEvent() { }

				public IntPtr message_template = default;
				public ulong template_length = default;

				public IntPtr message = default;
				public ulong message_length = default;

				public IntPtr parameters = default;
				public IntPtr parameter_lengths = default;
				public ulong parameter_count = default;

				public ulong timestamp = default;
				public ulong thread_id = default;
				public byte level = default;
			}

			[UnmanagedFunctionPointer(CallingConvention.StdCall)]
			public delegate void HandlerSig(IntPtr pData);

			[UnmanagedFunctionPointer(CallingConvention.StdCall)]
			public delegate void RelaySig(ulong tag, ulong layers, IntPtr pData);



			[MethodImpl(MethodImplOptions.AggressiveInlining)]
			[DllImport(
				dllName: Settings.frostApiPath,
				CallingConvention = CallingConvention.StdCall,
				EntryPoint = "event_system_emit")]
			public static extern void Emit(ulong tag, ulong layer, IntPtr p_data);

			[MethodImpl(MethodImplOptions.AggressiveInlining)]
			[DllImport(
				dllName: Settings.frostApiPath,
				CallingConvention = CallingConvention.StdCall,
				EntryPoint = "event_system_subscribe")]
			public static extern void Subscribe(ulong tag, ulong activation_layers, HandlerSig handler);

			[MethodImpl(MethodImplOptions.AggressiveInlining)]
			[DllImport(
				dllName: Settings.frostApiPath,
				CallingConvention = CallingConvention.StdCall,
				EntryPoint = "event_system_unsubscribe")]
			public static extern void Unsubscribe(ulong tag, ulong activation_layers, HandlerSig handler);



			[MethodImpl(MethodImplOptions.AggressiveInlining)]
			[DllImport(
				dllName: Settings.frostApiPath,
				CallingConvention = CallingConvention.StdCall,
				EntryPoint = "event_system_subscribe_relay")]
			public static extern void SubscribeRelay(RelaySig relay);

			[MethodImpl(MethodImplOptions.AggressiveInlining)]
			[DllImport(
				dllName: Settings.frostApiPath,
				CallingConvention = CallingConvention.StdCall,
				EntryPoint = "event_system_unsubscribe_relay")]
			public static extern void UnsubscribeRelay(RelaySig relay);
		}

		public static class Logging
		{
			[MethodImpl(MethodImplOptions.AggressiveInlining)]
			[DllImport(
				dllName: Settings.frostApiPath,
				CallingConvention = CallingConvention.StdCall,
				EntryPoint = "logging_get_log_event_tag")]
			public static extern ulong GetLogEventTag();

			[MethodImpl(MethodImplOptions.AggressiveInlining)]
			[DllImport(
				dllName: Settings.frostApiPath,
				CallingConvention = CallingConvention.StdCall,
				EntryPoint = "logging_verbose")]
			public static extern void LogVerbose(
				IntPtr message_template,
				ulong template_length,
				IntPtr parameters,
				IntPtr lengths,
				ulong param_count,
				ulong activation_layers);

			[MethodImpl(MethodImplOptions.AggressiveInlining)]
			[DllImport(
				dllName: Settings.frostApiPath,
				CallingConvention = CallingConvention.StdCall,
				EntryPoint = "logging_debug")]
			public static extern void LogDebug(
				IntPtr message_template,
				ulong template_length,
				IntPtr parameters,
				IntPtr lengths,
				ulong param_count,
				ulong activation_layers);

			[MethodImpl(MethodImplOptions.AggressiveInlining)]
			[DllImport(
				dllName: Settings.frostApiPath,
				CallingConvention = CallingConvention.StdCall,
				EntryPoint = "logging_info")]
			public static extern void LogInfo(
				IntPtr message_template,
				ulong template_length,
				IntPtr parameters,
				IntPtr lengths,
				ulong param_count,
				ulong activation_layers);

			[MethodImpl(MethodImplOptions.AggressiveInlining)]
			[DllImport(
				dllName: Settings.frostApiPath,
				CallingConvention = CallingConvention.StdCall,
				EntryPoint = "logging_warning")]
			public static extern void LogWarning(
				IntPtr message_template,
				ulong template_length,
				IntPtr parameters,
				IntPtr lengths,
				ulong param_count,
				ulong activation_layers);

			[MethodImpl(MethodImplOptions.AggressiveInlining)]
			[DllImport(
				dllName: Settings.frostApiPath,
				CallingConvention = CallingConvention.StdCall,
				EntryPoint = "logging_error")]
			public static extern void LogError(
				IntPtr message_template,
				ulong template_length,
				IntPtr parameters,
				IntPtr lengths,
				ulong param_count,
				ulong activation_layers);

			[MethodImpl(MethodImplOptions.AggressiveInlining)]
			[DllImport(
				dllName: Settings.frostApiPath,
				CallingConvention = CallingConvention.StdCall,
				EntryPoint = "logging_critical")]
			public static extern void LogCritical(
				IntPtr message_template,
				ulong template_length,
				IntPtr parameters,
				IntPtr lengths,
				ulong param_count,
				ulong activation_layers);
		}

		public static class Resource
		{
			[MethodImpl(MethodImplOptions.AggressiveInlining)]
			[DllImport(
				dllName: Settings.frostApiPath,
				CallingConvention = CallingConvention.StdCall,
				EntryPoint = "resource_get_reference_count")]
			public static extern ulong GetRefCount(IntPtr ptr);

			[MethodImpl(MethodImplOptions.AggressiveInlining)]
			[DllImport(
				dllName: Settings.frostApiPath,
				CallingConvention = CallingConvention.StdCall,
				EntryPoint = "resource_acquire_reference")]
			public static extern void AcquireReference(IntPtr ptr);

			[MethodImpl(MethodImplOptions.AggressiveInlining)]
			[DllImport(
				dllName: Settings.frostApiPath,
				CallingConvention = CallingConvention.StdCall,
				EntryPoint = "resource_release_reference")]
			public static extern void ReleaseReference(IntPtr ptr);
		}
	}
}
