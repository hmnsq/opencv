// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.
//
// Copyright (C) 2026, Advanced Micro Devices, Inc., all rights reserved.

#अन्तर्भाव "opencv2/core/hal/intrin.hpp"

नामाकाश cv { नामाकाश hal {

बाह्य स्थिरांक uchar काढा_मोजणी_Table[256];

प्रकारव्याख्या पूर्णांक (*NormHammingFunc)(स्थिरांक uchar*, पूर्णांक);
प्रकारव्याख्या पूर्णांक (*NormHammingDiffFunc)(स्थिरांक uchar*, स्थिरांक uchar*, पूर्णांक);

CV_CPU_OPTIMIZATION_NAMESPACE_BEGIN

// forward declarations
पूर्णांक normHamming(स्थिरांक uchar* a, पूर्णांक n);
पूर्णांक normHamming(स्थिरांक uchar* a, स्थिरांक uchar* b, पूर्णांक n);

NormHammingFunc getNormHammingFunc();
NormHammingDiffFunc getNormHammingDiffFunc();

#ifndef CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

#जर CV_AVX2
स्थिर ओळीत पूर्णांक _mm256_extract_epi32_(__m256i reg, स्थिरांक पूर्णांक i)
{
    CV_DECL_ALIGNED(32) पूर्णांक reg_data[8];
    CV_DbgAssert(0 <= i && i < 8);
    _mm256_store_si256((__m256i*)reg_data, reg);
    परत reg_data[i];
}
#endif

पूर्णांक normHamming(स्थिरांक uchar* a, पूर्णांक n)
{
    CV_AVX_GUARD;

    पूर्णांक i = 0;
    पूर्णांक result = 0;

#जर (CV_SIMD || CV_SIMD_SCALABLE)
    {
        v_uint64 t = vx_setzero_u64();
        साठी (; i <= n - VTraits<v_uint8>::vlanes(); i += VTraits<v_uint8>::vlanes())
            t = v_add(t, v_popcount(v_reinterpret_as_u64(vx_load(a + i))));
        result = (पूर्णांक)v_reduce_sum(t);
        vx_cleanup();
    }
#endif

#जर CV_POPCNT
    {
#  जर defined CV_POPCNT_U64
        साठी(; i <= n - 8; i += 8)
        {
            uint64_t val;
            std::स_नकल(&val, a + i, आकार(val));
            result += (पूर्णांक)CV_POPCNT_U64(val);
        }
#  endif
        साठी(; i <= n - 4; i += 4)
        {
            uint32_t val;
            std::स_नकल(&val, a + i, आकार(val));
            result += CV_POPCNT_U32(val);
        }
    }
#endif
#जर CV_ENABLE_UNROLLED
    साठी(; i <= n - 4; i += 4)
    {
        result += काढा_मोजणी_Table[a[i]] + काढा_मोजणी_Table[a[i+1]] +
        काढा_मोजणी_Table[a[i+2]] + काढा_मोजणी_Table[a[i+3]];
    }
#endif
    साठी(; i < n; i++)
    {
        result += काढा_मोजणी_Table[a[i]];
    }
    परत result;
}

पूर्णांक normHamming(स्थिरांक uchar* a, स्थिरांक uchar* b, पूर्णांक n)
{
    CV_AVX_GUARD;

    पूर्णांक i = 0;
    पूर्णांक result = 0;

#जर (CV_SIMD || CV_SIMD_SCALABLE)
    {
        v_uint64 t = vx_setzero_u64();
        साठी (; i <= n - VTraits<v_uint8>::vlanes(); i += VTraits<v_uint8>::vlanes())
            t = v_add(t, v_popcount(v_reinterpret_as_u64(v_xor(vx_load(a + i), vx_load(b + i)))));
        result += (पूर्णांक)v_reduce_sum(t);
    }
#endif

#जर CV_POPCNT
    {
#  जर defined CV_POPCNT_U64
        साठी(; i <= n - 8; i += 8)
        {
            uint64_t val_a, val_b;
            std::स_नकल(&val_a, a + i, आकार(val_a));
            std::स_नकल(&val_b, b + i, आकार(val_b));
            result += (पूर्णांक)CV_POPCNT_U64(val_a ^ val_b);
        }
#  endif
        साठी(; i <= n - 4; i += 4)
        {
            uint32_t val_a, val_b;
            std::स_नकल(&val_a, a + i, आकार(val_a));
            std::स_नकल(&val_b, b + i, आकार(val_b));
            result += (पूर्णांक)CV_POPCNT_U32(val_a ^ val_b);
        }
    }
#endif
#जर CV_ENABLE_UNROLLED
    साठी(; i <= n - 4; i += 4)
    {
        result += काढा_मोजणी_Table[a[i] ^ b[i]] + काढा_मोजणी_Table[a[i+1] ^ b[i+1]] +
                काढा_मोजणी_Table[a[i+2] ^ b[i+2]] + काढा_मोजणी_Table[a[i+3] ^ b[i+3]];
    }
#endif
    साठी(; i < n; i++)
    {
        result += काढा_मोजणी_Table[a[i] ^ b[i]];
    }
    परत result;
}

NormHammingFunc getNormHammingFunc()
{
    NormHammingFunc f = &normHamming; // disambiguate the (a,n) overload
    परत f;
}
NormHammingDiffFunc getNormHammingDiffFunc()
{
    NormHammingDiffFunc f = &normHamming; // disambiguate the (a,b,n) overload
    परत f;
}

#endif // CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

CV_CPU_OPTIMIZATION_NAMESPACE_END
}} //cv::hal
