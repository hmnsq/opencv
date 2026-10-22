// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html
// Copyright (C) 2026, Advanced Micro Devices, Inc., all rights reserved.
#ಅನ್ತರ್ಭಾವ "precomp.hpp"
#ಅನ್ತರ್ಭಾವ "opencl_kernels_core.hpp"
#ಅನ್ತರ್ಭಾವ "hal_replacement.hpp"
#ಅನ್ತರ್ಭಾವ "opencv2/core/detail/dispatch_helper.impl.hpp"

#ಅನ್ತರ್ಭಾವ "transpose.simd.hpp"
#ಅನ್ತರ್ಭಾವ "transpose.simd_declarations.hpp"
#ಅನ್ತರ್ಭಾವ "nd_copy.hpp"

#ಅನ್ತರ್ಭಾವ <algorithm> // std::swap_ranges
#ಅನ್ತರ್ಭಾವ <numeric> // std::accumulate

ನಾಮಾಕಾಶ cv {

////////////////////////////////////// transpose /////////////////////////////////////////
#ಆದರೆ CV_SIMD128
ಸ್ಥಿರ ಶೂನ್ಯ transpose_8bit_simd(ಸ್ಥಿರಾಂಕ uchar* src, size_t sstep, uchar* dst, size_t dstep, ಗಾತ್ರ_2 sz)
{
    ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ m = sz.ಅಗಲ, n = sz.ಎತ್ತರ;
    ಪೂರ್ಣಾಂಕ i = 0;
    ಗಾಗಿ (; i <= m - 16; i += 16)
    {
        ಪೂರ್ಣಾಂಕ j = 0;
        ಗಾಗಿ (; j <= n - 16; j += 16)
        {
            v_uint8x16 r0  = v_ತುಂಬು(src + i + sstep*(j+ 0));
            v_uint8x16 r1  = v_ತುಂಬು(src + i + sstep*(j+ 1));
            v_uint8x16 r2  = v_ತುಂಬು(src + i + sstep*(j+ 2));
            v_uint8x16 r3  = v_ತುಂಬು(src + i + sstep*(j+ 3));
            v_uint8x16 r4  = v_ತುಂಬು(src + i + sstep*(j+ 4));
            v_uint8x16 r5  = v_ತುಂಬು(src + i + sstep*(j+ 5));
            v_uint8x16 r6  = v_ತುಂಬು(src + i + sstep*(j+ 6));
            v_uint8x16 r7  = v_ತುಂಬು(src + i + sstep*(j+ 7));
            v_uint8x16 r8  = v_ತುಂಬು(src + i + sstep*(j+ 8));
            v_uint8x16 r9  = v_ತುಂಬು(src + i + sstep*(j+ 9));
            v_uint8x16 r10 = v_ತುಂಬು(src + i + sstep*(j+10));
            v_uint8x16 r11 = v_ತುಂಬು(src + i + sstep*(j+11));
            v_uint8x16 r12 = v_ತುಂಬು(src + i + sstep*(j+12));
            v_uint8x16 r13 = v_ತುಂಬು(src + i + sstep*(j+13));
            v_uint8x16 r14 = v_ತುಂಬು(src + i + sstep*(j+14));
            v_uint8x16 r15 = v_ತುಂಬು(src + i + sstep*(j+15));

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

            v_uint32x4 v0  = v_combine_ಕೆಳಗಿನ (u0,  u8);
            v_uint32x4 v1  = v_combine_ಮೇಲಿನ(u0,  u8);
            v_uint32x4 v2  = v_combine_ಕೆಳಗಿನ (u1,  u9);
            v_uint32x4 v3  = v_combine_ಮೇಲಿನ(u1,  u9);
            v_uint32x4 v4  = v_combine_ಕೆಳಗಿನ (u2,  u10);
            v_uint32x4 v5  = v_combine_ಮೇಲಿನ(u2,  u10);
            v_uint32x4 v6  = v_combine_ಕೆಳಗಿನ (u3,  u11);
            v_uint32x4 v7  = v_combine_ಮೇಲಿನ(u3,  u11);
            v_uint32x4 v8  = v_combine_ಕೆಳಗಿನ (u4,  u12);
            v_uint32x4 v9  = v_combine_ಮೇಲಿನ(u4,  u12);
            v_uint32x4 v10 = v_combine_ಕೆಳಗಿನ (u5,  u13);
            v_uint32x4 v11 = v_combine_ಮೇಲಿನ(u5,  u13);
            v_uint32x4 v12 = v_combine_ಕೆಳಗಿನ (u6,  u14);
            v_uint32x4 v13 = v_combine_ಮೇಲಿನ(u6,  u14);
            v_uint32x4 v14 = v_combine_ಕೆಳಗಿನ (u7,  u15);
            v_uint32x4 v15 = v_combine_ಮೇಲಿನ(u7,  u15);

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
        ಗಾಗಿ (; j < n; j++)
            ಗಾಗಿ (ಪೂರ್ಣಾಂಕ k = 0; k < 16; k++)
                dst[dstep*(i+k) + j] = src[i + sstep*j + k];
    }
    ಗಾಗಿ (; i < m; i++)
        ಗಾಗಿ (ಪೂರ್ಣಾಂಕ j = 0; j < n; j++)
            dst[dstep*i + j] = src[i + sstep*j];
}

ಸ್ಥಿರ ಶೂನ್ಯ transpose_16bit_simd(ಸ್ಥಿರಾಂಕ uchar* src, size_t sstep, uchar* dst, size_t dstep, ಗಾತ್ರ_2 sz)
{
    ಸ್ಥಿರಾಂಕ ushort* src16 = reinterpret_cast<ಸ್ಥಿರಾಂಕ ushort*>(src);
    ushort* dst16 = reinterpret_cast<ushort*>(dst);

    ಸ್ಥಿರಾಂಕ size_t sstep_e = sstep / ಗಾತ್ರ(ushort);
    ಸ್ಥಿರಾಂಕ size_t dstep_e = dstep / ಗಾತ್ರ(ushort);

    ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ m = sz.ಅಗಲ, n = sz.ಎತ್ತರ;
    ಪೂರ್ಣಾಂಕ i = 0;

    ಗಾಗಿ (; i <= m - 8; i += 8)
    {
        ಪೂರ್ಣಾಂಕ j = 0;
        ಗಾಗಿ (; j <= n - 8; j += 8)
        {
            v_uint16x8 r0 = v_ತುಂಬು(src16 + i + sstep_e*(j+0));
            v_uint16x8 r1 = v_ತುಂಬು(src16 + i + sstep_e*(j+1));
            v_uint16x8 r2 = v_ತುಂಬು(src16 + i + sstep_e*(j+2));
            v_uint16x8 r3 = v_ತುಂಬು(src16 + i + sstep_e*(j+3));
            v_uint16x8 r4 = v_ತುಂಬು(src16 + i + sstep_e*(j+4));
            v_uint16x8 r5 = v_ತುಂಬು(src16 + i + sstep_e*(j+5));
            v_uint16x8 r6 = v_ತುಂಬು(src16 + i + sstep_e*(j+6));
            v_uint16x8 r7 = v_ತುಂಬು(src16 + i + sstep_e*(j+7));

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
        ಗಾಗಿ (; j < n; j++)
            ಗಾಗಿ (ಪೂರ್ಣಾಂಕ k = 0; k < 8; k++)
                dst16[dstep_e*(i+k) + j] = src16[i + sstep_e*j + k];
    }
    ಗಾಗಿ (; i < m; i++)
        ಗಾಗಿ (ಪೂರ್ಣಾಂಕ j = 0; j < n; j++)
            dst16[dstep_e*i + j] = src16[i + sstep_e*j];
}

ಸ್ಥಿರ ಶೂನ್ಯ transpose_32bit_simd(ಸ್ಥಿರಾಂಕ uchar* src, size_t sstep, uchar* dst, size_t dstep, ಗಾತ್ರ_2 sz)
{
    // Cache-blocked + AVX2-aware path lives in transpose.simd.hpp; pick the
    // best variant available at runtime.
    CV_CPU_DISPATCH(transpose_32bit_ಖಂಡಗಳು_simd, (src, sstep, dst, dstep, sz),
                    CV_CPU_DISPATCH_MODES_ALL);
}

ಸ್ಥಿರ ಶೂನ್ಯ transpose_48bit_simd(ಸ್ಥಿರಾಂಕ uchar* src, size_t sstep, uchar* dst, size_t dstep, ಗಾತ್ರ_2 sz)
{
    ಸ್ಥಿರಾಂಕ ಗಿಡ್ಡ* src16 = reinterpret_cast<ಸ್ಥಿರಾಂಕ ಗಿಡ್ಡ*>(src);
    ಗಿಡ್ಡ* dst16 = reinterpret_cast<ಗಿಡ್ಡ*>(dst);

    ಸ್ಥಿರಾಂಕ size_t sstep_e = sstep / ಗಾತ್ರ(ಗಿಡ್ಡ);
    ಸ್ಥಿರಾಂಕ size_t dstep_e = dstep / ಗಾತ್ರ(ಗಿಡ್ಡ);

    ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ m = sz.ಅಗಲ, n = sz.ಎತ್ತರ;
    ಪೂರ್ಣಾಂಕ i = 0;

    ಗಾಗಿ (; i <= m - 8; i += 8)
    {
        ಪೂರ್ಣಾಂಕ j = 0;
        ಗಾಗಿ (; j <= n - 8; j += 8)
        {
            v_int16x8 C0_0, C1_0, C2_0;
            v_int16x8 C0_1, C1_1, C2_1;
            v_int16x8 C0_2, C1_2, C2_2;
            v_int16x8 C0_3, C1_3, C2_3;
            v_int16x8 C0_4, C1_4, C2_4;
            v_int16x8 C0_5, C1_5, C2_5;
            v_int16x8 C0_6, C1_6, C2_6;
            v_int16x8 C0_7, C1_7, C2_7;

            v_ತುಂಬು_deinterleave(src16 + sstep_e*(j+0) + i*3, C0_0, C1_0, C2_0);
            v_ತುಂಬು_deinterleave(src16 + sstep_e*(j+1) + i*3, C0_1, C1_1, C2_1);
            v_ತುಂಬು_deinterleave(src16 + sstep_e*(j+2) + i*3, C0_2, C1_2, C2_2);
            v_ತುಂಬು_deinterleave(src16 + sstep_e*(j+3) + i*3, C0_3, C1_3, C2_3);
            v_ತುಂಬು_deinterleave(src16 + sstep_e*(j+4) + i*3, C0_4, C1_4, C2_4);
            v_ತುಂಬು_deinterleave(src16 + sstep_e*(j+5) + i*3, C0_5, C1_5, C2_5);
            v_ತುಂಬು_deinterleave(src16 + sstep_e*(j+6) + i*3, C0_6, C1_6, C2_6);
            v_ತುಂಬು_deinterleave(src16 + sstep_e*(j+7) + i*3, C0_7, C1_7, C2_7);

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
        ಗಾಗಿ (; j < n; j++)
            ಗಾಗಿ (ಪೂರ್ಣಾಂಕ k = 0; k < 8; k++)
            {
                dst16[dstep_e*(i+k) + j*3 + 0] = src16[sstep_e*j + (i+k)*3 + 0];
                dst16[dstep_e*(i+k) + j*3 + 1] = src16[sstep_e*j + (i+k)*3 + 1];
                dst16[dstep_e*(i+k) + j*3 + 2] = src16[sstep_e*j + (i+k)*3 + 2];
            }
    }
    ಗಾಗಿ (; i < m; i++)
        ಗಾಗಿ (ಪೂರ್ಣಾಂಕ j = 0; j < n; j++)
        {
            dst16[dstep_e*i + j*3 + 0] = src16[sstep_e*j + i*3 + 0];
            dst16[dstep_e*i + j*3 + 1] = src16[sstep_e*j + i*3 + 1];
            dst16[dstep_e*i + j*3 + 2] = src16[sstep_e*j + i*3 + 2];
        }
}
#ಆದರೆ_ಅಂತ್ಯ

ಮಾದರಿ<ಬಗೆ_ಹೆಸರು T> ಸ್ಥಿರ ಶೂನ್ಯ
transpose_( ಸ್ಥಿರಾಂಕ uchar* src, size_t sstep, uchar* dst, size_t dstep, ಗಾತ್ರ_2 sz )
{
#ಆದರೆ CV_SIMD128
    ಆಯ್ಕೆ (ಗಾತ್ರ(T))
    {
        ಸ್ಥಿತಿ 1: transpose_8bit_simd(src, sstep, dst, dstep, sz);  ಹಿಂತಿರುಗಿ;
        ಸ್ಥಿತಿ 2: transpose_16bit_simd(src, sstep, dst, dstep, sz); ಹಿಂತಿರುಗಿ;
        ಸ್ಥಿತಿ 4: transpose_32bit_simd(src, sstep, dst, dstep, sz); ಹಿಂತಿರುಗಿ;
        ಸ್ಥಿತಿ 6: transpose_48bit_simd(src, sstep, dst, dstep, sz); ಹಿಂತಿರುಗಿ;
        ಪೂರ್ವನಿಯೋಜಿತ: ನಿಲ್ಲಿಸು;
    }
#ಆದರೆ_ಅಂತ್ಯ

    ಪೂರ್ಣಾಂಕ i = 0, j, m = sz.ಅಗಲ, n = sz.ಎತ್ತರ;

    #ಆದರೆ CV_ENABLE_UNROLLED
    ಗಾಗಿ(; i <= m - 4; i += 4 )
    {
        T* d0 = (T*)(dst + dstep*i);
        T* d1 = (T*)(dst + dstep*(i+1));
        T* d2 = (T*)(dst + dstep*(i+2));
        T* d3 = (T*)(dst + dstep*(i+3));

        ಗಾಗಿ( j = 0; j <= n - 4; j += 4 )
        {
            ಸ್ಥಿರಾಂಕ T* s0 = (ಸ್ಥಿರಾಂಕ T*)(src + i*ಗಾತ್ರ(T) + sstep*j);
            ಸ್ಥಿರಾಂಕ T* s1 = (ಸ್ಥಿರಾಂಕ T*)(src + i*ಗಾತ್ರ(T) + sstep*(j+1));
            ಸ್ಥಿರಾಂಕ T* s2 = (ಸ್ಥಿರಾಂಕ T*)(src + i*ಗಾತ್ರ(T) + sstep*(j+2));
            ಸ್ಥಿರಾಂಕ T* s3 = (ಸ್ಥಿರಾಂಕ T*)(src + i*ಗಾತ್ರ(T) + sstep*(j+3));

            d0[j] = s0[0]; d0[j+1] = s1[0]; d0[j+2] = s2[0]; d0[j+3] = s3[0];
            d1[j] = s0[1]; d1[j+1] = s1[1]; d1[j+2] = s2[1]; d1[j+3] = s3[1];
            d2[j] = s0[2]; d2[j+1] = s1[2]; d2[j+2] = s2[2]; d2[j+3] = s3[2];
            d3[j] = s0[3]; d3[j+1] = s1[3]; d3[j+2] = s2[3]; d3[j+3] = s3[3];
        }

        ಗಾಗಿ( ; j < n; j++ )
        {
            ಸ್ಥಿರಾಂಕ T* s0 = (ಸ್ಥಿರಾಂಕ T*)(src + i*ಗಾತ್ರ(T) + j*sstep);
            d0[j] = s0[0]; d1[j] = s0[1]; d2[j] = s0[2]; d3[j] = s0[3];
        }
    }
    #ಆದರೆ_ಅಂತ್ಯ
    ಗಾಗಿ( ; i < m; i++ )
    {
        T* d0 = (T*)(dst + dstep*i);
        j = 0;
        #ಆದರೆ CV_ENABLE_UNROLLED
        ಗಾಗಿ(; j <= n - 4; j += 4 )
        {
            ಸ್ಥಿರಾಂಕ T* s0 = (ಸ್ಥಿರಾಂಕ T*)(src + i*ಗಾತ್ರ(T) + sstep*j);
            ಸ್ಥಿರಾಂಕ T* s1 = (ಸ್ಥಿರಾಂಕ T*)(src + i*ಗಾತ್ರ(T) + sstep*(j+1));
            ಸ್ಥಿರಾಂಕ T* s2 = (ಸ್ಥಿರಾಂಕ T*)(src + i*ಗಾತ್ರ(T) + sstep*(j+2));
            ಸ್ಥಿರಾಂಕ T* s3 = (ಸ್ಥಿರಾಂಕ T*)(src + i*ಗಾತ್ರ(T) + sstep*(j+3));

            d0[j] = s0[0]; d0[j+1] = s1[0]; d0[j+2] = s2[0]; d0[j+3] = s3[0];
        }
        #ಆದರೆ_ಅಂತ್ಯ
        ಗಾಗಿ( ; j < n; j++ )
        {
            ಸ್ಥಿರಾಂಕ T* s0 = (ಸ್ಥಿರಾಂಕ T*)(src + i*ಗಾತ್ರ(T) + j*sstep);
            d0[j] = s0[0];
        }
    }
}

ಮಾದರಿ<ಬಗೆ_ಹೆಸರು T> ಸ್ಥಿರ ಶೂನ್ಯ
transposeI_( uchar* ದತ್ತಾಂಶ, size_t ಹೆಜ್ಜೆ, ಪೂರ್ಣಾಂಕ n )
{
    ಗಾಗಿ( ಪೂರ್ಣಾಂಕ i = 0; i < n; i++ )
    {
        T* ಅಡ್ಡಸಾಲು = (T*)(ದತ್ತಾಂಶ + ಹೆಜ್ಜೆ*i);
        uchar* data1 = ದತ್ತಾಂಶ + i*ಗಾತ್ರ(T);
        ಗಾಗಿ( ಪೂರ್ಣಾಂಕ j = i+1; j < n; j++ )
            std::swap( ಅಡ್ಡಸಾಲು[j], *(T*)(data1 + ಹೆಜ್ಜೆ*j) );
    }
}

ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಶೂನ್ಯ (*TransposeFunc)( ಸ್ಥಿರಾಂಕ uchar* src, size_t sstep, uchar* dst, size_t dstep, ಗಾತ್ರ_2 sz );
ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಶೂನ್ಯ (*TransposeInplaceFunc)( uchar* ದತ್ತಾಂಶ, size_t ಹೆಜ್ಜೆ, ಪೂರ್ಣಾಂಕ n );

#ವ್ಯಾಖ್ಯಾನಿಸು DEF_TRANSPOSE_FUNC(suffix, type) \
ಸ್ಥಿರ ಶೂನ್ಯ transpose_##suffix( ಸ್ಥಿರಾಂಕ uchar* src, size_t sstep, uchar* dst, size_t dstep, ಗಾತ್ರ_2 sz ) \
{ transpose_<type>(src, sstep, dst, dstep, sz); } \
\
ಸ್ಥಿರ ಶೂನ್ಯ transposeI_##suffix( uchar* ದತ್ತಾಂಶ, size_t ಹೆಜ್ಜೆ, ಪೂರ್ಣಾಂಕ n ) \
{ transposeI_<type>(ದತ್ತಾಂಶ, ಹೆಜ್ಜೆ, n); }

DEF_TRANSPOSE_FUNC(8u, uchar)
DEF_TRANSPOSE_FUNC(16u, ushort)
DEF_TRANSPOSE_FUNC(8uC3, Vec3b)
DEF_TRANSPOSE_FUNC(32s, ಪೂರ್ಣಾಂಕ)
DEF_TRANSPOSE_FUNC(16uC3, Vec3s)
DEF_TRANSPOSE_FUNC(32sC2, Vec2i)
DEF_TRANSPOSE_FUNC(32sC3, Vec3i)
DEF_TRANSPOSE_FUNC(32sC4, Vec4i)
DEF_TRANSPOSE_FUNC(32sC6, Vec6i)
DEF_TRANSPOSE_FUNC(32sC8, Vec8i)

ಸ್ಥಿರ TransposeFunc transposeTab[] =
{
    0, transpose_8u, transpose_16u, transpose_8uC3, transpose_32s, 0, transpose_16uC3, 0,
    transpose_32sC2, 0, 0, 0, transpose_32sC3, 0, 0, 0, transpose_32sC4,
    0, 0, 0, 0, 0, 0, 0, transpose_32sC6, 0, 0, 0, 0, 0, 0, 0, transpose_32sC8
};

nd::TransposeFunc nd::getTransposeFunc(size_t esz)
{
    ಹಿಂತಿರುಗಿ esz < ಗಾತ್ರ(transposeTab)/ಗಾತ್ರ(transposeTab[0]) ? transposeTab[esz] : ಶೂನ್ಯಸೂಚಕ;
}

ಸ್ಥಿರ TransposeInplaceFunc transposeInplaceTab[] =
{
    0, transposeI_8u, transposeI_16u, transposeI_8uC3, transposeI_32s, 0, transposeI_16uC3, 0,
    transposeI_32sC2, 0, 0, 0, transposeI_32sC3, 0, 0, 0, transposeI_32sC4,
    0, 0, 0, 0, 0, 0, 0, transposeI_32sC6, 0, 0, 0, 0, 0, 0, 0, transposeI_32sC8
};

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_OPENCL

ಸ್ಥಿರ ತಾರ್ಕಿಕ ocl_transpose( ಒಳಹರಿವು_Array _src, OutputArray _dst )
{
    ಸ್ಥಿರಾಂಕ ocl::Device & dev = ocl::Device::getDefault();
    ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ TILE_DIM = 32, ಖಂಡ_ಅಡ್ಡಸಾಲುಗಳು = 8;
    ಪೂರ್ಣಾಂಕ type = _src.type(), cn = CV_MAT_CN(type), ಆಳ = CV_MAT_ಆಳ(type),
        ಅಡ್ಡಸಾಲುಗಳು_ಪ್ರತಿ_WI = dev.isIntel() ? 4 : 1;

    UMat src = _src.getUMat();
    _dst.create(src.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ, src.ಅಡ್ಡಸಾಲುಗಳು, type);
    UMat dst = _dst.getUMat();

    String kernelName("transpose");
    ತಾರ್ಕಿಕ inplace = dst.u == src.u;

    ಆದರೆ (inplace)
    {
        CV_Assert(dst.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ == dst.ಅಡ್ಡಸಾಲುಗಳು);
        kernelName += "_inplace";
    }
    ಇಲ್ಲದಿದ್ದರೆ
    {
        // check required local memory size
        size_t required_local_ಸ್ಮರಣೆ = (size_t) TILE_DIM*(TILE_DIM+1)*CV_ELEM_ಗಾತ್ರ(type);
        ಆದರೆ (required_local_ಸ್ಮರಣೆ > ocl::Device::getDefault().local_Mem_ಗಾತ್ರ())
            ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;
    }

    String deviceMacro;
    ಆದರೆ (dev.isIntel())
        deviceMacro = " -D INTEL_GPU";
    ಇಲ್ಲದಿದ್ದರೆ
        deviceMacro = "";

    ocl::Kernel k(kernelName.c_str(), ocl::core::transpose_oclsrc,
                  format("-D T=%s -D T1=%s -D cn=%d -D TILE_DIM=%d -D BLOCK_ROWS=%d -D rowsPerWI=%d%s%s",
                         ocl::memop_Type_ಗಮ್ಯ_Str(type), ocl::memop_Type_ಗಮ್ಯ_Str(ಆಳ),
                         cn, TILE_DIM, ಖಂಡ_ಅಡ್ಡಸಾಲುಗಳು, ಅಡ್ಡಸಾಲುಗಳು_ಪ್ರತಿ_WI, inplace ? " -D INPLACE" : "", deviceMacro.c_str()));
    ಆದರೆ (k.empty())
        ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;

    ಆದರೆ (inplace)
        k.ವಾದಗಳು(ocl::KernelArg::Read_Write_No_ಗಾತ್ರ(dst), dst.ಅಡ್ಡಸಾಲುಗಳು);
    ಇಲ್ಲದಿದ್ದರೆ
        k.ವಾದಗಳು(ocl::KernelArg::ReadOnly(src),
               ocl::KernelArg::Write_Only_No_ಗಾತ್ರ(dst));

    size_t localsize[2]  = { TILE_DIM, ಖಂಡ_ಅಡ್ಡಸಾಲುಗಳು };
    size_t globalsize[2] = { (size_t)src.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ, inplace ? ((size_t)src.ಅಡ್ಡಸಾಲುಗಳು + ಅಡ್ಡಸಾಲುಗಳು_ಪ್ರತಿ_WI - 1) / ಅಡ್ಡಸಾಲುಗಳು_ಪ್ರತಿ_WI : (div_ಮೇಲೆ((size_t)src.ಅಡ್ಡಸಾಲುಗಳು, TILE_DIM) * ಖಂಡ_ಅಡ್ಡಸಾಲುಗಳು) };

    ಆದರೆ (inplace && dev.isIntel())
    {
        localsize[0] = 16;
        localsize[1] = dev.max_Work_ಗುಂಪು_ಗಾತ್ರ() / localsize[0];
    }

    ಹಿಂತಿರುಗಿ k.ಸತತ(2, globalsize, localsize, ಸುಳ್ಳು);
}

#ಆದರೆ_ಅಂತ್ಯ

ಶೂನ್ಯ transpose( ಒಳಹರಿವು_Array _src, OutputArray _dst )
{
    CV_INSTRUMENT_REGION();

    ಪೂರ್ಣಾಂಕ type = _src.type(), esz = CV_ELEM_ಗಾತ್ರ(type);
    CV_Assert( _src.dims() <= 2 && esz <= 32 );

    CV_OCL_ಸತತ(_dst.isUMat(),
               ocl_transpose(_src, _dst))

    Mat src = _src.getMat();
    ಆದರೆ( src.empty() )
    {
        _dst.release();
        ಹಿಂತಿರುಗಿ;
    }

    _dst.create(src.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ, src.ಅಡ್ಡಸಾಲುಗಳು, src.type());
    Mat dst = _dst.getMat();

    // handle the case of single-column/single-row matrices, stored in STL vectors.
    ಆದರೆ( src.ಅಡ್ಡಸಾಲುಗಳು != dst.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ || src.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ != dst.ಅಡ್ಡಸಾಲುಗಳು )
    {
        CV_Assert( src.size() == dst.size() && (src.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ == 1 || src.ಅಡ್ಡಸಾಲುಗಳು == 1) );
        src.copy_ಗಮ್ಯ(dst);
        ಹಿಂತಿರುಗಿ;
    }

    CALL_HAL(transpose2d, cv_hal_transpose2d, src.ದತ್ತಾಂಶ, src.ಹೆಜ್ಜೆ, dst.ದತ್ತಾಂಶ, dst.ಹೆಜ್ಜೆ, src.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ, src.ಅಡ್ಡಸಾಲುಗಳು, esz);

    ಆದರೆ( dst.ದತ್ತಾಂಶ == src.ದತ್ತಾಂಶ )
    {
        TransposeInplaceFunc func = transposeInplaceTab[esz];
        CV_Assert( func != 0 );
        CV_Assert( dst.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ == dst.ಅಡ್ಡಸಾಲುಗಳು );
        func( dst.ptr(), dst.ಹೆಜ್ಜೆ, dst.ಅಡ್ಡಸಾಲುಗಳು );
    }
    ಇಲ್ಲದಿದ್ದರೆ
    {
        TransposeFunc func = transposeTab[esz];
        CV_Assert( func != 0 );
        func( src.ptr(), src.ಹೆಜ್ಜೆ, dst.ptr(), dst.ಹೆಜ್ಜೆ, src.size() );
    }
}


ಶೂನ್ಯ transpose(ಒಳಹರಿವು_Array src_, ಸ್ಥಿರಾಂಕ std::vector<ಪೂರ್ಣಾಂಕ>& ಕ್ರಮ, OutputArray dst_)
{
    CV_INSTRUMENT_REGION();

    Mat inp = src_.getMat();
    ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ ndims = inp.dims;
    CV_Assert(!(ndims == 0 && inp.empty()));
    CV_CheckEQ(ಕ್ರಮ.size(), static_cast<size_t>(ndims), "Number of dimensions shouldn't change");

    ತಾರ್ಕಿಕ ಬಳಸಿದ[CV_MAX_DIM] = {ಸುಳ್ಳು};
    ಪೂರ್ಣಾಂಕ newShape[CV_MAX_DIM];
    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < ndims; i++)
    {
        ಪೂರ್ಣಾಂಕ j = ಕ್ರಮ[i];
        CV_Check(j, 0 <= j && j < ndims && !ಬಳಸಿದ[j], "New order should be a valid permutation of the old one");
        ಬಳಸಿದ[j] = ನಿಜ;
        newShape[i] = inp.size[j];
    }

    // plain 2D transpose keeps using cv::transpose (HAL, in-place square case);
    // not for an empty input, which cv::transpose would answer by releasing dst
    ಆದರೆ (ndims == 2 && ಕ್ರಮ[0] == 1 && !inp.empty() && nd::getTransposeFunc(inp.elem_ಗಾತ್ರ()))
    {
        transpose(inp, dst_);
        ಹಿಂತಿರುಗಿ;
    }

    dst_.create(ndims, newShape, inp.type());
    Mat out = dst_.getMat();
    ಆದರೆ (out.ಒಟ್ಟು() == 0)
        ಹಿಂತಿರುಗಿ;
    nd::copy(nd::ಕ್ರಮಬದಲಾವಣೆ(nd::view_ದ(inp), ಕ್ರಮ.ದತ್ತಾಂಶ()), nd::view_ದ(out));
}

ಶೂನ್ಯ transposeND(ಒಳಹರಿವು_Array src, ಸ್ಥಿರಾಂಕ std::vector<ಪೂರ್ಣಾಂಕ>& ಕ್ರಮ, OutputArray dst)
{
    transpose(src, ಕ್ರಮ, dst);
}


// Generic scalar fallback
CV_ALWAYS_INLINE ಶೂನ್ಯ flipHoriz_generic( ಸ್ಥಿರಾಂಕ uchar* src, size_t sstep, uchar* dst, size_t dstep, ಗಾತ್ರ_2 size, size_t esz )
{
    ಪೂರ್ಣಾಂಕ i, j, ಎಲ್ಲೆ = (ಪೂರ್ಣಾಂಕ)(((size.ಅಗಲ + 1)/2)*esz);
    ಪೂರ್ಣಾಂಕ ಎತ್ತರ = size.ಎತ್ತರ;
    Auto_ಬಫರ್<ಪೂರ್ಣಾಂಕ> _tab(size.ಅಗಲ*esz);
    ಪೂರ್ಣಾಂಕ* tab = _tab.ದತ್ತಾಂಶ();

    ಗಾಗಿ( i = 0; i < size.ಅಗಲ; i++ )
        ಗಾಗಿ( size_t k = 0; k < esz; k++ )
            tab[i*esz + k] = (ಪೂರ್ಣಾಂಕ)((size.ಅಗಲ - i - 1)*esz + k);

    ಗಾಗಿ( ; ಎತ್ತರ--; src += sstep, dst += dstep )
    {
        ಗಾಗಿ( i = 0; i < ಎಲ್ಲೆ; i++ )
        {
            j = tab[i];
            uchar t0 = src[i], t1 = src[j];
            dst[i] = t1; dst[j] = t0;
        }
    }
}

#ಆದರೆ CV_SIMD || CV_SIMD_SCALABLE
ಮಾದರಿ<ಬಗೆ_ಹೆಸರು V> CV_ALWAYS_INLINE ಶೂನ್ಯ flipHoriz_single( ಸ್ಥಿರಾಂಕ uchar* src, size_t sstep, uchar* dst, size_t dstep, ಗಾತ್ರ_2 size )
{
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಬಗೆ_ಹೆಸರು VTraits<V>::lane_type T;
    ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ vlanes = VTraits<v_uint8>::vlanes();
    ಪೂರ್ಣಾಂಕ end = (ಪೂರ್ಣಾಂಕ)(size.ಅಗಲ * ಗಾತ್ರ(T));
    ಪೂರ್ಣಾಂಕ ಅಗಲ = (end + 1) / 2;
    ಪೂರ್ಣಾಂಕ ಅಗಲ_simd = ಅಗಲ & -vlanes;
    ಪೂರ್ಣಾಂಕ ಎತ್ತರ = size.ಎತ್ತರ;

    CV_DbgAssert(!CV_STRONG_ALIGNMENT || isAligned<ಗಾತ್ರ(T)>(src, dst));

    ಗಾಗಿ( ; ಎತ್ತರ--; src += sstep, dst += dstep )
    {
        ಪೂರ್ಣಾಂಕ i = 0, j = end;
        ಗಾಗಿ( ; i < ಅಗಲ_simd; i += vlanes, j -= vlanes )
        {
            V t0 = vx_ತುಂಬು((ಸ್ಥಿರಾಂಕ T*)(src + i));
            V t1 = vx_ತುಂಬು((ಸ್ಥಿರಾಂಕ T*)(src + j - vlanes));
            t0 = v_ಹಿಮ್ಮುಖ(t0);
            t1 = v_ಹಿಮ್ಮುಖ(t1);
            v_store((T*)(dst + j - vlanes), t0);
            v_store((T*)(dst + i), t1);
        }

        // Scalar tail loop
        ಆದರೆ (isAligned<ಗಾತ್ರ(T)>(src, dst))
        {
            ಗಾಗಿ ( ; i < ಅಗಲ; i += ಗಾತ್ರ(T), j -= ಗಾತ್ರ(T) )
            {
                T t0 = *((ಸ್ಥಿರಾಂಕ T*)(src + i));
                T t1 = *((ಸ್ಥಿರಾಂಕ T*)(src + j - ಗಾತ್ರ(T)));
                *((T*)(dst + j - ಗಾತ್ರ(T))) = t0;
                *((T*)(dst + i)) = t1;
            }
        }
        ಇಲ್ಲದಿದ್ದರೆ
        {
            ಗಾಗಿ ( ; i < ಅಗಲ; i += ಗಾತ್ರ(T), j -= ಗಾತ್ರ(T) )
            {
                ಗಾಗಿ (ಪೂರ್ಣಾಂಕ k = 0; k < (ಪೂರ್ಣಾಂಕ)ಗಾತ್ರ(T); k++)
                {
                    uchar t0 = src[i + k];
                    uchar t1 = src[j + k - ಗಾತ್ರ(T)];
                    dst[j + k - ಗಾತ್ರ(T)] = t0;
                    dst[i + k] = t1;
                }
            }
        }
    }
}

// SIMD for C3
ಮಾದರಿ<ಬಗೆ_ಹೆಸರು V>
CV_ALWAYS_INLINE ಶೂನ್ಯ flipHoriz_c3( ಸ್ಥಿರಾಂಕ uchar* src, size_t sstep, uchar* dst, size_t dstep, ಗಾತ್ರ_2 size )
{
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಬಗೆ_ಹೆಸರು VTraits<V>::lane_type T;
    ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ vlanes = VTraits<V>::vlanes();
    ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ stride = 3 * ಗಾತ್ರ(T);
    ಪೂರ್ಣಾಂಕ ಅಗಲ = size.ಅಗಲ;
    ಪೂರ್ಣಾಂಕ centre = (ಅಗಲ + 1) / 2;
    ಪೂರ್ಣಾಂಕ ಅಗಲ_simd = (centre / vlanes) * vlanes;
    ಪೂರ್ಣಾಂಕ ಎತ್ತರ = size.ಎತ್ತರ;

    ಗಾಗಿ( ; ಎತ್ತರ--; src += sstep, dst += dstep )
    {
        ಪೂರ್ಣಾಂಕ i = 0;
        ಗಾಗಿ( ; i < ಅಗಲ_simd; i += vlanes )
        {
            V r0, g0, b0;
            v_ತುಂಬು_deinterleave((ಸ್ಥಿರಾಂಕ T*)(src + i * stride), r0, g0, b0);
            V r1, g1, b1;
            v_ತುಂಬು_deinterleave((ಸ್ಥಿರಾಂಕ T*)(src + (ಅಗಲ - i - vlanes) * stride), r1, g1, b1);
            r0 = v_ಹಿಮ್ಮುಖ(r0);
            g0 = v_ಹಿಮ್ಮುಖ(g0);
            b0 = v_ಹಿಮ್ಮುಖ(b0);
            r1 = v_ಹಿಮ್ಮುಖ(r1);
            g1 = v_ಹಿಮ್ಮುಖ(g1);
            b1 = v_ಹಿಮ್ಮುಖ(b1);
            v_store_interleave((T*)(dst + (ಅಗಲ - i - vlanes) * stride), r0, g0, b0);
            v_store_interleave((T*)(dst + i * stride), r1, g1, b1);
        }
        // Scalar tail loop for remaining pixels
        ಗಾಗಿ( ; i < centre; i++ )
        {
            ಪೂರ್ಣಾಂಕ j = ಅಗಲ - i - 1;
            T c0 = ((ಸ್ಥಿರಾಂಕ T*)(src + i * stride))[0];
            T c1 = ((ಸ್ಥಿರಾಂಕ T*)(src + i * stride))[1];
            T c2 = ((ಸ್ಥಿರಾಂಕ T*)(src + i * stride))[2];
            T c3 = ((ಸ್ಥಿರಾಂಕ T*)(src + j * stride))[0];
            T c4 = ((ಸ್ಥಿರಾಂಕ T*)(src + j * stride))[1];
            T c5 = ((ಸ್ಥಿರಾಂಕ T*)(src + j * stride))[2];
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
ಮಾದರಿ<size_t ESZ>
CV_ALWAYS_INLINE ಶೂನ್ಯ flip_Horiz_vlanes_ಹೊಂದಿಕೆ( ಸ್ಥಿರಾಂಕ uchar* src, size_t sstep, uchar* dst, size_t dstep, ಗಾತ್ರ_2 size)
{
    ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ vlanes = VTraits<v_uint8>::vlanes();
    ಪೂರ್ಣಾಂಕ end = (ಪೂರ್ಣಾಂಕ)(size.ಅಗಲ * ESZ);
    ಪೂರ್ಣಾಂಕ ಅಗಲ = end / 2;
    ಪೂರ್ಣಾಂಕ ಎತ್ತರ = size.ಎತ್ತರ;
    ಪೂರ್ಣಾಂಕ e_ಗಾತ್ರ = (ಪೂರ್ಣಾಂಕ)ESZ;
    ಗಾಗಿ( ; ಎತ್ತರ--; src += sstep, dst += dstep )
    {
        ಗಾಗಿ( ಪೂರ್ಣಾಂಕ i = 0, j = end - e_ಗಾತ್ರ; i < ಅಗಲ; i += e_ಗಾತ್ರ, j -= e_ಗಾತ್ರ )
        {
            ಗಾಗಿ( ಪೂರ್ಣಾಂಕ k = 0; k < e_ಗಾತ್ರ; k += vlanes )
            {
                v_uint8 t0 = vx_ತುಂಬು(src + i + k);
                v_uint8 t1 = vx_ತುಂಬು(src + j + k);
                v_store(dst + j + k, t0);
                v_store(dst + i + k, t1);
            }
        }
    }
}

#ಆದರೆ CV_SIMD128
// SIMD flip when ESZ=16 (128-bit)
ಮಾದರಿ<size_t ESZ>
CV_ALWAYS_INLINE ಶೂನ್ಯ flip_Horiz_vlanes_ಹೊಂದಿಕೆ_128( ಸ್ಥಿರಾಂಕ uchar* src, size_t sstep, uchar* dst, size_t dstep, ಗಾತ್ರ_2 size)
{
    ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ vlanes16 = VTraits<v_uint8x16>::vlanes();
    ಪೂರ್ಣಾಂಕ end = (ಪೂರ್ಣಾಂಕ)(size.ಅಗಲ * ESZ);
    ಪೂರ್ಣಾಂಕ ಅಗಲ = end / 2;
    ಪೂರ್ಣಾಂಕ ಎತ್ತರ = size.ಎತ್ತರ;
    ಗಾಗಿ( ; ಎತ್ತರ--; src += sstep, dst += dstep )
    {
        ಗಾಗಿ( ಪೂರ್ಣಾಂಕ i = 0, j = end - vlanes16; i < ಅಗಲ; i += vlanes16, j -= vlanes16 )
        {
            v_uint8x16 t0 = v_ತುಂಬು(src + i);
            v_uint8x16 t1 = v_ತುಂಬು(src + j);
            v_store(dst + j, t0);
            v_store(dst + i, t1);
        }
    }
}
#ಆದರೆ_ಅಂತ್ಯ // CV_SIMD128

// SIMD flip for ESZ=16,32
ಮಾದರಿ<size_t ESZ>
CV_ALWAYS_INLINE ಶೂನ್ಯ flipHoriz_vlanes_dispatch( ಸ್ಥಿರಾಂಕ uchar* src, size_t sstep, uchar* dst, size_t dstep, ಗಾತ್ರ_2 size )
{
    ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ vlanes = VTraits<v_uint8>::vlanes();
#ಆದರೆ CV_SIMD128
    ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ vlanes16 = VTraits<v_uint8x16>::vlanes();
#ಆದರೆ_ಅಂತ್ಯ
    ಆದರೆ ( (ESZ == (size_t)vlanes) || (ESZ == 2 * (size_t)vlanes))
    {
        flip_Horiz_vlanes_ಹೊಂದಿಕೆ<ESZ>(src, sstep, dst, dstep, size);
        ಹಿಂತಿರುಗಿ;
    }
#ಆದರೆ CV_SIMD128
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (ESZ == vlanes16)
    {
        flip_Horiz_vlanes_ಹೊಂದಿಕೆ_128<ESZ>(src, sstep, dst, dstep, size);
        ಹಿಂತಿರುಗಿ;
    }
#ಆದರೆ_ಅಂತ್ಯ
    flipHoriz_generic(src, sstep, dst, dstep, size, ESZ);
}

#ಆದರೆ CV_SIMD128
// SIMD flip for ESZ=24 (CV_64FC3)
CV_ALWAYS_INLINE ಶೂನ್ಯ flipHoriz_24( ಸ್ಥಿರಾಂಕ uchar* src, size_t sstep, uchar* dst, size_t dstep, ಗಾತ್ರ_2 size )
{
    CV_DbgAssert(!CV_STRONG_ALIGNMENT || isAligned<8>(((size_t)src)|((size_t)dst)|sstep|dstep));

    ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ lanes16 = 16;
    ಪೂರ್ಣಾಂಕ end = (ಪೂರ್ಣಾಂಕ)(size.ಅಗಲ * 24);
    ಪೂರ್ಣಾಂಕ ಅಗಲ = (end + 1) / 2;
    ಪೂರ್ಣಾಂಕ ಎತ್ತರ = size.ಎತ್ತರ;
    ಗಾಗಿ( ; ಎತ್ತರ--; src += sstep, dst += dstep )
    {
        ಗಾಗಿ ( ಪೂರ್ಣಾಂಕ i = 0, j = end; i < ಅಗಲ; i += lanes16 + 8, j -= lanes16 + 8 )
        {
            v_uint8x16 t0 = v_ತುಂಬು(src + i);
            uint64_t t2 = *reinterpret_cast<ಸ್ಥಿರಾಂಕ uint64_t*>(src + i + lanes16);
            v_uint8x16 t1 = v_ತುಂಬು(src + j - lanes16 - 8);
            uint64_t t3 = *reinterpret_cast<ಸ್ಥಿರಾಂಕ uint64_t*>(src + j - 8);
            v_store(dst + j - lanes16 - 8, t0);
            *reinterpret_cast<uint64_t*>(dst + j - 8) = t2;
            v_store(dst + i, t1);
            *reinterpret_cast<uint64_t*>(dst + i + lanes16) = t3;
        }
    }
}
#ಆದರೆ_ಅಂತ್ಯ // CV_SIMD128
#ಆದರೆ_ಅಂತ್ಯ // CV_SIMD || CV_SIMD_SCALABLE

ಸ್ಥಿರ ಶೂನ್ಯ flipHoriz( ಸ್ಥಿರಾಂಕ uchar* src, size_t sstep, uchar* dst, size_t dstep, ಗಾತ್ರ_2 size, size_t esz )
{
#ಆದರೆ CV_SIMD || CV_SIMD_SCALABLE
#ಆದರೆ CV_STRONG_ALIGNMENT
    ಸ್ಥಿರಾಂಕ size_t alignMark = ((size_t)src)|((size_t)dst)|sstep|dstep;
#ಇಲ್ಲದಿದ್ದರೆ
    ಸ್ಥಿರಾಂಕ size_t alignMark = 0; // unaligned access is allowed: all the checks below fold to true
#ಆದರೆ_ಅಂತ್ಯ

    // SIMD-optimized dispatch
    ಆಯ್ಕೆ(esz)
    {
        ಸ್ಥಿತಿ 1:                                                                               // CV_8UC1: 8-bit, 1 channel
            flipHoriz_single<v_uint8>(src, sstep, dst, dstep, size); ಹಿಂತಿರುಗಿ;
        ಸ್ಥಿತಿ 2:                                                                               // CV_8UC2, CV_16UC1: 8-bit 2-channel or 16-bit 1-channel
            ಆದರೆ (isAligned<2>(alignMark)) { flipHoriz_single<v_uint16>(src, sstep, dst, dstep, size); ಹಿಂತಿರುಗಿ; }
            ನಿಲ್ಲಿಸು;
        ಸ್ಥಿತಿ 3:                                                                               // CV_8UC3: 8-bit, 3 channels
            flipHoriz_c3<v_uint8>(src, sstep, dst, dstep, size); ಹಿಂತಿರುಗಿ;
        ಸ್ಥಿತಿ 4:                                                                               // CV_8UC4, CV_16UC2, CV_32SC1, CV_32FC1: 8-bit 4-channel, 16-bit 2-channel, or 32-bit 1-channel
            ಆದರೆ (isAligned<4>(alignMark)) { flipHoriz_single<v_uint32>(src, sstep, dst, dstep, size); ಹಿಂತಿರುಗಿ; }
            ನಿಲ್ಲಿಸು;
        ಸ್ಥಿತಿ 6:                                                                               // CV_16UC3, CV_16SC3: 16-bit, 3 channels
            ಆದರೆ (isAligned<2>(alignMark)) { flipHoriz_c3<v_uint16>(src, sstep, dst, dstep, size); ಹಿಂತಿರುಗಿ; }
            ನಿಲ್ಲಿಸು;
        ಸ್ಥಿತಿ 8:                                                                               // CV_16UC4, CV_32SC2, CV_32FC2, CV_64FC1: 16-bit 4-channel, 32-bit 2-channel, or 64-bit 1-channel
            ಆದರೆ (isAligned<8>(alignMark)) { flipHoriz_single<v_uint64>(src, sstep, dst, dstep, size); ಹಿಂತಿರುಗಿ; }
            ನಿಲ್ಲಿಸು;
        ಸ್ಥಿತಿ 12:                                                                              // CV_32SC3, CV_32FC3: 32-bit, 3 channels
            ಆದರೆ (isAligned<4>(alignMark)) { flipHoriz_c3<v_uint32>(src, sstep, dst, dstep, size); ಹಿಂತಿರುಗಿ; }
            ನಿಲ್ಲಿಸು;
        ಸ್ಥಿತಿ 16:                                                                              // CV_32SC4, CV_32FC4, CV_64FC2: 32-bit 4-channel or 64-bit 2-channel
            flipHoriz_vlanes_dispatch<16>(src, sstep, dst, dstep, size); ಹಿಂತಿರುಗಿ;               // byte-wise access only
#ಆದರೆ CV_SIMD128
        ಸ್ಥಿತಿ 24:                                                                              // CV_64FC3: 64-bit, 3 channels
            ಆದರೆ (isAligned<8>(alignMark)) { flipHoriz_24(src, sstep, dst, dstep, size); ಹಿಂತಿರುಗಿ; }
            ನಿಲ್ಲಿಸು;
#ಆದರೆ_ಅಂತ್ಯ
        ಸ್ಥಿತಿ 32:                                                                              // CV_64FC4: 64-bit, 4 channels
            flipHoriz_vlanes_dispatch<32>(src, sstep, dst, dstep, size); ಹಿಂತಿರುಗಿ;               // byte-wise access only
        ಪೂರ್ವನಿಯೋಜಿತ:
            ನಿಲ್ಲಿಸು; // Fall through to generic implementation
    }
#ಆದರೆ_ಅಂತ್ಯ
    // Fallback: generic scalar
    flipHoriz_generic(src, sstep, dst, dstep, size, esz);
}

ಸ್ಥಿರ ಶೂನ್ಯ
flipVert( ಸ್ಥಿರಾಂಕ uchar* src0, size_t sstep, uchar* dst0, size_t dstep, ಗಾತ್ರ_2 size, size_t esz )
{
    ಸ್ಥಿರಾಂಕ uchar* src1 = src0 + (size.ಎತ್ತರ - 1)*sstep;
    uchar* dst1 = dst0 + (size.ಎತ್ತರ - 1)*dstep;
    size.ಅಗಲ *= (ಪೂರ್ಣಾಂಕ)esz;

    ಗಾಗಿ( ಪೂರ್ಣಾಂಕ y = 0; y < (size.ಎತ್ತರ + 1)/2; y++, src0 += sstep, src1 -= sstep,
                                                  dst0 += dstep, dst1 -= dstep )
    {
        ಪೂರ್ಣಾಂಕ i = 0;
#ಆದರೆ (CV_SIMD || CV_SIMD_SCALABLE)
#ಆದರೆ CV_STRONG_ALIGNMENT
        ಆದರೆ (isAligned<ಗಾತ್ರ(ಪೂರ್ಣಾಂಕ)>(src0, src1, dst0, dst1))
#ಆದರೆ_ಅಂತ್ಯ
        {
            ಗಾಗಿ (; i <= size.ಅಗಲ - VTraits<v_uint8>::vlanes(); i += VTraits<v_uint8>::vlanes())
            {
                v_int32 t0 = v_reinterpret_as_s32(vx_ತುಂಬು(src0 + i));
                v_int32 t1 = v_reinterpret_as_s32(vx_ತುಂಬು(src1 + i));
                v_store(dst0 + i, v_reinterpret_as_u8(t1));
                v_store(dst1 + i, v_reinterpret_as_u8(t0));
            }
        }
#ಆದರೆ CV_STRONG_ALIGNMENT
        ಇಲ್ಲದಿದ್ದರೆ
        {
            ಗಾಗಿ (; i <= size.ಅಗಲ - VTraits<v_uint8>::vlanes(); i += VTraits<v_uint8>::vlanes())
            {
                v_uint8 t0 = vx_ತುಂಬು(src0 + i);
                v_uint8 t1 = vx_ತುಂಬು(src1 + i);
                v_store(dst0 + i, t1);
                v_store(dst1 + i, t0);
            }
        }
#ಆದರೆ_ಅಂತ್ಯ
#ಆದರೆ_ಅಂತ್ಯ

        ಆದರೆ (isAligned<ಗಾತ್ರ(ಪೂರ್ಣಾಂಕ)>(src0, src1, dst0, dst1))
        {
            ಗಾಗಿ( ; i <= size.ಅಗಲ - 16; i += 16 )
            {
                ಪೂರ್ಣಾಂಕ t0 = ((ಪೂರ್ಣಾಂಕ*)(src0 + i))[0];
                ಪೂರ್ಣಾಂಕ t1 = ((ಪೂರ್ಣಾಂಕ*)(src1 + i))[0];

                ((ಪೂರ್ಣಾಂಕ*)(dst0 + i))[0] = t1;
                ((ಪೂರ್ಣಾಂಕ*)(dst1 + i))[0] = t0;

                t0 = ((ಪೂರ್ಣಾಂಕ*)(src0 + i))[1];
                t1 = ((ಪೂರ್ಣಾಂಕ*)(src1 + i))[1];

                ((ಪೂರ್ಣಾಂಕ*)(dst0 + i))[1] = t1;
                ((ಪೂರ್ಣಾಂಕ*)(dst1 + i))[1] = t0;

                t0 = ((ಪೂರ್ಣಾಂಕ*)(src0 + i))[2];
                t1 = ((ಪೂರ್ಣಾಂಕ*)(src1 + i))[2];

                ((ಪೂರ್ಣಾಂಕ*)(dst0 + i))[2] = t1;
                ((ಪೂರ್ಣಾಂಕ*)(dst1 + i))[2] = t0;

                t0 = ((ಪೂರ್ಣಾಂಕ*)(src0 + i))[3];
                t1 = ((ಪೂರ್ಣಾಂಕ*)(src1 + i))[3];

                ((ಪೂರ್ಣಾಂಕ*)(dst0 + i))[3] = t1;
                ((ಪೂರ್ಣಾಂಕ*)(dst1 + i))[3] = t0;
            }

            ಗಾಗಿ( ; i <= size.ಅಗಲ - 4; i += 4 )
            {
                ಪೂರ್ಣಾಂಕ t0 = ((ಪೂರ್ಣಾಂಕ*)(src0 + i))[0];
                ಪೂರ್ಣಾಂಕ t1 = ((ಪೂರ್ಣಾಂಕ*)(src1 + i))[0];

                ((ಪೂರ್ಣಾಂಕ*)(dst0 + i))[0] = t1;
                ((ಪೂರ್ಣಾಂಕ*)(dst1 + i))[0] = t0;
            }
        }

        ಗಾಗಿ( ; i < size.ಅಗಲ; i++ )
        {
            uchar t0 = src0[i];
            uchar t1 = src1[i];

            dst0[i] = t1;
            dst1[i] = t0;
        }
    }
}

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_OPENCL

ಎಣಿಕೆ { FLIP_ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ = 1 << 0, FLIP_ಅಡ್ಡಸಾಲುಗಳು = 1 << 1, FLIP_BOTH = FLIP_ಅಡ್ಡಸಾಲುಗಳು | FLIP_ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ };

ಸ್ಥಿರ ತಾರ್ಕಿಕ ocl_flip(ಒಳಹರಿವು_Array _src, OutputArray _dst, ಪೂರ್ಣಾಂಕ flip_ಸಂಕೇತ )
{
    CV_Assert(flip_ಸಂಕೇತ >= -1 && flip_ಸಂಕೇತ <= 1);

    ಸ್ಥಿರಾಂಕ ocl::Device & dev = ocl::Device::getDefault();
    ಪೂರ್ಣಾಂಕ type = _src.type(), ಆಳ = CV_MAT_ಆಳ(type), cn = CV_MAT_CN(type),
            flipType, kercn = std::min(ocl::predict_Optimal_ಸದಿಶ_ಅಗಲ(_src, _dst), 4);

    ತಾರ್ಕಿಕ doubleSupport = dev.doubleFPConfig() > 0;
    ಆದರೆ (!doubleSupport && ಆಳ == CV_64F)
        kercn = cn;

    ಆದರೆ (cn > 4)
        ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;

    ಗಾತ್ರ_2 size = _src.size();
    _dst.create(size, type);
    UMat src = _src.getUMat(), dst = _dst.getUMat();
    ತಾರ್ಕಿಕ inplace = (dst.u == src.u);

    String kernelName;
    ಆದರೆ (flip_ಸಂಕೇತ == 0)
        kernelName = "arithm_flip_rows", flipType = FLIP_ಅಡ್ಡಸಾಲುಗಳು;
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (flip_ಸಂಕೇತ > 0)
        kernelName = "arithm_flip_cols", flipType = FLIP_ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ;
    ಇಲ್ಲದಿದ್ದರೆ
        kernelName = "arithm_flip_rows_cols", flipType = FLIP_BOTH;

    ಆದರೆ(inplace)
        kernelName += "_inplace";

    ಪೂರ್ಣಾಂಕ px_ಪ್ರತಿ_WIy = (dev.isIntel() && (dev.type() & ocl::Device::TYPE_GPU)) ? 4 : 1;
    kercn = (cn!=3 || flipType == FLIP_ಅಡ್ಡಸಾಲುಗಳು) ? std::max(kercn, cn) : cn;
    ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ TILE_ಗಾತ್ರ = 32, ಖಂಡ_ಅಡ್ಡಸಾಲುಗಳು = 8;

    ocl::Kernel k(kernelName.c_str(), ocl::core::flip_oclsrc,
                format( "-D T=%s -D T1=%s -D DEPTH=%d -D cn=%d -D PIX_PER_WI_Y=%d -D kercn=%d -D TILE_SIZE=%d -D BLOCK_ROWS=%d%s",
                kercn != cn ? ocl::type_ಗಮ್ಯ_Str(CV_MAKE_TYPE(ಆಳ, kercn)) : ocl::vecop_Type_ಗಮ್ಯ_Str(CV_MAKE_TYPE(ಆಳ, kercn)),
                kercn != cn ? ocl::type_ಗಮ್ಯ_Str(ಆಳ) : ocl::vecop_Type_ಗಮ್ಯ_Str(ಆಳ), ಆಳ, cn, px_ಪ್ರತಿ_WIy, kercn, TILE_ಗಾತ್ರ, ಖಂಡ_ಅಡ್ಡಸಾಲುಗಳು,
                inplace ? " -D INPLACE" : ""));
    ಆದರೆ (k.empty())
        ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;

    ಪೂರ್ಣಾಂಕ ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ = size.ಅಗಲ * cn / kercn, ಅಡ್ಡಸಾಲುಗಳು = size.ಎತ್ತರ;
    ಪೂರ್ಣಾಂಕ work_ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ = flipType == FLIP_ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ ? (ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ + 1) >> 1 : ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ;
    ಪೂರ್ಣಾಂಕ work_ಅಡ್ಡಸಾಲುಗಳು = flipType & FLIP_ಅಡ್ಡಸಾಲುಗಳು ? (ಅಡ್ಡಸಾಲುಗಳು + 1) >> 1 : ಅಡ್ಡಸಾಲುಗಳು;

    ಆದರೆ (inplace)
    {
        k.ವಾದಗಳು(ocl::KernelArg::Read_Write_No_ಗಾತ್ರ(dst), ಅಡ್ಡಸಾಲುಗಳು, ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ);

        ಪೂರ್ಣಾಂಕ gs_ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ, gs_ಅಡ್ಡಸಾಲುಗಳು;
        ಆದರೆ (flipType == FLIP_ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ)
        {
            gs_ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ = work_ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ;
            gs_ಅಡ್ಡಸಾಲುಗಳು = ಅಡ್ಡಸಾಲುಗಳು;
        }
        ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (flipType == FLIP_ಅಡ್ಡಸಾಲುಗಳು)
        {
            gs_ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ = ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ;
            gs_ಅಡ್ಡಸಾಲುಗಳು = work_ಅಡ್ಡಸಾಲುಗಳು;
        }
        ಇಲ್ಲದಿದ್ದರೆ // FLIP_BOTH
        {
            gs_ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ = ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ;
            gs_ಅಡ್ಡಸಾಲುಗಳು = ಅಡ್ಡಸಾಲುಗಳು;
        }

        size_t globalsize[2] = {
            (size_t)div_ಮೇಲೆ(gs_ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ, TILE_ಗಾತ್ರ) * TILE_ಗಾತ್ರ,
            (size_t)div_ಮೇಲೆ(gs_ಅಡ್ಡಸಾಲುಗಳು, TILE_ಗಾತ್ರ) * ಖಂಡ_ಅಡ್ಡಸಾಲುಗಳು
        };
        size_t localsize[2] = { TILE_ಗಾತ್ರ, ಖಂಡ_ಅಡ್ಡಸಾಲುಗಳು };
        ಹಿಂತಿರುಗಿ k.ಸತತ(2, globalsize, localsize, ಸುಳ್ಳು);
    }
    ಇಲ್ಲದಿದ್ದರೆ
    {
        k.ವಾದಗಳು(ocl::KernelArg::Read_Only_No_ಗಾತ್ರ(src),
           ocl::KernelArg::WriteOnly(dst, cn, kercn), work_ಅಡ್ಡಸಾಲುಗಳು, work_ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ);

        size_t max_Work_ಗುಂಪು_ಗಾತ್ರ = dev.max_Work_ಗುಂಪು_ಗಾತ್ರ();
        CV_Assert(max_Work_ಗುಂಪು_ಗಾತ್ರ % 4 == 0);

        size_t globalsize[2] = { (size_t)work_ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ, ((size_t)work_ಅಡ್ಡಸಾಲುಗಳು + px_ಪ್ರತಿ_WIy - 1) / px_ಪ್ರತಿ_WIy };
        size_t localsize[2]  = { max_Work_ಗುಂಪು_ಗಾತ್ರ / 4, 4 };
        ಹಿಂತಿರುಗಿ k.ಸತತ(2, globalsize, (flipType == FLIP_ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ) && !dev.isIntel() ? localsize : ಖಾಲಿ, ಸುಳ್ಳು);
    }
}

#ಆದರೆ_ಅಂತ್ಯ

ಶೂನ್ಯ flip( ಒಳಹರಿವು_Array _src, OutputArray _dst, ಪೂರ್ಣಾಂಕ flip_ಬಹುಲಕ )
{
    CV_INSTRUMENT_REGION();

    CV_Assert( _src.dims() <= 2 );
    ಗಾತ್ರ_2 size = _src.size();

    ಆದರೆ (flip_ಬಹುಲಕ < 0)
    {
        ಆದರೆ (size.ಅಗಲ == 1)
            flip_ಬಹುಲಕ = 0;
        ಆದರೆ (size.ಎತ್ತರ == 1)
            flip_ಬಹುಲಕ = 1;
    }

    ಆದರೆ ((size.ಅಗಲ == 1 && flip_ಬಹುಲಕ > 0) ||
        (size.ಎತ್ತರ == 1 && flip_ಬಹುಲಕ == 0))
    {
        ಹಿಂತಿರುಗಿ _src.copy_ಗಮ್ಯ(_dst);
    }

    CV_OCL_ಸತತ( _dst.isUMat(), ocl_flip(_src, _dst, flip_ಬಹುಲಕ))

    Mat src = _src.getMat();
    ಪೂರ್ಣಾಂಕ type = src.type();
    _dst.create( size, type );
    Mat dst = _dst.getMat();

    CALL_HAL(flip, cv_hal_flip, type, src.ptr(), src.ಹೆಜ್ಜೆ, src.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ, src.ಅಡ್ಡಸಾಲುಗಳು,
             dst.ptr(), dst.ಹೆಜ್ಜೆ, flip_ಬಹುಲಕ);

    size_t esz = CV_ELEM_ಗಾತ್ರ(type);

    ಆದರೆ( flip_ಬಹುಲಕ <= 0 )
        flipVert( src.ptr(), src.ಹೆಜ್ಜೆ, dst.ptr(), dst.ಹೆಜ್ಜೆ, src.size(), esz );
    ಇಲ್ಲದಿದ್ದರೆ
        flipHoriz( src.ptr(), src.ಹೆಜ್ಜೆ, dst.ptr(), dst.ಹೆಜ್ಜೆ, src.size(), esz );

    ಆದರೆ( flip_ಬಹುಲಕ < 0 )
        flipHoriz( dst.ptr(), dst.ಹೆಜ್ಜೆ, dst.ptr(), dst.ಹೆಜ್ಜೆ, dst.size(), esz );
}

// In-place flip of a continuous array: swap the slices along the axis, no temporary copy.
ಸ್ಥಿರ ಶೂನ್ಯ flipNDInplace(Mat& m, ಪೂರ್ಣಾಂಕ axis)
{
    size_t nouter = 1;
    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < axis; i++)
        nouter *= (size_t)m.size[i];
    ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ n = m.size[axis];
    ಸ್ಥಿರಾಂಕ size_t ಹೆಜ್ಜೆ = m.ಹೆಜ್ಜೆ[axis];
    uchar* ದತ್ತಾಂಶ = m.ptr();
    ಗಾಗಿ (size_t i = 0; i < nouter; i++, ದತ್ತಾಂಶ += n*ಹೆಜ್ಜೆ)
        ಗಾಗಿ (ಪೂರ್ಣಾಂಕ j = 0, k = n - 1; j < k; j++, k--)
            std::swap_ranges(ದತ್ತಾಂಶ + j*ಹೆಜ್ಜೆ, ದತ್ತಾಂಶ + (j + 1)*ಹೆಜ್ಜೆ, ದತ್ತಾಂಶ + k*ಹೆಜ್ಜೆ);
}

ಶೂನ್ಯ flipND(ಒಳಹರಿವು_Array _src, OutputArray _dst, ಪೂರ್ಣಾಂಕ _axis)
{
    CV_INSTRUMENT_REGION();

    Mat src = _src.getMat();

    // verify axis
    ಪೂರ್ಣಾಂಕ ndim = src.dims;
    CV_CheckLT(_axis, ndim, "flipND: given axis is out of range");
    CV_CheckGE(_axis, -ndim, "flipND: given axis is out of range");
    ಪೂರ್ಣಾಂಕ axis = (_axis + ndim) % ndim;

    _dst.create(ndim, src.size.p, src.type());
    Mat dst = _dst.getMat();
    ಆದರೆ (dst.ದತ್ತಾಂಶ == src.ದತ್ತಾಂಶ && src.isContinuous() && dst.isContinuous())
    {
        flipNDInplace(dst, axis);
        ಹಿಂತಿರುಗಿ;
    }

    nd::View v = nd::view_ದ(src);
    nd::flip(v, axis);
    nd::copy(v, nd::view_ದ(dst));
}

ಶೂನ್ಯ concat(ಒಳಹರಿವು_Array_ದ_Arrays _src, ಪೂರ್ಣಾಂಕ axis, OutputArray _dst)
{
    CV_INSTRUMENT_REGION();

    std::vector<Mat> src;
    _src.get_Mat_ಸದಿಶ(src);
    CV_Assert(!src.empty());

    ಸ್ಥಿರಾಂಕ Mat& src0 = src[0];
    ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ ndims = src0.dims, type = src0.type(), ninputs = (ಪೂರ್ಣಾಂಕ)src.size();
    CV_CheckGE(ndims, 1, "concat: 0-dimensional arrays are not supported");
    CV_CheckGE(axis, -ndims, "concat: axis is out of range");
    CV_CheckLT(axis, ndims, "concat: axis is out of range");
    ಆದರೆ (axis < 0)
        axis += ndims;

    ಪೂರ್ಣಾಂಕ ಹೊರಹೋಗು_Shape[CV_MAX_DIM];
    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < ndims; i++)
        ಹೊರಹೋಗು_Shape[i] = src0.size[i];
    int64 axis_ಗಾತ್ರ = 0;
    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ k = 0; k < ninputs; k++)
    {
        ಸ್ಥಿರಾಂಕ Mat& m = src[k];
        CV_CheckEQ(m.type(), type, "concat: all the input arrays must have the same type");
        CV_CheckEQ(m.dims, ndims, "concat: all the input arrays must have the same number of dimensions");
        ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < ndims; i++)
            ಆದರೆ (i != axis)
                CV_CheckEQ(m.size[i], ಹೊರಹೋಗು_Shape[i], "concat: the input shapes must match except along the axis");
        axis_ಗಾತ್ರ += m.size[axis];
    }
    CV_CheckLE((size_t)axis_ಗಾತ್ರ, (size_t)ಗರಿಷ್ಠ_ಪೂರ್ಣಾಂಕ, "concat: the output is too big");
    ಹೊರಹೋಗು_Shape[axis] = (ಪೂರ್ಣಾಂಕ)axis_ಗಾತ್ರ;

    _dst.create(ndims, ಹೊರಹೋಗು_Shape, type);
    Mat dst = _dst.getMat();

    Auto_ಬಫರ್<nd::View, 4> sv(ninputs), dv(ninputs);
    ಸ್ಥಿರಾಂಕ nd::View dst0 = nd::view_ದ(dst);
    ಪೂರ್ಣಾಂಕ ofs = 0;
    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ k = 0; k < ninputs; k++)
    {
        ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ sz = src[k].size[axis];
        sv[k] = nd::view_ದ(src[k]);
        dv[k] = dst0;
        nd::ತುಂಡು_2(dv[k], axis, ofs, 1, sz);
        ofs += sz;
    }
    nd::copyBatch(sv.ದತ್ತಾಂಶ(), dv.ದತ್ತಾಂಶ(), ninputs);
}

ಶೂನ್ಯ ವಿಭಜಿಸು(ಒಳಹರಿವು_Array _src, ಪೂರ್ಣಾಂಕ axis, ಸ್ಥಿರಾಂಕ std::vector<ಪೂರ್ಣಾಂಕ>& sizes, Output_Array_ದ_Arrays _dst)
{
    CV_INSTRUMENT_REGION();

    Mat src = _src.getMat();
    ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ ndims = src.dims, type = src.type(), noutputs = (ಪೂರ್ಣಾಂಕ)sizes.size();
    CV_CheckGE(ndims, 1, "split: 0-dimensional arrays are not supported");
    CV_CheckGE(axis, -ndims, "split: axis is out of range");
    CV_CheckLT(axis, ndims, "split: axis is out of range");
    ಆದರೆ (axis < 0)
        axis += ndims;
    CV_Assert(noutputs > 0);

    int64 axis_ಗಾತ್ರ = 0;
    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ k = 0; k < noutputs; k++)
    {
        CV_CheckGE(sizes[k], 0, "split: the sizes must be non-negative");
        axis_ಗಾತ್ರ += sizes[k];
    }
    CV_CheckEQ((size_t)axis_ಗಾತ್ರ, (size_t)src.size[axis], "split: the sizes must sum up to the size of the axis");

    ಪೂರ್ಣಾಂಕ ಹೊರಹೋಗು_Shape[CV_MAX_DIM];
    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < ndims; i++)
        ಹೊರಹೋಗು_Shape[i] = src.size[i];

    _dst.create(noutputs, 1, type, -1, ನಿಜ);
    Auto_ಬಫರ್<nd::View, 4> sv(noutputs), dv(noutputs);
    std::vector<Mat> dst(noutputs);
    ಸ್ಥಿರಾಂಕ nd::View src0 = nd::view_ದ(src);
    ಪೂರ್ಣಾಂಕ ofs = 0;
    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ k = 0; k < noutputs; k++)
    {
        ಹೊರಹೋಗು_Shape[axis] = sizes[k];
        _dst.create(ndims, ಹೊರಹೋಗು_Shape, type, k);
        dst[k] = _dst.getMat(k);
        sv[k] = src0;
        nd::ತುಂಡು_2(sv[k], axis, ofs, 1, sizes[k]);
        dv[k] = nd::view_ದ(dst[k]);
        ofs += sizes[k];
    }
    nd::copyBatch(sv.ದತ್ತಾಂಶ(), dv.ದತ್ತಾಂಶ(), noutputs);
}

ಶೂನ್ಯ repeat(ಒಳಹರಿವು_Array _src, ಸ್ಥಿರಾಂಕ std::vector<ಪೂರ್ಣಾಂಕ>& repeats, OutputArray _dst)
{
    CV_INSTRUMENT_REGION();

    Mat src = _src.getMat();
    ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ ndims = src.dims;
    CV_Assert(!(ndims == 0 && src.empty()));
    CV_CheckEQ(repeats.size(), (size_t)ndims, "repeat: the number of repeats must match the number of dimensions");

    ಪೂರ್ಣಾಂಕ ಹೊರಹೋಗು_Shape[CV_MAX_DIM];
    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < ndims; i++)
    {
        CV_CheckGE(repeats[i], 0, "repeat: the repeats must be non-negative");
        int64 sz = (int64)src.size[i]*repeats[i];
        CV_CheckLE((size_t)sz, (size_t)ಗರಿಷ್ಠ_ಪೂರ್ಣಾಂಕ, "repeat: the output is too big");
        ಹೊರಹೋಗು_Shape[i] = (ಪೂರ್ಣಾಂಕ)sz;
    }

    _dst.create(ndims, ಹೊರಹೋಗು_Shape, src.type());
    Mat dst = _dst.getMat();
    ಆದರೆ (dst.ಒಟ್ಟು() == 0 || src.empty())
        ಹಿಂತಿರುಗಿ;

    // Each repeated output axis is split into (repeat index, position), the former
    // with source step 0, so the whole tile is one broadcasting copy.
    nd::View sv, dv;
    sv.ದತ್ತಾಂಶ = src.ದತ್ತಾಂಶ;
    dv.ದತ್ತಾಂಶ = dst.ದತ್ತಾಂಶ;
    sv.esz = dv.esz = src.elem_ಗಾತ್ರ();
    ಪೂರ್ಣಾಂಕ n = 0;
    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < ndims; i++)
    {
        ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ sz = src.size[i], r = repeats[i];
        ಆದರೆ (r != 1)
        {
            CV_CheckLT(n, (ಪೂರ್ಣಾಂಕ)nd::MAX_VIEW_DIMS, "repeat: too many dimensions");
            sv.size[n] = dv.size[n] = r;
            sv.ಹೆಜ್ಜೆ[n] = 0;
            dv.ಹೆಜ್ಜೆ[n] = (ptrdiff_t)dst.ಹೆಜ್ಜೆ[i]*sz;
            n++;
        }
        ಆದರೆ (sz != 1 || r == 1)
        {
            CV_CheckLT(n, (ಪೂರ್ಣಾಂಕ)nd::MAX_VIEW_DIMS, "repeat: too many dimensions");
            sv.size[n] = dv.size[n] = sz;
            sv.ಹೆಜ್ಜೆ[n] = (ptrdiff_t)src.ಹೆಜ್ಜೆ[i];
            dv.ಹೆಜ್ಜೆ[n] = (ptrdiff_t)dst.ಹೆಜ್ಜೆ[i];
            n++;
        }
    }
    sv.dims = dv.dims = n;
    nd::copy(sv, dv);
}

ಶೂನ್ಯ ತುಂಡು_2(ಒಳಹರಿವು_Array _src, ಸ್ಥಿರಾಂಕ std::vector<ಪೂರ್ಣಾಂಕ>& starts, ಸ್ಥಿರಾಂಕ std::vector<ಪೂರ್ಣಾಂಕ>& ends,
             ಸ್ಥಿರಾಂಕ std::vector<ಪೂರ್ಣಾಂಕ>& ಹೆಜ್ಜೆಗಳು, OutputArray _dst)
{
    CV_INSTRUMENT_REGION();

    Mat src = _src.getMat();
    ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ ndims = src.dims, nslices = (ಪೂರ್ಣಾಂಕ)starts.size();
    CV_Assert(!(ndims == 0 && src.empty()));
    CV_CheckLE(nslices, ndims, "slice: too many starts");
    CV_CheckEQ(ends.size(), starts.size(), "slice: starts and ends must have the same size");
    CV_Check(ಹೆಜ್ಜೆಗಳು.size(), ಹೆಜ್ಜೆಗಳು.empty() || ಹೆಜ್ಜೆಗಳು.size() == starts.size(),
             "slice: steps must be empty or have the same size as starts");

    nd::View sv = nd::view_ದ(src);
    ಪೂರ್ಣಾಂಕ ಹೊರಹೋಗು_Shape[CV_MAX_DIM];
    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < ndims; i++)
        ಹೊರಹೋಗು_Shape[i] = src.size[i];

    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < nslices; i++)
    {
        ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ n = src.size[i], ಆರಂಭ = starts[i], end = ends[i];
        ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ ಹೆಜ್ಜೆ = ಹೆಜ್ಜೆಗಳು.empty() ? 1 : ಹೆಜ್ಜೆಗಳು[i];
        CV_CheckNE(ಹೆಜ್ಜೆ, 0, "slice: step must not be zero");
        ಪೂರ್ಣಾಂಕ ಎಣಿಕೆ_3;
        ಆದರೆ (ಹೆಜ್ಜೆ > 0)
        {
            CV_Check(ಆರಂಭ, 0 <= ಆರಂಭ && ಆರಂಭ <= n, "slice: start is out of range");
            CV_Check(end, 0 <= end && end <= n, "slice: end is out of range");
            ಎಣಿಕೆ_3 = end > ಆರಂಭ ? (ಪೂರ್ಣಾಂಕ)(((int64)end - ಆರಂಭ + ಹೆಜ್ಜೆ - 1)/ಹೆಜ್ಜೆ) : 0;
        }
        ಇಲ್ಲದಿದ್ದರೆ
        {
            CV_Check(ಆರಂಭ, -1 <= ಆರಂಭ && ಆರಂಭ < n, "slice: start is out of range");
            CV_Check(end, -1 <= end && end < n, "slice: end is out of range");
            ಎಣಿಕೆ_3 = ಆರಂಭ > end ? (ಪೂರ್ಣಾಂಕ)(((int64)ಆರಂಭ - end - ಹೆಜ್ಜೆ - 1)/(-ಹೆಜ್ಜೆ)) : 0;
        }
        nd::ತುಂಡು_2(sv, i, ಆರಂಭ, ಹೆಜ್ಜೆ, ಎಣಿಕೆ_3);
        ಹೊರಹೋಗು_Shape[i] = ಎಣಿಕೆ_3;
    }

    _dst.create(ndims, ಹೊರಹೋಗು_Shape, src.type());
    Mat dst = _dst.getMat();
    ಆದರೆ (dst.ಒಟ್ಟು() == 0 || src.empty())
        ಹಿಂತಿರುಗಿ;
    nd::copy(sv, nd::view_ದ(dst));
}

/*
    This function first prepends 1 to each tensor shape to have a common max_ndims dimension, then flatten non-broadcast dimensions.
*/
ಸ್ಥಿರ ತಾರ್ಕಿಕ flatten_for_ಪ್ರಸಾರ(ಪೂರ್ಣಾಂಕ narrays, ಪೂರ್ಣಾಂಕ max_ndims, ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ* ndims, ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ** orig_shape,
                                   ಪೂರ್ಣಾಂಕ** flatten_shape, size_t** flatten_ಹೆಜ್ಜೆ) {
    ಪೂರ್ಣಾಂಕ i, j, k;

    // step 1.
    // * make all inputs and the output max_ndims-dimensional.
    // * compute proper step's
    ಗಾಗಿ (i = max_ndims - 1; i >= 0; i-- ) {
        ಗಾಗಿ (k = 0; k < narrays; k++) {
            j = ndims[k] - (max_ndims - i);
            ಪೂರ್ಣಾಂಕ sz_i = j >= 0 ? orig_shape[k][j] : 1;
            size_t st_i = i == max_ndims - 1 ? 1 : flatten_ಹೆಜ್ಜೆ[k][i+1] * flatten_shape[k][i+1];
            flatten_shape[k][i] = sz_i;
            flatten_ಹೆಜ್ಜೆ[k][i] = st_i;
            ಆದರೆ (flatten_shape[k][i] == 0)
                ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;
        }
    }

    // step 2. Let's do the flattening first,
    // since we'd need proper values of steps to check continuity.
    // this loop is probably the most tricky part
    // in the whole implementation of broadcasting.
    j = max_ndims-1;
    ಗಾಗಿ (i = j - 1; i >= 0; i--) {
        ತಾರ್ಕಿಕ all_contiguous = ನಿಜ, all_scalars = ನಿಜ, all_consistent = ನಿಜ;
        ಗಾಗಿ(k = 0; k < narrays; k++) {
            size_t st = flatten_ಹೆಜ್ಜೆ[k][j] * flatten_shape[k][j];
            ತಾರ್ಕಿಕ prev_scalar = flatten_shape[k][j] == 1;
            ತಾರ್ಕಿಕ scalar = flatten_shape[k][i] == 1;
            all_contiguous = all_contiguous && (st == flatten_ಹೆಜ್ಜೆ[k][i]);
            all_scalars = all_scalars && scalar;
            all_consistent = all_consistent && (scalar == prev_scalar);
        }
        ಆದರೆ (all_contiguous && (all_consistent || all_scalars)) {
            ಗಾಗಿ(k = 0; k < narrays; k++)
                flatten_shape[k][j] *= flatten_shape[k][i];
        } ಇಲ್ಲದಿದ್ದರೆ {
            j--;
            ಆದರೆ (i < j) {
                ಗಾಗಿ(k = 0; k < narrays; k++) {
                    flatten_shape[k][j] = flatten_shape[k][i];
                    flatten_ಹೆಜ್ಜೆ[k][j] = flatten_ಹೆಜ್ಜೆ[k][i];
                }
            }
        }
    }

    // step 3. Set some step's to 0's.
    ಗಾಗಿ (i = max_ndims-1; i >= j; i--) {
        ಗಾಗಿ (k = 0; k < narrays; k++)
            flatten_ಹೆಜ್ಜೆ[k][i] = flatten_shape[k][i] == 1 ? 0 : flatten_ಹೆಜ್ಜೆ[k][i];
    }
    ಗಾಗಿ (; i >= 0; i--) {
        ಗಾಗಿ (k = 0; k < narrays; k++) {
            flatten_ಹೆಜ್ಜೆ[k][i] = 0;
            flatten_shape[k][i] = 1;
        }
    }
    ಹಿಂತಿರುಗಿ ನಿಜ;
}

ಶೂನ್ಯ ಪ್ರಸಾರ(ಒಳಹರಿವು_Array _src, ಒಳಹರಿವು_Array _shape, OutputArray _dst) {
    CV_INSTRUMENT_REGION();

    Mat src = _src.getMat();
    CV_CheckTrue(src.isContinuous(), "broadcast: input array must be contiguous");
    CV_CheckChannelsEQ(src.channels(), 1, "broadcast: input array must be single channel");

    Mat shape = _shape.getMat();
    CV_CheckTypeEQ(shape.type(), CV_32S, "broadcast: target shape must be of type int32");
    ಸ್ಥಿರಾಂಕ ಸ್ವಯಂಚಾಲಿತ dims_shape = static_cast<ಪೂರ್ಣಾಂಕ>(shape.ಒಟ್ಟು());
    ಸ್ಥಿರಾಂಕ ಸ್ವಯಂಚಾಲಿತ *ptr_shape = shape.ptr<ಪೂರ್ಣಾಂಕ>();

    // check valid shape, 1D/0D Mat would fail in the following checks
    ಸ್ಥಿರಾಂಕ ಸ್ವಯಂಚಾಲಿತ dims_src = src.dims;
    CV_CheckLE(dims_src, dims_shape,
               "broadcast: dimension of input array must be less than or equal to dimension of target shape");
    std::vector<ಪೂರ್ಣಾಂಕ> shape_src{src.size.p, src.size.p + dims_src};
    ಆದರೆ (shape_src.size() < static_cast<size_t>(dims_shape)) {
        shape_src.insert(shape_src.begin(), dims_shape - shape_src.size(), 1);
    }
    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < static_cast<ಪೂರ್ಣಾಂಕ>(shape_src.size()); ++i) {
        ಸ್ಥಿರಾಂಕ ಸ್ವಯಂಚಾಲಿತ *shape_ಗುರಿ = ptr_shape;
        ಆದರೆ (shape_src[i] != 1) {
            CV_CheckEQ(shape_src[i], shape_ಗುರಿ[i], "target shape must be equal to input shape or 1");
        }
    }

    // impl
    _dst.create(dims_shape, shape.ptr<ಪೂರ್ಣಾಂಕ>(), src.type());
    Mat dst = _dst.getMat();
    ಆದರೆ (dst.ಒಟ್ಟು() == 0)
        ಹಿಂತಿರುಗಿ;
    cv::Auto_ಬಫರ್<ಪೂರ್ಣಾಂಕ> is_same_shape(dims_shape, 0);
    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < static_cast<ಪೂರ್ಣಾಂಕ>(shape_src.size()); ++i) {
        ಆದರೆ (shape_src[i] == ptr_shape[i]) {
            is_same_shape[i] = 1;
        }
    }
    // copy if same shape
    ಆದರೆ (std::accumulate(is_same_shape.begin(), is_same_shape.end(), 1, std::multiplies<ಪೂರ್ಣಾಂಕ>()) != 0) {
        ಸ್ಥಿರಾಂಕ ಸ್ವಯಂಚಾಲಿತ *p_src = src.ptr<ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ>();
        ಸ್ವಯಂಚಾಲಿತ *p_dst = dst.ptr<ಅಕ್ಷರ>();
        std::ಸ್ಮರಣೆ_ನಕಲು(p_dst, p_src, dst.ಒಟ್ಟು() * dst.elem_ಗಾತ್ರ());
        ಹಿಂತಿರುಗಿ;
    }
    // other cases
    ಪೂರ್ಣಾಂಕ max_ndims = std::max(dims_src, dims_shape);
    ಆದರೆ (max_ndims < 2 && src.ಒಟ್ಟು() == 1) {
        ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ* p_src = src.ptr<ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ>();
        ಅಕ್ಷರ* p_dst = dst.ptr<ಅಕ್ಷರ>();
        size_t esz = src.elem_ಗಾತ್ರ();
        ಗಾಗಿ (size_t j = 0; j < dst.ಒಟ್ಟು(); j++)
            std::ಸ್ಮರಣೆ_ನಕಲು(p_dst + j * esz, p_src, esz);
        ಹಿಂತಿರುಗಿ;
    }
    ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ all_ndims[2] = {src.dims, dst.dims};
    ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ* orig_shapes[2] = {src.size.p, dst.size.p};
    cv::Auto_ಬಫರ್<size_t> buff(max_ndims * 4);
    ಪೂರ್ಣಾಂಕ* flatten_shapes[2] = {(ಪೂರ್ಣಾಂಕ*)buff.ದತ್ತಾಂಶ(), (ಪೂರ್ಣಾಂಕ*)(buff.ದತ್ತಾಂಶ() + max_ndims)};
    size_t* flatten_ಹೆಜ್ಜೆಗಳು[2] = {(size_t*)(buff.ದತ್ತಾಂಶ() + 2 * max_ndims), (size_t*)(buff.ದತ್ತಾಂಶ() + 3 * max_ndims)};
    ಆದರೆ (flatten_for_ಪ್ರಸಾರ(2, max_ndims, all_ndims, orig_shapes, flatten_shapes, flatten_ಹೆಜ್ಜೆಗಳು)) {
        size_t src_dp = flatten_ಹೆಜ್ಜೆಗಳು[0][max_ndims - 1];
        size_t dst_dp = flatten_ಹೆಜ್ಜೆಗಳು[1][max_ndims - 1];
        CV_Assert(dst_dp == 1 || dst_dp == 0);
        CV_Assert(max_ndims >= 2); // >= 3?
        size_t rowstep_src = flatten_ಹೆಜ್ಜೆಗಳು[0][max_ndims - 2];
        size_t rowstep_dst = flatten_ಹೆಜ್ಜೆಗಳು[1][max_ndims - 2];
        ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ* ptr_src = src.ptr<ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ>();
        ಅಕ್ಷರ* ptr_dst = dst.ptr<ಅಕ್ಷರ>();
        size_t esz = src.elem_ಗಾತ್ರ();
        ಪೂರ್ಣಾಂಕ nrows = flatten_shapes[1][max_ndims - 2];
        ಪೂರ್ಣಾಂಕ ncols = flatten_shapes[1][max_ndims - 1];
        ಪೂರ್ಣಾಂಕ nplanes = 1;
        CV_Check(esz, esz == 1 || esz == 2 || esz == 4 || esz == 8, "broadcast: not supported data type");

        ಗಾಗಿ (ಪೂರ್ಣಾಂಕ k = 0; k < max_ndims - 2; k++) {
            nplanes *= flatten_shapes[1][k];
        }
        ಗಾಗಿ (ಪೂರ್ಣಾಂಕ plane_idx = 0; plane_idx < nplanes; plane_idx++) {
            size_t ಸ್ಥಾನಾಂತರ_src = 0, ಸ್ಥಾನಾಂತರ_dst = 0;
            size_t idx = (size_t)plane_idx;
            ಗಾಗಿ (ಪೂರ್ಣಾಂಕ k = max_ndims - 3; k >= 0; k--) {
                size_t prev_idx = idx / flatten_shapes[1][k];
                size_t i_k = (ಪೂರ್ಣಾಂಕ)(idx - prev_idx * flatten_shapes[1][k]);
                ಸ್ಥಾನಾಂತರ_src += i_k * flatten_ಹೆಜ್ಜೆಗಳು[0][k];
                ಸ್ಥಾನಾಂತರ_dst += i_k * flatten_ಹೆಜ್ಜೆಗಳು[1][k];
                idx = prev_idx;
            }

            #ವ್ಯಾಖ್ಯಾನಿಸು OPENCV_CORE_ಪ್ರಸಾರ_LOOP(_Tp) \
                ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < nrows; i++) {   \
                    ಸ್ಥಿರಾಂಕ _Tp *ptr_src_ = (ಸ್ಥಿರಾಂಕ _Tp*)ptr_src + ಸ್ಥಾನಾಂತರ_src + rowstep_src * i; \
                    _Tp *ptr_dst_ = (_Tp*)ptr_dst + ಸ್ಥಾನಾಂತರ_dst + rowstep_dst * i; \
                    ಆದರೆ (src_dp == 1) { \
                        ಗಾಗಿ (ಪೂರ್ಣಾಂಕ j = 0; j < ncols; j++) { \
                            ptr_dst_[j] = ptr_src_[j]; \
                        } \
                    } ಇಲ್ಲದಿದ್ದರೆ { \
                        _Tp x = *ptr_src_; \
                        ಗಾಗಿ (ಪೂರ್ಣಾಂಕ j = 0; j < ncols; j++) { \
                            ptr_dst_[j] = x; \
                        } \
                    } \
                }

            ಆದರೆ (esz == 1) {
                OPENCV_CORE_ಪ್ರಸಾರ_LOOP(int8_t);
            } ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (esz == 2) {
                OPENCV_CORE_ಪ್ರಸಾರ_LOOP(int16_t);
            } ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (esz == 4) {
                OPENCV_CORE_ಪ್ರಸಾರ_LOOP(int32_t);
            } ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (esz == 8) {
                OPENCV_CORE_ಪ್ರಸಾರ_LOOP(int64_t);
            } ಇಲ್ಲದಿದ್ದರೆ {
                CV_Error(cv::Error::StsNotImplemented, "");
            }
            #ವ್ಯಾಖ್ಯಾನ_ರದ್ದು OPENCV_CORE_ಪ್ರಸಾರ_LOOP
        }
    } ಇಲ್ಲದಿದ್ದರೆ {
        ಆದರೆ (dst.ಒಟ್ಟು() == 0)
            ಹಿಂತಿರುಗಿ;
        // initial copy (src to dst)
        std::vector<size_t> ಹೆಜ್ಜೆ_src{src.ಹೆಜ್ಜೆ.p, src.ಹೆಜ್ಜೆ.p + dims_src};
        ಆದರೆ (ಹೆಜ್ಜೆ_src.size() < static_cast<size_t>(dims_shape)) {
            ಹೆಜ್ಜೆ_src.insert(ಹೆಜ್ಜೆ_src.begin(), dims_shape - ಹೆಜ್ಜೆ_src.size(), ಹೆಜ್ಜೆ_src[0]);
        }
        ಗಾಗಿ (size_t i = 0; i < src.ಒಟ್ಟು(); ++i) {
            size_t t = i;
            size_t src_ಸ್ಥಾನಾಂತರ = 0, dst_ಸ್ಥಾನಾಂತರ = 0;
            ಗಾಗಿ (ಪೂರ್ಣಾಂಕ j = static_cast<ಪೂರ್ಣಾಂಕ>(shape_src.size() - 1); j >= 0; --j) {
                size_t idx = t / shape_src[j];
                size_t offset = static_cast<size_t>(t - idx * shape_src[j]);
                src_ಸ್ಥಾನಾಂತರ += offset * ಹೆಜ್ಜೆ_src[j];
                dst_ಸ್ಥಾನಾಂತರ += offset * dst.ಹೆಜ್ಜೆ[j];
                t = idx;
            }
            ಸ್ಥಿರಾಂಕ ಸ್ವಯಂಚಾಲಿತ *p_src = src.ptr<ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ>();
            ಸ್ವಯಂಚಾಲಿತ *p_dst = dst.ptr<ಅಕ್ಷರ>();
            std::ಸ್ಮರಣೆ_ನಕಲು(p_dst + dst_ಸ್ಥಾನಾಂತರ, p_src + src_ಸ್ಥಾನಾಂತರ, dst.elem_ಗಾತ್ರ());
        }
        // broadcast copy (dst inplace)
        Auto_ಬಫರ್<ಪೂರ್ಣಾಂಕ> cumulative_shape(dims_shape, 1);
        ಪೂರ್ಣಾಂಕ ಒಟ್ಟು = static_cast<ಪೂರ್ಣಾಂಕ>(dst.ಒಟ್ಟು());
        ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = dims_shape - 1; i >= 0; --i) {
            cumulative_shape[i] = static_cast<ಪೂರ್ಣಾಂಕ>(ಒಟ್ಟು / ptr_shape[i]);
            ಒಟ್ಟು = cumulative_shape[i];
        }
        ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = dims_shape - 1; i >= 0; --i) {
            ಆದರೆ (is_same_shape[i] == 1) {
                ಮುಂದುವರಿ;
            }
            ಸ್ವಯಂಚಾಲಿತ ಹೆಜ್ಜೆ = dst.ಹೆಜ್ಜೆ[i];
            ಸ್ವಯಂಚಾಲಿತ *p_dst = dst.ptr<ಅಕ್ಷರ>();
            ಗಾಗಿ (ಪೂರ್ಣಾಂಕ j = 0; j < cumulative_shape[i]; j++) {
                ಗಾಗಿ (ಪೂರ್ಣಾಂಕ k = 0; k < ptr_shape[i] - 1; k++) {
                    std::ಸ್ಮರಣೆ_ನಕಲು(p_dst + ಹೆಜ್ಜೆ, p_dst, ಹೆಜ್ಜೆ);
                    p_dst += ಹೆಜ್ಜೆ;
                }
                p_dst += ಹೆಜ್ಜೆ;
            }
        }
    }
}

ಶೂನ್ಯ ಪ್ರಸಾರ(ಒಳಹರಿವು_Array _src, ಸ್ಥಿರಾಂಕ MatShape& _shape, OutputArray _dst)
{
    ಆದರೆ (_shape.dims < 0) {
        _dst.release();
    } ಇಲ್ಲದಿದ್ದರೆ {
        Mat shape(1, _shape.dims, CV_32S, (ಪೂರ್ಣಾಂಕ*)_shape.p);
        ಪ್ರಸಾರ(_src, shape, _dst);
    }
}

ಸ್ಥಿರ ಶೂನ್ಯ rotateImpl(ಒಳಹರಿವು_Array _src, OutputArray _dst, ಪೂರ್ಣಾಂಕ rotate_ಬಹುಲಕ)
{
    ಆಯ್ಕೆ (rotate_ಬಹುಲಕ)
    {
    ಸ್ಥಿತಿ ROTATE_90_CLOCKWISE:
        transpose(_src, _dst);
        flip(_dst, _dst, 1);
        ನಿಲ್ಲಿಸು;
    ಸ್ಥಿತಿ ROTATE_180:
        flip(_src, _dst, -1);
        ನಿಲ್ಲಿಸು;
    ಸ್ಥಿತಿ ROTATE_90_COUNTERCLOCKWISE:
        transpose(_src, _dst);
        flip(_dst, _dst, 0);
        ನಿಲ್ಲಿಸು;
    ಪೂರ್ವನಿಯೋಜಿತ:
        ನಿಲ್ಲಿಸು;
    }
}

ಶೂನ್ಯ rotate(ಒಳಹರಿವು_Array _src, OutputArray _dst, ಪೂರ್ಣಾಂಕ rotate_ಬಹುಲಕ)
{
    CV_Assert(_src.dims() <= 2);
    ಪೂರ್ಣಾಂಕ angle;

    ಆದರೆ (_dst.isUMat())
    {
        rotateImpl(_src, _dst, rotate_ಬಹುಲಕ);
        ಹಿಂತಿರುಗಿ;
    }

    Mat src = _src.getMat();
    ಪೂರ್ಣಾಂಕ type = src.type();
    ಆದರೆ( src.empty() )
    {
        _dst.release();
        ಹಿಂತಿರುಗಿ;
    }

    ಆಯ್ಕೆ (rotate_ಬಹುಲಕ)
    {
    ಸ್ಥಿತಿ ROTATE_90_CLOCKWISE:
        _dst.create(src.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ, src.ಅಡ್ಡಸಾಲುಗಳು, type);
        angle = 90;
        ನಿಲ್ಲಿಸು;
    ಸ್ಥಿತಿ ROTATE_180:
        _dst.create(src.ಅಡ್ಡಸಾಲುಗಳು, src.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ, type);
        angle = 180;
        ನಿಲ್ಲಿಸು;
    ಸ್ಥಿತಿ ROTATE_90_COUNTERCLOCKWISE:
        _dst.create(src.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ, src.ಅಡ್ಡಸಾಲುಗಳು, type);
        angle = 270;
        ನಿಲ್ಲಿಸು;
    ಪೂರ್ವನಿಯೋಜಿತ:
        _dst.create(src.ಅಡ್ಡಸಾಲುಗಳು, src.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ, type);
        angle = 0;
        ನಿಲ್ಲಿಸು;
    }

    Mat dst = _dst.getMat();
    CALL_HAL(rotate90, cv_hal_rotate90, type, src.ptr(), src.ಹೆಜ್ಜೆ, src.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ, src.ಅಡ್ಡಸಾಲುಗಳು,
             dst.ptr(), dst.ಹೆಜ್ಜೆ, angle);

    // use src (Mat) since _src (InputArray) is updated by _dst.create() when in-place
    rotateImpl(src, _dst, rotate_ಬಹುಲಕ);
}

}  // namespace
