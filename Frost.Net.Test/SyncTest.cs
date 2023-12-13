using Frost.Net.Synchronization;
using Frost.Net.Synchronization.Interface;

namespace Frost.Net.Test
{
    [TestClass]
	public class SyncTest
	{
		[TestMethod]
		public void TestSemaphore()
		{
			var sf = new Synchronization.Semaphore(2, 2);
			Assert.AreEqual(sf.Lock(), true);
			Assert.AreEqual(sf.TryLock(), true);
			Assert.AreEqual(sf.TryLock(), false);
			Assert.AreEqual(sf.Unlock(), true);
			Assert.AreEqual(sf.Unlock(), true);
			Assert.AreEqual(sf.Unlock(), false);
		}

		[TestMethod]
		public void TestMutex()
		{
			var mx = new Synchronization.Mutex(false);
			Assert.AreEqual(mx.Lock(), true);
			Assert.AreEqual(mx.TryLock(), true);
			Assert.AreEqual(mx.TryLock(), true);
			Assert.AreEqual(mx.Unlock(), true);
			Assert.AreEqual(mx.Unlock(), true);
			Assert.AreEqual(mx.Unlock(), true);
			Assert.AreEqual(mx.Unlock(), false);
		}

		[TestMethod]
		public void TestMultipleSync()
		{
			var sf = new Synchronization.Semaphore(2, 4);
			var mx = new Synchronization.Mutex(false);
			ReadOnlySpan<ISynchronizable> span = new ISynchronizable[2] { sf, mx };

			Assert.AreEqual(MultiSync.LockAll(span), true);
			Assert.AreEqual(MultiSync.LockAll(span), true);
			Assert.AreEqual(MultiSync.TryLockAll(span), false);
			Assert.AreEqual(MultiSync.LockOne(span), 1);
			Assert.AreEqual(MultiSync.TryLockOne(span), 1);
		}
	}
}
