using System.Reflection;
using Frost.Interoperability;
using Frost.Models;
using Frost.Net.Interoperability;
using Frost.Utilities;

namespace Frost;

public static class EventSystem
{
	public delegate void Handler<T>(ref T data) where T : struct;
	public delegate void HandlerRef<T>(ref T data) where T : struct;

    private static readonly Dictionary<ulong, Dictionary<ulong, object>> _handlers = new();
    private static readonly Dictionary<ulong, Relay> _relays = new();
    private static readonly ulong _logEventsTag = 0;
	private static readonly GCHandleHost _gcRelay;

    /// <summary>
    /// Subscribe method via reflections.
    /// </summary>
    /// <param name="activationLayers">Event layers that activate the method.</param>
    /// <param name="method">Method info of target method.</param>
    /// <param name="obj">Optional this parameter of the method.</param>
    public static void Subscribe(Layers activationLayers, MethodInfo method, object? obj = null)
	{
		var type = method.GetParameters().First().ParameterType.GetElementType();
		var delegateType = typeof(Handler<>).MakeGenericType(type!);
		var hash = (ulong)type!.GetHashCode();

		// Register tagged relay
		typeof(EventSystem)
			?.GetMethod(nameof(RegisterTaggedRelay))
			?.MakeGenericMethod(method.GetParameters().First().ParameterType).Invoke(null, new object[] { hash });

		// Find or create tagged collection
		if (!_handlers.TryGetValue(hash, out var collection))
		{
			collection = new Dictionary<ulong, object>();
			_handlers[hash] = collection;
		}
		// typeof(Handler<>).MakeGenericType(type)
		// Find or create layer collection
		if (collection.TryGetValue(activationLayers, out var handlerDelegate))
			collection[activationLayers] = Delegate.Combine(handlerDelegate as Delegate, method.CreateDelegate(delegateType));
		else
			collection[activationLayers] = method.CreateDelegate(delegateType);
		
	}

	/// <summary>
	/// Subscribe method via reflections.
	/// </summary>
	/// <typeparam name="T">Type of event data./typeparam>
	/// <param name="tag">Type tag, by default (ulong)typeof(T).GetHashCode()</param>
	/// <param name="activationLayers">Event layers that activate the method.</param>
	/// <param name="handler">Subscribed delegate.</param>
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

	/// <summary>
	/// Subscribe method via reflections.
	/// </summary>
	/// <typeparam name="T">Type of event data./typeparam>
	/// <param name="activationLayers">Event layers that activate the method.</param>
	/// <param name="handler">Subscribed delegate.</param>
	public static void Subscribe<T>(Layers activationLayers, Handler<T> handler) where T : struct =>
		Subscribe((ulong)typeof(T).GetHashCode(), activationLayers, handler);



	/// <summary>
	/// Emits an event on given layers.
	/// </summary>
	/// <typeparam name="T">Type of event data.</typeparam>
	/// <param name="activationLayers">Layers on which event is emitted on</param>
	/// <param name="eventData">Event data given to handlers</param>
	public static void Emit<T>(Layers activationLayers, ref T eventData) where T : struct
	{
		unsafe
		{
#pragma warning disable CS8500
			fixed (void* pEventData = &eventData)
			FrostApi.EventSystem.Emit((ulong)typeof(T).GetHashCode(), activationLayers.Value, (nint)pEventData);
#pragma warning restore CS8500
		}
	}

	/// <summary>
	/// Emits an event on given layers.
	/// </summary>
	/// <typeparam name="T">Type of event data.</typeparam>
	/// <param name="activationLayers">Layers on which event is emitted on</param>
	/// <param name="eventData">Event data given to handlers</param>
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
			var x = (pair.Value as Handler<T>);
			(pair.Value as Handler<T>)?.Invoke(ref e);
		}
	}

	private static void InteropRelay(ulong tag, ulong activationLayers, nint pData)
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
	private static void LogEventsRelay(ulong tag, ulong activationLayers, nint pLogEvent)
	{
		unsafe
		{
			FrostApi.Logging.RawLogEvent* pLog = (FrostApi.Logging.RawLogEvent*)pLogEvent;

			/* Read message template */
			Span<char> templateSpan = stackalloc char[(int)pLog->message_template_length];
			Unmanaged.ReadCWSTR(pLog->message_template, templateSpan);

			/* Read message */
			Span<char> messageSpan = stackalloc char[(int)pLog->message_length];
			Unmanaged.ReadCWSTR(pLog->message, messageSpan);

			/* Find largers parameter, use it for stack alloc parameter span*/
			ulong maxParamNameSize = 0;
			ulong maxParamValueSize = 0;
			for (ulong i = 0; i < pLog->parameter_count; i++)
			{
				FrostApi.Logging.LogParameter* pParameter = ((FrostApi.Logging.LogParameter*)pLog->parameters) + i;
				if (maxParamNameSize < pParameter->name_length)
					maxParamNameSize = pParameter->name_length;

				if (maxParamValueSize < pParameter->value_length)
					maxParamValueSize = pParameter->value_length;
			}

			/* Read parameters */
			Span<char> paramNameBuf = stackalloc char[(int)maxParamNameSize];
			Span<char> paramValueBuf = stackalloc char[(int)maxParamValueSize];
			Dictionary<string, string> parameters = new Dictionary<string, string>();
			for (int i = 0; i < (int)pLog->parameter_count; i++)
			{
				FrostApi.Logging.LogParameter* pParameter = ((FrostApi.Logging.LogParameter*)pLog->parameters) + i;
				var key = paramNameBuf.Slice(0, (int)pParameter->name_length);
				var value = paramValueBuf.Slice(0, (int)pParameter->value_length);
				Unmanaged.ReadCWSTR(pParameter->name, key);
				Unmanaged.ReadCWSTR(pParameter->value, value);
				parameters[key.ToString()] = value.ToString();
			}

			Logging.Log e = new Logging.Log()
			{
				template	= templateSpan.ToString(),
				message		= messageSpan.ToString(),
				parameters	= parameters,
				level		= (Logging.Log.Level)pLog->level
			};

			EmitInternal(activationLayers, ref e);
		}
	}

	private static void TaggedDirectRelay<T>(ulong tag, ulong activationLayers, nint pData) where T : struct
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

	static EventSystem()
	{
		_logEventsTag = FrostApi.Logging.GetLogEventTag();
        FrostApi.EventSystem.SubscribeRelay(GCHandleHost.Alloc<FrostApi.EventSystem.RelaySig>(InteropRelay, out _gcRelay));
    }

	private delegate void Relay(ulong tag, ulong activationLayers, nint pData);
}
