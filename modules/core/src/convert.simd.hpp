// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html

#شامل "precomp.hpp"
#شامل "convert.hpp"

#اگر !defined(OPENCV_SUPRESS_WARNING_AVX2_WITHOUT_FP16C) && \
    (defined(__GNUC__) && defined(__AVX2__) && !defined(__F16C__))
#warning "Non-optimal compiler flags: AVX2 without FP16. Generated code is very slow. Consider adding '-mf16c' compiler option."
#اگر_ختم

نامخلا cv {
نامخلا hal {
CV_CPU_OPTIMIZATION_NAMESPACE_BEGIN

خالی cvt16f32f(ثابت hfloat* src, اعشاری* dst, صحیح len);
خالی cvt32f16f(ثابت اعشاری* src, hfloat* dst, صحیح len);
خالی cvt16bf32f(ثابت bfloat* src, اعشاری* dst, صحیح len);
خالی cvt32f16bf(ثابت اعشاری* src, bfloat* dst, صحیح len);
خالی جوڑو_RNGBias32f(اعشاری* arr, ثابت اعشاری* scale_Bias_جوڑے, صحیح len, صحیح cn);
خالی جوڑو_RNGBias64f(دوہرا* arr, ثابت دوہرا* scale_Bias_جوڑے, صحیح len, صحیح cn);

CV_CPU_OPTIMIZATION_NAMESPACE_انتہا
} // namespace cv::hal

CV_CPU_OPTIMIZATION_NAMESPACE_BEGIN

ثنائی_Func getConvertFunc(صحیح sdepth, صحیح ddepth);

CV_CPU_OPTIMIZATION_NAMESPACE_انتہا

#اگر_غیر_تعریف_شدہ CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

نامخلا hal {
CV_CPU_OPTIMIZATION_NAMESPACE_BEGIN

ثنائی_Func getConvertFunc(صحیح sdepth, صحیح ddepth);

خالی cvt16f32f( ثابت hfloat* src, اعشاری* dst, صحیح len )
{
    CV_INSTRUMENT_REGION();
    صحیح j = 0;
#اگر (CV_SIMD || CV_SIMD_SCALABLE)
    ثابت صحیح VECSZ = VTraits<v_float32>::vlanes();
    برائے( ; j < len; j += VECSZ )
    {
        اگر( j > len - VECSZ )
        {
            اگر( j == 0 )
                توڑو;
            j = len - VECSZ;
        }
        v_store(dst + j, vx_بھرو_expand(src + j));
    }
#اگر_ختم
    برائے( ; j < len; j++ )
        dst[j] = (اعشاری)src[j];
}

خالی cvt32f16f( ثابت اعشاری* src, hfloat* dst, صحیح len )
{
    CV_INSTRUMENT_REGION();
    صحیح j = 0;
#اگر (CV_SIMD || CV_SIMD_SCALABLE)
    ثابت صحیح VECSZ = VTraits<v_float32>::vlanes();
    برائے( ; j < len; j += VECSZ )
    {
        اگر( j > len - VECSZ )
        {
            اگر( j == 0 )
                توڑو;
            j = len - VECSZ;
        }
        v_pack_store(dst + j, vx_بھرو(src + j));
    }
#اگر_ختم
    برائے( ; j < len; j++ )
        dst[j] = hfloat(src[j]);
}

خالی cvt32f16bf( ثابت اعشاری* src, bfloat* dst, صحیح len )
{
    CV_INSTRUMENT_REGION();
    صحیح j = 0;
#اگر (CV_SIMD || CV_SIMD_SCALABLE)
    ثابت صحیح VECSZ = VTraits<v_float32>::vlanes();
    برائے( ; j < len; j += VECSZ )
    {
        اگر( j > len - VECSZ )
        {
            اگر( j == 0 )
                توڑو;
            j = len - VECSZ;
        }
        v_pack_store(dst + j, vx_بھرو(src + j));
    }
#اگر_ختم
    برائے( ; j < len; j++ )
        dst[j] = bfloat(src[j]);
}

خالی جوڑو_RNGBias32f( اعشاری* arr, ثابت اعشاری* scale_Bias_جوڑے, صحیح len, صحیح cn )
{
    CV_INSTRUMENT_REGION();
    اگر (cn == 1) {
        اعشاری bias = scale_Bias_جوڑے[1];
        برائے( صحیح i = 0; i < len; i++ ) {
            arr[i] += bias;
        }
    } ورنہ {
        صحیح k = 0;
        len *= cn;
        cn--;
        برائے( صحیح i = 0; i < len; i++ ) {
            arr[i] += scale_Bias_جوڑے[k*2 + 1];
            k = (k + 1) & ((k >= cn) - 1);
        }
    }
}

خالی جوڑو_RNGBias64f( دوہرا* arr, ثابت دوہرا* scale_Bias_جوڑے, صحیح len, صحیح cn )
{
    CV_INSTRUMENT_REGION();
    اگر (cn == 1) {
        دوہرا bias = scale_Bias_جوڑے[1];
        برائے( صحیح i = 0; i < len; i++ ) {
            arr[i] += bias;
        }
    } ورنہ {
        صحیح k = 0;
        len *= cn;
        cn--;
        برائے( صحیح i = 0; i < len; i++ ) {
            arr[i] += scale_Bias_جوڑے[k*2 + 1];
            k = (k + 1) & ((k >= cn) - 1);
        }
    }
}

CV_CPU_OPTIMIZATION_NAMESPACE_انتہا
} // namespace cv::hal

// cv::
CV_CPU_OPTIMIZATION_NAMESPACE_BEGIN

سانچہ<قسم_نام _Ts, قسم_نام _Td, قسم_نام _Twvec> ساکن اندرونی خالی
cvt_( ثابت _Ts* src, size_t sstep, _Td* dst, size_t dstep, سائز size )
{
    sstep /= حجم(src[0]);
    dstep /= حجم(dst[0]);

    برائے( صحیح i = 0; i < size.اونچائی; i++, src += sstep, dst += dstep )
    {
        صحیح j = 0;
// Excluding GNU in CV_SIMD_SCALABLE because of "opencv/issues/26936"
#اگر (CV_SIMD || (CV_SIMD_SCALABLE && !(defined(__GNUC__) && !defined(__clang__))) )
        ثابت صحیح VECSZ = VTraits<_Twvec>::vlanes()*2;
        برائے( ; j < size.چوڑائی; j += VECSZ )
        {
            اگر( j > size.چوڑائی - VECSZ )
            {
                اگر( j == 0 || src == (_Ts*)dst )
                    توڑو;
                j = size.چوڑائی - VECSZ;
            }
            _Twvec v0, v1;
            vx_بھرو_pair_as(src + j, v0, v1);
            v_store_pair_as(dst + j, v0, v1);
        }
#اگر_ختم
        برائے( ; j < size.چوڑائی; j++ )
            dst[j] = saturate_cast<_Td>(src[j]);
    }
}

سانچہ<قسم_نام _Ts, قسم_نام _Td, قسم_نام dummy> ساکن اندرونی خالی
cvt_64f( ثابت _Ts* src, size_t sstep, _Td* dst, size_t dstep, سائز size )
{
    sstep /= حجم(src[0]);
    dstep /= حجم(dst[0]);

    برائے( صحیح i = 0; i < size.اونچائی; i++, src += sstep, dst += dstep )
    {
        صحیح j = 0;
#اگر (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)
        ثابت صحیح VECSZ = VTraits<v_float64>::vlanes()*2;
        برائے( ; j < size.چوڑائی; j += VECSZ )
        {
            اگر( j > size.چوڑائی - VECSZ )
            {
                اگر( j == 0 || src == (_Ts*)dst )
                    توڑو;
                j = size.چوڑائی - VECSZ;
            }
            v_float64 v0, v1;
            vx_بھرو_pair_as(src + j, v0, v1);
            v_store_pair_as(dst + j, v0, v1);
        }
#اگر_ختم
        برائے( ; j < size.چوڑائی; j++ )
            dst[j] = saturate_cast<_Td>(src[j]);
    }
}

// in order to reduce the code size, for (16f <-> ...) conversions
// we add a conversion function without loop unrolling
سانچہ<قسم_نام _Ts, قسم_نام _Td, قسم_نام _Twvec> ساکن اندرونی خالی
cvt1_( ثابت _Ts* src, size_t sstep, _Td* dst, size_t dstep, سائز size )
{
    sstep /= حجم(src[0]);
    dstep /= حجم(dst[0]);

    برائے( صحیح i = 0; i < size.اونچائی; i++, src += sstep, dst += dstep )
    {
        صحیح j = 0;
#اگر (CV_SIMD || CV_SIMD_SCALABLE)
        ثابت صحیح VECSZ = VTraits<_Twvec>::vlanes();
        برائے( ; j < size.چوڑائی; j += VECSZ )
        {
            اگر( j > size.چوڑائی - VECSZ )
            {
                اگر( j == 0 || src == (_Ts*)dst )
                    توڑو;
                j = size.چوڑائی - VECSZ;
            }
            _Twvec v;
            vx_بھرو_as(src + j, v);
            v_store_as(dst + j, v);
        }
        vx_cleanup();
#اگر_ختم
        برائے( ; j < size.چوڑائی; j++ )
            dst[j] = saturate_cast<_Td>(src[j]);
    }
}

ساکن خالی cvtCopy( ثابت uchar* src, size_t sstep,
                     uchar* dst, size_t dstep, سائز size, size_t elemsize)
{
    size_t len = size.چوڑائی*elemsize;
    برائے( صحیح i = 0; i < size.اونچائی; i++, src += sstep, dst += dstep )
    {
        حافظہ_نقل( dst, src, len );
    }
}

#تعریف_کرو DEF_CVT_FUNC(suffix, cvtfunc, _Ts, _Td, _Twvec) \
ساکن خالی cvt##suffix(ثابت uchar* src_, size_t sstep, ثابت uchar*, size_t, \
                        uchar* dst_, size_t dstep, سائز size, خالی*) \
{ \
    CV_INSTRUMENT_REGION(); \
    ثابت _Ts* src = (ثابت _Ts*)src_; \
    _Td* dst = (_Td*)dst_; \
    cvtfunc<_Ts, _Td, _Twvec>(src, sstep, dst, dstep, size); \
}

#تعریف_کرو DEF_CVT2BOOL_FUNC(suffix, _Ts, سرکاؤ) \
ساکن خالی cvt##suffix(ثابت uchar* src_, size_t sstep, ثابت uchar*, size_t, \
                        uchar* dst, size_t dstep, سائز size, خالی*) \
{ \
    CV_INSTRUMENT_REGION(); \
    ثابت _Ts* src = (ثابت _Ts*)src_; \
    sstep /= حجم(src[0]); \
    \
    برائے( صحیح i = 0; i < size.اونچائی; i++, src += sstep, dst += dstep ) { \
        برائے ( صحیح j = 0; j < size.چوڑائی; j++ ) \
            dst[j] = (src[j]<<سرکاؤ) != 0; \
    } \
}

#تعریف_کرو DEF_CVTBOOL2_FUNC(suffix, _Td, scale) \
ساکن خالی cvt##suffix(ثابت uchar* src, size_t sstep, ثابت uchar*, size_t, \
                        uchar* dst_, size_t dstep, سائز size, خالی*) \
{ \
    CV_INSTRUMENT_REGION(); \
    _Td* dst = (_Td*)dst_; \
    dstep /= حجم(dst[0]); \
    \
    برائے( صحیح i = 0; i < size.اونچائی; i++, src += sstep, dst += dstep ) { \
        برائے ( صحیح j = 0; j < size.چوڑائی; j++ ) \
            dst[j] = (_Td)((src[j] != 0)*scale); \
    } \
}

#تعریف_کرو DEF_CVT_SCALAR_FUNC(suffix, _Ts, _Td) \
ساکن خالی cvt##suffix(ثابت uchar* src_, size_t sstep, ثابت uchar*, size_t, \
                        uchar* dst_, size_t dstep, سائز size, خالی*) \
{ \
    CV_INSTRUMENT_REGION(); \
    ثابت _Ts* src = (ثابت _Ts*)src_; \
    _Td* dst = (_Td*)dst_; \
    sstep /= حجم(src[0]); \
    dstep /= حجم(dst[0]); \
    \
    برائے( صحیح i = 0; i < size.اونچائی; i++, src += sstep, dst += dstep ) { \
        برائے ( صحیح j = 0; j < size.چوڑائی; j++ ) \
            dst[j] = saturate_cast<_Td>(src[j]); \
    } \
}

#تعریف_کرو DEF_CVT_SCALAR_FUNC_S2U(suffix, _Ts, _Td, _Tw) \
ساکن خالی cvt##suffix(ثابت uchar* src_, size_t sstep, ثابت uchar*, size_t, \
                        uchar* dst_, size_t dstep, سائز size, خالی*) \
{ \
    CV_INSTRUMENT_REGION(); \
    ثابت _Ts* src = (ثابت _Ts*)src_; \
    _Td* dst = (_Td*)dst_; \
    sstep /= حجم(src[0]); \
    dstep /= حجم(dst[0]); \
    \
    برائے( صحیح i = 0; i < size.اونچائی; i++, src += sstep, dst += dstep ) { \
        برائے ( صحیح j = 0; j < size.چوڑائی; j++ ) \
            dst[j] = saturate_cast<_Td>(std::max((_Tw)src[j], (_Tw)0)); \
    } \
}

////////////////////// 8u -> ... ////////////////////////

DEF_CVT_FUNC(8u8s,  cvt_,  uchar, schar,    v_int16)
DEF_CVT_FUNC(8u16s, cvt_,  uchar, مختصر,    v_int16)
DEF_CVT_FUNC(8u32s, cvt_,  uchar, صحیح,      v_int32)
DEF_CVT_FUNC(8u32f, cvt_,  uchar, اعشاری,    v_float32)
DEF_CVT_FUNC(8u64f, cvt_,  uchar, دوہرا,   v_int32)
DEF_CVT_SCALAR_FUNC(8u64s, uchar, int64_t)
DEF_CVT_FUNC(8u16f, cvt1_, uchar, hfloat, v_float32)
DEF_CVT_FUNC(8u16bf, cvt1_, uchar, bfloat, v_float32)
DEF_CVT2BOOL_FUNC(8u8b, uchar, 0)

////////////////////// 8s -> ... ////////////////////////

DEF_CVT_FUNC(8s8u,  cvt_,  schar, uchar,    v_int16)
DEF_CVT_FUNC(8s16u, cvt_,  schar, ushort,   v_uint16)
DEF_CVT_FUNC(8s16s, cvt_,  schar, مختصر,    v_int16)
DEF_CVT_FUNC(8s32u, cvt_,  schar, غیرعلامتی, v_uint32)
DEF_CVT_FUNC(8s32s, cvt_,  schar, صحیح,      v_int32)
DEF_CVT_FUNC(8s32f, cvt_,  schar, اعشاری,    v_float32)
DEF_CVT_FUNC(8s64f, cvt_,  schar, دوہرا,   v_int32)
DEF_CVT_FUNC(8s64u, cvt_,  schar, uint64_t, v_uint32)
DEF_CVT_FUNC(8s64s, cvt_,  schar, int64_t,  v_int32)
DEF_CVT_FUNC(8s16f, cvt1_, schar, hfloat, v_float32)
DEF_CVT_FUNC(8s16bf, cvt1_, schar, bfloat, v_float32)

////////////////////// 8b -> ... ////////////////////////

DEF_CVTBOOL2_FUNC(8b8u,  uchar, 1)
DEF_CVTBOOL2_FUNC(8b16s, مختصر, 1)
DEF_CVTBOOL2_FUNC(8b32s, صحیح, 1)
DEF_CVTBOOL2_FUNC(8b32f, اعشاری, 1)
DEF_CVTBOOL2_FUNC(8b64f, دوہرا, 1)
DEF_CVTBOOL2_FUNC(8b64s, int64_t, 1)
DEF_CVTBOOL2_FUNC(8b16f, uint16_t, 0x3c00) // hfloat(1.0f)
DEF_CVTBOOL2_FUNC(8b16bf, uint16_t, 0x3f80) // bfloat(1.0f)

////////////////////// 16u -> ... ////////////////////////

DEF_CVT_FUNC(16u8u,  cvt_, ushort, uchar,  v_uint16)
DEF_CVT_FUNC(16u8s,  cvt_, ushort, schar,  v_uint16)
DEF_CVT_FUNC(16u16s, cvt_, ushort, مختصر,  v_int32)
DEF_CVT_FUNC(16u32s, cvt_, ushort, صحیح,    v_int32)
DEF_CVT_FUNC(16u32f, cvt_, ushort, اعشاری,  v_float32)
DEF_CVT_FUNC(16u64f, cvt_, ushort, دوہرا, v_int32)
DEF_CVT_SCALAR_FUNC(16u64s, ushort, int64_t)
DEF_CVT_FUNC(16u16f, cvt1_,ushort, hfloat, v_float32)
DEF_CVT_FUNC(16u16bf, cvt1_, ushort, bfloat, v_float32)

////////////////////// 16s -> ... ////////////////////////

DEF_CVT_FUNC(16s8u,  cvt_, مختصر, uchar,  v_int16)
DEF_CVT_FUNC(16s8s,  cvt_, مختصر, schar,  v_int16)
DEF_CVT_FUNC(16s16u, cvt_, مختصر, ushort, v_int32)
DEF_CVT_FUNC(16s32u, cvt_, مختصر, غیرعلامتی, v_uint32)
DEF_CVT_FUNC(16s32s, cvt_, مختصر, صحیح,    v_int32)
DEF_CVT_FUNC(16s32f, cvt_, مختصر, اعشاری,  v_float32)
DEF_CVT_FUNC(16s64f, cvt_, مختصر, دوہرا, v_int32)
DEF_CVT_FUNC(16s64u, cvt_, مختصر, uint64_t, v_uint32)
DEF_CVT_FUNC(16s64s, cvt_, مختصر, int64_t, v_int32)
DEF_CVT_FUNC(16s16f, cvt1_,مختصر, hfloat, v_float32)
DEF_CVT_FUNC(16s16bf, cvt1_, مختصر, bfloat, v_float32)
DEF_CVT2BOOL_FUNC(16s8b, مختصر, 0)

////////////////////// 32u -> ... ////////////////////////

DEF_CVT_FUNC(32u8u,  cvt_, غیرعلامتی, uchar,  v_uint32)
DEF_CVT_FUNC(32u8s,  cvt_, غیرعلامتی, schar,  v_int32)
DEF_CVT_FUNC(32u16u, cvt_, غیرعلامتی, ushort, v_uint32)
DEF_CVT_FUNC(32u16s, cvt_, غیرعلامتی, مختصر,  v_int32)
DEF_CVT_SCALAR_FUNC(32u32s, غیرعلامتی, صحیح)
DEF_CVT_FUNC(32u32f, cvt_, غیرعلامتی, اعشاری,  v_float32)
DEF_CVT_FUNC(32u64f, cvt_64f, غیرعلامتی, دوہرا, v_float64)
DEF_CVT_SCALAR_FUNC(32u64s, غیرعلامتی, int64_t)
DEF_CVT_SCALAR_FUNC(64u64s, uint64_t, int64_t)
DEF_CVT_FUNC(32u16f, cvt1_, غیرعلامتی, hfloat, v_float32)
DEF_CVT_FUNC(32u16bf, cvt1_, غیرعلامتی, bfloat, v_float32)

////////////////////// 32s -> ... ////////////////////////

DEF_CVT_FUNC(32s8u,  cvt_, صحیح, uchar,  v_int32)
DEF_CVT_FUNC(32s8s,  cvt_, صحیح, schar,  v_int32)
DEF_CVT_FUNC(32s16u, cvt_, صحیح, ushort, v_int32)
DEF_CVT_FUNC(32s16s, cvt_, صحیح, مختصر,  v_int32)
DEF_CVT_FUNC(32s32u, cvt_, صحیح, غیرعلامتی, v_uint32)
DEF_CVT_FUNC(32s32f, cvt_, صحیح, اعشاری,  v_float32)
DEF_CVT_FUNC(32s64f, cvt_, صحیح, دوہرا, v_int32)
DEF_CVT_FUNC(32s64u, cvt_, صحیح, uint64_t, v_uint32)
DEF_CVT_FUNC(32s64s, cvt_, صحیح, int64_t, v_int32)
DEF_CVT_FUNC(32s16f, cvt1_,صحیح, hfloat, v_float32)
DEF_CVT_FUNC(32s16bf, cvt1_, صحیح, bfloat, v_float32)
DEF_CVT2BOOL_FUNC(32s8b, صحیح, 0)

////////////////////// 32f -> ... ////////////////////////

DEF_CVT_FUNC(32f8u,  cvt_, اعشاری, uchar,  v_float32)
DEF_CVT_FUNC(32f8s,  cvt_, اعشاری, schar,  v_float32)
DEF_CVT_FUNC(32f16u, cvt_, اعشاری, ushort, v_float32)
DEF_CVT_FUNC(32f16s, cvt_, اعشاری, مختصر,  v_float32)
DEF_CVT_FUNC(32f32u, cvt_, اعشاری, غیرعلامتی, v_float32)
DEF_CVT_FUNC(32f32s, cvt_, اعشاری, صحیح,    v_float32)
DEF_CVT_FUNC(32f64f, cvt_, اعشاری, دوہرا, v_float32)
DEF_CVT_FUNC(32f64u, cvt_64f, اعشاری, uint64_t, v_float64)
DEF_CVT_FUNC(32f64s, cvt_64f, اعشاری, int64_t, v_float64)
DEF_CVT_FUNC(32f16f, cvt1_,اعشاری, hfloat, v_float32)
DEF_CVT_FUNC(32f16bf, cvt1_,اعشاری, bfloat, v_float32)
DEF_CVT2BOOL_FUNC(32f8b, صحیح, 1)

////////////////////// 64f -> ... ////////////////////////

DEF_CVT_FUNC(64f8u,  cvt_, دوہرا, uchar,  v_int32)
DEF_CVT_FUNC(64f8s,  cvt_, دوہرا, schar,  v_int32)
DEF_CVT_FUNC(64f16u, cvt_, دوہرا, ushort, v_int32)
DEF_CVT_FUNC(64f16s, cvt_, دوہرا, مختصر,  v_int32)
DEF_CVT_FUNC(64f32u, cvt_64f, دوہرا, غیرعلامتی, v_float32)
DEF_CVT_FUNC(64f32s, cvt_, دوہرا, صحیح,    v_int32)
DEF_CVT_FUNC(64f32f, cvt_, دوہرا, اعشاری,  v_float32)
DEF_CVT_FUNC(64f64u, cvt_64f, دوہرا, uint64_t, v_float64)
DEF_CVT_FUNC(64f64s, cvt_64f, دوہرا, int64_t, v_float64)
DEF_CVT_FUNC(64f16f, cvt1_,دوہرا, hfloat, v_float32)
DEF_CVT_FUNC(64f16bf, cvt1_,دوہرا, bfloat, v_float32)
DEF_CVT2BOOL_FUNC(64f8b, int64_t, 1)

////////////////////// 16f -> ... ////////////////////////

DEF_CVT_FUNC(16f8u,  cvt_,  hfloat, uchar,  v_float32)
DEF_CVT_FUNC(16f8s,  cvt_,  hfloat, schar,  v_float32)
DEF_CVT_FUNC(16f16u, cvt1_, hfloat, ushort, v_float32)
DEF_CVT_FUNC(16f16s, cvt1_, hfloat, مختصر,  v_float32)
DEF_CVT_FUNC(16f32u, cvt1_, hfloat, غیرعلامتی, v_float32)
DEF_CVT_FUNC(16f32s, cvt1_, hfloat, صحیح,    v_float32)
DEF_CVT_FUNC(16f32f, cvt1_, hfloat, اعشاری,  v_float32)
DEF_CVT_FUNC(16f64f, cvt1_, hfloat, دوہرا, v_float32)
DEF_CVT_FUNC(16f64u, cvt1_, hfloat, uint64_t, v_float32)
DEF_CVT_FUNC(16f64s, cvt1_, hfloat, int64_t, v_float32)
DEF_CVT_FUNC(16f16bf, cvt1_, hfloat, bfloat, v_float32)
DEF_CVT2BOOL_FUNC(16f8b, مختصر, 1)

////////////////////// 16bf -> ... ////////////////////////

DEF_CVT_FUNC(16bf8u,  cvt_,  bfloat, uchar,  v_float32)
DEF_CVT_FUNC(16bf8s,  cvt_,  bfloat, schar,  v_float32)
DEF_CVT_FUNC(16bf16u, cvt1_, bfloat, ushort, v_float32)
DEF_CVT_FUNC(16bf16s, cvt1_, bfloat, مختصر,  v_float32)
DEF_CVT_FUNC(16bf32u, cvt1_, bfloat, غیرعلامتی, v_float32)
DEF_CVT_FUNC(16bf32s, cvt1_, bfloat, صحیح,    v_float32)
DEF_CVT_FUNC(16bf32f, cvt1_, bfloat, اعشاری,  v_float32)
DEF_CVT_FUNC(16bf64f, cvt1_, bfloat, دوہرا, v_float32)
DEF_CVT_FUNC(16bf64u, cvt1_, bfloat, uint64_t, v_float32)
DEF_CVT_FUNC(16bf64s, cvt1_, bfloat, int64_t, v_float32)
DEF_CVT_FUNC(16bf16f, cvt1_, bfloat, hfloat, v_float32)

////////////////////// 64s -> ... ////////////////////////

DEF_CVT_FUNC(64s8u,  cvt_, int64_t, uchar,  v_int32)
DEF_CVT_FUNC(64s8s,  cvt_, int64_t, schar,  v_int32)
DEF_CVT_FUNC(64s16u, cvt_, int64_t, ushort, v_int32)
DEF_CVT_FUNC(64s16s, cvt_, int64_t, مختصر,  v_int32)
DEF_CVT_FUNC(64s32u, cvt_, int64_t, غیرعلامتی, v_uint32)
DEF_CVT_FUNC(64s32s, cvt_, int64_t, صحیح,    v_int32)
DEF_CVT_FUNC(64s32f, cvt_64f, int64_t, اعشاری,  v_float32)
DEF_CVT_FUNC(64s64f, cvt_64f, int64_t, دوہرا,  v_float64)
DEF_CVT_FUNC(64s64u, cvt_, int64_t, uint64_t, v_uint64)
DEF_CVT_FUNC(64s16f, cvt1_,int64_t, hfloat, v_float32)
DEF_CVT_FUNC(64s16bf, cvt1_, int64_t, bfloat, v_float32)
DEF_CVT2BOOL_FUNC(64s8b, int64_t, 0)

////////////////////// 64u -> ... ////////////////////////

DEF_CVT_FUNC(64u8u,  cvt_, uint64_t, uchar,  v_int32)
DEF_CVT_FUNC(64u8s,  cvt_, uint64_t, schar,  v_int32)
DEF_CVT_FUNC(64u16u, cvt_, uint64_t, ushort, v_int32)
DEF_CVT_FUNC(64u16s, cvt_, uint64_t, مختصر,  v_int32)
DEF_CVT_FUNC(64u32u, cvt_, uint64_t, غیرعلامتی, v_uint32)
DEF_CVT_FUNC(64u32s, cvt_, uint64_t, صحیح,   v_int32)
DEF_CVT_FUNC(64u32f, cvt_64f, uint64_t, اعشاری,  v_float64)
DEF_CVT_FUNC(64u64f, cvt_64f, uint64_t, دوہرا,  v_float64)
DEF_CVT_FUNC(64u16f, cvt1_,uint64_t, hfloat, v_float32)
DEF_CVT_FUNC(64u16bf, cvt1_, uint64_t, bfloat, v_float32)

///////////// "conversion" w/o conversion ///////////////

ساکن خالی cvt8u(ثابت uchar* src, size_t sstep, ثابت uchar*, size_t, uchar* dst, size_t dstep, سائز size, خالی*)
{ CV_INSTRUMENT_REGION(); cvtCopy(src, sstep, dst, dstep, size, 1); }

ساکن خالی cvt16u(ثابت uchar* src, size_t sstep, ثابت uchar*, size_t, uchar* dst, size_t dstep, سائز size, خالی*)
{ CV_INSTRUMENT_REGION(); cvtCopy((ثابت uchar*)src, sstep, (uchar*)dst, dstep, size, 2); }

ساکن خالی cvt32s(ثابت uchar* src, size_t sstep, ثابت uchar*, size_t, uchar* dst, size_t dstep, سائز size, خالی*)
{ CV_INSTRUMENT_REGION(); cvtCopy((ثابت uchar*)src, sstep, (uchar*)dst, dstep, size, 4); }

ساکن خالی cvt64s(ثابت uchar* src, size_t sstep, ثابت uchar*, size_t, uchar* dst, size_t dstep, سائز size, خالی*)
{ CV_INSTRUMENT_REGION(); cvtCopy((ثابت uchar*)src, sstep, (uchar*)dst, dstep, size, 8); }

//////////////////// FP8 (1-byte float) conversions — scalar via saturate_cast ////////////////////
// Suffixes: 8fe4m3 (E4M3FN), 8fe4m3u (E4M3FNUZ).
#تعریف_کرو DEF_CVT_FP8(S, T) \
    DEF_CVT_SCALAR_FUNC(S##8u,   T, uchar)     DEF_CVT_SCALAR_FUNC(8u##S,   uchar, T) \
    DEF_CVT_SCALAR_FUNC(S##8s,   T, schar)     DEF_CVT_SCALAR_FUNC(8s##S,   schar, T) \
    DEF_CVT_SCALAR_FUNC(S##16u,  T, ushort)    DEF_CVT_SCALAR_FUNC(16u##S,  ushort, T) \
    DEF_CVT_SCALAR_FUNC(S##16s,  T, مختصر)     DEF_CVT_SCALAR_FUNC(16s##S,  مختصر, T) \
    DEF_CVT_SCALAR_FUNC(S##32u,  T, غیرعلامتی)  DEF_CVT_SCALAR_FUNC(32u##S,  غیرعلامتی, T) \
    DEF_CVT_SCALAR_FUNC(S##32s,  T, صحیح)       DEF_CVT_SCALAR_FUNC(32s##S,  صحیح, T) \
    DEF_CVT_SCALAR_FUNC(S##64f,  T, دوہرا)    DEF_CVT_SCALAR_FUNC(64f##S,  دوہرا, T) \
    DEF_CVT_SCALAR_FUNC(S##16f,  T, hfloat)    DEF_CVT_SCALAR_FUNC(16f##S,  hfloat, T) \
    DEF_CVT_SCALAR_FUNC(S##16bf, T, bfloat)    DEF_CVT_SCALAR_FUNC(16bf##S, bfloat, T) \
    DEF_CVT_SCALAR_FUNC(S##64u,  T, uint64_t)  DEF_CVT_SCALAR_FUNC(64u##S,  uint64_t, T) \
    DEF_CVT_SCALAR_FUNC(S##64s,  T, int64_t)   DEF_CVT_SCALAR_FUNC(64s##S,  int64_t, T) \
    DEF_CVT_SCALAR_FUNC(S##8b,   T, منطقی)

DEF_CVT_FP8(8fe4m3,  fp8_t)
DEF_CVT_FP8(8fe4m3u, fp8a_t)

// FP8 -> FP8, cross-format only (identity uses the 1-byte copy cvt8u, like 16f->16f uses cvt16u)
DEF_CVT_SCALAR_FUNC(8fe4m38fe4m3u, fp8_t,   fp8a_t)
DEF_CVT_SCALAR_FUNC(8fe4m3u8fe4m3, fp8a_t, fp8_t)

// float32 -> FP8 encode via the shared building blocks in convert.hpp.
سانچہ<صحیح bias, منطقی fnuz> ساکن خالی
cvt_F32_مقصد_Fp8(ثابت uchar* src_, size_t sstep, ثابت uchar*, size_t,
            uchar* dst, size_t dstep, سائز size, خالی*)
{
    CV_INSTRUMENT_REGION();
    ثابت اعشاری* src = (ثابت اعشاری*)src_;
    sstep /= حجم(src[0]);

    برائے (صحیح i = 0; i < size.اونچائی; i++, src += sstep, dst += dstep)
    {
        صحیح j = 0;
#اگر (CV_SIMD || CV_SIMD_SCALABLE)
        ثابت صحیح LANES32 = VTraits<v_int32>::vlanes();
        ثابت صحیح VECSZ = LANES32 * 4;
        برائے (; j <= size.چوڑائی - VECSZ; j += VECSZ)
        {
            v_float32 vf0 = vx_بھرو(src + j);
            v_float32 vf1 = vx_بھرو(src + j + LANES32);
            v_float32 vf2 = vx_بھرو(src + j + 2 * LANES32);
            v_float32 vf3 = vx_بھرو(src + j + 3 * LANES32);
            encodeFp8Vec4<bias, fnuz>(vf0, vf1, vf2, vf3, src + j, VECSZ, dst + j);
        }
        vx_cleanup();
#اگر_ختم
        برائے (; j < size.چوڑائی; j++)
            dst[j] = fp8_detail::encodeE4M3(src[j], bias, fnuz);
    }
}
ساکن خالی cvt32f8fe4m3(ثابت uchar* s, size_t ss, ثابت uchar* p, size_t ps, uchar* d, size_t ds, سائز sz, خالی* x)
{ cvt_F32_مقصد_Fp8<7, جھوٹ>(s, ss, p, ps, d, ds, sz, x); }
ساکن خالی cvt32f8fe4m3u(ثابت uchar* s, size_t ss, ثابت uchar* p, size_t ps, uchar* d, size_t ds, سائز sz, خالی* x)
{ cvt_F32_مقصد_Fp8<8, سچ>(s, ss, p, ps, d, ds, sz, x); }

// FP8 -> float32: 256-entry decode table gathered via universal intrinsics (same table as scalar)
سانچہ<قسم_نام FP8>
ساکن خالی cvt_Fp8_مقصد_F32(ثابت uchar* src, size_t sstep, ثابت uchar*, size_t,
                        uchar* dst_, size_t dstep, سائز size, خالی*)
{
    CV_INSTRUMENT_REGION();
    اعشاری* dst = (اعشاری*)dst_;
    ثابت اعشاری* tab = FP8::decodeLUT();
    dstep /= حجم(dst[0]);
    برائے (صحیح i = 0; i < size.اونچائی; i++, src += sstep, dst += dstep)
    {
        صحیح j = 0;
#اگر (CV_SIMD || CV_SIMD_SCALABLE)
        ثابت صحیح VECSZ = VTraits<v_float32>::vlanes();
        برائے (; j <= size.چوڑائی - VECSZ; j += VECSZ)
            v_store(dst + j, v_lut(tab, v_reinterpret_as_s32(vx_بھرو_expand_q(src + j))));
#اگر_ختم
        برائے (; j < size.چوڑائی; j++) dst[j] = tab[src[j]];
    }
}
ساکن خالی cvt8fe4m332f(ثابت uchar* s, size_t ss, ثابت uchar* p, size_t ps, uchar* d, size_t ds, سائز sz, خالی* x)
{ cvt_Fp8_مقصد_F32<fp8_t>(s, ss, p, ps, d, ds, sz, x); }
ساکن خالی cvt8fe4m3u32f(ثابت uchar* s, size_t ss, ثابت uchar* p, size_t ps, uchar* d, size_t ds, سائز sz, خالی* x)
{ cvt_Fp8_مقصد_F32<fp8a_t>(s, ss, p, ps, d, ds, sz, x); }

ثنائی_Func getConvertFunc(صحیح sdepth_, صحیح ddepth_)
{
    صحیح sdepth = CV_MAT_گہرائی(sdepth_);
    صحیح ddepth = CV_MAT_گہرائی(ddepth_);
    ثنائی_Func func =
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
    واپس func;
}

CV_CPU_OPTIMIZATION_NAMESPACE_انتہا
#اگر_ختم
} // namespace
