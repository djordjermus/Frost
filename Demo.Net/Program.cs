using Frost;
using Frost.Logging;
using Frost.Models;
using Frost.Net.Logging.Sink;
using Frost.Net.Models;


var options = new WindowOptions();
options.procedure = e =>
{
	if (e is WindowEvent.KeyDown keyDown)
	{
		Log.EmitDebug(1, $"KEY DOWN: {keyDown.key}");
	}
	if (e is WindowEvent.KeyUp keyUp)
	{
		Log.EmitDebug(1, $"KEY UP: {keyUp.key}");
		if (keyUp.key == Keycode.Escape)
			e.target.State = WindowState.Hidden;
	}
	if (e is WindowEvent.DoubleClick doubleClick)
	{
		Log.EmitDebug(1, $"DOUBLE CLICK: {doubleClick.position.x}, {doubleClick.position.y}; {doubleClick.button}");
	}
	if (e is WindowEvent.Resize resize)
	{
		Log.EmitDebug(1, $"RESIZE: {resize.size.width}, {resize.size.height}");
	}
	if (e is WindowEvent.Move move)
	{
		Log.EmitDebug(1, $"MOVE:   {move.position.x}, {move.position.y}");
	}
	if (e is WindowEvent.MouseMove mouseMove)
	{
		Log.EmitDebug(1, $"MOUSE MOVE: {mouseMove.delta.x}, {mouseMove.delta.y}");
	}
	if (e is WindowEvent.MouseScroll scroll)
	{
		Log.EmitDebug(1, $"SCROLL: {scroll.delta.x}, {scroll.delta.y}");
	}
	if (e is WindowEvent.GainedFocus)
	{
		Log.EmitDebug(1, $"GAINED FOCUS");
	}
	if (e is WindowEvent.LostFocus)
	{
		Log.EmitDebug(1, $"LOST FOCUS");
	}
	
	if (e is WindowEvent.Activated)
	{
		Log.EmitDebug(1, $"ACTIVATED");
	}
	if (e is WindowEvent.Deactivated)
	{
		Log.EmitDebug(1, $"DEACTIVATED");
	}
	
	if (e is WindowEvent.Enabled)
	{
		Log.EmitDebug(1, $"ENABLED");
	}
	if (e is WindowEvent.Disabled)
	{
		Log.EmitDebug(1, $"DISABLED");
	}
	
	if (e is WindowEvent.ChangeState)
	{
		Log.EmitDebug(1, $"STATE CHANGED");
	}
	
	if (e is WindowEvent.Create)
	{
		Log.EmitDebug(1, $"CREATE");
	}
	if (e is WindowEvent.Close)
	{
		Log.EmitDebug(1, $"CLOSE");
	}
	if (e is WindowEvent.Destroy)
	{
		Log.EmitDebug(1, $"DESTROY");
	}
	
	if (e is WindowEvent.CursorEnter cursorEnter)
	{
		Log.EmitDebug(1, $"ENTER: {cursorEnter.position.x}, {cursorEnter.position.y}");
	}
	if (e is WindowEvent.CursorMove cursorMove)
	{
		Log.EmitDebug(1, $"MOVE: {cursorMove.position.x}, {cursorMove.position.y}");
	}
	if (e is WindowEvent.CursorLeave cursorLeave)
	{
		Log.EmitDebug(1, $"LEAVE: {cursorLeave.position.x}, {cursorLeave.position.y}");
	}
};

var txt = Log.Render("{@Greeting}, {@Subject}!", new Dictionary<string, string> { { "Greeting", "Hello"}, { "Subject", "World" } });

var consoleLogger = new ConsoleSink(
	new ConsoleSink.Options
{
	filter = Log.Levels.All,
	colorOptions = ConsoleSink.Options.ColorOptions.Colored
});

Log.Subscribe(Layers.All, consoleLogger.Handler);
while (true)
	Log.EmitFatal(1, "{@Greeting}, {@Subject}!", new Dictionary<string, string> { { "Greeting", "Hello" }, { "Subject", "World" } });
/*
using var window = new Window(options);
var msg = new Frost.Thread.Message();
while (window.State != WindowState.Hidden)
{
	msg.Wait();
	msg.Dispatch();
}
 */
