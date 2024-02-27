namespace Frost.Logging;

/// <summary>
/// Class for outputing log event data to console
/// </summary>
public class ConsoleLogger
{
	private static readonly string _default_log_header_format = "LEVEL: {0}\nTHREAD: {1}\nTIMESTAMP: {2}";
	private readonly Log.Levels _filter;
	private readonly string _log_header;
	private readonly Dictionary<Log.Level, (ConsoleColor, ConsoleColor)> _consoleColorMap;

	/// <summary>
	/// Sets header format.
	/// {0} is log level,
	/// {1} is thread id,
	/// {2} is timestamp.
	/// </summary>
	/// <param name="headerFormat">Format string</param>
	public ConsoleLogger(Log.Levels filter = Log.Levels.All, string? customHeaderFormat = null)
	{
		_filter = filter;
		_log_header = customHeaderFormat ?? _default_log_header_format;
		_consoleColorMap = new Dictionary<Log.Level, (ConsoleColor, ConsoleColor)>();
	}

	public void SetColorScheme(Log.Level level, ConsoleColor background, ConsoleColor foreground) =>
		_consoleColorMap[level] = (background, foreground);

	public void ClearColorScheme(Log.Level level) =>
		_consoleColorMap.Remove(level);

	public void Handler(ref Log e)
	{
		if (!Log.LevelFilter(e.level, _filter))
			return;

		var header = string.Format(
			_log_header,
			e.level,
			e.threadId,
			DateTime.FromFileTime((long)e.timeStamp).ToString("yyyy.MM.dd hh.mm.ss.fffffff"));

		if (_consoleColorMap.TryGetValue(e.level, out var colors))
		{
			(ConsoleColor bg, ConsoleColor fg) = (Console.BackgroundColor, Console.ForegroundColor);
			(Console.BackgroundColor, Console.ForegroundColor) = (colors.Item1, colors.Item2);
			Console.WriteLine(header);
			Console.WriteLine($"    {e.message}");
			(Console.BackgroundColor, Console.ForegroundColor) = (bg, fg);
		}
		else
		{
			Console.WriteLine(header);
			Console.WriteLine($"    {e.message}");
		}
	}

/*
	_consoleColorMap = new()
	{
		{ Level.Verbose,    (ConsoleColor.DarkGray, ConsoleColor.White) },
		{ Level.Debug,      (ConsoleColor.Blue,     ConsoleColor.White) },
		{ Level.Info,       (ConsoleColor.White,    ConsoleColor.Black) },
		{ Level.Warning,    (ConsoleColor.Yellow,   ConsoleColor.Black) },
		{ Level.Error,      (ConsoleColor.DarkRed,  ConsoleColor.White) },
		{ Level.Critical,   (ConsoleColor.Magenta,  ConsoleColor.White) }
	};
	*/
}
