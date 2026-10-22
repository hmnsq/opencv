// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html
// Copyright (C) 2026, Advanced Micro Devices, Inc., all rights reserved.
#అన్తర్భావ "precomp.hpp"
#అన్తర్భావ "opencl_kernels_core.hpp"
#అన్తర్భావ "hal_replacement.hpp"
#అన్తర్భావ "opencv2/core/detail/dispatch_helper.impl.hpp"

#అన్తర్భావ "transpose.simd.hpp"
#అన్తర్భావ "transpose.simd_declarations.hpp"
#అన్తర్భావ "nd_copy.hpp"

#అన్తర్భావ <algorithm> // std::swap_ranges
#అన్తర్భావ <numeric> // std::accumulate

నామాకాశ cv {

////////////////////////////////////// transpose /////////////////////////////////////////
#ఒకవేళ CV_SIMD128
స్థిర శూన్య transpose_8bit_simd(స్థిరరాశి uchar* src, size_t sstep, uchar* dst, size_t dstep, కొలత sz)
{
    స్థిరరాశి పూర్ణాంకం m = sz.వెడల్పు, n = sz.ఎత్తు;
    పూర్ణాంకం i = 0;
    కొరకు (; i <= m - 16; i += 16)
    {
        పూర్ణాంకం j = 0;
        కొరకు (; j <= n - 16; j += 16)
        {
            v_uint8x16 r0  = v_ఎక్కించు(src + i + sstep*(j+ 0));
            v_uint8x16 r1  = v_ఎక్కించు(src + i + sstep*(j+ 1));
            v_uint8x16 r2  = v_ఎక్కించు(src + i + sstep*(j+ 2));
            v_uint8x16 r3  = v_ఎక్కించు(src + i + sstep*(j+ 3));
            v_uint8x16 r4  = v_ఎక్కించు(src + i + sstep*(j+ 4));
            v_uint8x16 r5  = v_ఎక్కించు(src + i + sstep*(j+ 5));
            v_uint8x16 r6  = v_ఎక్కించు(src + i + sstep*(j+ 6));
            v_uint8x16 r7  = v_ఎక్కించు(src + i + sstep*(j+ 7));
            v_uint8x16 r8  = v_ఎక్కించు(src + i + sstep*(j+ 8));
            v_uint8x16 r9  = v_ఎక్కించు(src + i + sstep*(j+ 9));
            v_uint8x16 r10 = v_ఎక్కించు(src + i + sstep*(j+10));
            v_uint8x16 r11 = v_ఎక్కించు(src + i + sstep*(j+11));
            v_uint8x16 r12 = v_ఎక్కించు(src + i + sstep*(j+12));
            v_uint8x16 r13 = v_ఎక్కించు(src + i + sstep*(j+13));
            v_uint8x16 r14 = v_ఎక్కించు(src + i + sstep*(j+14));
            v_uint8x16 r15 = v_ఎక్కించు(src + i + sstep*(j+15));

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

            v_uint32x4 v0  = v_combine_దిగువ (u0,  u8);
            v_uint32x4 v1  = v_combine_ఎగువ(u0,  u8);
            v_uint32x4 v2  = v_combine_దిగువ (u1,  u9);
            v_uint32x4 v3  = v_combine_ఎగువ(u1,  u9);
            v_uint32x4 v4  = v_combine_దిగువ (u2,  u10);
            v_uint32x4 v5  = v_combine_ఎగువ(u2,  u10);
            v_uint32x4 v6  = v_combine_దిగువ (u3,  u11);
            v_uint32x4 v7  = v_combine_ఎగువ(u3,  u11);
            v_uint32x4 v8  = v_combine_దిగువ (u4,  u12);
            v_uint32x4 v9  = v_combine_ఎగువ(u4,  u12);
            v_uint32x4 v10 = v_combine_దిగువ (u5,  u13);
            v_uint32x4 v11 = v_combine_ఎగువ(u5,  u13);
            v_uint32x4 v12 = v_combine_దిగువ (u6,  u14);
            v_uint32x4 v13 = v_combine_ఎగువ(u6,  u14);
            v_uint32x4 v14 = v_combine_దిగువ (u7,  u15);
            v_uint32x4 v15 = v_combine_ఎగువ(u7,  u15);

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
        కొరకు (; j < n; j++)
            కొరకు (పూర్ణాంకం k = 0; k < 16; k++)
                dst[dstep*(i+k) + j] = src[i + sstep*j + k];
    }
    కొరకు (; i < m; i++)
        కొరకు (పూర్ణాంకం j = 0; j < n; j++)
            dst[dstep*i + j] = src[i + sstep*j];
}

స్థిర శూన్య transpose_16bit_simd(స్థిరరాశి uchar* src, size_t sstep, uchar* dst, size_t dstep, కొలత sz)
{
    స్థిరరాశి ushort* src16 = reinterpret_cast<స్థిరరాశి ushort*>(src);
    ushort* dst16 = reinterpret_cast<ushort*>(dst);

    స్థిరరాశి size_t sstep_e = sstep / పరిమాణం(ushort);
    స్థిరరాశి size_t dstep_e = dstep / పరిమాణం(ushort);

    స్థిరరాశి పూర్ణాంకం m = sz.వెడల్పు, n = sz.ఎత్తు;
    పూర్ణాంకం i = 0;

    కొరకు (; i <= m - 8; i += 8)
    {
        పూర్ణాంకం j = 0;
        కొరకు (; j <= n - 8; j += 8)
        {
            v_uint16x8 r0 = v_ఎక్కించు(src16 + i + sstep_e*(j+0));
            v_uint16x8 r1 = v_ఎక్కించు(src16 + i + sstep_e*(j+1));
            v_uint16x8 r2 = v_ఎక్కించు(src16 + i + sstep_e*(j+2));
            v_uint16x8 r3 = v_ఎక్కించు(src16 + i + sstep_e*(j+3));
            v_uint16x8 r4 = v_ఎక్కించు(src16 + i + sstep_e*(j+4));
            v_uint16x8 r5 = v_ఎక్కించు(src16 + i + sstep_e*(j+5));
            v_uint16x8 r6 = v_ఎక్కించు(src16 + i + sstep_e*(j+6));
            v_uint16x8 r7 = v_ఎక్కించు(src16 + i + sstep_e*(j+7));

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
        కొరకు (; j < n; j++)
            కొరకు (పూర్ణాంకం k = 0; k < 8; k++)
                dst16[dstep_e*(i+k) + j] = src16[i + sstep_e*j + k];
    }
    కొరకు (; i < m; i++)
        కొరకు (పూర్ణాంకం j = 0; j < n; j++)
            dst16[dstep_e*i + j] = src16[i + sstep_e*j];
}

స్థిర శూన్య transpose_32bit_simd(స్థిరరాశి uchar* src, size_t sstep, uchar* dst, size_t dstep, కొలత sz)
{
    // Cache-blocked + AVX2-aware path lives in transpose.simd.hpp; pick the
    // best variant available at runtime.
    CV_CPU_DISPATCH(transpose_32bit_ఖండాలు_simd, (src, sstep, dst, dstep, sz),
                    CV_CPU_DISPATCH_MODES_ALL);
}

స్థిర శూన్య transpose_48bit_simd(స్థిరరాశి uchar* src, size_t sstep, uchar* dst, size_t dstep, కొలత sz)
{
    స్థిరరాశి పొట్టి* src16 = reinterpret_cast<స్థిరరాశి పొట్టి*>(src);
    పొట్టి* dst16 = reinterpret_cast<పొట్టి*>(dst);

    స్థిరరాశి size_t sstep_e = sstep / పరిమాణం(పొట్టి);
    స్థిరరాశి size_t dstep_e = dstep / పరిమాణం(పొట్టి);

    స్థిరరాశి పూర్ణాంకం m = sz.వెడల్పు, n = sz.ఎత్తు;
    పూర్ణాంకం i = 0;

    కొరకు (; i <= m - 8; i += 8)
    {
        పూర్ణాంకం j = 0;
        కొరకు (; j <= n - 8; j += 8)
        {
            v_int16x8 C0_0, C1_0, C2_0;
            v_int16x8 C0_1, C1_1, C2_1;
            v_int16x8 C0_2, C1_2, C2_2;
            v_int16x8 C0_3, C1_3, C2_3;
            v_int16x8 C0_4, C1_4, C2_4;
            v_int16x8 C0_5, C1_5, C2_5;
            v_int16x8 C0_6, C1_6, C2_6;
            v_int16x8 C0_7, C1_7, C2_7;

            v_ఎక్కించు_deinterleave(src16 + sstep_e*(j+0) + i*3, C0_0, C1_0, C2_0);
            v_ఎక్కించు_deinterleave(src16 + sstep_e*(j+1) + i*3, C0_1, C1_1, C2_1);
            v_ఎక్కించు_deinterleave(src16 + sstep_e*(j+2) + i*3, C0_2, C1_2, C2_2);
            v_ఎక్కించు_deinterleave(src16 + sstep_e*(j+3) + i*3, C0_3, C1_3, C2_3);
            v_ఎక్కించు_deinterleave(src16 + sstep_e*(j+4) + i*3, C0_4, C1_4, C2_4);
            v_ఎక్కించు_deinterleave(src16 + sstep_e*(j+5) + i*3, C0_5, C1_5, C2_5);
            v_ఎక్కించు_deinterleave(src16 + sstep_e*(j+6) + i*3, C0_6, C1_6, C2_6);
            v_ఎక్కించు_deinterleave(src16 + sstep_e*(j+7) + i*3, C0_7, C1_7, C2_7);

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
        కొరకు (; j < n; j++)
            కొరకు (పూర్ణాంకం k = 0; k < 8; k++)
            {
                dst16[dstep_e*(i+k) + j*3 + 0] = src16[sstep_e*j + (i+k)*3 + 0];
                dst16[dstep_e*(i+k) + j*3 + 1] = src16[sstep_e*j + (i+k)*3 + 1];
                dst16[dstep_e*(i+k) + j*3 + 2] = src16[sstep_e*j + (i+k)*3 + 2];
            }
    }
    కొరకు (; i < m; i++)
        కొరకు (పూర్ణాంకం j = 0; j < n; j++)
        {
            dst16[dstep_e*i + j*3 + 0] = src16[sstep_e*j + i*3 + 0];
            dst16[dstep_e*i + j*3 + 1] = src16[sstep_e*j + i*3 + 1];
            dst16[dstep_e*i + j*3 + 2] = src16[sstep_e*j + i*3 + 2];
        }
}
#అయితే_ముగింపు

మూస<రకం_పేరు T> స్థిర శూన్య
transpose_( స్థిరరాశి uchar* src, size_t sstep, uchar* dst, size_t dstep, కొలత sz )
{
#ఒకవేళ CV_SIMD128
    ఎంపిక (పరిమాణం(T))
    {
        స్థితి 1: transpose_8bit_simd(src, sstep, dst, dstep, sz);  తిరిగి;
        స్థితి 2: transpose_16bit_simd(src, sstep, dst, dstep, sz); తిరిగి;
        స్థితి 4: transpose_32bit_simd(src, sstep, dst, dstep, sz); తిరిగి;
        స్థితి 6: transpose_48bit_simd(src, sstep, dst, dstep, sz); తిరిగి;
        అప్రమేయ: ఆపు;
    }
#అయితే_ముగింపు

    పూర్ణాంకం i = 0, j, m = sz.వెడల్పు, n = sz.ఎత్తు;

    #ఒకవేళ CV_ENABLE_UNROLLED
    కొరకు(; i <= m - 4; i += 4 )
    {
        T* d0 = (T*)(dst + dstep*i);
        T* d1 = (T*)(dst + dstep*(i+1));
        T* d2 = (T*)(dst + dstep*(i+2));
        T* d3 = (T*)(dst + dstep*(i+3));

        కొరకు( j = 0; j <= n - 4; j += 4 )
        {
            స్థిరరాశి T* s0 = (స్థిరరాశి T*)(src + i*పరిమాణం(T) + sstep*j);
            స్థిరరాశి T* s1 = (స్థిరరాశి T*)(src + i*పరిమాణం(T) + sstep*(j+1));
            స్థిరరాశి T* s2 = (స్థిరరాశి T*)(src + i*పరిమాణం(T) + sstep*(j+2));
            స్థిరరాశి T* s3 = (స్థిరరాశి T*)(src + i*పరిమాణం(T) + sstep*(j+3));

            d0[j] = s0[0]; d0[j+1] = s1[0]; d0[j+2] = s2[0]; d0[j+3] = s3[0];
            d1[j] = s0[1]; d1[j+1] = s1[1]; d1[j+2] = s2[1]; d1[j+3] = s3[1];
            d2[j] = s0[2]; d2[j+1] = s1[2]; d2[j+2] = s2[2]; d2[j+3] = s3[2];
            d3[j] = s0[3]; d3[j+1] = s1[3]; d3[j+2] = s2[3]; d3[j+3] = s3[3];
        }

        కొరకు( ; j < n; j++ )
        {
            స్థిరరాశి T* s0 = (స్థిరరాశి T*)(src + i*పరిమాణం(T) + j*sstep);
            d0[j] = s0[0]; d1[j] = s0[1]; d2[j] = s0[2]; d3[j] = s0[3];
        }
    }
    #అయితే_ముగింపు
    కొరకు( ; i < m; i++ )
    {
        T* d0 = (T*)(dst + dstep*i);
        j = 0;
        #ఒకవేళ CV_ENABLE_UNROLLED
        కొరకు(; j <= n - 4; j += 4 )
        {
            స్థిరరాశి T* s0 = (స్థిరరాశి T*)(src + i*పరిమాణం(T) + sstep*j);
            స్థిరరాశి T* s1 = (స్థిరరాశి T*)(src + i*పరిమాణం(T) + sstep*(j+1));
            స్థిరరాశి T* s2 = (స్థిరరాశి T*)(src + i*పరిమాణం(T) + sstep*(j+2));
            స్థిరరాశి T* s3 = (స్థిరరాశి T*)(src + i*పరిమాణం(T) + sstep*(j+3));

            d0[j] = s0[0]; d0[j+1] = s1[0]; d0[j+2] = s2[0]; d0[j+3] = s3[0];
        }
        #అయితే_ముగింపు
        కొరకు( ; j < n; j++ )
        {
            స్థిరరాశి T* s0 = (స్థిరరాశి T*)(src + i*పరిమాణం(T) + j*sstep);
            d0[j] = s0[0];
        }
    }
}

మూస<రకం_పేరు T> స్థిర శూన్య
transposeI_( uchar* దత్తాంశం, size_t అడుగు, పూర్ణాంకం n )
{
    కొరకు( పూర్ణాంకం i = 0; i < n; i++ )
    {
        T* అడ్డువరుస = (T*)(దత్తాంశం + అడుగు*i);
        uchar* data1 = దత్తాంశం + i*పరిమాణం(T);
        కొరకు( పూర్ణాంకం j = i+1; j < n; j++ )
            std::swap( అడ్డువరుస[j], *(T*)(data1 + అడుగు*j) );
    }
}

రకనిర్వచనం శూన్య (*TransposeFunc)( స్థిరరాశి uchar* src, size_t sstep, uchar* dst, size_t dstep, కొలత sz );
రకనిర్వచనం శూన్య (*TransposeInplaceFunc)( uchar* దత్తాంశం, size_t అడుగు, పూర్ణాంకం n );

#నిర్వచించు DEF_TRANSPOSE_FUNC(suffix, type) \
స్థిర శూన్య transpose_##suffix( స్థిరరాశి uchar* src, size_t sstep, uchar* dst, size_t dstep, కొలత sz ) \
{ transpose_<type>(src, sstep, dst, dstep, sz); } \
\
స్థిర శూన్య transposeI_##suffix( uchar* దత్తాంశం, size_t అడుగు, పూర్ణాంకం n ) \
{ transposeI_<type>(దత్తాంశం, అడుగు, n); }

DEF_TRANSPOSE_FUNC(8u, uchar)
DEF_TRANSPOSE_FUNC(16u, ushort)
DEF_TRANSPOSE_FUNC(8uC3, Vec3b)
DEF_TRANSPOSE_FUNC(32s, పూర్ణాంకం)
DEF_TRANSPOSE_FUNC(16uC3, Vec3s)
DEF_TRANSPOSE_FUNC(32sC2, Vec2i)
DEF_TRANSPOSE_FUNC(32sC3, Vec3i)
DEF_TRANSPOSE_FUNC(32sC4, Vec4i)
DEF_TRANSPOSE_FUNC(32sC6, Vec6i)
DEF_TRANSPOSE_FUNC(32sC8, Vec8i)

స్థిర TransposeFunc transposeTab[] =
{
    0, transpose_8u, transpose_16u, transpose_8uC3, transpose_32s, 0, transpose_16uC3, 0,
    transpose_32sC2, 0, 0, 0, transpose_32sC3, 0, 0, 0, transpose_32sC4,
    0, 0, 0, 0, 0, 0, 0, transpose_32sC6, 0, 0, 0, 0, 0, 0, 0, transpose_32sC8
};

nd::TransposeFunc nd::getTransposeFunc(size_t esz)
{
    తిరిగి esz < పరిమాణం(transposeTab)/పరిమాణం(transposeTab[0]) ? transposeTab[esz] : శూన్యసూచిక;
}

స్థిర TransposeInplaceFunc transposeInplaceTab[] =
{
    0, transposeI_8u, transposeI_16u, transposeI_8uC3, transposeI_32s, 0, transposeI_16uC3, 0,
    transposeI_32sC2, 0, 0, 0, transposeI_32sC3, 0, 0, 0, transposeI_32sC4,
    0, 0, 0, 0, 0, 0, 0, transposeI_32sC6, 0, 0, 0, 0, 0, 0, 0, transposeI_32sC8
};

#నిర్వచితమైతే HAVE_OPENCL

స్థిర తార్కిక ocl_transpose( ఆదానం_Array _src, OutputArray _dst )
{
    స్థిరరాశి ocl::Device & dev = ocl::Device::getDefault();
    స్థిరరాశి పూర్ణాంకం TILE_DIM = 32, ఖండం_అడ్డువరుసలు = 8;
    పూర్ణాంకం type = _src.type(), cn = CV_MAT_CN(type), లోతు = CV_MAT_లోతు(type),
        అడ్డువరుసలు_చొప్పున_WI = dev.isIntel() ? 4 : 1;

    UMat src = _src.getUMat();
    _dst.create(src.నిలువుల_సంఖ్య, src.అడ్డువరుసలు, type);
    UMat dst = _dst.getUMat();

    String kernelName("transpose");
    తార్కిక inplace = dst.u == src.u;

    ఒకవేళ (inplace)
    {
        CV_Assert(dst.నిలువుల_సంఖ్య == dst.అడ్డువరుసలు);
        kernelName += "_inplace";
    }
    లేకపోతే
    {
        // check required local memory size
        size_t required_local_జ్ఞాపకం = (size_t) TILE_DIM*(TILE_DIM+1)*CV_ELEM_కొలత(type);
        ఒకవేళ (required_local_జ్ఞాపకం > ocl::Device::getDefault().local_Mem_కొలత())
            తిరిగి అబద్ధం;
    }

    String deviceMacro;
    ఒకవేళ (dev.isIntel())
        deviceMacro = " -D INTEL_GPU";
    లేకపోతే
        deviceMacro = "";

    ocl::Kernel k(kernelName.c_str(), ocl::core::transpose_oclsrc,
                  format("-D T=%s -D T1=%s -D cn=%d -D TILE_DIM=%d -D BLOCK_ROWS=%d -D rowsPerWI=%d%s%s",
                         ocl::memop_Type_గమ్యం_Str(type), ocl::memop_Type_గమ్యం_Str(లోతు),
                         cn, TILE_DIM, ఖండం_అడ్డువరుసలు, అడ్డువరుసలు_చొప్పున_WI, inplace ? " -D INPLACE" : "", deviceMacro.c_str()));
    ఒకవేళ (k.empty())
        తిరిగి అబద్ధం;

    ఒకవేళ (inplace)
        k.వాదనలు(ocl::KernelArg::Read_Write_No_కొలత(dst), dst.అడ్డువరుసలు);
    లేకపోతే
        k.వాదనలు(ocl::KernelArg::ReadOnly(src),
               ocl::KernelArg::Write_Only_No_కొలత(dst));

    size_t localsize[2]  = { TILE_DIM, ఖండం_అడ్డువరుసలు };
    size_t globalsize[2] = { (size_t)src.నిలువుల_సంఖ్య, inplace ? ((size_t)src.అడ్డువరుసలు + అడ్డువరుసలు_చొప్పున_WI - 1) / అడ్డువరుసలు_చొప్పున_WI : (div_పైకి((size_t)src.అడ్డువరుసలు, TILE_DIM) * ఖండం_అడ్డువరుసలు) };

    ఒకవేళ (inplace && dev.isIntel())
    {
        localsize[0] = 16;
        localsize[1] = dev.max_Work_సమూహం_కొలత() / localsize[0];
    }

    తిరిగి k.నిరంతరం(2, globalsize, localsize, అబద్ధం);
}

#అయితే_ముగింపు

శూన్య transpose( ఆదానం_Array _src, OutputArray _dst )
{
    CV_INSTRUMENT_REGION();

    పూర్ణాంకం type = _src.type(), esz = CV_ELEM_కొలత(type);
    CV_Assert( _src.dims() <= 2 && esz <= 32 );

    CV_OCL_నిరంతరం(_dst.isUMat(),
               ocl_transpose(_src, _dst))

    Mat src = _src.getMat();
    ఒకవేళ( src.empty() )
    {
        _dst.release();
        తిరిగి;
    }

    _dst.create(src.నిలువుల_సంఖ్య, src.అడ్డువరుసలు, src.type());
    Mat dst = _dst.getMat();

    // handle the case of single-column/single-row matrices, stored in STL vectors.
    ఒకవేళ( src.అడ్డువరుసలు != dst.నిలువుల_సంఖ్య || src.నిలువుల_సంఖ్య != dst.అడ్డువరుసలు )
    {
        CV_Assert( src.size() == dst.size() && (src.నిలువుల_సంఖ్య == 1 || src.అడ్డువరుసలు == 1) );
        src.copy_గమ్యం(dst);
        తిరిగి;
    }

    CALL_HAL(transpose2d, cv_hal_transpose2d, src.దత్తాంశం, src.అడుగు, dst.దత్తాంశం, dst.అడుగు, src.నిలువుల_సంఖ్య, src.అడ్డువరుసలు, esz);

    ఒకవేళ( dst.దత్తాంశం == src.దత్తాంశం )
    {
        TransposeInplaceFunc func = transposeInplaceTab[esz];
        CV_Assert( func != 0 );
        CV_Assert( dst.నిలువుల_సంఖ్య == dst.అడ్డువరుసలు );
        func( dst.ptr(), dst.అడుగు, dst.అడ్డువరుసలు );
    }
    లేకపోతే
    {
        TransposeFunc func = transposeTab[esz];
        CV_Assert( func != 0 );
        func( src.ptr(), src.అడుగు, dst.ptr(), dst.అడుగు, src.size() );
    }
}


శూన్య transpose(ఆదానం_Array src_, స్థిరరాశి std::vector<పూర్ణాంకం>& క్రమం, OutputArray dst_)
{
    CV_INSTRUMENT_REGION();

    Mat inp = src_.getMat();
    స్థిరరాశి పూర్ణాంకం ndims = inp.dims;
    CV_Assert(!(ndims == 0 && inp.empty()));
    CV_CheckEQ(క్రమం.size(), static_cast<size_t>(ndims), "Number of dimensions shouldn't change");

    తార్కిక వాడిన[CV_MAX_DIM] = {అబద్ధం};
    పూర్ణాంకం newShape[CV_MAX_DIM];
    కొరకు (పూర్ణాంకం i = 0; i < ndims; i++)
    {
        పూర్ణాంకం j = క్రమం[i];
        CV_Check(j, 0 <= j && j < ndims && !వాడిన[j], "New order should be a valid permutation of the old one");
        వాడిన[j] = నిజం;
        newShape[i] = inp.size[j];
    }

    // plain 2D transpose keeps using cv::transpose (HAL, in-place square case);
    // not for an empty input, which cv::transpose would answer by releasing dst
    ఒకవేళ (ndims == 2 && క్రమం[0] == 1 && !inp.empty() && nd::getTransposeFunc(inp.elem_కొలత()))
    {
        transpose(inp, dst_);
        తిరిగి;
    }

    dst_.create(ndims, newShape, inp.type());
    Mat out = dst_.getMat();
    ఒకవేళ (out.మొత్తం() == 0)
        తిరిగి;
    nd::copy(nd::క్రమమార్పు(nd::view_యొక్క(inp), క్రమం.దత్తాంశం()), nd::view_యొక్క(out));
}

శూన్య transposeND(ఆదానం_Array src, స్థిరరాశి std::vector<పూర్ణాంకం>& క్రమం, OutputArray dst)
{
    transpose(src, క్రమం, dst);
}


// Generic scalar fallback
CV_ALWAYS_INLINE శూన్య flipHoriz_generic( స్థిరరాశి uchar* src, size_t sstep, uchar* dst, size_t dstep, కొలత size, size_t esz )
{
    పూర్ణాంకం i, j, హద్దు = (పూర్ణాంకం)(((size.వెడల్పు + 1)/2)*esz);
    పూర్ణాంకం ఎత్తు = size.ఎత్తు;
    Auto_బఫరు<పూర్ణాంకం> _tab(size.వెడల్పు*esz);
    పూర్ణాంకం* tab = _tab.దత్తాంశం();

    కొరకు( i = 0; i < size.వెడల్పు; i++ )
        కొరకు( size_t k = 0; k < esz; k++ )
            tab[i*esz + k] = (పూర్ణాంకం)((size.వెడల్పు - i - 1)*esz + k);

    కొరకు( ; ఎత్తు--; src += sstep, dst += dstep )
    {
        కొరకు( i = 0; i < హద్దు; i++ )
        {
            j = tab[i];
            uchar t0 = src[i], t1 = src[j];
            dst[i] = t1; dst[j] = t0;
        }
    }
}

#ఒకవేళ CV_SIMD || CV_SIMD_SCALABLE
మూస<రకం_పేరు V> CV_ALWAYS_INLINE శూన్య flipHoriz_single( స్థిరరాశి uchar* src, size_t sstep, uchar* dst, size_t dstep, కొలత size )
{
    రకనిర్వచనం రకం_పేరు VTraits<V>::lane_type T;
    స్థిరరాశి పూర్ణాంకం vlanes = VTraits<v_uint8>::vlanes();
    పూర్ణాంకం end = (పూర్ణాంకం)(size.వెడల్పు * పరిమాణం(T));
    పూర్ణాంకం వెడల్పు = (end + 1) / 2;
    పూర్ణాంకం వెడల్పు_simd = వెడల్పు & -vlanes;
    పూర్ణాంకం ఎత్తు = size.ఎత్తు;

    CV_DbgAssert(!CV_STRONG_ALIGNMENT || isAligned<పరిమాణం(T)>(src, dst));

    కొరకు( ; ఎత్తు--; src += sstep, dst += dstep )
    {
        పూర్ణాంకం i = 0, j = end;
        కొరకు( ; i < వెడల్పు_simd; i += vlanes, j -= vlanes )
        {
            V t0 = vx_ఎక్కించు((స్థిరరాశి T*)(src + i));
            V t1 = vx_ఎక్కించు((స్థిరరాశి T*)(src + j - vlanes));
            t0 = v_తిరగేయి(t0);
            t1 = v_తిరగేయి(t1);
            v_store((T*)(dst + j - vlanes), t0);
            v_store((T*)(dst + i), t1);
        }

        // Scalar tail loop
        ఒకవేళ (isAligned<పరిమాణం(T)>(src, dst))
        {
            కొరకు ( ; i < వెడల్పు; i += పరిమాణం(T), j -= పరిమాణం(T) )
            {
                T t0 = *((స్థిరరాశి T*)(src + i));
                T t1 = *((స్థిరరాశి T*)(src + j - పరిమాణం(T)));
                *((T*)(dst + j - పరిమాణం(T))) = t0;
                *((T*)(dst + i)) = t1;
            }
        }
        లేకపోతే
        {
            కొరకు ( ; i < వెడల్పు; i += పరిమాణం(T), j -= పరిమాణం(T) )
            {
                కొరకు (పూర్ణాంకం k = 0; k < (పూర్ణాంకం)పరిమాణం(T); k++)
                {
                    uchar t0 = src[i + k];
                    uchar t1 = src[j + k - పరిమాణం(T)];
                    dst[j + k - పరిమాణం(T)] = t0;
                    dst[i + k] = t1;
                }
            }
        }
    }
}

// SIMD for C3
మూస<రకం_పేరు V>
CV_ALWAYS_INLINE శూన్య flipHoriz_c3( స్థిరరాశి uchar* src, size_t sstep, uchar* dst, size_t dstep, కొలత size )
{
    రకనిర్వచనం రకం_పేరు VTraits<V>::lane_type T;
    స్థిరరాశి పూర్ణాంకం vlanes = VTraits<V>::vlanes();
    స్థిరరాశి పూర్ణాంకం stride = 3 * పరిమాణం(T);
    పూర్ణాంకం వెడల్పు = size.వెడల్పు;
    పూర్ణాంకం centre = (వెడల్పు + 1) / 2;
    పూర్ణాంకం వెడల్పు_simd = (centre / vlanes) * vlanes;
    పూర్ణాంకం ఎత్తు = size.ఎత్తు;

    కొరకు( ; ఎత్తు--; src += sstep, dst += dstep )
    {
        పూర్ణాంకం i = 0;
        కొరకు( ; i < వెడల్పు_simd; i += vlanes )
        {
            V r0, g0, b0;
            v_ఎక్కించు_deinterleave((స్థిరరాశి T*)(src + i * stride), r0, g0, b0);
            V r1, g1, b1;
            v_ఎక్కించు_deinterleave((స్థిరరాశి T*)(src + (వెడల్పు - i - vlanes) * stride), r1, g1, b1);
            r0 = v_తిరగేయి(r0);
            g0 = v_తిరగేయి(g0);
            b0 = v_తిరగేయి(b0);
            r1 = v_తిరగేయి(r1);
            g1 = v_తిరగేయి(g1);
            b1 = v_తిరగేయి(b1);
            v_store_interleave((T*)(dst + (వెడల్పు - i - vlanes) * stride), r0, g0, b0);
            v_store_interleave((T*)(dst + i * stride), r1, g1, b1);
        }
        // Scalar tail loop for remaining pixels
        కొరకు( ; i < centre; i++ )
        {
            పూర్ణాంకం j = వెడల్పు - i - 1;
            T c0 = ((స్థిరరాశి T*)(src + i * stride))[0];
            T c1 = ((స్థిరరాశి T*)(src + i * stride))[1];
            T c2 = ((స్థిరరాశి T*)(src + i * stride))[2];
            T c3 = ((స్థిరరాశి T*)(src + j * stride))[0];
            T c4 = ((స్థిరరాశి T*)(src + j * stride))[1];
            T c5 = ((స్థిరరాశి T*)(src + j * stride))[2];
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
మూస<size_t ESZ>
CV_ALWAYS_INLINE శూన్య flip_Horiz_vlanes_సరిపోలిక( స్థిరరాశి uchar* src, size_t sstep, uchar* dst, size_t dstep, కొలత size)
{
    స్థిరరాశి పూర్ణాంకం vlanes = VTraits<v_uint8>::vlanes();
    పూర్ణాంకం end = (పూర్ణాంకం)(size.వెడల్పు * ESZ);
    పూర్ణాంకం వెడల్పు = end / 2;
    పూర్ణాంకం ఎత్తు = size.ఎత్తు;
    పూర్ణాంకం e_కొలత = (పూర్ణాంకం)ESZ;
    కొరకు( ; ఎత్తు--; src += sstep, dst += dstep )
    {
        కొరకు( పూర్ణాంకం i = 0, j = end - e_కొలత; i < వెడల్పు; i += e_కొలత, j -= e_కొలత )
        {
            కొరకు( పూర్ణాంకం k = 0; k < e_కొలత; k += vlanes )
            {
                v_uint8 t0 = vx_ఎక్కించు(src + i + k);
                v_uint8 t1 = vx_ఎక్కించు(src + j + k);
                v_store(dst + j + k, t0);
                v_store(dst + i + k, t1);
            }
        }
    }
}

#ఒకవేళ CV_SIMD128
// SIMD flip when ESZ=16 (128-bit)
మూస<size_t ESZ>
CV_ALWAYS_INLINE శూన్య flip_Horiz_vlanes_సరిపోలిక_128( స్థిరరాశి uchar* src, size_t sstep, uchar* dst, size_t dstep, కొలత size)
{
    స్థిరరాశి పూర్ణాంకం vlanes16 = VTraits<v_uint8x16>::vlanes();
    పూర్ణాంకం end = (పూర్ణాంకం)(size.వెడల్పు * ESZ);
    పూర్ణాంకం వెడల్పు = end / 2;
    పూర్ణాంకం ఎత్తు = size.ఎత్తు;
    కొరకు( ; ఎత్తు--; src += sstep, dst += dstep )
    {
        కొరకు( పూర్ణాంకం i = 0, j = end - vlanes16; i < వెడల్పు; i += vlanes16, j -= vlanes16 )
        {
            v_uint8x16 t0 = v_ఎక్కించు(src + i);
            v_uint8x16 t1 = v_ఎక్కించు(src + j);
            v_store(dst + j, t0);
            v_store(dst + i, t1);
        }
    }
}
#అయితే_ముగింపు // CV_SIMD128

// SIMD flip for ESZ=16,32
మూస<size_t ESZ>
CV_ALWAYS_INLINE శూన్య flipHoriz_vlanes_dispatch( స్థిరరాశి uchar* src, size_t sstep, uchar* dst, size_t dstep, కొలత size )
{
    స్థిరరాశి పూర్ణాంకం vlanes = VTraits<v_uint8>::vlanes();
#ఒకవేళ CV_SIMD128
    స్థిరరాశి పూర్ణాంకం vlanes16 = VTraits<v_uint8x16>::vlanes();
#అయితే_ముగింపు
    ఒకవేళ ( (ESZ == (size_t)vlanes) || (ESZ == 2 * (size_t)vlanes))
    {
        flip_Horiz_vlanes_సరిపోలిక<ESZ>(src, sstep, dst, dstep, size);
        తిరిగి;
    }
#ఒకవేళ CV_SIMD128
    లేకపోతే ఒకవేళ (ESZ == vlanes16)
    {
        flip_Horiz_vlanes_సరిపోలిక_128<ESZ>(src, sstep, dst, dstep, size);
        తిరిగి;
    }
#అయితే_ముగింపు
    flipHoriz_generic(src, sstep, dst, dstep, size, ESZ);
}

#ఒకవేళ CV_SIMD128
// SIMD flip for ESZ=24 (CV_64FC3)
CV_ALWAYS_INLINE శూన్య flipHoriz_24( స్థిరరాశి uchar* src, size_t sstep, uchar* dst, size_t dstep, కొలత size )
{
    CV_DbgAssert(!CV_STRONG_ALIGNMENT || isAligned<8>(((size_t)src)|((size_t)dst)|sstep|dstep));

    స్థిరరాశి పూర్ణాంకం lanes16 = 16;
    పూర్ణాంకం end = (పూర్ణాంకం)(size.వెడల్పు * 24);
    పూర్ణాంకం వెడల్పు = (end + 1) / 2;
    పూర్ణాంకం ఎత్తు = size.ఎత్తు;
    కొరకు( ; ఎత్తు--; src += sstep, dst += dstep )
    {
        కొరకు ( పూర్ణాంకం i = 0, j = end; i < వెడల్పు; i += lanes16 + 8, j -= lanes16 + 8 )
        {
            v_uint8x16 t0 = v_ఎక్కించు(src + i);
            uint64_t t2 = *reinterpret_cast<స్థిరరాశి uint64_t*>(src + i + lanes16);
            v_uint8x16 t1 = v_ఎక్కించు(src + j - lanes16 - 8);
            uint64_t t3 = *reinterpret_cast<స్థిరరాశి uint64_t*>(src + j - 8);
            v_store(dst + j - lanes16 - 8, t0);
            *reinterpret_cast<uint64_t*>(dst + j - 8) = t2;
            v_store(dst + i, t1);
            *reinterpret_cast<uint64_t*>(dst + i + lanes16) = t3;
        }
    }
}
#అయితే_ముగింపు // CV_SIMD128
#అయితే_ముగింపు // CV_SIMD || CV_SIMD_SCALABLE

స్థిర శూన్య flipHoriz( స్థిరరాశి uchar* src, size_t sstep, uchar* dst, size_t dstep, కొలత size, size_t esz )
{
#ఒకవేళ CV_SIMD || CV_SIMD_SCALABLE
#ఒకవేళ CV_STRONG_ALIGNMENT
    స్థిరరాశి size_t alignMark = ((size_t)src)|((size_t)dst)|sstep|dstep;
#లేకపోతే
    స్థిరరాశి size_t alignMark = 0; // unaligned access is allowed: all the checks below fold to true
#అయితే_ముగింపు

    // SIMD-optimized dispatch
    ఎంపిక(esz)
    {
        స్థితి 1:                                                                               // CV_8UC1: 8-bit, 1 channel
            flipHoriz_single<v_uint8>(src, sstep, dst, dstep, size); తిరిగి;
        స్థితి 2:                                                                               // CV_8UC2, CV_16UC1: 8-bit 2-channel or 16-bit 1-channel
            ఒకవేళ (isAligned<2>(alignMark)) { flipHoriz_single<v_uint16>(src, sstep, dst, dstep, size); తిరిగి; }
            ఆపు;
        స్థితి 3:                                                                               // CV_8UC3: 8-bit, 3 channels
            flipHoriz_c3<v_uint8>(src, sstep, dst, dstep, size); తిరిగి;
        స్థితి 4:                                                                               // CV_8UC4, CV_16UC2, CV_32SC1, CV_32FC1: 8-bit 4-channel, 16-bit 2-channel, or 32-bit 1-channel
            ఒకవేళ (isAligned<4>(alignMark)) { flipHoriz_single<v_uint32>(src, sstep, dst, dstep, size); తిరిగి; }
            ఆపు;
        స్థితి 6:                                                                               // CV_16UC3, CV_16SC3: 16-bit, 3 channels
            ఒకవేళ (isAligned<2>(alignMark)) { flipHoriz_c3<v_uint16>(src, sstep, dst, dstep, size); తిరిగి; }
            ఆపు;
        స్థితి 8:                                                                               // CV_16UC4, CV_32SC2, CV_32FC2, CV_64FC1: 16-bit 4-channel, 32-bit 2-channel, or 64-bit 1-channel
            ఒకవేళ (isAligned<8>(alignMark)) { flipHoriz_single<v_uint64>(src, sstep, dst, dstep, size); తిరిగి; }
            ఆపు;
        స్థితి 12:                                                                              // CV_32SC3, CV_32FC3: 32-bit, 3 channels
            ఒకవేళ (isAligned<4>(alignMark)) { flipHoriz_c3<v_uint32>(src, sstep, dst, dstep, size); తిరిగి; }
            ఆపు;
        స్థితి 16:                                                                              // CV_32SC4, CV_32FC4, CV_64FC2: 32-bit 4-channel or 64-bit 2-channel
            flipHoriz_vlanes_dispatch<16>(src, sstep, dst, dstep, size); తిరిగి;               // byte-wise access only
#ఒకవేళ CV_SIMD128
        స్థితి 24:                                                                              // CV_64FC3: 64-bit, 3 channels
            ఒకవేళ (isAligned<8>(alignMark)) { flipHoriz_24(src, sstep, dst, dstep, size); తిరిగి; }
            ఆపు;
#అయితే_ముగింపు
        స్థితి 32:                                                                              // CV_64FC4: 64-bit, 4 channels
            flipHoriz_vlanes_dispatch<32>(src, sstep, dst, dstep, size); తిరిగి;               // byte-wise access only
        అప్రమేయ:
            ఆపు; // Fall through to generic implementation
    }
#అయితే_ముగింపు
    // Fallback: generic scalar
    flipHoriz_generic(src, sstep, dst, dstep, size, esz);
}

స్థిర శూన్య
flipVert( స్థిరరాశి uchar* src0, size_t sstep, uchar* dst0, size_t dstep, కొలత size, size_t esz )
{
    స్థిరరాశి uchar* src1 = src0 + (size.ఎత్తు - 1)*sstep;
    uchar* dst1 = dst0 + (size.ఎత్తు - 1)*dstep;
    size.వెడల్పు *= (పూర్ణాంకం)esz;

    కొరకు( పూర్ణాంకం y = 0; y < (size.ఎత్తు + 1)/2; y++, src0 += sstep, src1 -= sstep,
                                                  dst0 += dstep, dst1 -= dstep )
    {
        పూర్ణాంకం i = 0;
#ఒకవేళ (CV_SIMD || CV_SIMD_SCALABLE)
#ఒకవేళ CV_STRONG_ALIGNMENT
        ఒకవేళ (isAligned<పరిమాణం(పూర్ణాంకం)>(src0, src1, dst0, dst1))
#అయితే_ముగింపు
        {
            కొరకు (; i <= size.వెడల్పు - VTraits<v_uint8>::vlanes(); i += VTraits<v_uint8>::vlanes())
            {
                v_int32 t0 = v_reinterpret_as_s32(vx_ఎక్కించు(src0 + i));
                v_int32 t1 = v_reinterpret_as_s32(vx_ఎక్కించు(src1 + i));
                v_store(dst0 + i, v_reinterpret_as_u8(t1));
                v_store(dst1 + i, v_reinterpret_as_u8(t0));
            }
        }
#ఒకవేళ CV_STRONG_ALIGNMENT
        లేకపోతే
        {
            కొరకు (; i <= size.వెడల్పు - VTraits<v_uint8>::vlanes(); i += VTraits<v_uint8>::vlanes())
            {
                v_uint8 t0 = vx_ఎక్కించు(src0 + i);
                v_uint8 t1 = vx_ఎక్కించు(src1 + i);
                v_store(dst0 + i, t1);
                v_store(dst1 + i, t0);
            }
        }
#అయితే_ముగింపు
#అయితే_ముగింపు

        ఒకవేళ (isAligned<పరిమాణం(పూర్ణాంకం)>(src0, src1, dst0, dst1))
        {
            కొరకు( ; i <= size.వెడల్పు - 16; i += 16 )
            {
                పూర్ణాంకం t0 = ((పూర్ణాంకం*)(src0 + i))[0];
                పూర్ణాంకం t1 = ((పూర్ణాంకం*)(src1 + i))[0];

                ((పూర్ణాంకం*)(dst0 + i))[0] = t1;
                ((పూర్ణాంకం*)(dst1 + i))[0] = t0;

                t0 = ((పూర్ణాంకం*)(src0 + i))[1];
                t1 = ((పూర్ణాంకం*)(src1 + i))[1];

                ((పూర్ణాంకం*)(dst0 + i))[1] = t1;
                ((పూర్ణాంకం*)(dst1 + i))[1] = t0;

                t0 = ((పూర్ణాంకం*)(src0 + i))[2];
                t1 = ((పూర్ణాంకం*)(src1 + i))[2];

                ((పూర్ణాంకం*)(dst0 + i))[2] = t1;
                ((పూర్ణాంకం*)(dst1 + i))[2] = t0;

                t0 = ((పూర్ణాంకం*)(src0 + i))[3];
                t1 = ((పూర్ణాంకం*)(src1 + i))[3];

                ((పూర్ణాంకం*)(dst0 + i))[3] = t1;
                ((పూర్ణాంకం*)(dst1 + i))[3] = t0;
            }

            కొరకు( ; i <= size.వెడల్పు - 4; i += 4 )
            {
                పూర్ణాంకం t0 = ((పూర్ణాంకం*)(src0 + i))[0];
                పూర్ణాంకం t1 = ((పూర్ణాంకం*)(src1 + i))[0];

                ((పూర్ణాంకం*)(dst0 + i))[0] = t1;
                ((పూర్ణాంకం*)(dst1 + i))[0] = t0;
            }
        }

        కొరకు( ; i < size.వెడల్పు; i++ )
        {
            uchar t0 = src0[i];
            uchar t1 = src1[i];

            dst0[i] = t1;
            dst1[i] = t0;
        }
    }
}

#నిర్వచితమైతే HAVE_OPENCL

గణన { FLIP_నిలువుల_సంఖ్య = 1 << 0, FLIP_అడ్డువరుసలు = 1 << 1, FLIP_BOTH = FLIP_అడ్డువరుసలు | FLIP_నిలువుల_సంఖ్య };

స్థిర తార్కిక ocl_flip(ఆదానం_Array _src, OutputArray _dst, పూర్ణాంకం flip_సంకేతం )
{
    CV_Assert(flip_సంకేతం >= -1 && flip_సంకేతం <= 1);

    స్థిరరాశి ocl::Device & dev = ocl::Device::getDefault();
    పూర్ణాంకం type = _src.type(), లోతు = CV_MAT_లోతు(type), cn = CV_MAT_CN(type),
            flipType, kercn = std::min(ocl::predict_Optimal_సదిశ_వెడల్పు(_src, _dst), 4);

    తార్కిక doubleSupport = dev.doubleFPConfig() > 0;
    ఒకవేళ (!doubleSupport && లోతు == CV_64F)
        kercn = cn;

    ఒకవేళ (cn > 4)
        తిరిగి అబద్ధం;

    కొలత size = _src.size();
    _dst.create(size, type);
    UMat src = _src.getUMat(), dst = _dst.getUMat();
    తార్కిక inplace = (dst.u == src.u);

    String kernelName;
    ఒకవేళ (flip_సంకేతం == 0)
        kernelName = "arithm_flip_rows", flipType = FLIP_అడ్డువరుసలు;
    లేకపోతే ఒకవేళ (flip_సంకేతం > 0)
        kernelName = "arithm_flip_cols", flipType = FLIP_నిలువుల_సంఖ్య;
    లేకపోతే
        kernelName = "arithm_flip_rows_cols", flipType = FLIP_BOTH;

    ఒకవేళ(inplace)
        kernelName += "_inplace";

    పూర్ణాంకం px_చొప్పున_WIy = (dev.isIntel() && (dev.type() & ocl::Device::TYPE_GPU)) ? 4 : 1;
    kercn = (cn!=3 || flipType == FLIP_అడ్డువరుసలు) ? std::max(kercn, cn) : cn;
    స్థిరరాశి పూర్ణాంకం TILE_కొలత = 32, ఖండం_అడ్డువరుసలు = 8;

    ocl::Kernel k(kernelName.c_str(), ocl::core::flip_oclsrc,
                format( "-D T=%s -D T1=%s -D DEPTH=%d -D cn=%d -D PIX_PER_WI_Y=%d -D kercn=%d -D TILE_SIZE=%d -D BLOCK_ROWS=%d%s",
                kercn != cn ? ocl::type_గమ్యం_Str(CV_MAKE_TYPE(లోతు, kercn)) : ocl::vecop_Type_గమ్యం_Str(CV_MAKE_TYPE(లోతు, kercn)),
                kercn != cn ? ocl::type_గమ్యం_Str(లోతు) : ocl::vecop_Type_గమ్యం_Str(లోతు), లోతు, cn, px_చొప్పున_WIy, kercn, TILE_కొలత, ఖండం_అడ్డువరుసలు,
                inplace ? " -D INPLACE" : ""));
    ఒకవేళ (k.empty())
        తిరిగి అబద్ధం;

    పూర్ణాంకం నిలువుల_సంఖ్య = size.వెడల్పు * cn / kercn, అడ్డువరుసలు = size.ఎత్తు;
    పూర్ణాంకం work_నిలువుల_సంఖ్య = flipType == FLIP_నిలువుల_సంఖ్య ? (నిలువుల_సంఖ్య + 1) >> 1 : నిలువుల_సంఖ్య;
    పూర్ణాంకం work_అడ్డువరుసలు = flipType & FLIP_అడ్డువరుసలు ? (అడ్డువరుసలు + 1) >> 1 : అడ్డువరుసలు;

    ఒకవేళ (inplace)
    {
        k.వాదనలు(ocl::KernelArg::Read_Write_No_కొలత(dst), అడ్డువరుసలు, నిలువుల_సంఖ్య);

        పూర్ణాంకం gs_నిలువుల_సంఖ్య, gs_అడ్డువరుసలు;
        ఒకవేళ (flipType == FLIP_నిలువుల_సంఖ్య)
        {
            gs_నిలువుల_సంఖ్య = work_నిలువుల_సంఖ్య;
            gs_అడ్డువరుసలు = అడ్డువరుసలు;
        }
        లేకపోతే ఒకవేళ (flipType == FLIP_అడ్డువరుసలు)
        {
            gs_నిలువుల_సంఖ్య = నిలువుల_సంఖ్య;
            gs_అడ్డువరుసలు = work_అడ్డువరుసలు;
        }
        లేకపోతే // FLIP_BOTH
        {
            gs_నిలువుల_సంఖ్య = నిలువుల_సంఖ్య;
            gs_అడ్డువరుసలు = అడ్డువరుసలు;
        }

        size_t globalsize[2] = {
            (size_t)div_పైకి(gs_నిలువుల_సంఖ్య, TILE_కొలత) * TILE_కొలత,
            (size_t)div_పైకి(gs_అడ్డువరుసలు, TILE_కొలత) * ఖండం_అడ్డువరుసలు
        };
        size_t localsize[2] = { TILE_కొలత, ఖండం_అడ్డువరుసలు };
        తిరిగి k.నిరంతరం(2, globalsize, localsize, అబద్ధం);
    }
    లేకపోతే
    {
        k.వాదనలు(ocl::KernelArg::Read_Only_No_కొలత(src),
           ocl::KernelArg::WriteOnly(dst, cn, kercn), work_అడ్డువరుసలు, work_నిలువుల_సంఖ్య);

        size_t max_Work_సమూహం_కొలత = dev.max_Work_సమూహం_కొలత();
        CV_Assert(max_Work_సమూహం_కొలత % 4 == 0);

        size_t globalsize[2] = { (size_t)work_నిలువుల_సంఖ్య, ((size_t)work_అడ్డువరుసలు + px_చొప్పున_WIy - 1) / px_చొప్పున_WIy };
        size_t localsize[2]  = { max_Work_సమూహం_కొలత / 4, 4 };
        తిరిగి k.నిరంతరం(2, globalsize, (flipType == FLIP_నిలువుల_సంఖ్య) && !dev.isIntel() ? localsize : ఖాళీ, అబద్ధం);
    }
}

#అయితే_ముగింపు

శూన్య flip( ఆదానం_Array _src, OutputArray _dst, పూర్ణాంకం flip_బహుళకం )
{
    CV_INSTRUMENT_REGION();

    CV_Assert( _src.dims() <= 2 );
    కొలత size = _src.size();

    ఒకవేళ (flip_బహుళకం < 0)
    {
        ఒకవేళ (size.వెడల్పు == 1)
            flip_బహుళకం = 0;
        ఒకవేళ (size.ఎత్తు == 1)
            flip_బహుళకం = 1;
    }

    ఒకవేళ ((size.వెడల్పు == 1 && flip_బహుళకం > 0) ||
        (size.ఎత్తు == 1 && flip_బహుళకం == 0))
    {
        తిరిగి _src.copy_గమ్యం(_dst);
    }

    CV_OCL_నిరంతరం( _dst.isUMat(), ocl_flip(_src, _dst, flip_బహుళకం))

    Mat src = _src.getMat();
    పూర్ణాంకం type = src.type();
    _dst.create( size, type );
    Mat dst = _dst.getMat();

    CALL_HAL(flip, cv_hal_flip, type, src.ptr(), src.అడుగు, src.నిలువుల_సంఖ్య, src.అడ్డువరుసలు,
             dst.ptr(), dst.అడుగు, flip_బహుళకం);

    size_t esz = CV_ELEM_కొలత(type);

    ఒకవేళ( flip_బహుళకం <= 0 )
        flipVert( src.ptr(), src.అడుగు, dst.ptr(), dst.అడుగు, src.size(), esz );
    లేకపోతే
        flipHoriz( src.ptr(), src.అడుగు, dst.ptr(), dst.అడుగు, src.size(), esz );

    ఒకవేళ( flip_బహుళకం < 0 )
        flipHoriz( dst.ptr(), dst.అడుగు, dst.ptr(), dst.అడుగు, dst.size(), esz );
}

// In-place flip of a continuous array: swap the slices along the axis, no temporary copy.
స్థిర శూన్య flipNDInplace(Mat& m, పూర్ణాంకం axis)
{
    size_t nouter = 1;
    కొరకు (పూర్ణాంకం i = 0; i < axis; i++)
        nouter *= (size_t)m.size[i];
    స్థిరరాశి పూర్ణాంకం n = m.size[axis];
    స్థిరరాశి size_t అడుగు = m.అడుగు[axis];
    uchar* దత్తాంశం = m.ptr();
    కొరకు (size_t i = 0; i < nouter; i++, దత్తాంశం += n*అడుగు)
        కొరకు (పూర్ణాంకం j = 0, k = n - 1; j < k; j++, k--)
            std::swap_ranges(దత్తాంశం + j*అడుగు, దత్తాంశం + (j + 1)*అడుగు, దత్తాంశం + k*అడుగు);
}

శూన్య flipND(ఆదానం_Array _src, OutputArray _dst, పూర్ణాంకం _axis)
{
    CV_INSTRUMENT_REGION();

    Mat src = _src.getMat();

    // verify axis
    పూర్ణాంకం ndim = src.dims;
    CV_CheckLT(_axis, ndim, "flipND: given axis is out of range");
    CV_CheckGE(_axis, -ndim, "flipND: given axis is out of range");
    పూర్ణాంకం axis = (_axis + ndim) % ndim;

    _dst.create(ndim, src.size.p, src.type());
    Mat dst = _dst.getMat();
    ఒకవేళ (dst.దత్తాంశం == src.దత్తాంశం && src.isContinuous() && dst.isContinuous())
    {
        flipNDInplace(dst, axis);
        తిరిగి;
    }

    nd::View v = nd::view_యొక్క(src);
    nd::flip(v, axis);
    nd::copy(v, nd::view_యొక్క(dst));
}

శూన్య concat(ఆదానం_Array_యొక్క_Arrays _src, పూర్ణాంకం axis, OutputArray _dst)
{
    CV_INSTRUMENT_REGION();

    std::vector<Mat> src;
    _src.get_Mat_సదిశ(src);
    CV_Assert(!src.empty());

    స్థిరరాశి Mat& src0 = src[0];
    స్థిరరాశి పూర్ణాంకం ndims = src0.dims, type = src0.type(), ninputs = (పూర్ణాంకం)src.size();
    CV_CheckGE(ndims, 1, "concat: 0-dimensional arrays are not supported");
    CV_CheckGE(axis, -ndims, "concat: axis is out of range");
    CV_CheckLT(axis, ndims, "concat: axis is out of range");
    ఒకవేళ (axis < 0)
        axis += ndims;

    పూర్ణాంకం బహిర్గతం_Shape[CV_MAX_DIM];
    కొరకు (పూర్ణాంకం i = 0; i < ndims; i++)
        బహిర్గతం_Shape[i] = src0.size[i];
    int64 axis_కొలత = 0;
    కొరకు (పూర్ణాంకం k = 0; k < ninputs; k++)
    {
        స్థిరరాశి Mat& m = src[k];
        CV_CheckEQ(m.type(), type, "concat: all the input arrays must have the same type");
        CV_CheckEQ(m.dims, ndims, "concat: all the input arrays must have the same number of dimensions");
        కొరకు (పూర్ణాంకం i = 0; i < ndims; i++)
            ఒకవేళ (i != axis)
                CV_CheckEQ(m.size[i], బహిర్గతం_Shape[i], "concat: the input shapes must match except along the axis");
        axis_కొలత += m.size[axis];
    }
    CV_CheckLE((size_t)axis_కొలత, (size_t)గరిష్ఠ_పూర్ణాంకం, "concat: the output is too big");
    బహిర్గతం_Shape[axis] = (పూర్ణాంకం)axis_కొలత;

    _dst.create(ndims, బహిర్గతం_Shape, type);
    Mat dst = _dst.getMat();

    Auto_బఫరు<nd::View, 4> sv(ninputs), dv(ninputs);
    స్థిరరాశి nd::View dst0 = nd::view_యొక్క(dst);
    పూర్ణాంకం ofs = 0;
    కొరకు (పూర్ణాంకం k = 0; k < ninputs; k++)
    {
        స్థిరరాశి పూర్ణాంకం sz = src[k].size[axis];
        sv[k] = nd::view_యొక్క(src[k]);
        dv[k] = dst0;
        nd::ముక్క_2(dv[k], axis, ofs, 1, sz);
        ofs += sz;
    }
    nd::copyBatch(sv.దత్తాంశం(), dv.దత్తాంశం(), ninputs);
}

శూన్య విభజించు(ఆదానం_Array _src, పూర్ణాంకం axis, స్థిరరాశి std::vector<పూర్ణాంకం>& sizes, Output_Array_యొక్క_Arrays _dst)
{
    CV_INSTRUMENT_REGION();

    Mat src = _src.getMat();
    స్థిరరాశి పూర్ణాంకం ndims = src.dims, type = src.type(), noutputs = (పూర్ణాంకం)sizes.size();
    CV_CheckGE(ndims, 1, "split: 0-dimensional arrays are not supported");
    CV_CheckGE(axis, -ndims, "split: axis is out of range");
    CV_CheckLT(axis, ndims, "split: axis is out of range");
    ఒకవేళ (axis < 0)
        axis += ndims;
    CV_Assert(noutputs > 0);

    int64 axis_కొలత = 0;
    కొరకు (పూర్ణాంకం k = 0; k < noutputs; k++)
    {
        CV_CheckGE(sizes[k], 0, "split: the sizes must be non-negative");
        axis_కొలత += sizes[k];
    }
    CV_CheckEQ((size_t)axis_కొలత, (size_t)src.size[axis], "split: the sizes must sum up to the size of the axis");

    పూర్ణాంకం బహిర్గతం_Shape[CV_MAX_DIM];
    కొరకు (పూర్ణాంకం i = 0; i < ndims; i++)
        బహిర్గతం_Shape[i] = src.size[i];

    _dst.create(noutputs, 1, type, -1, నిజం);
    Auto_బఫరు<nd::View, 4> sv(noutputs), dv(noutputs);
    std::vector<Mat> dst(noutputs);
    స్థిరరాశి nd::View src0 = nd::view_యొక్క(src);
    పూర్ణాంకం ofs = 0;
    కొరకు (పూర్ణాంకం k = 0; k < noutputs; k++)
    {
        బహిర్గతం_Shape[axis] = sizes[k];
        _dst.create(ndims, బహిర్గతం_Shape, type, k);
        dst[k] = _dst.getMat(k);
        sv[k] = src0;
        nd::ముక్క_2(sv[k], axis, ofs, 1, sizes[k]);
        dv[k] = nd::view_యొక్క(dst[k]);
        ofs += sizes[k];
    }
    nd::copyBatch(sv.దత్తాంశం(), dv.దత్తాంశం(), noutputs);
}

శూన్య repeat(ఆదానం_Array _src, స్థిరరాశి std::vector<పూర్ణాంకం>& repeats, OutputArray _dst)
{
    CV_INSTRUMENT_REGION();

    Mat src = _src.getMat();
    స్థిరరాశి పూర్ణాంకం ndims = src.dims;
    CV_Assert(!(ndims == 0 && src.empty()));
    CV_CheckEQ(repeats.size(), (size_t)ndims, "repeat: the number of repeats must match the number of dimensions");

    పూర్ణాంకం బహిర్గతం_Shape[CV_MAX_DIM];
    కొరకు (పూర్ణాంకం i = 0; i < ndims; i++)
    {
        CV_CheckGE(repeats[i], 0, "repeat: the repeats must be non-negative");
        int64 sz = (int64)src.size[i]*repeats[i];
        CV_CheckLE((size_t)sz, (size_t)గరిష్ఠ_పూర్ణాంకం, "repeat: the output is too big");
        బహిర్గతం_Shape[i] = (పూర్ణాంకం)sz;
    }

    _dst.create(ndims, బహిర్గతం_Shape, src.type());
    Mat dst = _dst.getMat();
    ఒకవేళ (dst.మొత్తం() == 0 || src.empty())
        తిరిగి;

    // Each repeated output axis is split into (repeat index, position), the former
    // with source step 0, so the whole tile is one broadcasting copy.
    nd::View sv, dv;
    sv.దత్తాంశం = src.దత్తాంశం;
    dv.దత్తాంశం = dst.దత్తాంశం;
    sv.esz = dv.esz = src.elem_కొలత();
    పూర్ణాంకం n = 0;
    కొరకు (పూర్ణాంకం i = 0; i < ndims; i++)
    {
        స్థిరరాశి పూర్ణాంకం sz = src.size[i], r = repeats[i];
        ఒకవేళ (r != 1)
        {
            CV_CheckLT(n, (పూర్ణాంకం)nd::MAX_VIEW_DIMS, "repeat: too many dimensions");
            sv.size[n] = dv.size[n] = r;
            sv.అడుగు[n] = 0;
            dv.అడుగు[n] = (ptrdiff_t)dst.అడుగు[i]*sz;
            n++;
        }
        ఒకవేళ (sz != 1 || r == 1)
        {
            CV_CheckLT(n, (పూర్ణాంకం)nd::MAX_VIEW_DIMS, "repeat: too many dimensions");
            sv.size[n] = dv.size[n] = sz;
            sv.అడుగు[n] = (ptrdiff_t)src.అడుగు[i];
            dv.అడుగు[n] = (ptrdiff_t)dst.అడుగు[i];
            n++;
        }
    }
    sv.dims = dv.dims = n;
    nd::copy(sv, dv);
}

శూన్య ముక్క_2(ఆదానం_Array _src, స్థిరరాశి std::vector<పూర్ణాంకం>& starts, స్థిరరాశి std::vector<పూర్ణాంకం>& ends,
             స్థిరరాశి std::vector<పూర్ణాంకం>& అడుగులు, OutputArray _dst)
{
    CV_INSTRUMENT_REGION();

    Mat src = _src.getMat();
    స్థిరరాశి పూర్ణాంకం ndims = src.dims, nslices = (పూర్ణాంకం)starts.size();
    CV_Assert(!(ndims == 0 && src.empty()));
    CV_CheckLE(nslices, ndims, "slice: too many starts");
    CV_CheckEQ(ends.size(), starts.size(), "slice: starts and ends must have the same size");
    CV_Check(అడుగులు.size(), అడుగులు.empty() || అడుగులు.size() == starts.size(),
             "slice: steps must be empty or have the same size as starts");

    nd::View sv = nd::view_యొక్క(src);
    పూర్ణాంకం బహిర్గతం_Shape[CV_MAX_DIM];
    కొరకు (పూర్ణాంకం i = 0; i < ndims; i++)
        బహిర్గతం_Shape[i] = src.size[i];

    కొరకు (పూర్ణాంకం i = 0; i < nslices; i++)
    {
        స్థిరరాశి పూర్ణాంకం n = src.size[i], ప్రారంభం = starts[i], end = ends[i];
        స్థిరరాశి పూర్ణాంకం అడుగు = అడుగులు.empty() ? 1 : అడుగులు[i];
        CV_CheckNE(అడుగు, 0, "slice: step must not be zero");
        పూర్ణాంకం లెక్క_2;
        ఒకవేళ (అడుగు > 0)
        {
            CV_Check(ప్రారంభం, 0 <= ప్రారంభం && ప్రారంభం <= n, "slice: start is out of range");
            CV_Check(end, 0 <= end && end <= n, "slice: end is out of range");
            లెక్క_2 = end > ప్రారంభం ? (పూర్ణాంకం)(((int64)end - ప్రారంభం + అడుగు - 1)/అడుగు) : 0;
        }
        లేకపోతే
        {
            CV_Check(ప్రారంభం, -1 <= ప్రారంభం && ప్రారంభం < n, "slice: start is out of range");
            CV_Check(end, -1 <= end && end < n, "slice: end is out of range");
            లెక్క_2 = ప్రారంభం > end ? (పూర్ణాంకం)(((int64)ప్రారంభం - end - అడుగు - 1)/(-అడుగు)) : 0;
        }
        nd::ముక్క_2(sv, i, ప్రారంభం, అడుగు, లెక్క_2);
        బహిర్గతం_Shape[i] = లెక్క_2;
    }

    _dst.create(ndims, బహిర్గతం_Shape, src.type());
    Mat dst = _dst.getMat();
    ఒకవేళ (dst.మొత్తం() == 0 || src.empty())
        తిరిగి;
    nd::copy(sv, nd::view_యొక్క(dst));
}

/*
    This function first prepends 1 to each tensor shape to have a common max_ndims dimension, then flatten non-broadcast dimensions.
*/
స్థిర తార్కిక flatten_for_ప్రసారం(పూర్ణాంకం narrays, పూర్ణాంకం max_ndims, స్థిరరాశి పూర్ణాంకం* ndims, స్థిరరాశి పూర్ణాంకం** orig_shape,
                                   పూర్ణాంకం** flatten_shape, size_t** flatten_అడుగు) {
    పూర్ణాంకం i, j, k;

    // step 1.
    // * make all inputs and the output max_ndims-dimensional.
    // * compute proper step's
    కొరకు (i = max_ndims - 1; i >= 0; i-- ) {
        కొరకు (k = 0; k < narrays; k++) {
            j = ndims[k] - (max_ndims - i);
            పూర్ణాంకం sz_i = j >= 0 ? orig_shape[k][j] : 1;
            size_t st_i = i == max_ndims - 1 ? 1 : flatten_అడుగు[k][i+1] * flatten_shape[k][i+1];
            flatten_shape[k][i] = sz_i;
            flatten_అడుగు[k][i] = st_i;
            ఒకవేళ (flatten_shape[k][i] == 0)
                తిరిగి అబద్ధం;
        }
    }

    // step 2. Let's do the flattening first,
    // since we'd need proper values of steps to check continuity.
    // this loop is probably the most tricky part
    // in the whole implementation of broadcasting.
    j = max_ndims-1;
    కొరకు (i = j - 1; i >= 0; i--) {
        తార్కిక all_contiguous = నిజం, all_scalars = నిజం, all_consistent = నిజం;
        కొరకు(k = 0; k < narrays; k++) {
            size_t st = flatten_అడుగు[k][j] * flatten_shape[k][j];
            తార్కిక prev_scalar = flatten_shape[k][j] == 1;
            తార్కిక scalar = flatten_shape[k][i] == 1;
            all_contiguous = all_contiguous && (st == flatten_అడుగు[k][i]);
            all_scalars = all_scalars && scalar;
            all_consistent = all_consistent && (scalar == prev_scalar);
        }
        ఒకవేళ (all_contiguous && (all_consistent || all_scalars)) {
            కొరకు(k = 0; k < narrays; k++)
                flatten_shape[k][j] *= flatten_shape[k][i];
        } లేకపోతే {
            j--;
            ఒకవేళ (i < j) {
                కొరకు(k = 0; k < narrays; k++) {
                    flatten_shape[k][j] = flatten_shape[k][i];
                    flatten_అడుగు[k][j] = flatten_అడుగు[k][i];
                }
            }
        }
    }

    // step 3. Set some step's to 0's.
    కొరకు (i = max_ndims-1; i >= j; i--) {
        కొరకు (k = 0; k < narrays; k++)
            flatten_అడుగు[k][i] = flatten_shape[k][i] == 1 ? 0 : flatten_అడుగు[k][i];
    }
    కొరకు (; i >= 0; i--) {
        కొరకు (k = 0; k < narrays; k++) {
            flatten_అడుగు[k][i] = 0;
            flatten_shape[k][i] = 1;
        }
    }
    తిరిగి నిజం;
}

శూన్య ప్రసారం(ఆదానం_Array _src, ఆదానం_Array _shape, OutputArray _dst) {
    CV_INSTRUMENT_REGION();

    Mat src = _src.getMat();
    CV_CheckTrue(src.isContinuous(), "broadcast: input array must be contiguous");
    CV_CheckChannelsEQ(src.channels(), 1, "broadcast: input array must be single channel");

    Mat shape = _shape.getMat();
    CV_CheckTypeEQ(shape.type(), CV_32S, "broadcast: target shape must be of type int32");
    స్థిరరాశి స్వయంచాలక dims_shape = static_cast<పూర్ణాంకం>(shape.మొత్తం());
    స్థిరరాశి స్వయంచాలక *ptr_shape = shape.ptr<పూర్ణాంకం>();

    // check valid shape, 1D/0D Mat would fail in the following checks
    స్థిరరాశి స్వయంచాలక dims_src = src.dims;
    CV_CheckLE(dims_src, dims_shape,
               "broadcast: dimension of input array must be less than or equal to dimension of target shape");
    std::vector<పూర్ణాంకం> shape_src{src.size.p, src.size.p + dims_src};
    ఒకవేళ (shape_src.size() < static_cast<size_t>(dims_shape)) {
        shape_src.insert(shape_src.begin(), dims_shape - shape_src.size(), 1);
    }
    కొరకు (పూర్ణాంకం i = 0; i < static_cast<పూర్ణాంకం>(shape_src.size()); ++i) {
        స్థిరరాశి స్వయంచాలక *shape_లక్ష్యం = ptr_shape;
        ఒకవేళ (shape_src[i] != 1) {
            CV_CheckEQ(shape_src[i], shape_లక్ష్యం[i], "target shape must be equal to input shape or 1");
        }
    }

    // impl
    _dst.create(dims_shape, shape.ptr<పూర్ణాంకం>(), src.type());
    Mat dst = _dst.getMat();
    ఒకవేళ (dst.మొత్తం() == 0)
        తిరిగి;
    cv::Auto_బఫరు<పూర్ణాంకం> is_same_shape(dims_shape, 0);
    కొరకు (పూర్ణాంకం i = 0; i < static_cast<పూర్ణాంకం>(shape_src.size()); ++i) {
        ఒకవేళ (shape_src[i] == ptr_shape[i]) {
            is_same_shape[i] = 1;
        }
    }
    // copy if same shape
    ఒకవేళ (std::accumulate(is_same_shape.begin(), is_same_shape.end(), 1, std::multiplies<పూర్ణాంకం>()) != 0) {
        స్థిరరాశి స్వయంచాలక *p_src = src.ptr<స్థిరరాశి అక్షరం>();
        స్వయంచాలక *p_dst = dst.ptr<అక్షరం>();
        std::జ్ఞాపకం_నకలు(p_dst, p_src, dst.మొత్తం() * dst.elem_కొలత());
        తిరిగి;
    }
    // other cases
    పూర్ణాంకం max_ndims = std::max(dims_src, dims_shape);
    ఒకవేళ (max_ndims < 2 && src.మొత్తం() == 1) {
        స్థిరరాశి అక్షరం* p_src = src.ptr<స్థిరరాశి అక్షరం>();
        అక్షరం* p_dst = dst.ptr<అక్షరం>();
        size_t esz = src.elem_కొలత();
        కొరకు (size_t j = 0; j < dst.మొత్తం(); j++)
            std::జ్ఞాపకం_నకలు(p_dst + j * esz, p_src, esz);
        తిరిగి;
    }
    స్థిరరాశి పూర్ణాంకం all_ndims[2] = {src.dims, dst.dims};
    స్థిరరాశి పూర్ణాంకం* orig_shapes[2] = {src.size.p, dst.size.p};
    cv::Auto_బఫరు<size_t> buff(max_ndims * 4);
    పూర్ణాంకం* flatten_shapes[2] = {(పూర్ణాంకం*)buff.దత్తాంశం(), (పూర్ణాంకం*)(buff.దత్తాంశం() + max_ndims)};
    size_t* flatten_అడుగులు[2] = {(size_t*)(buff.దత్తాంశం() + 2 * max_ndims), (size_t*)(buff.దత్తాంశం() + 3 * max_ndims)};
    ఒకవేళ (flatten_for_ప్రసారం(2, max_ndims, all_ndims, orig_shapes, flatten_shapes, flatten_అడుగులు)) {
        size_t src_dp = flatten_అడుగులు[0][max_ndims - 1];
        size_t dst_dp = flatten_అడుగులు[1][max_ndims - 1];
        CV_Assert(dst_dp == 1 || dst_dp == 0);
        CV_Assert(max_ndims >= 2); // >= 3?
        size_t rowstep_src = flatten_అడుగులు[0][max_ndims - 2];
        size_t rowstep_dst = flatten_అడుగులు[1][max_ndims - 2];
        స్థిరరాశి అక్షరం* ptr_src = src.ptr<స్థిరరాశి అక్షరం>();
        అక్షరం* ptr_dst = dst.ptr<అక్షరం>();
        size_t esz = src.elem_కొలత();
        పూర్ణాంకం nrows = flatten_shapes[1][max_ndims - 2];
        పూర్ణాంకం ncols = flatten_shapes[1][max_ndims - 1];
        పూర్ణాంకం nplanes = 1;
        CV_Check(esz, esz == 1 || esz == 2 || esz == 4 || esz == 8, "broadcast: not supported data type");

        కొరకు (పూర్ణాంకం k = 0; k < max_ndims - 2; k++) {
            nplanes *= flatten_shapes[1][k];
        }
        కొరకు (పూర్ణాంకం plane_idx = 0; plane_idx < nplanes; plane_idx++) {
            size_t స్థానభ్రంశం_src = 0, స్థానభ్రంశం_dst = 0;
            size_t idx = (size_t)plane_idx;
            కొరకు (పూర్ణాంకం k = max_ndims - 3; k >= 0; k--) {
                size_t prev_idx = idx / flatten_shapes[1][k];
                size_t i_k = (పూర్ణాంకం)(idx - prev_idx * flatten_shapes[1][k]);
                స్థానభ్రంశం_src += i_k * flatten_అడుగులు[0][k];
                స్థానభ్రంశం_dst += i_k * flatten_అడుగులు[1][k];
                idx = prev_idx;
            }

            #నిర్వచించు OPENCV_CORE_ప్రసారం_LOOP(_Tp) \
                కొరకు (పూర్ణాంకం i = 0; i < nrows; i++) {   \
                    స్థిరరాశి _Tp *ptr_src_ = (స్థిరరాశి _Tp*)ptr_src + స్థానభ్రంశం_src + rowstep_src * i; \
                    _Tp *ptr_dst_ = (_Tp*)ptr_dst + స్థానభ్రంశం_dst + rowstep_dst * i; \
                    ఒకవేళ (src_dp == 1) { \
                        కొరకు (పూర్ణాంకం j = 0; j < ncols; j++) { \
                            ptr_dst_[j] = ptr_src_[j]; \
                        } \
                    } లేకపోతే { \
                        _Tp x = *ptr_src_; \
                        కొరకు (పూర్ణాంకం j = 0; j < ncols; j++) { \
                            ptr_dst_[j] = x; \
                        } \
                    } \
                }

            ఒకవేళ (esz == 1) {
                OPENCV_CORE_ప్రసారం_LOOP(int8_t);
            } లేకపోతే ఒకవేళ (esz == 2) {
                OPENCV_CORE_ప్రసారం_LOOP(int16_t);
            } లేకపోతే ఒకవేళ (esz == 4) {
                OPENCV_CORE_ప్రసారం_LOOP(int32_t);
            } లేకపోతే ఒకవేళ (esz == 8) {
                OPENCV_CORE_ప్రసారం_LOOP(int64_t);
            } లేకపోతే {
                CV_Error(cv::Error::StsNotImplemented, "");
            }
            #నిర్వచనం_రద్దు OPENCV_CORE_ప్రసారం_LOOP
        }
    } లేకపోతే {
        ఒకవేళ (dst.మొత్తం() == 0)
            తిరిగి;
        // initial copy (src to dst)
        std::vector<size_t> అడుగు_src{src.అడుగు.p, src.అడుగు.p + dims_src};
        ఒకవేళ (అడుగు_src.size() < static_cast<size_t>(dims_shape)) {
            అడుగు_src.insert(అడుగు_src.begin(), dims_shape - అడుగు_src.size(), అడుగు_src[0]);
        }
        కొరకు (size_t i = 0; i < src.మొత్తం(); ++i) {
            size_t t = i;
            size_t src_స్థానభ్రంశం = 0, dst_స్థానభ్రంశం = 0;
            కొరకు (పూర్ణాంకం j = static_cast<పూర్ణాంకం>(shape_src.size() - 1); j >= 0; --j) {
                size_t idx = t / shape_src[j];
                size_t offset = static_cast<size_t>(t - idx * shape_src[j]);
                src_స్థానభ్రంశం += offset * అడుగు_src[j];
                dst_స్థానభ్రంశం += offset * dst.అడుగు[j];
                t = idx;
            }
            స్థిరరాశి స్వయంచాలక *p_src = src.ptr<స్థిరరాశి అక్షరం>();
            స్వయంచాలక *p_dst = dst.ptr<అక్షరం>();
            std::జ్ఞాపకం_నకలు(p_dst + dst_స్థానభ్రంశం, p_src + src_స్థానభ్రంశం, dst.elem_కొలత());
        }
        // broadcast copy (dst inplace)
        Auto_బఫరు<పూర్ణాంకం> cumulative_shape(dims_shape, 1);
        పూర్ణాంకం మొత్తం = static_cast<పూర్ణాంకం>(dst.మొత్తం());
        కొరకు (పూర్ణాంకం i = dims_shape - 1; i >= 0; --i) {
            cumulative_shape[i] = static_cast<పూర్ణాంకం>(మొత్తం / ptr_shape[i]);
            మొత్తం = cumulative_shape[i];
        }
        కొరకు (పూర్ణాంకం i = dims_shape - 1; i >= 0; --i) {
            ఒకవేళ (is_same_shape[i] == 1) {
                కొనసాగు;
            }
            స్వయంచాలక అడుగు = dst.అడుగు[i];
            స్వయంచాలక *p_dst = dst.ptr<అక్షరం>();
            కొరకు (పూర్ణాంకం j = 0; j < cumulative_shape[i]; j++) {
                కొరకు (పూర్ణాంకం k = 0; k < ptr_shape[i] - 1; k++) {
                    std::జ్ఞాపకం_నకలు(p_dst + అడుగు, p_dst, అడుగు);
                    p_dst += అడుగు;
                }
                p_dst += అడుగు;
            }
        }
    }
}

శూన్య ప్రసారం(ఆదానం_Array _src, స్థిరరాశి MatShape& _shape, OutputArray _dst)
{
    ఒకవేళ (_shape.dims < 0) {
        _dst.release();
    } లేకపోతే {
        Mat shape(1, _shape.dims, CV_32S, (పూర్ణాంకం*)_shape.p);
        ప్రసారం(_src, shape, _dst);
    }
}

స్థిర శూన్య rotateImpl(ఆదానం_Array _src, OutputArray _dst, పూర్ణాంకం rotate_బహుళకం)
{
    ఎంపిక (rotate_బహుళకం)
    {
    స్థితి ROTATE_90_CLOCKWISE:
        transpose(_src, _dst);
        flip(_dst, _dst, 1);
        ఆపు;
    స్థితి ROTATE_180:
        flip(_src, _dst, -1);
        ఆపు;
    స్థితి ROTATE_90_COUNTERCLOCKWISE:
        transpose(_src, _dst);
        flip(_dst, _dst, 0);
        ఆపు;
    అప్రమేయ:
        ఆపు;
    }
}

శూన్య rotate(ఆదానం_Array _src, OutputArray _dst, పూర్ణాంకం rotate_బహుళకం)
{
    CV_Assert(_src.dims() <= 2);
    పూర్ణాంకం angle;

    ఒకవేళ (_dst.isUMat())
    {
        rotateImpl(_src, _dst, rotate_బహుళకం);
        తిరిగి;
    }

    Mat src = _src.getMat();
    పూర్ణాంకం type = src.type();
    ఒకవేళ( src.empty() )
    {
        _dst.release();
        తిరిగి;
    }

    ఎంపిక (rotate_బహుళకం)
    {
    స్థితి ROTATE_90_CLOCKWISE:
        _dst.create(src.నిలువుల_సంఖ్య, src.అడ్డువరుసలు, type);
        angle = 90;
        ఆపు;
    స్థితి ROTATE_180:
        _dst.create(src.అడ్డువరుసలు, src.నిలువుల_సంఖ్య, type);
        angle = 180;
        ఆపు;
    స్థితి ROTATE_90_COUNTERCLOCKWISE:
        _dst.create(src.నిలువుల_సంఖ్య, src.అడ్డువరుసలు, type);
        angle = 270;
        ఆపు;
    అప్రమేయ:
        _dst.create(src.అడ్డువరుసలు, src.నిలువుల_సంఖ్య, type);
        angle = 0;
        ఆపు;
    }

    Mat dst = _dst.getMat();
    CALL_HAL(rotate90, cv_hal_rotate90, type, src.ptr(), src.అడుగు, src.నిలువుల_సంఖ్య, src.అడ్డువరుసలు,
             dst.ptr(), dst.అడుగు, angle);

    // use src (Mat) since _src (InputArray) is updated by _dst.create() when in-place
    rotateImpl(src, _dst, rotate_బహుళకం);
}

}  // namespace
