using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;

namespace Frost.Net
{
	public class Semaphore : FrostObject, ISynchronizable
	{
		public Semaphore(int count, int max) :
			base(Interop.Create(count, max))
		{ }

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
				EntryPoint = "?create@semaphore@api@frost@@SAPEAV123@JJ@Z")]
			public static extern IntPtr Create(int count, int max);
		}
	}
}
