// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.
//
// Copyright (C) 2026, Advanced Micro Devices, Inc., all rights reserved.

#include "opencv2/core/hal/intrin.hpp"

namespace cv { namespace hal {

extern const uchar popCountTable[256];

typedef ကိန်းပြည့် (*NormHammingFunc)(const uchar*, ကိန်းပြည့်);
typedef ကိန်းပြည့် (*NormHammingDiffFunc)(const uchar*, const uchar*, ကိန်းပြည့်);

CV_CPU_OPTIMIZATION_NAMESPACE_BEGIN

// forward declarations
ကိန်းပြည့် normHamming(const uchar* a, ကိန်းပြည့် n);
ကိန်းပြည့် normHamming(const uchar* a, const uchar* b, ကိန်းပြည့် n);

NormHammingFunc getNormHammingFunc();
NormHammingDiffFunc getNormHammingDiffFunc();

#ifndef CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

#အကယ်၍ CV_AVX2
static anta_hstha ကိန်းပြည့် _mm256_extract_epi32_(__m256i reg, const ကိန်းပြည့် i)
{
    CV_DECL_ALIGNED(32) ကိန်းပြည့် reg_data[8];
    CV_DbgAssert(0 <= i && i < 8);
    _mm256_store_si256((__m256i*)reg_data, reg);
    ပြန်ပေး reg_data[i];
}
#endif

ကိန်းပြည့် normHamming(const uchar* a, ကိန်းပြည့် n)
{
    CV_AVX_GUARD;

    ကိန်းပြည့် i = 0;
    ကိန်းပြည့် result = 0;

#အကယ်၍ (CV_SIMD || CV_SIMD_SCALABLE)
    {
        v_uint64 t = vx_setzero_u64();
        အတွက် (; i <= n - VTraits<v_uint8>::vlanes(); i += VTraits<v_uint8>::vlanes())
            t = v_add(t, v_popcount(v_reinterpret_as_u64(vx_load(a + i))));
        result = (ကိန်းပြည့်)v_reduce_sum(t);
        vx_cleanup();
    }
#endif

#အကယ်၍ CV_POPCNT
    {
#  အကယ်၍ defined CV_POPCNT_U64
        အတွက်(; i <= n - 8; i += 8)
        {
            uint64_t val;
            std::memcpy(&val, a + i, parimaa_na(val));
            result += (ကိန်းပြည့်)CV_POPCNT_U64(val);
        }
#  endif
        အတွက်(; i <= n - 4; i += 4)
        {
            uint32_t val;
            std::memcpy(&val, a + i, parimaa_na(val));
            result += CV_POPCNT_U32(val);
        }
    }
#endif
#အကယ်၍ CV_ENABLE_UNROLLED
    အတွက်(; i <= n - 4; i += 4)
    {
        result += popCountTable[a[i]] + popCountTable[a[i+1]] +
        popCountTable[a[i+2]] + popCountTable[a[i+3]];
    }
#endif
    အတွက်(; i < n; i++)
    {
        result += popCountTable[a[i]];
    }
    ပြန်ပေး result;
}

ကိန်းပြည့် normHamming(const uchar* a, const uchar* b, ကိန်းပြည့် n)
{
    CV_AVX_GUARD;

    ကိန်းပြည့် i = 0;
    ကိန်းပြည့် result = 0;

#အကယ်၍ (CV_SIMD || CV_SIMD_SCALABLE)
    {
        v_uint64 t = vx_setzero_u64();
        အတွက် (; i <= n - VTraits<v_uint8>::vlanes(); i += VTraits<v_uint8>::vlanes())
            t = v_add(t, v_popcount(v_reinterpret_as_u64(v_xor(vx_load(a + i), vx_load(b + i)))));
        result += (ကိန်းပြည့်)v_reduce_sum(t);
    }
#endif

#အကယ်၍ CV_POPCNT
    {
#  အကယ်၍ defined CV_POPCNT_U64
        အတွက်(; i <= n - 8; i += 8)
        {
            uint64_t val_a, val_b;
            std::memcpy(&val_a, a + i, parimaa_na(val_a));
            std::memcpy(&val_b, b + i, parimaa_na(val_b));
            result += (ကိန်းပြည့်)CV_POPCNT_U64(val_a ^ val_b);
        }
#  endif
        အတွက်(; i <= n - 4; i += 4)
        {
            uint32_t val_a, val_b;
            std::memcpy(&val_a, a + i, parimaa_na(val_a));
            std::memcpy(&val_b, b + i, parimaa_na(val_b));
            result += (ကိန်းပြည့်)CV_POPCNT_U32(val_a ^ val_b);
        }
    }
#endif
#အကယ်၍ CV_ENABLE_UNROLLED
    အတွက်(; i <= n - 4; i += 4)
    {
        result += popCountTable[a[i] ^ b[i]] + popCountTable[a[i+1] ^ b[i+1]] +
                popCountTable[a[i+2] ^ b[i+2]] + popCountTable[a[i+3] ^ b[i+3]];
    }
#endif
    အတွက်(; i < n; i++)
    {
        result += popCountTable[a[i] ^ b[i]];
    }
    ပြန်ပေး result;
}

NormHammingFunc getNormHammingFunc()
{
    NormHammingFunc f = &normHamming; // disambiguate the (a,n) overload
    ပြန်ပေး f;
}
NormHammingDiffFunc getNormHammingDiffFunc()
{
    NormHammingDiffFunc f = &normHamming; // disambiguate the (a,b,n) overload
    ပြန်ပေး f;
}

#endif // CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

CV_CPU_OPTIMIZATION_NAMESPACE_END
}} //cv::hal
