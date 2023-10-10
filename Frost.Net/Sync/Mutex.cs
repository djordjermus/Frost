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
				EntryPoint = "?create@mutex@api@frost@@SAPEAV123@_N@Z")]
			public static extern IntPtr CreateMutex(bool autoAcquire);

			[MethodImpl(MethodImplOptions.AggressiveInlining)]
			[DllImport(
				dllName: Settings.frostApiPath,
				CallingConvention = CallingConvention.StdCall,
				EntryPoint = "?acquire@mutex@api@frost@@SA_NPEAV123@@Z")]
			public static extern bool AcquireMutex(IntPtr pImpl);

			[MethodImpl(MethodImplOptions.AggressiveInlining)]
			[DllImport(
				dllName: Settings.frostApiPath,
				CallingConvention = CallingConvention.StdCall,
				EntryPoint = "?try_acquire@mutex@api@frost@@SA_NPEAV123@@Z")]
			public static extern bool TryAcquireMutex(IntPtr pImpl);

			[MethodImpl(MethodImplOptions.AggressiveInlining)]
			[DllImport(
				dllName: Settings.frostApiPath,
				CallingConvention = CallingConvention.StdCall,
				EntryPoint = "?release@mutex@api@frost@@SA_NPEAV123@@Z")]
			public static extern bool ReleaseMutex(IntPtr pImpl);

			[MethodImpl(MethodImplOptions.AggressiveInlining)]
			[DllImport(
				dllName: Settings.frostApiPath,
				CallingConvention = CallingConvention.StdCall,
				EntryPoint = "?get_sync_object@mutex@api@frost@@SAPEAVsync_object@23@PEAV123@@Z")]
			public static extern IntPtr GetSyncObjectFromMutex(IntPtr pImpl);

			[MethodImpl(MethodImplOptions.AggressiveInlining)]
			[DllImport(
				dllName: Settings.frostApiPath,
				CallingConvention = CallingConvention.StdCall,
				EntryPoint = "?destroy@mutex@api@frost@@SA_NPEAV123@@Z")]
			public static extern bool DestroyMutex(IntPtr pImpl);
		}
	}
}
