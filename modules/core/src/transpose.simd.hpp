// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html
// Copyright (C) 2026, BigVision LLC, all rights reserved.
// Third party copyrights are property of their respective owners.

#অন্তর্ভাব "opencv2/core/hal/intrin.hpp"

নামাকাশ cv {

CV_CPU_OPTIMIZATION_NAMESPACE_BEGIN

শূন্য transpose_32bit_blocks_simd(ধ্রুব uchar* src, size_t sstep,
                                 uchar* dst, size_t dstep, Size sz);

#ifndef CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

স্থির অন্তঃস্থ শূন্য transpose_32bit_blocks_tile(ধ্রুব uint32_t* src32, size_t sstep_e,
                                               uint32_t* dst32, size_t dstep_e,
                                               পূর্ণাঙ্ক i_lo, পূর্ণাঙ্ক i_hi, পূর্ণাঙ্ক j_lo, পূর্ণাঙ্ক j_hi)
{
    পূর্ণাঙ্ক i = i_lo;
#যদি (CV_SIMD || CV_SIMD_SCALABLE)
    যদি (VTraits<v_uint32>::vlanes() == 8)
    {
        প্রতি (; i + 8 <= i_hi; i += 8)
        {
            পূর্ণাঙ্ক j = j_lo;
            প্রতি (; j + 8 <= j_hi; j += 8)
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
                v_uint32 b0 = v_combine_নিচের (a0, a4);
                v_uint32 b1 = v_combine_নিচের (a1, a5);
                v_uint32 b2 = v_combine_নিচের (a2, a6);
                v_uint32 b3 = v_combine_নিচের (a3, a7);
                v_uint32 b4 = v_combine_উপরের(a0, a4);
                v_uint32 b5 = v_combine_উপরের(a1, a5);
                v_uint32 b6 = v_combine_উপরের(a2, a6);
                v_uint32 b7 = v_combine_উপরের(a3, a7);
                vx_store(dst32 + dstep_e*(i+0) + j, b0);
                vx_store(dst32 + dstep_e*(i+1) + j, b1);
                vx_store(dst32 + dstep_e*(i+2) + j, b2);
                vx_store(dst32 + dstep_e*(i+3) + j, b3);
                vx_store(dst32 + dstep_e*(i+4) + j, b4);
                vx_store(dst32 + dstep_e*(i+5) + j, b5);
                vx_store(dst32 + dstep_e*(i+6) + j, b6);
                vx_store(dst32 + dstep_e*(i+7) + j, b7);
            }
            প্রতি (; j < j_hi; j++)
                প্রতি (পূর্ণাঙ্ক k = 0; k < 8; k++)
                    dst32[dstep_e*(i+k) + j] = src32[i + sstep_e*j + k];
        }
    }
    অন্যথা যদি (VTraits<v_uint32>::vlanes() == 4)
    {
        প্রতি (; i + 4 <= i_hi; i += 4)
        {
            পূর্ণাঙ্ক j = j_lo;
            প্রতি (; j + 4 <= j_hi; j += 4)
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
            প্রতি (; j < j_hi; j++)
                প্রতি (পূর্ণাঙ্ক k = 0; k < 4; k++)
                    dst32[dstep_e*(i+k) + j] = src32[i + sstep_e*j + k];
        }
    }
#endif
    প্রতি (; i < i_hi; i++)
        প্রতি (পূর্ণাঙ্ক j = j_lo; j < j_hi; j++)
            dst32[dstep_e*i + j] = src32[i + sstep_e*j];
}

শূন্য transpose_32bit_blocks_simd(ধ্রুব uchar* src, size_t sstep,
                                 uchar* dst, size_t dstep, Size sz)
{
    ধ্রুব uint32_t* src32 = reinterpret_cast<ধ্রুব uint32_t*>(src);
    uint32_t* dst32 = reinterpret_cast<uint32_t*>(dst);
    ধ্রুব size_t sstep_e = sstep / পরিমাণ(uint32_t);
    ধ্রুব size_t dstep_e = dstep / পরিমাণ(uint32_t);
    ধ্রুব পূর্ণাঙ্ক m = sz.width, n = sz.height;

    // Single-threaded for small inputs to avoid parallel_for_ overhead.
    ধ্রুব int64_t kMinParallelElements = 64 * 1024;
    যদি ((int64_t)m * n < kMinParallelElements)
    {
        transpose_32bit_blocks_tile(src32, sstep_e, dst32, dstep_e, 0, m, 0, n);
        প্রত্যাগম;
    }

    ধ্রুব পূর্ণাঙ্ক TILE = 32;
    ধ্রুব পূর্ণাঙ্ক mtiles = (m + TILE - 1) / TILE;
    ধ্রুব পূর্ণাঙ্ক ntiles = (n + TILE - 1) / TILE;
    ধ্রুব int64_t totalTiles = (int64_t)mtiles * ntiles;
    parallel_for_(Range(0, (পূর্ণাঙ্ক)totalTiles), [&](ধ্রুব Range& r)
    {
        প্রতি (পূর্ণাঙ্ক idx = r.start; idx < r.end; idx++)
        {
            পূর্ণাঙ্ক ti = idx / ntiles;
            পূর্ণাঙ্ক tj = idx - ti * ntiles;
            পূর্ণাঙ্ক i_lo = ti * TILE, i_hi = std::min(i_lo + TILE, m);
            পূর্ণাঙ্ক j_lo = tj * TILE, j_hi = std::min(j_lo + TILE, n);
            transpose_32bit_blocks_tile(src32, sstep_e, dst32, dstep_e, i_lo, i_hi, j_lo, j_hi);
        }
    });
}

#endif // CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

CV_CPU_OPTIMIZATION_NAMESPACE_END

} // namespace cv
