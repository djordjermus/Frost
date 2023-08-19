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
			_seed = Clock.Interop.GetTimestamp();
		}

		public ulong NextU64()
		{
			unsafe
			{
				fixed (ulong* pSeed = &_seed)
					return Interop.Generate(new IntPtr(pSeed));
			}
		}

		public ulong NextU64(ulong minimum, ulong range)
		{
			unsafe
			{
				fixed (ulong* pSeed = &_seed)
					return Interop.GenerateRange(new IntPtr(pSeed), minimum, range);
			}
		}

		public long NextI64()
		{
			unsafe
			{
				unchecked
				{ 
					fixed (ulong* pSeed = &_seed)
						return (long)Interop.Generate(new IntPtr(pSeed));
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
						return (long)Interop.GenerateRange(new IntPtr(pSeed), (ulong)minimum, range);
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
						return Interop.GenerateF64(new IntPtr(pSeed), minimum, range);
				}
			}
		}

		internal static class Interop
		{
			[MethodImpl(MethodImplOptions.AggressiveInlining)]
			[DllImport(
				Settings.frostApiPath,
				CallingConvention = CallingConvention.StdCall,
				EntryPoint = "random_generate")]
			public static extern ulong Generate(IntPtr pSeed);

			[MethodImpl(MethodImplOptions.AggressiveInlining)]
			[DllImport(
				Settings.frostApiPath,
				CallingConvention = CallingConvention.StdCall,
				EntryPoint = "random_generate_range")]
			public static extern ulong GenerateRange(IntPtr pSeed, ulong min, ulong range);

			[MethodImpl(MethodImplOptions.AggressiveInlining)]
			[DllImport(
				Settings.frostApiPath,
				CallingConvention = CallingConvention.StdCall,
				EntryPoint = "random_generate_range_double")]
			public static extern double GenerateF64(IntPtr pSeed, double min, double range);
		}
	}
}
