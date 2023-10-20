using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;

namespace Frost.Net
{
    public interface ISynchronizable : IFrostObject
	{
        protected IntPtr Handle { get; }
		public bool Lock();
		public bool TryLock();
		public bool Unlock();


		public static int LockOne(params ISynchronizable[] syncObjects)
        {
            unsafe
            {
                Span<IntPtr> arr = stackalloc IntPtr[syncObjects.Length];
                for (int i = 0; i < arr.Length; i++)
                    arr[i] = syncObjects[i].Handle;

                fixed (IntPtr* ptr = arr)
                    return Interop.LockOne((IntPtr)ptr, arr.Length);
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
                    return Interop.LockAll((IntPtr)ptr, arr.Length);
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
                    return Interop.TryLockOne((IntPtr)ptr, arr.Length);
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
                    return Interop.TryLockAll((IntPtr)ptr, arr.Length);
            }
        }



        internal static new class Interop
        {
            [MethodImpl(MethodImplOptions.AggressiveInlining)]
            [DllImport(
                dllName: Settings.frostApiPath,
                CallingConvention = CallingConvention.StdCall,
                EntryPoint = "?lock@synchronizable@api@frost@@SA_NPEAV123@@Z")]
            public static extern bool Lock(IntPtr target);

            [MethodImpl(MethodImplOptions.AggressiveInlining)]
            [DllImport(
                dllName: Settings.frostApiPath,
                CallingConvention = CallingConvention.StdCall,
                EntryPoint = "?try_lock@synchronizable@api@frost@@SA_NPEAV123@@Z")]
            public static extern bool TryLock(IntPtr target);

            [MethodImpl(MethodImplOptions.AggressiveInlining)]
            [DllImport(
                dllName: Settings.frostApiPath,
                CallingConvention = CallingConvention.StdCall,
                EntryPoint = "?unlock@synchronizable@api@frost@@SA_NPEAV123@@Z")]
            public static extern bool Unlock(IntPtr target);

            [MethodImpl(MethodImplOptions.AggressiveInlining)]
            [DllImport(
                dllName: Settings.frostApiPath,
                CallingConvention = CallingConvention.StdCall,
                EntryPoint = "?get_system_handle@sync_object@api@frost@@KAPEAXPEAV123@@Z")]
            public static extern IntPtr GetSystemHandle(IntPtr target);




            [MethodImpl(MethodImplOptions.AggressiveInlining)]
            [DllImport(
                dllName: Settings.frostApiPath,
                CallingConvention = CallingConvention.StdCall,
                EntryPoint = "?lock_one@synchronizable@api@frost@@SAJPEBQEAV123@J@Z")]
            public static extern int LockOne(IntPtr pArray, int count);

            [MethodImpl(MethodImplOptions.AggressiveInlining)]
            [DllImport(
                dllName: Settings.frostApiPath,
                CallingConvention = CallingConvention.StdCall,
                EntryPoint = "?lock_all@synchronizable@api@frost@@SA_NPEBQEAV123@J@Z")]
            public static extern bool LockAll(IntPtr pArray, int count);

            [MethodImpl(MethodImplOptions.AggressiveInlining)]
            [DllImport(
                dllName: Settings.frostApiPath,
                CallingConvention = CallingConvention.StdCall,
                EntryPoint = "?try_lock_one@synchronizable@api@frost@@SAJPEBQEAV123@J@Z")]
            public static extern int TryLockOne(IntPtr pArray, int count);

            [MethodImpl(MethodImplOptions.AggressiveInlining)]
            [DllImport(
                dllName: Settings.frostApiPath,
                CallingConvention = CallingConvention.StdCall,
                EntryPoint = "?try_lock_all@synchronizable@api@frost@@SA_NPEBQEAV123@J@Z")]
            public static extern bool TryLockAll(IntPtr pArray, int count);
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
