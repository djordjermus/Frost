using Frost.Net.Interoperability;
using Frost.Net.Synchronization.Interface;

namespace Frost.Net
{
	public class Thread : FrostResource, ISynchronizable
	{
		public Thread(Action procedure) : base()
		{
			_procedure = CreateProcedureFromAction(procedure);
			Handle = FrostApi.Thread.Create(_procedure, IntPtr.Zero);
		}

		public bool Wait() => FrostApi.Synchronizable.Wait(Handle);
		public bool TryWait() => FrostApi.Synchronizable.TryWait(Handle);
		public bool Signal() => FrostApi.Synchronizable.Signal(Handle);


		private static FrostApi.Procedure CreateProcedureFromAction(Action action)
		{
			return (IntPtr) => action();
		}

		private readonly FrostApi.Procedure _procedure;
	}
}
