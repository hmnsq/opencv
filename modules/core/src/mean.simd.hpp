// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html

// Copyright (C) 2026, Advanced Micro Devices, Inc., all rights reserved.

#అన్తర్భావ "precomp.hpp"
#అన్తర్భావ "stat.hpp"

నామాకాశ cv {
రకనిర్వచనం పూర్ణాంకం (*కూడిక_Sqr_Func)(స్థిరరాశి uchar*, స్థిరరాశి uchar* ముసుగు, uchar*, uchar*, పూర్ణాంకం, పూర్ణాంకం);

CV_CPU_OPTIMIZATION_NAMESPACE_BEGIN

మూస<రకం_పేరు T, రకం_పేరు ST, రకం_పేరు SQT, పూర్ణాంకం (*fn)(స్థిరరాశి T*, స్థిరరాశి uchar*, ST*, SQT*, పూర్ణాంకం, పూర్ణాంకం)>
స్థిర పూర్ణాంకం కూడిక_Sqr_Wrap(స్థిరరాశి uchar* src, స్థిరరాశి uchar* ముసుగు, uchar* కూడిక, uchar* sqsum, పూర్ణాంకం len, పూర్ణాంకం cn)
{
    తిరిగి fn((స్థిరరాశి T*)src, ముసుగు, (ST*)కూడిక, (SQT*)sqsum, len, cn);
}

కూడిక_Sqr_Func get_కూడిక_Sqr_Func(పూర్ణాంకం లోతు);

#నిర్వచితం_కాకపోతే CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

మూస <రకం_పేరు T, రకం_పేరు ST, రకం_పేరు SQT>
నిర్మాణం కూడిక_Sqr_SIMD
{
    అంతఃవరుస పూర్ణాంకం క్రియాకారకం () (స్థిరరాశి T *, స్థిరరాశి uchar *, ST *, SQT *, పూర్ణాంకం, పూర్ణాంకం) స్థిరరాశి
    {
        తిరిగి 0;
    }
};

#ఒకవేళ (CV_SIMD || CV_SIMD_SCALABLE)

మూస <>
నిర్మాణం కూడిక_Sqr_SIMD<uchar, పూర్ణాంకం, పూర్ణాంకం>
{
    పూర్ణాంకం క్రియాకారకం () (స్థిరరాశి uchar * src0, స్థిరరాశి uchar * ముసుగు, పూర్ణాంకం * కూడిక, పూర్ణాంకం * sqsum, పూర్ణాంకం len, పూర్ణాంకం cn) స్థిరరాశి
    {
        ఒకవేళ (ముసుగు)
            తిరిగి 0;
        // cn==3: deinterleave so each lane belongs to one channel, then reduce per
        // channel. sum/sqsum stay exact in s32 within the dispatcher's 1<<15 block.
        ఒకవేళ (cn == 3)
        {
            స్థిరరాశి పూర్ణాంకం vl8 = VTraits<v_uint8>::vlanes();
            v_int32 vs0 = vx_setzero_s32(), vs1 = vx_setzero_s32(), vs2 = vx_setzero_s32();
            v_int32 vq0 = vx_setzero_s32(), vq1 = vx_setzero_s32(), vq2 = vx_setzero_s32();
            స్వయంచాలక acc = [](స్థిరరాశి v_uint8& ch, v_int32& vs, v_int32& vq)
            {
                v_uint16 lo, hi; v_expand(ch, lo, hi);
                v_uint32 s0, s1, s2, s3; v_expand(lo, s0, s1); v_expand(hi, s2, s3);
                vs = v_కూడు(vs, v_reinterpret_as_s32(v_కూడు(v_కూడు(s0, s1), v_కూడు(s2, s3))));
                v_int16 d0 = v_reinterpret_as_s16(lo), d1 = v_reinterpret_as_s16(hi);
                vq = v_కూడు(vq, v_కూడు(v_dotprod(d0, d0), v_dotprod(d1, d1)));
            };
            పూర్ణాంకం e = 0;
            కొరకు (; e <= len - vl8; e += vl8)
            {
                v_uint8 a, b, c; v_ఎక్కించు_deinterleave(src0 + e * 3, a, b, c);
                acc(a, vs0, vq0); acc(b, vs1, vq1); acc(c, vs2, vq2);
            }
            కూడిక[0] += v_reduce_కూడిక(vs0); కూడిక[1] += v_reduce_కూడిక(vs1); కూడిక[2] += v_reduce_కూడిక(vs2);
            sqsum[0] += v_reduce_కూడిక(vq0); sqsum[1] += v_reduce_కూడిక(vq1); sqsum[2] += v_reduce_కూడిక(vq2);
            v_cleanup();
            తిరిగి e;
        }
        ఒకవేళ (cn != 1 && cn != 2 && cn != 4)
            తిరిగి 0;
        len *= cn;

        పూర్ణాంకం x = 0;
        v_int32 v_కూడిక = vx_setzero_s32();
        v_int32 v_sqsum = vx_setzero_s32();

        స్థిరరాశి పూర్ణాంకం len0 = len & -VTraits<v_uint8>::vlanes();
        అయితే(x < len0)
        {
            స్థిరరాశి పూర్ణాంకం len_tmp = min(x + 256*VTraits<v_uint16>::vlanes(), len0);
            v_uint16 v_sum16 = vx_setzero_u16();
            కొరకు ( ; x < len_tmp; x += VTraits<v_uint8>::vlanes())
            {
                v_uint16 v_src0 = vx_ఎక్కించు_expand(src0 + x);
                v_uint16 v_src1 = vx_ఎక్కించు_expand(src0 + x + VTraits<v_uint16>::vlanes());
                v_sum16 = v_కూడు(v_sum16, v_కూడు(v_src0, v_src1));
                v_int16 v_tmp0, v_tmp1;
                v_zip(v_reinterpret_as_s16(v_src0), v_reinterpret_as_s16(v_src1), v_tmp0, v_tmp1);
                v_sqsum = v_కూడు(v_sqsum, v_కూడు(v_dotprod(v_tmp0, v_tmp0), v_dotprod(v_tmp1, v_tmp1)));
            }
            v_uint32 v_half0, v_half1;
            v_expand(v_sum16, v_half0, v_half1);
            v_కూడిక = v_కూడు(v_కూడిక, v_reinterpret_as_s32(v_కూడు(v_half0, v_half1)));
        }
        ఒకవేళ (x <= len - VTraits<v_uint16>::vlanes())
        {
            v_uint16 v_src = vx_ఎక్కించు_expand(src0 + x);
            v_uint16 v_half = v_combine_ఎగువ(v_src, v_src);

            v_uint32 v_tmp0, v_tmp1;
            v_expand(v_కూడు(v_src, v_half), v_tmp0, v_tmp1);
            v_కూడిక = v_కూడు(v_కూడిక, v_reinterpret_as_s32(v_tmp0));

            v_int16 v_tmp2, v_tmp3;
            v_zip(v_reinterpret_as_s16(v_src), v_reinterpret_as_s16(v_half), v_tmp2, v_tmp3);
            v_sqsum = v_కూడు(v_sqsum, v_dotprod(v_tmp2, v_tmp2));
            x += VTraits<v_uint16>::vlanes();
        }

        ఒకవేళ (cn == 1)
        {
            *కూడిక += v_reduce_కూడిక(v_కూడిక);
            *sqsum += v_reduce_కూడిక(v_sqsum);
        }
        లేకపోతే
        {
            పూర్ణాంకం CV_DECL_ALIGNED(CV_SIMD_వెడల్పు) ar[2 * VTraits<v_int32>::max_nlanes];
            v_store(ar, v_కూడిక);
            v_store(ar + VTraits<v_int32>::vlanes(), v_sqsum);
            కొరకు (పూర్ణాంకం i = 0; i < VTraits<v_int32>::vlanes(); ++i)
            {
                కూడిక[i % cn] += ar[i];
                sqsum[i % cn] += ar[VTraits<v_int32>::vlanes() + i];
            }
        }
        v_cleanup();
        తిరిగి x / cn;
    }
};

మూస <>
నిర్మాణం కూడిక_Sqr_SIMD<schar, పూర్ణాంకం, పూర్ణాంకం>
{
    పూర్ణాంకం క్రియాకారకం () (స్థిరరాశి schar * src0, స్థిరరాశి uchar * ముసుగు, పూర్ణాంకం * కూడిక, పూర్ణాంకం * sqsum, పూర్ణాంకం len, పూర్ణాంకం cn) స్థిరరాశి
    {
        ఒకవేళ (ముసుగు)
            తిరిగి 0;
        ఒకవేళ (cn == 3)
        {
            స్థిరరాశి పూర్ణాంకం vl8 = VTraits<v_int8>::vlanes();
            v_int32 vs0 = vx_setzero_s32(), vs1 = vx_setzero_s32(), vs2 = vx_setzero_s32();
            v_int32 vq0 = vx_setzero_s32(), vq1 = vx_setzero_s32(), vq2 = vx_setzero_s32();
            స్వయంచాలక acc = [](స్థిరరాశి v_int8& ch, v_int32& vs, v_int32& vq)
            {
                v_int16 lo, hi; v_expand(ch, lo, hi);
                v_int32 s0, s1, s2, s3; v_expand(lo, s0, s1); v_expand(hi, s2, s3);
                vs = v_కూడు(vs, v_కూడు(v_కూడు(s0, s1), v_కూడు(s2, s3)));
                vq = v_కూడు(vq, v_కూడు(v_dotprod(lo, lo), v_dotprod(hi, hi)));
            };
            పూర్ణాంకం e = 0;
            కొరకు (; e <= len - vl8; e += vl8)
            {
                v_int8 a, b, c; v_ఎక్కించు_deinterleave(src0 + e * 3, a, b, c);
                acc(a, vs0, vq0); acc(b, vs1, vq1); acc(c, vs2, vq2);
            }
            కూడిక[0] += v_reduce_కూడిక(vs0); కూడిక[1] += v_reduce_కూడిక(vs1); కూడిక[2] += v_reduce_కూడిక(vs2);
            sqsum[0] += v_reduce_కూడిక(vq0); sqsum[1] += v_reduce_కూడిక(vq1); sqsum[2] += v_reduce_కూడిక(vq2);
            v_cleanup();
            తిరిగి e;
        }
        ఒకవేళ (cn != 1 && cn != 2 && cn != 4)
            తిరిగి 0;
        len *= cn;

        పూర్ణాంకం x = 0;
        v_int32 v_కూడిక = vx_setzero_s32();
        v_int32 v_sqsum = vx_setzero_s32();

        స్థిరరాశి పూర్ణాంకం len0 = len & -VTraits<v_int8>::vlanes();
        అయితే (x < len0)
        {
            స్థిరరాశి పూర్ణాంకం len_tmp = min(x + 256 * VTraits<v_int16>::vlanes(), len0);
            v_int16 v_sum16 = vx_setzero_s16();
            కొరకు (; x < len_tmp; x += VTraits<v_int8>::vlanes())
            {
                v_int16 v_src0 = vx_ఎక్కించు_expand(src0 + x);
                v_int16 v_src1 = vx_ఎక్కించు_expand(src0 + x + VTraits<v_int16>::vlanes());
                v_sum16 = v_కూడు(v_sum16, v_కూడు(v_src0, v_src1));
                v_int16 v_tmp0, v_tmp1;
                v_zip(v_src0, v_src1, v_tmp0, v_tmp1);
                v_sqsum = v_కూడు(v_sqsum, v_కూడు(v_dotprod(v_tmp0, v_tmp0), v_dotprod(v_tmp1, v_tmp1)));
            }
            v_int32 v_half0, v_half1;
            v_expand(v_sum16, v_half0, v_half1);
            v_కూడిక = v_కూడు(v_కూడిక, v_కూడు(v_half0, v_half1));
        }
        ఒకవేళ (x <= len - VTraits<v_int16>::vlanes())
        {
            v_int16 v_src = vx_ఎక్కించు_expand(src0 + x);
            v_int16 v_half = v_combine_ఎగువ(v_src, v_src);

            v_int32 v_tmp0, v_tmp1;
            v_expand(v_కూడు(v_src, v_half), v_tmp0, v_tmp1);
            v_కూడిక = v_కూడు(v_కూడిక, v_tmp0);

            v_int16 v_tmp2, v_tmp3;
            v_zip(v_src, v_half, v_tmp2, v_tmp3);
            v_sqsum = v_కూడు(v_sqsum, v_dotprod(v_tmp2, v_tmp2));
            x += VTraits<v_int16>::vlanes();
        }

        ఒకవేళ (cn == 1)
        {
            *కూడిక += v_reduce_కూడిక(v_కూడిక);
            *sqsum += v_reduce_కూడిక(v_sqsum);
        }
        లేకపోతే
        {
            పూర్ణాంకం CV_DECL_ALIGNED(CV_SIMD_వెడల్పు) ar[2 * VTraits<v_int32>::max_nlanes];
            v_store(ar, v_కూడిక);
            v_store(ar + VTraits<v_int32>::vlanes(), v_sqsum);
            కొరకు (పూర్ణాంకం i = 0; i < VTraits<v_int32>::vlanes(); ++i)
            {
                కూడిక[i % cn] += ar[i];
                sqsum[i % cn] += ar[VTraits<v_int32>::vlanes() + i];
            }
        }
        v_cleanup();
        తిరిగి x / cn;
    }
};

#ఒకవేళ (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)

// Expand a 16-bit vector into two s32 halves. The unsigned overload reinterprets
// after a widening expand (values 0..65535 stay non-negative in s32, so the
// subsequent v_cvt_f64 is exact); the signed overload expands directly. This is
// the only real difference between the ushort and short reductions below.
స్థిర అంతఃవరుస శూన్య కూడిక_Sqr_Expand_S32(స్థిరరాశి v_uint16& v, v_int32& lo, v_int32& hi)
{
    v_uint32 a, b; v_expand(v, a, b);
    lo = v_reinterpret_as_s32(a); hi = v_reinterpret_as_s32(b);
}
స్థిర అంతఃవరుస శూన్య కూడిక_Sqr_Expand_S32(స్థిరరాశి v_int16& v, v_int32& lo, v_int32& hi)
{
    v_expand(v, lo, hi);
}

// Shared 16-bit -> {s32 sum, f64 sqsum} reduction for ushort/short. The register
// type is deduced from the pointer, so the body is identical for both depths.
మూస <రకం_పేరు T>
స్థిర అంతఃవరుస పూర్ణాంకం కూడిక_Sqr16_గమ్యం_F64(స్థిరరాశి T* src0, పూర్ణాంకం* కూడిక, ద్విగుణ* sqsum, పూర్ణాంకం len, పూర్ణాంకం cn)
{
    రకనిర్వచనం decltype(vx_ఎక్కించు(src0)) VT; // v_uint16 or v_int16

    ఒకవేళ (cn == 3)
    {
        స్థిరరాశి పూర్ణాంకం vl = VTraits<VT>::vlanes();
        v_int32 vs0 = vx_setzero_s32(), vs1 = vx_setzero_s32(), vs2 = vx_setzero_s32();
        v_float64 vq0 = vx_setzero_f64(), vq1 = vx_setzero_f64(), vq2 = vx_setzero_f64();
        స్వయంచాలక acc = [](స్థిరరాశి VT& ch, v_int32& vs, v_float64& vq)
        {
            v_int32 lo, hi; కూడిక_Sqr_Expand_S32(ch, lo, hi);
            vs = v_కూడు(vs, v_కూడు(lo, hi));
            v_float64 f0 = v_cvt_f64(lo), f1 = v_cvt_f64_ఎగువ(lo);
            v_float64 f2 = v_cvt_f64(hi), f3 = v_cvt_f64_ఎగువ(hi);
            vq = v_fma(f0, f0, v_fma(f1, f1, v_fma(f2, f2, v_fma(f3, f3, vq))));
        };
        పూర్ణాంకం e = 0;
        కొరకు (; e <= len - vl; e += vl)
        {
            VT a, b, c; v_ఎక్కించు_deinterleave(src0 + e * 3, a, b, c);
            acc(a, vs0, vq0); acc(b, vs1, vq1); acc(c, vs2, vq2);
        }
        కూడిక[0] += v_reduce_కూడిక(vs0); కూడిక[1] += v_reduce_కూడిక(vs1); కూడిక[2] += v_reduce_కూడిక(vs2);
        sqsum[0] += v_reduce_కూడిక(vq0); sqsum[1] += v_reduce_కూడిక(vq1); sqsum[2] += v_reduce_కూడిక(vq2);
        v_cleanup();
        తిరిగి e;
    }
    ఒకవేళ (cn != 1 && cn != 2 && cn != 4)
        తిరిగి 0;
    len *= cn;

    స్థిరరాశి పూర్ణాంకం vl16 = VTraits<VT>::vlanes();
    స్థిరరాశి పూర్ణాంకం vl32 = VTraits<v_int32>::vlanes();
    స్థిరరాశి పూర్ణాంకం vl64 = VTraits<v_float64>::vlanes();

    // The lane->channel scatter below (i % cn) and the returned pixel count
    // (x / cn) require the lane counts to be a multiple of cn. This always holds
    // for fixed-width SIMD (vlanes is a power of two >= cn for cn in {1,2,4}),
    // but scalable backends (e.g. RVV) may report a non-multiple; fall back to
    // scalar there. vl16 == 2*vl32, so checking vl32 covers both scatter loops.
    ఒకవేళ (vl32 % cn != 0)
        తిరిగి 0;

    v_int32 v_కూడిక = vx_setzero_s32();
    v_float64 v_sq0 = vx_setzero_f64(), v_sq1 = vx_setzero_f64();
    v_float64 v_sq2 = vx_setzero_f64(), v_sq3 = vx_setzero_f64();

    పూర్ణాంకం x = 0;
    కొరకు (; x <= len - vl16; x += vl16)
    {
        VT v_src = vx_ఎక్కించు(src0 + x);
        v_int32 lo, hi; కూడిక_Sqr_Expand_S32(v_src, lo, hi);
        v_కూడిక = v_కూడు(v_కూడిక, v_కూడు(lo, hi));

        v_float64 f0 = v_cvt_f64(lo);
        v_float64 f1 = v_cvt_f64_ఎగువ(lo);
        v_float64 f2 = v_cvt_f64(hi);
        v_float64 f3 = v_cvt_f64_ఎగువ(hi);
        v_sq0 = v_fma(f0, f0, v_sq0);
        v_sq1 = v_fma(f1, f1, v_sq1);
        v_sq2 = v_fma(f2, f2, v_sq2);
        v_sq3 = v_fma(f3, f3, v_sq3);
    }

    పూర్ణాంకం    CV_DECL_ALIGNED(CV_SIMD_వెడల్పు) sbuf[VTraits<v_int32>::max_nlanes];
    ద్విగుణ CV_DECL_ALIGNED(CV_SIMD_వెడల్పు) qbuf[VTraits<v_float64>::max_nlanes * 4];
    v_store_aligned(sbuf, v_కూడిక);
    v_store_aligned(qbuf + vl64 * 0, v_sq0);
    v_store_aligned(qbuf + vl64 * 1, v_sq1);
    v_store_aligned(qbuf + vl64 * 2, v_sq2);
    v_store_aligned(qbuf + vl64 * 3, v_sq3);

    // sbuf lane j (j<vl32) holds samples j and j+vl32 -> channel j%cn (vl32%cn==0).
    // qbuf index i (i<vl16) holds sample lane i           -> channel i%cn (vl16%cn==0).
    కొరకు (పూర్ణాంకం i = 0; i < vl32; ++i)
        కూడిక[i % cn] += sbuf[i];
    కొరకు (పూర్ణాంకం i = 0; i < vl16; ++i)
        sqsum[i % cn] += qbuf[i];

    v_cleanup();
    తిరిగి x / cn;
}

// Shared 32-bit -> f64 reduction for int/float. Both sum and sqsum accumulate in
// f64 to match the scalar reference's double accumulation; values are widened to
// f64 before squaring so the square is computed in double (same as (double)v*v).
// The register type is deduced from the pointer, so int and float share one body.
మూస <రకం_పేరు T>
స్థిర అంతఃవరుస పూర్ణాంకం కూడిక_Sqr32_గమ్యం_F64(స్థిరరాశి T* src0, ద్విగుణ* కూడిక, ద్విగుణ* sqsum, పూర్ణాంకం len, పూర్ణాంకం cn)
{
    రకనిర్వచనం decltype(vx_ఎక్కించు(src0)) VT; // v_int32 or v_float32

    ఒకవేళ (cn == 3)
    {
        స్థిరరాశి పూర్ణాంకం vl = VTraits<VT>::vlanes();
        v_float64 vs0 = vx_setzero_f64(), vs1 = vx_setzero_f64(), vs2 = vx_setzero_f64();
        v_float64 vq0 = vx_setzero_f64(), vq1 = vx_setzero_f64(), vq2 = vx_setzero_f64();
        స్వయంచాలక acc = [](స్థిరరాశి VT& ch, v_float64& vs, v_float64& vq)
        {
            v_float64 f0 = v_cvt_f64(ch), f1 = v_cvt_f64_ఎగువ(ch);
            vs = v_కూడు(vs, v_కూడు(f0, f1));
            vq = v_fma(f0, f0, v_fma(f1, f1, vq));
        };
        పూర్ణాంకం e = 0;
        కొరకు (; e <= len - vl; e += vl)
        {
            VT a, b, c; v_ఎక్కించు_deinterleave(src0 + e * 3, a, b, c);
            acc(a, vs0, vq0); acc(b, vs1, vq1); acc(c, vs2, vq2);
        }
        కూడిక[0] += v_reduce_కూడిక(vs0); కూడిక[1] += v_reduce_కూడిక(vs1); కూడిక[2] += v_reduce_కూడిక(vs2);
        sqsum[0] += v_reduce_కూడిక(vq0); sqsum[1] += v_reduce_కూడిక(vq1); sqsum[2] += v_reduce_కూడిక(vq2);
        v_cleanup();
        తిరిగి e;
    }
    ఒకవేళ (cn != 1 && cn != 2 && cn != 4)
        తిరిగి 0;
    len *= cn;

    స్థిరరాశి పూర్ణాంకం vl32 = VTraits<VT>::vlanes();
    స్థిరరాశి పూర్ణాంకం vl64 = VTraits<v_float64>::vlanes();

    // See note in sumSqr16ToF64: guard scalable backends whose lane count may
    // not divide cn (the i % cn scatter / x / cn rely on it).
    ఒకవేళ (vl32 % cn != 0)
        తిరిగి 0;

    v_float64 vs0 = vx_setzero_f64(), vs1 = vx_setzero_f64();
    v_float64 vq0 = vx_setzero_f64(), vq1 = vx_setzero_f64();

    పూర్ణాంకం x = 0;
    కొరకు (; x <= len - vl32; x += vl32)
    {
        VT v_src = vx_ఎక్కించు(src0 + x);
        v_float64 f0 = v_cvt_f64(v_src);
        v_float64 f1 = v_cvt_f64_ఎగువ(v_src);
        vs0 = v_కూడు(vs0, f0);
        vs1 = v_కూడు(vs1, f1);
        vq0 = v_fma(f0, f0, vq0);
        vq1 = v_fma(f1, f1, vq1);
    }

    ద్విగుణ CV_DECL_ALIGNED(CV_SIMD_వెడల్పు) sbuf[VTraits<v_float64>::max_nlanes * 2];
    ద్విగుణ CV_DECL_ALIGNED(CV_SIMD_వెడల్పు) qbuf[VTraits<v_float64>::max_nlanes * 2];
    v_store_aligned(sbuf, vs0);          v_store_aligned(sbuf + vl64, vs1);
    v_store_aligned(qbuf, vq0);          v_store_aligned(qbuf + vl64, vq1);

    కొరకు (పూర్ణాంకం i = 0; i < vl32; ++i)
    {
        కూడిక[i % cn]   += sbuf[i];
        sqsum[i % cn] += qbuf[i];
    }

    v_cleanup();
    తిరిగి x / cn;
}

మూస <>
నిర్మాణం కూడిక_Sqr_SIMD<ushort, పూర్ణాంకం, ద్విగుణ>
{
    పూర్ణాంకం క్రియాకారకం () (స్థిరరాశి ushort* src0, స్థిరరాశి uchar* ముసుగు, పూర్ణాంకం* కూడిక, ద్విగుణ* sqsum, పూర్ణాంకం len, పూర్ణాంకం cn) స్థిరరాశి
    { తిరిగి ముసుగు ? 0 : కూడిక_Sqr16_గమ్యం_F64(src0, కూడిక, sqsum, len, cn); }
};

మూస <>
నిర్మాణం కూడిక_Sqr_SIMD<పొట్టి, పూర్ణాంకం, ద్విగుణ>
{
    పూర్ణాంకం క్రియాకారకం () (స్థిరరాశి పొట్టి* src0, స్థిరరాశి uchar* ముసుగు, పూర్ణాంకం* కూడిక, ద్విగుణ* sqsum, పూర్ణాంకం len, పూర్ణాంకం cn) స్థిరరాశి
    { తిరిగి ముసుగు ? 0 : కూడిక_Sqr16_గమ్యం_F64(src0, కూడిక, sqsum, len, cn); }
};

మూస <>
నిర్మాణం కూడిక_Sqr_SIMD<పూర్ణాంకం, ద్విగుణ, ద్విగుణ>
{
    పూర్ణాంకం క్రియాకారకం () (స్థిరరాశి పూర్ణాంకం* src0, స్థిరరాశి uchar* ముసుగు, ద్విగుణ* కూడిక, ద్విగుణ* sqsum, పూర్ణాంకం len, పూర్ణాంకం cn) స్థిరరాశి
    { తిరిగి ముసుగు ? 0 : కూడిక_Sqr32_గమ్యం_F64(src0, కూడిక, sqsum, len, cn); }
};

మూస <>
నిర్మాణం కూడిక_Sqr_SIMD<దశాంశం, ద్విగుణ, ద్విగుణ>
{
    పూర్ణాంకం క్రియాకారకం () (స్థిరరాశి దశాంశం* src0, స్థిరరాశి uchar* ముసుగు, ద్విగుణ* కూడిక, ద్విగుణ* sqsum, పూర్ణాంకం len, పూర్ణాంకం cn) స్థిరరాశి
    { తిరిగి ముసుగు ? 0 : కూడిక_Sqr32_గమ్యం_F64(src0, కూడిక, sqsum, len, cn); }
};

#అయితే_ముగింపు // CV_SIMD_64F

#అయితే_ముగింపు

మూస<రకం_పేరు T, రకం_పేరు ST, రకం_పేరు SQT>
స్థిర పూర్ణాంకం sumsqr_(స్థిరరాశి T* src0, స్థిరరాశి uchar* ముసుగు, ST* కూడిక, SQT* sqsum, పూర్ణాంకం len, పూర్ణాంకం cn )
{
    స్థిరరాశి T* src = src0;

    ఒకవేళ( !ముసుగు )
    {
        కూడిక_Sqr_SIMD<T, ST, SQT> vop;
        పూర్ణాంకం x = vop(src0, ముసుగు, కూడిక, sqsum, len, cn), k = cn % 4;
        src = src0 + x * cn;

        ఒకవేళ( k == 1 )
        {
            ST s0 = కూడిక[0];
            SQT sq0 = sqsum[0];
            కొరకు(పూర్ణాంకం i = x; i < len; i++, src += cn )
            {
                ST v = (ST)src[0];
                s0 += v; sq0 += (SQT)v*v;
            }
            కూడిక[0] = s0;
            sqsum[0] = sq0;
        }
        లేకపోతే ఒకవేళ( k == 2 )
        {
            ST s0 = కూడిక[0], s1 = కూడిక[1];
            SQT sq0 = sqsum[0], sq1 = sqsum[1];
            కొరకు(పూర్ణాంకం i = x; i < len; i++, src += cn )
            {
                ST v0 = (ST)src[0], v1 = (ST)src[1];
                s0 += v0; sq0 += (SQT)v0*v0;
                s1 += v1; sq1 += (SQT)v1*v1;
            }
            కూడిక[0] = s0; కూడిక[1] = s1;
            sqsum[0] = sq0; sqsum[1] = sq1;
        }
        లేకపోతే ఒకవేళ( k == 3 )
        {
            ST s0 = కూడిక[0], s1 = కూడిక[1], s2 = కూడిక[2];
            SQT sq0 = sqsum[0], sq1 = sqsum[1], sq2 = sqsum[2];
            కొరకు(పూర్ణాంకం i = x; i < len; i++, src += cn )
            {
                ST v0 = (ST)src[0], v1 = (ST)src[1], v2 = (ST)src[2];
                s0 += v0; sq0 += (SQT)v0*v0;
                s1 += v1; sq1 += (SQT)v1*v1;
                s2 += v2; sq2 += (SQT)v2*v2;
            }
            కూడిక[0] = s0; కూడిక[1] = s1; కూడిక[2] = s2;
            sqsum[0] = sq0; sqsum[1] = sq1; sqsum[2] = sq2;
        }

        కొరకు( ; k < cn; k += 4 )
        {
            src = src0 + x * cn + k;
            ST s0 = కూడిక[k], s1 = కూడిక[k+1], s2 = కూడిక[k+2], s3 = కూడిక[k+3];
            SQT sq0 = sqsum[k], sq1 = sqsum[k+1], sq2 = sqsum[k+2], sq3 = sqsum[k+3];
            కొరకు(పూర్ణాంకం i = x; i < len; i++, src += cn )
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
            కూడిక[k] = s0; కూడిక[k+1] = s1;
            కూడిక[k+2] = s2; కూడిక[k+3] = s3;
            sqsum[k] = sq0; sqsum[k+1] = sq1;
            sqsum[k+2] = sq2; sqsum[k+3] = sq3;
        }
        తిరిగి len;
    }

    పూర్ణాంకం i, nzm = 0;

    ఒకవేళ( cn == 1 )
    {
        ST s0 = కూడిక[0];
        SQT sq0 = sqsum[0];
        కొరకు( i = 0; i < len; i++ )
            ఒకవేళ( ముసుగు[i] )
            {
                ST v = (ST)src[i];
                s0 += v; sq0 += (SQT)v*v;
                nzm++;
            }
        కూడిక[0] = s0;
        sqsum[0] = sq0;
    }
    లేకపోతే ఒకవేళ( cn == 2 )
    {
        ST s0 = కూడిక[0], s1 = కూడిక[1];
        SQT sq0 = sqsum[0], sq1 = sqsum[1];
        కొరకు( i = 0; i < len; i++, src += 2 )
            ఒకవేళ( ముసుగు[i] )
            {
                T v0 = src[0], v1 = src[1];
                s0 += v0; sq0 += (SQT)v0*v0;
                s1 += v1; sq1 += (SQT)v1*v1;
                nzm++;
            }
        కూడిక[0] = s0; కూడిక[1] = s1;
        sqsum[0] = sq0; sqsum[1] = sq1;
    }
    లేకపోతే ఒకవేళ( cn == 3 )
    {
        ST s0 = కూడిక[0], s1 = కూడిక[1], s2 = కూడిక[2];
        SQT sq0 = sqsum[0], sq1 = sqsum[1], sq2 = sqsum[2];
        కొరకు( i = 0; i < len; i++, src += 3 )
            ఒకవేళ( ముసుగు[i] )
            {
                ST v0 = (ST)src[0], v1 = (ST)src[1], v2 = (ST)src[2];
                s0 += v0; sq0 += (SQT)v0*v0;
                s1 += v1; sq1 += (SQT)v1*v1;
                s2 += v2; sq2 += (SQT)v2*v2;
                nzm++;
            }
        కూడిక[0] = s0; కూడిక[1] = s1; కూడిక[2] = s2;
        sqsum[0] = sq0; sqsum[1] = sq1; sqsum[2] = sq2;
    }
    లేకపోతే ఒకవేళ( cn == 4 )
    {
        ST s0 = కూడిక[0], s1 = కూడిక[1], s2 = కూడిక[2], s3 = కూడిక[3];
        SQT sq0 = sqsum[0], sq1 = sqsum[1], sq2 = sqsum[2], sq3 = sqsum[3];
        కొరకు( i = 0; i < len; i++, src += 4 )
            ఒకవేళ( ముసుగు[i] )
            {
                T v0 = src[0], v1 = src[1], v2 = src[2], v3 = src[3];
                s0 += v0; sq0 += (SQT)v0*v0;
                s1 += v1; sq1 += (SQT)v1*v1;
                s2 += v2; sq2 += (SQT)v2*v2;
                s3 += v3; sq3 += (SQT)v3*v3;
                nzm++;
            }
        కూడిక[0] = s0; కూడిక[1] = s1; కూడిక[2] = s2; కూడిక[3] = s3;
        sqsum[0] = sq0; sqsum[1] = sq1; sqsum[2] = sq2; sqsum[3] = sq3;
    }
    లేకపోతే
    {
        కొరకు( i = 0; i < len; i++, src += cn )
            ఒకవేళ( ముసుగు[i] )
            {
                కొరకు( పూర్ణాంకం k = 0; k < cn; k++ )
                {
                    ST v = (ST)src[k];
                    ST s = కూడిక[k] + v;
                    SQT sq = sqsum[k] + (SQT)v*v;
                    కూడిక[k] = s; sqsum[k] = sq;
                }
                nzm++;
            }
    }
    తిరిగి nzm;
}


స్థిర పూర్ణాంకం sqsum8u( స్థిరరాశి uchar* src, స్థిరరాశి uchar* ముసుగు, పూర్ణాంకం* కూడిక, పూర్ణాంకం* sqsum, పూర్ణాంకం len, పూర్ణాంకం cn )
{ CV_INSTRUMENT_REGION(); తిరిగి sumsqr_(src, ముసుగు, కూడిక, sqsum, len, cn); }

స్థిర పూర్ణాంకం sqsum8s( స్థిరరాశి schar* src, స్థిరరాశి uchar* ముసుగు, పూర్ణాంకం* కూడిక, పూర్ణాంకం* sqsum, పూర్ణాంకం len, పూర్ణాంకం cn )
{ CV_INSTRUMENT_REGION(); తిరిగి sumsqr_(src, ముసుగు, కూడిక, sqsum, len, cn); }

స్థిర పూర్ణాంకం sqsum16u( స్థిరరాశి ushort* src, స్థిరరాశి uchar* ముసుగు, పూర్ణాంకం* కూడిక, ద్విగుణ* sqsum, పూర్ణాంకం len, పూర్ణాంకం cn )
{ CV_INSTRUMENT_REGION(); తిరిగి sumsqr_(src, ముసుగు, కూడిక, sqsum, len, cn); }

స్థిర పూర్ణాంకం sqsum16s( స్థిరరాశి పొట్టి* src, స్థిరరాశి uchar* ముసుగు, పూర్ణాంకం* కూడిక, ద్విగుణ* sqsum, పూర్ణాంకం len, పూర్ణాంకం cn )
{ CV_INSTRUMENT_REGION(); తిరిగి sumsqr_(src, ముసుగు, కూడిక, sqsum, len, cn); }

స్థిర పూర్ణాంకం sqsum32s( స్థిరరాశి పూర్ణాంకం* src, స్థిరరాశి uchar* ముసుగు, ద్విగుణ* కూడిక, ద్విగుణ* sqsum, పూర్ణాంకం len, పూర్ణాంకం cn )
{ CV_INSTRUMENT_REGION(); తిరిగి sumsqr_(src, ముసుగు, కూడిక, sqsum, len, cn); }

స్థిర పూర్ణాంకం sqsum32f( స్థిరరాశి దశాంశం* src, స్థిరరాశి uchar* ముసుగు, ద్విగుణ* కూడిక, ద్విగుణ* sqsum, పూర్ణాంకం len, పూర్ణాంకం cn )
{ CV_INSTRUMENT_REGION(); తిరిగి sumsqr_(src, ముసుగు, కూడిక, sqsum, len, cn); }

స్థిర పూర్ణాంకం sqsum64f( స్థిరరాశి ద్విగుణ* src, స్థిరరాశి uchar* ముసుగు, ద్విగుణ* కూడిక, ద్విగుణ* sqsum, పూర్ణాంకం len, పూర్ణాంకం cn )
{ CV_INSTRUMENT_REGION(); తిరిగి sumsqr_(src, ముసుగు, కూడిక, sqsum, len, cn); }

స్థిర పూర్ణాంకం sqsum16f( స్థిరరాశి hfloat* src, స్థిరరాశి uchar* ముసుగు, దశాంశం* కూడిక, ద్విగుణ* sqsum, పూర్ణాంకం len, పూర్ణాంకం cn )
{ CV_INSTRUMENT_REGION(); తిరిగి sumsqr_(src, ముసుగు, కూడిక, sqsum, len, cn); }

స్థిర పూర్ణాంకం sqsum16bf( స్థిరరాశి bfloat* src, స్థిరరాశి uchar* ముసుగు, దశాంశం* కూడిక, ద్విగుణ* sqsum, పూర్ణాంకం len, పూర్ణాంకం cn )
{ CV_INSTRUMENT_REGION(); తిరిగి sumsqr_(src, ముసుగు, కూడిక, sqsum, len, cn); }

స్థిర పూర్ణాంకం sqsum64u( స్థిరరాశి uint64* src, స్థిరరాశి uchar* ముసుగు, ద్విగుణ* కూడిక, ద్విగుణ* sqsum, పూర్ణాంకం len, పూర్ణాంకం cn )
{ CV_INSTRUMENT_REGION(); తిరిగి sumsqr_(src, ముసుగు, కూడిక, sqsum, len, cn); }

స్థిర పూర్ణాంకం sqsum64s( స్థిరరాశి int64* src, స్థిరరాశి uchar* ముసుగు, ద్విగుణ* కూడిక, ద్విగుణ* sqsum, పూర్ణాంకం len, పూర్ణాంకం cn )
{ CV_INSTRUMENT_REGION(); తిరిగి sumsqr_(src, ముసుగు, కూడిక, sqsum, len, cn); }

స్థిర పూర్ణాంకం sqsum32u( స్థిరరాశి గుర్తులేని* src, స్థిరరాశి uchar* ముసుగు, ద్విగుణ* కూడిక, ద్విగుణ* sqsum, పూర్ణాంకం len, పూర్ణాంకం cn )
{ CV_INSTRUMENT_REGION(); తిరిగి sumsqr_(src, ముసుగు, కూడిక, sqsum, len, cn); }

కూడిక_Sqr_Func get_కూడిక_Sqr_Func(పూర్ణాంకం లోతు)
{
    CV_INSTRUMENT_REGION();
    స్థిర కూడిక_Sqr_Func కూడిక_Sqr_Tab[CV_లోతు_MAX] =
    {
        కూడిక_Sqr_Wrap<uchar, పూర్ణాంకం, పూర్ణాంకం, sqsum8u>, కూడిక_Sqr_Wrap<schar, పూర్ణాంకం, పూర్ణాంకం, sqsum8s>,
        కూడిక_Sqr_Wrap<ushort, పూర్ణాంకం, ద్విగుణ, sqsum16u>, కూడిక_Sqr_Wrap<పొట్టి, పూర్ణాంకం, ద్విగుణ, sqsum16s>,
        కూడిక_Sqr_Wrap<పూర్ణాంకం, ద్విగుణ, ద్విగుణ, sqsum32s>, కూడిక_Sqr_Wrap<దశాంశం, ద్విగుణ, ద్విగుణ, sqsum32f>,
        కూడిక_Sqr_Wrap<ద్విగుణ, ద్విగుణ, ద్విగుణ, sqsum64f>,
        కూడిక_Sqr_Wrap<hfloat, దశాంశం, ద్విగుణ, sqsum16f>, కూడిక_Sqr_Wrap<bfloat, దశాంశం, ద్విగుణ, sqsum16bf>, 0,
        కూడిక_Sqr_Wrap<uint64, ద్విగుణ, ద్విగుణ, sqsum64u>, కూడిక_Sqr_Wrap<int64, ద్విగుణ, ద్విగుణ, sqsum64s>,
        కూడిక_Sqr_Wrap<గుర్తులేని, ద్విగుణ, ద్విగుణ, sqsum32u>, 0
    };

    తిరిగి కూడిక_Sqr_Tab[లోతు];
}

#అయితే_ముగింపు
CV_CPU_OPTIMIZATION_NAMESPACE_చివర
} // namespace
