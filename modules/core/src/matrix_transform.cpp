// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html
// Copyright (C) 2026, Advanced Micro Devices, Inc., all rights reserved.
#ਅਨ੍ਤਰ੍ਭਾਵ "precomp.hpp"
#ਅਨ੍ਤਰ੍ਭਾਵ "opencl_kernels_core.hpp"
#ਅਨ੍ਤਰ੍ਭਾਵ "hal_replacement.hpp"
#ਅਨ੍ਤਰ੍ਭਾਵ "opencv2/core/detail/dispatch_helper.impl.hpp"

#ਅਨ੍ਤਰ੍ਭਾਵ "transpose.simd.hpp"
#ਅਨ੍ਤਰ੍ਭਾਵ "transpose.simd_declarations.hpp"
#ਅਨ੍ਤਰ੍ਭਾਵ "nd_copy.hpp"

#ਅਨ੍ਤਰ੍ਭਾਵ <algorithm> // std::swap_ranges
#ਅਨ੍ਤਰ੍ਭਾਵ <numeric> // std::accumulate

ਨਾਮਾਕਾਸ਼ cv {

////////////////////////////////////// transpose /////////////////////////////////////////
#ਜੇ CV_SIMD128
ਸਥਿਰ ਖਾਲੀ transpose_8bit_simd(ਅਚਲ uchar* src, size_t sstep, uchar* dst, size_t dstep, ਮਾਪ sz)
{
    ਅਚਲ ਪੂਰਨ m = sz.ਚੌੜਾਈ, n = sz.ਉਚਾਈ;
    ਪੂਰਨ i = 0;
    ਲਈ (; i <= m - 16; i += 16)
    {
        ਪੂਰਨ j = 0;
        ਲਈ (; j <= n - 16; j += 16)
        {
            v_uint8x16 r0  = v_ਲੱਦੋ(src + i + sstep*(j+ 0));
            v_uint8x16 r1  = v_ਲੱਦੋ(src + i + sstep*(j+ 1));
            v_uint8x16 r2  = v_ਲੱਦੋ(src + i + sstep*(j+ 2));
            v_uint8x16 r3  = v_ਲੱਦੋ(src + i + sstep*(j+ 3));
            v_uint8x16 r4  = v_ਲੱਦੋ(src + i + sstep*(j+ 4));
            v_uint8x16 r5  = v_ਲੱਦੋ(src + i + sstep*(j+ 5));
            v_uint8x16 r6  = v_ਲੱਦੋ(src + i + sstep*(j+ 6));
            v_uint8x16 r7  = v_ਲੱਦੋ(src + i + sstep*(j+ 7));
            v_uint8x16 r8  = v_ਲੱਦੋ(src + i + sstep*(j+ 8));
            v_uint8x16 r9  = v_ਲੱਦੋ(src + i + sstep*(j+ 9));
            v_uint8x16 r10 = v_ਲੱਦੋ(src + i + sstep*(j+10));
            v_uint8x16 r11 = v_ਲੱਦੋ(src + i + sstep*(j+11));
            v_uint8x16 r12 = v_ਲੱਦੋ(src + i + sstep*(j+12));
            v_uint8x16 r13 = v_ਲੱਦੋ(src + i + sstep*(j+13));
            v_uint8x16 r14 = v_ਲੱਦੋ(src + i + sstep*(j+14));
            v_uint8x16 r15 = v_ਲੱਦੋ(src + i + sstep*(j+15));

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

            v_uint32x4 v0  = v_combine_ਨੀਵਾਂ (u0,  u8);
            v_uint32x4 v1  = v_combine_ਉੱਚਾ(u0,  u8);
            v_uint32x4 v2  = v_combine_ਨੀਵਾਂ (u1,  u9);
            v_uint32x4 v3  = v_combine_ਉੱਚਾ(u1,  u9);
            v_uint32x4 v4  = v_combine_ਨੀਵਾਂ (u2,  u10);
            v_uint32x4 v5  = v_combine_ਉੱਚਾ(u2,  u10);
            v_uint32x4 v6  = v_combine_ਨੀਵਾਂ (u3,  u11);
            v_uint32x4 v7  = v_combine_ਉੱਚਾ(u3,  u11);
            v_uint32x4 v8  = v_combine_ਨੀਵਾਂ (u4,  u12);
            v_uint32x4 v9  = v_combine_ਉੱਚਾ(u4,  u12);
            v_uint32x4 v10 = v_combine_ਨੀਵਾਂ (u5,  u13);
            v_uint32x4 v11 = v_combine_ਉੱਚਾ(u5,  u13);
            v_uint32x4 v12 = v_combine_ਨੀਵਾਂ (u6,  u14);
            v_uint32x4 v13 = v_combine_ਉੱਚਾ(u6,  u14);
            v_uint32x4 v14 = v_combine_ਨੀਵਾਂ (u7,  u15);
            v_uint32x4 v15 = v_combine_ਉੱਚਾ(u7,  u15);

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
        ਲਈ (; j < n; j++)
            ਲਈ (ਪੂਰਨ k = 0; k < 16; k++)
                dst[dstep*(i+k) + j] = src[i + sstep*j + k];
    }
    ਲਈ (; i < m; i++)
        ਲਈ (ਪੂਰਨ j = 0; j < n; j++)
            dst[dstep*i + j] = src[i + sstep*j];
}

ਸਥਿਰ ਖਾਲੀ transpose_16bit_simd(ਅਚਲ uchar* src, size_t sstep, uchar* dst, size_t dstep, ਮਾਪ sz)
{
    ਅਚਲ ushort* src16 = reinterpret_cast<ਅਚਲ ushort*>(src);
    ushort* dst16 = reinterpret_cast<ushort*>(dst);

    ਅਚਲ size_t sstep_e = sstep / ਆਕਾਰ(ushort);
    ਅਚਲ size_t dstep_e = dstep / ਆਕਾਰ(ushort);

    ਅਚਲ ਪੂਰਨ m = sz.ਚੌੜਾਈ, n = sz.ਉਚਾਈ;
    ਪੂਰਨ i = 0;

    ਲਈ (; i <= m - 8; i += 8)
    {
        ਪੂਰਨ j = 0;
        ਲਈ (; j <= n - 8; j += 8)
        {
            v_uint16x8 r0 = v_ਲੱਦੋ(src16 + i + sstep_e*(j+0));
            v_uint16x8 r1 = v_ਲੱਦੋ(src16 + i + sstep_e*(j+1));
            v_uint16x8 r2 = v_ਲੱਦੋ(src16 + i + sstep_e*(j+2));
            v_uint16x8 r3 = v_ਲੱਦੋ(src16 + i + sstep_e*(j+3));
            v_uint16x8 r4 = v_ਲੱਦੋ(src16 + i + sstep_e*(j+4));
            v_uint16x8 r5 = v_ਲੱਦੋ(src16 + i + sstep_e*(j+5));
            v_uint16x8 r6 = v_ਲੱਦੋ(src16 + i + sstep_e*(j+6));
            v_uint16x8 r7 = v_ਲੱਦੋ(src16 + i + sstep_e*(j+7));

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
        ਲਈ (; j < n; j++)
            ਲਈ (ਪੂਰਨ k = 0; k < 8; k++)
                dst16[dstep_e*(i+k) + j] = src16[i + sstep_e*j + k];
    }
    ਲਈ (; i < m; i++)
        ਲਈ (ਪੂਰਨ j = 0; j < n; j++)
            dst16[dstep_e*i + j] = src16[i + sstep_e*j];
}

ਸਥਿਰ ਖਾਲੀ transpose_32bit_simd(ਅਚਲ uchar* src, size_t sstep, uchar* dst, size_t dstep, ਮਾਪ sz)
{
    // Cache-blocked + AVX2-aware path lives in transpose.simd.hpp; pick the
    // best variant available at runtime.
    CV_CPU_DISPATCH(transpose_32bit_ਖੰਡਾਂ_simd, (src, sstep, dst, dstep, sz),
                    CV_CPU_DISPATCH_MODES_ALL);
}

ਸਥਿਰ ਖਾਲੀ transpose_48bit_simd(ਅਚਲ uchar* src, size_t sstep, uchar* dst, size_t dstep, ਮਾਪ sz)
{
    ਅਚਲ ਛੋਟਾ* src16 = reinterpret_cast<ਅਚਲ ਛੋਟਾ*>(src);
    ਛੋਟਾ* dst16 = reinterpret_cast<ਛੋਟਾ*>(dst);

    ਅਚਲ size_t sstep_e = sstep / ਆਕਾਰ(ਛੋਟਾ);
    ਅਚਲ size_t dstep_e = dstep / ਆਕਾਰ(ਛੋਟਾ);

    ਅਚਲ ਪੂਰਨ m = sz.ਚੌੜਾਈ, n = sz.ਉਚਾਈ;
    ਪੂਰਨ i = 0;

    ਲਈ (; i <= m - 8; i += 8)
    {
        ਪੂਰਨ j = 0;
        ਲਈ (; j <= n - 8; j += 8)
        {
            v_int16x8 C0_0, C1_0, C2_0;
            v_int16x8 C0_1, C1_1, C2_1;
            v_int16x8 C0_2, C1_2, C2_2;
            v_int16x8 C0_3, C1_3, C2_3;
            v_int16x8 C0_4, C1_4, C2_4;
            v_int16x8 C0_5, C1_5, C2_5;
            v_int16x8 C0_6, C1_6, C2_6;
            v_int16x8 C0_7, C1_7, C2_7;

            v_ਲੱਦੋ_deinterleave(src16 + sstep_e*(j+0) + i*3, C0_0, C1_0, C2_0);
            v_ਲੱਦੋ_deinterleave(src16 + sstep_e*(j+1) + i*3, C0_1, C1_1, C2_1);
            v_ਲੱਦੋ_deinterleave(src16 + sstep_e*(j+2) + i*3, C0_2, C1_2, C2_2);
            v_ਲੱਦੋ_deinterleave(src16 + sstep_e*(j+3) + i*3, C0_3, C1_3, C2_3);
            v_ਲੱਦੋ_deinterleave(src16 + sstep_e*(j+4) + i*3, C0_4, C1_4, C2_4);
            v_ਲੱਦੋ_deinterleave(src16 + sstep_e*(j+5) + i*3, C0_5, C1_5, C2_5);
            v_ਲੱਦੋ_deinterleave(src16 + sstep_e*(j+6) + i*3, C0_6, C1_6, C2_6);
            v_ਲੱਦੋ_deinterleave(src16 + sstep_e*(j+7) + i*3, C0_7, C1_7, C2_7);

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
        ਲਈ (; j < n; j++)
            ਲਈ (ਪੂਰਨ k = 0; k < 8; k++)
            {
                dst16[dstep_e*(i+k) + j*3 + 0] = src16[sstep_e*j + (i+k)*3 + 0];
                dst16[dstep_e*(i+k) + j*3 + 1] = src16[sstep_e*j + (i+k)*3 + 1];
                dst16[dstep_e*(i+k) + j*3 + 2] = src16[sstep_e*j + (i+k)*3 + 2];
            }
    }
    ਲਈ (; i < m; i++)
        ਲਈ (ਪੂਰਨ j = 0; j < n; j++)
        {
            dst16[dstep_e*i + j*3 + 0] = src16[sstep_e*j + i*3 + 0];
            dst16[dstep_e*i + j*3 + 1] = src16[sstep_e*j + i*3 + 1];
            dst16[dstep_e*i + j*3 + 2] = src16[sstep_e*j + i*3 + 2];
        }
}
#ਜੇ_ਅੰਤ

ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ T> ਸਥਿਰ ਖਾਲੀ
transpose_( ਅਚਲ uchar* src, size_t sstep, uchar* dst, size_t dstep, ਮਾਪ sz )
{
#ਜੇ CV_SIMD128
    ਚੋਣ (ਆਕਾਰ(T))
    {
        ਹਾਲਤ 1: transpose_8bit_simd(src, sstep, dst, dstep, sz);  ਵਾਪਸ;
        ਹਾਲਤ 2: transpose_16bit_simd(src, sstep, dst, dstep, sz); ਵਾਪਸ;
        ਹਾਲਤ 4: transpose_32bit_simd(src, sstep, dst, dstep, sz); ਵਾਪਸ;
        ਹਾਲਤ 6: transpose_48bit_simd(src, sstep, dst, dstep, sz); ਵਾਪਸ;
        ਮੂਲ: ਰੋਕੋ;
    }
#ਜੇ_ਅੰਤ

    ਪੂਰਨ i = 0, j, m = sz.ਚੌੜਾਈ, n = sz.ਉਚਾਈ;

    #ਜੇ CV_ENABLE_UNROLLED
    ਲਈ(; i <= m - 4; i += 4 )
    {
        T* d0 = (T*)(dst + dstep*i);
        T* d1 = (T*)(dst + dstep*(i+1));
        T* d2 = (T*)(dst + dstep*(i+2));
        T* d3 = (T*)(dst + dstep*(i+3));

        ਲਈ( j = 0; j <= n - 4; j += 4 )
        {
            ਅਚਲ T* s0 = (ਅਚਲ T*)(src + i*ਆਕਾਰ(T) + sstep*j);
            ਅਚਲ T* s1 = (ਅਚਲ T*)(src + i*ਆਕਾਰ(T) + sstep*(j+1));
            ਅਚਲ T* s2 = (ਅਚਲ T*)(src + i*ਆਕਾਰ(T) + sstep*(j+2));
            ਅਚਲ T* s3 = (ਅਚਲ T*)(src + i*ਆਕਾਰ(T) + sstep*(j+3));

            d0[j] = s0[0]; d0[j+1] = s1[0]; d0[j+2] = s2[0]; d0[j+3] = s3[0];
            d1[j] = s0[1]; d1[j+1] = s1[1]; d1[j+2] = s2[1]; d1[j+3] = s3[1];
            d2[j] = s0[2]; d2[j+1] = s1[2]; d2[j+2] = s2[2]; d2[j+3] = s3[2];
            d3[j] = s0[3]; d3[j+1] = s1[3]; d3[j+2] = s2[3]; d3[j+3] = s3[3];
        }

        ਲਈ( ; j < n; j++ )
        {
            ਅਚਲ T* s0 = (ਅਚਲ T*)(src + i*ਆਕਾਰ(T) + j*sstep);
            d0[j] = s0[0]; d1[j] = s0[1]; d2[j] = s0[2]; d3[j] = s0[3];
        }
    }
    #ਜੇ_ਅੰਤ
    ਲਈ( ; i < m; i++ )
    {
        T* d0 = (T*)(dst + dstep*i);
        j = 0;
        #ਜੇ CV_ENABLE_UNROLLED
        ਲਈ(; j <= n - 4; j += 4 )
        {
            ਅਚਲ T* s0 = (ਅਚਲ T*)(src + i*ਆਕਾਰ(T) + sstep*j);
            ਅਚਲ T* s1 = (ਅਚਲ T*)(src + i*ਆਕਾਰ(T) + sstep*(j+1));
            ਅਚਲ T* s2 = (ਅਚਲ T*)(src + i*ਆਕਾਰ(T) + sstep*(j+2));
            ਅਚਲ T* s3 = (ਅਚਲ T*)(src + i*ਆਕਾਰ(T) + sstep*(j+3));

            d0[j] = s0[0]; d0[j+1] = s1[0]; d0[j+2] = s2[0]; d0[j+3] = s3[0];
        }
        #ਜੇ_ਅੰਤ
        ਲਈ( ; j < n; j++ )
        {
            ਅਚਲ T* s0 = (ਅਚਲ T*)(src + i*ਆਕਾਰ(T) + j*sstep);
            d0[j] = s0[0];
        }
    }
}

ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ T> ਸਥਿਰ ਖਾਲੀ
transposeI_( uchar* ਅੰਕੜੇ, size_t ਕਦਮ, ਪੂਰਨ n )
{
    ਲਈ( ਪੂਰਨ i = 0; i < n; i++ )
    {
        T* ਪੰਕਤੀ = (T*)(ਅੰਕੜੇ + ਕਦਮ*i);
        uchar* data1 = ਅੰਕੜੇ + i*ਆਕਾਰ(T);
        ਲਈ( ਪੂਰਨ j = i+1; j < n; j++ )
            std::swap( ਪੰਕਤੀ[j], *(T*)(data1 + ਕਦਮ*j) );
    }
}

ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਖਾਲੀ (*TransposeFunc)( ਅਚਲ uchar* src, size_t sstep, uchar* dst, size_t dstep, ਮਾਪ sz );
ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਖਾਲੀ (*TransposeInplaceFunc)( uchar* ਅੰਕੜੇ, size_t ਕਦਮ, ਪੂਰਨ n );

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ DEF_TRANSPOSE_FUNC(suffix, type) \
ਸਥਿਰ ਖਾਲੀ transpose_##suffix( ਅਚਲ uchar* src, size_t sstep, uchar* dst, size_t dstep, ਮਾਪ sz ) \
{ transpose_<type>(src, sstep, dst, dstep, sz); } \
\
ਸਥਿਰ ਖਾਲੀ transposeI_##suffix( uchar* ਅੰਕੜੇ, size_t ਕਦਮ, ਪੂਰਨ n ) \
{ transposeI_<type>(ਅੰਕੜੇ, ਕਦਮ, n); }

DEF_TRANSPOSE_FUNC(8u, uchar)
DEF_TRANSPOSE_FUNC(16u, ushort)
DEF_TRANSPOSE_FUNC(8uC3, Vec3b)
DEF_TRANSPOSE_FUNC(32s, ਪੂਰਨ)
DEF_TRANSPOSE_FUNC(16uC3, Vec3s)
DEF_TRANSPOSE_FUNC(32sC2, Vec2i)
DEF_TRANSPOSE_FUNC(32sC3, Vec3i)
DEF_TRANSPOSE_FUNC(32sC4, Vec4i)
DEF_TRANSPOSE_FUNC(32sC6, Vec6i)
DEF_TRANSPOSE_FUNC(32sC8, Vec8i)

ਸਥਿਰ TransposeFunc transposeTab[] =
{
    0, transpose_8u, transpose_16u, transpose_8uC3, transpose_32s, 0, transpose_16uC3, 0,
    transpose_32sC2, 0, 0, 0, transpose_32sC3, 0, 0, 0, transpose_32sC4,
    0, 0, 0, 0, 0, 0, 0, transpose_32sC6, 0, 0, 0, 0, 0, 0, 0, transpose_32sC8
};

nd::TransposeFunc nd::getTransposeFunc(size_t esz)
{
    ਵਾਪਸ esz < ਆਕਾਰ(transposeTab)/ਆਕਾਰ(transposeTab[0]) ? transposeTab[esz] : ਖਾਲੀ_ਸੂਚਕ;
}

ਸਥਿਰ TransposeInplaceFunc transposeInplaceTab[] =
{
    0, transposeI_8u, transposeI_16u, transposeI_8uC3, transposeI_32s, 0, transposeI_16uC3, 0,
    transposeI_32sC2, 0, 0, 0, transposeI_32sC3, 0, 0, 0, transposeI_32sC4,
    0, 0, 0, 0, 0, 0, 0, transposeI_32sC6, 0, 0, 0, 0, 0, 0, 0, transposeI_32sC8
};

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_OPENCL

ਸਥਿਰ ਤਾਰਕਿਕ ocl_transpose( ਨਿਵੇਸ਼_Array _src, OutputArray _dst )
{
    ਅਚਲ ocl::Device & dev = ocl::Device::getDefault();
    ਅਚਲ ਪੂਰਨ TILE_DIM = 32, ਖੰਡ_ਪੰਕਤੀਆਂ = 8;
    ਪੂਰਨ type = _src.type(), cn = CV_MAT_CN(type), ਡੂੰਘਾਈ = CV_MAT_ਡੂੰਘਾਈ(type),
        ਪੰਕਤੀਆਂ_ਪ੍ਰਤੀ_WI = dev.isIntel() ? 4 : 1;

    UMat src = _src.getUMat();
    _dst.create(src.ਖਾਨੇ_ਗਿਣਤੀ, src.ਪੰਕਤੀਆਂ, type);
    UMat dst = _dst.getUMat();

    String kernelName("transpose");
    ਤਾਰਕਿਕ inplace = dst.u == src.u;

    ਜੇ (inplace)
    {
        CV_Assert(dst.ਖਾਨੇ_ਗਿਣਤੀ == dst.ਪੰਕਤੀਆਂ);
        kernelName += "_inplace";
    }
    ਨਹੀਂਤਾਂ
    {
        // check required local memory size
        size_t required_local_ਸਮ੍ਰਿਤੀ = (size_t) TILE_DIM*(TILE_DIM+1)*CV_ELEM_ਮਾਪ(type);
        ਜੇ (required_local_ਸਮ੍ਰਿਤੀ > ocl::Device::getDefault().local_Mem_ਮਾਪ())
            ਵਾਪਸ ਝੂਠ;
    }

    String deviceMacro;
    ਜੇ (dev.isIntel())
        deviceMacro = " -D INTEL_GPU";
    ਨਹੀਂਤਾਂ
        deviceMacro = "";

    ocl::Kernel k(kernelName.c_str(), ocl::core::transpose_oclsrc,
                  format("-D T=%s -D T1=%s -D cn=%d -D TILE_DIM=%d -D BLOCK_ROWS=%d -D rowsPerWI=%d%s%s",
                         ocl::memop_Type_ਮੰਜ਼ਿਲ_Str(type), ocl::memop_Type_ਮੰਜ਼ਿਲ_Str(ਡੂੰਘਾਈ),
                         cn, TILE_DIM, ਖੰਡ_ਪੰਕਤੀਆਂ, ਪੰਕਤੀਆਂ_ਪ੍ਰਤੀ_WI, inplace ? " -D INPLACE" : "", deviceMacro.c_str()));
    ਜੇ (k.empty())
        ਵਾਪਸ ਝੂਠ;

    ਜੇ (inplace)
        k.ਦਲੀਲਾਂ(ocl::KernelArg::Read_Write_No_ਮਾਪ(dst), dst.ਪੰਕਤੀਆਂ);
    ਨਹੀਂਤਾਂ
        k.ਦਲੀਲਾਂ(ocl::KernelArg::ReadOnly(src),
               ocl::KernelArg::Write_Only_No_ਮਾਪ(dst));

    size_t localsize[2]  = { TILE_DIM, ਖੰਡ_ਪੰਕਤੀਆਂ };
    size_t globalsize[2] = { (size_t)src.ਖਾਨੇ_ਗਿਣਤੀ, inplace ? ((size_t)src.ਪੰਕਤੀਆਂ + ਪੰਕਤੀਆਂ_ਪ੍ਰਤੀ_WI - 1) / ਪੰਕਤੀਆਂ_ਪ੍ਰਤੀ_WI : (div_ਉੱਪਰ((size_t)src.ਪੰਕਤੀਆਂ, TILE_DIM) * ਖੰਡ_ਪੰਕਤੀਆਂ) };

    ਜੇ (inplace && dev.isIntel())
    {
        localsize[0] = 16;
        localsize[1] = dev.max_Work_ਸਮੂਹ_ਮਾਪ() / localsize[0];
    }

    ਵਾਪਸ k.ਲੜੀਵਾਰ(2, globalsize, localsize, ਝੂਠ);
}

#ਜੇ_ਅੰਤ

ਖਾਲੀ transpose( ਨਿਵੇਸ਼_Array _src, OutputArray _dst )
{
    CV_INSTRUMENT_REGION();

    ਪੂਰਨ type = _src.type(), esz = CV_ELEM_ਮਾਪ(type);
    CV_Assert( _src.dims() <= 2 && esz <= 32 );

    CV_OCL_ਲੜੀਵਾਰ(_dst.isUMat(),
               ocl_transpose(_src, _dst))

    Mat src = _src.getMat();
    ਜੇ( src.empty() )
    {
        _dst.release();
        ਵਾਪਸ;
    }

    _dst.create(src.ਖਾਨੇ_ਗਿਣਤੀ, src.ਪੰਕਤੀਆਂ, src.type());
    Mat dst = _dst.getMat();

    // handle the case of single-column/single-row matrices, stored in STL vectors.
    ਜੇ( src.ਪੰਕਤੀਆਂ != dst.ਖਾਨੇ_ਗਿਣਤੀ || src.ਖਾਨੇ_ਗਿਣਤੀ != dst.ਪੰਕਤੀਆਂ )
    {
        CV_Assert( src.size() == dst.size() && (src.ਖਾਨੇ_ਗਿਣਤੀ == 1 || src.ਪੰਕਤੀਆਂ == 1) );
        src.copy_ਮੰਜ਼ਿਲ(dst);
        ਵਾਪਸ;
    }

    CALL_HAL(transpose2d, cv_hal_transpose2d, src.ਅੰਕੜੇ, src.ਕਦਮ, dst.ਅੰਕੜੇ, dst.ਕਦਮ, src.ਖਾਨੇ_ਗਿਣਤੀ, src.ਪੰਕਤੀਆਂ, esz);

    ਜੇ( dst.ਅੰਕੜੇ == src.ਅੰਕੜੇ )
    {
        TransposeInplaceFunc func = transposeInplaceTab[esz];
        CV_Assert( func != 0 );
        CV_Assert( dst.ਖਾਨੇ_ਗਿਣਤੀ == dst.ਪੰਕਤੀਆਂ );
        func( dst.ptr(), dst.ਕਦਮ, dst.ਪੰਕਤੀਆਂ );
    }
    ਨਹੀਂਤਾਂ
    {
        TransposeFunc func = transposeTab[esz];
        CV_Assert( func != 0 );
        func( src.ptr(), src.ਕਦਮ, dst.ptr(), dst.ਕਦਮ, src.size() );
    }
}


ਖਾਲੀ transpose(ਨਿਵੇਸ਼_Array src_, ਅਚਲ std::vector<ਪੂਰਨ>& ਕ੍ਰਮ, OutputArray dst_)
{
    CV_INSTRUMENT_REGION();

    Mat inp = src_.getMat();
    ਅਚਲ ਪੂਰਨ ndims = inp.dims;
    CV_Assert(!(ndims == 0 && inp.empty()));
    CV_CheckEQ(ਕ੍ਰਮ.size(), static_cast<size_t>(ndims), "Number of dimensions shouldn't change");

    ਤਾਰਕਿਕ ਵਰਤਿਆ[CV_MAX_DIM] = {ਝੂਠ};
    ਪੂਰਨ newShape[CV_MAX_DIM];
    ਲਈ (ਪੂਰਨ i = 0; i < ndims; i++)
    {
        ਪੂਰਨ j = ਕ੍ਰਮ[i];
        CV_Check(j, 0 <= j && j < ndims && !ਵਰਤਿਆ[j], "New order should be a valid permutation of the old one");
        ਵਰਤਿਆ[j] = ਸੱਚ;
        newShape[i] = inp.size[j];
    }

    // plain 2D transpose keeps using cv::transpose (HAL, in-place square case);
    // not for an empty input, which cv::transpose would answer by releasing dst
    ਜੇ (ndims == 2 && ਕ੍ਰਮ[0] == 1 && !inp.empty() && nd::getTransposeFunc(inp.elem_ਮਾਪ()))
    {
        transpose(inp, dst_);
        ਵਾਪਸ;
    }

    dst_.create(ndims, newShape, inp.type());
    Mat out = dst_.getMat();
    ਜੇ (out.ਕੁੱਲ() == 0)
        ਵਾਪਸ;
    nd::copy(nd::ਕ੍ਰਮ_ਬਦਲੋ(nd::view_ਦਾ(inp), ਕ੍ਰਮ.ਅੰਕੜੇ()), nd::view_ਦਾ(out));
}

ਖਾਲੀ transposeND(ਨਿਵੇਸ਼_Array src, ਅਚਲ std::vector<ਪੂਰਨ>& ਕ੍ਰਮ, OutputArray dst)
{
    transpose(src, ਕ੍ਰਮ, dst);
}


// Generic scalar fallback
CV_ALWAYS_INLINE ਖਾਲੀ flipHoriz_generic( ਅਚਲ uchar* src, size_t sstep, uchar* dst, size_t dstep, ਮਾਪ size, size_t esz )
{
    ਪੂਰਨ i, j, ਹੱਦਬੰਦੀ = (ਪੂਰਨ)(((size.ਚੌੜਾਈ + 1)/2)*esz);
    ਪੂਰਨ ਉਚਾਈ = size.ਉਚਾਈ;
    Auto_ਬਫਰ<ਪੂਰਨ> _tab(size.ਚੌੜਾਈ*esz);
    ਪੂਰਨ* tab = _tab.ਅੰਕੜੇ();

    ਲਈ( i = 0; i < size.ਚੌੜਾਈ; i++ )
        ਲਈ( size_t k = 0; k < esz; k++ )
            tab[i*esz + k] = (ਪੂਰਨ)((size.ਚੌੜਾਈ - i - 1)*esz + k);

    ਲਈ( ; ਉਚਾਈ--; src += sstep, dst += dstep )
    {
        ਲਈ( i = 0; i < ਹੱਦਬੰਦੀ; i++ )
        {
            j = tab[i];
            uchar t0 = src[i], t1 = src[j];
            dst[i] = t1; dst[j] = t0;
        }
    }
}

#ਜੇ CV_SIMD || CV_SIMD_SCALABLE
ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ V> CV_ALWAYS_INLINE ਖਾਲੀ flipHoriz_single( ਅਚਲ uchar* src, size_t sstep, uchar* dst, size_t dstep, ਮਾਪ size )
{
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਕਿਸਮ_ਨਾਂ VTraits<V>::lane_type T;
    ਅਚਲ ਪੂਰਨ vlanes = VTraits<v_uint8>::vlanes();
    ਪੂਰਨ end = (ਪੂਰਨ)(size.ਚੌੜਾਈ * ਆਕਾਰ(T));
    ਪੂਰਨ ਚੌੜਾਈ = (end + 1) / 2;
    ਪੂਰਨ ਚੌੜਾਈ_simd = ਚੌੜਾਈ & -vlanes;
    ਪੂਰਨ ਉਚਾਈ = size.ਉਚਾਈ;

    CV_DbgAssert(!CV_STRONG_ALIGNMENT || isAligned<ਆਕਾਰ(T)>(src, dst));

    ਲਈ( ; ਉਚਾਈ--; src += sstep, dst += dstep )
    {
        ਪੂਰਨ i = 0, j = end;
        ਲਈ( ; i < ਚੌੜਾਈ_simd; i += vlanes, j -= vlanes )
        {
            V t0 = vx_ਲੱਦੋ((ਅਚਲ T*)(src + i));
            V t1 = vx_ਲੱਦੋ((ਅਚਲ T*)(src + j - vlanes));
            t0 = v_ਉਲਟ(t0);
            t1 = v_ਉਲਟ(t1);
            v_store((T*)(dst + j - vlanes), t0);
            v_store((T*)(dst + i), t1);
        }

        // Scalar tail loop
        ਜੇ (isAligned<ਆਕਾਰ(T)>(src, dst))
        {
            ਲਈ ( ; i < ਚੌੜਾਈ; i += ਆਕਾਰ(T), j -= ਆਕਾਰ(T) )
            {
                T t0 = *((ਅਚਲ T*)(src + i));
                T t1 = *((ਅਚਲ T*)(src + j - ਆਕਾਰ(T)));
                *((T*)(dst + j - ਆਕਾਰ(T))) = t0;
                *((T*)(dst + i)) = t1;
            }
        }
        ਨਹੀਂਤਾਂ
        {
            ਲਈ ( ; i < ਚੌੜਾਈ; i += ਆਕਾਰ(T), j -= ਆਕਾਰ(T) )
            {
                ਲਈ (ਪੂਰਨ k = 0; k < (ਪੂਰਨ)ਆਕਾਰ(T); k++)
                {
                    uchar t0 = src[i + k];
                    uchar t1 = src[j + k - ਆਕਾਰ(T)];
                    dst[j + k - ਆਕਾਰ(T)] = t0;
                    dst[i + k] = t1;
                }
            }
        }
    }
}

// SIMD for C3
ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ V>
CV_ALWAYS_INLINE ਖਾਲੀ flipHoriz_c3( ਅਚਲ uchar* src, size_t sstep, uchar* dst, size_t dstep, ਮਾਪ size )
{
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਕਿਸਮ_ਨਾਂ VTraits<V>::lane_type T;
    ਅਚਲ ਪੂਰਨ vlanes = VTraits<V>::vlanes();
    ਅਚਲ ਪੂਰਨ stride = 3 * ਆਕਾਰ(T);
    ਪੂਰਨ ਚੌੜਾਈ = size.ਚੌੜਾਈ;
    ਪੂਰਨ centre = (ਚੌੜਾਈ + 1) / 2;
    ਪੂਰਨ ਚੌੜਾਈ_simd = (centre / vlanes) * vlanes;
    ਪੂਰਨ ਉਚਾਈ = size.ਉਚਾਈ;

    ਲਈ( ; ਉਚਾਈ--; src += sstep, dst += dstep )
    {
        ਪੂਰਨ i = 0;
        ਲਈ( ; i < ਚੌੜਾਈ_simd; i += vlanes )
        {
            V r0, g0, b0;
            v_ਲੱਦੋ_deinterleave((ਅਚਲ T*)(src + i * stride), r0, g0, b0);
            V r1, g1, b1;
            v_ਲੱਦੋ_deinterleave((ਅਚਲ T*)(src + (ਚੌੜਾਈ - i - vlanes) * stride), r1, g1, b1);
            r0 = v_ਉਲਟ(r0);
            g0 = v_ਉਲਟ(g0);
            b0 = v_ਉਲਟ(b0);
            r1 = v_ਉਲਟ(r1);
            g1 = v_ਉਲਟ(g1);
            b1 = v_ਉਲਟ(b1);
            v_store_interleave((T*)(dst + (ਚੌੜਾਈ - i - vlanes) * stride), r0, g0, b0);
            v_store_interleave((T*)(dst + i * stride), r1, g1, b1);
        }
        // Scalar tail loop for remaining pixels
        ਲਈ( ; i < centre; i++ )
        {
            ਪੂਰਨ j = ਚੌੜਾਈ - i - 1;
            T c0 = ((ਅਚਲ T*)(src + i * stride))[0];
            T c1 = ((ਅਚਲ T*)(src + i * stride))[1];
            T c2 = ((ਅਚਲ T*)(src + i * stride))[2];
            T c3 = ((ਅਚਲ T*)(src + j * stride))[0];
            T c4 = ((ਅਚਲ T*)(src + j * stride))[1];
            T c5 = ((ਅਚਲ T*)(src + j * stride))[2];
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
ਸਾਂਚਾ<size_t ESZ>
CV_ALWAYS_INLINE ਖਾਲੀ flip_Horiz_vlanes_ਮੇਲ( ਅਚਲ uchar* src, size_t sstep, uchar* dst, size_t dstep, ਮਾਪ size)
{
    ਅਚਲ ਪੂਰਨ vlanes = VTraits<v_uint8>::vlanes();
    ਪੂਰਨ end = (ਪੂਰਨ)(size.ਚੌੜਾਈ * ESZ);
    ਪੂਰਨ ਚੌੜਾਈ = end / 2;
    ਪੂਰਨ ਉਚਾਈ = size.ਉਚਾਈ;
    ਪੂਰਨ e_ਮਾਪ = (ਪੂਰਨ)ESZ;
    ਲਈ( ; ਉਚਾਈ--; src += sstep, dst += dstep )
    {
        ਲਈ( ਪੂਰਨ i = 0, j = end - e_ਮਾਪ; i < ਚੌੜਾਈ; i += e_ਮਾਪ, j -= e_ਮਾਪ )
        {
            ਲਈ( ਪੂਰਨ k = 0; k < e_ਮਾਪ; k += vlanes )
            {
                v_uint8 t0 = vx_ਲੱਦੋ(src + i + k);
                v_uint8 t1 = vx_ਲੱਦੋ(src + j + k);
                v_store(dst + j + k, t0);
                v_store(dst + i + k, t1);
            }
        }
    }
}

#ਜੇ CV_SIMD128
// SIMD flip when ESZ=16 (128-bit)
ਸਾਂਚਾ<size_t ESZ>
CV_ALWAYS_INLINE ਖਾਲੀ flip_Horiz_vlanes_ਮੇਲ_128( ਅਚਲ uchar* src, size_t sstep, uchar* dst, size_t dstep, ਮਾਪ size)
{
    ਅਚਲ ਪੂਰਨ vlanes16 = VTraits<v_uint8x16>::vlanes();
    ਪੂਰਨ end = (ਪੂਰਨ)(size.ਚੌੜਾਈ * ESZ);
    ਪੂਰਨ ਚੌੜਾਈ = end / 2;
    ਪੂਰਨ ਉਚਾਈ = size.ਉਚਾਈ;
    ਲਈ( ; ਉਚਾਈ--; src += sstep, dst += dstep )
    {
        ਲਈ( ਪੂਰਨ i = 0, j = end - vlanes16; i < ਚੌੜਾਈ; i += vlanes16, j -= vlanes16 )
        {
            v_uint8x16 t0 = v_ਲੱਦੋ(src + i);
            v_uint8x16 t1 = v_ਲੱਦੋ(src + j);
            v_store(dst + j, t0);
            v_store(dst + i, t1);
        }
    }
}
#ਜੇ_ਅੰਤ // CV_SIMD128

// SIMD flip for ESZ=16,32
ਸਾਂਚਾ<size_t ESZ>
CV_ALWAYS_INLINE ਖਾਲੀ flipHoriz_vlanes_dispatch( ਅਚਲ uchar* src, size_t sstep, uchar* dst, size_t dstep, ਮਾਪ size )
{
    ਅਚਲ ਪੂਰਨ vlanes = VTraits<v_uint8>::vlanes();
#ਜੇ CV_SIMD128
    ਅਚਲ ਪੂਰਨ vlanes16 = VTraits<v_uint8x16>::vlanes();
#ਜੇ_ਅੰਤ
    ਜੇ ( (ESZ == (size_t)vlanes) || (ESZ == 2 * (size_t)vlanes))
    {
        flip_Horiz_vlanes_ਮੇਲ<ESZ>(src, sstep, dst, dstep, size);
        ਵਾਪਸ;
    }
#ਜੇ CV_SIMD128
    ਨਹੀਂਤਾਂ ਜੇ (ESZ == vlanes16)
    {
        flip_Horiz_vlanes_ਮੇਲ_128<ESZ>(src, sstep, dst, dstep, size);
        ਵਾਪਸ;
    }
#ਜੇ_ਅੰਤ
    flipHoriz_generic(src, sstep, dst, dstep, size, ESZ);
}

#ਜੇ CV_SIMD128
// SIMD flip for ESZ=24 (CV_64FC3)
CV_ALWAYS_INLINE ਖਾਲੀ flipHoriz_24( ਅਚਲ uchar* src, size_t sstep, uchar* dst, size_t dstep, ਮਾਪ size )
{
    CV_DbgAssert(!CV_STRONG_ALIGNMENT || isAligned<8>(((size_t)src)|((size_t)dst)|sstep|dstep));

    ਅਚਲ ਪੂਰਨ lanes16 = 16;
    ਪੂਰਨ end = (ਪੂਰਨ)(size.ਚੌੜਾਈ * 24);
    ਪੂਰਨ ਚੌੜਾਈ = (end + 1) / 2;
    ਪੂਰਨ ਉਚਾਈ = size.ਉਚਾਈ;
    ਲਈ( ; ਉਚਾਈ--; src += sstep, dst += dstep )
    {
        ਲਈ ( ਪੂਰਨ i = 0, j = end; i < ਚੌੜਾਈ; i += lanes16 + 8, j -= lanes16 + 8 )
        {
            v_uint8x16 t0 = v_ਲੱਦੋ(src + i);
            uint64_t t2 = *reinterpret_cast<ਅਚਲ uint64_t*>(src + i + lanes16);
            v_uint8x16 t1 = v_ਲੱਦੋ(src + j - lanes16 - 8);
            uint64_t t3 = *reinterpret_cast<ਅਚਲ uint64_t*>(src + j - 8);
            v_store(dst + j - lanes16 - 8, t0);
            *reinterpret_cast<uint64_t*>(dst + j - 8) = t2;
            v_store(dst + i, t1);
            *reinterpret_cast<uint64_t*>(dst + i + lanes16) = t3;
        }
    }
}
#ਜੇ_ਅੰਤ // CV_SIMD128
#ਜੇ_ਅੰਤ // CV_SIMD || CV_SIMD_SCALABLE

ਸਥਿਰ ਖਾਲੀ flipHoriz( ਅਚਲ uchar* src, size_t sstep, uchar* dst, size_t dstep, ਮਾਪ size, size_t esz )
{
#ਜੇ CV_SIMD || CV_SIMD_SCALABLE
#ਜੇ CV_STRONG_ALIGNMENT
    ਅਚਲ size_t alignMark = ((size_t)src)|((size_t)dst)|sstep|dstep;
#ਨਹੀਂਤਾਂ
    ਅਚਲ size_t alignMark = 0; // unaligned access is allowed: all the checks below fold to true
#ਜੇ_ਅੰਤ

    // SIMD-optimized dispatch
    ਚੋਣ(esz)
    {
        ਹਾਲਤ 1:                                                                               // CV_8UC1: 8-bit, 1 channel
            flipHoriz_single<v_uint8>(src, sstep, dst, dstep, size); ਵਾਪਸ;
        ਹਾਲਤ 2:                                                                               // CV_8UC2, CV_16UC1: 8-bit 2-channel or 16-bit 1-channel
            ਜੇ (isAligned<2>(alignMark)) { flipHoriz_single<v_uint16>(src, sstep, dst, dstep, size); ਵਾਪਸ; }
            ਰੋਕੋ;
        ਹਾਲਤ 3:                                                                               // CV_8UC3: 8-bit, 3 channels
            flipHoriz_c3<v_uint8>(src, sstep, dst, dstep, size); ਵਾਪਸ;
        ਹਾਲਤ 4:                                                                               // CV_8UC4, CV_16UC2, CV_32SC1, CV_32FC1: 8-bit 4-channel, 16-bit 2-channel, or 32-bit 1-channel
            ਜੇ (isAligned<4>(alignMark)) { flipHoriz_single<v_uint32>(src, sstep, dst, dstep, size); ਵਾਪਸ; }
            ਰੋਕੋ;
        ਹਾਲਤ 6:                                                                               // CV_16UC3, CV_16SC3: 16-bit, 3 channels
            ਜੇ (isAligned<2>(alignMark)) { flipHoriz_c3<v_uint16>(src, sstep, dst, dstep, size); ਵਾਪਸ; }
            ਰੋਕੋ;
        ਹਾਲਤ 8:                                                                               // CV_16UC4, CV_32SC2, CV_32FC2, CV_64FC1: 16-bit 4-channel, 32-bit 2-channel, or 64-bit 1-channel
            ਜੇ (isAligned<8>(alignMark)) { flipHoriz_single<v_uint64>(src, sstep, dst, dstep, size); ਵਾਪਸ; }
            ਰੋਕੋ;
        ਹਾਲਤ 12:                                                                              // CV_32SC3, CV_32FC3: 32-bit, 3 channels
            ਜੇ (isAligned<4>(alignMark)) { flipHoriz_c3<v_uint32>(src, sstep, dst, dstep, size); ਵਾਪਸ; }
            ਰੋਕੋ;
        ਹਾਲਤ 16:                                                                              // CV_32SC4, CV_32FC4, CV_64FC2: 32-bit 4-channel or 64-bit 2-channel
            flipHoriz_vlanes_dispatch<16>(src, sstep, dst, dstep, size); ਵਾਪਸ;               // byte-wise access only
#ਜੇ CV_SIMD128
        ਹਾਲਤ 24:                                                                              // CV_64FC3: 64-bit, 3 channels
            ਜੇ (isAligned<8>(alignMark)) { flipHoriz_24(src, sstep, dst, dstep, size); ਵਾਪਸ; }
            ਰੋਕੋ;
#ਜੇ_ਅੰਤ
        ਹਾਲਤ 32:                                                                              // CV_64FC4: 64-bit, 4 channels
            flipHoriz_vlanes_dispatch<32>(src, sstep, dst, dstep, size); ਵਾਪਸ;               // byte-wise access only
        ਮੂਲ:
            ਰੋਕੋ; // Fall through to generic implementation
    }
#ਜੇ_ਅੰਤ
    // Fallback: generic scalar
    flipHoriz_generic(src, sstep, dst, dstep, size, esz);
}

ਸਥਿਰ ਖਾਲੀ
flipVert( ਅਚਲ uchar* src0, size_t sstep, uchar* dst0, size_t dstep, ਮਾਪ size, size_t esz )
{
    ਅਚਲ uchar* src1 = src0 + (size.ਉਚਾਈ - 1)*sstep;
    uchar* dst1 = dst0 + (size.ਉਚਾਈ - 1)*dstep;
    size.ਚੌੜਾਈ *= (ਪੂਰਨ)esz;

    ਲਈ( ਪੂਰਨ y = 0; y < (size.ਉਚਾਈ + 1)/2; y++, src0 += sstep, src1 -= sstep,
                                                  dst0 += dstep, dst1 -= dstep )
    {
        ਪੂਰਨ i = 0;
#ਜੇ (CV_SIMD || CV_SIMD_SCALABLE)
#ਜੇ CV_STRONG_ALIGNMENT
        ਜੇ (isAligned<ਆਕਾਰ(ਪੂਰਨ)>(src0, src1, dst0, dst1))
#ਜੇ_ਅੰਤ
        {
            ਲਈ (; i <= size.ਚੌੜਾਈ - VTraits<v_uint8>::vlanes(); i += VTraits<v_uint8>::vlanes())
            {
                v_int32 t0 = v_reinterpret_as_s32(vx_ਲੱਦੋ(src0 + i));
                v_int32 t1 = v_reinterpret_as_s32(vx_ਲੱਦੋ(src1 + i));
                v_store(dst0 + i, v_reinterpret_as_u8(t1));
                v_store(dst1 + i, v_reinterpret_as_u8(t0));
            }
        }
#ਜੇ CV_STRONG_ALIGNMENT
        ਨਹੀਂਤਾਂ
        {
            ਲਈ (; i <= size.ਚੌੜਾਈ - VTraits<v_uint8>::vlanes(); i += VTraits<v_uint8>::vlanes())
            {
                v_uint8 t0 = vx_ਲੱਦੋ(src0 + i);
                v_uint8 t1 = vx_ਲੱਦੋ(src1 + i);
                v_store(dst0 + i, t1);
                v_store(dst1 + i, t0);
            }
        }
#ਜੇ_ਅੰਤ
#ਜੇ_ਅੰਤ

        ਜੇ (isAligned<ਆਕਾਰ(ਪੂਰਨ)>(src0, src1, dst0, dst1))
        {
            ਲਈ( ; i <= size.ਚੌੜਾਈ - 16; i += 16 )
            {
                ਪੂਰਨ t0 = ((ਪੂਰਨ*)(src0 + i))[0];
                ਪੂਰਨ t1 = ((ਪੂਰਨ*)(src1 + i))[0];

                ((ਪੂਰਨ*)(dst0 + i))[0] = t1;
                ((ਪੂਰਨ*)(dst1 + i))[0] = t0;

                t0 = ((ਪੂਰਨ*)(src0 + i))[1];
                t1 = ((ਪੂਰਨ*)(src1 + i))[1];

                ((ਪੂਰਨ*)(dst0 + i))[1] = t1;
                ((ਪੂਰਨ*)(dst1 + i))[1] = t0;

                t0 = ((ਪੂਰਨ*)(src0 + i))[2];
                t1 = ((ਪੂਰਨ*)(src1 + i))[2];

                ((ਪੂਰਨ*)(dst0 + i))[2] = t1;
                ((ਪੂਰਨ*)(dst1 + i))[2] = t0;

                t0 = ((ਪੂਰਨ*)(src0 + i))[3];
                t1 = ((ਪੂਰਨ*)(src1 + i))[3];

                ((ਪੂਰਨ*)(dst0 + i))[3] = t1;
                ((ਪੂਰਨ*)(dst1 + i))[3] = t0;
            }

            ਲਈ( ; i <= size.ਚੌੜਾਈ - 4; i += 4 )
            {
                ਪੂਰਨ t0 = ((ਪੂਰਨ*)(src0 + i))[0];
                ਪੂਰਨ t1 = ((ਪੂਰਨ*)(src1 + i))[0];

                ((ਪੂਰਨ*)(dst0 + i))[0] = t1;
                ((ਪੂਰਨ*)(dst1 + i))[0] = t0;
            }
        }

        ਲਈ( ; i < size.ਚੌੜਾਈ; i++ )
        {
            uchar t0 = src0[i];
            uchar t1 = src1[i];

            dst0[i] = t1;
            dst1[i] = t0;
        }
    }
}

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_OPENCL

ਗਿਣਤੀ { FLIP_ਖਾਨੇ_ਗਿਣਤੀ = 1 << 0, FLIP_ਪੰਕਤੀਆਂ = 1 << 1, FLIP_BOTH = FLIP_ਪੰਕਤੀਆਂ | FLIP_ਖਾਨੇ_ਗਿਣਤੀ };

ਸਥਿਰ ਤਾਰਕਿਕ ocl_flip(ਨਿਵੇਸ਼_Array _src, OutputArray _dst, ਪੂਰਨ flip_ਕੋਡ )
{
    CV_Assert(flip_ਕੋਡ >= -1 && flip_ਕੋਡ <= 1);

    ਅਚਲ ocl::Device & dev = ocl::Device::getDefault();
    ਪੂਰਨ type = _src.type(), ਡੂੰਘਾਈ = CV_MAT_ਡੂੰਘਾਈ(type), cn = CV_MAT_CN(type),
            flipType, kercn = std::min(ocl::predict_Optimal_ਸਦਿਸ਼_ਚੌੜਾਈ(_src, _dst), 4);

    ਤਾਰਕਿਕ doubleSupport = dev.doubleFPConfig() > 0;
    ਜੇ (!doubleSupport && ਡੂੰਘਾਈ == CV_64F)
        kercn = cn;

    ਜੇ (cn > 4)
        ਵਾਪਸ ਝੂਠ;

    ਮਾਪ size = _src.size();
    _dst.create(size, type);
    UMat src = _src.getUMat(), dst = _dst.getUMat();
    ਤਾਰਕਿਕ inplace = (dst.u == src.u);

    String kernelName;
    ਜੇ (flip_ਕੋਡ == 0)
        kernelName = "arithm_flip_rows", flipType = FLIP_ਪੰਕਤੀਆਂ;
    ਨਹੀਂਤਾਂ ਜੇ (flip_ਕੋਡ > 0)
        kernelName = "arithm_flip_cols", flipType = FLIP_ਖਾਨੇ_ਗਿਣਤੀ;
    ਨਹੀਂਤਾਂ
        kernelName = "arithm_flip_rows_cols", flipType = FLIP_BOTH;

    ਜੇ(inplace)
        kernelName += "_inplace";

    ਪੂਰਨ px_ਪ੍ਰਤੀ_WIy = (dev.isIntel() && (dev.type() & ocl::Device::TYPE_GPU)) ? 4 : 1;
    kercn = (cn!=3 || flipType == FLIP_ਪੰਕਤੀਆਂ) ? std::max(kercn, cn) : cn;
    ਅਚਲ ਪੂਰਨ TILE_ਮਾਪ = 32, ਖੰਡ_ਪੰਕਤੀਆਂ = 8;

    ocl::Kernel k(kernelName.c_str(), ocl::core::flip_oclsrc,
                format( "-D T=%s -D T1=%s -D DEPTH=%d -D cn=%d -D PIX_PER_WI_Y=%d -D kercn=%d -D TILE_SIZE=%d -D BLOCK_ROWS=%d%s",
                kercn != cn ? ocl::type_ਮੰਜ਼ਿਲ_Str(CV_MAKE_TYPE(ਡੂੰਘਾਈ, kercn)) : ocl::vecop_Type_ਮੰਜ਼ਿਲ_Str(CV_MAKE_TYPE(ਡੂੰਘਾਈ, kercn)),
                kercn != cn ? ocl::type_ਮੰਜ਼ਿਲ_Str(ਡੂੰਘਾਈ) : ocl::vecop_Type_ਮੰਜ਼ਿਲ_Str(ਡੂੰਘਾਈ), ਡੂੰਘਾਈ, cn, px_ਪ੍ਰਤੀ_WIy, kercn, TILE_ਮਾਪ, ਖੰਡ_ਪੰਕਤੀਆਂ,
                inplace ? " -D INPLACE" : ""));
    ਜੇ (k.empty())
        ਵਾਪਸ ਝੂਠ;

    ਪੂਰਨ ਖਾਨੇ_ਗਿਣਤੀ = size.ਚੌੜਾਈ * cn / kercn, ਪੰਕਤੀਆਂ = size.ਉਚਾਈ;
    ਪੂਰਨ work_ਖਾਨੇ_ਗਿਣਤੀ = flipType == FLIP_ਖਾਨੇ_ਗਿਣਤੀ ? (ਖਾਨੇ_ਗਿਣਤੀ + 1) >> 1 : ਖਾਨੇ_ਗਿਣਤੀ;
    ਪੂਰਨ work_ਪੰਕਤੀਆਂ = flipType & FLIP_ਪੰਕਤੀਆਂ ? (ਪੰਕਤੀਆਂ + 1) >> 1 : ਪੰਕਤੀਆਂ;

    ਜੇ (inplace)
    {
        k.ਦਲੀਲਾਂ(ocl::KernelArg::Read_Write_No_ਮਾਪ(dst), ਪੰਕਤੀਆਂ, ਖਾਨੇ_ਗਿਣਤੀ);

        ਪੂਰਨ gs_ਖਾਨੇ_ਗਿਣਤੀ, gs_ਪੰਕਤੀਆਂ;
        ਜੇ (flipType == FLIP_ਖਾਨੇ_ਗਿਣਤੀ)
        {
            gs_ਖਾਨੇ_ਗਿਣਤੀ = work_ਖਾਨੇ_ਗਿਣਤੀ;
            gs_ਪੰਕਤੀਆਂ = ਪੰਕਤੀਆਂ;
        }
        ਨਹੀਂਤਾਂ ਜੇ (flipType == FLIP_ਪੰਕਤੀਆਂ)
        {
            gs_ਖਾਨੇ_ਗਿਣਤੀ = ਖਾਨੇ_ਗਿਣਤੀ;
            gs_ਪੰਕਤੀਆਂ = work_ਪੰਕਤੀਆਂ;
        }
        ਨਹੀਂਤਾਂ // FLIP_BOTH
        {
            gs_ਖਾਨੇ_ਗਿਣਤੀ = ਖਾਨੇ_ਗਿਣਤੀ;
            gs_ਪੰਕਤੀਆਂ = ਪੰਕਤੀਆਂ;
        }

        size_t globalsize[2] = {
            (size_t)div_ਉੱਪਰ(gs_ਖਾਨੇ_ਗਿਣਤੀ, TILE_ਮਾਪ) * TILE_ਮਾਪ,
            (size_t)div_ਉੱਪਰ(gs_ਪੰਕਤੀਆਂ, TILE_ਮਾਪ) * ਖੰਡ_ਪੰਕਤੀਆਂ
        };
        size_t localsize[2] = { TILE_ਮਾਪ, ਖੰਡ_ਪੰਕਤੀਆਂ };
        ਵਾਪਸ k.ਲੜੀਵਾਰ(2, globalsize, localsize, ਝੂਠ);
    }
    ਨਹੀਂਤਾਂ
    {
        k.ਦਲੀਲਾਂ(ocl::KernelArg::Read_Only_No_ਮਾਪ(src),
           ocl::KernelArg::WriteOnly(dst, cn, kercn), work_ਪੰਕਤੀਆਂ, work_ਖਾਨੇ_ਗਿਣਤੀ);

        size_t max_Work_ਸਮੂਹ_ਮਾਪ = dev.max_Work_ਸਮੂਹ_ਮਾਪ();
        CV_Assert(max_Work_ਸਮੂਹ_ਮਾਪ % 4 == 0);

        size_t globalsize[2] = { (size_t)work_ਖਾਨੇ_ਗਿਣਤੀ, ((size_t)work_ਪੰਕਤੀਆਂ + px_ਪ੍ਰਤੀ_WIy - 1) / px_ਪ੍ਰਤੀ_WIy };
        size_t localsize[2]  = { max_Work_ਸਮੂਹ_ਮਾਪ / 4, 4 };
        ਵਾਪਸ k.ਲੜੀਵਾਰ(2, globalsize, (flipType == FLIP_ਖਾਨੇ_ਗਿਣਤੀ) && !dev.isIntel() ? localsize : ਨਿਰਾ, ਝੂਠ);
    }
}

#ਜੇ_ਅੰਤ

ਖਾਲੀ flip( ਨਿਵੇਸ਼_Array _src, OutputArray _dst, ਪੂਰਨ flip_ਬਹੁਲਕ )
{
    CV_INSTRUMENT_REGION();

    CV_Assert( _src.dims() <= 2 );
    ਮਾਪ size = _src.size();

    ਜੇ (flip_ਬਹੁਲਕ < 0)
    {
        ਜੇ (size.ਚੌੜਾਈ == 1)
            flip_ਬਹੁਲਕ = 0;
        ਜੇ (size.ਉਚਾਈ == 1)
            flip_ਬਹੁਲਕ = 1;
    }

    ਜੇ ((size.ਚੌੜਾਈ == 1 && flip_ਬਹੁਲਕ > 0) ||
        (size.ਉਚਾਈ == 1 && flip_ਬਹੁਲਕ == 0))
    {
        ਵਾਪਸ _src.copy_ਮੰਜ਼ਿਲ(_dst);
    }

    CV_OCL_ਲੜੀਵਾਰ( _dst.isUMat(), ocl_flip(_src, _dst, flip_ਬਹੁਲਕ))

    Mat src = _src.getMat();
    ਪੂਰਨ type = src.type();
    _dst.create( size, type );
    Mat dst = _dst.getMat();

    CALL_HAL(flip, cv_hal_flip, type, src.ptr(), src.ਕਦਮ, src.ਖਾਨੇ_ਗਿਣਤੀ, src.ਪੰਕਤੀਆਂ,
             dst.ptr(), dst.ਕਦਮ, flip_ਬਹੁਲਕ);

    size_t esz = CV_ELEM_ਮਾਪ(type);

    ਜੇ( flip_ਬਹੁਲਕ <= 0 )
        flipVert( src.ptr(), src.ਕਦਮ, dst.ptr(), dst.ਕਦਮ, src.size(), esz );
    ਨਹੀਂਤਾਂ
        flipHoriz( src.ptr(), src.ਕਦਮ, dst.ptr(), dst.ਕਦਮ, src.size(), esz );

    ਜੇ( flip_ਬਹੁਲਕ < 0 )
        flipHoriz( dst.ptr(), dst.ਕਦਮ, dst.ptr(), dst.ਕਦਮ, dst.size(), esz );
}

// In-place flip of a continuous array: swap the slices along the axis, no temporary copy.
ਸਥਿਰ ਖਾਲੀ flipNDInplace(Mat& m, ਪੂਰਨ axis)
{
    size_t nouter = 1;
    ਲਈ (ਪੂਰਨ i = 0; i < axis; i++)
        nouter *= (size_t)m.size[i];
    ਅਚਲ ਪੂਰਨ n = m.size[axis];
    ਅਚਲ size_t ਕਦਮ = m.ਕਦਮ[axis];
    uchar* ਅੰਕੜੇ = m.ptr();
    ਲਈ (size_t i = 0; i < nouter; i++, ਅੰਕੜੇ += n*ਕਦਮ)
        ਲਈ (ਪੂਰਨ j = 0, k = n - 1; j < k; j++, k--)
            std::swap_ranges(ਅੰਕੜੇ + j*ਕਦਮ, ਅੰਕੜੇ + (j + 1)*ਕਦਮ, ਅੰਕੜੇ + k*ਕਦਮ);
}

ਖਾਲੀ flipND(ਨਿਵੇਸ਼_Array _src, OutputArray _dst, ਪੂਰਨ _axis)
{
    CV_INSTRUMENT_REGION();

    Mat src = _src.getMat();

    // verify axis
    ਪੂਰਨ ndim = src.dims;
    CV_CheckLT(_axis, ndim, "flipND: given axis is out of range");
    CV_CheckGE(_axis, -ndim, "flipND: given axis is out of range");
    ਪੂਰਨ axis = (_axis + ndim) % ndim;

    _dst.create(ndim, src.size.p, src.type());
    Mat dst = _dst.getMat();
    ਜੇ (dst.ਅੰਕੜੇ == src.ਅੰਕੜੇ && src.isContinuous() && dst.isContinuous())
    {
        flipNDInplace(dst, axis);
        ਵਾਪਸ;
    }

    nd::View v = nd::view_ਦਾ(src);
    nd::flip(v, axis);
    nd::copy(v, nd::view_ਦਾ(dst));
}

ਖਾਲੀ concat(ਨਿਵੇਸ਼_Array_ਦਾ_Arrays _src, ਪੂਰਨ axis, OutputArray _dst)
{
    CV_INSTRUMENT_REGION();

    std::vector<Mat> src;
    _src.get_Mat_ਸਦਿਸ਼(src);
    CV_Assert(!src.empty());

    ਅਚਲ Mat& src0 = src[0];
    ਅਚਲ ਪੂਰਨ ndims = src0.dims, type = src0.type(), ninputs = (ਪੂਰਨ)src.size();
    CV_CheckGE(ndims, 1, "concat: 0-dimensional arrays are not supported");
    CV_CheckGE(axis, -ndims, "concat: axis is out of range");
    CV_CheckLT(axis, ndims, "concat: axis is out of range");
    ਜੇ (axis < 0)
        axis += ndims;

    ਪੂਰਨ ਬਾਹਰਲਾ_Shape[CV_MAX_DIM];
    ਲਈ (ਪੂਰਨ i = 0; i < ndims; i++)
        ਬਾਹਰਲਾ_Shape[i] = src0.size[i];
    int64 axis_ਮਾਪ = 0;
    ਲਈ (ਪੂਰਨ k = 0; k < ninputs; k++)
    {
        ਅਚਲ Mat& m = src[k];
        CV_CheckEQ(m.type(), type, "concat: all the input arrays must have the same type");
        CV_CheckEQ(m.dims, ndims, "concat: all the input arrays must have the same number of dimensions");
        ਲਈ (ਪੂਰਨ i = 0; i < ndims; i++)
            ਜੇ (i != axis)
                CV_CheckEQ(m.size[i], ਬਾਹਰਲਾ_Shape[i], "concat: the input shapes must match except along the axis");
        axis_ਮਾਪ += m.size[axis];
    }
    CV_CheckLE((size_t)axis_ਮਾਪ, (size_t)ਵੱਧ_ਤੋਂ_ਵੱਧ_ਅੰਕ, "concat: the output is too big");
    ਬਾਹਰਲਾ_Shape[axis] = (ਪੂਰਨ)axis_ਮਾਪ;

    _dst.create(ndims, ਬਾਹਰਲਾ_Shape, type);
    Mat dst = _dst.getMat();

    Auto_ਬਫਰ<nd::View, 4> sv(ninputs), dv(ninputs);
    ਅਚਲ nd::View dst0 = nd::view_ਦਾ(dst);
    ਪੂਰਨ ofs = 0;
    ਲਈ (ਪੂਰਨ k = 0; k < ninputs; k++)
    {
        ਅਚਲ ਪੂਰਨ sz = src[k].size[axis];
        sv[k] = nd::view_ਦਾ(src[k]);
        dv[k] = dst0;
        nd::ਟੁਕੜਾ_2(dv[k], axis, ofs, 1, sz);
        ofs += sz;
    }
    nd::copyBatch(sv.ਅੰਕੜੇ(), dv.ਅੰਕੜੇ(), ninputs);
}

ਖਾਲੀ ਵੰਡੋ(ਨਿਵੇਸ਼_Array _src, ਪੂਰਨ axis, ਅਚਲ std::vector<ਪੂਰਨ>& sizes, Output_Array_ਦਾ_Arrays _dst)
{
    CV_INSTRUMENT_REGION();

    Mat src = _src.getMat();
    ਅਚਲ ਪੂਰਨ ndims = src.dims, type = src.type(), noutputs = (ਪੂਰਨ)sizes.size();
    CV_CheckGE(ndims, 1, "split: 0-dimensional arrays are not supported");
    CV_CheckGE(axis, -ndims, "split: axis is out of range");
    CV_CheckLT(axis, ndims, "split: axis is out of range");
    ਜੇ (axis < 0)
        axis += ndims;
    CV_Assert(noutputs > 0);

    int64 axis_ਮਾਪ = 0;
    ਲਈ (ਪੂਰਨ k = 0; k < noutputs; k++)
    {
        CV_CheckGE(sizes[k], 0, "split: the sizes must be non-negative");
        axis_ਮਾਪ += sizes[k];
    }
    CV_CheckEQ((size_t)axis_ਮਾਪ, (size_t)src.size[axis], "split: the sizes must sum up to the size of the axis");

    ਪੂਰਨ ਬਾਹਰਲਾ_Shape[CV_MAX_DIM];
    ਲਈ (ਪੂਰਨ i = 0; i < ndims; i++)
        ਬਾਹਰਲਾ_Shape[i] = src.size[i];

    _dst.create(noutputs, 1, type, -1, ਸੱਚ);
    Auto_ਬਫਰ<nd::View, 4> sv(noutputs), dv(noutputs);
    std::vector<Mat> dst(noutputs);
    ਅਚਲ nd::View src0 = nd::view_ਦਾ(src);
    ਪੂਰਨ ofs = 0;
    ਲਈ (ਪੂਰਨ k = 0; k < noutputs; k++)
    {
        ਬਾਹਰਲਾ_Shape[axis] = sizes[k];
        _dst.create(ndims, ਬਾਹਰਲਾ_Shape, type, k);
        dst[k] = _dst.getMat(k);
        sv[k] = src0;
        nd::ਟੁਕੜਾ_2(sv[k], axis, ofs, 1, sizes[k]);
        dv[k] = nd::view_ਦਾ(dst[k]);
        ofs += sizes[k];
    }
    nd::copyBatch(sv.ਅੰਕੜੇ(), dv.ਅੰਕੜੇ(), noutputs);
}

ਖਾਲੀ repeat(ਨਿਵੇਸ਼_Array _src, ਅਚਲ std::vector<ਪੂਰਨ>& repeats, OutputArray _dst)
{
    CV_INSTRUMENT_REGION();

    Mat src = _src.getMat();
    ਅਚਲ ਪੂਰਨ ndims = src.dims;
    CV_Assert(!(ndims == 0 && src.empty()));
    CV_CheckEQ(repeats.size(), (size_t)ndims, "repeat: the number of repeats must match the number of dimensions");

    ਪੂਰਨ ਬਾਹਰਲਾ_Shape[CV_MAX_DIM];
    ਲਈ (ਪੂਰਨ i = 0; i < ndims; i++)
    {
        CV_CheckGE(repeats[i], 0, "repeat: the repeats must be non-negative");
        int64 sz = (int64)src.size[i]*repeats[i];
        CV_CheckLE((size_t)sz, (size_t)ਵੱਧ_ਤੋਂ_ਵੱਧ_ਅੰਕ, "repeat: the output is too big");
        ਬਾਹਰਲਾ_Shape[i] = (ਪੂਰਨ)sz;
    }

    _dst.create(ndims, ਬਾਹਰਲਾ_Shape, src.type());
    Mat dst = _dst.getMat();
    ਜੇ (dst.ਕੁੱਲ() == 0 || src.empty())
        ਵਾਪਸ;

    // Each repeated output axis is split into (repeat index, position), the former
    // with source step 0, so the whole tile is one broadcasting copy.
    nd::View sv, dv;
    sv.ਅੰਕੜੇ = src.ਅੰਕੜੇ;
    dv.ਅੰਕੜੇ = dst.ਅੰਕੜੇ;
    sv.esz = dv.esz = src.elem_ਮਾਪ();
    ਪੂਰਨ n = 0;
    ਲਈ (ਪੂਰਨ i = 0; i < ndims; i++)
    {
        ਅਚਲ ਪੂਰਨ sz = src.size[i], r = repeats[i];
        ਜੇ (r != 1)
        {
            CV_CheckLT(n, (ਪੂਰਨ)nd::MAX_VIEW_DIMS, "repeat: too many dimensions");
            sv.size[n] = dv.size[n] = r;
            sv.ਕਦਮ[n] = 0;
            dv.ਕਦਮ[n] = (ptrdiff_t)dst.ਕਦਮ[i]*sz;
            n++;
        }
        ਜੇ (sz != 1 || r == 1)
        {
            CV_CheckLT(n, (ਪੂਰਨ)nd::MAX_VIEW_DIMS, "repeat: too many dimensions");
            sv.size[n] = dv.size[n] = sz;
            sv.ਕਦਮ[n] = (ptrdiff_t)src.ਕਦਮ[i];
            dv.ਕਦਮ[n] = (ptrdiff_t)dst.ਕਦਮ[i];
            n++;
        }
    }
    sv.dims = dv.dims = n;
    nd::copy(sv, dv);
}

ਖਾਲੀ ਟੁਕੜਾ_2(ਨਿਵੇਸ਼_Array _src, ਅਚਲ std::vector<ਪੂਰਨ>& starts, ਅਚਲ std::vector<ਪੂਰਨ>& ends,
             ਅਚਲ std::vector<ਪੂਰਨ>& ਕਦਮਾਂ, OutputArray _dst)
{
    CV_INSTRUMENT_REGION();

    Mat src = _src.getMat();
    ਅਚਲ ਪੂਰਨ ndims = src.dims, nslices = (ਪੂਰਨ)starts.size();
    CV_Assert(!(ndims == 0 && src.empty()));
    CV_CheckLE(nslices, ndims, "slice: too many starts");
    CV_CheckEQ(ends.size(), starts.size(), "slice: starts and ends must have the same size");
    CV_Check(ਕਦਮਾਂ.size(), ਕਦਮਾਂ.empty() || ਕਦਮਾਂ.size() == starts.size(),
             "slice: steps must be empty or have the same size as starts");

    nd::View sv = nd::view_ਦਾ(src);
    ਪੂਰਨ ਬਾਹਰਲਾ_Shape[CV_MAX_DIM];
    ਲਈ (ਪੂਰਨ i = 0; i < ndims; i++)
        ਬਾਹਰਲਾ_Shape[i] = src.size[i];

    ਲਈ (ਪੂਰਨ i = 0; i < nslices; i++)
    {
        ਅਚਲ ਪੂਰਨ n = src.size[i], ਸ਼ੁਰੂ = starts[i], end = ends[i];
        ਅਚਲ ਪੂਰਨ ਕਦਮ = ਕਦਮਾਂ.empty() ? 1 : ਕਦਮਾਂ[i];
        CV_CheckNE(ਕਦਮ, 0, "slice: step must not be zero");
        ਪੂਰਨ ਗਿਣਤੀ_3;
        ਜੇ (ਕਦਮ > 0)
        {
            CV_Check(ਸ਼ੁਰੂ, 0 <= ਸ਼ੁਰੂ && ਸ਼ੁਰੂ <= n, "slice: start is out of range");
            CV_Check(end, 0 <= end && end <= n, "slice: end is out of range");
            ਗਿਣਤੀ_3 = end > ਸ਼ੁਰੂ ? (ਪੂਰਨ)(((int64)end - ਸ਼ੁਰੂ + ਕਦਮ - 1)/ਕਦਮ) : 0;
        }
        ਨਹੀਂਤਾਂ
        {
            CV_Check(ਸ਼ੁਰੂ, -1 <= ਸ਼ੁਰੂ && ਸ਼ੁਰੂ < n, "slice: start is out of range");
            CV_Check(end, -1 <= end && end < n, "slice: end is out of range");
            ਗਿਣਤੀ_3 = ਸ਼ੁਰੂ > end ? (ਪੂਰਨ)(((int64)ਸ਼ੁਰੂ - end - ਕਦਮ - 1)/(-ਕਦਮ)) : 0;
        }
        nd::ਟੁਕੜਾ_2(sv, i, ਸ਼ੁਰੂ, ਕਦਮ, ਗਿਣਤੀ_3);
        ਬਾਹਰਲਾ_Shape[i] = ਗਿਣਤੀ_3;
    }

    _dst.create(ndims, ਬਾਹਰਲਾ_Shape, src.type());
    Mat dst = _dst.getMat();
    ਜੇ (dst.ਕੁੱਲ() == 0 || src.empty())
        ਵਾਪਸ;
    nd::copy(sv, nd::view_ਦਾ(dst));
}

/*
    This function first prepends 1 to each tensor shape to have a common max_ndims dimension, then flatten non-broadcast dimensions.
*/
ਸਥਿਰ ਤਾਰਕਿਕ flatten_for_ਪ੍ਰਸਾਰਣ(ਪੂਰਨ narrays, ਪੂਰਨ max_ndims, ਅਚਲ ਪੂਰਨ* ndims, ਅਚਲ ਪੂਰਨ** orig_shape,
                                   ਪੂਰਨ** flatten_shape, size_t** flatten_ਕਦਮ) {
    ਪੂਰਨ i, j, k;

    // step 1.
    // * make all inputs and the output max_ndims-dimensional.
    // * compute proper step's
    ਲਈ (i = max_ndims - 1; i >= 0; i-- ) {
        ਲਈ (k = 0; k < narrays; k++) {
            j = ndims[k] - (max_ndims - i);
            ਪੂਰਨ sz_i = j >= 0 ? orig_shape[k][j] : 1;
            size_t st_i = i == max_ndims - 1 ? 1 : flatten_ਕਦਮ[k][i+1] * flatten_shape[k][i+1];
            flatten_shape[k][i] = sz_i;
            flatten_ਕਦਮ[k][i] = st_i;
            ਜੇ (flatten_shape[k][i] == 0)
                ਵਾਪਸ ਝੂਠ;
        }
    }

    // step 2. Let's do the flattening first,
    // since we'd need proper values of steps to check continuity.
    // this loop is probably the most tricky part
    // in the whole implementation of broadcasting.
    j = max_ndims-1;
    ਲਈ (i = j - 1; i >= 0; i--) {
        ਤਾਰਕਿਕ all_contiguous = ਸੱਚ, all_scalars = ਸੱਚ, all_consistent = ਸੱਚ;
        ਲਈ(k = 0; k < narrays; k++) {
            size_t st = flatten_ਕਦਮ[k][j] * flatten_shape[k][j];
            ਤਾਰਕਿਕ prev_scalar = flatten_shape[k][j] == 1;
            ਤਾਰਕਿਕ scalar = flatten_shape[k][i] == 1;
            all_contiguous = all_contiguous && (st == flatten_ਕਦਮ[k][i]);
            all_scalars = all_scalars && scalar;
            all_consistent = all_consistent && (scalar == prev_scalar);
        }
        ਜੇ (all_contiguous && (all_consistent || all_scalars)) {
            ਲਈ(k = 0; k < narrays; k++)
                flatten_shape[k][j] *= flatten_shape[k][i];
        } ਨਹੀਂਤਾਂ {
            j--;
            ਜੇ (i < j) {
                ਲਈ(k = 0; k < narrays; k++) {
                    flatten_shape[k][j] = flatten_shape[k][i];
                    flatten_ਕਦਮ[k][j] = flatten_ਕਦਮ[k][i];
                }
            }
        }
    }

    // step 3. Set some step's to 0's.
    ਲਈ (i = max_ndims-1; i >= j; i--) {
        ਲਈ (k = 0; k < narrays; k++)
            flatten_ਕਦਮ[k][i] = flatten_shape[k][i] == 1 ? 0 : flatten_ਕਦਮ[k][i];
    }
    ਲਈ (; i >= 0; i--) {
        ਲਈ (k = 0; k < narrays; k++) {
            flatten_ਕਦਮ[k][i] = 0;
            flatten_shape[k][i] = 1;
        }
    }
    ਵਾਪਸ ਸੱਚ;
}

ਖਾਲੀ ਪ੍ਰਸਾਰਣ(ਨਿਵੇਸ਼_Array _src, ਨਿਵੇਸ਼_Array _shape, OutputArray _dst) {
    CV_INSTRUMENT_REGION();

    Mat src = _src.getMat();
    CV_CheckTrue(src.isContinuous(), "broadcast: input array must be contiguous");
    CV_CheckChannelsEQ(src.channels(), 1, "broadcast: input array must be single channel");

    Mat shape = _shape.getMat();
    CV_CheckTypeEQ(shape.type(), CV_32S, "broadcast: target shape must be of type int32");
    ਅਚਲ ਸਵੈਚਾਲਿਤ dims_shape = static_cast<ਪੂਰਨ>(shape.ਕੁੱਲ());
    ਅਚਲ ਸਵੈਚਾਲਿਤ *ptr_shape = shape.ptr<ਪੂਰਨ>();

    // check valid shape, 1D/0D Mat would fail in the following checks
    ਅਚਲ ਸਵੈਚਾਲਿਤ dims_src = src.dims;
    CV_CheckLE(dims_src, dims_shape,
               "broadcast: dimension of input array must be less than or equal to dimension of target shape");
    std::vector<ਪੂਰਨ> shape_src{src.size.p, src.size.p + dims_src};
    ਜੇ (shape_src.size() < static_cast<size_t>(dims_shape)) {
        shape_src.insert(shape_src.begin(), dims_shape - shape_src.size(), 1);
    }
    ਲਈ (ਪੂਰਨ i = 0; i < static_cast<ਪੂਰਨ>(shape_src.size()); ++i) {
        ਅਚਲ ਸਵੈਚਾਲਿਤ *shape_ਨਿਸ਼ਾਨਾ = ptr_shape;
        ਜੇ (shape_src[i] != 1) {
            CV_CheckEQ(shape_src[i], shape_ਨਿਸ਼ਾਨਾ[i], "target shape must be equal to input shape or 1");
        }
    }

    // impl
    _dst.create(dims_shape, shape.ptr<ਪੂਰਨ>(), src.type());
    Mat dst = _dst.getMat();
    ਜੇ (dst.ਕੁੱਲ() == 0)
        ਵਾਪਸ;
    cv::Auto_ਬਫਰ<ਪੂਰਨ> is_same_shape(dims_shape, 0);
    ਲਈ (ਪੂਰਨ i = 0; i < static_cast<ਪੂਰਨ>(shape_src.size()); ++i) {
        ਜੇ (shape_src[i] == ptr_shape[i]) {
            is_same_shape[i] = 1;
        }
    }
    // copy if same shape
    ਜੇ (std::accumulate(is_same_shape.begin(), is_same_shape.end(), 1, std::multiplies<ਪੂਰਨ>()) != 0) {
        ਅਚਲ ਸਵੈਚਾਲਿਤ *p_src = src.ptr<ਅਚਲ ਅੱਖਰ>();
        ਸਵੈਚਾਲਿਤ *p_dst = dst.ptr<ਅੱਖਰ>();
        std::ਸਮ੍ਰਿਤੀ_ਨਕਲ(p_dst, p_src, dst.ਕੁੱਲ() * dst.elem_ਮਾਪ());
        ਵਾਪਸ;
    }
    // other cases
    ਪੂਰਨ max_ndims = std::max(dims_src, dims_shape);
    ਜੇ (max_ndims < 2 && src.ਕੁੱਲ() == 1) {
        ਅਚਲ ਅੱਖਰ* p_src = src.ptr<ਅਚਲ ਅੱਖਰ>();
        ਅੱਖਰ* p_dst = dst.ptr<ਅੱਖਰ>();
        size_t esz = src.elem_ਮਾਪ();
        ਲਈ (size_t j = 0; j < dst.ਕੁੱਲ(); j++)
            std::ਸਮ੍ਰਿਤੀ_ਨਕਲ(p_dst + j * esz, p_src, esz);
        ਵਾਪਸ;
    }
    ਅਚਲ ਪੂਰਨ all_ndims[2] = {src.dims, dst.dims};
    ਅਚਲ ਪੂਰਨ* orig_shapes[2] = {src.size.p, dst.size.p};
    cv::Auto_ਬਫਰ<size_t> buff(max_ndims * 4);
    ਪੂਰਨ* flatten_shapes[2] = {(ਪੂਰਨ*)buff.ਅੰਕੜੇ(), (ਪੂਰਨ*)(buff.ਅੰਕੜੇ() + max_ndims)};
    size_t* flatten_ਕਦਮਾਂ[2] = {(size_t*)(buff.ਅੰਕੜੇ() + 2 * max_ndims), (size_t*)(buff.ਅੰਕੜੇ() + 3 * max_ndims)};
    ਜੇ (flatten_for_ਪ੍ਰਸਾਰਣ(2, max_ndims, all_ndims, orig_shapes, flatten_shapes, flatten_ਕਦਮਾਂ)) {
        size_t src_dp = flatten_ਕਦਮਾਂ[0][max_ndims - 1];
        size_t dst_dp = flatten_ਕਦਮਾਂ[1][max_ndims - 1];
        CV_Assert(dst_dp == 1 || dst_dp == 0);
        CV_Assert(max_ndims >= 2); // >= 3?
        size_t rowstep_src = flatten_ਕਦਮਾਂ[0][max_ndims - 2];
        size_t rowstep_dst = flatten_ਕਦਮਾਂ[1][max_ndims - 2];
        ਅਚਲ ਅੱਖਰ* ptr_src = src.ptr<ਅਚਲ ਅੱਖਰ>();
        ਅੱਖਰ* ptr_dst = dst.ptr<ਅੱਖਰ>();
        size_t esz = src.elem_ਮਾਪ();
        ਪੂਰਨ nrows = flatten_shapes[1][max_ndims - 2];
        ਪੂਰਨ ncols = flatten_shapes[1][max_ndims - 1];
        ਪੂਰਨ nplanes = 1;
        CV_Check(esz, esz == 1 || esz == 2 || esz == 4 || esz == 8, "broadcast: not supported data type");

        ਲਈ (ਪੂਰਨ k = 0; k < max_ndims - 2; k++) {
            nplanes *= flatten_shapes[1][k];
        }
        ਲਈ (ਪੂਰਨ plane_idx = 0; plane_idx < nplanes; plane_idx++) {
            size_t ਵਿੱਥ_src = 0, ਵਿੱਥ_dst = 0;
            size_t idx = (size_t)plane_idx;
            ਲਈ (ਪੂਰਨ k = max_ndims - 3; k >= 0; k--) {
                size_t prev_idx = idx / flatten_shapes[1][k];
                size_t i_k = (ਪੂਰਨ)(idx - prev_idx * flatten_shapes[1][k]);
                ਵਿੱਥ_src += i_k * flatten_ਕਦਮਾਂ[0][k];
                ਵਿੱਥ_dst += i_k * flatten_ਕਦਮਾਂ[1][k];
                idx = prev_idx;
            }

            #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ OPENCV_CORE_ਪ੍ਰਸਾਰਣ_LOOP(_Tp) \
                ਲਈ (ਪੂਰਨ i = 0; i < nrows; i++) {   \
                    ਅਚਲ _Tp *ptr_src_ = (ਅਚਲ _Tp*)ptr_src + ਵਿੱਥ_src + rowstep_src * i; \
                    _Tp *ptr_dst_ = (_Tp*)ptr_dst + ਵਿੱਥ_dst + rowstep_dst * i; \
                    ਜੇ (src_dp == 1) { \
                        ਲਈ (ਪੂਰਨ j = 0; j < ncols; j++) { \
                            ptr_dst_[j] = ptr_src_[j]; \
                        } \
                    } ਨਹੀਂਤਾਂ { \
                        _Tp x = *ptr_src_; \
                        ਲਈ (ਪੂਰਨ j = 0; j < ncols; j++) { \
                            ptr_dst_[j] = x; \
                        } \
                    } \
                }

            ਜੇ (esz == 1) {
                OPENCV_CORE_ਪ੍ਰਸਾਰਣ_LOOP(int8_t);
            } ਨਹੀਂਤਾਂ ਜੇ (esz == 2) {
                OPENCV_CORE_ਪ੍ਰਸਾਰਣ_LOOP(int16_t);
            } ਨਹੀਂਤਾਂ ਜੇ (esz == 4) {
                OPENCV_CORE_ਪ੍ਰਸਾਰਣ_LOOP(int32_t);
            } ਨਹੀਂਤਾਂ ਜੇ (esz == 8) {
                OPENCV_CORE_ਪ੍ਰਸਾਰਣ_LOOP(int64_t);
            } ਨਹੀਂਤਾਂ {
                CV_Error(cv::Error::StsNotImplemented, "");
            }
            #ਪਰਿਭਾਸ਼ਾ_ਰੱਦ OPENCV_CORE_ਪ੍ਰਸਾਰਣ_LOOP
        }
    } ਨਹੀਂਤਾਂ {
        ਜੇ (dst.ਕੁੱਲ() == 0)
            ਵਾਪਸ;
        // initial copy (src to dst)
        std::vector<size_t> ਕਦਮ_src{src.ਕਦਮ.p, src.ਕਦਮ.p + dims_src};
        ਜੇ (ਕਦਮ_src.size() < static_cast<size_t>(dims_shape)) {
            ਕਦਮ_src.insert(ਕਦਮ_src.begin(), dims_shape - ਕਦਮ_src.size(), ਕਦਮ_src[0]);
        }
        ਲਈ (size_t i = 0; i < src.ਕੁੱਲ(); ++i) {
            size_t t = i;
            size_t src_ਵਿੱਥ = 0, dst_ਵਿੱਥ = 0;
            ਲਈ (ਪੂਰਨ j = static_cast<ਪੂਰਨ>(shape_src.size() - 1); j >= 0; --j) {
                size_t idx = t / shape_src[j];
                size_t offset = static_cast<size_t>(t - idx * shape_src[j]);
                src_ਵਿੱਥ += offset * ਕਦਮ_src[j];
                dst_ਵਿੱਥ += offset * dst.ਕਦਮ[j];
                t = idx;
            }
            ਅਚਲ ਸਵੈਚਾਲਿਤ *p_src = src.ptr<ਅਚਲ ਅੱਖਰ>();
            ਸਵੈਚਾਲਿਤ *p_dst = dst.ptr<ਅੱਖਰ>();
            std::ਸਮ੍ਰਿਤੀ_ਨਕਲ(p_dst + dst_ਵਿੱਥ, p_src + src_ਵਿੱਥ, dst.elem_ਮਾਪ());
        }
        // broadcast copy (dst inplace)
        Auto_ਬਫਰ<ਪੂਰਨ> cumulative_shape(dims_shape, 1);
        ਪੂਰਨ ਕੁੱਲ = static_cast<ਪੂਰਨ>(dst.ਕੁੱਲ());
        ਲਈ (ਪੂਰਨ i = dims_shape - 1; i >= 0; --i) {
            cumulative_shape[i] = static_cast<ਪੂਰਨ>(ਕੁੱਲ / ptr_shape[i]);
            ਕੁੱਲ = cumulative_shape[i];
        }
        ਲਈ (ਪੂਰਨ i = dims_shape - 1; i >= 0; --i) {
            ਜੇ (is_same_shape[i] == 1) {
                ਜਾਰੀ;
            }
            ਸਵੈਚਾਲਿਤ ਕਦਮ = dst.ਕਦਮ[i];
            ਸਵੈਚਾਲਿਤ *p_dst = dst.ptr<ਅੱਖਰ>();
            ਲਈ (ਪੂਰਨ j = 0; j < cumulative_shape[i]; j++) {
                ਲਈ (ਪੂਰਨ k = 0; k < ptr_shape[i] - 1; k++) {
                    std::ਸਮ੍ਰਿਤੀ_ਨਕਲ(p_dst + ਕਦਮ, p_dst, ਕਦਮ);
                    p_dst += ਕਦਮ;
                }
                p_dst += ਕਦਮ;
            }
        }
    }
}

ਖਾਲੀ ਪ੍ਰਸਾਰਣ(ਨਿਵੇਸ਼_Array _src, ਅਚਲ MatShape& _shape, OutputArray _dst)
{
    ਜੇ (_shape.dims < 0) {
        _dst.release();
    } ਨਹੀਂਤਾਂ {
        Mat shape(1, _shape.dims, CV_32S, (ਪੂਰਨ*)_shape.p);
        ਪ੍ਰਸਾਰਣ(_src, shape, _dst);
    }
}

ਸਥਿਰ ਖਾਲੀ rotateImpl(ਨਿਵੇਸ਼_Array _src, OutputArray _dst, ਪੂਰਨ rotate_ਬਹੁਲਕ)
{
    ਚੋਣ (rotate_ਬਹੁਲਕ)
    {
    ਹਾਲਤ ROTATE_90_CLOCKWISE:
        transpose(_src, _dst);
        flip(_dst, _dst, 1);
        ਰੋਕੋ;
    ਹਾਲਤ ROTATE_180:
        flip(_src, _dst, -1);
        ਰੋਕੋ;
    ਹਾਲਤ ROTATE_90_COUNTERCLOCKWISE:
        transpose(_src, _dst);
        flip(_dst, _dst, 0);
        ਰੋਕੋ;
    ਮੂਲ:
        ਰੋਕੋ;
    }
}

ਖਾਲੀ rotate(ਨਿਵੇਸ਼_Array _src, OutputArray _dst, ਪੂਰਨ rotate_ਬਹੁਲਕ)
{
    CV_Assert(_src.dims() <= 2);
    ਪੂਰਨ angle;

    ਜੇ (_dst.isUMat())
    {
        rotateImpl(_src, _dst, rotate_ਬਹੁਲਕ);
        ਵਾਪਸ;
    }

    Mat src = _src.getMat();
    ਪੂਰਨ type = src.type();
    ਜੇ( src.empty() )
    {
        _dst.release();
        ਵਾਪਸ;
    }

    ਚੋਣ (rotate_ਬਹੁਲਕ)
    {
    ਹਾਲਤ ROTATE_90_CLOCKWISE:
        _dst.create(src.ਖਾਨੇ_ਗਿਣਤੀ, src.ਪੰਕਤੀਆਂ, type);
        angle = 90;
        ਰੋਕੋ;
    ਹਾਲਤ ROTATE_180:
        _dst.create(src.ਪੰਕਤੀਆਂ, src.ਖਾਨੇ_ਗਿਣਤੀ, type);
        angle = 180;
        ਰੋਕੋ;
    ਹਾਲਤ ROTATE_90_COUNTERCLOCKWISE:
        _dst.create(src.ਖਾਨੇ_ਗਿਣਤੀ, src.ਪੰਕਤੀਆਂ, type);
        angle = 270;
        ਰੋਕੋ;
    ਮੂਲ:
        _dst.create(src.ਪੰਕਤੀਆਂ, src.ਖਾਨੇ_ਗਿਣਤੀ, type);
        angle = 0;
        ਰੋਕੋ;
    }

    Mat dst = _dst.getMat();
    CALL_HAL(rotate90, cv_hal_rotate90, type, src.ptr(), src.ਕਦਮ, src.ਖਾਨੇ_ਗਿਣਤੀ, src.ਪੰਕਤੀਆਂ,
             dst.ptr(), dst.ਕਦਮ, angle);

    // use src (Mat) since _src (InputArray) is updated by _dst.create() when in-place
    rotateImpl(src, _dst, rotate_ਬਹੁਲਕ);
}

}  // namespace
