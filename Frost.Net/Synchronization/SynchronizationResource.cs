using Frost.Interoperability;

namespace Frost.Synchronization;

public class SynchronizationResource : FrostObject
{
    protected SynchronizationResource() :
        base() { }

    protected SynchronizationResource(nint handle) :
        base(handle) { }

    public bool Wait() =>
        FrostApi.Synchronizable.Wait(Handle);
    public bool TryWait() =>
        FrostApi.Synchronizable.TryWait(Handle);
    public bool Signal() =>
        FrostApi.Synchronizable.Signal(Handle);
    public bool Reset() =>
        FrostApi.Synchronizable.ResetEvent(Handle);
}