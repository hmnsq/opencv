// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html
//
// Copyright (C) 2026, Advanced Micro Devices, Inc., all rights reserved.


#ଅନ୍ତର୍ଭାଵ "precomp.hpp"
#ଅନ୍ତର୍ଭାଵ "opencl_kernels_core.hpp"
#ଅନ୍ତର୍ଭାଵ "stat.hpp"

#ଅନ୍ତର୍ଭାଵ "norm.simd.hpp"
#ଅନ୍ତର୍ଭାଵ "norm.simd_declarations.hpp"

/****************************************************************************************\
*                                         norm                                           *
\****************************************************************************************/

ନାମାକାଶ cv { ନାମାକାଶ hal {

ବାହ୍ୟ ଅଚଳ uchar କାଢ଼_ଗଣନା_Table[256] =
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

ସ୍ଥିର ଅଚଳ uchar କାଢ଼_ଗଣନା_Table2[] =
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

ସ୍ଥିର ଅଚଳ uchar କାଢ଼_ଗଣନା_Table4[] =
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


ପୂର୍ଣ୍ଣସଂଖ୍ୟା normHamming(ଅଚଳ uchar* a, ପୂର୍ଣ୍ଣସଂଖ୍ୟା n, ପୂର୍ଣ୍ଣସଂଖ୍ୟା cellSize)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା output;
    CALL_HAL_RET(normHamming8u, cv_hal_normHamming8u, output, a, n, cellSize);

    ଯଦି( cellSize == 1 )
        ଫେରନ୍ତୁ normHamming(a, n);
    ଅଚଳ uchar* tab = 0;
    ଯଦି( cellSize == 2 )
        tab = କାଢ଼_ଗଣନା_Table2;
    ଅନ୍ୟଥା ଯଦି( cellSize == 4 )
        tab = କାଢ଼_ଗଣନା_Table4;
    ଅନ୍ୟଥା
        ଫେରନ୍ତୁ -1;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା result = 0;
#ଯଦି (CV_SIMD || CV_SIMD_SCALABLE)
    v_uint64 t = vx_setzero_u64();
    ଯଦି ( cellSize == 2)
    {
        v_uint16 mask = v_reinterpret_as_u16(vx_setall_u8(0x55));
        ପାଇଁ(; i <= n - VTraits<v_uint8>::vlanes(); i += VTraits<v_uint8>::vlanes())
        {
            v_uint16 a0 = v_reinterpret_as_u16(vx_load(a + i));
            t = v_add(t, v_popcount(v_reinterpret_as_u64(v_and(v_or(a0, v_shr<1>(a0)), mask))));
        }
    }
    ଅନ୍ୟଥା    // cellSize == 4
    {
        v_uint16 mask = v_reinterpret_as_u16(vx_setall_u8(0x11));
        ପାଇଁ(; i <= n - VTraits<v_uint8>::vlanes(); i += VTraits<v_uint8>::vlanes())
        {
            v_uint16 a0 = v_reinterpret_as_u16(vx_load(a + i));
            v_uint16 a1 = v_or(a0, v_shr<2>(a0));
            t = v_add(t, v_popcount(v_reinterpret_as_u64(v_and(v_or(a1, v_shr<1>(a1)), mask))));

        }
    }
    result += (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)v_reduce_sum(t);
    vx_cleanup();
#ନଚେତଯଦି CV_ENABLE_UNROLLED
    ପାଇଁ( ; i <= n - 4; i += 4 )
        result += tab[a[i]] + tab[a[i+1]] + tab[a[i+2]] + tab[a[i+3]];
#endif
    ପାଇଁ( ; i < n; i++ )
        result += tab[a[i]];
    ଫେରନ୍ତୁ result;
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା normHamming(ଅଚଳ uchar* a, ଅଚଳ uchar* b, ପୂର୍ଣ୍ଣସଂଖ୍ୟା n, ପୂର୍ଣ୍ଣସଂଖ୍ୟା cellSize)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା output;
    CALL_HAL_RET(normHammingDiff8u, cv_hal_normHammingDiff8u, output, a, b, n, cellSize);

    ଯଦି( cellSize == 1 )
        ଫେରନ୍ତୁ normHamming(a, b, n);
    ଅଚଳ uchar* tab = 0;
    ଯଦି( cellSize == 2 )
        tab = କାଢ଼_ଗଣନା_Table2;
    ଅନ୍ୟଥା ଯଦି( cellSize == 4 )
        tab = କାଢ଼_ଗଣନା_Table4;
    ଅନ୍ୟଥା
        ଫେରନ୍ତୁ -1;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା result = 0;
#ଯଦି (CV_SIMD || CV_SIMD_SCALABLE)
    v_uint64 t = vx_setzero_u64();
    ଯଦି ( cellSize == 2)
    {
        v_uint16 mask = v_reinterpret_as_u16(vx_setall_u8(0x55));
        ପାଇଁ(; i <= n - VTraits<v_uint8>::vlanes(); i += VTraits<v_uint8>::vlanes())
        {
            v_uint16 ab0 = v_reinterpret_as_u16(v_xor(vx_load(a + i), vx_load(b + i)));
            t = v_add(t, v_popcount(v_reinterpret_as_u64(v_and(v_or(ab0, v_shr<1>(ab0)), mask))));
        }
    }
    ଅନ୍ୟଥା    // cellSize == 4
    {
        v_uint16 mask = v_reinterpret_as_u16(vx_setall_u8(0x11));
        ପାଇଁ(; i <= n - VTraits<v_uint8>::vlanes(); i += VTraits<v_uint8>::vlanes())
        {
            v_uint16 ab0 = v_reinterpret_as_u16(v_xor(vx_load(a + i), vx_load(b + i)));
            v_uint16 ab1 = v_or(ab0, v_shr<2>(ab0));
            t = v_add(t, v_popcount(v_reinterpret_as_u64(v_and(v_or(ab1, v_shr<1>(ab1)), mask))));
        }
    }
    result += (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)v_reduce_sum(t);
    vx_cleanup();
#ନଚେତଯଦି CV_ENABLE_UNROLLED
    ପାଇଁ( ; i <= n - 4; i += 4 )
        result += tab[a[i] ^ b[i]] + tab[a[i+1] ^ b[i+1]] +
                tab[a[i+2] ^ b[i+2]] + tab[a[i+3] ^ b[i+3]];
#endif
    ପାଇଁ( ; i < n; i++ )
        result += tab[a[i] ^ b[i]];
    ଫେରନ୍ତୁ result;
}

ଦଶମିକ normL2Sqr_(ଅଚଳ ଦଶମିକ* a, ଅଚଳ ଦଶମିକ* b, ପୂର୍ଣ୍ଣସଂଖ୍ୟା n)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା j = 0; ଦଶମିକ d = 0.f;
#ଯଦି (CV_SIMD || CV_SIMD_SCALABLE)
    ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା vl = VTraits<v_float32>::vlanes();
    v_float32 v_d0 = vx_setzero_f32(), v_d1 = vx_setzero_f32();
    v_float32 v_d2 = vx_setzero_f32(), v_d3 = vx_setzero_f32();
    ପାଇଁ (; j <= n - 4 * vl; j += 4 * vl)
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
    ପାଇଁ (; j <= n - vl; j += vl)
    {
        v_float32 t0 = v_sub(vx_load(a + j), vx_load(b + j));
        v_d0 = v_muladd(t0, t0, v_d0);
    }
    d = v_reduce_sum(v_d0);
#endif
    ପାଇଁ( ; j < n; j++ )
    {
        ଦଶମିକ t = a[j] - b[j];
        d += t*t;
    }
    ଫେରନ୍ତୁ d;
}


ଦଶମିକ normL1_(ଅଚଳ ଦଶମିକ* a, ଅଚଳ ଦଶମିକ* b, ପୂର୍ଣ୍ଣସଂଖ୍ୟା n)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା j = 0; ଦଶମିକ d = 0.f;
#ଯଦି (CV_SIMD || CV_SIMD_SCALABLE)
    ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା vl = VTraits<v_float32>::vlanes();
    v_float32 v_d0 = vx_setzero_f32(), v_d1 = vx_setzero_f32();
    v_float32 v_d2 = vx_setzero_f32(), v_d3 = vx_setzero_f32();
    ପାଇଁ (; j <= n - 4 * vl; j += 4 * vl)
    {
        v_d0 = v_add(v_d0, v_absdiff(vx_load(a + j), vx_load(b + j)));
        v_d1 = v_add(v_d1, v_absdiff(vx_load(a + j + vl), vx_load(b + j + vl)));
        v_d2 = v_add(v_d2, v_absdiff(vx_load(a + j + 2 * vl), vx_load(b + j + 2 * vl)));
        v_d3 = v_add(v_d3, v_absdiff(vx_load(a + j + 3 * vl), vx_load(b + j + 3 * vl)));
    }
    v_d0 = v_add(v_add(v_d0, v_d1), v_add(v_d2, v_d3));
    ପାଇଁ (; j <= n - vl; j += vl)
        v_d0 = v_add(v_d0, v_absdiff(vx_load(a + j), vx_load(b + j)));
    d = v_reduce_sum(v_d0);
#endif
    ପାଇଁ( ; j < n; j++ )
        d += std::abs(a[j] - b[j]);
    ଫେରନ୍ତୁ d;
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା normL1_(ଅଚଳ uchar* a, ଅଚଳ uchar* b, ପୂର୍ଣ୍ଣସଂଖ୍ୟା n)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା j = 0, d = 0;
#ଯଦି (CV_SIMD || CV_SIMD_SCALABLE)
    ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା vl = VTraits<v_uint8>::vlanes();
    ପାଇଁ (; j <= n - 4 * vl; j += 4 * vl)
        d += v_reduce_sad(vx_load(a + j), vx_load(b + j)) +
             v_reduce_sad(vx_load(a + j + vl), vx_load(b + j + vl)) +
             v_reduce_sad(vx_load(a + j + 2 * vl), vx_load(b + j + 2 * vl)) +
             v_reduce_sad(vx_load(a + j + 3 * vl), vx_load(b + j + 3 * vl));
    ପାଇଁ (; j <= n - vl; j += vl)
        d += v_reduce_sad(vx_load(a + j), vx_load(b + j));
#endif
    ପାଇଁ( ; j < n; j++ )
        d += std::abs(a[j] - b[j]);
    ଫେରନ୍ତୁ d;
}

} //cv::hal

//==================================================================================================

ପ୍ରକାରବ୍ୟାଖ୍ୟା ପୂର୍ଣ୍ଣସଂଖ୍ୟା (*NormFunc)(ଅଚଳ uchar*, ଅଚଳ uchar*, uchar*, ପୂର୍ଣ୍ଣସଂଖ୍ୟା, ପୂର୍ଣ୍ଣସଂଖ୍ୟା);
ପ୍ରକାରବ୍ୟାଖ୍ୟା ପୂର୍ଣ୍ଣସଂଖ୍ୟା (*NormDiffFunc)(ଅଚଳ uchar*, ଅଚଳ uchar*, ଅଚଳ uchar*, uchar*, ପୂର୍ଣ୍ଣସଂଖ୍ୟା, ପୂର୍ଣ୍ଣସଂଖ୍ୟା);

#ifdef HAVE_OPENCL

ସ୍ଥିର ତାର୍କିକ ocl_norm( ନିବେଶ_Array _src, ପୂର୍ଣ୍ଣସଂଖ୍ୟା normType, ନିବେଶ_Array _mask, ଦ୍ୱିଗୁଣ & result )
{
    ଅଚଳ ocl::Device & d = ocl::Device::getDefault();

#ifdef __ANDROID__
    ଯଦି (d.isNVidia())
        ଫେରନ୍ତୁ ମିଥ୍ୟା;
#endif
    ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା cn = _src.channels();
    ଯଦି (cn > 4)
        ଫେରନ୍ତୁ ମିଥ୍ୟା;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା type = _src.type(), depth = CV_MAT_DEPTH(type);
    ତାର୍କିକ doubleSupport = d.doubleFPConfig() > 0,
            haveMask = _mask.kind() != ନିବେଶ_Array_2::NONE;

    ଯଦି (depth >= CV_16F)
        ଫେରନ୍ତୁ ମିଥ୍ୟା;  // TODO: support FP16

    ଯଦି ( !(normType == NORM_INF || normType == NORM_L1 || normType == NORM_L2 || normType == NORM_L2SQR) ||
         (!doubleSupport && depth == CV_64F))
        ଫେରନ୍ତୁ ମିଥ୍ୟା;

    UMat src = _src.getUMat();

    ଯଦି (normType == NORM_INF)
    {
        ଯଦି (!ocl_minMaxIdx(_src, ଖାଲି, &result, ଖାଲି, ଖାଲି, _mask,
                           std::max(depth, CV_32S), depth != CV_8U && depth != CV_16U))
            ଫେରନ୍ତୁ ମିଥ୍ୟା;
    }
    ଅନ୍ୟଥା ଯଦି (normType == NORM_L1 || normType == NORM_L2 || normType == NORM_L2SQR)
    {
        Scalar sc;
        ତାର୍କିକ unstype = depth == CV_8U || depth == CV_16U;

        ଯଦି ( !ocl_sum(haveMask ? src : src.reshape(1), sc, normType == NORM_L2 || normType == NORM_L2SQR ?
                    OCL_OP_SUM_SQR : (unstype ? OCL_OP_SUM : OCL_OP_SUM_ABS), _mask) )
            ଫେରନ୍ତୁ ମିଥ୍ୟା;

        ଦ୍ୱିଗୁଣ s = 0.0;
        ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < (haveMask ? cn : 1); ++i)
            s += sc[i];

        result = normType == NORM_L1 || normType == NORM_L2SQR ? s : std::sqrt(s);
    }

    ଫେରନ୍ତୁ ସତ୍ୟ;
}

#endif

ସ୍ଥିର NormFunc getNormFunc(ପୂର୍ଣ୍ଣସଂଖ୍ୟା normType, ପୂର୍ଣ୍ଣସଂଖ୍ୟା depth) {
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getNormFunc, (normType, depth), CV_CPU_DISPATCH_MODES_ALL);
}
ସ୍ଥିର NormDiffFunc getNormDiffFunc(ପୂର୍ଣ୍ଣସଂଖ୍ୟା normType, ପୂର୍ଣ୍ଣସଂଖ୍ୟା depth) {
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getNormDiffFunc, (normType, depth), CV_CPU_DISPATCH_MODES_ALL);
}

ଦ୍ୱିଗୁଣ norm( ନିବେଶ_Array _src, ପୂର୍ଣ୍ଣସଂଖ୍ୟା normType, ନିବେଶ_Array _mask )
{
    CV_INSTRUMENT_REGION();

    normType &= NORM_TYPE_MASK;
    CV_Assert( normType == NORM_INF || normType == NORM_L1 ||
               normType == NORM_L2 || normType == NORM_L2SQR ||
               ((normType == NORM_HAMMING || normType == NORM_HAMMING2) && _src.type() == CV_8U) );

#ଯଦି defined HAVE_OPENCL
    ଦ୍ୱିଗୁଣ _result = 0;
#endif

#ifdef HAVE_OPENCL
    CV_OCL_RUN_(OCL_PERFORMANCE_CHECK(_src.isUMat()) && _src.dims() <= 2,
                ocl_norm(_src, normType, _mask, _result),
                _result)
#endif

    Mat src = _src.getMat(), mask = _mask.getMat();
    CV_Assert( mask.empty() || ((mask.type() == CV_8U || mask.type() == CV_8S || mask.type() == CV_Bool) && src.size == mask.size) );
    ଯଦି( src.empty() )
        ଫେରନ୍ତୁ 0.0;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା depth = src.depth(), cn = src.channels();
    ଯଦି( src.dims <= 2 )
    {
        ଦ୍ୱିଗୁଣ result;
        CALL_HAL_RET(norm, cv_hal_norm, result, src.data, src.step, mask.data, mask.step, src.cols, src.rows, src.type(), normType);
    }
    ଅନ୍ୟଥା ଯଦି( src.isContinuous() && mask.isContinuous() )
    {
        ଦ୍ୱିଗୁଣ result;
        CALL_HAL_RET(norm, cv_hal_norm, result, src.data, 0, mask.data, 0, (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)src.total(), 1, src.type(), normType);
    }

    NormFunc func = getNormFunc(normType >> 1, depth);
    CV_Assert( (normType >> 1) >= 3 || func != 0 );

    ଯଦି( src.isContinuous() && mask.empty() )
    {
        size_t len = src.total()*cn;
        ଯଦି( len == (size_t)(ପୂର୍ଣ୍ଣସଂଖ୍ୟା)len )
        {
            ଯଦି( depth == CV_32F )
            {
                ଅଚଳ uchar* data = src.ptr<ଅଚଳ uchar>();

                ଯଦି( normType == NORM_L2 || normType == NORM_L2SQR || normType == NORM_L1 )
                {
                    ଦ୍ୱିଗୁଣ result = 0;
                    func(data, 0, (uchar*)&result, (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)len, 1);
                    ଫେରନ୍ତୁ normType == NORM_L2 ? std::sqrt(result) : result;
                }
                ଯଦି( normType == NORM_INF )
                {
                    ଦଶମିକ result = 0;
                    func(data, 0, (uchar*)&result, (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)len, 1);
                    ଫେରନ୍ତୁ result;
                }
            }
            ଯଦି( depth == CV_8U )
            {
                ଅଚଳ uchar* data = src.ptr<uchar>();

                ଯଦି( normType == NORM_HAMMING )
                {
                    ଫେରନ୍ତୁ hal::normHamming(data, (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)len, 1);
                }

                ଯଦି( normType == NORM_HAMMING2 )
                {
                    ଫେରନ୍ତୁ hal::normHamming(data, (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)len, 2);
                }
            }
        }
    }

    CV_Assert( mask.empty() || mask.type() == CV_8U || mask.type() == CV_8S || mask.type() == CV_Bool );

    ଯଦି( normType == NORM_HAMMING || normType == NORM_HAMMING2 )
    {
        ଯଦି( !mask.empty() )
        {
            Mat temp;
            bitwise_and(src, mask, temp);
            ଫେରନ୍ତୁ norm(temp, normType);
        }
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା cellSize = normType == NORM_HAMMING ? 1 : 2;

        ଅଚଳ Mat* arrays[] = {&src, 0};
        uchar* ptrs[1] = {};
        NAryMatIterator it(arrays, ptrs);
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା total = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)it.size;
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା result = 0;

        ପାଇଁ( size_t i = 0; i < it.nplanes; i++, ++it )
        {
            result += hal::normHamming(ptrs[0], total, cellSize);
        }

        ଫେରନ୍ତୁ result;
    }

    ଅଚଳ Mat* arrays[] = {&src, &mask, 0};
    uchar* ptrs[2] = {};
    ସଂଘ
    {
        ଦ୍ୱିଗୁଣ d;
        unsigned u;
        uint64 UL;
        ଦଶମିକ f;
    }
    result;
    result.d = 0;
    NAryMatIterator it(arrays, ptrs);
    CV_CheckLT((size_t)it.size, (size_t)INT_MAX, "");
    ତାର୍କିକ is_fp16 = depth == CV_16F || depth == CV_16BF;

    ଯଦି ((normType == NORM_L1 && (depth <= CV_16S || depth == CV_Bool || is_fp16)) ||
        ((normType == NORM_L2 || normType == NORM_L2SQR) && (depth <= CV_8S || depth == CV_Bool || is_fp16)))
    {
        // special case to handle "integer" overflow in accumulator
        ଅଚଳ size_t esz = src.elemSize();
        ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା total = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)it.size;
        ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା blockSize0 = (is_fp16 ? (1 << 10) :
            depth == CV_Bool ? (1 << 30) :
            normType == NORM_L1 && depth <= CV_8S ? (1 << 23) : (1 << 15))/cn;
        ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା blockSize = std::min(total, blockSize0);
        ସଂଘ {
            ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;
            ଦଶମିକ f;
        } blocksum;
        blocksum.i = 0;
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଣନା_3 = 0;

        ପାଇଁ (size_t i = 0; i < it.nplanes; i++, ++it)
        {
            ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା j = 0; j < total; j += blockSize)
            {
                ପୂର୍ଣ୍ଣସଂଖ୍ୟା bsz = std::min(total - j, blockSize);
                func(ptrs[0], ptrs[1], (uchar*)&blocksum, bsz, cn);
                ଗଣନା_3 += bsz;
                ଯଦି (ଗଣନା_3 + blockSize >= blockSize0 || (i+1 >= it.nplanes && j+bsz >= total))
                {
                    result.d += is_fp16 ? (ଦ୍ୱିଗୁଣ)blocksum.f : (ଦ୍ୱିଗୁଣ)blocksum.i;
                    blocksum.i = 0;
                    ଗଣନା_3 = 0;
                }
                ptrs[0] += bsz*esz;
                ଯଦି (ptrs[1])
                    ptrs[1] += bsz;
            }
        }
    }
    ଅନ୍ୟଥା
    {
        // generic implementation
        ପାଇଁ (size_t i = 0; i < it.nplanes; i++, ++it)
        {
            func(ptrs[0], ptrs[1], (uchar*)&result, (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)it.size, cn);
        }
    }

    ଯଦି( normType == NORM_INF )
    {
        ଯଦି(depth <= CV_32S || depth == CV_32U || depth == CV_Bool)
            ଫେରନ୍ତୁ result.u;
        ଯଦି (depth == CV_32F || is_fp16)
            ଫେରନ୍ତୁ result.f;
        ଯଦି (depth == CV_64U || depth == CV_64S)
            ଫେରନ୍ତୁ (ଦ୍ୱିଗୁଣ)result.UL;
    }
    ଯଦି( normType == NORM_L2 )
        ଫେରନ୍ତୁ std::sqrt(result.d);

    ଫେରନ୍ତୁ result.d;
}

//==================================================================================================

#ifdef HAVE_OPENCL
ସ୍ଥିର ତାର୍କିକ ocl_norm( ନିବେଶ_Array _src1, ନିବେଶ_Array _src2, ପୂର୍ଣ୍ଣସଂଖ୍ୟା normType, ନିବେଶ_Array _mask, ଦ୍ୱିଗୁଣ & result )
{
#ifdef __ANDROID__
    ଯଦି (ocl::Device::getDefault().isNVidia())
        ଫେରନ୍ତୁ ମିଥ୍ୟା;
#endif

    Scalar sc1, sc2;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା cn = _src1.channels();
    ଯଦି (cn > 4)
        ଫେରନ୍ତୁ ମିଥ୍ୟା;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା type = _src1.type(), depth = CV_MAT_DEPTH(type);
    ତାର୍କିକ relative = (normType & NORM_RELATIVE) != 0;
    normType &= ~NORM_RELATIVE;
    ତାର୍କିକ normsum = normType == NORM_L1 || normType == NORM_L2 || normType == NORM_L2SQR;

#ifdef __APPLE__
    ଯଦି(normType == NORM_L1 && type == CV_16UC3 && !_mask.empty())
        ଫେରନ୍ତୁ ମିଥ୍ୟା;
#endif

    ଯଦି (normsum)
    {
        ଯଦି (!ocl_sum(_src1, sc1, normType == NORM_L2 || normType == NORM_L2SQR ?
                     OCL_OP_SUM_SQR : OCL_OP_SUM, _mask, _src2, relative, sc2))
            ଫେରନ୍ତୁ ମିଥ୍ୟା;
    }
    ଅନ୍ୟଥା
    {
        ଯଦି (!ocl_minMaxIdx(_src1, ଖାଲି, &sc1[0], ଖାଲି, ଖାଲି, _mask, std::max(CV_32S, depth),
                           ମିଥ୍ୟା, _src2, relative ? &sc2[0] : ଖାଲି))
            ଫେରନ୍ତୁ ମିଥ୍ୟା;
        cn = 1;
    }

    ଦ୍ୱିଗୁଣ s2 = 0;
    ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < cn; ++i)
    {
        result += sc1[i];
        ଯଦି (relative)
            s2 += sc2[i];
    }

    ଯଦି (normType == NORM_L2)
    {
        result = std::sqrt(result);
        ଯଦି (relative)
            s2 = std::sqrt(s2);
    }

    ଯଦି (relative)
        result /= (s2 + DBL_EPSILON);

    ଫେରନ୍ତୁ ସତ୍ୟ;
}  // ocl_norm()
#endif  // HAVE_OPENCL

ଦ୍ୱିଗୁଣ norm( ନିବେଶ_Array _src1, ନିବେଶ_Array _src2, ପୂର୍ଣ୍ଣସଂଖ୍ୟା normType, ନିବେଶ_Array _mask )
{
    CV_INSTRUMENT_REGION();

    CV_CheckTypeEQ(_src1.type(), _src2.type(), "Input type mismatch");
    CV_Assert(_src1.sameSize(_src2));

#ଯଦି defined HAVE_OPENCL
    ଦ୍ୱିଗୁଣ _result = 0;
#endif

#ifdef HAVE_OPENCL
    CV_OCL_RUN_(OCL_PERFORMANCE_CHECK(_src1.isUMat()),
                ocl_norm(_src1, _src2, normType, _mask, _result),
                _result)
#endif

    Mat src1 = _src1.getMat(), src2 = _src2.getMat(), mask = _mask.getMat();
    CV_Assert( mask.empty() || ((mask.type() == CV_8U || mask.type() == CV_8S || mask.type() == CV_Bool) && src1.size == mask.size) );
    ଯଦି( src1.empty() || src2.empty() )
        ଫେରନ୍ତୁ 0.0;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା depth = src1.depth(), cn = src1.channels();
    ଯଦି( src1.dims <= 2 )
    {
        ଦ୍ୱିଗୁଣ result;
        CALL_HAL_RET(normDiff, cv_hal_normDiff, result, src1.data, src1.step, src2.data, src2.step, mask.data, mask.step, src1.cols, src1.rows, src1.type(), normType);
    }
    ଅନ୍ୟଥା ଯଦି( src1.isContinuous() && src2.isContinuous() && mask.isContinuous() )
    {
        ଦ୍ୱିଗୁଣ result;
        CALL_HAL_RET(normDiff, cv_hal_normDiff, result, src1.data, 0, src2.data, 0, mask.data, 0, (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)src1.total(), 1, src1.type(), normType);
    }

    ଯଦି( normType & NORM_RELATIVE )
    {
        ଫେରନ୍ତୁ norm(_src1, _src2, normType & ~NORM_RELATIVE, _mask)/(norm(_src2, normType, _mask) + DBL_EPSILON);
    }

    normType &= 7;
    CV_Assert( normType == NORM_INF || normType == NORM_L1 ||
               normType == NORM_L2 || normType == NORM_L2SQR ||
              ((normType == NORM_HAMMING || normType == NORM_HAMMING2) && src1.type() == CV_8U) );

    NormDiffFunc func = getNormDiffFunc(normType >> 1, depth);
    CV_Assert( (normType >> 1) >= 3 || func != 0 );

    ଯଦି( src1.isContinuous() && src2.isContinuous() && mask.empty() )
    {
        size_t len = src1.total()*src1.channels();
        ଯଦି( len == (size_t)(ପୂର୍ଣ୍ଣସଂଖ୍ୟା)len )
        {
            ଯଦି( src1.depth() == CV_32F )
            {
                ଅଚଳ uchar* data1 = src1.ptr<ଅଚଳ uchar>();
                ଅଚଳ uchar* data2 = src2.ptr<ଅଚଳ uchar>();

                ଯଦି( normType == NORM_L2 || normType == NORM_L2SQR || normType == NORM_L1 )
                {
                    ଦ୍ୱିଗୁଣ result = 0;
                    func(data1, data2, 0, (uchar*)&result, (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)len, 1);
                    ଫେରନ୍ତୁ normType == NORM_L2 ? std::sqrt(result) : result;
                }
                ଯଦି( normType == NORM_INF )
                {
                    ଦଶମିକ result = 0;
                    func(data1, data2, 0, (uchar*)&result, (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)len, 1);
                    ଫେରନ୍ତୁ result;
                }
            }
        }
    }

    CV_Assert( mask.empty() || mask.type() == CV_8U || mask.type() == CV_8S || mask.type() == CV_Bool );

    ଯଦି( normType == NORM_HAMMING || normType == NORM_HAMMING2 )
    {
        ଯଦି( !mask.empty() )
        {
            Mat temp;
            bitwise_xor(src1, src2, temp);
            bitwise_and(temp, mask, temp);
            ଫେରନ୍ତୁ norm(temp, normType);
        }
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା cellSize = normType == NORM_HAMMING ? 1 : 2;

        ଅଚଳ Mat* arrays[] = {&src1, &src2, 0};
        uchar* ptrs[2] = {};
        NAryMatIterator it(arrays, ptrs);
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା total = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)it.size;
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା result = 0;

        ପାଇଁ( size_t i = 0; i < it.nplanes; i++, ++it )
        {
            result += hal::normHamming(ptrs[0], ptrs[1], total, cellSize);
        }

        ଫେରନ୍ତୁ result;
    }

    ଅଚଳ Mat* arrays[] = {&src1, &src2, &mask, 0};
    uchar* ptrs[3] = {};
    ସଂଘ
    {
        ଦ୍ୱିଗୁଣ d;
        ଦଶମିକ f;
        unsigned u;
        uint64 UL;
    }
    result;
    result.d = 0;
    NAryMatIterator it(arrays, ptrs);
    CV_CheckLT((size_t)it.size, (size_t)INT_MAX, "");

    ତାର୍କିକ is_fp16 = depth == CV_16F || depth == CV_16BF;

    ଯଦି ((normType == NORM_L1 && (depth <= CV_16S || depth == CV_Bool || is_fp16)) ||
        ((normType == NORM_L2 || normType == NORM_L2SQR) && (depth <= CV_8S || depth == CV_Bool || is_fp16)))
    {
        // special case to handle "integer" overflow in accumulator
        ଅଚଳ size_t esz = src1.elemSize();
        ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା total = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)it.size;
        ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା blockSize0 = (is_fp16 ? (1 << 10) :
            depth == CV_Bool ? (1 << 30) :
            normType == NORM_L1 && depth <= CV_8S ? (1 << 23) : (1 << 15))/cn;
        ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା blockSize = std::min(total, blockSize0);
        ସଂଘ {
            ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;
            ଦଶମିକ f;
        } blocksum;
        blocksum.i = 0;
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଣନା_3 = 0;

        ପାଇଁ (size_t i = 0; i < it.nplanes; i++, ++it)
        {
            ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା j = 0; j < total; j += blockSize)
            {
                ପୂର୍ଣ୍ଣସଂଖ୍ୟା bsz = std::min(total - j, blockSize);
                func(ptrs[0], ptrs[1], ptrs[2], (uchar*)&blocksum, bsz, cn);
                ଗଣନା_3 += bsz;
                ଯଦି (ଗଣନା_3 + blockSize >= blockSize0 || (i+1 >= it.nplanes && j+bsz >= total))
                {
                    result.d += is_fp16 ? (ଦ୍ୱିଗୁଣ)blocksum.f : (ଦ୍ୱିଗୁଣ)blocksum.i;
                    blocksum.i = 0;
                    ଗଣନା_3 = 0;
                }
                ptrs[0] += bsz*esz;
                ptrs[1] += bsz*esz;
                ଯଦି (ptrs[2])
                    ptrs[2] += bsz;
            }
        }
    }
    ଅନ୍ୟଥା
    {
        // generic implementation
        ପାଇଁ (size_t i = 0; i < it.nplanes; i++, ++it)
        {
            func(ptrs[0], ptrs[1], ptrs[2], (uchar*)&result, (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)it.size, cn);
        }
    }

    ଯଦି( normType == NORM_INF )
    {
        ଯଦି (depth <= CV_32S || depth == CV_32U || depth == CV_Bool)
            ଫେରନ୍ତୁ result.u;
        ଯଦି (depth == CV_32F || is_fp16)
            ଫେରନ୍ତୁ result.f;
        ଯଦି (depth == CV_64U || depth == CV_64S)
            ଫେରନ୍ତୁ (ଦ୍ୱିଗୁଣ)result.UL;
    }
    ଯଦି( normType == NORM_L2 )
        ଫେରନ୍ତୁ std::sqrt(result.d);

    ଫେରନ୍ତୁ result.d;
}

cv::Hamming::ResultType Hamming::operator()( ଅଚଳ unsigned ଅକ୍ଷର* a, ଅଚଳ unsigned ଅକ୍ଷର* b, ପୂର୍ଣ୍ଣସଂଖ୍ୟା size ) ଅଚଳ
{
    ଫେରନ୍ତୁ cv::hal::normHamming(a, b, size);
}

ଦ୍ୱିଗୁଣ PSNR(ନିବେଶ_Array _src1, ନିବେଶ_Array _src2, ଦ୍ୱିଗୁଣ R)
{
    CV_INSTRUMENT_REGION();

    //Input arrays must have depth CV_8U
    CV_Assert( _src1.type() == _src2.type() );

    ଦ୍ୱିଗୁଣ diff = std::sqrt(norm(_src1, _src2, NORM_L2SQR)/(_src1.total()*_src1.channels()));
    ଫେରନ୍ତୁ 20*log10(R/(diff+DBL_EPSILON));
}


#ifdef HAVE_OPENCL
ସ୍ଥିର ତାର୍କିକ ocl_normalize( ନିବେଶ_Array _src, ନିବେଶ_Output_Array _dst, ନିବେଶ_Array _mask, ପୂର୍ଣ୍ଣସଂଖ୍ୟା dtype,
                           ଦ୍ୱିଗୁଣ scale, ଦ୍ୱିଗୁଣ delta )
{
    UMat src = _src.getUMat();

    ଯଦି( _mask.empty() )
        src.convertTo( _dst, dtype, scale, delta );
    ଅନ୍ୟଥା ଯଦି (src.channels() <= 4)
    {
        ଅଚଳ ocl::Device & dev = ocl::Device::getDefault();

        ପୂର୍ଣ୍ଣସଂଖ୍ୟା stype = _src.type(), sdepth = CV_MAT_DEPTH(stype), cn = CV_MAT_CN(stype),
                ddepth = CV_MAT_DEPTH(dtype), wdepth = std::max(CV_32F, std::max(sdepth, ddepth)),
                rowsPerWI = dev.isIntel() ? 4 : 1;

        ଦଶମିକ fscale = static_cast<ଦଶମିକ>(scale), fdelta = static_cast<ଦଶମିକ>(delta);
        ତାର୍କିକ haveScale = std::fabs(scale - 1) > DBL_EPSILON,
                haveZeroScale = !(std::fabs(scale) > DBL_EPSILON),
                haveDelta = std::fabs(delta) > DBL_EPSILON,
                doubleSupport = dev.doubleFPConfig() > 0;

        ଯଦି (!haveScale && !haveDelta && stype == dtype)
        {
            _src.copyTo(_dst, _mask);
            ଫେରନ୍ତୁ ସତ୍ୟ;
        }
        ଯଦି (haveZeroScale)
        {
            _dst.setTo(Scalar(delta), _mask);
            ଫେରନ୍ତୁ ସତ୍ୟ;
        }

        ଯଦି ((sdepth == CV_64F || ddepth == CV_64F) && !doubleSupport)
            ଫେରନ୍ତୁ ମିଥ୍ୟା;

        ଅକ୍ଷର cvt[2][50];
        String opts = format("-D srcT=%s -D dstT=%s -D convertToWT=%s -D cn=%d -D rowsPerWI=%d"
                             " -D convertToDT=%s -D workT=%s%s%s%s -D srcT1=%s -D dstT1=%s",
                             ocl::typeToStr(stype), ocl::typeToStr(dtype),
                             ocl::convertTypeStr(sdepth, wdepth, cn, cvt[0], ଆକାର(cvt[0])), cn,
                             rowsPerWI, ocl::convertTypeStr(wdepth, ddepth, cn, cvt[1], ଆକାର(cvt[1])),
                             ocl::typeToStr(CV_MAKE_TYPE(wdepth, cn)),
                             doubleSupport ? " -D DOUBLE_SUPPORT" : "",
                             haveScale ? " -D HAVE_SCALE" : "",
                             haveDelta ? " -D HAVE_DELTA" : "",
                             ocl::typeToStr(sdepth), ocl::typeToStr(ddepth));

        ocl::Kernel k("normalizek", ocl::core::normalize_oclsrc, opts);
        ଯଦି (k.empty())
            ଫେରନ୍ତୁ ମିଥ୍ୟା;

        UMat mask = _mask.getUMat(), dst = _dst.getUMat();

        ocl::KernelArg srcarg = ocl::KernelArg::ReadOnlyNoSize(src),
                maskarg = ocl::KernelArg::ReadOnlyNoSize(mask),
                dstarg = ocl::KernelArg::ReadWrite(dst);

        ଯଦି (haveScale)
        {
            ଯଦି (haveDelta)
                k.args(srcarg, maskarg, dstarg, fscale, fdelta);
            ଅନ୍ୟଥା
                k.args(srcarg, maskarg, dstarg, fscale);
        }
        ଅନ୍ୟଥା
        {
            ଯଦି (haveDelta)
                k.args(srcarg, maskarg, dstarg, fdelta);
            ଅନ୍ୟଥା
                k.args(srcarg, maskarg, dstarg);
        }

        size_t globalsize[2] = { (size_t)src.cols, ((size_t)src.rows + rowsPerWI - 1) / rowsPerWI };
        ଫେରନ୍ତୁ k.run(2, globalsize, ଖାଲି, ମିଥ୍ୟା);
    }
    ଅନ୍ୟଥା
    {
        UMat temp;
        src.convertTo( temp, dtype, scale, delta );
        temp.copyTo( _dst, _mask );
    }

    ଫେରନ୍ତୁ ସତ୍ୟ;
}  // ocl_normalize
#endif  // HAVE_OPENCL

ଶୂନ୍ୟ normalize(ନିବେଶ_Array _src, ନିବେଶ_Output_Array _dst, ଦ୍ୱିଗୁଣ a, ଦ୍ୱିଗୁଣ b,
               ପୂର୍ଣ୍ଣସଂଖ୍ୟା norm_type, ପୂର୍ଣ୍ଣସଂଖ୍ୟା rtype, ନିବେଶ_Array _mask)
{
    CV_INSTRUMENT_REGION();

    ଦ୍ୱିଗୁଣ scale = 1, shift = 0;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା type = _src.type(), depth = CV_MAT_DEPTH(type);

    ଯଦି( rtype < 0 )
        rtype = _dst.fixedType() ? _dst.depth() : depth;

    ଯଦି( norm_type == NORM_MINMAX )
    {
        ଦ୍ୱିଗୁଣ smin = 0, smax = 0;
        ଦ୍ୱିଗୁଣ dmin = MIN( a, b ), dmax = MAX( a, b );
        minMaxIdx( _src, &smin, &smax, 0, 0, _mask );
        scale = (dmax - dmin)*(smax - smin > DBL_EPSILON ? 1./(smax - smin) : 0);
        ଯଦି( rtype == CV_32F )
        {
            scale = (ଦଶମିକ)scale;
            shift = (ଦଶମିକ)dmin - (ଦଶମିକ)(smin*scale);
        }
        ଅନ୍ୟଥା
            shift = dmin - smin*scale;
    }
    ଅନ୍ୟଥା ଯଦି( norm_type == NORM_L2 || norm_type == NORM_L1 || norm_type == NORM_INF )
    {
        scale = norm( _src, norm_type, _mask );
        scale = scale > DBL_EPSILON ? a/scale : 0.;
        shift = 0;
    }
    ଅନ୍ୟଥା
        CV_Error( cv::Error::StsBadArg, "Unknown/unsupported norm type" );

    CV_OCL_RUN(_dst.isUMat(),
               ocl_normalize(_src, _dst, _mask, rtype, scale, shift))

    Mat src = _src.getMat();
    ଯଦି( _mask.empty() )
        src.convertTo( _dst, rtype, scale, shift );
    ଅନ୍ୟଥା
    {
        Mat temp;
        src.convertTo( temp, rtype, scale, shift );
        temp.copyTo( _dst, _mask );
    }
}

}  // namespace
