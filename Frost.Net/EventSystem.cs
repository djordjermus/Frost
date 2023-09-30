using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;
namespace Frost.Net
{
	public static class EventSystem
	{
		public delegate void Handler<T>(T data);
		public delegate void HandlerRef<T>(T data);
		private delegate void Relay(ulong tag, ulong activationLayers, IntPtr pData);

		private static Dictionary<ulong, Dictionary<ulong, object>> _handlers = new();
		private static Dictionary<ulong, Relay> _relays = new();
		private static ulong _logEventsTag = 0;

		static EventSystem()
		{
			_logEventsTag = Log.Interop.GetLogEventTag();
			Interop.SubscribeRelay(InteropRelay);
		}

		public static void Subscribe<T>(ulong tag, Layers activationLayers, Handler<T> handler) where T : class
		{
			// Register tagged relay
			RegisterTaggedRelay<T>(tag);

			// Find or create tagged collection
			if (!_handlers.TryGetValue((ulong)typeof(T).GetHashCode(), out var collection))
			{
				collection = new Dictionary<ulong, object>();
				_handlers[tag] = collection;
			}

			// Find or create layer collection
			if (collection.TryGetValue(activationLayers, out var handlerDelegate))
				collection[activationLayers] = ((Handler<T>)handlerDelegate) + handler;
			else
				collection[activationLayers] = handler;
		}

		public static void Subscribe<T>(Layers activationLayers, Handler<T> handler) where T : class =>
			Subscribe((ulong)typeof(T).GetHashCode(), activationLayers, handler);
		

		public static void Emit<T>(Layers activationLayers, T eventData) where T : class
		{
			unsafe
			{
				Interop.Emit((ulong)typeof(T).GetHashCode(), activationLayers.Value, (IntPtr)(&eventData));
			}
		}

		private static void EmitInternal<T>(Layers activationLayers, T e) where T : class
		{
			if (!_handlers.TryGetValue((ulong)typeof(T).GetHashCode(), out var collection))
			{
				return;
			}

			foreach (var pair in collection)
			{
				if ((pair.Key & activationLayers.Value) == 0)
					continue;

				(pair.Value as Handler<T>)?.Invoke(e);
			}
		}

		private static void InteropRelay(ulong tag, ulong activationLayers, IntPtr pData)
		{
			if (tag == _logEventsTag)
			{
			 	LogEventsRelay(tag, activationLayers, pData);
			}
			else if (_relays.TryGetValue(tag, out var relay))
			{
			 	relay(tag, activationLayers, pData);
			}
		}

		private static void LogEventsRelay(ulong tag, ulong activationLayers, IntPtr pLogEvent)
		{
			unsafe
			{
				RawLogEvent* pLog = (RawLogEvent*)pLogEvent;
				string template = Unmanaged.StringFromUnmanagedWstr(pLog->message_template, (int)pLog->template_length);
				string message = Unmanaged.StringFromUnmanagedWstr(pLog->message, (int)pLog->message_length);
				List<string> parameters = new List<string>();
				for (int i = 0; i < (int)pLog->parameter_count; i++)
				{
					var parameter = Unmanaged.StringFromUnmanagedWstr(
						(IntPtr)((char**)pLog->parameters)[i], 
						((int*)pLog->parameter_lengths)[i]);
					parameters.Add(parameter);
				}

				Log.Event e = new Log.Event()
				{
					Template	= template,
					Message		= message,
					Parameters	= parameters,
					TimeStamp	= pLog->timestamp,
					ThreadId	= pLog->thread_id,
					Level		= (Log.Level)pLog->level
				};

				EmitInternal(activationLayers, e);
			}
		}

		private static void TaggedDirectRelay<T>(ulong tag, ulong activationLayers, IntPtr pData) where T : class
		{
			unsafe
			{
				T eventData = *(T*)pData.ToPointer();
				EmitInternal<T>(activationLayers, eventData);
			}
		}

		private static void RegisterTaggedRelay<T>(ulong tag) where T : class =>
			_relays[tag] = TaggedDirectRelay<T>;

		internal static class Interop
		{
			[UnmanagedFunctionPointer(CallingConvention.StdCall)]
			public delegate void HandlerSig(IntPtr pData);

			[UnmanagedFunctionPointer(CallingConvention.StdCall)]
			public delegate void RelaySig(ulong tag, ulong layers, IntPtr pData);



			[MethodImpl(MethodImplOptions.AggressiveInlining)]
			[DllImport(
				dllName: Settings.frostApiPath,
				CallingConvention = CallingConvention.StdCall,
				EntryPoint = "?event_system_emit@api@event_system@frost@@SAX_K0PEAX@Z")]
			public static extern void Emit(ulong tag, ulong layer, IntPtr p_data);

			[MethodImpl(MethodImplOptions.AggressiveInlining)]
			[DllImport(
				dllName: Settings.frostApiPath,
				CallingConvention = CallingConvention.StdCall,
				EntryPoint = "?event_system_subscribe@api@event_system@frost@@SAX_K0P6AXPEAX@Z@Z")]
			public static extern void Subscribe(ulong tag, ulong activation_layers, HandlerSig handler);

			[MethodImpl(MethodImplOptions.AggressiveInlining)]
			[DllImport(
				dllName: Settings.frostApiPath,
				CallingConvention = CallingConvention.StdCall,
				EntryPoint = "?event_system_unsubscribe@api@event_system@frost@@SAX_K0P6AXPEAX@Z@Z")]
			public static extern void Unsubscribe(ulong tag, ulong activation_layers, HandlerSig handler);



			[MethodImpl(MethodImplOptions.AggressiveInlining)]
			[DllImport(
				dllName: Settings.frostApiPath,
				CallingConvention = CallingConvention.StdCall,
				EntryPoint = "?event_system_subscribe_relay@api@event_system@frost@@SAXP6AX_K0PEAX@Z@Z")]
			public static extern void SubscribeRelay(RelaySig relay);

			[MethodImpl(MethodImplOptions.AggressiveInlining)]
			[DllImport(
				dllName: Settings.frostApiPath,
				CallingConvention = CallingConvention.StdCall,
				EntryPoint = "?event_system_unsubscribe_relay@api@event_system@frost@@SAXP6AX_K0PEAX@Z@Z")]
			public static extern void UnsubscribeRelay(RelaySig relay);
		}

		private struct RawLogEvent
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
	}
}
