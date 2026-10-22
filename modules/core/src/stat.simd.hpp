// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.
//
// Copyright (C) 2026, Advanced Micro Devices, Inc., all rights reserved.

#شامل "opencv2/core/hal/intrin.hpp"

نامخلا cv { نامخلا hal {

خارجی ثابت uchar نکالو_گنتی_جدول[256];

قسمتعریف صحیح (*NormHammingFunc)(ثابت uchar*, صحیح);
قسمتعریف صحیح (*NormHammingDiffFunc)(ثابت uchar*, ثابت uchar*, صحیح);

CV_CPU_OPTIMIZATION_NAMESPACE_BEGIN

// forward declarations
صحیح normHamming(ثابت uchar* a, صحیح n);
صحیح normHamming(ثابت uchar* a, ثابت uchar* b, صحیح n);

NormHammingFunc getNormHammingFunc();
NormHammingDiffFunc getNormHammingDiffFunc();

#اگر_غیر_تعریف_شدہ CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

#اگر CV_AVX2
ساکن اندرونی صحیح mm256_نکالو_epi32(__m256i reg, ثابت صحیح i)
{
    CV_DECL_ALIGNED(32) صحیح reg_ڈیٹا[8];
    CV_DbgAssert(0 <= i && i < 8);
    _mm256_store_si256((__m256i*)reg_ڈیٹا, reg);
    واپس reg_ڈیٹا[i];
}
#اگر_ختم

صحیح normHamming(ثابت uchar* a, صحیح n)
{
    CV_AVX_GUARD;

    صحیح i = 0;
    صحیح نتیجہ = 0;

#اگر (CV_SIMD || CV_SIMD_SCALABLE)
    {
        v_uint64 t = vx_setzero_u64();
        برائے (; i <= n - VTraits<v_uint8>::vlanes(); i += VTraits<v_uint8>::vlanes())
            t = v_جوڑو(t, v_popcount(v_reinterpret_as_u64(vx_بھرو(a + i))));
        نتیجہ = (صحیح)v_reduce_مجموع(t);
        vx_cleanup();
    }
#اگر_ختم

#اگر CV_POPCNT
    {
#  اگر defined CV_POPCNT_U64
        برائے(; i <= n - 8; i += 8)
        {
            uint64_t val;
            std::حافظہ_نقل(&val, a + i, حجم(val));
            نتیجہ += (صحیح)CV_POPCNT_U64(val);
        }
#  endif
        برائے(; i <= n - 4; i += 4)
        {
            uint32_t val;
            std::حافظہ_نقل(&val, a + i, حجم(val));
            نتیجہ += CV_POPCNT_U32(val);
        }
    }
#اگر_ختم
#اگر CV_ENABLE_UNROLLED
    برائے(; i <= n - 4; i += 4)
    {
        نتیجہ += نکالو_گنتی_جدول[a[i]] + نکالو_گنتی_جدول[a[i+1]] +
        نکالو_گنتی_جدول[a[i+2]] + نکالو_گنتی_جدول[a[i+3]];
    }
#اگر_ختم
    برائے(; i < n; i++)
    {
        نتیجہ += نکالو_گنتی_جدول[a[i]];
    }
    واپس نتیجہ;
}

صحیح normHamming(ثابت uchar* a, ثابت uchar* b, صحیح n)
{
    CV_AVX_GUARD;

    صحیح i = 0;
    صحیح نتیجہ = 0;

#اگر (CV_SIMD || CV_SIMD_SCALABLE)
    {
        v_uint64 t = vx_setzero_u64();
        برائے (; i <= n - VTraits<v_uint8>::vlanes(); i += VTraits<v_uint8>::vlanes())
            t = v_جوڑو(t, v_popcount(v_reinterpret_as_u64(v_xor(vx_بھرو(a + i), vx_بھرو(b + i)))));
        نتیجہ += (صحیح)v_reduce_مجموع(t);
    }
#اگر_ختم

#اگر CV_POPCNT
    {
#  اگر defined CV_POPCNT_U64
        برائے(; i <= n - 8; i += 8)
        {
            uint64_t val_a, val_b;
            std::حافظہ_نقل(&val_a, a + i, حجم(val_a));
            std::حافظہ_نقل(&val_b, b + i, حجم(val_b));
            نتیجہ += (صحیح)CV_POPCNT_U64(val_a ^ val_b);
        }
#  endif
        برائے(; i <= n - 4; i += 4)
        {
            uint32_t val_a, val_b;
            std::حافظہ_نقل(&val_a, a + i, حجم(val_a));
            std::حافظہ_نقل(&val_b, b + i, حجم(val_b));
            نتیجہ += (صحیح)CV_POPCNT_U32(val_a ^ val_b);
        }
    }
#اگر_ختم
#اگر CV_ENABLE_UNROLLED
    برائے(; i <= n - 4; i += 4)
    {
        نتیجہ += نکالو_گنتی_جدول[a[i] ^ b[i]] + نکالو_گنتی_جدول[a[i+1] ^ b[i+1]] +
                نکالو_گنتی_جدول[a[i+2] ^ b[i+2]] + نکالو_گنتی_جدول[a[i+3] ^ b[i+3]];
    }
#اگر_ختم
    برائے(; i < n; i++)
    {
        نتیجہ += نکالو_گنتی_جدول[a[i] ^ b[i]];
    }
    واپس نتیجہ;
}

NormHammingFunc getNormHammingFunc()
{
    NormHammingFunc f = &normHamming; // disambiguate the (a,n) overload
    واپس f;
}
NormHammingDiffFunc getNormHammingDiffFunc()
{
    NormHammingDiffFunc f = &normHamming; // disambiguate the (a,b,n) overload
    واپس f;
}

#اگر_ختم // CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

CV_CPU_OPTIMIZATION_NAMESPACE_انتہا
}} //cv::hal
