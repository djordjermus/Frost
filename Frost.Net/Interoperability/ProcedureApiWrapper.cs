namespace Frost.Net.Interoperability;

#pragma warning disable CS8500
internal struct ProcedureApiWrapper
{
    private readonly Procedure _procedure;

    public ProcedureApiWrapper(
        Procedure procedure)
    {
        _procedure = procedure;
    }

    public unsafe static void Dispatch(nint pStruct)
    {
        unsafe
        {
            var ptr = (ProcedureApiWrapper*)pStruct;
            ptr->_procedure();
        }
    }
}

internal struct ProcedureApiWrapper<T1>
{
    private readonly Procedure<T1> _procedure;
    private readonly T1 _argument1;

    public ProcedureApiWrapper(
        Procedure<T1> procedure,
        T1 argument1)
    {
        _procedure = procedure;
        _argument1 = argument1;
    }

    public unsafe static void Dispatch(nint pStruct)
    {
        unsafe
        {
            var ptr = (ProcedureApiWrapper<T1>*)pStruct;
            ptr->_procedure(
                ptr->_argument1);
        }
    }
}

internal struct ProcedureApiWrapper<T1, T2>
{
    private readonly Procedure<T1, T2> _procedure;
    private readonly T1 _argument1;
    private readonly T2 _argument2;

    public ProcedureApiWrapper(
        Procedure<T1, T2> procedure,
        T1 argument1,
        T2 argument2)
    {
        _procedure = procedure;
        _argument1 = argument1;
        _argument2 = argument2;
    }

    public unsafe static void Dispatch(nint pStruct)
    {
        unsafe
        {
            var ptr = (ProcedureApiWrapper<T1, T2>*)pStruct;
            ptr->_procedure(
                ptr->_argument1,
                ptr->_argument2);
        }
    }
}

internal struct ProcedureApiWrapper<T1, T2, T3>
{
    private readonly Procedure<T1, T2, T3> _procedure;
    private readonly T1 _argument1;
    private readonly T2 _argument2;
    private readonly T3 _argument3;

    public ProcedureApiWrapper(
        Procedure<T1, T2, T3> procedure,
        T1 argument1,
        T2 argument2,
        T3 argument3)
    {
        _procedure = procedure;
        _argument1 = argument1;
        _argument2 = argument2;
        _argument3 = argument3;
    }

    public unsafe static void Dispatch(nint pStruct)
    {
        unsafe
        {
            var ptr = (ProcedureApiWrapper<T1, T2, T3>*)pStruct;
            ptr->_procedure(
                ptr->_argument1,
                ptr->_argument2,
                ptr->_argument3);
        }
    }
}

internal struct ProcedureApiWrapper<T1, T2, T3, T4>
{
    private readonly Procedure<T1, T2, T3, T4> _procedure;
    private readonly T1 _argument1;
    private readonly T2 _argument2;
    private readonly T3 _argument3;
    private readonly T4 _argument4;

    public ProcedureApiWrapper(
        Procedure<T1, T2, T3, T4> procedure,
        T1 argument1,
        T2 argument2,
        T3 argument3,
        T4 argument4)
    {
        _procedure = procedure;
        _argument1 = argument1;
        _argument2 = argument2;
        _argument3 = argument3;
        _argument4 = argument4;
    }

    public unsafe static void Dispatch(nint pStruct)
    {
        unsafe
        {
            var ptr = (ProcedureApiWrapper<T1, T2, T3, T4>*)pStruct;
            ptr->_procedure(
                ptr->_argument1,
                ptr->_argument2,
                ptr->_argument3,
                ptr->_argument4);
        }
    }
}

internal struct ProcedureApiWrapper<T1, T2, T3, T4, T5>
{
    private readonly Procedure<T1, T2, T3, T4, T5> _procedure;
    private readonly T1 _argument1;
    private readonly T2 _argument2;
    private readonly T3 _argument3;
    private readonly T4 _argument4;
    private readonly T5 _argument5;

    public ProcedureApiWrapper(
        Procedure<T1, T2, T3, T4, T5> procedure,
        T1 argument1,
        T2 argument2,
        T3 argument3,
        T4 argument4,
        T5 argument5)
    {
        _procedure = procedure;
        _argument1 = argument1;
        _argument2 = argument2;
        _argument3 = argument3;
        _argument4 = argument4;
        _argument5 = argument5;
    }

    public unsafe static void Dispatch(nint pStruct)
    {
        unsafe
        {
            var ptr = (ProcedureApiWrapper<T1, T2, T3, T4, T5>*)pStruct;
            ptr->_procedure(
                ptr->_argument1,
                ptr->_argument2,
                ptr->_argument3,
                ptr->_argument4,
                ptr->_argument5);
        }
    }
}

internal struct ProcedureApiWrapper<T1, T2, T3, T4, T5, T6>
{
    private readonly Procedure<T1, T2, T3, T4, T5, T6> _procedure;
    private readonly T1 _argument1;
    private readonly T2 _argument2;
    private readonly T3 _argument3;
    private readonly T4 _argument4;
    private readonly T5 _argument5;
    private readonly T6 _argument6;

    public ProcedureApiWrapper(
        Procedure<T1, T2, T3, T4, T5, T6> procedure,
        T1 argument1,
        T2 argument2,
        T3 argument3,
        T4 argument4,
        T5 argument5,
        T6 argument6)
    {
        _procedure = procedure;
        _argument1 = argument1;
        _argument2 = argument2;
        _argument3 = argument3;
        _argument4 = argument4;
        _argument5 = argument5;
        _argument6 = argument6;
    }

    public unsafe static void Dispatch(nint pStruct)
    {
        unsafe
        {
            var ptr = (ProcedureApiWrapper<T1, T2, T3, T4, T5, T6>*)pStruct;
            ptr->_procedure(
                ptr->_argument1,
                ptr->_argument2,
                ptr->_argument3,
                ptr->_argument4,
                ptr->_argument5,
                ptr->_argument6);
        }
    }
}

internal struct ProcedureApiWrapper<T1, T2, T3, T4, T5, T6, T7>
{
    private readonly Procedure<T1, T2, T3, T4, T5, T6, T7> _procedure;
    private readonly T1 _argument1;
    private readonly T2 _argument2;
    private readonly T3 _argument3;
    private readonly T4 _argument4;
    private readonly T5 _argument5;
    private readonly T6 _argument6;
    private readonly T7 _argument7;

    public ProcedureApiWrapper(
        Procedure<T1, T2, T3, T4, T5, T6, T7> procedure,
        T1 argument1,
        T2 argument2,
        T3 argument3,
        T4 argument4,
        T5 argument5,
        T6 argument6,
        T7 argument7)
    {
        _procedure = procedure;
        _argument1 = argument1;
        _argument2 = argument2;
        _argument3 = argument3;
        _argument4 = argument4;
        _argument5 = argument5;
        _argument6 = argument6;
        _argument7 = argument7;
    }

    public unsafe static void Dispatch(nint pStruct)
    {
        unsafe
        {
            var ptr = (ProcedureApiWrapper<T1, T2, T3, T4, T5, T6, T7>*)pStruct;
            ptr->_procedure(
                ptr->_argument1,
                ptr->_argument2,
                ptr->_argument3,
                ptr->_argument4,
                ptr->_argument5,
                ptr->_argument6,
                ptr->_argument7);
        }
    }
}

internal struct ProcedureApiWrapper<T1, T2, T3, T4, T5, T6, T7, T8>
{
    private readonly Procedure<T1, T2, T3, T4, T5, T6, T7, T8> _procedure;
    private readonly T1 _argument1;
    private readonly T2 _argument2;
    private readonly T3 _argument3;
    private readonly T4 _argument4;
    private readonly T5 _argument5;
    private readonly T6 _argument6;
    private readonly T7 _argument7;
    private readonly T8 _argument8;

    public ProcedureApiWrapper(
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
        _procedure = procedure;
        _argument1 = argument1;
        _argument2 = argument2;
        _argument3 = argument3;
        _argument4 = argument4;
        _argument5 = argument5;
        _argument6 = argument6;
        _argument7 = argument7;
        _argument8 = argument8;
    }

    public unsafe static void Dispatch(nint pStruct)
    {
        unsafe
        {
            var ptr = (ProcedureApiWrapper<T1, T2, T3, T4, T5, T6, T7, T8>*)pStruct;
            ptr->_procedure(
                ptr->_argument1,
                ptr->_argument2,
                ptr->_argument3,
                ptr->_argument4,
                ptr->_argument5,
                ptr->_argument6,
                ptr->_argument7,
                ptr->_argument8);
        }
    }
}

#pragma warning restore CS8500
