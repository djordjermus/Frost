﻿using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;

namespace Frost.Net
{
	public static class Log
	{
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

		public static void Warn(Layers activationLayers, string template, params string[] parameters)
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
			Verbose	= 1,
			Debug	= 2,
			Info	= 4,
			Warning	= 8,
			Error	= 16,
			Fatal	= 32,
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
				EntryPoint = "?logging_get_log_event_tag@api@log@frost@@SA_KXZ")]
			public static extern ulong GetLogEventTag();

			[MethodImpl(MethodImplOptions.AggressiveInlining)]
			[DllImport(
				dllName: Settings.frostApiPath,
				CallingConvention = CallingConvention.StdCall,
				EntryPoint = "?logging_log_verbose@api@log@frost@@SAXPEB_W_KPEAPEB_WPEB_K11@Z")]
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
				EntryPoint = "?logging_log_debug@api@log@frost@@SAXPEB_W_KPEAPEB_WPEB_K11@Z")]
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
				EntryPoint = "?logging_log_info@api@log@frost@@SAXPEB_W_KPEAPEB_WPEB_K11@Z")]
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
				EntryPoint = "?logging_log_warning@api@log@frost@@SAXPEB_W_KPEAPEB_WPEB_K11@Z")]
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
				EntryPoint = "?logging_log_error@api@log@frost@@SAXPEB_W_KPEAPEB_WPEB_K11@Z")]
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
				EntryPoint = "?logging_log_critical@api@log@frost@@SAXPEB_W_KPEAPEB_WPEB_K11@Z")]
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
