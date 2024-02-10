using Frost.Net.Interoperability;

namespace Frost.Net.Synchronization.Interface;

internal interface ISynchronizableEvent : ISynchronizable
{
    public void Reset() => FrostApi.Synchronizable.ResetEvent(Handle);
}
