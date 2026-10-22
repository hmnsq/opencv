// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


#ضمن "precomp.hpp"
#ضمن "convert.hpp"

نطاق cv {
CV_CPU_OPTIMIZATION_NAMESPACE_BEGIN

ثنائی_Func getCvtScaleAbsFunc(صحيح depth);
ثنائی_Func getConvertScaleFunc(صحيح sdepth, صحيح ddepth);

#ifndef CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

/****************************************************************************************\
*                                convertScale[Abs]                                       *
\****************************************************************************************/

template<typename _Ts, typename _Td> مضمن فراغ
cvtabs_32f( ثابت _Ts* src, size_t sstep, _Td* dst, size_t dstep,
            Size size, عائم a, عائم b )
{
#إذا (CV_SIMD || CV_SIMD_SCALABLE)
    v_float32 va = vx_setall_f32(a), vb = vx_setall_f32(b);
    ثابت صحيح VECSZ = VTraits<v_float32>::vlanes()*2;
    // GCC miscompiles this scalable block only on VLEN=128 RVV (opencv/issues/26936);
    // use scalar there, keep SIMD on wider vectors. Ask the hardware for VLEN directly:
    // vlanes() would depend on the LMUL the universal intrinsics happen to use.
#إذا (CV_RVV && CV_SIMD_SCALABLE && defined(__GNUC__) && !defined(__clang__))
    ثابت منطقي useSIMD = __riscv_vlenb()*8 != 128;
#وإلا
    ثابت منطقي useSIMD = true;
#endif
#endif
    sstep /= حجم(src[0]);
    dstep /= حجم(dst[0]);

    لكل( صحيح i = 0; i < size.height; i++, src += sstep, dst += dstep )
    {
        صحيح j = 0;
#إذا (CV_SIMD || CV_SIMD_SCALABLE)
        إذا( useSIMD )
        لكل( ; j < size.width; j += VECSZ )
        {
            إذا( j > size.width - VECSZ )
            {
                إذا( j == 0 || src == (_Ts*)dst )
                    اكسر;
                j = size.width - VECSZ;
            }
            v_float32 v0, v1;
            vx_load_pair_as(src + j, v0, v1);
            v0 = v_fma(v0, va, vb);
            v1 = v_fma(v1, va, vb);
            v_store_pair_as(dst + j, v_abs(v0), v_abs(v1));
        }
#endif
        لكل( ; j < size.width; j++ )
            dst[j] = saturate_cast<_Td>(std::abs(src[j]*a + b));
    }
}

ساكن فراغ
cvtabs_32f( ثابت منطقي* src_, size_t sstep,
            uchar* dst, size_t dstep,
            Size size, عائم a, عائم b )
{
    ثابت uchar* src = (ثابت uchar*)src_;
    uchar v0 = saturate_cast<uchar>(std::abs(b));
    uchar v1 = saturate_cast<uchar>(std::abs(a + b));
    لكل( صحيح i = 0; i < size.height; i++, src += sstep, dst += dstep )
    {
        لكل (صحيح j = 0; j < size.width; j++)
            dst[j] = src[j] != 0 ? v1 : v0;
    }
}

template<typename _Ts, typename _Td> مضمن فراغ
cvt_32f( ثابت _Ts* src, size_t sstep, _Td* dst, size_t dstep,
         Size size, عائم a, عائم b )
{
#إذا (CV_SIMD || (CV_SIMD_SCALABLE && !(defined(__GNUC__) && !defined(__clang__))) )
    v_float32 va = vx_setall_f32(a), vb = vx_setall_f32(b);
    ثابت صحيح VECSZ = VTraits<v_float32>::vlanes()*2;
#endif
    sstep /= حجم(src[0]);
    dstep /= حجم(dst[0]);

    لكل( صحيح i = 0; i < size.height; i++, src += sstep, dst += dstep )
    {
        صحيح j = 0;
// Excluding GNU in CV_SIMD_SCALABLE because of "opencv/issues/26936"
#إذا (CV_SIMD || (CV_SIMD_SCALABLE && !(defined(__GNUC__) && !defined(__clang__))) )
        لكل( ; j < size.width; j += VECSZ )
        {
            إذا( j > size.width - VECSZ )
            {
                إذا( j == 0 || src == (_Ts*)dst )
                    اكسر;
                j = size.width - VECSZ;
            }
            v_float32 v0, v1;
            vx_load_pair_as(src + j, v0, v1);
            v0 = v_fma(v0, va, vb);
            v1 = v_fma(v1, va, vb);
            v_store_pair_as(dst + j, v0, v1);
        }
#endif
        لكل( ; j < size.width; j++ )
            dst[j] = saturate_cast<_Td>(src[j]*a + b);
    }
}

// variant for conversions 16f <-> ... w/o unrolling
template<typename _Ts, typename _Td> مضمن فراغ
cvt1_32f( ثابت _Ts* src, size_t sstep, _Td* dst, size_t dstep,
          Size size, عائم a, عائم b )
{
#إذا (CV_SIMD || CV_SIMD_SCALABLE)
    v_float32 va = vx_setall_f32(a), vb = vx_setall_f32(b);
    ثابت صحيح VECSZ = VTraits<v_float32>::vlanes();
#endif
    sstep /= حجم(src[0]);
    dstep /= حجم(dst[0]);

    لكل( صحيح i = 0; i < size.height; i++, src += sstep, dst += dstep )
    {
        صحيح j = 0;
#إذا (CV_SIMD || CV_SIMD_SCALABLE)
        لكل( ; j < size.width; j += VECSZ )
        {
            إذا( j > size.width - VECSZ )
            {
                إذا( j == 0 || src == (_Ts*)dst )
                    اكسر;
                j = size.width - VECSZ;
            }
            v_float32 v0;
            vx_load_as(src + j, v0);
            v0 = v_fma(v0, va, vb);
            v_store_as(dst + j, v0);
        }
#endif
        لكل( ; j < size.width; j++ )
            dst[j] = saturate_cast<_Td>(src[j]*a + b);
    }
}


template<typename _Ts, typename _Td> مضمن فراغ
cvt_64f( ثابت _Ts* src, size_t sstep, _Td* dst, size_t dstep,
         Size size, مزدوج a, مزدوج b )
{
#إذا (CV_SIMD_64F || (CV_SIMD_SCALABLE_64F && !(defined(__GNUC__) && !defined(__clang__))) )
    v_float64 va = vx_setall_f64(a), vb = vx_setall_f64(b);
    ثابت صحيح VECSZ = VTraits<v_float64>::vlanes()*2;
#endif
    sstep /= حجم(src[0]);
    dstep /= حجم(dst[0]);

    لكل( صحيح i = 0; i < size.height; i++, src += sstep, dst += dstep )
    {
        صحيح j = 0;
// Excluding GNU in CV_SIMD_SCALABLE because of "opencv/issues/26936"
#إذا (CV_SIMD_64F || (CV_SIMD_SCALABLE_64F && !(defined(__GNUC__) && !defined(__clang__))) )
        لكل( ; j < size.width; j += VECSZ )
        {
            إذا( j > size.width - VECSZ )
            {
                إذا( j == 0 || src == (_Ts*)dst )
                    اكسر;
                j = size.width - VECSZ;
            }
            v_float64 v0, v1;
            vx_load_pair_as(src + j, v0, v1);
            v0 = v_fma(v0, va, vb);
            v1 = v_fma(v1, va, vb);
            v_store_pair_as(dst + j, v0, v1);
        }
#endif
        لكل( ; j < size.width; j++ )
            dst[j] = saturate_cast<_Td>(src[j]*a + b);
    }
}

//==================================================================================================

#define DEF_CVT_SCALE_ABS_FUNC(suffix, cvt, stype, dtype, wtype) \
ساكن فراغ cvtScaleAbs##suffix( ثابت uchar* src_, size_t sstep, ثابت uchar*, size_t, \
                                 uchar* dst_, size_t dstep, Size size, فراغ* scale_) \
{ \
    ثابت stype* src = (ثابت stype*)src_; \
    dtype* dst = (dtype*)dst_; \
    مزدوج* scale = (مزدوج*)scale_; \
    cvt(src, sstep, dst, dstep, size, (wtype)scale[0], (wtype)scale[1]); \
}


#define DEF_CVT_SCALE_FUNC(suffix, cvt, stype, dtype, wtype) \
ساكن فراغ cvtScale##suffix( ثابت uchar* src_, size_t sstep, ثابت uchar*, size_t, \
                              uchar* dst_, size_t dstep, Size size, فراغ* scale_) \
{ \
    ثابت stype* src = (ثابت stype*)src_; \
    dtype* dst = (dtype*)dst_; \
    مزدوج* scale = (مزدوج*)scale_; \
    cvt(src, sstep, dst, dstep, size, (wtype)scale[0], (wtype)scale[1]); \
}

#define DEF_CVT_SCALE2BOOL_FUNC(suffix, stype, wtype) \
ساكن فراغ cvtScale##suffix( ثابت uchar* src_, size_t sstep, ثابت uchar*, size_t, \
                              uchar* dst, size_t dstep, Size size, فراغ* scale_) \
{ \
    ثابت stype* src = (ثابت stype*)src_; \
    ثابت مزدوج* scale = (ثابت مزدوج*)scale_; \
    wtype a = (wtype)scale[0], b = (wtype)scale[1]; \
    sstep /= حجم(src[0]); \
    لكل( صحيح i = 0; i < size.height; i++, src += sstep, dst += dstep ) \
        لكل (صحيح j = 0; j < size.width; j++) \
            dst[j] = (منطقي)((wtype)src[j]*a + b != 0); \
}

#define DEF_CVT_SCALEBOOL2_FUNC(suffix, dtype, wtype) \
ساكن فراغ cvtScale##suffix( ثابت uchar* src, size_t sstep, ثابت uchar*, size_t, \
                              uchar* dst_, size_t dstep, Size size, فراغ* scale_) \
{ \
    dtype* dst = (dtype*)dst_; \
    ثابت مزدوج* scale = (ثابت مزدوج*)scale_; \
    wtype a = (wtype)scale[0], b = (wtype)scale[1]; \
    dtype v0 = saturate_cast<dtype>(b), v1 = saturate_cast<dtype>(a + b); \
    dstep /= حجم(dst[0]); \
    لكل( صحيح i = 0; i < size.height; i++, src += sstep, dst += dstep ) \
        لكل (صحيح j = 0; j < size.width; j++) \
            dst[j] = src[j] != 0 ? v1 : v0; \
}

DEF_CVT_SCALE_ABS_FUNC(8u,    cvtabs_32f, uchar,  uchar, عائم)
DEF_CVT_SCALE_ABS_FUNC(8s8u,  cvtabs_32f, schar,  uchar, عائم)
DEF_CVT_SCALE_ABS_FUNC(8b8u,  cvtabs_32f, منطقي,  uchar, عائم)
DEF_CVT_SCALE_ABS_FUNC(16u8u, cvtabs_32f, ushort, uchar, عائم)
DEF_CVT_SCALE_ABS_FUNC(16s8u, cvtabs_32f, short,  uchar, عائم)
DEF_CVT_SCALE_ABS_FUNC(32u8u, cvtabs_32f, unsigned, uchar, عائم)
DEF_CVT_SCALE_ABS_FUNC(32s8u, cvtabs_32f, صحيح,    uchar, عائم)
DEF_CVT_SCALE_ABS_FUNC(32f8u, cvtabs_32f, عائم,  uchar, عائم)
DEF_CVT_SCALE_ABS_FUNC(64u8u, cvtabs_32f, uint64_t, uchar, عائم)
DEF_CVT_SCALE_ABS_FUNC(64s8u, cvtabs_32f, int64_t, uchar, عائم)
DEF_CVT_SCALE_ABS_FUNC(64f8u, cvtabs_32f, مزدوج, uchar, عائم)
DEF_CVT_SCALE_ABS_FUNC(16f8u, cvtabs_32f, hfloat, uchar, عائم)
DEF_CVT_SCALE_ABS_FUNC(16bf8u, cvtabs_32f, bfloat, uchar, عائم)

DEF_CVT_SCALE_FUNC(8u,     cvt_32f, uchar,  uchar, عائم)
DEF_CVT_SCALE_FUNC(8s8u,   cvt_32f, schar,  uchar, عائم)
DEF_CVT_SCALE_FUNC(16u8u,  cvt_32f, ushort, uchar, عائم)
DEF_CVT_SCALE_FUNC(16s8u,  cvt_32f, short,  uchar, عائم)
DEF_CVT_SCALE_FUNC(32u8u,  cvt_32f, unsigned, uchar, عائم)
DEF_CVT_SCALE_FUNC(32s8u,  cvt_32f, صحيح,    uchar, عائم)
DEF_CVT_SCALE_FUNC(32f8u,  cvt_32f, عائم,  uchar, عائم)
DEF_CVT_SCALE_FUNC(64f8u,  cvt_32f, مزدوج, uchar, عائم)
DEF_CVT_SCALE_FUNC(64u8u,  cvt_32f, uint64_t, uchar, عائم)
DEF_CVT_SCALE_FUNC(64s8u,  cvt_32f, int64_t, uchar, عائم)
DEF_CVT_SCALE_FUNC(16f8u,  cvt_32f, hfloat, uchar, عائم)
DEF_CVT_SCALE_FUNC(16bf8u, cvt_32f, bfloat, uchar, عائم)

DEF_CVT_SCALE_FUNC(8u8s,   cvt_32f, uchar,  schar, عائم)
DEF_CVT_SCALE_FUNC(8s,     cvt_32f, schar,  schar, عائم)
DEF_CVT_SCALE_FUNC(16u8s,  cvt_32f, ushort, schar, عائم)
DEF_CVT_SCALE_FUNC(16s8s,  cvt_32f, short,  schar, عائم)
DEF_CVT_SCALE_FUNC(32u8s,  cvt_32f, unsigned, schar, عائم)
DEF_CVT_SCALE_FUNC(32s8s,  cvt_32f, صحيح,    schar, عائم)
DEF_CVT_SCALE_FUNC(32f8s,  cvt_32f, عائم,  schar, عائم)
DEF_CVT_SCALE_FUNC(64f8s,  cvt_32f, مزدوج, schar, عائم)
DEF_CVT_SCALE_FUNC(64u8s,  cvt_32f, uint64_t, schar, عائم)
DEF_CVT_SCALE_FUNC(64s8s,  cvt_32f, int64_t, schar, عائم)
DEF_CVT_SCALE_FUNC(16f8s,  cvt_32f, hfloat, schar, عائم)
DEF_CVT_SCALE_FUNC(16bf8s, cvt_32f, bfloat, schar, عائم)

DEF_CVT_SCALE2BOOL_FUNC(8u8b, uchar, عائم)
DEF_CVT_SCALE2BOOL_FUNC(8s8b, schar, عائم)
DEF_CVT_SCALE2BOOL_FUNC(16u8b, ushort, عائم)
DEF_CVT_SCALE2BOOL_FUNC(16s8b, short, عائم)
DEF_CVT_SCALE2BOOL_FUNC(32u8b, unsigned, عائم)
DEF_CVT_SCALE2BOOL_FUNC(32s8b, صحيح, عائم)
DEF_CVT_SCALE2BOOL_FUNC(32f8b, عائم, عائم)
DEF_CVT_SCALE2BOOL_FUNC(64f8b, مزدوج, عائم)
DEF_CVT_SCALE2BOOL_FUNC(64u8b, uint64_t, عائم)
DEF_CVT_SCALE2BOOL_FUNC(64s8b, int64_t, عائم)
DEF_CVT_SCALE2BOOL_FUNC(16f8b, hfloat, عائم)
DEF_CVT_SCALE2BOOL_FUNC(16bf8b, bfloat, عائم)

DEF_CVT_SCALE_FUNC(8u16u,  cvt_32f, uchar,  ushort, عائم)
DEF_CVT_SCALE_FUNC(8s16u,  cvt_32f, schar,  ushort, عائم)
DEF_CVT_SCALE_FUNC(16u,    cvt_32f, ushort, ushort, عائم)
DEF_CVT_SCALE_FUNC(16s16u, cvt_32f, short,  ushort, عائم)
DEF_CVT_SCALE_FUNC(32u16u, cvt_32f, unsigned, ushort, عائم)
DEF_CVT_SCALE_FUNC(32s16u, cvt_32f, صحيح,    ushort, عائم)
DEF_CVT_SCALE_FUNC(32f16u, cvt_32f, عائم,  ushort, عائم)
DEF_CVT_SCALE_FUNC(64f16u, cvt_32f, مزدوج, ushort, عائم)
DEF_CVT_SCALE_FUNC(64u16u, cvt_32f, uint64_t, ushort, عائم)
DEF_CVT_SCALE_FUNC(64s16u, cvt_32f, int64_t, ushort, عائم)
DEF_CVT_SCALE_FUNC(16f16u, cvt1_32f, hfloat, ushort, عائم)
DEF_CVT_SCALE_FUNC(16bf16u, cvt1_32f, bfloat, ushort, عائم)

DEF_CVT_SCALE_FUNC(8u16s,  cvt_32f, uchar,  short, عائم)
DEF_CVT_SCALE_FUNC(8s16s,  cvt_32f, schar,  short, عائم)
DEF_CVT_SCALE_FUNC(16u16s, cvt_32f, ushort, short, عائم)
DEF_CVT_SCALE_FUNC(16s,    cvt_32f, short,  short, عائم)
DEF_CVT_SCALE_FUNC(32u16s, cvt_32f, unsigned, short, عائم)
DEF_CVT_SCALE_FUNC(32s16s, cvt_32f, صحيح,    short, عائم)
DEF_CVT_SCALE_FUNC(32f16s, cvt_32f, عائم,  short, عائم)
DEF_CVT_SCALE_FUNC(64f16s, cvt_32f, مزدوج, short, عائم)
DEF_CVT_SCALE_FUNC(64u16s, cvt_32f, uint64_t, short, عائم)
DEF_CVT_SCALE_FUNC(64s16s, cvt_32f, int64_t, short, عائم)
DEF_CVT_SCALE_FUNC(16f16s, cvt1_32f, hfloat, short, عائم)
DEF_CVT_SCALE_FUNC(16bf16s, cvt1_32f, bfloat, short, عائم)

DEF_CVT_SCALE_FUNC(8u32u,  cvt_32f, uchar,  unsigned, عائم)
DEF_CVT_SCALE_FUNC(8s32u,  cvt_32f, schar,  unsigned, عائم)
DEF_CVT_SCALE_FUNC(16u32u, cvt_32f, ushort, unsigned, عائم)
DEF_CVT_SCALE_FUNC(16s32u, cvt_32f, short,  unsigned, عائم)
DEF_CVT_SCALE_FUNC(32u, cvt_32f, unsigned, unsigned, عائم)
DEF_CVT_SCALE_FUNC(32s32u, cvt_64f, صحيح,    unsigned, مزدوج)
DEF_CVT_SCALE_FUNC(32f32u, cvt_32f, عائم,  unsigned, عائم)
DEF_CVT_SCALE_FUNC(64f32u, cvt_64f, مزدوج, unsigned, مزدوج)
DEF_CVT_SCALE_FUNC(64u32u, cvt_32f, uint64_t, unsigned, عائم)
DEF_CVT_SCALE_FUNC(64s32u, cvt_32f, int64_t, unsigned, عائم)
DEF_CVT_SCALE_FUNC(16f32u, cvt1_32f, hfloat, unsigned, عائم)
DEF_CVT_SCALE_FUNC(16bf32u, cvt1_32f, bfloat, unsigned, عائم)

DEF_CVT_SCALE_FUNC(8u32s,  cvt_32f, uchar,  صحيح, عائم)
DEF_CVT_SCALE_FUNC(8s32s,  cvt_32f, schar,  صحيح, عائم)
DEF_CVT_SCALE_FUNC(16u32s, cvt_32f, ushort, صحيح, عائم)
DEF_CVT_SCALE_FUNC(16s32s, cvt_32f, short,  صحيح, عائم)
DEF_CVT_SCALE_FUNC(32u32s, cvt_32f, unsigned, صحيح, عائم)
DEF_CVT_SCALE_FUNC(32s,    cvt_64f, صحيح,    صحيح, مزدوج)
DEF_CVT_SCALE_FUNC(32f32s, cvt_32f, عائم,  صحيح, عائم)
DEF_CVT_SCALE_FUNC(64f32s, cvt_64f, مزدوج, صحيح, مزدوج)
DEF_CVT_SCALE_FUNC(64u32s, cvt_32f, uint64_t, صحيح, عائم)
DEF_CVT_SCALE_FUNC(64s32s, cvt_32f, int64_t, صحيح, عائم)
DEF_CVT_SCALE_FUNC(16f32s, cvt1_32f, hfloat, صحيح, عائم)
DEF_CVT_SCALE_FUNC(16bf32s, cvt1_32f, bfloat, صحيح, عائم)

DEF_CVT_SCALE_FUNC(8u32f,  cvt_32f, uchar,  عائم, عائم)
DEF_CVT_SCALE_FUNC(8s32f,  cvt_32f, schar,  عائم, عائم)
DEF_CVT_SCALE_FUNC(16u32f, cvt_32f, ushort, عائم, عائم)
DEF_CVT_SCALE_FUNC(16s32f, cvt_32f, short,  عائم, عائم)
DEF_CVT_SCALE_FUNC(32u32f, cvt_32f, unsigned, عائم, عائم)
DEF_CVT_SCALE_FUNC(32s32f, cvt_32f, صحيح,    عائم, عائم)
DEF_CVT_SCALE_FUNC(32f,    cvt_32f, عائم,  عائم, عائم)
DEF_CVT_SCALE_FUNC(64f32f, cvt_64f, مزدوج, عائم, مزدوج)
DEF_CVT_SCALE_FUNC(64u32f, cvt_32f, uint64_t, عائم, عائم)
DEF_CVT_SCALE_FUNC(64s32f, cvt_32f, int64_t, عائم, عائم)
DEF_CVT_SCALE_FUNC(16f32f, cvt1_32f, hfloat, عائم, عائم)
DEF_CVT_SCALE_FUNC(16bf32f, cvt1_32f, bfloat, عائم, عائم)

DEF_CVT_SCALE_FUNC(8u64f,  cvt_64f, uchar,  مزدوج, مزدوج)
DEF_CVT_SCALE_FUNC(8s64f,  cvt_64f, schar,  مزدوج, مزدوج)
DEF_CVT_SCALE_FUNC(16u64f, cvt_64f, ushort, مزدوج, مزدوج)
DEF_CVT_SCALE_FUNC(16s64f, cvt_64f, short,  مزدوج, مزدوج)
DEF_CVT_SCALE_FUNC(32u64f, cvt_64f, unsigned, مزدوج, مزدوج)
DEF_CVT_SCALE_FUNC(32s64f, cvt_64f, صحيح,    مزدوج, مزدوج)
DEF_CVT_SCALE_FUNC(32f64f, cvt_64f, عائم,  مزدوج, مزدوج)
DEF_CVT_SCALE_FUNC(64f,    cvt_64f, مزدوج, مزدوج, مزدوج)
DEF_CVT_SCALE_FUNC(64u64f, cvt_64f, uint64_t, مزدوج, مزدوج)
DEF_CVT_SCALE_FUNC(64s64f, cvt_64f, int64_t, مزدوج, مزدوج)
DEF_CVT_SCALE_FUNC(16f64f, cvt_64f, hfloat, مزدوج, مزدوج)
DEF_CVT_SCALE_FUNC(16bf64f, cvt_64f, bfloat, مزدوج, مزدوج)

DEF_CVT_SCALE_FUNC(8u64u,  cvt_64f, uchar,  uint64_t, مزدوج)
DEF_CVT_SCALE_FUNC(8s64u,  cvt_64f, schar,  uint64_t, مزدوج)
DEF_CVT_SCALE_FUNC(16u64u, cvt_64f, ushort, uint64_t, مزدوج)
DEF_CVT_SCALE_FUNC(16s64u, cvt_64f, short,  uint64_t, مزدوج)
DEF_CVT_SCALE_FUNC(32u64u, cvt_64f, unsigned, uint64_t, مزدوج)
DEF_CVT_SCALE_FUNC(32s64u, cvt_64f, صحيح,    uint64_t, مزدوج)
DEF_CVT_SCALE_FUNC(32f64u, cvt_64f, عائم,  uint64_t, مزدوج)
DEF_CVT_SCALE_FUNC(64f64u, cvt_64f, مزدوج, uint64_t, مزدوج)
DEF_CVT_SCALE_FUNC(64u, cvt_64f, uint64_t, uint64_t, مزدوج)
DEF_CVT_SCALE_FUNC(64s64u, cvt_64f, int64_t, uint64_t, مزدوج)
DEF_CVT_SCALE_FUNC(16f64u, cvt_64f, hfloat, uint64_t, مزدوج)
DEF_CVT_SCALE_FUNC(16bf64u, cvt_64f, bfloat, uint64_t, مزدوج)

DEF_CVT_SCALE_FUNC(8u64s,  cvt_64f, uchar,  int64_t, مزدوج)
DEF_CVT_SCALE_FUNC(8s64s,  cvt_64f, schar,  int64_t, مزدوج)
DEF_CVT_SCALE_FUNC(16u64s, cvt_64f, ushort, int64_t, مزدوج)
DEF_CVT_SCALE_FUNC(16s64s, cvt_64f, short,  int64_t, مزدوج)
DEF_CVT_SCALE_FUNC(32u64s, cvt_64f, unsigned, int64_t, مزدوج)
DEF_CVT_SCALE_FUNC(32s64s, cvt_64f, صحيح,    int64_t, مزدوج)
DEF_CVT_SCALE_FUNC(32f64s, cvt_64f, عائم,  int64_t, مزدوج)
DEF_CVT_SCALE_FUNC(64f64s, cvt_64f, مزدوج, int64_t, مزدوج)
DEF_CVT_SCALE_FUNC(64u64s, cvt_64f, uint64_t, int64_t, مزدوج)
DEF_CVT_SCALE_FUNC(64s, cvt_64f, int64_t, int64_t, مزدوج)
DEF_CVT_SCALE_FUNC(16f64s, cvt_64f, hfloat, int64_t, مزدوج)
DEF_CVT_SCALE_FUNC(16bf64s, cvt_64f, bfloat, int64_t, مزدوج)

DEF_CVT_SCALE_FUNC(8u16f,  cvt1_32f, uchar,  hfloat, عائم)
DEF_CVT_SCALE_FUNC(8s16f,  cvt1_32f, schar,  hfloat, عائم)
DEF_CVT_SCALE_FUNC(16u16f, cvt1_32f, ushort, hfloat, عائم)
DEF_CVT_SCALE_FUNC(16s16f, cvt1_32f, short,  hfloat, عائم)
DEF_CVT_SCALE_FUNC(32u16f, cvt1_32f, unsigned, hfloat, عائم)
DEF_CVT_SCALE_FUNC(32s16f, cvt1_32f, صحيح,    hfloat, عائم)
DEF_CVT_SCALE_FUNC(32f16f, cvt1_32f, عائم,  hfloat, عائم)
DEF_CVT_SCALE_FUNC(64f16f, cvt1_32f, مزدوج, hfloat, عائم)
DEF_CVT_SCALE_FUNC(64u16f, cvt1_32f, uint64_t, hfloat, عائم)
DEF_CVT_SCALE_FUNC(64s16f, cvt1_32f, int64_t, hfloat, عائم)
DEF_CVT_SCALE_FUNC(16f,    cvt1_32f, hfloat, hfloat, عائم)
DEF_CVT_SCALE_FUNC(16bf16f, cvt1_32f, bfloat, hfloat, عائم)

DEF_CVT_SCALE_FUNC(8u16bf,  cvt1_32f, uchar,  bfloat, عائم)
DEF_CVT_SCALE_FUNC(8s16bf,  cvt1_32f, schar,  bfloat, عائم)
DEF_CVT_SCALE_FUNC(16u16bf, cvt1_32f, ushort, bfloat, عائم)
DEF_CVT_SCALE_FUNC(16s16bf, cvt1_32f, short,  bfloat, عائم)
DEF_CVT_SCALE_FUNC(32u16bf, cvt1_32f, unsigned, bfloat, عائم)
DEF_CVT_SCALE_FUNC(32s16bf, cvt1_32f, صحيح,    bfloat, عائم)
DEF_CVT_SCALE_FUNC(32f16bf, cvt1_32f, عائم,  bfloat, عائم)
DEF_CVT_SCALE_FUNC(64f16bf, cvt1_32f, مزدوج, bfloat, عائم)
DEF_CVT_SCALE_FUNC(64u16bf, cvt1_32f, uint64_t, bfloat, عائم)
DEF_CVT_SCALE_FUNC(64s16bf, cvt1_32f, int64_t, bfloat, عائم)
DEF_CVT_SCALE_FUNC(16f16bf, cvt1_32f, hfloat, bfloat, عائم)
DEF_CVT_SCALE_FUNC(16bf, cvt1_32f, bfloat, bfloat, عائم)

DEF_CVT_SCALEBOOL2_FUNC(8b8u, uchar, عائم)
DEF_CVT_SCALEBOOL2_FUNC(8b8s, schar, عائم)
DEF_CVT_SCALEBOOL2_FUNC(8b, منطقي, عائم)
DEF_CVT_SCALEBOOL2_FUNC(8b16u, ushort, عائم)
DEF_CVT_SCALEBOOL2_FUNC(8b16s, short, عائم)
DEF_CVT_SCALEBOOL2_FUNC(8b32u, unsigned, عائم)
DEF_CVT_SCALEBOOL2_FUNC(8b32s, صحيح, عائم)
DEF_CVT_SCALEBOOL2_FUNC(8b32f, عائم, عائم)
DEF_CVT_SCALEBOOL2_FUNC(8b64u, uint64_t, مزدوج)
DEF_CVT_SCALEBOOL2_FUNC(8b64s, int64_t, مزدوج)
DEF_CVT_SCALEBOOL2_FUNC(8b64f, مزدوج, مزدوج)
DEF_CVT_SCALEBOOL2_FUNC(8b16f, hfloat, عائم)
DEF_CVT_SCALEBOOL2_FUNC(8b16bf, bfloat, عائم)

// FP8 src -> CV_8U abs|scale|shift (scalar)
#define DEF_CVT_SCALE_ABS_SCALAR_FUNC(suffix, stype) \
ساكن فراغ cvtScaleAbs##suffix( ثابت uchar* src_, size_t sstep, ثابت uchar*, size_t, \
                                 uchar* dst, size_t dstep, Size size, فراغ* scale_) \
{ \
    ثابت stype* src = (ثابت stype*)src_; \
    ثابت مزدوج* scale = (ثابت مزدوج*)scale_; \
    عائم a = (عائم)scale[0], b = (عائم)scale[1]; \
    sstep /= حجم(src[0]); \
    لكل( صحيح i = 0; i < size.height; i++, src += sstep, dst += dstep ) \
        لكل (صحيح j = 0; j < size.width; j++) \
            dst[j] = saturate_cast<uchar>(std::abs((عائم)src[j]*a + b)); \
}
DEF_CVT_SCALE_ABS_SCALAR_FUNC(8fe4m3u8u, fp8a_t)
DEF_CVT_SCALE_ABS_SCALAR_FUNC(8fe4m38u,  fp8_t)

ثنائی_Func getCvtScaleAbsFunc(صحيح depth)
{
    ثنائی_Func func =
        depth == CV_8F_E4M3FN ? (ثنائی_Func)cvtScaleAbs8fe4m38u :
        depth == CV_8F_E4M3FNUZ ? (ثنائی_Func)cvtScaleAbs8fe4m3u8u :
        depth == CV_8U ? (ثنائی_Func)cvtScaleAbs8u :
        depth == CV_8S ? (ثنائی_Func)cvtScaleAbs8s8u :
        depth == CV_Bool ? (ثنائی_Func)cvtScaleAbs8b8u :
        depth == CV_16U ? (ثنائی_Func)cvtScaleAbs16u8u :
        depth == CV_16S ? (ثنائی_Func)cvtScaleAbs16s8u :
        depth == CV_16F ? (ثنائی_Func)cvtScaleAbs16f8u :
        depth == CV_16BF ? (ثنائی_Func)cvtScaleAbs16bf8u :
        depth == CV_32U ? (ثنائی_Func)cvtScaleAbs32u8u :
        depth == CV_32S ? (ثنائی_Func)cvtScaleAbs32s8u :
        depth == CV_32F ? (ثنائی_Func)cvtScaleAbs32f8u :
        depth == CV_64U ? (ثنائی_Func)cvtScaleAbs64u8u :
        depth == CV_64S ? (ثنائی_Func)cvtScaleAbs64s8u :
        depth == CV_64F ? (ثنائی_Func)cvtScaleAbs64f8u : 0;
    CV_Assert(func != 0);
    أرجع func;
}

//////////////////// FP8 scaled conversions — scalar via saturate_cast(src*a+b) ////////////////////
#define DEF_CVT_SCALE_SCALAR_FUNC(suffix, stype, dtype, wtype) \
ساكن فراغ cvtScale##suffix( ثابت uchar* src_, size_t sstep, ثابت uchar*, size_t, \
                              uchar* dst_, size_t dstep, Size size, فراغ* scale_) \
{ \
    ثابت stype* src = (ثابت stype*)src_; dtype* dst = (dtype*)dst_; \
    ثابت مزدوج* scale = (ثابت مزدوج*)scale_; \
    wtype a = (wtype)scale[0], b = (wtype)scale[1]; \
    sstep /= حجم(src[0]); dstep /= حجم(dst[0]); \
    لكل( صحيح i = 0; i < size.height; i++, src += sstep, dst += dstep ) \
        لكل (صحيح j = 0; j < size.width; j++) \
            dst[j] = saturate_cast<dtype>((wtype)src[j]*a + b); \
}

// double intermediate so coarse FP8 grids round identically to the (double-precision) reference
#define DEF_CVT_SCALE_FP8(S, T) \
    DEF_CVT_SCALE_SCALAR_FUNC(S##8u,   T, uchar, مزدوج)     DEF_CVT_SCALE_SCALAR_FUNC(8u##S,   uchar, T, مزدوج) \
    DEF_CVT_SCALE_SCALAR_FUNC(S##8s,   T, schar, مزدوج)     DEF_CVT_SCALE_SCALAR_FUNC(8s##S,   schar, T, مزدوج) \
    DEF_CVT_SCALE_SCALAR_FUNC(S##16u,  T, ushort, مزدوج)    DEF_CVT_SCALE_SCALAR_FUNC(16u##S,  ushort, T, مزدوج) \
    DEF_CVT_SCALE_SCALAR_FUNC(S##16s,  T, short, مزدوج)     DEF_CVT_SCALE_SCALAR_FUNC(16s##S,  short, T, مزدوج) \
    DEF_CVT_SCALE_SCALAR_FUNC(S##32u,  T, unsigned, مزدوج)  DEF_CVT_SCALE_SCALAR_FUNC(32u##S,  unsigned, T, مزدوج) \
    DEF_CVT_SCALE_SCALAR_FUNC(S##32s,  T, صحيح, مزدوج)       DEF_CVT_SCALE_SCALAR_FUNC(32s##S,  صحيح, T, مزدوج) \
    DEF_CVT_SCALE_SCALAR_FUNC(S##32f,  T, عائم, مزدوج) \
    DEF_CVT_SCALE_SCALAR_FUNC(S##64f,  T, مزدوج, مزدوج)    DEF_CVT_SCALE_SCALAR_FUNC(64f##S,  مزدوج, T, مزدوج) \
    DEF_CVT_SCALE_SCALAR_FUNC(S##16f,  T, hfloat, مزدوج)    DEF_CVT_SCALE_SCALAR_FUNC(16f##S,  hfloat, T, مزدوج) \
    DEF_CVT_SCALE_SCALAR_FUNC(S##16bf, T, bfloat, مزدوج)    DEF_CVT_SCALE_SCALAR_FUNC(16bf##S, bfloat, T, مزدوج) \
    DEF_CVT_SCALE_SCALAR_FUNC(S##64u,  T, uint64_t, مزدوج)  DEF_CVT_SCALE_SCALAR_FUNC(64u##S,  uint64_t, T, مزدوج) \
    DEF_CVT_SCALE_SCALAR_FUNC(S##64s,  T, int64_t, مزدوج)   DEF_CVT_SCALE_SCALAR_FUNC(64s##S,  int64_t, T, مزدوج) \
    DEF_CVT_SCALE_SCALAR_FUNC(S##8b,   T, منطقي, مزدوج) \
    DEF_CVT_SCALE_SCALAR_FUNC(S##8fe4m3,  T, fp8_t, مزدوج)   DEF_CVT_SCALE_SCALAR_FUNC(S##8fe4m3u, T, fp8a_t, مزدوج)

DEF_CVT_SCALE_FP8(8fe4m3,  fp8_t)
DEF_CVT_SCALE_FP8(8fe4m3u, fp8a_t)

// convertTo(dst, depth, alpha, beta) path -- real quantization. Reuses convert.hpp's fast path.
// Double precision here matches the scalar reference (wtype)x*a+b bit-for-bit.
template<صحيح bias, منطقي fnuz> ساكن فراغ
cvtScaleF32ToFp8(ثابت uchar* src_, size_t sstep, ثابت uchar*, size_t,
                 uchar* dst, size_t dstep, Size size, فراغ* scale_)
{
    CV_INSTRUMENT_REGION();
    ثابت عائم* src = (ثابت عائم*)src_;
    ثابت مزدوج* scale = (ثابت مزدوج*)scale_;
    مزدوج a = scale[0], b = scale[1];
    sstep /= حجم(src[0]);

    لكل (صحيح i = 0; i < size.height; i++, src += sstep, dst += dstep)
    {
        صحيح j = 0;
#إذا (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)
        v_float64 va = vx_setall_f64(a), vb = vx_setall_f64(b);
        ثابت صحيح LANES32 = VTraits<v_float32>::vlanes();
        ثابت صحيح VECSZ = LANES32 * 4;
        عائم scaled[VTraits<v_float32>::max_nlanes * 4];
        لكل (; j <= size.width - VECSZ; j += VECSZ)
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
        لكل (; j < size.width; j++)
            dst[j] = fp8_detail::encodeE4M3((عائم)((مزدوج)src[j]*a + b), bias, fnuz);
    }
}
ساكن فراغ cvtScale32f8fe4m3(ثابت uchar* s, size_t ss, ثابت uchar* p, size_t ps, uchar* d, size_t ds, Size sz, فراغ* x)
{ cvtScaleF32ToFp8<7, خطأ>(s, ss, p, ps, d, ds, sz, x); }
ساكن فراغ cvtScale32f8fe4m3u(ثابت uchar* s, size_t ss, ثابت uchar* p, size_t ps, uchar* d, size_t ds, Size sz, فراغ* x)
{ cvtScaleF32ToFp8<8, true>(s, ss, p, ps, d, ds, sz, x); }

ثنائی_Func getConvertScaleFunc(صحيح sdepth_, صحيح ddepth_)
{
    صحيح sdepth = CV_MAT_DEPTH(sdepth_);
    صحيح ddepth = CV_MAT_DEPTH(ddepth_);
    ثنائی_Func func =
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
    أرجع func;
}

#endif

CV_CPU_OPTIMIZATION_NAMESPACE_END
} // namespace
