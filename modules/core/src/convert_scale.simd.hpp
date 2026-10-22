// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


#ಅನ್ತರ್ಭಾವ "precomp.hpp"
#ಅನ್ತರ್ಭಾವ "convert.hpp"

ನಾಮಾಕಾಶ cv {
CV_CPU_OPTIMIZATION_NAMESPACE_BEGIN

ದ್ವಿಮಾನ_Func getCvtScaleAbsFunc(ಪೂರ್ಣಾಂಕ ಆಳ);
ದ್ವಿಮಾನ_Func getConvertScaleFunc(ಪೂರ್ಣಾಂಕ sdepth, ಪೂರ್ಣಾಂಕ ddepth);

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

/****************************************************************************************\
*                                convertScale[Abs]                                       *
\****************************************************************************************/

ಮಾದರಿ<ಬಗೆ_ಹೆಸರು _Ts, ಬಗೆ_ಹೆಸರು _Td> ಸಾಲೊಳಗೆ ಶೂನ್ಯ
cvtabs_32f( ಸ್ಥಿರಾಂಕ _Ts* src, size_t sstep, _Td* dst, size_t dstep,
            ಗಾತ್ರ_2 size, ದಶಮಾಂಶ a, ದಶಮಾಂಶ b )
{
#ಆದರೆ (CV_SIMD || CV_SIMD_SCALABLE)
    v_float32 va = vx_setall_f32(a), vb = vx_setall_f32(b);
    ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ VECSZ = VTraits<v_float32>::vlanes()*2;
    // GCC miscompiles this scalable block only on VLEN=128 RVV (opencv/issues/26936);
    // use scalar there, keep SIMD on wider vectors. Ask the hardware for VLEN directly:
    // vlanes() would depend on the LMUL the universal intrinsics happen to use.
#ಆದರೆ (CV_RVV && CV_SIMD_SCALABLE && defined(__GNUC__) && !defined(__clang__))
    ಸ್ಥಿರಾಂಕ ತಾರ್ಕಿಕ useSIMD = __riscv_vlenb()*8 != 128;
#ಇಲ್ಲದಿದ್ದರೆ
    ಸ್ಥಿರಾಂಕ ತಾರ್ಕಿಕ useSIMD = ನಿಜ;
#ಆದರೆ_ಅಂತ್ಯ
#ಆದರೆ_ಅಂತ್ಯ
    sstep /= ಗಾತ್ರ(src[0]);
    dstep /= ಗಾತ್ರ(dst[0]);

    ಗಾಗಿ( ಪೂರ್ಣಾಂಕ i = 0; i < size.ಎತ್ತರ; i++, src += sstep, dst += dstep )
    {
        ಪೂರ್ಣಾಂಕ j = 0;
#ಆದರೆ (CV_SIMD || CV_SIMD_SCALABLE)
        ಆದರೆ( useSIMD )
        ಗಾಗಿ( ; j < size.ಅಗಲ; j += VECSZ )
        {
            ಆದರೆ( j > size.ಅಗಲ - VECSZ )
            {
                ಆದರೆ( j == 0 || src == (_Ts*)dst )
                    ನಿಲ್ಲಿಸು;
                j = size.ಅಗಲ - VECSZ;
            }
            v_float32 v0, v1;
            vx_ತುಂಬು_pair_as(src + j, v0, v1);
            v0 = v_fma(v0, va, vb);
            v1 = v_fma(v1, va, vb);
            v_store_pair_as(dst + j, v_abs(v0), v_abs(v1));
        }
#ಆದರೆ_ಅಂತ್ಯ
        ಗಾಗಿ( ; j < size.ಅಗಲ; j++ )
            dst[j] = saturate_cast<_Td>(std::ನಿರಪೇಕ್ಷ(src[j]*a + b));
    }
}

ಸ್ಥಿರ ಶೂನ್ಯ
cvtabs_32f( ಸ್ಥಿರಾಂಕ ತಾರ್ಕಿಕ* src_, size_t sstep,
            uchar* dst, size_t dstep,
            ಗಾತ್ರ_2 size, ದಶಮಾಂಶ a, ದಶಮಾಂಶ b )
{
    ಸ್ಥಿರಾಂಕ uchar* src = (ಸ್ಥಿರಾಂಕ uchar*)src_;
    uchar v0 = saturate_cast<uchar>(std::ನಿರಪೇಕ್ಷ(b));
    uchar v1 = saturate_cast<uchar>(std::ನಿರಪೇಕ್ಷ(a + b));
    ಗಾಗಿ( ಪೂರ್ಣಾಂಕ i = 0; i < size.ಎತ್ತರ; i++, src += sstep, dst += dstep )
    {
        ಗಾಗಿ (ಪೂರ್ಣಾಂಕ j = 0; j < size.ಅಗಲ; j++)
            dst[j] = src[j] != 0 ? v1 : v0;
    }
}

ಮಾದರಿ<ಬಗೆ_ಹೆಸರು _Ts, ಬಗೆ_ಹೆಸರು _Td> ಸಾಲೊಳಗೆ ಶೂನ್ಯ
cvt_32f( ಸ್ಥಿರಾಂಕ _Ts* src, size_t sstep, _Td* dst, size_t dstep,
         ಗಾತ್ರ_2 size, ದಶಮಾಂಶ a, ದಶಮಾಂಶ b )
{
#ಆದರೆ (CV_SIMD || (CV_SIMD_SCALABLE && !(defined(__GNUC__) && !defined(__clang__))) )
    v_float32 va = vx_setall_f32(a), vb = vx_setall_f32(b);
    ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ VECSZ = VTraits<v_float32>::vlanes()*2;
#ಆದರೆ_ಅಂತ್ಯ
    sstep /= ಗಾತ್ರ(src[0]);
    dstep /= ಗಾತ್ರ(dst[0]);

    ಗಾಗಿ( ಪೂರ್ಣಾಂಕ i = 0; i < size.ಎತ್ತರ; i++, src += sstep, dst += dstep )
    {
        ಪೂರ್ಣಾಂಕ j = 0;
// Excluding GNU in CV_SIMD_SCALABLE because of "opencv/issues/26936"
#ಆದರೆ (CV_SIMD || (CV_SIMD_SCALABLE && !(defined(__GNUC__) && !defined(__clang__))) )
        ಗಾಗಿ( ; j < size.ಅಗಲ; j += VECSZ )
        {
            ಆದರೆ( j > size.ಅಗಲ - VECSZ )
            {
                ಆದರೆ( j == 0 || src == (_Ts*)dst )
                    ನಿಲ್ಲಿಸು;
                j = size.ಅಗಲ - VECSZ;
            }
            v_float32 v0, v1;
            vx_ತುಂಬು_pair_as(src + j, v0, v1);
            v0 = v_fma(v0, va, vb);
            v1 = v_fma(v1, va, vb);
            v_store_pair_as(dst + j, v0, v1);
        }
#ಆದರೆ_ಅಂತ್ಯ
        ಗಾಗಿ( ; j < size.ಅಗಲ; j++ )
            dst[j] = saturate_cast<_Td>(src[j]*a + b);
    }
}

// variant for conversions 16f <-> ... w/o unrolling
ಮಾದರಿ<ಬಗೆ_ಹೆಸರು _Ts, ಬಗೆ_ಹೆಸರು _Td> ಸಾಲೊಳಗೆ ಶೂನ್ಯ
cvt1_32f( ಸ್ಥಿರಾಂಕ _Ts* src, size_t sstep, _Td* dst, size_t dstep,
          ಗಾತ್ರ_2 size, ದಶಮಾಂಶ a, ದಶಮಾಂಶ b )
{
#ಆದರೆ (CV_SIMD || CV_SIMD_SCALABLE)
    v_float32 va = vx_setall_f32(a), vb = vx_setall_f32(b);
    ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ VECSZ = VTraits<v_float32>::vlanes();
#ಆದರೆ_ಅಂತ್ಯ
    sstep /= ಗಾತ್ರ(src[0]);
    dstep /= ಗಾತ್ರ(dst[0]);

    ಗಾಗಿ( ಪೂರ್ಣಾಂಕ i = 0; i < size.ಎತ್ತರ; i++, src += sstep, dst += dstep )
    {
        ಪೂರ್ಣಾಂಕ j = 0;
#ಆದರೆ (CV_SIMD || CV_SIMD_SCALABLE)
        ಗಾಗಿ( ; j < size.ಅಗಲ; j += VECSZ )
        {
            ಆದರೆ( j > size.ಅಗಲ - VECSZ )
            {
                ಆದರೆ( j == 0 || src == (_Ts*)dst )
                    ನಿಲ್ಲಿಸು;
                j = size.ಅಗಲ - VECSZ;
            }
            v_float32 v0;
            vx_ತುಂಬು_as(src + j, v0);
            v0 = v_fma(v0, va, vb);
            v_store_as(dst + j, v0);
        }
#ಆದರೆ_ಅಂತ್ಯ
        ಗಾಗಿ( ; j < size.ಅಗಲ; j++ )
            dst[j] = saturate_cast<_Td>(src[j]*a + b);
    }
}


ಮಾದರಿ<ಬಗೆ_ಹೆಸರು _Ts, ಬಗೆ_ಹೆಸರು _Td> ಸಾಲೊಳಗೆ ಶೂನ್ಯ
cvt_64f( ಸ್ಥಿರಾಂಕ _Ts* src, size_t sstep, _Td* dst, size_t dstep,
         ಗಾತ್ರ_2 size, ದ್ವಿಗುಣ a, ದ್ವಿಗುಣ b )
{
#ಆದರೆ (CV_SIMD_64F || (CV_SIMD_SCALABLE_64F && !(defined(__GNUC__) && !defined(__clang__))) )
    v_float64 va = vx_setall_f64(a), vb = vx_setall_f64(b);
    ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ VECSZ = VTraits<v_float64>::vlanes()*2;
#ಆದರೆ_ಅಂತ್ಯ
    sstep /= ಗಾತ್ರ(src[0]);
    dstep /= ಗಾತ್ರ(dst[0]);

    ಗಾಗಿ( ಪೂರ್ಣಾಂಕ i = 0; i < size.ಎತ್ತರ; i++, src += sstep, dst += dstep )
    {
        ಪೂರ್ಣಾಂಕ j = 0;
// Excluding GNU in CV_SIMD_SCALABLE because of "opencv/issues/26936"
#ಆದರೆ (CV_SIMD_64F || (CV_SIMD_SCALABLE_64F && !(defined(__GNUC__) && !defined(__clang__))) )
        ಗಾಗಿ( ; j < size.ಅಗಲ; j += VECSZ )
        {
            ಆದರೆ( j > size.ಅಗಲ - VECSZ )
            {
                ಆದರೆ( j == 0 || src == (_Ts*)dst )
                    ನಿಲ್ಲಿಸು;
                j = size.ಅಗಲ - VECSZ;
            }
            v_float64 v0, v1;
            vx_ತುಂಬು_pair_as(src + j, v0, v1);
            v0 = v_fma(v0, va, vb);
            v1 = v_fma(v1, va, vb);
            v_store_pair_as(dst + j, v0, v1);
        }
#ಆದರೆ_ಅಂತ್ಯ
        ಗಾಗಿ( ; j < size.ಅಗಲ; j++ )
            dst[j] = saturate_cast<_Td>(src[j]*a + b);
    }
}

//==================================================================================================

#ವ್ಯಾಖ್ಯಾನಿಸು DEF_CVT_SCALE_ABS_FUNC(suffix, cvt, stype, dtype, wtype) \
ಸ್ಥಿರ ಶೂನ್ಯ cvtScaleAbs##suffix( ಸ್ಥಿರಾಂಕ uchar* src_, size_t sstep, ಸ್ಥಿರಾಂಕ uchar*, size_t, \
                                 uchar* dst_, size_t dstep, ಗಾತ್ರ_2 size, ಶೂನ್ಯ* scale_) \
{ \
    ಸ್ಥಿರಾಂಕ stype* src = (ಸ್ಥಿರಾಂಕ stype*)src_; \
    dtype* dst = (dtype*)dst_; \
    ದ್ವಿಗುಣ* scale = (ದ್ವಿಗುಣ*)scale_; \
    cvt(src, sstep, dst, dstep, size, (wtype)scale[0], (wtype)scale[1]); \
}


#ವ್ಯಾಖ್ಯಾನಿಸು DEF_CVT_SCALE_FUNC(suffix, cvt, stype, dtype, wtype) \
ಸ್ಥಿರ ಶೂನ್ಯ cvtScale##suffix( ಸ್ಥಿರಾಂಕ uchar* src_, size_t sstep, ಸ್ಥಿರಾಂಕ uchar*, size_t, \
                              uchar* dst_, size_t dstep, ಗಾತ್ರ_2 size, ಶೂನ್ಯ* scale_) \
{ \
    ಸ್ಥಿರಾಂಕ stype* src = (ಸ್ಥಿರಾಂಕ stype*)src_; \
    dtype* dst = (dtype*)dst_; \
    ದ್ವಿಗುಣ* scale = (ದ್ವಿಗುಣ*)scale_; \
    cvt(src, sstep, dst, dstep, size, (wtype)scale[0], (wtype)scale[1]); \
}

#ವ್ಯಾಖ್ಯಾನಿಸು DEF_CVT_SCALE2BOOL_FUNC(suffix, stype, wtype) \
ಸ್ಥಿರ ಶೂನ್ಯ cvtScale##suffix( ಸ್ಥಿರಾಂಕ uchar* src_, size_t sstep, ಸ್ಥಿರಾಂಕ uchar*, size_t, \
                              uchar* dst, size_t dstep, ಗಾತ್ರ_2 size, ಶೂನ್ಯ* scale_) \
{ \
    ಸ್ಥಿರಾಂಕ stype* src = (ಸ್ಥಿರಾಂಕ stype*)src_; \
    ಸ್ಥಿರಾಂಕ ದ್ವಿಗುಣ* scale = (ಸ್ಥಿರಾಂಕ ದ್ವಿಗುಣ*)scale_; \
    wtype a = (wtype)scale[0], b = (wtype)scale[1]; \
    sstep /= ಗಾತ್ರ(src[0]); \
    ಗಾಗಿ( ಪೂರ್ಣಾಂಕ i = 0; i < size.ಎತ್ತರ; i++, src += sstep, dst += dstep ) \
        ಗಾಗಿ (ಪೂರ್ಣಾಂಕ j = 0; j < size.ಅಗಲ; j++) \
            dst[j] = (ತಾರ್ಕಿಕ)((wtype)src[j]*a + b != 0); \
}

#ವ್ಯಾಖ್ಯಾನಿಸು DEF_CVT_SCALEBOOL2_FUNC(suffix, dtype, wtype) \
ಸ್ಥಿರ ಶೂನ್ಯ cvtScale##suffix( ಸ್ಥಿರಾಂಕ uchar* src, size_t sstep, ಸ್ಥಿರಾಂಕ uchar*, size_t, \
                              uchar* dst_, size_t dstep, ಗಾತ್ರ_2 size, ಶೂನ್ಯ* scale_) \
{ \
    dtype* dst = (dtype*)dst_; \
    ಸ್ಥಿರಾಂಕ ದ್ವಿಗುಣ* scale = (ಸ್ಥಿರಾಂಕ ದ್ವಿಗುಣ*)scale_; \
    wtype a = (wtype)scale[0], b = (wtype)scale[1]; \
    dtype v0 = saturate_cast<dtype>(b), v1 = saturate_cast<dtype>(a + b); \
    dstep /= ಗಾತ್ರ(dst[0]); \
    ಗಾಗಿ( ಪೂರ್ಣಾಂಕ i = 0; i < size.ಎತ್ತರ; i++, src += sstep, dst += dstep ) \
        ಗಾಗಿ (ಪೂರ್ಣಾಂಕ j = 0; j < size.ಅಗಲ; j++) \
            dst[j] = src[j] != 0 ? v1 : v0; \
}

DEF_CVT_SCALE_ABS_FUNC(8u,    cvtabs_32f, uchar,  uchar, ದಶಮಾಂಶ)
DEF_CVT_SCALE_ABS_FUNC(8s8u,  cvtabs_32f, schar,  uchar, ದಶಮಾಂಶ)
DEF_CVT_SCALE_ABS_FUNC(8b8u,  cvtabs_32f, ತಾರ್ಕಿಕ,  uchar, ದಶಮಾಂಶ)
DEF_CVT_SCALE_ABS_FUNC(16u8u, cvtabs_32f, ushort, uchar, ದಶಮಾಂಶ)
DEF_CVT_SCALE_ABS_FUNC(16s8u, cvtabs_32f, ಗಿಡ್ಡ,  uchar, ದಶಮಾಂಶ)
DEF_CVT_SCALE_ABS_FUNC(32u8u, cvtabs_32f, ಚಿಹ್ನೆರಹಿತ, uchar, ದಶಮಾಂಶ)
DEF_CVT_SCALE_ABS_FUNC(32s8u, cvtabs_32f, ಪೂರ್ಣಾಂಕ,    uchar, ದಶಮಾಂಶ)
DEF_CVT_SCALE_ABS_FUNC(32f8u, cvtabs_32f, ದಶಮಾಂಶ,  uchar, ದಶಮಾಂಶ)
DEF_CVT_SCALE_ABS_FUNC(64u8u, cvtabs_32f, uint64_t, uchar, ದಶಮಾಂಶ)
DEF_CVT_SCALE_ABS_FUNC(64s8u, cvtabs_32f, int64_t, uchar, ದಶಮಾಂಶ)
DEF_CVT_SCALE_ABS_FUNC(64f8u, cvtabs_32f, ದ್ವಿಗುಣ, uchar, ದಶಮಾಂಶ)
DEF_CVT_SCALE_ABS_FUNC(16f8u, cvtabs_32f, hfloat, uchar, ದಶಮಾಂಶ)
DEF_CVT_SCALE_ABS_FUNC(16bf8u, cvtabs_32f, bfloat, uchar, ದಶಮಾಂಶ)

DEF_CVT_SCALE_FUNC(8u,     cvt_32f, uchar,  uchar, ದಶಮಾಂಶ)
DEF_CVT_SCALE_FUNC(8s8u,   cvt_32f, schar,  uchar, ದಶಮಾಂಶ)
DEF_CVT_SCALE_FUNC(16u8u,  cvt_32f, ushort, uchar, ದಶಮಾಂಶ)
DEF_CVT_SCALE_FUNC(16s8u,  cvt_32f, ಗಿಡ್ಡ,  uchar, ದಶಮಾಂಶ)
DEF_CVT_SCALE_FUNC(32u8u,  cvt_32f, ಚಿಹ್ನೆರಹಿತ, uchar, ದಶಮಾಂಶ)
DEF_CVT_SCALE_FUNC(32s8u,  cvt_32f, ಪೂರ್ಣಾಂಕ,    uchar, ದಶಮಾಂಶ)
DEF_CVT_SCALE_FUNC(32f8u,  cvt_32f, ದಶಮಾಂಶ,  uchar, ದಶಮಾಂಶ)
DEF_CVT_SCALE_FUNC(64f8u,  cvt_32f, ದ್ವಿಗುಣ, uchar, ದಶಮಾಂಶ)
DEF_CVT_SCALE_FUNC(64u8u,  cvt_32f, uint64_t, uchar, ದಶಮಾಂಶ)
DEF_CVT_SCALE_FUNC(64s8u,  cvt_32f, int64_t, uchar, ದಶಮಾಂಶ)
DEF_CVT_SCALE_FUNC(16f8u,  cvt_32f, hfloat, uchar, ದಶಮಾಂಶ)
DEF_CVT_SCALE_FUNC(16bf8u, cvt_32f, bfloat, uchar, ದಶಮಾಂಶ)

DEF_CVT_SCALE_FUNC(8u8s,   cvt_32f, uchar,  schar, ದಶಮಾಂಶ)
DEF_CVT_SCALE_FUNC(8s,     cvt_32f, schar,  schar, ದಶಮಾಂಶ)
DEF_CVT_SCALE_FUNC(16u8s,  cvt_32f, ushort, schar, ದಶಮಾಂಶ)
DEF_CVT_SCALE_FUNC(16s8s,  cvt_32f, ಗಿಡ್ಡ,  schar, ದಶಮಾಂಶ)
DEF_CVT_SCALE_FUNC(32u8s,  cvt_32f, ಚಿಹ್ನೆರಹಿತ, schar, ದಶಮಾಂಶ)
DEF_CVT_SCALE_FUNC(32s8s,  cvt_32f, ಪೂರ್ಣಾಂಕ,    schar, ದಶಮಾಂಶ)
DEF_CVT_SCALE_FUNC(32f8s,  cvt_32f, ದಶಮಾಂಶ,  schar, ದಶಮಾಂಶ)
DEF_CVT_SCALE_FUNC(64f8s,  cvt_32f, ದ್ವಿಗುಣ, schar, ದಶಮಾಂಶ)
DEF_CVT_SCALE_FUNC(64u8s,  cvt_32f, uint64_t, schar, ದಶಮಾಂಶ)
DEF_CVT_SCALE_FUNC(64s8s,  cvt_32f, int64_t, schar, ದಶಮಾಂಶ)
DEF_CVT_SCALE_FUNC(16f8s,  cvt_32f, hfloat, schar, ದಶಮಾಂಶ)
DEF_CVT_SCALE_FUNC(16bf8s, cvt_32f, bfloat, schar, ದಶಮಾಂಶ)

DEF_CVT_SCALE2BOOL_FUNC(8u8b, uchar, ದಶಮಾಂಶ)
DEF_CVT_SCALE2BOOL_FUNC(8s8b, schar, ದಶಮಾಂಶ)
DEF_CVT_SCALE2BOOL_FUNC(16u8b, ushort, ದಶಮಾಂಶ)
DEF_CVT_SCALE2BOOL_FUNC(16s8b, ಗಿಡ್ಡ, ದಶಮಾಂಶ)
DEF_CVT_SCALE2BOOL_FUNC(32u8b, ಚಿಹ್ನೆರಹಿತ, ದಶಮಾಂಶ)
DEF_CVT_SCALE2BOOL_FUNC(32s8b, ಪೂರ್ಣಾಂಕ, ದಶಮಾಂಶ)
DEF_CVT_SCALE2BOOL_FUNC(32f8b, ದಶಮಾಂಶ, ದಶಮಾಂಶ)
DEF_CVT_SCALE2BOOL_FUNC(64f8b, ದ್ವಿಗುಣ, ದಶಮಾಂಶ)
DEF_CVT_SCALE2BOOL_FUNC(64u8b, uint64_t, ದಶಮಾಂಶ)
DEF_CVT_SCALE2BOOL_FUNC(64s8b, int64_t, ದಶಮಾಂಶ)
DEF_CVT_SCALE2BOOL_FUNC(16f8b, hfloat, ದಶಮಾಂಶ)
DEF_CVT_SCALE2BOOL_FUNC(16bf8b, bfloat, ದಶಮಾಂಶ)

DEF_CVT_SCALE_FUNC(8u16u,  cvt_32f, uchar,  ushort, ದಶಮಾಂಶ)
DEF_CVT_SCALE_FUNC(8s16u,  cvt_32f, schar,  ushort, ದಶಮಾಂಶ)
DEF_CVT_SCALE_FUNC(16u,    cvt_32f, ushort, ushort, ದಶಮಾಂಶ)
DEF_CVT_SCALE_FUNC(16s16u, cvt_32f, ಗಿಡ್ಡ,  ushort, ದಶಮಾಂಶ)
DEF_CVT_SCALE_FUNC(32u16u, cvt_32f, ಚಿಹ್ನೆರಹಿತ, ushort, ದಶಮಾಂಶ)
DEF_CVT_SCALE_FUNC(32s16u, cvt_32f, ಪೂರ್ಣಾಂಕ,    ushort, ದಶಮಾಂಶ)
DEF_CVT_SCALE_FUNC(32f16u, cvt_32f, ದಶಮಾಂಶ,  ushort, ದಶಮಾಂಶ)
DEF_CVT_SCALE_FUNC(64f16u, cvt_32f, ದ್ವಿಗುಣ, ushort, ದಶಮಾಂಶ)
DEF_CVT_SCALE_FUNC(64u16u, cvt_32f, uint64_t, ushort, ದಶಮಾಂಶ)
DEF_CVT_SCALE_FUNC(64s16u, cvt_32f, int64_t, ushort, ದಶಮಾಂಶ)
DEF_CVT_SCALE_FUNC(16f16u, cvt1_32f, hfloat, ushort, ದಶಮಾಂಶ)
DEF_CVT_SCALE_FUNC(16bf16u, cvt1_32f, bfloat, ushort, ದಶಮಾಂಶ)

DEF_CVT_SCALE_FUNC(8u16s,  cvt_32f, uchar,  ಗಿಡ್ಡ, ದಶಮಾಂಶ)
DEF_CVT_SCALE_FUNC(8s16s,  cvt_32f, schar,  ಗಿಡ್ಡ, ದಶಮಾಂಶ)
DEF_CVT_SCALE_FUNC(16u16s, cvt_32f, ushort, ಗಿಡ್ಡ, ದಶಮಾಂಶ)
DEF_CVT_SCALE_FUNC(16s,    cvt_32f, ಗಿಡ್ಡ,  ಗಿಡ್ಡ, ದಶಮಾಂಶ)
DEF_CVT_SCALE_FUNC(32u16s, cvt_32f, ಚಿಹ್ನೆರಹಿತ, ಗಿಡ್ಡ, ದಶಮಾಂಶ)
DEF_CVT_SCALE_FUNC(32s16s, cvt_32f, ಪೂರ್ಣಾಂಕ,    ಗಿಡ್ಡ, ದಶಮಾಂಶ)
DEF_CVT_SCALE_FUNC(32f16s, cvt_32f, ದಶಮಾಂಶ,  ಗಿಡ್ಡ, ದಶಮಾಂಶ)
DEF_CVT_SCALE_FUNC(64f16s, cvt_32f, ದ್ವಿಗುಣ, ಗಿಡ್ಡ, ದಶಮಾಂಶ)
DEF_CVT_SCALE_FUNC(64u16s, cvt_32f, uint64_t, ಗಿಡ್ಡ, ದಶಮಾಂಶ)
DEF_CVT_SCALE_FUNC(64s16s, cvt_32f, int64_t, ಗಿಡ್ಡ, ದಶಮಾಂಶ)
DEF_CVT_SCALE_FUNC(16f16s, cvt1_32f, hfloat, ಗಿಡ್ಡ, ದಶಮಾಂಶ)
DEF_CVT_SCALE_FUNC(16bf16s, cvt1_32f, bfloat, ಗಿಡ್ಡ, ದಶಮಾಂಶ)

DEF_CVT_SCALE_FUNC(8u32u,  cvt_32f, uchar,  ಚಿಹ್ನೆರಹಿತ, ದಶಮಾಂಶ)
DEF_CVT_SCALE_FUNC(8s32u,  cvt_32f, schar,  ಚಿಹ್ನೆರಹಿತ, ದಶಮಾಂಶ)
DEF_CVT_SCALE_FUNC(16u32u, cvt_32f, ushort, ಚಿಹ್ನೆರಹಿತ, ದಶಮಾಂಶ)
DEF_CVT_SCALE_FUNC(16s32u, cvt_32f, ಗಿಡ್ಡ,  ಚಿಹ್ನೆರಹಿತ, ದಶಮಾಂಶ)
DEF_CVT_SCALE_FUNC(32u, cvt_32f, ಚಿಹ್ನೆರಹಿತ, ಚಿಹ್ನೆರಹಿತ, ದಶಮಾಂಶ)
DEF_CVT_SCALE_FUNC(32s32u, cvt_64f, ಪೂರ್ಣಾಂಕ,    ಚಿಹ್ನೆರಹಿತ, ದ್ವಿಗುಣ)
DEF_CVT_SCALE_FUNC(32f32u, cvt_32f, ದಶಮಾಂಶ,  ಚಿಹ್ನೆರಹಿತ, ದಶಮಾಂಶ)
DEF_CVT_SCALE_FUNC(64f32u, cvt_64f, ದ್ವಿಗುಣ, ಚಿಹ್ನೆರಹಿತ, ದ್ವಿಗುಣ)
DEF_CVT_SCALE_FUNC(64u32u, cvt_32f, uint64_t, ಚಿಹ್ನೆರಹಿತ, ದಶಮಾಂಶ)
DEF_CVT_SCALE_FUNC(64s32u, cvt_32f, int64_t, ಚಿಹ್ನೆರಹಿತ, ದಶಮಾಂಶ)
DEF_CVT_SCALE_FUNC(16f32u, cvt1_32f, hfloat, ಚಿಹ್ನೆರಹಿತ, ದಶಮಾಂಶ)
DEF_CVT_SCALE_FUNC(16bf32u, cvt1_32f, bfloat, ಚಿಹ್ನೆರಹಿತ, ದಶಮಾಂಶ)

DEF_CVT_SCALE_FUNC(8u32s,  cvt_32f, uchar,  ಪೂರ್ಣಾಂಕ, ದಶಮಾಂಶ)
DEF_CVT_SCALE_FUNC(8s32s,  cvt_32f, schar,  ಪೂರ್ಣಾಂಕ, ದಶಮಾಂಶ)
DEF_CVT_SCALE_FUNC(16u32s, cvt_32f, ushort, ಪೂರ್ಣಾಂಕ, ದಶಮಾಂಶ)
DEF_CVT_SCALE_FUNC(16s32s, cvt_32f, ಗಿಡ್ಡ,  ಪೂರ್ಣಾಂಕ, ದಶಮಾಂಶ)
DEF_CVT_SCALE_FUNC(32u32s, cvt_32f, ಚಿಹ್ನೆರಹಿತ, ಪೂರ್ಣಾಂಕ, ದಶಮಾಂಶ)
DEF_CVT_SCALE_FUNC(32s,    cvt_64f, ಪೂರ್ಣಾಂಕ,    ಪೂರ್ಣಾಂಕ, ದ್ವಿಗುಣ)
DEF_CVT_SCALE_FUNC(32f32s, cvt_32f, ದಶಮಾಂಶ,  ಪೂರ್ಣಾಂಕ, ದಶಮಾಂಶ)
DEF_CVT_SCALE_FUNC(64f32s, cvt_64f, ದ್ವಿಗುಣ, ಪೂರ್ಣಾಂಕ, ದ್ವಿಗುಣ)
DEF_CVT_SCALE_FUNC(64u32s, cvt_32f, uint64_t, ಪೂರ್ಣಾಂಕ, ದಶಮಾಂಶ)
DEF_CVT_SCALE_FUNC(64s32s, cvt_32f, int64_t, ಪೂರ್ಣಾಂಕ, ದಶಮಾಂಶ)
DEF_CVT_SCALE_FUNC(16f32s, cvt1_32f, hfloat, ಪೂರ್ಣಾಂಕ, ದಶಮಾಂಶ)
DEF_CVT_SCALE_FUNC(16bf32s, cvt1_32f, bfloat, ಪೂರ್ಣಾಂಕ, ದಶಮಾಂಶ)

DEF_CVT_SCALE_FUNC(8u32f,  cvt_32f, uchar,  ದಶಮಾಂಶ, ದಶಮಾಂಶ)
DEF_CVT_SCALE_FUNC(8s32f,  cvt_32f, schar,  ದಶಮಾಂಶ, ದಶಮಾಂಶ)
DEF_CVT_SCALE_FUNC(16u32f, cvt_32f, ushort, ದಶಮಾಂಶ, ದಶಮಾಂಶ)
DEF_CVT_SCALE_FUNC(16s32f, cvt_32f, ಗಿಡ್ಡ,  ದಶಮಾಂಶ, ದಶಮಾಂಶ)
DEF_CVT_SCALE_FUNC(32u32f, cvt_32f, ಚಿಹ್ನೆರಹಿತ, ದಶಮಾಂಶ, ದಶಮಾಂಶ)
DEF_CVT_SCALE_FUNC(32s32f, cvt_32f, ಪೂರ್ಣಾಂಕ,    ದಶಮಾಂಶ, ದಶಮಾಂಶ)
DEF_CVT_SCALE_FUNC(32f,    cvt_32f, ದಶಮಾಂಶ,  ದಶಮಾಂಶ, ದಶಮಾಂಶ)
DEF_CVT_SCALE_FUNC(64f32f, cvt_64f, ದ್ವಿಗುಣ, ದಶಮಾಂಶ, ದ್ವಿಗುಣ)
DEF_CVT_SCALE_FUNC(64u32f, cvt_32f, uint64_t, ದಶಮಾಂಶ, ದಶಮಾಂಶ)
DEF_CVT_SCALE_FUNC(64s32f, cvt_32f, int64_t, ದಶಮಾಂಶ, ದಶಮಾಂಶ)
DEF_CVT_SCALE_FUNC(16f32f, cvt1_32f, hfloat, ದಶಮಾಂಶ, ದಶಮಾಂಶ)
DEF_CVT_SCALE_FUNC(16bf32f, cvt1_32f, bfloat, ದಶಮಾಂಶ, ದಶಮಾಂಶ)

DEF_CVT_SCALE_FUNC(8u64f,  cvt_64f, uchar,  ದ್ವಿಗುಣ, ದ್ವಿಗುಣ)
DEF_CVT_SCALE_FUNC(8s64f,  cvt_64f, schar,  ದ್ವಿಗುಣ, ದ್ವಿಗುಣ)
DEF_CVT_SCALE_FUNC(16u64f, cvt_64f, ushort, ದ್ವಿಗುಣ, ದ್ವಿಗುಣ)
DEF_CVT_SCALE_FUNC(16s64f, cvt_64f, ಗಿಡ್ಡ,  ದ್ವಿಗುಣ, ದ್ವಿಗುಣ)
DEF_CVT_SCALE_FUNC(32u64f, cvt_64f, ಚಿಹ್ನೆರಹಿತ, ದ್ವಿಗುಣ, ದ್ವಿಗುಣ)
DEF_CVT_SCALE_FUNC(32s64f, cvt_64f, ಪೂರ್ಣಾಂಕ,    ದ್ವಿಗುಣ, ದ್ವಿಗುಣ)
DEF_CVT_SCALE_FUNC(32f64f, cvt_64f, ದಶಮಾಂಶ,  ದ್ವಿಗುಣ, ದ್ವಿಗುಣ)
DEF_CVT_SCALE_FUNC(64f,    cvt_64f, ದ್ವಿಗುಣ, ದ್ವಿಗುಣ, ದ್ವಿಗುಣ)
DEF_CVT_SCALE_FUNC(64u64f, cvt_64f, uint64_t, ದ್ವಿಗುಣ, ದ್ವಿಗುಣ)
DEF_CVT_SCALE_FUNC(64s64f, cvt_64f, int64_t, ದ್ವಿಗುಣ, ದ್ವಿಗುಣ)
DEF_CVT_SCALE_FUNC(16f64f, cvt_64f, hfloat, ದ್ವಿಗುಣ, ದ್ವಿಗುಣ)
DEF_CVT_SCALE_FUNC(16bf64f, cvt_64f, bfloat, ದ್ವಿಗುಣ, ದ್ವಿಗುಣ)

DEF_CVT_SCALE_FUNC(8u64u,  cvt_64f, uchar,  uint64_t, ದ್ವಿಗುಣ)
DEF_CVT_SCALE_FUNC(8s64u,  cvt_64f, schar,  uint64_t, ದ್ವಿಗುಣ)
DEF_CVT_SCALE_FUNC(16u64u, cvt_64f, ushort, uint64_t, ದ್ವಿಗುಣ)
DEF_CVT_SCALE_FUNC(16s64u, cvt_64f, ಗಿಡ್ಡ,  uint64_t, ದ್ವಿಗುಣ)
DEF_CVT_SCALE_FUNC(32u64u, cvt_64f, ಚಿಹ್ನೆರಹಿತ, uint64_t, ದ್ವಿಗುಣ)
DEF_CVT_SCALE_FUNC(32s64u, cvt_64f, ಪೂರ್ಣಾಂಕ,    uint64_t, ದ್ವಿಗುಣ)
DEF_CVT_SCALE_FUNC(32f64u, cvt_64f, ದಶಮಾಂಶ,  uint64_t, ದ್ವಿಗುಣ)
DEF_CVT_SCALE_FUNC(64f64u, cvt_64f, ದ್ವಿಗುಣ, uint64_t, ದ್ವಿಗುಣ)
DEF_CVT_SCALE_FUNC(64u, cvt_64f, uint64_t, uint64_t, ದ್ವಿಗುಣ)
DEF_CVT_SCALE_FUNC(64s64u, cvt_64f, int64_t, uint64_t, ದ್ವಿಗುಣ)
DEF_CVT_SCALE_FUNC(16f64u, cvt_64f, hfloat, uint64_t, ದ್ವಿಗುಣ)
DEF_CVT_SCALE_FUNC(16bf64u, cvt_64f, bfloat, uint64_t, ದ್ವಿಗುಣ)

DEF_CVT_SCALE_FUNC(8u64s,  cvt_64f, uchar,  int64_t, ದ್ವಿಗುಣ)
DEF_CVT_SCALE_FUNC(8s64s,  cvt_64f, schar,  int64_t, ದ್ವಿಗುಣ)
DEF_CVT_SCALE_FUNC(16u64s, cvt_64f, ushort, int64_t, ದ್ವಿಗುಣ)
DEF_CVT_SCALE_FUNC(16s64s, cvt_64f, ಗಿಡ್ಡ,  int64_t, ದ್ವಿಗುಣ)
DEF_CVT_SCALE_FUNC(32u64s, cvt_64f, ಚಿಹ್ನೆರಹಿತ, int64_t, ದ್ವಿಗುಣ)
DEF_CVT_SCALE_FUNC(32s64s, cvt_64f, ಪೂರ್ಣಾಂಕ,    int64_t, ದ್ವಿಗುಣ)
DEF_CVT_SCALE_FUNC(32f64s, cvt_64f, ದಶಮಾಂಶ,  int64_t, ದ್ವಿಗುಣ)
DEF_CVT_SCALE_FUNC(64f64s, cvt_64f, ದ್ವಿಗುಣ, int64_t, ದ್ವಿಗುಣ)
DEF_CVT_SCALE_FUNC(64u64s, cvt_64f, uint64_t, int64_t, ದ್ವಿಗುಣ)
DEF_CVT_SCALE_FUNC(64s, cvt_64f, int64_t, int64_t, ದ್ವಿಗುಣ)
DEF_CVT_SCALE_FUNC(16f64s, cvt_64f, hfloat, int64_t, ದ್ವಿಗುಣ)
DEF_CVT_SCALE_FUNC(16bf64s, cvt_64f, bfloat, int64_t, ದ್ವಿಗುಣ)

DEF_CVT_SCALE_FUNC(8u16f,  cvt1_32f, uchar,  hfloat, ದಶಮಾಂಶ)
DEF_CVT_SCALE_FUNC(8s16f,  cvt1_32f, schar,  hfloat, ದಶಮಾಂಶ)
DEF_CVT_SCALE_FUNC(16u16f, cvt1_32f, ushort, hfloat, ದಶಮಾಂಶ)
DEF_CVT_SCALE_FUNC(16s16f, cvt1_32f, ಗಿಡ್ಡ,  hfloat, ದಶಮಾಂಶ)
DEF_CVT_SCALE_FUNC(32u16f, cvt1_32f, ಚಿಹ್ನೆರಹಿತ, hfloat, ದಶಮಾಂಶ)
DEF_CVT_SCALE_FUNC(32s16f, cvt1_32f, ಪೂರ್ಣಾಂಕ,    hfloat, ದಶಮಾಂಶ)
DEF_CVT_SCALE_FUNC(32f16f, cvt1_32f, ದಶಮಾಂಶ,  hfloat, ದಶಮಾಂಶ)
DEF_CVT_SCALE_FUNC(64f16f, cvt1_32f, ದ್ವಿಗುಣ, hfloat, ದಶಮಾಂಶ)
DEF_CVT_SCALE_FUNC(64u16f, cvt1_32f, uint64_t, hfloat, ದಶಮಾಂಶ)
DEF_CVT_SCALE_FUNC(64s16f, cvt1_32f, int64_t, hfloat, ದಶಮಾಂಶ)
DEF_CVT_SCALE_FUNC(16f,    cvt1_32f, hfloat, hfloat, ದಶಮಾಂಶ)
DEF_CVT_SCALE_FUNC(16bf16f, cvt1_32f, bfloat, hfloat, ದಶಮಾಂಶ)

DEF_CVT_SCALE_FUNC(8u16bf,  cvt1_32f, uchar,  bfloat, ದಶಮಾಂಶ)
DEF_CVT_SCALE_FUNC(8s16bf,  cvt1_32f, schar,  bfloat, ದಶಮಾಂಶ)
DEF_CVT_SCALE_FUNC(16u16bf, cvt1_32f, ushort, bfloat, ದಶಮಾಂಶ)
DEF_CVT_SCALE_FUNC(16s16bf, cvt1_32f, ಗಿಡ್ಡ,  bfloat, ದಶಮಾಂಶ)
DEF_CVT_SCALE_FUNC(32u16bf, cvt1_32f, ಚಿಹ್ನೆರಹಿತ, bfloat, ದಶಮಾಂಶ)
DEF_CVT_SCALE_FUNC(32s16bf, cvt1_32f, ಪೂರ್ಣಾಂಕ,    bfloat, ದಶಮಾಂಶ)
DEF_CVT_SCALE_FUNC(32f16bf, cvt1_32f, ದಶಮಾಂಶ,  bfloat, ದಶಮಾಂಶ)
DEF_CVT_SCALE_FUNC(64f16bf, cvt1_32f, ದ್ವಿಗುಣ, bfloat, ದಶಮಾಂಶ)
DEF_CVT_SCALE_FUNC(64u16bf, cvt1_32f, uint64_t, bfloat, ದಶಮಾಂಶ)
DEF_CVT_SCALE_FUNC(64s16bf, cvt1_32f, int64_t, bfloat, ದಶಮಾಂಶ)
DEF_CVT_SCALE_FUNC(16f16bf, cvt1_32f, hfloat, bfloat, ದಶಮಾಂಶ)
DEF_CVT_SCALE_FUNC(16bf, cvt1_32f, bfloat, bfloat, ದಶಮಾಂಶ)

DEF_CVT_SCALEBOOL2_FUNC(8b8u, uchar, ದಶಮಾಂಶ)
DEF_CVT_SCALEBOOL2_FUNC(8b8s, schar, ದಶಮಾಂಶ)
DEF_CVT_SCALEBOOL2_FUNC(8b, ತಾರ್ಕಿಕ, ದಶಮಾಂಶ)
DEF_CVT_SCALEBOOL2_FUNC(8b16u, ushort, ದಶಮಾಂಶ)
DEF_CVT_SCALEBOOL2_FUNC(8b16s, ಗಿಡ್ಡ, ದಶಮಾಂಶ)
DEF_CVT_SCALEBOOL2_FUNC(8b32u, ಚಿಹ್ನೆರಹಿತ, ದಶಮಾಂಶ)
DEF_CVT_SCALEBOOL2_FUNC(8b32s, ಪೂರ್ಣಾಂಕ, ದಶಮಾಂಶ)
DEF_CVT_SCALEBOOL2_FUNC(8b32f, ದಶಮಾಂಶ, ದಶಮಾಂಶ)
DEF_CVT_SCALEBOOL2_FUNC(8b64u, uint64_t, ದ್ವಿಗುಣ)
DEF_CVT_SCALEBOOL2_FUNC(8b64s, int64_t, ದ್ವಿಗುಣ)
DEF_CVT_SCALEBOOL2_FUNC(8b64f, ದ್ವಿಗುಣ, ದ್ವಿಗುಣ)
DEF_CVT_SCALEBOOL2_FUNC(8b16f, hfloat, ದಶಮಾಂಶ)
DEF_CVT_SCALEBOOL2_FUNC(8b16bf, bfloat, ದಶಮಾಂಶ)

// FP8 src -> CV_8U abs|scale|shift (scalar)
#ವ್ಯಾಖ್ಯಾನಿಸು DEF_CVT_SCALE_ABS_SCALAR_FUNC(suffix, stype) \
ಸ್ಥಿರ ಶೂನ್ಯ cvtScaleAbs##suffix( ಸ್ಥಿರಾಂಕ uchar* src_, size_t sstep, ಸ್ಥಿರಾಂಕ uchar*, size_t, \
                                 uchar* dst, size_t dstep, ಗಾತ್ರ_2 size, ಶೂನ್ಯ* scale_) \
{ \
    ಸ್ಥಿರಾಂಕ stype* src = (ಸ್ಥಿರಾಂಕ stype*)src_; \
    ಸ್ಥಿರಾಂಕ ದ್ವಿಗುಣ* scale = (ಸ್ಥಿರಾಂಕ ದ್ವಿಗುಣ*)scale_; \
    ದಶಮಾಂಶ a = (ದಶಮಾಂಶ)scale[0], b = (ದಶಮಾಂಶ)scale[1]; \
    sstep /= ಗಾತ್ರ(src[0]); \
    ಗಾಗಿ( ಪೂರ್ಣಾಂಕ i = 0; i < size.ಎತ್ತರ; i++, src += sstep, dst += dstep ) \
        ಗಾಗಿ (ಪೂರ್ಣಾಂಕ j = 0; j < size.ಅಗಲ; j++) \
            dst[j] = saturate_cast<uchar>(std::ನಿರಪೇಕ್ಷ((ದಶಮಾಂಶ)src[j]*a + b)); \
}
DEF_CVT_SCALE_ABS_SCALAR_FUNC(8fe4m3u8u, fp8a_t)
DEF_CVT_SCALE_ABS_SCALAR_FUNC(8fe4m38u,  fp8_t)

ದ್ವಿಮಾನ_Func getCvtScaleAbsFunc(ಪೂರ್ಣಾಂಕ ಆಳ)
{
    ದ್ವಿಮಾನ_Func func =
        ಆಳ == CV_8F_E4M3FN ? (ದ್ವಿಮಾನ_Func)cvtScaleAbs8fe4m38u :
        ಆಳ == CV_8F_E4M3FNUZ ? (ದ್ವಿಮಾನ_Func)cvtScaleAbs8fe4m3u8u :
        ಆಳ == CV_8U ? (ದ್ವಿಮಾನ_Func)cvtScaleAbs8u :
        ಆಳ == CV_8S ? (ದ್ವಿಮಾನ_Func)cvtScaleAbs8s8u :
        ಆಳ == CV_Bool ? (ದ್ವಿಮಾನ_Func)cvtScaleAbs8b8u :
        ಆಳ == CV_16U ? (ದ್ವಿಮಾನ_Func)cvtScaleAbs16u8u :
        ಆಳ == CV_16S ? (ದ್ವಿಮಾನ_Func)cvtScaleAbs16s8u :
        ಆಳ == CV_16F ? (ದ್ವಿಮಾನ_Func)cvtScaleAbs16f8u :
        ಆಳ == CV_16BF ? (ದ್ವಿಮಾನ_Func)cvtScaleAbs16bf8u :
        ಆಳ == CV_32U ? (ದ್ವಿಮಾನ_Func)cvtScaleAbs32u8u :
        ಆಳ == CV_32S ? (ದ್ವಿಮಾನ_Func)cvtScaleAbs32s8u :
        ಆಳ == CV_32F ? (ದ್ವಿಮಾನ_Func)cvtScaleAbs32f8u :
        ಆಳ == CV_64U ? (ದ್ವಿಮಾನ_Func)cvtScaleAbs64u8u :
        ಆಳ == CV_64S ? (ದ್ವಿಮಾನ_Func)cvtScaleAbs64s8u :
        ಆಳ == CV_64F ? (ದ್ವಿಮಾನ_Func)cvtScaleAbs64f8u : 0;
    CV_Assert(func != 0);
    ಹಿಂತಿರುಗಿ func;
}

//////////////////// FP8 scaled conversions — scalar via saturate_cast(src*a+b) ////////////////////
#ವ್ಯಾಖ್ಯಾನಿಸು DEF_CVT_SCALE_SCALAR_FUNC(suffix, stype, dtype, wtype) \
ಸ್ಥಿರ ಶೂನ್ಯ cvtScale##suffix( ಸ್ಥಿರಾಂಕ uchar* src_, size_t sstep, ಸ್ಥಿರಾಂಕ uchar*, size_t, \
                              uchar* dst_, size_t dstep, ಗಾತ್ರ_2 size, ಶೂನ್ಯ* scale_) \
{ \
    ಸ್ಥಿರಾಂಕ stype* src = (ಸ್ಥಿರಾಂಕ stype*)src_; dtype* dst = (dtype*)dst_; \
    ಸ್ಥಿರಾಂಕ ದ್ವಿಗುಣ* scale = (ಸ್ಥಿರಾಂಕ ದ್ವಿಗುಣ*)scale_; \
    wtype a = (wtype)scale[0], b = (wtype)scale[1]; \
    sstep /= ಗಾತ್ರ(src[0]); dstep /= ಗಾತ್ರ(dst[0]); \
    ಗಾಗಿ( ಪೂರ್ಣಾಂಕ i = 0; i < size.ಎತ್ತರ; i++, src += sstep, dst += dstep ) \
        ಗಾಗಿ (ಪೂರ್ಣಾಂಕ j = 0; j < size.ಅಗಲ; j++) \
            dst[j] = saturate_cast<dtype>((wtype)src[j]*a + b); \
}

// double intermediate so coarse FP8 grids round identically to the (double-precision) reference
#ವ್ಯಾಖ್ಯಾನಿಸು DEF_CVT_SCALE_FP8(S, T) \
    DEF_CVT_SCALE_SCALAR_FUNC(S##8u,   T, uchar, ದ್ವಿಗುಣ)     DEF_CVT_SCALE_SCALAR_FUNC(8u##S,   uchar, T, ದ್ವಿಗುಣ) \
    DEF_CVT_SCALE_SCALAR_FUNC(S##8s,   T, schar, ದ್ವಿಗುಣ)     DEF_CVT_SCALE_SCALAR_FUNC(8s##S,   schar, T, ದ್ವಿಗುಣ) \
    DEF_CVT_SCALE_SCALAR_FUNC(S##16u,  T, ushort, ದ್ವಿಗುಣ)    DEF_CVT_SCALE_SCALAR_FUNC(16u##S,  ushort, T, ದ್ವಿಗುಣ) \
    DEF_CVT_SCALE_SCALAR_FUNC(S##16s,  T, ಗಿಡ್ಡ, ದ್ವಿಗುಣ)     DEF_CVT_SCALE_SCALAR_FUNC(16s##S,  ಗಿಡ್ಡ, T, ದ್ವಿಗುಣ) \
    DEF_CVT_SCALE_SCALAR_FUNC(S##32u,  T, ಚಿಹ್ನೆರಹಿತ, ದ್ವಿಗುಣ)  DEF_CVT_SCALE_SCALAR_FUNC(32u##S,  ಚಿಹ್ನೆರಹಿತ, T, ದ್ವಿಗುಣ) \
    DEF_CVT_SCALE_SCALAR_FUNC(S##32s,  T, ಪೂರ್ಣಾಂಕ, ದ್ವಿಗುಣ)       DEF_CVT_SCALE_SCALAR_FUNC(32s##S,  ಪೂರ್ಣಾಂಕ, T, ದ್ವಿಗುಣ) \
    DEF_CVT_SCALE_SCALAR_FUNC(S##32f,  T, ದಶಮಾಂಶ, ದ್ವಿಗುಣ) \
    DEF_CVT_SCALE_SCALAR_FUNC(S##64f,  T, ದ್ವಿಗುಣ, ದ್ವಿಗುಣ)    DEF_CVT_SCALE_SCALAR_FUNC(64f##S,  ದ್ವಿಗುಣ, T, ದ್ವಿಗುಣ) \
    DEF_CVT_SCALE_SCALAR_FUNC(S##16f,  T, hfloat, ದ್ವಿಗುಣ)    DEF_CVT_SCALE_SCALAR_FUNC(16f##S,  hfloat, T, ದ್ವಿಗುಣ) \
    DEF_CVT_SCALE_SCALAR_FUNC(S##16bf, T, bfloat, ದ್ವಿಗುಣ)    DEF_CVT_SCALE_SCALAR_FUNC(16bf##S, bfloat, T, ದ್ವಿಗುಣ) \
    DEF_CVT_SCALE_SCALAR_FUNC(S##64u,  T, uint64_t, ದ್ವಿಗುಣ)  DEF_CVT_SCALE_SCALAR_FUNC(64u##S,  uint64_t, T, ದ್ವಿಗುಣ) \
    DEF_CVT_SCALE_SCALAR_FUNC(S##64s,  T, int64_t, ದ್ವಿಗುಣ)   DEF_CVT_SCALE_SCALAR_FUNC(64s##S,  int64_t, T, ದ್ವಿಗುಣ) \
    DEF_CVT_SCALE_SCALAR_FUNC(S##8b,   T, ತಾರ್ಕಿಕ, ದ್ವಿಗುಣ) \
    DEF_CVT_SCALE_SCALAR_FUNC(S##8fe4m3,  T, fp8_t, ದ್ವಿಗುಣ)   DEF_CVT_SCALE_SCALAR_FUNC(S##8fe4m3u, T, fp8a_t, ದ್ವಿಗುಣ)

DEF_CVT_SCALE_FP8(8fe4m3,  fp8_t)
DEF_CVT_SCALE_FP8(8fe4m3u, fp8a_t)

// convertTo(dst, depth, alpha, beta) path -- real quantization. Reuses convert.hpp's fast path.
// Double precision here matches the scalar reference (wtype)x*a+b bit-for-bit.
ಮಾದರಿ<ಪೂರ್ಣಾಂಕ bias, ತಾರ್ಕಿಕ fnuz> ಸ್ಥಿರ ಶೂನ್ಯ
cvt_Scale_F32_ಗಮ್ಯ_Fp8(ಸ್ಥಿರಾಂಕ uchar* src_, size_t sstep, ಸ್ಥಿರಾಂಕ uchar*, size_t,
                 uchar* dst, size_t dstep, ಗಾತ್ರ_2 size, ಶೂನ್ಯ* scale_)
{
    CV_INSTRUMENT_REGION();
    ಸ್ಥಿರಾಂಕ ದಶಮಾಂಶ* src = (ಸ್ಥಿರಾಂಕ ದಶಮಾಂಶ*)src_;
    ಸ್ಥಿರಾಂಕ ದ್ವಿಗುಣ* scale = (ಸ್ಥಿರಾಂಕ ದ್ವಿಗುಣ*)scale_;
    ದ್ವಿಗುಣ a = scale[0], b = scale[1];
    sstep /= ಗಾತ್ರ(src[0]);

    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < size.ಎತ್ತರ; i++, src += sstep, dst += dstep)
    {
        ಪೂರ್ಣಾಂಕ j = 0;
#ಆದರೆ (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)
        v_float64 va = vx_setall_f64(a), vb = vx_setall_f64(b);
        ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ LANES32 = VTraits<v_float32>::vlanes();
        ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ VECSZ = LANES32 * 4;
        ದಶಮಾಂಶ scaled[VTraits<v_float32>::max_nlanes * 4];
        ಗಾಗಿ (; j <= size.ಅಗಲ - VECSZ; j += VECSZ)
        {
            v_float32 vf0, vf1, vf2, vf3;
            v_float64 d0, d1;

            vx_ತುಂಬು_pair_as(src + j + 0 * LANES32, d0, d1);
            d0 = v_ಕೂಡಿಸು(v_mul(d0, va), vb);
            d1 = v_ಕೂಡಿಸು(v_mul(d1, va), vb);
            vf0 = v_cvt_f32(d0, d1);
            v_store(scaled + 0 * LANES32, vf0);

            vx_ತುಂಬು_pair_as(src + j + 1 * LANES32, d0, d1);
            d0 = v_ಕೂಡಿಸು(v_mul(d0, va), vb);
            d1 = v_ಕೂಡಿಸು(v_mul(d1, va), vb);
            vf1 = v_cvt_f32(d0, d1);
            v_store(scaled + 1 * LANES32, vf1);

            vx_ತುಂಬು_pair_as(src + j + 2 * LANES32, d0, d1);
            d0 = v_ಕೂಡಿಸು(v_mul(d0, va), vb);
            d1 = v_ಕೂಡಿಸು(v_mul(d1, va), vb);
            vf2 = v_cvt_f32(d0, d1);
            v_store(scaled + 2 * LANES32, vf2);

            vx_ತುಂಬು_pair_as(src + j + 3 * LANES32, d0, d1);
            d0 = v_ಕೂಡಿಸು(v_mul(d0, va), vb);
            d1 = v_ಕೂಡಿಸು(v_mul(d1, va), vb);
            vf3 = v_cvt_f32(d0, d1);
            v_store(scaled + 3 * LANES32, vf3);

            encodeFp8Vec4<bias, fnuz>(vf0, vf1, vf2, vf3, scaled, VECSZ, dst + j);
        }
        vx_cleanup();
#ಆದರೆ_ಅಂತ್ಯ
        ಗಾಗಿ (; j < size.ಅಗಲ; j++)
            dst[j] = fp8_detail::encodeE4M3((ದಶಮಾಂಶ)((ದ್ವಿಗುಣ)src[j]*a + b), bias, fnuz);
    }
}
ಸ್ಥಿರ ಶೂನ್ಯ cvtScale32f8fe4m3(ಸ್ಥಿರಾಂಕ uchar* s, size_t ss, ಸ್ಥಿರಾಂಕ uchar* p, size_t ps, uchar* d, size_t ds, ಗಾತ್ರ_2 sz, ಶೂನ್ಯ* x)
{ cvt_Scale_F32_ಗಮ್ಯ_Fp8<7, ಸುಳ್ಳು>(s, ss, p, ps, d, ds, sz, x); }
ಸ್ಥಿರ ಶೂನ್ಯ cvtScale32f8fe4m3u(ಸ್ಥಿರಾಂಕ uchar* s, size_t ss, ಸ್ಥಿರಾಂಕ uchar* p, size_t ps, uchar* d, size_t ds, ಗಾತ್ರ_2 sz, ಶೂನ್ಯ* x)
{ cvt_Scale_F32_ಗಮ್ಯ_Fp8<8, ನಿಜ>(s, ss, p, ps, d, ds, sz, x); }

ದ್ವಿಮಾನ_Func getConvertScaleFunc(ಪೂರ್ಣಾಂಕ sdepth_, ಪೂರ್ಣಾಂಕ ddepth_)
{
    ಪೂರ್ಣಾಂಕ sdepth = CV_MAT_ಆಳ(sdepth_);
    ಪೂರ್ಣಾಂಕ ddepth = CV_MAT_ಆಳ(ddepth_);
    ದ್ವಿಮಾನ_Func func =
        ddepth == CV_8U ? (
            sdepth == CV_8U ? cvtScale8u :
            sdepth == CV_8S ? cvtScale8s8u :
            sdepth == CV_Bool ? cvtScale8b8u :
            sdepth == CV_16U ? cvtScale16u8u :
            sdepth == CV_16S ? cvtScale16s8u :
            sdepth == CV_32U ? cvtScale32u8u :
            sdepth == CV_32S ? cvtScale32s8u :
            sdepth == CV_32F ? cvtScale32f8u :
            sdepth == CV_64F ? cvtScale64f8u :
            sdepth == CV_16F ? cvtScale16f8u :
            sdepth == CV_16BF ? cvtScale16bf8u :
            sdepth == CV_64U ? cvtScale64u8u :
            sdepth == CV_64S ? cvtScale64s8u :
            sdepth == CV_8F_E4M3FN ? cvtScale8fe4m38u :
            sdepth == CV_8F_E4M3FNUZ ? cvtScale8fe4m3u8u :
            0) :
        ddepth == CV_8S ? (
            sdepth == CV_8U ? cvtScale8u8s :
            sdepth == CV_8S ? cvtScale8s :
            sdepth == CV_Bool ? cvtScale8b8s :
            sdepth == CV_16U ? cvtScale16u8s :
            sdepth == CV_16S ? cvtScale16s8s :
            sdepth == CV_32U ? cvtScale32u8s :
            sdepth == CV_32S ? cvtScale32s8s :
            sdepth == CV_32F ? cvtScale32f8s :
            sdepth == CV_64F ? cvtScale64f8s :
            sdepth == CV_16F ? cvtScale16f8s :
            sdepth == CV_16BF ? cvtScale16bf8s :
            sdepth == CV_64U ? cvtScale64u8s :
            sdepth == CV_64S ? cvtScale64s8s :
            sdepth == CV_8F_E4M3FN ? cvtScale8fe4m38s :
            sdepth == CV_8F_E4M3FNUZ ? cvtScale8fe4m3u8s :
            0) :
        ddepth == CV_16U ? (
            sdepth == CV_8U ? cvtScale8u16u :
            sdepth == CV_8S ? cvtScale8s16u :
            sdepth == CV_Bool ? cvtScale8b16u :
            sdepth == CV_16U ? cvtScale16u :
            sdepth == CV_16S ? cvtScale16s16u :
            sdepth == CV_32U ? cvtScale32u16u :
            sdepth == CV_32S ? cvtScale32s16u :
            sdepth == CV_32F ? cvtScale32f16u :
            sdepth == CV_64F ? cvtScale64f16u :
            sdepth == CV_16F ? cvtScale16f16u :
            sdepth == CV_16BF ? cvtScale16bf16u :
            sdepth == CV_64U ? cvtScale64u16u :
            sdepth == CV_64S ? cvtScale64s16u :
            sdepth == CV_8F_E4M3FN ? cvtScale8fe4m316u :
            sdepth == CV_8F_E4M3FNUZ ? cvtScale8fe4m3u16u :
            0) :
        ddepth == CV_16S ? (
            sdepth == CV_8U ? cvtScale8u16s :
            sdepth == CV_8S ? cvtScale8s16s :
            sdepth == CV_Bool ? cvtScale8b16s :
            sdepth == CV_16U ? cvtScale16u16s :
            sdepth == CV_16S ? cvtScale16s :
            sdepth == CV_32U ? cvtScale32u16s :
            sdepth == CV_32S ? cvtScale32s16s :
            sdepth == CV_32F ? cvtScale32f16s :
            sdepth == CV_64F ? cvtScale64f16s :
            sdepth == CV_16F ? cvtScale16f16s :
            sdepth == CV_16BF ? cvtScale16bf16s :
            sdepth == CV_64U ? cvtScale64u16s :
            sdepth == CV_64S ? cvtScale64s16s :
            sdepth == CV_8F_E4M3FN ? cvtScale8fe4m316s :
            sdepth == CV_8F_E4M3FNUZ ? cvtScale8fe4m3u16s :
            0) :
        ddepth == CV_32U ? (
            sdepth == CV_8U ? cvtScale8u32u :
            sdepth == CV_8S ? cvtScale8s32u :
            sdepth == CV_Bool ? cvtScale8b32u :
            sdepth == CV_16U ? cvtScale16u32u :
            sdepth == CV_16S ? cvtScale16s32u :
            sdepth == CV_32U ? cvtScale32u :
            sdepth == CV_32S ? cvtScale32s32u :
            sdepth == CV_32F ? cvtScale32f32u :
            sdepth == CV_64F ? cvtScale64f32u :
            sdepth == CV_16F ? cvtScale16f32u :
            sdepth == CV_16BF ? cvtScale16bf32u :
            sdepth == CV_64U ? cvtScale64u32u :
            sdepth == CV_64S ? cvtScale64s32u :
            sdepth == CV_8F_E4M3FN ? cvtScale8fe4m332u :
            sdepth == CV_8F_E4M3FNUZ ? cvtScale8fe4m3u32u :
            0) :
        ddepth == CV_32S ? (
            sdepth == CV_8U ? cvtScale8u32s :
            sdepth == CV_8S ? cvtScale8s32s :
            sdepth == CV_Bool ? cvtScale8b32s :
            sdepth == CV_16U ? cvtScale16u32s :
            sdepth == CV_16S ? cvtScale16s32s :
            sdepth == CV_32U ? cvtScale32u32s :
            sdepth == CV_32S ? cvtScale32s :
            sdepth == CV_32F ? cvtScale32f32s :
            sdepth == CV_64F ? cvtScale64f32s :
            sdepth == CV_16F ? cvtScale16f32s :
            sdepth == CV_16BF ? cvtScale16bf32s :
            sdepth == CV_64U ? cvtScale64u32s :
            sdepth == CV_64S ? cvtScale64s32s :
            sdepth == CV_8F_E4M3FN ? cvtScale8fe4m332s :
            sdepth == CV_8F_E4M3FNUZ ? cvtScale8fe4m3u32s :
            0) :
        ddepth == CV_32F ? (
            sdepth == CV_8U ? cvtScale8u32f :
            sdepth == CV_8S ? cvtScale8s32f :
            sdepth == CV_Bool ? cvtScale8b32f :
            sdepth == CV_16U ? cvtScale16u32f :
            sdepth == CV_16S ? cvtScale16s32f :
            sdepth == CV_32U ? cvtScale32u32f :
            sdepth == CV_32S ? cvtScale32s32f :
            sdepth == CV_32F ? cvtScale32f :
            sdepth == CV_64F ? cvtScale64f32f :
            sdepth == CV_16F ? cvtScale16f32f :
            sdepth == CV_16BF ? cvtScale16bf32f :
            sdepth == CV_64U ? cvtScale64u32f :
            sdepth == CV_64S ? cvtScale64s32f :
            sdepth == CV_8F_E4M3FN ? cvtScale8fe4m332f :
            sdepth == CV_8F_E4M3FNUZ ? cvtScale8fe4m3u32f :
            0) :
        ddepth == CV_64F ? (
            sdepth == CV_8U ? cvtScale8u64f :
            sdepth == CV_8S ? cvtScale8s64f :
            sdepth == CV_Bool ? cvtScale8b64f :
            sdepth == CV_16U ? cvtScale16u64f :
            sdepth == CV_16S ? cvtScale16s64f :
            sdepth == CV_32U ? cvtScale32u64f :
            sdepth == CV_32S ? cvtScale32s64f :
            sdepth == CV_32F ? cvtScale32f64f :
            sdepth == CV_64F ? cvtScale64f :
            sdepth == CV_16F ? cvtScale16f64f :
            sdepth == CV_16BF ? cvtScale16bf64f :
            sdepth == CV_64U ? cvtScale64u64f :
            sdepth == CV_64S ? cvtScale64s64f :
            sdepth == CV_8F_E4M3FN ? cvtScale8fe4m364f :
            sdepth == CV_8F_E4M3FNUZ ? cvtScale8fe4m3u64f :
            0) :
        ddepth == CV_16F ? (
            sdepth == CV_8U ? cvtScale8u16f :
            sdepth == CV_8S ? cvtScale8s16f :
            sdepth == CV_Bool ? cvtScale8b16f :
            sdepth == CV_16U ? cvtScale16u16f :
            sdepth == CV_16S ? cvtScale16s16f :
            sdepth == CV_32U ? cvtScale32u16f :
            sdepth == CV_32S ? cvtScale32s16f :
            sdepth == CV_32F ? cvtScale32f16f :
            sdepth == CV_64F ? cvtScale64f16f :
            sdepth == CV_16F ? cvtScale16f :
            sdepth == CV_16BF ? cvtScale16bf16f :
            sdepth == CV_64U ? cvtScale64u16f :
            sdepth == CV_64S ? cvtScale64s16f :
            sdepth == CV_8F_E4M3FN ? cvtScale8fe4m316f :
            sdepth == CV_8F_E4M3FNUZ ? cvtScale8fe4m3u16f :
            0) :
        ddepth == CV_16BF ? (
            sdepth == CV_8U ? cvtScale8u16bf :
            sdepth == CV_8S ? cvtScale8s16bf :
            sdepth == CV_Bool ? cvtScale8b16bf :
            sdepth == CV_16U ? cvtScale16u16bf :
            sdepth == CV_16S ? cvtScale16s16bf :
            sdepth == CV_32U ? cvtScale32u16bf :
            sdepth == CV_32S ? cvtScale32s16bf :
            sdepth == CV_32F ? cvtScale32f16bf :
            sdepth == CV_64F ? cvtScale64f16bf :
            sdepth == CV_16F ? cvtScale16f16bf :
            sdepth == CV_16BF ? cvtScale16bf :
            sdepth == CV_64U ? cvtScale64u16bf :
            sdepth == CV_64S ? cvtScale64s16bf :
            sdepth == CV_8F_E4M3FN ? cvtScale8fe4m316bf :
            sdepth == CV_8F_E4M3FNUZ ? cvtScale8fe4m3u16bf :
            0) :
        ddepth == CV_Bool ? (
            sdepth == CV_8U ? cvtScale8u8b :
            sdepth == CV_8S ? cvtScale8s8b :
            sdepth == CV_Bool ? cvtScale8b :
            sdepth == CV_16U ? cvtScale16u8b :
            sdepth == CV_16S ? cvtScale16s8b :
            sdepth == CV_32U ? cvtScale32u8b :
            sdepth == CV_32S ? cvtScale32s8b :
            sdepth == CV_32F ? cvtScale32f8b :
            sdepth == CV_64F ? cvtScale64f8b :
            sdepth == CV_16F ? cvtScale16f8b :
            sdepth == CV_16BF ? cvtScale16bf8b :
            sdepth == CV_64U ? cvtScale64u8b :
            sdepth == CV_64S ? cvtScale64s8b :
            sdepth == CV_8F_E4M3FN ? cvtScale8fe4m38b :
            sdepth == CV_8F_E4M3FNUZ ? cvtScale8fe4m3u8b :
            0) :
        ddepth == CV_64U ? (
            sdepth == CV_8U ? cvtScale8u64u :
            sdepth == CV_8S ? cvtScale8s64u :
            sdepth == CV_Bool ? cvtScale8b64u :
            sdepth == CV_16U ? cvtScale16u64u :
            sdepth == CV_16S ? cvtScale16s64u :
            sdepth == CV_32U ? cvtScale32u64u :
            sdepth == CV_32S ? cvtScale32s64u :
            sdepth == CV_32F ? cvtScale32f64u :
            sdepth == CV_64F ? cvtScale64f64u :
            sdepth == CV_16F ? cvtScale16f64u :
            sdepth == CV_16BF ? cvtScale16bf64u :
            sdepth == CV_64U ? cvtScale64u :
            sdepth == CV_64S ? cvtScale64s64u :
            sdepth == CV_8F_E4M3FN ? cvtScale8fe4m364u :
            sdepth == CV_8F_E4M3FNUZ ? cvtScale8fe4m3u64u :
            0) :
        ddepth == CV_64S ? (
            sdepth == CV_8U ? cvtScale8u64s :
            sdepth == CV_8S ? cvtScale8s64s :
            sdepth == CV_Bool ? cvtScale8b64s :
            sdepth == CV_16U ? cvtScale16u64s :
            sdepth == CV_16S ? cvtScale16s64s :
            sdepth == CV_32U ? cvtScale32u64s :
            sdepth == CV_32S ? cvtScale32s64s :
            sdepth == CV_32F ? cvtScale32f64s :
            sdepth == CV_64F ? cvtScale64f64s :
            sdepth == CV_16F ? cvtScale16f64s :
            sdepth == CV_16BF ? cvtScale16bf64s :
            sdepth == CV_64U ? cvtScale64u64s :
            sdepth == CV_64S ? cvtScale64s :
            sdepth == CV_8F_E4M3FN ? cvtScale8fe4m364s :
            sdepth == CV_8F_E4M3FNUZ ? cvtScale8fe4m3u64s :
            0) :
        ddepth == CV_8F_E4M3FN ? (
            sdepth == CV_8U ? cvtScale8u8fe4m3 :
            sdepth == CV_8S ? cvtScale8s8fe4m3 :
            sdepth == CV_Bool ? cvtScale8u8fe4m3 : // bool stored as 0/1 byte -> reuse uchar path
            sdepth == CV_16U ? cvtScale16u8fe4m3 :
            sdepth == CV_16S ? cvtScale16s8fe4m3 :
            sdepth == CV_32U ? cvtScale32u8fe4m3 :
            sdepth == CV_32S ? cvtScale32s8fe4m3 :
            sdepth == CV_32F ? cvtScale32f8fe4m3 :
            sdepth == CV_64F ? cvtScale64f8fe4m3 :
            sdepth == CV_16F ? cvtScale16f8fe4m3 :
            sdepth == CV_16BF ? cvtScale16bf8fe4m3 :
            sdepth == CV_64U ? cvtScale64u8fe4m3 :
            sdepth == CV_64S ? cvtScale64s8fe4m3 :
            sdepth == CV_8F_E4M3FN ? cvtScale8fe4m38fe4m3 :
            sdepth == CV_8F_E4M3FNUZ ? cvtScale8fe4m3u8fe4m3 :
            0) :
        ddepth == CV_8F_E4M3FNUZ ? (
            sdepth == CV_8U ? cvtScale8u8fe4m3u :
            sdepth == CV_8S ? cvtScale8s8fe4m3u :
            sdepth == CV_Bool ? cvtScale8u8fe4m3u : // bool stored as 0/1 byte -> reuse uchar path
            sdepth == CV_16U ? cvtScale16u8fe4m3u :
            sdepth == CV_16S ? cvtScale16s8fe4m3u :
            sdepth == CV_32U ? cvtScale32u8fe4m3u :
            sdepth == CV_32S ? cvtScale32s8fe4m3u :
            sdepth == CV_32F ? cvtScale32f8fe4m3u :
            sdepth == CV_64F ? cvtScale64f8fe4m3u :
            sdepth == CV_16F ? cvtScale16f8fe4m3u :
            sdepth == CV_16BF ? cvtScale16bf8fe4m3u :
            sdepth == CV_64U ? cvtScale64u8fe4m3u :
            sdepth == CV_64S ? cvtScale64s8fe4m3u :
            sdepth == CV_8F_E4M3FN ? cvtScale8fe4m38fe4m3u :
            sdepth == CV_8F_E4M3FNUZ ? cvtScale8fe4m3u8fe4m3u :
            0) :
        0;
    CV_Assert(func != 0);
    ಹಿಂತಿರುಗಿ func;
}

#ಆದರೆ_ಅಂತ್ಯ

CV_CPU_OPTIMIZATION_NAMESPACE_ಕೊನೆ
} // namespace
