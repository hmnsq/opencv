// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html
//
// Copyright (C) 2026, Advanced Micro Devices, Inc., all rights reserved.


#ਅਨ੍ਤਰ੍ਭਾਵ "precomp.hpp"
#ਅਨ੍ਤਰ੍ਭਾਵ "opencl_kernels_core.hpp"
#ਅਨ੍ਤਰ੍ਭਾਵ "stat.hpp"

#ਅਨ੍ਤਰ੍ਭਾਵ "norm.simd.hpp"
#ਅਨ੍ਤਰ੍ਭਾਵ "norm.simd_declarations.hpp"

/****************************************************************************************\
*                                         norm                                           *
\****************************************************************************************/

ਨਾਮਾਕਾਸ਼ cv { ਨਾਮਾਕਾਸ਼ hal {

ਬਾਹਰੀ ਅਚਲ uchar ਕੱਢੋ_ਗਿਣਤੀ_ਸਾਰਣੀ[256] =
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

ਸਥਿਰ ਅਚਲ uchar ਕੱਢੋ_ਗਿਣਤੀ_Table2[] =
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

ਸਥਿਰ ਅਚਲ uchar ਕੱਢੋ_ਗਿਣਤੀ_Table4[] =
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


ਪੂਰਨ normHamming(ਅਚਲ uchar* a, ਪੂਰਨ n, ਪੂਰਨ cell_ਮਾਪ)
{
    ਪੂਰਨ output;
    CALL_HAL_RET(normHamming8u, cv_hal_normHamming8u, output, a, n, cell_ਮਾਪ);

    ਜੇ( cell_ਮਾਪ == 1 )
        ਵਾਪਸ normHamming(a, n);
    ਅਚਲ uchar* tab = 0;
    ਜੇ( cell_ਮਾਪ == 2 )
        tab = ਕੱਢੋ_ਗਿਣਤੀ_Table2;
    ਨਹੀਂਤਾਂ ਜੇ( cell_ਮਾਪ == 4 )
        tab = ਕੱਢੋ_ਗਿਣਤੀ_Table4;
    ਨਹੀਂਤਾਂ
        ਵਾਪਸ -1;
    ਪੂਰਨ i = 0;
    ਪੂਰਨ ਨਤੀਜਾ = 0;
#ਜੇ (CV_SIMD || CV_SIMD_SCALABLE)
    v_uint64 t = vx_setzero_u64();
    ਜੇ ( cell_ਮਾਪ == 2)
    {
        v_uint16 ਨਕਾਬ = v_reinterpret_as_u16(vx_setall_u8(0x55));
        ਲਈ(; i <= n - VTraits<v_uint8>::vlanes(); i += VTraits<v_uint8>::vlanes())
        {
            v_uint16 a0 = v_reinterpret_as_u16(vx_ਲੱਦੋ(a + i));
            t = v_ਜੋੜੋ(t, v_popcount(v_reinterpret_as_u64(v_and(v_or(a0, v_shr<1>(a0)), ਨਕਾਬ))));
        }
    }
    ਨਹੀਂਤਾਂ    // cellSize == 4
    {
        v_uint16 ਨਕਾਬ = v_reinterpret_as_u16(vx_setall_u8(0x11));
        ਲਈ(; i <= n - VTraits<v_uint8>::vlanes(); i += VTraits<v_uint8>::vlanes())
        {
            v_uint16 a0 = v_reinterpret_as_u16(vx_ਲੱਦੋ(a + i));
            v_uint16 a1 = v_or(a0, v_shr<2>(a0));
            t = v_ਜੋੜੋ(t, v_popcount(v_reinterpret_as_u64(v_and(v_or(a1, v_shr<1>(a1)), ਨਕਾਬ))));

        }
    }
    ਨਤੀਜਾ += (ਪੂਰਨ)v_reduce_ਜੋੜ(t);
    vx_cleanup();
#ਜਾਂਜੇ CV_ENABLE_UNROLLED
    ਲਈ( ; i <= n - 4; i += 4 )
        ਨਤੀਜਾ += tab[a[i]] + tab[a[i+1]] + tab[a[i+2]] + tab[a[i+3]];
#ਜੇ_ਅੰਤ
    ਲਈ( ; i < n; i++ )
        ਨਤੀਜਾ += tab[a[i]];
    ਵਾਪਸ ਨਤੀਜਾ;
}

ਪੂਰਨ normHamming(ਅਚਲ uchar* a, ਅਚਲ uchar* b, ਪੂਰਨ n, ਪੂਰਨ cell_ਮਾਪ)
{
    ਪੂਰਨ output;
    CALL_HAL_RET(normHammingDiff8u, cv_hal_normHammingDiff8u, output, a, b, n, cell_ਮਾਪ);

    ਜੇ( cell_ਮਾਪ == 1 )
        ਵਾਪਸ normHamming(a, b, n);
    ਅਚਲ uchar* tab = 0;
    ਜੇ( cell_ਮਾਪ == 2 )
        tab = ਕੱਢੋ_ਗਿਣਤੀ_Table2;
    ਨਹੀਂਤਾਂ ਜੇ( cell_ਮਾਪ == 4 )
        tab = ਕੱਢੋ_ਗਿਣਤੀ_Table4;
    ਨਹੀਂਤਾਂ
        ਵਾਪਸ -1;
    ਪੂਰਨ i = 0;
    ਪੂਰਨ ਨਤੀਜਾ = 0;
#ਜੇ (CV_SIMD || CV_SIMD_SCALABLE)
    v_uint64 t = vx_setzero_u64();
    ਜੇ ( cell_ਮਾਪ == 2)
    {
        v_uint16 ਨਕਾਬ = v_reinterpret_as_u16(vx_setall_u8(0x55));
        ਲਈ(; i <= n - VTraits<v_uint8>::vlanes(); i += VTraits<v_uint8>::vlanes())
        {
            v_uint16 ab0 = v_reinterpret_as_u16(v_xor(vx_ਲੱਦੋ(a + i), vx_ਲੱਦੋ(b + i)));
            t = v_ਜੋੜੋ(t, v_popcount(v_reinterpret_as_u64(v_and(v_or(ab0, v_shr<1>(ab0)), ਨਕਾਬ))));
        }
    }
    ਨਹੀਂਤਾਂ    // cellSize == 4
    {
        v_uint16 ਨਕਾਬ = v_reinterpret_as_u16(vx_setall_u8(0x11));
        ਲਈ(; i <= n - VTraits<v_uint8>::vlanes(); i += VTraits<v_uint8>::vlanes())
        {
            v_uint16 ab0 = v_reinterpret_as_u16(v_xor(vx_ਲੱਦੋ(a + i), vx_ਲੱਦੋ(b + i)));
            v_uint16 ab1 = v_or(ab0, v_shr<2>(ab0));
            t = v_ਜੋੜੋ(t, v_popcount(v_reinterpret_as_u64(v_and(v_or(ab1, v_shr<1>(ab1)), ਨਕਾਬ))));
        }
    }
    ਨਤੀਜਾ += (ਪੂਰਨ)v_reduce_ਜੋੜ(t);
    vx_cleanup();
#ਜਾਂਜੇ CV_ENABLE_UNROLLED
    ਲਈ( ; i <= n - 4; i += 4 )
        ਨਤੀਜਾ += tab[a[i] ^ b[i]] + tab[a[i+1] ^ b[i+1]] +
                tab[a[i+2] ^ b[i+2]] + tab[a[i+3] ^ b[i+3]];
#ਜੇ_ਅੰਤ
    ਲਈ( ; i < n; i++ )
        ਨਤੀਜਾ += tab[a[i] ^ b[i]];
    ਵਾਪਸ ਨਤੀਜਾ;
}

ਦਸ਼ਮਲਵ normL2Sqr_(ਅਚਲ ਦਸ਼ਮਲਵ* a, ਅਚਲ ਦਸ਼ਮਲਵ* b, ਪੂਰਨ n)
{
    ਪੂਰਨ j = 0; ਦਸ਼ਮਲਵ d = 0.f;
#ਜੇ (CV_SIMD || CV_SIMD_SCALABLE)
    ਅਚਲ ਪੂਰਨ vl = VTraits<v_float32>::vlanes();
    v_float32 v_d0 = vx_setzero_f32(), v_d1 = vx_setzero_f32();
    v_float32 v_d2 = vx_setzero_f32(), v_d3 = vx_setzero_f32();
    ਲਈ (; j <= n - 4 * vl; j += 4 * vl)
    {
        v_float32 t0 = v_sub(vx_ਲੱਦੋ(a + j), vx_ਲੱਦੋ(b + j));
        v_float32 t1 = v_sub(vx_ਲੱਦੋ(a + j + vl), vx_ਲੱਦੋ(b + j + vl));
        v_d0 = v_muladd(t0, t0, v_d0);
        v_float32 t2 = v_sub(vx_ਲੱਦੋ(a + j + 2 * vl), vx_ਲੱਦੋ(b + j + 2 * vl));
        v_d1 = v_muladd(t1, t1, v_d1);
        v_float32 t3 = v_sub(vx_ਲੱਦੋ(a + j + 3 * vl), vx_ਲੱਦੋ(b + j + 3 * vl));
        v_d2 = v_muladd(t2, t2, v_d2);
        v_d3 = v_muladd(t3, t3, v_d3);
    }
    v_d0 = v_ਜੋੜੋ(v_ਜੋੜੋ(v_d0, v_d1), v_ਜੋੜੋ(v_d2, v_d3));
    ਲਈ (; j <= n - vl; j += vl)
    {
        v_float32 t0 = v_sub(vx_ਲੱਦੋ(a + j), vx_ਲੱਦੋ(b + j));
        v_d0 = v_muladd(t0, t0, v_d0);
    }
    d = v_reduce_ਜੋੜ(v_d0);
#ਜੇ_ਅੰਤ
    ਲਈ( ; j < n; j++ )
    {
        ਦਸ਼ਮਲਵ t = a[j] - b[j];
        d += t*t;
    }
    ਵਾਪਸ d;
}


ਦਸ਼ਮਲਵ normL1_(ਅਚਲ ਦਸ਼ਮਲਵ* a, ਅਚਲ ਦਸ਼ਮਲਵ* b, ਪੂਰਨ n)
{
    ਪੂਰਨ j = 0; ਦਸ਼ਮਲਵ d = 0.f;
#ਜੇ (CV_SIMD || CV_SIMD_SCALABLE)
    ਅਚਲ ਪੂਰਨ vl = VTraits<v_float32>::vlanes();
    v_float32 v_d0 = vx_setzero_f32(), v_d1 = vx_setzero_f32();
    v_float32 v_d2 = vx_setzero_f32(), v_d3 = vx_setzero_f32();
    ਲਈ (; j <= n - 4 * vl; j += 4 * vl)
    {
        v_d0 = v_ਜੋੜੋ(v_d0, v_absdiff(vx_ਲੱਦੋ(a + j), vx_ਲੱਦੋ(b + j)));
        v_d1 = v_ਜੋੜੋ(v_d1, v_absdiff(vx_ਲੱਦੋ(a + j + vl), vx_ਲੱਦੋ(b + j + vl)));
        v_d2 = v_ਜੋੜੋ(v_d2, v_absdiff(vx_ਲੱਦੋ(a + j + 2 * vl), vx_ਲੱਦੋ(b + j + 2 * vl)));
        v_d3 = v_ਜੋੜੋ(v_d3, v_absdiff(vx_ਲੱਦੋ(a + j + 3 * vl), vx_ਲੱਦੋ(b + j + 3 * vl)));
    }
    v_d0 = v_ਜੋੜੋ(v_ਜੋੜੋ(v_d0, v_d1), v_ਜੋੜੋ(v_d2, v_d3));
    ਲਈ (; j <= n - vl; j += vl)
        v_d0 = v_ਜੋੜੋ(v_d0, v_absdiff(vx_ਲੱਦੋ(a + j), vx_ਲੱਦੋ(b + j)));
    d = v_reduce_ਜੋੜ(v_d0);
#ਜੇ_ਅੰਤ
    ਲਈ( ; j < n; j++ )
        d += std::ਨਿਰਪੇਖ(a[j] - b[j]);
    ਵਾਪਸ d;
}

ਪੂਰਨ normL1_(ਅਚਲ uchar* a, ਅਚਲ uchar* b, ਪੂਰਨ n)
{
    ਪੂਰਨ j = 0, d = 0;
#ਜੇ (CV_SIMD || CV_SIMD_SCALABLE)
    ਅਚਲ ਪੂਰਨ vl = VTraits<v_uint8>::vlanes();
    ਲਈ (; j <= n - 4 * vl; j += 4 * vl)
        d += v_reduce_sad(vx_ਲੱਦੋ(a + j), vx_ਲੱਦੋ(b + j)) +
             v_reduce_sad(vx_ਲੱਦੋ(a + j + vl), vx_ਲੱਦੋ(b + j + vl)) +
             v_reduce_sad(vx_ਲੱਦੋ(a + j + 2 * vl), vx_ਲੱਦੋ(b + j + 2 * vl)) +
             v_reduce_sad(vx_ਲੱਦੋ(a + j + 3 * vl), vx_ਲੱਦੋ(b + j + 3 * vl));
    ਲਈ (; j <= n - vl; j += vl)
        d += v_reduce_sad(vx_ਲੱਦੋ(a + j), vx_ਲੱਦੋ(b + j));
#ਜੇ_ਅੰਤ
    ਲਈ( ; j < n; j++ )
        d += std::ਨਿਰਪੇਖ(a[j] - b[j]);
    ਵਾਪਸ d;
}

} //cv::hal

//==================================================================================================

ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਪੂਰਨ (*NormFunc)(ਅਚਲ uchar*, ਅਚਲ uchar*, uchar*, ਪੂਰਨ, ਪੂਰਨ);
ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਪੂਰਨ (*NormDiffFunc)(ਅਚਲ uchar*, ਅਚਲ uchar*, ਅਚਲ uchar*, uchar*, ਪੂਰਨ, ਪੂਰਨ);

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_OPENCL

ਸਥਿਰ ਤਾਰਕਿਕ ocl_norm( ਨਿਵੇਸ਼_Array _src, ਪੂਰਨ normType, ਨਿਵੇਸ਼_Array ਨਕਾਬ_2, ਦੁੱਗਣਾ & ਨਤੀਜਾ )
{
    ਅਚਲ ocl::Device & d = ocl::Device::getDefault();

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ __ANDROID__
    ਜੇ (d.isNVidia())
        ਵਾਪਸ ਝੂਠ;
#ਜੇ_ਅੰਤ
    ਅਚਲ ਪੂਰਨ cn = _src.channels();
    ਜੇ (cn > 4)
        ਵਾਪਸ ਝੂਠ;
    ਪੂਰਨ type = _src.type(), ਡੂੰਘਾਈ = CV_MAT_ਡੂੰਘਾਈ(type);
    ਤਾਰਕਿਕ doubleSupport = d.doubleFPConfig() > 0,
            have_ਨਕਾਬ = ਨਕਾਬ_2.ਕਿਸਮ() != ਨਿਵੇਸ਼_Array_2::NONE;

    ਜੇ (ਡੂੰਘਾਈ >= CV_16F)
        ਵਾਪਸ ਝੂਠ;  // TODO: support FP16

    ਜੇ ( !(normType == NORM_INF || normType == NORM_L1 || normType == NORM_L2 || normType == NORM_L2SQR) ||
         (!doubleSupport && ਡੂੰਘਾਈ == CV_64F))
        ਵਾਪਸ ਝੂਠ;

    UMat src = _src.getUMat();

    ਜੇ (normType == NORM_INF)
    {
        ਜੇ (!ocl_minMaxIdx(_src, ਨਿਰਾ, &ਨਤੀਜਾ, ਨਿਰਾ, ਨਿਰਾ, ਨਕਾਬ_2,
                           std::max(ਡੂੰਘਾਈ, CV_32S), ਡੂੰਘਾਈ != CV_8U && ਡੂੰਘਾਈ != CV_16U))
            ਵਾਪਸ ਝੂਠ;
    }
    ਨਹੀਂਤਾਂ ਜੇ (normType == NORM_L1 || normType == NORM_L2 || normType == NORM_L2SQR)
    {
        Scalar sc;
        ਤਾਰਕਿਕ unstype = ਡੂੰਘਾਈ == CV_8U || ਡੂੰਘਾਈ == CV_16U;

        ਜੇ ( !ocl_ਜੋੜ(have_ਨਕਾਬ ? src : src.reshape(1), sc, normType == NORM_L2 || normType == NORM_L2SQR ?
                    OCL_OP_ਜੋੜ_SQR : (unstype ? OCL_OP_ਜੋੜ : OCL_OP_ਜੋੜ_ABS), ਨਕਾਬ_2) )
            ਵਾਪਸ ਝੂਠ;

        ਦੁੱਗਣਾ s = 0.0;
        ਲਈ (ਪੂਰਨ i = 0; i < (have_ਨਕਾਬ ? cn : 1); ++i)
            s += sc[i];

        ਨਤੀਜਾ = normType == NORM_L1 || normType == NORM_L2SQR ? s : std::ਵਰਗਮੂਲ(s);
    }

    ਵਾਪਸ ਸੱਚ;
}

#ਜੇ_ਅੰਤ

ਸਥਿਰ NormFunc getNormFunc(ਪੂਰਨ normType, ਪੂਰਨ ਡੂੰਘਾਈ) {
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getNormFunc, (normType, ਡੂੰਘਾਈ), CV_CPU_DISPATCH_MODES_ALL);
}
ਸਥਿਰ NormDiffFunc getNormDiffFunc(ਪੂਰਨ normType, ਪੂਰਨ ਡੂੰਘਾਈ) {
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getNormDiffFunc, (normType, ਡੂੰਘਾਈ), CV_CPU_DISPATCH_MODES_ALL);
}

ਦੁੱਗਣਾ norm( ਨਿਵੇਸ਼_Array _src, ਪੂਰਨ normType, ਨਿਵੇਸ਼_Array ਨਕਾਬ_2 )
{
    CV_INSTRUMENT_REGION();

    normType &= NORM_TYPE_ਨਕਾਬ;
    CV_Assert( normType == NORM_INF || normType == NORM_L1 ||
               normType == NORM_L2 || normType == NORM_L2SQR ||
               ((normType == NORM_HAMMING || normType == NORM_HAMMING2) && _src.type() == CV_8U) );

#ਜੇ defined HAVE_OPENCL
    ਦੁੱਗਣਾ ਨਤੀਜਾ_2 = 0;
#ਜੇ_ਅੰਤ

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_OPENCL
    CV_OCL_ਲੜੀਵਾਰ_2(OCL_PERFORMANCE_CHECK(_src.isUMat()) && _src.dims() <= 2,
                ocl_norm(_src, normType, ਨਕਾਬ_2, ਨਤੀਜਾ_2),
                ਨਤੀਜਾ_2)
#ਜੇ_ਅੰਤ

    Mat src = _src.getMat(), ਨਕਾਬ = ਨਕਾਬ_2.getMat();
    CV_Assert( ਨਕਾਬ.empty() || ((ਨਕਾਬ.type() == CV_8U || ਨਕਾਬ.type() == CV_8S || ਨਕਾਬ.type() == CV_Bool) && src.size == ਨਕਾਬ.size) );
    ਜੇ( src.empty() )
        ਵਾਪਸ 0.0;
    ਪੂਰਨ ਡੂੰਘਾਈ = src.ਡੂੰਘਾਈ(), cn = src.channels();
    ਜੇ( src.dims <= 2 )
    {
        ਦੁੱਗਣਾ ਨਤੀਜਾ;
        CALL_HAL_RET(norm, cv_hal_norm, ਨਤੀਜਾ, src.ਅੰਕੜੇ, src.ਕਦਮ, ਨਕਾਬ.ਅੰਕੜੇ, ਨਕਾਬ.ਕਦਮ, src.ਖਾਨੇ_ਗਿਣਤੀ, src.ਪੰਕਤੀਆਂ, src.type(), normType);
    }
    ਨਹੀਂਤਾਂ ਜੇ( src.isContinuous() && ਨਕਾਬ.isContinuous() )
    {
        ਦੁੱਗਣਾ ਨਤੀਜਾ;
        CALL_HAL_RET(norm, cv_hal_norm, ਨਤੀਜਾ, src.ਅੰਕੜੇ, 0, ਨਕਾਬ.ਅੰਕੜੇ, 0, (ਪੂਰਨ)src.ਕੁੱਲ(), 1, src.type(), normType);
    }

    NormFunc func = getNormFunc(normType >> 1, ਡੂੰਘਾਈ);
    CV_Assert( (normType >> 1) >= 3 || func != 0 );

    ਜੇ( src.isContinuous() && ਨਕਾਬ.empty() )
    {
        size_t len = src.ਕੁੱਲ()*cn;
        ਜੇ( len == (size_t)(ਪੂਰਨ)len )
        {
            ਜੇ( ਡੂੰਘਾਈ == CV_32F )
            {
                ਅਚਲ uchar* ਅੰਕੜੇ = src.ptr<ਅਚਲ uchar>();

                ਜੇ( normType == NORM_L2 || normType == NORM_L2SQR || normType == NORM_L1 )
                {
                    ਦੁੱਗਣਾ ਨਤੀਜਾ = 0;
                    func(ਅੰਕੜੇ, 0, (uchar*)&ਨਤੀਜਾ, (ਪੂਰਨ)len, 1);
                    ਵਾਪਸ normType == NORM_L2 ? std::ਵਰਗਮੂਲ(ਨਤੀਜਾ) : ਨਤੀਜਾ;
                }
                ਜੇ( normType == NORM_INF )
                {
                    ਦਸ਼ਮਲਵ ਨਤੀਜਾ = 0;
                    func(ਅੰਕੜੇ, 0, (uchar*)&ਨਤੀਜਾ, (ਪੂਰਨ)len, 1);
                    ਵਾਪਸ ਨਤੀਜਾ;
                }
            }
            ਜੇ( ਡੂੰਘਾਈ == CV_8U )
            {
                ਅਚਲ uchar* ਅੰਕੜੇ = src.ptr<uchar>();

                ਜੇ( normType == NORM_HAMMING )
                {
                    ਵਾਪਸ hal::normHamming(ਅੰਕੜੇ, (ਪੂਰਨ)len, 1);
                }

                ਜੇ( normType == NORM_HAMMING2 )
                {
                    ਵਾਪਸ hal::normHamming(ਅੰਕੜੇ, (ਪੂਰਨ)len, 2);
                }
            }
        }
    }

    CV_Assert( ਨਕਾਬ.empty() || ਨਕਾਬ.type() == CV_8U || ਨਕਾਬ.type() == CV_8S || ਨਕਾਬ.type() == CV_Bool );

    ਜੇ( normType == NORM_HAMMING || normType == NORM_HAMMING2 )
    {
        ਜੇ( !ਨਕਾਬ.empty() )
        {
            Mat ਆਰਜ਼ੀ_2;
            bitwise_and(src, ਨਕਾਬ, ਆਰਜ਼ੀ_2);
            ਵਾਪਸ norm(ਆਰਜ਼ੀ_2, normType);
        }
        ਪੂਰਨ cell_ਮਾਪ = normType == NORM_HAMMING ? 1 : 2;

        ਅਚਲ Mat* arrays[] = {&src, 0};
        uchar* ptrs[1] = {};
        NAryMatIterator it(arrays, ptrs);
        ਪੂਰਨ ਕੁੱਲ = (ਪੂਰਨ)it.size;
        ਪੂਰਨ ਨਤੀਜਾ = 0;

        ਲਈ( size_t i = 0; i < it.nplanes; i++, ++it )
        {
            ਨਤੀਜਾ += hal::normHamming(ptrs[0], ਕੁੱਲ, cell_ਮਾਪ);
        }

        ਵਾਪਸ ਨਤੀਜਾ;
    }

    ਅਚਲ Mat* arrays[] = {&src, &ਨਕਾਬ, 0};
    uchar* ptrs[2] = {};
    ਸੰਘ
    {
        ਦੁੱਗਣਾ d;
        ਬਿਨਾਨਿਸ਼ਾਨੀ u;
        uint64 UL;
        ਦਸ਼ਮਲਵ f;
    }
    ਨਤੀਜਾ;
    ਨਤੀਜਾ.d = 0;
    NAryMatIterator it(arrays, ptrs);
    CV_CheckLT((size_t)it.size, (size_t)ਵੱਧ_ਤੋਂ_ਵੱਧ_ਅੰਕ, "");
    ਤਾਰਕਿਕ is_fp16 = ਡੂੰਘਾਈ == CV_16F || ਡੂੰਘਾਈ == CV_16BF;

    ਜੇ ((normType == NORM_L1 && (ਡੂੰਘਾਈ <= CV_16S || ਡੂੰਘਾਈ == CV_Bool || is_fp16)) ||
        ((normType == NORM_L2 || normType == NORM_L2SQR) && (ਡੂੰਘਾਈ <= CV_8S || ਡੂੰਘਾਈ == CV_Bool || is_fp16)))
    {
        // special case to handle "integer" overflow in accumulator
        ਅਚਲ size_t esz = src.elem_ਮਾਪ();
        ਅਚਲ ਪੂਰਨ ਕੁੱਲ = (ਪੂਰਨ)it.size;
        ਅਚਲ ਪੂਰਨ ਖੰਡ_Size0 = (is_fp16 ? (1 << 10) :
            ਡੂੰਘਾਈ == CV_Bool ? (1 << 30) :
            normType == NORM_L1 && ਡੂੰਘਾਈ <= CV_8S ? (1 << 23) : (1 << 15))/cn;
        ਅਚਲ ਪੂਰਨ ਖੰਡ_ਮਾਪ_2 = std::min(ਕੁੱਲ, ਖੰਡ_Size0);
        ਸੰਘ {
            ਪੂਰਨ i;
            ਦਸ਼ਮਲਵ f;
        } blocksum;
        blocksum.i = 0;
        ਪੂਰਨ ਗਿਣਤੀ_3 = 0;

        ਲਈ (size_t i = 0; i < it.nplanes; i++, ++it)
        {
            ਲਈ (ਪੂਰਨ j = 0; j < ਕੁੱਲ; j += ਖੰਡ_ਮਾਪ_2)
            {
                ਪੂਰਨ bsz = std::min(ਕੁੱਲ - j, ਖੰਡ_ਮਾਪ_2);
                func(ptrs[0], ptrs[1], (uchar*)&blocksum, bsz, cn);
                ਗਿਣਤੀ_3 += bsz;
                ਜੇ (ਗਿਣਤੀ_3 + ਖੰਡ_ਮਾਪ_2 >= ਖੰਡ_Size0 || (i+1 >= it.nplanes && j+bsz >= ਕੁੱਲ))
                {
                    ਨਤੀਜਾ.d += is_fp16 ? (ਦੁੱਗਣਾ)blocksum.f : (ਦੁੱਗਣਾ)blocksum.i;
                    blocksum.i = 0;
                    ਗਿਣਤੀ_3 = 0;
                }
                ptrs[0] += bsz*esz;
                ਜੇ (ptrs[1])
                    ptrs[1] += bsz;
            }
        }
    }
    ਨਹੀਂਤਾਂ
    {
        // generic implementation
        ਲਈ (size_t i = 0; i < it.nplanes; i++, ++it)
        {
            func(ptrs[0], ptrs[1], (uchar*)&ਨਤੀਜਾ, (ਪੂਰਨ)it.size, cn);
        }
    }

    ਜੇ( normType == NORM_INF )
    {
        ਜੇ(ਡੂੰਘਾਈ <= CV_32S || ਡੂੰਘਾਈ == CV_32U || ਡੂੰਘਾਈ == CV_Bool)
            ਵਾਪਸ ਨਤੀਜਾ.u;
        ਜੇ (ਡੂੰਘਾਈ == CV_32F || is_fp16)
            ਵਾਪਸ ਨਤੀਜਾ.f;
        ਜੇ (ਡੂੰਘਾਈ == CV_64U || ਡੂੰਘਾਈ == CV_64S)
            ਵਾਪਸ (ਦੁੱਗਣਾ)ਨਤੀਜਾ.UL;
    }
    ਜੇ( normType == NORM_L2 )
        ਵਾਪਸ std::ਵਰਗਮੂਲ(ਨਤੀਜਾ.d);

    ਵਾਪਸ ਨਤੀਜਾ.d;
}

//==================================================================================================

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_OPENCL
ਸਥਿਰ ਤਾਰਕਿਕ ocl_norm( ਨਿਵੇਸ਼_Array _src1, ਨਿਵੇਸ਼_Array _src2, ਪੂਰਨ normType, ਨਿਵੇਸ਼_Array ਨਕਾਬ_2, ਦੁੱਗਣਾ & ਨਤੀਜਾ )
{
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ __ANDROID__
    ਜੇ (ocl::Device::getDefault().isNVidia())
        ਵਾਪਸ ਝੂਠ;
#ਜੇ_ਅੰਤ

    Scalar sc1, sc2;
    ਪੂਰਨ cn = _src1.channels();
    ਜੇ (cn > 4)
        ਵਾਪਸ ਝੂਠ;
    ਪੂਰਨ type = _src1.type(), ਡੂੰਘਾਈ = CV_MAT_ਡੂੰਘਾਈ(type);
    ਤਾਰਕਿਕ relative = (normType & NORM_RELATIVE) != 0;
    normType &= ~NORM_RELATIVE;
    ਤਾਰਕਿਕ normsum = normType == NORM_L1 || normType == NORM_L2 || normType == NORM_L2SQR;

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ __APPLE__
    ਜੇ(normType == NORM_L1 && type == CV_16UC3 && !ਨਕਾਬ_2.empty())
        ਵਾਪਸ ਝੂਠ;
#ਜੇ_ਅੰਤ

    ਜੇ (normsum)
    {
        ਜੇ (!ocl_ਜੋੜ(_src1, sc1, normType == NORM_L2 || normType == NORM_L2SQR ?
                     OCL_OP_ਜੋੜ_SQR : OCL_OP_ਜੋੜ, ਨਕਾਬ_2, _src2, relative, sc2))
            ਵਾਪਸ ਝੂਠ;
    }
    ਨਹੀਂਤਾਂ
    {
        ਜੇ (!ocl_minMaxIdx(_src1, ਨਿਰਾ, &sc1[0], ਨਿਰਾ, ਨਿਰਾ, ਨਕਾਬ_2, std::max(CV_32S, ਡੂੰਘਾਈ),
                           ਝੂਠ, _src2, relative ? &sc2[0] : ਨਿਰਾ))
            ਵਾਪਸ ਝੂਠ;
        cn = 1;
    }

    ਦੁੱਗਣਾ s2 = 0;
    ਲਈ (ਪੂਰਨ i = 0; i < cn; ++i)
    {
        ਨਤੀਜਾ += sc1[i];
        ਜੇ (relative)
            s2 += sc2[i];
    }

    ਜੇ (normType == NORM_L2)
    {
        ਨਤੀਜਾ = std::ਵਰਗਮੂਲ(ਨਤੀਜਾ);
        ਜੇ (relative)
            s2 = std::ਵਰਗਮੂਲ(s2);
    }

    ਜੇ (relative)
        ਨਤੀਜਾ /= (s2 + DBL_EPSILON);

    ਵਾਪਸ ਸੱਚ;
}  // ocl_norm()
#ਜੇ_ਅੰਤ  // HAVE_OPENCL

ਦੁੱਗਣਾ norm( ਨਿਵੇਸ਼_Array _src1, ਨਿਵੇਸ਼_Array _src2, ਪੂਰਨ normType, ਨਿਵੇਸ਼_Array ਨਕਾਬ_2 )
{
    CV_INSTRUMENT_REGION();

    CV_CheckTypeEQ(_src1.type(), _src2.type(), "Input type mismatch");
    CV_Assert(_src1.same_ਮਾਪ(_src2));

#ਜੇ defined HAVE_OPENCL
    ਦੁੱਗਣਾ ਨਤੀਜਾ_2 = 0;
#ਜੇ_ਅੰਤ

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_OPENCL
    CV_OCL_ਲੜੀਵਾਰ_2(OCL_PERFORMANCE_CHECK(_src1.isUMat()),
                ocl_norm(_src1, _src2, normType, ਨਕਾਬ_2, ਨਤੀਜਾ_2),
                ਨਤੀਜਾ_2)
#ਜੇ_ਅੰਤ

    Mat src1 = _src1.getMat(), src2 = _src2.getMat(), ਨਕਾਬ = ਨਕਾਬ_2.getMat();
    CV_Assert( ਨਕਾਬ.empty() || ((ਨਕਾਬ.type() == CV_8U || ਨਕਾਬ.type() == CV_8S || ਨਕਾਬ.type() == CV_Bool) && src1.size == ਨਕਾਬ.size) );
    ਜੇ( src1.empty() || src2.empty() )
        ਵਾਪਸ 0.0;
    ਪੂਰਨ ਡੂੰਘਾਈ = src1.ਡੂੰਘਾਈ(), cn = src1.channels();
    ਜੇ( src1.dims <= 2 )
    {
        ਦੁੱਗਣਾ ਨਤੀਜਾ;
        CALL_HAL_RET(normDiff, cv_hal_normDiff, ਨਤੀਜਾ, src1.ਅੰਕੜੇ, src1.ਕਦਮ, src2.ਅੰਕੜੇ, src2.ਕਦਮ, ਨਕਾਬ.ਅੰਕੜੇ, ਨਕਾਬ.ਕਦਮ, src1.ਖਾਨੇ_ਗਿਣਤੀ, src1.ਪੰਕਤੀਆਂ, src1.type(), normType);
    }
    ਨਹੀਂਤਾਂ ਜੇ( src1.isContinuous() && src2.isContinuous() && ਨਕਾਬ.isContinuous() )
    {
        ਦੁੱਗਣਾ ਨਤੀਜਾ;
        CALL_HAL_RET(normDiff, cv_hal_normDiff, ਨਤੀਜਾ, src1.ਅੰਕੜੇ, 0, src2.ਅੰਕੜੇ, 0, ਨਕਾਬ.ਅੰਕੜੇ, 0, (ਪੂਰਨ)src1.ਕੁੱਲ(), 1, src1.type(), normType);
    }

    ਜੇ( normType & NORM_RELATIVE )
    {
        ਵਾਪਸ norm(_src1, _src2, normType & ~NORM_RELATIVE, ਨਕਾਬ_2)/(norm(_src2, normType, ਨਕਾਬ_2) + DBL_EPSILON);
    }

    normType &= 7;
    CV_Assert( normType == NORM_INF || normType == NORM_L1 ||
               normType == NORM_L2 || normType == NORM_L2SQR ||
              ((normType == NORM_HAMMING || normType == NORM_HAMMING2) && src1.type() == CV_8U) );

    NormDiffFunc func = getNormDiffFunc(normType >> 1, ਡੂੰਘਾਈ);
    CV_Assert( (normType >> 1) >= 3 || func != 0 );

    ਜੇ( src1.isContinuous() && src2.isContinuous() && ਨਕਾਬ.empty() )
    {
        size_t len = src1.ਕੁੱਲ()*src1.channels();
        ਜੇ( len == (size_t)(ਪੂਰਨ)len )
        {
            ਜੇ( src1.ਡੂੰਘਾਈ() == CV_32F )
            {
                ਅਚਲ uchar* data1 = src1.ptr<ਅਚਲ uchar>();
                ਅਚਲ uchar* data2 = src2.ptr<ਅਚਲ uchar>();

                ਜੇ( normType == NORM_L2 || normType == NORM_L2SQR || normType == NORM_L1 )
                {
                    ਦੁੱਗਣਾ ਨਤੀਜਾ = 0;
                    func(data1, data2, 0, (uchar*)&ਨਤੀਜਾ, (ਪੂਰਨ)len, 1);
                    ਵਾਪਸ normType == NORM_L2 ? std::ਵਰਗਮੂਲ(ਨਤੀਜਾ) : ਨਤੀਜਾ;
                }
                ਜੇ( normType == NORM_INF )
                {
                    ਦਸ਼ਮਲਵ ਨਤੀਜਾ = 0;
                    func(data1, data2, 0, (uchar*)&ਨਤੀਜਾ, (ਪੂਰਨ)len, 1);
                    ਵਾਪਸ ਨਤੀਜਾ;
                }
            }
        }
    }

    CV_Assert( ਨਕਾਬ.empty() || ਨਕਾਬ.type() == CV_8U || ਨਕਾਬ.type() == CV_8S || ਨਕਾਬ.type() == CV_Bool );

    ਜੇ( normType == NORM_HAMMING || normType == NORM_HAMMING2 )
    {
        ਜੇ( !ਨਕਾਬ.empty() )
        {
            Mat ਆਰਜ਼ੀ_2;
            bitwise_xor(src1, src2, ਆਰਜ਼ੀ_2);
            bitwise_and(ਆਰਜ਼ੀ_2, ਨਕਾਬ, ਆਰਜ਼ੀ_2);
            ਵਾਪਸ norm(ਆਰਜ਼ੀ_2, normType);
        }
        ਪੂਰਨ cell_ਮਾਪ = normType == NORM_HAMMING ? 1 : 2;

        ਅਚਲ Mat* arrays[] = {&src1, &src2, 0};
        uchar* ptrs[2] = {};
        NAryMatIterator it(arrays, ptrs);
        ਪੂਰਨ ਕੁੱਲ = (ਪੂਰਨ)it.size;
        ਪੂਰਨ ਨਤੀਜਾ = 0;

        ਲਈ( size_t i = 0; i < it.nplanes; i++, ++it )
        {
            ਨਤੀਜਾ += hal::normHamming(ptrs[0], ptrs[1], ਕੁੱਲ, cell_ਮਾਪ);
        }

        ਵਾਪਸ ਨਤੀਜਾ;
    }

    ਅਚਲ Mat* arrays[] = {&src1, &src2, &ਨਕਾਬ, 0};
    uchar* ptrs[3] = {};
    ਸੰਘ
    {
        ਦੁੱਗਣਾ d;
        ਦਸ਼ਮਲਵ f;
        ਬਿਨਾਨਿਸ਼ਾਨੀ u;
        uint64 UL;
    }
    ਨਤੀਜਾ;
    ਨਤੀਜਾ.d = 0;
    NAryMatIterator it(arrays, ptrs);
    CV_CheckLT((size_t)it.size, (size_t)ਵੱਧ_ਤੋਂ_ਵੱਧ_ਅੰਕ, "");

    ਤਾਰਕਿਕ is_fp16 = ਡੂੰਘਾਈ == CV_16F || ਡੂੰਘਾਈ == CV_16BF;

    ਜੇ ((normType == NORM_L1 && (ਡੂੰਘਾਈ <= CV_16S || ਡੂੰਘਾਈ == CV_Bool || is_fp16)) ||
        ((normType == NORM_L2 || normType == NORM_L2SQR) && (ਡੂੰਘਾਈ <= CV_8S || ਡੂੰਘਾਈ == CV_Bool || is_fp16)))
    {
        // special case to handle "integer" overflow in accumulator
        ਅਚਲ size_t esz = src1.elem_ਮਾਪ();
        ਅਚਲ ਪੂਰਨ ਕੁੱਲ = (ਪੂਰਨ)it.size;
        ਅਚਲ ਪੂਰਨ ਖੰਡ_Size0 = (is_fp16 ? (1 << 10) :
            ਡੂੰਘਾਈ == CV_Bool ? (1 << 30) :
            normType == NORM_L1 && ਡੂੰਘਾਈ <= CV_8S ? (1 << 23) : (1 << 15))/cn;
        ਅਚਲ ਪੂਰਨ ਖੰਡ_ਮਾਪ_2 = std::min(ਕੁੱਲ, ਖੰਡ_Size0);
        ਸੰਘ {
            ਪੂਰਨ i;
            ਦਸ਼ਮਲਵ f;
        } blocksum;
        blocksum.i = 0;
        ਪੂਰਨ ਗਿਣਤੀ_3 = 0;

        ਲਈ (size_t i = 0; i < it.nplanes; i++, ++it)
        {
            ਲਈ (ਪੂਰਨ j = 0; j < ਕੁੱਲ; j += ਖੰਡ_ਮਾਪ_2)
            {
                ਪੂਰਨ bsz = std::min(ਕੁੱਲ - j, ਖੰਡ_ਮਾਪ_2);
                func(ptrs[0], ptrs[1], ptrs[2], (uchar*)&blocksum, bsz, cn);
                ਗਿਣਤੀ_3 += bsz;
                ਜੇ (ਗਿਣਤੀ_3 + ਖੰਡ_ਮਾਪ_2 >= ਖੰਡ_Size0 || (i+1 >= it.nplanes && j+bsz >= ਕੁੱਲ))
                {
                    ਨਤੀਜਾ.d += is_fp16 ? (ਦੁੱਗਣਾ)blocksum.f : (ਦੁੱਗਣਾ)blocksum.i;
                    blocksum.i = 0;
                    ਗਿਣਤੀ_3 = 0;
                }
                ptrs[0] += bsz*esz;
                ptrs[1] += bsz*esz;
                ਜੇ (ptrs[2])
                    ptrs[2] += bsz;
            }
        }
    }
    ਨਹੀਂਤਾਂ
    {
        // generic implementation
        ਲਈ (size_t i = 0; i < it.nplanes; i++, ++it)
        {
            func(ptrs[0], ptrs[1], ptrs[2], (uchar*)&ਨਤੀਜਾ, (ਪੂਰਨ)it.size, cn);
        }
    }

    ਜੇ( normType == NORM_INF )
    {
        ਜੇ (ਡੂੰਘਾਈ <= CV_32S || ਡੂੰਘਾਈ == CV_32U || ਡੂੰਘਾਈ == CV_Bool)
            ਵਾਪਸ ਨਤੀਜਾ.u;
        ਜੇ (ਡੂੰਘਾਈ == CV_32F || is_fp16)
            ਵਾਪਸ ਨਤੀਜਾ.f;
        ਜੇ (ਡੂੰਘਾਈ == CV_64U || ਡੂੰਘਾਈ == CV_64S)
            ਵਾਪਸ (ਦੁੱਗਣਾ)ਨਤੀਜਾ.UL;
    }
    ਜੇ( normType == NORM_L2 )
        ਵਾਪਸ std::ਵਰਗਮੂਲ(ਨਤੀਜਾ.d);

    ਵਾਪਸ ਨਤੀਜਾ.d;
}

cv::Hamming::ਨਤੀਜਾ_Type Hamming::ਸੰਚਾਲਕ()( ਅਚਲ ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ* a, ਅਚਲ ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ* b, ਪੂਰਨ size ) ਅਚਲ
{
    ਵਾਪਸ cv::hal::normHamming(a, b, size);
}

ਦੁੱਗਣਾ PSNR(ਨਿਵੇਸ਼_Array _src1, ਨਿਵੇਸ਼_Array _src2, ਦੁੱਗਣਾ R)
{
    CV_INSTRUMENT_REGION();

    //Input arrays must have depth CV_8U
    CV_Assert( _src1.type() == _src2.type() );

    ਦੁੱਗਣਾ diff = std::ਵਰਗਮੂਲ(norm(_src1, _src2, NORM_L2SQR)/(_src1.ਕੁੱਲ()*_src1.channels()));
    ਵਾਪਸ 20*log10(R/(diff+DBL_EPSILON));
}


#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_OPENCL
ਸਥਿਰ ਤਾਰਕਿਕ ocl_normalize( ਨਿਵੇਸ਼_Array _src, ਨਿਵੇਸ਼_Output_Array _dst, ਨਿਵੇਸ਼_Array ਨਕਾਬ_2, ਪੂਰਨ dtype,
                           ਦੁੱਗਣਾ scale, ਦੁੱਗਣਾ delta )
{
    UMat src = _src.getUMat();

    ਜੇ( ਨਕਾਬ_2.empty() )
        src.convert_ਮੰਜ਼ਿਲ( _dst, dtype, scale, delta );
    ਨਹੀਂਤਾਂ ਜੇ (src.channels() <= 4)
    {
        ਅਚਲ ocl::Device & dev = ocl::Device::getDefault();

        ਪੂਰਨ stype = _src.type(), sdepth = CV_MAT_ਡੂੰਘਾਈ(stype), cn = CV_MAT_CN(stype),
                ddepth = CV_MAT_ਡੂੰਘਾਈ(dtype), wdepth = std::max(CV_32F, std::max(sdepth, ddepth)),
                ਪੰਕਤੀਆਂ_ਪ੍ਰਤੀ_WI = dev.isIntel() ? 4 : 1;

        ਦਸ਼ਮਲਵ fscale = static_cast<ਦਸ਼ਮਲਵ>(scale), fdelta = static_cast<ਦਸ਼ਮਲਵ>(delta);
        ਤਾਰਕਿਕ haveScale = std::fabs(scale - 1) > DBL_EPSILON,
                haveZeroScale = !(std::fabs(scale) > DBL_EPSILON),
                haveDelta = std::fabs(delta) > DBL_EPSILON,
                doubleSupport = dev.doubleFPConfig() > 0;

        ਜੇ (!haveScale && !haveDelta && stype == dtype)
        {
            _src.copy_ਮੰਜ਼ਿਲ(_dst, ਨਕਾਬ_2);
            ਵਾਪਸ ਸੱਚ;
        }
        ਜੇ (haveZeroScale)
        {
            _dst.ਸੈੱਟ_ਮੰਜ਼ਿਲ(Scalar(delta), ਨਕਾਬ_2);
            ਵਾਪਸ ਸੱਚ;
        }

        ਜੇ ((sdepth == CV_64F || ddepth == CV_64F) && !doubleSupport)
            ਵਾਪਸ ਝੂਠ;

        ਅੱਖਰ cvt[2][50];
        String opts = format("-D srcT=%s -D dstT=%s -D convertToWT=%s -D cn=%d -D rowsPerWI=%d"
                             " -D convertToDT=%s -D workT=%s%s%s%s -D srcT1=%s -D dstT1=%s",
                             ocl::type_ਮੰਜ਼ਿਲ_Str(stype), ocl::type_ਮੰਜ਼ਿਲ_Str(dtype),
                             ocl::convertTypeStr(sdepth, wdepth, cn, cvt[0], ਆਕਾਰ(cvt[0])), cn,
                             ਪੰਕਤੀਆਂ_ਪ੍ਰਤੀ_WI, ocl::convertTypeStr(wdepth, ddepth, cn, cvt[1], ਆਕਾਰ(cvt[1])),
                             ocl::type_ਮੰਜ਼ਿਲ_Str(CV_MAKE_TYPE(wdepth, cn)),
                             doubleSupport ? " -D DOUBLE_SUPPORT" : "",
                             haveScale ? " -D HAVE_SCALE" : "",
                             haveDelta ? " -D HAVE_DELTA" : "",
                             ocl::type_ਮੰਜ਼ਿਲ_Str(sdepth), ocl::type_ਮੰਜ਼ਿਲ_Str(ddepth));

        ocl::Kernel k("normalizek", ocl::core::normalize_oclsrc, opts);
        ਜੇ (k.empty())
            ਵਾਪਸ ਝੂਠ;

        UMat ਨਕਾਬ = ਨਕਾਬ_2.getUMat(), dst = _dst.getUMat();

        ocl::KernelArg srcarg = ocl::KernelArg::Read_Only_No_ਮਾਪ(src),
                maskarg = ocl::KernelArg::Read_Only_No_ਮਾਪ(ਨਕਾਬ),
                dstarg = ocl::KernelArg::ReadWrite(dst);

        ਜੇ (haveScale)
        {
            ਜੇ (haveDelta)
                k.ਦਲੀਲਾਂ(srcarg, maskarg, dstarg, fscale, fdelta);
            ਨਹੀਂਤਾਂ
                k.ਦਲੀਲਾਂ(srcarg, maskarg, dstarg, fscale);
        }
        ਨਹੀਂਤਾਂ
        {
            ਜੇ (haveDelta)
                k.ਦਲੀਲਾਂ(srcarg, maskarg, dstarg, fdelta);
            ਨਹੀਂਤਾਂ
                k.ਦਲੀਲਾਂ(srcarg, maskarg, dstarg);
        }

        size_t globalsize[2] = { (size_t)src.ਖਾਨੇ_ਗਿਣਤੀ, ((size_t)src.ਪੰਕਤੀਆਂ + ਪੰਕਤੀਆਂ_ਪ੍ਰਤੀ_WI - 1) / ਪੰਕਤੀਆਂ_ਪ੍ਰਤੀ_WI };
        ਵਾਪਸ k.ਲੜੀਵਾਰ(2, globalsize, ਨਿਰਾ, ਝੂਠ);
    }
    ਨਹੀਂਤਾਂ
    {
        UMat ਆਰਜ਼ੀ_2;
        src.convert_ਮੰਜ਼ਿਲ( ਆਰਜ਼ੀ_2, dtype, scale, delta );
        ਆਰਜ਼ੀ_2.copy_ਮੰਜ਼ਿਲ( _dst, ਨਕਾਬ_2 );
    }

    ਵਾਪਸ ਸੱਚ;
}  // ocl_normalize
#ਜੇ_ਅੰਤ  // HAVE_OPENCL

ਖਾਲੀ normalize(ਨਿਵੇਸ਼_Array _src, ਨਿਵੇਸ਼_Output_Array _dst, ਦੁੱਗਣਾ a, ਦੁੱਗਣਾ b,
               ਪੂਰਨ norm_type, ਪੂਰਨ rtype, ਨਿਵੇਸ਼_Array ਨਕਾਬ_2)
{
    CV_INSTRUMENT_REGION();

    ਦੁੱਗਣਾ scale = 1, ਖਿਸਕਾਓ = 0;
    ਪੂਰਨ type = _src.type(), ਡੂੰਘਾਈ = CV_MAT_ਡੂੰਘਾਈ(type);

    ਜੇ( rtype < 0 )
        rtype = _dst.fixedType() ? _dst.ਡੂੰਘਾਈ() : ਡੂੰਘਾਈ;

    ਜੇ( norm_type == NORM_MINMAX )
    {
        ਦੁੱਗਣਾ smin = 0, smax = 0;
        ਦੁੱਗਣਾ dmin = MIN( a, b ), dmax = MAX( a, b );
        minMaxIdx( _src, &smin, &smax, 0, 0, ਨਕਾਬ_2 );
        scale = (dmax - dmin)*(smax - smin > DBL_EPSILON ? 1./(smax - smin) : 0);
        ਜੇ( rtype == CV_32F )
        {
            scale = (ਦਸ਼ਮਲਵ)scale;
            ਖਿਸਕਾਓ = (ਦਸ਼ਮਲਵ)dmin - (ਦਸ਼ਮਲਵ)(smin*scale);
        }
        ਨਹੀਂਤਾਂ
            ਖਿਸਕਾਓ = dmin - smin*scale;
    }
    ਨਹੀਂਤਾਂ ਜੇ( norm_type == NORM_L2 || norm_type == NORM_L1 || norm_type == NORM_INF )
    {
        scale = norm( _src, norm_type, ਨਕਾਬ_2 );
        scale = scale > DBL_EPSILON ? a/scale : 0.;
        ਖਿਸਕਾਓ = 0;
    }
    ਨਹੀਂਤਾਂ
        CV_Error( cv::Error::StsBadArg, "Unknown/unsupported norm type" );

    CV_OCL_ਲੜੀਵਾਰ(_dst.isUMat(),
               ocl_normalize(_src, _dst, ਨਕਾਬ_2, rtype, scale, ਖਿਸਕਾਓ))

    Mat src = _src.getMat();
    ਜੇ( ਨਕਾਬ_2.empty() )
        src.convert_ਮੰਜ਼ਿਲ( _dst, rtype, scale, ਖਿਸਕਾਓ );
    ਨਹੀਂਤਾਂ
    {
        Mat ਆਰਜ਼ੀ_2;
        src.convert_ਮੰਜ਼ਿਲ( ਆਰਜ਼ੀ_2, rtype, scale, ਖਿਸਕਾਓ );
        ਆਰਜ਼ੀ_2.copy_ਮੰਜ਼ਿਲ( _dst, ਨਕਾਬ_2 );
    }
}

}  // namespace
