using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace Frost.Net.Models
{
	public sealed class SemanticVersion
	{
		public ushort Major { get; set; }
		public ushort Minor { get; set; }
		public uint   Patch { get; set; }
		public string Decoration { get; set; } = string.Empty;



		public bool CheckCompatibility(SemanticVersion target)
		{
			var current_fsv = new Interop.FrostSemanticVersion()
			{
				major = Major,
				minor = Minor,
				patch = Patch,
				decoration = IntPtr.Zero,
			};

			var target_fsv = new Interop.FrostSemanticVersion()
			{
				major = target.Major,
				minor = target.Minor,
				patch = target.Patch,
				decoration = IntPtr.Zero,
			};

			return Interop.CheckCompatibility(current_fsv, target_fsv);
		}



		public static SemanticVersion GetApiVersion()
		{
			Interop.GetApiVersion(out Interop.FrostSemanticVersion version);

			return new SemanticVersion()
			{
				Major = version.major,
				Minor = version.minor,
				Patch = version.patch,
				Decoration = Unmanaged.StringFromUnmanagedWstr(version.decoration)
			};
		}

		internal static class Interop
		{
			[StructLayout(LayoutKind.Sequential)]
			public struct FrostSemanticVersion
			{
				public ushort major;
				public ushort minor;
				public uint   patch;
				public IntPtr decoration;
			}
			[MethodImpl(MethodImplOptions.AggressiveInlining)]
			[DllImport(
				dllName: Settings.frostApiPath,
				CallingConvention = CallingConvention.StdCall,
				EntryPoint = "?get_api_version@api@semantic_version@frost@@SAXPEAV23@@Z")]
			public static extern void GetApiVersion(out FrostSemanticVersion output);

			[MethodImpl(MethodImplOptions.AggressiveInlining)]
			[DllImport(
				dllName: Settings.frostApiPath,
				CallingConvention = CallingConvention.StdCall,
				EntryPoint = "?check_compatibility@api@semantic_version@frost@@SA_NV23@0@Z")]
			public static extern bool CheckCompatibility(
				FrostSemanticVersion version, 
				FrostSemanticVersion target);
		}
	}
}
