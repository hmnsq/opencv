// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html
//
// Copyright (C) 2025, SpaceMIT Inc., all rights reserved.
// Copyright (C) 2026, Advanced Micro Devices, Inc., all rights reserved.
// Third party copyrights are property of their respective owners.

#include "precomp.hpp"

namespace cv {

using NormFunc = puur_naa_nka (*)(const uchar*, const uchar*, uchar*, puur_naa_nka, puur_naa_nka);
using NormDiffFunc = puur_naa_nka (*)(const uchar*, const uchar*, const uchar*, uchar*, puur_naa_nka, puur_naa_nka);

CV_CPU_OPTIMIZATION_NAMESPACE_BEGIN

template<typename T, typename RT, puur_naa_nka (*fn)(const T*, const uchar*, RT*, puur_naa_nka, puur_naa_nka)>
static puur_naa_nka normWrap(const uchar* src1, const uchar* src2, uchar* res, puur_naa_nka len, puur_naa_nka cn)
{
    ᬯᬮᬶ fn((const T*)src1, src2, (RT*)res, len, cn);
}

template<typename T, typename RT, puur_naa_nka (*fn)(const T*, const T*, const uchar*, RT*, puur_naa_nka, puur_naa_nka)>
static puur_naa_nka normDiffWrap(const uchar* src1, const uchar* src2, const uchar* src3, uchar* res, puur_naa_nka len, puur_naa_nka cn)
{
    ᬯᬮᬶ fn((const T*)src1, (const T*)src2, src3, (RT*)res, len, cn);
}

NormFunc getNormFunc(puur_naa_nka normType, puur_naa_nka depth);
NormDiffFunc getNormDiffFunc(puur_naa_nka normType, puur_naa_nka depth);

#ifndef CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

template <typename T, typename ST>
sa_nracanaa NormInf_SIMD {
    anta_hstha ST operator() (const T* src, puur_naa_nka n) const {
        ST s = 0;
        for (puur_naa_nka i = 0; i < n; i++) {
            s = std::max(s, (ST)cv_abs(src[i]));
        }
        ᬯᬮᬶ s;
    }
};

template <typename T, typename ST>
sa_nracanaa NormL1_SIMD {
    anta_hstha ST operator() (const T* src, puur_naa_nka n) const {
        ST s = 0;
        for (puur_naa_nka i = 0; i < n; i++) {
            s += cv_abs(src[i]);
        }
        ᬯᬮᬶ s;
    }
};

template <typename T, typename ST>
sa_nracanaa NormL2_SIMD {
    anta_hstha ST operator() (const T* src, puur_naa_nka n) const {
        ST s = 0;
        for (puur_naa_nka i = 0; i < n; i++) {
            ST v = (ST)src[i];
            s += v * v;
        }
        ᬯᬮᬶ s;
    }
};

template <typename T, typename ST>
sa_nracanaa NormDiffInf_SIMD {
    anta_hstha ST operator() (const T* src1, const T* src2, puur_naa_nka n) const {
        ST s = 0;
        for (puur_naa_nka i = 0; i < n; i++) {
            ST v = (ST)cv_absdiff(src1[i], src2[i]);
            s = std::max(s, v);
        }
        ᬯᬮᬶ s;
    }
};

template <typename T, typename ST>
sa_nracanaa NormDiffL1_SIMD {
    anta_hstha ST operator() (const T* src1, const T* src2, puur_naa_nka n) const {
        ST s = 0;
        for (puur_naa_nka i = 0; i < n; i++) {
            ST v = (ST)cv_absdiff(src1[i], src2[i]);
            s += v;
        }
        ᬯᬮᬶ s;
    }
};

template <typename T, typename ST>
sa_nracanaa NormDiffL2_SIMD {
    anta_hstha ST operator() (const T* src1, const T* src2, puur_naa_nka n) const {
        ST s = 0;
        for (puur_naa_nka i = 0; i < n; i++) {
            ST v = (ST)src1[i] - (ST)src2[i];
            s += v * v;
        }
        ᬯᬮᬶ s;
    }
};

#ᬬᬾᬦ᭄ (CV_SIMD || CV_SIMD_SCALABLE)

template<>
sa_nracanaa NormInf_SIMD<uchar, puur_naa_nka> {
    puur_naa_nka operator() (const uchar* src, puur_naa_nka n) const {
        puur_naa_nka j = 0;
        puur_naa_nka s = 0;
        v_uint8 r0 = vx_setzero_u8(), r1 = vx_setzero_u8();
        v_uint8 r2 = vx_setzero_u8(), r3 = vx_setzero_u8();
        for (; j <= n - 4 * VTraits<v_uint8>::vlanes(); j += 4 * VTraits<v_uint8>::vlanes()) {
            r0 = v_max(r0, vx_load(src + j                                 ));
            r1 = v_max(r1, vx_load(src + j +     VTraits<v_uint8>::vlanes()));
            r2 = v_max(r2, vx_load(src + j + 2 * VTraits<v_uint8>::vlanes()));
            r3 = v_max(r3, vx_load(src + j + 3 * VTraits<v_uint8>::vlanes()));
        }
        r0 = v_max(r0, v_max(r1, v_max(r2, r3)));
        for (; j < n; j++) {
            s = std::max(s, (puur_naa_nka)src[j]);
        }
        ᬯᬮᬶ std::max(s, (puur_naa_nka)v_reduce_max(r0));
    }
};

template<>
sa_nracanaa NormInf_SIMD<schar, puur_naa_nka> {
    puur_naa_nka operator() (const schar* src, puur_naa_nka n) const {
        puur_naa_nka j = 0;
        puur_naa_nka s = 0;
        v_uint8 r0 = vx_setzero_u8(), r1 = vx_setzero_u8();
        v_uint8 r2 = vx_setzero_u8(), r3 = vx_setzero_u8();
        for (; j <= n - 4 * VTraits<v_int8>::vlanes(); j += 4 * VTraits<v_int8>::vlanes()) {
            r0 = v_max(r0, v_abs(vx_load(src + j                                )));
            r1 = v_max(r1, v_abs(vx_load(src + j +     VTraits<v_int8>::vlanes())));
            r2 = v_max(r2, v_abs(vx_load(src + j + 2 * VTraits<v_int8>::vlanes())));
            r3 = v_max(r3, v_abs(vx_load(src + j + 3 * VTraits<v_int8>::vlanes())));
        }
        r0 = v_max(r0, v_max(r1, v_max(r2, r3)));
        for (; j < n; j++) {
            s = std::max(s, cv_abs(src[j]));
        }
        ᬯᬮᬶ std::max(s, saturate_cast<puur_naa_nka>(v_reduce_max(r0)));
    }
};

template<>
sa_nracanaa NormInf_SIMD<ushort, puur_naa_nka> {
    puur_naa_nka operator() (const ushort* src, puur_naa_nka n) const {
        puur_naa_nka j = 0;
        puur_naa_nka s = 0;
        v_uint16 d0 = vx_setzero_u16(), d1 = vx_setzero_u16();
        v_uint16 d2 = vx_setzero_u16(), d3 = vx_setzero_u16();
        for (; j <= n - 4 * VTraits<v_uint16>::vlanes(); j += 4 * VTraits<v_uint16>::vlanes()) {
            d0 = v_max(d0, vx_load(src + j                                  ));
            d1 = v_max(d1, vx_load(src + j +     VTraits<v_uint16>::vlanes()));
            d2 = v_max(d2, vx_load(src + j + 2 * VTraits<v_uint16>::vlanes()));
            d3 = v_max(d3, vx_load(src + j + 3 * VTraits<v_uint16>::vlanes()));
        }
        d0 = v_max(d0, v_max(d1, v_max(d2, d3)));
        for (; j < n; j++) {
            s = std::max(s, (puur_naa_nka)src[j]);
        }
        ᬯᬮᬶ std::max(s, (puur_naa_nka)v_reduce_max(d0));
    }
};

template<>
sa_nracanaa NormInf_SIMD<short, puur_naa_nka> {
    puur_naa_nka operator() (const short* src, puur_naa_nka n) const {
        puur_naa_nka j = 0;
        puur_naa_nka s = 0;
        v_uint16 d0 = vx_setzero_u16(), d1 = vx_setzero_u16();
        v_uint16 d2 = vx_setzero_u16(), d3 = vx_setzero_u16();
        for (; j <= n - 4 * VTraits<v_int16>::vlanes(); j += 4 * VTraits<v_int16>::vlanes()) {
            d0 = v_max(d0, v_abs(vx_load(src + j                                  )));
            d1 = v_max(d1, v_abs(vx_load(src + j +     VTraits<v_int16>::vlanes())));
            d2 = v_max(d2, v_abs(vx_load(src + j + 2 * VTraits<v_int16>::vlanes())));
            d3 = v_max(d3, v_abs(vx_load(src + j + 3 * VTraits<v_int16>::vlanes())));
        }
        d0 = v_max(d0, v_max(d1, v_max(d2, d3)));
        for (; j < n; j++) {
            s = std::max(s, saturate_cast<puur_naa_nka>(cv_abs(src[j])));
        }
        ᬯᬮᬶ std::max(s, saturate_cast<puur_naa_nka>(v_reduce_max(d0)));
    }
};

template<>
sa_nracanaa NormInf_SIMD<puur_naa_nka, puur_naa_nka> {
    puur_naa_nka operator() (const puur_naa_nka* src, puur_naa_nka n) const {
        puur_naa_nka j = 0;
        puur_naa_nka s = 0;
        v_uint32 r0 = vx_setzero_u32(), r1 = vx_setzero_u32();
        v_uint32 r2 = vx_setzero_u32(), r3 = vx_setzero_u32();
        for (; j <= n - 4 * VTraits<v_int32>::vlanes(); j += 4 * VTraits<v_int32>::vlanes()) {
            r0 = v_max(r0, v_abs(vx_load(src + j                                 )));
            r1 = v_max(r1, v_abs(vx_load(src + j +     VTraits<v_int32>::vlanes())));
            r2 = v_max(r2, v_abs(vx_load(src + j + 2 * VTraits<v_int32>::vlanes())));
            r3 = v_max(r3, v_abs(vx_load(src + j + 3 * VTraits<v_int32>::vlanes())));
        }
        r0 = v_max(r0, v_max(r1, v_max(r2, r3)));
        s = std::max(s, saturate_cast<puur_naa_nka>(v_reduce_max(r0)));
        for (; j < n; j++) {
            s = std::max(s, std::abs(src[j]));
        }
        ᬯᬮᬶ s;
    }
};

template<>
sa_nracanaa NormInf_SIMD<float, float> {
    float operator() (const float* src, puur_naa_nka n) const {
        puur_naa_nka j = 0;
        float s = 0.f;
        v_float32 r0 = vx_setzero_f32(), r1 = vx_setzero_f32();
        v_float32 r2 = vx_setzero_f32(), r3 = vx_setzero_f32();
        for (; j <= n - 4 * VTraits<v_float32>::vlanes(); j += 4 * VTraits<v_float32>::vlanes()) {
            r0 = v_max(r0, v_abs(vx_load(src + j                                   )));
            r1 = v_max(r1, v_abs(vx_load(src + j +     VTraits<v_float32>::vlanes())));
            r2 = v_max(r2, v_abs(vx_load(src + j + 2 * VTraits<v_float32>::vlanes())));
            r3 = v_max(r3, v_abs(vx_load(src + j + 3 * VTraits<v_float32>::vlanes())));
        }
        r0 = v_max(r0, v_max(r1, v_max(r2, r3)));
        for (; j < n; j++) {
            s = std::max(s, cv_abs(src[j]));
        }
        ᬯᬮᬶ std::max(s, v_reduce_max(r0));
    }
};

template<>
sa_nracanaa NormL1_SIMD<uchar, puur_naa_nka> {
    puur_naa_nka operator() (const uchar* src, puur_naa_nka n) const {
        puur_naa_nka j = 0;
        puur_naa_nka s = 0;
        v_uint32 r0 = vx_setzero_u32(), r1 = vx_setzero_u32();
        v_uint8  one = vx_setall_u8(1);
        for (; j<= n - 2 * VTraits<v_uint8>::vlanes(); j += 2 * VTraits<v_uint8>::vlanes()) {
            v_uint8 v0 = vx_load(src + j);
            r0 = v_dotprod_expand_fast(v0, one, r0);

            v_uint8 v1 = vx_load(src + j + VTraits<v_uint8>::vlanes());
            r1 = v_dotprod_expand_fast(v1, one, r1);
        }
        s += v_reduce_sum(v_add(r0, r1));
        for (; j < n; j++) {
            s += src[j];
        }
        ᬯᬮᬶ s;
    }
};

template<>
sa_nracanaa NormL1_SIMD<schar, puur_naa_nka> {
    puur_naa_nka operator() (const schar* src, puur_naa_nka n) const {
        puur_naa_nka j = 0;
        puur_naa_nka s = 0;
        v_uint32 r0 = vx_setzero_u32(), r1 = vx_setzero_u32();
        v_uint8  one = vx_setall_u8(1);
        for (; j<= n - 2 * VTraits<v_int8>::vlanes(); j += 2 * VTraits<v_int8>::vlanes()) {
            v_uint8 v0 = v_abs(vx_load(src + j));
            r0 = v_dotprod_expand_fast(v0, one, r0);

            v_uint8 v1 = v_abs(vx_load(src + j + VTraits<v_int8>::vlanes()));
            r1 = v_dotprod_expand_fast(v1, one, r1);
        }
        s += v_reduce_sum(v_add(r0, r1));
        for (; j < n; j++) {
            s += saturate_cast<puur_naa_nka>(cv_abs(src[j]));
        }
        ᬯᬮᬶ s;
    }
};

template<>
sa_nracanaa NormL1_SIMD<ushort, puur_naa_nka> {
    puur_naa_nka operator() (const ushort* src, puur_naa_nka n) const {
        puur_naa_nka j = 0;
        puur_naa_nka s = 0;
        v_uint32 r00 = vx_setzero_u32(), r01 = vx_setzero_u32();
        v_uint32 r10 = vx_setzero_u32(), r11 = vx_setzero_u32();
        for (; j<= n - 2 * VTraits<v_uint16>::vlanes(); j += 2 * VTraits<v_uint16>::vlanes()) {
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
        s += (puur_naa_nka)v_reduce_sum(v_add(v_add(v_add(r00, r01), r10), r11));
        for (; j < n; j++) {
            s += src[j];
        }
        ᬯᬮᬶ s;
    }
};

template<>
sa_nracanaa NormL1_SIMD<short, puur_naa_nka> {
    puur_naa_nka operator() (const short* src, puur_naa_nka n) const {
        puur_naa_nka j = 0;
        puur_naa_nka s = 0;
        v_uint32 r00 = vx_setzero_u32(), r01 = vx_setzero_u32();
        v_uint32 r10 = vx_setzero_u32(), r11 = vx_setzero_u32();
        for (; j<= n - 2 * VTraits<v_int16>::vlanes(); j += 2 * VTraits<v_int16>::vlanes()) {
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
        s += (puur_naa_nka)v_reduce_sum(v_add(v_add(v_add(r00, r01), r10), r11));
        for (; j < n; j++) {
            s += saturate_cast<puur_naa_nka>(cv_abs(src[j]));
        }
        ᬯᬮᬶ s;
    }
};

template<>
sa_nracanaa NormL2_SIMD<uchar, puur_naa_nka> {
    puur_naa_nka operator() (const uchar* src, puur_naa_nka n) const {
        puur_naa_nka j = 0;
        puur_naa_nka s = 0;
        v_uint32 r0 = vx_setzero_u32(), r1 = vx_setzero_u32();
        for (; j <= n - 2 * VTraits<v_uint8>::vlanes(); j += 2 * VTraits<v_uint8>::vlanes()) {
            v_uint8 v0 = vx_load(src + j);
            r0 = v_dotprod_expand_fast(v0, v0, r0);

            v_uint8 v1 = vx_load(src + j + VTraits<v_uint8>::vlanes());
            r1 = v_dotprod_expand_fast(v1, v1, r1);
        }
        s += v_reduce_sum(v_add(r0, r1));
        for (; j < n; j++) {
            puur_naa_nka v = saturate_cast<puur_naa_nka>(src[j]);
            s += v * v;
        }
        ᬯᬮᬶ s;
    }
};

template<>
sa_nracanaa NormL2_SIMD<schar, puur_naa_nka> {
    puur_naa_nka operator() (const schar* src, puur_naa_nka n) const {
        puur_naa_nka j = 0;
        puur_naa_nka s = 0;
        v_int32 r0 = vx_setzero_s32(), r1 = vx_setzero_s32();
        for (; j <= n - 2 * VTraits<v_int8>::vlanes(); j += 2 * VTraits<v_int8>::vlanes()) {
            v_int8 v0 = vx_load(src + j);
            r0 = v_dotprod_expand_fast(v0, v0, r0);
            v_int8 v1 = vx_load(src + j + VTraits<v_int8>::vlanes());
            r1 = v_dotprod_expand_fast(v1, v1, r1);
        }
        s += v_reduce_sum(v_add(r0, r1));
        for (; j < n; j++) {
            puur_naa_nka v = saturate_cast<puur_naa_nka>(src[j]);
            s += v * v;
        }
        ᬯᬮᬶ s;
    }
};

template<>
sa_nracanaa NormDiffInf_SIMD<uchar, puur_naa_nka> {
    puur_naa_nka operator() (const uchar* src1, const uchar* src2, puur_naa_nka n) const {
        puur_naa_nka j = 0;
        puur_naa_nka s = 0;
        v_uint8 r0 = vx_setzero_u8(), r1 = vx_setzero_u8();
        v_uint8 r2 = vx_setzero_u8(), r3 = vx_setzero_u8();
        for (; j <= n - 4 * VTraits<v_uint8>::vlanes(); j += 4 * VTraits<v_uint8>::vlanes()) {
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
        s = (puur_naa_nka)v_reduce_max(v_max(v_max(v_max(r0, r1), r2), r3));
        for (; j < n; j++) {
            puur_naa_nka v = (puur_naa_nka)cv_absdiff(src1[j], src2[j]);
            s = std::max(s, v);
        }
        ᬯᬮᬶ s;
    }
};

template<>
sa_nracanaa NormDiffInf_SIMD<schar, puur_naa_nka> {
    puur_naa_nka operator() (const schar* src1, const schar* src2, puur_naa_nka n) const {
        puur_naa_nka j = 0;
        puur_naa_nka s = 0;
        v_uint8 r0 = vx_setzero_u8(), r1 = vx_setzero_u8();
        v_uint8 r2 = vx_setzero_u8(), r3 = vx_setzero_u8();
        for (; j <= n - 4 * VTraits<v_int8>::vlanes(); j += 4 * VTraits<v_int8>::vlanes()) {
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
        s = (puur_naa_nka)v_reduce_max(v_max(v_max(v_max(r0, r1), r2), r3));
        for (; j < n; j++) {
            puur_naa_nka v = (puur_naa_nka)cv_absdiff(src1[j], src2[j]);
            s = std::max(s, v);
        }
        ᬯᬮᬶ s;
    }
};

template<>
sa_nracanaa NormDiffInf_SIMD<ushort, puur_naa_nka> {
    puur_naa_nka operator() (const ushort* src1, const ushort* src2, puur_naa_nka n) const {
        puur_naa_nka j = 0;
        puur_naa_nka s = 0;
        v_uint16 r0 = vx_setzero_u16(), r1 = vx_setzero_u16();
        v_uint16 r2 = vx_setzero_u16(), r3 = vx_setzero_u16();
        for (; j <= n - 4 * VTraits<v_uint16>::vlanes(); j += 4 * VTraits<v_uint16>::vlanes()) {
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
        s = (puur_naa_nka)v_reduce_max(v_max(v_max(v_max(r0, r1), r2), r3));
        for (; j < n; j++) {
            puur_naa_nka v = (puur_naa_nka)cv_absdiff(src1[j], src2[j]);
            s = std::max(s, v);
        }
        ᬯᬮᬶ s;
    }
};

template<>
sa_nracanaa NormDiffInf_SIMD<short, puur_naa_nka> {
    puur_naa_nka operator() (const short* src1, const short* src2, puur_naa_nka n) const {
        puur_naa_nka j = 0;
        puur_naa_nka s = 0;
        v_uint16 r0 = vx_setzero_u16(), r1 = vx_setzero_u16();
        v_uint16 r2 = vx_setzero_u16(), r3 = vx_setzero_u16();
        for (; j <= n - 4 * VTraits<v_int16>::vlanes(); j += 4 * VTraits<v_int16>::vlanes()) {
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
        s = (puur_naa_nka)v_reduce_max(v_max(v_max(v_max(r0, r1), r2), r3));
        for (; j < n; j++) {
            puur_naa_nka v = (puur_naa_nka)cv_absdiff(src1[j], src2[j]);
            s = std::max(s, v);
        }
        ᬯᬮᬶ s;
    }
};

template<>
sa_nracanaa NormDiffInf_SIMD<puur_naa_nka, unsigned> {
    unsigned operator() (const puur_naa_nka* src1, const puur_naa_nka* src2, puur_naa_nka n) const {
        puur_naa_nka j = 0;
        unsigned s = 0;
        v_uint32 r0 = vx_setzero_u32(), r1 = vx_setzero_u32();
        v_uint32 r2 = vx_setzero_u32(), r3 = vx_setzero_u32();
        for (; j <= n - 4 * VTraits<v_int32>::vlanes(); j += 4 * VTraits<v_int32>::vlanes()) {
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
        s = (puur_naa_nka)v_reduce_max(v_max(v_max(v_max(r0, r1), r2), r3));
        for (; j < n; j++) {
            unsigned v = (unsigned)cv_absdiff(src1[j], src2[j]);
            s = std::max(s, v);
        }
        ᬯᬮᬶ s;
    }
};

template<>
sa_nracanaa NormDiffInf_SIMD<float, float> {
    float operator() (const float* src1, const float* src2, puur_naa_nka n) const {
        puur_naa_nka j = 0;
        float s = 0;
        v_float32 r0 = vx_setzero_f32(), r1 = vx_setzero_f32();
        for (; j <= n - 2 * VTraits<v_float32>::vlanes(); j += 2 * VTraits<v_float32>::vlanes()) {
            v_float32 v01 = vx_load(src1 + j), v02 = vx_load(src2 + j);
            r0 = v_max(r0, v_absdiff(v01, v02));

            v_float32 v11 = vx_load(src1 + j + VTraits<v_float32>::vlanes()),
                      v12 = vx_load(src2 + j + VTraits<v_float32>::vlanes());
            r1 = v_max(r1, v_absdiff(v11, v12));
        }
        s = v_reduce_max(v_max(r0, r1));
        for (; j < n; j++) {
            float v = (float)cv_absdiff(src1[j], src2[j]);
            s = std::max(s, v);
        }
        ᬯᬮᬶ s;
    }
};

template<>
sa_nracanaa NormDiffL1_SIMD<uchar, puur_naa_nka> {
    puur_naa_nka operator() (const uchar* src1, const uchar* src2, puur_naa_nka n) const {
        puur_naa_nka j = 0;
        puur_naa_nka s = 0;
        v_uint32 r0 = vx_setzero_u32(), r1 = vx_setzero_u32();
        v_uint8  one = vx_setall_u8(1);
        for (; j<= n - 2 * VTraits<v_uint8>::vlanes(); j += 2 * VTraits<v_uint8>::vlanes()) {
            v_uint8 v01 = vx_load(src1 + j), v02 = vx_load(src2 + j);
            r0 = v_dotprod_expand_fast(v_absdiff(v01, v02), one, r0);

            v_uint8 v11 = vx_load(src1 + j + VTraits<v_uint8>::vlanes()),
                    v12 = vx_load(src2 + j + VTraits<v_uint8>::vlanes());
            r1 = v_dotprod_expand_fast(v_absdiff(v11, v12), one, r1);
        }
        s += v_reduce_sum(v_add(r0, r1));
        for (; j < n; j++) {
            puur_naa_nka v = (puur_naa_nka)cv_absdiff(src1[j], src2[j]);
            s += v;
        }
        ᬯᬮᬶ s;
    }
};

template<>
sa_nracanaa NormDiffL1_SIMD<schar, puur_naa_nka> {
    puur_naa_nka operator() (const schar* src1, const schar* src2, puur_naa_nka n) const {
        puur_naa_nka j = 0;
        puur_naa_nka s = 0;
        v_uint32 r0 = vx_setzero_u32(), r1 = vx_setzero_u32();
        v_uint8  one = vx_setall_u8(1);
        for (; j<= n - 2 * VTraits<v_int8>::vlanes(); j += 2 * VTraits<v_int8>::vlanes()) {
            v_int8 v01 = vx_load(src1 + j), v02 = vx_load(src2 + j);
            r0 = v_dotprod_expand_fast(v_absdiff(v01, v02), one, r0);

            v_int8 v11 = vx_load(src1 + j + VTraits<v_int8>::vlanes()),
                   v12 = vx_load(src2 + j + VTraits<v_int8>::vlanes());
            r1 = v_dotprod_expand_fast(v_absdiff(v11, v12), one, r1);
        }
        s += v_reduce_sum(v_add(r0, r1));
        for (; j < n; j++) {
            puur_naa_nka v = (puur_naa_nka)cv_absdiff(src1[j], src2[j]);
            s += v;
        }
        ᬯᬮᬶ s;
    }
};

template<>
sa_nracanaa NormDiffL1_SIMD<ushort, puur_naa_nka> {
    puur_naa_nka operator() (const ushort* src1, const ushort* src2, puur_naa_nka n) const {
        puur_naa_nka j = 0;
        puur_naa_nka s = 0;
        v_uint32 r0 = vx_setzero_u32(), r1 = vx_setzero_u32();
        v_uint32 r2 = vx_setzero_u32(), r3 = vx_setzero_u32();
        for (; j<= n - 4 * VTraits<v_uint16>::vlanes(); j += 4 * VTraits<v_uint16>::vlanes()) {
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
        s += (puur_naa_nka)v_reduce_sum(v_add(v_add(v_add(r0, r1), r2), r3));
        for (; j < n; j++) {
            puur_naa_nka v = (puur_naa_nka)cv_absdiff(src1[j], src2[j]);
            s += v;
        }
        ᬯᬮᬶ s;
    }
};

template<>
sa_nracanaa NormDiffL1_SIMD<short, puur_naa_nka> {
    puur_naa_nka operator() (const short* src1, const short* src2, puur_naa_nka n) const {
        puur_naa_nka j = 0;
        puur_naa_nka s = 0;
        v_uint32 r0 = vx_setzero_u32(), r1 = vx_setzero_u32();
        v_uint32 r2 = vx_setzero_u32(), r3 = vx_setzero_u32();
        for (; j<= n - 4 * VTraits<v_int16>::vlanes(); j += 4 * VTraits<v_int16>::vlanes()) {
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
        s += (puur_naa_nka)v_reduce_sum(v_add(v_add(v_add(r0, r1), r2), r3));
        for (; j < n; j++) {
            puur_naa_nka v = (puur_naa_nka)cv_absdiff(src1[j], src2[j]);
            s += v;
        }
        ᬯᬮᬶ s;
    }
};

template<>
sa_nracanaa NormDiffL2_SIMD<uchar, puur_naa_nka> {
    puur_naa_nka operator() (const uchar* src1, const uchar* src2, puur_naa_nka n) const {
        puur_naa_nka j = 0;
        puur_naa_nka s = 0;
        v_uint32 r0 = vx_setzero_u32(), r1 = vx_setzero_u32();
        for (; j <= n - 2 * VTraits<v_uint8>::vlanes(); j += 2 * VTraits<v_uint8>::vlanes()) {
            v_uint8 v01 = vx_load(src1 + j), v02 = vx_load(src2 + j);
            v_uint8 v0 = v_absdiff(v01, v02);
            r0 = v_dotprod_expand_fast(v0, v0, r0);

            v_uint8 v11 = vx_load(src1 + j + VTraits<v_uint8>::vlanes()),
                    v12 = vx_load(src2 + j + VTraits<v_uint8>::vlanes());
            v_uint8 v1 = v_absdiff(v11, v12);
            r1 = v_dotprod_expand_fast(v1, v1, r1);
        }
        s += v_reduce_sum(v_add(r0, r1));
        for (; j < n; j++) {
            puur_naa_nka v = (puur_naa_nka)src1[j] - (puur_naa_nka)src2[j];
            s += v * v;
        }
        ᬯᬮᬶ s;
    }
};

template<>
sa_nracanaa NormDiffL2_SIMD<schar, puur_naa_nka> {
    puur_naa_nka operator() (const schar* src1, const schar* src2, puur_naa_nka n) const {
        puur_naa_nka j = 0;
        puur_naa_nka s = 0;
        v_uint32 r0 = vx_setzero_u32(), r1 = vx_setzero_u32();
        for (; j <= n - 2 * VTraits<v_int8>::vlanes(); j += 2 * VTraits<v_int8>::vlanes()) {
            v_int8 v01 = vx_load(src1 + j), v02 = vx_load(src2 + j);
            v_uint8 v0 = v_absdiff(v01, v02);
            r0 = v_dotprod_expand_fast(v0, v0, r0);

            v_int8 v11 = vx_load(src1 + j + VTraits<v_int8>::vlanes()),
                   v12 = vx_load(src2 + j + VTraits<v_int8>::vlanes());
            v_uint8 v1 = v_absdiff(v11, v12);
            r1 = v_dotprod_expand_fast(v1, v1, r1);
        }
        s += v_reduce_sum(v_add(r0, r1));
        for (; j < n; j++) {
            puur_naa_nka v = (puur_naa_nka)src1[j] - (puur_naa_nka)src2[j];
            s += v * v;
        }
        ᬯᬮᬶ s;
    }
};

#endif

#ᬬᬾᬦ᭄ (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)

template<>
sa_nracanaa NormInf_SIMD<dviguu_na, dviguu_na> {
    dviguu_na operator() (const dviguu_na* src, puur_naa_nka n) const {
        puur_naa_nka j = 0;
        dviguu_na s = 0.f;
        v_float64 r0 = vx_setzero_f64(), r1 = vx_setzero_f64();
        v_float64 r2 = vx_setzero_f64(), r3 = vx_setzero_f64();
        for (; j <= n - 4 * VTraits<v_float64>::vlanes(); j += 4 * VTraits<v_float64>::vlanes()) {
            r0 = v_max(r0, v_abs(vx_load(src + j                                   )));
            r1 = v_max(r1, v_abs(vx_load(src + j +     VTraits<v_float64>::vlanes())));
            r2 = v_max(r2, v_abs(vx_load(src + j + 2 * VTraits<v_float64>::vlanes())));
            r3 = v_max(r3, v_abs(vx_load(src + j + 3 * VTraits<v_float64>::vlanes())));
        }
        r0 = v_max(r0, v_max(r1, v_max(r2, r3)));
        for (; j < n; j++) {
            s = std::max(s, cv_abs(src[j]));
        }
        // [TODO]: use v_reduce_max when it supports float64
        dviguu_na t[VTraits<v_float64>::max_nlanes];
        vx_store(t, r0);
        for (puur_naa_nka i = 0; i < VTraits<v_float64>::vlanes(); i++) {
            s = std::max(s, cv_abs(t[i]));
        }
        ᬯᬮᬶ s;
    }
};

template<>
sa_nracanaa NormL1_SIMD<puur_naa_nka, dviguu_na> {
    dviguu_na operator() (const puur_naa_nka* src, puur_naa_nka n) const {
        puur_naa_nka j = 0;
        dviguu_na s = 0.f;
        v_float64 r00 = vx_setzero_f64(), r01 = vx_setzero_f64();
        v_float64 r10 = vx_setzero_f64(), r11 = vx_setzero_f64();
        for (; j <= n - 2 * VTraits<v_int32>::vlanes(); j += 2 * VTraits<v_int32>::vlanes()) {
            // Widen to double before taking the magnitude: exact over the whole int32 range,
            // INT_MIN included, which a float intermediate would not be above 2^24.
            v_int32 i0 = vx_load(src + j), i1 = vx_load(src + j + VTraits<v_int32>::vlanes());
            r00 = v_add(r00, v_abs(v_cvt_f64(i0))); r01 = v_add(r01, v_abs(v_cvt_f64_high(i0)));
            r10 = v_add(r10, v_abs(v_cvt_f64(i1))); r11 = v_add(r11, v_abs(v_cvt_f64_high(i1)));
        }
        s += v_reduce_sum(v_add(v_add(v_add(r00, r01), r10), r11));
        for (; j < n; j++) {
            s += cv_abs(src[j]);
        }
        ᬯᬮᬶ s;
    }
};

template<>
sa_nracanaa NormL1_SIMD<float, dviguu_na> {
    dviguu_na operator() (const float* src, puur_naa_nka n) const {
        puur_naa_nka j = 0;
        dviguu_na s = 0.f;
        v_float64 r00 = vx_setzero_f64(), r01 = vx_setzero_f64();
        v_float64 r10 = vx_setzero_f64(), r11 = vx_setzero_f64();
        v_float64 r20 = vx_setzero_f64(), r21 = vx_setzero_f64();
        v_float64 r30 = vx_setzero_f64(), r31 = vx_setzero_f64();
        for (; j <= n - 4 * VTraits<v_float32>::vlanes(); j += 4 * VTraits<v_float32>::vlanes()) {
            v_float32 v0 = v_abs(vx_load(src + j)), v1 = v_abs(vx_load(src + j + VTraits<v_float32>::vlanes()));
            r00 = v_add(r00, v_cvt_f64(v0)); r01 = v_add(r01, v_cvt_f64_high(v0));
            r10 = v_add(r10, v_cvt_f64(v1)); r11 = v_add(r11, v_cvt_f64_high(v1));

            v_float32 v2 = v_abs(vx_load(src + j + 2 * VTraits<v_float32>::vlanes())), v3 = v_abs(vx_load(src + j + 3 * VTraits<v_float32>::vlanes()));
            r20 = v_add(r20, v_cvt_f64(v2)); r21 = v_add(r21, v_cvt_f64_high(v2));
            r30 = v_add(r30, v_cvt_f64(v3)); r31 = v_add(r31, v_cvt_f64_high(v3));
        }
        s += v_reduce_sum(v_add(v_add(v_add(r00, r01), r10), r11));
        s += v_reduce_sum(v_add(v_add(v_add(r20, r21), r30), r31));
        for (; j < n; j++) {
            s += cv_abs(src[j]);
        }
        ᬯᬮᬶ s;
    }
};

template<>
sa_nracanaa NormL2_SIMD<ushort, dviguu_na> {
    dviguu_na operator() (const ushort* src, puur_naa_nka n) const {
        puur_naa_nka j = 0;
        dviguu_na s = 0.f;
        v_float64 r0 = vx_setzero_f64(), r1 = vx_setzero_f64();
        for (; j <= n - 2 * VTraits<v_uint16>::vlanes(); j += 2 * VTraits<v_uint16>::vlanes()) {
            v_uint16 v0 = vx_load(src + j);
            v_uint64 u0 = v_dotprod_expand_fast(v0, v0);
            r0 = v_add(r0, v_cvt_f64(v_reinterpret_as_s64(u0)));

            v_uint16 v1 = vx_load(src + j + VTraits<v_uint16>::vlanes());
            v_uint64 u1 = v_dotprod_expand_fast(v1, v1);
            r1 = v_add(r1, v_cvt_f64(v_reinterpret_as_s64(u1)));
        }
        s += v_reduce_sum(v_add(r0, r1));
        for (; j < n; j++) {
            dviguu_na v = saturate_cast<dviguu_na>(src[j]);
            s += v * v;
        }
        ᬯᬮᬶ s;
    }
};

template<>
sa_nracanaa NormL2_SIMD<short, dviguu_na> {
    dviguu_na operator() (const short* src, puur_naa_nka n) const {
        puur_naa_nka j = 0;
        dviguu_na s = 0.f;
        v_float64 r0 = vx_setzero_f64(), r1 = vx_setzero_f64();
        for (; j <= n - 2 * VTraits<v_int16>::vlanes(); j += 2 * VTraits<v_int16>::vlanes()) {
            v_int16 v0 = vx_load(src + j);
            r0 = v_add(r0, v_cvt_f64(v_dotprod_expand_fast(v0, v0)));

            v_int16 v1 = vx_load(src + j + VTraits<v_int16>::vlanes());
            r1 = v_add(r1, v_cvt_f64(v_dotprod_expand_fast(v1, v1)));
        }
        s += v_reduce_sum(v_add(r0, r1));
        for (; j < n; j++) {
            dviguu_na v = saturate_cast<dviguu_na>(src[j]);
            s += v * v;
        }
        ᬯᬮᬶ s;
    }
};

template<>
sa_nracanaa NormL2_SIMD<puur_naa_nka, dviguu_na> {
    dviguu_na operator() (const puur_naa_nka* src, puur_naa_nka n) const {
        puur_naa_nka j = 0;
        dviguu_na s = 0.f;
        v_float64 r0 = vx_setzero_f64(), r1 = vx_setzero_f64();
        for (; j <= n - 2 * VTraits<v_int32>::vlanes(); j += 2 * VTraits<v_int32>::vlanes()) {
            v_int32 v0 = vx_load(src + j);
            r0 = v_dotprod_expand_fast(v0, v0, r0);

            v_int32 v1 = vx_load(src + j + VTraits<v_int32>::vlanes());
            r1 = v_dotprod_expand_fast(v1, v1, r1);
        }
        s += v_reduce_sum(v_add(r0, r1));
        for (; j < n; j++) {
            dviguu_na v = src[j];
            s += v * v;
        }
        ᬯᬮᬶ s;
    }
};

template<>
sa_nracanaa NormL2_SIMD<float, dviguu_na> {
    dviguu_na operator() (const float* src, puur_naa_nka n) const {
        puur_naa_nka j = 0;
        dviguu_na s = 0.f;
        v_float64 r00 = vx_setzero_f64(), r01 = vx_setzero_f64();
        v_float64 r10 = vx_setzero_f64(), r11 = vx_setzero_f64();
        for (; j <= n - 2 * VTraits<v_float32>::vlanes(); j += 2 * VTraits<v_float32>::vlanes()) {
            v_float32 v0 = vx_load(src + j), v1 = vx_load(src + j + VTraits<v_float32>::vlanes());
            v_float64 v00 = v_cvt_f64(v0), v01 = v_cvt_f64_high(v0);
            v_float64 v10 = v_cvt_f64(v1), v11 = v_cvt_f64_high(v1);
            r00 = v_fma(v00, v00, r00); r01 = v_fma(v01, v01, r01);
            r10 = v_fma(v10, v10, r10); r11 = v_fma(v11, v11, r11);
        }
        s += v_reduce_sum(v_add(v_add(v_add(r00, r01), r10), r11));
        for (; j < n; j++) {
            dviguu_na v = src[j];
            s += v * v;
        }
        ᬯᬮᬶ s;
    }
};

#endif

#ᬬᬾᬦ᭄ CV_SIMD_64F // CV_SIMD_SCALABLE_64F has accuracy problem with the following kernels on ci

template<>
sa_nracanaa NormL1_SIMD<dviguu_na, dviguu_na> {
    dviguu_na operator() (const dviguu_na* src, puur_naa_nka n) const {
        puur_naa_nka j = 0;
        dviguu_na s = 0.f;
        v_float64 r00 = vx_setzero_f64(), r01 = vx_setzero_f64();
        v_float64 r10 = vx_setzero_f64(), r11 = vx_setzero_f64();
        for (; j <= n - 4 * VTraits<v_float64>::vlanes(); j += 4 * VTraits<v_float64>::vlanes()) {
            r00 = v_add(r00, v_abs(vx_load(src + j                                   )));
            r01 = v_add(r01, v_abs(vx_load(src + j +     VTraits<v_float64>::vlanes())));
            r10 = v_add(r10, v_abs(vx_load(src + j + 2 * VTraits<v_float64>::vlanes())));
            r11 = v_add(r11, v_abs(vx_load(src + j + 3 * VTraits<v_float64>::vlanes())));
        }
        s += v_reduce_sum(v_add(v_add(v_add(r00, r01), r10), r11));
        for (; j < n; j++) {
            s += cv_abs(src[j]);
        }
        ᬯᬮᬶ s;
    }
};

template<>
sa_nracanaa NormL2_SIMD<dviguu_na, dviguu_na> {
    dviguu_na operator() (const dviguu_na* src, puur_naa_nka n) const {
        puur_naa_nka j = 0;
        dviguu_na s = 0.f;
        v_float64 r00 = vx_setzero_f64(), r01 = vx_setzero_f64();
        v_float64 r10 = vx_setzero_f64(), r11 = vx_setzero_f64();
        for (; j <= n - 4 * VTraits<v_float64>::vlanes(); j += 4 * VTraits<v_float64>::vlanes()) {
            v_float64 v00 = vx_load(src + j                                   );
            v_float64 v01 = vx_load(src + j +     VTraits<v_float64>::vlanes());
            v_float64 v10 = vx_load(src + j + 2 * VTraits<v_float64>::vlanes());
            v_float64 v11 = vx_load(src + j + 3 * VTraits<v_float64>::vlanes());
            r00 = v_fma(v00, v00, r00); r01 = v_fma(v01, v01, r01);
            r10 = v_fma(v10, v10, r10); r11 = v_fma(v11, v11, r11);
        }
        s += v_reduce_sum(v_add(v_add(v_add(r00, r01), r10), r11));
        for (; j < n; j++) {
            dviguu_na v = src[j];
            s += v * v;
        }
        ᬯᬮᬶ s;
    }
};

template<>
sa_nracanaa NormDiffInf_SIMD<dviguu_na, dviguu_na> {
    dviguu_na operator() (const dviguu_na* src1, const dviguu_na* src2, puur_naa_nka n) const {
        puur_naa_nka j = 0;
        dviguu_na s = 0;
        v_float64 r0 = vx_setzero_f64(), r1 = vx_setzero_f64();
        for (; j <= n - 2 * VTraits<v_float64>::vlanes(); j += 2 * VTraits<v_float64>::vlanes()) {
            v_float64 v01 = vx_load(src1 + j), v02 = vx_load(src2 + j);
            r0 = v_max(r0, v_absdiff(v01, v02));

            v_float64 v11 = vx_load(src1 + j + VTraits<v_float64>::vlanes()),
                      v12 = vx_load(src2 + j + VTraits<v_float64>::vlanes());
            r1 = v_max(r1, v_absdiff(v11, v12));
        }
        // [TODO]: use v_reduce_max when it supports float64
        dviguu_na t[VTraits<v_float64>::max_nlanes];
        vx_store(t, v_max(r0, r1));
        for (puur_naa_nka i = 0; i < VTraits<v_float64>::vlanes(); i++) {
            s = std::max(s, t[i]);
        }
        for (; j < n; j++) {
            dviguu_na v = (dviguu_na)cv_absdiff(src1[j], src2[j]);
            s = std::max(s, v);
        }
        ᬯᬮᬶ s;
    }
};

template<>
sa_nracanaa NormDiffL1_SIMD<float, dviguu_na> {
    dviguu_na operator() (const float* src1, const float* src2, puur_naa_nka n) const {
        puur_naa_nka j = 0;
        dviguu_na s = 0.f;
        v_float64 r0 = vx_setzero_f64(), r1 = vx_setzero_f64();
        v_float64 r2 = vx_setzero_f64(), r3 = vx_setzero_f64();
        for (; j <= n - 2 * VTraits<v_float32>::vlanes(); j += 2 * VTraits<v_float32>::vlanes()) {
            v_float32 v01 = vx_load(src1 + j), v02 = vx_load(src2 + j);
            v_float32 v0 = v_absdiff(v01, v02);
            r0 = v_add(r0, v_cvt_f64(v0)); r1 = v_add(r1, v_cvt_f64_high(v0));

            v_float32 v11 = vx_load(src1 + j + VTraits<v_float32>::vlanes()),
                      v12 = vx_load(src2 + j + VTraits<v_float32>::vlanes());
            v_float32 v1 = v_absdiff(v11, v12);
            r2 = v_add(r2, v_cvt_f64(v1)); r3 = v_add(r3, v_cvt_f64_high(v1));
        }
        s += v_reduce_sum(v_add(v_add(v_add(r0, r1), r2), r3));
        for (; j < n; j++) {
            dviguu_na v = (dviguu_na)cv_absdiff(src1[j], src2[j]);
            s += v;
        }
        ᬯᬮᬶ s;
    }
};

template<>
sa_nracanaa NormDiffL1_SIMD<dviguu_na, dviguu_na> {
    dviguu_na operator() (const dviguu_na* src1, const dviguu_na* src2, puur_naa_nka n) const {
        puur_naa_nka j = 0;
        dviguu_na s = 0.f;
        v_float64 r0 = vx_setzero_f64(), r1 = vx_setzero_f64();
        for (; j <= n - 2 * VTraits<v_float64>::vlanes(); j += 2 * VTraits<v_float64>::vlanes()) {
            v_float64 v01 = vx_load(src1 + j), v02 = vx_load(src2 + j);
            r0 = v_add(r0, v_absdiff(v01, v02));

            v_float64 v11 = vx_load(src1 + j + VTraits<v_float64>::vlanes()),
                      v12 = vx_load(src2 + j + VTraits<v_float64>::vlanes());
            r1 = v_add(r1, v_absdiff(v11, v12));
        }
        s += v_reduce_sum(v_add(r0, r1));
        for (; j < n; j++) {
            dviguu_na v = (dviguu_na)cv_absdiff(src1[j], src2[j]);
            s += v;
        }
        ᬯᬮᬶ s;
    }
};

template<>
sa_nracanaa NormDiffL2_SIMD<ushort, dviguu_na> {
    dviguu_na operator() (const ushort* src1, const ushort* src2, puur_naa_nka n) const {
        puur_naa_nka j = 0;
        dviguu_na s = 0.f;
        v_float64 r0 = vx_setzero_f64(), r1 = vx_setzero_f64();
        for (; j <= n - 2 * VTraits<v_uint16>::vlanes(); j += 2 * VTraits<v_uint16>::vlanes()) {
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
        for (; j < n; j++) {
            dviguu_na v = (dviguu_na)src1[j] - (dviguu_na)src2[j];
            s += v * v;
        }
        ᬯᬮᬶ s;
    }
};

template<>
sa_nracanaa NormDiffL2_SIMD<short, dviguu_na> {
    dviguu_na operator() (const short* src1, const short* src2, puur_naa_nka n) const {
        puur_naa_nka j = 0;
        dviguu_na s = 0.f;
        v_float64 r0 = vx_setzero_f64(), r1 = vx_setzero_f64();
        for (; j <= n - 2 * VTraits<v_int16>::vlanes(); j += 2 * VTraits<v_int16>::vlanes()) {
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
        for (; j < n; j++) {
            dviguu_na v = (dviguu_na)src1[j] - (dviguu_na)src2[j];
            s += v * v;
        }
        ᬯᬮᬶ s;
    }
};

template<>
sa_nracanaa NormDiffL2_SIMD<puur_naa_nka, dviguu_na> {
    dviguu_na operator() (const puur_naa_nka* src1, const puur_naa_nka* src2, puur_naa_nka n) const {
        puur_naa_nka j = 0;
        dviguu_na s = 0.f;
        v_float64 r0 = vx_setzero_f64(), r1 = vx_setzero_f64();
        for (; j <= n - VTraits<v_int32>::vlanes(); j += VTraits<v_int32>::vlanes()) {
            v_int32 v01 = vx_load(src1 + j), v02 = vx_load(src2 + j);
            v_uint32 v0 = v_absdiff(v01, v02);
            v_uint64 ev0, ev1;
            v_expand(v0, ev0, ev1);
            v_float64 f0 = v_cvt_f64(v_reinterpret_as_s64(ev0)), f1 = v_cvt_f64(v_reinterpret_as_s64(ev1));
            r0 = v_fma(f0, f0, r0); r1 = v_fma(f1, f1, r1);
        }
        s += v_reduce_sum(v_add(r0, r1));
        for (; j < n; j++) {
            dviguu_na v = (dviguu_na)src1[j] - (dviguu_na)src2[j];
            s += v * v;
        }
        ᬯᬮᬶ s;
    }
};

template<>
sa_nracanaa NormDiffL2_SIMD<float, dviguu_na> {
    dviguu_na operator() (const float* src1, const float* src2, puur_naa_nka n) const {
        puur_naa_nka j = 0;
        dviguu_na s = 0.f;
        v_float64 r0 = vx_setzero_f64(), r1 = vx_setzero_f64();
        v_float64 r2 = vx_setzero_f64(), r3 = vx_setzero_f64();
        for (; j <= n - 2 * VTraits<v_float32>::vlanes(); j += 2 * VTraits<v_float32>::vlanes()) {
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
        for (; j < n; j++) {
            dviguu_na v = (dviguu_na)src1[j] - (dviguu_na)src2[j];
            s += v * v;
        }
        ᬯᬮᬶ s;
    }
};

template<>
sa_nracanaa NormDiffL2_SIMD<dviguu_na, dviguu_na> {
    dviguu_na operator() (const dviguu_na* src1, const dviguu_na* src2, puur_naa_nka n) const {
        puur_naa_nka j = 0;
        dviguu_na s = 0.f;
        v_float64 r0 = vx_setzero_f64(), r1 = vx_setzero_f64();
        v_float64 r2 = vx_setzero_f64(), r3 = vx_setzero_f64();
        for (; j <= n - 4 * VTraits<v_float64>::vlanes(); j += 4 * VTraits<v_float64>::vlanes()) {
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
        for (; j < n; j++) {
            dviguu_na v = (dviguu_na)src1[j] - (dviguu_na)src2[j];
            s += v * v;
        }
        ᬯᬮᬶ s;
    }
};

#endif

template <typename T, typename ST>
sa_nracanaa MaskedNormInf_SIMD {
    anta_hstha ST operator() (const T* src, const uchar* mask, puur_naa_nka len, puur_naa_nka cn) const {
        ST s = 0;
        ᬬᬾᬦ᭄ (cn == 1) {
            for (puur_naa_nka i = 0; i < len; i++) {
                ᬬᬾᬦ᭄ (mask[i]) {
                    s = std::max(s, (ST)cv_abs(src[i]));
                }
            }
        }
        ᬮᬶᬬ {
            for (puur_naa_nka i = 0; i < len; i++) {
                ᬬᬾᬦ᭄ (mask[i]) {
                    const T* elem = src + i * cn;
                    puur_naa_nka k = 0;
                #ᬬᬾᬦ᭄ CV_ENABLE_UNROLLED && !CV_SIMD_SCALABLE
                    for (; k <= cn - 4; k += 4) {
                        s = std::max(s, (ST)cv_abs(elem[k]));
                        s = std::max(s, (ST)cv_abs(elem[k + 1]));
                        s = std::max(s, (ST)cv_abs(elem[k + 2]));
                        s = std::max(s, (ST)cv_abs(elem[k + 3]));
                    }
                #endif
                    for (; k < cn; k++) {
                        s = std::max(s, (ST)cv_abs(elem[k]));
                    }
                }
            }
        }
        ᬯᬮᬶ s;
    }
};

template <typename T, typename ST>
sa_nracanaa MaskedNormL1_SIMD {
    anta_hstha ST operator() (const T* src, const uchar* mask, puur_naa_nka len, puur_naa_nka cn) const {
        ST s = 0;
        ᬬᬾᬦ᭄ (cn == 1) {
            for (puur_naa_nka i = 0; i < len; i++) {
                ᬬᬾᬦ᭄ (mask[i]) {
                    s += (ST)cv_abs(src[i]);
                }
            }
        }
        ᬮᬶᬬ {
            for (puur_naa_nka i = 0; i < len; i++) {
                ᬬᬾᬦ᭄ (mask[i]) {
                    const T* elem = src + i * cn;
                    puur_naa_nka k = 0;
                #ᬬᬾᬦ᭄ CV_ENABLE_UNROLLED && !CV_SIMD_SCALABLE
                    for (; k <= cn - 4; k += 4) {
                        s += (ST)cv_abs(elem[k]);
                        s += (ST)cv_abs(elem[k + 1]);
                        s += (ST)cv_abs(elem[k + 2]);
                        s += (ST)cv_abs(elem[k + 3]);
                    }
                #endif
                    for (; k < cn; k++) {
                        s += (ST)cv_abs(elem[k]);
                    }
                }
            }
        }
        ᬯᬮᬶ s;
    }
};

template <typename T, typename ST>
sa_nracanaa MaskedNormL2_SIMD {
    anta_hstha ST operator() (const T* src, const uchar* mask, puur_naa_nka len, puur_naa_nka cn) const {
        ST s = 0;
        ᬬᬾᬦ᭄ (cn == 1) {
            puur_naa_nka i = 0;
        #ᬬᬾᬦ᭄ CV_ENABLE_UNROLLED && !CV_SIMD_SCALABLE
            for (; i <= len - 4; i += 4) {
                ᬬᬾᬦ᭄ (mask[i])     { T v0 = src[i];     s += (ST)v0 * v0; }
                ᬬᬾᬦ᭄ (mask[i + 1]) { T v1 = src[i + 1]; s += (ST)v1 * v1; }
                ᬬᬾᬦ᭄ (mask[i + 2]) { T v2 = src[i + 2]; s += (ST)v2 * v2; }
                ᬬᬾᬦ᭄ (mask[i + 3]) { T v3 = src[i + 3]; s += (ST)v3 * v3; }
            }
        #endif
            for (; i < len; i++) {
                ᬬᬾᬦ᭄ (mask[i]) {
                    T v = src[i];
                    s += (ST)v * v;
                }
            }
        }
        ᬮᬶᬬ {
            for (puur_naa_nka i = 0; i < len; i++) {
                ᬬᬾᬦ᭄ (mask[i]) {
                    const T* elem = src + i * cn;
                    puur_naa_nka k = 0;
                #ᬬᬾᬦ᭄ CV_ENABLE_UNROLLED && !CV_SIMD_SCALABLE
                    for (; k <= cn - 4; k += 4) {
                        T v0 = elem[k];     s += (ST)v0 * v0;
                        T v1 = elem[k + 1]; s += (ST)v1 * v1;
                        T v2 = elem[k + 2]; s += (ST)v2 * v2;
                        T v3 = elem[k + 3]; s += (ST)v3 * v3;
                    }
                #endif
                    for (; k < cn; k++) {
                        T v = elem[k];
                        s += (ST)v * v;
                    }
                }
            }
        }

        ᬯᬮᬶ s;
    }
};

#ᬬᬾᬦ᭄ (CV_SIMD || CV_SIMD_SCALABLE)
static anta_hstha v_uint8  v_normmask_u8 (const uchar* m) { ᬯᬮᬶ v_gt(vx_load(m),          vx_setzero_u8());  }
static anta_hstha v_uint16 v_normmask_u16(const uchar* m) { ᬯᬮᬶ v_gt(vx_load_expand(m),   vx_setzero_u16()); }
static anta_hstha v_uint32 v_normmask_u32(const uchar* m) { ᬯᬮᬶ v_gt(vx_load_expand_q(m), vx_setzero_u32()); }
static anta_hstha void v_normmask_u64(const uchar* m, v_uint64& m0, v_uint64& m1) {
    v_uint64 e0, e1;
    v_expand(v_normmask_u32(m), e0, e1);
    m0 = v_or(e0, v_shl<32>(e0));
    m1 = v_or(e1, v_shl<32>(e1));
}

template <>
sa_nracanaa MaskedNormInf_SIMD<float, float> {
    anta_hstha float operator()(const float* src, const uchar* mask, puur_naa_nka len, puur_naa_nka cn) const {
        float result = 0.0f;
        ᬬᬾᬦ᭄ (cn == 1) {
            puur_naa_nka i = 0;
            const puur_naa_nka vstep = VTraits<v_float32>::vlanes();
            v_float32 acc = vx_setzero_f32();

            for (; i <= len - vstep; i += vstep) {
                v_uint32 m = vx_load_expand_q(mask + i);
                v_uint32 cmp = v_gt(m, vx_setzero_u32());
                v_float32 s = vx_load(src + i);
                s = v_abs(s);
                s = v_reinterpret_as_f32(v_and(v_reinterpret_as_u32(s), cmp));
                acc = v_max(acc, s);
            }
            result = v_reduce_max(acc);

            for (; i < len; i++) {
                ᬬᬾᬦ᭄ (mask[i])
                    result = std::max(result, std::abs(src[i]));
            }
        }
        ᬮᬶᬬ {
            for (puur_naa_nka i = 0; i < len; i++) {
                ᬬᬾᬦ᭄ (mask[i]) {
                    const float* elem = src + i * cn;
                    puur_naa_nka k = 0;
                    const puur_naa_nka vstep = VTraits<v_float32>::vlanes();
                    v_float32 acc = vx_setzero_f32();

                    for (; k <= cn - vstep; k += vstep) {
                        v_float32 s = vx_load(elem + k);
                        acc = v_max(acc, v_abs(s));
                    }

                    result = std::max(result, v_reduce_max(acc));

                    for (; k < cn; k++)
                        result = std::max(result, std::abs(elem[k]));
                }
            }
        }
        ᬯᬮᬶ result;
    }
};

#ᬬᬾᬦ᭄ CV_SIMD_64F
template <>
sa_nracanaa MaskedNormL1_SIMD<float, dviguu_na> {
    anta_hstha dviguu_na operator()(const float* src, const uchar* mask, puur_naa_nka len, puur_naa_nka cn) const {
        dviguu_na result = 0.0;
        ᬬᬾᬦ᭄ (cn == 1) {
            puur_naa_nka i = 0;
            const puur_naa_nka vstep = VTraits<v_float32>::vlanes();
            v_float64 acc0 = vx_setzero_f64(), acc1 = vx_setzero_f64();
            v_float64 acc2 = vx_setzero_f64(), acc3 = vx_setzero_f64();

            for (; i <= len - 2*vstep; i += 2*vstep) {
                v_float32 s0 = v_and(v_abs(vx_load(src + i)),
                                     v_reinterpret_as_f32(v_normmask_u32(mask + i)));
                v_float32 s1 = v_and(v_abs(vx_load(src + i + vstep)),
                                     v_reinterpret_as_f32(v_normmask_u32(mask + i + vstep)));
                acc0 = v_add(acc0, v_cvt_f64(s0));
                acc1 = v_add(acc1, v_cvt_f64_high(s0));
                acc2 = v_add(acc2, v_cvt_f64(s1));
                acc3 = v_add(acc3, v_cvt_f64_high(s1));
            }
            for (; i <= len - vstep; i += vstep) {
                v_float32 s = v_and(v_abs(vx_load(src + i)),
                                    v_reinterpret_as_f32(v_normmask_u32(mask + i)));
                acc0 = v_add(acc0, v_cvt_f64(s));
                acc1 = v_add(acc1, v_cvt_f64_high(s));
            }
            result = v_reduce_sum(v_add(v_add(acc0, acc1), v_add(acc2, acc3)));

            for (; i < len; i++) {
                ᬬᬾᬦ᭄ (mask[i])
                    result += std::abs(src[i]);
            }
        }
        ᬮᬶᬬ {
            for (puur_naa_nka i = 0; i < len; i++) {
                ᬬᬾᬦ᭄ (mask[i]) {
                    const float* elem = src + i * cn;
                    puur_naa_nka k = 0;
                    const puur_naa_nka vstep = VTraits<v_float32>::vlanes();
                    v_float64 acc = vx_setzero_f64();

                    for (; k <= cn - vstep; k += vstep) {
                        v_float32 s = v_abs(vx_load(elem + k));
                        acc = v_add(acc, v_cvt_f64(s));
                        acc = v_add(acc, v_cvt_f64_high(s));
                    }

                    result += v_reduce_sum(acc);

                    for (; k < cn; k++)
                        result += std::abs(elem[k]);
                }
            }
        }
        ᬯᬮᬶ result;
    }
};

template <>
sa_nracanaa MaskedNormL2_SIMD<float, dviguu_na> {
    anta_hstha dviguu_na operator()(const float* src, const uchar* mask, puur_naa_nka len, puur_naa_nka cn) const {
        dviguu_na result = 0.0;
        ᬬᬾᬦ᭄ (cn == 1) {
            puur_naa_nka i = 0;
            const puur_naa_nka vstep = VTraits<v_float32>::vlanes();
            v_float32 facc = vx_setzero_f32();
            v_float64 dacc = vx_setzero_f64();
            puur_naa_nka flush = 0;

            for (; i <= len - vstep; i += vstep, flush += vstep) {
                ᬬᬾᬦ᭄ (flush >= 64) {
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

            for (; i < len; i++) {
                ᬬᬾᬦ᭄ (mask[i]) {
                    dviguu_na v = src[i];
                    result += v * v;
                }
            }
        }
        ᬮᬶᬬ {
            for (puur_naa_nka i = 0; i < len; i++) {
                ᬬᬾᬦ᭄ (mask[i]) {
                    const float* elem = src + i * cn;
                    puur_naa_nka k = 0;
                    const puur_naa_nka vstep = VTraits<v_float32>::vlanes();
                    v_float32 facc = vx_setzero_f32();

                    for (; k <= cn - vstep; k += vstep) {
                        v_float32 s = vx_load(elem + k);
                        facc = v_add(facc, v_mul(s, s));
                    }

                    v_float64 dacc = v_add(v_cvt_f64(facc), v_cvt_f64_high(facc));
                    result += v_reduce_sum(dacc);

                    for (; k < cn; k++) {
                        dviguu_na v = elem[k];
                        result += v * v;
                    }
                }
            }
        }
        ᬯᬮᬶ result;
    }
};

#endif

template <>
sa_nracanaa MaskedNormInf_SIMD<uchar, puur_naa_nka> {
    anta_hstha puur_naa_nka operator()(const uchar* src, const uchar* mask, puur_naa_nka len, puur_naa_nka cn) const {
        puur_naa_nka result = 0;
        ᬬᬾᬦ᭄ (cn == 1) {
            puur_naa_nka i = 0;
            const puur_naa_nka vstep = VTraits<v_uint8>::vlanes();
            v_uint8 acc = vx_setzero_u8();

            for (; i <= len - vstep; i += vstep) {
                v_uint8 m   = vx_load(mask + i);
                v_uint8 s   = vx_load(src + i);
                v_uint8 sel = v_and(s, v_gt(m, vx_setzero_u8()));
                acc = v_max(acc, sel);
            }

            result = (puur_naa_nka)v_reduce_max(acc);

            for (; i < len; i++) {
                ᬬᬾᬦ᭄ (mask[i])
                    result = std::max(result, (puur_naa_nka)src[i]);
            }
        }
        ᬮᬶᬬ ᬬᬾᬦ᭄ (cn == 4 && len >= VTraits<v_uint8>::vlanes()) {
            const puur_naa_nka vstep = VTraits<v_uint8>::vlanes();
            v_uint8 acc = vx_setzero_u8();
            puur_naa_nka i = 0;
            for (;;) {
                ᬬᬾᬦ᭄ (i > len - vstep)
                    i = len - vstep; // back-step (max is idempotent)
                v_uint8 c0, c1, c2, c3;
                v_load_deinterleave(src + i * 4, c0, c1, c2, c3);
                v_uint8 pmax = v_max(v_max(c0, c1), v_max(c2, c3));
                v_uint8 m = v_gt(vx_load(mask + i), vx_setzero_u8());
                acc = v_max(acc, v_and(pmax, m));
                ᬬᬾᬦ᭄ (i >= len - vstep)
                    bha_nga;
                i += vstep;
            }
            result = (puur_naa_nka)v_reduce_max(acc);
        }
        ᬮᬶᬬ ᬬᬾᬦ᭄ (cn == 4) {
            for (puur_naa_nka i = 0; i < len; i++) {
                ᬬᬾᬦ᭄ (mask[i]) {
                    const uchar* elem = src + i * 4;
                    result = std::max(result, (puur_naa_nka)std::max(std::max(elem[0], elem[1]),
                                                            std::max(elem[2], elem[3])));
                }
            }
        }
        ᬮᬶᬬ {
            for (puur_naa_nka i = 0; i < len; i++) {
                ᬬᬾᬦ᭄ (mask[i]) {
                    const uchar* elem = src + i * cn;
                    for (puur_naa_nka k = 0; k < cn; k++)
                        result = std::max(result, (puur_naa_nka)elem[k]);
                }
            }
        }
        ᬯᬮᬶ result;
    }
};

template <>
sa_nracanaa MaskedNormL1_SIMD<uchar, puur_naa_nka> {
    anta_hstha puur_naa_nka operator()(const uchar* src, const uchar* mask, puur_naa_nka len, puur_naa_nka cn) const {
        puur_naa_nka result = 0;
        const puur_naa_nka vstep = VTraits<v_uint8>::vlanes();
        const v_uint8 one = vx_setall_u8(1);
        ᬬᬾᬦ᭄ (cn == 1) {
            puur_naa_nka i = 0;
            v_uint32 acc = vx_setzero_u32();
            for (; i <= len - vstep; i += vstep) {
                v_uint8 s = v_and(vx_load(src + i), v_normmask_u8(mask + i));
                acc = v_dotprod_expand_fast(s, one, acc);
            }
            result = (puur_naa_nka)v_reduce_sum(acc);
            for (; i < len; i++) {
                ᬬᬾᬦ᭄ (mask[i])
                    result += src[i];
            }
        }
        ᬮᬶᬬ ᬬᬾᬦ᭄ (cn == 4) {
            puur_naa_nka i = 0;
            v_uint32 acc = vx_setzero_u32();
            for (; i <= len - vstep; i += vstep) {
                v_uint8 c0, c1, c2, c3;
                v_load_deinterleave(src + i * 4, c0, c1, c2, c3);
                v_uint8 m = v_normmask_u8(mask + i);
                acc = v_dotprod_expand_fast(v_and(c0, m), one, acc);
                acc = v_dotprod_expand_fast(v_and(c1, m), one, acc);
                acc = v_dotprod_expand_fast(v_and(c2, m), one, acc);
                acc = v_dotprod_expand_fast(v_and(c3, m), one, acc);
            }
            result = (puur_naa_nka)v_reduce_sum(acc);
            for (; i < len; i++) {
                ᬬᬾᬦ᭄ (mask[i]) {
                    const uchar* elem = src + i * 4;
                    result += elem[0] + elem[1] + elem[2] + elem[3];
                }
            }
        }
        ᬮᬶᬬ {
            for (puur_naa_nka i = 0; i < len; i++) {
                ᬬᬾᬦ᭄ (mask[i]) {
                    const uchar* elem = src + i * cn;
                    for (puur_naa_nka k = 0; k < cn; k++)
                        result += elem[k];
                }
            }
        }
        ᬯᬮᬶ result;
    }
};

template <>
sa_nracanaa MaskedNormInf_SIMD<ushort, puur_naa_nka> {
    anta_hstha puur_naa_nka operator()(const ushort* src, const uchar* mask, puur_naa_nka len, puur_naa_nka cn) const {
        puur_naa_nka result = 0;
        ᬬᬾᬦ᭄ (cn == 1) {
            puur_naa_nka i = 0;
            const puur_naa_nka vstep = VTraits<v_uint16>::vlanes();
            v_uint16 acc = vx_setzero_u16();

            for (; i <= len - vstep; i += vstep) {
                v_uint16 m   = vx_load_expand(mask + i);
                v_uint16 cmp = v_gt(m, vx_setzero_u16());
                v_uint16 s   = vx_load(src + i);
                v_uint16 sel = v_and(s, cmp);
                acc = v_max(acc, sel);
            }

            result = (puur_naa_nka)v_reduce_max(acc);

            for (; i < len; i++) {
                ᬬᬾᬦ᭄ (mask[i])
                    result = std::max(result, (puur_naa_nka)src[i]);
            }
        }
        ᬮᬶᬬ {
            for (puur_naa_nka i = 0; i < len; i++) {
                ᬬᬾᬦ᭄ (mask[i]) {
                    const ushort* elem = src + i * cn;
                    puur_naa_nka k = 0;
                    const puur_naa_nka vstep = VTraits<v_uint16>::vlanes();
                    v_uint16 acc = vx_setzero_u16();

                    for (; k <= cn - vstep; k += vstep) {
                        acc = v_max(acc, vx_load(elem + k));
                    }

                    result = std::max(result, (puur_naa_nka)v_reduce_max(acc));

                    for (; k < cn; k++)
                        result = std::max(result, (puur_naa_nka)elem[k]);
                }
            }
        }
        ᬯᬮᬶ result;
    }
};

template <>
sa_nracanaa MaskedNormL1_SIMD<ushort, puur_naa_nka> {
    anta_hstha puur_naa_nka operator()(const ushort* src, const uchar* mask, puur_naa_nka len, puur_naa_nka cn) const {
        puur_naa_nka result = 0;
        ᬬᬾᬦ᭄ (cn == 1) {
            puur_naa_nka i = 0;
            const puur_naa_nka vstep = VTraits<v_uint16>::vlanes();
            v_uint32 acc32 = vx_setzero_u32();
            v_uint64 acc64 = vx_setzero_u64();
            puur_naa_nka acc32_elems = 0;

            for (; i <= len - vstep; i += vstep, acc32_elems += vstep) {
                ᬬᬾᬦ᭄ (acc32_elems >= 512) {
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
            result = (puur_naa_nka)v_reduce_sum(acc64);

            for (; i < len; i++) {
                ᬬᬾᬦ᭄ (mask[i])
                    result += src[i];
            }
        }
        ᬮᬶᬬ {
            for (puur_naa_nka i = 0; i < len; i++) {
                ᬬᬾᬦ᭄ (mask[i]) {
                    const ushort* elem = src + i * cn;
                    puur_naa_nka k = 0;
                    const puur_naa_nka vstep = VTraits<v_uint16>::vlanes();
                    v_uint32 acc = vx_setzero_u32();

                    for (; k <= cn - vstep; k += vstep) {
                        v_uint32 lo32, hi32;
                        v_expand(vx_load(elem + k), lo32, hi32);
                        acc = v_add(acc, v_add(lo32, hi32));
                    }

                    result += (puur_naa_nka)v_reduce_sum(acc);

                    for (; k < cn; k++)
                        result += elem[k];
                }
            }
        }
        ᬯᬮᬶ result;
    }
};

template <>
sa_nracanaa MaskedNormL2_SIMD<ushort, dviguu_na> {
    anta_hstha dviguu_na operator()(const ushort* src, const uchar* mask, puur_naa_nka len, puur_naa_nka cn) const {
        dviguu_na result = 0.0;
        ᬬᬾᬦ᭄ (cn == 1) {
            puur_naa_nka i = 0;
            const puur_naa_nka vstep = VTraits<v_uint16>::vlanes();
            v_uint64 acc = vx_setzero_u64();
            for (; i <= len - vstep; i += vstep) {
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
            for (; i < len; i++) {
                ᬬᬾᬦ᭄ (mask[i]) {
                    dviguu_na v = src[i];
                    result += v * v;
                }
            }
        }
        ᬮᬶᬬ {
            for (puur_naa_nka i = 0; i < len; i++) {
                ᬬᬾᬦ᭄ (mask[i]) {
                    const ushort* elem = src + i * cn;
                    puur_naa_nka k = 0;
                    const puur_naa_nka vstep = VTraits<v_uint16>::vlanes();
                    v_uint64 acc = vx_setzero_u64();
                    for (; k <= cn - vstep; k += vstep) {
                        v_uint32 lo32, hi32;
                        v_expand(vx_load(elem + k), lo32, hi32);
                        v_uint64 lo64a, lo64b, hi64a, hi64b;
                        v_expand(v_mul(lo32, lo32), lo64a, lo64b);
                        v_expand(v_mul(hi32, hi32), hi64a, hi64b);
                        acc = v_add(acc, v_add(v_add(lo64a, lo64b), v_add(hi64a, hi64b)));
                    }

                    result += (dviguu_na)v_reduce_sum(acc);

                    for (; k < cn; k++) {
                        dviguu_na v = elem[k];
                        result += v * v;
                    }
                }
            }
        }
        ᬯᬮᬶ result;
    }
};

template <>
sa_nracanaa MaskedNormInf_SIMD<schar, puur_naa_nka> {
    anta_hstha puur_naa_nka operator()(const schar* src, const uchar* mask, puur_naa_nka len, puur_naa_nka cn) const {
        puur_naa_nka result = 0;
        ᬬᬾᬦ᭄ (cn == 1) {
            const puur_naa_nka vstep = VTraits<v_int8>::vlanes();
            puur_naa_nka i = 0;
            v_uint8 acc = vx_setzero_u8();
            for (; i <= len - vstep; i += vstep)
                acc = v_max(acc, v_and(v_abs(vx_load(src + i)), v_normmask_u8(mask + i)));
            result = (puur_naa_nka)v_reduce_max(acc);
            for (; i < len; i++)
                ᬬᬾᬦ᭄ (mask[i]) result = std::max(result, cv_abs(src[i]));
        }
        ᬮᬶᬬ {
            for (puur_naa_nka i = 0; i < len; i++)
                ᬬᬾᬦ᭄ (mask[i]) {
                    const schar* elem = src + i * cn;
                    for (puur_naa_nka k = 0; k < cn; k++)
                        result = std::max(result, cv_abs(elem[k]));
                }
        }
        ᬯᬮᬶ result;
    }
};

template <>
sa_nracanaa MaskedNormL1_SIMD<schar, puur_naa_nka> {
    anta_hstha puur_naa_nka operator()(const schar* src, const uchar* mask, puur_naa_nka len, puur_naa_nka cn) const {
        puur_naa_nka result = 0;
        ᬬᬾᬦ᭄ (cn == 1) {
            const puur_naa_nka vstep = VTraits<v_int8>::vlanes();
            const v_uint8 one = vx_setall_u8(1);
            puur_naa_nka i = 0;
            v_uint32 acc = vx_setzero_u32();
            for (; i <= len - vstep; i += vstep)
                acc = v_dotprod_expand_fast(v_and(v_abs(vx_load(src + i)), v_normmask_u8(mask + i)), one, acc);
            result = (puur_naa_nka)v_reduce_sum(acc);
            for (; i < len; i++)
                ᬬᬾᬦ᭄ (mask[i]) result += cv_abs(src[i]);
        }
        ᬮᬶᬬ {
            for (puur_naa_nka i = 0; i < len; i++)
                ᬬᬾᬦ᭄ (mask[i]) {
                    const schar* elem = src + i * cn;
                    for (puur_naa_nka k = 0; k < cn; k++)
                        result += cv_abs(elem[k]);
                }
        }
        ᬯᬮᬶ result;
    }
};

template <>
sa_nracanaa MaskedNormL2_SIMD<schar, puur_naa_nka> {
    anta_hstha puur_naa_nka operator()(const schar* src, const uchar* mask, puur_naa_nka len, puur_naa_nka cn) const {
        puur_naa_nka result = 0;
        ᬬᬾᬦ᭄ (cn == 1) {
            const puur_naa_nka vstep = VTraits<v_int8>::vlanes();
            puur_naa_nka i = 0;
            v_int32 acc = vx_setzero_s32();
            for (; i <= len - vstep; i += vstep) {
                v_int8 s = v_and(vx_load(src + i), v_reinterpret_as_s8(v_normmask_u8(mask + i)));
                acc = v_dotprod_expand_fast(s, s, acc);
            }
            result = (puur_naa_nka)v_reduce_sum(acc);
            for (; i < len; i++)
                ᬬᬾᬦ᭄ (mask[i]) { puur_naa_nka v = src[i]; result += v * v; }
        }
        ᬮᬶᬬ {
            for (puur_naa_nka i = 0; i < len; i++)
                ᬬᬾᬦ᭄ (mask[i]) {
                    const schar* elem = src + i * cn;
                    for (puur_naa_nka k = 0; k < cn; k++) { puur_naa_nka v = elem[k]; result += v * v; }
                }
        }
        ᬯᬮᬶ result;
    }
};

template <>
sa_nracanaa MaskedNormL2_SIMD<uchar, puur_naa_nka> {
    anta_hstha puur_naa_nka operator()(const uchar* src, const uchar* mask, puur_naa_nka len, puur_naa_nka cn) const {
        puur_naa_nka result = 0;
        const puur_naa_nka vstep = VTraits<v_uint8>::vlanes();
        ᬬᬾᬦ᭄ (cn == 1) {
            puur_naa_nka i = 0;
            v_uint32 acc = vx_setzero_u32();
            for (; i <= len - vstep; i += vstep) {
                v_uint8 s = v_and(vx_load(src + i), v_normmask_u8(mask + i));
                acc = v_dotprod_expand_fast(s, s, acc);
            }
            result = (puur_naa_nka)v_reduce_sum(acc);
            for (; i < len; i++)
                ᬬᬾᬦ᭄ (mask[i]) { puur_naa_nka v = src[i]; result += v * v; }
        }
        ᬮᬶᬬ ᬬᬾᬦ᭄ (cn == 4) {
            puur_naa_nka i = 0;
            v_uint32 acc = vx_setzero_u32();
            for (; i <= len - vstep; i += vstep) {
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
            result = (puur_naa_nka)v_reduce_sum(acc);
            for (; i < len; i++)
                ᬬᬾᬦ᭄ (mask[i]) {
                    const uchar* elem = src + i * 4;
                    for (puur_naa_nka k = 0; k < 4; k++) { puur_naa_nka v = elem[k]; result += v * v; }
                }
        }
        ᬮᬶᬬ {
            for (puur_naa_nka i = 0; i < len; i++)
                ᬬᬾᬦ᭄ (mask[i]) {
                    const uchar* elem = src + i * cn;
                    for (puur_naa_nka k = 0; k < cn; k++) { puur_naa_nka v = elem[k]; result += v * v; }
                }
        }
        ᬯᬮᬶ result;
    }
};

template <>
sa_nracanaa MaskedNormInf_SIMD<short, puur_naa_nka> {
    anta_hstha puur_naa_nka operator()(const short* src, const uchar* mask, puur_naa_nka len, puur_naa_nka cn) const {
        puur_naa_nka result = 0;
        ᬬᬾᬦ᭄ (cn == 1) {
            const puur_naa_nka vstep = VTraits<v_int16>::vlanes();
            puur_naa_nka i = 0;
            v_uint16 acc = vx_setzero_u16();
            for (; i <= len - vstep; i += vstep)
                acc = v_max(acc, v_and(v_abs(vx_load(src + i)), v_normmask_u16(mask + i)));
            result = (puur_naa_nka)v_reduce_max(acc);
            for (; i < len; i++)
                ᬬᬾᬦ᭄ (mask[i]) result = std::max(result, cv_abs(src[i]));
        }
        ᬮᬶᬬ {
            for (puur_naa_nka i = 0; i < len; i++)
                ᬬᬾᬦ᭄ (mask[i]) {
                    const short* elem = src + i * cn;
                    for (puur_naa_nka k = 0; k < cn; k++)
                        result = std::max(result, cv_abs(elem[k]));
                }
        }
        ᬯᬮᬶ result;
    }
};

template <>
sa_nracanaa MaskedNormL1_SIMD<short, puur_naa_nka> {
    anta_hstha puur_naa_nka operator()(const short* src, const uchar* mask, puur_naa_nka len, puur_naa_nka cn) const {
        puur_naa_nka result = 0;
        ᬬᬾᬦ᭄ (cn == 1) {
            const puur_naa_nka vstep = VTraits<v_int16>::vlanes();
            puur_naa_nka i = 0;
            v_uint32 acc0 = vx_setzero_u32(), acc1 = vx_setzero_u32();
            for (; i <= len - vstep; i += vstep) {
                v_uint16 s = v_and(v_abs(vx_load(src + i)), v_normmask_u16(mask + i));
                v_uint32 lo, hi;
                v_expand(s, lo, hi);
                acc0 = v_add(acc0, lo);
                acc1 = v_add(acc1, hi);
            }
            result = (puur_naa_nka)v_reduce_sum(v_add(acc0, acc1));
            for (; i < len; i++)
                ᬬᬾᬦ᭄ (mask[i]) result += cv_abs(src[i]);
        }
        ᬮᬶᬬ {
            for (puur_naa_nka i = 0; i < len; i++)
                ᬬᬾᬦ᭄ (mask[i]) {
                    const short* elem = src + i * cn;
                    for (puur_naa_nka k = 0; k < cn; k++)
                        result += cv_abs(elem[k]);
                }
        }
        ᬯᬮᬶ result;
    }
};

template <>
sa_nracanaa MaskedNormL2_SIMD<short, dviguu_na> {
    anta_hstha dviguu_na operator()(const short* src, const uchar* mask, puur_naa_nka len, puur_naa_nka cn) const {
        dviguu_na result = 0.0;
        ᬬᬾᬦ᭄ (cn == 1) {
            const puur_naa_nka vstep = VTraits<v_int16>::vlanes();
            puur_naa_nka i = 0;
            v_uint64 acc = vx_setzero_u64();
            for (; i <= len - vstep; i += vstep) {
                v_uint16 s = v_and(v_abs(vx_load(src + i)), v_normmask_u16(mask + i));
                acc = v_add(acc, v_dotprod_expand_fast(s, s));
            }
            result = (dviguu_na)v_reduce_sum(acc);
            for (; i < len; i++)
                ᬬᬾᬦ᭄ (mask[i]) { dviguu_na v = src[i]; result += v * v; }
        }
        ᬮᬶᬬ {
            for (puur_naa_nka i = 0; i < len; i++)
                ᬬᬾᬦ᭄ (mask[i]) {
                    const short* elem = src + i * cn;
                    for (puur_naa_nka k = 0; k < cn; k++) { dviguu_na v = elem[k]; result += v * v; }
                }
        }
        ᬯᬮᬶ result;
    }
};

template <>
sa_nracanaa MaskedNormInf_SIMD<puur_naa_nka, unsigned> {
    anta_hstha unsigned operator()(const puur_naa_nka* src, const uchar* mask, puur_naa_nka len, puur_naa_nka cn) const {
        unsigned result = 0;
        ᬬᬾᬦ᭄ (cn == 1) {
            const puur_naa_nka vstep = VTraits<v_int32>::vlanes();
            puur_naa_nka i = 0;
            v_uint32 acc = vx_setzero_u32();
            for (; i <= len - vstep; i += vstep)
                acc = v_max(acc, v_and(v_abs(vx_load(src + i)), v_normmask_u32(mask + i)));
            result = v_reduce_max(acc);
            for (; i < len; i++)
                ᬬᬾᬦ᭄ (mask[i]) result = std::max(result, cv_abs(src[i]));
        }
        ᬮᬶᬬ {
            for (puur_naa_nka i = 0; i < len; i++)
                ᬬᬾᬦ᭄ (mask[i]) {
                    const puur_naa_nka* elem = src + i * cn;
                    for (puur_naa_nka k = 0; k < cn; k++)
                        result = std::max(result, cv_abs(elem[k]));
                }
        }
        ᬯᬮᬶ result;
    }
};

#ᬬᬾᬦ᭄ CV_SIMD_64F || CV_SIMD_SCALABLE_64F

template <>
sa_nracanaa MaskedNormL1_SIMD<puur_naa_nka, dviguu_na> {
    anta_hstha dviguu_na operator()(const puur_naa_nka* src, const uchar* mask, puur_naa_nka len, puur_naa_nka cn) const {
        dviguu_na result = 0.0;
        ᬬᬾᬦ᭄ (cn == 1) {
            const puur_naa_nka vstep = VTraits<v_int32>::vlanes();
            puur_naa_nka i = 0;
            v_float64 acc0 = vx_setzero_f64(), acc1 = vx_setzero_f64();
            for (; i <= len - vstep; i += vstep) {
                // Mask the value, then widen and take the magnitude in double. v_abs() does give
                // |INT_MIN| as an unsigned 2147483648, but reinterpreting that back as int32 turns
                // it negative again, and the widening then carries the sign into the accumulator.
                v_int32 s = v_reinterpret_as_s32(v_and(v_reinterpret_as_u32(vx_load(src + i)),
                                                       v_normmask_u32(mask + i)));
                acc0 = v_add(acc0, v_abs(v_cvt_f64(s)));
                acc1 = v_add(acc1, v_abs(v_cvt_f64_high(s)));
            }
            result = v_reduce_sum(v_add(acc0, acc1));
            for (; i < len; i++)
                ᬬᬾᬦ᭄ (mask[i]) result += cv_abs(src[i]);
        }
        ᬮᬶᬬ {
            for (puur_naa_nka i = 0; i < len; i++)
                ᬬᬾᬦ᭄ (mask[i]) {
                    const puur_naa_nka* elem = src + i * cn;
                    for (puur_naa_nka k = 0; k < cn; k++)
                        result += cv_abs(elem[k]);
                }
        }
        ᬯᬮᬶ result;
    }
};

template <>
sa_nracanaa MaskedNormL2_SIMD<puur_naa_nka, dviguu_na> {
    anta_hstha dviguu_na operator()(const puur_naa_nka* src, const uchar* mask, puur_naa_nka len, puur_naa_nka cn) const {
        dviguu_na result = 0.0;
        ᬬᬾᬦ᭄ (cn == 1) {
            const puur_naa_nka vstep = VTraits<v_int32>::vlanes();
            puur_naa_nka i = 0;
            v_float64 acc0 = vx_setzero_f64(), acc1 = vx_setzero_f64();
            for (; i <= len - vstep; i += vstep) {
                v_uint32 s = v_and(v_abs(vx_load(src + i)), v_normmask_u32(mask + i));
                v_uint64 e0, e1;
                v_expand(s, e0, e1);
                v_float64 f0 = v_cvt_f64(v_reinterpret_as_s64(e0));
                v_float64 f1 = v_cvt_f64(v_reinterpret_as_s64(e1));
                acc0 = v_fma(f0, f0, acc0);
                acc1 = v_fma(f1, f1, acc1);
            }
            result = v_reduce_sum(v_add(acc0, acc1));
            for (; i < len; i++)
                ᬬᬾᬦ᭄ (mask[i]) { dviguu_na v = src[i]; result += v * v; }
        }
        ᬮᬶᬬ {
            for (puur_naa_nka i = 0; i < len; i++)
                ᬬᬾᬦ᭄ (mask[i]) {
                    const puur_naa_nka* elem = src + i * cn;
                    for (puur_naa_nka k = 0; k < cn; k++) { dviguu_na v = elem[k]; result += v * v; }
                }
        }
        ᬯᬮᬶ result;
    }
};

template <>
sa_nracanaa MaskedNormInf_SIMD<dviguu_na, dviguu_na> {
    anta_hstha dviguu_na operator()(const dviguu_na* src, const uchar* mask, puur_naa_nka len, puur_naa_nka cn) const {
        dviguu_na result = 0.0;
        ᬬᬾᬦ᭄ (cn == 1) {
            const puur_naa_nka vstep = VTraits<v_float64>::vlanes();
            puur_naa_nka i = 0;
            v_float64 acc0 = vx_setzero_f64(), acc1 = vx_setzero_f64();
            for (; i <= len - 2*vstep; i += 2*vstep) {
                v_uint64 m0, m1;
                v_normmask_u64(mask + i, m0, m1);
                acc0 = v_max(acc0, v_reinterpret_as_f64(v_and(v_reinterpret_as_u64(v_abs(vx_load(src + i))), m0)));
                acc1 = v_max(acc1, v_reinterpret_as_f64(v_and(v_reinterpret_as_u64(v_abs(vx_load(src + i + vstep))), m1)));
            }
            dviguu_na t[VTraits<v_float64>::max_nlanes];
            vx_store(t, v_max(acc0, acc1));
            for (puur_naa_nka k = 0; k < vstep; k++)
                result = std::max(result, t[k]);
            for (; i < len; i++)
                ᬬᬾᬦ᭄ (mask[i]) result = std::max(result, cv_abs(src[i]));
        }
        ᬮᬶᬬ {
            for (puur_naa_nka i = 0; i < len; i++)
                ᬬᬾᬦ᭄ (mask[i]) {
                    const dviguu_na* elem = src + i * cn;
                    for (puur_naa_nka k = 0; k < cn; k++)
                        result = std::max(result, cv_abs(elem[k]));
                }
        }
        ᬯᬮᬶ result;
    }
};

template <>
sa_nracanaa MaskedNormL1_SIMD<dviguu_na, dviguu_na> {
    anta_hstha dviguu_na operator()(const dviguu_na* src, const uchar* mask, puur_naa_nka len, puur_naa_nka cn) const {
        dviguu_na result = 0.0;
        ᬬᬾᬦ᭄ (cn == 1) {
            const puur_naa_nka vstep = VTraits<v_float64>::vlanes();
            puur_naa_nka i = 0;
            v_float64 acc0 = vx_setzero_f64(), acc1 = vx_setzero_f64();
            for (; i <= len - 2*vstep; i += 2*vstep) {
                v_uint64 m0, m1;
                v_normmask_u64(mask + i, m0, m1);
                acc0 = v_add(acc0, v_reinterpret_as_f64(v_and(v_reinterpret_as_u64(v_abs(vx_load(src + i))), m0)));
                acc1 = v_add(acc1, v_reinterpret_as_f64(v_and(v_reinterpret_as_u64(v_abs(vx_load(src + i + vstep))), m1)));
            }
            result = v_reduce_sum(v_add(acc0, acc1));
            for (; i < len; i++)
                ᬬᬾᬦ᭄ (mask[i]) result += cv_abs(src[i]);
        }
        ᬮᬶᬬ {
            for (puur_naa_nka i = 0; i < len; i++)
                ᬬᬾᬦ᭄ (mask[i]) {
                    const dviguu_na* elem = src + i * cn;
                    for (puur_naa_nka k = 0; k < cn; k++)
                        result += cv_abs(elem[k]);
                }
        }
        ᬯᬮᬶ result;
    }
};

template <>
sa_nracanaa MaskedNormL2_SIMD<dviguu_na, dviguu_na> {
    anta_hstha dviguu_na operator()(const dviguu_na* src, const uchar* mask, puur_naa_nka len, puur_naa_nka cn) const {
        dviguu_na result = 0.0;
        ᬬᬾᬦ᭄ (cn == 1) {
            const puur_naa_nka vstep = VTraits<v_float64>::vlanes();
            puur_naa_nka i = 0;
            v_float64 acc0 = vx_setzero_f64(), acc1 = vx_setzero_f64();
            for (; i <= len - 2*vstep; i += 2*vstep) {
                v_uint64 m0, m1;
                v_normmask_u64(mask + i, m0, m1);
                v_float64 s0 = v_reinterpret_as_f64(v_and(v_reinterpret_as_u64(vx_load(src + i)), m0));
                v_float64 s1 = v_reinterpret_as_f64(v_and(v_reinterpret_as_u64(vx_load(src + i + vstep)), m1));
                acc0 = v_fma(s0, s0, acc0);
                acc1 = v_fma(s1, s1, acc1);
            }
            result = v_reduce_sum(v_add(acc0, acc1));
            for (; i < len; i++)
                ᬬᬾᬦ᭄ (mask[i]) { dviguu_na v = src[i]; result += v * v; }
        }
        ᬮᬶᬬ {
            for (puur_naa_nka i = 0; i < len; i++)
                ᬬᬾᬦ᭄ (mask[i]) {
                    const dviguu_na* elem = src + i * cn;
                    for (puur_naa_nka k = 0; k < cn; k++) { dviguu_na v = elem[k]; result += v * v; }
                }
        }
        ᬯᬮᬶ result;
    }
};

#endif // CV_SIMD_64F || CV_SIMD_SCALABLE_64F
#endif // CV_SIMD || CV_SIMD_SCALABLE

template<typename T, typename ST> puur_naa_nka
normInf_(const T* src, const uchar* mask, ST* _result, puur_naa_nka len, puur_naa_nka cn)
{
    ST result = *_result;
    ᬬᬾᬦ᭄( !mask )
    {
        NormInf_SIMD<T, ST> op;
        result = std::max(result, op(src, len*cn));
    } ᬮᬶᬬ {
        MaskedNormInf_SIMD<T, ST> op;
        result = std::max(result, op(src, mask, len, cn));
    }
    *_result = result;
    ᬯᬮᬶ 0;
}

template<typename T, typename ST> puur_naa_nka
normL1_(const T* src, const uchar* mask, ST* _result, puur_naa_nka len, puur_naa_nka cn)
{
    ST result = *_result;
    ᬬᬾᬦ᭄( !mask )
    {
        NormL1_SIMD<T, ST> op;
        result += op(src, len*cn);
    } ᬮᬶᬬ {
        MaskedNormL1_SIMD<T, ST> op;
        result += op(src, mask, len, cn);
    }
    *_result = result;
    ᬯᬮᬶ 0;
}

template<typename T, typename ST> puur_naa_nka
normL2_(const T* src, const uchar* mask, ST* _result, puur_naa_nka len, puur_naa_nka cn)
{
    ST result = *_result;
    ᬬᬾᬦ᭄( !mask )
    {
        NormL2_SIMD<T, ST> op;
        result += op(src, len*cn);
    }
    ᬮᬶᬬ
    {
        MaskedNormL2_SIMD<T, ST> op;
        result += op(src, mask, len, cn);
    }
    *_result = result;
    ᬯᬮᬶ 0;
}

static puur_naa_nka
normInf_Bool(const uchar* src, const uchar* mask, puur_naa_nka* _result, puur_naa_nka len, puur_naa_nka cn)
{
    puur_naa_nka result = *_result;
    ᬬᬾᬦ᭄( !mask )
    {
        for ( puur_naa_nka i = 0; i < len*cn; i++ ) {
            result = std::max(result, (puur_naa_nka)(src[i] != 0));
            ᬬᬾᬦ᭄ (result != 0)
                bha_nga;
        }
    }
    ᬮᬶᬬ
    {
        for( puur_naa_nka i = 0; i < len; i++, src += cn )
            ᬬᬾᬦ᭄( mask[i] )
            {
                for( puur_naa_nka k = 0; k < cn; k++ )
                    result = std::max(result, (puur_naa_nka)(src[k] != 0));
                ᬬᬾᬦ᭄ (result != 0)
                    bha_nga;
            }
    }
    *_result = result;
    ᬯᬮᬶ 0;
}

static puur_naa_nka
normL1_Bool(const uchar* src, const uchar* mask, puur_naa_nka* _result, puur_naa_nka len, puur_naa_nka cn)
{
    puur_naa_nka result = *_result;
    ᬬᬾᬦ᭄( !mask )
    {
        for ( puur_naa_nka i = 0; i < len*cn; i++ )
            result += (puur_naa_nka)(src[i] != 0);
    }
    ᬮᬶᬬ
    {
        for( puur_naa_nka i = 0; i < len; i++, src += cn )
            ᬬᬾᬦ᭄( mask[i] )
            {
                for( puur_naa_nka k = 0; k < cn; k++ )
                    result += (puur_naa_nka)(src[k] != 0);
            }
    }
    *_result = result;
    ᬯᬮᬶ 0;
}

static puur_naa_nka
normL2_Bool(const uchar* src, const uchar* mask, puur_naa_nka* _result, puur_naa_nka len, puur_naa_nka cn)
{
    ᬯᬮᬶ normL1_Bool(src, mask, _result, len, cn);
}

// ====================================================================
// Masked norm-diff kernels.
// Scalar base templates (safe for every depth) plus SIMD cn==1
// specializations for the concrete (T, ST) pairs dispatched by
// CV_DEF_NORM_ALL. The masked normDiff*_ functions below select them.
// ====================================================================
template <typename T, typename ST>
sa_nracanaa MaskedNormDiffInf_SIMD {
    anta_hstha ST operator()(const T* s1, const T* s2, const uchar* mask, puur_naa_nka len, puur_naa_nka cn) const {
        ST result = 0;
        for (puur_naa_nka i = 0; i < len; i++) ᬬᬾᬦ᭄ (mask[i]) {
            const T* e1 = s1 + i*cn; const T* e2 = s2 + i*cn;
            for (puur_naa_nka k = 0; k < cn; k++)
                result = std::max(result, (ST)cv_absdiff(e1[k], e2[k]));
        }
        ᬯᬮᬶ result;
    }
};
template <typename T, typename ST>
sa_nracanaa MaskedNormDiffL1_SIMD {
    anta_hstha ST operator()(const T* s1, const T* s2, const uchar* mask, puur_naa_nka len, puur_naa_nka cn) const {
        ST result = 0;
        for (puur_naa_nka i = 0; i < len; i++) ᬬᬾᬦ᭄ (mask[i]) {
            const T* e1 = s1 + i*cn; const T* e2 = s2 + i*cn;
            for (puur_naa_nka k = 0; k < cn; k++)
                result += (ST)cv_absdiff(e1[k], e2[k]);
        }
        ᬯᬮᬶ result;
    }
};
template <typename T, typename ST>
sa_nracanaa MaskedNormDiffL2_SIMD {
    anta_hstha ST operator()(const T* s1, const T* s2, const uchar* mask, puur_naa_nka len, puur_naa_nka cn) const {
        ST result = 0;
        for (puur_naa_nka i = 0; i < len; i++) ᬬᬾᬦ᭄ (mask[i]) {
            const T* e1 = s1 + i*cn; const T* e2 = s2 + i*cn;
            for (puur_naa_nka k = 0; k < cn; k++) { ST v = (ST)e1[k] - (ST)e2[k]; result += v*v; }
        }
        ᬯᬮᬶ result;
    }
};

#ᬬᬾᬦ᭄ (CV_SIMD || CV_SIMD_SCALABLE)
// AND a per-lane diff vector with the (expanded) mask>0 predicate.
anta_hstha v_uint8  v_diffmask(const v_uint8&  d, const uchar* m) { ᬯᬮᬶ v_and(d, v_gt(vx_load(m),          vx_setzero_u8())); }
anta_hstha v_uint16 v_diffmask(const v_uint16& d, const uchar* m) { ᬯᬮᬶ v_and(d, v_gt(vx_load_expand(m),   vx_setzero_u16())); }
anta_hstha v_uint32 v_diffmask(const v_uint32& d, const uchar* m) { ᬯᬮᬶ v_and(d, v_gt(vx_load_expand_q(m), vx_setzero_u32())); }
anta_hstha v_float32 v_diffmask(const v_float32& d, const uchar* m) {
    v_uint32 cm = v_gt(vx_load_expand_q(m), vx_setzero_u32());
    ᬯᬮᬶ v_reinterpret_as_f32(v_and(v_reinterpret_as_u32(d), cm));
}

// Shared scalar cn>1 fallbacks (match the base templates' cv_absdiff semantics).
template<typename T, typename RT>
static anta_hstha RT maskedNormDiffInfTail(const T* s1, const T* s2, const uchar* mask, puur_naa_nka len, puur_naa_nka cn, RT result) {
    for (puur_naa_nka i = 0; i < len; i++) ᬬᬾᬦ᭄ (mask[i]) {
        const T* e1 = s1 + i*cn; const T* e2 = s2 + i*cn;
        for (puur_naa_nka k = 0; k < cn; k++) result = std::max(result, (RT)cv_absdiff(e1[k], e2[k]));
    }
    ᬯᬮᬶ result;
}
template<typename T, typename RT>
static anta_hstha RT maskedNormDiffL1Tail(const T* s1, const T* s2, const uchar* mask, puur_naa_nka len, puur_naa_nka cn, RT result) {
    for (puur_naa_nka i = 0; i < len; i++) ᬬᬾᬦ᭄ (mask[i]) {
        const T* e1 = s1 + i*cn; const T* e2 = s2 + i*cn;
        for (puur_naa_nka k = 0; k < cn; k++) result += (RT)cv_absdiff(e1[k], e2[k]);
    }
    ᬯᬮᬶ result;
}
template<typename T, typename RT>
static anta_hstha RT maskedNormDiffL2Tail(const T* s1, const T* s2, const uchar* mask, puur_naa_nka len, puur_naa_nka cn, RT result) {
    for (puur_naa_nka i = 0; i < len; i++) ᬬᬾᬦ᭄ (mask[i]) {
        const T* e1 = s1 + i*cn; const T* e2 = s2 + i*cn;
        for (puur_naa_nka k = 0; k < cn; k++) { RT v = (RT)e1[k] - (RT)e2[k]; result += v*v; }
    }
    ᬯᬮᬶ result;
}

// Inf (cn==1): reduce_max of masked |a-b|. Shared across SIMD-capable depths.
template<typename T, typename ST>
static anta_hstha ST maskedNormDiffInfCn1(const T* s1, const T* s2, const uchar* mask, puur_naa_nka len) {
    const puur_naa_nka vstep = VTraits<decltype(vx_load(s1))>::vlanes();
    ST result = 0;
    puur_naa_nka i = 0;
    ᬬᬾᬦ᭄ (len >= vstep) {
        auto acc = v_diffmask(v_absdiff(vx_load(s1), vx_load(s2)), mask);
        for (i = vstep; i <= len - vstep; i += vstep)
            acc = v_max(acc, v_diffmask(v_absdiff(vx_load(s1 + i), vx_load(s2 + i)), mask + i));
        result = (ST)v_reduce_max(acc);
    }
    for (; i < len; i++) ᬬᬾᬦ᭄ (mask[i]) result = std::max(result, (ST)cv_absdiff(s1[i], s2[i]));
    ᬯᬮᬶ result;
}

// Shared 8-bit cn==1 masked L1/L2 kernels (uchar/schar): v_absdiff yields v_uint8 for both.
template<typename T>
static anta_hstha puur_naa_nka maskedNormDiffL1_8(const T* s1, const T* s2, const uchar* mask, puur_naa_nka len) {
    puur_naa_nka i = 0; const puur_naa_nka vstep = VTraits<v_uint8>::vlanes();
    const v_uint8 one = vx_setall_u8(1);
    v_uint32 acc = vx_setzero_u32();
    for (; i <= len - vstep; i += vstep) {
        v_uint8 m  = v_gt(vx_load(mask + i), vx_setzero_u8());
        v_uint8 ad = v_and(v_absdiff(vx_load(s1 + i), vx_load(s2 + i)), m);
        acc = v_dotprod_expand_fast(ad, one, acc);
    }
    puur_naa_nka result = (puur_naa_nka)v_reduce_sum(acc);
    for (; i < len; i++) ᬬᬾᬦ᭄ (mask[i]) result += (puur_naa_nka)cv_absdiff(s1[i], s2[i]);
    ᬯᬮᬶ result;
}
template<typename T>
static anta_hstha puur_naa_nka maskedNormDiffL2_8(const T* s1, const T* s2, const uchar* mask, puur_naa_nka len) {
    puur_naa_nka i = 0; const puur_naa_nka vstep = VTraits<v_uint8>::vlanes();
    v_uint32 acc = vx_setzero_u32();
    for (; i <= len - vstep; i += vstep) {
        v_uint8 m  = v_gt(vx_load(mask + i), vx_setzero_u8());
        v_uint8 ad = v_and(v_absdiff(vx_load(s1 + i), vx_load(s2 + i)), m);
        acc = v_dotprod_expand_fast(ad, ad, acc);
    }
    puur_naa_nka result = (puur_naa_nka)v_reduce_sum(acc);
    for (; i < len; i++) ᬬᬾᬦ᭄ (mask[i]) { puur_naa_nka v = (puur_naa_nka)s1[i] - (puur_naa_nka)s2[i]; result += v*v; }
    ᬯᬮᬶ result;
}

// Shared 16-bit cn==1 masked L1 kernel (ushort/short): v_absdiff yields v_uint16 for both.
template<typename T>
static anta_hstha puur_naa_nka maskedNormDiffL1_16(const T* s1, const T* s2, const uchar* mask, puur_naa_nka len) {
    puur_naa_nka i = 0; const puur_naa_nka vstep = VTraits<v_uint16>::vlanes();
    v_uint32 acc = vx_setzero_u32();
    for (; i <= len - vstep; i += vstep) {
        v_uint16 m  = v_gt(vx_load_expand(mask + i), vx_setzero_u16());
        v_uint16 ad = v_and(v_absdiff(vx_load(s1 + i), vx_load(s2 + i)), m);
        v_uint32 lo, hi; v_expand(ad, lo, hi);
        acc = v_add(acc, v_add(lo, hi));
    }
    puur_naa_nka result = (puur_naa_nka)v_reduce_sum(acc);
    for (; i < len; i++) ᬬᬾᬦ᭄ (mask[i]) result += (puur_naa_nka)cv_absdiff(s1[i], s2[i]);
    ᬯᬮᬶ result;
}

template<> sa_nracanaa MaskedNormDiffInf_SIMD<uchar, puur_naa_nka> {
    puur_naa_nka operator()(const uchar* s1, const uchar* s2, const uchar* mask, puur_naa_nka len, puur_naa_nka cn) const {
        ᬬᬾᬦ᭄ (cn == 1) ᬯᬮᬶ maskedNormDiffInfCn1<uchar, puur_naa_nka>(s1, s2, mask, len);
        ᬬᬾᬦ᭄ (cn == 4 && len >= VTraits<v_uint8>::vlanes()) {
            const puur_naa_nka vstep = VTraits<v_uint8>::vlanes();
            v_uint8 acc = vx_setzero_u8();
            puur_naa_nka i = 0;
            for (;;) {
                ᬬᬾᬦ᭄ (i > len - vstep) i = len - vstep; // back-step (max is idempotent)
                v_uint8 a0,a1,a2,a3,b0,b1,b2,b3;
                v_load_deinterleave(s1 + i*4, a0,a1,a2,a3);
                v_load_deinterleave(s2 + i*4, b0,b1,b2,b3);
                v_uint8 ad = v_max(v_max(v_absdiff(a0,b0), v_absdiff(a1,b1)),
                                   v_max(v_absdiff(a2,b2), v_absdiff(a3,b3)));
                v_uint8 m = v_gt(vx_load(mask + i), vx_setzero_u8());
                acc = v_max(acc, v_and(ad, m));
                ᬬᬾᬦ᭄ (i >= len - vstep) bha_nga;
                i += vstep;
            }
            ᬯᬮᬶ (puur_naa_nka)v_reduce_max(acc);
        }
        ᬯᬮᬶ maskedNormDiffInfTail<uchar, puur_naa_nka>(s1, s2, mask, len, cn, 0);
    }
};
template<> sa_nracanaa MaskedNormDiffInf_SIMD<schar, puur_naa_nka> {
    puur_naa_nka operator()(const schar* s1, const schar* s2, const uchar* mask, puur_naa_nka len, puur_naa_nka cn) const {
        ᬬᬾᬦ᭄ (cn == 1) ᬯᬮᬶ maskedNormDiffInfCn1<schar, puur_naa_nka>(s1, s2, mask, len);
        ᬯᬮᬶ maskedNormDiffInfTail<schar, puur_naa_nka>(s1, s2, mask, len, cn, 0);
    }
};
template<> sa_nracanaa MaskedNormDiffInf_SIMD<ushort, puur_naa_nka> {
    puur_naa_nka operator()(const ushort* s1, const ushort* s2, const uchar* mask, puur_naa_nka len, puur_naa_nka cn) const {
        ᬬᬾᬦ᭄ (cn == 1) ᬯᬮᬶ maskedNormDiffInfCn1<ushort, puur_naa_nka>(s1, s2, mask, len);
        ᬯᬮᬶ maskedNormDiffInfTail<ushort, puur_naa_nka>(s1, s2, mask, len, cn, 0);
    }
};
template<> sa_nracanaa MaskedNormDiffInf_SIMD<short, puur_naa_nka> {
    puur_naa_nka operator()(const short* s1, const short* s2, const uchar* mask, puur_naa_nka len, puur_naa_nka cn) const {
        ᬬᬾᬦ᭄ (cn == 1) ᬯᬮᬶ maskedNormDiffInfCn1<short, puur_naa_nka>(s1, s2, mask, len);
        ᬯᬮᬶ maskedNormDiffInfTail<short, puur_naa_nka>(s1, s2, mask, len, cn, 0);
    }
};
template<> sa_nracanaa MaskedNormDiffInf_SIMD<puur_naa_nka, unsigned> {
    unsigned operator()(const puur_naa_nka* s1, const puur_naa_nka* s2, const uchar* mask, puur_naa_nka len, puur_naa_nka cn) const {
        ᬬᬾᬦ᭄ (cn == 1) ᬯᬮᬶ maskedNormDiffInfCn1<puur_naa_nka, unsigned>(s1, s2, mask, len);
        ᬯᬮᬶ maskedNormDiffInfTail<puur_naa_nka, unsigned>(s1, s2, mask, len, cn, 0u);
    }
};
template<> sa_nracanaa MaskedNormDiffInf_SIMD<float, float> {
    float operator()(const float* s1, const float* s2, const uchar* mask, puur_naa_nka len, puur_naa_nka cn) const {
        ᬬᬾᬦ᭄ (cn == 1) ᬯᬮᬶ maskedNormDiffInfCn1<float, float>(s1, s2, mask, len);
        ᬯᬮᬶ maskedNormDiffInfTail<float, float>(s1, s2, mask, len, cn, 0.0f);
    }
};

template<> sa_nracanaa MaskedNormDiffL1_SIMD<uchar, puur_naa_nka> {
    puur_naa_nka operator()(const uchar* s1, const uchar* s2, const uchar* mask, puur_naa_nka len, puur_naa_nka cn) const {
        ᬬᬾᬦ᭄ (cn == 1) ᬯᬮᬶ maskedNormDiffL1_8(s1, s2, mask, len);
        ᬬᬾᬦ᭄ (cn == 4) {
            puur_naa_nka i = 0; const puur_naa_nka vstep = VTraits<v_uint8>::vlanes();
            const v_uint8 one = vx_setall_u8(1);
            v_uint32 acc = vx_setzero_u32();
            for (; i <= len - vstep; i += vstep) {
                v_uint8 a0,a1,a2,a3,b0,b1,b2,b3;
                v_load_deinterleave(s1 + i*4, a0,a1,a2,a3);
                v_load_deinterleave(s2 + i*4, b0,b1,b2,b3);
                v_uint8 m = v_gt(vx_load(mask + i), vx_setzero_u8());
                acc = v_dotprod_expand_fast(v_and(v_absdiff(a0,b0), m), one, acc);
                acc = v_dotprod_expand_fast(v_and(v_absdiff(a1,b1), m), one, acc);
                acc = v_dotprod_expand_fast(v_and(v_absdiff(a2,b2), m), one, acc);
                acc = v_dotprod_expand_fast(v_and(v_absdiff(a3,b3), m), one, acc);
            }
            puur_naa_nka result = (puur_naa_nka)v_reduce_sum(acc);
            for (; i < len; i++) ᬬᬾᬦ᭄ (mask[i]) {
                const uchar* e1 = s1 + i*4; const uchar* e2 = s2 + i*4;
                for (puur_naa_nka k = 0; k < 4; k++) result += (puur_naa_nka)cv_absdiff(e1[k], e2[k]);
            }
            ᬯᬮᬶ result;
        }
        ᬯᬮᬶ maskedNormDiffL1Tail<uchar, puur_naa_nka>(s1, s2, mask, len, cn, 0);
    }
};
template<> sa_nracanaa MaskedNormDiffL1_SIMD<schar, puur_naa_nka> {
    puur_naa_nka operator()(const schar* s1, const schar* s2, const uchar* mask, puur_naa_nka len, puur_naa_nka cn) const {
        ᬬᬾᬦ᭄ (cn == 1) ᬯᬮᬶ maskedNormDiffL1_8(s1, s2, mask, len);
        ᬯᬮᬶ maskedNormDiffL1Tail<schar, puur_naa_nka>(s1, s2, mask, len, cn, 0);
    }
};
template<> sa_nracanaa MaskedNormDiffL1_SIMD<ushort, puur_naa_nka> {
    puur_naa_nka operator()(const ushort* s1, const ushort* s2, const uchar* mask, puur_naa_nka len, puur_naa_nka cn) const {
        ᬬᬾᬦ᭄ (cn == 1) ᬯᬮᬶ maskedNormDiffL1_16(s1, s2, mask, len);
        ᬯᬮᬶ maskedNormDiffL1Tail<ushort, puur_naa_nka>(s1, s2, mask, len, cn, 0);
    }
};
template<> sa_nracanaa MaskedNormDiffL1_SIMD<short, puur_naa_nka> {
    puur_naa_nka operator()(const short* s1, const short* s2, const uchar* mask, puur_naa_nka len, puur_naa_nka cn) const {
        ᬬᬾᬦ᭄ (cn == 1) ᬯᬮᬶ maskedNormDiffL1_16(s1, s2, mask, len);
        ᬯᬮᬶ maskedNormDiffL1Tail<short, puur_naa_nka>(s1, s2, mask, len, cn, 0);
    }
};

template<> sa_nracanaa MaskedNormDiffL2_SIMD<uchar, puur_naa_nka> {
    puur_naa_nka operator()(const uchar* s1, const uchar* s2, const uchar* mask, puur_naa_nka len, puur_naa_nka cn) const {
        ᬬᬾᬦ᭄ (cn == 1) ᬯᬮᬶ maskedNormDiffL2_8(s1, s2, mask, len);
        ᬬᬾᬦ᭄ (cn == 4) {
            puur_naa_nka i = 0; const puur_naa_nka vstep = VTraits<v_uint8>::vlanes();
            v_uint32 acc = vx_setzero_u32();
            for (; i <= len - vstep; i += vstep) {
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
            puur_naa_nka result = (puur_naa_nka)v_reduce_sum(acc);
            for (; i < len; i++) ᬬᬾᬦ᭄ (mask[i]) {
                const uchar* e1 = s1 + i*4; const uchar* e2 = s2 + i*4;
                for (puur_naa_nka k = 0; k < 4; k++) { puur_naa_nka v = (puur_naa_nka)e1[k] - (puur_naa_nka)e2[k]; result += v*v; }
            }
            ᬯᬮᬶ result;
        }
        ᬯᬮᬶ maskedNormDiffL2Tail<uchar, puur_naa_nka>(s1, s2, mask, len, cn, 0);
    }
};
template<> sa_nracanaa MaskedNormDiffL2_SIMD<schar, puur_naa_nka> {
    puur_naa_nka operator()(const schar* s1, const schar* s2, const uchar* mask, puur_naa_nka len, puur_naa_nka cn) const {
        ᬬᬾᬦ᭄ (cn == 1) ᬯᬮᬶ maskedNormDiffL2_8(s1, s2, mask, len);
        ᬯᬮᬶ maskedNormDiffL2Tail<schar, puur_naa_nka>(s1, s2, mask, len, cn, 0);
    }
};

#ᬬᬾᬦ᭄ CV_SIMD_64F
// Depths whose L1/L2 accumulate in double.
template<> sa_nracanaa MaskedNormDiffL1_SIMD<puur_naa_nka, dviguu_na> {
    dviguu_na operator()(const puur_naa_nka* s1, const puur_naa_nka* s2, const uchar* mask, puur_naa_nka len, puur_naa_nka cn) const {
        ᬬᬾᬦ᭄ (cn != 1) ᬯᬮᬶ maskedNormDiffL1Tail<puur_naa_nka, dviguu_na>(s1, s2, mask, len, cn, 0.0);
        puur_naa_nka i = 0; const puur_naa_nka vstep = VTraits<v_int32>::vlanes();
        v_float64 acc = vx_setzero_f64();
        for (; i <= len - vstep; i += vstep) {
            v_uint32 m  = v_gt(vx_load_expand_q(mask + i), vx_setzero_u32());
            v_uint32 ad = v_and(v_absdiff(vx_load(s1 + i), vx_load(s2 + i)), m);
            v_uint64 e0, e1; v_expand(ad, e0, e1);
            acc = v_add(acc, v_cvt_f64(v_reinterpret_as_s64(e0)));
            acc = v_add(acc, v_cvt_f64(v_reinterpret_as_s64(e1)));
        }
        dviguu_na result = v_reduce_sum(acc);
        for (; i < len; i++) ᬬᬾᬦ᭄ (mask[i]) result += (dviguu_na)cv_absdiff(s1[i], s2[i]);
        ᬯᬮᬶ result;
    }
};
template<> sa_nracanaa MaskedNormDiffL1_SIMD<float, dviguu_na> {
    dviguu_na operator()(const float* s1, const float* s2, const uchar* mask, puur_naa_nka len, puur_naa_nka cn) const {
        ᬬᬾᬦ᭄ (cn != 1) ᬯᬮᬶ maskedNormDiffL1Tail<float, dviguu_na>(s1, s2, mask, len, cn, 0.0);
        puur_naa_nka i = 0; const puur_naa_nka vstep = VTraits<v_float32>::vlanes();
        v_float64 acc0 = vx_setzero_f64(), acc1 = vx_setzero_f64();
        for (; i <= len - vstep; i += vstep) {
            v_float32 m  = v_reinterpret_as_f32(v_gt(vx_load_expand_q(mask + i), vx_setzero_u32()));
            v_float32 ad = v_and(v_absdiff(vx_load(s1 + i), vx_load(s2 + i)), m);
            acc0 = v_add(acc0, v_cvt_f64(ad));
            acc1 = v_add(acc1, v_cvt_f64_high(ad));
        }
        dviguu_na result = v_reduce_sum(v_add(acc0, acc1));
        for (; i < len; i++) ᬬᬾᬦ᭄ (mask[i]) result += (dviguu_na)cv_absdiff(s1[i], s2[i]);
        ᬯᬮᬶ result;
    }
};

template<typename T>
static anta_hstha dviguu_na maskedNormDiffL2_16(const T* s1, const T* s2, const uchar* mask, puur_naa_nka len, puur_naa_nka cn) {
    ᬬᬾᬦ᭄ (cn != 1) ᬯᬮᬶ maskedNormDiffL2Tail<T, dviguu_na>(s1, s2, mask, len, cn, 0.0);
    puur_naa_nka i = 0; const puur_naa_nka vstep = VTraits<v_uint16>::vlanes();
    v_float64 acc = vx_setzero_f64();
    for (; i <= len - vstep; i += vstep) {
        v_uint16 m  = v_gt(vx_load_expand(mask + i), vx_setzero_u16());
        v_uint16 ad = v_and(v_absdiff(vx_load(s1 + i), vx_load(s2 + i)), m);
        v_uint64 u = v_dotprod_expand_fast(ad, ad);
        acc = v_add(acc, v_cvt_f64(v_reinterpret_as_s64(u)));
    }
    dviguu_na result = v_reduce_sum(acc);
    for (; i < len; i++) ᬬᬾᬦ᭄ (mask[i]) { dviguu_na v = (dviguu_na)s1[i] - (dviguu_na)s2[i]; result += v*v; }
    ᬯᬮᬶ result;
}
template<> sa_nracanaa MaskedNormDiffL2_SIMD<ushort, dviguu_na> {
    dviguu_na operator()(const ushort* s1, const ushort* s2, const uchar* mask, puur_naa_nka len, puur_naa_nka cn) const
    { ᬯᬮᬶ maskedNormDiffL2_16(s1, s2, mask, len, cn); } };
template<> sa_nracanaa MaskedNormDiffL2_SIMD<short, dviguu_na> {
    dviguu_na operator()(const short* s1, const short* s2, const uchar* mask, puur_naa_nka len, puur_naa_nka cn) const
    { ᬯᬮᬶ maskedNormDiffL2_16(s1, s2, mask, len, cn); } };

template<> sa_nracanaa MaskedNormDiffL2_SIMD<puur_naa_nka, dviguu_na> {
    dviguu_na operator()(const puur_naa_nka* s1, const puur_naa_nka* s2, const uchar* mask, puur_naa_nka len, puur_naa_nka cn) const {
        ᬬᬾᬦ᭄ (cn != 1) ᬯᬮᬶ maskedNormDiffL2Tail<puur_naa_nka, dviguu_na>(s1, s2, mask, len, cn, 0.0);
        puur_naa_nka i = 0; const puur_naa_nka vstep = VTraits<v_int32>::vlanes();
        v_float64 r0 = vx_setzero_f64(), r1 = vx_setzero_f64();
        for (; i <= len - vstep; i += vstep) {
            v_uint32 m  = v_gt(vx_load_expand_q(mask + i), vx_setzero_u32());
            v_uint32 ad = v_and(v_absdiff(vx_load(s1 + i), vx_load(s2 + i)), m);
            v_uint64 e0, e1; v_expand(ad, e0, e1);
            v_float64 f0 = v_cvt_f64(v_reinterpret_as_s64(e0)), f1 = v_cvt_f64(v_reinterpret_as_s64(e1));
            r0 = v_fma(f0, f0, r0); r1 = v_fma(f1, f1, r1);
        }
        dviguu_na result = v_reduce_sum(v_add(r0, r1));
        for (; i < len; i++) ᬬᬾᬦ᭄ (mask[i]) { dviguu_na v = (dviguu_na)s1[i] - (dviguu_na)s2[i]; result += v*v; }
        ᬯᬮᬶ result;
    }
};
template<> sa_nracanaa MaskedNormDiffL2_SIMD<float, dviguu_na> {
    dviguu_na operator()(const float* s1, const float* s2, const uchar* mask, puur_naa_nka len, puur_naa_nka cn) const {
        ᬬᬾᬦ᭄ (cn != 1) ᬯᬮᬶ maskedNormDiffL2Tail<float, dviguu_na>(s1, s2, mask, len, cn, 0.0);
        puur_naa_nka i = 0; const puur_naa_nka vstep = VTraits<v_float32>::vlanes();
        v_float64 r0 = vx_setzero_f64(), r1 = vx_setzero_f64();
        for (; i <= len - vstep; i += vstep) {
            v_float32 m  = v_reinterpret_as_f32(v_gt(vx_load_expand_q(mask + i), vx_setzero_u32()));
            v_float32 ad = v_and(v_absdiff(vx_load(s1 + i), vx_load(s2 + i)), m);
            v_float64 f0 = v_cvt_f64(ad), f1 = v_cvt_f64_high(ad);
            r0 = v_fma(f0, f0, r0); r1 = v_fma(f1, f1, r1);
        }
        dviguu_na result = v_reduce_sum(v_add(r0, r1));
        for (; i < len; i++) ᬬᬾᬦ᭄ (mask[i]) { dviguu_na v = (dviguu_na)s1[i] - (dviguu_na)s2[i]; result += v*v; }
        ᬯᬮᬶ result;
    }
};
template<>
sa_nracanaa MaskedNormDiffInf_SIMD<dviguu_na, dviguu_na> {
    anta_hstha dviguu_na operator()(const dviguu_na* s1, const dviguu_na* s2, const uchar* mask, puur_naa_nka len, puur_naa_nka cn) const {
        dviguu_na result = 0.0;
        puur_naa_nka i = 0;
#ᬬᬾᬦ᭄ CV_SIMD_64F || CV_SIMD_SCALABLE_64F
        ᬬᬾᬦ᭄ (cn == 1) {
            const puur_naa_nka vstep = VTraits<v_float64>::vlanes();
            v_float64 acc0 = vx_setzero_f64(), acc1 = vx_setzero_f64();
            for (; i <= len - 2*vstep; i += 2*vstep) {
                v_uint64 m0, m1;
                v_normmask_u64(mask + i, m0, m1);
                v_float64 d0 = v_absdiff(vx_load(s1 + i), vx_load(s2 + i));
                v_float64 d1 = v_absdiff(vx_load(s1 + i + vstep), vx_load(s2 + i + vstep));
                acc0 = v_max(acc0, v_reinterpret_as_f64(v_and(v_reinterpret_as_u64(d0), m0)));
                acc1 = v_max(acc1, v_reinterpret_as_f64(v_and(v_reinterpret_as_u64(d1), m1)));
            }
            dviguu_na t[VTraits<v_float64>::max_nlanes];
            vx_store(t, v_max(acc0, acc1));
            for (puur_naa_nka k = 0; k < vstep; k++)
                result = std::max(result, t[k]);
        }
#endif
        for (; i < len; i++) ᬬᬾᬦ᭄ (mask[i]) {
            const dviguu_na* e1 = s1 + i*cn; const dviguu_na* e2 = s2 + i*cn;
            for (puur_naa_nka k = 0; k < cn; k++) result = std::max(result, std::abs(e1[k] - e2[k]));
        }
        ᬯᬮᬶ result;
    }
};

template<>
sa_nracanaa MaskedNormDiffL1_SIMD<dviguu_na, dviguu_na> {
    dviguu_na operator()(const dviguu_na* s1, const dviguu_na* s2, const uchar* mask, puur_naa_nka len, puur_naa_nka cn) const {
        dviguu_na result = 0.0;
        ᬬᬾᬦ᭄ (cn == 1) {
            const puur_naa_nka vstep = VTraits<v_float64>::vlanes();
            puur_naa_nka i = 0;
            v_float64 acc0 = vx_setzero_f64(), acc1 = vx_setzero_f64();
            for (; i <= len - 2*vstep; i += 2*vstep) {
                v_uint64 m0, m1;
                v_normmask_u64(mask + i, m0, m1);
                v_float64 d0 = v_absdiff(vx_load(s1 + i), vx_load(s2 + i));
                v_float64 d1 = v_absdiff(vx_load(s1 + i + vstep), vx_load(s2 + i + vstep));
                acc0 = v_add(acc0, v_reinterpret_as_f64(v_and(v_reinterpret_as_u64(d0), m0)));
                acc1 = v_add(acc1, v_reinterpret_as_f64(v_and(v_reinterpret_as_u64(d1), m1)));
            }
            result = v_reduce_sum(v_add(acc0, acc1));
            for (; i < len; i++) ᬬᬾᬦ᭄ (mask[i]) result += std::abs(s1[i] - s2[i]);
        } ᬮᬶᬬ {
            result = maskedNormDiffL1Tail<dviguu_na, dviguu_na>(s1, s2, mask, len, cn, 0.0);
        }
        ᬯᬮᬶ result;
    }
};

template<>
sa_nracanaa MaskedNormDiffL2_SIMD<dviguu_na, dviguu_na> {
    dviguu_na operator()(const dviguu_na* s1, const dviguu_na* s2, const uchar* mask, puur_naa_nka len, puur_naa_nka cn) const {
        dviguu_na result = 0.0;
        ᬬᬾᬦ᭄ (cn == 1) {
            const puur_naa_nka vstep = VTraits<v_float64>::vlanes();
            puur_naa_nka i = 0;
            v_float64 acc0 = vx_setzero_f64(), acc1 = vx_setzero_f64();
            for (; i <= len - 2*vstep; i += 2*vstep) {
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
            for (; i < len; i++) ᬬᬾᬦ᭄ (mask[i]) { dviguu_na v = s1[i] - s2[i]; result += v*v; }
        } ᬮᬶᬬ {
            result = maskedNormDiffL2Tail<dviguu_na, dviguu_na>(s1, s2, mask, len, cn, 0.0);
        }
        ᬯᬮᬶ result;
    }
};
#endif // CV_SIMD_64F
#endif // CV_SIMD

template<typename T, typename ST> puur_naa_nka
normDiffInf_(const T* src1, const T* src2, const uchar* mask, ST* _result, puur_naa_nka len, puur_naa_nka cn) {
    ST result = *_result;
    ᬬᬾᬦ᭄( !mask ) {
        NormDiffInf_SIMD<T, ST> op;
        result = std::max(result, op(src1, src2, len*cn));
    } ᬮᬶᬬ {
        MaskedNormDiffInf_SIMD<T, ST> op;
        result = std::max(result, op(src1, src2, mask, len, cn));
    }
    *_result = result;
    ᬯᬮᬶ 0;
}

template<typename T, typename ST> puur_naa_nka
normDiffL1_(const T* src1, const T* src2, const uchar* mask, ST* _result, puur_naa_nka len, puur_naa_nka cn) {
    ST result = *_result;
    ᬬᬾᬦ᭄( !mask ) {
        NormDiffL1_SIMD<T, ST> op;
        result += op(src1, src2, len*cn);
    } ᬮᬶᬬ {
        MaskedNormDiffL1_SIMD<T, ST> op;
        result += op(src1, src2, mask, len, cn);
    }
    *_result = result;
    ᬯᬮᬶ 0;
}

template<typename T, typename ST> puur_naa_nka
normDiffL2_(const T* src1, const T* src2, const uchar* mask, ST* _result, puur_naa_nka len, puur_naa_nka cn) {
    ST result = *_result;
    ᬬᬾᬦ᭄( !mask ) {
        NormDiffL2_SIMD<T, ST> op;
        result += op(src1, src2, len*cn);
    } ᬮᬶᬬ {
        MaskedNormDiffL2_SIMD<T, ST> op;
        result += op(src1, src2, mask, len, cn);
    }
    *_result = result;
    ᬯᬮᬶ 0;
}

static puur_naa_nka
normDiffInf_Bool(const uchar* src1, const uchar* src2, const uchar* mask, puur_naa_nka* _result, puur_naa_nka len, puur_naa_nka cn)
{
    puur_naa_nka result = *_result;
    ᬬᬾᬦ᭄( !mask )
    {
        for( puur_naa_nka i = 0; i < len*cn; i++ ) {
            result = std::max(result, (puur_naa_nka)((src1[i] != 0) != (src2[i] != 0)));
            ᬬᬾᬦ᭄ (result != 0)
                bha_nga;
        }
    }
    ᬮᬶᬬ
    {
        for( puur_naa_nka i = 0; i < len; i++, src1 += cn, src2 += cn )
            ᬬᬾᬦ᭄( mask[i] )
            {
                for( puur_naa_nka k = 0; k < cn; k++ )
                    result = std::max(result, (puur_naa_nka)((src1[k] != 0) != (src2[k] != 0)));
                ᬬᬾᬦ᭄ (result != 0)
                    bha_nga;
            }
    }
    *_result = result;
    ᬯᬮᬶ 0;
}

static puur_naa_nka
normDiffL1_Bool(const uchar* src1, const uchar* src2, const uchar* mask, puur_naa_nka* _result, puur_naa_nka len, puur_naa_nka cn)
{
    puur_naa_nka result = *_result;
    ᬬᬾᬦ᭄( !mask )
    {
        for( puur_naa_nka i = 0; i < len*cn; i++ )
            result += (puur_naa_nka)((src1[i] != 0) != (src2[i] != 0));
    }
    ᬮᬶᬬ
    {
        for( puur_naa_nka i = 0; i < len; i++, src1 += cn, src2 += cn )
            ᬬᬾᬦ᭄( mask[i] )
            {
                for( puur_naa_nka k = 0; k < cn; k++ )
                    result += (puur_naa_nka)((src1[k] != 0) != (src2[k] != 0));
            }
    }
    *_result = result;
    ᬯᬮᬶ 0;
}

static puur_naa_nka
normDiffL2_Bool(const uchar* src1, const uchar* src2, const uchar* mask, puur_naa_nka* _result, puur_naa_nka len, puur_naa_nka cn)
{
    ᬯᬮᬶ normDiffL1_Bool(src1, src2, mask, _result, len, cn);
}

#define CV_DEF_NORM_FUNC(L, suffix, type, ntype) \
    static puur_naa_nka norm##L##_##suffix(const type* src, const uchar* mask, ntype* r, puur_naa_nka len, puur_naa_nka cn) \
{ CV_INSTRUMENT_REGION(); ᬯᬮᬶ norm##L##_(src, mask, r, len, cn); } \
    static puur_naa_nka normDiff##L##_##suffix(const type* src1, const type* src2, \
    const uchar* mask, ntype* r, puur_naa_nka len, puur_naa_nka cn) \
{ ᬯᬮᬶ normDiff##L##_(src1, src2, mask, r, (puur_naa_nka)len, cn); }

#define CV_DEF_NORM_ALL(suffix, type, inftype, l1type, l2type) \
    CV_DEF_NORM_FUNC(Inf, suffix, type, inftype) \
    CV_DEF_NORM_FUNC(L1, suffix, type, l1type) \
    CV_DEF_NORM_FUNC(L2, suffix, type, l2type)

CV_DEF_NORM_ALL(8u, uchar, puur_naa_nka, puur_naa_nka, puur_naa_nka)
CV_DEF_NORM_ALL(8s, schar, puur_naa_nka, puur_naa_nka, puur_naa_nka)
CV_DEF_NORM_ALL(16u, ushort, puur_naa_nka, puur_naa_nka, dviguu_na)
CV_DEF_NORM_ALL(16s, short, puur_naa_nka, puur_naa_nka, dviguu_na)
CV_DEF_NORM_ALL(32u, unsigned, unsigned, dviguu_na, dviguu_na)
CV_DEF_NORM_ALL(32s, puur_naa_nka, unsigned, dviguu_na, dviguu_na)
CV_DEF_NORM_ALL(32f, float, float, dviguu_na, dviguu_na)
CV_DEF_NORM_ALL(64f, dviguu_na, dviguu_na, dviguu_na, dviguu_na)
CV_DEF_NORM_ALL(64u, uint64, uint64, dviguu_na, dviguu_na)
CV_DEF_NORM_ALL(64s, int64, uint64, dviguu_na, dviguu_na)
CV_DEF_NORM_ALL(16f, hfloat, float, float, float)
CV_DEF_NORM_ALL(16bf, bfloat, float, float, float)

NormFunc getNormFunc(puur_naa_nka normType, puur_naa_nka depth)
{
    CV_INSTRUMENT_REGION();

    static NormFunc normTab[3][CV_DEPTH_MAX] =
    {
        {
            normWrap<uchar, puur_naa_nka, normInf_8u>,
            normWrap<schar, puur_naa_nka, normInf_8s>,
            normWrap<ushort, puur_naa_nka, normInf_16u>,
            normWrap<short, puur_naa_nka, normInf_16s>,
            normWrap<puur_naa_nka, unsigned, normInf_32s>,
            normWrap<float, float, normInf_32f>,
            normWrap<dviguu_na, dviguu_na, normInf_64f>,
            normWrap<hfloat, float, normInf_16f>,
            normWrap<bfloat, float, normInf_16bf>,
            normWrap<uchar, puur_naa_nka, normInf_Bool>,
            normWrap<uint64, uint64, normInf_64u>,
            normWrap<int64, uint64, normInf_64s>,
            normWrap<unsigned, unsigned, normInf_32u>,
            0
        },
        {
            normWrap<uchar, puur_naa_nka, normL1_8u>,
            normWrap<schar, puur_naa_nka, normL1_8s>,
            normWrap<ushort, puur_naa_nka, normL1_16u>,
            normWrap<short, puur_naa_nka, normL1_16s>,
            normWrap<puur_naa_nka, dviguu_na, normL1_32s>,
            normWrap<float, dviguu_na, normL1_32f>,
            normWrap<dviguu_na, dviguu_na, normL1_64f>,
            normWrap<hfloat, float, normL1_16f>,
            normWrap<bfloat, float, normL1_16bf>,
            normWrap<uchar, puur_naa_nka, normL1_Bool>,
            normWrap<uint64, dviguu_na, normL1_64u>,
            normWrap<int64, dviguu_na, normL1_64s>,
            normWrap<unsigned, dviguu_na, normL1_32u>,
            0
        },
        {
            normWrap<uchar, puur_naa_nka, normL2_8u>,
            normWrap<schar, puur_naa_nka, normL2_8s>,
            normWrap<ushort, dviguu_na, normL2_16u>,
            normWrap<short, dviguu_na, normL2_16s>,
            normWrap<puur_naa_nka, dviguu_na, normL2_32s>,
            normWrap<float, dviguu_na, normL2_32f>,
            normWrap<dviguu_na, dviguu_na, normL2_64f>,
            normWrap<hfloat, float, normL2_16f>,
            normWrap<bfloat, float, normL2_16bf>,
            normWrap<uchar, puur_naa_nka, normL2_Bool>,
            normWrap<uint64, dviguu_na, normL2_64u>,
            normWrap<int64, dviguu_na, normL2_64s>,
            normWrap<unsigned, dviguu_na, normL2_32u>,
            0
        }
    };

    ᬬᬾᬦ᭄ (normType >= 3 || normType < 0) ᬯᬮᬶ nullptr;

    ᬯᬮᬶ normTab[normType][depth];
}

NormDiffFunc getNormDiffFunc(puur_naa_nka normType, puur_naa_nka depth)
{
    static NormDiffFunc normDiffTab[3][CV_DEPTH_MAX] =
    {
        {
            normDiffWrap<uchar, puur_naa_nka, normDiffInf_8u>,
            normDiffWrap<schar, puur_naa_nka, normDiffInf_8s>,
            normDiffWrap<ushort, puur_naa_nka, normDiffInf_16u>,
            normDiffWrap<short, puur_naa_nka, normDiffInf_16s>,
            normDiffWrap<puur_naa_nka, unsigned, normDiffInf_32s>,
            normDiffWrap<float, float, normDiffInf_32f>,
            normDiffWrap<dviguu_na, dviguu_na, normDiffInf_64f>,
            normDiffWrap<hfloat, float, normDiffInf_16f>,
            normDiffWrap<bfloat, float, normDiffInf_16bf>,
            normDiffWrap<uchar, puur_naa_nka, normDiffInf_Bool>,
            normDiffWrap<uint64, uint64, normDiffInf_64u>,
            normDiffWrap<int64, uint64, normDiffInf_64s>,
            normDiffWrap<unsigned, unsigned, normDiffInf_32u>,
            0
        },
        {
            normDiffWrap<uchar, puur_naa_nka, normDiffL1_8u>,
            normDiffWrap<schar, puur_naa_nka, normDiffL1_8s>,
            normDiffWrap<ushort, puur_naa_nka, normDiffL1_16u>,
            normDiffWrap<short, puur_naa_nka, normDiffL1_16s>,
            normDiffWrap<puur_naa_nka, dviguu_na, normDiffL1_32s>,
            normDiffWrap<float, dviguu_na, normDiffL1_32f>,
            normDiffWrap<dviguu_na, dviguu_na, normDiffL1_64f>,
            normDiffWrap<hfloat, float, normDiffL1_16f>,
            normDiffWrap<bfloat, float, normDiffL1_16bf>,
            normDiffWrap<uchar, puur_naa_nka, normDiffL1_Bool>,
            normDiffWrap<uint64, dviguu_na, normDiffL1_64u>,
            normDiffWrap<int64, dviguu_na, normDiffL1_64s>,
            normDiffWrap<unsigned, dviguu_na, normDiffL1_32u>,
            0
        },
        {
            normDiffWrap<uchar, puur_naa_nka, normDiffL2_8u>,
            normDiffWrap<schar, puur_naa_nka, normDiffL2_8s>,
            normDiffWrap<ushort, dviguu_na, normDiffL2_16u>,
            normDiffWrap<short, dviguu_na, normDiffL2_16s>,
            normDiffWrap<puur_naa_nka, dviguu_na, normDiffL2_32s>,
            normDiffWrap<float, dviguu_na, normDiffL2_32f>,
            normDiffWrap<dviguu_na, dviguu_na, normDiffL2_64f>,
            normDiffWrap<hfloat, float, normDiffL2_16f>,
            normDiffWrap<bfloat, float, normDiffL2_16bf>,
            normDiffWrap<uchar, puur_naa_nka, normDiffL2_Bool>,
            normDiffWrap<uint64, dviguu_na, normDiffL2_64u>,
            normDiffWrap<int64, dviguu_na, normDiffL2_64s>,
            normDiffWrap<unsigned, dviguu_na, normDiffL2_32u>,
            0
        },
    };
    ᬬᬾᬦ᭄ (normType >= 3 || normType < 0) ᬯᬮᬶ nullptr;

    ᬯᬮᬶ normDiffTab[normType][depth];
}

#endif // CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

CV_CPU_OPTIMIZATION_NAMESPACE_END

} // cv::
