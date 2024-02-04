﻿using Frost.Net.Interoperability;
using Frost.Net.Synchronization.Interface;
using System.Runtime.InteropServices;

namespace Frost.Net
{
	public class Thread : FrostResource, ISynchronizable
	{
		public Thread(Action procedure) :
			base()
		{
			_procedure = CreateProcedureFromAction(procedure);
			Handle = FrostApi.Thread.Create(_procedure, IntPtr.Zero);
		}

		private Thread(IntPtr handle) :
			base(handle) { }

		public static Thread ThisThread => new Thread(FrostApi.Thread.GetCurrent());

		public bool Wait() => FrostApi.Synchronizable.Wait(Handle);
		public bool TryWait() => FrostApi.Synchronizable.TryWait(Handle);
		public bool Signal() => FrostApi.Synchronizable.Signal(Handle);


		private static FrostApi.Procedure CreateProcedureFromAction(Action action)
		{
			return (IntPtr) => action();
		}

		public class Message : FrostResource
		{
			public Message() :
				base(FrostApi.Thread.CreateMessage()) { }

			public void Wait() => FrostApi.Thread.WaitMessage(Handle);
			public void Peek() => FrostApi.Thread.PeekMessage(Handle);
			public void Dispatch() => FrostApi.Thread.DispatchMessage(Handle);
			public void Discard() => FrostApi.Thread.DiscardMessage(Handle);

			public static bool Send(Thread target, Action action)
			{
				FrostApi.Procedure proc = ptr => action();
				return FrostApi.Thread.SendMessage(target.Handle, proc, IntPtr.Zero);
			}
		}

		private readonly FrostApi.Procedure? _procedure;
	}
}
