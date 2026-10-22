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

बाह्य ध्रुव uchar निष्कासय_गणनम्_सारणी[256] =
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


पूर्णाङ्क normHamming(ध्रुव uchar* a, पूर्णाङ्क n, पूर्णाङ्क cell_आकार)
{
    पूर्णाङ्क output;
    CALL_HAL_RET(normHamming8u, cv_hal_normHamming8u, output, a, n, cell_आकार);

    यदि( cell_आकार == 1 )
        प्रत्यागम normHamming(a, n);
    ध्रुव uchar* tab = 0;
    यदि( cell_आकार == 2 )
        tab = निष्कासय_गणनम्_Table2;
    अन्यथा यदि( cell_आकार == 4 )
        tab = निष्कासय_गणनम्_Table4;
    अन्यथा
        प्रत्यागम -1;
    पूर्णाङ्क i = 0;
    पूर्णाङ्क परिणाम = 0;
#यदि (CV_SIMD || CV_SIMD_SCALABLE)
    v_uint64 t = vx_setzero_u64();
    यदि ( cell_आकार == 2)
    {
        v_uint16 आवरणम् = v_reinterpret_as_u16(vx_setall_u8(0x55));
        प्रति(; i <= n - VTraits<v_uint8>::vlanes(); i += VTraits<v_uint8>::vlanes())
        {
            v_uint16 a0 = v_reinterpret_as_u16(vx_आरोपय(a + i));
            t = v_सङ्कलय(t, v_popcount(v_reinterpret_as_u64(v_and(v_or(a0, v_shr<1>(a0)), आवरणम्))));
        }
    }
    अन्यथा    // cellSize == 4
    {
        v_uint16 आवरणम् = v_reinterpret_as_u16(vx_setall_u8(0x11));
        प्रति(; i <= n - VTraits<v_uint8>::vlanes(); i += VTraits<v_uint8>::vlanes())
        {
            v_uint16 a0 = v_reinterpret_as_u16(vx_आरोपय(a + i));
            v_uint16 a1 = v_or(a0, v_shr<2>(a0));
            t = v_सङ्कलय(t, v_popcount(v_reinterpret_as_u64(v_and(v_or(a1, v_shr<1>(a1)), आवरणम्))));

        }
    }
    परिणाम += (पूर्णाङ्क)v_reduce_योग(t);
    vx_cleanup();
#नोचेद्यदि CV_ENABLE_UNROLLED
    प्रति( ; i <= n - 4; i += 4 )
        परिणाम += tab[a[i]] + tab[a[i+1]] + tab[a[i+2]] + tab[a[i+3]];
#यदि_अन्तः
    प्रति( ; i < n; i++ )
        परिणाम += tab[a[i]];
    प्रत्यागम परिणाम;
}

पूर्णाङ्क normHamming(ध्रुव uchar* a, ध्रुव uchar* b, पूर्णाङ्क n, पूर्णाङ्क cell_आकार)
{
    पूर्णाङ्क output;
    CALL_HAL_RET(normHammingDiff8u, cv_hal_normHammingDiff8u, output, a, b, n, cell_आकार);

    यदि( cell_आकार == 1 )
        प्रत्यागम normHamming(a, b, n);
    ध्रुव uchar* tab = 0;
    यदि( cell_आकार == 2 )
        tab = निष्कासय_गणनम्_Table2;
    अन्यथा यदि( cell_आकार == 4 )
        tab = निष्कासय_गणनम्_Table4;
    अन्यथा
        प्रत्यागम -1;
    पूर्णाङ्क i = 0;
    पूर्णाङ्क परिणाम = 0;
#यदि (CV_SIMD || CV_SIMD_SCALABLE)
    v_uint64 t = vx_setzero_u64();
    यदि ( cell_आकार == 2)
    {
        v_uint16 आवरणम् = v_reinterpret_as_u16(vx_setall_u8(0x55));
        प्रति(; i <= n - VTraits<v_uint8>::vlanes(); i += VTraits<v_uint8>::vlanes())
        {
            v_uint16 ab0 = v_reinterpret_as_u16(v_xor(vx_आरोपय(a + i), vx_आरोपय(b + i)));
            t = v_सङ्कलय(t, v_popcount(v_reinterpret_as_u64(v_and(v_or(ab0, v_shr<1>(ab0)), आवरणम्))));
        }
    }
    अन्यथा    // cellSize == 4
    {
        v_uint16 आवरणम् = v_reinterpret_as_u16(vx_setall_u8(0x11));
        प्रति(; i <= n - VTraits<v_uint8>::vlanes(); i += VTraits<v_uint8>::vlanes())
        {
            v_uint16 ab0 = v_reinterpret_as_u16(v_xor(vx_आरोपय(a + i), vx_आरोपय(b + i)));
            v_uint16 ab1 = v_or(ab0, v_shr<2>(ab0));
            t = v_सङ्कलय(t, v_popcount(v_reinterpret_as_u64(v_and(v_or(ab1, v_shr<1>(ab1)), आवरणम्))));
        }
    }
    परिणाम += (पूर्णाङ्क)v_reduce_योग(t);
    vx_cleanup();
#नोचेद्यदि CV_ENABLE_UNROLLED
    प्रति( ; i <= n - 4; i += 4 )
        परिणाम += tab[a[i] ^ b[i]] + tab[a[i+1] ^ b[i+1]] +
                tab[a[i+2] ^ b[i+2]] + tab[a[i+3] ^ b[i+3]];
#यदि_अन्तः
    प्रति( ; i < n; i++ )
        परिणाम += tab[a[i] ^ b[i]];
    प्रत्यागम परिणाम;
}

भिन्न normL2Sqr_(ध्रुव भिन्न* a, ध्रुव भिन्न* b, पूर्णाङ्क n)
{
    पूर्णाङ्क j = 0; भिन्न d = 0.f;
#यदि (CV_SIMD || CV_SIMD_SCALABLE)
    ध्रुव पूर्णाङ्क vl = VTraits<v_float32>::vlanes();
    v_float32 v_d0 = vx_setzero_f32(), v_d1 = vx_setzero_f32();
    v_float32 v_d2 = vx_setzero_f32(), v_d3 = vx_setzero_f32();
    प्रति (; j <= n - 4 * vl; j += 4 * vl)
    {
        v_float32 t0 = v_sub(vx_आरोपय(a + j), vx_आरोपय(b + j));
        v_float32 t1 = v_sub(vx_आरोपय(a + j + vl), vx_आरोपय(b + j + vl));
        v_d0 = v_muladd(t0, t0, v_d0);
        v_float32 t2 = v_sub(vx_आरोपय(a + j + 2 * vl), vx_आरोपय(b + j + 2 * vl));
        v_d1 = v_muladd(t1, t1, v_d1);
        v_float32 t3 = v_sub(vx_आरोपय(a + j + 3 * vl), vx_आरोपय(b + j + 3 * vl));
        v_d2 = v_muladd(t2, t2, v_d2);
        v_d3 = v_muladd(t3, t3, v_d3);
    }
    v_d0 = v_सङ्कलय(v_सङ्कलय(v_d0, v_d1), v_सङ्कलय(v_d2, v_d3));
    प्रति (; j <= n - vl; j += vl)
    {
        v_float32 t0 = v_sub(vx_आरोपय(a + j), vx_आरोपय(b + j));
        v_d0 = v_muladd(t0, t0, v_d0);
    }
    d = v_reduce_योग(v_d0);
#यदि_अन्तः
    प्रति( ; j < n; j++ )
    {
        भिन्न t = a[j] - b[j];
        d += t*t;
    }
    प्रत्यागम d;
}


भिन्न normL1_(ध्रुव भिन्न* a, ध्रुव भिन्न* b, पूर्णाङ्क n)
{
    पूर्णाङ्क j = 0; भिन्न d = 0.f;
#यदि (CV_SIMD || CV_SIMD_SCALABLE)
    ध्रुव पूर्णाङ्क vl = VTraits<v_float32>::vlanes();
    v_float32 v_d0 = vx_setzero_f32(), v_d1 = vx_setzero_f32();
    v_float32 v_d2 = vx_setzero_f32(), v_d3 = vx_setzero_f32();
    प्रति (; j <= n - 4 * vl; j += 4 * vl)
    {
        v_d0 = v_सङ्कलय(v_d0, v_absdiff(vx_आरोपय(a + j), vx_आरोपय(b + j)));
        v_d1 = v_सङ्कलय(v_d1, v_absdiff(vx_आरोपय(a + j + vl), vx_आरोपय(b + j + vl)));
        v_d2 = v_सङ्कलय(v_d2, v_absdiff(vx_आरोपय(a + j + 2 * vl), vx_आरोपय(b + j + 2 * vl)));
        v_d3 = v_सङ्कलय(v_d3, v_absdiff(vx_आरोपय(a + j + 3 * vl), vx_आरोपय(b + j + 3 * vl)));
    }
    v_d0 = v_सङ्कलय(v_सङ्कलय(v_d0, v_d1), v_सङ्कलय(v_d2, v_d3));
    प्रति (; j <= n - vl; j += vl)
        v_d0 = v_सङ्कलय(v_d0, v_absdiff(vx_आरोपय(a + j), vx_आरोपय(b + j)));
    d = v_reduce_योग(v_d0);
#यदि_अन्तः
    प्रति( ; j < n; j++ )
        d += std::निरपेक्षम्(a[j] - b[j]);
    प्रत्यागम d;
}

पूर्णाङ्क normL1_(ध्रुव uchar* a, ध्रुव uchar* b, पूर्णाङ्क n)
{
    पूर्णाङ्क j = 0, d = 0;
#यदि (CV_SIMD || CV_SIMD_SCALABLE)
    ध्रुव पूर्णाङ्क vl = VTraits<v_uint8>::vlanes();
    प्रति (; j <= n - 4 * vl; j += 4 * vl)
        d += v_reduce_sad(vx_आरोपय(a + j), vx_आरोपय(b + j)) +
             v_reduce_sad(vx_आरोपय(a + j + vl), vx_आरोपय(b + j + vl)) +
             v_reduce_sad(vx_आरोपय(a + j + 2 * vl), vx_आरोपय(b + j + 2 * vl)) +
             v_reduce_sad(vx_आरोपय(a + j + 3 * vl), vx_आरोपय(b + j + 3 * vl));
    प्रति (; j <= n - vl; j += vl)
        d += v_reduce_sad(vx_आरोपय(a + j), vx_आरोपय(b + j));
#यदि_अन्तः
    प्रति( ; j < n; j++ )
        d += std::निरपेक्षम्(a[j] - b[j]);
    प्रत्यागम d;
}

} //cv::hal

//==================================================================================================

प्रकारनिर्देश पूर्णाङ्क (*NormFunc)(ध्रुव uchar*, ध्रुव uchar*, uchar*, पूर्णाङ्क, पूर्णाङ्क);
प्रकारनिर्देश पूर्णाङ्क (*NormDiffFunc)(ध्रुव uchar*, ध्रुव uchar*, ध्रुव uchar*, uchar*, पूर्णाङ्क, पूर्णाङ्क);

#यदि_परिभाषितम् HAVE_OPENCL

स्थिर तार्किक ocl_norm( निवेशः_Array _src, पूर्णाङ्क normType, निवेशः_Array आवरणम्_2, द्विगुण & परिणाम )
{
    ध्रुव ocl::Device & d = ocl::Device::getDefault();

#यदि_परिभाषितम् __ANDROID__
    यदि (d.isNVidia())
        प्रत्यागम असत्य;
#यदि_अन्तः
    ध्रुव पूर्णाङ्क cn = _src.channels();
    यदि (cn > 4)
        प्रत्यागम असत्य;
    पूर्णाङ्क type = _src.type(), गभीरता = CV_MAT_गभीरता(type);
    तार्किक doubleSupport = d.doubleFPConfig() > 0,
            have_आवरणम् = आवरणम्_2.विधा() != निवेशः_Array_2::NONE;

    यदि (गभीरता >= CV_16F)
        प्रत्यागम असत्य;  // TODO: support FP16

    यदि ( !(normType == NORM_INF || normType == NORM_L1 || normType == NORM_L2 || normType == NORM_L2SQR) ||
         (!doubleSupport && गभीरता == CV_64F))
        प्रत्यागम असत्य;

    UMat src = _src.getUMat();

    यदि (normType == NORM_INF)
    {
        यदि (!ocl_minMaxIdx(_src, रिक्त, &परिणाम, रिक्त, रिक्त, आवरणम्_2,
                           std::max(गभीरता, CV_32S), गभीरता != CV_8U && गभीरता != CV_16U))
            प्रत्यागम असत्य;
    }
    अन्यथा यदि (normType == NORM_L1 || normType == NORM_L2 || normType == NORM_L2SQR)
    {
        Scalar sc;
        तार्किक unstype = गभीरता == CV_8U || गभीरता == CV_16U;

        यदि ( !ocl_योग(have_आवरणम् ? src : src.reshape(1), sc, normType == NORM_L2 || normType == NORM_L2SQR ?
                    OCL_OP_योग_SQR : (unstype ? OCL_OP_योग : OCL_OP_योग_ABS), आवरणम्_2) )
            प्रत्यागम असत्य;

        द्विगुण s = 0.0;
        प्रति (पूर्णाङ्क i = 0; i < (have_आवरणम् ? cn : 1); ++i)
            s += sc[i];

        परिणाम = normType == NORM_L1 || normType == NORM_L2SQR ? s : std::वर्गमूलम्(s);
    }

    प्रत्यागम सत्य;
}

#यदि_अन्तः

स्थिर NormFunc getNormFunc(पूर्णाङ्क normType, पूर्णाङ्क गभीरता) {
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getNormFunc, (normType, गभीरता), CV_CPU_DISPATCH_MODES_ALL);
}
स्थिर NormDiffFunc getNormDiffFunc(पूर्णाङ्क normType, पूर्णाङ्क गभीरता) {
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getNormDiffFunc, (normType, गभीरता), CV_CPU_DISPATCH_MODES_ALL);
}

द्विगुण norm( निवेशः_Array _src, पूर्णाङ्क normType, निवेशः_Array आवरणम्_2 )
{
    CV_INSTRUMENT_REGION();

    normType &= NORM_TYPE_आवरणम्;
    CV_Assert( normType == NORM_INF || normType == NORM_L1 ||
               normType == NORM_L2 || normType == NORM_L2SQR ||
               ((normType == NORM_HAMMING || normType == NORM_HAMMING2) && _src.type() == CV_8U) );

#यदि defined HAVE_OPENCL
    द्विगुण परिणाम_2 = 0;
#यदि_अन्तः

#यदि_परिभाषितम् HAVE_OPENCL
    CV_OCL_सततम्_2(OCL_PERFORMANCE_CHECK(_src.isUMat()) && _src.dims() <= 2,
                ocl_norm(_src, normType, आवरणम्_2, परिणाम_2),
                परिणाम_2)
#यदि_अन्तः

    Mat src = _src.getMat(), आवरणम् = आवरणम्_2.getMat();
    CV_Assert( आवरणम्.empty() || ((आवरणम्.type() == CV_8U || आवरणम्.type() == CV_8S || आवरणम्.type() == CV_Bool) && src.size == आवरणम्.size) );
    यदि( src.empty() )
        प्रत्यागम 0.0;
    पूर्णाङ्क गभीरता = src.गभीरता(), cn = src.channels();
    यदि( src.dims <= 2 )
    {
        द्विगुण परिणाम;
        CALL_HAL_RET(norm, cv_hal_norm, परिणाम, src.दत्त, src.चरणः, आवरणम्.दत्त, आवरणम्.चरणः, src.स्तम्भसङ्ख्या, src.पङ्क्तिकाः, src.type(), normType);
    }
    अन्यथा यदि( src.isContinuous() && आवरणम्.isContinuous() )
    {
        द्विगुण परिणाम;
        CALL_HAL_RET(norm, cv_hal_norm, परिणाम, src.दत्त, 0, आवरणम्.दत्त, 0, (पूर्णाङ्क)src.योग(), 1, src.type(), normType);
    }

    NormFunc func = getNormFunc(normType >> 1, गभीरता);
    CV_Assert( (normType >> 1) >= 3 || func != 0 );

    यदि( src.isContinuous() && आवरणम्.empty() )
    {
        size_t len = src.योग()*cn;
        यदि( len == (size_t)(पूर्णाङ्क)len )
        {
            यदि( गभीरता == CV_32F )
            {
                ध्रुव uchar* दत्त = src.ptr<ध्रुव uchar>();

                यदि( normType == NORM_L2 || normType == NORM_L2SQR || normType == NORM_L1 )
                {
                    द्विगुण परिणाम = 0;
                    func(दत्त, 0, (uchar*)&परिणाम, (पूर्णाङ्क)len, 1);
                    प्रत्यागम normType == NORM_L2 ? std::वर्गमूलम्(परिणाम) : परिणाम;
                }
                यदि( normType == NORM_INF )
                {
                    भिन्न परिणाम = 0;
                    func(दत्त, 0, (uchar*)&परिणाम, (पूर्णाङ्क)len, 1);
                    प्रत्यागम परिणाम;
                }
            }
            यदि( गभीरता == CV_8U )
            {
                ध्रुव uchar* दत्त = src.ptr<uchar>();

                यदि( normType == NORM_HAMMING )
                {
                    प्रत्यागम hal::normHamming(दत्त, (पूर्णाङ्क)len, 1);
                }

                यदि( normType == NORM_HAMMING2 )
                {
                    प्रत्यागम hal::normHamming(दत्त, (पूर्णाङ्क)len, 2);
                }
            }
        }
    }

    CV_Assert( आवरणम्.empty() || आवरणम्.type() == CV_8U || आवरणम्.type() == CV_8S || आवरणम्.type() == CV_Bool );

    यदि( normType == NORM_HAMMING || normType == NORM_HAMMING2 )
    {
        यदि( !आवरणम्.empty() )
        {
            Mat अस्थायी_2;
            bitwise_and(src, आवरणम्, अस्थायी_2);
            प्रत्यागम norm(अस्थायी_2, normType);
        }
        पूर्णाङ्क cell_आकार = normType == NORM_HAMMING ? 1 : 2;

        ध्रुव Mat* arrays[] = {&src, 0};
        uchar* ptrs[1] = {};
        NAryMatIterator it(arrays, ptrs);
        पूर्णाङ्क योग = (पूर्णाङ्क)it.size;
        पूर्णाङ्क परिणाम = 0;

        प्रति( size_t i = 0; i < it.nplanes; i++, ++it )
        {
            परिणाम += hal::normHamming(ptrs[0], योग, cell_आकार);
        }

        प्रत्यागम परिणाम;
    }

    ध्रुव Mat* arrays[] = {&src, &आवरणम्, 0};
    uchar* ptrs[2] = {};
    सङ्घ
    {
        द्विगुण d;
        अचिह्नित u;
        uint64 UL;
        भिन्न f;
    }
    परिणाम;
    परिणाम.d = 0;
    NAryMatIterator it(arrays, ptrs);
    CV_CheckLT((size_t)it.size, (size_t)अधिकतम_पूर्णाङ्कः, "");
    तार्किक is_fp16 = गभीरता == CV_16F || गभीरता == CV_16BF;

    यदि ((normType == NORM_L1 && (गभीरता <= CV_16S || गभीरता == CV_Bool || is_fp16)) ||
        ((normType == NORM_L2 || normType == NORM_L2SQR) && (गभीरता <= CV_8S || गभीरता == CV_Bool || is_fp16)))
    {
        // special case to handle "integer" overflow in accumulator
        ध्रुव size_t esz = src.elem_आकार();
        ध्रुव पूर्णाङ्क योग = (पूर्णाङ्क)it.size;
        ध्रुव पूर्णाङ्क खण्डम्_Size0 = (is_fp16 ? (1 << 10) :
            गभीरता == CV_Bool ? (1 << 30) :
            normType == NORM_L1 && गभीरता <= CV_8S ? (1 << 23) : (1 << 15))/cn;
        ध्रुव पूर्णाङ्क खण्डम्_आकार_2 = std::min(योग, खण्डम्_Size0);
        सङ्घ {
            पूर्णाङ्क i;
            भिन्न f;
        } blocksum;
        blocksum.i = 0;
        पूर्णाङ्क गणनम्_2 = 0;

        प्रति (size_t i = 0; i < it.nplanes; i++, ++it)
        {
            प्रति (पूर्णाङ्क j = 0; j < योग; j += खण्डम्_आकार_2)
            {
                पूर्णाङ्क bsz = std::min(योग - j, खण्डम्_आकार_2);
                func(ptrs[0], ptrs[1], (uchar*)&blocksum, bsz, cn);
                गणनम्_2 += bsz;
                यदि (गणनम्_2 + खण्डम्_आकार_2 >= खण्डम्_Size0 || (i+1 >= it.nplanes && j+bsz >= योग))
                {
                    परिणाम.d += is_fp16 ? (द्विगुण)blocksum.f : (द्विगुण)blocksum.i;
                    blocksum.i = 0;
                    गणनम्_2 = 0;
                }
                ptrs[0] += bsz*esz;
                यदि (ptrs[1])
                    ptrs[1] += bsz;
            }
        }
    }
    अन्यथा
    {
        // generic implementation
        प्रति (size_t i = 0; i < it.nplanes; i++, ++it)
        {
            func(ptrs[0], ptrs[1], (uchar*)&परिणाम, (पूर्णाङ्क)it.size, cn);
        }
    }

    यदि( normType == NORM_INF )
    {
        यदि(गभीरता <= CV_32S || गभीरता == CV_32U || गभीरता == CV_Bool)
            प्रत्यागम परिणाम.u;
        यदि (गभीरता == CV_32F || is_fp16)
            प्रत्यागम परिणाम.f;
        यदि (गभीरता == CV_64U || गभीरता == CV_64S)
            प्रत्यागम (द्विगुण)परिणाम.UL;
    }
    यदि( normType == NORM_L2 )
        प्रत्यागम std::वर्गमूलम्(परिणाम.d);

    प्रत्यागम परिणाम.d;
}

//==================================================================================================

#यदि_परिभाषितम् HAVE_OPENCL
स्थिर तार्किक ocl_norm( निवेशः_Array _src1, निवेशः_Array _src2, पूर्णाङ्क normType, निवेशः_Array आवरणम्_2, द्विगुण & परिणाम )
{
#यदि_परिभाषितम् __ANDROID__
    यदि (ocl::Device::getDefault().isNVidia())
        प्रत्यागम असत्य;
#यदि_अन्तः

    Scalar sc1, sc2;
    पूर्णाङ्क cn = _src1.channels();
    यदि (cn > 4)
        प्रत्यागम असत्य;
    पूर्णाङ्क type = _src1.type(), गभीरता = CV_MAT_गभीरता(type);
    तार्किक relative = (normType & NORM_RELATIVE) != 0;
    normType &= ~NORM_RELATIVE;
    तार्किक normsum = normType == NORM_L1 || normType == NORM_L2 || normType == NORM_L2SQR;

#यदि_परिभाषितम् __APPLE__
    यदि(normType == NORM_L1 && type == CV_16UC3 && !आवरणम्_2.empty())
        प्रत्यागम असत्य;
#यदि_अन्तः

    यदि (normsum)
    {
        यदि (!ocl_योग(_src1, sc1, normType == NORM_L2 || normType == NORM_L2SQR ?
                     OCL_OP_योग_SQR : OCL_OP_योग, आवरणम्_2, _src2, relative, sc2))
            प्रत्यागम असत्य;
    }
    अन्यथा
    {
        यदि (!ocl_minMaxIdx(_src1, रिक्त, &sc1[0], रिक्त, रिक्त, आवरणम्_2, std::max(CV_32S, गभीरता),
                           असत्य, _src2, relative ? &sc2[0] : रिक्त))
            प्रत्यागम असत्य;
        cn = 1;
    }

    द्विगुण s2 = 0;
    प्रति (पूर्णाङ्क i = 0; i < cn; ++i)
    {
        परिणाम += sc1[i];
        यदि (relative)
            s2 += sc2[i];
    }

    यदि (normType == NORM_L2)
    {
        परिणाम = std::वर्गमूलम्(परिणाम);
        यदि (relative)
            s2 = std::वर्गमूलम्(s2);
    }

    यदि (relative)
        परिणाम /= (s2 + DBL_EPSILON);

    प्रत्यागम सत्य;
}  // ocl_norm()
#यदि_अन्तः  // HAVE_OPENCL

द्विगुण norm( निवेशः_Array _src1, निवेशः_Array _src2, पूर्णाङ्क normType, निवेशः_Array आवरणम्_2 )
{
    CV_INSTRUMENT_REGION();

    CV_CheckTypeEQ(_src1.type(), _src2.type(), "Input type mismatch");
    CV_Assert(_src1.same_आकार(_src2));

#यदि defined HAVE_OPENCL
    द्विगुण परिणाम_2 = 0;
#यदि_अन्तः

#यदि_परिभाषितम् HAVE_OPENCL
    CV_OCL_सततम्_2(OCL_PERFORMANCE_CHECK(_src1.isUMat()),
                ocl_norm(_src1, _src2, normType, आवरणम्_2, परिणाम_2),
                परिणाम_2)
#यदि_अन्तः

    Mat src1 = _src1.getMat(), src2 = _src2.getMat(), आवरणम् = आवरणम्_2.getMat();
    CV_Assert( आवरणम्.empty() || ((आवरणम्.type() == CV_8U || आवरणम्.type() == CV_8S || आवरणम्.type() == CV_Bool) && src1.size == आवरणम्.size) );
    यदि( src1.empty() || src2.empty() )
        प्रत्यागम 0.0;
    पूर्णाङ्क गभीरता = src1.गभीरता(), cn = src1.channels();
    यदि( src1.dims <= 2 )
    {
        द्विगुण परिणाम;
        CALL_HAL_RET(normDiff, cv_hal_normDiff, परिणाम, src1.दत्त, src1.चरणः, src2.दत्त, src2.चरणः, आवरणम्.दत्त, आवरणम्.चरणः, src1.स्तम्भसङ्ख्या, src1.पङ्क्तिकाः, src1.type(), normType);
    }
    अन्यथा यदि( src1.isContinuous() && src2.isContinuous() && आवरणम्.isContinuous() )
    {
        द्विगुण परिणाम;
        CALL_HAL_RET(normDiff, cv_hal_normDiff, परिणाम, src1.दत्त, 0, src2.दत्त, 0, आवरणम्.दत्त, 0, (पूर्णाङ्क)src1.योग(), 1, src1.type(), normType);
    }

    यदि( normType & NORM_RELATIVE )
    {
        प्रत्यागम norm(_src1, _src2, normType & ~NORM_RELATIVE, आवरणम्_2)/(norm(_src2, normType, आवरणम्_2) + DBL_EPSILON);
    }

    normType &= 7;
    CV_Assert( normType == NORM_INF || normType == NORM_L1 ||
               normType == NORM_L2 || normType == NORM_L2SQR ||
              ((normType == NORM_HAMMING || normType == NORM_HAMMING2) && src1.type() == CV_8U) );

    NormDiffFunc func = getNormDiffFunc(normType >> 1, गभीरता);
    CV_Assert( (normType >> 1) >= 3 || func != 0 );

    यदि( src1.isContinuous() && src2.isContinuous() && आवरणम्.empty() )
    {
        size_t len = src1.योग()*src1.channels();
        यदि( len == (size_t)(पूर्णाङ्क)len )
        {
            यदि( src1.गभीरता() == CV_32F )
            {
                ध्रुव uchar* data1 = src1.ptr<ध्रुव uchar>();
                ध्रुव uchar* data2 = src2.ptr<ध्रुव uchar>();

                यदि( normType == NORM_L2 || normType == NORM_L2SQR || normType == NORM_L1 )
                {
                    द्विगुण परिणाम = 0;
                    func(data1, data2, 0, (uchar*)&परिणाम, (पूर्णाङ्क)len, 1);
                    प्रत्यागम normType == NORM_L2 ? std::वर्गमूलम्(परिणाम) : परिणाम;
                }
                यदि( normType == NORM_INF )
                {
                    भिन्न परिणाम = 0;
                    func(data1, data2, 0, (uchar*)&परिणाम, (पूर्णाङ्क)len, 1);
                    प्रत्यागम परिणाम;
                }
            }
        }
    }

    CV_Assert( आवरणम्.empty() || आवरणम्.type() == CV_8U || आवरणम्.type() == CV_8S || आवरणम्.type() == CV_Bool );

    यदि( normType == NORM_HAMMING || normType == NORM_HAMMING2 )
    {
        यदि( !आवरणम्.empty() )
        {
            Mat अस्थायी_2;
            bitwise_xor(src1, src2, अस्थायी_2);
            bitwise_and(अस्थायी_2, आवरणम्, अस्थायी_2);
            प्रत्यागम norm(अस्थायी_2, normType);
        }
        पूर्णाङ्क cell_आकार = normType == NORM_HAMMING ? 1 : 2;

        ध्रुव Mat* arrays[] = {&src1, &src2, 0};
        uchar* ptrs[2] = {};
        NAryMatIterator it(arrays, ptrs);
        पूर्णाङ्क योग = (पूर्णाङ्क)it.size;
        पूर्णाङ्क परिणाम = 0;

        प्रति( size_t i = 0; i < it.nplanes; i++, ++it )
        {
            परिणाम += hal::normHamming(ptrs[0], ptrs[1], योग, cell_आकार);
        }

        प्रत्यागम परिणाम;
    }

    ध्रुव Mat* arrays[] = {&src1, &src2, &आवरणम्, 0};
    uchar* ptrs[3] = {};
    सङ्घ
    {
        द्विगुण d;
        भिन्न f;
        अचिह्नित u;
        uint64 UL;
    }
    परिणाम;
    परिणाम.d = 0;
    NAryMatIterator it(arrays, ptrs);
    CV_CheckLT((size_t)it.size, (size_t)अधिकतम_पूर्णाङ्कः, "");

    तार्किक is_fp16 = गभीरता == CV_16F || गभीरता == CV_16BF;

    यदि ((normType == NORM_L1 && (गभीरता <= CV_16S || गभीरता == CV_Bool || is_fp16)) ||
        ((normType == NORM_L2 || normType == NORM_L2SQR) && (गभीरता <= CV_8S || गभीरता == CV_Bool || is_fp16)))
    {
        // special case to handle "integer" overflow in accumulator
        ध्रुव size_t esz = src1.elem_आकार();
        ध्रुव पूर्णाङ्क योग = (पूर्णाङ्क)it.size;
        ध्रुव पूर्णाङ्क खण्डम्_Size0 = (is_fp16 ? (1 << 10) :
            गभीरता == CV_Bool ? (1 << 30) :
            normType == NORM_L1 && गभीरता <= CV_8S ? (1 << 23) : (1 << 15))/cn;
        ध्रुव पूर्णाङ्क खण्डम्_आकार_2 = std::min(योग, खण्डम्_Size0);
        सङ्घ {
            पूर्णाङ्क i;
            भिन्न f;
        } blocksum;
        blocksum.i = 0;
        पूर्णाङ्क गणनम्_2 = 0;

        प्रति (size_t i = 0; i < it.nplanes; i++, ++it)
        {
            प्रति (पूर्णाङ्क j = 0; j < योग; j += खण्डम्_आकार_2)
            {
                पूर्णाङ्क bsz = std::min(योग - j, खण्डम्_आकार_2);
                func(ptrs[0], ptrs[1], ptrs[2], (uchar*)&blocksum, bsz, cn);
                गणनम्_2 += bsz;
                यदि (गणनम्_2 + खण्डम्_आकार_2 >= खण्डम्_Size0 || (i+1 >= it.nplanes && j+bsz >= योग))
                {
                    परिणाम.d += is_fp16 ? (द्विगुण)blocksum.f : (द्विगुण)blocksum.i;
                    blocksum.i = 0;
                    गणनम्_2 = 0;
                }
                ptrs[0] += bsz*esz;
                ptrs[1] += bsz*esz;
                यदि (ptrs[2])
                    ptrs[2] += bsz;
            }
        }
    }
    अन्यथा
    {
        // generic implementation
        प्रति (size_t i = 0; i < it.nplanes; i++, ++it)
        {
            func(ptrs[0], ptrs[1], ptrs[2], (uchar*)&परिणाम, (पूर्णाङ्क)it.size, cn);
        }
    }

    यदि( normType == NORM_INF )
    {
        यदि (गभीरता <= CV_32S || गभीरता == CV_32U || गभीरता == CV_Bool)
            प्रत्यागम परिणाम.u;
        यदि (गभीरता == CV_32F || is_fp16)
            प्रत्यागम परिणाम.f;
        यदि (गभीरता == CV_64U || गभीरता == CV_64S)
            प्रत्यागम (द्विगुण)परिणाम.UL;
    }
    यदि( normType == NORM_L2 )
        प्रत्यागम std::वर्गमूलम्(परिणाम.d);

    प्रत्यागम परिणाम.d;
}

cv::Hamming::परिणाम_Type Hamming::संक्रियकः()( ध्रुव अचिह्नित वर्ण* a, ध्रुव अचिह्नित वर्ण* b, पूर्णाङ्क size ) ध्रुव
{
    प्रत्यागम cv::hal::normHamming(a, b, size);
}

द्विगुण PSNR(निवेशः_Array _src1, निवेशः_Array _src2, द्विगुण R)
{
    CV_INSTRUMENT_REGION();

    //Input arrays must have depth CV_8U
    CV_Assert( _src1.type() == _src2.type() );

    द्विगुण diff = std::वर्गमूलम्(norm(_src1, _src2, NORM_L2SQR)/(_src1.योग()*_src1.channels()));
    प्रत्यागम 20*log10(R/(diff+DBL_EPSILON));
}


#यदि_परिभाषितम् HAVE_OPENCL
स्थिर तार्किक ocl_normalize( निवेशः_Array _src, निवेशः_Output_Array _dst, निवेशः_Array आवरणम्_2, पूर्णाङ्क dtype,
                           द्विगुण scale, द्विगुण delta )
{
    UMat src = _src.getUMat();

    यदि( आवरणम्_2.empty() )
        src.convert_गन्तव्यम्( _dst, dtype, scale, delta );
    अन्यथा यदि (src.channels() <= 4)
    {
        ध्रुव ocl::Device & dev = ocl::Device::getDefault();

        पूर्णाङ्क stype = _src.type(), sdepth = CV_MAT_गभीरता(stype), cn = CV_MAT_CN(stype),
                ddepth = CV_MAT_गभीरता(dtype), wdepth = std::max(CV_32F, std::max(sdepth, ddepth)),
                पङ्क्तिकाः_प्रत्येकम्_WI = dev.isIntel() ? 4 : 1;

        भिन्न fscale = static_cast<भिन्न>(scale), fdelta = static_cast<भिन्न>(delta);
        तार्किक haveScale = std::fabs(scale - 1) > DBL_EPSILON,
                haveZeroScale = !(std::fabs(scale) > DBL_EPSILON),
                haveDelta = std::fabs(delta) > DBL_EPSILON,
                doubleSupport = dev.doubleFPConfig() > 0;

        यदि (!haveScale && !haveDelta && stype == dtype)
        {
            _src.copy_गन्तव्यम्(_dst, आवरणम्_2);
            प्रत्यागम सत्य;
        }
        यदि (haveZeroScale)
        {
            _dst.समुच्चयः_गन्तव्यम्(Scalar(delta), आवरणम्_2);
            प्रत्यागम सत्य;
        }

        यदि ((sdepth == CV_64F || ddepth == CV_64F) && !doubleSupport)
            प्रत्यागम असत्य;

        वर्ण cvt[2][50];
        String opts = format("-D srcT=%s -D dstT=%s -D convertToWT=%s -D cn=%d -D rowsPerWI=%d"
                             " -D convertToDT=%s -D workT=%s%s%s%s -D srcT1=%s -D dstT1=%s",
                             ocl::type_गन्तव्यम्_Str(stype), ocl::type_गन्तव्यम्_Str(dtype),
                             ocl::convertTypeStr(sdepth, wdepth, cn, cvt[0], परिमाण(cvt[0])), cn,
                             पङ्क्तिकाः_प्रत्येकम्_WI, ocl::convertTypeStr(wdepth, ddepth, cn, cvt[1], परिमाण(cvt[1])),
                             ocl::type_गन्तव्यम्_Str(CV_MAKE_TYPE(wdepth, cn)),
                             doubleSupport ? " -D DOUBLE_SUPPORT" : "",
                             haveScale ? " -D HAVE_SCALE" : "",
                             haveDelta ? " -D HAVE_DELTA" : "",
                             ocl::type_गन्तव्यम्_Str(sdepth), ocl::type_गन्तव्यम्_Str(ddepth));

        ocl::Kernel k("normalizek", ocl::core::normalize_oclsrc, opts);
        यदि (k.empty())
            प्रत्यागम असत्य;

        UMat आवरणम् = आवरणम्_2.getUMat(), dst = _dst.getUMat();

        ocl::KernelArg srcarg = ocl::KernelArg::Read_Only_No_आकार(src),
                maskarg = ocl::KernelArg::Read_Only_No_आकार(आवरणम्),
                dstarg = ocl::KernelArg::ReadWrite(dst);

        यदि (haveScale)
        {
            यदि (haveDelta)
                k.तर्काः(srcarg, maskarg, dstarg, fscale, fdelta);
            अन्यथा
                k.तर्काः(srcarg, maskarg, dstarg, fscale);
        }
        अन्यथा
        {
            यदि (haveDelta)
                k.तर्काः(srcarg, maskarg, dstarg, fdelta);
            अन्यथा
                k.तर्काः(srcarg, maskarg, dstarg);
        }

        size_t globalsize[2] = { (size_t)src.स्तम्भसङ्ख्या, ((size_t)src.पङ्क्तिकाः + पङ्क्तिकाः_प्रत्येकम्_WI - 1) / पङ्क्तिकाः_प्रत्येकम्_WI };
        प्रत्यागम k.सततम्(2, globalsize, रिक्त, असत्य);
    }
    अन्यथा
    {
        UMat अस्थायी_2;
        src.convert_गन्तव्यम्( अस्थायी_2, dtype, scale, delta );
        अस्थायी_2.copy_गन्तव्यम्( _dst, आवरणम्_2 );
    }

    प्रत्यागम सत्य;
}  // ocl_normalize
#यदि_अन्तः  // HAVE_OPENCL

शून्य normalize(निवेशः_Array _src, निवेशः_Output_Array _dst, द्विगुण a, द्विगुण b,
               पूर्णाङ्क norm_type, पूर्णाङ्क rtype, निवेशः_Array आवरणम्_2)
{
    CV_INSTRUMENT_REGION();

    द्विगुण scale = 1, सरणम् = 0;
    पूर्णाङ्क type = _src.type(), गभीरता = CV_MAT_गभीरता(type);

    यदि( rtype < 0 )
        rtype = _dst.fixedType() ? _dst.गभीरता() : गभीरता;

    यदि( norm_type == NORM_MINMAX )
    {
        द्विगुण smin = 0, smax = 0;
        द्विगुण dmin = MIN( a, b ), dmax = MAX( a, b );
        minMaxIdx( _src, &smin, &smax, 0, 0, आवरणम्_2 );
        scale = (dmax - dmin)*(smax - smin > DBL_EPSILON ? 1./(smax - smin) : 0);
        यदि( rtype == CV_32F )
        {
            scale = (भिन्न)scale;
            सरणम् = (भिन्न)dmin - (भिन्न)(smin*scale);
        }
        अन्यथा
            सरणम् = dmin - smin*scale;
    }
    अन्यथा यदि( norm_type == NORM_L2 || norm_type == NORM_L1 || norm_type == NORM_INF )
    {
        scale = norm( _src, norm_type, आवरणम्_2 );
        scale = scale > DBL_EPSILON ? a/scale : 0.;
        सरणम् = 0;
    }
    अन्यथा
        CV_Error( cv::Error::StsBadArg, "Unknown/unsupported norm type" );

    CV_OCL_सततम्(_dst.isUMat(),
               ocl_normalize(_src, _dst, आवरणम्_2, rtype, scale, सरणम्))

    Mat src = _src.getMat();
    यदि( आवरणम्_2.empty() )
        src.convert_गन्तव्यम्( _dst, rtype, scale, सरणम् );
    अन्यथा
    {
        Mat अस्थायी_2;
        src.convert_गन्तव्यम्( अस्थायी_2, rtype, scale, सरणम् );
        अस्थायी_2.copy_गन्तव्यम्( _dst, आवरणम्_2 );
    }
}

}  // namespace
