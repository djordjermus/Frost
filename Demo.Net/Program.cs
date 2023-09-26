using Frost.Net;

double start = Clock.Timestamp;
var random = new Frost.Net.Random();
while (true)
{
	Console.WriteLine("next          : " + random.NextI64());
	Console.WriteLine("next in range : " + random.NextI64(0, 5));
	Console.WriteLine("next double   : " + random.NextF64());
	Thread.Sleep(10);
}
