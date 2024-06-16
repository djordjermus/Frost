using Frost.Interoperability;
using Frost.Utilities;

namespace Frost.Input;

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
				var length = FrostApi.Keycode.ToString((byte)keycode, new nint(output), size, normalizeCase);
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