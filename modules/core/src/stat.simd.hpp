// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.
//
// Copyright (C) 2026, Advanced Micro Devices, Inc., all rights reserved.

#అన్తర్భావ "opencv2/core/hal/intrin.hpp"

నామాకాశ cv { నామాకాశ hal {

బాహ్య స్థిరరాశి uchar తీయి_లెక్క_పట్టిక[256];

రకనిర్వచనం పూర్ణాంకం (*NormHammingFunc)(స్థిరరాశి uchar*, పూర్ణాంకం);
రకనిర్వచనం పూర్ణాంకం (*NormHammingDiffFunc)(స్థిరరాశి uchar*, స్థిరరాశి uchar*, పూర్ణాంకం);

CV_CPU_OPTIMIZATION_NAMESPACE_BEGIN

// forward declarations
పూర్ణాంకం normHamming(స్థిరరాశి uchar* a, పూర్ణాంకం n);
పూర్ణాంకం normHamming(స్థిరరాశి uchar* a, స్థిరరాశి uchar* b, పూర్ణాంకం n);

NormHammingFunc getNormHammingFunc();
NormHammingDiffFunc getNormHammingDiffFunc();

#నిర్వచితం_కాకపోతే CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

#ఒకవేళ CV_AVX2
స్థిర అంతఃవరుస పూర్ణాంకం mm256_వెలికితీయి_epi32(__m256i reg, స్థిరరాశి పూర్ణాంకం i)
{
    CV_DECL_ALIGNED(32) పూర్ణాంకం reg_దత్తాంశం[8];
    CV_DbgAssert(0 <= i && i < 8);
    _mm256_store_si256((__m256i*)reg_దత్తాంశం, reg);
    తిరిగి reg_దత్తాంశం[i];
}
#అయితే_ముగింపు

పూర్ణాంకం normHamming(స్థిరరాశి uchar* a, పూర్ణాంకం n)
{
    CV_AVX_GUARD;

    పూర్ణాంకం i = 0;
    పూర్ణాంకం ఫలితం = 0;

#ఒకవేళ (CV_SIMD || CV_SIMD_SCALABLE)
    {
        v_uint64 t = vx_setzero_u64();
        కొరకు (; i <= n - VTraits<v_uint8>::vlanes(); i += VTraits<v_uint8>::vlanes())
            t = v_కూడు(t, v_popcount(v_reinterpret_as_u64(vx_ఎక్కించు(a + i))));
        ఫలితం = (పూర్ణాంకం)v_reduce_కూడిక(t);
        vx_cleanup();
    }
#అయితే_ముగింపు

#ఒకవేళ CV_POPCNT
    {
#  ఒకవేళ defined CV_POPCNT_U64
        కొరకు(; i <= n - 8; i += 8)
        {
            uint64_t val;
            std::జ్ఞాపకం_నకలు(&val, a + i, పరిమాణం(val));
            ఫలితం += (పూర్ణాంకం)CV_POPCNT_U64(val);
        }
#  endif
        కొరకు(; i <= n - 4; i += 4)
        {
            uint32_t val;
            std::జ్ఞాపకం_నకలు(&val, a + i, పరిమాణం(val));
            ఫలితం += CV_POPCNT_U32(val);
        }
    }
#అయితే_ముగింపు
#ఒకవేళ CV_ENABLE_UNROLLED
    కొరకు(; i <= n - 4; i += 4)
    {
        ఫలితం += తీయి_లెక్క_పట్టిక[a[i]] + తీయి_లెక్క_పట్టిక[a[i+1]] +
        తీయి_లెక్క_పట్టిక[a[i+2]] + తీయి_లెక్క_పట్టిక[a[i+3]];
    }
#అయితే_ముగింపు
    కొరకు(; i < n; i++)
    {
        ఫలితం += తీయి_లెక్క_పట్టిక[a[i]];
    }
    తిరిగి ఫలితం;
}

పూర్ణాంకం normHamming(స్థిరరాశి uchar* a, స్థిరరాశి uchar* b, పూర్ణాంకం n)
{
    CV_AVX_GUARD;

    పూర్ణాంకం i = 0;
    పూర్ణాంకం ఫలితం = 0;

#ఒకవేళ (CV_SIMD || CV_SIMD_SCALABLE)
    {
        v_uint64 t = vx_setzero_u64();
        కొరకు (; i <= n - VTraits<v_uint8>::vlanes(); i += VTraits<v_uint8>::vlanes())
            t = v_కూడు(t, v_popcount(v_reinterpret_as_u64(v_xor(vx_ఎక్కించు(a + i), vx_ఎక్కించు(b + i)))));
        ఫలితం += (పూర్ణాంకం)v_reduce_కూడిక(t);
    }
#అయితే_ముగింపు

#ఒకవేళ CV_POPCNT
    {
#  ఒకవేళ defined CV_POPCNT_U64
        కొరకు(; i <= n - 8; i += 8)
        {
            uint64_t val_a, val_b;
            std::జ్ఞాపకం_నకలు(&val_a, a + i, పరిమాణం(val_a));
            std::జ్ఞాపకం_నకలు(&val_b, b + i, పరిమాణం(val_b));
            ఫలితం += (పూర్ణాంకం)CV_POPCNT_U64(val_a ^ val_b);
        }
#  endif
        కొరకు(; i <= n - 4; i += 4)
        {
            uint32_t val_a, val_b;
            std::జ్ఞాపకం_నకలు(&val_a, a + i, పరిమాణం(val_a));
            std::జ్ఞాపకం_నకలు(&val_b, b + i, పరిమాణం(val_b));
            ఫలితం += (పూర్ణాంకం)CV_POPCNT_U32(val_a ^ val_b);
        }
    }
#అయితే_ముగింపు
#ఒకవేళ CV_ENABLE_UNROLLED
    కొరకు(; i <= n - 4; i += 4)
    {
        ఫలితం += తీయి_లెక్క_పట్టిక[a[i] ^ b[i]] + తీయి_లెక్క_పట్టిక[a[i+1] ^ b[i+1]] +
                తీయి_లెక్క_పట్టిక[a[i+2] ^ b[i+2]] + తీయి_లెక్క_పట్టిక[a[i+3] ^ b[i+3]];
    }
#అయితే_ముగింపు
    కొరకు(; i < n; i++)
    {
        ఫలితం += తీయి_లెక్క_పట్టిక[a[i] ^ b[i]];
    }
    తిరిగి ఫలితం;
}

NormHammingFunc getNormHammingFunc()
{
    NormHammingFunc f = &normHamming; // disambiguate the (a,n) overload
    తిరిగి f;
}
NormHammingDiffFunc getNormHammingDiffFunc()
{
    NormHammingDiffFunc f = &normHamming; // disambiguate the (a,b,n) overload
    తిరిగి f;
}

#అయితే_ముగింపు // CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

CV_CPU_OPTIMIZATION_NAMESPACE_చివర
}} //cv::hal
