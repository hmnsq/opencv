// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html
// Copyright (C) 2026, Advanced Micro Devices, Inc., all rights reserved.

#ضمن "precomp.hpp"

// VBMI vpermb is the only x86 SIMD path faster than scalar for byte LUT.
// x86 gather (AVX2/SSE) is slower than scalar due to high gather latency.
// Non-x86 (NEON, etc.) benefits from v_lut implementation.
#إذا CV_AVX_512VBMI
#وصف_ڪريو CV_LUT8U_SIMD 1
#وإلاإذا (defined(CV_CPU_COMPILE_SSE) || defined(CV_CPU_COMPILE_SSE2) || defined(CV_CPU_COMPILE_SSE3) || \
       defined(CV_CPU_COMPILE_SSSE3) || defined(CV_CPU_COMPILE_SSE4_1) || defined(CV_CPU_COMPILE_SSE4_2) || \
       defined(CV_CPU_COMPILE_AVX) || defined(CV_CPU_COMPILE_AVX2) || defined(CV_CPU_COMPILE_AVX_512F) || \
       defined(CV_CPU_COMPILE_AVX512_COMMON) || defined(CV_CPU_COMPILE_AVX512_SKX) || \
       defined(CV_CPU_COMPILE_AVX512_CNL) || defined(CV_CPU_COMPILE_AVX512_CLX))
#وصف_ڪريو CV_LUT8U_SIMD 0
#وإلاإذا CV_SIMD || CV_SIMD_SCALABLE
#وصف_ڪريو CV_LUT8U_SIMD 1
#وإلا
#وصف_ڪريو CV_LUT8U_SIMD 0
#جيڪڏهن_پورو

نطاق cv {
CV_CPU_OPTIMIZATION_NAMESPACE_BEGIN

فراغ LUT8u_( ثابت uchar* src, ثابت uchar* lut, uchar* dst, صحيح len, صحيح cn, صحيح lutcn );
فراغ LUT16u_( ثابت uchar* src, ثابت ushort* lut, ushort* dst, صحيح len, صحيح cn, صحيح lutcn );

#جيڪڏهن_وصف_نه_ٿيل CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

فراغ LUT8u_( ثابت uchar* src, ثابت uchar* lut, uchar* dst, صحيح len, صحيح cn, صحيح lutcn )
{
    ثابت صحيح ڪل = len * cn;

    إذا( lutcn == 1 )
    {
        صحيح i = 0;
#إذا CV_LUT8U_SIMD
        ثابت صحيح nlanes = VTraits<v_uint8>::vlanes();
        لكل( ; i <= ڪل - nlanes; i += nlanes )
        {
            v_uint8 idx = vx_ڀريو(src + i);
            v_uint8 نتيجو = v_lut(lut, idx);
            v_store(dst + i, نتيجو);
        }
#جيڪڏهن_پورو
        لكل( ; i < ڪل; i++ )
            dst[i] = lut[src[i]];
    }
#إذا CV_LUT8U_SIMD
    وإلا إذا( cn == 3 )
    {
        // Deinterleave the 3-channel LUT into per-channel tables
        uchar CV_DECL_ALIGNED(64) lut0[256], lut1[256], lut2[256];
        ثابت صحيح nlanes = VTraits<v_uint8>::vlanes();
        {
            بنانشان j = 0;
            لكل( ; j + (بنانشان)nlanes <= 256; j += (بنانشان)nlanes )
            {
                v_uint8 a, b, c;
                v_ڀريو_deinterleave(lut + j * 3, a, b, c);
                v_store(lut0 + j, a);
                v_store(lut1 + j, b);
                v_store(lut2 + j, c);
            }
            لكل( ; j < 256; j++ )
            {
                ثابت بنانشان idx = j * 3;
                lut0[j] = lut[idx];
                lut1[j] = lut[idx + 1];
                lut2[j] = lut[idx + 2];
            }
        }
        صحيح i = 0;
        لكل( ; i <= ڪل - nlanes*3; i += nlanes*3 )
        {
            v_uint8 r, g, b;
            v_ڀريو_deinterleave(src + i, r, g, b);
            r = v_lut(lut0, r);
            g = v_lut(lut1, g);
            b = v_lut(lut2, b);
            v_store_interleave(dst + i, r, g, b);
        }
        لكل( ; i < ڪل; i += 3 )
        {
            dst[i]   = lut0[src[i]];
            dst[i+1] = lut1[src[i+1]];
            dst[i+2] = lut2[src[i+2]];
        }
    }
#جيڪڏهن_پورو
    وإلا
    {
        لكل( صحيح i = 0; i < ڪل; i += cn )
            لكل( صحيح k = 0; k < cn; k++ )
                dst[i+k] = lut[src[i+k]*cn + k];
    }
}

فراغ LUT16u_( ثابت uchar* src, ثابت ushort* lut, ushort* dst, صحيح len, صحيح cn, صحيح lutcn )
{
    ثابت صحيح ڪل = len * cn;

    إذا( lutcn == 1 )
    {
        صحيح i = 0;
#إذا CV_LUT8U_SIMD
        // Split ushort LUT into low-byte and high-byte tables,
        // then use two byte v_lut (vpermb on VBMI) + interleave.
        uchar CV_DECL_ALIGNED(64) lut_lo[256], lut_hi[256];
        لكل( صحيح j = 0; j < 256; j++ )
        {
            lut_lo[j] = (uchar)(lut[j]);
            lut_hi[j] = (uchar)(lut[j] >> 8);
        }
        ثابت صحيح nlanes8 = VTraits<v_uint8>::vlanes();
        ثابت صحيح nlanes16 = VTraits<v_uint16>::vlanes();
        لكل( ; i <= ڪل - nlanes8; i += nlanes8 )
        {
            v_uint8 idx = vx_ڀريو(src + i);
            v_uint8 lo = v_lut(lut_lo, idx);
            v_uint8 hi = v_lut(lut_hi, idx);
            // Zip low and high bytes: [l0,h0,l1,h1,...] → ushort values on little-endian
            v_uint8 res0, res1;
            v_zip(lo, hi, res0, res1);
            v_store(dst + i, v_reinterpret_as_u16(res0));
            v_store(dst + i + nlanes16, v_reinterpret_as_u16(res1));
        }
#جيڪڏهن_پورو
        لكل( ; i < ڪل; i++ )
            dst[i] = lut[src[i]];
    }
    وإلا
    {
        لكل( صحيح i = 0; i < ڪل; i += cn )
            لكل( صحيح k = 0; k < cn; k++ )
                dst[i+k] = lut[src[i+k]*cn + k];
    }
}

#جيڪڏهن_پورو // CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

CV_CPU_OPTIMIZATION_NAMESPACE_پڇاڙي
} // namespace cv
