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
			var version = Interop.GetApiVersion();

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
				EntryPoint = "version_get_api_version")]
			public static extern FrostSemanticVersion GetApiVersion();

			[MethodImpl(MethodImplOptions.AggressiveInlining)]
			[DllImport(
				dllName: Settings.frostApiPath,
				CallingConvention = CallingConvention.StdCall,
				EntryPoint = "version_check_compatibility")]
			public static extern bool CheckCompatibility(
				FrostSemanticVersion version, 
				FrostSemanticVersion target);
		}
	}
}
