// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.
//
// Copyright (C) 2026, Advanced Micro Devices, Inc., all rights reserved.

#ಅನ್ತರ್ಭಾವ "opencv2/core/hal/intrin.hpp"

ನಾಮಾಕಾಶ cv { ನಾಮಾಕಾಶ hal {

ಬಾಹ್ಯ ಧ್ರುವ uchar ತೆಗೆ_ಎಣಿಕೆ_Table[256];

ಪ್ರಕಾರನಿರ್ದೇಶ ಪೂರ್ಣಾಙ್ಕ (*NormHammingFunc)(ಧ್ರುವ uchar*, ಪೂರ್ಣಾಙ್ಕ);
ಪ್ರಕಾರನಿರ್ದೇಶ ಪೂರ್ಣಾಙ್ಕ (*NormHammingDiffFunc)(ಧ್ರುವ uchar*, ಧ್ರುವ uchar*, ಪೂರ್ಣಾಙ್ಕ);

CV_CPU_OPTIMIZATION_NAMESPACE_BEGIN

// forward declarations
ಪೂರ್ಣಾಙ್ಕ normHamming(ಧ್ರುವ uchar* a, ಪೂರ್ಣಾಙ್ಕ n);
ಪೂರ್ಣಾಙ್ಕ normHamming(ಧ್ರುವ uchar* a, ಧ್ರುವ uchar* b, ಪೂರ್ಣಾಙ್ಕ n);

NormHammingFunc getNormHammingFunc();
NormHammingDiffFunc getNormHammingDiffFunc();

#ifndef CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

#ಯದಿ CV_AVX2
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಪೂರ್ಣಾಙ್ಕ _mm256_extract_epi32_(__m256i reg, ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ i)
{
    CV_DECL_ALIGNED(32) ಪೂರ್ಣಾಙ್ಕ reg_data[8];
    CV_DbgAssert(0 <= i && i < 8);
    _mm256_store_si256((__m256i*)reg_data, reg);
    ಪ್ರತ್ಯಾಗಮ reg_data[i];
}
#endif

ಪೂರ್ಣಾಙ್ಕ normHamming(ಧ್ರುವ uchar* a, ಪೂರ್ಣಾಙ್ಕ n)
{
    CV_AVX_GUARD;

    ಪೂರ್ಣಾಙ್ಕ i = 0;
    ಪೂರ್ಣಾಙ್ಕ result = 0;

#ಯದಿ (CV_SIMD || CV_SIMD_SCALABLE)
    {
        v_uint64 t = vx_setzero_u64();
        ಪ್ರತಿ (; i <= n - VTraits<v_uint8>::vlanes(); i += VTraits<v_uint8>::vlanes())
            t = v_add(t, v_popcount(v_reinterpret_as_u64(vx_load(a + i))));
        result = (ಪೂರ್ಣಾಙ್ಕ)v_reduce_sum(t);
        vx_cleanup();
    }
#endif

#ಯದಿ CV_POPCNT
    {
#  ಯದಿ defined CV_POPCNT_U64
        ಪ್ರತಿ(; i <= n - 8; i += 8)
        {
            uint64_t val;
            std::memcpy(&val, a + i, ಪರಿಮಾಣ(val));
            result += (ಪೂರ್ಣಾಙ್ಕ)CV_POPCNT_U64(val);
        }
#  endif
        ಪ್ರತಿ(; i <= n - 4; i += 4)
        {
            uint32_t val;
            std::memcpy(&val, a + i, ಪರಿಮಾಣ(val));
            result += CV_POPCNT_U32(val);
        }
    }
#endif
#ಯದಿ CV_ENABLE_UNROLLED
    ಪ್ರತಿ(; i <= n - 4; i += 4)
    {
        result += ತೆಗೆ_ಎಣಿಕೆ_Table[a[i]] + ತೆಗೆ_ಎಣಿಕೆ_Table[a[i+1]] +
        ತೆಗೆ_ಎಣಿಕೆ_Table[a[i+2]] + ತೆಗೆ_ಎಣಿಕೆ_Table[a[i+3]];
    }
#endif
    ಪ್ರತಿ(; i < n; i++)
    {
        result += ತೆಗೆ_ಎಣಿಕೆ_Table[a[i]];
    }
    ಪ್ರತ್ಯಾಗಮ result;
}

ಪೂರ್ಣಾಙ್ಕ normHamming(ಧ್ರುವ uchar* a, ಧ್ರುವ uchar* b, ಪೂರ್ಣಾಙ್ಕ n)
{
    CV_AVX_GUARD;

    ಪೂರ್ಣಾಙ್ಕ i = 0;
    ಪೂರ್ಣಾಙ್ಕ result = 0;

#ಯದಿ (CV_SIMD || CV_SIMD_SCALABLE)
    {
        v_uint64 t = vx_setzero_u64();
        ಪ್ರತಿ (; i <= n - VTraits<v_uint8>::vlanes(); i += VTraits<v_uint8>::vlanes())
            t = v_add(t, v_popcount(v_reinterpret_as_u64(v_xor(vx_load(a + i), vx_load(b + i)))));
        result += (ಪೂರ್ಣಾಙ್ಕ)v_reduce_sum(t);
    }
#endif

#ಯದಿ CV_POPCNT
    {
#  ಯದಿ defined CV_POPCNT_U64
        ಪ್ರತಿ(; i <= n - 8; i += 8)
        {
            uint64_t val_a, val_b;
            std::memcpy(&val_a, a + i, ಪರಿಮಾಣ(val_a));
            std::memcpy(&val_b, b + i, ಪರಿಮಾಣ(val_b));
            result += (ಪೂರ್ಣಾಙ್ಕ)CV_POPCNT_U64(val_a ^ val_b);
        }
#  endif
        ಪ್ರತಿ(; i <= n - 4; i += 4)
        {
            uint32_t val_a, val_b;
            std::memcpy(&val_a, a + i, ಪರಿಮಾಣ(val_a));
            std::memcpy(&val_b, b + i, ಪರಿಮಾಣ(val_b));
            result += (ಪೂರ್ಣಾಙ್ಕ)CV_POPCNT_U32(val_a ^ val_b);
        }
    }
#endif
#ಯದಿ CV_ENABLE_UNROLLED
    ಪ್ರತಿ(; i <= n - 4; i += 4)
    {
        result += ತೆಗೆ_ಎಣಿಕೆ_Table[a[i] ^ b[i]] + ತೆಗೆ_ಎಣಿಕೆ_Table[a[i+1] ^ b[i+1]] +
                ತೆಗೆ_ಎಣಿಕೆ_Table[a[i+2] ^ b[i+2]] + ತೆಗೆ_ಎಣಿಕೆ_Table[a[i+3] ^ b[i+3]];
    }
#endif
    ಪ್ರತಿ(; i < n; i++)
    {
        result += ತೆಗೆ_ಎಣಿಕೆ_Table[a[i] ^ b[i]];
    }
    ಪ್ರತ್ಯಾಗಮ result;
}

NormHammingFunc getNormHammingFunc()
{
    NormHammingFunc f = &normHamming; // disambiguate the (a,n) overload
    ಪ್ರತ್ಯಾಗಮ f;
}
NormHammingDiffFunc getNormHammingDiffFunc()
{
    NormHammingDiffFunc f = &normHamming; // disambiguate the (a,b,n) overload
    ಪ್ರತ್ಯಾಗಮ f;
}

#endif // CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

CV_CPU_OPTIMIZATION_NAMESPACE_END
}} //cv::hal
