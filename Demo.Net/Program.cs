using Frost.Net.Sync;
Frost.Net.Random r = new Frost.Net.Random();
while (true)
{
	Console.WriteLine(r.NextF64(5, 5));
	Thread.Sleep(10);
}

