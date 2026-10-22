// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


#அந்தர்பாவ "precomp.hpp"
#அந்தர்பாவ "convert.hpp"

நாமாகாஶ cv {
CV_CPU_OPTIMIZATION_NAMESPACE_BEGIN

இருமம்_Func getCvtScaleAbsFunc(முழு depth);
இருமம்_Func getConvertScaleFunc(முழு sdepth, முழு ddepth);

#ifndef CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

/****************************************************************************************\
*                                convertScale[Abs]                                       *
\****************************************************************************************/

template<typename _Ts, typename _Td> உள்வரி வெறுமை
cvtabs_32f( மாறிலி _Ts* src, size_t sstep, _Td* dst, size_t dstep,
            Size size, தசம a, தசம b )
{
#எனில் (CV_SIMD || CV_SIMD_SCALABLE)
    v_float32 va = vx_setall_f32(a), vb = vx_setall_f32(b);
    மாறிலி முழு VECSZ = VTraits<v_float32>::vlanes()*2;
    // GCC miscompiles this scalable block only on VLEN=128 RVV (opencv/issues/26936);
    // use scalar there, keep SIMD on wider vectors. Ask the hardware for VLEN directly:
    // vlanes() would depend on the LMUL the universal intrinsics happen to use.
#எனில் (CV_RVV && CV_SIMD_SCALABLE && defined(__GNUC__) && !defined(__clang__))
    மாறிலி தருக்க useSIMD = __riscv_vlenb()*8 != 128;
#இல்லையெனில்
    மாறிலி தருக்க useSIMD = உண்மை;
#endif
#endif
    sstep /= அளவு(src[0]);
    dstep /= அளவு(dst[0]);

    க்கு( முழு i = 0; i < size.height; i++, src += sstep, dst += dstep )
    {
        முழு j = 0;
#எனில் (CV_SIMD || CV_SIMD_SCALABLE)
        எனில்( useSIMD )
        க்கு( ; j < size.width; j += VECSZ )
        {
            எனில்( j > size.width - VECSZ )
            {
                எனில்( j == 0 || src == (_Ts*)dst )
                    நிறுத்து;
                j = size.width - VECSZ;
            }
            v_float32 v0, v1;
            vx_load_pair_as(src + j, v0, v1);
            v0 = v_fma(v0, va, vb);
            v1 = v_fma(v1, va, vb);
            v_store_pair_as(dst + j, v_abs(v0), v_abs(v1));
        }
#endif
        க்கு( ; j < size.width; j++ )
            dst[j] = saturate_cast<_Td>(std::abs(src[j]*a + b));
    }
}

நிலையான வெறுமை
cvtabs_32f( மாறிலி தருக்க* src_, size_t sstep,
            uchar* dst, size_t dstep,
            Size size, தசம a, தசம b )
{
    மாறிலி uchar* src = (மாறிலி uchar*)src_;
    uchar v0 = saturate_cast<uchar>(std::abs(b));
    uchar v1 = saturate_cast<uchar>(std::abs(a + b));
    க்கு( முழு i = 0; i < size.height; i++, src += sstep, dst += dstep )
    {
        க்கு (முழு j = 0; j < size.width; j++)
            dst[j] = src[j] != 0 ? v1 : v0;
    }
}

template<typename _Ts, typename _Td> உள்வரி வெறுமை
cvt_32f( மாறிலி _Ts* src, size_t sstep, _Td* dst, size_t dstep,
         Size size, தசம a, தசம b )
{
#எனில் (CV_SIMD || (CV_SIMD_SCALABLE && !(defined(__GNUC__) && !defined(__clang__))) )
    v_float32 va = vx_setall_f32(a), vb = vx_setall_f32(b);
    மாறிலி முழு VECSZ = VTraits<v_float32>::vlanes()*2;
#endif
    sstep /= அளவு(src[0]);
    dstep /= அளவு(dst[0]);

    க்கு( முழு i = 0; i < size.height; i++, src += sstep, dst += dstep )
    {
        முழு j = 0;
// Excluding GNU in CV_SIMD_SCALABLE because of "opencv/issues/26936"
#எனில் (CV_SIMD || (CV_SIMD_SCALABLE && !(defined(__GNUC__) && !defined(__clang__))) )
        க்கு( ; j < size.width; j += VECSZ )
        {
            எனில்( j > size.width - VECSZ )
            {
                எனில்( j == 0 || src == (_Ts*)dst )
                    நிறுத்து;
                j = size.width - VECSZ;
            }
            v_float32 v0, v1;
            vx_load_pair_as(src + j, v0, v1);
            v0 = v_fma(v0, va, vb);
            v1 = v_fma(v1, va, vb);
            v_store_pair_as(dst + j, v0, v1);
        }
#endif
        க்கு( ; j < size.width; j++ )
            dst[j] = saturate_cast<_Td>(src[j]*a + b);
    }
}

// variant for conversions 16f <-> ... w/o unrolling
template<typename _Ts, typename _Td> உள்வரி வெறுமை
cvt1_32f( மாறிலி _Ts* src, size_t sstep, _Td* dst, size_t dstep,
          Size size, தசம a, தசம b )
{
#எனில் (CV_SIMD || CV_SIMD_SCALABLE)
    v_float32 va = vx_setall_f32(a), vb = vx_setall_f32(b);
    மாறிலி முழு VECSZ = VTraits<v_float32>::vlanes();
#endif
    sstep /= அளவு(src[0]);
    dstep /= அளவு(dst[0]);

    க்கு( முழு i = 0; i < size.height; i++, src += sstep, dst += dstep )
    {
        முழு j = 0;
#எனில் (CV_SIMD || CV_SIMD_SCALABLE)
        க்கு( ; j < size.width; j += VECSZ )
        {
            எனில்( j > size.width - VECSZ )
            {
                எனில்( j == 0 || src == (_Ts*)dst )
                    நிறுத்து;
                j = size.width - VECSZ;
            }
            v_float32 v0;
            vx_load_as(src + j, v0);
            v0 = v_fma(v0, va, vb);
            v_store_as(dst + j, v0);
        }
#endif
        க்கு( ; j < size.width; j++ )
            dst[j] = saturate_cast<_Td>(src[j]*a + b);
    }
}


template<typename _Ts, typename _Td> உள்வரி வெறுமை
cvt_64f( மாறிலி _Ts* src, size_t sstep, _Td* dst, size_t dstep,
         Size size, இரட்டை a, இரட்டை b )
{
#எனில் (CV_SIMD_64F || (CV_SIMD_SCALABLE_64F && !(defined(__GNUC__) && !defined(__clang__))) )
    v_float64 va = vx_setall_f64(a), vb = vx_setall_f64(b);
    மாறிலி முழு VECSZ = VTraits<v_float64>::vlanes()*2;
#endif
    sstep /= அளவு(src[0]);
    dstep /= அளவு(dst[0]);

    க்கு( முழு i = 0; i < size.height; i++, src += sstep, dst += dstep )
    {
        முழு j = 0;
// Excluding GNU in CV_SIMD_SCALABLE because of "opencv/issues/26936"
#எனில் (CV_SIMD_64F || (CV_SIMD_SCALABLE_64F && !(defined(__GNUC__) && !defined(__clang__))) )
        க்கு( ; j < size.width; j += VECSZ )
        {
            எனில்( j > size.width - VECSZ )
            {
                எனில்( j == 0 || src == (_Ts*)dst )
                    நிறுத்து;
                j = size.width - VECSZ;
            }
            v_float64 v0, v1;
            vx_load_pair_as(src + j, v0, v1);
            v0 = v_fma(v0, va, vb);
            v1 = v_fma(v1, va, vb);
            v_store_pair_as(dst + j, v0, v1);
        }
#endif
        க்கு( ; j < size.width; j++ )
            dst[j] = saturate_cast<_Td>(src[j]*a + b);
    }
}

//==================================================================================================

#define DEF_CVT_SCALE_ABS_FUNC(suffix, cvt, stype, dtype, wtype) \
நிலையான வெறுமை cvtScaleAbs##suffix( மாறிலி uchar* src_, size_t sstep, மாறிலி uchar*, size_t, \
                                 uchar* dst_, size_t dstep, Size size, வெறுமை* scale_) \
{ \
    மாறிலி stype* src = (மாறிலி stype*)src_; \
    dtype* dst = (dtype*)dst_; \
    இரட்டை* scale = (இரட்டை*)scale_; \
    cvt(src, sstep, dst, dstep, size, (wtype)scale[0], (wtype)scale[1]); \
}


#define DEF_CVT_SCALE_FUNC(suffix, cvt, stype, dtype, wtype) \
நிலையான வெறுமை cvtScale##suffix( மாறிலி uchar* src_, size_t sstep, மாறிலி uchar*, size_t, \
                              uchar* dst_, size_t dstep, Size size, வெறுமை* scale_) \
{ \
    மாறிலி stype* src = (மாறிலி stype*)src_; \
    dtype* dst = (dtype*)dst_; \
    இரட்டை* scale = (இரட்டை*)scale_; \
    cvt(src, sstep, dst, dstep, size, (wtype)scale[0], (wtype)scale[1]); \
}

#define DEF_CVT_SCALE2BOOL_FUNC(suffix, stype, wtype) \
நிலையான வெறுமை cvtScale##suffix( மாறிலி uchar* src_, size_t sstep, மாறிலி uchar*, size_t, \
                              uchar* dst, size_t dstep, Size size, வெறுமை* scale_) \
{ \
    மாறிலி stype* src = (மாறிலி stype*)src_; \
    மாறிலி இரட்டை* scale = (மாறிலி இரட்டை*)scale_; \
    wtype a = (wtype)scale[0], b = (wtype)scale[1]; \
    sstep /= அளவு(src[0]); \
    க்கு( முழு i = 0; i < size.height; i++, src += sstep, dst += dstep ) \
        க்கு (முழு j = 0; j < size.width; j++) \
            dst[j] = (தருக்க)((wtype)src[j]*a + b != 0); \
}

#define DEF_CVT_SCALEBOOL2_FUNC(suffix, dtype, wtype) \
நிலையான வெறுமை cvtScale##suffix( மாறிலி uchar* src, size_t sstep, மாறிலி uchar*, size_t, \
                              uchar* dst_, size_t dstep, Size size, வெறுமை* scale_) \
{ \
    dtype* dst = (dtype*)dst_; \
    மாறிலி இரட்டை* scale = (மாறிலி இரட்டை*)scale_; \
    wtype a = (wtype)scale[0], b = (wtype)scale[1]; \
    dtype v0 = saturate_cast<dtype>(b), v1 = saturate_cast<dtype>(a + b); \
    dstep /= அளவு(dst[0]); \
    க்கு( முழு i = 0; i < size.height; i++, src += sstep, dst += dstep ) \
        க்கு (முழு j = 0; j < size.width; j++) \
            dst[j] = src[j] != 0 ? v1 : v0; \
}

DEF_CVT_SCALE_ABS_FUNC(8u,    cvtabs_32f, uchar,  uchar, தசம)
DEF_CVT_SCALE_ABS_FUNC(8s8u,  cvtabs_32f, schar,  uchar, தசம)
DEF_CVT_SCALE_ABS_FUNC(8b8u,  cvtabs_32f, தருக்க,  uchar, தசம)
DEF_CVT_SCALE_ABS_FUNC(16u8u, cvtabs_32f, ushort, uchar, தசம)
DEF_CVT_SCALE_ABS_FUNC(16s8u, cvtabs_32f, short,  uchar, தசம)
DEF_CVT_SCALE_ABS_FUNC(32u8u, cvtabs_32f, unsigned, uchar, தசம)
DEF_CVT_SCALE_ABS_FUNC(32s8u, cvtabs_32f, முழு,    uchar, தசம)
DEF_CVT_SCALE_ABS_FUNC(32f8u, cvtabs_32f, தசம,  uchar, தசம)
DEF_CVT_SCALE_ABS_FUNC(64u8u, cvtabs_32f, uint64_t, uchar, தசம)
DEF_CVT_SCALE_ABS_FUNC(64s8u, cvtabs_32f, int64_t, uchar, தசம)
DEF_CVT_SCALE_ABS_FUNC(64f8u, cvtabs_32f, இரட்டை, uchar, தசம)
DEF_CVT_SCALE_ABS_FUNC(16f8u, cvtabs_32f, hfloat, uchar, தசம)
DEF_CVT_SCALE_ABS_FUNC(16bf8u, cvtabs_32f, bfloat, uchar, தசம)

DEF_CVT_SCALE_FUNC(8u,     cvt_32f, uchar,  uchar, தசம)
DEF_CVT_SCALE_FUNC(8s8u,   cvt_32f, schar,  uchar, தசம)
DEF_CVT_SCALE_FUNC(16u8u,  cvt_32f, ushort, uchar, தசம)
DEF_CVT_SCALE_FUNC(16s8u,  cvt_32f, short,  uchar, தசம)
DEF_CVT_SCALE_FUNC(32u8u,  cvt_32f, unsigned, uchar, தசம)
DEF_CVT_SCALE_FUNC(32s8u,  cvt_32f, முழு,    uchar, தசம)
DEF_CVT_SCALE_FUNC(32f8u,  cvt_32f, தசம,  uchar, தசம)
DEF_CVT_SCALE_FUNC(64f8u,  cvt_32f, இரட்டை, uchar, தசம)
DEF_CVT_SCALE_FUNC(64u8u,  cvt_32f, uint64_t, uchar, தசம)
DEF_CVT_SCALE_FUNC(64s8u,  cvt_32f, int64_t, uchar, தசம)
DEF_CVT_SCALE_FUNC(16f8u,  cvt_32f, hfloat, uchar, தசம)
DEF_CVT_SCALE_FUNC(16bf8u, cvt_32f, bfloat, uchar, தசம)

DEF_CVT_SCALE_FUNC(8u8s,   cvt_32f, uchar,  schar, தசம)
DEF_CVT_SCALE_FUNC(8s,     cvt_32f, schar,  schar, தசம)
DEF_CVT_SCALE_FUNC(16u8s,  cvt_32f, ushort, schar, தசம)
DEF_CVT_SCALE_FUNC(16s8s,  cvt_32f, short,  schar, தசம)
DEF_CVT_SCALE_FUNC(32u8s,  cvt_32f, unsigned, schar, தசம)
DEF_CVT_SCALE_FUNC(32s8s,  cvt_32f, முழு,    schar, தசம)
DEF_CVT_SCALE_FUNC(32f8s,  cvt_32f, தசம,  schar, தசம)
DEF_CVT_SCALE_FUNC(64f8s,  cvt_32f, இரட்டை, schar, தசம)
DEF_CVT_SCALE_FUNC(64u8s,  cvt_32f, uint64_t, schar, தசம)
DEF_CVT_SCALE_FUNC(64s8s,  cvt_32f, int64_t, schar, தசம)
DEF_CVT_SCALE_FUNC(16f8s,  cvt_32f, hfloat, schar, தசம)
DEF_CVT_SCALE_FUNC(16bf8s, cvt_32f, bfloat, schar, தசம)

DEF_CVT_SCALE2BOOL_FUNC(8u8b, uchar, தசம)
DEF_CVT_SCALE2BOOL_FUNC(8s8b, schar, தசம)
DEF_CVT_SCALE2BOOL_FUNC(16u8b, ushort, தசம)
DEF_CVT_SCALE2BOOL_FUNC(16s8b, short, தசம)
DEF_CVT_SCALE2BOOL_FUNC(32u8b, unsigned, தசம)
DEF_CVT_SCALE2BOOL_FUNC(32s8b, முழு, தசம)
DEF_CVT_SCALE2BOOL_FUNC(32f8b, தசம, தசம)
DEF_CVT_SCALE2BOOL_FUNC(64f8b, இரட்டை, தசம)
DEF_CVT_SCALE2BOOL_FUNC(64u8b, uint64_t, தசம)
DEF_CVT_SCALE2BOOL_FUNC(64s8b, int64_t, தசம)
DEF_CVT_SCALE2BOOL_FUNC(16f8b, hfloat, தசம)
DEF_CVT_SCALE2BOOL_FUNC(16bf8b, bfloat, தசம)

DEF_CVT_SCALE_FUNC(8u16u,  cvt_32f, uchar,  ushort, தசம)
DEF_CVT_SCALE_FUNC(8s16u,  cvt_32f, schar,  ushort, தசம)
DEF_CVT_SCALE_FUNC(16u,    cvt_32f, ushort, ushort, தசம)
DEF_CVT_SCALE_FUNC(16s16u, cvt_32f, short,  ushort, தசம)
DEF_CVT_SCALE_FUNC(32u16u, cvt_32f, unsigned, ushort, தசம)
DEF_CVT_SCALE_FUNC(32s16u, cvt_32f, முழு,    ushort, தசம)
DEF_CVT_SCALE_FUNC(32f16u, cvt_32f, தசம,  ushort, தசம)
DEF_CVT_SCALE_FUNC(64f16u, cvt_32f, இரட்டை, ushort, தசம)
DEF_CVT_SCALE_FUNC(64u16u, cvt_32f, uint64_t, ushort, தசம)
DEF_CVT_SCALE_FUNC(64s16u, cvt_32f, int64_t, ushort, தசம)
DEF_CVT_SCALE_FUNC(16f16u, cvt1_32f, hfloat, ushort, தசம)
DEF_CVT_SCALE_FUNC(16bf16u, cvt1_32f, bfloat, ushort, தசம)

DEF_CVT_SCALE_FUNC(8u16s,  cvt_32f, uchar,  short, தசம)
DEF_CVT_SCALE_FUNC(8s16s,  cvt_32f, schar,  short, தசம)
DEF_CVT_SCALE_FUNC(16u16s, cvt_32f, ushort, short, தசம)
DEF_CVT_SCALE_FUNC(16s,    cvt_32f, short,  short, தசம)
DEF_CVT_SCALE_FUNC(32u16s, cvt_32f, unsigned, short, தசம)
DEF_CVT_SCALE_FUNC(32s16s, cvt_32f, முழு,    short, தசம)
DEF_CVT_SCALE_FUNC(32f16s, cvt_32f, தசம,  short, தசம)
DEF_CVT_SCALE_FUNC(64f16s, cvt_32f, இரட்டை, short, தசம)
DEF_CVT_SCALE_FUNC(64u16s, cvt_32f, uint64_t, short, தசம)
DEF_CVT_SCALE_FUNC(64s16s, cvt_32f, int64_t, short, தசம)
DEF_CVT_SCALE_FUNC(16f16s, cvt1_32f, hfloat, short, தசம)
DEF_CVT_SCALE_FUNC(16bf16s, cvt1_32f, bfloat, short, தசம)

DEF_CVT_SCALE_FUNC(8u32u,  cvt_32f, uchar,  unsigned, தசம)
DEF_CVT_SCALE_FUNC(8s32u,  cvt_32f, schar,  unsigned, தசம)
DEF_CVT_SCALE_FUNC(16u32u, cvt_32f, ushort, unsigned, தசம)
DEF_CVT_SCALE_FUNC(16s32u, cvt_32f, short,  unsigned, தசம)
DEF_CVT_SCALE_FUNC(32u, cvt_32f, unsigned, unsigned, தசம)
DEF_CVT_SCALE_FUNC(32s32u, cvt_64f, முழு,    unsigned, இரட்டை)
DEF_CVT_SCALE_FUNC(32f32u, cvt_32f, தசம,  unsigned, தசம)
DEF_CVT_SCALE_FUNC(64f32u, cvt_64f, இரட்டை, unsigned, இரட்டை)
DEF_CVT_SCALE_FUNC(64u32u, cvt_32f, uint64_t, unsigned, தசம)
DEF_CVT_SCALE_FUNC(64s32u, cvt_32f, int64_t, unsigned, தசம)
DEF_CVT_SCALE_FUNC(16f32u, cvt1_32f, hfloat, unsigned, தசம)
DEF_CVT_SCALE_FUNC(16bf32u, cvt1_32f, bfloat, unsigned, தசம)

DEF_CVT_SCALE_FUNC(8u32s,  cvt_32f, uchar,  முழு, தசம)
DEF_CVT_SCALE_FUNC(8s32s,  cvt_32f, schar,  முழு, தசம)
DEF_CVT_SCALE_FUNC(16u32s, cvt_32f, ushort, முழு, தசம)
DEF_CVT_SCALE_FUNC(16s32s, cvt_32f, short,  முழு, தசம)
DEF_CVT_SCALE_FUNC(32u32s, cvt_32f, unsigned, முழு, தசம)
DEF_CVT_SCALE_FUNC(32s,    cvt_64f, முழு,    முழு, இரட்டை)
DEF_CVT_SCALE_FUNC(32f32s, cvt_32f, தசம,  முழு, தசம)
DEF_CVT_SCALE_FUNC(64f32s, cvt_64f, இரட்டை, முழு, இரட்டை)
DEF_CVT_SCALE_FUNC(64u32s, cvt_32f, uint64_t, முழு, தசம)
DEF_CVT_SCALE_FUNC(64s32s, cvt_32f, int64_t, முழு, தசம)
DEF_CVT_SCALE_FUNC(16f32s, cvt1_32f, hfloat, முழு, தசம)
DEF_CVT_SCALE_FUNC(16bf32s, cvt1_32f, bfloat, முழு, தசம)

DEF_CVT_SCALE_FUNC(8u32f,  cvt_32f, uchar,  தசம, தசம)
DEF_CVT_SCALE_FUNC(8s32f,  cvt_32f, schar,  தசம, தசம)
DEF_CVT_SCALE_FUNC(16u32f, cvt_32f, ushort, தசம, தசம)
DEF_CVT_SCALE_FUNC(16s32f, cvt_32f, short,  தசம, தசம)
DEF_CVT_SCALE_FUNC(32u32f, cvt_32f, unsigned, தசம, தசம)
DEF_CVT_SCALE_FUNC(32s32f, cvt_32f, முழு,    தசம, தசம)
DEF_CVT_SCALE_FUNC(32f,    cvt_32f, தசம,  தசம, தசம)
DEF_CVT_SCALE_FUNC(64f32f, cvt_64f, இரட்டை, தசம, இரட்டை)
DEF_CVT_SCALE_FUNC(64u32f, cvt_32f, uint64_t, தசம, தசம)
DEF_CVT_SCALE_FUNC(64s32f, cvt_32f, int64_t, தசம, தசம)
DEF_CVT_SCALE_FUNC(16f32f, cvt1_32f, hfloat, தசம, தசம)
DEF_CVT_SCALE_FUNC(16bf32f, cvt1_32f, bfloat, தசம, தசம)

DEF_CVT_SCALE_FUNC(8u64f,  cvt_64f, uchar,  இரட்டை, இரட்டை)
DEF_CVT_SCALE_FUNC(8s64f,  cvt_64f, schar,  இரட்டை, இரட்டை)
DEF_CVT_SCALE_FUNC(16u64f, cvt_64f, ushort, இரட்டை, இரட்டை)
DEF_CVT_SCALE_FUNC(16s64f, cvt_64f, short,  இரட்டை, இரட்டை)
DEF_CVT_SCALE_FUNC(32u64f, cvt_64f, unsigned, இரட்டை, இரட்டை)
DEF_CVT_SCALE_FUNC(32s64f, cvt_64f, முழு,    இரட்டை, இரட்டை)
DEF_CVT_SCALE_FUNC(32f64f, cvt_64f, தசம,  இரட்டை, இரட்டை)
DEF_CVT_SCALE_FUNC(64f,    cvt_64f, இரட்டை, இரட்டை, இரட்டை)
DEF_CVT_SCALE_FUNC(64u64f, cvt_64f, uint64_t, இரட்டை, இரட்டை)
DEF_CVT_SCALE_FUNC(64s64f, cvt_64f, int64_t, இரட்டை, இரட்டை)
DEF_CVT_SCALE_FUNC(16f64f, cvt_64f, hfloat, இரட்டை, இரட்டை)
DEF_CVT_SCALE_FUNC(16bf64f, cvt_64f, bfloat, இரட்டை, இரட்டை)

DEF_CVT_SCALE_FUNC(8u64u,  cvt_64f, uchar,  uint64_t, இரட்டை)
DEF_CVT_SCALE_FUNC(8s64u,  cvt_64f, schar,  uint64_t, இரட்டை)
DEF_CVT_SCALE_FUNC(16u64u, cvt_64f, ushort, uint64_t, இரட்டை)
DEF_CVT_SCALE_FUNC(16s64u, cvt_64f, short,  uint64_t, இரட்டை)
DEF_CVT_SCALE_FUNC(32u64u, cvt_64f, unsigned, uint64_t, இரட்டை)
DEF_CVT_SCALE_FUNC(32s64u, cvt_64f, முழு,    uint64_t, இரட்டை)
DEF_CVT_SCALE_FUNC(32f64u, cvt_64f, தசம,  uint64_t, இரட்டை)
DEF_CVT_SCALE_FUNC(64f64u, cvt_64f, இரட்டை, uint64_t, இரட்டை)
DEF_CVT_SCALE_FUNC(64u, cvt_64f, uint64_t, uint64_t, இரட்டை)
DEF_CVT_SCALE_FUNC(64s64u, cvt_64f, int64_t, uint64_t, இரட்டை)
DEF_CVT_SCALE_FUNC(16f64u, cvt_64f, hfloat, uint64_t, இரட்டை)
DEF_CVT_SCALE_FUNC(16bf64u, cvt_64f, bfloat, uint64_t, இரட்டை)

DEF_CVT_SCALE_FUNC(8u64s,  cvt_64f, uchar,  int64_t, இரட்டை)
DEF_CVT_SCALE_FUNC(8s64s,  cvt_64f, schar,  int64_t, இரட்டை)
DEF_CVT_SCALE_FUNC(16u64s, cvt_64f, ushort, int64_t, இரட்டை)
DEF_CVT_SCALE_FUNC(16s64s, cvt_64f, short,  int64_t, இரட்டை)
DEF_CVT_SCALE_FUNC(32u64s, cvt_64f, unsigned, int64_t, இரட்டை)
DEF_CVT_SCALE_FUNC(32s64s, cvt_64f, முழு,    int64_t, இரட்டை)
DEF_CVT_SCALE_FUNC(32f64s, cvt_64f, தசம,  int64_t, இரட்டை)
DEF_CVT_SCALE_FUNC(64f64s, cvt_64f, இரட்டை, int64_t, இரட்டை)
DEF_CVT_SCALE_FUNC(64u64s, cvt_64f, uint64_t, int64_t, இரட்டை)
DEF_CVT_SCALE_FUNC(64s, cvt_64f, int64_t, int64_t, இரட்டை)
DEF_CVT_SCALE_FUNC(16f64s, cvt_64f, hfloat, int64_t, இரட்டை)
DEF_CVT_SCALE_FUNC(16bf64s, cvt_64f, bfloat, int64_t, இரட்டை)

DEF_CVT_SCALE_FUNC(8u16f,  cvt1_32f, uchar,  hfloat, தசம)
DEF_CVT_SCALE_FUNC(8s16f,  cvt1_32f, schar,  hfloat, தசம)
DEF_CVT_SCALE_FUNC(16u16f, cvt1_32f, ushort, hfloat, தசம)
DEF_CVT_SCALE_FUNC(16s16f, cvt1_32f, short,  hfloat, தசம)
DEF_CVT_SCALE_FUNC(32u16f, cvt1_32f, unsigned, hfloat, தசம)
DEF_CVT_SCALE_FUNC(32s16f, cvt1_32f, முழு,    hfloat, தசம)
DEF_CVT_SCALE_FUNC(32f16f, cvt1_32f, தசம,  hfloat, தசம)
DEF_CVT_SCALE_FUNC(64f16f, cvt1_32f, இரட்டை, hfloat, தசம)
DEF_CVT_SCALE_FUNC(64u16f, cvt1_32f, uint64_t, hfloat, தசம)
DEF_CVT_SCALE_FUNC(64s16f, cvt1_32f, int64_t, hfloat, தசம)
DEF_CVT_SCALE_FUNC(16f,    cvt1_32f, hfloat, hfloat, தசம)
DEF_CVT_SCALE_FUNC(16bf16f, cvt1_32f, bfloat, hfloat, தசம)

DEF_CVT_SCALE_FUNC(8u16bf,  cvt1_32f, uchar,  bfloat, தசம)
DEF_CVT_SCALE_FUNC(8s16bf,  cvt1_32f, schar,  bfloat, தசம)
DEF_CVT_SCALE_FUNC(16u16bf, cvt1_32f, ushort, bfloat, தசம)
DEF_CVT_SCALE_FUNC(16s16bf, cvt1_32f, short,  bfloat, தசம)
DEF_CVT_SCALE_FUNC(32u16bf, cvt1_32f, unsigned, bfloat, தசம)
DEF_CVT_SCALE_FUNC(32s16bf, cvt1_32f, முழு,    bfloat, தசம)
DEF_CVT_SCALE_FUNC(32f16bf, cvt1_32f, தசம,  bfloat, தசம)
DEF_CVT_SCALE_FUNC(64f16bf, cvt1_32f, இரட்டை, bfloat, தசம)
DEF_CVT_SCALE_FUNC(64u16bf, cvt1_32f, uint64_t, bfloat, தசம)
DEF_CVT_SCALE_FUNC(64s16bf, cvt1_32f, int64_t, bfloat, தசம)
DEF_CVT_SCALE_FUNC(16f16bf, cvt1_32f, hfloat, bfloat, தசம)
DEF_CVT_SCALE_FUNC(16bf, cvt1_32f, bfloat, bfloat, தசம)

DEF_CVT_SCALEBOOL2_FUNC(8b8u, uchar, தசம)
DEF_CVT_SCALEBOOL2_FUNC(8b8s, schar, தசம)
DEF_CVT_SCALEBOOL2_FUNC(8b, தருக்க, தசம)
DEF_CVT_SCALEBOOL2_FUNC(8b16u, ushort, தசம)
DEF_CVT_SCALEBOOL2_FUNC(8b16s, short, தசம)
DEF_CVT_SCALEBOOL2_FUNC(8b32u, unsigned, தசம)
DEF_CVT_SCALEBOOL2_FUNC(8b32s, முழு, தசம)
DEF_CVT_SCALEBOOL2_FUNC(8b32f, தசம, தசம)
DEF_CVT_SCALEBOOL2_FUNC(8b64u, uint64_t, இரட்டை)
DEF_CVT_SCALEBOOL2_FUNC(8b64s, int64_t, இரட்டை)
DEF_CVT_SCALEBOOL2_FUNC(8b64f, இரட்டை, இரட்டை)
DEF_CVT_SCALEBOOL2_FUNC(8b16f, hfloat, தசம)
DEF_CVT_SCALEBOOL2_FUNC(8b16bf, bfloat, தசம)

// FP8 src -> CV_8U abs|scale|shift (scalar)
#define DEF_CVT_SCALE_ABS_SCALAR_FUNC(suffix, stype) \
நிலையான வெறுமை cvtScaleAbs##suffix( மாறிலி uchar* src_, size_t sstep, மாறிலி uchar*, size_t, \
                                 uchar* dst, size_t dstep, Size size, வெறுமை* scale_) \
{ \
    மாறிலி stype* src = (மாறிலி stype*)src_; \
    மாறிலி இரட்டை* scale = (மாறிலி இரட்டை*)scale_; \
    தசம a = (தசம)scale[0], b = (தசம)scale[1]; \
    sstep /= அளவு(src[0]); \
    க்கு( முழு i = 0; i < size.height; i++, src += sstep, dst += dstep ) \
        க்கு (முழு j = 0; j < size.width; j++) \
            dst[j] = saturate_cast<uchar>(std::abs((தசம)src[j]*a + b)); \
}
DEF_CVT_SCALE_ABS_SCALAR_FUNC(8fe4m3u8u, fp8a_t)
DEF_CVT_SCALE_ABS_SCALAR_FUNC(8fe4m38u,  fp8_t)

இருமம்_Func getCvtScaleAbsFunc(முழு depth)
{
    இருமம்_Func func =
        depth == CV_8F_E4M3FN ? (இருமம்_Func)cvtScaleAbs8fe4m38u :
        depth == CV_8F_E4M3FNUZ ? (இருமம்_Func)cvtScaleAbs8fe4m3u8u :
        depth == CV_8U ? (இருமம்_Func)cvtScaleAbs8u :
        depth == CV_8S ? (இருமம்_Func)cvtScaleAbs8s8u :
        depth == CV_Bool ? (இருமம்_Func)cvtScaleAbs8b8u :
        depth == CV_16U ? (இருமம்_Func)cvtScaleAbs16u8u :
        depth == CV_16S ? (இருமம்_Func)cvtScaleAbs16s8u :
        depth == CV_16F ? (இருமம்_Func)cvtScaleAbs16f8u :
        depth == CV_16BF ? (இருமம்_Func)cvtScaleAbs16bf8u :
        depth == CV_32U ? (இருமம்_Func)cvtScaleAbs32u8u :
        depth == CV_32S ? (இருமம்_Func)cvtScaleAbs32s8u :
        depth == CV_32F ? (இருமம்_Func)cvtScaleAbs32f8u :
        depth == CV_64U ? (இருமம்_Func)cvtScaleAbs64u8u :
        depth == CV_64S ? (இருமம்_Func)cvtScaleAbs64s8u :
        depth == CV_64F ? (இருமம்_Func)cvtScaleAbs64f8u : 0;
    CV_Assert(func != 0);
    திரும்பு func;
}

//////////////////// FP8 scaled conversions — scalar via saturate_cast(src*a+b) ////////////////////
#define DEF_CVT_SCALE_SCALAR_FUNC(suffix, stype, dtype, wtype) \
நிலையான வெறுமை cvtScale##suffix( மாறிலி uchar* src_, size_t sstep, மாறிலி uchar*, size_t, \
                              uchar* dst_, size_t dstep, Size size, வெறுமை* scale_) \
{ \
    மாறிலி stype* src = (மாறிலி stype*)src_; dtype* dst = (dtype*)dst_; \
    மாறிலி இரட்டை* scale = (மாறிலி இரட்டை*)scale_; \
    wtype a = (wtype)scale[0], b = (wtype)scale[1]; \
    sstep /= அளவு(src[0]); dstep /= அளவு(dst[0]); \
    க்கு( முழு i = 0; i < size.height; i++, src += sstep, dst += dstep ) \
        க்கு (முழு j = 0; j < size.width; j++) \
            dst[j] = saturate_cast<dtype>((wtype)src[j]*a + b); \
}

// double intermediate so coarse FP8 grids round identically to the (double-precision) reference
#define DEF_CVT_SCALE_FP8(S, T) \
    DEF_CVT_SCALE_SCALAR_FUNC(S##8u,   T, uchar, இரட்டை)     DEF_CVT_SCALE_SCALAR_FUNC(8u##S,   uchar, T, இரட்டை) \
    DEF_CVT_SCALE_SCALAR_FUNC(S##8s,   T, schar, இரட்டை)     DEF_CVT_SCALE_SCALAR_FUNC(8s##S,   schar, T, இரட்டை) \
    DEF_CVT_SCALE_SCALAR_FUNC(S##16u,  T, ushort, இரட்டை)    DEF_CVT_SCALE_SCALAR_FUNC(16u##S,  ushort, T, இரட்டை) \
    DEF_CVT_SCALE_SCALAR_FUNC(S##16s,  T, short, இரட்டை)     DEF_CVT_SCALE_SCALAR_FUNC(16s##S,  short, T, இரட்டை) \
    DEF_CVT_SCALE_SCALAR_FUNC(S##32u,  T, unsigned, இரட்டை)  DEF_CVT_SCALE_SCALAR_FUNC(32u##S,  unsigned, T, இரட்டை) \
    DEF_CVT_SCALE_SCALAR_FUNC(S##32s,  T, முழு, இரட்டை)       DEF_CVT_SCALE_SCALAR_FUNC(32s##S,  முழு, T, இரட்டை) \
    DEF_CVT_SCALE_SCALAR_FUNC(S##32f,  T, தசம, இரட்டை) \
    DEF_CVT_SCALE_SCALAR_FUNC(S##64f,  T, இரட்டை, இரட்டை)    DEF_CVT_SCALE_SCALAR_FUNC(64f##S,  இரட்டை, T, இரட்டை) \
    DEF_CVT_SCALE_SCALAR_FUNC(S##16f,  T, hfloat, இரட்டை)    DEF_CVT_SCALE_SCALAR_FUNC(16f##S,  hfloat, T, இரட்டை) \
    DEF_CVT_SCALE_SCALAR_FUNC(S##16bf, T, bfloat, இரட்டை)    DEF_CVT_SCALE_SCALAR_FUNC(16bf##S, bfloat, T, இரட்டை) \
    DEF_CVT_SCALE_SCALAR_FUNC(S##64u,  T, uint64_t, இரட்டை)  DEF_CVT_SCALE_SCALAR_FUNC(64u##S,  uint64_t, T, இரட்டை) \
    DEF_CVT_SCALE_SCALAR_FUNC(S##64s,  T, int64_t, இரட்டை)   DEF_CVT_SCALE_SCALAR_FUNC(64s##S,  int64_t, T, இரட்டை) \
    DEF_CVT_SCALE_SCALAR_FUNC(S##8b,   T, தருக்க, இரட்டை) \
    DEF_CVT_SCALE_SCALAR_FUNC(S##8fe4m3,  T, fp8_t, இரட்டை)   DEF_CVT_SCALE_SCALAR_FUNC(S##8fe4m3u, T, fp8a_t, இரட்டை)

DEF_CVT_SCALE_FP8(8fe4m3,  fp8_t)
DEF_CVT_SCALE_FP8(8fe4m3u, fp8a_t)

// convertTo(dst, depth, alpha, beta) path -- real quantization. Reuses convert.hpp's fast path.
// Double precision here matches the scalar reference (wtype)x*a+b bit-for-bit.
template<முழு bias, தருக்க fnuz> நிலையான வெறுமை
cvtScaleF32ToFp8(மாறிலி uchar* src_, size_t sstep, மாறிலி uchar*, size_t,
                 uchar* dst, size_t dstep, Size size, வெறுமை* scale_)
{
    CV_INSTRUMENT_REGION();
    மாறிலி தசம* src = (மாறிலி தசம*)src_;
    மாறிலி இரட்டை* scale = (மாறிலி இரட்டை*)scale_;
    இரட்டை a = scale[0], b = scale[1];
    sstep /= அளவு(src[0]);

    க்கு (முழு i = 0; i < size.height; i++, src += sstep, dst += dstep)
    {
        முழு j = 0;
#எனில் (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)
        v_float64 va = vx_setall_f64(a), vb = vx_setall_f64(b);
        மாறிலி முழு LANES32 = VTraits<v_float32>::vlanes();
        மாறிலி முழு VECSZ = LANES32 * 4;
        தசம scaled[VTraits<v_float32>::max_nlanes * 4];
        க்கு (; j <= size.width - VECSZ; j += VECSZ)
        {
            v_float32 vf0, vf1, vf2, vf3;
            v_float64 d0, d1;

            vx_load_pair_as(src + j + 0 * LANES32, d0, d1);
            d0 = v_add(v_mul(d0, va), vb);
            d1 = v_add(v_mul(d1, va), vb);
            vf0 = v_cvt_f32(d0, d1);
            v_store(scaled + 0 * LANES32, vf0);

            vx_load_pair_as(src + j + 1 * LANES32, d0, d1);
            d0 = v_add(v_mul(d0, va), vb);
            d1 = v_add(v_mul(d1, va), vb);
            vf1 = v_cvt_f32(d0, d1);
            v_store(scaled + 1 * LANES32, vf1);

            vx_load_pair_as(src + j + 2 * LANES32, d0, d1);
            d0 = v_add(v_mul(d0, va), vb);
            d1 = v_add(v_mul(d1, va), vb);
            vf2 = v_cvt_f32(d0, d1);
            v_store(scaled + 2 * LANES32, vf2);

            vx_load_pair_as(src + j + 3 * LANES32, d0, d1);
            d0 = v_add(v_mul(d0, va), vb);
            d1 = v_add(v_mul(d1, va), vb);
            vf3 = v_cvt_f32(d0, d1);
            v_store(scaled + 3 * LANES32, vf3);

            encodeFp8Vec4<bias, fnuz>(vf0, vf1, vf2, vf3, scaled, VECSZ, dst + j);
        }
        vx_cleanup();
#endif
        க்கு (; j < size.width; j++)
            dst[j] = fp8_detail::encodeE4M3((தசம)((இரட்டை)src[j]*a + b), bias, fnuz);
    }
}
நிலையான வெறுமை cvtScale32f8fe4m3(மாறிலி uchar* s, size_t ss, மாறிலி uchar* p, size_t ps, uchar* d, size_t ds, Size sz, வெறுமை* x)
{ cvtScaleF32ToFp8<7, பொய்>(s, ss, p, ps, d, ds, sz, x); }
நிலையான வெறுமை cvtScale32f8fe4m3u(மாறிலி uchar* s, size_t ss, மாறிலி uchar* p, size_t ps, uchar* d, size_t ds, Size sz, வெறுமை* x)
{ cvtScaleF32ToFp8<8, உண்மை>(s, ss, p, ps, d, ds, sz, x); }

இருமம்_Func getConvertScaleFunc(முழு sdepth_, முழு ddepth_)
{
    முழு sdepth = CV_MAT_DEPTH(sdepth_);
    முழு ddepth = CV_MAT_DEPTH(ddepth_);
    இருமம்_Func func =
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
    திரும்பு func;
}

#endif

CV_CPU_OPTIMIZATION_NAMESPACE_END
} // namespace
