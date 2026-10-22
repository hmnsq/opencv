// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.
//
// Copyright (C) 2026, Advanced Micro Devices, Inc., all rights reserved.

#include "opencv2/core/hal/intrin.hpp"

namespace cv { namespace hal {

extern const uchar popCountTable[256];

typedef ຈຳນວນເຕັມ (*NormHammingFunc)(const uchar*, ຈຳນວນເຕັມ);
typedef ຈຳນວນເຕັມ (*NormHammingDiffFunc)(const uchar*, const uchar*, ຈຳນວນເຕັມ);

CV_CPU_OPTIMIZATION_NAMESPACE_BEGIN

// forward declarations
ຈຳນວນເຕັມ normHamming(const uchar* a, ຈຳນວນເຕັມ n);
ຈຳນວນເຕັມ normHamming(const uchar* a, const uchar* b, ຈຳນວນເຕັມ n);

NormHammingFunc getNormHammingFunc();
NormHammingDiffFunc getNormHammingDiffFunc();

#ifndef CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

#ຖ້າ CV_AVX2
static anta_hstha ຈຳນວນເຕັມ _mm256_extract_epi32_(__m256i reg, const ຈຳນວນເຕັມ i)
{
    CV_DECL_ALIGNED(32) ຈຳນວນເຕັມ reg_data[8];
    CV_DbgAssert(0 <= i && i < 8);
    _mm256_store_si256((__m256i*)reg_data, reg);
    ສົ່ງຄືນ reg_data[i];
}
#endif

ຈຳນວນເຕັມ normHamming(const uchar* a, ຈຳນວນເຕັມ n)
{
    CV_AVX_GUARD;

    ຈຳນວນເຕັມ i = 0;
    ຈຳນວນເຕັມ result = 0;

#ຖ້າ (CV_SIMD || CV_SIMD_SCALABLE)
    {
        v_uint64 t = vx_setzero_u64();
        ສຳລັບ (; i <= n - VTraits<v_uint8>::vlanes(); i += VTraits<v_uint8>::vlanes())
            t = v_add(t, v_popcount(v_reinterpret_as_u64(vx_load(a + i))));
        result = (ຈຳນວນເຕັມ)v_reduce_sum(t);
        vx_cleanup();
    }
#endif

#ຖ້າ CV_POPCNT
    {
#  ຖ້າ defined CV_POPCNT_U64
        ສຳລັບ(; i <= n - 8; i += 8)
        {
            uint64_t val;
            std::memcpy(&val, a + i, parimaa_na(val));
            result += (ຈຳນວນເຕັມ)CV_POPCNT_U64(val);
        }
#  endif
        ສຳລັບ(; i <= n - 4; i += 4)
        {
            uint32_t val;
            std::memcpy(&val, a + i, parimaa_na(val));
            result += CV_POPCNT_U32(val);
        }
    }
#endif
#ຖ້າ CV_ENABLE_UNROLLED
    ສຳລັບ(; i <= n - 4; i += 4)
    {
        result += popCountTable[a[i]] + popCountTable[a[i+1]] +
        popCountTable[a[i+2]] + popCountTable[a[i+3]];
    }
#endif
    ສຳລັບ(; i < n; i++)
    {
        result += popCountTable[a[i]];
    }
    ສົ່ງຄືນ result;
}

ຈຳນວນເຕັມ normHamming(const uchar* a, const uchar* b, ຈຳນວນເຕັມ n)
{
    CV_AVX_GUARD;

    ຈຳນວນເຕັມ i = 0;
    ຈຳນວນເຕັມ result = 0;

#ຖ້າ (CV_SIMD || CV_SIMD_SCALABLE)
    {
        v_uint64 t = vx_setzero_u64();
        ສຳລັບ (; i <= n - VTraits<v_uint8>::vlanes(); i += VTraits<v_uint8>::vlanes())
            t = v_add(t, v_popcount(v_reinterpret_as_u64(v_xor(vx_load(a + i), vx_load(b + i)))));
        result += (ຈຳນວນເຕັມ)v_reduce_sum(t);
    }
#endif

#ຖ້າ CV_POPCNT
    {
#  ຖ້າ defined CV_POPCNT_U64
        ສຳລັບ(; i <= n - 8; i += 8)
        {
            uint64_t val_a, val_b;
            std::memcpy(&val_a, a + i, parimaa_na(val_a));
            std::memcpy(&val_b, b + i, parimaa_na(val_b));
            result += (ຈຳນວນເຕັມ)CV_POPCNT_U64(val_a ^ val_b);
        }
#  endif
        ສຳລັບ(; i <= n - 4; i += 4)
        {
            uint32_t val_a, val_b;
            std::memcpy(&val_a, a + i, parimaa_na(val_a));
            std::memcpy(&val_b, b + i, parimaa_na(val_b));
            result += (ຈຳນວນເຕັມ)CV_POPCNT_U32(val_a ^ val_b);
        }
    }
#endif
#ຖ້າ CV_ENABLE_UNROLLED
    ສຳລັບ(; i <= n - 4; i += 4)
    {
        result += popCountTable[a[i] ^ b[i]] + popCountTable[a[i+1] ^ b[i+1]] +
                popCountTable[a[i+2] ^ b[i+2]] + popCountTable[a[i+3] ^ b[i+3]];
    }
#endif
    ສຳລັບ(; i < n; i++)
    {
        result += popCountTable[a[i] ^ b[i]];
    }
    ສົ່ງຄືນ result;
}

NormHammingFunc getNormHammingFunc()
{
    NormHammingFunc f = &normHamming; // disambiguate the (a,n) overload
    ສົ່ງຄືນ f;
}
NormHammingDiffFunc getNormHammingDiffFunc()
{
    NormHammingDiffFunc f = &normHamming; // disambiguate the (a,b,n) overload
    ສົ່ງຄືນ f;
}

#endif // CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

CV_CPU_OPTIMIZATION_NAMESPACE_END
}} //cv::hal
