// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html

// Copyright (C) 2026, Advanced Micro Devices, Inc., all rights reserved.

#ಅನ್ತರ್ಭಾವ "precomp.hpp"
#ಅನ್ತರ್ಭಾವ "stat.hpp"

ನಾಮಾಕಾಶ cv {
ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಪೂರ್ಣಾಂಕ (*ಮೊತ್ತ_Sqr_Func)(ಸ್ಥಿರಾಂಕ uchar*, ಸ್ಥಿರಾಂಕ uchar* ಮುಖವಾಡ, uchar*, uchar*, ಪೂರ್ಣಾಂಕ, ಪೂರ್ಣಾಂಕ);

CV_CPU_OPTIMIZATION_NAMESPACE_BEGIN

ಮಾದರಿ<ಬಗೆ_ಹೆಸರು T, ಬಗೆ_ಹೆಸರು ST, ಬಗೆ_ಹೆಸರು SQT, ಪೂರ್ಣಾಂಕ (*fn)(ಸ್ಥಿರಾಂಕ T*, ಸ್ಥಿರಾಂಕ uchar*, ST*, SQT*, ಪೂರ್ಣಾಂಕ, ಪೂರ್ಣಾಂಕ)>
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ ಮೊತ್ತ_Sqr_Wrap(ಸ್ಥಿರಾಂಕ uchar* src, ಸ್ಥಿರಾಂಕ uchar* ಮುಖವಾಡ, uchar* ಮೊತ್ತ, uchar* sqsum, ಪೂರ್ಣಾಂಕ len, ಪೂರ್ಣಾಂಕ cn)
{
    ಹಿಂತಿರುಗಿ fn((ಸ್ಥಿರಾಂಕ T*)src, ಮುಖವಾಡ, (ST*)ಮೊತ್ತ, (SQT*)sqsum, len, cn);
}

ಮೊತ್ತ_Sqr_Func get_ಮೊತ್ತ_Sqr_Func(ಪೂರ್ಣಾಂಕ ಆಳ);

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

ಮಾದರಿ <ಬಗೆ_ಹೆಸರು T, ಬಗೆ_ಹೆಸರು ST, ಬಗೆ_ಹೆಸರು SQT>
ರಚನೆ ಮೊತ್ತ_Sqr_SIMD
{
    ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ ಕ್ರಿಯಾಕಾರಕ () (ಸ್ಥಿರಾಂಕ T *, ಸ್ಥಿರಾಂಕ uchar *, ST *, SQT *, ಪೂರ್ಣಾಂಕ, ಪೂರ್ಣಾಂಕ) ಸ್ಥಿರಾಂಕ
    {
        ಹಿಂತಿರುಗಿ 0;
    }
};

#ಆದರೆ (CV_SIMD || CV_SIMD_SCALABLE)

ಮಾದರಿ <>
ರಚನೆ ಮೊತ್ತ_Sqr_SIMD<uchar, ಪೂರ್ಣಾಂಕ, ಪೂರ್ಣಾಂಕ>
{
    ಪೂರ್ಣಾಂಕ ಕ್ರಿಯಾಕಾರಕ () (ಸ್ಥಿರಾಂಕ uchar * src0, ಸ್ಥಿರಾಂಕ uchar * ಮುಖವಾಡ, ಪೂರ್ಣಾಂಕ * ಮೊತ್ತ, ಪೂರ್ಣಾಂಕ * sqsum, ಪೂರ್ಣಾಂಕ len, ಪೂರ್ಣಾಂಕ cn) ಸ್ಥಿರಾಂಕ
    {
        ಆದರೆ (ಮುಖವಾಡ)
            ಹಿಂತಿರುಗಿ 0;
        // cn==3: deinterleave so each lane belongs to one channel, then reduce per
        // channel. sum/sqsum stay exact in s32 within the dispatcher's 1<<15 block.
        ಆದರೆ (cn == 3)
        {
            ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ vl8 = VTraits<v_uint8>::vlanes();
            v_int32 vs0 = vx_setzero_s32(), vs1 = vx_setzero_s32(), vs2 = vx_setzero_s32();
            v_int32 vq0 = vx_setzero_s32(), vq1 = vx_setzero_s32(), vq2 = vx_setzero_s32();
            ಸ್ವಯಂಚಾಲಿತ acc = [](ಸ್ಥಿರಾಂಕ v_uint8& ch, v_int32& vs, v_int32& vq)
            {
                v_uint16 lo, hi; v_expand(ch, lo, hi);
                v_uint32 s0, s1, s2, s3; v_expand(lo, s0, s1); v_expand(hi, s2, s3);
                vs = v_ಕೂಡಿಸು(vs, v_reinterpret_as_s32(v_ಕೂಡಿಸು(v_ಕೂಡಿಸು(s0, s1), v_ಕೂಡಿಸು(s2, s3))));
                v_int16 d0 = v_reinterpret_as_s16(lo), d1 = v_reinterpret_as_s16(hi);
                vq = v_ಕೂಡಿಸು(vq, v_ಕೂಡಿಸು(v_dotprod(d0, d0), v_dotprod(d1, d1)));
            };
            ಪೂರ್ಣಾಂಕ e = 0;
            ಗಾಗಿ (; e <= len - vl8; e += vl8)
            {
                v_uint8 a, b, c; v_ತುಂಬು_deinterleave(src0 + e * 3, a, b, c);
                acc(a, vs0, vq0); acc(b, vs1, vq1); acc(c, vs2, vq2);
            }
            ಮೊತ್ತ[0] += v_reduce_ಮೊತ್ತ(vs0); ಮೊತ್ತ[1] += v_reduce_ಮೊತ್ತ(vs1); ಮೊತ್ತ[2] += v_reduce_ಮೊತ್ತ(vs2);
            sqsum[0] += v_reduce_ಮೊತ್ತ(vq0); sqsum[1] += v_reduce_ಮೊತ್ತ(vq1); sqsum[2] += v_reduce_ಮೊತ್ತ(vq2);
            v_cleanup();
            ಹಿಂತಿರುಗಿ e;
        }
        ಆದರೆ (cn != 1 && cn != 2 && cn != 4)
            ಹಿಂತಿರುಗಿ 0;
        len *= cn;

        ಪೂರ್ಣಾಂಕ x = 0;
        v_int32 v_ಮೊತ್ತ = vx_setzero_s32();
        v_int32 v_sqsum = vx_setzero_s32();

        ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ len0 = len & -VTraits<v_uint8>::vlanes();
        ಆಗ(x < len0)
        {
            ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ len_tmp = min(x + 256*VTraits<v_uint16>::vlanes(), len0);
            v_uint16 v_sum16 = vx_setzero_u16();
            ಗಾಗಿ ( ; x < len_tmp; x += VTraits<v_uint8>::vlanes())
            {
                v_uint16 v_src0 = vx_ತುಂಬು_expand(src0 + x);
                v_uint16 v_src1 = vx_ತುಂಬು_expand(src0 + x + VTraits<v_uint16>::vlanes());
                v_sum16 = v_ಕೂಡಿಸು(v_sum16, v_ಕೂಡಿಸು(v_src0, v_src1));
                v_int16 v_tmp0, v_tmp1;
                v_zip(v_reinterpret_as_s16(v_src0), v_reinterpret_as_s16(v_src1), v_tmp0, v_tmp1);
                v_sqsum = v_ಕೂಡಿಸು(v_sqsum, v_ಕೂಡಿಸು(v_dotprod(v_tmp0, v_tmp0), v_dotprod(v_tmp1, v_tmp1)));
            }
            v_uint32 v_half0, v_half1;
            v_expand(v_sum16, v_half0, v_half1);
            v_ಮೊತ್ತ = v_ಕೂಡಿಸು(v_ಮೊತ್ತ, v_reinterpret_as_s32(v_ಕೂಡಿಸು(v_half0, v_half1)));
        }
        ಆದರೆ (x <= len - VTraits<v_uint16>::vlanes())
        {
            v_uint16 v_src = vx_ತುಂಬು_expand(src0 + x);
            v_uint16 v_half = v_combine_ಮೇಲಿನ(v_src, v_src);

            v_uint32 v_tmp0, v_tmp1;
            v_expand(v_ಕೂಡಿಸು(v_src, v_half), v_tmp0, v_tmp1);
            v_ಮೊತ್ತ = v_ಕೂಡಿಸು(v_ಮೊತ್ತ, v_reinterpret_as_s32(v_tmp0));

            v_int16 v_tmp2, v_tmp3;
            v_zip(v_reinterpret_as_s16(v_src), v_reinterpret_as_s16(v_half), v_tmp2, v_tmp3);
            v_sqsum = v_ಕೂಡಿಸು(v_sqsum, v_dotprod(v_tmp2, v_tmp2));
            x += VTraits<v_uint16>::vlanes();
        }

        ಆದರೆ (cn == 1)
        {
            *ಮೊತ್ತ += v_reduce_ಮೊತ್ತ(v_ಮೊತ್ತ);
            *sqsum += v_reduce_ಮೊತ್ತ(v_sqsum);
        }
        ಇಲ್ಲದಿದ್ದರೆ
        {
            ಪೂರ್ಣಾಂಕ CV_DECL_ALIGNED(CV_SIMD_ಅಗಲ) ar[2 * VTraits<v_int32>::max_nlanes];
            v_store(ar, v_ಮೊತ್ತ);
            v_store(ar + VTraits<v_int32>::vlanes(), v_sqsum);
            ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < VTraits<v_int32>::vlanes(); ++i)
            {
                ಮೊತ್ತ[i % cn] += ar[i];
                sqsum[i % cn] += ar[VTraits<v_int32>::vlanes() + i];
            }
        }
        v_cleanup();
        ಹಿಂತಿರುಗಿ x / cn;
    }
};

ಮಾದರಿ <>
ರಚನೆ ಮೊತ್ತ_Sqr_SIMD<schar, ಪೂರ್ಣಾಂಕ, ಪೂರ್ಣಾಂಕ>
{
    ಪೂರ್ಣಾಂಕ ಕ್ರಿಯಾಕಾರಕ () (ಸ್ಥಿರಾಂಕ schar * src0, ಸ್ಥಿರಾಂಕ uchar * ಮುಖವಾಡ, ಪೂರ್ಣಾಂಕ * ಮೊತ್ತ, ಪೂರ್ಣಾಂಕ * sqsum, ಪೂರ್ಣಾಂಕ len, ಪೂರ್ಣಾಂಕ cn) ಸ್ಥಿರಾಂಕ
    {
        ಆದರೆ (ಮುಖವಾಡ)
            ಹಿಂತಿರುಗಿ 0;
        ಆದರೆ (cn == 3)
        {
            ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ vl8 = VTraits<v_int8>::vlanes();
            v_int32 vs0 = vx_setzero_s32(), vs1 = vx_setzero_s32(), vs2 = vx_setzero_s32();
            v_int32 vq0 = vx_setzero_s32(), vq1 = vx_setzero_s32(), vq2 = vx_setzero_s32();
            ಸ್ವಯಂಚಾಲಿತ acc = [](ಸ್ಥಿರಾಂಕ v_int8& ch, v_int32& vs, v_int32& vq)
            {
                v_int16 lo, hi; v_expand(ch, lo, hi);
                v_int32 s0, s1, s2, s3; v_expand(lo, s0, s1); v_expand(hi, s2, s3);
                vs = v_ಕೂಡಿಸು(vs, v_ಕೂಡಿಸು(v_ಕೂಡಿಸು(s0, s1), v_ಕೂಡಿಸು(s2, s3)));
                vq = v_ಕೂಡಿಸು(vq, v_ಕೂಡಿಸು(v_dotprod(lo, lo), v_dotprod(hi, hi)));
            };
            ಪೂರ್ಣಾಂಕ e = 0;
            ಗಾಗಿ (; e <= len - vl8; e += vl8)
            {
                v_int8 a, b, c; v_ತುಂಬು_deinterleave(src0 + e * 3, a, b, c);
                acc(a, vs0, vq0); acc(b, vs1, vq1); acc(c, vs2, vq2);
            }
            ಮೊತ್ತ[0] += v_reduce_ಮೊತ್ತ(vs0); ಮೊತ್ತ[1] += v_reduce_ಮೊತ್ತ(vs1); ಮೊತ್ತ[2] += v_reduce_ಮೊತ್ತ(vs2);
            sqsum[0] += v_reduce_ಮೊತ್ತ(vq0); sqsum[1] += v_reduce_ಮೊತ್ತ(vq1); sqsum[2] += v_reduce_ಮೊತ್ತ(vq2);
            v_cleanup();
            ಹಿಂತಿರುಗಿ e;
        }
        ಆದರೆ (cn != 1 && cn != 2 && cn != 4)
            ಹಿಂತಿರುಗಿ 0;
        len *= cn;

        ಪೂರ್ಣಾಂಕ x = 0;
        v_int32 v_ಮೊತ್ತ = vx_setzero_s32();
        v_int32 v_sqsum = vx_setzero_s32();

        ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ len0 = len & -VTraits<v_int8>::vlanes();
        ಆಗ (x < len0)
        {
            ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ len_tmp = min(x + 256 * VTraits<v_int16>::vlanes(), len0);
            v_int16 v_sum16 = vx_setzero_s16();
            ಗಾಗಿ (; x < len_tmp; x += VTraits<v_int8>::vlanes())
            {
                v_int16 v_src0 = vx_ತುಂಬು_expand(src0 + x);
                v_int16 v_src1 = vx_ತುಂಬು_expand(src0 + x + VTraits<v_int16>::vlanes());
                v_sum16 = v_ಕೂಡಿಸು(v_sum16, v_ಕೂಡಿಸು(v_src0, v_src1));
                v_int16 v_tmp0, v_tmp1;
                v_zip(v_src0, v_src1, v_tmp0, v_tmp1);
                v_sqsum = v_ಕೂಡಿಸು(v_sqsum, v_ಕೂಡಿಸು(v_dotprod(v_tmp0, v_tmp0), v_dotprod(v_tmp1, v_tmp1)));
            }
            v_int32 v_half0, v_half1;
            v_expand(v_sum16, v_half0, v_half1);
            v_ಮೊತ್ತ = v_ಕೂಡಿಸು(v_ಮೊತ್ತ, v_ಕೂಡಿಸು(v_half0, v_half1));
        }
        ಆದರೆ (x <= len - VTraits<v_int16>::vlanes())
        {
            v_int16 v_src = vx_ತುಂಬು_expand(src0 + x);
            v_int16 v_half = v_combine_ಮೇಲಿನ(v_src, v_src);

            v_int32 v_tmp0, v_tmp1;
            v_expand(v_ಕೂಡಿಸು(v_src, v_half), v_tmp0, v_tmp1);
            v_ಮೊತ್ತ = v_ಕೂಡಿಸು(v_ಮೊತ್ತ, v_tmp0);

            v_int16 v_tmp2, v_tmp3;
            v_zip(v_src, v_half, v_tmp2, v_tmp3);
            v_sqsum = v_ಕೂಡಿಸು(v_sqsum, v_dotprod(v_tmp2, v_tmp2));
            x += VTraits<v_int16>::vlanes();
        }

        ಆದರೆ (cn == 1)
        {
            *ಮೊತ್ತ += v_reduce_ಮೊತ್ತ(v_ಮೊತ್ತ);
            *sqsum += v_reduce_ಮೊತ್ತ(v_sqsum);
        }
        ಇಲ್ಲದಿದ್ದರೆ
        {
            ಪೂರ್ಣಾಂಕ CV_DECL_ALIGNED(CV_SIMD_ಅಗಲ) ar[2 * VTraits<v_int32>::max_nlanes];
            v_store(ar, v_ಮೊತ್ತ);
            v_store(ar + VTraits<v_int32>::vlanes(), v_sqsum);
            ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < VTraits<v_int32>::vlanes(); ++i)
            {
                ಮೊತ್ತ[i % cn] += ar[i];
                sqsum[i % cn] += ar[VTraits<v_int32>::vlanes() + i];
            }
        }
        v_cleanup();
        ಹಿಂತಿರುಗಿ x / cn;
    }
};

#ಆದರೆ (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)

// Expand a 16-bit vector into two s32 halves. The unsigned overload reinterprets
// after a widening expand (values 0..65535 stay non-negative in s32, so the
// subsequent v_cvt_f64 is exact); the signed overload expands directly. This is
// the only real difference between the ushort and short reductions below.
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ ಮೊತ್ತ_Sqr_Expand_S32(ಸ್ಥಿರಾಂಕ v_uint16& v, v_int32& lo, v_int32& hi)
{
    v_uint32 a, b; v_expand(v, a, b);
    lo = v_reinterpret_as_s32(a); hi = v_reinterpret_as_s32(b);
}
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ ಮೊತ್ತ_Sqr_Expand_S32(ಸ್ಥಿರಾಂಕ v_int16& v, v_int32& lo, v_int32& hi)
{
    v_expand(v, lo, hi);
}

// Shared 16-bit -> {s32 sum, f64 sqsum} reduction for ushort/short. The register
// type is deduced from the pointer, so the body is identical for both depths.
ಮಾದರಿ <ಬಗೆ_ಹೆಸರು T>
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ ಮೊತ್ತ_Sqr16_ಗಮ್ಯ_F64(ಸ್ಥಿರಾಂಕ T* src0, ಪೂರ್ಣಾಂಕ* ಮೊತ್ತ, ದ್ವಿಗುಣ* sqsum, ಪೂರ್ಣಾಂಕ len, ಪೂರ್ಣಾಂಕ cn)
{
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ decltype(vx_ತುಂಬು(src0)) VT; // v_uint16 or v_int16

    ಆದರೆ (cn == 3)
    {
        ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ vl = VTraits<VT>::vlanes();
        v_int32 vs0 = vx_setzero_s32(), vs1 = vx_setzero_s32(), vs2 = vx_setzero_s32();
        v_float64 vq0 = vx_setzero_f64(), vq1 = vx_setzero_f64(), vq2 = vx_setzero_f64();
        ಸ್ವಯಂಚಾಲಿತ acc = [](ಸ್ಥಿರಾಂಕ VT& ch, v_int32& vs, v_float64& vq)
        {
            v_int32 lo, hi; ಮೊತ್ತ_Sqr_Expand_S32(ch, lo, hi);
            vs = v_ಕೂಡಿಸು(vs, v_ಕೂಡಿಸು(lo, hi));
            v_float64 f0 = v_cvt_f64(lo), f1 = v_cvt_f64_ಮೇಲಿನ(lo);
            v_float64 f2 = v_cvt_f64(hi), f3 = v_cvt_f64_ಮೇಲಿನ(hi);
            vq = v_fma(f0, f0, v_fma(f1, f1, v_fma(f2, f2, v_fma(f3, f3, vq))));
        };
        ಪೂರ್ಣಾಂಕ e = 0;
        ಗಾಗಿ (; e <= len - vl; e += vl)
        {
            VT a, b, c; v_ತುಂಬು_deinterleave(src0 + e * 3, a, b, c);
            acc(a, vs0, vq0); acc(b, vs1, vq1); acc(c, vs2, vq2);
        }
        ಮೊತ್ತ[0] += v_reduce_ಮೊತ್ತ(vs0); ಮೊತ್ತ[1] += v_reduce_ಮೊತ್ತ(vs1); ಮೊತ್ತ[2] += v_reduce_ಮೊತ್ತ(vs2);
        sqsum[0] += v_reduce_ಮೊತ್ತ(vq0); sqsum[1] += v_reduce_ಮೊತ್ತ(vq1); sqsum[2] += v_reduce_ಮೊತ್ತ(vq2);
        v_cleanup();
        ಹಿಂತಿರುಗಿ e;
    }
    ಆದರೆ (cn != 1 && cn != 2 && cn != 4)
        ಹಿಂತಿರುಗಿ 0;
    len *= cn;

    ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ vl16 = VTraits<VT>::vlanes();
    ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ vl32 = VTraits<v_int32>::vlanes();
    ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ vl64 = VTraits<v_float64>::vlanes();

    // The lane->channel scatter below (i % cn) and the returned pixel count
    // (x / cn) require the lane counts to be a multiple of cn. This always holds
    // for fixed-width SIMD (vlanes is a power of two >= cn for cn in {1,2,4}),
    // but scalable backends (e.g. RVV) may report a non-multiple; fall back to
    // scalar there. vl16 == 2*vl32, so checking vl32 covers both scatter loops.
    ಆದರೆ (vl32 % cn != 0)
        ಹಿಂತಿರುಗಿ 0;

    v_int32 v_ಮೊತ್ತ = vx_setzero_s32();
    v_float64 v_sq0 = vx_setzero_f64(), v_sq1 = vx_setzero_f64();
    v_float64 v_sq2 = vx_setzero_f64(), v_sq3 = vx_setzero_f64();

    ಪೂರ್ಣಾಂಕ x = 0;
    ಗಾಗಿ (; x <= len - vl16; x += vl16)
    {
        VT v_src = vx_ತುಂಬು(src0 + x);
        v_int32 lo, hi; ಮೊತ್ತ_Sqr_Expand_S32(v_src, lo, hi);
        v_ಮೊತ್ತ = v_ಕೂಡಿಸು(v_ಮೊತ್ತ, v_ಕೂಡಿಸು(lo, hi));

        v_float64 f0 = v_cvt_f64(lo);
        v_float64 f1 = v_cvt_f64_ಮೇಲಿನ(lo);
        v_float64 f2 = v_cvt_f64(hi);
        v_float64 f3 = v_cvt_f64_ಮೇಲಿನ(hi);
        v_sq0 = v_fma(f0, f0, v_sq0);
        v_sq1 = v_fma(f1, f1, v_sq1);
        v_sq2 = v_fma(f2, f2, v_sq2);
        v_sq3 = v_fma(f3, f3, v_sq3);
    }

    ಪೂರ್ಣಾಂಕ    CV_DECL_ALIGNED(CV_SIMD_ಅಗಲ) sbuf[VTraits<v_int32>::max_nlanes];
    ದ್ವಿಗುಣ CV_DECL_ALIGNED(CV_SIMD_ಅಗಲ) qbuf[VTraits<v_float64>::max_nlanes * 4];
    v_store_aligned(sbuf, v_ಮೊತ್ತ);
    v_store_aligned(qbuf + vl64 * 0, v_sq0);
    v_store_aligned(qbuf + vl64 * 1, v_sq1);
    v_store_aligned(qbuf + vl64 * 2, v_sq2);
    v_store_aligned(qbuf + vl64 * 3, v_sq3);

    // sbuf lane j (j<vl32) holds samples j and j+vl32 -> channel j%cn (vl32%cn==0).
    // qbuf index i (i<vl16) holds sample lane i           -> channel i%cn (vl16%cn==0).
    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < vl32; ++i)
        ಮೊತ್ತ[i % cn] += sbuf[i];
    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < vl16; ++i)
        sqsum[i % cn] += qbuf[i];

    v_cleanup();
    ಹಿಂತಿರುಗಿ x / cn;
}

// Shared 32-bit -> f64 reduction for int/float. Both sum and sqsum accumulate in
// f64 to match the scalar reference's double accumulation; values are widened to
// f64 before squaring so the square is computed in double (same as (double)v*v).
// The register type is deduced from the pointer, so int and float share one body.
ಮಾದರಿ <ಬಗೆ_ಹೆಸರು T>
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ ಮೊತ್ತ_Sqr32_ಗಮ್ಯ_F64(ಸ್ಥಿರಾಂಕ T* src0, ದ್ವಿಗುಣ* ಮೊತ್ತ, ದ್ವಿಗುಣ* sqsum, ಪೂರ್ಣಾಂಕ len, ಪೂರ್ಣಾಂಕ cn)
{
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ decltype(vx_ತುಂಬು(src0)) VT; // v_int32 or v_float32

    ಆದರೆ (cn == 3)
    {
        ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ vl = VTraits<VT>::vlanes();
        v_float64 vs0 = vx_setzero_f64(), vs1 = vx_setzero_f64(), vs2 = vx_setzero_f64();
        v_float64 vq0 = vx_setzero_f64(), vq1 = vx_setzero_f64(), vq2 = vx_setzero_f64();
        ಸ್ವಯಂಚಾಲಿತ acc = [](ಸ್ಥಿರಾಂಕ VT& ch, v_float64& vs, v_float64& vq)
        {
            v_float64 f0 = v_cvt_f64(ch), f1 = v_cvt_f64_ಮೇಲಿನ(ch);
            vs = v_ಕೂಡಿಸು(vs, v_ಕೂಡಿಸು(f0, f1));
            vq = v_fma(f0, f0, v_fma(f1, f1, vq));
        };
        ಪೂರ್ಣಾಂಕ e = 0;
        ಗಾಗಿ (; e <= len - vl; e += vl)
        {
            VT a, b, c; v_ತುಂಬು_deinterleave(src0 + e * 3, a, b, c);
            acc(a, vs0, vq0); acc(b, vs1, vq1); acc(c, vs2, vq2);
        }
        ಮೊತ್ತ[0] += v_reduce_ಮೊತ್ತ(vs0); ಮೊತ್ತ[1] += v_reduce_ಮೊತ್ತ(vs1); ಮೊತ್ತ[2] += v_reduce_ಮೊತ್ತ(vs2);
        sqsum[0] += v_reduce_ಮೊತ್ತ(vq0); sqsum[1] += v_reduce_ಮೊತ್ತ(vq1); sqsum[2] += v_reduce_ಮೊತ್ತ(vq2);
        v_cleanup();
        ಹಿಂತಿರುಗಿ e;
    }
    ಆದರೆ (cn != 1 && cn != 2 && cn != 4)
        ಹಿಂತಿರುಗಿ 0;
    len *= cn;

    ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ vl32 = VTraits<VT>::vlanes();
    ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ vl64 = VTraits<v_float64>::vlanes();

    // See note in sumSqr16ToF64: guard scalable backends whose lane count may
    // not divide cn (the i % cn scatter / x / cn rely on it).
    ಆದರೆ (vl32 % cn != 0)
        ಹಿಂತಿರುಗಿ 0;

    v_float64 vs0 = vx_setzero_f64(), vs1 = vx_setzero_f64();
    v_float64 vq0 = vx_setzero_f64(), vq1 = vx_setzero_f64();

    ಪೂರ್ಣಾಂಕ x = 0;
    ಗಾಗಿ (; x <= len - vl32; x += vl32)
    {
        VT v_src = vx_ತುಂಬು(src0 + x);
        v_float64 f0 = v_cvt_f64(v_src);
        v_float64 f1 = v_cvt_f64_ಮೇಲಿನ(v_src);
        vs0 = v_ಕೂಡಿಸು(vs0, f0);
        vs1 = v_ಕೂಡಿಸು(vs1, f1);
        vq0 = v_fma(f0, f0, vq0);
        vq1 = v_fma(f1, f1, vq1);
    }

    ದ್ವಿಗುಣ CV_DECL_ALIGNED(CV_SIMD_ಅಗಲ) sbuf[VTraits<v_float64>::max_nlanes * 2];
    ದ್ವಿಗುಣ CV_DECL_ALIGNED(CV_SIMD_ಅಗಲ) qbuf[VTraits<v_float64>::max_nlanes * 2];
    v_store_aligned(sbuf, vs0);          v_store_aligned(sbuf + vl64, vs1);
    v_store_aligned(qbuf, vq0);          v_store_aligned(qbuf + vl64, vq1);

    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < vl32; ++i)
    {
        ಮೊತ್ತ[i % cn]   += sbuf[i];
        sqsum[i % cn] += qbuf[i];
    }

    v_cleanup();
    ಹಿಂತಿರುಗಿ x / cn;
}

ಮಾದರಿ <>
ರಚನೆ ಮೊತ್ತ_Sqr_SIMD<ushort, ಪೂರ್ಣಾಂಕ, ದ್ವಿಗುಣ>
{
    ಪೂರ್ಣಾಂಕ ಕ್ರಿಯಾಕಾರಕ () (ಸ್ಥಿರಾಂಕ ushort* src0, ಸ್ಥಿರಾಂಕ uchar* ಮುಖವಾಡ, ಪೂರ್ಣಾಂಕ* ಮೊತ್ತ, ದ್ವಿಗುಣ* sqsum, ಪೂರ್ಣಾಂಕ len, ಪೂರ್ಣಾಂಕ cn) ಸ್ಥಿರಾಂಕ
    { ಹಿಂತಿರುಗಿ ಮುಖವಾಡ ? 0 : ಮೊತ್ತ_Sqr16_ಗಮ್ಯ_F64(src0, ಮೊತ್ತ, sqsum, len, cn); }
};

ಮಾದರಿ <>
ರಚನೆ ಮೊತ್ತ_Sqr_SIMD<ಗಿಡ್ಡ, ಪೂರ್ಣಾಂಕ, ದ್ವಿಗುಣ>
{
    ಪೂರ್ಣಾಂಕ ಕ್ರಿಯಾಕಾರಕ () (ಸ್ಥಿರಾಂಕ ಗಿಡ್ಡ* src0, ಸ್ಥಿರಾಂಕ uchar* ಮುಖವಾಡ, ಪೂರ್ಣಾಂಕ* ಮೊತ್ತ, ದ್ವಿಗುಣ* sqsum, ಪೂರ್ಣಾಂಕ len, ಪೂರ್ಣಾಂಕ cn) ಸ್ಥಿರಾಂಕ
    { ಹಿಂತಿರುಗಿ ಮುಖವಾಡ ? 0 : ಮೊತ್ತ_Sqr16_ಗಮ್ಯ_F64(src0, ಮೊತ್ತ, sqsum, len, cn); }
};

ಮಾದರಿ <>
ರಚನೆ ಮೊತ್ತ_Sqr_SIMD<ಪೂರ್ಣಾಂಕ, ದ್ವಿಗುಣ, ದ್ವಿಗುಣ>
{
    ಪೂರ್ಣಾಂಕ ಕ್ರಿಯಾಕಾರಕ () (ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ* src0, ಸ್ಥಿರಾಂಕ uchar* ಮುಖವಾಡ, ದ್ವಿಗುಣ* ಮೊತ್ತ, ದ್ವಿಗುಣ* sqsum, ಪೂರ್ಣಾಂಕ len, ಪೂರ್ಣಾಂಕ cn) ಸ್ಥಿರಾಂಕ
    { ಹಿಂತಿರುಗಿ ಮುಖವಾಡ ? 0 : ಮೊತ್ತ_Sqr32_ಗಮ್ಯ_F64(src0, ಮೊತ್ತ, sqsum, len, cn); }
};

ಮಾದರಿ <>
ರಚನೆ ಮೊತ್ತ_Sqr_SIMD<ದಶಮಾಂಶ, ದ್ವಿಗುಣ, ದ್ವಿಗುಣ>
{
    ಪೂರ್ಣಾಂಕ ಕ್ರಿಯಾಕಾರಕ () (ಸ್ಥಿರಾಂಕ ದಶಮಾಂಶ* src0, ಸ್ಥಿರಾಂಕ uchar* ಮುಖವಾಡ, ದ್ವಿಗುಣ* ಮೊತ್ತ, ದ್ವಿಗುಣ* sqsum, ಪೂರ್ಣಾಂಕ len, ಪೂರ್ಣಾಂಕ cn) ಸ್ಥಿರಾಂಕ
    { ಹಿಂತಿರುಗಿ ಮುಖವಾಡ ? 0 : ಮೊತ್ತ_Sqr32_ಗಮ್ಯ_F64(src0, ಮೊತ್ತ, sqsum, len, cn); }
};

#ಆದರೆ_ಅಂತ್ಯ // CV_SIMD_64F

#ಆದರೆ_ಅಂತ್ಯ

ಮಾದರಿ<ಬಗೆ_ಹೆಸರು T, ಬಗೆ_ಹೆಸರು ST, ಬಗೆ_ಹೆಸರು SQT>
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ sumsqr_(ಸ್ಥಿರಾಂಕ T* src0, ಸ್ಥಿರಾಂಕ uchar* ಮುಖವಾಡ, ST* ಮೊತ್ತ, SQT* sqsum, ಪೂರ್ಣಾಂಕ len, ಪೂರ್ಣಾಂಕ cn )
{
    ಸ್ಥಿರಾಂಕ T* src = src0;

    ಆದರೆ( !ಮುಖವಾಡ )
    {
        ಮೊತ್ತ_Sqr_SIMD<T, ST, SQT> vop;
        ಪೂರ್ಣಾಂಕ x = vop(src0, ಮುಖವಾಡ, ಮೊತ್ತ, sqsum, len, cn), k = cn % 4;
        src = src0 + x * cn;

        ಆದರೆ( k == 1 )
        {
            ST s0 = ಮೊತ್ತ[0];
            SQT sq0 = sqsum[0];
            ಗಾಗಿ(ಪೂರ್ಣಾಂಕ i = x; i < len; i++, src += cn )
            {
                ST v = (ST)src[0];
                s0 += v; sq0 += (SQT)v*v;
            }
            ಮೊತ್ತ[0] = s0;
            sqsum[0] = sq0;
        }
        ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ( k == 2 )
        {
            ST s0 = ಮೊತ್ತ[0], s1 = ಮೊತ್ತ[1];
            SQT sq0 = sqsum[0], sq1 = sqsum[1];
            ಗಾಗಿ(ಪೂರ್ಣಾಂಕ i = x; i < len; i++, src += cn )
            {
                ST v0 = (ST)src[0], v1 = (ST)src[1];
                s0 += v0; sq0 += (SQT)v0*v0;
                s1 += v1; sq1 += (SQT)v1*v1;
            }
            ಮೊತ್ತ[0] = s0; ಮೊತ್ತ[1] = s1;
            sqsum[0] = sq0; sqsum[1] = sq1;
        }
        ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ( k == 3 )
        {
            ST s0 = ಮೊತ್ತ[0], s1 = ಮೊತ್ತ[1], s2 = ಮೊತ್ತ[2];
            SQT sq0 = sqsum[0], sq1 = sqsum[1], sq2 = sqsum[2];
            ಗಾಗಿ(ಪೂರ್ಣಾಂಕ i = x; i < len; i++, src += cn )
            {
                ST v0 = (ST)src[0], v1 = (ST)src[1], v2 = (ST)src[2];
                s0 += v0; sq0 += (SQT)v0*v0;
                s1 += v1; sq1 += (SQT)v1*v1;
                s2 += v2; sq2 += (SQT)v2*v2;
            }
            ಮೊತ್ತ[0] = s0; ಮೊತ್ತ[1] = s1; ಮೊತ್ತ[2] = s2;
            sqsum[0] = sq0; sqsum[1] = sq1; sqsum[2] = sq2;
        }

        ಗಾಗಿ( ; k < cn; k += 4 )
        {
            src = src0 + x * cn + k;
            ST s0 = ಮೊತ್ತ[k], s1 = ಮೊತ್ತ[k+1], s2 = ಮೊತ್ತ[k+2], s3 = ಮೊತ್ತ[k+3];
            SQT sq0 = sqsum[k], sq1 = sqsum[k+1], sq2 = sqsum[k+2], sq3 = sqsum[k+3];
            ಗಾಗಿ(ಪೂರ್ಣಾಂಕ i = x; i < len; i++, src += cn )
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
            ಮೊತ್ತ[k] = s0; ಮೊತ್ತ[k+1] = s1;
            ಮೊತ್ತ[k+2] = s2; ಮೊತ್ತ[k+3] = s3;
            sqsum[k] = sq0; sqsum[k+1] = sq1;
            sqsum[k+2] = sq2; sqsum[k+3] = sq3;
        }
        ಹಿಂತಿರುಗಿ len;
    }

    ಪೂರ್ಣಾಂಕ i, nzm = 0;

    ಆದರೆ( cn == 1 )
    {
        ST s0 = ಮೊತ್ತ[0];
        SQT sq0 = sqsum[0];
        ಗಾಗಿ( i = 0; i < len; i++ )
            ಆದರೆ( ಮುಖವಾಡ[i] )
            {
                ST v = (ST)src[i];
                s0 += v; sq0 += (SQT)v*v;
                nzm++;
            }
        ಮೊತ್ತ[0] = s0;
        sqsum[0] = sq0;
    }
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ( cn == 2 )
    {
        ST s0 = ಮೊತ್ತ[0], s1 = ಮೊತ್ತ[1];
        SQT sq0 = sqsum[0], sq1 = sqsum[1];
        ಗಾಗಿ( i = 0; i < len; i++, src += 2 )
            ಆದರೆ( ಮುಖವಾಡ[i] )
            {
                T v0 = src[0], v1 = src[1];
                s0 += v0; sq0 += (SQT)v0*v0;
                s1 += v1; sq1 += (SQT)v1*v1;
                nzm++;
            }
        ಮೊತ್ತ[0] = s0; ಮೊತ್ತ[1] = s1;
        sqsum[0] = sq0; sqsum[1] = sq1;
    }
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ( cn == 3 )
    {
        ST s0 = ಮೊತ್ತ[0], s1 = ಮೊತ್ತ[1], s2 = ಮೊತ್ತ[2];
        SQT sq0 = sqsum[0], sq1 = sqsum[1], sq2 = sqsum[2];
        ಗಾಗಿ( i = 0; i < len; i++, src += 3 )
            ಆದರೆ( ಮುಖವಾಡ[i] )
            {
                ST v0 = (ST)src[0], v1 = (ST)src[1], v2 = (ST)src[2];
                s0 += v0; sq0 += (SQT)v0*v0;
                s1 += v1; sq1 += (SQT)v1*v1;
                s2 += v2; sq2 += (SQT)v2*v2;
                nzm++;
            }
        ಮೊತ್ತ[0] = s0; ಮೊತ್ತ[1] = s1; ಮೊತ್ತ[2] = s2;
        sqsum[0] = sq0; sqsum[1] = sq1; sqsum[2] = sq2;
    }
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ( cn == 4 )
    {
        ST s0 = ಮೊತ್ತ[0], s1 = ಮೊತ್ತ[1], s2 = ಮೊತ್ತ[2], s3 = ಮೊತ್ತ[3];
        SQT sq0 = sqsum[0], sq1 = sqsum[1], sq2 = sqsum[2], sq3 = sqsum[3];
        ಗಾಗಿ( i = 0; i < len; i++, src += 4 )
            ಆದರೆ( ಮುಖವಾಡ[i] )
            {
                T v0 = src[0], v1 = src[1], v2 = src[2], v3 = src[3];
                s0 += v0; sq0 += (SQT)v0*v0;
                s1 += v1; sq1 += (SQT)v1*v1;
                s2 += v2; sq2 += (SQT)v2*v2;
                s3 += v3; sq3 += (SQT)v3*v3;
                nzm++;
            }
        ಮೊತ್ತ[0] = s0; ಮೊತ್ತ[1] = s1; ಮೊತ್ತ[2] = s2; ಮೊತ್ತ[3] = s3;
        sqsum[0] = sq0; sqsum[1] = sq1; sqsum[2] = sq2; sqsum[3] = sq3;
    }
    ಇಲ್ಲದಿದ್ದರೆ
    {
        ಗಾಗಿ( i = 0; i < len; i++, src += cn )
            ಆದರೆ( ಮುಖವಾಡ[i] )
            {
                ಗಾಗಿ( ಪೂರ್ಣಾಂಕ k = 0; k < cn; k++ )
                {
                    ST v = (ST)src[k];
                    ST s = ಮೊತ್ತ[k] + v;
                    SQT sq = sqsum[k] + (SQT)v*v;
                    ಮೊತ್ತ[k] = s; sqsum[k] = sq;
                }
                nzm++;
            }
    }
    ಹಿಂತಿರುಗಿ nzm;
}


ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ sqsum8u( ಸ್ಥಿರಾಂಕ uchar* src, ಸ್ಥಿರಾಂಕ uchar* ಮುಖವಾಡ, ಪೂರ್ಣಾಂಕ* ಮೊತ್ತ, ಪೂರ್ಣಾಂಕ* sqsum, ಪೂರ್ಣಾಂಕ len, ಪೂರ್ಣಾಂಕ cn )
{ CV_INSTRUMENT_REGION(); ಹಿಂತಿರುಗಿ sumsqr_(src, ಮುಖವಾಡ, ಮೊತ್ತ, sqsum, len, cn); }

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ sqsum8s( ಸ್ಥಿರಾಂಕ schar* src, ಸ್ಥಿರಾಂಕ uchar* ಮುಖವಾಡ, ಪೂರ್ಣಾಂಕ* ಮೊತ್ತ, ಪೂರ್ಣಾಂಕ* sqsum, ಪೂರ್ಣಾಂಕ len, ಪೂರ್ಣಾಂಕ cn )
{ CV_INSTRUMENT_REGION(); ಹಿಂತಿರುಗಿ sumsqr_(src, ಮುಖವಾಡ, ಮೊತ್ತ, sqsum, len, cn); }

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ sqsum16u( ಸ್ಥಿರಾಂಕ ushort* src, ಸ್ಥಿರಾಂಕ uchar* ಮುಖವಾಡ, ಪೂರ್ಣಾಂಕ* ಮೊತ್ತ, ದ್ವಿಗುಣ* sqsum, ಪೂರ್ಣಾಂಕ len, ಪೂರ್ಣಾಂಕ cn )
{ CV_INSTRUMENT_REGION(); ಹಿಂತಿರುಗಿ sumsqr_(src, ಮುಖವಾಡ, ಮೊತ್ತ, sqsum, len, cn); }

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ sqsum16s( ಸ್ಥಿರಾಂಕ ಗಿಡ್ಡ* src, ಸ್ಥಿರಾಂಕ uchar* ಮುಖವಾಡ, ಪೂರ್ಣಾಂಕ* ಮೊತ್ತ, ದ್ವಿಗುಣ* sqsum, ಪೂರ್ಣಾಂಕ len, ಪೂರ್ಣಾಂಕ cn )
{ CV_INSTRUMENT_REGION(); ಹಿಂತಿರುಗಿ sumsqr_(src, ಮುಖವಾಡ, ಮೊತ್ತ, sqsum, len, cn); }

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ sqsum32s( ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ* src, ಸ್ಥಿರಾಂಕ uchar* ಮುಖವಾಡ, ದ್ವಿಗುಣ* ಮೊತ್ತ, ದ್ವಿಗುಣ* sqsum, ಪೂರ್ಣಾಂಕ len, ಪೂರ್ಣಾಂಕ cn )
{ CV_INSTRUMENT_REGION(); ಹಿಂತಿರುಗಿ sumsqr_(src, ಮುಖವಾಡ, ಮೊತ್ತ, sqsum, len, cn); }

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ sqsum32f( ಸ್ಥಿರಾಂಕ ದಶಮಾಂಶ* src, ಸ್ಥಿರಾಂಕ uchar* ಮುಖವಾಡ, ದ್ವಿಗುಣ* ಮೊತ್ತ, ದ್ವಿಗುಣ* sqsum, ಪೂರ್ಣಾಂಕ len, ಪೂರ್ಣಾಂಕ cn )
{ CV_INSTRUMENT_REGION(); ಹಿಂತಿರುಗಿ sumsqr_(src, ಮುಖವಾಡ, ಮೊತ್ತ, sqsum, len, cn); }

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ sqsum64f( ಸ್ಥಿರಾಂಕ ದ್ವಿಗುಣ* src, ಸ್ಥಿರಾಂಕ uchar* ಮುಖವಾಡ, ದ್ವಿಗುಣ* ಮೊತ್ತ, ದ್ವಿಗುಣ* sqsum, ಪೂರ್ಣಾಂಕ len, ಪೂರ್ಣಾಂಕ cn )
{ CV_INSTRUMENT_REGION(); ಹಿಂತಿರುಗಿ sumsqr_(src, ಮುಖವಾಡ, ಮೊತ್ತ, sqsum, len, cn); }

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ sqsum16f( ಸ್ಥಿರಾಂಕ hfloat* src, ಸ್ಥಿರಾಂಕ uchar* ಮುಖವಾಡ, ದಶಮಾಂಶ* ಮೊತ್ತ, ದ್ವಿಗುಣ* sqsum, ಪೂರ್ಣಾಂಕ len, ಪೂರ್ಣಾಂಕ cn )
{ CV_INSTRUMENT_REGION(); ಹಿಂತಿರುಗಿ sumsqr_(src, ಮುಖವಾಡ, ಮೊತ್ತ, sqsum, len, cn); }

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ sqsum16bf( ಸ್ಥಿರಾಂಕ bfloat* src, ಸ್ಥಿರಾಂಕ uchar* ಮುಖವಾಡ, ದಶಮಾಂಶ* ಮೊತ್ತ, ದ್ವಿಗುಣ* sqsum, ಪೂರ್ಣಾಂಕ len, ಪೂರ್ಣಾಂಕ cn )
{ CV_INSTRUMENT_REGION(); ಹಿಂತಿರುಗಿ sumsqr_(src, ಮುಖವಾಡ, ಮೊತ್ತ, sqsum, len, cn); }

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ sqsum64u( ಸ್ಥಿರಾಂಕ uint64* src, ಸ್ಥಿರಾಂಕ uchar* ಮುಖವಾಡ, ದ್ವಿಗುಣ* ಮೊತ್ತ, ದ್ವಿಗುಣ* sqsum, ಪೂರ್ಣಾಂಕ len, ಪೂರ್ಣಾಂಕ cn )
{ CV_INSTRUMENT_REGION(); ಹಿಂತಿರುಗಿ sumsqr_(src, ಮುಖವಾಡ, ಮೊತ್ತ, sqsum, len, cn); }

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ sqsum64s( ಸ್ಥಿರಾಂಕ int64* src, ಸ್ಥಿರಾಂಕ uchar* ಮುಖವಾಡ, ದ್ವಿಗುಣ* ಮೊತ್ತ, ದ್ವಿಗುಣ* sqsum, ಪೂರ್ಣಾಂಕ len, ಪೂರ್ಣಾಂಕ cn )
{ CV_INSTRUMENT_REGION(); ಹಿಂತಿರುಗಿ sumsqr_(src, ಮುಖವಾಡ, ಮೊತ್ತ, sqsum, len, cn); }

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ sqsum32u( ಸ್ಥಿರಾಂಕ ಚಿಹ್ನೆರಹಿತ* src, ಸ್ಥಿರಾಂಕ uchar* ಮುಖವಾಡ, ದ್ವಿಗುಣ* ಮೊತ್ತ, ದ್ವಿಗುಣ* sqsum, ಪೂರ್ಣಾಂಕ len, ಪೂರ್ಣಾಂಕ cn )
{ CV_INSTRUMENT_REGION(); ಹಿಂತಿರುಗಿ sumsqr_(src, ಮುಖವಾಡ, ಮೊತ್ತ, sqsum, len, cn); }

ಮೊತ್ತ_Sqr_Func get_ಮೊತ್ತ_Sqr_Func(ಪೂರ್ಣಾಂಕ ಆಳ)
{
    CV_INSTRUMENT_REGION();
    ಸ್ಥಿರ ಮೊತ್ತ_Sqr_Func ಮೊತ್ತ_Sqr_Tab[CV_ಆಳ_MAX] =
    {
        ಮೊತ್ತ_Sqr_Wrap<uchar, ಪೂರ್ಣಾಂಕ, ಪೂರ್ಣಾಂಕ, sqsum8u>, ಮೊತ್ತ_Sqr_Wrap<schar, ಪೂರ್ಣಾಂಕ, ಪೂರ್ಣಾಂಕ, sqsum8s>,
        ಮೊತ್ತ_Sqr_Wrap<ushort, ಪೂರ್ಣಾಂಕ, ದ್ವಿಗುಣ, sqsum16u>, ಮೊತ್ತ_Sqr_Wrap<ಗಿಡ್ಡ, ಪೂರ್ಣಾಂಕ, ದ್ವಿಗುಣ, sqsum16s>,
        ಮೊತ್ತ_Sqr_Wrap<ಪೂರ್ಣಾಂಕ, ದ್ವಿಗುಣ, ದ್ವಿಗುಣ, sqsum32s>, ಮೊತ್ತ_Sqr_Wrap<ದಶಮಾಂಶ, ದ್ವಿಗುಣ, ದ್ವಿಗುಣ, sqsum32f>,
        ಮೊತ್ತ_Sqr_Wrap<ದ್ವಿಗುಣ, ದ್ವಿಗುಣ, ದ್ವಿಗುಣ, sqsum64f>,
        ಮೊತ್ತ_Sqr_Wrap<hfloat, ದಶಮಾಂಶ, ದ್ವಿಗುಣ, sqsum16f>, ಮೊತ್ತ_Sqr_Wrap<bfloat, ದಶಮಾಂಶ, ದ್ವಿಗುಣ, sqsum16bf>, 0,
        ಮೊತ್ತ_Sqr_Wrap<uint64, ದ್ವಿಗುಣ, ದ್ವಿಗುಣ, sqsum64u>, ಮೊತ್ತ_Sqr_Wrap<int64, ದ್ವಿಗುಣ, ದ್ವಿಗುಣ, sqsum64s>,
        ಮೊತ್ತ_Sqr_Wrap<ಚಿಹ್ನೆರಹಿತ, ದ್ವಿಗುಣ, ದ್ವಿಗುಣ, sqsum32u>, 0
    };

    ಹಿಂತಿರುಗಿ ಮೊತ್ತ_Sqr_Tab[ಆಳ];
}

#ಆದರೆ_ಅಂತ್ಯ
CV_CPU_OPTIMIZATION_NAMESPACE_ಕೊನೆ
} // namespace
