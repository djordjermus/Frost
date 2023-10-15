using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;

namespace Frost.Net
{
    public class Sync : FrostObject
    {
        protected Sync(IntPtr handle) :
            base(handle) { }

        public static Sync CreateMutex(bool initialOwner) =>
            new Mutex(Interop.CreateMutex(initialOwner));
        public static Sync CreateSemaphore(int count, int max) =>
			new Semaphore(Interop.CreateSemaphore(count, max));



		public bool Lock() => Interop.Lock(Handle);
		public bool TryLock() => Interop.TryLock(Handle);
		public bool Unlock() => Interop.Unlock(Handle);



		public static int LockOne(params Sync[] syncObjects)
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

        public static bool LockAll(params Sync[] syncObjects)
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

        public static int TryLockOne(params Sync[] syncObjects)
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

        public static bool TryLockAll(params Sync[] syncObjects)
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
                EntryPoint = "?create_mutex@sync_object@api@frost@@SAPEAV123@_N@Z")]
            public static extern IntPtr CreateMutex(bool initialOwner);

            [MethodImpl(MethodImplOptions.AggressiveInlining)]
            [DllImport(
                dllName: Settings.frostApiPath,
                CallingConvention = CallingConvention.StdCall,
                EntryPoint = "?create_semaphore@sync_object@api@frost@@SAPEAV123@JJ@Z")]
            public static extern IntPtr CreateSemaphore(int count, int max);



            [MethodImpl(MethodImplOptions.AggressiveInlining)]
            [DllImport(
                dllName: Settings.frostApiPath,
                CallingConvention = CallingConvention.StdCall,
                EntryPoint = "?lock@sync_object@api@frost@@SA_NPEAV123@@Z")]
            public static extern bool Lock(IntPtr target);

            [MethodImpl(MethodImplOptions.AggressiveInlining)]
            [DllImport(
                dllName: Settings.frostApiPath,
                CallingConvention = CallingConvention.StdCall,
                EntryPoint = "?try_lock@sync_object@api@frost@@SA_NPEAV123@@Z")]
            public static extern bool TryLock(IntPtr target);

            [MethodImpl(MethodImplOptions.AggressiveInlining)]
            [DllImport(
                dllName: Settings.frostApiPath,
                CallingConvention = CallingConvention.StdCall,
                EntryPoint = "?unlock@sync_object@api@frost@@SA_NPEAV123@@Z")]
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
                EntryPoint = "?lock_one@sync_object@api@frost@@SAJPEBQEAV123@J@Z")]
            public static extern int LockOne(IntPtr pArray, int count);

            [MethodImpl(MethodImplOptions.AggressiveInlining)]
            [DllImport(
                dllName: Settings.frostApiPath,
                CallingConvention = CallingConvention.StdCall,
                EntryPoint = "?lock_all@sync_object@api@frost@@SA_NPEBQEAV123@J@Z")]
            public static extern bool LockAll(IntPtr pArray, int count);

            [MethodImpl(MethodImplOptions.AggressiveInlining)]
            [DllImport(
                dllName: Settings.frostApiPath,
                CallingConvention = CallingConvention.StdCall,
                EntryPoint = "?try_lock_one@sync_object@api@frost@@SAJPEBQEAV123@J@Z")]
            public static extern int TryLockOne(IntPtr pArray, int count);

            [MethodImpl(MethodImplOptions.AggressiveInlining)]
            [DllImport(
                dllName: Settings.frostApiPath,
                CallingConvention = CallingConvention.StdCall,
                EntryPoint = "?try_lock_all@sync_object@api@frost@@SA_NPEBQEAV123@J@Z")]
            public static extern bool TryLockAll(IntPtr pArray, int count);
        }



		private sealed class Mutex : Sync
		{
			public Mutex(IntPtr handle) :
                base(handle) { }
		}

		private sealed class Semaphore : Sync
		{
			public Semaphore(IntPtr handle) :
                base(handle) { }
		}
	}
}
