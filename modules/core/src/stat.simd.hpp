// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.
//
// Copyright (C) 2026, Advanced Micro Devices, Inc., all rights reserved.

#অন্তর্ভাব "opencv2/core/hal/intrin.hpp"

নামাকাশ cv { নামাকাশ hal {

বাহ্য ধ্রুব uchar বের_করো_গুনতি_Table[256];

প্রকারনির্দেশ পূর্ণাঙ্ক (*NormHammingFunc)(ধ্রুব uchar*, পূর্ণাঙ্ক);
প্রকারনির্দেশ পূর্ণাঙ্ক (*NormHammingDiffFunc)(ধ্রুব uchar*, ধ্রুব uchar*, পূর্ণাঙ্ক);

CV_CPU_OPTIMIZATION_NAMESPACE_BEGIN

// forward declarations
পূর্ণাঙ্ক normHamming(ধ্রুব uchar* a, পূর্ণাঙ্ক n);
পূর্ণাঙ্ক normHamming(ধ্রুব uchar* a, ধ্রুব uchar* b, পূর্ণাঙ্ক n);

NormHammingFunc getNormHammingFunc();
NormHammingDiffFunc getNormHammingDiffFunc();

#ifndef CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

#যদি CV_AVX2
স্থির অন্তঃস্থ পূর্ণাঙ্ক _mm256_extract_epi32_(__m256i reg, ধ্রুব পূর্ণাঙ্ক i)
{
    CV_DECL_ALIGNED(32) পূর্ণাঙ্ক reg_data[8];
    CV_DbgAssert(0 <= i && i < 8);
    _mm256_store_si256((__m256i*)reg_data, reg);
    প্রত্যাগম reg_data[i];
}
#endif

পূর্ণাঙ্ক normHamming(ধ্রুব uchar* a, পূর্ণাঙ্ক n)
{
    CV_AVX_GUARD;

    পূর্ণাঙ্ক i = 0;
    পূর্ণাঙ্ক result = 0;

#যদি (CV_SIMD || CV_SIMD_SCALABLE)
    {
        v_uint64 t = vx_setzero_u64();
        প্রতি (; i <= n - VTraits<v_uint8>::vlanes(); i += VTraits<v_uint8>::vlanes())
            t = v_add(t, v_popcount(v_reinterpret_as_u64(vx_load(a + i))));
        result = (পূর্ণাঙ্ক)v_reduce_sum(t);
        vx_cleanup();
    }
#endif

#যদি CV_POPCNT
    {
#  যদি defined CV_POPCNT_U64
        প্রতি(; i <= n - 8; i += 8)
        {
            uint64_t val;
            std::memcpy(&val, a + i, পরিমাণ(val));
            result += (পূর্ণাঙ্ক)CV_POPCNT_U64(val);
        }
#  endif
        প্রতি(; i <= n - 4; i += 4)
        {
            uint32_t val;
            std::memcpy(&val, a + i, পরিমাণ(val));
            result += CV_POPCNT_U32(val);
        }
    }
#endif
#যদি CV_ENABLE_UNROLLED
    প্রতি(; i <= n - 4; i += 4)
    {
        result += বের_করো_গুনতি_Table[a[i]] + বের_করো_গুনতি_Table[a[i+1]] +
        বের_করো_গুনতি_Table[a[i+2]] + বের_করো_গুনতি_Table[a[i+3]];
    }
#endif
    প্রতি(; i < n; i++)
    {
        result += বের_করো_গুনতি_Table[a[i]];
    }
    প্রত্যাগম result;
}

পূর্ণাঙ্ক normHamming(ধ্রুব uchar* a, ধ্রুব uchar* b, পূর্ণাঙ্ক n)
{
    CV_AVX_GUARD;

    পূর্ণাঙ্ক i = 0;
    পূর্ণাঙ্ক result = 0;

#যদি (CV_SIMD || CV_SIMD_SCALABLE)
    {
        v_uint64 t = vx_setzero_u64();
        প্রতি (; i <= n - VTraits<v_uint8>::vlanes(); i += VTraits<v_uint8>::vlanes())
            t = v_add(t, v_popcount(v_reinterpret_as_u64(v_xor(vx_load(a + i), vx_load(b + i)))));
        result += (পূর্ণাঙ্ক)v_reduce_sum(t);
    }
#endif

#যদি CV_POPCNT
    {
#  যদি defined CV_POPCNT_U64
        প্রতি(; i <= n - 8; i += 8)
        {
            uint64_t val_a, val_b;
            std::memcpy(&val_a, a + i, পরিমাণ(val_a));
            std::memcpy(&val_b, b + i, পরিমাণ(val_b));
            result += (পূর্ণাঙ্ক)CV_POPCNT_U64(val_a ^ val_b);
        }
#  endif
        প্রতি(; i <= n - 4; i += 4)
        {
            uint32_t val_a, val_b;
            std::memcpy(&val_a, a + i, পরিমাণ(val_a));
            std::memcpy(&val_b, b + i, পরিমাণ(val_b));
            result += (পূর্ণাঙ্ক)CV_POPCNT_U32(val_a ^ val_b);
        }
    }
#endif
#যদি CV_ENABLE_UNROLLED
    প্রতি(; i <= n - 4; i += 4)
    {
        result += বের_করো_গুনতি_Table[a[i] ^ b[i]] + বের_করো_গুনতি_Table[a[i+1] ^ b[i+1]] +
                বের_করো_গুনতি_Table[a[i+2] ^ b[i+2]] + বের_করো_গুনতি_Table[a[i+3] ^ b[i+3]];
    }
#endif
    প্রতি(; i < n; i++)
    {
        result += বের_করো_গুনতি_Table[a[i] ^ b[i]];
    }
    প্রত্যাগম result;
}

NormHammingFunc getNormHammingFunc()
{
    NormHammingFunc f = &normHamming; // disambiguate the (a,n) overload
    প্রত্যাগম f;
}
NormHammingDiffFunc getNormHammingDiffFunc()
{
    NormHammingDiffFunc f = &normHamming; // disambiguate the (a,b,n) overload
    প্রত্যাগম f;
}

#endif // CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

CV_CPU_OPTIMIZATION_NAMESPACE_END
}} //cv::hal
