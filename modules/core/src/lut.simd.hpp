// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html
// Copyright (C) 2026, Advanced Micro Devices, Inc., all rights reserved.

#అన్తర్భావ "precomp.hpp"

// VBMI vpermb is the only x86 SIMD path faster than scalar for byte LUT.
// x86 gather (AVX2/SSE) is slower than scalar due to high gather latency.
// Non-x86 (NEON, etc.) benefits from v_lut implementation.
#ఒకవేళ CV_AVX_512VBMI
#నిర్వచించు CV_LUT8U_SIMD 1
#లేదావేళ (defined(CV_CPU_COMPILE_SSE) || defined(CV_CPU_COMPILE_SSE2) || defined(CV_CPU_COMPILE_SSE3) || \
       defined(CV_CPU_COMPILE_SSSE3) || defined(CV_CPU_COMPILE_SSE4_1) || defined(CV_CPU_COMPILE_SSE4_2) || \
       defined(CV_CPU_COMPILE_AVX) || defined(CV_CPU_COMPILE_AVX2) || defined(CV_CPU_COMPILE_AVX_512F) || \
       defined(CV_CPU_COMPILE_AVX512_COMMON) || defined(CV_CPU_COMPILE_AVX512_SKX) || \
       defined(CV_CPU_COMPILE_AVX512_CNL) || defined(CV_CPU_COMPILE_AVX512_CLX))
#నిర్వచించు CV_LUT8U_SIMD 0
#లేదావేళ CV_SIMD || CV_SIMD_SCALABLE
#నిర్వచించు CV_LUT8U_SIMD 1
#లేకపోతే
#నిర్వచించు CV_LUT8U_SIMD 0
#అయితే_ముగింపు

నామాకాశ cv {
CV_CPU_OPTIMIZATION_NAMESPACE_BEGIN

శూన్య LUT8u_( స్థిరరాశి uchar* src, స్థిరరాశి uchar* lut, uchar* dst, పూర్ణాంకం len, పూర్ణాంకం cn, పూర్ణాంకం lutcn );
శూన్య LUT16u_( స్థిరరాశి uchar* src, స్థిరరాశి ushort* lut, ushort* dst, పూర్ణాంకం len, పూర్ణాంకం cn, పూర్ణాంకం lutcn );

#నిర్వచితం_కాకపోతే CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

శూన్య LUT8u_( స్థిరరాశి uchar* src, స్థిరరాశి uchar* lut, uchar* dst, పూర్ణాంకం len, పూర్ణాంకం cn, పూర్ణాంకం lutcn )
{
    స్థిరరాశి పూర్ణాంకం మొత్తం = len * cn;

    ఒకవేళ( lutcn == 1 )
    {
        పూర్ణాంకం i = 0;
#ఒకవేళ CV_LUT8U_SIMD
        స్థిరరాశి పూర్ణాంకం nlanes = VTraits<v_uint8>::vlanes();
        కొరకు( ; i <= మొత్తం - nlanes; i += nlanes )
        {
            v_uint8 idx = vx_ఎక్కించు(src + i);
            v_uint8 ఫలితం = v_lut(lut, idx);
            v_store(dst + i, ఫలితం);
        }
#అయితే_ముగింపు
        కొరకు( ; i < మొత్తం; i++ )
            dst[i] = lut[src[i]];
    }
#ఒకవేళ CV_LUT8U_SIMD
    లేకపోతే ఒకవేళ( cn == 3 )
    {
        // Deinterleave the 3-channel LUT into per-channel tables
        uchar CV_DECL_ALIGNED(64) lut0[256], lut1[256], lut2[256];
        స్థిరరాశి పూర్ణాంకం nlanes = VTraits<v_uint8>::vlanes();
        {
            గుర్తులేని j = 0;
            కొరకు( ; j + (గుర్తులేని)nlanes <= 256; j += (గుర్తులేని)nlanes )
            {
                v_uint8 a, b, c;
                v_ఎక్కించు_deinterleave(lut + j * 3, a, b, c);
                v_store(lut0 + j, a);
                v_store(lut1 + j, b);
                v_store(lut2 + j, c);
            }
            కొరకు( ; j < 256; j++ )
            {
                స్థిరరాశి గుర్తులేని idx = j * 3;
                lut0[j] = lut[idx];
                lut1[j] = lut[idx + 1];
                lut2[j] = lut[idx + 2];
            }
        }
        పూర్ణాంకం i = 0;
        కొరకు( ; i <= మొత్తం - nlanes*3; i += nlanes*3 )
        {
            v_uint8 r, g, b;
            v_ఎక్కించు_deinterleave(src + i, r, g, b);
            r = v_lut(lut0, r);
            g = v_lut(lut1, g);
            b = v_lut(lut2, b);
            v_store_interleave(dst + i, r, g, b);
        }
        కొరకు( ; i < మొత్తం; i += 3 )
        {
            dst[i]   = lut0[src[i]];
            dst[i+1] = lut1[src[i+1]];
            dst[i+2] = lut2[src[i+2]];
        }
    }
#అయితే_ముగింపు
    లేకపోతే
    {
        కొరకు( పూర్ణాంకం i = 0; i < మొత్తం; i += cn )
            కొరకు( పూర్ణాంకం k = 0; k < cn; k++ )
                dst[i+k] = lut[src[i+k]*cn + k];
    }
}

శూన్య LUT16u_( స్థిరరాశి uchar* src, స్థిరరాశి ushort* lut, ushort* dst, పూర్ణాంకం len, పూర్ణాంకం cn, పూర్ణాంకం lutcn )
{
    స్థిరరాశి పూర్ణాంకం మొత్తం = len * cn;

    ఒకవేళ( lutcn == 1 )
    {
        పూర్ణాంకం i = 0;
#ఒకవేళ CV_LUT8U_SIMD
        // Split ushort LUT into low-byte and high-byte tables,
        // then use two byte v_lut (vpermb on VBMI) + interleave.
        uchar CV_DECL_ALIGNED(64) lut_lo[256], lut_hi[256];
        కొరకు( పూర్ణాంకం j = 0; j < 256; j++ )
        {
            lut_lo[j] = (uchar)(lut[j]);
            lut_hi[j] = (uchar)(lut[j] >> 8);
        }
        స్థిరరాశి పూర్ణాంకం nlanes8 = VTraits<v_uint8>::vlanes();
        స్థిరరాశి పూర్ణాంకం nlanes16 = VTraits<v_uint16>::vlanes();
        కొరకు( ; i <= మొత్తం - nlanes8; i += nlanes8 )
        {
            v_uint8 idx = vx_ఎక్కించు(src + i);
            v_uint8 lo = v_lut(lut_lo, idx);
            v_uint8 hi = v_lut(lut_hi, idx);
            // Zip low and high bytes: [l0,h0,l1,h1,...] → ushort values on little-endian
            v_uint8 res0, res1;
            v_zip(lo, hi, res0, res1);
            v_store(dst + i, v_reinterpret_as_u16(res0));
            v_store(dst + i + nlanes16, v_reinterpret_as_u16(res1));
        }
#అయితే_ముగింపు
        కొరకు( ; i < మొత్తం; i++ )
            dst[i] = lut[src[i]];
    }
    లేకపోతే
    {
        కొరకు( పూర్ణాంకం i = 0; i < మొత్తం; i += cn )
            కొరకు( పూర్ణాంకం k = 0; k < cn; k++ )
                dst[i+k] = lut[src[i+k]*cn + k];
    }
}

#అయితే_ముగింపు // CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

CV_CPU_OPTIMIZATION_NAMESPACE_చివర
} // namespace cv
