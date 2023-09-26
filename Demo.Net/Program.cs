using Frost.Net;

double start = Clock.Timestamp;
while (true)
{
	Console.WriteLine(((Clock.Timestamp - start) * Clock.Period).ToString("0.00"));
	Thread.Sleep(10);
}
