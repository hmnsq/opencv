// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html
// Copyright (C) 2026, Advanced Micro Devices, Inc., all rights reserved.
#شامل "precomp.hpp"
#شامل "opencl_kernels_core.hpp"
#شامل "hal_replacement.hpp"
#شامل "opencv2/core/detail/dispatch_helper.impl.hpp"

#شامل "transpose.simd.hpp"
#شامل "transpose.simd_declarations.hpp"
#شامل "nd_copy.hpp"

#شامل <algorithm> // std::swap_ranges
#شامل <numeric> // std::accumulate

نامخلا cv {

////////////////////////////////////// transpose /////////////////////////////////////////
#اگر CV_SIMD128
ساکن خالی transpose_8bit_simd(ثابت uchar* src, size_t sstep, uchar* dst, size_t dstep, سائز sz)
{
    ثابت صحیح m = sz.چوڑائی, n = sz.اونچائی;
    صحیح i = 0;
    برائے (; i <= m - 16; i += 16)
    {
        صحیح j = 0;
        برائے (; j <= n - 16; j += 16)
        {
            v_uint8x16 r0  = v_بھرو(src + i + sstep*(j+ 0));
            v_uint8x16 r1  = v_بھرو(src + i + sstep*(j+ 1));
            v_uint8x16 r2  = v_بھرو(src + i + sstep*(j+ 2));
            v_uint8x16 r3  = v_بھرو(src + i + sstep*(j+ 3));
            v_uint8x16 r4  = v_بھرو(src + i + sstep*(j+ 4));
            v_uint8x16 r5  = v_بھرو(src + i + sstep*(j+ 5));
            v_uint8x16 r6  = v_بھرو(src + i + sstep*(j+ 6));
            v_uint8x16 r7  = v_بھرو(src + i + sstep*(j+ 7));
            v_uint8x16 r8  = v_بھرو(src + i + sstep*(j+ 8));
            v_uint8x16 r9  = v_بھرو(src + i + sstep*(j+ 9));
            v_uint8x16 r10 = v_بھرو(src + i + sstep*(j+10));
            v_uint8x16 r11 = v_بھرو(src + i + sstep*(j+11));
            v_uint8x16 r12 = v_بھرو(src + i + sstep*(j+12));
            v_uint8x16 r13 = v_بھرو(src + i + sstep*(j+13));
            v_uint8x16 r14 = v_بھرو(src + i + sstep*(j+14));
            v_uint8x16 r15 = v_بھرو(src + i + sstep*(j+15));

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

            v_uint32x4 v0  = v_combine_کم (u0,  u8);
            v_uint32x4 v1  = v_combine_بلند(u0,  u8);
            v_uint32x4 v2  = v_combine_کم (u1,  u9);
            v_uint32x4 v3  = v_combine_بلند(u1,  u9);
            v_uint32x4 v4  = v_combine_کم (u2,  u10);
            v_uint32x4 v5  = v_combine_بلند(u2,  u10);
            v_uint32x4 v6  = v_combine_کم (u3,  u11);
            v_uint32x4 v7  = v_combine_بلند(u3,  u11);
            v_uint32x4 v8  = v_combine_کم (u4,  u12);
            v_uint32x4 v9  = v_combine_بلند(u4,  u12);
            v_uint32x4 v10 = v_combine_کم (u5,  u13);
            v_uint32x4 v11 = v_combine_بلند(u5,  u13);
            v_uint32x4 v12 = v_combine_کم (u6,  u14);
            v_uint32x4 v13 = v_combine_بلند(u6,  u14);
            v_uint32x4 v14 = v_combine_کم (u7,  u15);
            v_uint32x4 v15 = v_combine_بلند(u7,  u15);

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
        برائے (; j < n; j++)
            برائے (صحیح k = 0; k < 16; k++)
                dst[dstep*(i+k) + j] = src[i + sstep*j + k];
    }
    برائے (; i < m; i++)
        برائے (صحیح j = 0; j < n; j++)
            dst[dstep*i + j] = src[i + sstep*j];
}

ساکن خالی transpose_16bit_simd(ثابت uchar* src, size_t sstep, uchar* dst, size_t dstep, سائز sz)
{
    ثابت ushort* src16 = reinterpret_cast<ثابت ushort*>(src);
    ushort* dst16 = reinterpret_cast<ushort*>(dst);

    ثابت size_t sstep_e = sstep / حجم(ushort);
    ثابت size_t dstep_e = dstep / حجم(ushort);

    ثابت صحیح m = sz.چوڑائی, n = sz.اونچائی;
    صحیح i = 0;

    برائے (; i <= m - 8; i += 8)
    {
        صحیح j = 0;
        برائے (; j <= n - 8; j += 8)
        {
            v_uint16x8 r0 = v_بھرو(src16 + i + sstep_e*(j+0));
            v_uint16x8 r1 = v_بھرو(src16 + i + sstep_e*(j+1));
            v_uint16x8 r2 = v_بھرو(src16 + i + sstep_e*(j+2));
            v_uint16x8 r3 = v_بھرو(src16 + i + sstep_e*(j+3));
            v_uint16x8 r4 = v_بھرو(src16 + i + sstep_e*(j+4));
            v_uint16x8 r5 = v_بھرو(src16 + i + sstep_e*(j+5));
            v_uint16x8 r6 = v_بھرو(src16 + i + sstep_e*(j+6));
            v_uint16x8 r7 = v_بھرو(src16 + i + sstep_e*(j+7));

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
        برائے (; j < n; j++)
            برائے (صحیح k = 0; k < 8; k++)
                dst16[dstep_e*(i+k) + j] = src16[i + sstep_e*j + k];
    }
    برائے (; i < m; i++)
        برائے (صحیح j = 0; j < n; j++)
            dst16[dstep_e*i + j] = src16[i + sstep_e*j];
}

ساکن خالی transpose_32bit_simd(ثابت uchar* src, size_t sstep, uchar* dst, size_t dstep, سائز sz)
{
    // Cache-blocked + AVX2-aware path lives in transpose.simd.hpp; pick the
    // best variant available at runtime.
    CV_CPU_DISPATCH(transpose_32bit_بلاکیں_simd, (src, sstep, dst, dstep, sz),
                    CV_CPU_DISPATCH_MODES_ALL);
}

ساکن خالی transpose_48bit_simd(ثابت uchar* src, size_t sstep, uchar* dst, size_t dstep, سائز sz)
{
    ثابت مختصر* src16 = reinterpret_cast<ثابت مختصر*>(src);
    مختصر* dst16 = reinterpret_cast<مختصر*>(dst);

    ثابت size_t sstep_e = sstep / حجم(مختصر);
    ثابت size_t dstep_e = dstep / حجم(مختصر);

    ثابت صحیح m = sz.چوڑائی, n = sz.اونچائی;
    صحیح i = 0;

    برائے (; i <= m - 8; i += 8)
    {
        صحیح j = 0;
        برائے (; j <= n - 8; j += 8)
        {
            v_int16x8 C0_0, C1_0, C2_0;
            v_int16x8 C0_1, C1_1, C2_1;
            v_int16x8 C0_2, C1_2, C2_2;
            v_int16x8 C0_3, C1_3, C2_3;
            v_int16x8 C0_4, C1_4, C2_4;
            v_int16x8 C0_5, C1_5, C2_5;
            v_int16x8 C0_6, C1_6, C2_6;
            v_int16x8 C0_7, C1_7, C2_7;

            v_بھرو_deinterleave(src16 + sstep_e*(j+0) + i*3, C0_0, C1_0, C2_0);
            v_بھرو_deinterleave(src16 + sstep_e*(j+1) + i*3, C0_1, C1_1, C2_1);
            v_بھرو_deinterleave(src16 + sstep_e*(j+2) + i*3, C0_2, C1_2, C2_2);
            v_بھرو_deinterleave(src16 + sstep_e*(j+3) + i*3, C0_3, C1_3, C2_3);
            v_بھرو_deinterleave(src16 + sstep_e*(j+4) + i*3, C0_4, C1_4, C2_4);
            v_بھرو_deinterleave(src16 + sstep_e*(j+5) + i*3, C0_5, C1_5, C2_5);
            v_بھرو_deinterleave(src16 + sstep_e*(j+6) + i*3, C0_6, C1_6, C2_6);
            v_بھرو_deinterleave(src16 + sstep_e*(j+7) + i*3, C0_7, C1_7, C2_7);

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
        برائے (; j < n; j++)
            برائے (صحیح k = 0; k < 8; k++)
            {
                dst16[dstep_e*(i+k) + j*3 + 0] = src16[sstep_e*j + (i+k)*3 + 0];
                dst16[dstep_e*(i+k) + j*3 + 1] = src16[sstep_e*j + (i+k)*3 + 1];
                dst16[dstep_e*(i+k) + j*3 + 2] = src16[sstep_e*j + (i+k)*3 + 2];
            }
    }
    برائے (; i < m; i++)
        برائے (صحیح j = 0; j < n; j++)
        {
            dst16[dstep_e*i + j*3 + 0] = src16[sstep_e*j + i*3 + 0];
            dst16[dstep_e*i + j*3 + 1] = src16[sstep_e*j + i*3 + 1];
            dst16[dstep_e*i + j*3 + 2] = src16[sstep_e*j + i*3 + 2];
        }
}
#اگر_ختم

سانچہ<قسم_نام T> ساکن خالی
transpose_( ثابت uchar* src, size_t sstep, uchar* dst, size_t dstep, سائز sz )
{
#اگر CV_SIMD128
    انتخاب (حجم(T))
    {
        حالت 1: transpose_8bit_simd(src, sstep, dst, dstep, sz);  واپس;
        حالت 2: transpose_16bit_simd(src, sstep, dst, dstep, sz); واپس;
        حالت 4: transpose_32bit_simd(src, sstep, dst, dstep, sz); واپس;
        حالت 6: transpose_48bit_simd(src, sstep, dst, dstep, sz); واپس;
        طےشدہ: توڑو;
    }
#اگر_ختم

    صحیح i = 0, j, m = sz.چوڑائی, n = sz.اونچائی;

    #اگر CV_ENABLE_UNROLLED
    برائے(; i <= m - 4; i += 4 )
    {
        T* d0 = (T*)(dst + dstep*i);
        T* d1 = (T*)(dst + dstep*(i+1));
        T* d2 = (T*)(dst + dstep*(i+2));
        T* d3 = (T*)(dst + dstep*(i+3));

        برائے( j = 0; j <= n - 4; j += 4 )
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

        برائے( ; j < n; j++ )
        {
            ثابت T* s0 = (ثابت T*)(src + i*حجم(T) + j*sstep);
            d0[j] = s0[0]; d1[j] = s0[1]; d2[j] = s0[2]; d3[j] = s0[3];
        }
    }
    #اگر_ختم
    برائے( ; i < m; i++ )
    {
        T* d0 = (T*)(dst + dstep*i);
        j = 0;
        #اگر CV_ENABLE_UNROLLED
        برائے(; j <= n - 4; j += 4 )
        {
            ثابت T* s0 = (ثابت T*)(src + i*حجم(T) + sstep*j);
            ثابت T* s1 = (ثابت T*)(src + i*حجم(T) + sstep*(j+1));
            ثابت T* s2 = (ثابت T*)(src + i*حجم(T) + sstep*(j+2));
            ثابت T* s3 = (ثابت T*)(src + i*حجم(T) + sstep*(j+3));

            d0[j] = s0[0]; d0[j+1] = s1[0]; d0[j+2] = s2[0]; d0[j+3] = s3[0];
        }
        #اگر_ختم
        برائے( ; j < n; j++ )
        {
            ثابت T* s0 = (ثابت T*)(src + i*حجم(T) + j*sstep);
            d0[j] = s0[0];
        }
    }
}

سانچہ<قسم_نام T> ساکن خالی
transposeI_( uchar* ڈیٹا, size_t قدم, صحیح n )
{
    برائے( صحیح i = 0; i < n; i++ )
    {
        T* صف = (T*)(ڈیٹا + قدم*i);
        uchar* data1 = ڈیٹا + i*حجم(T);
        برائے( صحیح j = i+1; j < n; j++ )
            std::swap( صف[j], *(T*)(data1 + قدم*j) );
    }
}

قسمتعریف خالی (*TransposeFunc)( ثابت uchar* src, size_t sstep, uchar* dst, size_t dstep, سائز sz );
قسمتعریف خالی (*TransposeInplaceFunc)( uchar* ڈیٹا, size_t قدم, صحیح n );

#تعریف_کرو DEF_TRANSPOSE_FUNC(suffix, type) \
ساکن خالی transpose_##suffix( ثابت uchar* src, size_t sstep, uchar* dst, size_t dstep, سائز sz ) \
{ transpose_<type>(src, sstep, dst, dstep, sz); } \
\
ساکن خالی transposeI_##suffix( uchar* ڈیٹا, size_t قدم, صحیح n ) \
{ transposeI_<type>(ڈیٹا, قدم, n); }

DEF_TRANSPOSE_FUNC(8u, uchar)
DEF_TRANSPOSE_FUNC(16u, ushort)
DEF_TRANSPOSE_FUNC(8uC3, Vec3b)
DEF_TRANSPOSE_FUNC(32s, صحیح)
DEF_TRANSPOSE_FUNC(16uC3, Vec3s)
DEF_TRANSPOSE_FUNC(32sC2, Vec2i)
DEF_TRANSPOSE_FUNC(32sC3, Vec3i)
DEF_TRANSPOSE_FUNC(32sC4, Vec4i)
DEF_TRANSPOSE_FUNC(32sC6, Vec6i)
DEF_TRANSPOSE_FUNC(32sC8, Vec8i)

ساکن TransposeFunc transposeTab[] =
{
    0, transpose_8u, transpose_16u, transpose_8uC3, transpose_32s, 0, transpose_16uC3, 0,
    transpose_32sC2, 0, 0, 0, transpose_32sC3, 0, 0, 0, transpose_32sC4,
    0, 0, 0, 0, 0, 0, 0, transpose_32sC6, 0, 0, 0, 0, 0, 0, 0, transpose_32sC8
};

nd::TransposeFunc nd::getTransposeFunc(size_t esz)
{
    واپس esz < حجم(transposeTab)/حجم(transposeTab[0]) ? transposeTab[esz] : خالی_اشاریہ;
}

ساکن TransposeInplaceFunc transposeInplaceTab[] =
{
    0, transposeI_8u, transposeI_16u, transposeI_8uC3, transposeI_32s, 0, transposeI_16uC3, 0,
    transposeI_32sC2, 0, 0, 0, transposeI_32sC3, 0, 0, 0, transposeI_32sC4,
    0, 0, 0, 0, 0, 0, 0, transposeI_32sC6, 0, 0, 0, 0, 0, 0, 0, transposeI_32sC8
};

#اگر_تعریف_شدہ HAVE_OPENCL

ساکن منطقی ocl_transpose( داخلہ_Array _src, OutputArray _dst )
{
    ثابت ocl::Device & dev = ocl::Device::getDefault();
    ثابت صحیح TILE_DIM = 32, بلاک_صفیں = 8;
    صحیح type = _src.type(), cn = CV_MAT_CN(type), گہرائی = CV_MAT_گہرائی(type),
        صفیں_فی_WI = dev.isIntel() ? 4 : 1;

    UMat src = _src.getUMat();
    _dst.create(src.ستون_تعداد, src.صفیں, type);
    UMat dst = _dst.getUMat();

    String kernelName("transpose");
    منطقی inplace = dst.u == src.u;

    اگر (inplace)
    {
        CV_Assert(dst.ستون_تعداد == dst.صفیں);
        kernelName += "_inplace";
    }
    ورنہ
    {
        // check required local memory size
        size_t required_local_حافظہ = (size_t) TILE_DIM*(TILE_DIM+1)*CV_ELEM_سائز(type);
        اگر (required_local_حافظہ > ocl::Device::getDefault().local_Mem_سائز())
            واپس جھوٹ;
    }

    String deviceMacro;
    اگر (dev.isIntel())
        deviceMacro = " -D INTEL_GPU";
    ورنہ
        deviceMacro = "";

    ocl::Kernel k(kernelName.c_str(), ocl::core::transpose_oclsrc,
                  format("-D T=%s -D T1=%s -D cn=%d -D TILE_DIM=%d -D BLOCK_ROWS=%d -D rowsPerWI=%d%s%s",
                         ocl::memop_Type_مقصد_Str(type), ocl::memop_Type_مقصد_Str(گہرائی),
                         cn, TILE_DIM, بلاک_صفیں, صفیں_فی_WI, inplace ? " -D INPLACE" : "", deviceMacro.c_str()));
    اگر (k.empty())
        واپس جھوٹ;

    اگر (inplace)
        k.دلائل(ocl::KernelArg::Read_Write_No_سائز(dst), dst.صفیں);
    ورنہ
        k.دلائل(ocl::KernelArg::ReadOnly(src),
               ocl::KernelArg::Write_Only_No_سائز(dst));

    size_t localsize[2]  = { TILE_DIM, بلاک_صفیں };
    size_t globalsize[2] = { (size_t)src.ستون_تعداد, inplace ? ((size_t)src.صفیں + صفیں_فی_WI - 1) / صفیں_فی_WI : (div_اوپر((size_t)src.صفیں, TILE_DIM) * بلاک_صفیں) };

    اگر (inplace && dev.isIntel())
    {
        localsize[0] = 16;
        localsize[1] = dev.max_Work_گروہ_سائز() / localsize[0];
    }

    واپس k.تسلسل(2, globalsize, localsize, جھوٹ);
}

#اگر_ختم

خالی transpose( داخلہ_Array _src, OutputArray _dst )
{
    CV_INSTRUMENT_REGION();

    صحیح type = _src.type(), esz = CV_ELEM_سائز(type);
    CV_Assert( _src.dims() <= 2 && esz <= 32 );

    CV_OCL_تسلسل(_dst.isUMat(),
               ocl_transpose(_src, _dst))

    Mat src = _src.getMat();
    اگر( src.empty() )
    {
        _dst.release();
        واپس;
    }

    _dst.create(src.ستون_تعداد, src.صفیں, src.type());
    Mat dst = _dst.getMat();

    // handle the case of single-column/single-row matrices, stored in STL vectors.
    اگر( src.صفیں != dst.ستون_تعداد || src.ستون_تعداد != dst.صفیں )
    {
        CV_Assert( src.size() == dst.size() && (src.ستون_تعداد == 1 || src.صفیں == 1) );
        src.copy_مقصد(dst);
        واپس;
    }

    CALL_HAL(transpose2d, cv_hal_transpose2d, src.ڈیٹا, src.قدم, dst.ڈیٹا, dst.قدم, src.ستون_تعداد, src.صفیں, esz);

    اگر( dst.ڈیٹا == src.ڈیٹا )
    {
        TransposeInplaceFunc func = transposeInplaceTab[esz];
        CV_Assert( func != 0 );
        CV_Assert( dst.ستون_تعداد == dst.صفیں );
        func( dst.ptr(), dst.قدم, dst.صفیں );
    }
    ورنہ
    {
        TransposeFunc func = transposeTab[esz];
        CV_Assert( func != 0 );
        func( src.ptr(), src.قدم, dst.ptr(), dst.قدم, src.size() );
    }
}


خالی transpose(داخلہ_Array src_, ثابت std::vector<صحیح>& ترتیب, OutputArray dst_)
{
    CV_INSTRUMENT_REGION();

    Mat inp = src_.getMat();
    ثابت صحیح ndims = inp.dims;
    CV_Assert(!(ndims == 0 && inp.empty()));
    CV_CheckEQ(ترتیب.size(), static_cast<size_t>(ndims), "Number of dimensions shouldn't change");

    منطقی مستعمل[CV_MAX_DIM] = {جھوٹ};
    صحیح newShape[CV_MAX_DIM];
    برائے (صحیح i = 0; i < ndims; i++)
    {
        صحیح j = ترتیب[i];
        CV_Check(j, 0 <= j && j < ndims && !مستعمل[j], "New order should be a valid permutation of the old one");
        مستعمل[j] = سچ;
        newShape[i] = inp.size[j];
    }

    // plain 2D transpose keeps using cv::transpose (HAL, in-place square case);
    // not for an empty input, which cv::transpose would answer by releasing dst
    اگر (ndims == 2 && ترتیب[0] == 1 && !inp.empty() && nd::getTransposeFunc(inp.elem_سائز()))
    {
        transpose(inp, dst_);
        واپس;
    }

    dst_.create(ndims, newShape, inp.type());
    Mat out = dst_.getMat();
    اگر (out.کل() == 0)
        واپس;
    nd::copy(nd::ترتیب_بدلو(nd::view_کا(inp), ترتیب.ڈیٹا()), nd::view_کا(out));
}

خالی transposeND(داخلہ_Array src, ثابت std::vector<صحیح>& ترتیب, OutputArray dst)
{
    transpose(src, ترتیب, dst);
}


// Generic scalar fallback
CV_ALWAYS_INLINE خالی flipHoriz_generic( ثابت uchar* src, size_t sstep, uchar* dst, size_t dstep, سائز size, size_t esz )
{
    صحیح i, j, حد_بندی = (صحیح)(((size.چوڑائی + 1)/2)*esz);
    صحیح اونچائی = size.اونچائی;
    Auto_بفر<صحیح> _tab(size.چوڑائی*esz);
    صحیح* tab = _tab.ڈیٹا();

    برائے( i = 0; i < size.چوڑائی; i++ )
        برائے( size_t k = 0; k < esz; k++ )
            tab[i*esz + k] = (صحیح)((size.چوڑائی - i - 1)*esz + k);

    برائے( ; اونچائی--; src += sstep, dst += dstep )
    {
        برائے( i = 0; i < حد_بندی; i++ )
        {
            j = tab[i];
            uchar t0 = src[i], t1 = src[j];
            dst[i] = t1; dst[j] = t0;
        }
    }
}

#اگر CV_SIMD || CV_SIMD_SCALABLE
سانچہ<قسم_نام V> CV_ALWAYS_INLINE خالی flipHoriz_single( ثابت uchar* src, size_t sstep, uchar* dst, size_t dstep, سائز size )
{
    قسمتعریف قسم_نام VTraits<V>::lane_type T;
    ثابت صحیح vlanes = VTraits<v_uint8>::vlanes();
    صحیح end = (صحیح)(size.چوڑائی * حجم(T));
    صحیح چوڑائی = (end + 1) / 2;
    صحیح چوڑائی_simd = چوڑائی & -vlanes;
    صحیح اونچائی = size.اونچائی;

    CV_DbgAssert(!CV_STRONG_ALIGNMENT || isAligned<حجم(T)>(src, dst));

    برائے( ; اونچائی--; src += sstep, dst += dstep )
    {
        صحیح i = 0, j = end;
        برائے( ; i < چوڑائی_simd; i += vlanes, j -= vlanes )
        {
            V t0 = vx_بھرو((ثابت T*)(src + i));
            V t1 = vx_بھرو((ثابت T*)(src + j - vlanes));
            t0 = v_الٹ(t0);
            t1 = v_الٹ(t1);
            v_store((T*)(dst + j - vlanes), t0);
            v_store((T*)(dst + i), t1);
        }

        // Scalar tail loop
        اگر (isAligned<حجم(T)>(src, dst))
        {
            برائے ( ; i < چوڑائی; i += حجم(T), j -= حجم(T) )
            {
                T t0 = *((ثابت T*)(src + i));
                T t1 = *((ثابت T*)(src + j - حجم(T)));
                *((T*)(dst + j - حجم(T))) = t0;
                *((T*)(dst + i)) = t1;
            }
        }
        ورنہ
        {
            برائے ( ; i < چوڑائی; i += حجم(T), j -= حجم(T) )
            {
                برائے (صحیح k = 0; k < (صحیح)حجم(T); k++)
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
سانچہ<قسم_نام V>
CV_ALWAYS_INLINE خالی flipHoriz_c3( ثابت uchar* src, size_t sstep, uchar* dst, size_t dstep, سائز size )
{
    قسمتعریف قسم_نام VTraits<V>::lane_type T;
    ثابت صحیح vlanes = VTraits<V>::vlanes();
    ثابت صحیح stride = 3 * حجم(T);
    صحیح چوڑائی = size.چوڑائی;
    صحیح centre = (چوڑائی + 1) / 2;
    صحیح چوڑائی_simd = (centre / vlanes) * vlanes;
    صحیح اونچائی = size.اونچائی;

    برائے( ; اونچائی--; src += sstep, dst += dstep )
    {
        صحیح i = 0;
        برائے( ; i < چوڑائی_simd; i += vlanes )
        {
            V r0, g0, b0;
            v_بھرو_deinterleave((ثابت T*)(src + i * stride), r0, g0, b0);
            V r1, g1, b1;
            v_بھرو_deinterleave((ثابت T*)(src + (چوڑائی - i - vlanes) * stride), r1, g1, b1);
            r0 = v_الٹ(r0);
            g0 = v_الٹ(g0);
            b0 = v_الٹ(b0);
            r1 = v_الٹ(r1);
            g1 = v_الٹ(g1);
            b1 = v_الٹ(b1);
            v_store_interleave((T*)(dst + (چوڑائی - i - vlanes) * stride), r0, g0, b0);
            v_store_interleave((T*)(dst + i * stride), r1, g1, b1);
        }
        // Scalar tail loop for remaining pixels
        برائے( ; i < centre; i++ )
        {
            صحیح j = چوڑائی - i - 1;
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
سانچہ<size_t ESZ>
CV_ALWAYS_INLINE خالی flip_Horiz_vlanes_مطابقت( ثابت uchar* src, size_t sstep, uchar* dst, size_t dstep, سائز size)
{
    ثابت صحیح vlanes = VTraits<v_uint8>::vlanes();
    صحیح end = (صحیح)(size.چوڑائی * ESZ);
    صحیح چوڑائی = end / 2;
    صحیح اونچائی = size.اونچائی;
    صحیح e_سائز = (صحیح)ESZ;
    برائے( ; اونچائی--; src += sstep, dst += dstep )
    {
        برائے( صحیح i = 0, j = end - e_سائز; i < چوڑائی; i += e_سائز, j -= e_سائز )
        {
            برائے( صحیح k = 0; k < e_سائز; k += vlanes )
            {
                v_uint8 t0 = vx_بھرو(src + i + k);
                v_uint8 t1 = vx_بھرو(src + j + k);
                v_store(dst + j + k, t0);
                v_store(dst + i + k, t1);
            }
        }
    }
}

#اگر CV_SIMD128
// SIMD flip when ESZ=16 (128-bit)
سانچہ<size_t ESZ>
CV_ALWAYS_INLINE خالی flip_Horiz_vlanes_مطابقت_128( ثابت uchar* src, size_t sstep, uchar* dst, size_t dstep, سائز size)
{
    ثابت صحیح vlanes16 = VTraits<v_uint8x16>::vlanes();
    صحیح end = (صحیح)(size.چوڑائی * ESZ);
    صحیح چوڑائی = end / 2;
    صحیح اونچائی = size.اونچائی;
    برائے( ; اونچائی--; src += sstep, dst += dstep )
    {
        برائے( صحیح i = 0, j = end - vlanes16; i < چوڑائی; i += vlanes16, j -= vlanes16 )
        {
            v_uint8x16 t0 = v_بھرو(src + i);
            v_uint8x16 t1 = v_بھرو(src + j);
            v_store(dst + j, t0);
            v_store(dst + i, t1);
        }
    }
}
#اگر_ختم // CV_SIMD128

// SIMD flip for ESZ=16,32
سانچہ<size_t ESZ>
CV_ALWAYS_INLINE خالی flipHoriz_vlanes_dispatch( ثابت uchar* src, size_t sstep, uchar* dst, size_t dstep, سائز size )
{
    ثابت صحیح vlanes = VTraits<v_uint8>::vlanes();
#اگر CV_SIMD128
    ثابت صحیح vlanes16 = VTraits<v_uint8x16>::vlanes();
#اگر_ختم
    اگر ( (ESZ == (size_t)vlanes) || (ESZ == 2 * (size_t)vlanes))
    {
        flip_Horiz_vlanes_مطابقت<ESZ>(src, sstep, dst, dstep, size);
        واپس;
    }
#اگر CV_SIMD128
    ورنہ اگر (ESZ == vlanes16)
    {
        flip_Horiz_vlanes_مطابقت_128<ESZ>(src, sstep, dst, dstep, size);
        واپس;
    }
#اگر_ختم
    flipHoriz_generic(src, sstep, dst, dstep, size, ESZ);
}

#اگر CV_SIMD128
// SIMD flip for ESZ=24 (CV_64FC3)
CV_ALWAYS_INLINE خالی flipHoriz_24( ثابت uchar* src, size_t sstep, uchar* dst, size_t dstep, سائز size )
{
    CV_DbgAssert(!CV_STRONG_ALIGNMENT || isAligned<8>(((size_t)src)|((size_t)dst)|sstep|dstep));

    ثابت صحیح lanes16 = 16;
    صحیح end = (صحیح)(size.چوڑائی * 24);
    صحیح چوڑائی = (end + 1) / 2;
    صحیح اونچائی = size.اونچائی;
    برائے( ; اونچائی--; src += sstep, dst += dstep )
    {
        برائے ( صحیح i = 0, j = end; i < چوڑائی; i += lanes16 + 8, j -= lanes16 + 8 )
        {
            v_uint8x16 t0 = v_بھرو(src + i);
            uint64_t t2 = *reinterpret_cast<ثابت uint64_t*>(src + i + lanes16);
            v_uint8x16 t1 = v_بھرو(src + j - lanes16 - 8);
            uint64_t t3 = *reinterpret_cast<ثابت uint64_t*>(src + j - 8);
            v_store(dst + j - lanes16 - 8, t0);
            *reinterpret_cast<uint64_t*>(dst + j - 8) = t2;
            v_store(dst + i, t1);
            *reinterpret_cast<uint64_t*>(dst + i + lanes16) = t3;
        }
    }
}
#اگر_ختم // CV_SIMD128
#اگر_ختم // CV_SIMD || CV_SIMD_SCALABLE

ساکن خالی flipHoriz( ثابت uchar* src, size_t sstep, uchar* dst, size_t dstep, سائز size, size_t esz )
{
#اگر CV_SIMD || CV_SIMD_SCALABLE
#اگر CV_STRONG_ALIGNMENT
    ثابت size_t alignMark = ((size_t)src)|((size_t)dst)|sstep|dstep;
#ورنہ
    ثابت size_t alignMark = 0; // unaligned access is allowed: all the checks below fold to true
#اگر_ختم

    // SIMD-optimized dispatch
    انتخاب(esz)
    {
        حالت 1:                                                                               // CV_8UC1: 8-bit, 1 channel
            flipHoriz_single<v_uint8>(src, sstep, dst, dstep, size); واپس;
        حالت 2:                                                                               // CV_8UC2, CV_16UC1: 8-bit 2-channel or 16-bit 1-channel
            اگر (isAligned<2>(alignMark)) { flipHoriz_single<v_uint16>(src, sstep, dst, dstep, size); واپس; }
            توڑو;
        حالت 3:                                                                               // CV_8UC3: 8-bit, 3 channels
            flipHoriz_c3<v_uint8>(src, sstep, dst, dstep, size); واپس;
        حالت 4:                                                                               // CV_8UC4, CV_16UC2, CV_32SC1, CV_32FC1: 8-bit 4-channel, 16-bit 2-channel, or 32-bit 1-channel
            اگر (isAligned<4>(alignMark)) { flipHoriz_single<v_uint32>(src, sstep, dst, dstep, size); واپس; }
            توڑو;
        حالت 6:                                                                               // CV_16UC3, CV_16SC3: 16-bit, 3 channels
            اگر (isAligned<2>(alignMark)) { flipHoriz_c3<v_uint16>(src, sstep, dst, dstep, size); واپس; }
            توڑو;
        حالت 8:                                                                               // CV_16UC4, CV_32SC2, CV_32FC2, CV_64FC1: 16-bit 4-channel, 32-bit 2-channel, or 64-bit 1-channel
            اگر (isAligned<8>(alignMark)) { flipHoriz_single<v_uint64>(src, sstep, dst, dstep, size); واپس; }
            توڑو;
        حالت 12:                                                                              // CV_32SC3, CV_32FC3: 32-bit, 3 channels
            اگر (isAligned<4>(alignMark)) { flipHoriz_c3<v_uint32>(src, sstep, dst, dstep, size); واپس; }
            توڑو;
        حالت 16:                                                                              // CV_32SC4, CV_32FC4, CV_64FC2: 32-bit 4-channel or 64-bit 2-channel
            flipHoriz_vlanes_dispatch<16>(src, sstep, dst, dstep, size); واپس;               // byte-wise access only
#اگر CV_SIMD128
        حالت 24:                                                                              // CV_64FC3: 64-bit, 3 channels
            اگر (isAligned<8>(alignMark)) { flipHoriz_24(src, sstep, dst, dstep, size); واپس; }
            توڑو;
#اگر_ختم
        حالت 32:                                                                              // CV_64FC4: 64-bit, 4 channels
            flipHoriz_vlanes_dispatch<32>(src, sstep, dst, dstep, size); واپس;               // byte-wise access only
        طےشدہ:
            توڑو; // Fall through to generic implementation
    }
#اگر_ختم
    // Fallback: generic scalar
    flipHoriz_generic(src, sstep, dst, dstep, size, esz);
}

ساکن خالی
flipVert( ثابت uchar* src0, size_t sstep, uchar* dst0, size_t dstep, سائز size, size_t esz )
{
    ثابت uchar* src1 = src0 + (size.اونچائی - 1)*sstep;
    uchar* dst1 = dst0 + (size.اونچائی - 1)*dstep;
    size.چوڑائی *= (صحیح)esz;

    برائے( صحیح y = 0; y < (size.اونچائی + 1)/2; y++, src0 += sstep, src1 -= sstep,
                                                  dst0 += dstep, dst1 -= dstep )
    {
        صحیح i = 0;
#اگر (CV_SIMD || CV_SIMD_SCALABLE)
#اگر CV_STRONG_ALIGNMENT
        اگر (isAligned<حجم(صحیح)>(src0, src1, dst0, dst1))
#اگر_ختم
        {
            برائے (; i <= size.چوڑائی - VTraits<v_uint8>::vlanes(); i += VTraits<v_uint8>::vlanes())
            {
                v_int32 t0 = v_reinterpret_as_s32(vx_بھرو(src0 + i));
                v_int32 t1 = v_reinterpret_as_s32(vx_بھرو(src1 + i));
                v_store(dst0 + i, v_reinterpret_as_u8(t1));
                v_store(dst1 + i, v_reinterpret_as_u8(t0));
            }
        }
#اگر CV_STRONG_ALIGNMENT
        ورنہ
        {
            برائے (; i <= size.چوڑائی - VTraits<v_uint8>::vlanes(); i += VTraits<v_uint8>::vlanes())
            {
                v_uint8 t0 = vx_بھرو(src0 + i);
                v_uint8 t1 = vx_بھرو(src1 + i);
                v_store(dst0 + i, t1);
                v_store(dst1 + i, t0);
            }
        }
#اگر_ختم
#اگر_ختم

        اگر (isAligned<حجم(صحیح)>(src0, src1, dst0, dst1))
        {
            برائے( ; i <= size.چوڑائی - 16; i += 16 )
            {
                صحیح t0 = ((صحیح*)(src0 + i))[0];
                صحیح t1 = ((صحیح*)(src1 + i))[0];

                ((صحیح*)(dst0 + i))[0] = t1;
                ((صحیح*)(dst1 + i))[0] = t0;

                t0 = ((صحیح*)(src0 + i))[1];
                t1 = ((صحیح*)(src1 + i))[1];

                ((صحیح*)(dst0 + i))[1] = t1;
                ((صحیح*)(dst1 + i))[1] = t0;

                t0 = ((صحیح*)(src0 + i))[2];
                t1 = ((صحیح*)(src1 + i))[2];

                ((صحیح*)(dst0 + i))[2] = t1;
                ((صحیح*)(dst1 + i))[2] = t0;

                t0 = ((صحیح*)(src0 + i))[3];
                t1 = ((صحیح*)(src1 + i))[3];

                ((صحیح*)(dst0 + i))[3] = t1;
                ((صحیح*)(dst1 + i))[3] = t0;
            }

            برائے( ; i <= size.چوڑائی - 4; i += 4 )
            {
                صحیح t0 = ((صحیح*)(src0 + i))[0];
                صحیح t1 = ((صحیح*)(src1 + i))[0];

                ((صحیح*)(dst0 + i))[0] = t1;
                ((صحیح*)(dst1 + i))[0] = t0;
            }
        }

        برائے( ; i < size.چوڑائی; i++ )
        {
            uchar t0 = src0[i];
            uchar t1 = src1[i];

            dst0[i] = t1;
            dst1[i] = t0;
        }
    }
}

#اگر_تعریف_شدہ HAVE_OPENCL

شمار { FLIP_ستون_تعداد = 1 << 0, FLIP_صفیں = 1 << 1, FLIP_BOTH = FLIP_صفیں | FLIP_ستون_تعداد };

ساکن منطقی ocl_flip(داخلہ_Array _src, OutputArray _dst, صحیح flip_رمز )
{
    CV_Assert(flip_رمز >= -1 && flip_رمز <= 1);

    ثابت ocl::Device & dev = ocl::Device::getDefault();
    صحیح type = _src.type(), گہرائی = CV_MAT_گہرائی(type), cn = CV_MAT_CN(type),
            flipType, kercn = std::min(ocl::predict_Optimal_سمتیہ_چوڑائی(_src, _dst), 4);

    منطقی doubleSupport = dev.doubleFPConfig() > 0;
    اگر (!doubleSupport && گہرائی == CV_64F)
        kercn = cn;

    اگر (cn > 4)
        واپس جھوٹ;

    سائز size = _src.size();
    _dst.create(size, type);
    UMat src = _src.getUMat(), dst = _dst.getUMat();
    منطقی inplace = (dst.u == src.u);

    String kernelName;
    اگر (flip_رمز == 0)
        kernelName = "arithm_flip_rows", flipType = FLIP_صفیں;
    ورنہ اگر (flip_رمز > 0)
        kernelName = "arithm_flip_cols", flipType = FLIP_ستون_تعداد;
    ورنہ
        kernelName = "arithm_flip_rows_cols", flipType = FLIP_BOTH;

    اگر(inplace)
        kernelName += "_inplace";

    صحیح px_فی_WIy = (dev.isIntel() && (dev.type() & ocl::Device::TYPE_GPU)) ? 4 : 1;
    kercn = (cn!=3 || flipType == FLIP_صفیں) ? std::max(kercn, cn) : cn;
    ثابت صحیح TILE_سائز = 32, بلاک_صفیں = 8;

    ocl::Kernel k(kernelName.c_str(), ocl::core::flip_oclsrc,
                format( "-D T=%s -D T1=%s -D DEPTH=%d -D cn=%d -D PIX_PER_WI_Y=%d -D kercn=%d -D TILE_SIZE=%d -D BLOCK_ROWS=%d%s",
                kercn != cn ? ocl::type_مقصد_Str(CV_MAKE_TYPE(گہرائی, kercn)) : ocl::vecop_Type_مقصد_Str(CV_MAKE_TYPE(گہرائی, kercn)),
                kercn != cn ? ocl::type_مقصد_Str(گہرائی) : ocl::vecop_Type_مقصد_Str(گہرائی), گہرائی, cn, px_فی_WIy, kercn, TILE_سائز, بلاک_صفیں,
                inplace ? " -D INPLACE" : ""));
    اگر (k.empty())
        واپس جھوٹ;

    صحیح ستون_تعداد = size.چوڑائی * cn / kercn, صفیں = size.اونچائی;
    صحیح work_ستون_تعداد = flipType == FLIP_ستون_تعداد ? (ستون_تعداد + 1) >> 1 : ستون_تعداد;
    صحیح work_صفیں = flipType & FLIP_صفیں ? (صفیں + 1) >> 1 : صفیں;

    اگر (inplace)
    {
        k.دلائل(ocl::KernelArg::Read_Write_No_سائز(dst), صفیں, ستون_تعداد);

        صحیح gs_ستون_تعداد, gs_صفیں;
        اگر (flipType == FLIP_ستون_تعداد)
        {
            gs_ستون_تعداد = work_ستون_تعداد;
            gs_صفیں = صفیں;
        }
        ورنہ اگر (flipType == FLIP_صفیں)
        {
            gs_ستون_تعداد = ستون_تعداد;
            gs_صفیں = work_صفیں;
        }
        ورنہ // FLIP_BOTH
        {
            gs_ستون_تعداد = ستون_تعداد;
            gs_صفیں = صفیں;
        }

        size_t globalsize[2] = {
            (size_t)div_اوپر(gs_ستون_تعداد, TILE_سائز) * TILE_سائز,
            (size_t)div_اوپر(gs_صفیں, TILE_سائز) * بلاک_صفیں
        };
        size_t localsize[2] = { TILE_سائز, بلاک_صفیں };
        واپس k.تسلسل(2, globalsize, localsize, جھوٹ);
    }
    ورنہ
    {
        k.دلائل(ocl::KernelArg::Read_Only_No_سائز(src),
           ocl::KernelArg::WriteOnly(dst, cn, kercn), work_صفیں, work_ستون_تعداد);

        size_t max_Work_گروہ_سائز = dev.max_Work_گروہ_سائز();
        CV_Assert(max_Work_گروہ_سائز % 4 == 0);

        size_t globalsize[2] = { (size_t)work_ستون_تعداد, ((size_t)work_صفیں + px_فی_WIy - 1) / px_فی_WIy };
        size_t localsize[2]  = { max_Work_گروہ_سائز / 4, 4 };
        واپس k.تسلسل(2, globalsize, (flipType == FLIP_ستون_تعداد) && !dev.isIntel() ? localsize : تہی, جھوٹ);
    }
}

#اگر_ختم

خالی flip( داخلہ_Array _src, OutputArray _dst, صحیح flip_منوال )
{
    CV_INSTRUMENT_REGION();

    CV_Assert( _src.dims() <= 2 );
    سائز size = _src.size();

    اگر (flip_منوال < 0)
    {
        اگر (size.چوڑائی == 1)
            flip_منوال = 0;
        اگر (size.اونچائی == 1)
            flip_منوال = 1;
    }

    اگر ((size.چوڑائی == 1 && flip_منوال > 0) ||
        (size.اونچائی == 1 && flip_منوال == 0))
    {
        واپس _src.copy_مقصد(_dst);
    }

    CV_OCL_تسلسل( _dst.isUMat(), ocl_flip(_src, _dst, flip_منوال))

    Mat src = _src.getMat();
    صحیح type = src.type();
    _dst.create( size, type );
    Mat dst = _dst.getMat();

    CALL_HAL(flip, cv_hal_flip, type, src.ptr(), src.قدم, src.ستون_تعداد, src.صفیں,
             dst.ptr(), dst.قدم, flip_منوال);

    size_t esz = CV_ELEM_سائز(type);

    اگر( flip_منوال <= 0 )
        flipVert( src.ptr(), src.قدم, dst.ptr(), dst.قدم, src.size(), esz );
    ورنہ
        flipHoriz( src.ptr(), src.قدم, dst.ptr(), dst.قدم, src.size(), esz );

    اگر( flip_منوال < 0 )
        flipHoriz( dst.ptr(), dst.قدم, dst.ptr(), dst.قدم, dst.size(), esz );
}

// In-place flip of a continuous array: swap the slices along the axis, no temporary copy.
ساکن خالی flipNDInplace(Mat& m, صحیح axis)
{
    size_t nouter = 1;
    برائے (صحیح i = 0; i < axis; i++)
        nouter *= (size_t)m.size[i];
    ثابت صحیح n = m.size[axis];
    ثابت size_t قدم = m.قدم[axis];
    uchar* ڈیٹا = m.ptr();
    برائے (size_t i = 0; i < nouter; i++, ڈیٹا += n*قدم)
        برائے (صحیح j = 0, k = n - 1; j < k; j++, k--)
            std::swap_ranges(ڈیٹا + j*قدم, ڈیٹا + (j + 1)*قدم, ڈیٹا + k*قدم);
}

خالی flipND(داخلہ_Array _src, OutputArray _dst, صحیح _axis)
{
    CV_INSTRUMENT_REGION();

    Mat src = _src.getMat();

    // verify axis
    صحیح ndim = src.dims;
    CV_CheckLT(_axis, ndim, "flipND: given axis is out of range");
    CV_CheckGE(_axis, -ndim, "flipND: given axis is out of range");
    صحیح axis = (_axis + ndim) % ndim;

    _dst.create(ndim, src.size.p, src.type());
    Mat dst = _dst.getMat();
    اگر (dst.ڈیٹا == src.ڈیٹا && src.isContinuous() && dst.isContinuous())
    {
        flipNDInplace(dst, axis);
        واپس;
    }

    nd::View v = nd::view_کا(src);
    nd::flip(v, axis);
    nd::copy(v, nd::view_کا(dst));
}

خالی concat(داخلہ_Array_کا_Arrays _src, صحیح axis, OutputArray _dst)
{
    CV_INSTRUMENT_REGION();

    std::vector<Mat> src;
    _src.get_Mat_سمتیہ(src);
    CV_Assert(!src.empty());

    ثابت Mat& src0 = src[0];
    ثابت صحیح ndims = src0.dims, type = src0.type(), ninputs = (صحیح)src.size();
    CV_CheckGE(ndims, 1, "concat: 0-dimensional arrays are not supported");
    CV_CheckGE(axis, -ndims, "concat: axis is out of range");
    CV_CheckLT(axis, ndims, "concat: axis is out of range");
    اگر (axis < 0)
        axis += ndims;

    صحیح برآمد_Shape[CV_MAX_DIM];
    برائے (صحیح i = 0; i < ndims; i++)
        برآمد_Shape[i] = src0.size[i];
    int64 axis_سائز = 0;
    برائے (صحیح k = 0; k < ninputs; k++)
    {
        ثابت Mat& m = src[k];
        CV_CheckEQ(m.type(), type, "concat: all the input arrays must have the same type");
        CV_CheckEQ(m.dims, ndims, "concat: all the input arrays must have the same number of dimensions");
        برائے (صحیح i = 0; i < ndims; i++)
            اگر (i != axis)
                CV_CheckEQ(m.size[i], برآمد_Shape[i], "concat: the input shapes must match except along the axis");
        axis_سائز += m.size[axis];
    }
    CV_CheckLE((size_t)axis_سائز, (size_t)زیادہ_سے_زیادہ_عدد, "concat: the output is too big");
    برآمد_Shape[axis] = (صحیح)axis_سائز;

    _dst.create(ndims, برآمد_Shape, type);
    Mat dst = _dst.getMat();

    Auto_بفر<nd::View, 4> sv(ninputs), dv(ninputs);
    ثابت nd::View dst0 = nd::view_کا(dst);
    صحیح ofs = 0;
    برائے (صحیح k = 0; k < ninputs; k++)
    {
        ثابت صحیح sz = src[k].size[axis];
        sv[k] = nd::view_کا(src[k]);
        dv[k] = dst0;
        nd::قاش_2(dv[k], axis, ofs, 1, sz);
        ofs += sz;
    }
    nd::copyBatch(sv.ڈیٹا(), dv.ڈیٹا(), ninputs);
}

خالی بانٹو(داخلہ_Array _src, صحیح axis, ثابت std::vector<صحیح>& sizes, Output_Array_کا_Arrays _dst)
{
    CV_INSTRUMENT_REGION();

    Mat src = _src.getMat();
    ثابت صحیح ndims = src.dims, type = src.type(), noutputs = (صحیح)sizes.size();
    CV_CheckGE(ndims, 1, "split: 0-dimensional arrays are not supported");
    CV_CheckGE(axis, -ndims, "split: axis is out of range");
    CV_CheckLT(axis, ndims, "split: axis is out of range");
    اگر (axis < 0)
        axis += ndims;
    CV_Assert(noutputs > 0);

    int64 axis_سائز = 0;
    برائے (صحیح k = 0; k < noutputs; k++)
    {
        CV_CheckGE(sizes[k], 0, "split: the sizes must be non-negative");
        axis_سائز += sizes[k];
    }
    CV_CheckEQ((size_t)axis_سائز, (size_t)src.size[axis], "split: the sizes must sum up to the size of the axis");

    صحیح برآمد_Shape[CV_MAX_DIM];
    برائے (صحیح i = 0; i < ndims; i++)
        برآمد_Shape[i] = src.size[i];

    _dst.create(noutputs, 1, type, -1, سچ);
    Auto_بفر<nd::View, 4> sv(noutputs), dv(noutputs);
    std::vector<Mat> dst(noutputs);
    ثابت nd::View src0 = nd::view_کا(src);
    صحیح ofs = 0;
    برائے (صحیح k = 0; k < noutputs; k++)
    {
        برآمد_Shape[axis] = sizes[k];
        _dst.create(ndims, برآمد_Shape, type, k);
        dst[k] = _dst.getMat(k);
        sv[k] = src0;
        nd::قاش_2(sv[k], axis, ofs, 1, sizes[k]);
        dv[k] = nd::view_کا(dst[k]);
        ofs += sizes[k];
    }
    nd::copyBatch(sv.ڈیٹا(), dv.ڈیٹا(), noutputs);
}

خالی repeat(داخلہ_Array _src, ثابت std::vector<صحیح>& repeats, OutputArray _dst)
{
    CV_INSTRUMENT_REGION();

    Mat src = _src.getMat();
    ثابت صحیح ndims = src.dims;
    CV_Assert(!(ndims == 0 && src.empty()));
    CV_CheckEQ(repeats.size(), (size_t)ndims, "repeat: the number of repeats must match the number of dimensions");

    صحیح برآمد_Shape[CV_MAX_DIM];
    برائے (صحیح i = 0; i < ndims; i++)
    {
        CV_CheckGE(repeats[i], 0, "repeat: the repeats must be non-negative");
        int64 sz = (int64)src.size[i]*repeats[i];
        CV_CheckLE((size_t)sz, (size_t)زیادہ_سے_زیادہ_عدد, "repeat: the output is too big");
        برآمد_Shape[i] = (صحیح)sz;
    }

    _dst.create(ndims, برآمد_Shape, src.type());
    Mat dst = _dst.getMat();
    اگر (dst.کل() == 0 || src.empty())
        واپس;

    // Each repeated output axis is split into (repeat index, position), the former
    // with source step 0, so the whole tile is one broadcasting copy.
    nd::View sv, dv;
    sv.ڈیٹا = src.ڈیٹا;
    dv.ڈیٹا = dst.ڈیٹا;
    sv.esz = dv.esz = src.elem_سائز();
    صحیح n = 0;
    برائے (صحیح i = 0; i < ndims; i++)
    {
        ثابت صحیح sz = src.size[i], r = repeats[i];
        اگر (r != 1)
        {
            CV_CheckLT(n, (صحیح)nd::MAX_VIEW_DIMS, "repeat: too many dimensions");
            sv.size[n] = dv.size[n] = r;
            sv.قدم[n] = 0;
            dv.قدم[n] = (ptrdiff_t)dst.قدم[i]*sz;
            n++;
        }
        اگر (sz != 1 || r == 1)
        {
            CV_CheckLT(n, (صحیح)nd::MAX_VIEW_DIMS, "repeat: too many dimensions");
            sv.size[n] = dv.size[n] = sz;
            sv.قدم[n] = (ptrdiff_t)src.قدم[i];
            dv.قدم[n] = (ptrdiff_t)dst.قدم[i];
            n++;
        }
    }
    sv.dims = dv.dims = n;
    nd::copy(sv, dv);
}

خالی قاش_2(داخلہ_Array _src, ثابت std::vector<صحیح>& starts, ثابت std::vector<صحیح>& ends,
             ثابت std::vector<صحیح>& اقدام, OutputArray _dst)
{
    CV_INSTRUMENT_REGION();

    Mat src = _src.getMat();
    ثابت صحیح ndims = src.dims, nslices = (صحیح)starts.size();
    CV_Assert(!(ndims == 0 && src.empty()));
    CV_CheckLE(nslices, ndims, "slice: too many starts");
    CV_CheckEQ(ends.size(), starts.size(), "slice: starts and ends must have the same size");
    CV_Check(اقدام.size(), اقدام.empty() || اقدام.size() == starts.size(),
             "slice: steps must be empty or have the same size as starts");

    nd::View sv = nd::view_کا(src);
    صحیح برآمد_Shape[CV_MAX_DIM];
    برائے (صحیح i = 0; i < ndims; i++)
        برآمد_Shape[i] = src.size[i];

    برائے (صحیح i = 0; i < nslices; i++)
    {
        ثابت صحیح n = src.size[i], آغاز = starts[i], end = ends[i];
        ثابت صحیح قدم = اقدام.empty() ? 1 : اقدام[i];
        CV_CheckNE(قدم, 0, "slice: step must not be zero");
        صحیح گنتی_2;
        اگر (قدم > 0)
        {
            CV_Check(آغاز, 0 <= آغاز && آغاز <= n, "slice: start is out of range");
            CV_Check(end, 0 <= end && end <= n, "slice: end is out of range");
            گنتی_2 = end > آغاز ? (صحیح)(((int64)end - آغاز + قدم - 1)/قدم) : 0;
        }
        ورنہ
        {
            CV_Check(آغاز, -1 <= آغاز && آغاز < n, "slice: start is out of range");
            CV_Check(end, -1 <= end && end < n, "slice: end is out of range");
            گنتی_2 = آغاز > end ? (صحیح)(((int64)آغاز - end - قدم - 1)/(-قدم)) : 0;
        }
        nd::قاش_2(sv, i, آغاز, قدم, گنتی_2);
        برآمد_Shape[i] = گنتی_2;
    }

    _dst.create(ndims, برآمد_Shape, src.type());
    Mat dst = _dst.getMat();
    اگر (dst.کل() == 0 || src.empty())
        واپس;
    nd::copy(sv, nd::view_کا(dst));
}

/*
    This function first prepends 1 to each tensor shape to have a common max_ndims dimension, then flatten non-broadcast dimensions.
*/
ساکن منطقی flatten_for_نشر(صحیح narrays, صحیح max_ndims, ثابت صحیح* ndims, ثابت صحیح** orig_shape,
                                   صحیح** flatten_shape, size_t** flatten_قدم) {
    صحیح i, j, k;

    // step 1.
    // * make all inputs and the output max_ndims-dimensional.
    // * compute proper step's
    برائے (i = max_ndims - 1; i >= 0; i-- ) {
        برائے (k = 0; k < narrays; k++) {
            j = ndims[k] - (max_ndims - i);
            صحیح sz_i = j >= 0 ? orig_shape[k][j] : 1;
            size_t st_i = i == max_ndims - 1 ? 1 : flatten_قدم[k][i+1] * flatten_shape[k][i+1];
            flatten_shape[k][i] = sz_i;
            flatten_قدم[k][i] = st_i;
            اگر (flatten_shape[k][i] == 0)
                واپس جھوٹ;
        }
    }

    // step 2. Let's do the flattening first,
    // since we'd need proper values of steps to check continuity.
    // this loop is probably the most tricky part
    // in the whole implementation of broadcasting.
    j = max_ndims-1;
    برائے (i = j - 1; i >= 0; i--) {
        منطقی all_contiguous = سچ, all_scalars = سچ, all_consistent = سچ;
        برائے(k = 0; k < narrays; k++) {
            size_t st = flatten_قدم[k][j] * flatten_shape[k][j];
            منطقی پچھلا_scalar = flatten_shape[k][j] == 1;
            منطقی scalar = flatten_shape[k][i] == 1;
            all_contiguous = all_contiguous && (st == flatten_قدم[k][i]);
            all_scalars = all_scalars && scalar;
            all_consistent = all_consistent && (scalar == پچھلا_scalar);
        }
        اگر (all_contiguous && (all_consistent || all_scalars)) {
            برائے(k = 0; k < narrays; k++)
                flatten_shape[k][j] *= flatten_shape[k][i];
        } ورنہ {
            j--;
            اگر (i < j) {
                برائے(k = 0; k < narrays; k++) {
                    flatten_shape[k][j] = flatten_shape[k][i];
                    flatten_قدم[k][j] = flatten_قدم[k][i];
                }
            }
        }
    }

    // step 3. Set some step's to 0's.
    برائے (i = max_ndims-1; i >= j; i--) {
        برائے (k = 0; k < narrays; k++)
            flatten_قدم[k][i] = flatten_shape[k][i] == 1 ? 0 : flatten_قدم[k][i];
    }
    برائے (; i >= 0; i--) {
        برائے (k = 0; k < narrays; k++) {
            flatten_قدم[k][i] = 0;
            flatten_shape[k][i] = 1;
        }
    }
    واپس سچ;
}

خالی نشر(داخلہ_Array _src, داخلہ_Array _shape, OutputArray _dst) {
    CV_INSTRUMENT_REGION();

    Mat src = _src.getMat();
    CV_CheckTrue(src.isContinuous(), "broadcast: input array must be contiguous");
    CV_CheckChannelsEQ(src.channels(), 1, "broadcast: input array must be single channel");

    Mat shape = _shape.getMat();
    CV_CheckTypeEQ(shape.type(), CV_32S, "broadcast: target shape must be of type int32");
    ثابت خودکار dims_shape = static_cast<صحیح>(shape.کل());
    ثابت خودکار *ptr_shape = shape.ptr<صحیح>();

    // check valid shape, 1D/0D Mat would fail in the following checks
    ثابت خودکار dims_src = src.dims;
    CV_CheckLE(dims_src, dims_shape,
               "broadcast: dimension of input array must be less than or equal to dimension of target shape");
    std::vector<صحیح> shape_src{src.size.p, src.size.p + dims_src};
    اگر (shape_src.size() < static_cast<size_t>(dims_shape)) {
        shape_src.insert(shape_src.begin(), dims_shape - shape_src.size(), 1);
    }
    برائے (صحیح i = 0; i < static_cast<صحیح>(shape_src.size()); ++i) {
        ثابت خودکار *shape_ہدف = ptr_shape;
        اگر (shape_src[i] != 1) {
            CV_CheckEQ(shape_src[i], shape_ہدف[i], "target shape must be equal to input shape or 1");
        }
    }

    // impl
    _dst.create(dims_shape, shape.ptr<صحیح>(), src.type());
    Mat dst = _dst.getMat();
    اگر (dst.کل() == 0)
        واپس;
    cv::Auto_بفر<صحیح> is_same_shape(dims_shape, 0);
    برائے (صحیح i = 0; i < static_cast<صحیح>(shape_src.size()); ++i) {
        اگر (shape_src[i] == ptr_shape[i]) {
            is_same_shape[i] = 1;
        }
    }
    // copy if same shape
    اگر (std::accumulate(is_same_shape.begin(), is_same_shape.end(), 1, std::multiplies<صحیح>()) != 0) {
        ثابت خودکار *p_src = src.ptr<ثابت حرف>();
        خودکار *p_dst = dst.ptr<حرف>();
        std::حافظہ_نقل(p_dst, p_src, dst.کل() * dst.elem_سائز());
        واپس;
    }
    // other cases
    صحیح max_ndims = std::max(dims_src, dims_shape);
    اگر (max_ndims < 2 && src.کل() == 1) {
        ثابت حرف* p_src = src.ptr<ثابت حرف>();
        حرف* p_dst = dst.ptr<حرف>();
        size_t esz = src.elem_سائز();
        برائے (size_t j = 0; j < dst.کل(); j++)
            std::حافظہ_نقل(p_dst + j * esz, p_src, esz);
        واپس;
    }
    ثابت صحیح all_ndims[2] = {src.dims, dst.dims};
    ثابت صحیح* orig_shapes[2] = {src.size.p, dst.size.p};
    cv::Auto_بفر<size_t> buff(max_ndims * 4);
    صحیح* flatten_shapes[2] = {(صحیح*)buff.ڈیٹا(), (صحیح*)(buff.ڈیٹا() + max_ndims)};
    size_t* flatten_اقدام[2] = {(size_t*)(buff.ڈیٹا() + 2 * max_ndims), (size_t*)(buff.ڈیٹا() + 3 * max_ndims)};
    اگر (flatten_for_نشر(2, max_ndims, all_ndims, orig_shapes, flatten_shapes, flatten_اقدام)) {
        size_t src_dp = flatten_اقدام[0][max_ndims - 1];
        size_t dst_dp = flatten_اقدام[1][max_ndims - 1];
        CV_Assert(dst_dp == 1 || dst_dp == 0);
        CV_Assert(max_ndims >= 2); // >= 3?
        size_t rowstep_src = flatten_اقدام[0][max_ndims - 2];
        size_t rowstep_dst = flatten_اقدام[1][max_ndims - 2];
        ثابت حرف* ptr_src = src.ptr<ثابت حرف>();
        حرف* ptr_dst = dst.ptr<حرف>();
        size_t esz = src.elem_سائز();
        صحیح nrows = flatten_shapes[1][max_ndims - 2];
        صحیح ncols = flatten_shapes[1][max_ndims - 1];
        صحیح nplanes = 1;
        CV_Check(esz, esz == 1 || esz == 2 || esz == 4 || esz == 8, "broadcast: not supported data type");

        برائے (صحیح k = 0; k < max_ndims - 2; k++) {
            nplanes *= flatten_shapes[1][k];
        }
        برائے (صحیح plane_idx = 0; plane_idx < nplanes; plane_idx++) {
            size_t انحراف_src = 0, انحراف_dst = 0;
            size_t idx = (size_t)plane_idx;
            برائے (صحیح k = max_ndims - 3; k >= 0; k--) {
                size_t پچھلا_idx = idx / flatten_shapes[1][k];
                size_t i_k = (صحیح)(idx - پچھلا_idx * flatten_shapes[1][k]);
                انحراف_src += i_k * flatten_اقدام[0][k];
                انحراف_dst += i_k * flatten_اقدام[1][k];
                idx = پچھلا_idx;
            }

            #تعریف_کرو OPENCV_CORE_نشر_LOOP(_Tp) \
                برائے (صحیح i = 0; i < nrows; i++) {   \
                    ثابت _Tp *ptr_src_ = (ثابت _Tp*)ptr_src + انحراف_src + rowstep_src * i; \
                    _Tp *ptr_dst_ = (_Tp*)ptr_dst + انحراف_dst + rowstep_dst * i; \
                    اگر (src_dp == 1) { \
                        برائے (صحیح j = 0; j < ncols; j++) { \
                            ptr_dst_[j] = ptr_src_[j]; \
                        } \
                    } ورنہ { \
                        _Tp x = *ptr_src_; \
                        برائے (صحیح j = 0; j < ncols; j++) { \
                            ptr_dst_[j] = x; \
                        } \
                    } \
                }

            اگر (esz == 1) {
                OPENCV_CORE_نشر_LOOP(int8_t);
            } ورنہ اگر (esz == 2) {
                OPENCV_CORE_نشر_LOOP(int16_t);
            } ورنہ اگر (esz == 4) {
                OPENCV_CORE_نشر_LOOP(int32_t);
            } ورنہ اگر (esz == 8) {
                OPENCV_CORE_نشر_LOOP(int64_t);
            } ورنہ {
                CV_Error(cv::Error::StsNotImplemented, "");
            }
            #تعریف_منسوخ OPENCV_CORE_نشر_LOOP
        }
    } ورنہ {
        اگر (dst.کل() == 0)
            واپس;
        // initial copy (src to dst)
        std::vector<size_t> قدم_src{src.قدم.p, src.قدم.p + dims_src};
        اگر (قدم_src.size() < static_cast<size_t>(dims_shape)) {
            قدم_src.insert(قدم_src.begin(), dims_shape - قدم_src.size(), قدم_src[0]);
        }
        برائے (size_t i = 0; i < src.کل(); ++i) {
            size_t t = i;
            size_t src_انحراف = 0, dst_انحراف = 0;
            برائے (صحیح j = static_cast<صحیح>(shape_src.size() - 1); j >= 0; --j) {
                size_t idx = t / shape_src[j];
                size_t offset = static_cast<size_t>(t - idx * shape_src[j]);
                src_انحراف += offset * قدم_src[j];
                dst_انحراف += offset * dst.قدم[j];
                t = idx;
            }
            ثابت خودکار *p_src = src.ptr<ثابت حرف>();
            خودکار *p_dst = dst.ptr<حرف>();
            std::حافظہ_نقل(p_dst + dst_انحراف, p_src + src_انحراف, dst.elem_سائز());
        }
        // broadcast copy (dst inplace)
        Auto_بفر<صحیح> cumulative_shape(dims_shape, 1);
        صحیح کل = static_cast<صحیح>(dst.کل());
        برائے (صحیح i = dims_shape - 1; i >= 0; --i) {
            cumulative_shape[i] = static_cast<صحیح>(کل / ptr_shape[i]);
            کل = cumulative_shape[i];
        }
        برائے (صحیح i = dims_shape - 1; i >= 0; --i) {
            اگر (is_same_shape[i] == 1) {
                جاری;
            }
            خودکار قدم = dst.قدم[i];
            خودکار *p_dst = dst.ptr<حرف>();
            برائے (صحیح j = 0; j < cumulative_shape[i]; j++) {
                برائے (صحیح k = 0; k < ptr_shape[i] - 1; k++) {
                    std::حافظہ_نقل(p_dst + قدم, p_dst, قدم);
                    p_dst += قدم;
                }
                p_dst += قدم;
            }
        }
    }
}

خالی نشر(داخلہ_Array _src, ثابت MatShape& _shape, OutputArray _dst)
{
    اگر (_shape.dims < 0) {
        _dst.release();
    } ورنہ {
        Mat shape(1, _shape.dims, CV_32S, (صحیح*)_shape.p);
        نشر(_src, shape, _dst);
    }
}

ساکن خالی rotateImpl(داخلہ_Array _src, OutputArray _dst, صحیح rotate_منوال)
{
    انتخاب (rotate_منوال)
    {
    حالت ROTATE_90_CLOCKWISE:
        transpose(_src, _dst);
        flip(_dst, _dst, 1);
        توڑو;
    حالت ROTATE_180:
        flip(_src, _dst, -1);
        توڑو;
    حالت ROTATE_90_COUNTERCLOCKWISE:
        transpose(_src, _dst);
        flip(_dst, _dst, 0);
        توڑو;
    طےشدہ:
        توڑو;
    }
}

خالی rotate(داخلہ_Array _src, OutputArray _dst, صحیح rotate_منوال)
{
    CV_Assert(_src.dims() <= 2);
    صحیح angle;

    اگر (_dst.isUMat())
    {
        rotateImpl(_src, _dst, rotate_منوال);
        واپس;
    }

    Mat src = _src.getMat();
    صحیح type = src.type();
    اگر( src.empty() )
    {
        _dst.release();
        واپس;
    }

    انتخاب (rotate_منوال)
    {
    حالت ROTATE_90_CLOCKWISE:
        _dst.create(src.ستون_تعداد, src.صفیں, type);
        angle = 90;
        توڑو;
    حالت ROTATE_180:
        _dst.create(src.صفیں, src.ستون_تعداد, type);
        angle = 180;
        توڑو;
    حالت ROTATE_90_COUNTERCLOCKWISE:
        _dst.create(src.ستون_تعداد, src.صفیں, type);
        angle = 270;
        توڑو;
    طےشدہ:
        _dst.create(src.صفیں, src.ستون_تعداد, type);
        angle = 0;
        توڑو;
    }

    Mat dst = _dst.getMat();
    CALL_HAL(rotate90, cv_hal_rotate90, type, src.ptr(), src.قدم, src.ستون_تعداد, src.صفیں,
             dst.ptr(), dst.قدم, angle);

    // use src (Mat) since _src (InputArray) is updated by _dst.create() when in-place
    rotateImpl(src, _dst, rotate_منوال);
}

}  // namespace
