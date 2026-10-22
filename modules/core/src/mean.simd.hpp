// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html

// Copyright (C) 2026, Advanced Micro Devices, Inc., all rights reserved.

#شامل "precomp.hpp"
#شامل "stat.hpp"

نامخلا cv {
قسمتعریف صحیح (*مجموع_Sqr_Func)(ثابت uchar*, ثابت uchar* نقاب, uchar*, uchar*, صحیح, صحیح);

CV_CPU_OPTIMIZATION_NAMESPACE_BEGIN

سانچہ<قسم_نام T, قسم_نام ST, قسم_نام SQT, صحیح (*fn)(ثابت T*, ثابت uchar*, ST*, SQT*, صحیح, صحیح)>
ساکن صحیح مجموع_Sqr_Wrap(ثابت uchar* src, ثابت uchar* نقاب, uchar* مجموع, uchar* sqsum, صحیح len, صحیح cn)
{
    واپس fn((ثابت T*)src, نقاب, (ST*)مجموع, (SQT*)sqsum, len, cn);
}

مجموع_Sqr_Func get_مجموع_Sqr_Func(صحیح گہرائی);

#اگر_غیر_تعریف_شدہ CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

سانچہ <قسم_نام T, قسم_نام ST, قسم_نام SQT>
ساخت مجموع_Sqr_SIMD
{
    اندرونی صحیح عامل () (ثابت T *, ثابت uchar *, ST *, SQT *, صحیح, صحیح) ثابت
    {
        واپس 0;
    }
};

#اگر (CV_SIMD || CV_SIMD_SCALABLE)

سانچہ <>
ساخت مجموع_Sqr_SIMD<uchar, صحیح, صحیح>
{
    صحیح عامل () (ثابت uchar * src0, ثابت uchar * نقاب, صحیح * مجموع, صحیح * sqsum, صحیح len, صحیح cn) ثابت
    {
        اگر (نقاب)
            واپس 0;
        // cn==3: deinterleave so each lane belongs to one channel, then reduce per
        // channel. sum/sqsum stay exact in s32 within the dispatcher's 1<<15 block.
        اگر (cn == 3)
        {
            ثابت صحیح vl8 = VTraits<v_uint8>::vlanes();
            v_int32 vs0 = vx_setzero_s32(), vs1 = vx_setzero_s32(), vs2 = vx_setzero_s32();
            v_int32 vq0 = vx_setzero_s32(), vq1 = vx_setzero_s32(), vq2 = vx_setzero_s32();
            خودکار acc = [](ثابت v_uint8& ch, v_int32& vs, v_int32& vq)
            {
                v_uint16 lo, hi; v_expand(ch, lo, hi);
                v_uint32 s0, s1, s2, s3; v_expand(lo, s0, s1); v_expand(hi, s2, s3);
                vs = v_جوڑو(vs, v_reinterpret_as_s32(v_جوڑو(v_جوڑو(s0, s1), v_جوڑو(s2, s3))));
                v_int16 d0 = v_reinterpret_as_s16(lo), d1 = v_reinterpret_as_s16(hi);
                vq = v_جوڑو(vq, v_جوڑو(v_dotprod(d0, d0), v_dotprod(d1, d1)));
            };
            صحیح e = 0;
            برائے (; e <= len - vl8; e += vl8)
            {
                v_uint8 a, b, c; v_بھرو_deinterleave(src0 + e * 3, a, b, c);
                acc(a, vs0, vq0); acc(b, vs1, vq1); acc(c, vs2, vq2);
            }
            مجموع[0] += v_reduce_مجموع(vs0); مجموع[1] += v_reduce_مجموع(vs1); مجموع[2] += v_reduce_مجموع(vs2);
            sqsum[0] += v_reduce_مجموع(vq0); sqsum[1] += v_reduce_مجموع(vq1); sqsum[2] += v_reduce_مجموع(vq2);
            v_cleanup();
            واپس e;
        }
        اگر (cn != 1 && cn != 2 && cn != 4)
            واپس 0;
        len *= cn;

        صحیح x = 0;
        v_int32 v_مجموع = vx_setzero_s32();
        v_int32 v_sqsum = vx_setzero_s32();

        ثابت صحیح len0 = len & -VTraits<v_uint8>::vlanes();
        جب(x < len0)
        {
            ثابت صحیح len_tmp = min(x + 256*VTraits<v_uint16>::vlanes(), len0);
            v_uint16 v_sum16 = vx_setzero_u16();
            برائے ( ; x < len_tmp; x += VTraits<v_uint8>::vlanes())
            {
                v_uint16 v_src0 = vx_بھرو_expand(src0 + x);
                v_uint16 v_src1 = vx_بھرو_expand(src0 + x + VTraits<v_uint16>::vlanes());
                v_sum16 = v_جوڑو(v_sum16, v_جوڑو(v_src0, v_src1));
                v_int16 v_tmp0, v_tmp1;
                v_zip(v_reinterpret_as_s16(v_src0), v_reinterpret_as_s16(v_src1), v_tmp0, v_tmp1);
                v_sqsum = v_جوڑو(v_sqsum, v_جوڑو(v_dotprod(v_tmp0, v_tmp0), v_dotprod(v_tmp1, v_tmp1)));
            }
            v_uint32 v_half0, v_half1;
            v_expand(v_sum16, v_half0, v_half1);
            v_مجموع = v_جوڑو(v_مجموع, v_reinterpret_as_s32(v_جوڑو(v_half0, v_half1)));
        }
        اگر (x <= len - VTraits<v_uint16>::vlanes())
        {
            v_uint16 v_src = vx_بھرو_expand(src0 + x);
            v_uint16 v_half = v_combine_بلند(v_src, v_src);

            v_uint32 v_tmp0, v_tmp1;
            v_expand(v_جوڑو(v_src, v_half), v_tmp0, v_tmp1);
            v_مجموع = v_جوڑو(v_مجموع, v_reinterpret_as_s32(v_tmp0));

            v_int16 v_tmp2, v_tmp3;
            v_zip(v_reinterpret_as_s16(v_src), v_reinterpret_as_s16(v_half), v_tmp2, v_tmp3);
            v_sqsum = v_جوڑو(v_sqsum, v_dotprod(v_tmp2, v_tmp2));
            x += VTraits<v_uint16>::vlanes();
        }

        اگر (cn == 1)
        {
            *مجموع += v_reduce_مجموع(v_مجموع);
            *sqsum += v_reduce_مجموع(v_sqsum);
        }
        ورنہ
        {
            صحیح CV_DECL_ALIGNED(CV_SIMD_چوڑائی) ar[2 * VTraits<v_int32>::max_nlanes];
            v_store(ar, v_مجموع);
            v_store(ar + VTraits<v_int32>::vlanes(), v_sqsum);
            برائے (صحیح i = 0; i < VTraits<v_int32>::vlanes(); ++i)
            {
                مجموع[i % cn] += ar[i];
                sqsum[i % cn] += ar[VTraits<v_int32>::vlanes() + i];
            }
        }
        v_cleanup();
        واپس x / cn;
    }
};

سانچہ <>
ساخت مجموع_Sqr_SIMD<schar, صحیح, صحیح>
{
    صحیح عامل () (ثابت schar * src0, ثابت uchar * نقاب, صحیح * مجموع, صحیح * sqsum, صحیح len, صحیح cn) ثابت
    {
        اگر (نقاب)
            واپس 0;
        اگر (cn == 3)
        {
            ثابت صحیح vl8 = VTraits<v_int8>::vlanes();
            v_int32 vs0 = vx_setzero_s32(), vs1 = vx_setzero_s32(), vs2 = vx_setzero_s32();
            v_int32 vq0 = vx_setzero_s32(), vq1 = vx_setzero_s32(), vq2 = vx_setzero_s32();
            خودکار acc = [](ثابت v_int8& ch, v_int32& vs, v_int32& vq)
            {
                v_int16 lo, hi; v_expand(ch, lo, hi);
                v_int32 s0, s1, s2, s3; v_expand(lo, s0, s1); v_expand(hi, s2, s3);
                vs = v_جوڑو(vs, v_جوڑو(v_جوڑو(s0, s1), v_جوڑو(s2, s3)));
                vq = v_جوڑو(vq, v_جوڑو(v_dotprod(lo, lo), v_dotprod(hi, hi)));
            };
            صحیح e = 0;
            برائے (; e <= len - vl8; e += vl8)
            {
                v_int8 a, b, c; v_بھرو_deinterleave(src0 + e * 3, a, b, c);
                acc(a, vs0, vq0); acc(b, vs1, vq1); acc(c, vs2, vq2);
            }
            مجموع[0] += v_reduce_مجموع(vs0); مجموع[1] += v_reduce_مجموع(vs1); مجموع[2] += v_reduce_مجموع(vs2);
            sqsum[0] += v_reduce_مجموع(vq0); sqsum[1] += v_reduce_مجموع(vq1); sqsum[2] += v_reduce_مجموع(vq2);
            v_cleanup();
            واپس e;
        }
        اگر (cn != 1 && cn != 2 && cn != 4)
            واپس 0;
        len *= cn;

        صحیح x = 0;
        v_int32 v_مجموع = vx_setzero_s32();
        v_int32 v_sqsum = vx_setzero_s32();

        ثابت صحیح len0 = len & -VTraits<v_int8>::vlanes();
        جب (x < len0)
        {
            ثابت صحیح len_tmp = min(x + 256 * VTraits<v_int16>::vlanes(), len0);
            v_int16 v_sum16 = vx_setzero_s16();
            برائے (; x < len_tmp; x += VTraits<v_int8>::vlanes())
            {
                v_int16 v_src0 = vx_بھرو_expand(src0 + x);
                v_int16 v_src1 = vx_بھرو_expand(src0 + x + VTraits<v_int16>::vlanes());
                v_sum16 = v_جوڑو(v_sum16, v_جوڑو(v_src0, v_src1));
                v_int16 v_tmp0, v_tmp1;
                v_zip(v_src0, v_src1, v_tmp0, v_tmp1);
                v_sqsum = v_جوڑو(v_sqsum, v_جوڑو(v_dotprod(v_tmp0, v_tmp0), v_dotprod(v_tmp1, v_tmp1)));
            }
            v_int32 v_half0, v_half1;
            v_expand(v_sum16, v_half0, v_half1);
            v_مجموع = v_جوڑو(v_مجموع, v_جوڑو(v_half0, v_half1));
        }
        اگر (x <= len - VTraits<v_int16>::vlanes())
        {
            v_int16 v_src = vx_بھرو_expand(src0 + x);
            v_int16 v_half = v_combine_بلند(v_src, v_src);

            v_int32 v_tmp0, v_tmp1;
            v_expand(v_جوڑو(v_src, v_half), v_tmp0, v_tmp1);
            v_مجموع = v_جوڑو(v_مجموع, v_tmp0);

            v_int16 v_tmp2, v_tmp3;
            v_zip(v_src, v_half, v_tmp2, v_tmp3);
            v_sqsum = v_جوڑو(v_sqsum, v_dotprod(v_tmp2, v_tmp2));
            x += VTraits<v_int16>::vlanes();
        }

        اگر (cn == 1)
        {
            *مجموع += v_reduce_مجموع(v_مجموع);
            *sqsum += v_reduce_مجموع(v_sqsum);
        }
        ورنہ
        {
            صحیح CV_DECL_ALIGNED(CV_SIMD_چوڑائی) ar[2 * VTraits<v_int32>::max_nlanes];
            v_store(ar, v_مجموع);
            v_store(ar + VTraits<v_int32>::vlanes(), v_sqsum);
            برائے (صحیح i = 0; i < VTraits<v_int32>::vlanes(); ++i)
            {
                مجموع[i % cn] += ar[i];
                sqsum[i % cn] += ar[VTraits<v_int32>::vlanes() + i];
            }
        }
        v_cleanup();
        واپس x / cn;
    }
};

#اگر (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)

// Expand a 16-bit vector into two s32 halves. The unsigned overload reinterprets
// after a widening expand (values 0..65535 stay non-negative in s32, so the
// subsequent v_cvt_f64 is exact); the signed overload expands directly. This is
// the only real difference between the ushort and short reductions below.
ساکن اندرونی خالی مجموع_Sqr_Expand_S32(ثابت v_uint16& v, v_int32& lo, v_int32& hi)
{
    v_uint32 a, b; v_expand(v, a, b);
    lo = v_reinterpret_as_s32(a); hi = v_reinterpret_as_s32(b);
}
ساکن اندرونی خالی مجموع_Sqr_Expand_S32(ثابت v_int16& v, v_int32& lo, v_int32& hi)
{
    v_expand(v, lo, hi);
}

// Shared 16-bit -> {s32 sum, f64 sqsum} reduction for ushort/short. The register
// type is deduced from the pointer, so the body is identical for both depths.
سانچہ <قسم_نام T>
ساکن اندرونی صحیح مجموع_Sqr16_مقصد_F64(ثابت T* src0, صحیح* مجموع, دوہرا* sqsum, صحیح len, صحیح cn)
{
    قسمتعریف decltype(vx_بھرو(src0)) VT; // v_uint16 or v_int16

    اگر (cn == 3)
    {
        ثابت صحیح vl = VTraits<VT>::vlanes();
        v_int32 vs0 = vx_setzero_s32(), vs1 = vx_setzero_s32(), vs2 = vx_setzero_s32();
        v_float64 vq0 = vx_setzero_f64(), vq1 = vx_setzero_f64(), vq2 = vx_setzero_f64();
        خودکار acc = [](ثابت VT& ch, v_int32& vs, v_float64& vq)
        {
            v_int32 lo, hi; مجموع_Sqr_Expand_S32(ch, lo, hi);
            vs = v_جوڑو(vs, v_جوڑو(lo, hi));
            v_float64 f0 = v_cvt_f64(lo), f1 = v_cvt_f64_بلند(lo);
            v_float64 f2 = v_cvt_f64(hi), f3 = v_cvt_f64_بلند(hi);
            vq = v_fma(f0, f0, v_fma(f1, f1, v_fma(f2, f2, v_fma(f3, f3, vq))));
        };
        صحیح e = 0;
        برائے (; e <= len - vl; e += vl)
        {
            VT a, b, c; v_بھرو_deinterleave(src0 + e * 3, a, b, c);
            acc(a, vs0, vq0); acc(b, vs1, vq1); acc(c, vs2, vq2);
        }
        مجموع[0] += v_reduce_مجموع(vs0); مجموع[1] += v_reduce_مجموع(vs1); مجموع[2] += v_reduce_مجموع(vs2);
        sqsum[0] += v_reduce_مجموع(vq0); sqsum[1] += v_reduce_مجموع(vq1); sqsum[2] += v_reduce_مجموع(vq2);
        v_cleanup();
        واپس e;
    }
    اگر (cn != 1 && cn != 2 && cn != 4)
        واپس 0;
    len *= cn;

    ثابت صحیح vl16 = VTraits<VT>::vlanes();
    ثابت صحیح vl32 = VTraits<v_int32>::vlanes();
    ثابت صحیح vl64 = VTraits<v_float64>::vlanes();

    // The lane->channel scatter below (i % cn) and the returned pixel count
    // (x / cn) require the lane counts to be a multiple of cn. This always holds
    // for fixed-width SIMD (vlanes is a power of two >= cn for cn in {1,2,4}),
    // but scalable backends (e.g. RVV) may report a non-multiple; fall back to
    // scalar there. vl16 == 2*vl32, so checking vl32 covers both scatter loops.
    اگر (vl32 % cn != 0)
        واپس 0;

    v_int32 v_مجموع = vx_setzero_s32();
    v_float64 v_sq0 = vx_setzero_f64(), v_sq1 = vx_setzero_f64();
    v_float64 v_sq2 = vx_setzero_f64(), v_sq3 = vx_setzero_f64();

    صحیح x = 0;
    برائے (; x <= len - vl16; x += vl16)
    {
        VT v_src = vx_بھرو(src0 + x);
        v_int32 lo, hi; مجموع_Sqr_Expand_S32(v_src, lo, hi);
        v_مجموع = v_جوڑو(v_مجموع, v_جوڑو(lo, hi));

        v_float64 f0 = v_cvt_f64(lo);
        v_float64 f1 = v_cvt_f64_بلند(lo);
        v_float64 f2 = v_cvt_f64(hi);
        v_float64 f3 = v_cvt_f64_بلند(hi);
        v_sq0 = v_fma(f0, f0, v_sq0);
        v_sq1 = v_fma(f1, f1, v_sq1);
        v_sq2 = v_fma(f2, f2, v_sq2);
        v_sq3 = v_fma(f3, f3, v_sq3);
    }

    صحیح    CV_DECL_ALIGNED(CV_SIMD_چوڑائی) sbuf[VTraits<v_int32>::max_nlanes];
    دوہرا CV_DECL_ALIGNED(CV_SIMD_چوڑائی) qbuf[VTraits<v_float64>::max_nlanes * 4];
    v_store_aligned(sbuf, v_مجموع);
    v_store_aligned(qbuf + vl64 * 0, v_sq0);
    v_store_aligned(qbuf + vl64 * 1, v_sq1);
    v_store_aligned(qbuf + vl64 * 2, v_sq2);
    v_store_aligned(qbuf + vl64 * 3, v_sq3);

    // sbuf lane j (j<vl32) holds samples j and j+vl32 -> channel j%cn (vl32%cn==0).
    // qbuf index i (i<vl16) holds sample lane i           -> channel i%cn (vl16%cn==0).
    برائے (صحیح i = 0; i < vl32; ++i)
        مجموع[i % cn] += sbuf[i];
    برائے (صحیح i = 0; i < vl16; ++i)
        sqsum[i % cn] += qbuf[i];

    v_cleanup();
    واپس x / cn;
}

// Shared 32-bit -> f64 reduction for int/float. Both sum and sqsum accumulate in
// f64 to match the scalar reference's double accumulation; values are widened to
// f64 before squaring so the square is computed in double (same as (double)v*v).
// The register type is deduced from the pointer, so int and float share one body.
سانچہ <قسم_نام T>
ساکن اندرونی صحیح مجموع_Sqr32_مقصد_F64(ثابت T* src0, دوہرا* مجموع, دوہرا* sqsum, صحیح len, صحیح cn)
{
    قسمتعریف decltype(vx_بھرو(src0)) VT; // v_int32 or v_float32

    اگر (cn == 3)
    {
        ثابت صحیح vl = VTraits<VT>::vlanes();
        v_float64 vs0 = vx_setzero_f64(), vs1 = vx_setzero_f64(), vs2 = vx_setzero_f64();
        v_float64 vq0 = vx_setzero_f64(), vq1 = vx_setzero_f64(), vq2 = vx_setzero_f64();
        خودکار acc = [](ثابت VT& ch, v_float64& vs, v_float64& vq)
        {
            v_float64 f0 = v_cvt_f64(ch), f1 = v_cvt_f64_بلند(ch);
            vs = v_جوڑو(vs, v_جوڑو(f0, f1));
            vq = v_fma(f0, f0, v_fma(f1, f1, vq));
        };
        صحیح e = 0;
        برائے (; e <= len - vl; e += vl)
        {
            VT a, b, c; v_بھرو_deinterleave(src0 + e * 3, a, b, c);
            acc(a, vs0, vq0); acc(b, vs1, vq1); acc(c, vs2, vq2);
        }
        مجموع[0] += v_reduce_مجموع(vs0); مجموع[1] += v_reduce_مجموع(vs1); مجموع[2] += v_reduce_مجموع(vs2);
        sqsum[0] += v_reduce_مجموع(vq0); sqsum[1] += v_reduce_مجموع(vq1); sqsum[2] += v_reduce_مجموع(vq2);
        v_cleanup();
        واپس e;
    }
    اگر (cn != 1 && cn != 2 && cn != 4)
        واپس 0;
    len *= cn;

    ثابت صحیح vl32 = VTraits<VT>::vlanes();
    ثابت صحیح vl64 = VTraits<v_float64>::vlanes();

    // See note in sumSqr16ToF64: guard scalable backends whose lane count may
    // not divide cn (the i % cn scatter / x / cn rely on it).
    اگر (vl32 % cn != 0)
        واپس 0;

    v_float64 vs0 = vx_setzero_f64(), vs1 = vx_setzero_f64();
    v_float64 vq0 = vx_setzero_f64(), vq1 = vx_setzero_f64();

    صحیح x = 0;
    برائے (; x <= len - vl32; x += vl32)
    {
        VT v_src = vx_بھرو(src0 + x);
        v_float64 f0 = v_cvt_f64(v_src);
        v_float64 f1 = v_cvt_f64_بلند(v_src);
        vs0 = v_جوڑو(vs0, f0);
        vs1 = v_جوڑو(vs1, f1);
        vq0 = v_fma(f0, f0, vq0);
        vq1 = v_fma(f1, f1, vq1);
    }

    دوہرا CV_DECL_ALIGNED(CV_SIMD_چوڑائی) sbuf[VTraits<v_float64>::max_nlanes * 2];
    دوہرا CV_DECL_ALIGNED(CV_SIMD_چوڑائی) qbuf[VTraits<v_float64>::max_nlanes * 2];
    v_store_aligned(sbuf, vs0);          v_store_aligned(sbuf + vl64, vs1);
    v_store_aligned(qbuf, vq0);          v_store_aligned(qbuf + vl64, vq1);

    برائے (صحیح i = 0; i < vl32; ++i)
    {
        مجموع[i % cn]   += sbuf[i];
        sqsum[i % cn] += qbuf[i];
    }

    v_cleanup();
    واپس x / cn;
}

سانچہ <>
ساخت مجموع_Sqr_SIMD<ushort, صحیح, دوہرا>
{
    صحیح عامل () (ثابت ushort* src0, ثابت uchar* نقاب, صحیح* مجموع, دوہرا* sqsum, صحیح len, صحیح cn) ثابت
    { واپس نقاب ? 0 : مجموع_Sqr16_مقصد_F64(src0, مجموع, sqsum, len, cn); }
};

سانچہ <>
ساخت مجموع_Sqr_SIMD<مختصر, صحیح, دوہرا>
{
    صحیح عامل () (ثابت مختصر* src0, ثابت uchar* نقاب, صحیح* مجموع, دوہرا* sqsum, صحیح len, صحیح cn) ثابت
    { واپس نقاب ? 0 : مجموع_Sqr16_مقصد_F64(src0, مجموع, sqsum, len, cn); }
};

سانچہ <>
ساخت مجموع_Sqr_SIMD<صحیح, دوہرا, دوہرا>
{
    صحیح عامل () (ثابت صحیح* src0, ثابت uchar* نقاب, دوہرا* مجموع, دوہرا* sqsum, صحیح len, صحیح cn) ثابت
    { واپس نقاب ? 0 : مجموع_Sqr32_مقصد_F64(src0, مجموع, sqsum, len, cn); }
};

سانچہ <>
ساخت مجموع_Sqr_SIMD<اعشاری, دوہرا, دوہرا>
{
    صحیح عامل () (ثابت اعشاری* src0, ثابت uchar* نقاب, دوہرا* مجموع, دوہرا* sqsum, صحیح len, صحیح cn) ثابت
    { واپس نقاب ? 0 : مجموع_Sqr32_مقصد_F64(src0, مجموع, sqsum, len, cn); }
};

#اگر_ختم // CV_SIMD_64F

#اگر_ختم

سانچہ<قسم_نام T, قسم_نام ST, قسم_نام SQT>
ساکن صحیح sumsqr_(ثابت T* src0, ثابت uchar* نقاب, ST* مجموع, SQT* sqsum, صحیح len, صحیح cn )
{
    ثابت T* src = src0;

    اگر( !نقاب )
    {
        مجموع_Sqr_SIMD<T, ST, SQT> vop;
        صحیح x = vop(src0, نقاب, مجموع, sqsum, len, cn), k = cn % 4;
        src = src0 + x * cn;

        اگر( k == 1 )
        {
            ST s0 = مجموع[0];
            SQT sq0 = sqsum[0];
            برائے(صحیح i = x; i < len; i++, src += cn )
            {
                ST v = (ST)src[0];
                s0 += v; sq0 += (SQT)v*v;
            }
            مجموع[0] = s0;
            sqsum[0] = sq0;
        }
        ورنہ اگر( k == 2 )
        {
            ST s0 = مجموع[0], s1 = مجموع[1];
            SQT sq0 = sqsum[0], sq1 = sqsum[1];
            برائے(صحیح i = x; i < len; i++, src += cn )
            {
                ST v0 = (ST)src[0], v1 = (ST)src[1];
                s0 += v0; sq0 += (SQT)v0*v0;
                s1 += v1; sq1 += (SQT)v1*v1;
            }
            مجموع[0] = s0; مجموع[1] = s1;
            sqsum[0] = sq0; sqsum[1] = sq1;
        }
        ورنہ اگر( k == 3 )
        {
            ST s0 = مجموع[0], s1 = مجموع[1], s2 = مجموع[2];
            SQT sq0 = sqsum[0], sq1 = sqsum[1], sq2 = sqsum[2];
            برائے(صحیح i = x; i < len; i++, src += cn )
            {
                ST v0 = (ST)src[0], v1 = (ST)src[1], v2 = (ST)src[2];
                s0 += v0; sq0 += (SQT)v0*v0;
                s1 += v1; sq1 += (SQT)v1*v1;
                s2 += v2; sq2 += (SQT)v2*v2;
            }
            مجموع[0] = s0; مجموع[1] = s1; مجموع[2] = s2;
            sqsum[0] = sq0; sqsum[1] = sq1; sqsum[2] = sq2;
        }

        برائے( ; k < cn; k += 4 )
        {
            src = src0 + x * cn + k;
            ST s0 = مجموع[k], s1 = مجموع[k+1], s2 = مجموع[k+2], s3 = مجموع[k+3];
            SQT sq0 = sqsum[k], sq1 = sqsum[k+1], sq2 = sqsum[k+2], sq3 = sqsum[k+3];
            برائے(صحیح i = x; i < len; i++, src += cn )
            {
                ST v0, v1;
                v0 = (ST)src[0], v1 = (ST)src[1];
                s0 += v0; sq0 += (SQT)v0*v0;
                s1 += v1; sq1 += (SQT)v1*v1;
                ST v2, v3;
                v2 = (ST)src[2], v3 = (ST)src[3];
                s2 += v2; sq2 += (SQT)v2*v2;
                s3 += v3; sq3 += (SQT)v3*v3;
            }
            مجموع[k] = s0; مجموع[k+1] = s1;
            مجموع[k+2] = s2; مجموع[k+3] = s3;
            sqsum[k] = sq0; sqsum[k+1] = sq1;
            sqsum[k+2] = sq2; sqsum[k+3] = sq3;
        }
        واپس len;
    }

    صحیح i, nzm = 0;

    اگر( cn == 1 )
    {
        ST s0 = مجموع[0];
        SQT sq0 = sqsum[0];
        برائے( i = 0; i < len; i++ )
            اگر( نقاب[i] )
            {
                ST v = (ST)src[i];
                s0 += v; sq0 += (SQT)v*v;
                nzm++;
            }
        مجموع[0] = s0;
        sqsum[0] = sq0;
    }
    ورنہ اگر( cn == 2 )
    {
        ST s0 = مجموع[0], s1 = مجموع[1];
        SQT sq0 = sqsum[0], sq1 = sqsum[1];
        برائے( i = 0; i < len; i++, src += 2 )
            اگر( نقاب[i] )
            {
                T v0 = src[0], v1 = src[1];
                s0 += v0; sq0 += (SQT)v0*v0;
                s1 += v1; sq1 += (SQT)v1*v1;
                nzm++;
            }
        مجموع[0] = s0; مجموع[1] = s1;
        sqsum[0] = sq0; sqsum[1] = sq1;
    }
    ورنہ اگر( cn == 3 )
    {
        ST s0 = مجموع[0], s1 = مجموع[1], s2 = مجموع[2];
        SQT sq0 = sqsum[0], sq1 = sqsum[1], sq2 = sqsum[2];
        برائے( i = 0; i < len; i++, src += 3 )
            اگر( نقاب[i] )
            {
                ST v0 = (ST)src[0], v1 = (ST)src[1], v2 = (ST)src[2];
                s0 += v0; sq0 += (SQT)v0*v0;
                s1 += v1; sq1 += (SQT)v1*v1;
                s2 += v2; sq2 += (SQT)v2*v2;
                nzm++;
            }
        مجموع[0] = s0; مجموع[1] = s1; مجموع[2] = s2;
        sqsum[0] = sq0; sqsum[1] = sq1; sqsum[2] = sq2;
    }
    ورنہ اگر( cn == 4 )
    {
        ST s0 = مجموع[0], s1 = مجموع[1], s2 = مجموع[2], s3 = مجموع[3];
        SQT sq0 = sqsum[0], sq1 = sqsum[1], sq2 = sqsum[2], sq3 = sqsum[3];
        برائے( i = 0; i < len; i++, src += 4 )
            اگر( نقاب[i] )
            {
                T v0 = src[0], v1 = src[1], v2 = src[2], v3 = src[3];
                s0 += v0; sq0 += (SQT)v0*v0;
                s1 += v1; sq1 += (SQT)v1*v1;
                s2 += v2; sq2 += (SQT)v2*v2;
                s3 += v3; sq3 += (SQT)v3*v3;
                nzm++;
            }
        مجموع[0] = s0; مجموع[1] = s1; مجموع[2] = s2; مجموع[3] = s3;
        sqsum[0] = sq0; sqsum[1] = sq1; sqsum[2] = sq2; sqsum[3] = sq3;
    }
    ورنہ
    {
        برائے( i = 0; i < len; i++, src += cn )
            اگر( نقاب[i] )
            {
                برائے( صحیح k = 0; k < cn; k++ )
                {
                    ST v = (ST)src[k];
                    ST s = مجموع[k] + v;
                    SQT sq = sqsum[k] + (SQT)v*v;
                    مجموع[k] = s; sqsum[k] = sq;
                }
                nzm++;
            }
    }
    واپس nzm;
}


ساکن صحیح sqsum8u( ثابت uchar* src, ثابت uchar* نقاب, صحیح* مجموع, صحیح* sqsum, صحیح len, صحیح cn )
{ CV_INSTRUMENT_REGION(); واپس sumsqr_(src, نقاب, مجموع, sqsum, len, cn); }

ساکن صحیح sqsum8s( ثابت schar* src, ثابت uchar* نقاب, صحیح* مجموع, صحیح* sqsum, صحیح len, صحیح cn )
{ CV_INSTRUMENT_REGION(); واپس sumsqr_(src, نقاب, مجموع, sqsum, len, cn); }

ساکن صحیح sqsum16u( ثابت ushort* src, ثابت uchar* نقاب, صحیح* مجموع, دوہرا* sqsum, صحیح len, صحیح cn )
{ CV_INSTRUMENT_REGION(); واپس sumsqr_(src, نقاب, مجموع, sqsum, len, cn); }

ساکن صحیح sqsum16s( ثابت مختصر* src, ثابت uchar* نقاب, صحیح* مجموع, دوہرا* sqsum, صحیح len, صحیح cn )
{ CV_INSTRUMENT_REGION(); واپس sumsqr_(src, نقاب, مجموع, sqsum, len, cn); }

ساکن صحیح sqsum32s( ثابت صحیح* src, ثابت uchar* نقاب, دوہرا* مجموع, دوہرا* sqsum, صحیح len, صحیح cn )
{ CV_INSTRUMENT_REGION(); واپس sumsqr_(src, نقاب, مجموع, sqsum, len, cn); }

ساکن صحیح sqsum32f( ثابت اعشاری* src, ثابت uchar* نقاب, دوہرا* مجموع, دوہرا* sqsum, صحیح len, صحیح cn )
{ CV_INSTRUMENT_REGION(); واپس sumsqr_(src, نقاب, مجموع, sqsum, len, cn); }

ساکن صحیح sqsum64f( ثابت دوہرا* src, ثابت uchar* نقاب, دوہرا* مجموع, دوہرا* sqsum, صحیح len, صحیح cn )
{ CV_INSTRUMENT_REGION(); واپس sumsqr_(src, نقاب, مجموع, sqsum, len, cn); }

ساکن صحیح sqsum16f( ثابت hfloat* src, ثابت uchar* نقاب, اعشاری* مجموع, دوہرا* sqsum, صحیح len, صحیح cn )
{ CV_INSTRUMENT_REGION(); واپس sumsqr_(src, نقاب, مجموع, sqsum, len, cn); }

ساکن صحیح sqsum16bf( ثابت bfloat* src, ثابت uchar* نقاب, اعشاری* مجموع, دوہرا* sqsum, صحیح len, صحیح cn )
{ CV_INSTRUMENT_REGION(); واپس sumsqr_(src, نقاب, مجموع, sqsum, len, cn); }

ساکن صحیح sqsum64u( ثابت uint64* src, ثابت uchar* نقاب, دوہرا* مجموع, دوہرا* sqsum, صحیح len, صحیح cn )
{ CV_INSTRUMENT_REGION(); واپس sumsqr_(src, نقاب, مجموع, sqsum, len, cn); }

ساکن صحیح sqsum64s( ثابت int64* src, ثابت uchar* نقاب, دوہرا* مجموع, دوہرا* sqsum, صحیح len, صحیح cn )
{ CV_INSTRUMENT_REGION(); واپس sumsqr_(src, نقاب, مجموع, sqsum, len, cn); }

ساکن صحیح sqsum32u( ثابت غیرعلامتی* src, ثابت uchar* نقاب, دوہرا* مجموع, دوہرا* sqsum, صحیح len, صحیح cn )
{ CV_INSTRUMENT_REGION(); واپس sumsqr_(src, نقاب, مجموع, sqsum, len, cn); }

مجموع_Sqr_Func get_مجموع_Sqr_Func(صحیح گہرائی)
{
    CV_INSTRUMENT_REGION();
    ساکن مجموع_Sqr_Func مجموع_Sqr_Tab[CV_گہرائی_MAX] =
    {
        مجموع_Sqr_Wrap<uchar, صحیح, صحیح, sqsum8u>, مجموع_Sqr_Wrap<schar, صحیح, صحیح, sqsum8s>,
        مجموع_Sqr_Wrap<ushort, صحیح, دوہرا, sqsum16u>, مجموع_Sqr_Wrap<مختصر, صحیح, دوہرا, sqsum16s>,
        مجموع_Sqr_Wrap<صحیح, دوہرا, دوہرا, sqsum32s>, مجموع_Sqr_Wrap<اعشاری, دوہرا, دوہرا, sqsum32f>,
        مجموع_Sqr_Wrap<دوہرا, دوہرا, دوہرا, sqsum64f>,
        مجموع_Sqr_Wrap<hfloat, اعشاری, دوہرا, sqsum16f>, مجموع_Sqr_Wrap<bfloat, اعشاری, دوہرا, sqsum16bf>, 0,
        مجموع_Sqr_Wrap<uint64, دوہرا, دوہرا, sqsum64u>, مجموع_Sqr_Wrap<int64, دوہرا, دوہرا, sqsum64s>,
        مجموع_Sqr_Wrap<غیرعلامتی, دوہرا, دوہرا, sqsum32u>, 0
    };

    واپس مجموع_Sqr_Tab[گہرائی];
}

#اگر_ختم
CV_CPU_OPTIMIZATION_NAMESPACE_انتہا
} // namespace
