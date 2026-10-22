// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.
//
// Copyright (C) 2026, Advanced Micro Devices, Inc., all rights reserved.

#include "opencv2/core/hal/intrin.hpp"

namespace cv { namespace hal {

extern const uchar popCountTable[256];

typedef ціле (*NormHammingFunc)(const uchar*, ціле);
typedef ціле (*NormHammingDiffFunc)(const uchar*, const uchar*, ціле);

CV_CPU_OPTIMIZATION_NAMESPACE_BEGIN

// forward declarations
ціле normHamming(const uchar* a, ціле n);
ціле normHamming(const uchar* a, const uchar* b, ціле n);

NormHammingFunc getNormHammingFunc();
NormHammingDiffFunc getNormHammingDiffFunc();

#ifndef CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

#якщо CV_AVX2
static inline ціле _mm256_extract_epi32_(__m256i reg, const ціле i)
{
    CV_DECL_ALIGNED(32) ціле reg_data[8];
    CV_DbgAssert(0 <= i && i < 8);
    _mm256_store_si256((__m256i*)reg_data, reg);
    повернути reg_data[i];
}
#endif

ціле normHamming(const uchar* a, ціле n)
{
    CV_AVX_GUARD;

    ціле i = 0;
    ціле result = 0;

#якщо (CV_SIMD || CV_SIMD_SCALABLE)
    {
        v_uint64 t = vx_setzero_u64();
        для (; i <= n - VTraits<v_uint8>::vlanes(); i += VTraits<v_uint8>::vlanes())
            t = v_add(t, v_popcount(v_reinterpret_as_u64(vx_load(a + i))));
        result = (ціле)v_reduce_sum(t);
        vx_cleanup();
    }
#endif

#якщо CV_POPCNT
    {
#  якщо defined CV_POPCNT_U64
        для(; i <= n - 8; i += 8)
        {
            uint64_t val;
            std::memcpy(&val, a + i, sizeof(val));
            result += (ціле)CV_POPCNT_U64(val);
        }
#  endif
        для(; i <= n - 4; i += 4)
        {
            uint32_t val;
            std::memcpy(&val, a + i, sizeof(val));
            result += CV_POPCNT_U32(val);
        }
    }
#endif
#якщо CV_ENABLE_UNROLLED
    для(; i <= n - 4; i += 4)
    {
        result += popCountTable[a[i]] + popCountTable[a[i+1]] +
        popCountTable[a[i+2]] + popCountTable[a[i+3]];
    }
#endif
    для(; i < n; i++)
    {
        result += popCountTable[a[i]];
    }
    повернути result;
}

ціле normHamming(const uchar* a, const uchar* b, ціле n)
{
    CV_AVX_GUARD;

    ціле i = 0;
    ціле result = 0;

#якщо (CV_SIMD || CV_SIMD_SCALABLE)
    {
        v_uint64 t = vx_setzero_u64();
        для (; i <= n - VTraits<v_uint8>::vlanes(); i += VTraits<v_uint8>::vlanes())
            t = v_add(t, v_popcount(v_reinterpret_as_u64(v_xor(vx_load(a + i), vx_load(b + i)))));
        result += (ціле)v_reduce_sum(t);
    }
#endif

#якщо CV_POPCNT
    {
#  якщо defined CV_POPCNT_U64
        для(; i <= n - 8; i += 8)
        {
            uint64_t val_a, val_b;
            std::memcpy(&val_a, a + i, sizeof(val_a));
            std::memcpy(&val_b, b + i, sizeof(val_b));
            result += (ціле)CV_POPCNT_U64(val_a ^ val_b);
        }
#  endif
        для(; i <= n - 4; i += 4)
        {
            uint32_t val_a, val_b;
            std::memcpy(&val_a, a + i, sizeof(val_a));
            std::memcpy(&val_b, b + i, sizeof(val_b));
            result += (ціле)CV_POPCNT_U32(val_a ^ val_b);
        }
    }
#endif
#якщо CV_ENABLE_UNROLLED
    для(; i <= n - 4; i += 4)
    {
        result += popCountTable[a[i] ^ b[i]] + popCountTable[a[i+1] ^ b[i+1]] +
                popCountTable[a[i+2] ^ b[i+2]] + popCountTable[a[i+3] ^ b[i+3]];
    }
#endif
    для(; i < n; i++)
    {
        result += popCountTable[a[i] ^ b[i]];
    }
    повернути result;
}

NormHammingFunc getNormHammingFunc()
{
    NormHammingFunc f = &normHamming; // disambiguate the (a,n) overload
    повернути f;
}
NormHammingDiffFunc getNormHammingDiffFunc()
{
    NormHammingDiffFunc f = &normHamming; // disambiguate the (a,b,n) overload
    повернути f;
}

#endif // CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

CV_CPU_OPTIMIZATION_NAMESPACE_END
}} //cv::hal
