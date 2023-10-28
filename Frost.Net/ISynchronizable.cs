using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;

namespace Frost.Net
{
    public interface ISynchronizable : IFrostResource
	{
		public bool Lock() => FrostApi.Synchronizable.Lock(Handle);
		public bool TryLock() => FrostApi.Synchronizable.TryLock(Handle);
		public bool Unlock() => FrostApi.Synchronizable.Unlock(Handle);


		public static int LockOne(params ISynchronizable[] syncObjects)
        {
            unsafe
            {
                Span<IntPtr> arr = stackalloc IntPtr[syncObjects.Length];
                for (int i = 0; i < arr.Length; i++)
                    arr[i] = syncObjects[i].Handle;

                fixed (IntPtr* ptr = arr)
                    return FrostApi.Synchronizable.LockOne((IntPtr)ptr, arr.Length);
            }
        }
        public static bool LockAll(params ISynchronizable[] syncObjects)
        {
            unsafe
            {
                Span<IntPtr> arr = stackalloc IntPtr[syncObjects.Length];

                for (int i = 0; i < arr.Length; i++)
                    arr[i] = syncObjects[i].Handle;

                fixed (IntPtr* ptr = arr)
                    return FrostApi.Synchronizable.LockAll((IntPtr)ptr, arr.Length);
            }
        }
        public static int TryLockOne(params ISynchronizable[] syncObjects)
        {
            unsafe
            {
                Span<IntPtr> arr = stackalloc IntPtr[syncObjects.Length];
                for (int i = 0; i < arr.Length; i++)
                    arr[i] = syncObjects[i].Handle;

                fixed (IntPtr* ptr = arr)
                    return FrostApi.Synchronizable.TryLockOne((IntPtr)ptr, arr.Length);
            }
        }
        public static bool TryLockAll(params ISynchronizable[] syncObjects)
        {
            unsafe
            {
                Span<IntPtr> arr = stackalloc IntPtr[syncObjects.Length];
                for (int i = 0; i < arr.Length; i++)
                    arr[i] = syncObjects[i].Handle;

                fixed (IntPtr* ptr = arr)
                    return FrostApi.Synchronizable.TryLockAll((IntPtr)ptr, arr.Length);
            }
        }



        internal static new class Interop
        {
            
        }


        /*
		private sealed class Mutex : ISynchronizable
		{
			public Mutex(IntPtr handle) :
                base(handle) { }
		}

		private sealed class Semaphore : ISynchronizable
		{
			public Semaphore(IntPtr handle) :
                base(handle) { }
		}
         */
	}
}
