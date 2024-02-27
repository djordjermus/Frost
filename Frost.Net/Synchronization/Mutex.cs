using Frost.Interoperability;
using Frost.Synchronization.Interface;

namespace Frost.Synchronization;

public class Mutex : FrostResource, ISynchronizable
{
    public Mutex(bool initialOwner) :
        base(FrostApi.Synchronizable.CreateMutex(initialOwner))
    { }

    public bool Wait() => FrostApi.Synchronizable.Wait(Handle);
    public bool TryWait() => FrostApi.Synchronizable.TryWait(Handle);
    public bool Signal() => FrostApi.Synchronizable.Signal(Handle);
}
