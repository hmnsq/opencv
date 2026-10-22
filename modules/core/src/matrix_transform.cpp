// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html
// Copyright (C) 2026, Advanced Micro Devices, Inc., all rights reserved.
#समावेश "precomp.hpp"
#समावेश "opencl_kernels_core.hpp"
#समावेश "hal_replacement.hpp"
#समावेश "opencv2/core/detail/dispatch_helper.impl.hpp"

#समावेश "transpose.simd.hpp"
#समावेश "transpose.simd_declarations.hpp"
#समावेश "nd_copy.hpp"

#समावेश <algorithm> // std::swap_ranges
#समावेश <numeric> // std::accumulate

namespace cv {

////////////////////////////////////// transpose /////////////////////////////////////////
#अगर CV_SIMD128
अटल व्योम transpose_8bit_simd(स्थिर uchar* src, माप_प्रकार sstep, uchar* dst, माप_प्रकार dstep, आकार sz)
{
    स्थिर पूर्णांक m = sz.width, n = sz.height;
    पूर्णांक i = 0;
    क्रम (; i <= m - 16; i += 16)
    {
        पूर्णांक j = 0;
        क्रम (; j <= n - 16; j += 16)
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

            v_uint32x4 v0  = v_combine_निचला (u0,  u8);
            v_uint32x4 v1  = v_combine_ऊपरी(u0,  u8);
            v_uint32x4 v2  = v_combine_निचला (u1,  u9);
            v_uint32x4 v3  = v_combine_ऊपरी(u1,  u9);
            v_uint32x4 v4  = v_combine_निचला (u2,  u10);
            v_uint32x4 v5  = v_combine_ऊपरी(u2,  u10);
            v_uint32x4 v6  = v_combine_निचला (u3,  u11);
            v_uint32x4 v7  = v_combine_ऊपरी(u3,  u11);
            v_uint32x4 v8  = v_combine_निचला (u4,  u12);
            v_uint32x4 v9  = v_combine_ऊपरी(u4,  u12);
            v_uint32x4 v10 = v_combine_निचला (u5,  u13);
            v_uint32x4 v11 = v_combine_ऊपरी(u5,  u13);
            v_uint32x4 v12 = v_combine_निचला (u6,  u14);
            v_uint32x4 v13 = v_combine_ऊपरी(u6,  u14);
            v_uint32x4 v14 = v_combine_निचला (u7,  u15);
            v_uint32x4 v15 = v_combine_ऊपरी(u7,  u15);

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
        क्रम (; j < n; j++)
            क्रम (पूर्णांक k = 0; k < 16; k++)
                dst[dstep*(i+k) + j] = src[i + sstep*j + k];
    }
    क्रम (; i < m; i++)
        क्रम (पूर्णांक j = 0; j < n; j++)
            dst[dstep*i + j] = src[i + sstep*j];
}

अटल व्योम transpose_16bit_simd(स्थिर uchar* src, माप_प्रकार sstep, uchar* dst, माप_प्रकार dstep, आकार sz)
{
    स्थिर ushort* src16 = reinterpret_cast<स्थिर ushort*>(src);
    ushort* dst16 = reinterpret_cast<ushort*>(dst);

    स्थिर माप_प्रकार sstep_e = sstep / माप(ushort);
    स्थिर माप_प्रकार dstep_e = dstep / माप(ushort);

    स्थिर पूर्णांक m = sz.width, n = sz.height;
    पूर्णांक i = 0;

    क्रम (; i <= m - 8; i += 8)
    {
        पूर्णांक j = 0;
        क्रम (; j <= n - 8; j += 8)
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
        क्रम (; j < n; j++)
            क्रम (पूर्णांक k = 0; k < 8; k++)
                dst16[dstep_e*(i+k) + j] = src16[i + sstep_e*j + k];
    }
    क्रम (; i < m; i++)
        क्रम (पूर्णांक j = 0; j < n; j++)
            dst16[dstep_e*i + j] = src16[i + sstep_e*j];
}

अटल व्योम transpose_32bit_simd(स्थिर uchar* src, माप_प्रकार sstep, uchar* dst, माप_प्रकार dstep, आकार sz)
{
    // Cache-blocked + AVX2-aware path lives in transpose.simd.hpp; pick the
    // best variant available at runtime.
    CV_CPU_DISPATCH(transpose_32bit_blocks_simd, (src, sstep, dst, dstep, sz),
                    CV_CPU_DISPATCH_MODES_ALL);
}

अटल व्योम transpose_48bit_simd(स्थिर uchar* src, माप_प्रकार sstep, uchar* dst, माप_प्रकार dstep, आकार sz)
{
    स्थिर लघु* src16 = reinterpret_cast<स्थिर लघु*>(src);
    लघु* dst16 = reinterpret_cast<लघु*>(dst);

    स्थिर माप_प्रकार sstep_e = sstep / माप(लघु);
    स्थिर माप_प्रकार dstep_e = dstep / माप(लघु);

    स्थिर पूर्णांक m = sz.width, n = sz.height;
    पूर्णांक i = 0;

    क्रम (; i <= m - 8; i += 8)
    {
        पूर्णांक j = 0;
        क्रम (; j <= n - 8; j += 8)
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
        क्रम (; j < n; j++)
            क्रम (पूर्णांक k = 0; k < 8; k++)
            {
                dst16[dstep_e*(i+k) + j*3 + 0] = src16[sstep_e*j + (i+k)*3 + 0];
                dst16[dstep_e*(i+k) + j*3 + 1] = src16[sstep_e*j + (i+k)*3 + 1];
                dst16[dstep_e*(i+k) + j*3 + 2] = src16[sstep_e*j + (i+k)*3 + 2];
            }
    }
    क्रम (; i < m; i++)
        क्रम (पूर्णांक j = 0; j < n; j++)
        {
            dst16[dstep_e*i + j*3 + 0] = src16[sstep_e*j + i*3 + 0];
            dst16[dstep_e*i + j*3 + 1] = src16[sstep_e*j + i*3 + 1];
            dst16[dstep_e*i + j*3 + 2] = src16[sstep_e*j + i*3 + 2];
        }
}
#पूर्ण_अगर

ढाँचा<typename T> अटल व्योम
transpose_( स्थिर uchar* src, माप_प्रकार sstep, uchar* dst, माप_प्रकार dstep, आकार sz )
{
#अगर CV_SIMD128
    चयन (माप(T))
    {
        हाल 1: transpose_8bit_simd(src, sstep, dst, dstep, sz);  वापस;
        हाल 2: transpose_16bit_simd(src, sstep, dst, dstep, sz); वापस;
        हाल 4: transpose_32bit_simd(src, sstep, dst, dstep, sz); वापस;
        हाल 6: transpose_48bit_simd(src, sstep, dst, dstep, sz); वापस;
        शेष: अवरोध;
    }
#पूर्ण_अगर

    पूर्णांक i = 0, j, m = sz.width, n = sz.height;

    #अगर CV_ENABLE_UNROLLED
    क्रम(; i <= m - 4; i += 4 )
    {
        T* d0 = (T*)(dst + dstep*i);
        T* d1 = (T*)(dst + dstep*(i+1));
        T* d2 = (T*)(dst + dstep*(i+2));
        T* d3 = (T*)(dst + dstep*(i+3));

        क्रम( j = 0; j <= n - 4; j += 4 )
        {
            स्थिर T* s0 = (स्थिर T*)(src + i*माप(T) + sstep*j);
            स्थिर T* s1 = (स्थिर T*)(src + i*माप(T) + sstep*(j+1));
            स्थिर T* s2 = (स्थिर T*)(src + i*माप(T) + sstep*(j+2));
            स्थिर T* s3 = (स्थिर T*)(src + i*माप(T) + sstep*(j+3));

            d0[j] = s0[0]; d0[j+1] = s1[0]; d0[j+2] = s2[0]; d0[j+3] = s3[0];
            d1[j] = s0[1]; d1[j+1] = s1[1]; d1[j+2] = s2[1]; d1[j+3] = s3[1];
            d2[j] = s0[2]; d2[j+1] = s1[2]; d2[j+2] = s2[2]; d2[j+3] = s3[2];
            d3[j] = s0[3]; d3[j+1] = s1[3]; d3[j+2] = s2[3]; d3[j+3] = s3[3];
        }

        क्रम( ; j < n; j++ )
        {
            स्थिर T* s0 = (स्थिर T*)(src + i*माप(T) + j*sstep);
            d0[j] = s0[0]; d1[j] = s0[1]; d2[j] = s0[2]; d3[j] = s0[3];
        }
    }
    #पूर्ण_अगर
    क्रम( ; i < m; i++ )
    {
        T* d0 = (T*)(dst + dstep*i);
        j = 0;
        #अगर CV_ENABLE_UNROLLED
        क्रम(; j <= n - 4; j += 4 )
        {
            स्थिर T* s0 = (स्थिर T*)(src + i*माप(T) + sstep*j);
            स्थिर T* s1 = (स्थिर T*)(src + i*माप(T) + sstep*(j+1));
            स्थिर T* s2 = (स्थिर T*)(src + i*माप(T) + sstep*(j+2));
            स्थिर T* s3 = (स्थिर T*)(src + i*माप(T) + sstep*(j+3));

            d0[j] = s0[0]; d0[j+1] = s1[0]; d0[j+2] = s2[0]; d0[j+3] = s3[0];
        }
        #पूर्ण_अगर
        क्रम( ; j < n; j++ )
        {
            स्थिर T* s0 = (स्थिर T*)(src + i*माप(T) + j*sstep);
            d0[j] = s0[0];
        }
    }
}

ढाँचा<typename T> अटल व्योम
transposeI_( uchar* तथ्य, माप_प्रकार step, पूर्णांक n )
{
    क्रम( पूर्णांक i = 0; i < n; i++ )
    {
        T* row = (T*)(तथ्य + step*i);
        uchar* data1 = तथ्य + i*माप(T);
        क्रम( पूर्णांक j = i+1; j < n; j++ )
            std::swap( row[j], *(T*)(data1 + step*j) );
    }
}

प्रकार व्योम (*TransposeFunc)( स्थिर uchar* src, माप_प्रकार sstep, uchar* dst, माप_प्रकार dstep, आकार sz );
प्रकार व्योम (*TransposeInplaceFunc)( uchar* तथ्य, माप_प्रकार step, पूर्णांक n );

#घोषणा DEF_TRANSPOSE_FUNC(suffix, type) \
अटल व्योम transpose_##suffix( स्थिर uchar* src, माप_प्रकार sstep, uchar* dst, माप_प्रकार dstep, आकार sz ) \
{ transpose_<type>(src, sstep, dst, dstep, sz); } \
\
अटल व्योम transposeI_##suffix( uchar* तथ्य, माप_प्रकार step, पूर्णांक n ) \
{ transposeI_<type>(तथ्य, step, n); }

DEF_TRANSPOSE_FUNC(8u, uchar)
DEF_TRANSPOSE_FUNC(16u, ushort)
DEF_TRANSPOSE_FUNC(8uC3, Vec3b)
DEF_TRANSPOSE_FUNC(32s, पूर्णांक)
DEF_TRANSPOSE_FUNC(16uC3, Vec3s)
DEF_TRANSPOSE_FUNC(32sC2, Vec2i)
DEF_TRANSPOSE_FUNC(32sC3, Vec3i)
DEF_TRANSPOSE_FUNC(32sC4, Vec4i)
DEF_TRANSPOSE_FUNC(32sC6, Vec6i)
DEF_TRANSPOSE_FUNC(32sC8, Vec8i)

अटल TransposeFunc transposeTab[] =
{
    0, transpose_8u, transpose_16u, transpose_8uC3, transpose_32s, 0, transpose_16uC3, 0,
    transpose_32sC2, 0, 0, 0, transpose_32sC3, 0, 0, 0, transpose_32sC4,
    0, 0, 0, 0, 0, 0, 0, transpose_32sC6, 0, 0, 0, 0, 0, 0, 0, transpose_32sC8
};

nd::TransposeFunc nd::getTransposeFunc(माप_प्रकार esz)
{
    वापस esz < माप(transposeTab)/माप(transposeTab[0]) ? transposeTab[esz] : nullptr;
}

अटल TransposeInplaceFunc transposeInplaceTab[] =
{
    0, transposeI_8u, transposeI_16u, transposeI_8uC3, transposeI_32s, 0, transposeI_16uC3, 0,
    transposeI_32sC2, 0, 0, 0, transposeI_32sC3, 0, 0, 0, transposeI_32sC4,
    0, 0, 0, 0, 0, 0, 0, transposeI_32sC6, 0, 0, 0, 0, 0, 0, 0, transposeI_32sC8
};

#अगर_घोषित HAVE_OPENCL

अटल bool ocl_transpose( निवेश_Array _src, OutputArray _dst )
{
    स्थिर ocl::Device & dev = ocl::Device::getDefault();
    स्थिर पूर्णांक TILE_DIM = 32, BLOCK_ROWS = 8;
    पूर्णांक type = _src.type(), cn = CV_MAT_CN(type), depth = CV_MAT_DEPTH(type),
        rowsPerWI = dev.isIntel() ? 4 : 1;

    UMat src = _src.getUMat();
    _dst.create(src.cols, src.rows, type);
    UMat dst = _dst.getUMat();

    String kernelName("transpose");
    bool inplace = dst.u == src.u;

    अगर (inplace)
    {
        CV_Assert(dst.cols == dst.rows);
        kernelName += "_inplace";
    }
    अन्यथा
    {
        // check required local memory size
        माप_प्रकार required_local_memory = (माप_प्रकार) TILE_DIM*(TILE_DIM+1)*CV_ELEM_आकार(type);
        अगर (required_local_memory > ocl::Device::getDefault().local_Mem_आकार())
            वापस false;
    }

    String deviceMacro;
    अगर (dev.isIntel())
        deviceMacro = " -D INTEL_GPU";
    अन्यथा
        deviceMacro = "";

    ocl::Kernel k(kernelName.c_str(), ocl::core::transpose_oclsrc,
                  format("-D T=%s -D T1=%s -D cn=%d -D TILE_DIM=%d -D BLOCK_ROWS=%d -D rowsPerWI=%d%s%s",
                         ocl::memopTypeToStr(type), ocl::memopTypeToStr(depth),
                         cn, TILE_DIM, BLOCK_ROWS, rowsPerWI, inplace ? " -D INPLACE" : "", deviceMacro.c_str()));
    अगर (k.empty())
        वापस false;

    अगर (inplace)
        k.args(ocl::KernelArg::Read_Write_No_आकार(dst), dst.rows);
    अन्यथा
        k.args(ocl::KernelArg::ReadOnly(src),
               ocl::KernelArg::Write_Only_No_आकार(dst));

    माप_प्रकार localsize[2]  = { TILE_DIM, BLOCK_ROWS };
    माप_प्रकार globalsize[2] = { (माप_प्रकार)src.cols, inplace ? ((माप_प्रकार)src.rows + rowsPerWI - 1) / rowsPerWI : (divUp((माप_प्रकार)src.rows, TILE_DIM) * BLOCK_ROWS) };

    अगर (inplace && dev.isIntel())
    {
        localsize[0] = 16;
        localsize[1] = dev.max_Work_Group_आकार() / localsize[0];
    }

    वापस k.run(2, globalsize, localsize, false);
}

#पूर्ण_अगर

व्योम transpose( निवेश_Array _src, OutputArray _dst )
{
    CV_INSTRUMENT_REGION();

    पूर्णांक type = _src.type(), esz = CV_ELEM_आकार(type);
    CV_Assert( _src.dims() <= 2 && esz <= 32 );

    CV_OCL_RUN(_dst.isUMat(),
               ocl_transpose(_src, _dst))

    Mat src = _src.getMat();
    अगर( src.empty() )
    {
        _dst.release();
        वापस;
    }

    _dst.create(src.cols, src.rows, src.type());
    Mat dst = _dst.getMat();

    // handle the case of single-column/single-row matrices, stored in STL vectors.
    अगर( src.rows != dst.cols || src.cols != dst.rows )
    {
        CV_Assert( src.size() == dst.size() && (src.cols == 1 || src.rows == 1) );
        src.copyTo(dst);
        वापस;
    }

    CALL_HAL(transpose2d, cv_hal_transpose2d, src.तथ्य, src.step, dst.तथ्य, dst.step, src.cols, src.rows, esz);

    अगर( dst.तथ्य == src.तथ्य )
    {
        TransposeInplaceFunc func = transposeInplaceTab[esz];
        CV_Assert( func != 0 );
        CV_Assert( dst.cols == dst.rows );
        func( dst.ptr(), dst.step, dst.rows );
    }
    अन्यथा
    {
        TransposeFunc func = transposeTab[esz];
        CV_Assert( func != 0 );
        func( src.ptr(), src.step, dst.ptr(), dst.step, src.size() );
    }
}


व्योम transpose(निवेश_Array src_, स्थिर std::vector<पूर्णांक>& order, OutputArray dst_)
{
    CV_INSTRUMENT_REGION();

    Mat inp = src_.getMat();
    स्थिर पूर्णांक ndims = inp.dims;
    CV_Assert(!(ndims == 0 && inp.empty()));
    CV_CheckEQ(order.size(), static_cast<माप_प्रकार>(ndims), "Number of dimensions shouldn't change");

    bool used[CV_MAX_DIM] = {false};
    पूर्णांक newShape[CV_MAX_DIM];
    क्रम (पूर्णांक i = 0; i < ndims; i++)
    {
        पूर्णांक j = order[i];
        CV_Check(j, 0 <= j && j < ndims && !used[j], "New order should be a valid permutation of the old one");
        used[j] = true;
        newShape[i] = inp.size[j];
    }

    // plain 2D transpose keeps using cv::transpose (HAL, in-place square case);
    // not for an empty input, which cv::transpose would answer by releasing dst
    अगर (ndims == 2 && order[0] == 1 && !inp.empty() && nd::getTransposeFunc(inp.elem_आकार()))
    {
        transpose(inp, dst_);
        वापस;
    }

    dst_.create(ndims, newShape, inp.type());
    Mat out = dst_.getMat();
    अगर (out.योग() == 0)
        वापस;
    nd::copy(nd::permute(nd::viewOf(inp), order.तथ्य()), nd::viewOf(out));
}

व्योम transposeND(निवेश_Array src, स्थिर std::vector<पूर्णांक>& order, OutputArray dst)
{
    transpose(src, order, dst);
}


// Generic scalar fallback
CV_ALWAYS_INLINE व्योम flipHoriz_generic( स्थिर uchar* src, माप_प्रकार sstep, uchar* dst, माप_प्रकार dstep, आकार size, माप_प्रकार esz )
{
    पूर्णांक i, j, limit = (पूर्णांक)(((size.width + 1)/2)*esz);
    पूर्णांक height = size.height;
    Auto_बफर<पूर्णांक> _tab(size.width*esz);
    पूर्णांक* tab = _tab.तथ्य();

    क्रम( i = 0; i < size.width; i++ )
        क्रम( माप_प्रकार k = 0; k < esz; k++ )
            tab[i*esz + k] = (पूर्णांक)((size.width - i - 1)*esz + k);

    क्रम( ; height--; src += sstep, dst += dstep )
    {
        क्रम( i = 0; i < limit; i++ )
        {
            j = tab[i];
            uchar t0 = src[i], t1 = src[j];
            dst[i] = t1; dst[j] = t0;
        }
    }
}

#अगर CV_SIMD || CV_SIMD_SCALABLE
ढाँचा<typename V> CV_ALWAYS_INLINE व्योम flipHoriz_single( स्थिर uchar* src, माप_प्रकार sstep, uchar* dst, माप_प्रकार dstep, आकार size )
{
    प्रकार typename VTraits<V>::lane_type T;
    स्थिर पूर्णांक vlanes = VTraits<v_uint8>::vlanes();
    पूर्णांक end = (पूर्णांक)(size.width * माप(T));
    पूर्णांक width = (end + 1) / 2;
    पूर्णांक width_simd = width & -vlanes;
    पूर्णांक height = size.height;

    CV_DbgAssert(!CV_STRONG_ALIGNMENT || isAligned<माप(T)>(src, dst));

    क्रम( ; height--; src += sstep, dst += dstep )
    {
        पूर्णांक i = 0, j = end;
        क्रम( ; i < width_simd; i += vlanes, j -= vlanes )
        {
            V t0 = vx_load((स्थिर T*)(src + i));
            V t1 = vx_load((स्थिर T*)(src + j - vlanes));
            t0 = v_reverse(t0);
            t1 = v_reverse(t1);
            v_store((T*)(dst + j - vlanes), t0);
            v_store((T*)(dst + i), t1);
        }

        // Scalar tail loop
        अगर (isAligned<माप(T)>(src, dst))
        {
            क्रम ( ; i < width; i += माप(T), j -= माप(T) )
            {
                T t0 = *((स्थिर T*)(src + i));
                T t1 = *((स्थिर T*)(src + j - माप(T)));
                *((T*)(dst + j - माप(T))) = t0;
                *((T*)(dst + i)) = t1;
            }
        }
        अन्यथा
        {
            क्रम ( ; i < width; i += माप(T), j -= माप(T) )
            {
                क्रम (पूर्णांक k = 0; k < (पूर्णांक)माप(T); k++)
                {
                    uchar t0 = src[i + k];
                    uchar t1 = src[j + k - माप(T)];
                    dst[j + k - माप(T)] = t0;
                    dst[i + k] = t1;
                }
            }
        }
    }
}

// SIMD for C3
ढाँचा<typename V>
CV_ALWAYS_INLINE व्योम flipHoriz_c3( स्थिर uchar* src, माप_प्रकार sstep, uchar* dst, माप_प्रकार dstep, आकार size )
{
    प्रकार typename VTraits<V>::lane_type T;
    स्थिर पूर्णांक vlanes = VTraits<V>::vlanes();
    स्थिर पूर्णांक stride = 3 * माप(T);
    पूर्णांक width = size.width;
    पूर्णांक centre = (width + 1) / 2;
    पूर्णांक width_simd = (centre / vlanes) * vlanes;
    पूर्णांक height = size.height;

    क्रम( ; height--; src += sstep, dst += dstep )
    {
        पूर्णांक i = 0;
        क्रम( ; i < width_simd; i += vlanes )
        {
            V r0, g0, b0;
            v_load_deinterleave((स्थिर T*)(src + i * stride), r0, g0, b0);
            V r1, g1, b1;
            v_load_deinterleave((स्थिर T*)(src + (width - i - vlanes) * stride), r1, g1, b1);
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
        क्रम( ; i < centre; i++ )
        {
            पूर्णांक j = width - i - 1;
            T c0 = ((स्थिर T*)(src + i * stride))[0];
            T c1 = ((स्थिर T*)(src + i * stride))[1];
            T c2 = ((स्थिर T*)(src + i * stride))[2];
            T c3 = ((स्थिर T*)(src + j * stride))[0];
            T c4 = ((स्थिर T*)(src + j * stride))[1];
            T c5 = ((स्थिर T*)(src + j * stride))[2];
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
ढाँचा<माप_प्रकार ESZ>
CV_ALWAYS_INLINE व्योम flipHoriz_vlanes_match( स्थिर uchar* src, माप_प्रकार sstep, uchar* dst, माप_प्रकार dstep, आकार size)
{
    स्थिर पूर्णांक vlanes = VTraits<v_uint8>::vlanes();
    पूर्णांक end = (पूर्णांक)(size.width * ESZ);
    पूर्णांक width = end / 2;
    पूर्णांक height = size.height;
    पूर्णांक e_आकार = (पूर्णांक)ESZ;
    क्रम( ; height--; src += sstep, dst += dstep )
    {
        क्रम( पूर्णांक i = 0, j = end - e_आकार; i < width; i += e_आकार, j -= e_आकार )
        {
            क्रम( पूर्णांक k = 0; k < e_आकार; k += vlanes )
            {
                v_uint8 t0 = vx_load(src + i + k);
                v_uint8 t1 = vx_load(src + j + k);
                v_store(dst + j + k, t0);
                v_store(dst + i + k, t1);
            }
        }
    }
}

#अगर CV_SIMD128
// SIMD flip when ESZ=16 (128-bit)
ढाँचा<माप_प्रकार ESZ>
CV_ALWAYS_INLINE व्योम flipHoriz_vlanes_match_128( स्थिर uchar* src, माप_प्रकार sstep, uchar* dst, माप_प्रकार dstep, आकार size)
{
    स्थिर पूर्णांक vlanes16 = VTraits<v_uint8x16>::vlanes();
    पूर्णांक end = (पूर्णांक)(size.width * ESZ);
    पूर्णांक width = end / 2;
    पूर्णांक height = size.height;
    क्रम( ; height--; src += sstep, dst += dstep )
    {
        क्रम( पूर्णांक i = 0, j = end - vlanes16; i < width; i += vlanes16, j -= vlanes16 )
        {
            v_uint8x16 t0 = v_load(src + i);
            v_uint8x16 t1 = v_load(src + j);
            v_store(dst + j, t0);
            v_store(dst + i, t1);
        }
    }
}
#पूर्ण_अगर // CV_SIMD128

// SIMD flip for ESZ=16,32
ढाँचा<माप_प्रकार ESZ>
CV_ALWAYS_INLINE व्योम flipHoriz_vlanes_dispatch( स्थिर uchar* src, माप_प्रकार sstep, uchar* dst, माप_प्रकार dstep, आकार size )
{
    स्थिर पूर्णांक vlanes = VTraits<v_uint8>::vlanes();
#अगर CV_SIMD128
    स्थिर पूर्णांक vlanes16 = VTraits<v_uint8x16>::vlanes();
#पूर्ण_अगर
    अगर ( (ESZ == (माप_प्रकार)vlanes) || (ESZ == 2 * (माप_प्रकार)vlanes))
    {
        flipHoriz_vlanes_match<ESZ>(src, sstep, dst, dstep, size);
        वापस;
    }
#अगर CV_SIMD128
    अन्यथा अगर (ESZ == vlanes16)
    {
        flipHoriz_vlanes_match_128<ESZ>(src, sstep, dst, dstep, size);
        वापस;
    }
#पूर्ण_अगर
    flipHoriz_generic(src, sstep, dst, dstep, size, ESZ);
}

#अगर CV_SIMD128
// SIMD flip for ESZ=24 (CV_64FC3)
CV_ALWAYS_INLINE व्योम flipHoriz_24( स्थिर uchar* src, माप_प्रकार sstep, uchar* dst, माप_प्रकार dstep, आकार size )
{
    CV_DbgAssert(!CV_STRONG_ALIGNMENT || isAligned<8>(((माप_प्रकार)src)|((माप_प्रकार)dst)|sstep|dstep));

    स्थिर पूर्णांक lanes16 = 16;
    पूर्णांक end = (पूर्णांक)(size.width * 24);
    पूर्णांक width = (end + 1) / 2;
    पूर्णांक height = size.height;
    क्रम( ; height--; src += sstep, dst += dstep )
    {
        क्रम ( पूर्णांक i = 0, j = end; i < width; i += lanes16 + 8, j -= lanes16 + 8 )
        {
            v_uint8x16 t0 = v_load(src + i);
            uint64_t t2 = *reinterpret_cast<स्थिर uint64_t*>(src + i + lanes16);
            v_uint8x16 t1 = v_load(src + j - lanes16 - 8);
            uint64_t t3 = *reinterpret_cast<स्थिर uint64_t*>(src + j - 8);
            v_store(dst + j - lanes16 - 8, t0);
            *reinterpret_cast<uint64_t*>(dst + j - 8) = t2;
            v_store(dst + i, t1);
            *reinterpret_cast<uint64_t*>(dst + i + lanes16) = t3;
        }
    }
}
#पूर्ण_अगर // CV_SIMD128
#पूर्ण_अगर // CV_SIMD || CV_SIMD_SCALABLE

अटल व्योम flipHoriz( स्थिर uchar* src, माप_प्रकार sstep, uchar* dst, माप_प्रकार dstep, आकार size, माप_प्रकार esz )
{
#अगर CV_SIMD || CV_SIMD_SCALABLE
#अगर CV_STRONG_ALIGNMENT
    स्थिर माप_प्रकार alignMark = ((माप_प्रकार)src)|((माप_प्रकार)dst)|sstep|dstep;
#अन्यथा
    स्थिर माप_प्रकार alignMark = 0; // unaligned access is allowed: all the checks below fold to true
#पूर्ण_अगर

    // SIMD-optimized dispatch
    चयन(esz)
    {
        हाल 1:                                                                               // CV_8UC1: 8-bit, 1 channel
            flipHoriz_single<v_uint8>(src, sstep, dst, dstep, size); वापस;
        हाल 2:                                                                               // CV_8UC2, CV_16UC1: 8-bit 2-channel or 16-bit 1-channel
            अगर (isAligned<2>(alignMark)) { flipHoriz_single<v_uint16>(src, sstep, dst, dstep, size); वापस; }
            अवरोध;
        हाल 3:                                                                               // CV_8UC3: 8-bit, 3 channels
            flipHoriz_c3<v_uint8>(src, sstep, dst, dstep, size); वापस;
        हाल 4:                                                                               // CV_8UC4, CV_16UC2, CV_32SC1, CV_32FC1: 8-bit 4-channel, 16-bit 2-channel, or 32-bit 1-channel
            अगर (isAligned<4>(alignMark)) { flipHoriz_single<v_uint32>(src, sstep, dst, dstep, size); वापस; }
            अवरोध;
        हाल 6:                                                                               // CV_16UC3, CV_16SC3: 16-bit, 3 channels
            अगर (isAligned<2>(alignMark)) { flipHoriz_c3<v_uint16>(src, sstep, dst, dstep, size); वापस; }
            अवरोध;
        हाल 8:                                                                               // CV_16UC4, CV_32SC2, CV_32FC2, CV_64FC1: 16-bit 4-channel, 32-bit 2-channel, or 64-bit 1-channel
            अगर (isAligned<8>(alignMark)) { flipHoriz_single<v_uint64>(src, sstep, dst, dstep, size); वापस; }
            अवरोध;
        हाल 12:                                                                              // CV_32SC3, CV_32FC3: 32-bit, 3 channels
            अगर (isAligned<4>(alignMark)) { flipHoriz_c3<v_uint32>(src, sstep, dst, dstep, size); वापस; }
            अवरोध;
        हाल 16:                                                                              // CV_32SC4, CV_32FC4, CV_64FC2: 32-bit 4-channel or 64-bit 2-channel
            flipHoriz_vlanes_dispatch<16>(src, sstep, dst, dstep, size); वापस;               // byte-wise access only
#अगर CV_SIMD128
        हाल 24:                                                                              // CV_64FC3: 64-bit, 3 channels
            अगर (isAligned<8>(alignMark)) { flipHoriz_24(src, sstep, dst, dstep, size); वापस; }
            अवरोध;
#पूर्ण_अगर
        हाल 32:                                                                              // CV_64FC4: 64-bit, 4 channels
            flipHoriz_vlanes_dispatch<32>(src, sstep, dst, dstep, size); वापस;               // byte-wise access only
        शेष:
            अवरोध; // Fall through to generic implementation
    }
#पूर्ण_अगर
    // Fallback: generic scalar
    flipHoriz_generic(src, sstep, dst, dstep, size, esz);
}

अटल व्योम
flipVert( स्थिर uchar* src0, माप_प्रकार sstep, uchar* dst0, माप_प्रकार dstep, आकार size, माप_प्रकार esz )
{
    स्थिर uchar* src1 = src0 + (size.height - 1)*sstep;
    uchar* dst1 = dst0 + (size.height - 1)*dstep;
    size.width *= (पूर्णांक)esz;

    क्रम( पूर्णांक y = 0; y < (size.height + 1)/2; y++, src0 += sstep, src1 -= sstep,
                                                  dst0 += dstep, dst1 -= dstep )
    {
        पूर्णांक i = 0;
#अगर (CV_SIMD || CV_SIMD_SCALABLE)
#अगर CV_STRONG_ALIGNMENT
        अगर (isAligned<माप(पूर्णांक)>(src0, src1, dst0, dst1))
#पूर्ण_अगर
        {
            क्रम (; i <= size.width - VTraits<v_uint8>::vlanes(); i += VTraits<v_uint8>::vlanes())
            {
                v_int32 t0 = v_reinterpret_as_s32(vx_load(src0 + i));
                v_int32 t1 = v_reinterpret_as_s32(vx_load(src1 + i));
                v_store(dst0 + i, v_reinterpret_as_u8(t1));
                v_store(dst1 + i, v_reinterpret_as_u8(t0));
            }
        }
#अगर CV_STRONG_ALIGNMENT
        अन्यथा
        {
            क्रम (; i <= size.width - VTraits<v_uint8>::vlanes(); i += VTraits<v_uint8>::vlanes())
            {
                v_uint8 t0 = vx_load(src0 + i);
                v_uint8 t1 = vx_load(src1 + i);
                v_store(dst0 + i, t1);
                v_store(dst1 + i, t0);
            }
        }
#पूर्ण_अगर
#पूर्ण_अगर

        अगर (isAligned<माप(पूर्णांक)>(src0, src1, dst0, dst1))
        {
            क्रम( ; i <= size.width - 16; i += 16 )
            {
                पूर्णांक t0 = ((पूर्णांक*)(src0 + i))[0];
                पूर्णांक t1 = ((पूर्णांक*)(src1 + i))[0];

                ((पूर्णांक*)(dst0 + i))[0] = t1;
                ((पूर्णांक*)(dst1 + i))[0] = t0;

                t0 = ((पूर्णांक*)(src0 + i))[1];
                t1 = ((पूर्णांक*)(src1 + i))[1];

                ((पूर्णांक*)(dst0 + i))[1] = t1;
                ((पूर्णांक*)(dst1 + i))[1] = t0;

                t0 = ((पूर्णांक*)(src0 + i))[2];
                t1 = ((पूर्णांक*)(src1 + i))[2];

                ((पूर्णांक*)(dst0 + i))[2] = t1;
                ((पूर्णांक*)(dst1 + i))[2] = t0;

                t0 = ((पूर्णांक*)(src0 + i))[3];
                t1 = ((पूर्णांक*)(src1 + i))[3];

                ((पूर्णांक*)(dst0 + i))[3] = t1;
                ((पूर्णांक*)(dst1 + i))[3] = t0;
            }

            क्रम( ; i <= size.width - 4; i += 4 )
            {
                पूर्णांक t0 = ((पूर्णांक*)(src0 + i))[0];
                पूर्णांक t1 = ((पूर्णांक*)(src1 + i))[0];

                ((पूर्णांक*)(dst0 + i))[0] = t1;
                ((पूर्णांक*)(dst1 + i))[0] = t0;
            }
        }

        क्रम( ; i < size.width; i++ )
        {
            uchar t0 = src0[i];
            uchar t1 = src1[i];

            dst0[i] = t1;
            dst1[i] = t0;
        }
    }
}

#अगर_घोषित HAVE_OPENCL

क्रमागत { FLIP_COLS = 1 << 0, FLIP_ROWS = 1 << 1, FLIP_BOTH = FLIP_ROWS | FLIP_COLS };

अटल bool ocl_flip(निवेश_Array _src, OutputArray _dst, पूर्णांक flipCode )
{
    CV_Assert(flipCode >= -1 && flipCode <= 1);

    स्थिर ocl::Device & dev = ocl::Device::getDefault();
    पूर्णांक type = _src.type(), depth = CV_MAT_DEPTH(type), cn = CV_MAT_CN(type),
            flipType, kercn = std::min(ocl::predictOptimalVectorWidth(_src, _dst), 4);

    bool doubleSupport = dev.doubleFPConfig() > 0;
    अगर (!doubleSupport && depth == CV_64F)
        kercn = cn;

    अगर (cn > 4)
        वापस false;

    आकार size = _src.size();
    _dst.create(size, type);
    UMat src = _src.getUMat(), dst = _dst.getUMat();
    bool inplace = (dst.u == src.u);

    String kernelName;
    अगर (flipCode == 0)
        kernelName = "arithm_flip_rows", flipType = FLIP_ROWS;
    अन्यथा अगर (flipCode > 0)
        kernelName = "arithm_flip_cols", flipType = FLIP_COLS;
    अन्यथा
        kernelName = "arithm_flip_rows_cols", flipType = FLIP_BOTH;

    अगर(inplace)
        kernelName += "_inplace";

    पूर्णांक pxPerWIy = (dev.isIntel() && (dev.type() & ocl::Device::TYPE_GPU)) ? 4 : 1;
    kercn = (cn!=3 || flipType == FLIP_ROWS) ? std::max(kercn, cn) : cn;
    स्थिर पूर्णांक TILE_आकार = 32, BLOCK_ROWS = 8;

    ocl::Kernel k(kernelName.c_str(), ocl::core::flip_oclsrc,
                format( "-D T=%s -D T1=%s -D DEPTH=%d -D cn=%d -D PIX_PER_WI_Y=%d -D kercn=%d -D TILE_SIZE=%d -D BLOCK_ROWS=%d%s",
                kercn != cn ? ocl::typeToStr(CV_MAKE_TYPE(depth, kercn)) : ocl::vecopTypeToStr(CV_MAKE_TYPE(depth, kercn)),
                kercn != cn ? ocl::typeToStr(depth) : ocl::vecopTypeToStr(depth), depth, cn, pxPerWIy, kercn, TILE_आकार, BLOCK_ROWS,
                inplace ? " -D INPLACE" : ""));
    अगर (k.empty())
        वापस false;

    पूर्णांक cols = size.width * cn / kercn, rows = size.height;
    पूर्णांक work_cols = flipType == FLIP_COLS ? (cols + 1) >> 1 : cols;
    पूर्णांक work_rows = flipType & FLIP_ROWS ? (rows + 1) >> 1 : rows;

    अगर (inplace)
    {
        k.args(ocl::KernelArg::Read_Write_No_आकार(dst), rows, cols);

        पूर्णांक gs_cols, gs_rows;
        अगर (flipType == FLIP_COLS)
        {
            gs_cols = work_cols;
            gs_rows = rows;
        }
        अन्यथा अगर (flipType == FLIP_ROWS)
        {
            gs_cols = cols;
            gs_rows = work_rows;
        }
        अन्यथा // FLIP_BOTH
        {
            gs_cols = cols;
            gs_rows = rows;
        }

        माप_प्रकार globalsize[2] = {
            (माप_प्रकार)divUp(gs_cols, TILE_आकार) * TILE_आकार,
            (माप_प्रकार)divUp(gs_rows, TILE_आकार) * BLOCK_ROWS
        };
        माप_प्रकार localsize[2] = { TILE_आकार, BLOCK_ROWS };
        वापस k.run(2, globalsize, localsize, false);
    }
    अन्यथा
    {
        k.args(ocl::KernelArg::Read_Only_No_आकार(src),
           ocl::KernelArg::WriteOnly(dst, cn, kercn), work_rows, work_cols);

        माप_प्रकार max_Work_Group_आकार = dev.max_Work_Group_आकार();
        CV_Assert(max_Work_Group_आकार % 4 == 0);

        माप_प्रकार globalsize[2] = { (माप_प्रकार)work_cols, ((माप_प्रकार)work_rows + pxPerWIy - 1) / pxPerWIy };
        माप_प्रकार localsize[2]  = { max_Work_Group_आकार / 4, 4 };
        वापस k.run(2, globalsize, (flipType == FLIP_COLS) && !dev.isIntel() ? localsize : शून्य, false);
    }
}

#पूर्ण_अगर

व्योम flip( निवेश_Array _src, OutputArray _dst, पूर्णांक flip_mode )
{
    CV_INSTRUMENT_REGION();

    CV_Assert( _src.dims() <= 2 );
    आकार size = _src.size();

    अगर (flip_mode < 0)
    {
        अगर (size.width == 1)
            flip_mode = 0;
        अगर (size.height == 1)
            flip_mode = 1;
    }

    अगर ((size.width == 1 && flip_mode > 0) ||
        (size.height == 1 && flip_mode == 0))
    {
        वापस _src.copyTo(_dst);
    }

    CV_OCL_RUN( _dst.isUMat(), ocl_flip(_src, _dst, flip_mode))

    Mat src = _src.getMat();
    पूर्णांक type = src.type();
    _dst.create( size, type );
    Mat dst = _dst.getMat();

    CALL_HAL(flip, cv_hal_flip, type, src.ptr(), src.step, src.cols, src.rows,
             dst.ptr(), dst.step, flip_mode);

    माप_प्रकार esz = CV_ELEM_आकार(type);

    अगर( flip_mode <= 0 )
        flipVert( src.ptr(), src.step, dst.ptr(), dst.step, src.size(), esz );
    अन्यथा
        flipHoriz( src.ptr(), src.step, dst.ptr(), dst.step, src.size(), esz );

    अगर( flip_mode < 0 )
        flipHoriz( dst.ptr(), dst.step, dst.ptr(), dst.step, dst.size(), esz );
}

// In-place flip of a continuous array: swap the slices along the axis, no temporary copy.
अटल व्योम flipNDInplace(Mat& m, पूर्णांक axis)
{
    माप_प्रकार nouter = 1;
    क्रम (पूर्णांक i = 0; i < axis; i++)
        nouter *= (माप_प्रकार)m.size[i];
    स्थिर पूर्णांक n = m.size[axis];
    स्थिर माप_प्रकार step = m.step[axis];
    uchar* तथ्य = m.ptr();
    क्रम (माप_प्रकार i = 0; i < nouter; i++, तथ्य += n*step)
        क्रम (पूर्णांक j = 0, k = n - 1; j < k; j++, k--)
            std::swap_ranges(तथ्य + j*step, तथ्य + (j + 1)*step, तथ्य + k*step);
}

व्योम flipND(निवेश_Array _src, OutputArray _dst, पूर्णांक _axis)
{
    CV_INSTRUMENT_REGION();

    Mat src = _src.getMat();

    // verify axis
    पूर्णांक ndim = src.dims;
    CV_CheckLT(_axis, ndim, "flipND: given axis is out of range");
    CV_CheckGE(_axis, -ndim, "flipND: given axis is out of range");
    पूर्णांक axis = (_axis + ndim) % ndim;

    _dst.create(ndim, src.size.p, src.type());
    Mat dst = _dst.getMat();
    अगर (dst.तथ्य == src.तथ्य && src.isContinuous() && dst.isContinuous())
    {
        flipNDInplace(dst, axis);
        वापस;
    }

    nd::View v = nd::viewOf(src);
    nd::flip(v, axis);
    nd::copy(v, nd::viewOf(dst));
}

व्योम concat(निवेश_Array_Of_Arrays _src, पूर्णांक axis, OutputArray _dst)
{
    CV_INSTRUMENT_REGION();

    std::vector<Mat> src;
    _src.getMatVector(src);
    CV_Assert(!src.empty());

    स्थिर Mat& src0 = src[0];
    स्थिर पूर्णांक ndims = src0.dims, type = src0.type(), ninputs = (पूर्णांक)src.size();
    CV_CheckGE(ndims, 1, "concat: 0-dimensional arrays are not supported");
    CV_CheckGE(axis, -ndims, "concat: axis is out of range");
    CV_CheckLT(axis, ndims, "concat: axis is out of range");
    अगर (axis < 0)
        axis += ndims;

    पूर्णांक outShape[CV_MAX_DIM];
    क्रम (पूर्णांक i = 0; i < ndims; i++)
        outShape[i] = src0.size[i];
    int64 axis_आकार = 0;
    क्रम (पूर्णांक k = 0; k < ninputs; k++)
    {
        स्थिर Mat& m = src[k];
        CV_CheckEQ(m.type(), type, "concat: all the input arrays must have the same type");
        CV_CheckEQ(m.dims, ndims, "concat: all the input arrays must have the same number of dimensions");
        क्रम (पूर्णांक i = 0; i < ndims; i++)
            अगर (i != axis)
                CV_CheckEQ(m.size[i], outShape[i], "concat: the input shapes must match except along the axis");
        axis_आकार += m.size[axis];
    }
    CV_CheckLE((माप_प्रकार)axis_आकार, (माप_प्रकार)पूर्णांक_उच्च, "concat: the output is too big");
    outShape[axis] = (पूर्णांक)axis_आकार;

    _dst.create(ndims, outShape, type);
    Mat dst = _dst.getMat();

    Auto_बफर<nd::View, 4> sv(ninputs), dv(ninputs);
    स्थिर nd::View dst0 = nd::viewOf(dst);
    पूर्णांक ofs = 0;
    क्रम (पूर्णांक k = 0; k < ninputs; k++)
    {
        स्थिर पूर्णांक sz = src[k].size[axis];
        sv[k] = nd::viewOf(src[k]);
        dv[k] = dst0;
        nd::slice(dv[k], axis, ofs, 1, sz);
        ofs += sz;
    }
    nd::copyBatch(sv.तथ्य(), dv.तथ्य(), ninputs);
}

व्योम split(निवेश_Array _src, पूर्णांक axis, स्थिर std::vector<पूर्णांक>& sizes, OutputArrayOfArrays _dst)
{
    CV_INSTRUMENT_REGION();

    Mat src = _src.getMat();
    स्थिर पूर्णांक ndims = src.dims, type = src.type(), noutputs = (पूर्णांक)sizes.size();
    CV_CheckGE(ndims, 1, "split: 0-dimensional arrays are not supported");
    CV_CheckGE(axis, -ndims, "split: axis is out of range");
    CV_CheckLT(axis, ndims, "split: axis is out of range");
    अगर (axis < 0)
        axis += ndims;
    CV_Assert(noutputs > 0);

    int64 axis_आकार = 0;
    क्रम (पूर्णांक k = 0; k < noutputs; k++)
    {
        CV_CheckGE(sizes[k], 0, "split: the sizes must be non-negative");
        axis_आकार += sizes[k];
    }
    CV_CheckEQ((माप_प्रकार)axis_आकार, (माप_प्रकार)src.size[axis], "split: the sizes must sum up to the size of the axis");

    पूर्णांक outShape[CV_MAX_DIM];
    क्रम (पूर्णांक i = 0; i < ndims; i++)
        outShape[i] = src.size[i];

    _dst.create(noutputs, 1, type, -1, true);
    Auto_बफर<nd::View, 4> sv(noutputs), dv(noutputs);
    std::vector<Mat> dst(noutputs);
    स्थिर nd::View src0 = nd::viewOf(src);
    पूर्णांक ofs = 0;
    क्रम (पूर्णांक k = 0; k < noutputs; k++)
    {
        outShape[axis] = sizes[k];
        _dst.create(ndims, outShape, type, k);
        dst[k] = _dst.getMat(k);
        sv[k] = src0;
        nd::slice(sv[k], axis, ofs, 1, sizes[k]);
        dv[k] = nd::viewOf(dst[k]);
        ofs += sizes[k];
    }
    nd::copyBatch(sv.तथ्य(), dv.तथ्य(), noutputs);
}

व्योम repeat(निवेश_Array _src, स्थिर std::vector<पूर्णांक>& repeats, OutputArray _dst)
{
    CV_INSTRUMENT_REGION();

    Mat src = _src.getMat();
    स्थिर पूर्णांक ndims = src.dims;
    CV_Assert(!(ndims == 0 && src.empty()));
    CV_CheckEQ(repeats.size(), (माप_प्रकार)ndims, "repeat: the number of repeats must match the number of dimensions");

    पूर्णांक outShape[CV_MAX_DIM];
    क्रम (पूर्णांक i = 0; i < ndims; i++)
    {
        CV_CheckGE(repeats[i], 0, "repeat: the repeats must be non-negative");
        int64 sz = (int64)src.size[i]*repeats[i];
        CV_CheckLE((माप_प्रकार)sz, (माप_प्रकार)पूर्णांक_उच्च, "repeat: the output is too big");
        outShape[i] = (पूर्णांक)sz;
    }

    _dst.create(ndims, outShape, src.type());
    Mat dst = _dst.getMat();
    अगर (dst.योग() == 0 || src.empty())
        वापस;

    // Each repeated output axis is split into (repeat index, position), the former
    // with source step 0, so the whole tile is one broadcasting copy.
    nd::View sv, dv;
    sv.तथ्य = src.तथ्य;
    dv.तथ्य = dst.तथ्य;
    sv.esz = dv.esz = src.elem_आकार();
    पूर्णांक n = 0;
    क्रम (पूर्णांक i = 0; i < ndims; i++)
    {
        स्थिर पूर्णांक sz = src.size[i], r = repeats[i];
        अगर (r != 1)
        {
            CV_CheckLT(n, (पूर्णांक)nd::MAX_VIEW_DIMS, "repeat: too many dimensions");
            sv.size[n] = dv.size[n] = r;
            sv.step[n] = 0;
            dv.step[n] = (सूचक_भेद_प्रकार)dst.step[i]*sz;
            n++;
        }
        अगर (sz != 1 || r == 1)
        {
            CV_CheckLT(n, (पूर्णांक)nd::MAX_VIEW_DIMS, "repeat: too many dimensions");
            sv.size[n] = dv.size[n] = sz;
            sv.step[n] = (सूचक_भेद_प्रकार)src.step[i];
            dv.step[n] = (सूचक_भेद_प्रकार)dst.step[i];
            n++;
        }
    }
    sv.dims = dv.dims = n;
    nd::copy(sv, dv);
}

व्योम slice(निवेश_Array _src, स्थिर std::vector<पूर्णांक>& starts, स्थिर std::vector<पूर्णांक>& म_पूर्ण,
             स्थिर std::vector<पूर्णांक>& steps, OutputArray _dst)
{
    CV_INSTRUMENT_REGION();

    Mat src = _src.getMat();
    स्थिर पूर्णांक ndims = src.dims, nslices = (पूर्णांक)starts.size();
    CV_Assert(!(ndims == 0 && src.empty()));
    CV_CheckLE(nslices, ndims, "slice: too many starts");
    CV_CheckEQ(म_पूर्ण.size(), starts.size(), "slice: starts and ends must have the same size");
    CV_Check(steps.size(), steps.empty() || steps.size() == starts.size(),
             "slice: steps must be empty or have the same size as starts");

    nd::View sv = nd::viewOf(src);
    पूर्णांक outShape[CV_MAX_DIM];
    क्रम (पूर्णांक i = 0; i < ndims; i++)
        outShape[i] = src.size[i];

    क्रम (पूर्णांक i = 0; i < nslices; i++)
    {
        स्थिर पूर्णांक n = src.size[i], start = starts[i], end = म_पूर्ण[i];
        स्थिर पूर्णांक step = steps.empty() ? 1 : steps[i];
        CV_CheckNE(step, 0, "slice: step must not be zero");
        पूर्णांक गिनती_2;
        अगर (step > 0)
        {
            CV_Check(start, 0 <= start && start <= n, "slice: start is out of range");
            CV_Check(end, 0 <= end && end <= n, "slice: end is out of range");
            गिनती_2 = end > start ? (पूर्णांक)(((int64)end - start + step - 1)/step) : 0;
        }
        अन्यथा
        {
            CV_Check(start, -1 <= start && start < n, "slice: start is out of range");
            CV_Check(end, -1 <= end && end < n, "slice: end is out of range");
            गिनती_2 = start > end ? (पूर्णांक)(((int64)start - end - step - 1)/(-step)) : 0;
        }
        nd::slice(sv, i, start, step, गिनती_2);
        outShape[i] = गिनती_2;
    }

    _dst.create(ndims, outShape, src.type());
    Mat dst = _dst.getMat();
    अगर (dst.योग() == 0 || src.empty())
        वापस;
    nd::copy(sv, nd::viewOf(dst));
}

/*
    This function first prepends 1 to each tensor shape to have a common max_ndims dimension, then flatten non-broadcast dimensions.
*/
अटल bool _flatten_for_broadcast(पूर्णांक narrays, पूर्णांक max_ndims, स्थिर पूर्णांक* ndims, स्थिर पूर्णांक** orig_shape,
                                   पूर्णांक** flatten_shape, माप_प्रकार** flatten_step) {
    पूर्णांक i, j, k;

    // step 1.
    // * make all inputs and the output max_ndims-dimensional.
    // * compute proper step's
    क्रम (i = max_ndims - 1; i >= 0; i-- ) {
        क्रम (k = 0; k < narrays; k++) {
            j = ndims[k] - (max_ndims - i);
            पूर्णांक sz_i = j >= 0 ? orig_shape[k][j] : 1;
            माप_प्रकार st_i = i == max_ndims - 1 ? 1 : flatten_step[k][i+1] * flatten_shape[k][i+1];
            flatten_shape[k][i] = sz_i;
            flatten_step[k][i] = st_i;
            अगर (flatten_shape[k][i] == 0)
                वापस false;
        }
    }

    // step 2. Let's do the flattening first,
    // since we'd need proper values of steps to check continuity.
    // this loop is probably the most tricky part
    // in the whole implementation of broadcasting.
    j = max_ndims-1;
    क्रम (i = j - 1; i >= 0; i--) {
        bool all_contiguous = true, all_scalars = true, all_consistent = true;
        क्रम(k = 0; k < narrays; k++) {
            माप_प्रकार st = flatten_step[k][j] * flatten_shape[k][j];
            bool पूर्व_scalar = flatten_shape[k][j] == 1;
            bool scalar = flatten_shape[k][i] == 1;
            all_contiguous = all_contiguous && (st == flatten_step[k][i]);
            all_scalars = all_scalars && scalar;
            all_consistent = all_consistent && (scalar == पूर्व_scalar);
        }
        अगर (all_contiguous && (all_consistent || all_scalars)) {
            क्रम(k = 0; k < narrays; k++)
                flatten_shape[k][j] *= flatten_shape[k][i];
        } अन्यथा {
            j--;
            अगर (i < j) {
                क्रम(k = 0; k < narrays; k++) {
                    flatten_shape[k][j] = flatten_shape[k][i];
                    flatten_step[k][j] = flatten_step[k][i];
                }
            }
        }
    }

    // step 3. Set some step's to 0's.
    क्रम (i = max_ndims-1; i >= j; i--) {
        क्रम (k = 0; k < narrays; k++)
            flatten_step[k][i] = flatten_shape[k][i] == 1 ? 0 : flatten_step[k][i];
    }
    क्रम (; i >= 0; i--) {
        क्रम (k = 0; k < narrays; k++) {
            flatten_step[k][i] = 0;
            flatten_shape[k][i] = 1;
        }
    }
    वापस true;
}

व्योम broadcast(निवेश_Array _src, निवेश_Array _shape, OutputArray _dst) {
    CV_INSTRUMENT_REGION();

    Mat src = _src.getMat();
    CV_CheckTrue(src.isContinuous(), "broadcast: input array must be contiguous");
    CV_CheckChannelsEQ(src.channels(), 1, "broadcast: input array must be single channel");

    Mat shape = _shape.getMat();
    CV_CheckTypeEQ(shape.type(), CV_32S, "broadcast: target shape must be of type int32");
    स्थिर स्वतः dims_shape = static_cast<पूर्णांक>(shape.योग());
    स्थिर स्वतः *ptr_shape = shape.ptr<पूर्णांक>();

    // check valid shape, 1D/0D Mat would fail in the following checks
    स्थिर स्वतः dims_src = src.dims;
    CV_CheckLE(dims_src, dims_shape,
               "broadcast: dimension of input array must be less than or equal to dimension of target shape");
    std::vector<पूर्णांक> shape_src{src.size.p, src.size.p + dims_src};
    अगर (shape_src.size() < static_cast<माप_प्रकार>(dims_shape)) {
        shape_src.insert(shape_src.begin(), dims_shape - shape_src.size(), 1);
    }
    क्रम (पूर्णांक i = 0; i < static_cast<पूर्णांक>(shape_src.size()); ++i) {
        स्थिर स्वतः *shape_target = ptr_shape;
        अगर (shape_src[i] != 1) {
            CV_CheckEQ(shape_src[i], shape_target[i], "target shape must be equal to input shape or 1");
        }
    }

    // impl
    _dst.create(dims_shape, shape.ptr<पूर्णांक>(), src.type());
    Mat dst = _dst.getMat();
    अगर (dst.योग() == 0)
        वापस;
    cv::Auto_बफर<पूर्णांक> is_same_shape(dims_shape, 0);
    क्रम (पूर्णांक i = 0; i < static_cast<पूर्णांक>(shape_src.size()); ++i) {
        अगर (shape_src[i] == ptr_shape[i]) {
            is_same_shape[i] = 1;
        }
    }
    // copy if same shape
    अगर (std::accumulate(is_same_shape.begin(), is_same_shape.end(), 1, std::multiplies<पूर्णांक>()) != 0) {
        स्थिर स्वतः *p_src = src.ptr<स्थिर अक्षर>();
        स्वतः *p_dst = dst.ptr<अक्षर>();
        std::स_नकल(p_dst, p_src, dst.योग() * dst.elem_आकार());
        वापस;
    }
    // other cases
    पूर्णांक max_ndims = std::max(dims_src, dims_shape);
    अगर (max_ndims < 2 && src.योग() == 1) {
        स्थिर अक्षर* p_src = src.ptr<स्थिर अक्षर>();
        अक्षर* p_dst = dst.ptr<अक्षर>();
        माप_प्रकार esz = src.elem_आकार();
        क्रम (माप_प्रकार j = 0; j < dst.योग(); j++)
            std::स_नकल(p_dst + j * esz, p_src, esz);
        वापस;
    }
    स्थिर पूर्णांक all_ndims[2] = {src.dims, dst.dims};
    स्थिर पूर्णांक* orig_shapes[2] = {src.size.p, dst.size.p};
    cv::Auto_बफर<माप_प्रकार> buff(max_ndims * 4);
    पूर्णांक* flatten_shapes[2] = {(पूर्णांक*)buff.तथ्य(), (पूर्णांक*)(buff.तथ्य() + max_ndims)};
    माप_प्रकार* flatten_steps[2] = {(माप_प्रकार*)(buff.तथ्य() + 2 * max_ndims), (माप_प्रकार*)(buff.तथ्य() + 3 * max_ndims)};
    अगर (_flatten_for_broadcast(2, max_ndims, all_ndims, orig_shapes, flatten_shapes, flatten_steps)) {
        माप_प्रकार src_dp = flatten_steps[0][max_ndims - 1];
        माप_प्रकार dst_dp = flatten_steps[1][max_ndims - 1];
        CV_Assert(dst_dp == 1 || dst_dp == 0);
        CV_Assert(max_ndims >= 2); // >= 3?
        माप_प्रकार rowstep_src = flatten_steps[0][max_ndims - 2];
        माप_प्रकार rowstep_dst = flatten_steps[1][max_ndims - 2];
        स्थिर अक्षर* ptr_src = src.ptr<स्थिर अक्षर>();
        अक्षर* ptr_dst = dst.ptr<अक्षर>();
        माप_प्रकार esz = src.elem_आकार();
        पूर्णांक nrows = flatten_shapes[1][max_ndims - 2];
        पूर्णांक ncols = flatten_shapes[1][max_ndims - 1];
        पूर्णांक nplanes = 1;
        CV_Check(esz, esz == 1 || esz == 2 || esz == 4 || esz == 8, "broadcast: not supported data type");

        क्रम (पूर्णांक k = 0; k < max_ndims - 2; k++) {
            nplanes *= flatten_shapes[1][k];
        }
        क्रम (पूर्णांक plane_idx = 0; plane_idx < nplanes; plane_idx++) {
            माप_प्रकार offset_src = 0, offset_dst = 0;
            माप_प्रकार idx = (माप_प्रकार)plane_idx;
            क्रम (पूर्णांक k = max_ndims - 3; k >= 0; k--) {
                माप_प्रकार पूर्व_idx = idx / flatten_shapes[1][k];
                माप_प्रकार i_k = (पूर्णांक)(idx - पूर्व_idx * flatten_shapes[1][k]);
                offset_src += i_k * flatten_steps[0][k];
                offset_dst += i_k * flatten_steps[1][k];
                idx = पूर्व_idx;
            }

            #घोषणा OPENCV_CORE_BROADCAST_LOOP(_Tp) \
                क्रम (पूर्णांक i = 0; i < nrows; i++) {   \
                    स्थिर _Tp *ptr_src_ = (स्थिर _Tp*)ptr_src + offset_src + rowstep_src * i; \
                    _Tp *ptr_dst_ = (_Tp*)ptr_dst + offset_dst + rowstep_dst * i; \
                    अगर (src_dp == 1) { \
                        क्रम (पूर्णांक j = 0; j < ncols; j++) { \
                            ptr_dst_[j] = ptr_src_[j]; \
                        } \
                    } अन्यथा { \
                        _Tp x = *ptr_src_; \
                        क्रम (पूर्णांक j = 0; j < ncols; j++) { \
                            ptr_dst_[j] = x; \
                        } \
                    } \
                }

            अगर (esz == 1) {
                OPENCV_CORE_BROADCAST_LOOP(int8_t);
            } अन्यथा अगर (esz == 2) {
                OPENCV_CORE_BROADCAST_LOOP(int16_t);
            } अन्यथा अगर (esz == 4) {
                OPENCV_CORE_BROADCAST_LOOP(int32_t);
            } अन्यथा अगर (esz == 8) {
                OPENCV_CORE_BROADCAST_LOOP(int64_t);
            } अन्यथा {
                CV_Error(cv::Error::StsNotImplemented, "");
            }
            #अघोषित OPENCV_CORE_BROADCAST_LOOP
        }
    } अन्यथा {
        अगर (dst.योग() == 0)
            वापस;
        // initial copy (src to dst)
        std::vector<माप_प्रकार> step_src{src.step.p, src.step.p + dims_src};
        अगर (step_src.size() < static_cast<माप_प्रकार>(dims_shape)) {
            step_src.insert(step_src.begin(), dims_shape - step_src.size(), step_src[0]);
        }
        क्रम (माप_प्रकार i = 0; i < src.योग(); ++i) {
            माप_प्रकार t = i;
            माप_प्रकार src_offset = 0, dst_offset = 0;
            क्रम (पूर्णांक j = static_cast<पूर्णांक>(shape_src.size() - 1); j >= 0; --j) {
                माप_प्रकार idx = t / shape_src[j];
                माप_प्रकार offset = static_cast<माप_प्रकार>(t - idx * shape_src[j]);
                src_offset += offset * step_src[j];
                dst_offset += offset * dst.step[j];
                t = idx;
            }
            स्थिर स्वतः *p_src = src.ptr<स्थिर अक्षर>();
            स्वतः *p_dst = dst.ptr<अक्षर>();
            std::स_नकल(p_dst + dst_offset, p_src + src_offset, dst.elem_आकार());
        }
        // broadcast copy (dst inplace)
        Auto_बफर<पूर्णांक> cumulative_shape(dims_shape, 1);
        पूर्णांक योग = static_cast<पूर्णांक>(dst.योग());
        क्रम (पूर्णांक i = dims_shape - 1; i >= 0; --i) {
            cumulative_shape[i] = static_cast<पूर्णांक>(योग / ptr_shape[i]);
            योग = cumulative_shape[i];
        }
        क्रम (पूर्णांक i = dims_shape - 1; i >= 0; --i) {
            अगर (is_same_shape[i] == 1) {
                जारी;
            }
            स्वतः step = dst.step[i];
            स्वतः *p_dst = dst.ptr<अक्षर>();
            क्रम (पूर्णांक j = 0; j < cumulative_shape[i]; j++) {
                क्रम (पूर्णांक k = 0; k < ptr_shape[i] - 1; k++) {
                    std::स_नकल(p_dst + step, p_dst, step);
                    p_dst += step;
                }
                p_dst += step;
            }
        }
    }
}

व्योम broadcast(निवेश_Array _src, स्थिर MatShape& _shape, OutputArray _dst)
{
    अगर (_shape.dims < 0) {
        _dst.release();
    } अन्यथा {
        Mat shape(1, _shape.dims, CV_32S, (पूर्णांक*)_shape.p);
        broadcast(_src, shape, _dst);
    }
}

अटल व्योम rotateImpl(निवेश_Array _src, OutputArray _dst, पूर्णांक rotateMode)
{
    चयन (rotateMode)
    {
    हाल ROTATE_90_CLOCKWISE:
        transpose(_src, _dst);
        flip(_dst, _dst, 1);
        अवरोध;
    हाल ROTATE_180:
        flip(_src, _dst, -1);
        अवरोध;
    हाल ROTATE_90_COUNTERCLOCKWISE:
        transpose(_src, _dst);
        flip(_dst, _dst, 0);
        अवरोध;
    शेष:
        अवरोध;
    }
}

व्योम rotate(निवेश_Array _src, OutputArray _dst, पूर्णांक rotateMode)
{
    CV_Assert(_src.dims() <= 2);
    पूर्णांक angle;

    अगर (_dst.isUMat())
    {
        rotateImpl(_src, _dst, rotateMode);
        वापस;
    }

    Mat src = _src.getMat();
    पूर्णांक type = src.type();
    अगर( src.empty() )
    {
        _dst.release();
        वापस;
    }

    चयन (rotateMode)
    {
    हाल ROTATE_90_CLOCKWISE:
        _dst.create(src.cols, src.rows, type);
        angle = 90;
        अवरोध;
    हाल ROTATE_180:
        _dst.create(src.rows, src.cols, type);
        angle = 180;
        अवरोध;
    हाल ROTATE_90_COUNTERCLOCKWISE:
        _dst.create(src.cols, src.rows, type);
        angle = 270;
        अवरोध;
    शेष:
        _dst.create(src.rows, src.cols, type);
        angle = 0;
        अवरोध;
    }

    Mat dst = _dst.getMat();
    CALL_HAL(rotate90, cv_hal_rotate90, type, src.ptr(), src.step, src.cols, src.rows,
             dst.ptr(), dst.step, angle);

    // use src (Mat) since _src (InputArray) is updated by _dst.create() when in-place
    rotateImpl(src, _dst, rotateMode);
}

}  // namespace
