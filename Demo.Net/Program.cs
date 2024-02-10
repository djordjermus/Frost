var mainThread = Frost.Net.Thread.ThisThread;
var e = new Frost.Net.Synchronization.SyncEvent();
var th = new Frost.Net.Thread(() => {
	e.Wait();
	int i = 0;
	while (true)
	{
		i++;
		Frost.Net.Thread.Message.Send(mainThread, () => { Console.WriteLine($"Hello, World!"); });
	}
});
var message = new Frost.Net.Thread.Message();
e.Signal();
while (true)
{
	message.Wait();
	message.Dispatch();
}

