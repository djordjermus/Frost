using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;

namespace Frost.Interoperability;

/// <summary>
/// Static class housing frost.api methods
/// </summary>
internal static partial class FrostApi
{
	public const string frostApiPath = "Frost.Api.dll";
	public struct Point2D { public int x; public int y; }
	public struct Size2D { public int width; public int height; }
	[UnmanagedFunctionPointer(CallingConvention.StdCall)]
	public delegate void Procedure(IntPtr pData);

	public static partial class Clock
	{
		[LibraryImport(
			frostApiPath,
			EntryPoint = "frost_api_clock_get_frequency")]
        [UnmanagedCallConv(CallConvs = [typeof(CallConvStdcall)])]
        public static partial ulong GetFrequency();


		[LibraryImport(
			frostApiPath,
			EntryPoint = "frost_api_clock_get_period")]
        [UnmanagedCallConv(CallConvs = [typeof(CallConvStdcall)])]
        public static partial double GetPeriod();


		[LibraryImport(
			frostApiPath,
			EntryPoint = "frost_api_clock_get_timestamp")]
        [UnmanagedCallConv(CallConvs = [typeof(CallConvStdcall)])]
        public static partial ulong GetTimestamp();
	}

    public static partial class Color
    {
        [LibraryImport(
			frostApiPath,
            EntryPoint = "frost_api_color_rgba8_to_rgba32")]
        [UnmanagedCallConv(CallConvs = [typeof(CallConvStdcall)])]
        public static partial void Rgba8ToRgba32(int input, IntPtr output);

		[LibraryImport(
	        frostApiPath,
	        EntryPoint = "frost_api_color_rgba32_to_rgba8")]
        [UnmanagedCallConv(CallConvs = [typeof(CallConvStdcall)])]
        public static partial void Rgba32ToRgba8(IntPtr input, IntPtr output);



		[LibraryImport(
			frostApiPath,
			EntryPoint = "frost_api_color_rgba32_to_hsva")]
        [UnmanagedCallConv(CallConvs = [typeof(CallConvStdcall)])]
        public static partial void Rgba32ToHsva(IntPtr input, IntPtr output);

		[LibraryImport(
			frostApiPath,
			EntryPoint = "frost_api_color_rgba32_to_hsla")]
        [UnmanagedCallConv(CallConvs = [typeof(CallConvStdcall)])]
        public static partial void Rgba32ToHsla(IntPtr input, IntPtr output);

		[LibraryImport(
			frostApiPath,
			EntryPoint = "frost_api_color_rgba32_to_cmyk")]
        [UnmanagedCallConv(CallConvs = [typeof(CallConvStdcall)])]
        public static partial void Rgba32ToCmyk(IntPtr input, IntPtr output);



		[LibraryImport(
			frostApiPath,
			EntryPoint = "frost_api_color_hsva_to_rgba32")]
        [UnmanagedCallConv(CallConvs = [typeof(CallConvStdcall)])]
        public static partial void HsvaToRgba32(IntPtr input, IntPtr output);

		[LibraryImport(
			frostApiPath,
			EntryPoint = "frost_api_color_hsla_to_rgba32")]
        [UnmanagedCallConv(CallConvs = [typeof(CallConvStdcall)])]
        public static partial void HslaToRgba32(IntPtr input, IntPtr output);

		[LibraryImport(
			frostApiPath,
			EntryPoint = "frost_api_color_cmyk_to_rgba32")]
        [UnmanagedCallConv(CallConvs = [typeof(CallConvStdcall)])]
        public static partial void CmykToRgba32(IntPtr input, IntPtr output);
    }

    public static partial class Random
    {
		[LibraryImport(
			frostApiPath,
			EntryPoint = "frost_api_random_generate")]
        [UnmanagedCallConv(CallConvs = [typeof(CallConvStdcall)])]
        public static partial ulong Generate(IntPtr pSeed);

		[LibraryImport(
			frostApiPath,
			EntryPoint = "frost_api_random_generate_range")]
        [UnmanagedCallConv(CallConvs = [typeof(CallConvStdcall)])]
        public static partial ulong GenerateRange(IntPtr pSeed, ulong min, ulong range);

		[LibraryImport(
			frostApiPath,
			EntryPoint = "frost_api_random_generate_range_double")]
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
			EntryPoint = "frost_api_semantic_version_get_api_version")]
        [UnmanagedCallConv(CallConvs = [typeof(CallConvStdcall)])]
        public static partial void GetApiVersion(out FrostSemanticVersion output);

		[LibraryImport(
			frostApiPath,
			EntryPoint = "frost_api_semantic_version_check_compatibility")]
        [UnmanagedCallConv(CallConvs = [typeof(CallConvStdcall)])]
        [return: MarshalAs(UnmanagedType.Bool)]
        public static partial bool CheckCompatibility(
            FrostSemanticVersion version,
            FrostSemanticVersion target);
    }

    public static partial class EventSystem
    {
        [UnmanagedFunctionPointer(CallingConvention.StdCall)]
        public delegate void HandlerSig(IntPtr pData);

        [UnmanagedFunctionPointer(CallingConvention.StdCall)]
        public delegate void RelaySig(ulong tag, ulong layers, IntPtr pData);



		[LibraryImport(
			frostApiPath,
			EntryPoint = "frost_api_event_system_get_api_broadcast_layer")]
        [UnmanagedCallConv(CallConvs = [typeof(CallConvStdcall)])]
        public static partial ulong GetApiBroadcastLayer();

		[LibraryImport(
			frostApiPath,
			EntryPoint = "frost_api_event_system_emit")]
        [UnmanagedCallConv(CallConvs = [typeof(CallConvStdcall)])]
        public static partial void Emit(ulong tag, ulong layer, IntPtr p_data);

		[LibraryImport(
			frostApiPath,
			EntryPoint = "frost_api_event_system_subscribe")]
        [UnmanagedCallConv(CallConvs = [typeof(CallConvStdcall)])]
        public static partial void Subscribe(ulong tag, ulong activation_layers, HandlerSig handler);

		[LibraryImport(
			frostApiPath,
			EntryPoint = "frost_api_event_system_unsubscribe")]
        [UnmanagedCallConv(CallConvs = [typeof(CallConvStdcall)])]
        public static partial void Unsubscribe(ulong tag, ulong activation_layers, HandlerSig handler);



		[LibraryImport(
			frostApiPath,
			EntryPoint = "frost_api_event_system_subscribe_relay")]
        [UnmanagedCallConv(CallConvs = [typeof(CallConvStdcall)])]
        public static partial void SubscribeRelay(RelaySig relay);

		[LibraryImport(
			frostApiPath,
			EntryPoint = "frost_api_event_system_unsubscribe_relay")]
        [UnmanagedCallConv(CallConvs = [typeof(CallConvStdcall)])]
        public static partial void UnsubscribeRelay(RelaySig relay);
    }

    public static partial class Logging
    {
		[StructLayout(LayoutKind.Sequential)]
		public struct LogParameter
		{
			public IntPtr name;
			public IntPtr value;
			public ulong name_length;
			public ulong value_length;
		};

		[StructLayout(LayoutKind.Sequential)]
		public struct RawLogEvent
		{
			public IntPtr message_template;
			public ulong message_template_length;

			public IntPtr message;
			public ulong message_length;
			
			public IntPtr parameters;
			public ulong parameter_count;
			public byte level;
		}

		[LibraryImport(
			frostApiPath,
			EntryPoint = "frost_api_logging_get_log_event_tag")]
        [UnmanagedCallConv(CallConvs = [typeof(CallConvStdcall)])]
        public static partial ulong GetLogEventTag();

		[LibraryImport(
			frostApiPath,
			EntryPoint = "frost_api_logging_render_message")]
		[UnmanagedCallConv(CallConvs = [typeof(CallConvStdcall)])]
		public static partial ulong Render(
			IntPtr message_template,
			ulong message_template_length,
			IntPtr parameters,
			ulong parameter_count,
			IntPtr output,
			ulong output_length);

		[LibraryImport(
			frostApiPath,
			EntryPoint = "frost_api_logging_log")]
		[UnmanagedCallConv(CallConvs = [typeof(CallConvStdcall)])]
		public static partial ulong Log(
			ulong activation_layers,
			IntPtr message_template,
			ulong message_template_length,
			IntPtr parameters,
			ulong parameter_count,
			byte level);
	}



    public static partial class Resource
    {
		[LibraryImport(
			frostApiPath,
			EntryPoint = "frost_api_resource_get_reference_count")]
        [UnmanagedCallConv(CallConvs = [typeof(CallConvStdcall)])]
        public static partial ulong GetRefCount(IntPtr ptr);

		[LibraryImport(
			frostApiPath,
			EntryPoint = "frost_api_resource_acquire_reference")]
        [UnmanagedCallConv(CallConvs = [typeof(CallConvStdcall)])]
        public static partial void AcquireReference(IntPtr ptr);

		[LibraryImport(
			frostApiPath,
			EntryPoint = "frost_api_resource_release_reference")]
        [UnmanagedCallConv(CallConvs = [typeof(CallConvStdcall)])]
        public static partial void ReleaseReference(IntPtr ptr);
    }

	public static partial class SystemResource
	{
		[LibraryImport(
			frostApiPath,
			EntryPoint = "frost_api_system_resource_get_system_handle")]
		[UnmanagedCallConv(CallConvs = [typeof(CallConvStdcall)])]
		public static partial IntPtr GetSystemHandle(IntPtr target);
	}

    public static partial class Synchronizable
    {
        [LibraryImport(
            frostApiPath,
            EntryPoint = "frost_api_synchronizable_create_mutex")]
        [UnmanagedCallConv(CallConvs = [typeof(CallConvStdcall)])]
        public static partial IntPtr CreateMutex([MarshalAs(UnmanagedType.Bool)] bool initialOwner);

		[LibraryImport(
			frostApiPath,
			EntryPoint = "frost_api_synchronizable_create_semaphore")]
        [UnmanagedCallConv(CallConvs = [typeof(CallConvStdcall)])]
        public static partial IntPtr CreateSemaphore(int count, int max);
		[LibraryImport(
			frostApiPath,
			EntryPoint = "frost_api_synchronizable_create_event")]
        [UnmanagedCallConv(CallConvs = [typeof(CallConvStdcall)])]
        public static partial IntPtr CreateEvent();




		[LibraryImport(
			frostApiPath,
			EntryPoint = "frost_api_synchronizable_wait")]
        [UnmanagedCallConv(CallConvs = [typeof(CallConvStdcall)])]
		[return: MarshalAs(UnmanagedType.Bool)]
		public static partial bool Wait(IntPtr target);

		[LibraryImport(
			frostApiPath,
			EntryPoint = "frost_api_synchronizable_try_wait")]
        [UnmanagedCallConv(CallConvs = [typeof(CallConvStdcall)])]
		[return: MarshalAs(UnmanagedType.Bool)]
        public static partial bool TryWait(IntPtr target);

		[LibraryImport(
	        frostApiPath,
	        EntryPoint = "frost_api_synchronizable_signal")]
        [UnmanagedCallConv(CallConvs = [typeof(CallConvStdcall)])]
		[return: MarshalAs(UnmanagedType.Bool)]
        public static partial bool Signal(IntPtr target);



		[LibraryImport(
			frostApiPath,
			EntryPoint = "frost_api_synchronizable_event_reset")]
        [UnmanagedCallConv(CallConvs = [typeof(CallConvStdcall)])]
		[return: MarshalAs(UnmanagedType.Bool)]
        public static partial bool ResetEvent(IntPtr target);




		[LibraryImport(
			frostApiPath,
			EntryPoint = "frost_api_synchronizable_wait_one")]
        [UnmanagedCallConv(CallConvs = [typeof(CallConvStdcall)])]
        public static partial int WaitOne(IntPtr pArray, int count);

		[LibraryImport(
			frostApiPath,
			EntryPoint = "frost_api_synchronizable_wait_all")]
        [UnmanagedCallConv(CallConvs = [typeof(CallConvStdcall)])]
		[return: MarshalAs(UnmanagedType.Bool)]
        public static partial bool WaitAll(IntPtr pArray, int count);

		[LibraryImport(
			frostApiPath,
			EntryPoint = "frost_api_synchronizable_try_wait_one")]
        [UnmanagedCallConv(CallConvs = [typeof(CallConvStdcall)])]
        public static partial int TryWaitOne(IntPtr pArray, int count);

		[LibraryImport(
			frostApiPath,
			EntryPoint = "frost_api_synchronizable_try_wait_all")]
        [UnmanagedCallConv(CallConvs = [typeof(CallConvStdcall)])]
		[return: MarshalAs(UnmanagedType.Bool)]
        public static partial bool TryWait(IntPtr pArray, int count);



		[LibraryImport(
			frostApiPath,
			EntryPoint = "frost_api_resource_is_synchronizable")]
        [UnmanagedCallConv(CallConvs = [typeof(CallConvStdcall)])]
		[return: MarshalAs(UnmanagedType.Bool)]
        public static partial bool IsSynchronizable(IntPtr handle);
	}

    public static partial class Keycode
    {
		[LibraryImport(
			frostApiPath,
			EntryPoint = "frost_api_keycode_to_wcs")]
        [UnmanagedCallConv(CallConvs = [typeof(CallConvStdcall)])]
        public static partial ulong ToString(
			byte keycode,
            IntPtr output,
            ulong capacity,
            [MarshalAs(UnmanagedType.Bool)]bool normalize_case);

		[LibraryImport(
			frostApiPath,
			EntryPoint = "frost_api_keycode_get_name")]
        [UnmanagedCallConv(CallConvs = [typeof(CallConvStdcall)])]
        public static partial IntPtr GetName(byte keycode);
    }

    public static partial class Thread
    {
		[LibraryImport(
		    frostApiPath,
		    EntryPoint = "frost_api_thread_create")]
		[UnmanagedCallConv(CallConvs = [typeof(CallConvStdcall)])]
		public static partial IntPtr Create(Procedure procedure, IntPtr argument);

		[LibraryImport(
			frostApiPath,
			EntryPoint = "frost_api_thread_get_current")]
		[UnmanagedCallConv(CallConvs = [typeof(CallConvStdcall)])]
		public static partial IntPtr GetCurrent();



		[LibraryImport(
			frostApiPath,
			EntryPoint = "frost_api_thread_message_create")]
		[UnmanagedCallConv(CallConvs = [typeof(CallConvStdcall)])]
		public static partial IntPtr CreateMessage();

		[LibraryImport(
			frostApiPath,
			EntryPoint = "frost_api_thread_message_wait")]
		[UnmanagedCallConv(CallConvs = [typeof(CallConvStdcall)])]
		public static partial void WaitMessage(IntPtr messagePtr);

		[LibraryImport(
			frostApiPath,
			EntryPoint = "frost_api_thread_message_peek")]
		[UnmanagedCallConv(CallConvs = [typeof(CallConvStdcall)])]
		public static partial void PeekMessage(IntPtr messagePtr);

		[LibraryImport(
			frostApiPath,
			EntryPoint = "frost_api_thread_message_dispatch")]
		[UnmanagedCallConv(CallConvs = [typeof(CallConvStdcall)])]
		public static partial void DispatchMessage(IntPtr messagePtr);

		[LibraryImport(
			frostApiPath,
			EntryPoint = "frost_api_thread_message_discard")]
		[UnmanagedCallConv(CallConvs = [typeof(CallConvStdcall)])]
		public static partial void DiscardMessage(IntPtr messagePtr);



		[LibraryImport(
			frostApiPath,
			EntryPoint = "frost_api_thread_message_send")]
		[UnmanagedCallConv(CallConvs = [typeof(CallConvStdcall)])]
		[return: MarshalAs(UnmanagedType.Bool)]
		public static partial bool SendMessage(IntPtr thread, Procedure procedure, IntPtr argument);

		[LibraryImport(
			frostApiPath,
			EntryPoint = "frost_api_thread_message_send_async")]
		[UnmanagedCallConv(CallConvs = [typeof(CallConvStdcall)])]
		[return: MarshalAs(UnmanagedType.Bool)]
		public static partial bool SendMessageAsync(IntPtr thread, IntPtr sync, Procedure procedure, IntPtr argument);
	}

	public static partial class Window
	{
		[StructLayout(LayoutKind.Sequential)]
		public unsafe struct Description
		{
            public Description() {}
            public Point2D position = new Point2D() { x = 200, y = 200 };
			public Size2D size = new Size2D() { width = 1280, height = 720 };

			public IntPtr procedure = IntPtr.Zero;
			public IntPtr data = IntPtr.Zero;

			public byte state = 2;
		}

		[StructLayout(LayoutKind.Explicit)]
		public unsafe struct WindowEvent
		{
			[FieldOffset(0)]
			public ulong type;
			[FieldOffset(8)]
			public IntPtr target;

			[FieldOffset(16)]
			public Point2D position;
			[FieldOffset(16)]
			public Size2D size;
			[FieldOffset(16)]
			public DoubleClick doubleClick;

			[FieldOffset(16)]
			public Point2D cursorEnter;
			[FieldOffset(16)]
			public Point2D cursorMove;
			[FieldOffset(16)]
			public Point2D cursorLeave;

			[FieldOffset(16)]
			public Point2D mouseMove;
			[FieldOffset(16)]
			public KeyDown keyDown;
			[FieldOffset(16)]
			public KeyUp keyUp;

			[FieldOffset(16)]
			public Point2D mouseScroll;

			public struct DoubleClick { public Point2D position; public byte key; }
			public struct KeyDown { public byte key; public IntPtr text; public bool repeat; }
			public struct KeyUp { public byte key; public IntPtr text; }

			public const ulong 
				invalid			= 0x00,
				
				enable			= 0x01,
				disable			= 0x02,
				
				activate		= 0x03,
				deactivate		= 0x04,
				
				gain_focus		= 0x05,
				lose_focus		= 0x06,
				
				change_state	= 0x07,
				
				move			= 0x08,
				resize			= 0x09,
				
				key_down		= 0x0A,
				key_up			= 0x0B,
				double_click	= 0x0C,
								   
				cursor_enter	= 0x0D,
				cursor_move		= 0x0E,
				cursor_leave	= 0x0F,
				mouse_move		= 0x10,
				mouse_scroll	= 0x11,
				
				create			= 0x12,
				close			= 0x13,
				destroy			= 0x14;
	};

		[LibraryImport(
			frostApiPath,
			EntryPoint = "frost_api_window_is_enabled")]
		[UnmanagedCallConv(CallConvs = [typeof(CallConvStdcall)])]
		[return: MarshalAs(UnmanagedType.Bool)]
		public static partial bool IsEnabled(IntPtr window);

		[LibraryImport(
			frostApiPath,
			EntryPoint = "frost_api_window_is_active")]
		[UnmanagedCallConv(CallConvs = [typeof(CallConvStdcall)])]
		[return: MarshalAs(UnmanagedType.Bool)]
		public static partial bool IsActive(IntPtr window);

		[LibraryImport(
			frostApiPath,
			EntryPoint = "frost_api_window_is_focused")]
		[UnmanagedCallConv(CallConvs = [typeof(CallConvStdcall)])]
		[return: MarshalAs(UnmanagedType.Bool)]
		public static partial bool IsFocused(IntPtr window);

		[LibraryImport(
			frostApiPath,
			EntryPoint = "frost_api_window_get_state")]
		[UnmanagedCallConv(CallConvs = [typeof(CallConvStdcall)])]
		public static partial byte GetState(IntPtr window);

		[LibraryImport(
			frostApiPath,
			EntryPoint = "frost_api_window_get_position")]
		[UnmanagedCallConv(CallConvs = [typeof(CallConvStdcall)])]
		public static partial Point2D GetPosition(IntPtr window);

		[LibraryImport(
			frostApiPath,
			EntryPoint = "frost_api_window_get_size")]
		[UnmanagedCallConv(CallConvs = [typeof(CallConvStdcall)])]
		public static partial Size2D GetSize(IntPtr window);

		[LibraryImport(
			frostApiPath,
			EntryPoint = "frost_api_window_get_procedure")]
		[UnmanagedCallConv(CallConvs = [typeof(CallConvStdcall)])]
		public static partial IntPtr GetProcedure(IntPtr window);

		[LibraryImport(
			frostApiPath,
			EntryPoint = "frost_api_window_get_data")]
		[UnmanagedCallConv(CallConvs = [typeof(CallConvStdcall)])]
		public static partial IntPtr GetData(IntPtr window);

		[LibraryImport(
			frostApiPath,
			EntryPoint = "frost_api_window_set_enabled")]
		[UnmanagedCallConv(CallConvs = [typeof(CallConvStdcall)])]
		public static partial void SetEnabled(IntPtr window, [MarshalAs(UnmanagedType.Bool)]bool enabled);

		[LibraryImport(
			frostApiPath,
			EntryPoint = "frost_api_window_set_active")]
		[UnmanagedCallConv(CallConvs = [typeof(CallConvStdcall)])]
		public static partial void SetActive(IntPtr window, [MarshalAs(UnmanagedType.Bool)]bool active);

		[LibraryImport(
			frostApiPath,
			EntryPoint = "frost_api_window_set_focused")]
		[UnmanagedCallConv(CallConvs = [typeof(CallConvStdcall)])]
		public static partial void SetFocused(IntPtr window, [MarshalAs(UnmanagedType.Bool)]bool focused);

		[LibraryImport(
			frostApiPath,
			EntryPoint = "frost_api_window_set_state")]
		[UnmanagedCallConv(CallConvs = [typeof(CallConvStdcall)])]
		public static partial void SetState(IntPtr window, byte state);

		[LibraryImport(
			frostApiPath,
			EntryPoint = "frost_api_window_set_position")]
		[UnmanagedCallConv(CallConvs = [typeof(CallConvStdcall)])]
		public static partial void SetPosition(IntPtr window, Point2D position);

		[LibraryImport(
			frostApiPath,
			EntryPoint = "frost_api_window_set_size")]
		[UnmanagedCallConv(CallConvs = [typeof(CallConvStdcall)])]
		public static partial void SetSize(IntPtr window, Size2D size);

		[LibraryImport(
			frostApiPath,
			EntryPoint = "frost_api_window_set_procedure")]
		[UnmanagedCallConv(CallConvs = [typeof(CallConvStdcall)])]
		public static partial void SetProcedure(IntPtr window, Procedure procedure);

		[LibraryImport(
			frostApiPath,
			EntryPoint = "frost_api_window_set_data")]
		[UnmanagedCallConv(CallConvs = [typeof(CallConvStdcall)])]
		public static partial void SetData(IntPtr window, IntPtr pData);


		[LibraryImport(
			frostApiPath,
			EntryPoint = "frost_api_window_pump_messages")]
		[UnmanagedCallConv(CallConvs = [typeof(CallConvStdcall)])]
		public static partial void Pump(IntPtr window);

		[LibraryImport(
			frostApiPath,
			EntryPoint = "frost_api_window_create")]
		[UnmanagedCallConv(CallConvs = [typeof(CallConvStdcall)])]
		public static partial IntPtr Create(IntPtr pDesc);

		[LibraryImport(
			frostApiPath,
			EntryPoint = "frost_api_resource_is_window")]
		[UnmanagedCallConv(CallConvs = [typeof(CallConvStdcall)])]
		[return: MarshalAs(UnmanagedType.Bool)]
		public static partial bool IsWindow(IntPtr resource);
	}
}
