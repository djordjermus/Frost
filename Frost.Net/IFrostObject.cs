namespace Frost.Net
{
	/// <summary>
	/// 
	/// </summary>
	public interface IFrostObject : IDisposable
	{
		public IntPtr Handle { get; }
		public ulong ReferenceCount { get; }
	}
}
