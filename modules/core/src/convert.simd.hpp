// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html

#ଅନ୍ତର୍ଭାଵ "precomp.hpp"
#ଅନ୍ତର୍ଭାଵ "convert.hpp"

#ଯଦି !defined(OPENCV_SUPRESS_WARNING_AVX2_WITHOUT_FP16C) && \
    (defined(__GNUC__) && defined(__AVX2__) && !defined(__F16C__))
#warning "Non-optimal compiler flags: AVX2 without FP16. Generated code is very slow. Consider adding '-mf16c' compiler option."
#endif

ନାମାକାଶ cv {
ନାମାକାଶ hal {
CV_CPU_OPTIMIZATION_NAMESPACE_BEGIN

ଶୂନ୍ୟ cvt16f32f(ଅଚଳ hfloat* src, ଦଶମିକ* dst, ପୂର୍ଣ୍ଣସଂଖ୍ୟା len);
ଶୂନ୍ୟ cvt32f16f(ଅଚଳ ଦଶମିକ* src, hfloat* dst, ପୂର୍ଣ୍ଣସଂଖ୍ୟା len);
ଶୂନ୍ୟ cvt16bf32f(ଅଚଳ bfloat* src, ଦଶମିକ* dst, ପୂର୍ଣ୍ଣସଂଖ୍ୟା len);
ଶୂନ୍ୟ cvt32f16bf(ଅଚଳ ଦଶମିକ* src, bfloat* dst, ପୂର୍ଣ୍ଣସଂଖ୍ୟା len);
ଶୂନ୍ୟ addRNGBias32f(ଦଶମିକ* arr, ଅଚଳ ଦଶମିକ* scaleBiasPairs, ପୂର୍ଣ୍ଣସଂଖ୍ୟା len, ପୂର୍ଣ୍ଣସଂଖ୍ୟା cn);
ଶୂନ୍ୟ addRNGBias64f(ଦ୍ୱିଗୁଣ* arr, ଅଚଳ ଦ୍ୱିଗୁଣ* scaleBiasPairs, ପୂର୍ଣ୍ଣସଂଖ୍ୟା len, ପୂର୍ଣ୍ଣସଂଖ୍ୟା cn);

CV_CPU_OPTIMIZATION_NAMESPACE_END
} // namespace cv::hal

CV_CPU_OPTIMIZATION_NAMESPACE_BEGIN

ଦ୍ୱିମିକ_Func getConvertFunc(ପୂର୍ଣ୍ଣସଂଖ୍ୟା sdepth, ପୂର୍ଣ୍ଣସଂଖ୍ୟା ddepth);

CV_CPU_OPTIMIZATION_NAMESPACE_END

#ifndef CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

ନାମାକାଶ hal {
CV_CPU_OPTIMIZATION_NAMESPACE_BEGIN

ଦ୍ୱିମିକ_Func getConvertFunc(ପୂର୍ଣ୍ଣସଂଖ୍ୟା sdepth, ପୂର୍ଣ୍ଣସଂଖ୍ୟା ddepth);

ଶୂନ୍ୟ cvt16f32f( ଅଚଳ hfloat* src, ଦଶମିକ* dst, ପୂର୍ଣ୍ଣସଂଖ୍ୟା len )
{
    CV_INSTRUMENT_REGION();
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା j = 0;
#ଯଦି (CV_SIMD || CV_SIMD_SCALABLE)
    ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା VECSZ = VTraits<v_float32>::vlanes();
    ପାଇଁ( ; j < len; j += VECSZ )
    {
        ଯଦି( j > len - VECSZ )
        {
            ଯଦି( j == 0 )
                ଭାଙ୍ଗ;
            j = len - VECSZ;
        }
        v_store(dst + j, vx_load_expand(src + j));
    }
#endif
    ପାଇଁ( ; j < len; j++ )
        dst[j] = (ଦଶମିକ)src[j];
}

ଶୂନ୍ୟ cvt32f16f( ଅଚଳ ଦଶମିକ* src, hfloat* dst, ପୂର୍ଣ୍ଣସଂଖ୍ୟା len )
{
    CV_INSTRUMENT_REGION();
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା j = 0;
#ଯଦି (CV_SIMD || CV_SIMD_SCALABLE)
    ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା VECSZ = VTraits<v_float32>::vlanes();
    ପାଇଁ( ; j < len; j += VECSZ )
    {
        ଯଦି( j > len - VECSZ )
        {
            ଯଦି( j == 0 )
                ଭାଙ୍ଗ;
            j = len - VECSZ;
        }
        v_pack_store(dst + j, vx_load(src + j));
    }
#endif
    ପାଇଁ( ; j < len; j++ )
        dst[j] = hfloat(src[j]);
}

ଶୂନ୍ୟ cvt32f16bf( ଅଚଳ ଦଶମିକ* src, bfloat* dst, ପୂର୍ଣ୍ଣସଂଖ୍ୟା len )
{
    CV_INSTRUMENT_REGION();
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା j = 0;
#ଯଦି (CV_SIMD || CV_SIMD_SCALABLE)
    ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା VECSZ = VTraits<v_float32>::vlanes();
    ପାଇଁ( ; j < len; j += VECSZ )
    {
        ଯଦି( j > len - VECSZ )
        {
            ଯଦି( j == 0 )
                ଭାଙ୍ଗ;
            j = len - VECSZ;
        }
        v_pack_store(dst + j, vx_load(src + j));
    }
#endif
    ପାଇଁ( ; j < len; j++ )
        dst[j] = bfloat(src[j]);
}

ଶୂନ୍ୟ addRNGBias32f( ଦଶମିକ* arr, ଅଚଳ ଦଶମିକ* scaleBiasPairs, ପୂର୍ଣ୍ଣସଂଖ୍ୟା len, ପୂର୍ଣ୍ଣସଂଖ୍ୟା cn )
{
    CV_INSTRUMENT_REGION();
    ଯଦି (cn == 1) {
        ଦଶମିକ bias = scaleBiasPairs[1];
        ପାଇଁ( ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < len; i++ ) {
            arr[i] += bias;
        }
    } ଅନ୍ୟଥା {
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା k = 0;
        len *= cn;
        cn--;
        ପାଇଁ( ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < len; i++ ) {
            arr[i] += scaleBiasPairs[k*2 + 1];
            k = (k + 1) & ((k >= cn) - 1);
        }
    }
}

ଶୂନ୍ୟ addRNGBias64f( ଦ୍ୱିଗୁଣ* arr, ଅଚଳ ଦ୍ୱିଗୁଣ* scaleBiasPairs, ପୂର୍ଣ୍ଣସଂଖ୍ୟା len, ପୂର୍ଣ୍ଣସଂଖ୍ୟା cn )
{
    CV_INSTRUMENT_REGION();
    ଯଦି (cn == 1) {
        ଦ୍ୱିଗୁଣ bias = scaleBiasPairs[1];
        ପାଇଁ( ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < len; i++ ) {
            arr[i] += bias;
        }
    } ଅନ୍ୟଥା {
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା k = 0;
        len *= cn;
        cn--;
        ପାଇଁ( ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < len; i++ ) {
            arr[i] += scaleBiasPairs[k*2 + 1];
            k = (k + 1) & ((k >= cn) - 1);
        }
    }
}

CV_CPU_OPTIMIZATION_NAMESPACE_END
} // namespace cv::hal

// cv::
CV_CPU_OPTIMIZATION_NAMESPACE_BEGIN

template<typename _Ts, typename _Td, typename _Twvec> ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ
cvt_( ଅଚଳ _Ts* src, size_t sstep, _Td* dst, size_t dstep, Size size )
{
    sstep /= ଆକାର(src[0]);
    dstep /= ଆକାର(dst[0]);

    ପାଇଁ( ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < size.height; i++, src += sstep, dst += dstep )
    {
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା j = 0;
// Excluding GNU in CV_SIMD_SCALABLE because of "opencv/issues/26936"
#ଯଦି (CV_SIMD || (CV_SIMD_SCALABLE && !(defined(__GNUC__) && !defined(__clang__))) )
        ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା VECSZ = VTraits<_Twvec>::vlanes()*2;
        ପାଇଁ( ; j < size.width; j += VECSZ )
        {
            ଯଦି( j > size.width - VECSZ )
            {
                ଯଦି( j == 0 || src == (_Ts*)dst )
                    ଭାଙ୍ଗ;
                j = size.width - VECSZ;
            }
            _Twvec v0, v1;
            vx_load_pair_as(src + j, v0, v1);
            v_store_pair_as(dst + j, v0, v1);
        }
#endif
        ପାଇଁ( ; j < size.width; j++ )
            dst[j] = saturate_cast<_Td>(src[j]);
    }
}

template<typename _Ts, typename _Td, typename dummy> ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ
cvt_64f( ଅଚଳ _Ts* src, size_t sstep, _Td* dst, size_t dstep, Size size )
{
    sstep /= ଆକାର(src[0]);
    dstep /= ଆକାର(dst[0]);

    ପାଇଁ( ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < size.height; i++, src += sstep, dst += dstep )
    {
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା j = 0;
#ଯଦି (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)
        ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା VECSZ = VTraits<v_float64>::vlanes()*2;
        ପାଇଁ( ; j < size.width; j += VECSZ )
        {
            ଯଦି( j > size.width - VECSZ )
            {
                ଯଦି( j == 0 || src == (_Ts*)dst )
                    ଭାଙ୍ଗ;
                j = size.width - VECSZ;
            }
            v_float64 v0, v1;
            vx_load_pair_as(src + j, v0, v1);
            v_store_pair_as(dst + j, v0, v1);
        }
#endif
        ପାଇଁ( ; j < size.width; j++ )
            dst[j] = saturate_cast<_Td>(src[j]);
    }
}

// in order to reduce the code size, for (16f <-> ...) conversions
// we add a conversion function without loop unrolling
template<typename _Ts, typename _Td, typename _Twvec> ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ
cvt1_( ଅଚଳ _Ts* src, size_t sstep, _Td* dst, size_t dstep, Size size )
{
    sstep /= ଆକାର(src[0]);
    dstep /= ଆକାର(dst[0]);

    ପାଇଁ( ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < size.height; i++, src += sstep, dst += dstep )
    {
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା j = 0;
#ଯଦି (CV_SIMD || CV_SIMD_SCALABLE)
        ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା VECSZ = VTraits<_Twvec>::vlanes();
        ପାଇଁ( ; j < size.width; j += VECSZ )
        {
            ଯଦି( j > size.width - VECSZ )
            {
                ଯଦି( j == 0 || src == (_Ts*)dst )
                    ଭାଙ୍ଗ;
                j = size.width - VECSZ;
            }
            _Twvec v;
            vx_load_as(src + j, v);
            v_store_as(dst + j, v);
        }
        vx_cleanup();
#endif
        ପାଇଁ( ; j < size.width; j++ )
            dst[j] = saturate_cast<_Td>(src[j]);
    }
}

ସ୍ଥିର ଶୂନ୍ୟ cvtCopy( ଅଚଳ uchar* src, size_t sstep,
                     uchar* dst, size_t dstep, Size size, size_t elemsize)
{
    size_t len = size.width*elemsize;
    ପାଇଁ( ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < size.height; i++, src += sstep, dst += dstep )
    {
        memcpy( dst, src, len );
    }
}

#define DEF_CVT_FUNC(suffix, cvtfunc, _Ts, _Td, _Twvec) \
ସ୍ଥିର ଶୂନ୍ୟ cvt##suffix(ଅଚଳ uchar* src_, size_t sstep, ଅଚଳ uchar*, size_t, \
                        uchar* dst_, size_t dstep, Size size, ଶୂନ୍ୟ*) \
{ \
    CV_INSTRUMENT_REGION(); \
    ଅଚଳ _Ts* src = (ଅଚଳ _Ts*)src_; \
    _Td* dst = (_Td*)dst_; \
    cvtfunc<_Ts, _Td, _Twvec>(src, sstep, dst, dstep, size); \
}

#define DEF_CVT2BOOL_FUNC(suffix, _Ts, shift) \
ସ୍ଥିର ଶୂନ୍ୟ cvt##suffix(ଅଚଳ uchar* src_, size_t sstep, ଅଚଳ uchar*, size_t, \
                        uchar* dst, size_t dstep, Size size, ଶୂନ୍ୟ*) \
{ \
    CV_INSTRUMENT_REGION(); \
    ଅଚଳ _Ts* src = (ଅଚଳ _Ts*)src_; \
    sstep /= ଆକାର(src[0]); \
    \
    ପାଇଁ( ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < size.height; i++, src += sstep, dst += dstep ) { \
        ପାଇଁ ( ପୂର୍ଣ୍ଣସଂଖ୍ୟା j = 0; j < size.width; j++ ) \
            dst[j] = (src[j]<<shift) != 0; \
    } \
}

#define DEF_CVTBOOL2_FUNC(suffix, _Td, scale) \
ସ୍ଥିର ଶୂନ୍ୟ cvt##suffix(ଅଚଳ uchar* src, size_t sstep, ଅଚଳ uchar*, size_t, \
                        uchar* dst_, size_t dstep, Size size, ଶୂନ୍ୟ*) \
{ \
    CV_INSTRUMENT_REGION(); \
    _Td* dst = (_Td*)dst_; \
    dstep /= ଆକାର(dst[0]); \
    \
    ପାଇଁ( ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < size.height; i++, src += sstep, dst += dstep ) { \
        ପାଇଁ ( ପୂର୍ଣ୍ଣସଂଖ୍ୟା j = 0; j < size.width; j++ ) \
            dst[j] = (_Td)((src[j] != 0)*scale); \
    } \
}

#define DEF_CVT_SCALAR_FUNC(suffix, _Ts, _Td) \
ସ୍ଥିର ଶୂନ୍ୟ cvt##suffix(ଅଚଳ uchar* src_, size_t sstep, ଅଚଳ uchar*, size_t, \
                        uchar* dst_, size_t dstep, Size size, ଶୂନ୍ୟ*) \
{ \
    CV_INSTRUMENT_REGION(); \
    ଅଚଳ _Ts* src = (ଅଚଳ _Ts*)src_; \
    _Td* dst = (_Td*)dst_; \
    sstep /= ଆକାର(src[0]); \
    dstep /= ଆକାର(dst[0]); \
    \
    ପାଇଁ( ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < size.height; i++, src += sstep, dst += dstep ) { \
        ପାଇଁ ( ପୂର୍ଣ୍ଣସଂଖ୍ୟା j = 0; j < size.width; j++ ) \
            dst[j] = saturate_cast<_Td>(src[j]); \
    } \
}

#define DEF_CVT_SCALAR_FUNC_S2U(suffix, _Ts, _Td, _Tw) \
ସ୍ଥିର ଶୂନ୍ୟ cvt##suffix(ଅଚଳ uchar* src_, size_t sstep, ଅଚଳ uchar*, size_t, \
                        uchar* dst_, size_t dstep, Size size, ଶୂନ୍ୟ*) \
{ \
    CV_INSTRUMENT_REGION(); \
    ଅଚଳ _Ts* src = (ଅଚଳ _Ts*)src_; \
    _Td* dst = (_Td*)dst_; \
    sstep /= ଆକାର(src[0]); \
    dstep /= ଆକାର(dst[0]); \
    \
    ପାଇଁ( ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < size.height; i++, src += sstep, dst += dstep ) { \
        ପାଇଁ ( ପୂର୍ଣ୍ଣସଂଖ୍ୟା j = 0; j < size.width; j++ ) \
            dst[j] = saturate_cast<_Td>(std::max((_Tw)src[j], (_Tw)0)); \
    } \
}

////////////////////// 8u -> ... ////////////////////////

DEF_CVT_FUNC(8u8s,  cvt_,  uchar, schar,    v_int16)
DEF_CVT_FUNC(8u16s, cvt_,  uchar, short,    v_int16)
DEF_CVT_FUNC(8u32s, cvt_,  uchar, ପୂର୍ଣ୍ଣସଂଖ୍ୟା,      v_int32)
DEF_CVT_FUNC(8u32f, cvt_,  uchar, ଦଶମିକ,    v_float32)
DEF_CVT_FUNC(8u64f, cvt_,  uchar, ଦ୍ୱିଗୁଣ,   v_int32)
DEF_CVT_SCALAR_FUNC(8u64s, uchar, int64_t)
DEF_CVT_FUNC(8u16f, cvt1_, uchar, hfloat, v_float32)
DEF_CVT_FUNC(8u16bf, cvt1_, uchar, bfloat, v_float32)
DEF_CVT2BOOL_FUNC(8u8b, uchar, 0)

////////////////////// 8s -> ... ////////////////////////

DEF_CVT_FUNC(8s8u,  cvt_,  schar, uchar,    v_int16)
DEF_CVT_FUNC(8s16u, cvt_,  schar, ushort,   v_uint16)
DEF_CVT_FUNC(8s16s, cvt_,  schar, short,    v_int16)
DEF_CVT_FUNC(8s32u, cvt_,  schar, unsigned, v_uint32)
DEF_CVT_FUNC(8s32s, cvt_,  schar, ପୂର୍ଣ୍ଣସଂଖ୍ୟା,      v_int32)
DEF_CVT_FUNC(8s32f, cvt_,  schar, ଦଶମିକ,    v_float32)
DEF_CVT_FUNC(8s64f, cvt_,  schar, ଦ୍ୱିଗୁଣ,   v_int32)
DEF_CVT_FUNC(8s64u, cvt_,  schar, uint64_t, v_uint32)
DEF_CVT_FUNC(8s64s, cvt_,  schar, int64_t,  v_int32)
DEF_CVT_FUNC(8s16f, cvt1_, schar, hfloat, v_float32)
DEF_CVT_FUNC(8s16bf, cvt1_, schar, bfloat, v_float32)

////////////////////// 8b -> ... ////////////////////////

DEF_CVTBOOL2_FUNC(8b8u,  uchar, 1)
DEF_CVTBOOL2_FUNC(8b16s, short, 1)
DEF_CVTBOOL2_FUNC(8b32s, ପୂର୍ଣ୍ଣସଂଖ୍ୟା, 1)
DEF_CVTBOOL2_FUNC(8b32f, ଦଶମିକ, 1)
DEF_CVTBOOL2_FUNC(8b64f, ଦ୍ୱିଗୁଣ, 1)
DEF_CVTBOOL2_FUNC(8b64s, int64_t, 1)
DEF_CVTBOOL2_FUNC(8b16f, uint16_t, 0x3c00) // hfloat(1.0f)
DEF_CVTBOOL2_FUNC(8b16bf, uint16_t, 0x3f80) // bfloat(1.0f)

////////////////////// 16u -> ... ////////////////////////

DEF_CVT_FUNC(16u8u,  cvt_, ushort, uchar,  v_uint16)
DEF_CVT_FUNC(16u8s,  cvt_, ushort, schar,  v_uint16)
DEF_CVT_FUNC(16u16s, cvt_, ushort, short,  v_int32)
DEF_CVT_FUNC(16u32s, cvt_, ushort, ପୂର୍ଣ୍ଣସଂଖ୍ୟା,    v_int32)
DEF_CVT_FUNC(16u32f, cvt_, ushort, ଦଶମିକ,  v_float32)
DEF_CVT_FUNC(16u64f, cvt_, ushort, ଦ୍ୱିଗୁଣ, v_int32)
DEF_CVT_SCALAR_FUNC(16u64s, ushort, int64_t)
DEF_CVT_FUNC(16u16f, cvt1_,ushort, hfloat, v_float32)
DEF_CVT_FUNC(16u16bf, cvt1_, ushort, bfloat, v_float32)

////////////////////// 16s -> ... ////////////////////////

DEF_CVT_FUNC(16s8u,  cvt_, short, uchar,  v_int16)
DEF_CVT_FUNC(16s8s,  cvt_, short, schar,  v_int16)
DEF_CVT_FUNC(16s16u, cvt_, short, ushort, v_int32)
DEF_CVT_FUNC(16s32u, cvt_, short, unsigned, v_uint32)
DEF_CVT_FUNC(16s32s, cvt_, short, ପୂର୍ଣ୍ଣସଂଖ୍ୟା,    v_int32)
DEF_CVT_FUNC(16s32f, cvt_, short, ଦଶମିକ,  v_float32)
DEF_CVT_FUNC(16s64f, cvt_, short, ଦ୍ୱିଗୁଣ, v_int32)
DEF_CVT_FUNC(16s64u, cvt_, short, uint64_t, v_uint32)
DEF_CVT_FUNC(16s64s, cvt_, short, int64_t, v_int32)
DEF_CVT_FUNC(16s16f, cvt1_,short, hfloat, v_float32)
DEF_CVT_FUNC(16s16bf, cvt1_, short, bfloat, v_float32)
DEF_CVT2BOOL_FUNC(16s8b, short, 0)

////////////////////// 32u -> ... ////////////////////////

DEF_CVT_FUNC(32u8u,  cvt_, unsigned, uchar,  v_uint32)
DEF_CVT_FUNC(32u8s,  cvt_, unsigned, schar,  v_int32)
DEF_CVT_FUNC(32u16u, cvt_, unsigned, ushort, v_uint32)
DEF_CVT_FUNC(32u16s, cvt_, unsigned, short,  v_int32)
DEF_CVT_SCALAR_FUNC(32u32s, unsigned, ପୂର୍ଣ୍ଣସଂଖ୍ୟା)
DEF_CVT_FUNC(32u32f, cvt_, unsigned, ଦଶମିକ,  v_float32)
DEF_CVT_FUNC(32u64f, cvt_64f, unsigned, ଦ୍ୱିଗୁଣ, v_float64)
DEF_CVT_SCALAR_FUNC(32u64s, unsigned, int64_t)
DEF_CVT_SCALAR_FUNC(64u64s, uint64_t, int64_t)
DEF_CVT_FUNC(32u16f, cvt1_, unsigned, hfloat, v_float32)
DEF_CVT_FUNC(32u16bf, cvt1_, unsigned, bfloat, v_float32)

////////////////////// 32s -> ... ////////////////////////

DEF_CVT_FUNC(32s8u,  cvt_, ପୂର୍ଣ୍ଣସଂଖ୍ୟା, uchar,  v_int32)
DEF_CVT_FUNC(32s8s,  cvt_, ପୂର୍ଣ୍ଣସଂଖ୍ୟା, schar,  v_int32)
DEF_CVT_FUNC(32s16u, cvt_, ପୂର୍ଣ୍ଣସଂଖ୍ୟା, ushort, v_int32)
DEF_CVT_FUNC(32s16s, cvt_, ପୂର୍ଣ୍ଣସଂଖ୍ୟା, short,  v_int32)
DEF_CVT_FUNC(32s32u, cvt_, ପୂର୍ଣ୍ଣସଂଖ୍ୟା, unsigned, v_uint32)
DEF_CVT_FUNC(32s32f, cvt_, ପୂର୍ଣ୍ଣସଂଖ୍ୟା, ଦଶମିକ,  v_float32)
DEF_CVT_FUNC(32s64f, cvt_, ପୂର୍ଣ୍ଣସଂଖ୍ୟା, ଦ୍ୱିଗୁଣ, v_int32)
DEF_CVT_FUNC(32s64u, cvt_, ପୂର୍ଣ୍ଣସଂଖ୍ୟା, uint64_t, v_uint32)
DEF_CVT_FUNC(32s64s, cvt_, ପୂର୍ଣ୍ଣସଂଖ୍ୟା, int64_t, v_int32)
DEF_CVT_FUNC(32s16f, cvt1_,ପୂର୍ଣ୍ଣସଂଖ୍ୟା, hfloat, v_float32)
DEF_CVT_FUNC(32s16bf, cvt1_, ପୂର୍ଣ୍ଣସଂଖ୍ୟା, bfloat, v_float32)
DEF_CVT2BOOL_FUNC(32s8b, ପୂର୍ଣ୍ଣସଂଖ୍ୟା, 0)

////////////////////// 32f -> ... ////////////////////////

DEF_CVT_FUNC(32f8u,  cvt_, ଦଶମିକ, uchar,  v_float32)
DEF_CVT_FUNC(32f8s,  cvt_, ଦଶମିକ, schar,  v_float32)
DEF_CVT_FUNC(32f16u, cvt_, ଦଶମିକ, ushort, v_float32)
DEF_CVT_FUNC(32f16s, cvt_, ଦଶମିକ, short,  v_float32)
DEF_CVT_FUNC(32f32u, cvt_, ଦଶମିକ, unsigned, v_float32)
DEF_CVT_FUNC(32f32s, cvt_, ଦଶମିକ, ପୂର୍ଣ୍ଣସଂଖ୍ୟା,    v_float32)
DEF_CVT_FUNC(32f64f, cvt_, ଦଶମିକ, ଦ୍ୱିଗୁଣ, v_float32)
DEF_CVT_FUNC(32f64u, cvt_64f, ଦଶମିକ, uint64_t, v_float64)
DEF_CVT_FUNC(32f64s, cvt_64f, ଦଶମିକ, int64_t, v_float64)
DEF_CVT_FUNC(32f16f, cvt1_,ଦଶମିକ, hfloat, v_float32)
DEF_CVT_FUNC(32f16bf, cvt1_,ଦଶମିକ, bfloat, v_float32)
DEF_CVT2BOOL_FUNC(32f8b, ପୂର୍ଣ୍ଣସଂଖ୍ୟା, 1)

////////////////////// 64f -> ... ////////////////////////

DEF_CVT_FUNC(64f8u,  cvt_, ଦ୍ୱିଗୁଣ, uchar,  v_int32)
DEF_CVT_FUNC(64f8s,  cvt_, ଦ୍ୱିଗୁଣ, schar,  v_int32)
DEF_CVT_FUNC(64f16u, cvt_, ଦ୍ୱିଗୁଣ, ushort, v_int32)
DEF_CVT_FUNC(64f16s, cvt_, ଦ୍ୱିଗୁଣ, short,  v_int32)
DEF_CVT_FUNC(64f32u, cvt_64f, ଦ୍ୱିଗୁଣ, unsigned, v_float32)
DEF_CVT_FUNC(64f32s, cvt_, ଦ୍ୱିଗୁଣ, ପୂର୍ଣ୍ଣସଂଖ୍ୟା,    v_int32)
DEF_CVT_FUNC(64f32f, cvt_, ଦ୍ୱିଗୁଣ, ଦଶମିକ,  v_float32)
DEF_CVT_FUNC(64f64u, cvt_64f, ଦ୍ୱିଗୁଣ, uint64_t, v_float64)
DEF_CVT_FUNC(64f64s, cvt_64f, ଦ୍ୱିଗୁଣ, int64_t, v_float64)
DEF_CVT_FUNC(64f16f, cvt1_,ଦ୍ୱିଗୁଣ, hfloat, v_float32)
DEF_CVT_FUNC(64f16bf, cvt1_,ଦ୍ୱିଗୁଣ, bfloat, v_float32)
DEF_CVT2BOOL_FUNC(64f8b, int64_t, 1)

////////////////////// 16f -> ... ////////////////////////

DEF_CVT_FUNC(16f8u,  cvt_,  hfloat, uchar,  v_float32)
DEF_CVT_FUNC(16f8s,  cvt_,  hfloat, schar,  v_float32)
DEF_CVT_FUNC(16f16u, cvt1_, hfloat, ushort, v_float32)
DEF_CVT_FUNC(16f16s, cvt1_, hfloat, short,  v_float32)
DEF_CVT_FUNC(16f32u, cvt1_, hfloat, unsigned, v_float32)
DEF_CVT_FUNC(16f32s, cvt1_, hfloat, ପୂର୍ଣ୍ଣସଂଖ୍ୟା,    v_float32)
DEF_CVT_FUNC(16f32f, cvt1_, hfloat, ଦଶମିକ,  v_float32)
DEF_CVT_FUNC(16f64f, cvt1_, hfloat, ଦ୍ୱିଗୁଣ, v_float32)
DEF_CVT_FUNC(16f64u, cvt1_, hfloat, uint64_t, v_float32)
DEF_CVT_FUNC(16f64s, cvt1_, hfloat, int64_t, v_float32)
DEF_CVT_FUNC(16f16bf, cvt1_, hfloat, bfloat, v_float32)
DEF_CVT2BOOL_FUNC(16f8b, short, 1)

////////////////////// 16bf -> ... ////////////////////////

DEF_CVT_FUNC(16bf8u,  cvt_,  bfloat, uchar,  v_float32)
DEF_CVT_FUNC(16bf8s,  cvt_,  bfloat, schar,  v_float32)
DEF_CVT_FUNC(16bf16u, cvt1_, bfloat, ushort, v_float32)
DEF_CVT_FUNC(16bf16s, cvt1_, bfloat, short,  v_float32)
DEF_CVT_FUNC(16bf32u, cvt1_, bfloat, unsigned, v_float32)
DEF_CVT_FUNC(16bf32s, cvt1_, bfloat, ପୂର୍ଣ୍ଣସଂଖ୍ୟା,    v_float32)
DEF_CVT_FUNC(16bf32f, cvt1_, bfloat, ଦଶମିକ,  v_float32)
DEF_CVT_FUNC(16bf64f, cvt1_, bfloat, ଦ୍ୱିଗୁଣ, v_float32)
DEF_CVT_FUNC(16bf64u, cvt1_, bfloat, uint64_t, v_float32)
DEF_CVT_FUNC(16bf64s, cvt1_, bfloat, int64_t, v_float32)
DEF_CVT_FUNC(16bf16f, cvt1_, bfloat, hfloat, v_float32)

////////////////////// 64s -> ... ////////////////////////

DEF_CVT_FUNC(64s8u,  cvt_, int64_t, uchar,  v_int32)
DEF_CVT_FUNC(64s8s,  cvt_, int64_t, schar,  v_int32)
DEF_CVT_FUNC(64s16u, cvt_, int64_t, ushort, v_int32)
DEF_CVT_FUNC(64s16s, cvt_, int64_t, short,  v_int32)
DEF_CVT_FUNC(64s32u, cvt_, int64_t, unsigned, v_uint32)
DEF_CVT_FUNC(64s32s, cvt_, int64_t, ପୂର୍ଣ୍ଣସଂଖ୍ୟା,    v_int32)
DEF_CVT_FUNC(64s32f, cvt_64f, int64_t, ଦଶମିକ,  v_float32)
DEF_CVT_FUNC(64s64f, cvt_64f, int64_t, ଦ୍ୱିଗୁଣ,  v_float64)
DEF_CVT_FUNC(64s64u, cvt_, int64_t, uint64_t, v_uint64)
DEF_CVT_FUNC(64s16f, cvt1_,int64_t, hfloat, v_float32)
DEF_CVT_FUNC(64s16bf, cvt1_, int64_t, bfloat, v_float32)
DEF_CVT2BOOL_FUNC(64s8b, int64_t, 0)

////////////////////// 64u -> ... ////////////////////////

DEF_CVT_FUNC(64u8u,  cvt_, uint64_t, uchar,  v_int32)
DEF_CVT_FUNC(64u8s,  cvt_, uint64_t, schar,  v_int32)
DEF_CVT_FUNC(64u16u, cvt_, uint64_t, ushort, v_int32)
DEF_CVT_FUNC(64u16s, cvt_, uint64_t, short,  v_int32)
DEF_CVT_FUNC(64u32u, cvt_, uint64_t, unsigned, v_uint32)
DEF_CVT_FUNC(64u32s, cvt_, uint64_t, ପୂର୍ଣ୍ଣସଂଖ୍ୟା,   v_int32)
DEF_CVT_FUNC(64u32f, cvt_64f, uint64_t, ଦଶମିକ,  v_float64)
DEF_CVT_FUNC(64u64f, cvt_64f, uint64_t, ଦ୍ୱିଗୁଣ,  v_float64)
DEF_CVT_FUNC(64u16f, cvt1_,uint64_t, hfloat, v_float32)
DEF_CVT_FUNC(64u16bf, cvt1_, uint64_t, bfloat, v_float32)

///////////// "conversion" w/o conversion ///////////////

ସ୍ଥିର ଶୂନ୍ୟ cvt8u(ଅଚଳ uchar* src, size_t sstep, ଅଚଳ uchar*, size_t, uchar* dst, size_t dstep, Size size, ଶୂନ୍ୟ*)
{ CV_INSTRUMENT_REGION(); cvtCopy(src, sstep, dst, dstep, size, 1); }

ସ୍ଥିର ଶୂନ୍ୟ cvt16u(ଅଚଳ uchar* src, size_t sstep, ଅଚଳ uchar*, size_t, uchar* dst, size_t dstep, Size size, ଶୂନ୍ୟ*)
{ CV_INSTRUMENT_REGION(); cvtCopy((ଅଚଳ uchar*)src, sstep, (uchar*)dst, dstep, size, 2); }

ସ୍ଥିର ଶୂନ୍ୟ cvt32s(ଅଚଳ uchar* src, size_t sstep, ଅଚଳ uchar*, size_t, uchar* dst, size_t dstep, Size size, ଶୂନ୍ୟ*)
{ CV_INSTRUMENT_REGION(); cvtCopy((ଅଚଳ uchar*)src, sstep, (uchar*)dst, dstep, size, 4); }

ସ୍ଥିର ଶୂନ୍ୟ cvt64s(ଅଚଳ uchar* src, size_t sstep, ଅଚଳ uchar*, size_t, uchar* dst, size_t dstep, Size size, ଶୂନ୍ୟ*)
{ CV_INSTRUMENT_REGION(); cvtCopy((ଅଚଳ uchar*)src, sstep, (uchar*)dst, dstep, size, 8); }

//////////////////// FP8 (1-byte float) conversions — scalar via saturate_cast ////////////////////
// Suffixes: 8fe4m3 (E4M3FN), 8fe4m3u (E4M3FNUZ).
#define DEF_CVT_FP8(S, T) \
    DEF_CVT_SCALAR_FUNC(S##8u,   T, uchar)     DEF_CVT_SCALAR_FUNC(8u##S,   uchar, T) \
    DEF_CVT_SCALAR_FUNC(S##8s,   T, schar)     DEF_CVT_SCALAR_FUNC(8s##S,   schar, T) \
    DEF_CVT_SCALAR_FUNC(S##16u,  T, ushort)    DEF_CVT_SCALAR_FUNC(16u##S,  ushort, T) \
    DEF_CVT_SCALAR_FUNC(S##16s,  T, short)     DEF_CVT_SCALAR_FUNC(16s##S,  short, T) \
    DEF_CVT_SCALAR_FUNC(S##32u,  T, unsigned)  DEF_CVT_SCALAR_FUNC(32u##S,  unsigned, T) \
    DEF_CVT_SCALAR_FUNC(S##32s,  T, ପୂର୍ଣ୍ଣସଂଖ୍ୟା)       DEF_CVT_SCALAR_FUNC(32s##S,  ପୂର୍ଣ୍ଣସଂଖ୍ୟା, T) \
    DEF_CVT_SCALAR_FUNC(S##64f,  T, ଦ୍ୱିଗୁଣ)    DEF_CVT_SCALAR_FUNC(64f##S,  ଦ୍ୱିଗୁଣ, T) \
    DEF_CVT_SCALAR_FUNC(S##16f,  T, hfloat)    DEF_CVT_SCALAR_FUNC(16f##S,  hfloat, T) \
    DEF_CVT_SCALAR_FUNC(S##16bf, T, bfloat)    DEF_CVT_SCALAR_FUNC(16bf##S, bfloat, T) \
    DEF_CVT_SCALAR_FUNC(S##64u,  T, uint64_t)  DEF_CVT_SCALAR_FUNC(64u##S,  uint64_t, T) \
    DEF_CVT_SCALAR_FUNC(S##64s,  T, int64_t)   DEF_CVT_SCALAR_FUNC(64s##S,  int64_t, T) \
    DEF_CVT_SCALAR_FUNC(S##8b,   T, ତାର୍କିକ)

DEF_CVT_FP8(8fe4m3,  fp8_t)
DEF_CVT_FP8(8fe4m3u, fp8a_t)

// FP8 -> FP8, cross-format only (identity uses the 1-byte copy cvt8u, like 16f->16f uses cvt16u)
DEF_CVT_SCALAR_FUNC(8fe4m38fe4m3u, fp8_t,   fp8a_t)
DEF_CVT_SCALAR_FUNC(8fe4m3u8fe4m3, fp8a_t, fp8_t)

// float32 -> FP8 encode via the shared building blocks in convert.hpp.
template<ପୂର୍ଣ୍ଣସଂଖ୍ୟା bias, ତାର୍କିକ fnuz> ସ୍ଥିର ଶୂନ୍ୟ
cvtF32ToFp8(ଅଚଳ uchar* src_, size_t sstep, ଅଚଳ uchar*, size_t,
            uchar* dst, size_t dstep, Size size, ଶୂନ୍ୟ*)
{
    CV_INSTRUMENT_REGION();
    ଅଚଳ ଦଶମିକ* src = (ଅଚଳ ଦଶମିକ*)src_;
    sstep /= ଆକାର(src[0]);

    ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < size.height; i++, src += sstep, dst += dstep)
    {
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା j = 0;
#ଯଦି (CV_SIMD || CV_SIMD_SCALABLE)
        ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା LANES32 = VTraits<v_int32>::vlanes();
        ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା VECSZ = LANES32 * 4;
        ପାଇଁ (; j <= size.width - VECSZ; j += VECSZ)
        {
            v_float32 vf0 = vx_load(src + j);
            v_float32 vf1 = vx_load(src + j + LANES32);
            v_float32 vf2 = vx_load(src + j + 2 * LANES32);
            v_float32 vf3 = vx_load(src + j + 3 * LANES32);
            encodeFp8Vec4<bias, fnuz>(vf0, vf1, vf2, vf3, src + j, VECSZ, dst + j);
        }
        vx_cleanup();
#endif
        ପାଇଁ (; j < size.width; j++)
            dst[j] = fp8_detail::encodeE4M3(src[j], bias, fnuz);
    }
}
ସ୍ଥିର ଶୂନ୍ୟ cvt32f8fe4m3(ଅଚଳ uchar* s, size_t ss, ଅଚଳ uchar* p, size_t ps, uchar* d, size_t ds, Size sz, ଶୂନ୍ୟ* x)
{ cvtF32ToFp8<7, ମିଥ୍ୟା>(s, ss, p, ps, d, ds, sz, x); }
ସ୍ଥିର ଶୂନ୍ୟ cvt32f8fe4m3u(ଅଚଳ uchar* s, size_t ss, ଅଚଳ uchar* p, size_t ps, uchar* d, size_t ds, Size sz, ଶୂନ୍ୟ* x)
{ cvtF32ToFp8<8, ସତ୍ୟ>(s, ss, p, ps, d, ds, sz, x); }

// FP8 -> float32: 256-entry decode table gathered via universal intrinsics (same table as scalar)
template<typename FP8>
ସ୍ଥିର ଶୂନ୍ୟ cvtFp8ToF32(ଅଚଳ uchar* src, size_t sstep, ଅଚଳ uchar*, size_t,
                        uchar* dst_, size_t dstep, Size size, ଶୂନ୍ୟ*)
{
    CV_INSTRUMENT_REGION();
    ଦଶମିକ* dst = (ଦଶମିକ*)dst_;
    ଅଚଳ ଦଶମିକ* tab = FP8::decodeLUT();
    dstep /= ଆକାର(dst[0]);
    ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < size.height; i++, src += sstep, dst += dstep)
    {
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା j = 0;
#ଯଦି (CV_SIMD || CV_SIMD_SCALABLE)
        ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା VECSZ = VTraits<v_float32>::vlanes();
        ପାଇଁ (; j <= size.width - VECSZ; j += VECSZ)
            v_store(dst + j, v_lut(tab, v_reinterpret_as_s32(vx_load_expand_q(src + j))));
#endif
        ପାଇଁ (; j < size.width; j++) dst[j] = tab[src[j]];
    }
}
ସ୍ଥିର ଶୂନ୍ୟ cvt8fe4m332f(ଅଚଳ uchar* s, size_t ss, ଅଚଳ uchar* p, size_t ps, uchar* d, size_t ds, Size sz, ଶୂନ୍ୟ* x)
{ cvtFp8ToF32<fp8_t>(s, ss, p, ps, d, ds, sz, x); }
ସ୍ଥିର ଶୂନ୍ୟ cvt8fe4m3u32f(ଅଚଳ uchar* s, size_t ss, ଅଚଳ uchar* p, size_t ps, uchar* d, size_t ds, Size sz, ଶୂନ୍ୟ* x)
{ cvtFp8ToF32<fp8a_t>(s, ss, p, ps, d, ds, sz, x); }

ଦ୍ୱିମିକ_Func getConvertFunc(ପୂର୍ଣ୍ଣସଂଖ୍ୟା sdepth_, ପୂର୍ଣ୍ଣସଂଖ୍ୟା ddepth_)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା sdepth = CV_MAT_DEPTH(sdepth_);
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା ddepth = CV_MAT_DEPTH(ddepth_);
    ଦ୍ୱିମିକ_Func func =
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
    ଫେରନ୍ତୁ func;
}

CV_CPU_OPTIMIZATION_NAMESPACE_END
#endif
} // namespace
