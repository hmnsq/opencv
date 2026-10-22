// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html
// Copyright (C) 2026, BigVision LLC, all rights reserved.
// Third party copyrights are property of their respective owners.

#ಅನ್ತರ್ಭಾವ "opencv2/core/hal/intrin.hpp"

ನಾಮಾಕಾಶ cv {

CV_CPU_OPTIMIZATION_NAMESPACE_BEGIN

ಶೂನ್ಯ transpose_32bit_ಖಂಡಗಳು_simd(ಸ್ಥಿರಾಂಕ uchar* src, size_t sstep,
                                 uchar* dst, size_t dstep, ಗಾತ್ರ_2 sz);

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ transpose_32bit_ಖಂಡಗಳು_tile(ಸ್ಥಿರಾಂಕ uint32_t* src32, size_t sstep_e,
                                               uint32_t* dst32, size_t dstep_e,
                                               ಪೂರ್ಣಾಂಕ i_lo, ಪೂರ್ಣಾಂಕ i_hi, ಪೂರ್ಣಾಂಕ j_lo, ಪೂರ್ಣಾಂಕ j_hi)
{
    ಪೂರ್ಣಾಂಕ i = i_lo;
#ಆದರೆ (CV_SIMD || CV_SIMD_SCALABLE)
    ಆದರೆ (VTraits<v_uint32>::vlanes() == 8)
    {
        ಗಾಗಿ (; i + 8 <= i_hi; i += 8)
        {
            ಪೂರ್ಣಾಂಕ j = j_lo;
            ಗಾಗಿ (; j + 8 <= j_hi; j += 8)
            {
                v_uint32 r0 = vx_ತುಂಬು(src32 + i + sstep_e*(j+0));
                v_uint32 r1 = vx_ತುಂಬು(src32 + i + sstep_e*(j+1));
                v_uint32 r2 = vx_ತುಂಬು(src32 + i + sstep_e*(j+2));
                v_uint32 r3 = vx_ತುಂಬು(src32 + i + sstep_e*(j+3));
                v_uint32 r4 = vx_ತುಂಬು(src32 + i + sstep_e*(j+4));
                v_uint32 r5 = vx_ತುಂಬು(src32 + i + sstep_e*(j+5));
                v_uint32 r6 = vx_ತುಂಬು(src32 + i + sstep_e*(j+6));
                v_uint32 r7 = vx_ತುಂಬು(src32 + i + sstep_e*(j+7));
                v_uint32 a0, a1, a2, a3, a4, a5, a6, a7;
                v_transpose4x4(r0, r1, r2, r3, a0, a1, a2, a3);
                v_transpose4x4(r4, r5, r6, r7, a4, a5, a6, a7);
                v_uint32 b0 = v_combine_ಕೆಳಗಿನ (a0, a4);
                v_uint32 b1 = v_combine_ಕೆಳಗಿನ (a1, a5);
                v_uint32 b2 = v_combine_ಕೆಳಗಿನ (a2, a6);
                v_uint32 b3 = v_combine_ಕೆಳಗಿನ (a3, a7);
                v_uint32 b4 = v_combine_ಮೇಲಿನ(a0, a4);
                v_uint32 b5 = v_combine_ಮೇಲಿನ(a1, a5);
                v_uint32 b6 = v_combine_ಮೇಲಿನ(a2, a6);
                v_uint32 b7 = v_combine_ಮೇಲಿನ(a3, a7);
                vx_store(dst32 + dstep_e*(i+0) + j, b0);
                vx_store(dst32 + dstep_e*(i+1) + j, b1);
                vx_store(dst32 + dstep_e*(i+2) + j, b2);
                vx_store(dst32 + dstep_e*(i+3) + j, b3);
                vx_store(dst32 + dstep_e*(i+4) + j, b4);
                vx_store(dst32 + dstep_e*(i+5) + j, b5);
                vx_store(dst32 + dstep_e*(i+6) + j, b6);
                vx_store(dst32 + dstep_e*(i+7) + j, b7);
            }
            ಗಾಗಿ (; j < j_hi; j++)
                ಗಾಗಿ (ಪೂರ್ಣಾಂಕ k = 0; k < 8; k++)
                    dst32[dstep_e*(i+k) + j] = src32[i + sstep_e*j + k];
        }
    }
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (VTraits<v_uint32>::vlanes() == 4)
    {
        ಗಾಗಿ (; i + 4 <= i_hi; i += 4)
        {
            ಪೂರ್ಣಾಂಕ j = j_lo;
            ಗಾಗಿ (; j + 4 <= j_hi; j += 4)
            {
                v_uint32 r0 = vx_ತುಂಬು(src32 + i + sstep_e*(j+0));
                v_uint32 r1 = vx_ತುಂಬು(src32 + i + sstep_e*(j+1));
                v_uint32 r2 = vx_ತುಂಬು(src32 + i + sstep_e*(j+2));
                v_uint32 r3 = vx_ತುಂಬು(src32 + i + sstep_e*(j+3));
                v_uint32 o0, o1, o2, o3;
                v_transpose4x4(r0, r1, r2, r3, o0, o1, o2, o3);
                vx_store(dst32 + dstep_e*(i+0) + j, o0);
                vx_store(dst32 + dstep_e*(i+1) + j, o1);
                vx_store(dst32 + dstep_e*(i+2) + j, o2);
                vx_store(dst32 + dstep_e*(i+3) + j, o3);
            }
            ಗಾಗಿ (; j < j_hi; j++)
                ಗಾಗಿ (ಪೂರ್ಣಾಂಕ k = 0; k < 4; k++)
                    dst32[dstep_e*(i+k) + j] = src32[i + sstep_e*j + k];
        }
    }
#ಆದರೆ_ಅಂತ್ಯ
    ಗಾಗಿ (; i < i_hi; i++)
        ಗಾಗಿ (ಪೂರ್ಣಾಂಕ j = j_lo; j < j_hi; j++)
            dst32[dstep_e*i + j] = src32[i + sstep_e*j];
}

ಶೂನ್ಯ transpose_32bit_ಖಂಡಗಳು_simd(ಸ್ಥಿರಾಂಕ uchar* src, size_t sstep,
                                 uchar* dst, size_t dstep, ಗಾತ್ರ_2 sz)
{
    ಸ್ಥಿರಾಂಕ uint32_t* src32 = reinterpret_cast<ಸ್ಥಿರಾಂಕ uint32_t*>(src);
    uint32_t* dst32 = reinterpret_cast<uint32_t*>(dst);
    ಸ್ಥಿರಾಂಕ size_t sstep_e = sstep / ಗಾತ್ರ(uint32_t);
    ಸ್ಥಿರಾಂಕ size_t dstep_e = dstep / ಗಾತ್ರ(uint32_t);
    ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ m = sz.ಅಗಲ, n = sz.ಎತ್ತರ;

    // Single-threaded for small inputs to avoid parallel_for_ overhead.
    ಸ್ಥಿರಾಂಕ int64_t kMinParallelElements = 64 * 1024;
    ಆದರೆ ((int64_t)m * n < kMinParallelElements)
    {
        transpose_32bit_ಖಂಡಗಳು_tile(src32, sstep_e, dst32, dstep_e, 0, m, 0, n);
        ಹಿಂತಿರುಗಿ;
    }

    ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ TILE = 32;
    ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ mtiles = (m + TILE - 1) / TILE;
    ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ ntiles = (n + TILE - 1) / TILE;
    ಸ್ಥಿರಾಂಕ int64_t ಒಟ್ಟು_Tiles = (int64_t)mtiles * ntiles;
    parallel_for_(Range(0, (ಪೂರ್ಣಾಂಕ)ಒಟ್ಟು_Tiles), [&](ಸ್ಥಿರಾಂಕ Range& r)
    {
        ಗಾಗಿ (ಪೂರ್ಣಾಂಕ idx = r.ಆರಂಭ; idx < r.end; idx++)
        {
            ಪೂರ್ಣಾಂಕ ti = idx / ntiles;
            ಪೂರ್ಣಾಂಕ tj = idx - ti * ntiles;
            ಪೂರ್ಣಾಂಕ i_lo = ti * TILE, i_hi = std::min(i_lo + TILE, m);
            ಪೂರ್ಣಾಂಕ j_lo = tj * TILE, j_hi = std::min(j_lo + TILE, n);
            transpose_32bit_ಖಂಡಗಳು_tile(src32, sstep_e, dst32, dstep_e, i_lo, i_hi, j_lo, j_hi);
        }
    });
}

#ಆದರೆ_ಅಂತ್ಯ // CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

CV_CPU_OPTIMIZATION_NAMESPACE_ಕೊನೆ

} // namespace cv
