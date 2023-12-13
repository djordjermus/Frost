namespace Frost.Net.Synchronization;

using Frost.Net.Interoperability;
using Frost.Net.Synchronization.Interface;

public static class MultiSync
{
	public static int LockOne(ReadOnlySpan<ISynchronizable> syncObjects)
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
	public static bool LockAll(ReadOnlySpan<ISynchronizable> syncObjects)
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
	public static int TryLockOne(ReadOnlySpan<ISynchronizable> syncObjects)
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
	public static bool TryLockAll(ReadOnlySpan<ISynchronizable> syncObjects)
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
}