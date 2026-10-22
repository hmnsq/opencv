// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html
//
// Copyright (C) 2026, Advanced Micro Devices, Inc., all rights reserved.


#অন্তর্ভাব "precomp.hpp"
#অন্তর্ভাব "opencl_kernels_core.hpp"
#অন্তর্ভাব "stat.hpp"

#অন্তর্ভাব "norm.simd.hpp"
#অন্তর্ভাব "norm.simd_declarations.hpp"

/****************************************************************************************\
*                                         norm                                           *
\****************************************************************************************/

নামাকাশ cv { নামাকাশ hal {

বাহ্যিক ধ্রুবক uchar বের_করো_গুনতি_Table[256] =
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

স্থির ধ্রুবক uchar বের_করো_গুনতি_Table2[] =
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

স্থির ধ্রুবক uchar বের_করো_গুনতি_Table4[] =
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


পূর্ণসংখ্যা normHamming(ধ্রুবক uchar* a, পূর্ণসংখ্যা n, পূর্ণসংখ্যা cell_আকার)
{
    পূর্ণসংখ্যা output;
    CALL_HAL_RET(normHamming8u, cv_hal_normHamming8u, output, a, n, cell_আকার);

    যদি( cell_আকার == 1 )
        ফেরত normHamming(a, n);
    ধ্রুবক uchar* tab = 0;
    যদি( cell_আকার == 2 )
        tab = বের_করো_গুনতি_Table2;
    অন্যথা যদি( cell_আকার == 4 )
        tab = বের_করো_গুনতি_Table4;
    অন্যথা
        ফেরত -1;
    পূর্ণসংখ্যা i = 0;
    পূর্ণসংখ্যা ফলাফল = 0;
#যদি (CV_SIMD || CV_SIMD_SCALABLE)
    v_uint64 t = vx_setzero_u64();
    যদি ( cell_আকার == 2)
    {
        v_uint16 mask = v_reinterpret_as_u16(vx_setall_u8(0x55));
        জন্য(; i <= n - VTraits<v_uint8>::vlanes(); i += VTraits<v_uint8>::vlanes())
        {
            v_uint16 a0 = v_reinterpret_as_u16(vx_load(a + i));
            t = v_add(t, v_popcount(v_reinterpret_as_u64(v_and(v_or(a0, v_shr<1>(a0)), mask))));
        }
    }
    অন্যথা    // cellSize == 4
    {
        v_uint16 mask = v_reinterpret_as_u16(vx_setall_u8(0x11));
        জন্য(; i <= n - VTraits<v_uint8>::vlanes(); i += VTraits<v_uint8>::vlanes())
        {
            v_uint16 a0 = v_reinterpret_as_u16(vx_load(a + i));
            v_uint16 a1 = v_or(a0, v_shr<2>(a0));
            t = v_add(t, v_popcount(v_reinterpret_as_u64(v_and(v_or(a1, v_shr<1>(a1)), mask))));

        }
    }
    ফলাফল += (পূর্ণসংখ্যা)v_reduce_যোগ(t);
    vx_cleanup();
#নতুবা_যদি CV_ENABLE_UNROLLED
    জন্য( ; i <= n - 4; i += 4 )
        ফলাফল += tab[a[i]] + tab[a[i+1]] + tab[a[i+2]] + tab[a[i+3]];
#endif
    জন্য( ; i < n; i++ )
        ফলাফল += tab[a[i]];
    ফেরত ফলাফল;
}

পূর্ণসংখ্যা normHamming(ধ্রুবক uchar* a, ধ্রুবক uchar* b, পূর্ণসংখ্যা n, পূর্ণসংখ্যা cell_আকার)
{
    পূর্ণসংখ্যা output;
    CALL_HAL_RET(normHammingDiff8u, cv_hal_normHammingDiff8u, output, a, b, n, cell_আকার);

    যদি( cell_আকার == 1 )
        ফেরত normHamming(a, b, n);
    ধ্রুবক uchar* tab = 0;
    যদি( cell_আকার == 2 )
        tab = বের_করো_গুনতি_Table2;
    অন্যথা যদি( cell_আকার == 4 )
        tab = বের_করো_গুনতি_Table4;
    অন্যথা
        ফেরত -1;
    পূর্ণসংখ্যা i = 0;
    পূর্ণসংখ্যা ফলাফল = 0;
#যদি (CV_SIMD || CV_SIMD_SCALABLE)
    v_uint64 t = vx_setzero_u64();
    যদি ( cell_আকার == 2)
    {
        v_uint16 mask = v_reinterpret_as_u16(vx_setall_u8(0x55));
        জন্য(; i <= n - VTraits<v_uint8>::vlanes(); i += VTraits<v_uint8>::vlanes())
        {
            v_uint16 ab0 = v_reinterpret_as_u16(v_xor(vx_load(a + i), vx_load(b + i)));
            t = v_add(t, v_popcount(v_reinterpret_as_u64(v_and(v_or(ab0, v_shr<1>(ab0)), mask))));
        }
    }
    অন্যথা    // cellSize == 4
    {
        v_uint16 mask = v_reinterpret_as_u16(vx_setall_u8(0x11));
        জন্য(; i <= n - VTraits<v_uint8>::vlanes(); i += VTraits<v_uint8>::vlanes())
        {
            v_uint16 ab0 = v_reinterpret_as_u16(v_xor(vx_load(a + i), vx_load(b + i)));
            v_uint16 ab1 = v_or(ab0, v_shr<2>(ab0));
            t = v_add(t, v_popcount(v_reinterpret_as_u64(v_and(v_or(ab1, v_shr<1>(ab1)), mask))));
        }
    }
    ফলাফল += (পূর্ণসংখ্যা)v_reduce_যোগ(t);
    vx_cleanup();
#নতুবা_যদি CV_ENABLE_UNROLLED
    জন্য( ; i <= n - 4; i += 4 )
        ফলাফল += tab[a[i] ^ b[i]] + tab[a[i+1] ^ b[i+1]] +
                tab[a[i+2] ^ b[i+2]] + tab[a[i+3] ^ b[i+3]];
#endif
    জন্য( ; i < n; i++ )
        ফলাফল += tab[a[i] ^ b[i]];
    ফেরত ফলাফল;
}

দশমিক normL2Sqr_(ধ্রুবক দশমিক* a, ধ্রুবক দশমিক* b, পূর্ণসংখ্যা n)
{
    পূর্ণসংখ্যা j = 0; দশমিক d = 0.f;
#যদি (CV_SIMD || CV_SIMD_SCALABLE)
    ধ্রুবক পূর্ণসংখ্যা vl = VTraits<v_float32>::vlanes();
    v_float32 v_d0 = vx_setzero_f32(), v_d1 = vx_setzero_f32();
    v_float32 v_d2 = vx_setzero_f32(), v_d3 = vx_setzero_f32();
    জন্য (; j <= n - 4 * vl; j += 4 * vl)
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
    জন্য (; j <= n - vl; j += vl)
    {
        v_float32 t0 = v_sub(vx_load(a + j), vx_load(b + j));
        v_d0 = v_muladd(t0, t0, v_d0);
    }
    d = v_reduce_যোগ(v_d0);
#endif
    জন্য( ; j < n; j++ )
    {
        দশমিক t = a[j] - b[j];
        d += t*t;
    }
    ফেরত d;
}


দশমিক normL1_(ধ্রুবক দশমিক* a, ধ্রুবক দশমিক* b, পূর্ণসংখ্যা n)
{
    পূর্ণসংখ্যা j = 0; দশমিক d = 0.f;
#যদি (CV_SIMD || CV_SIMD_SCALABLE)
    ধ্রুবক পূর্ণসংখ্যা vl = VTraits<v_float32>::vlanes();
    v_float32 v_d0 = vx_setzero_f32(), v_d1 = vx_setzero_f32();
    v_float32 v_d2 = vx_setzero_f32(), v_d3 = vx_setzero_f32();
    জন্য (; j <= n - 4 * vl; j += 4 * vl)
    {
        v_d0 = v_add(v_d0, v_absdiff(vx_load(a + j), vx_load(b + j)));
        v_d1 = v_add(v_d1, v_absdiff(vx_load(a + j + vl), vx_load(b + j + vl)));
        v_d2 = v_add(v_d2, v_absdiff(vx_load(a + j + 2 * vl), vx_load(b + j + 2 * vl)));
        v_d3 = v_add(v_d3, v_absdiff(vx_load(a + j + 3 * vl), vx_load(b + j + 3 * vl)));
    }
    v_d0 = v_add(v_add(v_d0, v_d1), v_add(v_d2, v_d3));
    জন্য (; j <= n - vl; j += vl)
        v_d0 = v_add(v_d0, v_absdiff(vx_load(a + j), vx_load(b + j)));
    d = v_reduce_যোগ(v_d0);
#endif
    জন্য( ; j < n; j++ )
        d += std::abs(a[j] - b[j]);
    ফেরত d;
}

পূর্ণসংখ্যা normL1_(ধ্রুবক uchar* a, ধ্রুবক uchar* b, পূর্ণসংখ্যা n)
{
    পূর্ণসংখ্যা j = 0, d = 0;
#যদি (CV_SIMD || CV_SIMD_SCALABLE)
    ধ্রুবক পূর্ণসংখ্যা vl = VTraits<v_uint8>::vlanes();
    জন্য (; j <= n - 4 * vl; j += 4 * vl)
        d += v_reduce_sad(vx_load(a + j), vx_load(b + j)) +
             v_reduce_sad(vx_load(a + j + vl), vx_load(b + j + vl)) +
             v_reduce_sad(vx_load(a + j + 2 * vl), vx_load(b + j + 2 * vl)) +
             v_reduce_sad(vx_load(a + j + 3 * vl), vx_load(b + j + 3 * vl));
    জন্য (; j <= n - vl; j += vl)
        d += v_reduce_sad(vx_load(a + j), vx_load(b + j));
#endif
    জন্য( ; j < n; j++ )
        d += std::abs(a[j] - b[j]);
    ফেরত d;
}

} //cv::hal

//==================================================================================================

প্রকারলেখ পূর্ণসংখ্যা (*NormFunc)(ধ্রুবক uchar*, ধ্রুবক uchar*, uchar*, পূর্ণসংখ্যা, পূর্ণসংখ্যা);
প্রকারলেখ পূর্ণসংখ্যা (*NormDiffFunc)(ধ্রুবক uchar*, ধ্রুবক uchar*, ধ্রুবক uchar*, uchar*, পূর্ণসংখ্যা, পূর্ণসংখ্যা);

#ifdef HAVE_OPENCL

স্থির যৌক্তিক ocl_norm( প্রবেশ_Array _src, পূর্ণসংখ্যা normType, প্রবেশ_Array _mask, দ্বিগুণ & ফলাফল )
{
    ধ্রুবক ocl::Device & d = ocl::Device::getDefault();

#ifdef __ANDROID__
    যদি (d.isNVidia())
        ফেরত মিথ্যা;
#endif
    ধ্রুবক পূর্ণসংখ্যা cn = _src.channels();
    যদি (cn > 4)
        ফেরত মিথ্যা;
    পূর্ণসংখ্যা type = _src.type(), depth = CV_MAT_DEPTH(type);
    যৌক্তিক doubleSupport = d.doubleFPConfig() > 0,
            haveMask = _mask.kind() != প্রবেশ_Array_2::NONE;

    যদি (depth >= CV_16F)
        ফেরত মিথ্যা;  // TODO: support FP16

    যদি ( !(normType == NORM_INF || normType == NORM_L1 || normType == NORM_L2 || normType == NORM_L2SQR) ||
         (!doubleSupport && depth == CV_64F))
        ফেরত মিথ্যা;

    UMat src = _src.getUMat();

    যদি (normType == NORM_INF)
    {
        যদি (!ocl_minMaxIdx(_src, ফাঁকা, &ফলাফল, ফাঁকা, ফাঁকা, _mask,
                           std::max(depth, CV_32S), depth != CV_8U && depth != CV_16U))
            ফেরত মিথ্যা;
    }
    অন্যথা যদি (normType == NORM_L1 || normType == NORM_L2 || normType == NORM_L2SQR)
    {
        Scalar sc;
        যৌক্তিক unstype = depth == CV_8U || depth == CV_16U;

        যদি ( !ocl_যোগ(haveMask ? src : src.reshape(1), sc, normType == NORM_L2 || normType == NORM_L2SQR ?
                    OCL_OP_যোগ_SQR : (unstype ? OCL_OP_যোগ : OCL_OP_যোগ_ABS), _mask) )
            ফেরত মিথ্যা;

        দ্বিগুণ s = 0.0;
        জন্য (পূর্ণসংখ্যা i = 0; i < (haveMask ? cn : 1); ++i)
            s += sc[i];

        ফলাফল = normType == NORM_L1 || normType == NORM_L2SQR ? s : std::sqrt(s);
    }

    ফেরত সত্য;
}

#endif

স্থির NormFunc getNormFunc(পূর্ণসংখ্যা normType, পূর্ণসংখ্যা depth) {
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getNormFunc, (normType, depth), CV_CPU_DISPATCH_MODES_ALL);
}
স্থির NormDiffFunc getNormDiffFunc(পূর্ণসংখ্যা normType, পূর্ণসংখ্যা depth) {
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getNormDiffFunc, (normType, depth), CV_CPU_DISPATCH_MODES_ALL);
}

দ্বিগুণ norm( প্রবেশ_Array _src, পূর্ণসংখ্যা normType, প্রবেশ_Array _mask )
{
    CV_INSTRUMENT_REGION();

    normType &= NORM_TYPE_MASK;
    CV_Assert( normType == NORM_INF || normType == NORM_L1 ||
               normType == NORM_L2 || normType == NORM_L2SQR ||
               ((normType == NORM_HAMMING || normType == NORM_HAMMING2) && _src.type() == CV_8U) );

#যদি defined HAVE_OPENCL
    দ্বিগুণ ফলাফল_2 = 0;
#endif

#ifdef HAVE_OPENCL
    CV_OCL_RUN_(OCL_PERFORMANCE_CHECK(_src.isUMat()) && _src.dims() <= 2,
                ocl_norm(_src, normType, _mask, ফলাফল_2),
                ফলাফল_2)
#endif

    Mat src = _src.getMat(), mask = _mask.getMat();
    CV_Assert( mask.empty() || ((mask.type() == CV_8U || mask.type() == CV_8S || mask.type() == CV_Bool) && src.size == mask.size) );
    যদি( src.empty() )
        ফেরত 0.0;
    পূর্ণসংখ্যা depth = src.depth(), cn = src.channels();
    যদি( src.dims <= 2 )
    {
        দ্বিগুণ ফলাফল;
        CALL_HAL_RET(norm, cv_hal_norm, ফলাফল, src.তথ্য, src.step, mask.তথ্য, mask.step, src.cols, src.rows, src.type(), normType);
    }
    অন্যথা যদি( src.isContinuous() && mask.isContinuous() )
    {
        দ্বিগুণ ফলাফল;
        CALL_HAL_RET(norm, cv_hal_norm, ফলাফল, src.তথ্য, 0, mask.তথ্য, 0, (পূর্ণসংখ্যা)src.যোগ(), 1, src.type(), normType);
    }

    NormFunc func = getNormFunc(normType >> 1, depth);
    CV_Assert( (normType >> 1) >= 3 || func != 0 );

    যদি( src.isContinuous() && mask.empty() )
    {
        size_t len = src.যোগ()*cn;
        যদি( len == (size_t)(পূর্ণসংখ্যা)len )
        {
            যদি( depth == CV_32F )
            {
                ধ্রুবক uchar* তথ্য = src.ptr<ধ্রুবক uchar>();

                যদি( normType == NORM_L2 || normType == NORM_L2SQR || normType == NORM_L1 )
                {
                    দ্বিগুণ ফলাফল = 0;
                    func(তথ্য, 0, (uchar*)&ফলাফল, (পূর্ণসংখ্যা)len, 1);
                    ফেরত normType == NORM_L2 ? std::sqrt(ফলাফল) : ফলাফল;
                }
                যদি( normType == NORM_INF )
                {
                    দশমিক ফলাফল = 0;
                    func(তথ্য, 0, (uchar*)&ফলাফল, (পূর্ণসংখ্যা)len, 1);
                    ফেরত ফলাফল;
                }
            }
            যদি( depth == CV_8U )
            {
                ধ্রুবক uchar* তথ্য = src.ptr<uchar>();

                যদি( normType == NORM_HAMMING )
                {
                    ফেরত hal::normHamming(তথ্য, (পূর্ণসংখ্যা)len, 1);
                }

                যদি( normType == NORM_HAMMING2 )
                {
                    ফেরত hal::normHamming(তথ্য, (পূর্ণসংখ্যা)len, 2);
                }
            }
        }
    }

    CV_Assert( mask.empty() || mask.type() == CV_8U || mask.type() == CV_8S || mask.type() == CV_Bool );

    যদি( normType == NORM_HAMMING || normType == NORM_HAMMING2 )
    {
        যদি( !mask.empty() )
        {
            Mat অস্থায়ী_2;
            bitwise_and(src, mask, অস্থায়ী_2);
            ফেরত norm(অস্থায়ী_2, normType);
        }
        পূর্ণসংখ্যা cell_আকার = normType == NORM_HAMMING ? 1 : 2;

        ধ্রুবক Mat* arrays[] = {&src, 0};
        uchar* ptrs[1] = {};
        NAryMatIterator it(arrays, ptrs);
        পূর্ণসংখ্যা যোগ = (পূর্ণসংখ্যা)it.size;
        পূর্ণসংখ্যা ফলাফল = 0;

        জন্য( size_t i = 0; i < it.nplanes; i++, ++it )
        {
            ফলাফল += hal::normHamming(ptrs[0], যোগ, cell_আকার);
        }

        ফেরত ফলাফল;
    }

    ধ্রুবক Mat* arrays[] = {&src, &mask, 0};
    uchar* ptrs[2] = {};
    সংঘ
    {
        দ্বিগুণ d;
        unsigned u;
        uint64 UL;
        দশমিক f;
    }
    ফলাফল;
    ফলাফল.d = 0;
    NAryMatIterator it(arrays, ptrs);
    CV_CheckLT((size_t)it.size, (size_t)INT_MAX, "");
    যৌক্তিক is_fp16 = depth == CV_16F || depth == CV_16BF;

    যদি ((normType == NORM_L1 && (depth <= CV_16S || depth == CV_Bool || is_fp16)) ||
        ((normType == NORM_L2 || normType == NORM_L2SQR) && (depth <= CV_8S || depth == CV_Bool || is_fp16)))
    {
        // special case to handle "integer" overflow in accumulator
        ধ্রুবক size_t esz = src.elem_আকার();
        ধ্রুবক পূর্ণসংখ্যা যোগ = (পূর্ণসংখ্যা)it.size;
        ধ্রুবক পূর্ণসংখ্যা blockSize0 = (is_fp16 ? (1 << 10) :
            depth == CV_Bool ? (1 << 30) :
            normType == NORM_L1 && depth <= CV_8S ? (1 << 23) : (1 << 15))/cn;
        ধ্রুবক পূর্ণসংখ্যা block_আকার = std::min(যোগ, blockSize0);
        সংঘ {
            পূর্ণসংখ্যা i;
            দশমিক f;
        } blocksum;
        blocksum.i = 0;
        পূর্ণসংখ্যা গুনতি_2 = 0;

        জন্য (size_t i = 0; i < it.nplanes; i++, ++it)
        {
            জন্য (পূর্ণসংখ্যা j = 0; j < যোগ; j += block_আকার)
            {
                পূর্ণসংখ্যা bsz = std::min(যোগ - j, block_আকার);
                func(ptrs[0], ptrs[1], (uchar*)&blocksum, bsz, cn);
                গুনতি_2 += bsz;
                যদি (গুনতি_2 + block_আকার >= blockSize0 || (i+1 >= it.nplanes && j+bsz >= যোগ))
                {
                    ফলাফল.d += is_fp16 ? (দ্বিগুণ)blocksum.f : (দ্বিগুণ)blocksum.i;
                    blocksum.i = 0;
                    গুনতি_2 = 0;
                }
                ptrs[0] += bsz*esz;
                যদি (ptrs[1])
                    ptrs[1] += bsz;
            }
        }
    }
    অন্যথা
    {
        // generic implementation
        জন্য (size_t i = 0; i < it.nplanes; i++, ++it)
        {
            func(ptrs[0], ptrs[1], (uchar*)&ফলাফল, (পূর্ণসংখ্যা)it.size, cn);
        }
    }

    যদি( normType == NORM_INF )
    {
        যদি(depth <= CV_32S || depth == CV_32U || depth == CV_Bool)
            ফেরত ফলাফল.u;
        যদি (depth == CV_32F || is_fp16)
            ফেরত ফলাফল.f;
        যদি (depth == CV_64U || depth == CV_64S)
            ফেরত (দ্বিগুণ)ফলাফল.UL;
    }
    যদি( normType == NORM_L2 )
        ফেরত std::sqrt(ফলাফল.d);

    ফেরত ফলাফল.d;
}

//==================================================================================================

#ifdef HAVE_OPENCL
স্থির যৌক্তিক ocl_norm( প্রবেশ_Array _src1, প্রবেশ_Array _src2, পূর্ণসংখ্যা normType, প্রবেশ_Array _mask, দ্বিগুণ & ফলাফল )
{
#ifdef __ANDROID__
    যদি (ocl::Device::getDefault().isNVidia())
        ফেরত মিথ্যা;
#endif

    Scalar sc1, sc2;
    পূর্ণসংখ্যা cn = _src1.channels();
    যদি (cn > 4)
        ফেরত মিথ্যা;
    পূর্ণসংখ্যা type = _src1.type(), depth = CV_MAT_DEPTH(type);
    যৌক্তিক relative = (normType & NORM_RELATIVE) != 0;
    normType &= ~NORM_RELATIVE;
    যৌক্তিক normsum = normType == NORM_L1 || normType == NORM_L2 || normType == NORM_L2SQR;

#ifdef __APPLE__
    যদি(normType == NORM_L1 && type == CV_16UC3 && !_mask.empty())
        ফেরত মিথ্যা;
#endif

    যদি (normsum)
    {
        যদি (!ocl_যোগ(_src1, sc1, normType == NORM_L2 || normType == NORM_L2SQR ?
                     OCL_OP_যোগ_SQR : OCL_OP_যোগ, _mask, _src2, relative, sc2))
            ফেরত মিথ্যা;
    }
    অন্যথা
    {
        যদি (!ocl_minMaxIdx(_src1, ফাঁকা, &sc1[0], ফাঁকা, ফাঁকা, _mask, std::max(CV_32S, depth),
                           মিথ্যা, _src2, relative ? &sc2[0] : ফাঁকা))
            ফেরত মিথ্যা;
        cn = 1;
    }

    দ্বিগুণ s2 = 0;
    জন্য (পূর্ণসংখ্যা i = 0; i < cn; ++i)
    {
        ফলাফল += sc1[i];
        যদি (relative)
            s2 += sc2[i];
    }

    যদি (normType == NORM_L2)
    {
        ফলাফল = std::sqrt(ফলাফল);
        যদি (relative)
            s2 = std::sqrt(s2);
    }

    যদি (relative)
        ফলাফল /= (s2 + DBL_EPSILON);

    ফেরত সত্য;
}  // ocl_norm()
#endif  // HAVE_OPENCL

দ্বিগুণ norm( প্রবেশ_Array _src1, প্রবেশ_Array _src2, পূর্ণসংখ্যা normType, প্রবেশ_Array _mask )
{
    CV_INSTRUMENT_REGION();

    CV_CheckTypeEQ(_src1.type(), _src2.type(), "Input type mismatch");
    CV_Assert(_src1.same_আকার(_src2));

#যদি defined HAVE_OPENCL
    দ্বিগুণ ফলাফল_2 = 0;
#endif

#ifdef HAVE_OPENCL
    CV_OCL_RUN_(OCL_PERFORMANCE_CHECK(_src1.isUMat()),
                ocl_norm(_src1, _src2, normType, _mask, ফলাফল_2),
                ফলাফল_2)
#endif

    Mat src1 = _src1.getMat(), src2 = _src2.getMat(), mask = _mask.getMat();
    CV_Assert( mask.empty() || ((mask.type() == CV_8U || mask.type() == CV_8S || mask.type() == CV_Bool) && src1.size == mask.size) );
    যদি( src1.empty() || src2.empty() )
        ফেরত 0.0;
    পূর্ণসংখ্যা depth = src1.depth(), cn = src1.channels();
    যদি( src1.dims <= 2 )
    {
        দ্বিগুণ ফলাফল;
        CALL_HAL_RET(normDiff, cv_hal_normDiff, ফলাফল, src1.তথ্য, src1.step, src2.তথ্য, src2.step, mask.তথ্য, mask.step, src1.cols, src1.rows, src1.type(), normType);
    }
    অন্যথা যদি( src1.isContinuous() && src2.isContinuous() && mask.isContinuous() )
    {
        দ্বিগুণ ফলাফল;
        CALL_HAL_RET(normDiff, cv_hal_normDiff, ফলাফল, src1.তথ্য, 0, src2.তথ্য, 0, mask.তথ্য, 0, (পূর্ণসংখ্যা)src1.যোগ(), 1, src1.type(), normType);
    }

    যদি( normType & NORM_RELATIVE )
    {
        ফেরত norm(_src1, _src2, normType & ~NORM_RELATIVE, _mask)/(norm(_src2, normType, _mask) + DBL_EPSILON);
    }

    normType &= 7;
    CV_Assert( normType == NORM_INF || normType == NORM_L1 ||
               normType == NORM_L2 || normType == NORM_L2SQR ||
              ((normType == NORM_HAMMING || normType == NORM_HAMMING2) && src1.type() == CV_8U) );

    NormDiffFunc func = getNormDiffFunc(normType >> 1, depth);
    CV_Assert( (normType >> 1) >= 3 || func != 0 );

    যদি( src1.isContinuous() && src2.isContinuous() && mask.empty() )
    {
        size_t len = src1.যোগ()*src1.channels();
        যদি( len == (size_t)(পূর্ণসংখ্যা)len )
        {
            যদি( src1.depth() == CV_32F )
            {
                ধ্রুবক uchar* data1 = src1.ptr<ধ্রুবক uchar>();
                ধ্রুবক uchar* data2 = src2.ptr<ধ্রুবক uchar>();

                যদি( normType == NORM_L2 || normType == NORM_L2SQR || normType == NORM_L1 )
                {
                    দ্বিগুণ ফলাফল = 0;
                    func(data1, data2, 0, (uchar*)&ফলাফল, (পূর্ণসংখ্যা)len, 1);
                    ফেরত normType == NORM_L2 ? std::sqrt(ফলাফল) : ফলাফল;
                }
                যদি( normType == NORM_INF )
                {
                    দশমিক ফলাফল = 0;
                    func(data1, data2, 0, (uchar*)&ফলাফল, (পূর্ণসংখ্যা)len, 1);
                    ফেরত ফলাফল;
                }
            }
        }
    }

    CV_Assert( mask.empty() || mask.type() == CV_8U || mask.type() == CV_8S || mask.type() == CV_Bool );

    যদি( normType == NORM_HAMMING || normType == NORM_HAMMING2 )
    {
        যদি( !mask.empty() )
        {
            Mat অস্থায়ী_2;
            bitwise_xor(src1, src2, অস্থায়ী_2);
            bitwise_and(অস্থায়ী_2, mask, অস্থায়ী_2);
            ফেরত norm(অস্থায়ী_2, normType);
        }
        পূর্ণসংখ্যা cell_আকার = normType == NORM_HAMMING ? 1 : 2;

        ধ্রুবক Mat* arrays[] = {&src1, &src2, 0};
        uchar* ptrs[2] = {};
        NAryMatIterator it(arrays, ptrs);
        পূর্ণসংখ্যা যোগ = (পূর্ণসংখ্যা)it.size;
        পূর্ণসংখ্যা ফলাফল = 0;

        জন্য( size_t i = 0; i < it.nplanes; i++, ++it )
        {
            ফলাফল += hal::normHamming(ptrs[0], ptrs[1], যোগ, cell_আকার);
        }

        ফেরত ফলাফল;
    }

    ধ্রুবক Mat* arrays[] = {&src1, &src2, &mask, 0};
    uchar* ptrs[3] = {};
    সংঘ
    {
        দ্বিগুণ d;
        দশমিক f;
        unsigned u;
        uint64 UL;
    }
    ফলাফল;
    ফলাফল.d = 0;
    NAryMatIterator it(arrays, ptrs);
    CV_CheckLT((size_t)it.size, (size_t)INT_MAX, "");

    যৌক্তিক is_fp16 = depth == CV_16F || depth == CV_16BF;

    যদি ((normType == NORM_L1 && (depth <= CV_16S || depth == CV_Bool || is_fp16)) ||
        ((normType == NORM_L2 || normType == NORM_L2SQR) && (depth <= CV_8S || depth == CV_Bool || is_fp16)))
    {
        // special case to handle "integer" overflow in accumulator
        ধ্রুবক size_t esz = src1.elem_আকার();
        ধ্রুবক পূর্ণসংখ্যা যোগ = (পূর্ণসংখ্যা)it.size;
        ধ্রুবক পূর্ণসংখ্যা blockSize0 = (is_fp16 ? (1 << 10) :
            depth == CV_Bool ? (1 << 30) :
            normType == NORM_L1 && depth <= CV_8S ? (1 << 23) : (1 << 15))/cn;
        ধ্রুবক পূর্ণসংখ্যা block_আকার = std::min(যোগ, blockSize0);
        সংঘ {
            পূর্ণসংখ্যা i;
            দশমিক f;
        } blocksum;
        blocksum.i = 0;
        পূর্ণসংখ্যা গুনতি_2 = 0;

        জন্য (size_t i = 0; i < it.nplanes; i++, ++it)
        {
            জন্য (পূর্ণসংখ্যা j = 0; j < যোগ; j += block_আকার)
            {
                পূর্ণসংখ্যা bsz = std::min(যোগ - j, block_আকার);
                func(ptrs[0], ptrs[1], ptrs[2], (uchar*)&blocksum, bsz, cn);
                গুনতি_2 += bsz;
                যদি (গুনতি_2 + block_আকার >= blockSize0 || (i+1 >= it.nplanes && j+bsz >= যোগ))
                {
                    ফলাফল.d += is_fp16 ? (দ্বিগুণ)blocksum.f : (দ্বিগুণ)blocksum.i;
                    blocksum.i = 0;
                    গুনতি_2 = 0;
                }
                ptrs[0] += bsz*esz;
                ptrs[1] += bsz*esz;
                যদি (ptrs[2])
                    ptrs[2] += bsz;
            }
        }
    }
    অন্যথা
    {
        // generic implementation
        জন্য (size_t i = 0; i < it.nplanes; i++, ++it)
        {
            func(ptrs[0], ptrs[1], ptrs[2], (uchar*)&ফলাফল, (পূর্ণসংখ্যা)it.size, cn);
        }
    }

    যদি( normType == NORM_INF )
    {
        যদি (depth <= CV_32S || depth == CV_32U || depth == CV_Bool)
            ফেরত ফলাফল.u;
        যদি (depth == CV_32F || is_fp16)
            ফেরত ফলাফল.f;
        যদি (depth == CV_64U || depth == CV_64S)
            ফেরত (দ্বিগুণ)ফলাফল.UL;
    }
    যদি( normType == NORM_L2 )
        ফেরত std::sqrt(ফলাফল.d);

    ফেরত ফলাফল.d;
}

cv::Hamming::ফলাফল_Type Hamming::operator()( ধ্রুবক unsigned অক্ষর* a, ধ্রুবক unsigned অক্ষর* b, পূর্ণসংখ্যা size ) ধ্রুবক
{
    ফেরত cv::hal::normHamming(a, b, size);
}

দ্বিগুণ PSNR(প্রবেশ_Array _src1, প্রবেশ_Array _src2, দ্বিগুণ R)
{
    CV_INSTRUMENT_REGION();

    //Input arrays must have depth CV_8U
    CV_Assert( _src1.type() == _src2.type() );

    দ্বিগুণ diff = std::sqrt(norm(_src1, _src2, NORM_L2SQR)/(_src1.যোগ()*_src1.channels()));
    ফেরত 20*log10(R/(diff+DBL_EPSILON));
}


#ifdef HAVE_OPENCL
স্থির যৌক্তিক ocl_normalize( প্রবেশ_Array _src, প্রবেশ_Output_Array _dst, প্রবেশ_Array _mask, পূর্ণসংখ্যা dtype,
                           দ্বিগুণ scale, দ্বিগুণ delta )
{
    UMat src = _src.getUMat();

    যদি( _mask.empty() )
        src.convertTo( _dst, dtype, scale, delta );
    অন্যথা যদি (src.channels() <= 4)
    {
        ধ্রুবক ocl::Device & dev = ocl::Device::getDefault();

        পূর্ণসংখ্যা stype = _src.type(), sdepth = CV_MAT_DEPTH(stype), cn = CV_MAT_CN(stype),
                ddepth = CV_MAT_DEPTH(dtype), wdepth = std::max(CV_32F, std::max(sdepth, ddepth)),
                rowsPerWI = dev.isIntel() ? 4 : 1;

        দশমিক fscale = static_cast<দশমিক>(scale), fdelta = static_cast<দশমিক>(delta);
        যৌক্তিক haveScale = std::fabs(scale - 1) > DBL_EPSILON,
                haveZeroScale = !(std::fabs(scale) > DBL_EPSILON),
                haveDelta = std::fabs(delta) > DBL_EPSILON,
                doubleSupport = dev.doubleFPConfig() > 0;

        যদি (!haveScale && !haveDelta && stype == dtype)
        {
            _src.copyTo(_dst, _mask);
            ফেরত সত্য;
        }
        যদি (haveZeroScale)
        {
            _dst.setTo(Scalar(delta), _mask);
            ফেরত সত্য;
        }

        যদি ((sdepth == CV_64F || ddepth == CV_64F) && !doubleSupport)
            ফেরত মিথ্যা;

        অক্ষর cvt[2][50];
        String opts = format("-D srcT=%s -D dstT=%s -D convertToWT=%s -D cn=%d -D rowsPerWI=%d"
                             " -D convertToDT=%s -D workT=%s%s%s%s -D srcT1=%s -D dstT1=%s",
                             ocl::typeToStr(stype), ocl::typeToStr(dtype),
                             ocl::convertTypeStr(sdepth, wdepth, cn, cvt[0], আকার(cvt[0])), cn,
                             rowsPerWI, ocl::convertTypeStr(wdepth, ddepth, cn, cvt[1], আকার(cvt[1])),
                             ocl::typeToStr(CV_MAKE_TYPE(wdepth, cn)),
                             doubleSupport ? " -D DOUBLE_SUPPORT" : "",
                             haveScale ? " -D HAVE_SCALE" : "",
                             haveDelta ? " -D HAVE_DELTA" : "",
                             ocl::typeToStr(sdepth), ocl::typeToStr(ddepth));

        ocl::Kernel k("normalizek", ocl::core::normalize_oclsrc, opts);
        যদি (k.empty())
            ফেরত মিথ্যা;

        UMat mask = _mask.getUMat(), dst = _dst.getUMat();

        ocl::KernelArg srcarg = ocl::KernelArg::Read_Only_No_আকার(src),
                maskarg = ocl::KernelArg::Read_Only_No_আকার(mask),
                dstarg = ocl::KernelArg::ReadWrite(dst);

        যদি (haveScale)
        {
            যদি (haveDelta)
                k.args(srcarg, maskarg, dstarg, fscale, fdelta);
            অন্যথা
                k.args(srcarg, maskarg, dstarg, fscale);
        }
        অন্যথা
        {
            যদি (haveDelta)
                k.args(srcarg, maskarg, dstarg, fdelta);
            অন্যথা
                k.args(srcarg, maskarg, dstarg);
        }

        size_t globalsize[2] = { (size_t)src.cols, ((size_t)src.rows + rowsPerWI - 1) / rowsPerWI };
        ফেরত k.run(2, globalsize, ফাঁকা, মিথ্যা);
    }
    অন্যথা
    {
        UMat অস্থায়ী_2;
        src.convertTo( অস্থায়ী_2, dtype, scale, delta );
        অস্থায়ী_2.copyTo( _dst, _mask );
    }

    ফেরত সত্য;
}  // ocl_normalize
#endif  // HAVE_OPENCL

শূন্য normalize(প্রবেশ_Array _src, প্রবেশ_Output_Array _dst, দ্বিগুণ a, দ্বিগুণ b,
               পূর্ণসংখ্যা norm_type, পূর্ণসংখ্যা rtype, প্রবেশ_Array _mask)
{
    CV_INSTRUMENT_REGION();

    দ্বিগুণ scale = 1, shift = 0;
    পূর্ণসংখ্যা type = _src.type(), depth = CV_MAT_DEPTH(type);

    যদি( rtype < 0 )
        rtype = _dst.fixedType() ? _dst.depth() : depth;

    যদি( norm_type == NORM_MINMAX )
    {
        দ্বিগুণ smin = 0, smax = 0;
        দ্বিগুণ dmin = MIN( a, b ), dmax = MAX( a, b );
        minMaxIdx( _src, &smin, &smax, 0, 0, _mask );
        scale = (dmax - dmin)*(smax - smin > DBL_EPSILON ? 1./(smax - smin) : 0);
        যদি( rtype == CV_32F )
        {
            scale = (দশমিক)scale;
            shift = (দশমিক)dmin - (দশমিক)(smin*scale);
        }
        অন্যথা
            shift = dmin - smin*scale;
    }
    অন্যথা যদি( norm_type == NORM_L2 || norm_type == NORM_L1 || norm_type == NORM_INF )
    {
        scale = norm( _src, norm_type, _mask );
        scale = scale > DBL_EPSILON ? a/scale : 0.;
        shift = 0;
    }
    অন্যথা
        CV_Error( cv::Error::StsBadArg, "Unknown/unsupported norm type" );

    CV_OCL_RUN(_dst.isUMat(),
               ocl_normalize(_src, _dst, _mask, rtype, scale, shift))

    Mat src = _src.getMat();
    যদি( _mask.empty() )
        src.convertTo( _dst, rtype, scale, shift );
    অন্যথা
    {
        Mat অস্থায়ী_2;
        src.convertTo( অস্থায়ী_2, rtype, scale, shift );
        অস্থায়ী_2.copyTo( _dst, _mask );
    }
}

}  // namespace
