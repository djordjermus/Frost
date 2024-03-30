using Frost.Interoperability;

namespace Frost.Synchronization.Interface;

public interface ISynchronizable : IFrostResource
{
    public bool Wait() => FrostApi.Synchronizable.Wait(Handle);
    public bool TryWait() => FrostApi.Synchronizable.TryWait(Handle);
    public bool Signal() => FrostApi.Synchronizable.Signal(Handle);
}
