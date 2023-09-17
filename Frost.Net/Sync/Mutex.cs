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
				EntryPoint = "sync_create_mutex")]
			public static extern IntPtr CreateMutex(bool autoAcquire);

			[MethodImpl(MethodImplOptions.AggressiveInlining)]
			[DllImport(
				dllName: Settings.frostApiPath,
				CallingConvention = CallingConvention.StdCall,
				EntryPoint = "sync_acquire_mutex")]
			public static extern bool AcquireMutex(IntPtr pImpl);

			[MethodImpl(MethodImplOptions.AggressiveInlining)]
			[DllImport(
				dllName: Settings.frostApiPath,
				CallingConvention = CallingConvention.StdCall,
				EntryPoint = "sync_try_acquire_mutex")]
			public static extern bool TryAcquireMutex(IntPtr pImpl);

			[MethodImpl(MethodImplOptions.AggressiveInlining)]
			[DllImport(
				dllName: Settings.frostApiPath,
				CallingConvention = CallingConvention.StdCall,
				EntryPoint = "sync_release_mutex")]
			public static extern bool ReleaseMutex(IntPtr pImpl);

			[MethodImpl(MethodImplOptions.AggressiveInlining)]
			[DllImport(
				dllName: Settings.frostApiPath,
				CallingConvention = CallingConvention.StdCall,
				EntryPoint = "sync_get_sync_object_from_mutex")]
			public static extern IntPtr GetSyncObjectFromMutex(IntPtr pImpl);

			[MethodImpl(MethodImplOptions.AggressiveInlining)]
			[DllImport(
				dllName: Settings.frostApiPath,
				CallingConvention = CallingConvention.StdCall,
				EntryPoint = "sync_destroy_mutex")]
			public static extern bool DestroyMutex(IntPtr pImpl);
		}
	}
}
