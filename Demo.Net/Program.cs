using Frost.Net;

var offset = Clock.Timestamp;
EventSystem.Subscribe<MyEvent>(1, fn);
EventSystem.Subscribe<MyEvent>(1, fn);
EventSystem.Emit(1, new	MyEvent());
EventSystem.Emit(2, new	MyEvent());

void fn(MyEvent e) =>
	Console.WriteLine(e.data);
class MyEvent { public string data = "Hello World!"; }
