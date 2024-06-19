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
	public delegate void Procedure(nint pData);

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
        public static partial void Rgba8ToRgba32(int input, nint output);

		[LibraryImport(
	        frostApiPath,
	        EntryPoint = "frost_api_color_rgba32_to_rgba8")]
        [UnmanagedCallConv(CallConvs = [typeof(CallConvStdcall)])]
        public static partial void Rgba32ToRgba8(nint input, nint output);



		[LibraryImport(
			frostApiPath,
			EntryPoint = "frost_api_color_rgba32_to_hsva")]
        [UnmanagedCallConv(CallConvs = [typeof(CallConvStdcall)])]
        public static partial void Rgba32ToHsva(nint input, nint output);

		[LibraryImport(
			frostApiPath,
			EntryPoint = "frost_api_color_rgba32_to_hsla")]
        [UnmanagedCallConv(CallConvs = [typeof(CallConvStdcall)])]
        public static partial void Rgba32ToHsla(nint input, nint output);

		[LibraryImport(
			frostApiPath,
			EntryPoint = "frost_api_color_rgba32_to_cmyk")]
        [UnmanagedCallConv(CallConvs = [typeof(CallConvStdcall)])]
        public static partial void Rgba32ToCmyk(nint input, nint output);



		[LibraryImport(
			frostApiPath,
			EntryPoint = "frost_api_color_hsva_to_rgba32")]
        [UnmanagedCallConv(CallConvs = [typeof(CallConvStdcall)])]
        public static partial void HsvaToRgba32(nint input, nint output);

		[LibraryImport(
			frostApiPath,
			EntryPoint = "frost_api_color_hsla_to_rgba32")]
        [UnmanagedCallConv(CallConvs = [typeof(CallConvStdcall)])]
        public static partial void HslaToRgba32(nint input, nint output);

		[LibraryImport(
			frostApiPath,
			EntryPoint = "frost_api_color_cmyk_to_rgba32")]
        [UnmanagedCallConv(CallConvs = [typeof(CallConvStdcall)])]
        public static partial void CmykToRgba32(nint input, nint output);
    }

    public static partial class Random
    {
		[LibraryImport(
			frostApiPath,
			EntryPoint = "frost_api_random_generate")]
        [UnmanagedCallConv(CallConvs = [typeof(CallConvStdcall)])]
        public static partial ulong Generate(nint pSeed);

		[LibraryImport(
			frostApiPath,
			EntryPoint = "frost_api_random_generate_range")]
        [UnmanagedCallConv(CallConvs = [typeof(CallConvStdcall)])]
        public static partial ulong GenerateRange(nint pSeed, ulong min, ulong range);

		[LibraryImport(
			frostApiPath,
			EntryPoint = "frost_api_random_generate_range_double")]
        [UnmanagedCallConv(CallConvs = [typeof(CallConvStdcall)])]
        public static partial double GenerateF64(nint pSeed, double min, double range);
    }

    public static partial class Version
    {
        [StructLayout(LayoutKind.Sequential)]
        public struct FrostVersion
        {
            public ushort major;
            public ushort minor;
            public ushort build;
            public ushort revision;
        }
		[LibraryImport(
			frostApiPath,
			EntryPoint = "frost_api_version_get_api_version")]
        [UnmanagedCallConv(CallConvs = [typeof(CallConvStdcall)])]
        public static partial void GetApiVersion(out FrostVersion output);
    }

    public static partial class EventSystem
    {
        [UnmanagedFunctionPointer(CallingConvention.StdCall)]
        public delegate void HandlerSig(nint pData);

        [UnmanagedFunctionPointer(CallingConvention.StdCall)]
        public delegate void RelaySig(ulong tag, ulong layers, nint pData);



		[LibraryImport(
			frostApiPath,
			EntryPoint = "frost_api_event_system_get_api_broadcast_layer")]
        [UnmanagedCallConv(CallConvs = [typeof(CallConvStdcall)])]
        public static partial ulong GetApiBroadcastLayer();

		[LibraryImport(
			frostApiPath,
			EntryPoint = "frost_api_event_system_emit")]
        [UnmanagedCallConv(CallConvs = [typeof(CallConvStdcall)])]
        public static partial void Emit(ulong tag, ulong layer, nint p_data);

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
			public nint name;
			public nint value;
			public ulong name_length;
			public ulong value_length;
		};

		[StructLayout(LayoutKind.Sequential)]
		public struct RawLogEvent
		{
			public nint message_template;
			public ulong message_template_length;

			public nint message;
			public ulong message_length;
			
			public nint parameters;
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
			nint message_template,
			ulong message_template_length,
			nint parameters,
			ulong parameter_count,
			nint output,
			ulong output_length);

		[LibraryImport(
			frostApiPath,
			EntryPoint = "frost_api_logging_log")]
		[UnmanagedCallConv(CallConvs = [typeof(CallConvStdcall)])]
		public static partial ulong Log(
			ulong activation_layers,
			nint message_template,
			ulong message_template_length,
			nint parameters,
			ulong parameter_count,
			byte level);
	}



    public static partial class Resource
    {
		[LibraryImport(
			frostApiPath,
			EntryPoint = "frost_api_object_get_reference_count")]
        [UnmanagedCallConv(CallConvs = [typeof(CallConvStdcall)])]
        public static partial ulong GetRefCount(nint ptr);

		[LibraryImport(
			frostApiPath,
			EntryPoint = "frost_api_object_acquire_reference")]
        [UnmanagedCallConv(CallConvs = [typeof(CallConvStdcall)])]
        public static partial void AcquireReference(nint ptr);

		[LibraryImport(
			frostApiPath,
			EntryPoint = "frost_api_object_release_reference")]
        [UnmanagedCallConv(CallConvs = [typeof(CallConvStdcall)])]
        public static partial void ReleaseReference(nint ptr);
    }

	public static partial class SystemHandleHost
	{
		[LibraryImport(
			frostApiPath,
			EntryPoint = "frost_api_system_handle_host_get_system_handle")]
		[UnmanagedCallConv(CallConvs = [typeof(CallConvStdcall)])]
		public static partial nint GetSystemHandle(nint target);
	}

    public static partial class Synchronizable
    {
        [LibraryImport(
            frostApiPath,
            EntryPoint = "frost_api_synchronizable_create_mutex")]
        [UnmanagedCallConv(CallConvs = [typeof(CallConvStdcall)])]
        public static partial nint CreateMutex([MarshalAs(UnmanagedType.Bool)] bool initialOwner);

		[LibraryImport(
			frostApiPath,
			EntryPoint = "frost_api_synchronizable_create_semaphore")]
        [UnmanagedCallConv(CallConvs = [typeof(CallConvStdcall)])]
        public static partial nint CreateSemaphore(int count, int max);
		[LibraryImport(
			frostApiPath,
			EntryPoint = "frost_api_synchronizable_create_event")]
        [UnmanagedCallConv(CallConvs = [typeof(CallConvStdcall)])]
        public static partial nint CreateEvent();




		[LibraryImport(
			frostApiPath,
			EntryPoint = "frost_api_synchronizable_wait")]
        [UnmanagedCallConv(CallConvs = [typeof(CallConvStdcall)])]
		[return: MarshalAs(UnmanagedType.Bool)]
		public static partial bool Wait(nint target);

		[LibraryImport(
			frostApiPath,
			EntryPoint = "frost_api_synchronizable_try_wait")]
        [UnmanagedCallConv(CallConvs = [typeof(CallConvStdcall)])]
		[return: MarshalAs(UnmanagedType.Bool)]
        public static partial bool TryWait(nint target);

		[LibraryImport(
	        frostApiPath,
	        EntryPoint = "frost_api_synchronizable_signal")]
        [UnmanagedCallConv(CallConvs = [typeof(CallConvStdcall)])]
		[return: MarshalAs(UnmanagedType.Bool)]
        public static partial bool Signal(nint target);



		[LibraryImport(
			frostApiPath,
			EntryPoint = "frost_api_synchronizable_event_reset")]
        [UnmanagedCallConv(CallConvs = [typeof(CallConvStdcall)])]
		[return: MarshalAs(UnmanagedType.Bool)]
        public static partial bool ResetEvent(nint target);




		[LibraryImport(
			frostApiPath,
			EntryPoint = "frost_api_synchronizable_wait_one")]
        [UnmanagedCallConv(CallConvs = [typeof(CallConvStdcall)])]
        public static partial int WaitOne(nint pArray, int count);

		[LibraryImport(
			frostApiPath,
			EntryPoint = "frost_api_synchronizable_wait_all")]
        [UnmanagedCallConv(CallConvs = [typeof(CallConvStdcall)])]
		[return: MarshalAs(UnmanagedType.Bool)]
        public static partial bool WaitAll(nint pArray, int count);

		[LibraryImport(
			frostApiPath,
			EntryPoint = "frost_api_synchronizable_try_wait_one")]
        [UnmanagedCallConv(CallConvs = [typeof(CallConvStdcall)])]
        public static partial int TryWaitOne(nint pArray, int count);

		[LibraryImport(
			frostApiPath,
			EntryPoint = "frost_api_synchronizable_try_wait_all")]
        [UnmanagedCallConv(CallConvs = [typeof(CallConvStdcall)])]
		[return: MarshalAs(UnmanagedType.Bool)]
        public static partial bool TryWait(nint pArray, int count);
	}

    public static partial class Keycode
    {
		[LibraryImport(
			frostApiPath,
			EntryPoint = "frost_api_keycode_to_wcs")]
        [UnmanagedCallConv(CallConvs = [typeof(CallConvStdcall)])]
        public static partial ulong ToString(
			byte keycode,
            nint output,
            ulong capacity,
            [MarshalAs(UnmanagedType.Bool)]bool normalize_case);

		[LibraryImport(
			frostApiPath,
			EntryPoint = "frost_api_keycode_get_name")]
        [UnmanagedCallConv(CallConvs = [typeof(CallConvStdcall)])]
        public static partial nint GetName(byte keycode);
    }

    public static partial class Thread
    {
		[LibraryImport(
		    frostApiPath,
		    EntryPoint = "frost_api_thread_create")]
		[UnmanagedCallConv(CallConvs = [typeof(CallConvStdcall)])]
		public static partial nint Create(Procedure procedure, nint argument);

		[LibraryImport(
			frostApiPath,
			EntryPoint = "frost_api_thread_get_current")]
		[UnmanagedCallConv(CallConvs = [typeof(CallConvStdcall)])]
		public static partial nint GetCurrent();



		[LibraryImport(
			frostApiPath,
			EntryPoint = "frost_api_thread_message_create")]
		[UnmanagedCallConv(CallConvs = [typeof(CallConvStdcall)])]
		public static partial nint CreateMessage();

		[LibraryImport(
			frostApiPath,
			EntryPoint = "frost_api_thread_message_dispatch")]
		[UnmanagedCallConv(CallConvs = [typeof(CallConvStdcall)])]
		public static partial void DispatchMessage(nint messagePtr);

		[LibraryImport(
			frostApiPath,
			EntryPoint = "frost_api_thread_message_discard")]
		[UnmanagedCallConv(CallConvs = [typeof(CallConvStdcall)])]
		public static partial void DiscardMessage(nint messagePtr);



		[LibraryImport(
			frostApiPath,
			EntryPoint = "frost_api_thread_message_send")]
		[UnmanagedCallConv(CallConvs = [typeof(CallConvStdcall)])]
		[return: MarshalAs(UnmanagedType.Bool)]
		public static partial bool SendMessage(nint thread, Procedure procedure, nint argument);

		[LibraryImport(
			frostApiPath,
			EntryPoint = "frost_api_thread_message_send_async")]
		[UnmanagedCallConv(CallConvs = [typeof(CallConvStdcall)])]
		[return: MarshalAs(UnmanagedType.Bool)]
		public static partial bool SendMessageAsync(nint thread, nint sync, Procedure procedure, nint argument);
	}

	public static partial class Window
	{
		[StructLayout(LayoutKind.Sequential)]
		public unsafe struct Description
		{
            public Description() {}
            public Point2D position = new Point2D() { x = 200, y = 200 };
			public Size2D size = new Size2D() { width = 1280, height = 720 };

			public nint procedure = nint.Zero;
			public nint data = nint.Zero;

			public byte state = 2;
		}

		[StructLayout(LayoutKind.Explicit)]
		public unsafe struct WindowEvent
		{
			[FieldOffset(0)]
			public ulong type;
			[FieldOffset(8)]
			public nint target;

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
			public struct KeyDown { public byte key; public nint text; public bool repeat; }
			public struct KeyUp { public byte key; public nint text; }

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
		public static partial bool IsEnabled(nint window);

		[LibraryImport(
			frostApiPath,
			EntryPoint = "frost_api_window_is_active")]
		[UnmanagedCallConv(CallConvs = [typeof(CallConvStdcall)])]
		[return: MarshalAs(UnmanagedType.Bool)]
		public static partial bool IsActive(nint window);

		[LibraryImport(
			frostApiPath,
			EntryPoint = "frost_api_window_is_focused")]
		[UnmanagedCallConv(CallConvs = [typeof(CallConvStdcall)])]
		[return: MarshalAs(UnmanagedType.Bool)]
		public static partial bool IsFocused(nint window);

		[LibraryImport(
			frostApiPath,
			EntryPoint = "frost_api_window_get_state")]
		[UnmanagedCallConv(CallConvs = [typeof(CallConvStdcall)])]
		public static partial byte GetState(nint window);

		[LibraryImport(
			frostApiPath,
			EntryPoint = "frost_api_window_get_position")]
		[UnmanagedCallConv(CallConvs = [typeof(CallConvStdcall)])]
		public static partial Point2D GetPosition(nint window);

		[LibraryImport(
			frostApiPath,
			EntryPoint = "frost_api_window_get_size")]
		[UnmanagedCallConv(CallConvs = [typeof(CallConvStdcall)])]
		public static partial Size2D GetSize(nint window);

		[LibraryImport(
			frostApiPath,
			EntryPoint = "frost_api_window_get_procedure")]
		[UnmanagedCallConv(CallConvs = [typeof(CallConvStdcall)])]
		public static partial nint GetProcedure(nint window);

		[LibraryImport(
			frostApiPath,
			EntryPoint = "frost_api_window_get_data")]
		[UnmanagedCallConv(CallConvs = [typeof(CallConvStdcall)])]
		public static partial nint GetData(nint window);

		[LibraryImport(
			frostApiPath,
			EntryPoint = "frost_api_window_set_enabled")]
		[UnmanagedCallConv(CallConvs = [typeof(CallConvStdcall)])]
		public static partial void SetEnabled(nint window, [MarshalAs(UnmanagedType.Bool)]bool enabled);

		[LibraryImport(
			frostApiPath,
			EntryPoint = "frost_api_window_set_active")]
		[UnmanagedCallConv(CallConvs = [typeof(CallConvStdcall)])]
		public static partial void SetActive(nint window, [MarshalAs(UnmanagedType.Bool)]bool active);

		[LibraryImport(
			frostApiPath,
			EntryPoint = "frost_api_window_set_focused")]
		[UnmanagedCallConv(CallConvs = [typeof(CallConvStdcall)])]
		public static partial void SetFocused(nint window, [MarshalAs(UnmanagedType.Bool)]bool focused);

		[LibraryImport(
			frostApiPath,
			EntryPoint = "frost_api_window_set_state")]
		[UnmanagedCallConv(CallConvs = [typeof(CallConvStdcall)])]
		public static partial void SetState(nint window, byte state);

		[LibraryImport(
			frostApiPath,
			EntryPoint = "frost_api_window_set_position")]
		[UnmanagedCallConv(CallConvs = [typeof(CallConvStdcall)])]
		public static partial void SetPosition(nint window, Point2D position);

		[LibraryImport(
			frostApiPath,
			EntryPoint = "frost_api_window_set_size")]
		[UnmanagedCallConv(CallConvs = [typeof(CallConvStdcall)])]
		public static partial void SetSize(nint window, Size2D size);

		[LibraryImport(
			frostApiPath,
			EntryPoint = "frost_api_window_set_procedure")]
		[UnmanagedCallConv(CallConvs = [typeof(CallConvStdcall)])]
		public static partial void SetProcedure(nint window, Procedure procedure);

		[LibraryImport(
			frostApiPath,
			EntryPoint = "frost_api_window_set_data")]
		[UnmanagedCallConv(CallConvs = [typeof(CallConvStdcall)])]
		public static partial void SetData(nint window, nint pData);


		[LibraryImport(
			frostApiPath,
			EntryPoint = "frost_api_window_pump_messages")]
		[UnmanagedCallConv(CallConvs = [typeof(CallConvStdcall)])]
		public static partial void Pump(nint window);

		[LibraryImport(
			frostApiPath,
			EntryPoint = "frost_api_window_create")]
		[UnmanagedCallConv(CallConvs = [typeof(CallConvStdcall)])]
		public static partial nint Create(nint pDesc);
	}

	public static partial class Audio
	{

        [LibraryImport(
            frostApiPath,
            EntryPoint = "test_audio")]
        [UnmanagedCallConv(CallConvs = [typeof(CallConvStdcall)])]
        public static partial void TestAudio();
    }
}
