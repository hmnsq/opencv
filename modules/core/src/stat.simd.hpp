// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.
//
// Copyright (C) 2026, Advanced Micro Devices, Inc., all rights reserved.

#অন্তর্ভাব "opencv2/core/hal/intrin.hpp"

নামাকাশ cv { নামাকাশ hal {

বাহ্যিক ধ্রুবক uchar বের_করো_গুনতি_Table[256];

প্রকারলেখ পূর্ণসংখ্যা (*NormHammingFunc)(ধ্রুবক uchar*, পূর্ণসংখ্যা);
প্রকারলেখ পূর্ণসংখ্যা (*NormHammingDiffFunc)(ধ্রুবক uchar*, ধ্রুবক uchar*, পূর্ণসংখ্যা);

CV_CPU_OPTIMIZATION_NAMESPACE_BEGIN

// forward declarations
পূর্ণসংখ্যা normHamming(ধ্রুবক uchar* a, পূর্ণসংখ্যা n);
পূর্ণসংখ্যা normHamming(ধ্রুবক uchar* a, ধ্রুবক uchar* b, পূর্ণসংখ্যা n);

NormHammingFunc getNormHammingFunc();
NormHammingDiffFunc getNormHammingDiffFunc();

#ifndef CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

#যদি CV_AVX2
স্থির অন্তঃরেখ পূর্ণসংখ্যা _mm256_extract_epi32_(__m256i reg, ধ্রুবক পূর্ণসংখ্যা i)
{
    CV_DECL_ALIGNED(32) পূর্ণসংখ্যা reg_তথ্য[8];
    CV_DbgAssert(0 <= i && i < 8);
    _mm256_store_si256((__m256i*)reg_তথ্য, reg);
    ফেরত reg_তথ্য[i];
}
#endif

পূর্ণসংখ্যা normHamming(ধ্রুবক uchar* a, পূর্ণসংখ্যা n)
{
    CV_AVX_GUARD;

    পূর্ণসংখ্যা i = 0;
    পূর্ণসংখ্যা ফলাফল = 0;

#যদি (CV_SIMD || CV_SIMD_SCALABLE)
    {
        v_uint64 t = vx_setzero_u64();
        জন্য (; i <= n - VTraits<v_uint8>::vlanes(); i += VTraits<v_uint8>::vlanes())
            t = v_add(t, v_popcount(v_reinterpret_as_u64(vx_load(a + i))));
        ফলাফল = (পূর্ণসংখ্যা)v_reduce_যোগ(t);
        vx_cleanup();
    }
#endif

#যদি CV_POPCNT
    {
#  যদি defined CV_POPCNT_U64
        জন্য(; i <= n - 8; i += 8)
        {
            uint64_t val;
            std::memcpy(&val, a + i, আকার(val));
            ফলাফল += (পূর্ণসংখ্যা)CV_POPCNT_U64(val);
        }
#  endif
        জন্য(; i <= n - 4; i += 4)
        {
            uint32_t val;
            std::memcpy(&val, a + i, আকার(val));
            ফলাফল += CV_POPCNT_U32(val);
        }
    }
#endif
#যদি CV_ENABLE_UNROLLED
    জন্য(; i <= n - 4; i += 4)
    {
        ফলাফল += বের_করো_গুনতি_Table[a[i]] + বের_করো_গুনতি_Table[a[i+1]] +
        বের_করো_গুনতি_Table[a[i+2]] + বের_করো_গুনতি_Table[a[i+3]];
    }
#endif
    জন্য(; i < n; i++)
    {
        ফলাফল += বের_করো_গুনতি_Table[a[i]];
    }
    ফেরত ফলাফল;
}

পূর্ণসংখ্যা normHamming(ধ্রুবক uchar* a, ধ্রুবক uchar* b, পূর্ণসংখ্যা n)
{
    CV_AVX_GUARD;

    পূর্ণসংখ্যা i = 0;
    পূর্ণসংখ্যা ফলাফল = 0;

#যদি (CV_SIMD || CV_SIMD_SCALABLE)
    {
        v_uint64 t = vx_setzero_u64();
        জন্য (; i <= n - VTraits<v_uint8>::vlanes(); i += VTraits<v_uint8>::vlanes())
            t = v_add(t, v_popcount(v_reinterpret_as_u64(v_xor(vx_load(a + i), vx_load(b + i)))));
        ফলাফল += (পূর্ণসংখ্যা)v_reduce_যোগ(t);
    }
#endif

#যদি CV_POPCNT
    {
#  যদি defined CV_POPCNT_U64
        জন্য(; i <= n - 8; i += 8)
        {
            uint64_t val_a, val_b;
            std::memcpy(&val_a, a + i, আকার(val_a));
            std::memcpy(&val_b, b + i, আকার(val_b));
            ফলাফল += (পূর্ণসংখ্যা)CV_POPCNT_U64(val_a ^ val_b);
        }
#  endif
        জন্য(; i <= n - 4; i += 4)
        {
            uint32_t val_a, val_b;
            std::memcpy(&val_a, a + i, আকার(val_a));
            std::memcpy(&val_b, b + i, আকার(val_b));
            ফলাফল += (পূর্ণসংখ্যা)CV_POPCNT_U32(val_a ^ val_b);
        }
    }
#endif
#যদি CV_ENABLE_UNROLLED
    জন্য(; i <= n - 4; i += 4)
    {
        ফলাফল += বের_করো_গুনতি_Table[a[i] ^ b[i]] + বের_করো_গুনতি_Table[a[i+1] ^ b[i+1]] +
                বের_করো_গুনতি_Table[a[i+2] ^ b[i+2]] + বের_করো_গুনতি_Table[a[i+3] ^ b[i+3]];
    }
#endif
    জন্য(; i < n; i++)
    {
        ফলাফল += বের_করো_গুনতি_Table[a[i] ^ b[i]];
    }
    ফেরত ফলাফল;
}

NormHammingFunc getNormHammingFunc()
{
    NormHammingFunc f = &normHamming; // disambiguate the (a,n) overload
    ফেরত f;
}
NormHammingDiffFunc getNormHammingDiffFunc()
{
    NormHammingDiffFunc f = &normHamming; // disambiguate the (a,b,n) overload
    ফেরত f;
}

#endif // CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

CV_CPU_OPTIMIZATION_NAMESPACE_END
}} //cv::hal
