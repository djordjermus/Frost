using System.Runtime.CompilerServices;
using Frost.Interoperability;

namespace Frost.Models;

/// <summary>
/// High dynamic range RGBA Color model - Red, Green, Blue, Alpha
/// </summary>
public struct HDRColor
{
    public float r;
    public float g;
    public float b;
    public float a;

    public HDRColor(float r, float g, float b, float a)
    {
        this.r = r;
        this.g = g;
        this.b = b;
        this.a = a;
    }
    public HDRColor(int value)
    {
        Unsafe.SkipInit(out r); // Skip initialization, initialized with Interop
        Unsafe.SkipInit(out g); // Skip initialization, initialized with Interop
        Unsafe.SkipInit(out b); // Skip initialization, initialized with Interop
        Unsafe.SkipInit(out a); // Skip initialization, initialized with Interop
        unsafe
        {
            fixed (void* output = &r)
                FrostApi.Color.Rgba8ToRgba32(value, new IntPtr(output));
        }
    }
    public HDRColor(Color rgba)
    {
        unsafe
        {
            Unsafe.SkipInit(out r); // Skip initialization, initialized with Interop
            Unsafe.SkipInit(out g); // Skip initialization, initialized with Interop
            Unsafe.SkipInit(out b); // Skip initialization, initialized with Interop
            Unsafe.SkipInit(out a); // Skip initialization, initialized with Interop

            fixed (void* output = &r)
                FrostApi.Color.Rgba8ToRgba32(rgba.value, new IntPtr(output));
        }
    }
    public HDRColor(HSVA hsva)
    {
        unsafe
        {
            Unsafe.SkipInit(out r); // Skip initialization, initialized with Interop
            Unsafe.SkipInit(out g); // Skip initialization, initialized with Interop
            Unsafe.SkipInit(out b); // Skip initialization, initialized with Interop
            Unsafe.SkipInit(out a); // Skip initialization, initialized with Interop

            fixed (void* output = &r)
                FrostApi.Color.HsvaToRgba32(new IntPtr(&hsva.h), new IntPtr(output));
        }
    }
    public HDRColor(HSLA hsla)
    {
        unsafe
        {
            Unsafe.SkipInit(out r); // Skip initialization, initialized with Interop
            Unsafe.SkipInit(out g); // Skip initialization, initialized with Interop
            Unsafe.SkipInit(out b); // Skip initialization, initialized with Interop
            Unsafe.SkipInit(out a); // Skip initialization, initialized with Interop

            fixed (void* output = &r)
                FrostApi.Color.HslaToRgba32(new IntPtr(&hsla.h), new IntPtr(output));
        }
    }
    public HDRColor(CMYK cmyk)
    {
        unsafe
        {
            Unsafe.SkipInit(out r); // Skip initialization, initialized with Interop
            Unsafe.SkipInit(out g); // Skip initialization, initialized with Interop
            Unsafe.SkipInit(out b); // Skip initialization, initialized with Interop
            Unsafe.SkipInit(out a); // Skip initialization, initialized with Interop

            fixed (void* output = &r)
                FrostApi.Color.CmykToRgba32(new IntPtr(&cmyk.c), new IntPtr(output));
        }
    }

    public HSVA ToHsva()
    {
        unsafe
        {
            float* hsva = stackalloc float[4];
            fixed (void* input = &r)
                FrostApi.Color.Rgba32ToHsva(new IntPtr(input), new IntPtr(hsva));
            return new HSVA() { h = hsva[0], s = hsva[1], v = hsva[2], a = hsva[3] };
        }
    }
    public HSLA ToHsla()
    {
        unsafe
        {
            float* hsla = stackalloc float[4];
            fixed (void* input = &r)
                FrostApi.Color.Rgba32ToHsla(new IntPtr(input), new IntPtr(hsla));
            return new HSLA() { h = hsla[0], s = hsla[1], l = hsla[2], a = hsla[3] };
        }
    }
    public CMYK ToCmyk()
    {
        unsafe
        {
            float* cmyk = stackalloc float[4];
            fixed (void* input = &r)
                FrostApi.Color.Rgba32ToCmyk(new IntPtr(input), new IntPtr(cmyk));
            return new CMYK() { c = cmyk[0], m = cmyk[1], y = cmyk[2], k = cmyk[3] };
        }
    }


    public static HDRColor FromHsva(HSVA hsva)
    {
        HDRColor result;
        Unsafe.SkipInit(out result);
        unsafe { FrostApi.Color.HsvaToRgba32(new IntPtr(&hsva.h), new IntPtr(&result.r)); }
        return result;
    }
    public static HDRColor FromHsla(HSLA hsla)
    {
        HDRColor result;
        Unsafe.SkipInit(out result);
        unsafe { FrostApi.Color.HslaToRgba32(new IntPtr(&hsla.h), new IntPtr(&result.r)); }
        return result;
    }
    public static HDRColor FromCmyk(CMYK cmyk)
    {
        HDRColor result;
        Unsafe.SkipInit(out result);
        unsafe { FrostApi.Color.CmykToRgba32(new IntPtr(&cmyk.c), new IntPtr(&result.r)); }
        return result;
    }
}
