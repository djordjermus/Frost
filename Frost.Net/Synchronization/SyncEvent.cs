using Frost.Net.Interoperability;
using Frost.Net.Synchronization.Interface;

namespace Frost.Net.Synchronization
{
    public class SyncEvent : FrostResource, ISynchronizableEvent
	{
        public SyncEvent() :
			base(FrostApi.Synchronizable.CreateEvent())
		{ }

        public bool Lock() => FrostApi.Synchronizable.Lock(Handle);
		public bool TryLock() => FrostApi.Synchronizable.TryLock(Handle);
		public bool Unlock() => FrostApi.Synchronizable.Unlock(Handle);
		public bool Reset() => FrostApi.Synchronizable.ResetEvent(Handle);
	}
}
