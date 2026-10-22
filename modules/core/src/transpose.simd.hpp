// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html
// Copyright (C) 2026, BigVision LLC, all rights reserved.
// Third party copyrights are property of their respective owners.

#समावेश "opencv2/core/hal/intrin.hpp"

namespace cv {

CV_CPU_OPTIMIZATION_NAMESPACE_BEGIN

व्योम transpose_32bit_blocks_simd(स्थिर uchar* src, माप_प्रकार sstep,
                                 uchar* dst, माप_प्रकार dstep, आकार sz);

#अगर_अघोषित CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

अटल अंतरभूत व्योम transpose_32bit_blocks_tile(स्थिर uint32_t* src32, माप_प्रकार sstep_e,
                                               uint32_t* dst32, माप_प्रकार dstep_e,
                                               पूर्णांक i_lo, पूर्णांक i_hi, पूर्णांक j_lo, पूर्णांक j_hi)
{
    पूर्णांक i = i_lo;
#अगर (CV_SIMD || CV_SIMD_SCALABLE)
    अगर (VTraits<v_uint32>::vlanes() == 8)
    {
        क्रम (; i + 8 <= i_hi; i += 8)
        {
            पूर्णांक j = j_lo;
            क्रम (; j + 8 <= j_hi; j += 8)
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
                v_uint32 b0 = v_combine_निचला (a0, a4);
                v_uint32 b1 = v_combine_निचला (a1, a5);
                v_uint32 b2 = v_combine_निचला (a2, a6);
                v_uint32 b3 = v_combine_निचला (a3, a7);
                v_uint32 b4 = v_combine_ऊपरी(a0, a4);
                v_uint32 b5 = v_combine_ऊपरी(a1, a5);
                v_uint32 b6 = v_combine_ऊपरी(a2, a6);
                v_uint32 b7 = v_combine_ऊपरी(a3, a7);
                vx_store(dst32 + dstep_e*(i+0) + j, b0);
                vx_store(dst32 + dstep_e*(i+1) + j, b1);
                vx_store(dst32 + dstep_e*(i+2) + j, b2);
                vx_store(dst32 + dstep_e*(i+3) + j, b3);
                vx_store(dst32 + dstep_e*(i+4) + j, b4);
                vx_store(dst32 + dstep_e*(i+5) + j, b5);
                vx_store(dst32 + dstep_e*(i+6) + j, b6);
                vx_store(dst32 + dstep_e*(i+7) + j, b7);
            }
            क्रम (; j < j_hi; j++)
                क्रम (पूर्णांक k = 0; k < 8; k++)
                    dst32[dstep_e*(i+k) + j] = src32[i + sstep_e*j + k];
        }
    }
    अन्यथा अगर (VTraits<v_uint32>::vlanes() == 4)
    {
        क्रम (; i + 4 <= i_hi; i += 4)
        {
            पूर्णांक j = j_lo;
            क्रम (; j + 4 <= j_hi; j += 4)
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
            क्रम (; j < j_hi; j++)
                क्रम (पूर्णांक k = 0; k < 4; k++)
                    dst32[dstep_e*(i+k) + j] = src32[i + sstep_e*j + k];
        }
    }
#पूर्ण_अगर
    क्रम (; i < i_hi; i++)
        क्रम (पूर्णांक j = j_lo; j < j_hi; j++)
            dst32[dstep_e*i + j] = src32[i + sstep_e*j];
}

व्योम transpose_32bit_blocks_simd(स्थिर uchar* src, माप_प्रकार sstep,
                                 uchar* dst, माप_प्रकार dstep, आकार sz)
{
    स्थिर uint32_t* src32 = reinterpret_cast<स्थिर uint32_t*>(src);
    uint32_t* dst32 = reinterpret_cast<uint32_t*>(dst);
    स्थिर माप_प्रकार sstep_e = sstep / माप(uint32_t);
    स्थिर माप_प्रकार dstep_e = dstep / माप(uint32_t);
    स्थिर पूर्णांक m = sz.width, n = sz.height;

    // Single-threaded for small inputs to avoid parallel_for_ overhead.
    स्थिर int64_t kMinParallelElements = 64 * 1024;
    अगर ((int64_t)m * n < kMinParallelElements)
    {
        transpose_32bit_blocks_tile(src32, sstep_e, dst32, dstep_e, 0, m, 0, n);
        वापस;
    }

    स्थिर पूर्णांक TILE = 32;
    स्थिर पूर्णांक mtiles = (m + TILE - 1) / TILE;
    स्थिर पूर्णांक ntiles = (n + TILE - 1) / TILE;
    स्थिर int64_t योग_Tiles = (int64_t)mtiles * ntiles;
    parallel_for_(Range(0, (पूर्णांक)योग_Tiles), [&](स्थिर Range& r)
    {
        क्रम (पूर्णांक idx = r.start; idx < r.end; idx++)
        {
            पूर्णांक ti = idx / ntiles;
            पूर्णांक tj = idx - ti * ntiles;
            पूर्णांक i_lo = ti * TILE, i_hi = std::min(i_lo + TILE, m);
            पूर्णांक j_lo = tj * TILE, j_hi = std::min(j_lo + TILE, n);
            transpose_32bit_blocks_tile(src32, sstep_e, dst32, dstep_e, i_lo, i_hi, j_lo, j_hi);
        }
    });
}

#पूर्ण_अगर // CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

CV_CPU_OPTIMIZATION_NAMESPACE_END

} // namespace cv
