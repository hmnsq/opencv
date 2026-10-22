// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html

#अन्तर्भाव "precomp.hpp"
#अन्तर्भाव "convert.hpp"

#यदि !defined(OPENCV_SUPRESS_WARNING_AVX2_WITHOUT_FP16C) && \
    (defined(__GNUC__) && defined(__AVX2__) && !defined(__F16C__))
#warning "Non-optimal compiler flags: AVX2 without FP16. Generated code is very slow. Consider adding '-mf16c' compiler option."
#यदि_अन्तः

नामाकाश cv {
नामाकाश hal {
CV_CPU_OPTIMIZATION_NAMESPACE_BEGIN

शून्य cvt16f32f(ध्रुव hfloat* src, भिन्न* dst, पूर्णाङ्क len);
शून्य cvt32f16f(ध्रुव भिन्न* src, hfloat* dst, पूर्णाङ्क len);
शून्य cvt16bf32f(ध्रुव bfloat* src, भिन्न* dst, पूर्णाङ्क len);
शून्य cvt32f16bf(ध्रुव भिन्न* src, bfloat* dst, पूर्णाङ्क len);
शून्य सङ्कलय_RNGBias32f(भिन्न* arr, ध्रुव भिन्न* scale_Bias_युग्मानि, पूर्णाङ्क len, पूर्णाङ्क cn);
शून्य सङ्कलय_RNGBias64f(द्विगुण* arr, ध्रुव द्विगुण* scale_Bias_युग्मानि, पूर्णाङ्क len, पूर्णाङ्क cn);

CV_CPU_OPTIMIZATION_NAMESPACE_अन्तः
} // namespace cv::hal

CV_CPU_OPTIMIZATION_NAMESPACE_BEGIN

द्विमान_Func getConvertFunc(पूर्णाङ्क sdepth, पूर्णाङ्क ddepth);

CV_CPU_OPTIMIZATION_NAMESPACE_अन्तः

#यदि_अपरिभाषितम् CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

नामाकाश hal {
CV_CPU_OPTIMIZATION_NAMESPACE_BEGIN

द्विमान_Func getConvertFunc(पूर्णाङ्क sdepth, पूर्णाङ्क ddepth);

शून्य cvt16f32f( ध्रुव hfloat* src, भिन्न* dst, पूर्णाङ्क len )
{
    CV_INSTRUMENT_REGION();
    पूर्णाङ्क j = 0;
#यदि (CV_SIMD || CV_SIMD_SCALABLE)
    ध्रुव पूर्णाङ्क VECSZ = VTraits<v_float32>::vlanes();
    प्रति( ; j < len; j += VECSZ )
    {
        यदि( j > len - VECSZ )
        {
            यदि( j == 0 )
                भङ्ग;
            j = len - VECSZ;
        }
        v_store(dst + j, vx_आरोपय_expand(src + j));
    }
#यदि_अन्तः
    प्रति( ; j < len; j++ )
        dst[j] = (भिन्न)src[j];
}

शून्य cvt32f16f( ध्रुव भिन्न* src, hfloat* dst, पूर्णाङ्क len )
{
    CV_INSTRUMENT_REGION();
    पूर्णाङ्क j = 0;
#यदि (CV_SIMD || CV_SIMD_SCALABLE)
    ध्रुव पूर्णाङ्क VECSZ = VTraits<v_float32>::vlanes();
    प्रति( ; j < len; j += VECSZ )
    {
        यदि( j > len - VECSZ )
        {
            यदि( j == 0 )
                भङ्ग;
            j = len - VECSZ;
        }
        v_pack_store(dst + j, vx_आरोपय(src + j));
    }
#यदि_अन्तः
    प्रति( ; j < len; j++ )
        dst[j] = hfloat(src[j]);
}

शून्य cvt32f16bf( ध्रुव भिन्न* src, bfloat* dst, पूर्णाङ्क len )
{
    CV_INSTRUMENT_REGION();
    पूर्णाङ्क j = 0;
#यदि (CV_SIMD || CV_SIMD_SCALABLE)
    ध्रुव पूर्णाङ्क VECSZ = VTraits<v_float32>::vlanes();
    प्रति( ; j < len; j += VECSZ )
    {
        यदि( j > len - VECSZ )
        {
            यदि( j == 0 )
                भङ्ग;
            j = len - VECSZ;
        }
        v_pack_store(dst + j, vx_आरोपय(src + j));
    }
#यदि_अन्तः
    प्रति( ; j < len; j++ )
        dst[j] = bfloat(src[j]);
}

शून्य सङ्कलय_RNGBias32f( भिन्न* arr, ध्रुव भिन्न* scale_Bias_युग्मानि, पूर्णाङ्क len, पूर्णाङ्क cn )
{
    CV_INSTRUMENT_REGION();
    यदि (cn == 1) {
        भिन्न bias = scale_Bias_युग्मानि[1];
        प्रति( पूर्णाङ्क i = 0; i < len; i++ ) {
            arr[i] += bias;
        }
    } अन्यथा {
        पूर्णाङ्क k = 0;
        len *= cn;
        cn--;
        प्रति( पूर्णाङ्क i = 0; i < len; i++ ) {
            arr[i] += scale_Bias_युग्मानि[k*2 + 1];
            k = (k + 1) & ((k >= cn) - 1);
        }
    }
}

शून्य सङ्कलय_RNGBias64f( द्विगुण* arr, ध्रुव द्विगुण* scale_Bias_युग्मानि, पूर्णाङ्क len, पूर्णाङ्क cn )
{
    CV_INSTRUMENT_REGION();
    यदि (cn == 1) {
        द्विगुण bias = scale_Bias_युग्मानि[1];
        प्रति( पूर्णाङ्क i = 0; i < len; i++ ) {
            arr[i] += bias;
        }
    } अन्यथा {
        पूर्णाङ्क k = 0;
        len *= cn;
        cn--;
        प्रति( पूर्णाङ्क i = 0; i < len; i++ ) {
            arr[i] += scale_Bias_युग्मानि[k*2 + 1];
            k = (k + 1) & ((k >= cn) - 1);
        }
    }
}

CV_CPU_OPTIMIZATION_NAMESPACE_अन्तः
} // namespace cv::hal

// cv::
CV_CPU_OPTIMIZATION_NAMESPACE_BEGIN

प्रतिरूपम्<प्रकारनाम _Ts, प्रकारनाम _Td, प्रकारनाम _Twvec> स्थिर अन्तःस्थ शून्य
cvt_( ध्रुव _Ts* src, size_t sstep, _Td* dst, size_t dstep, आकार size )
{
    sstep /= परिमाण(src[0]);
    dstep /= परिमाण(dst[0]);

    प्रति( पूर्णाङ्क i = 0; i < size.height; i++, src += sstep, dst += dstep )
    {
        पूर्णाङ्क j = 0;
// Excluding GNU in CV_SIMD_SCALABLE because of "opencv/issues/26936"
#यदि (CV_SIMD || (CV_SIMD_SCALABLE && !(defined(__GNUC__) && !defined(__clang__))) )
        ध्रुव पूर्णाङ्क VECSZ = VTraits<_Twvec>::vlanes()*2;
        प्रति( ; j < size.विस्तारः; j += VECSZ )
        {
            यदि( j > size.विस्तारः - VECSZ )
            {
                यदि( j == 0 || src == (_Ts*)dst )
                    भङ्ग;
                j = size.विस्तारः - VECSZ;
            }
            _Twvec v0, v1;
            vx_आरोपय_pair_as(src + j, v0, v1);
            v_store_pair_as(dst + j, v0, v1);
        }
#यदि_अन्तः
        प्रति( ; j < size.विस्तारः; j++ )
            dst[j] = saturate_cast<_Td>(src[j]);
    }
}

प्रतिरूपम्<प्रकारनाम _Ts, प्रकारनाम _Td, प्रकारनाम dummy> स्थिर अन्तःस्थ शून्य
cvt_64f( ध्रुव _Ts* src, size_t sstep, _Td* dst, size_t dstep, आकार size )
{
    sstep /= परिमाण(src[0]);
    dstep /= परिमाण(dst[0]);

    प्रति( पूर्णाङ्क i = 0; i < size.height; i++, src += sstep, dst += dstep )
    {
        पूर्णाङ्क j = 0;
#यदि (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)
        ध्रुव पूर्णाङ्क VECSZ = VTraits<v_float64>::vlanes()*2;
        प्रति( ; j < size.विस्तारः; j += VECSZ )
        {
            यदि( j > size.विस्तारः - VECSZ )
            {
                यदि( j == 0 || src == (_Ts*)dst )
                    भङ्ग;
                j = size.विस्तारः - VECSZ;
            }
            v_float64 v0, v1;
            vx_आरोपय_pair_as(src + j, v0, v1);
            v_store_pair_as(dst + j, v0, v1);
        }
#यदि_अन्तः
        प्रति( ; j < size.विस्तारः; j++ )
            dst[j] = saturate_cast<_Td>(src[j]);
    }
}

// in order to reduce the code size, for (16f <-> ...) conversions
// we add a conversion function without loop unrolling
प्रतिरूपम्<प्रकारनाम _Ts, प्रकारनाम _Td, प्रकारनाम _Twvec> स्थिर अन्तःस्थ शून्य
cvt1_( ध्रुव _Ts* src, size_t sstep, _Td* dst, size_t dstep, आकार size )
{
    sstep /= परिमाण(src[0]);
    dstep /= परिमाण(dst[0]);

    प्रति( पूर्णाङ्क i = 0; i < size.height; i++, src += sstep, dst += dstep )
    {
        पूर्णाङ्क j = 0;
#यदि (CV_SIMD || CV_SIMD_SCALABLE)
        ध्रुव पूर्णाङ्क VECSZ = VTraits<_Twvec>::vlanes();
        प्रति( ; j < size.विस्तारः; j += VECSZ )
        {
            यदि( j > size.विस्तारः - VECSZ )
            {
                यदि( j == 0 || src == (_Ts*)dst )
                    भङ्ग;
                j = size.विस्तारः - VECSZ;
            }
            _Twvec v;
            vx_आरोपय_as(src + j, v);
            v_store_as(dst + j, v);
        }
        vx_cleanup();
#यदि_अन्तः
        प्रति( ; j < size.विस्तारः; j++ )
            dst[j] = saturate_cast<_Td>(src[j]);
    }
}

स्थिर शून्य cvtCopy( ध्रुव uchar* src, size_t sstep,
                     uchar* dst, size_t dstep, आकार size, size_t elemsize)
{
    size_t len = size.विस्तारः*elemsize;
    प्रति( पूर्णाङ्क i = 0; i < size.height; i++, src += sstep, dst += dstep )
    {
        स्मृतिं_प्रतिलिख( dst, src, len );
    }
}

#परिभाषय DEF_CVT_FUNC(suffix, cvtfunc, _Ts, _Td, _Twvec) \
स्थिर शून्य cvt##suffix(ध्रुव uchar* src_, size_t sstep, ध्रुव uchar*, size_t, \
                        uchar* dst_, size_t dstep, आकार size, शून्य*) \
{ \
    CV_INSTRUMENT_REGION(); \
    ध्रुव _Ts* src = (ध्रुव _Ts*)src_; \
    _Td* dst = (_Td*)dst_; \
    cvtfunc<_Ts, _Td, _Twvec>(src, sstep, dst, dstep, size); \
}

#परिभाषय DEF_CVT2BOOL_FUNC(suffix, _Ts, सरणम्) \
स्थिर शून्य cvt##suffix(ध्रुव uchar* src_, size_t sstep, ध्रुव uchar*, size_t, \
                        uchar* dst, size_t dstep, आकार size, शून्य*) \
{ \
    CV_INSTRUMENT_REGION(); \
    ध्रुव _Ts* src = (ध्रुव _Ts*)src_; \
    sstep /= परिमाण(src[0]); \
    \
    प्रति( पूर्णाङ्क i = 0; i < size.height; i++, src += sstep, dst += dstep ) { \
        प्रति ( पूर्णाङ्क j = 0; j < size.विस्तारः; j++ ) \
            dst[j] = (src[j]<<सरणम्) != 0; \
    } \
}

#परिभाषय DEF_CVTBOOL2_FUNC(suffix, _Td, scale) \
स्थिर शून्य cvt##suffix(ध्रुव uchar* src, size_t sstep, ध्रुव uchar*, size_t, \
                        uchar* dst_, size_t dstep, आकार size, शून्य*) \
{ \
    CV_INSTRUMENT_REGION(); \
    _Td* dst = (_Td*)dst_; \
    dstep /= परिमाण(dst[0]); \
    \
    प्रति( पूर्णाङ्क i = 0; i < size.height; i++, src += sstep, dst += dstep ) { \
        प्रति ( पूर्णाङ्क j = 0; j < size.विस्तारः; j++ ) \
            dst[j] = (_Td)((src[j] != 0)*scale); \
    } \
}

#परिभाषय DEF_CVT_SCALAR_FUNC(suffix, _Ts, _Td) \
स्थिर शून्य cvt##suffix(ध्रुव uchar* src_, size_t sstep, ध्रुव uchar*, size_t, \
                        uchar* dst_, size_t dstep, आकार size, शून्य*) \
{ \
    CV_INSTRUMENT_REGION(); \
    ध्रुव _Ts* src = (ध्रुव _Ts*)src_; \
    _Td* dst = (_Td*)dst_; \
    sstep /= परिमाण(src[0]); \
    dstep /= परिमाण(dst[0]); \
    \
    प्रति( पूर्णाङ्क i = 0; i < size.height; i++, src += sstep, dst += dstep ) { \
        प्रति ( पूर्णाङ्क j = 0; j < size.विस्तारः; j++ ) \
            dst[j] = saturate_cast<_Td>(src[j]); \
    } \
}

#परिभाषय DEF_CVT_SCALAR_FUNC_S2U(suffix, _Ts, _Td, _Tw) \
स्थिर शून्य cvt##suffix(ध्रुव uchar* src_, size_t sstep, ध्रुव uchar*, size_t, \
                        uchar* dst_, size_t dstep, आकार size, शून्य*) \
{ \
    CV_INSTRUMENT_REGION(); \
    ध्रुव _Ts* src = (ध्रुव _Ts*)src_; \
    _Td* dst = (_Td*)dst_; \
    sstep /= परिमाण(src[0]); \
    dstep /= परिमाण(dst[0]); \
    \
    प्रति( पूर्णाङ्क i = 0; i < size.height; i++, src += sstep, dst += dstep ) { \
        प्रति ( पूर्णाङ्क j = 0; j < size.विस्तारः; j++ ) \
            dst[j] = saturate_cast<_Td>(std::max((_Tw)src[j], (_Tw)0)); \
    } \
}

////////////////////// 8u -> ... ////////////////////////

DEF_CVT_FUNC(8u8s,  cvt_,  uchar, schar,    v_int16)
DEF_CVT_FUNC(8u16s, cvt_,  uchar, ह्रस्व,    v_int16)
DEF_CVT_FUNC(8u32s, cvt_,  uchar, पूर्णाङ्क,      v_int32)
DEF_CVT_FUNC(8u32f, cvt_,  uchar, भिन्न,    v_float32)
DEF_CVT_FUNC(8u64f, cvt_,  uchar, द्विगुण,   v_int32)
DEF_CVT_SCALAR_FUNC(8u64s, uchar, int64_t)
DEF_CVT_FUNC(8u16f, cvt1_, uchar, hfloat, v_float32)
DEF_CVT_FUNC(8u16bf, cvt1_, uchar, bfloat, v_float32)
DEF_CVT2BOOL_FUNC(8u8b, uchar, 0)

////////////////////// 8s -> ... ////////////////////////

DEF_CVT_FUNC(8s8u,  cvt_,  schar, uchar,    v_int16)
DEF_CVT_FUNC(8s16u, cvt_,  schar, ushort,   v_uint16)
DEF_CVT_FUNC(8s16s, cvt_,  schar, ह्रस्व,    v_int16)
DEF_CVT_FUNC(8s32u, cvt_,  schar, अचिह्नित, v_uint32)
DEF_CVT_FUNC(8s32s, cvt_,  schar, पूर्णाङ्क,      v_int32)
DEF_CVT_FUNC(8s32f, cvt_,  schar, भिन्न,    v_float32)
DEF_CVT_FUNC(8s64f, cvt_,  schar, द्विगुण,   v_int32)
DEF_CVT_FUNC(8s64u, cvt_,  schar, uint64_t, v_uint32)
DEF_CVT_FUNC(8s64s, cvt_,  schar, int64_t,  v_int32)
DEF_CVT_FUNC(8s16f, cvt1_, schar, hfloat, v_float32)
DEF_CVT_FUNC(8s16bf, cvt1_, schar, bfloat, v_float32)

////////////////////// 8b -> ... ////////////////////////

DEF_CVTBOOL2_FUNC(8b8u,  uchar, 1)
DEF_CVTBOOL2_FUNC(8b16s, ह्रस्व, 1)
DEF_CVTBOOL2_FUNC(8b32s, पूर्णाङ्क, 1)
DEF_CVTBOOL2_FUNC(8b32f, भिन्न, 1)
DEF_CVTBOOL2_FUNC(8b64f, द्विगुण, 1)
DEF_CVTBOOL2_FUNC(8b64s, int64_t, 1)
DEF_CVTBOOL2_FUNC(8b16f, uint16_t, 0x3c00) // hfloat(1.0f)
DEF_CVTBOOL2_FUNC(8b16bf, uint16_t, 0x3f80) // bfloat(1.0f)

////////////////////// 16u -> ... ////////////////////////

DEF_CVT_FUNC(16u8u,  cvt_, ushort, uchar,  v_uint16)
DEF_CVT_FUNC(16u8s,  cvt_, ushort, schar,  v_uint16)
DEF_CVT_FUNC(16u16s, cvt_, ushort, ह्रस्व,  v_int32)
DEF_CVT_FUNC(16u32s, cvt_, ushort, पूर्णाङ्क,    v_int32)
DEF_CVT_FUNC(16u32f, cvt_, ushort, भिन्न,  v_float32)
DEF_CVT_FUNC(16u64f, cvt_, ushort, द्विगुण, v_int32)
DEF_CVT_SCALAR_FUNC(16u64s, ushort, int64_t)
DEF_CVT_FUNC(16u16f, cvt1_,ushort, hfloat, v_float32)
DEF_CVT_FUNC(16u16bf, cvt1_, ushort, bfloat, v_float32)

////////////////////// 16s -> ... ////////////////////////

DEF_CVT_FUNC(16s8u,  cvt_, ह्रस्व, uchar,  v_int16)
DEF_CVT_FUNC(16s8s,  cvt_, ह्रस्व, schar,  v_int16)
DEF_CVT_FUNC(16s16u, cvt_, ह्रस्व, ushort, v_int32)
DEF_CVT_FUNC(16s32u, cvt_, ह्रस्व, अचिह्नित, v_uint32)
DEF_CVT_FUNC(16s32s, cvt_, ह्रस्व, पूर्णाङ्क,    v_int32)
DEF_CVT_FUNC(16s32f, cvt_, ह्रस्व, भिन्न,  v_float32)
DEF_CVT_FUNC(16s64f, cvt_, ह्रस्व, द्विगुण, v_int32)
DEF_CVT_FUNC(16s64u, cvt_, ह्रस्व, uint64_t, v_uint32)
DEF_CVT_FUNC(16s64s, cvt_, ह्रस्व, int64_t, v_int32)
DEF_CVT_FUNC(16s16f, cvt1_,ह्रस्व, hfloat, v_float32)
DEF_CVT_FUNC(16s16bf, cvt1_, ह्रस्व, bfloat, v_float32)
DEF_CVT2BOOL_FUNC(16s8b, ह्रस्व, 0)

////////////////////// 32u -> ... ////////////////////////

DEF_CVT_FUNC(32u8u,  cvt_, अचिह्नित, uchar,  v_uint32)
DEF_CVT_FUNC(32u8s,  cvt_, अचिह्नित, schar,  v_int32)
DEF_CVT_FUNC(32u16u, cvt_, अचिह्नित, ushort, v_uint32)
DEF_CVT_FUNC(32u16s, cvt_, अचिह्नित, ह्रस्व,  v_int32)
DEF_CVT_SCALAR_FUNC(32u32s, अचिह्नित, पूर्णाङ्क)
DEF_CVT_FUNC(32u32f, cvt_, अचिह्नित, भिन्न,  v_float32)
DEF_CVT_FUNC(32u64f, cvt_64f, अचिह्नित, द्विगुण, v_float64)
DEF_CVT_SCALAR_FUNC(32u64s, अचिह्नित, int64_t)
DEF_CVT_SCALAR_FUNC(64u64s, uint64_t, int64_t)
DEF_CVT_FUNC(32u16f, cvt1_, अचिह्नित, hfloat, v_float32)
DEF_CVT_FUNC(32u16bf, cvt1_, अचिह्नित, bfloat, v_float32)

////////////////////// 32s -> ... ////////////////////////

DEF_CVT_FUNC(32s8u,  cvt_, पूर्णाङ्क, uchar,  v_int32)
DEF_CVT_FUNC(32s8s,  cvt_, पूर्णाङ्क, schar,  v_int32)
DEF_CVT_FUNC(32s16u, cvt_, पूर्णाङ्क, ushort, v_int32)
DEF_CVT_FUNC(32s16s, cvt_, पूर्णाङ्क, ह्रस्व,  v_int32)
DEF_CVT_FUNC(32s32u, cvt_, पूर्णाङ्क, अचिह्नित, v_uint32)
DEF_CVT_FUNC(32s32f, cvt_, पूर्णाङ्क, भिन्न,  v_float32)
DEF_CVT_FUNC(32s64f, cvt_, पूर्णाङ्क, द्विगुण, v_int32)
DEF_CVT_FUNC(32s64u, cvt_, पूर्णाङ्क, uint64_t, v_uint32)
DEF_CVT_FUNC(32s64s, cvt_, पूर्णाङ्क, int64_t, v_int32)
DEF_CVT_FUNC(32s16f, cvt1_,पूर्णाङ्क, hfloat, v_float32)
DEF_CVT_FUNC(32s16bf, cvt1_, पूर्णाङ्क, bfloat, v_float32)
DEF_CVT2BOOL_FUNC(32s8b, पूर्णाङ्क, 0)

////////////////////// 32f -> ... ////////////////////////

DEF_CVT_FUNC(32f8u,  cvt_, भिन्न, uchar,  v_float32)
DEF_CVT_FUNC(32f8s,  cvt_, भिन्न, schar,  v_float32)
DEF_CVT_FUNC(32f16u, cvt_, भिन्न, ushort, v_float32)
DEF_CVT_FUNC(32f16s, cvt_, भिन्न, ह्रस्व,  v_float32)
DEF_CVT_FUNC(32f32u, cvt_, भिन्न, अचिह्नित, v_float32)
DEF_CVT_FUNC(32f32s, cvt_, भिन्न, पूर्णाङ्क,    v_float32)
DEF_CVT_FUNC(32f64f, cvt_, भिन्न, द्विगुण, v_float32)
DEF_CVT_FUNC(32f64u, cvt_64f, भिन्न, uint64_t, v_float64)
DEF_CVT_FUNC(32f64s, cvt_64f, भिन्न, int64_t, v_float64)
DEF_CVT_FUNC(32f16f, cvt1_,भिन्न, hfloat, v_float32)
DEF_CVT_FUNC(32f16bf, cvt1_,भिन्न, bfloat, v_float32)
DEF_CVT2BOOL_FUNC(32f8b, पूर्णाङ्क, 1)

////////////////////// 64f -> ... ////////////////////////

DEF_CVT_FUNC(64f8u,  cvt_, द्विगुण, uchar,  v_int32)
DEF_CVT_FUNC(64f8s,  cvt_, द्विगुण, schar,  v_int32)
DEF_CVT_FUNC(64f16u, cvt_, द्विगुण, ushort, v_int32)
DEF_CVT_FUNC(64f16s, cvt_, द्विगुण, ह्रस्व,  v_int32)
DEF_CVT_FUNC(64f32u, cvt_64f, द्विगुण, अचिह्नित, v_float32)
DEF_CVT_FUNC(64f32s, cvt_, द्विगुण, पूर्णाङ्क,    v_int32)
DEF_CVT_FUNC(64f32f, cvt_, द्विगुण, भिन्न,  v_float32)
DEF_CVT_FUNC(64f64u, cvt_64f, द्विगुण, uint64_t, v_float64)
DEF_CVT_FUNC(64f64s, cvt_64f, द्विगुण, int64_t, v_float64)
DEF_CVT_FUNC(64f16f, cvt1_,द्विगुण, hfloat, v_float32)
DEF_CVT_FUNC(64f16bf, cvt1_,द्विगुण, bfloat, v_float32)
DEF_CVT2BOOL_FUNC(64f8b, int64_t, 1)

////////////////////// 16f -> ... ////////////////////////

DEF_CVT_FUNC(16f8u,  cvt_,  hfloat, uchar,  v_float32)
DEF_CVT_FUNC(16f8s,  cvt_,  hfloat, schar,  v_float32)
DEF_CVT_FUNC(16f16u, cvt1_, hfloat, ushort, v_float32)
DEF_CVT_FUNC(16f16s, cvt1_, hfloat, ह्रस्व,  v_float32)
DEF_CVT_FUNC(16f32u, cvt1_, hfloat, अचिह्नित, v_float32)
DEF_CVT_FUNC(16f32s, cvt1_, hfloat, पूर्णाङ्क,    v_float32)
DEF_CVT_FUNC(16f32f, cvt1_, hfloat, भिन्न,  v_float32)
DEF_CVT_FUNC(16f64f, cvt1_, hfloat, द्विगुण, v_float32)
DEF_CVT_FUNC(16f64u, cvt1_, hfloat, uint64_t, v_float32)
DEF_CVT_FUNC(16f64s, cvt1_, hfloat, int64_t, v_float32)
DEF_CVT_FUNC(16f16bf, cvt1_, hfloat, bfloat, v_float32)
DEF_CVT2BOOL_FUNC(16f8b, ह्रस्व, 1)

////////////////////// 16bf -> ... ////////////////////////

DEF_CVT_FUNC(16bf8u,  cvt_,  bfloat, uchar,  v_float32)
DEF_CVT_FUNC(16bf8s,  cvt_,  bfloat, schar,  v_float32)
DEF_CVT_FUNC(16bf16u, cvt1_, bfloat, ushort, v_float32)
DEF_CVT_FUNC(16bf16s, cvt1_, bfloat, ह्रस्व,  v_float32)
DEF_CVT_FUNC(16bf32u, cvt1_, bfloat, अचिह्नित, v_float32)
DEF_CVT_FUNC(16bf32s, cvt1_, bfloat, पूर्णाङ्क,    v_float32)
DEF_CVT_FUNC(16bf32f, cvt1_, bfloat, भिन्न,  v_float32)
DEF_CVT_FUNC(16bf64f, cvt1_, bfloat, द्विगुण, v_float32)
DEF_CVT_FUNC(16bf64u, cvt1_, bfloat, uint64_t, v_float32)
DEF_CVT_FUNC(16bf64s, cvt1_, bfloat, int64_t, v_float32)
DEF_CVT_FUNC(16bf16f, cvt1_, bfloat, hfloat, v_float32)

////////////////////// 64s -> ... ////////////////////////

DEF_CVT_FUNC(64s8u,  cvt_, int64_t, uchar,  v_int32)
DEF_CVT_FUNC(64s8s,  cvt_, int64_t, schar,  v_int32)
DEF_CVT_FUNC(64s16u, cvt_, int64_t, ushort, v_int32)
DEF_CVT_FUNC(64s16s, cvt_, int64_t, ह्रस्व,  v_int32)
DEF_CVT_FUNC(64s32u, cvt_, int64_t, अचिह्नित, v_uint32)
DEF_CVT_FUNC(64s32s, cvt_, int64_t, पूर्णाङ्क,    v_int32)
DEF_CVT_FUNC(64s32f, cvt_64f, int64_t, भिन्न,  v_float32)
DEF_CVT_FUNC(64s64f, cvt_64f, int64_t, द्विगुण,  v_float64)
DEF_CVT_FUNC(64s64u, cvt_, int64_t, uint64_t, v_uint64)
DEF_CVT_FUNC(64s16f, cvt1_,int64_t, hfloat, v_float32)
DEF_CVT_FUNC(64s16bf, cvt1_, int64_t, bfloat, v_float32)
DEF_CVT2BOOL_FUNC(64s8b, int64_t, 0)

////////////////////// 64u -> ... ////////////////////////

DEF_CVT_FUNC(64u8u,  cvt_, uint64_t, uchar,  v_int32)
DEF_CVT_FUNC(64u8s,  cvt_, uint64_t, schar,  v_int32)
DEF_CVT_FUNC(64u16u, cvt_, uint64_t, ushort, v_int32)
DEF_CVT_FUNC(64u16s, cvt_, uint64_t, ह्रस्व,  v_int32)
DEF_CVT_FUNC(64u32u, cvt_, uint64_t, अचिह्नित, v_uint32)
DEF_CVT_FUNC(64u32s, cvt_, uint64_t, पूर्णाङ्क,   v_int32)
DEF_CVT_FUNC(64u32f, cvt_64f, uint64_t, भिन्न,  v_float64)
DEF_CVT_FUNC(64u64f, cvt_64f, uint64_t, द्विगुण,  v_float64)
DEF_CVT_FUNC(64u16f, cvt1_,uint64_t, hfloat, v_float32)
DEF_CVT_FUNC(64u16bf, cvt1_, uint64_t, bfloat, v_float32)

///////////// "conversion" w/o conversion ///////////////

स्थिर शून्य cvt8u(ध्रुव uchar* src, size_t sstep, ध्रुव uchar*, size_t, uchar* dst, size_t dstep, आकार size, शून्य*)
{ CV_INSTRUMENT_REGION(); cvtCopy(src, sstep, dst, dstep, size, 1); }

स्थिर शून्य cvt16u(ध्रुव uchar* src, size_t sstep, ध्रुव uchar*, size_t, uchar* dst, size_t dstep, आकार size, शून्य*)
{ CV_INSTRUMENT_REGION(); cvtCopy((ध्रुव uchar*)src, sstep, (uchar*)dst, dstep, size, 2); }

स्थिर शून्य cvt32s(ध्रुव uchar* src, size_t sstep, ध्रुव uchar*, size_t, uchar* dst, size_t dstep, आकार size, शून्य*)
{ CV_INSTRUMENT_REGION(); cvtCopy((ध्रुव uchar*)src, sstep, (uchar*)dst, dstep, size, 4); }

स्थिर शून्य cvt64s(ध्रुव uchar* src, size_t sstep, ध्रुव uchar*, size_t, uchar* dst, size_t dstep, आकार size, शून्य*)
{ CV_INSTRUMENT_REGION(); cvtCopy((ध्रुव uchar*)src, sstep, (uchar*)dst, dstep, size, 8); }

//////////////////// FP8 (1-byte float) conversions — scalar via saturate_cast ////////////////////
// Suffixes: 8fe4m3 (E4M3FN), 8fe4m3u (E4M3FNUZ).
#परिभाषय DEF_CVT_FP8(S, T) \
    DEF_CVT_SCALAR_FUNC(S##8u,   T, uchar)     DEF_CVT_SCALAR_FUNC(8u##S,   uchar, T) \
    DEF_CVT_SCALAR_FUNC(S##8s,   T, schar)     DEF_CVT_SCALAR_FUNC(8s##S,   schar, T) \
    DEF_CVT_SCALAR_FUNC(S##16u,  T, ushort)    DEF_CVT_SCALAR_FUNC(16u##S,  ushort, T) \
    DEF_CVT_SCALAR_FUNC(S##16s,  T, ह्रस्व)     DEF_CVT_SCALAR_FUNC(16s##S,  ह्रस्व, T) \
    DEF_CVT_SCALAR_FUNC(S##32u,  T, अचिह्नित)  DEF_CVT_SCALAR_FUNC(32u##S,  अचिह्नित, T) \
    DEF_CVT_SCALAR_FUNC(S##32s,  T, पूर्णाङ्क)       DEF_CVT_SCALAR_FUNC(32s##S,  पूर्णाङ्क, T) \
    DEF_CVT_SCALAR_FUNC(S##64f,  T, द्विगुण)    DEF_CVT_SCALAR_FUNC(64f##S,  द्विगुण, T) \
    DEF_CVT_SCALAR_FUNC(S##16f,  T, hfloat)    DEF_CVT_SCALAR_FUNC(16f##S,  hfloat, T) \
    DEF_CVT_SCALAR_FUNC(S##16bf, T, bfloat)    DEF_CVT_SCALAR_FUNC(16bf##S, bfloat, T) \
    DEF_CVT_SCALAR_FUNC(S##64u,  T, uint64_t)  DEF_CVT_SCALAR_FUNC(64u##S,  uint64_t, T) \
    DEF_CVT_SCALAR_FUNC(S##64s,  T, int64_t)   DEF_CVT_SCALAR_FUNC(64s##S,  int64_t, T) \
    DEF_CVT_SCALAR_FUNC(S##8b,   T, तार्किक)

DEF_CVT_FP8(8fe4m3,  fp8_t)
DEF_CVT_FP8(8fe4m3u, fp8a_t)

// FP8 -> FP8, cross-format only (identity uses the 1-byte copy cvt8u, like 16f->16f uses cvt16u)
DEF_CVT_SCALAR_FUNC(8fe4m38fe4m3u, fp8_t,   fp8a_t)
DEF_CVT_SCALAR_FUNC(8fe4m3u8fe4m3, fp8a_t, fp8_t)

// float32 -> FP8 encode via the shared building blocks in convert.hpp.
प्रतिरूपम्<पूर्णाङ्क bias, तार्किक fnuz> स्थिर शून्य
cvt_F32_गन्तव्यम्_Fp8(ध्रुव uchar* src_, size_t sstep, ध्रुव uchar*, size_t,
            uchar* dst, size_t dstep, आकार size, शून्य*)
{
    CV_INSTRUMENT_REGION();
    ध्रुव भिन्न* src = (ध्रुव भिन्न*)src_;
    sstep /= परिमाण(src[0]);

    प्रति (पूर्णाङ्क i = 0; i < size.height; i++, src += sstep, dst += dstep)
    {
        पूर्णाङ्क j = 0;
#यदि (CV_SIMD || CV_SIMD_SCALABLE)
        ध्रुव पूर्णाङ्क LANES32 = VTraits<v_int32>::vlanes();
        ध्रुव पूर्णाङ्क VECSZ = LANES32 * 4;
        प्रति (; j <= size.विस्तारः - VECSZ; j += VECSZ)
        {
            v_float32 vf0 = vx_आरोपय(src + j);
            v_float32 vf1 = vx_आरोपय(src + j + LANES32);
            v_float32 vf2 = vx_आरोपय(src + j + 2 * LANES32);
            v_float32 vf3 = vx_आरोपय(src + j + 3 * LANES32);
            encodeFp8Vec4<bias, fnuz>(vf0, vf1, vf2, vf3, src + j, VECSZ, dst + j);
        }
        vx_cleanup();
#यदि_अन्तः
        प्रति (; j < size.विस्तारः; j++)
            dst[j] = fp8_detail::encodeE4M3(src[j], bias, fnuz);
    }
}
स्थिर शून्य cvt32f8fe4m3(ध्रुव uchar* s, size_t ss, ध्रुव uchar* p, size_t ps, uchar* d, size_t ds, आकार sz, शून्य* x)
{ cvt_F32_गन्तव्यम्_Fp8<7, असत्य>(s, ss, p, ps, d, ds, sz, x); }
स्थिर शून्य cvt32f8fe4m3u(ध्रुव uchar* s, size_t ss, ध्रुव uchar* p, size_t ps, uchar* d, size_t ds, आकार sz, शून्य* x)
{ cvt_F32_गन्तव्यम्_Fp8<8, सत्य>(s, ss, p, ps, d, ds, sz, x); }

// FP8 -> float32: 256-entry decode table gathered via universal intrinsics (same table as scalar)
प्रतिरूपम्<प्रकारनाम FP8>
स्थिर शून्य cvt_Fp8_गन्तव्यम्_F32(ध्रुव uchar* src, size_t sstep, ध्रुव uchar*, size_t,
                        uchar* dst_, size_t dstep, आकार size, शून्य*)
{
    CV_INSTRUMENT_REGION();
    भिन्न* dst = (भिन्न*)dst_;
    ध्रुव भिन्न* tab = FP8::decodeLUT();
    dstep /= परिमाण(dst[0]);
    प्रति (पूर्णाङ्क i = 0; i < size.height; i++, src += sstep, dst += dstep)
    {
        पूर्णाङ्क j = 0;
#यदि (CV_SIMD || CV_SIMD_SCALABLE)
        ध्रुव पूर्णाङ्क VECSZ = VTraits<v_float32>::vlanes();
        प्रति (; j <= size.विस्तारः - VECSZ; j += VECSZ)
            v_store(dst + j, v_lut(tab, v_reinterpret_as_s32(vx_आरोपय_expand_q(src + j))));
#यदि_अन्तः
        प्रति (; j < size.विस्तारः; j++) dst[j] = tab[src[j]];
    }
}
स्थिर शून्य cvt8fe4m332f(ध्रुव uchar* s, size_t ss, ध्रुव uchar* p, size_t ps, uchar* d, size_t ds, आकार sz, शून्य* x)
{ cvt_Fp8_गन्तव्यम्_F32<fp8_t>(s, ss, p, ps, d, ds, sz, x); }
स्थिर शून्य cvt8fe4m3u32f(ध्रुव uchar* s, size_t ss, ध्रुव uchar* p, size_t ps, uchar* d, size_t ds, आकार sz, शून्य* x)
{ cvt_Fp8_गन्तव्यम्_F32<fp8a_t>(s, ss, p, ps, d, ds, sz, x); }

द्विमान_Func getConvertFunc(पूर्णाङ्क sdepth_, पूर्णाङ्क ddepth_)
{
    पूर्णाङ्क sdepth = CV_MAT_गभीरता(sdepth_);
    पूर्णाङ्क ddepth = CV_MAT_गभीरता(ddepth_);
    द्विमान_Func func =
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
    प्रत्यागम func;
}

CV_CPU_OPTIMIZATION_NAMESPACE_अन्तः
#यदि_अन्तः
} // namespace
