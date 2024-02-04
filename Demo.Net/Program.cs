using Frost.Net;
var mainThread = Frost.Net.Thread.ThisThread;
var e = new Frost.Net.Synchronization.SyncEvent();
var th = new Frost.Net.Thread(() => {
	e.Wait();
	while(true)
		Frost.Net.Thread.Message.Send(mainThread, () => { Console.WriteLine("Hello, World!"); });
});
var message = new Frost.Net.Thread.Message();
e.Signal();
while (true)
{
	message.Wait();
	message.Dispatch();
}

