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

خارجي ثابت uchar راوباسه_شمېر_جدول[256] =
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

ساكن ثابت uchar راوباسه_شمېر_Table2[] =
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

ساكن ثابت uchar راوباسه_شمېر_Table4[] =
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


صحيح normHamming(ثابت uchar* a, صحيح n, صحيح cell_اندازه)
{
    صحيح output;
    CALL_HAL_RET(normHamming8u, cv_hal_normHamming8u, output, a, n, cell_اندازه);

    إذا( cell_اندازه == 1 )
        أرجع normHamming(a, n);
    ثابت uchar* tab = 0;
    إذا( cell_اندازه == 2 )
        tab = راوباسه_شمېر_Table2;
    وإلا إذا( cell_اندازه == 4 )
        tab = راوباسه_شمېر_Table4;
    وإلا
        أرجع -1;
    صحيح i = 0;
    صحيح پایله = 0;
#إذا (CV_SIMD || CV_SIMD_SCALABLE)
    v_uint64 t = vx_setzero_u64();
    إذا ( cell_اندازه == 2)
    {
        v_uint16 ماسک = v_reinterpret_as_u16(vx_setall_u8(0x55));
        لكل(; i <= n - VTraits<v_uint8>::vlanes(); i += VTraits<v_uint8>::vlanes())
        {
            v_uint16 a0 = v_reinterpret_as_u16(vx_بارول(a + i));
            t = v_جمع_کړه(t, v_popcount(v_reinterpret_as_u64(v_and(v_or(a0, v_shr<1>(a0)), ماسک))));
        }
    }
    وإلا    // cellSize == 4
    {
        v_uint16 ماسک = v_reinterpret_as_u16(vx_setall_u8(0x11));
        لكل(; i <= n - VTraits<v_uint8>::vlanes(); i += VTraits<v_uint8>::vlanes())
        {
            v_uint16 a0 = v_reinterpret_as_u16(vx_بارول(a + i));
            v_uint16 a1 = v_or(a0, v_shr<2>(a0));
            t = v_جمع_کړه(t, v_popcount(v_reinterpret_as_u64(v_and(v_or(a1, v_shr<1>(a1)), ماسک))));

        }
    }
    پایله += (صحيح)v_reduce_مجموعه(t);
    vx_cleanup();
#وإلاإذا CV_ENABLE_UNROLLED
    لكل( ; i <= n - 4; i += 4 )
        پایله += tab[a[i]] + tab[a[i+1]] + tab[a[i+2]] + tab[a[i+3]];
#د_که_پای
    لكل( ; i < n; i++ )
        پایله += tab[a[i]];
    أرجع پایله;
}

صحيح normHamming(ثابت uchar* a, ثابت uchar* b, صحيح n, صحيح cell_اندازه)
{
    صحيح output;
    CALL_HAL_RET(normHammingDiff8u, cv_hal_normHammingDiff8u, output, a, b, n, cell_اندازه);

    إذا( cell_اندازه == 1 )
        أرجع normHamming(a, b, n);
    ثابت uchar* tab = 0;
    إذا( cell_اندازه == 2 )
        tab = راوباسه_شمېر_Table2;
    وإلا إذا( cell_اندازه == 4 )
        tab = راوباسه_شمېر_Table4;
    وإلا
        أرجع -1;
    صحيح i = 0;
    صحيح پایله = 0;
#إذا (CV_SIMD || CV_SIMD_SCALABLE)
    v_uint64 t = vx_setzero_u64();
    إذا ( cell_اندازه == 2)
    {
        v_uint16 ماسک = v_reinterpret_as_u16(vx_setall_u8(0x55));
        لكل(; i <= n - VTraits<v_uint8>::vlanes(); i += VTraits<v_uint8>::vlanes())
        {
            v_uint16 ab0 = v_reinterpret_as_u16(v_xor(vx_بارول(a + i), vx_بارول(b + i)));
            t = v_جمع_کړه(t, v_popcount(v_reinterpret_as_u64(v_and(v_or(ab0, v_shr<1>(ab0)), ماسک))));
        }
    }
    وإلا    // cellSize == 4
    {
        v_uint16 ماسک = v_reinterpret_as_u16(vx_setall_u8(0x11));
        لكل(; i <= n - VTraits<v_uint8>::vlanes(); i += VTraits<v_uint8>::vlanes())
        {
            v_uint16 ab0 = v_reinterpret_as_u16(v_xor(vx_بارول(a + i), vx_بارول(b + i)));
            v_uint16 ab1 = v_or(ab0, v_shr<2>(ab0));
            t = v_جمع_کړه(t, v_popcount(v_reinterpret_as_u64(v_and(v_or(ab1, v_shr<1>(ab1)), ماسک))));
        }
    }
    پایله += (صحيح)v_reduce_مجموعه(t);
    vx_cleanup();
#وإلاإذا CV_ENABLE_UNROLLED
    لكل( ; i <= n - 4; i += 4 )
        پایله += tab[a[i] ^ b[i]] + tab[a[i+1] ^ b[i+1]] +
                tab[a[i+2] ^ b[i+2]] + tab[a[i+3] ^ b[i+3]];
#د_که_پای
    لكل( ; i < n; i++ )
        پایله += tab[a[i] ^ b[i]];
    أرجع پایله;
}

عائم normL2Sqr_(ثابت عائم* a, ثابت عائم* b, صحيح n)
{
    صحيح j = 0; عائم d = 0.f;
#إذا (CV_SIMD || CV_SIMD_SCALABLE)
    ثابت صحيح vl = VTraits<v_float32>::vlanes();
    v_float32 v_d0 = vx_setzero_f32(), v_d1 = vx_setzero_f32();
    v_float32 v_d2 = vx_setzero_f32(), v_d3 = vx_setzero_f32();
    لكل (; j <= n - 4 * vl; j += 4 * vl)
    {
        v_float32 t0 = v_sub(vx_بارول(a + j), vx_بارول(b + j));
        v_float32 t1 = v_sub(vx_بارول(a + j + vl), vx_بارول(b + j + vl));
        v_d0 = v_muladd(t0, t0, v_d0);
        v_float32 t2 = v_sub(vx_بارول(a + j + 2 * vl), vx_بارول(b + j + 2 * vl));
        v_d1 = v_muladd(t1, t1, v_d1);
        v_float32 t3 = v_sub(vx_بارول(a + j + 3 * vl), vx_بارول(b + j + 3 * vl));
        v_d2 = v_muladd(t2, t2, v_d2);
        v_d3 = v_muladd(t3, t3, v_d3);
    }
    v_d0 = v_جمع_کړه(v_جمع_کړه(v_d0, v_d1), v_جمع_کړه(v_d2, v_d3));
    لكل (; j <= n - vl; j += vl)
    {
        v_float32 t0 = v_sub(vx_بارول(a + j), vx_بارول(b + j));
        v_d0 = v_muladd(t0, t0, v_d0);
    }
    d = v_reduce_مجموعه(v_d0);
#د_که_پای
    لكل( ; j < n; j++ )
    {
        عائم t = a[j] - b[j];
        d += t*t;
    }
    أرجع d;
}


عائم normL1_(ثابت عائم* a, ثابت عائم* b, صحيح n)
{
    صحيح j = 0; عائم d = 0.f;
#إذا (CV_SIMD || CV_SIMD_SCALABLE)
    ثابت صحيح vl = VTraits<v_float32>::vlanes();
    v_float32 v_d0 = vx_setzero_f32(), v_d1 = vx_setzero_f32();
    v_float32 v_d2 = vx_setzero_f32(), v_d3 = vx_setzero_f32();
    لكل (; j <= n - 4 * vl; j += 4 * vl)
    {
        v_d0 = v_جمع_کړه(v_d0, v_absdiff(vx_بارول(a + j), vx_بارول(b + j)));
        v_d1 = v_جمع_کړه(v_d1, v_absdiff(vx_بارول(a + j + vl), vx_بارول(b + j + vl)));
        v_d2 = v_جمع_کړه(v_d2, v_absdiff(vx_بارول(a + j + 2 * vl), vx_بارول(b + j + 2 * vl)));
        v_d3 = v_جمع_کړه(v_d3, v_absdiff(vx_بارول(a + j + 3 * vl), vx_بارول(b + j + 3 * vl)));
    }
    v_d0 = v_جمع_کړه(v_جمع_کړه(v_d0, v_d1), v_جمع_کړه(v_d2, v_d3));
    لكل (; j <= n - vl; j += vl)
        v_d0 = v_جمع_کړه(v_d0, v_absdiff(vx_بارول(a + j), vx_بارول(b + j)));
    d = v_reduce_مجموعه(v_d0);
#د_که_پای
    لكل( ; j < n; j++ )
        d += std::مطلق_قیمت(a[j] - b[j]);
    أرجع d;
}

صحيح normL1_(ثابت uchar* a, ثابت uchar* b, صحيح n)
{
    صحيح j = 0, d = 0;
#إذا (CV_SIMD || CV_SIMD_SCALABLE)
    ثابت صحيح vl = VTraits<v_uint8>::vlanes();
    لكل (; j <= n - 4 * vl; j += 4 * vl)
        d += v_reduce_sad(vx_بارول(a + j), vx_بارول(b + j)) +
             v_reduce_sad(vx_بارول(a + j + vl), vx_بارول(b + j + vl)) +
             v_reduce_sad(vx_بارول(a + j + 2 * vl), vx_بارول(b + j + 2 * vl)) +
             v_reduce_sad(vx_بارول(a + j + 3 * vl), vx_بارول(b + j + 3 * vl));
    لكل (; j <= n - vl; j += vl)
        d += v_reduce_sad(vx_بارول(a + j), vx_بارول(b + j));
#د_که_پای
    لكل( ; j < n; j++ )
        d += std::مطلق_قیمت(a[j] - b[j]);
    أرجع d;
}

} //cv::hal

//==================================================================================================

تعريفنوع صحيح (*NormFunc)(ثابت uchar*, ثابت uchar*, uchar*, صحيح, صحيح);
تعريفنوع صحيح (*NormDiffFunc)(ثابت uchar*, ثابت uchar*, ثابت uchar*, uchar*, صحيح, صحيح);

#که_تعریف_شوی HAVE_OPENCL

ساكن منطقي ocl_norm( ننوت_Array _src, صحيح normType, ننوت_Array ماسک_2, مزدوج & پایله )
{
    ثابت ocl::Device & d = ocl::Device::getDefault();

#که_تعریف_شوی __ANDROID__
    إذا (d.isNVidia())
        أرجع خطأ;
#د_که_پای
    ثابت صحيح cn = _src.channels();
    إذا (cn > 4)
        أرجع خطأ;
    صحيح type = _src.type(), ژوروالی = CV_MAT_ژوروالی(type);
    منطقي doubleSupport = d.doubleFPConfig() > 0,
            have_ماسک = ماسک_2.ډول() != ننوت_Array_2::NONE;

    إذا (ژوروالی >= CV_16F)
        أرجع خطأ;  // TODO: support FP16

    إذا ( !(normType == NORM_INF || normType == NORM_L1 || normType == NORM_L2 || normType == NORM_L2SQR) ||
         (!doubleSupport && ژوروالی == CV_64F))
        أرجع خطأ;

    UMat src = _src.getUMat();

    إذا (normType == NORM_INF)
    {
        إذا (!ocl_minMaxIdx(_src, عدم, &پایله, عدم, عدم, ماسک_2,
                           std::max(ژوروالی, CV_32S), ژوروالی != CV_8U && ژوروالی != CV_16U))
            أرجع خطأ;
    }
    وإلا إذا (normType == NORM_L1 || normType == NORM_L2 || normType == NORM_L2SQR)
    {
        Scalar sc;
        منطقي unstype = ژوروالی == CV_8U || ژوروالی == CV_16U;

        إذا ( !ocl_مجموعه(have_ماسک ? src : src.reshape(1), sc, normType == NORM_L2 || normType == NORM_L2SQR ?
                    OCL_OP_مجموعه_SQR : (unstype ? OCL_OP_مجموعه : OCL_OP_مجموعه_ABS), ماسک_2) )
            أرجع خطأ;

        مزدوج s = 0.0;
        لكل (صحيح i = 0; i < (have_ماسک ? cn : 1); ++i)
            s += sc[i];

        پایله = normType == NORM_L1 || normType == NORM_L2SQR ? s : std::جذر(s);
    }

    أرجع ریښتیا;
}

#د_که_پای

ساكن NormFunc getNormFunc(صحيح normType, صحيح ژوروالی) {
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getNormFunc, (normType, ژوروالی), CV_CPU_DISPATCH_MODES_ALL);
}
ساكن NormDiffFunc getNormDiffFunc(صحيح normType, صحيح ژوروالی) {
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getNormDiffFunc, (normType, ژوروالی), CV_CPU_DISPATCH_MODES_ALL);
}

مزدوج norm( ننوت_Array _src, صحيح normType, ننوت_Array ماسک_2 )
{
    CV_INSTRUMENT_REGION();

    normType &= NORM_TYPE_ماسک;
    CV_Assert( normType == NORM_INF || normType == NORM_L1 ||
               normType == NORM_L2 || normType == NORM_L2SQR ||
               ((normType == NORM_HAMMING || normType == NORM_HAMMING2) && _src.type() == CV_8U) );

#إذا defined HAVE_OPENCL
    مزدوج پایله_2 = 0;
#د_که_پای

#که_تعریف_شوی HAVE_OPENCL
    CV_OCL_پرلپسې_2(OCL_PERFORMANCE_CHECK(_src.isUMat()) && _src.dims() <= 2,
                ocl_norm(_src, normType, ماسک_2, پایله_2),
                پایله_2)
#د_که_پای

    Mat src = _src.getMat(), ماسک = ماسک_2.getMat();
    CV_Assert( ماسک.empty() || ((ماسک.type() == CV_8U || ماسک.type() == CV_8S || ماسک.type() == CV_Bool) && src.size == ماسک.size) );
    إذا( src.empty() )
        أرجع 0.0;
    صحيح ژوروالی = src.ژوروالی(), cn = src.channels();
    إذا( src.dims <= 2 )
    {
        مزدوج پایله;
        CALL_HAL_RET(norm, cv_hal_norm, پایله, src.معلومات, src.ګام, ماسک.معلومات, ماسک.ګام, src.ستون_شمېر, src.لیکې, src.type(), normType);
    }
    وإلا إذا( src.isContinuous() && ماسک.isContinuous() )
    {
        مزدوج پایله;
        CALL_HAL_RET(norm, cv_hal_norm, پایله, src.معلومات, 0, ماسک.معلومات, 0, (صحيح)src.ټول(), 1, src.type(), normType);
    }

    NormFunc func = getNormFunc(normType >> 1, ژوروالی);
    CV_Assert( (normType >> 1) >= 3 || func != 0 );

    إذا( src.isContinuous() && ماسک.empty() )
    {
        size_t len = src.ټول()*cn;
        إذا( len == (size_t)(صحيح)len )
        {
            إذا( ژوروالی == CV_32F )
            {
                ثابت uchar* معلومات = src.ptr<ثابت uchar>();

                إذا( normType == NORM_L2 || normType == NORM_L2SQR || normType == NORM_L1 )
                {
                    مزدوج پایله = 0;
                    func(معلومات, 0, (uchar*)&پایله, (صحيح)len, 1);
                    أرجع normType == NORM_L2 ? std::جذر(پایله) : پایله;
                }
                إذا( normType == NORM_INF )
                {
                    عائم پایله = 0;
                    func(معلومات, 0, (uchar*)&پایله, (صحيح)len, 1);
                    أرجع پایله;
                }
            }
            إذا( ژوروالی == CV_8U )
            {
                ثابت uchar* معلومات = src.ptr<uchar>();

                إذا( normType == NORM_HAMMING )
                {
                    أرجع hal::normHamming(معلومات, (صحيح)len, 1);
                }

                إذا( normType == NORM_HAMMING2 )
                {
                    أرجع hal::normHamming(معلومات, (صحيح)len, 2);
                }
            }
        }
    }

    CV_Assert( ماسک.empty() || ماسک.type() == CV_8U || ماسک.type() == CV_8S || ماسک.type() == CV_Bool );

    إذا( normType == NORM_HAMMING || normType == NORM_HAMMING2 )
    {
        إذا( !ماسک.empty() )
        {
            Mat لنډمهاله_2;
            bitwise_and(src, ماسک, لنډمهاله_2);
            أرجع norm(لنډمهاله_2, normType);
        }
        صحيح cell_اندازه = normType == NORM_HAMMING ? 1 : 2;

        ثابت Mat* arrays[] = {&src, 0};
        uchar* ptrs[1] = {};
        NAryMatIterator it(arrays, ptrs);
        صحيح ټول = (صحيح)it.size;
        صحيح پایله = 0;

        لكل( size_t i = 0; i < it.nplanes; i++, ++it )
        {
            پایله += hal::normHamming(ptrs[0], ټول, cell_اندازه);
        }

        أرجع پایله;
    }

    ثابت Mat* arrays[] = {&src, &ماسک, 0};
    uchar* ptrs[2] = {};
    اتحاد
    {
        مزدوج d;
        بینښه u;
        uint64 UL;
        عائم f;
    }
    پایله;
    پایله.d = 0;
    NAryMatIterator it(arrays, ptrs);
    CV_CheckLT((size_t)it.size, (size_t)لوی_عدد, "");
    منطقي is_fp16 = ژوروالی == CV_16F || ژوروالی == CV_16BF;

    إذا ((normType == NORM_L1 && (ژوروالی <= CV_16S || ژوروالی == CV_Bool || is_fp16)) ||
        ((normType == NORM_L2 || normType == NORM_L2SQR) && (ژوروالی <= CV_8S || ژوروالی == CV_Bool || is_fp16)))
    {
        // special case to handle "integer" overflow in accumulator
        ثابت size_t esz = src.elem_اندازه();
        ثابت صحيح ټول = (صحيح)it.size;
        ثابت صحيح بلاک_Size0 = (is_fp16 ? (1 << 10) :
            ژوروالی == CV_Bool ? (1 << 30) :
            normType == NORM_L1 && ژوروالی <= CV_8S ? (1 << 23) : (1 << 15))/cn;
        ثابت صحيح بلاک_اندازه_2 = std::min(ټول, بلاک_Size0);
        اتحاد {
            صحيح i;
            عائم f;
        } blocksum;
        blocksum.i = 0;
        صحيح شمېر_2 = 0;

        لكل (size_t i = 0; i < it.nplanes; i++, ++it)
        {
            لكل (صحيح j = 0; j < ټول; j += بلاک_اندازه_2)
            {
                صحيح bsz = std::min(ټول - j, بلاک_اندازه_2);
                func(ptrs[0], ptrs[1], (uchar*)&blocksum, bsz, cn);
                شمېر_2 += bsz;
                إذا (شمېر_2 + بلاک_اندازه_2 >= بلاک_Size0 || (i+1 >= it.nplanes && j+bsz >= ټول))
                {
                    پایله.d += is_fp16 ? (مزدوج)blocksum.f : (مزدوج)blocksum.i;
                    blocksum.i = 0;
                    شمېر_2 = 0;
                }
                ptrs[0] += bsz*esz;
                إذا (ptrs[1])
                    ptrs[1] += bsz;
            }
        }
    }
    وإلا
    {
        // generic implementation
        لكل (size_t i = 0; i < it.nplanes; i++, ++it)
        {
            func(ptrs[0], ptrs[1], (uchar*)&پایله, (صحيح)it.size, cn);
        }
    }

    إذا( normType == NORM_INF )
    {
        إذا(ژوروالی <= CV_32S || ژوروالی == CV_32U || ژوروالی == CV_Bool)
            أرجع پایله.u;
        إذا (ژوروالی == CV_32F || is_fp16)
            أرجع پایله.f;
        إذا (ژوروالی == CV_64U || ژوروالی == CV_64S)
            أرجع (مزدوج)پایله.UL;
    }
    إذا( normType == NORM_L2 )
        أرجع std::جذر(پایله.d);

    أرجع پایله.d;
}

//==================================================================================================

#که_تعریف_شوی HAVE_OPENCL
ساكن منطقي ocl_norm( ننوت_Array _src1, ننوت_Array _src2, صحيح normType, ننوت_Array ماسک_2, مزدوج & پایله )
{
#که_تعریف_شوی __ANDROID__
    إذا (ocl::Device::getDefault().isNVidia())
        أرجع خطأ;
#د_که_پای

    Scalar sc1, sc2;
    صحيح cn = _src1.channels();
    إذا (cn > 4)
        أرجع خطأ;
    صحيح type = _src1.type(), ژوروالی = CV_MAT_ژوروالی(type);
    منطقي relative = (normType & NORM_RELATIVE) != 0;
    normType &= ~NORM_RELATIVE;
    منطقي normsum = normType == NORM_L1 || normType == NORM_L2 || normType == NORM_L2SQR;

#که_تعریف_شوی __APPLE__
    إذا(normType == NORM_L1 && type == CV_16UC3 && !ماسک_2.empty())
        أرجع خطأ;
#د_که_پای

    إذا (normsum)
    {
        إذا (!ocl_مجموعه(_src1, sc1, normType == NORM_L2 || normType == NORM_L2SQR ?
                     OCL_OP_مجموعه_SQR : OCL_OP_مجموعه, ماسک_2, _src2, relative, sc2))
            أرجع خطأ;
    }
    وإلا
    {
        إذا (!ocl_minMaxIdx(_src1, عدم, &sc1[0], عدم, عدم, ماسک_2, std::max(CV_32S, ژوروالی),
                           خطأ, _src2, relative ? &sc2[0] : عدم))
            أرجع خطأ;
        cn = 1;
    }

    مزدوج s2 = 0;
    لكل (صحيح i = 0; i < cn; ++i)
    {
        پایله += sc1[i];
        إذا (relative)
            s2 += sc2[i];
    }

    إذا (normType == NORM_L2)
    {
        پایله = std::جذر(پایله);
        إذا (relative)
            s2 = std::جذر(s2);
    }

    إذا (relative)
        پایله /= (s2 + DBL_EPSILON);

    أرجع ریښتیا;
}  // ocl_norm()
#د_که_پای  // HAVE_OPENCL

مزدوج norm( ننوت_Array _src1, ننوت_Array _src2, صحيح normType, ننوت_Array ماسک_2 )
{
    CV_INSTRUMENT_REGION();

    CV_CheckTypeEQ(_src1.type(), _src2.type(), "Input type mismatch");
    CV_Assert(_src1.same_اندازه(_src2));

#إذا defined HAVE_OPENCL
    مزدوج پایله_2 = 0;
#د_که_پای

#که_تعریف_شوی HAVE_OPENCL
    CV_OCL_پرلپسې_2(OCL_PERFORMANCE_CHECK(_src1.isUMat()),
                ocl_norm(_src1, _src2, normType, ماسک_2, پایله_2),
                پایله_2)
#د_که_پای

    Mat src1 = _src1.getMat(), src2 = _src2.getMat(), ماسک = ماسک_2.getMat();
    CV_Assert( ماسک.empty() || ((ماسک.type() == CV_8U || ماسک.type() == CV_8S || ماسک.type() == CV_Bool) && src1.size == ماسک.size) );
    إذا( src1.empty() || src2.empty() )
        أرجع 0.0;
    صحيح ژوروالی = src1.ژوروالی(), cn = src1.channels();
    إذا( src1.dims <= 2 )
    {
        مزدوج پایله;
        CALL_HAL_RET(normDiff, cv_hal_normDiff, پایله, src1.معلومات, src1.ګام, src2.معلومات, src2.ګام, ماسک.معلومات, ماسک.ګام, src1.ستون_شمېر, src1.لیکې, src1.type(), normType);
    }
    وإلا إذا( src1.isContinuous() && src2.isContinuous() && ماسک.isContinuous() )
    {
        مزدوج پایله;
        CALL_HAL_RET(normDiff, cv_hal_normDiff, پایله, src1.معلومات, 0, src2.معلومات, 0, ماسک.معلومات, 0, (صحيح)src1.ټول(), 1, src1.type(), normType);
    }

    إذا( normType & NORM_RELATIVE )
    {
        أرجع norm(_src1, _src2, normType & ~NORM_RELATIVE, ماسک_2)/(norm(_src2, normType, ماسک_2) + DBL_EPSILON);
    }

    normType &= 7;
    CV_Assert( normType == NORM_INF || normType == NORM_L1 ||
               normType == NORM_L2 || normType == NORM_L2SQR ||
              ((normType == NORM_HAMMING || normType == NORM_HAMMING2) && src1.type() == CV_8U) );

    NormDiffFunc func = getNormDiffFunc(normType >> 1, ژوروالی);
    CV_Assert( (normType >> 1) >= 3 || func != 0 );

    إذا( src1.isContinuous() && src2.isContinuous() && ماسک.empty() )
    {
        size_t len = src1.ټول()*src1.channels();
        إذا( len == (size_t)(صحيح)len )
        {
            إذا( src1.ژوروالی() == CV_32F )
            {
                ثابت uchar* data1 = src1.ptr<ثابت uchar>();
                ثابت uchar* data2 = src2.ptr<ثابت uchar>();

                إذا( normType == NORM_L2 || normType == NORM_L2SQR || normType == NORM_L1 )
                {
                    مزدوج پایله = 0;
                    func(data1, data2, 0, (uchar*)&پایله, (صحيح)len, 1);
                    أرجع normType == NORM_L2 ? std::جذر(پایله) : پایله;
                }
                إذا( normType == NORM_INF )
                {
                    عائم پایله = 0;
                    func(data1, data2, 0, (uchar*)&پایله, (صحيح)len, 1);
                    أرجع پایله;
                }
            }
        }
    }

    CV_Assert( ماسک.empty() || ماسک.type() == CV_8U || ماسک.type() == CV_8S || ماسک.type() == CV_Bool );

    إذا( normType == NORM_HAMMING || normType == NORM_HAMMING2 )
    {
        إذا( !ماسک.empty() )
        {
            Mat لنډمهاله_2;
            bitwise_xor(src1, src2, لنډمهاله_2);
            bitwise_and(لنډمهاله_2, ماسک, لنډمهاله_2);
            أرجع norm(لنډمهاله_2, normType);
        }
        صحيح cell_اندازه = normType == NORM_HAMMING ? 1 : 2;

        ثابت Mat* arrays[] = {&src1, &src2, 0};
        uchar* ptrs[2] = {};
        NAryMatIterator it(arrays, ptrs);
        صحيح ټول = (صحيح)it.size;
        صحيح پایله = 0;

        لكل( size_t i = 0; i < it.nplanes; i++, ++it )
        {
            پایله += hal::normHamming(ptrs[0], ptrs[1], ټول, cell_اندازه);
        }

        أرجع پایله;
    }

    ثابت Mat* arrays[] = {&src1, &src2, &ماسک, 0};
    uchar* ptrs[3] = {};
    اتحاد
    {
        مزدوج d;
        عائم f;
        بینښه u;
        uint64 UL;
    }
    پایله;
    پایله.d = 0;
    NAryMatIterator it(arrays, ptrs);
    CV_CheckLT((size_t)it.size, (size_t)لوی_عدد, "");

    منطقي is_fp16 = ژوروالی == CV_16F || ژوروالی == CV_16BF;

    إذا ((normType == NORM_L1 && (ژوروالی <= CV_16S || ژوروالی == CV_Bool || is_fp16)) ||
        ((normType == NORM_L2 || normType == NORM_L2SQR) && (ژوروالی <= CV_8S || ژوروالی == CV_Bool || is_fp16)))
    {
        // special case to handle "integer" overflow in accumulator
        ثابت size_t esz = src1.elem_اندازه();
        ثابت صحيح ټول = (صحيح)it.size;
        ثابت صحيح بلاک_Size0 = (is_fp16 ? (1 << 10) :
            ژوروالی == CV_Bool ? (1 << 30) :
            normType == NORM_L1 && ژوروالی <= CV_8S ? (1 << 23) : (1 << 15))/cn;
        ثابت صحيح بلاک_اندازه_2 = std::min(ټول, بلاک_Size0);
        اتحاد {
            صحيح i;
            عائم f;
        } blocksum;
        blocksum.i = 0;
        صحيح شمېر_2 = 0;

        لكل (size_t i = 0; i < it.nplanes; i++, ++it)
        {
            لكل (صحيح j = 0; j < ټول; j += بلاک_اندازه_2)
            {
                صحيح bsz = std::min(ټول - j, بلاک_اندازه_2);
                func(ptrs[0], ptrs[1], ptrs[2], (uchar*)&blocksum, bsz, cn);
                شمېر_2 += bsz;
                إذا (شمېر_2 + بلاک_اندازه_2 >= بلاک_Size0 || (i+1 >= it.nplanes && j+bsz >= ټول))
                {
                    پایله.d += is_fp16 ? (مزدوج)blocksum.f : (مزدوج)blocksum.i;
                    blocksum.i = 0;
                    شمېر_2 = 0;
                }
                ptrs[0] += bsz*esz;
                ptrs[1] += bsz*esz;
                إذا (ptrs[2])
                    ptrs[2] += bsz;
            }
        }
    }
    وإلا
    {
        // generic implementation
        لكل (size_t i = 0; i < it.nplanes; i++, ++it)
        {
            func(ptrs[0], ptrs[1], ptrs[2], (uchar*)&پایله, (صحيح)it.size, cn);
        }
    }

    إذا( normType == NORM_INF )
    {
        إذا (ژوروالی <= CV_32S || ژوروالی == CV_32U || ژوروالی == CV_Bool)
            أرجع پایله.u;
        إذا (ژوروالی == CV_32F || is_fp16)
            أرجع پایله.f;
        إذا (ژوروالی == CV_64U || ژوروالی == CV_64S)
            أرجع (مزدوج)پایله.UL;
    }
    إذا( normType == NORM_L2 )
        أرجع std::جذر(پایله.d);

    أرجع پایله.d;
}

cv::Hamming::پایله_Type Hamming::چلوونکی()( ثابت بینښه حرف* a, ثابت بینښه حرف* b, صحيح size ) ثابت
{
    أرجع cv::hal::normHamming(a, b, size);
}

مزدوج PSNR(ننوت_Array _src1, ننوت_Array _src2, مزدوج R)
{
    CV_INSTRUMENT_REGION();

    //Input arrays must have depth CV_8U
    CV_Assert( _src1.type() == _src2.type() );

    مزدوج diff = std::جذر(norm(_src1, _src2, NORM_L2SQR)/(_src1.ټول()*_src1.channels()));
    أرجع 20*log10(R/(diff+DBL_EPSILON));
}


#که_تعریف_شوی HAVE_OPENCL
ساكن منطقي ocl_normalize( ننوت_Array _src, ننوت_Output_Array _dst, ننوت_Array ماسک_2, صحيح dtype,
                           مزدوج scale, مزدوج delta )
{
    UMat src = _src.getUMat();

    إذا( ماسک_2.empty() )
        src.convert_منزل( _dst, dtype, scale, delta );
    وإلا إذا (src.channels() <= 4)
    {
        ثابت ocl::Device & dev = ocl::Device::getDefault();

        صحيح stype = _src.type(), sdepth = CV_MAT_ژوروالی(stype), cn = CV_MAT_CN(stype),
                ddepth = CV_MAT_ژوروالی(dtype), wdepth = std::max(CV_32F, std::max(sdepth, ddepth)),
                لیکې_فی_WI = dev.isIntel() ? 4 : 1;

        عائم fscale = static_cast<عائم>(scale), fdelta = static_cast<عائم>(delta);
        منطقي haveScale = std::fabs(scale - 1) > DBL_EPSILON,
                haveZeroScale = !(std::fabs(scale) > DBL_EPSILON),
                haveDelta = std::fabs(delta) > DBL_EPSILON,
                doubleSupport = dev.doubleFPConfig() > 0;

        إذا (!haveScale && !haveDelta && stype == dtype)
        {
            _src.copy_منزل(_dst, ماسک_2);
            أرجع ریښتیا;
        }
        إذا (haveZeroScale)
        {
            _dst.سېټ_منزل(Scalar(delta), ماسک_2);
            أرجع ریښتیا;
        }

        إذا ((sdepth == CV_64F || ddepth == CV_64F) && !doubleSupport)
            أرجع خطأ;

        حرف cvt[2][50];
        String opts = format("-D srcT=%s -D dstT=%s -D convertToWT=%s -D cn=%d -D rowsPerWI=%d"
                             " -D convertToDT=%s -D workT=%s%s%s%s -D srcT1=%s -D dstT1=%s",
                             ocl::type_منزل_Str(stype), ocl::type_منزل_Str(dtype),
                             ocl::convertTypeStr(sdepth, wdepth, cn, cvt[0], حجم(cvt[0])), cn,
                             لیکې_فی_WI, ocl::convertTypeStr(wdepth, ddepth, cn, cvt[1], حجم(cvt[1])),
                             ocl::type_منزل_Str(CV_MAKE_TYPE(wdepth, cn)),
                             doubleSupport ? " -D DOUBLE_SUPPORT" : "",
                             haveScale ? " -D HAVE_SCALE" : "",
                             haveDelta ? " -D HAVE_DELTA" : "",
                             ocl::type_منزل_Str(sdepth), ocl::type_منزل_Str(ddepth));

        ocl::Kernel k("normalizek", ocl::core::normalize_oclsrc, opts);
        إذا (k.empty())
            أرجع خطأ;

        UMat ماسک = ماسک_2.getUMat(), dst = _dst.getUMat();

        ocl::KernelArg srcarg = ocl::KernelArg::Read_Only_No_اندازه(src),
                maskarg = ocl::KernelArg::Read_Only_No_اندازه(ماسک),
                dstarg = ocl::KernelArg::ReadWrite(dst);

        إذا (haveScale)
        {
            إذا (haveDelta)
                k.دلیلونه(srcarg, maskarg, dstarg, fscale, fdelta);
            وإلا
                k.دلیلونه(srcarg, maskarg, dstarg, fscale);
        }
        وإلا
        {
            إذا (haveDelta)
                k.دلیلونه(srcarg, maskarg, dstarg, fdelta);
            وإلا
                k.دلیلونه(srcarg, maskarg, dstarg);
        }

        size_t globalsize[2] = { (size_t)src.ستون_شمېر, ((size_t)src.لیکې + لیکې_فی_WI - 1) / لیکې_فی_WI };
        أرجع k.پرلپسې(2, globalsize, عدم, خطأ);
    }
    وإلا
    {
        UMat لنډمهاله_2;
        src.convert_منزل( لنډمهاله_2, dtype, scale, delta );
        لنډمهاله_2.copy_منزل( _dst, ماسک_2 );
    }

    أرجع ریښتیا;
}  // ocl_normalize
#د_که_پای  // HAVE_OPENCL

فراغ normalize(ننوت_Array _src, ننوت_Output_Array _dst, مزدوج a, مزدوج b,
               صحيح norm_type, صحيح rtype, ننوت_Array ماسک_2)
{
    CV_INSTRUMENT_REGION();

    مزدوج scale = 1, ښويدنه = 0;
    صحيح type = _src.type(), ژوروالی = CV_MAT_ژوروالی(type);

    إذا( rtype < 0 )
        rtype = _dst.fixedType() ? _dst.ژوروالی() : ژوروالی;

    إذا( norm_type == NORM_MINMAX )
    {
        مزدوج smin = 0, smax = 0;
        مزدوج dmin = MIN( a, b ), dmax = MAX( a, b );
        minMaxIdx( _src, &smin, &smax, 0, 0, ماسک_2 );
        scale = (dmax - dmin)*(smax - smin > DBL_EPSILON ? 1./(smax - smin) : 0);
        إذا( rtype == CV_32F )
        {
            scale = (عائم)scale;
            ښويدنه = (عائم)dmin - (عائم)(smin*scale);
        }
        وإلا
            ښويدنه = dmin - smin*scale;
    }
    وإلا إذا( norm_type == NORM_L2 || norm_type == NORM_L1 || norm_type == NORM_INF )
    {
        scale = norm( _src, norm_type, ماسک_2 );
        scale = scale > DBL_EPSILON ? a/scale : 0.;
        ښويدنه = 0;
    }
    وإلا
        CV_Error( cv::Error::StsBadArg, "Unknown/unsupported norm type" );

    CV_OCL_پرلپسې(_dst.isUMat(),
               ocl_normalize(_src, _dst, ماسک_2, rtype, scale, ښويدنه))

    Mat src = _src.getMat();
    إذا( ماسک_2.empty() )
        src.convert_منزل( _dst, rtype, scale, ښويدنه );
    وإلا
    {
        Mat لنډمهاله_2;
        src.convert_منزل( لنډمهاله_2, rtype, scale, ښويدنه );
        لنډمهاله_2.copy_منزل( _dst, ماسک_2 );
    }
}

}  // namespace
