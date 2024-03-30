using Frost.Logging;

namespace Frost.Net.Logging.Sink;

/// <summary>
/// Class for outputing log event data to console
/// </summary>
public class ConsoleLogSink
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
    public ConsoleLogSink(Options options)
    {
        if (options is null)
            throw new ArgumentNullException(nameof(options));

        _filter = options.filter;
        _log_header = options?.customHeaderFormat ?? _default_log_header_format;
        _consoleColorMap = new Dictionary<Log.Level, (ConsoleColor, ConsoleColor)>();

        if (options!.colorOptions is not null)
        {
		    if (options!.colorOptions!.verbose is not null)
			    _consoleColorMap[Log.Level.Verbose] = options.colorOptions.verbose.Value;
		    if (options!.colorOptions!.debug is not null)
			    _consoleColorMap[Log.Level.Debug] = options.colorOptions.debug.Value;
		    if (options!.colorOptions!.info is not null)
			    _consoleColorMap[Log.Level.Info] = options.colorOptions.info.Value;
		    if (options!.colorOptions!.warning is not null)
			    _consoleColorMap[Log.Level.Warning] = options.colorOptions.warning.Value;
		    if (options!.colorOptions!.error is not null)
			    _consoleColorMap[Log.Level.Error] = options.colorOptions.error.Value;
		    if (options!.colorOptions!.critical is not null)
			    _consoleColorMap[Log.Level.Critical] = options.colorOptions.critical.Value;
        }
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

    public class Options
    {
        public Log.Levels filter = Log.Levels.All;

        public string customHeaderFormat = "LEVEL: {0}\nTHREAD: {1}\nTIMESTAMP: {2}";

        public ColorOptions colorOptions = ColorOptions.Default;


		public class ColorOptions
        {
			public (ConsoleColor background, ConsoleColor foreground)? verbose = null;

			public (ConsoleColor background, ConsoleColor foreground)? debug = null;

			public (ConsoleColor background, ConsoleColor foreground)? info = null;

			public (ConsoleColor background, ConsoleColor foreground)? warning = null;

			public (ConsoleColor background, ConsoleColor foreground)? error = null;

			public (ConsoleColor background, ConsoleColor foreground)? critical = null;



			public static ColorOptions Default => new ColorOptions { };

			public static ColorOptions Highlighted => new ColorOptions
			{
				verbose		= (ConsoleColor.DarkGray, ConsoleColor.White),
				debug		= (ConsoleColor.Blue, ConsoleColor.White),
				info		= (ConsoleColor.White, ConsoleColor.Black),
				warning		= (ConsoleColor.Yellow, ConsoleColor.Black),
				error		= (ConsoleColor.DarkRed, ConsoleColor.White),
				critical	= (ConsoleColor.Magenta, ConsoleColor.White),
			};

			public static ColorOptions Colored => new ColorOptions
			{
				verbose		= (ConsoleColor.Black, ConsoleColor.DarkGray),
				debug		= (ConsoleColor.Black, ConsoleColor.Blue),
				info		= (ConsoleColor.Black, ConsoleColor.White),
				warning		= (ConsoleColor.Black, ConsoleColor.Yellow),
				error		= (ConsoleColor.Black, ConsoleColor.Red),
				critical	= (ConsoleColor.Black, ConsoleColor.Magenta),
			};
		}
	}
}
