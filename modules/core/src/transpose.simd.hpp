// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html
// Copyright (C) 2026, BigVision LLC, all rights reserved.
// Third party copyrights are property of their respective owners.

#અન્તર્ભાવ "opencv2/core/hal/intrin.hpp"

નામાકાશ cv {

CV_CPU_OPTIMIZATION_NAMESPACE_BEGIN

શૂન્ય transpose_32bit_blocks_simd(અચલ uchar* src, size_t sstep,
                                 uchar* dst, size_t dstep, Size sz);

#ifndef CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

સ્થિર અંતઃરેખ શૂન્ય transpose_32bit_blocks_tile(અચલ uint32_t* src32, size_t sstep_e,
                                               uint32_t* dst32, size_t dstep_e,
                                               પૂર્ણાંક i_lo, પૂર્ણાંક i_hi, પૂર્ણાંક j_lo, પૂર્ણાંક j_hi)
{
    પૂર્ણાંક i = i_lo;
#જો (CV_SIMD || CV_SIMD_SCALABLE)
    જો (VTraits<v_uint32>::vlanes() == 8)
    {
        માટે (; i + 8 <= i_hi; i += 8)
        {
            પૂર્ણાંક j = j_lo;
            માટે (; j + 8 <= j_hi; j += 8)
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
                v_uint32 b0 = v_combine_નીચલું (a0, a4);
                v_uint32 b1 = v_combine_નીચલું (a1, a5);
                v_uint32 b2 = v_combine_નીચલું (a2, a6);
                v_uint32 b3 = v_combine_નીચલું (a3, a7);
                v_uint32 b4 = v_combine_ઉપલું(a0, a4);
                v_uint32 b5 = v_combine_ઉપલું(a1, a5);
                v_uint32 b6 = v_combine_ઉપલું(a2, a6);
                v_uint32 b7 = v_combine_ઉપલું(a3, a7);
                vx_store(dst32 + dstep_e*(i+0) + j, b0);
                vx_store(dst32 + dstep_e*(i+1) + j, b1);
                vx_store(dst32 + dstep_e*(i+2) + j, b2);
                vx_store(dst32 + dstep_e*(i+3) + j, b3);
                vx_store(dst32 + dstep_e*(i+4) + j, b4);
                vx_store(dst32 + dstep_e*(i+5) + j, b5);
                vx_store(dst32 + dstep_e*(i+6) + j, b6);
                vx_store(dst32 + dstep_e*(i+7) + j, b7);
            }
            માટે (; j < j_hi; j++)
                માટે (પૂર્ણાંક k = 0; k < 8; k++)
                    dst32[dstep_e*(i+k) + j] = src32[i + sstep_e*j + k];
        }
    }
    નહીંતર જો (VTraits<v_uint32>::vlanes() == 4)
    {
        માટે (; i + 4 <= i_hi; i += 4)
        {
            પૂર્ણાંક j = j_lo;
            માટે (; j + 4 <= j_hi; j += 4)
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
            માટે (; j < j_hi; j++)
                માટે (પૂર્ણાંક k = 0; k < 4; k++)
                    dst32[dstep_e*(i+k) + j] = src32[i + sstep_e*j + k];
        }
    }
#endif
    માટે (; i < i_hi; i++)
        માટે (પૂર્ણાંક j = j_lo; j < j_hi; j++)
            dst32[dstep_e*i + j] = src32[i + sstep_e*j];
}

શૂન્ય transpose_32bit_blocks_simd(અચલ uchar* src, size_t sstep,
                                 uchar* dst, size_t dstep, Size sz)
{
    અચલ uint32_t* src32 = reinterpret_cast<અચલ uint32_t*>(src);
    uint32_t* dst32 = reinterpret_cast<uint32_t*>(dst);
    અચલ size_t sstep_e = sstep / કદ(uint32_t);
    અચલ size_t dstep_e = dstep / કદ(uint32_t);
    અચલ પૂર્ણાંક m = sz.width, n = sz.height;

    // Single-threaded for small inputs to avoid parallel_for_ overhead.
    અચલ int64_t kMinParallelElements = 64 * 1024;
    જો ((int64_t)m * n < kMinParallelElements)
    {
        transpose_32bit_blocks_tile(src32, sstep_e, dst32, dstep_e, 0, m, 0, n);
        પાછા;
    }

    અચલ પૂર્ણાંક TILE = 32;
    અચલ પૂર્ણાંક mtiles = (m + TILE - 1) / TILE;
    અચલ પૂર્ણાંક ntiles = (n + TILE - 1) / TILE;
    અચલ int64_t totalTiles = (int64_t)mtiles * ntiles;
    parallel_for_(Range(0, (પૂર્ણાંક)totalTiles), [&](અચલ Range& r)
    {
        માટે (પૂર્ણાંક idx = r.start; idx < r.end; idx++)
        {
            પૂર્ણાંક ti = idx / ntiles;
            પૂર્ણાંક tj = idx - ti * ntiles;
            પૂર્ણાંક i_lo = ti * TILE, i_hi = std::min(i_lo + TILE, m);
            પૂર્ણાંક j_lo = tj * TILE, j_hi = std::min(j_lo + TILE, n);
            transpose_32bit_blocks_tile(src32, sstep_e, dst32, dstep_e, i_lo, i_hi, j_lo, j_hi);
        }
    });
}

#endif // CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

CV_CPU_OPTIMIZATION_NAMESPACE_END

} // namespace cv
