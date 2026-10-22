// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.
//
// Copyright (C) 2026, Advanced Micro Devices, Inc., all rights reserved.

#समावेश "opencv2/core/hal/intrin.hpp"

namespace cv { namespace hal {

बाह्य स्थिर uchar निकालो_गिनती_Table[256];

प्रकार पूर्णांक (*NormHammingFunc)(स्थिर uchar*, पूर्णांक);
प्रकार पूर्णांक (*NormHammingDiffFunc)(स्थिर uchar*, स्थिर uchar*, पूर्णांक);

CV_CPU_OPTIMIZATION_NAMESPACE_BEGIN

// forward declarations
पूर्णांक normHamming(स्थिर uchar* a, पूर्णांक n);
पूर्णांक normHamming(स्थिर uchar* a, स्थिर uchar* b, पूर्णांक n);

NormHammingFunc getNormHammingFunc();
NormHammingDiffFunc getNormHammingDiffFunc();

#अगर_अघोषित CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

#अगर CV_AVX2
अटल अंतरभूत पूर्णांक _mm256_extract_epi32_(__m256i reg, स्थिर पूर्णांक i)
{
    CV_DECL_ALIGNED(32) पूर्णांक reg_तथ्य[8];
    CV_DbgAssert(0 <= i && i < 8);
    _mm256_store_si256((__m256i*)reg_तथ्य, reg);
    वापस reg_तथ्य[i];
}
#पूर्ण_अगर

पूर्णांक normHamming(स्थिर uchar* a, पूर्णांक n)
{
    CV_AVX_GUARD;

    पूर्णांक i = 0;
    पूर्णांक परिणाम = 0;

#अगर (CV_SIMD || CV_SIMD_SCALABLE)
    {
        v_uint64 t = vx_setzero_u64();
        क्रम (; i <= n - VTraits<v_uint8>::vlanes(); i += VTraits<v_uint8>::vlanes())
            t = v_add(t, v_popcount(v_reinterpret_as_u64(vx_load(a + i))));
        परिणाम = (पूर्णांक)v_reduce_योग(t);
        vx_cleanup();
    }
#पूर्ण_अगर

#अगर CV_POPCNT
    {
#  अगर defined CV_POPCNT_U64
        क्रम(; i <= n - 8; i += 8)
        {
            uint64_t val;
            std::स_नकल(&val, a + i, माप(val));
            परिणाम += (पूर्णांक)CV_POPCNT_U64(val);
        }
#  endif
        क्रम(; i <= n - 4; i += 4)
        {
            uint32_t val;
            std::स_नकल(&val, a + i, माप(val));
            परिणाम += CV_POPCNT_U32(val);
        }
    }
#पूर्ण_अगर
#अगर CV_ENABLE_UNROLLED
    क्रम(; i <= n - 4; i += 4)
    {
        परिणाम += निकालो_गिनती_Table[a[i]] + निकालो_गिनती_Table[a[i+1]] +
        निकालो_गिनती_Table[a[i+2]] + निकालो_गिनती_Table[a[i+3]];
    }
#पूर्ण_अगर
    क्रम(; i < n; i++)
    {
        परिणाम += निकालो_गिनती_Table[a[i]];
    }
    वापस परिणाम;
}

पूर्णांक normHamming(स्थिर uchar* a, स्थिर uchar* b, पूर्णांक n)
{
    CV_AVX_GUARD;

    पूर्णांक i = 0;
    पूर्णांक परिणाम = 0;

#अगर (CV_SIMD || CV_SIMD_SCALABLE)
    {
        v_uint64 t = vx_setzero_u64();
        क्रम (; i <= n - VTraits<v_uint8>::vlanes(); i += VTraits<v_uint8>::vlanes())
            t = v_add(t, v_popcount(v_reinterpret_as_u64(v_xor(vx_load(a + i), vx_load(b + i)))));
        परिणाम += (पूर्णांक)v_reduce_योग(t);
    }
#पूर्ण_अगर

#अगर CV_POPCNT
    {
#  अगर defined CV_POPCNT_U64
        क्रम(; i <= n - 8; i += 8)
        {
            uint64_t val_a, val_b;
            std::स_नकल(&val_a, a + i, माप(val_a));
            std::स_नकल(&val_b, b + i, माप(val_b));
            परिणाम += (पूर्णांक)CV_POPCNT_U64(val_a ^ val_b);
        }
#  endif
        क्रम(; i <= n - 4; i += 4)
        {
            uint32_t val_a, val_b;
            std::स_नकल(&val_a, a + i, माप(val_a));
            std::स_नकल(&val_b, b + i, माप(val_b));
            परिणाम += (पूर्णांक)CV_POPCNT_U32(val_a ^ val_b);
        }
    }
#पूर्ण_अगर
#अगर CV_ENABLE_UNROLLED
    क्रम(; i <= n - 4; i += 4)
    {
        परिणाम += निकालो_गिनती_Table[a[i] ^ b[i]] + निकालो_गिनती_Table[a[i+1] ^ b[i+1]] +
                निकालो_गिनती_Table[a[i+2] ^ b[i+2]] + निकालो_गिनती_Table[a[i+3] ^ b[i+3]];
    }
#पूर्ण_अगर
    क्रम(; i < n; i++)
    {
        परिणाम += निकालो_गिनती_Table[a[i] ^ b[i]];
    }
    वापस परिणाम;
}

NormHammingFunc getNormHammingFunc()
{
    NormHammingFunc f = &normHamming; // disambiguate the (a,n) overload
    वापस f;
}
NormHammingDiffFunc getNormHammingDiffFunc()
{
    NormHammingDiffFunc f = &normHamming; // disambiguate the (a,b,n) overload
    वापस f;
}

#पूर्ण_अगर // CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

CV_CPU_OPTIMIZATION_NAMESPACE_END
}} //cv::hal
