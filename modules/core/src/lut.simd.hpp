// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html
// Copyright (C) 2026, Advanced Micro Devices, Inc., all rights reserved.

#समावेश "precomp.hpp"

// VBMI vpermb is the only x86 SIMD path faster than scalar for byte LUT.
// x86 gather (AVX2/SSE) is slower than scalar due to high gather latency.
// Non-x86 (NEON, etc.) benefits from v_lut implementation.
#अगर CV_AVX_512VBMI
#घोषणा CV_LUT8U_SIMD 1
#या_अगर (defined(CV_CPU_COMPILE_SSE) || defined(CV_CPU_COMPILE_SSE2) || defined(CV_CPU_COMPILE_SSE3) || \
       defined(CV_CPU_COMPILE_SSSE3) || defined(CV_CPU_COMPILE_SSE4_1) || defined(CV_CPU_COMPILE_SSE4_2) || \
       defined(CV_CPU_COMPILE_AVX) || defined(CV_CPU_COMPILE_AVX2) || defined(CV_CPU_COMPILE_AVX_512F) || \
       defined(CV_CPU_COMPILE_AVX512_COMMON) || defined(CV_CPU_COMPILE_AVX512_SKX) || \
       defined(CV_CPU_COMPILE_AVX512_CNL) || defined(CV_CPU_COMPILE_AVX512_CLX))
#घोषणा CV_LUT8U_SIMD 0
#या_अगर CV_SIMD || CV_SIMD_SCALABLE
#घोषणा CV_LUT8U_SIMD 1
#अन्यथा
#घोषणा CV_LUT8U_SIMD 0
#पूर्ण_अगर

namespace cv {
CV_CPU_OPTIMIZATION_NAMESPACE_BEGIN

व्योम LUT8u_( स्थिर uchar* src, स्थिर uchar* lut, uchar* dst, पूर्णांक len, पूर्णांक cn, पूर्णांक lutcn );
व्योम LUT16u_( स्थिर uchar* src, स्थिर ushort* lut, ushort* dst, पूर्णांक len, पूर्णांक cn, पूर्णांक lutcn );

#अगर_अघोषित CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

व्योम LUT8u_( स्थिर uchar* src, स्थिर uchar* lut, uchar* dst, पूर्णांक len, पूर्णांक cn, पूर्णांक lutcn )
{
    स्थिर पूर्णांक योग = len * cn;

    अगर( lutcn == 1 )
    {
        पूर्णांक i = 0;
#अगर CV_LUT8U_SIMD
        स्थिर पूर्णांक nlanes = VTraits<v_uint8>::vlanes();
        क्रम( ; i <= योग - nlanes; i += nlanes )
        {
            v_uint8 idx = vx_load(src + i);
            v_uint8 परिणाम = v_lut(lut, idx);
            v_store(dst + i, परिणाम);
        }
#पूर्ण_अगर
        क्रम( ; i < योग; i++ )
            dst[i] = lut[src[i]];
    }
#अगर CV_LUT8U_SIMD
    अन्यथा अगर( cn == 3 )
    {
        // Deinterleave the 3-channel LUT into per-channel tables
        uchar CV_DECL_ALIGNED(64) lut0[256], lut1[256], lut2[256];
        स्थिर पूर्णांक nlanes = VTraits<v_uint8>::vlanes();
        {
            अचिन्हित j = 0;
            क्रम( ; j + (अचिन्हित)nlanes <= 256; j += (अचिन्हित)nlanes )
            {
                v_uint8 a, b, c;
                v_load_deinterleave(lut + j * 3, a, b, c);
                v_store(lut0 + j, a);
                v_store(lut1 + j, b);
                v_store(lut2 + j, c);
            }
            क्रम( ; j < 256; j++ )
            {
                स्थिर अचिन्हित idx = j * 3;
                lut0[j] = lut[idx];
                lut1[j] = lut[idx + 1];
                lut2[j] = lut[idx + 2];
            }
        }
        पूर्णांक i = 0;
        क्रम( ; i <= योग - nlanes*3; i += nlanes*3 )
        {
            v_uint8 r, g, b;
            v_load_deinterleave(src + i, r, g, b);
            r = v_lut(lut0, r);
            g = v_lut(lut1, g);
            b = v_lut(lut2, b);
            v_store_interleave(dst + i, r, g, b);
        }
        क्रम( ; i < योग; i += 3 )
        {
            dst[i]   = lut0[src[i]];
            dst[i+1] = lut1[src[i+1]];
            dst[i+2] = lut2[src[i+2]];
        }
    }
#पूर्ण_अगर
    अन्यथा
    {
        क्रम( पूर्णांक i = 0; i < योग; i += cn )
            क्रम( पूर्णांक k = 0; k < cn; k++ )
                dst[i+k] = lut[src[i+k]*cn + k];
    }
}

व्योम LUT16u_( स्थिर uchar* src, स्थिर ushort* lut, ushort* dst, पूर्णांक len, पूर्णांक cn, पूर्णांक lutcn )
{
    स्थिर पूर्णांक योग = len * cn;

    अगर( lutcn == 1 )
    {
        पूर्णांक i = 0;
#अगर CV_LUT8U_SIMD
        // Split ushort LUT into low-byte and high-byte tables,
        // then use two byte v_lut (vpermb on VBMI) + interleave.
        uchar CV_DECL_ALIGNED(64) lut_lo[256], lut_hi[256];
        क्रम( पूर्णांक j = 0; j < 256; j++ )
        {
            lut_lo[j] = (uchar)(lut[j]);
            lut_hi[j] = (uchar)(lut[j] >> 8);
        }
        स्थिर पूर्णांक nlanes8 = VTraits<v_uint8>::vlanes();
        स्थिर पूर्णांक nlanes16 = VTraits<v_uint16>::vlanes();
        क्रम( ; i <= योग - nlanes8; i += nlanes8 )
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
#पूर्ण_अगर
        क्रम( ; i < योग; i++ )
            dst[i] = lut[src[i]];
    }
    अन्यथा
    {
        क्रम( पूर्णांक i = 0; i < योग; i += cn )
            क्रम( पूर्णांक k = 0; k < cn; k++ )
                dst[i+k] = lut[src[i+k]*cn + k];
    }
}

#पूर्ण_अगर // CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

CV_CPU_OPTIMIZATION_NAMESPACE_END
} // namespace cv
