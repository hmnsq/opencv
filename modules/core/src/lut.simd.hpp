// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html
// Copyright (C) 2026, Advanced Micro Devices, Inc., all rights reserved.

#ಅನ್ತರ್ಭಾವ "precomp.hpp"

// VBMI vpermb is the only x86 SIMD path faster than scalar for byte LUT.
// x86 gather (AVX2/SSE) is slower than scalar due to high gather latency.
// Non-x86 (NEON, etc.) benefits from v_lut implementation.
#ಆದರೆ CV_AVX_512VBMI
#ವ್ಯಾಖ್ಯಾನಿಸು CV_LUT8U_SIMD 1
#ಅಥವಾಆದರೆ (defined(CV_CPU_COMPILE_SSE) || defined(CV_CPU_COMPILE_SSE2) || defined(CV_CPU_COMPILE_SSE3) || \
       defined(CV_CPU_COMPILE_SSSE3) || defined(CV_CPU_COMPILE_SSE4_1) || defined(CV_CPU_COMPILE_SSE4_2) || \
       defined(CV_CPU_COMPILE_AVX) || defined(CV_CPU_COMPILE_AVX2) || defined(CV_CPU_COMPILE_AVX_512F) || \
       defined(CV_CPU_COMPILE_AVX512_COMMON) || defined(CV_CPU_COMPILE_AVX512_SKX) || \
       defined(CV_CPU_COMPILE_AVX512_CNL) || defined(CV_CPU_COMPILE_AVX512_CLX))
#ವ್ಯಾಖ್ಯಾನಿಸು CV_LUT8U_SIMD 0
#ಅಥವಾಆದರೆ CV_SIMD || CV_SIMD_SCALABLE
#ವ್ಯಾಖ್ಯಾನಿಸು CV_LUT8U_SIMD 1
#ಇಲ್ಲದಿದ್ದರೆ
#ವ್ಯಾಖ್ಯಾನಿಸು CV_LUT8U_SIMD 0
#ಆದರೆ_ಅಂತ್ಯ

ನಾಮಾಕಾಶ cv {
CV_CPU_OPTIMIZATION_NAMESPACE_BEGIN

ಶೂನ್ಯ LUT8u_( ಸ್ಥಿರಾಂಕ uchar* src, ಸ್ಥಿರಾಂಕ uchar* lut, uchar* dst, ಪೂರ್ಣಾಂಕ len, ಪೂರ್ಣಾಂಕ cn, ಪೂರ್ಣಾಂಕ lutcn );
ಶೂನ್ಯ LUT16u_( ಸ್ಥಿರಾಂಕ uchar* src, ಸ್ಥಿರಾಂಕ ushort* lut, ushort* dst, ಪೂರ್ಣಾಂಕ len, ಪೂರ್ಣಾಂಕ cn, ಪೂರ್ಣಾಂಕ lutcn );

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

ಶೂನ್ಯ LUT8u_( ಸ್ಥಿರಾಂಕ uchar* src, ಸ್ಥಿರಾಂಕ uchar* lut, uchar* dst, ಪೂರ್ಣಾಂಕ len, ಪೂರ್ಣಾಂಕ cn, ಪೂರ್ಣಾಂಕ lutcn )
{
    ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ ಒಟ್ಟು = len * cn;

    ಆದರೆ( lutcn == 1 )
    {
        ಪೂರ್ಣಾಂಕ i = 0;
#ಆದರೆ CV_LUT8U_SIMD
        ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ nlanes = VTraits<v_uint8>::vlanes();
        ಗಾಗಿ( ; i <= ಒಟ್ಟು - nlanes; i += nlanes )
        {
            v_uint8 idx = vx_ತುಂಬು(src + i);
            v_uint8 ಫಲಿತಾಂಶ = v_lut(lut, idx);
            v_store(dst + i, ಫಲಿತಾಂಶ);
        }
#ಆದರೆ_ಅಂತ್ಯ
        ಗಾಗಿ( ; i < ಒಟ್ಟು; i++ )
            dst[i] = lut[src[i]];
    }
#ಆದರೆ CV_LUT8U_SIMD
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ( cn == 3 )
    {
        // Deinterleave the 3-channel LUT into per-channel tables
        uchar CV_DECL_ALIGNED(64) lut0[256], lut1[256], lut2[256];
        ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ nlanes = VTraits<v_uint8>::vlanes();
        {
            ಚಿಹ್ನೆರಹಿತ j = 0;
            ಗಾಗಿ( ; j + (ಚಿಹ್ನೆರಹಿತ)nlanes <= 256; j += (ಚಿಹ್ನೆರಹಿತ)nlanes )
            {
                v_uint8 a, b, c;
                v_ತುಂಬು_deinterleave(lut + j * 3, a, b, c);
                v_store(lut0 + j, a);
                v_store(lut1 + j, b);
                v_store(lut2 + j, c);
            }
            ಗಾಗಿ( ; j < 256; j++ )
            {
                ಸ್ಥಿರಾಂಕ ಚಿಹ್ನೆರಹಿತ idx = j * 3;
                lut0[j] = lut[idx];
                lut1[j] = lut[idx + 1];
                lut2[j] = lut[idx + 2];
            }
        }
        ಪೂರ್ಣಾಂಕ i = 0;
        ಗಾಗಿ( ; i <= ಒಟ್ಟು - nlanes*3; i += nlanes*3 )
        {
            v_uint8 r, g, b;
            v_ತುಂಬು_deinterleave(src + i, r, g, b);
            r = v_lut(lut0, r);
            g = v_lut(lut1, g);
            b = v_lut(lut2, b);
            v_store_interleave(dst + i, r, g, b);
        }
        ಗಾಗಿ( ; i < ಒಟ್ಟು; i += 3 )
        {
            dst[i]   = lut0[src[i]];
            dst[i+1] = lut1[src[i+1]];
            dst[i+2] = lut2[src[i+2]];
        }
    }
#ಆದರೆ_ಅಂತ್ಯ
    ಇಲ್ಲದಿದ್ದರೆ
    {
        ಗಾಗಿ( ಪೂರ್ಣಾಂಕ i = 0; i < ಒಟ್ಟು; i += cn )
            ಗಾಗಿ( ಪೂರ್ಣಾಂಕ k = 0; k < cn; k++ )
                dst[i+k] = lut[src[i+k]*cn + k];
    }
}

ಶೂನ್ಯ LUT16u_( ಸ್ಥಿರಾಂಕ uchar* src, ಸ್ಥಿರಾಂಕ ushort* lut, ushort* dst, ಪೂರ್ಣಾಂಕ len, ಪೂರ್ಣಾಂಕ cn, ಪೂರ್ಣಾಂಕ lutcn )
{
    ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ ಒಟ್ಟು = len * cn;

    ಆದರೆ( lutcn == 1 )
    {
        ಪೂರ್ಣಾಂಕ i = 0;
#ಆದರೆ CV_LUT8U_SIMD
        // Split ushort LUT into low-byte and high-byte tables,
        // then use two byte v_lut (vpermb on VBMI) + interleave.
        uchar CV_DECL_ALIGNED(64) lut_lo[256], lut_hi[256];
        ಗಾಗಿ( ಪೂರ್ಣಾಂಕ j = 0; j < 256; j++ )
        {
            lut_lo[j] = (uchar)(lut[j]);
            lut_hi[j] = (uchar)(lut[j] >> 8);
        }
        ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ nlanes8 = VTraits<v_uint8>::vlanes();
        ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ nlanes16 = VTraits<v_uint16>::vlanes();
        ಗಾಗಿ( ; i <= ಒಟ್ಟು - nlanes8; i += nlanes8 )
        {
            v_uint8 idx = vx_ತುಂಬು(src + i);
            v_uint8 lo = v_lut(lut_lo, idx);
            v_uint8 hi = v_lut(lut_hi, idx);
            // Zip low and high bytes: [l0,h0,l1,h1,...] → ushort values on little-endian
            v_uint8 res0, res1;
            v_zip(lo, hi, res0, res1);
            v_store(dst + i, v_reinterpret_as_u16(res0));
            v_store(dst + i + nlanes16, v_reinterpret_as_u16(res1));
        }
#ಆದರೆ_ಅಂತ್ಯ
        ಗಾಗಿ( ; i < ಒಟ್ಟು; i++ )
            dst[i] = lut[src[i]];
    }
    ಇಲ್ಲದಿದ್ದರೆ
    {
        ಗಾಗಿ( ಪೂರ್ಣಾಂಕ i = 0; i < ಒಟ್ಟು; i += cn )
            ಗಾಗಿ( ಪೂರ್ಣಾಂಕ k = 0; k < cn; k++ )
                dst[i+k] = lut[src[i+k]*cn + k];
    }
}

#ಆದರೆ_ಅಂತ್ಯ // CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

CV_CPU_OPTIMIZATION_NAMESPACE_ಕೊನೆ
} // namespace cv
