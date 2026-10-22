// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.
//
// Copyright (C) 2026, Advanced Micro Devices, Inc., all rights reserved.

#அந்தர்பாவ "opencv2/core/hal/intrin.hpp"

நாமாகாஶ cv { நாமாகாஶ hal {

வெளி மாறிலி uchar எடு_மொத்தம்_Table[256];

வகைவரை முழு (*NormHammingFunc)(மாறிலி uchar*, முழு);
வகைவரை முழு (*NormHammingDiffFunc)(மாறிலி uchar*, மாறிலி uchar*, முழு);

CV_CPU_OPTIMIZATION_NAMESPACE_BEGIN

// forward declarations
முழு normHamming(மாறிலி uchar* a, முழு n);
முழு normHamming(மாறிலி uchar* a, மாறிலி uchar* b, முழு n);

NormHammingFunc getNormHammingFunc();
NormHammingDiffFunc getNormHammingDiffFunc();

#ifndef CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

#எனில் CV_AVX2
நிலையான உள்வரி முழு _mm256_extract_epi32_(__m256i reg, மாறிலி முழு i)
{
    CV_DECL_ALIGNED(32) முழு reg_data[8];
    CV_DbgAssert(0 <= i && i < 8);
    _mm256_store_si256((__m256i*)reg_data, reg);
    திரும்பு reg_data[i];
}
#endif

முழு normHamming(மாறிலி uchar* a, முழு n)
{
    CV_AVX_GUARD;

    முழு i = 0;
    முழு result = 0;

#எனில் (CV_SIMD || CV_SIMD_SCALABLE)
    {
        v_uint64 t = vx_setzero_u64();
        க்கு (; i <= n - VTraits<v_uint8>::vlanes(); i += VTraits<v_uint8>::vlanes())
            t = v_add(t, v_popcount(v_reinterpret_as_u64(vx_load(a + i))));
        result = (முழு)v_reduce_sum(t);
        vx_cleanup();
    }
#endif

#எனில் CV_POPCNT
    {
#  எனில் defined CV_POPCNT_U64
        க்கு(; i <= n - 8; i += 8)
        {
            uint64_t val;
            std::memcpy(&val, a + i, அளவு(val));
            result += (முழு)CV_POPCNT_U64(val);
        }
#  endif
        க்கு(; i <= n - 4; i += 4)
        {
            uint32_t val;
            std::memcpy(&val, a + i, அளவு(val));
            result += CV_POPCNT_U32(val);
        }
    }
#endif
#எனில் CV_ENABLE_UNROLLED
    க்கு(; i <= n - 4; i += 4)
    {
        result += எடு_மொத்தம்_Table[a[i]] + எடு_மொத்தம்_Table[a[i+1]] +
        எடு_மொத்தம்_Table[a[i+2]] + எடு_மொத்தம்_Table[a[i+3]];
    }
#endif
    க்கு(; i < n; i++)
    {
        result += எடு_மொத்தம்_Table[a[i]];
    }
    திரும்பு result;
}

முழு normHamming(மாறிலி uchar* a, மாறிலி uchar* b, முழு n)
{
    CV_AVX_GUARD;

    முழு i = 0;
    முழு result = 0;

#எனில் (CV_SIMD || CV_SIMD_SCALABLE)
    {
        v_uint64 t = vx_setzero_u64();
        க்கு (; i <= n - VTraits<v_uint8>::vlanes(); i += VTraits<v_uint8>::vlanes())
            t = v_add(t, v_popcount(v_reinterpret_as_u64(v_xor(vx_load(a + i), vx_load(b + i)))));
        result += (முழு)v_reduce_sum(t);
    }
#endif

#எனில் CV_POPCNT
    {
#  எனில் defined CV_POPCNT_U64
        க்கு(; i <= n - 8; i += 8)
        {
            uint64_t val_a, val_b;
            std::memcpy(&val_a, a + i, அளவு(val_a));
            std::memcpy(&val_b, b + i, அளவு(val_b));
            result += (முழு)CV_POPCNT_U64(val_a ^ val_b);
        }
#  endif
        க்கு(; i <= n - 4; i += 4)
        {
            uint32_t val_a, val_b;
            std::memcpy(&val_a, a + i, அளவு(val_a));
            std::memcpy(&val_b, b + i, அளவு(val_b));
            result += (முழு)CV_POPCNT_U32(val_a ^ val_b);
        }
    }
#endif
#எனில் CV_ENABLE_UNROLLED
    க்கு(; i <= n - 4; i += 4)
    {
        result += எடு_மொத்தம்_Table[a[i] ^ b[i]] + எடு_மொத்தம்_Table[a[i+1] ^ b[i+1]] +
                எடு_மொத்தம்_Table[a[i+2] ^ b[i+2]] + எடு_மொத்தம்_Table[a[i+3] ^ b[i+3]];
    }
#endif
    க்கு(; i < n; i++)
    {
        result += எடு_மொத்தம்_Table[a[i] ^ b[i]];
    }
    திரும்பு result;
}

NormHammingFunc getNormHammingFunc()
{
    NormHammingFunc f = &normHamming; // disambiguate the (a,n) overload
    திரும்பு f;
}
NormHammingDiffFunc getNormHammingDiffFunc()
{
    NormHammingDiffFunc f = &normHamming; // disambiguate the (a,b,n) overload
    திரும்பு f;
}

#endif // CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

CV_CPU_OPTIMIZATION_NAMESPACE_END
}} //cv::hal
