// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.
//
// Copyright (C) 2026, Advanced Micro Devices, Inc., all rights reserved.

#അന്തര്ഭാവ "opencv2/core/hal/intrin.hpp"

നാമാകാശ cv { നാമാകാശ hal {

ബാഹ്യ സ്ഥിരം uchar എടുക്കുക_എണ്ണം_Table[256];

തരനിർവചനം പൂർണ്ണസംഖ്യ (*NormHammingFunc)(സ്ഥിരം uchar*, പൂർണ്ണസംഖ്യ);
തരനിർവചനം പൂർണ്ണസംഖ്യ (*NormHammingDiffFunc)(സ്ഥിരം uchar*, സ്ഥിരം uchar*, പൂർണ്ണസംഖ്യ);

CV_CPU_OPTIMIZATION_NAMESPACE_BEGIN

// forward declarations
പൂർണ്ണസംഖ്യ normHamming(സ്ഥിരം uchar* a, പൂർണ്ണസംഖ്യ n);
പൂർണ്ണസംഖ്യ normHamming(സ്ഥിരം uchar* a, സ്ഥിരം uchar* b, പൂർണ്ണസംഖ്യ n);

NormHammingFunc getNormHammingFunc();
NormHammingDiffFunc getNormHammingDiffFunc();

#ifndef CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

#എങ്കിൽ CV_AVX2
സ്ഥിതം വരിയിൽ പൂർണ്ണസംഖ്യ _mm256_extract_epi32_(__m256i reg, സ്ഥിരം പൂർണ്ണസംഖ്യ i)
{
    CV_DECL_ALIGNED(32) പൂർണ്ണസംഖ്യ reg_data[8];
    CV_DbgAssert(0 <= i && i < 8);
    _mm256_store_si256((__m256i*)reg_data, reg);
    തിരികെ reg_data[i];
}
#endif

പൂർണ്ണസംഖ്യ normHamming(സ്ഥിരം uchar* a, പൂർണ്ണസംഖ്യ n)
{
    CV_AVX_GUARD;

    പൂർണ്ണസംഖ്യ i = 0;
    പൂർണ്ണസംഖ്യ result = 0;

#എങ്കിൽ (CV_SIMD || CV_SIMD_SCALABLE)
    {
        v_uint64 t = vx_setzero_u64();
        വേണ്ടി (; i <= n - VTraits<v_uint8>::vlanes(); i += VTraits<v_uint8>::vlanes())
            t = v_add(t, v_popcount(v_reinterpret_as_u64(vx_load(a + i))));
        result = (പൂർണ്ണസംഖ്യ)v_reduce_sum(t);
        vx_cleanup();
    }
#endif

#എങ്കിൽ CV_POPCNT
    {
#  എങ്കിൽ defined CV_POPCNT_U64
        വേണ്ടി(; i <= n - 8; i += 8)
        {
            uint64_t val;
            std::memcpy(&val, a + i, വലുപ്പം(val));
            result += (പൂർണ്ണസംഖ്യ)CV_POPCNT_U64(val);
        }
#  endif
        വേണ്ടി(; i <= n - 4; i += 4)
        {
            uint32_t val;
            std::memcpy(&val, a + i, വലുപ്പം(val));
            result += CV_POPCNT_U32(val);
        }
    }
#endif
#എങ്കിൽ CV_ENABLE_UNROLLED
    വേണ്ടി(; i <= n - 4; i += 4)
    {
        result += എടുക്കുക_എണ്ണം_Table[a[i]] + എടുക്കുക_എണ്ണം_Table[a[i+1]] +
        എടുക്കുക_എണ്ണം_Table[a[i+2]] + എടുക്കുക_എണ്ണം_Table[a[i+3]];
    }
#endif
    വേണ്ടി(; i < n; i++)
    {
        result += എടുക്കുക_എണ്ണം_Table[a[i]];
    }
    തിരികെ result;
}

പൂർണ്ണസംഖ്യ normHamming(സ്ഥിരം uchar* a, സ്ഥിരം uchar* b, പൂർണ്ണസംഖ്യ n)
{
    CV_AVX_GUARD;

    പൂർണ്ണസംഖ്യ i = 0;
    പൂർണ്ണസംഖ്യ result = 0;

#എങ്കിൽ (CV_SIMD || CV_SIMD_SCALABLE)
    {
        v_uint64 t = vx_setzero_u64();
        വേണ്ടി (; i <= n - VTraits<v_uint8>::vlanes(); i += VTraits<v_uint8>::vlanes())
            t = v_add(t, v_popcount(v_reinterpret_as_u64(v_xor(vx_load(a + i), vx_load(b + i)))));
        result += (പൂർണ്ണസംഖ്യ)v_reduce_sum(t);
    }
#endif

#എങ്കിൽ CV_POPCNT
    {
#  എങ്കിൽ defined CV_POPCNT_U64
        വേണ്ടി(; i <= n - 8; i += 8)
        {
            uint64_t val_a, val_b;
            std::memcpy(&val_a, a + i, വലുപ്പം(val_a));
            std::memcpy(&val_b, b + i, വലുപ്പം(val_b));
            result += (പൂർണ്ണസംഖ്യ)CV_POPCNT_U64(val_a ^ val_b);
        }
#  endif
        വേണ്ടി(; i <= n - 4; i += 4)
        {
            uint32_t val_a, val_b;
            std::memcpy(&val_a, a + i, വലുപ്പം(val_a));
            std::memcpy(&val_b, b + i, വലുപ്പം(val_b));
            result += (പൂർണ്ണസംഖ്യ)CV_POPCNT_U32(val_a ^ val_b);
        }
    }
#endif
#എങ്കിൽ CV_ENABLE_UNROLLED
    വേണ്ടി(; i <= n - 4; i += 4)
    {
        result += എടുക്കുക_എണ്ണം_Table[a[i] ^ b[i]] + എടുക്കുക_എണ്ണം_Table[a[i+1] ^ b[i+1]] +
                എടുക്കുക_എണ്ണം_Table[a[i+2] ^ b[i+2]] + എടുക്കുക_എണ്ണം_Table[a[i+3] ^ b[i+3]];
    }
#endif
    വേണ്ടി(; i < n; i++)
    {
        result += എടുക്കുക_എണ്ണം_Table[a[i] ^ b[i]];
    }
    തിരികെ result;
}

NormHammingFunc getNormHammingFunc()
{
    NormHammingFunc f = &normHamming; // disambiguate the (a,n) overload
    തിരികെ f;
}
NormHammingDiffFunc getNormHammingDiffFunc()
{
    NormHammingDiffFunc f = &normHamming; // disambiguate the (a,b,n) overload
    തിരികെ f;
}

#endif // CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

CV_CPU_OPTIMIZATION_NAMESPACE_END
}} //cv::hal
