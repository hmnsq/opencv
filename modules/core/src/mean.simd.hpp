// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html

// Copyright (C) 2026, Advanced Micro Devices, Inc., all rights reserved.

#समावेश "precomp.hpp"
#समावेश "stat.hpp"

namespace cv {
प्रकार पूर्णांक (*योग_Sqr_Func)(स्थिर uchar*, स्थिर uchar* mask, uchar*, uchar*, पूर्णांक, पूर्णांक);

CV_CPU_OPTIMIZATION_NAMESPACE_BEGIN

ढाँचा<typename T, typename ST, typename SQT, पूर्णांक (*fn)(स्थिर T*, स्थिर uchar*, ST*, SQT*, पूर्णांक, पूर्णांक)>
अटल पूर्णांक योग_Sqr_Wrap(स्थिर uchar* src, स्थिर uchar* mask, uchar* योग_2, uchar* sqsum, पूर्णांक len, पूर्णांक cn)
{
    वापस fn((स्थिर T*)src, mask, (ST*)योग_2, (SQT*)sqsum, len, cn);
}

योग_Sqr_Func get_योग_Sqr_Func(पूर्णांक depth);

#अगर_अघोषित CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

ढाँचा <typename T, typename ST, typename SQT>
काष्ठा योग_Sqr_SIMD
{
    अंतरभूत पूर्णांक चालक () (स्थिर T *, स्थिर uchar *, ST *, SQT *, पूर्णांक, पूर्णांक) स्थिर
    {
        वापस 0;
    }
};

#अगर (CV_SIMD || CV_SIMD_SCALABLE)

ढाँचा <>
काष्ठा योग_Sqr_SIMD<uchar, पूर्णांक, पूर्णांक>
{
    पूर्णांक चालक () (स्थिर uchar * src0, स्थिर uchar * mask, पूर्णांक * योग_2, पूर्णांक * sqsum, पूर्णांक len, पूर्णांक cn) स्थिर
    {
        अगर (mask)
            वापस 0;
        // cn==3: deinterleave so each lane belongs to one channel, then reduce per
        // channel. sum/sqsum stay exact in s32 within the dispatcher's 1<<15 block.
        अगर (cn == 3)
        {
            स्थिर पूर्णांक vl8 = VTraits<v_uint8>::vlanes();
            v_int32 vs0 = vx_setzero_s32(), vs1 = vx_setzero_s32(), vs2 = vx_setzero_s32();
            v_int32 vq0 = vx_setzero_s32(), vq1 = vx_setzero_s32(), vq2 = vx_setzero_s32();
            स्वतः acc = [](स्थिर v_uint8& ch, v_int32& vs, v_int32& vq)
            {
                v_uint16 lo, hi; v_expand(ch, lo, hi);
                v_uint32 s0, s1, s2, s3; v_expand(lo, s0, s1); v_expand(hi, s2, s3);
                vs = v_add(vs, v_reinterpret_as_s32(v_add(v_add(s0, s1), v_add(s2, s3))));
                v_int16 d0 = v_reinterpret_as_s16(lo), d1 = v_reinterpret_as_s16(hi);
                vq = v_add(vq, v_add(v_dotprod(d0, d0), v_dotprod(d1, d1)));
            };
            पूर्णांक e = 0;
            क्रम (; e <= len - vl8; e += vl8)
            {
                v_uint8 a, b, c; v_load_deinterleave(src0 + e * 3, a, b, c);
                acc(a, vs0, vq0); acc(b, vs1, vq1); acc(c, vs2, vq2);
            }
            योग_2[0] += v_reduce_योग(vs0); योग_2[1] += v_reduce_योग(vs1); योग_2[2] += v_reduce_योग(vs2);
            sqsum[0] += v_reduce_योग(vq0); sqsum[1] += v_reduce_योग(vq1); sqsum[2] += v_reduce_योग(vq2);
            v_cleanup();
            वापस e;
        }
        अगर (cn != 1 && cn != 2 && cn != 4)
            वापस 0;
        len *= cn;

        पूर्णांक x = 0;
        v_int32 v_योग = vx_setzero_s32();
        v_int32 v_sqsum = vx_setzero_s32();

        स्थिर पूर्णांक len0 = len & -VTraits<v_uint8>::vlanes();
        जबतक(x < len0)
        {
            स्थिर पूर्णांक len_tmp = min(x + 256*VTraits<v_uint16>::vlanes(), len0);
            v_uint16 v_sum16 = vx_setzero_u16();
            क्रम ( ; x < len_tmp; x += VTraits<v_uint8>::vlanes())
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
            v_योग = v_add(v_योग, v_reinterpret_as_s32(v_add(v_half0, v_half1)));
        }
        अगर (x <= len - VTraits<v_uint16>::vlanes())
        {
            v_uint16 v_src = vx_load_expand(src0 + x);
            v_uint16 v_half = v_combine_ऊपरी(v_src, v_src);

            v_uint32 v_tmp0, v_tmp1;
            v_expand(v_add(v_src, v_half), v_tmp0, v_tmp1);
            v_योग = v_add(v_योग, v_reinterpret_as_s32(v_tmp0));

            v_int16 v_tmp2, v_tmp3;
            v_zip(v_reinterpret_as_s16(v_src), v_reinterpret_as_s16(v_half), v_tmp2, v_tmp3);
            v_sqsum = v_add(v_sqsum, v_dotprod(v_tmp2, v_tmp2));
            x += VTraits<v_uint16>::vlanes();
        }

        अगर (cn == 1)
        {
            *योग_2 += v_reduce_योग(v_योग);
            *sqsum += v_reduce_योग(v_sqsum);
        }
        अन्यथा
        {
            पूर्णांक CV_DECL_ALIGNED(CV_SIMD_WIDTH) ar[2 * VTraits<v_int32>::max_nlanes];
            v_store(ar, v_योग);
            v_store(ar + VTraits<v_int32>::vlanes(), v_sqsum);
            क्रम (पूर्णांक i = 0; i < VTraits<v_int32>::vlanes(); ++i)
            {
                योग_2[i % cn] += ar[i];
                sqsum[i % cn] += ar[VTraits<v_int32>::vlanes() + i];
            }
        }
        v_cleanup();
        वापस x / cn;
    }
};

ढाँचा <>
काष्ठा योग_Sqr_SIMD<schar, पूर्णांक, पूर्णांक>
{
    पूर्णांक चालक () (स्थिर schar * src0, स्थिर uchar * mask, पूर्णांक * योग_2, पूर्णांक * sqsum, पूर्णांक len, पूर्णांक cn) स्थिर
    {
        अगर (mask)
            वापस 0;
        अगर (cn == 3)
        {
            स्थिर पूर्णांक vl8 = VTraits<v_int8>::vlanes();
            v_int32 vs0 = vx_setzero_s32(), vs1 = vx_setzero_s32(), vs2 = vx_setzero_s32();
            v_int32 vq0 = vx_setzero_s32(), vq1 = vx_setzero_s32(), vq2 = vx_setzero_s32();
            स्वतः acc = [](स्थिर v_int8& ch, v_int32& vs, v_int32& vq)
            {
                v_int16 lo, hi; v_expand(ch, lo, hi);
                v_int32 s0, s1, s2, s3; v_expand(lo, s0, s1); v_expand(hi, s2, s3);
                vs = v_add(vs, v_add(v_add(s0, s1), v_add(s2, s3)));
                vq = v_add(vq, v_add(v_dotprod(lo, lo), v_dotprod(hi, hi)));
            };
            पूर्णांक e = 0;
            क्रम (; e <= len - vl8; e += vl8)
            {
                v_int8 a, b, c; v_load_deinterleave(src0 + e * 3, a, b, c);
                acc(a, vs0, vq0); acc(b, vs1, vq1); acc(c, vs2, vq2);
            }
            योग_2[0] += v_reduce_योग(vs0); योग_2[1] += v_reduce_योग(vs1); योग_2[2] += v_reduce_योग(vs2);
            sqsum[0] += v_reduce_योग(vq0); sqsum[1] += v_reduce_योग(vq1); sqsum[2] += v_reduce_योग(vq2);
            v_cleanup();
            वापस e;
        }
        अगर (cn != 1 && cn != 2 && cn != 4)
            वापस 0;
        len *= cn;

        पूर्णांक x = 0;
        v_int32 v_योग = vx_setzero_s32();
        v_int32 v_sqsum = vx_setzero_s32();

        स्थिर पूर्णांक len0 = len & -VTraits<v_int8>::vlanes();
        जबतक (x < len0)
        {
            स्थिर पूर्णांक len_tmp = min(x + 256 * VTraits<v_int16>::vlanes(), len0);
            v_int16 v_sum16 = vx_setzero_s16();
            क्रम (; x < len_tmp; x += VTraits<v_int8>::vlanes())
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
            v_योग = v_add(v_योग, v_add(v_half0, v_half1));
        }
        अगर (x <= len - VTraits<v_int16>::vlanes())
        {
            v_int16 v_src = vx_load_expand(src0 + x);
            v_int16 v_half = v_combine_ऊपरी(v_src, v_src);

            v_int32 v_tmp0, v_tmp1;
            v_expand(v_add(v_src, v_half), v_tmp0, v_tmp1);
            v_योग = v_add(v_योग, v_tmp0);

            v_int16 v_tmp2, v_tmp3;
            v_zip(v_src, v_half, v_tmp2, v_tmp3);
            v_sqsum = v_add(v_sqsum, v_dotprod(v_tmp2, v_tmp2));
            x += VTraits<v_int16>::vlanes();
        }

        अगर (cn == 1)
        {
            *योग_2 += v_reduce_योग(v_योग);
            *sqsum += v_reduce_योग(v_sqsum);
        }
        अन्यथा
        {
            पूर्णांक CV_DECL_ALIGNED(CV_SIMD_WIDTH) ar[2 * VTraits<v_int32>::max_nlanes];
            v_store(ar, v_योग);
            v_store(ar + VTraits<v_int32>::vlanes(), v_sqsum);
            क्रम (पूर्णांक i = 0; i < VTraits<v_int32>::vlanes(); ++i)
            {
                योग_2[i % cn] += ar[i];
                sqsum[i % cn] += ar[VTraits<v_int32>::vlanes() + i];
            }
        }
        v_cleanup();
        वापस x / cn;
    }
};

#अगर (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)

// Expand a 16-bit vector into two s32 halves. The unsigned overload reinterprets
// after a widening expand (values 0..65535 stay non-negative in s32, so the
// subsequent v_cvt_f64 is exact); the signed overload expands directly. This is
// the only real difference between the ushort and short reductions below.
अटल अंतरभूत व्योम योग_Sqr_Expand_S32(स्थिर v_uint16& v, v_int32& lo, v_int32& hi)
{
    v_uint32 a, b; v_expand(v, a, b);
    lo = v_reinterpret_as_s32(a); hi = v_reinterpret_as_s32(b);
}
अटल अंतरभूत व्योम योग_Sqr_Expand_S32(स्थिर v_int16& v, v_int32& lo, v_int32& hi)
{
    v_expand(v, lo, hi);
}

// Shared 16-bit -> {s32 sum, f64 sqsum} reduction for ushort/short. The register
// type is deduced from the pointer, so the body is identical for both depths.
ढाँचा <typename T>
अटल अंतरभूत पूर्णांक योग_Sqr16_To_F64(स्थिर T* src0, पूर्णांक* योग_2, द्विगुन* sqsum, पूर्णांक len, पूर्णांक cn)
{
    प्रकार decltype(vx_load(src0)) VT; // v_uint16 or v_int16

    अगर (cn == 3)
    {
        स्थिर पूर्णांक vl = VTraits<VT>::vlanes();
        v_int32 vs0 = vx_setzero_s32(), vs1 = vx_setzero_s32(), vs2 = vx_setzero_s32();
        v_float64 vq0 = vx_setzero_f64(), vq1 = vx_setzero_f64(), vq2 = vx_setzero_f64();
        स्वतः acc = [](स्थिर VT& ch, v_int32& vs, v_float64& vq)
        {
            v_int32 lo, hi; योग_Sqr_Expand_S32(ch, lo, hi);
            vs = v_add(vs, v_add(lo, hi));
            v_float64 f0 = v_cvt_f64(lo), f1 = v_cvt_f64_ऊपरी(lo);
            v_float64 f2 = v_cvt_f64(hi), f3 = v_cvt_f64_ऊपरी(hi);
            vq = v_fma(f0, f0, v_fma(f1, f1, v_fma(f2, f2, v_fma(f3, f3, vq))));
        };
        पूर्णांक e = 0;
        क्रम (; e <= len - vl; e += vl)
        {
            VT a, b, c; v_load_deinterleave(src0 + e * 3, a, b, c);
            acc(a, vs0, vq0); acc(b, vs1, vq1); acc(c, vs2, vq2);
        }
        योग_2[0] += v_reduce_योग(vs0); योग_2[1] += v_reduce_योग(vs1); योग_2[2] += v_reduce_योग(vs2);
        sqsum[0] += v_reduce_योग(vq0); sqsum[1] += v_reduce_योग(vq1); sqsum[2] += v_reduce_योग(vq2);
        v_cleanup();
        वापस e;
    }
    अगर (cn != 1 && cn != 2 && cn != 4)
        वापस 0;
    len *= cn;

    स्थिर पूर्णांक vl16 = VTraits<VT>::vlanes();
    स्थिर पूर्णांक vl32 = VTraits<v_int32>::vlanes();
    स्थिर पूर्णांक vl64 = VTraits<v_float64>::vlanes();

    // The lane->channel scatter below (i % cn) and the returned pixel count
    // (x / cn) require the lane counts to be a multiple of cn. This always holds
    // for fixed-width SIMD (vlanes is a power of two >= cn for cn in {1,2,4}),
    // but scalable backends (e.g. RVV) may report a non-multiple; fall back to
    // scalar there. vl16 == 2*vl32, so checking vl32 covers both scatter loops.
    अगर (vl32 % cn != 0)
        वापस 0;

    v_int32 v_योग = vx_setzero_s32();
    v_float64 v_sq0 = vx_setzero_f64(), v_sq1 = vx_setzero_f64();
    v_float64 v_sq2 = vx_setzero_f64(), v_sq3 = vx_setzero_f64();

    पूर्णांक x = 0;
    क्रम (; x <= len - vl16; x += vl16)
    {
        VT v_src = vx_load(src0 + x);
        v_int32 lo, hi; योग_Sqr_Expand_S32(v_src, lo, hi);
        v_योग = v_add(v_योग, v_add(lo, hi));

        v_float64 f0 = v_cvt_f64(lo);
        v_float64 f1 = v_cvt_f64_ऊपरी(lo);
        v_float64 f2 = v_cvt_f64(hi);
        v_float64 f3 = v_cvt_f64_ऊपरी(hi);
        v_sq0 = v_fma(f0, f0, v_sq0);
        v_sq1 = v_fma(f1, f1, v_sq1);
        v_sq2 = v_fma(f2, f2, v_sq2);
        v_sq3 = v_fma(f3, f3, v_sq3);
    }

    पूर्णांक    CV_DECL_ALIGNED(CV_SIMD_WIDTH) sbuf[VTraits<v_int32>::max_nlanes];
    द्विगुन CV_DECL_ALIGNED(CV_SIMD_WIDTH) qbuf[VTraits<v_float64>::max_nlanes * 4];
    v_store_aligned(sbuf, v_योग);
    v_store_aligned(qbuf + vl64 * 0, v_sq0);
    v_store_aligned(qbuf + vl64 * 1, v_sq1);
    v_store_aligned(qbuf + vl64 * 2, v_sq2);
    v_store_aligned(qbuf + vl64 * 3, v_sq3);

    // sbuf lane j (j<vl32) holds samples j and j+vl32 -> channel j%cn (vl32%cn==0).
    // qbuf index i (i<vl16) holds sample lane i           -> channel i%cn (vl16%cn==0).
    क्रम (पूर्णांक i = 0; i < vl32; ++i)
        योग_2[i % cn] += sbuf[i];
    क्रम (पूर्णांक i = 0; i < vl16; ++i)
        sqsum[i % cn] += qbuf[i];

    v_cleanup();
    वापस x / cn;
}

// Shared 32-bit -> f64 reduction for int/float. Both sum and sqsum accumulate in
// f64 to match the scalar reference's double accumulation; values are widened to
// f64 before squaring so the square is computed in double (same as (double)v*v).
// The register type is deduced from the pointer, so int and float share one body.
ढाँचा <typename T>
अटल अंतरभूत पूर्णांक योग_Sqr32_To_F64(स्थिर T* src0, द्विगुन* योग_2, द्विगुन* sqsum, पूर्णांक len, पूर्णांक cn)
{
    प्रकार decltype(vx_load(src0)) VT; // v_int32 or v_float32

    अगर (cn == 3)
    {
        स्थिर पूर्णांक vl = VTraits<VT>::vlanes();
        v_float64 vs0 = vx_setzero_f64(), vs1 = vx_setzero_f64(), vs2 = vx_setzero_f64();
        v_float64 vq0 = vx_setzero_f64(), vq1 = vx_setzero_f64(), vq2 = vx_setzero_f64();
        स्वतः acc = [](स्थिर VT& ch, v_float64& vs, v_float64& vq)
        {
            v_float64 f0 = v_cvt_f64(ch), f1 = v_cvt_f64_ऊपरी(ch);
            vs = v_add(vs, v_add(f0, f1));
            vq = v_fma(f0, f0, v_fma(f1, f1, vq));
        };
        पूर्णांक e = 0;
        क्रम (; e <= len - vl; e += vl)
        {
            VT a, b, c; v_load_deinterleave(src0 + e * 3, a, b, c);
            acc(a, vs0, vq0); acc(b, vs1, vq1); acc(c, vs2, vq2);
        }
        योग_2[0] += v_reduce_योग(vs0); योग_2[1] += v_reduce_योग(vs1); योग_2[2] += v_reduce_योग(vs2);
        sqsum[0] += v_reduce_योग(vq0); sqsum[1] += v_reduce_योग(vq1); sqsum[2] += v_reduce_योग(vq2);
        v_cleanup();
        वापस e;
    }
    अगर (cn != 1 && cn != 2 && cn != 4)
        वापस 0;
    len *= cn;

    स्थिर पूर्णांक vl32 = VTraits<VT>::vlanes();
    स्थिर पूर्णांक vl64 = VTraits<v_float64>::vlanes();

    // See note in sumSqr16ToF64: guard scalable backends whose lane count may
    // not divide cn (the i % cn scatter / x / cn rely on it).
    अगर (vl32 % cn != 0)
        वापस 0;

    v_float64 vs0 = vx_setzero_f64(), vs1 = vx_setzero_f64();
    v_float64 vq0 = vx_setzero_f64(), vq1 = vx_setzero_f64();

    पूर्णांक x = 0;
    क्रम (; x <= len - vl32; x += vl32)
    {
        VT v_src = vx_load(src0 + x);
        v_float64 f0 = v_cvt_f64(v_src);
        v_float64 f1 = v_cvt_f64_ऊपरी(v_src);
        vs0 = v_add(vs0, f0);
        vs1 = v_add(vs1, f1);
        vq0 = v_fma(f0, f0, vq0);
        vq1 = v_fma(f1, f1, vq1);
    }

    द्विगुन CV_DECL_ALIGNED(CV_SIMD_WIDTH) sbuf[VTraits<v_float64>::max_nlanes * 2];
    द्विगुन CV_DECL_ALIGNED(CV_SIMD_WIDTH) qbuf[VTraits<v_float64>::max_nlanes * 2];
    v_store_aligned(sbuf, vs0);          v_store_aligned(sbuf + vl64, vs1);
    v_store_aligned(qbuf, vq0);          v_store_aligned(qbuf + vl64, vq1);

    क्रम (पूर्णांक i = 0; i < vl32; ++i)
    {
        योग_2[i % cn]   += sbuf[i];
        sqsum[i % cn] += qbuf[i];
    }

    v_cleanup();
    वापस x / cn;
}

ढाँचा <>
काष्ठा योग_Sqr_SIMD<ushort, पूर्णांक, द्विगुन>
{
    पूर्णांक चालक () (स्थिर ushort* src0, स्थिर uchar* mask, पूर्णांक* योग_2, द्विगुन* sqsum, पूर्णांक len, पूर्णांक cn) स्थिर
    { वापस mask ? 0 : योग_Sqr16_To_F64(src0, योग_2, sqsum, len, cn); }
};

ढाँचा <>
काष्ठा योग_Sqr_SIMD<लघु, पूर्णांक, द्विगुन>
{
    पूर्णांक चालक () (स्थिर लघु* src0, स्थिर uchar* mask, पूर्णांक* योग_2, द्विगुन* sqsum, पूर्णांक len, पूर्णांक cn) स्थिर
    { वापस mask ? 0 : योग_Sqr16_To_F64(src0, योग_2, sqsum, len, cn); }
};

ढाँचा <>
काष्ठा योग_Sqr_SIMD<पूर्णांक, द्विगुन, द्विगुन>
{
    पूर्णांक चालक () (स्थिर पूर्णांक* src0, स्थिर uchar* mask, द्विगुन* योग_2, द्विगुन* sqsum, पूर्णांक len, पूर्णांक cn) स्थिर
    { वापस mask ? 0 : योग_Sqr32_To_F64(src0, योग_2, sqsum, len, cn); }
};

ढाँचा <>
काष्ठा योग_Sqr_SIMD<भग्न, द्विगुन, द्विगुन>
{
    पूर्णांक चालक () (स्थिर भग्न* src0, स्थिर uchar* mask, द्विगुन* योग_2, द्विगुन* sqsum, पूर्णांक len, पूर्णांक cn) स्थिर
    { वापस mask ? 0 : योग_Sqr32_To_F64(src0, योग_2, sqsum, len, cn); }
};

#पूर्ण_अगर // CV_SIMD_64F

#पूर्ण_अगर

ढाँचा<typename T, typename ST, typename SQT>
अटल पूर्णांक sumsqr_(स्थिर T* src0, स्थिर uchar* mask, ST* योग_2, SQT* sqsum, पूर्णांक len, पूर्णांक cn )
{
    स्थिर T* src = src0;

    अगर( !mask )
    {
        योग_Sqr_SIMD<T, ST, SQT> vop;
        पूर्णांक x = vop(src0, mask, योग_2, sqsum, len, cn), k = cn % 4;
        src = src0 + x * cn;

        अगर( k == 1 )
        {
            ST s0 = योग_2[0];
            SQT sq0 = sqsum[0];
            क्रम(पूर्णांक i = x; i < len; i++, src += cn )
            {
                ST v = (ST)src[0];
                s0 += v; sq0 += (SQT)v*v;
            }
            योग_2[0] = s0;
            sqsum[0] = sq0;
        }
        अन्यथा अगर( k == 2 )
        {
            ST s0 = योग_2[0], s1 = योग_2[1];
            SQT sq0 = sqsum[0], sq1 = sqsum[1];
            क्रम(पूर्णांक i = x; i < len; i++, src += cn )
            {
                ST v0 = (ST)src[0], v1 = (ST)src[1];
                s0 += v0; sq0 += (SQT)v0*v0;
                s1 += v1; sq1 += (SQT)v1*v1;
            }
            योग_2[0] = s0; योग_2[1] = s1;
            sqsum[0] = sq0; sqsum[1] = sq1;
        }
        अन्यथा अगर( k == 3 )
        {
            ST s0 = योग_2[0], s1 = योग_2[1], s2 = योग_2[2];
            SQT sq0 = sqsum[0], sq1 = sqsum[1], sq2 = sqsum[2];
            क्रम(पूर्णांक i = x; i < len; i++, src += cn )
            {
                ST v0 = (ST)src[0], v1 = (ST)src[1], v2 = (ST)src[2];
                s0 += v0; sq0 += (SQT)v0*v0;
                s1 += v1; sq1 += (SQT)v1*v1;
                s2 += v2; sq2 += (SQT)v2*v2;
            }
            योग_2[0] = s0; योग_2[1] = s1; योग_2[2] = s2;
            sqsum[0] = sq0; sqsum[1] = sq1; sqsum[2] = sq2;
        }

        क्रम( ; k < cn; k += 4 )
        {
            src = src0 + x * cn + k;
            ST s0 = योग_2[k], s1 = योग_2[k+1], s2 = योग_2[k+2], s3 = योग_2[k+3];
            SQT sq0 = sqsum[k], sq1 = sqsum[k+1], sq2 = sqsum[k+2], sq3 = sqsum[k+3];
            क्रम(पूर्णांक i = x; i < len; i++, src += cn )
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
            योग_2[k] = s0; योग_2[k+1] = s1;
            योग_2[k+2] = s2; योग_2[k+3] = s3;
            sqsum[k] = sq0; sqsum[k+1] = sq1;
            sqsum[k+2] = sq2; sqsum[k+3] = sq3;
        }
        वापस len;
    }

    पूर्णांक i, nzm = 0;

    अगर( cn == 1 )
    {
        ST s0 = योग_2[0];
        SQT sq0 = sqsum[0];
        क्रम( i = 0; i < len; i++ )
            अगर( mask[i] )
            {
                ST v = (ST)src[i];
                s0 += v; sq0 += (SQT)v*v;
                nzm++;
            }
        योग_2[0] = s0;
        sqsum[0] = sq0;
    }
    अन्यथा अगर( cn == 2 )
    {
        ST s0 = योग_2[0], s1 = योग_2[1];
        SQT sq0 = sqsum[0], sq1 = sqsum[1];
        क्रम( i = 0; i < len; i++, src += 2 )
            अगर( mask[i] )
            {
                T v0 = src[0], v1 = src[1];
                s0 += v0; sq0 += (SQT)v0*v0;
                s1 += v1; sq1 += (SQT)v1*v1;
                nzm++;
            }
        योग_2[0] = s0; योग_2[1] = s1;
        sqsum[0] = sq0; sqsum[1] = sq1;
    }
    अन्यथा अगर( cn == 3 )
    {
        ST s0 = योग_2[0], s1 = योग_2[1], s2 = योग_2[2];
        SQT sq0 = sqsum[0], sq1 = sqsum[1], sq2 = sqsum[2];
        क्रम( i = 0; i < len; i++, src += 3 )
            अगर( mask[i] )
            {
                ST v0 = (ST)src[0], v1 = (ST)src[1], v2 = (ST)src[2];
                s0 += v0; sq0 += (SQT)v0*v0;
                s1 += v1; sq1 += (SQT)v1*v1;
                s2 += v2; sq2 += (SQT)v2*v2;
                nzm++;
            }
        योग_2[0] = s0; योग_2[1] = s1; योग_2[2] = s2;
        sqsum[0] = sq0; sqsum[1] = sq1; sqsum[2] = sq2;
    }
    अन्यथा अगर( cn == 4 )
    {
        ST s0 = योग_2[0], s1 = योग_2[1], s2 = योग_2[2], s3 = योग_2[3];
        SQT sq0 = sqsum[0], sq1 = sqsum[1], sq2 = sqsum[2], sq3 = sqsum[3];
        क्रम( i = 0; i < len; i++, src += 4 )
            अगर( mask[i] )
            {
                T v0 = src[0], v1 = src[1], v2 = src[2], v3 = src[3];
                s0 += v0; sq0 += (SQT)v0*v0;
                s1 += v1; sq1 += (SQT)v1*v1;
                s2 += v2; sq2 += (SQT)v2*v2;
                s3 += v3; sq3 += (SQT)v3*v3;
                nzm++;
            }
        योग_2[0] = s0; योग_2[1] = s1; योग_2[2] = s2; योग_2[3] = s3;
        sqsum[0] = sq0; sqsum[1] = sq1; sqsum[2] = sq2; sqsum[3] = sq3;
    }
    अन्यथा
    {
        क्रम( i = 0; i < len; i++, src += cn )
            अगर( mask[i] )
            {
                क्रम( पूर्णांक k = 0; k < cn; k++ )
                {
                    ST v = (ST)src[k];
                    ST s = योग_2[k] + v;
                    SQT sq = sqsum[k] + (SQT)v*v;
                    योग_2[k] = s; sqsum[k] = sq;
                }
                nzm++;
            }
    }
    वापस nzm;
}


अटल पूर्णांक sqsum8u( स्थिर uchar* src, स्थिर uchar* mask, पूर्णांक* योग_2, पूर्णांक* sqsum, पूर्णांक len, पूर्णांक cn )
{ CV_INSTRUMENT_REGION(); वापस sumsqr_(src, mask, योग_2, sqsum, len, cn); }

अटल पूर्णांक sqsum8s( स्थिर schar* src, स्थिर uchar* mask, पूर्णांक* योग_2, पूर्णांक* sqsum, पूर्णांक len, पूर्णांक cn )
{ CV_INSTRUMENT_REGION(); वापस sumsqr_(src, mask, योग_2, sqsum, len, cn); }

अटल पूर्णांक sqsum16u( स्थिर ushort* src, स्थिर uchar* mask, पूर्णांक* योग_2, द्विगुन* sqsum, पूर्णांक len, पूर्णांक cn )
{ CV_INSTRUMENT_REGION(); वापस sumsqr_(src, mask, योग_2, sqsum, len, cn); }

अटल पूर्णांक sqsum16s( स्थिर लघु* src, स्थिर uchar* mask, पूर्णांक* योग_2, द्विगुन* sqsum, पूर्णांक len, पूर्णांक cn )
{ CV_INSTRUMENT_REGION(); वापस sumsqr_(src, mask, योग_2, sqsum, len, cn); }

अटल पूर्णांक sqsum32s( स्थिर पूर्णांक* src, स्थिर uchar* mask, द्विगुन* योग_2, द्विगुन* sqsum, पूर्णांक len, पूर्णांक cn )
{ CV_INSTRUMENT_REGION(); वापस sumsqr_(src, mask, योग_2, sqsum, len, cn); }

अटल पूर्णांक sqsum32f( स्थिर भग्न* src, स्थिर uchar* mask, द्विगुन* योग_2, द्विगुन* sqsum, पूर्णांक len, पूर्णांक cn )
{ CV_INSTRUMENT_REGION(); वापस sumsqr_(src, mask, योग_2, sqsum, len, cn); }

अटल पूर्णांक sqsum64f( स्थिर द्विगुन* src, स्थिर uchar* mask, द्विगुन* योग_2, द्विगुन* sqsum, पूर्णांक len, पूर्णांक cn )
{ CV_INSTRUMENT_REGION(); वापस sumsqr_(src, mask, योग_2, sqsum, len, cn); }

अटल पूर्णांक sqsum16f( स्थिर hfloat* src, स्थिर uchar* mask, भग्न* योग_2, द्विगुन* sqsum, पूर्णांक len, पूर्णांक cn )
{ CV_INSTRUMENT_REGION(); वापस sumsqr_(src, mask, योग_2, sqsum, len, cn); }

अटल पूर्णांक sqsum16bf( स्थिर bfloat* src, स्थिर uchar* mask, भग्न* योग_2, द्विगुन* sqsum, पूर्णांक len, पूर्णांक cn )
{ CV_INSTRUMENT_REGION(); वापस sumsqr_(src, mask, योग_2, sqsum, len, cn); }

अटल पूर्णांक sqsum64u( स्थिर uint64* src, स्थिर uchar* mask, द्विगुन* योग_2, द्विगुन* sqsum, पूर्णांक len, पूर्णांक cn )
{ CV_INSTRUMENT_REGION(); वापस sumsqr_(src, mask, योग_2, sqsum, len, cn); }

अटल पूर्णांक sqsum64s( स्थिर int64* src, स्थिर uchar* mask, द्विगुन* योग_2, द्विगुन* sqsum, पूर्णांक len, पूर्णांक cn )
{ CV_INSTRUMENT_REGION(); वापस sumsqr_(src, mask, योग_2, sqsum, len, cn); }

अटल पूर्णांक sqsum32u( स्थिर अचिन्हित* src, स्थिर uchar* mask, द्विगुन* योग_2, द्विगुन* sqsum, पूर्णांक len, पूर्णांक cn )
{ CV_INSTRUMENT_REGION(); वापस sumsqr_(src, mask, योग_2, sqsum, len, cn); }

योग_Sqr_Func get_योग_Sqr_Func(पूर्णांक depth)
{
    CV_INSTRUMENT_REGION();
    अटल योग_Sqr_Func योग_Sqr_Tab[CV_DEPTH_MAX] =
    {
        योग_Sqr_Wrap<uchar, पूर्णांक, पूर्णांक, sqsum8u>, योग_Sqr_Wrap<schar, पूर्णांक, पूर्णांक, sqsum8s>,
        योग_Sqr_Wrap<ushort, पूर्णांक, द्विगुन, sqsum16u>, योग_Sqr_Wrap<लघु, पूर्णांक, द्विगुन, sqsum16s>,
        योग_Sqr_Wrap<पूर्णांक, द्विगुन, द्विगुन, sqsum32s>, योग_Sqr_Wrap<भग्न, द्विगुन, द्विगुन, sqsum32f>,
        योग_Sqr_Wrap<द्विगुन, द्विगुन, द्विगुन, sqsum64f>,
        योग_Sqr_Wrap<hfloat, भग्न, द्विगुन, sqsum16f>, योग_Sqr_Wrap<bfloat, भग्न, द्विगुन, sqsum16bf>, 0,
        योग_Sqr_Wrap<uint64, द्विगुन, द्विगुन, sqsum64u>, योग_Sqr_Wrap<int64, द्विगुन, द्विगुन, sqsum64s>,
        योग_Sqr_Wrap<अचिन्हित, द्विगुन, द्विगुन, sqsum32u>, 0
    };

    वापस योग_Sqr_Tab[depth];
}

#पूर्ण_अगर
CV_CPU_OPTIMIZATION_NAMESPACE_END
} // namespace
