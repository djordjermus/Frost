using Frost.Net.Interoperability;

namespace Frost.Net.Synchronization.Interface
{
    public interface ISynchronizable : IFrostResource
    {
        public bool Lock() => FrostApi.Synchronizable.Lock(Handle);
        public bool TryLock() => FrostApi.Synchronizable.TryLock(Handle);
        public bool Unlock() => FrostApi.Synchronizable.Unlock(Handle);
    }
}
