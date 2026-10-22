// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html

// Copyright (C) 2026, Advanced Micro Devices, Inc., all rights reserved.

#अन्तर्भाव "precomp.hpp"
#अन्तर्भाव "stat.hpp"

नामाकाश cv {
प्रकारनिर्देश पूर्णाङ्क (*योग_Sqr_Func)(ध्रुव uchar*, ध्रुव uchar* आवरणम्, uchar*, uchar*, पूर्णाङ्क, पूर्णाङ्क);

CV_CPU_OPTIMIZATION_NAMESPACE_BEGIN

प्रतिरूपम्<प्रकारनाम T, प्रकारनाम ST, प्रकारनाम SQT, पूर्णाङ्क (*fn)(ध्रुव T*, ध्रुव uchar*, ST*, SQT*, पूर्णाङ्क, पूर्णाङ्क)>
स्थिर पूर्णाङ्क योग_Sqr_Wrap(ध्रुव uchar* src, ध्रुव uchar* आवरणम्, uchar* योग_2, uchar* sqsum, पूर्णाङ्क len, पूर्णाङ्क cn)
{
    प्रत्यागम fn((ध्रुव T*)src, आवरणम्, (ST*)योग_2, (SQT*)sqsum, len, cn);
}

योग_Sqr_Func get_योग_Sqr_Func(पूर्णाङ्क गभीरता);

#यदि_अपरिभाषितम् CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

प्रतिरूपम् <प्रकारनाम T, प्रकारनाम ST, प्रकारनाम SQT>
संरचना योग_Sqr_SIMD
{
    अन्तःस्थ पूर्णाङ्क संक्रियकः () (ध्रुव T *, ध्रुव uchar *, ST *, SQT *, पूर्णाङ्क, पूर्णाङ्क) ध्रुव
    {
        प्रत्यागम 0;
    }
};

#यदि (CV_SIMD || CV_SIMD_SCALABLE)

प्रतिरूपम् <>
संरचना योग_Sqr_SIMD<uchar, पूर्णाङ्क, पूर्णाङ्क>
{
    पूर्णाङ्क संक्रियकः () (ध्रुव uchar * src0, ध्रुव uchar * आवरणम्, पूर्णाङ्क * योग_2, पूर्णाङ्क * sqsum, पूर्णाङ्क len, पूर्णाङ्क cn) ध्रुव
    {
        यदि (आवरणम्)
            प्रत्यागम 0;
        // cn==3: deinterleave so each lane belongs to one channel, then reduce per
        // channel. sum/sqsum stay exact in s32 within the dispatcher's 1<<15 block.
        यदि (cn == 3)
        {
            ध्रुव पूर्णाङ्क vl8 = VTraits<v_uint8>::vlanes();
            v_int32 vs0 = vx_setzero_s32(), vs1 = vx_setzero_s32(), vs2 = vx_setzero_s32();
            v_int32 vq0 = vx_setzero_s32(), vq1 = vx_setzero_s32(), vq2 = vx_setzero_s32();
            स्वचालित acc = [](ध्रुव v_uint8& ch, v_int32& vs, v_int32& vq)
            {
                v_uint16 lo, hi; v_expand(ch, lo, hi);
                v_uint32 s0, s1, s2, s3; v_expand(lo, s0, s1); v_expand(hi, s2, s3);
                vs = v_सङ्कलय(vs, v_reinterpret_as_s32(v_सङ्कलय(v_सङ्कलय(s0, s1), v_सङ्कलय(s2, s3))));
                v_int16 d0 = v_reinterpret_as_s16(lo), d1 = v_reinterpret_as_s16(hi);
                vq = v_सङ्कलय(vq, v_सङ्कलय(v_dotprod(d0, d0), v_dotprod(d1, d1)));
            };
            पूर्णाङ्क e = 0;
            प्रति (; e <= len - vl8; e += vl8)
            {
                v_uint8 a, b, c; v_आरोपय_deinterleave(src0 + e * 3, a, b, c);
                acc(a, vs0, vq0); acc(b, vs1, vq1); acc(c, vs2, vq2);
            }
            योग_2[0] += v_reduce_योग(vs0); योग_2[1] += v_reduce_योग(vs1); योग_2[2] += v_reduce_योग(vs2);
            sqsum[0] += v_reduce_योग(vq0); sqsum[1] += v_reduce_योग(vq1); sqsum[2] += v_reduce_योग(vq2);
            v_cleanup();
            प्रत्यागम e;
        }
        यदि (cn != 1 && cn != 2 && cn != 4)
            प्रत्यागम 0;
        len *= cn;

        पूर्णाङ्क x = 0;
        v_int32 v_योग = vx_setzero_s32();
        v_int32 v_sqsum = vx_setzero_s32();

        ध्रुव पूर्णाङ्क len0 = len & -VTraits<v_uint8>::vlanes();
        यावत्(x < len0)
        {
            ध्रुव पूर्णाङ्क len_tmp = min(x + 256*VTraits<v_uint16>::vlanes(), len0);
            v_uint16 v_sum16 = vx_setzero_u16();
            प्रति ( ; x < len_tmp; x += VTraits<v_uint8>::vlanes())
            {
                v_uint16 v_src0 = vx_आरोपय_expand(src0 + x);
                v_uint16 v_src1 = vx_आरोपय_expand(src0 + x + VTraits<v_uint16>::vlanes());
                v_sum16 = v_सङ्कलय(v_sum16, v_सङ्कलय(v_src0, v_src1));
                v_int16 v_tmp0, v_tmp1;
                v_zip(v_reinterpret_as_s16(v_src0), v_reinterpret_as_s16(v_src1), v_tmp0, v_tmp1);
                v_sqsum = v_सङ्कलय(v_sqsum, v_सङ्कलय(v_dotprod(v_tmp0, v_tmp0), v_dotprod(v_tmp1, v_tmp1)));
            }
            v_uint32 v_half0, v_half1;
            v_expand(v_sum16, v_half0, v_half1);
            v_योग = v_सङ्कलय(v_योग, v_reinterpret_as_s32(v_सङ्कलय(v_half0, v_half1)));
        }
        यदि (x <= len - VTraits<v_uint16>::vlanes())
        {
            v_uint16 v_src = vx_आरोपय_expand(src0 + x);
            v_uint16 v_half = v_combine_उच्चः(v_src, v_src);

            v_uint32 v_tmp0, v_tmp1;
            v_expand(v_सङ्कलय(v_src, v_half), v_tmp0, v_tmp1);
            v_योग = v_सङ्कलय(v_योग, v_reinterpret_as_s32(v_tmp0));

            v_int16 v_tmp2, v_tmp3;
            v_zip(v_reinterpret_as_s16(v_src), v_reinterpret_as_s16(v_half), v_tmp2, v_tmp3);
            v_sqsum = v_सङ्कलय(v_sqsum, v_dotprod(v_tmp2, v_tmp2));
            x += VTraits<v_uint16>::vlanes();
        }

        यदि (cn == 1)
        {
            *योग_2 += v_reduce_योग(v_योग);
            *sqsum += v_reduce_योग(v_sqsum);
        }
        अन्यथा
        {
            पूर्णाङ्क CV_DECL_ALIGNED(CV_SIMD_विस्तारः) ar[2 * VTraits<v_int32>::max_nlanes];
            v_store(ar, v_योग);
            v_store(ar + VTraits<v_int32>::vlanes(), v_sqsum);
            प्रति (पूर्णाङ्क i = 0; i < VTraits<v_int32>::vlanes(); ++i)
            {
                योग_2[i % cn] += ar[i];
                sqsum[i % cn] += ar[VTraits<v_int32>::vlanes() + i];
            }
        }
        v_cleanup();
        प्रत्यागम x / cn;
    }
};

प्रतिरूपम् <>
संरचना योग_Sqr_SIMD<schar, पूर्णाङ्क, पूर्णाङ्क>
{
    पूर्णाङ्क संक्रियकः () (ध्रुव schar * src0, ध्रुव uchar * आवरणम्, पूर्णाङ्क * योग_2, पूर्णाङ्क * sqsum, पूर्णाङ्क len, पूर्णाङ्क cn) ध्रुव
    {
        यदि (आवरणम्)
            प्रत्यागम 0;
        यदि (cn == 3)
        {
            ध्रुव पूर्णाङ्क vl8 = VTraits<v_int8>::vlanes();
            v_int32 vs0 = vx_setzero_s32(), vs1 = vx_setzero_s32(), vs2 = vx_setzero_s32();
            v_int32 vq0 = vx_setzero_s32(), vq1 = vx_setzero_s32(), vq2 = vx_setzero_s32();
            स्वचालित acc = [](ध्रुव v_int8& ch, v_int32& vs, v_int32& vq)
            {
                v_int16 lo, hi; v_expand(ch, lo, hi);
                v_int32 s0, s1, s2, s3; v_expand(lo, s0, s1); v_expand(hi, s2, s3);
                vs = v_सङ्कलय(vs, v_सङ्कलय(v_सङ्कलय(s0, s1), v_सङ्कलय(s2, s3)));
                vq = v_सङ्कलय(vq, v_सङ्कलय(v_dotprod(lo, lo), v_dotprod(hi, hi)));
            };
            पूर्णाङ्क e = 0;
            प्रति (; e <= len - vl8; e += vl8)
            {
                v_int8 a, b, c; v_आरोपय_deinterleave(src0 + e * 3, a, b, c);
                acc(a, vs0, vq0); acc(b, vs1, vq1); acc(c, vs2, vq2);
            }
            योग_2[0] += v_reduce_योग(vs0); योग_2[1] += v_reduce_योग(vs1); योग_2[2] += v_reduce_योग(vs2);
            sqsum[0] += v_reduce_योग(vq0); sqsum[1] += v_reduce_योग(vq1); sqsum[2] += v_reduce_योग(vq2);
            v_cleanup();
            प्रत्यागम e;
        }
        यदि (cn != 1 && cn != 2 && cn != 4)
            प्रत्यागम 0;
        len *= cn;

        पूर्णाङ्क x = 0;
        v_int32 v_योग = vx_setzero_s32();
        v_int32 v_sqsum = vx_setzero_s32();

        ध्रुव पूर्णाङ्क len0 = len & -VTraits<v_int8>::vlanes();
        यावत् (x < len0)
        {
            ध्रुव पूर्णाङ्क len_tmp = min(x + 256 * VTraits<v_int16>::vlanes(), len0);
            v_int16 v_sum16 = vx_setzero_s16();
            प्रति (; x < len_tmp; x += VTraits<v_int8>::vlanes())
            {
                v_int16 v_src0 = vx_आरोपय_expand(src0 + x);
                v_int16 v_src1 = vx_आरोपय_expand(src0 + x + VTraits<v_int16>::vlanes());
                v_sum16 = v_सङ्कलय(v_sum16, v_सङ्कलय(v_src0, v_src1));
                v_int16 v_tmp0, v_tmp1;
                v_zip(v_src0, v_src1, v_tmp0, v_tmp1);
                v_sqsum = v_सङ्कलय(v_sqsum, v_सङ्कलय(v_dotprod(v_tmp0, v_tmp0), v_dotprod(v_tmp1, v_tmp1)));
            }
            v_int32 v_half0, v_half1;
            v_expand(v_sum16, v_half0, v_half1);
            v_योग = v_सङ्कलय(v_योग, v_सङ्कलय(v_half0, v_half1));
        }
        यदि (x <= len - VTraits<v_int16>::vlanes())
        {
            v_int16 v_src = vx_आरोपय_expand(src0 + x);
            v_int16 v_half = v_combine_उच्चः(v_src, v_src);

            v_int32 v_tmp0, v_tmp1;
            v_expand(v_सङ्कलय(v_src, v_half), v_tmp0, v_tmp1);
            v_योग = v_सङ्कलय(v_योग, v_tmp0);

            v_int16 v_tmp2, v_tmp3;
            v_zip(v_src, v_half, v_tmp2, v_tmp3);
            v_sqsum = v_सङ्कलय(v_sqsum, v_dotprod(v_tmp2, v_tmp2));
            x += VTraits<v_int16>::vlanes();
        }

        यदि (cn == 1)
        {
            *योग_2 += v_reduce_योग(v_योग);
            *sqsum += v_reduce_योग(v_sqsum);
        }
        अन्यथा
        {
            पूर्णाङ्क CV_DECL_ALIGNED(CV_SIMD_विस्तारः) ar[2 * VTraits<v_int32>::max_nlanes];
            v_store(ar, v_योग);
            v_store(ar + VTraits<v_int32>::vlanes(), v_sqsum);
            प्रति (पूर्णाङ्क i = 0; i < VTraits<v_int32>::vlanes(); ++i)
            {
                योग_2[i % cn] += ar[i];
                sqsum[i % cn] += ar[VTraits<v_int32>::vlanes() + i];
            }
        }
        v_cleanup();
        प्रत्यागम x / cn;
    }
};

#यदि (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)

// Expand a 16-bit vector into two s32 halves. The unsigned overload reinterprets
// after a widening expand (values 0..65535 stay non-negative in s32, so the
// subsequent v_cvt_f64 is exact); the signed overload expands directly. This is
// the only real difference between the ushort and short reductions below.
स्थिर अन्तःस्थ शून्य योग_Sqr_Expand_S32(ध्रुव v_uint16& v, v_int32& lo, v_int32& hi)
{
    v_uint32 a, b; v_expand(v, a, b);
    lo = v_reinterpret_as_s32(a); hi = v_reinterpret_as_s32(b);
}
स्थिर अन्तःस्थ शून्य योग_Sqr_Expand_S32(ध्रुव v_int16& v, v_int32& lo, v_int32& hi)
{
    v_expand(v, lo, hi);
}

// Shared 16-bit -> {s32 sum, f64 sqsum} reduction for ushort/short. The register
// type is deduced from the pointer, so the body is identical for both depths.
प्रतिरूपम् <प्रकारनाम T>
स्थिर अन्तःस्थ पूर्णाङ्क योग_Sqr16_गन्तव्यम्_F64(ध्रुव T* src0, पूर्णाङ्क* योग_2, द्विगुण* sqsum, पूर्णाङ्क len, पूर्णाङ्क cn)
{
    प्रकारनिर्देश decltype(vx_आरोपय(src0)) VT; // v_uint16 or v_int16

    यदि (cn == 3)
    {
        ध्रुव पूर्णाङ्क vl = VTraits<VT>::vlanes();
        v_int32 vs0 = vx_setzero_s32(), vs1 = vx_setzero_s32(), vs2 = vx_setzero_s32();
        v_float64 vq0 = vx_setzero_f64(), vq1 = vx_setzero_f64(), vq2 = vx_setzero_f64();
        स्वचालित acc = [](ध्रुव VT& ch, v_int32& vs, v_float64& vq)
        {
            v_int32 lo, hi; योग_Sqr_Expand_S32(ch, lo, hi);
            vs = v_सङ्कलय(vs, v_सङ्कलय(lo, hi));
            v_float64 f0 = v_cvt_f64(lo), f1 = v_cvt_f64_उच्चः(lo);
            v_float64 f2 = v_cvt_f64(hi), f3 = v_cvt_f64_उच्चः(hi);
            vq = v_fma(f0, f0, v_fma(f1, f1, v_fma(f2, f2, v_fma(f3, f3, vq))));
        };
        पूर्णाङ्क e = 0;
        प्रति (; e <= len - vl; e += vl)
        {
            VT a, b, c; v_आरोपय_deinterleave(src0 + e * 3, a, b, c);
            acc(a, vs0, vq0); acc(b, vs1, vq1); acc(c, vs2, vq2);
        }
        योग_2[0] += v_reduce_योग(vs0); योग_2[1] += v_reduce_योग(vs1); योग_2[2] += v_reduce_योग(vs2);
        sqsum[0] += v_reduce_योग(vq0); sqsum[1] += v_reduce_योग(vq1); sqsum[2] += v_reduce_योग(vq2);
        v_cleanup();
        प्रत्यागम e;
    }
    यदि (cn != 1 && cn != 2 && cn != 4)
        प्रत्यागम 0;
    len *= cn;

    ध्रुव पूर्णाङ्क vl16 = VTraits<VT>::vlanes();
    ध्रुव पूर्णाङ्क vl32 = VTraits<v_int32>::vlanes();
    ध्रुव पूर्णाङ्क vl64 = VTraits<v_float64>::vlanes();

    // The lane->channel scatter below (i % cn) and the returned pixel count
    // (x / cn) require the lane counts to be a multiple of cn. This always holds
    // for fixed-width SIMD (vlanes is a power of two >= cn for cn in {1,2,4}),
    // but scalable backends (e.g. RVV) may report a non-multiple; fall back to
    // scalar there. vl16 == 2*vl32, so checking vl32 covers both scatter loops.
    यदि (vl32 % cn != 0)
        प्रत्यागम 0;

    v_int32 v_योग = vx_setzero_s32();
    v_float64 v_sq0 = vx_setzero_f64(), v_sq1 = vx_setzero_f64();
    v_float64 v_sq2 = vx_setzero_f64(), v_sq3 = vx_setzero_f64();

    पूर्णाङ्क x = 0;
    प्रति (; x <= len - vl16; x += vl16)
    {
        VT v_src = vx_आरोपय(src0 + x);
        v_int32 lo, hi; योग_Sqr_Expand_S32(v_src, lo, hi);
        v_योग = v_सङ्कलय(v_योग, v_सङ्कलय(lo, hi));

        v_float64 f0 = v_cvt_f64(lo);
        v_float64 f1 = v_cvt_f64_उच्चः(lo);
        v_float64 f2 = v_cvt_f64(hi);
        v_float64 f3 = v_cvt_f64_उच्चः(hi);
        v_sq0 = v_fma(f0, f0, v_sq0);
        v_sq1 = v_fma(f1, f1, v_sq1);
        v_sq2 = v_fma(f2, f2, v_sq2);
        v_sq3 = v_fma(f3, f3, v_sq3);
    }

    पूर्णाङ्क    CV_DECL_ALIGNED(CV_SIMD_विस्तारः) sbuf[VTraits<v_int32>::max_nlanes];
    द्विगुण CV_DECL_ALIGNED(CV_SIMD_विस्तारः) qbuf[VTraits<v_float64>::max_nlanes * 4];
    v_store_aligned(sbuf, v_योग);
    v_store_aligned(qbuf + vl64 * 0, v_sq0);
    v_store_aligned(qbuf + vl64 * 1, v_sq1);
    v_store_aligned(qbuf + vl64 * 2, v_sq2);
    v_store_aligned(qbuf + vl64 * 3, v_sq3);

    // sbuf lane j (j<vl32) holds samples j and j+vl32 -> channel j%cn (vl32%cn==0).
    // qbuf index i (i<vl16) holds sample lane i           -> channel i%cn (vl16%cn==0).
    प्रति (पूर्णाङ्क i = 0; i < vl32; ++i)
        योग_2[i % cn] += sbuf[i];
    प्रति (पूर्णाङ्क i = 0; i < vl16; ++i)
        sqsum[i % cn] += qbuf[i];

    v_cleanup();
    प्रत्यागम x / cn;
}

// Shared 32-bit -> f64 reduction for int/float. Both sum and sqsum accumulate in
// f64 to match the scalar reference's double accumulation; values are widened to
// f64 before squaring so the square is computed in double (same as (double)v*v).
// The register type is deduced from the pointer, so int and float share one body.
प्रतिरूपम् <प्रकारनाम T>
स्थिर अन्तःस्थ पूर्णाङ्क योग_Sqr32_गन्तव्यम्_F64(ध्रुव T* src0, द्विगुण* योग_2, द्विगुण* sqsum, पूर्णाङ्क len, पूर्णाङ्क cn)
{
    प्रकारनिर्देश decltype(vx_आरोपय(src0)) VT; // v_int32 or v_float32

    यदि (cn == 3)
    {
        ध्रुव पूर्णाङ्क vl = VTraits<VT>::vlanes();
        v_float64 vs0 = vx_setzero_f64(), vs1 = vx_setzero_f64(), vs2 = vx_setzero_f64();
        v_float64 vq0 = vx_setzero_f64(), vq1 = vx_setzero_f64(), vq2 = vx_setzero_f64();
        स्वचालित acc = [](ध्रुव VT& ch, v_float64& vs, v_float64& vq)
        {
            v_float64 f0 = v_cvt_f64(ch), f1 = v_cvt_f64_उच्चः(ch);
            vs = v_सङ्कलय(vs, v_सङ्कलय(f0, f1));
            vq = v_fma(f0, f0, v_fma(f1, f1, vq));
        };
        पूर्णाङ्क e = 0;
        प्रति (; e <= len - vl; e += vl)
        {
            VT a, b, c; v_आरोपय_deinterleave(src0 + e * 3, a, b, c);
            acc(a, vs0, vq0); acc(b, vs1, vq1); acc(c, vs2, vq2);
        }
        योग_2[0] += v_reduce_योग(vs0); योग_2[1] += v_reduce_योग(vs1); योग_2[2] += v_reduce_योग(vs2);
        sqsum[0] += v_reduce_योग(vq0); sqsum[1] += v_reduce_योग(vq1); sqsum[2] += v_reduce_योग(vq2);
        v_cleanup();
        प्रत्यागम e;
    }
    यदि (cn != 1 && cn != 2 && cn != 4)
        प्रत्यागम 0;
    len *= cn;

    ध्रुव पूर्णाङ्क vl32 = VTraits<VT>::vlanes();
    ध्रुव पूर्णाङ्क vl64 = VTraits<v_float64>::vlanes();

    // See note in sumSqr16ToF64: guard scalable backends whose lane count may
    // not divide cn (the i % cn scatter / x / cn rely on it).
    यदि (vl32 % cn != 0)
        प्रत्यागम 0;

    v_float64 vs0 = vx_setzero_f64(), vs1 = vx_setzero_f64();
    v_float64 vq0 = vx_setzero_f64(), vq1 = vx_setzero_f64();

    पूर्णाङ्क x = 0;
    प्रति (; x <= len - vl32; x += vl32)
    {
        VT v_src = vx_आरोपय(src0 + x);
        v_float64 f0 = v_cvt_f64(v_src);
        v_float64 f1 = v_cvt_f64_उच्चः(v_src);
        vs0 = v_सङ्कलय(vs0, f0);
        vs1 = v_सङ्कलय(vs1, f1);
        vq0 = v_fma(f0, f0, vq0);
        vq1 = v_fma(f1, f1, vq1);
    }

    द्विगुण CV_DECL_ALIGNED(CV_SIMD_विस्तारः) sbuf[VTraits<v_float64>::max_nlanes * 2];
    द्विगुण CV_DECL_ALIGNED(CV_SIMD_विस्तारः) qbuf[VTraits<v_float64>::max_nlanes * 2];
    v_store_aligned(sbuf, vs0);          v_store_aligned(sbuf + vl64, vs1);
    v_store_aligned(qbuf, vq0);          v_store_aligned(qbuf + vl64, vq1);

    प्रति (पूर्णाङ्क i = 0; i < vl32; ++i)
    {
        योग_2[i % cn]   += sbuf[i];
        sqsum[i % cn] += qbuf[i];
    }

    v_cleanup();
    प्रत्यागम x / cn;
}

प्रतिरूपम् <>
संरचना योग_Sqr_SIMD<ushort, पूर्णाङ्क, द्विगुण>
{
    पूर्णाङ्क संक्रियकः () (ध्रुव ushort* src0, ध्रुव uchar* आवरणम्, पूर्णाङ्क* योग_2, द्विगुण* sqsum, पूर्णाङ्क len, पूर्णाङ्क cn) ध्रुव
    { प्रत्यागम आवरणम् ? 0 : योग_Sqr16_गन्तव्यम्_F64(src0, योग_2, sqsum, len, cn); }
};

प्रतिरूपम् <>
संरचना योग_Sqr_SIMD<ह्रस्व, पूर्णाङ्क, द्विगुण>
{
    पूर्णाङ्क संक्रियकः () (ध्रुव ह्रस्व* src0, ध्रुव uchar* आवरणम्, पूर्णाङ्क* योग_2, द्विगुण* sqsum, पूर्णाङ्क len, पूर्णाङ्क cn) ध्रुव
    { प्रत्यागम आवरणम् ? 0 : योग_Sqr16_गन्तव्यम्_F64(src0, योग_2, sqsum, len, cn); }
};

प्रतिरूपम् <>
संरचना योग_Sqr_SIMD<पूर्णाङ्क, द्विगुण, द्विगुण>
{
    पूर्णाङ्क संक्रियकः () (ध्रुव पूर्णाङ्क* src0, ध्रुव uchar* आवरणम्, द्विगुण* योग_2, द्विगुण* sqsum, पूर्णाङ्क len, पूर्णाङ्क cn) ध्रुव
    { प्रत्यागम आवरणम् ? 0 : योग_Sqr32_गन्तव्यम्_F64(src0, योग_2, sqsum, len, cn); }
};

प्रतिरूपम् <>
संरचना योग_Sqr_SIMD<भिन्न, द्विगुण, द्विगुण>
{
    पूर्णाङ्क संक्रियकः () (ध्रुव भिन्न* src0, ध्रुव uchar* आवरणम्, द्विगुण* योग_2, द्विगुण* sqsum, पूर्णाङ्क len, पूर्णाङ्क cn) ध्रुव
    { प्रत्यागम आवरणम् ? 0 : योग_Sqr32_गन्तव्यम्_F64(src0, योग_2, sqsum, len, cn); }
};

#यदि_अन्तः // CV_SIMD_64F

#यदि_अन्तः

प्रतिरूपम्<प्रकारनाम T, प्रकारनाम ST, प्रकारनाम SQT>
स्थिर पूर्णाङ्क sumsqr_(ध्रुव T* src0, ध्रुव uchar* आवरणम्, ST* योग_2, SQT* sqsum, पूर्णाङ्क len, पूर्णाङ्क cn )
{
    ध्रुव T* src = src0;

    यदि( !आवरणम् )
    {
        योग_Sqr_SIMD<T, ST, SQT> vop;
        पूर्णाङ्क x = vop(src0, आवरणम्, योग_2, sqsum, len, cn), k = cn % 4;
        src = src0 + x * cn;

        यदि( k == 1 )
        {
            ST s0 = योग_2[0];
            SQT sq0 = sqsum[0];
            प्रति(पूर्णाङ्क i = x; i < len; i++, src += cn )
            {
                ST v = (ST)src[0];
                s0 += v; sq0 += (SQT)v*v;
            }
            योग_2[0] = s0;
            sqsum[0] = sq0;
        }
        अन्यथा यदि( k == 2 )
        {
            ST s0 = योग_2[0], s1 = योग_2[1];
            SQT sq0 = sqsum[0], sq1 = sqsum[1];
            प्रति(पूर्णाङ्क i = x; i < len; i++, src += cn )
            {
                ST v0 = (ST)src[0], v1 = (ST)src[1];
                s0 += v0; sq0 += (SQT)v0*v0;
                s1 += v1; sq1 += (SQT)v1*v1;
            }
            योग_2[0] = s0; योग_2[1] = s1;
            sqsum[0] = sq0; sqsum[1] = sq1;
        }
        अन्यथा यदि( k == 3 )
        {
            ST s0 = योग_2[0], s1 = योग_2[1], s2 = योग_2[2];
            SQT sq0 = sqsum[0], sq1 = sqsum[1], sq2 = sqsum[2];
            प्रति(पूर्णाङ्क i = x; i < len; i++, src += cn )
            {
                ST v0 = (ST)src[0], v1 = (ST)src[1], v2 = (ST)src[2];
                s0 += v0; sq0 += (SQT)v0*v0;
                s1 += v1; sq1 += (SQT)v1*v1;
                s2 += v2; sq2 += (SQT)v2*v2;
            }
            योग_2[0] = s0; योग_2[1] = s1; योग_2[2] = s2;
            sqsum[0] = sq0; sqsum[1] = sq1; sqsum[2] = sq2;
        }

        प्रति( ; k < cn; k += 4 )
        {
            src = src0 + x * cn + k;
            ST s0 = योग_2[k], s1 = योग_2[k+1], s2 = योग_2[k+2], s3 = योग_2[k+3];
            SQT sq0 = sqsum[k], sq1 = sqsum[k+1], sq2 = sqsum[k+2], sq3 = sqsum[k+3];
            प्रति(पूर्णाङ्क i = x; i < len; i++, src += cn )
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
        प्रत्यागम len;
    }

    पूर्णाङ्क i, nzm = 0;

    यदि( cn == 1 )
    {
        ST s0 = योग_2[0];
        SQT sq0 = sqsum[0];
        प्रति( i = 0; i < len; i++ )
            यदि( आवरणम्[i] )
            {
                ST v = (ST)src[i];
                s0 += v; sq0 += (SQT)v*v;
                nzm++;
            }
        योग_2[0] = s0;
        sqsum[0] = sq0;
    }
    अन्यथा यदि( cn == 2 )
    {
        ST s0 = योग_2[0], s1 = योग_2[1];
        SQT sq0 = sqsum[0], sq1 = sqsum[1];
        प्रति( i = 0; i < len; i++, src += 2 )
            यदि( आवरणम्[i] )
            {
                T v0 = src[0], v1 = src[1];
                s0 += v0; sq0 += (SQT)v0*v0;
                s1 += v1; sq1 += (SQT)v1*v1;
                nzm++;
            }
        योग_2[0] = s0; योग_2[1] = s1;
        sqsum[0] = sq0; sqsum[1] = sq1;
    }
    अन्यथा यदि( cn == 3 )
    {
        ST s0 = योग_2[0], s1 = योग_2[1], s2 = योग_2[2];
        SQT sq0 = sqsum[0], sq1 = sqsum[1], sq2 = sqsum[2];
        प्रति( i = 0; i < len; i++, src += 3 )
            यदि( आवरणम्[i] )
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
    अन्यथा यदि( cn == 4 )
    {
        ST s0 = योग_2[0], s1 = योग_2[1], s2 = योग_2[2], s3 = योग_2[3];
        SQT sq0 = sqsum[0], sq1 = sqsum[1], sq2 = sqsum[2], sq3 = sqsum[3];
        प्रति( i = 0; i < len; i++, src += 4 )
            यदि( आवरणम्[i] )
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
        प्रति( i = 0; i < len; i++, src += cn )
            यदि( आवरणम्[i] )
            {
                प्रति( पूर्णाङ्क k = 0; k < cn; k++ )
                {
                    ST v = (ST)src[k];
                    ST s = योग_2[k] + v;
                    SQT sq = sqsum[k] + (SQT)v*v;
                    योग_2[k] = s; sqsum[k] = sq;
                }
                nzm++;
            }
    }
    प्रत्यागम nzm;
}


स्थिर पूर्णाङ्क sqsum8u( ध्रुव uchar* src, ध्रुव uchar* आवरणम्, पूर्णाङ्क* योग_2, पूर्णाङ्क* sqsum, पूर्णाङ्क len, पूर्णाङ्क cn )
{ CV_INSTRUMENT_REGION(); प्रत्यागम sumsqr_(src, आवरणम्, योग_2, sqsum, len, cn); }

स्थिर पूर्णाङ्क sqsum8s( ध्रुव schar* src, ध्रुव uchar* आवरणम्, पूर्णाङ्क* योग_2, पूर्णाङ्क* sqsum, पूर्णाङ्क len, पूर्णाङ्क cn )
{ CV_INSTRUMENT_REGION(); प्रत्यागम sumsqr_(src, आवरणम्, योग_2, sqsum, len, cn); }

स्थिर पूर्णाङ्क sqsum16u( ध्रुव ushort* src, ध्रुव uchar* आवरणम्, पूर्णाङ्क* योग_2, द्विगुण* sqsum, पूर्णाङ्क len, पूर्णाङ्क cn )
{ CV_INSTRUMENT_REGION(); प्रत्यागम sumsqr_(src, आवरणम्, योग_2, sqsum, len, cn); }

स्थिर पूर्णाङ्क sqsum16s( ध्रुव ह्रस्व* src, ध्रुव uchar* आवरणम्, पूर्णाङ्क* योग_2, द्विगुण* sqsum, पूर्णाङ्क len, पूर्णाङ्क cn )
{ CV_INSTRUMENT_REGION(); प्रत्यागम sumsqr_(src, आवरणम्, योग_2, sqsum, len, cn); }

स्थिर पूर्णाङ्क sqsum32s( ध्रुव पूर्णाङ्क* src, ध्रुव uchar* आवरणम्, द्विगुण* योग_2, द्विगुण* sqsum, पूर्णाङ्क len, पूर्णाङ्क cn )
{ CV_INSTRUMENT_REGION(); प्रत्यागम sumsqr_(src, आवरणम्, योग_2, sqsum, len, cn); }

स्थिर पूर्णाङ्क sqsum32f( ध्रुव भिन्न* src, ध्रुव uchar* आवरणम्, द्विगुण* योग_2, द्विगुण* sqsum, पूर्णाङ्क len, पूर्णाङ्क cn )
{ CV_INSTRUMENT_REGION(); प्रत्यागम sumsqr_(src, आवरणम्, योग_2, sqsum, len, cn); }

स्थिर पूर्णाङ्क sqsum64f( ध्रुव द्विगुण* src, ध्रुव uchar* आवरणम्, द्विगुण* योग_2, द्विगुण* sqsum, पूर्णाङ्क len, पूर्णाङ्क cn )
{ CV_INSTRUMENT_REGION(); प्रत्यागम sumsqr_(src, आवरणम्, योग_2, sqsum, len, cn); }

स्थिर पूर्णाङ्क sqsum16f( ध्रुव hfloat* src, ध्रुव uchar* आवरणम्, भिन्न* योग_2, द्विगुण* sqsum, पूर्णाङ्क len, पूर्णाङ्क cn )
{ CV_INSTRUMENT_REGION(); प्रत्यागम sumsqr_(src, आवरणम्, योग_2, sqsum, len, cn); }

स्थिर पूर्णाङ्क sqsum16bf( ध्रुव bfloat* src, ध्रुव uchar* आवरणम्, भिन्न* योग_2, द्विगुण* sqsum, पूर्णाङ्क len, पूर्णाङ्क cn )
{ CV_INSTRUMENT_REGION(); प्रत्यागम sumsqr_(src, आवरणम्, योग_2, sqsum, len, cn); }

स्थिर पूर्णाङ्क sqsum64u( ध्रुव uint64* src, ध्रुव uchar* आवरणम्, द्विगुण* योग_2, द्विगुण* sqsum, पूर्णाङ्क len, पूर्णाङ्क cn )
{ CV_INSTRUMENT_REGION(); प्रत्यागम sumsqr_(src, आवरणम्, योग_2, sqsum, len, cn); }

स्थिर पूर्णाङ्क sqsum64s( ध्रुव int64* src, ध्रुव uchar* आवरणम्, द्विगुण* योग_2, द्विगुण* sqsum, पूर्णाङ्क len, पूर्णाङ्क cn )
{ CV_INSTRUMENT_REGION(); प्रत्यागम sumsqr_(src, आवरणम्, योग_2, sqsum, len, cn); }

स्थिर पूर्णाङ्क sqsum32u( ध्रुव अचिह्नित* src, ध्रुव uchar* आवरणम्, द्विगुण* योग_2, द्विगुण* sqsum, पूर्णाङ्क len, पूर्णाङ्क cn )
{ CV_INSTRUMENT_REGION(); प्रत्यागम sumsqr_(src, आवरणम्, योग_2, sqsum, len, cn); }

योग_Sqr_Func get_योग_Sqr_Func(पूर्णाङ्क गभीरता)
{
    CV_INSTRUMENT_REGION();
    स्थिर योग_Sqr_Func योग_Sqr_Tab[CV_गभीरता_MAX] =
    {
        योग_Sqr_Wrap<uchar, पूर्णाङ्क, पूर्णाङ्क, sqsum8u>, योग_Sqr_Wrap<schar, पूर्णाङ्क, पूर्णाङ्क, sqsum8s>,
        योग_Sqr_Wrap<ushort, पूर्णाङ्क, द्विगुण, sqsum16u>, योग_Sqr_Wrap<ह्रस्व, पूर्णाङ्क, द्विगुण, sqsum16s>,
        योग_Sqr_Wrap<पूर्णाङ्क, द्विगुण, द्विगुण, sqsum32s>, योग_Sqr_Wrap<भिन्न, द्विगुण, द्विगुण, sqsum32f>,
        योग_Sqr_Wrap<द्विगुण, द्विगुण, द्विगुण, sqsum64f>,
        योग_Sqr_Wrap<hfloat, भिन्न, द्विगुण, sqsum16f>, योग_Sqr_Wrap<bfloat, भिन्न, द्विगुण, sqsum16bf>, 0,
        योग_Sqr_Wrap<uint64, द्विगुण, द्विगुण, sqsum64u>, योग_Sqr_Wrap<int64, द्विगुण, द्विगुण, sqsum64s>,
        योग_Sqr_Wrap<अचिह्नित, द्विगुण, द्विगुण, sqsum32u>, 0
    };

    प्रत्यागम योग_Sqr_Tab[गभीरता];
}

#यदि_अन्तः
CV_CPU_OPTIMIZATION_NAMESPACE_अन्तः
} // namespace
