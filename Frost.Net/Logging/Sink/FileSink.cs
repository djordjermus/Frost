using Frost.Logging;
using System.IO;

namespace Frost.Net.Logging.Sink;

public class FileSink
{
	private readonly string _directory;
	private readonly Func<string> _fileName;
	private readonly int _maxSize;
	private readonly int _maxCount;
	private string? _target;

	public FileSink(Options options)
	{
		_directory = options.directory;
		_fileName = options.fileName;
		_maxSize = options.maxFileSize;
		_maxCount = options.maxFileCount;
		var info = Directory.CreateDirectory(_directory);
	}

	public void Handler(ref Log e)
	{
		try
		{
			_target = GetTarget(e.message);
			File.AppendAllText(_target!, $"{e.message}\r\n");
		}
		catch (Exception)
		{
			// SILENTLY CONTINUE
		}
	}

	private string GetTarget(string write)
	{
		if (_target is null)
		{
			var path = Path.Combine(_directory, _fileName());
			File.CreateText(path).Dispose();
			return path;
		}

		var writeSize = write.Length * sizeof(char);
		if ((new FileInfo(_target).Length + writeSize) > _maxSize)
		{
			var path = Path.Combine(_directory, _fileName());
			File.CreateText(path).Dispose();
			return path;
		}
		
		return _target!;
	}

	public class Options
	{
		public string directory = "./Logs/";
		public Func<string> fileName = () => $"log-{DateTime.Now:yyyy.MM.dd-HH-mm-ss}.txt";
		public int maxFileSize = 1024 * 1024 * 100;
		public int maxFileCount = 10;
	}
}
