using Frost.Net;
var logger = new ConsoleLogger();
Log.Subscribe(Layers.All, logger.Handler);
Log.Info("{0}, {1}!", "Hello", "World");

var str = Input.KeycodeToString(Input.Keycode.BracketClose);
str = Input.KeycodeToString(Input.Keycode.BracketOpen);
str = Input.GetKeycodeName(Input.Keycode.BracketOpen);
Console.ReadLine();
