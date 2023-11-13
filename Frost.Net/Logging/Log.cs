using System.Runtime.CompilerServices;

namespace Frost.Net
{
	public struct Log
	{
		public Log()
		{
			template = "";
			message = "";
			parameters = new List<string>();
			timeStamp = 0;
			threadId = 0;
			level = 0;
		}

		public readonly string template { get; init; }
		public readonly string message { get; init; }
		public readonly List<string> parameters { get; init; }
		public readonly ulong timeStamp { get; init; }
		public readonly ulong threadId { get; init; }
		public readonly Level level { get; init; }



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
		Verbose(Layers.Default, template, parameters);

		[MethodImpl(MethodImplOptions.AggressiveInlining)]
		public static void Debug(string template, params string[] parameters) =>
			Debug(Layers.Default, template, parameters);

		[MethodImpl(MethodImplOptions.AggressiveInlining)]
		public static void Info(string template, params string[] parameters) =>
			Info(Layers.Default, template, parameters);

		[MethodImpl(MethodImplOptions.AggressiveInlining)]
		public static void Warning(string template, params string[] parameters) =>
			Warning(Layers.Default, template, parameters);

		[MethodImpl(MethodImplOptions.AggressiveInlining)]
		public static void Error(string template, params string[] parameters) =>
			Error(Layers.Default, template, parameters);

		[MethodImpl(MethodImplOptions.AggressiveInlining)]
		public static void Critical(string template, params string[] parameters) =>
			Critical(Layers.Default, template, parameters);


		[MethodImpl(MethodImplOptions.AggressiveInlining)]
		public static void Subscribe(EventSystem.Handler<Log> handler) =>
			EventSystem.Subscribe(Layers.Default, handler);



		public static bool LevelFilter(Level level, Levels filter) =>
			((int)level & (int)filter) != 0;

		public enum Level
		{
			Verbose		= 1,
			Debug		= 2,
			Info		= 4,
			Warning		= 8,
			Errror		= 16,
			Critical	= 32,
		}

		[Flags]
		public enum Levels
		{
			Verbose			= 1,
			Debug			= 2,
			Info			= 4,
			Warning			= 8,
			Error			= 16,
			Critical		= 32,
			All				= 0b111111,
			DebugAndUp		= 0b111110,
			InfoAndUp		= 0b111100,
			WarningAndUp	= 0b111000,
			ErrorAndUp		= 0b110000,
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
