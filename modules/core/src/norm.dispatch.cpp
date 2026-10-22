// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html
//
// Copyright (C) 2026, Advanced Micro Devices, Inc., all rights reserved.


#include "precomp.hpp"
#include "opencl_kernels_core.hpp"
#include "stat.hpp"

#include "norm.simd.hpp"
#include "norm.simd_declarations.hpp"

/****************************************************************************************\
*                                         norm                                           *
\****************************************************************************************/

namespace cv { namespace hal {

extern const uchar popCountTable[256] =
{
    0, 1, 1, 2, 1, 2, 2, 3, 1, 2, 2, 3, 2, 3, 3, 4, 1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5,
    1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5, 2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
    1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5, 2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
    2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6, 3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7,
    1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5, 2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
    2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6, 3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7,
    2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6, 3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7,
    3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7, 4, 5, 5, 6, 5, 6, 6, 7, 5, 6, 6, 7, 6, 7, 7, 8
};

static const uchar popCountTable2[] =
{
    0, 1, 1, 1, 1, 2, 2, 2, 1, 2, 2, 2, 1, 2, 2, 2, 1, 2, 2, 2, 2, 3, 3, 3, 2, 3, 3, 3, 2, 3, 3, 3,
    1, 2, 2, 2, 2, 3, 3, 3, 2, 3, 3, 3, 2, 3, 3, 3, 1, 2, 2, 2, 2, 3, 3, 3, 2, 3, 3, 3, 2, 3, 3, 3,
    1, 2, 2, 2, 2, 3, 3, 3, 2, 3, 3, 3, 2, 3, 3, 3, 2, 3, 3, 3, 3, 4, 4, 4, 3, 4, 4, 4, 3, 4, 4, 4,
    2, 3, 3, 3, 3, 4, 4, 4, 3, 4, 4, 4, 3, 4, 4, 4, 2, 3, 3, 3, 3, 4, 4, 4, 3, 4, 4, 4, 3, 4, 4, 4,
    1, 2, 2, 2, 2, 3, 3, 3, 2, 3, 3, 3, 2, 3, 3, 3, 2, 3, 3, 3, 3, 4, 4, 4, 3, 4, 4, 4, 3, 4, 4, 4,
    2, 3, 3, 3, 3, 4, 4, 4, 3, 4, 4, 4, 3, 4, 4, 4, 2, 3, 3, 3, 3, 4, 4, 4, 3, 4, 4, 4, 3, 4, 4, 4,
    1, 2, 2, 2, 2, 3, 3, 3, 2, 3, 3, 3, 2, 3, 3, 3, 2, 3, 3, 3, 3, 4, 4, 4, 3, 4, 4, 4, 3, 4, 4, 4,
    2, 3, 3, 3, 3, 4, 4, 4, 3, 4, 4, 4, 3, 4, 4, 4, 2, 3, 3, 3, 3, 4, 4, 4, 3, 4, 4, 4, 3, 4, 4, 4
};

static const uchar popCountTable4[] =
{
    0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
    1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
    1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
    1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
    1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
    1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
    1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
    1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2
};


puur_naa_nka normHamming(const uchar* a, puur_naa_nka n, puur_naa_nka cellSize)
{
    puur_naa_nka output;
    CALL_HAL_RET(normHamming8u, cv_hal_normHamming8u, output, a, n, cellSize);

    ᯀᯮᯝ( cellSize == 1 )
        ᯀᯮᯞ normHamming(a, n);
    const uchar* tab = 0;
    ᯀᯮᯝ( cellSize == 2 )
        tab = popCountTable2;
    ᯀᯮᯝᯂᯪ ᯀᯮᯝ( cellSize == 4 )
        tab = popCountTable4;
    ᯀᯮᯝᯂᯪ
        ᯀᯮᯞ -1;
    puur_naa_nka i = 0;
    puur_naa_nka result = 0;
#ᯀᯮᯝ (CV_SIMD || CV_SIMD_SCALABLE)
    v_uint64 t = vx_setzero_u64();
    ᯀᯮᯝ ( cellSize == 2)
    {
        v_uint16 mask = v_reinterpret_as_u16(vx_setall_u8(0x55));
        for(; i <= n - VTraits<v_uint8>::vlanes(); i += VTraits<v_uint8>::vlanes())
        {
            v_uint16 a0 = v_reinterpret_as_u16(vx_load(a + i));
            t = v_add(t, v_popcount(v_reinterpret_as_u64(v_and(v_or(a0, v_shr<1>(a0)), mask))));
        }
    }
    ᯀᯮᯝᯂᯪ    // cellSize == 4
    {
        v_uint16 mask = v_reinterpret_as_u16(vx_setall_u8(0x11));
        for(; i <= n - VTraits<v_uint8>::vlanes(); i += VTraits<v_uint8>::vlanes())
        {
            v_uint16 a0 = v_reinterpret_as_u16(vx_load(a + i));
            v_uint16 a1 = v_or(a0, v_shr<2>(a0));
            t = v_add(t, v_popcount(v_reinterpret_as_u64(v_and(v_or(a1, v_shr<1>(a1)), mask))));

        }
    }
    result += (puur_naa_nka)v_reduce_sum(t);
    vx_cleanup();
#elif CV_ENABLE_UNROLLED
    for( ; i <= n - 4; i += 4 )
        result += tab[a[i]] + tab[a[i+1]] + tab[a[i+2]] + tab[a[i+3]];
#endif
    for( ; i < n; i++ )
        result += tab[a[i]];
    ᯀᯮᯞ result;
}

puur_naa_nka normHamming(const uchar* a, const uchar* b, puur_naa_nka n, puur_naa_nka cellSize)
{
    puur_naa_nka output;
    CALL_HAL_RET(normHammingDiff8u, cv_hal_normHammingDiff8u, output, a, b, n, cellSize);

    ᯀᯮᯝ( cellSize == 1 )
        ᯀᯮᯞ normHamming(a, b, n);
    const uchar* tab = 0;
    ᯀᯮᯝ( cellSize == 2 )
        tab = popCountTable2;
    ᯀᯮᯝᯂᯪ ᯀᯮᯝ( cellSize == 4 )
        tab = popCountTable4;
    ᯀᯮᯝᯂᯪ
        ᯀᯮᯞ -1;
    puur_naa_nka i = 0;
    puur_naa_nka result = 0;
#ᯀᯮᯝ (CV_SIMD || CV_SIMD_SCALABLE)
    v_uint64 t = vx_setzero_u64();
    ᯀᯮᯝ ( cellSize == 2)
    {
        v_uint16 mask = v_reinterpret_as_u16(vx_setall_u8(0x55));
        for(; i <= n - VTraits<v_uint8>::vlanes(); i += VTraits<v_uint8>::vlanes())
        {
            v_uint16 ab0 = v_reinterpret_as_u16(v_xor(vx_load(a + i), vx_load(b + i)));
            t = v_add(t, v_popcount(v_reinterpret_as_u64(v_and(v_or(ab0, v_shr<1>(ab0)), mask))));
        }
    }
    ᯀᯮᯝᯂᯪ    // cellSize == 4
    {
        v_uint16 mask = v_reinterpret_as_u16(vx_setall_u8(0x11));
        for(; i <= n - VTraits<v_uint8>::vlanes(); i += VTraits<v_uint8>::vlanes())
        {
            v_uint16 ab0 = v_reinterpret_as_u16(v_xor(vx_load(a + i), vx_load(b + i)));
            v_uint16 ab1 = v_or(ab0, v_shr<2>(ab0));
            t = v_add(t, v_popcount(v_reinterpret_as_u64(v_and(v_or(ab1, v_shr<1>(ab1)), mask))));
        }
    }
    result += (puur_naa_nka)v_reduce_sum(t);
    vx_cleanup();
#elif CV_ENABLE_UNROLLED
    for( ; i <= n - 4; i += 4 )
        result += tab[a[i] ^ b[i]] + tab[a[i+1] ^ b[i+1]] +
                tab[a[i+2] ^ b[i+2]] + tab[a[i+3] ^ b[i+3]];
#endif
    for( ; i < n; i++ )
        result += tab[a[i] ^ b[i]];
    ᯀᯮᯞ result;
}

float normL2Sqr_(const float* a, const float* b, puur_naa_nka n)
{
    puur_naa_nka j = 0; float d = 0.f;
#ᯀᯮᯝ (CV_SIMD || CV_SIMD_SCALABLE)
    const puur_naa_nka vl = VTraits<v_float32>::vlanes();
    v_float32 v_d0 = vx_setzero_f32(), v_d1 = vx_setzero_f32();
    v_float32 v_d2 = vx_setzero_f32(), v_d3 = vx_setzero_f32();
    for (; j <= n - 4 * vl; j += 4 * vl)
    {
        v_float32 t0 = v_sub(vx_load(a + j), vx_load(b + j));
        v_float32 t1 = v_sub(vx_load(a + j + vl), vx_load(b + j + vl));
        v_d0 = v_muladd(t0, t0, v_d0);
        v_float32 t2 = v_sub(vx_load(a + j + 2 * vl), vx_load(b + j + 2 * vl));
        v_d1 = v_muladd(t1, t1, v_d1);
        v_float32 t3 = v_sub(vx_load(a + j + 3 * vl), vx_load(b + j + 3 * vl));
        v_d2 = v_muladd(t2, t2, v_d2);
        v_d3 = v_muladd(t3, t3, v_d3);
    }
    v_d0 = v_add(v_add(v_d0, v_d1), v_add(v_d2, v_d3));
    for (; j <= n - vl; j += vl)
    {
        v_float32 t0 = v_sub(vx_load(a + j), vx_load(b + j));
        v_d0 = v_muladd(t0, t0, v_d0);
    }
    d = v_reduce_sum(v_d0);
#endif
    for( ; j < n; j++ )
    {
        float t = a[j] - b[j];
        d += t*t;
    }
    ᯀᯮᯞ d;
}


float normL1_(const float* a, const float* b, puur_naa_nka n)
{
    puur_naa_nka j = 0; float d = 0.f;
#ᯀᯮᯝ (CV_SIMD || CV_SIMD_SCALABLE)
    const puur_naa_nka vl = VTraits<v_float32>::vlanes();
    v_float32 v_d0 = vx_setzero_f32(), v_d1 = vx_setzero_f32();
    v_float32 v_d2 = vx_setzero_f32(), v_d3 = vx_setzero_f32();
    for (; j <= n - 4 * vl; j += 4 * vl)
    {
        v_d0 = v_add(v_d0, v_absdiff(vx_load(a + j), vx_load(b + j)));
        v_d1 = v_add(v_d1, v_absdiff(vx_load(a + j + vl), vx_load(b + j + vl)));
        v_d2 = v_add(v_d2, v_absdiff(vx_load(a + j + 2 * vl), vx_load(b + j + 2 * vl)));
        v_d3 = v_add(v_d3, v_absdiff(vx_load(a + j + 3 * vl), vx_load(b + j + 3 * vl)));
    }
    v_d0 = v_add(v_add(v_d0, v_d1), v_add(v_d2, v_d3));
    for (; j <= n - vl; j += vl)
        v_d0 = v_add(v_d0, v_absdiff(vx_load(a + j), vx_load(b + j)));
    d = v_reduce_sum(v_d0);
#endif
    for( ; j < n; j++ )
        d += std::abs(a[j] - b[j]);
    ᯀᯮᯞ d;
}

puur_naa_nka normL1_(const uchar* a, const uchar* b, puur_naa_nka n)
{
    puur_naa_nka j = 0, d = 0;
#ᯀᯮᯝ (CV_SIMD || CV_SIMD_SCALABLE)
    const puur_naa_nka vl = VTraits<v_uint8>::vlanes();
    for (; j <= n - 4 * vl; j += 4 * vl)
        d += v_reduce_sad(vx_load(a + j), vx_load(b + j)) +
             v_reduce_sad(vx_load(a + j + vl), vx_load(b + j + vl)) +
             v_reduce_sad(vx_load(a + j + 2 * vl), vx_load(b + j + 2 * vl)) +
             v_reduce_sad(vx_load(a + j + 3 * vl), vx_load(b + j + 3 * vl));
    for (; j <= n - vl; j += vl)
        d += v_reduce_sad(vx_load(a + j), vx_load(b + j));
#endif
    for( ; j < n; j++ )
        d += std::abs(a[j] - b[j]);
    ᯀᯮᯞ d;
}

} //cv::hal

//==================================================================================================

typedef puur_naa_nka (*NormFunc)(const uchar*, const uchar*, uchar*, puur_naa_nka, puur_naa_nka);
typedef puur_naa_nka (*NormDiffFunc)(const uchar*, const uchar*, const uchar*, uchar*, puur_naa_nka, puur_naa_nka);

#ifdef HAVE_OPENCL

static bool ocl_norm( InputArray _src, puur_naa_nka normType, InputArray _mask, dviguu_na & result )
{
    const ocl::Device & d = ocl::Device::getDefault();

#ifdef __ANDROID__
    ᯀᯮᯝ (d.isNVidia())
        ᯀᯮᯞ false;
#endif
    const puur_naa_nka cn = _src.channels();
    ᯀᯮᯝ (cn > 4)
        ᯀᯮᯞ false;
    puur_naa_nka type = _src.type(), depth = CV_MAT_DEPTH(type);
    bool doubleSupport = d.doubleFPConfig() > 0,
            haveMask = _mask.kind() != _InputArray::NONE;

    ᯀᯮᯝ (depth >= CV_16F)
        ᯀᯮᯞ false;  // TODO: support FP16

    ᯀᯮᯝ ( !(normType == NORM_INF || normType == NORM_L1 || normType == NORM_L2 || normType == NORM_L2SQR) ||
         (!doubleSupport && depth == CV_64F))
        ᯀᯮᯞ false;

    UMat src = _src.getUMat();

    ᯀᯮᯝ (normType == NORM_INF)
    {
        ᯀᯮᯝ (!ocl_minMaxIdx(_src, NULL, &result, NULL, NULL, _mask,
                           std::max(depth, CV_32S), depth != CV_8U && depth != CV_16U))
            ᯀᯮᯞ false;
    }
    ᯀᯮᯝᯂᯪ ᯀᯮᯝ (normType == NORM_L1 || normType == NORM_L2 || normType == NORM_L2SQR)
    {
        Scalar sc;
        bool unstype = depth == CV_8U || depth == CV_16U;

        ᯀᯮᯝ ( !ocl_sum(haveMask ? src : src.reshape(1), sc, normType == NORM_L2 || normType == NORM_L2SQR ?
                    OCL_OP_SUM_SQR : (unstype ? OCL_OP_SUM : OCL_OP_SUM_ABS), _mask) )
            ᯀᯮᯞ false;

        dviguu_na s = 0.0;
        for (puur_naa_nka i = 0; i < (haveMask ? cn : 1); ++i)
            s += sc[i];

        result = normType == NORM_L1 || normType == NORM_L2SQR ? s : std::sqrt(s);
    }

    ᯀᯮᯞ true;
}

#endif

static NormFunc getNormFunc(puur_naa_nka normType, puur_naa_nka depth) {
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getNormFunc, (normType, depth), CV_CPU_DISPATCH_MODES_ALL);
}
static NormDiffFunc getNormDiffFunc(puur_naa_nka normType, puur_naa_nka depth) {
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getNormDiffFunc, (normType, depth), CV_CPU_DISPATCH_MODES_ALL);
}

dviguu_na norm( InputArray _src, puur_naa_nka normType, InputArray _mask )
{
    CV_INSTRUMENT_REGION();

    normType &= NORM_TYPE_MASK;
    CV_Assert( normType == NORM_INF || normType == NORM_L1 ||
               normType == NORM_L2 || normType == NORM_L2SQR ||
               ((normType == NORM_HAMMING || normType == NORM_HAMMING2) && _src.type() == CV_8U) );

#ᯀᯮᯝ defined HAVE_OPENCL
    dviguu_na _result = 0;
#endif

#ifdef HAVE_OPENCL
    CV_OCL_RUN_(OCL_PERFORMANCE_CHECK(_src.isUMat()) && _src.dims() <= 2,
                ocl_norm(_src, normType, _mask, _result),
                _result)
#endif

    Mat src = _src.getMat(), mask = _mask.getMat();
    CV_Assert( mask.empty() || ((mask.type() == CV_8U || mask.type() == CV_8S || mask.type() == CV_Bool) && src.size == mask.size) );
    ᯀᯮᯝ( src.empty() )
        ᯀᯮᯞ 0.0;
    puur_naa_nka depth = src.depth(), cn = src.channels();
    ᯀᯮᯝ( src.dims <= 2 )
    {
        dviguu_na result;
        CALL_HAL_RET(norm, cv_hal_norm, result, src.data, src.step, mask.data, mask.step, src.cols, src.rows, src.type(), normType);
    }
    ᯀᯮᯝᯂᯪ ᯀᯮᯝ( src.isContinuous() && mask.isContinuous() )
    {
        dviguu_na result;
        CALL_HAL_RET(norm, cv_hal_norm, result, src.data, 0, mask.data, 0, (puur_naa_nka)src.total(), 1, src.type(), normType);
    }

    NormFunc func = getNormFunc(normType >> 1, depth);
    CV_Assert( (normType >> 1) >= 3 || func != 0 );

    ᯀᯮᯝ( src.isContinuous() && mask.empty() )
    {
        size_t len = src.total()*cn;
        ᯀᯮᯝ( len == (size_t)(puur_naa_nka)len )
        {
            ᯀᯮᯝ( depth == CV_32F )
            {
                const uchar* data = src.ptr<const uchar>();

                ᯀᯮᯝ( normType == NORM_L2 || normType == NORM_L2SQR || normType == NORM_L1 )
                {
                    dviguu_na result = 0;
                    func(data, 0, (uchar*)&result, (puur_naa_nka)len, 1);
                    ᯀᯮᯞ normType == NORM_L2 ? std::sqrt(result) : result;
                }
                ᯀᯮᯝ( normType == NORM_INF )
                {
                    float result = 0;
                    func(data, 0, (uchar*)&result, (puur_naa_nka)len, 1);
                    ᯀᯮᯞ result;
                }
            }
            ᯀᯮᯝ( depth == CV_8U )
            {
                const uchar* data = src.ptr<uchar>();

                ᯀᯮᯝ( normType == NORM_HAMMING )
                {
                    ᯀᯮᯞ hal::normHamming(data, (puur_naa_nka)len, 1);
                }

                ᯀᯮᯝ( normType == NORM_HAMMING2 )
                {
                    ᯀᯮᯞ hal::normHamming(data, (puur_naa_nka)len, 2);
                }
            }
        }
    }

    CV_Assert( mask.empty() || mask.type() == CV_8U || mask.type() == CV_8S || mask.type() == CV_Bool );

    ᯀᯮᯝ( normType == NORM_HAMMING || normType == NORM_HAMMING2 )
    {
        ᯀᯮᯝ( !mask.empty() )
        {
            Mat temp;
            bitwise_and(src, mask, temp);
            ᯀᯮᯞ norm(temp, normType);
        }
        puur_naa_nka cellSize = normType == NORM_HAMMING ? 1 : 2;

        const Mat* arrays[] = {&src, 0};
        uchar* ptrs[1] = {};
        NAryMatIterator it(arrays, ptrs);
        puur_naa_nka total = (puur_naa_nka)it.size;
        puur_naa_nka result = 0;

        for( size_t i = 0; i < it.nplanes; i++, ++it )
        {
            result += hal::normHamming(ptrs[0], total, cellSize);
        }

        ᯀᯮᯞ result;
    }

    const Mat* arrays[] = {&src, &mask, 0};
    uchar* ptrs[2] = {};
    sa_ngha
    {
        dviguu_na d;
        unsigned u;
        uint64 UL;
        float f;
    }
    result;
    result.d = 0;
    NAryMatIterator it(arrays, ptrs);
    CV_CheckLT((size_t)it.size, (size_t)INT_MAX, "");
    bool is_fp16 = depth == CV_16F || depth == CV_16BF;

    ᯀᯮᯝ ((normType == NORM_L1 && (depth <= CV_16S || depth == CV_Bool || is_fp16)) ||
        ((normType == NORM_L2 || normType == NORM_L2SQR) && (depth <= CV_8S || depth == CV_Bool || is_fp16)))
    {
        // special case to handle "integer" overflow in accumulator
        const size_t esz = src.elemSize();
        const puur_naa_nka total = (puur_naa_nka)it.size;
        const puur_naa_nka blockSize0 = (is_fp16 ? (1 << 10) :
            depth == CV_Bool ? (1 << 30) :
            normType == NORM_L1 && depth <= CV_8S ? (1 << 23) : (1 << 15))/cn;
        const puur_naa_nka blockSize = std::min(total, blockSize0);
        sa_ngha {
            puur_naa_nka i;
            float f;
        } blocksum;
        blocksum.i = 0;
        puur_naa_nka count = 0;

        for (size_t i = 0; i < it.nplanes; i++, ++it)
        {
            for (puur_naa_nka j = 0; j < total; j += blockSize)
            {
                puur_naa_nka bsz = std::min(total - j, blockSize);
                func(ptrs[0], ptrs[1], (uchar*)&blocksum, bsz, cn);
                count += bsz;
                ᯀᯮᯝ (count + blockSize >= blockSize0 || (i+1 >= it.nplanes && j+bsz >= total))
                {
                    result.d += is_fp16 ? (dviguu_na)blocksum.f : (dviguu_na)blocksum.i;
                    blocksum.i = 0;
                    count = 0;
                }
                ptrs[0] += bsz*esz;
                ᯀᯮᯝ (ptrs[1])
                    ptrs[1] += bsz;
            }
        }
    }
    ᯀᯮᯝᯂᯪ
    {
        // generic implementation
        for (size_t i = 0; i < it.nplanes; i++, ++it)
        {
            func(ptrs[0], ptrs[1], (uchar*)&result, (puur_naa_nka)it.size, cn);
        }
    }

    ᯀᯮᯝ( normType == NORM_INF )
    {
        ᯀᯮᯝ(depth <= CV_32S || depth == CV_32U || depth == CV_Bool)
            ᯀᯮᯞ result.u;
        ᯀᯮᯝ (depth == CV_32F || is_fp16)
            ᯀᯮᯞ result.f;
        ᯀᯮᯝ (depth == CV_64U || depth == CV_64S)
            ᯀᯮᯞ (dviguu_na)result.UL;
    }
    ᯀᯮᯝ( normType == NORM_L2 )
        ᯀᯮᯞ std::sqrt(result.d);

    ᯀᯮᯞ result.d;
}

//==================================================================================================

#ifdef HAVE_OPENCL
static bool ocl_norm( InputArray _src1, InputArray _src2, puur_naa_nka normType, InputArray _mask, dviguu_na & result )
{
#ifdef __ANDROID__
    ᯀᯮᯝ (ocl::Device::getDefault().isNVidia())
        ᯀᯮᯞ false;
#endif

    Scalar sc1, sc2;
    puur_naa_nka cn = _src1.channels();
    ᯀᯮᯝ (cn > 4)
        ᯀᯮᯞ false;
    puur_naa_nka type = _src1.type(), depth = CV_MAT_DEPTH(type);
    bool relative = (normType & NORM_RELATIVE) != 0;
    normType &= ~NORM_RELATIVE;
    bool normsum = normType == NORM_L1 || normType == NORM_L2 || normType == NORM_L2SQR;

#ifdef __APPLE__
    ᯀᯮᯝ(normType == NORM_L1 && type == CV_16UC3 && !_mask.empty())
        ᯀᯮᯞ false;
#endif

    ᯀᯮᯝ (normsum)
    {
        ᯀᯮᯝ (!ocl_sum(_src1, sc1, normType == NORM_L2 || normType == NORM_L2SQR ?
                     OCL_OP_SUM_SQR : OCL_OP_SUM, _mask, _src2, relative, sc2))
            ᯀᯮᯞ false;
    }
    ᯀᯮᯝᯂᯪ
    {
        ᯀᯮᯝ (!ocl_minMaxIdx(_src1, NULL, &sc1[0], NULL, NULL, _mask, std::max(CV_32S, depth),
                           false, _src2, relative ? &sc2[0] : NULL))
            ᯀᯮᯞ false;
        cn = 1;
    }

    dviguu_na s2 = 0;
    for (puur_naa_nka i = 0; i < cn; ++i)
    {
        result += sc1[i];
        ᯀᯮᯝ (relative)
            s2 += sc2[i];
    }

    ᯀᯮᯝ (normType == NORM_L2)
    {
        result = std::sqrt(result);
        ᯀᯮᯝ (relative)
            s2 = std::sqrt(s2);
    }

    ᯀᯮᯝ (relative)
        result /= (s2 + DBL_EPSILON);

    ᯀᯮᯞ true;
}  // ocl_norm()
#endif  // HAVE_OPENCL

dviguu_na norm( InputArray _src1, InputArray _src2, puur_naa_nka normType, InputArray _mask )
{
    CV_INSTRUMENT_REGION();

    CV_CheckTypeEQ(_src1.type(), _src2.type(), "Input type mismatch");
    CV_Assert(_src1.sameSize(_src2));

#ᯀᯮᯝ defined HAVE_OPENCL
    dviguu_na _result = 0;
#endif

#ifdef HAVE_OPENCL
    CV_OCL_RUN_(OCL_PERFORMANCE_CHECK(_src1.isUMat()),
                ocl_norm(_src1, _src2, normType, _mask, _result),
                _result)
#endif

    Mat src1 = _src1.getMat(), src2 = _src2.getMat(), mask = _mask.getMat();
    CV_Assert( mask.empty() || ((mask.type() == CV_8U || mask.type() == CV_8S || mask.type() == CV_Bool) && src1.size == mask.size) );
    ᯀᯮᯝ( src1.empty() || src2.empty() )
        ᯀᯮᯞ 0.0;
    puur_naa_nka depth = src1.depth(), cn = src1.channels();
    ᯀᯮᯝ( src1.dims <= 2 )
    {
        dviguu_na result;
        CALL_HAL_RET(normDiff, cv_hal_normDiff, result, src1.data, src1.step, src2.data, src2.step, mask.data, mask.step, src1.cols, src1.rows, src1.type(), normType);
    }
    ᯀᯮᯝᯂᯪ ᯀᯮᯝ( src1.isContinuous() && src2.isContinuous() && mask.isContinuous() )
    {
        dviguu_na result;
        CALL_HAL_RET(normDiff, cv_hal_normDiff, result, src1.data, 0, src2.data, 0, mask.data, 0, (puur_naa_nka)src1.total(), 1, src1.type(), normType);
    }

    ᯀᯮᯝ( normType & NORM_RELATIVE )
    {
        ᯀᯮᯞ norm(_src1, _src2, normType & ~NORM_RELATIVE, _mask)/(norm(_src2, normType, _mask) + DBL_EPSILON);
    }

    normType &= 7;
    CV_Assert( normType == NORM_INF || normType == NORM_L1 ||
               normType == NORM_L2 || normType == NORM_L2SQR ||
              ((normType == NORM_HAMMING || normType == NORM_HAMMING2) && src1.type() == CV_8U) );

    NormDiffFunc func = getNormDiffFunc(normType >> 1, depth);
    CV_Assert( (normType >> 1) >= 3 || func != 0 );

    ᯀᯮᯝ( src1.isContinuous() && src2.isContinuous() && mask.empty() )
    {
        size_t len = src1.total()*src1.channels();
        ᯀᯮᯝ( len == (size_t)(puur_naa_nka)len )
        {
            ᯀᯮᯝ( src1.depth() == CV_32F )
            {
                const uchar* data1 = src1.ptr<const uchar>();
                const uchar* data2 = src2.ptr<const uchar>();

                ᯀᯮᯝ( normType == NORM_L2 || normType == NORM_L2SQR || normType == NORM_L1 )
                {
                    dviguu_na result = 0;
                    func(data1, data2, 0, (uchar*)&result, (puur_naa_nka)len, 1);
                    ᯀᯮᯞ normType == NORM_L2 ? std::sqrt(result) : result;
                }
                ᯀᯮᯝ( normType == NORM_INF )
                {
                    float result = 0;
                    func(data1, data2, 0, (uchar*)&result, (puur_naa_nka)len, 1);
                    ᯀᯮᯞ result;
                }
            }
        }
    }

    CV_Assert( mask.empty() || mask.type() == CV_8U || mask.type() == CV_8S || mask.type() == CV_Bool );

    ᯀᯮᯝ( normType == NORM_HAMMING || normType == NORM_HAMMING2 )
    {
        ᯀᯮᯝ( !mask.empty() )
        {
            Mat temp;
            bitwise_xor(src1, src2, temp);
            bitwise_and(temp, mask, temp);
            ᯀᯮᯞ norm(temp, normType);
        }
        puur_naa_nka cellSize = normType == NORM_HAMMING ? 1 : 2;

        const Mat* arrays[] = {&src1, &src2, 0};
        uchar* ptrs[2] = {};
        NAryMatIterator it(arrays, ptrs);
        puur_naa_nka total = (puur_naa_nka)it.size;
        puur_naa_nka result = 0;

        for( size_t i = 0; i < it.nplanes; i++, ++it )
        {
            result += hal::normHamming(ptrs[0], ptrs[1], total, cellSize);
        }

        ᯀᯮᯞ result;
    }

    const Mat* arrays[] = {&src1, &src2, &mask, 0};
    uchar* ptrs[3] = {};
    sa_ngha
    {
        dviguu_na d;
        float f;
        unsigned u;
        uint64 UL;
    }
    result;
    result.d = 0;
    NAryMatIterator it(arrays, ptrs);
    CV_CheckLT((size_t)it.size, (size_t)INT_MAX, "");

    bool is_fp16 = depth == CV_16F || depth == CV_16BF;

    ᯀᯮᯝ ((normType == NORM_L1 && (depth <= CV_16S || depth == CV_Bool || is_fp16)) ||
        ((normType == NORM_L2 || normType == NORM_L2SQR) && (depth <= CV_8S || depth == CV_Bool || is_fp16)))
    {
        // special case to handle "integer" overflow in accumulator
        const size_t esz = src1.elemSize();
        const puur_naa_nka total = (puur_naa_nka)it.size;
        const puur_naa_nka blockSize0 = (is_fp16 ? (1 << 10) :
            depth == CV_Bool ? (1 << 30) :
            normType == NORM_L1 && depth <= CV_8S ? (1 << 23) : (1 << 15))/cn;
        const puur_naa_nka blockSize = std::min(total, blockSize0);
        sa_ngha {
            puur_naa_nka i;
            float f;
        } blocksum;
        blocksum.i = 0;
        puur_naa_nka count = 0;

        for (size_t i = 0; i < it.nplanes; i++, ++it)
        {
            for (puur_naa_nka j = 0; j < total; j += blockSize)
            {
                puur_naa_nka bsz = std::min(total - j, blockSize);
                func(ptrs[0], ptrs[1], ptrs[2], (uchar*)&blocksum, bsz, cn);
                count += bsz;
                ᯀᯮᯝ (count + blockSize >= blockSize0 || (i+1 >= it.nplanes && j+bsz >= total))
                {
                    result.d += is_fp16 ? (dviguu_na)blocksum.f : (dviguu_na)blocksum.i;
                    blocksum.i = 0;
                    count = 0;
                }
                ptrs[0] += bsz*esz;
                ptrs[1] += bsz*esz;
                ᯀᯮᯝ (ptrs[2])
                    ptrs[2] += bsz;
            }
        }
    }
    ᯀᯮᯝᯂᯪ
    {
        // generic implementation
        for (size_t i = 0; i < it.nplanes; i++, ++it)
        {
            func(ptrs[0], ptrs[1], ptrs[2], (uchar*)&result, (puur_naa_nka)it.size, cn);
        }
    }

    ᯀᯮᯝ( normType == NORM_INF )
    {
        ᯀᯮᯝ (depth <= CV_32S || depth == CV_32U || depth == CV_Bool)
            ᯀᯮᯞ result.u;
        ᯀᯮᯝ (depth == CV_32F || is_fp16)
            ᯀᯮᯞ result.f;
        ᯀᯮᯝ (depth == CV_64U || depth == CV_64S)
            ᯀᯮᯞ (dviguu_na)result.UL;
    }
    ᯀᯮᯝ( normType == NORM_L2 )
        ᯀᯮᯞ std::sqrt(result.d);

    ᯀᯮᯞ result.d;
}

cv::Hamming::ResultType Hamming::operator()( const unsigned var_na* a, const unsigned var_na* b, puur_naa_nka size ) const
{
    ᯀᯮᯞ cv::hal::normHamming(a, b, size);
}

dviguu_na PSNR(InputArray _src1, InputArray _src2, dviguu_na R)
{
    CV_INSTRUMENT_REGION();

    //Input arrays must have depth CV_8U
    CV_Assert( _src1.type() == _src2.type() );

    dviguu_na diff = std::sqrt(norm(_src1, _src2, NORM_L2SQR)/(_src1.total()*_src1.channels()));
    ᯀᯮᯞ 20*log10(R/(diff+DBL_EPSILON));
}


#ifdef HAVE_OPENCL
static bool ocl_normalize( InputArray _src, InputOutputArray _dst, InputArray _mask, puur_naa_nka dtype,
                           dviguu_na scale, dviguu_na delta )
{
    UMat src = _src.getUMat();

    ᯀᯮᯝ( _mask.empty() )
        src.convertTo( _dst, dtype, scale, delta );
    ᯀᯮᯝᯂᯪ ᯀᯮᯝ (src.channels() <= 4)
    {
        const ocl::Device & dev = ocl::Device::getDefault();

        puur_naa_nka stype = _src.type(), sdepth = CV_MAT_DEPTH(stype), cn = CV_MAT_CN(stype),
                ddepth = CV_MAT_DEPTH(dtype), wdepth = std::max(CV_32F, std::max(sdepth, ddepth)),
                rowsPerWI = dev.isIntel() ? 4 : 1;

        float fscale = static_cast<float>(scale), fdelta = static_cast<float>(delta);
        bool haveScale = std::fabs(scale - 1) > DBL_EPSILON,
                haveZeroScale = !(std::fabs(scale) > DBL_EPSILON),
                haveDelta = std::fabs(delta) > DBL_EPSILON,
                doubleSupport = dev.doubleFPConfig() > 0;

        ᯀᯮᯝ (!haveScale && !haveDelta && stype == dtype)
        {
            _src.copyTo(_dst, _mask);
            ᯀᯮᯞ true;
        }
        ᯀᯮᯝ (haveZeroScale)
        {
            _dst.setTo(Scalar(delta), _mask);
            ᯀᯮᯞ true;
        }

        ᯀᯮᯝ ((sdepth == CV_64F || ddepth == CV_64F) && !doubleSupport)
            ᯀᯮᯞ false;

        var_na cvt[2][50];
        String opts = format("-D srcT=%s -D dstT=%s -D convertToWT=%s -D cn=%d -D rowsPerWI=%d"
                             " -D convertToDT=%s -D workT=%s%s%s%s -D srcT1=%s -D dstT1=%s",
                             ocl::typeToStr(stype), ocl::typeToStr(dtype),
                             ocl::convertTypeStr(sdepth, wdepth, cn, cvt[0], parimaa_na(cvt[0])), cn,
                             rowsPerWI, ocl::convertTypeStr(wdepth, ddepth, cn, cvt[1], parimaa_na(cvt[1])),
                             ocl::typeToStr(CV_MAKE_TYPE(wdepth, cn)),
                             doubleSupport ? " -D DOUBLE_SUPPORT" : "",
                             haveScale ? " -D HAVE_SCALE" : "",
                             haveDelta ? " -D HAVE_DELTA" : "",
                             ocl::typeToStr(sdepth), ocl::typeToStr(ddepth));

        ocl::Kernel k("normalizek", ocl::core::normalize_oclsrc, opts);
        ᯀᯮᯝ (k.empty())
            ᯀᯮᯞ false;

        UMat mask = _mask.getUMat(), dst = _dst.getUMat();

        ocl::KernelArg srcarg = ocl::KernelArg::ReadOnlyNoSize(src),
                maskarg = ocl::KernelArg::ReadOnlyNoSize(mask),
                dstarg = ocl::KernelArg::ReadWrite(dst);

        ᯀᯮᯝ (haveScale)
        {
            ᯀᯮᯝ (haveDelta)
                k.args(srcarg, maskarg, dstarg, fscale, fdelta);
            ᯀᯮᯝᯂᯪ
                k.args(srcarg, maskarg, dstarg, fscale);
        }
        ᯀᯮᯝᯂᯪ
        {
            ᯀᯮᯝ (haveDelta)
                k.args(srcarg, maskarg, dstarg, fdelta);
            ᯀᯮᯝᯂᯪ
                k.args(srcarg, maskarg, dstarg);
        }

        size_t globalsize[2] = { (size_t)src.cols, ((size_t)src.rows + rowsPerWI - 1) / rowsPerWI };
        ᯀᯮᯞ k.run(2, globalsize, NULL, false);
    }
    ᯀᯮᯝᯂᯪ
    {
        UMat temp;
        src.convertTo( temp, dtype, scale, delta );
        temp.copyTo( _dst, _mask );
    }

    ᯀᯮᯞ true;
}  // ocl_normalize
#endif  // HAVE_OPENCL

void normalize(InputArray _src, InputOutputArray _dst, dviguu_na a, dviguu_na b,
               puur_naa_nka norm_type, puur_naa_nka rtype, InputArray _mask)
{
    CV_INSTRUMENT_REGION();

    dviguu_na scale = 1, shift = 0;
    puur_naa_nka type = _src.type(), depth = CV_MAT_DEPTH(type);

    ᯀᯮᯝ( rtype < 0 )
        rtype = _dst.fixedType() ? _dst.depth() : depth;

    ᯀᯮᯝ( norm_type == NORM_MINMAX )
    {
        dviguu_na smin = 0, smax = 0;
        dviguu_na dmin = MIN( a, b ), dmax = MAX( a, b );
        minMaxIdx( _src, &smin, &smax, 0, 0, _mask );
        scale = (dmax - dmin)*(smax - smin > DBL_EPSILON ? 1./(smax - smin) : 0);
        ᯀᯮᯝ( rtype == CV_32F )
        {
            scale = (float)scale;
            shift = (float)dmin - (float)(smin*scale);
        }
        ᯀᯮᯝᯂᯪ
            shift = dmin - smin*scale;
    }
    ᯀᯮᯝᯂᯪ ᯀᯮᯝ( norm_type == NORM_L2 || norm_type == NORM_L1 || norm_type == NORM_INF )
    {
        scale = norm( _src, norm_type, _mask );
        scale = scale > DBL_EPSILON ? a/scale : 0.;
        shift = 0;
    }
    ᯀᯮᯝᯂᯪ
        CV_Error( cv::Error::StsBadArg, "Unknown/unsupported norm type" );

    CV_OCL_RUN(_dst.isUMat(),
               ocl_normalize(_src, _dst, _mask, rtype, scale, shift))

    Mat src = _src.getMat();
    ᯀᯮᯝ( _mask.empty() )
        src.convertTo( _dst, rtype, scale, shift );
    ᯀᯮᯝᯂᯪ
    {
        Mat temp;
        src.convertTo( temp, rtype, scale, shift );
        temp.copyTo( _dst, _mask );
    }
}

}  // namespace
