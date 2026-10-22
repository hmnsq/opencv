// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


#അന്തര്ഭാവ "precomp.hpp"
#അന്തര്ഭാവ "convert.hpp"

നാമാകാശ cv {
CV_CPU_OPTIMIZATION_NAMESPACE_BEGIN

ദ്വിമാന_Func getCvtScaleAbsFunc(പൂർണ്ണസംഖ്യ depth);
ദ്വിമാന_Func getConvertScaleFunc(പൂർണ്ണസംഖ്യ sdepth, പൂർണ്ണസംഖ്യ ddepth);

#ifndef CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

/****************************************************************************************\
*                                convertScale[Abs]                                       *
\****************************************************************************************/

template<typename _Ts, typename _Td> വരിയിൽ ശൂന്യം
cvtabs_32f( സ്ഥിരം _Ts* src, size_t sstep, _Td* dst, size_t dstep,
            Size size, ദശാംശം a, ദശാംശം b )
{
#എങ്കിൽ (CV_SIMD || CV_SIMD_SCALABLE)
    v_float32 va = vx_setall_f32(a), vb = vx_setall_f32(b);
    സ്ഥിരം പൂർണ്ണസംഖ്യ VECSZ = VTraits<v_float32>::vlanes()*2;
    // GCC miscompiles this scalable block only on VLEN=128 RVV (opencv/issues/26936);
    // use scalar there, keep SIMD on wider vectors. Ask the hardware for VLEN directly:
    // vlanes() would depend on the LMUL the universal intrinsics happen to use.
#എങ്കിൽ (CV_RVV && CV_SIMD_SCALABLE && defined(__GNUC__) && !defined(__clang__))
    സ്ഥിരം തർക്കം useSIMD = __riscv_vlenb()*8 != 128;
#അല്ലെങ്കിൽ
    സ്ഥിരം തർക്കം useSIMD = സത്യം;
#endif
#endif
    sstep /= വലുപ്പം(src[0]);
    dstep /= വലുപ്പം(dst[0]);

    വേണ്ടി( പൂർണ്ണസംഖ്യ i = 0; i < size.height; i++, src += sstep, dst += dstep )
    {
        പൂർണ്ണസംഖ്യ j = 0;
#എങ്കിൽ (CV_SIMD || CV_SIMD_SCALABLE)
        എങ്കിൽ( useSIMD )
        വേണ്ടി( ; j < size.width; j += VECSZ )
        {
            എങ്കിൽ( j > size.width - VECSZ )
            {
                എങ്കിൽ( j == 0 || src == (_Ts*)dst )
                    നിർത്തുക;
                j = size.width - VECSZ;
            }
            v_float32 v0, v1;
            vx_load_pair_as(src + j, v0, v1);
            v0 = v_fma(v0, va, vb);
            v1 = v_fma(v1, va, vb);
            v_store_pair_as(dst + j, v_abs(v0), v_abs(v1));
        }
#endif
        വേണ്ടി( ; j < size.width; j++ )
            dst[j] = saturate_cast<_Td>(std::abs(src[j]*a + b));
    }
}

സ്ഥിതം ശൂന്യം
cvtabs_32f( സ്ഥിരം തർക്കം* src_, size_t sstep,
            uchar* dst, size_t dstep,
            Size size, ദശാംശം a, ദശാംശം b )
{
    സ്ഥിരം uchar* src = (സ്ഥിരം uchar*)src_;
    uchar v0 = saturate_cast<uchar>(std::abs(b));
    uchar v1 = saturate_cast<uchar>(std::abs(a + b));
    വേണ്ടി( പൂർണ്ണസംഖ്യ i = 0; i < size.height; i++, src += sstep, dst += dstep )
    {
        വേണ്ടി (പൂർണ്ണസംഖ്യ j = 0; j < size.width; j++)
            dst[j] = src[j] != 0 ? v1 : v0;
    }
}

template<typename _Ts, typename _Td> വരിയിൽ ശൂന്യം
cvt_32f( സ്ഥിരം _Ts* src, size_t sstep, _Td* dst, size_t dstep,
         Size size, ദശാംശം a, ദശാംശം b )
{
#എങ്കിൽ (CV_SIMD || (CV_SIMD_SCALABLE && !(defined(__GNUC__) && !defined(__clang__))) )
    v_float32 va = vx_setall_f32(a), vb = vx_setall_f32(b);
    സ്ഥിരം പൂർണ്ണസംഖ്യ VECSZ = VTraits<v_float32>::vlanes()*2;
#endif
    sstep /= വലുപ്പം(src[0]);
    dstep /= വലുപ്പം(dst[0]);

    വേണ്ടി( പൂർണ്ണസംഖ്യ i = 0; i < size.height; i++, src += sstep, dst += dstep )
    {
        പൂർണ്ണസംഖ്യ j = 0;
// Excluding GNU in CV_SIMD_SCALABLE because of "opencv/issues/26936"
#എങ്കിൽ (CV_SIMD || (CV_SIMD_SCALABLE && !(defined(__GNUC__) && !defined(__clang__))) )
        വേണ്ടി( ; j < size.width; j += VECSZ )
        {
            എങ്കിൽ( j > size.width - VECSZ )
            {
                എങ്കിൽ( j == 0 || src == (_Ts*)dst )
                    നിർത്തുക;
                j = size.width - VECSZ;
            }
            v_float32 v0, v1;
            vx_load_pair_as(src + j, v0, v1);
            v0 = v_fma(v0, va, vb);
            v1 = v_fma(v1, va, vb);
            v_store_pair_as(dst + j, v0, v1);
        }
#endif
        വേണ്ടി( ; j < size.width; j++ )
            dst[j] = saturate_cast<_Td>(src[j]*a + b);
    }
}

// variant for conversions 16f <-> ... w/o unrolling
template<typename _Ts, typename _Td> വരിയിൽ ശൂന്യം
cvt1_32f( സ്ഥിരം _Ts* src, size_t sstep, _Td* dst, size_t dstep,
          Size size, ദശാംശം a, ദശാംശം b )
{
#എങ്കിൽ (CV_SIMD || CV_SIMD_SCALABLE)
    v_float32 va = vx_setall_f32(a), vb = vx_setall_f32(b);
    സ്ഥിരം പൂർണ്ണസംഖ്യ VECSZ = VTraits<v_float32>::vlanes();
#endif
    sstep /= വലുപ്പം(src[0]);
    dstep /= വലുപ്പം(dst[0]);

    വേണ്ടി( പൂർണ്ണസംഖ്യ i = 0; i < size.height; i++, src += sstep, dst += dstep )
    {
        പൂർണ്ണസംഖ്യ j = 0;
#എങ്കിൽ (CV_SIMD || CV_SIMD_SCALABLE)
        വേണ്ടി( ; j < size.width; j += VECSZ )
        {
            എങ്കിൽ( j > size.width - VECSZ )
            {
                എങ്കിൽ( j == 0 || src == (_Ts*)dst )
                    നിർത്തുക;
                j = size.width - VECSZ;
            }
            v_float32 v0;
            vx_load_as(src + j, v0);
            v0 = v_fma(v0, va, vb);
            v_store_as(dst + j, v0);
        }
#endif
        വേണ്ടി( ; j < size.width; j++ )
            dst[j] = saturate_cast<_Td>(src[j]*a + b);
    }
}


template<typename _Ts, typename _Td> വരിയിൽ ശൂന്യം
cvt_64f( സ്ഥിരം _Ts* src, size_t sstep, _Td* dst, size_t dstep,
         Size size, ഇരട്ട a, ഇരട്ട b )
{
#എങ്കിൽ (CV_SIMD_64F || (CV_SIMD_SCALABLE_64F && !(defined(__GNUC__) && !defined(__clang__))) )
    v_float64 va = vx_setall_f64(a), vb = vx_setall_f64(b);
    സ്ഥിരം പൂർണ്ണസംഖ്യ VECSZ = VTraits<v_float64>::vlanes()*2;
#endif
    sstep /= വലുപ്പം(src[0]);
    dstep /= വലുപ്പം(dst[0]);

    വേണ്ടി( പൂർണ്ണസംഖ്യ i = 0; i < size.height; i++, src += sstep, dst += dstep )
    {
        പൂർണ്ണസംഖ്യ j = 0;
// Excluding GNU in CV_SIMD_SCALABLE because of "opencv/issues/26936"
#എങ്കിൽ (CV_SIMD_64F || (CV_SIMD_SCALABLE_64F && !(defined(__GNUC__) && !defined(__clang__))) )
        വേണ്ടി( ; j < size.width; j += VECSZ )
        {
            എങ്കിൽ( j > size.width - VECSZ )
            {
                എങ്കിൽ( j == 0 || src == (_Ts*)dst )
                    നിർത്തുക;
                j = size.width - VECSZ;
            }
            v_float64 v0, v1;
            vx_load_pair_as(src + j, v0, v1);
            v0 = v_fma(v0, va, vb);
            v1 = v_fma(v1, va, vb);
            v_store_pair_as(dst + j, v0, v1);
        }
#endif
        വേണ്ടി( ; j < size.width; j++ )
            dst[j] = saturate_cast<_Td>(src[j]*a + b);
    }
}

//==================================================================================================

#define DEF_CVT_SCALE_ABS_FUNC(suffix, cvt, stype, dtype, wtype) \
സ്ഥിതം ശൂന്യം cvtScaleAbs##suffix( സ്ഥിരം uchar* src_, size_t sstep, സ്ഥിരം uchar*, size_t, \
                                 uchar* dst_, size_t dstep, Size size, ശൂന്യം* scale_) \
{ \
    സ്ഥിരം stype* src = (സ്ഥിരം stype*)src_; \
    dtype* dst = (dtype*)dst_; \
    ഇരട്ട* scale = (ഇരട്ട*)scale_; \
    cvt(src, sstep, dst, dstep, size, (wtype)scale[0], (wtype)scale[1]); \
}


#define DEF_CVT_SCALE_FUNC(suffix, cvt, stype, dtype, wtype) \
സ്ഥിതം ശൂന്യം cvtScale##suffix( സ്ഥിരം uchar* src_, size_t sstep, സ്ഥിരം uchar*, size_t, \
                              uchar* dst_, size_t dstep, Size size, ശൂന്യം* scale_) \
{ \
    സ്ഥിരം stype* src = (സ്ഥിരം stype*)src_; \
    dtype* dst = (dtype*)dst_; \
    ഇരട്ട* scale = (ഇരട്ട*)scale_; \
    cvt(src, sstep, dst, dstep, size, (wtype)scale[0], (wtype)scale[1]); \
}

#define DEF_CVT_SCALE2BOOL_FUNC(suffix, stype, wtype) \
സ്ഥിതം ശൂന്യം cvtScale##suffix( സ്ഥിരം uchar* src_, size_t sstep, സ്ഥിരം uchar*, size_t, \
                              uchar* dst, size_t dstep, Size size, ശൂന്യം* scale_) \
{ \
    സ്ഥിരം stype* src = (സ്ഥിരം stype*)src_; \
    സ്ഥിരം ഇരട്ട* scale = (സ്ഥിരം ഇരട്ട*)scale_; \
    wtype a = (wtype)scale[0], b = (wtype)scale[1]; \
    sstep /= വലുപ്പം(src[0]); \
    വേണ്ടി( പൂർണ്ണസംഖ്യ i = 0; i < size.height; i++, src += sstep, dst += dstep ) \
        വേണ്ടി (പൂർണ്ണസംഖ്യ j = 0; j < size.width; j++) \
            dst[j] = (തർക്കം)((wtype)src[j]*a + b != 0); \
}

#define DEF_CVT_SCALEBOOL2_FUNC(suffix, dtype, wtype) \
സ്ഥിതം ശൂന്യം cvtScale##suffix( സ്ഥിരം uchar* src, size_t sstep, സ്ഥിരം uchar*, size_t, \
                              uchar* dst_, size_t dstep, Size size, ശൂന്യം* scale_) \
{ \
    dtype* dst = (dtype*)dst_; \
    സ്ഥിരം ഇരട്ട* scale = (സ്ഥിരം ഇരട്ട*)scale_; \
    wtype a = (wtype)scale[0], b = (wtype)scale[1]; \
    dtype v0 = saturate_cast<dtype>(b), v1 = saturate_cast<dtype>(a + b); \
    dstep /= വലുപ്പം(dst[0]); \
    വേണ്ടി( പൂർണ്ണസംഖ്യ i = 0; i < size.height; i++, src += sstep, dst += dstep ) \
        വേണ്ടി (പൂർണ്ണസംഖ്യ j = 0; j < size.width; j++) \
            dst[j] = src[j] != 0 ? v1 : v0; \
}

DEF_CVT_SCALE_ABS_FUNC(8u,    cvtabs_32f, uchar,  uchar, ദശാംശം)
DEF_CVT_SCALE_ABS_FUNC(8s8u,  cvtabs_32f, schar,  uchar, ദശാംശം)
DEF_CVT_SCALE_ABS_FUNC(8b8u,  cvtabs_32f, തർക്കം,  uchar, ദശാംശം)
DEF_CVT_SCALE_ABS_FUNC(16u8u, cvtabs_32f, ushort, uchar, ദശാംശം)
DEF_CVT_SCALE_ABS_FUNC(16s8u, cvtabs_32f, short,  uchar, ദശാംശം)
DEF_CVT_SCALE_ABS_FUNC(32u8u, cvtabs_32f, unsigned, uchar, ദശാംശം)
DEF_CVT_SCALE_ABS_FUNC(32s8u, cvtabs_32f, പൂർണ്ണസംഖ്യ,    uchar, ദശാംശം)
DEF_CVT_SCALE_ABS_FUNC(32f8u, cvtabs_32f, ദശാംശം,  uchar, ദശാംശം)
DEF_CVT_SCALE_ABS_FUNC(64u8u, cvtabs_32f, uint64_t, uchar, ദശാംശം)
DEF_CVT_SCALE_ABS_FUNC(64s8u, cvtabs_32f, int64_t, uchar, ദശാംശം)
DEF_CVT_SCALE_ABS_FUNC(64f8u, cvtabs_32f, ഇരട്ട, uchar, ദശാംശം)
DEF_CVT_SCALE_ABS_FUNC(16f8u, cvtabs_32f, hfloat, uchar, ദശാംശം)
DEF_CVT_SCALE_ABS_FUNC(16bf8u, cvtabs_32f, bfloat, uchar, ദശാംശം)

DEF_CVT_SCALE_FUNC(8u,     cvt_32f, uchar,  uchar, ദശാംശം)
DEF_CVT_SCALE_FUNC(8s8u,   cvt_32f, schar,  uchar, ദശാംശം)
DEF_CVT_SCALE_FUNC(16u8u,  cvt_32f, ushort, uchar, ദശാംശം)
DEF_CVT_SCALE_FUNC(16s8u,  cvt_32f, short,  uchar, ദശാംശം)
DEF_CVT_SCALE_FUNC(32u8u,  cvt_32f, unsigned, uchar, ദശാംശം)
DEF_CVT_SCALE_FUNC(32s8u,  cvt_32f, പൂർണ്ണസംഖ്യ,    uchar, ദശാംശം)
DEF_CVT_SCALE_FUNC(32f8u,  cvt_32f, ദശാംശം,  uchar, ദശാംശം)
DEF_CVT_SCALE_FUNC(64f8u,  cvt_32f, ഇരട്ട, uchar, ദശാംശം)
DEF_CVT_SCALE_FUNC(64u8u,  cvt_32f, uint64_t, uchar, ദശാംശം)
DEF_CVT_SCALE_FUNC(64s8u,  cvt_32f, int64_t, uchar, ദശാംശം)
DEF_CVT_SCALE_FUNC(16f8u,  cvt_32f, hfloat, uchar, ദശാംശം)
DEF_CVT_SCALE_FUNC(16bf8u, cvt_32f, bfloat, uchar, ദശാംശം)

DEF_CVT_SCALE_FUNC(8u8s,   cvt_32f, uchar,  schar, ദശാംശം)
DEF_CVT_SCALE_FUNC(8s,     cvt_32f, schar,  schar, ദശാംശം)
DEF_CVT_SCALE_FUNC(16u8s,  cvt_32f, ushort, schar, ദശാംശം)
DEF_CVT_SCALE_FUNC(16s8s,  cvt_32f, short,  schar, ദശാംശം)
DEF_CVT_SCALE_FUNC(32u8s,  cvt_32f, unsigned, schar, ദശാംശം)
DEF_CVT_SCALE_FUNC(32s8s,  cvt_32f, പൂർണ്ണസംഖ്യ,    schar, ദശാംശം)
DEF_CVT_SCALE_FUNC(32f8s,  cvt_32f, ദശാംശം,  schar, ദശാംശം)
DEF_CVT_SCALE_FUNC(64f8s,  cvt_32f, ഇരട്ട, schar, ദശാംശം)
DEF_CVT_SCALE_FUNC(64u8s,  cvt_32f, uint64_t, schar, ദശാംശം)
DEF_CVT_SCALE_FUNC(64s8s,  cvt_32f, int64_t, schar, ദശാംശം)
DEF_CVT_SCALE_FUNC(16f8s,  cvt_32f, hfloat, schar, ദശാംശം)
DEF_CVT_SCALE_FUNC(16bf8s, cvt_32f, bfloat, schar, ദശാംശം)

DEF_CVT_SCALE2BOOL_FUNC(8u8b, uchar, ദശാംശം)
DEF_CVT_SCALE2BOOL_FUNC(8s8b, schar, ദശാംശം)
DEF_CVT_SCALE2BOOL_FUNC(16u8b, ushort, ദശാംശം)
DEF_CVT_SCALE2BOOL_FUNC(16s8b, short, ദശാംശം)
DEF_CVT_SCALE2BOOL_FUNC(32u8b, unsigned, ദശാംശം)
DEF_CVT_SCALE2BOOL_FUNC(32s8b, പൂർണ്ണസംഖ്യ, ദശാംശം)
DEF_CVT_SCALE2BOOL_FUNC(32f8b, ദശാംശം, ദശാംശം)
DEF_CVT_SCALE2BOOL_FUNC(64f8b, ഇരട്ട, ദശാംശം)
DEF_CVT_SCALE2BOOL_FUNC(64u8b, uint64_t, ദശാംശം)
DEF_CVT_SCALE2BOOL_FUNC(64s8b, int64_t, ദശാംശം)
DEF_CVT_SCALE2BOOL_FUNC(16f8b, hfloat, ദശാംശം)
DEF_CVT_SCALE2BOOL_FUNC(16bf8b, bfloat, ദശാംശം)

DEF_CVT_SCALE_FUNC(8u16u,  cvt_32f, uchar,  ushort, ദശാംശം)
DEF_CVT_SCALE_FUNC(8s16u,  cvt_32f, schar,  ushort, ദശാംശം)
DEF_CVT_SCALE_FUNC(16u,    cvt_32f, ushort, ushort, ദശാംശം)
DEF_CVT_SCALE_FUNC(16s16u, cvt_32f, short,  ushort, ദശാംശം)
DEF_CVT_SCALE_FUNC(32u16u, cvt_32f, unsigned, ushort, ദശാംശം)
DEF_CVT_SCALE_FUNC(32s16u, cvt_32f, പൂർണ്ണസംഖ്യ,    ushort, ദശാംശം)
DEF_CVT_SCALE_FUNC(32f16u, cvt_32f, ദശാംശം,  ushort, ദശാംശം)
DEF_CVT_SCALE_FUNC(64f16u, cvt_32f, ഇരട്ട, ushort, ദശാംശം)
DEF_CVT_SCALE_FUNC(64u16u, cvt_32f, uint64_t, ushort, ദശാംശം)
DEF_CVT_SCALE_FUNC(64s16u, cvt_32f, int64_t, ushort, ദശാംശം)
DEF_CVT_SCALE_FUNC(16f16u, cvt1_32f, hfloat, ushort, ദശാംശം)
DEF_CVT_SCALE_FUNC(16bf16u, cvt1_32f, bfloat, ushort, ദശാംശം)

DEF_CVT_SCALE_FUNC(8u16s,  cvt_32f, uchar,  short, ദശാംശം)
DEF_CVT_SCALE_FUNC(8s16s,  cvt_32f, schar,  short, ദശാംശം)
DEF_CVT_SCALE_FUNC(16u16s, cvt_32f, ushort, short, ദശാംശം)
DEF_CVT_SCALE_FUNC(16s,    cvt_32f, short,  short, ദശാംശം)
DEF_CVT_SCALE_FUNC(32u16s, cvt_32f, unsigned, short, ദശാംശം)
DEF_CVT_SCALE_FUNC(32s16s, cvt_32f, പൂർണ്ണസംഖ്യ,    short, ദശാംശം)
DEF_CVT_SCALE_FUNC(32f16s, cvt_32f, ദശാംശം,  short, ദശാംശം)
DEF_CVT_SCALE_FUNC(64f16s, cvt_32f, ഇരട്ട, short, ദശാംശം)
DEF_CVT_SCALE_FUNC(64u16s, cvt_32f, uint64_t, short, ദശാംശം)
DEF_CVT_SCALE_FUNC(64s16s, cvt_32f, int64_t, short, ദശാംശം)
DEF_CVT_SCALE_FUNC(16f16s, cvt1_32f, hfloat, short, ദശാംശം)
DEF_CVT_SCALE_FUNC(16bf16s, cvt1_32f, bfloat, short, ദശാംശം)

DEF_CVT_SCALE_FUNC(8u32u,  cvt_32f, uchar,  unsigned, ദശാംശം)
DEF_CVT_SCALE_FUNC(8s32u,  cvt_32f, schar,  unsigned, ദശാംശം)
DEF_CVT_SCALE_FUNC(16u32u, cvt_32f, ushort, unsigned, ദശാംശം)
DEF_CVT_SCALE_FUNC(16s32u, cvt_32f, short,  unsigned, ദശാംശം)
DEF_CVT_SCALE_FUNC(32u, cvt_32f, unsigned, unsigned, ദശാംശം)
DEF_CVT_SCALE_FUNC(32s32u, cvt_64f, പൂർണ്ണസംഖ്യ,    unsigned, ഇരട്ട)
DEF_CVT_SCALE_FUNC(32f32u, cvt_32f, ദശാംശം,  unsigned, ദശാംശം)
DEF_CVT_SCALE_FUNC(64f32u, cvt_64f, ഇരട്ട, unsigned, ഇരട്ട)
DEF_CVT_SCALE_FUNC(64u32u, cvt_32f, uint64_t, unsigned, ദശാംശം)
DEF_CVT_SCALE_FUNC(64s32u, cvt_32f, int64_t, unsigned, ദശാംശം)
DEF_CVT_SCALE_FUNC(16f32u, cvt1_32f, hfloat, unsigned, ദശാംശം)
DEF_CVT_SCALE_FUNC(16bf32u, cvt1_32f, bfloat, unsigned, ദശാംശം)

DEF_CVT_SCALE_FUNC(8u32s,  cvt_32f, uchar,  പൂർണ്ണസംഖ്യ, ദശാംശം)
DEF_CVT_SCALE_FUNC(8s32s,  cvt_32f, schar,  പൂർണ്ണസംഖ്യ, ദശാംശം)
DEF_CVT_SCALE_FUNC(16u32s, cvt_32f, ushort, പൂർണ്ണസംഖ്യ, ദശാംശം)
DEF_CVT_SCALE_FUNC(16s32s, cvt_32f, short,  പൂർണ്ണസംഖ്യ, ദശാംശം)
DEF_CVT_SCALE_FUNC(32u32s, cvt_32f, unsigned, പൂർണ്ണസംഖ്യ, ദശാംശം)
DEF_CVT_SCALE_FUNC(32s,    cvt_64f, പൂർണ്ണസംഖ്യ,    പൂർണ്ണസംഖ്യ, ഇരട്ട)
DEF_CVT_SCALE_FUNC(32f32s, cvt_32f, ദശാംശം,  പൂർണ്ണസംഖ്യ, ദശാംശം)
DEF_CVT_SCALE_FUNC(64f32s, cvt_64f, ഇരട്ട, പൂർണ്ണസംഖ്യ, ഇരട്ട)
DEF_CVT_SCALE_FUNC(64u32s, cvt_32f, uint64_t, പൂർണ്ണസംഖ്യ, ദശാംശം)
DEF_CVT_SCALE_FUNC(64s32s, cvt_32f, int64_t, പൂർണ്ണസംഖ്യ, ദശാംശം)
DEF_CVT_SCALE_FUNC(16f32s, cvt1_32f, hfloat, പൂർണ്ണസംഖ്യ, ദശാംശം)
DEF_CVT_SCALE_FUNC(16bf32s, cvt1_32f, bfloat, പൂർണ്ണസംഖ്യ, ദശാംശം)

DEF_CVT_SCALE_FUNC(8u32f,  cvt_32f, uchar,  ദശാംശം, ദശാംശം)
DEF_CVT_SCALE_FUNC(8s32f,  cvt_32f, schar,  ദശാംശം, ദശാംശം)
DEF_CVT_SCALE_FUNC(16u32f, cvt_32f, ushort, ദശാംശം, ദശാംശം)
DEF_CVT_SCALE_FUNC(16s32f, cvt_32f, short,  ദശാംശം, ദശാംശം)
DEF_CVT_SCALE_FUNC(32u32f, cvt_32f, unsigned, ദശാംശം, ദശാംശം)
DEF_CVT_SCALE_FUNC(32s32f, cvt_32f, പൂർണ്ണസംഖ്യ,    ദശാംശം, ദശാംശം)
DEF_CVT_SCALE_FUNC(32f,    cvt_32f, ദശാംശം,  ദശാംശം, ദശാംശം)
DEF_CVT_SCALE_FUNC(64f32f, cvt_64f, ഇരട്ട, ദശാംശം, ഇരട്ട)
DEF_CVT_SCALE_FUNC(64u32f, cvt_32f, uint64_t, ദശാംശം, ദശാംശം)
DEF_CVT_SCALE_FUNC(64s32f, cvt_32f, int64_t, ദശാംശം, ദശാംശം)
DEF_CVT_SCALE_FUNC(16f32f, cvt1_32f, hfloat, ദശാംശം, ദശാംശം)
DEF_CVT_SCALE_FUNC(16bf32f, cvt1_32f, bfloat, ദശാംശം, ദശാംശം)

DEF_CVT_SCALE_FUNC(8u64f,  cvt_64f, uchar,  ഇരട്ട, ഇരട്ട)
DEF_CVT_SCALE_FUNC(8s64f,  cvt_64f, schar,  ഇരട്ട, ഇരട്ട)
DEF_CVT_SCALE_FUNC(16u64f, cvt_64f, ushort, ഇരട്ട, ഇരട്ട)
DEF_CVT_SCALE_FUNC(16s64f, cvt_64f, short,  ഇരട്ട, ഇരട്ട)
DEF_CVT_SCALE_FUNC(32u64f, cvt_64f, unsigned, ഇരട്ട, ഇരട്ട)
DEF_CVT_SCALE_FUNC(32s64f, cvt_64f, പൂർണ്ണസംഖ്യ,    ഇരട്ട, ഇരട്ട)
DEF_CVT_SCALE_FUNC(32f64f, cvt_64f, ദശാംശം,  ഇരട്ട, ഇരട്ട)
DEF_CVT_SCALE_FUNC(64f,    cvt_64f, ഇരട്ട, ഇരട്ട, ഇരട്ട)
DEF_CVT_SCALE_FUNC(64u64f, cvt_64f, uint64_t, ഇരട്ട, ഇരട്ട)
DEF_CVT_SCALE_FUNC(64s64f, cvt_64f, int64_t, ഇരട്ട, ഇരട്ട)
DEF_CVT_SCALE_FUNC(16f64f, cvt_64f, hfloat, ഇരട്ട, ഇരട്ട)
DEF_CVT_SCALE_FUNC(16bf64f, cvt_64f, bfloat, ഇരട്ട, ഇരട്ട)

DEF_CVT_SCALE_FUNC(8u64u,  cvt_64f, uchar,  uint64_t, ഇരട്ട)
DEF_CVT_SCALE_FUNC(8s64u,  cvt_64f, schar,  uint64_t, ഇരട്ട)
DEF_CVT_SCALE_FUNC(16u64u, cvt_64f, ushort, uint64_t, ഇരട്ട)
DEF_CVT_SCALE_FUNC(16s64u, cvt_64f, short,  uint64_t, ഇരട്ട)
DEF_CVT_SCALE_FUNC(32u64u, cvt_64f, unsigned, uint64_t, ഇരട്ട)
DEF_CVT_SCALE_FUNC(32s64u, cvt_64f, പൂർണ്ണസംഖ്യ,    uint64_t, ഇരട്ട)
DEF_CVT_SCALE_FUNC(32f64u, cvt_64f, ദശാംശം,  uint64_t, ഇരട്ട)
DEF_CVT_SCALE_FUNC(64f64u, cvt_64f, ഇരട്ട, uint64_t, ഇരട്ട)
DEF_CVT_SCALE_FUNC(64u, cvt_64f, uint64_t, uint64_t, ഇരട്ട)
DEF_CVT_SCALE_FUNC(64s64u, cvt_64f, int64_t, uint64_t, ഇരട്ട)
DEF_CVT_SCALE_FUNC(16f64u, cvt_64f, hfloat, uint64_t, ഇരട്ട)
DEF_CVT_SCALE_FUNC(16bf64u, cvt_64f, bfloat, uint64_t, ഇരട്ട)

DEF_CVT_SCALE_FUNC(8u64s,  cvt_64f, uchar,  int64_t, ഇരട്ട)
DEF_CVT_SCALE_FUNC(8s64s,  cvt_64f, schar,  int64_t, ഇരട്ട)
DEF_CVT_SCALE_FUNC(16u64s, cvt_64f, ushort, int64_t, ഇരട്ട)
DEF_CVT_SCALE_FUNC(16s64s, cvt_64f, short,  int64_t, ഇരട്ട)
DEF_CVT_SCALE_FUNC(32u64s, cvt_64f, unsigned, int64_t, ഇരട്ട)
DEF_CVT_SCALE_FUNC(32s64s, cvt_64f, പൂർണ്ണസംഖ്യ,    int64_t, ഇരട്ട)
DEF_CVT_SCALE_FUNC(32f64s, cvt_64f, ദശാംശം,  int64_t, ഇരട്ട)
DEF_CVT_SCALE_FUNC(64f64s, cvt_64f, ഇരട്ട, int64_t, ഇരട്ട)
DEF_CVT_SCALE_FUNC(64u64s, cvt_64f, uint64_t, int64_t, ഇരട്ട)
DEF_CVT_SCALE_FUNC(64s, cvt_64f, int64_t, int64_t, ഇരട്ട)
DEF_CVT_SCALE_FUNC(16f64s, cvt_64f, hfloat, int64_t, ഇരട്ട)
DEF_CVT_SCALE_FUNC(16bf64s, cvt_64f, bfloat, int64_t, ഇരട്ട)

DEF_CVT_SCALE_FUNC(8u16f,  cvt1_32f, uchar,  hfloat, ദശാംശം)
DEF_CVT_SCALE_FUNC(8s16f,  cvt1_32f, schar,  hfloat, ദശാംശം)
DEF_CVT_SCALE_FUNC(16u16f, cvt1_32f, ushort, hfloat, ദശാംശം)
DEF_CVT_SCALE_FUNC(16s16f, cvt1_32f, short,  hfloat, ദശാംശം)
DEF_CVT_SCALE_FUNC(32u16f, cvt1_32f, unsigned, hfloat, ദശാംശം)
DEF_CVT_SCALE_FUNC(32s16f, cvt1_32f, പൂർണ്ണസംഖ്യ,    hfloat, ദശാംശം)
DEF_CVT_SCALE_FUNC(32f16f, cvt1_32f, ദശാംശം,  hfloat, ദശാംശം)
DEF_CVT_SCALE_FUNC(64f16f, cvt1_32f, ഇരട്ട, hfloat, ദശാംശം)
DEF_CVT_SCALE_FUNC(64u16f, cvt1_32f, uint64_t, hfloat, ദശാംശം)
DEF_CVT_SCALE_FUNC(64s16f, cvt1_32f, int64_t, hfloat, ദശാംശം)
DEF_CVT_SCALE_FUNC(16f,    cvt1_32f, hfloat, hfloat, ദശാംശം)
DEF_CVT_SCALE_FUNC(16bf16f, cvt1_32f, bfloat, hfloat, ദശാംശം)

DEF_CVT_SCALE_FUNC(8u16bf,  cvt1_32f, uchar,  bfloat, ദശാംശം)
DEF_CVT_SCALE_FUNC(8s16bf,  cvt1_32f, schar,  bfloat, ദശാംശം)
DEF_CVT_SCALE_FUNC(16u16bf, cvt1_32f, ushort, bfloat, ദശാംശം)
DEF_CVT_SCALE_FUNC(16s16bf, cvt1_32f, short,  bfloat, ദശാംശം)
DEF_CVT_SCALE_FUNC(32u16bf, cvt1_32f, unsigned, bfloat, ദശാംശം)
DEF_CVT_SCALE_FUNC(32s16bf, cvt1_32f, പൂർണ്ണസംഖ്യ,    bfloat, ദശാംശം)
DEF_CVT_SCALE_FUNC(32f16bf, cvt1_32f, ദശാംശം,  bfloat, ദശാംശം)
DEF_CVT_SCALE_FUNC(64f16bf, cvt1_32f, ഇരട്ട, bfloat, ദശാംശം)
DEF_CVT_SCALE_FUNC(64u16bf, cvt1_32f, uint64_t, bfloat, ദശാംശം)
DEF_CVT_SCALE_FUNC(64s16bf, cvt1_32f, int64_t, bfloat, ദശാംശം)
DEF_CVT_SCALE_FUNC(16f16bf, cvt1_32f, hfloat, bfloat, ദശാംശം)
DEF_CVT_SCALE_FUNC(16bf, cvt1_32f, bfloat, bfloat, ദശാംശം)

DEF_CVT_SCALEBOOL2_FUNC(8b8u, uchar, ദശാംശം)
DEF_CVT_SCALEBOOL2_FUNC(8b8s, schar, ദശാംശം)
DEF_CVT_SCALEBOOL2_FUNC(8b, തർക്കം, ദശാംശം)
DEF_CVT_SCALEBOOL2_FUNC(8b16u, ushort, ദശാംശം)
DEF_CVT_SCALEBOOL2_FUNC(8b16s, short, ദശാംശം)
DEF_CVT_SCALEBOOL2_FUNC(8b32u, unsigned, ദശാംശം)
DEF_CVT_SCALEBOOL2_FUNC(8b32s, പൂർണ്ണസംഖ്യ, ദശാംശം)
DEF_CVT_SCALEBOOL2_FUNC(8b32f, ദശാംശം, ദശാംശം)
DEF_CVT_SCALEBOOL2_FUNC(8b64u, uint64_t, ഇരട്ട)
DEF_CVT_SCALEBOOL2_FUNC(8b64s, int64_t, ഇരട്ട)
DEF_CVT_SCALEBOOL2_FUNC(8b64f, ഇരട്ട, ഇരട്ട)
DEF_CVT_SCALEBOOL2_FUNC(8b16f, hfloat, ദശാംശം)
DEF_CVT_SCALEBOOL2_FUNC(8b16bf, bfloat, ദശാംശം)

// FP8 src -> CV_8U abs|scale|shift (scalar)
#define DEF_CVT_SCALE_ABS_SCALAR_FUNC(suffix, stype) \
സ്ഥിതം ശൂന്യം cvtScaleAbs##suffix( സ്ഥിരം uchar* src_, size_t sstep, സ്ഥിരം uchar*, size_t, \
                                 uchar* dst, size_t dstep, Size size, ശൂന്യം* scale_) \
{ \
    സ്ഥിരം stype* src = (സ്ഥിരം stype*)src_; \
    സ്ഥിരം ഇരട്ട* scale = (സ്ഥിരം ഇരട്ട*)scale_; \
    ദശാംശം a = (ദശാംശം)scale[0], b = (ദശാംശം)scale[1]; \
    sstep /= വലുപ്പം(src[0]); \
    വേണ്ടി( പൂർണ്ണസംഖ്യ i = 0; i < size.height; i++, src += sstep, dst += dstep ) \
        വേണ്ടി (പൂർണ്ണസംഖ്യ j = 0; j < size.width; j++) \
            dst[j] = saturate_cast<uchar>(std::abs((ദശാംശം)src[j]*a + b)); \
}
DEF_CVT_SCALE_ABS_SCALAR_FUNC(8fe4m3u8u, fp8a_t)
DEF_CVT_SCALE_ABS_SCALAR_FUNC(8fe4m38u,  fp8_t)

ദ്വിമാന_Func getCvtScaleAbsFunc(പൂർണ്ണസംഖ്യ depth)
{
    ദ്വിമാന_Func func =
        depth == CV_8F_E4M3FN ? (ദ്വിമാന_Func)cvtScaleAbs8fe4m38u :
        depth == CV_8F_E4M3FNUZ ? (ദ്വിമാന_Func)cvtScaleAbs8fe4m3u8u :
        depth == CV_8U ? (ദ്വിമാന_Func)cvtScaleAbs8u :
        depth == CV_8S ? (ദ്വിമാന_Func)cvtScaleAbs8s8u :
        depth == CV_Bool ? (ദ്വിമാന_Func)cvtScaleAbs8b8u :
        depth == CV_16U ? (ദ്വിമാന_Func)cvtScaleAbs16u8u :
        depth == CV_16S ? (ദ്വിമാന_Func)cvtScaleAbs16s8u :
        depth == CV_16F ? (ദ്വിമാന_Func)cvtScaleAbs16f8u :
        depth == CV_16BF ? (ദ്വിമാന_Func)cvtScaleAbs16bf8u :
        depth == CV_32U ? (ദ്വിമാന_Func)cvtScaleAbs32u8u :
        depth == CV_32S ? (ദ്വിമാന_Func)cvtScaleAbs32s8u :
        depth == CV_32F ? (ദ്വിമാന_Func)cvtScaleAbs32f8u :
        depth == CV_64U ? (ദ്വിമാന_Func)cvtScaleAbs64u8u :
        depth == CV_64S ? (ദ്വിമാന_Func)cvtScaleAbs64s8u :
        depth == CV_64F ? (ദ്വിമാന_Func)cvtScaleAbs64f8u : 0;
    CV_Assert(func != 0);
    തിരികെ func;
}

//////////////////// FP8 scaled conversions — scalar via saturate_cast(src*a+b) ////////////////////
#define DEF_CVT_SCALE_SCALAR_FUNC(suffix, stype, dtype, wtype) \
സ്ഥിതം ശൂന്യം cvtScale##suffix( സ്ഥിരം uchar* src_, size_t sstep, സ്ഥിരം uchar*, size_t, \
                              uchar* dst_, size_t dstep, Size size, ശൂന്യം* scale_) \
{ \
    സ്ഥിരം stype* src = (സ്ഥിരം stype*)src_; dtype* dst = (dtype*)dst_; \
    സ്ഥിരം ഇരട്ട* scale = (സ്ഥിരം ഇരട്ട*)scale_; \
    wtype a = (wtype)scale[0], b = (wtype)scale[1]; \
    sstep /= വലുപ്പം(src[0]); dstep /= വലുപ്പം(dst[0]); \
    വേണ്ടി( പൂർണ്ണസംഖ്യ i = 0; i < size.height; i++, src += sstep, dst += dstep ) \
        വേണ്ടി (പൂർണ്ണസംഖ്യ j = 0; j < size.width; j++) \
            dst[j] = saturate_cast<dtype>((wtype)src[j]*a + b); \
}

// double intermediate so coarse FP8 grids round identically to the (double-precision) reference
#define DEF_CVT_SCALE_FP8(S, T) \
    DEF_CVT_SCALE_SCALAR_FUNC(S##8u,   T, uchar, ഇരട്ട)     DEF_CVT_SCALE_SCALAR_FUNC(8u##S,   uchar, T, ഇരട്ട) \
    DEF_CVT_SCALE_SCALAR_FUNC(S##8s,   T, schar, ഇരട്ട)     DEF_CVT_SCALE_SCALAR_FUNC(8s##S,   schar, T, ഇരട്ട) \
    DEF_CVT_SCALE_SCALAR_FUNC(S##16u,  T, ushort, ഇരട്ട)    DEF_CVT_SCALE_SCALAR_FUNC(16u##S,  ushort, T, ഇരട്ട) \
    DEF_CVT_SCALE_SCALAR_FUNC(S##16s,  T, short, ഇരട്ട)     DEF_CVT_SCALE_SCALAR_FUNC(16s##S,  short, T, ഇരട്ട) \
    DEF_CVT_SCALE_SCALAR_FUNC(S##32u,  T, unsigned, ഇരട്ട)  DEF_CVT_SCALE_SCALAR_FUNC(32u##S,  unsigned, T, ഇരട്ട) \
    DEF_CVT_SCALE_SCALAR_FUNC(S##32s,  T, പൂർണ്ണസംഖ്യ, ഇരട്ട)       DEF_CVT_SCALE_SCALAR_FUNC(32s##S,  പൂർണ്ണസംഖ്യ, T, ഇരട്ട) \
    DEF_CVT_SCALE_SCALAR_FUNC(S##32f,  T, ദശാംശം, ഇരട്ട) \
    DEF_CVT_SCALE_SCALAR_FUNC(S##64f,  T, ഇരട്ട, ഇരട്ട)    DEF_CVT_SCALE_SCALAR_FUNC(64f##S,  ഇരട്ട, T, ഇരട്ട) \
    DEF_CVT_SCALE_SCALAR_FUNC(S##16f,  T, hfloat, ഇരട്ട)    DEF_CVT_SCALE_SCALAR_FUNC(16f##S,  hfloat, T, ഇരട്ട) \
    DEF_CVT_SCALE_SCALAR_FUNC(S##16bf, T, bfloat, ഇരട്ട)    DEF_CVT_SCALE_SCALAR_FUNC(16bf##S, bfloat, T, ഇരട്ട) \
    DEF_CVT_SCALE_SCALAR_FUNC(S##64u,  T, uint64_t, ഇരട്ട)  DEF_CVT_SCALE_SCALAR_FUNC(64u##S,  uint64_t, T, ഇരട്ട) \
    DEF_CVT_SCALE_SCALAR_FUNC(S##64s,  T, int64_t, ഇരട്ട)   DEF_CVT_SCALE_SCALAR_FUNC(64s##S,  int64_t, T, ഇരട്ട) \
    DEF_CVT_SCALE_SCALAR_FUNC(S##8b,   T, തർക്കം, ഇരട്ട) \
    DEF_CVT_SCALE_SCALAR_FUNC(S##8fe4m3,  T, fp8_t, ഇരട്ട)   DEF_CVT_SCALE_SCALAR_FUNC(S##8fe4m3u, T, fp8a_t, ഇരട്ട)

DEF_CVT_SCALE_FP8(8fe4m3,  fp8_t)
DEF_CVT_SCALE_FP8(8fe4m3u, fp8a_t)

// convertTo(dst, depth, alpha, beta) path -- real quantization. Reuses convert.hpp's fast path.
// Double precision here matches the scalar reference (wtype)x*a+b bit-for-bit.
template<പൂർണ്ണസംഖ്യ bias, തർക്കം fnuz> സ്ഥിതം ശൂന്യം
cvtScaleF32ToFp8(സ്ഥിരം uchar* src_, size_t sstep, സ്ഥിരം uchar*, size_t,
                 uchar* dst, size_t dstep, Size size, ശൂന്യം* scale_)
{
    CV_INSTRUMENT_REGION();
    സ്ഥിരം ദശാംശം* src = (സ്ഥിരം ദശാംശം*)src_;
    സ്ഥിരം ഇരട്ട* scale = (സ്ഥിരം ഇരട്ട*)scale_;
    ഇരട്ട a = scale[0], b = scale[1];
    sstep /= വലുപ്പം(src[0]);

    വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < size.height; i++, src += sstep, dst += dstep)
    {
        പൂർണ്ണസംഖ്യ j = 0;
#എങ്കിൽ (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)
        v_float64 va = vx_setall_f64(a), vb = vx_setall_f64(b);
        സ്ഥിരം പൂർണ്ണസംഖ്യ LANES32 = VTraits<v_float32>::vlanes();
        സ്ഥിരം പൂർണ്ണസംഖ്യ VECSZ = LANES32 * 4;
        ദശാംശം scaled[VTraits<v_float32>::max_nlanes * 4];
        വേണ്ടി (; j <= size.width - VECSZ; j += VECSZ)
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
        വേണ്ടി (; j < size.width; j++)
            dst[j] = fp8_detail::encodeE4M3((ദശാംശം)((ഇരട്ട)src[j]*a + b), bias, fnuz);
    }
}
സ്ഥിതം ശൂന്യം cvtScale32f8fe4m3(സ്ഥിരം uchar* s, size_t ss, സ്ഥിരം uchar* p, size_t ps, uchar* d, size_t ds, Size sz, ശൂന്യം* x)
{ cvtScaleF32ToFp8<7, അസത്യം>(s, ss, p, ps, d, ds, sz, x); }
സ്ഥിതം ശൂന്യം cvtScale32f8fe4m3u(സ്ഥിരം uchar* s, size_t ss, സ്ഥിരം uchar* p, size_t ps, uchar* d, size_t ds, Size sz, ശൂന്യം* x)
{ cvtScaleF32ToFp8<8, സത്യം>(s, ss, p, ps, d, ds, sz, x); }

ദ്വിമാന_Func getConvertScaleFunc(പൂർണ്ണസംഖ്യ sdepth_, പൂർണ്ണസംഖ്യ ddepth_)
{
    പൂർണ്ണസംഖ്യ sdepth = CV_MAT_DEPTH(sdepth_);
    പൂർണ്ണസംഖ്യ ddepth = CV_MAT_DEPTH(ddepth_);
    ദ്വിമാന_Func func =
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
    തിരികെ func;
}

#endif

CV_CPU_OPTIMIZATION_NAMESPACE_END
} // namespace
