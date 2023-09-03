using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;
using System.Collections.Generic;
namespace Frost.Net
{
	public static class EventSystem
	{
		public delegate void Handler<T>(T data);
		public delegate void HandlerRef<T>(T data);
		public delegate object Redirect(IntPtr pData);
		private delegate void Relay(ulong tag, ulong activationLayers, IntPtr pData);

		private static Dictionary<ulong, Dictionary<ulong, object>> _handlers = new();
		private static Dictionary<ulong, Relay> _relays = new();

		static EventSystem()
		{
			Interop.SubscribeRelay(InteropRelay);
		}

		public static void Subscribe<T>(Layers activationLayers, Handler<T> handler) where T : class
		{
			// Register tagged relay
			RegisterTaggedRelay<T>();

			// Find or create tagged collection
			if (!_handlers.TryGetValue((ulong)typeof(T).GetHashCode(), out var collection))
			{
				collection = new Dictionary<ulong, object>();
				_handlers[(ulong)typeof(T).GetHashCode()] = collection;
			}

			// Find or create layer collection
			if (collection.TryGetValue(activationLayers, out var handlerDelegate))
				collection[activationLayers] = ((Handler<T>)handlerDelegate) + handler;
			else
				collection[activationLayers] = handler;
		}

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
				return;

			foreach (var pair in collection)
			{
				if ((pair.Key & activationLayers.Value) == 0)
					continue;

				(pair.Value as Handler<T>)?.Invoke(e);
			}
		}

		private static void InteropRelay(ulong tag, ulong activationLayers, IntPtr pData)
		{
			if (_relays.TryGetValue(tag, out var relay))
				relay(tag, activationLayers, pData);
		}

		private static void TaggedDirectRelay<T>(ulong tag, ulong activationLayers, IntPtr pData) where T : class
		{
			unsafe
			{
				T eventData = *(T*)pData.ToPointer();
				EmitInternal<T>(activationLayers, eventData);
			}
		}

		private static void RegisterTaggedRelay<T>() where T : class =>
			_relays[(ulong)typeof(T).GetHashCode()] = TaggedDirectRelay<T>;
		
		

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
	}
}

/*
 * InteropRelay
 *		|
 *		V
 *   Relay[T]
 *      |
 *      V
 * InternalEmit
 */