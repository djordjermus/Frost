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
					Interop.LogVerbose(
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
					Interop.LogDebug(
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
					Interop.LogInfo(
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
					Interop.LogWarning(
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
				Interop.LogError(
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
					Interop.LogCritical(
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

		internal static class Interop
		{
			[MethodImpl(MethodImplOptions.AggressiveInlining)]
			[DllImport(
				dllName: Settings.frostApiPath,
				CallingConvention = CallingConvention.StdCall,
				EntryPoint = "?get_log_event_tag@logging@api@frost@@SA_KXZ")]
			public static extern ulong GetLogEventTag();

			[MethodImpl(MethodImplOptions.AggressiveInlining)]
			[DllImport(
				dllName: Settings.frostApiPath,
				CallingConvention = CallingConvention.StdCall,
				EntryPoint = "?verbose@logging@api@frost@@SAXPEB_W_KPEAPEB_WPEB_K11@Z")]
			public static extern void LogVerbose(
				IntPtr message_template,
				ulong template_length,
				IntPtr parameters,
				IntPtr lengths,
				ulong param_count,
				ulong activation_layers);

			[MethodImpl(MethodImplOptions.AggressiveInlining)]
			[DllImport(
				dllName: Settings.frostApiPath,
				CallingConvention = CallingConvention.StdCall,
				EntryPoint = "?debug@logging@api@frost@@SAXPEB_W_KPEAPEB_WPEB_K11@Z")]
			public static extern void LogDebug(
				IntPtr message_template,
				ulong template_length,
				IntPtr parameters,
				IntPtr lengths,
				ulong param_count,
				ulong activation_layers);

			[MethodImpl(MethodImplOptions.AggressiveInlining)]
			[DllImport(
				dllName: Settings.frostApiPath,
				CallingConvention = CallingConvention.StdCall,
				EntryPoint = "?info@logging@api@frost@@SAXPEB_W_KPEAPEB_WPEB_K11@Z")]
			public static extern void LogInfo(
				IntPtr message_template,
				ulong template_length,
				IntPtr parameters,
				IntPtr lengths,
				ulong param_count,
				ulong activation_layers);

			[MethodImpl(MethodImplOptions.AggressiveInlining)]
			[DllImport(
				dllName: Settings.frostApiPath,
				CallingConvention = CallingConvention.StdCall,
				EntryPoint = "?warning@logging@api@frost@@SAXPEB_W_KPEAPEB_WPEB_K11@Z")]
			public static extern void LogWarning(
				IntPtr message_template,
				ulong template_length,
				IntPtr parameters,
				IntPtr lengths,
				ulong param_count,
				ulong activation_layers);

			[MethodImpl(MethodImplOptions.AggressiveInlining)]
			[DllImport(
				dllName: Settings.frostApiPath,
				CallingConvention = CallingConvention.StdCall,
				EntryPoint = "?error@logging@api@frost@@SAXPEB_W_KPEAPEB_WPEB_K11@Z")]
			public static extern void LogError(
				IntPtr message_template,
				ulong template_length,
				IntPtr parameters,
				IntPtr lengths,
				ulong param_count,
				ulong activation_layers);

			[MethodImpl(MethodImplOptions.AggressiveInlining)]
			[DllImport(
				dllName: Settings.frostApiPath,
				CallingConvention = CallingConvention.StdCall,
				EntryPoint = "?critical@logging@api@frost@@SAXPEB_W_KPEAPEB_WPEB_K11@Z")]
			public static extern void LogCritical(
				IntPtr message_template,
				ulong template_length,
				IntPtr parameters,
				IntPtr lengths,
				ulong param_count,
				ulong activation_layers);
		}
	}
}
