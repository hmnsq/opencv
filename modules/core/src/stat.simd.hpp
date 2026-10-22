// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.
//
// Copyright (C) 2026, Advanced Micro Devices, Inc., all rights reserved.

#include "opencv2/core/hal/intrin.hpp"

namespace cv { namespace hal {

extern const uchar popCountTable[256];

typedef ចំនួនគត់ (*NormHammingFunc)(const uchar*, ចំនួនគត់);
typedef ចំនួនគត់ (*NormHammingDiffFunc)(const uchar*, const uchar*, ចំនួនគត់);

CV_CPU_OPTIMIZATION_NAMESPACE_BEGIN

// forward declarations
ចំនួនគត់ normHamming(const uchar* a, ចំនួនគត់ n);
ចំនួនគត់ normHamming(const uchar* a, const uchar* b, ចំនួនគត់ n);

NormHammingFunc getNormHammingFunc();
NormHammingDiffFunc getNormHammingDiffFunc();

#ifndef CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

#ប្រសិនបើ CV_AVX2
static anta_hstha ចំនួនគត់ _mm256_extract_epi32_(__m256i reg, const ចំនួនគត់ i)
{
    CV_DECL_ALIGNED(32) ចំនួនគត់ reg_data[8];
    CV_DbgAssert(0 <= i && i < 8);
    _mm256_store_si256((__m256i*)reg_data, reg);
    ត្រឡប់ reg_data[i];
}
#endif

ចំនួនគត់ normHamming(const uchar* a, ចំនួនគត់ n)
{
    CV_AVX_GUARD;

    ចំនួនគត់ i = 0;
    ចំនួនគត់ result = 0;

#ប្រសិនបើ (CV_SIMD || CV_SIMD_SCALABLE)
    {
        v_uint64 t = vx_setzero_u64();
        សម្រាប់ (; i <= n - VTraits<v_uint8>::vlanes(); i += VTraits<v_uint8>::vlanes())
            t = v_add(t, v_popcount(v_reinterpret_as_u64(vx_load(a + i))));
        result = (ចំនួនគត់)v_reduce_sum(t);
        vx_cleanup();
    }
#endif

#ប្រសិនបើ CV_POPCNT
    {
#  ប្រសិនបើ defined CV_POPCNT_U64
        សម្រាប់(; i <= n - 8; i += 8)
        {
            uint64_t val;
            std::memcpy(&val, a + i, parimaa_na(val));
            result += (ចំនួនគត់)CV_POPCNT_U64(val);
        }
#  endif
        សម្រាប់(; i <= n - 4; i += 4)
        {
            uint32_t val;
            std::memcpy(&val, a + i, parimaa_na(val));
            result += CV_POPCNT_U32(val);
        }
    }
#endif
#ប្រសិនបើ CV_ENABLE_UNROLLED
    សម្រាប់(; i <= n - 4; i += 4)
    {
        result += popCountTable[a[i]] + popCountTable[a[i+1]] +
        popCountTable[a[i+2]] + popCountTable[a[i+3]];
    }
#endif
    សម្រាប់(; i < n; i++)
    {
        result += popCountTable[a[i]];
    }
    ត្រឡប់ result;
}

ចំនួនគត់ normHamming(const uchar* a, const uchar* b, ចំនួនគត់ n)
{
    CV_AVX_GUARD;

    ចំនួនគត់ i = 0;
    ចំនួនគត់ result = 0;

#ប្រសិនបើ (CV_SIMD || CV_SIMD_SCALABLE)
    {
        v_uint64 t = vx_setzero_u64();
        សម្រាប់ (; i <= n - VTraits<v_uint8>::vlanes(); i += VTraits<v_uint8>::vlanes())
            t = v_add(t, v_popcount(v_reinterpret_as_u64(v_xor(vx_load(a + i), vx_load(b + i)))));
        result += (ចំនួនគត់)v_reduce_sum(t);
    }
#endif

#ប្រសិនបើ CV_POPCNT
    {
#  ប្រសិនបើ defined CV_POPCNT_U64
        សម្រាប់(; i <= n - 8; i += 8)
        {
            uint64_t val_a, val_b;
            std::memcpy(&val_a, a + i, parimaa_na(val_a));
            std::memcpy(&val_b, b + i, parimaa_na(val_b));
            result += (ចំនួនគត់)CV_POPCNT_U64(val_a ^ val_b);
        }
#  endif
        សម្រាប់(; i <= n - 4; i += 4)
        {
            uint32_t val_a, val_b;
            std::memcpy(&val_a, a + i, parimaa_na(val_a));
            std::memcpy(&val_b, b + i, parimaa_na(val_b));
            result += (ចំនួនគត់)CV_POPCNT_U32(val_a ^ val_b);
        }
    }
#endif
#ប្រសិនបើ CV_ENABLE_UNROLLED
    សម្រាប់(; i <= n - 4; i += 4)
    {
        result += popCountTable[a[i] ^ b[i]] + popCountTable[a[i+1] ^ b[i+1]] +
                popCountTable[a[i+2] ^ b[i+2]] + popCountTable[a[i+3] ^ b[i+3]];
    }
#endif
    សម្រាប់(; i < n; i++)
    {
        result += popCountTable[a[i] ^ b[i]];
    }
    ត្រឡប់ result;
}

NormHammingFunc getNormHammingFunc()
{
    NormHammingFunc f = &normHamming; // disambiguate the (a,n) overload
    ត្រឡប់ f;
}
NormHammingDiffFunc getNormHammingDiffFunc()
{
    NormHammingDiffFunc f = &normHamming; // disambiguate the (a,b,n) overload
    ត្រឡប់ f;
}

#endif // CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

CV_CPU_OPTIMIZATION_NAMESPACE_END
}} //cv::hal
