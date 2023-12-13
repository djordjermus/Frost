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

public enum Keycode : byte
{
	Nullkey				= 0x00,

	MouseLeft			= 0xe0,
	MouseRight			= 0xe1,
	MouseMiddle			= 0xe2,
	MouseX1				= 0xe3,
	MouseX2				= 0xe4,

	Escape				= 0x01,
	An1					= 0x02,
	An2					= 0x03,
	An3					= 0x04,
	An4					= 0x05,
	An5					= 0x06,
	An6					= 0x07,
	An7					= 0x08,
	An8					= 0x09,
	An9					= 0x0a,
	An0					= 0x0b,
	Dash				= 0x0c,
	Equals				= 0x0d,
	Backspace			= 0x0e,

	Tilde				= 0x29,
	Q					= 0x10,
	W					= 0x11,
	E					= 0x12,
	R					= 0x13,
	T					= 0x14,
	Y					= 0x15,
	U					= 0x16,
	I					= 0x17,
	O					= 0x18,
	P					= 0x19,
	BracketOpen			= 0x1a,
	BracketClose		= 0x1b,
	A					= 0x1e,
	S					= 0x1f,
	D					= 0x20,
	F					= 0x21,
	G					= 0x22,
	H					= 0x23,
	J					= 0x24,
	K					= 0x25,
	L					= 0x26,
	Semicolon			= 0x27,
	Quote				= 0x28,
	Z					= 0x2c,
	X					= 0x2d,
	C					= 0x2e,
	Backslash			= 0x2b,
	V					= 0x2f,
	B					= 0x30,
	N					= 0x31,
	M					= 0x32,
	Comma				= 0x33,
	Period				= 0x34,
	Slash				= 0x35,

	F1					= 0x3b,
	F2					= 0x3c,
	F3					= 0x3d,
	F4					= 0x3e,
	F5					= 0x3f,
	F6					= 0x40,
	F7					= 0x41,
	F8					= 0x42,
	F9					= 0x43,
	F10					= 0x44,
	F11					= 0x57,
	F12					= 0x58,

	Tab					= 0x0F,
	CapsLock			= 0x3a,
	LeftShift			= 0x2a,
	RightShift			= 0x36,
	LeftCtrl			= 0x1d,
	RightCtrl			= 0x76,
	LeftWin				= 0xB4,
	RightWin			= 0xB6,
	LeftAlt				= 0x38,
	RightAlt			= 0x91,

	space				= 0x39,
	enter				= 0x1c,

	LeftArrow			= 0xA4,
	UpArrow				= 0xA1,
	RightArrow			= 0xA6,
	DownArrow			= 0xA9,

	Numpad0				= 0x52,
	Numpad1				= 0x4f,
	Numpad2				= 0x50,
	Numpad3				= 0x51,
	Numpad4				= 0x4b,
	Numpad5				= 0x4c,
	Numpad6				= 0x4d,
	Numpad7				= 0x47,
	Numpad8				= 0x48,
	Numpad9				= 0x49,
	NumpadNumlock		= 0x45,
	NumpadDivide		= 0x8E,
	NumpadMultiply		= 0x37,
	NumpadMinus			= 0x4a,
	NumpadPlus			= 0x4e,
	NumpadEnter			= 0x75,
	NumpadSeparator		= 0x53,

	PrintScreen			= 0x83,
	ScrollLock			= 0x46,
	Pause				= 0xD3,

	Insert				= 0xAB,
	Home				= 0xA0,
	PageUp				= 0xA2,
	Delete				= 0xAC,
	End					= 0xA8,
	PageDown			= 0xAA,
};