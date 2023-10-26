using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;

namespace Frost.Net
{
	public class FrostObject : IFrostObject
	{
		private IntPtr _handle;

        protected FrostObject() =>
			_handle = IntPtr.Zero;
		protected FrostObject(IntPtr handle)
		{
			if (handle != IntPtr.Zero)
			{
				FrostApi.Resource.AcquireReference(handle);
				_handle = handle;
			}
		}

		public IntPtr Handle
		{
			get => _handle;
			protected set
			{
				if (_handle != IntPtr.Zero)
					FrostApi.Resource.ReleaseReference(_handle);
				if (value != IntPtr.Zero)
					FrostApi.Resource.AcquireReference(value);

				_handle = value;
			}
		}
		public ulong ReferenceCount => 
			Handle != IntPtr.Zero ?
				FrostApi.Resource.GetRefCount(Handle) :
				0;
		public bool Valid => Handle != IntPtr.Zero;

		~FrostObject()
		{
			if (_handle != IntPtr.Zero)
				FrostApi.Resource.ReleaseReference(_handle);
		}

		public void Dispose()
		{
			GC.SuppressFinalize(this);
			if (_handle != IntPtr.Zero)
				FrostApi.Resource.ReleaseReference(_handle);
		}
	}
}
