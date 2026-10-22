// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


#ਅਨ੍ਤਰ੍ਭਾਵ "precomp.hpp"
#ਅਨ੍ਤਰ੍ਭਾਵ "convert.hpp"

ਨਾਮਾਕਾਸ਼ cv {
CV_CPU_OPTIMIZATION_NAMESPACE_BEGIN

ਦੁਆਧਾਰੀ_Func getCvtScaleAbsFunc(ਪੂਰਨ ਡੂੰਘਾਈ);
ਦੁਆਧਾਰੀ_Func getConvertScaleFunc(ਪੂਰਨ sdepth, ਪੂਰਨ ddepth);

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

/****************************************************************************************\
*                                convertScale[Abs]                                       *
\****************************************************************************************/

ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ _Ts, ਕਿਸਮ_ਨਾਂ _Td> ਸਤਰਅੰਦਰ ਖਾਲੀ
cvtabs_32f( ਅਚਲ _Ts* src, size_t sstep, _Td* dst, size_t dstep,
            ਮਾਪ size, ਦਸ਼ਮਲਵ a, ਦਸ਼ਮਲਵ b )
{
#ਜੇ (CV_SIMD || CV_SIMD_SCALABLE)
    v_float32 va = vx_setall_f32(a), vb = vx_setall_f32(b);
    ਅਚਲ ਪੂਰਨ VECSZ = VTraits<v_float32>::vlanes()*2;
    // GCC miscompiles this scalable block only on VLEN=128 RVV (opencv/issues/26936);
    // use scalar there, keep SIMD on wider vectors. Ask the hardware for VLEN directly:
    // vlanes() would depend on the LMUL the universal intrinsics happen to use.
#ਜੇ (CV_RVV && CV_SIMD_SCALABLE && defined(__GNUC__) && !defined(__clang__))
    ਅਚਲ ਤਾਰਕਿਕ useSIMD = __riscv_vlenb()*8 != 128;
#ਨਹੀਂਤਾਂ
    ਅਚਲ ਤਾਰਕਿਕ useSIMD = ਸੱਚ;
#ਜੇ_ਅੰਤ
#ਜੇ_ਅੰਤ
    sstep /= ਆਕਾਰ(src[0]);
    dstep /= ਆਕਾਰ(dst[0]);

    ਲਈ( ਪੂਰਨ i = 0; i < size.ਉਚਾਈ; i++, src += sstep, dst += dstep )
    {
        ਪੂਰਨ j = 0;
#ਜੇ (CV_SIMD || CV_SIMD_SCALABLE)
        ਜੇ( useSIMD )
        ਲਈ( ; j < size.ਚੌੜਾਈ; j += VECSZ )
        {
            ਜੇ( j > size.ਚੌੜਾਈ - VECSZ )
            {
                ਜੇ( j == 0 || src == (_Ts*)dst )
                    ਰੋਕੋ;
                j = size.ਚੌੜਾਈ - VECSZ;
            }
            v_float32 v0, v1;
            vx_ਲੱਦੋ_pair_as(src + j, v0, v1);
            v0 = v_fma(v0, va, vb);
            v1 = v_fma(v1, va, vb);
            v_store_pair_as(dst + j, v_abs(v0), v_abs(v1));
        }
#ਜੇ_ਅੰਤ
        ਲਈ( ; j < size.ਚੌੜਾਈ; j++ )
            dst[j] = saturate_cast<_Td>(std::ਨਿਰਪੇਖ(src[j]*a + b));
    }
}

ਸਥਿਰ ਖਾਲੀ
cvtabs_32f( ਅਚਲ ਤਾਰਕਿਕ* src_, size_t sstep,
            uchar* dst, size_t dstep,
            ਮਾਪ size, ਦਸ਼ਮਲਵ a, ਦਸ਼ਮਲਵ b )
{
    ਅਚਲ uchar* src = (ਅਚਲ uchar*)src_;
    uchar v0 = saturate_cast<uchar>(std::ਨਿਰਪੇਖ(b));
    uchar v1 = saturate_cast<uchar>(std::ਨਿਰਪੇਖ(a + b));
    ਲਈ( ਪੂਰਨ i = 0; i < size.ਉਚਾਈ; i++, src += sstep, dst += dstep )
    {
        ਲਈ (ਪੂਰਨ j = 0; j < size.ਚੌੜਾਈ; j++)
            dst[j] = src[j] != 0 ? v1 : v0;
    }
}

ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ _Ts, ਕਿਸਮ_ਨਾਂ _Td> ਸਤਰਅੰਦਰ ਖਾਲੀ
cvt_32f( ਅਚਲ _Ts* src, size_t sstep, _Td* dst, size_t dstep,
         ਮਾਪ size, ਦਸ਼ਮਲਵ a, ਦਸ਼ਮਲਵ b )
{
#ਜੇ (CV_SIMD || (CV_SIMD_SCALABLE && !(defined(__GNUC__) && !defined(__clang__))) )
    v_float32 va = vx_setall_f32(a), vb = vx_setall_f32(b);
    ਅਚਲ ਪੂਰਨ VECSZ = VTraits<v_float32>::vlanes()*2;
#ਜੇ_ਅੰਤ
    sstep /= ਆਕਾਰ(src[0]);
    dstep /= ਆਕਾਰ(dst[0]);

    ਲਈ( ਪੂਰਨ i = 0; i < size.ਉਚਾਈ; i++, src += sstep, dst += dstep )
    {
        ਪੂਰਨ j = 0;
// Excluding GNU in CV_SIMD_SCALABLE because of "opencv/issues/26936"
#ਜੇ (CV_SIMD || (CV_SIMD_SCALABLE && !(defined(__GNUC__) && !defined(__clang__))) )
        ਲਈ( ; j < size.ਚੌੜਾਈ; j += VECSZ )
        {
            ਜੇ( j > size.ਚੌੜਾਈ - VECSZ )
            {
                ਜੇ( j == 0 || src == (_Ts*)dst )
                    ਰੋਕੋ;
                j = size.ਚੌੜਾਈ - VECSZ;
            }
            v_float32 v0, v1;
            vx_ਲੱਦੋ_pair_as(src + j, v0, v1);
            v0 = v_fma(v0, va, vb);
            v1 = v_fma(v1, va, vb);
            v_store_pair_as(dst + j, v0, v1);
        }
#ਜੇ_ਅੰਤ
        ਲਈ( ; j < size.ਚੌੜਾਈ; j++ )
            dst[j] = saturate_cast<_Td>(src[j]*a + b);
    }
}

// variant for conversions 16f <-> ... w/o unrolling
ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ _Ts, ਕਿਸਮ_ਨਾਂ _Td> ਸਤਰਅੰਦਰ ਖਾਲੀ
cvt1_32f( ਅਚਲ _Ts* src, size_t sstep, _Td* dst, size_t dstep,
          ਮਾਪ size, ਦਸ਼ਮਲਵ a, ਦਸ਼ਮਲਵ b )
{
#ਜੇ (CV_SIMD || CV_SIMD_SCALABLE)
    v_float32 va = vx_setall_f32(a), vb = vx_setall_f32(b);
    ਅਚਲ ਪੂਰਨ VECSZ = VTraits<v_float32>::vlanes();
#ਜੇ_ਅੰਤ
    sstep /= ਆਕਾਰ(src[0]);
    dstep /= ਆਕਾਰ(dst[0]);

    ਲਈ( ਪੂਰਨ i = 0; i < size.ਉਚਾਈ; i++, src += sstep, dst += dstep )
    {
        ਪੂਰਨ j = 0;
#ਜੇ (CV_SIMD || CV_SIMD_SCALABLE)
        ਲਈ( ; j < size.ਚੌੜਾਈ; j += VECSZ )
        {
            ਜੇ( j > size.ਚੌੜਾਈ - VECSZ )
            {
                ਜੇ( j == 0 || src == (_Ts*)dst )
                    ਰੋਕੋ;
                j = size.ਚੌੜਾਈ - VECSZ;
            }
            v_float32 v0;
            vx_ਲੱਦੋ_as(src + j, v0);
            v0 = v_fma(v0, va, vb);
            v_store_as(dst + j, v0);
        }
#ਜੇ_ਅੰਤ
        ਲਈ( ; j < size.ਚੌੜਾਈ; j++ )
            dst[j] = saturate_cast<_Td>(src[j]*a + b);
    }
}


ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ _Ts, ਕਿਸਮ_ਨਾਂ _Td> ਸਤਰਅੰਦਰ ਖਾਲੀ
cvt_64f( ਅਚਲ _Ts* src, size_t sstep, _Td* dst, size_t dstep,
         ਮਾਪ size, ਦੁੱਗਣਾ a, ਦੁੱਗਣਾ b )
{
#ਜੇ (CV_SIMD_64F || (CV_SIMD_SCALABLE_64F && !(defined(__GNUC__) && !defined(__clang__))) )
    v_float64 va = vx_setall_f64(a), vb = vx_setall_f64(b);
    ਅਚਲ ਪੂਰਨ VECSZ = VTraits<v_float64>::vlanes()*2;
#ਜੇ_ਅੰਤ
    sstep /= ਆਕਾਰ(src[0]);
    dstep /= ਆਕਾਰ(dst[0]);

    ਲਈ( ਪੂਰਨ i = 0; i < size.ਉਚਾਈ; i++, src += sstep, dst += dstep )
    {
        ਪੂਰਨ j = 0;
// Excluding GNU in CV_SIMD_SCALABLE because of "opencv/issues/26936"
#ਜੇ (CV_SIMD_64F || (CV_SIMD_SCALABLE_64F && !(defined(__GNUC__) && !defined(__clang__))) )
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
            v0 = v_fma(v0, va, vb);
            v1 = v_fma(v1, va, vb);
            v_store_pair_as(dst + j, v0, v1);
        }
#ਜੇ_ਅੰਤ
        ਲਈ( ; j < size.ਚੌੜਾਈ; j++ )
            dst[j] = saturate_cast<_Td>(src[j]*a + b);
    }
}

//==================================================================================================

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ DEF_CVT_SCALE_ABS_FUNC(suffix, cvt, stype, dtype, wtype) \
ਸਥਿਰ ਖਾਲੀ cvtScaleAbs##suffix( ਅਚਲ uchar* src_, size_t sstep, ਅਚਲ uchar*, size_t, \
                                 uchar* dst_, size_t dstep, ਮਾਪ size, ਖਾਲੀ* scale_) \
{ \
    ਅਚਲ stype* src = (ਅਚਲ stype*)src_; \
    dtype* dst = (dtype*)dst_; \
    ਦੁੱਗਣਾ* scale = (ਦੁੱਗਣਾ*)scale_; \
    cvt(src, sstep, dst, dstep, size, (wtype)scale[0], (wtype)scale[1]); \
}


#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ DEF_CVT_SCALE_FUNC(suffix, cvt, stype, dtype, wtype) \
ਸਥਿਰ ਖਾਲੀ cvtScale##suffix( ਅਚਲ uchar* src_, size_t sstep, ਅਚਲ uchar*, size_t, \
                              uchar* dst_, size_t dstep, ਮਾਪ size, ਖਾਲੀ* scale_) \
{ \
    ਅਚਲ stype* src = (ਅਚਲ stype*)src_; \
    dtype* dst = (dtype*)dst_; \
    ਦੁੱਗਣਾ* scale = (ਦੁੱਗਣਾ*)scale_; \
    cvt(src, sstep, dst, dstep, size, (wtype)scale[0], (wtype)scale[1]); \
}

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ DEF_CVT_SCALE2BOOL_FUNC(suffix, stype, wtype) \
ਸਥਿਰ ਖਾਲੀ cvtScale##suffix( ਅਚਲ uchar* src_, size_t sstep, ਅਚਲ uchar*, size_t, \
                              uchar* dst, size_t dstep, ਮਾਪ size, ਖਾਲੀ* scale_) \
{ \
    ਅਚਲ stype* src = (ਅਚਲ stype*)src_; \
    ਅਚਲ ਦੁੱਗਣਾ* scale = (ਅਚਲ ਦੁੱਗਣਾ*)scale_; \
    wtype a = (wtype)scale[0], b = (wtype)scale[1]; \
    sstep /= ਆਕਾਰ(src[0]); \
    ਲਈ( ਪੂਰਨ i = 0; i < size.ਉਚਾਈ; i++, src += sstep, dst += dstep ) \
        ਲਈ (ਪੂਰਨ j = 0; j < size.ਚੌੜਾਈ; j++) \
            dst[j] = (ਤਾਰਕਿਕ)((wtype)src[j]*a + b != 0); \
}

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ DEF_CVT_SCALEBOOL2_FUNC(suffix, dtype, wtype) \
ਸਥਿਰ ਖਾਲੀ cvtScale##suffix( ਅਚਲ uchar* src, size_t sstep, ਅਚਲ uchar*, size_t, \
                              uchar* dst_, size_t dstep, ਮਾਪ size, ਖਾਲੀ* scale_) \
{ \
    dtype* dst = (dtype*)dst_; \
    ਅਚਲ ਦੁੱਗਣਾ* scale = (ਅਚਲ ਦੁੱਗਣਾ*)scale_; \
    wtype a = (wtype)scale[0], b = (wtype)scale[1]; \
    dtype v0 = saturate_cast<dtype>(b), v1 = saturate_cast<dtype>(a + b); \
    dstep /= ਆਕਾਰ(dst[0]); \
    ਲਈ( ਪੂਰਨ i = 0; i < size.ਉਚਾਈ; i++, src += sstep, dst += dstep ) \
        ਲਈ (ਪੂਰਨ j = 0; j < size.ਚੌੜਾਈ; j++) \
            dst[j] = src[j] != 0 ? v1 : v0; \
}

DEF_CVT_SCALE_ABS_FUNC(8u,    cvtabs_32f, uchar,  uchar, ਦਸ਼ਮਲਵ)
DEF_CVT_SCALE_ABS_FUNC(8s8u,  cvtabs_32f, schar,  uchar, ਦਸ਼ਮਲਵ)
DEF_CVT_SCALE_ABS_FUNC(8b8u,  cvtabs_32f, ਤਾਰਕਿਕ,  uchar, ਦਸ਼ਮਲਵ)
DEF_CVT_SCALE_ABS_FUNC(16u8u, cvtabs_32f, ushort, uchar, ਦਸ਼ਮਲਵ)
DEF_CVT_SCALE_ABS_FUNC(16s8u, cvtabs_32f, ਛੋਟਾ,  uchar, ਦਸ਼ਮਲਵ)
DEF_CVT_SCALE_ABS_FUNC(32u8u, cvtabs_32f, ਬਿਨਾਨਿਸ਼ਾਨੀ, uchar, ਦਸ਼ਮਲਵ)
DEF_CVT_SCALE_ABS_FUNC(32s8u, cvtabs_32f, ਪੂਰਨ,    uchar, ਦਸ਼ਮਲਵ)
DEF_CVT_SCALE_ABS_FUNC(32f8u, cvtabs_32f, ਦਸ਼ਮਲਵ,  uchar, ਦਸ਼ਮਲਵ)
DEF_CVT_SCALE_ABS_FUNC(64u8u, cvtabs_32f, uint64_t, uchar, ਦਸ਼ਮਲਵ)
DEF_CVT_SCALE_ABS_FUNC(64s8u, cvtabs_32f, int64_t, uchar, ਦਸ਼ਮਲਵ)
DEF_CVT_SCALE_ABS_FUNC(64f8u, cvtabs_32f, ਦੁੱਗਣਾ, uchar, ਦਸ਼ਮਲਵ)
DEF_CVT_SCALE_ABS_FUNC(16f8u, cvtabs_32f, hfloat, uchar, ਦਸ਼ਮਲਵ)
DEF_CVT_SCALE_ABS_FUNC(16bf8u, cvtabs_32f, bfloat, uchar, ਦਸ਼ਮਲਵ)

DEF_CVT_SCALE_FUNC(8u,     cvt_32f, uchar,  uchar, ਦਸ਼ਮਲਵ)
DEF_CVT_SCALE_FUNC(8s8u,   cvt_32f, schar,  uchar, ਦਸ਼ਮਲਵ)
DEF_CVT_SCALE_FUNC(16u8u,  cvt_32f, ushort, uchar, ਦਸ਼ਮਲਵ)
DEF_CVT_SCALE_FUNC(16s8u,  cvt_32f, ਛੋਟਾ,  uchar, ਦਸ਼ਮਲਵ)
DEF_CVT_SCALE_FUNC(32u8u,  cvt_32f, ਬਿਨਾਨਿਸ਼ਾਨੀ, uchar, ਦਸ਼ਮਲਵ)
DEF_CVT_SCALE_FUNC(32s8u,  cvt_32f, ਪੂਰਨ,    uchar, ਦਸ਼ਮਲਵ)
DEF_CVT_SCALE_FUNC(32f8u,  cvt_32f, ਦਸ਼ਮਲਵ,  uchar, ਦਸ਼ਮਲਵ)
DEF_CVT_SCALE_FUNC(64f8u,  cvt_32f, ਦੁੱਗਣਾ, uchar, ਦਸ਼ਮਲਵ)
DEF_CVT_SCALE_FUNC(64u8u,  cvt_32f, uint64_t, uchar, ਦਸ਼ਮਲਵ)
DEF_CVT_SCALE_FUNC(64s8u,  cvt_32f, int64_t, uchar, ਦਸ਼ਮਲਵ)
DEF_CVT_SCALE_FUNC(16f8u,  cvt_32f, hfloat, uchar, ਦਸ਼ਮਲਵ)
DEF_CVT_SCALE_FUNC(16bf8u, cvt_32f, bfloat, uchar, ਦਸ਼ਮਲਵ)

DEF_CVT_SCALE_FUNC(8u8s,   cvt_32f, uchar,  schar, ਦਸ਼ਮਲਵ)
DEF_CVT_SCALE_FUNC(8s,     cvt_32f, schar,  schar, ਦਸ਼ਮਲਵ)
DEF_CVT_SCALE_FUNC(16u8s,  cvt_32f, ushort, schar, ਦਸ਼ਮਲਵ)
DEF_CVT_SCALE_FUNC(16s8s,  cvt_32f, ਛੋਟਾ,  schar, ਦਸ਼ਮਲਵ)
DEF_CVT_SCALE_FUNC(32u8s,  cvt_32f, ਬਿਨਾਨਿਸ਼ਾਨੀ, schar, ਦਸ਼ਮਲਵ)
DEF_CVT_SCALE_FUNC(32s8s,  cvt_32f, ਪੂਰਨ,    schar, ਦਸ਼ਮਲਵ)
DEF_CVT_SCALE_FUNC(32f8s,  cvt_32f, ਦਸ਼ਮਲਵ,  schar, ਦਸ਼ਮਲਵ)
DEF_CVT_SCALE_FUNC(64f8s,  cvt_32f, ਦੁੱਗਣਾ, schar, ਦਸ਼ਮਲਵ)
DEF_CVT_SCALE_FUNC(64u8s,  cvt_32f, uint64_t, schar, ਦਸ਼ਮਲਵ)
DEF_CVT_SCALE_FUNC(64s8s,  cvt_32f, int64_t, schar, ਦਸ਼ਮਲਵ)
DEF_CVT_SCALE_FUNC(16f8s,  cvt_32f, hfloat, schar, ਦਸ਼ਮਲਵ)
DEF_CVT_SCALE_FUNC(16bf8s, cvt_32f, bfloat, schar, ਦਸ਼ਮਲਵ)

DEF_CVT_SCALE2BOOL_FUNC(8u8b, uchar, ਦਸ਼ਮਲਵ)
DEF_CVT_SCALE2BOOL_FUNC(8s8b, schar, ਦਸ਼ਮਲਵ)
DEF_CVT_SCALE2BOOL_FUNC(16u8b, ushort, ਦਸ਼ਮਲਵ)
DEF_CVT_SCALE2BOOL_FUNC(16s8b, ਛੋਟਾ, ਦਸ਼ਮਲਵ)
DEF_CVT_SCALE2BOOL_FUNC(32u8b, ਬਿਨਾਨਿਸ਼ਾਨੀ, ਦਸ਼ਮਲਵ)
DEF_CVT_SCALE2BOOL_FUNC(32s8b, ਪੂਰਨ, ਦਸ਼ਮਲਵ)
DEF_CVT_SCALE2BOOL_FUNC(32f8b, ਦਸ਼ਮਲਵ, ਦਸ਼ਮਲਵ)
DEF_CVT_SCALE2BOOL_FUNC(64f8b, ਦੁੱਗਣਾ, ਦਸ਼ਮਲਵ)
DEF_CVT_SCALE2BOOL_FUNC(64u8b, uint64_t, ਦਸ਼ਮਲਵ)
DEF_CVT_SCALE2BOOL_FUNC(64s8b, int64_t, ਦਸ਼ਮਲਵ)
DEF_CVT_SCALE2BOOL_FUNC(16f8b, hfloat, ਦਸ਼ਮਲਵ)
DEF_CVT_SCALE2BOOL_FUNC(16bf8b, bfloat, ਦਸ਼ਮਲਵ)

DEF_CVT_SCALE_FUNC(8u16u,  cvt_32f, uchar,  ushort, ਦਸ਼ਮਲਵ)
DEF_CVT_SCALE_FUNC(8s16u,  cvt_32f, schar,  ushort, ਦਸ਼ਮਲਵ)
DEF_CVT_SCALE_FUNC(16u,    cvt_32f, ushort, ushort, ਦਸ਼ਮਲਵ)
DEF_CVT_SCALE_FUNC(16s16u, cvt_32f, ਛੋਟਾ,  ushort, ਦਸ਼ਮਲਵ)
DEF_CVT_SCALE_FUNC(32u16u, cvt_32f, ਬਿਨਾਨਿਸ਼ਾਨੀ, ushort, ਦਸ਼ਮਲਵ)
DEF_CVT_SCALE_FUNC(32s16u, cvt_32f, ਪੂਰਨ,    ushort, ਦਸ਼ਮਲਵ)
DEF_CVT_SCALE_FUNC(32f16u, cvt_32f, ਦਸ਼ਮਲਵ,  ushort, ਦਸ਼ਮਲਵ)
DEF_CVT_SCALE_FUNC(64f16u, cvt_32f, ਦੁੱਗਣਾ, ushort, ਦਸ਼ਮਲਵ)
DEF_CVT_SCALE_FUNC(64u16u, cvt_32f, uint64_t, ushort, ਦਸ਼ਮਲਵ)
DEF_CVT_SCALE_FUNC(64s16u, cvt_32f, int64_t, ushort, ਦਸ਼ਮਲਵ)
DEF_CVT_SCALE_FUNC(16f16u, cvt1_32f, hfloat, ushort, ਦਸ਼ਮਲਵ)
DEF_CVT_SCALE_FUNC(16bf16u, cvt1_32f, bfloat, ushort, ਦਸ਼ਮਲਵ)

DEF_CVT_SCALE_FUNC(8u16s,  cvt_32f, uchar,  ਛੋਟਾ, ਦਸ਼ਮਲਵ)
DEF_CVT_SCALE_FUNC(8s16s,  cvt_32f, schar,  ਛੋਟਾ, ਦਸ਼ਮਲਵ)
DEF_CVT_SCALE_FUNC(16u16s, cvt_32f, ushort, ਛੋਟਾ, ਦਸ਼ਮਲਵ)
DEF_CVT_SCALE_FUNC(16s,    cvt_32f, ਛੋਟਾ,  ਛੋਟਾ, ਦਸ਼ਮਲਵ)
DEF_CVT_SCALE_FUNC(32u16s, cvt_32f, ਬਿਨਾਨਿਸ਼ਾਨੀ, ਛੋਟਾ, ਦਸ਼ਮਲਵ)
DEF_CVT_SCALE_FUNC(32s16s, cvt_32f, ਪੂਰਨ,    ਛੋਟਾ, ਦਸ਼ਮਲਵ)
DEF_CVT_SCALE_FUNC(32f16s, cvt_32f, ਦਸ਼ਮਲਵ,  ਛੋਟਾ, ਦਸ਼ਮਲਵ)
DEF_CVT_SCALE_FUNC(64f16s, cvt_32f, ਦੁੱਗਣਾ, ਛੋਟਾ, ਦਸ਼ਮਲਵ)
DEF_CVT_SCALE_FUNC(64u16s, cvt_32f, uint64_t, ਛੋਟਾ, ਦਸ਼ਮਲਵ)
DEF_CVT_SCALE_FUNC(64s16s, cvt_32f, int64_t, ਛੋਟਾ, ਦਸ਼ਮਲਵ)
DEF_CVT_SCALE_FUNC(16f16s, cvt1_32f, hfloat, ਛੋਟਾ, ਦਸ਼ਮਲਵ)
DEF_CVT_SCALE_FUNC(16bf16s, cvt1_32f, bfloat, ਛੋਟਾ, ਦਸ਼ਮਲਵ)

DEF_CVT_SCALE_FUNC(8u32u,  cvt_32f, uchar,  ਬਿਨਾਨਿਸ਼ਾਨੀ, ਦਸ਼ਮਲਵ)
DEF_CVT_SCALE_FUNC(8s32u,  cvt_32f, schar,  ਬਿਨਾਨਿਸ਼ਾਨੀ, ਦਸ਼ਮਲਵ)
DEF_CVT_SCALE_FUNC(16u32u, cvt_32f, ushort, ਬਿਨਾਨਿਸ਼ਾਨੀ, ਦਸ਼ਮਲਵ)
DEF_CVT_SCALE_FUNC(16s32u, cvt_32f, ਛੋਟਾ,  ਬਿਨਾਨਿਸ਼ਾਨੀ, ਦਸ਼ਮਲਵ)
DEF_CVT_SCALE_FUNC(32u, cvt_32f, ਬਿਨਾਨਿਸ਼ਾਨੀ, ਬਿਨਾਨਿਸ਼ਾਨੀ, ਦਸ਼ਮਲਵ)
DEF_CVT_SCALE_FUNC(32s32u, cvt_64f, ਪੂਰਨ,    ਬਿਨਾਨਿਸ਼ਾਨੀ, ਦੁੱਗਣਾ)
DEF_CVT_SCALE_FUNC(32f32u, cvt_32f, ਦਸ਼ਮਲਵ,  ਬਿਨਾਨਿਸ਼ਾਨੀ, ਦਸ਼ਮਲਵ)
DEF_CVT_SCALE_FUNC(64f32u, cvt_64f, ਦੁੱਗਣਾ, ਬਿਨਾਨਿਸ਼ਾਨੀ, ਦੁੱਗਣਾ)
DEF_CVT_SCALE_FUNC(64u32u, cvt_32f, uint64_t, ਬਿਨਾਨਿਸ਼ਾਨੀ, ਦਸ਼ਮਲਵ)
DEF_CVT_SCALE_FUNC(64s32u, cvt_32f, int64_t, ਬਿਨਾਨਿਸ਼ਾਨੀ, ਦਸ਼ਮਲਵ)
DEF_CVT_SCALE_FUNC(16f32u, cvt1_32f, hfloat, ਬਿਨਾਨਿਸ਼ਾਨੀ, ਦਸ਼ਮਲਵ)
DEF_CVT_SCALE_FUNC(16bf32u, cvt1_32f, bfloat, ਬਿਨਾਨਿਸ਼ਾਨੀ, ਦਸ਼ਮਲਵ)

DEF_CVT_SCALE_FUNC(8u32s,  cvt_32f, uchar,  ਪੂਰਨ, ਦਸ਼ਮਲਵ)
DEF_CVT_SCALE_FUNC(8s32s,  cvt_32f, schar,  ਪੂਰਨ, ਦਸ਼ਮਲਵ)
DEF_CVT_SCALE_FUNC(16u32s, cvt_32f, ushort, ਪੂਰਨ, ਦਸ਼ਮਲਵ)
DEF_CVT_SCALE_FUNC(16s32s, cvt_32f, ਛੋਟਾ,  ਪੂਰਨ, ਦਸ਼ਮਲਵ)
DEF_CVT_SCALE_FUNC(32u32s, cvt_32f, ਬਿਨਾਨਿਸ਼ਾਨੀ, ਪੂਰਨ, ਦਸ਼ਮਲਵ)
DEF_CVT_SCALE_FUNC(32s,    cvt_64f, ਪੂਰਨ,    ਪੂਰਨ, ਦੁੱਗਣਾ)
DEF_CVT_SCALE_FUNC(32f32s, cvt_32f, ਦਸ਼ਮਲਵ,  ਪੂਰਨ, ਦਸ਼ਮਲਵ)
DEF_CVT_SCALE_FUNC(64f32s, cvt_64f, ਦੁੱਗਣਾ, ਪੂਰਨ, ਦੁੱਗਣਾ)
DEF_CVT_SCALE_FUNC(64u32s, cvt_32f, uint64_t, ਪੂਰਨ, ਦਸ਼ਮਲਵ)
DEF_CVT_SCALE_FUNC(64s32s, cvt_32f, int64_t, ਪੂਰਨ, ਦਸ਼ਮਲਵ)
DEF_CVT_SCALE_FUNC(16f32s, cvt1_32f, hfloat, ਪੂਰਨ, ਦਸ਼ਮਲਵ)
DEF_CVT_SCALE_FUNC(16bf32s, cvt1_32f, bfloat, ਪੂਰਨ, ਦਸ਼ਮਲਵ)

DEF_CVT_SCALE_FUNC(8u32f,  cvt_32f, uchar,  ਦਸ਼ਮਲਵ, ਦਸ਼ਮਲਵ)
DEF_CVT_SCALE_FUNC(8s32f,  cvt_32f, schar,  ਦਸ਼ਮਲਵ, ਦਸ਼ਮਲਵ)
DEF_CVT_SCALE_FUNC(16u32f, cvt_32f, ushort, ਦਸ਼ਮਲਵ, ਦਸ਼ਮਲਵ)
DEF_CVT_SCALE_FUNC(16s32f, cvt_32f, ਛੋਟਾ,  ਦਸ਼ਮਲਵ, ਦਸ਼ਮਲਵ)
DEF_CVT_SCALE_FUNC(32u32f, cvt_32f, ਬਿਨਾਨਿਸ਼ਾਨੀ, ਦਸ਼ਮਲਵ, ਦਸ਼ਮਲਵ)
DEF_CVT_SCALE_FUNC(32s32f, cvt_32f, ਪੂਰਨ,    ਦਸ਼ਮਲਵ, ਦਸ਼ਮਲਵ)
DEF_CVT_SCALE_FUNC(32f,    cvt_32f, ਦਸ਼ਮਲਵ,  ਦਸ਼ਮਲਵ, ਦਸ਼ਮਲਵ)
DEF_CVT_SCALE_FUNC(64f32f, cvt_64f, ਦੁੱਗਣਾ, ਦਸ਼ਮਲਵ, ਦੁੱਗਣਾ)
DEF_CVT_SCALE_FUNC(64u32f, cvt_32f, uint64_t, ਦਸ਼ਮਲਵ, ਦਸ਼ਮਲਵ)
DEF_CVT_SCALE_FUNC(64s32f, cvt_32f, int64_t, ਦਸ਼ਮਲਵ, ਦਸ਼ਮਲਵ)
DEF_CVT_SCALE_FUNC(16f32f, cvt1_32f, hfloat, ਦਸ਼ਮਲਵ, ਦਸ਼ਮਲਵ)
DEF_CVT_SCALE_FUNC(16bf32f, cvt1_32f, bfloat, ਦਸ਼ਮਲਵ, ਦਸ਼ਮਲਵ)

DEF_CVT_SCALE_FUNC(8u64f,  cvt_64f, uchar,  ਦੁੱਗਣਾ, ਦੁੱਗਣਾ)
DEF_CVT_SCALE_FUNC(8s64f,  cvt_64f, schar,  ਦੁੱਗਣਾ, ਦੁੱਗਣਾ)
DEF_CVT_SCALE_FUNC(16u64f, cvt_64f, ushort, ਦੁੱਗਣਾ, ਦੁੱਗਣਾ)
DEF_CVT_SCALE_FUNC(16s64f, cvt_64f, ਛੋਟਾ,  ਦੁੱਗਣਾ, ਦੁੱਗਣਾ)
DEF_CVT_SCALE_FUNC(32u64f, cvt_64f, ਬਿਨਾਨਿਸ਼ਾਨੀ, ਦੁੱਗਣਾ, ਦੁੱਗਣਾ)
DEF_CVT_SCALE_FUNC(32s64f, cvt_64f, ਪੂਰਨ,    ਦੁੱਗਣਾ, ਦੁੱਗਣਾ)
DEF_CVT_SCALE_FUNC(32f64f, cvt_64f, ਦਸ਼ਮਲਵ,  ਦੁੱਗਣਾ, ਦੁੱਗਣਾ)
DEF_CVT_SCALE_FUNC(64f,    cvt_64f, ਦੁੱਗਣਾ, ਦੁੱਗਣਾ, ਦੁੱਗਣਾ)
DEF_CVT_SCALE_FUNC(64u64f, cvt_64f, uint64_t, ਦੁੱਗਣਾ, ਦੁੱਗਣਾ)
DEF_CVT_SCALE_FUNC(64s64f, cvt_64f, int64_t, ਦੁੱਗਣਾ, ਦੁੱਗਣਾ)
DEF_CVT_SCALE_FUNC(16f64f, cvt_64f, hfloat, ਦੁੱਗਣਾ, ਦੁੱਗਣਾ)
DEF_CVT_SCALE_FUNC(16bf64f, cvt_64f, bfloat, ਦੁੱਗਣਾ, ਦੁੱਗਣਾ)

DEF_CVT_SCALE_FUNC(8u64u,  cvt_64f, uchar,  uint64_t, ਦੁੱਗਣਾ)
DEF_CVT_SCALE_FUNC(8s64u,  cvt_64f, schar,  uint64_t, ਦੁੱਗਣਾ)
DEF_CVT_SCALE_FUNC(16u64u, cvt_64f, ushort, uint64_t, ਦੁੱਗਣਾ)
DEF_CVT_SCALE_FUNC(16s64u, cvt_64f, ਛੋਟਾ,  uint64_t, ਦੁੱਗਣਾ)
DEF_CVT_SCALE_FUNC(32u64u, cvt_64f, ਬਿਨਾਨਿਸ਼ਾਨੀ, uint64_t, ਦੁੱਗਣਾ)
DEF_CVT_SCALE_FUNC(32s64u, cvt_64f, ਪੂਰਨ,    uint64_t, ਦੁੱਗਣਾ)
DEF_CVT_SCALE_FUNC(32f64u, cvt_64f, ਦਸ਼ਮਲਵ,  uint64_t, ਦੁੱਗਣਾ)
DEF_CVT_SCALE_FUNC(64f64u, cvt_64f, ਦੁੱਗਣਾ, uint64_t, ਦੁੱਗਣਾ)
DEF_CVT_SCALE_FUNC(64u, cvt_64f, uint64_t, uint64_t, ਦੁੱਗਣਾ)
DEF_CVT_SCALE_FUNC(64s64u, cvt_64f, int64_t, uint64_t, ਦੁੱਗਣਾ)
DEF_CVT_SCALE_FUNC(16f64u, cvt_64f, hfloat, uint64_t, ਦੁੱਗਣਾ)
DEF_CVT_SCALE_FUNC(16bf64u, cvt_64f, bfloat, uint64_t, ਦੁੱਗਣਾ)

DEF_CVT_SCALE_FUNC(8u64s,  cvt_64f, uchar,  int64_t, ਦੁੱਗਣਾ)
DEF_CVT_SCALE_FUNC(8s64s,  cvt_64f, schar,  int64_t, ਦੁੱਗਣਾ)
DEF_CVT_SCALE_FUNC(16u64s, cvt_64f, ushort, int64_t, ਦੁੱਗਣਾ)
DEF_CVT_SCALE_FUNC(16s64s, cvt_64f, ਛੋਟਾ,  int64_t, ਦੁੱਗਣਾ)
DEF_CVT_SCALE_FUNC(32u64s, cvt_64f, ਬਿਨਾਨਿਸ਼ਾਨੀ, int64_t, ਦੁੱਗਣਾ)
DEF_CVT_SCALE_FUNC(32s64s, cvt_64f, ਪੂਰਨ,    int64_t, ਦੁੱਗਣਾ)
DEF_CVT_SCALE_FUNC(32f64s, cvt_64f, ਦਸ਼ਮਲਵ,  int64_t, ਦੁੱਗਣਾ)
DEF_CVT_SCALE_FUNC(64f64s, cvt_64f, ਦੁੱਗਣਾ, int64_t, ਦੁੱਗਣਾ)
DEF_CVT_SCALE_FUNC(64u64s, cvt_64f, uint64_t, int64_t, ਦੁੱਗਣਾ)
DEF_CVT_SCALE_FUNC(64s, cvt_64f, int64_t, int64_t, ਦੁੱਗਣਾ)
DEF_CVT_SCALE_FUNC(16f64s, cvt_64f, hfloat, int64_t, ਦੁੱਗਣਾ)
DEF_CVT_SCALE_FUNC(16bf64s, cvt_64f, bfloat, int64_t, ਦੁੱਗਣਾ)

DEF_CVT_SCALE_FUNC(8u16f,  cvt1_32f, uchar,  hfloat, ਦਸ਼ਮਲਵ)
DEF_CVT_SCALE_FUNC(8s16f,  cvt1_32f, schar,  hfloat, ਦਸ਼ਮਲਵ)
DEF_CVT_SCALE_FUNC(16u16f, cvt1_32f, ushort, hfloat, ਦਸ਼ਮਲਵ)
DEF_CVT_SCALE_FUNC(16s16f, cvt1_32f, ਛੋਟਾ,  hfloat, ਦਸ਼ਮਲਵ)
DEF_CVT_SCALE_FUNC(32u16f, cvt1_32f, ਬਿਨਾਨਿਸ਼ਾਨੀ, hfloat, ਦਸ਼ਮਲਵ)
DEF_CVT_SCALE_FUNC(32s16f, cvt1_32f, ਪੂਰਨ,    hfloat, ਦਸ਼ਮਲਵ)
DEF_CVT_SCALE_FUNC(32f16f, cvt1_32f, ਦਸ਼ਮਲਵ,  hfloat, ਦਸ਼ਮਲਵ)
DEF_CVT_SCALE_FUNC(64f16f, cvt1_32f, ਦੁੱਗਣਾ, hfloat, ਦਸ਼ਮਲਵ)
DEF_CVT_SCALE_FUNC(64u16f, cvt1_32f, uint64_t, hfloat, ਦਸ਼ਮਲਵ)
DEF_CVT_SCALE_FUNC(64s16f, cvt1_32f, int64_t, hfloat, ਦਸ਼ਮਲਵ)
DEF_CVT_SCALE_FUNC(16f,    cvt1_32f, hfloat, hfloat, ਦਸ਼ਮਲਵ)
DEF_CVT_SCALE_FUNC(16bf16f, cvt1_32f, bfloat, hfloat, ਦਸ਼ਮਲਵ)

DEF_CVT_SCALE_FUNC(8u16bf,  cvt1_32f, uchar,  bfloat, ਦਸ਼ਮਲਵ)
DEF_CVT_SCALE_FUNC(8s16bf,  cvt1_32f, schar,  bfloat, ਦਸ਼ਮਲਵ)
DEF_CVT_SCALE_FUNC(16u16bf, cvt1_32f, ushort, bfloat, ਦਸ਼ਮਲਵ)
DEF_CVT_SCALE_FUNC(16s16bf, cvt1_32f, ਛੋਟਾ,  bfloat, ਦਸ਼ਮਲਵ)
DEF_CVT_SCALE_FUNC(32u16bf, cvt1_32f, ਬਿਨਾਨਿਸ਼ਾਨੀ, bfloat, ਦਸ਼ਮਲਵ)
DEF_CVT_SCALE_FUNC(32s16bf, cvt1_32f, ਪੂਰਨ,    bfloat, ਦਸ਼ਮਲਵ)
DEF_CVT_SCALE_FUNC(32f16bf, cvt1_32f, ਦਸ਼ਮਲਵ,  bfloat, ਦਸ਼ਮਲਵ)
DEF_CVT_SCALE_FUNC(64f16bf, cvt1_32f, ਦੁੱਗਣਾ, bfloat, ਦਸ਼ਮਲਵ)
DEF_CVT_SCALE_FUNC(64u16bf, cvt1_32f, uint64_t, bfloat, ਦਸ਼ਮਲਵ)
DEF_CVT_SCALE_FUNC(64s16bf, cvt1_32f, int64_t, bfloat, ਦਸ਼ਮਲਵ)
DEF_CVT_SCALE_FUNC(16f16bf, cvt1_32f, hfloat, bfloat, ਦਸ਼ਮਲਵ)
DEF_CVT_SCALE_FUNC(16bf, cvt1_32f, bfloat, bfloat, ਦਸ਼ਮਲਵ)

DEF_CVT_SCALEBOOL2_FUNC(8b8u, uchar, ਦਸ਼ਮਲਵ)
DEF_CVT_SCALEBOOL2_FUNC(8b8s, schar, ਦਸ਼ਮਲਵ)
DEF_CVT_SCALEBOOL2_FUNC(8b, ਤਾਰਕਿਕ, ਦਸ਼ਮਲਵ)
DEF_CVT_SCALEBOOL2_FUNC(8b16u, ushort, ਦਸ਼ਮਲਵ)
DEF_CVT_SCALEBOOL2_FUNC(8b16s, ਛੋਟਾ, ਦਸ਼ਮਲਵ)
DEF_CVT_SCALEBOOL2_FUNC(8b32u, ਬਿਨਾਨਿਸ਼ਾਨੀ, ਦਸ਼ਮਲਵ)
DEF_CVT_SCALEBOOL2_FUNC(8b32s, ਪੂਰਨ, ਦਸ਼ਮਲਵ)
DEF_CVT_SCALEBOOL2_FUNC(8b32f, ਦਸ਼ਮਲਵ, ਦਸ਼ਮਲਵ)
DEF_CVT_SCALEBOOL2_FUNC(8b64u, uint64_t, ਦੁੱਗਣਾ)
DEF_CVT_SCALEBOOL2_FUNC(8b64s, int64_t, ਦੁੱਗਣਾ)
DEF_CVT_SCALEBOOL2_FUNC(8b64f, ਦੁੱਗਣਾ, ਦੁੱਗਣਾ)
DEF_CVT_SCALEBOOL2_FUNC(8b16f, hfloat, ਦਸ਼ਮਲਵ)
DEF_CVT_SCALEBOOL2_FUNC(8b16bf, bfloat, ਦਸ਼ਮਲਵ)

// FP8 src -> CV_8U abs|scale|shift (scalar)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ DEF_CVT_SCALE_ABS_SCALAR_FUNC(suffix, stype) \
ਸਥਿਰ ਖਾਲੀ cvtScaleAbs##suffix( ਅਚਲ uchar* src_, size_t sstep, ਅਚਲ uchar*, size_t, \
                                 uchar* dst, size_t dstep, ਮਾਪ size, ਖਾਲੀ* scale_) \
{ \
    ਅਚਲ stype* src = (ਅਚਲ stype*)src_; \
    ਅਚਲ ਦੁੱਗਣਾ* scale = (ਅਚਲ ਦੁੱਗਣਾ*)scale_; \
    ਦਸ਼ਮਲਵ a = (ਦਸ਼ਮਲਵ)scale[0], b = (ਦਸ਼ਮਲਵ)scale[1]; \
    sstep /= ਆਕਾਰ(src[0]); \
    ਲਈ( ਪੂਰਨ i = 0; i < size.ਉਚਾਈ; i++, src += sstep, dst += dstep ) \
        ਲਈ (ਪੂਰਨ j = 0; j < size.ਚੌੜਾਈ; j++) \
            dst[j] = saturate_cast<uchar>(std::ਨਿਰਪੇਖ((ਦਸ਼ਮਲਵ)src[j]*a + b)); \
}
DEF_CVT_SCALE_ABS_SCALAR_FUNC(8fe4m3u8u, fp8a_t)
DEF_CVT_SCALE_ABS_SCALAR_FUNC(8fe4m38u,  fp8_t)

ਦੁਆਧਾਰੀ_Func getCvtScaleAbsFunc(ਪੂਰਨ ਡੂੰਘਾਈ)
{
    ਦੁਆਧਾਰੀ_Func func =
        ਡੂੰਘਾਈ == CV_8F_E4M3FN ? (ਦੁਆਧਾਰੀ_Func)cvtScaleAbs8fe4m38u :
        ਡੂੰਘਾਈ == CV_8F_E4M3FNUZ ? (ਦੁਆਧਾਰੀ_Func)cvtScaleAbs8fe4m3u8u :
        ਡੂੰਘਾਈ == CV_8U ? (ਦੁਆਧਾਰੀ_Func)cvtScaleAbs8u :
        ਡੂੰਘਾਈ == CV_8S ? (ਦੁਆਧਾਰੀ_Func)cvtScaleAbs8s8u :
        ਡੂੰਘਾਈ == CV_Bool ? (ਦੁਆਧਾਰੀ_Func)cvtScaleAbs8b8u :
        ਡੂੰਘਾਈ == CV_16U ? (ਦੁਆਧਾਰੀ_Func)cvtScaleAbs16u8u :
        ਡੂੰਘਾਈ == CV_16S ? (ਦੁਆਧਾਰੀ_Func)cvtScaleAbs16s8u :
        ਡੂੰਘਾਈ == CV_16F ? (ਦੁਆਧਾਰੀ_Func)cvtScaleAbs16f8u :
        ਡੂੰਘਾਈ == CV_16BF ? (ਦੁਆਧਾਰੀ_Func)cvtScaleAbs16bf8u :
        ਡੂੰਘਾਈ == CV_32U ? (ਦੁਆਧਾਰੀ_Func)cvtScaleAbs32u8u :
        ਡੂੰਘਾਈ == CV_32S ? (ਦੁਆਧਾਰੀ_Func)cvtScaleAbs32s8u :
        ਡੂੰਘਾਈ == CV_32F ? (ਦੁਆਧਾਰੀ_Func)cvtScaleAbs32f8u :
        ਡੂੰਘਾਈ == CV_64U ? (ਦੁਆਧਾਰੀ_Func)cvtScaleAbs64u8u :
        ਡੂੰਘਾਈ == CV_64S ? (ਦੁਆਧਾਰੀ_Func)cvtScaleAbs64s8u :
        ਡੂੰਘਾਈ == CV_64F ? (ਦੁਆਧਾਰੀ_Func)cvtScaleAbs64f8u : 0;
    CV_Assert(func != 0);
    ਵਾਪਸ func;
}

//////////////////// FP8 scaled conversions — scalar via saturate_cast(src*a+b) ////////////////////
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ DEF_CVT_SCALE_SCALAR_FUNC(suffix, stype, dtype, wtype) \
ਸਥਿਰ ਖਾਲੀ cvtScale##suffix( ਅਚਲ uchar* src_, size_t sstep, ਅਚਲ uchar*, size_t, \
                              uchar* dst_, size_t dstep, ਮਾਪ size, ਖਾਲੀ* scale_) \
{ \
    ਅਚਲ stype* src = (ਅਚਲ stype*)src_; dtype* dst = (dtype*)dst_; \
    ਅਚਲ ਦੁੱਗਣਾ* scale = (ਅਚਲ ਦੁੱਗਣਾ*)scale_; \
    wtype a = (wtype)scale[0], b = (wtype)scale[1]; \
    sstep /= ਆਕਾਰ(src[0]); dstep /= ਆਕਾਰ(dst[0]); \
    ਲਈ( ਪੂਰਨ i = 0; i < size.ਉਚਾਈ; i++, src += sstep, dst += dstep ) \
        ਲਈ (ਪੂਰਨ j = 0; j < size.ਚੌੜਾਈ; j++) \
            dst[j] = saturate_cast<dtype>((wtype)src[j]*a + b); \
}

// double intermediate so coarse FP8 grids round identically to the (double-precision) reference
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ DEF_CVT_SCALE_FP8(S, T) \
    DEF_CVT_SCALE_SCALAR_FUNC(S##8u,   T, uchar, ਦੁੱਗਣਾ)     DEF_CVT_SCALE_SCALAR_FUNC(8u##S,   uchar, T, ਦੁੱਗਣਾ) \
    DEF_CVT_SCALE_SCALAR_FUNC(S##8s,   T, schar, ਦੁੱਗਣਾ)     DEF_CVT_SCALE_SCALAR_FUNC(8s##S,   schar, T, ਦੁੱਗਣਾ) \
    DEF_CVT_SCALE_SCALAR_FUNC(S##16u,  T, ushort, ਦੁੱਗਣਾ)    DEF_CVT_SCALE_SCALAR_FUNC(16u##S,  ushort, T, ਦੁੱਗਣਾ) \
    DEF_CVT_SCALE_SCALAR_FUNC(S##16s,  T, ਛੋਟਾ, ਦੁੱਗਣਾ)     DEF_CVT_SCALE_SCALAR_FUNC(16s##S,  ਛੋਟਾ, T, ਦੁੱਗਣਾ) \
    DEF_CVT_SCALE_SCALAR_FUNC(S##32u,  T, ਬਿਨਾਨਿਸ਼ਾਨੀ, ਦੁੱਗਣਾ)  DEF_CVT_SCALE_SCALAR_FUNC(32u##S,  ਬਿਨਾਨਿਸ਼ਾਨੀ, T, ਦੁੱਗਣਾ) \
    DEF_CVT_SCALE_SCALAR_FUNC(S##32s,  T, ਪੂਰਨ, ਦੁੱਗਣਾ)       DEF_CVT_SCALE_SCALAR_FUNC(32s##S,  ਪੂਰਨ, T, ਦੁੱਗਣਾ) \
    DEF_CVT_SCALE_SCALAR_FUNC(S##32f,  T, ਦਸ਼ਮਲਵ, ਦੁੱਗਣਾ) \
    DEF_CVT_SCALE_SCALAR_FUNC(S##64f,  T, ਦੁੱਗਣਾ, ਦੁੱਗਣਾ)    DEF_CVT_SCALE_SCALAR_FUNC(64f##S,  ਦੁੱਗਣਾ, T, ਦੁੱਗਣਾ) \
    DEF_CVT_SCALE_SCALAR_FUNC(S##16f,  T, hfloat, ਦੁੱਗਣਾ)    DEF_CVT_SCALE_SCALAR_FUNC(16f##S,  hfloat, T, ਦੁੱਗਣਾ) \
    DEF_CVT_SCALE_SCALAR_FUNC(S##16bf, T, bfloat, ਦੁੱਗਣਾ)    DEF_CVT_SCALE_SCALAR_FUNC(16bf##S, bfloat, T, ਦੁੱਗਣਾ) \
    DEF_CVT_SCALE_SCALAR_FUNC(S##64u,  T, uint64_t, ਦੁੱਗਣਾ)  DEF_CVT_SCALE_SCALAR_FUNC(64u##S,  uint64_t, T, ਦੁੱਗਣਾ) \
    DEF_CVT_SCALE_SCALAR_FUNC(S##64s,  T, int64_t, ਦੁੱਗਣਾ)   DEF_CVT_SCALE_SCALAR_FUNC(64s##S,  int64_t, T, ਦੁੱਗਣਾ) \
    DEF_CVT_SCALE_SCALAR_FUNC(S##8b,   T, ਤਾਰਕਿਕ, ਦੁੱਗਣਾ) \
    DEF_CVT_SCALE_SCALAR_FUNC(S##8fe4m3,  T, fp8_t, ਦੁੱਗਣਾ)   DEF_CVT_SCALE_SCALAR_FUNC(S##8fe4m3u, T, fp8a_t, ਦੁੱਗਣਾ)

DEF_CVT_SCALE_FP8(8fe4m3,  fp8_t)
DEF_CVT_SCALE_FP8(8fe4m3u, fp8a_t)

// convertTo(dst, depth, alpha, beta) path -- real quantization. Reuses convert.hpp's fast path.
// Double precision here matches the scalar reference (wtype)x*a+b bit-for-bit.
ਸਾਂਚਾ<ਪੂਰਨ bias, ਤਾਰਕਿਕ fnuz> ਸਥਿਰ ਖਾਲੀ
cvt_Scale_F32_ਮੰਜ਼ਿਲ_Fp8(ਅਚਲ uchar* src_, size_t sstep, ਅਚਲ uchar*, size_t,
                 uchar* dst, size_t dstep, ਮਾਪ size, ਖਾਲੀ* scale_)
{
    CV_INSTRUMENT_REGION();
    ਅਚਲ ਦਸ਼ਮਲਵ* src = (ਅਚਲ ਦਸ਼ਮਲਵ*)src_;
    ਅਚਲ ਦੁੱਗਣਾ* scale = (ਅਚਲ ਦੁੱਗਣਾ*)scale_;
    ਦੁੱਗਣਾ a = scale[0], b = scale[1];
    sstep /= ਆਕਾਰ(src[0]);

    ਲਈ (ਪੂਰਨ i = 0; i < size.ਉਚਾਈ; i++, src += sstep, dst += dstep)
    {
        ਪੂਰਨ j = 0;
#ਜੇ (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)
        v_float64 va = vx_setall_f64(a), vb = vx_setall_f64(b);
        ਅਚਲ ਪੂਰਨ LANES32 = VTraits<v_float32>::vlanes();
        ਅਚਲ ਪੂਰਨ VECSZ = LANES32 * 4;
        ਦਸ਼ਮਲਵ scaled[VTraits<v_float32>::max_nlanes * 4];
        ਲਈ (; j <= size.ਚੌੜਾਈ - VECSZ; j += VECSZ)
        {
            v_float32 vf0, vf1, vf2, vf3;
            v_float64 d0, d1;

            vx_ਲੱਦੋ_pair_as(src + j + 0 * LANES32, d0, d1);
            d0 = v_ਜੋੜੋ(v_mul(d0, va), vb);
            d1 = v_ਜੋੜੋ(v_mul(d1, va), vb);
            vf0 = v_cvt_f32(d0, d1);
            v_store(scaled + 0 * LANES32, vf0);

            vx_ਲੱਦੋ_pair_as(src + j + 1 * LANES32, d0, d1);
            d0 = v_ਜੋੜੋ(v_mul(d0, va), vb);
            d1 = v_ਜੋੜੋ(v_mul(d1, va), vb);
            vf1 = v_cvt_f32(d0, d1);
            v_store(scaled + 1 * LANES32, vf1);

            vx_ਲੱਦੋ_pair_as(src + j + 2 * LANES32, d0, d1);
            d0 = v_ਜੋੜੋ(v_mul(d0, va), vb);
            d1 = v_ਜੋੜੋ(v_mul(d1, va), vb);
            vf2 = v_cvt_f32(d0, d1);
            v_store(scaled + 2 * LANES32, vf2);

            vx_ਲੱਦੋ_pair_as(src + j + 3 * LANES32, d0, d1);
            d0 = v_ਜੋੜੋ(v_mul(d0, va), vb);
            d1 = v_ਜੋੜੋ(v_mul(d1, va), vb);
            vf3 = v_cvt_f32(d0, d1);
            v_store(scaled + 3 * LANES32, vf3);

            encodeFp8Vec4<bias, fnuz>(vf0, vf1, vf2, vf3, scaled, VECSZ, dst + j);
        }
        vx_cleanup();
#ਜੇ_ਅੰਤ
        ਲਈ (; j < size.ਚੌੜਾਈ; j++)
            dst[j] = fp8_detail::encodeE4M3((ਦਸ਼ਮਲਵ)((ਦੁੱਗਣਾ)src[j]*a + b), bias, fnuz);
    }
}
ਸਥਿਰ ਖਾਲੀ cvtScale32f8fe4m3(ਅਚਲ uchar* s, size_t ss, ਅਚਲ uchar* p, size_t ps, uchar* d, size_t ds, ਮਾਪ sz, ਖਾਲੀ* x)
{ cvt_Scale_F32_ਮੰਜ਼ਿਲ_Fp8<7, ਝੂਠ>(s, ss, p, ps, d, ds, sz, x); }
ਸਥਿਰ ਖਾਲੀ cvtScale32f8fe4m3u(ਅਚਲ uchar* s, size_t ss, ਅਚਲ uchar* p, size_t ps, uchar* d, size_t ds, ਮਾਪ sz, ਖਾਲੀ* x)
{ cvt_Scale_F32_ਮੰਜ਼ਿਲ_Fp8<8, ਸੱਚ>(s, ss, p, ps, d, ds, sz, x); }

ਦੁਆਧਾਰੀ_Func getConvertScaleFunc(ਪੂਰਨ sdepth_, ਪੂਰਨ ddepth_)
{
    ਪੂਰਨ sdepth = CV_MAT_ਡੂੰਘਾਈ(sdepth_);
    ਪੂਰਨ ddepth = CV_MAT_ਡੂੰਘਾਈ(ddepth_);
    ਦੁਆਧਾਰੀ_Func func =
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
    ਵਾਪਸ func;
}

#ਜੇ_ਅੰਤ

CV_CPU_OPTIMIZATION_NAMESPACE_ਸਿਰਾ
} // namespace
