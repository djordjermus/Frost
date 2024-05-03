using Frost.Interoperability;

namespace Frost.Synchronization;

public class Mutex : SynchronizationResource
{
    public Mutex(bool initialOwner) :
        base(FrostApi.Synchronizable.CreateMutex(initialOwner)) { }
}
