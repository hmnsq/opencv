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

خارجي ثابت uchar ڪڍو_ڳڻپ_جدول[256] =
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

ساكن ثابت uchar ڪڍو_ڳڻپ_Table2[] =
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

ساكن ثابت uchar ڪڍو_ڳڻپ_Table4[] =
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


صحيح normHamming(ثابت uchar* a, صحيح n, صحيح cell_سائيز)
{
    صحيح output;
    CALL_HAL_RET(normHamming8u, cv_hal_normHamming8u, output, a, n, cell_سائيز);

    إذا( cell_سائيز == 1 )
        أرجع normHamming(a, n);
    ثابت uchar* tab = 0;
    إذا( cell_سائيز == 2 )
        tab = ڪڍو_ڳڻپ_Table2;
    وإلا إذا( cell_سائيز == 4 )
        tab = ڪڍو_ڳڻپ_Table4;
    وإلا
        أرجع -1;
    صحيح i = 0;
    صحيح نتيجو = 0;
#إذا (CV_SIMD || CV_SIMD_SCALABLE)
    v_uint64 t = vx_setzero_u64();
    إذا ( cell_سائيز == 2)
    {
        v_uint16 نقاب = v_reinterpret_as_u16(vx_setall_u8(0x55));
        لكل(; i <= n - VTraits<v_uint8>::vlanes(); i += VTraits<v_uint8>::vlanes())
        {
            v_uint16 a0 = v_reinterpret_as_u16(vx_ڀريو(a + i));
            t = v_جوڙيو(t, v_popcount(v_reinterpret_as_u64(v_and(v_or(a0, v_shr<1>(a0)), نقاب))));
        }
    }
    وإلا    // cellSize == 4
    {
        v_uint16 نقاب = v_reinterpret_as_u16(vx_setall_u8(0x11));
        لكل(; i <= n - VTraits<v_uint8>::vlanes(); i += VTraits<v_uint8>::vlanes())
        {
            v_uint16 a0 = v_reinterpret_as_u16(vx_ڀريو(a + i));
            v_uint16 a1 = v_or(a0, v_shr<2>(a0));
            t = v_جوڙيو(t, v_popcount(v_reinterpret_as_u64(v_and(v_or(a1, v_shr<1>(a1)), نقاب))));

        }
    }
    نتيجو += (صحيح)v_reduce_جوڙ(t);
    vx_cleanup();
#وإلاإذا CV_ENABLE_UNROLLED
    لكل( ; i <= n - 4; i += 4 )
        نتيجو += tab[a[i]] + tab[a[i+1]] + tab[a[i+2]] + tab[a[i+3]];
#جيڪڏهن_پورو
    لكل( ; i < n; i++ )
        نتيجو += tab[a[i]];
    أرجع نتيجو;
}

صحيح normHamming(ثابت uchar* a, ثابت uchar* b, صحيح n, صحيح cell_سائيز)
{
    صحيح output;
    CALL_HAL_RET(normHammingDiff8u, cv_hal_normHammingDiff8u, output, a, b, n, cell_سائيز);

    إذا( cell_سائيز == 1 )
        أرجع normHamming(a, b, n);
    ثابت uchar* tab = 0;
    إذا( cell_سائيز == 2 )
        tab = ڪڍو_ڳڻپ_Table2;
    وإلا إذا( cell_سائيز == 4 )
        tab = ڪڍو_ڳڻپ_Table4;
    وإلا
        أرجع -1;
    صحيح i = 0;
    صحيح نتيجو = 0;
#إذا (CV_SIMD || CV_SIMD_SCALABLE)
    v_uint64 t = vx_setzero_u64();
    إذا ( cell_سائيز == 2)
    {
        v_uint16 نقاب = v_reinterpret_as_u16(vx_setall_u8(0x55));
        لكل(; i <= n - VTraits<v_uint8>::vlanes(); i += VTraits<v_uint8>::vlanes())
        {
            v_uint16 ab0 = v_reinterpret_as_u16(v_xor(vx_ڀريو(a + i), vx_ڀريو(b + i)));
            t = v_جوڙيو(t, v_popcount(v_reinterpret_as_u64(v_and(v_or(ab0, v_shr<1>(ab0)), نقاب))));
        }
    }
    وإلا    // cellSize == 4
    {
        v_uint16 نقاب = v_reinterpret_as_u16(vx_setall_u8(0x11));
        لكل(; i <= n - VTraits<v_uint8>::vlanes(); i += VTraits<v_uint8>::vlanes())
        {
            v_uint16 ab0 = v_reinterpret_as_u16(v_xor(vx_ڀريو(a + i), vx_ڀريو(b + i)));
            v_uint16 ab1 = v_or(ab0, v_shr<2>(ab0));
            t = v_جوڙيو(t, v_popcount(v_reinterpret_as_u64(v_and(v_or(ab1, v_shr<1>(ab1)), نقاب))));
        }
    }
    نتيجو += (صحيح)v_reduce_جوڙ(t);
    vx_cleanup();
#وإلاإذا CV_ENABLE_UNROLLED
    لكل( ; i <= n - 4; i += 4 )
        نتيجو += tab[a[i] ^ b[i]] + tab[a[i+1] ^ b[i+1]] +
                tab[a[i+2] ^ b[i+2]] + tab[a[i+3] ^ b[i+3]];
#جيڪڏهن_پورو
    لكل( ; i < n; i++ )
        نتيجو += tab[a[i] ^ b[i]];
    أرجع نتيجو;
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
        v_float32 t0 = v_sub(vx_ڀريو(a + j), vx_ڀريو(b + j));
        v_float32 t1 = v_sub(vx_ڀريو(a + j + vl), vx_ڀريو(b + j + vl));
        v_d0 = v_muladd(t0, t0, v_d0);
        v_float32 t2 = v_sub(vx_ڀريو(a + j + 2 * vl), vx_ڀريو(b + j + 2 * vl));
        v_d1 = v_muladd(t1, t1, v_d1);
        v_float32 t3 = v_sub(vx_ڀريو(a + j + 3 * vl), vx_ڀريو(b + j + 3 * vl));
        v_d2 = v_muladd(t2, t2, v_d2);
        v_d3 = v_muladd(t3, t3, v_d3);
    }
    v_d0 = v_جوڙيو(v_جوڙيو(v_d0, v_d1), v_جوڙيو(v_d2, v_d3));
    لكل (; j <= n - vl; j += vl)
    {
        v_float32 t0 = v_sub(vx_ڀريو(a + j), vx_ڀريو(b + j));
        v_d0 = v_muladd(t0, t0, v_d0);
    }
    d = v_reduce_جوڙ(v_d0);
#جيڪڏهن_پورو
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
        v_d0 = v_جوڙيو(v_d0, v_absdiff(vx_ڀريو(a + j), vx_ڀريو(b + j)));
        v_d1 = v_جوڙيو(v_d1, v_absdiff(vx_ڀريو(a + j + vl), vx_ڀريو(b + j + vl)));
        v_d2 = v_جوڙيو(v_d2, v_absdiff(vx_ڀريو(a + j + 2 * vl), vx_ڀريو(b + j + 2 * vl)));
        v_d3 = v_جوڙيو(v_d3, v_absdiff(vx_ڀريو(a + j + 3 * vl), vx_ڀريو(b + j + 3 * vl)));
    }
    v_d0 = v_جوڙيو(v_جوڙيو(v_d0, v_d1), v_جوڙيو(v_d2, v_d3));
    لكل (; j <= n - vl; j += vl)
        v_d0 = v_جوڙيو(v_d0, v_absdiff(vx_ڀريو(a + j), vx_ڀريو(b + j)));
    d = v_reduce_جوڙ(v_d0);
#جيڪڏهن_پورو
    لكل( ; j < n; j++ )
        d += std::مطلق_قدر(a[j] - b[j]);
    أرجع d;
}

صحيح normL1_(ثابت uchar* a, ثابت uchar* b, صحيح n)
{
    صحيح j = 0, d = 0;
#إذا (CV_SIMD || CV_SIMD_SCALABLE)
    ثابت صحيح vl = VTraits<v_uint8>::vlanes();
    لكل (; j <= n - 4 * vl; j += 4 * vl)
        d += v_reduce_sad(vx_ڀريو(a + j), vx_ڀريو(b + j)) +
             v_reduce_sad(vx_ڀريو(a + j + vl), vx_ڀريو(b + j + vl)) +
             v_reduce_sad(vx_ڀريو(a + j + 2 * vl), vx_ڀريو(b + j + 2 * vl)) +
             v_reduce_sad(vx_ڀريو(a + j + 3 * vl), vx_ڀريو(b + j + 3 * vl));
    لكل (; j <= n - vl; j += vl)
        d += v_reduce_sad(vx_ڀريو(a + j), vx_ڀريو(b + j));
#جيڪڏهن_پورو
    لكل( ; j < n; j++ )
        d += std::مطلق_قدر(a[j] - b[j]);
    أرجع d;
}

} //cv::hal

//==================================================================================================

تعريفنوع صحيح (*NormFunc)(ثابت uchar*, ثابت uchar*, uchar*, صحيح, صحيح);
تعريفنوع صحيح (*NormDiffFunc)(ثابت uchar*, ثابت uchar*, ثابت uchar*, uchar*, صحيح, صحيح);

#جيڪڏهن_وصف_ٿيل HAVE_OPENCL

ساكن منطقي ocl_norm( داخلا_Array _src, صحيح normType, داخلا_Array نقاب_2, مزدوج & نتيجو )
{
    ثابت ocl::Device & d = ocl::Device::getDefault();

#جيڪڏهن_وصف_ٿيل __ANDROID__
    إذا (d.isNVidia())
        أرجع خطأ;
#جيڪڏهن_پورو
    ثابت صحيح cn = _src.channels();
    إذا (cn > 4)
        أرجع خطأ;
    صحيح type = _src.type(), اونهائي = CV_MAT_اونهائي(type);
    منطقي doubleSupport = d.doubleFPConfig() > 0,
            have_نقاب = نقاب_2.قسم() != داخلا_Array_2::NONE;

    إذا (اونهائي >= CV_16F)
        أرجع خطأ;  // TODO: support FP16

    إذا ( !(normType == NORM_INF || normType == NORM_L1 || normType == NORM_L2 || normType == NORM_L2SQR) ||
         (!doubleSupport && اونهائي == CV_64F))
        أرجع خطأ;

    UMat src = _src.getUMat();

    إذا (normType == NORM_INF)
    {
        إذا (!ocl_minMaxIdx(_src, عدم, &نتيجو, عدم, عدم, نقاب_2,
                           std::max(اونهائي, CV_32S), اونهائي != CV_8U && اونهائي != CV_16U))
            أرجع خطأ;
    }
    وإلا إذا (normType == NORM_L1 || normType == NORM_L2 || normType == NORM_L2SQR)
    {
        Scalar sc;
        منطقي unstype = اونهائي == CV_8U || اونهائي == CV_16U;

        إذا ( !ocl_جوڙ(have_نقاب ? src : src.reshape(1), sc, normType == NORM_L2 || normType == NORM_L2SQR ?
                    OCL_OP_جوڙ_SQR : (unstype ? OCL_OP_جوڙ : OCL_OP_جوڙ_ABS), نقاب_2) )
            أرجع خطأ;

        مزدوج s = 0.0;
        لكل (صحيح i = 0; i < (have_نقاب ? cn : 1); ++i)
            s += sc[i];

        نتيجو = normType == NORM_L1 || normType == NORM_L2SQR ? s : std::جذر(s);
    }

    أرجع سچ;
}

#جيڪڏهن_پورو

ساكن NormFunc getNormFunc(صحيح normType, صحيح اونهائي) {
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getNormFunc, (normType, اونهائي), CV_CPU_DISPATCH_MODES_ALL);
}
ساكن NormDiffFunc getNormDiffFunc(صحيح normType, صحيح اونهائي) {
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getNormDiffFunc, (normType, اونهائي), CV_CPU_DISPATCH_MODES_ALL);
}

مزدوج norm( داخلا_Array _src, صحيح normType, داخلا_Array نقاب_2 )
{
    CV_INSTRUMENT_REGION();

    normType &= NORM_TYPE_نقاب;
    CV_Assert( normType == NORM_INF || normType == NORM_L1 ||
               normType == NORM_L2 || normType == NORM_L2SQR ||
               ((normType == NORM_HAMMING || normType == NORM_HAMMING2) && _src.type() == CV_8U) );

#إذا defined HAVE_OPENCL
    مزدوج نتيجو_2 = 0;
#جيڪڏهن_پورو

#جيڪڏهن_وصف_ٿيل HAVE_OPENCL
    CV_OCL_لاڳيتو_2(OCL_PERFORMANCE_CHECK(_src.isUMat()) && _src.dims() <= 2,
                ocl_norm(_src, normType, نقاب_2, نتيجو_2),
                نتيجو_2)
#جيڪڏهن_پورو

    Mat src = _src.getMat(), نقاب = نقاب_2.getMat();
    CV_Assert( نقاب.empty() || ((نقاب.type() == CV_8U || نقاب.type() == CV_8S || نقاب.type() == CV_Bool) && src.size == نقاب.size) );
    إذا( src.empty() )
        أرجع 0.0;
    صحيح اونهائي = src.اونهائي(), cn = src.channels();
    إذا( src.dims <= 2 )
    {
        مزدوج نتيجو;
        CALL_HAL_RET(norm, cv_hal_norm, نتيجو, src.معلومات, src.قدم, نقاب.معلومات, نقاب.قدم, src.کانن_ڳڻپ, src.صفون, src.type(), normType);
    }
    وإلا إذا( src.isContinuous() && نقاب.isContinuous() )
    {
        مزدوج نتيجو;
        CALL_HAL_RET(norm, cv_hal_norm, نتيجو, src.معلومات, 0, نقاب.معلومات, 0, (صحيح)src.ڪل(), 1, src.type(), normType);
    }

    NormFunc func = getNormFunc(normType >> 1, اونهائي);
    CV_Assert( (normType >> 1) >= 3 || func != 0 );

    إذا( src.isContinuous() && نقاب.empty() )
    {
        size_t len = src.ڪل()*cn;
        إذا( len == (size_t)(صحيح)len )
        {
            إذا( اونهائي == CV_32F )
            {
                ثابت uchar* معلومات = src.ptr<ثابت uchar>();

                إذا( normType == NORM_L2 || normType == NORM_L2SQR || normType == NORM_L1 )
                {
                    مزدوج نتيجو = 0;
                    func(معلومات, 0, (uchar*)&نتيجو, (صحيح)len, 1);
                    أرجع normType == NORM_L2 ? std::جذر(نتيجو) : نتيجو;
                }
                إذا( normType == NORM_INF )
                {
                    عائم نتيجو = 0;
                    func(معلومات, 0, (uchar*)&نتيجو, (صحيح)len, 1);
                    أرجع نتيجو;
                }
            }
            إذا( اونهائي == CV_8U )
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

    CV_Assert( نقاب.empty() || نقاب.type() == CV_8U || نقاب.type() == CV_8S || نقاب.type() == CV_Bool );

    إذا( normType == NORM_HAMMING || normType == NORM_HAMMING2 )
    {
        إذا( !نقاب.empty() )
        {
            Mat عارضي_2;
            bitwise_and(src, نقاب, عارضي_2);
            أرجع norm(عارضي_2, normType);
        }
        صحيح cell_سائيز = normType == NORM_HAMMING ? 1 : 2;

        ثابت Mat* arrays[] = {&src, 0};
        uchar* ptrs[1] = {};
        NAryMatIterator it(arrays, ptrs);
        صحيح ڪل = (صحيح)it.size;
        صحيح نتيجو = 0;

        لكل( size_t i = 0; i < it.nplanes; i++, ++it )
        {
            نتيجو += hal::normHamming(ptrs[0], ڪل, cell_سائيز);
        }

        أرجع نتيجو;
    }

    ثابت Mat* arrays[] = {&src, &نقاب, 0};
    uchar* ptrs[2] = {};
    اتحاد
    {
        مزدوج d;
        بنانشان u;
        uint64 UL;
        عائم f;
    }
    نتيجو;
    نتيجو.d = 0;
    NAryMatIterator it(arrays, ptrs);
    CV_CheckLT((size_t)it.size, (size_t)وڌ_کان_وڌ_عدد, "");
    منطقي is_fp16 = اونهائي == CV_16F || اونهائي == CV_16BF;

    إذا ((normType == NORM_L1 && (اونهائي <= CV_16S || اونهائي == CV_Bool || is_fp16)) ||
        ((normType == NORM_L2 || normType == NORM_L2SQR) && (اونهائي <= CV_8S || اونهائي == CV_Bool || is_fp16)))
    {
        // special case to handle "integer" overflow in accumulator
        ثابت size_t esz = src.elem_سائيز();
        ثابت صحيح ڪل = (صحيح)it.size;
        ثابت صحيح بلاڪ_Size0 = (is_fp16 ? (1 << 10) :
            اونهائي == CV_Bool ? (1 << 30) :
            normType == NORM_L1 && اونهائي <= CV_8S ? (1 << 23) : (1 << 15))/cn;
        ثابت صحيح بلاڪ_سائيز_2 = std::min(ڪل, بلاڪ_Size0);
        اتحاد {
            صحيح i;
            عائم f;
        } blocksum;
        blocksum.i = 0;
        صحيح ڳڻپ_2 = 0;

        لكل (size_t i = 0; i < it.nplanes; i++, ++it)
        {
            لكل (صحيح j = 0; j < ڪل; j += بلاڪ_سائيز_2)
            {
                صحيح bsz = std::min(ڪل - j, بلاڪ_سائيز_2);
                func(ptrs[0], ptrs[1], (uchar*)&blocksum, bsz, cn);
                ڳڻپ_2 += bsz;
                إذا (ڳڻپ_2 + بلاڪ_سائيز_2 >= بلاڪ_Size0 || (i+1 >= it.nplanes && j+bsz >= ڪل))
                {
                    نتيجو.d += is_fp16 ? (مزدوج)blocksum.f : (مزدوج)blocksum.i;
                    blocksum.i = 0;
                    ڳڻپ_2 = 0;
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
            func(ptrs[0], ptrs[1], (uchar*)&نتيجو, (صحيح)it.size, cn);
        }
    }

    إذا( normType == NORM_INF )
    {
        إذا(اونهائي <= CV_32S || اونهائي == CV_32U || اونهائي == CV_Bool)
            أرجع نتيجو.u;
        إذا (اونهائي == CV_32F || is_fp16)
            أرجع نتيجو.f;
        إذا (اونهائي == CV_64U || اونهائي == CV_64S)
            أرجع (مزدوج)نتيجو.UL;
    }
    إذا( normType == NORM_L2 )
        أرجع std::جذر(نتيجو.d);

    أرجع نتيجو.d;
}

//==================================================================================================

#جيڪڏهن_وصف_ٿيل HAVE_OPENCL
ساكن منطقي ocl_norm( داخلا_Array _src1, داخلا_Array _src2, صحيح normType, داخلا_Array نقاب_2, مزدوج & نتيجو )
{
#جيڪڏهن_وصف_ٿيل __ANDROID__
    إذا (ocl::Device::getDefault().isNVidia())
        أرجع خطأ;
#جيڪڏهن_پورو

    Scalar sc1, sc2;
    صحيح cn = _src1.channels();
    إذا (cn > 4)
        أرجع خطأ;
    صحيح type = _src1.type(), اونهائي = CV_MAT_اونهائي(type);
    منطقي relative = (normType & NORM_RELATIVE) != 0;
    normType &= ~NORM_RELATIVE;
    منطقي normsum = normType == NORM_L1 || normType == NORM_L2 || normType == NORM_L2SQR;

#جيڪڏهن_وصف_ٿيل __APPLE__
    إذا(normType == NORM_L1 && type == CV_16UC3 && !نقاب_2.empty())
        أرجع خطأ;
#جيڪڏهن_پورو

    إذا (normsum)
    {
        إذا (!ocl_جوڙ(_src1, sc1, normType == NORM_L2 || normType == NORM_L2SQR ?
                     OCL_OP_جوڙ_SQR : OCL_OP_جوڙ, نقاب_2, _src2, relative, sc2))
            أرجع خطأ;
    }
    وإلا
    {
        إذا (!ocl_minMaxIdx(_src1, عدم, &sc1[0], عدم, عدم, نقاب_2, std::max(CV_32S, اونهائي),
                           خطأ, _src2, relative ? &sc2[0] : عدم))
            أرجع خطأ;
        cn = 1;
    }

    مزدوج s2 = 0;
    لكل (صحيح i = 0; i < cn; ++i)
    {
        نتيجو += sc1[i];
        إذا (relative)
            s2 += sc2[i];
    }

    إذا (normType == NORM_L2)
    {
        نتيجو = std::جذر(نتيجو);
        إذا (relative)
            s2 = std::جذر(s2);
    }

    إذا (relative)
        نتيجو /= (s2 + DBL_EPSILON);

    أرجع سچ;
}  // ocl_norm()
#جيڪڏهن_پورو  // HAVE_OPENCL

مزدوج norm( داخلا_Array _src1, داخلا_Array _src2, صحيح normType, داخلا_Array نقاب_2 )
{
    CV_INSTRUMENT_REGION();

    CV_CheckTypeEQ(_src1.type(), _src2.type(), "Input type mismatch");
    CV_Assert(_src1.same_سائيز(_src2));

#إذا defined HAVE_OPENCL
    مزدوج نتيجو_2 = 0;
#جيڪڏهن_پورو

#جيڪڏهن_وصف_ٿيل HAVE_OPENCL
    CV_OCL_لاڳيتو_2(OCL_PERFORMANCE_CHECK(_src1.isUMat()),
                ocl_norm(_src1, _src2, normType, نقاب_2, نتيجو_2),
                نتيجو_2)
#جيڪڏهن_پورو

    Mat src1 = _src1.getMat(), src2 = _src2.getMat(), نقاب = نقاب_2.getMat();
    CV_Assert( نقاب.empty() || ((نقاب.type() == CV_8U || نقاب.type() == CV_8S || نقاب.type() == CV_Bool) && src1.size == نقاب.size) );
    إذا( src1.empty() || src2.empty() )
        أرجع 0.0;
    صحيح اونهائي = src1.اونهائي(), cn = src1.channels();
    إذا( src1.dims <= 2 )
    {
        مزدوج نتيجو;
        CALL_HAL_RET(normDiff, cv_hal_normDiff, نتيجو, src1.معلومات, src1.قدم, src2.معلومات, src2.قدم, نقاب.معلومات, نقاب.قدم, src1.کانن_ڳڻپ, src1.صفون, src1.type(), normType);
    }
    وإلا إذا( src1.isContinuous() && src2.isContinuous() && نقاب.isContinuous() )
    {
        مزدوج نتيجو;
        CALL_HAL_RET(normDiff, cv_hal_normDiff, نتيجو, src1.معلومات, 0, src2.معلومات, 0, نقاب.معلومات, 0, (صحيح)src1.ڪل(), 1, src1.type(), normType);
    }

    إذا( normType & NORM_RELATIVE )
    {
        أرجع norm(_src1, _src2, normType & ~NORM_RELATIVE, نقاب_2)/(norm(_src2, normType, نقاب_2) + DBL_EPSILON);
    }

    normType &= 7;
    CV_Assert( normType == NORM_INF || normType == NORM_L1 ||
               normType == NORM_L2 || normType == NORM_L2SQR ||
              ((normType == NORM_HAMMING || normType == NORM_HAMMING2) && src1.type() == CV_8U) );

    NormDiffFunc func = getNormDiffFunc(normType >> 1, اونهائي);
    CV_Assert( (normType >> 1) >= 3 || func != 0 );

    إذا( src1.isContinuous() && src2.isContinuous() && نقاب.empty() )
    {
        size_t len = src1.ڪل()*src1.channels();
        إذا( len == (size_t)(صحيح)len )
        {
            إذا( src1.اونهائي() == CV_32F )
            {
                ثابت uchar* data1 = src1.ptr<ثابت uchar>();
                ثابت uchar* data2 = src2.ptr<ثابت uchar>();

                إذا( normType == NORM_L2 || normType == NORM_L2SQR || normType == NORM_L1 )
                {
                    مزدوج نتيجو = 0;
                    func(data1, data2, 0, (uchar*)&نتيجو, (صحيح)len, 1);
                    أرجع normType == NORM_L2 ? std::جذر(نتيجو) : نتيجو;
                }
                إذا( normType == NORM_INF )
                {
                    عائم نتيجو = 0;
                    func(data1, data2, 0, (uchar*)&نتيجو, (صحيح)len, 1);
                    أرجع نتيجو;
                }
            }
        }
    }

    CV_Assert( نقاب.empty() || نقاب.type() == CV_8U || نقاب.type() == CV_8S || نقاب.type() == CV_Bool );

    إذا( normType == NORM_HAMMING || normType == NORM_HAMMING2 )
    {
        إذا( !نقاب.empty() )
        {
            Mat عارضي_2;
            bitwise_xor(src1, src2, عارضي_2);
            bitwise_and(عارضي_2, نقاب, عارضي_2);
            أرجع norm(عارضي_2, normType);
        }
        صحيح cell_سائيز = normType == NORM_HAMMING ? 1 : 2;

        ثابت Mat* arrays[] = {&src1, &src2, 0};
        uchar* ptrs[2] = {};
        NAryMatIterator it(arrays, ptrs);
        صحيح ڪل = (صحيح)it.size;
        صحيح نتيجو = 0;

        لكل( size_t i = 0; i < it.nplanes; i++, ++it )
        {
            نتيجو += hal::normHamming(ptrs[0], ptrs[1], ڪل, cell_سائيز);
        }

        أرجع نتيجو;
    }

    ثابت Mat* arrays[] = {&src1, &src2, &نقاب, 0};
    uchar* ptrs[3] = {};
    اتحاد
    {
        مزدوج d;
        عائم f;
        بنانشان u;
        uint64 UL;
    }
    نتيجو;
    نتيجو.d = 0;
    NAryMatIterator it(arrays, ptrs);
    CV_CheckLT((size_t)it.size, (size_t)وڌ_کان_وڌ_عدد, "");

    منطقي is_fp16 = اونهائي == CV_16F || اونهائي == CV_16BF;

    إذا ((normType == NORM_L1 && (اونهائي <= CV_16S || اونهائي == CV_Bool || is_fp16)) ||
        ((normType == NORM_L2 || normType == NORM_L2SQR) && (اونهائي <= CV_8S || اونهائي == CV_Bool || is_fp16)))
    {
        // special case to handle "integer" overflow in accumulator
        ثابت size_t esz = src1.elem_سائيز();
        ثابت صحيح ڪل = (صحيح)it.size;
        ثابت صحيح بلاڪ_Size0 = (is_fp16 ? (1 << 10) :
            اونهائي == CV_Bool ? (1 << 30) :
            normType == NORM_L1 && اونهائي <= CV_8S ? (1 << 23) : (1 << 15))/cn;
        ثابت صحيح بلاڪ_سائيز_2 = std::min(ڪل, بلاڪ_Size0);
        اتحاد {
            صحيح i;
            عائم f;
        } blocksum;
        blocksum.i = 0;
        صحيح ڳڻپ_2 = 0;

        لكل (size_t i = 0; i < it.nplanes; i++, ++it)
        {
            لكل (صحيح j = 0; j < ڪل; j += بلاڪ_سائيز_2)
            {
                صحيح bsz = std::min(ڪل - j, بلاڪ_سائيز_2);
                func(ptrs[0], ptrs[1], ptrs[2], (uchar*)&blocksum, bsz, cn);
                ڳڻپ_2 += bsz;
                إذا (ڳڻپ_2 + بلاڪ_سائيز_2 >= بلاڪ_Size0 || (i+1 >= it.nplanes && j+bsz >= ڪل))
                {
                    نتيجو.d += is_fp16 ? (مزدوج)blocksum.f : (مزدوج)blocksum.i;
                    blocksum.i = 0;
                    ڳڻپ_2 = 0;
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
            func(ptrs[0], ptrs[1], ptrs[2], (uchar*)&نتيجو, (صحيح)it.size, cn);
        }
    }

    إذا( normType == NORM_INF )
    {
        إذا (اونهائي <= CV_32S || اونهائي == CV_32U || اونهائي == CV_Bool)
            أرجع نتيجو.u;
        إذا (اونهائي == CV_32F || is_fp16)
            أرجع نتيجو.f;
        إذا (اونهائي == CV_64U || اونهائي == CV_64S)
            أرجع (مزدوج)نتيجو.UL;
    }
    إذا( normType == NORM_L2 )
        أرجع std::جذر(نتيجو.d);

    أرجع نتيجو.d;
}

cv::Hamming::نتيجو_Type Hamming::عامل()( ثابت بنانشان حرف* a, ثابت بنانشان حرف* b, صحيح size ) ثابت
{
    أرجع cv::hal::normHamming(a, b, size);
}

مزدوج PSNR(داخلا_Array _src1, داخلا_Array _src2, مزدوج R)
{
    CV_INSTRUMENT_REGION();

    //Input arrays must have depth CV_8U
    CV_Assert( _src1.type() == _src2.type() );

    مزدوج diff = std::جذر(norm(_src1, _src2, NORM_L2SQR)/(_src1.ڪل()*_src1.channels()));
    أرجع 20*log10(R/(diff+DBL_EPSILON));
}


#جيڪڏهن_وصف_ٿيل HAVE_OPENCL
ساكن منطقي ocl_normalize( داخلا_Array _src, داخلا_Output_Array _dst, داخلا_Array نقاب_2, صحيح dtype,
                           مزدوج scale, مزدوج delta )
{
    UMat src = _src.getUMat();

    إذا( نقاب_2.empty() )
        src.convert_منزل( _dst, dtype, scale, delta );
    وإلا إذا (src.channels() <= 4)
    {
        ثابت ocl::Device & dev = ocl::Device::getDefault();

        صحيح stype = _src.type(), sdepth = CV_MAT_اونهائي(stype), cn = CV_MAT_CN(stype),
                ddepth = CV_MAT_اونهائي(dtype), wdepth = std::max(CV_32F, std::max(sdepth, ddepth)),
                صفون_في_WI = dev.isIntel() ? 4 : 1;

        عائم fscale = static_cast<عائم>(scale), fdelta = static_cast<عائم>(delta);
        منطقي haveScale = std::fabs(scale - 1) > DBL_EPSILON,
                haveZeroScale = !(std::fabs(scale) > DBL_EPSILON),
                haveDelta = std::fabs(delta) > DBL_EPSILON,
                doubleSupport = dev.doubleFPConfig() > 0;

        إذا (!haveScale && !haveDelta && stype == dtype)
        {
            _src.copy_منزل(_dst, نقاب_2);
            أرجع سچ;
        }
        إذا (haveZeroScale)
        {
            _dst.سيٽ_منزل(Scalar(delta), نقاب_2);
            أرجع سچ;
        }

        إذا ((sdepth == CV_64F || ddepth == CV_64F) && !doubleSupport)
            أرجع خطأ;

        حرف cvt[2][50];
        String opts = format("-D srcT=%s -D dstT=%s -D convertToWT=%s -D cn=%d -D rowsPerWI=%d"
                             " -D convertToDT=%s -D workT=%s%s%s%s -D srcT1=%s -D dstT1=%s",
                             ocl::type_منزل_Str(stype), ocl::type_منزل_Str(dtype),
                             ocl::convertTypeStr(sdepth, wdepth, cn, cvt[0], حجم(cvt[0])), cn,
                             صفون_في_WI, ocl::convertTypeStr(wdepth, ddepth, cn, cvt[1], حجم(cvt[1])),
                             ocl::type_منزل_Str(CV_MAKE_TYPE(wdepth, cn)),
                             doubleSupport ? " -D DOUBLE_SUPPORT" : "",
                             haveScale ? " -D HAVE_SCALE" : "",
                             haveDelta ? " -D HAVE_DELTA" : "",
                             ocl::type_منزل_Str(sdepth), ocl::type_منزل_Str(ddepth));

        ocl::Kernel k("normalizek", ocl::core::normalize_oclsrc, opts);
        إذا (k.empty())
            أرجع خطأ;

        UMat نقاب = نقاب_2.getUMat(), dst = _dst.getUMat();

        ocl::KernelArg srcarg = ocl::KernelArg::Read_Only_No_سائيز(src),
                maskarg = ocl::KernelArg::Read_Only_No_سائيز(نقاب),
                dstarg = ocl::KernelArg::ReadWrite(dst);

        إذا (haveScale)
        {
            إذا (haveDelta)
                k.دليلون(srcarg, maskarg, dstarg, fscale, fdelta);
            وإلا
                k.دليلون(srcarg, maskarg, dstarg, fscale);
        }
        وإلا
        {
            إذا (haveDelta)
                k.دليلون(srcarg, maskarg, dstarg, fdelta);
            وإلا
                k.دليلون(srcarg, maskarg, dstarg);
        }

        size_t globalsize[2] = { (size_t)src.کانن_ڳڻپ, ((size_t)src.صفون + صفون_في_WI - 1) / صفون_في_WI };
        أرجع k.لاڳيتو(2, globalsize, عدم, خطأ);
    }
    وإلا
    {
        UMat عارضي_2;
        src.convert_منزل( عارضي_2, dtype, scale, delta );
        عارضي_2.copy_منزل( _dst, نقاب_2 );
    }

    أرجع سچ;
}  // ocl_normalize
#جيڪڏهن_پورو  // HAVE_OPENCL

فراغ normalize(داخلا_Array _src, داخلا_Output_Array _dst, مزدوج a, مزدوج b,
               صحيح norm_type, صحيح rtype, داخلا_Array نقاب_2)
{
    CV_INSTRUMENT_REGION();

    مزدوج scale = 1, سرڪايو = 0;
    صحيح type = _src.type(), اونهائي = CV_MAT_اونهائي(type);

    إذا( rtype < 0 )
        rtype = _dst.fixedType() ? _dst.اونهائي() : اونهائي;

    إذا( norm_type == NORM_MINMAX )
    {
        مزدوج smin = 0, smax = 0;
        مزدوج dmin = MIN( a, b ), dmax = MAX( a, b );
        minMaxIdx( _src, &smin, &smax, 0, 0, نقاب_2 );
        scale = (dmax - dmin)*(smax - smin > DBL_EPSILON ? 1./(smax - smin) : 0);
        إذا( rtype == CV_32F )
        {
            scale = (عائم)scale;
            سرڪايو = (عائم)dmin - (عائم)(smin*scale);
        }
        وإلا
            سرڪايو = dmin - smin*scale;
    }
    وإلا إذا( norm_type == NORM_L2 || norm_type == NORM_L1 || norm_type == NORM_INF )
    {
        scale = norm( _src, norm_type, نقاب_2 );
        scale = scale > DBL_EPSILON ? a/scale : 0.;
        سرڪايو = 0;
    }
    وإلا
        CV_Error( cv::Error::StsBadArg, "Unknown/unsupported norm type" );

    CV_OCL_لاڳيتو(_dst.isUMat(),
               ocl_normalize(_src, _dst, نقاب_2, rtype, scale, سرڪايو))

    Mat src = _src.getMat();
    إذا( نقاب_2.empty() )
        src.convert_منزل( _dst, rtype, scale, سرڪايو );
    وإلا
    {
        Mat عارضي_2;
        src.convert_منزل( عارضي_2, rtype, scale, سرڪايو );
        عارضي_2.copy_منزل( _dst, نقاب_2 );
    }
}

}  // namespace
