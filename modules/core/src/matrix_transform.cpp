// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html
// Copyright (C) 2026, Advanced Micro Devices, Inc., all rights reserved.
#ضمن "precomp.hpp"
#ضمن "opencl_kernels_core.hpp"
#ضمن "hal_replacement.hpp"
#ضمن "opencv2/core/detail/dispatch_helper.impl.hpp"

#ضمن "transpose.simd.hpp"
#ضمن "transpose.simd_declarations.hpp"
#ضمن "nd_copy.hpp"

#ضمن <algorithm> // std::swap_ranges
#ضمن <numeric> // std::accumulate

نطاق cv {

////////////////////////////////////// transpose /////////////////////////////////////////
#إذا CV_SIMD128
ساكن فراغ transpose_8bit_simd(ثابت uchar* src, size_t sstep, uchar* dst, size_t dstep, اندازه sz)
{
    ثابت صحيح m = sz.پلنوالی, n = sz.لوړوالی;
    صحيح i = 0;
    لكل (; i <= m - 16; i += 16)
    {
        صحيح j = 0;
        لكل (; j <= n - 16; j += 16)
        {
            v_uint8x16 r0  = v_بارول(src + i + sstep*(j+ 0));
            v_uint8x16 r1  = v_بارول(src + i + sstep*(j+ 1));
            v_uint8x16 r2  = v_بارول(src + i + sstep*(j+ 2));
            v_uint8x16 r3  = v_بارول(src + i + sstep*(j+ 3));
            v_uint8x16 r4  = v_بارول(src + i + sstep*(j+ 4));
            v_uint8x16 r5  = v_بارول(src + i + sstep*(j+ 5));
            v_uint8x16 r6  = v_بارول(src + i + sstep*(j+ 6));
            v_uint8x16 r7  = v_بارول(src + i + sstep*(j+ 7));
            v_uint8x16 r8  = v_بارول(src + i + sstep*(j+ 8));
            v_uint8x16 r9  = v_بارول(src + i + sstep*(j+ 9));
            v_uint8x16 r10 = v_بارول(src + i + sstep*(j+10));
            v_uint8x16 r11 = v_بارول(src + i + sstep*(j+11));
            v_uint8x16 r12 = v_بارول(src + i + sstep*(j+12));
            v_uint8x16 r13 = v_بارول(src + i + sstep*(j+13));
            v_uint8x16 r14 = v_بارول(src + i + sstep*(j+14));
            v_uint8x16 r15 = v_بارول(src + i + sstep*(j+15));

            v_uint8x16 t0, t1, t2, t3, t4, t5, t6, t7,
                       t8, t9, t10, t11, t12, t13, t14, t15;

            v_zip(r0,  r1,  t0,  t1);
            v_zip(r2,  r3,  t2,  t3);
            v_zip(r4,  r5,  t4,  t5);
            v_zip(r6,  r7,  t6,  t7);
            v_zip(r8,  r9,  t8,  t9);
            v_zip(r10, r11, t10, t11);
            v_zip(r12, r13, t12, t13);
            v_zip(r14, r15, t14, t15);

            v_uint16x8 s0, s1, s2, s3, s4, s5, s6, s7,
                       s8, s9, s10, s11, s12, s13, s14, s15;
            v_zip(v_reinterpret_as_u16(t0),  v_reinterpret_as_u16(t2),  s0,  s1);
            v_zip(v_reinterpret_as_u16(t1),  v_reinterpret_as_u16(t3),  s2,  s3);
            v_zip(v_reinterpret_as_u16(t4),  v_reinterpret_as_u16(t6),  s4,  s5);
            v_zip(v_reinterpret_as_u16(t5),  v_reinterpret_as_u16(t7),  s6,  s7);
            v_zip(v_reinterpret_as_u16(t8),  v_reinterpret_as_u16(t10), s8,  s9);
            v_zip(v_reinterpret_as_u16(t9),  v_reinterpret_as_u16(t11), s10, s11);
            v_zip(v_reinterpret_as_u16(t12), v_reinterpret_as_u16(t14), s12, s13);
            v_zip(v_reinterpret_as_u16(t13), v_reinterpret_as_u16(t15), s14, s15);

            v_uint32x4 u0, u1, u2, u3, u4, u5, u6, u7,
                       u8, u9, u10, u11, u12, u13, u14, u15;

            v_zip(v_reinterpret_as_u32(s0),  v_reinterpret_as_u32(s4),  u0,  u1);
            v_zip(v_reinterpret_as_u32(s1),  v_reinterpret_as_u32(s5),  u2,  u3);
            v_zip(v_reinterpret_as_u32(s2),  v_reinterpret_as_u32(s6),  u4,  u5);
            v_zip(v_reinterpret_as_u32(s3),  v_reinterpret_as_u32(s7),  u6,  u7);
            v_zip(v_reinterpret_as_u32(s8),  v_reinterpret_as_u32(s12), u8,  u9);
            v_zip(v_reinterpret_as_u32(s9),  v_reinterpret_as_u32(s13), u10, u11);
            v_zip(v_reinterpret_as_u32(s10), v_reinterpret_as_u32(s14), u12, u13);
            v_zip(v_reinterpret_as_u32(s11), v_reinterpret_as_u32(s15), u14, u15);

            v_uint32x4 v0  = v_combine_ټیټ (u0,  u8);
            v_uint32x4 v1  = v_combine_لوړ(u0,  u8);
            v_uint32x4 v2  = v_combine_ټیټ (u1,  u9);
            v_uint32x4 v3  = v_combine_لوړ(u1,  u9);
            v_uint32x4 v4  = v_combine_ټیټ (u2,  u10);
            v_uint32x4 v5  = v_combine_لوړ(u2,  u10);
            v_uint32x4 v6  = v_combine_ټیټ (u3,  u11);
            v_uint32x4 v7  = v_combine_لوړ(u3,  u11);
            v_uint32x4 v8  = v_combine_ټیټ (u4,  u12);
            v_uint32x4 v9  = v_combine_لوړ(u4,  u12);
            v_uint32x4 v10 = v_combine_ټیټ (u5,  u13);
            v_uint32x4 v11 = v_combine_لوړ(u5,  u13);
            v_uint32x4 v12 = v_combine_ټیټ (u6,  u14);
            v_uint32x4 v13 = v_combine_لوړ(u6,  u14);
            v_uint32x4 v14 = v_combine_ټیټ (u7,  u15);
            v_uint32x4 v15 = v_combine_لوړ(u7,  u15);

            v_store(dst + dstep*(i+ 0) + j, v_reinterpret_as_u8(v0));
            v_store(dst + dstep*(i+ 1) + j, v_reinterpret_as_u8(v1));
            v_store(dst + dstep*(i+ 2) + j, v_reinterpret_as_u8(v2));
            v_store(dst + dstep*(i+ 3) + j, v_reinterpret_as_u8(v3));
            v_store(dst + dstep*(i+ 4) + j, v_reinterpret_as_u8(v4));
            v_store(dst + dstep*(i+ 5) + j, v_reinterpret_as_u8(v5));
            v_store(dst + dstep*(i+ 6) + j, v_reinterpret_as_u8(v6));
            v_store(dst + dstep*(i+ 7) + j, v_reinterpret_as_u8(v7));
            v_store(dst + dstep*(i+ 8) + j, v_reinterpret_as_u8(v8));
            v_store(dst + dstep*(i+ 9) + j, v_reinterpret_as_u8(v9));
            v_store(dst + dstep*(i+10) + j, v_reinterpret_as_u8(v10));
            v_store(dst + dstep*(i+11) + j, v_reinterpret_as_u8(v11));
            v_store(dst + dstep*(i+12) + j, v_reinterpret_as_u8(v12));
            v_store(dst + dstep*(i+13) + j, v_reinterpret_as_u8(v13));
            v_store(dst + dstep*(i+14) + j, v_reinterpret_as_u8(v14));
            v_store(dst + dstep*(i+15) + j, v_reinterpret_as_u8(v15));
        }
        لكل (; j < n; j++)
            لكل (صحيح k = 0; k < 16; k++)
                dst[dstep*(i+k) + j] = src[i + sstep*j + k];
    }
    لكل (; i < m; i++)
        لكل (صحيح j = 0; j < n; j++)
            dst[dstep*i + j] = src[i + sstep*j];
}

ساكن فراغ transpose_16bit_simd(ثابت uchar* src, size_t sstep, uchar* dst, size_t dstep, اندازه sz)
{
    ثابت ushort* src16 = reinterpret_cast<ثابت ushort*>(src);
    ushort* dst16 = reinterpret_cast<ushort*>(dst);

    ثابت size_t sstep_e = sstep / حجم(ushort);
    ثابت size_t dstep_e = dstep / حجم(ushort);

    ثابت صحيح m = sz.پلنوالی, n = sz.لوړوالی;
    صحيح i = 0;

    لكل (; i <= m - 8; i += 8)
    {
        صحيح j = 0;
        لكل (; j <= n - 8; j += 8)
        {
            v_uint16x8 r0 = v_بارول(src16 + i + sstep_e*(j+0));
            v_uint16x8 r1 = v_بارول(src16 + i + sstep_e*(j+1));
            v_uint16x8 r2 = v_بارول(src16 + i + sstep_e*(j+2));
            v_uint16x8 r3 = v_بارول(src16 + i + sstep_e*(j+3));
            v_uint16x8 r4 = v_بارول(src16 + i + sstep_e*(j+4));
            v_uint16x8 r5 = v_بارول(src16 + i + sstep_e*(j+5));
            v_uint16x8 r6 = v_بارول(src16 + i + sstep_e*(j+6));
            v_uint16x8 r7 = v_بارول(src16 + i + sstep_e*(j+7));

            v_uint16x8 t0, t1, t2, t3, t4, t5, t6, t7;
            v_zip(r0, r1, t0, t1);
            v_zip(r2, r3, t2, t3);
            v_zip(r4, r5, t4, t5);
            v_zip(r6, r7, t6, t7);
            v_uint32x4 u0, u1, u2, u3, u4, u5, u6, u7;
            v_zip(v_reinterpret_as_u32(t0), v_reinterpret_as_u32(t4), u0, u1);
            v_zip(v_reinterpret_as_u32(t1), v_reinterpret_as_u32(t5), u2, u3);
            v_zip(v_reinterpret_as_u32(t2), v_reinterpret_as_u32(t6), u4, u5);
            v_zip(v_reinterpret_as_u32(t3), v_reinterpret_as_u32(t7), u6, u7);
            v_uint32x4 v0, v1, v2, v3, v4, v5, v6, v7;
            v_zip(u0, u4, v0, v1);
            v_zip(u1, u5, v2, v3);
            v_zip(u2, u6, v4, v5);
            v_zip(u3, u7, v6, v7);

            v_store(dst16 + dstep_e*(i+0) + j, v_reinterpret_as_u16(v0));
            v_store(dst16 + dstep_e*(i+1) + j, v_reinterpret_as_u16(v1));
            v_store(dst16 + dstep_e*(i+2) + j, v_reinterpret_as_u16(v2));
            v_store(dst16 + dstep_e*(i+3) + j, v_reinterpret_as_u16(v3));
            v_store(dst16 + dstep_e*(i+4) + j, v_reinterpret_as_u16(v4));
            v_store(dst16 + dstep_e*(i+5) + j, v_reinterpret_as_u16(v5));
            v_store(dst16 + dstep_e*(i+6) + j, v_reinterpret_as_u16(v6));
            v_store(dst16 + dstep_e*(i+7) + j, v_reinterpret_as_u16(v7));
        }
        لكل (; j < n; j++)
            لكل (صحيح k = 0; k < 8; k++)
                dst16[dstep_e*(i+k) + j] = src16[i + sstep_e*j + k];
    }
    لكل (; i < m; i++)
        لكل (صحيح j = 0; j < n; j++)
            dst16[dstep_e*i + j] = src16[i + sstep_e*j];
}

ساكن فراغ transpose_32bit_simd(ثابت uchar* src, size_t sstep, uchar* dst, size_t dstep, اندازه sz)
{
    // Cache-blocked + AVX2-aware path lives in transpose.simd.hpp; pick the
    // best variant available at runtime.
    CV_CPU_DISPATCH(transpose_32bit_بلاکونه_simd, (src, sstep, dst, dstep, sz),
                    CV_CPU_DISPATCH_MODES_ALL);
}

ساكن فراغ transpose_48bit_simd(ثابت uchar* src, size_t sstep, uchar* dst, size_t dstep, اندازه sz)
{
    ثابت لنډ* src16 = reinterpret_cast<ثابت لنډ*>(src);
    لنډ* dst16 = reinterpret_cast<لنډ*>(dst);

    ثابت size_t sstep_e = sstep / حجم(لنډ);
    ثابت size_t dstep_e = dstep / حجم(لنډ);

    ثابت صحيح m = sz.پلنوالی, n = sz.لوړوالی;
    صحيح i = 0;

    لكل (; i <= m - 8; i += 8)
    {
        صحيح j = 0;
        لكل (; j <= n - 8; j += 8)
        {
            v_int16x8 C0_0, C1_0, C2_0;
            v_int16x8 C0_1, C1_1, C2_1;
            v_int16x8 C0_2, C1_2, C2_2;
            v_int16x8 C0_3, C1_3, C2_3;
            v_int16x8 C0_4, C1_4, C2_4;
            v_int16x8 C0_5, C1_5, C2_5;
            v_int16x8 C0_6, C1_6, C2_6;
            v_int16x8 C0_7, C1_7, C2_7;

            v_بارول_deinterleave(src16 + sstep_e*(j+0) + i*3, C0_0, C1_0, C2_0);
            v_بارول_deinterleave(src16 + sstep_e*(j+1) + i*3, C0_1, C1_1, C2_1);
            v_بارول_deinterleave(src16 + sstep_e*(j+2) + i*3, C0_2, C1_2, C2_2);
            v_بارول_deinterleave(src16 + sstep_e*(j+3) + i*3, C0_3, C1_3, C2_3);
            v_بارول_deinterleave(src16 + sstep_e*(j+4) + i*3, C0_4, C1_4, C2_4);
            v_بارول_deinterleave(src16 + sstep_e*(j+5) + i*3, C0_5, C1_5, C2_5);
            v_بارول_deinterleave(src16 + sstep_e*(j+6) + i*3, C0_6, C1_6, C2_6);
            v_بارول_deinterleave(src16 + sstep_e*(j+7) + i*3, C0_7, C1_7, C2_7);

            v_uint16x8 t0, t1, t2, t3, t4, t5, t6, t7;
            v_zip(v_reinterpret_as_u16(C0_0), v_reinterpret_as_u16(C0_1), t0, t1);
            v_zip(v_reinterpret_as_u16(C0_2), v_reinterpret_as_u16(C0_3), t2, t3);
            v_zip(v_reinterpret_as_u16(C0_4), v_reinterpret_as_u16(C0_5), t4, t5);
            v_zip(v_reinterpret_as_u16(C0_6), v_reinterpret_as_u16(C0_7), t6, t7);
            v_uint32x4 u0, u1, u2, u3, u4, u5, u6, u7;
            v_zip(v_reinterpret_as_u32(t0), v_reinterpret_as_u32(t4), u0, u1);
            v_zip(v_reinterpret_as_u32(t1), v_reinterpret_as_u32(t5), u2, u3);
            v_zip(v_reinterpret_as_u32(t2), v_reinterpret_as_u32(t6), u4, u5);
            v_zip(v_reinterpret_as_u32(t3), v_reinterpret_as_u32(t7), u6, u7);
            v_uint32x4 s0, s1, s2, s3, s4, s5, s6, s7;
            v_zip(u0, u4, s0, s1); v_zip(u1, u5, s2, s3);
            v_zip(u2, u6, s4, s5); v_zip(u3, u7, s6, s7);
            v_int16x8 r0_0 = v_reinterpret_as_s16(s0), r0_1 = v_reinterpret_as_s16(s1);
            v_int16x8 r0_2 = v_reinterpret_as_s16(s2), r0_3 = v_reinterpret_as_s16(s3);
            v_int16x8 r0_4 = v_reinterpret_as_s16(s4), r0_5 = v_reinterpret_as_s16(s5);
            v_int16x8 r0_6 = v_reinterpret_as_s16(s6), r0_7 = v_reinterpret_as_s16(s7);

            v_zip(v_reinterpret_as_u16(C1_0), v_reinterpret_as_u16(C1_1), t0, t1);
            v_zip(v_reinterpret_as_u16(C1_2), v_reinterpret_as_u16(C1_3), t2, t3);
            v_zip(v_reinterpret_as_u16(C1_4), v_reinterpret_as_u16(C1_5), t4, t5);
            v_zip(v_reinterpret_as_u16(C1_6), v_reinterpret_as_u16(C1_7), t6, t7);
            v_zip(v_reinterpret_as_u32(t0), v_reinterpret_as_u32(t4), u0, u1);
            v_zip(v_reinterpret_as_u32(t1), v_reinterpret_as_u32(t5), u2, u3);
            v_zip(v_reinterpret_as_u32(t2), v_reinterpret_as_u32(t6), u4, u5);
            v_zip(v_reinterpret_as_u32(t3), v_reinterpret_as_u32(t7), u6, u7);
            v_zip(u0, u4, s0, s1); v_zip(u1, u5, s2, s3);
            v_zip(u2, u6, s4, s5); v_zip(u3, u7, s6, s7);
            v_int16x8 r1_0 = v_reinterpret_as_s16(s0), r1_1 = v_reinterpret_as_s16(s1);
            v_int16x8 r1_2 = v_reinterpret_as_s16(s2), r1_3 = v_reinterpret_as_s16(s3);
            v_int16x8 r1_4 = v_reinterpret_as_s16(s4), r1_5 = v_reinterpret_as_s16(s5);
            v_int16x8 r1_6 = v_reinterpret_as_s16(s6), r1_7 = v_reinterpret_as_s16(s7);

            v_zip(v_reinterpret_as_u16(C2_0), v_reinterpret_as_u16(C2_1), t0, t1);
            v_zip(v_reinterpret_as_u16(C2_2), v_reinterpret_as_u16(C2_3), t2, t3);
            v_zip(v_reinterpret_as_u16(C2_4), v_reinterpret_as_u16(C2_5), t4, t5);
            v_zip(v_reinterpret_as_u16(C2_6), v_reinterpret_as_u16(C2_7), t6, t7);
            v_zip(v_reinterpret_as_u32(t0), v_reinterpret_as_u32(t4), u0, u1);
            v_zip(v_reinterpret_as_u32(t1), v_reinterpret_as_u32(t5), u2, u3);
            v_zip(v_reinterpret_as_u32(t2), v_reinterpret_as_u32(t6), u4, u5);
            v_zip(v_reinterpret_as_u32(t3), v_reinterpret_as_u32(t7), u6, u7);
            v_zip(u0, u4, s0, s1); v_zip(u1, u5, s2, s3);
            v_zip(u2, u6, s4, s5); v_zip(u3, u7, s6, s7);
            v_int16x8 r2_0 = v_reinterpret_as_s16(s0), r2_1 = v_reinterpret_as_s16(s1);
            v_int16x8 r2_2 = v_reinterpret_as_s16(s2), r2_3 = v_reinterpret_as_s16(s3);
            v_int16x8 r2_4 = v_reinterpret_as_s16(s4), r2_5 = v_reinterpret_as_s16(s5);
            v_int16x8 r2_6 = v_reinterpret_as_s16(s6), r2_7 = v_reinterpret_as_s16(s7);

            v_store_interleave(dst16 + dstep_e*(i+0) + j*3, r0_0, r1_0, r2_0);
            v_store_interleave(dst16 + dstep_e*(i+1) + j*3, r0_1, r1_1, r2_1);
            v_store_interleave(dst16 + dstep_e*(i+2) + j*3, r0_2, r1_2, r2_2);
            v_store_interleave(dst16 + dstep_e*(i+3) + j*3, r0_3, r1_3, r2_3);
            v_store_interleave(dst16 + dstep_e*(i+4) + j*3, r0_4, r1_4, r2_4);
            v_store_interleave(dst16 + dstep_e*(i+5) + j*3, r0_5, r1_5, r2_5);
            v_store_interleave(dst16 + dstep_e*(i+6) + j*3, r0_6, r1_6, r2_6);
            v_store_interleave(dst16 + dstep_e*(i+7) + j*3, r0_7, r1_7, r2_7);
        }
        لكل (; j < n; j++)
            لكل (صحيح k = 0; k < 8; k++)
            {
                dst16[dstep_e*(i+k) + j*3 + 0] = src16[sstep_e*j + (i+k)*3 + 0];
                dst16[dstep_e*(i+k) + j*3 + 1] = src16[sstep_e*j + (i+k)*3 + 1];
                dst16[dstep_e*(i+k) + j*3 + 2] = src16[sstep_e*j + (i+k)*3 + 2];
            }
    }
    لكل (; i < m; i++)
        لكل (صحيح j = 0; j < n; j++)
        {
            dst16[dstep_e*i + j*3 + 0] = src16[sstep_e*j + i*3 + 0];
            dst16[dstep_e*i + j*3 + 1] = src16[sstep_e*j + i*3 + 1];
            dst16[dstep_e*i + j*3 + 2] = src16[sstep_e*j + i*3 + 2];
        }
}
#د_که_پای

کينډۍ<ډول_نوم T> ساكن فراغ
transpose_( ثابت uchar* src, size_t sstep, uchar* dst, size_t dstep, اندازه sz )
{
#إذا CV_SIMD128
    بدل (حجم(T))
    {
        حالة 1: transpose_8bit_simd(src, sstep, dst, dstep, sz);  أرجع;
        حالة 2: transpose_16bit_simd(src, sstep, dst, dstep, sz); أرجع;
        حالة 4: transpose_32bit_simd(src, sstep, dst, dstep, sz); أرجع;
        حالة 6: transpose_48bit_simd(src, sstep, dst, dstep, sz); أرجع;
        افتراضي: اكسر;
    }
#د_که_پای

    صحيح i = 0, j, m = sz.پلنوالی, n = sz.لوړوالی;

    #إذا CV_ENABLE_UNROLLED
    لكل(; i <= m - 4; i += 4 )
    {
        T* d0 = (T*)(dst + dstep*i);
        T* d1 = (T*)(dst + dstep*(i+1));
        T* d2 = (T*)(dst + dstep*(i+2));
        T* d3 = (T*)(dst + dstep*(i+3));

        لكل( j = 0; j <= n - 4; j += 4 )
        {
            ثابت T* s0 = (ثابت T*)(src + i*حجم(T) + sstep*j);
            ثابت T* s1 = (ثابت T*)(src + i*حجم(T) + sstep*(j+1));
            ثابت T* s2 = (ثابت T*)(src + i*حجم(T) + sstep*(j+2));
            ثابت T* s3 = (ثابت T*)(src + i*حجم(T) + sstep*(j+3));

            d0[j] = s0[0]; d0[j+1] = s1[0]; d0[j+2] = s2[0]; d0[j+3] = s3[0];
            d1[j] = s0[1]; d1[j+1] = s1[1]; d1[j+2] = s2[1]; d1[j+3] = s3[1];
            d2[j] = s0[2]; d2[j+1] = s1[2]; d2[j+2] = s2[2]; d2[j+3] = s3[2];
            d3[j] = s0[3]; d3[j+1] = s1[3]; d3[j+2] = s2[3]; d3[j+3] = s3[3];
        }

        لكل( ; j < n; j++ )
        {
            ثابت T* s0 = (ثابت T*)(src + i*حجم(T) + j*sstep);
            d0[j] = s0[0]; d1[j] = s0[1]; d2[j] = s0[2]; d3[j] = s0[3];
        }
    }
    #د_که_پای
    لكل( ; i < m; i++ )
    {
        T* d0 = (T*)(dst + dstep*i);
        j = 0;
        #إذا CV_ENABLE_UNROLLED
        لكل(; j <= n - 4; j += 4 )
        {
            ثابت T* s0 = (ثابت T*)(src + i*حجم(T) + sstep*j);
            ثابت T* s1 = (ثابت T*)(src + i*حجم(T) + sstep*(j+1));
            ثابت T* s2 = (ثابت T*)(src + i*حجم(T) + sstep*(j+2));
            ثابت T* s3 = (ثابت T*)(src + i*حجم(T) + sstep*(j+3));

            d0[j] = s0[0]; d0[j+1] = s1[0]; d0[j+2] = s2[0]; d0[j+3] = s3[0];
        }
        #د_که_پای
        لكل( ; j < n; j++ )
        {
            ثابت T* s0 = (ثابت T*)(src + i*حجم(T) + j*sstep);
            d0[j] = s0[0];
        }
    }
}

کينډۍ<ډول_نوم T> ساكن فراغ
transposeI_( uchar* معلومات, size_t ګام, صحيح n )
{
    لكل( صحيح i = 0; i < n; i++ )
    {
        T* لیکه = (T*)(معلومات + ګام*i);
        uchar* data1 = معلومات + i*حجم(T);
        لكل( صحيح j = i+1; j < n; j++ )
            std::swap( لیکه[j], *(T*)(data1 + ګام*j) );
    }
}

تعريفنوع فراغ (*TransposeFunc)( ثابت uchar* src, size_t sstep, uchar* dst, size_t dstep, اندازه sz );
تعريفنوع فراغ (*TransposeInplaceFunc)( uchar* معلومات, size_t ګام, صحيح n );

#تعریف_کړه DEF_TRANSPOSE_FUNC(suffix, type) \
ساكن فراغ transpose_##suffix( ثابت uchar* src, size_t sstep, uchar* dst, size_t dstep, اندازه sz ) \
{ transpose_<type>(src, sstep, dst, dstep, sz); } \
\
ساكن فراغ transposeI_##suffix( uchar* معلومات, size_t ګام, صحيح n ) \
{ transposeI_<type>(معلومات, ګام, n); }

DEF_TRANSPOSE_FUNC(8u, uchar)
DEF_TRANSPOSE_FUNC(16u, ushort)
DEF_TRANSPOSE_FUNC(8uC3, Vec3b)
DEF_TRANSPOSE_FUNC(32s, صحيح)
DEF_TRANSPOSE_FUNC(16uC3, Vec3s)
DEF_TRANSPOSE_FUNC(32sC2, Vec2i)
DEF_TRANSPOSE_FUNC(32sC3, Vec3i)
DEF_TRANSPOSE_FUNC(32sC4, Vec4i)
DEF_TRANSPOSE_FUNC(32sC6, Vec6i)
DEF_TRANSPOSE_FUNC(32sC8, Vec8i)

ساكن TransposeFunc transposeTab[] =
{
    0, transpose_8u, transpose_16u, transpose_8uC3, transpose_32s, 0, transpose_16uC3, 0,
    transpose_32sC2, 0, 0, 0, transpose_32sC3, 0, 0, 0, transpose_32sC4,
    0, 0, 0, 0, 0, 0, 0, transpose_32sC6, 0, 0, 0, 0, 0, 0, 0, transpose_32sC8
};

nd::TransposeFunc nd::getTransposeFunc(size_t esz)
{
    أرجع esz < حجم(transposeTab)/حجم(transposeTab[0]) ? transposeTab[esz] : تش_اشاره;
}

ساكن TransposeInplaceFunc transposeInplaceTab[] =
{
    0, transposeI_8u, transposeI_16u, transposeI_8uC3, transposeI_32s, 0, transposeI_16uC3, 0,
    transposeI_32sC2, 0, 0, 0, transposeI_32sC3, 0, 0, 0, transposeI_32sC4,
    0, 0, 0, 0, 0, 0, 0, transposeI_32sC6, 0, 0, 0, 0, 0, 0, 0, transposeI_32sC8
};

#که_تعریف_شوی HAVE_OPENCL

ساكن منطقي ocl_transpose( ننوت_Array _src, OutputArray _dst )
{
    ثابت ocl::Device & dev = ocl::Device::getDefault();
    ثابت صحيح TILE_DIM = 32, بلاک_لیکې = 8;
    صحيح type = _src.type(), cn = CV_MAT_CN(type), ژوروالی = CV_MAT_ژوروالی(type),
        لیکې_فی_WI = dev.isIntel() ? 4 : 1;

    UMat src = _src.getUMat();
    _dst.create(src.ستون_شمېر, src.لیکې, type);
    UMat dst = _dst.getUMat();

    String kernelName("transpose");
    منطقي inplace = dst.u == src.u;

    إذا (inplace)
    {
        CV_Assert(dst.ستون_شمېر == dst.لیکې);
        kernelName += "_inplace";
    }
    وإلا
    {
        // check required local memory size
        size_t required_local_حافظه = (size_t) TILE_DIM*(TILE_DIM+1)*CV_ELEM_اندازه(type);
        إذا (required_local_حافظه > ocl::Device::getDefault().local_Mem_اندازه())
            أرجع خطأ;
    }

    String deviceMacro;
    إذا (dev.isIntel())
        deviceMacro = " -D INTEL_GPU";
    وإلا
        deviceMacro = "";

    ocl::Kernel k(kernelName.c_str(), ocl::core::transpose_oclsrc,
                  format("-D T=%s -D T1=%s -D cn=%d -D TILE_DIM=%d -D BLOCK_ROWS=%d -D rowsPerWI=%d%s%s",
                         ocl::memop_Type_منزل_Str(type), ocl::memop_Type_منزل_Str(ژوروالی),
                         cn, TILE_DIM, بلاک_لیکې, لیکې_فی_WI, inplace ? " -D INPLACE" : "", deviceMacro.c_str()));
    إذا (k.empty())
        أرجع خطأ;

    إذا (inplace)
        k.دلیلونه(ocl::KernelArg::Read_Write_No_اندازه(dst), dst.لیکې);
    وإلا
        k.دلیلونه(ocl::KernelArg::ReadOnly(src),
               ocl::KernelArg::Write_Only_No_اندازه(dst));

    size_t localsize[2]  = { TILE_DIM, بلاک_لیکې };
    size_t globalsize[2] = { (size_t)src.ستون_شمېر, inplace ? ((size_t)src.لیکې + لیکې_فی_WI - 1) / لیکې_فی_WI : (div_پورته((size_t)src.لیکې, TILE_DIM) * بلاک_لیکې) };

    إذا (inplace && dev.isIntel())
    {
        localsize[0] = 16;
        localsize[1] = dev.max_Work_ډله_اندازه() / localsize[0];
    }

    أرجع k.پرلپسې(2, globalsize, localsize, خطأ);
}

#د_که_پای

فراغ transpose( ننوت_Array _src, OutputArray _dst )
{
    CV_INSTRUMENT_REGION();

    صحيح type = _src.type(), esz = CV_ELEM_اندازه(type);
    CV_Assert( _src.dims() <= 2 && esz <= 32 );

    CV_OCL_پرلپسې(_dst.isUMat(),
               ocl_transpose(_src, _dst))

    Mat src = _src.getMat();
    إذا( src.empty() )
    {
        _dst.release();
        أرجع;
    }

    _dst.create(src.ستون_شمېر, src.لیکې, src.type());
    Mat dst = _dst.getMat();

    // handle the case of single-column/single-row matrices, stored in STL vectors.
    إذا( src.لیکې != dst.ستون_شمېر || src.ستون_شمېر != dst.لیکې )
    {
        CV_Assert( src.size() == dst.size() && (src.ستون_شمېر == 1 || src.لیکې == 1) );
        src.copy_منزل(dst);
        أرجع;
    }

    CALL_HAL(transpose2d, cv_hal_transpose2d, src.معلومات, src.ګام, dst.معلومات, dst.ګام, src.ستون_شمېر, src.لیکې, esz);

    إذا( dst.معلومات == src.معلومات )
    {
        TransposeInplaceFunc func = transposeInplaceTab[esz];
        CV_Assert( func != 0 );
        CV_Assert( dst.ستون_شمېر == dst.لیکې );
        func( dst.ptr(), dst.ګام, dst.لیکې );
    }
    وإلا
    {
        TransposeFunc func = transposeTab[esz];
        CV_Assert( func != 0 );
        func( src.ptr(), src.ګام, dst.ptr(), dst.ګام, src.size() );
    }
}


فراغ transpose(ننوت_Array src_, ثابت std::vector<صحيح>& ترتیب, OutputArray dst_)
{
    CV_INSTRUMENT_REGION();

    Mat inp = src_.getMat();
    ثابت صحيح ndims = inp.dims;
    CV_Assert(!(ndims == 0 && inp.empty()));
    CV_CheckEQ(ترتیب.size(), static_cast<size_t>(ndims), "Number of dimensions shouldn't change");

    منطقي کارول_شوی[CV_MAX_DIM] = {خطأ};
    صحيح newShape[CV_MAX_DIM];
    لكل (صحيح i = 0; i < ndims; i++)
    {
        صحيح j = ترتیب[i];
        CV_Check(j, 0 <= j && j < ndims && !کارول_شوی[j], "New order should be a valid permutation of the old one");
        کارول_شوی[j] = ریښتیا;
        newShape[i] = inp.size[j];
    }

    // plain 2D transpose keeps using cv::transpose (HAL, in-place square case);
    // not for an empty input, which cv::transpose would answer by releasing dst
    إذا (ndims == 2 && ترتیب[0] == 1 && !inp.empty() && nd::getTransposeFunc(inp.elem_اندازه()))
    {
        transpose(inp, dst_);
        أرجع;
    }

    dst_.create(ndims, newShape, inp.type());
    Mat out = dst_.getMat();
    إذا (out.ټول() == 0)
        أرجع;
    nd::copy(nd::ترتیب_بدلول(nd::view_د(inp), ترتیب.معلومات()), nd::view_د(out));
}

فراغ transposeND(ننوت_Array src, ثابت std::vector<صحيح>& ترتیب, OutputArray dst)
{
    transpose(src, ترتیب, dst);
}


// Generic scalar fallback
CV_ALWAYS_INLINE فراغ flipHoriz_generic( ثابت uchar* src, size_t sstep, uchar* dst, size_t dstep, اندازه size, size_t esz )
{
    صحيح i, j, حد = (صحيح)(((size.پلنوالی + 1)/2)*esz);
    صحيح لوړوالی = size.لوړوالی;
    Auto_بفر<صحيح> _tab(size.پلنوالی*esz);
    صحيح* tab = _tab.معلومات();

    لكل( i = 0; i < size.پلنوالی; i++ )
        لكل( size_t k = 0; k < esz; k++ )
            tab[i*esz + k] = (صحيح)((size.پلنوالی - i - 1)*esz + k);

    لكل( ; لوړوالی--; src += sstep, dst += dstep )
    {
        لكل( i = 0; i < حد; i++ )
        {
            j = tab[i];
            uchar t0 = src[i], t1 = src[j];
            dst[i] = t1; dst[j] = t0;
        }
    }
}

#إذا CV_SIMD || CV_SIMD_SCALABLE
کينډۍ<ډول_نوم V> CV_ALWAYS_INLINE فراغ flipHoriz_single( ثابت uchar* src, size_t sstep, uchar* dst, size_t dstep, اندازه size )
{
    تعريفنوع ډول_نوم VTraits<V>::lane_type T;
    ثابت صحيح vlanes = VTraits<v_uint8>::vlanes();
    صحيح end = (صحيح)(size.پلنوالی * حجم(T));
    صحيح پلنوالی = (end + 1) / 2;
    صحيح پلنوالی_simd = پلنوالی & -vlanes;
    صحيح لوړوالی = size.لوړوالی;

    CV_DbgAssert(!CV_STRONG_ALIGNMENT || isAligned<حجم(T)>(src, dst));

    لكل( ; لوړوالی--; src += sstep, dst += dstep )
    {
        صحيح i = 0, j = end;
        لكل( ; i < پلنوالی_simd; i += vlanes, j -= vlanes )
        {
            V t0 = vx_بارول((ثابت T*)(src + i));
            V t1 = vx_بارول((ثابت T*)(src + j - vlanes));
            t0 = v_سرچپه(t0);
            t1 = v_سرچپه(t1);
            v_store((T*)(dst + j - vlanes), t0);
            v_store((T*)(dst + i), t1);
        }

        // Scalar tail loop
        إذا (isAligned<حجم(T)>(src, dst))
        {
            لكل ( ; i < پلنوالی; i += حجم(T), j -= حجم(T) )
            {
                T t0 = *((ثابت T*)(src + i));
                T t1 = *((ثابت T*)(src + j - حجم(T)));
                *((T*)(dst + j - حجم(T))) = t0;
                *((T*)(dst + i)) = t1;
            }
        }
        وإلا
        {
            لكل ( ; i < پلنوالی; i += حجم(T), j -= حجم(T) )
            {
                لكل (صحيح k = 0; k < (صحيح)حجم(T); k++)
                {
                    uchar t0 = src[i + k];
                    uchar t1 = src[j + k - حجم(T)];
                    dst[j + k - حجم(T)] = t0;
                    dst[i + k] = t1;
                }
            }
        }
    }
}

// SIMD for C3
کينډۍ<ډول_نوم V>
CV_ALWAYS_INLINE فراغ flipHoriz_c3( ثابت uchar* src, size_t sstep, uchar* dst, size_t dstep, اندازه size )
{
    تعريفنوع ډول_نوم VTraits<V>::lane_type T;
    ثابت صحيح vlanes = VTraits<V>::vlanes();
    ثابت صحيح stride = 3 * حجم(T);
    صحيح پلنوالی = size.پلنوالی;
    صحيح centre = (پلنوالی + 1) / 2;
    صحيح پلنوالی_simd = (centre / vlanes) * vlanes;
    صحيح لوړوالی = size.لوړوالی;

    لكل( ; لوړوالی--; src += sstep, dst += dstep )
    {
        صحيح i = 0;
        لكل( ; i < پلنوالی_simd; i += vlanes )
        {
            V r0, g0, b0;
            v_بارول_deinterleave((ثابت T*)(src + i * stride), r0, g0, b0);
            V r1, g1, b1;
            v_بارول_deinterleave((ثابت T*)(src + (پلنوالی - i - vlanes) * stride), r1, g1, b1);
            r0 = v_سرچپه(r0);
            g0 = v_سرچپه(g0);
            b0 = v_سرچپه(b0);
            r1 = v_سرچپه(r1);
            g1 = v_سرچپه(g1);
            b1 = v_سرچپه(b1);
            v_store_interleave((T*)(dst + (پلنوالی - i - vlanes) * stride), r0, g0, b0);
            v_store_interleave((T*)(dst + i * stride), r1, g1, b1);
        }
        // Scalar tail loop for remaining pixels
        لكل( ; i < centre; i++ )
        {
            صحيح j = پلنوالی - i - 1;
            T c0 = ((ثابت T*)(src + i * stride))[0];
            T c1 = ((ثابت T*)(src + i * stride))[1];
            T c2 = ((ثابت T*)(src + i * stride))[2];
            T c3 = ((ثابت T*)(src + j * stride))[0];
            T c4 = ((ثابت T*)(src + j * stride))[1];
            T c5 = ((ثابت T*)(src + j * stride))[2];
            ((T*)(dst + j * stride))[0] = c0;
            ((T*)(dst + j * stride))[1] = c1;
            ((T*)(dst + j * stride))[2] = c2;
            ((T*)(dst + i * stride))[0] = c3;
            ((T*)(dst + i * stride))[1] = c4;
            ((T*)(dst + i * stride))[2] = c5;
        }
    }
}

// SIMD flip when ESZ multiple of vlanes
کينډۍ<size_t ESZ>
CV_ALWAYS_INLINE فراغ flip_Horiz_vlanes_سمون( ثابت uchar* src, size_t sstep, uchar* dst, size_t dstep, اندازه size)
{
    ثابت صحيح vlanes = VTraits<v_uint8>::vlanes();
    صحيح end = (صحيح)(size.پلنوالی * ESZ);
    صحيح پلنوالی = end / 2;
    صحيح لوړوالی = size.لوړوالی;
    صحيح e_اندازه = (صحيح)ESZ;
    لكل( ; لوړوالی--; src += sstep, dst += dstep )
    {
        لكل( صحيح i = 0, j = end - e_اندازه; i < پلنوالی; i += e_اندازه, j -= e_اندازه )
        {
            لكل( صحيح k = 0; k < e_اندازه; k += vlanes )
            {
                v_uint8 t0 = vx_بارول(src + i + k);
                v_uint8 t1 = vx_بارول(src + j + k);
                v_store(dst + j + k, t0);
                v_store(dst + i + k, t1);
            }
        }
    }
}

#إذا CV_SIMD128
// SIMD flip when ESZ=16 (128-bit)
کينډۍ<size_t ESZ>
CV_ALWAYS_INLINE فراغ flip_Horiz_vlanes_سمون_128( ثابت uchar* src, size_t sstep, uchar* dst, size_t dstep, اندازه size)
{
    ثابت صحيح vlanes16 = VTraits<v_uint8x16>::vlanes();
    صحيح end = (صحيح)(size.پلنوالی * ESZ);
    صحيح پلنوالی = end / 2;
    صحيح لوړوالی = size.لوړوالی;
    لكل( ; لوړوالی--; src += sstep, dst += dstep )
    {
        لكل( صحيح i = 0, j = end - vlanes16; i < پلنوالی; i += vlanes16, j -= vlanes16 )
        {
            v_uint8x16 t0 = v_بارول(src + i);
            v_uint8x16 t1 = v_بارول(src + j);
            v_store(dst + j, t0);
            v_store(dst + i, t1);
        }
    }
}
#د_که_پای // CV_SIMD128

// SIMD flip for ESZ=16,32
کينډۍ<size_t ESZ>
CV_ALWAYS_INLINE فراغ flipHoriz_vlanes_dispatch( ثابت uchar* src, size_t sstep, uchar* dst, size_t dstep, اندازه size )
{
    ثابت صحيح vlanes = VTraits<v_uint8>::vlanes();
#إذا CV_SIMD128
    ثابت صحيح vlanes16 = VTraits<v_uint8x16>::vlanes();
#د_که_پای
    إذا ( (ESZ == (size_t)vlanes) || (ESZ == 2 * (size_t)vlanes))
    {
        flip_Horiz_vlanes_سمون<ESZ>(src, sstep, dst, dstep, size);
        أرجع;
    }
#إذا CV_SIMD128
    وإلا إذا (ESZ == vlanes16)
    {
        flip_Horiz_vlanes_سمون_128<ESZ>(src, sstep, dst, dstep, size);
        أرجع;
    }
#د_که_پای
    flipHoriz_generic(src, sstep, dst, dstep, size, ESZ);
}

#إذا CV_SIMD128
// SIMD flip for ESZ=24 (CV_64FC3)
CV_ALWAYS_INLINE فراغ flipHoriz_24( ثابت uchar* src, size_t sstep, uchar* dst, size_t dstep, اندازه size )
{
    CV_DbgAssert(!CV_STRONG_ALIGNMENT || isAligned<8>(((size_t)src)|((size_t)dst)|sstep|dstep));

    ثابت صحيح lanes16 = 16;
    صحيح end = (صحيح)(size.پلنوالی * 24);
    صحيح پلنوالی = (end + 1) / 2;
    صحيح لوړوالی = size.لوړوالی;
    لكل( ; لوړوالی--; src += sstep, dst += dstep )
    {
        لكل ( صحيح i = 0, j = end; i < پلنوالی; i += lanes16 + 8, j -= lanes16 + 8 )
        {
            v_uint8x16 t0 = v_بارول(src + i);
            uint64_t t2 = *reinterpret_cast<ثابت uint64_t*>(src + i + lanes16);
            v_uint8x16 t1 = v_بارول(src + j - lanes16 - 8);
            uint64_t t3 = *reinterpret_cast<ثابت uint64_t*>(src + j - 8);
            v_store(dst + j - lanes16 - 8, t0);
            *reinterpret_cast<uint64_t*>(dst + j - 8) = t2;
            v_store(dst + i, t1);
            *reinterpret_cast<uint64_t*>(dst + i + lanes16) = t3;
        }
    }
}
#د_که_پای // CV_SIMD128
#د_که_پای // CV_SIMD || CV_SIMD_SCALABLE

ساكن فراغ flipHoriz( ثابت uchar* src, size_t sstep, uchar* dst, size_t dstep, اندازه size, size_t esz )
{
#إذا CV_SIMD || CV_SIMD_SCALABLE
#إذا CV_STRONG_ALIGNMENT
    ثابت size_t alignMark = ((size_t)src)|((size_t)dst)|sstep|dstep;
#وإلا
    ثابت size_t alignMark = 0; // unaligned access is allowed: all the checks below fold to true
#د_که_پای

    // SIMD-optimized dispatch
    بدل(esz)
    {
        حالة 1:                                                                               // CV_8UC1: 8-bit, 1 channel
            flipHoriz_single<v_uint8>(src, sstep, dst, dstep, size); أرجع;
        حالة 2:                                                                               // CV_8UC2, CV_16UC1: 8-bit 2-channel or 16-bit 1-channel
            إذا (isAligned<2>(alignMark)) { flipHoriz_single<v_uint16>(src, sstep, dst, dstep, size); أرجع; }
            اكسر;
        حالة 3:                                                                               // CV_8UC3: 8-bit, 3 channels
            flipHoriz_c3<v_uint8>(src, sstep, dst, dstep, size); أرجع;
        حالة 4:                                                                               // CV_8UC4, CV_16UC2, CV_32SC1, CV_32FC1: 8-bit 4-channel, 16-bit 2-channel, or 32-bit 1-channel
            إذا (isAligned<4>(alignMark)) { flipHoriz_single<v_uint32>(src, sstep, dst, dstep, size); أرجع; }
            اكسر;
        حالة 6:                                                                               // CV_16UC3, CV_16SC3: 16-bit, 3 channels
            إذا (isAligned<2>(alignMark)) { flipHoriz_c3<v_uint16>(src, sstep, dst, dstep, size); أرجع; }
            اكسر;
        حالة 8:                                                                               // CV_16UC4, CV_32SC2, CV_32FC2, CV_64FC1: 16-bit 4-channel, 32-bit 2-channel, or 64-bit 1-channel
            إذا (isAligned<8>(alignMark)) { flipHoriz_single<v_uint64>(src, sstep, dst, dstep, size); أرجع; }
            اكسر;
        حالة 12:                                                                              // CV_32SC3, CV_32FC3: 32-bit, 3 channels
            إذا (isAligned<4>(alignMark)) { flipHoriz_c3<v_uint32>(src, sstep, dst, dstep, size); أرجع; }
            اكسر;
        حالة 16:                                                                              // CV_32SC4, CV_32FC4, CV_64FC2: 32-bit 4-channel or 64-bit 2-channel
            flipHoriz_vlanes_dispatch<16>(src, sstep, dst, dstep, size); أرجع;               // byte-wise access only
#إذا CV_SIMD128
        حالة 24:                                                                              // CV_64FC3: 64-bit, 3 channels
            إذا (isAligned<8>(alignMark)) { flipHoriz_24(src, sstep, dst, dstep, size); أرجع; }
            اكسر;
#د_که_پای
        حالة 32:                                                                              // CV_64FC4: 64-bit, 4 channels
            flipHoriz_vlanes_dispatch<32>(src, sstep, dst, dstep, size); أرجع;               // byte-wise access only
        افتراضي:
            اكسر; // Fall through to generic implementation
    }
#د_که_پای
    // Fallback: generic scalar
    flipHoriz_generic(src, sstep, dst, dstep, size, esz);
}

ساكن فراغ
flipVert( ثابت uchar* src0, size_t sstep, uchar* dst0, size_t dstep, اندازه size, size_t esz )
{
    ثابت uchar* src1 = src0 + (size.لوړوالی - 1)*sstep;
    uchar* dst1 = dst0 + (size.لوړوالی - 1)*dstep;
    size.پلنوالی *= (صحيح)esz;

    لكل( صحيح y = 0; y < (size.لوړوالی + 1)/2; y++, src0 += sstep, src1 -= sstep,
                                                  dst0 += dstep, dst1 -= dstep )
    {
        صحيح i = 0;
#إذا (CV_SIMD || CV_SIMD_SCALABLE)
#إذا CV_STRONG_ALIGNMENT
        إذا (isAligned<حجم(صحيح)>(src0, src1, dst0, dst1))
#د_که_پای
        {
            لكل (; i <= size.پلنوالی - VTraits<v_uint8>::vlanes(); i += VTraits<v_uint8>::vlanes())
            {
                v_int32 t0 = v_reinterpret_as_s32(vx_بارول(src0 + i));
                v_int32 t1 = v_reinterpret_as_s32(vx_بارول(src1 + i));
                v_store(dst0 + i, v_reinterpret_as_u8(t1));
                v_store(dst1 + i, v_reinterpret_as_u8(t0));
            }
        }
#إذا CV_STRONG_ALIGNMENT
        وإلا
        {
            لكل (; i <= size.پلنوالی - VTraits<v_uint8>::vlanes(); i += VTraits<v_uint8>::vlanes())
            {
                v_uint8 t0 = vx_بارول(src0 + i);
                v_uint8 t1 = vx_بارول(src1 + i);
                v_store(dst0 + i, t1);
                v_store(dst1 + i, t0);
            }
        }
#د_که_پای
#د_که_پای

        إذا (isAligned<حجم(صحيح)>(src0, src1, dst0, dst1))
        {
            لكل( ; i <= size.پلنوالی - 16; i += 16 )
            {
                صحيح t0 = ((صحيح*)(src0 + i))[0];
                صحيح t1 = ((صحيح*)(src1 + i))[0];

                ((صحيح*)(dst0 + i))[0] = t1;
                ((صحيح*)(dst1 + i))[0] = t0;

                t0 = ((صحيح*)(src0 + i))[1];
                t1 = ((صحيح*)(src1 + i))[1];

                ((صحيح*)(dst0 + i))[1] = t1;
                ((صحيح*)(dst1 + i))[1] = t0;

                t0 = ((صحيح*)(src0 + i))[2];
                t1 = ((صحيح*)(src1 + i))[2];

                ((صحيح*)(dst0 + i))[2] = t1;
                ((صحيح*)(dst1 + i))[2] = t0;

                t0 = ((صحيح*)(src0 + i))[3];
                t1 = ((صحيح*)(src1 + i))[3];

                ((صحيح*)(dst0 + i))[3] = t1;
                ((صحيح*)(dst1 + i))[3] = t0;
            }

            لكل( ; i <= size.پلنوالی - 4; i += 4 )
            {
                صحيح t0 = ((صحيح*)(src0 + i))[0];
                صحيح t1 = ((صحيح*)(src1 + i))[0];

                ((صحيح*)(dst0 + i))[0] = t1;
                ((صحيح*)(dst1 + i))[0] = t0;
            }
        }

        لكل( ; i < size.پلنوالی; i++ )
        {
            uchar t0 = src0[i];
            uchar t1 = src1[i];

            dst0[i] = t1;
            dst1[i] = t0;
        }
    }
}

#که_تعریف_شوی HAVE_OPENCL

تعداد { FLIP_ستون_شمېر = 1 << 0, FLIP_لیکې = 1 << 1, FLIP_BOTH = FLIP_لیکې | FLIP_ستون_شمېر };

ساكن منطقي ocl_flip(ننوت_Array _src, OutputArray _dst, صحيح flip_کوډ )
{
    CV_Assert(flip_کوډ >= -1 && flip_کوډ <= 1);

    ثابت ocl::Device & dev = ocl::Device::getDefault();
    صحيح type = _src.type(), ژوروالی = CV_MAT_ژوروالی(type), cn = CV_MAT_CN(type),
            flipType, kercn = std::min(ocl::predict_Optimal_ویکټور_پلنوالی(_src, _dst), 4);

    منطقي doubleSupport = dev.doubleFPConfig() > 0;
    إذا (!doubleSupport && ژوروالی == CV_64F)
        kercn = cn;

    إذا (cn > 4)
        أرجع خطأ;

    اندازه size = _src.size();
    _dst.create(size, type);
    UMat src = _src.getUMat(), dst = _dst.getUMat();
    منطقي inplace = (dst.u == src.u);

    String kernelName;
    إذا (flip_کوډ == 0)
        kernelName = "arithm_flip_rows", flipType = FLIP_لیکې;
    وإلا إذا (flip_کوډ > 0)
        kernelName = "arithm_flip_cols", flipType = FLIP_ستون_شمېر;
    وإلا
        kernelName = "arithm_flip_rows_cols", flipType = FLIP_BOTH;

    إذا(inplace)
        kernelName += "_inplace";

    صحيح px_فی_WIy = (dev.isIntel() && (dev.type() & ocl::Device::TYPE_GPU)) ? 4 : 1;
    kercn = (cn!=3 || flipType == FLIP_لیکې) ? std::max(kercn, cn) : cn;
    ثابت صحيح TILE_اندازه = 32, بلاک_لیکې = 8;

    ocl::Kernel k(kernelName.c_str(), ocl::core::flip_oclsrc,
                format( "-D T=%s -D T1=%s -D DEPTH=%d -D cn=%d -D PIX_PER_WI_Y=%d -D kercn=%d -D TILE_SIZE=%d -D BLOCK_ROWS=%d%s",
                kercn != cn ? ocl::type_منزل_Str(CV_MAKE_TYPE(ژوروالی, kercn)) : ocl::vecop_Type_منزل_Str(CV_MAKE_TYPE(ژوروالی, kercn)),
                kercn != cn ? ocl::type_منزل_Str(ژوروالی) : ocl::vecop_Type_منزل_Str(ژوروالی), ژوروالی, cn, px_فی_WIy, kercn, TILE_اندازه, بلاک_لیکې,
                inplace ? " -D INPLACE" : ""));
    إذا (k.empty())
        أرجع خطأ;

    صحيح ستون_شمېر = size.پلنوالی * cn / kercn, لیکې = size.لوړوالی;
    صحيح work_ستون_شمېر = flipType == FLIP_ستون_شمېر ? (ستون_شمېر + 1) >> 1 : ستون_شمېر;
    صحيح work_لیکې = flipType & FLIP_لیکې ? (لیکې + 1) >> 1 : لیکې;

    إذا (inplace)
    {
        k.دلیلونه(ocl::KernelArg::Read_Write_No_اندازه(dst), لیکې, ستون_شمېر);

        صحيح gs_ستون_شمېر, gs_لیکې;
        إذا (flipType == FLIP_ستون_شمېر)
        {
            gs_ستون_شمېر = work_ستون_شمېر;
            gs_لیکې = لیکې;
        }
        وإلا إذا (flipType == FLIP_لیکې)
        {
            gs_ستون_شمېر = ستون_شمېر;
            gs_لیکې = work_لیکې;
        }
        وإلا // FLIP_BOTH
        {
            gs_ستون_شمېر = ستون_شمېر;
            gs_لیکې = لیکې;
        }

        size_t globalsize[2] = {
            (size_t)div_پورته(gs_ستون_شمېر, TILE_اندازه) * TILE_اندازه,
            (size_t)div_پورته(gs_لیکې, TILE_اندازه) * بلاک_لیکې
        };
        size_t localsize[2] = { TILE_اندازه, بلاک_لیکې };
        أرجع k.پرلپسې(2, globalsize, localsize, خطأ);
    }
    وإلا
    {
        k.دلیلونه(ocl::KernelArg::Read_Only_No_اندازه(src),
           ocl::KernelArg::WriteOnly(dst, cn, kercn), work_لیکې, work_ستون_شمېر);

        size_t max_Work_ډله_اندازه = dev.max_Work_ډله_اندازه();
        CV_Assert(max_Work_ډله_اندازه % 4 == 0);

        size_t globalsize[2] = { (size_t)work_ستون_شمېر, ((size_t)work_لیکې + px_فی_WIy - 1) / px_فی_WIy };
        size_t localsize[2]  = { max_Work_ډله_اندازه / 4, 4 };
        أرجع k.پرلپسې(2, globalsize, (flipType == FLIP_ستون_شمېر) && !dev.isIntel() ? localsize : عدم, خطأ);
    }
}

#د_که_پای

فراغ flip( ننوت_Array _src, OutputArray _dst, صحيح flip_ډېرپېښ )
{
    CV_INSTRUMENT_REGION();

    CV_Assert( _src.dims() <= 2 );
    اندازه size = _src.size();

    إذا (flip_ډېرپېښ < 0)
    {
        إذا (size.پلنوالی == 1)
            flip_ډېرپېښ = 0;
        إذا (size.لوړوالی == 1)
            flip_ډېرپېښ = 1;
    }

    إذا ((size.پلنوالی == 1 && flip_ډېرپېښ > 0) ||
        (size.لوړوالی == 1 && flip_ډېرپېښ == 0))
    {
        أرجع _src.copy_منزل(_dst);
    }

    CV_OCL_پرلپسې( _dst.isUMat(), ocl_flip(_src, _dst, flip_ډېرپېښ))

    Mat src = _src.getMat();
    صحيح type = src.type();
    _dst.create( size, type );
    Mat dst = _dst.getMat();

    CALL_HAL(flip, cv_hal_flip, type, src.ptr(), src.ګام, src.ستون_شمېر, src.لیکې,
             dst.ptr(), dst.ګام, flip_ډېرپېښ);

    size_t esz = CV_ELEM_اندازه(type);

    إذا( flip_ډېرپېښ <= 0 )
        flipVert( src.ptr(), src.ګام, dst.ptr(), dst.ګام, src.size(), esz );
    وإلا
        flipHoriz( src.ptr(), src.ګام, dst.ptr(), dst.ګام, src.size(), esz );

    إذا( flip_ډېرپېښ < 0 )
        flipHoriz( dst.ptr(), dst.ګام, dst.ptr(), dst.ګام, dst.size(), esz );
}

// In-place flip of a continuous array: swap the slices along the axis, no temporary copy.
ساكن فراغ flipNDInplace(Mat& m, صحيح axis)
{
    size_t nouter = 1;
    لكل (صحيح i = 0; i < axis; i++)
        nouter *= (size_t)m.size[i];
    ثابت صحيح n = m.size[axis];
    ثابت size_t ګام = m.ګام[axis];
    uchar* معلومات = m.ptr();
    لكل (size_t i = 0; i < nouter; i++, معلومات += n*ګام)
        لكل (صحيح j = 0, k = n - 1; j < k; j++, k--)
            std::swap_ranges(معلومات + j*ګام, معلومات + (j + 1)*ګام, معلومات + k*ګام);
}

فراغ flipND(ننوت_Array _src, OutputArray _dst, صحيح _axis)
{
    CV_INSTRUMENT_REGION();

    Mat src = _src.getMat();

    // verify axis
    صحيح ndim = src.dims;
    CV_CheckLT(_axis, ndim, "flipND: given axis is out of range");
    CV_CheckGE(_axis, -ndim, "flipND: given axis is out of range");
    صحيح axis = (_axis + ndim) % ndim;

    _dst.create(ndim, src.size.p, src.type());
    Mat dst = _dst.getMat();
    إذا (dst.معلومات == src.معلومات && src.isContinuous() && dst.isContinuous())
    {
        flipNDInplace(dst, axis);
        أرجع;
    }

    nd::View v = nd::view_د(src);
    nd::flip(v, axis);
    nd::copy(v, nd::view_د(dst));
}

فراغ concat(ننوت_Array_د_Arrays _src, صحيح axis, OutputArray _dst)
{
    CV_INSTRUMENT_REGION();

    std::vector<Mat> src;
    _src.get_Mat_ویکټور(src);
    CV_Assert(!src.empty());

    ثابت Mat& src0 = src[0];
    ثابت صحيح ndims = src0.dims, type = src0.type(), ninputs = (صحيح)src.size();
    CV_CheckGE(ndims, 1, "concat: 0-dimensional arrays are not supported");
    CV_CheckGE(axis, -ndims, "concat: axis is out of range");
    CV_CheckLT(axis, ndims, "concat: axis is out of range");
    إذا (axis < 0)
        axis += ndims;

    صحيح بهر_Shape[CV_MAX_DIM];
    لكل (صحيح i = 0; i < ndims; i++)
        بهر_Shape[i] = src0.size[i];
    int64 axis_اندازه = 0;
    لكل (صحيح k = 0; k < ninputs; k++)
    {
        ثابت Mat& m = src[k];
        CV_CheckEQ(m.type(), type, "concat: all the input arrays must have the same type");
        CV_CheckEQ(m.dims, ndims, "concat: all the input arrays must have the same number of dimensions");
        لكل (صحيح i = 0; i < ndims; i++)
            إذا (i != axis)
                CV_CheckEQ(m.size[i], بهر_Shape[i], "concat: the input shapes must match except along the axis");
        axis_اندازه += m.size[axis];
    }
    CV_CheckLE((size_t)axis_اندازه, (size_t)لوی_عدد, "concat: the output is too big");
    بهر_Shape[axis] = (صحيح)axis_اندازه;

    _dst.create(ndims, بهر_Shape, type);
    Mat dst = _dst.getMat();

    Auto_بفر<nd::View, 4> sv(ninputs), dv(ninputs);
    ثابت nd::View dst0 = nd::view_د(dst);
    صحيح ofs = 0;
    لكل (صحيح k = 0; k < ninputs; k++)
    {
        ثابت صحيح sz = src[k].size[axis];
        sv[k] = nd::view_د(src[k]);
        dv[k] = dst0;
        nd::ټوټه_2(dv[k], axis, ofs, 1, sz);
        ofs += sz;
    }
    nd::copyBatch(sv.معلومات(), dv.معلومات(), ninputs);
}

فراغ وېشل(ننوت_Array _src, صحيح axis, ثابت std::vector<صحيح>& sizes, Output_Array_د_Arrays _dst)
{
    CV_INSTRUMENT_REGION();

    Mat src = _src.getMat();
    ثابت صحيح ndims = src.dims, type = src.type(), noutputs = (صحيح)sizes.size();
    CV_CheckGE(ndims, 1, "split: 0-dimensional arrays are not supported");
    CV_CheckGE(axis, -ndims, "split: axis is out of range");
    CV_CheckLT(axis, ndims, "split: axis is out of range");
    إذا (axis < 0)
        axis += ndims;
    CV_Assert(noutputs > 0);

    int64 axis_اندازه = 0;
    لكل (صحيح k = 0; k < noutputs; k++)
    {
        CV_CheckGE(sizes[k], 0, "split: the sizes must be non-negative");
        axis_اندازه += sizes[k];
    }
    CV_CheckEQ((size_t)axis_اندازه, (size_t)src.size[axis], "split: the sizes must sum up to the size of the axis");

    صحيح بهر_Shape[CV_MAX_DIM];
    لكل (صحيح i = 0; i < ndims; i++)
        بهر_Shape[i] = src.size[i];

    _dst.create(noutputs, 1, type, -1, ریښتیا);
    Auto_بفر<nd::View, 4> sv(noutputs), dv(noutputs);
    std::vector<Mat> dst(noutputs);
    ثابت nd::View src0 = nd::view_د(src);
    صحيح ofs = 0;
    لكل (صحيح k = 0; k < noutputs; k++)
    {
        بهر_Shape[axis] = sizes[k];
        _dst.create(ndims, بهر_Shape, type, k);
        dst[k] = _dst.getMat(k);
        sv[k] = src0;
        nd::ټوټه_2(sv[k], axis, ofs, 1, sizes[k]);
        dv[k] = nd::view_د(dst[k]);
        ofs += sizes[k];
    }
    nd::copyBatch(sv.معلومات(), dv.معلومات(), noutputs);
}

فراغ repeat(ننوت_Array _src, ثابت std::vector<صحيح>& repeats, OutputArray _dst)
{
    CV_INSTRUMENT_REGION();

    Mat src = _src.getMat();
    ثابت صحيح ndims = src.dims;
    CV_Assert(!(ndims == 0 && src.empty()));
    CV_CheckEQ(repeats.size(), (size_t)ndims, "repeat: the number of repeats must match the number of dimensions");

    صحيح بهر_Shape[CV_MAX_DIM];
    لكل (صحيح i = 0; i < ndims; i++)
    {
        CV_CheckGE(repeats[i], 0, "repeat: the repeats must be non-negative");
        int64 sz = (int64)src.size[i]*repeats[i];
        CV_CheckLE((size_t)sz, (size_t)لوی_عدد, "repeat: the output is too big");
        بهر_Shape[i] = (صحيح)sz;
    }

    _dst.create(ndims, بهر_Shape, src.type());
    Mat dst = _dst.getMat();
    إذا (dst.ټول() == 0 || src.empty())
        أرجع;

    // Each repeated output axis is split into (repeat index, position), the former
    // with source step 0, so the whole tile is one broadcasting copy.
    nd::View sv, dv;
    sv.معلومات = src.معلومات;
    dv.معلومات = dst.معلومات;
    sv.esz = dv.esz = src.elem_اندازه();
    صحيح n = 0;
    لكل (صحيح i = 0; i < ndims; i++)
    {
        ثابت صحيح sz = src.size[i], r = repeats[i];
        إذا (r != 1)
        {
            CV_CheckLT(n, (صحيح)nd::MAX_VIEW_DIMS, "repeat: too many dimensions");
            sv.size[n] = dv.size[n] = r;
            sv.ګام[n] = 0;
            dv.ګام[n] = (ptrdiff_t)dst.ګام[i]*sz;
            n++;
        }
        إذا (sz != 1 || r == 1)
        {
            CV_CheckLT(n, (صحيح)nd::MAX_VIEW_DIMS, "repeat: too many dimensions");
            sv.size[n] = dv.size[n] = sz;
            sv.ګام[n] = (ptrdiff_t)src.ګام[i];
            dv.ګام[n] = (ptrdiff_t)dst.ګام[i];
            n++;
        }
    }
    sv.dims = dv.dims = n;
    nd::copy(sv, dv);
}

فراغ ټوټه_2(ننوت_Array _src, ثابت std::vector<صحيح>& starts, ثابت std::vector<صحيح>& ends,
             ثابت std::vector<صحيح>& ګامونه, OutputArray _dst)
{
    CV_INSTRUMENT_REGION();

    Mat src = _src.getMat();
    ثابت صحيح ndims = src.dims, nslices = (صحيح)starts.size();
    CV_Assert(!(ndims == 0 && src.empty()));
    CV_CheckLE(nslices, ndims, "slice: too many starts");
    CV_CheckEQ(ends.size(), starts.size(), "slice: starts and ends must have the same size");
    CV_Check(ګامونه.size(), ګامونه.empty() || ګامونه.size() == starts.size(),
             "slice: steps must be empty or have the same size as starts");

    nd::View sv = nd::view_د(src);
    صحيح بهر_Shape[CV_MAX_DIM];
    لكل (صحيح i = 0; i < ndims; i++)
        بهر_Shape[i] = src.size[i];

    لكل (صحيح i = 0; i < nslices; i++)
    {
        ثابت صحيح n = src.size[i], پیل = starts[i], end = ends[i];
        ثابت صحيح ګام = ګامونه.empty() ? 1 : ګامونه[i];
        CV_CheckNE(ګام, 0, "slice: step must not be zero");
        صحيح شمېر_2;
        إذا (ګام > 0)
        {
            CV_Check(پیل, 0 <= پیل && پیل <= n, "slice: start is out of range");
            CV_Check(end, 0 <= end && end <= n, "slice: end is out of range");
            شمېر_2 = end > پیل ? (صحيح)(((int64)end - پیل + ګام - 1)/ګام) : 0;
        }
        وإلا
        {
            CV_Check(پیل, -1 <= پیل && پیل < n, "slice: start is out of range");
            CV_Check(end, -1 <= end && end < n, "slice: end is out of range");
            شمېر_2 = پیل > end ? (صحيح)(((int64)پیل - end - ګام - 1)/(-ګام)) : 0;
        }
        nd::ټوټه_2(sv, i, پیل, ګام, شمېر_2);
        بهر_Shape[i] = شمېر_2;
    }

    _dst.create(ndims, بهر_Shape, src.type());
    Mat dst = _dst.getMat();
    إذا (dst.ټول() == 0 || src.empty())
        أرجع;
    nd::copy(sv, nd::view_د(dst));
}

/*
    This function first prepends 1 to each tensor shape to have a common max_ndims dimension, then flatten non-broadcast dimensions.
*/
ساكن منطقي flatten_for_خپرونه(صحيح narrays, صحيح max_ndims, ثابت صحيح* ndims, ثابت صحيح** orig_shape,
                                   صحيح** flatten_shape, size_t** flatten_ګام) {
    صحيح i, j, k;

    // step 1.
    // * make all inputs and the output max_ndims-dimensional.
    // * compute proper step's
    لكل (i = max_ndims - 1; i >= 0; i-- ) {
        لكل (k = 0; k < narrays; k++) {
            j = ndims[k] - (max_ndims - i);
            صحيح sz_i = j >= 0 ? orig_shape[k][j] : 1;
            size_t st_i = i == max_ndims - 1 ? 1 : flatten_ګام[k][i+1] * flatten_shape[k][i+1];
            flatten_shape[k][i] = sz_i;
            flatten_ګام[k][i] = st_i;
            إذا (flatten_shape[k][i] == 0)
                أرجع خطأ;
        }
    }

    // step 2. Let's do the flattening first,
    // since we'd need proper values of steps to check continuity.
    // this loop is probably the most tricky part
    // in the whole implementation of broadcasting.
    j = max_ndims-1;
    لكل (i = j - 1; i >= 0; i--) {
        منطقي all_contiguous = ریښتیا, all_scalars = ریښتیا, all_consistent = ریښتیا;
        لكل(k = 0; k < narrays; k++) {
            size_t st = flatten_ګام[k][j] * flatten_shape[k][j];
            منطقي prev_scalar = flatten_shape[k][j] == 1;
            منطقي scalar = flatten_shape[k][i] == 1;
            all_contiguous = all_contiguous && (st == flatten_ګام[k][i]);
            all_scalars = all_scalars && scalar;
            all_consistent = all_consistent && (scalar == prev_scalar);
        }
        إذا (all_contiguous && (all_consistent || all_scalars)) {
            لكل(k = 0; k < narrays; k++)
                flatten_shape[k][j] *= flatten_shape[k][i];
        } وإلا {
            j--;
            إذا (i < j) {
                لكل(k = 0; k < narrays; k++) {
                    flatten_shape[k][j] = flatten_shape[k][i];
                    flatten_ګام[k][j] = flatten_ګام[k][i];
                }
            }
        }
    }

    // step 3. Set some step's to 0's.
    لكل (i = max_ndims-1; i >= j; i--) {
        لكل (k = 0; k < narrays; k++)
            flatten_ګام[k][i] = flatten_shape[k][i] == 1 ? 0 : flatten_ګام[k][i];
    }
    لكل (; i >= 0; i--) {
        لكل (k = 0; k < narrays; k++) {
            flatten_ګام[k][i] = 0;
            flatten_shape[k][i] = 1;
        }
    }
    أرجع ریښتیا;
}

فراغ خپرونه(ننوت_Array _src, ننوت_Array _shape, OutputArray _dst) {
    CV_INSTRUMENT_REGION();

    Mat src = _src.getMat();
    CV_CheckTrue(src.isContinuous(), "broadcast: input array must be contiguous");
    CV_CheckChannelsEQ(src.channels(), 1, "broadcast: input array must be single channel");

    Mat shape = _shape.getMat();
    CV_CheckTypeEQ(shape.type(), CV_32S, "broadcast: target shape must be of type int32");
    ثابت خپلکاری dims_shape = static_cast<صحيح>(shape.ټول());
    ثابت خپلکاری *ptr_shape = shape.ptr<صحيح>();

    // check valid shape, 1D/0D Mat would fail in the following checks
    ثابت خپلکاری dims_src = src.dims;
    CV_CheckLE(dims_src, dims_shape,
               "broadcast: dimension of input array must be less than or equal to dimension of target shape");
    std::vector<صحيح> shape_src{src.size.p, src.size.p + dims_src};
    إذا (shape_src.size() < static_cast<size_t>(dims_shape)) {
        shape_src.insert(shape_src.begin(), dims_shape - shape_src.size(), 1);
    }
    لكل (صحيح i = 0; i < static_cast<صحيح>(shape_src.size()); ++i) {
        ثابت خپلکاری *shape_موخه = ptr_shape;
        إذا (shape_src[i] != 1) {
            CV_CheckEQ(shape_src[i], shape_موخه[i], "target shape must be equal to input shape or 1");
        }
    }

    // impl
    _dst.create(dims_shape, shape.ptr<صحيح>(), src.type());
    Mat dst = _dst.getMat();
    إذا (dst.ټول() == 0)
        أرجع;
    cv::Auto_بفر<صحيح> is_same_shape(dims_shape, 0);
    لكل (صحيح i = 0; i < static_cast<صحيح>(shape_src.size()); ++i) {
        إذا (shape_src[i] == ptr_shape[i]) {
            is_same_shape[i] = 1;
        }
    }
    // copy if same shape
    إذا (std::accumulate(is_same_shape.begin(), is_same_shape.end(), 1, std::multiplies<صحيح>()) != 0) {
        ثابت خپلکاری *p_src = src.ptr<ثابت حرف>();
        خپلکاری *p_dst = dst.ptr<حرف>();
        std::حافظه_نقل(p_dst, p_src, dst.ټول() * dst.elem_اندازه());
        أرجع;
    }
    // other cases
    صحيح max_ndims = std::max(dims_src, dims_shape);
    إذا (max_ndims < 2 && src.ټول() == 1) {
        ثابت حرف* p_src = src.ptr<ثابت حرف>();
        حرف* p_dst = dst.ptr<حرف>();
        size_t esz = src.elem_اندازه();
        لكل (size_t j = 0; j < dst.ټول(); j++)
            std::حافظه_نقل(p_dst + j * esz, p_src, esz);
        أرجع;
    }
    ثابت صحيح all_ndims[2] = {src.dims, dst.dims};
    ثابت صحيح* orig_shapes[2] = {src.size.p, dst.size.p};
    cv::Auto_بفر<size_t> buff(max_ndims * 4);
    صحيح* flatten_shapes[2] = {(صحيح*)buff.معلومات(), (صحيح*)(buff.معلومات() + max_ndims)};
    size_t* flatten_ګامونه[2] = {(size_t*)(buff.معلومات() + 2 * max_ndims), (size_t*)(buff.معلومات() + 3 * max_ndims)};
    إذا (flatten_for_خپرونه(2, max_ndims, all_ndims, orig_shapes, flatten_shapes, flatten_ګامونه)) {
        size_t src_dp = flatten_ګامونه[0][max_ndims - 1];
        size_t dst_dp = flatten_ګامونه[1][max_ndims - 1];
        CV_Assert(dst_dp == 1 || dst_dp == 0);
        CV_Assert(max_ndims >= 2); // >= 3?
        size_t rowstep_src = flatten_ګامونه[0][max_ndims - 2];
        size_t rowstep_dst = flatten_ګامونه[1][max_ndims - 2];
        ثابت حرف* ptr_src = src.ptr<ثابت حرف>();
        حرف* ptr_dst = dst.ptr<حرف>();
        size_t esz = src.elem_اندازه();
        صحيح nrows = flatten_shapes[1][max_ndims - 2];
        صحيح ncols = flatten_shapes[1][max_ndims - 1];
        صحيح nplanes = 1;
        CV_Check(esz, esz == 1 || esz == 2 || esz == 4 || esz == 8, "broadcast: not supported data type");

        لكل (صحيح k = 0; k < max_ndims - 2; k++) {
            nplanes *= flatten_shapes[1][k];
        }
        لكل (صحيح plane_idx = 0; plane_idx < nplanes; plane_idx++) {
            size_t دننه_واټن_src = 0, دننه_واټن_dst = 0;
            size_t idx = (size_t)plane_idx;
            لكل (صحيح k = max_ndims - 3; k >= 0; k--) {
                size_t prev_idx = idx / flatten_shapes[1][k];
                size_t i_k = (صحيح)(idx - prev_idx * flatten_shapes[1][k]);
                دننه_واټن_src += i_k * flatten_ګامونه[0][k];
                دننه_واټن_dst += i_k * flatten_ګامونه[1][k];
                idx = prev_idx;
            }

            #تعریف_کړه OPENCV_CORE_خپرونه_LOOP(_Tp) \
                لكل (صحيح i = 0; i < nrows; i++) {   \
                    ثابت _Tp *ptr_src_ = (ثابت _Tp*)ptr_src + دننه_واټن_src + rowstep_src * i; \
                    _Tp *ptr_dst_ = (_Tp*)ptr_dst + دننه_واټن_dst + rowstep_dst * i; \
                    إذا (src_dp == 1) { \
                        لكل (صحيح j = 0; j < ncols; j++) { \
                            ptr_dst_[j] = ptr_src_[j]; \
                        } \
                    } وإلا { \
                        _Tp x = *ptr_src_; \
                        لكل (صحيح j = 0; j < ncols; j++) { \
                            ptr_dst_[j] = x; \
                        } \
                    } \
                }

            إذا (esz == 1) {
                OPENCV_CORE_خپرونه_LOOP(int8_t);
            } وإلا إذا (esz == 2) {
                OPENCV_CORE_خپرونه_LOOP(int16_t);
            } وإلا إذا (esz == 4) {
                OPENCV_CORE_خپرونه_LOOP(int32_t);
            } وإلا إذا (esz == 8) {
                OPENCV_CORE_خپرونه_LOOP(int64_t);
            } وإلا {
                CV_Error(cv::Error::StsNotImplemented, "");
            }
            #تعریف_لغوه OPENCV_CORE_خپرونه_LOOP
        }
    } وإلا {
        إذا (dst.ټول() == 0)
            أرجع;
        // initial copy (src to dst)
        std::vector<size_t> ګام_src{src.ګام.p, src.ګام.p + dims_src};
        إذا (ګام_src.size() < static_cast<size_t>(dims_shape)) {
            ګام_src.insert(ګام_src.begin(), dims_shape - ګام_src.size(), ګام_src[0]);
        }
        لكل (size_t i = 0; i < src.ټول(); ++i) {
            size_t t = i;
            size_t src_دننه_واټن = 0, dst_دننه_واټن = 0;
            لكل (صحيح j = static_cast<صحيح>(shape_src.size() - 1); j >= 0; --j) {
                size_t idx = t / shape_src[j];
                size_t offset = static_cast<size_t>(t - idx * shape_src[j]);
                src_دننه_واټن += offset * ګام_src[j];
                dst_دننه_واټن += offset * dst.ګام[j];
                t = idx;
            }
            ثابت خپلکاری *p_src = src.ptr<ثابت حرف>();
            خپلکاری *p_dst = dst.ptr<حرف>();
            std::حافظه_نقل(p_dst + dst_دننه_واټن, p_src + src_دننه_واټن, dst.elem_اندازه());
        }
        // broadcast copy (dst inplace)
        Auto_بفر<صحيح> cumulative_shape(dims_shape, 1);
        صحيح ټول = static_cast<صحيح>(dst.ټول());
        لكل (صحيح i = dims_shape - 1; i >= 0; --i) {
            cumulative_shape[i] = static_cast<صحيح>(ټول / ptr_shape[i]);
            ټول = cumulative_shape[i];
        }
        لكل (صحيح i = dims_shape - 1; i >= 0; --i) {
            إذا (is_same_shape[i] == 1) {
                استمر;
            }
            خپلکاری ګام = dst.ګام[i];
            خپلکاری *p_dst = dst.ptr<حرف>();
            لكل (صحيح j = 0; j < cumulative_shape[i]; j++) {
                لكل (صحيح k = 0; k < ptr_shape[i] - 1; k++) {
                    std::حافظه_نقل(p_dst + ګام, p_dst, ګام);
                    p_dst += ګام;
                }
                p_dst += ګام;
            }
        }
    }
}

فراغ خپرونه(ننوت_Array _src, ثابت MatShape& _shape, OutputArray _dst)
{
    إذا (_shape.dims < 0) {
        _dst.release();
    } وإلا {
        Mat shape(1, _shape.dims, CV_32S, (صحيح*)_shape.p);
        خپرونه(_src, shape, _dst);
    }
}

ساكن فراغ rotateImpl(ننوت_Array _src, OutputArray _dst, صحيح rotate_ډېرپېښ)
{
    بدل (rotate_ډېرپېښ)
    {
    حالة ROTATE_90_CLOCKWISE:
        transpose(_src, _dst);
        flip(_dst, _dst, 1);
        اكسر;
    حالة ROTATE_180:
        flip(_src, _dst, -1);
        اكسر;
    حالة ROTATE_90_COUNTERCLOCKWISE:
        transpose(_src, _dst);
        flip(_dst, _dst, 0);
        اكسر;
    افتراضي:
        اكسر;
    }
}

فراغ rotate(ننوت_Array _src, OutputArray _dst, صحيح rotate_ډېرپېښ)
{
    CV_Assert(_src.dims() <= 2);
    صحيح angle;

    إذا (_dst.isUMat())
    {
        rotateImpl(_src, _dst, rotate_ډېرپېښ);
        أرجع;
    }

    Mat src = _src.getMat();
    صحيح type = src.type();
    إذا( src.empty() )
    {
        _dst.release();
        أرجع;
    }

    بدل (rotate_ډېرپېښ)
    {
    حالة ROTATE_90_CLOCKWISE:
        _dst.create(src.ستون_شمېر, src.لیکې, type);
        angle = 90;
        اكسر;
    حالة ROTATE_180:
        _dst.create(src.لیکې, src.ستون_شمېر, type);
        angle = 180;
        اكسر;
    حالة ROTATE_90_COUNTERCLOCKWISE:
        _dst.create(src.ستون_شمېر, src.لیکې, type);
        angle = 270;
        اكسر;
    افتراضي:
        _dst.create(src.لیکې, src.ستون_شمېر, type);
        angle = 0;
        اكسر;
    }

    Mat dst = _dst.getMat();
    CALL_HAL(rotate90, cv_hal_rotate90, type, src.ptr(), src.ګام, src.ستون_شمېر, src.لیکې,
             dst.ptr(), dst.ګام, angle);

    // use src (Mat) since _src (InputArray) is updated by _dst.create() when in-place
    rotateImpl(src, _dst, rotate_ډېرپېښ);
}

}  // namespace
