// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.
//
// Copyright (C) 2026, Advanced Micro Devices, Inc., all rights reserved.

#અન્તર્ભાવ "opencv2/core/hal/intrin.hpp"

નામાકાશ cv { નામાકાશ hal {

બાહ્ય અચલ uchar કાઢો_ગણતરી_Table[256];

પ્રકારવ્યાખ્યા પૂર્ણાંક (*NormHammingFunc)(અચલ uchar*, પૂર્ણાંક);
પ્રકારવ્યાખ્યા પૂર્ણાંક (*NormHammingDiffFunc)(અચલ uchar*, અચલ uchar*, પૂર્ણાંક);

CV_CPU_OPTIMIZATION_NAMESPACE_BEGIN

// forward declarations
પૂર્ણાંક normHamming(અચલ uchar* a, પૂર્ણાંક n);
પૂર્ણાંક normHamming(અચલ uchar* a, અચલ uchar* b, પૂર્ણાંક n);

NormHammingFunc getNormHammingFunc();
NormHammingDiffFunc getNormHammingDiffFunc();

#ifndef CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

#જો CV_AVX2
સ્થિર અંતઃરેખ પૂર્ણાંક _mm256_extract_epi32_(__m256i reg, અચલ પૂર્ણાંક i)
{
    CV_DECL_ALIGNED(32) પૂર્ણાંક reg_data[8];
    CV_DbgAssert(0 <= i && i < 8);
    _mm256_store_si256((__m256i*)reg_data, reg);
    પાછા reg_data[i];
}
#endif

પૂર્ણાંક normHamming(અચલ uchar* a, પૂર્ણાંક n)
{
    CV_AVX_GUARD;

    પૂર્ણાંક i = 0;
    પૂર્ણાંક result = 0;

#જો (CV_SIMD || CV_SIMD_SCALABLE)
    {
        v_uint64 t = vx_setzero_u64();
        માટે (; i <= n - VTraits<v_uint8>::vlanes(); i += VTraits<v_uint8>::vlanes())
            t = v_add(t, v_popcount(v_reinterpret_as_u64(vx_load(a + i))));
        result = (પૂર્ણાંક)v_reduce_sum(t);
        vx_cleanup();
    }
#endif

#જો CV_POPCNT
    {
#  જો defined CV_POPCNT_U64
        માટે(; i <= n - 8; i += 8)
        {
            uint64_t val;
            std::memcpy(&val, a + i, કદ(val));
            result += (પૂર્ણાંક)CV_POPCNT_U64(val);
        }
#  endif
        માટે(; i <= n - 4; i += 4)
        {
            uint32_t val;
            std::memcpy(&val, a + i, કદ(val));
            result += CV_POPCNT_U32(val);
        }
    }
#endif
#જો CV_ENABLE_UNROLLED
    માટે(; i <= n - 4; i += 4)
    {
        result += કાઢો_ગણતરી_Table[a[i]] + કાઢો_ગણતરી_Table[a[i+1]] +
        કાઢો_ગણતરી_Table[a[i+2]] + કાઢો_ગણતરી_Table[a[i+3]];
    }
#endif
    માટે(; i < n; i++)
    {
        result += કાઢો_ગણતરી_Table[a[i]];
    }
    પાછા result;
}

પૂર્ણાંક normHamming(અચલ uchar* a, અચલ uchar* b, પૂર્ણાંક n)
{
    CV_AVX_GUARD;

    પૂર્ણાંક i = 0;
    પૂર્ણાંક result = 0;

#જો (CV_SIMD || CV_SIMD_SCALABLE)
    {
        v_uint64 t = vx_setzero_u64();
        માટે (; i <= n - VTraits<v_uint8>::vlanes(); i += VTraits<v_uint8>::vlanes())
            t = v_add(t, v_popcount(v_reinterpret_as_u64(v_xor(vx_load(a + i), vx_load(b + i)))));
        result += (પૂર્ણાંક)v_reduce_sum(t);
    }
#endif

#જો CV_POPCNT
    {
#  જો defined CV_POPCNT_U64
        માટે(; i <= n - 8; i += 8)
        {
            uint64_t val_a, val_b;
            std::memcpy(&val_a, a + i, કદ(val_a));
            std::memcpy(&val_b, b + i, કદ(val_b));
            result += (પૂર્ણાંક)CV_POPCNT_U64(val_a ^ val_b);
        }
#  endif
        માટે(; i <= n - 4; i += 4)
        {
            uint32_t val_a, val_b;
            std::memcpy(&val_a, a + i, કદ(val_a));
            std::memcpy(&val_b, b + i, કદ(val_b));
            result += (પૂર્ણાંક)CV_POPCNT_U32(val_a ^ val_b);
        }
    }
#endif
#જો CV_ENABLE_UNROLLED
    માટે(; i <= n - 4; i += 4)
    {
        result += કાઢો_ગણતરી_Table[a[i] ^ b[i]] + કાઢો_ગણતરી_Table[a[i+1] ^ b[i+1]] +
                કાઢો_ગણતરી_Table[a[i+2] ^ b[i+2]] + કાઢો_ગણતરી_Table[a[i+3] ^ b[i+3]];
    }
#endif
    માટે(; i < n; i++)
    {
        result += કાઢો_ગણતરી_Table[a[i] ^ b[i]];
    }
    પાછા result;
}

NormHammingFunc getNormHammingFunc()
{
    NormHammingFunc f = &normHamming; // disambiguate the (a,n) overload
    પાછા f;
}
NormHammingDiffFunc getNormHammingDiffFunc()
{
    NormHammingDiffFunc f = &normHamming; // disambiguate the (a,b,n) overload
    પાછા f;
}

#endif // CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

CV_CPU_OPTIMIZATION_NAMESPACE_END
}} //cv::hal
