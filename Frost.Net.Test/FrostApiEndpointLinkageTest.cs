using Frost.Net.Sync;

namespace Frost.Net.Test
{
	[TestClass]
	public class FrostApiEndpointLinkageTest
	{
		class EventData { public int value; }

		[TestMethod]
		public void TestClockApi()
		{
			var frequency = Clock.Timestamp;
			var period = Clock.Period;
			var timestamp = Clock.Timestamp;
		}

		[TestMethod]
		public void TestRandomApi()
		{
			var rng = new Frost.Net.Random();
			rng.NextU64();
			rng.NextU64(5, 10);
			rng.NextF64();
		}

		[TestMethod]
		public void TestSyncSemaphoreApi()
		{
			using var sf = new Sync.Semaphore(2, 2);
			sf.Acquire();
			sf.TryAcquire();
			sf.Release();
		}

		[TestMethod]
		public void TestSyncMutexApi()
		{
			using var mx = new Sync.Mutex(false);
			mx.Acquire();
			mx.TryAcquire();
			mx.Release();
		}

		[TestMethod]
		public void TestSyncApi()
		{
			using var mx = new Sync.Mutex(false);
			using var sf = new Sync.Semaphore(2, 2);
			SyncObject.AcquireOne(sf, mx);
			SyncObject.AcquireAll(sf, mx);
			SyncObject.TryAcquireOne(sf, mx);
			SyncObject.TryAcquireAll(sf, mx);
		}

		[TestMethod]
		public void TestSemanticVersionApi()
		{
			var version = Models.SemanticVersion.GetApiVersion();
			version.CheckCompatibility(version);
		}

		[TestMethod]
		public void TestColorApi()
		{
			var color = new Models.Color();
			var hdr = new Models.HDRColor();
			color = new(hdr);
			hdr = new(color);

			color = new(new Models.HSVA());
			color = new(new Models.HSLA());
			color = new(new Models.CMYK());

			hdr = new(new Models.HSVA());
			hdr = new(new Models.HSLA());
			hdr = new(new Models.CMYK());

			var hsva = hdr.ToHsla();
			var hsla = hdr.ToHsva();
			var cmyk = hdr.ToCmyk();
		}

		[TestMethod]
		public void TestEventSystem()
		{
			EventSystem.Subscribe(0b01, (EventData e) => Assert.AreNotEqual(e.value & 0b01, 0b00));
			EventSystem.Subscribe(0b10, (EventData e) => Assert.AreNotEqual(e.value & 0b10, 0b00));
			EventSystem.Subscribe(0b11, (EventData e) => Assert.AreNotEqual(e.value & 0b11, 0b00));
			EventSystem.Emit(0b01, new EventData() { value = 0b01 });
			EventSystem.Emit(0b10, new EventData() { value = 0b10 });
			EventSystem.Emit(0b11, new EventData() { value = 0b11 });
		}

		[TestMethod]
		public void TestLog()
		{
			EventSystem.Subscribe(1, (Log.Event l) => Console.WriteLine(l.Message));
			Log.Verbose(1, "{1}, {0}!", "World", "Hello");
			Log.Debug(1, "{1}, {0}!", "World", "Hello");
			Log.Info(1, "{1}, {0}!", "World", "Hello");
			Log.Warning(1, "{1}, {0}!", "World", "Hello");
			Log.Error(1, "{1}, {0}!", "World", "Hello");
			Log.Critical(1, "{1}, {0}!", "World", "Hello");
		}
	}
}