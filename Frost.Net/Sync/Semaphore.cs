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
				EntryPoint = "?create_semaphore@api@semaphore@sync@frost@@SAPEAV?$impl_t@Vsemaphore@sync@frost@@@4@JJ@Z")]
			public static extern IntPtr CreateSemaphore(int count, int maximum);

			[MethodImpl(MethodImplOptions.AggressiveInlining)]
			[DllImport(
				dllName: Settings.frostApiPath,
				CallingConvention = CallingConvention.StdCall,
				EntryPoint = "?acquire_semaphore@api@semaphore@sync@frost@@SA_NPEAV?$impl_t@Vsemaphore@sync@frost@@@4@@Z")]
			public static extern bool AcquireSemaphore(IntPtr pImpl);

			[MethodImpl(MethodImplOptions.AggressiveInlining)]
			[DllImport(
				dllName: Settings.frostApiPath,
				CallingConvention = CallingConvention.StdCall,
				EntryPoint = "?try_acquire_semaphore@api@semaphore@sync@frost@@SA_NPEAV?$impl_t@Vsemaphore@sync@frost@@@4@@Z")]
			public static extern bool TryAcquireSemaphore(IntPtr pImpl);

			[MethodImpl(MethodImplOptions.AggressiveInlining)]
			[DllImport(
				dllName: Settings.frostApiPath,
				CallingConvention = CallingConvention.StdCall,
				EntryPoint = "?release_semaphore@api@semaphore@sync@frost@@SA_NPEAV?$impl_t@Vsemaphore@sync@frost@@@4@@Z")]
			public static extern bool ReleaseSemaphore(IntPtr pImpl);

			[MethodImpl(MethodImplOptions.AggressiveInlining)]
			[DllImport(
				dllName: Settings.frostApiPath,
				CallingConvention = CallingConvention.StdCall,
				EntryPoint = "?get_sync_object_from_semaphore@api@semaphore@sync@frost@@SAPEAV?$impl_t@Vsync_object@sync@frost@@@4@PEAV?$impl_t@Vsemaphore@sync@frost@@@4@@Z")]
			public static extern IntPtr GetSyncObjectFromSemaphore(IntPtr pImpl);

			[MethodImpl(MethodImplOptions.AggressiveInlining)]
			[DllImport(
				dllName: Settings.frostApiPath,
				CallingConvention = CallingConvention.StdCall,
				EntryPoint = "?destroy_semaphore@api@semaphore@sync@frost@@SA_NPEAV?$impl_t@Vsemaphore@sync@frost@@@4@@Z")]
			public static extern bool DestroySemaphore(IntPtr pImpl);
		}
	}
}
