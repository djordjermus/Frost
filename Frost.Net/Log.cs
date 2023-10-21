using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;

namespace Frost.Net
{
	public static class Log
	{
		public static readonly Layers defaultLayers = new(0b01);



		public static void Verbose(Layers activationLayers, string template, params string[] parameters)
		{
			unsafe
			{
				var lengths = ExtractParamsLengths(parameters);
				var pointers = ParamsToIntPtrArray(parameters);
				fixed (void* pTemplate = template, pParams = pointers, pLengths = lengths)
					FrostApi.Logging.LogVerbose(
						(IntPtr)pTemplate,
						(ulong)template.Length,
						(IntPtr)pParams,
						(IntPtr)pLengths,
						(ulong)parameters.Length,
						activationLayers.Value);
			}
		}

		public static void Debug(Layers activationLayers, string template, params string[] parameters)
		{
			unsafe
			{
				var lengths = ExtractParamsLengths(parameters);
				var pointers = ParamsToIntPtrArray(parameters);
				fixed (void* pTemplate = template, pParams = pointers, pLengths = lengths)
					FrostApi.Logging.LogDebug(
						(IntPtr)pTemplate,
						(ulong)template.Length,
						(IntPtr)pParams,
						(IntPtr)pLengths,
						(ulong)parameters.Length,
						activationLayers.Value);
			}
		}

		public static void Info(Layers activationLayers, string template, params string[] parameters)
		{
			unsafe
			{
				var lengths = ExtractParamsLengths(parameters);
				var pointers = ParamsToIntPtrArray(parameters);
				fixed (void* pTemplate = template, pParams = pointers, pLengths = lengths)
					FrostApi.Logging.LogInfo(
						(IntPtr)pTemplate,
						(ulong)template.Length,
						(IntPtr)pParams,
						(IntPtr)pLengths,
						(ulong)parameters.Length,
						activationLayers.Value);
			}
		}

		public static void Warning(Layers activationLayers, string template, params string[] parameters)
		{
			unsafe
			{
				var lengths = ExtractParamsLengths(parameters);
				var pointers = ParamsToIntPtrArray(parameters);
				fixed (void* pTemplate = template, pParams = pointers, pLengths = lengths)
					FrostApi.Logging.LogWarning(
						(IntPtr)pTemplate,
						(ulong)template.Length,
						(IntPtr)pParams,
						(IntPtr)pLengths,
						(ulong)parameters.Length,
						activationLayers.Value);
			}
		}

		public static void Error(Layers activationLayers, string template, params string[] parameters)
		{
			unsafe
			{
				var lengths = ExtractParamsLengths(parameters);
				var pointers = ParamsToIntPtrArray(parameters);
				fixed (void* pTemplate = template, pParams = pointers, pLengths = lengths)
				FrostApi.Logging.LogError(
					(IntPtr)pTemplate,
					(ulong)template.Length,
					(IntPtr)pParams,
					(IntPtr)pLengths,
					(ulong)parameters.Length,
					activationLayers.Value);
			}
		}

		public static void Critical(Layers activationLayers, string template, params string[] parameters)
		{
			unsafe
			{
				var lengths = ExtractParamsLengths(parameters);
				var pointers = ParamsToIntPtrArray(parameters);
				fixed (void* pTemplate = template, pParams = pointers, pLengths = lengths)
					FrostApi.Logging.LogCritical(
						(IntPtr)pTemplate,
						(ulong)template.Length,
						(IntPtr)pParams,
						(IntPtr)pLengths,
						(ulong)parameters.Length,
						activationLayers.Value);
			}
		}



		[MethodImpl(MethodImplOptions.AggressiveInlining)]
		public static void Verbose(string template, params string[] parameters) =>
		Verbose(defaultLayers, template, parameters);

		[MethodImpl(MethodImplOptions.AggressiveInlining)]
		public static void Debug(string template, params string[] parameters) =>
			Debug(defaultLayers, template, parameters);

		[MethodImpl(MethodImplOptions.AggressiveInlining)]
		public static void Info(string template, params string[] parameters) =>
			Info(defaultLayers, template, parameters);

		[MethodImpl(MethodImplOptions.AggressiveInlining)]
		public static void Warning(string template, params string[] parameters) =>
			Warning(defaultLayers, template, parameters);

		[MethodImpl(MethodImplOptions.AggressiveInlining)]
		public static void Error(string template, params string[] parameters) =>
			Error(defaultLayers, template, parameters);

		[MethodImpl(MethodImplOptions.AggressiveInlining)]
		public static void Critical(string template, params string[] parameters) =>
			Critical(defaultLayers, template, parameters);


		[MethodImpl(MethodImplOptions.AggressiveInlining)]
		public static void Subscribe(EventSystem.Handler<Event> handler) =>
			EventSystem.Subscribe(defaultLayers, handler);

		public sealed class Event
		{
			public string Template { get; init; } = "";
			public string Message { get; init; } = "";
			public List<string> Parameters { get; init; } = new List<string>();
			public ulong TimeStamp { get; init; }
			public ulong ThreadId { get; init; }
			public Level Level { get; init; }
		}

		public enum Level
		{
			Verbose		= 1,
			Debug		= 2,
			Info		= 4,
			Warning		= 8,
			Error		= 16,
			Critical	= 32,
		}

		private static ulong[] ExtractParamsLengths(string[] parameters)
		{
			var result = new ulong[parameters.Length];
			for (int i = 0; i < result.Length; i++)
				result[i] = (ulong)(parameters[i]?.Length ?? 0);
			return result;
		}

		private static IntPtr[] ParamsToIntPtrArray(string[] parameters)
		{
			var result = new IntPtr[parameters.Length];
			for (int i = 0; i < result.Length; i++)
			{
				string next = parameters[i];
				unsafe
				{
					fixed (void* pNext = next)
						result[i] = (IntPtr)pNext;
				}
			}
			return result;
		}
	}
}
