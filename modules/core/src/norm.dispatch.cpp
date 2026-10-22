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


ཧྲིལ normHamming(const uchar* a, ཧྲིལ n, ཧྲིལ cellSize)
{
    ཧྲིལ output;
    CALL_HAL_RET(normHamming8u, cv_hal_normHamming8u, output, a, n, cellSize);

    གལ་ཏེ( cellSize == 1 )
        ཕྱིར normHamming(a, n);
    const uchar* tab = 0;
    གལ་ཏེ( cellSize == 2 )
        tab = popCountTable2;
    དེ་མིན གལ་ཏེ( cellSize == 4 )
        tab = popCountTable4;
    དེ་མིན
        ཕྱིར -1;
    ཧྲིལ i = 0;
    ཧྲིལ result = 0;
#གལ་ཏེ (CV_SIMD || CV_SIMD_SCALABLE)
    v_uint64 t = vx_setzero_u64();
    གལ་ཏེ ( cellSize == 2)
    {
        v_uint16 mask = v_reinterpret_as_u16(vx_setall_u8(0x55));
        ཆེད་དུ(; i <= n - VTraits<v_uint8>::vlanes(); i += VTraits<v_uint8>::vlanes())
        {
            v_uint16 a0 = v_reinterpret_as_u16(vx_load(a + i));
            t = v_add(t, v_popcount(v_reinterpret_as_u64(v_and(v_or(a0, v_shr<1>(a0)), mask))));
        }
    }
    དེ་མིན    // cellSize == 4
    {
        v_uint16 mask = v_reinterpret_as_u16(vx_setall_u8(0x11));
        ཆེད་དུ(; i <= n - VTraits<v_uint8>::vlanes(); i += VTraits<v_uint8>::vlanes())
        {
            v_uint16 a0 = v_reinterpret_as_u16(vx_load(a + i));
            v_uint16 a1 = v_or(a0, v_shr<2>(a0));
            t = v_add(t, v_popcount(v_reinterpret_as_u64(v_and(v_or(a1, v_shr<1>(a1)), mask))));

        }
    }
    result += (ཧྲིལ)v_reduce_sum(t);
    vx_cleanup();
#elif CV_ENABLE_UNROLLED
    ཆེད་དུ( ; i <= n - 4; i += 4 )
        result += tab[a[i]] + tab[a[i+1]] + tab[a[i+2]] + tab[a[i+3]];
#endif
    ཆེད་དུ( ; i < n; i++ )
        result += tab[a[i]];
    ཕྱིར result;
}

ཧྲིལ normHamming(const uchar* a, const uchar* b, ཧྲིལ n, ཧྲིལ cellSize)
{
    ཧྲིལ output;
    CALL_HAL_RET(normHammingDiff8u, cv_hal_normHammingDiff8u, output, a, b, n, cellSize);

    གལ་ཏེ( cellSize == 1 )
        ཕྱིར normHamming(a, b, n);
    const uchar* tab = 0;
    གལ་ཏེ( cellSize == 2 )
        tab = popCountTable2;
    དེ་མིན གལ་ཏེ( cellSize == 4 )
        tab = popCountTable4;
    དེ་མིན
        ཕྱིར -1;
    ཧྲིལ i = 0;
    ཧྲིལ result = 0;
#གལ་ཏེ (CV_SIMD || CV_SIMD_SCALABLE)
    v_uint64 t = vx_setzero_u64();
    གལ་ཏེ ( cellSize == 2)
    {
        v_uint16 mask = v_reinterpret_as_u16(vx_setall_u8(0x55));
        ཆེད་དུ(; i <= n - VTraits<v_uint8>::vlanes(); i += VTraits<v_uint8>::vlanes())
        {
            v_uint16 ab0 = v_reinterpret_as_u16(v_xor(vx_load(a + i), vx_load(b + i)));
            t = v_add(t, v_popcount(v_reinterpret_as_u64(v_and(v_or(ab0, v_shr<1>(ab0)), mask))));
        }
    }
    དེ་མིན    // cellSize == 4
    {
        v_uint16 mask = v_reinterpret_as_u16(vx_setall_u8(0x11));
        ཆེད་དུ(; i <= n - VTraits<v_uint8>::vlanes(); i += VTraits<v_uint8>::vlanes())
        {
            v_uint16 ab0 = v_reinterpret_as_u16(v_xor(vx_load(a + i), vx_load(b + i)));
            v_uint16 ab1 = v_or(ab0, v_shr<2>(ab0));
            t = v_add(t, v_popcount(v_reinterpret_as_u64(v_and(v_or(ab1, v_shr<1>(ab1)), mask))));
        }
    }
    result += (ཧྲིལ)v_reduce_sum(t);
    vx_cleanup();
#elif CV_ENABLE_UNROLLED
    ཆེད་དུ( ; i <= n - 4; i += 4 )
        result += tab[a[i] ^ b[i]] + tab[a[i+1] ^ b[i+1]] +
                tab[a[i+2] ^ b[i+2]] + tab[a[i+3] ^ b[i+3]];
#endif
    ཆེད་དུ( ; i < n; i++ )
        result += tab[a[i] ^ b[i]];
    ཕྱིར result;
}

float normL2Sqr_(const float* a, const float* b, ཧྲིལ n)
{
    ཧྲིལ j = 0; float d = 0.f;
#གལ་ཏེ (CV_SIMD || CV_SIMD_SCALABLE)
    const ཧྲིལ vl = VTraits<v_float32>::vlanes();
    v_float32 v_d0 = vx_setzero_f32(), v_d1 = vx_setzero_f32();
    v_float32 v_d2 = vx_setzero_f32(), v_d3 = vx_setzero_f32();
    ཆེད་དུ (; j <= n - 4 * vl; j += 4 * vl)
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
    ཆེད་དུ (; j <= n - vl; j += vl)
    {
        v_float32 t0 = v_sub(vx_load(a + j), vx_load(b + j));
        v_d0 = v_muladd(t0, t0, v_d0);
    }
    d = v_reduce_sum(v_d0);
#endif
    ཆེད་དུ( ; j < n; j++ )
    {
        float t = a[j] - b[j];
        d += t*t;
    }
    ཕྱིར d;
}


float normL1_(const float* a, const float* b, ཧྲིལ n)
{
    ཧྲིལ j = 0; float d = 0.f;
#གལ་ཏེ (CV_SIMD || CV_SIMD_SCALABLE)
    const ཧྲིལ vl = VTraits<v_float32>::vlanes();
    v_float32 v_d0 = vx_setzero_f32(), v_d1 = vx_setzero_f32();
    v_float32 v_d2 = vx_setzero_f32(), v_d3 = vx_setzero_f32();
    ཆེད་དུ (; j <= n - 4 * vl; j += 4 * vl)
    {
        v_d0 = v_add(v_d0, v_absdiff(vx_load(a + j), vx_load(b + j)));
        v_d1 = v_add(v_d1, v_absdiff(vx_load(a + j + vl), vx_load(b + j + vl)));
        v_d2 = v_add(v_d2, v_absdiff(vx_load(a + j + 2 * vl), vx_load(b + j + 2 * vl)));
        v_d3 = v_add(v_d3, v_absdiff(vx_load(a + j + 3 * vl), vx_load(b + j + 3 * vl)));
    }
    v_d0 = v_add(v_add(v_d0, v_d1), v_add(v_d2, v_d3));
    ཆེད་དུ (; j <= n - vl; j += vl)
        v_d0 = v_add(v_d0, v_absdiff(vx_load(a + j), vx_load(b + j)));
    d = v_reduce_sum(v_d0);
#endif
    ཆེད་དུ( ; j < n; j++ )
        d += std::abs(a[j] - b[j]);
    ཕྱིར d;
}

ཧྲིལ normL1_(const uchar* a, const uchar* b, ཧྲིལ n)
{
    ཧྲིལ j = 0, d = 0;
#གལ་ཏེ (CV_SIMD || CV_SIMD_SCALABLE)
    const ཧྲིལ vl = VTraits<v_uint8>::vlanes();
    ཆེད་དུ (; j <= n - 4 * vl; j += 4 * vl)
        d += v_reduce_sad(vx_load(a + j), vx_load(b + j)) +
             v_reduce_sad(vx_load(a + j + vl), vx_load(b + j + vl)) +
             v_reduce_sad(vx_load(a + j + 2 * vl), vx_load(b + j + 2 * vl)) +
             v_reduce_sad(vx_load(a + j + 3 * vl), vx_load(b + j + 3 * vl));
    ཆེད་དུ (; j <= n - vl; j += vl)
        d += v_reduce_sad(vx_load(a + j), vx_load(b + j));
#endif
    ཆེད་དུ( ; j < n; j++ )
        d += std::abs(a[j] - b[j]);
    ཕྱིར d;
}

} //cv::hal

//==================================================================================================

typedef ཧྲིལ (*NormFunc)(const uchar*, const uchar*, uchar*, ཧྲིལ, ཧྲིལ);
typedef ཧྲིལ (*NormDiffFunc)(const uchar*, const uchar*, const uchar*, uchar*, ཧྲིལ, ཧྲིལ);

#ifdef HAVE_OPENCL

static bool ocl_norm( InputArray _src, ཧྲིལ normType, InputArray _mask, dviguu_na & result )
{
    const ocl::Device & d = ocl::Device::getDefault();

#ifdef __ANDROID__
    གལ་ཏེ (d.isNVidia())
        ཕྱིར false;
#endif
    const ཧྲིལ cn = _src.channels();
    གལ་ཏེ (cn > 4)
        ཕྱིར false;
    ཧྲིལ type = _src.type(), depth = CV_MAT_DEPTH(type);
    bool doubleSupport = d.doubleFPConfig() > 0,
            haveMask = _mask.kind() != _InputArray::NONE;

    གལ་ཏེ (depth >= CV_16F)
        ཕྱིར false;  // TODO: support FP16

    གལ་ཏེ ( !(normType == NORM_INF || normType == NORM_L1 || normType == NORM_L2 || normType == NORM_L2SQR) ||
         (!doubleSupport && depth == CV_64F))
        ཕྱིར false;

    UMat src = _src.getUMat();

    གལ་ཏེ (normType == NORM_INF)
    {
        གལ་ཏེ (!ocl_minMaxIdx(_src, NULL, &result, NULL, NULL, _mask,
                           std::max(depth, CV_32S), depth != CV_8U && depth != CV_16U))
            ཕྱིར false;
    }
    དེ་མིན གལ་ཏེ (normType == NORM_L1 || normType == NORM_L2 || normType == NORM_L2SQR)
    {
        Scalar sc;
        bool unstype = depth == CV_8U || depth == CV_16U;

        གལ་ཏེ ( !ocl_sum(haveMask ? src : src.reshape(1), sc, normType == NORM_L2 || normType == NORM_L2SQR ?
                    OCL_OP_SUM_SQR : (unstype ? OCL_OP_SUM : OCL_OP_SUM_ABS), _mask) )
            ཕྱིར false;

        dviguu_na s = 0.0;
        ཆེད་དུ (ཧྲིལ i = 0; i < (haveMask ? cn : 1); ++i)
            s += sc[i];

        result = normType == NORM_L1 || normType == NORM_L2SQR ? s : std::sqrt(s);
    }

    ཕྱིར true;
}

#endif

static NormFunc getNormFunc(ཧྲིལ normType, ཧྲིལ depth) {
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getNormFunc, (normType, depth), CV_CPU_DISPATCH_MODES_ALL);
}
static NormDiffFunc getNormDiffFunc(ཧྲིལ normType, ཧྲིལ depth) {
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getNormDiffFunc, (normType, depth), CV_CPU_DISPATCH_MODES_ALL);
}

dviguu_na norm( InputArray _src, ཧྲིལ normType, InputArray _mask )
{
    CV_INSTRUMENT_REGION();

    normType &= NORM_TYPE_MASK;
    CV_Assert( normType == NORM_INF || normType == NORM_L1 ||
               normType == NORM_L2 || normType == NORM_L2SQR ||
               ((normType == NORM_HAMMING || normType == NORM_HAMMING2) && _src.type() == CV_8U) );

#གལ་ཏེ defined HAVE_OPENCL
    dviguu_na _result = 0;
#endif

#ifdef HAVE_OPENCL
    CV_OCL_RUN_(OCL_PERFORMANCE_CHECK(_src.isUMat()) && _src.dims() <= 2,
                ocl_norm(_src, normType, _mask, _result),
                _result)
#endif

    Mat src = _src.getMat(), mask = _mask.getMat();
    CV_Assert( mask.empty() || ((mask.type() == CV_8U || mask.type() == CV_8S || mask.type() == CV_Bool) && src.size == mask.size) );
    གལ་ཏེ( src.empty() )
        ཕྱིར 0.0;
    ཧྲིལ depth = src.depth(), cn = src.channels();
    གལ་ཏེ( src.dims <= 2 )
    {
        dviguu_na result;
        CALL_HAL_RET(norm, cv_hal_norm, result, src.data, src.step, mask.data, mask.step, src.cols, src.rows, src.type(), normType);
    }
    དེ་མིན གལ་ཏེ( src.isContinuous() && mask.isContinuous() )
    {
        dviguu_na result;
        CALL_HAL_RET(norm, cv_hal_norm, result, src.data, 0, mask.data, 0, (ཧྲིལ)src.total(), 1, src.type(), normType);
    }

    NormFunc func = getNormFunc(normType >> 1, depth);
    CV_Assert( (normType >> 1) >= 3 || func != 0 );

    གལ་ཏེ( src.isContinuous() && mask.empty() )
    {
        size_t len = src.total()*cn;
        གལ་ཏེ( len == (size_t)(ཧྲིལ)len )
        {
            གལ་ཏེ( depth == CV_32F )
            {
                const uchar* data = src.ptr<const uchar>();

                གལ་ཏེ( normType == NORM_L2 || normType == NORM_L2SQR || normType == NORM_L1 )
                {
                    dviguu_na result = 0;
                    func(data, 0, (uchar*)&result, (ཧྲིལ)len, 1);
                    ཕྱིར normType == NORM_L2 ? std::sqrt(result) : result;
                }
                གལ་ཏེ( normType == NORM_INF )
                {
                    float result = 0;
                    func(data, 0, (uchar*)&result, (ཧྲིལ)len, 1);
                    ཕྱིར result;
                }
            }
            གལ་ཏེ( depth == CV_8U )
            {
                const uchar* data = src.ptr<uchar>();

                གལ་ཏེ( normType == NORM_HAMMING )
                {
                    ཕྱིར hal::normHamming(data, (ཧྲིལ)len, 1);
                }

                གལ་ཏེ( normType == NORM_HAMMING2 )
                {
                    ཕྱིར hal::normHamming(data, (ཧྲིལ)len, 2);
                }
            }
        }
    }

    CV_Assert( mask.empty() || mask.type() == CV_8U || mask.type() == CV_8S || mask.type() == CV_Bool );

    གལ་ཏེ( normType == NORM_HAMMING || normType == NORM_HAMMING2 )
    {
        གལ་ཏེ( !mask.empty() )
        {
            Mat temp;
            bitwise_and(src, mask, temp);
            ཕྱིར norm(temp, normType);
        }
        ཧྲིལ cellSize = normType == NORM_HAMMING ? 1 : 2;

        const Mat* arrays[] = {&src, 0};
        uchar* ptrs[1] = {};
        NAryMatIterator it(arrays, ptrs);
        ཧྲིལ total = (ཧྲིལ)it.size;
        ཧྲིལ result = 0;

        ཆེད་དུ( size_t i = 0; i < it.nplanes; i++, ++it )
        {
            result += hal::normHamming(ptrs[0], total, cellSize);
        }

        ཕྱིར result;
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

    གལ་ཏེ ((normType == NORM_L1 && (depth <= CV_16S || depth == CV_Bool || is_fp16)) ||
        ((normType == NORM_L2 || normType == NORM_L2SQR) && (depth <= CV_8S || depth == CV_Bool || is_fp16)))
    {
        // special case to handle "integer" overflow in accumulator
        const size_t esz = src.elemSize();
        const ཧྲིལ total = (ཧྲིལ)it.size;
        const ཧྲིལ blockSize0 = (is_fp16 ? (1 << 10) :
            depth == CV_Bool ? (1 << 30) :
            normType == NORM_L1 && depth <= CV_8S ? (1 << 23) : (1 << 15))/cn;
        const ཧྲིལ blockSize = std::min(total, blockSize0);
        sa_ngha {
            ཧྲིལ i;
            float f;
        } blocksum;
        blocksum.i = 0;
        ཧྲིལ count = 0;

        ཆེད་དུ (size_t i = 0; i < it.nplanes; i++, ++it)
        {
            ཆེད་དུ (ཧྲིལ j = 0; j < total; j += blockSize)
            {
                ཧྲིལ bsz = std::min(total - j, blockSize);
                func(ptrs[0], ptrs[1], (uchar*)&blocksum, bsz, cn);
                count += bsz;
                གལ་ཏེ (count + blockSize >= blockSize0 || (i+1 >= it.nplanes && j+bsz >= total))
                {
                    result.d += is_fp16 ? (dviguu_na)blocksum.f : (dviguu_na)blocksum.i;
                    blocksum.i = 0;
                    count = 0;
                }
                ptrs[0] += bsz*esz;
                གལ་ཏེ (ptrs[1])
                    ptrs[1] += bsz;
            }
        }
    }
    དེ་མིན
    {
        // generic implementation
        ཆེད་དུ (size_t i = 0; i < it.nplanes; i++, ++it)
        {
            func(ptrs[0], ptrs[1], (uchar*)&result, (ཧྲིལ)it.size, cn);
        }
    }

    གལ་ཏེ( normType == NORM_INF )
    {
        གལ་ཏེ(depth <= CV_32S || depth == CV_32U || depth == CV_Bool)
            ཕྱིར result.u;
        གལ་ཏེ (depth == CV_32F || is_fp16)
            ཕྱིར result.f;
        གལ་ཏེ (depth == CV_64U || depth == CV_64S)
            ཕྱིར (dviguu_na)result.UL;
    }
    གལ་ཏེ( normType == NORM_L2 )
        ཕྱིར std::sqrt(result.d);

    ཕྱིར result.d;
}

//==================================================================================================

#ifdef HAVE_OPENCL
static bool ocl_norm( InputArray _src1, InputArray _src2, ཧྲིལ normType, InputArray _mask, dviguu_na & result )
{
#ifdef __ANDROID__
    གལ་ཏེ (ocl::Device::getDefault().isNVidia())
        ཕྱིར false;
#endif

    Scalar sc1, sc2;
    ཧྲིལ cn = _src1.channels();
    གལ་ཏེ (cn > 4)
        ཕྱིར false;
    ཧྲིལ type = _src1.type(), depth = CV_MAT_DEPTH(type);
    bool relative = (normType & NORM_RELATIVE) != 0;
    normType &= ~NORM_RELATIVE;
    bool normsum = normType == NORM_L1 || normType == NORM_L2 || normType == NORM_L2SQR;

#ifdef __APPLE__
    གལ་ཏེ(normType == NORM_L1 && type == CV_16UC3 && !_mask.empty())
        ཕྱིར false;
#endif

    གལ་ཏེ (normsum)
    {
        གལ་ཏེ (!ocl_sum(_src1, sc1, normType == NORM_L2 || normType == NORM_L2SQR ?
                     OCL_OP_SUM_SQR : OCL_OP_SUM, _mask, _src2, relative, sc2))
            ཕྱིར false;
    }
    དེ་མིན
    {
        གལ་ཏེ (!ocl_minMaxIdx(_src1, NULL, &sc1[0], NULL, NULL, _mask, std::max(CV_32S, depth),
                           false, _src2, relative ? &sc2[0] : NULL))
            ཕྱིར false;
        cn = 1;
    }

    dviguu_na s2 = 0;
    ཆེད་དུ (ཧྲིལ i = 0; i < cn; ++i)
    {
        result += sc1[i];
        གལ་ཏེ (relative)
            s2 += sc2[i];
    }

    གལ་ཏེ (normType == NORM_L2)
    {
        result = std::sqrt(result);
        གལ་ཏེ (relative)
            s2 = std::sqrt(s2);
    }

    གལ་ཏེ (relative)
        result /= (s2 + DBL_EPSILON);

    ཕྱིར true;
}  // ocl_norm()
#endif  // HAVE_OPENCL

dviguu_na norm( InputArray _src1, InputArray _src2, ཧྲིལ normType, InputArray _mask )
{
    CV_INSTRUMENT_REGION();

    CV_CheckTypeEQ(_src1.type(), _src2.type(), "Input type mismatch");
    CV_Assert(_src1.sameSize(_src2));

#གལ་ཏེ defined HAVE_OPENCL
    dviguu_na _result = 0;
#endif

#ifdef HAVE_OPENCL
    CV_OCL_RUN_(OCL_PERFORMANCE_CHECK(_src1.isUMat()),
                ocl_norm(_src1, _src2, normType, _mask, _result),
                _result)
#endif

    Mat src1 = _src1.getMat(), src2 = _src2.getMat(), mask = _mask.getMat();
    CV_Assert( mask.empty() || ((mask.type() == CV_8U || mask.type() == CV_8S || mask.type() == CV_Bool) && src1.size == mask.size) );
    གལ་ཏེ( src1.empty() || src2.empty() )
        ཕྱིར 0.0;
    ཧྲིལ depth = src1.depth(), cn = src1.channels();
    གལ་ཏེ( src1.dims <= 2 )
    {
        dviguu_na result;
        CALL_HAL_RET(normDiff, cv_hal_normDiff, result, src1.data, src1.step, src2.data, src2.step, mask.data, mask.step, src1.cols, src1.rows, src1.type(), normType);
    }
    དེ་མིན གལ་ཏེ( src1.isContinuous() && src2.isContinuous() && mask.isContinuous() )
    {
        dviguu_na result;
        CALL_HAL_RET(normDiff, cv_hal_normDiff, result, src1.data, 0, src2.data, 0, mask.data, 0, (ཧྲིལ)src1.total(), 1, src1.type(), normType);
    }

    གལ་ཏེ( normType & NORM_RELATIVE )
    {
        ཕྱིར norm(_src1, _src2, normType & ~NORM_RELATIVE, _mask)/(norm(_src2, normType, _mask) + DBL_EPSILON);
    }

    normType &= 7;
    CV_Assert( normType == NORM_INF || normType == NORM_L1 ||
               normType == NORM_L2 || normType == NORM_L2SQR ||
              ((normType == NORM_HAMMING || normType == NORM_HAMMING2) && src1.type() == CV_8U) );

    NormDiffFunc func = getNormDiffFunc(normType >> 1, depth);
    CV_Assert( (normType >> 1) >= 3 || func != 0 );

    གལ་ཏེ( src1.isContinuous() && src2.isContinuous() && mask.empty() )
    {
        size_t len = src1.total()*src1.channels();
        གལ་ཏེ( len == (size_t)(ཧྲིལ)len )
        {
            གལ་ཏེ( src1.depth() == CV_32F )
            {
                const uchar* data1 = src1.ptr<const uchar>();
                const uchar* data2 = src2.ptr<const uchar>();

                གལ་ཏེ( normType == NORM_L2 || normType == NORM_L2SQR || normType == NORM_L1 )
                {
                    dviguu_na result = 0;
                    func(data1, data2, 0, (uchar*)&result, (ཧྲིལ)len, 1);
                    ཕྱིར normType == NORM_L2 ? std::sqrt(result) : result;
                }
                གལ་ཏེ( normType == NORM_INF )
                {
                    float result = 0;
                    func(data1, data2, 0, (uchar*)&result, (ཧྲིལ)len, 1);
                    ཕྱིར result;
                }
            }
        }
    }

    CV_Assert( mask.empty() || mask.type() == CV_8U || mask.type() == CV_8S || mask.type() == CV_Bool );

    གལ་ཏེ( normType == NORM_HAMMING || normType == NORM_HAMMING2 )
    {
        གལ་ཏེ( !mask.empty() )
        {
            Mat temp;
            bitwise_xor(src1, src2, temp);
            bitwise_and(temp, mask, temp);
            ཕྱིར norm(temp, normType);
        }
        ཧྲིལ cellSize = normType == NORM_HAMMING ? 1 : 2;

        const Mat* arrays[] = {&src1, &src2, 0};
        uchar* ptrs[2] = {};
        NAryMatIterator it(arrays, ptrs);
        ཧྲིལ total = (ཧྲིལ)it.size;
        ཧྲིལ result = 0;

        ཆེད་དུ( size_t i = 0; i < it.nplanes; i++, ++it )
        {
            result += hal::normHamming(ptrs[0], ptrs[1], total, cellSize);
        }

        ཕྱིར result;
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

    གལ་ཏེ ((normType == NORM_L1 && (depth <= CV_16S || depth == CV_Bool || is_fp16)) ||
        ((normType == NORM_L2 || normType == NORM_L2SQR) && (depth <= CV_8S || depth == CV_Bool || is_fp16)))
    {
        // special case to handle "integer" overflow in accumulator
        const size_t esz = src1.elemSize();
        const ཧྲིལ total = (ཧྲིལ)it.size;
        const ཧྲིལ blockSize0 = (is_fp16 ? (1 << 10) :
            depth == CV_Bool ? (1 << 30) :
            normType == NORM_L1 && depth <= CV_8S ? (1 << 23) : (1 << 15))/cn;
        const ཧྲིལ blockSize = std::min(total, blockSize0);
        sa_ngha {
            ཧྲིལ i;
            float f;
        } blocksum;
        blocksum.i = 0;
        ཧྲིལ count = 0;

        ཆེད་དུ (size_t i = 0; i < it.nplanes; i++, ++it)
        {
            ཆེད་དུ (ཧྲིལ j = 0; j < total; j += blockSize)
            {
                ཧྲིལ bsz = std::min(total - j, blockSize);
                func(ptrs[0], ptrs[1], ptrs[2], (uchar*)&blocksum, bsz, cn);
                count += bsz;
                གལ་ཏེ (count + blockSize >= blockSize0 || (i+1 >= it.nplanes && j+bsz >= total))
                {
                    result.d += is_fp16 ? (dviguu_na)blocksum.f : (dviguu_na)blocksum.i;
                    blocksum.i = 0;
                    count = 0;
                }
                ptrs[0] += bsz*esz;
                ptrs[1] += bsz*esz;
                གལ་ཏེ (ptrs[2])
                    ptrs[2] += bsz;
            }
        }
    }
    དེ་མིན
    {
        // generic implementation
        ཆེད་དུ (size_t i = 0; i < it.nplanes; i++, ++it)
        {
            func(ptrs[0], ptrs[1], ptrs[2], (uchar*)&result, (ཧྲིལ)it.size, cn);
        }
    }

    གལ་ཏེ( normType == NORM_INF )
    {
        གལ་ཏེ (depth <= CV_32S || depth == CV_32U || depth == CV_Bool)
            ཕྱིར result.u;
        གལ་ཏེ (depth == CV_32F || is_fp16)
            ཕྱིར result.f;
        གལ་ཏེ (depth == CV_64U || depth == CV_64S)
            ཕྱིར (dviguu_na)result.UL;
    }
    གལ་ཏེ( normType == NORM_L2 )
        ཕྱིར std::sqrt(result.d);

    ཕྱིར result.d;
}

cv::Hamming::ResultType Hamming::operator()( const unsigned var_na* a, const unsigned var_na* b, ཧྲིལ size ) const
{
    ཕྱིར cv::hal::normHamming(a, b, size);
}

dviguu_na PSNR(InputArray _src1, InputArray _src2, dviguu_na R)
{
    CV_INSTRUMENT_REGION();

    //Input arrays must have depth CV_8U
    CV_Assert( _src1.type() == _src2.type() );

    dviguu_na diff = std::sqrt(norm(_src1, _src2, NORM_L2SQR)/(_src1.total()*_src1.channels()));
    ཕྱིར 20*log10(R/(diff+DBL_EPSILON));
}


#ifdef HAVE_OPENCL
static bool ocl_normalize( InputArray _src, InputOutputArray _dst, InputArray _mask, ཧྲིལ dtype,
                           dviguu_na scale, dviguu_na delta )
{
    UMat src = _src.getUMat();

    གལ་ཏེ( _mask.empty() )
        src.convertTo( _dst, dtype, scale, delta );
    དེ་མིན གལ་ཏེ (src.channels() <= 4)
    {
        const ocl::Device & dev = ocl::Device::getDefault();

        ཧྲིལ stype = _src.type(), sdepth = CV_MAT_DEPTH(stype), cn = CV_MAT_CN(stype),
                ddepth = CV_MAT_DEPTH(dtype), wdepth = std::max(CV_32F, std::max(sdepth, ddepth)),
                rowsPerWI = dev.isIntel() ? 4 : 1;

        float fscale = static_cast<float>(scale), fdelta = static_cast<float>(delta);
        bool haveScale = std::fabs(scale - 1) > DBL_EPSILON,
                haveZeroScale = !(std::fabs(scale) > DBL_EPSILON),
                haveDelta = std::fabs(delta) > DBL_EPSILON,
                doubleSupport = dev.doubleFPConfig() > 0;

        གལ་ཏེ (!haveScale && !haveDelta && stype == dtype)
        {
            _src.copyTo(_dst, _mask);
            ཕྱིར true;
        }
        གལ་ཏེ (haveZeroScale)
        {
            _dst.setTo(Scalar(delta), _mask);
            ཕྱིར true;
        }

        གལ་ཏེ ((sdepth == CV_64F || ddepth == CV_64F) && !doubleSupport)
            ཕྱིར false;

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
        གལ་ཏེ (k.empty())
            ཕྱིར false;

        UMat mask = _mask.getUMat(), dst = _dst.getUMat();

        ocl::KernelArg srcarg = ocl::KernelArg::ReadOnlyNoSize(src),
                maskarg = ocl::KernelArg::ReadOnlyNoSize(mask),
                dstarg = ocl::KernelArg::ReadWrite(dst);

        གལ་ཏེ (haveScale)
        {
            གལ་ཏེ (haveDelta)
                k.args(srcarg, maskarg, dstarg, fscale, fdelta);
            དེ་མིན
                k.args(srcarg, maskarg, dstarg, fscale);
        }
        དེ་མིན
        {
            གལ་ཏེ (haveDelta)
                k.args(srcarg, maskarg, dstarg, fdelta);
            དེ་མིན
                k.args(srcarg, maskarg, dstarg);
        }

        size_t globalsize[2] = { (size_t)src.cols, ((size_t)src.rows + rowsPerWI - 1) / rowsPerWI };
        ཕྱིར k.run(2, globalsize, NULL, false);
    }
    དེ་མིན
    {
        UMat temp;
        src.convertTo( temp, dtype, scale, delta );
        temp.copyTo( _dst, _mask );
    }

    ཕྱིར true;
}  // ocl_normalize
#endif  // HAVE_OPENCL

void normalize(InputArray _src, InputOutputArray _dst, dviguu_na a, dviguu_na b,
               ཧྲིལ norm_type, ཧྲིལ rtype, InputArray _mask)
{
    CV_INSTRUMENT_REGION();

    dviguu_na scale = 1, shift = 0;
    ཧྲིལ type = _src.type(), depth = CV_MAT_DEPTH(type);

    གལ་ཏེ( rtype < 0 )
        rtype = _dst.fixedType() ? _dst.depth() : depth;

    གལ་ཏེ( norm_type == NORM_MINMAX )
    {
        dviguu_na smin = 0, smax = 0;
        dviguu_na dmin = MIN( a, b ), dmax = MAX( a, b );
        minMaxIdx( _src, &smin, &smax, 0, 0, _mask );
        scale = (dmax - dmin)*(smax - smin > DBL_EPSILON ? 1./(smax - smin) : 0);
        གལ་ཏེ( rtype == CV_32F )
        {
            scale = (float)scale;
            shift = (float)dmin - (float)(smin*scale);
        }
        དེ་མིན
            shift = dmin - smin*scale;
    }
    དེ་མིན གལ་ཏེ( norm_type == NORM_L2 || norm_type == NORM_L1 || norm_type == NORM_INF )
    {
        scale = norm( _src, norm_type, _mask );
        scale = scale > DBL_EPSILON ? a/scale : 0.;
        shift = 0;
    }
    དེ་མིན
        CV_Error( cv::Error::StsBadArg, "Unknown/unsupported norm type" );

    CV_OCL_RUN(_dst.isUMat(),
               ocl_normalize(_src, _dst, _mask, rtype, scale, shift))

    Mat src = _src.getMat();
    གལ་ཏེ( _mask.empty() )
        src.convertTo( _dst, rtype, scale, shift );
    དེ་མིན
    {
        Mat temp;
        src.convertTo( temp, rtype, scale, shift );
        temp.copyTo( _dst, _mask );
    }
}

}  // namespace
