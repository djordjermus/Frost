using Frost.Interoperability;

namespace Frost.Synchronization;

public class Semaphore : SynchronizationResource
{
    public Semaphore(int count, int max) :
        base(FrostApi.Synchronizable.CreateSemaphore(count, max)) { }
}
