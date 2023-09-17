using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;

namespace Frost.Net.Sync
{
	public interface SyncObject
	{
		protected IntPtr GetSyncObject();



		public static int AcquireOne(params SyncObject[] syncObjects)
		{
			unsafe
			{
				Span<IntPtr> arr = stackalloc IntPtr[syncObjects.Length];
				for (int i = 0; i < arr.Length; i++)
					arr[i] = syncObjects[i].GetSyncObject();

				fixed (IntPtr* ptr = arr)
					return Interop.AcquireOne((IntPtr)ptr, arr.Length);
			}
		}

		public static bool AcquireAll(params SyncObject[] syncObjects)
		{
			unsafe
			{
				Span<IntPtr> arr = stackalloc IntPtr[syncObjects.Length];
				
				for (int i = 0; i < arr.Length; i++)
					arr[i] = syncObjects[i].GetSyncObject();

				fixed (IntPtr* ptr = arr)
					return Interop.AcquireAll((IntPtr)ptr, arr.Length);
			}
		}

		public static int TryAcquireOne(params SyncObject[] syncObjects)
		{
			unsafe
			{
				Span<IntPtr> arr = stackalloc IntPtr[syncObjects.Length];
				for (int i = 0; i < arr.Length; i++)
					arr[i] = syncObjects[i].GetSyncObject();

				fixed (IntPtr* ptr = arr)
					return Interop.TryAcquireOne((IntPtr)ptr, arr.Length);
			}
		}

		public static bool TryAcquireAll(params SyncObject[] syncObjects)
		{
			unsafe
			{
				Span<IntPtr> arr = stackalloc IntPtr[syncObjects.Length];
				for (int i = 0; i < arr.Length; i++)
					arr[i] = syncObjects[i].GetSyncObject();

				fixed (IntPtr* ptr = arr)
					return Interop.TryAcquireAll((IntPtr)ptr, arr.Length);
			}
		}



		internal static class Interop
		{
			[MethodImpl(MethodImplOptions.AggressiveInlining)]
			[DllImport(
				dllName: Settings.frostApiPath,
				CallingConvention = CallingConvention.StdCall,
				EntryPoint = "sync_acquire_one")]
			public static extern int AcquireOne(IntPtr pArray, int count);

			[MethodImpl(MethodImplOptions.AggressiveInlining)]
			[DllImport(
				dllName: Settings.frostApiPath,
				CallingConvention = CallingConvention.StdCall,
				EntryPoint = "sync_acquire_all")]
			public static extern bool AcquireAll(IntPtr pArray, int count);

			[MethodImpl(MethodImplOptions.AggressiveInlining)]
			[DllImport(
				dllName: Settings.frostApiPath,
				CallingConvention = CallingConvention.StdCall,
				EntryPoint = "sync_try_acquire_one")]
			public static extern int TryAcquireOne(IntPtr pArray, int count);

			[MethodImpl(MethodImplOptions.AggressiveInlining)]
			[DllImport(
				dllName: Settings.frostApiPath,
				CallingConvention = CallingConvention.StdCall,
				EntryPoint = "sync_try_acquire_all")]
			public static extern bool TryAcquireAll(IntPtr pArray, int count);
		}
	}
}
