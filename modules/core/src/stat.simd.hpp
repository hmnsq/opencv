// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.
//
// Copyright (C) 2026, Advanced Micro Devices, Inc., all rights reserved.

#ضمن "opencv2/core/hal/intrin.hpp"

نطاق cv { نطاق hal {

خارجي ثابت uchar ڪڍو_ڳڻپ_جدول[256];

تعريفنوع صحيح (*NormHammingFunc)(ثابت uchar*, صحيح);
تعريفنوع صحيح (*NormHammingDiffFunc)(ثابت uchar*, ثابت uchar*, صحيح);

CV_CPU_OPTIMIZATION_NAMESPACE_BEGIN

// forward declarations
صحيح normHamming(ثابت uchar* a, صحيح n);
صحيح normHamming(ثابت uchar* a, ثابت uchar* b, صحيح n);

NormHammingFunc getNormHammingFunc();
NormHammingDiffFunc getNormHammingDiffFunc();

#جيڪڏهن_وصف_نه_ٿيل CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

#إذا CV_AVX2
ساكن مضمن صحيح mm256_ڪڍو_epi32(__m256i reg, ثابت صحيح i)
{
    CV_DECL_ALIGNED(32) صحيح reg_معلومات[8];
    CV_DbgAssert(0 <= i && i < 8);
    _mm256_store_si256((__m256i*)reg_معلومات, reg);
    أرجع reg_معلومات[i];
}
#جيڪڏهن_پورو

صحيح normHamming(ثابت uchar* a, صحيح n)
{
    CV_AVX_GUARD;

    صحيح i = 0;
    صحيح نتيجو = 0;

#إذا (CV_SIMD || CV_SIMD_SCALABLE)
    {
        v_uint64 t = vx_setzero_u64();
        لكل (; i <= n - VTraits<v_uint8>::vlanes(); i += VTraits<v_uint8>::vlanes())
            t = v_جوڙيو(t, v_popcount(v_reinterpret_as_u64(vx_ڀريو(a + i))));
        نتيجو = (صحيح)v_reduce_جوڙ(t);
        vx_cleanup();
    }
#جيڪڏهن_پورو

#إذا CV_POPCNT
    {
#  إذا defined CV_POPCNT_U64
        لكل(; i <= n - 8; i += 8)
        {
            uint64_t val;
            std::يادگيري_نقل(&val, a + i, حجم(val));
            نتيجو += (صحيح)CV_POPCNT_U64(val);
        }
#  endif
        لكل(; i <= n - 4; i += 4)
        {
            uint32_t val;
            std::يادگيري_نقل(&val, a + i, حجم(val));
            نتيجو += CV_POPCNT_U32(val);
        }
    }
#جيڪڏهن_پورو
#إذا CV_ENABLE_UNROLLED
    لكل(; i <= n - 4; i += 4)
    {
        نتيجو += ڪڍو_ڳڻپ_جدول[a[i]] + ڪڍو_ڳڻپ_جدول[a[i+1]] +
        ڪڍو_ڳڻپ_جدول[a[i+2]] + ڪڍو_ڳڻپ_جدول[a[i+3]];
    }
#جيڪڏهن_پورو
    لكل(; i < n; i++)
    {
        نتيجو += ڪڍو_ڳڻپ_جدول[a[i]];
    }
    أرجع نتيجو;
}

صحيح normHamming(ثابت uchar* a, ثابت uchar* b, صحيح n)
{
    CV_AVX_GUARD;

    صحيح i = 0;
    صحيح نتيجو = 0;

#إذا (CV_SIMD || CV_SIMD_SCALABLE)
    {
        v_uint64 t = vx_setzero_u64();
        لكل (; i <= n - VTraits<v_uint8>::vlanes(); i += VTraits<v_uint8>::vlanes())
            t = v_جوڙيو(t, v_popcount(v_reinterpret_as_u64(v_xor(vx_ڀريو(a + i), vx_ڀريو(b + i)))));
        نتيجو += (صحيح)v_reduce_جوڙ(t);
    }
#جيڪڏهن_پورو

#إذا CV_POPCNT
    {
#  إذا defined CV_POPCNT_U64
        لكل(; i <= n - 8; i += 8)
        {
            uint64_t val_a, val_b;
            std::يادگيري_نقل(&val_a, a + i, حجم(val_a));
            std::يادگيري_نقل(&val_b, b + i, حجم(val_b));
            نتيجو += (صحيح)CV_POPCNT_U64(val_a ^ val_b);
        }
#  endif
        لكل(; i <= n - 4; i += 4)
        {
            uint32_t val_a, val_b;
            std::يادگيري_نقل(&val_a, a + i, حجم(val_a));
            std::يادگيري_نقل(&val_b, b + i, حجم(val_b));
            نتيجو += (صحيح)CV_POPCNT_U32(val_a ^ val_b);
        }
    }
#جيڪڏهن_پورو
#إذا CV_ENABLE_UNROLLED
    لكل(; i <= n - 4; i += 4)
    {
        نتيجو += ڪڍو_ڳڻپ_جدول[a[i] ^ b[i]] + ڪڍو_ڳڻپ_جدول[a[i+1] ^ b[i+1]] +
                ڪڍو_ڳڻپ_جدول[a[i+2] ^ b[i+2]] + ڪڍو_ڳڻپ_جدول[a[i+3] ^ b[i+3]];
    }
#جيڪڏهن_پورو
    لكل(; i < n; i++)
    {
        نتيجو += ڪڍو_ڳڻپ_جدول[a[i] ^ b[i]];
    }
    أرجع نتيجو;
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

#جيڪڏهن_پورو // CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

CV_CPU_OPTIMIZATION_NAMESPACE_پڇاڙي
}} //cv::hal
