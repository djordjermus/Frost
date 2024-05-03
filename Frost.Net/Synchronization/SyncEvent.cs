using Frost.Interoperability;

namespace Frost.Synchronization;

public class SyncEvent : SynchronizationResource
{
        public SyncEvent() :
		base(FrostApi.Synchronizable.CreateEvent()) { }
}
