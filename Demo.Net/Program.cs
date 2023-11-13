using Frost.Net;

var logger = new ConsoleLogger(Log.Levels.All);
Log.Subscribe(Layers.InternalLayer, logger.Handler);
DoThing();

void DoThing()
{
	using var sync = new Frost.Net.Mutex(false);
}



