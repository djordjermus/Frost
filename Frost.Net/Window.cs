using Frost.Interoperability;
using Frost.Synchronization.Interface;
using System.Runtime.InteropServices;

namespace Frost
{
	public sealed class Window : FrostResource, ISynchronizable
	{
		public Window() : base(InternalCreate(out var proc))
		{
			_proc = proc;
		}

		private static IntPtr InternalCreate(out FrostApi.Procedure proc)
		{
			var desc = new FrostApi.Window.Description();
			proc = EmptyProcedure;
			desc.procedure = Marshal.GetFunctionPointerForDelegate(proc);
			unsafe
			{
				var x = Marshal.SizeOf(desc);
				return FrostApi.Window.Create((IntPtr)(&desc));
			}
		}
		private static void EmptyProcedure(IntPtr ptr) { }
		private FrostApi.Procedure _proc;
	}
}
