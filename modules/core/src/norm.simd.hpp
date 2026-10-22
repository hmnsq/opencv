// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html
//
// Copyright (C) 2025, SpaceMIT Inc., all rights reserved.
// Copyright (C) 2026, Advanced Micro Devices, Inc., all rights reserved.
// Third party copyrights are property of their respective owners.

#ಅನ್ತರ್ಭಾವ "precomp.hpp"

ನಾಮಾಕಾಶ cv {

using NormFunc = ಪೂರ್ಣಾಙ್ಕ (*)(ಧ್ರುವ uchar*, ಧ್ರುವ uchar*, uchar*, ಪೂರ್ಣಾಙ್ಕ, ಪೂರ್ಣಾಙ್ಕ);
using NormDiffFunc = ಪೂರ್ಣಾಙ್ಕ (*)(ಧ್ರುವ uchar*, ಧ್ರುವ uchar*, ಧ್ರುವ uchar*, uchar*, ಪೂರ್ಣಾಙ್ಕ, ಪೂರ್ಣಾಙ್ಕ);

CV_CPU_OPTIMIZATION_NAMESPACE_BEGIN

template<typename T, typename RT, ಪೂರ್ಣಾಙ್ಕ (*fn)(ಧ್ರುವ T*, ಧ್ರುವ uchar*, RT*, ಪೂರ್ಣಾಙ್ಕ, ಪೂರ್ಣಾಙ್ಕ)>
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ normWrap(ಧ್ರುವ uchar* src1, ಧ್ರುವ uchar* src2, uchar* res, ಪೂರ್ಣಾಙ್ಕ len, ಪೂರ್ಣಾಙ್ಕ cn)
{
    ಪ್ರತ್ಯಾಗಮ fn((ಧ್ರುವ T*)src1, src2, (RT*)res, len, cn);
}

template<typename T, typename RT, ಪೂರ್ಣಾಙ್ಕ (*fn)(ಧ್ರುವ T*, ಧ್ರುವ T*, ಧ್ರುವ uchar*, RT*, ಪೂರ್ಣಾಙ್ಕ, ಪೂರ್ಣಾಙ್ಕ)>
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ normDiffWrap(ಧ್ರುವ uchar* src1, ಧ್ರುವ uchar* src2, ಧ್ರುವ uchar* src3, uchar* res, ಪೂರ್ಣಾಙ್ಕ len, ಪೂರ್ಣಾಙ್ಕ cn)
{
    ಪ್ರತ್ಯಾಗಮ fn((ಧ್ರುವ T*)src1, (ಧ್ರುವ T*)src2, src3, (RT*)res, len, cn);
}

NormFunc getNormFunc(ಪೂರ್ಣಾಙ್ಕ normType, ಪೂರ್ಣಾಙ್ಕ depth);
NormDiffFunc getNormDiffFunc(ಪೂರ್ಣಾಙ್ಕ normType, ಪೂರ್ಣಾಙ್ಕ depth);

#ifndef CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

template <typename T, typename ST>
ಸಂರಚನಾ NormInf_SIMD {
    ಅನ್ತಃಸ್ಥ ST operator() (ಧ್ರುವ T* src, ಪೂರ್ಣಾಙ್ಕ n) ಧ್ರುವ {
        ST s = 0;
        ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < n; i++) {
            s = std::max(s, (ST)cv_abs(src[i]));
        }
        ಪ್ರತ್ಯಾಗಮ s;
    }
};

template <typename T, typename ST>
ಸಂರಚನಾ NormL1_SIMD {
    ಅನ್ತಃಸ್ಥ ST operator() (ಧ್ರುವ T* src, ಪೂರ್ಣಾಙ್ಕ n) ಧ್ರುವ {
        ST s = 0;
        ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < n; i++) {
            s += cv_abs(src[i]);
        }
        ಪ್ರತ್ಯಾಗಮ s;
    }
};

template <typename T, typename ST>
ಸಂರಚನಾ NormL2_SIMD {
    ಅನ್ತಃಸ್ಥ ST operator() (ಧ್ರುವ T* src, ಪೂರ್ಣಾಙ್ಕ n) ಧ್ರುವ {
        ST s = 0;
        ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < n; i++) {
            ST v = (ST)src[i];
            s += v * v;
        }
        ಪ್ರತ್ಯಾಗಮ s;
    }
};

template <typename T, typename ST>
ಸಂರಚನಾ NormDiffInf_SIMD {
    ಅನ್ತಃಸ್ಥ ST operator() (ಧ್ರುವ T* src1, ಧ್ರುವ T* src2, ಪೂರ್ಣಾಙ್ಕ n) ಧ್ರುವ {
        ST s = 0;
        ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < n; i++) {
            ST v = (ST)cv_absdiff(src1[i], src2[i]);
            s = std::max(s, v);
        }
        ಪ್ರತ್ಯಾಗಮ s;
    }
};

template <typename T, typename ST>
ಸಂರಚನಾ NormDiffL1_SIMD {
    ಅನ್ತಃಸ್ಥ ST operator() (ಧ್ರುವ T* src1, ಧ್ರುವ T* src2, ಪೂರ್ಣಾಙ್ಕ n) ಧ್ರುವ {
        ST s = 0;
        ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < n; i++) {
            ST v = (ST)cv_absdiff(src1[i], src2[i]);
            s += v;
        }
        ಪ್ರತ್ಯಾಗಮ s;
    }
};

template <typename T, typename ST>
ಸಂರಚನಾ NormDiffL2_SIMD {
    ಅನ್ತಃಸ್ಥ ST operator() (ಧ್ರುವ T* src1, ಧ್ರುವ T* src2, ಪೂರ್ಣಾಙ್ಕ n) ಧ್ರುವ {
        ST s = 0;
        ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < n; i++) {
            ST v = (ST)src1[i] - (ST)src2[i];
            s += v * v;
        }
        ಪ್ರತ್ಯಾಗಮ s;
    }
};

#ಯದಿ (CV_SIMD || CV_SIMD_SCALABLE)

template<>
ಸಂರಚನಾ NormInf_SIMD<uchar, ಪೂರ್ಣಾಙ್ಕ> {
    ಪೂರ್ಣಾಙ್ಕ operator() (ಧ್ರುವ uchar* src, ಪೂರ್ಣಾಙ್ಕ n) ಧ್ರುವ {
        ಪೂರ್ಣಾಙ್ಕ j = 0;
        ಪೂರ್ಣಾಙ್ಕ s = 0;
        v_uint8 r0 = vx_setzero_u8(), r1 = vx_setzero_u8();
        v_uint8 r2 = vx_setzero_u8(), r3 = vx_setzero_u8();
        ಪ್ರತಿ (; j <= n - 4 * VTraits<v_uint8>::vlanes(); j += 4 * VTraits<v_uint8>::vlanes()) {
            r0 = v_max(r0, vx_load(src + j                                 ));
            r1 = v_max(r1, vx_load(src + j +     VTraits<v_uint8>::vlanes()));
            r2 = v_max(r2, vx_load(src + j + 2 * VTraits<v_uint8>::vlanes()));
            r3 = v_max(r3, vx_load(src + j + 3 * VTraits<v_uint8>::vlanes()));
        }
        r0 = v_max(r0, v_max(r1, v_max(r2, r3)));
        ಪ್ರತಿ (; j < n; j++) {
            s = std::max(s, (ಪೂರ್ಣಾಙ್ಕ)src[j]);
        }
        ಪ್ರತ್ಯಾಗಮ std::max(s, (ಪೂರ್ಣಾಙ್ಕ)v_reduce_max(r0));
    }
};

template<>
ಸಂರಚನಾ NormInf_SIMD<schar, ಪೂರ್ಣಾಙ್ಕ> {
    ಪೂರ್ಣಾಙ್ಕ operator() (ಧ್ರುವ schar* src, ಪೂರ್ಣಾಙ್ಕ n) ಧ್ರುವ {
        ಪೂರ್ಣಾಙ್ಕ j = 0;
        ಪೂರ್ಣಾಙ್ಕ s = 0;
        v_uint8 r0 = vx_setzero_u8(), r1 = vx_setzero_u8();
        v_uint8 r2 = vx_setzero_u8(), r3 = vx_setzero_u8();
        ಪ್ರತಿ (; j <= n - 4 * VTraits<v_int8>::vlanes(); j += 4 * VTraits<v_int8>::vlanes()) {
            r0 = v_max(r0, v_abs(vx_load(src + j                                )));
            r1 = v_max(r1, v_abs(vx_load(src + j +     VTraits<v_int8>::vlanes())));
            r2 = v_max(r2, v_abs(vx_load(src + j + 2 * VTraits<v_int8>::vlanes())));
            r3 = v_max(r3, v_abs(vx_load(src + j + 3 * VTraits<v_int8>::vlanes())));
        }
        r0 = v_max(r0, v_max(r1, v_max(r2, r3)));
        ಪ್ರತಿ (; j < n; j++) {
            s = std::max(s, cv_abs(src[j]));
        }
        ಪ್ರತ್ಯಾಗಮ std::max(s, saturate_cast<ಪೂರ್ಣಾಙ್ಕ>(v_reduce_max(r0)));
    }
};

template<>
ಸಂರಚನಾ NormInf_SIMD<ushort, ಪೂರ್ಣಾಙ್ಕ> {
    ಪೂರ್ಣಾಙ್ಕ operator() (ಧ್ರುವ ushort* src, ಪೂರ್ಣಾಙ್ಕ n) ಧ್ರುವ {
        ಪೂರ್ಣಾಙ್ಕ j = 0;
        ಪೂರ್ಣಾಙ್ಕ s = 0;
        v_uint16 d0 = vx_setzero_u16(), d1 = vx_setzero_u16();
        v_uint16 d2 = vx_setzero_u16(), d3 = vx_setzero_u16();
        ಪ್ರತಿ (; j <= n - 4 * VTraits<v_uint16>::vlanes(); j += 4 * VTraits<v_uint16>::vlanes()) {
            d0 = v_max(d0, vx_load(src + j                                  ));
            d1 = v_max(d1, vx_load(src + j +     VTraits<v_uint16>::vlanes()));
            d2 = v_max(d2, vx_load(src + j + 2 * VTraits<v_uint16>::vlanes()));
            d3 = v_max(d3, vx_load(src + j + 3 * VTraits<v_uint16>::vlanes()));
        }
        d0 = v_max(d0, v_max(d1, v_max(d2, d3)));
        ಪ್ರತಿ (; j < n; j++) {
            s = std::max(s, (ಪೂರ್ಣಾಙ್ಕ)src[j]);
        }
        ಪ್ರತ್ಯಾಗಮ std::max(s, (ಪೂರ್ಣಾಙ್ಕ)v_reduce_max(d0));
    }
};

template<>
ಸಂರಚನಾ NormInf_SIMD<short, ಪೂರ್ಣಾಙ್ಕ> {
    ಪೂರ್ಣಾಙ್ಕ operator() (ಧ್ರುವ short* src, ಪೂರ್ಣಾಙ್ಕ n) ಧ್ರುವ {
        ಪೂರ್ಣಾಙ್ಕ j = 0;
        ಪೂರ್ಣಾಙ್ಕ s = 0;
        v_uint16 d0 = vx_setzero_u16(), d1 = vx_setzero_u16();
        v_uint16 d2 = vx_setzero_u16(), d3 = vx_setzero_u16();
        ಪ್ರತಿ (; j <= n - 4 * VTraits<v_int16>::vlanes(); j += 4 * VTraits<v_int16>::vlanes()) {
            d0 = v_max(d0, v_abs(vx_load(src + j                                  )));
            d1 = v_max(d1, v_abs(vx_load(src + j +     VTraits<v_int16>::vlanes())));
            d2 = v_max(d2, v_abs(vx_load(src + j + 2 * VTraits<v_int16>::vlanes())));
            d3 = v_max(d3, v_abs(vx_load(src + j + 3 * VTraits<v_int16>::vlanes())));
        }
        d0 = v_max(d0, v_max(d1, v_max(d2, d3)));
        ಪ್ರತಿ (; j < n; j++) {
            s = std::max(s, saturate_cast<ಪೂರ್ಣಾಙ್ಕ>(cv_abs(src[j])));
        }
        ಪ್ರತ್ಯಾಗಮ std::max(s, saturate_cast<ಪೂರ್ಣಾಙ್ಕ>(v_reduce_max(d0)));
    }
};

template<>
ಸಂರಚನಾ NormInf_SIMD<ಪೂರ್ಣಾಙ್ಕ, ಪೂರ್ಣಾಙ್ಕ> {
    ಪೂರ್ಣಾಙ್ಕ operator() (ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ* src, ಪೂರ್ಣಾಙ್ಕ n) ಧ್ರುವ {
        ಪೂರ್ಣಾಙ್ಕ j = 0;
        ಪೂರ್ಣಾಙ್ಕ s = 0;
        v_uint32 r0 = vx_setzero_u32(), r1 = vx_setzero_u32();
        v_uint32 r2 = vx_setzero_u32(), r3 = vx_setzero_u32();
        ಪ್ರತಿ (; j <= n - 4 * VTraits<v_int32>::vlanes(); j += 4 * VTraits<v_int32>::vlanes()) {
            r0 = v_max(r0, v_abs(vx_load(src + j                                 )));
            r1 = v_max(r1, v_abs(vx_load(src + j +     VTraits<v_int32>::vlanes())));
            r2 = v_max(r2, v_abs(vx_load(src + j + 2 * VTraits<v_int32>::vlanes())));
            r3 = v_max(r3, v_abs(vx_load(src + j + 3 * VTraits<v_int32>::vlanes())));
        }
        r0 = v_max(r0, v_max(r1, v_max(r2, r3)));
        s = std::max(s, saturate_cast<ಪೂರ್ಣಾಙ್ಕ>(v_reduce_max(r0)));
        ಪ್ರತಿ (; j < n; j++) {
            s = std::max(s, std::abs(src[j]));
        }
        ಪ್ರತ್ಯಾಗಮ s;
    }
};

template<>
ಸಂರಚನಾ NormInf_SIMD<ಭಿನ್ನ, ಭಿನ್ನ> {
    ಭಿನ್ನ operator() (ಧ್ರುವ ಭಿನ್ನ* src, ಪೂರ್ಣಾಙ್ಕ n) ಧ್ರುವ {
        ಪೂರ್ಣಾಙ್ಕ j = 0;
        ಭಿನ್ನ s = 0.f;
        v_float32 r0 = vx_setzero_f32(), r1 = vx_setzero_f32();
        v_float32 r2 = vx_setzero_f32(), r3 = vx_setzero_f32();
        ಪ್ರತಿ (; j <= n - 4 * VTraits<v_float32>::vlanes(); j += 4 * VTraits<v_float32>::vlanes()) {
            r0 = v_max(r0, v_abs(vx_load(src + j                                   )));
            r1 = v_max(r1, v_abs(vx_load(src + j +     VTraits<v_float32>::vlanes())));
            r2 = v_max(r2, v_abs(vx_load(src + j + 2 * VTraits<v_float32>::vlanes())));
            r3 = v_max(r3, v_abs(vx_load(src + j + 3 * VTraits<v_float32>::vlanes())));
        }
        r0 = v_max(r0, v_max(r1, v_max(r2, r3)));
        ಪ್ರತಿ (; j < n; j++) {
            s = std::max(s, cv_abs(src[j]));
        }
        ಪ್ರತ್ಯಾಗಮ std::max(s, v_reduce_max(r0));
    }
};

template<>
ಸಂರಚನಾ NormL1_SIMD<uchar, ಪೂರ್ಣಾಙ್ಕ> {
    ಪೂರ್ಣಾಙ್ಕ operator() (ಧ್ರುವ uchar* src, ಪೂರ್ಣಾಙ್ಕ n) ಧ್ರುವ {
        ಪೂರ್ಣಾಙ್ಕ j = 0;
        ಪೂರ್ಣಾಙ್ಕ s = 0;
        v_uint32 r0 = vx_setzero_u32(), r1 = vx_setzero_u32();
        v_uint8  one = vx_setall_u8(1);
        ಪ್ರತಿ (; j<= n - 2 * VTraits<v_uint8>::vlanes(); j += 2 * VTraits<v_uint8>::vlanes()) {
            v_uint8 v0 = vx_load(src + j);
            r0 = v_dotprod_expand_fast(v0, one, r0);

            v_uint8 v1 = vx_load(src + j + VTraits<v_uint8>::vlanes());
            r1 = v_dotprod_expand_fast(v1, one, r1);
        }
        s += v_reduce_sum(v_add(r0, r1));
        ಪ್ರತಿ (; j < n; j++) {
            s += src[j];
        }
        ಪ್ರತ್ಯಾಗಮ s;
    }
};

template<>
ಸಂರಚನಾ NormL1_SIMD<schar, ಪೂರ್ಣಾಙ್ಕ> {
    ಪೂರ್ಣಾಙ್ಕ operator() (ಧ್ರುವ schar* src, ಪೂರ್ಣಾಙ್ಕ n) ಧ್ರುವ {
        ಪೂರ್ಣಾಙ್ಕ j = 0;
        ಪೂರ್ಣಾಙ್ಕ s = 0;
        v_uint32 r0 = vx_setzero_u32(), r1 = vx_setzero_u32();
        v_uint8  one = vx_setall_u8(1);
        ಪ್ರತಿ (; j<= n - 2 * VTraits<v_int8>::vlanes(); j += 2 * VTraits<v_int8>::vlanes()) {
            v_uint8 v0 = v_abs(vx_load(src + j));
            r0 = v_dotprod_expand_fast(v0, one, r0);

            v_uint8 v1 = v_abs(vx_load(src + j + VTraits<v_int8>::vlanes()));
            r1 = v_dotprod_expand_fast(v1, one, r1);
        }
        s += v_reduce_sum(v_add(r0, r1));
        ಪ್ರತಿ (; j < n; j++) {
            s += saturate_cast<ಪೂರ್ಣಾಙ್ಕ>(cv_abs(src[j]));
        }
        ಪ್ರತ್ಯಾಗಮ s;
    }
};

template<>
ಸಂರಚನಾ NormL1_SIMD<ushort, ಪೂರ್ಣಾಙ್ಕ> {
    ಪೂರ್ಣಾಙ್ಕ operator() (ಧ್ರುವ ushort* src, ಪೂರ್ಣಾಙ್ಕ n) ಧ್ರುವ {
        ಪೂರ್ಣಾಙ್ಕ j = 0;
        ಪೂರ್ಣಾಙ್ಕ s = 0;
        v_uint32 r00 = vx_setzero_u32(), r01 = vx_setzero_u32();
        v_uint32 r10 = vx_setzero_u32(), r11 = vx_setzero_u32();
        ಪ್ರತಿ (; j<= n - 2 * VTraits<v_uint16>::vlanes(); j += 2 * VTraits<v_uint16>::vlanes()) {
            v_uint16 v0 = vx_load(src + j);
            v_uint32 v00, v01;
            v_expand(v0, v00, v01);
            r00 = v_add(r00, v00);
            r01 = v_add(r01, v01);

            v_uint16 v1 = vx_load(src + j + VTraits<v_uint16>::vlanes());
            v_uint32 v10, v11;
            v_expand(v1, v10, v11);
            r10 = v_add(r10, v10);
            r11 = v_add(r11, v11);
        }
        s += (ಪೂರ್ಣಾಙ್ಕ)v_reduce_sum(v_add(v_add(v_add(r00, r01), r10), r11));
        ಪ್ರತಿ (; j < n; j++) {
            s += src[j];
        }
        ಪ್ರತ್ಯಾಗಮ s;
    }
};

template<>
ಸಂರಚನಾ NormL1_SIMD<short, ಪೂರ್ಣಾಙ್ಕ> {
    ಪೂರ್ಣಾಙ್ಕ operator() (ಧ್ರುವ short* src, ಪೂರ್ಣಾಙ್ಕ n) ಧ್ರುವ {
        ಪೂರ್ಣಾಙ್ಕ j = 0;
        ಪೂರ್ಣಾಙ್ಕ s = 0;
        v_uint32 r00 = vx_setzero_u32(), r01 = vx_setzero_u32();
        v_uint32 r10 = vx_setzero_u32(), r11 = vx_setzero_u32();
        ಪ್ರತಿ (; j<= n - 2 * VTraits<v_int16>::vlanes(); j += 2 * VTraits<v_int16>::vlanes()) {
            v_uint16 v0 = v_abs(vx_load(src + j));
            v_uint32 v00, v01;
            v_expand(v0, v00, v01);
            r00 = v_add(r00, v00);
            r01 = v_add(r01, v01);

            v_uint16 v1 = v_abs(vx_load(src + j + VTraits<v_int16>::vlanes()));
            v_uint32 v10, v11;
            v_expand(v1, v10, v11);
            r10 = v_add(r10, v10);
            r11 = v_add(r11, v11);
        }
        s += (ಪೂರ್ಣಾಙ್ಕ)v_reduce_sum(v_add(v_add(v_add(r00, r01), r10), r11));
        ಪ್ರತಿ (; j < n; j++) {
            s += saturate_cast<ಪೂರ್ಣಾಙ್ಕ>(cv_abs(src[j]));
        }
        ಪ್ರತ್ಯಾಗಮ s;
    }
};

template<>
ಸಂರಚನಾ NormL2_SIMD<uchar, ಪೂರ್ಣಾಙ್ಕ> {
    ಪೂರ್ಣಾಙ್ಕ operator() (ಧ್ರುವ uchar* src, ಪೂರ್ಣಾಙ್ಕ n) ಧ್ರುವ {
        ಪೂರ್ಣಾಙ್ಕ j = 0;
        ಪೂರ್ಣಾಙ್ಕ s = 0;
        v_uint32 r0 = vx_setzero_u32(), r1 = vx_setzero_u32();
        ಪ್ರತಿ (; j <= n - 2 * VTraits<v_uint8>::vlanes(); j += 2 * VTraits<v_uint8>::vlanes()) {
            v_uint8 v0 = vx_load(src + j);
            r0 = v_dotprod_expand_fast(v0, v0, r0);

            v_uint8 v1 = vx_load(src + j + VTraits<v_uint8>::vlanes());
            r1 = v_dotprod_expand_fast(v1, v1, r1);
        }
        s += v_reduce_sum(v_add(r0, r1));
        ಪ್ರತಿ (; j < n; j++) {
            ಪೂರ್ಣಾಙ್ಕ v = saturate_cast<ಪೂರ್ಣಾಙ್ಕ>(src[j]);
            s += v * v;
        }
        ಪ್ರತ್ಯಾಗಮ s;
    }
};

template<>
ಸಂರಚನಾ NormL2_SIMD<schar, ಪೂರ್ಣಾಙ್ಕ> {
    ಪೂರ್ಣಾಙ್ಕ operator() (ಧ್ರುವ schar* src, ಪೂರ್ಣಾಙ್ಕ n) ಧ್ರುವ {
        ಪೂರ್ಣಾಙ್ಕ j = 0;
        ಪೂರ್ಣಾಙ್ಕ s = 0;
        v_int32 r0 = vx_setzero_s32(), r1 = vx_setzero_s32();
        ಪ್ರತಿ (; j <= n - 2 * VTraits<v_int8>::vlanes(); j += 2 * VTraits<v_int8>::vlanes()) {
            v_int8 v0 = vx_load(src + j);
            r0 = v_dotprod_expand_fast(v0, v0, r0);
            v_int8 v1 = vx_load(src + j + VTraits<v_int8>::vlanes());
            r1 = v_dotprod_expand_fast(v1, v1, r1);
        }
        s += v_reduce_sum(v_add(r0, r1));
        ಪ್ರತಿ (; j < n; j++) {
            ಪೂರ್ಣಾಙ್ಕ v = saturate_cast<ಪೂರ್ಣಾಙ್ಕ>(src[j]);
            s += v * v;
        }
        ಪ್ರತ್ಯಾಗಮ s;
    }
};

template<>
ಸಂರಚನಾ NormDiffInf_SIMD<uchar, ಪೂರ್ಣಾಙ್ಕ> {
    ಪೂರ್ಣಾಙ್ಕ operator() (ಧ್ರುವ uchar* src1, ಧ್ರುವ uchar* src2, ಪೂರ್ಣಾಙ್ಕ n) ಧ್ರುವ {
        ಪೂರ್ಣಾಙ್ಕ j = 0;
        ಪೂರ್ಣಾಙ್ಕ s = 0;
        v_uint8 r0 = vx_setzero_u8(), r1 = vx_setzero_u8();
        v_uint8 r2 = vx_setzero_u8(), r3 = vx_setzero_u8();
        ಪ್ರತಿ (; j <= n - 4 * VTraits<v_uint8>::vlanes(); j += 4 * VTraits<v_uint8>::vlanes()) {
            v_uint8 v01 = vx_load(src1 + j), v02 = vx_load(src2 + j);
            r0 = v_max(r0, v_absdiff(v01, v02));

            v_uint8 v11 = vx_load(src1 + j + VTraits<v_uint8>::vlanes()),
                    v12 = vx_load(src2 + j + VTraits<v_uint8>::vlanes());
            r1 = v_max(r1, v_absdiff(v11, v12));

            v_uint8 v21 = vx_load(src1 + j + 2 * VTraits<v_uint8>::vlanes()),
                    v22 = vx_load(src2 + j + 2 * VTraits<v_uint8>::vlanes());
            r2 = v_max(r2, v_absdiff(v21, v22));

            v_uint8 v31 = vx_load(src1 + j + 3 * VTraits<v_uint8>::vlanes()),
                    v32 = vx_load(src2 + j + 3 * VTraits<v_uint8>::vlanes());
            r3 = v_max(r3, v_absdiff(v31, v32));
        }
        s = (ಪೂರ್ಣಾಙ್ಕ)v_reduce_max(v_max(v_max(v_max(r0, r1), r2), r3));
        ಪ್ರತಿ (; j < n; j++) {
            ಪೂರ್ಣಾಙ್ಕ v = (ಪೂರ್ಣಾಙ್ಕ)cv_absdiff(src1[j], src2[j]);
            s = std::max(s, v);
        }
        ಪ್ರತ್ಯಾಗಮ s;
    }
};

template<>
ಸಂರಚನಾ NormDiffInf_SIMD<schar, ಪೂರ್ಣಾಙ್ಕ> {
    ಪೂರ್ಣಾಙ್ಕ operator() (ಧ್ರುವ schar* src1, ಧ್ರುವ schar* src2, ಪೂರ್ಣಾಙ್ಕ n) ಧ್ರುವ {
        ಪೂರ್ಣಾಙ್ಕ j = 0;
        ಪೂರ್ಣಾಙ್ಕ s = 0;
        v_uint8 r0 = vx_setzero_u8(), r1 = vx_setzero_u8();
        v_uint8 r2 = vx_setzero_u8(), r3 = vx_setzero_u8();
        ಪ್ರತಿ (; j <= n - 4 * VTraits<v_int8>::vlanes(); j += 4 * VTraits<v_int8>::vlanes()) {
            v_int8 v01 = vx_load(src1 + j), v02 = vx_load(src2 + j);
            r0 = v_max(r0, v_absdiff(v01, v02));

            v_int8 v11 = vx_load(src1 + j + VTraits<v_int8>::vlanes()),
                   v12 = vx_load(src2 + j + VTraits<v_int8>::vlanes());
            r1 = v_max(r1, v_absdiff(v11, v12));

            v_int8 v21 = vx_load(src1 + j + 2 * VTraits<v_int8>::vlanes()),
                   v22 = vx_load(src2 + j + 2 * VTraits<v_int8>::vlanes());
            r2 = v_max(r2, v_absdiff(v21, v22));

            v_int8 v31 = vx_load(src1 + j + 3 * VTraits<v_int8>::vlanes()),
                   v32 = vx_load(src2 + j + 3 * VTraits<v_int8>::vlanes());
            r3 = v_max(r3, v_absdiff(v31, v32));
        }
        s = (ಪೂರ್ಣಾಙ್ಕ)v_reduce_max(v_max(v_max(v_max(r0, r1), r2), r3));
        ಪ್ರತಿ (; j < n; j++) {
            ಪೂರ್ಣಾಙ್ಕ v = (ಪೂರ್ಣಾಙ್ಕ)cv_absdiff(src1[j], src2[j]);
            s = std::max(s, v);
        }
        ಪ್ರತ್ಯಾಗಮ s;
    }
};

template<>
ಸಂರಚನಾ NormDiffInf_SIMD<ushort, ಪೂರ್ಣಾಙ್ಕ> {
    ಪೂರ್ಣಾಙ್ಕ operator() (ಧ್ರುವ ushort* src1, ಧ್ರುವ ushort* src2, ಪೂರ್ಣಾಙ್ಕ n) ಧ್ರುವ {
        ಪೂರ್ಣಾಙ್ಕ j = 0;
        ಪೂರ್ಣಾಙ್ಕ s = 0;
        v_uint16 r0 = vx_setzero_u16(), r1 = vx_setzero_u16();
        v_uint16 r2 = vx_setzero_u16(), r3 = vx_setzero_u16();
        ಪ್ರತಿ (; j <= n - 4 * VTraits<v_uint16>::vlanes(); j += 4 * VTraits<v_uint16>::vlanes()) {
            v_uint16 v01 = vx_load(src1 + j), v02 = vx_load(src2 + j);
            r0 = v_max(r0, v_absdiff(v01, v02));

            v_uint16 v11 = vx_load(src1 + j + VTraits<v_uint16>::vlanes()),
                     v12 = vx_load(src2 + j + VTraits<v_uint16>::vlanes());
            r1 = v_max(r1, v_absdiff(v11, v12));

            v_uint16 v21 = vx_load(src1 + j + 2 * VTraits<v_uint16>::vlanes()),
                     v22 = vx_load(src2 + j + 2 * VTraits<v_uint16>::vlanes());
            r2 = v_max(r2, v_absdiff(v21, v22));

            v_uint16 v31 = vx_load(src1 + j + 3 * VTraits<v_uint16>::vlanes()),
                     v32 = vx_load(src2 + j + 3 * VTraits<v_uint16>::vlanes());
            r3 = v_max(r3, v_absdiff(v31, v32));
        }
        s = (ಪೂರ್ಣಾಙ್ಕ)v_reduce_max(v_max(v_max(v_max(r0, r1), r2), r3));
        ಪ್ರತಿ (; j < n; j++) {
            ಪೂರ್ಣಾಙ್ಕ v = (ಪೂರ್ಣಾಙ್ಕ)cv_absdiff(src1[j], src2[j]);
            s = std::max(s, v);
        }
        ಪ್ರತ್ಯಾಗಮ s;
    }
};

template<>
ಸಂರಚನಾ NormDiffInf_SIMD<short, ಪೂರ್ಣಾಙ್ಕ> {
    ಪೂರ್ಣಾಙ್ಕ operator() (ಧ್ರುವ short* src1, ಧ್ರುವ short* src2, ಪೂರ್ಣಾಙ್ಕ n) ಧ್ರುವ {
        ಪೂರ್ಣಾಙ್ಕ j = 0;
        ಪೂರ್ಣಾಙ್ಕ s = 0;
        v_uint16 r0 = vx_setzero_u16(), r1 = vx_setzero_u16();
        v_uint16 r2 = vx_setzero_u16(), r3 = vx_setzero_u16();
        ಪ್ರತಿ (; j <= n - 4 * VTraits<v_int16>::vlanes(); j += 4 * VTraits<v_int16>::vlanes()) {
            v_int16 v01 = vx_load(src1 + j), v02 = vx_load(src2 + j);
            r0 = v_max(r0, v_absdiff(v01, v02));

            v_int16 v11 = vx_load(src1 + j + VTraits<v_int16>::vlanes()),
                    v12 = vx_load(src2 + j + VTraits<v_int16>::vlanes());
            r1 = v_max(r1, v_absdiff(v11, v12));

            v_int16 v21 = vx_load(src1 + j + 2 * VTraits<v_int16>::vlanes()),
                    v22 = vx_load(src2 + j + 2 * VTraits<v_int16>::vlanes());
            r2 = v_max(r2, v_absdiff(v21, v22));

            v_int16 v31 = vx_load(src1 + j + 3 * VTraits<v_int16>::vlanes()),
                    v32 = vx_load(src2 + j + 3 * VTraits<v_int16>::vlanes());
            r3 = v_max(r3, v_absdiff(v31, v32));
        }
        s = (ಪೂರ್ಣಾಙ್ಕ)v_reduce_max(v_max(v_max(v_max(r0, r1), r2), r3));
        ಪ್ರತಿ (; j < n; j++) {
            ಪೂರ್ಣಾಙ್ಕ v = (ಪೂರ್ಣಾಙ್ಕ)cv_absdiff(src1[j], src2[j]);
            s = std::max(s, v);
        }
        ಪ್ರತ್ಯಾಗಮ s;
    }
};

template<>
ಸಂರಚನಾ NormDiffInf_SIMD<ಪೂರ್ಣಾಙ್ಕ, unsigned> {
    unsigned operator() (ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ* src1, ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ* src2, ಪೂರ್ಣಾಙ್ಕ n) ಧ್ರುವ {
        ಪೂರ್ಣಾಙ್ಕ j = 0;
        unsigned s = 0;
        v_uint32 r0 = vx_setzero_u32(), r1 = vx_setzero_u32();
        v_uint32 r2 = vx_setzero_u32(), r3 = vx_setzero_u32();
        ಪ್ರತಿ (; j <= n - 4 * VTraits<v_int32>::vlanes(); j += 4 * VTraits<v_int32>::vlanes()) {
            v_int32 v01 = vx_load(src1 + j), v02 = vx_load(src2 + j);
            r0 = v_max(r0, v_absdiff(v01, v02));

            v_int32 v11 = vx_load(src1 + j + VTraits<v_int32>::vlanes()),
                    v12 = vx_load(src2 + j + VTraits<v_int32>::vlanes());
            r1 = v_max(r1, v_absdiff(v11, v12));

            v_int32 v21 = vx_load(src1 + j + 2 * VTraits<v_int32>::vlanes()),
                    v22 = vx_load(src2 + j + 2 * VTraits<v_int32>::vlanes());
            r2 = v_max(r2, v_absdiff(v21, v22));

            v_int32 v31 = vx_load(src1 + j + 3 * VTraits<v_int32>::vlanes()),
                    v32 = vx_load(src2 + j + 3 * VTraits<v_int32>::vlanes());
            r3 = v_max(r3, v_absdiff(v31, v32));
        }
        s = (ಪೂರ್ಣಾಙ್ಕ)v_reduce_max(v_max(v_max(v_max(r0, r1), r2), r3));
        ಪ್ರತಿ (; j < n; j++) {
            unsigned v = (unsigned)cv_absdiff(src1[j], src2[j]);
            s = std::max(s, v);
        }
        ಪ್ರತ್ಯಾಗಮ s;
    }
};

template<>
ಸಂರಚನಾ NormDiffInf_SIMD<ಭಿನ್ನ, ಭಿನ್ನ> {
    ಭಿನ್ನ operator() (ಧ್ರುವ ಭಿನ್ನ* src1, ಧ್ರುವ ಭಿನ್ನ* src2, ಪೂರ್ಣಾಙ್ಕ n) ಧ್ರುವ {
        ಪೂರ್ಣಾಙ್ಕ j = 0;
        ಭಿನ್ನ s = 0;
        v_float32 r0 = vx_setzero_f32(), r1 = vx_setzero_f32();
        ಪ್ರತಿ (; j <= n - 2 * VTraits<v_float32>::vlanes(); j += 2 * VTraits<v_float32>::vlanes()) {
            v_float32 v01 = vx_load(src1 + j), v02 = vx_load(src2 + j);
            r0 = v_max(r0, v_absdiff(v01, v02));

            v_float32 v11 = vx_load(src1 + j + VTraits<v_float32>::vlanes()),
                      v12 = vx_load(src2 + j + VTraits<v_float32>::vlanes());
            r1 = v_max(r1, v_absdiff(v11, v12));
        }
        s = v_reduce_max(v_max(r0, r1));
        ಪ್ರತಿ (; j < n; j++) {
            ಭಿನ್ನ v = (ಭಿನ್ನ)cv_absdiff(src1[j], src2[j]);
            s = std::max(s, v);
        }
        ಪ್ರತ್ಯಾಗಮ s;
    }
};

template<>
ಸಂರಚನಾ NormDiffL1_SIMD<uchar, ಪೂರ್ಣಾಙ್ಕ> {
    ಪೂರ್ಣಾಙ್ಕ operator() (ಧ್ರುವ uchar* src1, ಧ್ರುವ uchar* src2, ಪೂರ್ಣಾಙ್ಕ n) ಧ್ರುವ {
        ಪೂರ್ಣಾಙ್ಕ j = 0;
        ಪೂರ್ಣಾಙ್ಕ s = 0;
        v_uint32 r0 = vx_setzero_u32(), r1 = vx_setzero_u32();
        v_uint8  one = vx_setall_u8(1);
        ಪ್ರತಿ (; j<= n - 2 * VTraits<v_uint8>::vlanes(); j += 2 * VTraits<v_uint8>::vlanes()) {
            v_uint8 v01 = vx_load(src1 + j), v02 = vx_load(src2 + j);
            r0 = v_dotprod_expand_fast(v_absdiff(v01, v02), one, r0);

            v_uint8 v11 = vx_load(src1 + j + VTraits<v_uint8>::vlanes()),
                    v12 = vx_load(src2 + j + VTraits<v_uint8>::vlanes());
            r1 = v_dotprod_expand_fast(v_absdiff(v11, v12), one, r1);
        }
        s += v_reduce_sum(v_add(r0, r1));
        ಪ್ರತಿ (; j < n; j++) {
            ಪೂರ್ಣಾಙ್ಕ v = (ಪೂರ್ಣಾಙ್ಕ)cv_absdiff(src1[j], src2[j]);
            s += v;
        }
        ಪ್ರತ್ಯಾಗಮ s;
    }
};

template<>
ಸಂರಚನಾ NormDiffL1_SIMD<schar, ಪೂರ್ಣಾಙ್ಕ> {
    ಪೂರ್ಣಾಙ್ಕ operator() (ಧ್ರುವ schar* src1, ಧ್ರುವ schar* src2, ಪೂರ್ಣಾಙ್ಕ n) ಧ್ರುವ {
        ಪೂರ್ಣಾಙ್ಕ j = 0;
        ಪೂರ್ಣಾಙ್ಕ s = 0;
        v_uint32 r0 = vx_setzero_u32(), r1 = vx_setzero_u32();
        v_uint8  one = vx_setall_u8(1);
        ಪ್ರತಿ (; j<= n - 2 * VTraits<v_int8>::vlanes(); j += 2 * VTraits<v_int8>::vlanes()) {
            v_int8 v01 = vx_load(src1 + j), v02 = vx_load(src2 + j);
            r0 = v_dotprod_expand_fast(v_absdiff(v01, v02), one, r0);

            v_int8 v11 = vx_load(src1 + j + VTraits<v_int8>::vlanes()),
                   v12 = vx_load(src2 + j + VTraits<v_int8>::vlanes());
            r1 = v_dotprod_expand_fast(v_absdiff(v11, v12), one, r1);
        }
        s += v_reduce_sum(v_add(r0, r1));
        ಪ್ರತಿ (; j < n; j++) {
            ಪೂರ್ಣಾಙ್ಕ v = (ಪೂರ್ಣಾಙ್ಕ)cv_absdiff(src1[j], src2[j]);
            s += v;
        }
        ಪ್ರತ್ಯಾಗಮ s;
    }
};

template<>
ಸಂರಚನಾ NormDiffL1_SIMD<ushort, ಪೂರ್ಣಾಙ್ಕ> {
    ಪೂರ್ಣಾಙ್ಕ operator() (ಧ್ರುವ ushort* src1, ಧ್ರುವ ushort* src2, ಪೂರ್ಣಾಙ್ಕ n) ಧ್ರುವ {
        ಪೂರ್ಣಾಙ್ಕ j = 0;
        ಪೂರ್ಣಾಙ್ಕ s = 0;
        v_uint32 r0 = vx_setzero_u32(), r1 = vx_setzero_u32();
        v_uint32 r2 = vx_setzero_u32(), r3 = vx_setzero_u32();
        ಪ್ರತಿ (; j<= n - 4 * VTraits<v_uint16>::vlanes(); j += 4 * VTraits<v_uint16>::vlanes()) {
            v_uint16 v01 = vx_load(src1 + j), v02 = vx_load(src2 + j);
            v_uint32 u00, u01;
            v_expand(v_absdiff(v01, v02), u00, u01);
            r0 = v_add(r0, v_add(u00, u01));

            v_uint16 v11 = vx_load(src1 + j + VTraits<v_uint16>::vlanes()),
                     v12 = vx_load(src2 + j + VTraits<v_uint16>::vlanes());
            v_uint32 u10, u11;
            v_expand(v_absdiff(v11, v12), u10, u11);
            r1 = v_add(r1, v_add(u10, u11));

            v_uint16 v21 = vx_load(src1 + j + 2 * VTraits<v_uint16>::vlanes()),
                     v22 = vx_load(src2 + j + 2 * VTraits<v_uint16>::vlanes());
            v_uint32 u20, u21;
            v_expand(v_absdiff(v21, v22), u20, u21);
            r2 = v_add(r2, v_add(u20, u21));

            v_uint16 v31 = vx_load(src1 + j + 3 * VTraits<v_uint16>::vlanes()),
                     v32 = vx_load(src2 + j + 3 * VTraits<v_uint16>::vlanes());
            v_uint32 u30, u31;
            v_expand(v_absdiff(v31, v32), u30, u31);
            r3 = v_add(r3, v_add(u30, u31));
        }
        s += (ಪೂರ್ಣಾಙ್ಕ)v_reduce_sum(v_add(v_add(v_add(r0, r1), r2), r3));
        ಪ್ರತಿ (; j < n; j++) {
            ಪೂರ್ಣಾಙ್ಕ v = (ಪೂರ್ಣಾಙ್ಕ)cv_absdiff(src1[j], src2[j]);
            s += v;
        }
        ಪ್ರತ್ಯಾಗಮ s;
    }
};

template<>
ಸಂರಚನಾ NormDiffL1_SIMD<short, ಪೂರ್ಣಾಙ್ಕ> {
    ಪೂರ್ಣಾಙ್ಕ operator() (ಧ್ರುವ short* src1, ಧ್ರುವ short* src2, ಪೂರ್ಣಾಙ್ಕ n) ಧ್ರುವ {
        ಪೂರ್ಣಾಙ್ಕ j = 0;
        ಪೂರ್ಣಾಙ್ಕ s = 0;
        v_uint32 r0 = vx_setzero_u32(), r1 = vx_setzero_u32();
        v_uint32 r2 = vx_setzero_u32(), r3 = vx_setzero_u32();
        ಪ್ರತಿ (; j<= n - 4 * VTraits<v_int16>::vlanes(); j += 4 * VTraits<v_int16>::vlanes()) {
            v_int16 v01 = vx_load(src1 + j), v02 = vx_load(src2 + j);
            v_uint32 u00, u01;
            v_expand(v_absdiff(v01, v02), u00, u01);
            r0 = v_add(r0, v_add(u00, u01));

            v_int16 v11 = vx_load(src1 + j + VTraits<v_int16>::vlanes()),
                    v12 = vx_load(src2 + j + VTraits<v_int16>::vlanes());
            v_uint32 u10, u11;
            v_expand(v_absdiff(v11, v12), u10, u11);
            r1 = v_add(r1, v_add(u10, u11));

            v_int16 v21 = vx_load(src1 + j + 2 * VTraits<v_int16>::vlanes()),
                    v22 = vx_load(src2 + j + 2 * VTraits<v_int16>::vlanes());
            v_uint32 u20, u21;
            v_expand(v_absdiff(v21, v22), u20, u21);
            r2 = v_add(r2, v_add(u20, u21));

            v_int16 v31 = vx_load(src1 + j + 3 * VTraits<v_int16>::vlanes()),
                    v32 = vx_load(src2 + j + 3 * VTraits<v_int16>::vlanes());
            v_uint32 u30, u31;
            v_expand(v_absdiff(v31, v32), u30, u31);
            r3 = v_add(r3, v_add(u30, u31));
        }
        s += (ಪೂರ್ಣಾಙ್ಕ)v_reduce_sum(v_add(v_add(v_add(r0, r1), r2), r3));
        ಪ್ರತಿ (; j < n; j++) {
            ಪೂರ್ಣಾಙ್ಕ v = (ಪೂರ್ಣಾಙ್ಕ)cv_absdiff(src1[j], src2[j]);
            s += v;
        }
        ಪ್ರತ್ಯಾಗಮ s;
    }
};

template<>
ಸಂರಚನಾ NormDiffL2_SIMD<uchar, ಪೂರ್ಣಾಙ್ಕ> {
    ಪೂರ್ಣಾಙ್ಕ operator() (ಧ್ರುವ uchar* src1, ಧ್ರುವ uchar* src2, ಪೂರ್ಣಾಙ್ಕ n) ಧ್ರುವ {
        ಪೂರ್ಣಾಙ್ಕ j = 0;
        ಪೂರ್ಣಾಙ್ಕ s = 0;
        v_uint32 r0 = vx_setzero_u32(), r1 = vx_setzero_u32();
        ಪ್ರತಿ (; j <= n - 2 * VTraits<v_uint8>::vlanes(); j += 2 * VTraits<v_uint8>::vlanes()) {
            v_uint8 v01 = vx_load(src1 + j), v02 = vx_load(src2 + j);
            v_uint8 v0 = v_absdiff(v01, v02);
            r0 = v_dotprod_expand_fast(v0, v0, r0);

            v_uint8 v11 = vx_load(src1 + j + VTraits<v_uint8>::vlanes()),
                    v12 = vx_load(src2 + j + VTraits<v_uint8>::vlanes());
            v_uint8 v1 = v_absdiff(v11, v12);
            r1 = v_dotprod_expand_fast(v1, v1, r1);
        }
        s += v_reduce_sum(v_add(r0, r1));
        ಪ್ರತಿ (; j < n; j++) {
            ಪೂರ್ಣಾಙ್ಕ v = (ಪೂರ್ಣಾಙ್ಕ)src1[j] - (ಪೂರ್ಣಾಙ್ಕ)src2[j];
            s += v * v;
        }
        ಪ್ರತ್ಯಾಗಮ s;
    }
};

template<>
ಸಂರಚನಾ NormDiffL2_SIMD<schar, ಪೂರ್ಣಾಙ್ಕ> {
    ಪೂರ್ಣಾಙ್ಕ operator() (ಧ್ರುವ schar* src1, ಧ್ರುವ schar* src2, ಪೂರ್ಣಾಙ್ಕ n) ಧ್ರುವ {
        ಪೂರ್ಣಾಙ್ಕ j = 0;
        ಪೂರ್ಣಾಙ್ಕ s = 0;
        v_uint32 r0 = vx_setzero_u32(), r1 = vx_setzero_u32();
        ಪ್ರತಿ (; j <= n - 2 * VTraits<v_int8>::vlanes(); j += 2 * VTraits<v_int8>::vlanes()) {
            v_int8 v01 = vx_load(src1 + j), v02 = vx_load(src2 + j);
            v_uint8 v0 = v_absdiff(v01, v02);
            r0 = v_dotprod_expand_fast(v0, v0, r0);

            v_int8 v11 = vx_load(src1 + j + VTraits<v_int8>::vlanes()),
                   v12 = vx_load(src2 + j + VTraits<v_int8>::vlanes());
            v_uint8 v1 = v_absdiff(v11, v12);
            r1 = v_dotprod_expand_fast(v1, v1, r1);
        }
        s += v_reduce_sum(v_add(r0, r1));
        ಪ್ರತಿ (; j < n; j++) {
            ಪೂರ್ಣಾಙ್ಕ v = (ಪೂರ್ಣಾಙ್ಕ)src1[j] - (ಪೂರ್ಣಾಙ್ಕ)src2[j];
            s += v * v;
        }
        ಪ್ರತ್ಯಾಗಮ s;
    }
};

#endif

#ಯದಿ (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)

template<>
ಸಂರಚನಾ NormInf_SIMD<ದ್ವಿಗುಣ, ದ್ವಿಗುಣ> {
    ದ್ವಿಗುಣ operator() (ಧ್ರುವ ದ್ವಿಗುಣ* src, ಪೂರ್ಣಾಙ್ಕ n) ಧ್ರುವ {
        ಪೂರ್ಣಾಙ್ಕ j = 0;
        ದ್ವಿಗುಣ s = 0.f;
        v_float64 r0 = vx_setzero_f64(), r1 = vx_setzero_f64();
        v_float64 r2 = vx_setzero_f64(), r3 = vx_setzero_f64();
        ಪ್ರತಿ (; j <= n - 4 * VTraits<v_float64>::vlanes(); j += 4 * VTraits<v_float64>::vlanes()) {
            r0 = v_max(r0, v_abs(vx_load(src + j                                   )));
            r1 = v_max(r1, v_abs(vx_load(src + j +     VTraits<v_float64>::vlanes())));
            r2 = v_max(r2, v_abs(vx_load(src + j + 2 * VTraits<v_float64>::vlanes())));
            r3 = v_max(r3, v_abs(vx_load(src + j + 3 * VTraits<v_float64>::vlanes())));
        }
        r0 = v_max(r0, v_max(r1, v_max(r2, r3)));
        ಪ್ರತಿ (; j < n; j++) {
            s = std::max(s, cv_abs(src[j]));
        }
        // [TODO]: use v_reduce_max when it supports float64
        ದ್ವಿಗುಣ t[VTraits<v_float64>::max_nlanes];
        vx_store(t, r0);
        ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < VTraits<v_float64>::vlanes(); i++) {
            s = std::max(s, cv_abs(t[i]));
        }
        ಪ್ರತ್ಯಾಗಮ s;
    }
};

template<>
ಸಂರಚನಾ NormL1_SIMD<ಪೂರ್ಣಾಙ್ಕ, ದ್ವಿಗುಣ> {
    ದ್ವಿಗುಣ operator() (ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ* src, ಪೂರ್ಣಾಙ್ಕ n) ಧ್ರುವ {
        ಪೂರ್ಣಾಙ್ಕ j = 0;
        ದ್ವಿಗುಣ s = 0.f;
        v_float64 r00 = vx_setzero_f64(), r01 = vx_setzero_f64();
        v_float64 r10 = vx_setzero_f64(), r11 = vx_setzero_f64();
        ಪ್ರತಿ (; j <= n - 2 * VTraits<v_int32>::vlanes(); j += 2 * VTraits<v_int32>::vlanes()) {
            // Widen to double before taking the magnitude: exact over the whole int32 range,
            // INT_MIN included, which a float intermediate would not be above 2^24.
            v_int32 i0 = vx_load(src + j), i1 = vx_load(src + j + VTraits<v_int32>::vlanes());
            r00 = v_add(r00, v_abs(v_cvt_f64(i0))); r01 = v_add(r01, v_abs(v_cvt_f64_ಮೇಲಿನ(i0)));
            r10 = v_add(r10, v_abs(v_cvt_f64(i1))); r11 = v_add(r11, v_abs(v_cvt_f64_ಮೇಲಿನ(i1)));
        }
        s += v_reduce_sum(v_add(v_add(v_add(r00, r01), r10), r11));
        ಪ್ರತಿ (; j < n; j++) {
            s += cv_abs(src[j]);
        }
        ಪ್ರತ್ಯಾಗಮ s;
    }
};

template<>
ಸಂರಚನಾ NormL1_SIMD<ಭಿನ್ನ, ದ್ವಿಗುಣ> {
    ದ್ವಿಗುಣ operator() (ಧ್ರುವ ಭಿನ್ನ* src, ಪೂರ್ಣಾಙ್ಕ n) ಧ್ರುವ {
        ಪೂರ್ಣಾಙ್ಕ j = 0;
        ದ್ವಿಗುಣ s = 0.f;
        v_float64 r00 = vx_setzero_f64(), r01 = vx_setzero_f64();
        v_float64 r10 = vx_setzero_f64(), r11 = vx_setzero_f64();
        v_float64 r20 = vx_setzero_f64(), r21 = vx_setzero_f64();
        v_float64 r30 = vx_setzero_f64(), r31 = vx_setzero_f64();
        ಪ್ರತಿ (; j <= n - 4 * VTraits<v_float32>::vlanes(); j += 4 * VTraits<v_float32>::vlanes()) {
            v_float32 v0 = v_abs(vx_load(src + j)), v1 = v_abs(vx_load(src + j + VTraits<v_float32>::vlanes()));
            r00 = v_add(r00, v_cvt_f64(v0)); r01 = v_add(r01, v_cvt_f64_ಮೇಲಿನ(v0));
            r10 = v_add(r10, v_cvt_f64(v1)); r11 = v_add(r11, v_cvt_f64_ಮೇಲಿನ(v1));

            v_float32 v2 = v_abs(vx_load(src + j + 2 * VTraits<v_float32>::vlanes())), v3 = v_abs(vx_load(src + j + 3 * VTraits<v_float32>::vlanes()));
            r20 = v_add(r20, v_cvt_f64(v2)); r21 = v_add(r21, v_cvt_f64_ಮೇಲಿನ(v2));
            r30 = v_add(r30, v_cvt_f64(v3)); r31 = v_add(r31, v_cvt_f64_ಮೇಲಿನ(v3));
        }
        s += v_reduce_sum(v_add(v_add(v_add(r00, r01), r10), r11));
        s += v_reduce_sum(v_add(v_add(v_add(r20, r21), r30), r31));
        ಪ್ರತಿ (; j < n; j++) {
            s += cv_abs(src[j]);
        }
        ಪ್ರತ್ಯಾಗಮ s;
    }
};

template<>
ಸಂರಚನಾ NormL2_SIMD<ushort, ದ್ವಿಗುಣ> {
    ದ್ವಿಗುಣ operator() (ಧ್ರುವ ushort* src, ಪೂರ್ಣಾಙ್ಕ n) ಧ್ರುವ {
        ಪೂರ್ಣಾಙ್ಕ j = 0;
        ದ್ವಿಗುಣ s = 0.f;
        v_float64 r0 = vx_setzero_f64(), r1 = vx_setzero_f64();
        ಪ್ರತಿ (; j <= n - 2 * VTraits<v_uint16>::vlanes(); j += 2 * VTraits<v_uint16>::vlanes()) {
            v_uint16 v0 = vx_load(src + j);
            v_uint64 u0 = v_dotprod_expand_fast(v0, v0);
            r0 = v_add(r0, v_cvt_f64(v_reinterpret_as_s64(u0)));

            v_uint16 v1 = vx_load(src + j + VTraits<v_uint16>::vlanes());
            v_uint64 u1 = v_dotprod_expand_fast(v1, v1);
            r1 = v_add(r1, v_cvt_f64(v_reinterpret_as_s64(u1)));
        }
        s += v_reduce_sum(v_add(r0, r1));
        ಪ್ರತಿ (; j < n; j++) {
            ದ್ವಿಗುಣ v = saturate_cast<ದ್ವಿಗುಣ>(src[j]);
            s += v * v;
        }
        ಪ್ರತ್ಯಾಗಮ s;
    }
};

template<>
ಸಂರಚನಾ NormL2_SIMD<short, ದ್ವಿಗುಣ> {
    ದ್ವಿಗುಣ operator() (ಧ್ರುವ short* src, ಪೂರ್ಣಾಙ್ಕ n) ಧ್ರುವ {
        ಪೂರ್ಣಾಙ್ಕ j = 0;
        ದ್ವಿಗುಣ s = 0.f;
        v_float64 r0 = vx_setzero_f64(), r1 = vx_setzero_f64();
        ಪ್ರತಿ (; j <= n - 2 * VTraits<v_int16>::vlanes(); j += 2 * VTraits<v_int16>::vlanes()) {
            v_int16 v0 = vx_load(src + j);
            r0 = v_add(r0, v_cvt_f64(v_dotprod_expand_fast(v0, v0)));

            v_int16 v1 = vx_load(src + j + VTraits<v_int16>::vlanes());
            r1 = v_add(r1, v_cvt_f64(v_dotprod_expand_fast(v1, v1)));
        }
        s += v_reduce_sum(v_add(r0, r1));
        ಪ್ರತಿ (; j < n; j++) {
            ದ್ವಿಗುಣ v = saturate_cast<ದ್ವಿಗುಣ>(src[j]);
            s += v * v;
        }
        ಪ್ರತ್ಯಾಗಮ s;
    }
};

template<>
ಸಂರಚನಾ NormL2_SIMD<ಪೂರ್ಣಾಙ್ಕ, ದ್ವಿಗುಣ> {
    ದ್ವಿಗುಣ operator() (ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ* src, ಪೂರ್ಣಾಙ್ಕ n) ಧ್ರುವ {
        ಪೂರ್ಣಾಙ್ಕ j = 0;
        ದ್ವಿಗುಣ s = 0.f;
        v_float64 r0 = vx_setzero_f64(), r1 = vx_setzero_f64();
        ಪ್ರತಿ (; j <= n - 2 * VTraits<v_int32>::vlanes(); j += 2 * VTraits<v_int32>::vlanes()) {
            v_int32 v0 = vx_load(src + j);
            r0 = v_dotprod_expand_fast(v0, v0, r0);

            v_int32 v1 = vx_load(src + j + VTraits<v_int32>::vlanes());
            r1 = v_dotprod_expand_fast(v1, v1, r1);
        }
        s += v_reduce_sum(v_add(r0, r1));
        ಪ್ರತಿ (; j < n; j++) {
            ದ್ವಿಗುಣ v = src[j];
            s += v * v;
        }
        ಪ್ರತ್ಯಾಗಮ s;
    }
};

template<>
ಸಂರಚನಾ NormL2_SIMD<ಭಿನ್ನ, ದ್ವಿಗುಣ> {
    ದ್ವಿಗುಣ operator() (ಧ್ರುವ ಭಿನ್ನ* src, ಪೂರ್ಣಾಙ್ಕ n) ಧ್ರುವ {
        ಪೂರ್ಣಾಙ್ಕ j = 0;
        ದ್ವಿಗುಣ s = 0.f;
        v_float64 r00 = vx_setzero_f64(), r01 = vx_setzero_f64();
        v_float64 r10 = vx_setzero_f64(), r11 = vx_setzero_f64();
        ಪ್ರತಿ (; j <= n - 2 * VTraits<v_float32>::vlanes(); j += 2 * VTraits<v_float32>::vlanes()) {
            v_float32 v0 = vx_load(src + j), v1 = vx_load(src + j + VTraits<v_float32>::vlanes());
            v_float64 v00 = v_cvt_f64(v0), v01 = v_cvt_f64_ಮೇಲಿನ(v0);
            v_float64 v10 = v_cvt_f64(v1), v11 = v_cvt_f64_ಮೇಲಿನ(v1);
            r00 = v_fma(v00, v00, r00); r01 = v_fma(v01, v01, r01);
            r10 = v_fma(v10, v10, r10); r11 = v_fma(v11, v11, r11);
        }
        s += v_reduce_sum(v_add(v_add(v_add(r00, r01), r10), r11));
        ಪ್ರತಿ (; j < n; j++) {
            ದ್ವಿಗುಣ v = src[j];
            s += v * v;
        }
        ಪ್ರತ್ಯಾಗಮ s;
    }
};

#endif

#ಯದಿ CV_SIMD_64F // CV_SIMD_SCALABLE_64F has accuracy problem with the following kernels on ci

template<>
ಸಂರಚನಾ NormL1_SIMD<ದ್ವಿಗುಣ, ದ್ವಿಗುಣ> {
    ದ್ವಿಗುಣ operator() (ಧ್ರುವ ದ್ವಿಗುಣ* src, ಪೂರ್ಣಾಙ್ಕ n) ಧ್ರುವ {
        ಪೂರ್ಣಾಙ್ಕ j = 0;
        ದ್ವಿಗುಣ s = 0.f;
        v_float64 r00 = vx_setzero_f64(), r01 = vx_setzero_f64();
        v_float64 r10 = vx_setzero_f64(), r11 = vx_setzero_f64();
        ಪ್ರತಿ (; j <= n - 4 * VTraits<v_float64>::vlanes(); j += 4 * VTraits<v_float64>::vlanes()) {
            r00 = v_add(r00, v_abs(vx_load(src + j                                   )));
            r01 = v_add(r01, v_abs(vx_load(src + j +     VTraits<v_float64>::vlanes())));
            r10 = v_add(r10, v_abs(vx_load(src + j + 2 * VTraits<v_float64>::vlanes())));
            r11 = v_add(r11, v_abs(vx_load(src + j + 3 * VTraits<v_float64>::vlanes())));
        }
        s += v_reduce_sum(v_add(v_add(v_add(r00, r01), r10), r11));
        ಪ್ರತಿ (; j < n; j++) {
            s += cv_abs(src[j]);
        }
        ಪ್ರತ್ಯಾಗಮ s;
    }
};

template<>
ಸಂರಚನಾ NormL2_SIMD<ದ್ವಿಗುಣ, ದ್ವಿಗುಣ> {
    ದ್ವಿಗುಣ operator() (ಧ್ರುವ ದ್ವಿಗುಣ* src, ಪೂರ್ಣಾಙ್ಕ n) ಧ್ರುವ {
        ಪೂರ್ಣಾಙ್ಕ j = 0;
        ದ್ವಿಗುಣ s = 0.f;
        v_float64 r00 = vx_setzero_f64(), r01 = vx_setzero_f64();
        v_float64 r10 = vx_setzero_f64(), r11 = vx_setzero_f64();
        ಪ್ರತಿ (; j <= n - 4 * VTraits<v_float64>::vlanes(); j += 4 * VTraits<v_float64>::vlanes()) {
            v_float64 v00 = vx_load(src + j                                   );
            v_float64 v01 = vx_load(src + j +     VTraits<v_float64>::vlanes());
            v_float64 v10 = vx_load(src + j + 2 * VTraits<v_float64>::vlanes());
            v_float64 v11 = vx_load(src + j + 3 * VTraits<v_float64>::vlanes());
            r00 = v_fma(v00, v00, r00); r01 = v_fma(v01, v01, r01);
            r10 = v_fma(v10, v10, r10); r11 = v_fma(v11, v11, r11);
        }
        s += v_reduce_sum(v_add(v_add(v_add(r00, r01), r10), r11));
        ಪ್ರತಿ (; j < n; j++) {
            ದ್ವಿಗುಣ v = src[j];
            s += v * v;
        }
        ಪ್ರತ್ಯಾಗಮ s;
    }
};

template<>
ಸಂರಚನಾ NormDiffInf_SIMD<ದ್ವಿಗುಣ, ದ್ವಿಗುಣ> {
    ದ್ವಿಗುಣ operator() (ಧ್ರುವ ದ್ವಿಗುಣ* src1, ಧ್ರುವ ದ್ವಿಗುಣ* src2, ಪೂರ್ಣಾಙ್ಕ n) ಧ್ರುವ {
        ಪೂರ್ಣಾಙ್ಕ j = 0;
        ದ್ವಿಗುಣ s = 0;
        v_float64 r0 = vx_setzero_f64(), r1 = vx_setzero_f64();
        ಪ್ರತಿ (; j <= n - 2 * VTraits<v_float64>::vlanes(); j += 2 * VTraits<v_float64>::vlanes()) {
            v_float64 v01 = vx_load(src1 + j), v02 = vx_load(src2 + j);
            r0 = v_max(r0, v_absdiff(v01, v02));

            v_float64 v11 = vx_load(src1 + j + VTraits<v_float64>::vlanes()),
                      v12 = vx_load(src2 + j + VTraits<v_float64>::vlanes());
            r1 = v_max(r1, v_absdiff(v11, v12));
        }
        // [TODO]: use v_reduce_max when it supports float64
        ದ್ವಿಗುಣ t[VTraits<v_float64>::max_nlanes];
        vx_store(t, v_max(r0, r1));
        ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < VTraits<v_float64>::vlanes(); i++) {
            s = std::max(s, t[i]);
        }
        ಪ್ರತಿ (; j < n; j++) {
            ದ್ವಿಗುಣ v = (ದ್ವಿಗುಣ)cv_absdiff(src1[j], src2[j]);
            s = std::max(s, v);
        }
        ಪ್ರತ್ಯಾಗಮ s;
    }
};

template<>
ಸಂರಚನಾ NormDiffL1_SIMD<ಭಿನ್ನ, ದ್ವಿಗುಣ> {
    ದ್ವಿಗುಣ operator() (ಧ್ರುವ ಭಿನ್ನ* src1, ಧ್ರುವ ಭಿನ್ನ* src2, ಪೂರ್ಣಾಙ್ಕ n) ಧ್ರುವ {
        ಪೂರ್ಣಾಙ್ಕ j = 0;
        ದ್ವಿಗುಣ s = 0.f;
        v_float64 r0 = vx_setzero_f64(), r1 = vx_setzero_f64();
        v_float64 r2 = vx_setzero_f64(), r3 = vx_setzero_f64();
        ಪ್ರತಿ (; j <= n - 2 * VTraits<v_float32>::vlanes(); j += 2 * VTraits<v_float32>::vlanes()) {
            v_float32 v01 = vx_load(src1 + j), v02 = vx_load(src2 + j);
            v_float32 v0 = v_absdiff(v01, v02);
            r0 = v_add(r0, v_cvt_f64(v0)); r1 = v_add(r1, v_cvt_f64_ಮೇಲಿನ(v0));

            v_float32 v11 = vx_load(src1 + j + VTraits<v_float32>::vlanes()),
                      v12 = vx_load(src2 + j + VTraits<v_float32>::vlanes());
            v_float32 v1 = v_absdiff(v11, v12);
            r2 = v_add(r2, v_cvt_f64(v1)); r3 = v_add(r3, v_cvt_f64_ಮೇಲಿನ(v1));
        }
        s += v_reduce_sum(v_add(v_add(v_add(r0, r1), r2), r3));
        ಪ್ರತಿ (; j < n; j++) {
            ದ್ವಿಗುಣ v = (ದ್ವಿಗುಣ)cv_absdiff(src1[j], src2[j]);
            s += v;
        }
        ಪ್ರತ್ಯಾಗಮ s;
    }
};

template<>
ಸಂರಚನಾ NormDiffL1_SIMD<ದ್ವಿಗುಣ, ದ್ವಿಗುಣ> {
    ದ್ವಿಗುಣ operator() (ಧ್ರುವ ದ್ವಿಗುಣ* src1, ಧ್ರುವ ದ್ವಿಗುಣ* src2, ಪೂರ್ಣಾಙ್ಕ n) ಧ್ರುವ {
        ಪೂರ್ಣಾಙ್ಕ j = 0;
        ದ್ವಿಗುಣ s = 0.f;
        v_float64 r0 = vx_setzero_f64(), r1 = vx_setzero_f64();
        ಪ್ರತಿ (; j <= n - 2 * VTraits<v_float64>::vlanes(); j += 2 * VTraits<v_float64>::vlanes()) {
            v_float64 v01 = vx_load(src1 + j), v02 = vx_load(src2 + j);
            r0 = v_add(r0, v_absdiff(v01, v02));

            v_float64 v11 = vx_load(src1 + j + VTraits<v_float64>::vlanes()),
                      v12 = vx_load(src2 + j + VTraits<v_float64>::vlanes());
            r1 = v_add(r1, v_absdiff(v11, v12));
        }
        s += v_reduce_sum(v_add(r0, r1));
        ಪ್ರತಿ (; j < n; j++) {
            ದ್ವಿಗುಣ v = (ದ್ವಿಗುಣ)cv_absdiff(src1[j], src2[j]);
            s += v;
        }
        ಪ್ರತ್ಯಾಗಮ s;
    }
};

template<>
ಸಂರಚನಾ NormDiffL2_SIMD<ushort, ದ್ವಿಗುಣ> {
    ದ್ವಿಗುಣ operator() (ಧ್ರುವ ushort* src1, ಧ್ರುವ ushort* src2, ಪೂರ್ಣಾಙ್ಕ n) ಧ್ರುವ {
        ಪೂರ್ಣಾಙ್ಕ j = 0;
        ದ್ವಿಗುಣ s = 0.f;
        v_float64 r0 = vx_setzero_f64(), r1 = vx_setzero_f64();
        ಪ್ರತಿ (; j <= n - 2 * VTraits<v_uint16>::vlanes(); j += 2 * VTraits<v_uint16>::vlanes()) {
            v_uint16 v01 = vx_load(src1 + j), v02 = vx_load(src2 + j);
            v_uint16 v0 = v_absdiff(v01, v02);
            v_uint64 u0 = v_dotprod_expand_fast(v0, v0);
            r0 = v_add(r0, v_cvt_f64(v_reinterpret_as_s64(u0)));

            v_uint16 v11 = vx_load(src1 + j + VTraits<v_uint16>::vlanes()),
                     v12 = vx_load(src2 + j + VTraits<v_uint16>::vlanes());
            v_uint16 v1 = v_absdiff(v11, v12);
            v_uint64 u1 = v_dotprod_expand_fast(v1, v1);
            r1 = v_add(r1, v_cvt_f64(v_reinterpret_as_s64(u1)));
        }
        s += v_reduce_sum(v_add(r0, r1));
        ಪ್ರತಿ (; j < n; j++) {
            ದ್ವಿಗುಣ v = (ದ್ವಿಗುಣ)src1[j] - (ದ್ವಿಗುಣ)src2[j];
            s += v * v;
        }
        ಪ್ರತ್ಯಾಗಮ s;
    }
};

template<>
ಸಂರಚನಾ NormDiffL2_SIMD<short, ದ್ವಿಗುಣ> {
    ದ್ವಿಗುಣ operator() (ಧ್ರುವ short* src1, ಧ್ರುವ short* src2, ಪೂರ್ಣಾಙ್ಕ n) ಧ್ರುವ {
        ಪೂರ್ಣಾಙ್ಕ j = 0;
        ದ್ವಿಗುಣ s = 0.f;
        v_float64 r0 = vx_setzero_f64(), r1 = vx_setzero_f64();
        ಪ್ರತಿ (; j <= n - 2 * VTraits<v_int16>::vlanes(); j += 2 * VTraits<v_int16>::vlanes()) {
            v_int16 v01 = vx_load(src1 + j), v02 = vx_load(src2 + j);
            v_uint16 v0 = v_absdiff(v01, v02);
            v_uint64 u0 = v_dotprod_expand_fast(v0, v0);
            r0 = v_add(r0, v_cvt_f64(v_reinterpret_as_s64(u0)));

            v_int16 v11 = vx_load(src1 + j + VTraits<v_uint16>::vlanes()),
                    v12 = vx_load(src2 + j + VTraits<v_uint16>::vlanes());
            v_uint16 v1 = v_absdiff(v11, v12);
            v_uint64 u1 = v_dotprod_expand_fast(v1, v1);
            r1 = v_add(r1, v_cvt_f64(v_reinterpret_as_s64(u1)));
        }
        s += v_reduce_sum(v_add(r0, r1));
        ಪ್ರತಿ (; j < n; j++) {
            ದ್ವಿಗುಣ v = (ದ್ವಿಗುಣ)src1[j] - (ದ್ವಿಗುಣ)src2[j];
            s += v * v;
        }
        ಪ್ರತ್ಯಾಗಮ s;
    }
};

template<>
ಸಂರಚನಾ NormDiffL2_SIMD<ಪೂರ್ಣಾಙ್ಕ, ದ್ವಿಗುಣ> {
    ದ್ವಿಗುಣ operator() (ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ* src1, ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ* src2, ಪೂರ್ಣಾಙ್ಕ n) ಧ್ರುವ {
        ಪೂರ್ಣಾಙ್ಕ j = 0;
        ದ್ವಿಗುಣ s = 0.f;
        v_float64 r0 = vx_setzero_f64(), r1 = vx_setzero_f64();
        ಪ್ರತಿ (; j <= n - VTraits<v_int32>::vlanes(); j += VTraits<v_int32>::vlanes()) {
            v_int32 v01 = vx_load(src1 + j), v02 = vx_load(src2 + j);
            v_uint32 v0 = v_absdiff(v01, v02);
            v_uint64 ev0, ev1;
            v_expand(v0, ev0, ev1);
            v_float64 f0 = v_cvt_f64(v_reinterpret_as_s64(ev0)), f1 = v_cvt_f64(v_reinterpret_as_s64(ev1));
            r0 = v_fma(f0, f0, r0); r1 = v_fma(f1, f1, r1);
        }
        s += v_reduce_sum(v_add(r0, r1));
        ಪ್ರತಿ (; j < n; j++) {
            ದ್ವಿಗುಣ v = (ದ್ವಿಗುಣ)src1[j] - (ದ್ವಿಗುಣ)src2[j];
            s += v * v;
        }
        ಪ್ರತ್ಯಾಗಮ s;
    }
};

template<>
ಸಂರಚನಾ NormDiffL2_SIMD<ಭಿನ್ನ, ದ್ವಿಗುಣ> {
    ದ್ವಿಗುಣ operator() (ಧ್ರುವ ಭಿನ್ನ* src1, ಧ್ರುವ ಭಿನ್ನ* src2, ಪೂರ್ಣಾಙ್ಕ n) ಧ್ರುವ {
        ಪೂರ್ಣಾಙ್ಕ j = 0;
        ದ್ವಿಗುಣ s = 0.f;
        v_float64 r0 = vx_setzero_f64(), r1 = vx_setzero_f64();
        v_float64 r2 = vx_setzero_f64(), r3 = vx_setzero_f64();
        ಪ್ರತಿ (; j <= n - 2 * VTraits<v_float32>::vlanes(); j += 2 * VTraits<v_float32>::vlanes()) {
            v_float32 v01 = vx_load(src1 + j), v02 = vx_load(src2 + j);
            v_float32 v0 = v_absdiff(v01, v02);
            v_float64 f01 = v_cvt_f64(v0), f02 = v_cvt_f64_ಮೇಲಿನ(v0);
            r0 = v_fma(f01, f01, r0); r1 = v_fma(f02, f02, r1);

            v_float32 v11 = vx_load(src1 + j + VTraits<v_float32>::vlanes()),
                      v12 = vx_load(src2 + j + VTraits<v_float32>::vlanes());
            v_float32 v1 = v_absdiff(v11, v12);
            v_float64 f11 = v_cvt_f64(v1), f12 = v_cvt_f64_ಮೇಲಿನ(v1);
            r2 = v_fma(f11, f11, r2); r3 = v_fma(f12, f12, r3);
        }
        s += v_reduce_sum(v_add(v_add(v_add(r0, r1), r2), r3));
        ಪ್ರತಿ (; j < n; j++) {
            ದ್ವಿಗುಣ v = (ದ್ವಿಗುಣ)src1[j] - (ದ್ವಿಗುಣ)src2[j];
            s += v * v;
        }
        ಪ್ರತ್ಯಾಗಮ s;
    }
};

template<>
ಸಂರಚನಾ NormDiffL2_SIMD<ದ್ವಿಗುಣ, ದ್ವಿಗುಣ> {
    ದ್ವಿಗುಣ operator() (ಧ್ರುವ ದ್ವಿಗುಣ* src1, ಧ್ರುವ ದ್ವಿಗುಣ* src2, ಪೂರ್ಣಾಙ್ಕ n) ಧ್ರುವ {
        ಪೂರ್ಣಾಙ್ಕ j = 0;
        ದ್ವಿಗುಣ s = 0.f;
        v_float64 r0 = vx_setzero_f64(), r1 = vx_setzero_f64();
        v_float64 r2 = vx_setzero_f64(), r3 = vx_setzero_f64();
        ಪ್ರತಿ (; j <= n - 4 * VTraits<v_float64>::vlanes(); j += 4 * VTraits<v_float64>::vlanes()) {
            v_float64 v01 = vx_load(src1 + j), v02 = vx_load(src2 + j);
            v_float64 v0 = v_absdiff(v01, v02);
            r0 = v_fma(v0, v0, r0);

            v_float64 v11 = vx_load(src1 + j + VTraits<v_float64>::vlanes()),
                      v12 = vx_load(src2 + j + VTraits<v_float64>::vlanes());
            v_float64 v1 = v_absdiff(v11, v12);
            r1 = v_fma(v1, v1, r1);

            v_float64 v21 = vx_load(src1 + j + 2 * VTraits<v_float64>::vlanes()),
                      v22 = vx_load(src2 + j + 2 * VTraits<v_float64>::vlanes());
            v_float64 v2 = v_absdiff(v21, v22);
            r2 = v_fma(v2, v2, r2);

            v_float64 v31 = vx_load(src1 + j + 3 * VTraits<v_float64>::vlanes()),
                      v32 = vx_load(src2 + j + 3 * VTraits<v_float64>::vlanes());
            v_float64 v3 = v_absdiff(v31, v32);
            r3 = v_fma(v3, v3, r3);
        }
        s += v_reduce_sum(v_add(v_add(v_add(r0, r1), r2), r3));
        ಪ್ರತಿ (; j < n; j++) {
            ದ್ವಿಗುಣ v = (ದ್ವಿಗುಣ)src1[j] - (ದ್ವಿಗುಣ)src2[j];
            s += v * v;
        }
        ಪ್ರತ್ಯಾಗಮ s;
    }
};

#endif

template <typename T, typename ST>
ಸಂರಚನಾ MaskedNormInf_SIMD {
    ಅನ್ತಃಸ್ಥ ST operator() (ಧ್ರುವ T* src, ಧ್ರುವ uchar* mask, ಪೂರ್ಣಾಙ್ಕ len, ಪೂರ್ಣಾಙ್ಕ cn) ಧ್ರುವ {
        ST s = 0;
        ಯದಿ (cn == 1) {
            ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < len; i++) {
                ಯದಿ (mask[i]) {
                    s = std::max(s, (ST)cv_abs(src[i]));
                }
            }
        }
        ಅನ್ಯಥಾ {
            ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < len; i++) {
                ಯದಿ (mask[i]) {
                    ಧ್ರುವ T* elem = src + i * cn;
                    ಪೂರ್ಣಾಙ್ಕ k = 0;
                #ಯದಿ CV_ENABLE_UNROLLED && !CV_SIMD_SCALABLE
                    ಪ್ರತಿ (; k <= cn - 4; k += 4) {
                        s = std::max(s, (ST)cv_abs(elem[k]));
                        s = std::max(s, (ST)cv_abs(elem[k + 1]));
                        s = std::max(s, (ST)cv_abs(elem[k + 2]));
                        s = std::max(s, (ST)cv_abs(elem[k + 3]));
                    }
                #endif
                    ಪ್ರತಿ (; k < cn; k++) {
                        s = std::max(s, (ST)cv_abs(elem[k]));
                    }
                }
            }
        }
        ಪ್ರತ್ಯಾಗಮ s;
    }
};

template <typename T, typename ST>
ಸಂರಚನಾ MaskedNormL1_SIMD {
    ಅನ್ತಃಸ್ಥ ST operator() (ಧ್ರುವ T* src, ಧ್ರುವ uchar* mask, ಪೂರ್ಣಾಙ್ಕ len, ಪೂರ್ಣಾಙ್ಕ cn) ಧ್ರುವ {
        ST s = 0;
        ಯದಿ (cn == 1) {
            ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < len; i++) {
                ಯದಿ (mask[i]) {
                    s += (ST)cv_abs(src[i]);
                }
            }
        }
        ಅನ್ಯಥಾ {
            ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < len; i++) {
                ಯದಿ (mask[i]) {
                    ಧ್ರುವ T* elem = src + i * cn;
                    ಪೂರ್ಣಾಙ್ಕ k = 0;
                #ಯದಿ CV_ENABLE_UNROLLED && !CV_SIMD_SCALABLE
                    ಪ್ರತಿ (; k <= cn - 4; k += 4) {
                        s += (ST)cv_abs(elem[k]);
                        s += (ST)cv_abs(elem[k + 1]);
                        s += (ST)cv_abs(elem[k + 2]);
                        s += (ST)cv_abs(elem[k + 3]);
                    }
                #endif
                    ಪ್ರತಿ (; k < cn; k++) {
                        s += (ST)cv_abs(elem[k]);
                    }
                }
            }
        }
        ಪ್ರತ್ಯಾಗಮ s;
    }
};

template <typename T, typename ST>
ಸಂರಚನಾ MaskedNormL2_SIMD {
    ಅನ್ತಃಸ್ಥ ST operator() (ಧ್ರುವ T* src, ಧ್ರುವ uchar* mask, ಪೂರ್ಣಾಙ್ಕ len, ಪೂರ್ಣಾಙ್ಕ cn) ಧ್ರುವ {
        ST s = 0;
        ಯದಿ (cn == 1) {
            ಪೂರ್ಣಾಙ್ಕ i = 0;
        #ಯದಿ CV_ENABLE_UNROLLED && !CV_SIMD_SCALABLE
            ಪ್ರತಿ (; i <= len - 4; i += 4) {
                ಯದಿ (mask[i])     { T v0 = src[i];     s += (ST)v0 * v0; }
                ಯದಿ (mask[i + 1]) { T v1 = src[i + 1]; s += (ST)v1 * v1; }
                ಯದಿ (mask[i + 2]) { T v2 = src[i + 2]; s += (ST)v2 * v2; }
                ಯದಿ (mask[i + 3]) { T v3 = src[i + 3]; s += (ST)v3 * v3; }
            }
        #endif
            ಪ್ರತಿ (; i < len; i++) {
                ಯದಿ (mask[i]) {
                    T v = src[i];
                    s += (ST)v * v;
                }
            }
        }
        ಅನ್ಯಥಾ {
            ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < len; i++) {
                ಯದಿ (mask[i]) {
                    ಧ್ರುವ T* elem = src + i * cn;
                    ಪೂರ್ಣಾಙ್ಕ k = 0;
                #ಯದಿ CV_ENABLE_UNROLLED && !CV_SIMD_SCALABLE
                    ಪ್ರತಿ (; k <= cn - 4; k += 4) {
                        T v0 = elem[k];     s += (ST)v0 * v0;
                        T v1 = elem[k + 1]; s += (ST)v1 * v1;
                        T v2 = elem[k + 2]; s += (ST)v2 * v2;
                        T v3 = elem[k + 3]; s += (ST)v3 * v3;
                    }
                #endif
                    ಪ್ರತಿ (; k < cn; k++) {
                        T v = elem[k];
                        s += (ST)v * v;
                    }
                }
            }
        }

        ಪ್ರತ್ಯಾಗಮ s;
    }
};

#ಯದಿ (CV_SIMD || CV_SIMD_SCALABLE)
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ v_uint8  v_normmask_u8 (ಧ್ರುವ uchar* m) { ಪ್ರತ್ಯಾಗಮ v_gt(vx_load(m),          vx_setzero_u8());  }
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ v_uint16 v_normmask_u16(ಧ್ರುವ uchar* m) { ಪ್ರತ್ಯಾಗಮ v_gt(vx_load_expand(m),   vx_setzero_u16()); }
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ v_uint32 v_normmask_u32(ಧ್ರುವ uchar* m) { ಪ್ರತ್ಯಾಗಮ v_gt(vx_load_expand_q(m), vx_setzero_u32()); }
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ v_normmask_u64(ಧ್ರುವ uchar* m, v_uint64& m0, v_uint64& m1) {
    v_uint64 e0, e1;
    v_expand(v_normmask_u32(m), e0, e1);
    m0 = v_or(e0, v_shl<32>(e0));
    m1 = v_or(e1, v_shl<32>(e1));
}

template <>
ಸಂರಚನಾ MaskedNormInf_SIMD<ಭಿನ್ನ, ಭಿನ್ನ> {
    ಅನ್ತಃಸ್ಥ ಭಿನ್ನ operator()(ಧ್ರುವ ಭಿನ್ನ* src, ಧ್ರುವ uchar* mask, ಪೂರ್ಣಾಙ್ಕ len, ಪೂರ್ಣಾಙ್ಕ cn) ಧ್ರುವ {
        ಭಿನ್ನ result = 0.0f;
        ಯದಿ (cn == 1) {
            ಪೂರ್ಣಾಙ್ಕ i = 0;
            ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ vstep = VTraits<v_float32>::vlanes();
            v_float32 acc = vx_setzero_f32();

            ಪ್ರತಿ (; i <= len - vstep; i += vstep) {
                v_uint32 m = vx_load_expand_q(mask + i);
                v_uint32 cmp = v_gt(m, vx_setzero_u32());
                v_float32 s = vx_load(src + i);
                s = v_abs(s);
                s = v_reinterpret_as_f32(v_and(v_reinterpret_as_u32(s), cmp));
                acc = v_max(acc, s);
            }
            result = v_reduce_max(acc);

            ಪ್ರತಿ (; i < len; i++) {
                ಯದಿ (mask[i])
                    result = std::max(result, std::abs(src[i]));
            }
        }
        ಅನ್ಯಥಾ {
            ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < len; i++) {
                ಯದಿ (mask[i]) {
                    ಧ್ರುವ ಭಿನ್ನ* elem = src + i * cn;
                    ಪೂರ್ಣಾಙ್ಕ k = 0;
                    ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ vstep = VTraits<v_float32>::vlanes();
                    v_float32 acc = vx_setzero_f32();

                    ಪ್ರತಿ (; k <= cn - vstep; k += vstep) {
                        v_float32 s = vx_load(elem + k);
                        acc = v_max(acc, v_abs(s));
                    }

                    result = std::max(result, v_reduce_max(acc));

                    ಪ್ರತಿ (; k < cn; k++)
                        result = std::max(result, std::abs(elem[k]));
                }
            }
        }
        ಪ್ರತ್ಯಾಗಮ result;
    }
};

#ಯದಿ CV_SIMD_64F
template <>
ಸಂರಚನಾ MaskedNormL1_SIMD<ಭಿನ್ನ, ದ್ವಿಗುಣ> {
    ಅನ್ತಃಸ್ಥ ದ್ವಿಗುಣ operator()(ಧ್ರುವ ಭಿನ್ನ* src, ಧ್ರುವ uchar* mask, ಪೂರ್ಣಾಙ್ಕ len, ಪೂರ್ಣಾಙ್ಕ cn) ಧ್ರುವ {
        ದ್ವಿಗುಣ result = 0.0;
        ಯದಿ (cn == 1) {
            ಪೂರ್ಣಾಙ್ಕ i = 0;
            ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ vstep = VTraits<v_float32>::vlanes();
            v_float64 acc0 = vx_setzero_f64(), acc1 = vx_setzero_f64();
            v_float64 acc2 = vx_setzero_f64(), acc3 = vx_setzero_f64();

            ಪ್ರತಿ (; i <= len - 2*vstep; i += 2*vstep) {
                v_float32 s0 = v_and(v_abs(vx_load(src + i)),
                                     v_reinterpret_as_f32(v_normmask_u32(mask + i)));
                v_float32 s1 = v_and(v_abs(vx_load(src + i + vstep)),
                                     v_reinterpret_as_f32(v_normmask_u32(mask + i + vstep)));
                acc0 = v_add(acc0, v_cvt_f64(s0));
                acc1 = v_add(acc1, v_cvt_f64_ಮೇಲಿನ(s0));
                acc2 = v_add(acc2, v_cvt_f64(s1));
                acc3 = v_add(acc3, v_cvt_f64_ಮೇಲಿನ(s1));
            }
            ಪ್ರತಿ (; i <= len - vstep; i += vstep) {
                v_float32 s = v_and(v_abs(vx_load(src + i)),
                                    v_reinterpret_as_f32(v_normmask_u32(mask + i)));
                acc0 = v_add(acc0, v_cvt_f64(s));
                acc1 = v_add(acc1, v_cvt_f64_ಮೇಲಿನ(s));
            }
            result = v_reduce_sum(v_add(v_add(acc0, acc1), v_add(acc2, acc3)));

            ಪ್ರತಿ (; i < len; i++) {
                ಯದಿ (mask[i])
                    result += std::abs(src[i]);
            }
        }
        ಅನ್ಯಥಾ {
            ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < len; i++) {
                ಯದಿ (mask[i]) {
                    ಧ್ರುವ ಭಿನ್ನ* elem = src + i * cn;
                    ಪೂರ್ಣಾಙ್ಕ k = 0;
                    ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ vstep = VTraits<v_float32>::vlanes();
                    v_float64 acc = vx_setzero_f64();

                    ಪ್ರತಿ (; k <= cn - vstep; k += vstep) {
                        v_float32 s = v_abs(vx_load(elem + k));
                        acc = v_add(acc, v_cvt_f64(s));
                        acc = v_add(acc, v_cvt_f64_ಮೇಲಿನ(s));
                    }

                    result += v_reduce_sum(acc);

                    ಪ್ರತಿ (; k < cn; k++)
                        result += std::abs(elem[k]);
                }
            }
        }
        ಪ್ರತ್ಯಾಗಮ result;
    }
};

template <>
ಸಂರಚನಾ MaskedNormL2_SIMD<ಭಿನ್ನ, ದ್ವಿಗುಣ> {
    ಅನ್ತಃಸ್ಥ ದ್ವಿಗುಣ operator()(ಧ್ರುವ ಭಿನ್ನ* src, ಧ್ರುವ uchar* mask, ಪೂರ್ಣಾಙ್ಕ len, ಪೂರ್ಣಾಙ್ಕ cn) ಧ್ರುವ {
        ದ್ವಿಗುಣ result = 0.0;
        ಯದಿ (cn == 1) {
            ಪೂರ್ಣಾಙ್ಕ i = 0;
            ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ vstep = VTraits<v_float32>::vlanes();
            v_float32 facc = vx_setzero_f32();
            v_float64 dacc = vx_setzero_f64();
            ಪೂರ್ಣಾಙ್ಕ flush = 0;

            ಪ್ರತಿ (; i <= len - vstep; i += vstep, flush += vstep) {
                ಯದಿ (flush >= 64) {
                    dacc = v_add(dacc, v_cvt_f64(facc));
                    dacc = v_add(dacc, v_cvt_f64_ಮೇಲಿನ(facc));
                    facc = vx_setzero_f32();
                    flush = 0;
                }
                v_uint32 cmp = v_gt(vx_load_expand_q(mask + i), vx_setzero_u32());
                v_float32 s  = v_reinterpret_as_f32(v_and(v_reinterpret_as_u32(vx_load(src + i)), cmp));
                facc = v_add(facc, v_mul(s, s));
            }

            dacc = v_add(dacc, v_cvt_f64(facc));
            dacc = v_add(dacc, v_cvt_f64_ಮೇಲಿನ(facc));
            result = v_reduce_sum(dacc);

            ಪ್ರತಿ (; i < len; i++) {
                ಯದಿ (mask[i]) {
                    ದ್ವಿಗುಣ v = src[i];
                    result += v * v;
                }
            }
        }
        ಅನ್ಯಥಾ {
            ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < len; i++) {
                ಯದಿ (mask[i]) {
                    ಧ್ರುವ ಭಿನ್ನ* elem = src + i * cn;
                    ಪೂರ್ಣಾಙ್ಕ k = 0;
                    ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ vstep = VTraits<v_float32>::vlanes();
                    v_float32 facc = vx_setzero_f32();

                    ಪ್ರತಿ (; k <= cn - vstep; k += vstep) {
                        v_float32 s = vx_load(elem + k);
                        facc = v_add(facc, v_mul(s, s));
                    }

                    v_float64 dacc = v_add(v_cvt_f64(facc), v_cvt_f64_ಮೇಲಿನ(facc));
                    result += v_reduce_sum(dacc);

                    ಪ್ರತಿ (; k < cn; k++) {
                        ದ್ವಿಗುಣ v = elem[k];
                        result += v * v;
                    }
                }
            }
        }
        ಪ್ರತ್ಯಾಗಮ result;
    }
};

#endif

template <>
ಸಂರಚನಾ MaskedNormInf_SIMD<uchar, ಪೂರ್ಣಾಙ್ಕ> {
    ಅನ್ತಃಸ್ಥ ಪೂರ್ಣಾಙ್ಕ operator()(ಧ್ರುವ uchar* src, ಧ್ರುವ uchar* mask, ಪೂರ್ಣಾಙ್ಕ len, ಪೂರ್ಣಾಙ್ಕ cn) ಧ್ರುವ {
        ಪೂರ್ಣಾಙ್ಕ result = 0;
        ಯದಿ (cn == 1) {
            ಪೂರ್ಣಾಙ್ಕ i = 0;
            ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ vstep = VTraits<v_uint8>::vlanes();
            v_uint8 acc = vx_setzero_u8();

            ಪ್ರತಿ (; i <= len - vstep; i += vstep) {
                v_uint8 m   = vx_load(mask + i);
                v_uint8 s   = vx_load(src + i);
                v_uint8 sel = v_and(s, v_gt(m, vx_setzero_u8()));
                acc = v_max(acc, sel);
            }

            result = (ಪೂರ್ಣಾಙ್ಕ)v_reduce_max(acc);

            ಪ್ರತಿ (; i < len; i++) {
                ಯದಿ (mask[i])
                    result = std::max(result, (ಪೂರ್ಣಾಙ್ಕ)src[i]);
            }
        }
        ಅನ್ಯಥಾ ಯದಿ (cn == 4 && len >= VTraits<v_uint8>::vlanes()) {
            ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ vstep = VTraits<v_uint8>::vlanes();
            v_uint8 acc = vx_setzero_u8();
            ಪೂರ್ಣಾಙ್ಕ i = 0;
            ಪ್ರತಿ (;;) {
                ಯದಿ (i > len - vstep)
                    i = len - vstep; // back-step (max is idempotent)
                v_uint8 c0, c1, c2, c3;
                v_load_deinterleave(src + i * 4, c0, c1, c2, c3);
                v_uint8 pmax = v_max(v_max(c0, c1), v_max(c2, c3));
                v_uint8 m = v_gt(vx_load(mask + i), vx_setzero_u8());
                acc = v_max(acc, v_and(pmax, m));
                ಯದಿ (i >= len - vstep)
                    ಭಙ್ಗ;
                i += vstep;
            }
            result = (ಪೂರ್ಣಾಙ್ಕ)v_reduce_max(acc);
        }
        ಅನ್ಯಥಾ ಯದಿ (cn == 4) {
            ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < len; i++) {
                ಯದಿ (mask[i]) {
                    ಧ್ರುವ uchar* elem = src + i * 4;
                    result = std::max(result, (ಪೂರ್ಣಾಙ್ಕ)std::max(std::max(elem[0], elem[1]),
                                                            std::max(elem[2], elem[3])));
                }
            }
        }
        ಅನ್ಯಥಾ {
            ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < len; i++) {
                ಯದಿ (mask[i]) {
                    ಧ್ರುವ uchar* elem = src + i * cn;
                    ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ k = 0; k < cn; k++)
                        result = std::max(result, (ಪೂರ್ಣಾಙ್ಕ)elem[k]);
                }
            }
        }
        ಪ್ರತ್ಯಾಗಮ result;
    }
};

template <>
ಸಂರಚನಾ MaskedNormL1_SIMD<uchar, ಪೂರ್ಣಾಙ್ಕ> {
    ಅನ್ತಃಸ್ಥ ಪೂರ್ಣಾಙ್ಕ operator()(ಧ್ರುವ uchar* src, ಧ್ರುವ uchar* mask, ಪೂರ್ಣಾಙ್ಕ len, ಪೂರ್ಣಾಙ್ಕ cn) ಧ್ರುವ {
        ಪೂರ್ಣಾಙ್ಕ result = 0;
        ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ vstep = VTraits<v_uint8>::vlanes();
        ಧ್ರುವ v_uint8 one = vx_setall_u8(1);
        ಯದಿ (cn == 1) {
            ಪೂರ್ಣಾಙ್ಕ i = 0;
            v_uint32 acc = vx_setzero_u32();
            ಪ್ರತಿ (; i <= len - vstep; i += vstep) {
                v_uint8 s = v_and(vx_load(src + i), v_normmask_u8(mask + i));
                acc = v_dotprod_expand_fast(s, one, acc);
            }
            result = (ಪೂರ್ಣಾಙ್ಕ)v_reduce_sum(acc);
            ಪ್ರತಿ (; i < len; i++) {
                ಯದಿ (mask[i])
                    result += src[i];
            }
        }
        ಅನ್ಯಥಾ ಯದಿ (cn == 4) {
            ಪೂರ್ಣಾಙ್ಕ i = 0;
            v_uint32 acc = vx_setzero_u32();
            ಪ್ರತಿ (; i <= len - vstep; i += vstep) {
                v_uint8 c0, c1, c2, c3;
                v_load_deinterleave(src + i * 4, c0, c1, c2, c3);
                v_uint8 m = v_normmask_u8(mask + i);
                acc = v_dotprod_expand_fast(v_and(c0, m), one, acc);
                acc = v_dotprod_expand_fast(v_and(c1, m), one, acc);
                acc = v_dotprod_expand_fast(v_and(c2, m), one, acc);
                acc = v_dotprod_expand_fast(v_and(c3, m), one, acc);
            }
            result = (ಪೂರ್ಣಾಙ್ಕ)v_reduce_sum(acc);
            ಪ್ರತಿ (; i < len; i++) {
                ಯದಿ (mask[i]) {
                    ಧ್ರುವ uchar* elem = src + i * 4;
                    result += elem[0] + elem[1] + elem[2] + elem[3];
                }
            }
        }
        ಅನ್ಯಥಾ {
            ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < len; i++) {
                ಯದಿ (mask[i]) {
                    ಧ್ರುವ uchar* elem = src + i * cn;
                    ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ k = 0; k < cn; k++)
                        result += elem[k];
                }
            }
        }
        ಪ್ರತ್ಯಾಗಮ result;
    }
};

template <>
ಸಂರಚನಾ MaskedNormInf_SIMD<ushort, ಪೂರ್ಣಾಙ್ಕ> {
    ಅನ್ತಃಸ್ಥ ಪೂರ್ಣಾಙ್ಕ operator()(ಧ್ರುವ ushort* src, ಧ್ರುವ uchar* mask, ಪೂರ್ಣಾಙ್ಕ len, ಪೂರ್ಣಾಙ್ಕ cn) ಧ್ರುವ {
        ಪೂರ್ಣಾಙ್ಕ result = 0;
        ಯದಿ (cn == 1) {
            ಪೂರ್ಣಾಙ್ಕ i = 0;
            ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ vstep = VTraits<v_uint16>::vlanes();
            v_uint16 acc = vx_setzero_u16();

            ಪ್ರತಿ (; i <= len - vstep; i += vstep) {
                v_uint16 m   = vx_load_expand(mask + i);
                v_uint16 cmp = v_gt(m, vx_setzero_u16());
                v_uint16 s   = vx_load(src + i);
                v_uint16 sel = v_and(s, cmp);
                acc = v_max(acc, sel);
            }

            result = (ಪೂರ್ಣಾಙ್ಕ)v_reduce_max(acc);

            ಪ್ರತಿ (; i < len; i++) {
                ಯದಿ (mask[i])
                    result = std::max(result, (ಪೂರ್ಣಾಙ್ಕ)src[i]);
            }
        }
        ಅನ್ಯಥಾ {
            ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < len; i++) {
                ಯದಿ (mask[i]) {
                    ಧ್ರುವ ushort* elem = src + i * cn;
                    ಪೂರ್ಣಾಙ್ಕ k = 0;
                    ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ vstep = VTraits<v_uint16>::vlanes();
                    v_uint16 acc = vx_setzero_u16();

                    ಪ್ರತಿ (; k <= cn - vstep; k += vstep) {
                        acc = v_max(acc, vx_load(elem + k));
                    }

                    result = std::max(result, (ಪೂರ್ಣಾಙ್ಕ)v_reduce_max(acc));

                    ಪ್ರತಿ (; k < cn; k++)
                        result = std::max(result, (ಪೂರ್ಣಾಙ್ಕ)elem[k]);
                }
            }
        }
        ಪ್ರತ್ಯಾಗಮ result;
    }
};

template <>
ಸಂರಚನಾ MaskedNormL1_SIMD<ushort, ಪೂರ್ಣಾಙ್ಕ> {
    ಅನ್ತಃಸ್ಥ ಪೂರ್ಣಾಙ್ಕ operator()(ಧ್ರುವ ushort* src, ಧ್ರುವ uchar* mask, ಪೂರ್ಣಾಙ್ಕ len, ಪೂರ್ಣಾಙ್ಕ cn) ಧ್ರುವ {
        ಪೂರ್ಣಾಙ್ಕ result = 0;
        ಯದಿ (cn == 1) {
            ಪೂರ್ಣಾಙ್ಕ i = 0;
            ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ vstep = VTraits<v_uint16>::vlanes();
            v_uint32 acc32 = vx_setzero_u32();
            v_uint64 acc64 = vx_setzero_u64();
            ಪೂರ್ಣಾಙ್ಕ acc32_elems = 0;

            ಪ್ರತಿ (; i <= len - vstep; i += vstep, acc32_elems += vstep) {
                ಯದಿ (acc32_elems >= 512) {
                    v_uint64 lo64, hi64;
                    v_expand(acc32, lo64, hi64);
                    acc64 = v_add(acc64, v_add(lo64, hi64));
                    acc32 = vx_setzero_u32();
                    acc32_elems = 0;
                }
                v_uint16 m   = vx_load_expand(mask + i);
                v_uint16 cmp = v_gt(m, vx_setzero_u16());
                v_uint16 s   = v_and(vx_load(src + i), cmp);
                v_uint32 lo32, hi32;
                v_expand(s, lo32, hi32);
                acc32 = v_add(acc32, v_add(lo32, hi32));
            }

            v_uint64 lo64, hi64;
            v_expand(acc32, lo64, hi64);
            acc64 = v_add(acc64, v_add(lo64, hi64));
            result = (ಪೂರ್ಣಾಙ್ಕ)v_reduce_sum(acc64);

            ಪ್ರತಿ (; i < len; i++) {
                ಯದಿ (mask[i])
                    result += src[i];
            }
        }
        ಅನ್ಯಥಾ {
            ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < len; i++) {
                ಯದಿ (mask[i]) {
                    ಧ್ರುವ ushort* elem = src + i * cn;
                    ಪೂರ್ಣಾಙ್ಕ k = 0;
                    ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ vstep = VTraits<v_uint16>::vlanes();
                    v_uint32 acc = vx_setzero_u32();

                    ಪ್ರತಿ (; k <= cn - vstep; k += vstep) {
                        v_uint32 lo32, hi32;
                        v_expand(vx_load(elem + k), lo32, hi32);
                        acc = v_add(acc, v_add(lo32, hi32));
                    }

                    result += (ಪೂರ್ಣಾಙ್ಕ)v_reduce_sum(acc);

                    ಪ್ರತಿ (; k < cn; k++)
                        result += elem[k];
                }
            }
        }
        ಪ್ರತ್ಯಾಗಮ result;
    }
};

template <>
ಸಂರಚನಾ MaskedNormL2_SIMD<ushort, ದ್ವಿಗುಣ> {
    ಅನ್ತಃಸ್ಥ ದ್ವಿಗುಣ operator()(ಧ್ರುವ ushort* src, ಧ್ರುವ uchar* mask, ಪೂರ್ಣಾಙ್ಕ len, ಪೂರ್ಣಾಙ್ಕ cn) ಧ್ರುವ {
        ದ್ವಿಗುಣ result = 0.0;
        ಯದಿ (cn == 1) {
            ಪೂರ್ಣಾಙ್ಕ i = 0;
            ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ vstep = VTraits<v_uint16>::vlanes();
            v_uint64 acc = vx_setzero_u64();
            ಪ್ರತಿ (; i <= len - vstep; i += vstep) {
                v_uint16 m   = vx_load_expand(mask + i);
                v_uint16 cmp = v_gt(m, vx_setzero_u16());
                v_uint16 s   = v_and(vx_load(src + i), cmp);
                v_uint32 lo32, hi32;
                v_expand(s, lo32, hi32);
                v_uint64 lo64a, lo64b, hi64a, hi64b;
                v_expand(v_mul(lo32, lo32), lo64a, lo64b);
                v_expand(v_mul(hi32, hi32), hi64a, hi64b);
                acc = v_add(acc, v_add(v_add(lo64a, lo64b), v_add(hi64a, hi64b)));
            }
            result = (ದ್ವಿಗುಣ)v_reduce_sum(acc);
            ಪ್ರತಿ (; i < len; i++) {
                ಯದಿ (mask[i]) {
                    ದ್ವಿಗುಣ v = src[i];
                    result += v * v;
                }
            }
        }
        ಅನ್ಯಥಾ {
            ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < len; i++) {
                ಯದಿ (mask[i]) {
                    ಧ್ರುವ ushort* elem = src + i * cn;
                    ಪೂರ್ಣಾಙ್ಕ k = 0;
                    ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ vstep = VTraits<v_uint16>::vlanes();
                    v_uint64 acc = vx_setzero_u64();
                    ಪ್ರತಿ (; k <= cn - vstep; k += vstep) {
                        v_uint32 lo32, hi32;
                        v_expand(vx_load(elem + k), lo32, hi32);
                        v_uint64 lo64a, lo64b, hi64a, hi64b;
                        v_expand(v_mul(lo32, lo32), lo64a, lo64b);
                        v_expand(v_mul(hi32, hi32), hi64a, hi64b);
                        acc = v_add(acc, v_add(v_add(lo64a, lo64b), v_add(hi64a, hi64b)));
                    }

                    result += (ದ್ವಿಗುಣ)v_reduce_sum(acc);

                    ಪ್ರತಿ (; k < cn; k++) {
                        ದ್ವಿಗುಣ v = elem[k];
                        result += v * v;
                    }
                }
            }
        }
        ಪ್ರತ್ಯಾಗಮ result;
    }
};

template <>
ಸಂರಚನಾ MaskedNormInf_SIMD<schar, ಪೂರ್ಣಾಙ್ಕ> {
    ಅನ್ತಃಸ್ಥ ಪೂರ್ಣಾಙ್ಕ operator()(ಧ್ರುವ schar* src, ಧ್ರುವ uchar* mask, ಪೂರ್ಣಾಙ್ಕ len, ಪೂರ್ಣಾಙ್ಕ cn) ಧ್ರುವ {
        ಪೂರ್ಣಾಙ್ಕ result = 0;
        ಯದಿ (cn == 1) {
            ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ vstep = VTraits<v_int8>::vlanes();
            ಪೂರ್ಣಾಙ್ಕ i = 0;
            v_uint8 acc = vx_setzero_u8();
            ಪ್ರತಿ (; i <= len - vstep; i += vstep)
                acc = v_max(acc, v_and(v_abs(vx_load(src + i)), v_normmask_u8(mask + i)));
            result = (ಪೂರ್ಣಾಙ್ಕ)v_reduce_max(acc);
            ಪ್ರತಿ (; i < len; i++)
                ಯದಿ (mask[i]) result = std::max(result, cv_abs(src[i]));
        }
        ಅನ್ಯಥಾ {
            ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < len; i++)
                ಯದಿ (mask[i]) {
                    ಧ್ರುವ schar* elem = src + i * cn;
                    ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ k = 0; k < cn; k++)
                        result = std::max(result, cv_abs(elem[k]));
                }
        }
        ಪ್ರತ್ಯಾಗಮ result;
    }
};

template <>
ಸಂರಚನಾ MaskedNormL1_SIMD<schar, ಪೂರ್ಣಾಙ್ಕ> {
    ಅನ್ತಃಸ್ಥ ಪೂರ್ಣಾಙ್ಕ operator()(ಧ್ರುವ schar* src, ಧ್ರುವ uchar* mask, ಪೂರ್ಣಾಙ್ಕ len, ಪೂರ್ಣಾಙ್ಕ cn) ಧ್ರುವ {
        ಪೂರ್ಣಾಙ್ಕ result = 0;
        ಯದಿ (cn == 1) {
            ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ vstep = VTraits<v_int8>::vlanes();
            ಧ್ರುವ v_uint8 one = vx_setall_u8(1);
            ಪೂರ್ಣಾಙ್ಕ i = 0;
            v_uint32 acc = vx_setzero_u32();
            ಪ್ರತಿ (; i <= len - vstep; i += vstep)
                acc = v_dotprod_expand_fast(v_and(v_abs(vx_load(src + i)), v_normmask_u8(mask + i)), one, acc);
            result = (ಪೂರ್ಣಾಙ್ಕ)v_reduce_sum(acc);
            ಪ್ರತಿ (; i < len; i++)
                ಯದಿ (mask[i]) result += cv_abs(src[i]);
        }
        ಅನ್ಯಥಾ {
            ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < len; i++)
                ಯದಿ (mask[i]) {
                    ಧ್ರುವ schar* elem = src + i * cn;
                    ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ k = 0; k < cn; k++)
                        result += cv_abs(elem[k]);
                }
        }
        ಪ್ರತ್ಯಾಗಮ result;
    }
};

template <>
ಸಂರಚನಾ MaskedNormL2_SIMD<schar, ಪೂರ್ಣಾಙ್ಕ> {
    ಅನ್ತಃಸ್ಥ ಪೂರ್ಣಾಙ್ಕ operator()(ಧ್ರುವ schar* src, ಧ್ರುವ uchar* mask, ಪೂರ್ಣಾಙ್ಕ len, ಪೂರ್ಣಾಙ್ಕ cn) ಧ್ರುವ {
        ಪೂರ್ಣಾಙ್ಕ result = 0;
        ಯದಿ (cn == 1) {
            ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ vstep = VTraits<v_int8>::vlanes();
            ಪೂರ್ಣಾಙ್ಕ i = 0;
            v_int32 acc = vx_setzero_s32();
            ಪ್ರತಿ (; i <= len - vstep; i += vstep) {
                v_int8 s = v_and(vx_load(src + i), v_reinterpret_as_s8(v_normmask_u8(mask + i)));
                acc = v_dotprod_expand_fast(s, s, acc);
            }
            result = (ಪೂರ್ಣಾಙ್ಕ)v_reduce_sum(acc);
            ಪ್ರತಿ (; i < len; i++)
                ಯದಿ (mask[i]) { ಪೂರ್ಣಾಙ್ಕ v = src[i]; result += v * v; }
        }
        ಅನ್ಯಥಾ {
            ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < len; i++)
                ಯದಿ (mask[i]) {
                    ಧ್ರುವ schar* elem = src + i * cn;
                    ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ k = 0; k < cn; k++) { ಪೂರ್ಣಾಙ್ಕ v = elem[k]; result += v * v; }
                }
        }
        ಪ್ರತ್ಯಾಗಮ result;
    }
};

template <>
ಸಂರಚನಾ MaskedNormL2_SIMD<uchar, ಪೂರ್ಣಾಙ್ಕ> {
    ಅನ್ತಃಸ್ಥ ಪೂರ್ಣಾಙ್ಕ operator()(ಧ್ರುವ uchar* src, ಧ್ರುವ uchar* mask, ಪೂರ್ಣಾಙ್ಕ len, ಪೂರ್ಣಾಙ್ಕ cn) ಧ್ರುವ {
        ಪೂರ್ಣಾಙ್ಕ result = 0;
        ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ vstep = VTraits<v_uint8>::vlanes();
        ಯದಿ (cn == 1) {
            ಪೂರ್ಣಾಙ್ಕ i = 0;
            v_uint32 acc = vx_setzero_u32();
            ಪ್ರತಿ (; i <= len - vstep; i += vstep) {
                v_uint8 s = v_and(vx_load(src + i), v_normmask_u8(mask + i));
                acc = v_dotprod_expand_fast(s, s, acc);
            }
            result = (ಪೂರ್ಣಾಙ್ಕ)v_reduce_sum(acc);
            ಪ್ರತಿ (; i < len; i++)
                ಯದಿ (mask[i]) { ಪೂರ್ಣಾಙ್ಕ v = src[i]; result += v * v; }
        }
        ಅನ್ಯಥಾ ಯದಿ (cn == 4) {
            ಪೂರ್ಣಾಙ್ಕ i = 0;
            v_uint32 acc = vx_setzero_u32();
            ಪ್ರತಿ (; i <= len - vstep; i += vstep) {
                v_uint8 c0, c1, c2, c3;
                v_load_deinterleave(src + i * 4, c0, c1, c2, c3);
                v_uint8 m = v_normmask_u8(mask + i);
                c0 = v_and(c0, m); c1 = v_and(c1, m);
                c2 = v_and(c2, m); c3 = v_and(c3, m);
                acc = v_dotprod_expand_fast(c0, c0, acc);
                acc = v_dotprod_expand_fast(c1, c1, acc);
                acc = v_dotprod_expand_fast(c2, c2, acc);
                acc = v_dotprod_expand_fast(c3, c3, acc);
            }
            result = (ಪೂರ್ಣಾಙ್ಕ)v_reduce_sum(acc);
            ಪ್ರತಿ (; i < len; i++)
                ಯದಿ (mask[i]) {
                    ಧ್ರುವ uchar* elem = src + i * 4;
                    ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ k = 0; k < 4; k++) { ಪೂರ್ಣಾಙ್ಕ v = elem[k]; result += v * v; }
                }
        }
        ಅನ್ಯಥಾ {
            ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < len; i++)
                ಯದಿ (mask[i]) {
                    ಧ್ರುವ uchar* elem = src + i * cn;
                    ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ k = 0; k < cn; k++) { ಪೂರ್ಣಾಙ್ಕ v = elem[k]; result += v * v; }
                }
        }
        ಪ್ರತ್ಯಾಗಮ result;
    }
};

template <>
ಸಂರಚನಾ MaskedNormInf_SIMD<short, ಪೂರ್ಣಾಙ್ಕ> {
    ಅನ್ತಃಸ್ಥ ಪೂರ್ಣಾಙ್ಕ operator()(ಧ್ರುವ short* src, ಧ್ರುವ uchar* mask, ಪೂರ್ಣಾಙ್ಕ len, ಪೂರ್ಣಾಙ್ಕ cn) ಧ್ರುವ {
        ಪೂರ್ಣಾಙ್ಕ result = 0;
        ಯದಿ (cn == 1) {
            ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ vstep = VTraits<v_int16>::vlanes();
            ಪೂರ್ಣಾಙ್ಕ i = 0;
            v_uint16 acc = vx_setzero_u16();
            ಪ್ರತಿ (; i <= len - vstep; i += vstep)
                acc = v_max(acc, v_and(v_abs(vx_load(src + i)), v_normmask_u16(mask + i)));
            result = (ಪೂರ್ಣಾಙ್ಕ)v_reduce_max(acc);
            ಪ್ರತಿ (; i < len; i++)
                ಯದಿ (mask[i]) result = std::max(result, cv_abs(src[i]));
        }
        ಅನ್ಯಥಾ {
            ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < len; i++)
                ಯದಿ (mask[i]) {
                    ಧ್ರುವ short* elem = src + i * cn;
                    ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ k = 0; k < cn; k++)
                        result = std::max(result, cv_abs(elem[k]));
                }
        }
        ಪ್ರತ್ಯಾಗಮ result;
    }
};

template <>
ಸಂರಚನಾ MaskedNormL1_SIMD<short, ಪೂರ್ಣಾಙ್ಕ> {
    ಅನ್ತಃಸ್ಥ ಪೂರ್ಣಾಙ್ಕ operator()(ಧ್ರುವ short* src, ಧ್ರುವ uchar* mask, ಪೂರ್ಣಾಙ್ಕ len, ಪೂರ್ಣಾಙ್ಕ cn) ಧ್ರುವ {
        ಪೂರ್ಣಾಙ್ಕ result = 0;
        ಯದಿ (cn == 1) {
            ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ vstep = VTraits<v_int16>::vlanes();
            ಪೂರ್ಣಾಙ್ಕ i = 0;
            v_uint32 acc0 = vx_setzero_u32(), acc1 = vx_setzero_u32();
            ಪ್ರತಿ (; i <= len - vstep; i += vstep) {
                v_uint16 s = v_and(v_abs(vx_load(src + i)), v_normmask_u16(mask + i));
                v_uint32 lo, hi;
                v_expand(s, lo, hi);
                acc0 = v_add(acc0, lo);
                acc1 = v_add(acc1, hi);
            }
            result = (ಪೂರ್ಣಾಙ್ಕ)v_reduce_sum(v_add(acc0, acc1));
            ಪ್ರತಿ (; i < len; i++)
                ಯದಿ (mask[i]) result += cv_abs(src[i]);
        }
        ಅನ್ಯಥಾ {
            ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < len; i++)
                ಯದಿ (mask[i]) {
                    ಧ್ರುವ short* elem = src + i * cn;
                    ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ k = 0; k < cn; k++)
                        result += cv_abs(elem[k]);
                }
        }
        ಪ್ರತ್ಯಾಗಮ result;
    }
};

template <>
ಸಂರಚನಾ MaskedNormL2_SIMD<short, ದ್ವಿಗುಣ> {
    ಅನ್ತಃಸ್ಥ ದ್ವಿಗುಣ operator()(ಧ್ರುವ short* src, ಧ್ರುವ uchar* mask, ಪೂರ್ಣಾಙ್ಕ len, ಪೂರ್ಣಾಙ್ಕ cn) ಧ್ರುವ {
        ದ್ವಿಗುಣ result = 0.0;
        ಯದಿ (cn == 1) {
            ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ vstep = VTraits<v_int16>::vlanes();
            ಪೂರ್ಣಾಙ್ಕ i = 0;
            v_uint64 acc = vx_setzero_u64();
            ಪ್ರತಿ (; i <= len - vstep; i += vstep) {
                v_uint16 s = v_and(v_abs(vx_load(src + i)), v_normmask_u16(mask + i));
                acc = v_add(acc, v_dotprod_expand_fast(s, s));
            }
            result = (ದ್ವಿಗುಣ)v_reduce_sum(acc);
            ಪ್ರತಿ (; i < len; i++)
                ಯದಿ (mask[i]) { ದ್ವಿಗುಣ v = src[i]; result += v * v; }
        }
        ಅನ್ಯಥಾ {
            ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < len; i++)
                ಯದಿ (mask[i]) {
                    ಧ್ರುವ short* elem = src + i * cn;
                    ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ k = 0; k < cn; k++) { ದ್ವಿಗುಣ v = elem[k]; result += v * v; }
                }
        }
        ಪ್ರತ್ಯಾಗಮ result;
    }
};

template <>
ಸಂರಚನಾ MaskedNormInf_SIMD<ಪೂರ್ಣಾಙ್ಕ, unsigned> {
    ಅನ್ತಃಸ್ಥ unsigned operator()(ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ* src, ಧ್ರುವ uchar* mask, ಪೂರ್ಣಾಙ್ಕ len, ಪೂರ್ಣಾಙ್ಕ cn) ಧ್ರುವ {
        unsigned result = 0;
        ಯದಿ (cn == 1) {
            ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ vstep = VTraits<v_int32>::vlanes();
            ಪೂರ್ಣಾಙ್ಕ i = 0;
            v_uint32 acc = vx_setzero_u32();
            ಪ್ರತಿ (; i <= len - vstep; i += vstep)
                acc = v_max(acc, v_and(v_abs(vx_load(src + i)), v_normmask_u32(mask + i)));
            result = v_reduce_max(acc);
            ಪ್ರತಿ (; i < len; i++)
                ಯದಿ (mask[i]) result = std::max(result, cv_abs(src[i]));
        }
        ಅನ್ಯಥಾ {
            ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < len; i++)
                ಯದಿ (mask[i]) {
                    ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ* elem = src + i * cn;
                    ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ k = 0; k < cn; k++)
                        result = std::max(result, cv_abs(elem[k]));
                }
        }
        ಪ್ರತ್ಯಾಗಮ result;
    }
};

#ಯದಿ CV_SIMD_64F || CV_SIMD_SCALABLE_64F

template <>
ಸಂರಚನಾ MaskedNormL1_SIMD<ಪೂರ್ಣಾಙ್ಕ, ದ್ವಿಗುಣ> {
    ಅನ್ತಃಸ್ಥ ದ್ವಿಗುಣ operator()(ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ* src, ಧ್ರುವ uchar* mask, ಪೂರ್ಣಾಙ್ಕ len, ಪೂರ್ಣಾಙ್ಕ cn) ಧ್ರುವ {
        ದ್ವಿಗುಣ result = 0.0;
        ಯದಿ (cn == 1) {
            ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ vstep = VTraits<v_int32>::vlanes();
            ಪೂರ್ಣಾಙ್ಕ i = 0;
            v_float64 acc0 = vx_setzero_f64(), acc1 = vx_setzero_f64();
            ಪ್ರತಿ (; i <= len - vstep; i += vstep) {
                // Mask the value, then widen and take the magnitude in double. v_abs() does give
                // |INT_MIN| as an unsigned 2147483648, but reinterpreting that back as int32 turns
                // it negative again, and the widening then carries the sign into the accumulator.
                v_int32 s = v_reinterpret_as_s32(v_and(v_reinterpret_as_u32(vx_load(src + i)),
                                                       v_normmask_u32(mask + i)));
                acc0 = v_add(acc0, v_abs(v_cvt_f64(s)));
                acc1 = v_add(acc1, v_abs(v_cvt_f64_ಮೇಲಿನ(s)));
            }
            result = v_reduce_sum(v_add(acc0, acc1));
            ಪ್ರತಿ (; i < len; i++)
                ಯದಿ (mask[i]) result += cv_abs(src[i]);
        }
        ಅನ್ಯಥಾ {
            ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < len; i++)
                ಯದಿ (mask[i]) {
                    ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ* elem = src + i * cn;
                    ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ k = 0; k < cn; k++)
                        result += cv_abs(elem[k]);
                }
        }
        ಪ್ರತ್ಯಾಗಮ result;
    }
};

template <>
ಸಂರಚನಾ MaskedNormL2_SIMD<ಪೂರ್ಣಾಙ್ಕ, ದ್ವಿಗುಣ> {
    ಅನ್ತಃಸ್ಥ ದ್ವಿಗುಣ operator()(ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ* src, ಧ್ರುವ uchar* mask, ಪೂರ್ಣಾಙ್ಕ len, ಪೂರ್ಣಾಙ್ಕ cn) ಧ್ರುವ {
        ದ್ವಿಗುಣ result = 0.0;
        ಯದಿ (cn == 1) {
            ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ vstep = VTraits<v_int32>::vlanes();
            ಪೂರ್ಣಾಙ್ಕ i = 0;
            v_float64 acc0 = vx_setzero_f64(), acc1 = vx_setzero_f64();
            ಪ್ರತಿ (; i <= len - vstep; i += vstep) {
                v_uint32 s = v_and(v_abs(vx_load(src + i)), v_normmask_u32(mask + i));
                v_uint64 e0, e1;
                v_expand(s, e0, e1);
                v_float64 f0 = v_cvt_f64(v_reinterpret_as_s64(e0));
                v_float64 f1 = v_cvt_f64(v_reinterpret_as_s64(e1));
                acc0 = v_fma(f0, f0, acc0);
                acc1 = v_fma(f1, f1, acc1);
            }
            result = v_reduce_sum(v_add(acc0, acc1));
            ಪ್ರತಿ (; i < len; i++)
                ಯದಿ (mask[i]) { ದ್ವಿಗುಣ v = src[i]; result += v * v; }
        }
        ಅನ್ಯಥಾ {
            ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < len; i++)
                ಯದಿ (mask[i]) {
                    ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ* elem = src + i * cn;
                    ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ k = 0; k < cn; k++) { ದ್ವಿಗುಣ v = elem[k]; result += v * v; }
                }
        }
        ಪ್ರತ್ಯಾಗಮ result;
    }
};

template <>
ಸಂರಚನಾ MaskedNormInf_SIMD<ದ್ವಿಗುಣ, ದ್ವಿಗುಣ> {
    ಅನ್ತಃಸ್ಥ ದ್ವಿಗುಣ operator()(ಧ್ರುವ ದ್ವಿಗುಣ* src, ಧ್ರುವ uchar* mask, ಪೂರ್ಣಾಙ್ಕ len, ಪೂರ್ಣಾಙ್ಕ cn) ಧ್ರುವ {
        ದ್ವಿಗುಣ result = 0.0;
        ಯದಿ (cn == 1) {
            ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ vstep = VTraits<v_float64>::vlanes();
            ಪೂರ್ಣಾಙ್ಕ i = 0;
            v_float64 acc0 = vx_setzero_f64(), acc1 = vx_setzero_f64();
            ಪ್ರತಿ (; i <= len - 2*vstep; i += 2*vstep) {
                v_uint64 m0, m1;
                v_normmask_u64(mask + i, m0, m1);
                acc0 = v_max(acc0, v_reinterpret_as_f64(v_and(v_reinterpret_as_u64(v_abs(vx_load(src + i))), m0)));
                acc1 = v_max(acc1, v_reinterpret_as_f64(v_and(v_reinterpret_as_u64(v_abs(vx_load(src + i + vstep))), m1)));
            }
            ದ್ವಿಗುಣ t[VTraits<v_float64>::max_nlanes];
            vx_store(t, v_max(acc0, acc1));
            ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ k = 0; k < vstep; k++)
                result = std::max(result, t[k]);
            ಪ್ರತಿ (; i < len; i++)
                ಯದಿ (mask[i]) result = std::max(result, cv_abs(src[i]));
        }
        ಅನ್ಯಥಾ {
            ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < len; i++)
                ಯದಿ (mask[i]) {
                    ಧ್ರುವ ದ್ವಿಗುಣ* elem = src + i * cn;
                    ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ k = 0; k < cn; k++)
                        result = std::max(result, cv_abs(elem[k]));
                }
        }
        ಪ್ರತ್ಯಾಗಮ result;
    }
};

template <>
ಸಂರಚನಾ MaskedNormL1_SIMD<ದ್ವಿಗುಣ, ದ್ವಿಗುಣ> {
    ಅನ್ತಃಸ್ಥ ದ್ವಿಗುಣ operator()(ಧ್ರುವ ದ್ವಿಗುಣ* src, ಧ್ರುವ uchar* mask, ಪೂರ್ಣಾಙ್ಕ len, ಪೂರ್ಣಾಙ್ಕ cn) ಧ್ರುವ {
        ದ್ವಿಗುಣ result = 0.0;
        ಯದಿ (cn == 1) {
            ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ vstep = VTraits<v_float64>::vlanes();
            ಪೂರ್ಣಾಙ್ಕ i = 0;
            v_float64 acc0 = vx_setzero_f64(), acc1 = vx_setzero_f64();
            ಪ್ರತಿ (; i <= len - 2*vstep; i += 2*vstep) {
                v_uint64 m0, m1;
                v_normmask_u64(mask + i, m0, m1);
                acc0 = v_add(acc0, v_reinterpret_as_f64(v_and(v_reinterpret_as_u64(v_abs(vx_load(src + i))), m0)));
                acc1 = v_add(acc1, v_reinterpret_as_f64(v_and(v_reinterpret_as_u64(v_abs(vx_load(src + i + vstep))), m1)));
            }
            result = v_reduce_sum(v_add(acc0, acc1));
            ಪ್ರತಿ (; i < len; i++)
                ಯದಿ (mask[i]) result += cv_abs(src[i]);
        }
        ಅನ್ಯಥಾ {
            ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < len; i++)
                ಯದಿ (mask[i]) {
                    ಧ್ರುವ ದ್ವಿಗುಣ* elem = src + i * cn;
                    ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ k = 0; k < cn; k++)
                        result += cv_abs(elem[k]);
                }
        }
        ಪ್ರತ್ಯಾಗಮ result;
    }
};

template <>
ಸಂರಚನಾ MaskedNormL2_SIMD<ದ್ವಿಗುಣ, ದ್ವಿಗುಣ> {
    ಅನ್ತಃಸ್ಥ ದ್ವಿಗುಣ operator()(ಧ್ರುವ ದ್ವಿಗುಣ* src, ಧ್ರುವ uchar* mask, ಪೂರ್ಣಾಙ್ಕ len, ಪೂರ್ಣಾಙ್ಕ cn) ಧ್ರುವ {
        ದ್ವಿಗುಣ result = 0.0;
        ಯದಿ (cn == 1) {
            ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ vstep = VTraits<v_float64>::vlanes();
            ಪೂರ್ಣಾಙ್ಕ i = 0;
            v_float64 acc0 = vx_setzero_f64(), acc1 = vx_setzero_f64();
            ಪ್ರತಿ (; i <= len - 2*vstep; i += 2*vstep) {
                v_uint64 m0, m1;
                v_normmask_u64(mask + i, m0, m1);
                v_float64 s0 = v_reinterpret_as_f64(v_and(v_reinterpret_as_u64(vx_load(src + i)), m0));
                v_float64 s1 = v_reinterpret_as_f64(v_and(v_reinterpret_as_u64(vx_load(src + i + vstep)), m1));
                acc0 = v_fma(s0, s0, acc0);
                acc1 = v_fma(s1, s1, acc1);
            }
            result = v_reduce_sum(v_add(acc0, acc1));
            ಪ್ರತಿ (; i < len; i++)
                ಯದಿ (mask[i]) { ದ್ವಿಗುಣ v = src[i]; result += v * v; }
        }
        ಅನ್ಯಥಾ {
            ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < len; i++)
                ಯದಿ (mask[i]) {
                    ಧ್ರುವ ದ್ವಿಗುಣ* elem = src + i * cn;
                    ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ k = 0; k < cn; k++) { ದ್ವಿಗುಣ v = elem[k]; result += v * v; }
                }
        }
        ಪ್ರತ್ಯಾಗಮ result;
    }
};

#endif // CV_SIMD_64F || CV_SIMD_SCALABLE_64F
#endif // CV_SIMD || CV_SIMD_SCALABLE

template<typename T, typename ST> ಪೂರ್ಣಾಙ್ಕ
normInf_(ಧ್ರುವ T* src, ಧ್ರುವ uchar* mask, ST* _result, ಪೂರ್ಣಾಙ್ಕ len, ಪೂರ್ಣಾಙ್ಕ cn)
{
    ST result = *_result;
    ಯದಿ( !mask )
    {
        NormInf_SIMD<T, ST> op;
        result = std::max(result, op(src, len*cn));
    } ಅನ್ಯಥಾ {
        MaskedNormInf_SIMD<T, ST> op;
        result = std::max(result, op(src, mask, len, cn));
    }
    *_result = result;
    ಪ್ರತ್ಯಾಗಮ 0;
}

template<typename T, typename ST> ಪೂರ್ಣಾಙ್ಕ
normL1_(ಧ್ರುವ T* src, ಧ್ರುವ uchar* mask, ST* _result, ಪೂರ್ಣಾಙ್ಕ len, ಪೂರ್ಣಾಙ್ಕ cn)
{
    ST result = *_result;
    ಯದಿ( !mask )
    {
        NormL1_SIMD<T, ST> op;
        result += op(src, len*cn);
    } ಅನ್ಯಥಾ {
        MaskedNormL1_SIMD<T, ST> op;
        result += op(src, mask, len, cn);
    }
    *_result = result;
    ಪ್ರತ್ಯಾಗಮ 0;
}

template<typename T, typename ST> ಪೂರ್ಣಾಙ್ಕ
normL2_(ಧ್ರುವ T* src, ಧ್ರುವ uchar* mask, ST* _result, ಪೂರ್ಣಾಙ್ಕ len, ಪೂರ್ಣಾಙ್ಕ cn)
{
    ST result = *_result;
    ಯದಿ( !mask )
    {
        NormL2_SIMD<T, ST> op;
        result += op(src, len*cn);
    }
    ಅನ್ಯಥಾ
    {
        MaskedNormL2_SIMD<T, ST> op;
        result += op(src, mask, len, cn);
    }
    *_result = result;
    ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
normInf_Bool(ಧ್ರುವ uchar* src, ಧ್ರುವ uchar* mask, ಪೂರ್ಣಾಙ್ಕ* _result, ಪೂರ್ಣಾಙ್ಕ len, ಪೂರ್ಣಾಙ್ಕ cn)
{
    ಪೂರ್ಣಾಙ್ಕ result = *_result;
    ಯದಿ( !mask )
    {
        ಪ್ರತಿ ( ಪೂರ್ಣಾಙ್ಕ i = 0; i < len*cn; i++ ) {
            result = std::max(result, (ಪೂರ್ಣಾಙ್ಕ)(src[i] != 0));
            ಯದಿ (result != 0)
                ಭಙ್ಗ;
        }
    }
    ಅನ್ಯಥಾ
    {
        ಪ್ರತಿ( ಪೂರ್ಣಾಙ್ಕ i = 0; i < len; i++, src += cn )
            ಯದಿ( mask[i] )
            {
                ಪ್ರತಿ( ಪೂರ್ಣಾಙ್ಕ k = 0; k < cn; k++ )
                    result = std::max(result, (ಪೂರ್ಣಾಙ್ಕ)(src[k] != 0));
                ಯದಿ (result != 0)
                    ಭಙ್ಗ;
            }
    }
    *_result = result;
    ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
normL1_Bool(ಧ್ರುವ uchar* src, ಧ್ರುವ uchar* mask, ಪೂರ್ಣಾಙ್ಕ* _result, ಪೂರ್ಣಾಙ್ಕ len, ಪೂರ್ಣಾಙ್ಕ cn)
{
    ಪೂರ್ಣಾಙ್ಕ result = *_result;
    ಯದಿ( !mask )
    {
        ಪ್ರತಿ ( ಪೂರ್ಣಾಙ್ಕ i = 0; i < len*cn; i++ )
            result += (ಪೂರ್ಣಾಙ್ಕ)(src[i] != 0);
    }
    ಅನ್ಯಥಾ
    {
        ಪ್ರತಿ( ಪೂರ್ಣಾಙ್ಕ i = 0; i < len; i++, src += cn )
            ಯದಿ( mask[i] )
            {
                ಪ್ರತಿ( ಪೂರ್ಣಾಙ್ಕ k = 0; k < cn; k++ )
                    result += (ಪೂರ್ಣಾಙ್ಕ)(src[k] != 0);
            }
    }
    *_result = result;
    ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
normL2_Bool(ಧ್ರುವ uchar* src, ಧ್ರುವ uchar* mask, ಪೂರ್ಣಾಙ್ಕ* _result, ಪೂರ್ಣಾಙ್ಕ len, ಪೂರ್ಣಾಙ್ಕ cn)
{
    ಪ್ರತ್ಯಾಗಮ normL1_Bool(src, mask, _result, len, cn);
}

// ====================================================================
// Masked norm-diff kernels.
// Scalar base templates (safe for every depth) plus SIMD cn==1
// specializations for the concrete (T, ST) pairs dispatched by
// CV_DEF_NORM_ALL. The masked normDiff*_ functions below select them.
// ====================================================================
template <typename T, typename ST>
ಸಂರಚನಾ MaskedNormDiffInf_SIMD {
    ಅನ್ತಃಸ್ಥ ST operator()(ಧ್ರುವ T* s1, ಧ್ರುವ T* s2, ಧ್ರುವ uchar* mask, ಪೂರ್ಣಾಙ್ಕ len, ಪೂರ್ಣಾಙ್ಕ cn) ಧ್ರುವ {
        ST result = 0;
        ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < len; i++) ಯದಿ (mask[i]) {
            ಧ್ರುವ T* e1 = s1 + i*cn; ಧ್ರುವ T* e2 = s2 + i*cn;
            ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ k = 0; k < cn; k++)
                result = std::max(result, (ST)cv_absdiff(e1[k], e2[k]));
        }
        ಪ್ರತ್ಯಾಗಮ result;
    }
};
template <typename T, typename ST>
ಸಂರಚನಾ MaskedNormDiffL1_SIMD {
    ಅನ್ತಃಸ್ಥ ST operator()(ಧ್ರುವ T* s1, ಧ್ರುವ T* s2, ಧ್ರುವ uchar* mask, ಪೂರ್ಣಾಙ್ಕ len, ಪೂರ್ಣಾಙ್ಕ cn) ಧ್ರುವ {
        ST result = 0;
        ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < len; i++) ಯದಿ (mask[i]) {
            ಧ್ರುವ T* e1 = s1 + i*cn; ಧ್ರುವ T* e2 = s2 + i*cn;
            ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ k = 0; k < cn; k++)
                result += (ST)cv_absdiff(e1[k], e2[k]);
        }
        ಪ್ರತ್ಯಾಗಮ result;
    }
};
template <typename T, typename ST>
ಸಂರಚನಾ MaskedNormDiffL2_SIMD {
    ಅನ್ತಃಸ್ಥ ST operator()(ಧ್ರುವ T* s1, ಧ್ರುವ T* s2, ಧ್ರುವ uchar* mask, ಪೂರ್ಣಾಙ್ಕ len, ಪೂರ್ಣಾಙ್ಕ cn) ಧ್ರುವ {
        ST result = 0;
        ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < len; i++) ಯದಿ (mask[i]) {
            ಧ್ರುವ T* e1 = s1 + i*cn; ಧ್ರುವ T* e2 = s2 + i*cn;
            ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ k = 0; k < cn; k++) { ST v = (ST)e1[k] - (ST)e2[k]; result += v*v; }
        }
        ಪ್ರತ್ಯಾಗಮ result;
    }
};

#ಯದಿ (CV_SIMD || CV_SIMD_SCALABLE)
// AND a per-lane diff vector with the (expanded) mask>0 predicate.
ಅನ್ತಃಸ್ಥ v_uint8  v_diffmask(ಧ್ರುವ v_uint8&  d, ಧ್ರುವ uchar* m) { ಪ್ರತ್ಯಾಗಮ v_and(d, v_gt(vx_load(m),          vx_setzero_u8())); }
ಅನ್ತಃಸ್ಥ v_uint16 v_diffmask(ಧ್ರುವ v_uint16& d, ಧ್ರುವ uchar* m) { ಪ್ರತ್ಯಾಗಮ v_and(d, v_gt(vx_load_expand(m),   vx_setzero_u16())); }
ಅನ್ತಃಸ್ಥ v_uint32 v_diffmask(ಧ್ರುವ v_uint32& d, ಧ್ರುವ uchar* m) { ಪ್ರತ್ಯಾಗಮ v_and(d, v_gt(vx_load_expand_q(m), vx_setzero_u32())); }
ಅನ್ತಃಸ್ಥ v_float32 v_diffmask(ಧ್ರುವ v_float32& d, ಧ್ರುವ uchar* m) {
    v_uint32 cm = v_gt(vx_load_expand_q(m), vx_setzero_u32());
    ಪ್ರತ್ಯಾಗಮ v_reinterpret_as_f32(v_and(v_reinterpret_as_u32(d), cm));
}

// Shared scalar cn>1 fallbacks (match the base templates' cv_absdiff semantics).
template<typename T, typename RT>
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ RT maskedNormDiffInfTail(ಧ್ರುವ T* s1, ಧ್ರುವ T* s2, ಧ್ರುವ uchar* mask, ಪೂರ್ಣಾಙ್ಕ len, ಪೂರ್ಣಾಙ್ಕ cn, RT result) {
    ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < len; i++) ಯದಿ (mask[i]) {
        ಧ್ರುವ T* e1 = s1 + i*cn; ಧ್ರುವ T* e2 = s2 + i*cn;
        ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ k = 0; k < cn; k++) result = std::max(result, (RT)cv_absdiff(e1[k], e2[k]));
    }
    ಪ್ರತ್ಯಾಗಮ result;
}
template<typename T, typename RT>
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ RT maskedNormDiffL1Tail(ಧ್ರುವ T* s1, ಧ್ರುವ T* s2, ಧ್ರುವ uchar* mask, ಪೂರ್ಣಾಙ್ಕ len, ಪೂರ್ಣಾಙ್ಕ cn, RT result) {
    ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < len; i++) ಯದಿ (mask[i]) {
        ಧ್ರುವ T* e1 = s1 + i*cn; ಧ್ರುವ T* e2 = s2 + i*cn;
        ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ k = 0; k < cn; k++) result += (RT)cv_absdiff(e1[k], e2[k]);
    }
    ಪ್ರತ್ಯಾಗಮ result;
}
template<typename T, typename RT>
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ RT maskedNormDiffL2Tail(ಧ್ರುವ T* s1, ಧ್ರುವ T* s2, ಧ್ರುವ uchar* mask, ಪೂರ್ಣಾಙ್ಕ len, ಪೂರ್ಣಾಙ್ಕ cn, RT result) {
    ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < len; i++) ಯದಿ (mask[i]) {
        ಧ್ರುವ T* e1 = s1 + i*cn; ಧ್ರುವ T* e2 = s2 + i*cn;
        ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ k = 0; k < cn; k++) { RT v = (RT)e1[k] - (RT)e2[k]; result += v*v; }
    }
    ಪ್ರತ್ಯಾಗಮ result;
}

// Inf (cn==1): reduce_max of masked |a-b|. Shared across SIMD-capable depths.
template<typename T, typename ST>
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ST maskedNormDiffInfCn1(ಧ್ರುವ T* s1, ಧ್ರುವ T* s2, ಧ್ರುವ uchar* mask, ಪೂರ್ಣಾಙ್ಕ len) {
    ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ vstep = VTraits<decltype(vx_load(s1))>::vlanes();
    ST result = 0;
    ಪೂರ್ಣಾಙ್ಕ i = 0;
    ಯದಿ (len >= vstep) {
        auto acc = v_diffmask(v_absdiff(vx_load(s1), vx_load(s2)), mask);
        ಪ್ರತಿ (i = vstep; i <= len - vstep; i += vstep)
            acc = v_max(acc, v_diffmask(v_absdiff(vx_load(s1 + i), vx_load(s2 + i)), mask + i));
        result = (ST)v_reduce_max(acc);
    }
    ಪ್ರತಿ (; i < len; i++) ಯದಿ (mask[i]) result = std::max(result, (ST)cv_absdiff(s1[i], s2[i]));
    ಪ್ರತ್ಯಾಗಮ result;
}

// Shared 8-bit cn==1 masked L1/L2 kernels (uchar/schar): v_absdiff yields v_uint8 for both.
template<typename T>
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಪೂರ್ಣಾಙ್ಕ maskedNormDiffL1_8(ಧ್ರುವ T* s1, ಧ್ರುವ T* s2, ಧ್ರುವ uchar* mask, ಪೂರ್ಣಾಙ್ಕ len) {
    ಪೂರ್ಣಾಙ್ಕ i = 0; ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ vstep = VTraits<v_uint8>::vlanes();
    ಧ್ರುವ v_uint8 one = vx_setall_u8(1);
    v_uint32 acc = vx_setzero_u32();
    ಪ್ರತಿ (; i <= len - vstep; i += vstep) {
        v_uint8 m  = v_gt(vx_load(mask + i), vx_setzero_u8());
        v_uint8 ad = v_and(v_absdiff(vx_load(s1 + i), vx_load(s2 + i)), m);
        acc = v_dotprod_expand_fast(ad, one, acc);
    }
    ಪೂರ್ಣಾಙ್ಕ result = (ಪೂರ್ಣಾಙ್ಕ)v_reduce_sum(acc);
    ಪ್ರತಿ (; i < len; i++) ಯದಿ (mask[i]) result += (ಪೂರ್ಣಾಙ್ಕ)cv_absdiff(s1[i], s2[i]);
    ಪ್ರತ್ಯಾಗಮ result;
}
template<typename T>
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಪೂರ್ಣಾಙ್ಕ maskedNormDiffL2_8(ಧ್ರುವ T* s1, ಧ್ರುವ T* s2, ಧ್ರುವ uchar* mask, ಪೂರ್ಣಾಙ್ಕ len) {
    ಪೂರ್ಣಾಙ್ಕ i = 0; ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ vstep = VTraits<v_uint8>::vlanes();
    v_uint32 acc = vx_setzero_u32();
    ಪ್ರತಿ (; i <= len - vstep; i += vstep) {
        v_uint8 m  = v_gt(vx_load(mask + i), vx_setzero_u8());
        v_uint8 ad = v_and(v_absdiff(vx_load(s1 + i), vx_load(s2 + i)), m);
        acc = v_dotprod_expand_fast(ad, ad, acc);
    }
    ಪೂರ್ಣಾಙ್ಕ result = (ಪೂರ್ಣಾಙ್ಕ)v_reduce_sum(acc);
    ಪ್ರತಿ (; i < len; i++) ಯದಿ (mask[i]) { ಪೂರ್ಣಾಙ್ಕ v = (ಪೂರ್ಣಾಙ್ಕ)s1[i] - (ಪೂರ್ಣಾಙ್ಕ)s2[i]; result += v*v; }
    ಪ್ರತ್ಯಾಗಮ result;
}

// Shared 16-bit cn==1 masked L1 kernel (ushort/short): v_absdiff yields v_uint16 for both.
template<typename T>
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಪೂರ್ಣಾಙ್ಕ maskedNormDiffL1_16(ಧ್ರುವ T* s1, ಧ್ರುವ T* s2, ಧ್ರುವ uchar* mask, ಪೂರ್ಣಾಙ್ಕ len) {
    ಪೂರ್ಣಾಙ್ಕ i = 0; ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ vstep = VTraits<v_uint16>::vlanes();
    v_uint32 acc = vx_setzero_u32();
    ಪ್ರತಿ (; i <= len - vstep; i += vstep) {
        v_uint16 m  = v_gt(vx_load_expand(mask + i), vx_setzero_u16());
        v_uint16 ad = v_and(v_absdiff(vx_load(s1 + i), vx_load(s2 + i)), m);
        v_uint32 lo, hi; v_expand(ad, lo, hi);
        acc = v_add(acc, v_add(lo, hi));
    }
    ಪೂರ್ಣಾಙ್ಕ result = (ಪೂರ್ಣಾಙ್ಕ)v_reduce_sum(acc);
    ಪ್ರತಿ (; i < len; i++) ಯದಿ (mask[i]) result += (ಪೂರ್ಣಾಙ್ಕ)cv_absdiff(s1[i], s2[i]);
    ಪ್ರತ್ಯಾಗಮ result;
}

template<> ಸಂರಚನಾ MaskedNormDiffInf_SIMD<uchar, ಪೂರ್ಣಾಙ್ಕ> {
    ಪೂರ್ಣಾಙ್ಕ operator()(ಧ್ರುವ uchar* s1, ಧ್ರುವ uchar* s2, ಧ್ರುವ uchar* mask, ಪೂರ್ಣಾಙ್ಕ len, ಪೂರ್ಣಾಙ್ಕ cn) ಧ್ರುವ {
        ಯದಿ (cn == 1) ಪ್ರತ್ಯಾಗಮ maskedNormDiffInfCn1<uchar, ಪೂರ್ಣಾಙ್ಕ>(s1, s2, mask, len);
        ಯದಿ (cn == 4 && len >= VTraits<v_uint8>::vlanes()) {
            ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ vstep = VTraits<v_uint8>::vlanes();
            v_uint8 acc = vx_setzero_u8();
            ಪೂರ್ಣಾಙ್ಕ i = 0;
            ಪ್ರತಿ (;;) {
                ಯದಿ (i > len - vstep) i = len - vstep; // back-step (max is idempotent)
                v_uint8 a0,a1,a2,a3,b0,b1,b2,b3;
                v_load_deinterleave(s1 + i*4, a0,a1,a2,a3);
                v_load_deinterleave(s2 + i*4, b0,b1,b2,b3);
                v_uint8 ad = v_max(v_max(v_absdiff(a0,b0), v_absdiff(a1,b1)),
                                   v_max(v_absdiff(a2,b2), v_absdiff(a3,b3)));
                v_uint8 m = v_gt(vx_load(mask + i), vx_setzero_u8());
                acc = v_max(acc, v_and(ad, m));
                ಯದಿ (i >= len - vstep) ಭಙ್ಗ;
                i += vstep;
            }
            ಪ್ರತ್ಯಾಗಮ (ಪೂರ್ಣಾಙ್ಕ)v_reduce_max(acc);
        }
        ಪ್ರತ್ಯಾಗಮ maskedNormDiffInfTail<uchar, ಪೂರ್ಣಾಙ್ಕ>(s1, s2, mask, len, cn, 0);
    }
};
template<> ಸಂರಚನಾ MaskedNormDiffInf_SIMD<schar, ಪೂರ್ಣಾಙ್ಕ> {
    ಪೂರ್ಣಾಙ್ಕ operator()(ಧ್ರುವ schar* s1, ಧ್ರುವ schar* s2, ಧ್ರುವ uchar* mask, ಪೂರ್ಣಾಙ್ಕ len, ಪೂರ್ಣಾಙ್ಕ cn) ಧ್ರುವ {
        ಯದಿ (cn == 1) ಪ್ರತ್ಯಾಗಮ maskedNormDiffInfCn1<schar, ಪೂರ್ಣಾಙ್ಕ>(s1, s2, mask, len);
        ಪ್ರತ್ಯಾಗಮ maskedNormDiffInfTail<schar, ಪೂರ್ಣಾಙ್ಕ>(s1, s2, mask, len, cn, 0);
    }
};
template<> ಸಂರಚನಾ MaskedNormDiffInf_SIMD<ushort, ಪೂರ್ಣಾಙ್ಕ> {
    ಪೂರ್ಣಾಙ್ಕ operator()(ಧ್ರುವ ushort* s1, ಧ್ರುವ ushort* s2, ಧ್ರುವ uchar* mask, ಪೂರ್ಣಾಙ್ಕ len, ಪೂರ್ಣಾಙ್ಕ cn) ಧ್ರುವ {
        ಯದಿ (cn == 1) ಪ್ರತ್ಯಾಗಮ maskedNormDiffInfCn1<ushort, ಪೂರ್ಣಾಙ್ಕ>(s1, s2, mask, len);
        ಪ್ರತ್ಯಾಗಮ maskedNormDiffInfTail<ushort, ಪೂರ್ಣಾಙ್ಕ>(s1, s2, mask, len, cn, 0);
    }
};
template<> ಸಂರಚನಾ MaskedNormDiffInf_SIMD<short, ಪೂರ್ಣಾಙ್ಕ> {
    ಪೂರ್ಣಾಙ್ಕ operator()(ಧ್ರುವ short* s1, ಧ್ರುವ short* s2, ಧ್ರುವ uchar* mask, ಪೂರ್ಣಾಙ್ಕ len, ಪೂರ್ಣಾಙ್ಕ cn) ಧ್ರುವ {
        ಯದಿ (cn == 1) ಪ್ರತ್ಯಾಗಮ maskedNormDiffInfCn1<short, ಪೂರ್ಣಾಙ್ಕ>(s1, s2, mask, len);
        ಪ್ರತ್ಯಾಗಮ maskedNormDiffInfTail<short, ಪೂರ್ಣಾಙ್ಕ>(s1, s2, mask, len, cn, 0);
    }
};
template<> ಸಂರಚನಾ MaskedNormDiffInf_SIMD<ಪೂರ್ಣಾಙ್ಕ, unsigned> {
    unsigned operator()(ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ* s1, ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ* s2, ಧ್ರುವ uchar* mask, ಪೂರ್ಣಾಙ್ಕ len, ಪೂರ್ಣಾಙ್ಕ cn) ಧ್ರುವ {
        ಯದಿ (cn == 1) ಪ್ರತ್ಯಾಗಮ maskedNormDiffInfCn1<ಪೂರ್ಣಾಙ್ಕ, unsigned>(s1, s2, mask, len);
        ಪ್ರತ್ಯಾಗಮ maskedNormDiffInfTail<ಪೂರ್ಣಾಙ್ಕ, unsigned>(s1, s2, mask, len, cn, 0u);
    }
};
template<> ಸಂರಚನಾ MaskedNormDiffInf_SIMD<ಭಿನ್ನ, ಭಿನ್ನ> {
    ಭಿನ್ನ operator()(ಧ್ರುವ ಭಿನ್ನ* s1, ಧ್ರುವ ಭಿನ್ನ* s2, ಧ್ರುವ uchar* mask, ಪೂರ್ಣಾಙ್ಕ len, ಪೂರ್ಣಾಙ್ಕ cn) ಧ್ರುವ {
        ಯದಿ (cn == 1) ಪ್ರತ್ಯಾಗಮ maskedNormDiffInfCn1<ಭಿನ್ನ, ಭಿನ್ನ>(s1, s2, mask, len);
        ಪ್ರತ್ಯಾಗಮ maskedNormDiffInfTail<ಭಿನ್ನ, ಭಿನ್ನ>(s1, s2, mask, len, cn, 0.0f);
    }
};

template<> ಸಂರಚನಾ MaskedNormDiffL1_SIMD<uchar, ಪೂರ್ಣಾಙ್ಕ> {
    ಪೂರ್ಣಾಙ್ಕ operator()(ಧ್ರುವ uchar* s1, ಧ್ರುವ uchar* s2, ಧ್ರುವ uchar* mask, ಪೂರ್ಣಾಙ್ಕ len, ಪೂರ್ಣಾಙ್ಕ cn) ಧ್ರುವ {
        ಯದಿ (cn == 1) ಪ್ರತ್ಯಾಗಮ maskedNormDiffL1_8(s1, s2, mask, len);
        ಯದಿ (cn == 4) {
            ಪೂರ್ಣಾಙ್ಕ i = 0; ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ vstep = VTraits<v_uint8>::vlanes();
            ಧ್ರುವ v_uint8 one = vx_setall_u8(1);
            v_uint32 acc = vx_setzero_u32();
            ಪ್ರತಿ (; i <= len - vstep; i += vstep) {
                v_uint8 a0,a1,a2,a3,b0,b1,b2,b3;
                v_load_deinterleave(s1 + i*4, a0,a1,a2,a3);
                v_load_deinterleave(s2 + i*4, b0,b1,b2,b3);
                v_uint8 m = v_gt(vx_load(mask + i), vx_setzero_u8());
                acc = v_dotprod_expand_fast(v_and(v_absdiff(a0,b0), m), one, acc);
                acc = v_dotprod_expand_fast(v_and(v_absdiff(a1,b1), m), one, acc);
                acc = v_dotprod_expand_fast(v_and(v_absdiff(a2,b2), m), one, acc);
                acc = v_dotprod_expand_fast(v_and(v_absdiff(a3,b3), m), one, acc);
            }
            ಪೂರ್ಣಾಙ್ಕ result = (ಪೂರ್ಣಾಙ್ಕ)v_reduce_sum(acc);
            ಪ್ರತಿ (; i < len; i++) ಯದಿ (mask[i]) {
                ಧ್ರುವ uchar* e1 = s1 + i*4; ಧ್ರುವ uchar* e2 = s2 + i*4;
                ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ k = 0; k < 4; k++) result += (ಪೂರ್ಣಾಙ್ಕ)cv_absdiff(e1[k], e2[k]);
            }
            ಪ್ರತ್ಯಾಗಮ result;
        }
        ಪ್ರತ್ಯಾಗಮ maskedNormDiffL1Tail<uchar, ಪೂರ್ಣಾಙ್ಕ>(s1, s2, mask, len, cn, 0);
    }
};
template<> ಸಂರಚನಾ MaskedNormDiffL1_SIMD<schar, ಪೂರ್ಣಾಙ್ಕ> {
    ಪೂರ್ಣಾಙ್ಕ operator()(ಧ್ರುವ schar* s1, ಧ್ರುವ schar* s2, ಧ್ರುವ uchar* mask, ಪೂರ್ಣಾಙ್ಕ len, ಪೂರ್ಣಾಙ್ಕ cn) ಧ್ರುವ {
        ಯದಿ (cn == 1) ಪ್ರತ್ಯಾಗಮ maskedNormDiffL1_8(s1, s2, mask, len);
        ಪ್ರತ್ಯಾಗಮ maskedNormDiffL1Tail<schar, ಪೂರ್ಣಾಙ್ಕ>(s1, s2, mask, len, cn, 0);
    }
};
template<> ಸಂರಚನಾ MaskedNormDiffL1_SIMD<ushort, ಪೂರ್ಣಾಙ್ಕ> {
    ಪೂರ್ಣಾಙ್ಕ operator()(ಧ್ರುವ ushort* s1, ಧ್ರುವ ushort* s2, ಧ್ರುವ uchar* mask, ಪೂರ್ಣಾಙ್ಕ len, ಪೂರ್ಣಾಙ್ಕ cn) ಧ್ರುವ {
        ಯದಿ (cn == 1) ಪ್ರತ್ಯಾಗಮ maskedNormDiffL1_16(s1, s2, mask, len);
        ಪ್ರತ್ಯಾಗಮ maskedNormDiffL1Tail<ushort, ಪೂರ್ಣಾಙ್ಕ>(s1, s2, mask, len, cn, 0);
    }
};
template<> ಸಂರಚನಾ MaskedNormDiffL1_SIMD<short, ಪೂರ್ಣಾಙ್ಕ> {
    ಪೂರ್ಣಾಙ್ಕ operator()(ಧ್ರುವ short* s1, ಧ್ರುವ short* s2, ಧ್ರುವ uchar* mask, ಪೂರ್ಣಾಙ್ಕ len, ಪೂರ್ಣಾಙ್ಕ cn) ಧ್ರುವ {
        ಯದಿ (cn == 1) ಪ್ರತ್ಯಾಗಮ maskedNormDiffL1_16(s1, s2, mask, len);
        ಪ್ರತ್ಯಾಗಮ maskedNormDiffL1Tail<short, ಪೂರ್ಣಾಙ್ಕ>(s1, s2, mask, len, cn, 0);
    }
};

template<> ಸಂರಚನಾ MaskedNormDiffL2_SIMD<uchar, ಪೂರ್ಣಾಙ್ಕ> {
    ಪೂರ್ಣಾಙ್ಕ operator()(ಧ್ರುವ uchar* s1, ಧ್ರುವ uchar* s2, ಧ್ರುವ uchar* mask, ಪೂರ್ಣಾಙ್ಕ len, ಪೂರ್ಣಾಙ್ಕ cn) ಧ್ರುವ {
        ಯದಿ (cn == 1) ಪ್ರತ್ಯಾಗಮ maskedNormDiffL2_8(s1, s2, mask, len);
        ಯದಿ (cn == 4) {
            ಪೂರ್ಣಾಙ್ಕ i = 0; ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ vstep = VTraits<v_uint8>::vlanes();
            v_uint32 acc = vx_setzero_u32();
            ಪ್ರತಿ (; i <= len - vstep; i += vstep) {
                v_uint8 a0,a1,a2,a3,b0,b1,b2,b3;
                v_load_deinterleave(s1 + i*4, a0,a1,a2,a3);
                v_load_deinterleave(s2 + i*4, b0,b1,b2,b3);
                v_uint8 m = v_gt(vx_load(mask + i), vx_setzero_u8());
                v_uint8 d0 = v_and(v_absdiff(a0,b0), m), d1 = v_and(v_absdiff(a1,b1), m);
                v_uint8 d2 = v_and(v_absdiff(a2,b2), m), d3 = v_and(v_absdiff(a3,b3), m);
                acc = v_dotprod_expand_fast(d0, d0, acc);
                acc = v_dotprod_expand_fast(d1, d1, acc);
                acc = v_dotprod_expand_fast(d2, d2, acc);
                acc = v_dotprod_expand_fast(d3, d3, acc);
            }
            ಪೂರ್ಣಾಙ್ಕ result = (ಪೂರ್ಣಾಙ್ಕ)v_reduce_sum(acc);
            ಪ್ರತಿ (; i < len; i++) ಯದಿ (mask[i]) {
                ಧ್ರುವ uchar* e1 = s1 + i*4; ಧ್ರುವ uchar* e2 = s2 + i*4;
                ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ k = 0; k < 4; k++) { ಪೂರ್ಣಾಙ್ಕ v = (ಪೂರ್ಣಾಙ್ಕ)e1[k] - (ಪೂರ್ಣಾಙ್ಕ)e2[k]; result += v*v; }
            }
            ಪ್ರತ್ಯಾಗಮ result;
        }
        ಪ್ರತ್ಯಾಗಮ maskedNormDiffL2Tail<uchar, ಪೂರ್ಣಾಙ್ಕ>(s1, s2, mask, len, cn, 0);
    }
};
template<> ಸಂರಚನಾ MaskedNormDiffL2_SIMD<schar, ಪೂರ್ಣಾಙ್ಕ> {
    ಪೂರ್ಣಾಙ್ಕ operator()(ಧ್ರುವ schar* s1, ಧ್ರುವ schar* s2, ಧ್ರುವ uchar* mask, ಪೂರ್ಣಾಙ್ಕ len, ಪೂರ್ಣಾಙ್ಕ cn) ಧ್ರುವ {
        ಯದಿ (cn == 1) ಪ್ರತ್ಯಾಗಮ maskedNormDiffL2_8(s1, s2, mask, len);
        ಪ್ರತ್ಯಾಗಮ maskedNormDiffL2Tail<schar, ಪೂರ್ಣಾಙ್ಕ>(s1, s2, mask, len, cn, 0);
    }
};

#ಯದಿ CV_SIMD_64F
// Depths whose L1/L2 accumulate in double.
template<> ಸಂರಚನಾ MaskedNormDiffL1_SIMD<ಪೂರ್ಣಾಙ್ಕ, ದ್ವಿಗುಣ> {
    ದ್ವಿಗುಣ operator()(ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ* s1, ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ* s2, ಧ್ರುವ uchar* mask, ಪೂರ್ಣಾಙ್ಕ len, ಪೂರ್ಣಾಙ್ಕ cn) ಧ್ರುವ {
        ಯದಿ (cn != 1) ಪ್ರತ್ಯಾಗಮ maskedNormDiffL1Tail<ಪೂರ್ಣಾಙ್ಕ, ದ್ವಿಗುಣ>(s1, s2, mask, len, cn, 0.0);
        ಪೂರ್ಣಾಙ್ಕ i = 0; ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ vstep = VTraits<v_int32>::vlanes();
        v_float64 acc = vx_setzero_f64();
        ಪ್ರತಿ (; i <= len - vstep; i += vstep) {
            v_uint32 m  = v_gt(vx_load_expand_q(mask + i), vx_setzero_u32());
            v_uint32 ad = v_and(v_absdiff(vx_load(s1 + i), vx_load(s2 + i)), m);
            v_uint64 e0, e1; v_expand(ad, e0, e1);
            acc = v_add(acc, v_cvt_f64(v_reinterpret_as_s64(e0)));
            acc = v_add(acc, v_cvt_f64(v_reinterpret_as_s64(e1)));
        }
        ದ್ವಿಗುಣ result = v_reduce_sum(acc);
        ಪ್ರತಿ (; i < len; i++) ಯದಿ (mask[i]) result += (ದ್ವಿಗುಣ)cv_absdiff(s1[i], s2[i]);
        ಪ್ರತ್ಯಾಗಮ result;
    }
};
template<> ಸಂರಚನಾ MaskedNormDiffL1_SIMD<ಭಿನ್ನ, ದ್ವಿಗುಣ> {
    ದ್ವಿಗುಣ operator()(ಧ್ರುವ ಭಿನ್ನ* s1, ಧ್ರುವ ಭಿನ್ನ* s2, ಧ್ರುವ uchar* mask, ಪೂರ್ಣಾಙ್ಕ len, ಪೂರ್ಣಾಙ್ಕ cn) ಧ್ರುವ {
        ಯದಿ (cn != 1) ಪ್ರತ್ಯಾಗಮ maskedNormDiffL1Tail<ಭಿನ್ನ, ದ್ವಿಗುಣ>(s1, s2, mask, len, cn, 0.0);
        ಪೂರ್ಣಾಙ್ಕ i = 0; ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ vstep = VTraits<v_float32>::vlanes();
        v_float64 acc0 = vx_setzero_f64(), acc1 = vx_setzero_f64();
        ಪ್ರತಿ (; i <= len - vstep; i += vstep) {
            v_float32 m  = v_reinterpret_as_f32(v_gt(vx_load_expand_q(mask + i), vx_setzero_u32()));
            v_float32 ad = v_and(v_absdiff(vx_load(s1 + i), vx_load(s2 + i)), m);
            acc0 = v_add(acc0, v_cvt_f64(ad));
            acc1 = v_add(acc1, v_cvt_f64_ಮೇಲಿನ(ad));
        }
        ದ್ವಿಗುಣ result = v_reduce_sum(v_add(acc0, acc1));
        ಪ್ರತಿ (; i < len; i++) ಯದಿ (mask[i]) result += (ದ್ವಿಗುಣ)cv_absdiff(s1[i], s2[i]);
        ಪ್ರತ್ಯಾಗಮ result;
    }
};

template<typename T>
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ದ್ವಿಗುಣ maskedNormDiffL2_16(ಧ್ರುವ T* s1, ಧ್ರುವ T* s2, ಧ್ರುವ uchar* mask, ಪೂರ್ಣಾಙ್ಕ len, ಪೂರ್ಣಾಙ್ಕ cn) {
    ಯದಿ (cn != 1) ಪ್ರತ್ಯಾಗಮ maskedNormDiffL2Tail<T, ದ್ವಿಗುಣ>(s1, s2, mask, len, cn, 0.0);
    ಪೂರ್ಣಾಙ್ಕ i = 0; ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ vstep = VTraits<v_uint16>::vlanes();
    v_float64 acc = vx_setzero_f64();
    ಪ್ರತಿ (; i <= len - vstep; i += vstep) {
        v_uint16 m  = v_gt(vx_load_expand(mask + i), vx_setzero_u16());
        v_uint16 ad = v_and(v_absdiff(vx_load(s1 + i), vx_load(s2 + i)), m);
        v_uint64 u = v_dotprod_expand_fast(ad, ad);
        acc = v_add(acc, v_cvt_f64(v_reinterpret_as_s64(u)));
    }
    ದ್ವಿಗುಣ result = v_reduce_sum(acc);
    ಪ್ರತಿ (; i < len; i++) ಯದಿ (mask[i]) { ದ್ವಿಗುಣ v = (ದ್ವಿಗುಣ)s1[i] - (ದ್ವಿಗುಣ)s2[i]; result += v*v; }
    ಪ್ರತ್ಯಾಗಮ result;
}
template<> ಸಂರಚನಾ MaskedNormDiffL2_SIMD<ushort, ದ್ವಿಗುಣ> {
    ದ್ವಿಗುಣ operator()(ಧ್ರುವ ushort* s1, ಧ್ರುವ ushort* s2, ಧ್ರುವ uchar* mask, ಪೂರ್ಣಾಙ್ಕ len, ಪೂರ್ಣಾಙ್ಕ cn) ಧ್ರುವ
    { ಪ್ರತ್ಯಾಗಮ maskedNormDiffL2_16(s1, s2, mask, len, cn); } };
template<> ಸಂರಚನಾ MaskedNormDiffL2_SIMD<short, ದ್ವಿಗುಣ> {
    ದ್ವಿಗುಣ operator()(ಧ್ರುವ short* s1, ಧ್ರುವ short* s2, ಧ್ರುವ uchar* mask, ಪೂರ್ಣಾಙ್ಕ len, ಪೂರ್ಣಾಙ್ಕ cn) ಧ್ರುವ
    { ಪ್ರತ್ಯಾಗಮ maskedNormDiffL2_16(s1, s2, mask, len, cn); } };

template<> ಸಂರಚನಾ MaskedNormDiffL2_SIMD<ಪೂರ್ಣಾಙ್ಕ, ದ್ವಿಗುಣ> {
    ದ್ವಿಗುಣ operator()(ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ* s1, ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ* s2, ಧ್ರುವ uchar* mask, ಪೂರ್ಣಾಙ್ಕ len, ಪೂರ್ಣಾಙ್ಕ cn) ಧ್ರುವ {
        ಯದಿ (cn != 1) ಪ್ರತ್ಯಾಗಮ maskedNormDiffL2Tail<ಪೂರ್ಣಾಙ್ಕ, ದ್ವಿಗುಣ>(s1, s2, mask, len, cn, 0.0);
        ಪೂರ್ಣಾಙ್ಕ i = 0; ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ vstep = VTraits<v_int32>::vlanes();
        v_float64 r0 = vx_setzero_f64(), r1 = vx_setzero_f64();
        ಪ್ರತಿ (; i <= len - vstep; i += vstep) {
            v_uint32 m  = v_gt(vx_load_expand_q(mask + i), vx_setzero_u32());
            v_uint32 ad = v_and(v_absdiff(vx_load(s1 + i), vx_load(s2 + i)), m);
            v_uint64 e0, e1; v_expand(ad, e0, e1);
            v_float64 f0 = v_cvt_f64(v_reinterpret_as_s64(e0)), f1 = v_cvt_f64(v_reinterpret_as_s64(e1));
            r0 = v_fma(f0, f0, r0); r1 = v_fma(f1, f1, r1);
        }
        ದ್ವಿಗುಣ result = v_reduce_sum(v_add(r0, r1));
        ಪ್ರತಿ (; i < len; i++) ಯದಿ (mask[i]) { ದ್ವಿಗುಣ v = (ದ್ವಿಗುಣ)s1[i] - (ದ್ವಿಗುಣ)s2[i]; result += v*v; }
        ಪ್ರತ್ಯಾಗಮ result;
    }
};
template<> ಸಂರಚನಾ MaskedNormDiffL2_SIMD<ಭಿನ್ನ, ದ್ವಿಗುಣ> {
    ದ್ವಿಗುಣ operator()(ಧ್ರುವ ಭಿನ್ನ* s1, ಧ್ರುವ ಭಿನ್ನ* s2, ಧ್ರುವ uchar* mask, ಪೂರ್ಣಾಙ್ಕ len, ಪೂರ್ಣಾಙ್ಕ cn) ಧ್ರುವ {
        ಯದಿ (cn != 1) ಪ್ರತ್ಯಾಗಮ maskedNormDiffL2Tail<ಭಿನ್ನ, ದ್ವಿಗುಣ>(s1, s2, mask, len, cn, 0.0);
        ಪೂರ್ಣಾಙ್ಕ i = 0; ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ vstep = VTraits<v_float32>::vlanes();
        v_float64 r0 = vx_setzero_f64(), r1 = vx_setzero_f64();
        ಪ್ರತಿ (; i <= len - vstep; i += vstep) {
            v_float32 m  = v_reinterpret_as_f32(v_gt(vx_load_expand_q(mask + i), vx_setzero_u32()));
            v_float32 ad = v_and(v_absdiff(vx_load(s1 + i), vx_load(s2 + i)), m);
            v_float64 f0 = v_cvt_f64(ad), f1 = v_cvt_f64_ಮೇಲಿನ(ad);
            r0 = v_fma(f0, f0, r0); r1 = v_fma(f1, f1, r1);
        }
        ದ್ವಿಗುಣ result = v_reduce_sum(v_add(r0, r1));
        ಪ್ರತಿ (; i < len; i++) ಯದಿ (mask[i]) { ದ್ವಿಗುಣ v = (ದ್ವಿಗುಣ)s1[i] - (ದ್ವಿಗುಣ)s2[i]; result += v*v; }
        ಪ್ರತ್ಯಾಗಮ result;
    }
};
template<>
ಸಂರಚನಾ MaskedNormDiffInf_SIMD<ದ್ವಿಗುಣ, ದ್ವಿಗುಣ> {
    ಅನ್ತಃಸ್ಥ ದ್ವಿಗುಣ operator()(ಧ್ರುವ ದ್ವಿಗುಣ* s1, ಧ್ರುವ ದ್ವಿಗುಣ* s2, ಧ್ರುವ uchar* mask, ಪೂರ್ಣಾಙ್ಕ len, ಪೂರ್ಣಾಙ್ಕ cn) ಧ್ರುವ {
        ದ್ವಿಗುಣ result = 0.0;
        ಪೂರ್ಣಾಙ್ಕ i = 0;
#ಯದಿ CV_SIMD_64F || CV_SIMD_SCALABLE_64F
        ಯದಿ (cn == 1) {
            ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ vstep = VTraits<v_float64>::vlanes();
            v_float64 acc0 = vx_setzero_f64(), acc1 = vx_setzero_f64();
            ಪ್ರತಿ (; i <= len - 2*vstep; i += 2*vstep) {
                v_uint64 m0, m1;
                v_normmask_u64(mask + i, m0, m1);
                v_float64 d0 = v_absdiff(vx_load(s1 + i), vx_load(s2 + i));
                v_float64 d1 = v_absdiff(vx_load(s1 + i + vstep), vx_load(s2 + i + vstep));
                acc0 = v_max(acc0, v_reinterpret_as_f64(v_and(v_reinterpret_as_u64(d0), m0)));
                acc1 = v_max(acc1, v_reinterpret_as_f64(v_and(v_reinterpret_as_u64(d1), m1)));
            }
            ದ್ವಿಗುಣ t[VTraits<v_float64>::max_nlanes];
            vx_store(t, v_max(acc0, acc1));
            ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ k = 0; k < vstep; k++)
                result = std::max(result, t[k]);
        }
#endif
        ಪ್ರತಿ (; i < len; i++) ಯದಿ (mask[i]) {
            ಧ್ರುವ ದ್ವಿಗುಣ* e1 = s1 + i*cn; ಧ್ರುವ ದ್ವಿಗುಣ* e2 = s2 + i*cn;
            ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ k = 0; k < cn; k++) result = std::max(result, std::abs(e1[k] - e2[k]));
        }
        ಪ್ರತ್ಯಾಗಮ result;
    }
};

template<>
ಸಂರಚನಾ MaskedNormDiffL1_SIMD<ದ್ವಿಗುಣ, ದ್ವಿಗುಣ> {
    ದ್ವಿಗುಣ operator()(ಧ್ರುವ ದ್ವಿಗುಣ* s1, ಧ್ರುವ ದ್ವಿಗುಣ* s2, ಧ್ರುವ uchar* mask, ಪೂರ್ಣಾಙ್ಕ len, ಪೂರ್ಣಾಙ್ಕ cn) ಧ್ರುವ {
        ದ್ವಿಗುಣ result = 0.0;
        ಯದಿ (cn == 1) {
            ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ vstep = VTraits<v_float64>::vlanes();
            ಪೂರ್ಣಾಙ್ಕ i = 0;
            v_float64 acc0 = vx_setzero_f64(), acc1 = vx_setzero_f64();
            ಪ್ರತಿ (; i <= len - 2*vstep; i += 2*vstep) {
                v_uint64 m0, m1;
                v_normmask_u64(mask + i, m0, m1);
                v_float64 d0 = v_absdiff(vx_load(s1 + i), vx_load(s2 + i));
                v_float64 d1 = v_absdiff(vx_load(s1 + i + vstep), vx_load(s2 + i + vstep));
                acc0 = v_add(acc0, v_reinterpret_as_f64(v_and(v_reinterpret_as_u64(d0), m0)));
                acc1 = v_add(acc1, v_reinterpret_as_f64(v_and(v_reinterpret_as_u64(d1), m1)));
            }
            result = v_reduce_sum(v_add(acc0, acc1));
            ಪ್ರತಿ (; i < len; i++) ಯದಿ (mask[i]) result += std::abs(s1[i] - s2[i]);
        } ಅನ್ಯಥಾ {
            result = maskedNormDiffL1Tail<ದ್ವಿಗುಣ, ದ್ವಿಗುಣ>(s1, s2, mask, len, cn, 0.0);
        }
        ಪ್ರತ್ಯಾಗಮ result;
    }
};

template<>
ಸಂರಚನಾ MaskedNormDiffL2_SIMD<ದ್ವಿಗುಣ, ದ್ವಿಗುಣ> {
    ದ್ವಿಗುಣ operator()(ಧ್ರುವ ದ್ವಿಗುಣ* s1, ಧ್ರುವ ದ್ವಿಗುಣ* s2, ಧ್ರುವ uchar* mask, ಪೂರ್ಣಾಙ್ಕ len, ಪೂರ್ಣಾಙ್ಕ cn) ಧ್ರುವ {
        ದ್ವಿಗುಣ result = 0.0;
        ಯದಿ (cn == 1) {
            ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ vstep = VTraits<v_float64>::vlanes();
            ಪೂರ್ಣಾಙ್ಕ i = 0;
            v_float64 acc0 = vx_setzero_f64(), acc1 = vx_setzero_f64();
            ಪ್ರತಿ (; i <= len - 2*vstep; i += 2*vstep) {
                v_uint64 m0, m1;
                v_normmask_u64(mask + i, m0, m1);
                v_float64 d0 = v_sub(vx_load(s1 + i), vx_load(s2 + i));
                v_float64 d1 = v_sub(vx_load(s1 + i + vstep), vx_load(s2 + i + vstep));
                d0 = v_reinterpret_as_f64(v_and(v_reinterpret_as_u64(d0), m0));
                d1 = v_reinterpret_as_f64(v_and(v_reinterpret_as_u64(d1), m1));
                acc0 = v_fma(d0, d0, acc0);
                acc1 = v_fma(d1, d1, acc1);
            }
            result = v_reduce_sum(v_add(acc0, acc1));
            ಪ್ರತಿ (; i < len; i++) ಯದಿ (mask[i]) { ದ್ವಿಗುಣ v = s1[i] - s2[i]; result += v*v; }
        } ಅನ್ಯಥಾ {
            result = maskedNormDiffL2Tail<ದ್ವಿಗುಣ, ದ್ವಿಗುಣ>(s1, s2, mask, len, cn, 0.0);
        }
        ಪ್ರತ್ಯಾಗಮ result;
    }
};
#endif // CV_SIMD_64F
#endif // CV_SIMD

template<typename T, typename ST> ಪೂರ್ಣಾಙ್ಕ
normDiffInf_(ಧ್ರುವ T* src1, ಧ್ರುವ T* src2, ಧ್ರುವ uchar* mask, ST* _result, ಪೂರ್ಣಾಙ್ಕ len, ಪೂರ್ಣಾಙ್ಕ cn) {
    ST result = *_result;
    ಯದಿ( !mask ) {
        NormDiffInf_SIMD<T, ST> op;
        result = std::max(result, op(src1, src2, len*cn));
    } ಅನ್ಯಥಾ {
        MaskedNormDiffInf_SIMD<T, ST> op;
        result = std::max(result, op(src1, src2, mask, len, cn));
    }
    *_result = result;
    ಪ್ರತ್ಯಾಗಮ 0;
}

template<typename T, typename ST> ಪೂರ್ಣಾಙ್ಕ
normDiffL1_(ಧ್ರುವ T* src1, ಧ್ರುವ T* src2, ಧ್ರುವ uchar* mask, ST* _result, ಪೂರ್ಣಾಙ್ಕ len, ಪೂರ್ಣಾಙ್ಕ cn) {
    ST result = *_result;
    ಯದಿ( !mask ) {
        NormDiffL1_SIMD<T, ST> op;
        result += op(src1, src2, len*cn);
    } ಅನ್ಯಥಾ {
        MaskedNormDiffL1_SIMD<T, ST> op;
        result += op(src1, src2, mask, len, cn);
    }
    *_result = result;
    ಪ್ರತ್ಯಾಗಮ 0;
}

template<typename T, typename ST> ಪೂರ್ಣಾಙ್ಕ
normDiffL2_(ಧ್ರುವ T* src1, ಧ್ರುವ T* src2, ಧ್ರುವ uchar* mask, ST* _result, ಪೂರ್ಣಾಙ್ಕ len, ಪೂರ್ಣಾಙ್ಕ cn) {
    ST result = *_result;
    ಯದಿ( !mask ) {
        NormDiffL2_SIMD<T, ST> op;
        result += op(src1, src2, len*cn);
    } ಅನ್ಯಥಾ {
        MaskedNormDiffL2_SIMD<T, ST> op;
        result += op(src1, src2, mask, len, cn);
    }
    *_result = result;
    ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
normDiffInf_Bool(ಧ್ರುವ uchar* src1, ಧ್ರುವ uchar* src2, ಧ್ರುವ uchar* mask, ಪೂರ್ಣಾಙ್ಕ* _result, ಪೂರ್ಣಾಙ್ಕ len, ಪೂರ್ಣಾಙ್ಕ cn)
{
    ಪೂರ್ಣಾಙ್ಕ result = *_result;
    ಯದಿ( !mask )
    {
        ಪ್ರತಿ( ಪೂರ್ಣಾಙ್ಕ i = 0; i < len*cn; i++ ) {
            result = std::max(result, (ಪೂರ್ಣಾಙ್ಕ)((src1[i] != 0) != (src2[i] != 0)));
            ಯದಿ (result != 0)
                ಭಙ್ಗ;
        }
    }
    ಅನ್ಯಥಾ
    {
        ಪ್ರತಿ( ಪೂರ್ಣಾಙ್ಕ i = 0; i < len; i++, src1 += cn, src2 += cn )
            ಯದಿ( mask[i] )
            {
                ಪ್ರತಿ( ಪೂರ್ಣಾಙ್ಕ k = 0; k < cn; k++ )
                    result = std::max(result, (ಪೂರ್ಣಾಙ್ಕ)((src1[k] != 0) != (src2[k] != 0)));
                ಯದಿ (result != 0)
                    ಭಙ್ಗ;
            }
    }
    *_result = result;
    ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
normDiffL1_Bool(ಧ್ರುವ uchar* src1, ಧ್ರುವ uchar* src2, ಧ್ರುವ uchar* mask, ಪೂರ್ಣಾಙ್ಕ* _result, ಪೂರ್ಣಾಙ್ಕ len, ಪೂರ್ಣಾಙ್ಕ cn)
{
    ಪೂರ್ಣಾಙ್ಕ result = *_result;
    ಯದಿ( !mask )
    {
        ಪ್ರತಿ( ಪೂರ್ಣಾಙ್ಕ i = 0; i < len*cn; i++ )
            result += (ಪೂರ್ಣಾಙ್ಕ)((src1[i] != 0) != (src2[i] != 0));
    }
    ಅನ್ಯಥಾ
    {
        ಪ್ರತಿ( ಪೂರ್ಣಾಙ್ಕ i = 0; i < len; i++, src1 += cn, src2 += cn )
            ಯದಿ( mask[i] )
            {
                ಪ್ರತಿ( ಪೂರ್ಣಾಙ್ಕ k = 0; k < cn; k++ )
                    result += (ಪೂರ್ಣಾಙ್ಕ)((src1[k] != 0) != (src2[k] != 0));
            }
    }
    *_result = result;
    ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
normDiffL2_Bool(ಧ್ರುವ uchar* src1, ಧ್ರುವ uchar* src2, ಧ್ರುವ uchar* mask, ಪೂರ್ಣಾಙ್ಕ* _result, ಪೂರ್ಣಾಙ್ಕ len, ಪೂರ್ಣಾಙ್ಕ cn)
{
    ಪ್ರತ್ಯಾಗಮ normDiffL1_Bool(src1, src2, mask, _result, len, cn);
}

#define CV_DEF_NORM_FUNC(L, suffix, type, ntype) \
    ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ norm##L##_##suffix(ಧ್ರುವ type* src, ಧ್ರುವ uchar* mask, ntype* r, ಪೂರ್ಣಾಙ್ಕ len, ಪೂರ್ಣಾಙ್ಕ cn) \
{ CV_INSTRUMENT_REGION(); ಪ್ರತ್ಯಾಗಮ norm##L##_(src, mask, r, len, cn); } \
    ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ normDiff##L##_##suffix(ಧ್ರುವ type* src1, ಧ್ರುವ type* src2, \
    ಧ್ರುವ uchar* mask, ntype* r, ಪೂರ್ಣಾಙ್ಕ len, ಪೂರ್ಣಾಙ್ಕ cn) \
{ ಪ್ರತ್ಯಾಗಮ normDiff##L##_(src1, src2, mask, r, (ಪೂರ್ಣಾಙ್ಕ)len, cn); }

#define CV_DEF_NORM_ALL(suffix, type, inftype, l1type, l2type) \
    CV_DEF_NORM_FUNC(Inf, suffix, type, inftype) \
    CV_DEF_NORM_FUNC(L1, suffix, type, l1type) \
    CV_DEF_NORM_FUNC(L2, suffix, type, l2type)

CV_DEF_NORM_ALL(8u, uchar, ಪೂರ್ಣಾಙ್ಕ, ಪೂರ್ಣಾಙ್ಕ, ಪೂರ್ಣಾಙ್ಕ)
CV_DEF_NORM_ALL(8s, schar, ಪೂರ್ಣಾಙ್ಕ, ಪೂರ್ಣಾಙ್ಕ, ಪೂರ್ಣಾಙ್ಕ)
CV_DEF_NORM_ALL(16u, ushort, ಪೂರ್ಣಾಙ್ಕ, ಪೂರ್ಣಾಙ್ಕ, ದ್ವಿಗುಣ)
CV_DEF_NORM_ALL(16s, short, ಪೂರ್ಣಾಙ್ಕ, ಪೂರ್ಣಾಙ್ಕ, ದ್ವಿಗುಣ)
CV_DEF_NORM_ALL(32u, unsigned, unsigned, ದ್ವಿಗುಣ, ದ್ವಿಗುಣ)
CV_DEF_NORM_ALL(32s, ಪೂರ್ಣಾಙ್ಕ, unsigned, ದ್ವಿಗುಣ, ದ್ವಿಗುಣ)
CV_DEF_NORM_ALL(32f, ಭಿನ್ನ, ಭಿನ್ನ, ದ್ವಿಗುಣ, ದ್ವಿಗುಣ)
CV_DEF_NORM_ALL(64f, ದ್ವಿಗುಣ, ದ್ವಿಗುಣ, ದ್ವಿಗುಣ, ದ್ವಿಗುಣ)
CV_DEF_NORM_ALL(64u, uint64, uint64, ದ್ವಿಗುಣ, ದ್ವಿಗುಣ)
CV_DEF_NORM_ALL(64s, int64, uint64, ದ್ವಿಗುಣ, ದ್ವಿಗುಣ)
CV_DEF_NORM_ALL(16f, hfloat, ಭಿನ್ನ, ಭಿನ್ನ, ಭಿನ್ನ)
CV_DEF_NORM_ALL(16bf, bfloat, ಭಿನ್ನ, ಭಿನ್ನ, ಭಿನ್ನ)

NormFunc getNormFunc(ಪೂರ್ಣಾಙ್ಕ normType, ಪೂರ್ಣಾಙ್ಕ depth)
{
    CV_INSTRUMENT_REGION();

    ಸ್ಥಿರ NormFunc normTab[3][CV_DEPTH_MAX] =
    {
        {
            normWrap<uchar, ಪೂರ್ಣಾಙ್ಕ, normInf_8u>,
            normWrap<schar, ಪೂರ್ಣಾಙ್ಕ, normInf_8s>,
            normWrap<ushort, ಪೂರ್ಣಾಙ್ಕ, normInf_16u>,
            normWrap<short, ಪೂರ್ಣಾಙ್ಕ, normInf_16s>,
            normWrap<ಪೂರ್ಣಾಙ್ಕ, unsigned, normInf_32s>,
            normWrap<ಭಿನ್ನ, ಭಿನ್ನ, normInf_32f>,
            normWrap<ದ್ವಿಗುಣ, ದ್ವಿಗುಣ, normInf_64f>,
            normWrap<hfloat, ಭಿನ್ನ, normInf_16f>,
            normWrap<bfloat, ಭಿನ್ನ, normInf_16bf>,
            normWrap<uchar, ಪೂರ್ಣಾಙ್ಕ, normInf_Bool>,
            normWrap<uint64, uint64, normInf_64u>,
            normWrap<int64, uint64, normInf_64s>,
            normWrap<unsigned, unsigned, normInf_32u>,
            0
        },
        {
            normWrap<uchar, ಪೂರ್ಣಾಙ್ಕ, normL1_8u>,
            normWrap<schar, ಪೂರ್ಣಾಙ್ಕ, normL1_8s>,
            normWrap<ushort, ಪೂರ್ಣಾಙ್ಕ, normL1_16u>,
            normWrap<short, ಪೂರ್ಣಾಙ್ಕ, normL1_16s>,
            normWrap<ಪೂರ್ಣಾಙ್ಕ, ದ್ವಿಗುಣ, normL1_32s>,
            normWrap<ಭಿನ್ನ, ದ್ವಿಗುಣ, normL1_32f>,
            normWrap<ದ್ವಿಗುಣ, ದ್ವಿಗುಣ, normL1_64f>,
            normWrap<hfloat, ಭಿನ್ನ, normL1_16f>,
            normWrap<bfloat, ಭಿನ್ನ, normL1_16bf>,
            normWrap<uchar, ಪೂರ್ಣಾಙ್ಕ, normL1_Bool>,
            normWrap<uint64, ದ್ವಿಗುಣ, normL1_64u>,
            normWrap<int64, ದ್ವಿಗುಣ, normL1_64s>,
            normWrap<unsigned, ದ್ವಿಗುಣ, normL1_32u>,
            0
        },
        {
            normWrap<uchar, ಪೂರ್ಣಾಙ್ಕ, normL2_8u>,
            normWrap<schar, ಪೂರ್ಣಾಙ್ಕ, normL2_8s>,
            normWrap<ushort, ದ್ವಿಗುಣ, normL2_16u>,
            normWrap<short, ದ್ವಿಗುಣ, normL2_16s>,
            normWrap<ಪೂರ್ಣಾಙ್ಕ, ದ್ವಿಗುಣ, normL2_32s>,
            normWrap<ಭಿನ್ನ, ದ್ವಿಗುಣ, normL2_32f>,
            normWrap<ದ್ವಿಗುಣ, ದ್ವಿಗುಣ, normL2_64f>,
            normWrap<hfloat, ಭಿನ್ನ, normL2_16f>,
            normWrap<bfloat, ಭಿನ್ನ, normL2_16bf>,
            normWrap<uchar, ಪೂರ್ಣಾಙ್ಕ, normL2_Bool>,
            normWrap<uint64, ದ್ವಿಗುಣ, normL2_64u>,
            normWrap<int64, ದ್ವಿಗುಣ, normL2_64s>,
            normWrap<unsigned, ದ್ವಿಗುಣ, normL2_32u>,
            0
        }
    };

    ಯದಿ (normType >= 3 || normType < 0) ಪ್ರತ್ಯಾಗಮ nullptr;

    ಪ್ರತ್ಯಾಗಮ normTab[normType][depth];
}

NormDiffFunc getNormDiffFunc(ಪೂರ್ಣಾಙ್ಕ normType, ಪೂರ್ಣಾಙ್ಕ depth)
{
    ಸ್ಥಿರ NormDiffFunc normDiffTab[3][CV_DEPTH_MAX] =
    {
        {
            normDiffWrap<uchar, ಪೂರ್ಣಾಙ್ಕ, normDiffInf_8u>,
            normDiffWrap<schar, ಪೂರ್ಣಾಙ್ಕ, normDiffInf_8s>,
            normDiffWrap<ushort, ಪೂರ್ಣಾಙ್ಕ, normDiffInf_16u>,
            normDiffWrap<short, ಪೂರ್ಣಾಙ್ಕ, normDiffInf_16s>,
            normDiffWrap<ಪೂರ್ಣಾಙ್ಕ, unsigned, normDiffInf_32s>,
            normDiffWrap<ಭಿನ್ನ, ಭಿನ್ನ, normDiffInf_32f>,
            normDiffWrap<ದ್ವಿಗುಣ, ದ್ವಿಗುಣ, normDiffInf_64f>,
            normDiffWrap<hfloat, ಭಿನ್ನ, normDiffInf_16f>,
            normDiffWrap<bfloat, ಭಿನ್ನ, normDiffInf_16bf>,
            normDiffWrap<uchar, ಪೂರ್ಣಾಙ್ಕ, normDiffInf_Bool>,
            normDiffWrap<uint64, uint64, normDiffInf_64u>,
            normDiffWrap<int64, uint64, normDiffInf_64s>,
            normDiffWrap<unsigned, unsigned, normDiffInf_32u>,
            0
        },
        {
            normDiffWrap<uchar, ಪೂರ್ಣಾಙ್ಕ, normDiffL1_8u>,
            normDiffWrap<schar, ಪೂರ್ಣಾಙ್ಕ, normDiffL1_8s>,
            normDiffWrap<ushort, ಪೂರ್ಣಾಙ್ಕ, normDiffL1_16u>,
            normDiffWrap<short, ಪೂರ್ಣಾಙ್ಕ, normDiffL1_16s>,
            normDiffWrap<ಪೂರ್ಣಾಙ್ಕ, ದ್ವಿಗುಣ, normDiffL1_32s>,
            normDiffWrap<ಭಿನ್ನ, ದ್ವಿಗುಣ, normDiffL1_32f>,
            normDiffWrap<ದ್ವಿಗುಣ, ದ್ವಿಗುಣ, normDiffL1_64f>,
            normDiffWrap<hfloat, ಭಿನ್ನ, normDiffL1_16f>,
            normDiffWrap<bfloat, ಭಿನ್ನ, normDiffL1_16bf>,
            normDiffWrap<uchar, ಪೂರ್ಣಾಙ್ಕ, normDiffL1_Bool>,
            normDiffWrap<uint64, ದ್ವಿಗುಣ, normDiffL1_64u>,
            normDiffWrap<int64, ದ್ವಿಗುಣ, normDiffL1_64s>,
            normDiffWrap<unsigned, ದ್ವಿಗುಣ, normDiffL1_32u>,
            0
        },
        {
            normDiffWrap<uchar, ಪೂರ್ಣಾಙ್ಕ, normDiffL2_8u>,
            normDiffWrap<schar, ಪೂರ್ಣಾಙ್ಕ, normDiffL2_8s>,
            normDiffWrap<ushort, ದ್ವಿಗುಣ, normDiffL2_16u>,
            normDiffWrap<short, ದ್ವಿಗುಣ, normDiffL2_16s>,
            normDiffWrap<ಪೂರ್ಣಾಙ್ಕ, ದ್ವಿಗುಣ, normDiffL2_32s>,
            normDiffWrap<ಭಿನ್ನ, ದ್ವಿಗುಣ, normDiffL2_32f>,
            normDiffWrap<ದ್ವಿಗುಣ, ದ್ವಿಗುಣ, normDiffL2_64f>,
            normDiffWrap<hfloat, ಭಿನ್ನ, normDiffL2_16f>,
            normDiffWrap<bfloat, ಭಿನ್ನ, normDiffL2_16bf>,
            normDiffWrap<uchar, ಪೂರ್ಣಾಙ್ಕ, normDiffL2_Bool>,
            normDiffWrap<uint64, ದ್ವಿಗುಣ, normDiffL2_64u>,
            normDiffWrap<int64, ದ್ವಿಗುಣ, normDiffL2_64s>,
            normDiffWrap<unsigned, ದ್ವಿಗುಣ, normDiffL2_32u>,
            0
        },
    };
    ಯದಿ (normType >= 3 || normType < 0) ಪ್ರತ್ಯಾಗಮ nullptr;

    ಪ್ರತ್ಯಾಗಮ normDiffTab[normType][depth];
}

#endif // CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

CV_CPU_OPTIMIZATION_NAMESPACE_END

} // cv::
