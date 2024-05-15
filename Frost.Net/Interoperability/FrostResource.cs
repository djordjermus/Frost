using System.Runtime.InteropServices;

namespace Frost.Interoperability;

/// <summary>
/// Frost.Api resource encapsulation
/// </summary>
public class FrostResource : IFrostResource
{
    private nint _handle;

    /// <summary>
    /// Consturct null resource
    /// </summary>
    protected FrostResource()
    {
        _handle = nint.Zero;
	}

    /// <summary>
    /// Constructs a resource from handle
    /// </summary>
    /// <param name="handle">Raw handle to Frost.Api resource</param>
    protected FrostResource(nint handle)
    {
        if (handle != nint.Zero)
        {
            FrostApi.Resource.AcquireReference(handle);
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
                FrostApi.Resource.ReleaseReference(_handle);
            if (value != nint.Zero)
                FrostApi.Resource.AcquireReference(value);

            _handle = value;
        }
    }
        
    public ulong ReferenceCount =>
        Handle != nint.Zero ?
            FrostApi.Resource.GetRefCount(Handle) :
            0;



    ~FrostResource()
    {
        if (_handle != nint.Zero)
            FrostApi.Resource.ReleaseReference(_handle);
    }

    public void Dispose()
    {
        GC.SuppressFinalize(this);
        if (_handle != nint.Zero)
            FrostApi.Resource.ReleaseReference(_handle);
	}
}
