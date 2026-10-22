// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html

// Copyright (C) 2026, Advanced Micro Devices, Inc., all rights reserved.

#অন্তর্ভাব "precomp.hpp"
#অন্তর্ভাব "stat.hpp"

নামাকাশ cv {
প্রকারনির্দেশ পূর্ণাঙ্ক (*SumSqrFunc)(ধ্রুব uchar*, ধ্রুব uchar* mask, uchar*, uchar*, পূর্ণাঙ্ক, পূর্ণাঙ্ক);

CV_CPU_OPTIMIZATION_NAMESPACE_BEGIN

template<typename T, typename ST, typename SQT, পূর্ণাঙ্ক (*fn)(ধ্রুব T*, ধ্রুব uchar*, ST*, SQT*, পূর্ণাঙ্ক, পূর্ণাঙ্ক)>
স্থির পূর্ণাঙ্ক sumSqrWrap(ধ্রুব uchar* src, ধ্রুব uchar* mask, uchar* sum, uchar* sqsum, পূর্ণাঙ্ক len, পূর্ণাঙ্ক cn)
{
    প্রত্যাগম fn((ধ্রুব T*)src, mask, (ST*)sum, (SQT*)sqsum, len, cn);
}

SumSqrFunc getSumSqrFunc(পূর্ণাঙ্ক depth);

#ifndef CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

template <typename T, typename ST, typename SQT>
সংরচনা SumSqr_SIMD
{
    অন্তঃস্থ পূর্ণাঙ্ক operator () (ধ্রুব T *, ধ্রুব uchar *, ST *, SQT *, পূর্ণাঙ্ক, পূর্ণাঙ্ক) ধ্রুব
    {
        প্রত্যাগম 0;
    }
};

#যদি (CV_SIMD || CV_SIMD_SCALABLE)

template <>
সংরচনা SumSqr_SIMD<uchar, পূর্ণাঙ্ক, পূর্ণাঙ্ক>
{
    পূর্ণাঙ্ক operator () (ধ্রুব uchar * src0, ধ্রুব uchar * mask, পূর্ণাঙ্ক * sum, পূর্ণাঙ্ক * sqsum, পূর্ণাঙ্ক len, পূর্ণাঙ্ক cn) ধ্রুব
    {
        যদি (mask)
            প্রত্যাগম 0;
        // cn==3: deinterleave so each lane belongs to one channel, then reduce per
        // channel. sum/sqsum stay exact in s32 within the dispatcher's 1<<15 block.
        যদি (cn == 3)
        {
            ধ্রুব পূর্ণাঙ্ক vl8 = VTraits<v_uint8>::vlanes();
            v_int32 vs0 = vx_setzero_s32(), vs1 = vx_setzero_s32(), vs2 = vx_setzero_s32();
            v_int32 vq0 = vx_setzero_s32(), vq1 = vx_setzero_s32(), vq2 = vx_setzero_s32();
            auto acc = [](ধ্রুব v_uint8& ch, v_int32& vs, v_int32& vq)
            {
                v_uint16 lo, hi; v_expand(ch, lo, hi);
                v_uint32 s0, s1, s2, s3; v_expand(lo, s0, s1); v_expand(hi, s2, s3);
                vs = v_add(vs, v_reinterpret_as_s32(v_add(v_add(s0, s1), v_add(s2, s3))));
                v_int16 d0 = v_reinterpret_as_s16(lo), d1 = v_reinterpret_as_s16(hi);
                vq = v_add(vq, v_add(v_dotprod(d0, d0), v_dotprod(d1, d1)));
            };
            পূর্ণাঙ্ক e = 0;
            প্রতি (; e <= len - vl8; e += vl8)
            {
                v_uint8 a, b, c; v_load_deinterleave(src0 + e * 3, a, b, c);
                acc(a, vs0, vq0); acc(b, vs1, vq1); acc(c, vs2, vq2);
            }
            sum[0] += v_reduce_sum(vs0); sum[1] += v_reduce_sum(vs1); sum[2] += v_reduce_sum(vs2);
            sqsum[0] += v_reduce_sum(vq0); sqsum[1] += v_reduce_sum(vq1); sqsum[2] += v_reduce_sum(vq2);
            v_cleanup();
            প্রত্যাগম e;
        }
        যদি (cn != 1 && cn != 2 && cn != 4)
            প্রত্যাগম 0;
        len *= cn;

        পূর্ণাঙ্ক x = 0;
        v_int32 v_sum = vx_setzero_s32();
        v_int32 v_sqsum = vx_setzero_s32();

        ধ্রুব পূর্ণাঙ্ক len0 = len & -VTraits<v_uint8>::vlanes();
        যাবত্(x < len0)
        {
            ধ্রুব পূর্ণাঙ্ক len_tmp = min(x + 256*VTraits<v_uint16>::vlanes(), len0);
            v_uint16 v_sum16 = vx_setzero_u16();
            প্রতি ( ; x < len_tmp; x += VTraits<v_uint8>::vlanes())
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
        যদি (x <= len - VTraits<v_uint16>::vlanes())
        {
            v_uint16 v_src = vx_load_expand(src0 + x);
            v_uint16 v_half = v_combine_উপরের(v_src, v_src);

            v_uint32 v_tmp0, v_tmp1;
            v_expand(v_add(v_src, v_half), v_tmp0, v_tmp1);
            v_sum = v_add(v_sum, v_reinterpret_as_s32(v_tmp0));

            v_int16 v_tmp2, v_tmp3;
            v_zip(v_reinterpret_as_s16(v_src), v_reinterpret_as_s16(v_half), v_tmp2, v_tmp3);
            v_sqsum = v_add(v_sqsum, v_dotprod(v_tmp2, v_tmp2));
            x += VTraits<v_uint16>::vlanes();
        }

        যদি (cn == 1)
        {
            *sum += v_reduce_sum(v_sum);
            *sqsum += v_reduce_sum(v_sqsum);
        }
        অন্যথা
        {
            পূর্ণাঙ্ক CV_DECL_ALIGNED(CV_SIMD_WIDTH) ar[2 * VTraits<v_int32>::max_nlanes];
            v_store(ar, v_sum);
            v_store(ar + VTraits<v_int32>::vlanes(), v_sqsum);
            প্রতি (পূর্ণাঙ্ক i = 0; i < VTraits<v_int32>::vlanes(); ++i)
            {
                sum[i % cn] += ar[i];
                sqsum[i % cn] += ar[VTraits<v_int32>::vlanes() + i];
            }
        }
        v_cleanup();
        প্রত্যাগম x / cn;
    }
};

template <>
সংরচনা SumSqr_SIMD<schar, পূর্ণাঙ্ক, পূর্ণাঙ্ক>
{
    পূর্ণাঙ্ক operator () (ধ্রুব schar * src0, ধ্রুব uchar * mask, পূর্ণাঙ্ক * sum, পূর্ণাঙ্ক * sqsum, পূর্ণাঙ্ক len, পূর্ণাঙ্ক cn) ধ্রুব
    {
        যদি (mask)
            প্রত্যাগম 0;
        যদি (cn == 3)
        {
            ধ্রুব পূর্ণাঙ্ক vl8 = VTraits<v_int8>::vlanes();
            v_int32 vs0 = vx_setzero_s32(), vs1 = vx_setzero_s32(), vs2 = vx_setzero_s32();
            v_int32 vq0 = vx_setzero_s32(), vq1 = vx_setzero_s32(), vq2 = vx_setzero_s32();
            auto acc = [](ধ্রুব v_int8& ch, v_int32& vs, v_int32& vq)
            {
                v_int16 lo, hi; v_expand(ch, lo, hi);
                v_int32 s0, s1, s2, s3; v_expand(lo, s0, s1); v_expand(hi, s2, s3);
                vs = v_add(vs, v_add(v_add(s0, s1), v_add(s2, s3)));
                vq = v_add(vq, v_add(v_dotprod(lo, lo), v_dotprod(hi, hi)));
            };
            পূর্ণাঙ্ক e = 0;
            প্রতি (; e <= len - vl8; e += vl8)
            {
                v_int8 a, b, c; v_load_deinterleave(src0 + e * 3, a, b, c);
                acc(a, vs0, vq0); acc(b, vs1, vq1); acc(c, vs2, vq2);
            }
            sum[0] += v_reduce_sum(vs0); sum[1] += v_reduce_sum(vs1); sum[2] += v_reduce_sum(vs2);
            sqsum[0] += v_reduce_sum(vq0); sqsum[1] += v_reduce_sum(vq1); sqsum[2] += v_reduce_sum(vq2);
            v_cleanup();
            প্রত্যাগম e;
        }
        যদি (cn != 1 && cn != 2 && cn != 4)
            প্রত্যাগম 0;
        len *= cn;

        পূর্ণাঙ্ক x = 0;
        v_int32 v_sum = vx_setzero_s32();
        v_int32 v_sqsum = vx_setzero_s32();

        ধ্রুব পূর্ণাঙ্ক len0 = len & -VTraits<v_int8>::vlanes();
        যাবত্ (x < len0)
        {
            ধ্রুব পূর্ণাঙ্ক len_tmp = min(x + 256 * VTraits<v_int16>::vlanes(), len0);
            v_int16 v_sum16 = vx_setzero_s16();
            প্রতি (; x < len_tmp; x += VTraits<v_int8>::vlanes())
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
        যদি (x <= len - VTraits<v_int16>::vlanes())
        {
            v_int16 v_src = vx_load_expand(src0 + x);
            v_int16 v_half = v_combine_উপরের(v_src, v_src);

            v_int32 v_tmp0, v_tmp1;
            v_expand(v_add(v_src, v_half), v_tmp0, v_tmp1);
            v_sum = v_add(v_sum, v_tmp0);

            v_int16 v_tmp2, v_tmp3;
            v_zip(v_src, v_half, v_tmp2, v_tmp3);
            v_sqsum = v_add(v_sqsum, v_dotprod(v_tmp2, v_tmp2));
            x += VTraits<v_int16>::vlanes();
        }

        যদি (cn == 1)
        {
            *sum += v_reduce_sum(v_sum);
            *sqsum += v_reduce_sum(v_sqsum);
        }
        অন্যথা
        {
            পূর্ণাঙ্ক CV_DECL_ALIGNED(CV_SIMD_WIDTH) ar[2 * VTraits<v_int32>::max_nlanes];
            v_store(ar, v_sum);
            v_store(ar + VTraits<v_int32>::vlanes(), v_sqsum);
            প্রতি (পূর্ণাঙ্ক i = 0; i < VTraits<v_int32>::vlanes(); ++i)
            {
                sum[i % cn] += ar[i];
                sqsum[i % cn] += ar[VTraits<v_int32>::vlanes() + i];
            }
        }
        v_cleanup();
        প্রত্যাগম x / cn;
    }
};

#যদি (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)

// Expand a 16-bit vector into two s32 halves. The unsigned overload reinterprets
// after a widening expand (values 0..65535 stay non-negative in s32, so the
// subsequent v_cvt_f64 is exact); the signed overload expands directly. This is
// the only real difference between the ushort and short reductions below.
স্থির অন্তঃস্থ শূন্য sumSqrExpandS32(ধ্রুব v_uint16& v, v_int32& lo, v_int32& hi)
{
    v_uint32 a, b; v_expand(v, a, b);
    lo = v_reinterpret_as_s32(a); hi = v_reinterpret_as_s32(b);
}
স্থির অন্তঃস্থ শূন্য sumSqrExpandS32(ধ্রুব v_int16& v, v_int32& lo, v_int32& hi)
{
    v_expand(v, lo, hi);
}

// Shared 16-bit -> {s32 sum, f64 sqsum} reduction for ushort/short. The register
// type is deduced from the pointer, so the body is identical for both depths.
template <typename T>
স্থির অন্তঃস্থ পূর্ণাঙ্ক sumSqr16ToF64(ধ্রুব T* src0, পূর্ণাঙ্ক* sum, দ্বিগুণ* sqsum, পূর্ণাঙ্ক len, পূর্ণাঙ্ক cn)
{
    প্রকারনির্দেশ decltype(vx_load(src0)) VT; // v_uint16 or v_int16

    যদি (cn == 3)
    {
        ধ্রুব পূর্ণাঙ্ক vl = VTraits<VT>::vlanes();
        v_int32 vs0 = vx_setzero_s32(), vs1 = vx_setzero_s32(), vs2 = vx_setzero_s32();
        v_float64 vq0 = vx_setzero_f64(), vq1 = vx_setzero_f64(), vq2 = vx_setzero_f64();
        auto acc = [](ধ্রুব VT& ch, v_int32& vs, v_float64& vq)
        {
            v_int32 lo, hi; sumSqrExpandS32(ch, lo, hi);
            vs = v_add(vs, v_add(lo, hi));
            v_float64 f0 = v_cvt_f64(lo), f1 = v_cvt_f64_উপরের(lo);
            v_float64 f2 = v_cvt_f64(hi), f3 = v_cvt_f64_উপরের(hi);
            vq = v_fma(f0, f0, v_fma(f1, f1, v_fma(f2, f2, v_fma(f3, f3, vq))));
        };
        পূর্ণাঙ্ক e = 0;
        প্রতি (; e <= len - vl; e += vl)
        {
            VT a, b, c; v_load_deinterleave(src0 + e * 3, a, b, c);
            acc(a, vs0, vq0); acc(b, vs1, vq1); acc(c, vs2, vq2);
        }
        sum[0] += v_reduce_sum(vs0); sum[1] += v_reduce_sum(vs1); sum[2] += v_reduce_sum(vs2);
        sqsum[0] += v_reduce_sum(vq0); sqsum[1] += v_reduce_sum(vq1); sqsum[2] += v_reduce_sum(vq2);
        v_cleanup();
        প্রত্যাগম e;
    }
    যদি (cn != 1 && cn != 2 && cn != 4)
        প্রত্যাগম 0;
    len *= cn;

    ধ্রুব পূর্ণাঙ্ক vl16 = VTraits<VT>::vlanes();
    ধ্রুব পূর্ণাঙ্ক vl32 = VTraits<v_int32>::vlanes();
    ধ্রুব পূর্ণাঙ্ক vl64 = VTraits<v_float64>::vlanes();

    // The lane->channel scatter below (i % cn) and the returned pixel count
    // (x / cn) require the lane counts to be a multiple of cn. This always holds
    // for fixed-width SIMD (vlanes is a power of two >= cn for cn in {1,2,4}),
    // but scalable backends (e.g. RVV) may report a non-multiple; fall back to
    // scalar there. vl16 == 2*vl32, so checking vl32 covers both scatter loops.
    যদি (vl32 % cn != 0)
        প্রত্যাগম 0;

    v_int32 v_sum = vx_setzero_s32();
    v_float64 v_sq0 = vx_setzero_f64(), v_sq1 = vx_setzero_f64();
    v_float64 v_sq2 = vx_setzero_f64(), v_sq3 = vx_setzero_f64();

    পূর্ণাঙ্ক x = 0;
    প্রতি (; x <= len - vl16; x += vl16)
    {
        VT v_src = vx_load(src0 + x);
        v_int32 lo, hi; sumSqrExpandS32(v_src, lo, hi);
        v_sum = v_add(v_sum, v_add(lo, hi));

        v_float64 f0 = v_cvt_f64(lo);
        v_float64 f1 = v_cvt_f64_উপরের(lo);
        v_float64 f2 = v_cvt_f64(hi);
        v_float64 f3 = v_cvt_f64_উপরের(hi);
        v_sq0 = v_fma(f0, f0, v_sq0);
        v_sq1 = v_fma(f1, f1, v_sq1);
        v_sq2 = v_fma(f2, f2, v_sq2);
        v_sq3 = v_fma(f3, f3, v_sq3);
    }

    পূর্ণাঙ্ক    CV_DECL_ALIGNED(CV_SIMD_WIDTH) sbuf[VTraits<v_int32>::max_nlanes];
    দ্বিগুণ CV_DECL_ALIGNED(CV_SIMD_WIDTH) qbuf[VTraits<v_float64>::max_nlanes * 4];
    v_store_aligned(sbuf, v_sum);
    v_store_aligned(qbuf + vl64 * 0, v_sq0);
    v_store_aligned(qbuf + vl64 * 1, v_sq1);
    v_store_aligned(qbuf + vl64 * 2, v_sq2);
    v_store_aligned(qbuf + vl64 * 3, v_sq3);

    // sbuf lane j (j<vl32) holds samples j and j+vl32 -> channel j%cn (vl32%cn==0).
    // qbuf index i (i<vl16) holds sample lane i           -> channel i%cn (vl16%cn==0).
    প্রতি (পূর্ণাঙ্ক i = 0; i < vl32; ++i)
        sum[i % cn] += sbuf[i];
    প্রতি (পূর্ণাঙ্ক i = 0; i < vl16; ++i)
        sqsum[i % cn] += qbuf[i];

    v_cleanup();
    প্রত্যাগম x / cn;
}

// Shared 32-bit -> f64 reduction for int/float. Both sum and sqsum accumulate in
// f64 to match the scalar reference's double accumulation; values are widened to
// f64 before squaring so the square is computed in double (same as (double)v*v).
// The register type is deduced from the pointer, so int and float share one body.
template <typename T>
স্থির অন্তঃস্থ পূর্ণাঙ্ক sumSqr32ToF64(ধ্রুব T* src0, দ্বিগুণ* sum, দ্বিগুণ* sqsum, পূর্ণাঙ্ক len, পূর্ণাঙ্ক cn)
{
    প্রকারনির্দেশ decltype(vx_load(src0)) VT; // v_int32 or v_float32

    যদি (cn == 3)
    {
        ধ্রুব পূর্ণাঙ্ক vl = VTraits<VT>::vlanes();
        v_float64 vs0 = vx_setzero_f64(), vs1 = vx_setzero_f64(), vs2 = vx_setzero_f64();
        v_float64 vq0 = vx_setzero_f64(), vq1 = vx_setzero_f64(), vq2 = vx_setzero_f64();
        auto acc = [](ধ্রুব VT& ch, v_float64& vs, v_float64& vq)
        {
            v_float64 f0 = v_cvt_f64(ch), f1 = v_cvt_f64_উপরের(ch);
            vs = v_add(vs, v_add(f0, f1));
            vq = v_fma(f0, f0, v_fma(f1, f1, vq));
        };
        পূর্ণাঙ্ক e = 0;
        প্রতি (; e <= len - vl; e += vl)
        {
            VT a, b, c; v_load_deinterleave(src0 + e * 3, a, b, c);
            acc(a, vs0, vq0); acc(b, vs1, vq1); acc(c, vs2, vq2);
        }
        sum[0] += v_reduce_sum(vs0); sum[1] += v_reduce_sum(vs1); sum[2] += v_reduce_sum(vs2);
        sqsum[0] += v_reduce_sum(vq0); sqsum[1] += v_reduce_sum(vq1); sqsum[2] += v_reduce_sum(vq2);
        v_cleanup();
        প্রত্যাগম e;
    }
    যদি (cn != 1 && cn != 2 && cn != 4)
        প্রত্যাগম 0;
    len *= cn;

    ধ্রুব পূর্ণাঙ্ক vl32 = VTraits<VT>::vlanes();
    ধ্রুব পূর্ণাঙ্ক vl64 = VTraits<v_float64>::vlanes();

    // See note in sumSqr16ToF64: guard scalable backends whose lane count may
    // not divide cn (the i % cn scatter / x / cn rely on it).
    যদি (vl32 % cn != 0)
        প্রত্যাগম 0;

    v_float64 vs0 = vx_setzero_f64(), vs1 = vx_setzero_f64();
    v_float64 vq0 = vx_setzero_f64(), vq1 = vx_setzero_f64();

    পূর্ণাঙ্ক x = 0;
    প্রতি (; x <= len - vl32; x += vl32)
    {
        VT v_src = vx_load(src0 + x);
        v_float64 f0 = v_cvt_f64(v_src);
        v_float64 f1 = v_cvt_f64_উপরের(v_src);
        vs0 = v_add(vs0, f0);
        vs1 = v_add(vs1, f1);
        vq0 = v_fma(f0, f0, vq0);
        vq1 = v_fma(f1, f1, vq1);
    }

    দ্বিগুণ CV_DECL_ALIGNED(CV_SIMD_WIDTH) sbuf[VTraits<v_float64>::max_nlanes * 2];
    দ্বিগুণ CV_DECL_ALIGNED(CV_SIMD_WIDTH) qbuf[VTraits<v_float64>::max_nlanes * 2];
    v_store_aligned(sbuf, vs0);          v_store_aligned(sbuf + vl64, vs1);
    v_store_aligned(qbuf, vq0);          v_store_aligned(qbuf + vl64, vq1);

    প্রতি (পূর্ণাঙ্ক i = 0; i < vl32; ++i)
    {
        sum[i % cn]   += sbuf[i];
        sqsum[i % cn] += qbuf[i];
    }

    v_cleanup();
    প্রত্যাগম x / cn;
}

template <>
সংরচনা SumSqr_SIMD<ushort, পূর্ণাঙ্ক, দ্বিগুণ>
{
    পূর্ণাঙ্ক operator () (ধ্রুব ushort* src0, ধ্রুব uchar* mask, পূর্ণাঙ্ক* sum, দ্বিগুণ* sqsum, পূর্ণাঙ্ক len, পূর্ণাঙ্ক cn) ধ্রুব
    { প্রত্যাগম mask ? 0 : sumSqr16ToF64(src0, sum, sqsum, len, cn); }
};

template <>
সংরচনা SumSqr_SIMD<short, পূর্ণাঙ্ক, দ্বিগুণ>
{
    পূর্ণাঙ্ক operator () (ধ্রুব short* src0, ধ্রুব uchar* mask, পূর্ণাঙ্ক* sum, দ্বিগুণ* sqsum, পূর্ণাঙ্ক len, পূর্ণাঙ্ক cn) ধ্রুব
    { প্রত্যাগম mask ? 0 : sumSqr16ToF64(src0, sum, sqsum, len, cn); }
};

template <>
সংরচনা SumSqr_SIMD<পূর্ণাঙ্ক, দ্বিগুণ, দ্বিগুণ>
{
    পূর্ণাঙ্ক operator () (ধ্রুব পূর্ণাঙ্ক* src0, ধ্রুব uchar* mask, দ্বিগুণ* sum, দ্বিগুণ* sqsum, পূর্ণাঙ্ক len, পূর্ণাঙ্ক cn) ধ্রুব
    { প্রত্যাগম mask ? 0 : sumSqr32ToF64(src0, sum, sqsum, len, cn); }
};

template <>
সংরচনা SumSqr_SIMD<ভিন্ন, দ্বিগুণ, দ্বিগুণ>
{
    পূর্ণাঙ্ক operator () (ধ্রুব ভিন্ন* src0, ধ্রুব uchar* mask, দ্বিগুণ* sum, দ্বিগুণ* sqsum, পূর্ণাঙ্ক len, পূর্ণাঙ্ক cn) ধ্রুব
    { প্রত্যাগম mask ? 0 : sumSqr32ToF64(src0, sum, sqsum, len, cn); }
};

#endif // CV_SIMD_64F

#endif

template<typename T, typename ST, typename SQT>
স্থির পূর্ণাঙ্ক sumsqr_(ধ্রুব T* src0, ধ্রুব uchar* mask, ST* sum, SQT* sqsum, পূর্ণাঙ্ক len, পূর্ণাঙ্ক cn )
{
    ধ্রুব T* src = src0;

    যদি( !mask )
    {
        SumSqr_SIMD<T, ST, SQT> vop;
        পূর্ণাঙ্ক x = vop(src0, mask, sum, sqsum, len, cn), k = cn % 4;
        src = src0 + x * cn;

        যদি( k == 1 )
        {
            ST s0 = sum[0];
            SQT sq0 = sqsum[0];
            প্রতি(পূর্ণাঙ্ক i = x; i < len; i++, src += cn )
            {
                ST v = (ST)src[0];
                s0 += v; sq0 += (SQT)v*v;
            }
            sum[0] = s0;
            sqsum[0] = sq0;
        }
        অন্যথা যদি( k == 2 )
        {
            ST s0 = sum[0], s1 = sum[1];
            SQT sq0 = sqsum[0], sq1 = sqsum[1];
            প্রতি(পূর্ণাঙ্ক i = x; i < len; i++, src += cn )
            {
                ST v0 = (ST)src[0], v1 = (ST)src[1];
                s0 += v0; sq0 += (SQT)v0*v0;
                s1 += v1; sq1 += (SQT)v1*v1;
            }
            sum[0] = s0; sum[1] = s1;
            sqsum[0] = sq0; sqsum[1] = sq1;
        }
        অন্যথা যদি( k == 3 )
        {
            ST s0 = sum[0], s1 = sum[1], s2 = sum[2];
            SQT sq0 = sqsum[0], sq1 = sqsum[1], sq2 = sqsum[2];
            প্রতি(পূর্ণাঙ্ক i = x; i < len; i++, src += cn )
            {
                ST v0 = (ST)src[0], v1 = (ST)src[1], v2 = (ST)src[2];
                s0 += v0; sq0 += (SQT)v0*v0;
                s1 += v1; sq1 += (SQT)v1*v1;
                s2 += v2; sq2 += (SQT)v2*v2;
            }
            sum[0] = s0; sum[1] = s1; sum[2] = s2;
            sqsum[0] = sq0; sqsum[1] = sq1; sqsum[2] = sq2;
        }

        প্রতি( ; k < cn; k += 4 )
        {
            src = src0 + x * cn + k;
            ST s0 = sum[k], s1 = sum[k+1], s2 = sum[k+2], s3 = sum[k+3];
            SQT sq0 = sqsum[k], sq1 = sqsum[k+1], sq2 = sqsum[k+2], sq3 = sqsum[k+3];
            প্রতি(পূর্ণাঙ্ক i = x; i < len; i++, src += cn )
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
        প্রত্যাগম len;
    }

    পূর্ণাঙ্ক i, nzm = 0;

    যদি( cn == 1 )
    {
        ST s0 = sum[0];
        SQT sq0 = sqsum[0];
        প্রতি( i = 0; i < len; i++ )
            যদি( mask[i] )
            {
                ST v = (ST)src[i];
                s0 += v; sq0 += (SQT)v*v;
                nzm++;
            }
        sum[0] = s0;
        sqsum[0] = sq0;
    }
    অন্যথা যদি( cn == 2 )
    {
        ST s0 = sum[0], s1 = sum[1];
        SQT sq0 = sqsum[0], sq1 = sqsum[1];
        প্রতি( i = 0; i < len; i++, src += 2 )
            যদি( mask[i] )
            {
                T v0 = src[0], v1 = src[1];
                s0 += v0; sq0 += (SQT)v0*v0;
                s1 += v1; sq1 += (SQT)v1*v1;
                nzm++;
            }
        sum[0] = s0; sum[1] = s1;
        sqsum[0] = sq0; sqsum[1] = sq1;
    }
    অন্যথা যদি( cn == 3 )
    {
        ST s0 = sum[0], s1 = sum[1], s2 = sum[2];
        SQT sq0 = sqsum[0], sq1 = sqsum[1], sq2 = sqsum[2];
        প্রতি( i = 0; i < len; i++, src += 3 )
            যদি( mask[i] )
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
    অন্যথা যদি( cn == 4 )
    {
        ST s0 = sum[0], s1 = sum[1], s2 = sum[2], s3 = sum[3];
        SQT sq0 = sqsum[0], sq1 = sqsum[1], sq2 = sqsum[2], sq3 = sqsum[3];
        প্রতি( i = 0; i < len; i++, src += 4 )
            যদি( mask[i] )
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
    অন্যথা
    {
        প্রতি( i = 0; i < len; i++, src += cn )
            যদি( mask[i] )
            {
                প্রতি( পূর্ণাঙ্ক k = 0; k < cn; k++ )
                {
                    ST v = (ST)src[k];
                    ST s = sum[k] + v;
                    SQT sq = sqsum[k] + (SQT)v*v;
                    sum[k] = s; sqsum[k] = sq;
                }
                nzm++;
            }
    }
    প্রত্যাগম nzm;
}


স্থির পূর্ণাঙ্ক sqsum8u( ধ্রুব uchar* src, ধ্রুব uchar* mask, পূর্ণাঙ্ক* sum, পূর্ণাঙ্ক* sqsum, পূর্ণাঙ্ক len, পূর্ণাঙ্ক cn )
{ CV_INSTRUMENT_REGION(); প্রত্যাগম sumsqr_(src, mask, sum, sqsum, len, cn); }

স্থির পূর্ণাঙ্ক sqsum8s( ধ্রুব schar* src, ধ্রুব uchar* mask, পূর্ণাঙ্ক* sum, পূর্ণাঙ্ক* sqsum, পূর্ণাঙ্ক len, পূর্ণাঙ্ক cn )
{ CV_INSTRUMENT_REGION(); প্রত্যাগম sumsqr_(src, mask, sum, sqsum, len, cn); }

স্থির পূর্ণাঙ্ক sqsum16u( ধ্রুব ushort* src, ধ্রুব uchar* mask, পূর্ণাঙ্ক* sum, দ্বিগুণ* sqsum, পূর্ণাঙ্ক len, পূর্ণাঙ্ক cn )
{ CV_INSTRUMENT_REGION(); প্রত্যাগম sumsqr_(src, mask, sum, sqsum, len, cn); }

স্থির পূর্ণাঙ্ক sqsum16s( ধ্রুব short* src, ধ্রুব uchar* mask, পূর্ণাঙ্ক* sum, দ্বিগুণ* sqsum, পূর্ণাঙ্ক len, পূর্ণাঙ্ক cn )
{ CV_INSTRUMENT_REGION(); প্রত্যাগম sumsqr_(src, mask, sum, sqsum, len, cn); }

স্থির পূর্ণাঙ্ক sqsum32s( ধ্রুব পূর্ণাঙ্ক* src, ধ্রুব uchar* mask, দ্বিগুণ* sum, দ্বিগুণ* sqsum, পূর্ণাঙ্ক len, পূর্ণাঙ্ক cn )
{ CV_INSTRUMENT_REGION(); প্রত্যাগম sumsqr_(src, mask, sum, sqsum, len, cn); }

স্থির পূর্ণাঙ্ক sqsum32f( ধ্রুব ভিন্ন* src, ধ্রুব uchar* mask, দ্বিগুণ* sum, দ্বিগুণ* sqsum, পূর্ণাঙ্ক len, পূর্ণাঙ্ক cn )
{ CV_INSTRUMENT_REGION(); প্রত্যাগম sumsqr_(src, mask, sum, sqsum, len, cn); }

স্থির পূর্ণাঙ্ক sqsum64f( ধ্রুব দ্বিগুণ* src, ধ্রুব uchar* mask, দ্বিগুণ* sum, দ্বিগুণ* sqsum, পূর্ণাঙ্ক len, পূর্ণাঙ্ক cn )
{ CV_INSTRUMENT_REGION(); প্রত্যাগম sumsqr_(src, mask, sum, sqsum, len, cn); }

স্থির পূর্ণাঙ্ক sqsum16f( ধ্রুব hfloat* src, ধ্রুব uchar* mask, ভিন্ন* sum, দ্বিগুণ* sqsum, পূর্ণাঙ্ক len, পূর্ণাঙ্ক cn )
{ CV_INSTRUMENT_REGION(); প্রত্যাগম sumsqr_(src, mask, sum, sqsum, len, cn); }

স্থির পূর্ণাঙ্ক sqsum16bf( ধ্রুব bfloat* src, ধ্রুব uchar* mask, ভিন্ন* sum, দ্বিগুণ* sqsum, পূর্ণাঙ্ক len, পূর্ণাঙ্ক cn )
{ CV_INSTRUMENT_REGION(); প্রত্যাগম sumsqr_(src, mask, sum, sqsum, len, cn); }

স্থির পূর্ণাঙ্ক sqsum64u( ধ্রুব uint64* src, ধ্রুব uchar* mask, দ্বিগুণ* sum, দ্বিগুণ* sqsum, পূর্ণাঙ্ক len, পূর্ণাঙ্ক cn )
{ CV_INSTRUMENT_REGION(); প্রত্যাগম sumsqr_(src, mask, sum, sqsum, len, cn); }

স্থির পূর্ণাঙ্ক sqsum64s( ধ্রুব int64* src, ধ্রুব uchar* mask, দ্বিগুণ* sum, দ্বিগুণ* sqsum, পূর্ণাঙ্ক len, পূর্ণাঙ্ক cn )
{ CV_INSTRUMENT_REGION(); প্রত্যাগম sumsqr_(src, mask, sum, sqsum, len, cn); }

স্থির পূর্ণাঙ্ক sqsum32u( ধ্রুব unsigned* src, ধ্রুব uchar* mask, দ্বিগুণ* sum, দ্বিগুণ* sqsum, পূর্ণাঙ্ক len, পূর্ণাঙ্ক cn )
{ CV_INSTRUMENT_REGION(); প্রত্যাগম sumsqr_(src, mask, sum, sqsum, len, cn); }

SumSqrFunc getSumSqrFunc(পূর্ণাঙ্ক depth)
{
    CV_INSTRUMENT_REGION();
    স্থির SumSqrFunc sumSqrTab[CV_DEPTH_MAX] =
    {
        sumSqrWrap<uchar, পূর্ণাঙ্ক, পূর্ণাঙ্ক, sqsum8u>, sumSqrWrap<schar, পূর্ণাঙ্ক, পূর্ণাঙ্ক, sqsum8s>,
        sumSqrWrap<ushort, পূর্ণাঙ্ক, দ্বিগুণ, sqsum16u>, sumSqrWrap<short, পূর্ণাঙ্ক, দ্বিগুণ, sqsum16s>,
        sumSqrWrap<পূর্ণাঙ্ক, দ্বিগুণ, দ্বিগুণ, sqsum32s>, sumSqrWrap<ভিন্ন, দ্বিগুণ, দ্বিগুণ, sqsum32f>,
        sumSqrWrap<দ্বিগুণ, দ্বিগুণ, দ্বিগুণ, sqsum64f>,
        sumSqrWrap<hfloat, ভিন্ন, দ্বিগুণ, sqsum16f>, sumSqrWrap<bfloat, ভিন্ন, দ্বিগুণ, sqsum16bf>, 0,
        sumSqrWrap<uint64, দ্বিগুণ, দ্বিগুণ, sqsum64u>, sumSqrWrap<int64, দ্বিগুণ, দ্বিগুণ, sqsum64s>,
        sumSqrWrap<unsigned, দ্বিগুণ, দ্বিগুণ, sqsum32u>, 0
    };

    প্রত্যাগম sumSqrTab[depth];
}

#endif
CV_CPU_OPTIMIZATION_NAMESPACE_END
} // namespace
