namespace Frost.Net.Test
{
	[TestClass]
	public class SyncTest
	{
		[TestMethod]
		public void TestSemaphore()
		{
			var sf = Sync.CreateSemaphore(2, 2);
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
			var mx = Sync.CreateMutex(false);
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
			var sf = Sync.CreateSemaphore(2, 4);
			var mx = Sync.CreateMutex(false);

			Assert.AreEqual(Sync.LockAll(sf, mx), true);
			Assert.AreEqual(Sync.LockAll(sf, mx), true);
			Assert.AreEqual(Sync.TryLockAll(sf, mx), false);
			Assert.AreEqual(Sync.LockOne(sf, mx), 1);
			Assert.AreEqual(Sync.TryLockOne(sf, mx), 1);
		}
	}
}
