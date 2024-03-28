using Frost;
using Frost.Net.Models;


var options = new WindowOptions();
options.procedure = e =>
{
	if (e is WindowEvent.KeyDown keyDown)
		Console.Write(keyDown.text);
};
var window = new Window(options);
var msg = new Frost.Thread.Message();
while (window.State != WindowState.Hidden)
{
	msg.Wait();
	msg.Dispatch();
}
