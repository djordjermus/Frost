using Frost.Synchronization;
using System.Runtime.CompilerServices;

namespace Frost.Extensions;

public static class MultiSyncExtensions
{
	[MethodImpl(MethodImplOptions.AggressiveInlining)]
	public static int WaitOne(this ReadOnlySpan<SynchronizationResource> syncObjects) => MultiSync.WaitOne(syncObjects);
	[MethodImpl(MethodImplOptions.AggressiveInlining)]
	public static bool WaitAll(this ReadOnlySpan<SynchronizationResource> syncObjects) => MultiSync.WaitAll(syncObjects);
	[MethodImpl(MethodImplOptions.AggressiveInlining)]
	public static int TryWaitOne(this ReadOnlySpan<SynchronizationResource> syncObjects) => MultiSync.TryWaitOne(syncObjects);
	[MethodImpl(MethodImplOptions.AggressiveInlining)]
	public static bool TryWaitAllExtension(this ReadOnlySpan<SynchronizationResource> syncObjects) => MultiSync.TryWaitAll(syncObjects);



	[MethodImpl(MethodImplOptions.AggressiveInlining)]
	public static int WaitOne(this SynchronizationResource[] syncObjects) => MultiSync.WaitOne(syncObjects);
	[MethodImpl(MethodImplOptions.AggressiveInlining)]
	public static bool WaitAll(this SynchronizationResource[] syncObjects) => MultiSync.WaitAll(syncObjects);
	[MethodImpl(MethodImplOptions.AggressiveInlining)]
	public static int TryWaitOne(this SynchronizationResource[] syncObjects) => MultiSync.TryWaitOne(syncObjects);
	[MethodImpl(MethodImplOptions.AggressiveInlining)]
	public static bool TryWaitAll(this SynchronizationResource[] syncObjects) => MultiSync.TryWaitAll(syncObjects);
}
