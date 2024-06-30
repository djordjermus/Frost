using Frost.Models;
using Frost.Logging;
using Frost.Synchronization;

namespace Frost.Test;

[TestClass]
public class FrostApiEndpointLinkageTest
{
	struct EventData { public int value; }

	[TestMethod]
	public void TestClockApi()
	{
		var frequency = Clock.Frequency;
		var period = Clock.Period;
		var timestamp = Clock.Timestamp;
	}

	[TestMethod]
	public void TestRandomApi()
	{
		var rng = new Frost.Random();
		rng.NextU64();
		rng.NextU64(5, 10);
		rng.NextF64();
	}

	[TestMethod]
	public void TestSyncSemaphoreApi()
	{
		var sf = new Synchronization.Semaphore(2, 2);
		sf.Wait();
		sf.TryWait();
		sf.Signal();
	}

	[TestMethod]
	public void TestSyncMutexApi()
	{
		var mx = new Synchronization.Mutex(false);
		mx.Wait();
		mx.TryWait();
		mx.Signal();
	}

	[TestMethod]
	public void TestSyncEventApi()
	{
		var ev = new SyncEvent();
		ev.Signal();
		ev.Wait();
		ev.TryWait();
	}

	[TestMethod]
	public void TestSyncApi()
	{
		var mx = new Synchronization.Mutex(false);
		var sf = new Synchronization.Semaphore(2, 2);

		ReadOnlySpan<SynchronizationResource> span = new SynchronizationResource[2] { sf, mx };
		MultiSync.WaitOne(span);
		MultiSync.WaitAll(span);
		MultiSync.TryWaitOne(span);
		MultiSync.TryWaitAll(span);
	}

	[TestMethod]
	public void TestVersionApi()
	{
		var version = Api.GetApiVersion();
	}

	[TestMethod]
	public void TestColorApi()
	{
		var color = new Color();
		var hdr = new HDRColor();
		color = new(hdr);
		hdr = new(color);

		color = new(new HSVA());
		color = new(new HSLA());
		color = new(new CMYK());

		hdr = new(new HSVA());
		hdr = new(new HSLA());
		hdr = new(new CMYK());

		var hsva = hdr.ToHsla();
		var hsla = hdr.ToHsva();
		var cmyk = hdr.ToCmyk();
	}

	[TestMethod]
	public void TestEventSystem()
	{
		var apiLayer = Layers.InternalLayer;
		EventSystem.Subscribe(0b01, (ref EventData e) => Assert.AreNotEqual(e.value & 0b01, 0b00));
		EventSystem.Subscribe(0b10, (ref EventData e) => Assert.AreNotEqual(e.value & 0b10, 0b00));
		EventSystem.Subscribe(0b11, (ref EventData e) => Assert.AreNotEqual(e.value & 0b11, 0b00));
		EventSystem.Emit(0b01, new EventData() { value = 0b01 });
		EventSystem.Emit(0b10, new EventData() { value = 0b10 });
		EventSystem.Emit(0b11, new EventData() { value = 0b11 });
	}

	[TestMethod]
	public void TestLog()
	{
		EventSystem.Subscribe(1, (ref Log l) => Console.WriteLine($"[{l.level}]{l.message}"));
		Log.Emit(1, Log.Level.Trace, "Hello, World!", null);
		Log.Render("{@Greeting}, {@Subject}!", new Dictionary<string, string> { { "Greeting", "Hello" }, { "Subject", "World" } });
	}
}