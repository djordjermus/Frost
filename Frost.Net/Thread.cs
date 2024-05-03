using Frost.Interoperability;
using Frost.Synchronization;

namespace Frost;

public class Thread : SynchronizationResource
{
	private readonly FrostApi.Procedure? _pin = null;
	private Thread() :
		base() { }

	private Thread(IntPtr handle) :
		base(handle) { }

	public Thread(Procedure procedure) :
		base()
	{
		_pin = new FrostApi.Procedure(ptr => procedure());

		unsafe
		{
			Handle = FrostApi.Thread.Create(_pin, IntPtr.Zero);
		}
	}



	public static Thread ThisThread => new Thread(FrostApi.Thread.GetCurrent());

	public class Message : FrostResource
	{

		public Message() :
			base(FrostApi.Thread.CreateMessage())
		{ }

		public void Wait() => FrostApi.Thread.WaitMessage(Handle);
		public void Peek() => FrostApi.Thread.PeekMessage(Handle);
		public void Dispatch() => FrostApi.Thread.DispatchMessage(Handle);
		public void Discard() => FrostApi.Thread.DiscardMessage(Handle);

#pragma warning disable CS8500
		public static bool Send(
			Thread target,
			Procedure procedure)
		{
			unsafe
			{
				var dispatcher = new ProcedureWrapper(
					procedure);

				var result =
					FrostApi.Thread.SendMessage(
						target.Handle,
						ProcedureWrapper.Dispatch,
						(IntPtr)(&dispatcher));
				return result;
			}
		}


		public static bool Send<T1>(
			Thread target,
			Procedure<T1> procedure,
			T1 argument1)
		{
			unsafe
			{
				var dispatcher = new ProcedureWrapper<T1>(
					procedure,
					argument1);

				var result =
					FrostApi.Thread.SendMessage(
						target.Handle,
						ProcedureWrapper<T1>.Dispatch,
						(IntPtr)(&dispatcher));
				return result;
			}
		}

		public static bool Send<T1, T2>(
			Thread target,
			Procedure<T1, T2> procedure,
			T1 argument1,
			T2 argument2)
		{
			unsafe
			{
				var dispatcher = new ProcedureWrapper<T1, T2>(
					procedure,
					argument1,
					argument2);

				var result =
					FrostApi.Thread.SendMessage(
						target.Handle,
						ProcedureWrapper<T1, T2>.Dispatch,
						(IntPtr)(&dispatcher));
				return result;
			}
		}

		public static bool Send<T1, T2, T3>(
			Thread target,
			Procedure<T1, T2, T3> procedure,
			T1 argument1,
			T2 argument2,
			T3 argument3)
		{
			unsafe
			{
				var dispatcher = new ProcedureWrapper<T1, T2, T3>(
					procedure,
					argument1,
					argument2,
					argument3);

				var result =
					FrostApi.Thread.SendMessage(
						target.Handle,
						ProcedureWrapper<T1, T2, T3>.Dispatch,
						(IntPtr)(&dispatcher));
				return result;
			}
		}

		public static bool Send<T1, T2, T3, T4>(
			Thread target,
			Procedure<T1, T2, T3, T4> procedure,
			T1 argument1,
			T2 argument2,
			T3 argument3,
			T4 argument4)
		{
			unsafe
			{
				var dispatcher = new ProcedureWrapper<T1, T2, T3, T4>(
					procedure,
					argument1,
					argument2,
					argument3,
					argument4);

				var result =
					FrostApi.Thread.SendMessage(
						target.Handle,
						ProcedureWrapper<T1, T2, T3, T4>.Dispatch,
						(IntPtr)(&dispatcher));
				return result;
			}
		}

		public static bool Send<T1, T2, T3, T4, T5>(
			Thread target,
			Procedure<T1, T2, T3, T4, T5> procedure,
			T1 argument1,
			T2 argument2,
			T3 argument3,
			T4 argument4,
			T5 argument5)
		{
			unsafe
			{
				var dispatcher = new ProcedureWrapper<T1, T2, T3, T4, T5>(
					procedure,
					argument1,
					argument2,
					argument3,
					argument4,
					argument5);

				var result =
					FrostApi.Thread.SendMessage(
						target.Handle,
						ProcedureWrapper<T1, T2, T3, T4, T5>.Dispatch,
						(IntPtr)(&dispatcher));
				return result;
			}
		}

		public static bool Send<T1, T2, T3, T4, T5, T6>(
			Thread target,
			Procedure<T1, T2, T3, T4, T5, T6> procedure,
			T1 argument1,
			T2 argument2,
			T3 argument3,
			T4 argument4,
			T5 argument5,
			T6 argument6)
		{
			unsafe
			{
				var dispatcher = new ProcedureWrapper<T1, T2, T3, T4, T5, T6>(
					procedure,
					argument1,
					argument2,
					argument3,
					argument4,
					argument5,
					argument6);

				var result =
					FrostApi.Thread.SendMessage(
						target.Handle,
						ProcedureWrapper<T1, T2, T3, T4, T5, T6>.Dispatch,
						(IntPtr)(&dispatcher));
				return result;
			}
		}

		public static bool Send<T1, T2, T3, T4, T5, T6, T7>(
			Thread target,
			Procedure<T1, T2, T3, T4, T5, T6, T7> procedure,
			T1 argument1,
			T2 argument2,
			T3 argument3,
			T4 argument4,
			T5 argument5,
			T6 argument6,
			T7 argument7)
		{
			unsafe
			{
				var dispatcher = new ProcedureWrapper<T1, T2, T3, T4, T5, T6, T7>(
					procedure,
					argument1,
					argument2,
					argument3,
					argument4,
					argument5,
					argument6,
					argument7);

				var result =
					FrostApi.Thread.SendMessage(
						target.Handle,
						ProcedureWrapper<T1, T2, T3, T4, T5, T6, T7>.Dispatch,
						(IntPtr)(&dispatcher));
				return result;
			}
		}

		public static bool Send<T1, T2, T3, T4, T5, T6, T7, T8>(
			Thread target,
			Procedure<T1, T2, T3, T4, T5, T6, T7, T8> procedure,
			T1 argument1,
			T2 argument2,
			T3 argument3,
			T4 argument4,
			T5 argument5,
			T6 argument6,
			T7 argument7,
			T8 argument8)
		{
			unsafe
			{
				var dispatcher = new ProcedureWrapper<T1, T2, T3, T4, T5, T6, T7, T8>(
					procedure,
					argument1,
					argument2,
					argument3,
					argument4,
					argument5,
					argument6,
					argument7,
					argument8);

				var result =
					FrostApi.Thread.SendMessage(
						target.Handle,
						ProcedureWrapper<T1, T2, T3, T4, T5, T6, T7, T8>.Dispatch,
						(IntPtr)(&dispatcher));
				return result;
			}
		}

#pragma warning restore CS8500
	}
}
