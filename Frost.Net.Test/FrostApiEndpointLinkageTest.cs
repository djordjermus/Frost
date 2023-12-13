using Frost.Net.Models;
using Frost.Net.Logging;
using Frost.Net.Synchronization;
using Frost.Net.Synchronization.Interface;

namespace Frost.Net.Test
{
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
			var rng = new Frost.Net.Random();
			rng.NextU64();
			rng.NextU64(5, 10);
			rng.NextF64();
		}

		[TestMethod]
		public void TestSyncSemaphoreApi()
		{
			var sf = new Synchronization.Semaphore(2, 2);
			sf.Lock();
			sf.TryLock();
			sf.Unlock();
		}

		[TestMethod]
		public void TestSyncMutexApi()
		{
			var mx = new Synchronization.Mutex(false);
			mx.Lock();
			mx.TryLock();
			mx.Unlock();
		}

		[TestMethod]
		public void TestSyncEventApi()
		{
			var ev = new SyncEvent();
			ev.Unlock();
			ev.Lock();
			ev.TryLock();
		}

		[TestMethod]
		public void TestSyncApi()
		{
			var mx = new Synchronization.Mutex(false);
			var sf = new Synchronization.Semaphore(2, 2);

			ReadOnlySpan<ISynchronizable> span = new ISynchronizable[2] { sf, mx };
			MultiSync.LockOne(span);
			MultiSync.LockAll(span);
			MultiSync.TryLockOne(span);
			MultiSync.TryLockAll(span);
		}

		[TestMethod]
		public void TestSemanticVersionApi()
		{
			var version = SemanticVersion.GetApiVersion();
			version.CheckCompatibility(version);
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
			Log.Verbose("{1}, {0}!", "World", "Hello");
			Log.Debug("{1}, {0}!", "World", "Hello");
			Log.Info("{1}, {0}!", "World", "Hello");
			Log.Warning("{1}, {0}!", "World", "Hello");
			Log.Error("{1}, {0}!", "World", "Hello");
			Log.Critical("{1}, {0}!", "World", "Hello");
		}
	}
}