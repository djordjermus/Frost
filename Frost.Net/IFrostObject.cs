using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Frost.Net
{
	public interface IFrostObject
	{
		public IntPtr Handle { get; }
	}
}
