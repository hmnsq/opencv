// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


#अन्तर्भाव "precomp.hpp"
#अन्तर्भाव "convert.hpp"

नामाकाश cv {
CV_CPU_OPTIMIZATION_NAMESPACE_BEGIN

द्विआधारी_Func getCvtScaleAbsFunc(पूर्णाङ्क depth);
द्विआधारी_Func getConvertScaleFunc(पूर्णाङ्क sdepth, पूर्णाङ्क ddepth);

#ifndef CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

/****************************************************************************************\
*                                convertScale[Abs]                                       *
\****************************************************************************************/

template<typename _Ts, typename _Td> अन्तःस्थ शून्य
cvtabs_32f( ध्रुव _Ts* src, size_t sstep, _Td* dst, size_t dstep,
            Size size, भिन्न a, भिन्न b )
{
#यदि (CV_SIMD || CV_SIMD_SCALABLE)
    v_float32 va = vx_setall_f32(a), vb = vx_setall_f32(b);
    ध्रुव पूर्णाङ्क VECSZ = VTraits<v_float32>::vlanes()*2;
    // GCC miscompiles this scalable block only on VLEN=128 RVV (opencv/issues/26936);
    // use scalar there, keep SIMD on wider vectors. Ask the hardware for VLEN directly:
    // vlanes() would depend on the LMUL the universal intrinsics happen to use.
#यदि (CV_RVV && CV_SIMD_SCALABLE && defined(__GNUC__) && !defined(__clang__))
    ध्रुव तार्किक useSIMD = __riscv_vlenb()*8 != 128;
#अन्यथा
    ध्रुव तार्किक useSIMD = सत्य;
#endif
#endif
    sstep /= परिमाण(src[0]);
    dstep /= परिमाण(dst[0]);

    प्रति( पूर्णाङ्क i = 0; i < size.height; i++, src += sstep, dst += dstep )
    {
        पूर्णाङ्क j = 0;
#यदि (CV_SIMD || CV_SIMD_SCALABLE)
        यदि( useSIMD )
        प्रति( ; j < size.width; j += VECSZ )
        {
            यदि( j > size.width - VECSZ )
            {
                यदि( j == 0 || src == (_Ts*)dst )
                    भङ्ग;
                j = size.width - VECSZ;
            }
            v_float32 v0, v1;
            vx_load_pair_as(src + j, v0, v1);
            v0 = v_fma(v0, va, vb);
            v1 = v_fma(v1, va, vb);
            v_store_pair_as(dst + j, v_abs(v0), v_abs(v1));
        }
#endif
        प्रति( ; j < size.width; j++ )
            dst[j] = saturate_cast<_Td>(std::abs(src[j]*a + b));
    }
}

स्थिर शून्य
cvtabs_32f( ध्रुव तार्किक* src_, size_t sstep,
            uchar* dst, size_t dstep,
            Size size, भिन्न a, भिन्न b )
{
    ध्रुव uchar* src = (ध्रुव uchar*)src_;
    uchar v0 = saturate_cast<uchar>(std::abs(b));
    uchar v1 = saturate_cast<uchar>(std::abs(a + b));
    प्रति( पूर्णाङ्क i = 0; i < size.height; i++, src += sstep, dst += dstep )
    {
        प्रति (पूर्णाङ्क j = 0; j < size.width; j++)
            dst[j] = src[j] != 0 ? v1 : v0;
    }
}

template<typename _Ts, typename _Td> अन्तःस्थ शून्य
cvt_32f( ध्रुव _Ts* src, size_t sstep, _Td* dst, size_t dstep,
         Size size, भिन्न a, भिन्न b )
{
#यदि (CV_SIMD || (CV_SIMD_SCALABLE && !(defined(__GNUC__) && !defined(__clang__))) )
    v_float32 va = vx_setall_f32(a), vb = vx_setall_f32(b);
    ध्रुव पूर्णाङ्क VECSZ = VTraits<v_float32>::vlanes()*2;
#endif
    sstep /= परिमाण(src[0]);
    dstep /= परिमाण(dst[0]);

    प्रति( पूर्णाङ्क i = 0; i < size.height; i++, src += sstep, dst += dstep )
    {
        पूर्णाङ्क j = 0;
// Excluding GNU in CV_SIMD_SCALABLE because of "opencv/issues/26936"
#यदि (CV_SIMD || (CV_SIMD_SCALABLE && !(defined(__GNUC__) && !defined(__clang__))) )
        प्रति( ; j < size.width; j += VECSZ )
        {
            यदि( j > size.width - VECSZ )
            {
                यदि( j == 0 || src == (_Ts*)dst )
                    भङ्ग;
                j = size.width - VECSZ;
            }
            v_float32 v0, v1;
            vx_load_pair_as(src + j, v0, v1);
            v0 = v_fma(v0, va, vb);
            v1 = v_fma(v1, va, vb);
            v_store_pair_as(dst + j, v0, v1);
        }
#endif
        प्रति( ; j < size.width; j++ )
            dst[j] = saturate_cast<_Td>(src[j]*a + b);
    }
}

// variant for conversions 16f <-> ... w/o unrolling
template<typename _Ts, typename _Td> अन्तःस्थ शून्य
cvt1_32f( ध्रुव _Ts* src, size_t sstep, _Td* dst, size_t dstep,
          Size size, भिन्न a, भिन्न b )
{
#यदि (CV_SIMD || CV_SIMD_SCALABLE)
    v_float32 va = vx_setall_f32(a), vb = vx_setall_f32(b);
    ध्रुव पूर्णाङ्क VECSZ = VTraits<v_float32>::vlanes();
#endif
    sstep /= परिमाण(src[0]);
    dstep /= परिमाण(dst[0]);

    प्रति( पूर्णाङ्क i = 0; i < size.height; i++, src += sstep, dst += dstep )
    {
        पूर्णाङ्क j = 0;
#यदि (CV_SIMD || CV_SIMD_SCALABLE)
        प्रति( ; j < size.width; j += VECSZ )
        {
            यदि( j > size.width - VECSZ )
            {
                यदि( j == 0 || src == (_Ts*)dst )
                    भङ्ग;
                j = size.width - VECSZ;
            }
            v_float32 v0;
            vx_load_as(src + j, v0);
            v0 = v_fma(v0, va, vb);
            v_store_as(dst + j, v0);
        }
#endif
        प्रति( ; j < size.width; j++ )
            dst[j] = saturate_cast<_Td>(src[j]*a + b);
    }
}


template<typename _Ts, typename _Td> अन्तःस्थ शून्य
cvt_64f( ध्रुव _Ts* src, size_t sstep, _Td* dst, size_t dstep,
         Size size, द्विगुण a, द्विगुण b )
{
#यदि (CV_SIMD_64F || (CV_SIMD_SCALABLE_64F && !(defined(__GNUC__) && !defined(__clang__))) )
    v_float64 va = vx_setall_f64(a), vb = vx_setall_f64(b);
    ध्रुव पूर्णाङ्क VECSZ = VTraits<v_float64>::vlanes()*2;
#endif
    sstep /= परिमाण(src[0]);
    dstep /= परिमाण(dst[0]);

    प्रति( पूर्णाङ्क i = 0; i < size.height; i++, src += sstep, dst += dstep )
    {
        पूर्णाङ्क j = 0;
// Excluding GNU in CV_SIMD_SCALABLE because of "opencv/issues/26936"
#यदि (CV_SIMD_64F || (CV_SIMD_SCALABLE_64F && !(defined(__GNUC__) && !defined(__clang__))) )
        प्रति( ; j < size.width; j += VECSZ )
        {
            यदि( j > size.width - VECSZ )
            {
                यदि( j == 0 || src == (_Ts*)dst )
                    भङ्ग;
                j = size.width - VECSZ;
            }
            v_float64 v0, v1;
            vx_load_pair_as(src + j, v0, v1);
            v0 = v_fma(v0, va, vb);
            v1 = v_fma(v1, va, vb);
            v_store_pair_as(dst + j, v0, v1);
        }
#endif
        प्रति( ; j < size.width; j++ )
            dst[j] = saturate_cast<_Td>(src[j]*a + b);
    }
}

//==================================================================================================

#define DEF_CVT_SCALE_ABS_FUNC(suffix, cvt, stype, dtype, wtype) \
स्थिर शून्य cvtScaleAbs##suffix( ध्रुव uchar* src_, size_t sstep, ध्रुव uchar*, size_t, \
                                 uchar* dst_, size_t dstep, Size size, शून्य* scale_) \
{ \
    ध्रुव stype* src = (ध्रुव stype*)src_; \
    dtype* dst = (dtype*)dst_; \
    द्विगुण* scale = (द्विगुण*)scale_; \
    cvt(src, sstep, dst, dstep, size, (wtype)scale[0], (wtype)scale[1]); \
}


#define DEF_CVT_SCALE_FUNC(suffix, cvt, stype, dtype, wtype) \
स्थिर शून्य cvtScale##suffix( ध्रुव uchar* src_, size_t sstep, ध्रुव uchar*, size_t, \
                              uchar* dst_, size_t dstep, Size size, शून्य* scale_) \
{ \
    ध्रुव stype* src = (ध्रुव stype*)src_; \
    dtype* dst = (dtype*)dst_; \
    द्विगुण* scale = (द्विगुण*)scale_; \
    cvt(src, sstep, dst, dstep, size, (wtype)scale[0], (wtype)scale[1]); \
}

#define DEF_CVT_SCALE2BOOL_FUNC(suffix, stype, wtype) \
स्थिर शून्य cvtScale##suffix( ध्रुव uchar* src_, size_t sstep, ध्रुव uchar*, size_t, \
                              uchar* dst, size_t dstep, Size size, शून्य* scale_) \
{ \
    ध्रुव stype* src = (ध्रुव stype*)src_; \
    ध्रुव द्विगुण* scale = (ध्रुव द्विगुण*)scale_; \
    wtype a = (wtype)scale[0], b = (wtype)scale[1]; \
    sstep /= परिमाण(src[0]); \
    प्रति( पूर्णाङ्क i = 0; i < size.height; i++, src += sstep, dst += dstep ) \
        प्रति (पूर्णाङ्क j = 0; j < size.width; j++) \
            dst[j] = (तार्किक)((wtype)src[j]*a + b != 0); \
}

#define DEF_CVT_SCALEBOOL2_FUNC(suffix, dtype, wtype) \
स्थिर शून्य cvtScale##suffix( ध्रुव uchar* src, size_t sstep, ध्रुव uchar*, size_t, \
                              uchar* dst_, size_t dstep, Size size, शून्य* scale_) \
{ \
    dtype* dst = (dtype*)dst_; \
    ध्रुव द्विगुण* scale = (ध्रुव द्विगुण*)scale_; \
    wtype a = (wtype)scale[0], b = (wtype)scale[1]; \
    dtype v0 = saturate_cast<dtype>(b), v1 = saturate_cast<dtype>(a + b); \
    dstep /= परिमाण(dst[0]); \
    प्रति( पूर्णाङ्क i = 0; i < size.height; i++, src += sstep, dst += dstep ) \
        प्रति (पूर्णाङ्क j = 0; j < size.width; j++) \
            dst[j] = src[j] != 0 ? v1 : v0; \
}

DEF_CVT_SCALE_ABS_FUNC(8u,    cvtabs_32f, uchar,  uchar, भिन्न)
DEF_CVT_SCALE_ABS_FUNC(8s8u,  cvtabs_32f, schar,  uchar, भिन्न)
DEF_CVT_SCALE_ABS_FUNC(8b8u,  cvtabs_32f, तार्किक,  uchar, भिन्न)
DEF_CVT_SCALE_ABS_FUNC(16u8u, cvtabs_32f, ushort, uchar, भिन्न)
DEF_CVT_SCALE_ABS_FUNC(16s8u, cvtabs_32f, short,  uchar, भिन्न)
DEF_CVT_SCALE_ABS_FUNC(32u8u, cvtabs_32f, unsigned, uchar, भिन्न)
DEF_CVT_SCALE_ABS_FUNC(32s8u, cvtabs_32f, पूर्णाङ्क,    uchar, भिन्न)
DEF_CVT_SCALE_ABS_FUNC(32f8u, cvtabs_32f, भिन्न,  uchar, भिन्न)
DEF_CVT_SCALE_ABS_FUNC(64u8u, cvtabs_32f, uint64_t, uchar, भिन्न)
DEF_CVT_SCALE_ABS_FUNC(64s8u, cvtabs_32f, int64_t, uchar, भिन्न)
DEF_CVT_SCALE_ABS_FUNC(64f8u, cvtabs_32f, द्विगुण, uchar, भिन्न)
DEF_CVT_SCALE_ABS_FUNC(16f8u, cvtabs_32f, hfloat, uchar, भिन्न)
DEF_CVT_SCALE_ABS_FUNC(16bf8u, cvtabs_32f, bfloat, uchar, भिन्न)

DEF_CVT_SCALE_FUNC(8u,     cvt_32f, uchar,  uchar, भिन्न)
DEF_CVT_SCALE_FUNC(8s8u,   cvt_32f, schar,  uchar, भिन्न)
DEF_CVT_SCALE_FUNC(16u8u,  cvt_32f, ushort, uchar, भिन्न)
DEF_CVT_SCALE_FUNC(16s8u,  cvt_32f, short,  uchar, भिन्न)
DEF_CVT_SCALE_FUNC(32u8u,  cvt_32f, unsigned, uchar, भिन्न)
DEF_CVT_SCALE_FUNC(32s8u,  cvt_32f, पूर्णाङ्क,    uchar, भिन्न)
DEF_CVT_SCALE_FUNC(32f8u,  cvt_32f, भिन्न,  uchar, भिन्न)
DEF_CVT_SCALE_FUNC(64f8u,  cvt_32f, द्विगुण, uchar, भिन्न)
DEF_CVT_SCALE_FUNC(64u8u,  cvt_32f, uint64_t, uchar, भिन्न)
DEF_CVT_SCALE_FUNC(64s8u,  cvt_32f, int64_t, uchar, भिन्न)
DEF_CVT_SCALE_FUNC(16f8u,  cvt_32f, hfloat, uchar, भिन्न)
DEF_CVT_SCALE_FUNC(16bf8u, cvt_32f, bfloat, uchar, भिन्न)

DEF_CVT_SCALE_FUNC(8u8s,   cvt_32f, uchar,  schar, भिन्न)
DEF_CVT_SCALE_FUNC(8s,     cvt_32f, schar,  schar, भिन्न)
DEF_CVT_SCALE_FUNC(16u8s,  cvt_32f, ushort, schar, भिन्न)
DEF_CVT_SCALE_FUNC(16s8s,  cvt_32f, short,  schar, भिन्न)
DEF_CVT_SCALE_FUNC(32u8s,  cvt_32f, unsigned, schar, भिन्न)
DEF_CVT_SCALE_FUNC(32s8s,  cvt_32f, पूर्णाङ्क,    schar, भिन्न)
DEF_CVT_SCALE_FUNC(32f8s,  cvt_32f, भिन्न,  schar, भिन्न)
DEF_CVT_SCALE_FUNC(64f8s,  cvt_32f, द्विगुण, schar, भिन्न)
DEF_CVT_SCALE_FUNC(64u8s,  cvt_32f, uint64_t, schar, भिन्न)
DEF_CVT_SCALE_FUNC(64s8s,  cvt_32f, int64_t, schar, भिन्न)
DEF_CVT_SCALE_FUNC(16f8s,  cvt_32f, hfloat, schar, भिन्न)
DEF_CVT_SCALE_FUNC(16bf8s, cvt_32f, bfloat, schar, भिन्न)

DEF_CVT_SCALE2BOOL_FUNC(8u8b, uchar, भिन्न)
DEF_CVT_SCALE2BOOL_FUNC(8s8b, schar, भिन्न)
DEF_CVT_SCALE2BOOL_FUNC(16u8b, ushort, भिन्न)
DEF_CVT_SCALE2BOOL_FUNC(16s8b, short, भिन्न)
DEF_CVT_SCALE2BOOL_FUNC(32u8b, unsigned, भिन्न)
DEF_CVT_SCALE2BOOL_FUNC(32s8b, पूर्णाङ्क, भिन्न)
DEF_CVT_SCALE2BOOL_FUNC(32f8b, भिन्न, भिन्न)
DEF_CVT_SCALE2BOOL_FUNC(64f8b, द्विगुण, भिन्न)
DEF_CVT_SCALE2BOOL_FUNC(64u8b, uint64_t, भिन्न)
DEF_CVT_SCALE2BOOL_FUNC(64s8b, int64_t, भिन्न)
DEF_CVT_SCALE2BOOL_FUNC(16f8b, hfloat, भिन्न)
DEF_CVT_SCALE2BOOL_FUNC(16bf8b, bfloat, भिन्न)

DEF_CVT_SCALE_FUNC(8u16u,  cvt_32f, uchar,  ushort, भिन्न)
DEF_CVT_SCALE_FUNC(8s16u,  cvt_32f, schar,  ushort, भिन्न)
DEF_CVT_SCALE_FUNC(16u,    cvt_32f, ushort, ushort, भिन्न)
DEF_CVT_SCALE_FUNC(16s16u, cvt_32f, short,  ushort, भिन्न)
DEF_CVT_SCALE_FUNC(32u16u, cvt_32f, unsigned, ushort, भिन्न)
DEF_CVT_SCALE_FUNC(32s16u, cvt_32f, पूर्णाङ्क,    ushort, भिन्न)
DEF_CVT_SCALE_FUNC(32f16u, cvt_32f, भिन्न,  ushort, भिन्न)
DEF_CVT_SCALE_FUNC(64f16u, cvt_32f, द्विगुण, ushort, भिन्न)
DEF_CVT_SCALE_FUNC(64u16u, cvt_32f, uint64_t, ushort, भिन्न)
DEF_CVT_SCALE_FUNC(64s16u, cvt_32f, int64_t, ushort, भिन्न)
DEF_CVT_SCALE_FUNC(16f16u, cvt1_32f, hfloat, ushort, भिन्न)
DEF_CVT_SCALE_FUNC(16bf16u, cvt1_32f, bfloat, ushort, भिन्न)

DEF_CVT_SCALE_FUNC(8u16s,  cvt_32f, uchar,  short, भिन्न)
DEF_CVT_SCALE_FUNC(8s16s,  cvt_32f, schar,  short, भिन्न)
DEF_CVT_SCALE_FUNC(16u16s, cvt_32f, ushort, short, भिन्न)
DEF_CVT_SCALE_FUNC(16s,    cvt_32f, short,  short, भिन्न)
DEF_CVT_SCALE_FUNC(32u16s, cvt_32f, unsigned, short, भिन्न)
DEF_CVT_SCALE_FUNC(32s16s, cvt_32f, पूर्णाङ्क,    short, भिन्न)
DEF_CVT_SCALE_FUNC(32f16s, cvt_32f, भिन्न,  short, भिन्न)
DEF_CVT_SCALE_FUNC(64f16s, cvt_32f, द्विगुण, short, भिन्न)
DEF_CVT_SCALE_FUNC(64u16s, cvt_32f, uint64_t, short, भिन्न)
DEF_CVT_SCALE_FUNC(64s16s, cvt_32f, int64_t, short, भिन्न)
DEF_CVT_SCALE_FUNC(16f16s, cvt1_32f, hfloat, short, भिन्न)
DEF_CVT_SCALE_FUNC(16bf16s, cvt1_32f, bfloat, short, भिन्न)

DEF_CVT_SCALE_FUNC(8u32u,  cvt_32f, uchar,  unsigned, भिन्न)
DEF_CVT_SCALE_FUNC(8s32u,  cvt_32f, schar,  unsigned, भिन्न)
DEF_CVT_SCALE_FUNC(16u32u, cvt_32f, ushort, unsigned, भिन्न)
DEF_CVT_SCALE_FUNC(16s32u, cvt_32f, short,  unsigned, भिन्न)
DEF_CVT_SCALE_FUNC(32u, cvt_32f, unsigned, unsigned, भिन्न)
DEF_CVT_SCALE_FUNC(32s32u, cvt_64f, पूर्णाङ्क,    unsigned, द्विगुण)
DEF_CVT_SCALE_FUNC(32f32u, cvt_32f, भिन्न,  unsigned, भिन्न)
DEF_CVT_SCALE_FUNC(64f32u, cvt_64f, द्विगुण, unsigned, द्विगुण)
DEF_CVT_SCALE_FUNC(64u32u, cvt_32f, uint64_t, unsigned, भिन्न)
DEF_CVT_SCALE_FUNC(64s32u, cvt_32f, int64_t, unsigned, भिन्न)
DEF_CVT_SCALE_FUNC(16f32u, cvt1_32f, hfloat, unsigned, भिन्न)
DEF_CVT_SCALE_FUNC(16bf32u, cvt1_32f, bfloat, unsigned, भिन्न)

DEF_CVT_SCALE_FUNC(8u32s,  cvt_32f, uchar,  पूर्णाङ्क, भिन्न)
DEF_CVT_SCALE_FUNC(8s32s,  cvt_32f, schar,  पूर्णाङ्क, भिन्न)
DEF_CVT_SCALE_FUNC(16u32s, cvt_32f, ushort, पूर्णाङ्क, भिन्न)
DEF_CVT_SCALE_FUNC(16s32s, cvt_32f, short,  पूर्णाङ्क, भिन्न)
DEF_CVT_SCALE_FUNC(32u32s, cvt_32f, unsigned, पूर्णाङ्क, भिन्न)
DEF_CVT_SCALE_FUNC(32s,    cvt_64f, पूर्णाङ्क,    पूर्णाङ्क, द्विगुण)
DEF_CVT_SCALE_FUNC(32f32s, cvt_32f, भिन्न,  पूर्णाङ्क, भिन्न)
DEF_CVT_SCALE_FUNC(64f32s, cvt_64f, द्विगुण, पूर्णाङ्क, द्विगुण)
DEF_CVT_SCALE_FUNC(64u32s, cvt_32f, uint64_t, पूर्णाङ्क, भिन्न)
DEF_CVT_SCALE_FUNC(64s32s, cvt_32f, int64_t, पूर्णाङ्क, भिन्न)
DEF_CVT_SCALE_FUNC(16f32s, cvt1_32f, hfloat, पूर्णाङ्क, भिन्न)
DEF_CVT_SCALE_FUNC(16bf32s, cvt1_32f, bfloat, पूर्णाङ्क, भिन्न)

DEF_CVT_SCALE_FUNC(8u32f,  cvt_32f, uchar,  भिन्न, भिन्न)
DEF_CVT_SCALE_FUNC(8s32f,  cvt_32f, schar,  भिन्न, भिन्न)
DEF_CVT_SCALE_FUNC(16u32f, cvt_32f, ushort, भिन्न, भिन्न)
DEF_CVT_SCALE_FUNC(16s32f, cvt_32f, short,  भिन्न, भिन्न)
DEF_CVT_SCALE_FUNC(32u32f, cvt_32f, unsigned, भिन्न, भिन्न)
DEF_CVT_SCALE_FUNC(32s32f, cvt_32f, पूर्णाङ्क,    भिन्न, भिन्न)
DEF_CVT_SCALE_FUNC(32f,    cvt_32f, भिन्न,  भिन्न, भिन्न)
DEF_CVT_SCALE_FUNC(64f32f, cvt_64f, द्विगुण, भिन्न, द्विगुण)
DEF_CVT_SCALE_FUNC(64u32f, cvt_32f, uint64_t, भिन्न, भिन्न)
DEF_CVT_SCALE_FUNC(64s32f, cvt_32f, int64_t, भिन्न, भिन्न)
DEF_CVT_SCALE_FUNC(16f32f, cvt1_32f, hfloat, भिन्न, भिन्न)
DEF_CVT_SCALE_FUNC(16bf32f, cvt1_32f, bfloat, भिन्न, भिन्न)

DEF_CVT_SCALE_FUNC(8u64f,  cvt_64f, uchar,  द्विगुण, द्विगुण)
DEF_CVT_SCALE_FUNC(8s64f,  cvt_64f, schar,  द्विगुण, द्विगुण)
DEF_CVT_SCALE_FUNC(16u64f, cvt_64f, ushort, द्विगुण, द्विगुण)
DEF_CVT_SCALE_FUNC(16s64f, cvt_64f, short,  द्विगुण, द्विगुण)
DEF_CVT_SCALE_FUNC(32u64f, cvt_64f, unsigned, द्विगुण, द्विगुण)
DEF_CVT_SCALE_FUNC(32s64f, cvt_64f, पूर्णाङ्क,    द्विगुण, द्विगुण)
DEF_CVT_SCALE_FUNC(32f64f, cvt_64f, भिन्न,  द्विगुण, द्विगुण)
DEF_CVT_SCALE_FUNC(64f,    cvt_64f, द्विगुण, द्विगुण, द्विगुण)
DEF_CVT_SCALE_FUNC(64u64f, cvt_64f, uint64_t, द्विगुण, द्विगुण)
DEF_CVT_SCALE_FUNC(64s64f, cvt_64f, int64_t, द्विगुण, द्विगुण)
DEF_CVT_SCALE_FUNC(16f64f, cvt_64f, hfloat, द्विगुण, द्विगुण)
DEF_CVT_SCALE_FUNC(16bf64f, cvt_64f, bfloat, द्विगुण, द्विगुण)

DEF_CVT_SCALE_FUNC(8u64u,  cvt_64f, uchar,  uint64_t, द्विगुण)
DEF_CVT_SCALE_FUNC(8s64u,  cvt_64f, schar,  uint64_t, द्विगुण)
DEF_CVT_SCALE_FUNC(16u64u, cvt_64f, ushort, uint64_t, द्विगुण)
DEF_CVT_SCALE_FUNC(16s64u, cvt_64f, short,  uint64_t, द्विगुण)
DEF_CVT_SCALE_FUNC(32u64u, cvt_64f, unsigned, uint64_t, द्विगुण)
DEF_CVT_SCALE_FUNC(32s64u, cvt_64f, पूर्णाङ्क,    uint64_t, द्विगुण)
DEF_CVT_SCALE_FUNC(32f64u, cvt_64f, भिन्न,  uint64_t, द्विगुण)
DEF_CVT_SCALE_FUNC(64f64u, cvt_64f, द्विगुण, uint64_t, द्विगुण)
DEF_CVT_SCALE_FUNC(64u, cvt_64f, uint64_t, uint64_t, द्विगुण)
DEF_CVT_SCALE_FUNC(64s64u, cvt_64f, int64_t, uint64_t, द्विगुण)
DEF_CVT_SCALE_FUNC(16f64u, cvt_64f, hfloat, uint64_t, द्विगुण)
DEF_CVT_SCALE_FUNC(16bf64u, cvt_64f, bfloat, uint64_t, द्विगुण)

DEF_CVT_SCALE_FUNC(8u64s,  cvt_64f, uchar,  int64_t, द्विगुण)
DEF_CVT_SCALE_FUNC(8s64s,  cvt_64f, schar,  int64_t, द्विगुण)
DEF_CVT_SCALE_FUNC(16u64s, cvt_64f, ushort, int64_t, द्विगुण)
DEF_CVT_SCALE_FUNC(16s64s, cvt_64f, short,  int64_t, द्विगुण)
DEF_CVT_SCALE_FUNC(32u64s, cvt_64f, unsigned, int64_t, द्विगुण)
DEF_CVT_SCALE_FUNC(32s64s, cvt_64f, पूर्णाङ्क,    int64_t, द्विगुण)
DEF_CVT_SCALE_FUNC(32f64s, cvt_64f, भिन्न,  int64_t, द्विगुण)
DEF_CVT_SCALE_FUNC(64f64s, cvt_64f, द्विगुण, int64_t, द्विगुण)
DEF_CVT_SCALE_FUNC(64u64s, cvt_64f, uint64_t, int64_t, द्विगुण)
DEF_CVT_SCALE_FUNC(64s, cvt_64f, int64_t, int64_t, द्विगुण)
DEF_CVT_SCALE_FUNC(16f64s, cvt_64f, hfloat, int64_t, द्विगुण)
DEF_CVT_SCALE_FUNC(16bf64s, cvt_64f, bfloat, int64_t, द्विगुण)

DEF_CVT_SCALE_FUNC(8u16f,  cvt1_32f, uchar,  hfloat, भिन्न)
DEF_CVT_SCALE_FUNC(8s16f,  cvt1_32f, schar,  hfloat, भिन्न)
DEF_CVT_SCALE_FUNC(16u16f, cvt1_32f, ushort, hfloat, भिन्न)
DEF_CVT_SCALE_FUNC(16s16f, cvt1_32f, short,  hfloat, भिन्न)
DEF_CVT_SCALE_FUNC(32u16f, cvt1_32f, unsigned, hfloat, भिन्न)
DEF_CVT_SCALE_FUNC(32s16f, cvt1_32f, पूर्णाङ्क,    hfloat, भिन्न)
DEF_CVT_SCALE_FUNC(32f16f, cvt1_32f, भिन्न,  hfloat, भिन्न)
DEF_CVT_SCALE_FUNC(64f16f, cvt1_32f, द्विगुण, hfloat, भिन्न)
DEF_CVT_SCALE_FUNC(64u16f, cvt1_32f, uint64_t, hfloat, भिन्न)
DEF_CVT_SCALE_FUNC(64s16f, cvt1_32f, int64_t, hfloat, भिन्न)
DEF_CVT_SCALE_FUNC(16f,    cvt1_32f, hfloat, hfloat, भिन्न)
DEF_CVT_SCALE_FUNC(16bf16f, cvt1_32f, bfloat, hfloat, भिन्न)

DEF_CVT_SCALE_FUNC(8u16bf,  cvt1_32f, uchar,  bfloat, भिन्न)
DEF_CVT_SCALE_FUNC(8s16bf,  cvt1_32f, schar,  bfloat, भिन्न)
DEF_CVT_SCALE_FUNC(16u16bf, cvt1_32f, ushort, bfloat, भिन्न)
DEF_CVT_SCALE_FUNC(16s16bf, cvt1_32f, short,  bfloat, भिन्न)
DEF_CVT_SCALE_FUNC(32u16bf, cvt1_32f, unsigned, bfloat, भिन्न)
DEF_CVT_SCALE_FUNC(32s16bf, cvt1_32f, पूर्णाङ्क,    bfloat, भिन्न)
DEF_CVT_SCALE_FUNC(32f16bf, cvt1_32f, भिन्न,  bfloat, भिन्न)
DEF_CVT_SCALE_FUNC(64f16bf, cvt1_32f, द्विगुण, bfloat, भिन्न)
DEF_CVT_SCALE_FUNC(64u16bf, cvt1_32f, uint64_t, bfloat, भिन्न)
DEF_CVT_SCALE_FUNC(64s16bf, cvt1_32f, int64_t, bfloat, भिन्न)
DEF_CVT_SCALE_FUNC(16f16bf, cvt1_32f, hfloat, bfloat, भिन्न)
DEF_CVT_SCALE_FUNC(16bf, cvt1_32f, bfloat, bfloat, भिन्न)

DEF_CVT_SCALEBOOL2_FUNC(8b8u, uchar, भिन्न)
DEF_CVT_SCALEBOOL2_FUNC(8b8s, schar, भिन्न)
DEF_CVT_SCALEBOOL2_FUNC(8b, तार्किक, भिन्न)
DEF_CVT_SCALEBOOL2_FUNC(8b16u, ushort, भिन्न)
DEF_CVT_SCALEBOOL2_FUNC(8b16s, short, भिन्न)
DEF_CVT_SCALEBOOL2_FUNC(8b32u, unsigned, भिन्न)
DEF_CVT_SCALEBOOL2_FUNC(8b32s, पूर्णाङ्क, भिन्न)
DEF_CVT_SCALEBOOL2_FUNC(8b32f, भिन्न, भिन्न)
DEF_CVT_SCALEBOOL2_FUNC(8b64u, uint64_t, द्विगुण)
DEF_CVT_SCALEBOOL2_FUNC(8b64s, int64_t, द्विगुण)
DEF_CVT_SCALEBOOL2_FUNC(8b64f, द्विगुण, द्विगुण)
DEF_CVT_SCALEBOOL2_FUNC(8b16f, hfloat, भिन्न)
DEF_CVT_SCALEBOOL2_FUNC(8b16bf, bfloat, भिन्न)

// FP8 src -> CV_8U abs|scale|shift (scalar)
#define DEF_CVT_SCALE_ABS_SCALAR_FUNC(suffix, stype) \
स्थिर शून्य cvtScaleAbs##suffix( ध्रुव uchar* src_, size_t sstep, ध्रुव uchar*, size_t, \
                                 uchar* dst, size_t dstep, Size size, शून्य* scale_) \
{ \
    ध्रुव stype* src = (ध्रुव stype*)src_; \
    ध्रुव द्विगुण* scale = (ध्रुव द्विगुण*)scale_; \
    भिन्न a = (भिन्न)scale[0], b = (भिन्न)scale[1]; \
    sstep /= परिमाण(src[0]); \
    प्रति( पूर्णाङ्क i = 0; i < size.height; i++, src += sstep, dst += dstep ) \
        प्रति (पूर्णाङ्क j = 0; j < size.width; j++) \
            dst[j] = saturate_cast<uchar>(std::abs((भिन्न)src[j]*a + b)); \
}
DEF_CVT_SCALE_ABS_SCALAR_FUNC(8fe4m3u8u, fp8a_t)
DEF_CVT_SCALE_ABS_SCALAR_FUNC(8fe4m38u,  fp8_t)

द्विआधारी_Func getCvtScaleAbsFunc(पूर्णाङ्क depth)
{
    द्विआधारी_Func func =
        depth == CV_8F_E4M3FN ? (द्विआधारी_Func)cvtScaleAbs8fe4m38u :
        depth == CV_8F_E4M3FNUZ ? (द्विआधारी_Func)cvtScaleAbs8fe4m3u8u :
        depth == CV_8U ? (द्विआधारी_Func)cvtScaleAbs8u :
        depth == CV_8S ? (द्विआधारी_Func)cvtScaleAbs8s8u :
        depth == CV_Bool ? (द्विआधारी_Func)cvtScaleAbs8b8u :
        depth == CV_16U ? (द्विआधारी_Func)cvtScaleAbs16u8u :
        depth == CV_16S ? (द्विआधारी_Func)cvtScaleAbs16s8u :
        depth == CV_16F ? (द्विआधारी_Func)cvtScaleAbs16f8u :
        depth == CV_16BF ? (द्विआधारी_Func)cvtScaleAbs16bf8u :
        depth == CV_32U ? (द्विआधारी_Func)cvtScaleAbs32u8u :
        depth == CV_32S ? (द्विआधारी_Func)cvtScaleAbs32s8u :
        depth == CV_32F ? (द्विआधारी_Func)cvtScaleAbs32f8u :
        depth == CV_64U ? (द्विआधारी_Func)cvtScaleAbs64u8u :
        depth == CV_64S ? (द्विआधारी_Func)cvtScaleAbs64s8u :
        depth == CV_64F ? (द्विआधारी_Func)cvtScaleAbs64f8u : 0;
    CV_Assert(func != 0);
    प्रत्यागम func;
}

//////////////////// FP8 scaled conversions — scalar via saturate_cast(src*a+b) ////////////////////
#define DEF_CVT_SCALE_SCALAR_FUNC(suffix, stype, dtype, wtype) \
स्थिर शून्य cvtScale##suffix( ध्रुव uchar* src_, size_t sstep, ध्रुव uchar*, size_t, \
                              uchar* dst_, size_t dstep, Size size, शून्य* scale_) \
{ \
    ध्रुव stype* src = (ध्रुव stype*)src_; dtype* dst = (dtype*)dst_; \
    ध्रुव द्विगुण* scale = (ध्रुव द्विगुण*)scale_; \
    wtype a = (wtype)scale[0], b = (wtype)scale[1]; \
    sstep /= परिमाण(src[0]); dstep /= परिमाण(dst[0]); \
    प्रति( पूर्णाङ्क i = 0; i < size.height; i++, src += sstep, dst += dstep ) \
        प्रति (पूर्णाङ्क j = 0; j < size.width; j++) \
            dst[j] = saturate_cast<dtype>((wtype)src[j]*a + b); \
}

// double intermediate so coarse FP8 grids round identically to the (double-precision) reference
#define DEF_CVT_SCALE_FP8(S, T) \
    DEF_CVT_SCALE_SCALAR_FUNC(S##8u,   T, uchar, द्विगुण)     DEF_CVT_SCALE_SCALAR_FUNC(8u##S,   uchar, T, द्विगुण) \
    DEF_CVT_SCALE_SCALAR_FUNC(S##8s,   T, schar, द्विगुण)     DEF_CVT_SCALE_SCALAR_FUNC(8s##S,   schar, T, द्विगुण) \
    DEF_CVT_SCALE_SCALAR_FUNC(S##16u,  T, ushort, द्विगुण)    DEF_CVT_SCALE_SCALAR_FUNC(16u##S,  ushort, T, द्विगुण) \
    DEF_CVT_SCALE_SCALAR_FUNC(S##16s,  T, short, द्विगुण)     DEF_CVT_SCALE_SCALAR_FUNC(16s##S,  short, T, द्विगुण) \
    DEF_CVT_SCALE_SCALAR_FUNC(S##32u,  T, unsigned, द्विगुण)  DEF_CVT_SCALE_SCALAR_FUNC(32u##S,  unsigned, T, द्विगुण) \
    DEF_CVT_SCALE_SCALAR_FUNC(S##32s,  T, पूर्णाङ्क, द्विगुण)       DEF_CVT_SCALE_SCALAR_FUNC(32s##S,  पूर्णाङ्क, T, द्विगुण) \
    DEF_CVT_SCALE_SCALAR_FUNC(S##32f,  T, भिन्न, द्विगुण) \
    DEF_CVT_SCALE_SCALAR_FUNC(S##64f,  T, द्विगुण, द्विगुण)    DEF_CVT_SCALE_SCALAR_FUNC(64f##S,  द्विगुण, T, द्विगुण) \
    DEF_CVT_SCALE_SCALAR_FUNC(S##16f,  T, hfloat, द्विगुण)    DEF_CVT_SCALE_SCALAR_FUNC(16f##S,  hfloat, T, द्विगुण) \
    DEF_CVT_SCALE_SCALAR_FUNC(S##16bf, T, bfloat, द्विगुण)    DEF_CVT_SCALE_SCALAR_FUNC(16bf##S, bfloat, T, द्विगुण) \
    DEF_CVT_SCALE_SCALAR_FUNC(S##64u,  T, uint64_t, द्विगुण)  DEF_CVT_SCALE_SCALAR_FUNC(64u##S,  uint64_t, T, द्विगुण) \
    DEF_CVT_SCALE_SCALAR_FUNC(S##64s,  T, int64_t, द्विगुण)   DEF_CVT_SCALE_SCALAR_FUNC(64s##S,  int64_t, T, द्विगुण) \
    DEF_CVT_SCALE_SCALAR_FUNC(S##8b,   T, तार्किक, द्विगुण) \
    DEF_CVT_SCALE_SCALAR_FUNC(S##8fe4m3,  T, fp8_t, द्विगुण)   DEF_CVT_SCALE_SCALAR_FUNC(S##8fe4m3u, T, fp8a_t, द्विगुण)

DEF_CVT_SCALE_FP8(8fe4m3,  fp8_t)
DEF_CVT_SCALE_FP8(8fe4m3u, fp8a_t)

// convertTo(dst, depth, alpha, beta) path -- real quantization. Reuses convert.hpp's fast path.
// Double precision here matches the scalar reference (wtype)x*a+b bit-for-bit.
template<पूर्णाङ्क bias, तार्किक fnuz> स्थिर शून्य
cvtScaleF32ToFp8(ध्रुव uchar* src_, size_t sstep, ध्रुव uchar*, size_t,
                 uchar* dst, size_t dstep, Size size, शून्य* scale_)
{
    CV_INSTRUMENT_REGION();
    ध्रुव भिन्न* src = (ध्रुव भिन्न*)src_;
    ध्रुव द्विगुण* scale = (ध्रुव द्विगुण*)scale_;
    द्विगुण a = scale[0], b = scale[1];
    sstep /= परिमाण(src[0]);

    प्रति (पूर्णाङ्क i = 0; i < size.height; i++, src += sstep, dst += dstep)
    {
        पूर्णाङ्क j = 0;
#यदि (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)
        v_float64 va = vx_setall_f64(a), vb = vx_setall_f64(b);
        ध्रुव पूर्णाङ्क LANES32 = VTraits<v_float32>::vlanes();
        ध्रुव पूर्णाङ्क VECSZ = LANES32 * 4;
        भिन्न scaled[VTraits<v_float32>::max_nlanes * 4];
        प्रति (; j <= size.width - VECSZ; j += VECSZ)
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
        प्रति (; j < size.width; j++)
            dst[j] = fp8_detail::encodeE4M3((भिन्न)((द्विगुण)src[j]*a + b), bias, fnuz);
    }
}
स्थिर शून्य cvtScale32f8fe4m3(ध्रुव uchar* s, size_t ss, ध्रुव uchar* p, size_t ps, uchar* d, size_t ds, Size sz, शून्य* x)
{ cvtScaleF32ToFp8<7, असत्य>(s, ss, p, ps, d, ds, sz, x); }
स्थिर शून्य cvtScale32f8fe4m3u(ध्रुव uchar* s, size_t ss, ध्रुव uchar* p, size_t ps, uchar* d, size_t ds, Size sz, शून्य* x)
{ cvtScaleF32ToFp8<8, सत्य>(s, ss, p, ps, d, ds, sz, x); }

द्विआधारी_Func getConvertScaleFunc(पूर्णाङ्क sdepth_, पूर्णाङ्क ddepth_)
{
    पूर्णाङ्क sdepth = CV_MAT_DEPTH(sdepth_);
    पूर्णाङ्क ddepth = CV_MAT_DEPTH(ddepth_);
    द्विआधारी_Func func =
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
    प्रत्यागम func;
}

#endif

CV_CPU_OPTIMIZATION_NAMESPACE_END
} // namespace
