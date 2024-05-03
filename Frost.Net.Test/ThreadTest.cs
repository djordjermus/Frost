namespace Frost.Test;
[TestClass]
public class ThreadTest
{
    private readonly Frost.Synchronization.SyncEvent sync = new Frost.Synchronization.SyncEvent();
    private int counter = 0;
    [TestMethod]
    public void ThreadMessagingTest()
    {
        var thread = new Thread(() =>
        {
            using var msg = new Frost.Thread.Message();
            sync.Signal();
            for (int i = 0; i < 10; i++)
            {
                msg.Wait();
                msg.Dispatch();
            }
        });
        Assert.IsTrue(sync.Wait());
        for (int i = 0; i < 10; i++)
        {
            Assert.IsTrue(Frost.Thread.Message.Send(thread, value => counter = counter + value, i));
        }

        Assert.AreEqual(45, counter);
    }
}