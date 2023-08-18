// See https://aka.ms/new-console-template for more information
using Frost.Net;
var offset = Clock.Timestamp;
while (true)
{
	Console.WriteLine((Clock.Timestamp - offset) * Clock.Period);
	Thread.Sleep(100);
}
