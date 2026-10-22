// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.
//
// Copyright (C) 2026, Advanced Micro Devices, Inc., all rights reserved.

#अन्तर्भाव "opencv2/core/hal/intrin.hpp"

नामाकाश cv { नामाकाश hal {

बाह्य अचर uchar झिक_गन्ती_Table[256];

प्रकारनिर्देश पूर्णाङ्क (*NormHammingFunc)(अचर uchar*, पूर्णाङ्क);
प्रकारनिर्देश पूर्णाङ्क (*NormHammingDiffFunc)(अचर uchar*, अचर uchar*, पूर्णाङ्क);

CV_CPU_OPTIMIZATION_NAMESPACE_BEGIN

// forward declarations
पूर्णाङ्क normHamming(अचर uchar* a, पूर्णाङ्क n);
पूर्णाङ्क normHamming(अचर uchar* a, अचर uchar* b, पूर्णाङ्क n);

NormHammingFunc getNormHammingFunc();
NormHammingDiffFunc getNormHammingDiffFunc();

#ifndef CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

#यदि CV_AVX2
स्थिर अन्तःस्थ पूर्णाङ्क _mm256_extract_epi32_(__m256i reg, अचर पूर्णाङ्क i)
{
    CV_DECL_ALIGNED(32) पूर्णाङ्क reg_data[8];
    CV_DbgAssert(0 <= i && i < 8);
    _mm256_store_si256((__m256i*)reg_data, reg);
    फर्काऊ reg_data[i];
}
#endif

पूर्णाङ्क normHamming(अचर uchar* a, पूर्णाङ्क n)
{
    CV_AVX_GUARD;

    पूर्णाङ्क i = 0;
    पूर्णाङ्क result = 0;

#यदि (CV_SIMD || CV_SIMD_SCALABLE)
    {
        v_uint64 t = vx_setzero_u64();
        लागि (; i <= n - VTraits<v_uint8>::vlanes(); i += VTraits<v_uint8>::vlanes())
            t = v_add(t, v_popcount(v_reinterpret_as_u64(vx_load(a + i))));
        result = (पूर्णाङ्क)v_reduce_sum(t);
        vx_cleanup();
    }
#endif

#यदि CV_POPCNT
    {
#  यदि defined CV_POPCNT_U64
        लागि(; i <= n - 8; i += 8)
        {
            uint64_t val;
            std::स_नकल(&val, a + i, परिमाण(val));
            result += (पूर्णाङ्क)CV_POPCNT_U64(val);
        }
#  endif
        लागि(; i <= n - 4; i += 4)
        {
            uint32_t val;
            std::स_नकल(&val, a + i, परिमाण(val));
            result += CV_POPCNT_U32(val);
        }
    }
#endif
#यदि CV_ENABLE_UNROLLED
    लागि(; i <= n - 4; i += 4)
    {
        result += झिक_गन्ती_Table[a[i]] + झिक_गन्ती_Table[a[i+1]] +
        झिक_गन्ती_Table[a[i+2]] + झिक_गन्ती_Table[a[i+3]];
    }
#endif
    लागि(; i < n; i++)
    {
        result += झिक_गन्ती_Table[a[i]];
    }
    फर्काऊ result;
}

पूर्णाङ्क normHamming(अचर uchar* a, अचर uchar* b, पूर्णाङ्क n)
{
    CV_AVX_GUARD;

    पूर्णाङ्क i = 0;
    पूर्णाङ्क result = 0;

#यदि (CV_SIMD || CV_SIMD_SCALABLE)
    {
        v_uint64 t = vx_setzero_u64();
        लागि (; i <= n - VTraits<v_uint8>::vlanes(); i += VTraits<v_uint8>::vlanes())
            t = v_add(t, v_popcount(v_reinterpret_as_u64(v_xor(vx_load(a + i), vx_load(b + i)))));
        result += (पूर्णाङ्क)v_reduce_sum(t);
    }
#endif

#यदि CV_POPCNT
    {
#  यदि defined CV_POPCNT_U64
        लागि(; i <= n - 8; i += 8)
        {
            uint64_t val_a, val_b;
            std::स_नकल(&val_a, a + i, परिमाण(val_a));
            std::स_नकल(&val_b, b + i, परिमाण(val_b));
            result += (पूर्णाङ्क)CV_POPCNT_U64(val_a ^ val_b);
        }
#  endif
        लागि(; i <= n - 4; i += 4)
        {
            uint32_t val_a, val_b;
            std::स_नकल(&val_a, a + i, परिमाण(val_a));
            std::स_नकल(&val_b, b + i, परिमाण(val_b));
            result += (पूर्णाङ्क)CV_POPCNT_U32(val_a ^ val_b);
        }
    }
#endif
#यदि CV_ENABLE_UNROLLED
    लागि(; i <= n - 4; i += 4)
    {
        result += झिक_गन्ती_Table[a[i] ^ b[i]] + झिक_गन्ती_Table[a[i+1] ^ b[i+1]] +
                झिक_गन्ती_Table[a[i+2] ^ b[i+2]] + झिक_गन्ती_Table[a[i+3] ^ b[i+3]];
    }
#endif
    लागि(; i < n; i++)
    {
        result += झिक_गन्ती_Table[a[i] ^ b[i]];
    }
    फर्काऊ result;
}

NormHammingFunc getNormHammingFunc()
{
    NormHammingFunc f = &normHamming; // disambiguate the (a,n) overload
    फर्काऊ f;
}
NormHammingDiffFunc getNormHammingDiffFunc()
{
    NormHammingDiffFunc f = &normHamming; // disambiguate the (a,b,n) overload
    फर्काऊ f;
}

#endif // CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

CV_CPU_OPTIMIZATION_NAMESPACE_END
}} //cv::hal
