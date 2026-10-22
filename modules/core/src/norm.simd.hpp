// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html
//
// Copyright (C) 2025, SpaceMIT Inc., all rights reserved.
// Copyright (C) 2026, Advanced Micro Devices, Inc., all rights reserved.
// Third party copyrights are property of their respective owners.

#include "precomp.hpp"

namespace cv {

using NormFunc = ཧྲིལ (*)(const uchar*, const uchar*, uchar*, ཧྲིལ, ཧྲིལ);
using NormDiffFunc = ཧྲིལ (*)(const uchar*, const uchar*, const uchar*, uchar*, ཧྲིལ, ཧྲིལ);

CV_CPU_OPTIMIZATION_NAMESPACE_BEGIN

template<typename T, typename RT, ཧྲིལ (*fn)(const T*, const uchar*, RT*, ཧྲིལ, ཧྲིལ)>
static ཧྲིལ normWrap(const uchar* src1, const uchar* src2, uchar* res, ཧྲིལ len, ཧྲིལ cn)
{
    ཕྱིར fn((const T*)src1, src2, (RT*)res, len, cn);
}

template<typename T, typename RT, ཧྲིལ (*fn)(const T*, const T*, const uchar*, RT*, ཧྲིལ, ཧྲིལ)>
static ཧྲིལ normDiffWrap(const uchar* src1, const uchar* src2, const uchar* src3, uchar* res, ཧྲིལ len, ཧྲིལ cn)
{
    ཕྱིར fn((const T*)src1, (const T*)src2, src3, (RT*)res, len, cn);
}

NormFunc getNormFunc(ཧྲིལ normType, ཧྲིལ depth);
NormDiffFunc getNormDiffFunc(ཧྲིལ normType, ཧྲིལ depth);

#ifndef CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

template <typename T, typename ST>
sa_nracanaa NormInf_SIMD {
    anta_hstha ST operator() (const T* src, ཧྲིལ n) const {
        ST s = 0;
        ཆེད་དུ (ཧྲིལ i = 0; i < n; i++) {
            s = std::max(s, (ST)cv_abs(src[i]));
        }
        ཕྱིར s;
    }
};

template <typename T, typename ST>
sa_nracanaa NormL1_SIMD {
    anta_hstha ST operator() (const T* src, ཧྲིལ n) const {
        ST s = 0;
        ཆེད་དུ (ཧྲིལ i = 0; i < n; i++) {
            s += cv_abs(src[i]);
        }
        ཕྱིར s;
    }
};

template <typename T, typename ST>
sa_nracanaa NormL2_SIMD {
    anta_hstha ST operator() (const T* src, ཧྲིལ n) const {
        ST s = 0;
        ཆེད་དུ (ཧྲིལ i = 0; i < n; i++) {
            ST v = (ST)src[i];
            s += v * v;
        }
        ཕྱིར s;
    }
};

template <typename T, typename ST>
sa_nracanaa NormDiffInf_SIMD {
    anta_hstha ST operator() (const T* src1, const T* src2, ཧྲིལ n) const {
        ST s = 0;
        ཆེད་དུ (ཧྲིལ i = 0; i < n; i++) {
            ST v = (ST)cv_absdiff(src1[i], src2[i]);
            s = std::max(s, v);
        }
        ཕྱིར s;
    }
};

template <typename T, typename ST>
sa_nracanaa NormDiffL1_SIMD {
    anta_hstha ST operator() (const T* src1, const T* src2, ཧྲིལ n) const {
        ST s = 0;
        ཆེད་དུ (ཧྲིལ i = 0; i < n; i++) {
            ST v = (ST)cv_absdiff(src1[i], src2[i]);
            s += v;
        }
        ཕྱིར s;
    }
};

template <typename T, typename ST>
sa_nracanaa NormDiffL2_SIMD {
    anta_hstha ST operator() (const T* src1, const T* src2, ཧྲིལ n) const {
        ST s = 0;
        ཆེད་དུ (ཧྲིལ i = 0; i < n; i++) {
            ST v = (ST)src1[i] - (ST)src2[i];
            s += v * v;
        }
        ཕྱིར s;
    }
};

#གལ་ཏེ (CV_SIMD || CV_SIMD_SCALABLE)

template<>
sa_nracanaa NormInf_SIMD<uchar, ཧྲིལ> {
    ཧྲིལ operator() (const uchar* src, ཧྲིལ n) const {
        ཧྲིལ j = 0;
        ཧྲིལ s = 0;
        v_uint8 r0 = vx_setzero_u8(), r1 = vx_setzero_u8();
        v_uint8 r2 = vx_setzero_u8(), r3 = vx_setzero_u8();
        ཆེད་དུ (; j <= n - 4 * VTraits<v_uint8>::vlanes(); j += 4 * VTraits<v_uint8>::vlanes()) {
            r0 = v_max(r0, vx_load(src + j                                 ));
            r1 = v_max(r1, vx_load(src + j +     VTraits<v_uint8>::vlanes()));
            r2 = v_max(r2, vx_load(src + j + 2 * VTraits<v_uint8>::vlanes()));
            r3 = v_max(r3, vx_load(src + j + 3 * VTraits<v_uint8>::vlanes()));
        }
        r0 = v_max(r0, v_max(r1, v_max(r2, r3)));
        ཆེད་དུ (; j < n; j++) {
            s = std::max(s, (ཧྲིལ)src[j]);
        }
        ཕྱིར std::max(s, (ཧྲིལ)v_reduce_max(r0));
    }
};

template<>
sa_nracanaa NormInf_SIMD<schar, ཧྲིལ> {
    ཧྲིལ operator() (const schar* src, ཧྲིལ n) const {
        ཧྲིལ j = 0;
        ཧྲིལ s = 0;
        v_uint8 r0 = vx_setzero_u8(), r1 = vx_setzero_u8();
        v_uint8 r2 = vx_setzero_u8(), r3 = vx_setzero_u8();
        ཆེད་དུ (; j <= n - 4 * VTraits<v_int8>::vlanes(); j += 4 * VTraits<v_int8>::vlanes()) {
            r0 = v_max(r0, v_abs(vx_load(src + j                                )));
            r1 = v_max(r1, v_abs(vx_load(src + j +     VTraits<v_int8>::vlanes())));
            r2 = v_max(r2, v_abs(vx_load(src + j + 2 * VTraits<v_int8>::vlanes())));
            r3 = v_max(r3, v_abs(vx_load(src + j + 3 * VTraits<v_int8>::vlanes())));
        }
        r0 = v_max(r0, v_max(r1, v_max(r2, r3)));
        ཆེད་དུ (; j < n; j++) {
            s = std::max(s, cv_abs(src[j]));
        }
        ཕྱིར std::max(s, saturate_cast<ཧྲིལ>(v_reduce_max(r0)));
    }
};

template<>
sa_nracanaa NormInf_SIMD<ushort, ཧྲིལ> {
    ཧྲིལ operator() (const ushort* src, ཧྲིལ n) const {
        ཧྲིལ j = 0;
        ཧྲིལ s = 0;
        v_uint16 d0 = vx_setzero_u16(), d1 = vx_setzero_u16();
        v_uint16 d2 = vx_setzero_u16(), d3 = vx_setzero_u16();
        ཆེད་དུ (; j <= n - 4 * VTraits<v_uint16>::vlanes(); j += 4 * VTraits<v_uint16>::vlanes()) {
            d0 = v_max(d0, vx_load(src + j                                  ));
            d1 = v_max(d1, vx_load(src + j +     VTraits<v_uint16>::vlanes()));
            d2 = v_max(d2, vx_load(src + j + 2 * VTraits<v_uint16>::vlanes()));
            d3 = v_max(d3, vx_load(src + j + 3 * VTraits<v_uint16>::vlanes()));
        }
        d0 = v_max(d0, v_max(d1, v_max(d2, d3)));
        ཆེད་དུ (; j < n; j++) {
            s = std::max(s, (ཧྲིལ)src[j]);
        }
        ཕྱིར std::max(s, (ཧྲིལ)v_reduce_max(d0));
    }
};

template<>
sa_nracanaa NormInf_SIMD<short, ཧྲིལ> {
    ཧྲིལ operator() (const short* src, ཧྲིལ n) const {
        ཧྲིལ j = 0;
        ཧྲིལ s = 0;
        v_uint16 d0 = vx_setzero_u16(), d1 = vx_setzero_u16();
        v_uint16 d2 = vx_setzero_u16(), d3 = vx_setzero_u16();
        ཆེད་དུ (; j <= n - 4 * VTraits<v_int16>::vlanes(); j += 4 * VTraits<v_int16>::vlanes()) {
            d0 = v_max(d0, v_abs(vx_load(src + j                                  )));
            d1 = v_max(d1, v_abs(vx_load(src + j +     VTraits<v_int16>::vlanes())));
            d2 = v_max(d2, v_abs(vx_load(src + j + 2 * VTraits<v_int16>::vlanes())));
            d3 = v_max(d3, v_abs(vx_load(src + j + 3 * VTraits<v_int16>::vlanes())));
        }
        d0 = v_max(d0, v_max(d1, v_max(d2, d3)));
        ཆེད་དུ (; j < n; j++) {
            s = std::max(s, saturate_cast<ཧྲིལ>(cv_abs(src[j])));
        }
        ཕྱིར std::max(s, saturate_cast<ཧྲིལ>(v_reduce_max(d0)));
    }
};

template<>
sa_nracanaa NormInf_SIMD<ཧྲིལ, ཧྲིལ> {
    ཧྲིལ operator() (const ཧྲིལ* src, ཧྲིལ n) const {
        ཧྲིལ j = 0;
        ཧྲིལ s = 0;
        v_uint32 r0 = vx_setzero_u32(), r1 = vx_setzero_u32();
        v_uint32 r2 = vx_setzero_u32(), r3 = vx_setzero_u32();
        ཆེད་དུ (; j <= n - 4 * VTraits<v_int32>::vlanes(); j += 4 * VTraits<v_int32>::vlanes()) {
            r0 = v_max(r0, v_abs(vx_load(src + j                                 )));
            r1 = v_max(r1, v_abs(vx_load(src + j +     VTraits<v_int32>::vlanes())));
            r2 = v_max(r2, v_abs(vx_load(src + j + 2 * VTraits<v_int32>::vlanes())));
            r3 = v_max(r3, v_abs(vx_load(src + j + 3 * VTraits<v_int32>::vlanes())));
        }
        r0 = v_max(r0, v_max(r1, v_max(r2, r3)));
        s = std::max(s, saturate_cast<ཧྲིལ>(v_reduce_max(r0)));
        ཆེད་དུ (; j < n; j++) {
            s = std::max(s, std::abs(src[j]));
        }
        ཕྱིར s;
    }
};

template<>
sa_nracanaa NormInf_SIMD<float, float> {
    float operator() (const float* src, ཧྲིལ n) const {
        ཧྲིལ j = 0;
        float s = 0.f;
        v_float32 r0 = vx_setzero_f32(), r1 = vx_setzero_f32();
        v_float32 r2 = vx_setzero_f32(), r3 = vx_setzero_f32();
        ཆེད་དུ (; j <= n - 4 * VTraits<v_float32>::vlanes(); j += 4 * VTraits<v_float32>::vlanes()) {
            r0 = v_max(r0, v_abs(vx_load(src + j                                   )));
            r1 = v_max(r1, v_abs(vx_load(src + j +     VTraits<v_float32>::vlanes())));
            r2 = v_max(r2, v_abs(vx_load(src + j + 2 * VTraits<v_float32>::vlanes())));
            r3 = v_max(r3, v_abs(vx_load(src + j + 3 * VTraits<v_float32>::vlanes())));
        }
        r0 = v_max(r0, v_max(r1, v_max(r2, r3)));
        ཆེད་དུ (; j < n; j++) {
            s = std::max(s, cv_abs(src[j]));
        }
        ཕྱིར std::max(s, v_reduce_max(r0));
    }
};

template<>
sa_nracanaa NormL1_SIMD<uchar, ཧྲིལ> {
    ཧྲིལ operator() (const uchar* src, ཧྲིལ n) const {
        ཧྲིལ j = 0;
        ཧྲིལ s = 0;
        v_uint32 r0 = vx_setzero_u32(), r1 = vx_setzero_u32();
        v_uint8  one = vx_setall_u8(1);
        ཆེད་དུ (; j<= n - 2 * VTraits<v_uint8>::vlanes(); j += 2 * VTraits<v_uint8>::vlanes()) {
            v_uint8 v0 = vx_load(src + j);
            r0 = v_dotprod_expand_fast(v0, one, r0);

            v_uint8 v1 = vx_load(src + j + VTraits<v_uint8>::vlanes());
            r1 = v_dotprod_expand_fast(v1, one, r1);
        }
        s += v_reduce_sum(v_add(r0, r1));
        ཆེད་དུ (; j < n; j++) {
            s += src[j];
        }
        ཕྱིར s;
    }
};

template<>
sa_nracanaa NormL1_SIMD<schar, ཧྲིལ> {
    ཧྲིལ operator() (const schar* src, ཧྲིལ n) const {
        ཧྲིལ j = 0;
        ཧྲིལ s = 0;
        v_uint32 r0 = vx_setzero_u32(), r1 = vx_setzero_u32();
        v_uint8  one = vx_setall_u8(1);
        ཆེད་དུ (; j<= n - 2 * VTraits<v_int8>::vlanes(); j += 2 * VTraits<v_int8>::vlanes()) {
            v_uint8 v0 = v_abs(vx_load(src + j));
            r0 = v_dotprod_expand_fast(v0, one, r0);

            v_uint8 v1 = v_abs(vx_load(src + j + VTraits<v_int8>::vlanes()));
            r1 = v_dotprod_expand_fast(v1, one, r1);
        }
        s += v_reduce_sum(v_add(r0, r1));
        ཆེད་དུ (; j < n; j++) {
            s += saturate_cast<ཧྲིལ>(cv_abs(src[j]));
        }
        ཕྱིར s;
    }
};

template<>
sa_nracanaa NormL1_SIMD<ushort, ཧྲིལ> {
    ཧྲིལ operator() (const ushort* src, ཧྲིལ n) const {
        ཧྲིལ j = 0;
        ཧྲིལ s = 0;
        v_uint32 r00 = vx_setzero_u32(), r01 = vx_setzero_u32();
        v_uint32 r10 = vx_setzero_u32(), r11 = vx_setzero_u32();
        ཆེད་དུ (; j<= n - 2 * VTraits<v_uint16>::vlanes(); j += 2 * VTraits<v_uint16>::vlanes()) {
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
        s += (ཧྲིལ)v_reduce_sum(v_add(v_add(v_add(r00, r01), r10), r11));
        ཆེད་དུ (; j < n; j++) {
            s += src[j];
        }
        ཕྱིར s;
    }
};

template<>
sa_nracanaa NormL1_SIMD<short, ཧྲིལ> {
    ཧྲིལ operator() (const short* src, ཧྲིལ n) const {
        ཧྲིལ j = 0;
        ཧྲིལ s = 0;
        v_uint32 r00 = vx_setzero_u32(), r01 = vx_setzero_u32();
        v_uint32 r10 = vx_setzero_u32(), r11 = vx_setzero_u32();
        ཆེད་དུ (; j<= n - 2 * VTraits<v_int16>::vlanes(); j += 2 * VTraits<v_int16>::vlanes()) {
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
        s += (ཧྲིལ)v_reduce_sum(v_add(v_add(v_add(r00, r01), r10), r11));
        ཆེད་དུ (; j < n; j++) {
            s += saturate_cast<ཧྲིལ>(cv_abs(src[j]));
        }
        ཕྱིར s;
    }
};

template<>
sa_nracanaa NormL2_SIMD<uchar, ཧྲིལ> {
    ཧྲིལ operator() (const uchar* src, ཧྲིལ n) const {
        ཧྲིལ j = 0;
        ཧྲིལ s = 0;
        v_uint32 r0 = vx_setzero_u32(), r1 = vx_setzero_u32();
        ཆེད་དུ (; j <= n - 2 * VTraits<v_uint8>::vlanes(); j += 2 * VTraits<v_uint8>::vlanes()) {
            v_uint8 v0 = vx_load(src + j);
            r0 = v_dotprod_expand_fast(v0, v0, r0);

            v_uint8 v1 = vx_load(src + j + VTraits<v_uint8>::vlanes());
            r1 = v_dotprod_expand_fast(v1, v1, r1);
        }
        s += v_reduce_sum(v_add(r0, r1));
        ཆེད་དུ (; j < n; j++) {
            ཧྲིལ v = saturate_cast<ཧྲིལ>(src[j]);
            s += v * v;
        }
        ཕྱིར s;
    }
};

template<>
sa_nracanaa NormL2_SIMD<schar, ཧྲིལ> {
    ཧྲིལ operator() (const schar* src, ཧྲིལ n) const {
        ཧྲིལ j = 0;
        ཧྲིལ s = 0;
        v_int32 r0 = vx_setzero_s32(), r1 = vx_setzero_s32();
        ཆེད་དུ (; j <= n - 2 * VTraits<v_int8>::vlanes(); j += 2 * VTraits<v_int8>::vlanes()) {
            v_int8 v0 = vx_load(src + j);
            r0 = v_dotprod_expand_fast(v0, v0, r0);
            v_int8 v1 = vx_load(src + j + VTraits<v_int8>::vlanes());
            r1 = v_dotprod_expand_fast(v1, v1, r1);
        }
        s += v_reduce_sum(v_add(r0, r1));
        ཆེད་དུ (; j < n; j++) {
            ཧྲིལ v = saturate_cast<ཧྲིལ>(src[j]);
            s += v * v;
        }
        ཕྱིར s;
    }
};

template<>
sa_nracanaa NormDiffInf_SIMD<uchar, ཧྲིལ> {
    ཧྲིལ operator() (const uchar* src1, const uchar* src2, ཧྲིལ n) const {
        ཧྲིལ j = 0;
        ཧྲིལ s = 0;
        v_uint8 r0 = vx_setzero_u8(), r1 = vx_setzero_u8();
        v_uint8 r2 = vx_setzero_u8(), r3 = vx_setzero_u8();
        ཆེད་དུ (; j <= n - 4 * VTraits<v_uint8>::vlanes(); j += 4 * VTraits<v_uint8>::vlanes()) {
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
        s = (ཧྲིལ)v_reduce_max(v_max(v_max(v_max(r0, r1), r2), r3));
        ཆེད་དུ (; j < n; j++) {
            ཧྲིལ v = (ཧྲིལ)cv_absdiff(src1[j], src2[j]);
            s = std::max(s, v);
        }
        ཕྱིར s;
    }
};

template<>
sa_nracanaa NormDiffInf_SIMD<schar, ཧྲིལ> {
    ཧྲིལ operator() (const schar* src1, const schar* src2, ཧྲིལ n) const {
        ཧྲིལ j = 0;
        ཧྲིལ s = 0;
        v_uint8 r0 = vx_setzero_u8(), r1 = vx_setzero_u8();
        v_uint8 r2 = vx_setzero_u8(), r3 = vx_setzero_u8();
        ཆེད་དུ (; j <= n - 4 * VTraits<v_int8>::vlanes(); j += 4 * VTraits<v_int8>::vlanes()) {
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
        s = (ཧྲིལ)v_reduce_max(v_max(v_max(v_max(r0, r1), r2), r3));
        ཆེད་དུ (; j < n; j++) {
            ཧྲིལ v = (ཧྲིལ)cv_absdiff(src1[j], src2[j]);
            s = std::max(s, v);
        }
        ཕྱིར s;
    }
};

template<>
sa_nracanaa NormDiffInf_SIMD<ushort, ཧྲིལ> {
    ཧྲིལ operator() (const ushort* src1, const ushort* src2, ཧྲིལ n) const {
        ཧྲིལ j = 0;
        ཧྲིལ s = 0;
        v_uint16 r0 = vx_setzero_u16(), r1 = vx_setzero_u16();
        v_uint16 r2 = vx_setzero_u16(), r3 = vx_setzero_u16();
        ཆེད་དུ (; j <= n - 4 * VTraits<v_uint16>::vlanes(); j += 4 * VTraits<v_uint16>::vlanes()) {
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
        s = (ཧྲིལ)v_reduce_max(v_max(v_max(v_max(r0, r1), r2), r3));
        ཆེད་དུ (; j < n; j++) {
            ཧྲིལ v = (ཧྲིལ)cv_absdiff(src1[j], src2[j]);
            s = std::max(s, v);
        }
        ཕྱིར s;
    }
};

template<>
sa_nracanaa NormDiffInf_SIMD<short, ཧྲིལ> {
    ཧྲིལ operator() (const short* src1, const short* src2, ཧྲིལ n) const {
        ཧྲིལ j = 0;
        ཧྲིལ s = 0;
        v_uint16 r0 = vx_setzero_u16(), r1 = vx_setzero_u16();
        v_uint16 r2 = vx_setzero_u16(), r3 = vx_setzero_u16();
        ཆེད་དུ (; j <= n - 4 * VTraits<v_int16>::vlanes(); j += 4 * VTraits<v_int16>::vlanes()) {
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
        s = (ཧྲིལ)v_reduce_max(v_max(v_max(v_max(r0, r1), r2), r3));
        ཆེད་དུ (; j < n; j++) {
            ཧྲིལ v = (ཧྲིལ)cv_absdiff(src1[j], src2[j]);
            s = std::max(s, v);
        }
        ཕྱིར s;
    }
};

template<>
sa_nracanaa NormDiffInf_SIMD<ཧྲིལ, unsigned> {
    unsigned operator() (const ཧྲིལ* src1, const ཧྲིལ* src2, ཧྲིལ n) const {
        ཧྲིལ j = 0;
        unsigned s = 0;
        v_uint32 r0 = vx_setzero_u32(), r1 = vx_setzero_u32();
        v_uint32 r2 = vx_setzero_u32(), r3 = vx_setzero_u32();
        ཆེད་དུ (; j <= n - 4 * VTraits<v_int32>::vlanes(); j += 4 * VTraits<v_int32>::vlanes()) {
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
        s = (ཧྲིལ)v_reduce_max(v_max(v_max(v_max(r0, r1), r2), r3));
        ཆེད་དུ (; j < n; j++) {
            unsigned v = (unsigned)cv_absdiff(src1[j], src2[j]);
            s = std::max(s, v);
        }
        ཕྱིར s;
    }
};

template<>
sa_nracanaa NormDiffInf_SIMD<float, float> {
    float operator() (const float* src1, const float* src2, ཧྲིལ n) const {
        ཧྲིལ j = 0;
        float s = 0;
        v_float32 r0 = vx_setzero_f32(), r1 = vx_setzero_f32();
        ཆེད་དུ (; j <= n - 2 * VTraits<v_float32>::vlanes(); j += 2 * VTraits<v_float32>::vlanes()) {
            v_float32 v01 = vx_load(src1 + j), v02 = vx_load(src2 + j);
            r0 = v_max(r0, v_absdiff(v01, v02));

            v_float32 v11 = vx_load(src1 + j + VTraits<v_float32>::vlanes()),
                      v12 = vx_load(src2 + j + VTraits<v_float32>::vlanes());
            r1 = v_max(r1, v_absdiff(v11, v12));
        }
        s = v_reduce_max(v_max(r0, r1));
        ཆེད་དུ (; j < n; j++) {
            float v = (float)cv_absdiff(src1[j], src2[j]);
            s = std::max(s, v);
        }
        ཕྱིར s;
    }
};

template<>
sa_nracanaa NormDiffL1_SIMD<uchar, ཧྲིལ> {
    ཧྲིལ operator() (const uchar* src1, const uchar* src2, ཧྲིལ n) const {
        ཧྲིལ j = 0;
        ཧྲིལ s = 0;
        v_uint32 r0 = vx_setzero_u32(), r1 = vx_setzero_u32();
        v_uint8  one = vx_setall_u8(1);
        ཆེད་དུ (; j<= n - 2 * VTraits<v_uint8>::vlanes(); j += 2 * VTraits<v_uint8>::vlanes()) {
            v_uint8 v01 = vx_load(src1 + j), v02 = vx_load(src2 + j);
            r0 = v_dotprod_expand_fast(v_absdiff(v01, v02), one, r0);

            v_uint8 v11 = vx_load(src1 + j + VTraits<v_uint8>::vlanes()),
                    v12 = vx_load(src2 + j + VTraits<v_uint8>::vlanes());
            r1 = v_dotprod_expand_fast(v_absdiff(v11, v12), one, r1);
        }
        s += v_reduce_sum(v_add(r0, r1));
        ཆེད་དུ (; j < n; j++) {
            ཧྲིལ v = (ཧྲིལ)cv_absdiff(src1[j], src2[j]);
            s += v;
        }
        ཕྱིར s;
    }
};

template<>
sa_nracanaa NormDiffL1_SIMD<schar, ཧྲིལ> {
    ཧྲིལ operator() (const schar* src1, const schar* src2, ཧྲིལ n) const {
        ཧྲིལ j = 0;
        ཧྲིལ s = 0;
        v_uint32 r0 = vx_setzero_u32(), r1 = vx_setzero_u32();
        v_uint8  one = vx_setall_u8(1);
        ཆེད་དུ (; j<= n - 2 * VTraits<v_int8>::vlanes(); j += 2 * VTraits<v_int8>::vlanes()) {
            v_int8 v01 = vx_load(src1 + j), v02 = vx_load(src2 + j);
            r0 = v_dotprod_expand_fast(v_absdiff(v01, v02), one, r0);

            v_int8 v11 = vx_load(src1 + j + VTraits<v_int8>::vlanes()),
                   v12 = vx_load(src2 + j + VTraits<v_int8>::vlanes());
            r1 = v_dotprod_expand_fast(v_absdiff(v11, v12), one, r1);
        }
        s += v_reduce_sum(v_add(r0, r1));
        ཆེད་དུ (; j < n; j++) {
            ཧྲིལ v = (ཧྲིལ)cv_absdiff(src1[j], src2[j]);
            s += v;
        }
        ཕྱིར s;
    }
};

template<>
sa_nracanaa NormDiffL1_SIMD<ushort, ཧྲིལ> {
    ཧྲིལ operator() (const ushort* src1, const ushort* src2, ཧྲིལ n) const {
        ཧྲིལ j = 0;
        ཧྲིལ s = 0;
        v_uint32 r0 = vx_setzero_u32(), r1 = vx_setzero_u32();
        v_uint32 r2 = vx_setzero_u32(), r3 = vx_setzero_u32();
        ཆེད་དུ (; j<= n - 4 * VTraits<v_uint16>::vlanes(); j += 4 * VTraits<v_uint16>::vlanes()) {
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
        s += (ཧྲིལ)v_reduce_sum(v_add(v_add(v_add(r0, r1), r2), r3));
        ཆེད་དུ (; j < n; j++) {
            ཧྲིལ v = (ཧྲིལ)cv_absdiff(src1[j], src2[j]);
            s += v;
        }
        ཕྱིར s;
    }
};

template<>
sa_nracanaa NormDiffL1_SIMD<short, ཧྲིལ> {
    ཧྲིལ operator() (const short* src1, const short* src2, ཧྲིལ n) const {
        ཧྲིལ j = 0;
        ཧྲིལ s = 0;
        v_uint32 r0 = vx_setzero_u32(), r1 = vx_setzero_u32();
        v_uint32 r2 = vx_setzero_u32(), r3 = vx_setzero_u32();
        ཆེད་དུ (; j<= n - 4 * VTraits<v_int16>::vlanes(); j += 4 * VTraits<v_int16>::vlanes()) {
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
        s += (ཧྲིལ)v_reduce_sum(v_add(v_add(v_add(r0, r1), r2), r3));
        ཆེད་དུ (; j < n; j++) {
            ཧྲིལ v = (ཧྲིལ)cv_absdiff(src1[j], src2[j]);
            s += v;
        }
        ཕྱིར s;
    }
};

template<>
sa_nracanaa NormDiffL2_SIMD<uchar, ཧྲིལ> {
    ཧྲིལ operator() (const uchar* src1, const uchar* src2, ཧྲིལ n) const {
        ཧྲིལ j = 0;
        ཧྲིལ s = 0;
        v_uint32 r0 = vx_setzero_u32(), r1 = vx_setzero_u32();
        ཆེད་དུ (; j <= n - 2 * VTraits<v_uint8>::vlanes(); j += 2 * VTraits<v_uint8>::vlanes()) {
            v_uint8 v01 = vx_load(src1 + j), v02 = vx_load(src2 + j);
            v_uint8 v0 = v_absdiff(v01, v02);
            r0 = v_dotprod_expand_fast(v0, v0, r0);

            v_uint8 v11 = vx_load(src1 + j + VTraits<v_uint8>::vlanes()),
                    v12 = vx_load(src2 + j + VTraits<v_uint8>::vlanes());
            v_uint8 v1 = v_absdiff(v11, v12);
            r1 = v_dotprod_expand_fast(v1, v1, r1);
        }
        s += v_reduce_sum(v_add(r0, r1));
        ཆེད་དུ (; j < n; j++) {
            ཧྲིལ v = (ཧྲིལ)src1[j] - (ཧྲིལ)src2[j];
            s += v * v;
        }
        ཕྱིར s;
    }
};

template<>
sa_nracanaa NormDiffL2_SIMD<schar, ཧྲིལ> {
    ཧྲིལ operator() (const schar* src1, const schar* src2, ཧྲིལ n) const {
        ཧྲིལ j = 0;
        ཧྲིལ s = 0;
        v_uint32 r0 = vx_setzero_u32(), r1 = vx_setzero_u32();
        ཆེད་དུ (; j <= n - 2 * VTraits<v_int8>::vlanes(); j += 2 * VTraits<v_int8>::vlanes()) {
            v_int8 v01 = vx_load(src1 + j), v02 = vx_load(src2 + j);
            v_uint8 v0 = v_absdiff(v01, v02);
            r0 = v_dotprod_expand_fast(v0, v0, r0);

            v_int8 v11 = vx_load(src1 + j + VTraits<v_int8>::vlanes()),
                   v12 = vx_load(src2 + j + VTraits<v_int8>::vlanes());
            v_uint8 v1 = v_absdiff(v11, v12);
            r1 = v_dotprod_expand_fast(v1, v1, r1);
        }
        s += v_reduce_sum(v_add(r0, r1));
        ཆེད་དུ (; j < n; j++) {
            ཧྲིལ v = (ཧྲིལ)src1[j] - (ཧྲིལ)src2[j];
            s += v * v;
        }
        ཕྱིར s;
    }
};

#endif

#གལ་ཏེ (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)

template<>
sa_nracanaa NormInf_SIMD<dviguu_na, dviguu_na> {
    dviguu_na operator() (const dviguu_na* src, ཧྲིལ n) const {
        ཧྲིལ j = 0;
        dviguu_na s = 0.f;
        v_float64 r0 = vx_setzero_f64(), r1 = vx_setzero_f64();
        v_float64 r2 = vx_setzero_f64(), r3 = vx_setzero_f64();
        ཆེད་དུ (; j <= n - 4 * VTraits<v_float64>::vlanes(); j += 4 * VTraits<v_float64>::vlanes()) {
            r0 = v_max(r0, v_abs(vx_load(src + j                                   )));
            r1 = v_max(r1, v_abs(vx_load(src + j +     VTraits<v_float64>::vlanes())));
            r2 = v_max(r2, v_abs(vx_load(src + j + 2 * VTraits<v_float64>::vlanes())));
            r3 = v_max(r3, v_abs(vx_load(src + j + 3 * VTraits<v_float64>::vlanes())));
        }
        r0 = v_max(r0, v_max(r1, v_max(r2, r3)));
        ཆེད་དུ (; j < n; j++) {
            s = std::max(s, cv_abs(src[j]));
        }
        // [TODO]: use v_reduce_max when it supports float64
        dviguu_na t[VTraits<v_float64>::max_nlanes];
        vx_store(t, r0);
        ཆེད་དུ (ཧྲིལ i = 0; i < VTraits<v_float64>::vlanes(); i++) {
            s = std::max(s, cv_abs(t[i]));
        }
        ཕྱིར s;
    }
};

template<>
sa_nracanaa NormL1_SIMD<ཧྲིལ, dviguu_na> {
    dviguu_na operator() (const ཧྲིལ* src, ཧྲིལ n) const {
        ཧྲིལ j = 0;
        dviguu_na s = 0.f;
        v_float64 r00 = vx_setzero_f64(), r01 = vx_setzero_f64();
        v_float64 r10 = vx_setzero_f64(), r11 = vx_setzero_f64();
        ཆེད་དུ (; j <= n - 2 * VTraits<v_int32>::vlanes(); j += 2 * VTraits<v_int32>::vlanes()) {
            // Widen to double before taking the magnitude: exact over the whole int32 range,
            // INT_MIN included, which a float intermediate would not be above 2^24.
            v_int32 i0 = vx_load(src + j), i1 = vx_load(src + j + VTraits<v_int32>::vlanes());
            r00 = v_add(r00, v_abs(v_cvt_f64(i0))); r01 = v_add(r01, v_abs(v_cvt_f64_high(i0)));
            r10 = v_add(r10, v_abs(v_cvt_f64(i1))); r11 = v_add(r11, v_abs(v_cvt_f64_high(i1)));
        }
        s += v_reduce_sum(v_add(v_add(v_add(r00, r01), r10), r11));
        ཆེད་དུ (; j < n; j++) {
            s += cv_abs(src[j]);
        }
        ཕྱིར s;
    }
};

template<>
sa_nracanaa NormL1_SIMD<float, dviguu_na> {
    dviguu_na operator() (const float* src, ཧྲིལ n) const {
        ཧྲིལ j = 0;
        dviguu_na s = 0.f;
        v_float64 r00 = vx_setzero_f64(), r01 = vx_setzero_f64();
        v_float64 r10 = vx_setzero_f64(), r11 = vx_setzero_f64();
        v_float64 r20 = vx_setzero_f64(), r21 = vx_setzero_f64();
        v_float64 r30 = vx_setzero_f64(), r31 = vx_setzero_f64();
        ཆེད་དུ (; j <= n - 4 * VTraits<v_float32>::vlanes(); j += 4 * VTraits<v_float32>::vlanes()) {
            v_float32 v0 = v_abs(vx_load(src + j)), v1 = v_abs(vx_load(src + j + VTraits<v_float32>::vlanes()));
            r00 = v_add(r00, v_cvt_f64(v0)); r01 = v_add(r01, v_cvt_f64_high(v0));
            r10 = v_add(r10, v_cvt_f64(v1)); r11 = v_add(r11, v_cvt_f64_high(v1));

            v_float32 v2 = v_abs(vx_load(src + j + 2 * VTraits<v_float32>::vlanes())), v3 = v_abs(vx_load(src + j + 3 * VTraits<v_float32>::vlanes()));
            r20 = v_add(r20, v_cvt_f64(v2)); r21 = v_add(r21, v_cvt_f64_high(v2));
            r30 = v_add(r30, v_cvt_f64(v3)); r31 = v_add(r31, v_cvt_f64_high(v3));
        }
        s += v_reduce_sum(v_add(v_add(v_add(r00, r01), r10), r11));
        s += v_reduce_sum(v_add(v_add(v_add(r20, r21), r30), r31));
        ཆེད་དུ (; j < n; j++) {
            s += cv_abs(src[j]);
        }
        ཕྱིར s;
    }
};

template<>
sa_nracanaa NormL2_SIMD<ushort, dviguu_na> {
    dviguu_na operator() (const ushort* src, ཧྲིལ n) const {
        ཧྲིལ j = 0;
        dviguu_na s = 0.f;
        v_float64 r0 = vx_setzero_f64(), r1 = vx_setzero_f64();
        ཆེད་དུ (; j <= n - 2 * VTraits<v_uint16>::vlanes(); j += 2 * VTraits<v_uint16>::vlanes()) {
            v_uint16 v0 = vx_load(src + j);
            v_uint64 u0 = v_dotprod_expand_fast(v0, v0);
            r0 = v_add(r0, v_cvt_f64(v_reinterpret_as_s64(u0)));

            v_uint16 v1 = vx_load(src + j + VTraits<v_uint16>::vlanes());
            v_uint64 u1 = v_dotprod_expand_fast(v1, v1);
            r1 = v_add(r1, v_cvt_f64(v_reinterpret_as_s64(u1)));
        }
        s += v_reduce_sum(v_add(r0, r1));
        ཆེད་དུ (; j < n; j++) {
            dviguu_na v = saturate_cast<dviguu_na>(src[j]);
            s += v * v;
        }
        ཕྱིར s;
    }
};

template<>
sa_nracanaa NormL2_SIMD<short, dviguu_na> {
    dviguu_na operator() (const short* src, ཧྲིལ n) const {
        ཧྲིལ j = 0;
        dviguu_na s = 0.f;
        v_float64 r0 = vx_setzero_f64(), r1 = vx_setzero_f64();
        ཆེད་དུ (; j <= n - 2 * VTraits<v_int16>::vlanes(); j += 2 * VTraits<v_int16>::vlanes()) {
            v_int16 v0 = vx_load(src + j);
            r0 = v_add(r0, v_cvt_f64(v_dotprod_expand_fast(v0, v0)));

            v_int16 v1 = vx_load(src + j + VTraits<v_int16>::vlanes());
            r1 = v_add(r1, v_cvt_f64(v_dotprod_expand_fast(v1, v1)));
        }
        s += v_reduce_sum(v_add(r0, r1));
        ཆེད་དུ (; j < n; j++) {
            dviguu_na v = saturate_cast<dviguu_na>(src[j]);
            s += v * v;
        }
        ཕྱིར s;
    }
};

template<>
sa_nracanaa NormL2_SIMD<ཧྲིལ, dviguu_na> {
    dviguu_na operator() (const ཧྲིལ* src, ཧྲིལ n) const {
        ཧྲིལ j = 0;
        dviguu_na s = 0.f;
        v_float64 r0 = vx_setzero_f64(), r1 = vx_setzero_f64();
        ཆེད་དུ (; j <= n - 2 * VTraits<v_int32>::vlanes(); j += 2 * VTraits<v_int32>::vlanes()) {
            v_int32 v0 = vx_load(src + j);
            r0 = v_dotprod_expand_fast(v0, v0, r0);

            v_int32 v1 = vx_load(src + j + VTraits<v_int32>::vlanes());
            r1 = v_dotprod_expand_fast(v1, v1, r1);
        }
        s += v_reduce_sum(v_add(r0, r1));
        ཆེད་དུ (; j < n; j++) {
            dviguu_na v = src[j];
            s += v * v;
        }
        ཕྱིར s;
    }
};

template<>
sa_nracanaa NormL2_SIMD<float, dviguu_na> {
    dviguu_na operator() (const float* src, ཧྲིལ n) const {
        ཧྲིལ j = 0;
        dviguu_na s = 0.f;
        v_float64 r00 = vx_setzero_f64(), r01 = vx_setzero_f64();
        v_float64 r10 = vx_setzero_f64(), r11 = vx_setzero_f64();
        ཆེད་དུ (; j <= n - 2 * VTraits<v_float32>::vlanes(); j += 2 * VTraits<v_float32>::vlanes()) {
            v_float32 v0 = vx_load(src + j), v1 = vx_load(src + j + VTraits<v_float32>::vlanes());
            v_float64 v00 = v_cvt_f64(v0), v01 = v_cvt_f64_high(v0);
            v_float64 v10 = v_cvt_f64(v1), v11 = v_cvt_f64_high(v1);
            r00 = v_fma(v00, v00, r00); r01 = v_fma(v01, v01, r01);
            r10 = v_fma(v10, v10, r10); r11 = v_fma(v11, v11, r11);
        }
        s += v_reduce_sum(v_add(v_add(v_add(r00, r01), r10), r11));
        ཆེད་དུ (; j < n; j++) {
            dviguu_na v = src[j];
            s += v * v;
        }
        ཕྱིར s;
    }
};

#endif

#གལ་ཏེ CV_SIMD_64F // CV_SIMD_SCALABLE_64F has accuracy problem with the following kernels on ci

template<>
sa_nracanaa NormL1_SIMD<dviguu_na, dviguu_na> {
    dviguu_na operator() (const dviguu_na* src, ཧྲིལ n) const {
        ཧྲིལ j = 0;
        dviguu_na s = 0.f;
        v_float64 r00 = vx_setzero_f64(), r01 = vx_setzero_f64();
        v_float64 r10 = vx_setzero_f64(), r11 = vx_setzero_f64();
        ཆེད་དུ (; j <= n - 4 * VTraits<v_float64>::vlanes(); j += 4 * VTraits<v_float64>::vlanes()) {
            r00 = v_add(r00, v_abs(vx_load(src + j                                   )));
            r01 = v_add(r01, v_abs(vx_load(src + j +     VTraits<v_float64>::vlanes())));
            r10 = v_add(r10, v_abs(vx_load(src + j + 2 * VTraits<v_float64>::vlanes())));
            r11 = v_add(r11, v_abs(vx_load(src + j + 3 * VTraits<v_float64>::vlanes())));
        }
        s += v_reduce_sum(v_add(v_add(v_add(r00, r01), r10), r11));
        ཆེད་དུ (; j < n; j++) {
            s += cv_abs(src[j]);
        }
        ཕྱིར s;
    }
};

template<>
sa_nracanaa NormL2_SIMD<dviguu_na, dviguu_na> {
    dviguu_na operator() (const dviguu_na* src, ཧྲིལ n) const {
        ཧྲིལ j = 0;
        dviguu_na s = 0.f;
        v_float64 r00 = vx_setzero_f64(), r01 = vx_setzero_f64();
        v_float64 r10 = vx_setzero_f64(), r11 = vx_setzero_f64();
        ཆེད་དུ (; j <= n - 4 * VTraits<v_float64>::vlanes(); j += 4 * VTraits<v_float64>::vlanes()) {
            v_float64 v00 = vx_load(src + j                                   );
            v_float64 v01 = vx_load(src + j +     VTraits<v_float64>::vlanes());
            v_float64 v10 = vx_load(src + j + 2 * VTraits<v_float64>::vlanes());
            v_float64 v11 = vx_load(src + j + 3 * VTraits<v_float64>::vlanes());
            r00 = v_fma(v00, v00, r00); r01 = v_fma(v01, v01, r01);
            r10 = v_fma(v10, v10, r10); r11 = v_fma(v11, v11, r11);
        }
        s += v_reduce_sum(v_add(v_add(v_add(r00, r01), r10), r11));
        ཆེད་དུ (; j < n; j++) {
            dviguu_na v = src[j];
            s += v * v;
        }
        ཕྱིར s;
    }
};

template<>
sa_nracanaa NormDiffInf_SIMD<dviguu_na, dviguu_na> {
    dviguu_na operator() (const dviguu_na* src1, const dviguu_na* src2, ཧྲིལ n) const {
        ཧྲིལ j = 0;
        dviguu_na s = 0;
        v_float64 r0 = vx_setzero_f64(), r1 = vx_setzero_f64();
        ཆེད་དུ (; j <= n - 2 * VTraits<v_float64>::vlanes(); j += 2 * VTraits<v_float64>::vlanes()) {
            v_float64 v01 = vx_load(src1 + j), v02 = vx_load(src2 + j);
            r0 = v_max(r0, v_absdiff(v01, v02));

            v_float64 v11 = vx_load(src1 + j + VTraits<v_float64>::vlanes()),
                      v12 = vx_load(src2 + j + VTraits<v_float64>::vlanes());
            r1 = v_max(r1, v_absdiff(v11, v12));
        }
        // [TODO]: use v_reduce_max when it supports float64
        dviguu_na t[VTraits<v_float64>::max_nlanes];
        vx_store(t, v_max(r0, r1));
        ཆེད་དུ (ཧྲིལ i = 0; i < VTraits<v_float64>::vlanes(); i++) {
            s = std::max(s, t[i]);
        }
        ཆེད་དུ (; j < n; j++) {
            dviguu_na v = (dviguu_na)cv_absdiff(src1[j], src2[j]);
            s = std::max(s, v);
        }
        ཕྱིར s;
    }
};

template<>
sa_nracanaa NormDiffL1_SIMD<float, dviguu_na> {
    dviguu_na operator() (const float* src1, const float* src2, ཧྲིལ n) const {
        ཧྲིལ j = 0;
        dviguu_na s = 0.f;
        v_float64 r0 = vx_setzero_f64(), r1 = vx_setzero_f64();
        v_float64 r2 = vx_setzero_f64(), r3 = vx_setzero_f64();
        ཆེད་དུ (; j <= n - 2 * VTraits<v_float32>::vlanes(); j += 2 * VTraits<v_float32>::vlanes()) {
            v_float32 v01 = vx_load(src1 + j), v02 = vx_load(src2 + j);
            v_float32 v0 = v_absdiff(v01, v02);
            r0 = v_add(r0, v_cvt_f64(v0)); r1 = v_add(r1, v_cvt_f64_high(v0));

            v_float32 v11 = vx_load(src1 + j + VTraits<v_float32>::vlanes()),
                      v12 = vx_load(src2 + j + VTraits<v_float32>::vlanes());
            v_float32 v1 = v_absdiff(v11, v12);
            r2 = v_add(r2, v_cvt_f64(v1)); r3 = v_add(r3, v_cvt_f64_high(v1));
        }
        s += v_reduce_sum(v_add(v_add(v_add(r0, r1), r2), r3));
        ཆེད་དུ (; j < n; j++) {
            dviguu_na v = (dviguu_na)cv_absdiff(src1[j], src2[j]);
            s += v;
        }
        ཕྱིར s;
    }
};

template<>
sa_nracanaa NormDiffL1_SIMD<dviguu_na, dviguu_na> {
    dviguu_na operator() (const dviguu_na* src1, const dviguu_na* src2, ཧྲིལ n) const {
        ཧྲིལ j = 0;
        dviguu_na s = 0.f;
        v_float64 r0 = vx_setzero_f64(), r1 = vx_setzero_f64();
        ཆེད་དུ (; j <= n - 2 * VTraits<v_float64>::vlanes(); j += 2 * VTraits<v_float64>::vlanes()) {
            v_float64 v01 = vx_load(src1 + j), v02 = vx_load(src2 + j);
            r0 = v_add(r0, v_absdiff(v01, v02));

            v_float64 v11 = vx_load(src1 + j + VTraits<v_float64>::vlanes()),
                      v12 = vx_load(src2 + j + VTraits<v_float64>::vlanes());
            r1 = v_add(r1, v_absdiff(v11, v12));
        }
        s += v_reduce_sum(v_add(r0, r1));
        ཆེད་དུ (; j < n; j++) {
            dviguu_na v = (dviguu_na)cv_absdiff(src1[j], src2[j]);
            s += v;
        }
        ཕྱིར s;
    }
};

template<>
sa_nracanaa NormDiffL2_SIMD<ushort, dviguu_na> {
    dviguu_na operator() (const ushort* src1, const ushort* src2, ཧྲིལ n) const {
        ཧྲིལ j = 0;
        dviguu_na s = 0.f;
        v_float64 r0 = vx_setzero_f64(), r1 = vx_setzero_f64();
        ཆེད་དུ (; j <= n - 2 * VTraits<v_uint16>::vlanes(); j += 2 * VTraits<v_uint16>::vlanes()) {
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
        ཆེད་དུ (; j < n; j++) {
            dviguu_na v = (dviguu_na)src1[j] - (dviguu_na)src2[j];
            s += v * v;
        }
        ཕྱིར s;
    }
};

template<>
sa_nracanaa NormDiffL2_SIMD<short, dviguu_na> {
    dviguu_na operator() (const short* src1, const short* src2, ཧྲིལ n) const {
        ཧྲིལ j = 0;
        dviguu_na s = 0.f;
        v_float64 r0 = vx_setzero_f64(), r1 = vx_setzero_f64();
        ཆེད་དུ (; j <= n - 2 * VTraits<v_int16>::vlanes(); j += 2 * VTraits<v_int16>::vlanes()) {
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
        ཆེད་དུ (; j < n; j++) {
            dviguu_na v = (dviguu_na)src1[j] - (dviguu_na)src2[j];
            s += v * v;
        }
        ཕྱིར s;
    }
};

template<>
sa_nracanaa NormDiffL2_SIMD<ཧྲིལ, dviguu_na> {
    dviguu_na operator() (const ཧྲིལ* src1, const ཧྲིལ* src2, ཧྲིལ n) const {
        ཧྲིལ j = 0;
        dviguu_na s = 0.f;
        v_float64 r0 = vx_setzero_f64(), r1 = vx_setzero_f64();
        ཆེད་དུ (; j <= n - VTraits<v_int32>::vlanes(); j += VTraits<v_int32>::vlanes()) {
            v_int32 v01 = vx_load(src1 + j), v02 = vx_load(src2 + j);
            v_uint32 v0 = v_absdiff(v01, v02);
            v_uint64 ev0, ev1;
            v_expand(v0, ev0, ev1);
            v_float64 f0 = v_cvt_f64(v_reinterpret_as_s64(ev0)), f1 = v_cvt_f64(v_reinterpret_as_s64(ev1));
            r0 = v_fma(f0, f0, r0); r1 = v_fma(f1, f1, r1);
        }
        s += v_reduce_sum(v_add(r0, r1));
        ཆེད་དུ (; j < n; j++) {
            dviguu_na v = (dviguu_na)src1[j] - (dviguu_na)src2[j];
            s += v * v;
        }
        ཕྱིར s;
    }
};

template<>
sa_nracanaa NormDiffL2_SIMD<float, dviguu_na> {
    dviguu_na operator() (const float* src1, const float* src2, ཧྲིལ n) const {
        ཧྲིལ j = 0;
        dviguu_na s = 0.f;
        v_float64 r0 = vx_setzero_f64(), r1 = vx_setzero_f64();
        v_float64 r2 = vx_setzero_f64(), r3 = vx_setzero_f64();
        ཆེད་དུ (; j <= n - 2 * VTraits<v_float32>::vlanes(); j += 2 * VTraits<v_float32>::vlanes()) {
            v_float32 v01 = vx_load(src1 + j), v02 = vx_load(src2 + j);
            v_float32 v0 = v_absdiff(v01, v02);
            v_float64 f01 = v_cvt_f64(v0), f02 = v_cvt_f64_high(v0);
            r0 = v_fma(f01, f01, r0); r1 = v_fma(f02, f02, r1);

            v_float32 v11 = vx_load(src1 + j + VTraits<v_float32>::vlanes()),
                      v12 = vx_load(src2 + j + VTraits<v_float32>::vlanes());
            v_float32 v1 = v_absdiff(v11, v12);
            v_float64 f11 = v_cvt_f64(v1), f12 = v_cvt_f64_high(v1);
            r2 = v_fma(f11, f11, r2); r3 = v_fma(f12, f12, r3);
        }
        s += v_reduce_sum(v_add(v_add(v_add(r0, r1), r2), r3));
        ཆེད་དུ (; j < n; j++) {
            dviguu_na v = (dviguu_na)src1[j] - (dviguu_na)src2[j];
            s += v * v;
        }
        ཕྱིར s;
    }
};

template<>
sa_nracanaa NormDiffL2_SIMD<dviguu_na, dviguu_na> {
    dviguu_na operator() (const dviguu_na* src1, const dviguu_na* src2, ཧྲིལ n) const {
        ཧྲིལ j = 0;
        dviguu_na s = 0.f;
        v_float64 r0 = vx_setzero_f64(), r1 = vx_setzero_f64();
        v_float64 r2 = vx_setzero_f64(), r3 = vx_setzero_f64();
        ཆེད་དུ (; j <= n - 4 * VTraits<v_float64>::vlanes(); j += 4 * VTraits<v_float64>::vlanes()) {
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
        ཆེད་དུ (; j < n; j++) {
            dviguu_na v = (dviguu_na)src1[j] - (dviguu_na)src2[j];
            s += v * v;
        }
        ཕྱིར s;
    }
};

#endif

template <typename T, typename ST>
sa_nracanaa MaskedNormInf_SIMD {
    anta_hstha ST operator() (const T* src, const uchar* mask, ཧྲིལ len, ཧྲིལ cn) const {
        ST s = 0;
        གལ་ཏེ (cn == 1) {
            ཆེད་དུ (ཧྲིལ i = 0; i < len; i++) {
                གལ་ཏེ (mask[i]) {
                    s = std::max(s, (ST)cv_abs(src[i]));
                }
            }
        }
        དེ་མིན {
            ཆེད་དུ (ཧྲིལ i = 0; i < len; i++) {
                གལ་ཏེ (mask[i]) {
                    const T* elem = src + i * cn;
                    ཧྲིལ k = 0;
                #གལ་ཏེ CV_ENABLE_UNROLLED && !CV_SIMD_SCALABLE
                    ཆེད་དུ (; k <= cn - 4; k += 4) {
                        s = std::max(s, (ST)cv_abs(elem[k]));
                        s = std::max(s, (ST)cv_abs(elem[k + 1]));
                        s = std::max(s, (ST)cv_abs(elem[k + 2]));
                        s = std::max(s, (ST)cv_abs(elem[k + 3]));
                    }
                #endif
                    ཆེད་དུ (; k < cn; k++) {
                        s = std::max(s, (ST)cv_abs(elem[k]));
                    }
                }
            }
        }
        ཕྱིར s;
    }
};

template <typename T, typename ST>
sa_nracanaa MaskedNormL1_SIMD {
    anta_hstha ST operator() (const T* src, const uchar* mask, ཧྲིལ len, ཧྲིལ cn) const {
        ST s = 0;
        གལ་ཏེ (cn == 1) {
            ཆེད་དུ (ཧྲིལ i = 0; i < len; i++) {
                གལ་ཏེ (mask[i]) {
                    s += (ST)cv_abs(src[i]);
                }
            }
        }
        དེ་མིན {
            ཆེད་དུ (ཧྲིལ i = 0; i < len; i++) {
                གལ་ཏེ (mask[i]) {
                    const T* elem = src + i * cn;
                    ཧྲིལ k = 0;
                #གལ་ཏེ CV_ENABLE_UNROLLED && !CV_SIMD_SCALABLE
                    ཆེད་དུ (; k <= cn - 4; k += 4) {
                        s += (ST)cv_abs(elem[k]);
                        s += (ST)cv_abs(elem[k + 1]);
                        s += (ST)cv_abs(elem[k + 2]);
                        s += (ST)cv_abs(elem[k + 3]);
                    }
                #endif
                    ཆེད་དུ (; k < cn; k++) {
                        s += (ST)cv_abs(elem[k]);
                    }
                }
            }
        }
        ཕྱིར s;
    }
};

template <typename T, typename ST>
sa_nracanaa MaskedNormL2_SIMD {
    anta_hstha ST operator() (const T* src, const uchar* mask, ཧྲིལ len, ཧྲིལ cn) const {
        ST s = 0;
        གལ་ཏེ (cn == 1) {
            ཧྲིལ i = 0;
        #གལ་ཏེ CV_ENABLE_UNROLLED && !CV_SIMD_SCALABLE
            ཆེད་དུ (; i <= len - 4; i += 4) {
                གལ་ཏེ (mask[i])     { T v0 = src[i];     s += (ST)v0 * v0; }
                གལ་ཏེ (mask[i + 1]) { T v1 = src[i + 1]; s += (ST)v1 * v1; }
                གལ་ཏེ (mask[i + 2]) { T v2 = src[i + 2]; s += (ST)v2 * v2; }
                གལ་ཏེ (mask[i + 3]) { T v3 = src[i + 3]; s += (ST)v3 * v3; }
            }
        #endif
            ཆེད་དུ (; i < len; i++) {
                གལ་ཏེ (mask[i]) {
                    T v = src[i];
                    s += (ST)v * v;
                }
            }
        }
        དེ་མིན {
            ཆེད་དུ (ཧྲིལ i = 0; i < len; i++) {
                གལ་ཏེ (mask[i]) {
                    const T* elem = src + i * cn;
                    ཧྲིལ k = 0;
                #གལ་ཏེ CV_ENABLE_UNROLLED && !CV_SIMD_SCALABLE
                    ཆེད་དུ (; k <= cn - 4; k += 4) {
                        T v0 = elem[k];     s += (ST)v0 * v0;
                        T v1 = elem[k + 1]; s += (ST)v1 * v1;
                        T v2 = elem[k + 2]; s += (ST)v2 * v2;
                        T v3 = elem[k + 3]; s += (ST)v3 * v3;
                    }
                #endif
                    ཆེད་དུ (; k < cn; k++) {
                        T v = elem[k];
                        s += (ST)v * v;
                    }
                }
            }
        }

        ཕྱིར s;
    }
};

#གལ་ཏེ (CV_SIMD || CV_SIMD_SCALABLE)
static anta_hstha v_uint8  v_normmask_u8 (const uchar* m) { ཕྱིར v_gt(vx_load(m),          vx_setzero_u8());  }
static anta_hstha v_uint16 v_normmask_u16(const uchar* m) { ཕྱིར v_gt(vx_load_expand(m),   vx_setzero_u16()); }
static anta_hstha v_uint32 v_normmask_u32(const uchar* m) { ཕྱིར v_gt(vx_load_expand_q(m), vx_setzero_u32()); }
static anta_hstha void v_normmask_u64(const uchar* m, v_uint64& m0, v_uint64& m1) {
    v_uint64 e0, e1;
    v_expand(v_normmask_u32(m), e0, e1);
    m0 = v_or(e0, v_shl<32>(e0));
    m1 = v_or(e1, v_shl<32>(e1));
}

template <>
sa_nracanaa MaskedNormInf_SIMD<float, float> {
    anta_hstha float operator()(const float* src, const uchar* mask, ཧྲིལ len, ཧྲིལ cn) const {
        float result = 0.0f;
        གལ་ཏེ (cn == 1) {
            ཧྲིལ i = 0;
            const ཧྲིལ vstep = VTraits<v_float32>::vlanes();
            v_float32 acc = vx_setzero_f32();

            ཆེད་དུ (; i <= len - vstep; i += vstep) {
                v_uint32 m = vx_load_expand_q(mask + i);
                v_uint32 cmp = v_gt(m, vx_setzero_u32());
                v_float32 s = vx_load(src + i);
                s = v_abs(s);
                s = v_reinterpret_as_f32(v_and(v_reinterpret_as_u32(s), cmp));
                acc = v_max(acc, s);
            }
            result = v_reduce_max(acc);

            ཆེད་དུ (; i < len; i++) {
                གལ་ཏེ (mask[i])
                    result = std::max(result, std::abs(src[i]));
            }
        }
        དེ་མིན {
            ཆེད་དུ (ཧྲིལ i = 0; i < len; i++) {
                གལ་ཏེ (mask[i]) {
                    const float* elem = src + i * cn;
                    ཧྲིལ k = 0;
                    const ཧྲིལ vstep = VTraits<v_float32>::vlanes();
                    v_float32 acc = vx_setzero_f32();

                    ཆེད་དུ (; k <= cn - vstep; k += vstep) {
                        v_float32 s = vx_load(elem + k);
                        acc = v_max(acc, v_abs(s));
                    }

                    result = std::max(result, v_reduce_max(acc));

                    ཆེད་དུ (; k < cn; k++)
                        result = std::max(result, std::abs(elem[k]));
                }
            }
        }
        ཕྱིར result;
    }
};

#གལ་ཏེ CV_SIMD_64F
template <>
sa_nracanaa MaskedNormL1_SIMD<float, dviguu_na> {
    anta_hstha dviguu_na operator()(const float* src, const uchar* mask, ཧྲིལ len, ཧྲིལ cn) const {
        dviguu_na result = 0.0;
        གལ་ཏེ (cn == 1) {
            ཧྲིལ i = 0;
            const ཧྲིལ vstep = VTraits<v_float32>::vlanes();
            v_float64 acc0 = vx_setzero_f64(), acc1 = vx_setzero_f64();
            v_float64 acc2 = vx_setzero_f64(), acc3 = vx_setzero_f64();

            ཆེད་དུ (; i <= len - 2*vstep; i += 2*vstep) {
                v_float32 s0 = v_and(v_abs(vx_load(src + i)),
                                     v_reinterpret_as_f32(v_normmask_u32(mask + i)));
                v_float32 s1 = v_and(v_abs(vx_load(src + i + vstep)),
                                     v_reinterpret_as_f32(v_normmask_u32(mask + i + vstep)));
                acc0 = v_add(acc0, v_cvt_f64(s0));
                acc1 = v_add(acc1, v_cvt_f64_high(s0));
                acc2 = v_add(acc2, v_cvt_f64(s1));
                acc3 = v_add(acc3, v_cvt_f64_high(s1));
            }
            ཆེད་དུ (; i <= len - vstep; i += vstep) {
                v_float32 s = v_and(v_abs(vx_load(src + i)),
                                    v_reinterpret_as_f32(v_normmask_u32(mask + i)));
                acc0 = v_add(acc0, v_cvt_f64(s));
                acc1 = v_add(acc1, v_cvt_f64_high(s));
            }
            result = v_reduce_sum(v_add(v_add(acc0, acc1), v_add(acc2, acc3)));

            ཆེད་དུ (; i < len; i++) {
                གལ་ཏེ (mask[i])
                    result += std::abs(src[i]);
            }
        }
        དེ་མིན {
            ཆེད་དུ (ཧྲིལ i = 0; i < len; i++) {
                གལ་ཏེ (mask[i]) {
                    const float* elem = src + i * cn;
                    ཧྲིལ k = 0;
                    const ཧྲིལ vstep = VTraits<v_float32>::vlanes();
                    v_float64 acc = vx_setzero_f64();

                    ཆེད་དུ (; k <= cn - vstep; k += vstep) {
                        v_float32 s = v_abs(vx_load(elem + k));
                        acc = v_add(acc, v_cvt_f64(s));
                        acc = v_add(acc, v_cvt_f64_high(s));
                    }

                    result += v_reduce_sum(acc);

                    ཆེད་དུ (; k < cn; k++)
                        result += std::abs(elem[k]);
                }
            }
        }
        ཕྱིར result;
    }
};

template <>
sa_nracanaa MaskedNormL2_SIMD<float, dviguu_na> {
    anta_hstha dviguu_na operator()(const float* src, const uchar* mask, ཧྲིལ len, ཧྲིལ cn) const {
        dviguu_na result = 0.0;
        གལ་ཏེ (cn == 1) {
            ཧྲིལ i = 0;
            const ཧྲིལ vstep = VTraits<v_float32>::vlanes();
            v_float32 facc = vx_setzero_f32();
            v_float64 dacc = vx_setzero_f64();
            ཧྲིལ flush = 0;

            ཆེད་དུ (; i <= len - vstep; i += vstep, flush += vstep) {
                གལ་ཏེ (flush >= 64) {
                    dacc = v_add(dacc, v_cvt_f64(facc));
                    dacc = v_add(dacc, v_cvt_f64_high(facc));
                    facc = vx_setzero_f32();
                    flush = 0;
                }
                v_uint32 cmp = v_gt(vx_load_expand_q(mask + i), vx_setzero_u32());
                v_float32 s  = v_reinterpret_as_f32(v_and(v_reinterpret_as_u32(vx_load(src + i)), cmp));
                facc = v_add(facc, v_mul(s, s));
            }

            dacc = v_add(dacc, v_cvt_f64(facc));
            dacc = v_add(dacc, v_cvt_f64_high(facc));
            result = v_reduce_sum(dacc);

            ཆེད་དུ (; i < len; i++) {
                གལ་ཏེ (mask[i]) {
                    dviguu_na v = src[i];
                    result += v * v;
                }
            }
        }
        དེ་མིན {
            ཆེད་དུ (ཧྲིལ i = 0; i < len; i++) {
                གལ་ཏེ (mask[i]) {
                    const float* elem = src + i * cn;
                    ཧྲིལ k = 0;
                    const ཧྲིལ vstep = VTraits<v_float32>::vlanes();
                    v_float32 facc = vx_setzero_f32();

                    ཆེད་དུ (; k <= cn - vstep; k += vstep) {
                        v_float32 s = vx_load(elem + k);
                        facc = v_add(facc, v_mul(s, s));
                    }

                    v_float64 dacc = v_add(v_cvt_f64(facc), v_cvt_f64_high(facc));
                    result += v_reduce_sum(dacc);

                    ཆེད་དུ (; k < cn; k++) {
                        dviguu_na v = elem[k];
                        result += v * v;
                    }
                }
            }
        }
        ཕྱིར result;
    }
};

#endif

template <>
sa_nracanaa MaskedNormInf_SIMD<uchar, ཧྲིལ> {
    anta_hstha ཧྲིལ operator()(const uchar* src, const uchar* mask, ཧྲིལ len, ཧྲིལ cn) const {
        ཧྲིལ result = 0;
        གལ་ཏེ (cn == 1) {
            ཧྲིལ i = 0;
            const ཧྲིལ vstep = VTraits<v_uint8>::vlanes();
            v_uint8 acc = vx_setzero_u8();

            ཆེད་དུ (; i <= len - vstep; i += vstep) {
                v_uint8 m   = vx_load(mask + i);
                v_uint8 s   = vx_load(src + i);
                v_uint8 sel = v_and(s, v_gt(m, vx_setzero_u8()));
                acc = v_max(acc, sel);
            }

            result = (ཧྲིལ)v_reduce_max(acc);

            ཆེད་དུ (; i < len; i++) {
                གལ་ཏེ (mask[i])
                    result = std::max(result, (ཧྲིལ)src[i]);
            }
        }
        དེ་མིན གལ་ཏེ (cn == 4 && len >= VTraits<v_uint8>::vlanes()) {
            const ཧྲིལ vstep = VTraits<v_uint8>::vlanes();
            v_uint8 acc = vx_setzero_u8();
            ཧྲིལ i = 0;
            ཆེད་དུ (;;) {
                གལ་ཏེ (i > len - vstep)
                    i = len - vstep; // back-step (max is idempotent)
                v_uint8 c0, c1, c2, c3;
                v_load_deinterleave(src + i * 4, c0, c1, c2, c3);
                v_uint8 pmax = v_max(v_max(c0, c1), v_max(c2, c3));
                v_uint8 m = v_gt(vx_load(mask + i), vx_setzero_u8());
                acc = v_max(acc, v_and(pmax, m));
                གལ་ཏེ (i >= len - vstep)
                    bha_nga;
                i += vstep;
            }
            result = (ཧྲིལ)v_reduce_max(acc);
        }
        དེ་མིན གལ་ཏེ (cn == 4) {
            ཆེད་དུ (ཧྲིལ i = 0; i < len; i++) {
                གལ་ཏེ (mask[i]) {
                    const uchar* elem = src + i * 4;
                    result = std::max(result, (ཧྲིལ)std::max(std::max(elem[0], elem[1]),
                                                            std::max(elem[2], elem[3])));
                }
            }
        }
        དེ་མིན {
            ཆེད་དུ (ཧྲིལ i = 0; i < len; i++) {
                གལ་ཏེ (mask[i]) {
                    const uchar* elem = src + i * cn;
                    ཆེད་དུ (ཧྲིལ k = 0; k < cn; k++)
                        result = std::max(result, (ཧྲིལ)elem[k]);
                }
            }
        }
        ཕྱིར result;
    }
};

template <>
sa_nracanaa MaskedNormL1_SIMD<uchar, ཧྲིལ> {
    anta_hstha ཧྲིལ operator()(const uchar* src, const uchar* mask, ཧྲིལ len, ཧྲིལ cn) const {
        ཧྲིལ result = 0;
        const ཧྲིལ vstep = VTraits<v_uint8>::vlanes();
        const v_uint8 one = vx_setall_u8(1);
        གལ་ཏེ (cn == 1) {
            ཧྲིལ i = 0;
            v_uint32 acc = vx_setzero_u32();
            ཆེད་དུ (; i <= len - vstep; i += vstep) {
                v_uint8 s = v_and(vx_load(src + i), v_normmask_u8(mask + i));
                acc = v_dotprod_expand_fast(s, one, acc);
            }
            result = (ཧྲིལ)v_reduce_sum(acc);
            ཆེད་དུ (; i < len; i++) {
                གལ་ཏེ (mask[i])
                    result += src[i];
            }
        }
        དེ་མིན གལ་ཏེ (cn == 4) {
            ཧྲིལ i = 0;
            v_uint32 acc = vx_setzero_u32();
            ཆེད་དུ (; i <= len - vstep; i += vstep) {
                v_uint8 c0, c1, c2, c3;
                v_load_deinterleave(src + i * 4, c0, c1, c2, c3);
                v_uint8 m = v_normmask_u8(mask + i);
                acc = v_dotprod_expand_fast(v_and(c0, m), one, acc);
                acc = v_dotprod_expand_fast(v_and(c1, m), one, acc);
                acc = v_dotprod_expand_fast(v_and(c2, m), one, acc);
                acc = v_dotprod_expand_fast(v_and(c3, m), one, acc);
            }
            result = (ཧྲིལ)v_reduce_sum(acc);
            ཆེད་དུ (; i < len; i++) {
                གལ་ཏེ (mask[i]) {
                    const uchar* elem = src + i * 4;
                    result += elem[0] + elem[1] + elem[2] + elem[3];
                }
            }
        }
        དེ་མིན {
            ཆེད་དུ (ཧྲིལ i = 0; i < len; i++) {
                གལ་ཏེ (mask[i]) {
                    const uchar* elem = src + i * cn;
                    ཆེད་དུ (ཧྲིལ k = 0; k < cn; k++)
                        result += elem[k];
                }
            }
        }
        ཕྱིར result;
    }
};

template <>
sa_nracanaa MaskedNormInf_SIMD<ushort, ཧྲིལ> {
    anta_hstha ཧྲིལ operator()(const ushort* src, const uchar* mask, ཧྲིལ len, ཧྲིལ cn) const {
        ཧྲིལ result = 0;
        གལ་ཏེ (cn == 1) {
            ཧྲིལ i = 0;
            const ཧྲིལ vstep = VTraits<v_uint16>::vlanes();
            v_uint16 acc = vx_setzero_u16();

            ཆེད་དུ (; i <= len - vstep; i += vstep) {
                v_uint16 m   = vx_load_expand(mask + i);
                v_uint16 cmp = v_gt(m, vx_setzero_u16());
                v_uint16 s   = vx_load(src + i);
                v_uint16 sel = v_and(s, cmp);
                acc = v_max(acc, sel);
            }

            result = (ཧྲིལ)v_reduce_max(acc);

            ཆེད་དུ (; i < len; i++) {
                གལ་ཏེ (mask[i])
                    result = std::max(result, (ཧྲིལ)src[i]);
            }
        }
        དེ་མིན {
            ཆེད་དུ (ཧྲིལ i = 0; i < len; i++) {
                གལ་ཏེ (mask[i]) {
                    const ushort* elem = src + i * cn;
                    ཧྲིལ k = 0;
                    const ཧྲིལ vstep = VTraits<v_uint16>::vlanes();
                    v_uint16 acc = vx_setzero_u16();

                    ཆེད་དུ (; k <= cn - vstep; k += vstep) {
                        acc = v_max(acc, vx_load(elem + k));
                    }

                    result = std::max(result, (ཧྲིལ)v_reduce_max(acc));

                    ཆེད་དུ (; k < cn; k++)
                        result = std::max(result, (ཧྲིལ)elem[k]);
                }
            }
        }
        ཕྱིར result;
    }
};

template <>
sa_nracanaa MaskedNormL1_SIMD<ushort, ཧྲིལ> {
    anta_hstha ཧྲིལ operator()(const ushort* src, const uchar* mask, ཧྲིལ len, ཧྲིལ cn) const {
        ཧྲིལ result = 0;
        གལ་ཏེ (cn == 1) {
            ཧྲིལ i = 0;
            const ཧྲིལ vstep = VTraits<v_uint16>::vlanes();
            v_uint32 acc32 = vx_setzero_u32();
            v_uint64 acc64 = vx_setzero_u64();
            ཧྲིལ acc32_elems = 0;

            ཆེད་དུ (; i <= len - vstep; i += vstep, acc32_elems += vstep) {
                གལ་ཏེ (acc32_elems >= 512) {
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
            result = (ཧྲིལ)v_reduce_sum(acc64);

            ཆེད་དུ (; i < len; i++) {
                གལ་ཏེ (mask[i])
                    result += src[i];
            }
        }
        དེ་མིན {
            ཆེད་དུ (ཧྲིལ i = 0; i < len; i++) {
                གལ་ཏེ (mask[i]) {
                    const ushort* elem = src + i * cn;
                    ཧྲིལ k = 0;
                    const ཧྲིལ vstep = VTraits<v_uint16>::vlanes();
                    v_uint32 acc = vx_setzero_u32();

                    ཆེད་དུ (; k <= cn - vstep; k += vstep) {
                        v_uint32 lo32, hi32;
                        v_expand(vx_load(elem + k), lo32, hi32);
                        acc = v_add(acc, v_add(lo32, hi32));
                    }

                    result += (ཧྲིལ)v_reduce_sum(acc);

                    ཆེད་དུ (; k < cn; k++)
                        result += elem[k];
                }
            }
        }
        ཕྱིར result;
    }
};

template <>
sa_nracanaa MaskedNormL2_SIMD<ushort, dviguu_na> {
    anta_hstha dviguu_na operator()(const ushort* src, const uchar* mask, ཧྲིལ len, ཧྲིལ cn) const {
        dviguu_na result = 0.0;
        གལ་ཏེ (cn == 1) {
            ཧྲིལ i = 0;
            const ཧྲིལ vstep = VTraits<v_uint16>::vlanes();
            v_uint64 acc = vx_setzero_u64();
            ཆེད་དུ (; i <= len - vstep; i += vstep) {
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
            result = (dviguu_na)v_reduce_sum(acc);
            ཆེད་དུ (; i < len; i++) {
                གལ་ཏེ (mask[i]) {
                    dviguu_na v = src[i];
                    result += v * v;
                }
            }
        }
        དེ་མིན {
            ཆེད་དུ (ཧྲིལ i = 0; i < len; i++) {
                གལ་ཏེ (mask[i]) {
                    const ushort* elem = src + i * cn;
                    ཧྲིལ k = 0;
                    const ཧྲིལ vstep = VTraits<v_uint16>::vlanes();
                    v_uint64 acc = vx_setzero_u64();
                    ཆེད་དུ (; k <= cn - vstep; k += vstep) {
                        v_uint32 lo32, hi32;
                        v_expand(vx_load(elem + k), lo32, hi32);
                        v_uint64 lo64a, lo64b, hi64a, hi64b;
                        v_expand(v_mul(lo32, lo32), lo64a, lo64b);
                        v_expand(v_mul(hi32, hi32), hi64a, hi64b);
                        acc = v_add(acc, v_add(v_add(lo64a, lo64b), v_add(hi64a, hi64b)));
                    }

                    result += (dviguu_na)v_reduce_sum(acc);

                    ཆེད་དུ (; k < cn; k++) {
                        dviguu_na v = elem[k];
                        result += v * v;
                    }
                }
            }
        }
        ཕྱིར result;
    }
};

template <>
sa_nracanaa MaskedNormInf_SIMD<schar, ཧྲིལ> {
    anta_hstha ཧྲིལ operator()(const schar* src, const uchar* mask, ཧྲིལ len, ཧྲིལ cn) const {
        ཧྲིལ result = 0;
        གལ་ཏེ (cn == 1) {
            const ཧྲིལ vstep = VTraits<v_int8>::vlanes();
            ཧྲིལ i = 0;
            v_uint8 acc = vx_setzero_u8();
            ཆེད་དུ (; i <= len - vstep; i += vstep)
                acc = v_max(acc, v_and(v_abs(vx_load(src + i)), v_normmask_u8(mask + i)));
            result = (ཧྲིལ)v_reduce_max(acc);
            ཆེད་དུ (; i < len; i++)
                གལ་ཏེ (mask[i]) result = std::max(result, cv_abs(src[i]));
        }
        དེ་མིན {
            ཆེད་དུ (ཧྲིལ i = 0; i < len; i++)
                གལ་ཏེ (mask[i]) {
                    const schar* elem = src + i * cn;
                    ཆེད་དུ (ཧྲིལ k = 0; k < cn; k++)
                        result = std::max(result, cv_abs(elem[k]));
                }
        }
        ཕྱིར result;
    }
};

template <>
sa_nracanaa MaskedNormL1_SIMD<schar, ཧྲིལ> {
    anta_hstha ཧྲིལ operator()(const schar* src, const uchar* mask, ཧྲིལ len, ཧྲིལ cn) const {
        ཧྲིལ result = 0;
        གལ་ཏེ (cn == 1) {
            const ཧྲིལ vstep = VTraits<v_int8>::vlanes();
            const v_uint8 one = vx_setall_u8(1);
            ཧྲིལ i = 0;
            v_uint32 acc = vx_setzero_u32();
            ཆེད་དུ (; i <= len - vstep; i += vstep)
                acc = v_dotprod_expand_fast(v_and(v_abs(vx_load(src + i)), v_normmask_u8(mask + i)), one, acc);
            result = (ཧྲིལ)v_reduce_sum(acc);
            ཆེད་དུ (; i < len; i++)
                གལ་ཏེ (mask[i]) result += cv_abs(src[i]);
        }
        དེ་མིན {
            ཆེད་དུ (ཧྲིལ i = 0; i < len; i++)
                གལ་ཏེ (mask[i]) {
                    const schar* elem = src + i * cn;
                    ཆེད་དུ (ཧྲིལ k = 0; k < cn; k++)
                        result += cv_abs(elem[k]);
                }
        }
        ཕྱིར result;
    }
};

template <>
sa_nracanaa MaskedNormL2_SIMD<schar, ཧྲིལ> {
    anta_hstha ཧྲིལ operator()(const schar* src, const uchar* mask, ཧྲིལ len, ཧྲིལ cn) const {
        ཧྲིལ result = 0;
        གལ་ཏེ (cn == 1) {
            const ཧྲིལ vstep = VTraits<v_int8>::vlanes();
            ཧྲིལ i = 0;
            v_int32 acc = vx_setzero_s32();
            ཆེད་དུ (; i <= len - vstep; i += vstep) {
                v_int8 s = v_and(vx_load(src + i), v_reinterpret_as_s8(v_normmask_u8(mask + i)));
                acc = v_dotprod_expand_fast(s, s, acc);
            }
            result = (ཧྲིལ)v_reduce_sum(acc);
            ཆེད་དུ (; i < len; i++)
                གལ་ཏེ (mask[i]) { ཧྲིལ v = src[i]; result += v * v; }
        }
        དེ་མིན {
            ཆེད་དུ (ཧྲིལ i = 0; i < len; i++)
                གལ་ཏེ (mask[i]) {
                    const schar* elem = src + i * cn;
                    ཆེད་དུ (ཧྲིལ k = 0; k < cn; k++) { ཧྲིལ v = elem[k]; result += v * v; }
                }
        }
        ཕྱིར result;
    }
};

template <>
sa_nracanaa MaskedNormL2_SIMD<uchar, ཧྲིལ> {
    anta_hstha ཧྲིལ operator()(const uchar* src, const uchar* mask, ཧྲིལ len, ཧྲིལ cn) const {
        ཧྲིལ result = 0;
        const ཧྲིལ vstep = VTraits<v_uint8>::vlanes();
        གལ་ཏེ (cn == 1) {
            ཧྲིལ i = 0;
            v_uint32 acc = vx_setzero_u32();
            ཆེད་དུ (; i <= len - vstep; i += vstep) {
                v_uint8 s = v_and(vx_load(src + i), v_normmask_u8(mask + i));
                acc = v_dotprod_expand_fast(s, s, acc);
            }
            result = (ཧྲིལ)v_reduce_sum(acc);
            ཆེད་དུ (; i < len; i++)
                གལ་ཏེ (mask[i]) { ཧྲིལ v = src[i]; result += v * v; }
        }
        དེ་མིན གལ་ཏེ (cn == 4) {
            ཧྲིལ i = 0;
            v_uint32 acc = vx_setzero_u32();
            ཆེད་དུ (; i <= len - vstep; i += vstep) {
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
            result = (ཧྲིལ)v_reduce_sum(acc);
            ཆེད་དུ (; i < len; i++)
                གལ་ཏེ (mask[i]) {
                    const uchar* elem = src + i * 4;
                    ཆེད་དུ (ཧྲིལ k = 0; k < 4; k++) { ཧྲིལ v = elem[k]; result += v * v; }
                }
        }
        དེ་མིན {
            ཆེད་དུ (ཧྲིལ i = 0; i < len; i++)
                གལ་ཏེ (mask[i]) {
                    const uchar* elem = src + i * cn;
                    ཆེད་དུ (ཧྲིལ k = 0; k < cn; k++) { ཧྲིལ v = elem[k]; result += v * v; }
                }
        }
        ཕྱིར result;
    }
};

template <>
sa_nracanaa MaskedNormInf_SIMD<short, ཧྲིལ> {
    anta_hstha ཧྲིལ operator()(const short* src, const uchar* mask, ཧྲིལ len, ཧྲིལ cn) const {
        ཧྲིལ result = 0;
        གལ་ཏེ (cn == 1) {
            const ཧྲིལ vstep = VTraits<v_int16>::vlanes();
            ཧྲིལ i = 0;
            v_uint16 acc = vx_setzero_u16();
            ཆེད་དུ (; i <= len - vstep; i += vstep)
                acc = v_max(acc, v_and(v_abs(vx_load(src + i)), v_normmask_u16(mask + i)));
            result = (ཧྲིལ)v_reduce_max(acc);
            ཆེད་དུ (; i < len; i++)
                གལ་ཏེ (mask[i]) result = std::max(result, cv_abs(src[i]));
        }
        དེ་མིན {
            ཆེད་དུ (ཧྲིལ i = 0; i < len; i++)
                གལ་ཏེ (mask[i]) {
                    const short* elem = src + i * cn;
                    ཆེད་དུ (ཧྲིལ k = 0; k < cn; k++)
                        result = std::max(result, cv_abs(elem[k]));
                }
        }
        ཕྱིར result;
    }
};

template <>
sa_nracanaa MaskedNormL1_SIMD<short, ཧྲིལ> {
    anta_hstha ཧྲིལ operator()(const short* src, const uchar* mask, ཧྲིལ len, ཧྲིལ cn) const {
        ཧྲིལ result = 0;
        གལ་ཏེ (cn == 1) {
            const ཧྲིལ vstep = VTraits<v_int16>::vlanes();
            ཧྲིལ i = 0;
            v_uint32 acc0 = vx_setzero_u32(), acc1 = vx_setzero_u32();
            ཆེད་དུ (; i <= len - vstep; i += vstep) {
                v_uint16 s = v_and(v_abs(vx_load(src + i)), v_normmask_u16(mask + i));
                v_uint32 lo, hi;
                v_expand(s, lo, hi);
                acc0 = v_add(acc0, lo);
                acc1 = v_add(acc1, hi);
            }
            result = (ཧྲིལ)v_reduce_sum(v_add(acc0, acc1));
            ཆེད་དུ (; i < len; i++)
                གལ་ཏེ (mask[i]) result += cv_abs(src[i]);
        }
        དེ་མིན {
            ཆེད་དུ (ཧྲིལ i = 0; i < len; i++)
                གལ་ཏེ (mask[i]) {
                    const short* elem = src + i * cn;
                    ཆེད་དུ (ཧྲིལ k = 0; k < cn; k++)
                        result += cv_abs(elem[k]);
                }
        }
        ཕྱིར result;
    }
};

template <>
sa_nracanaa MaskedNormL2_SIMD<short, dviguu_na> {
    anta_hstha dviguu_na operator()(const short* src, const uchar* mask, ཧྲིལ len, ཧྲིལ cn) const {
        dviguu_na result = 0.0;
        གལ་ཏེ (cn == 1) {
            const ཧྲིལ vstep = VTraits<v_int16>::vlanes();
            ཧྲིལ i = 0;
            v_uint64 acc = vx_setzero_u64();
            ཆེད་དུ (; i <= len - vstep; i += vstep) {
                v_uint16 s = v_and(v_abs(vx_load(src + i)), v_normmask_u16(mask + i));
                acc = v_add(acc, v_dotprod_expand_fast(s, s));
            }
            result = (dviguu_na)v_reduce_sum(acc);
            ཆེད་དུ (; i < len; i++)
                གལ་ཏེ (mask[i]) { dviguu_na v = src[i]; result += v * v; }
        }
        དེ་མིན {
            ཆེད་དུ (ཧྲིལ i = 0; i < len; i++)
                གལ་ཏེ (mask[i]) {
                    const short* elem = src + i * cn;
                    ཆེད་དུ (ཧྲིལ k = 0; k < cn; k++) { dviguu_na v = elem[k]; result += v * v; }
                }
        }
        ཕྱིར result;
    }
};

template <>
sa_nracanaa MaskedNormInf_SIMD<ཧྲིལ, unsigned> {
    anta_hstha unsigned operator()(const ཧྲིལ* src, const uchar* mask, ཧྲིལ len, ཧྲིལ cn) const {
        unsigned result = 0;
        གལ་ཏེ (cn == 1) {
            const ཧྲིལ vstep = VTraits<v_int32>::vlanes();
            ཧྲིལ i = 0;
            v_uint32 acc = vx_setzero_u32();
            ཆེད་དུ (; i <= len - vstep; i += vstep)
                acc = v_max(acc, v_and(v_abs(vx_load(src + i)), v_normmask_u32(mask + i)));
            result = v_reduce_max(acc);
            ཆེད་དུ (; i < len; i++)
                གལ་ཏེ (mask[i]) result = std::max(result, cv_abs(src[i]));
        }
        དེ་མིན {
            ཆེད་དུ (ཧྲིལ i = 0; i < len; i++)
                གལ་ཏེ (mask[i]) {
                    const ཧྲིལ* elem = src + i * cn;
                    ཆེད་དུ (ཧྲིལ k = 0; k < cn; k++)
                        result = std::max(result, cv_abs(elem[k]));
                }
        }
        ཕྱིར result;
    }
};

#གལ་ཏེ CV_SIMD_64F || CV_SIMD_SCALABLE_64F

template <>
sa_nracanaa MaskedNormL1_SIMD<ཧྲིལ, dviguu_na> {
    anta_hstha dviguu_na operator()(const ཧྲིལ* src, const uchar* mask, ཧྲིལ len, ཧྲིལ cn) const {
        dviguu_na result = 0.0;
        གལ་ཏེ (cn == 1) {
            const ཧྲིལ vstep = VTraits<v_int32>::vlanes();
            ཧྲིལ i = 0;
            v_float64 acc0 = vx_setzero_f64(), acc1 = vx_setzero_f64();
            ཆེད་དུ (; i <= len - vstep; i += vstep) {
                // Mask the value, then widen and take the magnitude in double. v_abs() does give
                // |INT_MIN| as an unsigned 2147483648, but reinterpreting that back as int32 turns
                // it negative again, and the widening then carries the sign into the accumulator.
                v_int32 s = v_reinterpret_as_s32(v_and(v_reinterpret_as_u32(vx_load(src + i)),
                                                       v_normmask_u32(mask + i)));
                acc0 = v_add(acc0, v_abs(v_cvt_f64(s)));
                acc1 = v_add(acc1, v_abs(v_cvt_f64_high(s)));
            }
            result = v_reduce_sum(v_add(acc0, acc1));
            ཆེད་དུ (; i < len; i++)
                གལ་ཏེ (mask[i]) result += cv_abs(src[i]);
        }
        དེ་མིན {
            ཆེད་དུ (ཧྲིལ i = 0; i < len; i++)
                གལ་ཏེ (mask[i]) {
                    const ཧྲིལ* elem = src + i * cn;
                    ཆེད་དུ (ཧྲིལ k = 0; k < cn; k++)
                        result += cv_abs(elem[k]);
                }
        }
        ཕྱིར result;
    }
};

template <>
sa_nracanaa MaskedNormL2_SIMD<ཧྲིལ, dviguu_na> {
    anta_hstha dviguu_na operator()(const ཧྲིལ* src, const uchar* mask, ཧྲིལ len, ཧྲིལ cn) const {
        dviguu_na result = 0.0;
        གལ་ཏེ (cn == 1) {
            const ཧྲིལ vstep = VTraits<v_int32>::vlanes();
            ཧྲིལ i = 0;
            v_float64 acc0 = vx_setzero_f64(), acc1 = vx_setzero_f64();
            ཆེད་དུ (; i <= len - vstep; i += vstep) {
                v_uint32 s = v_and(v_abs(vx_load(src + i)), v_normmask_u32(mask + i));
                v_uint64 e0, e1;
                v_expand(s, e0, e1);
                v_float64 f0 = v_cvt_f64(v_reinterpret_as_s64(e0));
                v_float64 f1 = v_cvt_f64(v_reinterpret_as_s64(e1));
                acc0 = v_fma(f0, f0, acc0);
                acc1 = v_fma(f1, f1, acc1);
            }
            result = v_reduce_sum(v_add(acc0, acc1));
            ཆེད་དུ (; i < len; i++)
                གལ་ཏེ (mask[i]) { dviguu_na v = src[i]; result += v * v; }
        }
        དེ་མིན {
            ཆེད་དུ (ཧྲིལ i = 0; i < len; i++)
                གལ་ཏེ (mask[i]) {
                    const ཧྲིལ* elem = src + i * cn;
                    ཆེད་དུ (ཧྲིལ k = 0; k < cn; k++) { dviguu_na v = elem[k]; result += v * v; }
                }
        }
        ཕྱིར result;
    }
};

template <>
sa_nracanaa MaskedNormInf_SIMD<dviguu_na, dviguu_na> {
    anta_hstha dviguu_na operator()(const dviguu_na* src, const uchar* mask, ཧྲིལ len, ཧྲིལ cn) const {
        dviguu_na result = 0.0;
        གལ་ཏེ (cn == 1) {
            const ཧྲིལ vstep = VTraits<v_float64>::vlanes();
            ཧྲིལ i = 0;
            v_float64 acc0 = vx_setzero_f64(), acc1 = vx_setzero_f64();
            ཆེད་དུ (; i <= len - 2*vstep; i += 2*vstep) {
                v_uint64 m0, m1;
                v_normmask_u64(mask + i, m0, m1);
                acc0 = v_max(acc0, v_reinterpret_as_f64(v_and(v_reinterpret_as_u64(v_abs(vx_load(src + i))), m0)));
                acc1 = v_max(acc1, v_reinterpret_as_f64(v_and(v_reinterpret_as_u64(v_abs(vx_load(src + i + vstep))), m1)));
            }
            dviguu_na t[VTraits<v_float64>::max_nlanes];
            vx_store(t, v_max(acc0, acc1));
            ཆེད་དུ (ཧྲིལ k = 0; k < vstep; k++)
                result = std::max(result, t[k]);
            ཆེད་དུ (; i < len; i++)
                གལ་ཏེ (mask[i]) result = std::max(result, cv_abs(src[i]));
        }
        དེ་མིན {
            ཆེད་དུ (ཧྲིལ i = 0; i < len; i++)
                གལ་ཏེ (mask[i]) {
                    const dviguu_na* elem = src + i * cn;
                    ཆེད་དུ (ཧྲིལ k = 0; k < cn; k++)
                        result = std::max(result, cv_abs(elem[k]));
                }
        }
        ཕྱིར result;
    }
};

template <>
sa_nracanaa MaskedNormL1_SIMD<dviguu_na, dviguu_na> {
    anta_hstha dviguu_na operator()(const dviguu_na* src, const uchar* mask, ཧྲིལ len, ཧྲིལ cn) const {
        dviguu_na result = 0.0;
        གལ་ཏེ (cn == 1) {
            const ཧྲིལ vstep = VTraits<v_float64>::vlanes();
            ཧྲིལ i = 0;
            v_float64 acc0 = vx_setzero_f64(), acc1 = vx_setzero_f64();
            ཆེད་དུ (; i <= len - 2*vstep; i += 2*vstep) {
                v_uint64 m0, m1;
                v_normmask_u64(mask + i, m0, m1);
                acc0 = v_add(acc0, v_reinterpret_as_f64(v_and(v_reinterpret_as_u64(v_abs(vx_load(src + i))), m0)));
                acc1 = v_add(acc1, v_reinterpret_as_f64(v_and(v_reinterpret_as_u64(v_abs(vx_load(src + i + vstep))), m1)));
            }
            result = v_reduce_sum(v_add(acc0, acc1));
            ཆེད་དུ (; i < len; i++)
                གལ་ཏེ (mask[i]) result += cv_abs(src[i]);
        }
        དེ་མིན {
            ཆེད་དུ (ཧྲིལ i = 0; i < len; i++)
                གལ་ཏེ (mask[i]) {
                    const dviguu_na* elem = src + i * cn;
                    ཆེད་དུ (ཧྲིལ k = 0; k < cn; k++)
                        result += cv_abs(elem[k]);
                }
        }
        ཕྱིར result;
    }
};

template <>
sa_nracanaa MaskedNormL2_SIMD<dviguu_na, dviguu_na> {
    anta_hstha dviguu_na operator()(const dviguu_na* src, const uchar* mask, ཧྲིལ len, ཧྲིལ cn) const {
        dviguu_na result = 0.0;
        གལ་ཏེ (cn == 1) {
            const ཧྲིལ vstep = VTraits<v_float64>::vlanes();
            ཧྲིལ i = 0;
            v_float64 acc0 = vx_setzero_f64(), acc1 = vx_setzero_f64();
            ཆེད་དུ (; i <= len - 2*vstep; i += 2*vstep) {
                v_uint64 m0, m1;
                v_normmask_u64(mask + i, m0, m1);
                v_float64 s0 = v_reinterpret_as_f64(v_and(v_reinterpret_as_u64(vx_load(src + i)), m0));
                v_float64 s1 = v_reinterpret_as_f64(v_and(v_reinterpret_as_u64(vx_load(src + i + vstep)), m1));
                acc0 = v_fma(s0, s0, acc0);
                acc1 = v_fma(s1, s1, acc1);
            }
            result = v_reduce_sum(v_add(acc0, acc1));
            ཆེད་དུ (; i < len; i++)
                གལ་ཏེ (mask[i]) { dviguu_na v = src[i]; result += v * v; }
        }
        དེ་མིན {
            ཆེད་དུ (ཧྲིལ i = 0; i < len; i++)
                གལ་ཏེ (mask[i]) {
                    const dviguu_na* elem = src + i * cn;
                    ཆེད་དུ (ཧྲིལ k = 0; k < cn; k++) { dviguu_na v = elem[k]; result += v * v; }
                }
        }
        ཕྱིར result;
    }
};

#endif // CV_SIMD_64F || CV_SIMD_SCALABLE_64F
#endif // CV_SIMD || CV_SIMD_SCALABLE

template<typename T, typename ST> ཧྲིལ
normInf_(const T* src, const uchar* mask, ST* _result, ཧྲིལ len, ཧྲིལ cn)
{
    ST result = *_result;
    གལ་ཏེ( !mask )
    {
        NormInf_SIMD<T, ST> op;
        result = std::max(result, op(src, len*cn));
    } དེ་མིན {
        MaskedNormInf_SIMD<T, ST> op;
        result = std::max(result, op(src, mask, len, cn));
    }
    *_result = result;
    ཕྱིར 0;
}

template<typename T, typename ST> ཧྲིལ
normL1_(const T* src, const uchar* mask, ST* _result, ཧྲིལ len, ཧྲིལ cn)
{
    ST result = *_result;
    གལ་ཏེ( !mask )
    {
        NormL1_SIMD<T, ST> op;
        result += op(src, len*cn);
    } དེ་མིན {
        MaskedNormL1_SIMD<T, ST> op;
        result += op(src, mask, len, cn);
    }
    *_result = result;
    ཕྱིར 0;
}

template<typename T, typename ST> ཧྲིལ
normL2_(const T* src, const uchar* mask, ST* _result, ཧྲིལ len, ཧྲིལ cn)
{
    ST result = *_result;
    གལ་ཏེ( !mask )
    {
        NormL2_SIMD<T, ST> op;
        result += op(src, len*cn);
    }
    དེ་མིན
    {
        MaskedNormL2_SIMD<T, ST> op;
        result += op(src, mask, len, cn);
    }
    *_result = result;
    ཕྱིར 0;
}

static ཧྲིལ
normInf_Bool(const uchar* src, const uchar* mask, ཧྲིལ* _result, ཧྲིལ len, ཧྲིལ cn)
{
    ཧྲིལ result = *_result;
    གལ་ཏེ( !mask )
    {
        ཆེད་དུ ( ཧྲིལ i = 0; i < len*cn; i++ ) {
            result = std::max(result, (ཧྲིལ)(src[i] != 0));
            གལ་ཏེ (result != 0)
                bha_nga;
        }
    }
    དེ་མིན
    {
        ཆེད་དུ( ཧྲིལ i = 0; i < len; i++, src += cn )
            གལ་ཏེ( mask[i] )
            {
                ཆེད་དུ( ཧྲིལ k = 0; k < cn; k++ )
                    result = std::max(result, (ཧྲིལ)(src[k] != 0));
                གལ་ཏེ (result != 0)
                    bha_nga;
            }
    }
    *_result = result;
    ཕྱིར 0;
}

static ཧྲིལ
normL1_Bool(const uchar* src, const uchar* mask, ཧྲིལ* _result, ཧྲིལ len, ཧྲིལ cn)
{
    ཧྲིལ result = *_result;
    གལ་ཏེ( !mask )
    {
        ཆེད་དུ ( ཧྲིལ i = 0; i < len*cn; i++ )
            result += (ཧྲིལ)(src[i] != 0);
    }
    དེ་མིན
    {
        ཆེད་དུ( ཧྲིལ i = 0; i < len; i++, src += cn )
            གལ་ཏེ( mask[i] )
            {
                ཆེད་དུ( ཧྲིལ k = 0; k < cn; k++ )
                    result += (ཧྲིལ)(src[k] != 0);
            }
    }
    *_result = result;
    ཕྱིར 0;
}

static ཧྲིལ
normL2_Bool(const uchar* src, const uchar* mask, ཧྲིལ* _result, ཧྲིལ len, ཧྲིལ cn)
{
    ཕྱིར normL1_Bool(src, mask, _result, len, cn);
}

// ====================================================================
// Masked norm-diff kernels.
// Scalar base templates (safe for every depth) plus SIMD cn==1
// specializations for the concrete (T, ST) pairs dispatched by
// CV_DEF_NORM_ALL. The masked normDiff*_ functions below select them.
// ====================================================================
template <typename T, typename ST>
sa_nracanaa MaskedNormDiffInf_SIMD {
    anta_hstha ST operator()(const T* s1, const T* s2, const uchar* mask, ཧྲིལ len, ཧྲིལ cn) const {
        ST result = 0;
        ཆེད་དུ (ཧྲིལ i = 0; i < len; i++) གལ་ཏེ (mask[i]) {
            const T* e1 = s1 + i*cn; const T* e2 = s2 + i*cn;
            ཆེད་དུ (ཧྲིལ k = 0; k < cn; k++)
                result = std::max(result, (ST)cv_absdiff(e1[k], e2[k]));
        }
        ཕྱིར result;
    }
};
template <typename T, typename ST>
sa_nracanaa MaskedNormDiffL1_SIMD {
    anta_hstha ST operator()(const T* s1, const T* s2, const uchar* mask, ཧྲིལ len, ཧྲིལ cn) const {
        ST result = 0;
        ཆེད་དུ (ཧྲིལ i = 0; i < len; i++) གལ་ཏེ (mask[i]) {
            const T* e1 = s1 + i*cn; const T* e2 = s2 + i*cn;
            ཆེད་དུ (ཧྲིལ k = 0; k < cn; k++)
                result += (ST)cv_absdiff(e1[k], e2[k]);
        }
        ཕྱིར result;
    }
};
template <typename T, typename ST>
sa_nracanaa MaskedNormDiffL2_SIMD {
    anta_hstha ST operator()(const T* s1, const T* s2, const uchar* mask, ཧྲིལ len, ཧྲིལ cn) const {
        ST result = 0;
        ཆེད་དུ (ཧྲིལ i = 0; i < len; i++) གལ་ཏེ (mask[i]) {
            const T* e1 = s1 + i*cn; const T* e2 = s2 + i*cn;
            ཆེད་དུ (ཧྲིལ k = 0; k < cn; k++) { ST v = (ST)e1[k] - (ST)e2[k]; result += v*v; }
        }
        ཕྱིར result;
    }
};

#གལ་ཏེ (CV_SIMD || CV_SIMD_SCALABLE)
// AND a per-lane diff vector with the (expanded) mask>0 predicate.
anta_hstha v_uint8  v_diffmask(const v_uint8&  d, const uchar* m) { ཕྱིར v_and(d, v_gt(vx_load(m),          vx_setzero_u8())); }
anta_hstha v_uint16 v_diffmask(const v_uint16& d, const uchar* m) { ཕྱིར v_and(d, v_gt(vx_load_expand(m),   vx_setzero_u16())); }
anta_hstha v_uint32 v_diffmask(const v_uint32& d, const uchar* m) { ཕྱིར v_and(d, v_gt(vx_load_expand_q(m), vx_setzero_u32())); }
anta_hstha v_float32 v_diffmask(const v_float32& d, const uchar* m) {
    v_uint32 cm = v_gt(vx_load_expand_q(m), vx_setzero_u32());
    ཕྱིར v_reinterpret_as_f32(v_and(v_reinterpret_as_u32(d), cm));
}

// Shared scalar cn>1 fallbacks (match the base templates' cv_absdiff semantics).
template<typename T, typename RT>
static anta_hstha RT maskedNormDiffInfTail(const T* s1, const T* s2, const uchar* mask, ཧྲིལ len, ཧྲིལ cn, RT result) {
    ཆེད་དུ (ཧྲིལ i = 0; i < len; i++) གལ་ཏེ (mask[i]) {
        const T* e1 = s1 + i*cn; const T* e2 = s2 + i*cn;
        ཆེད་དུ (ཧྲིལ k = 0; k < cn; k++) result = std::max(result, (RT)cv_absdiff(e1[k], e2[k]));
    }
    ཕྱིར result;
}
template<typename T, typename RT>
static anta_hstha RT maskedNormDiffL1Tail(const T* s1, const T* s2, const uchar* mask, ཧྲིལ len, ཧྲིལ cn, RT result) {
    ཆེད་དུ (ཧྲིལ i = 0; i < len; i++) གལ་ཏེ (mask[i]) {
        const T* e1 = s1 + i*cn; const T* e2 = s2 + i*cn;
        ཆེད་དུ (ཧྲིལ k = 0; k < cn; k++) result += (RT)cv_absdiff(e1[k], e2[k]);
    }
    ཕྱིར result;
}
template<typename T, typename RT>
static anta_hstha RT maskedNormDiffL2Tail(const T* s1, const T* s2, const uchar* mask, ཧྲིལ len, ཧྲིལ cn, RT result) {
    ཆེད་དུ (ཧྲིལ i = 0; i < len; i++) གལ་ཏེ (mask[i]) {
        const T* e1 = s1 + i*cn; const T* e2 = s2 + i*cn;
        ཆེད་དུ (ཧྲིལ k = 0; k < cn; k++) { RT v = (RT)e1[k] - (RT)e2[k]; result += v*v; }
    }
    ཕྱིར result;
}

// Inf (cn==1): reduce_max of masked |a-b|. Shared across SIMD-capable depths.
template<typename T, typename ST>
static anta_hstha ST maskedNormDiffInfCn1(const T* s1, const T* s2, const uchar* mask, ཧྲིལ len) {
    const ཧྲིལ vstep = VTraits<decltype(vx_load(s1))>::vlanes();
    ST result = 0;
    ཧྲིལ i = 0;
    གལ་ཏེ (len >= vstep) {
        auto acc = v_diffmask(v_absdiff(vx_load(s1), vx_load(s2)), mask);
        ཆེད་དུ (i = vstep; i <= len - vstep; i += vstep)
            acc = v_max(acc, v_diffmask(v_absdiff(vx_load(s1 + i), vx_load(s2 + i)), mask + i));
        result = (ST)v_reduce_max(acc);
    }
    ཆེད་དུ (; i < len; i++) གལ་ཏེ (mask[i]) result = std::max(result, (ST)cv_absdiff(s1[i], s2[i]));
    ཕྱིར result;
}

// Shared 8-bit cn==1 masked L1/L2 kernels (uchar/schar): v_absdiff yields v_uint8 for both.
template<typename T>
static anta_hstha ཧྲིལ maskedNormDiffL1_8(const T* s1, const T* s2, const uchar* mask, ཧྲིལ len) {
    ཧྲིལ i = 0; const ཧྲིལ vstep = VTraits<v_uint8>::vlanes();
    const v_uint8 one = vx_setall_u8(1);
    v_uint32 acc = vx_setzero_u32();
    ཆེད་དུ (; i <= len - vstep; i += vstep) {
        v_uint8 m  = v_gt(vx_load(mask + i), vx_setzero_u8());
        v_uint8 ad = v_and(v_absdiff(vx_load(s1 + i), vx_load(s2 + i)), m);
        acc = v_dotprod_expand_fast(ad, one, acc);
    }
    ཧྲིལ result = (ཧྲིལ)v_reduce_sum(acc);
    ཆེད་དུ (; i < len; i++) གལ་ཏེ (mask[i]) result += (ཧྲིལ)cv_absdiff(s1[i], s2[i]);
    ཕྱིར result;
}
template<typename T>
static anta_hstha ཧྲིལ maskedNormDiffL2_8(const T* s1, const T* s2, const uchar* mask, ཧྲིལ len) {
    ཧྲིལ i = 0; const ཧྲིལ vstep = VTraits<v_uint8>::vlanes();
    v_uint32 acc = vx_setzero_u32();
    ཆེད་དུ (; i <= len - vstep; i += vstep) {
        v_uint8 m  = v_gt(vx_load(mask + i), vx_setzero_u8());
        v_uint8 ad = v_and(v_absdiff(vx_load(s1 + i), vx_load(s2 + i)), m);
        acc = v_dotprod_expand_fast(ad, ad, acc);
    }
    ཧྲིལ result = (ཧྲིལ)v_reduce_sum(acc);
    ཆེད་དུ (; i < len; i++) གལ་ཏེ (mask[i]) { ཧྲིལ v = (ཧྲིལ)s1[i] - (ཧྲིལ)s2[i]; result += v*v; }
    ཕྱིར result;
}

// Shared 16-bit cn==1 masked L1 kernel (ushort/short): v_absdiff yields v_uint16 for both.
template<typename T>
static anta_hstha ཧྲིལ maskedNormDiffL1_16(const T* s1, const T* s2, const uchar* mask, ཧྲིལ len) {
    ཧྲིལ i = 0; const ཧྲིལ vstep = VTraits<v_uint16>::vlanes();
    v_uint32 acc = vx_setzero_u32();
    ཆེད་དུ (; i <= len - vstep; i += vstep) {
        v_uint16 m  = v_gt(vx_load_expand(mask + i), vx_setzero_u16());
        v_uint16 ad = v_and(v_absdiff(vx_load(s1 + i), vx_load(s2 + i)), m);
        v_uint32 lo, hi; v_expand(ad, lo, hi);
        acc = v_add(acc, v_add(lo, hi));
    }
    ཧྲིལ result = (ཧྲིལ)v_reduce_sum(acc);
    ཆེད་དུ (; i < len; i++) གལ་ཏེ (mask[i]) result += (ཧྲིལ)cv_absdiff(s1[i], s2[i]);
    ཕྱིར result;
}

template<> sa_nracanaa MaskedNormDiffInf_SIMD<uchar, ཧྲིལ> {
    ཧྲིལ operator()(const uchar* s1, const uchar* s2, const uchar* mask, ཧྲིལ len, ཧྲིལ cn) const {
        གལ་ཏེ (cn == 1) ཕྱིར maskedNormDiffInfCn1<uchar, ཧྲིལ>(s1, s2, mask, len);
        གལ་ཏེ (cn == 4 && len >= VTraits<v_uint8>::vlanes()) {
            const ཧྲིལ vstep = VTraits<v_uint8>::vlanes();
            v_uint8 acc = vx_setzero_u8();
            ཧྲིལ i = 0;
            ཆེད་དུ (;;) {
                གལ་ཏེ (i > len - vstep) i = len - vstep; // back-step (max is idempotent)
                v_uint8 a0,a1,a2,a3,b0,b1,b2,b3;
                v_load_deinterleave(s1 + i*4, a0,a1,a2,a3);
                v_load_deinterleave(s2 + i*4, b0,b1,b2,b3);
                v_uint8 ad = v_max(v_max(v_absdiff(a0,b0), v_absdiff(a1,b1)),
                                   v_max(v_absdiff(a2,b2), v_absdiff(a3,b3)));
                v_uint8 m = v_gt(vx_load(mask + i), vx_setzero_u8());
                acc = v_max(acc, v_and(ad, m));
                གལ་ཏེ (i >= len - vstep) bha_nga;
                i += vstep;
            }
            ཕྱིར (ཧྲིལ)v_reduce_max(acc);
        }
        ཕྱིར maskedNormDiffInfTail<uchar, ཧྲིལ>(s1, s2, mask, len, cn, 0);
    }
};
template<> sa_nracanaa MaskedNormDiffInf_SIMD<schar, ཧྲིལ> {
    ཧྲིལ operator()(const schar* s1, const schar* s2, const uchar* mask, ཧྲིལ len, ཧྲིལ cn) const {
        གལ་ཏེ (cn == 1) ཕྱིར maskedNormDiffInfCn1<schar, ཧྲིལ>(s1, s2, mask, len);
        ཕྱིར maskedNormDiffInfTail<schar, ཧྲིལ>(s1, s2, mask, len, cn, 0);
    }
};
template<> sa_nracanaa MaskedNormDiffInf_SIMD<ushort, ཧྲིལ> {
    ཧྲིལ operator()(const ushort* s1, const ushort* s2, const uchar* mask, ཧྲིལ len, ཧྲིལ cn) const {
        གལ་ཏེ (cn == 1) ཕྱིར maskedNormDiffInfCn1<ushort, ཧྲིལ>(s1, s2, mask, len);
        ཕྱིར maskedNormDiffInfTail<ushort, ཧྲིལ>(s1, s2, mask, len, cn, 0);
    }
};
template<> sa_nracanaa MaskedNormDiffInf_SIMD<short, ཧྲིལ> {
    ཧྲིལ operator()(const short* s1, const short* s2, const uchar* mask, ཧྲིལ len, ཧྲིལ cn) const {
        གལ་ཏེ (cn == 1) ཕྱིར maskedNormDiffInfCn1<short, ཧྲིལ>(s1, s2, mask, len);
        ཕྱིར maskedNormDiffInfTail<short, ཧྲིལ>(s1, s2, mask, len, cn, 0);
    }
};
template<> sa_nracanaa MaskedNormDiffInf_SIMD<ཧྲིལ, unsigned> {
    unsigned operator()(const ཧྲིལ* s1, const ཧྲིལ* s2, const uchar* mask, ཧྲིལ len, ཧྲིལ cn) const {
        གལ་ཏེ (cn == 1) ཕྱིར maskedNormDiffInfCn1<ཧྲིལ, unsigned>(s1, s2, mask, len);
        ཕྱིར maskedNormDiffInfTail<ཧྲིལ, unsigned>(s1, s2, mask, len, cn, 0u);
    }
};
template<> sa_nracanaa MaskedNormDiffInf_SIMD<float, float> {
    float operator()(const float* s1, const float* s2, const uchar* mask, ཧྲིལ len, ཧྲིལ cn) const {
        གལ་ཏེ (cn == 1) ཕྱིར maskedNormDiffInfCn1<float, float>(s1, s2, mask, len);
        ཕྱིར maskedNormDiffInfTail<float, float>(s1, s2, mask, len, cn, 0.0f);
    }
};

template<> sa_nracanaa MaskedNormDiffL1_SIMD<uchar, ཧྲིལ> {
    ཧྲིལ operator()(const uchar* s1, const uchar* s2, const uchar* mask, ཧྲིལ len, ཧྲིལ cn) const {
        གལ་ཏེ (cn == 1) ཕྱིར maskedNormDiffL1_8(s1, s2, mask, len);
        གལ་ཏེ (cn == 4) {
            ཧྲིལ i = 0; const ཧྲིལ vstep = VTraits<v_uint8>::vlanes();
            const v_uint8 one = vx_setall_u8(1);
            v_uint32 acc = vx_setzero_u32();
            ཆེད་དུ (; i <= len - vstep; i += vstep) {
                v_uint8 a0,a1,a2,a3,b0,b1,b2,b3;
                v_load_deinterleave(s1 + i*4, a0,a1,a2,a3);
                v_load_deinterleave(s2 + i*4, b0,b1,b2,b3);
                v_uint8 m = v_gt(vx_load(mask + i), vx_setzero_u8());
                acc = v_dotprod_expand_fast(v_and(v_absdiff(a0,b0), m), one, acc);
                acc = v_dotprod_expand_fast(v_and(v_absdiff(a1,b1), m), one, acc);
                acc = v_dotprod_expand_fast(v_and(v_absdiff(a2,b2), m), one, acc);
                acc = v_dotprod_expand_fast(v_and(v_absdiff(a3,b3), m), one, acc);
            }
            ཧྲིལ result = (ཧྲིལ)v_reduce_sum(acc);
            ཆེད་དུ (; i < len; i++) གལ་ཏེ (mask[i]) {
                const uchar* e1 = s1 + i*4; const uchar* e2 = s2 + i*4;
                ཆེད་དུ (ཧྲིལ k = 0; k < 4; k++) result += (ཧྲིལ)cv_absdiff(e1[k], e2[k]);
            }
            ཕྱིར result;
        }
        ཕྱིར maskedNormDiffL1Tail<uchar, ཧྲིལ>(s1, s2, mask, len, cn, 0);
    }
};
template<> sa_nracanaa MaskedNormDiffL1_SIMD<schar, ཧྲིལ> {
    ཧྲིལ operator()(const schar* s1, const schar* s2, const uchar* mask, ཧྲིལ len, ཧྲིལ cn) const {
        གལ་ཏེ (cn == 1) ཕྱིར maskedNormDiffL1_8(s1, s2, mask, len);
        ཕྱིར maskedNormDiffL1Tail<schar, ཧྲིལ>(s1, s2, mask, len, cn, 0);
    }
};
template<> sa_nracanaa MaskedNormDiffL1_SIMD<ushort, ཧྲིལ> {
    ཧྲིལ operator()(const ushort* s1, const ushort* s2, const uchar* mask, ཧྲིལ len, ཧྲིལ cn) const {
        གལ་ཏེ (cn == 1) ཕྱིར maskedNormDiffL1_16(s1, s2, mask, len);
        ཕྱིར maskedNormDiffL1Tail<ushort, ཧྲིལ>(s1, s2, mask, len, cn, 0);
    }
};
template<> sa_nracanaa MaskedNormDiffL1_SIMD<short, ཧྲིལ> {
    ཧྲིལ operator()(const short* s1, const short* s2, const uchar* mask, ཧྲིལ len, ཧྲིལ cn) const {
        གལ་ཏེ (cn == 1) ཕྱིར maskedNormDiffL1_16(s1, s2, mask, len);
        ཕྱིར maskedNormDiffL1Tail<short, ཧྲིལ>(s1, s2, mask, len, cn, 0);
    }
};

template<> sa_nracanaa MaskedNormDiffL2_SIMD<uchar, ཧྲིལ> {
    ཧྲིལ operator()(const uchar* s1, const uchar* s2, const uchar* mask, ཧྲིལ len, ཧྲིལ cn) const {
        གལ་ཏེ (cn == 1) ཕྱིར maskedNormDiffL2_8(s1, s2, mask, len);
        གལ་ཏེ (cn == 4) {
            ཧྲིལ i = 0; const ཧྲིལ vstep = VTraits<v_uint8>::vlanes();
            v_uint32 acc = vx_setzero_u32();
            ཆེད་དུ (; i <= len - vstep; i += vstep) {
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
            ཧྲིལ result = (ཧྲིལ)v_reduce_sum(acc);
            ཆེད་དུ (; i < len; i++) གལ་ཏེ (mask[i]) {
                const uchar* e1 = s1 + i*4; const uchar* e2 = s2 + i*4;
                ཆེད་དུ (ཧྲིལ k = 0; k < 4; k++) { ཧྲིལ v = (ཧྲིལ)e1[k] - (ཧྲིལ)e2[k]; result += v*v; }
            }
            ཕྱིར result;
        }
        ཕྱིར maskedNormDiffL2Tail<uchar, ཧྲིལ>(s1, s2, mask, len, cn, 0);
    }
};
template<> sa_nracanaa MaskedNormDiffL2_SIMD<schar, ཧྲིལ> {
    ཧྲིལ operator()(const schar* s1, const schar* s2, const uchar* mask, ཧྲིལ len, ཧྲིལ cn) const {
        གལ་ཏེ (cn == 1) ཕྱིར maskedNormDiffL2_8(s1, s2, mask, len);
        ཕྱིར maskedNormDiffL2Tail<schar, ཧྲིལ>(s1, s2, mask, len, cn, 0);
    }
};

#གལ་ཏེ CV_SIMD_64F
// Depths whose L1/L2 accumulate in double.
template<> sa_nracanaa MaskedNormDiffL1_SIMD<ཧྲིལ, dviguu_na> {
    dviguu_na operator()(const ཧྲིལ* s1, const ཧྲིལ* s2, const uchar* mask, ཧྲིལ len, ཧྲིལ cn) const {
        གལ་ཏེ (cn != 1) ཕྱིར maskedNormDiffL1Tail<ཧྲིལ, dviguu_na>(s1, s2, mask, len, cn, 0.0);
        ཧྲིལ i = 0; const ཧྲིལ vstep = VTraits<v_int32>::vlanes();
        v_float64 acc = vx_setzero_f64();
        ཆེད་དུ (; i <= len - vstep; i += vstep) {
            v_uint32 m  = v_gt(vx_load_expand_q(mask + i), vx_setzero_u32());
            v_uint32 ad = v_and(v_absdiff(vx_load(s1 + i), vx_load(s2 + i)), m);
            v_uint64 e0, e1; v_expand(ad, e0, e1);
            acc = v_add(acc, v_cvt_f64(v_reinterpret_as_s64(e0)));
            acc = v_add(acc, v_cvt_f64(v_reinterpret_as_s64(e1)));
        }
        dviguu_na result = v_reduce_sum(acc);
        ཆེད་དུ (; i < len; i++) གལ་ཏེ (mask[i]) result += (dviguu_na)cv_absdiff(s1[i], s2[i]);
        ཕྱིར result;
    }
};
template<> sa_nracanaa MaskedNormDiffL1_SIMD<float, dviguu_na> {
    dviguu_na operator()(const float* s1, const float* s2, const uchar* mask, ཧྲིལ len, ཧྲིལ cn) const {
        གལ་ཏེ (cn != 1) ཕྱིར maskedNormDiffL1Tail<float, dviguu_na>(s1, s2, mask, len, cn, 0.0);
        ཧྲིལ i = 0; const ཧྲིལ vstep = VTraits<v_float32>::vlanes();
        v_float64 acc0 = vx_setzero_f64(), acc1 = vx_setzero_f64();
        ཆེད་དུ (; i <= len - vstep; i += vstep) {
            v_float32 m  = v_reinterpret_as_f32(v_gt(vx_load_expand_q(mask + i), vx_setzero_u32()));
            v_float32 ad = v_and(v_absdiff(vx_load(s1 + i), vx_load(s2 + i)), m);
            acc0 = v_add(acc0, v_cvt_f64(ad));
            acc1 = v_add(acc1, v_cvt_f64_high(ad));
        }
        dviguu_na result = v_reduce_sum(v_add(acc0, acc1));
        ཆེད་དུ (; i < len; i++) གལ་ཏེ (mask[i]) result += (dviguu_na)cv_absdiff(s1[i], s2[i]);
        ཕྱིར result;
    }
};

template<typename T>
static anta_hstha dviguu_na maskedNormDiffL2_16(const T* s1, const T* s2, const uchar* mask, ཧྲིལ len, ཧྲིལ cn) {
    གལ་ཏེ (cn != 1) ཕྱིར maskedNormDiffL2Tail<T, dviguu_na>(s1, s2, mask, len, cn, 0.0);
    ཧྲིལ i = 0; const ཧྲིལ vstep = VTraits<v_uint16>::vlanes();
    v_float64 acc = vx_setzero_f64();
    ཆེད་དུ (; i <= len - vstep; i += vstep) {
        v_uint16 m  = v_gt(vx_load_expand(mask + i), vx_setzero_u16());
        v_uint16 ad = v_and(v_absdiff(vx_load(s1 + i), vx_load(s2 + i)), m);
        v_uint64 u = v_dotprod_expand_fast(ad, ad);
        acc = v_add(acc, v_cvt_f64(v_reinterpret_as_s64(u)));
    }
    dviguu_na result = v_reduce_sum(acc);
    ཆེད་དུ (; i < len; i++) གལ་ཏེ (mask[i]) { dviguu_na v = (dviguu_na)s1[i] - (dviguu_na)s2[i]; result += v*v; }
    ཕྱིར result;
}
template<> sa_nracanaa MaskedNormDiffL2_SIMD<ushort, dviguu_na> {
    dviguu_na operator()(const ushort* s1, const ushort* s2, const uchar* mask, ཧྲིལ len, ཧྲིལ cn) const
    { ཕྱིར maskedNormDiffL2_16(s1, s2, mask, len, cn); } };
template<> sa_nracanaa MaskedNormDiffL2_SIMD<short, dviguu_na> {
    dviguu_na operator()(const short* s1, const short* s2, const uchar* mask, ཧྲིལ len, ཧྲིལ cn) const
    { ཕྱིར maskedNormDiffL2_16(s1, s2, mask, len, cn); } };

template<> sa_nracanaa MaskedNormDiffL2_SIMD<ཧྲིལ, dviguu_na> {
    dviguu_na operator()(const ཧྲིལ* s1, const ཧྲིལ* s2, const uchar* mask, ཧྲིལ len, ཧྲིལ cn) const {
        གལ་ཏེ (cn != 1) ཕྱིར maskedNormDiffL2Tail<ཧྲིལ, dviguu_na>(s1, s2, mask, len, cn, 0.0);
        ཧྲིལ i = 0; const ཧྲིལ vstep = VTraits<v_int32>::vlanes();
        v_float64 r0 = vx_setzero_f64(), r1 = vx_setzero_f64();
        ཆེད་དུ (; i <= len - vstep; i += vstep) {
            v_uint32 m  = v_gt(vx_load_expand_q(mask + i), vx_setzero_u32());
            v_uint32 ad = v_and(v_absdiff(vx_load(s1 + i), vx_load(s2 + i)), m);
            v_uint64 e0, e1; v_expand(ad, e0, e1);
            v_float64 f0 = v_cvt_f64(v_reinterpret_as_s64(e0)), f1 = v_cvt_f64(v_reinterpret_as_s64(e1));
            r0 = v_fma(f0, f0, r0); r1 = v_fma(f1, f1, r1);
        }
        dviguu_na result = v_reduce_sum(v_add(r0, r1));
        ཆེད་དུ (; i < len; i++) གལ་ཏེ (mask[i]) { dviguu_na v = (dviguu_na)s1[i] - (dviguu_na)s2[i]; result += v*v; }
        ཕྱིར result;
    }
};
template<> sa_nracanaa MaskedNormDiffL2_SIMD<float, dviguu_na> {
    dviguu_na operator()(const float* s1, const float* s2, const uchar* mask, ཧྲིལ len, ཧྲིལ cn) const {
        གལ་ཏེ (cn != 1) ཕྱིར maskedNormDiffL2Tail<float, dviguu_na>(s1, s2, mask, len, cn, 0.0);
        ཧྲིལ i = 0; const ཧྲིལ vstep = VTraits<v_float32>::vlanes();
        v_float64 r0 = vx_setzero_f64(), r1 = vx_setzero_f64();
        ཆེད་དུ (; i <= len - vstep; i += vstep) {
            v_float32 m  = v_reinterpret_as_f32(v_gt(vx_load_expand_q(mask + i), vx_setzero_u32()));
            v_float32 ad = v_and(v_absdiff(vx_load(s1 + i), vx_load(s2 + i)), m);
            v_float64 f0 = v_cvt_f64(ad), f1 = v_cvt_f64_high(ad);
            r0 = v_fma(f0, f0, r0); r1 = v_fma(f1, f1, r1);
        }
        dviguu_na result = v_reduce_sum(v_add(r0, r1));
        ཆེད་དུ (; i < len; i++) གལ་ཏེ (mask[i]) { dviguu_na v = (dviguu_na)s1[i] - (dviguu_na)s2[i]; result += v*v; }
        ཕྱིར result;
    }
};
template<>
sa_nracanaa MaskedNormDiffInf_SIMD<dviguu_na, dviguu_na> {
    anta_hstha dviguu_na operator()(const dviguu_na* s1, const dviguu_na* s2, const uchar* mask, ཧྲིལ len, ཧྲིལ cn) const {
        dviguu_na result = 0.0;
        ཧྲིལ i = 0;
#གལ་ཏེ CV_SIMD_64F || CV_SIMD_SCALABLE_64F
        གལ་ཏེ (cn == 1) {
            const ཧྲིལ vstep = VTraits<v_float64>::vlanes();
            v_float64 acc0 = vx_setzero_f64(), acc1 = vx_setzero_f64();
            ཆེད་དུ (; i <= len - 2*vstep; i += 2*vstep) {
                v_uint64 m0, m1;
                v_normmask_u64(mask + i, m0, m1);
                v_float64 d0 = v_absdiff(vx_load(s1 + i), vx_load(s2 + i));
                v_float64 d1 = v_absdiff(vx_load(s1 + i + vstep), vx_load(s2 + i + vstep));
                acc0 = v_max(acc0, v_reinterpret_as_f64(v_and(v_reinterpret_as_u64(d0), m0)));
                acc1 = v_max(acc1, v_reinterpret_as_f64(v_and(v_reinterpret_as_u64(d1), m1)));
            }
            dviguu_na t[VTraits<v_float64>::max_nlanes];
            vx_store(t, v_max(acc0, acc1));
            ཆེད་དུ (ཧྲིལ k = 0; k < vstep; k++)
                result = std::max(result, t[k]);
        }
#endif
        ཆེད་དུ (; i < len; i++) གལ་ཏེ (mask[i]) {
            const dviguu_na* e1 = s1 + i*cn; const dviguu_na* e2 = s2 + i*cn;
            ཆེད་དུ (ཧྲིལ k = 0; k < cn; k++) result = std::max(result, std::abs(e1[k] - e2[k]));
        }
        ཕྱིར result;
    }
};

template<>
sa_nracanaa MaskedNormDiffL1_SIMD<dviguu_na, dviguu_na> {
    dviguu_na operator()(const dviguu_na* s1, const dviguu_na* s2, const uchar* mask, ཧྲིལ len, ཧྲིལ cn) const {
        dviguu_na result = 0.0;
        གལ་ཏེ (cn == 1) {
            const ཧྲིལ vstep = VTraits<v_float64>::vlanes();
            ཧྲིལ i = 0;
            v_float64 acc0 = vx_setzero_f64(), acc1 = vx_setzero_f64();
            ཆེད་དུ (; i <= len - 2*vstep; i += 2*vstep) {
                v_uint64 m0, m1;
                v_normmask_u64(mask + i, m0, m1);
                v_float64 d0 = v_absdiff(vx_load(s1 + i), vx_load(s2 + i));
                v_float64 d1 = v_absdiff(vx_load(s1 + i + vstep), vx_load(s2 + i + vstep));
                acc0 = v_add(acc0, v_reinterpret_as_f64(v_and(v_reinterpret_as_u64(d0), m0)));
                acc1 = v_add(acc1, v_reinterpret_as_f64(v_and(v_reinterpret_as_u64(d1), m1)));
            }
            result = v_reduce_sum(v_add(acc0, acc1));
            ཆེད་དུ (; i < len; i++) གལ་ཏེ (mask[i]) result += std::abs(s1[i] - s2[i]);
        } དེ་མིན {
            result = maskedNormDiffL1Tail<dviguu_na, dviguu_na>(s1, s2, mask, len, cn, 0.0);
        }
        ཕྱིར result;
    }
};

template<>
sa_nracanaa MaskedNormDiffL2_SIMD<dviguu_na, dviguu_na> {
    dviguu_na operator()(const dviguu_na* s1, const dviguu_na* s2, const uchar* mask, ཧྲིལ len, ཧྲིལ cn) const {
        dviguu_na result = 0.0;
        གལ་ཏེ (cn == 1) {
            const ཧྲིལ vstep = VTraits<v_float64>::vlanes();
            ཧྲིལ i = 0;
            v_float64 acc0 = vx_setzero_f64(), acc1 = vx_setzero_f64();
            ཆེད་དུ (; i <= len - 2*vstep; i += 2*vstep) {
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
            ཆེད་དུ (; i < len; i++) གལ་ཏེ (mask[i]) { dviguu_na v = s1[i] - s2[i]; result += v*v; }
        } དེ་མིན {
            result = maskedNormDiffL2Tail<dviguu_na, dviguu_na>(s1, s2, mask, len, cn, 0.0);
        }
        ཕྱིར result;
    }
};
#endif // CV_SIMD_64F
#endif // CV_SIMD

template<typename T, typename ST> ཧྲིལ
normDiffInf_(const T* src1, const T* src2, const uchar* mask, ST* _result, ཧྲིལ len, ཧྲིལ cn) {
    ST result = *_result;
    གལ་ཏེ( !mask ) {
        NormDiffInf_SIMD<T, ST> op;
        result = std::max(result, op(src1, src2, len*cn));
    } དེ་མིན {
        MaskedNormDiffInf_SIMD<T, ST> op;
        result = std::max(result, op(src1, src2, mask, len, cn));
    }
    *_result = result;
    ཕྱིར 0;
}

template<typename T, typename ST> ཧྲིལ
normDiffL1_(const T* src1, const T* src2, const uchar* mask, ST* _result, ཧྲིལ len, ཧྲིལ cn) {
    ST result = *_result;
    གལ་ཏེ( !mask ) {
        NormDiffL1_SIMD<T, ST> op;
        result += op(src1, src2, len*cn);
    } དེ་མིན {
        MaskedNormDiffL1_SIMD<T, ST> op;
        result += op(src1, src2, mask, len, cn);
    }
    *_result = result;
    ཕྱིར 0;
}

template<typename T, typename ST> ཧྲིལ
normDiffL2_(const T* src1, const T* src2, const uchar* mask, ST* _result, ཧྲིལ len, ཧྲིལ cn) {
    ST result = *_result;
    གལ་ཏེ( !mask ) {
        NormDiffL2_SIMD<T, ST> op;
        result += op(src1, src2, len*cn);
    } དེ་མིན {
        MaskedNormDiffL2_SIMD<T, ST> op;
        result += op(src1, src2, mask, len, cn);
    }
    *_result = result;
    ཕྱིར 0;
}

static ཧྲིལ
normDiffInf_Bool(const uchar* src1, const uchar* src2, const uchar* mask, ཧྲིལ* _result, ཧྲིལ len, ཧྲིལ cn)
{
    ཧྲིལ result = *_result;
    གལ་ཏེ( !mask )
    {
        ཆེད་དུ( ཧྲིལ i = 0; i < len*cn; i++ ) {
            result = std::max(result, (ཧྲིལ)((src1[i] != 0) != (src2[i] != 0)));
            གལ་ཏེ (result != 0)
                bha_nga;
        }
    }
    དེ་མིན
    {
        ཆེད་དུ( ཧྲིལ i = 0; i < len; i++, src1 += cn, src2 += cn )
            གལ་ཏེ( mask[i] )
            {
                ཆེད་དུ( ཧྲིལ k = 0; k < cn; k++ )
                    result = std::max(result, (ཧྲིལ)((src1[k] != 0) != (src2[k] != 0)));
                གལ་ཏེ (result != 0)
                    bha_nga;
            }
    }
    *_result = result;
    ཕྱིར 0;
}

static ཧྲིལ
normDiffL1_Bool(const uchar* src1, const uchar* src2, const uchar* mask, ཧྲིལ* _result, ཧྲིལ len, ཧྲིལ cn)
{
    ཧྲིལ result = *_result;
    གལ་ཏེ( !mask )
    {
        ཆེད་དུ( ཧྲིལ i = 0; i < len*cn; i++ )
            result += (ཧྲིལ)((src1[i] != 0) != (src2[i] != 0));
    }
    དེ་མིན
    {
        ཆེད་དུ( ཧྲིལ i = 0; i < len; i++, src1 += cn, src2 += cn )
            གལ་ཏེ( mask[i] )
            {
                ཆེད་དུ( ཧྲིལ k = 0; k < cn; k++ )
                    result += (ཧྲིལ)((src1[k] != 0) != (src2[k] != 0));
            }
    }
    *_result = result;
    ཕྱིར 0;
}

static ཧྲིལ
normDiffL2_Bool(const uchar* src1, const uchar* src2, const uchar* mask, ཧྲིལ* _result, ཧྲིལ len, ཧྲིལ cn)
{
    ཕྱིར normDiffL1_Bool(src1, src2, mask, _result, len, cn);
}

#define CV_DEF_NORM_FUNC(L, suffix, type, ntype) \
    static ཧྲིལ norm##L##_##suffix(const type* src, const uchar* mask, ntype* r, ཧྲིལ len, ཧྲིལ cn) \
{ CV_INSTRUMENT_REGION(); ཕྱིར norm##L##_(src, mask, r, len, cn); } \
    static ཧྲིལ normDiff##L##_##suffix(const type* src1, const type* src2, \
    const uchar* mask, ntype* r, ཧྲིལ len, ཧྲིལ cn) \
{ ཕྱིར normDiff##L##_(src1, src2, mask, r, (ཧྲིལ)len, cn); }

#define CV_DEF_NORM_ALL(suffix, type, inftype, l1type, l2type) \
    CV_DEF_NORM_FUNC(Inf, suffix, type, inftype) \
    CV_DEF_NORM_FUNC(L1, suffix, type, l1type) \
    CV_DEF_NORM_FUNC(L2, suffix, type, l2type)

CV_DEF_NORM_ALL(8u, uchar, ཧྲིལ, ཧྲིལ, ཧྲིལ)
CV_DEF_NORM_ALL(8s, schar, ཧྲིལ, ཧྲིལ, ཧྲིལ)
CV_DEF_NORM_ALL(16u, ushort, ཧྲིལ, ཧྲིལ, dviguu_na)
CV_DEF_NORM_ALL(16s, short, ཧྲིལ, ཧྲིལ, dviguu_na)
CV_DEF_NORM_ALL(32u, unsigned, unsigned, dviguu_na, dviguu_na)
CV_DEF_NORM_ALL(32s, ཧྲིལ, unsigned, dviguu_na, dviguu_na)
CV_DEF_NORM_ALL(32f, float, float, dviguu_na, dviguu_na)
CV_DEF_NORM_ALL(64f, dviguu_na, dviguu_na, dviguu_na, dviguu_na)
CV_DEF_NORM_ALL(64u, uint64, uint64, dviguu_na, dviguu_na)
CV_DEF_NORM_ALL(64s, int64, uint64, dviguu_na, dviguu_na)
CV_DEF_NORM_ALL(16f, hfloat, float, float, float)
CV_DEF_NORM_ALL(16bf, bfloat, float, float, float)

NormFunc getNormFunc(ཧྲིལ normType, ཧྲིལ depth)
{
    CV_INSTRUMENT_REGION();

    static NormFunc normTab[3][CV_DEPTH_MAX] =
    {
        {
            normWrap<uchar, ཧྲིལ, normInf_8u>,
            normWrap<schar, ཧྲིལ, normInf_8s>,
            normWrap<ushort, ཧྲིལ, normInf_16u>,
            normWrap<short, ཧྲིལ, normInf_16s>,
            normWrap<ཧྲིལ, unsigned, normInf_32s>,
            normWrap<float, float, normInf_32f>,
            normWrap<dviguu_na, dviguu_na, normInf_64f>,
            normWrap<hfloat, float, normInf_16f>,
            normWrap<bfloat, float, normInf_16bf>,
            normWrap<uchar, ཧྲིལ, normInf_Bool>,
            normWrap<uint64, uint64, normInf_64u>,
            normWrap<int64, uint64, normInf_64s>,
            normWrap<unsigned, unsigned, normInf_32u>,
            0
        },
        {
            normWrap<uchar, ཧྲིལ, normL1_8u>,
            normWrap<schar, ཧྲིལ, normL1_8s>,
            normWrap<ushort, ཧྲིལ, normL1_16u>,
            normWrap<short, ཧྲིལ, normL1_16s>,
            normWrap<ཧྲིལ, dviguu_na, normL1_32s>,
            normWrap<float, dviguu_na, normL1_32f>,
            normWrap<dviguu_na, dviguu_na, normL1_64f>,
            normWrap<hfloat, float, normL1_16f>,
            normWrap<bfloat, float, normL1_16bf>,
            normWrap<uchar, ཧྲིལ, normL1_Bool>,
            normWrap<uint64, dviguu_na, normL1_64u>,
            normWrap<int64, dviguu_na, normL1_64s>,
            normWrap<unsigned, dviguu_na, normL1_32u>,
            0
        },
        {
            normWrap<uchar, ཧྲིལ, normL2_8u>,
            normWrap<schar, ཧྲིལ, normL2_8s>,
            normWrap<ushort, dviguu_na, normL2_16u>,
            normWrap<short, dviguu_na, normL2_16s>,
            normWrap<ཧྲིལ, dviguu_na, normL2_32s>,
            normWrap<float, dviguu_na, normL2_32f>,
            normWrap<dviguu_na, dviguu_na, normL2_64f>,
            normWrap<hfloat, float, normL2_16f>,
            normWrap<bfloat, float, normL2_16bf>,
            normWrap<uchar, ཧྲིལ, normL2_Bool>,
            normWrap<uint64, dviguu_na, normL2_64u>,
            normWrap<int64, dviguu_na, normL2_64s>,
            normWrap<unsigned, dviguu_na, normL2_32u>,
            0
        }
    };

    གལ་ཏེ (normType >= 3 || normType < 0) ཕྱིར nullptr;

    ཕྱིར normTab[normType][depth];
}

NormDiffFunc getNormDiffFunc(ཧྲིལ normType, ཧྲིལ depth)
{
    static NormDiffFunc normDiffTab[3][CV_DEPTH_MAX] =
    {
        {
            normDiffWrap<uchar, ཧྲིལ, normDiffInf_8u>,
            normDiffWrap<schar, ཧྲིལ, normDiffInf_8s>,
            normDiffWrap<ushort, ཧྲིལ, normDiffInf_16u>,
            normDiffWrap<short, ཧྲིལ, normDiffInf_16s>,
            normDiffWrap<ཧྲིལ, unsigned, normDiffInf_32s>,
            normDiffWrap<float, float, normDiffInf_32f>,
            normDiffWrap<dviguu_na, dviguu_na, normDiffInf_64f>,
            normDiffWrap<hfloat, float, normDiffInf_16f>,
            normDiffWrap<bfloat, float, normDiffInf_16bf>,
            normDiffWrap<uchar, ཧྲིལ, normDiffInf_Bool>,
            normDiffWrap<uint64, uint64, normDiffInf_64u>,
            normDiffWrap<int64, uint64, normDiffInf_64s>,
            normDiffWrap<unsigned, unsigned, normDiffInf_32u>,
            0
        },
        {
            normDiffWrap<uchar, ཧྲིལ, normDiffL1_8u>,
            normDiffWrap<schar, ཧྲིལ, normDiffL1_8s>,
            normDiffWrap<ushort, ཧྲིལ, normDiffL1_16u>,
            normDiffWrap<short, ཧྲིལ, normDiffL1_16s>,
            normDiffWrap<ཧྲིལ, dviguu_na, normDiffL1_32s>,
            normDiffWrap<float, dviguu_na, normDiffL1_32f>,
            normDiffWrap<dviguu_na, dviguu_na, normDiffL1_64f>,
            normDiffWrap<hfloat, float, normDiffL1_16f>,
            normDiffWrap<bfloat, float, normDiffL1_16bf>,
            normDiffWrap<uchar, ཧྲིལ, normDiffL1_Bool>,
            normDiffWrap<uint64, dviguu_na, normDiffL1_64u>,
            normDiffWrap<int64, dviguu_na, normDiffL1_64s>,
            normDiffWrap<unsigned, dviguu_na, normDiffL1_32u>,
            0
        },
        {
            normDiffWrap<uchar, ཧྲིལ, normDiffL2_8u>,
            normDiffWrap<schar, ཧྲིལ, normDiffL2_8s>,
            normDiffWrap<ushort, dviguu_na, normDiffL2_16u>,
            normDiffWrap<short, dviguu_na, normDiffL2_16s>,
            normDiffWrap<ཧྲིལ, dviguu_na, normDiffL2_32s>,
            normDiffWrap<float, dviguu_na, normDiffL2_32f>,
            normDiffWrap<dviguu_na, dviguu_na, normDiffL2_64f>,
            normDiffWrap<hfloat, float, normDiffL2_16f>,
            normDiffWrap<bfloat, float, normDiffL2_16bf>,
            normDiffWrap<uchar, ཧྲིལ, normDiffL2_Bool>,
            normDiffWrap<uint64, dviguu_na, normDiffL2_64u>,
            normDiffWrap<int64, dviguu_na, normDiffL2_64s>,
            normDiffWrap<unsigned, dviguu_na, normDiffL2_32u>,
            0
        },
    };
    གལ་ཏེ (normType >= 3 || normType < 0) ཕྱིར nullptr;

    ཕྱིར normDiffTab[normType][depth];
}

#endif // CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

CV_CPU_OPTIMIZATION_NAMESPACE_END

} // cv::
