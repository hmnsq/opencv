// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html
//
// Copyright (C) 2026, Advanced Micro Devices, Inc., all rights reserved.


#அந்தர்பாவ "precomp.hpp"
#அந்தர்பாவ "opencl_kernels_core.hpp"
#அந்தர்பாவ "stat.hpp"

#அந்தர்பாவ "norm.simd.hpp"
#அந்தர்பாவ "norm.simd_declarations.hpp"

/****************************************************************************************\
*                                         norm                                           *
\****************************************************************************************/

நாமாகாஶ cv { நாமாகாஶ hal {

வெளி மாறிலி uchar எடு_மொத்தம்_Table[256] =
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

நிலையான மாறிலி uchar எடு_மொத்தம்_Table2[] =
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

நிலையான மாறிலி uchar எடு_மொத்தம்_Table4[] =
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


முழு normHamming(மாறிலி uchar* a, முழு n, முழு cellSize)
{
    முழு output;
    CALL_HAL_RET(normHamming8u, cv_hal_normHamming8u, output, a, n, cellSize);

    எனில்( cellSize == 1 )
        திரும்பு normHamming(a, n);
    மாறிலி uchar* tab = 0;
    எனில்( cellSize == 2 )
        tab = எடு_மொத்தம்_Table2;
    இல்லையெனில் எனில்( cellSize == 4 )
        tab = எடு_மொத்தம்_Table4;
    இல்லையெனில்
        திரும்பு -1;
    முழு i = 0;
    முழு result = 0;
#எனில் (CV_SIMD || CV_SIMD_SCALABLE)
    v_uint64 t = vx_setzero_u64();
    எனில் ( cellSize == 2)
    {
        v_uint16 mask = v_reinterpret_as_u16(vx_setall_u8(0x55));
        க்கு(; i <= n - VTraits<v_uint8>::vlanes(); i += VTraits<v_uint8>::vlanes())
        {
            v_uint16 a0 = v_reinterpret_as_u16(vx_load(a + i));
            t = v_add(t, v_popcount(v_reinterpret_as_u64(v_and(v_or(a0, v_shr<1>(a0)), mask))));
        }
    }
    இல்லையெனில்    // cellSize == 4
    {
        v_uint16 mask = v_reinterpret_as_u16(vx_setall_u8(0x11));
        க்கு(; i <= n - VTraits<v_uint8>::vlanes(); i += VTraits<v_uint8>::vlanes())
        {
            v_uint16 a0 = v_reinterpret_as_u16(vx_load(a + i));
            v_uint16 a1 = v_or(a0, v_shr<2>(a0));
            t = v_add(t, v_popcount(v_reinterpret_as_u64(v_and(v_or(a1, v_shr<1>(a1)), mask))));

        }
    }
    result += (முழு)v_reduce_sum(t);
    vx_cleanup();
#அல்லதெனில் CV_ENABLE_UNROLLED
    க்கு( ; i <= n - 4; i += 4 )
        result += tab[a[i]] + tab[a[i+1]] + tab[a[i+2]] + tab[a[i+3]];
#endif
    க்கு( ; i < n; i++ )
        result += tab[a[i]];
    திரும்பு result;
}

முழு normHamming(மாறிலி uchar* a, மாறிலி uchar* b, முழு n, முழு cellSize)
{
    முழு output;
    CALL_HAL_RET(normHammingDiff8u, cv_hal_normHammingDiff8u, output, a, b, n, cellSize);

    எனில்( cellSize == 1 )
        திரும்பு normHamming(a, b, n);
    மாறிலி uchar* tab = 0;
    எனில்( cellSize == 2 )
        tab = எடு_மொத்தம்_Table2;
    இல்லையெனில் எனில்( cellSize == 4 )
        tab = எடு_மொத்தம்_Table4;
    இல்லையெனில்
        திரும்பு -1;
    முழு i = 0;
    முழு result = 0;
#எனில் (CV_SIMD || CV_SIMD_SCALABLE)
    v_uint64 t = vx_setzero_u64();
    எனில் ( cellSize == 2)
    {
        v_uint16 mask = v_reinterpret_as_u16(vx_setall_u8(0x55));
        க்கு(; i <= n - VTraits<v_uint8>::vlanes(); i += VTraits<v_uint8>::vlanes())
        {
            v_uint16 ab0 = v_reinterpret_as_u16(v_xor(vx_load(a + i), vx_load(b + i)));
            t = v_add(t, v_popcount(v_reinterpret_as_u64(v_and(v_or(ab0, v_shr<1>(ab0)), mask))));
        }
    }
    இல்லையெனில்    // cellSize == 4
    {
        v_uint16 mask = v_reinterpret_as_u16(vx_setall_u8(0x11));
        க்கு(; i <= n - VTraits<v_uint8>::vlanes(); i += VTraits<v_uint8>::vlanes())
        {
            v_uint16 ab0 = v_reinterpret_as_u16(v_xor(vx_load(a + i), vx_load(b + i)));
            v_uint16 ab1 = v_or(ab0, v_shr<2>(ab0));
            t = v_add(t, v_popcount(v_reinterpret_as_u64(v_and(v_or(ab1, v_shr<1>(ab1)), mask))));
        }
    }
    result += (முழு)v_reduce_sum(t);
    vx_cleanup();
#அல்லதெனில் CV_ENABLE_UNROLLED
    க்கு( ; i <= n - 4; i += 4 )
        result += tab[a[i] ^ b[i]] + tab[a[i+1] ^ b[i+1]] +
                tab[a[i+2] ^ b[i+2]] + tab[a[i+3] ^ b[i+3]];
#endif
    க்கு( ; i < n; i++ )
        result += tab[a[i] ^ b[i]];
    திரும்பு result;
}

தசம normL2Sqr_(மாறிலி தசம* a, மாறிலி தசம* b, முழு n)
{
    முழு j = 0; தசம d = 0.f;
#எனில் (CV_SIMD || CV_SIMD_SCALABLE)
    மாறிலி முழு vl = VTraits<v_float32>::vlanes();
    v_float32 v_d0 = vx_setzero_f32(), v_d1 = vx_setzero_f32();
    v_float32 v_d2 = vx_setzero_f32(), v_d3 = vx_setzero_f32();
    க்கு (; j <= n - 4 * vl; j += 4 * vl)
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
    க்கு (; j <= n - vl; j += vl)
    {
        v_float32 t0 = v_sub(vx_load(a + j), vx_load(b + j));
        v_d0 = v_muladd(t0, t0, v_d0);
    }
    d = v_reduce_sum(v_d0);
#endif
    க்கு( ; j < n; j++ )
    {
        தசம t = a[j] - b[j];
        d += t*t;
    }
    திரும்பு d;
}


தசம normL1_(மாறிலி தசம* a, மாறிலி தசம* b, முழு n)
{
    முழு j = 0; தசம d = 0.f;
#எனில் (CV_SIMD || CV_SIMD_SCALABLE)
    மாறிலி முழு vl = VTraits<v_float32>::vlanes();
    v_float32 v_d0 = vx_setzero_f32(), v_d1 = vx_setzero_f32();
    v_float32 v_d2 = vx_setzero_f32(), v_d3 = vx_setzero_f32();
    க்கு (; j <= n - 4 * vl; j += 4 * vl)
    {
        v_d0 = v_add(v_d0, v_absdiff(vx_load(a + j), vx_load(b + j)));
        v_d1 = v_add(v_d1, v_absdiff(vx_load(a + j + vl), vx_load(b + j + vl)));
        v_d2 = v_add(v_d2, v_absdiff(vx_load(a + j + 2 * vl), vx_load(b + j + 2 * vl)));
        v_d3 = v_add(v_d3, v_absdiff(vx_load(a + j + 3 * vl), vx_load(b + j + 3 * vl)));
    }
    v_d0 = v_add(v_add(v_d0, v_d1), v_add(v_d2, v_d3));
    க்கு (; j <= n - vl; j += vl)
        v_d0 = v_add(v_d0, v_absdiff(vx_load(a + j), vx_load(b + j)));
    d = v_reduce_sum(v_d0);
#endif
    க்கு( ; j < n; j++ )
        d += std::abs(a[j] - b[j]);
    திரும்பு d;
}

முழு normL1_(மாறிலி uchar* a, மாறிலி uchar* b, முழு n)
{
    முழு j = 0, d = 0;
#எனில் (CV_SIMD || CV_SIMD_SCALABLE)
    மாறிலி முழு vl = VTraits<v_uint8>::vlanes();
    க்கு (; j <= n - 4 * vl; j += 4 * vl)
        d += v_reduce_sad(vx_load(a + j), vx_load(b + j)) +
             v_reduce_sad(vx_load(a + j + vl), vx_load(b + j + vl)) +
             v_reduce_sad(vx_load(a + j + 2 * vl), vx_load(b + j + 2 * vl)) +
             v_reduce_sad(vx_load(a + j + 3 * vl), vx_load(b + j + 3 * vl));
    க்கு (; j <= n - vl; j += vl)
        d += v_reduce_sad(vx_load(a + j), vx_load(b + j));
#endif
    க்கு( ; j < n; j++ )
        d += std::abs(a[j] - b[j]);
    திரும்பு d;
}

} //cv::hal

//==================================================================================================

வகைவரை முழு (*NormFunc)(மாறிலி uchar*, மாறிலி uchar*, uchar*, முழு, முழு);
வகைவரை முழு (*NormDiffFunc)(மாறிலி uchar*, மாறிலி uchar*, மாறிலி uchar*, uchar*, முழு, முழு);

#ifdef HAVE_OPENCL

நிலையான தருக்க ocl_norm( உள்ளீடு_Array _src, முழு normType, உள்ளீடு_Array _mask, இரட்டை & result )
{
    மாறிலி ocl::Device & d = ocl::Device::getDefault();

#ifdef __ANDROID__
    எனில் (d.isNVidia())
        திரும்பு பொய்;
#endif
    மாறிலி முழு cn = _src.channels();
    எனில் (cn > 4)
        திரும்பு பொய்;
    முழு type = _src.type(), depth = CV_MAT_DEPTH(type);
    தருக்க doubleSupport = d.doubleFPConfig() > 0,
            haveMask = _mask.kind() != உள்ளீடு_Array_2::NONE;

    எனில் (depth >= CV_16F)
        திரும்பு பொய்;  // TODO: support FP16

    எனில் ( !(normType == NORM_INF || normType == NORM_L1 || normType == NORM_L2 || normType == NORM_L2SQR) ||
         (!doubleSupport && depth == CV_64F))
        திரும்பு பொய்;

    UMat src = _src.getUMat();

    எனில் (normType == NORM_INF)
    {
        எனில் (!ocl_minMaxIdx(_src, வெற்று, &result, வெற்று, வெற்று, _mask,
                           std::max(depth, CV_32S), depth != CV_8U && depth != CV_16U))
            திரும்பு பொய்;
    }
    இல்லையெனில் எனில் (normType == NORM_L1 || normType == NORM_L2 || normType == NORM_L2SQR)
    {
        Scalar sc;
        தருக்க unstype = depth == CV_8U || depth == CV_16U;

        எனில் ( !ocl_sum(haveMask ? src : src.reshape(1), sc, normType == NORM_L2 || normType == NORM_L2SQR ?
                    OCL_OP_SUM_SQR : (unstype ? OCL_OP_SUM : OCL_OP_SUM_ABS), _mask) )
            திரும்பு பொய்;

        இரட்டை s = 0.0;
        க்கு (முழு i = 0; i < (haveMask ? cn : 1); ++i)
            s += sc[i];

        result = normType == NORM_L1 || normType == NORM_L2SQR ? s : std::sqrt(s);
    }

    திரும்பு உண்மை;
}

#endif

நிலையான NormFunc getNormFunc(முழு normType, முழு depth) {
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getNormFunc, (normType, depth), CV_CPU_DISPATCH_MODES_ALL);
}
நிலையான NormDiffFunc getNormDiffFunc(முழு normType, முழு depth) {
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getNormDiffFunc, (normType, depth), CV_CPU_DISPATCH_MODES_ALL);
}

இரட்டை norm( உள்ளீடு_Array _src, முழு normType, உள்ளீடு_Array _mask )
{
    CV_INSTRUMENT_REGION();

    normType &= NORM_TYPE_MASK;
    CV_Assert( normType == NORM_INF || normType == NORM_L1 ||
               normType == NORM_L2 || normType == NORM_L2SQR ||
               ((normType == NORM_HAMMING || normType == NORM_HAMMING2) && _src.type() == CV_8U) );

#எனில் defined HAVE_OPENCL
    இரட்டை _result = 0;
#endif

#ifdef HAVE_OPENCL
    CV_OCL_RUN_(OCL_PERFORMANCE_CHECK(_src.isUMat()) && _src.dims() <= 2,
                ocl_norm(_src, normType, _mask, _result),
                _result)
#endif

    Mat src = _src.getMat(), mask = _mask.getMat();
    CV_Assert( mask.empty() || ((mask.type() == CV_8U || mask.type() == CV_8S || mask.type() == CV_Bool) && src.size == mask.size) );
    எனில்( src.empty() )
        திரும்பு 0.0;
    முழு depth = src.depth(), cn = src.channels();
    எனில்( src.dims <= 2 )
    {
        இரட்டை result;
        CALL_HAL_RET(norm, cv_hal_norm, result, src.data, src.step, mask.data, mask.step, src.cols, src.rows, src.type(), normType);
    }
    இல்லையெனில் எனில்( src.isContinuous() && mask.isContinuous() )
    {
        இரட்டை result;
        CALL_HAL_RET(norm, cv_hal_norm, result, src.data, 0, mask.data, 0, (முழு)src.total(), 1, src.type(), normType);
    }

    NormFunc func = getNormFunc(normType >> 1, depth);
    CV_Assert( (normType >> 1) >= 3 || func != 0 );

    எனில்( src.isContinuous() && mask.empty() )
    {
        size_t len = src.total()*cn;
        எனில்( len == (size_t)(முழு)len )
        {
            எனில்( depth == CV_32F )
            {
                மாறிலி uchar* data = src.ptr<மாறிலி uchar>();

                எனில்( normType == NORM_L2 || normType == NORM_L2SQR || normType == NORM_L1 )
                {
                    இரட்டை result = 0;
                    func(data, 0, (uchar*)&result, (முழு)len, 1);
                    திரும்பு normType == NORM_L2 ? std::sqrt(result) : result;
                }
                எனில்( normType == NORM_INF )
                {
                    தசம result = 0;
                    func(data, 0, (uchar*)&result, (முழு)len, 1);
                    திரும்பு result;
                }
            }
            எனில்( depth == CV_8U )
            {
                மாறிலி uchar* data = src.ptr<uchar>();

                எனில்( normType == NORM_HAMMING )
                {
                    திரும்பு hal::normHamming(data, (முழு)len, 1);
                }

                எனில்( normType == NORM_HAMMING2 )
                {
                    திரும்பு hal::normHamming(data, (முழு)len, 2);
                }
            }
        }
    }

    CV_Assert( mask.empty() || mask.type() == CV_8U || mask.type() == CV_8S || mask.type() == CV_Bool );

    எனில்( normType == NORM_HAMMING || normType == NORM_HAMMING2 )
    {
        எனில்( !mask.empty() )
        {
            Mat temp;
            bitwise_and(src, mask, temp);
            திரும்பு norm(temp, normType);
        }
        முழு cellSize = normType == NORM_HAMMING ? 1 : 2;

        மாறிலி Mat* arrays[] = {&src, 0};
        uchar* ptrs[1] = {};
        NAryMatIterator it(arrays, ptrs);
        முழு total = (முழு)it.size;
        முழு result = 0;

        க்கு( size_t i = 0; i < it.nplanes; i++, ++it )
        {
            result += hal::normHamming(ptrs[0], total, cellSize);
        }

        திரும்பு result;
    }

    மாறிலி Mat* arrays[] = {&src, &mask, 0};
    uchar* ptrs[2] = {};
    ஒன்றிப்பு
    {
        இரட்டை d;
        unsigned u;
        uint64 UL;
        தசம f;
    }
    result;
    result.d = 0;
    NAryMatIterator it(arrays, ptrs);
    CV_CheckLT((size_t)it.size, (size_t)INT_MAX, "");
    தருக்க is_fp16 = depth == CV_16F || depth == CV_16BF;

    எனில் ((normType == NORM_L1 && (depth <= CV_16S || depth == CV_Bool || is_fp16)) ||
        ((normType == NORM_L2 || normType == NORM_L2SQR) && (depth <= CV_8S || depth == CV_Bool || is_fp16)))
    {
        // special case to handle "integer" overflow in accumulator
        மாறிலி size_t esz = src.elemSize();
        மாறிலி முழு total = (முழு)it.size;
        மாறிலி முழு blockSize0 = (is_fp16 ? (1 << 10) :
            depth == CV_Bool ? (1 << 30) :
            normType == NORM_L1 && depth <= CV_8S ? (1 << 23) : (1 << 15))/cn;
        மாறிலி முழு blockSize = std::min(total, blockSize0);
        ஒன்றிப்பு {
            முழு i;
            தசம f;
        } blocksum;
        blocksum.i = 0;
        முழு மொத்தம்_2 = 0;

        க்கு (size_t i = 0; i < it.nplanes; i++, ++it)
        {
            க்கு (முழு j = 0; j < total; j += blockSize)
            {
                முழு bsz = std::min(total - j, blockSize);
                func(ptrs[0], ptrs[1], (uchar*)&blocksum, bsz, cn);
                மொத்தம்_2 += bsz;
                எனில் (மொத்தம்_2 + blockSize >= blockSize0 || (i+1 >= it.nplanes && j+bsz >= total))
                {
                    result.d += is_fp16 ? (இரட்டை)blocksum.f : (இரட்டை)blocksum.i;
                    blocksum.i = 0;
                    மொத்தம்_2 = 0;
                }
                ptrs[0] += bsz*esz;
                எனில் (ptrs[1])
                    ptrs[1] += bsz;
            }
        }
    }
    இல்லையெனில்
    {
        // generic implementation
        க்கு (size_t i = 0; i < it.nplanes; i++, ++it)
        {
            func(ptrs[0], ptrs[1], (uchar*)&result, (முழு)it.size, cn);
        }
    }

    எனில்( normType == NORM_INF )
    {
        எனில்(depth <= CV_32S || depth == CV_32U || depth == CV_Bool)
            திரும்பு result.u;
        எனில் (depth == CV_32F || is_fp16)
            திரும்பு result.f;
        எனில் (depth == CV_64U || depth == CV_64S)
            திரும்பு (இரட்டை)result.UL;
    }
    எனில்( normType == NORM_L2 )
        திரும்பு std::sqrt(result.d);

    திரும்பு result.d;
}

//==================================================================================================

#ifdef HAVE_OPENCL
நிலையான தருக்க ocl_norm( உள்ளீடு_Array _src1, உள்ளீடு_Array _src2, முழு normType, உள்ளீடு_Array _mask, இரட்டை & result )
{
#ifdef __ANDROID__
    எனில் (ocl::Device::getDefault().isNVidia())
        திரும்பு பொய்;
#endif

    Scalar sc1, sc2;
    முழு cn = _src1.channels();
    எனில் (cn > 4)
        திரும்பு பொய்;
    முழு type = _src1.type(), depth = CV_MAT_DEPTH(type);
    தருக்க relative = (normType & NORM_RELATIVE) != 0;
    normType &= ~NORM_RELATIVE;
    தருக்க normsum = normType == NORM_L1 || normType == NORM_L2 || normType == NORM_L2SQR;

#ifdef __APPLE__
    எனில்(normType == NORM_L1 && type == CV_16UC3 && !_mask.empty())
        திரும்பு பொய்;
#endif

    எனில் (normsum)
    {
        எனில் (!ocl_sum(_src1, sc1, normType == NORM_L2 || normType == NORM_L2SQR ?
                     OCL_OP_SUM_SQR : OCL_OP_SUM, _mask, _src2, relative, sc2))
            திரும்பு பொய்;
    }
    இல்லையெனில்
    {
        எனில் (!ocl_minMaxIdx(_src1, வெற்று, &sc1[0], வெற்று, வெற்று, _mask, std::max(CV_32S, depth),
                           பொய், _src2, relative ? &sc2[0] : வெற்று))
            திரும்பு பொய்;
        cn = 1;
    }

    இரட்டை s2 = 0;
    க்கு (முழு i = 0; i < cn; ++i)
    {
        result += sc1[i];
        எனில் (relative)
            s2 += sc2[i];
    }

    எனில் (normType == NORM_L2)
    {
        result = std::sqrt(result);
        எனில் (relative)
            s2 = std::sqrt(s2);
    }

    எனில் (relative)
        result /= (s2 + DBL_EPSILON);

    திரும்பு உண்மை;
}  // ocl_norm()
#endif  // HAVE_OPENCL

இரட்டை norm( உள்ளீடு_Array _src1, உள்ளீடு_Array _src2, முழு normType, உள்ளீடு_Array _mask )
{
    CV_INSTRUMENT_REGION();

    CV_CheckTypeEQ(_src1.type(), _src2.type(), "Input type mismatch");
    CV_Assert(_src1.sameSize(_src2));

#எனில் defined HAVE_OPENCL
    இரட்டை _result = 0;
#endif

#ifdef HAVE_OPENCL
    CV_OCL_RUN_(OCL_PERFORMANCE_CHECK(_src1.isUMat()),
                ocl_norm(_src1, _src2, normType, _mask, _result),
                _result)
#endif

    Mat src1 = _src1.getMat(), src2 = _src2.getMat(), mask = _mask.getMat();
    CV_Assert( mask.empty() || ((mask.type() == CV_8U || mask.type() == CV_8S || mask.type() == CV_Bool) && src1.size == mask.size) );
    எனில்( src1.empty() || src2.empty() )
        திரும்பு 0.0;
    முழு depth = src1.depth(), cn = src1.channels();
    எனில்( src1.dims <= 2 )
    {
        இரட்டை result;
        CALL_HAL_RET(normDiff, cv_hal_normDiff, result, src1.data, src1.step, src2.data, src2.step, mask.data, mask.step, src1.cols, src1.rows, src1.type(), normType);
    }
    இல்லையெனில் எனில்( src1.isContinuous() && src2.isContinuous() && mask.isContinuous() )
    {
        இரட்டை result;
        CALL_HAL_RET(normDiff, cv_hal_normDiff, result, src1.data, 0, src2.data, 0, mask.data, 0, (முழு)src1.total(), 1, src1.type(), normType);
    }

    எனில்( normType & NORM_RELATIVE )
    {
        திரும்பு norm(_src1, _src2, normType & ~NORM_RELATIVE, _mask)/(norm(_src2, normType, _mask) + DBL_EPSILON);
    }

    normType &= 7;
    CV_Assert( normType == NORM_INF || normType == NORM_L1 ||
               normType == NORM_L2 || normType == NORM_L2SQR ||
              ((normType == NORM_HAMMING || normType == NORM_HAMMING2) && src1.type() == CV_8U) );

    NormDiffFunc func = getNormDiffFunc(normType >> 1, depth);
    CV_Assert( (normType >> 1) >= 3 || func != 0 );

    எனில்( src1.isContinuous() && src2.isContinuous() && mask.empty() )
    {
        size_t len = src1.total()*src1.channels();
        எனில்( len == (size_t)(முழு)len )
        {
            எனில்( src1.depth() == CV_32F )
            {
                மாறிலி uchar* data1 = src1.ptr<மாறிலி uchar>();
                மாறிலி uchar* data2 = src2.ptr<மாறிலி uchar>();

                எனில்( normType == NORM_L2 || normType == NORM_L2SQR || normType == NORM_L1 )
                {
                    இரட்டை result = 0;
                    func(data1, data2, 0, (uchar*)&result, (முழு)len, 1);
                    திரும்பு normType == NORM_L2 ? std::sqrt(result) : result;
                }
                எனில்( normType == NORM_INF )
                {
                    தசம result = 0;
                    func(data1, data2, 0, (uchar*)&result, (முழு)len, 1);
                    திரும்பு result;
                }
            }
        }
    }

    CV_Assert( mask.empty() || mask.type() == CV_8U || mask.type() == CV_8S || mask.type() == CV_Bool );

    எனில்( normType == NORM_HAMMING || normType == NORM_HAMMING2 )
    {
        எனில்( !mask.empty() )
        {
            Mat temp;
            bitwise_xor(src1, src2, temp);
            bitwise_and(temp, mask, temp);
            திரும்பு norm(temp, normType);
        }
        முழு cellSize = normType == NORM_HAMMING ? 1 : 2;

        மாறிலி Mat* arrays[] = {&src1, &src2, 0};
        uchar* ptrs[2] = {};
        NAryMatIterator it(arrays, ptrs);
        முழு total = (முழு)it.size;
        முழு result = 0;

        க்கு( size_t i = 0; i < it.nplanes; i++, ++it )
        {
            result += hal::normHamming(ptrs[0], ptrs[1], total, cellSize);
        }

        திரும்பு result;
    }

    மாறிலி Mat* arrays[] = {&src1, &src2, &mask, 0};
    uchar* ptrs[3] = {};
    ஒன்றிப்பு
    {
        இரட்டை d;
        தசம f;
        unsigned u;
        uint64 UL;
    }
    result;
    result.d = 0;
    NAryMatIterator it(arrays, ptrs);
    CV_CheckLT((size_t)it.size, (size_t)INT_MAX, "");

    தருக்க is_fp16 = depth == CV_16F || depth == CV_16BF;

    எனில் ((normType == NORM_L1 && (depth <= CV_16S || depth == CV_Bool || is_fp16)) ||
        ((normType == NORM_L2 || normType == NORM_L2SQR) && (depth <= CV_8S || depth == CV_Bool || is_fp16)))
    {
        // special case to handle "integer" overflow in accumulator
        மாறிலி size_t esz = src1.elemSize();
        மாறிலி முழு total = (முழு)it.size;
        மாறிலி முழு blockSize0 = (is_fp16 ? (1 << 10) :
            depth == CV_Bool ? (1 << 30) :
            normType == NORM_L1 && depth <= CV_8S ? (1 << 23) : (1 << 15))/cn;
        மாறிலி முழு blockSize = std::min(total, blockSize0);
        ஒன்றிப்பு {
            முழு i;
            தசம f;
        } blocksum;
        blocksum.i = 0;
        முழு மொத்தம்_2 = 0;

        க்கு (size_t i = 0; i < it.nplanes; i++, ++it)
        {
            க்கு (முழு j = 0; j < total; j += blockSize)
            {
                முழு bsz = std::min(total - j, blockSize);
                func(ptrs[0], ptrs[1], ptrs[2], (uchar*)&blocksum, bsz, cn);
                மொத்தம்_2 += bsz;
                எனில் (மொத்தம்_2 + blockSize >= blockSize0 || (i+1 >= it.nplanes && j+bsz >= total))
                {
                    result.d += is_fp16 ? (இரட்டை)blocksum.f : (இரட்டை)blocksum.i;
                    blocksum.i = 0;
                    மொத்தம்_2 = 0;
                }
                ptrs[0] += bsz*esz;
                ptrs[1] += bsz*esz;
                எனில் (ptrs[2])
                    ptrs[2] += bsz;
            }
        }
    }
    இல்லையெனில்
    {
        // generic implementation
        க்கு (size_t i = 0; i < it.nplanes; i++, ++it)
        {
            func(ptrs[0], ptrs[1], ptrs[2], (uchar*)&result, (முழு)it.size, cn);
        }
    }

    எனில்( normType == NORM_INF )
    {
        எனில் (depth <= CV_32S || depth == CV_32U || depth == CV_Bool)
            திரும்பு result.u;
        எனில் (depth == CV_32F || is_fp16)
            திரும்பு result.f;
        எனில் (depth == CV_64U || depth == CV_64S)
            திரும்பு (இரட்டை)result.UL;
    }
    எனில்( normType == NORM_L2 )
        திரும்பு std::sqrt(result.d);

    திரும்பு result.d;
}

cv::Hamming::ResultType Hamming::operator()( மாறிலி unsigned எழுத்து* a, மாறிலி unsigned எழுத்து* b, முழு size ) மாறிலி
{
    திரும்பு cv::hal::normHamming(a, b, size);
}

இரட்டை PSNR(உள்ளீடு_Array _src1, உள்ளீடு_Array _src2, இரட்டை R)
{
    CV_INSTRUMENT_REGION();

    //Input arrays must have depth CV_8U
    CV_Assert( _src1.type() == _src2.type() );

    இரட்டை diff = std::sqrt(norm(_src1, _src2, NORM_L2SQR)/(_src1.total()*_src1.channels()));
    திரும்பு 20*log10(R/(diff+DBL_EPSILON));
}


#ifdef HAVE_OPENCL
நிலையான தருக்க ocl_normalize( உள்ளீடு_Array _src, உள்ளீடு_Output_Array _dst, உள்ளீடு_Array _mask, முழு dtype,
                           இரட்டை scale, இரட்டை delta )
{
    UMat src = _src.getUMat();

    எனில்( _mask.empty() )
        src.convertTo( _dst, dtype, scale, delta );
    இல்லையெனில் எனில் (src.channels() <= 4)
    {
        மாறிலி ocl::Device & dev = ocl::Device::getDefault();

        முழு stype = _src.type(), sdepth = CV_MAT_DEPTH(stype), cn = CV_MAT_CN(stype),
                ddepth = CV_MAT_DEPTH(dtype), wdepth = std::max(CV_32F, std::max(sdepth, ddepth)),
                rowsPerWI = dev.isIntel() ? 4 : 1;

        தசம fscale = static_cast<தசம>(scale), fdelta = static_cast<தசம>(delta);
        தருக்க haveScale = std::fabs(scale - 1) > DBL_EPSILON,
                haveZeroScale = !(std::fabs(scale) > DBL_EPSILON),
                haveDelta = std::fabs(delta) > DBL_EPSILON,
                doubleSupport = dev.doubleFPConfig() > 0;

        எனில் (!haveScale && !haveDelta && stype == dtype)
        {
            _src.copyTo(_dst, _mask);
            திரும்பு உண்மை;
        }
        எனில் (haveZeroScale)
        {
            _dst.setTo(Scalar(delta), _mask);
            திரும்பு உண்மை;
        }

        எனில் ((sdepth == CV_64F || ddepth == CV_64F) && !doubleSupport)
            திரும்பு பொய்;

        எழுத்து cvt[2][50];
        String opts = format("-D srcT=%s -D dstT=%s -D convertToWT=%s -D cn=%d -D rowsPerWI=%d"
                             " -D convertToDT=%s -D workT=%s%s%s%s -D srcT1=%s -D dstT1=%s",
                             ocl::typeToStr(stype), ocl::typeToStr(dtype),
                             ocl::convertTypeStr(sdepth, wdepth, cn, cvt[0], அளவு(cvt[0])), cn,
                             rowsPerWI, ocl::convertTypeStr(wdepth, ddepth, cn, cvt[1], அளவு(cvt[1])),
                             ocl::typeToStr(CV_MAKE_TYPE(wdepth, cn)),
                             doubleSupport ? " -D DOUBLE_SUPPORT" : "",
                             haveScale ? " -D HAVE_SCALE" : "",
                             haveDelta ? " -D HAVE_DELTA" : "",
                             ocl::typeToStr(sdepth), ocl::typeToStr(ddepth));

        ocl::Kernel k("normalizek", ocl::core::normalize_oclsrc, opts);
        எனில் (k.empty())
            திரும்பு பொய்;

        UMat mask = _mask.getUMat(), dst = _dst.getUMat();

        ocl::KernelArg srcarg = ocl::KernelArg::ReadOnlyNoSize(src),
                maskarg = ocl::KernelArg::ReadOnlyNoSize(mask),
                dstarg = ocl::KernelArg::ReadWrite(dst);

        எனில் (haveScale)
        {
            எனில் (haveDelta)
                k.args(srcarg, maskarg, dstarg, fscale, fdelta);
            இல்லையெனில்
                k.args(srcarg, maskarg, dstarg, fscale);
        }
        இல்லையெனில்
        {
            எனில் (haveDelta)
                k.args(srcarg, maskarg, dstarg, fdelta);
            இல்லையெனில்
                k.args(srcarg, maskarg, dstarg);
        }

        size_t globalsize[2] = { (size_t)src.cols, ((size_t)src.rows + rowsPerWI - 1) / rowsPerWI };
        திரும்பு k.run(2, globalsize, வெற்று, பொய்);
    }
    இல்லையெனில்
    {
        UMat temp;
        src.convertTo( temp, dtype, scale, delta );
        temp.copyTo( _dst, _mask );
    }

    திரும்பு உண்மை;
}  // ocl_normalize
#endif  // HAVE_OPENCL

வெறுமை normalize(உள்ளீடு_Array _src, உள்ளீடு_Output_Array _dst, இரட்டை a, இரட்டை b,
               முழு norm_type, முழு rtype, உள்ளீடு_Array _mask)
{
    CV_INSTRUMENT_REGION();

    இரட்டை scale = 1, shift = 0;
    முழு type = _src.type(), depth = CV_MAT_DEPTH(type);

    எனில்( rtype < 0 )
        rtype = _dst.fixedType() ? _dst.depth() : depth;

    எனில்( norm_type == NORM_MINMAX )
    {
        இரட்டை smin = 0, smax = 0;
        இரட்டை dmin = MIN( a, b ), dmax = MAX( a, b );
        minMaxIdx( _src, &smin, &smax, 0, 0, _mask );
        scale = (dmax - dmin)*(smax - smin > DBL_EPSILON ? 1./(smax - smin) : 0);
        எனில்( rtype == CV_32F )
        {
            scale = (தசம)scale;
            shift = (தசம)dmin - (தசம)(smin*scale);
        }
        இல்லையெனில்
            shift = dmin - smin*scale;
    }
    இல்லையெனில் எனில்( norm_type == NORM_L2 || norm_type == NORM_L1 || norm_type == NORM_INF )
    {
        scale = norm( _src, norm_type, _mask );
        scale = scale > DBL_EPSILON ? a/scale : 0.;
        shift = 0;
    }
    இல்லையெனில்
        CV_Error( cv::Error::StsBadArg, "Unknown/unsupported norm type" );

    CV_OCL_RUN(_dst.isUMat(),
               ocl_normalize(_src, _dst, _mask, rtype, scale, shift))

    Mat src = _src.getMat();
    எனில்( _mask.empty() )
        src.convertTo( _dst, rtype, scale, shift );
    இல்லையெனில்
    {
        Mat temp;
        src.convertTo( temp, rtype, scale, shift );
        temp.copyTo( _dst, _mask );
    }
}

}  // namespace
