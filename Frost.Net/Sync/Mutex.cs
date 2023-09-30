using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;

namespace Frost.Net.Sync
{
	public sealed class Mutex : SyncObject, IDisposable
	{
		private readonly IntPtr _handle;

		public Mutex(bool autoAcquire = false)
		{
			_handle = Interop.CreateMutex(autoAcquire);
		}



		IntPtr SyncObject.GetSyncObject() => Interop.GetSyncObjectFromMutex(_handle);
		public bool Acquire() => Interop.AcquireMutex(_handle);
		public bool TryAcquire() => Interop.TryAcquireMutex(_handle);
		public bool Release() => Interop.ReleaseMutex(_handle);
		public bool IsValid() => _handle != IntPtr.Zero;



		public void Dispose()
		{
			Interop.DestroyMutex(_handle);
			GC.SuppressFinalize(this);
		}

		~Mutex() => Interop.DestroyMutex(_handle);

		

		internal static class Interop
		{
			[MethodImpl(MethodImplOptions.AggressiveInlining)]
			[DllImport(
				dllName: Settings.frostApiPath,
				CallingConvention = CallingConvention.StdCall,
				EntryPoint = "?create_mutex@api@mutex@sync@frost@@SAPEAV?$impl_t@Vmutex@sync@frost@@@4@_N@Z")]
			public static extern IntPtr CreateMutex(bool autoAcquire);

			[MethodImpl(MethodImplOptions.AggressiveInlining)]
			[DllImport(
				dllName: Settings.frostApiPath,
				CallingConvention = CallingConvention.StdCall,
				EntryPoint = "?acquire_mutex@api@mutex@sync@frost@@SA_NPEAV?$impl_t@Vmutex@sync@frost@@@4@@Z")]
			public static extern bool AcquireMutex(IntPtr pImpl);

			[MethodImpl(MethodImplOptions.AggressiveInlining)]
			[DllImport(
				dllName: Settings.frostApiPath,
				CallingConvention = CallingConvention.StdCall,
				EntryPoint = "?try_acquire_mutex@api@mutex@sync@frost@@SA_NPEAV?$impl_t@Vmutex@sync@frost@@@4@@Z")]
			public static extern bool TryAcquireMutex(IntPtr pImpl);

			[MethodImpl(MethodImplOptions.AggressiveInlining)]
			[DllImport(
				dllName: Settings.frostApiPath,
				CallingConvention = CallingConvention.StdCall,
				EntryPoint = "?release_mutex@api@mutex@sync@frost@@SA_NPEAV?$impl_t@Vmutex@sync@frost@@@4@@Z")]
			public static extern bool ReleaseMutex(IntPtr pImpl);

			[MethodImpl(MethodImplOptions.AggressiveInlining)]
			[DllImport(
				dllName: Settings.frostApiPath,
				CallingConvention = CallingConvention.StdCall,
				EntryPoint = "?get_sync_object_from_mutex@api@mutex@sync@frost@@SAPEAV?$impl_t@Vsync_object@sync@frost@@@4@PEAV?$impl_t@Vmutex@sync@frost@@@4@@Z")]
			public static extern IntPtr GetSyncObjectFromMutex(IntPtr pImpl);

			[MethodImpl(MethodImplOptions.AggressiveInlining)]
			[DllImport(
				dllName: Settings.frostApiPath,
				CallingConvention = CallingConvention.StdCall,
				EntryPoint = "?destroy_mutex@api@mutex@sync@frost@@SA_NPEAV?$impl_t@Vmutex@sync@frost@@@4@@Z")]
			public static extern bool DestroyMutex(IntPtr pImpl);
		}
	}
}
