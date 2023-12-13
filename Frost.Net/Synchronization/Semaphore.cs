using Frost.Net.Interoperability;
using Frost.Net.Synchronization.Interface;

namespace Frost.Net.Synchronization
{
    public class Semaphore : FrostResource, ISynchronizable
    {
        public Semaphore(int count, int max) :
            base(FrostApi.Synchronizable.CreateSemaphore(count, max))
        { }

        public bool Lock() => FrostApi.Synchronizable.Lock(Handle);
        public bool TryLock() => FrostApi.Synchronizable.TryLock(Handle);
        public bool Unlock() => FrostApi.Synchronizable.Unlock(Handle);
    }
}
