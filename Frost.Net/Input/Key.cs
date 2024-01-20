using Frost.Net.Interoperability;
using Frost.Net.Utilities;

namespace Frost.Net.Input;

public static class Key
{
	public static string Translate(Keycode keycode, bool normalizeCase = true)
	{
		const int size = 16;
		Span<char> chars = stackalloc char[size];
		unsafe
		{
			fixed (char* output = chars)
			{
				var length = FrostApi.Keycode.ToString((byte)keycode, new IntPtr(output), size, normalizeCase);
				return new string(chars.Slice(0, (int)length));
			}
		}
	}

	public static string GetName(Keycode keycode)
	{
		unsafe
		{
			var ptr = FrostApi.Keycode.GetName((byte)keycode);
			return Unmanaged.StringFromUnmanagedWstr(ptr);
		}
	}
}