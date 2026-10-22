// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


#شامل "precomp.hpp"
#شامل "convert.hpp"

فضاینام cv {
CV_CPU_OPTIMIZATION_NAMESPACE_BEGIN

دودویی_Func getCvtScaleAbsFunc(صحیح depth);
دودویی_Func getConvertScaleFunc(صحیح sdepth, صحیح ddepth);

#ifndef CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

/****************************************************************************************\
*                                convertScale[Abs]                                       *
\****************************************************************************************/

template<typename _Ts, typename _Td> درونخطی تهی
cvtabs_32f( ثابت _Ts* src, size_t sstep, _Td* dst, size_t dstep,
            Size size, اعشاری a, اعشاری b )
{
#اگر (CV_SIMD || CV_SIMD_SCALABLE)
    v_float32 va = vx_setall_f32(a), vb = vx_setall_f32(b);
    ثابت صحیح VECSZ = VTraits<v_float32>::vlanes()*2;
    // GCC miscompiles this scalable block only on VLEN=128 RVV (opencv/issues/26936);
    // use scalar there, keep SIMD on wider vectors. Ask the hardware for VLEN directly:
    // vlanes() would depend on the LMUL the universal intrinsics happen to use.
#اگر (CV_RVV && CV_SIMD_SCALABLE && defined(__GNUC__) && !defined(__clang__))
    ثابت منطقی useSIMD = __riscv_vlenb()*8 != 128;
#وگرنه
    ثابت منطقی useSIMD = درست;
#endif
#endif
    sstep /= اندازه(src[0]);
    dstep /= اندازه(dst[0]);

    برای( صحیح i = 0; i < size.height; i++, src += sstep, dst += dstep )
    {
        صحیح j = 0;
#اگر (CV_SIMD || CV_SIMD_SCALABLE)
        اگر( useSIMD )
        برای( ; j < size.width; j += VECSZ )
        {
            اگر( j > size.width - VECSZ )
            {
                اگر( j == 0 || src == (_Ts*)dst )
                    بشکن;
                j = size.width - VECSZ;
            }
            v_float32 v0, v1;
            vx_load_pair_as(src + j, v0, v1);
            v0 = v_fma(v0, va, vb);
            v1 = v_fma(v1, va, vb);
            v_store_pair_as(dst + j, v_abs(v0), v_abs(v1));
        }
#endif
        برای( ; j < size.width; j++ )
            dst[j] = saturate_cast<_Td>(std::abs(src[j]*a + b));
    }
}

ایستا تهی
cvtabs_32f( ثابت منطقی* src_, size_t sstep,
            uchar* dst, size_t dstep,
            Size size, اعشاری a, اعشاری b )
{
    ثابت uchar* src = (ثابت uchar*)src_;
    uchar v0 = saturate_cast<uchar>(std::abs(b));
    uchar v1 = saturate_cast<uchar>(std::abs(a + b));
    برای( صحیح i = 0; i < size.height; i++, src += sstep, dst += dstep )
    {
        برای (صحیح j = 0; j < size.width; j++)
            dst[j] = src[j] != 0 ? v1 : v0;
    }
}

template<typename _Ts, typename _Td> درونخطی تهی
cvt_32f( ثابت _Ts* src, size_t sstep, _Td* dst, size_t dstep,
         Size size, اعشاری a, اعشاری b )
{
#اگر (CV_SIMD || (CV_SIMD_SCALABLE && !(defined(__GNUC__) && !defined(__clang__))) )
    v_float32 va = vx_setall_f32(a), vb = vx_setall_f32(b);
    ثابت صحیح VECSZ = VTraits<v_float32>::vlanes()*2;
#endif
    sstep /= اندازه(src[0]);
    dstep /= اندازه(dst[0]);

    برای( صحیح i = 0; i < size.height; i++, src += sstep, dst += dstep )
    {
        صحیح j = 0;
// Excluding GNU in CV_SIMD_SCALABLE because of "opencv/issues/26936"
#اگر (CV_SIMD || (CV_SIMD_SCALABLE && !(defined(__GNUC__) && !defined(__clang__))) )
        برای( ; j < size.width; j += VECSZ )
        {
            اگر( j > size.width - VECSZ )
            {
                اگر( j == 0 || src == (_Ts*)dst )
                    بشکن;
                j = size.width - VECSZ;
            }
            v_float32 v0, v1;
            vx_load_pair_as(src + j, v0, v1);
            v0 = v_fma(v0, va, vb);
            v1 = v_fma(v1, va, vb);
            v_store_pair_as(dst + j, v0, v1);
        }
#endif
        برای( ; j < size.width; j++ )
            dst[j] = saturate_cast<_Td>(src[j]*a + b);
    }
}

// variant for conversions 16f <-> ... w/o unrolling
template<typename _Ts, typename _Td> درونخطی تهی
cvt1_32f( ثابت _Ts* src, size_t sstep, _Td* dst, size_t dstep,
          Size size, اعشاری a, اعشاری b )
{
#اگر (CV_SIMD || CV_SIMD_SCALABLE)
    v_float32 va = vx_setall_f32(a), vb = vx_setall_f32(b);
    ثابت صحیح VECSZ = VTraits<v_float32>::vlanes();
#endif
    sstep /= اندازه(src[0]);
    dstep /= اندازه(dst[0]);

    برای( صحیح i = 0; i < size.height; i++, src += sstep, dst += dstep )
    {
        صحیح j = 0;
#اگر (CV_SIMD || CV_SIMD_SCALABLE)
        برای( ; j < size.width; j += VECSZ )
        {
            اگر( j > size.width - VECSZ )
            {
                اگر( j == 0 || src == (_Ts*)dst )
                    بشکن;
                j = size.width - VECSZ;
            }
            v_float32 v0;
            vx_load_as(src + j, v0);
            v0 = v_fma(v0, va, vb);
            v_store_as(dst + j, v0);
        }
#endif
        برای( ; j < size.width; j++ )
            dst[j] = saturate_cast<_Td>(src[j]*a + b);
    }
}


template<typename _Ts, typename _Td> درونخطی تهی
cvt_64f( ثابت _Ts* src, size_t sstep, _Td* dst, size_t dstep,
         Size size, دوگانه a, دوگانه b )
{
#اگر (CV_SIMD_64F || (CV_SIMD_SCALABLE_64F && !(defined(__GNUC__) && !defined(__clang__))) )
    v_float64 va = vx_setall_f64(a), vb = vx_setall_f64(b);
    ثابت صحیح VECSZ = VTraits<v_float64>::vlanes()*2;
#endif
    sstep /= اندازه(src[0]);
    dstep /= اندازه(dst[0]);

    برای( صحیح i = 0; i < size.height; i++, src += sstep, dst += dstep )
    {
        صحیح j = 0;
// Excluding GNU in CV_SIMD_SCALABLE because of "opencv/issues/26936"
#اگر (CV_SIMD_64F || (CV_SIMD_SCALABLE_64F && !(defined(__GNUC__) && !defined(__clang__))) )
        برای( ; j < size.width; j += VECSZ )
        {
            اگر( j > size.width - VECSZ )
            {
                اگر( j == 0 || src == (_Ts*)dst )
                    بشکن;
                j = size.width - VECSZ;
            }
            v_float64 v0, v1;
            vx_load_pair_as(src + j, v0, v1);
            v0 = v_fma(v0, va, vb);
            v1 = v_fma(v1, va, vb);
            v_store_pair_as(dst + j, v0, v1);
        }
#endif
        برای( ; j < size.width; j++ )
            dst[j] = saturate_cast<_Td>(src[j]*a + b);
    }
}

//==================================================================================================

#define DEF_CVT_SCALE_ABS_FUNC(suffix, cvt, stype, dtype, wtype) \
ایستا تهی cvtScaleAbs##suffix( ثابت uchar* src_, size_t sstep, ثابت uchar*, size_t, \
                                 uchar* dst_, size_t dstep, Size size, تهی* scale_) \
{ \
    ثابت stype* src = (ثابت stype*)src_; \
    dtype* dst = (dtype*)dst_; \
    دوگانه* scale = (دوگانه*)scale_; \
    cvt(src, sstep, dst, dstep, size, (wtype)scale[0], (wtype)scale[1]); \
}


#define DEF_CVT_SCALE_FUNC(suffix, cvt, stype, dtype, wtype) \
ایستا تهی cvtScale##suffix( ثابت uchar* src_, size_t sstep, ثابت uchar*, size_t, \
                              uchar* dst_, size_t dstep, Size size, تهی* scale_) \
{ \
    ثابت stype* src = (ثابت stype*)src_; \
    dtype* dst = (dtype*)dst_; \
    دوگانه* scale = (دوگانه*)scale_; \
    cvt(src, sstep, dst, dstep, size, (wtype)scale[0], (wtype)scale[1]); \
}

#define DEF_CVT_SCALE2BOOL_FUNC(suffix, stype, wtype) \
ایستا تهی cvtScale##suffix( ثابت uchar* src_, size_t sstep, ثابت uchar*, size_t, \
                              uchar* dst, size_t dstep, Size size, تهی* scale_) \
{ \
    ثابت stype* src = (ثابت stype*)src_; \
    ثابت دوگانه* scale = (ثابت دوگانه*)scale_; \
    wtype a = (wtype)scale[0], b = (wtype)scale[1]; \
    sstep /= اندازه(src[0]); \
    برای( صحیح i = 0; i < size.height; i++, src += sstep, dst += dstep ) \
        برای (صحیح j = 0; j < size.width; j++) \
            dst[j] = (منطقی)((wtype)src[j]*a + b != 0); \
}

#define DEF_CVT_SCALEBOOL2_FUNC(suffix, dtype, wtype) \
ایستا تهی cvtScale##suffix( ثابت uchar* src, size_t sstep, ثابت uchar*, size_t, \
                              uchar* dst_, size_t dstep, Size size, تهی* scale_) \
{ \
    dtype* dst = (dtype*)dst_; \
    ثابت دوگانه* scale = (ثابت دوگانه*)scale_; \
    wtype a = (wtype)scale[0], b = (wtype)scale[1]; \
    dtype v0 = saturate_cast<dtype>(b), v1 = saturate_cast<dtype>(a + b); \
    dstep /= اندازه(dst[0]); \
    برای( صحیح i = 0; i < size.height; i++, src += sstep, dst += dstep ) \
        برای (صحیح j = 0; j < size.width; j++) \
            dst[j] = src[j] != 0 ? v1 : v0; \
}

DEF_CVT_SCALE_ABS_FUNC(8u,    cvtabs_32f, uchar,  uchar, اعشاری)
DEF_CVT_SCALE_ABS_FUNC(8s8u,  cvtabs_32f, schar,  uchar, اعشاری)
DEF_CVT_SCALE_ABS_FUNC(8b8u,  cvtabs_32f, منطقی,  uchar, اعشاری)
DEF_CVT_SCALE_ABS_FUNC(16u8u, cvtabs_32f, ushort, uchar, اعشاری)
DEF_CVT_SCALE_ABS_FUNC(16s8u, cvtabs_32f, short,  uchar, اعشاری)
DEF_CVT_SCALE_ABS_FUNC(32u8u, cvtabs_32f, unsigned, uchar, اعشاری)
DEF_CVT_SCALE_ABS_FUNC(32s8u, cvtabs_32f, صحیح,    uchar, اعشاری)
DEF_CVT_SCALE_ABS_FUNC(32f8u, cvtabs_32f, اعشاری,  uchar, اعشاری)
DEF_CVT_SCALE_ABS_FUNC(64u8u, cvtabs_32f, uint64_t, uchar, اعشاری)
DEF_CVT_SCALE_ABS_FUNC(64s8u, cvtabs_32f, int64_t, uchar, اعشاری)
DEF_CVT_SCALE_ABS_FUNC(64f8u, cvtabs_32f, دوگانه, uchar, اعشاری)
DEF_CVT_SCALE_ABS_FUNC(16f8u, cvtabs_32f, hfloat, uchar, اعشاری)
DEF_CVT_SCALE_ABS_FUNC(16bf8u, cvtabs_32f, bfloat, uchar, اعشاری)

DEF_CVT_SCALE_FUNC(8u,     cvt_32f, uchar,  uchar, اعشاری)
DEF_CVT_SCALE_FUNC(8s8u,   cvt_32f, schar,  uchar, اعشاری)
DEF_CVT_SCALE_FUNC(16u8u,  cvt_32f, ushort, uchar, اعشاری)
DEF_CVT_SCALE_FUNC(16s8u,  cvt_32f, short,  uchar, اعشاری)
DEF_CVT_SCALE_FUNC(32u8u,  cvt_32f, unsigned, uchar, اعشاری)
DEF_CVT_SCALE_FUNC(32s8u,  cvt_32f, صحیح,    uchar, اعشاری)
DEF_CVT_SCALE_FUNC(32f8u,  cvt_32f, اعشاری,  uchar, اعشاری)
DEF_CVT_SCALE_FUNC(64f8u,  cvt_32f, دوگانه, uchar, اعشاری)
DEF_CVT_SCALE_FUNC(64u8u,  cvt_32f, uint64_t, uchar, اعشاری)
DEF_CVT_SCALE_FUNC(64s8u,  cvt_32f, int64_t, uchar, اعشاری)
DEF_CVT_SCALE_FUNC(16f8u,  cvt_32f, hfloat, uchar, اعشاری)
DEF_CVT_SCALE_FUNC(16bf8u, cvt_32f, bfloat, uchar, اعشاری)

DEF_CVT_SCALE_FUNC(8u8s,   cvt_32f, uchar,  schar, اعشاری)
DEF_CVT_SCALE_FUNC(8s,     cvt_32f, schar,  schar, اعشاری)
DEF_CVT_SCALE_FUNC(16u8s,  cvt_32f, ushort, schar, اعشاری)
DEF_CVT_SCALE_FUNC(16s8s,  cvt_32f, short,  schar, اعشاری)
DEF_CVT_SCALE_FUNC(32u8s,  cvt_32f, unsigned, schar, اعشاری)
DEF_CVT_SCALE_FUNC(32s8s,  cvt_32f, صحیح,    schar, اعشاری)
DEF_CVT_SCALE_FUNC(32f8s,  cvt_32f, اعشاری,  schar, اعشاری)
DEF_CVT_SCALE_FUNC(64f8s,  cvt_32f, دوگانه, schar, اعشاری)
DEF_CVT_SCALE_FUNC(64u8s,  cvt_32f, uint64_t, schar, اعشاری)
DEF_CVT_SCALE_FUNC(64s8s,  cvt_32f, int64_t, schar, اعشاری)
DEF_CVT_SCALE_FUNC(16f8s,  cvt_32f, hfloat, schar, اعشاری)
DEF_CVT_SCALE_FUNC(16bf8s, cvt_32f, bfloat, schar, اعشاری)

DEF_CVT_SCALE2BOOL_FUNC(8u8b, uchar, اعشاری)
DEF_CVT_SCALE2BOOL_FUNC(8s8b, schar, اعشاری)
DEF_CVT_SCALE2BOOL_FUNC(16u8b, ushort, اعشاری)
DEF_CVT_SCALE2BOOL_FUNC(16s8b, short, اعشاری)
DEF_CVT_SCALE2BOOL_FUNC(32u8b, unsigned, اعشاری)
DEF_CVT_SCALE2BOOL_FUNC(32s8b, صحیح, اعشاری)
DEF_CVT_SCALE2BOOL_FUNC(32f8b, اعشاری, اعشاری)
DEF_CVT_SCALE2BOOL_FUNC(64f8b, دوگانه, اعشاری)
DEF_CVT_SCALE2BOOL_FUNC(64u8b, uint64_t, اعشاری)
DEF_CVT_SCALE2BOOL_FUNC(64s8b, int64_t, اعشاری)
DEF_CVT_SCALE2BOOL_FUNC(16f8b, hfloat, اعشاری)
DEF_CVT_SCALE2BOOL_FUNC(16bf8b, bfloat, اعشاری)

DEF_CVT_SCALE_FUNC(8u16u,  cvt_32f, uchar,  ushort, اعشاری)
DEF_CVT_SCALE_FUNC(8s16u,  cvt_32f, schar,  ushort, اعشاری)
DEF_CVT_SCALE_FUNC(16u,    cvt_32f, ushort, ushort, اعشاری)
DEF_CVT_SCALE_FUNC(16s16u, cvt_32f, short,  ushort, اعشاری)
DEF_CVT_SCALE_FUNC(32u16u, cvt_32f, unsigned, ushort, اعشاری)
DEF_CVT_SCALE_FUNC(32s16u, cvt_32f, صحیح,    ushort, اعشاری)
DEF_CVT_SCALE_FUNC(32f16u, cvt_32f, اعشاری,  ushort, اعشاری)
DEF_CVT_SCALE_FUNC(64f16u, cvt_32f, دوگانه, ushort, اعشاری)
DEF_CVT_SCALE_FUNC(64u16u, cvt_32f, uint64_t, ushort, اعشاری)
DEF_CVT_SCALE_FUNC(64s16u, cvt_32f, int64_t, ushort, اعشاری)
DEF_CVT_SCALE_FUNC(16f16u, cvt1_32f, hfloat, ushort, اعشاری)
DEF_CVT_SCALE_FUNC(16bf16u, cvt1_32f, bfloat, ushort, اعشاری)

DEF_CVT_SCALE_FUNC(8u16s,  cvt_32f, uchar,  short, اعشاری)
DEF_CVT_SCALE_FUNC(8s16s,  cvt_32f, schar,  short, اعشاری)
DEF_CVT_SCALE_FUNC(16u16s, cvt_32f, ushort, short, اعشاری)
DEF_CVT_SCALE_FUNC(16s,    cvt_32f, short,  short, اعشاری)
DEF_CVT_SCALE_FUNC(32u16s, cvt_32f, unsigned, short, اعشاری)
DEF_CVT_SCALE_FUNC(32s16s, cvt_32f, صحیح,    short, اعشاری)
DEF_CVT_SCALE_FUNC(32f16s, cvt_32f, اعشاری,  short, اعشاری)
DEF_CVT_SCALE_FUNC(64f16s, cvt_32f, دوگانه, short, اعشاری)
DEF_CVT_SCALE_FUNC(64u16s, cvt_32f, uint64_t, short, اعشاری)
DEF_CVT_SCALE_FUNC(64s16s, cvt_32f, int64_t, short, اعشاری)
DEF_CVT_SCALE_FUNC(16f16s, cvt1_32f, hfloat, short, اعشاری)
DEF_CVT_SCALE_FUNC(16bf16s, cvt1_32f, bfloat, short, اعشاری)

DEF_CVT_SCALE_FUNC(8u32u,  cvt_32f, uchar,  unsigned, اعشاری)
DEF_CVT_SCALE_FUNC(8s32u,  cvt_32f, schar,  unsigned, اعشاری)
DEF_CVT_SCALE_FUNC(16u32u, cvt_32f, ushort, unsigned, اعشاری)
DEF_CVT_SCALE_FUNC(16s32u, cvt_32f, short,  unsigned, اعشاری)
DEF_CVT_SCALE_FUNC(32u, cvt_32f, unsigned, unsigned, اعشاری)
DEF_CVT_SCALE_FUNC(32s32u, cvt_64f, صحیح,    unsigned, دوگانه)
DEF_CVT_SCALE_FUNC(32f32u, cvt_32f, اعشاری,  unsigned, اعشاری)
DEF_CVT_SCALE_FUNC(64f32u, cvt_64f, دوگانه, unsigned, دوگانه)
DEF_CVT_SCALE_FUNC(64u32u, cvt_32f, uint64_t, unsigned, اعشاری)
DEF_CVT_SCALE_FUNC(64s32u, cvt_32f, int64_t, unsigned, اعشاری)
DEF_CVT_SCALE_FUNC(16f32u, cvt1_32f, hfloat, unsigned, اعشاری)
DEF_CVT_SCALE_FUNC(16bf32u, cvt1_32f, bfloat, unsigned, اعشاری)

DEF_CVT_SCALE_FUNC(8u32s,  cvt_32f, uchar,  صحیح, اعشاری)
DEF_CVT_SCALE_FUNC(8s32s,  cvt_32f, schar,  صحیح, اعشاری)
DEF_CVT_SCALE_FUNC(16u32s, cvt_32f, ushort, صحیح, اعشاری)
DEF_CVT_SCALE_FUNC(16s32s, cvt_32f, short,  صحیح, اعشاری)
DEF_CVT_SCALE_FUNC(32u32s, cvt_32f, unsigned, صحیح, اعشاری)
DEF_CVT_SCALE_FUNC(32s,    cvt_64f, صحیح,    صحیح, دوگانه)
DEF_CVT_SCALE_FUNC(32f32s, cvt_32f, اعشاری,  صحیح, اعشاری)
DEF_CVT_SCALE_FUNC(64f32s, cvt_64f, دوگانه, صحیح, دوگانه)
DEF_CVT_SCALE_FUNC(64u32s, cvt_32f, uint64_t, صحیح, اعشاری)
DEF_CVT_SCALE_FUNC(64s32s, cvt_32f, int64_t, صحیح, اعشاری)
DEF_CVT_SCALE_FUNC(16f32s, cvt1_32f, hfloat, صحیح, اعشاری)
DEF_CVT_SCALE_FUNC(16bf32s, cvt1_32f, bfloat, صحیح, اعشاری)

DEF_CVT_SCALE_FUNC(8u32f,  cvt_32f, uchar,  اعشاری, اعشاری)
DEF_CVT_SCALE_FUNC(8s32f,  cvt_32f, schar,  اعشاری, اعشاری)
DEF_CVT_SCALE_FUNC(16u32f, cvt_32f, ushort, اعشاری, اعشاری)
DEF_CVT_SCALE_FUNC(16s32f, cvt_32f, short,  اعشاری, اعشاری)
DEF_CVT_SCALE_FUNC(32u32f, cvt_32f, unsigned, اعشاری, اعشاری)
DEF_CVT_SCALE_FUNC(32s32f, cvt_32f, صحیح,    اعشاری, اعشاری)
DEF_CVT_SCALE_FUNC(32f,    cvt_32f, اعشاری,  اعشاری, اعشاری)
DEF_CVT_SCALE_FUNC(64f32f, cvt_64f, دوگانه, اعشاری, دوگانه)
DEF_CVT_SCALE_FUNC(64u32f, cvt_32f, uint64_t, اعشاری, اعشاری)
DEF_CVT_SCALE_FUNC(64s32f, cvt_32f, int64_t, اعشاری, اعشاری)
DEF_CVT_SCALE_FUNC(16f32f, cvt1_32f, hfloat, اعشاری, اعشاری)
DEF_CVT_SCALE_FUNC(16bf32f, cvt1_32f, bfloat, اعشاری, اعشاری)

DEF_CVT_SCALE_FUNC(8u64f,  cvt_64f, uchar,  دوگانه, دوگانه)
DEF_CVT_SCALE_FUNC(8s64f,  cvt_64f, schar,  دوگانه, دوگانه)
DEF_CVT_SCALE_FUNC(16u64f, cvt_64f, ushort, دوگانه, دوگانه)
DEF_CVT_SCALE_FUNC(16s64f, cvt_64f, short,  دوگانه, دوگانه)
DEF_CVT_SCALE_FUNC(32u64f, cvt_64f, unsigned, دوگانه, دوگانه)
DEF_CVT_SCALE_FUNC(32s64f, cvt_64f, صحیح,    دوگانه, دوگانه)
DEF_CVT_SCALE_FUNC(32f64f, cvt_64f, اعشاری,  دوگانه, دوگانه)
DEF_CVT_SCALE_FUNC(64f,    cvt_64f, دوگانه, دوگانه, دوگانه)
DEF_CVT_SCALE_FUNC(64u64f, cvt_64f, uint64_t, دوگانه, دوگانه)
DEF_CVT_SCALE_FUNC(64s64f, cvt_64f, int64_t, دوگانه, دوگانه)
DEF_CVT_SCALE_FUNC(16f64f, cvt_64f, hfloat, دوگانه, دوگانه)
DEF_CVT_SCALE_FUNC(16bf64f, cvt_64f, bfloat, دوگانه, دوگانه)

DEF_CVT_SCALE_FUNC(8u64u,  cvt_64f, uchar,  uint64_t, دوگانه)
DEF_CVT_SCALE_FUNC(8s64u,  cvt_64f, schar,  uint64_t, دوگانه)
DEF_CVT_SCALE_FUNC(16u64u, cvt_64f, ushort, uint64_t, دوگانه)
DEF_CVT_SCALE_FUNC(16s64u, cvt_64f, short,  uint64_t, دوگانه)
DEF_CVT_SCALE_FUNC(32u64u, cvt_64f, unsigned, uint64_t, دوگانه)
DEF_CVT_SCALE_FUNC(32s64u, cvt_64f, صحیح,    uint64_t, دوگانه)
DEF_CVT_SCALE_FUNC(32f64u, cvt_64f, اعشاری,  uint64_t, دوگانه)
DEF_CVT_SCALE_FUNC(64f64u, cvt_64f, دوگانه, uint64_t, دوگانه)
DEF_CVT_SCALE_FUNC(64u, cvt_64f, uint64_t, uint64_t, دوگانه)
DEF_CVT_SCALE_FUNC(64s64u, cvt_64f, int64_t, uint64_t, دوگانه)
DEF_CVT_SCALE_FUNC(16f64u, cvt_64f, hfloat, uint64_t, دوگانه)
DEF_CVT_SCALE_FUNC(16bf64u, cvt_64f, bfloat, uint64_t, دوگانه)

DEF_CVT_SCALE_FUNC(8u64s,  cvt_64f, uchar,  int64_t, دوگانه)
DEF_CVT_SCALE_FUNC(8s64s,  cvt_64f, schar,  int64_t, دوگانه)
DEF_CVT_SCALE_FUNC(16u64s, cvt_64f, ushort, int64_t, دوگانه)
DEF_CVT_SCALE_FUNC(16s64s, cvt_64f, short,  int64_t, دوگانه)
DEF_CVT_SCALE_FUNC(32u64s, cvt_64f, unsigned, int64_t, دوگانه)
DEF_CVT_SCALE_FUNC(32s64s, cvt_64f, صحیح,    int64_t, دوگانه)
DEF_CVT_SCALE_FUNC(32f64s, cvt_64f, اعشاری,  int64_t, دوگانه)
DEF_CVT_SCALE_FUNC(64f64s, cvt_64f, دوگانه, int64_t, دوگانه)
DEF_CVT_SCALE_FUNC(64u64s, cvt_64f, uint64_t, int64_t, دوگانه)
DEF_CVT_SCALE_FUNC(64s, cvt_64f, int64_t, int64_t, دوگانه)
DEF_CVT_SCALE_FUNC(16f64s, cvt_64f, hfloat, int64_t, دوگانه)
DEF_CVT_SCALE_FUNC(16bf64s, cvt_64f, bfloat, int64_t, دوگانه)

DEF_CVT_SCALE_FUNC(8u16f,  cvt1_32f, uchar,  hfloat, اعشاری)
DEF_CVT_SCALE_FUNC(8s16f,  cvt1_32f, schar,  hfloat, اعشاری)
DEF_CVT_SCALE_FUNC(16u16f, cvt1_32f, ushort, hfloat, اعشاری)
DEF_CVT_SCALE_FUNC(16s16f, cvt1_32f, short,  hfloat, اعشاری)
DEF_CVT_SCALE_FUNC(32u16f, cvt1_32f, unsigned, hfloat, اعشاری)
DEF_CVT_SCALE_FUNC(32s16f, cvt1_32f, صحیح,    hfloat, اعشاری)
DEF_CVT_SCALE_FUNC(32f16f, cvt1_32f, اعشاری,  hfloat, اعشاری)
DEF_CVT_SCALE_FUNC(64f16f, cvt1_32f, دوگانه, hfloat, اعشاری)
DEF_CVT_SCALE_FUNC(64u16f, cvt1_32f, uint64_t, hfloat, اعشاری)
DEF_CVT_SCALE_FUNC(64s16f, cvt1_32f, int64_t, hfloat, اعشاری)
DEF_CVT_SCALE_FUNC(16f,    cvt1_32f, hfloat, hfloat, اعشاری)
DEF_CVT_SCALE_FUNC(16bf16f, cvt1_32f, bfloat, hfloat, اعشاری)

DEF_CVT_SCALE_FUNC(8u16bf,  cvt1_32f, uchar,  bfloat, اعشاری)
DEF_CVT_SCALE_FUNC(8s16bf,  cvt1_32f, schar,  bfloat, اعشاری)
DEF_CVT_SCALE_FUNC(16u16bf, cvt1_32f, ushort, bfloat, اعشاری)
DEF_CVT_SCALE_FUNC(16s16bf, cvt1_32f, short,  bfloat, اعشاری)
DEF_CVT_SCALE_FUNC(32u16bf, cvt1_32f, unsigned, bfloat, اعشاری)
DEF_CVT_SCALE_FUNC(32s16bf, cvt1_32f, صحیح,    bfloat, اعشاری)
DEF_CVT_SCALE_FUNC(32f16bf, cvt1_32f, اعشاری,  bfloat, اعشاری)
DEF_CVT_SCALE_FUNC(64f16bf, cvt1_32f, دوگانه, bfloat, اعشاری)
DEF_CVT_SCALE_FUNC(64u16bf, cvt1_32f, uint64_t, bfloat, اعشاری)
DEF_CVT_SCALE_FUNC(64s16bf, cvt1_32f, int64_t, bfloat, اعشاری)
DEF_CVT_SCALE_FUNC(16f16bf, cvt1_32f, hfloat, bfloat, اعشاری)
DEF_CVT_SCALE_FUNC(16bf, cvt1_32f, bfloat, bfloat, اعشاری)

DEF_CVT_SCALEBOOL2_FUNC(8b8u, uchar, اعشاری)
DEF_CVT_SCALEBOOL2_FUNC(8b8s, schar, اعشاری)
DEF_CVT_SCALEBOOL2_FUNC(8b, منطقی, اعشاری)
DEF_CVT_SCALEBOOL2_FUNC(8b16u, ushort, اعشاری)
DEF_CVT_SCALEBOOL2_FUNC(8b16s, short, اعشاری)
DEF_CVT_SCALEBOOL2_FUNC(8b32u, unsigned, اعشاری)
DEF_CVT_SCALEBOOL2_FUNC(8b32s, صحیح, اعشاری)
DEF_CVT_SCALEBOOL2_FUNC(8b32f, اعشاری, اعشاری)
DEF_CVT_SCALEBOOL2_FUNC(8b64u, uint64_t, دوگانه)
DEF_CVT_SCALEBOOL2_FUNC(8b64s, int64_t, دوگانه)
DEF_CVT_SCALEBOOL2_FUNC(8b64f, دوگانه, دوگانه)
DEF_CVT_SCALEBOOL2_FUNC(8b16f, hfloat, اعشاری)
DEF_CVT_SCALEBOOL2_FUNC(8b16bf, bfloat, اعشاری)

// FP8 src -> CV_8U abs|scale|shift (scalar)
#define DEF_CVT_SCALE_ABS_SCALAR_FUNC(suffix, stype) \
ایستا تهی cvtScaleAbs##suffix( ثابت uchar* src_, size_t sstep, ثابت uchar*, size_t, \
                                 uchar* dst, size_t dstep, Size size, تهی* scale_) \
{ \
    ثابت stype* src = (ثابت stype*)src_; \
    ثابت دوگانه* scale = (ثابت دوگانه*)scale_; \
    اعشاری a = (اعشاری)scale[0], b = (اعشاری)scale[1]; \
    sstep /= اندازه(src[0]); \
    برای( صحیح i = 0; i < size.height; i++, src += sstep, dst += dstep ) \
        برای (صحیح j = 0; j < size.width; j++) \
            dst[j] = saturate_cast<uchar>(std::abs((اعشاری)src[j]*a + b)); \
}
DEF_CVT_SCALE_ABS_SCALAR_FUNC(8fe4m3u8u, fp8a_t)
DEF_CVT_SCALE_ABS_SCALAR_FUNC(8fe4m38u,  fp8_t)

دودویی_Func getCvtScaleAbsFunc(صحیح depth)
{
    دودویی_Func func =
        depth == CV_8F_E4M3FN ? (دودویی_Func)cvtScaleAbs8fe4m38u :
        depth == CV_8F_E4M3FNUZ ? (دودویی_Func)cvtScaleAbs8fe4m3u8u :
        depth == CV_8U ? (دودویی_Func)cvtScaleAbs8u :
        depth == CV_8S ? (دودویی_Func)cvtScaleAbs8s8u :
        depth == CV_Bool ? (دودویی_Func)cvtScaleAbs8b8u :
        depth == CV_16U ? (دودویی_Func)cvtScaleAbs16u8u :
        depth == CV_16S ? (دودویی_Func)cvtScaleAbs16s8u :
        depth == CV_16F ? (دودویی_Func)cvtScaleAbs16f8u :
        depth == CV_16BF ? (دودویی_Func)cvtScaleAbs16bf8u :
        depth == CV_32U ? (دودویی_Func)cvtScaleAbs32u8u :
        depth == CV_32S ? (دودویی_Func)cvtScaleAbs32s8u :
        depth == CV_32F ? (دودویی_Func)cvtScaleAbs32f8u :
        depth == CV_64U ? (دودویی_Func)cvtScaleAbs64u8u :
        depth == CV_64S ? (دودویی_Func)cvtScaleAbs64s8u :
        depth == CV_64F ? (دودویی_Func)cvtScaleAbs64f8u : 0;
    CV_Assert(func != 0);
    بازگشت func;
}

//////////////////// FP8 scaled conversions — scalar via saturate_cast(src*a+b) ////////////////////
#define DEF_CVT_SCALE_SCALAR_FUNC(suffix, stype, dtype, wtype) \
ایستا تهی cvtScale##suffix( ثابت uchar* src_, size_t sstep, ثابت uchar*, size_t, \
                              uchar* dst_, size_t dstep, Size size, تهی* scale_) \
{ \
    ثابت stype* src = (ثابت stype*)src_; dtype* dst = (dtype*)dst_; \
    ثابت دوگانه* scale = (ثابت دوگانه*)scale_; \
    wtype a = (wtype)scale[0], b = (wtype)scale[1]; \
    sstep /= اندازه(src[0]); dstep /= اندازه(dst[0]); \
    برای( صحیح i = 0; i < size.height; i++, src += sstep, dst += dstep ) \
        برای (صحیح j = 0; j < size.width; j++) \
            dst[j] = saturate_cast<dtype>((wtype)src[j]*a + b); \
}

// double intermediate so coarse FP8 grids round identically to the (double-precision) reference
#define DEF_CVT_SCALE_FP8(S, T) \
    DEF_CVT_SCALE_SCALAR_FUNC(S##8u,   T, uchar, دوگانه)     DEF_CVT_SCALE_SCALAR_FUNC(8u##S,   uchar, T, دوگانه) \
    DEF_CVT_SCALE_SCALAR_FUNC(S##8s,   T, schar, دوگانه)     DEF_CVT_SCALE_SCALAR_FUNC(8s##S,   schar, T, دوگانه) \
    DEF_CVT_SCALE_SCALAR_FUNC(S##16u,  T, ushort, دوگانه)    DEF_CVT_SCALE_SCALAR_FUNC(16u##S,  ushort, T, دوگانه) \
    DEF_CVT_SCALE_SCALAR_FUNC(S##16s,  T, short, دوگانه)     DEF_CVT_SCALE_SCALAR_FUNC(16s##S,  short, T, دوگانه) \
    DEF_CVT_SCALE_SCALAR_FUNC(S##32u,  T, unsigned, دوگانه)  DEF_CVT_SCALE_SCALAR_FUNC(32u##S,  unsigned, T, دوگانه) \
    DEF_CVT_SCALE_SCALAR_FUNC(S##32s,  T, صحیح, دوگانه)       DEF_CVT_SCALE_SCALAR_FUNC(32s##S,  صحیح, T, دوگانه) \
    DEF_CVT_SCALE_SCALAR_FUNC(S##32f,  T, اعشاری, دوگانه) \
    DEF_CVT_SCALE_SCALAR_FUNC(S##64f,  T, دوگانه, دوگانه)    DEF_CVT_SCALE_SCALAR_FUNC(64f##S,  دوگانه, T, دوگانه) \
    DEF_CVT_SCALE_SCALAR_FUNC(S##16f,  T, hfloat, دوگانه)    DEF_CVT_SCALE_SCALAR_FUNC(16f##S,  hfloat, T, دوگانه) \
    DEF_CVT_SCALE_SCALAR_FUNC(S##16bf, T, bfloat, دوگانه)    DEF_CVT_SCALE_SCALAR_FUNC(16bf##S, bfloat, T, دوگانه) \
    DEF_CVT_SCALE_SCALAR_FUNC(S##64u,  T, uint64_t, دوگانه)  DEF_CVT_SCALE_SCALAR_FUNC(64u##S,  uint64_t, T, دوگانه) \
    DEF_CVT_SCALE_SCALAR_FUNC(S##64s,  T, int64_t, دوگانه)   DEF_CVT_SCALE_SCALAR_FUNC(64s##S,  int64_t, T, دوگانه) \
    DEF_CVT_SCALE_SCALAR_FUNC(S##8b,   T, منطقی, دوگانه) \
    DEF_CVT_SCALE_SCALAR_FUNC(S##8fe4m3,  T, fp8_t, دوگانه)   DEF_CVT_SCALE_SCALAR_FUNC(S##8fe4m3u, T, fp8a_t, دوگانه)

DEF_CVT_SCALE_FP8(8fe4m3,  fp8_t)
DEF_CVT_SCALE_FP8(8fe4m3u, fp8a_t)

// convertTo(dst, depth, alpha, beta) path -- real quantization. Reuses convert.hpp's fast path.
// Double precision here matches the scalar reference (wtype)x*a+b bit-for-bit.
template<صحیح bias, منطقی fnuz> ایستا تهی
cvtScaleF32ToFp8(ثابت uchar* src_, size_t sstep, ثابت uchar*, size_t,
                 uchar* dst, size_t dstep, Size size, تهی* scale_)
{
    CV_INSTRUMENT_REGION();
    ثابت اعشاری* src = (ثابت اعشاری*)src_;
    ثابت دوگانه* scale = (ثابت دوگانه*)scale_;
    دوگانه a = scale[0], b = scale[1];
    sstep /= اندازه(src[0]);

    برای (صحیح i = 0; i < size.height; i++, src += sstep, dst += dstep)
    {
        صحیح j = 0;
#اگر (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)
        v_float64 va = vx_setall_f64(a), vb = vx_setall_f64(b);
        ثابت صحیح LANES32 = VTraits<v_float32>::vlanes();
        ثابت صحیح VECSZ = LANES32 * 4;
        اعشاری scaled[VTraits<v_float32>::max_nlanes * 4];
        برای (; j <= size.width - VECSZ; j += VECSZ)
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
        برای (; j < size.width; j++)
            dst[j] = fp8_detail::encodeE4M3((اعشاری)((دوگانه)src[j]*a + b), bias, fnuz);
    }
}
ایستا تهی cvtScale32f8fe4m3(ثابت uchar* s, size_t ss, ثابت uchar* p, size_t ps, uchar* d, size_t ds, Size sz, تهی* x)
{ cvtScaleF32ToFp8<7, نادرست>(s, ss, p, ps, d, ds, sz, x); }
ایستا تهی cvtScale32f8fe4m3u(ثابت uchar* s, size_t ss, ثابت uchar* p, size_t ps, uchar* d, size_t ds, Size sz, تهی* x)
{ cvtScaleF32ToFp8<8, درست>(s, ss, p, ps, d, ds, sz, x); }

دودویی_Func getConvertScaleFunc(صحیح sdepth_, صحیح ddepth_)
{
    صحیح sdepth = CV_MAT_DEPTH(sdepth_);
    صحیح ddepth = CV_MAT_DEPTH(ddepth_);
    دودویی_Func func =
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
    بازگشت func;
}

#endif

CV_CPU_OPTIMIZATION_NAMESPACE_END
} // namespace
