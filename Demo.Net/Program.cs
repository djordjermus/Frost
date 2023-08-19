// See https://aka.ms/new-console-template for more information
using Frost.Net;
var offset = Clock.Timestamp;
Frost.Net.Random r = new Frost.Net.Random();
while (true)
{
	var d = r.NextF64(10, 5);
	Console.WriteLine(d.ToString("0.0000"));
	Thread.Sleep(100);
}
