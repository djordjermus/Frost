using Frost.Interoperability;
using Frost.Synchronization.Interface;

namespace Frost.Synchronization;

public class Semaphore : FrostResource, ISynchronizable
{
    public Semaphore(int count, int max) :
        base(FrostApi.Synchronizable.CreateSemaphore(count, max))
    { }

    public bool Wait() => FrostApi.Synchronizable.Wait(Handle);
    public bool TryWait() => FrostApi.Synchronizable.TryWait(Handle);
    public bool Signal() => FrostApi.Synchronizable.Signal(Handle);
}
