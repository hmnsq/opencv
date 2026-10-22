// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.
//
// Copyright (C) 2026, Advanced Micro Devices, Inc., all rights reserved.

#include "opencv2/core/hal/intrin.hpp"

namespace cv { namespace hal {

extern const uchar popCountTable[256];

typedef 정수 (*NormHammingFunc)(const uchar*, 정수);
typedef 정수 (*NormHammingDiffFunc)(const uchar*, const uchar*, 정수);

CV_CPU_OPTIMIZATION_NAMESPACE_BEGIN

// forward declarations
정수 normHamming(const uchar* a, 정수 n);
정수 normHamming(const uchar* a, const uchar* b, 정수 n);

NormHammingFunc getNormHammingFunc();
NormHammingDiffFunc getNormHammingDiffFunc();

#ifndef CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

#만약 CV_AVX2
static inline 정수 _mm256_extract_epi32_(__m256i reg, const 정수 i)
{
    CV_DECL_ALIGNED(32) 정수 reg_data[8];
    CV_DbgAssert(0 <= i && i < 8);
    _mm256_store_si256((__m256i*)reg_data, reg);
    반환 reg_data[i];
}
#endif

정수 normHamming(const uchar* a, 정수 n)
{
    CV_AVX_GUARD;

    정수 i = 0;
    정수 result = 0;

#만약 (CV_SIMD || CV_SIMD_SCALABLE)
    {
        v_uint64 t = vx_setzero_u64();
        위해 (; i <= n - VTraits<v_uint8>::vlanes(); i += VTraits<v_uint8>::vlanes())
            t = v_add(t, v_popcount(v_reinterpret_as_u64(vx_load(a + i))));
        result = (정수)v_reduce_sum(t);
        vx_cleanup();
    }
#endif

#만약 CV_POPCNT
    {
#  만약 defined CV_POPCNT_U64
        위해(; i <= n - 8; i += 8)
        {
            uint64_t val;
            std::memcpy(&val, a + i, sizeof(val));
            result += (정수)CV_POPCNT_U64(val);
        }
#  endif
        위해(; i <= n - 4; i += 4)
        {
            uint32_t val;
            std::memcpy(&val, a + i, sizeof(val));
            result += CV_POPCNT_U32(val);
        }
    }
#endif
#만약 CV_ENABLE_UNROLLED
    위해(; i <= n - 4; i += 4)
    {
        result += popCountTable[a[i]] + popCountTable[a[i+1]] +
        popCountTable[a[i+2]] + popCountTable[a[i+3]];
    }
#endif
    위해(; i < n; i++)
    {
        result += popCountTable[a[i]];
    }
    반환 result;
}

정수 normHamming(const uchar* a, const uchar* b, 정수 n)
{
    CV_AVX_GUARD;

    정수 i = 0;
    정수 result = 0;

#만약 (CV_SIMD || CV_SIMD_SCALABLE)
    {
        v_uint64 t = vx_setzero_u64();
        위해 (; i <= n - VTraits<v_uint8>::vlanes(); i += VTraits<v_uint8>::vlanes())
            t = v_add(t, v_popcount(v_reinterpret_as_u64(v_xor(vx_load(a + i), vx_load(b + i)))));
        result += (정수)v_reduce_sum(t);
    }
#endif

#만약 CV_POPCNT
    {
#  만약 defined CV_POPCNT_U64
        위해(; i <= n - 8; i += 8)
        {
            uint64_t val_a, val_b;
            std::memcpy(&val_a, a + i, sizeof(val_a));
            std::memcpy(&val_b, b + i, sizeof(val_b));
            result += (정수)CV_POPCNT_U64(val_a ^ val_b);
        }
#  endif
        위해(; i <= n - 4; i += 4)
        {
            uint32_t val_a, val_b;
            std::memcpy(&val_a, a + i, sizeof(val_a));
            std::memcpy(&val_b, b + i, sizeof(val_b));
            result += (정수)CV_POPCNT_U32(val_a ^ val_b);
        }
    }
#endif
#만약 CV_ENABLE_UNROLLED
    위해(; i <= n - 4; i += 4)
    {
        result += popCountTable[a[i] ^ b[i]] + popCountTable[a[i+1] ^ b[i+1]] +
                popCountTable[a[i+2] ^ b[i+2]] + popCountTable[a[i+3] ^ b[i+3]];
    }
#endif
    위해(; i < n; i++)
    {
        result += popCountTable[a[i] ^ b[i]];
    }
    반환 result;
}

NormHammingFunc getNormHammingFunc()
{
    NormHammingFunc f = &normHamming; // disambiguate the (a,n) overload
    반환 f;
}
NormHammingDiffFunc getNormHammingDiffFunc()
{
    NormHammingDiffFunc f = &normHamming; // disambiguate the (a,b,n) overload
    반환 f;
}

#endif // CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

CV_CPU_OPTIMIZATION_NAMESPACE_END
}} //cv::hal
