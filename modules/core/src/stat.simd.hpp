// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.
//
// Copyright (C) 2026, Advanced Micro Devices, Inc., all rights reserved.

#ಅನ್ತರ್ಭಾವ "opencv2/core/hal/intrin.hpp"

ನಾಮಾಕಾಶ cv { ನಾಮಾಕಾಶ hal {

ಬಾಹ್ಯ ಸ್ಥಿರಾಂಕ uchar ತೆಗೆ_ಎಣಿಕೆ_ಕೋಷ್ಟಕ[256];

ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಪೂರ್ಣಾಂಕ (*NormHammingFunc)(ಸ್ಥಿರಾಂಕ uchar*, ಪೂರ್ಣಾಂಕ);
ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಪೂರ್ಣಾಂಕ (*NormHammingDiffFunc)(ಸ್ಥಿರಾಂಕ uchar*, ಸ್ಥಿರಾಂಕ uchar*, ಪೂರ್ಣಾಂಕ);

CV_CPU_OPTIMIZATION_NAMESPACE_BEGIN

// forward declarations
ಪೂರ್ಣಾಂಕ normHamming(ಸ್ಥಿರಾಂಕ uchar* a, ಪೂರ್ಣಾಂಕ n);
ಪೂರ್ಣಾಂಕ normHamming(ಸ್ಥಿರಾಂಕ uchar* a, ಸ್ಥಿರಾಂಕ uchar* b, ಪೂರ್ಣಾಂಕ n);

NormHammingFunc getNormHammingFunc();
NormHammingDiffFunc getNormHammingDiffFunc();

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

#ಆದರೆ CV_AVX2
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ mm256_ಹೊರತೆಗೆ_epi32(__m256i reg, ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ i)
{
    CV_DECL_ALIGNED(32) ಪೂರ್ಣಾಂಕ reg_ದತ್ತಾಂಶ[8];
    CV_DbgAssert(0 <= i && i < 8);
    _mm256_store_si256((__m256i*)reg_ದತ್ತಾಂಶ, reg);
    ಹಿಂತಿರುಗಿ reg_ದತ್ತಾಂಶ[i];
}
#ಆದರೆ_ಅಂತ್ಯ

ಪೂರ್ಣಾಂಕ normHamming(ಸ್ಥಿರಾಂಕ uchar* a, ಪೂರ್ಣಾಂಕ n)
{
    CV_AVX_GUARD;

    ಪೂರ್ಣಾಂಕ i = 0;
    ಪೂರ್ಣಾಂಕ ಫಲಿತಾಂಶ = 0;

#ಆದರೆ (CV_SIMD || CV_SIMD_SCALABLE)
    {
        v_uint64 t = vx_setzero_u64();
        ಗಾಗಿ (; i <= n - VTraits<v_uint8>::vlanes(); i += VTraits<v_uint8>::vlanes())
            t = v_ಕೂಡಿಸು(t, v_popcount(v_reinterpret_as_u64(vx_ತುಂಬು(a + i))));
        ಫಲಿತಾಂಶ = (ಪೂರ್ಣಾಂಕ)v_reduce_ಮೊತ್ತ(t);
        vx_cleanup();
    }
#ಆದರೆ_ಅಂತ್ಯ

#ಆದರೆ CV_POPCNT
    {
#  ಆದರೆ defined CV_POPCNT_U64
        ಗಾಗಿ(; i <= n - 8; i += 8)
        {
            uint64_t val;
            std::ಸ್ಮರಣೆ_ನಕಲು(&val, a + i, ಗಾತ್ರ(val));
            ಫಲಿತಾಂಶ += (ಪೂರ್ಣಾಂಕ)CV_POPCNT_U64(val);
        }
#  endif
        ಗಾಗಿ(; i <= n - 4; i += 4)
        {
            uint32_t val;
            std::ಸ್ಮರಣೆ_ನಕಲು(&val, a + i, ಗಾತ್ರ(val));
            ಫಲಿತಾಂಶ += CV_POPCNT_U32(val);
        }
    }
#ಆದರೆ_ಅಂತ್ಯ
#ಆದರೆ CV_ENABLE_UNROLLED
    ಗಾಗಿ(; i <= n - 4; i += 4)
    {
        ಫಲಿತಾಂಶ += ತೆಗೆ_ಎಣಿಕೆ_ಕೋಷ್ಟಕ[a[i]] + ತೆಗೆ_ಎಣಿಕೆ_ಕೋಷ್ಟಕ[a[i+1]] +
        ತೆಗೆ_ಎಣಿಕೆ_ಕೋಷ್ಟಕ[a[i+2]] + ತೆಗೆ_ಎಣಿಕೆ_ಕೋಷ್ಟಕ[a[i+3]];
    }
#ಆದರೆ_ಅಂತ್ಯ
    ಗಾಗಿ(; i < n; i++)
    {
        ಫಲಿತಾಂಶ += ತೆಗೆ_ಎಣಿಕೆ_ಕೋಷ್ಟಕ[a[i]];
    }
    ಹಿಂತಿರುಗಿ ಫಲಿತಾಂಶ;
}

ಪೂರ್ಣಾಂಕ normHamming(ಸ್ಥಿರಾಂಕ uchar* a, ಸ್ಥಿರಾಂಕ uchar* b, ಪೂರ್ಣಾಂಕ n)
{
    CV_AVX_GUARD;

    ಪೂರ್ಣಾಂಕ i = 0;
    ಪೂರ್ಣಾಂಕ ಫಲಿತಾಂಶ = 0;

#ಆದರೆ (CV_SIMD || CV_SIMD_SCALABLE)
    {
        v_uint64 t = vx_setzero_u64();
        ಗಾಗಿ (; i <= n - VTraits<v_uint8>::vlanes(); i += VTraits<v_uint8>::vlanes())
            t = v_ಕೂಡಿಸು(t, v_popcount(v_reinterpret_as_u64(v_xor(vx_ತುಂಬು(a + i), vx_ತುಂಬು(b + i)))));
        ಫಲಿತಾಂಶ += (ಪೂರ್ಣಾಂಕ)v_reduce_ಮೊತ್ತ(t);
    }
#ಆದರೆ_ಅಂತ್ಯ

#ಆದರೆ CV_POPCNT
    {
#  ಆದರೆ defined CV_POPCNT_U64
        ಗಾಗಿ(; i <= n - 8; i += 8)
        {
            uint64_t val_a, val_b;
            std::ಸ್ಮರಣೆ_ನಕಲು(&val_a, a + i, ಗಾತ್ರ(val_a));
            std::ಸ್ಮರಣೆ_ನಕಲು(&val_b, b + i, ಗಾತ್ರ(val_b));
            ಫಲಿತಾಂಶ += (ಪೂರ್ಣಾಂಕ)CV_POPCNT_U64(val_a ^ val_b);
        }
#  endif
        ಗಾಗಿ(; i <= n - 4; i += 4)
        {
            uint32_t val_a, val_b;
            std::ಸ್ಮರಣೆ_ನಕಲು(&val_a, a + i, ಗಾತ್ರ(val_a));
            std::ಸ್ಮರಣೆ_ನಕಲು(&val_b, b + i, ಗಾತ್ರ(val_b));
            ಫಲಿತಾಂಶ += (ಪೂರ್ಣಾಂಕ)CV_POPCNT_U32(val_a ^ val_b);
        }
    }
#ಆದರೆ_ಅಂತ್ಯ
#ಆದರೆ CV_ENABLE_UNROLLED
    ಗಾಗಿ(; i <= n - 4; i += 4)
    {
        ಫಲಿತಾಂಶ += ತೆಗೆ_ಎಣಿಕೆ_ಕೋಷ್ಟಕ[a[i] ^ b[i]] + ತೆಗೆ_ಎಣಿಕೆ_ಕೋಷ್ಟಕ[a[i+1] ^ b[i+1]] +
                ತೆಗೆ_ಎಣಿಕೆ_ಕೋಷ್ಟಕ[a[i+2] ^ b[i+2]] + ತೆಗೆ_ಎಣಿಕೆ_ಕೋಷ್ಟಕ[a[i+3] ^ b[i+3]];
    }
#ಆದರೆ_ಅಂತ್ಯ
    ಗಾಗಿ(; i < n; i++)
    {
        ಫಲಿತಾಂಶ += ತೆಗೆ_ಎಣಿಕೆ_ಕೋಷ್ಟಕ[a[i] ^ b[i]];
    }
    ಹಿಂತಿರುಗಿ ಫಲಿತಾಂಶ;
}

NormHammingFunc getNormHammingFunc()
{
    NormHammingFunc f = &normHamming; // disambiguate the (a,n) overload
    ಹಿಂತಿರುಗಿ f;
}
NormHammingDiffFunc getNormHammingDiffFunc()
{
    NormHammingDiffFunc f = &normHamming; // disambiguate the (a,b,n) overload
    ಹಿಂತಿರುಗಿ f;
}

#ಆದರೆ_ಅಂತ್ಯ // CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

CV_CPU_OPTIMIZATION_NAMESPACE_ಕೊನೆ
}} //cv::hal
