using Frost.Net;
using System.Text.Json;

EventSystem.Subscribe<Log.Event>(1, LogHandler);
while (true)
{
	Log.LogError(1, new("{{0}, {1}, {abc}, {3}, {2}, {"), "", "abc", "123", "XYZ");
	Thread.Sleep(500);
}

void LogHandler(Log.Event e)
{
	Console.WriteLine(JsonSerializer.Serialize(e, options: new JsonSerializerOptions() { WriteIndented = true }));
}
void MyEventHandler(MyEvent e)
{
	Console.WriteLine(e.Text);
}
class MyEvent { public string Text { get; init; } = "Hello, World!"; }