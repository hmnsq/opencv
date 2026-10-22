// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html
// Copyright (C) 2026, Advanced Micro Devices, Inc., all rights reserved.
#অন্তর্ভাব "precomp.hpp"
#অন্তর্ভাব "opencl_kernels_core.hpp"
#অন্তর্ভাব "hal_replacement.hpp"
#অন্তর্ভাব "opencv2/core/detail/dispatch_helper.impl.hpp"

#অন্তর্ভাব "transpose.simd.hpp"
#অন্তর্ভাব "transpose.simd_declarations.hpp"
#অন্তর্ভাব "nd_copy.hpp"

#অন্তর্ভাব <algorithm> // std::swap_ranges
#অন্তর্ভাব <numeric> // std::accumulate

নামাকাশ cv {

////////////////////////////////////// transpose /////////////////////////////////////////
#যদি CV_SIMD128
স্থির শূন্য transpose_8bit_simd(ধ্রুব uchar* src, size_t sstep, uchar* dst, size_t dstep, Size sz)
{
    ধ্রুব পূর্ণাঙ্ক m = sz.width, n = sz.height;
    পূর্ণাঙ্ক i = 0;
    প্রতি (; i <= m - 16; i += 16)
    {
        পূর্ণাঙ্ক j = 0;
        প্রতি (; j <= n - 16; j += 16)
        {
            v_uint8x16 r0  = v_load(src + i + sstep*(j+ 0));
            v_uint8x16 r1  = v_load(src + i + sstep*(j+ 1));
            v_uint8x16 r2  = v_load(src + i + sstep*(j+ 2));
            v_uint8x16 r3  = v_load(src + i + sstep*(j+ 3));
            v_uint8x16 r4  = v_load(src + i + sstep*(j+ 4));
            v_uint8x16 r5  = v_load(src + i + sstep*(j+ 5));
            v_uint8x16 r6  = v_load(src + i + sstep*(j+ 6));
            v_uint8x16 r7  = v_load(src + i + sstep*(j+ 7));
            v_uint8x16 r8  = v_load(src + i + sstep*(j+ 8));
            v_uint8x16 r9  = v_load(src + i + sstep*(j+ 9));
            v_uint8x16 r10 = v_load(src + i + sstep*(j+10));
            v_uint8x16 r11 = v_load(src + i + sstep*(j+11));
            v_uint8x16 r12 = v_load(src + i + sstep*(j+12));
            v_uint8x16 r13 = v_load(src + i + sstep*(j+13));
            v_uint8x16 r14 = v_load(src + i + sstep*(j+14));
            v_uint8x16 r15 = v_load(src + i + sstep*(j+15));

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

            v_uint32x4 v0  = v_combine_নিচের (u0,  u8);
            v_uint32x4 v1  = v_combine_উপরের(u0,  u8);
            v_uint32x4 v2  = v_combine_নিচের (u1,  u9);
            v_uint32x4 v3  = v_combine_উপরের(u1,  u9);
            v_uint32x4 v4  = v_combine_নিচের (u2,  u10);
            v_uint32x4 v5  = v_combine_উপরের(u2,  u10);
            v_uint32x4 v6  = v_combine_নিচের (u3,  u11);
            v_uint32x4 v7  = v_combine_উপরের(u3,  u11);
            v_uint32x4 v8  = v_combine_নিচের (u4,  u12);
            v_uint32x4 v9  = v_combine_উপরের(u4,  u12);
            v_uint32x4 v10 = v_combine_নিচের (u5,  u13);
            v_uint32x4 v11 = v_combine_উপরের(u5,  u13);
            v_uint32x4 v12 = v_combine_নিচের (u6,  u14);
            v_uint32x4 v13 = v_combine_উপরের(u6,  u14);
            v_uint32x4 v14 = v_combine_নিচের (u7,  u15);
            v_uint32x4 v15 = v_combine_উপরের(u7,  u15);

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
        প্রতি (; j < n; j++)
            প্রতি (পূর্ণাঙ্ক k = 0; k < 16; k++)
                dst[dstep*(i+k) + j] = src[i + sstep*j + k];
    }
    প্রতি (; i < m; i++)
        প্রতি (পূর্ণাঙ্ক j = 0; j < n; j++)
            dst[dstep*i + j] = src[i + sstep*j];
}

স্থির শূন্য transpose_16bit_simd(ধ্রুব uchar* src, size_t sstep, uchar* dst, size_t dstep, Size sz)
{
    ধ্রুব ushort* src16 = reinterpret_cast<ধ্রুব ushort*>(src);
    ushort* dst16 = reinterpret_cast<ushort*>(dst);

    ধ্রুব size_t sstep_e = sstep / পরিমাণ(ushort);
    ধ্রুব size_t dstep_e = dstep / পরিমাণ(ushort);

    ধ্রুব পূর্ণাঙ্ক m = sz.width, n = sz.height;
    পূর্ণাঙ্ক i = 0;

    প্রতি (; i <= m - 8; i += 8)
    {
        পূর্ণাঙ্ক j = 0;
        প্রতি (; j <= n - 8; j += 8)
        {
            v_uint16x8 r0 = v_load(src16 + i + sstep_e*(j+0));
            v_uint16x8 r1 = v_load(src16 + i + sstep_e*(j+1));
            v_uint16x8 r2 = v_load(src16 + i + sstep_e*(j+2));
            v_uint16x8 r3 = v_load(src16 + i + sstep_e*(j+3));
            v_uint16x8 r4 = v_load(src16 + i + sstep_e*(j+4));
            v_uint16x8 r5 = v_load(src16 + i + sstep_e*(j+5));
            v_uint16x8 r6 = v_load(src16 + i + sstep_e*(j+6));
            v_uint16x8 r7 = v_load(src16 + i + sstep_e*(j+7));

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
        প্রতি (; j < n; j++)
            প্রতি (পূর্ণাঙ্ক k = 0; k < 8; k++)
                dst16[dstep_e*(i+k) + j] = src16[i + sstep_e*j + k];
    }
    প্রতি (; i < m; i++)
        প্রতি (পূর্ণাঙ্ক j = 0; j < n; j++)
            dst16[dstep_e*i + j] = src16[i + sstep_e*j];
}

স্থির শূন্য transpose_32bit_simd(ধ্রুব uchar* src, size_t sstep, uchar* dst, size_t dstep, Size sz)
{
    // Cache-blocked + AVX2-aware path lives in transpose.simd.hpp; pick the
    // best variant available at runtime.
    CV_CPU_DISPATCH(transpose_32bit_blocks_simd, (src, sstep, dst, dstep, sz),
                    CV_CPU_DISPATCH_MODES_ALL);
}

স্থির শূন্য transpose_48bit_simd(ধ্রুব uchar* src, size_t sstep, uchar* dst, size_t dstep, Size sz)
{
    ধ্রুব short* src16 = reinterpret_cast<ধ্রুব short*>(src);
    short* dst16 = reinterpret_cast<short*>(dst);

    ধ্রুব size_t sstep_e = sstep / পরিমাণ(short);
    ধ্রুব size_t dstep_e = dstep / পরিমাণ(short);

    ধ্রুব পূর্ণাঙ্ক m = sz.width, n = sz.height;
    পূর্ণাঙ্ক i = 0;

    প্রতি (; i <= m - 8; i += 8)
    {
        পূর্ণাঙ্ক j = 0;
        প্রতি (; j <= n - 8; j += 8)
        {
            v_int16x8 C0_0, C1_0, C2_0;
            v_int16x8 C0_1, C1_1, C2_1;
            v_int16x8 C0_2, C1_2, C2_2;
            v_int16x8 C0_3, C1_3, C2_3;
            v_int16x8 C0_4, C1_4, C2_4;
            v_int16x8 C0_5, C1_5, C2_5;
            v_int16x8 C0_6, C1_6, C2_6;
            v_int16x8 C0_7, C1_7, C2_7;

            v_load_deinterleave(src16 + sstep_e*(j+0) + i*3, C0_0, C1_0, C2_0);
            v_load_deinterleave(src16 + sstep_e*(j+1) + i*3, C0_1, C1_1, C2_1);
            v_load_deinterleave(src16 + sstep_e*(j+2) + i*3, C0_2, C1_2, C2_2);
            v_load_deinterleave(src16 + sstep_e*(j+3) + i*3, C0_3, C1_3, C2_3);
            v_load_deinterleave(src16 + sstep_e*(j+4) + i*3, C0_4, C1_4, C2_4);
            v_load_deinterleave(src16 + sstep_e*(j+5) + i*3, C0_5, C1_5, C2_5);
            v_load_deinterleave(src16 + sstep_e*(j+6) + i*3, C0_6, C1_6, C2_6);
            v_load_deinterleave(src16 + sstep_e*(j+7) + i*3, C0_7, C1_7, C2_7);

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
        প্রতি (; j < n; j++)
            প্রতি (পূর্ণাঙ্ক k = 0; k < 8; k++)
            {
                dst16[dstep_e*(i+k) + j*3 + 0] = src16[sstep_e*j + (i+k)*3 + 0];
                dst16[dstep_e*(i+k) + j*3 + 1] = src16[sstep_e*j + (i+k)*3 + 1];
                dst16[dstep_e*(i+k) + j*3 + 2] = src16[sstep_e*j + (i+k)*3 + 2];
            }
    }
    প্রতি (; i < m; i++)
        প্রতি (পূর্ণাঙ্ক j = 0; j < n; j++)
        {
            dst16[dstep_e*i + j*3 + 0] = src16[sstep_e*j + i*3 + 0];
            dst16[dstep_e*i + j*3 + 1] = src16[sstep_e*j + i*3 + 1];
            dst16[dstep_e*i + j*3 + 2] = src16[sstep_e*j + i*3 + 2];
        }
}
#endif

template<typename T> স্থির শূন্য
transpose_( ধ্রুব uchar* src, size_t sstep, uchar* dst, size_t dstep, Size sz )
{
#যদি CV_SIMD128
    বিকল্প (পরিমাণ(T))
    {
        পক্ষ 1: transpose_8bit_simd(src, sstep, dst, dstep, sz);  প্রত্যাগম;
        পক্ষ 2: transpose_16bit_simd(src, sstep, dst, dstep, sz); প্রত্যাগম;
        পক্ষ 4: transpose_32bit_simd(src, sstep, dst, dstep, sz); প্রত্যাগম;
        পক্ষ 6: transpose_48bit_simd(src, sstep, dst, dstep, sz); প্রত্যাগম;
        স্বভাব: ভঙ্গ;
    }
#endif

    পূর্ণাঙ্ক i = 0, j, m = sz.width, n = sz.height;

    #যদি CV_ENABLE_UNROLLED
    প্রতি(; i <= m - 4; i += 4 )
    {
        T* d0 = (T*)(dst + dstep*i);
        T* d1 = (T*)(dst + dstep*(i+1));
        T* d2 = (T*)(dst + dstep*(i+2));
        T* d3 = (T*)(dst + dstep*(i+3));

        প্রতি( j = 0; j <= n - 4; j += 4 )
        {
            ধ্রুব T* s0 = (ধ্রুব T*)(src + i*পরিমাণ(T) + sstep*j);
            ধ্রুব T* s1 = (ধ্রুব T*)(src + i*পরিমাণ(T) + sstep*(j+1));
            ধ্রুব T* s2 = (ধ্রুব T*)(src + i*পরিমাণ(T) + sstep*(j+2));
            ধ্রুব T* s3 = (ধ্রুব T*)(src + i*পরিমাণ(T) + sstep*(j+3));

            d0[j] = s0[0]; d0[j+1] = s1[0]; d0[j+2] = s2[0]; d0[j+3] = s3[0];
            d1[j] = s0[1]; d1[j+1] = s1[1]; d1[j+2] = s2[1]; d1[j+3] = s3[1];
            d2[j] = s0[2]; d2[j+1] = s1[2]; d2[j+2] = s2[2]; d2[j+3] = s3[2];
            d3[j] = s0[3]; d3[j+1] = s1[3]; d3[j+2] = s2[3]; d3[j+3] = s3[3];
        }

        প্রতি( ; j < n; j++ )
        {
            ধ্রুব T* s0 = (ধ্রুব T*)(src + i*পরিমাণ(T) + j*sstep);
            d0[j] = s0[0]; d1[j] = s0[1]; d2[j] = s0[2]; d3[j] = s0[3];
        }
    }
    #endif
    প্রতি( ; i < m; i++ )
    {
        T* d0 = (T*)(dst + dstep*i);
        j = 0;
        #যদি CV_ENABLE_UNROLLED
        প্রতি(; j <= n - 4; j += 4 )
        {
            ধ্রুব T* s0 = (ধ্রুব T*)(src + i*পরিমাণ(T) + sstep*j);
            ধ্রুব T* s1 = (ধ্রুব T*)(src + i*পরিমাণ(T) + sstep*(j+1));
            ধ্রুব T* s2 = (ধ্রুব T*)(src + i*পরিমাণ(T) + sstep*(j+2));
            ধ্রুব T* s3 = (ধ্রুব T*)(src + i*পরিমাণ(T) + sstep*(j+3));

            d0[j] = s0[0]; d0[j+1] = s1[0]; d0[j+2] = s2[0]; d0[j+3] = s3[0];
        }
        #endif
        প্রতি( ; j < n; j++ )
        {
            ধ্রুব T* s0 = (ধ্রুব T*)(src + i*পরিমাণ(T) + j*sstep);
            d0[j] = s0[0];
        }
    }
}

template<typename T> স্থির শূন্য
transposeI_( uchar* data, size_t step, পূর্ণাঙ্ক n )
{
    প্রতি( পূর্ণাঙ্ক i = 0; i < n; i++ )
    {
        T* row = (T*)(data + step*i);
        uchar* data1 = data + i*পরিমাণ(T);
        প্রতি( পূর্ণাঙ্ক j = i+1; j < n; j++ )
            std::swap( row[j], *(T*)(data1 + step*j) );
    }
}

প্রকারনির্দেশ শূন্য (*TransposeFunc)( ধ্রুব uchar* src, size_t sstep, uchar* dst, size_t dstep, Size sz );
প্রকারনির্দেশ শূন্য (*TransposeInplaceFunc)( uchar* data, size_t step, পূর্ণাঙ্ক n );

#define DEF_TRANSPOSE_FUNC(suffix, type) \
স্থির শূন্য transpose_##suffix( ধ্রুব uchar* src, size_t sstep, uchar* dst, size_t dstep, Size sz ) \
{ transpose_<type>(src, sstep, dst, dstep, sz); } \
\
স্থির শূন্য transposeI_##suffix( uchar* data, size_t step, পূর্ণাঙ্ক n ) \
{ transposeI_<type>(data, step, n); }

DEF_TRANSPOSE_FUNC(8u, uchar)
DEF_TRANSPOSE_FUNC(16u, ushort)
DEF_TRANSPOSE_FUNC(8uC3, Vec3b)
DEF_TRANSPOSE_FUNC(32s, পূর্ণাঙ্ক)
DEF_TRANSPOSE_FUNC(16uC3, Vec3s)
DEF_TRANSPOSE_FUNC(32sC2, Vec2i)
DEF_TRANSPOSE_FUNC(32sC3, Vec3i)
DEF_TRANSPOSE_FUNC(32sC4, Vec4i)
DEF_TRANSPOSE_FUNC(32sC6, Vec6i)
DEF_TRANSPOSE_FUNC(32sC8, Vec8i)

স্থির TransposeFunc transposeTab[] =
{
    0, transpose_8u, transpose_16u, transpose_8uC3, transpose_32s, 0, transpose_16uC3, 0,
    transpose_32sC2, 0, 0, 0, transpose_32sC3, 0, 0, 0, transpose_32sC4,
    0, 0, 0, 0, 0, 0, 0, transpose_32sC6, 0, 0, 0, 0, 0, 0, 0, transpose_32sC8
};

nd::TransposeFunc nd::getTransposeFunc(size_t esz)
{
    প্রত্যাগম esz < পরিমাণ(transposeTab)/পরিমাণ(transposeTab[0]) ? transposeTab[esz] : nullptr;
}

স্থির TransposeInplaceFunc transposeInplaceTab[] =
{
    0, transposeI_8u, transposeI_16u, transposeI_8uC3, transposeI_32s, 0, transposeI_16uC3, 0,
    transposeI_32sC2, 0, 0, 0, transposeI_32sC3, 0, 0, 0, transposeI_32sC4,
    0, 0, 0, 0, 0, 0, 0, transposeI_32sC6, 0, 0, 0, 0, 0, 0, 0, transposeI_32sC8
};

#ifdef HAVE_OPENCL

স্থির তার্কিক ocl_transpose( প্রবেশ_Array _src, OutputArray _dst )
{
    ধ্রুব ocl::Device & dev = ocl::Device::getDefault();
    ধ্রুব পূর্ণাঙ্ক TILE_DIM = 32, BLOCK_ROWS = 8;
    পূর্ণাঙ্ক type = _src.type(), cn = CV_MAT_CN(type), depth = CV_MAT_DEPTH(type),
        rowsPerWI = dev.isIntel() ? 4 : 1;

    UMat src = _src.getUMat();
    _dst.create(src.cols, src.rows, type);
    UMat dst = _dst.getUMat();

    String kernelName("transpose");
    তার্কিক inplace = dst.u == src.u;

    যদি (inplace)
    {
        CV_Assert(dst.cols == dst.rows);
        kernelName += "_inplace";
    }
    অন্যথা
    {
        // check required local memory size
        size_t required_local_memory = (size_t) TILE_DIM*(TILE_DIM+1)*CV_ELEM_SIZE(type);
        যদি (required_local_memory > ocl::Device::getDefault().localMemSize())
            প্রত্যাগম অসত্য;
    }

    String deviceMacro;
    যদি (dev.isIntel())
        deviceMacro = " -D INTEL_GPU";
    অন্যথা
        deviceMacro = "";

    ocl::Kernel k(kernelName.c_str(), ocl::core::transpose_oclsrc,
                  format("-D T=%s -D T1=%s -D cn=%d -D TILE_DIM=%d -D BLOCK_ROWS=%d -D rowsPerWI=%d%s%s",
                         ocl::memopTypeToStr(type), ocl::memopTypeToStr(depth),
                         cn, TILE_DIM, BLOCK_ROWS, rowsPerWI, inplace ? " -D INPLACE" : "", deviceMacro.c_str()));
    যদি (k.empty())
        প্রত্যাগম অসত্য;

    যদি (inplace)
        k.args(ocl::KernelArg::ReadWriteNoSize(dst), dst.rows);
    অন্যথা
        k.args(ocl::KernelArg::ReadOnly(src),
               ocl::KernelArg::WriteOnlyNoSize(dst));

    size_t localsize[2]  = { TILE_DIM, BLOCK_ROWS };
    size_t globalsize[2] = { (size_t)src.cols, inplace ? ((size_t)src.rows + rowsPerWI - 1) / rowsPerWI : (divUp((size_t)src.rows, TILE_DIM) * BLOCK_ROWS) };

    যদি (inplace && dev.isIntel())
    {
        localsize[0] = 16;
        localsize[1] = dev.maxWorkGroupSize() / localsize[0];
    }

    প্রত্যাগম k.run(2, globalsize, localsize, অসত্য);
}

#endif

শূন্য transpose( প্রবেশ_Array _src, OutputArray _dst )
{
    CV_INSTRUMENT_REGION();

    পূর্ণাঙ্ক type = _src.type(), esz = CV_ELEM_SIZE(type);
    CV_Assert( _src.dims() <= 2 && esz <= 32 );

    CV_OCL_RUN(_dst.isUMat(),
               ocl_transpose(_src, _dst))

    Mat src = _src.getMat();
    যদি( src.empty() )
    {
        _dst.release();
        প্রত্যাগম;
    }

    _dst.create(src.cols, src.rows, src.type());
    Mat dst = _dst.getMat();

    // handle the case of single-column/single-row matrices, stored in STL vectors.
    যদি( src.rows != dst.cols || src.cols != dst.rows )
    {
        CV_Assert( src.size() == dst.size() && (src.cols == 1 || src.rows == 1) );
        src.copyTo(dst);
        প্রত্যাগম;
    }

    CALL_HAL(transpose2d, cv_hal_transpose2d, src.data, src.step, dst.data, dst.step, src.cols, src.rows, esz);

    যদি( dst.data == src.data )
    {
        TransposeInplaceFunc func = transposeInplaceTab[esz];
        CV_Assert( func != 0 );
        CV_Assert( dst.cols == dst.rows );
        func( dst.ptr(), dst.step, dst.rows );
    }
    অন্যথা
    {
        TransposeFunc func = transposeTab[esz];
        CV_Assert( func != 0 );
        func( src.ptr(), src.step, dst.ptr(), dst.step, src.size() );
    }
}


শূন্য transpose(প্রবেশ_Array src_, ধ্রুব std::vector<পূর্ণাঙ্ক>& order, OutputArray dst_)
{
    CV_INSTRUMENT_REGION();

    Mat inp = src_.getMat();
    ধ্রুব পূর্ণাঙ্ক ndims = inp.dims;
    CV_Assert(!(ndims == 0 && inp.empty()));
    CV_CheckEQ(order.size(), static_cast<size_t>(ndims), "Number of dimensions shouldn't change");

    তার্কিক used[CV_MAX_DIM] = {অসত্য};
    পূর্ণাঙ্ক newShape[CV_MAX_DIM];
    প্রতি (পূর্ণাঙ্ক i = 0; i < ndims; i++)
    {
        পূর্ণাঙ্ক j = order[i];
        CV_Check(j, 0 <= j && j < ndims && !used[j], "New order should be a valid permutation of the old one");
        used[j] = সত্য;
        newShape[i] = inp.size[j];
    }

    // plain 2D transpose keeps using cv::transpose (HAL, in-place square case);
    // not for an empty input, which cv::transpose would answer by releasing dst
    যদি (ndims == 2 && order[0] == 1 && !inp.empty() && nd::getTransposeFunc(inp.elemSize()))
    {
        transpose(inp, dst_);
        প্রত্যাগম;
    }

    dst_.create(ndims, newShape, inp.type());
    Mat out = dst_.getMat();
    যদি (out.total() == 0)
        প্রত্যাগম;
    nd::copy(nd::permute(nd::viewOf(inp), order.data()), nd::viewOf(out));
}

শূন্য transposeND(প্রবেশ_Array src, ধ্রুব std::vector<পূর্ণাঙ্ক>& order, OutputArray dst)
{
    transpose(src, order, dst);
}


// Generic scalar fallback
CV_ALWAYS_INLINE শূন্য flipHoriz_generic( ধ্রুব uchar* src, size_t sstep, uchar* dst, size_t dstep, Size size, size_t esz )
{
    পূর্ণাঙ্ক i, j, limit = (পূর্ণাঙ্ক)(((size.width + 1)/2)*esz);
    পূর্ণাঙ্ক height = size.height;
    AutoBuffer<পূর্ণাঙ্ক> _tab(size.width*esz);
    পূর্ণাঙ্ক* tab = _tab.data();

    প্রতি( i = 0; i < size.width; i++ )
        প্রতি( size_t k = 0; k < esz; k++ )
            tab[i*esz + k] = (পূর্ণাঙ্ক)((size.width - i - 1)*esz + k);

    প্রতি( ; height--; src += sstep, dst += dstep )
    {
        প্রতি( i = 0; i < limit; i++ )
        {
            j = tab[i];
            uchar t0 = src[i], t1 = src[j];
            dst[i] = t1; dst[j] = t0;
        }
    }
}

#যদি CV_SIMD || CV_SIMD_SCALABLE
template<typename V> CV_ALWAYS_INLINE শূন্য flipHoriz_single( ধ্রুব uchar* src, size_t sstep, uchar* dst, size_t dstep, Size size )
{
    প্রকারনির্দেশ typename VTraits<V>::lane_type T;
    ধ্রুব পূর্ণাঙ্ক vlanes = VTraits<v_uint8>::vlanes();
    পূর্ণাঙ্ক end = (পূর্ণাঙ্ক)(size.width * পরিমাণ(T));
    পূর্ণাঙ্ক width = (end + 1) / 2;
    পূর্ণাঙ্ক width_simd = width & -vlanes;
    পূর্ণাঙ্ক height = size.height;

    CV_DbgAssert(!CV_STRONG_ALIGNMENT || isAligned<পরিমাণ(T)>(src, dst));

    প্রতি( ; height--; src += sstep, dst += dstep )
    {
        পূর্ণাঙ্ক i = 0, j = end;
        প্রতি( ; i < width_simd; i += vlanes, j -= vlanes )
        {
            V t0 = vx_load((ধ্রুব T*)(src + i));
            V t1 = vx_load((ধ্রুব T*)(src + j - vlanes));
            t0 = v_reverse(t0);
            t1 = v_reverse(t1);
            v_store((T*)(dst + j - vlanes), t0);
            v_store((T*)(dst + i), t1);
        }

        // Scalar tail loop
        যদি (isAligned<পরিমাণ(T)>(src, dst))
        {
            প্রতি ( ; i < width; i += পরিমাণ(T), j -= পরিমাণ(T) )
            {
                T t0 = *((ধ্রুব T*)(src + i));
                T t1 = *((ধ্রুব T*)(src + j - পরিমাণ(T)));
                *((T*)(dst + j - পরিমাণ(T))) = t0;
                *((T*)(dst + i)) = t1;
            }
        }
        অন্যথা
        {
            প্রতি ( ; i < width; i += পরিমাণ(T), j -= পরিমাণ(T) )
            {
                প্রতি (পূর্ণাঙ্ক k = 0; k < (পূর্ণাঙ্ক)পরিমাণ(T); k++)
                {
                    uchar t0 = src[i + k];
                    uchar t1 = src[j + k - পরিমাণ(T)];
                    dst[j + k - পরিমাণ(T)] = t0;
                    dst[i + k] = t1;
                }
            }
        }
    }
}

// SIMD for C3
template<typename V>
CV_ALWAYS_INLINE শূন্য flipHoriz_c3( ধ্রুব uchar* src, size_t sstep, uchar* dst, size_t dstep, Size size )
{
    প্রকারনির্দেশ typename VTraits<V>::lane_type T;
    ধ্রুব পূর্ণাঙ্ক vlanes = VTraits<V>::vlanes();
    ধ্রুব পূর্ণাঙ্ক stride = 3 * পরিমাণ(T);
    পূর্ণাঙ্ক width = size.width;
    পূর্ণাঙ্ক centre = (width + 1) / 2;
    পূর্ণাঙ্ক width_simd = (centre / vlanes) * vlanes;
    পূর্ণাঙ্ক height = size.height;

    প্রতি( ; height--; src += sstep, dst += dstep )
    {
        পূর্ণাঙ্ক i = 0;
        প্রতি( ; i < width_simd; i += vlanes )
        {
            V r0, g0, b0;
            v_load_deinterleave((ধ্রুব T*)(src + i * stride), r0, g0, b0);
            V r1, g1, b1;
            v_load_deinterleave((ধ্রুব T*)(src + (width - i - vlanes) * stride), r1, g1, b1);
            r0 = v_reverse(r0);
            g0 = v_reverse(g0);
            b0 = v_reverse(b0);
            r1 = v_reverse(r1);
            g1 = v_reverse(g1);
            b1 = v_reverse(b1);
            v_store_interleave((T*)(dst + (width - i - vlanes) * stride), r0, g0, b0);
            v_store_interleave((T*)(dst + i * stride), r1, g1, b1);
        }
        // Scalar tail loop for remaining pixels
        প্রতি( ; i < centre; i++ )
        {
            পূর্ণাঙ্ক j = width - i - 1;
            T c0 = ((ধ্রুব T*)(src + i * stride))[0];
            T c1 = ((ধ্রুব T*)(src + i * stride))[1];
            T c2 = ((ধ্রুব T*)(src + i * stride))[2];
            T c3 = ((ধ্রুব T*)(src + j * stride))[0];
            T c4 = ((ধ্রুব T*)(src + j * stride))[1];
            T c5 = ((ধ্রুব T*)(src + j * stride))[2];
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
template<size_t ESZ>
CV_ALWAYS_INLINE শূন্য flipHoriz_vlanes_match( ধ্রুব uchar* src, size_t sstep, uchar* dst, size_t dstep, Size size)
{
    ধ্রুব পূর্ণাঙ্ক vlanes = VTraits<v_uint8>::vlanes();
    পূর্ণাঙ্ক end = (পূর্ণাঙ্ক)(size.width * ESZ);
    পূর্ণাঙ্ক width = end / 2;
    পূর্ণাঙ্ক height = size.height;
    পূর্ণাঙ্ক eSize = (পূর্ণাঙ্ক)ESZ;
    প্রতি( ; height--; src += sstep, dst += dstep )
    {
        প্রতি( পূর্ণাঙ্ক i = 0, j = end - eSize; i < width; i += eSize, j -= eSize )
        {
            প্রতি( পূর্ণাঙ্ক k = 0; k < eSize; k += vlanes )
            {
                v_uint8 t0 = vx_load(src + i + k);
                v_uint8 t1 = vx_load(src + j + k);
                v_store(dst + j + k, t0);
                v_store(dst + i + k, t1);
            }
        }
    }
}

#যদি CV_SIMD128
// SIMD flip when ESZ=16 (128-bit)
template<size_t ESZ>
CV_ALWAYS_INLINE শূন্য flipHoriz_vlanes_match_128( ধ্রুব uchar* src, size_t sstep, uchar* dst, size_t dstep, Size size)
{
    ধ্রুব পূর্ণাঙ্ক vlanes16 = VTraits<v_uint8x16>::vlanes();
    পূর্ণাঙ্ক end = (পূর্ণাঙ্ক)(size.width * ESZ);
    পূর্ণাঙ্ক width = end / 2;
    পূর্ণাঙ্ক height = size.height;
    প্রতি( ; height--; src += sstep, dst += dstep )
    {
        প্রতি( পূর্ণাঙ্ক i = 0, j = end - vlanes16; i < width; i += vlanes16, j -= vlanes16 )
        {
            v_uint8x16 t0 = v_load(src + i);
            v_uint8x16 t1 = v_load(src + j);
            v_store(dst + j, t0);
            v_store(dst + i, t1);
        }
    }
}
#endif // CV_SIMD128

// SIMD flip for ESZ=16,32
template<size_t ESZ>
CV_ALWAYS_INLINE শূন্য flipHoriz_vlanes_dispatch( ধ্রুব uchar* src, size_t sstep, uchar* dst, size_t dstep, Size size )
{
    ধ্রুব পূর্ণাঙ্ক vlanes = VTraits<v_uint8>::vlanes();
#যদি CV_SIMD128
    ধ্রুব পূর্ণাঙ্ক vlanes16 = VTraits<v_uint8x16>::vlanes();
#endif
    যদি ( (ESZ == (size_t)vlanes) || (ESZ == 2 * (size_t)vlanes))
    {
        flipHoriz_vlanes_match<ESZ>(src, sstep, dst, dstep, size);
        প্রত্যাগম;
    }
#যদি CV_SIMD128
    অন্যথা যদি (ESZ == vlanes16)
    {
        flipHoriz_vlanes_match_128<ESZ>(src, sstep, dst, dstep, size);
        প্রত্যাগম;
    }
#endif
    flipHoriz_generic(src, sstep, dst, dstep, size, ESZ);
}

#যদি CV_SIMD128
// SIMD flip for ESZ=24 (CV_64FC3)
CV_ALWAYS_INLINE শূন্য flipHoriz_24( ধ্রুব uchar* src, size_t sstep, uchar* dst, size_t dstep, Size size )
{
    CV_DbgAssert(!CV_STRONG_ALIGNMENT || isAligned<8>(((size_t)src)|((size_t)dst)|sstep|dstep));

    ধ্রুব পূর্ণাঙ্ক lanes16 = 16;
    পূর্ণাঙ্ক end = (পূর্ণাঙ্ক)(size.width * 24);
    পূর্ণাঙ্ক width = (end + 1) / 2;
    পূর্ণাঙ্ক height = size.height;
    প্রতি( ; height--; src += sstep, dst += dstep )
    {
        প্রতি ( পূর্ণাঙ্ক i = 0, j = end; i < width; i += lanes16 + 8, j -= lanes16 + 8 )
        {
            v_uint8x16 t0 = v_load(src + i);
            uint64_t t2 = *reinterpret_cast<ধ্রুব uint64_t*>(src + i + lanes16);
            v_uint8x16 t1 = v_load(src + j - lanes16 - 8);
            uint64_t t3 = *reinterpret_cast<ধ্রুব uint64_t*>(src + j - 8);
            v_store(dst + j - lanes16 - 8, t0);
            *reinterpret_cast<uint64_t*>(dst + j - 8) = t2;
            v_store(dst + i, t1);
            *reinterpret_cast<uint64_t*>(dst + i + lanes16) = t3;
        }
    }
}
#endif // CV_SIMD128
#endif // CV_SIMD || CV_SIMD_SCALABLE

স্থির শূন্য flipHoriz( ধ্রুব uchar* src, size_t sstep, uchar* dst, size_t dstep, Size size, size_t esz )
{
#যদি CV_SIMD || CV_SIMD_SCALABLE
#যদি CV_STRONG_ALIGNMENT
    ধ্রুব size_t alignMark = ((size_t)src)|((size_t)dst)|sstep|dstep;
#অন্যথা
    ধ্রুব size_t alignMark = 0; // unaligned access is allowed: all the checks below fold to true
#endif

    // SIMD-optimized dispatch
    বিকল্প(esz)
    {
        পক্ষ 1:                                                                               // CV_8UC1: 8-bit, 1 channel
            flipHoriz_single<v_uint8>(src, sstep, dst, dstep, size); প্রত্যাগম;
        পক্ষ 2:                                                                               // CV_8UC2, CV_16UC1: 8-bit 2-channel or 16-bit 1-channel
            যদি (isAligned<2>(alignMark)) { flipHoriz_single<v_uint16>(src, sstep, dst, dstep, size); প্রত্যাগম; }
            ভঙ্গ;
        পক্ষ 3:                                                                               // CV_8UC3: 8-bit, 3 channels
            flipHoriz_c3<v_uint8>(src, sstep, dst, dstep, size); প্রত্যাগম;
        পক্ষ 4:                                                                               // CV_8UC4, CV_16UC2, CV_32SC1, CV_32FC1: 8-bit 4-channel, 16-bit 2-channel, or 32-bit 1-channel
            যদি (isAligned<4>(alignMark)) { flipHoriz_single<v_uint32>(src, sstep, dst, dstep, size); প্রত্যাগম; }
            ভঙ্গ;
        পক্ষ 6:                                                                               // CV_16UC3, CV_16SC3: 16-bit, 3 channels
            যদি (isAligned<2>(alignMark)) { flipHoriz_c3<v_uint16>(src, sstep, dst, dstep, size); প্রত্যাগম; }
            ভঙ্গ;
        পক্ষ 8:                                                                               // CV_16UC4, CV_32SC2, CV_32FC2, CV_64FC1: 16-bit 4-channel, 32-bit 2-channel, or 64-bit 1-channel
            যদি (isAligned<8>(alignMark)) { flipHoriz_single<v_uint64>(src, sstep, dst, dstep, size); প্রত্যাগম; }
            ভঙ্গ;
        পক্ষ 12:                                                                              // CV_32SC3, CV_32FC3: 32-bit, 3 channels
            যদি (isAligned<4>(alignMark)) { flipHoriz_c3<v_uint32>(src, sstep, dst, dstep, size); প্রত্যাগম; }
            ভঙ্গ;
        পক্ষ 16:                                                                              // CV_32SC4, CV_32FC4, CV_64FC2: 32-bit 4-channel or 64-bit 2-channel
            flipHoriz_vlanes_dispatch<16>(src, sstep, dst, dstep, size); প্রত্যাগম;               // byte-wise access only
#যদি CV_SIMD128
        পক্ষ 24:                                                                              // CV_64FC3: 64-bit, 3 channels
            যদি (isAligned<8>(alignMark)) { flipHoriz_24(src, sstep, dst, dstep, size); প্রত্যাগম; }
            ভঙ্গ;
#endif
        পক্ষ 32:                                                                              // CV_64FC4: 64-bit, 4 channels
            flipHoriz_vlanes_dispatch<32>(src, sstep, dst, dstep, size); প্রত্যাগম;               // byte-wise access only
        স্বভাব:
            ভঙ্গ; // Fall through to generic implementation
    }
#endif
    // Fallback: generic scalar
    flipHoriz_generic(src, sstep, dst, dstep, size, esz);
}

স্থির শূন্য
flipVert( ধ্রুব uchar* src0, size_t sstep, uchar* dst0, size_t dstep, Size size, size_t esz )
{
    ধ্রুব uchar* src1 = src0 + (size.height - 1)*sstep;
    uchar* dst1 = dst0 + (size.height - 1)*dstep;
    size.width *= (পূর্ণাঙ্ক)esz;

    প্রতি( পূর্ণাঙ্ক y = 0; y < (size.height + 1)/2; y++, src0 += sstep, src1 -= sstep,
                                                  dst0 += dstep, dst1 -= dstep )
    {
        পূর্ণাঙ্ক i = 0;
#যদি (CV_SIMD || CV_SIMD_SCALABLE)
#যদি CV_STRONG_ALIGNMENT
        যদি (isAligned<পরিমাণ(পূর্ণাঙ্ক)>(src0, src1, dst0, dst1))
#endif
        {
            প্রতি (; i <= size.width - VTraits<v_uint8>::vlanes(); i += VTraits<v_uint8>::vlanes())
            {
                v_int32 t0 = v_reinterpret_as_s32(vx_load(src0 + i));
                v_int32 t1 = v_reinterpret_as_s32(vx_load(src1 + i));
                v_store(dst0 + i, v_reinterpret_as_u8(t1));
                v_store(dst1 + i, v_reinterpret_as_u8(t0));
            }
        }
#যদি CV_STRONG_ALIGNMENT
        অন্যথা
        {
            প্রতি (; i <= size.width - VTraits<v_uint8>::vlanes(); i += VTraits<v_uint8>::vlanes())
            {
                v_uint8 t0 = vx_load(src0 + i);
                v_uint8 t1 = vx_load(src1 + i);
                v_store(dst0 + i, t1);
                v_store(dst1 + i, t0);
            }
        }
#endif
#endif

        যদি (isAligned<পরিমাণ(পূর্ণাঙ্ক)>(src0, src1, dst0, dst1))
        {
            প্রতি( ; i <= size.width - 16; i += 16 )
            {
                পূর্ণাঙ্ক t0 = ((পূর্ণাঙ্ক*)(src0 + i))[0];
                পূর্ণাঙ্ক t1 = ((পূর্ণাঙ্ক*)(src1 + i))[0];

                ((পূর্ণাঙ্ক*)(dst0 + i))[0] = t1;
                ((পূর্ণাঙ্ক*)(dst1 + i))[0] = t0;

                t0 = ((পূর্ণাঙ্ক*)(src0 + i))[1];
                t1 = ((পূর্ণাঙ্ক*)(src1 + i))[1];

                ((পূর্ণাঙ্ক*)(dst0 + i))[1] = t1;
                ((পূর্ণাঙ্ক*)(dst1 + i))[1] = t0;

                t0 = ((পূর্ণাঙ্ক*)(src0 + i))[2];
                t1 = ((পূর্ণাঙ্ক*)(src1 + i))[2];

                ((পূর্ণাঙ্ক*)(dst0 + i))[2] = t1;
                ((পূর্ণাঙ্ক*)(dst1 + i))[2] = t0;

                t0 = ((পূর্ণাঙ্ক*)(src0 + i))[3];
                t1 = ((পূর্ণাঙ্ক*)(src1 + i))[3];

                ((পূর্ণাঙ্ক*)(dst0 + i))[3] = t1;
                ((পূর্ণাঙ্ক*)(dst1 + i))[3] = t0;
            }

            প্রতি( ; i <= size.width - 4; i += 4 )
            {
                পূর্ণাঙ্ক t0 = ((পূর্ণাঙ্ক*)(src0 + i))[0];
                পূর্ণাঙ্ক t1 = ((পূর্ণাঙ্ক*)(src1 + i))[0];

                ((পূর্ণাঙ্ক*)(dst0 + i))[0] = t1;
                ((পূর্ণাঙ্ক*)(dst1 + i))[0] = t0;
            }
        }

        প্রতি( ; i < size.width; i++ )
        {
            uchar t0 = src0[i];
            uchar t1 = src1[i];

            dst0[i] = t1;
            dst1[i] = t0;
        }
    }
}

#ifdef HAVE_OPENCL

গণনা { FLIP_COLS = 1 << 0, FLIP_ROWS = 1 << 1, FLIP_BOTH = FLIP_ROWS | FLIP_COLS };

স্থির তার্কিক ocl_flip(প্রবেশ_Array _src, OutputArray _dst, পূর্ণাঙ্ক flipCode )
{
    CV_Assert(flipCode >= -1 && flipCode <= 1);

    ধ্রুব ocl::Device & dev = ocl::Device::getDefault();
    পূর্ণাঙ্ক type = _src.type(), depth = CV_MAT_DEPTH(type), cn = CV_MAT_CN(type),
            flipType, kercn = std::min(ocl::predictOptimalVectorWidth(_src, _dst), 4);

    তার্কিক doubleSupport = dev.doubleFPConfig() > 0;
    যদি (!doubleSupport && depth == CV_64F)
        kercn = cn;

    যদি (cn > 4)
        প্রত্যাগম অসত্য;

    Size size = _src.size();
    _dst.create(size, type);
    UMat src = _src.getUMat(), dst = _dst.getUMat();
    তার্কিক inplace = (dst.u == src.u);

    String kernelName;
    যদি (flipCode == 0)
        kernelName = "arithm_flip_rows", flipType = FLIP_ROWS;
    অন্যথা যদি (flipCode > 0)
        kernelName = "arithm_flip_cols", flipType = FLIP_COLS;
    অন্যথা
        kernelName = "arithm_flip_rows_cols", flipType = FLIP_BOTH;

    যদি(inplace)
        kernelName += "_inplace";

    পূর্ণাঙ্ক pxPerWIy = (dev.isIntel() && (dev.type() & ocl::Device::TYPE_GPU)) ? 4 : 1;
    kercn = (cn!=3 || flipType == FLIP_ROWS) ? std::max(kercn, cn) : cn;
    ধ্রুব পূর্ণাঙ্ক TILE_SIZE = 32, BLOCK_ROWS = 8;

    ocl::Kernel k(kernelName.c_str(), ocl::core::flip_oclsrc,
                format( "-D T=%s -D T1=%s -D DEPTH=%d -D cn=%d -D PIX_PER_WI_Y=%d -D kercn=%d -D TILE_SIZE=%d -D BLOCK_ROWS=%d%s",
                kercn != cn ? ocl::typeToStr(CV_MAKE_TYPE(depth, kercn)) : ocl::vecopTypeToStr(CV_MAKE_TYPE(depth, kercn)),
                kercn != cn ? ocl::typeToStr(depth) : ocl::vecopTypeToStr(depth), depth, cn, pxPerWIy, kercn, TILE_SIZE, BLOCK_ROWS,
                inplace ? " -D INPLACE" : ""));
    যদি (k.empty())
        প্রত্যাগম অসত্য;

    পূর্ণাঙ্ক cols = size.width * cn / kercn, rows = size.height;
    পূর্ণাঙ্ক work_cols = flipType == FLIP_COLS ? (cols + 1) >> 1 : cols;
    পূর্ণাঙ্ক work_rows = flipType & FLIP_ROWS ? (rows + 1) >> 1 : rows;

    যদি (inplace)
    {
        k.args(ocl::KernelArg::ReadWriteNoSize(dst), rows, cols);

        পূর্ণাঙ্ক gs_cols, gs_rows;
        যদি (flipType == FLIP_COLS)
        {
            gs_cols = work_cols;
            gs_rows = rows;
        }
        অন্যথা যদি (flipType == FLIP_ROWS)
        {
            gs_cols = cols;
            gs_rows = work_rows;
        }
        অন্যথা // FLIP_BOTH
        {
            gs_cols = cols;
            gs_rows = rows;
        }

        size_t globalsize[2] = {
            (size_t)divUp(gs_cols, TILE_SIZE) * TILE_SIZE,
            (size_t)divUp(gs_rows, TILE_SIZE) * BLOCK_ROWS
        };
        size_t localsize[2] = { TILE_SIZE, BLOCK_ROWS };
        প্রত্যাগম k.run(2, globalsize, localsize, অসত্য);
    }
    অন্যথা
    {
        k.args(ocl::KernelArg::ReadOnlyNoSize(src),
           ocl::KernelArg::WriteOnly(dst, cn, kercn), work_rows, work_cols);

        size_t maxWorkGroupSize = dev.maxWorkGroupSize();
        CV_Assert(maxWorkGroupSize % 4 == 0);

        size_t globalsize[2] = { (size_t)work_cols, ((size_t)work_rows + pxPerWIy - 1) / pxPerWIy };
        size_t localsize[2]  = { maxWorkGroupSize / 4, 4 };
        প্রত্যাগম k.run(2, globalsize, (flipType == FLIP_COLS) && !dev.isIntel() ? localsize : রিক্ত, অসত্য);
    }
}

#endif

শূন্য flip( প্রবেশ_Array _src, OutputArray _dst, পূর্ণাঙ্ক flip_mode )
{
    CV_INSTRUMENT_REGION();

    CV_Assert( _src.dims() <= 2 );
    Size size = _src.size();

    যদি (flip_mode < 0)
    {
        যদি (size.width == 1)
            flip_mode = 0;
        যদি (size.height == 1)
            flip_mode = 1;
    }

    যদি ((size.width == 1 && flip_mode > 0) ||
        (size.height == 1 && flip_mode == 0))
    {
        প্রত্যাগম _src.copyTo(_dst);
    }

    CV_OCL_RUN( _dst.isUMat(), ocl_flip(_src, _dst, flip_mode))

    Mat src = _src.getMat();
    পূর্ণাঙ্ক type = src.type();
    _dst.create( size, type );
    Mat dst = _dst.getMat();

    CALL_HAL(flip, cv_hal_flip, type, src.ptr(), src.step, src.cols, src.rows,
             dst.ptr(), dst.step, flip_mode);

    size_t esz = CV_ELEM_SIZE(type);

    যদি( flip_mode <= 0 )
        flipVert( src.ptr(), src.step, dst.ptr(), dst.step, src.size(), esz );
    অন্যথা
        flipHoriz( src.ptr(), src.step, dst.ptr(), dst.step, src.size(), esz );

    যদি( flip_mode < 0 )
        flipHoriz( dst.ptr(), dst.step, dst.ptr(), dst.step, dst.size(), esz );
}

// In-place flip of a continuous array: swap the slices along the axis, no temporary copy.
স্থির শূন্য flipNDInplace(Mat& m, পূর্ণাঙ্ক axis)
{
    size_t nouter = 1;
    প্রতি (পূর্ণাঙ্ক i = 0; i < axis; i++)
        nouter *= (size_t)m.size[i];
    ধ্রুব পূর্ণাঙ্ক n = m.size[axis];
    ধ্রুব size_t step = m.step[axis];
    uchar* data = m.ptr();
    প্রতি (size_t i = 0; i < nouter; i++, data += n*step)
        প্রতি (পূর্ণাঙ্ক j = 0, k = n - 1; j < k; j++, k--)
            std::swap_ranges(data + j*step, data + (j + 1)*step, data + k*step);
}

শূন্য flipND(প্রবেশ_Array _src, OutputArray _dst, পূর্ণাঙ্ক _axis)
{
    CV_INSTRUMENT_REGION();

    Mat src = _src.getMat();

    // verify axis
    পূর্ণাঙ্ক ndim = src.dims;
    CV_CheckLT(_axis, ndim, "flipND: given axis is out of range");
    CV_CheckGE(_axis, -ndim, "flipND: given axis is out of range");
    পূর্ণাঙ্ক axis = (_axis + ndim) % ndim;

    _dst.create(ndim, src.size.p, src.type());
    Mat dst = _dst.getMat();
    যদি (dst.data == src.data && src.isContinuous() && dst.isContinuous())
    {
        flipNDInplace(dst, axis);
        প্রত্যাগম;
    }

    nd::View v = nd::viewOf(src);
    nd::flip(v, axis);
    nd::copy(v, nd::viewOf(dst));
}

শূন্য concat(প্রবেশ_Array_Of_Arrays _src, পূর্ণাঙ্ক axis, OutputArray _dst)
{
    CV_INSTRUMENT_REGION();

    std::vector<Mat> src;
    _src.getMatVector(src);
    CV_Assert(!src.empty());

    ধ্রুব Mat& src0 = src[0];
    ধ্রুব পূর্ণাঙ্ক ndims = src0.dims, type = src0.type(), ninputs = (পূর্ণাঙ্ক)src.size();
    CV_CheckGE(ndims, 1, "concat: 0-dimensional arrays are not supported");
    CV_CheckGE(axis, -ndims, "concat: axis is out of range");
    CV_CheckLT(axis, ndims, "concat: axis is out of range");
    যদি (axis < 0)
        axis += ndims;

    পূর্ণাঙ্ক outShape[CV_MAX_DIM];
    প্রতি (পূর্ণাঙ্ক i = 0; i < ndims; i++)
        outShape[i] = src0.size[i];
    int64 axisSize = 0;
    প্রতি (পূর্ণাঙ্ক k = 0; k < ninputs; k++)
    {
        ধ্রুব Mat& m = src[k];
        CV_CheckEQ(m.type(), type, "concat: all the input arrays must have the same type");
        CV_CheckEQ(m.dims, ndims, "concat: all the input arrays must have the same number of dimensions");
        প্রতি (পূর্ণাঙ্ক i = 0; i < ndims; i++)
            যদি (i != axis)
                CV_CheckEQ(m.size[i], outShape[i], "concat: the input shapes must match except along the axis");
        axisSize += m.size[axis];
    }
    CV_CheckLE((size_t)axisSize, (size_t)INT_MAX, "concat: the output is too big");
    outShape[axis] = (পূর্ণাঙ্ক)axisSize;

    _dst.create(ndims, outShape, type);
    Mat dst = _dst.getMat();

    AutoBuffer<nd::View, 4> sv(ninputs), dv(ninputs);
    ধ্রুব nd::View dst0 = nd::viewOf(dst);
    পূর্ণাঙ্ক ofs = 0;
    প্রতি (পূর্ণাঙ্ক k = 0; k < ninputs; k++)
    {
        ধ্রুব পূর্ণাঙ্ক sz = src[k].size[axis];
        sv[k] = nd::viewOf(src[k]);
        dv[k] = dst0;
        nd::slice(dv[k], axis, ofs, 1, sz);
        ofs += sz;
    }
    nd::copyBatch(sv.data(), dv.data(), ninputs);
}

শূন্য split(প্রবেশ_Array _src, পূর্ণাঙ্ক axis, ধ্রুব std::vector<পূর্ণাঙ্ক>& sizes, OutputArrayOfArrays _dst)
{
    CV_INSTRUMENT_REGION();

    Mat src = _src.getMat();
    ধ্রুব পূর্ণাঙ্ক ndims = src.dims, type = src.type(), noutputs = (পূর্ণাঙ্ক)sizes.size();
    CV_CheckGE(ndims, 1, "split: 0-dimensional arrays are not supported");
    CV_CheckGE(axis, -ndims, "split: axis is out of range");
    CV_CheckLT(axis, ndims, "split: axis is out of range");
    যদি (axis < 0)
        axis += ndims;
    CV_Assert(noutputs > 0);

    int64 axisSize = 0;
    প্রতি (পূর্ণাঙ্ক k = 0; k < noutputs; k++)
    {
        CV_CheckGE(sizes[k], 0, "split: the sizes must be non-negative");
        axisSize += sizes[k];
    }
    CV_CheckEQ((size_t)axisSize, (size_t)src.size[axis], "split: the sizes must sum up to the size of the axis");

    পূর্ণাঙ্ক outShape[CV_MAX_DIM];
    প্রতি (পূর্ণাঙ্ক i = 0; i < ndims; i++)
        outShape[i] = src.size[i];

    _dst.create(noutputs, 1, type, -1, সত্য);
    AutoBuffer<nd::View, 4> sv(noutputs), dv(noutputs);
    std::vector<Mat> dst(noutputs);
    ধ্রুব nd::View src0 = nd::viewOf(src);
    পূর্ণাঙ্ক ofs = 0;
    প্রতি (পূর্ণাঙ্ক k = 0; k < noutputs; k++)
    {
        outShape[axis] = sizes[k];
        _dst.create(ndims, outShape, type, k);
        dst[k] = _dst.getMat(k);
        sv[k] = src0;
        nd::slice(sv[k], axis, ofs, 1, sizes[k]);
        dv[k] = nd::viewOf(dst[k]);
        ofs += sizes[k];
    }
    nd::copyBatch(sv.data(), dv.data(), noutputs);
}

শূন্য repeat(প্রবেশ_Array _src, ধ্রুব std::vector<পূর্ণাঙ্ক>& repeats, OutputArray _dst)
{
    CV_INSTRUMENT_REGION();

    Mat src = _src.getMat();
    ধ্রুব পূর্ণাঙ্ক ndims = src.dims;
    CV_Assert(!(ndims == 0 && src.empty()));
    CV_CheckEQ(repeats.size(), (size_t)ndims, "repeat: the number of repeats must match the number of dimensions");

    পূর্ণাঙ্ক outShape[CV_MAX_DIM];
    প্রতি (পূর্ণাঙ্ক i = 0; i < ndims; i++)
    {
        CV_CheckGE(repeats[i], 0, "repeat: the repeats must be non-negative");
        int64 sz = (int64)src.size[i]*repeats[i];
        CV_CheckLE((size_t)sz, (size_t)INT_MAX, "repeat: the output is too big");
        outShape[i] = (পূর্ণাঙ্ক)sz;
    }

    _dst.create(ndims, outShape, src.type());
    Mat dst = _dst.getMat();
    যদি (dst.total() == 0 || src.empty())
        প্রত্যাগম;

    // Each repeated output axis is split into (repeat index, position), the former
    // with source step 0, so the whole tile is one broadcasting copy.
    nd::View sv, dv;
    sv.data = src.data;
    dv.data = dst.data;
    sv.esz = dv.esz = src.elemSize();
    পূর্ণাঙ্ক n = 0;
    প্রতি (পূর্ণাঙ্ক i = 0; i < ndims; i++)
    {
        ধ্রুব পূর্ণাঙ্ক sz = src.size[i], r = repeats[i];
        যদি (r != 1)
        {
            CV_CheckLT(n, (পূর্ণাঙ্ক)nd::MAX_VIEW_DIMS, "repeat: too many dimensions");
            sv.size[n] = dv.size[n] = r;
            sv.step[n] = 0;
            dv.step[n] = (ptrdiff_t)dst.step[i]*sz;
            n++;
        }
        যদি (sz != 1 || r == 1)
        {
            CV_CheckLT(n, (পূর্ণাঙ্ক)nd::MAX_VIEW_DIMS, "repeat: too many dimensions");
            sv.size[n] = dv.size[n] = sz;
            sv.step[n] = (ptrdiff_t)src.step[i];
            dv.step[n] = (ptrdiff_t)dst.step[i];
            n++;
        }
    }
    sv.dims = dv.dims = n;
    nd::copy(sv, dv);
}

শূন্য slice(প্রবেশ_Array _src, ধ্রুব std::vector<পূর্ণাঙ্ক>& starts, ধ্রুব std::vector<পূর্ণাঙ্ক>& ends,
             ধ্রুব std::vector<পূর্ণাঙ্ক>& steps, OutputArray _dst)
{
    CV_INSTRUMENT_REGION();

    Mat src = _src.getMat();
    ধ্রুব পূর্ণাঙ্ক ndims = src.dims, nslices = (পূর্ণাঙ্ক)starts.size();
    CV_Assert(!(ndims == 0 && src.empty()));
    CV_CheckLE(nslices, ndims, "slice: too many starts");
    CV_CheckEQ(ends.size(), starts.size(), "slice: starts and ends must have the same size");
    CV_Check(steps.size(), steps.empty() || steps.size() == starts.size(),
             "slice: steps must be empty or have the same size as starts");

    nd::View sv = nd::viewOf(src);
    পূর্ণাঙ্ক outShape[CV_MAX_DIM];
    প্রতি (পূর্ণাঙ্ক i = 0; i < ndims; i++)
        outShape[i] = src.size[i];

    প্রতি (পূর্ণাঙ্ক i = 0; i < nslices; i++)
    {
        ধ্রুব পূর্ণাঙ্ক n = src.size[i], start = starts[i], end = ends[i];
        ধ্রুব পূর্ণাঙ্ক step = steps.empty() ? 1 : steps[i];
        CV_CheckNE(step, 0, "slice: step must not be zero");
        পূর্ণাঙ্ক গুনতি_2;
        যদি (step > 0)
        {
            CV_Check(start, 0 <= start && start <= n, "slice: start is out of range");
            CV_Check(end, 0 <= end && end <= n, "slice: end is out of range");
            গুনতি_2 = end > start ? (পূর্ণাঙ্ক)(((int64)end - start + step - 1)/step) : 0;
        }
        অন্যথা
        {
            CV_Check(start, -1 <= start && start < n, "slice: start is out of range");
            CV_Check(end, -1 <= end && end < n, "slice: end is out of range");
            গুনতি_2 = start > end ? (পূর্ণাঙ্ক)(((int64)start - end - step - 1)/(-step)) : 0;
        }
        nd::slice(sv, i, start, step, গুনতি_2);
        outShape[i] = গুনতি_2;
    }

    _dst.create(ndims, outShape, src.type());
    Mat dst = _dst.getMat();
    যদি (dst.total() == 0 || src.empty())
        প্রত্যাগম;
    nd::copy(sv, nd::viewOf(dst));
}

/*
    This function first prepends 1 to each tensor shape to have a common max_ndims dimension, then flatten non-broadcast dimensions.
*/
স্থির তার্কিক _flatten_for_broadcast(পূর্ণাঙ্ক narrays, পূর্ণাঙ্ক max_ndims, ধ্রুব পূর্ণাঙ্ক* ndims, ধ্রুব পূর্ণাঙ্ক** orig_shape,
                                   পূর্ণাঙ্ক** flatten_shape, size_t** flatten_step) {
    পূর্ণাঙ্ক i, j, k;

    // step 1.
    // * make all inputs and the output max_ndims-dimensional.
    // * compute proper step's
    প্রতি (i = max_ndims - 1; i >= 0; i-- ) {
        প্রতি (k = 0; k < narrays; k++) {
            j = ndims[k] - (max_ndims - i);
            পূর্ণাঙ্ক sz_i = j >= 0 ? orig_shape[k][j] : 1;
            size_t st_i = i == max_ndims - 1 ? 1 : flatten_step[k][i+1] * flatten_shape[k][i+1];
            flatten_shape[k][i] = sz_i;
            flatten_step[k][i] = st_i;
            যদি (flatten_shape[k][i] == 0)
                প্রত্যাগম অসত্য;
        }
    }

    // step 2. Let's do the flattening first,
    // since we'd need proper values of steps to check continuity.
    // this loop is probably the most tricky part
    // in the whole implementation of broadcasting.
    j = max_ndims-1;
    প্রতি (i = j - 1; i >= 0; i--) {
        তার্কিক all_contiguous = সত্য, all_scalars = সত্য, all_consistent = সত্য;
        প্রতি(k = 0; k < narrays; k++) {
            size_t st = flatten_step[k][j] * flatten_shape[k][j];
            তার্কিক prev_scalar = flatten_shape[k][j] == 1;
            তার্কিক scalar = flatten_shape[k][i] == 1;
            all_contiguous = all_contiguous && (st == flatten_step[k][i]);
            all_scalars = all_scalars && scalar;
            all_consistent = all_consistent && (scalar == prev_scalar);
        }
        যদি (all_contiguous && (all_consistent || all_scalars)) {
            প্রতি(k = 0; k < narrays; k++)
                flatten_shape[k][j] *= flatten_shape[k][i];
        } অন্যথা {
            j--;
            যদি (i < j) {
                প্রতি(k = 0; k < narrays; k++) {
                    flatten_shape[k][j] = flatten_shape[k][i];
                    flatten_step[k][j] = flatten_step[k][i];
                }
            }
        }
    }

    // step 3. Set some step's to 0's.
    প্রতি (i = max_ndims-1; i >= j; i--) {
        প্রতি (k = 0; k < narrays; k++)
            flatten_step[k][i] = flatten_shape[k][i] == 1 ? 0 : flatten_step[k][i];
    }
    প্রতি (; i >= 0; i--) {
        প্রতি (k = 0; k < narrays; k++) {
            flatten_step[k][i] = 0;
            flatten_shape[k][i] = 1;
        }
    }
    প্রত্যাগম সত্য;
}

শূন্য broadcast(প্রবেশ_Array _src, প্রবেশ_Array _shape, OutputArray _dst) {
    CV_INSTRUMENT_REGION();

    Mat src = _src.getMat();
    CV_CheckTrue(src.isContinuous(), "broadcast: input array must be contiguous");
    CV_CheckChannelsEQ(src.channels(), 1, "broadcast: input array must be single channel");

    Mat shape = _shape.getMat();
    CV_CheckTypeEQ(shape.type(), CV_32S, "broadcast: target shape must be of type int32");
    ধ্রুব auto dims_shape = static_cast<পূর্ণাঙ্ক>(shape.total());
    ধ্রুব auto *ptr_shape = shape.ptr<পূর্ণাঙ্ক>();

    // check valid shape, 1D/0D Mat would fail in the following checks
    ধ্রুব auto dims_src = src.dims;
    CV_CheckLE(dims_src, dims_shape,
               "broadcast: dimension of input array must be less than or equal to dimension of target shape");
    std::vector<পূর্ণাঙ্ক> shape_src{src.size.p, src.size.p + dims_src};
    যদি (shape_src.size() < static_cast<size_t>(dims_shape)) {
        shape_src.insert(shape_src.begin(), dims_shape - shape_src.size(), 1);
    }
    প্রতি (পূর্ণাঙ্ক i = 0; i < static_cast<পূর্ণাঙ্ক>(shape_src.size()); ++i) {
        ধ্রুব auto *shape_target = ptr_shape;
        যদি (shape_src[i] != 1) {
            CV_CheckEQ(shape_src[i], shape_target[i], "target shape must be equal to input shape or 1");
        }
    }

    // impl
    _dst.create(dims_shape, shape.ptr<পূর্ণাঙ্ক>(), src.type());
    Mat dst = _dst.getMat();
    যদি (dst.total() == 0)
        প্রত্যাগম;
    cv::AutoBuffer<পূর্ণাঙ্ক> is_same_shape(dims_shape, 0);
    প্রতি (পূর্ণাঙ্ক i = 0; i < static_cast<পূর্ণাঙ্ক>(shape_src.size()); ++i) {
        যদি (shape_src[i] == ptr_shape[i]) {
            is_same_shape[i] = 1;
        }
    }
    // copy if same shape
    যদি (std::accumulate(is_same_shape.begin(), is_same_shape.end(), 1, std::multiplies<পূর্ণাঙ্ক>()) != 0) {
        ধ্রুব auto *p_src = src.ptr<ধ্রুব বর্ণ>();
        auto *p_dst = dst.ptr<বর্ণ>();
        std::memcpy(p_dst, p_src, dst.total() * dst.elemSize());
        প্রত্যাগম;
    }
    // other cases
    পূর্ণাঙ্ক max_ndims = std::max(dims_src, dims_shape);
    যদি (max_ndims < 2 && src.total() == 1) {
        ধ্রুব বর্ণ* p_src = src.ptr<ধ্রুব বর্ণ>();
        বর্ণ* p_dst = dst.ptr<বর্ণ>();
        size_t esz = src.elemSize();
        প্রতি (size_t j = 0; j < dst.total(); j++)
            std::memcpy(p_dst + j * esz, p_src, esz);
        প্রত্যাগম;
    }
    ধ্রুব পূর্ণাঙ্ক all_ndims[2] = {src.dims, dst.dims};
    ধ্রুব পূর্ণাঙ্ক* orig_shapes[2] = {src.size.p, dst.size.p};
    cv::AutoBuffer<size_t> buff(max_ndims * 4);
    পূর্ণাঙ্ক* flatten_shapes[2] = {(পূর্ণাঙ্ক*)buff.data(), (পূর্ণাঙ্ক*)(buff.data() + max_ndims)};
    size_t* flatten_steps[2] = {(size_t*)(buff.data() + 2 * max_ndims), (size_t*)(buff.data() + 3 * max_ndims)};
    যদি (_flatten_for_broadcast(2, max_ndims, all_ndims, orig_shapes, flatten_shapes, flatten_steps)) {
        size_t src_dp = flatten_steps[0][max_ndims - 1];
        size_t dst_dp = flatten_steps[1][max_ndims - 1];
        CV_Assert(dst_dp == 1 || dst_dp == 0);
        CV_Assert(max_ndims >= 2); // >= 3?
        size_t rowstep_src = flatten_steps[0][max_ndims - 2];
        size_t rowstep_dst = flatten_steps[1][max_ndims - 2];
        ধ্রুব বর্ণ* ptr_src = src.ptr<ধ্রুব বর্ণ>();
        বর্ণ* ptr_dst = dst.ptr<বর্ণ>();
        size_t esz = src.elemSize();
        পূর্ণাঙ্ক nrows = flatten_shapes[1][max_ndims - 2];
        পূর্ণাঙ্ক ncols = flatten_shapes[1][max_ndims - 1];
        পূর্ণাঙ্ক nplanes = 1;
        CV_Check(esz, esz == 1 || esz == 2 || esz == 4 || esz == 8, "broadcast: not supported data type");

        প্রতি (পূর্ণাঙ্ক k = 0; k < max_ndims - 2; k++) {
            nplanes *= flatten_shapes[1][k];
        }
        প্রতি (পূর্ণাঙ্ক plane_idx = 0; plane_idx < nplanes; plane_idx++) {
            size_t offset_src = 0, offset_dst = 0;
            size_t idx = (size_t)plane_idx;
            প্রতি (পূর্ণাঙ্ক k = max_ndims - 3; k >= 0; k--) {
                size_t prev_idx = idx / flatten_shapes[1][k];
                size_t i_k = (পূর্ণাঙ্ক)(idx - prev_idx * flatten_shapes[1][k]);
                offset_src += i_k * flatten_steps[0][k];
                offset_dst += i_k * flatten_steps[1][k];
                idx = prev_idx;
            }

            #define OPENCV_CORE_BROADCAST_LOOP(_Tp) \
                প্রতি (পূর্ণাঙ্ক i = 0; i < nrows; i++) {   \
                    ধ্রুব _Tp *ptr_src_ = (ধ্রুব _Tp*)ptr_src + offset_src + rowstep_src * i; \
                    _Tp *ptr_dst_ = (_Tp*)ptr_dst + offset_dst + rowstep_dst * i; \
                    যদি (src_dp == 1) { \
                        প্রতি (পূর্ণাঙ্ক j = 0; j < ncols; j++) { \
                            ptr_dst_[j] = ptr_src_[j]; \
                        } \
                    } অন্যথা { \
                        _Tp x = *ptr_src_; \
                        প্রতি (পূর্ণাঙ্ক j = 0; j < ncols; j++) { \
                            ptr_dst_[j] = x; \
                        } \
                    } \
                }

            যদি (esz == 1) {
                OPENCV_CORE_BROADCAST_LOOP(int8_t);
            } অন্যথা যদি (esz == 2) {
                OPENCV_CORE_BROADCAST_LOOP(int16_t);
            } অন্যথা যদি (esz == 4) {
                OPENCV_CORE_BROADCAST_LOOP(int32_t);
            } অন্যথা যদি (esz == 8) {
                OPENCV_CORE_BROADCAST_LOOP(int64_t);
            } অন্যথা {
                CV_Error(cv::Error::StsNotImplemented, "");
            }
            #undef OPENCV_CORE_BROADCAST_LOOP
        }
    } অন্যথা {
        যদি (dst.total() == 0)
            প্রত্যাগম;
        // initial copy (src to dst)
        std::vector<size_t> step_src{src.step.p, src.step.p + dims_src};
        যদি (step_src.size() < static_cast<size_t>(dims_shape)) {
            step_src.insert(step_src.begin(), dims_shape - step_src.size(), step_src[0]);
        }
        প্রতি (size_t i = 0; i < src.total(); ++i) {
            size_t t = i;
            size_t src_offset = 0, dst_offset = 0;
            প্রতি (পূর্ণাঙ্ক j = static_cast<পূর্ণাঙ্ক>(shape_src.size() - 1); j >= 0; --j) {
                size_t idx = t / shape_src[j];
                size_t offset = static_cast<size_t>(t - idx * shape_src[j]);
                src_offset += offset * step_src[j];
                dst_offset += offset * dst.step[j];
                t = idx;
            }
            ধ্রুব auto *p_src = src.ptr<ধ্রুব বর্ণ>();
            auto *p_dst = dst.ptr<বর্ণ>();
            std::memcpy(p_dst + dst_offset, p_src + src_offset, dst.elemSize());
        }
        // broadcast copy (dst inplace)
        AutoBuffer<পূর্ণাঙ্ক> cumulative_shape(dims_shape, 1);
        পূর্ণাঙ্ক total = static_cast<পূর্ণাঙ্ক>(dst.total());
        প্রতি (পূর্ণাঙ্ক i = dims_shape - 1; i >= 0; --i) {
            cumulative_shape[i] = static_cast<পূর্ণাঙ্ক>(total / ptr_shape[i]);
            total = cumulative_shape[i];
        }
        প্রতি (পূর্ণাঙ্ক i = dims_shape - 1; i >= 0; --i) {
            যদি (is_same_shape[i] == 1) {
                অনুবর্ত;
            }
            auto step = dst.step[i];
            auto *p_dst = dst.ptr<বর্ণ>();
            প্রতি (পূর্ণাঙ্ক j = 0; j < cumulative_shape[i]; j++) {
                প্রতি (পূর্ণাঙ্ক k = 0; k < ptr_shape[i] - 1; k++) {
                    std::memcpy(p_dst + step, p_dst, step);
                    p_dst += step;
                }
                p_dst += step;
            }
        }
    }
}

শূন্য broadcast(প্রবেশ_Array _src, ধ্রুব MatShape& _shape, OutputArray _dst)
{
    যদি (_shape.dims < 0) {
        _dst.release();
    } অন্যথা {
        Mat shape(1, _shape.dims, CV_32S, (পূর্ণাঙ্ক*)_shape.p);
        broadcast(_src, shape, _dst);
    }
}

স্থির শূন্য rotateImpl(প্রবেশ_Array _src, OutputArray _dst, পূর্ণাঙ্ক rotateMode)
{
    বিকল্প (rotateMode)
    {
    পক্ষ ROTATE_90_CLOCKWISE:
        transpose(_src, _dst);
        flip(_dst, _dst, 1);
        ভঙ্গ;
    পক্ষ ROTATE_180:
        flip(_src, _dst, -1);
        ভঙ্গ;
    পক্ষ ROTATE_90_COUNTERCLOCKWISE:
        transpose(_src, _dst);
        flip(_dst, _dst, 0);
        ভঙ্গ;
    স্বভাব:
        ভঙ্গ;
    }
}

শূন্য rotate(প্রবেশ_Array _src, OutputArray _dst, পূর্ণাঙ্ক rotateMode)
{
    CV_Assert(_src.dims() <= 2);
    পূর্ণাঙ্ক angle;

    যদি (_dst.isUMat())
    {
        rotateImpl(_src, _dst, rotateMode);
        প্রত্যাগম;
    }

    Mat src = _src.getMat();
    পূর্ণাঙ্ক type = src.type();
    যদি( src.empty() )
    {
        _dst.release();
        প্রত্যাগম;
    }

    বিকল্প (rotateMode)
    {
    পক্ষ ROTATE_90_CLOCKWISE:
        _dst.create(src.cols, src.rows, type);
        angle = 90;
        ভঙ্গ;
    পক্ষ ROTATE_180:
        _dst.create(src.rows, src.cols, type);
        angle = 180;
        ভঙ্গ;
    পক্ষ ROTATE_90_COUNTERCLOCKWISE:
        _dst.create(src.cols, src.rows, type);
        angle = 270;
        ভঙ্গ;
    স্বভাব:
        _dst.create(src.rows, src.cols, type);
        angle = 0;
        ভঙ্গ;
    }

    Mat dst = _dst.getMat();
    CALL_HAL(rotate90, cv_hal_rotate90, type, src.ptr(), src.step, src.cols, src.rows,
             dst.ptr(), dst.step, angle);

    // use src (Mat) since _src (InputArray) is updated by _dst.create() when in-place
    rotateImpl(src, _dst, rotateMode);
}

}  // namespace
