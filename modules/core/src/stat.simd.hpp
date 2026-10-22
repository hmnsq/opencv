// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.
//
// Copyright (C) 2026, Advanced Micro Devices, Inc., all rights reserved.

#include "opencv2/core/hal/intrin.hpp"

namespace cv { namespace hal {

extern const uchar popCountTable[256];

typedef số_nguyên (*NormHammingFunc)(const uchar*, số_nguyên);
typedef số_nguyên (*NormHammingDiffFunc)(const uchar*, const uchar*, số_nguyên);

CV_CPU_OPTIMIZATION_NAMESPACE_BEGIN

// forward declarations
số_nguyên normHamming(const uchar* a, số_nguyên n);
số_nguyên normHamming(const uchar* a, const uchar* b, số_nguyên n);

NormHammingFunc getNormHammingFunc();
NormHammingDiffFunc getNormHammingDiffFunc();

#ifndef CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

#nếu CV_AVX2
static inline số_nguyên _mm256_extract_epi32_(__m256i reg, const số_nguyên i)
{
    CV_DECL_ALIGNED(32) số_nguyên reg_data[8];
    CV_DbgAssert(0 <= i && i < 8);
    _mm256_store_si256((__m256i*)reg_data, reg);
    trả_về reg_data[i];
}
#endif

số_nguyên normHamming(const uchar* a, số_nguyên n)
{
    CV_AVX_GUARD;

    số_nguyên i = 0;
    số_nguyên result = 0;

#nếu (CV_SIMD || CV_SIMD_SCALABLE)
    {
        v_uint64 t = vx_setzero_u64();
        cho (; i <= n - VTraits<v_uint8>::vlanes(); i += VTraits<v_uint8>::vlanes())
            t = v_add(t, v_popcount(v_reinterpret_as_u64(vx_load(a + i))));
        result = (số_nguyên)v_reduce_sum(t);
        vx_cleanup();
    }
#endif

#nếu CV_POPCNT
    {
#  nếu defined CV_POPCNT_U64
        cho(; i <= n - 8; i += 8)
        {
            uint64_t val;
            std::memcpy(&val, a + i, sizeof(val));
            result += (số_nguyên)CV_POPCNT_U64(val);
        }
#  endif
        cho(; i <= n - 4; i += 4)
        {
            uint32_t val;
            std::memcpy(&val, a + i, sizeof(val));
            result += CV_POPCNT_U32(val);
        }
    }
#endif
#nếu CV_ENABLE_UNROLLED
    cho(; i <= n - 4; i += 4)
    {
        result += popCountTable[a[i]] + popCountTable[a[i+1]] +
        popCountTable[a[i+2]] + popCountTable[a[i+3]];
    }
#endif
    cho(; i < n; i++)
    {
        result += popCountTable[a[i]];
    }
    trả_về result;
}

số_nguyên normHamming(const uchar* a, const uchar* b, số_nguyên n)
{
    CV_AVX_GUARD;

    số_nguyên i = 0;
    số_nguyên result = 0;

#nếu (CV_SIMD || CV_SIMD_SCALABLE)
    {
        v_uint64 t = vx_setzero_u64();
        cho (; i <= n - VTraits<v_uint8>::vlanes(); i += VTraits<v_uint8>::vlanes())
            t = v_add(t, v_popcount(v_reinterpret_as_u64(v_xor(vx_load(a + i), vx_load(b + i)))));
        result += (số_nguyên)v_reduce_sum(t);
    }
#endif

#nếu CV_POPCNT
    {
#  nếu defined CV_POPCNT_U64
        cho(; i <= n - 8; i += 8)
        {
            uint64_t val_a, val_b;
            std::memcpy(&val_a, a + i, sizeof(val_a));
            std::memcpy(&val_b, b + i, sizeof(val_b));
            result += (số_nguyên)CV_POPCNT_U64(val_a ^ val_b);
        }
#  endif
        cho(; i <= n - 4; i += 4)
        {
            uint32_t val_a, val_b;
            std::memcpy(&val_a, a + i, sizeof(val_a));
            std::memcpy(&val_b, b + i, sizeof(val_b));
            result += (số_nguyên)CV_POPCNT_U32(val_a ^ val_b);
        }
    }
#endif
#nếu CV_ENABLE_UNROLLED
    cho(; i <= n - 4; i += 4)
    {
        result += popCountTable[a[i] ^ b[i]] + popCountTable[a[i+1] ^ b[i+1]] +
                popCountTable[a[i+2] ^ b[i+2]] + popCountTable[a[i+3] ^ b[i+3]];
    }
#endif
    cho(; i < n; i++)
    {
        result += popCountTable[a[i] ^ b[i]];
    }
    trả_về result;
}

NormHammingFunc getNormHammingFunc()
{
    NormHammingFunc f = &normHamming; // disambiguate the (a,n) overload
    trả_về f;
}
NormHammingDiffFunc getNormHammingDiffFunc()
{
    NormHammingDiffFunc f = &normHamming; // disambiguate the (a,b,n) overload
    trả_về f;
}

#endif // CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

CV_CPU_OPTIMIZATION_NAMESPACE_END
}} //cv::hal
