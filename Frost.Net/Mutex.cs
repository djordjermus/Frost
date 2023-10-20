using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;

namespace Frost.Net
{
	public class Mutex : FrostObject, ISynchronizable
	{
		public Mutex(bool initialOwner) :
			base(Interop.Create(initialOwner)) { }

		IntPtr ISynchronizable.Handle { get => Handle; }
		public bool Lock() => ISynchronizable.Interop.Lock(Handle);
		public bool TryLock() => ISynchronizable.Interop.TryLock(Handle);
		public bool Unlock() => ISynchronizable.Interop.Unlock(Handle);

		internal new static class Interop
		{
			[MethodImpl(MethodImplOptions.AggressiveInlining)]
			[DllImport(
				dllName: Settings.frostApiPath,
				CallingConvention = CallingConvention.StdCall,
				EntryPoint = "?create@mutex@api@frost@@SAPEAV123@_N@Z")]
			public static extern IntPtr Create(bool initialOwner);
		}
	}
}
