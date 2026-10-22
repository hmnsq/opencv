// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html
// Copyright (C) 2026, BigVision LLC, all rights reserved.
// Third party copyrights are property of their respective owners.

#अन्तर्भाव "opencv2/core/hal/intrin.hpp"

नामाकाश cv {

CV_CPU_OPTIMIZATION_NAMESPACE_BEGIN

शून्य transpose_32bit_खण्डानि_simd(ध्रुव uchar* src, size_t sstep,
                                 uchar* dst, size_t dstep, आकार sz);

#यदि_अपरिभाषितम् CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

स्थिर अन्तःस्थ शून्य transpose_32bit_खण्डानि_tile(ध्रुव uint32_t* src32, size_t sstep_e,
                                               uint32_t* dst32, size_t dstep_e,
                                               पूर्णाङ्क i_lo, पूर्णाङ्क i_hi, पूर्णाङ्क j_lo, पूर्णाङ्क j_hi)
{
    पूर्णाङ्क i = i_lo;
#यदि (CV_SIMD || CV_SIMD_SCALABLE)
    यदि (VTraits<v_uint32>::vlanes() == 8)
    {
        प्रति (; i + 8 <= i_hi; i += 8)
        {
            पूर्णाङ्क j = j_lo;
            प्रति (; j + 8 <= j_hi; j += 8)
            {
                v_uint32 r0 = vx_आरोपय(src32 + i + sstep_e*(j+0));
                v_uint32 r1 = vx_आरोपय(src32 + i + sstep_e*(j+1));
                v_uint32 r2 = vx_आरोपय(src32 + i + sstep_e*(j+2));
                v_uint32 r3 = vx_आरोपय(src32 + i + sstep_e*(j+3));
                v_uint32 r4 = vx_आरोपय(src32 + i + sstep_e*(j+4));
                v_uint32 r5 = vx_आरोपय(src32 + i + sstep_e*(j+5));
                v_uint32 r6 = vx_आरोपय(src32 + i + sstep_e*(j+6));
                v_uint32 r7 = vx_आरोपय(src32 + i + sstep_e*(j+7));
                v_uint32 a0, a1, a2, a3, a4, a5, a6, a7;
                v_transpose4x4(r0, r1, r2, r3, a0, a1, a2, a3);
                v_transpose4x4(r4, r5, r6, r7, a4, a5, a6, a7);
                v_uint32 b0 = v_combine_नीचः (a0, a4);
                v_uint32 b1 = v_combine_नीचः (a1, a5);
                v_uint32 b2 = v_combine_नीचः (a2, a6);
                v_uint32 b3 = v_combine_नीचः (a3, a7);
                v_uint32 b4 = v_combine_उच्चः(a0, a4);
                v_uint32 b5 = v_combine_उच्चः(a1, a5);
                v_uint32 b6 = v_combine_उच्चः(a2, a6);
                v_uint32 b7 = v_combine_उच्चः(a3, a7);
                vx_store(dst32 + dstep_e*(i+0) + j, b0);
                vx_store(dst32 + dstep_e*(i+1) + j, b1);
                vx_store(dst32 + dstep_e*(i+2) + j, b2);
                vx_store(dst32 + dstep_e*(i+3) + j, b3);
                vx_store(dst32 + dstep_e*(i+4) + j, b4);
                vx_store(dst32 + dstep_e*(i+5) + j, b5);
                vx_store(dst32 + dstep_e*(i+6) + j, b6);
                vx_store(dst32 + dstep_e*(i+7) + j, b7);
            }
            प्रति (; j < j_hi; j++)
                प्रति (पूर्णाङ्क k = 0; k < 8; k++)
                    dst32[dstep_e*(i+k) + j] = src32[i + sstep_e*j + k];
        }
    }
    अन्यथा यदि (VTraits<v_uint32>::vlanes() == 4)
    {
        प्रति (; i + 4 <= i_hi; i += 4)
        {
            पूर्णाङ्क j = j_lo;
            प्रति (; j + 4 <= j_hi; j += 4)
            {
                v_uint32 r0 = vx_आरोपय(src32 + i + sstep_e*(j+0));
                v_uint32 r1 = vx_आरोपय(src32 + i + sstep_e*(j+1));
                v_uint32 r2 = vx_आरोपय(src32 + i + sstep_e*(j+2));
                v_uint32 r3 = vx_आरोपय(src32 + i + sstep_e*(j+3));
                v_uint32 o0, o1, o2, o3;
                v_transpose4x4(r0, r1, r2, r3, o0, o1, o2, o3);
                vx_store(dst32 + dstep_e*(i+0) + j, o0);
                vx_store(dst32 + dstep_e*(i+1) + j, o1);
                vx_store(dst32 + dstep_e*(i+2) + j, o2);
                vx_store(dst32 + dstep_e*(i+3) + j, o3);
            }
            प्रति (; j < j_hi; j++)
                प्रति (पूर्णाङ्क k = 0; k < 4; k++)
                    dst32[dstep_e*(i+k) + j] = src32[i + sstep_e*j + k];
        }
    }
#यदि_अन्तः
    प्रति (; i < i_hi; i++)
        प्रति (पूर्णाङ्क j = j_lo; j < j_hi; j++)
            dst32[dstep_e*i + j] = src32[i + sstep_e*j];
}

शून्य transpose_32bit_खण्डानि_simd(ध्रुव uchar* src, size_t sstep,
                                 uchar* dst, size_t dstep, आकार sz)
{
    ध्रुव uint32_t* src32 = reinterpret_cast<ध्रुव uint32_t*>(src);
    uint32_t* dst32 = reinterpret_cast<uint32_t*>(dst);
    ध्रुव size_t sstep_e = sstep / परिमाण(uint32_t);
    ध्रुव size_t dstep_e = dstep / परिमाण(uint32_t);
    ध्रुव पूर्णाङ्क m = sz.विस्तारः, n = sz.height;

    // Single-threaded for small inputs to avoid parallel_for_ overhead.
    ध्रुव int64_t kMinParallelElements = 64 * 1024;
    यदि ((int64_t)m * n < kMinParallelElements)
    {
        transpose_32bit_खण्डानि_tile(src32, sstep_e, dst32, dstep_e, 0, m, 0, n);
        प्रत्यागम;
    }

    ध्रुव पूर्णाङ्क TILE = 32;
    ध्रुव पूर्णाङ्क mtiles = (m + TILE - 1) / TILE;
    ध्रुव पूर्णाङ्क ntiles = (n + TILE - 1) / TILE;
    ध्रुव int64_t योग_Tiles = (int64_t)mtiles * ntiles;
    parallel_for_(Range(0, (पूर्णाङ्क)योग_Tiles), [&](ध्रुव Range& r)
    {
        प्रति (पूर्णाङ्क idx = r.start; idx < r.end; idx++)
        {
            पूर्णाङ्क ti = idx / ntiles;
            पूर्णाङ्क tj = idx - ti * ntiles;
            पूर्णाङ्क i_lo = ti * TILE, i_hi = std::min(i_lo + TILE, m);
            पूर्णाङ्क j_lo = tj * TILE, j_hi = std::min(j_lo + TILE, n);
            transpose_32bit_खण्डानि_tile(src32, sstep_e, dst32, dstep_e, i_lo, i_hi, j_lo, j_hi);
        }
    });
}

#यदि_अन्तः // CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

CV_CPU_OPTIMIZATION_NAMESPACE_अन्तः

} // namespace cv
