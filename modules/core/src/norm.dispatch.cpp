// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html
//
// Copyright (C) 2026, Advanced Micro Devices, Inc., all rights reserved.


#അന്തര്ഭാവ "precomp.hpp"
#അന്തര്ഭാവ "opencl_kernels_core.hpp"
#അന്തര്ഭാവ "stat.hpp"

#അന്തര്ഭാവ "norm.simd.hpp"
#അന്തര്ഭാവ "norm.simd_declarations.hpp"

/****************************************************************************************\
*                                         norm                                           *
\****************************************************************************************/

നാമാകാശ cv { നാമാകാശ hal {

ബാഹ്യ സ്ഥിരം uchar എടുക്കുക_എണ്ണം_Table[256] =
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

സ്ഥിതം സ്ഥിരം uchar എടുക്കുക_എണ്ണം_Table2[] =
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

സ്ഥിതം സ്ഥിരം uchar എടുക്കുക_എണ്ണം_Table4[] =
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


പൂർണ്ണസംഖ്യ normHamming(സ്ഥിരം uchar* a, പൂർണ്ണസംഖ്യ n, പൂർണ്ണസംഖ്യ cellSize)
{
    പൂർണ്ണസംഖ്യ output;
    CALL_HAL_RET(normHamming8u, cv_hal_normHamming8u, output, a, n, cellSize);

    എങ്കിൽ( cellSize == 1 )
        തിരികെ normHamming(a, n);
    സ്ഥിരം uchar* tab = 0;
    എങ്കിൽ( cellSize == 2 )
        tab = എടുക്കുക_എണ്ണം_Table2;
    അല്ലെങ്കിൽ എങ്കിൽ( cellSize == 4 )
        tab = എടുക്കുക_എണ്ണം_Table4;
    അല്ലെങ്കിൽ
        തിരികെ -1;
    പൂർണ്ണസംഖ്യ i = 0;
    പൂർണ്ണസംഖ്യ result = 0;
#എങ്കിൽ (CV_SIMD || CV_SIMD_SCALABLE)
    v_uint64 t = vx_setzero_u64();
    എങ്കിൽ ( cellSize == 2)
    {
        v_uint16 mask = v_reinterpret_as_u16(vx_setall_u8(0x55));
        വേണ്ടി(; i <= n - VTraits<v_uint8>::vlanes(); i += VTraits<v_uint8>::vlanes())
        {
            v_uint16 a0 = v_reinterpret_as_u16(vx_load(a + i));
            t = v_add(t, v_popcount(v_reinterpret_as_u64(v_and(v_or(a0, v_shr<1>(a0)), mask))));
        }
    }
    അല്ലെങ്കിൽ    // cellSize == 4
    {
        v_uint16 mask = v_reinterpret_as_u16(vx_setall_u8(0x11));
        വേണ്ടി(; i <= n - VTraits<v_uint8>::vlanes(); i += VTraits<v_uint8>::vlanes())
        {
            v_uint16 a0 = v_reinterpret_as_u16(vx_load(a + i));
            v_uint16 a1 = v_or(a0, v_shr<2>(a0));
            t = v_add(t, v_popcount(v_reinterpret_as_u64(v_and(v_or(a1, v_shr<1>(a1)), mask))));

        }
    }
    result += (പൂർണ്ണസംഖ്യ)v_reduce_sum(t);
    vx_cleanup();
#അല്ലെങ്കിലെങ്കിൽ CV_ENABLE_UNROLLED
    വേണ്ടി( ; i <= n - 4; i += 4 )
        result += tab[a[i]] + tab[a[i+1]] + tab[a[i+2]] + tab[a[i+3]];
#endif
    വേണ്ടി( ; i < n; i++ )
        result += tab[a[i]];
    തിരികെ result;
}

പൂർണ്ണസംഖ്യ normHamming(സ്ഥിരം uchar* a, സ്ഥിരം uchar* b, പൂർണ്ണസംഖ്യ n, പൂർണ്ണസംഖ്യ cellSize)
{
    പൂർണ്ണസംഖ്യ output;
    CALL_HAL_RET(normHammingDiff8u, cv_hal_normHammingDiff8u, output, a, b, n, cellSize);

    എങ്കിൽ( cellSize == 1 )
        തിരികെ normHamming(a, b, n);
    സ്ഥിരം uchar* tab = 0;
    എങ്കിൽ( cellSize == 2 )
        tab = എടുക്കുക_എണ്ണം_Table2;
    അല്ലെങ്കിൽ എങ്കിൽ( cellSize == 4 )
        tab = എടുക്കുക_എണ്ണം_Table4;
    അല്ലെങ്കിൽ
        തിരികെ -1;
    പൂർണ്ണസംഖ്യ i = 0;
    പൂർണ്ണസംഖ്യ result = 0;
#എങ്കിൽ (CV_SIMD || CV_SIMD_SCALABLE)
    v_uint64 t = vx_setzero_u64();
    എങ്കിൽ ( cellSize == 2)
    {
        v_uint16 mask = v_reinterpret_as_u16(vx_setall_u8(0x55));
        വേണ്ടി(; i <= n - VTraits<v_uint8>::vlanes(); i += VTraits<v_uint8>::vlanes())
        {
            v_uint16 ab0 = v_reinterpret_as_u16(v_xor(vx_load(a + i), vx_load(b + i)));
            t = v_add(t, v_popcount(v_reinterpret_as_u64(v_and(v_or(ab0, v_shr<1>(ab0)), mask))));
        }
    }
    അല്ലെങ്കിൽ    // cellSize == 4
    {
        v_uint16 mask = v_reinterpret_as_u16(vx_setall_u8(0x11));
        വേണ്ടി(; i <= n - VTraits<v_uint8>::vlanes(); i += VTraits<v_uint8>::vlanes())
        {
            v_uint16 ab0 = v_reinterpret_as_u16(v_xor(vx_load(a + i), vx_load(b + i)));
            v_uint16 ab1 = v_or(ab0, v_shr<2>(ab0));
            t = v_add(t, v_popcount(v_reinterpret_as_u64(v_and(v_or(ab1, v_shr<1>(ab1)), mask))));
        }
    }
    result += (പൂർണ്ണസംഖ്യ)v_reduce_sum(t);
    vx_cleanup();
#അല്ലെങ്കിലെങ്കിൽ CV_ENABLE_UNROLLED
    വേണ്ടി( ; i <= n - 4; i += 4 )
        result += tab[a[i] ^ b[i]] + tab[a[i+1] ^ b[i+1]] +
                tab[a[i+2] ^ b[i+2]] + tab[a[i+3] ^ b[i+3]];
#endif
    വേണ്ടി( ; i < n; i++ )
        result += tab[a[i] ^ b[i]];
    തിരികെ result;
}

ദശാംശം normL2Sqr_(സ്ഥിരം ദശാംശം* a, സ്ഥിരം ദശാംശം* b, പൂർണ്ണസംഖ്യ n)
{
    പൂർണ്ണസംഖ്യ j = 0; ദശാംശം d = 0.f;
#എങ്കിൽ (CV_SIMD || CV_SIMD_SCALABLE)
    സ്ഥിരം പൂർണ്ണസംഖ്യ vl = VTraits<v_float32>::vlanes();
    v_float32 v_d0 = vx_setzero_f32(), v_d1 = vx_setzero_f32();
    v_float32 v_d2 = vx_setzero_f32(), v_d3 = vx_setzero_f32();
    വേണ്ടി (; j <= n - 4 * vl; j += 4 * vl)
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
    വേണ്ടി (; j <= n - vl; j += vl)
    {
        v_float32 t0 = v_sub(vx_load(a + j), vx_load(b + j));
        v_d0 = v_muladd(t0, t0, v_d0);
    }
    d = v_reduce_sum(v_d0);
#endif
    വേണ്ടി( ; j < n; j++ )
    {
        ദശാംശം t = a[j] - b[j];
        d += t*t;
    }
    തിരികെ d;
}


ദശാംശം normL1_(സ്ഥിരം ദശാംശം* a, സ്ഥിരം ദശാംശം* b, പൂർണ്ണസംഖ്യ n)
{
    പൂർണ്ണസംഖ്യ j = 0; ദശാംശം d = 0.f;
#എങ്കിൽ (CV_SIMD || CV_SIMD_SCALABLE)
    സ്ഥിരം പൂർണ്ണസംഖ്യ vl = VTraits<v_float32>::vlanes();
    v_float32 v_d0 = vx_setzero_f32(), v_d1 = vx_setzero_f32();
    v_float32 v_d2 = vx_setzero_f32(), v_d3 = vx_setzero_f32();
    വേണ്ടി (; j <= n - 4 * vl; j += 4 * vl)
    {
        v_d0 = v_add(v_d0, v_absdiff(vx_load(a + j), vx_load(b + j)));
        v_d1 = v_add(v_d1, v_absdiff(vx_load(a + j + vl), vx_load(b + j + vl)));
        v_d2 = v_add(v_d2, v_absdiff(vx_load(a + j + 2 * vl), vx_load(b + j + 2 * vl)));
        v_d3 = v_add(v_d3, v_absdiff(vx_load(a + j + 3 * vl), vx_load(b + j + 3 * vl)));
    }
    v_d0 = v_add(v_add(v_d0, v_d1), v_add(v_d2, v_d3));
    വേണ്ടി (; j <= n - vl; j += vl)
        v_d0 = v_add(v_d0, v_absdiff(vx_load(a + j), vx_load(b + j)));
    d = v_reduce_sum(v_d0);
#endif
    വേണ്ടി( ; j < n; j++ )
        d += std::abs(a[j] - b[j]);
    തിരികെ d;
}

പൂർണ്ണസംഖ്യ normL1_(സ്ഥിരം uchar* a, സ്ഥിരം uchar* b, പൂർണ്ണസംഖ്യ n)
{
    പൂർണ്ണസംഖ്യ j = 0, d = 0;
#എങ്കിൽ (CV_SIMD || CV_SIMD_SCALABLE)
    സ്ഥിരം പൂർണ്ണസംഖ്യ vl = VTraits<v_uint8>::vlanes();
    വേണ്ടി (; j <= n - 4 * vl; j += 4 * vl)
        d += v_reduce_sad(vx_load(a + j), vx_load(b + j)) +
             v_reduce_sad(vx_load(a + j + vl), vx_load(b + j + vl)) +
             v_reduce_sad(vx_load(a + j + 2 * vl), vx_load(b + j + 2 * vl)) +
             v_reduce_sad(vx_load(a + j + 3 * vl), vx_load(b + j + 3 * vl));
    വേണ്ടി (; j <= n - vl; j += vl)
        d += v_reduce_sad(vx_load(a + j), vx_load(b + j));
#endif
    വേണ്ടി( ; j < n; j++ )
        d += std::abs(a[j] - b[j]);
    തിരികെ d;
}

} //cv::hal

//==================================================================================================

തരനിർവചനം പൂർണ്ണസംഖ്യ (*NormFunc)(സ്ഥിരം uchar*, സ്ഥിരം uchar*, uchar*, പൂർണ്ണസംഖ്യ, പൂർണ്ണസംഖ്യ);
തരനിർവചനം പൂർണ്ണസംഖ്യ (*NormDiffFunc)(സ്ഥിരം uchar*, സ്ഥിരം uchar*, സ്ഥിരം uchar*, uchar*, പൂർണ്ണസംഖ്യ, പൂർണ്ണസംഖ്യ);

#ifdef HAVE_OPENCL

സ്ഥിതം തർക്കം ocl_norm( നിവേശം_Array _src, പൂർണ്ണസംഖ്യ normType, നിവേശം_Array _mask, ഇരട്ട & result )
{
    സ്ഥിരം ocl::Device & d = ocl::Device::getDefault();

#ifdef __ANDROID__
    എങ്കിൽ (d.isNVidia())
        തിരികെ അസത്യം;
#endif
    സ്ഥിരം പൂർണ്ണസംഖ്യ cn = _src.channels();
    എങ്കിൽ (cn > 4)
        തിരികെ അസത്യം;
    പൂർണ്ണസംഖ്യ type = _src.type(), depth = CV_MAT_DEPTH(type);
    തർക്കം doubleSupport = d.doubleFPConfig() > 0,
            haveMask = _mask.kind() != നിവേശം_Array_2::NONE;

    എങ്കിൽ (depth >= CV_16F)
        തിരികെ അസത്യം;  // TODO: support FP16

    എങ്കിൽ ( !(normType == NORM_INF || normType == NORM_L1 || normType == NORM_L2 || normType == NORM_L2SQR) ||
         (!doubleSupport && depth == CV_64F))
        തിരികെ അസത്യം;

    UMat src = _src.getUMat();

    എങ്കിൽ (normType == NORM_INF)
    {
        എങ്കിൽ (!ocl_minMaxIdx(_src, ശൂന്യ, &result, ശൂന്യ, ശൂന്യ, _mask,
                           std::max(depth, CV_32S), depth != CV_8U && depth != CV_16U))
            തിരികെ അസത്യം;
    }
    അല്ലെങ്കിൽ എങ്കിൽ (normType == NORM_L1 || normType == NORM_L2 || normType == NORM_L2SQR)
    {
        Scalar sc;
        തർക്കം unstype = depth == CV_8U || depth == CV_16U;

        എങ്കിൽ ( !ocl_sum(haveMask ? src : src.reshape(1), sc, normType == NORM_L2 || normType == NORM_L2SQR ?
                    OCL_OP_SUM_SQR : (unstype ? OCL_OP_SUM : OCL_OP_SUM_ABS), _mask) )
            തിരികെ അസത്യം;

        ഇരട്ട s = 0.0;
        വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < (haveMask ? cn : 1); ++i)
            s += sc[i];

        result = normType == NORM_L1 || normType == NORM_L2SQR ? s : std::sqrt(s);
    }

    തിരികെ സത്യം;
}

#endif

സ്ഥിതം NormFunc getNormFunc(പൂർണ്ണസംഖ്യ normType, പൂർണ്ണസംഖ്യ depth) {
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getNormFunc, (normType, depth), CV_CPU_DISPATCH_MODES_ALL);
}
സ്ഥിതം NormDiffFunc getNormDiffFunc(പൂർണ്ണസംഖ്യ normType, പൂർണ്ണസംഖ്യ depth) {
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getNormDiffFunc, (normType, depth), CV_CPU_DISPATCH_MODES_ALL);
}

ഇരട്ട norm( നിവേശം_Array _src, പൂർണ്ണസംഖ്യ normType, നിവേശം_Array _mask )
{
    CV_INSTRUMENT_REGION();

    normType &= NORM_TYPE_MASK;
    CV_Assert( normType == NORM_INF || normType == NORM_L1 ||
               normType == NORM_L2 || normType == NORM_L2SQR ||
               ((normType == NORM_HAMMING || normType == NORM_HAMMING2) && _src.type() == CV_8U) );

#എങ്കിൽ defined HAVE_OPENCL
    ഇരട്ട _result = 0;
#endif

#ifdef HAVE_OPENCL
    CV_OCL_RUN_(OCL_PERFORMANCE_CHECK(_src.isUMat()) && _src.dims() <= 2,
                ocl_norm(_src, normType, _mask, _result),
                _result)
#endif

    Mat src = _src.getMat(), mask = _mask.getMat();
    CV_Assert( mask.empty() || ((mask.type() == CV_8U || mask.type() == CV_8S || mask.type() == CV_Bool) && src.size == mask.size) );
    എങ്കിൽ( src.empty() )
        തിരികെ 0.0;
    പൂർണ്ണസംഖ്യ depth = src.depth(), cn = src.channels();
    എങ്കിൽ( src.dims <= 2 )
    {
        ഇരട്ട result;
        CALL_HAL_RET(norm, cv_hal_norm, result, src.data, src.step, mask.data, mask.step, src.cols, src.rows, src.type(), normType);
    }
    അല്ലെങ്കിൽ എങ്കിൽ( src.isContinuous() && mask.isContinuous() )
    {
        ഇരട്ട result;
        CALL_HAL_RET(norm, cv_hal_norm, result, src.data, 0, mask.data, 0, (പൂർണ്ണസംഖ്യ)src.total(), 1, src.type(), normType);
    }

    NormFunc func = getNormFunc(normType >> 1, depth);
    CV_Assert( (normType >> 1) >= 3 || func != 0 );

    എങ്കിൽ( src.isContinuous() && mask.empty() )
    {
        size_t len = src.total()*cn;
        എങ്കിൽ( len == (size_t)(പൂർണ്ണസംഖ്യ)len )
        {
            എങ്കിൽ( depth == CV_32F )
            {
                സ്ഥിരം uchar* data = src.ptr<സ്ഥിരം uchar>();

                എങ്കിൽ( normType == NORM_L2 || normType == NORM_L2SQR || normType == NORM_L1 )
                {
                    ഇരട്ട result = 0;
                    func(data, 0, (uchar*)&result, (പൂർണ്ണസംഖ്യ)len, 1);
                    തിരികെ normType == NORM_L2 ? std::sqrt(result) : result;
                }
                എങ്കിൽ( normType == NORM_INF )
                {
                    ദശാംശം result = 0;
                    func(data, 0, (uchar*)&result, (പൂർണ്ണസംഖ്യ)len, 1);
                    തിരികെ result;
                }
            }
            എങ്കിൽ( depth == CV_8U )
            {
                സ്ഥിരം uchar* data = src.ptr<uchar>();

                എങ്കിൽ( normType == NORM_HAMMING )
                {
                    തിരികെ hal::normHamming(data, (പൂർണ്ണസംഖ്യ)len, 1);
                }

                എങ്കിൽ( normType == NORM_HAMMING2 )
                {
                    തിരികെ hal::normHamming(data, (പൂർണ്ണസംഖ്യ)len, 2);
                }
            }
        }
    }

    CV_Assert( mask.empty() || mask.type() == CV_8U || mask.type() == CV_8S || mask.type() == CV_Bool );

    എങ്കിൽ( normType == NORM_HAMMING || normType == NORM_HAMMING2 )
    {
        എങ്കിൽ( !mask.empty() )
        {
            Mat temp;
            bitwise_and(src, mask, temp);
            തിരികെ norm(temp, normType);
        }
        പൂർണ്ണസംഖ്യ cellSize = normType == NORM_HAMMING ? 1 : 2;

        സ്ഥിരം Mat* arrays[] = {&src, 0};
        uchar* ptrs[1] = {};
        NAryMatIterator it(arrays, ptrs);
        പൂർണ്ണസംഖ്യ total = (പൂർണ്ണസംഖ്യ)it.size;
        പൂർണ്ണസംഖ്യ result = 0;

        വേണ്ടി( size_t i = 0; i < it.nplanes; i++, ++it )
        {
            result += hal::normHamming(ptrs[0], total, cellSize);
        }

        തിരികെ result;
    }

    സ്ഥിരം Mat* arrays[] = {&src, &mask, 0};
    uchar* ptrs[2] = {};
    സംഘം
    {
        ഇരട്ട d;
        unsigned u;
        uint64 UL;
        ദശാംശം f;
    }
    result;
    result.d = 0;
    NAryMatIterator it(arrays, ptrs);
    CV_CheckLT((size_t)it.size, (size_t)INT_MAX, "");
    തർക്കം is_fp16 = depth == CV_16F || depth == CV_16BF;

    എങ്കിൽ ((normType == NORM_L1 && (depth <= CV_16S || depth == CV_Bool || is_fp16)) ||
        ((normType == NORM_L2 || normType == NORM_L2SQR) && (depth <= CV_8S || depth == CV_Bool || is_fp16)))
    {
        // special case to handle "integer" overflow in accumulator
        സ്ഥിരം size_t esz = src.elemSize();
        സ്ഥിരം പൂർണ്ണസംഖ്യ total = (പൂർണ്ണസംഖ്യ)it.size;
        സ്ഥിരം പൂർണ്ണസംഖ്യ blockSize0 = (is_fp16 ? (1 << 10) :
            depth == CV_Bool ? (1 << 30) :
            normType == NORM_L1 && depth <= CV_8S ? (1 << 23) : (1 << 15))/cn;
        സ്ഥിരം പൂർണ്ണസംഖ്യ blockSize = std::min(total, blockSize0);
        സംഘം {
            പൂർണ്ണസംഖ്യ i;
            ദശാംശം f;
        } blocksum;
        blocksum.i = 0;
        പൂർണ്ണസംഖ്യ എണ്ണം_2 = 0;

        വേണ്ടി (size_t i = 0; i < it.nplanes; i++, ++it)
        {
            വേണ്ടി (പൂർണ്ണസംഖ്യ j = 0; j < total; j += blockSize)
            {
                പൂർണ്ണസംഖ്യ bsz = std::min(total - j, blockSize);
                func(ptrs[0], ptrs[1], (uchar*)&blocksum, bsz, cn);
                എണ്ണം_2 += bsz;
                എങ്കിൽ (എണ്ണം_2 + blockSize >= blockSize0 || (i+1 >= it.nplanes && j+bsz >= total))
                {
                    result.d += is_fp16 ? (ഇരട്ട)blocksum.f : (ഇരട്ട)blocksum.i;
                    blocksum.i = 0;
                    എണ്ണം_2 = 0;
                }
                ptrs[0] += bsz*esz;
                എങ്കിൽ (ptrs[1])
                    ptrs[1] += bsz;
            }
        }
    }
    അല്ലെങ്കിൽ
    {
        // generic implementation
        വേണ്ടി (size_t i = 0; i < it.nplanes; i++, ++it)
        {
            func(ptrs[0], ptrs[1], (uchar*)&result, (പൂർണ്ണസംഖ്യ)it.size, cn);
        }
    }

    എങ്കിൽ( normType == NORM_INF )
    {
        എങ്കിൽ(depth <= CV_32S || depth == CV_32U || depth == CV_Bool)
            തിരികെ result.u;
        എങ്കിൽ (depth == CV_32F || is_fp16)
            തിരികെ result.f;
        എങ്കിൽ (depth == CV_64U || depth == CV_64S)
            തിരികെ (ഇരട്ട)result.UL;
    }
    എങ്കിൽ( normType == NORM_L2 )
        തിരികെ std::sqrt(result.d);

    തിരികെ result.d;
}

//==================================================================================================

#ifdef HAVE_OPENCL
സ്ഥിതം തർക്കം ocl_norm( നിവേശം_Array _src1, നിവേശം_Array _src2, പൂർണ്ണസംഖ്യ normType, നിവേശം_Array _mask, ഇരട്ട & result )
{
#ifdef __ANDROID__
    എങ്കിൽ (ocl::Device::getDefault().isNVidia())
        തിരികെ അസത്യം;
#endif

    Scalar sc1, sc2;
    പൂർണ്ണസംഖ്യ cn = _src1.channels();
    എങ്കിൽ (cn > 4)
        തിരികെ അസത്യം;
    പൂർണ്ണസംഖ്യ type = _src1.type(), depth = CV_MAT_DEPTH(type);
    തർക്കം relative = (normType & NORM_RELATIVE) != 0;
    normType &= ~NORM_RELATIVE;
    തർക്കം normsum = normType == NORM_L1 || normType == NORM_L2 || normType == NORM_L2SQR;

#ifdef __APPLE__
    എങ്കിൽ(normType == NORM_L1 && type == CV_16UC3 && !_mask.empty())
        തിരികെ അസത്യം;
#endif

    എങ്കിൽ (normsum)
    {
        എങ്കിൽ (!ocl_sum(_src1, sc1, normType == NORM_L2 || normType == NORM_L2SQR ?
                     OCL_OP_SUM_SQR : OCL_OP_SUM, _mask, _src2, relative, sc2))
            തിരികെ അസത്യം;
    }
    അല്ലെങ്കിൽ
    {
        എങ്കിൽ (!ocl_minMaxIdx(_src1, ശൂന്യ, &sc1[0], ശൂന്യ, ശൂന്യ, _mask, std::max(CV_32S, depth),
                           അസത്യം, _src2, relative ? &sc2[0] : ശൂന്യ))
            തിരികെ അസത്യം;
        cn = 1;
    }

    ഇരട്ട s2 = 0;
    വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < cn; ++i)
    {
        result += sc1[i];
        എങ്കിൽ (relative)
            s2 += sc2[i];
    }

    എങ്കിൽ (normType == NORM_L2)
    {
        result = std::sqrt(result);
        എങ്കിൽ (relative)
            s2 = std::sqrt(s2);
    }

    എങ്കിൽ (relative)
        result /= (s2 + DBL_EPSILON);

    തിരികെ സത്യം;
}  // ocl_norm()
#endif  // HAVE_OPENCL

ഇരട്ട norm( നിവേശം_Array _src1, നിവേശം_Array _src2, പൂർണ്ണസംഖ്യ normType, നിവേശം_Array _mask )
{
    CV_INSTRUMENT_REGION();

    CV_CheckTypeEQ(_src1.type(), _src2.type(), "Input type mismatch");
    CV_Assert(_src1.sameSize(_src2));

#എങ്കിൽ defined HAVE_OPENCL
    ഇരട്ട _result = 0;
#endif

#ifdef HAVE_OPENCL
    CV_OCL_RUN_(OCL_PERFORMANCE_CHECK(_src1.isUMat()),
                ocl_norm(_src1, _src2, normType, _mask, _result),
                _result)
#endif

    Mat src1 = _src1.getMat(), src2 = _src2.getMat(), mask = _mask.getMat();
    CV_Assert( mask.empty() || ((mask.type() == CV_8U || mask.type() == CV_8S || mask.type() == CV_Bool) && src1.size == mask.size) );
    എങ്കിൽ( src1.empty() || src2.empty() )
        തിരികെ 0.0;
    പൂർണ്ണസംഖ്യ depth = src1.depth(), cn = src1.channels();
    എങ്കിൽ( src1.dims <= 2 )
    {
        ഇരട്ട result;
        CALL_HAL_RET(normDiff, cv_hal_normDiff, result, src1.data, src1.step, src2.data, src2.step, mask.data, mask.step, src1.cols, src1.rows, src1.type(), normType);
    }
    അല്ലെങ്കിൽ എങ്കിൽ( src1.isContinuous() && src2.isContinuous() && mask.isContinuous() )
    {
        ഇരട്ട result;
        CALL_HAL_RET(normDiff, cv_hal_normDiff, result, src1.data, 0, src2.data, 0, mask.data, 0, (പൂർണ്ണസംഖ്യ)src1.total(), 1, src1.type(), normType);
    }

    എങ്കിൽ( normType & NORM_RELATIVE )
    {
        തിരികെ norm(_src1, _src2, normType & ~NORM_RELATIVE, _mask)/(norm(_src2, normType, _mask) + DBL_EPSILON);
    }

    normType &= 7;
    CV_Assert( normType == NORM_INF || normType == NORM_L1 ||
               normType == NORM_L2 || normType == NORM_L2SQR ||
              ((normType == NORM_HAMMING || normType == NORM_HAMMING2) && src1.type() == CV_8U) );

    NormDiffFunc func = getNormDiffFunc(normType >> 1, depth);
    CV_Assert( (normType >> 1) >= 3 || func != 0 );

    എങ്കിൽ( src1.isContinuous() && src2.isContinuous() && mask.empty() )
    {
        size_t len = src1.total()*src1.channels();
        എങ്കിൽ( len == (size_t)(പൂർണ്ണസംഖ്യ)len )
        {
            എങ്കിൽ( src1.depth() == CV_32F )
            {
                സ്ഥിരം uchar* data1 = src1.ptr<സ്ഥിരം uchar>();
                സ്ഥിരം uchar* data2 = src2.ptr<സ്ഥിരം uchar>();

                എങ്കിൽ( normType == NORM_L2 || normType == NORM_L2SQR || normType == NORM_L1 )
                {
                    ഇരട്ട result = 0;
                    func(data1, data2, 0, (uchar*)&result, (പൂർണ്ണസംഖ്യ)len, 1);
                    തിരികെ normType == NORM_L2 ? std::sqrt(result) : result;
                }
                എങ്കിൽ( normType == NORM_INF )
                {
                    ദശാംശം result = 0;
                    func(data1, data2, 0, (uchar*)&result, (പൂർണ്ണസംഖ്യ)len, 1);
                    തിരികെ result;
                }
            }
        }
    }

    CV_Assert( mask.empty() || mask.type() == CV_8U || mask.type() == CV_8S || mask.type() == CV_Bool );

    എങ്കിൽ( normType == NORM_HAMMING || normType == NORM_HAMMING2 )
    {
        എങ്കിൽ( !mask.empty() )
        {
            Mat temp;
            bitwise_xor(src1, src2, temp);
            bitwise_and(temp, mask, temp);
            തിരികെ norm(temp, normType);
        }
        പൂർണ്ണസംഖ്യ cellSize = normType == NORM_HAMMING ? 1 : 2;

        സ്ഥിരം Mat* arrays[] = {&src1, &src2, 0};
        uchar* ptrs[2] = {};
        NAryMatIterator it(arrays, ptrs);
        പൂർണ്ണസംഖ്യ total = (പൂർണ്ണസംഖ്യ)it.size;
        പൂർണ്ണസംഖ്യ result = 0;

        വേണ്ടി( size_t i = 0; i < it.nplanes; i++, ++it )
        {
            result += hal::normHamming(ptrs[0], ptrs[1], total, cellSize);
        }

        തിരികെ result;
    }

    സ്ഥിരം Mat* arrays[] = {&src1, &src2, &mask, 0};
    uchar* ptrs[3] = {};
    സംഘം
    {
        ഇരട്ട d;
        ദശാംശം f;
        unsigned u;
        uint64 UL;
    }
    result;
    result.d = 0;
    NAryMatIterator it(arrays, ptrs);
    CV_CheckLT((size_t)it.size, (size_t)INT_MAX, "");

    തർക്കം is_fp16 = depth == CV_16F || depth == CV_16BF;

    എങ്കിൽ ((normType == NORM_L1 && (depth <= CV_16S || depth == CV_Bool || is_fp16)) ||
        ((normType == NORM_L2 || normType == NORM_L2SQR) && (depth <= CV_8S || depth == CV_Bool || is_fp16)))
    {
        // special case to handle "integer" overflow in accumulator
        സ്ഥിരം size_t esz = src1.elemSize();
        സ്ഥിരം പൂർണ്ണസംഖ്യ total = (പൂർണ്ണസംഖ്യ)it.size;
        സ്ഥിരം പൂർണ്ണസംഖ്യ blockSize0 = (is_fp16 ? (1 << 10) :
            depth == CV_Bool ? (1 << 30) :
            normType == NORM_L1 && depth <= CV_8S ? (1 << 23) : (1 << 15))/cn;
        സ്ഥിരം പൂർണ്ണസംഖ്യ blockSize = std::min(total, blockSize0);
        സംഘം {
            പൂർണ്ണസംഖ്യ i;
            ദശാംശം f;
        } blocksum;
        blocksum.i = 0;
        പൂർണ്ണസംഖ്യ എണ്ണം_2 = 0;

        വേണ്ടി (size_t i = 0; i < it.nplanes; i++, ++it)
        {
            വേണ്ടി (പൂർണ്ണസംഖ്യ j = 0; j < total; j += blockSize)
            {
                പൂർണ്ണസംഖ്യ bsz = std::min(total - j, blockSize);
                func(ptrs[0], ptrs[1], ptrs[2], (uchar*)&blocksum, bsz, cn);
                എണ്ണം_2 += bsz;
                എങ്കിൽ (എണ്ണം_2 + blockSize >= blockSize0 || (i+1 >= it.nplanes && j+bsz >= total))
                {
                    result.d += is_fp16 ? (ഇരട്ട)blocksum.f : (ഇരട്ട)blocksum.i;
                    blocksum.i = 0;
                    എണ്ണം_2 = 0;
                }
                ptrs[0] += bsz*esz;
                ptrs[1] += bsz*esz;
                എങ്കിൽ (ptrs[2])
                    ptrs[2] += bsz;
            }
        }
    }
    അല്ലെങ്കിൽ
    {
        // generic implementation
        വേണ്ടി (size_t i = 0; i < it.nplanes; i++, ++it)
        {
            func(ptrs[0], ptrs[1], ptrs[2], (uchar*)&result, (പൂർണ്ണസംഖ്യ)it.size, cn);
        }
    }

    എങ്കിൽ( normType == NORM_INF )
    {
        എങ്കിൽ (depth <= CV_32S || depth == CV_32U || depth == CV_Bool)
            തിരികെ result.u;
        എങ്കിൽ (depth == CV_32F || is_fp16)
            തിരികെ result.f;
        എങ്കിൽ (depth == CV_64U || depth == CV_64S)
            തിരികെ (ഇരട്ട)result.UL;
    }
    എങ്കിൽ( normType == NORM_L2 )
        തിരികെ std::sqrt(result.d);

    തിരികെ result.d;
}

cv::Hamming::ResultType Hamming::operator()( സ്ഥിരം unsigned അക്ഷരം* a, സ്ഥിരം unsigned അക്ഷരം* b, പൂർണ്ണസംഖ്യ size ) സ്ഥിരം
{
    തിരികെ cv::hal::normHamming(a, b, size);
}

ഇരട്ട PSNR(നിവേശം_Array _src1, നിവേശം_Array _src2, ഇരട്ട R)
{
    CV_INSTRUMENT_REGION();

    //Input arrays must have depth CV_8U
    CV_Assert( _src1.type() == _src2.type() );

    ഇരട്ട diff = std::sqrt(norm(_src1, _src2, NORM_L2SQR)/(_src1.total()*_src1.channels()));
    തിരികെ 20*log10(R/(diff+DBL_EPSILON));
}


#ifdef HAVE_OPENCL
സ്ഥിതം തർക്കം ocl_normalize( നിവേശം_Array _src, നിവേശം_Output_Array _dst, നിവേശം_Array _mask, പൂർണ്ണസംഖ്യ dtype,
                           ഇരട്ട scale, ഇരട്ട delta )
{
    UMat src = _src.getUMat();

    എങ്കിൽ( _mask.empty() )
        src.convertTo( _dst, dtype, scale, delta );
    അല്ലെങ്കിൽ എങ്കിൽ (src.channels() <= 4)
    {
        സ്ഥിരം ocl::Device & dev = ocl::Device::getDefault();

        പൂർണ്ണസംഖ്യ stype = _src.type(), sdepth = CV_MAT_DEPTH(stype), cn = CV_MAT_CN(stype),
                ddepth = CV_MAT_DEPTH(dtype), wdepth = std::max(CV_32F, std::max(sdepth, ddepth)),
                rowsPerWI = dev.isIntel() ? 4 : 1;

        ദശാംശം fscale = static_cast<ദശാംശം>(scale), fdelta = static_cast<ദശാംശം>(delta);
        തർക്കം haveScale = std::fabs(scale - 1) > DBL_EPSILON,
                haveZeroScale = !(std::fabs(scale) > DBL_EPSILON),
                haveDelta = std::fabs(delta) > DBL_EPSILON,
                doubleSupport = dev.doubleFPConfig() > 0;

        എങ്കിൽ (!haveScale && !haveDelta && stype == dtype)
        {
            _src.copyTo(_dst, _mask);
            തിരികെ സത്യം;
        }
        എങ്കിൽ (haveZeroScale)
        {
            _dst.setTo(Scalar(delta), _mask);
            തിരികെ സത്യം;
        }

        എങ്കിൽ ((sdepth == CV_64F || ddepth == CV_64F) && !doubleSupport)
            തിരികെ അസത്യം;

        അക്ഷരം cvt[2][50];
        String opts = format("-D srcT=%s -D dstT=%s -D convertToWT=%s -D cn=%d -D rowsPerWI=%d"
                             " -D convertToDT=%s -D workT=%s%s%s%s -D srcT1=%s -D dstT1=%s",
                             ocl::typeToStr(stype), ocl::typeToStr(dtype),
                             ocl::convertTypeStr(sdepth, wdepth, cn, cvt[0], വലുപ്പം(cvt[0])), cn,
                             rowsPerWI, ocl::convertTypeStr(wdepth, ddepth, cn, cvt[1], വലുപ്പം(cvt[1])),
                             ocl::typeToStr(CV_MAKE_TYPE(wdepth, cn)),
                             doubleSupport ? " -D DOUBLE_SUPPORT" : "",
                             haveScale ? " -D HAVE_SCALE" : "",
                             haveDelta ? " -D HAVE_DELTA" : "",
                             ocl::typeToStr(sdepth), ocl::typeToStr(ddepth));

        ocl::Kernel k("normalizek", ocl::core::normalize_oclsrc, opts);
        എങ്കിൽ (k.empty())
            തിരികെ അസത്യം;

        UMat mask = _mask.getUMat(), dst = _dst.getUMat();

        ocl::KernelArg srcarg = ocl::KernelArg::ReadOnlyNoSize(src),
                maskarg = ocl::KernelArg::ReadOnlyNoSize(mask),
                dstarg = ocl::KernelArg::ReadWrite(dst);

        എങ്കിൽ (haveScale)
        {
            എങ്കിൽ (haveDelta)
                k.args(srcarg, maskarg, dstarg, fscale, fdelta);
            അല്ലെങ്കിൽ
                k.args(srcarg, maskarg, dstarg, fscale);
        }
        അല്ലെങ്കിൽ
        {
            എങ്കിൽ (haveDelta)
                k.args(srcarg, maskarg, dstarg, fdelta);
            അല്ലെങ്കിൽ
                k.args(srcarg, maskarg, dstarg);
        }

        size_t globalsize[2] = { (size_t)src.cols, ((size_t)src.rows + rowsPerWI - 1) / rowsPerWI };
        തിരികെ k.run(2, globalsize, ശൂന്യ, അസത്യം);
    }
    അല്ലെങ്കിൽ
    {
        UMat temp;
        src.convertTo( temp, dtype, scale, delta );
        temp.copyTo( _dst, _mask );
    }

    തിരികെ സത്യം;
}  // ocl_normalize
#endif  // HAVE_OPENCL

ശൂന്യം normalize(നിവേശം_Array _src, നിവേശം_Output_Array _dst, ഇരട്ട a, ഇരട്ട b,
               പൂർണ്ണസംഖ്യ norm_type, പൂർണ്ണസംഖ്യ rtype, നിവേശം_Array _mask)
{
    CV_INSTRUMENT_REGION();

    ഇരട്ട scale = 1, shift = 0;
    പൂർണ്ണസംഖ്യ type = _src.type(), depth = CV_MAT_DEPTH(type);

    എങ്കിൽ( rtype < 0 )
        rtype = _dst.fixedType() ? _dst.depth() : depth;

    എങ്കിൽ( norm_type == NORM_MINMAX )
    {
        ഇരട്ട smin = 0, smax = 0;
        ഇരട്ട dmin = MIN( a, b ), dmax = MAX( a, b );
        minMaxIdx( _src, &smin, &smax, 0, 0, _mask );
        scale = (dmax - dmin)*(smax - smin > DBL_EPSILON ? 1./(smax - smin) : 0);
        എങ്കിൽ( rtype == CV_32F )
        {
            scale = (ദശാംശം)scale;
            shift = (ദശാംശം)dmin - (ദശാംശം)(smin*scale);
        }
        അല്ലെങ്കിൽ
            shift = dmin - smin*scale;
    }
    അല്ലെങ്കിൽ എങ്കിൽ( norm_type == NORM_L2 || norm_type == NORM_L1 || norm_type == NORM_INF )
    {
        scale = norm( _src, norm_type, _mask );
        scale = scale > DBL_EPSILON ? a/scale : 0.;
        shift = 0;
    }
    അല്ലെങ്കിൽ
        CV_Error( cv::Error::StsBadArg, "Unknown/unsupported norm type" );

    CV_OCL_RUN(_dst.isUMat(),
               ocl_normalize(_src, _dst, _mask, rtype, scale, shift))

    Mat src = _src.getMat();
    എങ്കിൽ( _mask.empty() )
        src.convertTo( _dst, rtype, scale, shift );
    അല്ലെങ്കിൽ
    {
        Mat temp;
        src.convertTo( temp, rtype, scale, shift );
        temp.copyTo( _dst, _mask );
    }
}

}  // namespace
