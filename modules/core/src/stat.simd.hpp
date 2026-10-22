// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.
//
// Copyright (C) 2026, Advanced Micro Devices, Inc., all rights reserved.

#ضمن "opencv2/core/hal/intrin.hpp"

نطاق cv { نطاق hal {

خارجي ثابت uchar راوباسه_شمېر_جدول[256];

تعريفنوع صحيح (*NormHammingFunc)(ثابت uchar*, صحيح);
تعريفنوع صحيح (*NormHammingDiffFunc)(ثابت uchar*, ثابت uchar*, صحيح);

CV_CPU_OPTIMIZATION_NAMESPACE_BEGIN

// forward declarations
صحيح normHamming(ثابت uchar* a, صحيح n);
صحيح normHamming(ثابت uchar* a, ثابت uchar* b, صحيح n);

NormHammingFunc getNormHammingFunc();
NormHammingDiffFunc getNormHammingDiffFunc();

#که_نه_تعریف_شوی CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

#إذا CV_AVX2
ساكن مضمن صحيح mm256_ایستل_epi32(__m256i reg, ثابت صحيح i)
{
    CV_DECL_ALIGNED(32) صحيح reg_معلومات[8];
    CV_DbgAssert(0 <= i && i < 8);
    _mm256_store_si256((__m256i*)reg_معلومات, reg);
    أرجع reg_معلومات[i];
}
#د_که_پای

صحيح normHamming(ثابت uchar* a, صحيح n)
{
    CV_AVX_GUARD;

    صحيح i = 0;
    صحيح پایله = 0;

#إذا (CV_SIMD || CV_SIMD_SCALABLE)
    {
        v_uint64 t = vx_setzero_u64();
        لكل (; i <= n - VTraits<v_uint8>::vlanes(); i += VTraits<v_uint8>::vlanes())
            t = v_جمع_کړه(t, v_popcount(v_reinterpret_as_u64(vx_بارول(a + i))));
        پایله = (صحيح)v_reduce_مجموعه(t);
        vx_cleanup();
    }
#د_که_پای

#إذا CV_POPCNT
    {
#  إذا defined CV_POPCNT_U64
        لكل(; i <= n - 8; i += 8)
        {
            uint64_t val;
            std::حافظه_نقل(&val, a + i, حجم(val));
            پایله += (صحيح)CV_POPCNT_U64(val);
        }
#  endif
        لكل(; i <= n - 4; i += 4)
        {
            uint32_t val;
            std::حافظه_نقل(&val, a + i, حجم(val));
            پایله += CV_POPCNT_U32(val);
        }
    }
#د_که_پای
#إذا CV_ENABLE_UNROLLED
    لكل(; i <= n - 4; i += 4)
    {
        پایله += راوباسه_شمېر_جدول[a[i]] + راوباسه_شمېر_جدول[a[i+1]] +
        راوباسه_شمېر_جدول[a[i+2]] + راوباسه_شمېر_جدول[a[i+3]];
    }
#د_که_پای
    لكل(; i < n; i++)
    {
        پایله += راوباسه_شمېر_جدول[a[i]];
    }
    أرجع پایله;
}

صحيح normHamming(ثابت uchar* a, ثابت uchar* b, صحيح n)
{
    CV_AVX_GUARD;

    صحيح i = 0;
    صحيح پایله = 0;

#إذا (CV_SIMD || CV_SIMD_SCALABLE)
    {
        v_uint64 t = vx_setzero_u64();
        لكل (; i <= n - VTraits<v_uint8>::vlanes(); i += VTraits<v_uint8>::vlanes())
            t = v_جمع_کړه(t, v_popcount(v_reinterpret_as_u64(v_xor(vx_بارول(a + i), vx_بارول(b + i)))));
        پایله += (صحيح)v_reduce_مجموعه(t);
    }
#د_که_پای

#إذا CV_POPCNT
    {
#  إذا defined CV_POPCNT_U64
        لكل(; i <= n - 8; i += 8)
        {
            uint64_t val_a, val_b;
            std::حافظه_نقل(&val_a, a + i, حجم(val_a));
            std::حافظه_نقل(&val_b, b + i, حجم(val_b));
            پایله += (صحيح)CV_POPCNT_U64(val_a ^ val_b);
        }
#  endif
        لكل(; i <= n - 4; i += 4)
        {
            uint32_t val_a, val_b;
            std::حافظه_نقل(&val_a, a + i, حجم(val_a));
            std::حافظه_نقل(&val_b, b + i, حجم(val_b));
            پایله += (صحيح)CV_POPCNT_U32(val_a ^ val_b);
        }
    }
#د_که_پای
#إذا CV_ENABLE_UNROLLED
    لكل(; i <= n - 4; i += 4)
    {
        پایله += راوباسه_شمېر_جدول[a[i] ^ b[i]] + راوباسه_شمېر_جدول[a[i+1] ^ b[i+1]] +
                راوباسه_شمېر_جدول[a[i+2] ^ b[i+2]] + راوباسه_شمېر_جدول[a[i+3] ^ b[i+3]];
    }
#د_که_پای
    لكل(; i < n; i++)
    {
        پایله += راوباسه_شمېر_جدول[a[i] ^ b[i]];
    }
    أرجع پایله;
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

#د_که_پای // CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

CV_CPU_OPTIMIZATION_NAMESPACE_پای_ټکی
}} //cv::hal
