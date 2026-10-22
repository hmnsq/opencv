// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html

// Copyright (C) 2026, Advanced Micro Devices, Inc., all rights reserved.

#include "precomp.hpp"
#include "stat.hpp"

namespace cv {
typedef ចំនួនគត់ (*SumSqrFunc)(const uchar*, const uchar* mask, uchar*, uchar*, ចំនួនគត់, ចំនួនគត់);

CV_CPU_OPTIMIZATION_NAMESPACE_BEGIN

template<typename T, typename ST, typename SQT, ចំនួនគត់ (*fn)(const T*, const uchar*, ST*, SQT*, ចំនួនគត់, ចំនួនគត់)>
static ចំនួនគត់ sumSqrWrap(const uchar* src, const uchar* mask, uchar* sum, uchar* sqsum, ចំនួនគត់ len, ចំនួនគត់ cn)
{
    ត្រឡប់ fn((const T*)src, mask, (ST*)sum, (SQT*)sqsum, len, cn);
}

SumSqrFunc getSumSqrFunc(ចំនួនគត់ depth);

#ifndef CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

template <typename T, typename ST, typename SQT>
sa_nracanaa SumSqr_SIMD
{
    anta_hstha ចំនួនគត់ operator () (const T *, const uchar *, ST *, SQT *, ចំនួនគត់, ចំនួនគត់) const
    {
        ត្រឡប់ 0;
    }
};

#ប្រសិនបើ (CV_SIMD || CV_SIMD_SCALABLE)

template <>
sa_nracanaa SumSqr_SIMD<uchar, ចំនួនគត់, ចំនួនគត់>
{
    ចំនួនគត់ operator () (const uchar * src0, const uchar * mask, ចំនួនគត់ * sum, ចំនួនគត់ * sqsum, ចំនួនគត់ len, ចំនួនគត់ cn) const
    {
        ប្រសិនបើ (mask)
            ត្រឡប់ 0;
        // cn==3: deinterleave so each lane belongs to one channel, then reduce per
        // channel. sum/sqsum stay exact in s32 within the dispatcher's 1<<15 block.
        ប្រសិនបើ (cn == 3)
        {
            const ចំនួនគត់ vl8 = VTraits<v_uint8>::vlanes();
            v_int32 vs0 = vx_setzero_s32(), vs1 = vx_setzero_s32(), vs2 = vx_setzero_s32();
            v_int32 vq0 = vx_setzero_s32(), vq1 = vx_setzero_s32(), vq2 = vx_setzero_s32();
            auto acc = [](const v_uint8& ch, v_int32& vs, v_int32& vq)
            {
                v_uint16 lo, hi; v_expand(ch, lo, hi);
                v_uint32 s0, s1, s2, s3; v_expand(lo, s0, s1); v_expand(hi, s2, s3);
                vs = v_add(vs, v_reinterpret_as_s32(v_add(v_add(s0, s1), v_add(s2, s3))));
                v_int16 d0 = v_reinterpret_as_s16(lo), d1 = v_reinterpret_as_s16(hi);
                vq = v_add(vq, v_add(v_dotprod(d0, d0), v_dotprod(d1, d1)));
            };
            ចំនួនគត់ e = 0;
            សម្រាប់ (; e <= len - vl8; e += vl8)
            {
                v_uint8 a, b, c; v_load_deinterleave(src0 + e * 3, a, b, c);
                acc(a, vs0, vq0); acc(b, vs1, vq1); acc(c, vs2, vq2);
            }
            sum[0] += v_reduce_sum(vs0); sum[1] += v_reduce_sum(vs1); sum[2] += v_reduce_sum(vs2);
            sqsum[0] += v_reduce_sum(vq0); sqsum[1] += v_reduce_sum(vq1); sqsum[2] += v_reduce_sum(vq2);
            v_cleanup();
            ត្រឡប់ e;
        }
        ប្រសិនបើ (cn != 1 && cn != 2 && cn != 4)
            ត្រឡប់ 0;
        len *= cn;

        ចំនួនគត់ x = 0;
        v_int32 v_sum = vx_setzero_s32();
        v_int32 v_sqsum = vx_setzero_s32();

        const ចំនួនគត់ len0 = len & -VTraits<v_uint8>::vlanes();
        ខណៈពេល(x < len0)
        {
            const ចំនួនគត់ len_tmp = min(x + 256*VTraits<v_uint16>::vlanes(), len0);
            v_uint16 v_sum16 = vx_setzero_u16();
            សម្រាប់ ( ; x < len_tmp; x += VTraits<v_uint8>::vlanes())
            {
                v_uint16 v_src0 = vx_load_expand(src0 + x);
                v_uint16 v_src1 = vx_load_expand(src0 + x + VTraits<v_uint16>::vlanes());
                v_sum16 = v_add(v_sum16, v_add(v_src0, v_src1));
                v_int16 v_tmp0, v_tmp1;
                v_zip(v_reinterpret_as_s16(v_src0), v_reinterpret_as_s16(v_src1), v_tmp0, v_tmp1);
                v_sqsum = v_add(v_sqsum, v_add(v_dotprod(v_tmp0, v_tmp0), v_dotprod(v_tmp1, v_tmp1)));
            }
            v_uint32 v_half0, v_half1;
            v_expand(v_sum16, v_half0, v_half1);
            v_sum = v_add(v_sum, v_reinterpret_as_s32(v_add(v_half0, v_half1)));
        }
        ប្រសិនបើ (x <= len - VTraits<v_uint16>::vlanes())
        {
            v_uint16 v_src = vx_load_expand(src0 + x);
            v_uint16 v_half = v_combine_high(v_src, v_src);

            v_uint32 v_tmp0, v_tmp1;
            v_expand(v_add(v_src, v_half), v_tmp0, v_tmp1);
            v_sum = v_add(v_sum, v_reinterpret_as_s32(v_tmp0));

            v_int16 v_tmp2, v_tmp3;
            v_zip(v_reinterpret_as_s16(v_src), v_reinterpret_as_s16(v_half), v_tmp2, v_tmp3);
            v_sqsum = v_add(v_sqsum, v_dotprod(v_tmp2, v_tmp2));
            x += VTraits<v_uint16>::vlanes();
        }

        ប្រសិនបើ (cn == 1)
        {
            *sum += v_reduce_sum(v_sum);
            *sqsum += v_reduce_sum(v_sqsum);
        }
        បើមិនដូច្នេះ
        {
            ចំនួនគត់ CV_DECL_ALIGNED(CV_SIMD_WIDTH) ar[2 * VTraits<v_int32>::max_nlanes];
            v_store(ar, v_sum);
            v_store(ar + VTraits<v_int32>::vlanes(), v_sqsum);
            សម្រាប់ (ចំនួនគត់ i = 0; i < VTraits<v_int32>::vlanes(); ++i)
            {
                sum[i % cn] += ar[i];
                sqsum[i % cn] += ar[VTraits<v_int32>::vlanes() + i];
            }
        }
        v_cleanup();
        ត្រឡប់ x / cn;
    }
};

template <>
sa_nracanaa SumSqr_SIMD<schar, ចំនួនគត់, ចំនួនគត់>
{
    ចំនួនគត់ operator () (const schar * src0, const uchar * mask, ចំនួនគត់ * sum, ចំនួនគត់ * sqsum, ចំនួនគត់ len, ចំនួនគត់ cn) const
    {
        ប្រសិនបើ (mask)
            ត្រឡប់ 0;
        ប្រសិនបើ (cn == 3)
        {
            const ចំនួនគត់ vl8 = VTraits<v_int8>::vlanes();
            v_int32 vs0 = vx_setzero_s32(), vs1 = vx_setzero_s32(), vs2 = vx_setzero_s32();
            v_int32 vq0 = vx_setzero_s32(), vq1 = vx_setzero_s32(), vq2 = vx_setzero_s32();
            auto acc = [](const v_int8& ch, v_int32& vs, v_int32& vq)
            {
                v_int16 lo, hi; v_expand(ch, lo, hi);
                v_int32 s0, s1, s2, s3; v_expand(lo, s0, s1); v_expand(hi, s2, s3);
                vs = v_add(vs, v_add(v_add(s0, s1), v_add(s2, s3)));
                vq = v_add(vq, v_add(v_dotprod(lo, lo), v_dotprod(hi, hi)));
            };
            ចំនួនគត់ e = 0;
            សម្រាប់ (; e <= len - vl8; e += vl8)
            {
                v_int8 a, b, c; v_load_deinterleave(src0 + e * 3, a, b, c);
                acc(a, vs0, vq0); acc(b, vs1, vq1); acc(c, vs2, vq2);
            }
            sum[0] += v_reduce_sum(vs0); sum[1] += v_reduce_sum(vs1); sum[2] += v_reduce_sum(vs2);
            sqsum[0] += v_reduce_sum(vq0); sqsum[1] += v_reduce_sum(vq1); sqsum[2] += v_reduce_sum(vq2);
            v_cleanup();
            ត្រឡប់ e;
        }
        ប្រសិនបើ (cn != 1 && cn != 2 && cn != 4)
            ត្រឡប់ 0;
        len *= cn;

        ចំនួនគត់ x = 0;
        v_int32 v_sum = vx_setzero_s32();
        v_int32 v_sqsum = vx_setzero_s32();

        const ចំនួនគត់ len0 = len & -VTraits<v_int8>::vlanes();
        ខណៈពេល (x < len0)
        {
            const ចំនួនគត់ len_tmp = min(x + 256 * VTraits<v_int16>::vlanes(), len0);
            v_int16 v_sum16 = vx_setzero_s16();
            សម្រាប់ (; x < len_tmp; x += VTraits<v_int8>::vlanes())
            {
                v_int16 v_src0 = vx_load_expand(src0 + x);
                v_int16 v_src1 = vx_load_expand(src0 + x + VTraits<v_int16>::vlanes());
                v_sum16 = v_add(v_sum16, v_add(v_src0, v_src1));
                v_int16 v_tmp0, v_tmp1;
                v_zip(v_src0, v_src1, v_tmp0, v_tmp1);
                v_sqsum = v_add(v_sqsum, v_add(v_dotprod(v_tmp0, v_tmp0), v_dotprod(v_tmp1, v_tmp1)));
            }
            v_int32 v_half0, v_half1;
            v_expand(v_sum16, v_half0, v_half1);
            v_sum = v_add(v_sum, v_add(v_half0, v_half1));
        }
        ប្រសិនបើ (x <= len - VTraits<v_int16>::vlanes())
        {
            v_int16 v_src = vx_load_expand(src0 + x);
            v_int16 v_half = v_combine_high(v_src, v_src);

            v_int32 v_tmp0, v_tmp1;
            v_expand(v_add(v_src, v_half), v_tmp0, v_tmp1);
            v_sum = v_add(v_sum, v_tmp0);

            v_int16 v_tmp2, v_tmp3;
            v_zip(v_src, v_half, v_tmp2, v_tmp3);
            v_sqsum = v_add(v_sqsum, v_dotprod(v_tmp2, v_tmp2));
            x += VTraits<v_int16>::vlanes();
        }

        ប្រសិនបើ (cn == 1)
        {
            *sum += v_reduce_sum(v_sum);
            *sqsum += v_reduce_sum(v_sqsum);
        }
        បើមិនដូច្នេះ
        {
            ចំនួនគត់ CV_DECL_ALIGNED(CV_SIMD_WIDTH) ar[2 * VTraits<v_int32>::max_nlanes];
            v_store(ar, v_sum);
            v_store(ar + VTraits<v_int32>::vlanes(), v_sqsum);
            សម្រាប់ (ចំនួនគត់ i = 0; i < VTraits<v_int32>::vlanes(); ++i)
            {
                sum[i % cn] += ar[i];
                sqsum[i % cn] += ar[VTraits<v_int32>::vlanes() + i];
            }
        }
        v_cleanup();
        ត្រឡប់ x / cn;
    }
};

#ប្រសិនបើ (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)

// Expand a 16-bit vector into two s32 halves. The unsigned overload reinterprets
// after a widening expand (values 0..65535 stay non-negative in s32, so the
// subsequent v_cvt_f64 is exact); the signed overload expands directly. This is
// the only real difference between the ushort and short reductions below.
static anta_hstha void sumSqrExpandS32(const v_uint16& v, v_int32& lo, v_int32& hi)
{
    v_uint32 a, b; v_expand(v, a, b);
    lo = v_reinterpret_as_s32(a); hi = v_reinterpret_as_s32(b);
}
static anta_hstha void sumSqrExpandS32(const v_int16& v, v_int32& lo, v_int32& hi)
{
    v_expand(v, lo, hi);
}

// Shared 16-bit -> {s32 sum, f64 sqsum} reduction for ushort/short. The register
// type is deduced from the pointer, so the body is identical for both depths.
template <typename T>
static anta_hstha ចំនួនគត់ sumSqr16ToF64(const T* src0, ចំនួនគត់* sum, dviguu_na* sqsum, ចំនួនគត់ len, ចំនួនគត់ cn)
{
    typedef decltype(vx_load(src0)) VT; // v_uint16 or v_int16

    ប្រសិនបើ (cn == 3)
    {
        const ចំនួនគត់ vl = VTraits<VT>::vlanes();
        v_int32 vs0 = vx_setzero_s32(), vs1 = vx_setzero_s32(), vs2 = vx_setzero_s32();
        v_float64 vq0 = vx_setzero_f64(), vq1 = vx_setzero_f64(), vq2 = vx_setzero_f64();
        auto acc = [](const VT& ch, v_int32& vs, v_float64& vq)
        {
            v_int32 lo, hi; sumSqrExpandS32(ch, lo, hi);
            vs = v_add(vs, v_add(lo, hi));
            v_float64 f0 = v_cvt_f64(lo), f1 = v_cvt_f64_high(lo);
            v_float64 f2 = v_cvt_f64(hi), f3 = v_cvt_f64_high(hi);
            vq = v_fma(f0, f0, v_fma(f1, f1, v_fma(f2, f2, v_fma(f3, f3, vq))));
        };
        ចំនួនគត់ e = 0;
        សម្រាប់ (; e <= len - vl; e += vl)
        {
            VT a, b, c; v_load_deinterleave(src0 + e * 3, a, b, c);
            acc(a, vs0, vq0); acc(b, vs1, vq1); acc(c, vs2, vq2);
        }
        sum[0] += v_reduce_sum(vs0); sum[1] += v_reduce_sum(vs1); sum[2] += v_reduce_sum(vs2);
        sqsum[0] += v_reduce_sum(vq0); sqsum[1] += v_reduce_sum(vq1); sqsum[2] += v_reduce_sum(vq2);
        v_cleanup();
        ត្រឡប់ e;
    }
    ប្រសិនបើ (cn != 1 && cn != 2 && cn != 4)
        ត្រឡប់ 0;
    len *= cn;

    const ចំនួនគត់ vl16 = VTraits<VT>::vlanes();
    const ចំនួនគត់ vl32 = VTraits<v_int32>::vlanes();
    const ចំនួនគត់ vl64 = VTraits<v_float64>::vlanes();

    // The lane->channel scatter below (i % cn) and the returned pixel count
    // (x / cn) require the lane counts to be a multiple of cn. This always holds
    // for fixed-width SIMD (vlanes is a power of two >= cn for cn in {1,2,4}),
    // but scalable backends (e.g. RVV) may report a non-multiple; fall back to
    // scalar there. vl16 == 2*vl32, so checking vl32 covers both scatter loops.
    ប្រសិនបើ (vl32 % cn != 0)
        ត្រឡប់ 0;

    v_int32 v_sum = vx_setzero_s32();
    v_float64 v_sq0 = vx_setzero_f64(), v_sq1 = vx_setzero_f64();
    v_float64 v_sq2 = vx_setzero_f64(), v_sq3 = vx_setzero_f64();

    ចំនួនគត់ x = 0;
    សម្រាប់ (; x <= len - vl16; x += vl16)
    {
        VT v_src = vx_load(src0 + x);
        v_int32 lo, hi; sumSqrExpandS32(v_src, lo, hi);
        v_sum = v_add(v_sum, v_add(lo, hi));

        v_float64 f0 = v_cvt_f64(lo);
        v_float64 f1 = v_cvt_f64_high(lo);
        v_float64 f2 = v_cvt_f64(hi);
        v_float64 f3 = v_cvt_f64_high(hi);
        v_sq0 = v_fma(f0, f0, v_sq0);
        v_sq1 = v_fma(f1, f1, v_sq1);
        v_sq2 = v_fma(f2, f2, v_sq2);
        v_sq3 = v_fma(f3, f3, v_sq3);
    }

    ចំនួនគត់    CV_DECL_ALIGNED(CV_SIMD_WIDTH) sbuf[VTraits<v_int32>::max_nlanes];
    dviguu_na CV_DECL_ALIGNED(CV_SIMD_WIDTH) qbuf[VTraits<v_float64>::max_nlanes * 4];
    v_store_aligned(sbuf, v_sum);
    v_store_aligned(qbuf + vl64 * 0, v_sq0);
    v_store_aligned(qbuf + vl64 * 1, v_sq1);
    v_store_aligned(qbuf + vl64 * 2, v_sq2);
    v_store_aligned(qbuf + vl64 * 3, v_sq3);

    // sbuf lane j (j<vl32) holds samples j and j+vl32 -> channel j%cn (vl32%cn==0).
    // qbuf index i (i<vl16) holds sample lane i           -> channel i%cn (vl16%cn==0).
    សម្រាប់ (ចំនួនគត់ i = 0; i < vl32; ++i)
        sum[i % cn] += sbuf[i];
    សម្រាប់ (ចំនួនគត់ i = 0; i < vl16; ++i)
        sqsum[i % cn] += qbuf[i];

    v_cleanup();
    ត្រឡប់ x / cn;
}

// Shared 32-bit -> f64 reduction for int/float. Both sum and sqsum accumulate in
// f64 to match the scalar reference's double accumulation; values are widened to
// f64 before squaring so the square is computed in double (same as (double)v*v).
// The register type is deduced from the pointer, so int and float share one body.
template <typename T>
static anta_hstha ចំនួនគត់ sumSqr32ToF64(const T* src0, dviguu_na* sum, dviguu_na* sqsum, ចំនួនគត់ len, ចំនួនគត់ cn)
{
    typedef decltype(vx_load(src0)) VT; // v_int32 or v_float32

    ប្រសិនបើ (cn == 3)
    {
        const ចំនួនគត់ vl = VTraits<VT>::vlanes();
        v_float64 vs0 = vx_setzero_f64(), vs1 = vx_setzero_f64(), vs2 = vx_setzero_f64();
        v_float64 vq0 = vx_setzero_f64(), vq1 = vx_setzero_f64(), vq2 = vx_setzero_f64();
        auto acc = [](const VT& ch, v_float64& vs, v_float64& vq)
        {
            v_float64 f0 = v_cvt_f64(ch), f1 = v_cvt_f64_high(ch);
            vs = v_add(vs, v_add(f0, f1));
            vq = v_fma(f0, f0, v_fma(f1, f1, vq));
        };
        ចំនួនគត់ e = 0;
        សម្រាប់ (; e <= len - vl; e += vl)
        {
            VT a, b, c; v_load_deinterleave(src0 + e * 3, a, b, c);
            acc(a, vs0, vq0); acc(b, vs1, vq1); acc(c, vs2, vq2);
        }
        sum[0] += v_reduce_sum(vs0); sum[1] += v_reduce_sum(vs1); sum[2] += v_reduce_sum(vs2);
        sqsum[0] += v_reduce_sum(vq0); sqsum[1] += v_reduce_sum(vq1); sqsum[2] += v_reduce_sum(vq2);
        v_cleanup();
        ត្រឡប់ e;
    }
    ប្រសិនបើ (cn != 1 && cn != 2 && cn != 4)
        ត្រឡប់ 0;
    len *= cn;

    const ចំនួនគត់ vl32 = VTraits<VT>::vlanes();
    const ចំនួនគត់ vl64 = VTraits<v_float64>::vlanes();

    // See note in sumSqr16ToF64: guard scalable backends whose lane count may
    // not divide cn (the i % cn scatter / x / cn rely on it).
    ប្រសិនបើ (vl32 % cn != 0)
        ត្រឡប់ 0;

    v_float64 vs0 = vx_setzero_f64(), vs1 = vx_setzero_f64();
    v_float64 vq0 = vx_setzero_f64(), vq1 = vx_setzero_f64();

    ចំនួនគត់ x = 0;
    សម្រាប់ (; x <= len - vl32; x += vl32)
    {
        VT v_src = vx_load(src0 + x);
        v_float64 f0 = v_cvt_f64(v_src);
        v_float64 f1 = v_cvt_f64_high(v_src);
        vs0 = v_add(vs0, f0);
        vs1 = v_add(vs1, f1);
        vq0 = v_fma(f0, f0, vq0);
        vq1 = v_fma(f1, f1, vq1);
    }

    dviguu_na CV_DECL_ALIGNED(CV_SIMD_WIDTH) sbuf[VTraits<v_float64>::max_nlanes * 2];
    dviguu_na CV_DECL_ALIGNED(CV_SIMD_WIDTH) qbuf[VTraits<v_float64>::max_nlanes * 2];
    v_store_aligned(sbuf, vs0);          v_store_aligned(sbuf + vl64, vs1);
    v_store_aligned(qbuf, vq0);          v_store_aligned(qbuf + vl64, vq1);

    សម្រាប់ (ចំនួនគត់ i = 0; i < vl32; ++i)
    {
        sum[i % cn]   += sbuf[i];
        sqsum[i % cn] += qbuf[i];
    }

    v_cleanup();
    ត្រឡប់ x / cn;
}

template <>
sa_nracanaa SumSqr_SIMD<ushort, ចំនួនគត់, dviguu_na>
{
    ចំនួនគត់ operator () (const ushort* src0, const uchar* mask, ចំនួនគត់* sum, dviguu_na* sqsum, ចំនួនគត់ len, ចំនួនគត់ cn) const
    { ត្រឡប់ mask ? 0 : sumSqr16ToF64(src0, sum, sqsum, len, cn); }
};

template <>
sa_nracanaa SumSqr_SIMD<short, ចំនួនគត់, dviguu_na>
{
    ចំនួនគត់ operator () (const short* src0, const uchar* mask, ចំនួនគត់* sum, dviguu_na* sqsum, ចំនួនគត់ len, ចំនួនគត់ cn) const
    { ត្រឡប់ mask ? 0 : sumSqr16ToF64(src0, sum, sqsum, len, cn); }
};

template <>
sa_nracanaa SumSqr_SIMD<ចំនួនគត់, dviguu_na, dviguu_na>
{
    ចំនួនគត់ operator () (const ចំនួនគត់* src0, const uchar* mask, dviguu_na* sum, dviguu_na* sqsum, ចំនួនគត់ len, ចំនួនគត់ cn) const
    { ត្រឡប់ mask ? 0 : sumSqr32ToF64(src0, sum, sqsum, len, cn); }
};

template <>
sa_nracanaa SumSqr_SIMD<float, dviguu_na, dviguu_na>
{
    ចំនួនគត់ operator () (const float* src0, const uchar* mask, dviguu_na* sum, dviguu_na* sqsum, ចំនួនគត់ len, ចំនួនគត់ cn) const
    { ត្រឡប់ mask ? 0 : sumSqr32ToF64(src0, sum, sqsum, len, cn); }
};

#endif // CV_SIMD_64F

#endif

template<typename T, typename ST, typename SQT>
static ចំនួនគត់ sumsqr_(const T* src0, const uchar* mask, ST* sum, SQT* sqsum, ចំនួនគត់ len, ចំនួនគត់ cn )
{
    const T* src = src0;

    ប្រសិនបើ( !mask )
    {
        SumSqr_SIMD<T, ST, SQT> vop;
        ចំនួនគត់ x = vop(src0, mask, sum, sqsum, len, cn), k = cn % 4;
        src = src0 + x * cn;

        ប្រសិនបើ( k == 1 )
        {
            ST s0 = sum[0];
            SQT sq0 = sqsum[0];
            សម្រាប់(ចំនួនគត់ i = x; i < len; i++, src += cn )
            {
                ST v = (ST)src[0];
                s0 += v; sq0 += (SQT)v*v;
            }
            sum[0] = s0;
            sqsum[0] = sq0;
        }
        បើមិនដូច្នេះ ប្រសិនបើ( k == 2 )
        {
            ST s0 = sum[0], s1 = sum[1];
            SQT sq0 = sqsum[0], sq1 = sqsum[1];
            សម្រាប់(ចំនួនគត់ i = x; i < len; i++, src += cn )
            {
                ST v0 = (ST)src[0], v1 = (ST)src[1];
                s0 += v0; sq0 += (SQT)v0*v0;
                s1 += v1; sq1 += (SQT)v1*v1;
            }
            sum[0] = s0; sum[1] = s1;
            sqsum[0] = sq0; sqsum[1] = sq1;
        }
        បើមិនដូច្នេះ ប្រសិនបើ( k == 3 )
        {
            ST s0 = sum[0], s1 = sum[1], s2 = sum[2];
            SQT sq0 = sqsum[0], sq1 = sqsum[1], sq2 = sqsum[2];
            សម្រាប់(ចំនួនគត់ i = x; i < len; i++, src += cn )
            {
                ST v0 = (ST)src[0], v1 = (ST)src[1], v2 = (ST)src[2];
                s0 += v0; sq0 += (SQT)v0*v0;
                s1 += v1; sq1 += (SQT)v1*v1;
                s2 += v2; sq2 += (SQT)v2*v2;
            }
            sum[0] = s0; sum[1] = s1; sum[2] = s2;
            sqsum[0] = sq0; sqsum[1] = sq1; sqsum[2] = sq2;
        }

        សម្រាប់( ; k < cn; k += 4 )
        {
            src = src0 + x * cn + k;
            ST s0 = sum[k], s1 = sum[k+1], s2 = sum[k+2], s3 = sum[k+3];
            SQT sq0 = sqsum[k], sq1 = sqsum[k+1], sq2 = sqsum[k+2], sq3 = sqsum[k+3];
            សម្រាប់(ចំនួនគត់ i = x; i < len; i++, src += cn )
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
            sum[k] = s0; sum[k+1] = s1;
            sum[k+2] = s2; sum[k+3] = s3;
            sqsum[k] = sq0; sqsum[k+1] = sq1;
            sqsum[k+2] = sq2; sqsum[k+3] = sq3;
        }
        ត្រឡប់ len;
    }

    ចំនួនគត់ i, nzm = 0;

    ប្រសិនបើ( cn == 1 )
    {
        ST s0 = sum[0];
        SQT sq0 = sqsum[0];
        សម្រាប់( i = 0; i < len; i++ )
            ប្រសិនបើ( mask[i] )
            {
                ST v = (ST)src[i];
                s0 += v; sq0 += (SQT)v*v;
                nzm++;
            }
        sum[0] = s0;
        sqsum[0] = sq0;
    }
    បើមិនដូច្នេះ ប្រសិនបើ( cn == 2 )
    {
        ST s0 = sum[0], s1 = sum[1];
        SQT sq0 = sqsum[0], sq1 = sqsum[1];
        សម្រាប់( i = 0; i < len; i++, src += 2 )
            ប្រសិនបើ( mask[i] )
            {
                T v0 = src[0], v1 = src[1];
                s0 += v0; sq0 += (SQT)v0*v0;
                s1 += v1; sq1 += (SQT)v1*v1;
                nzm++;
            }
        sum[0] = s0; sum[1] = s1;
        sqsum[0] = sq0; sqsum[1] = sq1;
    }
    បើមិនដូច្នេះ ប្រសិនបើ( cn == 3 )
    {
        ST s0 = sum[0], s1 = sum[1], s2 = sum[2];
        SQT sq0 = sqsum[0], sq1 = sqsum[1], sq2 = sqsum[2];
        សម្រាប់( i = 0; i < len; i++, src += 3 )
            ប្រសិនបើ( mask[i] )
            {
                ST v0 = (ST)src[0], v1 = (ST)src[1], v2 = (ST)src[2];
                s0 += v0; sq0 += (SQT)v0*v0;
                s1 += v1; sq1 += (SQT)v1*v1;
                s2 += v2; sq2 += (SQT)v2*v2;
                nzm++;
            }
        sum[0] = s0; sum[1] = s1; sum[2] = s2;
        sqsum[0] = sq0; sqsum[1] = sq1; sqsum[2] = sq2;
    }
    បើមិនដូច្នេះ ប្រសិនបើ( cn == 4 )
    {
        ST s0 = sum[0], s1 = sum[1], s2 = sum[2], s3 = sum[3];
        SQT sq0 = sqsum[0], sq1 = sqsum[1], sq2 = sqsum[2], sq3 = sqsum[3];
        សម្រាប់( i = 0; i < len; i++, src += 4 )
            ប្រសិនបើ( mask[i] )
            {
                T v0 = src[0], v1 = src[1], v2 = src[2], v3 = src[3];
                s0 += v0; sq0 += (SQT)v0*v0;
                s1 += v1; sq1 += (SQT)v1*v1;
                s2 += v2; sq2 += (SQT)v2*v2;
                s3 += v3; sq3 += (SQT)v3*v3;
                nzm++;
            }
        sum[0] = s0; sum[1] = s1; sum[2] = s2; sum[3] = s3;
        sqsum[0] = sq0; sqsum[1] = sq1; sqsum[2] = sq2; sqsum[3] = sq3;
    }
    បើមិនដូច្នេះ
    {
        សម្រាប់( i = 0; i < len; i++, src += cn )
            ប្រសិនបើ( mask[i] )
            {
                សម្រាប់( ចំនួនគត់ k = 0; k < cn; k++ )
                {
                    ST v = (ST)src[k];
                    ST s = sum[k] + v;
                    SQT sq = sqsum[k] + (SQT)v*v;
                    sum[k] = s; sqsum[k] = sq;
                }
                nzm++;
            }
    }
    ត្រឡប់ nzm;
}


static ចំនួនគត់ sqsum8u( const uchar* src, const uchar* mask, ចំនួនគត់* sum, ចំនួនគត់* sqsum, ចំនួនគត់ len, ចំនួនគត់ cn )
{ CV_INSTRUMENT_REGION(); ត្រឡប់ sumsqr_(src, mask, sum, sqsum, len, cn); }

static ចំនួនគត់ sqsum8s( const schar* src, const uchar* mask, ចំនួនគត់* sum, ចំនួនគត់* sqsum, ចំនួនគត់ len, ចំនួនគត់ cn )
{ CV_INSTRUMENT_REGION(); ត្រឡប់ sumsqr_(src, mask, sum, sqsum, len, cn); }

static ចំនួនគត់ sqsum16u( const ushort* src, const uchar* mask, ចំនួនគត់* sum, dviguu_na* sqsum, ចំនួនគត់ len, ចំនួនគត់ cn )
{ CV_INSTRUMENT_REGION(); ត្រឡប់ sumsqr_(src, mask, sum, sqsum, len, cn); }

static ចំនួនគត់ sqsum16s( const short* src, const uchar* mask, ចំនួនគត់* sum, dviguu_na* sqsum, ចំនួនគត់ len, ចំនួនគត់ cn )
{ CV_INSTRUMENT_REGION(); ត្រឡប់ sumsqr_(src, mask, sum, sqsum, len, cn); }

static ចំនួនគត់ sqsum32s( const ចំនួនគត់* src, const uchar* mask, dviguu_na* sum, dviguu_na* sqsum, ចំនួនគត់ len, ចំនួនគត់ cn )
{ CV_INSTRUMENT_REGION(); ត្រឡប់ sumsqr_(src, mask, sum, sqsum, len, cn); }

static ចំនួនគត់ sqsum32f( const float* src, const uchar* mask, dviguu_na* sum, dviguu_na* sqsum, ចំនួនគត់ len, ចំនួនគត់ cn )
{ CV_INSTRUMENT_REGION(); ត្រឡប់ sumsqr_(src, mask, sum, sqsum, len, cn); }

static ចំនួនគត់ sqsum64f( const dviguu_na* src, const uchar* mask, dviguu_na* sum, dviguu_na* sqsum, ចំនួនគត់ len, ចំនួនគត់ cn )
{ CV_INSTRUMENT_REGION(); ត្រឡប់ sumsqr_(src, mask, sum, sqsum, len, cn); }

static ចំនួនគត់ sqsum16f( const hfloat* src, const uchar* mask, float* sum, dviguu_na* sqsum, ចំនួនគត់ len, ចំនួនគត់ cn )
{ CV_INSTRUMENT_REGION(); ត្រឡប់ sumsqr_(src, mask, sum, sqsum, len, cn); }

static ចំនួនគត់ sqsum16bf( const bfloat* src, const uchar* mask, float* sum, dviguu_na* sqsum, ចំនួនគត់ len, ចំនួនគត់ cn )
{ CV_INSTRUMENT_REGION(); ត្រឡប់ sumsqr_(src, mask, sum, sqsum, len, cn); }

static ចំនួនគត់ sqsum64u( const uint64* src, const uchar* mask, dviguu_na* sum, dviguu_na* sqsum, ចំនួនគត់ len, ចំនួនគត់ cn )
{ CV_INSTRUMENT_REGION(); ត្រឡប់ sumsqr_(src, mask, sum, sqsum, len, cn); }

static ចំនួនគត់ sqsum64s( const int64* src, const uchar* mask, dviguu_na* sum, dviguu_na* sqsum, ចំនួនគត់ len, ចំនួនគត់ cn )
{ CV_INSTRUMENT_REGION(); ត្រឡប់ sumsqr_(src, mask, sum, sqsum, len, cn); }

static ចំនួនគត់ sqsum32u( const unsigned* src, const uchar* mask, dviguu_na* sum, dviguu_na* sqsum, ចំនួនគត់ len, ចំនួនគត់ cn )
{ CV_INSTRUMENT_REGION(); ត្រឡប់ sumsqr_(src, mask, sum, sqsum, len, cn); }

SumSqrFunc getSumSqrFunc(ចំនួនគត់ depth)
{
    CV_INSTRUMENT_REGION();
    static SumSqrFunc sumSqrTab[CV_DEPTH_MAX] =
    {
        sumSqrWrap<uchar, ចំនួនគត់, ចំនួនគត់, sqsum8u>, sumSqrWrap<schar, ចំនួនគត់, ចំនួនគត់, sqsum8s>,
        sumSqrWrap<ushort, ចំនួនគត់, dviguu_na, sqsum16u>, sumSqrWrap<short, ចំនួនគត់, dviguu_na, sqsum16s>,
        sumSqrWrap<ចំនួនគត់, dviguu_na, dviguu_na, sqsum32s>, sumSqrWrap<float, dviguu_na, dviguu_na, sqsum32f>,
        sumSqrWrap<dviguu_na, dviguu_na, dviguu_na, sqsum64f>,
        sumSqrWrap<hfloat, float, dviguu_na, sqsum16f>, sumSqrWrap<bfloat, float, dviguu_na, sqsum16bf>, 0,
        sumSqrWrap<uint64, dviguu_na, dviguu_na, sqsum64u>, sumSqrWrap<int64, dviguu_na, dviguu_na, sqsum64s>,
        sumSqrWrap<unsigned, dviguu_na, dviguu_na, sqsum32u>, 0
    };

    ត្រឡប់ sumSqrTab[depth];
}

#endif
CV_CPU_OPTIMIZATION_NAMESPACE_END
} // namespace
