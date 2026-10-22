// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html

// Copyright (C) 2026, Advanced Micro Devices, Inc., all rights reserved.

#ਅਨ੍ਤਰ੍ਭਾਵ "precomp.hpp"
#ਅਨ੍ਤਰ੍ਭਾਵ "stat.hpp"

ਨਾਮਾਕਾਸ਼ cv {
ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਪੂਰਨ (*ਜੋੜ_Sqr_Func)(ਅਚਲ uchar*, ਅਚਲ uchar* ਨਕਾਬ, uchar*, uchar*, ਪੂਰਨ, ਪੂਰਨ);

CV_CPU_OPTIMIZATION_NAMESPACE_BEGIN

ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ T, ਕਿਸਮ_ਨਾਂ ST, ਕਿਸਮ_ਨਾਂ SQT, ਪੂਰਨ (*fn)(ਅਚਲ T*, ਅਚਲ uchar*, ST*, SQT*, ਪੂਰਨ, ਪੂਰਨ)>
ਸਥਿਰ ਪੂਰਨ ਜੋੜ_Sqr_Wrap(ਅਚਲ uchar* src, ਅਚਲ uchar* ਨਕਾਬ, uchar* ਜੋੜ, uchar* sqsum, ਪੂਰਨ len, ਪੂਰਨ cn)
{
    ਵਾਪਸ fn((ਅਚਲ T*)src, ਨਕਾਬ, (ST*)ਜੋੜ, (SQT*)sqsum, len, cn);
}

ਜੋੜ_Sqr_Func get_ਜੋੜ_Sqr_Func(ਪੂਰਨ ਡੂੰਘਾਈ);

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T, ਕਿਸਮ_ਨਾਂ ST, ਕਿਸਮ_ਨਾਂ SQT>
ਬਣਤਰ ਜੋੜ_Sqr_SIMD
{
    ਸਤਰਅੰਦਰ ਪੂਰਨ ਸੰਚਾਲਕ () (ਅਚਲ T *, ਅਚਲ uchar *, ST *, SQT *, ਪੂਰਨ, ਪੂਰਨ) ਅਚਲ
    {
        ਵਾਪਸ 0;
    }
};

#ਜੇ (CV_SIMD || CV_SIMD_SCALABLE)

ਸਾਂਚਾ <>
ਬਣਤਰ ਜੋੜ_Sqr_SIMD<uchar, ਪੂਰਨ, ਪੂਰਨ>
{
    ਪੂਰਨ ਸੰਚਾਲਕ () (ਅਚਲ uchar * src0, ਅਚਲ uchar * ਨਕਾਬ, ਪੂਰਨ * ਜੋੜ, ਪੂਰਨ * sqsum, ਪੂਰਨ len, ਪੂਰਨ cn) ਅਚਲ
    {
        ਜੇ (ਨਕਾਬ)
            ਵਾਪਸ 0;
        // cn==3: deinterleave so each lane belongs to one channel, then reduce per
        // channel. sum/sqsum stay exact in s32 within the dispatcher's 1<<15 block.
        ਜੇ (cn == 3)
        {
            ਅਚਲ ਪੂਰਨ vl8 = VTraits<v_uint8>::vlanes();
            v_int32 vs0 = vx_setzero_s32(), vs1 = vx_setzero_s32(), vs2 = vx_setzero_s32();
            v_int32 vq0 = vx_setzero_s32(), vq1 = vx_setzero_s32(), vq2 = vx_setzero_s32();
            ਸਵੈਚਾਲਿਤ acc = [](ਅਚਲ v_uint8& ch, v_int32& vs, v_int32& vq)
            {
                v_uint16 lo, hi; v_expand(ch, lo, hi);
                v_uint32 s0, s1, s2, s3; v_expand(lo, s0, s1); v_expand(hi, s2, s3);
                vs = v_ਜੋੜੋ(vs, v_reinterpret_as_s32(v_ਜੋੜੋ(v_ਜੋੜੋ(s0, s1), v_ਜੋੜੋ(s2, s3))));
                v_int16 d0 = v_reinterpret_as_s16(lo), d1 = v_reinterpret_as_s16(hi);
                vq = v_ਜੋੜੋ(vq, v_ਜੋੜੋ(v_dotprod(d0, d0), v_dotprod(d1, d1)));
            };
            ਪੂਰਨ e = 0;
            ਲਈ (; e <= len - vl8; e += vl8)
            {
                v_uint8 a, b, c; v_ਲੱਦੋ_deinterleave(src0 + e * 3, a, b, c);
                acc(a, vs0, vq0); acc(b, vs1, vq1); acc(c, vs2, vq2);
            }
            ਜੋੜ[0] += v_reduce_ਜੋੜ(vs0); ਜੋੜ[1] += v_reduce_ਜੋੜ(vs1); ਜੋੜ[2] += v_reduce_ਜੋੜ(vs2);
            sqsum[0] += v_reduce_ਜੋੜ(vq0); sqsum[1] += v_reduce_ਜੋੜ(vq1); sqsum[2] += v_reduce_ਜੋੜ(vq2);
            v_cleanup();
            ਵਾਪਸ e;
        }
        ਜੇ (cn != 1 && cn != 2 && cn != 4)
            ਵਾਪਸ 0;
        len *= cn;

        ਪੂਰਨ x = 0;
        v_int32 v_ਜੋੜ = vx_setzero_s32();
        v_int32 v_sqsum = vx_setzero_s32();

        ਅਚਲ ਪੂਰਨ len0 = len & -VTraits<v_uint8>::vlanes();
        ਜਦੋਂ(x < len0)
        {
            ਅਚਲ ਪੂਰਨ len_tmp = min(x + 256*VTraits<v_uint16>::vlanes(), len0);
            v_uint16 v_sum16 = vx_setzero_u16();
            ਲਈ ( ; x < len_tmp; x += VTraits<v_uint8>::vlanes())
            {
                v_uint16 v_src0 = vx_ਲੱਦੋ_expand(src0 + x);
                v_uint16 v_src1 = vx_ਲੱਦੋ_expand(src0 + x + VTraits<v_uint16>::vlanes());
                v_sum16 = v_ਜੋੜੋ(v_sum16, v_ਜੋੜੋ(v_src0, v_src1));
                v_int16 v_tmp0, v_tmp1;
                v_zip(v_reinterpret_as_s16(v_src0), v_reinterpret_as_s16(v_src1), v_tmp0, v_tmp1);
                v_sqsum = v_ਜੋੜੋ(v_sqsum, v_ਜੋੜੋ(v_dotprod(v_tmp0, v_tmp0), v_dotprod(v_tmp1, v_tmp1)));
            }
            v_uint32 v_half0, v_half1;
            v_expand(v_sum16, v_half0, v_half1);
            v_ਜੋੜ = v_ਜੋੜੋ(v_ਜੋੜ, v_reinterpret_as_s32(v_ਜੋੜੋ(v_half0, v_half1)));
        }
        ਜੇ (x <= len - VTraits<v_uint16>::vlanes())
        {
            v_uint16 v_src = vx_ਲੱਦੋ_expand(src0 + x);
            v_uint16 v_half = v_combine_ਉੱਚਾ(v_src, v_src);

            v_uint32 v_tmp0, v_tmp1;
            v_expand(v_ਜੋੜੋ(v_src, v_half), v_tmp0, v_tmp1);
            v_ਜੋੜ = v_ਜੋੜੋ(v_ਜੋੜ, v_reinterpret_as_s32(v_tmp0));

            v_int16 v_tmp2, v_tmp3;
            v_zip(v_reinterpret_as_s16(v_src), v_reinterpret_as_s16(v_half), v_tmp2, v_tmp3);
            v_sqsum = v_ਜੋੜੋ(v_sqsum, v_dotprod(v_tmp2, v_tmp2));
            x += VTraits<v_uint16>::vlanes();
        }

        ਜੇ (cn == 1)
        {
            *ਜੋੜ += v_reduce_ਜੋੜ(v_ਜੋੜ);
            *sqsum += v_reduce_ਜੋੜ(v_sqsum);
        }
        ਨਹੀਂਤਾਂ
        {
            ਪੂਰਨ CV_DECL_ALIGNED(CV_SIMD_ਚੌੜਾਈ) ar[2 * VTraits<v_int32>::max_nlanes];
            v_store(ar, v_ਜੋੜ);
            v_store(ar + VTraits<v_int32>::vlanes(), v_sqsum);
            ਲਈ (ਪੂਰਨ i = 0; i < VTraits<v_int32>::vlanes(); ++i)
            {
                ਜੋੜ[i % cn] += ar[i];
                sqsum[i % cn] += ar[VTraits<v_int32>::vlanes() + i];
            }
        }
        v_cleanup();
        ਵਾਪਸ x / cn;
    }
};

ਸਾਂਚਾ <>
ਬਣਤਰ ਜੋੜ_Sqr_SIMD<schar, ਪੂਰਨ, ਪੂਰਨ>
{
    ਪੂਰਨ ਸੰਚਾਲਕ () (ਅਚਲ schar * src0, ਅਚਲ uchar * ਨਕਾਬ, ਪੂਰਨ * ਜੋੜ, ਪੂਰਨ * sqsum, ਪੂਰਨ len, ਪੂਰਨ cn) ਅਚਲ
    {
        ਜੇ (ਨਕਾਬ)
            ਵਾਪਸ 0;
        ਜੇ (cn == 3)
        {
            ਅਚਲ ਪੂਰਨ vl8 = VTraits<v_int8>::vlanes();
            v_int32 vs0 = vx_setzero_s32(), vs1 = vx_setzero_s32(), vs2 = vx_setzero_s32();
            v_int32 vq0 = vx_setzero_s32(), vq1 = vx_setzero_s32(), vq2 = vx_setzero_s32();
            ਸਵੈਚਾਲਿਤ acc = [](ਅਚਲ v_int8& ch, v_int32& vs, v_int32& vq)
            {
                v_int16 lo, hi; v_expand(ch, lo, hi);
                v_int32 s0, s1, s2, s3; v_expand(lo, s0, s1); v_expand(hi, s2, s3);
                vs = v_ਜੋੜੋ(vs, v_ਜੋੜੋ(v_ਜੋੜੋ(s0, s1), v_ਜੋੜੋ(s2, s3)));
                vq = v_ਜੋੜੋ(vq, v_ਜੋੜੋ(v_dotprod(lo, lo), v_dotprod(hi, hi)));
            };
            ਪੂਰਨ e = 0;
            ਲਈ (; e <= len - vl8; e += vl8)
            {
                v_int8 a, b, c; v_ਲੱਦੋ_deinterleave(src0 + e * 3, a, b, c);
                acc(a, vs0, vq0); acc(b, vs1, vq1); acc(c, vs2, vq2);
            }
            ਜੋੜ[0] += v_reduce_ਜੋੜ(vs0); ਜੋੜ[1] += v_reduce_ਜੋੜ(vs1); ਜੋੜ[2] += v_reduce_ਜੋੜ(vs2);
            sqsum[0] += v_reduce_ਜੋੜ(vq0); sqsum[1] += v_reduce_ਜੋੜ(vq1); sqsum[2] += v_reduce_ਜੋੜ(vq2);
            v_cleanup();
            ਵਾਪਸ e;
        }
        ਜੇ (cn != 1 && cn != 2 && cn != 4)
            ਵਾਪਸ 0;
        len *= cn;

        ਪੂਰਨ x = 0;
        v_int32 v_ਜੋੜ = vx_setzero_s32();
        v_int32 v_sqsum = vx_setzero_s32();

        ਅਚਲ ਪੂਰਨ len0 = len & -VTraits<v_int8>::vlanes();
        ਜਦੋਂ (x < len0)
        {
            ਅਚਲ ਪੂਰਨ len_tmp = min(x + 256 * VTraits<v_int16>::vlanes(), len0);
            v_int16 v_sum16 = vx_setzero_s16();
            ਲਈ (; x < len_tmp; x += VTraits<v_int8>::vlanes())
            {
                v_int16 v_src0 = vx_ਲੱਦੋ_expand(src0 + x);
                v_int16 v_src1 = vx_ਲੱਦੋ_expand(src0 + x + VTraits<v_int16>::vlanes());
                v_sum16 = v_ਜੋੜੋ(v_sum16, v_ਜੋੜੋ(v_src0, v_src1));
                v_int16 v_tmp0, v_tmp1;
                v_zip(v_src0, v_src1, v_tmp0, v_tmp1);
                v_sqsum = v_ਜੋੜੋ(v_sqsum, v_ਜੋੜੋ(v_dotprod(v_tmp0, v_tmp0), v_dotprod(v_tmp1, v_tmp1)));
            }
            v_int32 v_half0, v_half1;
            v_expand(v_sum16, v_half0, v_half1);
            v_ਜੋੜ = v_ਜੋੜੋ(v_ਜੋੜ, v_ਜੋੜੋ(v_half0, v_half1));
        }
        ਜੇ (x <= len - VTraits<v_int16>::vlanes())
        {
            v_int16 v_src = vx_ਲੱਦੋ_expand(src0 + x);
            v_int16 v_half = v_combine_ਉੱਚਾ(v_src, v_src);

            v_int32 v_tmp0, v_tmp1;
            v_expand(v_ਜੋੜੋ(v_src, v_half), v_tmp0, v_tmp1);
            v_ਜੋੜ = v_ਜੋੜੋ(v_ਜੋੜ, v_tmp0);

            v_int16 v_tmp2, v_tmp3;
            v_zip(v_src, v_half, v_tmp2, v_tmp3);
            v_sqsum = v_ਜੋੜੋ(v_sqsum, v_dotprod(v_tmp2, v_tmp2));
            x += VTraits<v_int16>::vlanes();
        }

        ਜੇ (cn == 1)
        {
            *ਜੋੜ += v_reduce_ਜੋੜ(v_ਜੋੜ);
            *sqsum += v_reduce_ਜੋੜ(v_sqsum);
        }
        ਨਹੀਂਤਾਂ
        {
            ਪੂਰਨ CV_DECL_ALIGNED(CV_SIMD_ਚੌੜਾਈ) ar[2 * VTraits<v_int32>::max_nlanes];
            v_store(ar, v_ਜੋੜ);
            v_store(ar + VTraits<v_int32>::vlanes(), v_sqsum);
            ਲਈ (ਪੂਰਨ i = 0; i < VTraits<v_int32>::vlanes(); ++i)
            {
                ਜੋੜ[i % cn] += ar[i];
                sqsum[i % cn] += ar[VTraits<v_int32>::vlanes() + i];
            }
        }
        v_cleanup();
        ਵਾਪਸ x / cn;
    }
};

#ਜੇ (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)

// Expand a 16-bit vector into two s32 halves. The unsigned overload reinterprets
// after a widening expand (values 0..65535 stay non-negative in s32, so the
// subsequent v_cvt_f64 is exact); the signed overload expands directly. This is
// the only real difference between the ushort and short reductions below.
ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ ਜੋੜ_Sqr_Expand_S32(ਅਚਲ v_uint16& v, v_int32& lo, v_int32& hi)
{
    v_uint32 a, b; v_expand(v, a, b);
    lo = v_reinterpret_as_s32(a); hi = v_reinterpret_as_s32(b);
}
ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ ਜੋੜ_Sqr_Expand_S32(ਅਚਲ v_int16& v, v_int32& lo, v_int32& hi)
{
    v_expand(v, lo, hi);
}

// Shared 16-bit -> {s32 sum, f64 sqsum} reduction for ushort/short. The register
// type is deduced from the pointer, so the body is identical for both depths.
ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T>
ਸਥਿਰ ਸਤਰਅੰਦਰ ਪੂਰਨ ਜੋੜ_Sqr16_ਮੰਜ਼ਿਲ_F64(ਅਚਲ T* src0, ਪੂਰਨ* ਜੋੜ, ਦੁੱਗਣਾ* sqsum, ਪੂਰਨ len, ਪੂਰਨ cn)
{
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ decltype(vx_ਲੱਦੋ(src0)) VT; // v_uint16 or v_int16

    ਜੇ (cn == 3)
    {
        ਅਚਲ ਪੂਰਨ vl = VTraits<VT>::vlanes();
        v_int32 vs0 = vx_setzero_s32(), vs1 = vx_setzero_s32(), vs2 = vx_setzero_s32();
        v_float64 vq0 = vx_setzero_f64(), vq1 = vx_setzero_f64(), vq2 = vx_setzero_f64();
        ਸਵੈਚਾਲਿਤ acc = [](ਅਚਲ VT& ch, v_int32& vs, v_float64& vq)
        {
            v_int32 lo, hi; ਜੋੜ_Sqr_Expand_S32(ch, lo, hi);
            vs = v_ਜੋੜੋ(vs, v_ਜੋੜੋ(lo, hi));
            v_float64 f0 = v_cvt_f64(lo), f1 = v_cvt_f64_ਉੱਚਾ(lo);
            v_float64 f2 = v_cvt_f64(hi), f3 = v_cvt_f64_ਉੱਚਾ(hi);
            vq = v_fma(f0, f0, v_fma(f1, f1, v_fma(f2, f2, v_fma(f3, f3, vq))));
        };
        ਪੂਰਨ e = 0;
        ਲਈ (; e <= len - vl; e += vl)
        {
            VT a, b, c; v_ਲੱਦੋ_deinterleave(src0 + e * 3, a, b, c);
            acc(a, vs0, vq0); acc(b, vs1, vq1); acc(c, vs2, vq2);
        }
        ਜੋੜ[0] += v_reduce_ਜੋੜ(vs0); ਜੋੜ[1] += v_reduce_ਜੋੜ(vs1); ਜੋੜ[2] += v_reduce_ਜੋੜ(vs2);
        sqsum[0] += v_reduce_ਜੋੜ(vq0); sqsum[1] += v_reduce_ਜੋੜ(vq1); sqsum[2] += v_reduce_ਜੋੜ(vq2);
        v_cleanup();
        ਵਾਪਸ e;
    }
    ਜੇ (cn != 1 && cn != 2 && cn != 4)
        ਵਾਪਸ 0;
    len *= cn;

    ਅਚਲ ਪੂਰਨ vl16 = VTraits<VT>::vlanes();
    ਅਚਲ ਪੂਰਨ vl32 = VTraits<v_int32>::vlanes();
    ਅਚਲ ਪੂਰਨ vl64 = VTraits<v_float64>::vlanes();

    // The lane->channel scatter below (i % cn) and the returned pixel count
    // (x / cn) require the lane counts to be a multiple of cn. This always holds
    // for fixed-width SIMD (vlanes is a power of two >= cn for cn in {1,2,4}),
    // but scalable backends (e.g. RVV) may report a non-multiple; fall back to
    // scalar there. vl16 == 2*vl32, so checking vl32 covers both scatter loops.
    ਜੇ (vl32 % cn != 0)
        ਵਾਪਸ 0;

    v_int32 v_ਜੋੜ = vx_setzero_s32();
    v_float64 v_sq0 = vx_setzero_f64(), v_sq1 = vx_setzero_f64();
    v_float64 v_sq2 = vx_setzero_f64(), v_sq3 = vx_setzero_f64();

    ਪੂਰਨ x = 0;
    ਲਈ (; x <= len - vl16; x += vl16)
    {
        VT v_src = vx_ਲੱਦੋ(src0 + x);
        v_int32 lo, hi; ਜੋੜ_Sqr_Expand_S32(v_src, lo, hi);
        v_ਜੋੜ = v_ਜੋੜੋ(v_ਜੋੜ, v_ਜੋੜੋ(lo, hi));

        v_float64 f0 = v_cvt_f64(lo);
        v_float64 f1 = v_cvt_f64_ਉੱਚਾ(lo);
        v_float64 f2 = v_cvt_f64(hi);
        v_float64 f3 = v_cvt_f64_ਉੱਚਾ(hi);
        v_sq0 = v_fma(f0, f0, v_sq0);
        v_sq1 = v_fma(f1, f1, v_sq1);
        v_sq2 = v_fma(f2, f2, v_sq2);
        v_sq3 = v_fma(f3, f3, v_sq3);
    }

    ਪੂਰਨ    CV_DECL_ALIGNED(CV_SIMD_ਚੌੜਾਈ) sbuf[VTraits<v_int32>::max_nlanes];
    ਦੁੱਗਣਾ CV_DECL_ALIGNED(CV_SIMD_ਚੌੜਾਈ) qbuf[VTraits<v_float64>::max_nlanes * 4];
    v_store_aligned(sbuf, v_ਜੋੜ);
    v_store_aligned(qbuf + vl64 * 0, v_sq0);
    v_store_aligned(qbuf + vl64 * 1, v_sq1);
    v_store_aligned(qbuf + vl64 * 2, v_sq2);
    v_store_aligned(qbuf + vl64 * 3, v_sq3);

    // sbuf lane j (j<vl32) holds samples j and j+vl32 -> channel j%cn (vl32%cn==0).
    // qbuf index i (i<vl16) holds sample lane i           -> channel i%cn (vl16%cn==0).
    ਲਈ (ਪੂਰਨ i = 0; i < vl32; ++i)
        ਜੋੜ[i % cn] += sbuf[i];
    ਲਈ (ਪੂਰਨ i = 0; i < vl16; ++i)
        sqsum[i % cn] += qbuf[i];

    v_cleanup();
    ਵਾਪਸ x / cn;
}

// Shared 32-bit -> f64 reduction for int/float. Both sum and sqsum accumulate in
// f64 to match the scalar reference's double accumulation; values are widened to
// f64 before squaring so the square is computed in double (same as (double)v*v).
// The register type is deduced from the pointer, so int and float share one body.
ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T>
ਸਥਿਰ ਸਤਰਅੰਦਰ ਪੂਰਨ ਜੋੜ_Sqr32_ਮੰਜ਼ਿਲ_F64(ਅਚਲ T* src0, ਦੁੱਗਣਾ* ਜੋੜ, ਦੁੱਗਣਾ* sqsum, ਪੂਰਨ len, ਪੂਰਨ cn)
{
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ decltype(vx_ਲੱਦੋ(src0)) VT; // v_int32 or v_float32

    ਜੇ (cn == 3)
    {
        ਅਚਲ ਪੂਰਨ vl = VTraits<VT>::vlanes();
        v_float64 vs0 = vx_setzero_f64(), vs1 = vx_setzero_f64(), vs2 = vx_setzero_f64();
        v_float64 vq0 = vx_setzero_f64(), vq1 = vx_setzero_f64(), vq2 = vx_setzero_f64();
        ਸਵੈਚਾਲਿਤ acc = [](ਅਚਲ VT& ch, v_float64& vs, v_float64& vq)
        {
            v_float64 f0 = v_cvt_f64(ch), f1 = v_cvt_f64_ਉੱਚਾ(ch);
            vs = v_ਜੋੜੋ(vs, v_ਜੋੜੋ(f0, f1));
            vq = v_fma(f0, f0, v_fma(f1, f1, vq));
        };
        ਪੂਰਨ e = 0;
        ਲਈ (; e <= len - vl; e += vl)
        {
            VT a, b, c; v_ਲੱਦੋ_deinterleave(src0 + e * 3, a, b, c);
            acc(a, vs0, vq0); acc(b, vs1, vq1); acc(c, vs2, vq2);
        }
        ਜੋੜ[0] += v_reduce_ਜੋੜ(vs0); ਜੋੜ[1] += v_reduce_ਜੋੜ(vs1); ਜੋੜ[2] += v_reduce_ਜੋੜ(vs2);
        sqsum[0] += v_reduce_ਜੋੜ(vq0); sqsum[1] += v_reduce_ਜੋੜ(vq1); sqsum[2] += v_reduce_ਜੋੜ(vq2);
        v_cleanup();
        ਵਾਪਸ e;
    }
    ਜੇ (cn != 1 && cn != 2 && cn != 4)
        ਵਾਪਸ 0;
    len *= cn;

    ਅਚਲ ਪੂਰਨ vl32 = VTraits<VT>::vlanes();
    ਅਚਲ ਪੂਰਨ vl64 = VTraits<v_float64>::vlanes();

    // See note in sumSqr16ToF64: guard scalable backends whose lane count may
    // not divide cn (the i % cn scatter / x / cn rely on it).
    ਜੇ (vl32 % cn != 0)
        ਵਾਪਸ 0;

    v_float64 vs0 = vx_setzero_f64(), vs1 = vx_setzero_f64();
    v_float64 vq0 = vx_setzero_f64(), vq1 = vx_setzero_f64();

    ਪੂਰਨ x = 0;
    ਲਈ (; x <= len - vl32; x += vl32)
    {
        VT v_src = vx_ਲੱਦੋ(src0 + x);
        v_float64 f0 = v_cvt_f64(v_src);
        v_float64 f1 = v_cvt_f64_ਉੱਚਾ(v_src);
        vs0 = v_ਜੋੜੋ(vs0, f0);
        vs1 = v_ਜੋੜੋ(vs1, f1);
        vq0 = v_fma(f0, f0, vq0);
        vq1 = v_fma(f1, f1, vq1);
    }

    ਦੁੱਗਣਾ CV_DECL_ALIGNED(CV_SIMD_ਚੌੜਾਈ) sbuf[VTraits<v_float64>::max_nlanes * 2];
    ਦੁੱਗਣਾ CV_DECL_ALIGNED(CV_SIMD_ਚੌੜਾਈ) qbuf[VTraits<v_float64>::max_nlanes * 2];
    v_store_aligned(sbuf, vs0);          v_store_aligned(sbuf + vl64, vs1);
    v_store_aligned(qbuf, vq0);          v_store_aligned(qbuf + vl64, vq1);

    ਲਈ (ਪੂਰਨ i = 0; i < vl32; ++i)
    {
        ਜੋੜ[i % cn]   += sbuf[i];
        sqsum[i % cn] += qbuf[i];
    }

    v_cleanup();
    ਵਾਪਸ x / cn;
}

ਸਾਂਚਾ <>
ਬਣਤਰ ਜੋੜ_Sqr_SIMD<ushort, ਪੂਰਨ, ਦੁੱਗਣਾ>
{
    ਪੂਰਨ ਸੰਚਾਲਕ () (ਅਚਲ ushort* src0, ਅਚਲ uchar* ਨਕਾਬ, ਪੂਰਨ* ਜੋੜ, ਦੁੱਗਣਾ* sqsum, ਪੂਰਨ len, ਪੂਰਨ cn) ਅਚਲ
    { ਵਾਪਸ ਨਕਾਬ ? 0 : ਜੋੜ_Sqr16_ਮੰਜ਼ਿਲ_F64(src0, ਜੋੜ, sqsum, len, cn); }
};

ਸਾਂਚਾ <>
ਬਣਤਰ ਜੋੜ_Sqr_SIMD<ਛੋਟਾ, ਪੂਰਨ, ਦੁੱਗਣਾ>
{
    ਪੂਰਨ ਸੰਚਾਲਕ () (ਅਚਲ ਛੋਟਾ* src0, ਅਚਲ uchar* ਨਕਾਬ, ਪੂਰਨ* ਜੋੜ, ਦੁੱਗਣਾ* sqsum, ਪੂਰਨ len, ਪੂਰਨ cn) ਅਚਲ
    { ਵਾਪਸ ਨਕਾਬ ? 0 : ਜੋੜ_Sqr16_ਮੰਜ਼ਿਲ_F64(src0, ਜੋੜ, sqsum, len, cn); }
};

ਸਾਂਚਾ <>
ਬਣਤਰ ਜੋੜ_Sqr_SIMD<ਪੂਰਨ, ਦੁੱਗਣਾ, ਦੁੱਗਣਾ>
{
    ਪੂਰਨ ਸੰਚਾਲਕ () (ਅਚਲ ਪੂਰਨ* src0, ਅਚਲ uchar* ਨਕਾਬ, ਦੁੱਗਣਾ* ਜੋੜ, ਦੁੱਗਣਾ* sqsum, ਪੂਰਨ len, ਪੂਰਨ cn) ਅਚਲ
    { ਵਾਪਸ ਨਕਾਬ ? 0 : ਜੋੜ_Sqr32_ਮੰਜ਼ਿਲ_F64(src0, ਜੋੜ, sqsum, len, cn); }
};

ਸਾਂਚਾ <>
ਬਣਤਰ ਜੋੜ_Sqr_SIMD<ਦਸ਼ਮਲਵ, ਦੁੱਗਣਾ, ਦੁੱਗਣਾ>
{
    ਪੂਰਨ ਸੰਚਾਲਕ () (ਅਚਲ ਦਸ਼ਮਲਵ* src0, ਅਚਲ uchar* ਨਕਾਬ, ਦੁੱਗਣਾ* ਜੋੜ, ਦੁੱਗਣਾ* sqsum, ਪੂਰਨ len, ਪੂਰਨ cn) ਅਚਲ
    { ਵਾਪਸ ਨਕਾਬ ? 0 : ਜੋੜ_Sqr32_ਮੰਜ਼ਿਲ_F64(src0, ਜੋੜ, sqsum, len, cn); }
};

#ਜੇ_ਅੰਤ // CV_SIMD_64F

#ਜੇ_ਅੰਤ

ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ T, ਕਿਸਮ_ਨਾਂ ST, ਕਿਸਮ_ਨਾਂ SQT>
ਸਥਿਰ ਪੂਰਨ sumsqr_(ਅਚਲ T* src0, ਅਚਲ uchar* ਨਕਾਬ, ST* ਜੋੜ, SQT* sqsum, ਪੂਰਨ len, ਪੂਰਨ cn )
{
    ਅਚਲ T* src = src0;

    ਜੇ( !ਨਕਾਬ )
    {
        ਜੋੜ_Sqr_SIMD<T, ST, SQT> vop;
        ਪੂਰਨ x = vop(src0, ਨਕਾਬ, ਜੋੜ, sqsum, len, cn), k = cn % 4;
        src = src0 + x * cn;

        ਜੇ( k == 1 )
        {
            ST s0 = ਜੋੜ[0];
            SQT sq0 = sqsum[0];
            ਲਈ(ਪੂਰਨ i = x; i < len; i++, src += cn )
            {
                ST v = (ST)src[0];
                s0 += v; sq0 += (SQT)v*v;
            }
            ਜੋੜ[0] = s0;
            sqsum[0] = sq0;
        }
        ਨਹੀਂਤਾਂ ਜੇ( k == 2 )
        {
            ST s0 = ਜੋੜ[0], s1 = ਜੋੜ[1];
            SQT sq0 = sqsum[0], sq1 = sqsum[1];
            ਲਈ(ਪੂਰਨ i = x; i < len; i++, src += cn )
            {
                ST v0 = (ST)src[0], v1 = (ST)src[1];
                s0 += v0; sq0 += (SQT)v0*v0;
                s1 += v1; sq1 += (SQT)v1*v1;
            }
            ਜੋੜ[0] = s0; ਜੋੜ[1] = s1;
            sqsum[0] = sq0; sqsum[1] = sq1;
        }
        ਨਹੀਂਤਾਂ ਜੇ( k == 3 )
        {
            ST s0 = ਜੋੜ[0], s1 = ਜੋੜ[1], s2 = ਜੋੜ[2];
            SQT sq0 = sqsum[0], sq1 = sqsum[1], sq2 = sqsum[2];
            ਲਈ(ਪੂਰਨ i = x; i < len; i++, src += cn )
            {
                ST v0 = (ST)src[0], v1 = (ST)src[1], v2 = (ST)src[2];
                s0 += v0; sq0 += (SQT)v0*v0;
                s1 += v1; sq1 += (SQT)v1*v1;
                s2 += v2; sq2 += (SQT)v2*v2;
            }
            ਜੋੜ[0] = s0; ਜੋੜ[1] = s1; ਜੋੜ[2] = s2;
            sqsum[0] = sq0; sqsum[1] = sq1; sqsum[2] = sq2;
        }

        ਲਈ( ; k < cn; k += 4 )
        {
            src = src0 + x * cn + k;
            ST s0 = ਜੋੜ[k], s1 = ਜੋੜ[k+1], s2 = ਜੋੜ[k+2], s3 = ਜੋੜ[k+3];
            SQT sq0 = sqsum[k], sq1 = sqsum[k+1], sq2 = sqsum[k+2], sq3 = sqsum[k+3];
            ਲਈ(ਪੂਰਨ i = x; i < len; i++, src += cn )
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
            ਜੋੜ[k] = s0; ਜੋੜ[k+1] = s1;
            ਜੋੜ[k+2] = s2; ਜੋੜ[k+3] = s3;
            sqsum[k] = sq0; sqsum[k+1] = sq1;
            sqsum[k+2] = sq2; sqsum[k+3] = sq3;
        }
        ਵਾਪਸ len;
    }

    ਪੂਰਨ i, nzm = 0;

    ਜੇ( cn == 1 )
    {
        ST s0 = ਜੋੜ[0];
        SQT sq0 = sqsum[0];
        ਲਈ( i = 0; i < len; i++ )
            ਜੇ( ਨਕਾਬ[i] )
            {
                ST v = (ST)src[i];
                s0 += v; sq0 += (SQT)v*v;
                nzm++;
            }
        ਜੋੜ[0] = s0;
        sqsum[0] = sq0;
    }
    ਨਹੀਂਤਾਂ ਜੇ( cn == 2 )
    {
        ST s0 = ਜੋੜ[0], s1 = ਜੋੜ[1];
        SQT sq0 = sqsum[0], sq1 = sqsum[1];
        ਲਈ( i = 0; i < len; i++, src += 2 )
            ਜੇ( ਨਕਾਬ[i] )
            {
                T v0 = src[0], v1 = src[1];
                s0 += v0; sq0 += (SQT)v0*v0;
                s1 += v1; sq1 += (SQT)v1*v1;
                nzm++;
            }
        ਜੋੜ[0] = s0; ਜੋੜ[1] = s1;
        sqsum[0] = sq0; sqsum[1] = sq1;
    }
    ਨਹੀਂਤਾਂ ਜੇ( cn == 3 )
    {
        ST s0 = ਜੋੜ[0], s1 = ਜੋੜ[1], s2 = ਜੋੜ[2];
        SQT sq0 = sqsum[0], sq1 = sqsum[1], sq2 = sqsum[2];
        ਲਈ( i = 0; i < len; i++, src += 3 )
            ਜੇ( ਨਕਾਬ[i] )
            {
                ST v0 = (ST)src[0], v1 = (ST)src[1], v2 = (ST)src[2];
                s0 += v0; sq0 += (SQT)v0*v0;
                s1 += v1; sq1 += (SQT)v1*v1;
                s2 += v2; sq2 += (SQT)v2*v2;
                nzm++;
            }
        ਜੋੜ[0] = s0; ਜੋੜ[1] = s1; ਜੋੜ[2] = s2;
        sqsum[0] = sq0; sqsum[1] = sq1; sqsum[2] = sq2;
    }
    ਨਹੀਂਤਾਂ ਜੇ( cn == 4 )
    {
        ST s0 = ਜੋੜ[0], s1 = ਜੋੜ[1], s2 = ਜੋੜ[2], s3 = ਜੋੜ[3];
        SQT sq0 = sqsum[0], sq1 = sqsum[1], sq2 = sqsum[2], sq3 = sqsum[3];
        ਲਈ( i = 0; i < len; i++, src += 4 )
            ਜੇ( ਨਕਾਬ[i] )
            {
                T v0 = src[0], v1 = src[1], v2 = src[2], v3 = src[3];
                s0 += v0; sq0 += (SQT)v0*v0;
                s1 += v1; sq1 += (SQT)v1*v1;
                s2 += v2; sq2 += (SQT)v2*v2;
                s3 += v3; sq3 += (SQT)v3*v3;
                nzm++;
            }
        ਜੋੜ[0] = s0; ਜੋੜ[1] = s1; ਜੋੜ[2] = s2; ਜੋੜ[3] = s3;
        sqsum[0] = sq0; sqsum[1] = sq1; sqsum[2] = sq2; sqsum[3] = sq3;
    }
    ਨਹੀਂਤਾਂ
    {
        ਲਈ( i = 0; i < len; i++, src += cn )
            ਜੇ( ਨਕਾਬ[i] )
            {
                ਲਈ( ਪੂਰਨ k = 0; k < cn; k++ )
                {
                    ST v = (ST)src[k];
                    ST s = ਜੋੜ[k] + v;
                    SQT sq = sqsum[k] + (SQT)v*v;
                    ਜੋੜ[k] = s; sqsum[k] = sq;
                }
                nzm++;
            }
    }
    ਵਾਪਸ nzm;
}


ਸਥਿਰ ਪੂਰਨ sqsum8u( ਅਚਲ uchar* src, ਅਚਲ uchar* ਨਕਾਬ, ਪੂਰਨ* ਜੋੜ, ਪੂਰਨ* sqsum, ਪੂਰਨ len, ਪੂਰਨ cn )
{ CV_INSTRUMENT_REGION(); ਵਾਪਸ sumsqr_(src, ਨਕਾਬ, ਜੋੜ, sqsum, len, cn); }

ਸਥਿਰ ਪੂਰਨ sqsum8s( ਅਚਲ schar* src, ਅਚਲ uchar* ਨਕਾਬ, ਪੂਰਨ* ਜੋੜ, ਪੂਰਨ* sqsum, ਪੂਰਨ len, ਪੂਰਨ cn )
{ CV_INSTRUMENT_REGION(); ਵਾਪਸ sumsqr_(src, ਨਕਾਬ, ਜੋੜ, sqsum, len, cn); }

ਸਥਿਰ ਪੂਰਨ sqsum16u( ਅਚਲ ushort* src, ਅਚਲ uchar* ਨਕਾਬ, ਪੂਰਨ* ਜੋੜ, ਦੁੱਗਣਾ* sqsum, ਪੂਰਨ len, ਪੂਰਨ cn )
{ CV_INSTRUMENT_REGION(); ਵਾਪਸ sumsqr_(src, ਨਕਾਬ, ਜੋੜ, sqsum, len, cn); }

ਸਥਿਰ ਪੂਰਨ sqsum16s( ਅਚਲ ਛੋਟਾ* src, ਅਚਲ uchar* ਨਕਾਬ, ਪੂਰਨ* ਜੋੜ, ਦੁੱਗਣਾ* sqsum, ਪੂਰਨ len, ਪੂਰਨ cn )
{ CV_INSTRUMENT_REGION(); ਵਾਪਸ sumsqr_(src, ਨਕਾਬ, ਜੋੜ, sqsum, len, cn); }

ਸਥਿਰ ਪੂਰਨ sqsum32s( ਅਚਲ ਪੂਰਨ* src, ਅਚਲ uchar* ਨਕਾਬ, ਦੁੱਗਣਾ* ਜੋੜ, ਦੁੱਗਣਾ* sqsum, ਪੂਰਨ len, ਪੂਰਨ cn )
{ CV_INSTRUMENT_REGION(); ਵਾਪਸ sumsqr_(src, ਨਕਾਬ, ਜੋੜ, sqsum, len, cn); }

ਸਥਿਰ ਪੂਰਨ sqsum32f( ਅਚਲ ਦਸ਼ਮਲਵ* src, ਅਚਲ uchar* ਨਕਾਬ, ਦੁੱਗਣਾ* ਜੋੜ, ਦੁੱਗਣਾ* sqsum, ਪੂਰਨ len, ਪੂਰਨ cn )
{ CV_INSTRUMENT_REGION(); ਵਾਪਸ sumsqr_(src, ਨਕਾਬ, ਜੋੜ, sqsum, len, cn); }

ਸਥਿਰ ਪੂਰਨ sqsum64f( ਅਚਲ ਦੁੱਗਣਾ* src, ਅਚਲ uchar* ਨਕਾਬ, ਦੁੱਗਣਾ* ਜੋੜ, ਦੁੱਗਣਾ* sqsum, ਪੂਰਨ len, ਪੂਰਨ cn )
{ CV_INSTRUMENT_REGION(); ਵਾਪਸ sumsqr_(src, ਨਕਾਬ, ਜੋੜ, sqsum, len, cn); }

ਸਥਿਰ ਪੂਰਨ sqsum16f( ਅਚਲ hfloat* src, ਅਚਲ uchar* ਨਕਾਬ, ਦਸ਼ਮਲਵ* ਜੋੜ, ਦੁੱਗਣਾ* sqsum, ਪੂਰਨ len, ਪੂਰਨ cn )
{ CV_INSTRUMENT_REGION(); ਵਾਪਸ sumsqr_(src, ਨਕਾਬ, ਜੋੜ, sqsum, len, cn); }

ਸਥਿਰ ਪੂਰਨ sqsum16bf( ਅਚਲ bfloat* src, ਅਚਲ uchar* ਨਕਾਬ, ਦਸ਼ਮਲਵ* ਜੋੜ, ਦੁੱਗਣਾ* sqsum, ਪੂਰਨ len, ਪੂਰਨ cn )
{ CV_INSTRUMENT_REGION(); ਵਾਪਸ sumsqr_(src, ਨਕਾਬ, ਜੋੜ, sqsum, len, cn); }

ਸਥਿਰ ਪੂਰਨ sqsum64u( ਅਚਲ uint64* src, ਅਚਲ uchar* ਨਕਾਬ, ਦੁੱਗਣਾ* ਜੋੜ, ਦੁੱਗਣਾ* sqsum, ਪੂਰਨ len, ਪੂਰਨ cn )
{ CV_INSTRUMENT_REGION(); ਵਾਪਸ sumsqr_(src, ਨਕਾਬ, ਜੋੜ, sqsum, len, cn); }

ਸਥਿਰ ਪੂਰਨ sqsum64s( ਅਚਲ int64* src, ਅਚਲ uchar* ਨਕਾਬ, ਦੁੱਗਣਾ* ਜੋੜ, ਦੁੱਗਣਾ* sqsum, ਪੂਰਨ len, ਪੂਰਨ cn )
{ CV_INSTRUMENT_REGION(); ਵਾਪਸ sumsqr_(src, ਨਕਾਬ, ਜੋੜ, sqsum, len, cn); }

ਸਥਿਰ ਪੂਰਨ sqsum32u( ਅਚਲ ਬਿਨਾਨਿਸ਼ਾਨੀ* src, ਅਚਲ uchar* ਨਕਾਬ, ਦੁੱਗਣਾ* ਜੋੜ, ਦੁੱਗਣਾ* sqsum, ਪੂਰਨ len, ਪੂਰਨ cn )
{ CV_INSTRUMENT_REGION(); ਵਾਪਸ sumsqr_(src, ਨਕਾਬ, ਜੋੜ, sqsum, len, cn); }

ਜੋੜ_Sqr_Func get_ਜੋੜ_Sqr_Func(ਪੂਰਨ ਡੂੰਘਾਈ)
{
    CV_INSTRUMENT_REGION();
    ਸਥਿਰ ਜੋੜ_Sqr_Func ਜੋੜ_Sqr_Tab[CV_ਡੂੰਘਾਈ_MAX] =
    {
        ਜੋੜ_Sqr_Wrap<uchar, ਪੂਰਨ, ਪੂਰਨ, sqsum8u>, ਜੋੜ_Sqr_Wrap<schar, ਪੂਰਨ, ਪੂਰਨ, sqsum8s>,
        ਜੋੜ_Sqr_Wrap<ushort, ਪੂਰਨ, ਦੁੱਗਣਾ, sqsum16u>, ਜੋੜ_Sqr_Wrap<ਛੋਟਾ, ਪੂਰਨ, ਦੁੱਗਣਾ, sqsum16s>,
        ਜੋੜ_Sqr_Wrap<ਪੂਰਨ, ਦੁੱਗਣਾ, ਦੁੱਗਣਾ, sqsum32s>, ਜੋੜ_Sqr_Wrap<ਦਸ਼ਮਲਵ, ਦੁੱਗਣਾ, ਦੁੱਗਣਾ, sqsum32f>,
        ਜੋੜ_Sqr_Wrap<ਦੁੱਗਣਾ, ਦੁੱਗਣਾ, ਦੁੱਗਣਾ, sqsum64f>,
        ਜੋੜ_Sqr_Wrap<hfloat, ਦਸ਼ਮਲਵ, ਦੁੱਗਣਾ, sqsum16f>, ਜੋੜ_Sqr_Wrap<bfloat, ਦਸ਼ਮਲਵ, ਦੁੱਗਣਾ, sqsum16bf>, 0,
        ਜੋੜ_Sqr_Wrap<uint64, ਦੁੱਗਣਾ, ਦੁੱਗਣਾ, sqsum64u>, ਜੋੜ_Sqr_Wrap<int64, ਦੁੱਗਣਾ, ਦੁੱਗਣਾ, sqsum64s>,
        ਜੋੜ_Sqr_Wrap<ਬਿਨਾਨਿਸ਼ਾਨੀ, ਦੁੱਗਣਾ, ਦੁੱਗਣਾ, sqsum32u>, 0
    };

    ਵਾਪਸ ਜੋੜ_Sqr_Tab[ਡੂੰਘਾਈ];
}

#ਜੇ_ਅੰਤ
CV_CPU_OPTIMIZATION_NAMESPACE_ਸਿਰਾ
} // namespace
