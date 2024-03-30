using Frost.Interoperability;

namespace Frost.Synchronization.Interface;

internal interface ISynchronizableEvent : ISynchronizable
{
    public void Reset() => FrostApi.Synchronizable.ResetEvent(Handle);
}
