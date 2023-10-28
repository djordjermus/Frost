namespace Frost.Net
{
	/// <summary>
	/// 
	/// </summary>
	public interface IFrostResource : IDisposable
	{
		public IntPtr Handle { get; }
		public ulong ReferenceCount { get; }
	}
}
