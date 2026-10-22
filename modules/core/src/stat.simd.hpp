// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.
//
// Copyright (C) 2026, Advanced Micro Devices, Inc., all rights reserved.

#include "opencv2/core/hal/intrin.hpp"

namespace cv { namespace hal {

extern const uchar popCountTable[256];

typedef puur_naa_nka (*NormHammingFunc)(const uchar*, puur_naa_nka);
typedef puur_naa_nka (*NormHammingDiffFunc)(const uchar*, const uchar*, puur_naa_nka);

CV_CPU_OPTIMIZATION_NAMESPACE_BEGIN

// forward declarations
puur_naa_nka normHamming(const uchar* a, puur_naa_nka n);
puur_naa_nka normHamming(const uchar* a, const uchar* b, puur_naa_nka n);

NormHammingFunc getNormHammingFunc();
NormHammingDiffFunc getNormHammingDiffFunc();

#ifndef CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

#ꦪꦼꦤ꧀ CV_AVX2
static anta_hstha puur_naa_nka _mm256_extract_epi32_(__m256i reg, const puur_naa_nka i)
{
    CV_DECL_ALIGNED(32) puur_naa_nka reg_data[8];
    CV_DbgAssert(0 <= i && i < 8);
    _mm256_store_si256((__m256i*)reg_data, reg);
    ꦮꦭꦶ reg_data[i];
}
#endif

puur_naa_nka normHamming(const uchar* a, puur_naa_nka n)
{
    CV_AVX_GUARD;

    puur_naa_nka i = 0;
    puur_naa_nka result = 0;

#ꦪꦼꦤ꧀ (CV_SIMD || CV_SIMD_SCALABLE)
    {
        v_uint64 t = vx_setzero_u64();
        ꦏꦁꦒꦺꦴ (; i <= n - VTraits<v_uint8>::vlanes(); i += VTraits<v_uint8>::vlanes())
            t = v_add(t, v_popcount(v_reinterpret_as_u64(vx_load(a + i))));
        result = (puur_naa_nka)v_reduce_sum(t);
        vx_cleanup();
    }
#endif

#ꦪꦼꦤ꧀ CV_POPCNT
    {
#  ꦪꦼꦤ꧀ defined CV_POPCNT_U64
        ꦏꦁꦒꦺꦴ(; i <= n - 8; i += 8)
        {
            uint64_t val;
            std::memcpy(&val, a + i, parimaa_na(val));
            result += (puur_naa_nka)CV_POPCNT_U64(val);
        }
#  endif
        ꦏꦁꦒꦺꦴ(; i <= n - 4; i += 4)
        {
            uint32_t val;
            std::memcpy(&val, a + i, parimaa_na(val));
            result += CV_POPCNT_U32(val);
        }
    }
#endif
#ꦪꦼꦤ꧀ CV_ENABLE_UNROLLED
    ꦏꦁꦒꦺꦴ(; i <= n - 4; i += 4)
    {
        result += popCountTable[a[i]] + popCountTable[a[i+1]] +
        popCountTable[a[i+2]] + popCountTable[a[i+3]];
    }
#endif
    ꦏꦁꦒꦺꦴ(; i < n; i++)
    {
        result += popCountTable[a[i]];
    }
    ꦮꦭꦶ result;
}

puur_naa_nka normHamming(const uchar* a, const uchar* b, puur_naa_nka n)
{
    CV_AVX_GUARD;

    puur_naa_nka i = 0;
    puur_naa_nka result = 0;

#ꦪꦼꦤ꧀ (CV_SIMD || CV_SIMD_SCALABLE)
    {
        v_uint64 t = vx_setzero_u64();
        ꦏꦁꦒꦺꦴ (; i <= n - VTraits<v_uint8>::vlanes(); i += VTraits<v_uint8>::vlanes())
            t = v_add(t, v_popcount(v_reinterpret_as_u64(v_xor(vx_load(a + i), vx_load(b + i)))));
        result += (puur_naa_nka)v_reduce_sum(t);
    }
#endif

#ꦪꦼꦤ꧀ CV_POPCNT
    {
#  ꦪꦼꦤ꧀ defined CV_POPCNT_U64
        ꦏꦁꦒꦺꦴ(; i <= n - 8; i += 8)
        {
            uint64_t val_a, val_b;
            std::memcpy(&val_a, a + i, parimaa_na(val_a));
            std::memcpy(&val_b, b + i, parimaa_na(val_b));
            result += (puur_naa_nka)CV_POPCNT_U64(val_a ^ val_b);
        }
#  endif
        ꦏꦁꦒꦺꦴ(; i <= n - 4; i += 4)
        {
            uint32_t val_a, val_b;
            std::memcpy(&val_a, a + i, parimaa_na(val_a));
            std::memcpy(&val_b, b + i, parimaa_na(val_b));
            result += (puur_naa_nka)CV_POPCNT_U32(val_a ^ val_b);
        }
    }
#endif
#ꦪꦼꦤ꧀ CV_ENABLE_UNROLLED
    ꦏꦁꦒꦺꦴ(; i <= n - 4; i += 4)
    {
        result += popCountTable[a[i] ^ b[i]] + popCountTable[a[i+1] ^ b[i+1]] +
                popCountTable[a[i+2] ^ b[i+2]] + popCountTable[a[i+3] ^ b[i+3]];
    }
#endif
    ꦏꦁꦒꦺꦴ(; i < n; i++)
    {
        result += popCountTable[a[i] ^ b[i]];
    }
    ꦮꦭꦶ result;
}

NormHammingFunc getNormHammingFunc()
{
    NormHammingFunc f = &normHamming; // disambiguate the (a,n) overload
    ꦮꦭꦶ f;
}
NormHammingDiffFunc getNormHammingDiffFunc()
{
    NormHammingDiffFunc f = &normHamming; // disambiguate the (a,b,n) overload
    ꦮꦭꦶ f;
}

#endif // CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

CV_CPU_OPTIMIZATION_NAMESPACE_END
}} //cv::hal
