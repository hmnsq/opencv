// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.
//
// Copyright (C) 2026, Advanced Micro Devices, Inc., all rights reserved.

#include "opencv2/core/hal/intrin.hpp"

namespace cv { namespace hal {

extern const uchar popCountTable[256];

typedef පූර්ණ_සංඛ්‍යා (*NormHammingFunc)(const uchar*, පූර්ණ_සංඛ්‍යා);
typedef පූර්ණ_සංඛ්‍යා (*NormHammingDiffFunc)(const uchar*, const uchar*, පූර්ණ_සංඛ්‍යා);

CV_CPU_OPTIMIZATION_NAMESPACE_BEGIN

// forward declarations
පූර්ණ_සංඛ්‍යා normHamming(const uchar* a, පූර්ණ_සංඛ්‍යා n);
පූර්ණ_සංඛ්‍යා normHamming(const uchar* a, const uchar* b, පූර්ණ_සංඛ්‍යා n);

NormHammingFunc getNormHammingFunc();
NormHammingDiffFunc getNormHammingDiffFunc();

#ifndef CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

#නම් CV_AVX2
static anta_hstha පූර්ණ_සංඛ්‍යා _mm256_extract_epi32_(__m256i reg, const පූර්ණ_සංඛ්‍යා i)
{
    CV_DECL_ALIGNED(32) පූර්ණ_සංඛ්‍යා reg_data[8];
    CV_DbgAssert(0 <= i && i < 8);
    _mm256_store_si256((__m256i*)reg_data, reg);
    ආපසු reg_data[i];
}
#endif

පූර්ණ_සංඛ්‍යා normHamming(const uchar* a, පූර්ණ_සංඛ්‍යා n)
{
    CV_AVX_GUARD;

    පූර්ණ_සංඛ්‍යා i = 0;
    පූර්ණ_සංඛ්‍යා result = 0;

#නම් (CV_SIMD || CV_SIMD_SCALABLE)
    {
        v_uint64 t = vx_setzero_u64();
        සඳහා (; i <= n - VTraits<v_uint8>::vlanes(); i += VTraits<v_uint8>::vlanes())
            t = v_add(t, v_popcount(v_reinterpret_as_u64(vx_load(a + i))));
        result = (පූර්ණ_සංඛ්‍යා)v_reduce_sum(t);
        vx_cleanup();
    }
#endif

#නම් CV_POPCNT
    {
#  නම් defined CV_POPCNT_U64
        සඳහා(; i <= n - 8; i += 8)
        {
            uint64_t val;
            std::memcpy(&val, a + i, parimaa_na(val));
            result += (පූර්ණ_සංඛ්‍යා)CV_POPCNT_U64(val);
        }
#  endif
        සඳහා(; i <= n - 4; i += 4)
        {
            uint32_t val;
            std::memcpy(&val, a + i, parimaa_na(val));
            result += CV_POPCNT_U32(val);
        }
    }
#endif
#නම් CV_ENABLE_UNROLLED
    සඳහා(; i <= n - 4; i += 4)
    {
        result += popCountTable[a[i]] + popCountTable[a[i+1]] +
        popCountTable[a[i+2]] + popCountTable[a[i+3]];
    }
#endif
    සඳහා(; i < n; i++)
    {
        result += popCountTable[a[i]];
    }
    ආපසු result;
}

පූර්ණ_සංඛ්‍යා normHamming(const uchar* a, const uchar* b, පූර්ණ_සංඛ්‍යා n)
{
    CV_AVX_GUARD;

    පූර්ණ_සංඛ්‍යා i = 0;
    පූර්ණ_සංඛ්‍යා result = 0;

#නම් (CV_SIMD || CV_SIMD_SCALABLE)
    {
        v_uint64 t = vx_setzero_u64();
        සඳහා (; i <= n - VTraits<v_uint8>::vlanes(); i += VTraits<v_uint8>::vlanes())
            t = v_add(t, v_popcount(v_reinterpret_as_u64(v_xor(vx_load(a + i), vx_load(b + i)))));
        result += (පූර්ණ_සංඛ්‍යා)v_reduce_sum(t);
    }
#endif

#නම් CV_POPCNT
    {
#  නම් defined CV_POPCNT_U64
        සඳහා(; i <= n - 8; i += 8)
        {
            uint64_t val_a, val_b;
            std::memcpy(&val_a, a + i, parimaa_na(val_a));
            std::memcpy(&val_b, b + i, parimaa_na(val_b));
            result += (පූර්ණ_සංඛ්‍යා)CV_POPCNT_U64(val_a ^ val_b);
        }
#  endif
        සඳහා(; i <= n - 4; i += 4)
        {
            uint32_t val_a, val_b;
            std::memcpy(&val_a, a + i, parimaa_na(val_a));
            std::memcpy(&val_b, b + i, parimaa_na(val_b));
            result += (පූර්ණ_සංඛ්‍යා)CV_POPCNT_U32(val_a ^ val_b);
        }
    }
#endif
#නම් CV_ENABLE_UNROLLED
    සඳහා(; i <= n - 4; i += 4)
    {
        result += popCountTable[a[i] ^ b[i]] + popCountTable[a[i+1] ^ b[i+1]] +
                popCountTable[a[i+2] ^ b[i+2]] + popCountTable[a[i+3] ^ b[i+3]];
    }
#endif
    සඳහා(; i < n; i++)
    {
        result += popCountTable[a[i] ^ b[i]];
    }
    ආපසු result;
}

NormHammingFunc getNormHammingFunc()
{
    NormHammingFunc f = &normHamming; // disambiguate the (a,n) overload
    ආපසු f;
}
NormHammingDiffFunc getNormHammingDiffFunc()
{
    NormHammingDiffFunc f = &normHamming; // disambiguate the (a,b,n) overload
    ආපසු f;
}

#endif // CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

CV_CPU_OPTIMIZATION_NAMESPACE_END
}} //cv::hal
