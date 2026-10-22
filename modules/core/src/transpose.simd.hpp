// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html
// Copyright (C) 2026, BigVision LLC, all rights reserved.
// Third party copyrights are property of their respective owners.

#అన్తర్భావ "opencv2/core/hal/intrin.hpp"

నామాకాశ cv {

CV_CPU_OPTIMIZATION_NAMESPACE_BEGIN

శూన్య transpose_32bit_ఖండాలు_simd(స్థిరరాశి uchar* src, size_t sstep,
                                 uchar* dst, size_t dstep, కొలత sz);

#నిర్వచితం_కాకపోతే CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

స్థిర అంతఃవరుస శూన్య transpose_32bit_ఖండాలు_tile(స్థిరరాశి uint32_t* src32, size_t sstep_e,
                                               uint32_t* dst32, size_t dstep_e,
                                               పూర్ణాంకం i_lo, పూర్ణాంకం i_hi, పూర్ణాంకం j_lo, పూర్ణాంకం j_hi)
{
    పూర్ణాంకం i = i_lo;
#ఒకవేళ (CV_SIMD || CV_SIMD_SCALABLE)
    ఒకవేళ (VTraits<v_uint32>::vlanes() == 8)
    {
        కొరకు (; i + 8 <= i_hi; i += 8)
        {
            పూర్ణాంకం j = j_lo;
            కొరకు (; j + 8 <= j_hi; j += 8)
            {
                v_uint32 r0 = vx_ఎక్కించు(src32 + i + sstep_e*(j+0));
                v_uint32 r1 = vx_ఎక్కించు(src32 + i + sstep_e*(j+1));
                v_uint32 r2 = vx_ఎక్కించు(src32 + i + sstep_e*(j+2));
                v_uint32 r3 = vx_ఎక్కించు(src32 + i + sstep_e*(j+3));
                v_uint32 r4 = vx_ఎక్కించు(src32 + i + sstep_e*(j+4));
                v_uint32 r5 = vx_ఎక్కించు(src32 + i + sstep_e*(j+5));
                v_uint32 r6 = vx_ఎక్కించు(src32 + i + sstep_e*(j+6));
                v_uint32 r7 = vx_ఎక్కించు(src32 + i + sstep_e*(j+7));
                v_uint32 a0, a1, a2, a3, a4, a5, a6, a7;
                v_transpose4x4(r0, r1, r2, r3, a0, a1, a2, a3);
                v_transpose4x4(r4, r5, r6, r7, a4, a5, a6, a7);
                v_uint32 b0 = v_combine_దిగువ (a0, a4);
                v_uint32 b1 = v_combine_దిగువ (a1, a5);
                v_uint32 b2 = v_combine_దిగువ (a2, a6);
                v_uint32 b3 = v_combine_దిగువ (a3, a7);
                v_uint32 b4 = v_combine_ఎగువ(a0, a4);
                v_uint32 b5 = v_combine_ఎగువ(a1, a5);
                v_uint32 b6 = v_combine_ఎగువ(a2, a6);
                v_uint32 b7 = v_combine_ఎగువ(a3, a7);
                vx_store(dst32 + dstep_e*(i+0) + j, b0);
                vx_store(dst32 + dstep_e*(i+1) + j, b1);
                vx_store(dst32 + dstep_e*(i+2) + j, b2);
                vx_store(dst32 + dstep_e*(i+3) + j, b3);
                vx_store(dst32 + dstep_e*(i+4) + j, b4);
                vx_store(dst32 + dstep_e*(i+5) + j, b5);
                vx_store(dst32 + dstep_e*(i+6) + j, b6);
                vx_store(dst32 + dstep_e*(i+7) + j, b7);
            }
            కొరకు (; j < j_hi; j++)
                కొరకు (పూర్ణాంకం k = 0; k < 8; k++)
                    dst32[dstep_e*(i+k) + j] = src32[i + sstep_e*j + k];
        }
    }
    లేకపోతే ఒకవేళ (VTraits<v_uint32>::vlanes() == 4)
    {
        కొరకు (; i + 4 <= i_hi; i += 4)
        {
            పూర్ణాంకం j = j_lo;
            కొరకు (; j + 4 <= j_hi; j += 4)
            {
                v_uint32 r0 = vx_ఎక్కించు(src32 + i + sstep_e*(j+0));
                v_uint32 r1 = vx_ఎక్కించు(src32 + i + sstep_e*(j+1));
                v_uint32 r2 = vx_ఎక్కించు(src32 + i + sstep_e*(j+2));
                v_uint32 r3 = vx_ఎక్కించు(src32 + i + sstep_e*(j+3));
                v_uint32 o0, o1, o2, o3;
                v_transpose4x4(r0, r1, r2, r3, o0, o1, o2, o3);
                vx_store(dst32 + dstep_e*(i+0) + j, o0);
                vx_store(dst32 + dstep_e*(i+1) + j, o1);
                vx_store(dst32 + dstep_e*(i+2) + j, o2);
                vx_store(dst32 + dstep_e*(i+3) + j, o3);
            }
            కొరకు (; j < j_hi; j++)
                కొరకు (పూర్ణాంకం k = 0; k < 4; k++)
                    dst32[dstep_e*(i+k) + j] = src32[i + sstep_e*j + k];
        }
    }
#అయితే_ముగింపు
    కొరకు (; i < i_hi; i++)
        కొరకు (పూర్ణాంకం j = j_lo; j < j_hi; j++)
            dst32[dstep_e*i + j] = src32[i + sstep_e*j];
}

శూన్య transpose_32bit_ఖండాలు_simd(స్థిరరాశి uchar* src, size_t sstep,
                                 uchar* dst, size_t dstep, కొలత sz)
{
    స్థిరరాశి uint32_t* src32 = reinterpret_cast<స్థిరరాశి uint32_t*>(src);
    uint32_t* dst32 = reinterpret_cast<uint32_t*>(dst);
    స్థిరరాశి size_t sstep_e = sstep / పరిమాణం(uint32_t);
    స్థిరరాశి size_t dstep_e = dstep / పరిమాణం(uint32_t);
    స్థిరరాశి పూర్ణాంకం m = sz.వెడల్పు, n = sz.ఎత్తు;

    // Single-threaded for small inputs to avoid parallel_for_ overhead.
    స్థిరరాశి int64_t kMinParallelElements = 64 * 1024;
    ఒకవేళ ((int64_t)m * n < kMinParallelElements)
    {
        transpose_32bit_ఖండాలు_tile(src32, sstep_e, dst32, dstep_e, 0, m, 0, n);
        తిరిగి;
    }

    స్థిరరాశి పూర్ణాంకం TILE = 32;
    స్థిరరాశి పూర్ణాంకం mtiles = (m + TILE - 1) / TILE;
    స్థిరరాశి పూర్ణాంకం ntiles = (n + TILE - 1) / TILE;
    స్థిరరాశి int64_t మొత్తం_Tiles = (int64_t)mtiles * ntiles;
    parallel_for_(Range(0, (పూర్ణాంకం)మొత్తం_Tiles), [&](స్థిరరాశి Range& r)
    {
        కొరకు (పూర్ణాంకం idx = r.ప్రారంభం; idx < r.end; idx++)
        {
            పూర్ణాంకం ti = idx / ntiles;
            పూర్ణాంకం tj = idx - ti * ntiles;
            పూర్ణాంకం i_lo = ti * TILE, i_hi = std::min(i_lo + TILE, m);
            పూర్ణాంకం j_lo = tj * TILE, j_hi = std::min(j_lo + TILE, n);
            transpose_32bit_ఖండాలు_tile(src32, sstep_e, dst32, dstep_e, i_lo, i_hi, j_lo, j_hi);
        }
    });
}

#అయితే_ముగింపు // CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

CV_CPU_OPTIMIZATION_NAMESPACE_చివర

} // namespace cv
