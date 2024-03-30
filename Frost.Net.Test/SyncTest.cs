using Frost.Synchronization;
using Frost.Synchronization.Interface;

namespace Frost.Test;

[TestClass]
public class SyncTest
{
	[TestMethod]
	public void TestSemaphore()
	{
		var sf = new Synchronization.Semaphore(2, 2);
		Assert.AreEqual(sf.Wait(), true);
		Assert.AreEqual(sf.TryWait(), true);
		Assert.AreEqual(sf.TryWait(), false);
		Assert.AreEqual(sf.Signal(), true);
		Assert.AreEqual(sf.Signal(), true);
		Assert.AreEqual(sf.Signal(), false);
	}

	[TestMethod]
	public void TestMutex()
	{
		var mx = new Synchronization.Mutex(false);
		Assert.AreEqual(mx.Wait(), true);
		Assert.AreEqual(mx.TryWait(), true);
		Assert.AreEqual(mx.TryWait(), true);
		Assert.AreEqual(mx.Signal(), true);
		Assert.AreEqual(mx.Signal(), true);
		Assert.AreEqual(mx.Signal(), true);
		Assert.AreEqual(mx.Signal(), false);
	}

	[TestMethod]
	public void TestMultipleSync()
	{
		var sf = new Synchronization.Semaphore(2, 4);
		var mx = new Synchronization.Mutex(false);
		ReadOnlySpan<ISynchronizable> span = new ISynchronizable[2] { sf, mx };

		Assert.AreEqual(MultiSync.WaitAll(span), true);
		Assert.AreEqual(MultiSync.WaitAll(span), true);
		Assert.AreEqual(MultiSync.TryWaitAll(span), false);
		Assert.AreEqual(MultiSync.WaitOne(span), 1);
		Assert.AreEqual(MultiSync.TryWaitOne(span), 1);
	}
}
