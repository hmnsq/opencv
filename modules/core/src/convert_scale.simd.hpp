// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


#అన్తర్భావ "precomp.hpp"
#అన్తర్భావ "convert.hpp"

నామాకాశ cv {
CV_CPU_OPTIMIZATION_NAMESPACE_BEGIN

ద్విమాన_Func getCvtScaleAbsFunc(పూర్ణాంకం లోతు);
ద్విమాన_Func getConvertScaleFunc(పూర్ణాంకం sdepth, పూర్ణాంకం ddepth);

#నిర్వచితం_కాకపోతే CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

/****************************************************************************************\
*                                convertScale[Abs]                                       *
\****************************************************************************************/

మూస<రకం_పేరు _Ts, రకం_పేరు _Td> అంతఃవరుస శూన్య
cvtabs_32f( స్థిరరాశి _Ts* src, size_t sstep, _Td* dst, size_t dstep,
            కొలత size, దశాంశం a, దశాంశం b )
{
#ఒకవేళ (CV_SIMD || CV_SIMD_SCALABLE)
    v_float32 va = vx_setall_f32(a), vb = vx_setall_f32(b);
    స్థిరరాశి పూర్ణాంకం VECSZ = VTraits<v_float32>::vlanes()*2;
    // GCC miscompiles this scalable block only on VLEN=128 RVV (opencv/issues/26936);
    // use scalar there, keep SIMD on wider vectors. Ask the hardware for VLEN directly:
    // vlanes() would depend on the LMUL the universal intrinsics happen to use.
#ఒకవేళ (CV_RVV && CV_SIMD_SCALABLE && defined(__GNUC__) && !defined(__clang__))
    స్థిరరాశి తార్కిక useSIMD = __riscv_vlenb()*8 != 128;
#లేకపోతే
    స్థిరరాశి తార్కిక useSIMD = నిజం;
#అయితే_ముగింపు
#అయితే_ముగింపు
    sstep /= పరిమాణం(src[0]);
    dstep /= పరిమాణం(dst[0]);

    కొరకు( పూర్ణాంకం i = 0; i < size.ఎత్తు; i++, src += sstep, dst += dstep )
    {
        పూర్ణాంకం j = 0;
#ఒకవేళ (CV_SIMD || CV_SIMD_SCALABLE)
        ఒకవేళ( useSIMD )
        కొరకు( ; j < size.వెడల్పు; j += VECSZ )
        {
            ఒకవేళ( j > size.వెడల్పు - VECSZ )
            {
                ఒకవేళ( j == 0 || src == (_Ts*)dst )
                    ఆపు;
                j = size.వెడల్పు - VECSZ;
            }
            v_float32 v0, v1;
            vx_ఎక్కించు_pair_as(src + j, v0, v1);
            v0 = v_fma(v0, va, vb);
            v1 = v_fma(v1, va, vb);
            v_store_pair_as(dst + j, v_abs(v0), v_abs(v1));
        }
#అయితే_ముగింపు
        కొరకు( ; j < size.వెడల్పు; j++ )
            dst[j] = saturate_cast<_Td>(std::నిరపేక్ష(src[j]*a + b));
    }
}

స్థిర శూన్య
cvtabs_32f( స్థిరరాశి తార్కిక* src_, size_t sstep,
            uchar* dst, size_t dstep,
            కొలత size, దశాంశం a, దశాంశం b )
{
    స్థిరరాశి uchar* src = (స్థిరరాశి uchar*)src_;
    uchar v0 = saturate_cast<uchar>(std::నిరపేక్ష(b));
    uchar v1 = saturate_cast<uchar>(std::నిరపేక్ష(a + b));
    కొరకు( పూర్ణాంకం i = 0; i < size.ఎత్తు; i++, src += sstep, dst += dstep )
    {
        కొరకు (పూర్ణాంకం j = 0; j < size.వెడల్పు; j++)
            dst[j] = src[j] != 0 ? v1 : v0;
    }
}

మూస<రకం_పేరు _Ts, రకం_పేరు _Td> అంతఃవరుస శూన్య
cvt_32f( స్థిరరాశి _Ts* src, size_t sstep, _Td* dst, size_t dstep,
         కొలత size, దశాంశం a, దశాంశం b )
{
#ఒకవేళ (CV_SIMD || (CV_SIMD_SCALABLE && !(defined(__GNUC__) && !defined(__clang__))) )
    v_float32 va = vx_setall_f32(a), vb = vx_setall_f32(b);
    స్థిరరాశి పూర్ణాంకం VECSZ = VTraits<v_float32>::vlanes()*2;
#అయితే_ముగింపు
    sstep /= పరిమాణం(src[0]);
    dstep /= పరిమాణం(dst[0]);

    కొరకు( పూర్ణాంకం i = 0; i < size.ఎత్తు; i++, src += sstep, dst += dstep )
    {
        పూర్ణాంకం j = 0;
// Excluding GNU in CV_SIMD_SCALABLE because of "opencv/issues/26936"
#ఒకవేళ (CV_SIMD || (CV_SIMD_SCALABLE && !(defined(__GNUC__) && !defined(__clang__))) )
        కొరకు( ; j < size.వెడల్పు; j += VECSZ )
        {
            ఒకవేళ( j > size.వెడల్పు - VECSZ )
            {
                ఒకవేళ( j == 0 || src == (_Ts*)dst )
                    ఆపు;
                j = size.వెడల్పు - VECSZ;
            }
            v_float32 v0, v1;
            vx_ఎక్కించు_pair_as(src + j, v0, v1);
            v0 = v_fma(v0, va, vb);
            v1 = v_fma(v1, va, vb);
            v_store_pair_as(dst + j, v0, v1);
        }
#అయితే_ముగింపు
        కొరకు( ; j < size.వెడల్పు; j++ )
            dst[j] = saturate_cast<_Td>(src[j]*a + b);
    }
}

// variant for conversions 16f <-> ... w/o unrolling
మూస<రకం_పేరు _Ts, రకం_పేరు _Td> అంతఃవరుస శూన్య
cvt1_32f( స్థిరరాశి _Ts* src, size_t sstep, _Td* dst, size_t dstep,
          కొలత size, దశాంశం a, దశాంశం b )
{
#ఒకవేళ (CV_SIMD || CV_SIMD_SCALABLE)
    v_float32 va = vx_setall_f32(a), vb = vx_setall_f32(b);
    స్థిరరాశి పూర్ణాంకం VECSZ = VTraits<v_float32>::vlanes();
#అయితే_ముగింపు
    sstep /= పరిమాణం(src[0]);
    dstep /= పరిమాణం(dst[0]);

    కొరకు( పూర్ణాంకం i = 0; i < size.ఎత్తు; i++, src += sstep, dst += dstep )
    {
        పూర్ణాంకం j = 0;
#ఒకవేళ (CV_SIMD || CV_SIMD_SCALABLE)
        కొరకు( ; j < size.వెడల్పు; j += VECSZ )
        {
            ఒకవేళ( j > size.వెడల్పు - VECSZ )
            {
                ఒకవేళ( j == 0 || src == (_Ts*)dst )
                    ఆపు;
                j = size.వెడల్పు - VECSZ;
            }
            v_float32 v0;
            vx_ఎక్కించు_as(src + j, v0);
            v0 = v_fma(v0, va, vb);
            v_store_as(dst + j, v0);
        }
#అయితే_ముగింపు
        కొరకు( ; j < size.వెడల్పు; j++ )
            dst[j] = saturate_cast<_Td>(src[j]*a + b);
    }
}


మూస<రకం_పేరు _Ts, రకం_పేరు _Td> అంతఃవరుస శూన్య
cvt_64f( స్థిరరాశి _Ts* src, size_t sstep, _Td* dst, size_t dstep,
         కొలత size, ద్విగుణ a, ద్విగుణ b )
{
#ఒకవేళ (CV_SIMD_64F || (CV_SIMD_SCALABLE_64F && !(defined(__GNUC__) && !defined(__clang__))) )
    v_float64 va = vx_setall_f64(a), vb = vx_setall_f64(b);
    స్థిరరాశి పూర్ణాంకం VECSZ = VTraits<v_float64>::vlanes()*2;
#అయితే_ముగింపు
    sstep /= పరిమాణం(src[0]);
    dstep /= పరిమాణం(dst[0]);

    కొరకు( పూర్ణాంకం i = 0; i < size.ఎత్తు; i++, src += sstep, dst += dstep )
    {
        పూర్ణాంకం j = 0;
// Excluding GNU in CV_SIMD_SCALABLE because of "opencv/issues/26936"
#ఒకవేళ (CV_SIMD_64F || (CV_SIMD_SCALABLE_64F && !(defined(__GNUC__) && !defined(__clang__))) )
        కొరకు( ; j < size.వెడల్పు; j += VECSZ )
        {
            ఒకవేళ( j > size.వెడల్పు - VECSZ )
            {
                ఒకవేళ( j == 0 || src == (_Ts*)dst )
                    ఆపు;
                j = size.వెడల్పు - VECSZ;
            }
            v_float64 v0, v1;
            vx_ఎక్కించు_pair_as(src + j, v0, v1);
            v0 = v_fma(v0, va, vb);
            v1 = v_fma(v1, va, vb);
            v_store_pair_as(dst + j, v0, v1);
        }
#అయితే_ముగింపు
        కొరకు( ; j < size.వెడల్పు; j++ )
            dst[j] = saturate_cast<_Td>(src[j]*a + b);
    }
}

//==================================================================================================

#నిర్వచించు DEF_CVT_SCALE_ABS_FUNC(suffix, cvt, stype, dtype, wtype) \
స్థిర శూన్య cvtScaleAbs##suffix( స్థిరరాశి uchar* src_, size_t sstep, స్థిరరాశి uchar*, size_t, \
                                 uchar* dst_, size_t dstep, కొలత size, శూన్య* scale_) \
{ \
    స్థిరరాశి stype* src = (స్థిరరాశి stype*)src_; \
    dtype* dst = (dtype*)dst_; \
    ద్విగుణ* scale = (ద్విగుణ*)scale_; \
    cvt(src, sstep, dst, dstep, size, (wtype)scale[0], (wtype)scale[1]); \
}


#నిర్వచించు DEF_CVT_SCALE_FUNC(suffix, cvt, stype, dtype, wtype) \
స్థిర శూన్య cvtScale##suffix( స్థిరరాశి uchar* src_, size_t sstep, స్థిరరాశి uchar*, size_t, \
                              uchar* dst_, size_t dstep, కొలత size, శూన్య* scale_) \
{ \
    స్థిరరాశి stype* src = (స్థిరరాశి stype*)src_; \
    dtype* dst = (dtype*)dst_; \
    ద్విగుణ* scale = (ద్విగుణ*)scale_; \
    cvt(src, sstep, dst, dstep, size, (wtype)scale[0], (wtype)scale[1]); \
}

#నిర్వచించు DEF_CVT_SCALE2BOOL_FUNC(suffix, stype, wtype) \
స్థిర శూన్య cvtScale##suffix( స్థిరరాశి uchar* src_, size_t sstep, స్థిరరాశి uchar*, size_t, \
                              uchar* dst, size_t dstep, కొలత size, శూన్య* scale_) \
{ \
    స్థిరరాశి stype* src = (స్థిరరాశి stype*)src_; \
    స్థిరరాశి ద్విగుణ* scale = (స్థిరరాశి ద్విగుణ*)scale_; \
    wtype a = (wtype)scale[0], b = (wtype)scale[1]; \
    sstep /= పరిమాణం(src[0]); \
    కొరకు( పూర్ణాంకం i = 0; i < size.ఎత్తు; i++, src += sstep, dst += dstep ) \
        కొరకు (పూర్ణాంకం j = 0; j < size.వెడల్పు; j++) \
            dst[j] = (తార్కిక)((wtype)src[j]*a + b != 0); \
}

#నిర్వచించు DEF_CVT_SCALEBOOL2_FUNC(suffix, dtype, wtype) \
స్థిర శూన్య cvtScale##suffix( స్థిరరాశి uchar* src, size_t sstep, స్థిరరాశి uchar*, size_t, \
                              uchar* dst_, size_t dstep, కొలత size, శూన్య* scale_) \
{ \
    dtype* dst = (dtype*)dst_; \
    స్థిరరాశి ద్విగుణ* scale = (స్థిరరాశి ద్విగుణ*)scale_; \
    wtype a = (wtype)scale[0], b = (wtype)scale[1]; \
    dtype v0 = saturate_cast<dtype>(b), v1 = saturate_cast<dtype>(a + b); \
    dstep /= పరిమాణం(dst[0]); \
    కొరకు( పూర్ణాంకం i = 0; i < size.ఎత్తు; i++, src += sstep, dst += dstep ) \
        కొరకు (పూర్ణాంకం j = 0; j < size.వెడల్పు; j++) \
            dst[j] = src[j] != 0 ? v1 : v0; \
}

DEF_CVT_SCALE_ABS_FUNC(8u,    cvtabs_32f, uchar,  uchar, దశాంశం)
DEF_CVT_SCALE_ABS_FUNC(8s8u,  cvtabs_32f, schar,  uchar, దశాంశం)
DEF_CVT_SCALE_ABS_FUNC(8b8u,  cvtabs_32f, తార్కిక,  uchar, దశాంశం)
DEF_CVT_SCALE_ABS_FUNC(16u8u, cvtabs_32f, ushort, uchar, దశాంశం)
DEF_CVT_SCALE_ABS_FUNC(16s8u, cvtabs_32f, పొట్టి,  uchar, దశాంశం)
DEF_CVT_SCALE_ABS_FUNC(32u8u, cvtabs_32f, గుర్తులేని, uchar, దశాంశం)
DEF_CVT_SCALE_ABS_FUNC(32s8u, cvtabs_32f, పూర్ణాంకం,    uchar, దశాంశం)
DEF_CVT_SCALE_ABS_FUNC(32f8u, cvtabs_32f, దశాంశం,  uchar, దశాంశం)
DEF_CVT_SCALE_ABS_FUNC(64u8u, cvtabs_32f, uint64_t, uchar, దశాంశం)
DEF_CVT_SCALE_ABS_FUNC(64s8u, cvtabs_32f, int64_t, uchar, దశాంశం)
DEF_CVT_SCALE_ABS_FUNC(64f8u, cvtabs_32f, ద్విగుణ, uchar, దశాంశం)
DEF_CVT_SCALE_ABS_FUNC(16f8u, cvtabs_32f, hfloat, uchar, దశాంశం)
DEF_CVT_SCALE_ABS_FUNC(16bf8u, cvtabs_32f, bfloat, uchar, దశాంశం)

DEF_CVT_SCALE_FUNC(8u,     cvt_32f, uchar,  uchar, దశాంశం)
DEF_CVT_SCALE_FUNC(8s8u,   cvt_32f, schar,  uchar, దశాంశం)
DEF_CVT_SCALE_FUNC(16u8u,  cvt_32f, ushort, uchar, దశాంశం)
DEF_CVT_SCALE_FUNC(16s8u,  cvt_32f, పొట్టి,  uchar, దశాంశం)
DEF_CVT_SCALE_FUNC(32u8u,  cvt_32f, గుర్తులేని, uchar, దశాంశం)
DEF_CVT_SCALE_FUNC(32s8u,  cvt_32f, పూర్ణాంకం,    uchar, దశాంశం)
DEF_CVT_SCALE_FUNC(32f8u,  cvt_32f, దశాంశం,  uchar, దశాంశం)
DEF_CVT_SCALE_FUNC(64f8u,  cvt_32f, ద్విగుణ, uchar, దశాంశం)
DEF_CVT_SCALE_FUNC(64u8u,  cvt_32f, uint64_t, uchar, దశాంశం)
DEF_CVT_SCALE_FUNC(64s8u,  cvt_32f, int64_t, uchar, దశాంశం)
DEF_CVT_SCALE_FUNC(16f8u,  cvt_32f, hfloat, uchar, దశాంశం)
DEF_CVT_SCALE_FUNC(16bf8u, cvt_32f, bfloat, uchar, దశాంశం)

DEF_CVT_SCALE_FUNC(8u8s,   cvt_32f, uchar,  schar, దశాంశం)
DEF_CVT_SCALE_FUNC(8s,     cvt_32f, schar,  schar, దశాంశం)
DEF_CVT_SCALE_FUNC(16u8s,  cvt_32f, ushort, schar, దశాంశం)
DEF_CVT_SCALE_FUNC(16s8s,  cvt_32f, పొట్టి,  schar, దశాంశం)
DEF_CVT_SCALE_FUNC(32u8s,  cvt_32f, గుర్తులేని, schar, దశాంశం)
DEF_CVT_SCALE_FUNC(32s8s,  cvt_32f, పూర్ణాంకం,    schar, దశాంశం)
DEF_CVT_SCALE_FUNC(32f8s,  cvt_32f, దశాంశం,  schar, దశాంశం)
DEF_CVT_SCALE_FUNC(64f8s,  cvt_32f, ద్విగుణ, schar, దశాంశం)
DEF_CVT_SCALE_FUNC(64u8s,  cvt_32f, uint64_t, schar, దశాంశం)
DEF_CVT_SCALE_FUNC(64s8s,  cvt_32f, int64_t, schar, దశాంశం)
DEF_CVT_SCALE_FUNC(16f8s,  cvt_32f, hfloat, schar, దశాంశం)
DEF_CVT_SCALE_FUNC(16bf8s, cvt_32f, bfloat, schar, దశాంశం)

DEF_CVT_SCALE2BOOL_FUNC(8u8b, uchar, దశాంశం)
DEF_CVT_SCALE2BOOL_FUNC(8s8b, schar, దశాంశం)
DEF_CVT_SCALE2BOOL_FUNC(16u8b, ushort, దశాంశం)
DEF_CVT_SCALE2BOOL_FUNC(16s8b, పొట్టి, దశాంశం)
DEF_CVT_SCALE2BOOL_FUNC(32u8b, గుర్తులేని, దశాంశం)
DEF_CVT_SCALE2BOOL_FUNC(32s8b, పూర్ణాంకం, దశాంశం)
DEF_CVT_SCALE2BOOL_FUNC(32f8b, దశాంశం, దశాంశం)
DEF_CVT_SCALE2BOOL_FUNC(64f8b, ద్విగుణ, దశాంశం)
DEF_CVT_SCALE2BOOL_FUNC(64u8b, uint64_t, దశాంశం)
DEF_CVT_SCALE2BOOL_FUNC(64s8b, int64_t, దశాంశం)
DEF_CVT_SCALE2BOOL_FUNC(16f8b, hfloat, దశాంశం)
DEF_CVT_SCALE2BOOL_FUNC(16bf8b, bfloat, దశాంశం)

DEF_CVT_SCALE_FUNC(8u16u,  cvt_32f, uchar,  ushort, దశాంశం)
DEF_CVT_SCALE_FUNC(8s16u,  cvt_32f, schar,  ushort, దశాంశం)
DEF_CVT_SCALE_FUNC(16u,    cvt_32f, ushort, ushort, దశాంశం)
DEF_CVT_SCALE_FUNC(16s16u, cvt_32f, పొట్టి,  ushort, దశాంశం)
DEF_CVT_SCALE_FUNC(32u16u, cvt_32f, గుర్తులేని, ushort, దశాంశం)
DEF_CVT_SCALE_FUNC(32s16u, cvt_32f, పూర్ణాంకం,    ushort, దశాంశం)
DEF_CVT_SCALE_FUNC(32f16u, cvt_32f, దశాంశం,  ushort, దశాంశం)
DEF_CVT_SCALE_FUNC(64f16u, cvt_32f, ద్విగుణ, ushort, దశాంశం)
DEF_CVT_SCALE_FUNC(64u16u, cvt_32f, uint64_t, ushort, దశాంశం)
DEF_CVT_SCALE_FUNC(64s16u, cvt_32f, int64_t, ushort, దశాంశం)
DEF_CVT_SCALE_FUNC(16f16u, cvt1_32f, hfloat, ushort, దశాంశం)
DEF_CVT_SCALE_FUNC(16bf16u, cvt1_32f, bfloat, ushort, దశాంశం)

DEF_CVT_SCALE_FUNC(8u16s,  cvt_32f, uchar,  పొట్టి, దశాంశం)
DEF_CVT_SCALE_FUNC(8s16s,  cvt_32f, schar,  పొట్టి, దశాంశం)
DEF_CVT_SCALE_FUNC(16u16s, cvt_32f, ushort, పొట్టి, దశాంశం)
DEF_CVT_SCALE_FUNC(16s,    cvt_32f, పొట్టి,  పొట్టి, దశాంశం)
DEF_CVT_SCALE_FUNC(32u16s, cvt_32f, గుర్తులేని, పొట్టి, దశాంశం)
DEF_CVT_SCALE_FUNC(32s16s, cvt_32f, పూర్ణాంకం,    పొట్టి, దశాంశం)
DEF_CVT_SCALE_FUNC(32f16s, cvt_32f, దశాంశం,  పొట్టి, దశాంశం)
DEF_CVT_SCALE_FUNC(64f16s, cvt_32f, ద్విగుణ, పొట్టి, దశాంశం)
DEF_CVT_SCALE_FUNC(64u16s, cvt_32f, uint64_t, పొట్టి, దశాంశం)
DEF_CVT_SCALE_FUNC(64s16s, cvt_32f, int64_t, పొట్టి, దశాంశం)
DEF_CVT_SCALE_FUNC(16f16s, cvt1_32f, hfloat, పొట్టి, దశాంశం)
DEF_CVT_SCALE_FUNC(16bf16s, cvt1_32f, bfloat, పొట్టి, దశాంశం)

DEF_CVT_SCALE_FUNC(8u32u,  cvt_32f, uchar,  గుర్తులేని, దశాంశం)
DEF_CVT_SCALE_FUNC(8s32u,  cvt_32f, schar,  గుర్తులేని, దశాంశం)
DEF_CVT_SCALE_FUNC(16u32u, cvt_32f, ushort, గుర్తులేని, దశాంశం)
DEF_CVT_SCALE_FUNC(16s32u, cvt_32f, పొట్టి,  గుర్తులేని, దశాంశం)
DEF_CVT_SCALE_FUNC(32u, cvt_32f, గుర్తులేని, గుర్తులేని, దశాంశం)
DEF_CVT_SCALE_FUNC(32s32u, cvt_64f, పూర్ణాంకం,    గుర్తులేని, ద్విగుణ)
DEF_CVT_SCALE_FUNC(32f32u, cvt_32f, దశాంశం,  గుర్తులేని, దశాంశం)
DEF_CVT_SCALE_FUNC(64f32u, cvt_64f, ద్విగుణ, గుర్తులేని, ద్విగుణ)
DEF_CVT_SCALE_FUNC(64u32u, cvt_32f, uint64_t, గుర్తులేని, దశాంశం)
DEF_CVT_SCALE_FUNC(64s32u, cvt_32f, int64_t, గుర్తులేని, దశాంశం)
DEF_CVT_SCALE_FUNC(16f32u, cvt1_32f, hfloat, గుర్తులేని, దశాంశం)
DEF_CVT_SCALE_FUNC(16bf32u, cvt1_32f, bfloat, గుర్తులేని, దశాంశం)

DEF_CVT_SCALE_FUNC(8u32s,  cvt_32f, uchar,  పూర్ణాంకం, దశాంశం)
DEF_CVT_SCALE_FUNC(8s32s,  cvt_32f, schar,  పూర్ణాంకం, దశాంశం)
DEF_CVT_SCALE_FUNC(16u32s, cvt_32f, ushort, పూర్ణాంకం, దశాంశం)
DEF_CVT_SCALE_FUNC(16s32s, cvt_32f, పొట్టి,  పూర్ణాంకం, దశాంశం)
DEF_CVT_SCALE_FUNC(32u32s, cvt_32f, గుర్తులేని, పూర్ణాంకం, దశాంశం)
DEF_CVT_SCALE_FUNC(32s,    cvt_64f, పూర్ణాంకం,    పూర్ణాంకం, ద్విగుణ)
DEF_CVT_SCALE_FUNC(32f32s, cvt_32f, దశాంశం,  పూర్ణాంకం, దశాంశం)
DEF_CVT_SCALE_FUNC(64f32s, cvt_64f, ద్విగుణ, పూర్ణాంకం, ద్విగుణ)
DEF_CVT_SCALE_FUNC(64u32s, cvt_32f, uint64_t, పూర్ణాంకం, దశాంశం)
DEF_CVT_SCALE_FUNC(64s32s, cvt_32f, int64_t, పూర్ణాంకం, దశాంశం)
DEF_CVT_SCALE_FUNC(16f32s, cvt1_32f, hfloat, పూర్ణాంకం, దశాంశం)
DEF_CVT_SCALE_FUNC(16bf32s, cvt1_32f, bfloat, పూర్ణాంకం, దశాంశం)

DEF_CVT_SCALE_FUNC(8u32f,  cvt_32f, uchar,  దశాంశం, దశాంశం)
DEF_CVT_SCALE_FUNC(8s32f,  cvt_32f, schar,  దశాంశం, దశాంశం)
DEF_CVT_SCALE_FUNC(16u32f, cvt_32f, ushort, దశాంశం, దశాంశం)
DEF_CVT_SCALE_FUNC(16s32f, cvt_32f, పొట్టి,  దశాంశం, దశాంశం)
DEF_CVT_SCALE_FUNC(32u32f, cvt_32f, గుర్తులేని, దశాంశం, దశాంశం)
DEF_CVT_SCALE_FUNC(32s32f, cvt_32f, పూర్ణాంకం,    దశాంశం, దశాంశం)
DEF_CVT_SCALE_FUNC(32f,    cvt_32f, దశాంశం,  దశాంశం, దశాంశం)
DEF_CVT_SCALE_FUNC(64f32f, cvt_64f, ద్విగుణ, దశాంశం, ద్విగుణ)
DEF_CVT_SCALE_FUNC(64u32f, cvt_32f, uint64_t, దశాంశం, దశాంశం)
DEF_CVT_SCALE_FUNC(64s32f, cvt_32f, int64_t, దశాంశం, దశాంశం)
DEF_CVT_SCALE_FUNC(16f32f, cvt1_32f, hfloat, దశాంశం, దశాంశం)
DEF_CVT_SCALE_FUNC(16bf32f, cvt1_32f, bfloat, దశాంశం, దశాంశం)

DEF_CVT_SCALE_FUNC(8u64f,  cvt_64f, uchar,  ద్విగుణ, ద్విగుణ)
DEF_CVT_SCALE_FUNC(8s64f,  cvt_64f, schar,  ద్విగుణ, ద్విగుణ)
DEF_CVT_SCALE_FUNC(16u64f, cvt_64f, ushort, ద్విగుణ, ద్విగుణ)
DEF_CVT_SCALE_FUNC(16s64f, cvt_64f, పొట్టి,  ద్విగుణ, ద్విగుణ)
DEF_CVT_SCALE_FUNC(32u64f, cvt_64f, గుర్తులేని, ద్విగుణ, ద్విగుణ)
DEF_CVT_SCALE_FUNC(32s64f, cvt_64f, పూర్ణాంకం,    ద్విగుణ, ద్విగుణ)
DEF_CVT_SCALE_FUNC(32f64f, cvt_64f, దశాంశం,  ద్విగుణ, ద్విగుణ)
DEF_CVT_SCALE_FUNC(64f,    cvt_64f, ద్విగుణ, ద్విగుణ, ద్విగుణ)
DEF_CVT_SCALE_FUNC(64u64f, cvt_64f, uint64_t, ద్విగుణ, ద్విగుణ)
DEF_CVT_SCALE_FUNC(64s64f, cvt_64f, int64_t, ద్విగుణ, ద్విగుణ)
DEF_CVT_SCALE_FUNC(16f64f, cvt_64f, hfloat, ద్విగుణ, ద్విగుణ)
DEF_CVT_SCALE_FUNC(16bf64f, cvt_64f, bfloat, ద్విగుణ, ద్విగుణ)

DEF_CVT_SCALE_FUNC(8u64u,  cvt_64f, uchar,  uint64_t, ద్విగుణ)
DEF_CVT_SCALE_FUNC(8s64u,  cvt_64f, schar,  uint64_t, ద్విగుణ)
DEF_CVT_SCALE_FUNC(16u64u, cvt_64f, ushort, uint64_t, ద్విగుణ)
DEF_CVT_SCALE_FUNC(16s64u, cvt_64f, పొట్టి,  uint64_t, ద్విగుణ)
DEF_CVT_SCALE_FUNC(32u64u, cvt_64f, గుర్తులేని, uint64_t, ద్విగుణ)
DEF_CVT_SCALE_FUNC(32s64u, cvt_64f, పూర్ణాంకం,    uint64_t, ద్విగుణ)
DEF_CVT_SCALE_FUNC(32f64u, cvt_64f, దశాంశం,  uint64_t, ద్విగుణ)
DEF_CVT_SCALE_FUNC(64f64u, cvt_64f, ద్విగుణ, uint64_t, ద్విగుణ)
DEF_CVT_SCALE_FUNC(64u, cvt_64f, uint64_t, uint64_t, ద్విగుణ)
DEF_CVT_SCALE_FUNC(64s64u, cvt_64f, int64_t, uint64_t, ద్విగుణ)
DEF_CVT_SCALE_FUNC(16f64u, cvt_64f, hfloat, uint64_t, ద్విగుణ)
DEF_CVT_SCALE_FUNC(16bf64u, cvt_64f, bfloat, uint64_t, ద్విగుణ)

DEF_CVT_SCALE_FUNC(8u64s,  cvt_64f, uchar,  int64_t, ద్విగుణ)
DEF_CVT_SCALE_FUNC(8s64s,  cvt_64f, schar,  int64_t, ద్విగుణ)
DEF_CVT_SCALE_FUNC(16u64s, cvt_64f, ushort, int64_t, ద్విగుణ)
DEF_CVT_SCALE_FUNC(16s64s, cvt_64f, పొట్టి,  int64_t, ద్విగుణ)
DEF_CVT_SCALE_FUNC(32u64s, cvt_64f, గుర్తులేని, int64_t, ద్విగుణ)
DEF_CVT_SCALE_FUNC(32s64s, cvt_64f, పూర్ణాంకం,    int64_t, ద్విగుణ)
DEF_CVT_SCALE_FUNC(32f64s, cvt_64f, దశాంశం,  int64_t, ద్విగుణ)
DEF_CVT_SCALE_FUNC(64f64s, cvt_64f, ద్విగుణ, int64_t, ద్విగుణ)
DEF_CVT_SCALE_FUNC(64u64s, cvt_64f, uint64_t, int64_t, ద్విగుణ)
DEF_CVT_SCALE_FUNC(64s, cvt_64f, int64_t, int64_t, ద్విగుణ)
DEF_CVT_SCALE_FUNC(16f64s, cvt_64f, hfloat, int64_t, ద్విగుణ)
DEF_CVT_SCALE_FUNC(16bf64s, cvt_64f, bfloat, int64_t, ద్విగుణ)

DEF_CVT_SCALE_FUNC(8u16f,  cvt1_32f, uchar,  hfloat, దశాంశం)
DEF_CVT_SCALE_FUNC(8s16f,  cvt1_32f, schar,  hfloat, దశాంశం)
DEF_CVT_SCALE_FUNC(16u16f, cvt1_32f, ushort, hfloat, దశాంశం)
DEF_CVT_SCALE_FUNC(16s16f, cvt1_32f, పొట్టి,  hfloat, దశాంశం)
DEF_CVT_SCALE_FUNC(32u16f, cvt1_32f, గుర్తులేని, hfloat, దశాంశం)
DEF_CVT_SCALE_FUNC(32s16f, cvt1_32f, పూర్ణాంకం,    hfloat, దశాంశం)
DEF_CVT_SCALE_FUNC(32f16f, cvt1_32f, దశాంశం,  hfloat, దశాంశం)
DEF_CVT_SCALE_FUNC(64f16f, cvt1_32f, ద్విగుణ, hfloat, దశాంశం)
DEF_CVT_SCALE_FUNC(64u16f, cvt1_32f, uint64_t, hfloat, దశాంశం)
DEF_CVT_SCALE_FUNC(64s16f, cvt1_32f, int64_t, hfloat, దశాంశం)
DEF_CVT_SCALE_FUNC(16f,    cvt1_32f, hfloat, hfloat, దశాంశం)
DEF_CVT_SCALE_FUNC(16bf16f, cvt1_32f, bfloat, hfloat, దశాంశం)

DEF_CVT_SCALE_FUNC(8u16bf,  cvt1_32f, uchar,  bfloat, దశాంశం)
DEF_CVT_SCALE_FUNC(8s16bf,  cvt1_32f, schar,  bfloat, దశాంశం)
DEF_CVT_SCALE_FUNC(16u16bf, cvt1_32f, ushort, bfloat, దశాంశం)
DEF_CVT_SCALE_FUNC(16s16bf, cvt1_32f, పొట్టి,  bfloat, దశాంశం)
DEF_CVT_SCALE_FUNC(32u16bf, cvt1_32f, గుర్తులేని, bfloat, దశాంశం)
DEF_CVT_SCALE_FUNC(32s16bf, cvt1_32f, పూర్ణాంకం,    bfloat, దశాంశం)
DEF_CVT_SCALE_FUNC(32f16bf, cvt1_32f, దశాంశం,  bfloat, దశాంశం)
DEF_CVT_SCALE_FUNC(64f16bf, cvt1_32f, ద్విగుణ, bfloat, దశాంశం)
DEF_CVT_SCALE_FUNC(64u16bf, cvt1_32f, uint64_t, bfloat, దశాంశం)
DEF_CVT_SCALE_FUNC(64s16bf, cvt1_32f, int64_t, bfloat, దశాంశం)
DEF_CVT_SCALE_FUNC(16f16bf, cvt1_32f, hfloat, bfloat, దశాంశం)
DEF_CVT_SCALE_FUNC(16bf, cvt1_32f, bfloat, bfloat, దశాంశం)

DEF_CVT_SCALEBOOL2_FUNC(8b8u, uchar, దశాంశం)
DEF_CVT_SCALEBOOL2_FUNC(8b8s, schar, దశాంశం)
DEF_CVT_SCALEBOOL2_FUNC(8b, తార్కిక, దశాంశం)
DEF_CVT_SCALEBOOL2_FUNC(8b16u, ushort, దశాంశం)
DEF_CVT_SCALEBOOL2_FUNC(8b16s, పొట్టి, దశాంశం)
DEF_CVT_SCALEBOOL2_FUNC(8b32u, గుర్తులేని, దశాంశం)
DEF_CVT_SCALEBOOL2_FUNC(8b32s, పూర్ణాంకం, దశాంశం)
DEF_CVT_SCALEBOOL2_FUNC(8b32f, దశాంశం, దశాంశం)
DEF_CVT_SCALEBOOL2_FUNC(8b64u, uint64_t, ద్విగుణ)
DEF_CVT_SCALEBOOL2_FUNC(8b64s, int64_t, ద్విగుణ)
DEF_CVT_SCALEBOOL2_FUNC(8b64f, ద్విగుణ, ద్విగుణ)
DEF_CVT_SCALEBOOL2_FUNC(8b16f, hfloat, దశాంశం)
DEF_CVT_SCALEBOOL2_FUNC(8b16bf, bfloat, దశాంశం)

// FP8 src -> CV_8U abs|scale|shift (scalar)
#నిర్వచించు DEF_CVT_SCALE_ABS_SCALAR_FUNC(suffix, stype) \
స్థిర శూన్య cvtScaleAbs##suffix( స్థిరరాశి uchar* src_, size_t sstep, స్థిరరాశి uchar*, size_t, \
                                 uchar* dst, size_t dstep, కొలత size, శూన్య* scale_) \
{ \
    స్థిరరాశి stype* src = (స్థిరరాశి stype*)src_; \
    స్థిరరాశి ద్విగుణ* scale = (స్థిరరాశి ద్విగుణ*)scale_; \
    దశాంశం a = (దశాంశం)scale[0], b = (దశాంశం)scale[1]; \
    sstep /= పరిమాణం(src[0]); \
    కొరకు( పూర్ణాంకం i = 0; i < size.ఎత్తు; i++, src += sstep, dst += dstep ) \
        కొరకు (పూర్ణాంకం j = 0; j < size.వెడల్పు; j++) \
            dst[j] = saturate_cast<uchar>(std::నిరపేక్ష((దశాంశం)src[j]*a + b)); \
}
DEF_CVT_SCALE_ABS_SCALAR_FUNC(8fe4m3u8u, fp8a_t)
DEF_CVT_SCALE_ABS_SCALAR_FUNC(8fe4m38u,  fp8_t)

ద్విమాన_Func getCvtScaleAbsFunc(పూర్ణాంకం లోతు)
{
    ద్విమాన_Func func =
        లోతు == CV_8F_E4M3FN ? (ద్విమాన_Func)cvtScaleAbs8fe4m38u :
        లోతు == CV_8F_E4M3FNUZ ? (ద్విమాన_Func)cvtScaleAbs8fe4m3u8u :
        లోతు == CV_8U ? (ద్విమాన_Func)cvtScaleAbs8u :
        లోతు == CV_8S ? (ద్విమాన_Func)cvtScaleAbs8s8u :
        లోతు == CV_Bool ? (ద్విమాన_Func)cvtScaleAbs8b8u :
        లోతు == CV_16U ? (ద్విమాన_Func)cvtScaleAbs16u8u :
        లోతు == CV_16S ? (ద్విమాన_Func)cvtScaleAbs16s8u :
        లోతు == CV_16F ? (ద్విమాన_Func)cvtScaleAbs16f8u :
        లోతు == CV_16BF ? (ద్విమాన_Func)cvtScaleAbs16bf8u :
        లోతు == CV_32U ? (ద్విమాన_Func)cvtScaleAbs32u8u :
        లోతు == CV_32S ? (ద్విమాన_Func)cvtScaleAbs32s8u :
        లోతు == CV_32F ? (ద్విమాన_Func)cvtScaleAbs32f8u :
        లోతు == CV_64U ? (ద్విమాన_Func)cvtScaleAbs64u8u :
        లోతు == CV_64S ? (ద్విమాన_Func)cvtScaleAbs64s8u :
        లోతు == CV_64F ? (ద్విమాన_Func)cvtScaleAbs64f8u : 0;
    CV_Assert(func != 0);
    తిరిగి func;
}

//////////////////// FP8 scaled conversions — scalar via saturate_cast(src*a+b) ////////////////////
#నిర్వచించు DEF_CVT_SCALE_SCALAR_FUNC(suffix, stype, dtype, wtype) \
స్థిర శూన్య cvtScale##suffix( స్థిరరాశి uchar* src_, size_t sstep, స్థిరరాశి uchar*, size_t, \
                              uchar* dst_, size_t dstep, కొలత size, శూన్య* scale_) \
{ \
    స్థిరరాశి stype* src = (స్థిరరాశి stype*)src_; dtype* dst = (dtype*)dst_; \
    స్థిరరాశి ద్విగుణ* scale = (స్థిరరాశి ద్విగుణ*)scale_; \
    wtype a = (wtype)scale[0], b = (wtype)scale[1]; \
    sstep /= పరిమాణం(src[0]); dstep /= పరిమాణం(dst[0]); \
    కొరకు( పూర్ణాంకం i = 0; i < size.ఎత్తు; i++, src += sstep, dst += dstep ) \
        కొరకు (పూర్ణాంకం j = 0; j < size.వెడల్పు; j++) \
            dst[j] = saturate_cast<dtype>((wtype)src[j]*a + b); \
}

// double intermediate so coarse FP8 grids round identically to the (double-precision) reference
#నిర్వచించు DEF_CVT_SCALE_FP8(S, T) \
    DEF_CVT_SCALE_SCALAR_FUNC(S##8u,   T, uchar, ద్విగుణ)     DEF_CVT_SCALE_SCALAR_FUNC(8u##S,   uchar, T, ద్విగుణ) \
    DEF_CVT_SCALE_SCALAR_FUNC(S##8s,   T, schar, ద్విగుణ)     DEF_CVT_SCALE_SCALAR_FUNC(8s##S,   schar, T, ద్విగుణ) \
    DEF_CVT_SCALE_SCALAR_FUNC(S##16u,  T, ushort, ద్విగుణ)    DEF_CVT_SCALE_SCALAR_FUNC(16u##S,  ushort, T, ద్విగుణ) \
    DEF_CVT_SCALE_SCALAR_FUNC(S##16s,  T, పొట్టి, ద్విగుణ)     DEF_CVT_SCALE_SCALAR_FUNC(16s##S,  పొట్టి, T, ద్విగుణ) \
    DEF_CVT_SCALE_SCALAR_FUNC(S##32u,  T, గుర్తులేని, ద్విగుణ)  DEF_CVT_SCALE_SCALAR_FUNC(32u##S,  గుర్తులేని, T, ద్విగుణ) \
    DEF_CVT_SCALE_SCALAR_FUNC(S##32s,  T, పూర్ణాంకం, ద్విగుణ)       DEF_CVT_SCALE_SCALAR_FUNC(32s##S,  పూర్ణాంకం, T, ద్విగుణ) \
    DEF_CVT_SCALE_SCALAR_FUNC(S##32f,  T, దశాంశం, ద్విగుణ) \
    DEF_CVT_SCALE_SCALAR_FUNC(S##64f,  T, ద్విగుణ, ద్విగుణ)    DEF_CVT_SCALE_SCALAR_FUNC(64f##S,  ద్విగుణ, T, ద్విగుణ) \
    DEF_CVT_SCALE_SCALAR_FUNC(S##16f,  T, hfloat, ద్విగుణ)    DEF_CVT_SCALE_SCALAR_FUNC(16f##S,  hfloat, T, ద్విగుణ) \
    DEF_CVT_SCALE_SCALAR_FUNC(S##16bf, T, bfloat, ద్విగుణ)    DEF_CVT_SCALE_SCALAR_FUNC(16bf##S, bfloat, T, ద్విగుణ) \
    DEF_CVT_SCALE_SCALAR_FUNC(S##64u,  T, uint64_t, ద్విగుణ)  DEF_CVT_SCALE_SCALAR_FUNC(64u##S,  uint64_t, T, ద్విగుణ) \
    DEF_CVT_SCALE_SCALAR_FUNC(S##64s,  T, int64_t, ద్విగుణ)   DEF_CVT_SCALE_SCALAR_FUNC(64s##S,  int64_t, T, ద్విగుణ) \
    DEF_CVT_SCALE_SCALAR_FUNC(S##8b,   T, తార్కిక, ద్విగుణ) \
    DEF_CVT_SCALE_SCALAR_FUNC(S##8fe4m3,  T, fp8_t, ద్విగుణ)   DEF_CVT_SCALE_SCALAR_FUNC(S##8fe4m3u, T, fp8a_t, ద్విగుణ)

DEF_CVT_SCALE_FP8(8fe4m3,  fp8_t)
DEF_CVT_SCALE_FP8(8fe4m3u, fp8a_t)

// convertTo(dst, depth, alpha, beta) path -- real quantization. Reuses convert.hpp's fast path.
// Double precision here matches the scalar reference (wtype)x*a+b bit-for-bit.
మూస<పూర్ణాంకం bias, తార్కిక fnuz> స్థిర శూన్య
cvt_Scale_F32_గమ్యం_Fp8(స్థిరరాశి uchar* src_, size_t sstep, స్థిరరాశి uchar*, size_t,
                 uchar* dst, size_t dstep, కొలత size, శూన్య* scale_)
{
    CV_INSTRUMENT_REGION();
    స్థిరరాశి దశాంశం* src = (స్థిరరాశి దశాంశం*)src_;
    స్థిరరాశి ద్విగుణ* scale = (స్థిరరాశి ద్విగుణ*)scale_;
    ద్విగుణ a = scale[0], b = scale[1];
    sstep /= పరిమాణం(src[0]);

    కొరకు (పూర్ణాంకం i = 0; i < size.ఎత్తు; i++, src += sstep, dst += dstep)
    {
        పూర్ణాంకం j = 0;
#ఒకవేళ (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)
        v_float64 va = vx_setall_f64(a), vb = vx_setall_f64(b);
        స్థిరరాశి పూర్ణాంకం LANES32 = VTraits<v_float32>::vlanes();
        స్థిరరాశి పూర్ణాంకం VECSZ = LANES32 * 4;
        దశాంశం scaled[VTraits<v_float32>::max_nlanes * 4];
        కొరకు (; j <= size.వెడల్పు - VECSZ; j += VECSZ)
        {
            v_float32 vf0, vf1, vf2, vf3;
            v_float64 d0, d1;

            vx_ఎక్కించు_pair_as(src + j + 0 * LANES32, d0, d1);
            d0 = v_కూడు(v_mul(d0, va), vb);
            d1 = v_కూడు(v_mul(d1, va), vb);
            vf0 = v_cvt_f32(d0, d1);
            v_store(scaled + 0 * LANES32, vf0);

            vx_ఎక్కించు_pair_as(src + j + 1 * LANES32, d0, d1);
            d0 = v_కూడు(v_mul(d0, va), vb);
            d1 = v_కూడు(v_mul(d1, va), vb);
            vf1 = v_cvt_f32(d0, d1);
            v_store(scaled + 1 * LANES32, vf1);

            vx_ఎక్కించు_pair_as(src + j + 2 * LANES32, d0, d1);
            d0 = v_కూడు(v_mul(d0, va), vb);
            d1 = v_కూడు(v_mul(d1, va), vb);
            vf2 = v_cvt_f32(d0, d1);
            v_store(scaled + 2 * LANES32, vf2);

            vx_ఎక్కించు_pair_as(src + j + 3 * LANES32, d0, d1);
            d0 = v_కూడు(v_mul(d0, va), vb);
            d1 = v_కూడు(v_mul(d1, va), vb);
            vf3 = v_cvt_f32(d0, d1);
            v_store(scaled + 3 * LANES32, vf3);

            encodeFp8Vec4<bias, fnuz>(vf0, vf1, vf2, vf3, scaled, VECSZ, dst + j);
        }
        vx_cleanup();
#అయితే_ముగింపు
        కొరకు (; j < size.వెడల్పు; j++)
            dst[j] = fp8_detail::encodeE4M3((దశాంశం)((ద్విగుణ)src[j]*a + b), bias, fnuz);
    }
}
స్థిర శూన్య cvtScale32f8fe4m3(స్థిరరాశి uchar* s, size_t ss, స్థిరరాశి uchar* p, size_t ps, uchar* d, size_t ds, కొలత sz, శూన్య* x)
{ cvt_Scale_F32_గమ్యం_Fp8<7, అబద్ధం>(s, ss, p, ps, d, ds, sz, x); }
స్థిర శూన్య cvtScale32f8fe4m3u(స్థిరరాశి uchar* s, size_t ss, స్థిరరాశి uchar* p, size_t ps, uchar* d, size_t ds, కొలత sz, శూన్య* x)
{ cvt_Scale_F32_గమ్యం_Fp8<8, నిజం>(s, ss, p, ps, d, ds, sz, x); }

ద్విమాన_Func getConvertScaleFunc(పూర్ణాంకం sdepth_, పూర్ణాంకం ddepth_)
{
    పూర్ణాంకం sdepth = CV_MAT_లోతు(sdepth_);
    పూర్ణాంకం ddepth = CV_MAT_లోతు(ddepth_);
    ద్విమాన_Func func =
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
    తిరిగి func;
}

#అయితే_ముగింపు

CV_CPU_OPTIMIZATION_NAMESPACE_చివర
} // namespace
