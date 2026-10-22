// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html
//
// Copyright (C) 2026, Advanced Micro Devices, Inc., all rights reserved.


#अन्तर्भाव "precomp.hpp"
#अन्तर्भाव "opencl_kernels_core.hpp"
#अन्तर्भाव "stat.hpp"

#अन्तर्भाव "norm.simd.hpp"
#अन्तर्भाव "norm.simd_declarations.hpp"

/****************************************************************************************\
*                                         norm                                           *
\****************************************************************************************/

नामाकाश cv { नामाकाश hal {

बाह्य ध्रुव uchar निष्कासय_गणनम्_Table[256] =
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

स्थिर ध्रुव uchar निष्कासय_गणनम्_Table2[] =
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

स्थिर ध्रुव uchar निष्कासय_गणनम्_Table4[] =
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


पूर्णाङ्क normHamming(ध्रुव uchar* a, पूर्णाङ्क n, पूर्णाङ्क cellSize)
{
    पूर्णाङ्क output;
    CALL_HAL_RET(normHamming8u, cv_hal_normHamming8u, output, a, n, cellSize);

    सचे( cellSize == 1 )
        पटिनिवत्त normHamming(a, n);
    ध्रुव uchar* tab = 0;
    सचे( cellSize == 2 )
        tab = निष्कासय_गणनम्_Table2;
    अञ्ञथा सचे( cellSize == 4 )
        tab = निष्कासय_गणनम्_Table4;
    अञ्ञथा
        पटिनिवत्त -1;
    पूर्णाङ्क i = 0;
    पूर्णाङ्क result = 0;
#सचे (CV_SIMD || CV_SIMD_SCALABLE)
    v_uint64 t = vx_setzero_u64();
    सचे ( cellSize == 2)
    {
        v_uint16 mask = v_reinterpret_as_u16(vx_setall_u8(0x55));
        अत्थाय(; i <= n - VTraits<v_uint8>::vlanes(); i += VTraits<v_uint8>::vlanes())
        {
            v_uint16 a0 = v_reinterpret_as_u16(vx_load(a + i));
            t = v_add(t, v_popcount(v_reinterpret_as_u64(v_and(v_or(a0, v_shr<1>(a0)), mask))));
        }
    }
    अञ्ञथा    // cellSize == 4
    {
        v_uint16 mask = v_reinterpret_as_u16(vx_setall_u8(0x11));
        अत्थाय(; i <= n - VTraits<v_uint8>::vlanes(); i += VTraits<v_uint8>::vlanes())
        {
            v_uint16 a0 = v_reinterpret_as_u16(vx_load(a + i));
            v_uint16 a1 = v_or(a0, v_shr<2>(a0));
            t = v_add(t, v_popcount(v_reinterpret_as_u64(v_and(v_or(a1, v_shr<1>(a1)), mask))));

        }
    }
    result += (पूर्णाङ्क)v_reduce_sum(t);
    vx_cleanup();
#नोचेद्यदि CV_ENABLE_UNROLLED
    अत्थाय( ; i <= n - 4; i += 4 )
        result += tab[a[i]] + tab[a[i+1]] + tab[a[i+2]] + tab[a[i+3]];
#endif
    अत्थाय( ; i < n; i++ )
        result += tab[a[i]];
    पटिनिवत्त result;
}

पूर्णाङ्क normHamming(ध्रुव uchar* a, ध्रुव uchar* b, पूर्णाङ्क n, पूर्णाङ्क cellSize)
{
    पूर्णाङ्क output;
    CALL_HAL_RET(normHammingDiff8u, cv_hal_normHammingDiff8u, output, a, b, n, cellSize);

    सचे( cellSize == 1 )
        पटिनिवत्त normHamming(a, b, n);
    ध्रुव uchar* tab = 0;
    सचे( cellSize == 2 )
        tab = निष्कासय_गणनम्_Table2;
    अञ्ञथा सचे( cellSize == 4 )
        tab = निष्कासय_गणनम्_Table4;
    अञ्ञथा
        पटिनिवत्त -1;
    पूर्णाङ्क i = 0;
    पूर्णाङ्क result = 0;
#सचे (CV_SIMD || CV_SIMD_SCALABLE)
    v_uint64 t = vx_setzero_u64();
    सचे ( cellSize == 2)
    {
        v_uint16 mask = v_reinterpret_as_u16(vx_setall_u8(0x55));
        अत्थाय(; i <= n - VTraits<v_uint8>::vlanes(); i += VTraits<v_uint8>::vlanes())
        {
            v_uint16 ab0 = v_reinterpret_as_u16(v_xor(vx_load(a + i), vx_load(b + i)));
            t = v_add(t, v_popcount(v_reinterpret_as_u64(v_and(v_or(ab0, v_shr<1>(ab0)), mask))));
        }
    }
    अञ्ञथा    // cellSize == 4
    {
        v_uint16 mask = v_reinterpret_as_u16(vx_setall_u8(0x11));
        अत्थाय(; i <= n - VTraits<v_uint8>::vlanes(); i += VTraits<v_uint8>::vlanes())
        {
            v_uint16 ab0 = v_reinterpret_as_u16(v_xor(vx_load(a + i), vx_load(b + i)));
            v_uint16 ab1 = v_or(ab0, v_shr<2>(ab0));
            t = v_add(t, v_popcount(v_reinterpret_as_u64(v_and(v_or(ab1, v_shr<1>(ab1)), mask))));
        }
    }
    result += (पूर्णाङ्क)v_reduce_sum(t);
    vx_cleanup();
#नोचेद्यदि CV_ENABLE_UNROLLED
    अत्थाय( ; i <= n - 4; i += 4 )
        result += tab[a[i] ^ b[i]] + tab[a[i+1] ^ b[i+1]] +
                tab[a[i+2] ^ b[i+2]] + tab[a[i+3] ^ b[i+3]];
#endif
    अत्थाय( ; i < n; i++ )
        result += tab[a[i] ^ b[i]];
    पटिनिवत्त result;
}

भिन्न normL2Sqr_(ध्रुव भिन्न* a, ध्रुव भिन्न* b, पूर्णाङ्क n)
{
    पूर्णाङ्क j = 0; भिन्न d = 0.f;
#सचे (CV_SIMD || CV_SIMD_SCALABLE)
    ध्रुव पूर्णाङ्क vl = VTraits<v_float32>::vlanes();
    v_float32 v_d0 = vx_setzero_f32(), v_d1 = vx_setzero_f32();
    v_float32 v_d2 = vx_setzero_f32(), v_d3 = vx_setzero_f32();
    अत्थाय (; j <= n - 4 * vl; j += 4 * vl)
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
    अत्थाय (; j <= n - vl; j += vl)
    {
        v_float32 t0 = v_sub(vx_load(a + j), vx_load(b + j));
        v_d0 = v_muladd(t0, t0, v_d0);
    }
    d = v_reduce_sum(v_d0);
#endif
    अत्थाय( ; j < n; j++ )
    {
        भिन्न t = a[j] - b[j];
        d += t*t;
    }
    पटिनिवत्त d;
}


भिन्न normL1_(ध्रुव भिन्न* a, ध्रुव भिन्न* b, पूर्णाङ्क n)
{
    पूर्णाङ्क j = 0; भिन्न d = 0.f;
#सचे (CV_SIMD || CV_SIMD_SCALABLE)
    ध्रुव पूर्णाङ्क vl = VTraits<v_float32>::vlanes();
    v_float32 v_d0 = vx_setzero_f32(), v_d1 = vx_setzero_f32();
    v_float32 v_d2 = vx_setzero_f32(), v_d3 = vx_setzero_f32();
    अत्थाय (; j <= n - 4 * vl; j += 4 * vl)
    {
        v_d0 = v_add(v_d0, v_absdiff(vx_load(a + j), vx_load(b + j)));
        v_d1 = v_add(v_d1, v_absdiff(vx_load(a + j + vl), vx_load(b + j + vl)));
        v_d2 = v_add(v_d2, v_absdiff(vx_load(a + j + 2 * vl), vx_load(b + j + 2 * vl)));
        v_d3 = v_add(v_d3, v_absdiff(vx_load(a + j + 3 * vl), vx_load(b + j + 3 * vl)));
    }
    v_d0 = v_add(v_add(v_d0, v_d1), v_add(v_d2, v_d3));
    अत्थाय (; j <= n - vl; j += vl)
        v_d0 = v_add(v_d0, v_absdiff(vx_load(a + j), vx_load(b + j)));
    d = v_reduce_sum(v_d0);
#endif
    अत्थाय( ; j < n; j++ )
        d += std::abs(a[j] - b[j]);
    पटिनिवत्त d;
}

पूर्णाङ्क normL1_(ध्रुव uchar* a, ध्रुव uchar* b, पूर्णाङ्क n)
{
    पूर्णाङ्क j = 0, d = 0;
#सचे (CV_SIMD || CV_SIMD_SCALABLE)
    ध्रुव पूर्णाङ्क vl = VTraits<v_uint8>::vlanes();
    अत्थाय (; j <= n - 4 * vl; j += 4 * vl)
        d += v_reduce_sad(vx_load(a + j), vx_load(b + j)) +
             v_reduce_sad(vx_load(a + j + vl), vx_load(b + j + vl)) +
             v_reduce_sad(vx_load(a + j + 2 * vl), vx_load(b + j + 2 * vl)) +
             v_reduce_sad(vx_load(a + j + 3 * vl), vx_load(b + j + 3 * vl));
    अत्थाय (; j <= n - vl; j += vl)
        d += v_reduce_sad(vx_load(a + j), vx_load(b + j));
#endif
    अत्थाय( ; j < n; j++ )
        d += std::abs(a[j] - b[j]);
    पटिनिवत्त d;
}

} //cv::hal

//==================================================================================================

प्रकारनिर्देश पूर्णाङ्क (*NormFunc)(ध्रुव uchar*, ध्रुव uchar*, uchar*, पूर्णाङ्क, पूर्णाङ्क);
प्रकारनिर्देश पूर्णाङ्क (*NormDiffFunc)(ध्रुव uchar*, ध्रुव uchar*, ध्रुव uchar*, uchar*, पूर्णाङ्क, पूर्णाङ्क);

#ifdef HAVE_OPENCL

स्थिर तार्किक ocl_norm( निवेशः_Array _src, पूर्णाङ्क normType, निवेशः_Array _mask, द्विगुण & result )
{
    ध्रुव ocl::Device & d = ocl::Device::getDefault();

#ifdef __ANDROID__
    सचे (d.isNVidia())
        पटिनिवत्त मिच्छा;
#endif
    ध्रुव पूर्णाङ्क cn = _src.channels();
    सचे (cn > 4)
        पटिनिवत्त मिच्छा;
    पूर्णाङ्क type = _src.type(), depth = CV_MAT_DEPTH(type);
    तार्किक doubleSupport = d.doubleFPConfig() > 0,
            haveMask = _mask.kind() != निवेशः_Array_2::NONE;

    सचे (depth >= CV_16F)
        पटिनिवत्त मिच्छा;  // TODO: support FP16

    सचे ( !(normType == NORM_INF || normType == NORM_L1 || normType == NORM_L2 || normType == NORM_L2SQR) ||
         (!doubleSupport && depth == CV_64F))
        पटिनिवत्त मिच्छा;

    UMat src = _src.getUMat();

    सचे (normType == NORM_INF)
    {
        सचे (!ocl_minMaxIdx(_src, रिक्त, &result, रिक्त, रिक्त, _mask,
                           std::max(depth, CV_32S), depth != CV_8U && depth != CV_16U))
            पटिनिवत्त मिच्छा;
    }
    अञ्ञथा सचे (normType == NORM_L1 || normType == NORM_L2 || normType == NORM_L2SQR)
    {
        Scalar sc;
        तार्किक unstype = depth == CV_8U || depth == CV_16U;

        सचे ( !ocl_sum(haveMask ? src : src.reshape(1), sc, normType == NORM_L2 || normType == NORM_L2SQR ?
                    OCL_OP_SUM_SQR : (unstype ? OCL_OP_SUM : OCL_OP_SUM_ABS), _mask) )
            पटिनिवत्त मिच्छा;

        द्विगुण s = 0.0;
        अत्थाय (पूर्णाङ्क i = 0; i < (haveMask ? cn : 1); ++i)
            s += sc[i];

        result = normType == NORM_L1 || normType == NORM_L2SQR ? s : std::sqrt(s);
    }

    पटिनिवत्त सच्च;
}

#endif

स्थिर NormFunc getNormFunc(पूर्णाङ्क normType, पूर्णाङ्क depth) {
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getNormFunc, (normType, depth), CV_CPU_DISPATCH_MODES_ALL);
}
स्थिर NormDiffFunc getNormDiffFunc(पूर्णाङ्क normType, पूर्णाङ्क depth) {
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getNormDiffFunc, (normType, depth), CV_CPU_DISPATCH_MODES_ALL);
}

द्विगुण norm( निवेशः_Array _src, पूर्णाङ्क normType, निवेशः_Array _mask )
{
    CV_INSTRUMENT_REGION();

    normType &= NORM_TYPE_MASK;
    CV_Assert( normType == NORM_INF || normType == NORM_L1 ||
               normType == NORM_L2 || normType == NORM_L2SQR ||
               ((normType == NORM_HAMMING || normType == NORM_HAMMING2) && _src.type() == CV_8U) );

#सचे defined HAVE_OPENCL
    द्विगुण _result = 0;
#endif

#ifdef HAVE_OPENCL
    CV_OCL_RUN_(OCL_PERFORMANCE_CHECK(_src.isUMat()) && _src.dims() <= 2,
                ocl_norm(_src, normType, _mask, _result),
                _result)
#endif

    Mat src = _src.getMat(), mask = _mask.getMat();
    CV_Assert( mask.empty() || ((mask.type() == CV_8U || mask.type() == CV_8S || mask.type() == CV_Bool) && src.size == mask.size) );
    सचे( src.empty() )
        पटिनिवत्त 0.0;
    पूर्णाङ्क depth = src.depth(), cn = src.channels();
    सचे( src.dims <= 2 )
    {
        द्विगुण result;
        CALL_HAL_RET(norm, cv_hal_norm, result, src.data, src.step, mask.data, mask.step, src.cols, src.rows, src.type(), normType);
    }
    अञ्ञथा सचे( src.isContinuous() && mask.isContinuous() )
    {
        द्विगुण result;
        CALL_HAL_RET(norm, cv_hal_norm, result, src.data, 0, mask.data, 0, (पूर्णाङ्क)src.total(), 1, src.type(), normType);
    }

    NormFunc func = getNormFunc(normType >> 1, depth);
    CV_Assert( (normType >> 1) >= 3 || func != 0 );

    सचे( src.isContinuous() && mask.empty() )
    {
        size_t len = src.total()*cn;
        सचे( len == (size_t)(पूर्णाङ्क)len )
        {
            सचे( depth == CV_32F )
            {
                ध्रुव uchar* data = src.ptr<ध्रुव uchar>();

                सचे( normType == NORM_L2 || normType == NORM_L2SQR || normType == NORM_L1 )
                {
                    द्विगुण result = 0;
                    func(data, 0, (uchar*)&result, (पूर्णाङ्क)len, 1);
                    पटिनिवत्त normType == NORM_L2 ? std::sqrt(result) : result;
                }
                सचे( normType == NORM_INF )
                {
                    भिन्न result = 0;
                    func(data, 0, (uchar*)&result, (पूर्णाङ्क)len, 1);
                    पटिनिवत्त result;
                }
            }
            सचे( depth == CV_8U )
            {
                ध्रुव uchar* data = src.ptr<uchar>();

                सचे( normType == NORM_HAMMING )
                {
                    पटिनिवत्त hal::normHamming(data, (पूर्णाङ्क)len, 1);
                }

                सचे( normType == NORM_HAMMING2 )
                {
                    पटिनिवत्त hal::normHamming(data, (पूर्णाङ्क)len, 2);
                }
            }
        }
    }

    CV_Assert( mask.empty() || mask.type() == CV_8U || mask.type() == CV_8S || mask.type() == CV_Bool );

    सचे( normType == NORM_HAMMING || normType == NORM_HAMMING2 )
    {
        सचे( !mask.empty() )
        {
            Mat temp;
            bitwise_and(src, mask, temp);
            पटिनिवत्त norm(temp, normType);
        }
        पूर्णाङ्क cellSize = normType == NORM_HAMMING ? 1 : 2;

        ध्रुव Mat* arrays[] = {&src, 0};
        uchar* ptrs[1] = {};
        NAryMatIterator it(arrays, ptrs);
        पूर्णाङ्क total = (पूर्णाङ्क)it.size;
        पूर्णाङ्क result = 0;

        अत्थाय( size_t i = 0; i < it.nplanes; i++, ++it )
        {
            result += hal::normHamming(ptrs[0], total, cellSize);
        }

        पटिनिवत्त result;
    }

    ध्रुव Mat* arrays[] = {&src, &mask, 0};
    uchar* ptrs[2] = {};
    सङ्घ
    {
        द्विगुण d;
        unsigned u;
        uint64 UL;
        भिन्न f;
    }
    result;
    result.d = 0;
    NAryMatIterator it(arrays, ptrs);
    CV_CheckLT((size_t)it.size, (size_t)INT_MAX, "");
    तार्किक is_fp16 = depth == CV_16F || depth == CV_16BF;

    सचे ((normType == NORM_L1 && (depth <= CV_16S || depth == CV_Bool || is_fp16)) ||
        ((normType == NORM_L2 || normType == NORM_L2SQR) && (depth <= CV_8S || depth == CV_Bool || is_fp16)))
    {
        // special case to handle "integer" overflow in accumulator
        ध्रुव size_t esz = src.elemSize();
        ध्रुव पूर्णाङ्क total = (पूर्णाङ्क)it.size;
        ध्रुव पूर्णाङ्क blockSize0 = (is_fp16 ? (1 << 10) :
            depth == CV_Bool ? (1 << 30) :
            normType == NORM_L1 && depth <= CV_8S ? (1 << 23) : (1 << 15))/cn;
        ध्रुव पूर्णाङ्क blockSize = std::min(total, blockSize0);
        सङ्घ {
            पूर्णाङ्क i;
            भिन्न f;
        } blocksum;
        blocksum.i = 0;
        पूर्णाङ्क गणनम्_2 = 0;

        अत्थाय (size_t i = 0; i < it.nplanes; i++, ++it)
        {
            अत्थाय (पूर्णाङ्क j = 0; j < total; j += blockSize)
            {
                पूर्णाङ्क bsz = std::min(total - j, blockSize);
                func(ptrs[0], ptrs[1], (uchar*)&blocksum, bsz, cn);
                गणनम्_2 += bsz;
                सचे (गणनम्_2 + blockSize >= blockSize0 || (i+1 >= it.nplanes && j+bsz >= total))
                {
                    result.d += is_fp16 ? (द्विगुण)blocksum.f : (द्विगुण)blocksum.i;
                    blocksum.i = 0;
                    गणनम्_2 = 0;
                }
                ptrs[0] += bsz*esz;
                सचे (ptrs[1])
                    ptrs[1] += bsz;
            }
        }
    }
    अञ्ञथा
    {
        // generic implementation
        अत्थाय (size_t i = 0; i < it.nplanes; i++, ++it)
        {
            func(ptrs[0], ptrs[1], (uchar*)&result, (पूर्णाङ्क)it.size, cn);
        }
    }

    सचे( normType == NORM_INF )
    {
        सचे(depth <= CV_32S || depth == CV_32U || depth == CV_Bool)
            पटिनिवत्त result.u;
        सचे (depth == CV_32F || is_fp16)
            पटिनिवत्त result.f;
        सचे (depth == CV_64U || depth == CV_64S)
            पटिनिवत्त (द्विगुण)result.UL;
    }
    सचे( normType == NORM_L2 )
        पटिनिवत्त std::sqrt(result.d);

    पटिनिवत्त result.d;
}

//==================================================================================================

#ifdef HAVE_OPENCL
स्थिर तार्किक ocl_norm( निवेशः_Array _src1, निवेशः_Array _src2, पूर्णाङ्क normType, निवेशः_Array _mask, द्विगुण & result )
{
#ifdef __ANDROID__
    सचे (ocl::Device::getDefault().isNVidia())
        पटिनिवत्त मिच्छा;
#endif

    Scalar sc1, sc2;
    पूर्णाङ्क cn = _src1.channels();
    सचे (cn > 4)
        पटिनिवत्त मिच्छा;
    पूर्णाङ्क type = _src1.type(), depth = CV_MAT_DEPTH(type);
    तार्किक relative = (normType & NORM_RELATIVE) != 0;
    normType &= ~NORM_RELATIVE;
    तार्किक normsum = normType == NORM_L1 || normType == NORM_L2 || normType == NORM_L2SQR;

#ifdef __APPLE__
    सचे(normType == NORM_L1 && type == CV_16UC3 && !_mask.empty())
        पटिनिवत्त मिच्छा;
#endif

    सचे (normsum)
    {
        सचे (!ocl_sum(_src1, sc1, normType == NORM_L2 || normType == NORM_L2SQR ?
                     OCL_OP_SUM_SQR : OCL_OP_SUM, _mask, _src2, relative, sc2))
            पटिनिवत्त मिच्छा;
    }
    अञ्ञथा
    {
        सचे (!ocl_minMaxIdx(_src1, रिक्त, &sc1[0], रिक्त, रिक्त, _mask, std::max(CV_32S, depth),
                           मिच्छा, _src2, relative ? &sc2[0] : रिक्त))
            पटिनिवत्त मिच्छा;
        cn = 1;
    }

    द्विगुण s2 = 0;
    अत्थाय (पूर्णाङ्क i = 0; i < cn; ++i)
    {
        result += sc1[i];
        सचे (relative)
            s2 += sc2[i];
    }

    सचे (normType == NORM_L2)
    {
        result = std::sqrt(result);
        सचे (relative)
            s2 = std::sqrt(s2);
    }

    सचे (relative)
        result /= (s2 + DBL_EPSILON);

    पटिनिवत्त सच्च;
}  // ocl_norm()
#endif  // HAVE_OPENCL

द्विगुण norm( निवेशः_Array _src1, निवेशः_Array _src2, पूर्णाङ्क normType, निवेशः_Array _mask )
{
    CV_INSTRUMENT_REGION();

    CV_CheckTypeEQ(_src1.type(), _src2.type(), "Input type mismatch");
    CV_Assert(_src1.sameSize(_src2));

#सचे defined HAVE_OPENCL
    द्विगुण _result = 0;
#endif

#ifdef HAVE_OPENCL
    CV_OCL_RUN_(OCL_PERFORMANCE_CHECK(_src1.isUMat()),
                ocl_norm(_src1, _src2, normType, _mask, _result),
                _result)
#endif

    Mat src1 = _src1.getMat(), src2 = _src2.getMat(), mask = _mask.getMat();
    CV_Assert( mask.empty() || ((mask.type() == CV_8U || mask.type() == CV_8S || mask.type() == CV_Bool) && src1.size == mask.size) );
    सचे( src1.empty() || src2.empty() )
        पटिनिवत्त 0.0;
    पूर्णाङ्क depth = src1.depth(), cn = src1.channels();
    सचे( src1.dims <= 2 )
    {
        द्विगुण result;
        CALL_HAL_RET(normDiff, cv_hal_normDiff, result, src1.data, src1.step, src2.data, src2.step, mask.data, mask.step, src1.cols, src1.rows, src1.type(), normType);
    }
    अञ्ञथा सचे( src1.isContinuous() && src2.isContinuous() && mask.isContinuous() )
    {
        द्विगुण result;
        CALL_HAL_RET(normDiff, cv_hal_normDiff, result, src1.data, 0, src2.data, 0, mask.data, 0, (पूर्णाङ्क)src1.total(), 1, src1.type(), normType);
    }

    सचे( normType & NORM_RELATIVE )
    {
        पटिनिवत्त norm(_src1, _src2, normType & ~NORM_RELATIVE, _mask)/(norm(_src2, normType, _mask) + DBL_EPSILON);
    }

    normType &= 7;
    CV_Assert( normType == NORM_INF || normType == NORM_L1 ||
               normType == NORM_L2 || normType == NORM_L2SQR ||
              ((normType == NORM_HAMMING || normType == NORM_HAMMING2) && src1.type() == CV_8U) );

    NormDiffFunc func = getNormDiffFunc(normType >> 1, depth);
    CV_Assert( (normType >> 1) >= 3 || func != 0 );

    सचे( src1.isContinuous() && src2.isContinuous() && mask.empty() )
    {
        size_t len = src1.total()*src1.channels();
        सचे( len == (size_t)(पूर्णाङ्क)len )
        {
            सचे( src1.depth() == CV_32F )
            {
                ध्रुव uchar* data1 = src1.ptr<ध्रुव uchar>();
                ध्रुव uchar* data2 = src2.ptr<ध्रुव uchar>();

                सचे( normType == NORM_L2 || normType == NORM_L2SQR || normType == NORM_L1 )
                {
                    द्विगुण result = 0;
                    func(data1, data2, 0, (uchar*)&result, (पूर्णाङ्क)len, 1);
                    पटिनिवत्त normType == NORM_L2 ? std::sqrt(result) : result;
                }
                सचे( normType == NORM_INF )
                {
                    भिन्न result = 0;
                    func(data1, data2, 0, (uchar*)&result, (पूर्णाङ्क)len, 1);
                    पटिनिवत्त result;
                }
            }
        }
    }

    CV_Assert( mask.empty() || mask.type() == CV_8U || mask.type() == CV_8S || mask.type() == CV_Bool );

    सचे( normType == NORM_HAMMING || normType == NORM_HAMMING2 )
    {
        सचे( !mask.empty() )
        {
            Mat temp;
            bitwise_xor(src1, src2, temp);
            bitwise_and(temp, mask, temp);
            पटिनिवत्त norm(temp, normType);
        }
        पूर्णाङ्क cellSize = normType == NORM_HAMMING ? 1 : 2;

        ध्रुव Mat* arrays[] = {&src1, &src2, 0};
        uchar* ptrs[2] = {};
        NAryMatIterator it(arrays, ptrs);
        पूर्णाङ्क total = (पूर्णाङ्क)it.size;
        पूर्णाङ्क result = 0;

        अत्थाय( size_t i = 0; i < it.nplanes; i++, ++it )
        {
            result += hal::normHamming(ptrs[0], ptrs[1], total, cellSize);
        }

        पटिनिवत्त result;
    }

    ध्रुव Mat* arrays[] = {&src1, &src2, &mask, 0};
    uchar* ptrs[3] = {};
    सङ्घ
    {
        द्विगुण d;
        भिन्न f;
        unsigned u;
        uint64 UL;
    }
    result;
    result.d = 0;
    NAryMatIterator it(arrays, ptrs);
    CV_CheckLT((size_t)it.size, (size_t)INT_MAX, "");

    तार्किक is_fp16 = depth == CV_16F || depth == CV_16BF;

    सचे ((normType == NORM_L1 && (depth <= CV_16S || depth == CV_Bool || is_fp16)) ||
        ((normType == NORM_L2 || normType == NORM_L2SQR) && (depth <= CV_8S || depth == CV_Bool || is_fp16)))
    {
        // special case to handle "integer" overflow in accumulator
        ध्रुव size_t esz = src1.elemSize();
        ध्रुव पूर्णाङ्क total = (पूर्णाङ्क)it.size;
        ध्रुव पूर्णाङ्क blockSize0 = (is_fp16 ? (1 << 10) :
            depth == CV_Bool ? (1 << 30) :
            normType == NORM_L1 && depth <= CV_8S ? (1 << 23) : (1 << 15))/cn;
        ध्रुव पूर्णाङ्क blockSize = std::min(total, blockSize0);
        सङ्घ {
            पूर्णाङ्क i;
            भिन्न f;
        } blocksum;
        blocksum.i = 0;
        पूर्णाङ्क गणनम्_2 = 0;

        अत्थाय (size_t i = 0; i < it.nplanes; i++, ++it)
        {
            अत्थाय (पूर्णाङ्क j = 0; j < total; j += blockSize)
            {
                पूर्णाङ्क bsz = std::min(total - j, blockSize);
                func(ptrs[0], ptrs[1], ptrs[2], (uchar*)&blocksum, bsz, cn);
                गणनम्_2 += bsz;
                सचे (गणनम्_2 + blockSize >= blockSize0 || (i+1 >= it.nplanes && j+bsz >= total))
                {
                    result.d += is_fp16 ? (द्विगुण)blocksum.f : (द्विगुण)blocksum.i;
                    blocksum.i = 0;
                    गणनम्_2 = 0;
                }
                ptrs[0] += bsz*esz;
                ptrs[1] += bsz*esz;
                सचे (ptrs[2])
                    ptrs[2] += bsz;
            }
        }
    }
    अञ्ञथा
    {
        // generic implementation
        अत्थाय (size_t i = 0; i < it.nplanes; i++, ++it)
        {
            func(ptrs[0], ptrs[1], ptrs[2], (uchar*)&result, (पूर्णाङ्क)it.size, cn);
        }
    }

    सचे( normType == NORM_INF )
    {
        सचे (depth <= CV_32S || depth == CV_32U || depth == CV_Bool)
            पटिनिवत्त result.u;
        सचे (depth == CV_32F || is_fp16)
            पटिनिवत्त result.f;
        सचे (depth == CV_64U || depth == CV_64S)
            पटिनिवत्त (द्विगुण)result.UL;
    }
    सचे( normType == NORM_L2 )
        पटिनिवत्त std::sqrt(result.d);

    पटिनिवत्त result.d;
}

cv::Hamming::ResultType Hamming::operator()( ध्रुव unsigned वर्ण* a, ध्रुव unsigned वर्ण* b, पूर्णाङ्क size ) ध्रुव
{
    पटिनिवत्त cv::hal::normHamming(a, b, size);
}

द्विगुण PSNR(निवेशः_Array _src1, निवेशः_Array _src2, द्विगुण R)
{
    CV_INSTRUMENT_REGION();

    //Input arrays must have depth CV_8U
    CV_Assert( _src1.type() == _src2.type() );

    द्विगुण diff = std::sqrt(norm(_src1, _src2, NORM_L2SQR)/(_src1.total()*_src1.channels()));
    पटिनिवत्त 20*log10(R/(diff+DBL_EPSILON));
}


#ifdef HAVE_OPENCL
स्थिर तार्किक ocl_normalize( निवेशः_Array _src, निवेशः_Output_Array _dst, निवेशः_Array _mask, पूर्णाङ्क dtype,
                           द्विगुण scale, द्विगुण delta )
{
    UMat src = _src.getUMat();

    सचे( _mask.empty() )
        src.convertTo( _dst, dtype, scale, delta );
    अञ्ञथा सचे (src.channels() <= 4)
    {
        ध्रुव ocl::Device & dev = ocl::Device::getDefault();

        पूर्णाङ्क stype = _src.type(), sdepth = CV_MAT_DEPTH(stype), cn = CV_MAT_CN(stype),
                ddepth = CV_MAT_DEPTH(dtype), wdepth = std::max(CV_32F, std::max(sdepth, ddepth)),
                rowsPerWI = dev.isIntel() ? 4 : 1;

        भिन्न fscale = static_cast<भिन्न>(scale), fdelta = static_cast<भिन्न>(delta);
        तार्किक haveScale = std::fabs(scale - 1) > DBL_EPSILON,
                haveZeroScale = !(std::fabs(scale) > DBL_EPSILON),
                haveDelta = std::fabs(delta) > DBL_EPSILON,
                doubleSupport = dev.doubleFPConfig() > 0;

        सचे (!haveScale && !haveDelta && stype == dtype)
        {
            _src.copyTo(_dst, _mask);
            पटिनिवत्त सच्च;
        }
        सचे (haveZeroScale)
        {
            _dst.setTo(Scalar(delta), _mask);
            पटिनिवत्त सच्च;
        }

        सचे ((sdepth == CV_64F || ddepth == CV_64F) && !doubleSupport)
            पटिनिवत्त मिच्छा;

        वर्ण cvt[2][50];
        String opts = format("-D srcT=%s -D dstT=%s -D convertToWT=%s -D cn=%d -D rowsPerWI=%d"
                             " -D convertToDT=%s -D workT=%s%s%s%s -D srcT1=%s -D dstT1=%s",
                             ocl::typeToStr(stype), ocl::typeToStr(dtype),
                             ocl::convertTypeStr(sdepth, wdepth, cn, cvt[0], परिमाण(cvt[0])), cn,
                             rowsPerWI, ocl::convertTypeStr(wdepth, ddepth, cn, cvt[1], परिमाण(cvt[1])),
                             ocl::typeToStr(CV_MAKE_TYPE(wdepth, cn)),
                             doubleSupport ? " -D DOUBLE_SUPPORT" : "",
                             haveScale ? " -D HAVE_SCALE" : "",
                             haveDelta ? " -D HAVE_DELTA" : "",
                             ocl::typeToStr(sdepth), ocl::typeToStr(ddepth));

        ocl::Kernel k("normalizek", ocl::core::normalize_oclsrc, opts);
        सचे (k.empty())
            पटिनिवत्त मिच्छा;

        UMat mask = _mask.getUMat(), dst = _dst.getUMat();

        ocl::KernelArg srcarg = ocl::KernelArg::ReadOnlyNoSize(src),
                maskarg = ocl::KernelArg::ReadOnlyNoSize(mask),
                dstarg = ocl::KernelArg::ReadWrite(dst);

        सचे (haveScale)
        {
            सचे (haveDelta)
                k.args(srcarg, maskarg, dstarg, fscale, fdelta);
            अञ्ञथा
                k.args(srcarg, maskarg, dstarg, fscale);
        }
        अञ्ञथा
        {
            सचे (haveDelta)
                k.args(srcarg, maskarg, dstarg, fdelta);
            अञ्ञथा
                k.args(srcarg, maskarg, dstarg);
        }

        size_t globalsize[2] = { (size_t)src.cols, ((size_t)src.rows + rowsPerWI - 1) / rowsPerWI };
        पटिनिवत्त k.run(2, globalsize, रिक्त, मिच्छा);
    }
    अञ्ञथा
    {
        UMat temp;
        src.convertTo( temp, dtype, scale, delta );
        temp.copyTo( _dst, _mask );
    }

    पटिनिवत्त सच्च;
}  // ocl_normalize
#endif  // HAVE_OPENCL

शून्य normalize(निवेशः_Array _src, निवेशः_Output_Array _dst, द्विगुण a, द्विगुण b,
               पूर्णाङ्क norm_type, पूर्णाङ्क rtype, निवेशः_Array _mask)
{
    CV_INSTRUMENT_REGION();

    द्विगुण scale = 1, shift = 0;
    पूर्णाङ्क type = _src.type(), depth = CV_MAT_DEPTH(type);

    सचे( rtype < 0 )
        rtype = _dst.fixedType() ? _dst.depth() : depth;

    सचे( norm_type == NORM_MINMAX )
    {
        द्विगुण smin = 0, smax = 0;
        द्विगुण dmin = MIN( a, b ), dmax = MAX( a, b );
        minMaxIdx( _src, &smin, &smax, 0, 0, _mask );
        scale = (dmax - dmin)*(smax - smin > DBL_EPSILON ? 1./(smax - smin) : 0);
        सचे( rtype == CV_32F )
        {
            scale = (भिन्न)scale;
            shift = (भिन्न)dmin - (भिन्न)(smin*scale);
        }
        अञ्ञथा
            shift = dmin - smin*scale;
    }
    अञ्ञथा सचे( norm_type == NORM_L2 || norm_type == NORM_L1 || norm_type == NORM_INF )
    {
        scale = norm( _src, norm_type, _mask );
        scale = scale > DBL_EPSILON ? a/scale : 0.;
        shift = 0;
    }
    अञ्ञथा
        CV_Error( cv::Error::StsBadArg, "Unknown/unsupported norm type" );

    CV_OCL_RUN(_dst.isUMat(),
               ocl_normalize(_src, _dst, _mask, rtype, scale, shift))

    Mat src = _src.getMat();
    सचे( _mask.empty() )
        src.convertTo( _dst, rtype, scale, shift );
    अञ्ञथा
    {
        Mat temp;
        src.convertTo( temp, rtype, scale, shift );
        temp.copyTo( _dst, _mask );
    }
}

}  // namespace
