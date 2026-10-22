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

बाह्य स्थिरांक uchar काढा_मोजणी_Table[256] =
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

स्थिर स्थिरांक uchar काढा_मोजणी_Table2[] =
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

स्थिर स्थिरांक uchar काढा_मोजणी_Table4[] =
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


पूर्णांक normHamming(स्थिरांक uchar* a, पूर्णांक n, पूर्णांक cellSize)
{
    पूर्णांक output;
    CALL_HAL_RET(normHamming8u, cv_hal_normHamming8u, output, a, n, cellSize);

    जर( cellSize == 1 )
        परत normHamming(a, n);
    स्थिरांक uchar* tab = 0;
    जर( cellSize == 2 )
        tab = काढा_मोजणी_Table2;
    अन्यथा जर( cellSize == 4 )
        tab = काढा_मोजणी_Table4;
    अन्यथा
        परत -1;
    पूर्णांक i = 0;
    पूर्णांक result = 0;
#जर (CV_SIMD || CV_SIMD_SCALABLE)
    v_uint64 t = vx_setzero_u64();
    जर ( cellSize == 2)
    {
        v_uint16 mask = v_reinterpret_as_u16(vx_setall_u8(0x55));
        साठी(; i <= n - VTraits<v_uint8>::vlanes(); i += VTraits<v_uint8>::vlanes())
        {
            v_uint16 a0 = v_reinterpret_as_u16(vx_load(a + i));
            t = v_add(t, v_popcount(v_reinterpret_as_u64(v_and(v_or(a0, v_shr<1>(a0)), mask))));
        }
    }
    अन्यथा    // cellSize == 4
    {
        v_uint16 mask = v_reinterpret_as_u16(vx_setall_u8(0x11));
        साठी(; i <= n - VTraits<v_uint8>::vlanes(); i += VTraits<v_uint8>::vlanes())
        {
            v_uint16 a0 = v_reinterpret_as_u16(vx_load(a + i));
            v_uint16 a1 = v_or(a0, v_shr<2>(a0));
            t = v_add(t, v_popcount(v_reinterpret_as_u64(v_and(v_or(a1, v_shr<1>(a1)), mask))));

        }
    }
    result += (पूर्णांक)v_reduce_sum(t);
    vx_cleanup();
#किंवाजर CV_ENABLE_UNROLLED
    साठी( ; i <= n - 4; i += 4 )
        result += tab[a[i]] + tab[a[i+1]] + tab[a[i+2]] + tab[a[i+3]];
#endif
    साठी( ; i < n; i++ )
        result += tab[a[i]];
    परत result;
}

पूर्णांक normHamming(स्थिरांक uchar* a, स्थिरांक uchar* b, पूर्णांक n, पूर्णांक cellSize)
{
    पूर्णांक output;
    CALL_HAL_RET(normHammingDiff8u, cv_hal_normHammingDiff8u, output, a, b, n, cellSize);

    जर( cellSize == 1 )
        परत normHamming(a, b, n);
    स्थिरांक uchar* tab = 0;
    जर( cellSize == 2 )
        tab = काढा_मोजणी_Table2;
    अन्यथा जर( cellSize == 4 )
        tab = काढा_मोजणी_Table4;
    अन्यथा
        परत -1;
    पूर्णांक i = 0;
    पूर्णांक result = 0;
#जर (CV_SIMD || CV_SIMD_SCALABLE)
    v_uint64 t = vx_setzero_u64();
    जर ( cellSize == 2)
    {
        v_uint16 mask = v_reinterpret_as_u16(vx_setall_u8(0x55));
        साठी(; i <= n - VTraits<v_uint8>::vlanes(); i += VTraits<v_uint8>::vlanes())
        {
            v_uint16 ab0 = v_reinterpret_as_u16(v_xor(vx_load(a + i), vx_load(b + i)));
            t = v_add(t, v_popcount(v_reinterpret_as_u64(v_and(v_or(ab0, v_shr<1>(ab0)), mask))));
        }
    }
    अन्यथा    // cellSize == 4
    {
        v_uint16 mask = v_reinterpret_as_u16(vx_setall_u8(0x11));
        साठी(; i <= n - VTraits<v_uint8>::vlanes(); i += VTraits<v_uint8>::vlanes())
        {
            v_uint16 ab0 = v_reinterpret_as_u16(v_xor(vx_load(a + i), vx_load(b + i)));
            v_uint16 ab1 = v_or(ab0, v_shr<2>(ab0));
            t = v_add(t, v_popcount(v_reinterpret_as_u64(v_and(v_or(ab1, v_shr<1>(ab1)), mask))));
        }
    }
    result += (पूर्णांक)v_reduce_sum(t);
    vx_cleanup();
#किंवाजर CV_ENABLE_UNROLLED
    साठी( ; i <= n - 4; i += 4 )
        result += tab[a[i] ^ b[i]] + tab[a[i+1] ^ b[i+1]] +
                tab[a[i+2] ^ b[i+2]] + tab[a[i+3] ^ b[i+3]];
#endif
    साठी( ; i < n; i++ )
        result += tab[a[i] ^ b[i]];
    परत result;
}

दशांश normL2Sqr_(स्थिरांक दशांश* a, स्थिरांक दशांश* b, पूर्णांक n)
{
    पूर्णांक j = 0; दशांश d = 0.f;
#जर (CV_SIMD || CV_SIMD_SCALABLE)
    स्थिरांक पूर्णांक vl = VTraits<v_float32>::vlanes();
    v_float32 v_d0 = vx_setzero_f32(), v_d1 = vx_setzero_f32();
    v_float32 v_d2 = vx_setzero_f32(), v_d3 = vx_setzero_f32();
    साठी (; j <= n - 4 * vl; j += 4 * vl)
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
    साठी (; j <= n - vl; j += vl)
    {
        v_float32 t0 = v_sub(vx_load(a + j), vx_load(b + j));
        v_d0 = v_muladd(t0, t0, v_d0);
    }
    d = v_reduce_sum(v_d0);
#endif
    साठी( ; j < n; j++ )
    {
        दशांश t = a[j] - b[j];
        d += t*t;
    }
    परत d;
}


दशांश normL1_(स्थिरांक दशांश* a, स्थिरांक दशांश* b, पूर्णांक n)
{
    पूर्णांक j = 0; दशांश d = 0.f;
#जर (CV_SIMD || CV_SIMD_SCALABLE)
    स्थिरांक पूर्णांक vl = VTraits<v_float32>::vlanes();
    v_float32 v_d0 = vx_setzero_f32(), v_d1 = vx_setzero_f32();
    v_float32 v_d2 = vx_setzero_f32(), v_d3 = vx_setzero_f32();
    साठी (; j <= n - 4 * vl; j += 4 * vl)
    {
        v_d0 = v_add(v_d0, v_absdiff(vx_load(a + j), vx_load(b + j)));
        v_d1 = v_add(v_d1, v_absdiff(vx_load(a + j + vl), vx_load(b + j + vl)));
        v_d2 = v_add(v_d2, v_absdiff(vx_load(a + j + 2 * vl), vx_load(b + j + 2 * vl)));
        v_d3 = v_add(v_d3, v_absdiff(vx_load(a + j + 3 * vl), vx_load(b + j + 3 * vl)));
    }
    v_d0 = v_add(v_add(v_d0, v_d1), v_add(v_d2, v_d3));
    साठी (; j <= n - vl; j += vl)
        v_d0 = v_add(v_d0, v_absdiff(vx_load(a + j), vx_load(b + j)));
    d = v_reduce_sum(v_d0);
#endif
    साठी( ; j < n; j++ )
        d += std::असल(a[j] - b[j]);
    परत d;
}

पूर्णांक normL1_(स्थिरांक uchar* a, स्थिरांक uchar* b, पूर्णांक n)
{
    पूर्णांक j = 0, d = 0;
#जर (CV_SIMD || CV_SIMD_SCALABLE)
    स्थिरांक पूर्णांक vl = VTraits<v_uint8>::vlanes();
    साठी (; j <= n - 4 * vl; j += 4 * vl)
        d += v_reduce_sad(vx_load(a + j), vx_load(b + j)) +
             v_reduce_sad(vx_load(a + j + vl), vx_load(b + j + vl)) +
             v_reduce_sad(vx_load(a + j + 2 * vl), vx_load(b + j + 2 * vl)) +
             v_reduce_sad(vx_load(a + j + 3 * vl), vx_load(b + j + 3 * vl));
    साठी (; j <= n - vl; j += vl)
        d += v_reduce_sad(vx_load(a + j), vx_load(b + j));
#endif
    साठी( ; j < n; j++ )
        d += std::असल(a[j] - b[j]);
    परत d;
}

} //cv::hal

//==================================================================================================

प्रकारव्याख्या पूर्णांक (*NormFunc)(स्थिरांक uchar*, स्थिरांक uchar*, uchar*, पूर्णांक, पूर्णांक);
प्रकारव्याख्या पूर्णांक (*NormDiffFunc)(स्थिरांक uchar*, स्थिरांक uchar*, स्थिरांक uchar*, uchar*, पूर्णांक, पूर्णांक);

#ifdef HAVE_OPENCL

स्थिर तार्किक ocl_norm( आदान_Array _src, पूर्णांक normType, आदान_Array _mask, द्विगुण & result )
{
    स्थिरांक ocl::Device & d = ocl::Device::getDefault();

#ifdef __ANDROID__
    जर (d.isNVidia())
        परत असत्य;
#endif
    स्थिरांक पूर्णांक cn = _src.channels();
    जर (cn > 4)
        परत असत्य;
    पूर्णांक type = _src.type(), depth = CV_MAT_DEPTH(type);
    तार्किक doubleSupport = d.doubleFPConfig() > 0,
            haveMask = _mask.kind() != आदान_Array_2::NONE;

    जर (depth >= CV_16F)
        परत असत्य;  // TODO: support FP16

    जर ( !(normType == NORM_INF || normType == NORM_L1 || normType == NORM_L2 || normType == NORM_L2SQR) ||
         (!doubleSupport && depth == CV_64F))
        परत असत्य;

    UMat src = _src.getUMat();

    जर (normType == NORM_INF)
    {
        जर (!ocl_minMaxIdx(_src, रिक्त, &result, रिक्त, रिक्त, _mask,
                           std::max(depth, CV_32S), depth != CV_8U && depth != CV_16U))
            परत असत्य;
    }
    अन्यथा जर (normType == NORM_L1 || normType == NORM_L2 || normType == NORM_L2SQR)
    {
        Scalar sc;
        तार्किक unstype = depth == CV_8U || depth == CV_16U;

        जर ( !ocl_sum(haveMask ? src : src.reshape(1), sc, normType == NORM_L2 || normType == NORM_L2SQR ?
                    OCL_OP_SUM_SQR : (unstype ? OCL_OP_SUM : OCL_OP_SUM_ABS), _mask) )
            परत असत्य;

        द्विगुण s = 0.0;
        साठी (पूर्णांक i = 0; i < (haveMask ? cn : 1); ++i)
            s += sc[i];

        result = normType == NORM_L1 || normType == NORM_L2SQR ? s : std::वर्ग_मूल(s);
    }

    परत सत्य;
}

#endif

स्थिर NormFunc getNormFunc(पूर्णांक normType, पूर्णांक depth) {
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getNormFunc, (normType, depth), CV_CPU_DISPATCH_MODES_ALL);
}
स्थिर NormDiffFunc getNormDiffFunc(पूर्णांक normType, पूर्णांक depth) {
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getNormDiffFunc, (normType, depth), CV_CPU_DISPATCH_MODES_ALL);
}

द्विगुण norm( आदान_Array _src, पूर्णांक normType, आदान_Array _mask )
{
    CV_INSTRUMENT_REGION();

    normType &= NORM_TYPE_MASK;
    CV_Assert( normType == NORM_INF || normType == NORM_L1 ||
               normType == NORM_L2 || normType == NORM_L2SQR ||
               ((normType == NORM_HAMMING || normType == NORM_HAMMING2) && _src.type() == CV_8U) );

#जर defined HAVE_OPENCL
    द्विगुण _result = 0;
#endif

#ifdef HAVE_OPENCL
    CV_OCL_RUN_(OCL_PERFORMANCE_CHECK(_src.isUMat()) && _src.dims() <= 2,
                ocl_norm(_src, normType, _mask, _result),
                _result)
#endif

    Mat src = _src.getMat(), mask = _mask.getMat();
    CV_Assert( mask.empty() || ((mask.type() == CV_8U || mask.type() == CV_8S || mask.type() == CV_Bool) && src.size == mask.size) );
    जर( src.empty() )
        परत 0.0;
    पूर्णांक depth = src.depth(), cn = src.channels();
    जर( src.dims <= 2 )
    {
        द्विगुण result;
        CALL_HAL_RET(norm, cv_hal_norm, result, src.data, src.step, mask.data, mask.step, src.cols, src.rows, src.type(), normType);
    }
    अन्यथा जर( src.isContinuous() && mask.isContinuous() )
    {
        द्विगुण result;
        CALL_HAL_RET(norm, cv_hal_norm, result, src.data, 0, mask.data, 0, (पूर्णांक)src.total(), 1, src.type(), normType);
    }

    NormFunc func = getNormFunc(normType >> 1, depth);
    CV_Assert( (normType >> 1) >= 3 || func != 0 );

    जर( src.isContinuous() && mask.empty() )
    {
        माप_प्रकार len = src.total()*cn;
        जर( len == (माप_प्रकार)(पूर्णांक)len )
        {
            जर( depth == CV_32F )
            {
                स्थिरांक uchar* data = src.ptr<स्थिरांक uchar>();

                जर( normType == NORM_L2 || normType == NORM_L2SQR || normType == NORM_L1 )
                {
                    द्विगुण result = 0;
                    func(data, 0, (uchar*)&result, (पूर्णांक)len, 1);
                    परत normType == NORM_L2 ? std::वर्ग_मूल(result) : result;
                }
                जर( normType == NORM_INF )
                {
                    दशांश result = 0;
                    func(data, 0, (uchar*)&result, (पूर्णांक)len, 1);
                    परत result;
                }
            }
            जर( depth == CV_8U )
            {
                स्थिरांक uchar* data = src.ptr<uchar>();

                जर( normType == NORM_HAMMING )
                {
                    परत hal::normHamming(data, (पूर्णांक)len, 1);
                }

                जर( normType == NORM_HAMMING2 )
                {
                    परत hal::normHamming(data, (पूर्णांक)len, 2);
                }
            }
        }
    }

    CV_Assert( mask.empty() || mask.type() == CV_8U || mask.type() == CV_8S || mask.type() == CV_Bool );

    जर( normType == NORM_HAMMING || normType == NORM_HAMMING2 )
    {
        जर( !mask.empty() )
        {
            Mat temp;
            bitwise_and(src, mask, temp);
            परत norm(temp, normType);
        }
        पूर्णांक cellSize = normType == NORM_HAMMING ? 1 : 2;

        स्थिरांक Mat* arrays[] = {&src, 0};
        uchar* ptrs[1] = {};
        NAryMatIterator it(arrays, ptrs);
        पूर्णांक total = (पूर्णांक)it.size;
        पूर्णांक result = 0;

        साठी( माप_प्रकार i = 0; i < it.nplanes; i++, ++it )
        {
            result += hal::normHamming(ptrs[0], total, cellSize);
        }

        परत result;
    }

    स्थिरांक Mat* arrays[] = {&src, &mask, 0};
    uchar* ptrs[2] = {};
    संघ
    {
        द्विगुण d;
        अचिन्हित u;
        uint64 UL;
        दशांश f;
    }
    result;
    result.d = 0;
    NAryMatIterator it(arrays, ptrs);
    CV_CheckLT((माप_प्रकार)it.size, (माप_प्रकार)पूर्णांक_उच्च, "");
    तार्किक is_fp16 = depth == CV_16F || depth == CV_16BF;

    जर ((normType == NORM_L1 && (depth <= CV_16S || depth == CV_Bool || is_fp16)) ||
        ((normType == NORM_L2 || normType == NORM_L2SQR) && (depth <= CV_8S || depth == CV_Bool || is_fp16)))
    {
        // special case to handle "integer" overflow in accumulator
        स्थिरांक माप_प्रकार esz = src.elemSize();
        स्थिरांक पूर्णांक total = (पूर्णांक)it.size;
        स्थिरांक पूर्णांक blockSize0 = (is_fp16 ? (1 << 10) :
            depth == CV_Bool ? (1 << 30) :
            normType == NORM_L1 && depth <= CV_8S ? (1 << 23) : (1 << 15))/cn;
        स्थिरांक पूर्णांक blockSize = std::min(total, blockSize0);
        संघ {
            पूर्णांक i;
            दशांश f;
        } blocksum;
        blocksum.i = 0;
        पूर्णांक मोजणी_2 = 0;

        साठी (माप_प्रकार i = 0; i < it.nplanes; i++, ++it)
        {
            साठी (पूर्णांक j = 0; j < total; j += blockSize)
            {
                पूर्णांक bsz = std::min(total - j, blockSize);
                func(ptrs[0], ptrs[1], (uchar*)&blocksum, bsz, cn);
                मोजणी_2 += bsz;
                जर (मोजणी_2 + blockSize >= blockSize0 || (i+1 >= it.nplanes && j+bsz >= total))
                {
                    result.d += is_fp16 ? (द्विगुण)blocksum.f : (द्विगुण)blocksum.i;
                    blocksum.i = 0;
                    मोजणी_2 = 0;
                }
                ptrs[0] += bsz*esz;
                जर (ptrs[1])
                    ptrs[1] += bsz;
            }
        }
    }
    अन्यथा
    {
        // generic implementation
        साठी (माप_प्रकार i = 0; i < it.nplanes; i++, ++it)
        {
            func(ptrs[0], ptrs[1], (uchar*)&result, (पूर्णांक)it.size, cn);
        }
    }

    जर( normType == NORM_INF )
    {
        जर(depth <= CV_32S || depth == CV_32U || depth == CV_Bool)
            परत result.u;
        जर (depth == CV_32F || is_fp16)
            परत result.f;
        जर (depth == CV_64U || depth == CV_64S)
            परत (द्विगुण)result.UL;
    }
    जर( normType == NORM_L2 )
        परत std::वर्ग_मूल(result.d);

    परत result.d;
}

//==================================================================================================

#ifdef HAVE_OPENCL
स्थिर तार्किक ocl_norm( आदान_Array _src1, आदान_Array _src2, पूर्णांक normType, आदान_Array _mask, द्विगुण & result )
{
#ifdef __ANDROID__
    जर (ocl::Device::getDefault().isNVidia())
        परत असत्य;
#endif

    Scalar sc1, sc2;
    पूर्णांक cn = _src1.channels();
    जर (cn > 4)
        परत असत्य;
    पूर्णांक type = _src1.type(), depth = CV_MAT_DEPTH(type);
    तार्किक relative = (normType & NORM_RELATIVE) != 0;
    normType &= ~NORM_RELATIVE;
    तार्किक normsum = normType == NORM_L1 || normType == NORM_L2 || normType == NORM_L2SQR;

#ifdef __APPLE__
    जर(normType == NORM_L1 && type == CV_16UC3 && !_mask.empty())
        परत असत्य;
#endif

    जर (normsum)
    {
        जर (!ocl_sum(_src1, sc1, normType == NORM_L2 || normType == NORM_L2SQR ?
                     OCL_OP_SUM_SQR : OCL_OP_SUM, _mask, _src2, relative, sc2))
            परत असत्य;
    }
    अन्यथा
    {
        जर (!ocl_minMaxIdx(_src1, रिक्त, &sc1[0], रिक्त, रिक्त, _mask, std::max(CV_32S, depth),
                           असत्य, _src2, relative ? &sc2[0] : रिक्त))
            परत असत्य;
        cn = 1;
    }

    द्विगुण s2 = 0;
    साठी (पूर्णांक i = 0; i < cn; ++i)
    {
        result += sc1[i];
        जर (relative)
            s2 += sc2[i];
    }

    जर (normType == NORM_L2)
    {
        result = std::वर्ग_मूल(result);
        जर (relative)
            s2 = std::वर्ग_मूल(s2);
    }

    जर (relative)
        result /= (s2 + द्विग_अंतर);

    परत सत्य;
}  // ocl_norm()
#endif  // HAVE_OPENCL

द्विगुण norm( आदान_Array _src1, आदान_Array _src2, पूर्णांक normType, आदान_Array _mask )
{
    CV_INSTRUMENT_REGION();

    CV_CheckTypeEQ(_src1.type(), _src2.type(), "Input type mismatch");
    CV_Assert(_src1.sameSize(_src2));

#जर defined HAVE_OPENCL
    द्विगुण _result = 0;
#endif

#ifdef HAVE_OPENCL
    CV_OCL_RUN_(OCL_PERFORMANCE_CHECK(_src1.isUMat()),
                ocl_norm(_src1, _src2, normType, _mask, _result),
                _result)
#endif

    Mat src1 = _src1.getMat(), src2 = _src2.getMat(), mask = _mask.getMat();
    CV_Assert( mask.empty() || ((mask.type() == CV_8U || mask.type() == CV_8S || mask.type() == CV_Bool) && src1.size == mask.size) );
    जर( src1.empty() || src2.empty() )
        परत 0.0;
    पूर्णांक depth = src1.depth(), cn = src1.channels();
    जर( src1.dims <= 2 )
    {
        द्विगुण result;
        CALL_HAL_RET(normDiff, cv_hal_normDiff, result, src1.data, src1.step, src2.data, src2.step, mask.data, mask.step, src1.cols, src1.rows, src1.type(), normType);
    }
    अन्यथा जर( src1.isContinuous() && src2.isContinuous() && mask.isContinuous() )
    {
        द्विगुण result;
        CALL_HAL_RET(normDiff, cv_hal_normDiff, result, src1.data, 0, src2.data, 0, mask.data, 0, (पूर्णांक)src1.total(), 1, src1.type(), normType);
    }

    जर( normType & NORM_RELATIVE )
    {
        परत norm(_src1, _src2, normType & ~NORM_RELATIVE, _mask)/(norm(_src2, normType, _mask) + द्विग_अंतर);
    }

    normType &= 7;
    CV_Assert( normType == NORM_INF || normType == NORM_L1 ||
               normType == NORM_L2 || normType == NORM_L2SQR ||
              ((normType == NORM_HAMMING || normType == NORM_HAMMING2) && src1.type() == CV_8U) );

    NormDiffFunc func = getNormDiffFunc(normType >> 1, depth);
    CV_Assert( (normType >> 1) >= 3 || func != 0 );

    जर( src1.isContinuous() && src2.isContinuous() && mask.empty() )
    {
        माप_प्रकार len = src1.total()*src1.channels();
        जर( len == (माप_प्रकार)(पूर्णांक)len )
        {
            जर( src1.depth() == CV_32F )
            {
                स्थिरांक uchar* data1 = src1.ptr<स्थिरांक uchar>();
                स्थिरांक uchar* data2 = src2.ptr<स्थिरांक uchar>();

                जर( normType == NORM_L2 || normType == NORM_L2SQR || normType == NORM_L1 )
                {
                    द्विगुण result = 0;
                    func(data1, data2, 0, (uchar*)&result, (पूर्णांक)len, 1);
                    परत normType == NORM_L2 ? std::वर्ग_मूल(result) : result;
                }
                जर( normType == NORM_INF )
                {
                    दशांश result = 0;
                    func(data1, data2, 0, (uchar*)&result, (पूर्णांक)len, 1);
                    परत result;
                }
            }
        }
    }

    CV_Assert( mask.empty() || mask.type() == CV_8U || mask.type() == CV_8S || mask.type() == CV_Bool );

    जर( normType == NORM_HAMMING || normType == NORM_HAMMING2 )
    {
        जर( !mask.empty() )
        {
            Mat temp;
            bitwise_xor(src1, src2, temp);
            bitwise_and(temp, mask, temp);
            परत norm(temp, normType);
        }
        पूर्णांक cellSize = normType == NORM_HAMMING ? 1 : 2;

        स्थिरांक Mat* arrays[] = {&src1, &src2, 0};
        uchar* ptrs[2] = {};
        NAryMatIterator it(arrays, ptrs);
        पूर्णांक total = (पूर्णांक)it.size;
        पूर्णांक result = 0;

        साठी( माप_प्रकार i = 0; i < it.nplanes; i++, ++it )
        {
            result += hal::normHamming(ptrs[0], ptrs[1], total, cellSize);
        }

        परत result;
    }

    स्थिरांक Mat* arrays[] = {&src1, &src2, &mask, 0};
    uchar* ptrs[3] = {};
    संघ
    {
        द्विगुण d;
        दशांश f;
        अचिन्हित u;
        uint64 UL;
    }
    result;
    result.d = 0;
    NAryMatIterator it(arrays, ptrs);
    CV_CheckLT((माप_प्रकार)it.size, (माप_प्रकार)पूर्णांक_उच्च, "");

    तार्किक is_fp16 = depth == CV_16F || depth == CV_16BF;

    जर ((normType == NORM_L1 && (depth <= CV_16S || depth == CV_Bool || is_fp16)) ||
        ((normType == NORM_L2 || normType == NORM_L2SQR) && (depth <= CV_8S || depth == CV_Bool || is_fp16)))
    {
        // special case to handle "integer" overflow in accumulator
        स्थिरांक माप_प्रकार esz = src1.elemSize();
        स्थिरांक पूर्णांक total = (पूर्णांक)it.size;
        स्थिरांक पूर्णांक blockSize0 = (is_fp16 ? (1 << 10) :
            depth == CV_Bool ? (1 << 30) :
            normType == NORM_L1 && depth <= CV_8S ? (1 << 23) : (1 << 15))/cn;
        स्थिरांक पूर्णांक blockSize = std::min(total, blockSize0);
        संघ {
            पूर्णांक i;
            दशांश f;
        } blocksum;
        blocksum.i = 0;
        पूर्णांक मोजणी_2 = 0;

        साठी (माप_प्रकार i = 0; i < it.nplanes; i++, ++it)
        {
            साठी (पूर्णांक j = 0; j < total; j += blockSize)
            {
                पूर्णांक bsz = std::min(total - j, blockSize);
                func(ptrs[0], ptrs[1], ptrs[2], (uchar*)&blocksum, bsz, cn);
                मोजणी_2 += bsz;
                जर (मोजणी_2 + blockSize >= blockSize0 || (i+1 >= it.nplanes && j+bsz >= total))
                {
                    result.d += is_fp16 ? (द्विगुण)blocksum.f : (द्विगुण)blocksum.i;
                    blocksum.i = 0;
                    मोजणी_2 = 0;
                }
                ptrs[0] += bsz*esz;
                ptrs[1] += bsz*esz;
                जर (ptrs[2])
                    ptrs[2] += bsz;
            }
        }
    }
    अन्यथा
    {
        // generic implementation
        साठी (माप_प्रकार i = 0; i < it.nplanes; i++, ++it)
        {
            func(ptrs[0], ptrs[1], ptrs[2], (uchar*)&result, (पूर्णांक)it.size, cn);
        }
    }

    जर( normType == NORM_INF )
    {
        जर (depth <= CV_32S || depth == CV_32U || depth == CV_Bool)
            परत result.u;
        जर (depth == CV_32F || is_fp16)
            परत result.f;
        जर (depth == CV_64U || depth == CV_64S)
            परत (द्विगुण)result.UL;
    }
    जर( normType == NORM_L2 )
        परत std::वर्ग_मूल(result.d);

    परत result.d;
}

cv::Hamming::ResultType Hamming::चालक()( स्थिरांक अचिन्हित अक्षर* a, स्थिरांक अचिन्हित अक्षर* b, पूर्णांक size ) स्थिरांक
{
    परत cv::hal::normHamming(a, b, size);
}

द्विगुण PSNR(आदान_Array _src1, आदान_Array _src2, द्विगुण R)
{
    CV_INSTRUMENT_REGION();

    //Input arrays must have depth CV_8U
    CV_Assert( _src1.type() == _src2.type() );

    द्विगुण diff = std::वर्ग_मूल(norm(_src1, _src2, NORM_L2SQR)/(_src1.total()*_src1.channels()));
    परत 20*log10(R/(diff+द्विग_अंतर));
}


#ifdef HAVE_OPENCL
स्थिर तार्किक ocl_normalize( आदान_Array _src, आदान_Output_Array _dst, आदान_Array _mask, पूर्णांक dtype,
                           द्विगुण scale, द्विगुण delta )
{
    UMat src = _src.getUMat();

    जर( _mask.empty() )
        src.convertTo( _dst, dtype, scale, delta );
    अन्यथा जर (src.channels() <= 4)
    {
        स्थिरांक ocl::Device & dev = ocl::Device::getDefault();

        पूर्णांक stype = _src.type(), sdepth = CV_MAT_DEPTH(stype), cn = CV_MAT_CN(stype),
                ddepth = CV_MAT_DEPTH(dtype), wdepth = std::max(CV_32F, std::max(sdepth, ddepth)),
                rowsPerWI = dev.isIntel() ? 4 : 1;

        दशांश fscale = static_cast<दशांश>(scale), fdelta = static_cast<दशांश>(delta);
        तार्किक haveScale = std::भ_असल(scale - 1) > द्विग_अंतर,
                haveZeroScale = !(std::भ_असल(scale) > द्विग_अंतर),
                haveDelta = std::भ_असल(delta) > द्विग_अंतर,
                doubleSupport = dev.doubleFPConfig() > 0;

        जर (!haveScale && !haveDelta && stype == dtype)
        {
            _src.copyTo(_dst, _mask);
            परत सत्य;
        }
        जर (haveZeroScale)
        {
            _dst.setTo(Scalar(delta), _mask);
            परत सत्य;
        }

        जर ((sdepth == CV_64F || ddepth == CV_64F) && !doubleSupport)
            परत असत्य;

        अक्षर cvt[2][50];
        String opts = format("-D srcT=%s -D dstT=%s -D convertToWT=%s -D cn=%d -D rowsPerWI=%d"
                             " -D convertToDT=%s -D workT=%s%s%s%s -D srcT1=%s -D dstT1=%s",
                             ocl::typeToStr(stype), ocl::typeToStr(dtype),
                             ocl::convertTypeStr(sdepth, wdepth, cn, cvt[0], आकार(cvt[0])), cn,
                             rowsPerWI, ocl::convertTypeStr(wdepth, ddepth, cn, cvt[1], आकार(cvt[1])),
                             ocl::typeToStr(CV_MAKE_TYPE(wdepth, cn)),
                             doubleSupport ? " -D DOUBLE_SUPPORT" : "",
                             haveScale ? " -D HAVE_SCALE" : "",
                             haveDelta ? " -D HAVE_DELTA" : "",
                             ocl::typeToStr(sdepth), ocl::typeToStr(ddepth));

        ocl::Kernel k("normalizek", ocl::core::normalize_oclsrc, opts);
        जर (k.empty())
            परत असत्य;

        UMat mask = _mask.getUMat(), dst = _dst.getUMat();

        ocl::KernelArg srcarg = ocl::KernelArg::ReadOnlyNoSize(src),
                maskarg = ocl::KernelArg::ReadOnlyNoSize(mask),
                dstarg = ocl::KernelArg::ReadWrite(dst);

        जर (haveScale)
        {
            जर (haveDelta)
                k.args(srcarg, maskarg, dstarg, fscale, fdelta);
            अन्यथा
                k.args(srcarg, maskarg, dstarg, fscale);
        }
        अन्यथा
        {
            जर (haveDelta)
                k.args(srcarg, maskarg, dstarg, fdelta);
            अन्यथा
                k.args(srcarg, maskarg, dstarg);
        }

        माप_प्रकार globalsize[2] = { (माप_प्रकार)src.cols, ((माप_प्रकार)src.rows + rowsPerWI - 1) / rowsPerWI };
        परत k.run(2, globalsize, रिक्त, असत्य);
    }
    अन्यथा
    {
        UMat temp;
        src.convertTo( temp, dtype, scale, delta );
        temp.copyTo( _dst, _mask );
    }

    परत सत्य;
}  // ocl_normalize
#endif  // HAVE_OPENCL

शून्य normalize(आदान_Array _src, आदान_Output_Array _dst, द्विगुण a, द्विगुण b,
               पूर्णांक norm_type, पूर्णांक rtype, आदान_Array _mask)
{
    CV_INSTRUMENT_REGION();

    द्विगुण scale = 1, shift = 0;
    पूर्णांक type = _src.type(), depth = CV_MAT_DEPTH(type);

    जर( rtype < 0 )
        rtype = _dst.fixedType() ? _dst.depth() : depth;

    जर( norm_type == NORM_MINMAX )
    {
        द्विगुण smin = 0, smax = 0;
        द्विगुण dmin = MIN( a, b ), dmax = MAX( a, b );
        minMaxIdx( _src, &smin, &smax, 0, 0, _mask );
        scale = (dmax - dmin)*(smax - smin > द्विग_अंतर ? 1./(smax - smin) : 0);
        जर( rtype == CV_32F )
        {
            scale = (दशांश)scale;
            shift = (दशांश)dmin - (दशांश)(smin*scale);
        }
        अन्यथा
            shift = dmin - smin*scale;
    }
    अन्यथा जर( norm_type == NORM_L2 || norm_type == NORM_L1 || norm_type == NORM_INF )
    {
        scale = norm( _src, norm_type, _mask );
        scale = scale > द्विग_अंतर ? a/scale : 0.;
        shift = 0;
    }
    अन्यथा
        CV_Error( cv::Error::StsBadArg, "Unknown/unsupported norm type" );

    CV_OCL_RUN(_dst.isUMat(),
               ocl_normalize(_src, _dst, _mask, rtype, scale, shift))

    Mat src = _src.getMat();
    जर( _mask.empty() )
        src.convertTo( _dst, rtype, scale, shift );
    अन्यथा
    {
        Mat temp;
        src.convertTo( temp, rtype, scale, shift );
        temp.copyTo( _dst, _mask );
    }
}

}  // namespace
