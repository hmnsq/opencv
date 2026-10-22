// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html
//
// Copyright (C) 2026, Advanced Micro Devices, Inc., all rights reserved.


#ضمن "precomp.hpp"
#ضمن "opencl_kernels_core.hpp"
#ضمن "stat.hpp"

#ضمن "norm.simd.hpp"
#ضمن "norm.simd_declarations.hpp"

/****************************************************************************************\
*                                         norm                                           *
\****************************************************************************************/

نطاق cv { نطاق hal {

خارجي ثابت uchar popCountTable[256] =
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

ساكن ثابت uchar popCountTable2[] =
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

ساكن ثابت uchar popCountTable4[] =
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


صحيح normHamming(ثابت uchar* a, صحيح n, صحيح cellSize)
{
    صحيح output;
    CALL_HAL_RET(normHamming8u, cv_hal_normHamming8u, output, a, n, cellSize);

    אם( cellSize == 1 )
        החזר normHamming(a, n);
    ثابت uchar* tab = 0;
    אם( cellSize == 2 )
        tab = popCountTable2;
    אחרת אם( cellSize == 4 )
        tab = popCountTable4;
    אחרת
        החזר -1;
    صحيح i = 0;
    صحيح result = 0;
#אם (CV_SIMD || CV_SIMD_SCALABLE)
    v_uint64 t = vx_setzero_u64();
    אם ( cellSize == 2)
    {
        v_uint16 mask = v_reinterpret_as_u16(vx_setall_u8(0x55));
        עבור(; i <= n - VTraits<v_uint8>::vlanes(); i += VTraits<v_uint8>::vlanes())
        {
            v_uint16 a0 = v_reinterpret_as_u16(vx_load(a + i));
            t = v_add(t, v_popcount(v_reinterpret_as_u64(v_and(v_or(a0, v_shr<1>(a0)), mask))));
        }
    }
    אחרת    // cellSize == 4
    {
        v_uint16 mask = v_reinterpret_as_u16(vx_setall_u8(0x11));
        עבור(; i <= n - VTraits<v_uint8>::vlanes(); i += VTraits<v_uint8>::vlanes())
        {
            v_uint16 a0 = v_reinterpret_as_u16(vx_load(a + i));
            v_uint16 a1 = v_or(a0, v_shr<2>(a0));
            t = v_add(t, v_popcount(v_reinterpret_as_u64(v_and(v_or(a1, v_shr<1>(a1)), mask))));

        }
    }
    result += (صحيح)v_reduce_sum(t);
    vx_cleanup();
#وإلاإذا CV_ENABLE_UNROLLED
    עבור( ; i <= n - 4; i += 4 )
        result += tab[a[i]] + tab[a[i+1]] + tab[a[i+2]] + tab[a[i+3]];
#endif
    עבור( ; i < n; i++ )
        result += tab[a[i]];
    החזר result;
}

صحيح normHamming(ثابت uchar* a, ثابت uchar* b, صحيح n, صحيح cellSize)
{
    صحيح output;
    CALL_HAL_RET(normHammingDiff8u, cv_hal_normHammingDiff8u, output, a, b, n, cellSize);

    אם( cellSize == 1 )
        החזר normHamming(a, b, n);
    ثابت uchar* tab = 0;
    אם( cellSize == 2 )
        tab = popCountTable2;
    אחרת אם( cellSize == 4 )
        tab = popCountTable4;
    אחרת
        החזר -1;
    صحيح i = 0;
    صحيح result = 0;
#אם (CV_SIMD || CV_SIMD_SCALABLE)
    v_uint64 t = vx_setzero_u64();
    אם ( cellSize == 2)
    {
        v_uint16 mask = v_reinterpret_as_u16(vx_setall_u8(0x55));
        עבור(; i <= n - VTraits<v_uint8>::vlanes(); i += VTraits<v_uint8>::vlanes())
        {
            v_uint16 ab0 = v_reinterpret_as_u16(v_xor(vx_load(a + i), vx_load(b + i)));
            t = v_add(t, v_popcount(v_reinterpret_as_u64(v_and(v_or(ab0, v_shr<1>(ab0)), mask))));
        }
    }
    אחרת    // cellSize == 4
    {
        v_uint16 mask = v_reinterpret_as_u16(vx_setall_u8(0x11));
        עבור(; i <= n - VTraits<v_uint8>::vlanes(); i += VTraits<v_uint8>::vlanes())
        {
            v_uint16 ab0 = v_reinterpret_as_u16(v_xor(vx_load(a + i), vx_load(b + i)));
            v_uint16 ab1 = v_or(ab0, v_shr<2>(ab0));
            t = v_add(t, v_popcount(v_reinterpret_as_u64(v_and(v_or(ab1, v_shr<1>(ab1)), mask))));
        }
    }
    result += (صحيح)v_reduce_sum(t);
    vx_cleanup();
#وإلاإذا CV_ENABLE_UNROLLED
    עבור( ; i <= n - 4; i += 4 )
        result += tab[a[i] ^ b[i]] + tab[a[i+1] ^ b[i+1]] +
                tab[a[i+2] ^ b[i+2]] + tab[a[i+3] ^ b[i+3]];
#endif
    עבור( ; i < n; i++ )
        result += tab[a[i] ^ b[i]];
    החזר result;
}

عائم normL2Sqr_(ثابت عائم* a, ثابت عائم* b, صحيح n)
{
    صحيح j = 0; عائم d = 0.f;
#אם (CV_SIMD || CV_SIMD_SCALABLE)
    ثابت صحيح vl = VTraits<v_float32>::vlanes();
    v_float32 v_d0 = vx_setzero_f32(), v_d1 = vx_setzero_f32();
    v_float32 v_d2 = vx_setzero_f32(), v_d3 = vx_setzero_f32();
    עבור (; j <= n - 4 * vl; j += 4 * vl)
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
    עבור (; j <= n - vl; j += vl)
    {
        v_float32 t0 = v_sub(vx_load(a + j), vx_load(b + j));
        v_d0 = v_muladd(t0, t0, v_d0);
    }
    d = v_reduce_sum(v_d0);
#endif
    עבור( ; j < n; j++ )
    {
        عائم t = a[j] - b[j];
        d += t*t;
    }
    החזר d;
}


عائم normL1_(ثابت عائم* a, ثابت عائم* b, صحيح n)
{
    صحيح j = 0; عائم d = 0.f;
#אם (CV_SIMD || CV_SIMD_SCALABLE)
    ثابت صحيح vl = VTraits<v_float32>::vlanes();
    v_float32 v_d0 = vx_setzero_f32(), v_d1 = vx_setzero_f32();
    v_float32 v_d2 = vx_setzero_f32(), v_d3 = vx_setzero_f32();
    עבור (; j <= n - 4 * vl; j += 4 * vl)
    {
        v_d0 = v_add(v_d0, v_absdiff(vx_load(a + j), vx_load(b + j)));
        v_d1 = v_add(v_d1, v_absdiff(vx_load(a + j + vl), vx_load(b + j + vl)));
        v_d2 = v_add(v_d2, v_absdiff(vx_load(a + j + 2 * vl), vx_load(b + j + 2 * vl)));
        v_d3 = v_add(v_d3, v_absdiff(vx_load(a + j + 3 * vl), vx_load(b + j + 3 * vl)));
    }
    v_d0 = v_add(v_add(v_d0, v_d1), v_add(v_d2, v_d3));
    עבור (; j <= n - vl; j += vl)
        v_d0 = v_add(v_d0, v_absdiff(vx_load(a + j), vx_load(b + j)));
    d = v_reduce_sum(v_d0);
#endif
    עבור( ; j < n; j++ )
        d += std::abs(a[j] - b[j]);
    החזר d;
}

صحيح normL1_(ثابت uchar* a, ثابت uchar* b, صحيح n)
{
    صحيح j = 0, d = 0;
#אם (CV_SIMD || CV_SIMD_SCALABLE)
    ثابت صحيح vl = VTraits<v_uint8>::vlanes();
    עבור (; j <= n - 4 * vl; j += 4 * vl)
        d += v_reduce_sad(vx_load(a + j), vx_load(b + j)) +
             v_reduce_sad(vx_load(a + j + vl), vx_load(b + j + vl)) +
             v_reduce_sad(vx_load(a + j + 2 * vl), vx_load(b + j + 2 * vl)) +
             v_reduce_sad(vx_load(a + j + 3 * vl), vx_load(b + j + 3 * vl));
    עבור (; j <= n - vl; j += vl)
        d += v_reduce_sad(vx_load(a + j), vx_load(b + j));
#endif
    עבור( ; j < n; j++ )
        d += std::abs(a[j] - b[j]);
    החזר d;
}

} //cv::hal

//==================================================================================================

تعريفنوع صحيح (*NormFunc)(ثابت uchar*, ثابت uchar*, uchar*, صحيح, صحيح);
تعريفنوع صحيح (*NormDiffFunc)(ثابت uchar*, ثابت uchar*, ثابت uchar*, uchar*, صحيح, صحيح);

#ifdef HAVE_OPENCL

ساكن منطقي ocl_norm( InputArray _src, صحيح normType, InputArray _mask, مزدوج & result )
{
    ثابت ocl::Device & d = ocl::Device::getDefault();

#ifdef __ANDROID__
    אם (d.isNVidia())
        החזר שקר;
#endif
    ثابت صحيح cn = _src.channels();
    אם (cn > 4)
        החזר שקר;
    صحيح type = _src.type(), depth = CV_MAT_DEPTH(type);
    منطقي doubleSupport = d.doubleFPConfig() > 0,
            haveMask = _mask.kind() != _InputArray::NONE;

    אם (depth >= CV_16F)
        החזר שקר;  // TODO: support FP16

    אם ( !(normType == NORM_INF || normType == NORM_L1 || normType == NORM_L2 || normType == NORM_L2SQR) ||
         (!doubleSupport && depth == CV_64F))
        החזר שקר;

    UMat src = _src.getUMat();

    אם (normType == NORM_INF)
    {
        אם (!ocl_minMaxIdx(_src, عدم, &result, عدم, عدم, _mask,
                           std::max(depth, CV_32S), depth != CV_8U && depth != CV_16U))
            החזר שקר;
    }
    אחרת אם (normType == NORM_L1 || normType == NORM_L2 || normType == NORM_L2SQR)
    {
        Scalar sc;
        منطقي unstype = depth == CV_8U || depth == CV_16U;

        אם ( !ocl_sum(haveMask ? src : src.reshape(1), sc, normType == NORM_L2 || normType == NORM_L2SQR ?
                    OCL_OP_SUM_SQR : (unstype ? OCL_OP_SUM : OCL_OP_SUM_ABS), _mask) )
            החזר שקר;

        مزدوج s = 0.0;
        עבור (صحيح i = 0; i < (haveMask ? cn : 1); ++i)
            s += sc[i];

        result = normType == NORM_L1 || normType == NORM_L2SQR ? s : std::sqrt(s);
    }

    החזר אמת;
}

#endif

ساكن NormFunc getNormFunc(صحيح normType, صحيح depth) {
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getNormFunc, (normType, depth), CV_CPU_DISPATCH_MODES_ALL);
}
ساكن NormDiffFunc getNormDiffFunc(صحيح normType, صحيح depth) {
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getNormDiffFunc, (normType, depth), CV_CPU_DISPATCH_MODES_ALL);
}

مزدوج norm( InputArray _src, صحيح normType, InputArray _mask )
{
    CV_INSTRUMENT_REGION();

    normType &= NORM_TYPE_MASK;
    CV_Assert( normType == NORM_INF || normType == NORM_L1 ||
               normType == NORM_L2 || normType == NORM_L2SQR ||
               ((normType == NORM_HAMMING || normType == NORM_HAMMING2) && _src.type() == CV_8U) );

#אם defined HAVE_OPENCL
    مزدوج _result = 0;
#endif

#ifdef HAVE_OPENCL
    CV_OCL_RUN_(OCL_PERFORMANCE_CHECK(_src.isUMat()) && _src.dims() <= 2,
                ocl_norm(_src, normType, _mask, _result),
                _result)
#endif

    Mat src = _src.getMat(), mask = _mask.getMat();
    CV_Assert( mask.empty() || ((mask.type() == CV_8U || mask.type() == CV_8S || mask.type() == CV_Bool) && src.size == mask.size) );
    אם( src.empty() )
        החזר 0.0;
    صحيح depth = src.depth(), cn = src.channels();
    אם( src.dims <= 2 )
    {
        مزدوج result;
        CALL_HAL_RET(norm, cv_hal_norm, result, src.data, src.step, mask.data, mask.step, src.cols, src.rows, src.type(), normType);
    }
    אחרת אם( src.isContinuous() && mask.isContinuous() )
    {
        مزدوج result;
        CALL_HAL_RET(norm, cv_hal_norm, result, src.data, 0, mask.data, 0, (صحيح)src.total(), 1, src.type(), normType);
    }

    NormFunc func = getNormFunc(normType >> 1, depth);
    CV_Assert( (normType >> 1) >= 3 || func != 0 );

    אם( src.isContinuous() && mask.empty() )
    {
        size_t len = src.total()*cn;
        אם( len == (size_t)(صحيح)len )
        {
            אם( depth == CV_32F )
            {
                ثابت uchar* data = src.ptr<ثابت uchar>();

                אם( normType == NORM_L2 || normType == NORM_L2SQR || normType == NORM_L1 )
                {
                    مزدوج result = 0;
                    func(data, 0, (uchar*)&result, (صحيح)len, 1);
                    החזר normType == NORM_L2 ? std::sqrt(result) : result;
                }
                אם( normType == NORM_INF )
                {
                    عائم result = 0;
                    func(data, 0, (uchar*)&result, (صحيح)len, 1);
                    החזר result;
                }
            }
            אם( depth == CV_8U )
            {
                ثابت uchar* data = src.ptr<uchar>();

                אם( normType == NORM_HAMMING )
                {
                    החזר hal::normHamming(data, (صحيح)len, 1);
                }

                אם( normType == NORM_HAMMING2 )
                {
                    החזר hal::normHamming(data, (صحيح)len, 2);
                }
            }
        }
    }

    CV_Assert( mask.empty() || mask.type() == CV_8U || mask.type() == CV_8S || mask.type() == CV_Bool );

    אם( normType == NORM_HAMMING || normType == NORM_HAMMING2 )
    {
        אם( !mask.empty() )
        {
            Mat temp;
            bitwise_and(src, mask, temp);
            החזר norm(temp, normType);
        }
        صحيح cellSize = normType == NORM_HAMMING ? 1 : 2;

        ثابت Mat* arrays[] = {&src, 0};
        uchar* ptrs[1] = {};
        NAryMatIterator it(arrays, ptrs);
        صحيح total = (صحيح)it.size;
        صحيح result = 0;

        עבור( size_t i = 0; i < it.nplanes; i++, ++it )
        {
            result += hal::normHamming(ptrs[0], total, cellSize);
        }

        החזר result;
    }

    ثابت Mat* arrays[] = {&src, &mask, 0};
    uchar* ptrs[2] = {};
    اتحاد
    {
        مزدوج d;
        unsigned u;
        uint64 UL;
        عائم f;
    }
    result;
    result.d = 0;
    NAryMatIterator it(arrays, ptrs);
    CV_CheckLT((size_t)it.size, (size_t)INT_MAX, "");
    منطقي is_fp16 = depth == CV_16F || depth == CV_16BF;

    אם ((normType == NORM_L1 && (depth <= CV_16S || depth == CV_Bool || is_fp16)) ||
        ((normType == NORM_L2 || normType == NORM_L2SQR) && (depth <= CV_8S || depth == CV_Bool || is_fp16)))
    {
        // special case to handle "integer" overflow in accumulator
        ثابت size_t esz = src.elemSize();
        ثابت صحيح total = (صحيح)it.size;
        ثابت صحيح blockSize0 = (is_fp16 ? (1 << 10) :
            depth == CV_Bool ? (1 << 30) :
            normType == NORM_L1 && depth <= CV_8S ? (1 << 23) : (1 << 15))/cn;
        ثابت صحيح blockSize = std::min(total, blockSize0);
        اتحاد {
            صحيح i;
            عائم f;
        } blocksum;
        blocksum.i = 0;
        صحيح count = 0;

        עבור (size_t i = 0; i < it.nplanes; i++, ++it)
        {
            עבור (صحيح j = 0; j < total; j += blockSize)
            {
                صحيح bsz = std::min(total - j, blockSize);
                func(ptrs[0], ptrs[1], (uchar*)&blocksum, bsz, cn);
                count += bsz;
                אם (count + blockSize >= blockSize0 || (i+1 >= it.nplanes && j+bsz >= total))
                {
                    result.d += is_fp16 ? (مزدوج)blocksum.f : (مزدوج)blocksum.i;
                    blocksum.i = 0;
                    count = 0;
                }
                ptrs[0] += bsz*esz;
                אם (ptrs[1])
                    ptrs[1] += bsz;
            }
        }
    }
    אחרת
    {
        // generic implementation
        עבור (size_t i = 0; i < it.nplanes; i++, ++it)
        {
            func(ptrs[0], ptrs[1], (uchar*)&result, (صحيح)it.size, cn);
        }
    }

    אם( normType == NORM_INF )
    {
        אם(depth <= CV_32S || depth == CV_32U || depth == CV_Bool)
            החזר result.u;
        אם (depth == CV_32F || is_fp16)
            החזר result.f;
        אם (depth == CV_64U || depth == CV_64S)
            החזר (مزدوج)result.UL;
    }
    אם( normType == NORM_L2 )
        החזר std::sqrt(result.d);

    החזר result.d;
}

//==================================================================================================

#ifdef HAVE_OPENCL
ساكن منطقي ocl_norm( InputArray _src1, InputArray _src2, صحيح normType, InputArray _mask, مزدوج & result )
{
#ifdef __ANDROID__
    אם (ocl::Device::getDefault().isNVidia())
        החזר שקר;
#endif

    Scalar sc1, sc2;
    صحيح cn = _src1.channels();
    אם (cn > 4)
        החזר שקר;
    صحيح type = _src1.type(), depth = CV_MAT_DEPTH(type);
    منطقي relative = (normType & NORM_RELATIVE) != 0;
    normType &= ~NORM_RELATIVE;
    منطقي normsum = normType == NORM_L1 || normType == NORM_L2 || normType == NORM_L2SQR;

#ifdef __APPLE__
    אם(normType == NORM_L1 && type == CV_16UC3 && !_mask.empty())
        החזר שקר;
#endif

    אם (normsum)
    {
        אם (!ocl_sum(_src1, sc1, normType == NORM_L2 || normType == NORM_L2SQR ?
                     OCL_OP_SUM_SQR : OCL_OP_SUM, _mask, _src2, relative, sc2))
            החזר שקר;
    }
    אחרת
    {
        אם (!ocl_minMaxIdx(_src1, عدم, &sc1[0], عدم, عدم, _mask, std::max(CV_32S, depth),
                           שקר, _src2, relative ? &sc2[0] : عدم))
            החזר שקר;
        cn = 1;
    }

    مزدوج s2 = 0;
    עבור (صحيح i = 0; i < cn; ++i)
    {
        result += sc1[i];
        אם (relative)
            s2 += sc2[i];
    }

    אם (normType == NORM_L2)
    {
        result = std::sqrt(result);
        אם (relative)
            s2 = std::sqrt(s2);
    }

    אם (relative)
        result /= (s2 + DBL_EPSILON);

    החזר אמת;
}  // ocl_norm()
#endif  // HAVE_OPENCL

مزدوج norm( InputArray _src1, InputArray _src2, صحيح normType, InputArray _mask )
{
    CV_INSTRUMENT_REGION();

    CV_CheckTypeEQ(_src1.type(), _src2.type(), "Input type mismatch");
    CV_Assert(_src1.sameSize(_src2));

#אם defined HAVE_OPENCL
    مزدوج _result = 0;
#endif

#ifdef HAVE_OPENCL
    CV_OCL_RUN_(OCL_PERFORMANCE_CHECK(_src1.isUMat()),
                ocl_norm(_src1, _src2, normType, _mask, _result),
                _result)
#endif

    Mat src1 = _src1.getMat(), src2 = _src2.getMat(), mask = _mask.getMat();
    CV_Assert( mask.empty() || ((mask.type() == CV_8U || mask.type() == CV_8S || mask.type() == CV_Bool) && src1.size == mask.size) );
    אם( src1.empty() || src2.empty() )
        החזר 0.0;
    صحيح depth = src1.depth(), cn = src1.channels();
    אם( src1.dims <= 2 )
    {
        مزدوج result;
        CALL_HAL_RET(normDiff, cv_hal_normDiff, result, src1.data, src1.step, src2.data, src2.step, mask.data, mask.step, src1.cols, src1.rows, src1.type(), normType);
    }
    אחרת אם( src1.isContinuous() && src2.isContinuous() && mask.isContinuous() )
    {
        مزدوج result;
        CALL_HAL_RET(normDiff, cv_hal_normDiff, result, src1.data, 0, src2.data, 0, mask.data, 0, (صحيح)src1.total(), 1, src1.type(), normType);
    }

    אם( normType & NORM_RELATIVE )
    {
        החזר norm(_src1, _src2, normType & ~NORM_RELATIVE, _mask)/(norm(_src2, normType, _mask) + DBL_EPSILON);
    }

    normType &= 7;
    CV_Assert( normType == NORM_INF || normType == NORM_L1 ||
               normType == NORM_L2 || normType == NORM_L2SQR ||
              ((normType == NORM_HAMMING || normType == NORM_HAMMING2) && src1.type() == CV_8U) );

    NormDiffFunc func = getNormDiffFunc(normType >> 1, depth);
    CV_Assert( (normType >> 1) >= 3 || func != 0 );

    אם( src1.isContinuous() && src2.isContinuous() && mask.empty() )
    {
        size_t len = src1.total()*src1.channels();
        אם( len == (size_t)(صحيح)len )
        {
            אם( src1.depth() == CV_32F )
            {
                ثابت uchar* data1 = src1.ptr<ثابت uchar>();
                ثابت uchar* data2 = src2.ptr<ثابت uchar>();

                אם( normType == NORM_L2 || normType == NORM_L2SQR || normType == NORM_L1 )
                {
                    مزدوج result = 0;
                    func(data1, data2, 0, (uchar*)&result, (صحيح)len, 1);
                    החזר normType == NORM_L2 ? std::sqrt(result) : result;
                }
                אם( normType == NORM_INF )
                {
                    عائم result = 0;
                    func(data1, data2, 0, (uchar*)&result, (صحيح)len, 1);
                    החזר result;
                }
            }
        }
    }

    CV_Assert( mask.empty() || mask.type() == CV_8U || mask.type() == CV_8S || mask.type() == CV_Bool );

    אם( normType == NORM_HAMMING || normType == NORM_HAMMING2 )
    {
        אם( !mask.empty() )
        {
            Mat temp;
            bitwise_xor(src1, src2, temp);
            bitwise_and(temp, mask, temp);
            החזר norm(temp, normType);
        }
        صحيح cellSize = normType == NORM_HAMMING ? 1 : 2;

        ثابت Mat* arrays[] = {&src1, &src2, 0};
        uchar* ptrs[2] = {};
        NAryMatIterator it(arrays, ptrs);
        صحيح total = (صحيح)it.size;
        صحيح result = 0;

        עבור( size_t i = 0; i < it.nplanes; i++, ++it )
        {
            result += hal::normHamming(ptrs[0], ptrs[1], total, cellSize);
        }

        החזר result;
    }

    ثابت Mat* arrays[] = {&src1, &src2, &mask, 0};
    uchar* ptrs[3] = {};
    اتحاد
    {
        مزدوج d;
        عائم f;
        unsigned u;
        uint64 UL;
    }
    result;
    result.d = 0;
    NAryMatIterator it(arrays, ptrs);
    CV_CheckLT((size_t)it.size, (size_t)INT_MAX, "");

    منطقي is_fp16 = depth == CV_16F || depth == CV_16BF;

    אם ((normType == NORM_L1 && (depth <= CV_16S || depth == CV_Bool || is_fp16)) ||
        ((normType == NORM_L2 || normType == NORM_L2SQR) && (depth <= CV_8S || depth == CV_Bool || is_fp16)))
    {
        // special case to handle "integer" overflow in accumulator
        ثابت size_t esz = src1.elemSize();
        ثابت صحيح total = (صحيح)it.size;
        ثابت صحيح blockSize0 = (is_fp16 ? (1 << 10) :
            depth == CV_Bool ? (1 << 30) :
            normType == NORM_L1 && depth <= CV_8S ? (1 << 23) : (1 << 15))/cn;
        ثابت صحيح blockSize = std::min(total, blockSize0);
        اتحاد {
            صحيح i;
            عائم f;
        } blocksum;
        blocksum.i = 0;
        صحيح count = 0;

        עבור (size_t i = 0; i < it.nplanes; i++, ++it)
        {
            עבור (صحيح j = 0; j < total; j += blockSize)
            {
                صحيح bsz = std::min(total - j, blockSize);
                func(ptrs[0], ptrs[1], ptrs[2], (uchar*)&blocksum, bsz, cn);
                count += bsz;
                אם (count + blockSize >= blockSize0 || (i+1 >= it.nplanes && j+bsz >= total))
                {
                    result.d += is_fp16 ? (مزدوج)blocksum.f : (مزدوج)blocksum.i;
                    blocksum.i = 0;
                    count = 0;
                }
                ptrs[0] += bsz*esz;
                ptrs[1] += bsz*esz;
                אם (ptrs[2])
                    ptrs[2] += bsz;
            }
        }
    }
    אחרת
    {
        // generic implementation
        עבור (size_t i = 0; i < it.nplanes; i++, ++it)
        {
            func(ptrs[0], ptrs[1], ptrs[2], (uchar*)&result, (صحيح)it.size, cn);
        }
    }

    אם( normType == NORM_INF )
    {
        אם (depth <= CV_32S || depth == CV_32U || depth == CV_Bool)
            החזר result.u;
        אם (depth == CV_32F || is_fp16)
            החזר result.f;
        אם (depth == CV_64U || depth == CV_64S)
            החזר (مزدوج)result.UL;
    }
    אם( normType == NORM_L2 )
        החזר std::sqrt(result.d);

    החזר result.d;
}

cv::Hamming::ResultType Hamming::operator()( ثابت unsigned حرف* a, ثابت unsigned حرف* b, صحيح size ) ثابت
{
    החזר cv::hal::normHamming(a, b, size);
}

مزدوج PSNR(InputArray _src1, InputArray _src2, مزدوج R)
{
    CV_INSTRUMENT_REGION();

    //Input arrays must have depth CV_8U
    CV_Assert( _src1.type() == _src2.type() );

    مزدوج diff = std::sqrt(norm(_src1, _src2, NORM_L2SQR)/(_src1.total()*_src1.channels()));
    החזר 20*log10(R/(diff+DBL_EPSILON));
}


#ifdef HAVE_OPENCL
ساكن منطقي ocl_normalize( InputArray _src, InputOutputArray _dst, InputArray _mask, صحيح dtype,
                           مزدوج scale, مزدوج delta )
{
    UMat src = _src.getUMat();

    אם( _mask.empty() )
        src.convertTo( _dst, dtype, scale, delta );
    אחרת אם (src.channels() <= 4)
    {
        ثابت ocl::Device & dev = ocl::Device::getDefault();

        صحيح stype = _src.type(), sdepth = CV_MAT_DEPTH(stype), cn = CV_MAT_CN(stype),
                ddepth = CV_MAT_DEPTH(dtype), wdepth = std::max(CV_32F, std::max(sdepth, ddepth)),
                rowsPerWI = dev.isIntel() ? 4 : 1;

        عائم fscale = static_cast<عائم>(scale), fdelta = static_cast<عائم>(delta);
        منطقي haveScale = std::fabs(scale - 1) > DBL_EPSILON,
                haveZeroScale = !(std::fabs(scale) > DBL_EPSILON),
                haveDelta = std::fabs(delta) > DBL_EPSILON,
                doubleSupport = dev.doubleFPConfig() > 0;

        אם (!haveScale && !haveDelta && stype == dtype)
        {
            _src.copyTo(_dst, _mask);
            החזר אמת;
        }
        אם (haveZeroScale)
        {
            _dst.setTo(Scalar(delta), _mask);
            החזר אמת;
        }

        אם ((sdepth == CV_64F || ddepth == CV_64F) && !doubleSupport)
            החזר שקר;

        حرف cvt[2][50];
        String opts = format("-D srcT=%s -D dstT=%s -D convertToWT=%s -D cn=%d -D rowsPerWI=%d"
                             " -D convertToDT=%s -D workT=%s%s%s%s -D srcT1=%s -D dstT1=%s",
                             ocl::typeToStr(stype), ocl::typeToStr(dtype),
                             ocl::convertTypeStr(sdepth, wdepth, cn, cvt[0], حجم(cvt[0])), cn,
                             rowsPerWI, ocl::convertTypeStr(wdepth, ddepth, cn, cvt[1], حجم(cvt[1])),
                             ocl::typeToStr(CV_MAKE_TYPE(wdepth, cn)),
                             doubleSupport ? " -D DOUBLE_SUPPORT" : "",
                             haveScale ? " -D HAVE_SCALE" : "",
                             haveDelta ? " -D HAVE_DELTA" : "",
                             ocl::typeToStr(sdepth), ocl::typeToStr(ddepth));

        ocl::Kernel k("normalizek", ocl::core::normalize_oclsrc, opts);
        אם (k.empty())
            החזר שקר;

        UMat mask = _mask.getUMat(), dst = _dst.getUMat();

        ocl::KernelArg srcarg = ocl::KernelArg::ReadOnlyNoSize(src),
                maskarg = ocl::KernelArg::ReadOnlyNoSize(mask),
                dstarg = ocl::KernelArg::ReadWrite(dst);

        אם (haveScale)
        {
            אם (haveDelta)
                k.args(srcarg, maskarg, dstarg, fscale, fdelta);
            אחרת
                k.args(srcarg, maskarg, dstarg, fscale);
        }
        אחרת
        {
            אם (haveDelta)
                k.args(srcarg, maskarg, dstarg, fdelta);
            אחרת
                k.args(srcarg, maskarg, dstarg);
        }

        size_t globalsize[2] = { (size_t)src.cols, ((size_t)src.rows + rowsPerWI - 1) / rowsPerWI };
        החזר k.run(2, globalsize, عدم, שקר);
    }
    אחרת
    {
        UMat temp;
        src.convertTo( temp, dtype, scale, delta );
        temp.copyTo( _dst, _mask );
    }

    החזר אמת;
}  // ocl_normalize
#endif  // HAVE_OPENCL

فراغ normalize(InputArray _src, InputOutputArray _dst, مزدوج a, مزدوج b,
               صحيح norm_type, صحيح rtype, InputArray _mask)
{
    CV_INSTRUMENT_REGION();

    مزدوج scale = 1, shift = 0;
    صحيح type = _src.type(), depth = CV_MAT_DEPTH(type);

    אם( rtype < 0 )
        rtype = _dst.fixedType() ? _dst.depth() : depth;

    אם( norm_type == NORM_MINMAX )
    {
        مزدوج smin = 0, smax = 0;
        مزدوج dmin = MIN( a, b ), dmax = MAX( a, b );
        minMaxIdx( _src, &smin, &smax, 0, 0, _mask );
        scale = (dmax - dmin)*(smax - smin > DBL_EPSILON ? 1./(smax - smin) : 0);
        אם( rtype == CV_32F )
        {
            scale = (عائم)scale;
            shift = (عائم)dmin - (عائم)(smin*scale);
        }
        אחרת
            shift = dmin - smin*scale;
    }
    אחרת אם( norm_type == NORM_L2 || norm_type == NORM_L1 || norm_type == NORM_INF )
    {
        scale = norm( _src, norm_type, _mask );
        scale = scale > DBL_EPSILON ? a/scale : 0.;
        shift = 0;
    }
    אחרת
        CV_Error( cv::Error::StsBadArg, "Unknown/unsupported norm type" );

    CV_OCL_RUN(_dst.isUMat(),
               ocl_normalize(_src, _dst, _mask, rtype, scale, shift))

    Mat src = _src.getMat();
    אם( _mask.empty() )
        src.convertTo( _dst, rtype, scale, shift );
    אחרת
    {
        Mat temp;
        src.convertTo( temp, rtype, scale, shift );
        temp.copyTo( _dst, _mask );
    }
}

}  // namespace
