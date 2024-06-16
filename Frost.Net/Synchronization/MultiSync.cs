using Frost.Interoperability;

namespace Frost.Synchronization;

public static class MultiSync
{
	public static int WaitOne(ReadOnlySpan<SynchronizationResource> syncObjects)
	{
		unsafe
		{
			Span<nint> arr = stackalloc nint[syncObjects.Length];
			for (int i = 0; i < arr.Length; i++)
				arr[i] = syncObjects[i].Handle;

			fixed (nint* ptr = arr)
				return FrostApi.Synchronizable.WaitOne((nint)ptr, arr.Length);
		}
	}
	public static bool WaitAll(ReadOnlySpan<SynchronizationResource> syncObjects)
	{
		unsafe
		{
			Span<nint> arr = stackalloc nint[syncObjects.Length];

			for (int i = 0; i < arr.Length; i++)
				arr[i] = syncObjects[i].Handle;

			fixed (nint* ptr = arr)
				return FrostApi.Synchronizable.WaitAll((nint)ptr, arr.Length);
		}
	}
	public static int TryWaitOne(ReadOnlySpan<SynchronizationResource> syncObjects)
	{
		unsafe
		{
			Span<nint> arr = stackalloc nint[syncObjects.Length];
			for (int i = 0; i < arr.Length; i++)
				arr[i] = syncObjects[i].Handle;

			fixed (nint* ptr = arr)
				return FrostApi.Synchronizable.TryWaitOne((nint)ptr, arr.Length);
		}
	}
	public static bool TryWaitAll(ReadOnlySpan<SynchronizationResource> syncObjects)
	{
		unsafe
		{
			Span<nint> arr = stackalloc nint[syncObjects.Length];
			for (int i = 0; i < arr.Length; i++)
				arr[i] = syncObjects[i].Handle;

			fixed (nint* ptr = arr)
				return FrostApi.Synchronizable.TryWait((nint)ptr, arr.Length);
		}
	}
}