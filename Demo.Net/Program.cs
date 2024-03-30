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
		Log.Verbose($"KEY DOWN: {keyDown.key}");
	}
	if (e is WindowEvent.KeyDown keyUp)
	{
		Log.Verbose($"KEY UP: {keyUp.key}");
		if (keyUp.key == Keycode.Escape)
			e.target.State = WindowState.Hidden;
	}
	if (e is WindowEvent.DoubleClick doubleClick)
	{
		Log.Verbose($"DOUBLE CLICK: {doubleClick.position.x}, {doubleClick.position.y}; {doubleClick.button}");
	}
	if (e is WindowEvent.Resize resize)
	{
		Log.Verbose($"RESIZE: {resize.size.width}, {resize.size.height}");
	}
	if (e is WindowEvent.Move move)
	{
		Log.Verbose($"MOVE:   {move.position.x}, {move.position.y}");
	}
	if (e is WindowEvent.MouseMove mouseMove)
	{
		Log.Verbose($"MOUSE MOVE: {mouseMove.delta.x}, {mouseMove.delta.y}");
	}
	if (e is WindowEvent.MouseScroll scroll)
	{
		Log.Verbose($"SCROLL: {scroll.delta.x}, {scroll.delta.y}");
	}
	if (e is WindowEvent.GainedFocus)
	{
		Log.Verbose($"GAINED FOCUS");
	}
	if (e is WindowEvent.LostFocus)
	{
		Log.Verbose($"LOST FOCUS");
	}
	
	if (e is WindowEvent.Activated)
	{
		Log.Verbose($"ACTIVATED");
	}
	if (e is WindowEvent.Deactivated)
	{
		Log.Verbose($"DEACTIVATED");
	}
	
	if (e is WindowEvent.Enabled)
	{
		Log.Verbose($"ENABLED");
	}
	if (e is WindowEvent.Disabled)
	{
		Log.Verbose($"DISABLED");
	}
	
	if (e is WindowEvent.ChangeState)
	{
		Log.Verbose($"STATE CHANGED");
	}
	
	if (e is WindowEvent.Create)
	{
		Log.Verbose($"CREATE");
	}
	if (e is WindowEvent.Close)
	{
		Log.Verbose($"CLOSE");
	}
	if (e is WindowEvent.Destroy)
	{
		Log.Verbose($"DESTROY");
	}
	
	if (e is WindowEvent.CursorEnter cursorEnter)
	{
		Log.Verbose($"ENTER: {cursorEnter.position.x}, {cursorEnter.position.y}");
	}
	if (e is WindowEvent.CursorMove cursorMove)
	{
		Log.Verbose($"MOVE: {cursorMove.position.x}, {cursorMove.position.y}");
	}
	if (e is WindowEvent.CursorLeave cursorLeave)
	{
		Log.Verbose($"LEAVE: {cursorLeave.position.x}, {cursorLeave.position.y}");
	}
};
var consoleLogger = new ConsoleLogSink(
	new ConsoleLogSink.Options
{
	filter = Log.Levels.All,
	colorOptions = ConsoleLogSink.Options.ColorOptions.Colored
});

Log.Subscribe(Layers.All, consoleLogger.Handler);

using var window = new Window(options);
var msg = new Frost.Thread.Message();
while (window.State != WindowState.Hidden)
{
	msg.Wait();
	msg.Dispatch();
}
