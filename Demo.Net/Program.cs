using Frost;
using Frost.Net.Models;


var options = new WindowOptions();
options.procedure = e =>
{
	if (e is WindowEvent.KeyDown keyDown)
	{
		Console.WriteLine($"KEY DOWN: {keyDown.key}");
	}
	if (e is WindowEvent.KeyDown keyUp)
	{
		Console.WriteLine($"KEY UP: {keyUp.key}");
		if (keyUp.key == Keycode.Escape)
			e.target.State = WindowState.Hidden;
	}
	if (e is WindowEvent.DoubleClick doubleClick)
	{
		Console.WriteLine($"DOUBLE CLICK: {doubleClick.position.x}, {doubleClick.position.y}; {doubleClick.button}");
	}
	if (e is WindowEvent.Resize resize)
	{
		Console.WriteLine($"RESIZE: {resize.size.width}, {resize.size.height}");
	}
	if (e is WindowEvent.Move move)
	{
		Console.WriteLine($"MOVE:   {move.position.x}, {move.position.y}");
	}
	if (e is WindowEvent.MouseMove mouseMove)
	{
		Console.WriteLine($"MOUSE MOVE: {mouseMove.delta.x}, {mouseMove.delta.y}");
	}
	if (e is WindowEvent.MouseScroll scroll)
	{
		Console.WriteLine($"SCROLL: {scroll.delta.x}, {scroll.delta.y}");
	}
	if (e is WindowEvent.GainedFocus)
	{
		Console.WriteLine($"GAINED FOCUS");
	}
	if (e is WindowEvent.LostFocus)
	{
		Console.WriteLine($"LOST FOCUS");
	}

	if (e is WindowEvent.Activated)
	{
		Console.WriteLine($"ACTIVATED");
	}
	if (e is WindowEvent.Deactivated)
	{
		Console.WriteLine($"DEACTIVATED");
	}

	if (e is WindowEvent.Enabled)
	{
		Console.WriteLine($"ENABLED");
	}
	if (e is WindowEvent.Disabled)
	{
		Console.WriteLine($"DISABLED");
	}

	if (e is WindowEvent.ChangeState)
	{
		Console.WriteLine($"STATE CHANGED");
	}

	if (e is WindowEvent.Create)
	{
		Console.WriteLine($"CREATE");
	}
	if (e is WindowEvent.Close)
	{
		Console.WriteLine($"CLOSE");
	}
	if (e is WindowEvent.Destroy)
	{
		Console.WriteLine($"DESTROY");
	}

	if (e is WindowEvent.CursorEnter cursorEnter)
	{
		Console.WriteLine($"ENTER: {cursorEnter.position.x}, {cursorEnter.position.y}");
	}
	if (e is WindowEvent.CursorMove cursorMove)
	{
		Console.WriteLine($"MOVE: {cursorMove.position.x}, {cursorMove.position.y}");
	}
	if (e is WindowEvent.CursorLeave cursorLeave)
	{
		Console.WriteLine($"LEAVE: {cursorLeave.position.x}, {cursorLeave.position.y}");
	}
};
using var window = new Window(options);
var msg = new Frost.Thread.Message();
while (window.State != WindowState.Hidden)
{
	msg.Wait();
	msg.Dispatch();
}
