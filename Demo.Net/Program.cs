using Frost.Net.Logging;
using Frost.Net.Models;

var e = new Frost.Net.Synchronization.SyncEvent();
var i = 0;
var logger = new ConsoleLogger();
Log.Subscribe(Layers.All, logger.Handler);
Log.Info("{0}, {1}!", "Hello", "World");


var task = Task.Run(() =>
{
	while (true)
	{
		Thread.Sleep(100);
		e.Unlock();
	}
});

while (true)
{
	Console.WriteLine(i++);
	e.Lock();
	e.Reset();
}
