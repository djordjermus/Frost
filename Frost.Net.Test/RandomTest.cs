﻿namespace Frost.Net.Test
{
	[TestClass]
	public class RandomTest
	{
		[TestMethod]
		public void TestU64()
		{
			const ulong minimum = 10;
			const ulong range = 5;
			var rng = new Random();
			for (int i = 0; i < 1_000_000; i++)
			{
				ulong value = rng.NextU64(minimum, range);
				if (value < minimum || value >= (minimum + range))
					Assert.Fail($"Random.NextU64({minimum}, {range}) => {value}");
			}
		}

		[TestMethod]
		public void TestI64()
		{
			const long minimum = -10;
			const long range = 5;
			var rng = new Random();
			for (int i = 0; i < 1_000_000; i++)
			{
				long value = rng.NextI64(minimum, range);
				if (value < minimum || value >= (minimum + range))
					Assert.Fail($"Random.NextI64({minimum}, {range}) => {value}");
			}
		}

		[TestMethod]
		public void TestF64()
		{
			const double minimum = -10;
			const double range   = 5;
			var rng = new Random();
			for (int i = 0; i < 1_000_000; i++)
			{
				double value = rng.NextF64(minimum, range);
				if (value < minimum || value >= (minimum + range))
					Assert.Fail($"Random.NextF64({minimum}, {range}) => {value}");
			}
		}
	}
}