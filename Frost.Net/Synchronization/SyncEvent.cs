using Frost.Net.Interoperability;
using Frost.Net.Synchronization.Interface;

namespace Frost.Net.Synchronization
{
    public class SyncEvent : FrostResource, ISynchronizableEvent
	{
        public SyncEvent() :
			base(FrostApi.Synchronizable.CreateEvent())
		{ }

        public bool Wait() => FrostApi.Synchronizable.Wait(Handle);
		public bool TryWait() => FrostApi.Synchronizable.TryWait(Handle);
		public bool Signal() => FrostApi.Synchronizable.Signal(Handle);
		public bool Reset() => FrostApi.Synchronizable.ResetEvent(Handle);
	}
}
