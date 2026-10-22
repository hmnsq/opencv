// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html
//
// Copyright (C) 2026, Advanced Micro Devices, Inc., all rights reserved.


#અન્તર્ભાવ "precomp.hpp"
#અન્તર્ભાવ "opencl_kernels_core.hpp"
#અન્તર્ભાવ "stat.hpp"

#અન્તર્ભાવ "norm.simd.hpp"
#અન્તર્ભાવ "norm.simd_declarations.hpp"

/****************************************************************************************\
*                                         norm                                           *
\****************************************************************************************/

નામાકાશ cv { નામાકાશ hal {

બાહ્ય અચલ uchar કાઢો_ગણતરી_Table[256] =
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

સ્થિર અચલ uchar કાઢો_ગણતરી_Table2[] =
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

સ્થિર અચલ uchar કાઢો_ગણતરી_Table4[] =
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


પૂર્ણાંક normHamming(અચલ uchar* a, પૂર્ણાંક n, પૂર્ણાંક cellSize)
{
    પૂર્ણાંક output;
    CALL_HAL_RET(normHamming8u, cv_hal_normHamming8u, output, a, n, cellSize);

    જો( cellSize == 1 )
        પાછા normHamming(a, n);
    અચલ uchar* tab = 0;
    જો( cellSize == 2 )
        tab = કાઢો_ગણતરી_Table2;
    નહીંતર જો( cellSize == 4 )
        tab = કાઢો_ગણતરી_Table4;
    નહીંતર
        પાછા -1;
    પૂર્ણાંક i = 0;
    પૂર્ણાંક result = 0;
#જો (CV_SIMD || CV_SIMD_SCALABLE)
    v_uint64 t = vx_setzero_u64();
    જો ( cellSize == 2)
    {
        v_uint16 mask = v_reinterpret_as_u16(vx_setall_u8(0x55));
        માટે(; i <= n - VTraits<v_uint8>::vlanes(); i += VTraits<v_uint8>::vlanes())
        {
            v_uint16 a0 = v_reinterpret_as_u16(vx_load(a + i));
            t = v_add(t, v_popcount(v_reinterpret_as_u64(v_and(v_or(a0, v_shr<1>(a0)), mask))));
        }
    }
    નહીંતર    // cellSize == 4
    {
        v_uint16 mask = v_reinterpret_as_u16(vx_setall_u8(0x11));
        માટે(; i <= n - VTraits<v_uint8>::vlanes(); i += VTraits<v_uint8>::vlanes())
        {
            v_uint16 a0 = v_reinterpret_as_u16(vx_load(a + i));
            v_uint16 a1 = v_or(a0, v_shr<2>(a0));
            t = v_add(t, v_popcount(v_reinterpret_as_u64(v_and(v_or(a1, v_shr<1>(a1)), mask))));

        }
    }
    result += (પૂર્ણાંક)v_reduce_sum(t);
    vx_cleanup();
#અથવાજો CV_ENABLE_UNROLLED
    માટે( ; i <= n - 4; i += 4 )
        result += tab[a[i]] + tab[a[i+1]] + tab[a[i+2]] + tab[a[i+3]];
#endif
    માટે( ; i < n; i++ )
        result += tab[a[i]];
    પાછા result;
}

પૂર્ણાંક normHamming(અચલ uchar* a, અચલ uchar* b, પૂર્ણાંક n, પૂર્ણાંક cellSize)
{
    પૂર્ણાંક output;
    CALL_HAL_RET(normHammingDiff8u, cv_hal_normHammingDiff8u, output, a, b, n, cellSize);

    જો( cellSize == 1 )
        પાછા normHamming(a, b, n);
    અચલ uchar* tab = 0;
    જો( cellSize == 2 )
        tab = કાઢો_ગણતરી_Table2;
    નહીંતર જો( cellSize == 4 )
        tab = કાઢો_ગણતરી_Table4;
    નહીંતર
        પાછા -1;
    પૂર્ણાંક i = 0;
    પૂર્ણાંક result = 0;
#જો (CV_SIMD || CV_SIMD_SCALABLE)
    v_uint64 t = vx_setzero_u64();
    જો ( cellSize == 2)
    {
        v_uint16 mask = v_reinterpret_as_u16(vx_setall_u8(0x55));
        માટે(; i <= n - VTraits<v_uint8>::vlanes(); i += VTraits<v_uint8>::vlanes())
        {
            v_uint16 ab0 = v_reinterpret_as_u16(v_xor(vx_load(a + i), vx_load(b + i)));
            t = v_add(t, v_popcount(v_reinterpret_as_u64(v_and(v_or(ab0, v_shr<1>(ab0)), mask))));
        }
    }
    નહીંતર    // cellSize == 4
    {
        v_uint16 mask = v_reinterpret_as_u16(vx_setall_u8(0x11));
        માટે(; i <= n - VTraits<v_uint8>::vlanes(); i += VTraits<v_uint8>::vlanes())
        {
            v_uint16 ab0 = v_reinterpret_as_u16(v_xor(vx_load(a + i), vx_load(b + i)));
            v_uint16 ab1 = v_or(ab0, v_shr<2>(ab0));
            t = v_add(t, v_popcount(v_reinterpret_as_u64(v_and(v_or(ab1, v_shr<1>(ab1)), mask))));
        }
    }
    result += (પૂર્ણાંક)v_reduce_sum(t);
    vx_cleanup();
#અથવાજો CV_ENABLE_UNROLLED
    માટે( ; i <= n - 4; i += 4 )
        result += tab[a[i] ^ b[i]] + tab[a[i+1] ^ b[i+1]] +
                tab[a[i+2] ^ b[i+2]] + tab[a[i+3] ^ b[i+3]];
#endif
    માટે( ; i < n; i++ )
        result += tab[a[i] ^ b[i]];
    પાછા result;
}

દશાંશ normL2Sqr_(અચલ દશાંશ* a, અચલ દશાંશ* b, પૂર્ણાંક n)
{
    પૂર્ણાંક j = 0; દશાંશ d = 0.f;
#જો (CV_SIMD || CV_SIMD_SCALABLE)
    અચલ પૂર્ણાંક vl = VTraits<v_float32>::vlanes();
    v_float32 v_d0 = vx_setzero_f32(), v_d1 = vx_setzero_f32();
    v_float32 v_d2 = vx_setzero_f32(), v_d3 = vx_setzero_f32();
    માટે (; j <= n - 4 * vl; j += 4 * vl)
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
    માટે (; j <= n - vl; j += vl)
    {
        v_float32 t0 = v_sub(vx_load(a + j), vx_load(b + j));
        v_d0 = v_muladd(t0, t0, v_d0);
    }
    d = v_reduce_sum(v_d0);
#endif
    માટે( ; j < n; j++ )
    {
        દશાંશ t = a[j] - b[j];
        d += t*t;
    }
    પાછા d;
}


દશાંશ normL1_(અચલ દશાંશ* a, અચલ દશાંશ* b, પૂર્ણાંક n)
{
    પૂર્ણાંક j = 0; દશાંશ d = 0.f;
#જો (CV_SIMD || CV_SIMD_SCALABLE)
    અચલ પૂર્ણાંક vl = VTraits<v_float32>::vlanes();
    v_float32 v_d0 = vx_setzero_f32(), v_d1 = vx_setzero_f32();
    v_float32 v_d2 = vx_setzero_f32(), v_d3 = vx_setzero_f32();
    માટે (; j <= n - 4 * vl; j += 4 * vl)
    {
        v_d0 = v_add(v_d0, v_absdiff(vx_load(a + j), vx_load(b + j)));
        v_d1 = v_add(v_d1, v_absdiff(vx_load(a + j + vl), vx_load(b + j + vl)));
        v_d2 = v_add(v_d2, v_absdiff(vx_load(a + j + 2 * vl), vx_load(b + j + 2 * vl)));
        v_d3 = v_add(v_d3, v_absdiff(vx_load(a + j + 3 * vl), vx_load(b + j + 3 * vl)));
    }
    v_d0 = v_add(v_add(v_d0, v_d1), v_add(v_d2, v_d3));
    માટે (; j <= n - vl; j += vl)
        v_d0 = v_add(v_d0, v_absdiff(vx_load(a + j), vx_load(b + j)));
    d = v_reduce_sum(v_d0);
#endif
    માટે( ; j < n; j++ )
        d += std::abs(a[j] - b[j]);
    પાછા d;
}

પૂર્ણાંક normL1_(અચલ uchar* a, અચલ uchar* b, પૂર્ણાંક n)
{
    પૂર્ણાંક j = 0, d = 0;
#જો (CV_SIMD || CV_SIMD_SCALABLE)
    અચલ પૂર્ણાંક vl = VTraits<v_uint8>::vlanes();
    માટે (; j <= n - 4 * vl; j += 4 * vl)
        d += v_reduce_sad(vx_load(a + j), vx_load(b + j)) +
             v_reduce_sad(vx_load(a + j + vl), vx_load(b + j + vl)) +
             v_reduce_sad(vx_load(a + j + 2 * vl), vx_load(b + j + 2 * vl)) +
             v_reduce_sad(vx_load(a + j + 3 * vl), vx_load(b + j + 3 * vl));
    માટે (; j <= n - vl; j += vl)
        d += v_reduce_sad(vx_load(a + j), vx_load(b + j));
#endif
    માટે( ; j < n; j++ )
        d += std::abs(a[j] - b[j]);
    પાછા d;
}

} //cv::hal

//==================================================================================================

પ્રકારવ્યાખ્યા પૂર્ણાંક (*NormFunc)(અચલ uchar*, અચલ uchar*, uchar*, પૂર્ણાંક, પૂર્ણાંક);
પ્રકારવ્યાખ્યા પૂર્ણાંક (*NormDiffFunc)(અચલ uchar*, અચલ uchar*, અચલ uchar*, uchar*, પૂર્ણાંક, પૂર્ણાંક);

#ifdef HAVE_OPENCL

સ્થિર તાર્કિક ocl_norm( આગત_Array _src, પૂર્ણાંક normType, આગત_Array _mask, દ્વિગુણ & result )
{
    અચલ ocl::Device & d = ocl::Device::getDefault();

#ifdef __ANDROID__
    જો (d.isNVidia())
        પાછા ખોટું;
#endif
    અચલ પૂર્ણાંક cn = _src.channels();
    જો (cn > 4)
        પાછા ખોટું;
    પૂર્ણાંક type = _src.type(), depth = CV_MAT_DEPTH(type);
    તાર્કિક doubleSupport = d.doubleFPConfig() > 0,
            haveMask = _mask.kind() != આગત_Array_2::NONE;

    જો (depth >= CV_16F)
        પાછા ખોટું;  // TODO: support FP16

    જો ( !(normType == NORM_INF || normType == NORM_L1 || normType == NORM_L2 || normType == NORM_L2SQR) ||
         (!doubleSupport && depth == CV_64F))
        પાછા ખોટું;

    UMat src = _src.getUMat();

    જો (normType == NORM_INF)
    {
        જો (!ocl_minMaxIdx(_src, ખાલી, &result, ખાલી, ખાલી, _mask,
                           std::max(depth, CV_32S), depth != CV_8U && depth != CV_16U))
            પાછા ખોટું;
    }
    નહીંતર જો (normType == NORM_L1 || normType == NORM_L2 || normType == NORM_L2SQR)
    {
        Scalar sc;
        તાર્કિક unstype = depth == CV_8U || depth == CV_16U;

        જો ( !ocl_sum(haveMask ? src : src.reshape(1), sc, normType == NORM_L2 || normType == NORM_L2SQR ?
                    OCL_OP_SUM_SQR : (unstype ? OCL_OP_SUM : OCL_OP_SUM_ABS), _mask) )
            પાછા ખોટું;

        દ્વિગુણ s = 0.0;
        માટે (પૂર્ણાંક i = 0; i < (haveMask ? cn : 1); ++i)
            s += sc[i];

        result = normType == NORM_L1 || normType == NORM_L2SQR ? s : std::sqrt(s);
    }

    પાછા સાચું;
}

#endif

સ્થિર NormFunc getNormFunc(પૂર્ણાંક normType, પૂર્ણાંક depth) {
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getNormFunc, (normType, depth), CV_CPU_DISPATCH_MODES_ALL);
}
સ્થિર NormDiffFunc getNormDiffFunc(પૂર્ણાંક normType, પૂર્ણાંક depth) {
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getNormDiffFunc, (normType, depth), CV_CPU_DISPATCH_MODES_ALL);
}

દ્વિગુણ norm( આગત_Array _src, પૂર્ણાંક normType, આગત_Array _mask )
{
    CV_INSTRUMENT_REGION();

    normType &= NORM_TYPE_MASK;
    CV_Assert( normType == NORM_INF || normType == NORM_L1 ||
               normType == NORM_L2 || normType == NORM_L2SQR ||
               ((normType == NORM_HAMMING || normType == NORM_HAMMING2) && _src.type() == CV_8U) );

#જો defined HAVE_OPENCL
    દ્વિગુણ _result = 0;
#endif

#ifdef HAVE_OPENCL
    CV_OCL_RUN_(OCL_PERFORMANCE_CHECK(_src.isUMat()) && _src.dims() <= 2,
                ocl_norm(_src, normType, _mask, _result),
                _result)
#endif

    Mat src = _src.getMat(), mask = _mask.getMat();
    CV_Assert( mask.empty() || ((mask.type() == CV_8U || mask.type() == CV_8S || mask.type() == CV_Bool) && src.size == mask.size) );
    જો( src.empty() )
        પાછા 0.0;
    પૂર્ણાંક depth = src.depth(), cn = src.channels();
    જો( src.dims <= 2 )
    {
        દ્વિગુણ result;
        CALL_HAL_RET(norm, cv_hal_norm, result, src.data, src.step, mask.data, mask.step, src.cols, src.rows, src.type(), normType);
    }
    નહીંતર જો( src.isContinuous() && mask.isContinuous() )
    {
        દ્વિગુણ result;
        CALL_HAL_RET(norm, cv_hal_norm, result, src.data, 0, mask.data, 0, (પૂર્ણાંક)src.total(), 1, src.type(), normType);
    }

    NormFunc func = getNormFunc(normType >> 1, depth);
    CV_Assert( (normType >> 1) >= 3 || func != 0 );

    જો( src.isContinuous() && mask.empty() )
    {
        size_t len = src.total()*cn;
        જો( len == (size_t)(પૂર્ણાંક)len )
        {
            જો( depth == CV_32F )
            {
                અચલ uchar* data = src.ptr<અચલ uchar>();

                જો( normType == NORM_L2 || normType == NORM_L2SQR || normType == NORM_L1 )
                {
                    દ્વિગુણ result = 0;
                    func(data, 0, (uchar*)&result, (પૂર્ણાંક)len, 1);
                    પાછા normType == NORM_L2 ? std::sqrt(result) : result;
                }
                જો( normType == NORM_INF )
                {
                    દશાંશ result = 0;
                    func(data, 0, (uchar*)&result, (પૂર્ણાંક)len, 1);
                    પાછા result;
                }
            }
            જો( depth == CV_8U )
            {
                અચલ uchar* data = src.ptr<uchar>();

                જો( normType == NORM_HAMMING )
                {
                    પાછા hal::normHamming(data, (પૂર્ણાંક)len, 1);
                }

                જો( normType == NORM_HAMMING2 )
                {
                    પાછા hal::normHamming(data, (પૂર્ણાંક)len, 2);
                }
            }
        }
    }

    CV_Assert( mask.empty() || mask.type() == CV_8U || mask.type() == CV_8S || mask.type() == CV_Bool );

    જો( normType == NORM_HAMMING || normType == NORM_HAMMING2 )
    {
        જો( !mask.empty() )
        {
            Mat temp;
            bitwise_and(src, mask, temp);
            પાછા norm(temp, normType);
        }
        પૂર્ણાંક cellSize = normType == NORM_HAMMING ? 1 : 2;

        અચલ Mat* arrays[] = {&src, 0};
        uchar* ptrs[1] = {};
        NAryMatIterator it(arrays, ptrs);
        પૂર્ણાંક total = (પૂર્ણાંક)it.size;
        પૂર્ણાંક result = 0;

        માટે( size_t i = 0; i < it.nplanes; i++, ++it )
        {
            result += hal::normHamming(ptrs[0], total, cellSize);
        }

        પાછા result;
    }

    અચલ Mat* arrays[] = {&src, &mask, 0};
    uchar* ptrs[2] = {};
    સંઘ
    {
        દ્વિગુણ d;
        unsigned u;
        uint64 UL;
        દશાંશ f;
    }
    result;
    result.d = 0;
    NAryMatIterator it(arrays, ptrs);
    CV_CheckLT((size_t)it.size, (size_t)INT_MAX, "");
    તાર્કિક is_fp16 = depth == CV_16F || depth == CV_16BF;

    જો ((normType == NORM_L1 && (depth <= CV_16S || depth == CV_Bool || is_fp16)) ||
        ((normType == NORM_L2 || normType == NORM_L2SQR) && (depth <= CV_8S || depth == CV_Bool || is_fp16)))
    {
        // special case to handle "integer" overflow in accumulator
        અચલ size_t esz = src.elemSize();
        અચલ પૂર્ણાંક total = (પૂર્ણાંક)it.size;
        અચલ પૂર્ણાંક blockSize0 = (is_fp16 ? (1 << 10) :
            depth == CV_Bool ? (1 << 30) :
            normType == NORM_L1 && depth <= CV_8S ? (1 << 23) : (1 << 15))/cn;
        અચલ પૂર્ણાંક blockSize = std::min(total, blockSize0);
        સંઘ {
            પૂર્ણાંક i;
            દશાંશ f;
        } blocksum;
        blocksum.i = 0;
        પૂર્ણાંક ગણતરી_2 = 0;

        માટે (size_t i = 0; i < it.nplanes; i++, ++it)
        {
            માટે (પૂર્ણાંક j = 0; j < total; j += blockSize)
            {
                પૂર્ણાંક bsz = std::min(total - j, blockSize);
                func(ptrs[0], ptrs[1], (uchar*)&blocksum, bsz, cn);
                ગણતરી_2 += bsz;
                જો (ગણતરી_2 + blockSize >= blockSize0 || (i+1 >= it.nplanes && j+bsz >= total))
                {
                    result.d += is_fp16 ? (દ્વિગુણ)blocksum.f : (દ્વિગુણ)blocksum.i;
                    blocksum.i = 0;
                    ગણતરી_2 = 0;
                }
                ptrs[0] += bsz*esz;
                જો (ptrs[1])
                    ptrs[1] += bsz;
            }
        }
    }
    નહીંતર
    {
        // generic implementation
        માટે (size_t i = 0; i < it.nplanes; i++, ++it)
        {
            func(ptrs[0], ptrs[1], (uchar*)&result, (પૂર્ણાંક)it.size, cn);
        }
    }

    જો( normType == NORM_INF )
    {
        જો(depth <= CV_32S || depth == CV_32U || depth == CV_Bool)
            પાછા result.u;
        જો (depth == CV_32F || is_fp16)
            પાછા result.f;
        જો (depth == CV_64U || depth == CV_64S)
            પાછા (દ્વિગુણ)result.UL;
    }
    જો( normType == NORM_L2 )
        પાછા std::sqrt(result.d);

    પાછા result.d;
}

//==================================================================================================

#ifdef HAVE_OPENCL
સ્થિર તાર્કિક ocl_norm( આગત_Array _src1, આગત_Array _src2, પૂર્ણાંક normType, આગત_Array _mask, દ્વિગુણ & result )
{
#ifdef __ANDROID__
    જો (ocl::Device::getDefault().isNVidia())
        પાછા ખોટું;
#endif

    Scalar sc1, sc2;
    પૂર્ણાંક cn = _src1.channels();
    જો (cn > 4)
        પાછા ખોટું;
    પૂર્ણાંક type = _src1.type(), depth = CV_MAT_DEPTH(type);
    તાર્કિક relative = (normType & NORM_RELATIVE) != 0;
    normType &= ~NORM_RELATIVE;
    તાર્કિક normsum = normType == NORM_L1 || normType == NORM_L2 || normType == NORM_L2SQR;

#ifdef __APPLE__
    જો(normType == NORM_L1 && type == CV_16UC3 && !_mask.empty())
        પાછા ખોટું;
#endif

    જો (normsum)
    {
        જો (!ocl_sum(_src1, sc1, normType == NORM_L2 || normType == NORM_L2SQR ?
                     OCL_OP_SUM_SQR : OCL_OP_SUM, _mask, _src2, relative, sc2))
            પાછા ખોટું;
    }
    નહીંતર
    {
        જો (!ocl_minMaxIdx(_src1, ખાલી, &sc1[0], ખાલી, ખાલી, _mask, std::max(CV_32S, depth),
                           ખોટું, _src2, relative ? &sc2[0] : ખાલી))
            પાછા ખોટું;
        cn = 1;
    }

    દ્વિગુણ s2 = 0;
    માટે (પૂર્ણાંક i = 0; i < cn; ++i)
    {
        result += sc1[i];
        જો (relative)
            s2 += sc2[i];
    }

    જો (normType == NORM_L2)
    {
        result = std::sqrt(result);
        જો (relative)
            s2 = std::sqrt(s2);
    }

    જો (relative)
        result /= (s2 + DBL_EPSILON);

    પાછા સાચું;
}  // ocl_norm()
#endif  // HAVE_OPENCL

દ્વિગુણ norm( આગત_Array _src1, આગત_Array _src2, પૂર્ણાંક normType, આગત_Array _mask )
{
    CV_INSTRUMENT_REGION();

    CV_CheckTypeEQ(_src1.type(), _src2.type(), "Input type mismatch");
    CV_Assert(_src1.sameSize(_src2));

#જો defined HAVE_OPENCL
    દ્વિગુણ _result = 0;
#endif

#ifdef HAVE_OPENCL
    CV_OCL_RUN_(OCL_PERFORMANCE_CHECK(_src1.isUMat()),
                ocl_norm(_src1, _src2, normType, _mask, _result),
                _result)
#endif

    Mat src1 = _src1.getMat(), src2 = _src2.getMat(), mask = _mask.getMat();
    CV_Assert( mask.empty() || ((mask.type() == CV_8U || mask.type() == CV_8S || mask.type() == CV_Bool) && src1.size == mask.size) );
    જો( src1.empty() || src2.empty() )
        પાછા 0.0;
    પૂર્ણાંક depth = src1.depth(), cn = src1.channels();
    જો( src1.dims <= 2 )
    {
        દ્વિગુણ result;
        CALL_HAL_RET(normDiff, cv_hal_normDiff, result, src1.data, src1.step, src2.data, src2.step, mask.data, mask.step, src1.cols, src1.rows, src1.type(), normType);
    }
    નહીંતર જો( src1.isContinuous() && src2.isContinuous() && mask.isContinuous() )
    {
        દ્વિગુણ result;
        CALL_HAL_RET(normDiff, cv_hal_normDiff, result, src1.data, 0, src2.data, 0, mask.data, 0, (પૂર્ણાંક)src1.total(), 1, src1.type(), normType);
    }

    જો( normType & NORM_RELATIVE )
    {
        પાછા norm(_src1, _src2, normType & ~NORM_RELATIVE, _mask)/(norm(_src2, normType, _mask) + DBL_EPSILON);
    }

    normType &= 7;
    CV_Assert( normType == NORM_INF || normType == NORM_L1 ||
               normType == NORM_L2 || normType == NORM_L2SQR ||
              ((normType == NORM_HAMMING || normType == NORM_HAMMING2) && src1.type() == CV_8U) );

    NormDiffFunc func = getNormDiffFunc(normType >> 1, depth);
    CV_Assert( (normType >> 1) >= 3 || func != 0 );

    જો( src1.isContinuous() && src2.isContinuous() && mask.empty() )
    {
        size_t len = src1.total()*src1.channels();
        જો( len == (size_t)(પૂર્ણાંક)len )
        {
            જો( src1.depth() == CV_32F )
            {
                અચલ uchar* data1 = src1.ptr<અચલ uchar>();
                અચલ uchar* data2 = src2.ptr<અચલ uchar>();

                જો( normType == NORM_L2 || normType == NORM_L2SQR || normType == NORM_L1 )
                {
                    દ્વિગુણ result = 0;
                    func(data1, data2, 0, (uchar*)&result, (પૂર્ણાંક)len, 1);
                    પાછા normType == NORM_L2 ? std::sqrt(result) : result;
                }
                જો( normType == NORM_INF )
                {
                    દશાંશ result = 0;
                    func(data1, data2, 0, (uchar*)&result, (પૂર્ણાંક)len, 1);
                    પાછા result;
                }
            }
        }
    }

    CV_Assert( mask.empty() || mask.type() == CV_8U || mask.type() == CV_8S || mask.type() == CV_Bool );

    જો( normType == NORM_HAMMING || normType == NORM_HAMMING2 )
    {
        જો( !mask.empty() )
        {
            Mat temp;
            bitwise_xor(src1, src2, temp);
            bitwise_and(temp, mask, temp);
            પાછા norm(temp, normType);
        }
        પૂર્ણાંક cellSize = normType == NORM_HAMMING ? 1 : 2;

        અચલ Mat* arrays[] = {&src1, &src2, 0};
        uchar* ptrs[2] = {};
        NAryMatIterator it(arrays, ptrs);
        પૂર્ણાંક total = (પૂર્ણાંક)it.size;
        પૂર્ણાંક result = 0;

        માટે( size_t i = 0; i < it.nplanes; i++, ++it )
        {
            result += hal::normHamming(ptrs[0], ptrs[1], total, cellSize);
        }

        પાછા result;
    }

    અચલ Mat* arrays[] = {&src1, &src2, &mask, 0};
    uchar* ptrs[3] = {};
    સંઘ
    {
        દ્વિગુણ d;
        દશાંશ f;
        unsigned u;
        uint64 UL;
    }
    result;
    result.d = 0;
    NAryMatIterator it(arrays, ptrs);
    CV_CheckLT((size_t)it.size, (size_t)INT_MAX, "");

    તાર્કિક is_fp16 = depth == CV_16F || depth == CV_16BF;

    જો ((normType == NORM_L1 && (depth <= CV_16S || depth == CV_Bool || is_fp16)) ||
        ((normType == NORM_L2 || normType == NORM_L2SQR) && (depth <= CV_8S || depth == CV_Bool || is_fp16)))
    {
        // special case to handle "integer" overflow in accumulator
        અચલ size_t esz = src1.elemSize();
        અચલ પૂર્ણાંક total = (પૂર્ણાંક)it.size;
        અચલ પૂર્ણાંક blockSize0 = (is_fp16 ? (1 << 10) :
            depth == CV_Bool ? (1 << 30) :
            normType == NORM_L1 && depth <= CV_8S ? (1 << 23) : (1 << 15))/cn;
        અચલ પૂર્ણાંક blockSize = std::min(total, blockSize0);
        સંઘ {
            પૂર્ણાંક i;
            દશાંશ f;
        } blocksum;
        blocksum.i = 0;
        પૂર્ણાંક ગણતરી_2 = 0;

        માટે (size_t i = 0; i < it.nplanes; i++, ++it)
        {
            માટે (પૂર્ણાંક j = 0; j < total; j += blockSize)
            {
                પૂર્ણાંક bsz = std::min(total - j, blockSize);
                func(ptrs[0], ptrs[1], ptrs[2], (uchar*)&blocksum, bsz, cn);
                ગણતરી_2 += bsz;
                જો (ગણતરી_2 + blockSize >= blockSize0 || (i+1 >= it.nplanes && j+bsz >= total))
                {
                    result.d += is_fp16 ? (દ્વિગુણ)blocksum.f : (દ્વિગુણ)blocksum.i;
                    blocksum.i = 0;
                    ગણતરી_2 = 0;
                }
                ptrs[0] += bsz*esz;
                ptrs[1] += bsz*esz;
                જો (ptrs[2])
                    ptrs[2] += bsz;
            }
        }
    }
    નહીંતર
    {
        // generic implementation
        માટે (size_t i = 0; i < it.nplanes; i++, ++it)
        {
            func(ptrs[0], ptrs[1], ptrs[2], (uchar*)&result, (પૂર્ણાંક)it.size, cn);
        }
    }

    જો( normType == NORM_INF )
    {
        જો (depth <= CV_32S || depth == CV_32U || depth == CV_Bool)
            પાછા result.u;
        જો (depth == CV_32F || is_fp16)
            પાછા result.f;
        જો (depth == CV_64U || depth == CV_64S)
            પાછા (દ્વિગુણ)result.UL;
    }
    જો( normType == NORM_L2 )
        પાછા std::sqrt(result.d);

    પાછા result.d;
}

cv::Hamming::ResultType Hamming::operator()( અચલ unsigned અક્ષર* a, અચલ unsigned અક્ષર* b, પૂર્ણાંક size ) અચલ
{
    પાછા cv::hal::normHamming(a, b, size);
}

દ્વિગુણ PSNR(આગત_Array _src1, આગત_Array _src2, દ્વિગુણ R)
{
    CV_INSTRUMENT_REGION();

    //Input arrays must have depth CV_8U
    CV_Assert( _src1.type() == _src2.type() );

    દ્વિગુણ diff = std::sqrt(norm(_src1, _src2, NORM_L2SQR)/(_src1.total()*_src1.channels()));
    પાછા 20*log10(R/(diff+DBL_EPSILON));
}


#ifdef HAVE_OPENCL
સ્થિર તાર્કિક ocl_normalize( આગત_Array _src, આગત_Output_Array _dst, આગત_Array _mask, પૂર્ણાંક dtype,
                           દ્વિગુણ scale, દ્વિગુણ delta )
{
    UMat src = _src.getUMat();

    જો( _mask.empty() )
        src.convertTo( _dst, dtype, scale, delta );
    નહીંતર જો (src.channels() <= 4)
    {
        અચલ ocl::Device & dev = ocl::Device::getDefault();

        પૂર્ણાંક stype = _src.type(), sdepth = CV_MAT_DEPTH(stype), cn = CV_MAT_CN(stype),
                ddepth = CV_MAT_DEPTH(dtype), wdepth = std::max(CV_32F, std::max(sdepth, ddepth)),
                rowsPerWI = dev.isIntel() ? 4 : 1;

        દશાંશ fscale = static_cast<દશાંશ>(scale), fdelta = static_cast<દશાંશ>(delta);
        તાર્કિક haveScale = std::fabs(scale - 1) > DBL_EPSILON,
                haveZeroScale = !(std::fabs(scale) > DBL_EPSILON),
                haveDelta = std::fabs(delta) > DBL_EPSILON,
                doubleSupport = dev.doubleFPConfig() > 0;

        જો (!haveScale && !haveDelta && stype == dtype)
        {
            _src.copyTo(_dst, _mask);
            પાછા સાચું;
        }
        જો (haveZeroScale)
        {
            _dst.setTo(Scalar(delta), _mask);
            પાછા સાચું;
        }

        જો ((sdepth == CV_64F || ddepth == CV_64F) && !doubleSupport)
            પાછા ખોટું;

        અક્ષર cvt[2][50];
        String opts = format("-D srcT=%s -D dstT=%s -D convertToWT=%s -D cn=%d -D rowsPerWI=%d"
                             " -D convertToDT=%s -D workT=%s%s%s%s -D srcT1=%s -D dstT1=%s",
                             ocl::typeToStr(stype), ocl::typeToStr(dtype),
                             ocl::convertTypeStr(sdepth, wdepth, cn, cvt[0], કદ(cvt[0])), cn,
                             rowsPerWI, ocl::convertTypeStr(wdepth, ddepth, cn, cvt[1], કદ(cvt[1])),
                             ocl::typeToStr(CV_MAKE_TYPE(wdepth, cn)),
                             doubleSupport ? " -D DOUBLE_SUPPORT" : "",
                             haveScale ? " -D HAVE_SCALE" : "",
                             haveDelta ? " -D HAVE_DELTA" : "",
                             ocl::typeToStr(sdepth), ocl::typeToStr(ddepth));

        ocl::Kernel k("normalizek", ocl::core::normalize_oclsrc, opts);
        જો (k.empty())
            પાછા ખોટું;

        UMat mask = _mask.getUMat(), dst = _dst.getUMat();

        ocl::KernelArg srcarg = ocl::KernelArg::ReadOnlyNoSize(src),
                maskarg = ocl::KernelArg::ReadOnlyNoSize(mask),
                dstarg = ocl::KernelArg::ReadWrite(dst);

        જો (haveScale)
        {
            જો (haveDelta)
                k.args(srcarg, maskarg, dstarg, fscale, fdelta);
            નહીંતર
                k.args(srcarg, maskarg, dstarg, fscale);
        }
        નહીંતર
        {
            જો (haveDelta)
                k.args(srcarg, maskarg, dstarg, fdelta);
            નહીંતર
                k.args(srcarg, maskarg, dstarg);
        }

        size_t globalsize[2] = { (size_t)src.cols, ((size_t)src.rows + rowsPerWI - 1) / rowsPerWI };
        પાછા k.run(2, globalsize, ખાલી, ખોટું);
    }
    નહીંતર
    {
        UMat temp;
        src.convertTo( temp, dtype, scale, delta );
        temp.copyTo( _dst, _mask );
    }

    પાછા સાચું;
}  // ocl_normalize
#endif  // HAVE_OPENCL

શૂન્ય normalize(આગત_Array _src, આગત_Output_Array _dst, દ્વિગુણ a, દ્વિગુણ b,
               પૂર્ણાંક norm_type, પૂર્ણાંક rtype, આગત_Array _mask)
{
    CV_INSTRUMENT_REGION();

    દ્વિગુણ scale = 1, shift = 0;
    પૂર્ણાંક type = _src.type(), depth = CV_MAT_DEPTH(type);

    જો( rtype < 0 )
        rtype = _dst.fixedType() ? _dst.depth() : depth;

    જો( norm_type == NORM_MINMAX )
    {
        દ્વિગુણ smin = 0, smax = 0;
        દ્વિગુણ dmin = MIN( a, b ), dmax = MAX( a, b );
        minMaxIdx( _src, &smin, &smax, 0, 0, _mask );
        scale = (dmax - dmin)*(smax - smin > DBL_EPSILON ? 1./(smax - smin) : 0);
        જો( rtype == CV_32F )
        {
            scale = (દશાંશ)scale;
            shift = (દશાંશ)dmin - (દશાંશ)(smin*scale);
        }
        નહીંતર
            shift = dmin - smin*scale;
    }
    નહીંતર જો( norm_type == NORM_L2 || norm_type == NORM_L1 || norm_type == NORM_INF )
    {
        scale = norm( _src, norm_type, _mask );
        scale = scale > DBL_EPSILON ? a/scale : 0.;
        shift = 0;
    }
    નહીંતર
        CV_Error( cv::Error::StsBadArg, "Unknown/unsupported norm type" );

    CV_OCL_RUN(_dst.isUMat(),
               ocl_normalize(_src, _dst, _mask, rtype, scale, shift))

    Mat src = _src.getMat();
    જો( _mask.empty() )
        src.convertTo( _dst, rtype, scale, shift );
    નહીંતર
    {
        Mat temp;
        src.convertTo( temp, rtype, scale, shift );
        temp.copyTo( _dst, _mask );
    }
}

}  // namespace
