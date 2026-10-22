// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.
//
// Copyright (C) 2026, Advanced Micro Devices, Inc., all rights reserved.

#include "opencv2/core/hal/intrin.hpp"

namespace cv { namespace hal {

extern const uchar popCountTable[256];

typedef จำนวนเต็ม (*NormHammingFunc)(const uchar*, จำนวนเต็ม);
typedef จำนวนเต็ม (*NormHammingDiffFunc)(const uchar*, const uchar*, จำนวนเต็ม);

CV_CPU_OPTIMIZATION_NAMESPACE_BEGIN

// forward declarations
จำนวนเต็ม normHamming(const uchar* a, จำนวนเต็ม n);
จำนวนเต็ม normHamming(const uchar* a, const uchar* b, จำนวนเต็ม n);

NormHammingFunc getNormHammingFunc();
NormHammingDiffFunc getNormHammingDiffFunc();

#ifndef CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

#ถ้า CV_AVX2
static anta_hstha จำนวนเต็ม _mm256_extract_epi32_(__m256i reg, const จำนวนเต็ม i)
{
    CV_DECL_ALIGNED(32) จำนวนเต็ม reg_data[8];
    CV_DbgAssert(0 <= i && i < 8);
    _mm256_store_si256((__m256i*)reg_data, reg);
    คืน reg_data[i];
}
#endif

จำนวนเต็ม normHamming(const uchar* a, จำนวนเต็ม n)
{
    CV_AVX_GUARD;

    จำนวนเต็ม i = 0;
    จำนวนเต็ม result = 0;

#ถ้า (CV_SIMD || CV_SIMD_SCALABLE)
    {
        v_uint64 t = vx_setzero_u64();
        สำหรับ (; i <= n - VTraits<v_uint8>::vlanes(); i += VTraits<v_uint8>::vlanes())
            t = v_add(t, v_popcount(v_reinterpret_as_u64(vx_load(a + i))));
        result = (จำนวนเต็ม)v_reduce_sum(t);
        vx_cleanup();
    }
#endif

#ถ้า CV_POPCNT
    {
#  ถ้า defined CV_POPCNT_U64
        สำหรับ(; i <= n - 8; i += 8)
        {
            uint64_t val;
            std::memcpy(&val, a + i, parimaa_na(val));
            result += (จำนวนเต็ม)CV_POPCNT_U64(val);
        }
#  endif
        สำหรับ(; i <= n - 4; i += 4)
        {
            uint32_t val;
            std::memcpy(&val, a + i, parimaa_na(val));
            result += CV_POPCNT_U32(val);
        }
    }
#endif
#ถ้า CV_ENABLE_UNROLLED
    สำหรับ(; i <= n - 4; i += 4)
    {
        result += popCountTable[a[i]] + popCountTable[a[i+1]] +
        popCountTable[a[i+2]] + popCountTable[a[i+3]];
    }
#endif
    สำหรับ(; i < n; i++)
    {
        result += popCountTable[a[i]];
    }
    คืน result;
}

จำนวนเต็ม normHamming(const uchar* a, const uchar* b, จำนวนเต็ม n)
{
    CV_AVX_GUARD;

    จำนวนเต็ม i = 0;
    จำนวนเต็ม result = 0;

#ถ้า (CV_SIMD || CV_SIMD_SCALABLE)
    {
        v_uint64 t = vx_setzero_u64();
        สำหรับ (; i <= n - VTraits<v_uint8>::vlanes(); i += VTraits<v_uint8>::vlanes())
            t = v_add(t, v_popcount(v_reinterpret_as_u64(v_xor(vx_load(a + i), vx_load(b + i)))));
        result += (จำนวนเต็ม)v_reduce_sum(t);
    }
#endif

#ถ้า CV_POPCNT
    {
#  ถ้า defined CV_POPCNT_U64
        สำหรับ(; i <= n - 8; i += 8)
        {
            uint64_t val_a, val_b;
            std::memcpy(&val_a, a + i, parimaa_na(val_a));
            std::memcpy(&val_b, b + i, parimaa_na(val_b));
            result += (จำนวนเต็ม)CV_POPCNT_U64(val_a ^ val_b);
        }
#  endif
        สำหรับ(; i <= n - 4; i += 4)
        {
            uint32_t val_a, val_b;
            std::memcpy(&val_a, a + i, parimaa_na(val_a));
            std::memcpy(&val_b, b + i, parimaa_na(val_b));
            result += (จำนวนเต็ม)CV_POPCNT_U32(val_a ^ val_b);
        }
    }
#endif
#ถ้า CV_ENABLE_UNROLLED
    สำหรับ(; i <= n - 4; i += 4)
    {
        result += popCountTable[a[i] ^ b[i]] + popCountTable[a[i+1] ^ b[i+1]] +
                popCountTable[a[i+2] ^ b[i+2]] + popCountTable[a[i+3] ^ b[i+3]];
    }
#endif
    สำหรับ(; i < n; i++)
    {
        result += popCountTable[a[i] ^ b[i]];
    }
    คืน result;
}

NormHammingFunc getNormHammingFunc()
{
    NormHammingFunc f = &normHamming; // disambiguate the (a,n) overload
    คืน f;
}
NormHammingDiffFunc getNormHammingDiffFunc()
{
    NormHammingDiffFunc f = &normHamming; // disambiguate the (a,b,n) overload
    คืน f;
}

#endif // CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

CV_CPU_OPTIMIZATION_NAMESPACE_END
}} //cv::hal
