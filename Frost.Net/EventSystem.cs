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
			_logEventsTag = FrostApi.Logging.GetLogEventTag();
			FrostApi.EventSystem.SubscribeRelay(InteropRelay);
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
#pragma warning disable CS8500
				FrostApi.EventSystem.Emit((ulong)typeof(T).GetHashCode(), activationLayers.Value, (IntPtr)(&eventData));
#pragma warning restore CS8500
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
				FrostApi.EventSystem.RawLogEvent* pLog = (FrostApi.EventSystem.RawLogEvent*)pLogEvent;
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
#pragma warning disable CS8500
				T eventData = *(T*)pData.ToPointer();
#pragma warning restore CS8500
				EmitInternal(activationLayers, eventData);
			}
		}

		private static void RegisterTaggedRelay<T>(ulong tag) where T : class =>
			_relays[tag] = TaggedDirectRelay<T>;
	}
}
