namespace Frost.Net.Test
{
	[TestClass]
	public class SyncTest
	{
		[TestMethod]
		public void TestSemaphore()
		{
			var sf = new Semaphore(2, 2);
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
			var mx = new Mutex(false);
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
			var sf = new Semaphore(2, 4);
			var mx = new Mutex(false);

			Assert.AreEqual(ISynchronizable.LockAll(sf, mx), true);
			Assert.AreEqual(ISynchronizable.LockAll(sf, mx), true);
			Assert.AreEqual(ISynchronizable.TryLockAll(sf, mx), false);
			Assert.AreEqual(ISynchronizable.LockOne(sf, mx), 1);
			Assert.AreEqual(ISynchronizable.TryLockOne(sf, mx), 1);
		}
	}
}
