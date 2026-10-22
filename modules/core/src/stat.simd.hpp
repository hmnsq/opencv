// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.
//
// Copyright (C) 2026, Advanced Micro Devices, Inc., all rights reserved.

#ضمن "opencv2/core/hal/intrin.hpp"

نطاق cv { نطاق hal {

خارجي ثابت uchar برداشت_شمار_جدول[256];

تعريفنوع صحيح (*NormHammingFunc)(ثابت uchar*, صحيح);
تعريفنوع صحيح (*NormHammingDiffFunc)(ثابت uchar*, ثابت uchar*, صحيح);

CV_CPU_OPTIMIZATION_NAMESPACE_BEGIN

// forward declarations
صحيح normHamming(ثابت uchar* a, صحيح n);
صحيح normHamming(ثابت uchar* a, ثابت uchar* b, صحيح n);

NormHammingFunc getNormHammingFunc();
NormHammingDiffFunc getNormHammingDiffFunc();

#اگر_تعریف_نشده CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

#إذا CV_AVX2
ساكن مضمن صحيح mm256_استخراج_epi32(__m256i reg, ثابت صحيح i)
{
    CV_DECL_ALIGNED(32) صحيح reg_داده[8];
    CV_DbgAssert(0 <= i && i < 8);
    _mm256_store_si256((__m256i*)reg_داده, reg);
    أرجع reg_داده[i];
}
#پایان_اگر

صحيح normHamming(ثابت uchar* a, صحيح n)
{
    CV_AVX_GUARD;

    صحيح i = 0;
    صحيح نتیجه = 0;

#إذا (CV_SIMD || CV_SIMD_SCALABLE)
    {
        v_uint64 t = vx_setzero_u64();
        لكل (; i <= n - VTraits<v_uint8>::vlanes(); i += VTraits<v_uint8>::vlanes())
            t = v_جمع_کن(t, v_popcount(v_reinterpret_as_u64(vx_بارگیری(a + i))));
        نتیجه = (صحيح)v_reduce_جمع(t);
        vx_cleanup();
    }
#پایان_اگر

#إذا CV_POPCNT
    {
#  إذا defined CV_POPCNT_U64
        لكل(; i <= n - 8; i += 8)
        {
            uint64_t val;
            std::کپی_حافظه(&val, a + i, حجم(val));
            نتیجه += (صحيح)CV_POPCNT_U64(val);
        }
#  endif
        لكل(; i <= n - 4; i += 4)
        {
            uint32_t val;
            std::کپی_حافظه(&val, a + i, حجم(val));
            نتیجه += CV_POPCNT_U32(val);
        }
    }
#پایان_اگر
#إذا CV_ENABLE_UNROLLED
    لكل(; i <= n - 4; i += 4)
    {
        نتیجه += برداشت_شمار_جدول[a[i]] + برداشت_شمار_جدول[a[i+1]] +
        برداشت_شمار_جدول[a[i+2]] + برداشت_شمار_جدول[a[i+3]];
    }
#پایان_اگر
    لكل(; i < n; i++)
    {
        نتیجه += برداشت_شمار_جدول[a[i]];
    }
    أرجع نتیجه;
}

صحيح normHamming(ثابت uchar* a, ثابت uchar* b, صحيح n)
{
    CV_AVX_GUARD;

    صحيح i = 0;
    صحيح نتیجه = 0;

#إذا (CV_SIMD || CV_SIMD_SCALABLE)
    {
        v_uint64 t = vx_setzero_u64();
        لكل (; i <= n - VTraits<v_uint8>::vlanes(); i += VTraits<v_uint8>::vlanes())
            t = v_جمع_کن(t, v_popcount(v_reinterpret_as_u64(v_xor(vx_بارگیری(a + i), vx_بارگیری(b + i)))));
        نتیجه += (صحيح)v_reduce_جمع(t);
    }
#پایان_اگر

#إذا CV_POPCNT
    {
#  إذا defined CV_POPCNT_U64
        لكل(; i <= n - 8; i += 8)
        {
            uint64_t val_a, val_b;
            std::کپی_حافظه(&val_a, a + i, حجم(val_a));
            std::کپی_حافظه(&val_b, b + i, حجم(val_b));
            نتیجه += (صحيح)CV_POPCNT_U64(val_a ^ val_b);
        }
#  endif
        لكل(; i <= n - 4; i += 4)
        {
            uint32_t val_a, val_b;
            std::کپی_حافظه(&val_a, a + i, حجم(val_a));
            std::کپی_حافظه(&val_b, b + i, حجم(val_b));
            نتیجه += (صحيح)CV_POPCNT_U32(val_a ^ val_b);
        }
    }
#پایان_اگر
#إذا CV_ENABLE_UNROLLED
    لكل(; i <= n - 4; i += 4)
    {
        نتیجه += برداشت_شمار_جدول[a[i] ^ b[i]] + برداشت_شمار_جدول[a[i+1] ^ b[i+1]] +
                برداشت_شمار_جدول[a[i+2] ^ b[i+2]] + برداشت_شمار_جدول[a[i+3] ^ b[i+3]];
    }
#پایان_اگر
    لكل(; i < n; i++)
    {
        نتیجه += برداشت_شمار_جدول[a[i] ^ b[i]];
    }
    أرجع نتیجه;
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

#پایان_اگر // CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

CV_CPU_OPTIMIZATION_NAMESPACE_انتها
}} //cv::hal
