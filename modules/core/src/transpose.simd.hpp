// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html
// Copyright (C) 2026, BigVision LLC, all rights reserved.
// Third party copyrights are property of their respective owners.

#ਅਨ੍ਤਰ੍ਭਾਵ "opencv2/core/hal/intrin.hpp"

ਨਾਮਾਕਾਸ਼ cv {

CV_CPU_OPTIMIZATION_NAMESPACE_BEGIN

ਖਾਲੀ transpose_32bit_ਖੰਡਾਂ_simd(ਅਚਲ uchar* src, size_t sstep,
                                 uchar* dst, size_t dstep, ਮਾਪ sz);

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ transpose_32bit_ਖੰਡਾਂ_tile(ਅਚਲ uint32_t* src32, size_t sstep_e,
                                               uint32_t* dst32, size_t dstep_e,
                                               ਪੂਰਨ i_lo, ਪੂਰਨ i_hi, ਪੂਰਨ j_lo, ਪੂਰਨ j_hi)
{
    ਪੂਰਨ i = i_lo;
#ਜੇ (CV_SIMD || CV_SIMD_SCALABLE)
    ਜੇ (VTraits<v_uint32>::vlanes() == 8)
    {
        ਲਈ (; i + 8 <= i_hi; i += 8)
        {
            ਪੂਰਨ j = j_lo;
            ਲਈ (; j + 8 <= j_hi; j += 8)
            {
                v_uint32 r0 = vx_ਲੱਦੋ(src32 + i + sstep_e*(j+0));
                v_uint32 r1 = vx_ਲੱਦੋ(src32 + i + sstep_e*(j+1));
                v_uint32 r2 = vx_ਲੱਦੋ(src32 + i + sstep_e*(j+2));
                v_uint32 r3 = vx_ਲੱਦੋ(src32 + i + sstep_e*(j+3));
                v_uint32 r4 = vx_ਲੱਦੋ(src32 + i + sstep_e*(j+4));
                v_uint32 r5 = vx_ਲੱਦੋ(src32 + i + sstep_e*(j+5));
                v_uint32 r6 = vx_ਲੱਦੋ(src32 + i + sstep_e*(j+6));
                v_uint32 r7 = vx_ਲੱਦੋ(src32 + i + sstep_e*(j+7));
                v_uint32 a0, a1, a2, a3, a4, a5, a6, a7;
                v_transpose4x4(r0, r1, r2, r3, a0, a1, a2, a3);
                v_transpose4x4(r4, r5, r6, r7, a4, a5, a6, a7);
                v_uint32 b0 = v_combine_ਨੀਵਾਂ (a0, a4);
                v_uint32 b1 = v_combine_ਨੀਵਾਂ (a1, a5);
                v_uint32 b2 = v_combine_ਨੀਵਾਂ (a2, a6);
                v_uint32 b3 = v_combine_ਨੀਵਾਂ (a3, a7);
                v_uint32 b4 = v_combine_ਉੱਚਾ(a0, a4);
                v_uint32 b5 = v_combine_ਉੱਚਾ(a1, a5);
                v_uint32 b6 = v_combine_ਉੱਚਾ(a2, a6);
                v_uint32 b7 = v_combine_ਉੱਚਾ(a3, a7);
                vx_store(dst32 + dstep_e*(i+0) + j, b0);
                vx_store(dst32 + dstep_e*(i+1) + j, b1);
                vx_store(dst32 + dstep_e*(i+2) + j, b2);
                vx_store(dst32 + dstep_e*(i+3) + j, b3);
                vx_store(dst32 + dstep_e*(i+4) + j, b4);
                vx_store(dst32 + dstep_e*(i+5) + j, b5);
                vx_store(dst32 + dstep_e*(i+6) + j, b6);
                vx_store(dst32 + dstep_e*(i+7) + j, b7);
            }
            ਲਈ (; j < j_hi; j++)
                ਲਈ (ਪੂਰਨ k = 0; k < 8; k++)
                    dst32[dstep_e*(i+k) + j] = src32[i + sstep_e*j + k];
        }
    }
    ਨਹੀਂਤਾਂ ਜੇ (VTraits<v_uint32>::vlanes() == 4)
    {
        ਲਈ (; i + 4 <= i_hi; i += 4)
        {
            ਪੂਰਨ j = j_lo;
            ਲਈ (; j + 4 <= j_hi; j += 4)
            {
                v_uint32 r0 = vx_ਲੱਦੋ(src32 + i + sstep_e*(j+0));
                v_uint32 r1 = vx_ਲੱਦੋ(src32 + i + sstep_e*(j+1));
                v_uint32 r2 = vx_ਲੱਦੋ(src32 + i + sstep_e*(j+2));
                v_uint32 r3 = vx_ਲੱਦੋ(src32 + i + sstep_e*(j+3));
                v_uint32 o0, o1, o2, o3;
                v_transpose4x4(r0, r1, r2, r3, o0, o1, o2, o3);
                vx_store(dst32 + dstep_e*(i+0) + j, o0);
                vx_store(dst32 + dstep_e*(i+1) + j, o1);
                vx_store(dst32 + dstep_e*(i+2) + j, o2);
                vx_store(dst32 + dstep_e*(i+3) + j, o3);
            }
            ਲਈ (; j < j_hi; j++)
                ਲਈ (ਪੂਰਨ k = 0; k < 4; k++)
                    dst32[dstep_e*(i+k) + j] = src32[i + sstep_e*j + k];
        }
    }
#ਜੇ_ਅੰਤ
    ਲਈ (; i < i_hi; i++)
        ਲਈ (ਪੂਰਨ j = j_lo; j < j_hi; j++)
            dst32[dstep_e*i + j] = src32[i + sstep_e*j];
}

ਖਾਲੀ transpose_32bit_ਖੰਡਾਂ_simd(ਅਚਲ uchar* src, size_t sstep,
                                 uchar* dst, size_t dstep, ਮਾਪ sz)
{
    ਅਚਲ uint32_t* src32 = reinterpret_cast<ਅਚਲ uint32_t*>(src);
    uint32_t* dst32 = reinterpret_cast<uint32_t*>(dst);
    ਅਚਲ size_t sstep_e = sstep / ਆਕਾਰ(uint32_t);
    ਅਚਲ size_t dstep_e = dstep / ਆਕਾਰ(uint32_t);
    ਅਚਲ ਪੂਰਨ m = sz.ਚੌੜਾਈ, n = sz.ਉਚਾਈ;

    // Single-threaded for small inputs to avoid parallel_for_ overhead.
    ਅਚਲ int64_t kMinParallelElements = 64 * 1024;
    ਜੇ ((int64_t)m * n < kMinParallelElements)
    {
        transpose_32bit_ਖੰਡਾਂ_tile(src32, sstep_e, dst32, dstep_e, 0, m, 0, n);
        ਵਾਪਸ;
    }

    ਅਚਲ ਪੂਰਨ TILE = 32;
    ਅਚਲ ਪੂਰਨ mtiles = (m + TILE - 1) / TILE;
    ਅਚਲ ਪੂਰਨ ntiles = (n + TILE - 1) / TILE;
    ਅਚਲ int64_t ਕੁੱਲ_Tiles = (int64_t)mtiles * ntiles;
    parallel_for_(Range(0, (ਪੂਰਨ)ਕੁੱਲ_Tiles), [&](ਅਚਲ Range& r)
    {
        ਲਈ (ਪੂਰਨ idx = r.ਸ਼ੁਰੂ; idx < r.end; idx++)
        {
            ਪੂਰਨ ti = idx / ntiles;
            ਪੂਰਨ tj = idx - ti * ntiles;
            ਪੂਰਨ i_lo = ti * TILE, i_hi = std::min(i_lo + TILE, m);
            ਪੂਰਨ j_lo = tj * TILE, j_hi = std::min(j_lo + TILE, n);
            transpose_32bit_ਖੰਡਾਂ_tile(src32, sstep_e, dst32, dstep_e, i_lo, i_hi, j_lo, j_hi);
        }
    });
}

#ਜੇ_ਅੰਤ // CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

CV_CPU_OPTIMIZATION_NAMESPACE_ਸਿਰਾ

} // namespace cv
