using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;

namespace Frost.Net
{
	public class FrostObject
	{
		private IntPtr _handle;
        protected FrostObject() => _handle = IntPtr.Zero;
		protected FrostObject(IntPtr handle)
		{
			if (handle != IntPtr.Zero)
			{
				Interop.AcquireReference(handle);
				_handle = handle;
			}
		}



		public IntPtr Handle
		{
			get => _handle;
			protected set
			{
				if (_handle != IntPtr.Zero)
					Interop.ReleaseReference(_handle);
				if (value != IntPtr.Zero)
					Interop.AcquireReference(value);

				_handle = value;
			}
		}

		public ulong ReferenceCount => 
			Handle != IntPtr.Zero ?
				Interop.GetRefCount(Handle) : 
				0;

		~FrostObject()
		{
			if (_handle != IntPtr.Zero)
				Interop.ReleaseReference(_handle);
		}


		static internal class Interop
		{
			[MethodImpl(MethodImplOptions.AggressiveInlining)]
			[DllImport(
				dllName: Settings.frostApiPath,
				CallingConvention = CallingConvention.StdCall,
				EntryPoint = "?get_reference_count@object@api@frost@@SA_KPEAV123@@Z")]
			public static extern ulong GetRefCount(IntPtr ptr);

			[MethodImpl(MethodImplOptions.AggressiveInlining)]
			[DllImport(
				dllName: Settings.frostApiPath,
				CallingConvention = CallingConvention.StdCall,
				EntryPoint = "?acquire_reference@object@api@frost@@SAXPEAV123@@Z")]
			public static extern void AcquireReference(IntPtr ptr);

			[MethodImpl(MethodImplOptions.AggressiveInlining)]
			[DllImport(
				dllName: Settings.frostApiPath,
				CallingConvention = CallingConvention.StdCall,
				EntryPoint = "?release_reference@object@api@frost@@SAXPEAV123@@Z")]
			public static extern void ReleaseReference(IntPtr ptr);
		}
	}
}
