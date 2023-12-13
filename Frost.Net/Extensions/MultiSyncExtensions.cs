namespace Frost.Net.Extensions;

using Frost.Net.Synchronization;
using Frost.Net.Synchronization.Interface;

public static class MultiSyncExtensions
{
	public static int LockOneExtension(this ReadOnlySpan<ISynchronizable> syncObjects) => MultiSync.LockOne(syncObjects);
	public static bool LockAllExtension(this ReadOnlySpan<ISynchronizable> syncObjects) => MultiSync.LockAll(syncObjects);
	public static int TryLockOneExtension(this ReadOnlySpan<ISynchronizable> syncObjects) => MultiSync.TryLockOne(syncObjects);
	public static bool TryLockAllExtension(this ReadOnlySpan<ISynchronizable> syncObjects) => MultiSync.TryLockAll(syncObjects);

	public static int LockOneExtension(this ISynchronizable[] syncObjects) => MultiSync.LockOne(syncObjects);
	public static bool LockAllExtension(this ISynchronizable[] syncObjects) => MultiSync.LockAll(syncObjects);
	public static int TryLockOneExtension(this ISynchronizable[] syncObjects) => MultiSync.TryLockOne(syncObjects);
	public static bool TryLockAllExtension(this ISynchronizable[] syncObjects) => MultiSync.TryLockAll(syncObjects);
}
