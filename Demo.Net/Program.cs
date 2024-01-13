var thread = new Frost.Net.Thread(() =>
{
	for (int i = 0; i < 10_000; i++)
		Console.WriteLine(i);
});
thread.Wait();