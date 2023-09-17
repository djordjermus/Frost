using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;

namespace Frost.Net.Sync
{
	public sealed class Semaphore : SyncObject, IDisposable
	{
		private readonly IntPtr _handle;

		public Semaphore(int count, int maximum)
		{
			_handle = Interop.CreateSemaphore(count, maximum);
		}



		IntPtr SyncObject.GetSyncObject() => Interop.GetSyncObjectFromSemaphore(_handle);
		public bool Acquire() => Interop.AcquireSemaphore(_handle);
		public bool TryAcquire() => Interop.TryAcquireSemaphore(_handle);
		public bool Release() => Interop.ReleaseSemaphore(_handle);
		public bool IsValid() => _handle != IntPtr.Zero;



		public void Dispose()
		{
			Interop.DestroySemaphore(_handle);
			GC.SuppressFinalize(this);
		}

		~Semaphore() => Interop.DestroySemaphore(_handle);



		internal static class Interop
		{
			[MethodImpl(MethodImplOptions.AggressiveInlining)]
			[DllImport(
				dllName: Settings.frostApiPath,
				CallingConvention = CallingConvention.StdCall,
				EntryPoint = "sync_create_semaphore")]
			public static extern IntPtr CreateSemaphore(int count, int maximum);

			[MethodImpl(MethodImplOptions.AggressiveInlining)]
			[DllImport(
				dllName: Settings.frostApiPath,
				CallingConvention = CallingConvention.StdCall,
				EntryPoint = "sync_acquire_semaphore")]
			public static extern bool AcquireSemaphore(IntPtr pImpl);

			[MethodImpl(MethodImplOptions.AggressiveInlining)]
			[DllImport(
				dllName: Settings.frostApiPath,
				CallingConvention = CallingConvention.StdCall,
				EntryPoint = "sync_try_acquire_semaphore")]
			public static extern bool TryAcquireSemaphore(IntPtr pImpl);

			[MethodImpl(MethodImplOptions.AggressiveInlining)]
			[DllImport(
				dllName: Settings.frostApiPath,
				CallingConvention = CallingConvention.StdCall,
				EntryPoint = "sync_release_semaphore")]
			public static extern bool ReleaseSemaphore(IntPtr pImpl);

			[MethodImpl(MethodImplOptions.AggressiveInlining)]
			[DllImport(
				dllName: Settings.frostApiPath,
				CallingConvention = CallingConvention.StdCall,
				EntryPoint = "sync_get_sync_object_from_semaphore")]
			public static extern IntPtr GetSyncObjectFromSemaphore(IntPtr pImpl);

			[MethodImpl(MethodImplOptions.AggressiveInlining)]
			[DllImport(
				dllName: Settings.frostApiPath,
				CallingConvention = CallingConvention.StdCall,
				EntryPoint = "sync_destroy_semaphore")]
			public static extern bool DestroySemaphore(IntPtr pImpl);
		}
	}
}
