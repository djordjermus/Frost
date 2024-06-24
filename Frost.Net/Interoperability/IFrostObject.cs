namespace Frost.Interoperability;

/// <summary>
/// Defines a Frost.Api resource methods
/// </summary>
public interface IFrostObject : IDisposable
{
		/// <summary>
		/// Returns raw handle of the api resource
		/// </summary>
		public nint Handle { get; }

    /// <summary>
    /// Returns reference count of the api resource
    /// </summary>
    public ulong ReferenceCount { get; }
}
