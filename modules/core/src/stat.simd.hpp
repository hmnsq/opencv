// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.
//
// Copyright (C) 2026, Advanced Micro Devices, Inc., all rights reserved.

#ضمن "opencv2/core/hal/intrin.hpp"

نطاق cv { نطاق hal {

خارجي ثابت uchar چىقىرىش_سانى_Table[256];

تعريفنوع صحيح (*NormHammingFunc)(ثابت uchar*, صحيح);
تعريفنوع صحيح (*NormHammingDiffFunc)(ثابت uchar*, ثابت uchar*, صحيح);

CV_CPU_OPTIMIZATION_NAMESPACE_BEGIN

// forward declarations
صحيح normHamming(ثابت uchar* a, صحيح n);
صحيح normHamming(ثابت uchar* a, ثابت uchar* b, صحيح n);

NormHammingFunc getNormHammingFunc();
NormHammingDiffFunc getNormHammingDiffFunc();

#ifndef CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

#إذا CV_AVX2
ساكن مضمن صحيح _mm256_extract_epi32_(__m256i reg, ثابت صحيح i)
{
    CV_DECL_ALIGNED(32) صحيح reg_data[8];
    CV_DbgAssert(0 <= i && i < 8);
    _mm256_store_si256((__m256i*)reg_data, reg);
    أرجع reg_data[i];
}
#endif

صحيح normHamming(ثابت uchar* a, صحيح n)
{
    CV_AVX_GUARD;

    صحيح i = 0;
    صحيح result = 0;

#إذا (CV_SIMD || CV_SIMD_SCALABLE)
    {
        v_uint64 t = vx_setzero_u64();
        لكل (; i <= n - VTraits<v_uint8>::vlanes(); i += VTraits<v_uint8>::vlanes())
            t = v_add(t, v_popcount(v_reinterpret_as_u64(vx_load(a + i))));
        result = (صحيح)v_reduce_sum(t);
        vx_cleanup();
    }
#endif

#إذا CV_POPCNT
    {
#  إذا defined CV_POPCNT_U64
        لكل(; i <= n - 8; i += 8)
        {
            uint64_t val;
            std::memcpy(&val, a + i, حجم(val));
            result += (صحيح)CV_POPCNT_U64(val);
        }
#  endif
        لكل(; i <= n - 4; i += 4)
        {
            uint32_t val;
            std::memcpy(&val, a + i, حجم(val));
            result += CV_POPCNT_U32(val);
        }
    }
#endif
#إذا CV_ENABLE_UNROLLED
    لكل(; i <= n - 4; i += 4)
    {
        result += چىقىرىش_سانى_Table[a[i]] + چىقىرىش_سانى_Table[a[i+1]] +
        چىقىرىش_سانى_Table[a[i+2]] + چىقىرىش_سانى_Table[a[i+3]];
    }
#endif
    لكل(; i < n; i++)
    {
        result += چىقىرىش_سانى_Table[a[i]];
    }
    أرجع result;
}

صحيح normHamming(ثابت uchar* a, ثابت uchar* b, صحيح n)
{
    CV_AVX_GUARD;

    صحيح i = 0;
    صحيح result = 0;

#إذا (CV_SIMD || CV_SIMD_SCALABLE)
    {
        v_uint64 t = vx_setzero_u64();
        لكل (; i <= n - VTraits<v_uint8>::vlanes(); i += VTraits<v_uint8>::vlanes())
            t = v_add(t, v_popcount(v_reinterpret_as_u64(v_xor(vx_load(a + i), vx_load(b + i)))));
        result += (صحيح)v_reduce_sum(t);
    }
#endif

#إذا CV_POPCNT
    {
#  إذا defined CV_POPCNT_U64
        لكل(; i <= n - 8; i += 8)
        {
            uint64_t val_a, val_b;
            std::memcpy(&val_a, a + i, حجم(val_a));
            std::memcpy(&val_b, b + i, حجم(val_b));
            result += (صحيح)CV_POPCNT_U64(val_a ^ val_b);
        }
#  endif
        لكل(; i <= n - 4; i += 4)
        {
            uint32_t val_a, val_b;
            std::memcpy(&val_a, a + i, حجم(val_a));
            std::memcpy(&val_b, b + i, حجم(val_b));
            result += (صحيح)CV_POPCNT_U32(val_a ^ val_b);
        }
    }
#endif
#إذا CV_ENABLE_UNROLLED
    لكل(; i <= n - 4; i += 4)
    {
        result += چىقىرىش_سانى_Table[a[i] ^ b[i]] + چىقىرىش_سانى_Table[a[i+1] ^ b[i+1]] +
                چىقىرىش_سانى_Table[a[i+2] ^ b[i+2]] + چىقىرىش_سانى_Table[a[i+3] ^ b[i+3]];
    }
#endif
    لكل(; i < n; i++)
    {
        result += چىقىرىش_سانى_Table[a[i] ^ b[i]];
    }
    أرجع result;
}

NormHammingFunc getNormHammingFunc()
{
    NormHammingFunc f = &normHamming; // disambiguate the (a,n) overload
    أرجع f;
}
NormHammingDiffFunc getNormHammingDiffFunc()
{
    NormHammingDiffFunc f = &normHamming; // disambiguate the (a,b,n) overload
    أرجع f;
}

#endif // CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

CV_CPU_OPTIMIZATION_NAMESPACE_END
}} //cv::hal
