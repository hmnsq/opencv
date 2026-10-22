// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html

// Copyright (C) 2026, Advanced Micro Devices, Inc., all rights reserved.

#ضمن "precomp.hpp"
#ضمن "stat.hpp"

نطاق cv {
تعريفنوع صحيح (*جوڙ_Sqr_Func)(ثابت uchar*, ثابت uchar* نقاب, uchar*, uchar*, صحيح, صحيح);

CV_CPU_OPTIMIZATION_NAMESPACE_BEGIN

سانچو<قسم_نالو T, قسم_نالو ST, قسم_نالو SQT, صحيح (*fn)(ثابت T*, ثابت uchar*, ST*, SQT*, صحيح, صحيح)>
ساكن صحيح جوڙ_Sqr_Wrap(ثابت uchar* src, ثابت uchar* نقاب, uchar* جوڙ, uchar* sqsum, صحيح len, صحيح cn)
{
    أرجع fn((ثابت T*)src, نقاب, (ST*)جوڙ, (SQT*)sqsum, len, cn);
}

جوڙ_Sqr_Func get_جوڙ_Sqr_Func(صحيح اونهائي);

#جيڪڏهن_وصف_نه_ٿيل CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

سانچو <قسم_نالو T, قسم_نالو ST, قسم_نالو SQT>
بنية جوڙ_Sqr_SIMD
{
    مضمن صحيح عامل () (ثابت T *, ثابت uchar *, ST *, SQT *, صحيح, صحيح) ثابت
    {
        أرجع 0;
    }
};

#إذا (CV_SIMD || CV_SIMD_SCALABLE)

سانچو <>
بنية جوڙ_Sqr_SIMD<uchar, صحيح, صحيح>
{
    صحيح عامل () (ثابت uchar * src0, ثابت uchar * نقاب, صحيح * جوڙ, صحيح * sqsum, صحيح len, صحيح cn) ثابت
    {
        إذا (نقاب)
            أرجع 0;
        // cn==3: deinterleave so each lane belongs to one channel, then reduce per
        // channel. sum/sqsum stay exact in s32 within the dispatcher's 1<<15 block.
        إذا (cn == 3)
        {
            ثابت صحيح vl8 = VTraits<v_uint8>::vlanes();
            v_int32 vs0 = vx_setzero_s32(), vs1 = vx_setzero_s32(), vs2 = vx_setzero_s32();
            v_int32 vq0 = vx_setzero_s32(), vq1 = vx_setzero_s32(), vq2 = vx_setzero_s32();
            خودڪار acc = [](ثابت v_uint8& ch, v_int32& vs, v_int32& vq)
            {
                v_uint16 lo, hi; v_expand(ch, lo, hi);
                v_uint32 s0, s1, s2, s3; v_expand(lo, s0, s1); v_expand(hi, s2, s3);
                vs = v_جوڙيو(vs, v_reinterpret_as_s32(v_جوڙيو(v_جوڙيو(s0, s1), v_جوڙيو(s2, s3))));
                v_int16 d0 = v_reinterpret_as_s16(lo), d1 = v_reinterpret_as_s16(hi);
                vq = v_جوڙيو(vq, v_جوڙيو(v_dotprod(d0, d0), v_dotprod(d1, d1)));
            };
            صحيح e = 0;
            لكل (; e <= len - vl8; e += vl8)
            {
                v_uint8 a, b, c; v_ڀريو_deinterleave(src0 + e * 3, a, b, c);
                acc(a, vs0, vq0); acc(b, vs1, vq1); acc(c, vs2, vq2);
            }
            جوڙ[0] += v_reduce_جوڙ(vs0); جوڙ[1] += v_reduce_جوڙ(vs1); جوڙ[2] += v_reduce_جوڙ(vs2);
            sqsum[0] += v_reduce_جوڙ(vq0); sqsum[1] += v_reduce_جوڙ(vq1); sqsum[2] += v_reduce_جوڙ(vq2);
            v_cleanup();
            أرجع e;
        }
        إذا (cn != 1 && cn != 2 && cn != 4)
            أرجع 0;
        len *= cn;

        صحيح x = 0;
        v_int32 v_جوڙ = vx_setzero_s32();
        v_int32 v_sqsum = vx_setzero_s32();

        ثابت صحيح len0 = len & -VTraits<v_uint8>::vlanes();
        طالما(x < len0)
        {
            ثابت صحيح len_tmp = min(x + 256*VTraits<v_uint16>::vlanes(), len0);
            v_uint16 v_sum16 = vx_setzero_u16();
            لكل ( ; x < len_tmp; x += VTraits<v_uint8>::vlanes())
            {
                v_uint16 v_src0 = vx_ڀريو_expand(src0 + x);
                v_uint16 v_src1 = vx_ڀريو_expand(src0 + x + VTraits<v_uint16>::vlanes());
                v_sum16 = v_جوڙيو(v_sum16, v_جوڙيو(v_src0, v_src1));
                v_int16 v_tmp0, v_tmp1;
                v_zip(v_reinterpret_as_s16(v_src0), v_reinterpret_as_s16(v_src1), v_tmp0, v_tmp1);
                v_sqsum = v_جوڙيو(v_sqsum, v_جوڙيو(v_dotprod(v_tmp0, v_tmp0), v_dotprod(v_tmp1, v_tmp1)));
            }
            v_uint32 v_half0, v_half1;
            v_expand(v_sum16, v_half0, v_half1);
            v_جوڙ = v_جوڙيو(v_جوڙ, v_reinterpret_as_s32(v_جوڙيو(v_half0, v_half1)));
        }
        إذا (x <= len - VTraits<v_uint16>::vlanes())
        {
            v_uint16 v_src = vx_ڀريو_expand(src0 + x);
            v_uint16 v_half = v_combine_مٿاهون(v_src, v_src);

            v_uint32 v_tmp0, v_tmp1;
            v_expand(v_جوڙيو(v_src, v_half), v_tmp0, v_tmp1);
            v_جوڙ = v_جوڙيو(v_جوڙ, v_reinterpret_as_s32(v_tmp0));

            v_int16 v_tmp2, v_tmp3;
            v_zip(v_reinterpret_as_s16(v_src), v_reinterpret_as_s16(v_half), v_tmp2, v_tmp3);
            v_sqsum = v_جوڙيو(v_sqsum, v_dotprod(v_tmp2, v_tmp2));
            x += VTraits<v_uint16>::vlanes();
        }

        إذا (cn == 1)
        {
            *جوڙ += v_reduce_جوڙ(v_جوڙ);
            *sqsum += v_reduce_جوڙ(v_sqsum);
        }
        وإلا
        {
            صحيح CV_DECL_ALIGNED(CV_SIMD_ويڪر) ar[2 * VTraits<v_int32>::max_nlanes];
            v_store(ar, v_جوڙ);
            v_store(ar + VTraits<v_int32>::vlanes(), v_sqsum);
            لكل (صحيح i = 0; i < VTraits<v_int32>::vlanes(); ++i)
            {
                جوڙ[i % cn] += ar[i];
                sqsum[i % cn] += ar[VTraits<v_int32>::vlanes() + i];
            }
        }
        v_cleanup();
        أرجع x / cn;
    }
};

سانچو <>
بنية جوڙ_Sqr_SIMD<schar, صحيح, صحيح>
{
    صحيح عامل () (ثابت schar * src0, ثابت uchar * نقاب, صحيح * جوڙ, صحيح * sqsum, صحيح len, صحيح cn) ثابت
    {
        إذا (نقاب)
            أرجع 0;
        إذا (cn == 3)
        {
            ثابت صحيح vl8 = VTraits<v_int8>::vlanes();
            v_int32 vs0 = vx_setzero_s32(), vs1 = vx_setzero_s32(), vs2 = vx_setzero_s32();
            v_int32 vq0 = vx_setzero_s32(), vq1 = vx_setzero_s32(), vq2 = vx_setzero_s32();
            خودڪار acc = [](ثابت v_int8& ch, v_int32& vs, v_int32& vq)
            {
                v_int16 lo, hi; v_expand(ch, lo, hi);
                v_int32 s0, s1, s2, s3; v_expand(lo, s0, s1); v_expand(hi, s2, s3);
                vs = v_جوڙيو(vs, v_جوڙيو(v_جوڙيو(s0, s1), v_جوڙيو(s2, s3)));
                vq = v_جوڙيو(vq, v_جوڙيو(v_dotprod(lo, lo), v_dotprod(hi, hi)));
            };
            صحيح e = 0;
            لكل (; e <= len - vl8; e += vl8)
            {
                v_int8 a, b, c; v_ڀريو_deinterleave(src0 + e * 3, a, b, c);
                acc(a, vs0, vq0); acc(b, vs1, vq1); acc(c, vs2, vq2);
            }
            جوڙ[0] += v_reduce_جوڙ(vs0); جوڙ[1] += v_reduce_جوڙ(vs1); جوڙ[2] += v_reduce_جوڙ(vs2);
            sqsum[0] += v_reduce_جوڙ(vq0); sqsum[1] += v_reduce_جوڙ(vq1); sqsum[2] += v_reduce_جوڙ(vq2);
            v_cleanup();
            أرجع e;
        }
        إذا (cn != 1 && cn != 2 && cn != 4)
            أرجع 0;
        len *= cn;

        صحيح x = 0;
        v_int32 v_جوڙ = vx_setzero_s32();
        v_int32 v_sqsum = vx_setzero_s32();

        ثابت صحيح len0 = len & -VTraits<v_int8>::vlanes();
        طالما (x < len0)
        {
            ثابت صحيح len_tmp = min(x + 256 * VTraits<v_int16>::vlanes(), len0);
            v_int16 v_sum16 = vx_setzero_s16();
            لكل (; x < len_tmp; x += VTraits<v_int8>::vlanes())
            {
                v_int16 v_src0 = vx_ڀريو_expand(src0 + x);
                v_int16 v_src1 = vx_ڀريو_expand(src0 + x + VTraits<v_int16>::vlanes());
                v_sum16 = v_جوڙيو(v_sum16, v_جوڙيو(v_src0, v_src1));
                v_int16 v_tmp0, v_tmp1;
                v_zip(v_src0, v_src1, v_tmp0, v_tmp1);
                v_sqsum = v_جوڙيو(v_sqsum, v_جوڙيو(v_dotprod(v_tmp0, v_tmp0), v_dotprod(v_tmp1, v_tmp1)));
            }
            v_int32 v_half0, v_half1;
            v_expand(v_sum16, v_half0, v_half1);
            v_جوڙ = v_جوڙيو(v_جوڙ, v_جوڙيو(v_half0, v_half1));
        }
        إذا (x <= len - VTraits<v_int16>::vlanes())
        {
            v_int16 v_src = vx_ڀريو_expand(src0 + x);
            v_int16 v_half = v_combine_مٿاهون(v_src, v_src);

            v_int32 v_tmp0, v_tmp1;
            v_expand(v_جوڙيو(v_src, v_half), v_tmp0, v_tmp1);
            v_جوڙ = v_جوڙيو(v_جوڙ, v_tmp0);

            v_int16 v_tmp2, v_tmp3;
            v_zip(v_src, v_half, v_tmp2, v_tmp3);
            v_sqsum = v_جوڙيو(v_sqsum, v_dotprod(v_tmp2, v_tmp2));
            x += VTraits<v_int16>::vlanes();
        }

        إذا (cn == 1)
        {
            *جوڙ += v_reduce_جوڙ(v_جوڙ);
            *sqsum += v_reduce_جوڙ(v_sqsum);
        }
        وإلا
        {
            صحيح CV_DECL_ALIGNED(CV_SIMD_ويڪر) ar[2 * VTraits<v_int32>::max_nlanes];
            v_store(ar, v_جوڙ);
            v_store(ar + VTraits<v_int32>::vlanes(), v_sqsum);
            لكل (صحيح i = 0; i < VTraits<v_int32>::vlanes(); ++i)
            {
                جوڙ[i % cn] += ar[i];
                sqsum[i % cn] += ar[VTraits<v_int32>::vlanes() + i];
            }
        }
        v_cleanup();
        أرجع x / cn;
    }
};

#إذا (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)

// Expand a 16-bit vector into two s32 halves. The unsigned overload reinterprets
// after a widening expand (values 0..65535 stay non-negative in s32, so the
// subsequent v_cvt_f64 is exact); the signed overload expands directly. This is
// the only real difference between the ushort and short reductions below.
ساكن مضمن فراغ جوڙ_Sqr_Expand_S32(ثابت v_uint16& v, v_int32& lo, v_int32& hi)
{
    v_uint32 a, b; v_expand(v, a, b);
    lo = v_reinterpret_as_s32(a); hi = v_reinterpret_as_s32(b);
}
ساكن مضمن فراغ جوڙ_Sqr_Expand_S32(ثابت v_int16& v, v_int32& lo, v_int32& hi)
{
    v_expand(v, lo, hi);
}

// Shared 16-bit -> {s32 sum, f64 sqsum} reduction for ushort/short. The register
// type is deduced from the pointer, so the body is identical for both depths.
سانچو <قسم_نالو T>
ساكن مضمن صحيح جوڙ_Sqr16_منزل_F64(ثابت T* src0, صحيح* جوڙ, مزدوج* sqsum, صحيح len, صحيح cn)
{
    تعريفنوع decltype(vx_ڀريو(src0)) VT; // v_uint16 or v_int16

    إذا (cn == 3)
    {
        ثابت صحيح vl = VTraits<VT>::vlanes();
        v_int32 vs0 = vx_setzero_s32(), vs1 = vx_setzero_s32(), vs2 = vx_setzero_s32();
        v_float64 vq0 = vx_setzero_f64(), vq1 = vx_setzero_f64(), vq2 = vx_setzero_f64();
        خودڪار acc = [](ثابت VT& ch, v_int32& vs, v_float64& vq)
        {
            v_int32 lo, hi; جوڙ_Sqr_Expand_S32(ch, lo, hi);
            vs = v_جوڙيو(vs, v_جوڙيو(lo, hi));
            v_float64 f0 = v_cvt_f64(lo), f1 = v_cvt_f64_مٿاهون(lo);
            v_float64 f2 = v_cvt_f64(hi), f3 = v_cvt_f64_مٿاهون(hi);
            vq = v_fma(f0, f0, v_fma(f1, f1, v_fma(f2, f2, v_fma(f3, f3, vq))));
        };
        صحيح e = 0;
        لكل (; e <= len - vl; e += vl)
        {
            VT a, b, c; v_ڀريو_deinterleave(src0 + e * 3, a, b, c);
            acc(a, vs0, vq0); acc(b, vs1, vq1); acc(c, vs2, vq2);
        }
        جوڙ[0] += v_reduce_جوڙ(vs0); جوڙ[1] += v_reduce_جوڙ(vs1); جوڙ[2] += v_reduce_جوڙ(vs2);
        sqsum[0] += v_reduce_جوڙ(vq0); sqsum[1] += v_reduce_جوڙ(vq1); sqsum[2] += v_reduce_جوڙ(vq2);
        v_cleanup();
        أرجع e;
    }
    إذا (cn != 1 && cn != 2 && cn != 4)
        أرجع 0;
    len *= cn;

    ثابت صحيح vl16 = VTraits<VT>::vlanes();
    ثابت صحيح vl32 = VTraits<v_int32>::vlanes();
    ثابت صحيح vl64 = VTraits<v_float64>::vlanes();

    // The lane->channel scatter below (i % cn) and the returned pixel count
    // (x / cn) require the lane counts to be a multiple of cn. This always holds
    // for fixed-width SIMD (vlanes is a power of two >= cn for cn in {1,2,4}),
    // but scalable backends (e.g. RVV) may report a non-multiple; fall back to
    // scalar there. vl16 == 2*vl32, so checking vl32 covers both scatter loops.
    إذا (vl32 % cn != 0)
        أرجع 0;

    v_int32 v_جوڙ = vx_setzero_s32();
    v_float64 v_sq0 = vx_setzero_f64(), v_sq1 = vx_setzero_f64();
    v_float64 v_sq2 = vx_setzero_f64(), v_sq3 = vx_setzero_f64();

    صحيح x = 0;
    لكل (; x <= len - vl16; x += vl16)
    {
        VT v_src = vx_ڀريو(src0 + x);
        v_int32 lo, hi; جوڙ_Sqr_Expand_S32(v_src, lo, hi);
        v_جوڙ = v_جوڙيو(v_جوڙ, v_جوڙيو(lo, hi));

        v_float64 f0 = v_cvt_f64(lo);
        v_float64 f1 = v_cvt_f64_مٿاهون(lo);
        v_float64 f2 = v_cvt_f64(hi);
        v_float64 f3 = v_cvt_f64_مٿاهون(hi);
        v_sq0 = v_fma(f0, f0, v_sq0);
        v_sq1 = v_fma(f1, f1, v_sq1);
        v_sq2 = v_fma(f2, f2, v_sq2);
        v_sq3 = v_fma(f3, f3, v_sq3);
    }

    صحيح    CV_DECL_ALIGNED(CV_SIMD_ويڪر) sbuf[VTraits<v_int32>::max_nlanes];
    مزدوج CV_DECL_ALIGNED(CV_SIMD_ويڪر) qbuf[VTraits<v_float64>::max_nlanes * 4];
    v_store_aligned(sbuf, v_جوڙ);
    v_store_aligned(qbuf + vl64 * 0, v_sq0);
    v_store_aligned(qbuf + vl64 * 1, v_sq1);
    v_store_aligned(qbuf + vl64 * 2, v_sq2);
    v_store_aligned(qbuf + vl64 * 3, v_sq3);

    // sbuf lane j (j<vl32) holds samples j and j+vl32 -> channel j%cn (vl32%cn==0).
    // qbuf index i (i<vl16) holds sample lane i           -> channel i%cn (vl16%cn==0).
    لكل (صحيح i = 0; i < vl32; ++i)
        جوڙ[i % cn] += sbuf[i];
    لكل (صحيح i = 0; i < vl16; ++i)
        sqsum[i % cn] += qbuf[i];

    v_cleanup();
    أرجع x / cn;
}

// Shared 32-bit -> f64 reduction for int/float. Both sum and sqsum accumulate in
// f64 to match the scalar reference's double accumulation; values are widened to
// f64 before squaring so the square is computed in double (same as (double)v*v).
// The register type is deduced from the pointer, so int and float share one body.
سانچو <قسم_نالو T>
ساكن مضمن صحيح جوڙ_Sqr32_منزل_F64(ثابت T* src0, مزدوج* جوڙ, مزدوج* sqsum, صحيح len, صحيح cn)
{
    تعريفنوع decltype(vx_ڀريو(src0)) VT; // v_int32 or v_float32

    إذا (cn == 3)
    {
        ثابت صحيح vl = VTraits<VT>::vlanes();
        v_float64 vs0 = vx_setzero_f64(), vs1 = vx_setzero_f64(), vs2 = vx_setzero_f64();
        v_float64 vq0 = vx_setzero_f64(), vq1 = vx_setzero_f64(), vq2 = vx_setzero_f64();
        خودڪار acc = [](ثابت VT& ch, v_float64& vs, v_float64& vq)
        {
            v_float64 f0 = v_cvt_f64(ch), f1 = v_cvt_f64_مٿاهون(ch);
            vs = v_جوڙيو(vs, v_جوڙيو(f0, f1));
            vq = v_fma(f0, f0, v_fma(f1, f1, vq));
        };
        صحيح e = 0;
        لكل (; e <= len - vl; e += vl)
        {
            VT a, b, c; v_ڀريو_deinterleave(src0 + e * 3, a, b, c);
            acc(a, vs0, vq0); acc(b, vs1, vq1); acc(c, vs2, vq2);
        }
        جوڙ[0] += v_reduce_جوڙ(vs0); جوڙ[1] += v_reduce_جوڙ(vs1); جوڙ[2] += v_reduce_جوڙ(vs2);
        sqsum[0] += v_reduce_جوڙ(vq0); sqsum[1] += v_reduce_جوڙ(vq1); sqsum[2] += v_reduce_جوڙ(vq2);
        v_cleanup();
        أرجع e;
    }
    إذا (cn != 1 && cn != 2 && cn != 4)
        أرجع 0;
    len *= cn;

    ثابت صحيح vl32 = VTraits<VT>::vlanes();
    ثابت صحيح vl64 = VTraits<v_float64>::vlanes();

    // See note in sumSqr16ToF64: guard scalable backends whose lane count may
    // not divide cn (the i % cn scatter / x / cn rely on it).
    إذا (vl32 % cn != 0)
        أرجع 0;

    v_float64 vs0 = vx_setzero_f64(), vs1 = vx_setzero_f64();
    v_float64 vq0 = vx_setzero_f64(), vq1 = vx_setzero_f64();

    صحيح x = 0;
    لكل (; x <= len - vl32; x += vl32)
    {
        VT v_src = vx_ڀريو(src0 + x);
        v_float64 f0 = v_cvt_f64(v_src);
        v_float64 f1 = v_cvt_f64_مٿاهون(v_src);
        vs0 = v_جوڙيو(vs0, f0);
        vs1 = v_جوڙيو(vs1, f1);
        vq0 = v_fma(f0, f0, vq0);
        vq1 = v_fma(f1, f1, vq1);
    }

    مزدوج CV_DECL_ALIGNED(CV_SIMD_ويڪر) sbuf[VTraits<v_float64>::max_nlanes * 2];
    مزدوج CV_DECL_ALIGNED(CV_SIMD_ويڪر) qbuf[VTraits<v_float64>::max_nlanes * 2];
    v_store_aligned(sbuf, vs0);          v_store_aligned(sbuf + vl64, vs1);
    v_store_aligned(qbuf, vq0);          v_store_aligned(qbuf + vl64, vq1);

    لكل (صحيح i = 0; i < vl32; ++i)
    {
        جوڙ[i % cn]   += sbuf[i];
        sqsum[i % cn] += qbuf[i];
    }

    v_cleanup();
    أرجع x / cn;
}

سانچو <>
بنية جوڙ_Sqr_SIMD<ushort, صحيح, مزدوج>
{
    صحيح عامل () (ثابت ushort* src0, ثابت uchar* نقاب, صحيح* جوڙ, مزدوج* sqsum, صحيح len, صحيح cn) ثابت
    { أرجع نقاب ? 0 : جوڙ_Sqr16_منزل_F64(src0, جوڙ, sqsum, len, cn); }
};

سانچو <>
بنية جوڙ_Sqr_SIMD<ننڍو, صحيح, مزدوج>
{
    صحيح عامل () (ثابت ننڍو* src0, ثابت uchar* نقاب, صحيح* جوڙ, مزدوج* sqsum, صحيح len, صحيح cn) ثابت
    { أرجع نقاب ? 0 : جوڙ_Sqr16_منزل_F64(src0, جوڙ, sqsum, len, cn); }
};

سانچو <>
بنية جوڙ_Sqr_SIMD<صحيح, مزدوج, مزدوج>
{
    صحيح عامل () (ثابت صحيح* src0, ثابت uchar* نقاب, مزدوج* جوڙ, مزدوج* sqsum, صحيح len, صحيح cn) ثابت
    { أرجع نقاب ? 0 : جوڙ_Sqr32_منزل_F64(src0, جوڙ, sqsum, len, cn); }
};

سانچو <>
بنية جوڙ_Sqr_SIMD<عائم, مزدوج, مزدوج>
{
    صحيح عامل () (ثابت عائم* src0, ثابت uchar* نقاب, مزدوج* جوڙ, مزدوج* sqsum, صحيح len, صحيح cn) ثابت
    { أرجع نقاب ? 0 : جوڙ_Sqr32_منزل_F64(src0, جوڙ, sqsum, len, cn); }
};

#جيڪڏهن_پورو // CV_SIMD_64F

#جيڪڏهن_پورو

سانچو<قسم_نالو T, قسم_نالو ST, قسم_نالو SQT>
ساكن صحيح sumsqr_(ثابت T* src0, ثابت uchar* نقاب, ST* جوڙ, SQT* sqsum, صحيح len, صحيح cn )
{
    ثابت T* src = src0;

    إذا( !نقاب )
    {
        جوڙ_Sqr_SIMD<T, ST, SQT> vop;
        صحيح x = vop(src0, نقاب, جوڙ, sqsum, len, cn), k = cn % 4;
        src = src0 + x * cn;

        إذا( k == 1 )
        {
            ST s0 = جوڙ[0];
            SQT sq0 = sqsum[0];
            لكل(صحيح i = x; i < len; i++, src += cn )
            {
                ST v = (ST)src[0];
                s0 += v; sq0 += (SQT)v*v;
            }
            جوڙ[0] = s0;
            sqsum[0] = sq0;
        }
        وإلا إذا( k == 2 )
        {
            ST s0 = جوڙ[0], s1 = جوڙ[1];
            SQT sq0 = sqsum[0], sq1 = sqsum[1];
            لكل(صحيح i = x; i < len; i++, src += cn )
            {
                ST v0 = (ST)src[0], v1 = (ST)src[1];
                s0 += v0; sq0 += (SQT)v0*v0;
                s1 += v1; sq1 += (SQT)v1*v1;
            }
            جوڙ[0] = s0; جوڙ[1] = s1;
            sqsum[0] = sq0; sqsum[1] = sq1;
        }
        وإلا إذا( k == 3 )
        {
            ST s0 = جوڙ[0], s1 = جوڙ[1], s2 = جوڙ[2];
            SQT sq0 = sqsum[0], sq1 = sqsum[1], sq2 = sqsum[2];
            لكل(صحيح i = x; i < len; i++, src += cn )
            {
                ST v0 = (ST)src[0], v1 = (ST)src[1], v2 = (ST)src[2];
                s0 += v0; sq0 += (SQT)v0*v0;
                s1 += v1; sq1 += (SQT)v1*v1;
                s2 += v2; sq2 += (SQT)v2*v2;
            }
            جوڙ[0] = s0; جوڙ[1] = s1; جوڙ[2] = s2;
            sqsum[0] = sq0; sqsum[1] = sq1; sqsum[2] = sq2;
        }

        لكل( ; k < cn; k += 4 )
        {
            src = src0 + x * cn + k;
            ST s0 = جوڙ[k], s1 = جوڙ[k+1], s2 = جوڙ[k+2], s3 = جوڙ[k+3];
            SQT sq0 = sqsum[k], sq1 = sqsum[k+1], sq2 = sqsum[k+2], sq3 = sqsum[k+3];
            لكل(صحيح i = x; i < len; i++, src += cn )
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
            جوڙ[k] = s0; جوڙ[k+1] = s1;
            جوڙ[k+2] = s2; جوڙ[k+3] = s3;
            sqsum[k] = sq0; sqsum[k+1] = sq1;
            sqsum[k+2] = sq2; sqsum[k+3] = sq3;
        }
        أرجع len;
    }

    صحيح i, nzm = 0;

    إذا( cn == 1 )
    {
        ST s0 = جوڙ[0];
        SQT sq0 = sqsum[0];
        لكل( i = 0; i < len; i++ )
            إذا( نقاب[i] )
            {
                ST v = (ST)src[i];
                s0 += v; sq0 += (SQT)v*v;
                nzm++;
            }
        جوڙ[0] = s0;
        sqsum[0] = sq0;
    }
    وإلا إذا( cn == 2 )
    {
        ST s0 = جوڙ[0], s1 = جوڙ[1];
        SQT sq0 = sqsum[0], sq1 = sqsum[1];
        لكل( i = 0; i < len; i++, src += 2 )
            إذا( نقاب[i] )
            {
                T v0 = src[0], v1 = src[1];
                s0 += v0; sq0 += (SQT)v0*v0;
                s1 += v1; sq1 += (SQT)v1*v1;
                nzm++;
            }
        جوڙ[0] = s0; جوڙ[1] = s1;
        sqsum[0] = sq0; sqsum[1] = sq1;
    }
    وإلا إذا( cn == 3 )
    {
        ST s0 = جوڙ[0], s1 = جوڙ[1], s2 = جوڙ[2];
        SQT sq0 = sqsum[0], sq1 = sqsum[1], sq2 = sqsum[2];
        لكل( i = 0; i < len; i++, src += 3 )
            إذا( نقاب[i] )
            {
                ST v0 = (ST)src[0], v1 = (ST)src[1], v2 = (ST)src[2];
                s0 += v0; sq0 += (SQT)v0*v0;
                s1 += v1; sq1 += (SQT)v1*v1;
                s2 += v2; sq2 += (SQT)v2*v2;
                nzm++;
            }
        جوڙ[0] = s0; جوڙ[1] = s1; جوڙ[2] = s2;
        sqsum[0] = sq0; sqsum[1] = sq1; sqsum[2] = sq2;
    }
    وإلا إذا( cn == 4 )
    {
        ST s0 = جوڙ[0], s1 = جوڙ[1], s2 = جوڙ[2], s3 = جوڙ[3];
        SQT sq0 = sqsum[0], sq1 = sqsum[1], sq2 = sqsum[2], sq3 = sqsum[3];
        لكل( i = 0; i < len; i++, src += 4 )
            إذا( نقاب[i] )
            {
                T v0 = src[0], v1 = src[1], v2 = src[2], v3 = src[3];
                s0 += v0; sq0 += (SQT)v0*v0;
                s1 += v1; sq1 += (SQT)v1*v1;
                s2 += v2; sq2 += (SQT)v2*v2;
                s3 += v3; sq3 += (SQT)v3*v3;
                nzm++;
            }
        جوڙ[0] = s0; جوڙ[1] = s1; جوڙ[2] = s2; جوڙ[3] = s3;
        sqsum[0] = sq0; sqsum[1] = sq1; sqsum[2] = sq2; sqsum[3] = sq3;
    }
    وإلا
    {
        لكل( i = 0; i < len; i++, src += cn )
            إذا( نقاب[i] )
            {
                لكل( صحيح k = 0; k < cn; k++ )
                {
                    ST v = (ST)src[k];
                    ST s = جوڙ[k] + v;
                    SQT sq = sqsum[k] + (SQT)v*v;
                    جوڙ[k] = s; sqsum[k] = sq;
                }
                nzm++;
            }
    }
    أرجع nzm;
}


ساكن صحيح sqsum8u( ثابت uchar* src, ثابت uchar* نقاب, صحيح* جوڙ, صحيح* sqsum, صحيح len, صحيح cn )
{ CV_INSTRUMENT_REGION(); أرجع sumsqr_(src, نقاب, جوڙ, sqsum, len, cn); }

ساكن صحيح sqsum8s( ثابت schar* src, ثابت uchar* نقاب, صحيح* جوڙ, صحيح* sqsum, صحيح len, صحيح cn )
{ CV_INSTRUMENT_REGION(); أرجع sumsqr_(src, نقاب, جوڙ, sqsum, len, cn); }

ساكن صحيح sqsum16u( ثابت ushort* src, ثابت uchar* نقاب, صحيح* جوڙ, مزدوج* sqsum, صحيح len, صحيح cn )
{ CV_INSTRUMENT_REGION(); أرجع sumsqr_(src, نقاب, جوڙ, sqsum, len, cn); }

ساكن صحيح sqsum16s( ثابت ننڍو* src, ثابت uchar* نقاب, صحيح* جوڙ, مزدوج* sqsum, صحيح len, صحيح cn )
{ CV_INSTRUMENT_REGION(); أرجع sumsqr_(src, نقاب, جوڙ, sqsum, len, cn); }

ساكن صحيح sqsum32s( ثابت صحيح* src, ثابت uchar* نقاب, مزدوج* جوڙ, مزدوج* sqsum, صحيح len, صحيح cn )
{ CV_INSTRUMENT_REGION(); أرجع sumsqr_(src, نقاب, جوڙ, sqsum, len, cn); }

ساكن صحيح sqsum32f( ثابت عائم* src, ثابت uchar* نقاب, مزدوج* جوڙ, مزدوج* sqsum, صحيح len, صحيح cn )
{ CV_INSTRUMENT_REGION(); أرجع sumsqr_(src, نقاب, جوڙ, sqsum, len, cn); }

ساكن صحيح sqsum64f( ثابت مزدوج* src, ثابت uchar* نقاب, مزدوج* جوڙ, مزدوج* sqsum, صحيح len, صحيح cn )
{ CV_INSTRUMENT_REGION(); أرجع sumsqr_(src, نقاب, جوڙ, sqsum, len, cn); }

ساكن صحيح sqsum16f( ثابت hfloat* src, ثابت uchar* نقاب, عائم* جوڙ, مزدوج* sqsum, صحيح len, صحيح cn )
{ CV_INSTRUMENT_REGION(); أرجع sumsqr_(src, نقاب, جوڙ, sqsum, len, cn); }

ساكن صحيح sqsum16bf( ثابت bfloat* src, ثابت uchar* نقاب, عائم* جوڙ, مزدوج* sqsum, صحيح len, صحيح cn )
{ CV_INSTRUMENT_REGION(); أرجع sumsqr_(src, نقاب, جوڙ, sqsum, len, cn); }

ساكن صحيح sqsum64u( ثابت uint64* src, ثابت uchar* نقاب, مزدوج* جوڙ, مزدوج* sqsum, صحيح len, صحيح cn )
{ CV_INSTRUMENT_REGION(); أرجع sumsqr_(src, نقاب, جوڙ, sqsum, len, cn); }

ساكن صحيح sqsum64s( ثابت int64* src, ثابت uchar* نقاب, مزدوج* جوڙ, مزدوج* sqsum, صحيح len, صحيح cn )
{ CV_INSTRUMENT_REGION(); أرجع sumsqr_(src, نقاب, جوڙ, sqsum, len, cn); }

ساكن صحيح sqsum32u( ثابت بنانشان* src, ثابت uchar* نقاب, مزدوج* جوڙ, مزدوج* sqsum, صحيح len, صحيح cn )
{ CV_INSTRUMENT_REGION(); أرجع sumsqr_(src, نقاب, جوڙ, sqsum, len, cn); }

جوڙ_Sqr_Func get_جوڙ_Sqr_Func(صحيح اونهائي)
{
    CV_INSTRUMENT_REGION();
    ساكن جوڙ_Sqr_Func جوڙ_Sqr_Tab[CV_اونهائي_MAX] =
    {
        جوڙ_Sqr_Wrap<uchar, صحيح, صحيح, sqsum8u>, جوڙ_Sqr_Wrap<schar, صحيح, صحيح, sqsum8s>,
        جوڙ_Sqr_Wrap<ushort, صحيح, مزدوج, sqsum16u>, جوڙ_Sqr_Wrap<ننڍو, صحيح, مزدوج, sqsum16s>,
        جوڙ_Sqr_Wrap<صحيح, مزدوج, مزدوج, sqsum32s>, جوڙ_Sqr_Wrap<عائم, مزدوج, مزدوج, sqsum32f>,
        جوڙ_Sqr_Wrap<مزدوج, مزدوج, مزدوج, sqsum64f>,
        جوڙ_Sqr_Wrap<hfloat, عائم, مزدوج, sqsum16f>, جوڙ_Sqr_Wrap<bfloat, عائم, مزدوج, sqsum16bf>, 0,
        جوڙ_Sqr_Wrap<uint64, مزدوج, مزدوج, sqsum64u>, جوڙ_Sqr_Wrap<int64, مزدوج, مزدوج, sqsum64s>,
        جوڙ_Sqr_Wrap<بنانشان, مزدوج, مزدوج, sqsum32u>, 0
    };

    أرجع جوڙ_Sqr_Tab[اونهائي];
}

#جيڪڏهن_پورو
CV_CPU_OPTIMIZATION_NAMESPACE_پڇاڙي
} // namespace
