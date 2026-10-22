// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.
//
// Copyright (C) 2026, Advanced Micro Devices, Inc., all rights reserved.

#include "opencv2/core/hal/intrin.hpp"

namespace cv { namespace hal {

extern const uchar popCountTable[256];

typedef цяло (*NormHammingFunc)(const uchar*, цяло);
typedef цяло (*NormHammingDiffFunc)(const uchar*, const uchar*, цяло);

CV_CPU_OPTIMIZATION_NAMESPACE_BEGIN

// forward declarations
цяло normHamming(const uchar* a, цяло n);
цяло normHamming(const uchar* a, const uchar* b, цяло n);

NormHammingFunc getNormHammingFunc();
NormHammingDiffFunc getNormHammingDiffFunc();

#ifndef CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

#ако CV_AVX2
static inline цяло _mm256_extract_epi32_(__m256i reg, const цяло i)
{
    CV_DECL_ALIGNED(32) цяло reg_data[8];
    CV_DbgAssert(0 <= i && i < 8);
    _mm256_store_si256((__m256i*)reg_data, reg);
    върни reg_data[i];
}
#endif

цяло normHamming(const uchar* a, цяло n)
{
    CV_AVX_GUARD;

    цяло i = 0;
    цяло result = 0;

#ако (CV_SIMD || CV_SIMD_SCALABLE)
    {
        v_uint64 t = vx_setzero_u64();
        за (; i <= n - VTraits<v_uint8>::vlanes(); i += VTraits<v_uint8>::vlanes())
            t = v_add(t, v_popcount(v_reinterpret_as_u64(vx_load(a + i))));
        result = (цяло)v_reduce_sum(t);
        vx_cleanup();
    }
#endif

#ако CV_POPCNT
    {
#  ако defined CV_POPCNT_U64
        за(; i <= n - 8; i += 8)
        {
            uint64_t val;
            std::memcpy(&val, a + i, sizeof(val));
            result += (цяло)CV_POPCNT_U64(val);
        }
#  endif
        за(; i <= n - 4; i += 4)
        {
            uint32_t val;
            std::memcpy(&val, a + i, sizeof(val));
            result += CV_POPCNT_U32(val);
        }
    }
#endif
#ако CV_ENABLE_UNROLLED
    за(; i <= n - 4; i += 4)
    {
        result += popCountTable[a[i]] + popCountTable[a[i+1]] +
        popCountTable[a[i+2]] + popCountTable[a[i+3]];
    }
#endif
    за(; i < n; i++)
    {
        result += popCountTable[a[i]];
    }
    върни result;
}

цяло normHamming(const uchar* a, const uchar* b, цяло n)
{
    CV_AVX_GUARD;

    цяло i = 0;
    цяло result = 0;

#ако (CV_SIMD || CV_SIMD_SCALABLE)
    {
        v_uint64 t = vx_setzero_u64();
        за (; i <= n - VTraits<v_uint8>::vlanes(); i += VTraits<v_uint8>::vlanes())
            t = v_add(t, v_popcount(v_reinterpret_as_u64(v_xor(vx_load(a + i), vx_load(b + i)))));
        result += (цяло)v_reduce_sum(t);
    }
#endif

#ако CV_POPCNT
    {
#  ако defined CV_POPCNT_U64
        за(; i <= n - 8; i += 8)
        {
            uint64_t val_a, val_b;
            std::memcpy(&val_a, a + i, sizeof(val_a));
            std::memcpy(&val_b, b + i, sizeof(val_b));
            result += (цяло)CV_POPCNT_U64(val_a ^ val_b);
        }
#  endif
        за(; i <= n - 4; i += 4)
        {
            uint32_t val_a, val_b;
            std::memcpy(&val_a, a + i, sizeof(val_a));
            std::memcpy(&val_b, b + i, sizeof(val_b));
            result += (цяло)CV_POPCNT_U32(val_a ^ val_b);
        }
    }
#endif
#ако CV_ENABLE_UNROLLED
    за(; i <= n - 4; i += 4)
    {
        result += popCountTable[a[i] ^ b[i]] + popCountTable[a[i+1] ^ b[i+1]] +
                popCountTable[a[i+2] ^ b[i+2]] + popCountTable[a[i+3] ^ b[i+3]];
    }
#endif
    за(; i < n; i++)
    {
        result += popCountTable[a[i] ^ b[i]];
    }
    върни result;
}

NormHammingFunc getNormHammingFunc()
{
    NormHammingFunc f = &normHamming; // disambiguate the (a,n) overload
    върни f;
}
NormHammingDiffFunc getNormHammingDiffFunc()
{
    NormHammingDiffFunc f = &normHamming; // disambiguate the (a,b,n) overload
    върни f;
}

#endif // CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

CV_CPU_OPTIMIZATION_NAMESPACE_END
}} //cv::hal
