namespace Frost.Interoperability;

/// <summary>
/// Frost.Api resource encapsulation
/// </summary>
public class FrostResource : IFrostResource
{
    private IntPtr _handle;

    /// <summary>
    /// Consturct null resource
    /// </summary>
    protected FrostResource() =>
        _handle = IntPtr.Zero;

    /// <summary>
    /// Constructs a resource from handle
    /// </summary>
    /// <param name="handle">Raw handle to Frost.Api resource</param>
    protected FrostResource(IntPtr handle)
    {
        if (handle != IntPtr.Zero)
        {
            FrostApi.Resource.AcquireReference(handle);
            _handle = handle;
        }
    }



    /// <summary>
    /// Returns true if handle to api resource is valid
    /// </summary>
    public bool Valid => Handle != IntPtr.Zero;
        
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



    ~FrostResource()
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
