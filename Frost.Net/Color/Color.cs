﻿using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;

namespace Frost.Net
{
    [StructLayout(LayoutKind.Explicit)]
    public class Color
    {
        [FieldOffset(0)]
        public int value;
        [FieldOffset(0)]
        public byte r;
        [FieldOffset(1)]
        public byte g;
        [FieldOffset(2)]
        public byte b;
        [FieldOffset(3)]
        public byte a;



        public Color(byte r, byte g, byte b, byte a)
        {
            Unsafe.SkipInit(out value); // Skip initialization, bytes initialized
            this.r = r;
            this.g = g;
            this.b = b;
            this.a = a;
        }
        public Color(float r, float g, float b, float a)
        {
            Unsafe.SkipInit(out value); // Skip initialization, bytes initialized
            this.r = unchecked((byte)((r < 0.0f ? 0.0f : r > 1.0f ? 1.0f : r) * 255));
            this.g = unchecked((byte)((g < 0.0f ? 0.0f : g > 1.0f ? 1.0f : g) * 255));
            this.b = unchecked((byte)((b < 0.0f ? 0.0f : b > 1.0f ? 1.0f : b) * 255));
            this.a = unchecked((byte)((a < 0.0f ? 0.0f : a > 1.0f ? 1.0f : a) * 255));
        }
        public Color(int value)
        {
            Unsafe.SkipInit(out r); // Skip initialization, value initialized
            Unsafe.SkipInit(out g); // Skip initialization, value initialized
            Unsafe.SkipInit(out b); // Skip initialization, value initialized
            Unsafe.SkipInit(out a); // Skip initialization, value initialized
            this.value = value;
        }
        public Color(HDRColor rgba)
        {
            unsafe
            {
                Unsafe.SkipInit(out r); // Skip initialization, value initialized
                Unsafe.SkipInit(out g); // Skip initialization, value initialized
                Unsafe.SkipInit(out b); // Skip initialization, value initialized
                Unsafe.SkipInit(out a); // Skip initialization, value initialized

                fixed (void* output = &value)
                    Interop.Rgba32ToRgba8(new IntPtr(&rgba.r), new IntPtr(output));
            }
        }
        public Color(HSVA hsva)
        {
            unsafe
            {
                Unsafe.SkipInit(out value);
                float* rgba32 = stackalloc float[4];
                Interop.HsvaToRgba32(new IntPtr(&hsva.h), new IntPtr(rgba32));
                r = unchecked((byte)(rgba32[0] * 255));
                g = unchecked((byte)(rgba32[1] * 255));
                b = unchecked((byte)(rgba32[2] * 255));
                a = unchecked((byte)(rgba32[3] * 255));
            }
        }
        public Color(HSLA hsla)
        {
            unsafe
            {
                Unsafe.SkipInit(out value);
                float* rgba32 = stackalloc float[4];
                Interop.HslaToRgba32(new IntPtr(&hsla.h), new IntPtr(rgba32));
                r = unchecked((byte)(rgba32[0] * 255));
                g = unchecked((byte)(rgba32[1] * 255));
                b = unchecked((byte)(rgba32[2] * 255));
                a = unchecked((byte)(rgba32[3] * 255));
            }
        }
        public Color(CMYK cmyk)
        {
            unsafe
            {
                Unsafe.SkipInit(out value);
                float* rgba32 = stackalloc float[4];
                Interop.CmykToRgba32(new IntPtr(&cmyk.c), new IntPtr(rgba32));
                r = unchecked((byte)(rgba32[0] * 255));
                g = unchecked((byte)(rgba32[1] * 255));
                b = unchecked((byte)(rgba32[2] * 255));
                a = unchecked((byte)(rgba32[3] * 255));
            }
        }

        public HSVA ToHsva()
        {
            unsafe
            {
                float* rgba32 = stackalloc float[4];
                float* hsva = stackalloc float[4];
                Interop.Rgba8ToRgba32(value, new IntPtr(rgba32));
                Interop.Rgba32ToHsva(new IntPtr(rgba32), new IntPtr(hsva));
                return new HSVA() { h = hsva[0], s = hsva[1], v = hsva[2], a = hsva[3] };
            }
        }
        public HSLA ToHsla()
        {
            unsafe
            {
                float* rgba32 = stackalloc float[4];
                float* hsla = stackalloc float[4];
                Interop.Rgba8ToRgba32(value, new IntPtr(rgba32));
                Interop.Rgba32ToHsla(new IntPtr(rgba32), new IntPtr(hsla));
                return new HSLA() { h = hsla[0], s = hsla[1], l = hsla[2], a = hsla[3] };
            }
        }
        public CMYK ToCmyk()
        {
            unsafe
            {
                float* rgba32 = stackalloc float[4];
                float* cmyk = stackalloc float[4];
                Interop.Rgba8ToRgba32(value, new IntPtr(rgba32));
                Interop.Rgba32ToCmyk(new IntPtr(rgba32), new IntPtr(cmyk));
                return new CMYK() { c = cmyk[0], m = cmyk[1], y = cmyk[2], k = cmyk[3] };
            }
        }



        internal static class Interop
        {
            [MethodImpl(MethodImplOptions.AggressiveInlining)]
            [DllImport(dllName: Settings.frostApiPath, CallingConvention = CallingConvention.StdCall, EntryPoint = "color_rgba8_to_rgba32")]
            public extern static void Rgba8ToRgba32(int input, IntPtr output);

            [MethodImpl(MethodImplOptions.AggressiveInlining)]
            [DllImport(dllName: Settings.frostApiPath, CallingConvention = CallingConvention.StdCall, EntryPoint = "color_rgba32_to_rgba8")]
            public extern static void Rgba32ToRgba8(IntPtr input, IntPtr output);



            [MethodImpl(MethodImplOptions.AggressiveInlining)]
            [DllImport(dllName: Settings.frostApiPath, CallingConvention = CallingConvention.StdCall, EntryPoint = "color_rgba32_to_hsva")]
            public extern static void Rgba32ToHsva(IntPtr input, IntPtr output);

            [MethodImpl(MethodImplOptions.AggressiveInlining)]
            [DllImport(dllName: Settings.frostApiPath, CallingConvention = CallingConvention.StdCall, EntryPoint = "color_rgba32_to_hsla")]
            public extern static void Rgba32ToHsla(IntPtr input, IntPtr output);

            [MethodImpl(MethodImplOptions.AggressiveInlining)]
            [DllImport(dllName: Settings.frostApiPath, CallingConvention = CallingConvention.StdCall, EntryPoint = "color_rgba32_to_cmyk")]
            public extern static void Rgba32ToCmyk(IntPtr input, IntPtr output);



            [MethodImpl(MethodImplOptions.AggressiveInlining)]
            [DllImport(dllName: Settings.frostApiPath, CallingConvention = CallingConvention.StdCall, EntryPoint = "color_hsva_to_rgba32")]
            public extern static void HsvaToRgba32(IntPtr input, IntPtr output);

            [MethodImpl(MethodImplOptions.AggressiveInlining)]
            [DllImport(dllName: Settings.frostApiPath, CallingConvention = CallingConvention.StdCall, EntryPoint = "color_hsla_to_rgba32")]
            public extern static void HslaToRgba32(IntPtr input, IntPtr output);

            [MethodImpl(MethodImplOptions.AggressiveInlining)]
            [DllImport(dllName: Settings.frostApiPath, CallingConvention = CallingConvention.StdCall, EntryPoint = "color_cmyk_to_rgba32")]
            public extern static void CmykToRgba32(IntPtr input, IntPtr output);
        }
    }
}
