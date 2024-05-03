using Frost.Interoperability;

namespace Frost.Synchronization;

public static class MultiSync
{
	public static int WaitOne(ReadOnlySpan<SynchronizationResource> syncObjects)
	{
		unsafe
		{
			Span<IntPtr> arr = stackalloc IntPtr[syncObjects.Length];
			for (int i = 0; i < arr.Length; i++)
				arr[i] = syncObjects[i].Handle;

			fixed (IntPtr* ptr = arr)
				return FrostApi.Synchronizable.WaitOne((IntPtr)ptr, arr.Length);
		}
	}
	public static bool WaitAll(ReadOnlySpan<SynchronizationResource> syncObjects)
	{
		unsafe
		{
			Span<IntPtr> arr = stackalloc IntPtr[syncObjects.Length];

			for (int i = 0; i < arr.Length; i++)
				arr[i] = syncObjects[i].Handle;

			fixed (IntPtr* ptr = arr)
				return FrostApi.Synchronizable.WaitAll((IntPtr)ptr, arr.Length);
		}
	}
	public static int TryWaitOne(ReadOnlySpan<SynchronizationResource> syncObjects)
	{
		unsafe
		{
			Span<IntPtr> arr = stackalloc IntPtr[syncObjects.Length];
			for (int i = 0; i < arr.Length; i++)
				arr[i] = syncObjects[i].Handle;

			fixed (IntPtr* ptr = arr)
				return FrostApi.Synchronizable.TryWaitOne((IntPtr)ptr, arr.Length);
		}
	}
	public static bool TryWaitAll(ReadOnlySpan<SynchronizationResource> syncObjects)
	{
		unsafe
		{
			Span<IntPtr> arr = stackalloc IntPtr[syncObjects.Length];
			for (int i = 0; i < arr.Length; i++)
				arr[i] = syncObjects[i].Handle;

			fixed (IntPtr* ptr = arr)
				return FrostApi.Synchronizable.TryWait((IntPtr)ptr, arr.Length);
		}
	}
}