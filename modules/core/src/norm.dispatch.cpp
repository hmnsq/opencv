// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html
//
// Copyright (C) 2026, Advanced Micro Devices, Inc., all rights reserved.


#অন্তর্ভাৱ "precomp.hpp"
#অন্তর্ভাৱ "opencl_kernels_core.hpp"
#অন্তর্ভাৱ "stat.hpp"

#অন্তর্ভাৱ "norm.simd.hpp"
#অন্তর্ভাৱ "norm.simd_declarations.hpp"

/****************************************************************************************\
*                                         norm                                           *
\****************************************************************************************/

নামাকাশ cv { নামাকাশ hal {

বাহ্য ধ্রুৱ uchar বেৰ_কৰো_গুনতি_Table[256] =
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

স্থির ধ্রুৱ uchar বেৰ_কৰো_গুনতি_Table2[] =
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

স্থির ধ্রুৱ uchar বেৰ_কৰো_গুনতি_Table4[] =
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


পূর্ণাঙ্ক normHamming(ধ্রুৱ uchar* a, পূর্ণাঙ্ক n, পূর্ণাঙ্ক cellSize)
{
    পূর্ণাঙ্ক output;
    CALL_HAL_RET(normHamming8u, cv_hal_normHamming8u, output, a, n, cellSize);

    যদি( cellSize == 1 )
        ঘূৰাই normHamming(a, n);
    ধ্রুৱ uchar* tab = 0;
    যদি( cellSize == 2 )
        tab = বেৰ_কৰো_গুনতি_Table2;
    অন্যথা যদি( cellSize == 4 )
        tab = বেৰ_কৰো_গুনতি_Table4;
    অন্যথা
        ঘূৰাই -1;
    পূর্ণাঙ্ক i = 0;
    পূর্ণাঙ্ক result = 0;
#যদি (CV_SIMD || CV_SIMD_SCALABLE)
    v_uint64 t = vx_setzero_u64();
    যদি ( cellSize == 2)
    {
        v_uint16 mask = v_reinterpret_as_u16(vx_setall_u8(0x55));
        বাবে(; i <= n - VTraits<v_uint8>::vlanes(); i += VTraits<v_uint8>::vlanes())
        {
            v_uint16 a0 = v_reinterpret_as_u16(vx_load(a + i));
            t = v_add(t, v_popcount(v_reinterpret_as_u64(v_and(v_or(a0, v_shr<1>(a0)), mask))));
        }
    }
    অন্যথা    // cellSize == 4
    {
        v_uint16 mask = v_reinterpret_as_u16(vx_setall_u8(0x11));
        বাবে(; i <= n - VTraits<v_uint8>::vlanes(); i += VTraits<v_uint8>::vlanes())
        {
            v_uint16 a0 = v_reinterpret_as_u16(vx_load(a + i));
            v_uint16 a1 = v_or(a0, v_shr<2>(a0));
            t = v_add(t, v_popcount(v_reinterpret_as_u64(v_and(v_or(a1, v_shr<1>(a1)), mask))));

        }
    }
    result += (পূর্ণাঙ্ক)v_reduce_sum(t);
    vx_cleanup();
#নোচেদ্যদি CV_ENABLE_UNROLLED
    বাবে( ; i <= n - 4; i += 4 )
        result += tab[a[i]] + tab[a[i+1]] + tab[a[i+2]] + tab[a[i+3]];
#endif
    বাবে( ; i < n; i++ )
        result += tab[a[i]];
    ঘূৰাই result;
}

পূর্ণাঙ্ক normHamming(ধ্রুৱ uchar* a, ধ্রুৱ uchar* b, পূর্ণাঙ্ক n, পূর্ণাঙ্ক cellSize)
{
    পূর্ণাঙ্ক output;
    CALL_HAL_RET(normHammingDiff8u, cv_hal_normHammingDiff8u, output, a, b, n, cellSize);

    যদি( cellSize == 1 )
        ঘূৰাই normHamming(a, b, n);
    ধ্রুৱ uchar* tab = 0;
    যদি( cellSize == 2 )
        tab = বেৰ_কৰো_গুনতি_Table2;
    অন্যথা যদি( cellSize == 4 )
        tab = বেৰ_কৰো_গুনতি_Table4;
    অন্যথা
        ঘূৰাই -1;
    পূর্ণাঙ্ক i = 0;
    পূর্ণাঙ্ক result = 0;
#যদি (CV_SIMD || CV_SIMD_SCALABLE)
    v_uint64 t = vx_setzero_u64();
    যদি ( cellSize == 2)
    {
        v_uint16 mask = v_reinterpret_as_u16(vx_setall_u8(0x55));
        বাবে(; i <= n - VTraits<v_uint8>::vlanes(); i += VTraits<v_uint8>::vlanes())
        {
            v_uint16 ab0 = v_reinterpret_as_u16(v_xor(vx_load(a + i), vx_load(b + i)));
            t = v_add(t, v_popcount(v_reinterpret_as_u64(v_and(v_or(ab0, v_shr<1>(ab0)), mask))));
        }
    }
    অন্যথা    // cellSize == 4
    {
        v_uint16 mask = v_reinterpret_as_u16(vx_setall_u8(0x11));
        বাবে(; i <= n - VTraits<v_uint8>::vlanes(); i += VTraits<v_uint8>::vlanes())
        {
            v_uint16 ab0 = v_reinterpret_as_u16(v_xor(vx_load(a + i), vx_load(b + i)));
            v_uint16 ab1 = v_or(ab0, v_shr<2>(ab0));
            t = v_add(t, v_popcount(v_reinterpret_as_u64(v_and(v_or(ab1, v_shr<1>(ab1)), mask))));
        }
    }
    result += (পূর্ণাঙ্ক)v_reduce_sum(t);
    vx_cleanup();
#নোচেদ্যদি CV_ENABLE_UNROLLED
    বাবে( ; i <= n - 4; i += 4 )
        result += tab[a[i] ^ b[i]] + tab[a[i+1] ^ b[i+1]] +
                tab[a[i+2] ^ b[i+2]] + tab[a[i+3] ^ b[i+3]];
#endif
    বাবে( ; i < n; i++ )
        result += tab[a[i] ^ b[i]];
    ঘূৰাই result;
}

ভিন্ন normL2Sqr_(ধ্রুৱ ভিন্ন* a, ধ্রুৱ ভিন্ন* b, পূর্ণাঙ্ক n)
{
    পূর্ণাঙ্ক j = 0; ভিন্ন d = 0.f;
#যদি (CV_SIMD || CV_SIMD_SCALABLE)
    ধ্রুৱ পূর্ণাঙ্ক vl = VTraits<v_float32>::vlanes();
    v_float32 v_d0 = vx_setzero_f32(), v_d1 = vx_setzero_f32();
    v_float32 v_d2 = vx_setzero_f32(), v_d3 = vx_setzero_f32();
    বাবে (; j <= n - 4 * vl; j += 4 * vl)
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
    বাবে (; j <= n - vl; j += vl)
    {
        v_float32 t0 = v_sub(vx_load(a + j), vx_load(b + j));
        v_d0 = v_muladd(t0, t0, v_d0);
    }
    d = v_reduce_sum(v_d0);
#endif
    বাবে( ; j < n; j++ )
    {
        ভিন্ন t = a[j] - b[j];
        d += t*t;
    }
    ঘূৰাই d;
}


ভিন্ন normL1_(ধ্রুৱ ভিন্ন* a, ধ্রুৱ ভিন্ন* b, পূর্ণাঙ্ক n)
{
    পূর্ণাঙ্ক j = 0; ভিন্ন d = 0.f;
#যদি (CV_SIMD || CV_SIMD_SCALABLE)
    ধ্রুৱ পূর্ণাঙ্ক vl = VTraits<v_float32>::vlanes();
    v_float32 v_d0 = vx_setzero_f32(), v_d1 = vx_setzero_f32();
    v_float32 v_d2 = vx_setzero_f32(), v_d3 = vx_setzero_f32();
    বাবে (; j <= n - 4 * vl; j += 4 * vl)
    {
        v_d0 = v_add(v_d0, v_absdiff(vx_load(a + j), vx_load(b + j)));
        v_d1 = v_add(v_d1, v_absdiff(vx_load(a + j + vl), vx_load(b + j + vl)));
        v_d2 = v_add(v_d2, v_absdiff(vx_load(a + j + 2 * vl), vx_load(b + j + 2 * vl)));
        v_d3 = v_add(v_d3, v_absdiff(vx_load(a + j + 3 * vl), vx_load(b + j + 3 * vl)));
    }
    v_d0 = v_add(v_add(v_d0, v_d1), v_add(v_d2, v_d3));
    বাবে (; j <= n - vl; j += vl)
        v_d0 = v_add(v_d0, v_absdiff(vx_load(a + j), vx_load(b + j)));
    d = v_reduce_sum(v_d0);
#endif
    বাবে( ; j < n; j++ )
        d += std::abs(a[j] - b[j]);
    ঘূৰাই d;
}

পূর্ণাঙ্ক normL1_(ধ্রুৱ uchar* a, ধ্রুৱ uchar* b, পূর্ণাঙ্ক n)
{
    পূর্ণাঙ্ক j = 0, d = 0;
#যদি (CV_SIMD || CV_SIMD_SCALABLE)
    ধ্রুৱ পূর্ণাঙ্ক vl = VTraits<v_uint8>::vlanes();
    বাবে (; j <= n - 4 * vl; j += 4 * vl)
        d += v_reduce_sad(vx_load(a + j), vx_load(b + j)) +
             v_reduce_sad(vx_load(a + j + vl), vx_load(b + j + vl)) +
             v_reduce_sad(vx_load(a + j + 2 * vl), vx_load(b + j + 2 * vl)) +
             v_reduce_sad(vx_load(a + j + 3 * vl), vx_load(b + j + 3 * vl));
    বাবে (; j <= n - vl; j += vl)
        d += v_reduce_sad(vx_load(a + j), vx_load(b + j));
#endif
    বাবে( ; j < n; j++ )
        d += std::abs(a[j] - b[j]);
    ঘূৰাই d;
}

} //cv::hal

//==================================================================================================

প্রকারনির্দেশ পূর্ণাঙ্ক (*NormFunc)(ধ্রুৱ uchar*, ধ্রুৱ uchar*, uchar*, পূর্ণাঙ্ক, পূর্ণাঙ্ক);
প্রকারনির্দেশ পূর্ণাঙ্ক (*NormDiffFunc)(ধ্রুৱ uchar*, ধ্রুৱ uchar*, ধ্রুৱ uchar*, uchar*, পূর্ণাঙ্ক, পূর্ণাঙ্ক);

#ifdef HAVE_OPENCL

স্থির তার্কিক ocl_norm( প্ৰবেশ_Array _src, পূর্ণাঙ্ক normType, প্ৰবেশ_Array _mask, দ্ৱিগুণ & result )
{
    ধ্রুৱ ocl::Device & d = ocl::Device::getDefault();

#ifdef __ANDROID__
    যদি (d.isNVidia())
        ঘূৰাই মিছা;
#endif
    ধ্রুৱ পূর্ণাঙ্ক cn = _src.channels();
    যদি (cn > 4)
        ঘূৰাই মিছা;
    পূর্ণাঙ্ক type = _src.type(), depth = CV_MAT_DEPTH(type);
    তার্কিক doubleSupport = d.doubleFPConfig() > 0,
            haveMask = _mask.kind() != প্ৰবেশ_Array_2::NONE;

    যদি (depth >= CV_16F)
        ঘূৰাই মিছা;  // TODO: support FP16

    যদি ( !(normType == NORM_INF || normType == NORM_L1 || normType == NORM_L2 || normType == NORM_L2SQR) ||
         (!doubleSupport && depth == CV_64F))
        ঘূৰাই মিছা;

    UMat src = _src.getUMat();

    যদি (normType == NORM_INF)
    {
        যদি (!ocl_minMaxIdx(_src, রিক্ত, &result, রিক্ত, রিক্ত, _mask,
                           std::max(depth, CV_32S), depth != CV_8U && depth != CV_16U))
            ঘূৰাই মিছা;
    }
    অন্যথা যদি (normType == NORM_L1 || normType == NORM_L2 || normType == NORM_L2SQR)
    {
        Scalar sc;
        তার্কিক unstype = depth == CV_8U || depth == CV_16U;

        যদি ( !ocl_sum(haveMask ? src : src.reshape(1), sc, normType == NORM_L2 || normType == NORM_L2SQR ?
                    OCL_OP_SUM_SQR : (unstype ? OCL_OP_SUM : OCL_OP_SUM_ABS), _mask) )
            ঘূৰাই মিছা;

        দ্ৱিগুণ s = 0.0;
        বাবে (পূর্ণাঙ্ক i = 0; i < (haveMask ? cn : 1); ++i)
            s += sc[i];

        result = normType == NORM_L1 || normType == NORM_L2SQR ? s : std::sqrt(s);
    }

    ঘূৰাই সঁচা;
}

#endif

স্থির NormFunc getNormFunc(পূর্ণাঙ্ক normType, পূর্ণাঙ্ক depth) {
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getNormFunc, (normType, depth), CV_CPU_DISPATCH_MODES_ALL);
}
স্থির NormDiffFunc getNormDiffFunc(পূর্ণাঙ্ক normType, পূর্ণাঙ্ক depth) {
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getNormDiffFunc, (normType, depth), CV_CPU_DISPATCH_MODES_ALL);
}

দ্ৱিগুণ norm( প্ৰবেশ_Array _src, পূর্ণাঙ্ক normType, প্ৰবেশ_Array _mask )
{
    CV_INSTRUMENT_REGION();

    normType &= NORM_TYPE_MASK;
    CV_Assert( normType == NORM_INF || normType == NORM_L1 ||
               normType == NORM_L2 || normType == NORM_L2SQR ||
               ((normType == NORM_HAMMING || normType == NORM_HAMMING2) && _src.type() == CV_8U) );

#যদি defined HAVE_OPENCL
    দ্ৱিগুণ _result = 0;
#endif

#ifdef HAVE_OPENCL
    CV_OCL_RUN_(OCL_PERFORMANCE_CHECK(_src.isUMat()) && _src.dims() <= 2,
                ocl_norm(_src, normType, _mask, _result),
                _result)
#endif

    Mat src = _src.getMat(), mask = _mask.getMat();
    CV_Assert( mask.empty() || ((mask.type() == CV_8U || mask.type() == CV_8S || mask.type() == CV_Bool) && src.size == mask.size) );
    যদি( src.empty() )
        ঘূৰাই 0.0;
    পূর্ণাঙ্ক depth = src.depth(), cn = src.channels();
    যদি( src.dims <= 2 )
    {
        দ্ৱিগুণ result;
        CALL_HAL_RET(norm, cv_hal_norm, result, src.data, src.step, mask.data, mask.step, src.cols, src.rows, src.type(), normType);
    }
    অন্যথা যদি( src.isContinuous() && mask.isContinuous() )
    {
        দ্ৱিগুণ result;
        CALL_HAL_RET(norm, cv_hal_norm, result, src.data, 0, mask.data, 0, (পূর্ণাঙ্ক)src.total(), 1, src.type(), normType);
    }

    NormFunc func = getNormFunc(normType >> 1, depth);
    CV_Assert( (normType >> 1) >= 3 || func != 0 );

    যদি( src.isContinuous() && mask.empty() )
    {
        size_t len = src.total()*cn;
        যদি( len == (size_t)(পূর্ণাঙ্ক)len )
        {
            যদি( depth == CV_32F )
            {
                ধ্রুৱ uchar* data = src.ptr<ধ্রুৱ uchar>();

                যদি( normType == NORM_L2 || normType == NORM_L2SQR || normType == NORM_L1 )
                {
                    দ্ৱিগুণ result = 0;
                    func(data, 0, (uchar*)&result, (পূর্ণাঙ্ক)len, 1);
                    ঘূৰাই normType == NORM_L2 ? std::sqrt(result) : result;
                }
                যদি( normType == NORM_INF )
                {
                    ভিন্ন result = 0;
                    func(data, 0, (uchar*)&result, (পূর্ণাঙ্ক)len, 1);
                    ঘূৰাই result;
                }
            }
            যদি( depth == CV_8U )
            {
                ধ্রুৱ uchar* data = src.ptr<uchar>();

                যদি( normType == NORM_HAMMING )
                {
                    ঘূৰাই hal::normHamming(data, (পূর্ণাঙ্ক)len, 1);
                }

                যদি( normType == NORM_HAMMING2 )
                {
                    ঘূৰাই hal::normHamming(data, (পূর্ণাঙ্ক)len, 2);
                }
            }
        }
    }

    CV_Assert( mask.empty() || mask.type() == CV_8U || mask.type() == CV_8S || mask.type() == CV_Bool );

    যদি( normType == NORM_HAMMING || normType == NORM_HAMMING2 )
    {
        যদি( !mask.empty() )
        {
            Mat temp;
            bitwise_and(src, mask, temp);
            ঘূৰাই norm(temp, normType);
        }
        পূর্ণাঙ্ক cellSize = normType == NORM_HAMMING ? 1 : 2;

        ধ্রুৱ Mat* arrays[] = {&src, 0};
        uchar* ptrs[1] = {};
        NAryMatIterator it(arrays, ptrs);
        পূর্ণাঙ্ক total = (পূর্ণাঙ্ক)it.size;
        পূর্ণাঙ্ক result = 0;

        বাবে( size_t i = 0; i < it.nplanes; i++, ++it )
        {
            result += hal::normHamming(ptrs[0], total, cellSize);
        }

        ঘূৰাই result;
    }

    ধ্রুৱ Mat* arrays[] = {&src, &mask, 0};
    uchar* ptrs[2] = {};
    সঙ্ঘ
    {
        দ্ৱিগুণ d;
        unsigned u;
        uint64 UL;
        ভিন্ন f;
    }
    result;
    result.d = 0;
    NAryMatIterator it(arrays, ptrs);
    CV_CheckLT((size_t)it.size, (size_t)INT_MAX, "");
    তার্কিক is_fp16 = depth == CV_16F || depth == CV_16BF;

    যদি ((normType == NORM_L1 && (depth <= CV_16S || depth == CV_Bool || is_fp16)) ||
        ((normType == NORM_L2 || normType == NORM_L2SQR) && (depth <= CV_8S || depth == CV_Bool || is_fp16)))
    {
        // special case to handle "integer" overflow in accumulator
        ধ্রুৱ size_t esz = src.elemSize();
        ধ্রুৱ পূর্ণাঙ্ক total = (পূর্ণাঙ্ক)it.size;
        ধ্রুৱ পূর্ণাঙ্ক blockSize0 = (is_fp16 ? (1 << 10) :
            depth == CV_Bool ? (1 << 30) :
            normType == NORM_L1 && depth <= CV_8S ? (1 << 23) : (1 << 15))/cn;
        ধ্রুৱ পূর্ণাঙ্ক blockSize = std::min(total, blockSize0);
        সঙ্ঘ {
            পূর্ণাঙ্ক i;
            ভিন্ন f;
        } blocksum;
        blocksum.i = 0;
        পূর্ণাঙ্ক গুনতি_2 = 0;

        বাবে (size_t i = 0; i < it.nplanes; i++, ++it)
        {
            বাবে (পূর্ণাঙ্ক j = 0; j < total; j += blockSize)
            {
                পূর্ণাঙ্ক bsz = std::min(total - j, blockSize);
                func(ptrs[0], ptrs[1], (uchar*)&blocksum, bsz, cn);
                গুনতি_2 += bsz;
                যদি (গুনতি_2 + blockSize >= blockSize0 || (i+1 >= it.nplanes && j+bsz >= total))
                {
                    result.d += is_fp16 ? (দ্ৱিগুণ)blocksum.f : (দ্ৱিগুণ)blocksum.i;
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
        বাবে (size_t i = 0; i < it.nplanes; i++, ++it)
        {
            func(ptrs[0], ptrs[1], (uchar*)&result, (পূর্ণাঙ্ক)it.size, cn);
        }
    }

    যদি( normType == NORM_INF )
    {
        যদি(depth <= CV_32S || depth == CV_32U || depth == CV_Bool)
            ঘূৰাই result.u;
        যদি (depth == CV_32F || is_fp16)
            ঘূৰাই result.f;
        যদি (depth == CV_64U || depth == CV_64S)
            ঘূৰাই (দ্ৱিগুণ)result.UL;
    }
    যদি( normType == NORM_L2 )
        ঘূৰাই std::sqrt(result.d);

    ঘূৰাই result.d;
}

//==================================================================================================

#ifdef HAVE_OPENCL
স্থির তার্কিক ocl_norm( প্ৰবেশ_Array _src1, প্ৰবেশ_Array _src2, পূর্ণাঙ্ক normType, প্ৰবেশ_Array _mask, দ্ৱিগুণ & result )
{
#ifdef __ANDROID__
    যদি (ocl::Device::getDefault().isNVidia())
        ঘূৰাই মিছা;
#endif

    Scalar sc1, sc2;
    পূর্ণাঙ্ক cn = _src1.channels();
    যদি (cn > 4)
        ঘূৰাই মিছা;
    পূর্ণাঙ্ক type = _src1.type(), depth = CV_MAT_DEPTH(type);
    তার্কিক relative = (normType & NORM_RELATIVE) != 0;
    normType &= ~NORM_RELATIVE;
    তার্কিক normsum = normType == NORM_L1 || normType == NORM_L2 || normType == NORM_L2SQR;

#ifdef __APPLE__
    যদি(normType == NORM_L1 && type == CV_16UC3 && !_mask.empty())
        ঘূৰাই মিছা;
#endif

    যদি (normsum)
    {
        যদি (!ocl_sum(_src1, sc1, normType == NORM_L2 || normType == NORM_L2SQR ?
                     OCL_OP_SUM_SQR : OCL_OP_SUM, _mask, _src2, relative, sc2))
            ঘূৰাই মিছা;
    }
    অন্যথা
    {
        যদি (!ocl_minMaxIdx(_src1, রিক্ত, &sc1[0], রিক্ত, রিক্ত, _mask, std::max(CV_32S, depth),
                           মিছা, _src2, relative ? &sc2[0] : রিক্ত))
            ঘূৰাই মিছা;
        cn = 1;
    }

    দ্ৱিগুণ s2 = 0;
    বাবে (পূর্ণাঙ্ক i = 0; i < cn; ++i)
    {
        result += sc1[i];
        যদি (relative)
            s2 += sc2[i];
    }

    যদি (normType == NORM_L2)
    {
        result = std::sqrt(result);
        যদি (relative)
            s2 = std::sqrt(s2);
    }

    যদি (relative)
        result /= (s2 + DBL_EPSILON);

    ঘূৰাই সঁচা;
}  // ocl_norm()
#endif  // HAVE_OPENCL

দ্ৱিগুণ norm( প্ৰবেশ_Array _src1, প্ৰবেশ_Array _src2, পূর্ণাঙ্ক normType, প্ৰবেশ_Array _mask )
{
    CV_INSTRUMENT_REGION();

    CV_CheckTypeEQ(_src1.type(), _src2.type(), "Input type mismatch");
    CV_Assert(_src1.sameSize(_src2));

#যদি defined HAVE_OPENCL
    দ্ৱিগুণ _result = 0;
#endif

#ifdef HAVE_OPENCL
    CV_OCL_RUN_(OCL_PERFORMANCE_CHECK(_src1.isUMat()),
                ocl_norm(_src1, _src2, normType, _mask, _result),
                _result)
#endif

    Mat src1 = _src1.getMat(), src2 = _src2.getMat(), mask = _mask.getMat();
    CV_Assert( mask.empty() || ((mask.type() == CV_8U || mask.type() == CV_8S || mask.type() == CV_Bool) && src1.size == mask.size) );
    যদি( src1.empty() || src2.empty() )
        ঘূৰাই 0.0;
    পূর্ণাঙ্ক depth = src1.depth(), cn = src1.channels();
    যদি( src1.dims <= 2 )
    {
        দ্ৱিগুণ result;
        CALL_HAL_RET(normDiff, cv_hal_normDiff, result, src1.data, src1.step, src2.data, src2.step, mask.data, mask.step, src1.cols, src1.rows, src1.type(), normType);
    }
    অন্যথা যদি( src1.isContinuous() && src2.isContinuous() && mask.isContinuous() )
    {
        দ্ৱিগুণ result;
        CALL_HAL_RET(normDiff, cv_hal_normDiff, result, src1.data, 0, src2.data, 0, mask.data, 0, (পূর্ণাঙ্ক)src1.total(), 1, src1.type(), normType);
    }

    যদি( normType & NORM_RELATIVE )
    {
        ঘূৰাই norm(_src1, _src2, normType & ~NORM_RELATIVE, _mask)/(norm(_src2, normType, _mask) + DBL_EPSILON);
    }

    normType &= 7;
    CV_Assert( normType == NORM_INF || normType == NORM_L1 ||
               normType == NORM_L2 || normType == NORM_L2SQR ||
              ((normType == NORM_HAMMING || normType == NORM_HAMMING2) && src1.type() == CV_8U) );

    NormDiffFunc func = getNormDiffFunc(normType >> 1, depth);
    CV_Assert( (normType >> 1) >= 3 || func != 0 );

    যদি( src1.isContinuous() && src2.isContinuous() && mask.empty() )
    {
        size_t len = src1.total()*src1.channels();
        যদি( len == (size_t)(পূর্ণাঙ্ক)len )
        {
            যদি( src1.depth() == CV_32F )
            {
                ধ্রুৱ uchar* data1 = src1.ptr<ধ্রুৱ uchar>();
                ধ্রুৱ uchar* data2 = src2.ptr<ধ্রুৱ uchar>();

                যদি( normType == NORM_L2 || normType == NORM_L2SQR || normType == NORM_L1 )
                {
                    দ্ৱিগুণ result = 0;
                    func(data1, data2, 0, (uchar*)&result, (পূর্ণাঙ্ক)len, 1);
                    ঘূৰাই normType == NORM_L2 ? std::sqrt(result) : result;
                }
                যদি( normType == NORM_INF )
                {
                    ভিন্ন result = 0;
                    func(data1, data2, 0, (uchar*)&result, (পূর্ণাঙ্ক)len, 1);
                    ঘূৰাই result;
                }
            }
        }
    }

    CV_Assert( mask.empty() || mask.type() == CV_8U || mask.type() == CV_8S || mask.type() == CV_Bool );

    যদি( normType == NORM_HAMMING || normType == NORM_HAMMING2 )
    {
        যদি( !mask.empty() )
        {
            Mat temp;
            bitwise_xor(src1, src2, temp);
            bitwise_and(temp, mask, temp);
            ঘূৰাই norm(temp, normType);
        }
        পূর্ণাঙ্ক cellSize = normType == NORM_HAMMING ? 1 : 2;

        ধ্রুৱ Mat* arrays[] = {&src1, &src2, 0};
        uchar* ptrs[2] = {};
        NAryMatIterator it(arrays, ptrs);
        পূর্ণাঙ্ক total = (পূর্ণাঙ্ক)it.size;
        পূর্ণাঙ্ক result = 0;

        বাবে( size_t i = 0; i < it.nplanes; i++, ++it )
        {
            result += hal::normHamming(ptrs[0], ptrs[1], total, cellSize);
        }

        ঘূৰাই result;
    }

    ধ্রুৱ Mat* arrays[] = {&src1, &src2, &mask, 0};
    uchar* ptrs[3] = {};
    সঙ্ঘ
    {
        দ্ৱিগুণ d;
        ভিন্ন f;
        unsigned u;
        uint64 UL;
    }
    result;
    result.d = 0;
    NAryMatIterator it(arrays, ptrs);
    CV_CheckLT((size_t)it.size, (size_t)INT_MAX, "");

    তার্কিক is_fp16 = depth == CV_16F || depth == CV_16BF;

    যদি ((normType == NORM_L1 && (depth <= CV_16S || depth == CV_Bool || is_fp16)) ||
        ((normType == NORM_L2 || normType == NORM_L2SQR) && (depth <= CV_8S || depth == CV_Bool || is_fp16)))
    {
        // special case to handle "integer" overflow in accumulator
        ধ্রুৱ size_t esz = src1.elemSize();
        ধ্রুৱ পূর্ণাঙ্ক total = (পূর্ণাঙ্ক)it.size;
        ধ্রুৱ পূর্ণাঙ্ক blockSize0 = (is_fp16 ? (1 << 10) :
            depth == CV_Bool ? (1 << 30) :
            normType == NORM_L1 && depth <= CV_8S ? (1 << 23) : (1 << 15))/cn;
        ধ্রুৱ পূর্ণাঙ্ক blockSize = std::min(total, blockSize0);
        সঙ্ঘ {
            পূর্ণাঙ্ক i;
            ভিন্ন f;
        } blocksum;
        blocksum.i = 0;
        পূর্ণাঙ্ক গুনতি_2 = 0;

        বাবে (size_t i = 0; i < it.nplanes; i++, ++it)
        {
            বাবে (পূর্ণাঙ্ক j = 0; j < total; j += blockSize)
            {
                পূর্ণাঙ্ক bsz = std::min(total - j, blockSize);
                func(ptrs[0], ptrs[1], ptrs[2], (uchar*)&blocksum, bsz, cn);
                গুনতি_2 += bsz;
                যদি (গুনতি_2 + blockSize >= blockSize0 || (i+1 >= it.nplanes && j+bsz >= total))
                {
                    result.d += is_fp16 ? (দ্ৱিগুণ)blocksum.f : (দ্ৱিগুণ)blocksum.i;
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
        বাবে (size_t i = 0; i < it.nplanes; i++, ++it)
        {
            func(ptrs[0], ptrs[1], ptrs[2], (uchar*)&result, (পূর্ণাঙ্ক)it.size, cn);
        }
    }

    যদি( normType == NORM_INF )
    {
        যদি (depth <= CV_32S || depth == CV_32U || depth == CV_Bool)
            ঘূৰাই result.u;
        যদি (depth == CV_32F || is_fp16)
            ঘূৰাই result.f;
        যদি (depth == CV_64U || depth == CV_64S)
            ঘূৰাই (দ্ৱিগুণ)result.UL;
    }
    যদি( normType == NORM_L2 )
        ঘূৰাই std::sqrt(result.d);

    ঘূৰাই result.d;
}

cv::Hamming::ResultType Hamming::operator()( ধ্রুৱ unsigned ৱর্ণ* a, ধ্রুৱ unsigned ৱর্ণ* b, পূর্ণাঙ্ক size ) ধ্রুৱ
{
    ঘূৰাই cv::hal::normHamming(a, b, size);
}

দ্ৱিগুণ PSNR(প্ৰবেশ_Array _src1, প্ৰবেশ_Array _src2, দ্ৱিগুণ R)
{
    CV_INSTRUMENT_REGION();

    //Input arrays must have depth CV_8U
    CV_Assert( _src1.type() == _src2.type() );

    দ্ৱিগুণ diff = std::sqrt(norm(_src1, _src2, NORM_L2SQR)/(_src1.total()*_src1.channels()));
    ঘূৰাই 20*log10(R/(diff+DBL_EPSILON));
}


#ifdef HAVE_OPENCL
স্থির তার্কিক ocl_normalize( প্ৰবেশ_Array _src, প্ৰবেশ_Output_Array _dst, প্ৰবেশ_Array _mask, পূর্ণাঙ্ক dtype,
                           দ্ৱিগুণ scale, দ্ৱিগুণ delta )
{
    UMat src = _src.getUMat();

    যদি( _mask.empty() )
        src.convertTo( _dst, dtype, scale, delta );
    অন্যথা যদি (src.channels() <= 4)
    {
        ধ্রুৱ ocl::Device & dev = ocl::Device::getDefault();

        পূর্ণাঙ্ক stype = _src.type(), sdepth = CV_MAT_DEPTH(stype), cn = CV_MAT_CN(stype),
                ddepth = CV_MAT_DEPTH(dtype), wdepth = std::max(CV_32F, std::max(sdepth, ddepth)),
                rowsPerWI = dev.isIntel() ? 4 : 1;

        ভিন্ন fscale = static_cast<ভিন্ন>(scale), fdelta = static_cast<ভিন্ন>(delta);
        তার্কিক haveScale = std::fabs(scale - 1) > DBL_EPSILON,
                haveZeroScale = !(std::fabs(scale) > DBL_EPSILON),
                haveDelta = std::fabs(delta) > DBL_EPSILON,
                doubleSupport = dev.doubleFPConfig() > 0;

        যদি (!haveScale && !haveDelta && stype == dtype)
        {
            _src.copyTo(_dst, _mask);
            ঘূৰাই সঁচা;
        }
        যদি (haveZeroScale)
        {
            _dst.setTo(Scalar(delta), _mask);
            ঘূৰাই সঁচা;
        }

        যদি ((sdepth == CV_64F || ddepth == CV_64F) && !doubleSupport)
            ঘূৰাই মিছা;

        ৱর্ণ cvt[2][50];
        String opts = format("-D srcT=%s -D dstT=%s -D convertToWT=%s -D cn=%d -D rowsPerWI=%d"
                             " -D convertToDT=%s -D workT=%s%s%s%s -D srcT1=%s -D dstT1=%s",
                             ocl::typeToStr(stype), ocl::typeToStr(dtype),
                             ocl::convertTypeStr(sdepth, wdepth, cn, cvt[0], পরিমাণ(cvt[0])), cn,
                             rowsPerWI, ocl::convertTypeStr(wdepth, ddepth, cn, cvt[1], পরিমাণ(cvt[1])),
                             ocl::typeToStr(CV_MAKE_TYPE(wdepth, cn)),
                             doubleSupport ? " -D DOUBLE_SUPPORT" : "",
                             haveScale ? " -D HAVE_SCALE" : "",
                             haveDelta ? " -D HAVE_DELTA" : "",
                             ocl::typeToStr(sdepth), ocl::typeToStr(ddepth));

        ocl::Kernel k("normalizek", ocl::core::normalize_oclsrc, opts);
        যদি (k.empty())
            ঘূৰাই মিছা;

        UMat mask = _mask.getUMat(), dst = _dst.getUMat();

        ocl::KernelArg srcarg = ocl::KernelArg::ReadOnlyNoSize(src),
                maskarg = ocl::KernelArg::ReadOnlyNoSize(mask),
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
        ঘূৰাই k.run(2, globalsize, রিক্ত, মিছা);
    }
    অন্যথা
    {
        UMat temp;
        src.convertTo( temp, dtype, scale, delta );
        temp.copyTo( _dst, _mask );
    }

    ঘূৰাই সঁচা;
}  // ocl_normalize
#endif  // HAVE_OPENCL

শূন্য normalize(প্ৰবেশ_Array _src, প্ৰবেশ_Output_Array _dst, দ্ৱিগুণ a, দ্ৱিগুণ b,
               পূর্ণাঙ্ক norm_type, পূর্ণাঙ্ক rtype, প্ৰবেশ_Array _mask)
{
    CV_INSTRUMENT_REGION();

    দ্ৱিগুণ scale = 1, shift = 0;
    পূর্ণাঙ্ক type = _src.type(), depth = CV_MAT_DEPTH(type);

    যদি( rtype < 0 )
        rtype = _dst.fixedType() ? _dst.depth() : depth;

    যদি( norm_type == NORM_MINMAX )
    {
        দ্ৱিগুণ smin = 0, smax = 0;
        দ্ৱিগুণ dmin = MIN( a, b ), dmax = MAX( a, b );
        minMaxIdx( _src, &smin, &smax, 0, 0, _mask );
        scale = (dmax - dmin)*(smax - smin > DBL_EPSILON ? 1./(smax - smin) : 0);
        যদি( rtype == CV_32F )
        {
            scale = (ভিন্ন)scale;
            shift = (ভিন্ন)dmin - (ভিন্ন)(smin*scale);
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
        Mat temp;
        src.convertTo( temp, rtype, scale, shift );
        temp.copyTo( _dst, _mask );
    }
}

}  // namespace
