namespace Frost.Interoperability;

/// <summary>
/// Frost.Api resource encapsulation
/// </summary>
public class FrostObject : IFrostObject
{
    private nint _handle;

    /// <summary>
    /// Consturct null resource
    /// </summary>
    protected FrostObject()
    {
        _handle = nint.Zero;
	}

    /// <summary>
    /// Constructs a resource from handle
    /// </summary>
    /// <param name="handle">Raw handle to Frost.Api resource</param>
    protected FrostObject(nint handle)
    {
        if (handle != nint.Zero)
        {
            FrostApi.Object.AcquireReference(handle);
            _handle = handle;
		}
	}



    /// <summary>
    /// Returns true if handle to api resource is valid
    /// </summary>
    public bool Valid => Handle != nint.Zero;
        
    public nint Handle
    {
        get => _handle;
        protected set
        {
            if (_handle != nint.Zero)
                FrostApi.Object.ReleaseReference(_handle);
            if (value != nint.Zero)
                FrostApi.Object.AcquireReference(value);

            _handle = value;
        }
    }
        
    public ulong ReferenceCount =>
        Handle != nint.Zero ?
            FrostApi.Object.GetRefCount(Handle) :
            0;



    ~FrostObject()
    {
        if (_handle != nint.Zero)
            FrostApi.Object.ReleaseReference(_handle);
    }

    public void Dispose()
    {
        GC.SuppressFinalize(this);
        if (_handle != nint.Zero)
            FrostApi.Object.ReleaseReference(_handle);
	}
}
