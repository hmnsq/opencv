// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html
// Copyright (C) 2026, Advanced Micro Devices, Inc., all rights reserved.

#ਅਨ੍ਤਰ੍ਭਾਵ "precomp.hpp"

// VBMI vpermb is the only x86 SIMD path faster than scalar for byte LUT.
// x86 gather (AVX2/SSE) is slower than scalar due to high gather latency.
// Non-x86 (NEON, etc.) benefits from v_lut implementation.
#ਜੇ CV_AVX_512VBMI
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ CV_LUT8U_SIMD 1
#ਜਾਂਜੇ (defined(CV_CPU_COMPILE_SSE) || defined(CV_CPU_COMPILE_SSE2) || defined(CV_CPU_COMPILE_SSE3) || \
       defined(CV_CPU_COMPILE_SSSE3) || defined(CV_CPU_COMPILE_SSE4_1) || defined(CV_CPU_COMPILE_SSE4_2) || \
       defined(CV_CPU_COMPILE_AVX) || defined(CV_CPU_COMPILE_AVX2) || defined(CV_CPU_COMPILE_AVX_512F) || \
       defined(CV_CPU_COMPILE_AVX512_COMMON) || defined(CV_CPU_COMPILE_AVX512_SKX) || \
       defined(CV_CPU_COMPILE_AVX512_CNL) || defined(CV_CPU_COMPILE_AVX512_CLX))
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ CV_LUT8U_SIMD 0
#ਜਾਂਜੇ CV_SIMD || CV_SIMD_SCALABLE
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ CV_LUT8U_SIMD 1
#ਨਹੀਂਤਾਂ
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ CV_LUT8U_SIMD 0
#ਜੇ_ਅੰਤ

ਨਾਮਾਕਾਸ਼ cv {
CV_CPU_OPTIMIZATION_NAMESPACE_BEGIN

ਖਾਲੀ LUT8u_( ਅਚਲ uchar* src, ਅਚਲ uchar* lut, uchar* dst, ਪੂਰਨ len, ਪੂਰਨ cn, ਪੂਰਨ lutcn );
ਖਾਲੀ LUT16u_( ਅਚਲ uchar* src, ਅਚਲ ushort* lut, ushort* dst, ਪੂਰਨ len, ਪੂਰਨ cn, ਪੂਰਨ lutcn );

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

ਖਾਲੀ LUT8u_( ਅਚਲ uchar* src, ਅਚਲ uchar* lut, uchar* dst, ਪੂਰਨ len, ਪੂਰਨ cn, ਪੂਰਨ lutcn )
{
    ਅਚਲ ਪੂਰਨ ਕੁੱਲ = len * cn;

    ਜੇ( lutcn == 1 )
    {
        ਪੂਰਨ i = 0;
#ਜੇ CV_LUT8U_SIMD
        ਅਚਲ ਪੂਰਨ nlanes = VTraits<v_uint8>::vlanes();
        ਲਈ( ; i <= ਕੁੱਲ - nlanes; i += nlanes )
        {
            v_uint8 idx = vx_ਲੱਦੋ(src + i);
            v_uint8 ਨਤੀਜਾ = v_lut(lut, idx);
            v_store(dst + i, ਨਤੀਜਾ);
        }
#ਜੇ_ਅੰਤ
        ਲਈ( ; i < ਕੁੱਲ; i++ )
            dst[i] = lut[src[i]];
    }
#ਜੇ CV_LUT8U_SIMD
    ਨਹੀਂਤਾਂ ਜੇ( cn == 3 )
    {
        // Deinterleave the 3-channel LUT into per-channel tables
        uchar CV_DECL_ALIGNED(64) lut0[256], lut1[256], lut2[256];
        ਅਚਲ ਪੂਰਨ nlanes = VTraits<v_uint8>::vlanes();
        {
            ਬਿਨਾਨਿਸ਼ਾਨੀ j = 0;
            ਲਈ( ; j + (ਬਿਨਾਨਿਸ਼ਾਨੀ)nlanes <= 256; j += (ਬਿਨਾਨਿਸ਼ਾਨੀ)nlanes )
            {
                v_uint8 a, b, c;
                v_ਲੱਦੋ_deinterleave(lut + j * 3, a, b, c);
                v_store(lut0 + j, a);
                v_store(lut1 + j, b);
                v_store(lut2 + j, c);
            }
            ਲਈ( ; j < 256; j++ )
            {
                ਅਚਲ ਬਿਨਾਨਿਸ਼ਾਨੀ idx = j * 3;
                lut0[j] = lut[idx];
                lut1[j] = lut[idx + 1];
                lut2[j] = lut[idx + 2];
            }
        }
        ਪੂਰਨ i = 0;
        ਲਈ( ; i <= ਕੁੱਲ - nlanes*3; i += nlanes*3 )
        {
            v_uint8 r, g, b;
            v_ਲੱਦੋ_deinterleave(src + i, r, g, b);
            r = v_lut(lut0, r);
            g = v_lut(lut1, g);
            b = v_lut(lut2, b);
            v_store_interleave(dst + i, r, g, b);
        }
        ਲਈ( ; i < ਕੁੱਲ; i += 3 )
        {
            dst[i]   = lut0[src[i]];
            dst[i+1] = lut1[src[i+1]];
            dst[i+2] = lut2[src[i+2]];
        }
    }
#ਜੇ_ਅੰਤ
    ਨਹੀਂਤਾਂ
    {
        ਲਈ( ਪੂਰਨ i = 0; i < ਕੁੱਲ; i += cn )
            ਲਈ( ਪੂਰਨ k = 0; k < cn; k++ )
                dst[i+k] = lut[src[i+k]*cn + k];
    }
}

ਖਾਲੀ LUT16u_( ਅਚਲ uchar* src, ਅਚਲ ushort* lut, ushort* dst, ਪੂਰਨ len, ਪੂਰਨ cn, ਪੂਰਨ lutcn )
{
    ਅਚਲ ਪੂਰਨ ਕੁੱਲ = len * cn;

    ਜੇ( lutcn == 1 )
    {
        ਪੂਰਨ i = 0;
#ਜੇ CV_LUT8U_SIMD
        // Split ushort LUT into low-byte and high-byte tables,
        // then use two byte v_lut (vpermb on VBMI) + interleave.
        uchar CV_DECL_ALIGNED(64) lut_lo[256], lut_hi[256];
        ਲਈ( ਪੂਰਨ j = 0; j < 256; j++ )
        {
            lut_lo[j] = (uchar)(lut[j]);
            lut_hi[j] = (uchar)(lut[j] >> 8);
        }
        ਅਚਲ ਪੂਰਨ nlanes8 = VTraits<v_uint8>::vlanes();
        ਅਚਲ ਪੂਰਨ nlanes16 = VTraits<v_uint16>::vlanes();
        ਲਈ( ; i <= ਕੁੱਲ - nlanes8; i += nlanes8 )
        {
            v_uint8 idx = vx_ਲੱਦੋ(src + i);
            v_uint8 lo = v_lut(lut_lo, idx);
            v_uint8 hi = v_lut(lut_hi, idx);
            // Zip low and high bytes: [l0,h0,l1,h1,...] → ushort values on little-endian
            v_uint8 res0, res1;
            v_zip(lo, hi, res0, res1);
            v_store(dst + i, v_reinterpret_as_u16(res0));
            v_store(dst + i + nlanes16, v_reinterpret_as_u16(res1));
        }
#ਜੇ_ਅੰਤ
        ਲਈ( ; i < ਕੁੱਲ; i++ )
            dst[i] = lut[src[i]];
    }
    ਨਹੀਂਤਾਂ
    {
        ਲਈ( ਪੂਰਨ i = 0; i < ਕੁੱਲ; i += cn )
            ਲਈ( ਪੂਰਨ k = 0; k < cn; k++ )
                dst[i+k] = lut[src[i+k]*cn + k];
    }
}

#ਜੇ_ਅੰਤ // CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

CV_CPU_OPTIMIZATION_NAMESPACE_ਸਿਰਾ
} // namespace cv
