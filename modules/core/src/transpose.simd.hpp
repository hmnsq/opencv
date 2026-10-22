// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html
// Copyright (C) 2026, BigVision LLC, all rights reserved.
// Third party copyrights are property of their respective owners.

#شامل "opencv2/core/hal/intrin.hpp"

نامخلا cv {

CV_CPU_OPTIMIZATION_NAMESPACE_BEGIN

خالی transpose_32bit_بلاکیں_simd(ثابت uchar* src, size_t sstep,
                                 uchar* dst, size_t dstep, سائز sz);

#اگر_غیر_تعریف_شدہ CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

ساکن اندرونی خالی transpose_32bit_بلاکیں_tile(ثابت uint32_t* src32, size_t sstep_e,
                                               uint32_t* dst32, size_t dstep_e,
                                               صحیح i_lo, صحیح i_hi, صحیح j_lo, صحیح j_hi)
{
    صحیح i = i_lo;
#اگر (CV_SIMD || CV_SIMD_SCALABLE)
    اگر (VTraits<v_uint32>::vlanes() == 8)
    {
        برائے (; i + 8 <= i_hi; i += 8)
        {
            صحیح j = j_lo;
            برائے (; j + 8 <= j_hi; j += 8)
            {
                v_uint32 r0 = vx_بھرو(src32 + i + sstep_e*(j+0));
                v_uint32 r1 = vx_بھرو(src32 + i + sstep_e*(j+1));
                v_uint32 r2 = vx_بھرو(src32 + i + sstep_e*(j+2));
                v_uint32 r3 = vx_بھرو(src32 + i + sstep_e*(j+3));
                v_uint32 r4 = vx_بھرو(src32 + i + sstep_e*(j+4));
                v_uint32 r5 = vx_بھرو(src32 + i + sstep_e*(j+5));
                v_uint32 r6 = vx_بھرو(src32 + i + sstep_e*(j+6));
                v_uint32 r7 = vx_بھرو(src32 + i + sstep_e*(j+7));
                v_uint32 a0, a1, a2, a3, a4, a5, a6, a7;
                v_transpose4x4(r0, r1, r2, r3, a0, a1, a2, a3);
                v_transpose4x4(r4, r5, r6, r7, a4, a5, a6, a7);
                v_uint32 b0 = v_combine_کم (a0, a4);
                v_uint32 b1 = v_combine_کم (a1, a5);
                v_uint32 b2 = v_combine_کم (a2, a6);
                v_uint32 b3 = v_combine_کم (a3, a7);
                v_uint32 b4 = v_combine_بلند(a0, a4);
                v_uint32 b5 = v_combine_بلند(a1, a5);
                v_uint32 b6 = v_combine_بلند(a2, a6);
                v_uint32 b7 = v_combine_بلند(a3, a7);
                vx_store(dst32 + dstep_e*(i+0) + j, b0);
                vx_store(dst32 + dstep_e*(i+1) + j, b1);
                vx_store(dst32 + dstep_e*(i+2) + j, b2);
                vx_store(dst32 + dstep_e*(i+3) + j, b3);
                vx_store(dst32 + dstep_e*(i+4) + j, b4);
                vx_store(dst32 + dstep_e*(i+5) + j, b5);
                vx_store(dst32 + dstep_e*(i+6) + j, b6);
                vx_store(dst32 + dstep_e*(i+7) + j, b7);
            }
            برائے (; j < j_hi; j++)
                برائے (صحیح k = 0; k < 8; k++)
                    dst32[dstep_e*(i+k) + j] = src32[i + sstep_e*j + k];
        }
    }
    ورنہ اگر (VTraits<v_uint32>::vlanes() == 4)
    {
        برائے (; i + 4 <= i_hi; i += 4)
        {
            صحیح j = j_lo;
            برائے (; j + 4 <= j_hi; j += 4)
            {
                v_uint32 r0 = vx_بھرو(src32 + i + sstep_e*(j+0));
                v_uint32 r1 = vx_بھرو(src32 + i + sstep_e*(j+1));
                v_uint32 r2 = vx_بھرو(src32 + i + sstep_e*(j+2));
                v_uint32 r3 = vx_بھرو(src32 + i + sstep_e*(j+3));
                v_uint32 o0, o1, o2, o3;
                v_transpose4x4(r0, r1, r2, r3, o0, o1, o2, o3);
                vx_store(dst32 + dstep_e*(i+0) + j, o0);
                vx_store(dst32 + dstep_e*(i+1) + j, o1);
                vx_store(dst32 + dstep_e*(i+2) + j, o2);
                vx_store(dst32 + dstep_e*(i+3) + j, o3);
            }
            برائے (; j < j_hi; j++)
                برائے (صحیح k = 0; k < 4; k++)
                    dst32[dstep_e*(i+k) + j] = src32[i + sstep_e*j + k];
        }
    }
#اگر_ختم
    برائے (; i < i_hi; i++)
        برائے (صحیح j = j_lo; j < j_hi; j++)
            dst32[dstep_e*i + j] = src32[i + sstep_e*j];
}

خالی transpose_32bit_بلاکیں_simd(ثابت uchar* src, size_t sstep,
                                 uchar* dst, size_t dstep, سائز sz)
{
    ثابت uint32_t* src32 = reinterpret_cast<ثابت uint32_t*>(src);
    uint32_t* dst32 = reinterpret_cast<uint32_t*>(dst);
    ثابت size_t sstep_e = sstep / حجم(uint32_t);
    ثابت size_t dstep_e = dstep / حجم(uint32_t);
    ثابت صحیح m = sz.چوڑائی, n = sz.اونچائی;

    // Single-threaded for small inputs to avoid parallel_for_ overhead.
    ثابت int64_t kMinParallelElements = 64 * 1024;
    اگر ((int64_t)m * n < kMinParallelElements)
    {
        transpose_32bit_بلاکیں_tile(src32, sstep_e, dst32, dstep_e, 0, m, 0, n);
        واپس;
    }

    ثابت صحیح TILE = 32;
    ثابت صحیح mtiles = (m + TILE - 1) / TILE;
    ثابت صحیح ntiles = (n + TILE - 1) / TILE;
    ثابت int64_t کل_Tiles = (int64_t)mtiles * ntiles;
    parallel_for_(Range(0, (صحیح)کل_Tiles), [&](ثابت Range& r)
    {
        برائے (صحیح idx = r.آغاز; idx < r.end; idx++)
        {
            صحیح ti = idx / ntiles;
            صحیح tj = idx - ti * ntiles;
            صحیح i_lo = ti * TILE, i_hi = std::min(i_lo + TILE, m);
            صحیح j_lo = tj * TILE, j_hi = std::min(j_lo + TILE, n);
            transpose_32bit_بلاکیں_tile(src32, sstep_e, dst32, dstep_e, i_lo, i_hi, j_lo, j_hi);
        }
    });
}

#اگر_ختم // CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

CV_CPU_OPTIMIZATION_NAMESPACE_انتہا

} // namespace cv
