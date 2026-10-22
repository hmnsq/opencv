// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.
//
// Copyright (C) 2026, Advanced Micro Devices, Inc., all rights reserved.

#ଅନ୍ତର୍ଭାଵ "opencv2/core/hal/intrin.hpp"

ନାମାକାଶ cv { ନାମାକାଶ hal {

ବାହ୍ୟ ଅଚଳ uchar କାଢ଼_ଗଣନା_Table[256];

ପ୍ରକାରବ୍ୟାଖ୍ୟା ପୂର୍ଣ୍ଣସଂଖ୍ୟା (*NormHammingFunc)(ଅଚଳ uchar*, ପୂର୍ଣ୍ଣସଂଖ୍ୟା);
ପ୍ରକାରବ୍ୟାଖ୍ୟା ପୂର୍ଣ୍ଣସଂଖ୍ୟା (*NormHammingDiffFunc)(ଅଚଳ uchar*, ଅଚଳ uchar*, ପୂର୍ଣ୍ଣସଂଖ୍ୟା);

CV_CPU_OPTIMIZATION_NAMESPACE_BEGIN

// forward declarations
ପୂର୍ଣ୍ଣସଂଖ୍ୟା normHamming(ଅଚଳ uchar* a, ପୂର୍ଣ୍ଣସଂଖ୍ୟା n);
ପୂର୍ଣ୍ଣସଂଖ୍ୟା normHamming(ଅଚଳ uchar* a, ଅଚଳ uchar* b, ପୂର୍ଣ୍ଣସଂଖ୍ୟା n);

NormHammingFunc getNormHammingFunc();
NormHammingDiffFunc getNormHammingDiffFunc();

#ifndef CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

#ଯଦି CV_AVX2
ସ୍ଥିର ଧାଡ଼ିଭିତରେ ପୂର୍ଣ୍ଣସଂଖ୍ୟା _mm256_extract_epi32_(__m256i reg, ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା i)
{
    CV_DECL_ALIGNED(32) ପୂର୍ଣ୍ଣସଂଖ୍ୟା reg_data[8];
    CV_DbgAssert(0 <= i && i < 8);
    _mm256_store_si256((__m256i*)reg_data, reg);
    ଫେରନ୍ତୁ reg_data[i];
}
#endif

ପୂର୍ଣ୍ଣସଂଖ୍ୟା normHamming(ଅଚଳ uchar* a, ପୂର୍ଣ୍ଣସଂଖ୍ୟା n)
{
    CV_AVX_GUARD;

    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା result = 0;

#ଯଦି (CV_SIMD || CV_SIMD_SCALABLE)
    {
        v_uint64 t = vx_setzero_u64();
        ପାଇଁ (; i <= n - VTraits<v_uint8>::vlanes(); i += VTraits<v_uint8>::vlanes())
            t = v_add(t, v_popcount(v_reinterpret_as_u64(vx_load(a + i))));
        result = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)v_reduce_sum(t);
        vx_cleanup();
    }
#endif

#ଯଦି CV_POPCNT
    {
#  ଯଦି defined CV_POPCNT_U64
        ପାଇଁ(; i <= n - 8; i += 8)
        {
            uint64_t val;
            std::memcpy(&val, a + i, ଆକାର(val));
            result += (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)CV_POPCNT_U64(val);
        }
#  endif
        ପାଇଁ(; i <= n - 4; i += 4)
        {
            uint32_t val;
            std::memcpy(&val, a + i, ଆକାର(val));
            result += CV_POPCNT_U32(val);
        }
    }
#endif
#ଯଦି CV_ENABLE_UNROLLED
    ପାଇଁ(; i <= n - 4; i += 4)
    {
        result += କାଢ଼_ଗଣନା_Table[a[i]] + କାଢ଼_ଗଣନା_Table[a[i+1]] +
        କାଢ଼_ଗଣନା_Table[a[i+2]] + କାଢ଼_ଗଣନା_Table[a[i+3]];
    }
#endif
    ପାଇଁ(; i < n; i++)
    {
        result += କାଢ଼_ଗଣନା_Table[a[i]];
    }
    ଫେରନ୍ତୁ result;
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା normHamming(ଅଚଳ uchar* a, ଅଚଳ uchar* b, ପୂର୍ଣ୍ଣସଂଖ୍ୟା n)
{
    CV_AVX_GUARD;

    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା result = 0;

#ଯଦି (CV_SIMD || CV_SIMD_SCALABLE)
    {
        v_uint64 t = vx_setzero_u64();
        ପାଇଁ (; i <= n - VTraits<v_uint8>::vlanes(); i += VTraits<v_uint8>::vlanes())
            t = v_add(t, v_popcount(v_reinterpret_as_u64(v_xor(vx_load(a + i), vx_load(b + i)))));
        result += (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)v_reduce_sum(t);
    }
#endif

#ଯଦି CV_POPCNT
    {
#  ଯଦି defined CV_POPCNT_U64
        ପାଇଁ(; i <= n - 8; i += 8)
        {
            uint64_t val_a, val_b;
            std::memcpy(&val_a, a + i, ଆକାର(val_a));
            std::memcpy(&val_b, b + i, ଆକାର(val_b));
            result += (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)CV_POPCNT_U64(val_a ^ val_b);
        }
#  endif
        ପାଇଁ(; i <= n - 4; i += 4)
        {
            uint32_t val_a, val_b;
            std::memcpy(&val_a, a + i, ଆକାର(val_a));
            std::memcpy(&val_b, b + i, ଆକାର(val_b));
            result += (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)CV_POPCNT_U32(val_a ^ val_b);
        }
    }
#endif
#ଯଦି CV_ENABLE_UNROLLED
    ପାଇଁ(; i <= n - 4; i += 4)
    {
        result += କାଢ଼_ଗଣନା_Table[a[i] ^ b[i]] + କାଢ଼_ଗଣନା_Table[a[i+1] ^ b[i+1]] +
                କାଢ଼_ଗଣନା_Table[a[i+2] ^ b[i+2]] + କାଢ଼_ଗଣନା_Table[a[i+3] ^ b[i+3]];
    }
#endif
    ପାଇଁ(; i < n; i++)
    {
        result += କାଢ଼_ଗଣନା_Table[a[i] ^ b[i]];
    }
    ଫେରନ୍ତୁ result;
}

NormHammingFunc getNormHammingFunc()
{
    NormHammingFunc f = &normHamming; // disambiguate the (a,n) overload
    ଫେରନ୍ତୁ f;
}
NormHammingDiffFunc getNormHammingDiffFunc()
{
    NormHammingDiffFunc f = &normHamming; // disambiguate the (a,b,n) overload
    ଫେରନ୍ତୁ f;
}

#endif // CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

CV_CPU_OPTIMIZATION_NAMESPACE_END
}} //cv::hal
