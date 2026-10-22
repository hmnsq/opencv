// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html
//
// Copyright (C) 2026, Advanced Micro Devices, Inc., all rights reserved.


#समावेश "precomp.hpp"
#समावेश "opencl_kernels_core.hpp"
#समावेश "stat.hpp"

#समावेश "norm.simd.hpp"
#समावेश "norm.simd_declarations.hpp"

/****************************************************************************************\
*                                         norm                                           *
\****************************************************************************************/

namespace cv { namespace hal {

बाह्य स्थिर uchar निकालो_गिनती_Table[256] =
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

अटल स्थिर uchar निकालो_गिनती_Table2[] =
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

अटल स्थिर uchar निकालो_गिनती_Table4[] =
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


पूर्णांक normHamming(स्थिर uchar* a, पूर्णांक n, पूर्णांक cell_आकार)
{
    पूर्णांक output;
    CALL_HAL_RET(normHamming8u, cv_hal_normHamming8u, output, a, n, cell_आकार);

    अगर( cell_आकार == 1 )
        वापस normHamming(a, n);
    स्थिर uchar* tab = 0;
    अगर( cell_आकार == 2 )
        tab = निकालो_गिनती_Table2;
    अन्यथा अगर( cell_आकार == 4 )
        tab = निकालो_गिनती_Table4;
    अन्यथा
        वापस -1;
    पूर्णांक i = 0;
    पूर्णांक परिणाम = 0;
#अगर (CV_SIMD || CV_SIMD_SCALABLE)
    v_uint64 t = vx_setzero_u64();
    अगर ( cell_आकार == 2)
    {
        v_uint16 mask = v_reinterpret_as_u16(vx_setall_u8(0x55));
        क्रम(; i <= n - VTraits<v_uint8>::vlanes(); i += VTraits<v_uint8>::vlanes())
        {
            v_uint16 a0 = v_reinterpret_as_u16(vx_load(a + i));
            t = v_add(t, v_popcount(v_reinterpret_as_u64(v_and(v_or(a0, v_shr<1>(a0)), mask))));
        }
    }
    अन्यथा    // cellSize == 4
    {
        v_uint16 mask = v_reinterpret_as_u16(vx_setall_u8(0x11));
        क्रम(; i <= n - VTraits<v_uint8>::vlanes(); i += VTraits<v_uint8>::vlanes())
        {
            v_uint16 a0 = v_reinterpret_as_u16(vx_load(a + i));
            v_uint16 a1 = v_or(a0, v_shr<2>(a0));
            t = v_add(t, v_popcount(v_reinterpret_as_u64(v_and(v_or(a1, v_shr<1>(a1)), mask))));

        }
    }
    परिणाम += (पूर्णांक)v_reduce_योग(t);
    vx_cleanup();
#या_अगर CV_ENABLE_UNROLLED
    क्रम( ; i <= n - 4; i += 4 )
        परिणाम += tab[a[i]] + tab[a[i+1]] + tab[a[i+2]] + tab[a[i+3]];
#पूर्ण_अगर
    क्रम( ; i < n; i++ )
        परिणाम += tab[a[i]];
    वापस परिणाम;
}

पूर्णांक normHamming(स्थिर uchar* a, स्थिर uchar* b, पूर्णांक n, पूर्णांक cell_आकार)
{
    पूर्णांक output;
    CALL_HAL_RET(normHammingDiff8u, cv_hal_normHammingDiff8u, output, a, b, n, cell_आकार);

    अगर( cell_आकार == 1 )
        वापस normHamming(a, b, n);
    स्थिर uchar* tab = 0;
    अगर( cell_आकार == 2 )
        tab = निकालो_गिनती_Table2;
    अन्यथा अगर( cell_आकार == 4 )
        tab = निकालो_गिनती_Table4;
    अन्यथा
        वापस -1;
    पूर्णांक i = 0;
    पूर्णांक परिणाम = 0;
#अगर (CV_SIMD || CV_SIMD_SCALABLE)
    v_uint64 t = vx_setzero_u64();
    अगर ( cell_आकार == 2)
    {
        v_uint16 mask = v_reinterpret_as_u16(vx_setall_u8(0x55));
        क्रम(; i <= n - VTraits<v_uint8>::vlanes(); i += VTraits<v_uint8>::vlanes())
        {
            v_uint16 ab0 = v_reinterpret_as_u16(v_xor(vx_load(a + i), vx_load(b + i)));
            t = v_add(t, v_popcount(v_reinterpret_as_u64(v_and(v_or(ab0, v_shr<1>(ab0)), mask))));
        }
    }
    अन्यथा    // cellSize == 4
    {
        v_uint16 mask = v_reinterpret_as_u16(vx_setall_u8(0x11));
        क्रम(; i <= n - VTraits<v_uint8>::vlanes(); i += VTraits<v_uint8>::vlanes())
        {
            v_uint16 ab0 = v_reinterpret_as_u16(v_xor(vx_load(a + i), vx_load(b + i)));
            v_uint16 ab1 = v_or(ab0, v_shr<2>(ab0));
            t = v_add(t, v_popcount(v_reinterpret_as_u64(v_and(v_or(ab1, v_shr<1>(ab1)), mask))));
        }
    }
    परिणाम += (पूर्णांक)v_reduce_योग(t);
    vx_cleanup();
#या_अगर CV_ENABLE_UNROLLED
    क्रम( ; i <= n - 4; i += 4 )
        परिणाम += tab[a[i] ^ b[i]] + tab[a[i+1] ^ b[i+1]] +
                tab[a[i+2] ^ b[i+2]] + tab[a[i+3] ^ b[i+3]];
#पूर्ण_अगर
    क्रम( ; i < n; i++ )
        परिणाम += tab[a[i] ^ b[i]];
    वापस परिणाम;
}

भग्न normL2Sqr_(स्थिर भग्न* a, स्थिर भग्न* b, पूर्णांक n)
{
    पूर्णांक j = 0; भग्न d = 0.f;
#अगर (CV_SIMD || CV_SIMD_SCALABLE)
    स्थिर पूर्णांक vl = VTraits<v_float32>::vlanes();
    v_float32 v_d0 = vx_setzero_f32(), v_d1 = vx_setzero_f32();
    v_float32 v_d2 = vx_setzero_f32(), v_d3 = vx_setzero_f32();
    क्रम (; j <= n - 4 * vl; j += 4 * vl)
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
    क्रम (; j <= n - vl; j += vl)
    {
        v_float32 t0 = v_sub(vx_load(a + j), vx_load(b + j));
        v_d0 = v_muladd(t0, t0, v_d0);
    }
    d = v_reduce_योग(v_d0);
#पूर्ण_अगर
    क्रम( ; j < n; j++ )
    {
        भग्न t = a[j] - b[j];
        d += t*t;
    }
    वापस d;
}


भग्न normL1_(स्थिर भग्न* a, स्थिर भग्न* b, पूर्णांक n)
{
    पूर्णांक j = 0; भग्न d = 0.f;
#अगर (CV_SIMD || CV_SIMD_SCALABLE)
    स्थिर पूर्णांक vl = VTraits<v_float32>::vlanes();
    v_float32 v_d0 = vx_setzero_f32(), v_d1 = vx_setzero_f32();
    v_float32 v_d2 = vx_setzero_f32(), v_d3 = vx_setzero_f32();
    क्रम (; j <= n - 4 * vl; j += 4 * vl)
    {
        v_d0 = v_add(v_d0, v_absdiff(vx_load(a + j), vx_load(b + j)));
        v_d1 = v_add(v_d1, v_absdiff(vx_load(a + j + vl), vx_load(b + j + vl)));
        v_d2 = v_add(v_d2, v_absdiff(vx_load(a + j + 2 * vl), vx_load(b + j + 2 * vl)));
        v_d3 = v_add(v_d3, v_absdiff(vx_load(a + j + 3 * vl), vx_load(b + j + 3 * vl)));
    }
    v_d0 = v_add(v_add(v_d0, v_d1), v_add(v_d2, v_d3));
    क्रम (; j <= n - vl; j += vl)
        v_d0 = v_add(v_d0, v_absdiff(vx_load(a + j), vx_load(b + j)));
    d = v_reduce_योग(v_d0);
#पूर्ण_अगर
    क्रम( ; j < n; j++ )
        d += std::असल(a[j] - b[j]);
    वापस d;
}

पूर्णांक normL1_(स्थिर uchar* a, स्थिर uchar* b, पूर्णांक n)
{
    पूर्णांक j = 0, d = 0;
#अगर (CV_SIMD || CV_SIMD_SCALABLE)
    स्थिर पूर्णांक vl = VTraits<v_uint8>::vlanes();
    क्रम (; j <= n - 4 * vl; j += 4 * vl)
        d += v_reduce_sad(vx_load(a + j), vx_load(b + j)) +
             v_reduce_sad(vx_load(a + j + vl), vx_load(b + j + vl)) +
             v_reduce_sad(vx_load(a + j + 2 * vl), vx_load(b + j + 2 * vl)) +
             v_reduce_sad(vx_load(a + j + 3 * vl), vx_load(b + j + 3 * vl));
    क्रम (; j <= n - vl; j += vl)
        d += v_reduce_sad(vx_load(a + j), vx_load(b + j));
#पूर्ण_अगर
    क्रम( ; j < n; j++ )
        d += std::असल(a[j] - b[j]);
    वापस d;
}

} //cv::hal

//==================================================================================================

प्रकार पूर्णांक (*NormFunc)(स्थिर uchar*, स्थिर uchar*, uchar*, पूर्णांक, पूर्णांक);
प्रकार पूर्णांक (*NormDiffFunc)(स्थिर uchar*, स्थिर uchar*, स्थिर uchar*, uchar*, पूर्णांक, पूर्णांक);

#अगर_घोषित HAVE_OPENCL

अटल bool ocl_norm( निवेश_Array _src, पूर्णांक normType, निवेश_Array _mask, द्विगुन & परिणाम )
{
    स्थिर ocl::Device & d = ocl::Device::getDefault();

#अगर_घोषित __ANDROID__
    अगर (d.isNVidia())
        वापस false;
#पूर्ण_अगर
    स्थिर पूर्णांक cn = _src.channels();
    अगर (cn > 4)
        वापस false;
    पूर्णांक type = _src.type(), depth = CV_MAT_DEPTH(type);
    bool doubleSupport = d.doubleFPConfig() > 0,
            haveMask = _mask.kind() != निवेश_Array_2::NONE;

    अगर (depth >= CV_16F)
        वापस false;  // TODO: support FP16

    अगर ( !(normType == NORM_INF || normType == NORM_L1 || normType == NORM_L2 || normType == NORM_L2SQR) ||
         (!doubleSupport && depth == CV_64F))
        वापस false;

    UMat src = _src.getUMat();

    अगर (normType == NORM_INF)
    {
        अगर (!ocl_minMaxIdx(_src, शून्य, &परिणाम, शून्य, शून्य, _mask,
                           std::max(depth, CV_32S), depth != CV_8U && depth != CV_16U))
            वापस false;
    }
    अन्यथा अगर (normType == NORM_L1 || normType == NORM_L2 || normType == NORM_L2SQR)
    {
        Scalar sc;
        bool unstype = depth == CV_8U || depth == CV_16U;

        अगर ( !ocl_योग(haveMask ? src : src.reshape(1), sc, normType == NORM_L2 || normType == NORM_L2SQR ?
                    OCL_OP_योग_SQR : (unstype ? OCL_OP_योग : OCL_OP_योग_ABS), _mask) )
            वापस false;

        द्विगुन s = 0.0;
        क्रम (पूर्णांक i = 0; i < (haveMask ? cn : 1); ++i)
            s += sc[i];

        परिणाम = normType == NORM_L1 || normType == NORM_L2SQR ? s : std::वर्ग_मूल(s);
    }

    वापस true;
}

#पूर्ण_अगर

अटल NormFunc getNormFunc(पूर्णांक normType, पूर्णांक depth) {
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getNormFunc, (normType, depth), CV_CPU_DISPATCH_MODES_ALL);
}
अटल NormDiffFunc getNormDiffFunc(पूर्णांक normType, पूर्णांक depth) {
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getNormDiffFunc, (normType, depth), CV_CPU_DISPATCH_MODES_ALL);
}

द्विगुन norm( निवेश_Array _src, पूर्णांक normType, निवेश_Array _mask )
{
    CV_INSTRUMENT_REGION();

    normType &= NORM_TYPE_MASK;
    CV_Assert( normType == NORM_INF || normType == NORM_L1 ||
               normType == NORM_L2 || normType == NORM_L2SQR ||
               ((normType == NORM_HAMMING || normType == NORM_HAMMING2) && _src.type() == CV_8U) );

#अगर defined HAVE_OPENCL
    द्विगुन परिणाम_2 = 0;
#पूर्ण_अगर

#अगर_घोषित HAVE_OPENCL
    CV_OCL_RUN_(OCL_PERFORMANCE_CHECK(_src.isUMat()) && _src.dims() <= 2,
                ocl_norm(_src, normType, _mask, परिणाम_2),
                परिणाम_2)
#पूर्ण_अगर

    Mat src = _src.getMat(), mask = _mask.getMat();
    CV_Assert( mask.empty() || ((mask.type() == CV_8U || mask.type() == CV_8S || mask.type() == CV_Bool) && src.size == mask.size) );
    अगर( src.empty() )
        वापस 0.0;
    पूर्णांक depth = src.depth(), cn = src.channels();
    अगर( src.dims <= 2 )
    {
        द्विगुन परिणाम;
        CALL_HAL_RET(norm, cv_hal_norm, परिणाम, src.तथ्य, src.step, mask.तथ्य, mask.step, src.cols, src.rows, src.type(), normType);
    }
    अन्यथा अगर( src.isContinuous() && mask.isContinuous() )
    {
        द्विगुन परिणाम;
        CALL_HAL_RET(norm, cv_hal_norm, परिणाम, src.तथ्य, 0, mask.तथ्य, 0, (पूर्णांक)src.योग(), 1, src.type(), normType);
    }

    NormFunc func = getNormFunc(normType >> 1, depth);
    CV_Assert( (normType >> 1) >= 3 || func != 0 );

    अगर( src.isContinuous() && mask.empty() )
    {
        माप_प्रकार len = src.योग()*cn;
        अगर( len == (माप_प्रकार)(पूर्णांक)len )
        {
            अगर( depth == CV_32F )
            {
                स्थिर uchar* तथ्य = src.ptr<स्थिर uchar>();

                अगर( normType == NORM_L2 || normType == NORM_L2SQR || normType == NORM_L1 )
                {
                    द्विगुन परिणाम = 0;
                    func(तथ्य, 0, (uchar*)&परिणाम, (पूर्णांक)len, 1);
                    वापस normType == NORM_L2 ? std::वर्ग_मूल(परिणाम) : परिणाम;
                }
                अगर( normType == NORM_INF )
                {
                    भग्न परिणाम = 0;
                    func(तथ्य, 0, (uchar*)&परिणाम, (पूर्णांक)len, 1);
                    वापस परिणाम;
                }
            }
            अगर( depth == CV_8U )
            {
                स्थिर uchar* तथ्य = src.ptr<uchar>();

                अगर( normType == NORM_HAMMING )
                {
                    वापस hal::normHamming(तथ्य, (पूर्णांक)len, 1);
                }

                अगर( normType == NORM_HAMMING2 )
                {
                    वापस hal::normHamming(तथ्य, (पूर्णांक)len, 2);
                }
            }
        }
    }

    CV_Assert( mask.empty() || mask.type() == CV_8U || mask.type() == CV_8S || mask.type() == CV_Bool );

    अगर( normType == NORM_HAMMING || normType == NORM_HAMMING2 )
    {
        अगर( !mask.empty() )
        {
            Mat अस्थायी_2;
            bitwise_and(src, mask, अस्थायी_2);
            वापस norm(अस्थायी_2, normType);
        }
        पूर्णांक cell_आकार = normType == NORM_HAMMING ? 1 : 2;

        स्थिर Mat* arrays[] = {&src, 0};
        uchar* ptrs[1] = {};
        NAryMatIterator it(arrays, ptrs);
        पूर्णांक योग = (पूर्णांक)it.size;
        पूर्णांक परिणाम = 0;

        क्रम( माप_प्रकार i = 0; i < it.nplanes; i++, ++it )
        {
            परिणाम += hal::normHamming(ptrs[0], योग, cell_आकार);
        }

        वापस परिणाम;
    }

    स्थिर Mat* arrays[] = {&src, &mask, 0};
    uchar* ptrs[2] = {};
    जोड़
    {
        द्विगुन d;
        अचिन्हित u;
        uint64 UL;
        भग्न f;
    }
    परिणाम;
    परिणाम.d = 0;
    NAryMatIterator it(arrays, ptrs);
    CV_CheckLT((माप_प्रकार)it.size, (माप_प्रकार)पूर्णांक_उच्च, "");
    bool is_fp16 = depth == CV_16F || depth == CV_16BF;

    अगर ((normType == NORM_L1 && (depth <= CV_16S || depth == CV_Bool || is_fp16)) ||
        ((normType == NORM_L2 || normType == NORM_L2SQR) && (depth <= CV_8S || depth == CV_Bool || is_fp16)))
    {
        // special case to handle "integer" overflow in accumulator
        स्थिर माप_प्रकार esz = src.elem_आकार();
        स्थिर पूर्णांक योग = (पूर्णांक)it.size;
        स्थिर पूर्णांक blockSize0 = (is_fp16 ? (1 << 10) :
            depth == CV_Bool ? (1 << 30) :
            normType == NORM_L1 && depth <= CV_8S ? (1 << 23) : (1 << 15))/cn;
        स्थिर पूर्णांक block_आकार = std::min(योग, blockSize0);
        जोड़ {
            पूर्णांक i;
            भग्न f;
        } blocksum;
        blocksum.i = 0;
        पूर्णांक गिनती_2 = 0;

        क्रम (माप_प्रकार i = 0; i < it.nplanes; i++, ++it)
        {
            क्रम (पूर्णांक j = 0; j < योग; j += block_आकार)
            {
                पूर्णांक bsz = std::min(योग - j, block_आकार);
                func(ptrs[0], ptrs[1], (uchar*)&blocksum, bsz, cn);
                गिनती_2 += bsz;
                अगर (गिनती_2 + block_आकार >= blockSize0 || (i+1 >= it.nplanes && j+bsz >= योग))
                {
                    परिणाम.d += is_fp16 ? (द्विगुन)blocksum.f : (द्विगुन)blocksum.i;
                    blocksum.i = 0;
                    गिनती_2 = 0;
                }
                ptrs[0] += bsz*esz;
                अगर (ptrs[1])
                    ptrs[1] += bsz;
            }
        }
    }
    अन्यथा
    {
        // generic implementation
        क्रम (माप_प्रकार i = 0; i < it.nplanes; i++, ++it)
        {
            func(ptrs[0], ptrs[1], (uchar*)&परिणाम, (पूर्णांक)it.size, cn);
        }
    }

    अगर( normType == NORM_INF )
    {
        अगर(depth <= CV_32S || depth == CV_32U || depth == CV_Bool)
            वापस परिणाम.u;
        अगर (depth == CV_32F || is_fp16)
            वापस परिणाम.f;
        अगर (depth == CV_64U || depth == CV_64S)
            वापस (द्विगुन)परिणाम.UL;
    }
    अगर( normType == NORM_L2 )
        वापस std::वर्ग_मूल(परिणाम.d);

    वापस परिणाम.d;
}

//==================================================================================================

#अगर_घोषित HAVE_OPENCL
अटल bool ocl_norm( निवेश_Array _src1, निवेश_Array _src2, पूर्णांक normType, निवेश_Array _mask, द्विगुन & परिणाम )
{
#अगर_घोषित __ANDROID__
    अगर (ocl::Device::getDefault().isNVidia())
        वापस false;
#पूर्ण_अगर

    Scalar sc1, sc2;
    पूर्णांक cn = _src1.channels();
    अगर (cn > 4)
        वापस false;
    पूर्णांक type = _src1.type(), depth = CV_MAT_DEPTH(type);
    bool relative = (normType & NORM_RELATIVE) != 0;
    normType &= ~NORM_RELATIVE;
    bool normsum = normType == NORM_L1 || normType == NORM_L2 || normType == NORM_L2SQR;

#अगर_घोषित __APPLE__
    अगर(normType == NORM_L1 && type == CV_16UC3 && !_mask.empty())
        वापस false;
#पूर्ण_अगर

    अगर (normsum)
    {
        अगर (!ocl_योग(_src1, sc1, normType == NORM_L2 || normType == NORM_L2SQR ?
                     OCL_OP_योग_SQR : OCL_OP_योग, _mask, _src2, relative, sc2))
            वापस false;
    }
    अन्यथा
    {
        अगर (!ocl_minMaxIdx(_src1, शून्य, &sc1[0], शून्य, शून्य, _mask, std::max(CV_32S, depth),
                           false, _src2, relative ? &sc2[0] : शून्य))
            वापस false;
        cn = 1;
    }

    द्विगुन s2 = 0;
    क्रम (पूर्णांक i = 0; i < cn; ++i)
    {
        परिणाम += sc1[i];
        अगर (relative)
            s2 += sc2[i];
    }

    अगर (normType == NORM_L2)
    {
        परिणाम = std::वर्ग_मूल(परिणाम);
        अगर (relative)
            s2 = std::वर्ग_मूल(s2);
    }

    अगर (relative)
        परिणाम /= (s2 + द्विग_अंतर);

    वापस true;
}  // ocl_norm()
#पूर्ण_अगर  // HAVE_OPENCL

द्विगुन norm( निवेश_Array _src1, निवेश_Array _src2, पूर्णांक normType, निवेश_Array _mask )
{
    CV_INSTRUMENT_REGION();

    CV_CheckTypeEQ(_src1.type(), _src2.type(), "Input type mismatch");
    CV_Assert(_src1.same_आकार(_src2));

#अगर defined HAVE_OPENCL
    द्विगुन परिणाम_2 = 0;
#पूर्ण_अगर

#अगर_घोषित HAVE_OPENCL
    CV_OCL_RUN_(OCL_PERFORMANCE_CHECK(_src1.isUMat()),
                ocl_norm(_src1, _src2, normType, _mask, परिणाम_2),
                परिणाम_2)
#पूर्ण_अगर

    Mat src1 = _src1.getMat(), src2 = _src2.getMat(), mask = _mask.getMat();
    CV_Assert( mask.empty() || ((mask.type() == CV_8U || mask.type() == CV_8S || mask.type() == CV_Bool) && src1.size == mask.size) );
    अगर( src1.empty() || src2.empty() )
        वापस 0.0;
    पूर्णांक depth = src1.depth(), cn = src1.channels();
    अगर( src1.dims <= 2 )
    {
        द्विगुन परिणाम;
        CALL_HAL_RET(normDiff, cv_hal_normDiff, परिणाम, src1.तथ्य, src1.step, src2.तथ्य, src2.step, mask.तथ्य, mask.step, src1.cols, src1.rows, src1.type(), normType);
    }
    अन्यथा अगर( src1.isContinuous() && src2.isContinuous() && mask.isContinuous() )
    {
        द्विगुन परिणाम;
        CALL_HAL_RET(normDiff, cv_hal_normDiff, परिणाम, src1.तथ्य, 0, src2.तथ्य, 0, mask.तथ्य, 0, (पूर्णांक)src1.योग(), 1, src1.type(), normType);
    }

    अगर( normType & NORM_RELATIVE )
    {
        वापस norm(_src1, _src2, normType & ~NORM_RELATIVE, _mask)/(norm(_src2, normType, _mask) + द्विग_अंतर);
    }

    normType &= 7;
    CV_Assert( normType == NORM_INF || normType == NORM_L1 ||
               normType == NORM_L2 || normType == NORM_L2SQR ||
              ((normType == NORM_HAMMING || normType == NORM_HAMMING2) && src1.type() == CV_8U) );

    NormDiffFunc func = getNormDiffFunc(normType >> 1, depth);
    CV_Assert( (normType >> 1) >= 3 || func != 0 );

    अगर( src1.isContinuous() && src2.isContinuous() && mask.empty() )
    {
        माप_प्रकार len = src1.योग()*src1.channels();
        अगर( len == (माप_प्रकार)(पूर्णांक)len )
        {
            अगर( src1.depth() == CV_32F )
            {
                स्थिर uchar* data1 = src1.ptr<स्थिर uchar>();
                स्थिर uchar* data2 = src2.ptr<स्थिर uchar>();

                अगर( normType == NORM_L2 || normType == NORM_L2SQR || normType == NORM_L1 )
                {
                    द्विगुन परिणाम = 0;
                    func(data1, data2, 0, (uchar*)&परिणाम, (पूर्णांक)len, 1);
                    वापस normType == NORM_L2 ? std::वर्ग_मूल(परिणाम) : परिणाम;
                }
                अगर( normType == NORM_INF )
                {
                    भग्न परिणाम = 0;
                    func(data1, data2, 0, (uchar*)&परिणाम, (पूर्णांक)len, 1);
                    वापस परिणाम;
                }
            }
        }
    }

    CV_Assert( mask.empty() || mask.type() == CV_8U || mask.type() == CV_8S || mask.type() == CV_Bool );

    अगर( normType == NORM_HAMMING || normType == NORM_HAMMING2 )
    {
        अगर( !mask.empty() )
        {
            Mat अस्थायी_2;
            bitwise_xor(src1, src2, अस्थायी_2);
            bitwise_and(अस्थायी_2, mask, अस्थायी_2);
            वापस norm(अस्थायी_2, normType);
        }
        पूर्णांक cell_आकार = normType == NORM_HAMMING ? 1 : 2;

        स्थिर Mat* arrays[] = {&src1, &src2, 0};
        uchar* ptrs[2] = {};
        NAryMatIterator it(arrays, ptrs);
        पूर्णांक योग = (पूर्णांक)it.size;
        पूर्णांक परिणाम = 0;

        क्रम( माप_प्रकार i = 0; i < it.nplanes; i++, ++it )
        {
            परिणाम += hal::normHamming(ptrs[0], ptrs[1], योग, cell_आकार);
        }

        वापस परिणाम;
    }

    स्थिर Mat* arrays[] = {&src1, &src2, &mask, 0};
    uchar* ptrs[3] = {};
    जोड़
    {
        द्विगुन d;
        भग्न f;
        अचिन्हित u;
        uint64 UL;
    }
    परिणाम;
    परिणाम.d = 0;
    NAryMatIterator it(arrays, ptrs);
    CV_CheckLT((माप_प्रकार)it.size, (माप_प्रकार)पूर्णांक_उच्च, "");

    bool is_fp16 = depth == CV_16F || depth == CV_16BF;

    अगर ((normType == NORM_L1 && (depth <= CV_16S || depth == CV_Bool || is_fp16)) ||
        ((normType == NORM_L2 || normType == NORM_L2SQR) && (depth <= CV_8S || depth == CV_Bool || is_fp16)))
    {
        // special case to handle "integer" overflow in accumulator
        स्थिर माप_प्रकार esz = src1.elem_आकार();
        स्थिर पूर्णांक योग = (पूर्णांक)it.size;
        स्थिर पूर्णांक blockSize0 = (is_fp16 ? (1 << 10) :
            depth == CV_Bool ? (1 << 30) :
            normType == NORM_L1 && depth <= CV_8S ? (1 << 23) : (1 << 15))/cn;
        स्थिर पूर्णांक block_आकार = std::min(योग, blockSize0);
        जोड़ {
            पूर्णांक i;
            भग्न f;
        } blocksum;
        blocksum.i = 0;
        पूर्णांक गिनती_2 = 0;

        क्रम (माप_प्रकार i = 0; i < it.nplanes; i++, ++it)
        {
            क्रम (पूर्णांक j = 0; j < योग; j += block_आकार)
            {
                पूर्णांक bsz = std::min(योग - j, block_आकार);
                func(ptrs[0], ptrs[1], ptrs[2], (uchar*)&blocksum, bsz, cn);
                गिनती_2 += bsz;
                अगर (गिनती_2 + block_आकार >= blockSize0 || (i+1 >= it.nplanes && j+bsz >= योग))
                {
                    परिणाम.d += is_fp16 ? (द्विगुन)blocksum.f : (द्विगुन)blocksum.i;
                    blocksum.i = 0;
                    गिनती_2 = 0;
                }
                ptrs[0] += bsz*esz;
                ptrs[1] += bsz*esz;
                अगर (ptrs[2])
                    ptrs[2] += bsz;
            }
        }
    }
    अन्यथा
    {
        // generic implementation
        क्रम (माप_प्रकार i = 0; i < it.nplanes; i++, ++it)
        {
            func(ptrs[0], ptrs[1], ptrs[2], (uchar*)&परिणाम, (पूर्णांक)it.size, cn);
        }
    }

    अगर( normType == NORM_INF )
    {
        अगर (depth <= CV_32S || depth == CV_32U || depth == CV_Bool)
            वापस परिणाम.u;
        अगर (depth == CV_32F || is_fp16)
            वापस परिणाम.f;
        अगर (depth == CV_64U || depth == CV_64S)
            वापस (द्विगुन)परिणाम.UL;
    }
    अगर( normType == NORM_L2 )
        वापस std::वर्ग_मूल(परिणाम.d);

    वापस परिणाम.d;
}

cv::Hamming::परिणाम_Type Hamming::चालक()( स्थिर अचिन्हित अक्षर* a, स्थिर अचिन्हित अक्षर* b, पूर्णांक size ) स्थिर
{
    वापस cv::hal::normHamming(a, b, size);
}

द्विगुन PSNR(निवेश_Array _src1, निवेश_Array _src2, द्विगुन R)
{
    CV_INSTRUMENT_REGION();

    //Input arrays must have depth CV_8U
    CV_Assert( _src1.type() == _src2.type() );

    द्विगुन diff = std::वर्ग_मूल(norm(_src1, _src2, NORM_L2SQR)/(_src1.योग()*_src1.channels()));
    वापस 20*log10(R/(diff+द्विग_अंतर));
}


#अगर_घोषित HAVE_OPENCL
अटल bool ocl_normalize( निवेश_Array _src, निवेश_Output_Array _dst, निवेश_Array _mask, पूर्णांक dtype,
                           द्विगुन scale, द्विगुन delta )
{
    UMat src = _src.getUMat();

    अगर( _mask.empty() )
        src.convertTo( _dst, dtype, scale, delta );
    अन्यथा अगर (src.channels() <= 4)
    {
        स्थिर ocl::Device & dev = ocl::Device::getDefault();

        पूर्णांक stype = _src.type(), sdepth = CV_MAT_DEPTH(stype), cn = CV_MAT_CN(stype),
                ddepth = CV_MAT_DEPTH(dtype), wdepth = std::max(CV_32F, std::max(sdepth, ddepth)),
                rowsPerWI = dev.isIntel() ? 4 : 1;

        भग्न fscale = static_cast<भग्न>(scale), fdelta = static_cast<भग्न>(delta);
        bool haveScale = std::भ_असल(scale - 1) > द्विग_अंतर,
                haveZeroScale = !(std::भ_असल(scale) > द्विग_अंतर),
                haveDelta = std::भ_असल(delta) > द्विग_अंतर,
                doubleSupport = dev.doubleFPConfig() > 0;

        अगर (!haveScale && !haveDelta && stype == dtype)
        {
            _src.copyTo(_dst, _mask);
            वापस true;
        }
        अगर (haveZeroScale)
        {
            _dst.setTo(Scalar(delta), _mask);
            वापस true;
        }

        अगर ((sdepth == CV_64F || ddepth == CV_64F) && !doubleSupport)
            वापस false;

        अक्षर cvt[2][50];
        String opts = format("-D srcT=%s -D dstT=%s -D convertToWT=%s -D cn=%d -D rowsPerWI=%d"
                             " -D convertToDT=%s -D workT=%s%s%s%s -D srcT1=%s -D dstT1=%s",
                             ocl::typeToStr(stype), ocl::typeToStr(dtype),
                             ocl::convertTypeStr(sdepth, wdepth, cn, cvt[0], माप(cvt[0])), cn,
                             rowsPerWI, ocl::convertTypeStr(wdepth, ddepth, cn, cvt[1], माप(cvt[1])),
                             ocl::typeToStr(CV_MAKE_TYPE(wdepth, cn)),
                             doubleSupport ? " -D DOUBLE_SUPPORT" : "",
                             haveScale ? " -D HAVE_SCALE" : "",
                             haveDelta ? " -D HAVE_DELTA" : "",
                             ocl::typeToStr(sdepth), ocl::typeToStr(ddepth));

        ocl::Kernel k("normalizek", ocl::core::normalize_oclsrc, opts);
        अगर (k.empty())
            वापस false;

        UMat mask = _mask.getUMat(), dst = _dst.getUMat();

        ocl::KernelArg srcarg = ocl::KernelArg::Read_Only_No_आकार(src),
                maskarg = ocl::KernelArg::Read_Only_No_आकार(mask),
                dstarg = ocl::KernelArg::ReadWrite(dst);

        अगर (haveScale)
        {
            अगर (haveDelta)
                k.args(srcarg, maskarg, dstarg, fscale, fdelta);
            अन्यथा
                k.args(srcarg, maskarg, dstarg, fscale);
        }
        अन्यथा
        {
            अगर (haveDelta)
                k.args(srcarg, maskarg, dstarg, fdelta);
            अन्यथा
                k.args(srcarg, maskarg, dstarg);
        }

        माप_प्रकार globalsize[2] = { (माप_प्रकार)src.cols, ((माप_प्रकार)src.rows + rowsPerWI - 1) / rowsPerWI };
        वापस k.run(2, globalsize, शून्य, false);
    }
    अन्यथा
    {
        UMat अस्थायी_2;
        src.convertTo( अस्थायी_2, dtype, scale, delta );
        अस्थायी_2.copyTo( _dst, _mask );
    }

    वापस true;
}  // ocl_normalize
#पूर्ण_अगर  // HAVE_OPENCL

व्योम normalize(निवेश_Array _src, निवेश_Output_Array _dst, द्विगुन a, द्विगुन b,
               पूर्णांक norm_type, पूर्णांक rtype, निवेश_Array _mask)
{
    CV_INSTRUMENT_REGION();

    द्विगुन scale = 1, shift = 0;
    पूर्णांक type = _src.type(), depth = CV_MAT_DEPTH(type);

    अगर( rtype < 0 )
        rtype = _dst.fixedType() ? _dst.depth() : depth;

    अगर( norm_type == NORM_MINMAX )
    {
        द्विगुन smin = 0, smax = 0;
        द्विगुन dmin = MIN( a, b ), dmax = MAX( a, b );
        minMaxIdx( _src, &smin, &smax, 0, 0, _mask );
        scale = (dmax - dmin)*(smax - smin > द्विग_अंतर ? 1./(smax - smin) : 0);
        अगर( rtype == CV_32F )
        {
            scale = (भग्न)scale;
            shift = (भग्न)dmin - (भग्न)(smin*scale);
        }
        अन्यथा
            shift = dmin - smin*scale;
    }
    अन्यथा अगर( norm_type == NORM_L2 || norm_type == NORM_L1 || norm_type == NORM_INF )
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
    अगर( _mask.empty() )
        src.convertTo( _dst, rtype, scale, shift );
    अन्यथा
    {
        Mat अस्थायी_2;
        src.convertTo( अस्थायी_2, rtype, scale, shift );
        अस्थायी_2.copyTo( _dst, _mask );
    }
}

}  // namespace
