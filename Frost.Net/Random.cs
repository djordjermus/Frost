using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;

namespace Frost.Net
{
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

		public ulong NextU64()
		{
			unsafe
			{
				fixed (ulong* pSeed = &_seed)
					return FrostApi.Random.Generate(new IntPtr(pSeed));
			}
		}

		public ulong NextU64(ulong minimum, ulong range)
		{
			unsafe
			{
				fixed (ulong* pSeed = &_seed)
					return FrostApi.Random.GenerateRange(new IntPtr(pSeed), minimum, range);
			}
		}

		public long NextI64()
		{
			unsafe
			{
				unchecked
				{ 
					fixed (ulong* pSeed = &_seed)
						return (long)FrostApi.Random.Generate(new IntPtr(pSeed));
				}
			}
		}

		public long NextI64(long minimum, ulong range)
		{
			unsafe
			{
				unchecked
				{
					fixed (ulong* pSeed = &_seed)
						return (long)FrostApi.Random.GenerateRange(new IntPtr(pSeed), (ulong)minimum, range);
				}
			}
		}

		public double NextF64(double minimum = 0.0, double range = 1.0)
		{
			unsafe
			{
				unchecked
				{
					fixed (ulong* pSeed = &_seed)
						return FrostApi.Random.GenerateF64(new IntPtr(pSeed), minimum, range);
				}
			}
		}
	}
}
