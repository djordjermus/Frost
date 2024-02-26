var mainThread = Frost.Net.Thread.ThisThread;
var e = new Frost.Net.Synchronization.SyncEvent();
var th = new Frost.Net.Thread(() => {
	TextClass data = new TextClass() { Text = "Hello World!" };
	TextClassBundle bundle = new TextClassBundle()
	{
		t1 = data,
		t2 = data,
		t3 = data,
		t4 = data,
		t5 = data,
		t6 = data,
		t7 = data,
		t8 = data,
	};
	e.Wait();
	while (true)
	{
		Frost.Net.Thread.Message.Send(mainThread, TextClassBundle.PrintTextStatic8, bundle);
		// Frost.Net.Thread.Message.Send(mainThread, TextClass.PrintTextStatic8, data, data, data, data, data, data, data, data);
	}
});
var message = new Frost.Net.Thread.Message();
e.Signal();
while (true)
{
	message.Wait();
	message.Dispatch();
}

class TextClass
{
	public string Text { get; set; }
	public void PrintText() => Console.WriteLine(Text);
	public static void PrintTextStatic(TextClass data) => Console.WriteLine(data.Text);
	public static void PrintTextStatic8(TextClass a1, TextClass a2, TextClass a3, TextClass a4, TextClass a5, TextClass a6, TextClass a7, TextClass a8)
	{
		PrintTextStatic(a1);
		PrintTextStatic(a2);
		PrintTextStatic(a3);
		PrintTextStatic(a4);
		PrintTextStatic(a5);
		PrintTextStatic(a6);
		PrintTextStatic(a7);
		PrintTextStatic(a8);
	}
}

struct TextStruct
{
	public string Text { get; set; }
	public void PrintText() => Console.WriteLine(Text);
	public static void RefPrint(ref TextStruct data)
	{
		Console.WriteLine(data.Text);
		data.Text = (int.Parse(data.Text) + 1).ToString();
	}
	public static void PrintTextStatic(TextStruct data) => Console.WriteLine(data.Text);
	public static void PrintTextStatic8(TextStruct a1, TextStruct a2, TextStruct a3, TextStruct a4, TextStruct a5, TextStruct a6, TextStruct a7, TextStruct a8)
	{
		PrintTextStatic(a1);
		PrintTextStatic(a2);
		PrintTextStatic(a3);
		PrintTextStatic(a4);
		PrintTextStatic(a5);
		PrintTextStatic(a6);
		PrintTextStatic(a7);
		PrintTextStatic(a8);
	}
}
class TextClassBundle
{
	public TextClass t1, t2, t3, t4, t5, t6, t7, t8;
	public static void PrintTextStatic8(TextClassBundle data) =>
		TextClass.PrintTextStatic8(data.t1, data.t2, data.t3, data.t4, data.t5, data.t6, data.t7, data.t8);
}