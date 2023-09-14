using System.Text;

namespace Frost.Net
{
	internal static class Unmanaged
	{
		public static string StringFromUnmanagedWstr(IntPtr pString, int length)
		{
			unsafe
			{
				char* read = (char*)pString;
				char* end = read + length;
				StringBuilder sb = new StringBuilder(length);
				while (read != end)				{
					sb.Append(*read);
					read++;
				}
				return sb.ToString();
			}
		}
	}
}
