// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html
//
// Copyright (C) 2026, Advanced Micro Devices, Inc., all rights reserved.


#అన్తర్భావ "precomp.hpp"
#అన్తర్భావ "opencl_kernels_core.hpp"
#అన్తర్భావ "stat.hpp"

#అన్తర్భావ "norm.simd.hpp"
#అన్తర్భావ "norm.simd_declarations.hpp"

/****************************************************************************************\
*                                         norm                                           *
\****************************************************************************************/

నామాకాశ cv { నామాకాశ hal {

బాహ్య స్థిరరాశి uchar తీయి_లెక్క_పట్టిక[256] =
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

స్థిర స్థిరరాశి uchar తీయి_లెక్క_Table2[] =
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

స్థిర స్థిరరాశి uchar తీయి_లెక్క_Table4[] =
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


పూర్ణాంకం normHamming(స్థిరరాశి uchar* a, పూర్ణాంకం n, పూర్ణాంకం cell_కొలత)
{
    పూర్ణాంకం output;
    CALL_HAL_RET(normHamming8u, cv_hal_normHamming8u, output, a, n, cell_కొలత);

    ఒకవేళ( cell_కొలత == 1 )
        తిరిగి normHamming(a, n);
    స్థిరరాశి uchar* tab = 0;
    ఒకవేళ( cell_కొలత == 2 )
        tab = తీయి_లెక్క_Table2;
    లేకపోతే ఒకవేళ( cell_కొలత == 4 )
        tab = తీయి_లెక్క_Table4;
    లేకపోతే
        తిరిగి -1;
    పూర్ణాంకం i = 0;
    పూర్ణాంకం ఫలితం = 0;
#ఒకవేళ (CV_SIMD || CV_SIMD_SCALABLE)
    v_uint64 t = vx_setzero_u64();
    ఒకవేళ ( cell_కొలత == 2)
    {
        v_uint16 ముసుగు = v_reinterpret_as_u16(vx_setall_u8(0x55));
        కొరకు(; i <= n - VTraits<v_uint8>::vlanes(); i += VTraits<v_uint8>::vlanes())
        {
            v_uint16 a0 = v_reinterpret_as_u16(vx_ఎక్కించు(a + i));
            t = v_కూడు(t, v_popcount(v_reinterpret_as_u64(v_and(v_or(a0, v_shr<1>(a0)), ముసుగు))));
        }
    }
    లేకపోతే    // cellSize == 4
    {
        v_uint16 ముసుగు = v_reinterpret_as_u16(vx_setall_u8(0x11));
        కొరకు(; i <= n - VTraits<v_uint8>::vlanes(); i += VTraits<v_uint8>::vlanes())
        {
            v_uint16 a0 = v_reinterpret_as_u16(vx_ఎక్కించు(a + i));
            v_uint16 a1 = v_or(a0, v_shr<2>(a0));
            t = v_కూడు(t, v_popcount(v_reinterpret_as_u64(v_and(v_or(a1, v_shr<1>(a1)), ముసుగు))));

        }
    }
    ఫలితం += (పూర్ణాంకం)v_reduce_కూడిక(t);
    vx_cleanup();
#లేదావేళ CV_ENABLE_UNROLLED
    కొరకు( ; i <= n - 4; i += 4 )
        ఫలితం += tab[a[i]] + tab[a[i+1]] + tab[a[i+2]] + tab[a[i+3]];
#అయితే_ముగింపు
    కొరకు( ; i < n; i++ )
        ఫలితం += tab[a[i]];
    తిరిగి ఫలితం;
}

పూర్ణాంకం normHamming(స్థిరరాశి uchar* a, స్థిరరాశి uchar* b, పూర్ణాంకం n, పూర్ణాంకం cell_కొలత)
{
    పూర్ణాంకం output;
    CALL_HAL_RET(normHammingDiff8u, cv_hal_normHammingDiff8u, output, a, b, n, cell_కొలత);

    ఒకవేళ( cell_కొలత == 1 )
        తిరిగి normHamming(a, b, n);
    స్థిరరాశి uchar* tab = 0;
    ఒకవేళ( cell_కొలత == 2 )
        tab = తీయి_లెక్క_Table2;
    లేకపోతే ఒకవేళ( cell_కొలత == 4 )
        tab = తీయి_లెక్క_Table4;
    లేకపోతే
        తిరిగి -1;
    పూర్ణాంకం i = 0;
    పూర్ణాంకం ఫలితం = 0;
#ఒకవేళ (CV_SIMD || CV_SIMD_SCALABLE)
    v_uint64 t = vx_setzero_u64();
    ఒకవేళ ( cell_కొలత == 2)
    {
        v_uint16 ముసుగు = v_reinterpret_as_u16(vx_setall_u8(0x55));
        కొరకు(; i <= n - VTraits<v_uint8>::vlanes(); i += VTraits<v_uint8>::vlanes())
        {
            v_uint16 ab0 = v_reinterpret_as_u16(v_xor(vx_ఎక్కించు(a + i), vx_ఎక్కించు(b + i)));
            t = v_కూడు(t, v_popcount(v_reinterpret_as_u64(v_and(v_or(ab0, v_shr<1>(ab0)), ముసుగు))));
        }
    }
    లేకపోతే    // cellSize == 4
    {
        v_uint16 ముసుగు = v_reinterpret_as_u16(vx_setall_u8(0x11));
        కొరకు(; i <= n - VTraits<v_uint8>::vlanes(); i += VTraits<v_uint8>::vlanes())
        {
            v_uint16 ab0 = v_reinterpret_as_u16(v_xor(vx_ఎక్కించు(a + i), vx_ఎక్కించు(b + i)));
            v_uint16 ab1 = v_or(ab0, v_shr<2>(ab0));
            t = v_కూడు(t, v_popcount(v_reinterpret_as_u64(v_and(v_or(ab1, v_shr<1>(ab1)), ముసుగు))));
        }
    }
    ఫలితం += (పూర్ణాంకం)v_reduce_కూడిక(t);
    vx_cleanup();
#లేదావేళ CV_ENABLE_UNROLLED
    కొరకు( ; i <= n - 4; i += 4 )
        ఫలితం += tab[a[i] ^ b[i]] + tab[a[i+1] ^ b[i+1]] +
                tab[a[i+2] ^ b[i+2]] + tab[a[i+3] ^ b[i+3]];
#అయితే_ముగింపు
    కొరకు( ; i < n; i++ )
        ఫలితం += tab[a[i] ^ b[i]];
    తిరిగి ఫలితం;
}

దశాంశం normL2Sqr_(స్థిరరాశి దశాంశం* a, స్థిరరాశి దశాంశం* b, పూర్ణాంకం n)
{
    పూర్ణాంకం j = 0; దశాంశం d = 0.f;
#ఒకవేళ (CV_SIMD || CV_SIMD_SCALABLE)
    స్థిరరాశి పూర్ణాంకం vl = VTraits<v_float32>::vlanes();
    v_float32 v_d0 = vx_setzero_f32(), v_d1 = vx_setzero_f32();
    v_float32 v_d2 = vx_setzero_f32(), v_d3 = vx_setzero_f32();
    కొరకు (; j <= n - 4 * vl; j += 4 * vl)
    {
        v_float32 t0 = v_sub(vx_ఎక్కించు(a + j), vx_ఎక్కించు(b + j));
        v_float32 t1 = v_sub(vx_ఎక్కించు(a + j + vl), vx_ఎక్కించు(b + j + vl));
        v_d0 = v_muladd(t0, t0, v_d0);
        v_float32 t2 = v_sub(vx_ఎక్కించు(a + j + 2 * vl), vx_ఎక్కించు(b + j + 2 * vl));
        v_d1 = v_muladd(t1, t1, v_d1);
        v_float32 t3 = v_sub(vx_ఎక్కించు(a + j + 3 * vl), vx_ఎక్కించు(b + j + 3 * vl));
        v_d2 = v_muladd(t2, t2, v_d2);
        v_d3 = v_muladd(t3, t3, v_d3);
    }
    v_d0 = v_కూడు(v_కూడు(v_d0, v_d1), v_కూడు(v_d2, v_d3));
    కొరకు (; j <= n - vl; j += vl)
    {
        v_float32 t0 = v_sub(vx_ఎక్కించు(a + j), vx_ఎక్కించు(b + j));
        v_d0 = v_muladd(t0, t0, v_d0);
    }
    d = v_reduce_కూడిక(v_d0);
#అయితే_ముగింపు
    కొరకు( ; j < n; j++ )
    {
        దశాంశం t = a[j] - b[j];
        d += t*t;
    }
    తిరిగి d;
}


దశాంశం normL1_(స్థిరరాశి దశాంశం* a, స్థిరరాశి దశాంశం* b, పూర్ణాంకం n)
{
    పూర్ణాంకం j = 0; దశాంశం d = 0.f;
#ఒకవేళ (CV_SIMD || CV_SIMD_SCALABLE)
    స్థిరరాశి పూర్ణాంకం vl = VTraits<v_float32>::vlanes();
    v_float32 v_d0 = vx_setzero_f32(), v_d1 = vx_setzero_f32();
    v_float32 v_d2 = vx_setzero_f32(), v_d3 = vx_setzero_f32();
    కొరకు (; j <= n - 4 * vl; j += 4 * vl)
    {
        v_d0 = v_కూడు(v_d0, v_absdiff(vx_ఎక్కించు(a + j), vx_ఎక్కించు(b + j)));
        v_d1 = v_కూడు(v_d1, v_absdiff(vx_ఎక్కించు(a + j + vl), vx_ఎక్కించు(b + j + vl)));
        v_d2 = v_కూడు(v_d2, v_absdiff(vx_ఎక్కించు(a + j + 2 * vl), vx_ఎక్కించు(b + j + 2 * vl)));
        v_d3 = v_కూడు(v_d3, v_absdiff(vx_ఎక్కించు(a + j + 3 * vl), vx_ఎక్కించు(b + j + 3 * vl)));
    }
    v_d0 = v_కూడు(v_కూడు(v_d0, v_d1), v_కూడు(v_d2, v_d3));
    కొరకు (; j <= n - vl; j += vl)
        v_d0 = v_కూడు(v_d0, v_absdiff(vx_ఎక్కించు(a + j), vx_ఎక్కించు(b + j)));
    d = v_reduce_కూడిక(v_d0);
#అయితే_ముగింపు
    కొరకు( ; j < n; j++ )
        d += std::నిరపేక్ష(a[j] - b[j]);
    తిరిగి d;
}

పూర్ణాంకం normL1_(స్థిరరాశి uchar* a, స్థిరరాశి uchar* b, పూర్ణాంకం n)
{
    పూర్ణాంకం j = 0, d = 0;
#ఒకవేళ (CV_SIMD || CV_SIMD_SCALABLE)
    స్థిరరాశి పూర్ణాంకం vl = VTraits<v_uint8>::vlanes();
    కొరకు (; j <= n - 4 * vl; j += 4 * vl)
        d += v_reduce_sad(vx_ఎక్కించు(a + j), vx_ఎక్కించు(b + j)) +
             v_reduce_sad(vx_ఎక్కించు(a + j + vl), vx_ఎక్కించు(b + j + vl)) +
             v_reduce_sad(vx_ఎక్కించు(a + j + 2 * vl), vx_ఎక్కించు(b + j + 2 * vl)) +
             v_reduce_sad(vx_ఎక్కించు(a + j + 3 * vl), vx_ఎక్కించు(b + j + 3 * vl));
    కొరకు (; j <= n - vl; j += vl)
        d += v_reduce_sad(vx_ఎక్కించు(a + j), vx_ఎక్కించు(b + j));
#అయితే_ముగింపు
    కొరకు( ; j < n; j++ )
        d += std::నిరపేక్ష(a[j] - b[j]);
    తిరిగి d;
}

} //cv::hal

//==================================================================================================

రకనిర్వచనం పూర్ణాంకం (*NormFunc)(స్థిరరాశి uchar*, స్థిరరాశి uchar*, uchar*, పూర్ణాంకం, పూర్ణాంకం);
రకనిర్వచనం పూర్ణాంకం (*NormDiffFunc)(స్థిరరాశి uchar*, స్థిరరాశి uchar*, స్థిరరాశి uchar*, uchar*, పూర్ణాంకం, పూర్ణాంకం);

#నిర్వచితమైతే HAVE_OPENCL

స్థిర తార్కిక ocl_norm( ఆదానం_Array _src, పూర్ణాంకం normType, ఆదానం_Array ముసుగు_2, ద్విగుణ & ఫలితం )
{
    స్థిరరాశి ocl::Device & d = ocl::Device::getDefault();

#నిర్వచితమైతే __ANDROID__
    ఒకవేళ (d.isNVidia())
        తిరిగి అబద్ధం;
#అయితే_ముగింపు
    స్థిరరాశి పూర్ణాంకం cn = _src.channels();
    ఒకవేళ (cn > 4)
        తిరిగి అబద్ధం;
    పూర్ణాంకం type = _src.type(), లోతు = CV_MAT_లోతు(type);
    తార్కిక doubleSupport = d.doubleFPConfig() > 0,
            have_ముసుగు = ముసుగు_2.రకం() != ఆదానం_Array_2::NONE;

    ఒకవేళ (లోతు >= CV_16F)
        తిరిగి అబద్ధం;  // TODO: support FP16

    ఒకవేళ ( !(normType == NORM_INF || normType == NORM_L1 || normType == NORM_L2 || normType == NORM_L2SQR) ||
         (!doubleSupport && లోతు == CV_64F))
        తిరిగి అబద్ధం;

    UMat src = _src.getUMat();

    ఒకవేళ (normType == NORM_INF)
    {
        ఒకవేళ (!ocl_minMaxIdx(_src, ఖాళీ, &ఫలితం, ఖాళీ, ఖాళీ, ముసుగు_2,
                           std::max(లోతు, CV_32S), లోతు != CV_8U && లోతు != CV_16U))
            తిరిగి అబద్ధం;
    }
    లేకపోతే ఒకవేళ (normType == NORM_L1 || normType == NORM_L2 || normType == NORM_L2SQR)
    {
        Scalar sc;
        తార్కిక unstype = లోతు == CV_8U || లోతు == CV_16U;

        ఒకవేళ ( !ocl_కూడిక(have_ముసుగు ? src : src.reshape(1), sc, normType == NORM_L2 || normType == NORM_L2SQR ?
                    OCL_OP_కూడిక_SQR : (unstype ? OCL_OP_కూడిక : OCL_OP_కూడిక_ABS), ముసుగు_2) )
            తిరిగి అబద్ధం;

        ద్విగుణ s = 0.0;
        కొరకు (పూర్ణాంకం i = 0; i < (have_ముసుగు ? cn : 1); ++i)
            s += sc[i];

        ఫలితం = normType == NORM_L1 || normType == NORM_L2SQR ? s : std::వర్గమూలం(s);
    }

    తిరిగి నిజం;
}

#అయితే_ముగింపు

స్థిర NormFunc getNormFunc(పూర్ణాంకం normType, పూర్ణాంకం లోతు) {
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getNormFunc, (normType, లోతు), CV_CPU_DISPATCH_MODES_ALL);
}
స్థిర NormDiffFunc getNormDiffFunc(పూర్ణాంకం normType, పూర్ణాంకం లోతు) {
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getNormDiffFunc, (normType, లోతు), CV_CPU_DISPATCH_MODES_ALL);
}

ద్విగుణ norm( ఆదానం_Array _src, పూర్ణాంకం normType, ఆదానం_Array ముసుగు_2 )
{
    CV_INSTRUMENT_REGION();

    normType &= NORM_TYPE_ముసుగు;
    CV_Assert( normType == NORM_INF || normType == NORM_L1 ||
               normType == NORM_L2 || normType == NORM_L2SQR ||
               ((normType == NORM_HAMMING || normType == NORM_HAMMING2) && _src.type() == CV_8U) );

#ఒకవేళ defined HAVE_OPENCL
    ద్విగుణ ఫలితం_2 = 0;
#అయితే_ముగింపు

#నిర్వచితమైతే HAVE_OPENCL
    CV_OCL_నిరంతరం_2(OCL_PERFORMANCE_CHECK(_src.isUMat()) && _src.dims() <= 2,
                ocl_norm(_src, normType, ముసుగు_2, ఫలితం_2),
                ఫలితం_2)
#అయితే_ముగింపు

    Mat src = _src.getMat(), ముసుగు = ముసుగు_2.getMat();
    CV_Assert( ముసుగు.empty() || ((ముసుగు.type() == CV_8U || ముసుగు.type() == CV_8S || ముసుగు.type() == CV_Bool) && src.size == ముసుగు.size) );
    ఒకవేళ( src.empty() )
        తిరిగి 0.0;
    పూర్ణాంకం లోతు = src.లోతు(), cn = src.channels();
    ఒకవేళ( src.dims <= 2 )
    {
        ద్విగుణ ఫలితం;
        CALL_HAL_RET(norm, cv_hal_norm, ఫలితం, src.దత్తాంశం, src.అడుగు, ముసుగు.దత్తాంశం, ముసుగు.అడుగు, src.నిలువుల_సంఖ్య, src.అడ్డువరుసలు, src.type(), normType);
    }
    లేకపోతే ఒకవేళ( src.isContinuous() && ముసుగు.isContinuous() )
    {
        ద్విగుణ ఫలితం;
        CALL_HAL_RET(norm, cv_hal_norm, ఫలితం, src.దత్తాంశం, 0, ముసుగు.దత్తాంశం, 0, (పూర్ణాంకం)src.మొత్తం(), 1, src.type(), normType);
    }

    NormFunc func = getNormFunc(normType >> 1, లోతు);
    CV_Assert( (normType >> 1) >= 3 || func != 0 );

    ఒకవేళ( src.isContinuous() && ముసుగు.empty() )
    {
        size_t len = src.మొత్తం()*cn;
        ఒకవేళ( len == (size_t)(పూర్ణాంకం)len )
        {
            ఒకవేళ( లోతు == CV_32F )
            {
                స్థిరరాశి uchar* దత్తాంశం = src.ptr<స్థిరరాశి uchar>();

                ఒకవేళ( normType == NORM_L2 || normType == NORM_L2SQR || normType == NORM_L1 )
                {
                    ద్విగుణ ఫలితం = 0;
                    func(దత్తాంశం, 0, (uchar*)&ఫలితం, (పూర్ణాంకం)len, 1);
                    తిరిగి normType == NORM_L2 ? std::వర్గమూలం(ఫలితం) : ఫలితం;
                }
                ఒకవేళ( normType == NORM_INF )
                {
                    దశాంశం ఫలితం = 0;
                    func(దత్తాంశం, 0, (uchar*)&ఫలితం, (పూర్ణాంకం)len, 1);
                    తిరిగి ఫలితం;
                }
            }
            ఒకవేళ( లోతు == CV_8U )
            {
                స్థిరరాశి uchar* దత్తాంశం = src.ptr<uchar>();

                ఒకవేళ( normType == NORM_HAMMING )
                {
                    తిరిగి hal::normHamming(దత్తాంశం, (పూర్ణాంకం)len, 1);
                }

                ఒకవేళ( normType == NORM_HAMMING2 )
                {
                    తిరిగి hal::normHamming(దత్తాంశం, (పూర్ణాంకం)len, 2);
                }
            }
        }
    }

    CV_Assert( ముసుగు.empty() || ముసుగు.type() == CV_8U || ముసుగు.type() == CV_8S || ముసుగు.type() == CV_Bool );

    ఒకవేళ( normType == NORM_HAMMING || normType == NORM_HAMMING2 )
    {
        ఒకవేళ( !ముసుగు.empty() )
        {
            Mat తాత్కాలిక_2;
            bitwise_and(src, ముసుగు, తాత్కాలిక_2);
            తిరిగి norm(తాత్కాలిక_2, normType);
        }
        పూర్ణాంకం cell_కొలత = normType == NORM_HAMMING ? 1 : 2;

        స్థిరరాశి Mat* arrays[] = {&src, 0};
        uchar* ptrs[1] = {};
        NAryMatIterator it(arrays, ptrs);
        పూర్ణాంకం మొత్తం = (పూర్ణాంకం)it.size;
        పూర్ణాంకం ఫలితం = 0;

        కొరకు( size_t i = 0; i < it.nplanes; i++, ++it )
        {
            ఫలితం += hal::normHamming(ptrs[0], మొత్తం, cell_కొలత);
        }

        తిరిగి ఫలితం;
    }

    స్థిరరాశి Mat* arrays[] = {&src, &ముసుగు, 0};
    uchar* ptrs[2] = {};
    సంఘం
    {
        ద్విగుణ d;
        గుర్తులేని u;
        uint64 UL;
        దశాంశం f;
    }
    ఫలితం;
    ఫలితం.d = 0;
    NAryMatIterator it(arrays, ptrs);
    CV_CheckLT((size_t)it.size, (size_t)గరిష్ఠ_పూర్ణాంకం, "");
    తార్కిక is_fp16 = లోతు == CV_16F || లోతు == CV_16BF;

    ఒకవేళ ((normType == NORM_L1 && (లోతు <= CV_16S || లోతు == CV_Bool || is_fp16)) ||
        ((normType == NORM_L2 || normType == NORM_L2SQR) && (లోతు <= CV_8S || లోతు == CV_Bool || is_fp16)))
    {
        // special case to handle "integer" overflow in accumulator
        స్థిరరాశి size_t esz = src.elem_కొలత();
        స్థిరరాశి పూర్ణాంకం మొత్తం = (పూర్ణాంకం)it.size;
        స్థిరరాశి పూర్ణాంకం ఖండం_Size0 = (is_fp16 ? (1 << 10) :
            లోతు == CV_Bool ? (1 << 30) :
            normType == NORM_L1 && లోతు <= CV_8S ? (1 << 23) : (1 << 15))/cn;
        స్థిరరాశి పూర్ణాంకం ఖండం_కొలత_2 = std::min(మొత్తం, ఖండం_Size0);
        సంఘం {
            పూర్ణాంకం i;
            దశాంశం f;
        } blocksum;
        blocksum.i = 0;
        పూర్ణాంకం లెక్క_2 = 0;

        కొరకు (size_t i = 0; i < it.nplanes; i++, ++it)
        {
            కొరకు (పూర్ణాంకం j = 0; j < మొత్తం; j += ఖండం_కొలత_2)
            {
                పూర్ణాంకం bsz = std::min(మొత్తం - j, ఖండం_కొలత_2);
                func(ptrs[0], ptrs[1], (uchar*)&blocksum, bsz, cn);
                లెక్క_2 += bsz;
                ఒకవేళ (లెక్క_2 + ఖండం_కొలత_2 >= ఖండం_Size0 || (i+1 >= it.nplanes && j+bsz >= మొత్తం))
                {
                    ఫలితం.d += is_fp16 ? (ద్విగుణ)blocksum.f : (ద్విగుణ)blocksum.i;
                    blocksum.i = 0;
                    లెక్క_2 = 0;
                }
                ptrs[0] += bsz*esz;
                ఒకవేళ (ptrs[1])
                    ptrs[1] += bsz;
            }
        }
    }
    లేకపోతే
    {
        // generic implementation
        కొరకు (size_t i = 0; i < it.nplanes; i++, ++it)
        {
            func(ptrs[0], ptrs[1], (uchar*)&ఫలితం, (పూర్ణాంకం)it.size, cn);
        }
    }

    ఒకవేళ( normType == NORM_INF )
    {
        ఒకవేళ(లోతు <= CV_32S || లోతు == CV_32U || లోతు == CV_Bool)
            తిరిగి ఫలితం.u;
        ఒకవేళ (లోతు == CV_32F || is_fp16)
            తిరిగి ఫలితం.f;
        ఒకవేళ (లోతు == CV_64U || లోతు == CV_64S)
            తిరిగి (ద్విగుణ)ఫలితం.UL;
    }
    ఒకవేళ( normType == NORM_L2 )
        తిరిగి std::వర్గమూలం(ఫలితం.d);

    తిరిగి ఫలితం.d;
}

//==================================================================================================

#నిర్వచితమైతే HAVE_OPENCL
స్థిర తార్కిక ocl_norm( ఆదానం_Array _src1, ఆదానం_Array _src2, పూర్ణాంకం normType, ఆదానం_Array ముసుగు_2, ద్విగుణ & ఫలితం )
{
#నిర్వచితమైతే __ANDROID__
    ఒకవేళ (ocl::Device::getDefault().isNVidia())
        తిరిగి అబద్ధం;
#అయితే_ముగింపు

    Scalar sc1, sc2;
    పూర్ణాంకం cn = _src1.channels();
    ఒకవేళ (cn > 4)
        తిరిగి అబద్ధం;
    పూర్ణాంకం type = _src1.type(), లోతు = CV_MAT_లోతు(type);
    తార్కిక relative = (normType & NORM_RELATIVE) != 0;
    normType &= ~NORM_RELATIVE;
    తార్కిక normsum = normType == NORM_L1 || normType == NORM_L2 || normType == NORM_L2SQR;

#నిర్వచితమైతే __APPLE__
    ఒకవేళ(normType == NORM_L1 && type == CV_16UC3 && !ముసుగు_2.empty())
        తిరిగి అబద్ధం;
#అయితే_ముగింపు

    ఒకవేళ (normsum)
    {
        ఒకవేళ (!ocl_కూడిక(_src1, sc1, normType == NORM_L2 || normType == NORM_L2SQR ?
                     OCL_OP_కూడిక_SQR : OCL_OP_కూడిక, ముసుగు_2, _src2, relative, sc2))
            తిరిగి అబద్ధం;
    }
    లేకపోతే
    {
        ఒకవేళ (!ocl_minMaxIdx(_src1, ఖాళీ, &sc1[0], ఖాళీ, ఖాళీ, ముసుగు_2, std::max(CV_32S, లోతు),
                           అబద్ధం, _src2, relative ? &sc2[0] : ఖాళీ))
            తిరిగి అబద్ధం;
        cn = 1;
    }

    ద్విగుణ s2 = 0;
    కొరకు (పూర్ణాంకం i = 0; i < cn; ++i)
    {
        ఫలితం += sc1[i];
        ఒకవేళ (relative)
            s2 += sc2[i];
    }

    ఒకవేళ (normType == NORM_L2)
    {
        ఫలితం = std::వర్గమూలం(ఫలితం);
        ఒకవేళ (relative)
            s2 = std::వర్గమూలం(s2);
    }

    ఒకవేళ (relative)
        ఫలితం /= (s2 + DBL_EPSILON);

    తిరిగి నిజం;
}  // ocl_norm()
#అయితే_ముగింపు  // HAVE_OPENCL

ద్విగుణ norm( ఆదానం_Array _src1, ఆదానం_Array _src2, పూర్ణాంకం normType, ఆదానం_Array ముసుగు_2 )
{
    CV_INSTRUMENT_REGION();

    CV_CheckTypeEQ(_src1.type(), _src2.type(), "Input type mismatch");
    CV_Assert(_src1.same_కొలత(_src2));

#ఒకవేళ defined HAVE_OPENCL
    ద్విగుణ ఫలితం_2 = 0;
#అయితే_ముగింపు

#నిర్వచితమైతే HAVE_OPENCL
    CV_OCL_నిరంతరం_2(OCL_PERFORMANCE_CHECK(_src1.isUMat()),
                ocl_norm(_src1, _src2, normType, ముసుగు_2, ఫలితం_2),
                ఫలితం_2)
#అయితే_ముగింపు

    Mat src1 = _src1.getMat(), src2 = _src2.getMat(), ముసుగు = ముసుగు_2.getMat();
    CV_Assert( ముసుగు.empty() || ((ముసుగు.type() == CV_8U || ముసుగు.type() == CV_8S || ముసుగు.type() == CV_Bool) && src1.size == ముసుగు.size) );
    ఒకవేళ( src1.empty() || src2.empty() )
        తిరిగి 0.0;
    పూర్ణాంకం లోతు = src1.లోతు(), cn = src1.channels();
    ఒకవేళ( src1.dims <= 2 )
    {
        ద్విగుణ ఫలితం;
        CALL_HAL_RET(normDiff, cv_hal_normDiff, ఫలితం, src1.దత్తాంశం, src1.అడుగు, src2.దత్తాంశం, src2.అడుగు, ముసుగు.దత్తాంశం, ముసుగు.అడుగు, src1.నిలువుల_సంఖ్య, src1.అడ్డువరుసలు, src1.type(), normType);
    }
    లేకపోతే ఒకవేళ( src1.isContinuous() && src2.isContinuous() && ముసుగు.isContinuous() )
    {
        ద్విగుణ ఫలితం;
        CALL_HAL_RET(normDiff, cv_hal_normDiff, ఫలితం, src1.దత్తాంశం, 0, src2.దత్తాంశం, 0, ముసుగు.దత్తాంశం, 0, (పూర్ణాంకం)src1.మొత్తం(), 1, src1.type(), normType);
    }

    ఒకవేళ( normType & NORM_RELATIVE )
    {
        తిరిగి norm(_src1, _src2, normType & ~NORM_RELATIVE, ముసుగు_2)/(norm(_src2, normType, ముసుగు_2) + DBL_EPSILON);
    }

    normType &= 7;
    CV_Assert( normType == NORM_INF || normType == NORM_L1 ||
               normType == NORM_L2 || normType == NORM_L2SQR ||
              ((normType == NORM_HAMMING || normType == NORM_HAMMING2) && src1.type() == CV_8U) );

    NormDiffFunc func = getNormDiffFunc(normType >> 1, లోతు);
    CV_Assert( (normType >> 1) >= 3 || func != 0 );

    ఒకవేళ( src1.isContinuous() && src2.isContinuous() && ముసుగు.empty() )
    {
        size_t len = src1.మొత్తం()*src1.channels();
        ఒకవేళ( len == (size_t)(పూర్ణాంకం)len )
        {
            ఒకవేళ( src1.లోతు() == CV_32F )
            {
                స్థిరరాశి uchar* data1 = src1.ptr<స్థిరరాశి uchar>();
                స్థిరరాశి uchar* data2 = src2.ptr<స్థిరరాశి uchar>();

                ఒకవేళ( normType == NORM_L2 || normType == NORM_L2SQR || normType == NORM_L1 )
                {
                    ద్విగుణ ఫలితం = 0;
                    func(data1, data2, 0, (uchar*)&ఫలితం, (పూర్ణాంకం)len, 1);
                    తిరిగి normType == NORM_L2 ? std::వర్గమూలం(ఫలితం) : ఫలితం;
                }
                ఒకవేళ( normType == NORM_INF )
                {
                    దశాంశం ఫలితం = 0;
                    func(data1, data2, 0, (uchar*)&ఫలితం, (పూర్ణాంకం)len, 1);
                    తిరిగి ఫలితం;
                }
            }
        }
    }

    CV_Assert( ముసుగు.empty() || ముసుగు.type() == CV_8U || ముసుగు.type() == CV_8S || ముసుగు.type() == CV_Bool );

    ఒకవేళ( normType == NORM_HAMMING || normType == NORM_HAMMING2 )
    {
        ఒకవేళ( !ముసుగు.empty() )
        {
            Mat తాత్కాలిక_2;
            bitwise_xor(src1, src2, తాత్కాలిక_2);
            bitwise_and(తాత్కాలిక_2, ముసుగు, తాత్కాలిక_2);
            తిరిగి norm(తాత్కాలిక_2, normType);
        }
        పూర్ణాంకం cell_కొలత = normType == NORM_HAMMING ? 1 : 2;

        స్థిరరాశి Mat* arrays[] = {&src1, &src2, 0};
        uchar* ptrs[2] = {};
        NAryMatIterator it(arrays, ptrs);
        పూర్ణాంకం మొత్తం = (పూర్ణాంకం)it.size;
        పూర్ణాంకం ఫలితం = 0;

        కొరకు( size_t i = 0; i < it.nplanes; i++, ++it )
        {
            ఫలితం += hal::normHamming(ptrs[0], ptrs[1], మొత్తం, cell_కొలత);
        }

        తిరిగి ఫలితం;
    }

    స్థిరరాశి Mat* arrays[] = {&src1, &src2, &ముసుగు, 0};
    uchar* ptrs[3] = {};
    సంఘం
    {
        ద్విగుణ d;
        దశాంశం f;
        గుర్తులేని u;
        uint64 UL;
    }
    ఫలితం;
    ఫలితం.d = 0;
    NAryMatIterator it(arrays, ptrs);
    CV_CheckLT((size_t)it.size, (size_t)గరిష్ఠ_పూర్ణాంకం, "");

    తార్కిక is_fp16 = లోతు == CV_16F || లోతు == CV_16BF;

    ఒకవేళ ((normType == NORM_L1 && (లోతు <= CV_16S || లోతు == CV_Bool || is_fp16)) ||
        ((normType == NORM_L2 || normType == NORM_L2SQR) && (లోతు <= CV_8S || లోతు == CV_Bool || is_fp16)))
    {
        // special case to handle "integer" overflow in accumulator
        స్థిరరాశి size_t esz = src1.elem_కొలత();
        స్థిరరాశి పూర్ణాంకం మొత్తం = (పూర్ణాంకం)it.size;
        స్థిరరాశి పూర్ణాంకం ఖండం_Size0 = (is_fp16 ? (1 << 10) :
            లోతు == CV_Bool ? (1 << 30) :
            normType == NORM_L1 && లోతు <= CV_8S ? (1 << 23) : (1 << 15))/cn;
        స్థిరరాశి పూర్ణాంకం ఖండం_కొలత_2 = std::min(మొత్తం, ఖండం_Size0);
        సంఘం {
            పూర్ణాంకం i;
            దశాంశం f;
        } blocksum;
        blocksum.i = 0;
        పూర్ణాంకం లెక్క_2 = 0;

        కొరకు (size_t i = 0; i < it.nplanes; i++, ++it)
        {
            కొరకు (పూర్ణాంకం j = 0; j < మొత్తం; j += ఖండం_కొలత_2)
            {
                పూర్ణాంకం bsz = std::min(మొత్తం - j, ఖండం_కొలత_2);
                func(ptrs[0], ptrs[1], ptrs[2], (uchar*)&blocksum, bsz, cn);
                లెక్క_2 += bsz;
                ఒకవేళ (లెక్క_2 + ఖండం_కొలత_2 >= ఖండం_Size0 || (i+1 >= it.nplanes && j+bsz >= మొత్తం))
                {
                    ఫలితం.d += is_fp16 ? (ద్విగుణ)blocksum.f : (ద్విగుణ)blocksum.i;
                    blocksum.i = 0;
                    లెక్క_2 = 0;
                }
                ptrs[0] += bsz*esz;
                ptrs[1] += bsz*esz;
                ఒకవేళ (ptrs[2])
                    ptrs[2] += bsz;
            }
        }
    }
    లేకపోతే
    {
        // generic implementation
        కొరకు (size_t i = 0; i < it.nplanes; i++, ++it)
        {
            func(ptrs[0], ptrs[1], ptrs[2], (uchar*)&ఫలితం, (పూర్ణాంకం)it.size, cn);
        }
    }

    ఒకవేళ( normType == NORM_INF )
    {
        ఒకవేళ (లోతు <= CV_32S || లోతు == CV_32U || లోతు == CV_Bool)
            తిరిగి ఫలితం.u;
        ఒకవేళ (లోతు == CV_32F || is_fp16)
            తిరిగి ఫలితం.f;
        ఒకవేళ (లోతు == CV_64U || లోతు == CV_64S)
            తిరిగి (ద్విగుణ)ఫలితం.UL;
    }
    ఒకవేళ( normType == NORM_L2 )
        తిరిగి std::వర్గమూలం(ఫలితం.d);

    తిరిగి ఫలితం.d;
}

cv::Hamming::ఫలితం_Type Hamming::క్రియాకారకం()( స్థిరరాశి గుర్తులేని అక్షరం* a, స్థిరరాశి గుర్తులేని అక్షరం* b, పూర్ణాంకం size ) స్థిరరాశి
{
    తిరిగి cv::hal::normHamming(a, b, size);
}

ద్విగుణ PSNR(ఆదానం_Array _src1, ఆదానం_Array _src2, ద్విగుణ R)
{
    CV_INSTRUMENT_REGION();

    //Input arrays must have depth CV_8U
    CV_Assert( _src1.type() == _src2.type() );

    ద్విగుణ diff = std::వర్గమూలం(norm(_src1, _src2, NORM_L2SQR)/(_src1.మొత్తం()*_src1.channels()));
    తిరిగి 20*log10(R/(diff+DBL_EPSILON));
}


#నిర్వచితమైతే HAVE_OPENCL
స్థిర తార్కిక ocl_normalize( ఆదానం_Array _src, ఆదానం_Output_Array _dst, ఆదానం_Array ముసుగు_2, పూర్ణాంకం dtype,
                           ద్విగుణ scale, ద్విగుణ delta )
{
    UMat src = _src.getUMat();

    ఒకవేళ( ముసుగు_2.empty() )
        src.convert_గమ్యం( _dst, dtype, scale, delta );
    లేకపోతే ఒకవేళ (src.channels() <= 4)
    {
        స్థిరరాశి ocl::Device & dev = ocl::Device::getDefault();

        పూర్ణాంకం stype = _src.type(), sdepth = CV_MAT_లోతు(stype), cn = CV_MAT_CN(stype),
                ddepth = CV_MAT_లోతు(dtype), wdepth = std::max(CV_32F, std::max(sdepth, ddepth)),
                అడ్డువరుసలు_చొప్పున_WI = dev.isIntel() ? 4 : 1;

        దశాంశం fscale = static_cast<దశాంశం>(scale), fdelta = static_cast<దశాంశం>(delta);
        తార్కిక haveScale = std::fabs(scale - 1) > DBL_EPSILON,
                haveZeroScale = !(std::fabs(scale) > DBL_EPSILON),
                haveDelta = std::fabs(delta) > DBL_EPSILON,
                doubleSupport = dev.doubleFPConfig() > 0;

        ఒకవేళ (!haveScale && !haveDelta && stype == dtype)
        {
            _src.copy_గమ్యం(_dst, ముసుగు_2);
            తిరిగి నిజం;
        }
        ఒకవేళ (haveZeroScale)
        {
            _dst.సమితి_గమ్యం(Scalar(delta), ముసుగు_2);
            తిరిగి నిజం;
        }

        ఒకవేళ ((sdepth == CV_64F || ddepth == CV_64F) && !doubleSupport)
            తిరిగి అబద్ధం;

        అక్షరం cvt[2][50];
        String opts = format("-D srcT=%s -D dstT=%s -D convertToWT=%s -D cn=%d -D rowsPerWI=%d"
                             " -D convertToDT=%s -D workT=%s%s%s%s -D srcT1=%s -D dstT1=%s",
                             ocl::type_గమ్యం_Str(stype), ocl::type_గమ్యం_Str(dtype),
                             ocl::convertTypeStr(sdepth, wdepth, cn, cvt[0], పరిమాణం(cvt[0])), cn,
                             అడ్డువరుసలు_చొప్పున_WI, ocl::convertTypeStr(wdepth, ddepth, cn, cvt[1], పరిమాణం(cvt[1])),
                             ocl::type_గమ్యం_Str(CV_MAKE_TYPE(wdepth, cn)),
                             doubleSupport ? " -D DOUBLE_SUPPORT" : "",
                             haveScale ? " -D HAVE_SCALE" : "",
                             haveDelta ? " -D HAVE_DELTA" : "",
                             ocl::type_గమ్యం_Str(sdepth), ocl::type_గమ్యం_Str(ddepth));

        ocl::Kernel k("normalizek", ocl::core::normalize_oclsrc, opts);
        ఒకవేళ (k.empty())
            తిరిగి అబద్ధం;

        UMat ముసుగు = ముసుగు_2.getUMat(), dst = _dst.getUMat();

        ocl::KernelArg srcarg = ocl::KernelArg::Read_Only_No_కొలత(src),
                maskarg = ocl::KernelArg::Read_Only_No_కొలత(ముసుగు),
                dstarg = ocl::KernelArg::ReadWrite(dst);

        ఒకవేళ (haveScale)
        {
            ఒకవేళ (haveDelta)
                k.వాదనలు(srcarg, maskarg, dstarg, fscale, fdelta);
            లేకపోతే
                k.వాదనలు(srcarg, maskarg, dstarg, fscale);
        }
        లేకపోతే
        {
            ఒకవేళ (haveDelta)
                k.వాదనలు(srcarg, maskarg, dstarg, fdelta);
            లేకపోతే
                k.వాదనలు(srcarg, maskarg, dstarg);
        }

        size_t globalsize[2] = { (size_t)src.నిలువుల_సంఖ్య, ((size_t)src.అడ్డువరుసలు + అడ్డువరుసలు_చొప్పున_WI - 1) / అడ్డువరుసలు_చొప్పున_WI };
        తిరిగి k.నిరంతరం(2, globalsize, ఖాళీ, అబద్ధం);
    }
    లేకపోతే
    {
        UMat తాత్కాలిక_2;
        src.convert_గమ్యం( తాత్కాలిక_2, dtype, scale, delta );
        తాత్కాలిక_2.copy_గమ్యం( _dst, ముసుగు_2 );
    }

    తిరిగి నిజం;
}  // ocl_normalize
#అయితే_ముగింపు  // HAVE_OPENCL

శూన్య normalize(ఆదానం_Array _src, ఆదానం_Output_Array _dst, ద్విగుణ a, ద్విగుణ b,
               పూర్ణాంకం norm_type, పూర్ణాంకం rtype, ఆదానం_Array ముసుగు_2)
{
    CV_INSTRUMENT_REGION();

    ద్విగుణ scale = 1, జరుపు = 0;
    పూర్ణాంకం type = _src.type(), లోతు = CV_MAT_లోతు(type);

    ఒకవేళ( rtype < 0 )
        rtype = _dst.fixedType() ? _dst.లోతు() : లోతు;

    ఒకవేళ( norm_type == NORM_MINMAX )
    {
        ద్విగుణ smin = 0, smax = 0;
        ద్విగుణ dmin = MIN( a, b ), dmax = MAX( a, b );
        minMaxIdx( _src, &smin, &smax, 0, 0, ముసుగు_2 );
        scale = (dmax - dmin)*(smax - smin > DBL_EPSILON ? 1./(smax - smin) : 0);
        ఒకవేళ( rtype == CV_32F )
        {
            scale = (దశాంశం)scale;
            జరుపు = (దశాంశం)dmin - (దశాంశం)(smin*scale);
        }
        లేకపోతే
            జరుపు = dmin - smin*scale;
    }
    లేకపోతే ఒకవేళ( norm_type == NORM_L2 || norm_type == NORM_L1 || norm_type == NORM_INF )
    {
        scale = norm( _src, norm_type, ముసుగు_2 );
        scale = scale > DBL_EPSILON ? a/scale : 0.;
        జరుపు = 0;
    }
    లేకపోతే
        CV_Error( cv::Error::StsBadArg, "Unknown/unsupported norm type" );

    CV_OCL_నిరంతరం(_dst.isUMat(),
               ocl_normalize(_src, _dst, ముసుగు_2, rtype, scale, జరుపు))

    Mat src = _src.getMat();
    ఒకవేళ( ముసుగు_2.empty() )
        src.convert_గమ్యం( _dst, rtype, scale, జరుపు );
    లేకపోతే
    {
        Mat తాత్కాలిక_2;
        src.convert_గమ్యం( తాత్కాలిక_2, rtype, scale, జరుపు );
        తాత్కాలిక_2.copy_గమ్యం( _dst, ముసుగు_2 );
    }
}

}  // namespace
