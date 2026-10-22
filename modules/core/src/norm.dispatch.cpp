// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html
//
// Copyright (C) 2026, Advanced Micro Devices, Inc., all rights reserved.


#شامل "precomp.hpp"
#شامل "opencl_kernels_core.hpp"
#شامل "stat.hpp"

#شامل "norm.simd.hpp"
#شامل "norm.simd_declarations.hpp"

/****************************************************************************************\
*                                         norm                                           *
\****************************************************************************************/

فضاینام cv { فضاینام hal {

بیرونی ثابت uchar برداشت_شمار_Table[256] =
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

ایستا ثابت uchar برداشت_شمار_Table2[] =
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

ایستا ثابت uchar برداشت_شمار_Table4[] =
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


صحیح normHamming(ثابت uchar* a, صحیح n, صحیح cellSize)
{
    صحیح output;
    CALL_HAL_RET(normHamming8u, cv_hal_normHamming8u, output, a, n, cellSize);

    اگر( cellSize == 1 )
        بازگشت normHamming(a, n);
    ثابت uchar* tab = 0;
    اگر( cellSize == 2 )
        tab = برداشت_شمار_Table2;
    وگرنه اگر( cellSize == 4 )
        tab = برداشت_شمار_Table4;
    وگرنه
        بازگشت -1;
    صحیح i = 0;
    صحیح result = 0;
#اگر (CV_SIMD || CV_SIMD_SCALABLE)
    v_uint64 t = vx_setzero_u64();
    اگر ( cellSize == 2)
    {
        v_uint16 mask = v_reinterpret_as_u16(vx_setall_u8(0x55));
        برای(; i <= n - VTraits<v_uint8>::vlanes(); i += VTraits<v_uint8>::vlanes())
        {
            v_uint16 a0 = v_reinterpret_as_u16(vx_load(a + i));
            t = v_add(t, v_popcount(v_reinterpret_as_u64(v_and(v_or(a0, v_shr<1>(a0)), mask))));
        }
    }
    وگرنه    // cellSize == 4
    {
        v_uint16 mask = v_reinterpret_as_u16(vx_setall_u8(0x11));
        برای(; i <= n - VTraits<v_uint8>::vlanes(); i += VTraits<v_uint8>::vlanes())
        {
            v_uint16 a0 = v_reinterpret_as_u16(vx_load(a + i));
            v_uint16 a1 = v_or(a0, v_shr<2>(a0));
            t = v_add(t, v_popcount(v_reinterpret_as_u64(v_and(v_or(a1, v_shr<1>(a1)), mask))));

        }
    }
    result += (صحیح)v_reduce_sum(t);
    vx_cleanup();
#وگرنهاگر CV_ENABLE_UNROLLED
    برای( ; i <= n - 4; i += 4 )
        result += tab[a[i]] + tab[a[i+1]] + tab[a[i+2]] + tab[a[i+3]];
#endif
    برای( ; i < n; i++ )
        result += tab[a[i]];
    بازگشت result;
}

صحیح normHamming(ثابت uchar* a, ثابت uchar* b, صحیح n, صحیح cellSize)
{
    صحیح output;
    CALL_HAL_RET(normHammingDiff8u, cv_hal_normHammingDiff8u, output, a, b, n, cellSize);

    اگر( cellSize == 1 )
        بازگشت normHamming(a, b, n);
    ثابت uchar* tab = 0;
    اگر( cellSize == 2 )
        tab = برداشت_شمار_Table2;
    وگرنه اگر( cellSize == 4 )
        tab = برداشت_شمار_Table4;
    وگرنه
        بازگشت -1;
    صحیح i = 0;
    صحیح result = 0;
#اگر (CV_SIMD || CV_SIMD_SCALABLE)
    v_uint64 t = vx_setzero_u64();
    اگر ( cellSize == 2)
    {
        v_uint16 mask = v_reinterpret_as_u16(vx_setall_u8(0x55));
        برای(; i <= n - VTraits<v_uint8>::vlanes(); i += VTraits<v_uint8>::vlanes())
        {
            v_uint16 ab0 = v_reinterpret_as_u16(v_xor(vx_load(a + i), vx_load(b + i)));
            t = v_add(t, v_popcount(v_reinterpret_as_u64(v_and(v_or(ab0, v_shr<1>(ab0)), mask))));
        }
    }
    وگرنه    // cellSize == 4
    {
        v_uint16 mask = v_reinterpret_as_u16(vx_setall_u8(0x11));
        برای(; i <= n - VTraits<v_uint8>::vlanes(); i += VTraits<v_uint8>::vlanes())
        {
            v_uint16 ab0 = v_reinterpret_as_u16(v_xor(vx_load(a + i), vx_load(b + i)));
            v_uint16 ab1 = v_or(ab0, v_shr<2>(ab0));
            t = v_add(t, v_popcount(v_reinterpret_as_u64(v_and(v_or(ab1, v_shr<1>(ab1)), mask))));
        }
    }
    result += (صحیح)v_reduce_sum(t);
    vx_cleanup();
#وگرنهاگر CV_ENABLE_UNROLLED
    برای( ; i <= n - 4; i += 4 )
        result += tab[a[i] ^ b[i]] + tab[a[i+1] ^ b[i+1]] +
                tab[a[i+2] ^ b[i+2]] + tab[a[i+3] ^ b[i+3]];
#endif
    برای( ; i < n; i++ )
        result += tab[a[i] ^ b[i]];
    بازگشت result;
}

اعشاری normL2Sqr_(ثابت اعشاری* a, ثابت اعشاری* b, صحیح n)
{
    صحیح j = 0; اعشاری d = 0.f;
#اگر (CV_SIMD || CV_SIMD_SCALABLE)
    ثابت صحیح vl = VTraits<v_float32>::vlanes();
    v_float32 v_d0 = vx_setzero_f32(), v_d1 = vx_setzero_f32();
    v_float32 v_d2 = vx_setzero_f32(), v_d3 = vx_setzero_f32();
    برای (; j <= n - 4 * vl; j += 4 * vl)
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
    برای (; j <= n - vl; j += vl)
    {
        v_float32 t0 = v_sub(vx_load(a + j), vx_load(b + j));
        v_d0 = v_muladd(t0, t0, v_d0);
    }
    d = v_reduce_sum(v_d0);
#endif
    برای( ; j < n; j++ )
    {
        اعشاری t = a[j] - b[j];
        d += t*t;
    }
    بازگشت d;
}


اعشاری normL1_(ثابت اعشاری* a, ثابت اعشاری* b, صحیح n)
{
    صحیح j = 0; اعشاری d = 0.f;
#اگر (CV_SIMD || CV_SIMD_SCALABLE)
    ثابت صحیح vl = VTraits<v_float32>::vlanes();
    v_float32 v_d0 = vx_setzero_f32(), v_d1 = vx_setzero_f32();
    v_float32 v_d2 = vx_setzero_f32(), v_d3 = vx_setzero_f32();
    برای (; j <= n - 4 * vl; j += 4 * vl)
    {
        v_d0 = v_add(v_d0, v_absdiff(vx_load(a + j), vx_load(b + j)));
        v_d1 = v_add(v_d1, v_absdiff(vx_load(a + j + vl), vx_load(b + j + vl)));
        v_d2 = v_add(v_d2, v_absdiff(vx_load(a + j + 2 * vl), vx_load(b + j + 2 * vl)));
        v_d3 = v_add(v_d3, v_absdiff(vx_load(a + j + 3 * vl), vx_load(b + j + 3 * vl)));
    }
    v_d0 = v_add(v_add(v_d0, v_d1), v_add(v_d2, v_d3));
    برای (; j <= n - vl; j += vl)
        v_d0 = v_add(v_d0, v_absdiff(vx_load(a + j), vx_load(b + j)));
    d = v_reduce_sum(v_d0);
#endif
    برای( ; j < n; j++ )
        d += std::abs(a[j] - b[j]);
    بازگشت d;
}

صحیح normL1_(ثابت uchar* a, ثابت uchar* b, صحیح n)
{
    صحیح j = 0, d = 0;
#اگر (CV_SIMD || CV_SIMD_SCALABLE)
    ثابت صحیح vl = VTraits<v_uint8>::vlanes();
    برای (; j <= n - 4 * vl; j += 4 * vl)
        d += v_reduce_sad(vx_load(a + j), vx_load(b + j)) +
             v_reduce_sad(vx_load(a + j + vl), vx_load(b + j + vl)) +
             v_reduce_sad(vx_load(a + j + 2 * vl), vx_load(b + j + 2 * vl)) +
             v_reduce_sad(vx_load(a + j + 3 * vl), vx_load(b + j + 3 * vl));
    برای (; j <= n - vl; j += vl)
        d += v_reduce_sad(vx_load(a + j), vx_load(b + j));
#endif
    برای( ; j < n; j++ )
        d += std::abs(a[j] - b[j]);
    بازگشت d;
}

} //cv::hal

//==================================================================================================

تعریفنوع صحیح (*NormFunc)(ثابت uchar*, ثابت uchar*, uchar*, صحیح, صحیح);
تعریفنوع صحیح (*NormDiffFunc)(ثابت uchar*, ثابت uchar*, ثابت uchar*, uchar*, صحیح, صحیح);

#ifdef HAVE_OPENCL

ایستا منطقی ocl_norm( درون_داد_Array _src, صحیح normType, درون_داد_Array _mask, دوگانه & result )
{
    ثابت ocl::Device & d = ocl::Device::getDefault();

#ifdef __ANDROID__
    اگر (d.isNVidia())
        بازگشت نادرست;
#endif
    ثابت صحیح cn = _src.channels();
    اگر (cn > 4)
        بازگشت نادرست;
    صحیح type = _src.type(), depth = CV_MAT_DEPTH(type);
    منطقی doubleSupport = d.doubleFPConfig() > 0,
            haveMask = _mask.kind() != درون_داد_Array_2::NONE;

    اگر (depth >= CV_16F)
        بازگشت نادرست;  // TODO: support FP16

    اگر ( !(normType == NORM_INF || normType == NORM_L1 || normType == NORM_L2 || normType == NORM_L2SQR) ||
         (!doubleSupport && depth == CV_64F))
        بازگشت نادرست;

    UMat src = _src.getUMat();

    اگر (normType == NORM_INF)
    {
        اگر (!ocl_minMaxIdx(_src, پوچ, &result, پوچ, پوچ, _mask,
                           std::max(depth, CV_32S), depth != CV_8U && depth != CV_16U))
            بازگشت نادرست;
    }
    وگرنه اگر (normType == NORM_L1 || normType == NORM_L2 || normType == NORM_L2SQR)
    {
        Scalar sc;
        منطقی unstype = depth == CV_8U || depth == CV_16U;

        اگر ( !ocl_sum(haveMask ? src : src.reshape(1), sc, normType == NORM_L2 || normType == NORM_L2SQR ?
                    OCL_OP_SUM_SQR : (unstype ? OCL_OP_SUM : OCL_OP_SUM_ABS), _mask) )
            بازگشت نادرست;

        دوگانه s = 0.0;
        برای (صحیح i = 0; i < (haveMask ? cn : 1); ++i)
            s += sc[i];

        result = normType == NORM_L1 || normType == NORM_L2SQR ? s : std::sqrt(s);
    }

    بازگشت درست;
}

#endif

ایستا NormFunc getNormFunc(صحیح normType, صحیح depth) {
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getNormFunc, (normType, depth), CV_CPU_DISPATCH_MODES_ALL);
}
ایستا NormDiffFunc getNormDiffFunc(صحیح normType, صحیح depth) {
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getNormDiffFunc, (normType, depth), CV_CPU_DISPATCH_MODES_ALL);
}

دوگانه norm( درون_داد_Array _src, صحیح normType, درون_داد_Array _mask )
{
    CV_INSTRUMENT_REGION();

    normType &= NORM_TYPE_MASK;
    CV_Assert( normType == NORM_INF || normType == NORM_L1 ||
               normType == NORM_L2 || normType == NORM_L2SQR ||
               ((normType == NORM_HAMMING || normType == NORM_HAMMING2) && _src.type() == CV_8U) );

#اگر defined HAVE_OPENCL
    دوگانه _result = 0;
#endif

#ifdef HAVE_OPENCL
    CV_OCL_RUN_(OCL_PERFORMANCE_CHECK(_src.isUMat()) && _src.dims() <= 2,
                ocl_norm(_src, normType, _mask, _result),
                _result)
#endif

    Mat src = _src.getMat(), mask = _mask.getMat();
    CV_Assert( mask.empty() || ((mask.type() == CV_8U || mask.type() == CV_8S || mask.type() == CV_Bool) && src.size == mask.size) );
    اگر( src.empty() )
        بازگشت 0.0;
    صحیح depth = src.depth(), cn = src.channels();
    اگر( src.dims <= 2 )
    {
        دوگانه result;
        CALL_HAL_RET(norm, cv_hal_norm, result, src.data, src.step, mask.data, mask.step, src.cols, src.rows, src.type(), normType);
    }
    وگرنه اگر( src.isContinuous() && mask.isContinuous() )
    {
        دوگانه result;
        CALL_HAL_RET(norm, cv_hal_norm, result, src.data, 0, mask.data, 0, (صحیح)src.total(), 1, src.type(), normType);
    }

    NormFunc func = getNormFunc(normType >> 1, depth);
    CV_Assert( (normType >> 1) >= 3 || func != 0 );

    اگر( src.isContinuous() && mask.empty() )
    {
        size_t len = src.total()*cn;
        اگر( len == (size_t)(صحیح)len )
        {
            اگر( depth == CV_32F )
            {
                ثابت uchar* data = src.ptr<ثابت uchar>();

                اگر( normType == NORM_L2 || normType == NORM_L2SQR || normType == NORM_L1 )
                {
                    دوگانه result = 0;
                    func(data, 0, (uchar*)&result, (صحیح)len, 1);
                    بازگشت normType == NORM_L2 ? std::sqrt(result) : result;
                }
                اگر( normType == NORM_INF )
                {
                    اعشاری result = 0;
                    func(data, 0, (uchar*)&result, (صحیح)len, 1);
                    بازگشت result;
                }
            }
            اگر( depth == CV_8U )
            {
                ثابت uchar* data = src.ptr<uchar>();

                اگر( normType == NORM_HAMMING )
                {
                    بازگشت hal::normHamming(data, (صحیح)len, 1);
                }

                اگر( normType == NORM_HAMMING2 )
                {
                    بازگشت hal::normHamming(data, (صحیح)len, 2);
                }
            }
        }
    }

    CV_Assert( mask.empty() || mask.type() == CV_8U || mask.type() == CV_8S || mask.type() == CV_Bool );

    اگر( normType == NORM_HAMMING || normType == NORM_HAMMING2 )
    {
        اگر( !mask.empty() )
        {
            Mat temp;
            bitwise_and(src, mask, temp);
            بازگشت norm(temp, normType);
        }
        صحیح cellSize = normType == NORM_HAMMING ? 1 : 2;

        ثابت Mat* arrays[] = {&src, 0};
        uchar* ptrs[1] = {};
        NAryMatIterator it(arrays, ptrs);
        صحیح total = (صحیح)it.size;
        صحیح result = 0;

        برای( size_t i = 0; i < it.nplanes; i++, ++it )
        {
            result += hal::normHamming(ptrs[0], total, cellSize);
        }

        بازگشت result;
    }

    ثابت Mat* arrays[] = {&src, &mask, 0};
    uchar* ptrs[2] = {};
    اجتماع
    {
        دوگانه d;
        unsigned u;
        uint64 UL;
        اعشاری f;
    }
    result;
    result.d = 0;
    NAryMatIterator it(arrays, ptrs);
    CV_CheckLT((size_t)it.size, (size_t)INT_MAX, "");
    منطقی is_fp16 = depth == CV_16F || depth == CV_16BF;

    اگر ((normType == NORM_L1 && (depth <= CV_16S || depth == CV_Bool || is_fp16)) ||
        ((normType == NORM_L2 || normType == NORM_L2SQR) && (depth <= CV_8S || depth == CV_Bool || is_fp16)))
    {
        // special case to handle "integer" overflow in accumulator
        ثابت size_t esz = src.elemSize();
        ثابت صحیح total = (صحیح)it.size;
        ثابت صحیح blockSize0 = (is_fp16 ? (1 << 10) :
            depth == CV_Bool ? (1 << 30) :
            normType == NORM_L1 && depth <= CV_8S ? (1 << 23) : (1 << 15))/cn;
        ثابت صحیح blockSize = std::min(total, blockSize0);
        اجتماع {
            صحیح i;
            اعشاری f;
        } blocksum;
        blocksum.i = 0;
        صحیح شمار_2 = 0;

        برای (size_t i = 0; i < it.nplanes; i++, ++it)
        {
            برای (صحیح j = 0; j < total; j += blockSize)
            {
                صحیح bsz = std::min(total - j, blockSize);
                func(ptrs[0], ptrs[1], (uchar*)&blocksum, bsz, cn);
                شمار_2 += bsz;
                اگر (شمار_2 + blockSize >= blockSize0 || (i+1 >= it.nplanes && j+bsz >= total))
                {
                    result.d += is_fp16 ? (دوگانه)blocksum.f : (دوگانه)blocksum.i;
                    blocksum.i = 0;
                    شمار_2 = 0;
                }
                ptrs[0] += bsz*esz;
                اگر (ptrs[1])
                    ptrs[1] += bsz;
            }
        }
    }
    وگرنه
    {
        // generic implementation
        برای (size_t i = 0; i < it.nplanes; i++, ++it)
        {
            func(ptrs[0], ptrs[1], (uchar*)&result, (صحیح)it.size, cn);
        }
    }

    اگر( normType == NORM_INF )
    {
        اگر(depth <= CV_32S || depth == CV_32U || depth == CV_Bool)
            بازگشت result.u;
        اگر (depth == CV_32F || is_fp16)
            بازگشت result.f;
        اگر (depth == CV_64U || depth == CV_64S)
            بازگشت (دوگانه)result.UL;
    }
    اگر( normType == NORM_L2 )
        بازگشت std::sqrt(result.d);

    بازگشت result.d;
}

//==================================================================================================

#ifdef HAVE_OPENCL
ایستا منطقی ocl_norm( درون_داد_Array _src1, درون_داد_Array _src2, صحیح normType, درون_داد_Array _mask, دوگانه & result )
{
#ifdef __ANDROID__
    اگر (ocl::Device::getDefault().isNVidia())
        بازگشت نادرست;
#endif

    Scalar sc1, sc2;
    صحیح cn = _src1.channels();
    اگر (cn > 4)
        بازگشت نادرست;
    صحیح type = _src1.type(), depth = CV_MAT_DEPTH(type);
    منطقی relative = (normType & NORM_RELATIVE) != 0;
    normType &= ~NORM_RELATIVE;
    منطقی normsum = normType == NORM_L1 || normType == NORM_L2 || normType == NORM_L2SQR;

#ifdef __APPLE__
    اگر(normType == NORM_L1 && type == CV_16UC3 && !_mask.empty())
        بازگشت نادرست;
#endif

    اگر (normsum)
    {
        اگر (!ocl_sum(_src1, sc1, normType == NORM_L2 || normType == NORM_L2SQR ?
                     OCL_OP_SUM_SQR : OCL_OP_SUM, _mask, _src2, relative, sc2))
            بازگشت نادرست;
    }
    وگرنه
    {
        اگر (!ocl_minMaxIdx(_src1, پوچ, &sc1[0], پوچ, پوچ, _mask, std::max(CV_32S, depth),
                           نادرست, _src2, relative ? &sc2[0] : پوچ))
            بازگشت نادرست;
        cn = 1;
    }

    دوگانه s2 = 0;
    برای (صحیح i = 0; i < cn; ++i)
    {
        result += sc1[i];
        اگر (relative)
            s2 += sc2[i];
    }

    اگر (normType == NORM_L2)
    {
        result = std::sqrt(result);
        اگر (relative)
            s2 = std::sqrt(s2);
    }

    اگر (relative)
        result /= (s2 + DBL_EPSILON);

    بازگشت درست;
}  // ocl_norm()
#endif  // HAVE_OPENCL

دوگانه norm( درون_داد_Array _src1, درون_داد_Array _src2, صحیح normType, درون_داد_Array _mask )
{
    CV_INSTRUMENT_REGION();

    CV_CheckTypeEQ(_src1.type(), _src2.type(), "Input type mismatch");
    CV_Assert(_src1.sameSize(_src2));

#اگر defined HAVE_OPENCL
    دوگانه _result = 0;
#endif

#ifdef HAVE_OPENCL
    CV_OCL_RUN_(OCL_PERFORMANCE_CHECK(_src1.isUMat()),
                ocl_norm(_src1, _src2, normType, _mask, _result),
                _result)
#endif

    Mat src1 = _src1.getMat(), src2 = _src2.getMat(), mask = _mask.getMat();
    CV_Assert( mask.empty() || ((mask.type() == CV_8U || mask.type() == CV_8S || mask.type() == CV_Bool) && src1.size == mask.size) );
    اگر( src1.empty() || src2.empty() )
        بازگشت 0.0;
    صحیح depth = src1.depth(), cn = src1.channels();
    اگر( src1.dims <= 2 )
    {
        دوگانه result;
        CALL_HAL_RET(normDiff, cv_hal_normDiff, result, src1.data, src1.step, src2.data, src2.step, mask.data, mask.step, src1.cols, src1.rows, src1.type(), normType);
    }
    وگرنه اگر( src1.isContinuous() && src2.isContinuous() && mask.isContinuous() )
    {
        دوگانه result;
        CALL_HAL_RET(normDiff, cv_hal_normDiff, result, src1.data, 0, src2.data, 0, mask.data, 0, (صحیح)src1.total(), 1, src1.type(), normType);
    }

    اگر( normType & NORM_RELATIVE )
    {
        بازگشت norm(_src1, _src2, normType & ~NORM_RELATIVE, _mask)/(norm(_src2, normType, _mask) + DBL_EPSILON);
    }

    normType &= 7;
    CV_Assert( normType == NORM_INF || normType == NORM_L1 ||
               normType == NORM_L2 || normType == NORM_L2SQR ||
              ((normType == NORM_HAMMING || normType == NORM_HAMMING2) && src1.type() == CV_8U) );

    NormDiffFunc func = getNormDiffFunc(normType >> 1, depth);
    CV_Assert( (normType >> 1) >= 3 || func != 0 );

    اگر( src1.isContinuous() && src2.isContinuous() && mask.empty() )
    {
        size_t len = src1.total()*src1.channels();
        اگر( len == (size_t)(صحیح)len )
        {
            اگر( src1.depth() == CV_32F )
            {
                ثابت uchar* data1 = src1.ptr<ثابت uchar>();
                ثابت uchar* data2 = src2.ptr<ثابت uchar>();

                اگر( normType == NORM_L2 || normType == NORM_L2SQR || normType == NORM_L1 )
                {
                    دوگانه result = 0;
                    func(data1, data2, 0, (uchar*)&result, (صحیح)len, 1);
                    بازگشت normType == NORM_L2 ? std::sqrt(result) : result;
                }
                اگر( normType == NORM_INF )
                {
                    اعشاری result = 0;
                    func(data1, data2, 0, (uchar*)&result, (صحیح)len, 1);
                    بازگشت result;
                }
            }
        }
    }

    CV_Assert( mask.empty() || mask.type() == CV_8U || mask.type() == CV_8S || mask.type() == CV_Bool );

    اگر( normType == NORM_HAMMING || normType == NORM_HAMMING2 )
    {
        اگر( !mask.empty() )
        {
            Mat temp;
            bitwise_xor(src1, src2, temp);
            bitwise_and(temp, mask, temp);
            بازگشت norm(temp, normType);
        }
        صحیح cellSize = normType == NORM_HAMMING ? 1 : 2;

        ثابت Mat* arrays[] = {&src1, &src2, 0};
        uchar* ptrs[2] = {};
        NAryMatIterator it(arrays, ptrs);
        صحیح total = (صحیح)it.size;
        صحیح result = 0;

        برای( size_t i = 0; i < it.nplanes; i++, ++it )
        {
            result += hal::normHamming(ptrs[0], ptrs[1], total, cellSize);
        }

        بازگشت result;
    }

    ثابت Mat* arrays[] = {&src1, &src2, &mask, 0};
    uchar* ptrs[3] = {};
    اجتماع
    {
        دوگانه d;
        اعشاری f;
        unsigned u;
        uint64 UL;
    }
    result;
    result.d = 0;
    NAryMatIterator it(arrays, ptrs);
    CV_CheckLT((size_t)it.size, (size_t)INT_MAX, "");

    منطقی is_fp16 = depth == CV_16F || depth == CV_16BF;

    اگر ((normType == NORM_L1 && (depth <= CV_16S || depth == CV_Bool || is_fp16)) ||
        ((normType == NORM_L2 || normType == NORM_L2SQR) && (depth <= CV_8S || depth == CV_Bool || is_fp16)))
    {
        // special case to handle "integer" overflow in accumulator
        ثابت size_t esz = src1.elemSize();
        ثابت صحیح total = (صحیح)it.size;
        ثابت صحیح blockSize0 = (is_fp16 ? (1 << 10) :
            depth == CV_Bool ? (1 << 30) :
            normType == NORM_L1 && depth <= CV_8S ? (1 << 23) : (1 << 15))/cn;
        ثابت صحیح blockSize = std::min(total, blockSize0);
        اجتماع {
            صحیح i;
            اعشاری f;
        } blocksum;
        blocksum.i = 0;
        صحیح شمار_2 = 0;

        برای (size_t i = 0; i < it.nplanes; i++, ++it)
        {
            برای (صحیح j = 0; j < total; j += blockSize)
            {
                صحیح bsz = std::min(total - j, blockSize);
                func(ptrs[0], ptrs[1], ptrs[2], (uchar*)&blocksum, bsz, cn);
                شمار_2 += bsz;
                اگر (شمار_2 + blockSize >= blockSize0 || (i+1 >= it.nplanes && j+bsz >= total))
                {
                    result.d += is_fp16 ? (دوگانه)blocksum.f : (دوگانه)blocksum.i;
                    blocksum.i = 0;
                    شمار_2 = 0;
                }
                ptrs[0] += bsz*esz;
                ptrs[1] += bsz*esz;
                اگر (ptrs[2])
                    ptrs[2] += bsz;
            }
        }
    }
    وگرنه
    {
        // generic implementation
        برای (size_t i = 0; i < it.nplanes; i++, ++it)
        {
            func(ptrs[0], ptrs[1], ptrs[2], (uchar*)&result, (صحیح)it.size, cn);
        }
    }

    اگر( normType == NORM_INF )
    {
        اگر (depth <= CV_32S || depth == CV_32U || depth == CV_Bool)
            بازگشت result.u;
        اگر (depth == CV_32F || is_fp16)
            بازگشت result.f;
        اگر (depth == CV_64U || depth == CV_64S)
            بازگشت (دوگانه)result.UL;
    }
    اگر( normType == NORM_L2 )
        بازگشت std::sqrt(result.d);

    بازگشت result.d;
}

cv::Hamming::ResultType Hamming::operator()( ثابت unsigned نویسه* a, ثابت unsigned نویسه* b, صحیح size ) ثابت
{
    بازگشت cv::hal::normHamming(a, b, size);
}

دوگانه PSNR(درون_داد_Array _src1, درون_داد_Array _src2, دوگانه R)
{
    CV_INSTRUMENT_REGION();

    //Input arrays must have depth CV_8U
    CV_Assert( _src1.type() == _src2.type() );

    دوگانه diff = std::sqrt(norm(_src1, _src2, NORM_L2SQR)/(_src1.total()*_src1.channels()));
    بازگشت 20*log10(R/(diff+DBL_EPSILON));
}


#ifdef HAVE_OPENCL
ایستا منطقی ocl_normalize( درون_داد_Array _src, درون_داد_Output_Array _dst, درون_داد_Array _mask, صحیح dtype,
                           دوگانه scale, دوگانه delta )
{
    UMat src = _src.getUMat();

    اگر( _mask.empty() )
        src.convertTo( _dst, dtype, scale, delta );
    وگرنه اگر (src.channels() <= 4)
    {
        ثابت ocl::Device & dev = ocl::Device::getDefault();

        صحیح stype = _src.type(), sdepth = CV_MAT_DEPTH(stype), cn = CV_MAT_CN(stype),
                ddepth = CV_MAT_DEPTH(dtype), wdepth = std::max(CV_32F, std::max(sdepth, ddepth)),
                rowsPerWI = dev.isIntel() ? 4 : 1;

        اعشاری fscale = static_cast<اعشاری>(scale), fdelta = static_cast<اعشاری>(delta);
        منطقی haveScale = std::fabs(scale - 1) > DBL_EPSILON,
                haveZeroScale = !(std::fabs(scale) > DBL_EPSILON),
                haveDelta = std::fabs(delta) > DBL_EPSILON,
                doubleSupport = dev.doubleFPConfig() > 0;

        اگر (!haveScale && !haveDelta && stype == dtype)
        {
            _src.copyTo(_dst, _mask);
            بازگشت درست;
        }
        اگر (haveZeroScale)
        {
            _dst.setTo(Scalar(delta), _mask);
            بازگشت درست;
        }

        اگر ((sdepth == CV_64F || ddepth == CV_64F) && !doubleSupport)
            بازگشت نادرست;

        نویسه cvt[2][50];
        String opts = format("-D srcT=%s -D dstT=%s -D convertToWT=%s -D cn=%d -D rowsPerWI=%d"
                             " -D convertToDT=%s -D workT=%s%s%s%s -D srcT1=%s -D dstT1=%s",
                             ocl::typeToStr(stype), ocl::typeToStr(dtype),
                             ocl::convertTypeStr(sdepth, wdepth, cn, cvt[0], اندازه(cvt[0])), cn,
                             rowsPerWI, ocl::convertTypeStr(wdepth, ddepth, cn, cvt[1], اندازه(cvt[1])),
                             ocl::typeToStr(CV_MAKE_TYPE(wdepth, cn)),
                             doubleSupport ? " -D DOUBLE_SUPPORT" : "",
                             haveScale ? " -D HAVE_SCALE" : "",
                             haveDelta ? " -D HAVE_DELTA" : "",
                             ocl::typeToStr(sdepth), ocl::typeToStr(ddepth));

        ocl::Kernel k("normalizek", ocl::core::normalize_oclsrc, opts);
        اگر (k.empty())
            بازگشت نادرست;

        UMat mask = _mask.getUMat(), dst = _dst.getUMat();

        ocl::KernelArg srcarg = ocl::KernelArg::ReadOnlyNoSize(src),
                maskarg = ocl::KernelArg::ReadOnlyNoSize(mask),
                dstarg = ocl::KernelArg::ReadWrite(dst);

        اگر (haveScale)
        {
            اگر (haveDelta)
                k.args(srcarg, maskarg, dstarg, fscale, fdelta);
            وگرنه
                k.args(srcarg, maskarg, dstarg, fscale);
        }
        وگرنه
        {
            اگر (haveDelta)
                k.args(srcarg, maskarg, dstarg, fdelta);
            وگرنه
                k.args(srcarg, maskarg, dstarg);
        }

        size_t globalsize[2] = { (size_t)src.cols, ((size_t)src.rows + rowsPerWI - 1) / rowsPerWI };
        بازگشت k.run(2, globalsize, پوچ, نادرست);
    }
    وگرنه
    {
        UMat temp;
        src.convertTo( temp, dtype, scale, delta );
        temp.copyTo( _dst, _mask );
    }

    بازگشت درست;
}  // ocl_normalize
#endif  // HAVE_OPENCL

تهی normalize(درون_داد_Array _src, درون_داد_Output_Array _dst, دوگانه a, دوگانه b,
               صحیح norm_type, صحیح rtype, درون_داد_Array _mask)
{
    CV_INSTRUMENT_REGION();

    دوگانه scale = 1, shift = 0;
    صحیح type = _src.type(), depth = CV_MAT_DEPTH(type);

    اگر( rtype < 0 )
        rtype = _dst.fixedType() ? _dst.depth() : depth;

    اگر( norm_type == NORM_MINMAX )
    {
        دوگانه smin = 0, smax = 0;
        دوگانه dmin = MIN( a, b ), dmax = MAX( a, b );
        minMaxIdx( _src, &smin, &smax, 0, 0, _mask );
        scale = (dmax - dmin)*(smax - smin > DBL_EPSILON ? 1./(smax - smin) : 0);
        اگر( rtype == CV_32F )
        {
            scale = (اعشاری)scale;
            shift = (اعشاری)dmin - (اعشاری)(smin*scale);
        }
        وگرنه
            shift = dmin - smin*scale;
    }
    وگرنه اگر( norm_type == NORM_L2 || norm_type == NORM_L1 || norm_type == NORM_INF )
    {
        scale = norm( _src, norm_type, _mask );
        scale = scale > DBL_EPSILON ? a/scale : 0.;
        shift = 0;
    }
    وگرنه
        CV_Error( cv::Error::StsBadArg, "Unknown/unsupported norm type" );

    CV_OCL_RUN(_dst.isUMat(),
               ocl_normalize(_src, _dst, _mask, rtype, scale, shift))

    Mat src = _src.getMat();
    اگر( _mask.empty() )
        src.convertTo( _dst, rtype, scale, shift );
    وگرنه
    {
        Mat temp;
        src.convertTo( temp, rtype, scale, shift );
        temp.copyTo( _dst, _mask );
    }
}

}  // namespace
