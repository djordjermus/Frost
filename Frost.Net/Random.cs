namespace Frost.Net;

using Frost.Net.Interoperability;
using System.Runtime.CompilerServices;

public class Random
{
	private ulong _seed;

	public Random(ulong seed)
	{
		_seed = seed;
	}

	public Random()
	{
		_seed = FrostApi.Clock.GetTimestamp();
	}



	public ulong NextU64() =>
		NextU64(ref _seed);

	public ulong NextU64(ulong minimum, ulong range) =>
		NextU64(ref _seed, minimum, range);

	public long NextI64() =>
		NextI64(ref _seed);

	public long NextI64(long minimum, ulong range) =>
		NextI64(ref _seed, minimum, range);

	public double NextF64(double minimum = 0.0, double range = 1.0) =>
		NextF64(ref _seed, minimum, range);



	[MethodImpl(MethodImplOptions.AggressiveInlining)]
	public static ulong NextU64(ref ulong seed)
	{
		unsafe
		{
			fixed (ulong* pSeed = &seed)
				return FrostApi.Random.Generate(new IntPtr(pSeed));
		}
	}
	
	[MethodImpl(MethodImplOptions.AggressiveInlining)]
	public static ulong NextU64(ref ulong seed, ulong minimum, ulong range)
	{
		unsafe
		{
			fixed (ulong* pSeed = &seed)
				return FrostApi.Random.GenerateRange(new IntPtr(pSeed), minimum, range);
		}
	}

	[MethodImpl(MethodImplOptions.AggressiveInlining)]
	public static long NextI64(ref ulong seed)
	{
		unsafe
		{
			unchecked
			{
				fixed (ulong* pSeed = &seed)
					return (long)FrostApi.Random.Generate(new IntPtr(pSeed));
			}
		}
	}

	[MethodImpl(MethodImplOptions.AggressiveInlining)]
	public static long NextI64(ref ulong seed, long minimum, ulong range)
	{
		unsafe
		{
			unchecked
			{
				fixed (ulong* pSeed = &seed)
					return (long)FrostApi.Random.GenerateRange(new IntPtr(pSeed), (ulong)minimum, range);
			}
		}
	}

	[MethodImpl(MethodImplOptions.AggressiveInlining)]
	public static double NextF64(ref ulong seed, double minimum = 0.0, double range = 1.0)
	{
		unsafe
		{
			unchecked
			{
				fixed (ulong* pSeed = &seed)
					return FrostApi.Random.GenerateF64(new IntPtr(pSeed), minimum, range);
			}
		}
	}
}
