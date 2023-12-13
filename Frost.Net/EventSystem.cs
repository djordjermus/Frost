namespace Frost.Net;

using Frost.Net.Interoperability;
using Frost.Net.Models;
using Frost.Net.Utilities;

public static class EventSystem
{
	public delegate void Handler<T>(ref T data) where T : struct;
	public delegate void HandlerRef<T>(ref T data) where T : struct;



	static EventSystem()
	{
		_logEventsTag = FrostApi.Logging.GetLogEventTag();
		FrostApi.EventSystem.SubscribeRelay(InteropRelay);
	}

	public static void Subscribe<T>(ulong tag, Layers activationLayers, Handler<T> handler) where T : struct
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
	public static void Subscribe<T>(Layers activationLayers, Handler<T> handler) where T : struct =>
		Subscribe((ulong)typeof(T).GetHashCode(), activationLayers, handler);


	public static void Emit<T>(Layers activationLayers, ref T eventData) where T : struct
	{
		unsafe
		{
#pragma warning disable CS8500
			fixed (void* pEventData = &eventData)
			FrostApi.EventSystem.Emit((ulong)typeof(T).GetHashCode(), activationLayers.Value, (IntPtr)pEventData);
#pragma warning restore CS8500
		}
	}
	public static void Emit<T>(Layers activationLayers, T eventData) where T : struct =>
		Emit(activationLayers, ref eventData);
	private static void EmitInternal<T>(Layers activationLayers, ref T e) where T : struct
	{
		if (!_handlers.TryGetValue((ulong)typeof(T).GetHashCode(), out var collection))
		{
			return;
		}

		foreach (var pair in collection)
		{
			if ((pair.Key & activationLayers.Value) == 0)
				continue;

			(pair.Value as Handler<T>)?.Invoke(ref e);
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

			/* Read message template */
			Span<char> templateSpan = stackalloc char[(int)pLog->template_length];
			Unmanaged.ReadCWSTR(pLog->message_template, templateSpan);

			/* Read message */
			Span<char> messageSpan = stackalloc char[(int)pLog->message_length];
			Unmanaged.ReadCWSTR(pLog->message, messageSpan);

			/* Find largers parameter, use it for stack alloc parameter span*/
			int maximumParameterSize = 0;
			for (int i = 0; i < (int)pLog->parameter_count; i++) 
			{
				if (maximumParameterSize < ((int*)pLog->parameter_lengths)[i])
				{
					maximumParameterSize = ((int*)pLog->parameter_lengths)[i];
				}
			}

			/* Read parameters */
			Span<char> paramBuffer = stackalloc char[maximumParameterSize];
			List<string> parameters = new List<string>();
			for (int i = 0; i < (int)pLog->parameter_count; i++)
			{
				Unmanaged.ReadCWSTR((IntPtr)((char**)pLog->parameters)[i], paramBuffer);
				var parameter = paramBuffer.Slice(0, ((int*)pLog->parameter_lengths)[i]);
				parameters.Add(parameter.ToString());
			}

			Logging.Log e = new Logging.Log()
			{
				template	= templateSpan.ToString(),
				message		= messageSpan.ToString(),
				parameters	= parameters,
				timeStamp	= pLog->timestamp,
				threadId	= pLog->thread_id,
				level		= (Logging.Log.Level)pLog->level
			};

			EmitInternal(activationLayers, ref e);
		}
	}

	private static void TaggedDirectRelay<T>(ulong tag, ulong activationLayers, IntPtr pData) where T : struct
	{
		unsafe
		{
#pragma warning disable CS8500
			T* eventData = (T*)pData.ToPointer();
			EmitInternal(activationLayers, ref *eventData);
#pragma warning restore CS8500
		}
	}
	private static void RegisterTaggedRelay<T>(ulong tag) where T : struct =>
		_relays[tag] = TaggedDirectRelay<T>;



	private delegate void Relay(ulong tag, ulong activationLayers, IntPtr pData);

	private static Dictionary<ulong, Dictionary<ulong, object>> _handlers = new();
	private static Dictionary<ulong, Relay> _relays = new();
	private static ulong _logEventsTag = 0;
}
