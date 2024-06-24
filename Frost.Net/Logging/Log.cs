using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;
using Frost.Interoperability;
using Frost.Models;

namespace Frost.Logging;

/// <summary>
/// Contains log event data
/// </summary>
public struct Log
{
	public Log()
	{
		template = "";
		message = "";
		parameters = new Dictionary<string, string>();
		level = 0;
	}

	public readonly string template { get; init; }
	public readonly string message { get; init; }
	public readonly Dictionary<string, string> parameters { get; init; }
	public readonly Level level { get; init; }

	public static string? Render(string template, IReadOnlyDictionary<string, string>? parameters)
	{
		unsafe
		{
			string? result = null;
			var logParams = DictionaryToParameters(parameters, out var handles);

			fixed (void* pTemplate = template, pParams = logParams)
			{
				ulong length = FrostApi.Logging.Render(
					(nint)pTemplate,
					(ulong)template.Length,
					(nint)pParams,
					(ulong)(logParams?.Length ?? 0),
					nint.Zero,
					0);

				result = new string('\0', (int)length);
				fixed (void* pResult = result)
				FrostApi.Logging.Render(
					(nint)pTemplate,
					(ulong)template.Length,
					(nint)pParams,
					(ulong)(logParams?.Length ?? 0),
					(nint)pResult,
					length);
			}
			if (handles is not null)
			{
				foreach (var handle in handles)
					handle.Free();
			}

			return result;
		}
	}

	public static void Emit(
		Layers activationLayers,
		Level level,
		string template,
		IReadOnlyDictionary<string, string>? parameters)
	{
		unsafe
		{
			var logParams = DictionaryToParameters(parameters, out var handles);

			fixed (void* pTemplate = template, pParams = logParams)
			{
				FrostApi.Logging.Log(
					activationLayers.Value,
					(nint)pTemplate,
					(ulong)template.Length,
					(nint)pParams, 
					(ulong)(logParams?.Length ?? 0),
					(byte)level);
			}
			if (handles is not null)
			{
				foreach (var handle in handles)
					handle.Free();
            }
		}
	}

	public static void EmitTrace(
		Layers activationLayers,
		string template,
		IReadOnlyDictionary<string, string>? parameters = null) =>
		Emit(activationLayers, Level.Trace, template, parameters);

	public static void EmitDebug(
		Layers activationLayers,
		string template,
		IReadOnlyDictionary<string, string>? parameters = null) =>
		Emit(activationLayers, Level.Debug, template, parameters);

	public static void EmitInformation(
		Layers activationLayers,
		string template,
		IReadOnlyDictionary<string, string>? parameters = null) =>
		Emit(activationLayers, Level.Information, template, parameters);

	public static void EmitWarning(
		Layers activationLayers,
		string template,
		IReadOnlyDictionary<string, string>? parameters = null) =>
		Emit(activationLayers, Level.Warning, template, parameters);

	public static void EmitError(
		Layers activationLayers,
		string template,
		IReadOnlyDictionary<string, string>? parameters = null) =>
		Emit(activationLayers, Level.Error, template, parameters);

	public static void EmitFatal(
		Layers activationLayers,
		string template,
		IReadOnlyDictionary<string, string>? parameters = null) =>
		Emit(activationLayers, Level.Fatal, template, parameters);


	[MethodImpl(MethodImplOptions.AggressiveInlining)]
	public static void Subscribe(Layers activationLayers, EventSystem.Handler<Log> handler) =>
		EventSystem.Subscribe(activationLayers, handler);



	public static bool LevelFilter(Level level, Levels filter) =>
		((int)level & (int)filter) != 0;

	public enum Level
	{
		Trace		= 1,
		Debug		= 2,
		Information	= 4,
		Warning		= 8,
		Error		= 16,
		Fatal		= 32,
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

	[MethodImpl(MethodImplOptions.AggressiveInlining)]

	private static FrostApi.Logging.LogParameter[]? DictionaryToParameters(
		IReadOnlyDictionary<string, string>? dictionary,
		out GCHandle[]? handles)
	{
		if (dictionary is null)
		{
			handles = null;
			return null;
		}

		var result = new FrostApi.Logging.LogParameter[dictionary.Count];
		handles = new GCHandle[result.Length * 2];

		int i = 0;
		foreach (var entry in dictionary)
		{
			var keyHandle = GCHandle.Alloc(entry.Key, GCHandleType.Pinned);
			var valueHandle = GCHandle.Alloc(entry.Value, GCHandleType.Pinned);
			handles[2 * i]		= keyHandle;
			handles[2 * i + 1]	= valueHandle;
			
			result[i] = new FrostApi.Logging.LogParameter
			{
				name = keyHandle.AddrOfPinnedObject(),
				value = valueHandle.AddrOfPinnedObject(),
				name_length = (ulong)entry.Key.Length,
				value_length = (ulong)entry.Value.Length
			};
			i++;
		}

		return result;
	}
}
