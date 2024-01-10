using Frost.Net.Interoperability;
using System.Runtime.CompilerServices;

namespace Frost.Net;

public class Random
{
	/// <summary>
	/// Creates an instance with given seed
	/// </summary>
	/// <param name="seed">First seed used for generating random numbers</param>
	public Random(ulong seed)
	{
		_seed = seed;
	}

	/// <summary>
	/// Creates an instance with timestamp as seed
	/// </summary>
	public Random()
	{
		_seed = FrostApi.Clock.GetTimestamp();
	}


	/// <summary>
	/// Generates a random number.
	/// </summary>
	/// <returns>Unsigned integer</returns>
	public ulong NextU64() =>
		NextU64(ref _seed);

	/// <summary>
	/// Generates a random number in bounds [minimum, minimum + range).
	/// </summary>
	/// <param name="minimum">Minimum value of generated number.</param>
	/// <param name="range">Maximum offset of generated number from the minimum.</param>
	/// <returns>Unsigned 64bit integer</returns>
	public ulong NextU64(ulong minimum, ulong range) =>
		NextU64(ref _seed, minimum, range);

	/// <summary>
	/// Generates a random number.
	/// </summary>
	/// <returns>Signed integer</returns>
	public long NextI64() =>
		NextI64(ref _seed);

	/// <summary>
	/// Generates a random number in bounds [minimum, minimum + range).
	/// </summary>
	/// <param name="minimum">Minimum value of generated number.</param>
	/// <param name="range">Maximum offset of generated number from the minimum.</param>
	/// <returns>Signed 64bit integer</returns>
	public long NextI64(long minimum, ulong range) =>
		NextI64(ref _seed, minimum, range);

	/// <summary>
	/// Generates a random number in bounds [minimum, minimum + range).
	/// </summary>
	/// <param name="minimum">Minimum value of generated number.</param>
	/// <param name="range">Maximum offset of generated number from the minimum.</param>
	/// <returns>64bit floating point number</returns>
	public double NextF64(double minimum = 0.0, double range = 1.0) =>
		NextF64(ref _seed, minimum, range);

	/// <summary>
	/// Generates a random number.
	/// </summary>
	/// <param name="seed">Seed used for generating random number.</param>
	/// <returns>Random unsigned integer.</returns>

	[MethodImpl(MethodImplOptions.AggressiveInlining)]
	public static ulong NextU64(ref ulong seed)
	{
		unsafe
		{
			fixed (ulong* pSeed = &seed)
				return FrostApi.Random.Generate(new IntPtr(pSeed));
		}
	}

	/// <summary>
	/// Generates a random number in bounds [minimum, minimum + range).
	/// </summary>
	/// <param name="seed">Seed used for generating random number.</param>
	/// <param name="minimum">Minimum value of generated number.</param>
	/// <param name="range">Maximum offset of generated number from the minimum.</param>
	/// <returns>Random unsigned integer.</returns>
	[MethodImpl(MethodImplOptions.AggressiveInlining)]
	public static ulong NextU64(ref ulong seed, ulong minimum, ulong range)
	{
		unsafe
		{
			fixed (ulong* pSeed = &seed)
				return FrostApi.Random.GenerateRange(new IntPtr(pSeed), minimum, range);
		}
	}
	
	/// <summary>
	/// Generates a random number.
	/// </summary>
	/// <param name="seed">Seed used for generating random number.</param>
	/// <returns>Random signed integer.</returns>
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
	
	/// <summary>
	/// Generates a random number in bounds [minimum, minimum + range).
	/// </summary>
	/// <param name="seed">Seed used for generating random number.</param>
	/// <param name="minimum">Minimum value of generated number.</param>
	/// <param name="range">Maximum offset of generated number from the minimum.</param>
	/// <returns>Random signed integer.</returns>
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

	/// <summary>
	/// Generates a random number in bounds [minimum, minimum + range).
	/// </summary>
	/// <param name="seed">Seed used for generating random number.</param>
	/// <param name="minimum">Minimum value of generated number.</param>
	/// <param name="range">Maximum offset of generated number from the minimum.</param>
	/// <returns>Random 64bit floating point number.</returns>
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

	private ulong _seed;
}
