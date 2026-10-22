// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.
//
// Copyright (C) 2026, Advanced Micro Devices, Inc., all rights reserved.

#ਅਨ੍ਤਰ੍ਭਾਵ "opencv2/core/hal/intrin.hpp"

ਨਾਮਾਕਾਸ਼ cv { ਨਾਮਾਕਾਸ਼ hal {

ਬਾਹਰੀ ਅਚਲ uchar ਕੱਢੋ_ਗਿਣਤੀ_ਸਾਰਣੀ[256];

ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਪੂਰਨ (*NormHammingFunc)(ਅਚਲ uchar*, ਪੂਰਨ);
ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਪੂਰਨ (*NormHammingDiffFunc)(ਅਚਲ uchar*, ਅਚਲ uchar*, ਪੂਰਨ);

CV_CPU_OPTIMIZATION_NAMESPACE_BEGIN

// forward declarations
ਪੂਰਨ normHamming(ਅਚਲ uchar* a, ਪੂਰਨ n);
ਪੂਰਨ normHamming(ਅਚਲ uchar* a, ਅਚਲ uchar* b, ਪੂਰਨ n);

NormHammingFunc getNormHammingFunc();
NormHammingDiffFunc getNormHammingDiffFunc();

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

#ਜੇ CV_AVX2
ਸਥਿਰ ਸਤਰਅੰਦਰ ਪੂਰਨ mm256_ਕੱਢੋ_epi32(__m256i reg, ਅਚਲ ਪੂਰਨ i)
{
    CV_DECL_ALIGNED(32) ਪੂਰਨ reg_ਅੰਕੜੇ[8];
    CV_DbgAssert(0 <= i && i < 8);
    _mm256_store_si256((__m256i*)reg_ਅੰਕੜੇ, reg);
    ਵਾਪਸ reg_ਅੰਕੜੇ[i];
}
#ਜੇ_ਅੰਤ

ਪੂਰਨ normHamming(ਅਚਲ uchar* a, ਪੂਰਨ n)
{
    CV_AVX_GUARD;

    ਪੂਰਨ i = 0;
    ਪੂਰਨ ਨਤੀਜਾ = 0;

#ਜੇ (CV_SIMD || CV_SIMD_SCALABLE)
    {
        v_uint64 t = vx_setzero_u64();
        ਲਈ (; i <= n - VTraits<v_uint8>::vlanes(); i += VTraits<v_uint8>::vlanes())
            t = v_ਜੋੜੋ(t, v_popcount(v_reinterpret_as_u64(vx_ਲੱਦੋ(a + i))));
        ਨਤੀਜਾ = (ਪੂਰਨ)v_reduce_ਜੋੜ(t);
        vx_cleanup();
    }
#ਜੇ_ਅੰਤ

#ਜੇ CV_POPCNT
    {
#  ਜੇ defined CV_POPCNT_U64
        ਲਈ(; i <= n - 8; i += 8)
        {
            uint64_t val;
            std::ਸਮ੍ਰਿਤੀ_ਨਕਲ(&val, a + i, ਆਕਾਰ(val));
            ਨਤੀਜਾ += (ਪੂਰਨ)CV_POPCNT_U64(val);
        }
#  endif
        ਲਈ(; i <= n - 4; i += 4)
        {
            uint32_t val;
            std::ਸਮ੍ਰਿਤੀ_ਨਕਲ(&val, a + i, ਆਕਾਰ(val));
            ਨਤੀਜਾ += CV_POPCNT_U32(val);
        }
    }
#ਜੇ_ਅੰਤ
#ਜੇ CV_ENABLE_UNROLLED
    ਲਈ(; i <= n - 4; i += 4)
    {
        ਨਤੀਜਾ += ਕੱਢੋ_ਗਿਣਤੀ_ਸਾਰਣੀ[a[i]] + ਕੱਢੋ_ਗਿਣਤੀ_ਸਾਰਣੀ[a[i+1]] +
        ਕੱਢੋ_ਗਿਣਤੀ_ਸਾਰਣੀ[a[i+2]] + ਕੱਢੋ_ਗਿਣਤੀ_ਸਾਰਣੀ[a[i+3]];
    }
#ਜੇ_ਅੰਤ
    ਲਈ(; i < n; i++)
    {
        ਨਤੀਜਾ += ਕੱਢੋ_ਗਿਣਤੀ_ਸਾਰਣੀ[a[i]];
    }
    ਵਾਪਸ ਨਤੀਜਾ;
}

ਪੂਰਨ normHamming(ਅਚਲ uchar* a, ਅਚਲ uchar* b, ਪੂਰਨ n)
{
    CV_AVX_GUARD;

    ਪੂਰਨ i = 0;
    ਪੂਰਨ ਨਤੀਜਾ = 0;

#ਜੇ (CV_SIMD || CV_SIMD_SCALABLE)
    {
        v_uint64 t = vx_setzero_u64();
        ਲਈ (; i <= n - VTraits<v_uint8>::vlanes(); i += VTraits<v_uint8>::vlanes())
            t = v_ਜੋੜੋ(t, v_popcount(v_reinterpret_as_u64(v_xor(vx_ਲੱਦੋ(a + i), vx_ਲੱਦੋ(b + i)))));
        ਨਤੀਜਾ += (ਪੂਰਨ)v_reduce_ਜੋੜ(t);
    }
#ਜੇ_ਅੰਤ

#ਜੇ CV_POPCNT
    {
#  ਜੇ defined CV_POPCNT_U64
        ਲਈ(; i <= n - 8; i += 8)
        {
            uint64_t val_a, val_b;
            std::ਸਮ੍ਰਿਤੀ_ਨਕਲ(&val_a, a + i, ਆਕਾਰ(val_a));
            std::ਸਮ੍ਰਿਤੀ_ਨਕਲ(&val_b, b + i, ਆਕਾਰ(val_b));
            ਨਤੀਜਾ += (ਪੂਰਨ)CV_POPCNT_U64(val_a ^ val_b);
        }
#  endif
        ਲਈ(; i <= n - 4; i += 4)
        {
            uint32_t val_a, val_b;
            std::ਸਮ੍ਰਿਤੀ_ਨਕਲ(&val_a, a + i, ਆਕਾਰ(val_a));
            std::ਸਮ੍ਰਿਤੀ_ਨਕਲ(&val_b, b + i, ਆਕਾਰ(val_b));
            ਨਤੀਜਾ += (ਪੂਰਨ)CV_POPCNT_U32(val_a ^ val_b);
        }
    }
#ਜੇ_ਅੰਤ
#ਜੇ CV_ENABLE_UNROLLED
    ਲਈ(; i <= n - 4; i += 4)
    {
        ਨਤੀਜਾ += ਕੱਢੋ_ਗਿਣਤੀ_ਸਾਰਣੀ[a[i] ^ b[i]] + ਕੱਢੋ_ਗਿਣਤੀ_ਸਾਰਣੀ[a[i+1] ^ b[i+1]] +
                ਕੱਢੋ_ਗਿਣਤੀ_ਸਾਰਣੀ[a[i+2] ^ b[i+2]] + ਕੱਢੋ_ਗਿਣਤੀ_ਸਾਰਣੀ[a[i+3] ^ b[i+3]];
    }
#ਜੇ_ਅੰਤ
    ਲਈ(; i < n; i++)
    {
        ਨਤੀਜਾ += ਕੱਢੋ_ਗਿਣਤੀ_ਸਾਰਣੀ[a[i] ^ b[i]];
    }
    ਵਾਪਸ ਨਤੀਜਾ;
}

NormHammingFunc getNormHammingFunc()
{
    NormHammingFunc f = &normHamming; // disambiguate the (a,n) overload
    ਵਾਪਸ f;
}
NormHammingDiffFunc getNormHammingDiffFunc()
{
    NormHammingDiffFunc f = &normHamming; // disambiguate the (a,b,n) overload
    ਵਾਪਸ f;
}

#ਜੇ_ਅੰਤ // CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

CV_CPU_OPTIMIZATION_NAMESPACE_ਸਿਰਾ
}} //cv::hal
