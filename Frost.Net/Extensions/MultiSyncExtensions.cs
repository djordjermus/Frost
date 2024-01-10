using Frost.Net.Synchronization;
using Frost.Net.Synchronization.Interface;
using System.Runtime.CompilerServices;

namespace Frost.Net.Extensions;

public static class MultiSyncExtensions
{
	[MethodImpl(MethodImplOptions.AggressiveInlining)]
	public static int WaitOne(this ReadOnlySpan<ISynchronizable> syncObjects) => MultiSync.WaitOne(syncObjects);
	[MethodImpl(MethodImplOptions.AggressiveInlining)]
	public static bool WaitAll(this ReadOnlySpan<ISynchronizable> syncObjects) => MultiSync.WaitAll(syncObjects);
	[MethodImpl(MethodImplOptions.AggressiveInlining)]
	public static int TryWaitOne(this ReadOnlySpan<ISynchronizable> syncObjects) => MultiSync.TryWaitOne(syncObjects);
	[MethodImpl(MethodImplOptions.AggressiveInlining)]
	public static bool TryWaitAllExtension(this ReadOnlySpan<ISynchronizable> syncObjects) => MultiSync.TryWaitAll(syncObjects);



	[MethodImpl(MethodImplOptions.AggressiveInlining)]
	public static int WaitOne(this ISynchronizable[] syncObjects) => MultiSync.WaitOne(syncObjects);
	[MethodImpl(MethodImplOptions.AggressiveInlining)]
	public static bool WaitAll(this ISynchronizable[] syncObjects) => MultiSync.WaitAll(syncObjects);
	[MethodImpl(MethodImplOptions.AggressiveInlining)]
	public static int TryWaitOne(this ISynchronizable[] syncObjects) => MultiSync.TryWaitOne(syncObjects);
	[MethodImpl(MethodImplOptions.AggressiveInlining)]
	public static bool TryWaitAll(this ISynchronizable[] syncObjects) => MultiSync.TryWaitAll(syncObjects);
}
