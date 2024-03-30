namespace Frost.Net.Models;

public class WindowOptions
{
	public int x = 200;
	public int y = 200;
	public int width = 1280;
	public int height = 720;
	public Procedure<WindowEvent> procedure = e => { };
	public WindowState state = WindowState.Normal;
}
