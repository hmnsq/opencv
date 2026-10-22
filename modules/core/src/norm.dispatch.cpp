// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html
//
// Copyright (C) 2026, Advanced Micro Devices, Inc., all rights reserved.


#ಅನ್ತರ್ಭಾವ "precomp.hpp"
#ಅನ್ತರ್ಭಾವ "opencl_kernels_core.hpp"
#ಅನ್ತರ್ಭಾವ "stat.hpp"

#ಅನ್ತರ್ಭಾವ "norm.simd.hpp"
#ಅನ್ತರ್ಭಾವ "norm.simd_declarations.hpp"

/****************************************************************************************\
*                                         norm                                           *
\****************************************************************************************/

ನಾಮಾಕಾಶ cv { ನಾಮಾಕಾಶ hal {

ಬಾಹ್ಯ ಸ್ಥಿರಾಂಕ uchar ತೆಗೆ_ಎಣಿಕೆ_ಕೋಷ್ಟಕ[256] =
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

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ uchar ತೆಗೆ_ಎಣಿಕೆ_Table2[] =
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

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ uchar ತೆಗೆ_ಎಣಿಕೆ_Table4[] =
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


ಪೂರ್ಣಾಂಕ normHamming(ಸ್ಥಿರಾಂಕ uchar* a, ಪೂರ್ಣಾಂಕ n, ಪೂರ್ಣಾಂಕ cell_ಗಾತ್ರ)
{
    ಪೂರ್ಣಾಂಕ output;
    CALL_HAL_RET(normHamming8u, cv_hal_normHamming8u, output, a, n, cell_ಗಾತ್ರ);

    ಆದರೆ( cell_ಗಾತ್ರ == 1 )
        ಹಿಂತಿರುಗಿ normHamming(a, n);
    ಸ್ಥಿರಾಂಕ uchar* tab = 0;
    ಆದರೆ( cell_ಗಾತ್ರ == 2 )
        tab = ತೆಗೆ_ಎಣಿಕೆ_Table2;
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ( cell_ಗಾತ್ರ == 4 )
        tab = ತೆಗೆ_ಎಣಿಕೆ_Table4;
    ಇಲ್ಲದಿದ್ದರೆ
        ಹಿಂತಿರುಗಿ -1;
    ಪೂರ್ಣಾಂಕ i = 0;
    ಪೂರ್ಣಾಂಕ ಫಲಿತಾಂಶ = 0;
#ಆದರೆ (CV_SIMD || CV_SIMD_SCALABLE)
    v_uint64 t = vx_setzero_u64();
    ಆದರೆ ( cell_ಗಾತ್ರ == 2)
    {
        v_uint16 ಮುಖವಾಡ = v_reinterpret_as_u16(vx_setall_u8(0x55));
        ಗಾಗಿ(; i <= n - VTraits<v_uint8>::vlanes(); i += VTraits<v_uint8>::vlanes())
        {
            v_uint16 a0 = v_reinterpret_as_u16(vx_ತುಂಬು(a + i));
            t = v_ಕೂಡಿಸು(t, v_popcount(v_reinterpret_as_u64(v_and(v_or(a0, v_shr<1>(a0)), ಮುಖವಾಡ))));
        }
    }
    ಇಲ್ಲದಿದ್ದರೆ    // cellSize == 4
    {
        v_uint16 ಮುಖವಾಡ = v_reinterpret_as_u16(vx_setall_u8(0x11));
        ಗಾಗಿ(; i <= n - VTraits<v_uint8>::vlanes(); i += VTraits<v_uint8>::vlanes())
        {
            v_uint16 a0 = v_reinterpret_as_u16(vx_ತುಂಬು(a + i));
            v_uint16 a1 = v_or(a0, v_shr<2>(a0));
            t = v_ಕೂಡಿಸು(t, v_popcount(v_reinterpret_as_u64(v_and(v_or(a1, v_shr<1>(a1)), ಮುಖವಾಡ))));

        }
    }
    ಫಲಿತಾಂಶ += (ಪೂರ್ಣಾಂಕ)v_reduce_ಮೊತ್ತ(t);
    vx_cleanup();
#ಅಥವಾಆದರೆ CV_ENABLE_UNROLLED
    ಗಾಗಿ( ; i <= n - 4; i += 4 )
        ಫಲಿತಾಂಶ += tab[a[i]] + tab[a[i+1]] + tab[a[i+2]] + tab[a[i+3]];
#ಆದರೆ_ಅಂತ್ಯ
    ಗಾಗಿ( ; i < n; i++ )
        ಫಲಿತಾಂಶ += tab[a[i]];
    ಹಿಂತಿರುಗಿ ಫಲಿತಾಂಶ;
}

ಪೂರ್ಣಾಂಕ normHamming(ಸ್ಥಿರಾಂಕ uchar* a, ಸ್ಥಿರಾಂಕ uchar* b, ಪೂರ್ಣಾಂಕ n, ಪೂರ್ಣಾಂಕ cell_ಗಾತ್ರ)
{
    ಪೂರ್ಣಾಂಕ output;
    CALL_HAL_RET(normHammingDiff8u, cv_hal_normHammingDiff8u, output, a, b, n, cell_ಗಾತ್ರ);

    ಆದರೆ( cell_ಗಾತ್ರ == 1 )
        ಹಿಂತಿರುಗಿ normHamming(a, b, n);
    ಸ್ಥಿರಾಂಕ uchar* tab = 0;
    ಆದರೆ( cell_ಗಾತ್ರ == 2 )
        tab = ತೆಗೆ_ಎಣಿಕೆ_Table2;
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ( cell_ಗಾತ್ರ == 4 )
        tab = ತೆಗೆ_ಎಣಿಕೆ_Table4;
    ಇಲ್ಲದಿದ್ದರೆ
        ಹಿಂತಿರುಗಿ -1;
    ಪೂರ್ಣಾಂಕ i = 0;
    ಪೂರ್ಣಾಂಕ ಫಲಿತಾಂಶ = 0;
#ಆದರೆ (CV_SIMD || CV_SIMD_SCALABLE)
    v_uint64 t = vx_setzero_u64();
    ಆದರೆ ( cell_ಗಾತ್ರ == 2)
    {
        v_uint16 ಮುಖವಾಡ = v_reinterpret_as_u16(vx_setall_u8(0x55));
        ಗಾಗಿ(; i <= n - VTraits<v_uint8>::vlanes(); i += VTraits<v_uint8>::vlanes())
        {
            v_uint16 ab0 = v_reinterpret_as_u16(v_xor(vx_ತುಂಬು(a + i), vx_ತುಂಬು(b + i)));
            t = v_ಕೂಡಿಸು(t, v_popcount(v_reinterpret_as_u64(v_and(v_or(ab0, v_shr<1>(ab0)), ಮುಖವಾಡ))));
        }
    }
    ಇಲ್ಲದಿದ್ದರೆ    // cellSize == 4
    {
        v_uint16 ಮುಖವಾಡ = v_reinterpret_as_u16(vx_setall_u8(0x11));
        ಗಾಗಿ(; i <= n - VTraits<v_uint8>::vlanes(); i += VTraits<v_uint8>::vlanes())
        {
            v_uint16 ab0 = v_reinterpret_as_u16(v_xor(vx_ತುಂಬು(a + i), vx_ತುಂಬು(b + i)));
            v_uint16 ab1 = v_or(ab0, v_shr<2>(ab0));
            t = v_ಕೂಡಿಸು(t, v_popcount(v_reinterpret_as_u64(v_and(v_or(ab1, v_shr<1>(ab1)), ಮುಖವಾಡ))));
        }
    }
    ಫಲಿತಾಂಶ += (ಪೂರ್ಣಾಂಕ)v_reduce_ಮೊತ್ತ(t);
    vx_cleanup();
#ಅಥವಾಆದರೆ CV_ENABLE_UNROLLED
    ಗಾಗಿ( ; i <= n - 4; i += 4 )
        ಫಲಿತಾಂಶ += tab[a[i] ^ b[i]] + tab[a[i+1] ^ b[i+1]] +
                tab[a[i+2] ^ b[i+2]] + tab[a[i+3] ^ b[i+3]];
#ಆದರೆ_ಅಂತ್ಯ
    ಗಾಗಿ( ; i < n; i++ )
        ಫಲಿತಾಂಶ += tab[a[i] ^ b[i]];
    ಹಿಂತಿರುಗಿ ಫಲಿತಾಂಶ;
}

ದಶಮಾಂಶ normL2Sqr_(ಸ್ಥಿರಾಂಕ ದಶಮಾಂಶ* a, ಸ್ಥಿರಾಂಕ ದಶಮಾಂಶ* b, ಪೂರ್ಣಾಂಕ n)
{
    ಪೂರ್ಣಾಂಕ j = 0; ದಶಮಾಂಶ d = 0.f;
#ಆದರೆ (CV_SIMD || CV_SIMD_SCALABLE)
    ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ vl = VTraits<v_float32>::vlanes();
    v_float32 v_d0 = vx_setzero_f32(), v_d1 = vx_setzero_f32();
    v_float32 v_d2 = vx_setzero_f32(), v_d3 = vx_setzero_f32();
    ಗಾಗಿ (; j <= n - 4 * vl; j += 4 * vl)
    {
        v_float32 t0 = v_sub(vx_ತುಂಬು(a + j), vx_ತುಂಬು(b + j));
        v_float32 t1 = v_sub(vx_ತುಂಬು(a + j + vl), vx_ತುಂಬು(b + j + vl));
        v_d0 = v_muladd(t0, t0, v_d0);
        v_float32 t2 = v_sub(vx_ತುಂಬು(a + j + 2 * vl), vx_ತುಂಬು(b + j + 2 * vl));
        v_d1 = v_muladd(t1, t1, v_d1);
        v_float32 t3 = v_sub(vx_ತುಂಬು(a + j + 3 * vl), vx_ತುಂಬು(b + j + 3 * vl));
        v_d2 = v_muladd(t2, t2, v_d2);
        v_d3 = v_muladd(t3, t3, v_d3);
    }
    v_d0 = v_ಕೂಡಿಸು(v_ಕೂಡಿಸು(v_d0, v_d1), v_ಕೂಡಿಸು(v_d2, v_d3));
    ಗಾಗಿ (; j <= n - vl; j += vl)
    {
        v_float32 t0 = v_sub(vx_ತುಂಬು(a + j), vx_ತುಂಬು(b + j));
        v_d0 = v_muladd(t0, t0, v_d0);
    }
    d = v_reduce_ಮೊತ್ತ(v_d0);
#ಆದರೆ_ಅಂತ್ಯ
    ಗಾಗಿ( ; j < n; j++ )
    {
        ದಶಮಾಂಶ t = a[j] - b[j];
        d += t*t;
    }
    ಹಿಂತಿರುಗಿ d;
}


ದಶಮಾಂಶ normL1_(ಸ್ಥಿರಾಂಕ ದಶಮಾಂಶ* a, ಸ್ಥಿರಾಂಕ ದಶಮಾಂಶ* b, ಪೂರ್ಣಾಂಕ n)
{
    ಪೂರ್ಣಾಂಕ j = 0; ದಶಮಾಂಶ d = 0.f;
#ಆದರೆ (CV_SIMD || CV_SIMD_SCALABLE)
    ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ vl = VTraits<v_float32>::vlanes();
    v_float32 v_d0 = vx_setzero_f32(), v_d1 = vx_setzero_f32();
    v_float32 v_d2 = vx_setzero_f32(), v_d3 = vx_setzero_f32();
    ಗಾಗಿ (; j <= n - 4 * vl; j += 4 * vl)
    {
        v_d0 = v_ಕೂಡಿಸು(v_d0, v_absdiff(vx_ತುಂಬು(a + j), vx_ತುಂಬು(b + j)));
        v_d1 = v_ಕೂಡಿಸು(v_d1, v_absdiff(vx_ತುಂಬು(a + j + vl), vx_ತುಂಬು(b + j + vl)));
        v_d2 = v_ಕೂಡಿಸು(v_d2, v_absdiff(vx_ತುಂಬು(a + j + 2 * vl), vx_ತುಂಬು(b + j + 2 * vl)));
        v_d3 = v_ಕೂಡಿಸು(v_d3, v_absdiff(vx_ತುಂಬು(a + j + 3 * vl), vx_ತುಂಬು(b + j + 3 * vl)));
    }
    v_d0 = v_ಕೂಡಿಸು(v_ಕೂಡಿಸು(v_d0, v_d1), v_ಕೂಡಿಸು(v_d2, v_d3));
    ಗಾಗಿ (; j <= n - vl; j += vl)
        v_d0 = v_ಕೂಡಿಸು(v_d0, v_absdiff(vx_ತುಂಬು(a + j), vx_ತುಂಬು(b + j)));
    d = v_reduce_ಮೊತ್ತ(v_d0);
#ಆದರೆ_ಅಂತ್ಯ
    ಗಾಗಿ( ; j < n; j++ )
        d += std::ನಿರಪೇಕ್ಷ(a[j] - b[j]);
    ಹಿಂತಿರುಗಿ d;
}

ಪೂರ್ಣಾಂಕ normL1_(ಸ್ಥಿರಾಂಕ uchar* a, ಸ್ಥಿರಾಂಕ uchar* b, ಪೂರ್ಣಾಂಕ n)
{
    ಪೂರ್ಣಾಂಕ j = 0, d = 0;
#ಆದರೆ (CV_SIMD || CV_SIMD_SCALABLE)
    ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ vl = VTraits<v_uint8>::vlanes();
    ಗಾಗಿ (; j <= n - 4 * vl; j += 4 * vl)
        d += v_reduce_sad(vx_ತುಂಬು(a + j), vx_ತುಂಬು(b + j)) +
             v_reduce_sad(vx_ತುಂಬು(a + j + vl), vx_ತುಂಬು(b + j + vl)) +
             v_reduce_sad(vx_ತುಂಬು(a + j + 2 * vl), vx_ತುಂಬು(b + j + 2 * vl)) +
             v_reduce_sad(vx_ತುಂಬು(a + j + 3 * vl), vx_ತುಂಬು(b + j + 3 * vl));
    ಗಾಗಿ (; j <= n - vl; j += vl)
        d += v_reduce_sad(vx_ತುಂಬು(a + j), vx_ತುಂಬು(b + j));
#ಆದರೆ_ಅಂತ್ಯ
    ಗಾಗಿ( ; j < n; j++ )
        d += std::ನಿರಪೇಕ್ಷ(a[j] - b[j]);
    ಹಿಂತಿರುಗಿ d;
}

} //cv::hal

//==================================================================================================

ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಪೂರ್ಣಾಂಕ (*NormFunc)(ಸ್ಥಿರಾಂಕ uchar*, ಸ್ಥಿರಾಂಕ uchar*, uchar*, ಪೂರ್ಣಾಂಕ, ಪೂರ್ಣಾಂಕ);
ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಪೂರ್ಣಾಂಕ (*NormDiffFunc)(ಸ್ಥಿರಾಂಕ uchar*, ಸ್ಥಿರಾಂಕ uchar*, ಸ್ಥಿರಾಂಕ uchar*, uchar*, ಪೂರ್ಣಾಂಕ, ಪೂರ್ಣಾಂಕ);

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_OPENCL

ಸ್ಥಿರ ತಾರ್ಕಿಕ ocl_norm( ಒಳಹರಿವು_Array _src, ಪೂರ್ಣಾಂಕ normType, ಒಳಹರಿವು_Array ಮುಖವಾಡ_2, ದ್ವಿಗುಣ & ಫಲಿತಾಂಶ )
{
    ಸ್ಥಿರಾಂಕ ocl::Device & d = ocl::Device::getDefault();

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ __ANDROID__
    ಆದರೆ (d.isNVidia())
        ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;
#ಆದರೆ_ಅಂತ್ಯ
    ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ cn = _src.channels();
    ಆದರೆ (cn > 4)
        ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;
    ಪೂರ್ಣಾಂಕ type = _src.type(), ಆಳ = CV_MAT_ಆಳ(type);
    ತಾರ್ಕಿಕ doubleSupport = d.doubleFPConfig() > 0,
            have_ಮುಖವಾಡ = ಮುಖವಾಡ_2.ಬಗೆ() != ಒಳಹರಿವು_Array_2::NONE;

    ಆದರೆ (ಆಳ >= CV_16F)
        ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;  // TODO: support FP16

    ಆದರೆ ( !(normType == NORM_INF || normType == NORM_L1 || normType == NORM_L2 || normType == NORM_L2SQR) ||
         (!doubleSupport && ಆಳ == CV_64F))
        ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;

    UMat src = _src.getUMat();

    ಆದರೆ (normType == NORM_INF)
    {
        ಆದರೆ (!ocl_minMaxIdx(_src, ಖಾಲಿ, &ಫಲಿತಾಂಶ, ಖಾಲಿ, ಖಾಲಿ, ಮುಖವಾಡ_2,
                           std::max(ಆಳ, CV_32S), ಆಳ != CV_8U && ಆಳ != CV_16U))
            ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;
    }
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (normType == NORM_L1 || normType == NORM_L2 || normType == NORM_L2SQR)
    {
        Scalar sc;
        ತಾರ್ಕಿಕ unstype = ಆಳ == CV_8U || ಆಳ == CV_16U;

        ಆದರೆ ( !ocl_ಮೊತ್ತ(have_ಮುಖವಾಡ ? src : src.reshape(1), sc, normType == NORM_L2 || normType == NORM_L2SQR ?
                    OCL_OP_ಮೊತ್ತ_SQR : (unstype ? OCL_OP_ಮೊತ್ತ : OCL_OP_ಮೊತ್ತ_ABS), ಮುಖವಾಡ_2) )
            ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;

        ದ್ವಿಗುಣ s = 0.0;
        ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < (have_ಮುಖವಾಡ ? cn : 1); ++i)
            s += sc[i];

        ಫಲಿತಾಂಶ = normType == NORM_L1 || normType == NORM_L2SQR ? s : std::ವರ್ಗಮೂಲ(s);
    }

    ಹಿಂತಿರುಗಿ ನಿಜ;
}

#ಆದರೆ_ಅಂತ್ಯ

ಸ್ಥಿರ NormFunc getNormFunc(ಪೂರ್ಣಾಂಕ normType, ಪೂರ್ಣಾಂಕ ಆಳ) {
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getNormFunc, (normType, ಆಳ), CV_CPU_DISPATCH_MODES_ALL);
}
ಸ್ಥಿರ NormDiffFunc getNormDiffFunc(ಪೂರ್ಣಾಂಕ normType, ಪೂರ್ಣಾಂಕ ಆಳ) {
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getNormDiffFunc, (normType, ಆಳ), CV_CPU_DISPATCH_MODES_ALL);
}

ದ್ವಿಗುಣ norm( ಒಳಹರಿವು_Array _src, ಪೂರ್ಣಾಂಕ normType, ಒಳಹರಿವು_Array ಮುಖವಾಡ_2 )
{
    CV_INSTRUMENT_REGION();

    normType &= NORM_TYPE_ಮುಖವಾಡ;
    CV_Assert( normType == NORM_INF || normType == NORM_L1 ||
               normType == NORM_L2 || normType == NORM_L2SQR ||
               ((normType == NORM_HAMMING || normType == NORM_HAMMING2) && _src.type() == CV_8U) );

#ಆದರೆ defined HAVE_OPENCL
    ದ್ವಿಗುಣ ಫಲಿತಾಂಶ_2 = 0;
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_OPENCL
    CV_OCL_ಸತತ_2(OCL_PERFORMANCE_CHECK(_src.isUMat()) && _src.dims() <= 2,
                ocl_norm(_src, normType, ಮುಖವಾಡ_2, ಫಲಿತಾಂಶ_2),
                ಫಲಿತಾಂಶ_2)
#ಆದರೆ_ಅಂತ್ಯ

    Mat src = _src.getMat(), ಮುಖವಾಡ = ಮುಖವಾಡ_2.getMat();
    CV_Assert( ಮುಖವಾಡ.empty() || ((ಮುಖವಾಡ.type() == CV_8U || ಮುಖವಾಡ.type() == CV_8S || ಮುಖವಾಡ.type() == CV_Bool) && src.size == ಮುಖವಾಡ.size) );
    ಆದರೆ( src.empty() )
        ಹಿಂತಿರುಗಿ 0.0;
    ಪೂರ್ಣಾಂಕ ಆಳ = src.ಆಳ(), cn = src.channels();
    ಆದರೆ( src.dims <= 2 )
    {
        ದ್ವಿಗುಣ ಫಲಿತಾಂಶ;
        CALL_HAL_RET(norm, cv_hal_norm, ಫಲಿತಾಂಶ, src.ದತ್ತಾಂಶ, src.ಹೆಜ್ಜೆ, ಮುಖವಾಡ.ದತ್ತಾಂಶ, ಮುಖವಾಡ.ಹೆಜ್ಜೆ, src.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ, src.ಅಡ್ಡಸಾಲುಗಳು, src.type(), normType);
    }
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ( src.isContinuous() && ಮುಖವಾಡ.isContinuous() )
    {
        ದ್ವಿಗುಣ ಫಲಿತಾಂಶ;
        CALL_HAL_RET(norm, cv_hal_norm, ಫಲಿತಾಂಶ, src.ದತ್ತಾಂಶ, 0, ಮುಖವಾಡ.ದತ್ತಾಂಶ, 0, (ಪೂರ್ಣಾಂಕ)src.ಒಟ್ಟು(), 1, src.type(), normType);
    }

    NormFunc func = getNormFunc(normType >> 1, ಆಳ);
    CV_Assert( (normType >> 1) >= 3 || func != 0 );

    ಆದರೆ( src.isContinuous() && ಮುಖವಾಡ.empty() )
    {
        size_t len = src.ಒಟ್ಟು()*cn;
        ಆದರೆ( len == (size_t)(ಪೂರ್ಣಾಂಕ)len )
        {
            ಆದರೆ( ಆಳ == CV_32F )
            {
                ಸ್ಥಿರಾಂಕ uchar* ದತ್ತಾಂಶ = src.ptr<ಸ್ಥಿರಾಂಕ uchar>();

                ಆದರೆ( normType == NORM_L2 || normType == NORM_L2SQR || normType == NORM_L1 )
                {
                    ದ್ವಿಗುಣ ಫಲಿತಾಂಶ = 0;
                    func(ದತ್ತಾಂಶ, 0, (uchar*)&ಫಲಿತಾಂಶ, (ಪೂರ್ಣಾಂಕ)len, 1);
                    ಹಿಂತಿರುಗಿ normType == NORM_L2 ? std::ವರ್ಗಮೂಲ(ಫಲಿತಾಂಶ) : ಫಲಿತಾಂಶ;
                }
                ಆದರೆ( normType == NORM_INF )
                {
                    ದಶಮಾಂಶ ಫಲಿತಾಂಶ = 0;
                    func(ದತ್ತಾಂಶ, 0, (uchar*)&ಫಲಿತಾಂಶ, (ಪೂರ್ಣಾಂಕ)len, 1);
                    ಹಿಂತಿರುಗಿ ಫಲಿತಾಂಶ;
                }
            }
            ಆದರೆ( ಆಳ == CV_8U )
            {
                ಸ್ಥಿರಾಂಕ uchar* ದತ್ತಾಂಶ = src.ptr<uchar>();

                ಆದರೆ( normType == NORM_HAMMING )
                {
                    ಹಿಂತಿರುಗಿ hal::normHamming(ದತ್ತಾಂಶ, (ಪೂರ್ಣಾಂಕ)len, 1);
                }

                ಆದರೆ( normType == NORM_HAMMING2 )
                {
                    ಹಿಂತಿರುಗಿ hal::normHamming(ದತ್ತಾಂಶ, (ಪೂರ್ಣಾಂಕ)len, 2);
                }
            }
        }
    }

    CV_Assert( ಮುಖವಾಡ.empty() || ಮುಖವಾಡ.type() == CV_8U || ಮುಖವಾಡ.type() == CV_8S || ಮುಖವಾಡ.type() == CV_Bool );

    ಆದರೆ( normType == NORM_HAMMING || normType == NORM_HAMMING2 )
    {
        ಆದರೆ( !ಮುಖವಾಡ.empty() )
        {
            Mat ತಾತ್ಕಾಲಿಕ_2;
            bitwise_and(src, ಮುಖವಾಡ, ತಾತ್ಕಾಲಿಕ_2);
            ಹಿಂತಿರುಗಿ norm(ತಾತ್ಕಾಲಿಕ_2, normType);
        }
        ಪೂರ್ಣಾಂಕ cell_ಗಾತ್ರ = normType == NORM_HAMMING ? 1 : 2;

        ಸ್ಥಿರಾಂಕ Mat* arrays[] = {&src, 0};
        uchar* ptrs[1] = {};
        NAryMatIterator it(arrays, ptrs);
        ಪೂರ್ಣಾಂಕ ಒಟ್ಟು = (ಪೂರ್ಣಾಂಕ)it.size;
        ಪೂರ್ಣಾಂಕ ಫಲಿತಾಂಶ = 0;

        ಗಾಗಿ( size_t i = 0; i < it.nplanes; i++, ++it )
        {
            ಫಲಿತಾಂಶ += hal::normHamming(ptrs[0], ಒಟ್ಟು, cell_ಗಾತ್ರ);
        }

        ಹಿಂತಿರುಗಿ ಫಲಿತಾಂಶ;
    }

    ಸ್ಥಿರಾಂಕ Mat* arrays[] = {&src, &ಮುಖವಾಡ, 0};
    uchar* ptrs[2] = {};
    ಒಕ್ಕೂಟ
    {
        ದ್ವಿಗುಣ d;
        ಚಿಹ್ನೆರಹಿತ u;
        uint64 UL;
        ದಶಮಾಂಶ f;
    }
    ಫಲಿತಾಂಶ;
    ಫಲಿತಾಂಶ.d = 0;
    NAryMatIterator it(arrays, ptrs);
    CV_CheckLT((size_t)it.size, (size_t)ಗರಿಷ್ಠ_ಪೂರ್ಣಾಂಕ, "");
    ತಾರ್ಕಿಕ is_fp16 = ಆಳ == CV_16F || ಆಳ == CV_16BF;

    ಆದರೆ ((normType == NORM_L1 && (ಆಳ <= CV_16S || ಆಳ == CV_Bool || is_fp16)) ||
        ((normType == NORM_L2 || normType == NORM_L2SQR) && (ಆಳ <= CV_8S || ಆಳ == CV_Bool || is_fp16)))
    {
        // special case to handle "integer" overflow in accumulator
        ಸ್ಥಿರಾಂಕ size_t esz = src.elem_ಗಾತ್ರ();
        ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ ಒಟ್ಟು = (ಪೂರ್ಣಾಂಕ)it.size;
        ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ ಖಂಡ_Size0 = (is_fp16 ? (1 << 10) :
            ಆಳ == CV_Bool ? (1 << 30) :
            normType == NORM_L1 && ಆಳ <= CV_8S ? (1 << 23) : (1 << 15))/cn;
        ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ ಖಂಡ_ಗಾತ್ರ_2 = std::min(ಒಟ್ಟು, ಖಂಡ_Size0);
        ಒಕ್ಕೂಟ {
            ಪೂರ್ಣಾಂಕ i;
            ದಶಮಾಂಶ f;
        } blocksum;
        blocksum.i = 0;
        ಪೂರ್ಣಾಂಕ ಎಣಿಕೆ_3 = 0;

        ಗಾಗಿ (size_t i = 0; i < it.nplanes; i++, ++it)
        {
            ಗಾಗಿ (ಪೂರ್ಣಾಂಕ j = 0; j < ಒಟ್ಟು; j += ಖಂಡ_ಗಾತ್ರ_2)
            {
                ಪೂರ್ಣಾಂಕ bsz = std::min(ಒಟ್ಟು - j, ಖಂಡ_ಗಾತ್ರ_2);
                func(ptrs[0], ptrs[1], (uchar*)&blocksum, bsz, cn);
                ಎಣಿಕೆ_3 += bsz;
                ಆದರೆ (ಎಣಿಕೆ_3 + ಖಂಡ_ಗಾತ್ರ_2 >= ಖಂಡ_Size0 || (i+1 >= it.nplanes && j+bsz >= ಒಟ್ಟು))
                {
                    ಫಲಿತಾಂಶ.d += is_fp16 ? (ದ್ವಿಗುಣ)blocksum.f : (ದ್ವಿಗುಣ)blocksum.i;
                    blocksum.i = 0;
                    ಎಣಿಕೆ_3 = 0;
                }
                ptrs[0] += bsz*esz;
                ಆದರೆ (ptrs[1])
                    ptrs[1] += bsz;
            }
        }
    }
    ಇಲ್ಲದಿದ್ದರೆ
    {
        // generic implementation
        ಗಾಗಿ (size_t i = 0; i < it.nplanes; i++, ++it)
        {
            func(ptrs[0], ptrs[1], (uchar*)&ಫಲಿತಾಂಶ, (ಪೂರ್ಣಾಂಕ)it.size, cn);
        }
    }

    ಆದರೆ( normType == NORM_INF )
    {
        ಆದರೆ(ಆಳ <= CV_32S || ಆಳ == CV_32U || ಆಳ == CV_Bool)
            ಹಿಂತಿರುಗಿ ಫಲಿತಾಂಶ.u;
        ಆದರೆ (ಆಳ == CV_32F || is_fp16)
            ಹಿಂತಿರುಗಿ ಫಲಿತಾಂಶ.f;
        ಆದರೆ (ಆಳ == CV_64U || ಆಳ == CV_64S)
            ಹಿಂತಿರುಗಿ (ದ್ವಿಗುಣ)ಫಲಿತಾಂಶ.UL;
    }
    ಆದರೆ( normType == NORM_L2 )
        ಹಿಂತಿರುಗಿ std::ವರ್ಗಮೂಲ(ಫಲಿತಾಂಶ.d);

    ಹಿಂತಿರುಗಿ ಫಲಿತಾಂಶ.d;
}

//==================================================================================================

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_OPENCL
ಸ್ಥಿರ ತಾರ್ಕಿಕ ocl_norm( ಒಳಹರಿವು_Array _src1, ಒಳಹರಿವು_Array _src2, ಪೂರ್ಣಾಂಕ normType, ಒಳಹರಿವು_Array ಮುಖವಾಡ_2, ದ್ವಿಗುಣ & ಫಲಿತಾಂಶ )
{
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ __ANDROID__
    ಆದರೆ (ocl::Device::getDefault().isNVidia())
        ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;
#ಆದರೆ_ಅಂತ್ಯ

    Scalar sc1, sc2;
    ಪೂರ್ಣಾಂಕ cn = _src1.channels();
    ಆದರೆ (cn > 4)
        ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;
    ಪೂರ್ಣಾಂಕ type = _src1.type(), ಆಳ = CV_MAT_ಆಳ(type);
    ತಾರ್ಕಿಕ relative = (normType & NORM_RELATIVE) != 0;
    normType &= ~NORM_RELATIVE;
    ತಾರ್ಕಿಕ normsum = normType == NORM_L1 || normType == NORM_L2 || normType == NORM_L2SQR;

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ __APPLE__
    ಆದರೆ(normType == NORM_L1 && type == CV_16UC3 && !ಮುಖವಾಡ_2.empty())
        ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;
#ಆದರೆ_ಅಂತ್ಯ

    ಆದರೆ (normsum)
    {
        ಆದರೆ (!ocl_ಮೊತ್ತ(_src1, sc1, normType == NORM_L2 || normType == NORM_L2SQR ?
                     OCL_OP_ಮೊತ್ತ_SQR : OCL_OP_ಮೊತ್ತ, ಮುಖವಾಡ_2, _src2, relative, sc2))
            ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;
    }
    ಇಲ್ಲದಿದ್ದರೆ
    {
        ಆದರೆ (!ocl_minMaxIdx(_src1, ಖಾಲಿ, &sc1[0], ಖಾಲಿ, ಖಾಲಿ, ಮುಖವಾಡ_2, std::max(CV_32S, ಆಳ),
                           ಸುಳ್ಳು, _src2, relative ? &sc2[0] : ಖಾಲಿ))
            ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;
        cn = 1;
    }

    ದ್ವಿಗುಣ s2 = 0;
    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < cn; ++i)
    {
        ಫಲಿತಾಂಶ += sc1[i];
        ಆದರೆ (relative)
            s2 += sc2[i];
    }

    ಆದರೆ (normType == NORM_L2)
    {
        ಫಲಿತಾಂಶ = std::ವರ್ಗಮೂಲ(ಫಲಿತಾಂಶ);
        ಆದರೆ (relative)
            s2 = std::ವರ್ಗಮೂಲ(s2);
    }

    ಆದರೆ (relative)
        ಫಲಿತಾಂಶ /= (s2 + DBL_EPSILON);

    ಹಿಂತಿರುಗಿ ನಿಜ;
}  // ocl_norm()
#ಆದರೆ_ಅಂತ್ಯ  // HAVE_OPENCL

ದ್ವಿಗುಣ norm( ಒಳಹರಿವು_Array _src1, ಒಳಹರಿವು_Array _src2, ಪೂರ್ಣಾಂಕ normType, ಒಳಹರಿವು_Array ಮುಖವಾಡ_2 )
{
    CV_INSTRUMENT_REGION();

    CV_CheckTypeEQ(_src1.type(), _src2.type(), "Input type mismatch");
    CV_Assert(_src1.same_ಗಾತ್ರ(_src2));

#ಆದರೆ defined HAVE_OPENCL
    ದ್ವಿಗುಣ ಫಲಿತಾಂಶ_2 = 0;
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_OPENCL
    CV_OCL_ಸತತ_2(OCL_PERFORMANCE_CHECK(_src1.isUMat()),
                ocl_norm(_src1, _src2, normType, ಮುಖವಾಡ_2, ಫಲಿತಾಂಶ_2),
                ಫಲಿತಾಂಶ_2)
#ಆದರೆ_ಅಂತ್ಯ

    Mat src1 = _src1.getMat(), src2 = _src2.getMat(), ಮುಖವಾಡ = ಮುಖವಾಡ_2.getMat();
    CV_Assert( ಮುಖವಾಡ.empty() || ((ಮುಖವಾಡ.type() == CV_8U || ಮುಖವಾಡ.type() == CV_8S || ಮುಖವಾಡ.type() == CV_Bool) && src1.size == ಮುಖವಾಡ.size) );
    ಆದರೆ( src1.empty() || src2.empty() )
        ಹಿಂತಿರುಗಿ 0.0;
    ಪೂರ್ಣಾಂಕ ಆಳ = src1.ಆಳ(), cn = src1.channels();
    ಆದರೆ( src1.dims <= 2 )
    {
        ದ್ವಿಗುಣ ಫಲಿತಾಂಶ;
        CALL_HAL_RET(normDiff, cv_hal_normDiff, ಫಲಿತಾಂಶ, src1.ದತ್ತಾಂಶ, src1.ಹೆಜ್ಜೆ, src2.ದತ್ತಾಂಶ, src2.ಹೆಜ್ಜೆ, ಮುಖವಾಡ.ದತ್ತಾಂಶ, ಮುಖವಾಡ.ಹೆಜ್ಜೆ, src1.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ, src1.ಅಡ್ಡಸಾಲುಗಳು, src1.type(), normType);
    }
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ( src1.isContinuous() && src2.isContinuous() && ಮುಖವಾಡ.isContinuous() )
    {
        ದ್ವಿಗುಣ ಫಲಿತಾಂಶ;
        CALL_HAL_RET(normDiff, cv_hal_normDiff, ಫಲಿತಾಂಶ, src1.ದತ್ತಾಂಶ, 0, src2.ದತ್ತಾಂಶ, 0, ಮುಖವಾಡ.ದತ್ತಾಂಶ, 0, (ಪೂರ್ಣಾಂಕ)src1.ಒಟ್ಟು(), 1, src1.type(), normType);
    }

    ಆದರೆ( normType & NORM_RELATIVE )
    {
        ಹಿಂತಿರುಗಿ norm(_src1, _src2, normType & ~NORM_RELATIVE, ಮುಖವಾಡ_2)/(norm(_src2, normType, ಮುಖವಾಡ_2) + DBL_EPSILON);
    }

    normType &= 7;
    CV_Assert( normType == NORM_INF || normType == NORM_L1 ||
               normType == NORM_L2 || normType == NORM_L2SQR ||
              ((normType == NORM_HAMMING || normType == NORM_HAMMING2) && src1.type() == CV_8U) );

    NormDiffFunc func = getNormDiffFunc(normType >> 1, ಆಳ);
    CV_Assert( (normType >> 1) >= 3 || func != 0 );

    ಆದರೆ( src1.isContinuous() && src2.isContinuous() && ಮುಖವಾಡ.empty() )
    {
        size_t len = src1.ಒಟ್ಟು()*src1.channels();
        ಆದರೆ( len == (size_t)(ಪೂರ್ಣಾಂಕ)len )
        {
            ಆದರೆ( src1.ಆಳ() == CV_32F )
            {
                ಸ್ಥಿರಾಂಕ uchar* data1 = src1.ptr<ಸ್ಥಿರಾಂಕ uchar>();
                ಸ್ಥಿರಾಂಕ uchar* data2 = src2.ptr<ಸ್ಥಿರಾಂಕ uchar>();

                ಆದರೆ( normType == NORM_L2 || normType == NORM_L2SQR || normType == NORM_L1 )
                {
                    ದ್ವಿಗುಣ ಫಲಿತಾಂಶ = 0;
                    func(data1, data2, 0, (uchar*)&ಫಲಿತಾಂಶ, (ಪೂರ್ಣಾಂಕ)len, 1);
                    ಹಿಂತಿರುಗಿ normType == NORM_L2 ? std::ವರ್ಗಮೂಲ(ಫಲಿತಾಂಶ) : ಫಲಿತಾಂಶ;
                }
                ಆದರೆ( normType == NORM_INF )
                {
                    ದಶಮಾಂಶ ಫಲಿತಾಂಶ = 0;
                    func(data1, data2, 0, (uchar*)&ಫಲಿತಾಂಶ, (ಪೂರ್ಣಾಂಕ)len, 1);
                    ಹಿಂತಿರುಗಿ ಫಲಿತಾಂಶ;
                }
            }
        }
    }

    CV_Assert( ಮುಖವಾಡ.empty() || ಮುಖವಾಡ.type() == CV_8U || ಮುಖವಾಡ.type() == CV_8S || ಮುಖವಾಡ.type() == CV_Bool );

    ಆದರೆ( normType == NORM_HAMMING || normType == NORM_HAMMING2 )
    {
        ಆದರೆ( !ಮುಖವಾಡ.empty() )
        {
            Mat ತಾತ್ಕಾಲಿಕ_2;
            bitwise_xor(src1, src2, ತಾತ್ಕಾಲಿಕ_2);
            bitwise_and(ತಾತ್ಕಾಲಿಕ_2, ಮುಖವಾಡ, ತಾತ್ಕಾಲಿಕ_2);
            ಹಿಂತಿರುಗಿ norm(ತಾತ್ಕಾಲಿಕ_2, normType);
        }
        ಪೂರ್ಣಾಂಕ cell_ಗಾತ್ರ = normType == NORM_HAMMING ? 1 : 2;

        ಸ್ಥಿರಾಂಕ Mat* arrays[] = {&src1, &src2, 0};
        uchar* ptrs[2] = {};
        NAryMatIterator it(arrays, ptrs);
        ಪೂರ್ಣಾಂಕ ಒಟ್ಟು = (ಪೂರ್ಣಾಂಕ)it.size;
        ಪೂರ್ಣಾಂಕ ಫಲಿತಾಂಶ = 0;

        ಗಾಗಿ( size_t i = 0; i < it.nplanes; i++, ++it )
        {
            ಫಲಿತಾಂಶ += hal::normHamming(ptrs[0], ptrs[1], ಒಟ್ಟು, cell_ಗಾತ್ರ);
        }

        ಹಿಂತಿರುಗಿ ಫಲಿತಾಂಶ;
    }

    ಸ್ಥಿರಾಂಕ Mat* arrays[] = {&src1, &src2, &ಮುಖವಾಡ, 0};
    uchar* ptrs[3] = {};
    ಒಕ್ಕೂಟ
    {
        ದ್ವಿಗುಣ d;
        ದಶಮಾಂಶ f;
        ಚಿಹ್ನೆರಹಿತ u;
        uint64 UL;
    }
    ಫಲಿತಾಂಶ;
    ಫಲಿತಾಂಶ.d = 0;
    NAryMatIterator it(arrays, ptrs);
    CV_CheckLT((size_t)it.size, (size_t)ಗರಿಷ್ಠ_ಪೂರ್ಣಾಂಕ, "");

    ತಾರ್ಕಿಕ is_fp16 = ಆಳ == CV_16F || ಆಳ == CV_16BF;

    ಆದರೆ ((normType == NORM_L1 && (ಆಳ <= CV_16S || ಆಳ == CV_Bool || is_fp16)) ||
        ((normType == NORM_L2 || normType == NORM_L2SQR) && (ಆಳ <= CV_8S || ಆಳ == CV_Bool || is_fp16)))
    {
        // special case to handle "integer" overflow in accumulator
        ಸ್ಥಿರಾಂಕ size_t esz = src1.elem_ಗಾತ್ರ();
        ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ ಒಟ್ಟು = (ಪೂರ್ಣಾಂಕ)it.size;
        ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ ಖಂಡ_Size0 = (is_fp16 ? (1 << 10) :
            ಆಳ == CV_Bool ? (1 << 30) :
            normType == NORM_L1 && ಆಳ <= CV_8S ? (1 << 23) : (1 << 15))/cn;
        ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ ಖಂಡ_ಗಾತ್ರ_2 = std::min(ಒಟ್ಟು, ಖಂಡ_Size0);
        ಒಕ್ಕೂಟ {
            ಪೂರ್ಣಾಂಕ i;
            ದಶಮಾಂಶ f;
        } blocksum;
        blocksum.i = 0;
        ಪೂರ್ಣಾಂಕ ಎಣಿಕೆ_3 = 0;

        ಗಾಗಿ (size_t i = 0; i < it.nplanes; i++, ++it)
        {
            ಗಾಗಿ (ಪೂರ್ಣಾಂಕ j = 0; j < ಒಟ್ಟು; j += ಖಂಡ_ಗಾತ್ರ_2)
            {
                ಪೂರ್ಣಾಂಕ bsz = std::min(ಒಟ್ಟು - j, ಖಂಡ_ಗಾತ್ರ_2);
                func(ptrs[0], ptrs[1], ptrs[2], (uchar*)&blocksum, bsz, cn);
                ಎಣಿಕೆ_3 += bsz;
                ಆದರೆ (ಎಣಿಕೆ_3 + ಖಂಡ_ಗಾತ್ರ_2 >= ಖಂಡ_Size0 || (i+1 >= it.nplanes && j+bsz >= ಒಟ್ಟು))
                {
                    ಫಲಿತಾಂಶ.d += is_fp16 ? (ದ್ವಿಗುಣ)blocksum.f : (ದ್ವಿಗುಣ)blocksum.i;
                    blocksum.i = 0;
                    ಎಣಿಕೆ_3 = 0;
                }
                ptrs[0] += bsz*esz;
                ptrs[1] += bsz*esz;
                ಆದರೆ (ptrs[2])
                    ptrs[2] += bsz;
            }
        }
    }
    ಇಲ್ಲದಿದ್ದರೆ
    {
        // generic implementation
        ಗಾಗಿ (size_t i = 0; i < it.nplanes; i++, ++it)
        {
            func(ptrs[0], ptrs[1], ptrs[2], (uchar*)&ಫಲಿತಾಂಶ, (ಪೂರ್ಣಾಂಕ)it.size, cn);
        }
    }

    ಆದರೆ( normType == NORM_INF )
    {
        ಆದರೆ (ಆಳ <= CV_32S || ಆಳ == CV_32U || ಆಳ == CV_Bool)
            ಹಿಂತಿರುಗಿ ಫಲಿತಾಂಶ.u;
        ಆದರೆ (ಆಳ == CV_32F || is_fp16)
            ಹಿಂತಿರುಗಿ ಫಲಿತಾಂಶ.f;
        ಆದರೆ (ಆಳ == CV_64U || ಆಳ == CV_64S)
            ಹಿಂತಿರುಗಿ (ದ್ವಿಗುಣ)ಫಲಿತಾಂಶ.UL;
    }
    ಆದರೆ( normType == NORM_L2 )
        ಹಿಂತಿರುಗಿ std::ವರ್ಗಮೂಲ(ಫಲಿತಾಂಶ.d);

    ಹಿಂತಿರುಗಿ ಫಲಿತಾಂಶ.d;
}

cv::Hamming::ಫಲಿತಾಂಶ_Type Hamming::ಕ್ರಿಯಾಕಾರಕ()( ಸ್ಥಿರಾಂಕ ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ* a, ಸ್ಥಿರಾಂಕ ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ* b, ಪೂರ್ಣಾಂಕ size ) ಸ್ಥಿರಾಂಕ
{
    ಹಿಂತಿರುಗಿ cv::hal::normHamming(a, b, size);
}

ದ್ವಿಗುಣ PSNR(ಒಳಹರಿವು_Array _src1, ಒಳಹರಿವು_Array _src2, ದ್ವಿಗುಣ R)
{
    CV_INSTRUMENT_REGION();

    //Input arrays must have depth CV_8U
    CV_Assert( _src1.type() == _src2.type() );

    ದ್ವಿಗುಣ diff = std::ವರ್ಗಮೂಲ(norm(_src1, _src2, NORM_L2SQR)/(_src1.ಒಟ್ಟು()*_src1.channels()));
    ಹಿಂತಿರುಗಿ 20*log10(R/(diff+DBL_EPSILON));
}


#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_OPENCL
ಸ್ಥಿರ ತಾರ್ಕಿಕ ocl_normalize( ಒಳಹರಿವು_Array _src, ಒಳಹರಿವು_Output_Array _dst, ಒಳಹರಿವು_Array ಮುಖವಾಡ_2, ಪೂರ್ಣಾಂಕ dtype,
                           ದ್ವಿಗುಣ scale, ದ್ವಿಗುಣ delta )
{
    UMat src = _src.getUMat();

    ಆದರೆ( ಮುಖವಾಡ_2.empty() )
        src.convert_ಗಮ್ಯ( _dst, dtype, scale, delta );
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (src.channels() <= 4)
    {
        ಸ್ಥಿರಾಂಕ ocl::Device & dev = ocl::Device::getDefault();

        ಪೂರ್ಣಾಂಕ stype = _src.type(), sdepth = CV_MAT_ಆಳ(stype), cn = CV_MAT_CN(stype),
                ddepth = CV_MAT_ಆಳ(dtype), wdepth = std::max(CV_32F, std::max(sdepth, ddepth)),
                ಅಡ್ಡಸಾಲುಗಳು_ಪ್ರತಿ_WI = dev.isIntel() ? 4 : 1;

        ದಶಮಾಂಶ fscale = static_cast<ದಶಮಾಂಶ>(scale), fdelta = static_cast<ದಶಮಾಂಶ>(delta);
        ತಾರ್ಕಿಕ haveScale = std::fabs(scale - 1) > DBL_EPSILON,
                haveZeroScale = !(std::fabs(scale) > DBL_EPSILON),
                haveDelta = std::fabs(delta) > DBL_EPSILON,
                doubleSupport = dev.doubleFPConfig() > 0;

        ಆದರೆ (!haveScale && !haveDelta && stype == dtype)
        {
            _src.copy_ಗಮ್ಯ(_dst, ಮುಖವಾಡ_2);
            ಹಿಂತಿರುಗಿ ನಿಜ;
        }
        ಆದರೆ (haveZeroScale)
        {
            _dst.ಗಣ_ಗಮ್ಯ(Scalar(delta), ಮುಖವಾಡ_2);
            ಹಿಂತಿರುಗಿ ನಿಜ;
        }

        ಆದರೆ ((sdepth == CV_64F || ddepth == CV_64F) && !doubleSupport)
            ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;

        ಅಕ್ಷರ cvt[2][50];
        String opts = format("-D srcT=%s -D dstT=%s -D convertToWT=%s -D cn=%d -D rowsPerWI=%d"
                             " -D convertToDT=%s -D workT=%s%s%s%s -D srcT1=%s -D dstT1=%s",
                             ocl::type_ಗಮ್ಯ_Str(stype), ocl::type_ಗಮ್ಯ_Str(dtype),
                             ocl::convertTypeStr(sdepth, wdepth, cn, cvt[0], ಗಾತ್ರ(cvt[0])), cn,
                             ಅಡ್ಡಸಾಲುಗಳು_ಪ್ರತಿ_WI, ocl::convertTypeStr(wdepth, ddepth, cn, cvt[1], ಗಾತ್ರ(cvt[1])),
                             ocl::type_ಗಮ್ಯ_Str(CV_MAKE_TYPE(wdepth, cn)),
                             doubleSupport ? " -D DOUBLE_SUPPORT" : "",
                             haveScale ? " -D HAVE_SCALE" : "",
                             haveDelta ? " -D HAVE_DELTA" : "",
                             ocl::type_ಗಮ್ಯ_Str(sdepth), ocl::type_ಗಮ್ಯ_Str(ddepth));

        ocl::Kernel k("normalizek", ocl::core::normalize_oclsrc, opts);
        ಆದರೆ (k.empty())
            ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;

        UMat ಮುಖವಾಡ = ಮುಖವಾಡ_2.getUMat(), dst = _dst.getUMat();

        ocl::KernelArg srcarg = ocl::KernelArg::Read_Only_No_ಗಾತ್ರ(src),
                maskarg = ocl::KernelArg::Read_Only_No_ಗಾತ್ರ(ಮುಖವಾಡ),
                dstarg = ocl::KernelArg::ReadWrite(dst);

        ಆದರೆ (haveScale)
        {
            ಆದರೆ (haveDelta)
                k.ವಾದಗಳು(srcarg, maskarg, dstarg, fscale, fdelta);
            ಇಲ್ಲದಿದ್ದರೆ
                k.ವಾದಗಳು(srcarg, maskarg, dstarg, fscale);
        }
        ಇಲ್ಲದಿದ್ದರೆ
        {
            ಆದರೆ (haveDelta)
                k.ವಾದಗಳು(srcarg, maskarg, dstarg, fdelta);
            ಇಲ್ಲದಿದ್ದರೆ
                k.ವಾದಗಳು(srcarg, maskarg, dstarg);
        }

        size_t globalsize[2] = { (size_t)src.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ, ((size_t)src.ಅಡ್ಡಸಾಲುಗಳು + ಅಡ್ಡಸಾಲುಗಳು_ಪ್ರತಿ_WI - 1) / ಅಡ್ಡಸಾಲುಗಳು_ಪ್ರತಿ_WI };
        ಹಿಂತಿರುಗಿ k.ಸತತ(2, globalsize, ಖಾಲಿ, ಸುಳ್ಳು);
    }
    ಇಲ್ಲದಿದ್ದರೆ
    {
        UMat ತಾತ್ಕಾಲಿಕ_2;
        src.convert_ಗಮ್ಯ( ತಾತ್ಕಾಲಿಕ_2, dtype, scale, delta );
        ತಾತ್ಕಾಲಿಕ_2.copy_ಗಮ್ಯ( _dst, ಮುಖವಾಡ_2 );
    }

    ಹಿಂತಿರುಗಿ ನಿಜ;
}  // ocl_normalize
#ಆದರೆ_ಅಂತ್ಯ  // HAVE_OPENCL

ಶೂನ್ಯ normalize(ಒಳಹರಿವು_Array _src, ಒಳಹರಿವು_Output_Array _dst, ದ್ವಿಗುಣ a, ದ್ವಿಗುಣ b,
               ಪೂರ್ಣಾಂಕ norm_type, ಪೂರ್ಣಾಂಕ rtype, ಒಳಹರಿವು_Array ಮುಖವಾಡ_2)
{
    CV_INSTRUMENT_REGION();

    ದ್ವಿಗುಣ scale = 1, ಸರಿಸು = 0;
    ಪೂರ್ಣಾಂಕ type = _src.type(), ಆಳ = CV_MAT_ಆಳ(type);

    ಆದರೆ( rtype < 0 )
        rtype = _dst.fixedType() ? _dst.ಆಳ() : ಆಳ;

    ಆದರೆ( norm_type == NORM_MINMAX )
    {
        ದ್ವಿಗುಣ smin = 0, smax = 0;
        ದ್ವಿಗುಣ dmin = MIN( a, b ), dmax = MAX( a, b );
        minMaxIdx( _src, &smin, &smax, 0, 0, ಮುಖವಾಡ_2 );
        scale = (dmax - dmin)*(smax - smin > DBL_EPSILON ? 1./(smax - smin) : 0);
        ಆದರೆ( rtype == CV_32F )
        {
            scale = (ದಶಮಾಂಶ)scale;
            ಸರಿಸು = (ದಶಮಾಂಶ)dmin - (ದಶಮಾಂಶ)(smin*scale);
        }
        ಇಲ್ಲದಿದ್ದರೆ
            ಸರಿಸು = dmin - smin*scale;
    }
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ( norm_type == NORM_L2 || norm_type == NORM_L1 || norm_type == NORM_INF )
    {
        scale = norm( _src, norm_type, ಮುಖವಾಡ_2 );
        scale = scale > DBL_EPSILON ? a/scale : 0.;
        ಸರಿಸು = 0;
    }
    ಇಲ್ಲದಿದ್ದರೆ
        CV_Error( cv::Error::StsBadArg, "Unknown/unsupported norm type" );

    CV_OCL_ಸತತ(_dst.isUMat(),
               ocl_normalize(_src, _dst, ಮುಖವಾಡ_2, rtype, scale, ಸರಿಸು))

    Mat src = _src.getMat();
    ಆದರೆ( ಮುಖವಾಡ_2.empty() )
        src.convert_ಗಮ್ಯ( _dst, rtype, scale, ಸರಿಸು );
    ಇಲ್ಲದಿದ್ದರೆ
    {
        Mat ತಾತ್ಕಾಲಿಕ_2;
        src.convert_ಗಮ್ಯ( ತಾತ್ಕಾಲಿಕ_2, rtype, scale, ಸರಿಸು );
        ತಾತ್ಕಾಲಿಕ_2.copy_ಗಮ್ಯ( _dst, ಮುಖವಾಡ_2 );
    }
}

}  // namespace
