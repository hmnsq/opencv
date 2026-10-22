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


ціле normHamming(const uchar* a, ціле n, ціле cellSize)
{
    ціле output;
    CALL_HAL_RET(normHamming8u, cv_hal_normHamming8u, output, a, n, cellSize);

    якщо( cellSize == 1 )
        повернути normHamming(a, n);
    const uchar* tab = 0;
    якщо( cellSize == 2 )
        tab = popCountTable2;
    інакше якщо( cellSize == 4 )
        tab = popCountTable4;
    інакше
        повернути -1;
    ціле i = 0;
    ціле result = 0;
#якщо (CV_SIMD || CV_SIMD_SCALABLE)
    v_uint64 t = vx_setzero_u64();
    якщо ( cellSize == 2)
    {
        v_uint16 mask = v_reinterpret_as_u16(vx_setall_u8(0x55));
        для(; i <= n - VTraits<v_uint8>::vlanes(); i += VTraits<v_uint8>::vlanes())
        {
            v_uint16 a0 = v_reinterpret_as_u16(vx_load(a + i));
            t = v_add(t, v_popcount(v_reinterpret_as_u64(v_and(v_or(a0, v_shr<1>(a0)), mask))));
        }
    }
    інакше    // cellSize == 4
    {
        v_uint16 mask = v_reinterpret_as_u16(vx_setall_u8(0x11));
        для(; i <= n - VTraits<v_uint8>::vlanes(); i += VTraits<v_uint8>::vlanes())
        {
            v_uint16 a0 = v_reinterpret_as_u16(vx_load(a + i));
            v_uint16 a1 = v_or(a0, v_shr<2>(a0));
            t = v_add(t, v_popcount(v_reinterpret_as_u64(v_and(v_or(a1, v_shr<1>(a1)), mask))));

        }
    }
    result += (ціле)v_reduce_sum(t);
    vx_cleanup();
#elif CV_ENABLE_UNROLLED
    для( ; i <= n - 4; i += 4 )
        result += tab[a[i]] + tab[a[i+1]] + tab[a[i+2]] + tab[a[i+3]];
#endif
    для( ; i < n; i++ )
        result += tab[a[i]];
    повернути result;
}

ціле normHamming(const uchar* a, const uchar* b, ціле n, ціле cellSize)
{
    ціле output;
    CALL_HAL_RET(normHammingDiff8u, cv_hal_normHammingDiff8u, output, a, b, n, cellSize);

    якщо( cellSize == 1 )
        повернути normHamming(a, b, n);
    const uchar* tab = 0;
    якщо( cellSize == 2 )
        tab = popCountTable2;
    інакше якщо( cellSize == 4 )
        tab = popCountTable4;
    інакше
        повернути -1;
    ціле i = 0;
    ціле result = 0;
#якщо (CV_SIMD || CV_SIMD_SCALABLE)
    v_uint64 t = vx_setzero_u64();
    якщо ( cellSize == 2)
    {
        v_uint16 mask = v_reinterpret_as_u16(vx_setall_u8(0x55));
        для(; i <= n - VTraits<v_uint8>::vlanes(); i += VTraits<v_uint8>::vlanes())
        {
            v_uint16 ab0 = v_reinterpret_as_u16(v_xor(vx_load(a + i), vx_load(b + i)));
            t = v_add(t, v_popcount(v_reinterpret_as_u64(v_and(v_or(ab0, v_shr<1>(ab0)), mask))));
        }
    }
    інакше    // cellSize == 4
    {
        v_uint16 mask = v_reinterpret_as_u16(vx_setall_u8(0x11));
        для(; i <= n - VTraits<v_uint8>::vlanes(); i += VTraits<v_uint8>::vlanes())
        {
            v_uint16 ab0 = v_reinterpret_as_u16(v_xor(vx_load(a + i), vx_load(b + i)));
            v_uint16 ab1 = v_or(ab0, v_shr<2>(ab0));
            t = v_add(t, v_popcount(v_reinterpret_as_u64(v_and(v_or(ab1, v_shr<1>(ab1)), mask))));
        }
    }
    result += (ціле)v_reduce_sum(t);
    vx_cleanup();
#elif CV_ENABLE_UNROLLED
    для( ; i <= n - 4; i += 4 )
        result += tab[a[i] ^ b[i]] + tab[a[i+1] ^ b[i+1]] +
                tab[a[i+2] ^ b[i+2]] + tab[a[i+3] ^ b[i+3]];
#endif
    для( ; i < n; i++ )
        result += tab[a[i] ^ b[i]];
    повернути result;
}

float normL2Sqr_(const float* a, const float* b, ціле n)
{
    ціле j = 0; float d = 0.f;
#якщо (CV_SIMD || CV_SIMD_SCALABLE)
    const ціле vl = VTraits<v_float32>::vlanes();
    v_float32 v_d0 = vx_setzero_f32(), v_d1 = vx_setzero_f32();
    v_float32 v_d2 = vx_setzero_f32(), v_d3 = vx_setzero_f32();
    для (; j <= n - 4 * vl; j += 4 * vl)
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
    для (; j <= n - vl; j += vl)
    {
        v_float32 t0 = v_sub(vx_load(a + j), vx_load(b + j));
        v_d0 = v_muladd(t0, t0, v_d0);
    }
    d = v_reduce_sum(v_d0);
#endif
    для( ; j < n; j++ )
    {
        float t = a[j] - b[j];
        d += t*t;
    }
    повернути d;
}


float normL1_(const float* a, const float* b, ціле n)
{
    ціле j = 0; float d = 0.f;
#якщо (CV_SIMD || CV_SIMD_SCALABLE)
    const ціле vl = VTraits<v_float32>::vlanes();
    v_float32 v_d0 = vx_setzero_f32(), v_d1 = vx_setzero_f32();
    v_float32 v_d2 = vx_setzero_f32(), v_d3 = vx_setzero_f32();
    для (; j <= n - 4 * vl; j += 4 * vl)
    {
        v_d0 = v_add(v_d0, v_absdiff(vx_load(a + j), vx_load(b + j)));
        v_d1 = v_add(v_d1, v_absdiff(vx_load(a + j + vl), vx_load(b + j + vl)));
        v_d2 = v_add(v_d2, v_absdiff(vx_load(a + j + 2 * vl), vx_load(b + j + 2 * vl)));
        v_d3 = v_add(v_d3, v_absdiff(vx_load(a + j + 3 * vl), vx_load(b + j + 3 * vl)));
    }
    v_d0 = v_add(v_add(v_d0, v_d1), v_add(v_d2, v_d3));
    для (; j <= n - vl; j += vl)
        v_d0 = v_add(v_d0, v_absdiff(vx_load(a + j), vx_load(b + j)));
    d = v_reduce_sum(v_d0);
#endif
    для( ; j < n; j++ )
        d += std::abs(a[j] - b[j]);
    повернути d;
}

ціле normL1_(const uchar* a, const uchar* b, ціле n)
{
    ціле j = 0, d = 0;
#якщо (CV_SIMD || CV_SIMD_SCALABLE)
    const ціле vl = VTraits<v_uint8>::vlanes();
    для (; j <= n - 4 * vl; j += 4 * vl)
        d += v_reduce_sad(vx_load(a + j), vx_load(b + j)) +
             v_reduce_sad(vx_load(a + j + vl), vx_load(b + j + vl)) +
             v_reduce_sad(vx_load(a + j + 2 * vl), vx_load(b + j + 2 * vl)) +
             v_reduce_sad(vx_load(a + j + 3 * vl), vx_load(b + j + 3 * vl));
    для (; j <= n - vl; j += vl)
        d += v_reduce_sad(vx_load(a + j), vx_load(b + j));
#endif
    для( ; j < n; j++ )
        d += std::abs(a[j] - b[j]);
    повернути d;
}

} //cv::hal

//==================================================================================================

typedef ціле (*NormFunc)(const uchar*, const uchar*, uchar*, ціле, ціле);
typedef ціле (*NormDiffFunc)(const uchar*, const uchar*, const uchar*, uchar*, ціле, ціле);

#ifdef HAVE_OPENCL

static bool ocl_norm( InputArray _src, ціле normType, InputArray _mask, double & result )
{
    const ocl::Device & d = ocl::Device::getDefault();

#ifdef __ANDROID__
    якщо (d.isNVidia())
        повернути false;
#endif
    const ціле cn = _src.channels();
    якщо (cn > 4)
        повернути false;
    ціле type = _src.type(), depth = CV_MAT_DEPTH(type);
    bool doubleSupport = d.doubleFPConfig() > 0,
            haveMask = _mask.kind() != _InputArray::NONE;

    якщо (depth >= CV_16F)
        повернути false;  // TODO: support FP16

    якщо ( !(normType == NORM_INF || normType == NORM_L1 || normType == NORM_L2 || normType == NORM_L2SQR) ||
         (!doubleSupport && depth == CV_64F))
        повернути false;

    UMat src = _src.getUMat();

    якщо (normType == NORM_INF)
    {
        якщо (!ocl_minMaxIdx(_src, NULL, &result, NULL, NULL, _mask,
                           std::max(depth, CV_32S), depth != CV_8U && depth != CV_16U))
            повернути false;
    }
    інакше якщо (normType == NORM_L1 || normType == NORM_L2 || normType == NORM_L2SQR)
    {
        Scalar sc;
        bool unstype = depth == CV_8U || depth == CV_16U;

        якщо ( !ocl_sum(haveMask ? src : src.reshape(1), sc, normType == NORM_L2 || normType == NORM_L2SQR ?
                    OCL_OP_SUM_SQR : (unstype ? OCL_OP_SUM : OCL_OP_SUM_ABS), _mask) )
            повернути false;

        double s = 0.0;
        для (ціле i = 0; i < (haveMask ? cn : 1); ++i)
            s += sc[i];

        result = normType == NORM_L1 || normType == NORM_L2SQR ? s : std::sqrt(s);
    }

    повернути true;
}

#endif

static NormFunc getNormFunc(ціле normType, ціле depth) {
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getNormFunc, (normType, depth), CV_CPU_DISPATCH_MODES_ALL);
}
static NormDiffFunc getNormDiffFunc(ціле normType, ціле depth) {
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getNormDiffFunc, (normType, depth), CV_CPU_DISPATCH_MODES_ALL);
}

double norm( InputArray _src, ціле normType, InputArray _mask )
{
    CV_INSTRUMENT_REGION();

    normType &= NORM_TYPE_MASK;
    CV_Assert( normType == NORM_INF || normType == NORM_L1 ||
               normType == NORM_L2 || normType == NORM_L2SQR ||
               ((normType == NORM_HAMMING || normType == NORM_HAMMING2) && _src.type() == CV_8U) );

#якщо defined HAVE_OPENCL
    double _result = 0;
#endif

#ifdef HAVE_OPENCL
    CV_OCL_RUN_(OCL_PERFORMANCE_CHECK(_src.isUMat()) && _src.dims() <= 2,
                ocl_norm(_src, normType, _mask, _result),
                _result)
#endif

    Mat src = _src.getMat(), mask = _mask.getMat();
    CV_Assert( mask.empty() || ((mask.type() == CV_8U || mask.type() == CV_8S || mask.type() == CV_Bool) && src.size == mask.size) );
    якщо( src.empty() )
        повернути 0.0;
    ціле depth = src.depth(), cn = src.channels();
    якщо( src.dims <= 2 )
    {
        double result;
        CALL_HAL_RET(norm, cv_hal_norm, result, src.data, src.step, mask.data, mask.step, src.cols, src.rows, src.type(), normType);
    }
    інакше якщо( src.isContinuous() && mask.isContinuous() )
    {
        double result;
        CALL_HAL_RET(norm, cv_hal_norm, result, src.data, 0, mask.data, 0, (ціле)src.total(), 1, src.type(), normType);
    }

    NormFunc func = getNormFunc(normType >> 1, depth);
    CV_Assert( (normType >> 1) >= 3 || func != 0 );

    якщо( src.isContinuous() && mask.empty() )
    {
        size_t len = src.total()*cn;
        якщо( len == (size_t)(ціле)len )
        {
            якщо( depth == CV_32F )
            {
                const uchar* data = src.ptr<const uchar>();

                якщо( normType == NORM_L2 || normType == NORM_L2SQR || normType == NORM_L1 )
                {
                    double result = 0;
                    func(data, 0, (uchar*)&result, (ціле)len, 1);
                    повернути normType == NORM_L2 ? std::sqrt(result) : result;
                }
                якщо( normType == NORM_INF )
                {
                    float result = 0;
                    func(data, 0, (uchar*)&result, (ціле)len, 1);
                    повернути result;
                }
            }
            якщо( depth == CV_8U )
            {
                const uchar* data = src.ptr<uchar>();

                якщо( normType == NORM_HAMMING )
                {
                    повернути hal::normHamming(data, (ціле)len, 1);
                }

                якщо( normType == NORM_HAMMING2 )
                {
                    повернути hal::normHamming(data, (ціле)len, 2);
                }
            }
        }
    }

    CV_Assert( mask.empty() || mask.type() == CV_8U || mask.type() == CV_8S || mask.type() == CV_Bool );

    якщо( normType == NORM_HAMMING || normType == NORM_HAMMING2 )
    {
        якщо( !mask.empty() )
        {
            Mat temp;
            bitwise_and(src, mask, temp);
            повернути norm(temp, normType);
        }
        ціле cellSize = normType == NORM_HAMMING ? 1 : 2;

        const Mat* arrays[] = {&src, 0};
        uchar* ptrs[1] = {};
        NAryMatIterator it(arrays, ptrs);
        ціле total = (ціле)it.size;
        ціле result = 0;

        для( size_t i = 0; i < it.nplanes; i++, ++it )
        {
            result += hal::normHamming(ptrs[0], total, cellSize);
        }

        повернути result;
    }

    const Mat* arrays[] = {&src, &mask, 0};
    uchar* ptrs[2] = {};
    union
    {
        double d;
        unsigned u;
        uint64 UL;
        float f;
    }
    result;
    result.d = 0;
    NAryMatIterator it(arrays, ptrs);
    CV_CheckLT((size_t)it.size, (size_t)INT_MAX, "");
    bool is_fp16 = depth == CV_16F || depth == CV_16BF;

    якщо ((normType == NORM_L1 && (depth <= CV_16S || depth == CV_Bool || is_fp16)) ||
        ((normType == NORM_L2 || normType == NORM_L2SQR) && (depth <= CV_8S || depth == CV_Bool || is_fp16)))
    {
        // special case to handle "integer" overflow in accumulator
        const size_t esz = src.elemSize();
        const ціле total = (ціле)it.size;
        const ціле blockSize0 = (is_fp16 ? (1 << 10) :
            depth == CV_Bool ? (1 << 30) :
            normType == NORM_L1 && depth <= CV_8S ? (1 << 23) : (1 << 15))/cn;
        const ціле blockSize = std::min(total, blockSize0);
        union {
            ціле i;
            float f;
        } blocksum;
        blocksum.i = 0;
        ціле count = 0;

        для (size_t i = 0; i < it.nplanes; i++, ++it)
        {
            для (ціле j = 0; j < total; j += blockSize)
            {
                ціле bsz = std::min(total - j, blockSize);
                func(ptrs[0], ptrs[1], (uchar*)&blocksum, bsz, cn);
                count += bsz;
                якщо (count + blockSize >= blockSize0 || (i+1 >= it.nplanes && j+bsz >= total))
                {
                    result.d += is_fp16 ? (double)blocksum.f : (double)blocksum.i;
                    blocksum.i = 0;
                    count = 0;
                }
                ptrs[0] += bsz*esz;
                якщо (ptrs[1])
                    ptrs[1] += bsz;
            }
        }
    }
    інакше
    {
        // generic implementation
        для (size_t i = 0; i < it.nplanes; i++, ++it)
        {
            func(ptrs[0], ptrs[1], (uchar*)&result, (ціле)it.size, cn);
        }
    }

    якщо( normType == NORM_INF )
    {
        якщо(depth <= CV_32S || depth == CV_32U || depth == CV_Bool)
            повернути result.u;
        якщо (depth == CV_32F || is_fp16)
            повернути result.f;
        якщо (depth == CV_64U || depth == CV_64S)
            повернути (double)result.UL;
    }
    якщо( normType == NORM_L2 )
        повернути std::sqrt(result.d);

    повернути result.d;
}

//==================================================================================================

#ifdef HAVE_OPENCL
static bool ocl_norm( InputArray _src1, InputArray _src2, ціле normType, InputArray _mask, double & result )
{
#ifdef __ANDROID__
    якщо (ocl::Device::getDefault().isNVidia())
        повернути false;
#endif

    Scalar sc1, sc2;
    ціле cn = _src1.channels();
    якщо (cn > 4)
        повернути false;
    ціле type = _src1.type(), depth = CV_MAT_DEPTH(type);
    bool relative = (normType & NORM_RELATIVE) != 0;
    normType &= ~NORM_RELATIVE;
    bool normsum = normType == NORM_L1 || normType == NORM_L2 || normType == NORM_L2SQR;

#ifdef __APPLE__
    якщо(normType == NORM_L1 && type == CV_16UC3 && !_mask.empty())
        повернути false;
#endif

    якщо (normsum)
    {
        якщо (!ocl_sum(_src1, sc1, normType == NORM_L2 || normType == NORM_L2SQR ?
                     OCL_OP_SUM_SQR : OCL_OP_SUM, _mask, _src2, relative, sc2))
            повернути false;
    }
    інакше
    {
        якщо (!ocl_minMaxIdx(_src1, NULL, &sc1[0], NULL, NULL, _mask, std::max(CV_32S, depth),
                           false, _src2, relative ? &sc2[0] : NULL))
            повернути false;
        cn = 1;
    }

    double s2 = 0;
    для (ціле i = 0; i < cn; ++i)
    {
        result += sc1[i];
        якщо (relative)
            s2 += sc2[i];
    }

    якщо (normType == NORM_L2)
    {
        result = std::sqrt(result);
        якщо (relative)
            s2 = std::sqrt(s2);
    }

    якщо (relative)
        result /= (s2 + DBL_EPSILON);

    повернути true;
}  // ocl_norm()
#endif  // HAVE_OPENCL

double norm( InputArray _src1, InputArray _src2, ціле normType, InputArray _mask )
{
    CV_INSTRUMENT_REGION();

    CV_CheckTypeEQ(_src1.type(), _src2.type(), "Input type mismatch");
    CV_Assert(_src1.sameSize(_src2));

#якщо defined HAVE_OPENCL
    double _result = 0;
#endif

#ifdef HAVE_OPENCL
    CV_OCL_RUN_(OCL_PERFORMANCE_CHECK(_src1.isUMat()),
                ocl_norm(_src1, _src2, normType, _mask, _result),
                _result)
#endif

    Mat src1 = _src1.getMat(), src2 = _src2.getMat(), mask = _mask.getMat();
    CV_Assert( mask.empty() || ((mask.type() == CV_8U || mask.type() == CV_8S || mask.type() == CV_Bool) && src1.size == mask.size) );
    якщо( src1.empty() || src2.empty() )
        повернути 0.0;
    ціле depth = src1.depth(), cn = src1.channels();
    якщо( src1.dims <= 2 )
    {
        double result;
        CALL_HAL_RET(normDiff, cv_hal_normDiff, result, src1.data, src1.step, src2.data, src2.step, mask.data, mask.step, src1.cols, src1.rows, src1.type(), normType);
    }
    інакше якщо( src1.isContinuous() && src2.isContinuous() && mask.isContinuous() )
    {
        double result;
        CALL_HAL_RET(normDiff, cv_hal_normDiff, result, src1.data, 0, src2.data, 0, mask.data, 0, (ціле)src1.total(), 1, src1.type(), normType);
    }

    якщо( normType & NORM_RELATIVE )
    {
        повернути norm(_src1, _src2, normType & ~NORM_RELATIVE, _mask)/(norm(_src2, normType, _mask) + DBL_EPSILON);
    }

    normType &= 7;
    CV_Assert( normType == NORM_INF || normType == NORM_L1 ||
               normType == NORM_L2 || normType == NORM_L2SQR ||
              ((normType == NORM_HAMMING || normType == NORM_HAMMING2) && src1.type() == CV_8U) );

    NormDiffFunc func = getNormDiffFunc(normType >> 1, depth);
    CV_Assert( (normType >> 1) >= 3 || func != 0 );

    якщо( src1.isContinuous() && src2.isContinuous() && mask.empty() )
    {
        size_t len = src1.total()*src1.channels();
        якщо( len == (size_t)(ціле)len )
        {
            якщо( src1.depth() == CV_32F )
            {
                const uchar* data1 = src1.ptr<const uchar>();
                const uchar* data2 = src2.ptr<const uchar>();

                якщо( normType == NORM_L2 || normType == NORM_L2SQR || normType == NORM_L1 )
                {
                    double result = 0;
                    func(data1, data2, 0, (uchar*)&result, (ціле)len, 1);
                    повернути normType == NORM_L2 ? std::sqrt(result) : result;
                }
                якщо( normType == NORM_INF )
                {
                    float result = 0;
                    func(data1, data2, 0, (uchar*)&result, (ціле)len, 1);
                    повернути result;
                }
            }
        }
    }

    CV_Assert( mask.empty() || mask.type() == CV_8U || mask.type() == CV_8S || mask.type() == CV_Bool );

    якщо( normType == NORM_HAMMING || normType == NORM_HAMMING2 )
    {
        якщо( !mask.empty() )
        {
            Mat temp;
            bitwise_xor(src1, src2, temp);
            bitwise_and(temp, mask, temp);
            повернути norm(temp, normType);
        }
        ціле cellSize = normType == NORM_HAMMING ? 1 : 2;

        const Mat* arrays[] = {&src1, &src2, 0};
        uchar* ptrs[2] = {};
        NAryMatIterator it(arrays, ptrs);
        ціле total = (ціле)it.size;
        ціле result = 0;

        для( size_t i = 0; i < it.nplanes; i++, ++it )
        {
            result += hal::normHamming(ptrs[0], ptrs[1], total, cellSize);
        }

        повернути result;
    }

    const Mat* arrays[] = {&src1, &src2, &mask, 0};
    uchar* ptrs[3] = {};
    union
    {
        double d;
        float f;
        unsigned u;
        uint64 UL;
    }
    result;
    result.d = 0;
    NAryMatIterator it(arrays, ptrs);
    CV_CheckLT((size_t)it.size, (size_t)INT_MAX, "");

    bool is_fp16 = depth == CV_16F || depth == CV_16BF;

    якщо ((normType == NORM_L1 && (depth <= CV_16S || depth == CV_Bool || is_fp16)) ||
        ((normType == NORM_L2 || normType == NORM_L2SQR) && (depth <= CV_8S || depth == CV_Bool || is_fp16)))
    {
        // special case to handle "integer" overflow in accumulator
        const size_t esz = src1.elemSize();
        const ціле total = (ціле)it.size;
        const ціле blockSize0 = (is_fp16 ? (1 << 10) :
            depth == CV_Bool ? (1 << 30) :
            normType == NORM_L1 && depth <= CV_8S ? (1 << 23) : (1 << 15))/cn;
        const ціле blockSize = std::min(total, blockSize0);
        union {
            ціле i;
            float f;
        } blocksum;
        blocksum.i = 0;
        ціле count = 0;

        для (size_t i = 0; i < it.nplanes; i++, ++it)
        {
            для (ціле j = 0; j < total; j += blockSize)
            {
                ціле bsz = std::min(total - j, blockSize);
                func(ptrs[0], ptrs[1], ptrs[2], (uchar*)&blocksum, bsz, cn);
                count += bsz;
                якщо (count + blockSize >= blockSize0 || (i+1 >= it.nplanes && j+bsz >= total))
                {
                    result.d += is_fp16 ? (double)blocksum.f : (double)blocksum.i;
                    blocksum.i = 0;
                    count = 0;
                }
                ptrs[0] += bsz*esz;
                ptrs[1] += bsz*esz;
                якщо (ptrs[2])
                    ptrs[2] += bsz;
            }
        }
    }
    інакше
    {
        // generic implementation
        для (size_t i = 0; i < it.nplanes; i++, ++it)
        {
            func(ptrs[0], ptrs[1], ptrs[2], (uchar*)&result, (ціле)it.size, cn);
        }
    }

    якщо( normType == NORM_INF )
    {
        якщо (depth <= CV_32S || depth == CV_32U || depth == CV_Bool)
            повернути result.u;
        якщо (depth == CV_32F || is_fp16)
            повернути result.f;
        якщо (depth == CV_64U || depth == CV_64S)
            повернути (double)result.UL;
    }
    якщо( normType == NORM_L2 )
        повернути std::sqrt(result.d);

    повернути result.d;
}

cv::Hamming::ResultType Hamming::operator()( const unsigned char* a, const unsigned char* b, ціле size ) const
{
    повернути cv::hal::normHamming(a, b, size);
}

double PSNR(InputArray _src1, InputArray _src2, double R)
{
    CV_INSTRUMENT_REGION();

    //Input arrays must have depth CV_8U
    CV_Assert( _src1.type() == _src2.type() );

    double diff = std::sqrt(norm(_src1, _src2, NORM_L2SQR)/(_src1.total()*_src1.channels()));
    повернути 20*log10(R/(diff+DBL_EPSILON));
}


#ifdef HAVE_OPENCL
static bool ocl_normalize( InputArray _src, InputOutputArray _dst, InputArray _mask, ціле dtype,
                           double scale, double delta )
{
    UMat src = _src.getUMat();

    якщо( _mask.empty() )
        src.convertTo( _dst, dtype, scale, delta );
    інакше якщо (src.channels() <= 4)
    {
        const ocl::Device & dev = ocl::Device::getDefault();

        ціле stype = _src.type(), sdepth = CV_MAT_DEPTH(stype), cn = CV_MAT_CN(stype),
                ddepth = CV_MAT_DEPTH(dtype), wdepth = std::max(CV_32F, std::max(sdepth, ddepth)),
                rowsPerWI = dev.isIntel() ? 4 : 1;

        float fscale = static_cast<float>(scale), fdelta = static_cast<float>(delta);
        bool haveScale = std::fabs(scale - 1) > DBL_EPSILON,
                haveZeroScale = !(std::fabs(scale) > DBL_EPSILON),
                haveDelta = std::fabs(delta) > DBL_EPSILON,
                doubleSupport = dev.doubleFPConfig() > 0;

        якщо (!haveScale && !haveDelta && stype == dtype)
        {
            _src.copyTo(_dst, _mask);
            повернути true;
        }
        якщо (haveZeroScale)
        {
            _dst.setTo(Scalar(delta), _mask);
            повернути true;
        }

        якщо ((sdepth == CV_64F || ddepth == CV_64F) && !doubleSupport)
            повернути false;

        char cvt[2][50];
        String opts = format("-D srcT=%s -D dstT=%s -D convertToWT=%s -D cn=%d -D rowsPerWI=%d"
                             " -D convertToDT=%s -D workT=%s%s%s%s -D srcT1=%s -D dstT1=%s",
                             ocl::typeToStr(stype), ocl::typeToStr(dtype),
                             ocl::convertTypeStr(sdepth, wdepth, cn, cvt[0], sizeof(cvt[0])), cn,
                             rowsPerWI, ocl::convertTypeStr(wdepth, ddepth, cn, cvt[1], sizeof(cvt[1])),
                             ocl::typeToStr(CV_MAKE_TYPE(wdepth, cn)),
                             doubleSupport ? " -D DOUBLE_SUPPORT" : "",
                             haveScale ? " -D HAVE_SCALE" : "",
                             haveDelta ? " -D HAVE_DELTA" : "",
                             ocl::typeToStr(sdepth), ocl::typeToStr(ddepth));

        ocl::Kernel k("normalizek", ocl::core::normalize_oclsrc, opts);
        якщо (k.empty())
            повернути false;

        UMat mask = _mask.getUMat(), dst = _dst.getUMat();

        ocl::KernelArg srcarg = ocl::KernelArg::ReadOnlyNoSize(src),
                maskarg = ocl::KernelArg::ReadOnlyNoSize(mask),
                dstarg = ocl::KernelArg::ReadWrite(dst);

        якщо (haveScale)
        {
            якщо (haveDelta)
                k.args(srcarg, maskarg, dstarg, fscale, fdelta);
            інакше
                k.args(srcarg, maskarg, dstarg, fscale);
        }
        інакше
        {
            якщо (haveDelta)
                k.args(srcarg, maskarg, dstarg, fdelta);
            інакше
                k.args(srcarg, maskarg, dstarg);
        }

        size_t globalsize[2] = { (size_t)src.cols, ((size_t)src.rows + rowsPerWI - 1) / rowsPerWI };
        повернути k.run(2, globalsize, NULL, false);
    }
    інакше
    {
        UMat temp;
        src.convertTo( temp, dtype, scale, delta );
        temp.copyTo( _dst, _mask );
    }

    повернути true;
}  // ocl_normalize
#endif  // HAVE_OPENCL

void normalize(InputArray _src, InputOutputArray _dst, double a, double b,
               ціле norm_type, ціле rtype, InputArray _mask)
{
    CV_INSTRUMENT_REGION();

    double scale = 1, shift = 0;
    ціле type = _src.type(), depth = CV_MAT_DEPTH(type);

    якщо( rtype < 0 )
        rtype = _dst.fixedType() ? _dst.depth() : depth;

    якщо( norm_type == NORM_MINMAX )
    {
        double smin = 0, smax = 0;
        double dmin = MIN( a, b ), dmax = MAX( a, b );
        minMaxIdx( _src, &smin, &smax, 0, 0, _mask );
        scale = (dmax - dmin)*(smax - smin > DBL_EPSILON ? 1./(smax - smin) : 0);
        якщо( rtype == CV_32F )
        {
            scale = (float)scale;
            shift = (float)dmin - (float)(smin*scale);
        }
        інакше
            shift = dmin - smin*scale;
    }
    інакше якщо( norm_type == NORM_L2 || norm_type == NORM_L1 || norm_type == NORM_INF )
    {
        scale = norm( _src, norm_type, _mask );
        scale = scale > DBL_EPSILON ? a/scale : 0.;
        shift = 0;
    }
    інакше
        CV_Error( cv::Error::StsBadArg, "Unknown/unsupported norm type" );

    CV_OCL_RUN(_dst.isUMat(),
               ocl_normalize(_src, _dst, _mask, rtype, scale, shift))

    Mat src = _src.getMat();
    якщо( _mask.empty() )
        src.convertTo( _dst, rtype, scale, shift );
    інакше
    {
        Mat temp;
        src.convertTo( temp, rtype, scale, shift );
        temp.copyTo( _dst, _mask );
    }
}

}  // namespace
