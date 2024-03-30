using System.Text;

namespace Frost.Utilities;

internal static class Unmanaged
{
    public static string StringFromUnmanagedWstr(IntPtr pString)
    {
        unsafe
        {
            char* read = (char*)pString;
            StringBuilder sb = new StringBuilder();
            while (*read != '\0')
            {
                sb.Append(*read);
                read++;
            }
            return sb.ToString();
        }
    }

    public static void ReadCWSTR(IntPtr pString, Span<char> output)
    {
        unsafe
        {
            char* read = (char*)pString;
            for (int i = 0; i < output.Length; i++)
                output[i] = read[i];
        }
    }
}
