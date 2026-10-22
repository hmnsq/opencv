// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.
//
// Copyright (C) 2026, Advanced Micro Devices, Inc., all rights reserved.

#شامل "opencv2/core/hal/intrin.hpp"

فضاینام cv { فضاینام hal {

بیرونی ثابت uchar برداشت_شمار_Table[256];

تعریفنوع صحیح (*NormHammingFunc)(ثابت uchar*, صحیح);
تعریفنوع صحیح (*NormHammingDiffFunc)(ثابت uchar*, ثابت uchar*, صحیح);

CV_CPU_OPTIMIZATION_NAMESPACE_BEGIN

// forward declarations
صحیح normHamming(ثابت uchar* a, صحیح n);
صحیح normHamming(ثابت uchar* a, ثابت uchar* b, صحیح n);

NormHammingFunc getNormHammingFunc();
NormHammingDiffFunc getNormHammingDiffFunc();

#ifndef CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

#اگر CV_AVX2
ایستا درونخطی صحیح _mm256_extract_epi32_(__m256i reg, ثابت صحیح i)
{
    CV_DECL_ALIGNED(32) صحیح reg_data[8];
    CV_DbgAssert(0 <= i && i < 8);
    _mm256_store_si256((__m256i*)reg_data, reg);
    بازگشت reg_data[i];
}
#endif

صحیح normHamming(ثابت uchar* a, صحیح n)
{
    CV_AVX_GUARD;

    صحیح i = 0;
    صحیح result = 0;

#اگر (CV_SIMD || CV_SIMD_SCALABLE)
    {
        v_uint64 t = vx_setzero_u64();
        برای (; i <= n - VTraits<v_uint8>::vlanes(); i += VTraits<v_uint8>::vlanes())
            t = v_add(t, v_popcount(v_reinterpret_as_u64(vx_load(a + i))));
        result = (صحیح)v_reduce_sum(t);
        vx_cleanup();
    }
#endif

#اگر CV_POPCNT
    {
#  اگر defined CV_POPCNT_U64
        برای(; i <= n - 8; i += 8)
        {
            uint64_t val;
            std::memcpy(&val, a + i, اندازه(val));
            result += (صحیح)CV_POPCNT_U64(val);
        }
#  endif
        برای(; i <= n - 4; i += 4)
        {
            uint32_t val;
            std::memcpy(&val, a + i, اندازه(val));
            result += CV_POPCNT_U32(val);
        }
    }
#endif
#اگر CV_ENABLE_UNROLLED
    برای(; i <= n - 4; i += 4)
    {
        result += برداشت_شمار_Table[a[i]] + برداشت_شمار_Table[a[i+1]] +
        برداشت_شمار_Table[a[i+2]] + برداشت_شمار_Table[a[i+3]];
    }
#endif
    برای(; i < n; i++)
    {
        result += برداشت_شمار_Table[a[i]];
    }
    بازگشت result;
}

صحیح normHamming(ثابت uchar* a, ثابت uchar* b, صحیح n)
{
    CV_AVX_GUARD;

    صحیح i = 0;
    صحیح result = 0;

#اگر (CV_SIMD || CV_SIMD_SCALABLE)
    {
        v_uint64 t = vx_setzero_u64();
        برای (; i <= n - VTraits<v_uint8>::vlanes(); i += VTraits<v_uint8>::vlanes())
            t = v_add(t, v_popcount(v_reinterpret_as_u64(v_xor(vx_load(a + i), vx_load(b + i)))));
        result += (صحیح)v_reduce_sum(t);
    }
#endif

#اگر CV_POPCNT
    {
#  اگر defined CV_POPCNT_U64
        برای(; i <= n - 8; i += 8)
        {
            uint64_t val_a, val_b;
            std::memcpy(&val_a, a + i, اندازه(val_a));
            std::memcpy(&val_b, b + i, اندازه(val_b));
            result += (صحیح)CV_POPCNT_U64(val_a ^ val_b);
        }
#  endif
        برای(; i <= n - 4; i += 4)
        {
            uint32_t val_a, val_b;
            std::memcpy(&val_a, a + i, اندازه(val_a));
            std::memcpy(&val_b, b + i, اندازه(val_b));
            result += (صحیح)CV_POPCNT_U32(val_a ^ val_b);
        }
    }
#endif
#اگر CV_ENABLE_UNROLLED
    برای(; i <= n - 4; i += 4)
    {
        result += برداشت_شمار_Table[a[i] ^ b[i]] + برداشت_شمار_Table[a[i+1] ^ b[i+1]] +
                برداشت_شمار_Table[a[i+2] ^ b[i+2]] + برداشت_شمار_Table[a[i+3] ^ b[i+3]];
    }
#endif
    برای(; i < n; i++)
    {
        result += برداشت_شمار_Table[a[i] ^ b[i]];
    }
    بازگشت result;
}

NormHammingFunc getNormHammingFunc()
{
    NormHammingFunc f = &normHamming; // disambiguate the (a,n) overload
    بازگشت f;
}
NormHammingDiffFunc getNormHammingDiffFunc()
{
    NormHammingDiffFunc f = &normHamming; // disambiguate the (a,b,n) overload
    بازگشت f;
}

#endif // CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

CV_CPU_OPTIMIZATION_NAMESPACE_END
}} //cv::hal
