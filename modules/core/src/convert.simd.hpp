// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html

#ಅನ್ತರ್ಭಾವ "precomp.hpp"
#ಅನ್ತರ್ಭಾವ "convert.hpp"

#ಆದರೆ !defined(OPENCV_SUPRESS_WARNING_AVX2_WITHOUT_FP16C) && \
    (defined(__GNUC__) && defined(__AVX2__) && !defined(__F16C__))
#warning "Non-optimal compiler flags: AVX2 without FP16. Generated code is very slow. Consider adding '-mf16c' compiler option."
#ಆದರೆ_ಅಂತ್ಯ

ನಾಮಾಕಾಶ cv {
ನಾಮಾಕಾಶ hal {
CV_CPU_OPTIMIZATION_NAMESPACE_BEGIN

ಶೂನ್ಯ cvt16f32f(ಸ್ಥಿರಾಂಕ hfloat* src, ದಶಮಾಂಶ* dst, ಪೂರ್ಣಾಂಕ len);
ಶೂನ್ಯ cvt32f16f(ಸ್ಥಿರಾಂಕ ದಶಮಾಂಶ* src, hfloat* dst, ಪೂರ್ಣಾಂಕ len);
ಶೂನ್ಯ cvt16bf32f(ಸ್ಥಿರಾಂಕ bfloat* src, ದಶಮಾಂಶ* dst, ಪೂರ್ಣಾಂಕ len);
ಶೂನ್ಯ cvt32f16bf(ಸ್ಥಿರಾಂಕ ದಶಮಾಂಶ* src, bfloat* dst, ಪೂರ್ಣಾಂಕ len);
ಶೂನ್ಯ ಕೂಡಿಸು_RNGBias32f(ದಶಮಾಂಶ* arr, ಸ್ಥಿರಾಂಕ ದಶಮಾಂಶ* scale_Bias_ಜೋಡಿಗಳು, ಪೂರ್ಣಾಂಕ len, ಪೂರ್ಣಾಂಕ cn);
ಶೂನ್ಯ ಕೂಡಿಸು_RNGBias64f(ದ್ವಿಗುಣ* arr, ಸ್ಥಿರಾಂಕ ದ್ವಿಗುಣ* scale_Bias_ಜೋಡಿಗಳು, ಪೂರ್ಣಾಂಕ len, ಪೂರ್ಣಾಂಕ cn);

CV_CPU_OPTIMIZATION_NAMESPACE_ಕೊನೆ
} // namespace cv::hal

CV_CPU_OPTIMIZATION_NAMESPACE_BEGIN

ದ್ವಿಮಾನ_Func getConvertFunc(ಪೂರ್ಣಾಂಕ sdepth, ಪೂರ್ಣಾಂಕ ddepth);

CV_CPU_OPTIMIZATION_NAMESPACE_ಕೊನೆ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

ನಾಮಾಕಾಶ hal {
CV_CPU_OPTIMIZATION_NAMESPACE_BEGIN

ದ್ವಿಮಾನ_Func getConvertFunc(ಪೂರ್ಣಾಂಕ sdepth, ಪೂರ್ಣಾಂಕ ddepth);

ಶೂನ್ಯ cvt16f32f( ಸ್ಥಿರಾಂಕ hfloat* src, ದಶಮಾಂಶ* dst, ಪೂರ್ಣಾಂಕ len )
{
    CV_INSTRUMENT_REGION();
    ಪೂರ್ಣಾಂಕ j = 0;
#ಆದರೆ (CV_SIMD || CV_SIMD_SCALABLE)
    ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ VECSZ = VTraits<v_float32>::vlanes();
    ಗಾಗಿ( ; j < len; j += VECSZ )
    {
        ಆದರೆ( j > len - VECSZ )
        {
            ಆದರೆ( j == 0 )
                ನಿಲ್ಲಿಸು;
            j = len - VECSZ;
        }
        v_store(dst + j, vx_ತುಂಬು_expand(src + j));
    }
#ಆದರೆ_ಅಂತ್ಯ
    ಗಾಗಿ( ; j < len; j++ )
        dst[j] = (ದಶಮಾಂಶ)src[j];
}

ಶೂನ್ಯ cvt32f16f( ಸ್ಥಿರಾಂಕ ದಶಮಾಂಶ* src, hfloat* dst, ಪೂರ್ಣಾಂಕ len )
{
    CV_INSTRUMENT_REGION();
    ಪೂರ್ಣಾಂಕ j = 0;
#ಆದರೆ (CV_SIMD || CV_SIMD_SCALABLE)
    ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ VECSZ = VTraits<v_float32>::vlanes();
    ಗಾಗಿ( ; j < len; j += VECSZ )
    {
        ಆದರೆ( j > len - VECSZ )
        {
            ಆದರೆ( j == 0 )
                ನಿಲ್ಲಿಸು;
            j = len - VECSZ;
        }
        v_pack_store(dst + j, vx_ತುಂಬು(src + j));
    }
#ಆದರೆ_ಅಂತ್ಯ
    ಗಾಗಿ( ; j < len; j++ )
        dst[j] = hfloat(src[j]);
}

ಶೂನ್ಯ cvt32f16bf( ಸ್ಥಿರಾಂಕ ದಶಮಾಂಶ* src, bfloat* dst, ಪೂರ್ಣಾಂಕ len )
{
    CV_INSTRUMENT_REGION();
    ಪೂರ್ಣಾಂಕ j = 0;
#ಆದರೆ (CV_SIMD || CV_SIMD_SCALABLE)
    ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ VECSZ = VTraits<v_float32>::vlanes();
    ಗಾಗಿ( ; j < len; j += VECSZ )
    {
        ಆದರೆ( j > len - VECSZ )
        {
            ಆದರೆ( j == 0 )
                ನಿಲ್ಲಿಸು;
            j = len - VECSZ;
        }
        v_pack_store(dst + j, vx_ತುಂಬು(src + j));
    }
#ಆದರೆ_ಅಂತ್ಯ
    ಗಾಗಿ( ; j < len; j++ )
        dst[j] = bfloat(src[j]);
}

ಶೂನ್ಯ ಕೂಡಿಸು_RNGBias32f( ದಶಮಾಂಶ* arr, ಸ್ಥಿರಾಂಕ ದಶಮಾಂಶ* scale_Bias_ಜೋಡಿಗಳು, ಪೂರ್ಣಾಂಕ len, ಪೂರ್ಣಾಂಕ cn )
{
    CV_INSTRUMENT_REGION();
    ಆದರೆ (cn == 1) {
        ದಶಮಾಂಶ bias = scale_Bias_ಜೋಡಿಗಳು[1];
        ಗಾಗಿ( ಪೂರ್ಣಾಂಕ i = 0; i < len; i++ ) {
            arr[i] += bias;
        }
    } ಇಲ್ಲದಿದ್ದರೆ {
        ಪೂರ್ಣಾಂಕ k = 0;
        len *= cn;
        cn--;
        ಗಾಗಿ( ಪೂರ್ಣಾಂಕ i = 0; i < len; i++ ) {
            arr[i] += scale_Bias_ಜೋಡಿಗಳು[k*2 + 1];
            k = (k + 1) & ((k >= cn) - 1);
        }
    }
}

ಶೂನ್ಯ ಕೂಡಿಸು_RNGBias64f( ದ್ವಿಗುಣ* arr, ಸ್ಥಿರಾಂಕ ದ್ವಿಗುಣ* scale_Bias_ಜೋಡಿಗಳು, ಪೂರ್ಣಾಂಕ len, ಪೂರ್ಣಾಂಕ cn )
{
    CV_INSTRUMENT_REGION();
    ಆದರೆ (cn == 1) {
        ದ್ವಿಗುಣ bias = scale_Bias_ಜೋಡಿಗಳು[1];
        ಗಾಗಿ( ಪೂರ್ಣಾಂಕ i = 0; i < len; i++ ) {
            arr[i] += bias;
        }
    } ಇಲ್ಲದಿದ್ದರೆ {
        ಪೂರ್ಣಾಂಕ k = 0;
        len *= cn;
        cn--;
        ಗಾಗಿ( ಪೂರ್ಣಾಂಕ i = 0; i < len; i++ ) {
            arr[i] += scale_Bias_ಜೋಡಿಗಳು[k*2 + 1];
            k = (k + 1) & ((k >= cn) - 1);
        }
    }
}

CV_CPU_OPTIMIZATION_NAMESPACE_ಕೊನೆ
} // namespace cv::hal

// cv::
CV_CPU_OPTIMIZATION_NAMESPACE_BEGIN

ಮಾದರಿ<ಬಗೆ_ಹೆಸರು _Ts, ಬಗೆ_ಹೆಸರು _Td, ಬಗೆ_ಹೆಸರು _Twvec> ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ
cvt_( ಸ್ಥಿರಾಂಕ _Ts* src, size_t sstep, _Td* dst, size_t dstep, ಗಾತ್ರ_2 size )
{
    sstep /= ಗಾತ್ರ(src[0]);
    dstep /= ಗಾತ್ರ(dst[0]);

    ಗಾಗಿ( ಪೂರ್ಣಾಂಕ i = 0; i < size.ಎತ್ತರ; i++, src += sstep, dst += dstep )
    {
        ಪೂರ್ಣಾಂಕ j = 0;
// Excluding GNU in CV_SIMD_SCALABLE because of "opencv/issues/26936"
#ಆದರೆ (CV_SIMD || (CV_SIMD_SCALABLE && !(defined(__GNUC__) && !defined(__clang__))) )
        ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ VECSZ = VTraits<_Twvec>::vlanes()*2;
        ಗಾಗಿ( ; j < size.ಅಗಲ; j += VECSZ )
        {
            ಆದರೆ( j > size.ಅಗಲ - VECSZ )
            {
                ಆದರೆ( j == 0 || src == (_Ts*)dst )
                    ನಿಲ್ಲಿಸು;
                j = size.ಅಗಲ - VECSZ;
            }
            _Twvec v0, v1;
            vx_ತುಂಬು_pair_as(src + j, v0, v1);
            v_store_pair_as(dst + j, v0, v1);
        }
#ಆದರೆ_ಅಂತ್ಯ
        ಗಾಗಿ( ; j < size.ಅಗಲ; j++ )
            dst[j] = saturate_cast<_Td>(src[j]);
    }
}

ಮಾದರಿ<ಬಗೆ_ಹೆಸರು _Ts, ಬಗೆ_ಹೆಸರು _Td, ಬಗೆ_ಹೆಸರು dummy> ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ
cvt_64f( ಸ್ಥಿರಾಂಕ _Ts* src, size_t sstep, _Td* dst, size_t dstep, ಗಾತ್ರ_2 size )
{
    sstep /= ಗಾತ್ರ(src[0]);
    dstep /= ಗಾತ್ರ(dst[0]);

    ಗಾಗಿ( ಪೂರ್ಣಾಂಕ i = 0; i < size.ಎತ್ತರ; i++, src += sstep, dst += dstep )
    {
        ಪೂರ್ಣಾಂಕ j = 0;
#ಆದರೆ (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)
        ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ VECSZ = VTraits<v_float64>::vlanes()*2;
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
            v_store_pair_as(dst + j, v0, v1);
        }
#ಆದರೆ_ಅಂತ್ಯ
        ಗಾಗಿ( ; j < size.ಅಗಲ; j++ )
            dst[j] = saturate_cast<_Td>(src[j]);
    }
}

// in order to reduce the code size, for (16f <-> ...) conversions
// we add a conversion function without loop unrolling
ಮಾದರಿ<ಬಗೆ_ಹೆಸರು _Ts, ಬಗೆ_ಹೆಸರು _Td, ಬಗೆ_ಹೆಸರು _Twvec> ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ
cvt1_( ಸ್ಥಿರಾಂಕ _Ts* src, size_t sstep, _Td* dst, size_t dstep, ಗಾತ್ರ_2 size )
{
    sstep /= ಗಾತ್ರ(src[0]);
    dstep /= ಗಾತ್ರ(dst[0]);

    ಗಾಗಿ( ಪೂರ್ಣಾಂಕ i = 0; i < size.ಎತ್ತರ; i++, src += sstep, dst += dstep )
    {
        ಪೂರ್ಣಾಂಕ j = 0;
#ಆದರೆ (CV_SIMD || CV_SIMD_SCALABLE)
        ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ VECSZ = VTraits<_Twvec>::vlanes();
        ಗಾಗಿ( ; j < size.ಅಗಲ; j += VECSZ )
        {
            ಆದರೆ( j > size.ಅಗಲ - VECSZ )
            {
                ಆದರೆ( j == 0 || src == (_Ts*)dst )
                    ನಿಲ್ಲಿಸು;
                j = size.ಅಗಲ - VECSZ;
            }
            _Twvec v;
            vx_ತುಂಬು_as(src + j, v);
            v_store_as(dst + j, v);
        }
        vx_cleanup();
#ಆದರೆ_ಅಂತ್ಯ
        ಗಾಗಿ( ; j < size.ಅಗಲ; j++ )
            dst[j] = saturate_cast<_Td>(src[j]);
    }
}

ಸ್ಥಿರ ಶೂನ್ಯ cvtCopy( ಸ್ಥಿರಾಂಕ uchar* src, size_t sstep,
                     uchar* dst, size_t dstep, ಗಾತ್ರ_2 size, size_t elemsize)
{
    size_t len = size.ಅಗಲ*elemsize;
    ಗಾಗಿ( ಪೂರ್ಣಾಂಕ i = 0; i < size.ಎತ್ತರ; i++, src += sstep, dst += dstep )
    {
        ಸ್ಮರಣೆ_ನಕಲು( dst, src, len );
    }
}

#ವ್ಯಾಖ್ಯಾನಿಸು DEF_CVT_FUNC(suffix, cvtfunc, _Ts, _Td, _Twvec) \
ಸ್ಥಿರ ಶೂನ್ಯ cvt##suffix(ಸ್ಥಿರಾಂಕ uchar* src_, size_t sstep, ಸ್ಥಿರಾಂಕ uchar*, size_t, \
                        uchar* dst_, size_t dstep, ಗಾತ್ರ_2 size, ಶೂನ್ಯ*) \
{ \
    CV_INSTRUMENT_REGION(); \
    ಸ್ಥಿರಾಂಕ _Ts* src = (ಸ್ಥಿರಾಂಕ _Ts*)src_; \
    _Td* dst = (_Td*)dst_; \
    cvtfunc<_Ts, _Td, _Twvec>(src, sstep, dst, dstep, size); \
}

#ವ್ಯಾಖ್ಯಾನಿಸು DEF_CVT2BOOL_FUNC(suffix, _Ts, ಸರಿಸು) \
ಸ್ಥಿರ ಶೂನ್ಯ cvt##suffix(ಸ್ಥಿರಾಂಕ uchar* src_, size_t sstep, ಸ್ಥಿರಾಂಕ uchar*, size_t, \
                        uchar* dst, size_t dstep, ಗಾತ್ರ_2 size, ಶೂನ್ಯ*) \
{ \
    CV_INSTRUMENT_REGION(); \
    ಸ್ಥಿರಾಂಕ _Ts* src = (ಸ್ಥಿರಾಂಕ _Ts*)src_; \
    sstep /= ಗಾತ್ರ(src[0]); \
    \
    ಗಾಗಿ( ಪೂರ್ಣಾಂಕ i = 0; i < size.ಎತ್ತರ; i++, src += sstep, dst += dstep ) { \
        ಗಾಗಿ ( ಪೂರ್ಣಾಂಕ j = 0; j < size.ಅಗಲ; j++ ) \
            dst[j] = (src[j]<<ಸರಿಸು) != 0; \
    } \
}

#ವ್ಯಾಖ್ಯಾನಿಸು DEF_CVTBOOL2_FUNC(suffix, _Td, scale) \
ಸ್ಥಿರ ಶೂನ್ಯ cvt##suffix(ಸ್ಥಿರಾಂಕ uchar* src, size_t sstep, ಸ್ಥಿರಾಂಕ uchar*, size_t, \
                        uchar* dst_, size_t dstep, ಗಾತ್ರ_2 size, ಶೂನ್ಯ*) \
{ \
    CV_INSTRUMENT_REGION(); \
    _Td* dst = (_Td*)dst_; \
    dstep /= ಗಾತ್ರ(dst[0]); \
    \
    ಗಾಗಿ( ಪೂರ್ಣಾಂಕ i = 0; i < size.ಎತ್ತರ; i++, src += sstep, dst += dstep ) { \
        ಗಾಗಿ ( ಪೂರ್ಣಾಂಕ j = 0; j < size.ಅಗಲ; j++ ) \
            dst[j] = (_Td)((src[j] != 0)*scale); \
    } \
}

#ವ್ಯಾಖ್ಯಾನಿಸು DEF_CVT_SCALAR_FUNC(suffix, _Ts, _Td) \
ಸ್ಥಿರ ಶೂನ್ಯ cvt##suffix(ಸ್ಥಿರಾಂಕ uchar* src_, size_t sstep, ಸ್ಥಿರಾಂಕ uchar*, size_t, \
                        uchar* dst_, size_t dstep, ಗಾತ್ರ_2 size, ಶೂನ್ಯ*) \
{ \
    CV_INSTRUMENT_REGION(); \
    ಸ್ಥಿರಾಂಕ _Ts* src = (ಸ್ಥಿರಾಂಕ _Ts*)src_; \
    _Td* dst = (_Td*)dst_; \
    sstep /= ಗಾತ್ರ(src[0]); \
    dstep /= ಗಾತ್ರ(dst[0]); \
    \
    ಗಾಗಿ( ಪೂರ್ಣಾಂಕ i = 0; i < size.ಎತ್ತರ; i++, src += sstep, dst += dstep ) { \
        ಗಾಗಿ ( ಪೂರ್ಣಾಂಕ j = 0; j < size.ಅಗಲ; j++ ) \
            dst[j] = saturate_cast<_Td>(src[j]); \
    } \
}

#ವ್ಯಾಖ್ಯಾನಿಸು DEF_CVT_SCALAR_FUNC_S2U(suffix, _Ts, _Td, _Tw) \
ಸ್ಥಿರ ಶೂನ್ಯ cvt##suffix(ಸ್ಥಿರಾಂಕ uchar* src_, size_t sstep, ಸ್ಥಿರಾಂಕ uchar*, size_t, \
                        uchar* dst_, size_t dstep, ಗಾತ್ರ_2 size, ಶೂನ್ಯ*) \
{ \
    CV_INSTRUMENT_REGION(); \
    ಸ್ಥಿರಾಂಕ _Ts* src = (ಸ್ಥಿರಾಂಕ _Ts*)src_; \
    _Td* dst = (_Td*)dst_; \
    sstep /= ಗಾತ್ರ(src[0]); \
    dstep /= ಗಾತ್ರ(dst[0]); \
    \
    ಗಾಗಿ( ಪೂರ್ಣಾಂಕ i = 0; i < size.ಎತ್ತರ; i++, src += sstep, dst += dstep ) { \
        ಗಾಗಿ ( ಪೂರ್ಣಾಂಕ j = 0; j < size.ಅಗಲ; j++ ) \
            dst[j] = saturate_cast<_Td>(std::max((_Tw)src[j], (_Tw)0)); \
    } \
}

////////////////////// 8u -> ... ////////////////////////

DEF_CVT_FUNC(8u8s,  cvt_,  uchar, schar,    v_int16)
DEF_CVT_FUNC(8u16s, cvt_,  uchar, ಗಿಡ್ಡ,    v_int16)
DEF_CVT_FUNC(8u32s, cvt_,  uchar, ಪೂರ್ಣಾಂಕ,      v_int32)
DEF_CVT_FUNC(8u32f, cvt_,  uchar, ದಶಮಾಂಶ,    v_float32)
DEF_CVT_FUNC(8u64f, cvt_,  uchar, ದ್ವಿಗುಣ,   v_int32)
DEF_CVT_SCALAR_FUNC(8u64s, uchar, int64_t)
DEF_CVT_FUNC(8u16f, cvt1_, uchar, hfloat, v_float32)
DEF_CVT_FUNC(8u16bf, cvt1_, uchar, bfloat, v_float32)
DEF_CVT2BOOL_FUNC(8u8b, uchar, 0)

////////////////////// 8s -> ... ////////////////////////

DEF_CVT_FUNC(8s8u,  cvt_,  schar, uchar,    v_int16)
DEF_CVT_FUNC(8s16u, cvt_,  schar, ushort,   v_uint16)
DEF_CVT_FUNC(8s16s, cvt_,  schar, ಗಿಡ್ಡ,    v_int16)
DEF_CVT_FUNC(8s32u, cvt_,  schar, ಚಿಹ್ನೆರಹಿತ, v_uint32)
DEF_CVT_FUNC(8s32s, cvt_,  schar, ಪೂರ್ಣಾಂಕ,      v_int32)
DEF_CVT_FUNC(8s32f, cvt_,  schar, ದಶಮಾಂಶ,    v_float32)
DEF_CVT_FUNC(8s64f, cvt_,  schar, ದ್ವಿಗುಣ,   v_int32)
DEF_CVT_FUNC(8s64u, cvt_,  schar, uint64_t, v_uint32)
DEF_CVT_FUNC(8s64s, cvt_,  schar, int64_t,  v_int32)
DEF_CVT_FUNC(8s16f, cvt1_, schar, hfloat, v_float32)
DEF_CVT_FUNC(8s16bf, cvt1_, schar, bfloat, v_float32)

////////////////////// 8b -> ... ////////////////////////

DEF_CVTBOOL2_FUNC(8b8u,  uchar, 1)
DEF_CVTBOOL2_FUNC(8b16s, ಗಿಡ್ಡ, 1)
DEF_CVTBOOL2_FUNC(8b32s, ಪೂರ್ಣಾಂಕ, 1)
DEF_CVTBOOL2_FUNC(8b32f, ದಶಮಾಂಶ, 1)
DEF_CVTBOOL2_FUNC(8b64f, ದ್ವಿಗುಣ, 1)
DEF_CVTBOOL2_FUNC(8b64s, int64_t, 1)
DEF_CVTBOOL2_FUNC(8b16f, uint16_t, 0x3c00) // hfloat(1.0f)
DEF_CVTBOOL2_FUNC(8b16bf, uint16_t, 0x3f80) // bfloat(1.0f)

////////////////////// 16u -> ... ////////////////////////

DEF_CVT_FUNC(16u8u,  cvt_, ushort, uchar,  v_uint16)
DEF_CVT_FUNC(16u8s,  cvt_, ushort, schar,  v_uint16)
DEF_CVT_FUNC(16u16s, cvt_, ushort, ಗಿಡ್ಡ,  v_int32)
DEF_CVT_FUNC(16u32s, cvt_, ushort, ಪೂರ್ಣಾಂಕ,    v_int32)
DEF_CVT_FUNC(16u32f, cvt_, ushort, ದಶಮಾಂಶ,  v_float32)
DEF_CVT_FUNC(16u64f, cvt_, ushort, ದ್ವಿಗುಣ, v_int32)
DEF_CVT_SCALAR_FUNC(16u64s, ushort, int64_t)
DEF_CVT_FUNC(16u16f, cvt1_,ushort, hfloat, v_float32)
DEF_CVT_FUNC(16u16bf, cvt1_, ushort, bfloat, v_float32)

////////////////////// 16s -> ... ////////////////////////

DEF_CVT_FUNC(16s8u,  cvt_, ಗಿಡ್ಡ, uchar,  v_int16)
DEF_CVT_FUNC(16s8s,  cvt_, ಗಿಡ್ಡ, schar,  v_int16)
DEF_CVT_FUNC(16s16u, cvt_, ಗಿಡ್ಡ, ushort, v_int32)
DEF_CVT_FUNC(16s32u, cvt_, ಗಿಡ್ಡ, ಚಿಹ್ನೆರಹಿತ, v_uint32)
DEF_CVT_FUNC(16s32s, cvt_, ಗಿಡ್ಡ, ಪೂರ್ಣಾಂಕ,    v_int32)
DEF_CVT_FUNC(16s32f, cvt_, ಗಿಡ್ಡ, ದಶಮಾಂಶ,  v_float32)
DEF_CVT_FUNC(16s64f, cvt_, ಗಿಡ್ಡ, ದ್ವಿಗುಣ, v_int32)
DEF_CVT_FUNC(16s64u, cvt_, ಗಿಡ್ಡ, uint64_t, v_uint32)
DEF_CVT_FUNC(16s64s, cvt_, ಗಿಡ್ಡ, int64_t, v_int32)
DEF_CVT_FUNC(16s16f, cvt1_,ಗಿಡ್ಡ, hfloat, v_float32)
DEF_CVT_FUNC(16s16bf, cvt1_, ಗಿಡ್ಡ, bfloat, v_float32)
DEF_CVT2BOOL_FUNC(16s8b, ಗಿಡ್ಡ, 0)

////////////////////// 32u -> ... ////////////////////////

DEF_CVT_FUNC(32u8u,  cvt_, ಚಿಹ್ನೆರಹಿತ, uchar,  v_uint32)
DEF_CVT_FUNC(32u8s,  cvt_, ಚಿಹ್ನೆರಹಿತ, schar,  v_int32)
DEF_CVT_FUNC(32u16u, cvt_, ಚಿಹ್ನೆರಹಿತ, ushort, v_uint32)
DEF_CVT_FUNC(32u16s, cvt_, ಚಿಹ್ನೆರಹಿತ, ಗಿಡ್ಡ,  v_int32)
DEF_CVT_SCALAR_FUNC(32u32s, ಚಿಹ್ನೆರಹಿತ, ಪೂರ್ಣಾಂಕ)
DEF_CVT_FUNC(32u32f, cvt_, ಚಿಹ್ನೆರಹಿತ, ದಶಮಾಂಶ,  v_float32)
DEF_CVT_FUNC(32u64f, cvt_64f, ಚಿಹ್ನೆರಹಿತ, ದ್ವಿಗುಣ, v_float64)
DEF_CVT_SCALAR_FUNC(32u64s, ಚಿಹ್ನೆರಹಿತ, int64_t)
DEF_CVT_SCALAR_FUNC(64u64s, uint64_t, int64_t)
DEF_CVT_FUNC(32u16f, cvt1_, ಚಿಹ್ನೆರಹಿತ, hfloat, v_float32)
DEF_CVT_FUNC(32u16bf, cvt1_, ಚಿಹ್ನೆರಹಿತ, bfloat, v_float32)

////////////////////// 32s -> ... ////////////////////////

DEF_CVT_FUNC(32s8u,  cvt_, ಪೂರ್ಣಾಂಕ, uchar,  v_int32)
DEF_CVT_FUNC(32s8s,  cvt_, ಪೂರ್ಣಾಂಕ, schar,  v_int32)
DEF_CVT_FUNC(32s16u, cvt_, ಪೂರ್ಣಾಂಕ, ushort, v_int32)
DEF_CVT_FUNC(32s16s, cvt_, ಪೂರ್ಣಾಂಕ, ಗಿಡ್ಡ,  v_int32)
DEF_CVT_FUNC(32s32u, cvt_, ಪೂರ್ಣಾಂಕ, ಚಿಹ್ನೆರಹಿತ, v_uint32)
DEF_CVT_FUNC(32s32f, cvt_, ಪೂರ್ಣಾಂಕ, ದಶಮಾಂಶ,  v_float32)
DEF_CVT_FUNC(32s64f, cvt_, ಪೂರ್ಣಾಂಕ, ದ್ವಿಗುಣ, v_int32)
DEF_CVT_FUNC(32s64u, cvt_, ಪೂರ್ಣಾಂಕ, uint64_t, v_uint32)
DEF_CVT_FUNC(32s64s, cvt_, ಪೂರ್ಣಾಂಕ, int64_t, v_int32)
DEF_CVT_FUNC(32s16f, cvt1_,ಪೂರ್ಣಾಂಕ, hfloat, v_float32)
DEF_CVT_FUNC(32s16bf, cvt1_, ಪೂರ್ಣಾಂಕ, bfloat, v_float32)
DEF_CVT2BOOL_FUNC(32s8b, ಪೂರ್ಣಾಂಕ, 0)

////////////////////// 32f -> ... ////////////////////////

DEF_CVT_FUNC(32f8u,  cvt_, ದಶಮಾಂಶ, uchar,  v_float32)
DEF_CVT_FUNC(32f8s,  cvt_, ದಶಮಾಂಶ, schar,  v_float32)
DEF_CVT_FUNC(32f16u, cvt_, ದಶಮಾಂಶ, ushort, v_float32)
DEF_CVT_FUNC(32f16s, cvt_, ದಶಮಾಂಶ, ಗಿಡ್ಡ,  v_float32)
DEF_CVT_FUNC(32f32u, cvt_, ದಶಮಾಂಶ, ಚಿಹ್ನೆರಹಿತ, v_float32)
DEF_CVT_FUNC(32f32s, cvt_, ದಶಮಾಂಶ, ಪೂರ್ಣಾಂಕ,    v_float32)
DEF_CVT_FUNC(32f64f, cvt_, ದಶಮಾಂಶ, ದ್ವಿಗುಣ, v_float32)
DEF_CVT_FUNC(32f64u, cvt_64f, ದಶಮಾಂಶ, uint64_t, v_float64)
DEF_CVT_FUNC(32f64s, cvt_64f, ದಶಮಾಂಶ, int64_t, v_float64)
DEF_CVT_FUNC(32f16f, cvt1_,ದಶಮಾಂಶ, hfloat, v_float32)
DEF_CVT_FUNC(32f16bf, cvt1_,ದಶಮಾಂಶ, bfloat, v_float32)
DEF_CVT2BOOL_FUNC(32f8b, ಪೂರ್ಣಾಂಕ, 1)

////////////////////// 64f -> ... ////////////////////////

DEF_CVT_FUNC(64f8u,  cvt_, ದ್ವಿಗುಣ, uchar,  v_int32)
DEF_CVT_FUNC(64f8s,  cvt_, ದ್ವಿಗುಣ, schar,  v_int32)
DEF_CVT_FUNC(64f16u, cvt_, ದ್ವಿಗುಣ, ushort, v_int32)
DEF_CVT_FUNC(64f16s, cvt_, ದ್ವಿಗುಣ, ಗಿಡ್ಡ,  v_int32)
DEF_CVT_FUNC(64f32u, cvt_64f, ದ್ವಿಗುಣ, ಚಿಹ್ನೆರಹಿತ, v_float32)
DEF_CVT_FUNC(64f32s, cvt_, ದ್ವಿಗುಣ, ಪೂರ್ಣಾಂಕ,    v_int32)
DEF_CVT_FUNC(64f32f, cvt_, ದ್ವಿಗುಣ, ದಶಮಾಂಶ,  v_float32)
DEF_CVT_FUNC(64f64u, cvt_64f, ದ್ವಿಗುಣ, uint64_t, v_float64)
DEF_CVT_FUNC(64f64s, cvt_64f, ದ್ವಿಗುಣ, int64_t, v_float64)
DEF_CVT_FUNC(64f16f, cvt1_,ದ್ವಿಗುಣ, hfloat, v_float32)
DEF_CVT_FUNC(64f16bf, cvt1_,ದ್ವಿಗುಣ, bfloat, v_float32)
DEF_CVT2BOOL_FUNC(64f8b, int64_t, 1)

////////////////////// 16f -> ... ////////////////////////

DEF_CVT_FUNC(16f8u,  cvt_,  hfloat, uchar,  v_float32)
DEF_CVT_FUNC(16f8s,  cvt_,  hfloat, schar,  v_float32)
DEF_CVT_FUNC(16f16u, cvt1_, hfloat, ushort, v_float32)
DEF_CVT_FUNC(16f16s, cvt1_, hfloat, ಗಿಡ್ಡ,  v_float32)
DEF_CVT_FUNC(16f32u, cvt1_, hfloat, ಚಿಹ್ನೆರಹಿತ, v_float32)
DEF_CVT_FUNC(16f32s, cvt1_, hfloat, ಪೂರ್ಣಾಂಕ,    v_float32)
DEF_CVT_FUNC(16f32f, cvt1_, hfloat, ದಶಮಾಂಶ,  v_float32)
DEF_CVT_FUNC(16f64f, cvt1_, hfloat, ದ್ವಿಗುಣ, v_float32)
DEF_CVT_FUNC(16f64u, cvt1_, hfloat, uint64_t, v_float32)
DEF_CVT_FUNC(16f64s, cvt1_, hfloat, int64_t, v_float32)
DEF_CVT_FUNC(16f16bf, cvt1_, hfloat, bfloat, v_float32)
DEF_CVT2BOOL_FUNC(16f8b, ಗಿಡ್ಡ, 1)

////////////////////// 16bf -> ... ////////////////////////

DEF_CVT_FUNC(16bf8u,  cvt_,  bfloat, uchar,  v_float32)
DEF_CVT_FUNC(16bf8s,  cvt_,  bfloat, schar,  v_float32)
DEF_CVT_FUNC(16bf16u, cvt1_, bfloat, ushort, v_float32)
DEF_CVT_FUNC(16bf16s, cvt1_, bfloat, ಗಿಡ್ಡ,  v_float32)
DEF_CVT_FUNC(16bf32u, cvt1_, bfloat, ಚಿಹ್ನೆರಹಿತ, v_float32)
DEF_CVT_FUNC(16bf32s, cvt1_, bfloat, ಪೂರ್ಣಾಂಕ,    v_float32)
DEF_CVT_FUNC(16bf32f, cvt1_, bfloat, ದಶಮಾಂಶ,  v_float32)
DEF_CVT_FUNC(16bf64f, cvt1_, bfloat, ದ್ವಿಗುಣ, v_float32)
DEF_CVT_FUNC(16bf64u, cvt1_, bfloat, uint64_t, v_float32)
DEF_CVT_FUNC(16bf64s, cvt1_, bfloat, int64_t, v_float32)
DEF_CVT_FUNC(16bf16f, cvt1_, bfloat, hfloat, v_float32)

////////////////////// 64s -> ... ////////////////////////

DEF_CVT_FUNC(64s8u,  cvt_, int64_t, uchar,  v_int32)
DEF_CVT_FUNC(64s8s,  cvt_, int64_t, schar,  v_int32)
DEF_CVT_FUNC(64s16u, cvt_, int64_t, ushort, v_int32)
DEF_CVT_FUNC(64s16s, cvt_, int64_t, ಗಿಡ್ಡ,  v_int32)
DEF_CVT_FUNC(64s32u, cvt_, int64_t, ಚಿಹ್ನೆರಹಿತ, v_uint32)
DEF_CVT_FUNC(64s32s, cvt_, int64_t, ಪೂರ್ಣಾಂಕ,    v_int32)
DEF_CVT_FUNC(64s32f, cvt_64f, int64_t, ದಶಮಾಂಶ,  v_float32)
DEF_CVT_FUNC(64s64f, cvt_64f, int64_t, ದ್ವಿಗುಣ,  v_float64)
DEF_CVT_FUNC(64s64u, cvt_, int64_t, uint64_t, v_uint64)
DEF_CVT_FUNC(64s16f, cvt1_,int64_t, hfloat, v_float32)
DEF_CVT_FUNC(64s16bf, cvt1_, int64_t, bfloat, v_float32)
DEF_CVT2BOOL_FUNC(64s8b, int64_t, 0)

////////////////////// 64u -> ... ////////////////////////

DEF_CVT_FUNC(64u8u,  cvt_, uint64_t, uchar,  v_int32)
DEF_CVT_FUNC(64u8s,  cvt_, uint64_t, schar,  v_int32)
DEF_CVT_FUNC(64u16u, cvt_, uint64_t, ushort, v_int32)
DEF_CVT_FUNC(64u16s, cvt_, uint64_t, ಗಿಡ್ಡ,  v_int32)
DEF_CVT_FUNC(64u32u, cvt_, uint64_t, ಚಿಹ್ನೆರಹಿತ, v_uint32)
DEF_CVT_FUNC(64u32s, cvt_, uint64_t, ಪೂರ್ಣಾಂಕ,   v_int32)
DEF_CVT_FUNC(64u32f, cvt_64f, uint64_t, ದಶಮಾಂಶ,  v_float64)
DEF_CVT_FUNC(64u64f, cvt_64f, uint64_t, ದ್ವಿಗುಣ,  v_float64)
DEF_CVT_FUNC(64u16f, cvt1_,uint64_t, hfloat, v_float32)
DEF_CVT_FUNC(64u16bf, cvt1_, uint64_t, bfloat, v_float32)

///////////// "conversion" w/o conversion ///////////////

ಸ್ಥಿರ ಶೂನ್ಯ cvt8u(ಸ್ಥಿರಾಂಕ uchar* src, size_t sstep, ಸ್ಥಿರಾಂಕ uchar*, size_t, uchar* dst, size_t dstep, ಗಾತ್ರ_2 size, ಶೂನ್ಯ*)
{ CV_INSTRUMENT_REGION(); cvtCopy(src, sstep, dst, dstep, size, 1); }

ಸ್ಥಿರ ಶೂನ್ಯ cvt16u(ಸ್ಥಿರಾಂಕ uchar* src, size_t sstep, ಸ್ಥಿರಾಂಕ uchar*, size_t, uchar* dst, size_t dstep, ಗಾತ್ರ_2 size, ಶೂನ್ಯ*)
{ CV_INSTRUMENT_REGION(); cvtCopy((ಸ್ಥಿರಾಂಕ uchar*)src, sstep, (uchar*)dst, dstep, size, 2); }

ಸ್ಥಿರ ಶೂನ್ಯ cvt32s(ಸ್ಥಿರಾಂಕ uchar* src, size_t sstep, ಸ್ಥಿರಾಂಕ uchar*, size_t, uchar* dst, size_t dstep, ಗಾತ್ರ_2 size, ಶೂನ್ಯ*)
{ CV_INSTRUMENT_REGION(); cvtCopy((ಸ್ಥಿರಾಂಕ uchar*)src, sstep, (uchar*)dst, dstep, size, 4); }

ಸ್ಥಿರ ಶೂನ್ಯ cvt64s(ಸ್ಥಿರಾಂಕ uchar* src, size_t sstep, ಸ್ಥಿರಾಂಕ uchar*, size_t, uchar* dst, size_t dstep, ಗಾತ್ರ_2 size, ಶೂನ್ಯ*)
{ CV_INSTRUMENT_REGION(); cvtCopy((ಸ್ಥಿರಾಂಕ uchar*)src, sstep, (uchar*)dst, dstep, size, 8); }

//////////////////// FP8 (1-byte float) conversions — scalar via saturate_cast ////////////////////
// Suffixes: 8fe4m3 (E4M3FN), 8fe4m3u (E4M3FNUZ).
#ವ್ಯಾಖ್ಯಾನಿಸು DEF_CVT_FP8(S, T) \
    DEF_CVT_SCALAR_FUNC(S##8u,   T, uchar)     DEF_CVT_SCALAR_FUNC(8u##S,   uchar, T) \
    DEF_CVT_SCALAR_FUNC(S##8s,   T, schar)     DEF_CVT_SCALAR_FUNC(8s##S,   schar, T) \
    DEF_CVT_SCALAR_FUNC(S##16u,  T, ushort)    DEF_CVT_SCALAR_FUNC(16u##S,  ushort, T) \
    DEF_CVT_SCALAR_FUNC(S##16s,  T, ಗಿಡ್ಡ)     DEF_CVT_SCALAR_FUNC(16s##S,  ಗಿಡ್ಡ, T) \
    DEF_CVT_SCALAR_FUNC(S##32u,  T, ಚಿಹ್ನೆರಹಿತ)  DEF_CVT_SCALAR_FUNC(32u##S,  ಚಿಹ್ನೆರಹಿತ, T) \
    DEF_CVT_SCALAR_FUNC(S##32s,  T, ಪೂರ್ಣಾಂಕ)       DEF_CVT_SCALAR_FUNC(32s##S,  ಪೂರ್ಣಾಂಕ, T) \
    DEF_CVT_SCALAR_FUNC(S##64f,  T, ದ್ವಿಗುಣ)    DEF_CVT_SCALAR_FUNC(64f##S,  ದ್ವಿಗುಣ, T) \
    DEF_CVT_SCALAR_FUNC(S##16f,  T, hfloat)    DEF_CVT_SCALAR_FUNC(16f##S,  hfloat, T) \
    DEF_CVT_SCALAR_FUNC(S##16bf, T, bfloat)    DEF_CVT_SCALAR_FUNC(16bf##S, bfloat, T) \
    DEF_CVT_SCALAR_FUNC(S##64u,  T, uint64_t)  DEF_CVT_SCALAR_FUNC(64u##S,  uint64_t, T) \
    DEF_CVT_SCALAR_FUNC(S##64s,  T, int64_t)   DEF_CVT_SCALAR_FUNC(64s##S,  int64_t, T) \
    DEF_CVT_SCALAR_FUNC(S##8b,   T, ತಾರ್ಕಿಕ)

DEF_CVT_FP8(8fe4m3,  fp8_t)
DEF_CVT_FP8(8fe4m3u, fp8a_t)

// FP8 -> FP8, cross-format only (identity uses the 1-byte copy cvt8u, like 16f->16f uses cvt16u)
DEF_CVT_SCALAR_FUNC(8fe4m38fe4m3u, fp8_t,   fp8a_t)
DEF_CVT_SCALAR_FUNC(8fe4m3u8fe4m3, fp8a_t, fp8_t)

// float32 -> FP8 encode via the shared building blocks in convert.hpp.
ಮಾದರಿ<ಪೂರ್ಣಾಂಕ bias, ತಾರ್ಕಿಕ fnuz> ಸ್ಥಿರ ಶೂನ್ಯ
cvt_F32_ಗಮ್ಯ_Fp8(ಸ್ಥಿರಾಂಕ uchar* src_, size_t sstep, ಸ್ಥಿರಾಂಕ uchar*, size_t,
            uchar* dst, size_t dstep, ಗಾತ್ರ_2 size, ಶೂನ್ಯ*)
{
    CV_INSTRUMENT_REGION();
    ಸ್ಥಿರಾಂಕ ದಶಮಾಂಶ* src = (ಸ್ಥಿರಾಂಕ ದಶಮಾಂಶ*)src_;
    sstep /= ಗಾತ್ರ(src[0]);

    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < size.ಎತ್ತರ; i++, src += sstep, dst += dstep)
    {
        ಪೂರ್ಣಾಂಕ j = 0;
#ಆದರೆ (CV_SIMD || CV_SIMD_SCALABLE)
        ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ LANES32 = VTraits<v_int32>::vlanes();
        ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ VECSZ = LANES32 * 4;
        ಗಾಗಿ (; j <= size.ಅಗಲ - VECSZ; j += VECSZ)
        {
            v_float32 vf0 = vx_ತುಂಬು(src + j);
            v_float32 vf1 = vx_ತುಂಬು(src + j + LANES32);
            v_float32 vf2 = vx_ತುಂಬು(src + j + 2 * LANES32);
            v_float32 vf3 = vx_ತುಂಬು(src + j + 3 * LANES32);
            encodeFp8Vec4<bias, fnuz>(vf0, vf1, vf2, vf3, src + j, VECSZ, dst + j);
        }
        vx_cleanup();
#ಆದರೆ_ಅಂತ್ಯ
        ಗಾಗಿ (; j < size.ಅಗಲ; j++)
            dst[j] = fp8_detail::encodeE4M3(src[j], bias, fnuz);
    }
}
ಸ್ಥಿರ ಶೂನ್ಯ cvt32f8fe4m3(ಸ್ಥಿರಾಂಕ uchar* s, size_t ss, ಸ್ಥಿರಾಂಕ uchar* p, size_t ps, uchar* d, size_t ds, ಗಾತ್ರ_2 sz, ಶೂನ್ಯ* x)
{ cvt_F32_ಗಮ್ಯ_Fp8<7, ಸುಳ್ಳು>(s, ss, p, ps, d, ds, sz, x); }
ಸ್ಥಿರ ಶೂನ್ಯ cvt32f8fe4m3u(ಸ್ಥಿರಾಂಕ uchar* s, size_t ss, ಸ್ಥಿರಾಂಕ uchar* p, size_t ps, uchar* d, size_t ds, ಗಾತ್ರ_2 sz, ಶೂನ್ಯ* x)
{ cvt_F32_ಗಮ್ಯ_Fp8<8, ನಿಜ>(s, ss, p, ps, d, ds, sz, x); }

// FP8 -> float32: 256-entry decode table gathered via universal intrinsics (same table as scalar)
ಮಾದರಿ<ಬಗೆ_ಹೆಸರು FP8>
ಸ್ಥಿರ ಶೂನ್ಯ cvt_Fp8_ಗಮ್ಯ_F32(ಸ್ಥಿರಾಂಕ uchar* src, size_t sstep, ಸ್ಥಿರಾಂಕ uchar*, size_t,
                        uchar* dst_, size_t dstep, ಗಾತ್ರ_2 size, ಶೂನ್ಯ*)
{
    CV_INSTRUMENT_REGION();
    ದಶಮಾಂಶ* dst = (ದಶಮಾಂಶ*)dst_;
    ಸ್ಥಿರಾಂಕ ದಶಮಾಂಶ* tab = FP8::decodeLUT();
    dstep /= ಗಾತ್ರ(dst[0]);
    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < size.ಎತ್ತರ; i++, src += sstep, dst += dstep)
    {
        ಪೂರ್ಣಾಂಕ j = 0;
#ಆದರೆ (CV_SIMD || CV_SIMD_SCALABLE)
        ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ VECSZ = VTraits<v_float32>::vlanes();
        ಗಾಗಿ (; j <= size.ಅಗಲ - VECSZ; j += VECSZ)
            v_store(dst + j, v_lut(tab, v_reinterpret_as_s32(vx_ತುಂಬು_expand_q(src + j))));
#ಆದರೆ_ಅಂತ್ಯ
        ಗಾಗಿ (; j < size.ಅಗಲ; j++) dst[j] = tab[src[j]];
    }
}
ಸ್ಥಿರ ಶೂನ್ಯ cvt8fe4m332f(ಸ್ಥಿರಾಂಕ uchar* s, size_t ss, ಸ್ಥಿರಾಂಕ uchar* p, size_t ps, uchar* d, size_t ds, ಗಾತ್ರ_2 sz, ಶೂನ್ಯ* x)
{ cvt_Fp8_ಗಮ್ಯ_F32<fp8_t>(s, ss, p, ps, d, ds, sz, x); }
ಸ್ಥಿರ ಶೂನ್ಯ cvt8fe4m3u32f(ಸ್ಥಿರಾಂಕ uchar* s, size_t ss, ಸ್ಥಿರಾಂಕ uchar* p, size_t ps, uchar* d, size_t ds, ಗಾತ್ರ_2 sz, ಶೂನ್ಯ* x)
{ cvt_Fp8_ಗಮ್ಯ_F32<fp8a_t>(s, ss, p, ps, d, ds, sz, x); }

ದ್ವಿಮಾನ_Func getConvertFunc(ಪೂರ್ಣಾಂಕ sdepth_, ಪೂರ್ಣಾಂಕ ddepth_)
{
    ಪೂರ್ಣಾಂಕ sdepth = CV_MAT_ಆಳ(sdepth_);
    ಪೂರ್ಣಾಂಕ ddepth = CV_MAT_ಆಳ(ddepth_);
    ದ್ವಿಮಾನ_Func func =
        ddepth == CV_8U ? (
            sdepth == CV_8U ? cvt8u :
            sdepth == CV_8S ? cvt8s8u :
            sdepth == CV_16U ? cvt16u8u :
            sdepth == CV_16S ? cvt16s8u :
            sdepth == CV_32U ? cvt32u8u :
            sdepth == CV_32S ? cvt32s8u :
            sdepth == CV_32F ? cvt32f8u :
            sdepth == CV_64F ? cvt64f8u :
            sdepth == CV_16F ? cvt16f8u :
            sdepth == CV_16BF ? cvt16bf8u :
            sdepth == CV_Bool ? cvt8b8u :
            sdepth == CV_64U ? cvt64u8u :
            sdepth == CV_64S ? cvt64s8u :
            sdepth == CV_8F_E4M3FN ? cvt8fe4m38u :
            sdepth == CV_8F_E4M3FNUZ ? cvt8fe4m3u8u :
            0) :
        ddepth == CV_8S ? (
            sdepth == CV_8U ? cvt8u8s :
            sdepth == CV_8S ? cvt8u :
            sdepth == CV_16U ? cvt16u8s :
            sdepth == CV_16S ? cvt16s8s :
            sdepth == CV_32U ? cvt32u8s :
            sdepth == CV_32S ? cvt32s8s :
            sdepth == CV_32F ? cvt32f8s :
            sdepth == CV_64F ? cvt64f8s :
            sdepth == CV_16F ? cvt16f8s :
            sdepth == CV_16BF ? cvt16bf8s :
            sdepth == CV_Bool ? cvt8b8u :
            sdepth == CV_64U ? cvt64u8s :
            sdepth == CV_64S ? cvt64s8s :
            sdepth == CV_8F_E4M3FN ? cvt8fe4m38s :
            sdepth == CV_8F_E4M3FNUZ ? cvt8fe4m3u8s :
            0) :
        ddepth == CV_16U ? (
            sdepth == CV_8U ? cvt8u16s : // same as cvt8u16u
            sdepth == CV_8S ? cvt8s16u :
            sdepth == CV_16U ? cvt16u :
            sdepth == CV_16S ? cvt16s16u :
            sdepth == CV_32U ? cvt32u16u :
            sdepth == CV_32S ? cvt32s16u :
            sdepth == CV_32F ? cvt32f16u :
            sdepth == CV_64F ? cvt64f16u :
            sdepth == CV_16F ? cvt16f16u :
            sdepth == CV_16BF ? cvt16bf16u :
            sdepth == CV_Bool ? cvt8b16s :
            sdepth == CV_64U ? cvt64u16u :
            sdepth == CV_64S ? cvt64s16u :
            sdepth == CV_8F_E4M3FN ? cvt8fe4m316u :
            sdepth == CV_8F_E4M3FNUZ ? cvt8fe4m3u16u :
            0) :
        ddepth == CV_16S ? (
            sdepth == CV_8U ? cvt8u16s :
            sdepth == CV_8S ? cvt8s16s :
            sdepth == CV_16U ? cvt16u16s :
            sdepth == CV_16S ? cvt16u :
            sdepth == CV_32U ? cvt32u16s :
            sdepth == CV_32S ? cvt32s16s :
            sdepth == CV_32F ? cvt32f16s :
            sdepth == CV_64F ? cvt64f16s :
            sdepth == CV_16F ? cvt16f16s :
            sdepth == CV_16BF ? cvt16bf16s :
            sdepth == CV_Bool ? cvt8b16s :
            sdepth == CV_64U ? cvt64u16s :
            sdepth == CV_64S ? cvt64s16s :
            sdepth == CV_8F_E4M3FN ? cvt8fe4m316s :
            sdepth == CV_8F_E4M3FNUZ ? cvt8fe4m3u16s :
            0) :
        ddepth == CV_32U ? (
            sdepth == CV_8U ? cvt8u32s : // same as cvt8u32u
            sdepth == CV_8S ? cvt8s32u :
            sdepth == CV_16U ? cvt16u32s : // same as cvt16u32u
            sdepth == CV_16S ? cvt16s32u :
            sdepth == CV_32U ? cvt32s :
            sdepth == CV_32S ? cvt32s32u :
            sdepth == CV_32F ? cvt32f32u :
            sdepth == CV_64F ? cvt64f32u :
            sdepth == CV_16F ? cvt16f32u :
            sdepth == CV_16BF ? cvt16bf32u :
            sdepth == CV_Bool ? cvt8b32s :
            sdepth == CV_64U ? cvt64u32u :
            sdepth == CV_64S ? cvt64s32u :
            sdepth == CV_8F_E4M3FN ? cvt8fe4m332u :
            sdepth == CV_8F_E4M3FNUZ ? cvt8fe4m3u32u :
            0) :
        ddepth == CV_32S ? (
            sdepth == CV_8U ? cvt8u32s :
            sdepth == CV_8S ? cvt8s32s :
            sdepth == CV_16U ? cvt16u32s :
            sdepth == CV_16S ? cvt16s32s :
            sdepth == CV_32U ? cvt32u32s :
            sdepth == CV_32S ? cvt32s :
            sdepth == CV_32F ? cvt32f32s :
            sdepth == CV_64F ? cvt64f32s :
            sdepth == CV_16F ? cvt16f32s :
            sdepth == CV_16BF ? cvt16bf32s :
            sdepth == CV_Bool ? cvt8b32s :
            sdepth == CV_64U ? cvt64u32s :
            sdepth == CV_64S ? cvt64s32s :
            sdepth == CV_8F_E4M3FN ? cvt8fe4m332s :
            sdepth == CV_8F_E4M3FNUZ ? cvt8fe4m3u32s :
            0) :
        ddepth == CV_32F ? (
            sdepth == CV_8U ? cvt8u32f :
            sdepth == CV_8S ? cvt8s32f :
            sdepth == CV_16U ? cvt16u32f :
            sdepth == CV_16S ? cvt16s32f :
            sdepth == CV_32U ? cvt32u32f :
            sdepth == CV_32S ? cvt32s32f :
            sdepth == CV_32F ? cvt32s :
            sdepth == CV_64F ? cvt64f32f :
            sdepth == CV_16F ? cvt16f32f :
            sdepth == CV_16BF ? cvt16bf32f :
            sdepth == CV_Bool ? cvt8b32f :
            sdepth == CV_64U ? cvt64u32f :
            sdepth == CV_64S ? cvt64s32f :
            sdepth == CV_8F_E4M3FN ? cvt8fe4m332f :
            sdepth == CV_8F_E4M3FNUZ ? cvt8fe4m3u32f :
            0) :
        ddepth == CV_64F ? (
            sdepth == CV_8U ? cvt8u64f :
            sdepth == CV_8S ? cvt8s64f :
            sdepth == CV_16U ? cvt16u64f :
            sdepth == CV_16S ? cvt16s64f :
            sdepth == CV_32U ? cvt32u64f :
            sdepth == CV_32S ? cvt32s64f :
            sdepth == CV_32F ? cvt32f64f :
            sdepth == CV_64F ? cvt64s :
            sdepth == CV_16F ? cvt16f64f :
            sdepth == CV_16BF ? cvt16bf64f :
            sdepth == CV_Bool ? cvt8b64f :
            sdepth == CV_64U ? cvt64u64f :
            sdepth == CV_64S ? cvt64s64f :
            sdepth == CV_8F_E4M3FN ? cvt8fe4m364f :
            sdepth == CV_8F_E4M3FNUZ ? cvt8fe4m3u64f :
            0) :
        ddepth == CV_16F ? (
            sdepth == CV_8U ? cvt8u16f :
            sdepth == CV_8S ? cvt8s16f :
            sdepth == CV_16U ? cvt16u16f :
            sdepth == CV_16S ? cvt16s16f :
            sdepth == CV_32U ? cvt32u16f :
            sdepth == CV_32S ? cvt32s16f :
            sdepth == CV_32F ? cvt32f16f :
            sdepth == CV_64F ? cvt64f16f :
            sdepth == CV_16F ? cvt16u :
            sdepth == CV_16BF ? cvt16bf16f :
            sdepth == CV_Bool ? cvt8b16f :
            sdepth == CV_64U ? cvt64u16f :
            sdepth == CV_64S ? cvt64s16f :
            sdepth == CV_8F_E4M3FN ? cvt8fe4m316f :
            sdepth == CV_8F_E4M3FNUZ ? cvt8fe4m3u16f :
            0) :
        ddepth == CV_16BF ? (
            sdepth == CV_8U ? cvt8u16bf :
            sdepth == CV_8S ? cvt8s16bf :
            sdepth == CV_16U ? cvt16u16bf :
            sdepth == CV_16S ? cvt16s16bf :
            sdepth == CV_32U ? cvt32u16bf :
            sdepth == CV_32S ? cvt32s16bf :
            sdepth == CV_32F ? cvt32f16bf :
            sdepth == CV_64F ? cvt64f16bf :
            sdepth == CV_16F ? cvt16f16bf :
            sdepth == CV_16BF ? cvt16u :
            sdepth == CV_Bool ? cvt8b16bf :
            sdepth == CV_64U ? cvt64u16bf :
            sdepth == CV_64S ? cvt64s16bf :
            sdepth == CV_8F_E4M3FN ? cvt8fe4m316bf :
            sdepth == CV_8F_E4M3FNUZ ? cvt8fe4m3u16bf :
            0) :
        ddepth == CV_Bool ? (
            sdepth == CV_8U ? cvt8u8b :
            sdepth == CV_8S ? cvt8u8b :
            sdepth == CV_16U ? cvt16s8b :
            sdepth == CV_16S ? cvt16s8b :
            sdepth == CV_32U ? cvt32s8b :
            sdepth == CV_32S ? cvt32s8b :
            sdepth == CV_32F ? cvt32f8b :
            sdepth == CV_64F ? cvt64f8b :
            sdepth == CV_16F ? cvt16f8b :
            sdepth == CV_16BF ? cvt16f8b : // same as cvt16f8b
            sdepth == CV_Bool ? cvt8u :
            sdepth == CV_64U ? cvt64s8b :
            sdepth == CV_64S ? cvt64s8b :
            sdepth == CV_8F_E4M3FN ? cvt8fe4m38b :
            sdepth == CV_8F_E4M3FNUZ ? cvt8fe4m3u8b :
            0) :
        ddepth == CV_64U ? (
            sdepth == CV_8U ? cvt8u64s : // same as cvt8u64u
            sdepth == CV_8S ? cvt8s64u :
            sdepth == CV_16U ? cvt16u64s : // same as cvt16u64u
            sdepth == CV_16S ? cvt16s64u :
            sdepth == CV_32U ? cvt32u64s : // same as cvt32u64u
            sdepth == CV_32S ? cvt32s64u :
            sdepth == CV_32F ? cvt32f64u :
            sdepth == CV_64F ? cvt64f64u :
            sdepth == CV_16F ? cvt16f64u :
            sdepth == CV_16BF ? cvt16bf64u :
            sdepth == CV_Bool ? cvt8b64s :
            sdepth == CV_64U ? cvt64s :
            sdepth == CV_64S ? cvt64s64u :
            sdepth == CV_8F_E4M3FN ? cvt8fe4m364u :
            sdepth == CV_8F_E4M3FNUZ ? cvt8fe4m3u64u :
            0) :
        ddepth == CV_64S ? (
            sdepth == CV_8U ? cvt8u64s :
            sdepth == CV_8S ? cvt8s64s :
            sdepth == CV_16U ? cvt16u64s :
            sdepth == CV_16S ? cvt16s64s :
            sdepth == CV_32U ? cvt32u64s :
            sdepth == CV_32S ? cvt32s64s :
            sdepth == CV_32F ? cvt32f64s :
            sdepth == CV_64F ? cvt64f64s :
            sdepth == CV_16F ? cvt16f64s :
            sdepth == CV_16BF ? cvt16bf64s :
            sdepth == CV_Bool ? cvt8b64s :
            sdepth == CV_64U ? cvt64u64s :
            sdepth == CV_64S ? cvt64s :
            sdepth == CV_8F_E4M3FN ? cvt8fe4m364s :
            sdepth == CV_8F_E4M3FNUZ ? cvt8fe4m3u64s :
            0) :
        ddepth == CV_8F_E4M3FN ? (
            sdepth == CV_8U ? cvt8u8fe4m3 :
            sdepth == CV_8S ? cvt8s8fe4m3 :
            sdepth == CV_16U ? cvt16u8fe4m3 :
            sdepth == CV_16S ? cvt16s8fe4m3 :
            sdepth == CV_32U ? cvt32u8fe4m3 :
            sdepth == CV_32S ? cvt32s8fe4m3 :
            sdepth == CV_32F ? cvt32f8fe4m3 :
            sdepth == CV_64F ? cvt64f8fe4m3 :
            sdepth == CV_16F ? cvt16f8fe4m3 :
            sdepth == CV_16BF ? cvt16bf8fe4m3 :
            sdepth == CV_Bool ? cvt8u8fe4m3 : // bool stored as 0/1 byte -> reuse uchar path
            sdepth == CV_64U ? cvt64u8fe4m3 :
            sdepth == CV_64S ? cvt64s8fe4m3 :
            sdepth == CV_8F_E4M3FN ? cvt8u : // identity: 1-byte copy
            sdepth == CV_8F_E4M3FNUZ ? cvt8fe4m3u8fe4m3 :
            0) :
        ddepth == CV_8F_E4M3FNUZ ? (
            sdepth == CV_8U ? cvt8u8fe4m3u :
            sdepth == CV_8S ? cvt8s8fe4m3u :
            sdepth == CV_16U ? cvt16u8fe4m3u :
            sdepth == CV_16S ? cvt16s8fe4m3u :
            sdepth == CV_32U ? cvt32u8fe4m3u :
            sdepth == CV_32S ? cvt32s8fe4m3u :
            sdepth == CV_32F ? cvt32f8fe4m3u :
            sdepth == CV_64F ? cvt64f8fe4m3u :
            sdepth == CV_16F ? cvt16f8fe4m3u :
            sdepth == CV_16BF ? cvt16bf8fe4m3u :
            sdepth == CV_Bool ? cvt8u8fe4m3u : // bool stored as 0/1 byte -> reuse uchar path
            sdepth == CV_64U ? cvt64u8fe4m3u :
            sdepth == CV_64S ? cvt64s8fe4m3u :
            sdepth == CV_8F_E4M3FN ? cvt8fe4m38fe4m3u :
            sdepth == CV_8F_E4M3FNUZ ? cvt8u : // identity: 1-byte copy
            0) :
        0;
    CV_Assert(func != 0);
    ಹಿಂತಿರುಗಿ func;
}

CV_CPU_OPTIMIZATION_NAMESPACE_ಕೊನೆ
#ಆದರೆ_ಅಂತ್ಯ
} // namespace
