using Frost.Net.Synchronization;
using Frost.Net.Synchronization.Interface;

namespace Frost.Net.Extensions;

public static class MultiSyncExtensions
{
	public static int WaitOneExtension(this ReadOnlySpan<ISynchronizable> syncObjects) => MultiSync.WaitOne(syncObjects);
	public static bool WaitAllExtension(this ReadOnlySpan<ISynchronizable> syncObjects) => MultiSync.WaitAll(syncObjects);
	public static int TryWaitOneExtension(this ReadOnlySpan<ISynchronizable> syncObjects) => MultiSync.TryWaitOne(syncObjects);
	public static bool TryWaitAllExtension(this ReadOnlySpan<ISynchronizable> syncObjects) => MultiSync.TryWaitAll(syncObjects);

	public static int WaitOneExtension(this ISynchronizable[] syncObjects) => MultiSync.WaitOne(syncObjects);
	public static bool WaitAllExtension(this ISynchronizable[] syncObjects) => MultiSync.WaitAll(syncObjects);
	public static int TryWaitOneExtension(this ISynchronizable[] syncObjects) => MultiSync.TryWaitOne(syncObjects);
	public static bool TryWaitAllExtension(this ISynchronizable[] syncObjects) => MultiSync.TryWaitAll(syncObjects);
}
