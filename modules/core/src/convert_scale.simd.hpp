// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


#समावेश "precomp.hpp"
#समावेश "convert.hpp"

namespace cv {
CV_CPU_OPTIMIZATION_NAMESPACE_BEGIN

द्विआधारी_Func getCvtScaleAbsFunc(पूर्णांक depth);
द्विआधारी_Func getConvertScaleFunc(पूर्णांक sdepth, पूर्णांक ddepth);

#अगर_अघोषित CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

/****************************************************************************************\
*                                convertScale[Abs]                                       *
\****************************************************************************************/

ढाँचा<typename _Ts, typename _Td> अंतरभूत व्योम
cvtabs_32f( स्थिर _Ts* src, माप_प्रकार sstep, _Td* dst, माप_प्रकार dstep,
            आकार size, भग्न a, भग्न b )
{
#अगर (CV_SIMD || CV_SIMD_SCALABLE)
    v_float32 va = vx_setall_f32(a), vb = vx_setall_f32(b);
    स्थिर पूर्णांक VECSZ = VTraits<v_float32>::vlanes()*2;
    // GCC miscompiles this scalable block only on VLEN=128 RVV (opencv/issues/26936);
    // use scalar there, keep SIMD on wider vectors. Ask the hardware for VLEN directly:
    // vlanes() would depend on the LMUL the universal intrinsics happen to use.
#अगर (CV_RVV && CV_SIMD_SCALABLE && defined(__GNUC__) && !defined(__clang__))
    स्थिर bool useSIMD = __riscv_vlenb()*8 != 128;
#अन्यथा
    स्थिर bool useSIMD = true;
#पूर्ण_अगर
#पूर्ण_अगर
    sstep /= माप(src[0]);
    dstep /= माप(dst[0]);

    क्रम( पूर्णांक i = 0; i < size.height; i++, src += sstep, dst += dstep )
    {
        पूर्णांक j = 0;
#अगर (CV_SIMD || CV_SIMD_SCALABLE)
        अगर( useSIMD )
        क्रम( ; j < size.width; j += VECSZ )
        {
            अगर( j > size.width - VECSZ )
            {
                अगर( j == 0 || src == (_Ts*)dst )
                    अवरोध;
                j = size.width - VECSZ;
            }
            v_float32 v0, v1;
            vx_load_pair_as(src + j, v0, v1);
            v0 = v_fma(v0, va, vb);
            v1 = v_fma(v1, va, vb);
            v_store_pair_as(dst + j, v_abs(v0), v_abs(v1));
        }
#पूर्ण_अगर
        क्रम( ; j < size.width; j++ )
            dst[j] = saturate_cast<_Td>(std::असल(src[j]*a + b));
    }
}

अटल व्योम
cvtabs_32f( स्थिर bool* src_, माप_प्रकार sstep,
            uchar* dst, माप_प्रकार dstep,
            आकार size, भग्न a, भग्न b )
{
    स्थिर uchar* src = (स्थिर uchar*)src_;
    uchar v0 = saturate_cast<uchar>(std::असल(b));
    uchar v1 = saturate_cast<uchar>(std::असल(a + b));
    क्रम( पूर्णांक i = 0; i < size.height; i++, src += sstep, dst += dstep )
    {
        क्रम (पूर्णांक j = 0; j < size.width; j++)
            dst[j] = src[j] != 0 ? v1 : v0;
    }
}

ढाँचा<typename _Ts, typename _Td> अंतरभूत व्योम
cvt_32f( स्थिर _Ts* src, माप_प्रकार sstep, _Td* dst, माप_प्रकार dstep,
         आकार size, भग्न a, भग्न b )
{
#अगर (CV_SIMD || (CV_SIMD_SCALABLE && !(defined(__GNUC__) && !defined(__clang__))) )
    v_float32 va = vx_setall_f32(a), vb = vx_setall_f32(b);
    स्थिर पूर्णांक VECSZ = VTraits<v_float32>::vlanes()*2;
#पूर्ण_अगर
    sstep /= माप(src[0]);
    dstep /= माप(dst[0]);

    क्रम( पूर्णांक i = 0; i < size.height; i++, src += sstep, dst += dstep )
    {
        पूर्णांक j = 0;
// Excluding GNU in CV_SIMD_SCALABLE because of "opencv/issues/26936"
#अगर (CV_SIMD || (CV_SIMD_SCALABLE && !(defined(__GNUC__) && !defined(__clang__))) )
        क्रम( ; j < size.width; j += VECSZ )
        {
            अगर( j > size.width - VECSZ )
            {
                अगर( j == 0 || src == (_Ts*)dst )
                    अवरोध;
                j = size.width - VECSZ;
            }
            v_float32 v0, v1;
            vx_load_pair_as(src + j, v0, v1);
            v0 = v_fma(v0, va, vb);
            v1 = v_fma(v1, va, vb);
            v_store_pair_as(dst + j, v0, v1);
        }
#पूर्ण_अगर
        क्रम( ; j < size.width; j++ )
            dst[j] = saturate_cast<_Td>(src[j]*a + b);
    }
}

// variant for conversions 16f <-> ... w/o unrolling
ढाँचा<typename _Ts, typename _Td> अंतरभूत व्योम
cvt1_32f( स्थिर _Ts* src, माप_प्रकार sstep, _Td* dst, माप_प्रकार dstep,
          आकार size, भग्न a, भग्न b )
{
#अगर (CV_SIMD || CV_SIMD_SCALABLE)
    v_float32 va = vx_setall_f32(a), vb = vx_setall_f32(b);
    स्थिर पूर्णांक VECSZ = VTraits<v_float32>::vlanes();
#पूर्ण_अगर
    sstep /= माप(src[0]);
    dstep /= माप(dst[0]);

    क्रम( पूर्णांक i = 0; i < size.height; i++, src += sstep, dst += dstep )
    {
        पूर्णांक j = 0;
#अगर (CV_SIMD || CV_SIMD_SCALABLE)
        क्रम( ; j < size.width; j += VECSZ )
        {
            अगर( j > size.width - VECSZ )
            {
                अगर( j == 0 || src == (_Ts*)dst )
                    अवरोध;
                j = size.width - VECSZ;
            }
            v_float32 v0;
            vx_load_as(src + j, v0);
            v0 = v_fma(v0, va, vb);
            v_store_as(dst + j, v0);
        }
#पूर्ण_अगर
        क्रम( ; j < size.width; j++ )
            dst[j] = saturate_cast<_Td>(src[j]*a + b);
    }
}


ढाँचा<typename _Ts, typename _Td> अंतरभूत व्योम
cvt_64f( स्थिर _Ts* src, माप_प्रकार sstep, _Td* dst, माप_प्रकार dstep,
         आकार size, द्विगुन a, द्विगुन b )
{
#अगर (CV_SIMD_64F || (CV_SIMD_SCALABLE_64F && !(defined(__GNUC__) && !defined(__clang__))) )
    v_float64 va = vx_setall_f64(a), vb = vx_setall_f64(b);
    स्थिर पूर्णांक VECSZ = VTraits<v_float64>::vlanes()*2;
#पूर्ण_अगर
    sstep /= माप(src[0]);
    dstep /= माप(dst[0]);

    क्रम( पूर्णांक i = 0; i < size.height; i++, src += sstep, dst += dstep )
    {
        पूर्णांक j = 0;
// Excluding GNU in CV_SIMD_SCALABLE because of "opencv/issues/26936"
#अगर (CV_SIMD_64F || (CV_SIMD_SCALABLE_64F && !(defined(__GNUC__) && !defined(__clang__))) )
        क्रम( ; j < size.width; j += VECSZ )
        {
            अगर( j > size.width - VECSZ )
            {
                अगर( j == 0 || src == (_Ts*)dst )
                    अवरोध;
                j = size.width - VECSZ;
            }
            v_float64 v0, v1;
            vx_load_pair_as(src + j, v0, v1);
            v0 = v_fma(v0, va, vb);
            v1 = v_fma(v1, va, vb);
            v_store_pair_as(dst + j, v0, v1);
        }
#पूर्ण_अगर
        क्रम( ; j < size.width; j++ )
            dst[j] = saturate_cast<_Td>(src[j]*a + b);
    }
}

//==================================================================================================

#घोषणा DEF_CVT_SCALE_ABS_FUNC(suffix, cvt, stype, dtype, wtype) \
अटल व्योम cvtScaleAbs##suffix( स्थिर uchar* src_, माप_प्रकार sstep, स्थिर uchar*, माप_प्रकार, \
                                 uchar* dst_, माप_प्रकार dstep, आकार size, व्योम* scale_) \
{ \
    स्थिर stype* src = (स्थिर stype*)src_; \
    dtype* dst = (dtype*)dst_; \
    द्विगुन* scale = (द्विगुन*)scale_; \
    cvt(src, sstep, dst, dstep, size, (wtype)scale[0], (wtype)scale[1]); \
}


#घोषणा DEF_CVT_SCALE_FUNC(suffix, cvt, stype, dtype, wtype) \
अटल व्योम cvtScale##suffix( स्थिर uchar* src_, माप_प्रकार sstep, स्थिर uchar*, माप_प्रकार, \
                              uchar* dst_, माप_प्रकार dstep, आकार size, व्योम* scale_) \
{ \
    स्थिर stype* src = (स्थिर stype*)src_; \
    dtype* dst = (dtype*)dst_; \
    द्विगुन* scale = (द्विगुन*)scale_; \
    cvt(src, sstep, dst, dstep, size, (wtype)scale[0], (wtype)scale[1]); \
}

#घोषणा DEF_CVT_SCALE2BOOL_FUNC(suffix, stype, wtype) \
अटल व्योम cvtScale##suffix( स्थिर uchar* src_, माप_प्रकार sstep, स्थिर uchar*, माप_प्रकार, \
                              uchar* dst, माप_प्रकार dstep, आकार size, व्योम* scale_) \
{ \
    स्थिर stype* src = (स्थिर stype*)src_; \
    स्थिर द्विगुन* scale = (स्थिर द्विगुन*)scale_; \
    wtype a = (wtype)scale[0], b = (wtype)scale[1]; \
    sstep /= माप(src[0]); \
    क्रम( पूर्णांक i = 0; i < size.height; i++, src += sstep, dst += dstep ) \
        क्रम (पूर्णांक j = 0; j < size.width; j++) \
            dst[j] = (bool)((wtype)src[j]*a + b != 0); \
}

#घोषणा DEF_CVT_SCALEBOOL2_FUNC(suffix, dtype, wtype) \
अटल व्योम cvtScale##suffix( स्थिर uchar* src, माप_प्रकार sstep, स्थिर uchar*, माप_प्रकार, \
                              uchar* dst_, माप_प्रकार dstep, आकार size, व्योम* scale_) \
{ \
    dtype* dst = (dtype*)dst_; \
    स्थिर द्विगुन* scale = (स्थिर द्विगुन*)scale_; \
    wtype a = (wtype)scale[0], b = (wtype)scale[1]; \
    dtype v0 = saturate_cast<dtype>(b), v1 = saturate_cast<dtype>(a + b); \
    dstep /= माप(dst[0]); \
    क्रम( पूर्णांक i = 0; i < size.height; i++, src += sstep, dst += dstep ) \
        क्रम (पूर्णांक j = 0; j < size.width; j++) \
            dst[j] = src[j] != 0 ? v1 : v0; \
}

DEF_CVT_SCALE_ABS_FUNC(8u,    cvtabs_32f, uchar,  uchar, भग्न)
DEF_CVT_SCALE_ABS_FUNC(8s8u,  cvtabs_32f, schar,  uchar, भग्न)
DEF_CVT_SCALE_ABS_FUNC(8b8u,  cvtabs_32f, bool,  uchar, भग्न)
DEF_CVT_SCALE_ABS_FUNC(16u8u, cvtabs_32f, ushort, uchar, भग्न)
DEF_CVT_SCALE_ABS_FUNC(16s8u, cvtabs_32f, लघु,  uchar, भग्न)
DEF_CVT_SCALE_ABS_FUNC(32u8u, cvtabs_32f, अचिन्हित, uchar, भग्न)
DEF_CVT_SCALE_ABS_FUNC(32s8u, cvtabs_32f, पूर्णांक,    uchar, भग्न)
DEF_CVT_SCALE_ABS_FUNC(32f8u, cvtabs_32f, भग्न,  uchar, भग्न)
DEF_CVT_SCALE_ABS_FUNC(64u8u, cvtabs_32f, uint64_t, uchar, भग्न)
DEF_CVT_SCALE_ABS_FUNC(64s8u, cvtabs_32f, int64_t, uchar, भग्न)
DEF_CVT_SCALE_ABS_FUNC(64f8u, cvtabs_32f, द्विगुन, uchar, भग्न)
DEF_CVT_SCALE_ABS_FUNC(16f8u, cvtabs_32f, hfloat, uchar, भग्न)
DEF_CVT_SCALE_ABS_FUNC(16bf8u, cvtabs_32f, bfloat, uchar, भग्न)

DEF_CVT_SCALE_FUNC(8u,     cvt_32f, uchar,  uchar, भग्न)
DEF_CVT_SCALE_FUNC(8s8u,   cvt_32f, schar,  uchar, भग्न)
DEF_CVT_SCALE_FUNC(16u8u,  cvt_32f, ushort, uchar, भग्न)
DEF_CVT_SCALE_FUNC(16s8u,  cvt_32f, लघु,  uchar, भग्न)
DEF_CVT_SCALE_FUNC(32u8u,  cvt_32f, अचिन्हित, uchar, भग्न)
DEF_CVT_SCALE_FUNC(32s8u,  cvt_32f, पूर्णांक,    uchar, भग्न)
DEF_CVT_SCALE_FUNC(32f8u,  cvt_32f, भग्न,  uchar, भग्न)
DEF_CVT_SCALE_FUNC(64f8u,  cvt_32f, द्विगुन, uchar, भग्न)
DEF_CVT_SCALE_FUNC(64u8u,  cvt_32f, uint64_t, uchar, भग्न)
DEF_CVT_SCALE_FUNC(64s8u,  cvt_32f, int64_t, uchar, भग्न)
DEF_CVT_SCALE_FUNC(16f8u,  cvt_32f, hfloat, uchar, भग्न)
DEF_CVT_SCALE_FUNC(16bf8u, cvt_32f, bfloat, uchar, भग्न)

DEF_CVT_SCALE_FUNC(8u8s,   cvt_32f, uchar,  schar, भग्न)
DEF_CVT_SCALE_FUNC(8s,     cvt_32f, schar,  schar, भग्न)
DEF_CVT_SCALE_FUNC(16u8s,  cvt_32f, ushort, schar, भग्न)
DEF_CVT_SCALE_FUNC(16s8s,  cvt_32f, लघु,  schar, भग्न)
DEF_CVT_SCALE_FUNC(32u8s,  cvt_32f, अचिन्हित, schar, भग्न)
DEF_CVT_SCALE_FUNC(32s8s,  cvt_32f, पूर्णांक,    schar, भग्न)
DEF_CVT_SCALE_FUNC(32f8s,  cvt_32f, भग्न,  schar, भग्न)
DEF_CVT_SCALE_FUNC(64f8s,  cvt_32f, द्विगुन, schar, भग्न)
DEF_CVT_SCALE_FUNC(64u8s,  cvt_32f, uint64_t, schar, भग्न)
DEF_CVT_SCALE_FUNC(64s8s,  cvt_32f, int64_t, schar, भग्न)
DEF_CVT_SCALE_FUNC(16f8s,  cvt_32f, hfloat, schar, भग्न)
DEF_CVT_SCALE_FUNC(16bf8s, cvt_32f, bfloat, schar, भग्न)

DEF_CVT_SCALE2BOOL_FUNC(8u8b, uchar, भग्न)
DEF_CVT_SCALE2BOOL_FUNC(8s8b, schar, भग्न)
DEF_CVT_SCALE2BOOL_FUNC(16u8b, ushort, भग्न)
DEF_CVT_SCALE2BOOL_FUNC(16s8b, लघु, भग्न)
DEF_CVT_SCALE2BOOL_FUNC(32u8b, अचिन्हित, भग्न)
DEF_CVT_SCALE2BOOL_FUNC(32s8b, पूर्णांक, भग्न)
DEF_CVT_SCALE2BOOL_FUNC(32f8b, भग्न, भग्न)
DEF_CVT_SCALE2BOOL_FUNC(64f8b, द्विगुन, भग्न)
DEF_CVT_SCALE2BOOL_FUNC(64u8b, uint64_t, भग्न)
DEF_CVT_SCALE2BOOL_FUNC(64s8b, int64_t, भग्न)
DEF_CVT_SCALE2BOOL_FUNC(16f8b, hfloat, भग्न)
DEF_CVT_SCALE2BOOL_FUNC(16bf8b, bfloat, भग्न)

DEF_CVT_SCALE_FUNC(8u16u,  cvt_32f, uchar,  ushort, भग्न)
DEF_CVT_SCALE_FUNC(8s16u,  cvt_32f, schar,  ushort, भग्न)
DEF_CVT_SCALE_FUNC(16u,    cvt_32f, ushort, ushort, भग्न)
DEF_CVT_SCALE_FUNC(16s16u, cvt_32f, लघु,  ushort, भग्न)
DEF_CVT_SCALE_FUNC(32u16u, cvt_32f, अचिन्हित, ushort, भग्न)
DEF_CVT_SCALE_FUNC(32s16u, cvt_32f, पूर्णांक,    ushort, भग्न)
DEF_CVT_SCALE_FUNC(32f16u, cvt_32f, भग्न,  ushort, भग्न)
DEF_CVT_SCALE_FUNC(64f16u, cvt_32f, द्विगुन, ushort, भग्न)
DEF_CVT_SCALE_FUNC(64u16u, cvt_32f, uint64_t, ushort, भग्न)
DEF_CVT_SCALE_FUNC(64s16u, cvt_32f, int64_t, ushort, भग्न)
DEF_CVT_SCALE_FUNC(16f16u, cvt1_32f, hfloat, ushort, भग्न)
DEF_CVT_SCALE_FUNC(16bf16u, cvt1_32f, bfloat, ushort, भग्न)

DEF_CVT_SCALE_FUNC(8u16s,  cvt_32f, uchar,  लघु, भग्न)
DEF_CVT_SCALE_FUNC(8s16s,  cvt_32f, schar,  लघु, भग्न)
DEF_CVT_SCALE_FUNC(16u16s, cvt_32f, ushort, लघु, भग्न)
DEF_CVT_SCALE_FUNC(16s,    cvt_32f, लघु,  लघु, भग्न)
DEF_CVT_SCALE_FUNC(32u16s, cvt_32f, अचिन्हित, लघु, भग्न)
DEF_CVT_SCALE_FUNC(32s16s, cvt_32f, पूर्णांक,    लघु, भग्न)
DEF_CVT_SCALE_FUNC(32f16s, cvt_32f, भग्न,  लघु, भग्न)
DEF_CVT_SCALE_FUNC(64f16s, cvt_32f, द्विगुन, लघु, भग्न)
DEF_CVT_SCALE_FUNC(64u16s, cvt_32f, uint64_t, लघु, भग्न)
DEF_CVT_SCALE_FUNC(64s16s, cvt_32f, int64_t, लघु, भग्न)
DEF_CVT_SCALE_FUNC(16f16s, cvt1_32f, hfloat, लघु, भग्न)
DEF_CVT_SCALE_FUNC(16bf16s, cvt1_32f, bfloat, लघु, भग्न)

DEF_CVT_SCALE_FUNC(8u32u,  cvt_32f, uchar,  अचिन्हित, भग्न)
DEF_CVT_SCALE_FUNC(8s32u,  cvt_32f, schar,  अचिन्हित, भग्न)
DEF_CVT_SCALE_FUNC(16u32u, cvt_32f, ushort, अचिन्हित, भग्न)
DEF_CVT_SCALE_FUNC(16s32u, cvt_32f, लघु,  अचिन्हित, भग्न)
DEF_CVT_SCALE_FUNC(32u, cvt_32f, अचिन्हित, अचिन्हित, भग्न)
DEF_CVT_SCALE_FUNC(32s32u, cvt_64f, पूर्णांक,    अचिन्हित, द्विगुन)
DEF_CVT_SCALE_FUNC(32f32u, cvt_32f, भग्न,  अचिन्हित, भग्न)
DEF_CVT_SCALE_FUNC(64f32u, cvt_64f, द्विगुन, अचिन्हित, द्विगुन)
DEF_CVT_SCALE_FUNC(64u32u, cvt_32f, uint64_t, अचिन्हित, भग्न)
DEF_CVT_SCALE_FUNC(64s32u, cvt_32f, int64_t, अचिन्हित, भग्न)
DEF_CVT_SCALE_FUNC(16f32u, cvt1_32f, hfloat, अचिन्हित, भग्न)
DEF_CVT_SCALE_FUNC(16bf32u, cvt1_32f, bfloat, अचिन्हित, भग्न)

DEF_CVT_SCALE_FUNC(8u32s,  cvt_32f, uchar,  पूर्णांक, भग्न)
DEF_CVT_SCALE_FUNC(8s32s,  cvt_32f, schar,  पूर्णांक, भग्न)
DEF_CVT_SCALE_FUNC(16u32s, cvt_32f, ushort, पूर्णांक, भग्न)
DEF_CVT_SCALE_FUNC(16s32s, cvt_32f, लघु,  पूर्णांक, भग्न)
DEF_CVT_SCALE_FUNC(32u32s, cvt_32f, अचिन्हित, पूर्णांक, भग्न)
DEF_CVT_SCALE_FUNC(32s,    cvt_64f, पूर्णांक,    पूर्णांक, द्विगुन)
DEF_CVT_SCALE_FUNC(32f32s, cvt_32f, भग्न,  पूर्णांक, भग्न)
DEF_CVT_SCALE_FUNC(64f32s, cvt_64f, द्विगुन, पूर्णांक, द्विगुन)
DEF_CVT_SCALE_FUNC(64u32s, cvt_32f, uint64_t, पूर्णांक, भग्न)
DEF_CVT_SCALE_FUNC(64s32s, cvt_32f, int64_t, पूर्णांक, भग्न)
DEF_CVT_SCALE_FUNC(16f32s, cvt1_32f, hfloat, पूर्णांक, भग्न)
DEF_CVT_SCALE_FUNC(16bf32s, cvt1_32f, bfloat, पूर्णांक, भग्न)

DEF_CVT_SCALE_FUNC(8u32f,  cvt_32f, uchar,  भग्न, भग्न)
DEF_CVT_SCALE_FUNC(8s32f,  cvt_32f, schar,  भग्न, भग्न)
DEF_CVT_SCALE_FUNC(16u32f, cvt_32f, ushort, भग्न, भग्न)
DEF_CVT_SCALE_FUNC(16s32f, cvt_32f, लघु,  भग्न, भग्न)
DEF_CVT_SCALE_FUNC(32u32f, cvt_32f, अचिन्हित, भग्न, भग्न)
DEF_CVT_SCALE_FUNC(32s32f, cvt_32f, पूर्णांक,    भग्न, भग्न)
DEF_CVT_SCALE_FUNC(32f,    cvt_32f, भग्न,  भग्न, भग्न)
DEF_CVT_SCALE_FUNC(64f32f, cvt_64f, द्विगुन, भग्न, द्विगुन)
DEF_CVT_SCALE_FUNC(64u32f, cvt_32f, uint64_t, भग्न, भग्न)
DEF_CVT_SCALE_FUNC(64s32f, cvt_32f, int64_t, भग्न, भग्न)
DEF_CVT_SCALE_FUNC(16f32f, cvt1_32f, hfloat, भग्न, भग्न)
DEF_CVT_SCALE_FUNC(16bf32f, cvt1_32f, bfloat, भग्न, भग्न)

DEF_CVT_SCALE_FUNC(8u64f,  cvt_64f, uchar,  द्विगुन, द्विगुन)
DEF_CVT_SCALE_FUNC(8s64f,  cvt_64f, schar,  द्विगुन, द्विगुन)
DEF_CVT_SCALE_FUNC(16u64f, cvt_64f, ushort, द्विगुन, द्विगुन)
DEF_CVT_SCALE_FUNC(16s64f, cvt_64f, लघु,  द्विगुन, द्विगुन)
DEF_CVT_SCALE_FUNC(32u64f, cvt_64f, अचिन्हित, द्विगुन, द्विगुन)
DEF_CVT_SCALE_FUNC(32s64f, cvt_64f, पूर्णांक,    द्विगुन, द्विगुन)
DEF_CVT_SCALE_FUNC(32f64f, cvt_64f, भग्न,  द्विगुन, द्विगुन)
DEF_CVT_SCALE_FUNC(64f,    cvt_64f, द्विगुन, द्विगुन, द्विगुन)
DEF_CVT_SCALE_FUNC(64u64f, cvt_64f, uint64_t, द्विगुन, द्विगुन)
DEF_CVT_SCALE_FUNC(64s64f, cvt_64f, int64_t, द्विगुन, द्विगुन)
DEF_CVT_SCALE_FUNC(16f64f, cvt_64f, hfloat, द्विगुन, द्विगुन)
DEF_CVT_SCALE_FUNC(16bf64f, cvt_64f, bfloat, द्विगुन, द्विगुन)

DEF_CVT_SCALE_FUNC(8u64u,  cvt_64f, uchar,  uint64_t, द्विगुन)
DEF_CVT_SCALE_FUNC(8s64u,  cvt_64f, schar,  uint64_t, द्विगुन)
DEF_CVT_SCALE_FUNC(16u64u, cvt_64f, ushort, uint64_t, द्विगुन)
DEF_CVT_SCALE_FUNC(16s64u, cvt_64f, लघु,  uint64_t, द्विगुन)
DEF_CVT_SCALE_FUNC(32u64u, cvt_64f, अचिन्हित, uint64_t, द्विगुन)
DEF_CVT_SCALE_FUNC(32s64u, cvt_64f, पूर्णांक,    uint64_t, द्विगुन)
DEF_CVT_SCALE_FUNC(32f64u, cvt_64f, भग्न,  uint64_t, द्विगुन)
DEF_CVT_SCALE_FUNC(64f64u, cvt_64f, द्विगुन, uint64_t, द्विगुन)
DEF_CVT_SCALE_FUNC(64u, cvt_64f, uint64_t, uint64_t, द्विगुन)
DEF_CVT_SCALE_FUNC(64s64u, cvt_64f, int64_t, uint64_t, द्विगुन)
DEF_CVT_SCALE_FUNC(16f64u, cvt_64f, hfloat, uint64_t, द्विगुन)
DEF_CVT_SCALE_FUNC(16bf64u, cvt_64f, bfloat, uint64_t, द्विगुन)

DEF_CVT_SCALE_FUNC(8u64s,  cvt_64f, uchar,  int64_t, द्विगुन)
DEF_CVT_SCALE_FUNC(8s64s,  cvt_64f, schar,  int64_t, द्विगुन)
DEF_CVT_SCALE_FUNC(16u64s, cvt_64f, ushort, int64_t, द्विगुन)
DEF_CVT_SCALE_FUNC(16s64s, cvt_64f, लघु,  int64_t, द्विगुन)
DEF_CVT_SCALE_FUNC(32u64s, cvt_64f, अचिन्हित, int64_t, द्विगुन)
DEF_CVT_SCALE_FUNC(32s64s, cvt_64f, पूर्णांक,    int64_t, द्विगुन)
DEF_CVT_SCALE_FUNC(32f64s, cvt_64f, भग्न,  int64_t, द्विगुन)
DEF_CVT_SCALE_FUNC(64f64s, cvt_64f, द्विगुन, int64_t, द्विगुन)
DEF_CVT_SCALE_FUNC(64u64s, cvt_64f, uint64_t, int64_t, द्विगुन)
DEF_CVT_SCALE_FUNC(64s, cvt_64f, int64_t, int64_t, द्विगुन)
DEF_CVT_SCALE_FUNC(16f64s, cvt_64f, hfloat, int64_t, द्विगुन)
DEF_CVT_SCALE_FUNC(16bf64s, cvt_64f, bfloat, int64_t, द्विगुन)

DEF_CVT_SCALE_FUNC(8u16f,  cvt1_32f, uchar,  hfloat, भग्न)
DEF_CVT_SCALE_FUNC(8s16f,  cvt1_32f, schar,  hfloat, भग्न)
DEF_CVT_SCALE_FUNC(16u16f, cvt1_32f, ushort, hfloat, भग्न)
DEF_CVT_SCALE_FUNC(16s16f, cvt1_32f, लघु,  hfloat, भग्न)
DEF_CVT_SCALE_FUNC(32u16f, cvt1_32f, अचिन्हित, hfloat, भग्न)
DEF_CVT_SCALE_FUNC(32s16f, cvt1_32f, पूर्णांक,    hfloat, भग्न)
DEF_CVT_SCALE_FUNC(32f16f, cvt1_32f, भग्न,  hfloat, भग्न)
DEF_CVT_SCALE_FUNC(64f16f, cvt1_32f, द्विगुन, hfloat, भग्न)
DEF_CVT_SCALE_FUNC(64u16f, cvt1_32f, uint64_t, hfloat, भग्न)
DEF_CVT_SCALE_FUNC(64s16f, cvt1_32f, int64_t, hfloat, भग्न)
DEF_CVT_SCALE_FUNC(16f,    cvt1_32f, hfloat, hfloat, भग्न)
DEF_CVT_SCALE_FUNC(16bf16f, cvt1_32f, bfloat, hfloat, भग्न)

DEF_CVT_SCALE_FUNC(8u16bf,  cvt1_32f, uchar,  bfloat, भग्न)
DEF_CVT_SCALE_FUNC(8s16bf,  cvt1_32f, schar,  bfloat, भग्न)
DEF_CVT_SCALE_FUNC(16u16bf, cvt1_32f, ushort, bfloat, भग्न)
DEF_CVT_SCALE_FUNC(16s16bf, cvt1_32f, लघु,  bfloat, भग्न)
DEF_CVT_SCALE_FUNC(32u16bf, cvt1_32f, अचिन्हित, bfloat, भग्न)
DEF_CVT_SCALE_FUNC(32s16bf, cvt1_32f, पूर्णांक,    bfloat, भग्न)
DEF_CVT_SCALE_FUNC(32f16bf, cvt1_32f, भग्न,  bfloat, भग्न)
DEF_CVT_SCALE_FUNC(64f16bf, cvt1_32f, द्विगुन, bfloat, भग्न)
DEF_CVT_SCALE_FUNC(64u16bf, cvt1_32f, uint64_t, bfloat, भग्न)
DEF_CVT_SCALE_FUNC(64s16bf, cvt1_32f, int64_t, bfloat, भग्न)
DEF_CVT_SCALE_FUNC(16f16bf, cvt1_32f, hfloat, bfloat, भग्न)
DEF_CVT_SCALE_FUNC(16bf, cvt1_32f, bfloat, bfloat, भग्न)

DEF_CVT_SCALEBOOL2_FUNC(8b8u, uchar, भग्न)
DEF_CVT_SCALEBOOL2_FUNC(8b8s, schar, भग्न)
DEF_CVT_SCALEBOOL2_FUNC(8b, bool, भग्न)
DEF_CVT_SCALEBOOL2_FUNC(8b16u, ushort, भग्न)
DEF_CVT_SCALEBOOL2_FUNC(8b16s, लघु, भग्न)
DEF_CVT_SCALEBOOL2_FUNC(8b32u, अचिन्हित, भग्न)
DEF_CVT_SCALEBOOL2_FUNC(8b32s, पूर्णांक, भग्न)
DEF_CVT_SCALEBOOL2_FUNC(8b32f, भग्न, भग्न)
DEF_CVT_SCALEBOOL2_FUNC(8b64u, uint64_t, द्विगुन)
DEF_CVT_SCALEBOOL2_FUNC(8b64s, int64_t, द्विगुन)
DEF_CVT_SCALEBOOL2_FUNC(8b64f, द्विगुन, द्विगुन)
DEF_CVT_SCALEBOOL2_FUNC(8b16f, hfloat, भग्न)
DEF_CVT_SCALEBOOL2_FUNC(8b16bf, bfloat, भग्न)

// FP8 src -> CV_8U abs|scale|shift (scalar)
#घोषणा DEF_CVT_SCALE_ABS_SCALAR_FUNC(suffix, stype) \
अटल व्योम cvtScaleAbs##suffix( स्थिर uchar* src_, माप_प्रकार sstep, स्थिर uchar*, माप_प्रकार, \
                                 uchar* dst, माप_प्रकार dstep, आकार size, व्योम* scale_) \
{ \
    स्थिर stype* src = (स्थिर stype*)src_; \
    स्थिर द्विगुन* scale = (स्थिर द्विगुन*)scale_; \
    भग्न a = (भग्न)scale[0], b = (भग्न)scale[1]; \
    sstep /= माप(src[0]); \
    क्रम( पूर्णांक i = 0; i < size.height; i++, src += sstep, dst += dstep ) \
        क्रम (पूर्णांक j = 0; j < size.width; j++) \
            dst[j] = saturate_cast<uchar>(std::असल((भग्न)src[j]*a + b)); \
}
DEF_CVT_SCALE_ABS_SCALAR_FUNC(8fe4m3u8u, fp8a_t)
DEF_CVT_SCALE_ABS_SCALAR_FUNC(8fe4m38u,  fp8_t)

द्विआधारी_Func getCvtScaleAbsFunc(पूर्णांक depth)
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
    वापस func;
}

//////////////////// FP8 scaled conversions — scalar via saturate_cast(src*a+b) ////////////////////
#घोषणा DEF_CVT_SCALE_SCALAR_FUNC(suffix, stype, dtype, wtype) \
अटल व्योम cvtScale##suffix( स्थिर uchar* src_, माप_प्रकार sstep, स्थिर uchar*, माप_प्रकार, \
                              uchar* dst_, माप_प्रकार dstep, आकार size, व्योम* scale_) \
{ \
    स्थिर stype* src = (स्थिर stype*)src_; dtype* dst = (dtype*)dst_; \
    स्थिर द्विगुन* scale = (स्थिर द्विगुन*)scale_; \
    wtype a = (wtype)scale[0], b = (wtype)scale[1]; \
    sstep /= माप(src[0]); dstep /= माप(dst[0]); \
    क्रम( पूर्णांक i = 0; i < size.height; i++, src += sstep, dst += dstep ) \
        क्रम (पूर्णांक j = 0; j < size.width; j++) \
            dst[j] = saturate_cast<dtype>((wtype)src[j]*a + b); \
}

// double intermediate so coarse FP8 grids round identically to the (double-precision) reference
#घोषणा DEF_CVT_SCALE_FP8(S, T) \
    DEF_CVT_SCALE_SCALAR_FUNC(S##8u,   T, uchar, द्विगुन)     DEF_CVT_SCALE_SCALAR_FUNC(8u##S,   uchar, T, द्विगुन) \
    DEF_CVT_SCALE_SCALAR_FUNC(S##8s,   T, schar, द्विगुन)     DEF_CVT_SCALE_SCALAR_FUNC(8s##S,   schar, T, द्विगुन) \
    DEF_CVT_SCALE_SCALAR_FUNC(S##16u,  T, ushort, द्विगुन)    DEF_CVT_SCALE_SCALAR_FUNC(16u##S,  ushort, T, द्विगुन) \
    DEF_CVT_SCALE_SCALAR_FUNC(S##16s,  T, लघु, द्विगुन)     DEF_CVT_SCALE_SCALAR_FUNC(16s##S,  लघु, T, द्विगुन) \
    DEF_CVT_SCALE_SCALAR_FUNC(S##32u,  T, अचिन्हित, द्विगुन)  DEF_CVT_SCALE_SCALAR_FUNC(32u##S,  अचिन्हित, T, द्विगुन) \
    DEF_CVT_SCALE_SCALAR_FUNC(S##32s,  T, पूर्णांक, द्विगुन)       DEF_CVT_SCALE_SCALAR_FUNC(32s##S,  पूर्णांक, T, द्विगुन) \
    DEF_CVT_SCALE_SCALAR_FUNC(S##32f,  T, भग्न, द्विगुन) \
    DEF_CVT_SCALE_SCALAR_FUNC(S##64f,  T, द्विगुन, द्विगुन)    DEF_CVT_SCALE_SCALAR_FUNC(64f##S,  द्विगुन, T, द्विगुन) \
    DEF_CVT_SCALE_SCALAR_FUNC(S##16f,  T, hfloat, द्विगुन)    DEF_CVT_SCALE_SCALAR_FUNC(16f##S,  hfloat, T, द्विगुन) \
    DEF_CVT_SCALE_SCALAR_FUNC(S##16bf, T, bfloat, द्विगुन)    DEF_CVT_SCALE_SCALAR_FUNC(16bf##S, bfloat, T, द्विगुन) \
    DEF_CVT_SCALE_SCALAR_FUNC(S##64u,  T, uint64_t, द्विगुन)  DEF_CVT_SCALE_SCALAR_FUNC(64u##S,  uint64_t, T, द्विगुन) \
    DEF_CVT_SCALE_SCALAR_FUNC(S##64s,  T, int64_t, द्विगुन)   DEF_CVT_SCALE_SCALAR_FUNC(64s##S,  int64_t, T, द्विगुन) \
    DEF_CVT_SCALE_SCALAR_FUNC(S##8b,   T, bool, द्विगुन) \
    DEF_CVT_SCALE_SCALAR_FUNC(S##8fe4m3,  T, fp8_t, द्विगुन)   DEF_CVT_SCALE_SCALAR_FUNC(S##8fe4m3u, T, fp8a_t, द्विगुन)

DEF_CVT_SCALE_FP8(8fe4m3,  fp8_t)
DEF_CVT_SCALE_FP8(8fe4m3u, fp8a_t)

// convertTo(dst, depth, alpha, beta) path -- real quantization. Reuses convert.hpp's fast path.
// Double precision here matches the scalar reference (wtype)x*a+b bit-for-bit.
ढाँचा<पूर्णांक bias, bool fnuz> अटल व्योम
cvtScaleF32ToFp8(स्थिर uchar* src_, माप_प्रकार sstep, स्थिर uchar*, माप_प्रकार,
                 uchar* dst, माप_प्रकार dstep, आकार size, व्योम* scale_)
{
    CV_INSTRUMENT_REGION();
    स्थिर भग्न* src = (स्थिर भग्न*)src_;
    स्थिर द्विगुन* scale = (स्थिर द्विगुन*)scale_;
    द्विगुन a = scale[0], b = scale[1];
    sstep /= माप(src[0]);

    क्रम (पूर्णांक i = 0; i < size.height; i++, src += sstep, dst += dstep)
    {
        पूर्णांक j = 0;
#अगर (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)
        v_float64 va = vx_setall_f64(a), vb = vx_setall_f64(b);
        स्थिर पूर्णांक LANES32 = VTraits<v_float32>::vlanes();
        स्थिर पूर्णांक VECSZ = LANES32 * 4;
        भग्न scaled[VTraits<v_float32>::max_nlanes * 4];
        क्रम (; j <= size.width - VECSZ; j += VECSZ)
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
#पूर्ण_अगर
        क्रम (; j < size.width; j++)
            dst[j] = fp8_detail::encodeE4M3((भग्न)((द्विगुन)src[j]*a + b), bias, fnuz);
    }
}
अटल व्योम cvtScale32f8fe4m3(स्थिर uchar* s, माप_प्रकार ss, स्थिर uchar* p, माप_प्रकार ps, uchar* d, माप_प्रकार ds, आकार sz, व्योम* x)
{ cvtScaleF32ToFp8<7, false>(s, ss, p, ps, d, ds, sz, x); }
अटल व्योम cvtScale32f8fe4m3u(स्थिर uchar* s, माप_प्रकार ss, स्थिर uchar* p, माप_प्रकार ps, uchar* d, माप_प्रकार ds, आकार sz, व्योम* x)
{ cvtScaleF32ToFp8<8, true>(s, ss, p, ps, d, ds, sz, x); }

द्विआधारी_Func getConvertScaleFunc(पूर्णांक sdepth_, पूर्णांक ddepth_)
{
    पूर्णांक sdepth = CV_MAT_DEPTH(sdepth_);
    पूर्णांक ddepth = CV_MAT_DEPTH(ddepth_);
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
    वापस func;
}

#पूर्ण_अगर

CV_CPU_OPTIMIZATION_NAMESPACE_END
} // namespace
