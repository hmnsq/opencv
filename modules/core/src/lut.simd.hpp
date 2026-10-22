// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html
// Copyright (C) 2026, Advanced Micro Devices, Inc., all rights reserved.

#அந்தர்பாவ "precomp.hpp"

// VBMI vpermb is the only x86 SIMD path faster than scalar for byte LUT.
// x86 gather (AVX2/SSE) is slower than scalar due to high gather latency.
// Non-x86 (NEON, etc.) benefits from v_lut implementation.
#எனில் CV_AVX_512VBMI
#define CV_LUT8U_SIMD 1
#அல்லதெனில் (defined(CV_CPU_COMPILE_SSE) || defined(CV_CPU_COMPILE_SSE2) || defined(CV_CPU_COMPILE_SSE3) || \
       defined(CV_CPU_COMPILE_SSSE3) || defined(CV_CPU_COMPILE_SSE4_1) || defined(CV_CPU_COMPILE_SSE4_2) || \
       defined(CV_CPU_COMPILE_AVX) || defined(CV_CPU_COMPILE_AVX2) || defined(CV_CPU_COMPILE_AVX_512F) || \
       defined(CV_CPU_COMPILE_AVX512_COMMON) || defined(CV_CPU_COMPILE_AVX512_SKX) || \
       defined(CV_CPU_COMPILE_AVX512_CNL) || defined(CV_CPU_COMPILE_AVX512_CLX))
#define CV_LUT8U_SIMD 0
#அல்லதெனில் CV_SIMD || CV_SIMD_SCALABLE
#define CV_LUT8U_SIMD 1
#இல்லையெனில்
#define CV_LUT8U_SIMD 0
#endif

நாமாகாஶ cv {
CV_CPU_OPTIMIZATION_NAMESPACE_BEGIN

வெறுமை LUT8u_( மாறிலி uchar* src, மாறிலி uchar* lut, uchar* dst, முழு len, முழு cn, முழு lutcn );
வெறுமை LUT16u_( மாறிலி uchar* src, மாறிலி ushort* lut, ushort* dst, முழு len, முழு cn, முழு lutcn );

#ifndef CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

வெறுமை LUT8u_( மாறிலி uchar* src, மாறிலி uchar* lut, uchar* dst, முழு len, முழு cn, முழு lutcn )
{
    மாறிலி முழு total = len * cn;

    எனில்( lutcn == 1 )
    {
        முழு i = 0;
#எனில் CV_LUT8U_SIMD
        மாறிலி முழு nlanes = VTraits<v_uint8>::vlanes();
        க்கு( ; i <= total - nlanes; i += nlanes )
        {
            v_uint8 idx = vx_load(src + i);
            v_uint8 result = v_lut(lut, idx);
            v_store(dst + i, result);
        }
#endif
        க்கு( ; i < total; i++ )
            dst[i] = lut[src[i]];
    }
#எனில் CV_LUT8U_SIMD
    இல்லையெனில் எனில்( cn == 3 )
    {
        // Deinterleave the 3-channel LUT into per-channel tables
        uchar CV_DECL_ALIGNED(64) lut0[256], lut1[256], lut2[256];
        மாறிலி முழு nlanes = VTraits<v_uint8>::vlanes();
        {
            unsigned j = 0;
            க்கு( ; j + (unsigned)nlanes <= 256; j += (unsigned)nlanes )
            {
                v_uint8 a, b, c;
                v_load_deinterleave(lut + j * 3, a, b, c);
                v_store(lut0 + j, a);
                v_store(lut1 + j, b);
                v_store(lut2 + j, c);
            }
            க்கு( ; j < 256; j++ )
            {
                மாறிலி unsigned idx = j * 3;
                lut0[j] = lut[idx];
                lut1[j] = lut[idx + 1];
                lut2[j] = lut[idx + 2];
            }
        }
        முழு i = 0;
        க்கு( ; i <= total - nlanes*3; i += nlanes*3 )
        {
            v_uint8 r, g, b;
            v_load_deinterleave(src + i, r, g, b);
            r = v_lut(lut0, r);
            g = v_lut(lut1, g);
            b = v_lut(lut2, b);
            v_store_interleave(dst + i, r, g, b);
        }
        க்கு( ; i < total; i += 3 )
        {
            dst[i]   = lut0[src[i]];
            dst[i+1] = lut1[src[i+1]];
            dst[i+2] = lut2[src[i+2]];
        }
    }
#endif
    இல்லையெனில்
    {
        க்கு( முழு i = 0; i < total; i += cn )
            க்கு( முழு k = 0; k < cn; k++ )
                dst[i+k] = lut[src[i+k]*cn + k];
    }
}

வெறுமை LUT16u_( மாறிலி uchar* src, மாறிலி ushort* lut, ushort* dst, முழு len, முழு cn, முழு lutcn )
{
    மாறிலி முழு total = len * cn;

    எனில்( lutcn == 1 )
    {
        முழு i = 0;
#எனில் CV_LUT8U_SIMD
        // Split ushort LUT into low-byte and high-byte tables,
        // then use two byte v_lut (vpermb on VBMI) + interleave.
        uchar CV_DECL_ALIGNED(64) lut_lo[256], lut_hi[256];
        க்கு( முழு j = 0; j < 256; j++ )
        {
            lut_lo[j] = (uchar)(lut[j]);
            lut_hi[j] = (uchar)(lut[j] >> 8);
        }
        மாறிலி முழு nlanes8 = VTraits<v_uint8>::vlanes();
        மாறிலி முழு nlanes16 = VTraits<v_uint16>::vlanes();
        க்கு( ; i <= total - nlanes8; i += nlanes8 )
        {
            v_uint8 idx = vx_load(src + i);
            v_uint8 lo = v_lut(lut_lo, idx);
            v_uint8 hi = v_lut(lut_hi, idx);
            // Zip low and high bytes: [l0,h0,l1,h1,...] → ushort values on little-endian
            v_uint8 res0, res1;
            v_zip(lo, hi, res0, res1);
            v_store(dst + i, v_reinterpret_as_u16(res0));
            v_store(dst + i + nlanes16, v_reinterpret_as_u16(res1));
        }
#endif
        க்கு( ; i < total; i++ )
            dst[i] = lut[src[i]];
    }
    இல்லையெனில்
    {
        க்கு( முழு i = 0; i < total; i += cn )
            க்கு( முழு k = 0; k < cn; k++ )
                dst[i+k] = lut[src[i+k]*cn + k];
    }
}

#endif // CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

CV_CPU_OPTIMIZATION_NAMESPACE_END
} // namespace cv
