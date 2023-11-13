namespace Frost.Net.Test
{
    [TestClass]
	public class FrostApiEndpointLinkageTest
	{
		struct EventData { public int value; }

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
			var sf = new Semaphore(2, 2);
			sf.Lock();
			sf.TryLock();
			sf.Unlock();
		}

		[TestMethod]
		public void TestSyncMutexApi()
		{
			var mx = new Mutex(false);
			mx.Lock();
			mx.TryLock();
			mx.Unlock();
		}

		[TestMethod]
		public void TestSyncApi()
		{
			var mx = new Mutex(false);
			var sf = new Semaphore(2, 2);
			ISynchronizable.LockOne(sf, mx);
			ISynchronizable.LockAll(sf, mx);
			ISynchronizable.TryLockOne(sf, mx);
			ISynchronizable.TryLockAll(sf, mx);
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
			Log.Verbose("{1}, {0}!", "World", "Hello");
			Log.Debug("{1}, {0}!", "World", "Hello");
			Log.Info("{1}, {0}!", "World", "Hello");
			Log.Warning("{1}, {0}!", "World", "Hello");
			Log.Error("{1}, {0}!", "World", "Hello");
			Log.Critical("{1}, {0}!", "World", "Hello");
		}
	}
}