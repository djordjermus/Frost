using Frost.Net.Interoperability;
using Frost.Net.Synchronization.Interface;

namespace Frost.Net.Synchronization
{
    public class Mutex : FrostResource, ISynchronizable
    {
        public Mutex(bool initialOwner) :
            base(FrostApi.Synchronizable.CreateMutex(initialOwner))
        { }

        public bool Lock() => FrostApi.Synchronizable.Lock(Handle);
        public bool TryLock() => FrostApi.Synchronizable.TryLock(Handle);
        public bool Unlock() => FrostApi.Synchronizable.Unlock(Handle);
    }
}
