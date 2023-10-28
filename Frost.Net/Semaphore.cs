using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;

namespace Frost.Net
{
	public class Semaphore : FrostResource, ISynchronizable
	{
		public Semaphore(int count, int max) :
			base(FrostApi.Synchronizable.CreateSemaphore(count, max)) { }

		public bool Lock() => FrostApi.Synchronizable.Lock(Handle);
		public bool TryLock() => FrostApi.Synchronizable.TryLock(Handle);
		public bool Unlock() => FrostApi.Synchronizable.Unlock(Handle);
	}
}
