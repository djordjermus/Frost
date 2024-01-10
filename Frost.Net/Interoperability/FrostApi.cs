using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;
using System.Runtime.InteropServices.Marshalling;

namespace Frost.Net.Interoperability;

/// <summary>
/// Static class housing frost.api methods
/// </summary>
internal static partial class FrostApi
{
	public const string frostApiPath = "Frost.Api.dll";
    public static partial class Clock
	{
		[LibraryImport(
			frostApiPath,
			EntryPoint = "clock_get_frequency")]
        [UnmanagedCallConv(CallConvs = [typeof(CallConvStdcall)])]
        public static partial ulong GetFrequency();


		[LibraryImport(
			frostApiPath,
			EntryPoint = "clock_get_period")]
        [UnmanagedCallConv(CallConvs = [typeof(CallConvStdcall)])]
        public static partial double GetPeriod();


		[LibraryImport(
			frostApiPath,
			EntryPoint = "clock_get_timestamp")]
        [UnmanagedCallConv(CallConvs = [typeof(CallConvStdcall)])]
        public static partial ulong GetTimestamp();
	}

    public static partial class Color
    {
        [LibraryImport(
			frostApiPath,
            EntryPoint = "color_rgba8_to_rgba32")]
        [UnmanagedCallConv(CallConvs = [typeof(CallConvStdcall)])]
        public static partial void Rgba8ToRgba32(int input, IntPtr output);

		[LibraryImport(
	        frostApiPath,
	        EntryPoint = "color_rgba32_to_rgba8")]
        [UnmanagedCallConv(CallConvs = [typeof(CallConvStdcall)])]
        public static partial void Rgba32ToRgba8(IntPtr input, IntPtr output);



		[LibraryImport(
			frostApiPath,
			EntryPoint = "color_rgba32_to_hsva")]
        [UnmanagedCallConv(CallConvs = [typeof(CallConvStdcall)])]
        public static partial void Rgba32ToHsva(IntPtr input, IntPtr output);

		[LibraryImport(
			frostApiPath,
			EntryPoint = "color_rgba32_to_hsla")]
        [UnmanagedCallConv(CallConvs = [typeof(CallConvStdcall)])]
        public static partial void Rgba32ToHsla(IntPtr input, IntPtr output);

		[LibraryImport(
			frostApiPath,
			EntryPoint = "color_rgba32_to_cmyk")]
        [UnmanagedCallConv(CallConvs = [typeof(CallConvStdcall)])]
        public static partial void Rgba32ToCmyk(IntPtr input, IntPtr output);



		[LibraryImport(
			frostApiPath,
			EntryPoint = "color_hsva_to_rgba32")]
        [UnmanagedCallConv(CallConvs = [typeof(CallConvStdcall)])]
        public static partial void HsvaToRgba32(IntPtr input, IntPtr output);

		[LibraryImport(
			frostApiPath,
			EntryPoint = "color_hsla_to_rgba32")]
        [UnmanagedCallConv(CallConvs = [typeof(CallConvStdcall)])]
        public static partial void HslaToRgba32(IntPtr input, IntPtr output);

		[LibraryImport(
			frostApiPath,
			EntryPoint = "color_cmyk_to_rgba32")]
        [UnmanagedCallConv(CallConvs = [typeof(CallConvStdcall)])]
        public static partial void CmykToRgba32(IntPtr input, IntPtr output);
    }

    public static partial class Random
    {
		[LibraryImport(
			frostApiPath,
			EntryPoint = "random_generate")]
        [UnmanagedCallConv(CallConvs = [typeof(CallConvStdcall)])]
        public static partial ulong Generate(IntPtr pSeed);

		[LibraryImport(
			frostApiPath,
			EntryPoint = "random_generate_range")]
        [UnmanagedCallConv(CallConvs = [typeof(CallConvStdcall)])]
        public static partial ulong GenerateRange(IntPtr pSeed, ulong min, ulong range);

		[LibraryImport(
			frostApiPath,
			EntryPoint = "random_generate_range_double")]
        [UnmanagedCallConv(CallConvs = [typeof(CallConvStdcall)])]
        public static partial double GenerateF64(IntPtr pSeed, double min, double range);
    }

    public static partial class SemanticVersion
    {
        [StructLayout(LayoutKind.Sequential)]
        public struct FrostSemanticVersion
        {
            public ushort major;
            public ushort minor;
            public uint patch;
            public IntPtr decoration;
        }
		[LibraryImport(
			frostApiPath,
			EntryPoint = "semantic_version_get_api_version")]
        [UnmanagedCallConv(CallConvs = [typeof(CallConvStdcall)])]
        public static partial void GetApiVersion(out FrostSemanticVersion output);

		[LibraryImport(
			frostApiPath,
			EntryPoint = "semantic_version_check_compatibility")]
        [UnmanagedCallConv(CallConvs = [typeof(CallConvStdcall)])]
        [return: MarshalAs(UnmanagedType.Bool)]
        public static partial bool CheckCompatibility(
            FrostSemanticVersion version,
            FrostSemanticVersion target);
    }

    public static partial class EventSystem
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



		[LibraryImport(
			frostApiPath,
			EntryPoint = "event_system_get_api_broadcast_layer")]
        [UnmanagedCallConv(CallConvs = [typeof(CallConvStdcall)])]
        public static partial ulong GetApiBroadcastLayer();

		[LibraryImport(
			frostApiPath,
			EntryPoint = "event_system_emit")]
        [UnmanagedCallConv(CallConvs = [typeof(CallConvStdcall)])]
        public static partial void Emit(ulong tag, ulong layer, IntPtr p_data);

		[LibraryImport(
			frostApiPath,
			EntryPoint = "event_system_subscribe")]
        [UnmanagedCallConv(CallConvs = [typeof(CallConvStdcall)])]
        public static partial void Subscribe(ulong tag, ulong activation_layers, HandlerSig handler);

		[LibraryImport(
			frostApiPath,
			EntryPoint = "event_system_unsubscribe")]
        [UnmanagedCallConv(CallConvs = [typeof(CallConvStdcall)])]
        public static partial void Unsubscribe(ulong tag, ulong activation_layers, HandlerSig handler);



		[LibraryImport(
			frostApiPath,
			EntryPoint = "event_system_subscribe_relay")]
        [UnmanagedCallConv(CallConvs = [typeof(CallConvStdcall)])]
        public static partial void SubscribeRelay(RelaySig relay);

		[LibraryImport(
			frostApiPath,
			EntryPoint = "event_system_unsubscribe_relay")]
        [UnmanagedCallConv(CallConvs = [typeof(CallConvStdcall)])]
        public static partial void UnsubscribeRelay(RelaySig relay);
    }

    public static partial class Logging
    {
		[LibraryImport(
			frostApiPath,
			EntryPoint = "logging_get_log_event_tag")]
        [UnmanagedCallConv(CallConvs = [typeof(CallConvStdcall)])]
        public static partial ulong GetLogEventTag();

		[LibraryImport(
			frostApiPath,
			EntryPoint = "logging_verbose")]
        [UnmanagedCallConv(CallConvs = [typeof(CallConvStdcall)])]
        public static partial void LogVerbose(
            IntPtr message_template,
            ulong template_length,
            IntPtr parameters,
            IntPtr lengths,
            ulong param_count,
            ulong activation_layers);

		[LibraryImport(
			frostApiPath,
			EntryPoint = "logging_debug")]
        [UnmanagedCallConv(CallConvs = [typeof(CallConvStdcall)])]
        public static partial void LogDebug(
            IntPtr message_template,
            ulong template_length,
            IntPtr parameters,
            IntPtr lengths,
            ulong param_count,
            ulong activation_layers);

		[LibraryImport(
			frostApiPath,
			EntryPoint = "logging_info")]
        [UnmanagedCallConv(CallConvs = [typeof(CallConvStdcall)])]
        public static partial void LogInfo(
            IntPtr message_template,
            ulong template_length,
            IntPtr parameters,
            IntPtr lengths,
            ulong param_count,
            ulong activation_layers);

		[LibraryImport(
			frostApiPath,
			EntryPoint = "logging_warning")]
        [UnmanagedCallConv(CallConvs = [typeof(CallConvStdcall)])]
        public static partial void LogWarning(
            IntPtr message_template,
            ulong template_length,
            IntPtr parameters,
            IntPtr lengths,
            ulong param_count,
            ulong activation_layers);

		[LibraryImport(
			frostApiPath,
			EntryPoint = "logging_error")]
        [UnmanagedCallConv(CallConvs = [typeof(CallConvStdcall)])]
        public static partial void LogError(
            IntPtr message_template,
            ulong template_length,
            IntPtr parameters,
            IntPtr lengths,
            ulong param_count,
            ulong activation_layers);

		[LibraryImport(
			frostApiPath,
			EntryPoint = "logging_critical")]
        [UnmanagedCallConv(CallConvs = [typeof(CallConvStdcall)])]
        public static partial void LogCritical(
            IntPtr message_template,
            ulong template_length,
            IntPtr parameters,
            IntPtr lengths,
            ulong param_count,
            ulong activation_layers);
    }

    public static partial class Resource
    {
		[LibraryImport(
			frostApiPath,
			EntryPoint = "resource_get_reference_count")]
        [UnmanagedCallConv(CallConvs = [typeof(CallConvStdcall)])]
        public static partial ulong GetRefCount(IntPtr ptr);

		[LibraryImport(
			frostApiPath,
			EntryPoint = "resource_acquire_reference")]
        [UnmanagedCallConv(CallConvs = [typeof(CallConvStdcall)])]
        public static partial void AcquireReference(IntPtr ptr);

		[LibraryImport(
			frostApiPath,
			EntryPoint = "resource_release_reference")]
        [UnmanagedCallConv(CallConvs = [typeof(CallConvStdcall)])]
        public static partial void ReleaseReference(IntPtr ptr);
    }

    public static partial class Synchronizable
    {
        [LibraryImport(
            frostApiPath,
            EntryPoint = "synchronizable_create_mutex")]
        [UnmanagedCallConv(CallConvs = [typeof(CallConvStdcall)])]
        public static partial IntPtr CreateMutex([MarshalAs(UnmanagedType.Bool)] bool initialOwner);

		[LibraryImport(
			frostApiPath,
			EntryPoint = "synchronizable_create_semaphore")]
        [UnmanagedCallConv(CallConvs = [typeof(CallConvStdcall)])]
        public static partial IntPtr CreateSemaphore(int count, int max);
		[LibraryImport(
			frostApiPath,
			EntryPoint = "synchronizable_create_event")]
        [UnmanagedCallConv(CallConvs = [typeof(CallConvStdcall)])]
        public static partial IntPtr CreateEvent();




		[LibraryImport(
			frostApiPath,
			EntryPoint = "synchronizable_wait")]
        [UnmanagedCallConv(CallConvs = [typeof(CallConvStdcall)])]
		[return: MarshalAs(UnmanagedType.Bool)]
		public static partial bool Wait(IntPtr target);

		[LibraryImport(
			frostApiPath,
			EntryPoint = "synchronizable_try_wait")]
        [UnmanagedCallConv(CallConvs = [typeof(CallConvStdcall)])]
		[return: MarshalAs(UnmanagedType.Bool)]
        public static partial bool TryWait(IntPtr target);

		[LibraryImport(
	        frostApiPath,
	        EntryPoint = "synchronizable_signal")]
        [UnmanagedCallConv(CallConvs = [typeof(CallConvStdcall)])]
		[return: MarshalAs(UnmanagedType.Bool)]
        public static partial bool Signal(IntPtr target);

		[LibraryImport(
			frostApiPath,
			EntryPoint = "synchronizable_get_internal_handle")]
        [UnmanagedCallConv(CallConvs = [typeof(CallConvStdcall)])]
        public static partial IntPtr GetSystemHandle(IntPtr target);



		[LibraryImport(
			frostApiPath,
			EntryPoint = "synchronizable_event_reset")]
        [UnmanagedCallConv(CallConvs = [typeof(CallConvStdcall)])]
		[return: MarshalAs(UnmanagedType.Bool)]
        public static partial bool ResetEvent(IntPtr target);




		[LibraryImport(
			frostApiPath,
			EntryPoint = "synchronizable_wait_one")]
        [UnmanagedCallConv(CallConvs = [typeof(CallConvStdcall)])]
        public static partial int WaitOne(IntPtr pArray, int count);

		[LibraryImport(
			frostApiPath,
			EntryPoint = "synchronizable_wait_all")]
        [UnmanagedCallConv(CallConvs = [typeof(CallConvStdcall)])]
		[return: MarshalAs(UnmanagedType.Bool)]
        public static partial bool WaitAll(IntPtr pArray, int count);

		[LibraryImport(
			frostApiPath,
			EntryPoint = "synchronizable_try_wait_one")]
        [UnmanagedCallConv(CallConvs = [typeof(CallConvStdcall)])]
        public static partial int TryWaitOne(IntPtr pArray, int count);

		[LibraryImport(
			frostApiPath,
			EntryPoint = "synchronizable_try_wait_all")]
        [UnmanagedCallConv(CallConvs = [typeof(CallConvStdcall)])]
		[return: MarshalAs(UnmanagedType.Bool)]
        public static partial bool TryWait(IntPtr pArray, int count);



		[LibraryImport(
			frostApiPath,
			EntryPoint = "resource_is_synchronizable")]
        [UnmanagedCallConv(CallConvs = [typeof(CallConvStdcall)])]
		[return: MarshalAs(UnmanagedType.Bool)]
        public static partial bool IsSynchronizable(IntPtr handle);
	}

    public static partial class Keycode
    {
        [MethodImpl(MethodImplOptions.AggressiveInlining)]
		[LibraryImport(
			frostApiPath,
			EntryPoint = "keycode_to_wcs")]
        [UnmanagedCallConv(CallConvs = [typeof(CallConvStdcall)])]
        public static partial ulong ToString(
			byte keycode,
            IntPtr output,
            ulong capacity,
            [MarshalAs(UnmanagedType.Bool)]bool normalize_case);

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
		[LibraryImport(
			frostApiPath,
			EntryPoint = "keycode_get_name")]
        [UnmanagedCallConv(CallConvs = [typeof(CallConvStdcall)])]
        public static partial IntPtr GetName(byte keycode);
    }
}
