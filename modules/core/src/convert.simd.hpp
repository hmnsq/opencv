// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html

#ਅਨ੍ਤਰ੍ਭਾਵ "precomp.hpp"
#ਅਨ੍ਤਰ੍ਭਾਵ "convert.hpp"

#ਜੇ !defined(OPENCV_SUPRESS_WARNING_AVX2_WITHOUT_FP16C) && \
    (defined(__GNUC__) && defined(__AVX2__) && !defined(__F16C__))
#warning "Non-optimal compiler flags: AVX2 without FP16. Generated code is very slow. Consider adding '-mf16c' compiler option."
#ਜੇ_ਅੰਤ

ਨਾਮਾਕਾਸ਼ cv {
ਨਾਮਾਕਾਸ਼ hal {
CV_CPU_OPTIMIZATION_NAMESPACE_BEGIN

ਖਾਲੀ cvt16f32f(ਅਚਲ hfloat* src, ਦਸ਼ਮਲਵ* dst, ਪੂਰਨ len);
ਖਾਲੀ cvt32f16f(ਅਚਲ ਦਸ਼ਮਲਵ* src, hfloat* dst, ਪੂਰਨ len);
ਖਾਲੀ cvt16bf32f(ਅਚਲ bfloat* src, ਦਸ਼ਮਲਵ* dst, ਪੂਰਨ len);
ਖਾਲੀ cvt32f16bf(ਅਚਲ ਦਸ਼ਮਲਵ* src, bfloat* dst, ਪੂਰਨ len);
ਖਾਲੀ ਜੋੜੋ_RNGBias32f(ਦਸ਼ਮਲਵ* arr, ਅਚਲ ਦਸ਼ਮਲਵ* scale_Bias_ਜੋੜੇ, ਪੂਰਨ len, ਪੂਰਨ cn);
ਖਾਲੀ ਜੋੜੋ_RNGBias64f(ਦੁੱਗਣਾ* arr, ਅਚਲ ਦੁੱਗਣਾ* scale_Bias_ਜੋੜੇ, ਪੂਰਨ len, ਪੂਰਨ cn);

CV_CPU_OPTIMIZATION_NAMESPACE_ਸਿਰਾ
} // namespace cv::hal

CV_CPU_OPTIMIZATION_NAMESPACE_BEGIN

ਦੁਆਧਾਰੀ_Func getConvertFunc(ਪੂਰਨ sdepth, ਪੂਰਨ ddepth);

CV_CPU_OPTIMIZATION_NAMESPACE_ਸਿਰਾ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

ਨਾਮਾਕਾਸ਼ hal {
CV_CPU_OPTIMIZATION_NAMESPACE_BEGIN

ਦੁਆਧਾਰੀ_Func getConvertFunc(ਪੂਰਨ sdepth, ਪੂਰਨ ddepth);

ਖਾਲੀ cvt16f32f( ਅਚਲ hfloat* src, ਦਸ਼ਮਲਵ* dst, ਪੂਰਨ len )
{
    CV_INSTRUMENT_REGION();
    ਪੂਰਨ j = 0;
#ਜੇ (CV_SIMD || CV_SIMD_SCALABLE)
    ਅਚਲ ਪੂਰਨ VECSZ = VTraits<v_float32>::vlanes();
    ਲਈ( ; j < len; j += VECSZ )
    {
        ਜੇ( j > len - VECSZ )
        {
            ਜੇ( j == 0 )
                ਰੋਕੋ;
            j = len - VECSZ;
        }
        v_store(dst + j, vx_ਲੱਦੋ_expand(src + j));
    }
#ਜੇ_ਅੰਤ
    ਲਈ( ; j < len; j++ )
        dst[j] = (ਦਸ਼ਮਲਵ)src[j];
}

ਖਾਲੀ cvt32f16f( ਅਚਲ ਦਸ਼ਮਲਵ* src, hfloat* dst, ਪੂਰਨ len )
{
    CV_INSTRUMENT_REGION();
    ਪੂਰਨ j = 0;
#ਜੇ (CV_SIMD || CV_SIMD_SCALABLE)
    ਅਚਲ ਪੂਰਨ VECSZ = VTraits<v_float32>::vlanes();
    ਲਈ( ; j < len; j += VECSZ )
    {
        ਜੇ( j > len - VECSZ )
        {
            ਜੇ( j == 0 )
                ਰੋਕੋ;
            j = len - VECSZ;
        }
        v_pack_store(dst + j, vx_ਲੱਦੋ(src + j));
    }
#ਜੇ_ਅੰਤ
    ਲਈ( ; j < len; j++ )
        dst[j] = hfloat(src[j]);
}

ਖਾਲੀ cvt32f16bf( ਅਚਲ ਦਸ਼ਮਲਵ* src, bfloat* dst, ਪੂਰਨ len )
{
    CV_INSTRUMENT_REGION();
    ਪੂਰਨ j = 0;
#ਜੇ (CV_SIMD || CV_SIMD_SCALABLE)
    ਅਚਲ ਪੂਰਨ VECSZ = VTraits<v_float32>::vlanes();
    ਲਈ( ; j < len; j += VECSZ )
    {
        ਜੇ( j > len - VECSZ )
        {
            ਜੇ( j == 0 )
                ਰੋਕੋ;
            j = len - VECSZ;
        }
        v_pack_store(dst + j, vx_ਲੱਦੋ(src + j));
    }
#ਜੇ_ਅੰਤ
    ਲਈ( ; j < len; j++ )
        dst[j] = bfloat(src[j]);
}

ਖਾਲੀ ਜੋੜੋ_RNGBias32f( ਦਸ਼ਮਲਵ* arr, ਅਚਲ ਦਸ਼ਮਲਵ* scale_Bias_ਜੋੜੇ, ਪੂਰਨ len, ਪੂਰਨ cn )
{
    CV_INSTRUMENT_REGION();
    ਜੇ (cn == 1) {
        ਦਸ਼ਮਲਵ bias = scale_Bias_ਜੋੜੇ[1];
        ਲਈ( ਪੂਰਨ i = 0; i < len; i++ ) {
            arr[i] += bias;
        }
    } ਨਹੀਂਤਾਂ {
        ਪੂਰਨ k = 0;
        len *= cn;
        cn--;
        ਲਈ( ਪੂਰਨ i = 0; i < len; i++ ) {
            arr[i] += scale_Bias_ਜੋੜੇ[k*2 + 1];
            k = (k + 1) & ((k >= cn) - 1);
        }
    }
}

ਖਾਲੀ ਜੋੜੋ_RNGBias64f( ਦੁੱਗਣਾ* arr, ਅਚਲ ਦੁੱਗਣਾ* scale_Bias_ਜੋੜੇ, ਪੂਰਨ len, ਪੂਰਨ cn )
{
    CV_INSTRUMENT_REGION();
    ਜੇ (cn == 1) {
        ਦੁੱਗਣਾ bias = scale_Bias_ਜੋੜੇ[1];
        ਲਈ( ਪੂਰਨ i = 0; i < len; i++ ) {
            arr[i] += bias;
        }
    } ਨਹੀਂਤਾਂ {
        ਪੂਰਨ k = 0;
        len *= cn;
        cn--;
        ਲਈ( ਪੂਰਨ i = 0; i < len; i++ ) {
            arr[i] += scale_Bias_ਜੋੜੇ[k*2 + 1];
            k = (k + 1) & ((k >= cn) - 1);
        }
    }
}

CV_CPU_OPTIMIZATION_NAMESPACE_ਸਿਰਾ
} // namespace cv::hal

// cv::
CV_CPU_OPTIMIZATION_NAMESPACE_BEGIN

ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ _Ts, ਕਿਸਮ_ਨਾਂ _Td, ਕਿਸਮ_ਨਾਂ _Twvec> ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ
cvt_( ਅਚਲ _Ts* src, size_t sstep, _Td* dst, size_t dstep, ਮਾਪ size )
{
    sstep /= ਆਕਾਰ(src[0]);
    dstep /= ਆਕਾਰ(dst[0]);

    ਲਈ( ਪੂਰਨ i = 0; i < size.ਉਚਾਈ; i++, src += sstep, dst += dstep )
    {
        ਪੂਰਨ j = 0;
// Excluding GNU in CV_SIMD_SCALABLE because of "opencv/issues/26936"
#ਜੇ (CV_SIMD || (CV_SIMD_SCALABLE && !(defined(__GNUC__) && !defined(__clang__))) )
        ਅਚਲ ਪੂਰਨ VECSZ = VTraits<_Twvec>::vlanes()*2;
        ਲਈ( ; j < size.ਚੌੜਾਈ; j += VECSZ )
        {
            ਜੇ( j > size.ਚੌੜਾਈ - VECSZ )
            {
                ਜੇ( j == 0 || src == (_Ts*)dst )
                    ਰੋਕੋ;
                j = size.ਚੌੜਾਈ - VECSZ;
            }
            _Twvec v0, v1;
            vx_ਲੱਦੋ_pair_as(src + j, v0, v1);
            v_store_pair_as(dst + j, v0, v1);
        }
#ਜੇ_ਅੰਤ
        ਲਈ( ; j < size.ਚੌੜਾਈ; j++ )
            dst[j] = saturate_cast<_Td>(src[j]);
    }
}

ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ _Ts, ਕਿਸਮ_ਨਾਂ _Td, ਕਿਸਮ_ਨਾਂ dummy> ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ
cvt_64f( ਅਚਲ _Ts* src, size_t sstep, _Td* dst, size_t dstep, ਮਾਪ size )
{
    sstep /= ਆਕਾਰ(src[0]);
    dstep /= ਆਕਾਰ(dst[0]);

    ਲਈ( ਪੂਰਨ i = 0; i < size.ਉਚਾਈ; i++, src += sstep, dst += dstep )
    {
        ਪੂਰਨ j = 0;
#ਜੇ (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)
        ਅਚਲ ਪੂਰਨ VECSZ = VTraits<v_float64>::vlanes()*2;
        ਲਈ( ; j < size.ਚੌੜਾਈ; j += VECSZ )
        {
            ਜੇ( j > size.ਚੌੜਾਈ - VECSZ )
            {
                ਜੇ( j == 0 || src == (_Ts*)dst )
                    ਰੋਕੋ;
                j = size.ਚੌੜਾਈ - VECSZ;
            }
            v_float64 v0, v1;
            vx_ਲੱਦੋ_pair_as(src + j, v0, v1);
            v_store_pair_as(dst + j, v0, v1);
        }
#ਜੇ_ਅੰਤ
        ਲਈ( ; j < size.ਚੌੜਾਈ; j++ )
            dst[j] = saturate_cast<_Td>(src[j]);
    }
}

// in order to reduce the code size, for (16f <-> ...) conversions
// we add a conversion function without loop unrolling
ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ _Ts, ਕਿਸਮ_ਨਾਂ _Td, ਕਿਸਮ_ਨਾਂ _Twvec> ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ
cvt1_( ਅਚਲ _Ts* src, size_t sstep, _Td* dst, size_t dstep, ਮਾਪ size )
{
    sstep /= ਆਕਾਰ(src[0]);
    dstep /= ਆਕਾਰ(dst[0]);

    ਲਈ( ਪੂਰਨ i = 0; i < size.ਉਚਾਈ; i++, src += sstep, dst += dstep )
    {
        ਪੂਰਨ j = 0;
#ਜੇ (CV_SIMD || CV_SIMD_SCALABLE)
        ਅਚਲ ਪੂਰਨ VECSZ = VTraits<_Twvec>::vlanes();
        ਲਈ( ; j < size.ਚੌੜਾਈ; j += VECSZ )
        {
            ਜੇ( j > size.ਚੌੜਾਈ - VECSZ )
            {
                ਜੇ( j == 0 || src == (_Ts*)dst )
                    ਰੋਕੋ;
                j = size.ਚੌੜਾਈ - VECSZ;
            }
            _Twvec v;
            vx_ਲੱਦੋ_as(src + j, v);
            v_store_as(dst + j, v);
        }
        vx_cleanup();
#ਜੇ_ਅੰਤ
        ਲਈ( ; j < size.ਚੌੜਾਈ; j++ )
            dst[j] = saturate_cast<_Td>(src[j]);
    }
}

ਸਥਿਰ ਖਾਲੀ cvtCopy( ਅਚਲ uchar* src, size_t sstep,
                     uchar* dst, size_t dstep, ਮਾਪ size, size_t elemsize)
{
    size_t len = size.ਚੌੜਾਈ*elemsize;
    ਲਈ( ਪੂਰਨ i = 0; i < size.ਉਚਾਈ; i++, src += sstep, dst += dstep )
    {
        ਸਮ੍ਰਿਤੀ_ਨਕਲ( dst, src, len );
    }
}

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ DEF_CVT_FUNC(suffix, cvtfunc, _Ts, _Td, _Twvec) \
ਸਥਿਰ ਖਾਲੀ cvt##suffix(ਅਚਲ uchar* src_, size_t sstep, ਅਚਲ uchar*, size_t, \
                        uchar* dst_, size_t dstep, ਮਾਪ size, ਖਾਲੀ*) \
{ \
    CV_INSTRUMENT_REGION(); \
    ਅਚਲ _Ts* src = (ਅਚਲ _Ts*)src_; \
    _Td* dst = (_Td*)dst_; \
    cvtfunc<_Ts, _Td, _Twvec>(src, sstep, dst, dstep, size); \
}

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ DEF_CVT2BOOL_FUNC(suffix, _Ts, ਖਿਸਕਾਓ) \
ਸਥਿਰ ਖਾਲੀ cvt##suffix(ਅਚਲ uchar* src_, size_t sstep, ਅਚਲ uchar*, size_t, \
                        uchar* dst, size_t dstep, ਮਾਪ size, ਖਾਲੀ*) \
{ \
    CV_INSTRUMENT_REGION(); \
    ਅਚਲ _Ts* src = (ਅਚਲ _Ts*)src_; \
    sstep /= ਆਕਾਰ(src[0]); \
    \
    ਲਈ( ਪੂਰਨ i = 0; i < size.ਉਚਾਈ; i++, src += sstep, dst += dstep ) { \
        ਲਈ ( ਪੂਰਨ j = 0; j < size.ਚੌੜਾਈ; j++ ) \
            dst[j] = (src[j]<<ਖਿਸਕਾਓ) != 0; \
    } \
}

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ DEF_CVTBOOL2_FUNC(suffix, _Td, scale) \
ਸਥਿਰ ਖਾਲੀ cvt##suffix(ਅਚਲ uchar* src, size_t sstep, ਅਚਲ uchar*, size_t, \
                        uchar* dst_, size_t dstep, ਮਾਪ size, ਖਾਲੀ*) \
{ \
    CV_INSTRUMENT_REGION(); \
    _Td* dst = (_Td*)dst_; \
    dstep /= ਆਕਾਰ(dst[0]); \
    \
    ਲਈ( ਪੂਰਨ i = 0; i < size.ਉਚਾਈ; i++, src += sstep, dst += dstep ) { \
        ਲਈ ( ਪੂਰਨ j = 0; j < size.ਚੌੜਾਈ; j++ ) \
            dst[j] = (_Td)((src[j] != 0)*scale); \
    } \
}

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ DEF_CVT_SCALAR_FUNC(suffix, _Ts, _Td) \
ਸਥਿਰ ਖਾਲੀ cvt##suffix(ਅਚਲ uchar* src_, size_t sstep, ਅਚਲ uchar*, size_t, \
                        uchar* dst_, size_t dstep, ਮਾਪ size, ਖਾਲੀ*) \
{ \
    CV_INSTRUMENT_REGION(); \
    ਅਚਲ _Ts* src = (ਅਚਲ _Ts*)src_; \
    _Td* dst = (_Td*)dst_; \
    sstep /= ਆਕਾਰ(src[0]); \
    dstep /= ਆਕਾਰ(dst[0]); \
    \
    ਲਈ( ਪੂਰਨ i = 0; i < size.ਉਚਾਈ; i++, src += sstep, dst += dstep ) { \
        ਲਈ ( ਪੂਰਨ j = 0; j < size.ਚੌੜਾਈ; j++ ) \
            dst[j] = saturate_cast<_Td>(src[j]); \
    } \
}

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ DEF_CVT_SCALAR_FUNC_S2U(suffix, _Ts, _Td, _Tw) \
ਸਥਿਰ ਖਾਲੀ cvt##suffix(ਅਚਲ uchar* src_, size_t sstep, ਅਚਲ uchar*, size_t, \
                        uchar* dst_, size_t dstep, ਮਾਪ size, ਖਾਲੀ*) \
{ \
    CV_INSTRUMENT_REGION(); \
    ਅਚਲ _Ts* src = (ਅਚਲ _Ts*)src_; \
    _Td* dst = (_Td*)dst_; \
    sstep /= ਆਕਾਰ(src[0]); \
    dstep /= ਆਕਾਰ(dst[0]); \
    \
    ਲਈ( ਪੂਰਨ i = 0; i < size.ਉਚਾਈ; i++, src += sstep, dst += dstep ) { \
        ਲਈ ( ਪੂਰਨ j = 0; j < size.ਚੌੜਾਈ; j++ ) \
            dst[j] = saturate_cast<_Td>(std::max((_Tw)src[j], (_Tw)0)); \
    } \
}

////////////////////// 8u -> ... ////////////////////////

DEF_CVT_FUNC(8u8s,  cvt_,  uchar, schar,    v_int16)
DEF_CVT_FUNC(8u16s, cvt_,  uchar, ਛੋਟਾ,    v_int16)
DEF_CVT_FUNC(8u32s, cvt_,  uchar, ਪੂਰਨ,      v_int32)
DEF_CVT_FUNC(8u32f, cvt_,  uchar, ਦਸ਼ਮਲਵ,    v_float32)
DEF_CVT_FUNC(8u64f, cvt_,  uchar, ਦੁੱਗਣਾ,   v_int32)
DEF_CVT_SCALAR_FUNC(8u64s, uchar, int64_t)
DEF_CVT_FUNC(8u16f, cvt1_, uchar, hfloat, v_float32)
DEF_CVT_FUNC(8u16bf, cvt1_, uchar, bfloat, v_float32)
DEF_CVT2BOOL_FUNC(8u8b, uchar, 0)

////////////////////// 8s -> ... ////////////////////////

DEF_CVT_FUNC(8s8u,  cvt_,  schar, uchar,    v_int16)
DEF_CVT_FUNC(8s16u, cvt_,  schar, ushort,   v_uint16)
DEF_CVT_FUNC(8s16s, cvt_,  schar, ਛੋਟਾ,    v_int16)
DEF_CVT_FUNC(8s32u, cvt_,  schar, ਬਿਨਾਨਿਸ਼ਾਨੀ, v_uint32)
DEF_CVT_FUNC(8s32s, cvt_,  schar, ਪੂਰਨ,      v_int32)
DEF_CVT_FUNC(8s32f, cvt_,  schar, ਦਸ਼ਮਲਵ,    v_float32)
DEF_CVT_FUNC(8s64f, cvt_,  schar, ਦੁੱਗਣਾ,   v_int32)
DEF_CVT_FUNC(8s64u, cvt_,  schar, uint64_t, v_uint32)
DEF_CVT_FUNC(8s64s, cvt_,  schar, int64_t,  v_int32)
DEF_CVT_FUNC(8s16f, cvt1_, schar, hfloat, v_float32)
DEF_CVT_FUNC(8s16bf, cvt1_, schar, bfloat, v_float32)

////////////////////// 8b -> ... ////////////////////////

DEF_CVTBOOL2_FUNC(8b8u,  uchar, 1)
DEF_CVTBOOL2_FUNC(8b16s, ਛੋਟਾ, 1)
DEF_CVTBOOL2_FUNC(8b32s, ਪੂਰਨ, 1)
DEF_CVTBOOL2_FUNC(8b32f, ਦਸ਼ਮਲਵ, 1)
DEF_CVTBOOL2_FUNC(8b64f, ਦੁੱਗਣਾ, 1)
DEF_CVTBOOL2_FUNC(8b64s, int64_t, 1)
DEF_CVTBOOL2_FUNC(8b16f, uint16_t, 0x3c00) // hfloat(1.0f)
DEF_CVTBOOL2_FUNC(8b16bf, uint16_t, 0x3f80) // bfloat(1.0f)

////////////////////// 16u -> ... ////////////////////////

DEF_CVT_FUNC(16u8u,  cvt_, ushort, uchar,  v_uint16)
DEF_CVT_FUNC(16u8s,  cvt_, ushort, schar,  v_uint16)
DEF_CVT_FUNC(16u16s, cvt_, ushort, ਛੋਟਾ,  v_int32)
DEF_CVT_FUNC(16u32s, cvt_, ushort, ਪੂਰਨ,    v_int32)
DEF_CVT_FUNC(16u32f, cvt_, ushort, ਦਸ਼ਮਲਵ,  v_float32)
DEF_CVT_FUNC(16u64f, cvt_, ushort, ਦੁੱਗਣਾ, v_int32)
DEF_CVT_SCALAR_FUNC(16u64s, ushort, int64_t)
DEF_CVT_FUNC(16u16f, cvt1_,ushort, hfloat, v_float32)
DEF_CVT_FUNC(16u16bf, cvt1_, ushort, bfloat, v_float32)

////////////////////// 16s -> ... ////////////////////////

DEF_CVT_FUNC(16s8u,  cvt_, ਛੋਟਾ, uchar,  v_int16)
DEF_CVT_FUNC(16s8s,  cvt_, ਛੋਟਾ, schar,  v_int16)
DEF_CVT_FUNC(16s16u, cvt_, ਛੋਟਾ, ushort, v_int32)
DEF_CVT_FUNC(16s32u, cvt_, ਛੋਟਾ, ਬਿਨਾਨਿਸ਼ਾਨੀ, v_uint32)
DEF_CVT_FUNC(16s32s, cvt_, ਛੋਟਾ, ਪੂਰਨ,    v_int32)
DEF_CVT_FUNC(16s32f, cvt_, ਛੋਟਾ, ਦਸ਼ਮਲਵ,  v_float32)
DEF_CVT_FUNC(16s64f, cvt_, ਛੋਟਾ, ਦੁੱਗਣਾ, v_int32)
DEF_CVT_FUNC(16s64u, cvt_, ਛੋਟਾ, uint64_t, v_uint32)
DEF_CVT_FUNC(16s64s, cvt_, ਛੋਟਾ, int64_t, v_int32)
DEF_CVT_FUNC(16s16f, cvt1_,ਛੋਟਾ, hfloat, v_float32)
DEF_CVT_FUNC(16s16bf, cvt1_, ਛੋਟਾ, bfloat, v_float32)
DEF_CVT2BOOL_FUNC(16s8b, ਛੋਟਾ, 0)

////////////////////// 32u -> ... ////////////////////////

DEF_CVT_FUNC(32u8u,  cvt_, ਬਿਨਾਨਿਸ਼ਾਨੀ, uchar,  v_uint32)
DEF_CVT_FUNC(32u8s,  cvt_, ਬਿਨਾਨਿਸ਼ਾਨੀ, schar,  v_int32)
DEF_CVT_FUNC(32u16u, cvt_, ਬਿਨਾਨਿਸ਼ਾਨੀ, ushort, v_uint32)
DEF_CVT_FUNC(32u16s, cvt_, ਬਿਨਾਨਿਸ਼ਾਨੀ, ਛੋਟਾ,  v_int32)
DEF_CVT_SCALAR_FUNC(32u32s, ਬਿਨਾਨਿਸ਼ਾਨੀ, ਪੂਰਨ)
DEF_CVT_FUNC(32u32f, cvt_, ਬਿਨਾਨਿਸ਼ਾਨੀ, ਦਸ਼ਮਲਵ,  v_float32)
DEF_CVT_FUNC(32u64f, cvt_64f, ਬਿਨਾਨਿਸ਼ਾਨੀ, ਦੁੱਗਣਾ, v_float64)
DEF_CVT_SCALAR_FUNC(32u64s, ਬਿਨਾਨਿਸ਼ਾਨੀ, int64_t)
DEF_CVT_SCALAR_FUNC(64u64s, uint64_t, int64_t)
DEF_CVT_FUNC(32u16f, cvt1_, ਬਿਨਾਨਿਸ਼ਾਨੀ, hfloat, v_float32)
DEF_CVT_FUNC(32u16bf, cvt1_, ਬਿਨਾਨਿਸ਼ਾਨੀ, bfloat, v_float32)

////////////////////// 32s -> ... ////////////////////////

DEF_CVT_FUNC(32s8u,  cvt_, ਪੂਰਨ, uchar,  v_int32)
DEF_CVT_FUNC(32s8s,  cvt_, ਪੂਰਨ, schar,  v_int32)
DEF_CVT_FUNC(32s16u, cvt_, ਪੂਰਨ, ushort, v_int32)
DEF_CVT_FUNC(32s16s, cvt_, ਪੂਰਨ, ਛੋਟਾ,  v_int32)
DEF_CVT_FUNC(32s32u, cvt_, ਪੂਰਨ, ਬਿਨਾਨਿਸ਼ਾਨੀ, v_uint32)
DEF_CVT_FUNC(32s32f, cvt_, ਪੂਰਨ, ਦਸ਼ਮਲਵ,  v_float32)
DEF_CVT_FUNC(32s64f, cvt_, ਪੂਰਨ, ਦੁੱਗਣਾ, v_int32)
DEF_CVT_FUNC(32s64u, cvt_, ਪੂਰਨ, uint64_t, v_uint32)
DEF_CVT_FUNC(32s64s, cvt_, ਪੂਰਨ, int64_t, v_int32)
DEF_CVT_FUNC(32s16f, cvt1_,ਪੂਰਨ, hfloat, v_float32)
DEF_CVT_FUNC(32s16bf, cvt1_, ਪੂਰਨ, bfloat, v_float32)
DEF_CVT2BOOL_FUNC(32s8b, ਪੂਰਨ, 0)

////////////////////// 32f -> ... ////////////////////////

DEF_CVT_FUNC(32f8u,  cvt_, ਦਸ਼ਮਲਵ, uchar,  v_float32)
DEF_CVT_FUNC(32f8s,  cvt_, ਦਸ਼ਮਲਵ, schar,  v_float32)
DEF_CVT_FUNC(32f16u, cvt_, ਦਸ਼ਮਲਵ, ushort, v_float32)
DEF_CVT_FUNC(32f16s, cvt_, ਦਸ਼ਮਲਵ, ਛੋਟਾ,  v_float32)
DEF_CVT_FUNC(32f32u, cvt_, ਦਸ਼ਮਲਵ, ਬਿਨਾਨਿਸ਼ਾਨੀ, v_float32)
DEF_CVT_FUNC(32f32s, cvt_, ਦਸ਼ਮਲਵ, ਪੂਰਨ,    v_float32)
DEF_CVT_FUNC(32f64f, cvt_, ਦਸ਼ਮਲਵ, ਦੁੱਗਣਾ, v_float32)
DEF_CVT_FUNC(32f64u, cvt_64f, ਦਸ਼ਮਲਵ, uint64_t, v_float64)
DEF_CVT_FUNC(32f64s, cvt_64f, ਦਸ਼ਮਲਵ, int64_t, v_float64)
DEF_CVT_FUNC(32f16f, cvt1_,ਦਸ਼ਮਲਵ, hfloat, v_float32)
DEF_CVT_FUNC(32f16bf, cvt1_,ਦਸ਼ਮਲਵ, bfloat, v_float32)
DEF_CVT2BOOL_FUNC(32f8b, ਪੂਰਨ, 1)

////////////////////// 64f -> ... ////////////////////////

DEF_CVT_FUNC(64f8u,  cvt_, ਦੁੱਗਣਾ, uchar,  v_int32)
DEF_CVT_FUNC(64f8s,  cvt_, ਦੁੱਗਣਾ, schar,  v_int32)
DEF_CVT_FUNC(64f16u, cvt_, ਦੁੱਗਣਾ, ushort, v_int32)
DEF_CVT_FUNC(64f16s, cvt_, ਦੁੱਗਣਾ, ਛੋਟਾ,  v_int32)
DEF_CVT_FUNC(64f32u, cvt_64f, ਦੁੱਗਣਾ, ਬਿਨਾਨਿਸ਼ਾਨੀ, v_float32)
DEF_CVT_FUNC(64f32s, cvt_, ਦੁੱਗਣਾ, ਪੂਰਨ,    v_int32)
DEF_CVT_FUNC(64f32f, cvt_, ਦੁੱਗਣਾ, ਦਸ਼ਮਲਵ,  v_float32)
DEF_CVT_FUNC(64f64u, cvt_64f, ਦੁੱਗਣਾ, uint64_t, v_float64)
DEF_CVT_FUNC(64f64s, cvt_64f, ਦੁੱਗਣਾ, int64_t, v_float64)
DEF_CVT_FUNC(64f16f, cvt1_,ਦੁੱਗਣਾ, hfloat, v_float32)
DEF_CVT_FUNC(64f16bf, cvt1_,ਦੁੱਗਣਾ, bfloat, v_float32)
DEF_CVT2BOOL_FUNC(64f8b, int64_t, 1)

////////////////////// 16f -> ... ////////////////////////

DEF_CVT_FUNC(16f8u,  cvt_,  hfloat, uchar,  v_float32)
DEF_CVT_FUNC(16f8s,  cvt_,  hfloat, schar,  v_float32)
DEF_CVT_FUNC(16f16u, cvt1_, hfloat, ushort, v_float32)
DEF_CVT_FUNC(16f16s, cvt1_, hfloat, ਛੋਟਾ,  v_float32)
DEF_CVT_FUNC(16f32u, cvt1_, hfloat, ਬਿਨਾਨਿਸ਼ਾਨੀ, v_float32)
DEF_CVT_FUNC(16f32s, cvt1_, hfloat, ਪੂਰਨ,    v_float32)
DEF_CVT_FUNC(16f32f, cvt1_, hfloat, ਦਸ਼ਮਲਵ,  v_float32)
DEF_CVT_FUNC(16f64f, cvt1_, hfloat, ਦੁੱਗਣਾ, v_float32)
DEF_CVT_FUNC(16f64u, cvt1_, hfloat, uint64_t, v_float32)
DEF_CVT_FUNC(16f64s, cvt1_, hfloat, int64_t, v_float32)
DEF_CVT_FUNC(16f16bf, cvt1_, hfloat, bfloat, v_float32)
DEF_CVT2BOOL_FUNC(16f8b, ਛੋਟਾ, 1)

////////////////////// 16bf -> ... ////////////////////////

DEF_CVT_FUNC(16bf8u,  cvt_,  bfloat, uchar,  v_float32)
DEF_CVT_FUNC(16bf8s,  cvt_,  bfloat, schar,  v_float32)
DEF_CVT_FUNC(16bf16u, cvt1_, bfloat, ushort, v_float32)
DEF_CVT_FUNC(16bf16s, cvt1_, bfloat, ਛੋਟਾ,  v_float32)
DEF_CVT_FUNC(16bf32u, cvt1_, bfloat, ਬਿਨਾਨਿਸ਼ਾਨੀ, v_float32)
DEF_CVT_FUNC(16bf32s, cvt1_, bfloat, ਪੂਰਨ,    v_float32)
DEF_CVT_FUNC(16bf32f, cvt1_, bfloat, ਦਸ਼ਮਲਵ,  v_float32)
DEF_CVT_FUNC(16bf64f, cvt1_, bfloat, ਦੁੱਗਣਾ, v_float32)
DEF_CVT_FUNC(16bf64u, cvt1_, bfloat, uint64_t, v_float32)
DEF_CVT_FUNC(16bf64s, cvt1_, bfloat, int64_t, v_float32)
DEF_CVT_FUNC(16bf16f, cvt1_, bfloat, hfloat, v_float32)

////////////////////// 64s -> ... ////////////////////////

DEF_CVT_FUNC(64s8u,  cvt_, int64_t, uchar,  v_int32)
DEF_CVT_FUNC(64s8s,  cvt_, int64_t, schar,  v_int32)
DEF_CVT_FUNC(64s16u, cvt_, int64_t, ushort, v_int32)
DEF_CVT_FUNC(64s16s, cvt_, int64_t, ਛੋਟਾ,  v_int32)
DEF_CVT_FUNC(64s32u, cvt_, int64_t, ਬਿਨਾਨਿਸ਼ਾਨੀ, v_uint32)
DEF_CVT_FUNC(64s32s, cvt_, int64_t, ਪੂਰਨ,    v_int32)
DEF_CVT_FUNC(64s32f, cvt_64f, int64_t, ਦਸ਼ਮਲਵ,  v_float32)
DEF_CVT_FUNC(64s64f, cvt_64f, int64_t, ਦੁੱਗਣਾ,  v_float64)
DEF_CVT_FUNC(64s64u, cvt_, int64_t, uint64_t, v_uint64)
DEF_CVT_FUNC(64s16f, cvt1_,int64_t, hfloat, v_float32)
DEF_CVT_FUNC(64s16bf, cvt1_, int64_t, bfloat, v_float32)
DEF_CVT2BOOL_FUNC(64s8b, int64_t, 0)

////////////////////// 64u -> ... ////////////////////////

DEF_CVT_FUNC(64u8u,  cvt_, uint64_t, uchar,  v_int32)
DEF_CVT_FUNC(64u8s,  cvt_, uint64_t, schar,  v_int32)
DEF_CVT_FUNC(64u16u, cvt_, uint64_t, ushort, v_int32)
DEF_CVT_FUNC(64u16s, cvt_, uint64_t, ਛੋਟਾ,  v_int32)
DEF_CVT_FUNC(64u32u, cvt_, uint64_t, ਬਿਨਾਨਿਸ਼ਾਨੀ, v_uint32)
DEF_CVT_FUNC(64u32s, cvt_, uint64_t, ਪੂਰਨ,   v_int32)
DEF_CVT_FUNC(64u32f, cvt_64f, uint64_t, ਦਸ਼ਮਲਵ,  v_float64)
DEF_CVT_FUNC(64u64f, cvt_64f, uint64_t, ਦੁੱਗਣਾ,  v_float64)
DEF_CVT_FUNC(64u16f, cvt1_,uint64_t, hfloat, v_float32)
DEF_CVT_FUNC(64u16bf, cvt1_, uint64_t, bfloat, v_float32)

///////////// "conversion" w/o conversion ///////////////

ਸਥਿਰ ਖਾਲੀ cvt8u(ਅਚਲ uchar* src, size_t sstep, ਅਚਲ uchar*, size_t, uchar* dst, size_t dstep, ਮਾਪ size, ਖਾਲੀ*)
{ CV_INSTRUMENT_REGION(); cvtCopy(src, sstep, dst, dstep, size, 1); }

ਸਥਿਰ ਖਾਲੀ cvt16u(ਅਚਲ uchar* src, size_t sstep, ਅਚਲ uchar*, size_t, uchar* dst, size_t dstep, ਮਾਪ size, ਖਾਲੀ*)
{ CV_INSTRUMENT_REGION(); cvtCopy((ਅਚਲ uchar*)src, sstep, (uchar*)dst, dstep, size, 2); }

ਸਥਿਰ ਖਾਲੀ cvt32s(ਅਚਲ uchar* src, size_t sstep, ਅਚਲ uchar*, size_t, uchar* dst, size_t dstep, ਮਾਪ size, ਖਾਲੀ*)
{ CV_INSTRUMENT_REGION(); cvtCopy((ਅਚਲ uchar*)src, sstep, (uchar*)dst, dstep, size, 4); }

ਸਥਿਰ ਖਾਲੀ cvt64s(ਅਚਲ uchar* src, size_t sstep, ਅਚਲ uchar*, size_t, uchar* dst, size_t dstep, ਮਾਪ size, ਖਾਲੀ*)
{ CV_INSTRUMENT_REGION(); cvtCopy((ਅਚਲ uchar*)src, sstep, (uchar*)dst, dstep, size, 8); }

//////////////////// FP8 (1-byte float) conversions — scalar via saturate_cast ////////////////////
// Suffixes: 8fe4m3 (E4M3FN), 8fe4m3u (E4M3FNUZ).
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ DEF_CVT_FP8(S, T) \
    DEF_CVT_SCALAR_FUNC(S##8u,   T, uchar)     DEF_CVT_SCALAR_FUNC(8u##S,   uchar, T) \
    DEF_CVT_SCALAR_FUNC(S##8s,   T, schar)     DEF_CVT_SCALAR_FUNC(8s##S,   schar, T) \
    DEF_CVT_SCALAR_FUNC(S##16u,  T, ushort)    DEF_CVT_SCALAR_FUNC(16u##S,  ushort, T) \
    DEF_CVT_SCALAR_FUNC(S##16s,  T, ਛੋਟਾ)     DEF_CVT_SCALAR_FUNC(16s##S,  ਛੋਟਾ, T) \
    DEF_CVT_SCALAR_FUNC(S##32u,  T, ਬਿਨਾਨਿਸ਼ਾਨੀ)  DEF_CVT_SCALAR_FUNC(32u##S,  ਬਿਨਾਨਿਸ਼ਾਨੀ, T) \
    DEF_CVT_SCALAR_FUNC(S##32s,  T, ਪੂਰਨ)       DEF_CVT_SCALAR_FUNC(32s##S,  ਪੂਰਨ, T) \
    DEF_CVT_SCALAR_FUNC(S##64f,  T, ਦੁੱਗਣਾ)    DEF_CVT_SCALAR_FUNC(64f##S,  ਦੁੱਗਣਾ, T) \
    DEF_CVT_SCALAR_FUNC(S##16f,  T, hfloat)    DEF_CVT_SCALAR_FUNC(16f##S,  hfloat, T) \
    DEF_CVT_SCALAR_FUNC(S##16bf, T, bfloat)    DEF_CVT_SCALAR_FUNC(16bf##S, bfloat, T) \
    DEF_CVT_SCALAR_FUNC(S##64u,  T, uint64_t)  DEF_CVT_SCALAR_FUNC(64u##S,  uint64_t, T) \
    DEF_CVT_SCALAR_FUNC(S##64s,  T, int64_t)   DEF_CVT_SCALAR_FUNC(64s##S,  int64_t, T) \
    DEF_CVT_SCALAR_FUNC(S##8b,   T, ਤਾਰਕਿਕ)

DEF_CVT_FP8(8fe4m3,  fp8_t)
DEF_CVT_FP8(8fe4m3u, fp8a_t)

// FP8 -> FP8, cross-format only (identity uses the 1-byte copy cvt8u, like 16f->16f uses cvt16u)
DEF_CVT_SCALAR_FUNC(8fe4m38fe4m3u, fp8_t,   fp8a_t)
DEF_CVT_SCALAR_FUNC(8fe4m3u8fe4m3, fp8a_t, fp8_t)

// float32 -> FP8 encode via the shared building blocks in convert.hpp.
ਸਾਂਚਾ<ਪੂਰਨ bias, ਤਾਰਕਿਕ fnuz> ਸਥਿਰ ਖਾਲੀ
cvt_F32_ਮੰਜ਼ਿਲ_Fp8(ਅਚਲ uchar* src_, size_t sstep, ਅਚਲ uchar*, size_t,
            uchar* dst, size_t dstep, ਮਾਪ size, ਖਾਲੀ*)
{
    CV_INSTRUMENT_REGION();
    ਅਚਲ ਦਸ਼ਮਲਵ* src = (ਅਚਲ ਦਸ਼ਮਲਵ*)src_;
    sstep /= ਆਕਾਰ(src[0]);

    ਲਈ (ਪੂਰਨ i = 0; i < size.ਉਚਾਈ; i++, src += sstep, dst += dstep)
    {
        ਪੂਰਨ j = 0;
#ਜੇ (CV_SIMD || CV_SIMD_SCALABLE)
        ਅਚਲ ਪੂਰਨ LANES32 = VTraits<v_int32>::vlanes();
        ਅਚਲ ਪੂਰਨ VECSZ = LANES32 * 4;
        ਲਈ (; j <= size.ਚੌੜਾਈ - VECSZ; j += VECSZ)
        {
            v_float32 vf0 = vx_ਲੱਦੋ(src + j);
            v_float32 vf1 = vx_ਲੱਦੋ(src + j + LANES32);
            v_float32 vf2 = vx_ਲੱਦੋ(src + j + 2 * LANES32);
            v_float32 vf3 = vx_ਲੱਦੋ(src + j + 3 * LANES32);
            encodeFp8Vec4<bias, fnuz>(vf0, vf1, vf2, vf3, src + j, VECSZ, dst + j);
        }
        vx_cleanup();
#ਜੇ_ਅੰਤ
        ਲਈ (; j < size.ਚੌੜਾਈ; j++)
            dst[j] = fp8_detail::encodeE4M3(src[j], bias, fnuz);
    }
}
ਸਥਿਰ ਖਾਲੀ cvt32f8fe4m3(ਅਚਲ uchar* s, size_t ss, ਅਚਲ uchar* p, size_t ps, uchar* d, size_t ds, ਮਾਪ sz, ਖਾਲੀ* x)
{ cvt_F32_ਮੰਜ਼ਿਲ_Fp8<7, ਝੂਠ>(s, ss, p, ps, d, ds, sz, x); }
ਸਥਿਰ ਖਾਲੀ cvt32f8fe4m3u(ਅਚਲ uchar* s, size_t ss, ਅਚਲ uchar* p, size_t ps, uchar* d, size_t ds, ਮਾਪ sz, ਖਾਲੀ* x)
{ cvt_F32_ਮੰਜ਼ਿਲ_Fp8<8, ਸੱਚ>(s, ss, p, ps, d, ds, sz, x); }

// FP8 -> float32: 256-entry decode table gathered via universal intrinsics (same table as scalar)
ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ FP8>
ਸਥਿਰ ਖਾਲੀ cvt_Fp8_ਮੰਜ਼ਿਲ_F32(ਅਚਲ uchar* src, size_t sstep, ਅਚਲ uchar*, size_t,
                        uchar* dst_, size_t dstep, ਮਾਪ size, ਖਾਲੀ*)
{
    CV_INSTRUMENT_REGION();
    ਦਸ਼ਮਲਵ* dst = (ਦਸ਼ਮਲਵ*)dst_;
    ਅਚਲ ਦਸ਼ਮਲਵ* tab = FP8::decodeLUT();
    dstep /= ਆਕਾਰ(dst[0]);
    ਲਈ (ਪੂਰਨ i = 0; i < size.ਉਚਾਈ; i++, src += sstep, dst += dstep)
    {
        ਪੂਰਨ j = 0;
#ਜੇ (CV_SIMD || CV_SIMD_SCALABLE)
        ਅਚਲ ਪੂਰਨ VECSZ = VTraits<v_float32>::vlanes();
        ਲਈ (; j <= size.ਚੌੜਾਈ - VECSZ; j += VECSZ)
            v_store(dst + j, v_lut(tab, v_reinterpret_as_s32(vx_ਲੱਦੋ_expand_q(src + j))));
#ਜੇ_ਅੰਤ
        ਲਈ (; j < size.ਚੌੜਾਈ; j++) dst[j] = tab[src[j]];
    }
}
ਸਥਿਰ ਖਾਲੀ cvt8fe4m332f(ਅਚਲ uchar* s, size_t ss, ਅਚਲ uchar* p, size_t ps, uchar* d, size_t ds, ਮਾਪ sz, ਖਾਲੀ* x)
{ cvt_Fp8_ਮੰਜ਼ਿਲ_F32<fp8_t>(s, ss, p, ps, d, ds, sz, x); }
ਸਥਿਰ ਖਾਲੀ cvt8fe4m3u32f(ਅਚਲ uchar* s, size_t ss, ਅਚਲ uchar* p, size_t ps, uchar* d, size_t ds, ਮਾਪ sz, ਖਾਲੀ* x)
{ cvt_Fp8_ਮੰਜ਼ਿਲ_F32<fp8a_t>(s, ss, p, ps, d, ds, sz, x); }

ਦੁਆਧਾਰੀ_Func getConvertFunc(ਪੂਰਨ sdepth_, ਪੂਰਨ ddepth_)
{
    ਪੂਰਨ sdepth = CV_MAT_ਡੂੰਘਾਈ(sdepth_);
    ਪੂਰਨ ddepth = CV_MAT_ਡੂੰਘਾਈ(ddepth_);
    ਦੁਆਧਾਰੀ_Func func =
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
    ਵਾਪਸ func;
}

CV_CPU_OPTIMIZATION_NAMESPACE_ਸਿਰਾ
#ਜੇ_ਅੰਤ
} // namespace
