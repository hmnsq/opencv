// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html
// Copyright (C) 2026, BigVision LLC, all rights reserved.
// Third party copyrights are property of their respective owners.

#अन्तर्भाव "opencv2/core/hal/intrin.hpp"

नामाकाश cv {

CV_CPU_OPTIMIZATION_NAMESPACE_BEGIN

शून्य transpose_32bit_blocks_simd(स्थिरांक uchar* src, माप_प्रकार sstep,
                                 uchar* dst, माप_प्रकार dstep, Size sz);

#ifndef CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

स्थिर ओळीत शून्य transpose_32bit_blocks_tile(स्थिरांक uint32_t* src32, माप_प्रकार sstep_e,
                                               uint32_t* dst32, माप_प्रकार dstep_e,
                                               पूर्णांक i_lo, पूर्णांक i_hi, पूर्णांक j_lo, पूर्णांक j_hi)
{
    पूर्णांक i = i_lo;
#जर (CV_SIMD || CV_SIMD_SCALABLE)
    जर (VTraits<v_uint32>::vlanes() == 8)
    {
        साठी (; i + 8 <= i_hi; i += 8)
        {
            पूर्णांक j = j_lo;
            साठी (; j + 8 <= j_hi; j += 8)
            {
                v_uint32 r0 = vx_load(src32 + i + sstep_e*(j+0));
                v_uint32 r1 = vx_load(src32 + i + sstep_e*(j+1));
                v_uint32 r2 = vx_load(src32 + i + sstep_e*(j+2));
                v_uint32 r3 = vx_load(src32 + i + sstep_e*(j+3));
                v_uint32 r4 = vx_load(src32 + i + sstep_e*(j+4));
                v_uint32 r5 = vx_load(src32 + i + sstep_e*(j+5));
                v_uint32 r6 = vx_load(src32 + i + sstep_e*(j+6));
                v_uint32 r7 = vx_load(src32 + i + sstep_e*(j+7));
                v_uint32 a0, a1, a2, a3, a4, a5, a6, a7;
                v_transpose4x4(r0, r1, r2, r3, a0, a1, a2, a3);
                v_transpose4x4(r4, r5, r6, r7, a4, a5, a6, a7);
                v_uint32 b0 = v_combine_खालचा (a0, a4);
                v_uint32 b1 = v_combine_खालचा (a1, a5);
                v_uint32 b2 = v_combine_खालचा (a2, a6);
                v_uint32 b3 = v_combine_खालचा (a3, a7);
                v_uint32 b4 = v_combine_वरचा(a0, a4);
                v_uint32 b5 = v_combine_वरचा(a1, a5);
                v_uint32 b6 = v_combine_वरचा(a2, a6);
                v_uint32 b7 = v_combine_वरचा(a3, a7);
                vx_store(dst32 + dstep_e*(i+0) + j, b0);
                vx_store(dst32 + dstep_e*(i+1) + j, b1);
                vx_store(dst32 + dstep_e*(i+2) + j, b2);
                vx_store(dst32 + dstep_e*(i+3) + j, b3);
                vx_store(dst32 + dstep_e*(i+4) + j, b4);
                vx_store(dst32 + dstep_e*(i+5) + j, b5);
                vx_store(dst32 + dstep_e*(i+6) + j, b6);
                vx_store(dst32 + dstep_e*(i+7) + j, b7);
            }
            साठी (; j < j_hi; j++)
                साठी (पूर्णांक k = 0; k < 8; k++)
                    dst32[dstep_e*(i+k) + j] = src32[i + sstep_e*j + k];
        }
    }
    अन्यथा जर (VTraits<v_uint32>::vlanes() == 4)
    {
        साठी (; i + 4 <= i_hi; i += 4)
        {
            पूर्णांक j = j_lo;
            साठी (; j + 4 <= j_hi; j += 4)
            {
                v_uint32 r0 = vx_load(src32 + i + sstep_e*(j+0));
                v_uint32 r1 = vx_load(src32 + i + sstep_e*(j+1));
                v_uint32 r2 = vx_load(src32 + i + sstep_e*(j+2));
                v_uint32 r3 = vx_load(src32 + i + sstep_e*(j+3));
                v_uint32 o0, o1, o2, o3;
                v_transpose4x4(r0, r1, r2, r3, o0, o1, o2, o3);
                vx_store(dst32 + dstep_e*(i+0) + j, o0);
                vx_store(dst32 + dstep_e*(i+1) + j, o1);
                vx_store(dst32 + dstep_e*(i+2) + j, o2);
                vx_store(dst32 + dstep_e*(i+3) + j, o3);
            }
            साठी (; j < j_hi; j++)
                साठी (पूर्णांक k = 0; k < 4; k++)
                    dst32[dstep_e*(i+k) + j] = src32[i + sstep_e*j + k];
        }
    }
#endif
    साठी (; i < i_hi; i++)
        साठी (पूर्णांक j = j_lo; j < j_hi; j++)
            dst32[dstep_e*i + j] = src32[i + sstep_e*j];
}

शून्य transpose_32bit_blocks_simd(स्थिरांक uchar* src, माप_प्रकार sstep,
                                 uchar* dst, माप_प्रकार dstep, Size sz)
{
    स्थिरांक uint32_t* src32 = reinterpret_cast<स्थिरांक uint32_t*>(src);
    uint32_t* dst32 = reinterpret_cast<uint32_t*>(dst);
    स्थिरांक माप_प्रकार sstep_e = sstep / आकार(uint32_t);
    स्थिरांक माप_प्रकार dstep_e = dstep / आकार(uint32_t);
    स्थिरांक पूर्णांक m = sz.width, n = sz.height;

    // Single-threaded for small inputs to avoid parallel_for_ overhead.
    स्थिरांक int64_t kMinParallelElements = 64 * 1024;
    जर ((int64_t)m * n < kMinParallelElements)
    {
        transpose_32bit_blocks_tile(src32, sstep_e, dst32, dstep_e, 0, m, 0, n);
        परत;
    }

    स्थिरांक पूर्णांक TILE = 32;
    स्थिरांक पूर्णांक mtiles = (m + TILE - 1) / TILE;
    स्थिरांक पूर्णांक ntiles = (n + TILE - 1) / TILE;
    स्थिरांक int64_t totalTiles = (int64_t)mtiles * ntiles;
    parallel_for_(Range(0, (पूर्णांक)totalTiles), [&](स्थिरांक Range& r)
    {
        साठी (पूर्णांक idx = r.start; idx < r.end; idx++)
        {
            पूर्णांक ti = idx / ntiles;
            पूर्णांक tj = idx - ti * ntiles;
            पूर्णांक i_lo = ti * TILE, i_hi = std::min(i_lo + TILE, m);
            पूर्णांक j_lo = tj * TILE, j_hi = std::min(j_lo + TILE, n);
            transpose_32bit_blocks_tile(src32, sstep_e, dst32, dstep_e, i_lo, i_hi, j_lo, j_hi);
        }
    });
}

#endif // CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

CV_CPU_OPTIMIZATION_NAMESPACE_END

} // namespace cv
