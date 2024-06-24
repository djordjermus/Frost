using System.Runtime.InteropServices;

namespace Frost.Net.Interoperability;

public sealed class GCHandleHost
{
    public GCHandle Handle { get; }

    public GCHandleHost(GCHandle handle)
    {
        Handle = handle;
    }

    public static T Alloc<T>(T target, out GCHandleHost host) where T : class
    {
        GCHandle handle = GCHandle.Alloc(target);
        host = new GCHandleHost(handle);
        return target;
    }

    ~GCHandleHost()
    {
        if (Handle.IsAllocated)
            Handle.Free();
    }
}
