namespace Frost;

public delegate void Procedure
	();

public delegate void Procedure<T>
	(T arg);

public delegate void Procedure<T1, T2>
	(T1 arg1, T2 arg2);

public delegate void Procedure<T1, T2, T3>
	(T1 arg1, T2 arg2, T3 arg3);

public delegate void Procedure<T1, T2, T3, T4>
	(T1 arg1, T2 arg2, T3 arg3, T4 arg4);

public delegate void Procedure<T1, T2, T3, T4, T5>
	(T1 arg1, T2 arg2, T3 arg3, T4 arg4, T5 arg5);

public delegate void Procedure<T1, T2, T3, T4, T5, T6>
	(T1 arg1, T2 arg2, T3 arg3, T4 arg4, T5 arg5, T6 arg6);

public delegate void Procedure<T1, T2, T3, T4, T5, T6, T7>
	(T1 arg1, T2 arg2, T3 arg3, T4 arg4, T5 arg5, T6 arg6, T7 arg7);

public delegate void Procedure<T1, T2, T3, T4, T5, T6, T7, T8>
	(T1 arg1, T2 arg2, T3 arg3, T4 arg4, T5 arg5, T6 arg6, T7 arg7, T8 arg8);



public delegate void RefProcedure<T1>
	(ref T1 arg1);

public delegate void RefProcedure<T1, T2>
	(ref T1 arg1, ref T2 arg2);

public delegate void RefProcedure<T1, T2, T3>
	(ref T1 arg1, ref T2 arg2, ref T3 arg3);

public delegate void RefProcedure<T1, T2, T3, T4>
	(ref T1 arg1, ref T2 arg2, ref T3 arg3, ref T4 arg4);

public delegate void RefProcedure<T1, T2, T3, T4, T5>
	(ref T1 arg1, ref T2 arg2, ref T3 arg3, ref T4 arg4, ref T5 arg5);

public delegate void RefProcedure<T1, T2, T3, T4, T5, T6>
	(ref T1 arg1, ref T2 arg2, ref T3 arg3, ref T4 arg4, ref T5 arg5, ref T6 arg6);

public delegate void RefProcedure<T1, T2, T3, T4, T5, T6, T7>
	(ref T1 arg1, ref T2 arg2, ref T3 arg3, ref T4 arg4, ref T5 arg5, ref T6 arg6, ref T7 arg7);

public delegate void RefProcedure<T1, T2, T3, T4, T5, T6, T7, T8>
	(ref T1 arg1, ref T2 arg2, ref T3 arg3, ref T4 arg4, ref T5 arg5, ref T6 arg6, ref T7 arg7, ref T8 arg8);
