// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html
//
// Copyright (C) 2025, SpaceMIT Inc., all rights reserved.
// Copyright (C) 2026, Advanced Micro Devices, Inc., all rights reserved.
// Third party copyrights are property of their respective owners.

#include "precomp.hpp"

namespace cv {

using NormFunc = số_nguyên (*)(const uchar*, const uchar*, uchar*, số_nguyên, số_nguyên);
using NormDiffFunc = số_nguyên (*)(const uchar*, const uchar*, const uchar*, uchar*, số_nguyên, số_nguyên);

CV_CPU_OPTIMIZATION_NAMESPACE_BEGIN

template<typename T, typename RT, số_nguyên (*fn)(const T*, const uchar*, RT*, số_nguyên, số_nguyên)>
static số_nguyên normWrap(const uchar* src1, const uchar* src2, uchar* res, số_nguyên len, số_nguyên cn)
{
    trả_về fn((const T*)src1, src2, (RT*)res, len, cn);
}

template<typename T, typename RT, số_nguyên (*fn)(const T*, const T*, const uchar*, RT*, số_nguyên, số_nguyên)>
static số_nguyên normDiffWrap(const uchar* src1, const uchar* src2, const uchar* src3, uchar* res, số_nguyên len, số_nguyên cn)
{
    trả_về fn((const T*)src1, (const T*)src2, src3, (RT*)res, len, cn);
}

NormFunc getNormFunc(số_nguyên normType, số_nguyên depth);
NormDiffFunc getNormDiffFunc(số_nguyên normType, số_nguyên depth);

#ifndef CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

template <typename T, typename ST>
struct NormInf_SIMD {
    inline ST operator() (const T* src, số_nguyên n) const {
        ST s = 0;
        cho (số_nguyên i = 0; i < n; i++) {
            s = std::max(s, (ST)cv_abs(src[i]));
        }
        trả_về s;
    }
};

template <typename T, typename ST>
struct NormL1_SIMD {
    inline ST operator() (const T* src, số_nguyên n) const {
        ST s = 0;
        cho (số_nguyên i = 0; i < n; i++) {
            s += cv_abs(src[i]);
        }
        trả_về s;
    }
};

template <typename T, typename ST>
struct NormL2_SIMD {
    inline ST operator() (const T* src, số_nguyên n) const {
        ST s = 0;
        cho (số_nguyên i = 0; i < n; i++) {
            ST v = (ST)src[i];
            s += v * v;
        }
        trả_về s;
    }
};

template <typename T, typename ST>
struct NormDiffInf_SIMD {
    inline ST operator() (const T* src1, const T* src2, số_nguyên n) const {
        ST s = 0;
        cho (số_nguyên i = 0; i < n; i++) {
            ST v = (ST)cv_absdiff(src1[i], src2[i]);
            s = std::max(s, v);
        }
        trả_về s;
    }
};

template <typename T, typename ST>
struct NormDiffL1_SIMD {
    inline ST operator() (const T* src1, const T* src2, số_nguyên n) const {
        ST s = 0;
        cho (số_nguyên i = 0; i < n; i++) {
            ST v = (ST)cv_absdiff(src1[i], src2[i]);
            s += v;
        }
        trả_về s;
    }
};

template <typename T, typename ST>
struct NormDiffL2_SIMD {
    inline ST operator() (const T* src1, const T* src2, số_nguyên n) const {
        ST s = 0;
        cho (số_nguyên i = 0; i < n; i++) {
            ST v = (ST)src1[i] - (ST)src2[i];
            s += v * v;
        }
        trả_về s;
    }
};

#nếu (CV_SIMD || CV_SIMD_SCALABLE)

template<>
struct NormInf_SIMD<uchar, số_nguyên> {
    số_nguyên operator() (const uchar* src, số_nguyên n) const {
        số_nguyên j = 0;
        số_nguyên s = 0;
        v_uint8 r0 = vx_setzero_u8(), r1 = vx_setzero_u8();
        v_uint8 r2 = vx_setzero_u8(), r3 = vx_setzero_u8();
        cho (; j <= n - 4 * VTraits<v_uint8>::vlanes(); j += 4 * VTraits<v_uint8>::vlanes()) {
            r0 = v_max(r0, vx_load(src + j                                 ));
            r1 = v_max(r1, vx_load(src + j +     VTraits<v_uint8>::vlanes()));
            r2 = v_max(r2, vx_load(src + j + 2 * VTraits<v_uint8>::vlanes()));
            r3 = v_max(r3, vx_load(src + j + 3 * VTraits<v_uint8>::vlanes()));
        }
        r0 = v_max(r0, v_max(r1, v_max(r2, r3)));
        cho (; j < n; j++) {
            s = std::max(s, (số_nguyên)src[j]);
        }
        trả_về std::max(s, (số_nguyên)v_reduce_max(r0));
    }
};

template<>
struct NormInf_SIMD<schar, số_nguyên> {
    số_nguyên operator() (const schar* src, số_nguyên n) const {
        số_nguyên j = 0;
        số_nguyên s = 0;
        v_uint8 r0 = vx_setzero_u8(), r1 = vx_setzero_u8();
        v_uint8 r2 = vx_setzero_u8(), r3 = vx_setzero_u8();
        cho (; j <= n - 4 * VTraits<v_int8>::vlanes(); j += 4 * VTraits<v_int8>::vlanes()) {
            r0 = v_max(r0, v_abs(vx_load(src + j                                )));
            r1 = v_max(r1, v_abs(vx_load(src + j +     VTraits<v_int8>::vlanes())));
            r2 = v_max(r2, v_abs(vx_load(src + j + 2 * VTraits<v_int8>::vlanes())));
            r3 = v_max(r3, v_abs(vx_load(src + j + 3 * VTraits<v_int8>::vlanes())));
        }
        r0 = v_max(r0, v_max(r1, v_max(r2, r3)));
        cho (; j < n; j++) {
            s = std::max(s, cv_abs(src[j]));
        }
        trả_về std::max(s, saturate_cast<số_nguyên>(v_reduce_max(r0)));
    }
};

template<>
struct NormInf_SIMD<ushort, số_nguyên> {
    số_nguyên operator() (const ushort* src, số_nguyên n) const {
        số_nguyên j = 0;
        số_nguyên s = 0;
        v_uint16 d0 = vx_setzero_u16(), d1 = vx_setzero_u16();
        v_uint16 d2 = vx_setzero_u16(), d3 = vx_setzero_u16();
        cho (; j <= n - 4 * VTraits<v_uint16>::vlanes(); j += 4 * VTraits<v_uint16>::vlanes()) {
            d0 = v_max(d0, vx_load(src + j                                  ));
            d1 = v_max(d1, vx_load(src + j +     VTraits<v_uint16>::vlanes()));
            d2 = v_max(d2, vx_load(src + j + 2 * VTraits<v_uint16>::vlanes()));
            d3 = v_max(d3, vx_load(src + j + 3 * VTraits<v_uint16>::vlanes()));
        }
        d0 = v_max(d0, v_max(d1, v_max(d2, d3)));
        cho (; j < n; j++) {
            s = std::max(s, (số_nguyên)src[j]);
        }
        trả_về std::max(s, (số_nguyên)v_reduce_max(d0));
    }
};

template<>
struct NormInf_SIMD<short, số_nguyên> {
    số_nguyên operator() (const short* src, số_nguyên n) const {
        số_nguyên j = 0;
        số_nguyên s = 0;
        v_uint16 d0 = vx_setzero_u16(), d1 = vx_setzero_u16();
        v_uint16 d2 = vx_setzero_u16(), d3 = vx_setzero_u16();
        cho (; j <= n - 4 * VTraits<v_int16>::vlanes(); j += 4 * VTraits<v_int16>::vlanes()) {
            d0 = v_max(d0, v_abs(vx_load(src + j                                  )));
            d1 = v_max(d1, v_abs(vx_load(src + j +     VTraits<v_int16>::vlanes())));
            d2 = v_max(d2, v_abs(vx_load(src + j + 2 * VTraits<v_int16>::vlanes())));
            d3 = v_max(d3, v_abs(vx_load(src + j + 3 * VTraits<v_int16>::vlanes())));
        }
        d0 = v_max(d0, v_max(d1, v_max(d2, d3)));
        cho (; j < n; j++) {
            s = std::max(s, saturate_cast<số_nguyên>(cv_abs(src[j])));
        }
        trả_về std::max(s, saturate_cast<số_nguyên>(v_reduce_max(d0)));
    }
};

template<>
struct NormInf_SIMD<số_nguyên, số_nguyên> {
    số_nguyên operator() (const số_nguyên* src, số_nguyên n) const {
        số_nguyên j = 0;
        số_nguyên s = 0;
        v_uint32 r0 = vx_setzero_u32(), r1 = vx_setzero_u32();
        v_uint32 r2 = vx_setzero_u32(), r3 = vx_setzero_u32();
        cho (; j <= n - 4 * VTraits<v_int32>::vlanes(); j += 4 * VTraits<v_int32>::vlanes()) {
            r0 = v_max(r0, v_abs(vx_load(src + j                                 )));
            r1 = v_max(r1, v_abs(vx_load(src + j +     VTraits<v_int32>::vlanes())));
            r2 = v_max(r2, v_abs(vx_load(src + j + 2 * VTraits<v_int32>::vlanes())));
            r3 = v_max(r3, v_abs(vx_load(src + j + 3 * VTraits<v_int32>::vlanes())));
        }
        r0 = v_max(r0, v_max(r1, v_max(r2, r3)));
        s = std::max(s, saturate_cast<số_nguyên>(v_reduce_max(r0)));
        cho (; j < n; j++) {
            s = std::max(s, std::abs(src[j]));
        }
        trả_về s;
    }
};

template<>
struct NormInf_SIMD<float, float> {
    float operator() (const float* src, số_nguyên n) const {
        số_nguyên j = 0;
        float s = 0.f;
        v_float32 r0 = vx_setzero_f32(), r1 = vx_setzero_f32();
        v_float32 r2 = vx_setzero_f32(), r3 = vx_setzero_f32();
        cho (; j <= n - 4 * VTraits<v_float32>::vlanes(); j += 4 * VTraits<v_float32>::vlanes()) {
            r0 = v_max(r0, v_abs(vx_load(src + j                                   )));
            r1 = v_max(r1, v_abs(vx_load(src + j +     VTraits<v_float32>::vlanes())));
            r2 = v_max(r2, v_abs(vx_load(src + j + 2 * VTraits<v_float32>::vlanes())));
            r3 = v_max(r3, v_abs(vx_load(src + j + 3 * VTraits<v_float32>::vlanes())));
        }
        r0 = v_max(r0, v_max(r1, v_max(r2, r3)));
        cho (; j < n; j++) {
            s = std::max(s, cv_abs(src[j]));
        }
        trả_về std::max(s, v_reduce_max(r0));
    }
};

template<>
struct NormL1_SIMD<uchar, số_nguyên> {
    số_nguyên operator() (const uchar* src, số_nguyên n) const {
        số_nguyên j = 0;
        số_nguyên s = 0;
        v_uint32 r0 = vx_setzero_u32(), r1 = vx_setzero_u32();
        v_uint8  one = vx_setall_u8(1);
        cho (; j<= n - 2 * VTraits<v_uint8>::vlanes(); j += 2 * VTraits<v_uint8>::vlanes()) {
            v_uint8 v0 = vx_load(src + j);
            r0 = v_dotprod_expand_fast(v0, one, r0);

            v_uint8 v1 = vx_load(src + j + VTraits<v_uint8>::vlanes());
            r1 = v_dotprod_expand_fast(v1, one, r1);
        }
        s += v_reduce_sum(v_add(r0, r1));
        cho (; j < n; j++) {
            s += src[j];
        }
        trả_về s;
    }
};

template<>
struct NormL1_SIMD<schar, số_nguyên> {
    số_nguyên operator() (const schar* src, số_nguyên n) const {
        số_nguyên j = 0;
        số_nguyên s = 0;
        v_uint32 r0 = vx_setzero_u32(), r1 = vx_setzero_u32();
        v_uint8  one = vx_setall_u8(1);
        cho (; j<= n - 2 * VTraits<v_int8>::vlanes(); j += 2 * VTraits<v_int8>::vlanes()) {
            v_uint8 v0 = v_abs(vx_load(src + j));
            r0 = v_dotprod_expand_fast(v0, one, r0);

            v_uint8 v1 = v_abs(vx_load(src + j + VTraits<v_int8>::vlanes()));
            r1 = v_dotprod_expand_fast(v1, one, r1);
        }
        s += v_reduce_sum(v_add(r0, r1));
        cho (; j < n; j++) {
            s += saturate_cast<số_nguyên>(cv_abs(src[j]));
        }
        trả_về s;
    }
};

template<>
struct NormL1_SIMD<ushort, số_nguyên> {
    số_nguyên operator() (const ushort* src, số_nguyên n) const {
        số_nguyên j = 0;
        số_nguyên s = 0;
        v_uint32 r00 = vx_setzero_u32(), r01 = vx_setzero_u32();
        v_uint32 r10 = vx_setzero_u32(), r11 = vx_setzero_u32();
        cho (; j<= n - 2 * VTraits<v_uint16>::vlanes(); j += 2 * VTraits<v_uint16>::vlanes()) {
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
        s += (số_nguyên)v_reduce_sum(v_add(v_add(v_add(r00, r01), r10), r11));
        cho (; j < n; j++) {
            s += src[j];
        }
        trả_về s;
    }
};

template<>
struct NormL1_SIMD<short, số_nguyên> {
    số_nguyên operator() (const short* src, số_nguyên n) const {
        số_nguyên j = 0;
        số_nguyên s = 0;
        v_uint32 r00 = vx_setzero_u32(), r01 = vx_setzero_u32();
        v_uint32 r10 = vx_setzero_u32(), r11 = vx_setzero_u32();
        cho (; j<= n - 2 * VTraits<v_int16>::vlanes(); j += 2 * VTraits<v_int16>::vlanes()) {
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
        s += (số_nguyên)v_reduce_sum(v_add(v_add(v_add(r00, r01), r10), r11));
        cho (; j < n; j++) {
            s += saturate_cast<số_nguyên>(cv_abs(src[j]));
        }
        trả_về s;
    }
};

template<>
struct NormL2_SIMD<uchar, số_nguyên> {
    số_nguyên operator() (const uchar* src, số_nguyên n) const {
        số_nguyên j = 0;
        số_nguyên s = 0;
        v_uint32 r0 = vx_setzero_u32(), r1 = vx_setzero_u32();
        cho (; j <= n - 2 * VTraits<v_uint8>::vlanes(); j += 2 * VTraits<v_uint8>::vlanes()) {
            v_uint8 v0 = vx_load(src + j);
            r0 = v_dotprod_expand_fast(v0, v0, r0);

            v_uint8 v1 = vx_load(src + j + VTraits<v_uint8>::vlanes());
            r1 = v_dotprod_expand_fast(v1, v1, r1);
        }
        s += v_reduce_sum(v_add(r0, r1));
        cho (; j < n; j++) {
            số_nguyên v = saturate_cast<số_nguyên>(src[j]);
            s += v * v;
        }
        trả_về s;
    }
};

template<>
struct NormL2_SIMD<schar, số_nguyên> {
    số_nguyên operator() (const schar* src, số_nguyên n) const {
        số_nguyên j = 0;
        số_nguyên s = 0;
        v_int32 r0 = vx_setzero_s32(), r1 = vx_setzero_s32();
        cho (; j <= n - 2 * VTraits<v_int8>::vlanes(); j += 2 * VTraits<v_int8>::vlanes()) {
            v_int8 v0 = vx_load(src + j);
            r0 = v_dotprod_expand_fast(v0, v0, r0);
            v_int8 v1 = vx_load(src + j + VTraits<v_int8>::vlanes());
            r1 = v_dotprod_expand_fast(v1, v1, r1);
        }
        s += v_reduce_sum(v_add(r0, r1));
        cho (; j < n; j++) {
            số_nguyên v = saturate_cast<số_nguyên>(src[j]);
            s += v * v;
        }
        trả_về s;
    }
};

template<>
struct NormDiffInf_SIMD<uchar, số_nguyên> {
    số_nguyên operator() (const uchar* src1, const uchar* src2, số_nguyên n) const {
        số_nguyên j = 0;
        số_nguyên s = 0;
        v_uint8 r0 = vx_setzero_u8(), r1 = vx_setzero_u8();
        v_uint8 r2 = vx_setzero_u8(), r3 = vx_setzero_u8();
        cho (; j <= n - 4 * VTraits<v_uint8>::vlanes(); j += 4 * VTraits<v_uint8>::vlanes()) {
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
        s = (số_nguyên)v_reduce_max(v_max(v_max(v_max(r0, r1), r2), r3));
        cho (; j < n; j++) {
            số_nguyên v = (số_nguyên)cv_absdiff(src1[j], src2[j]);
            s = std::max(s, v);
        }
        trả_về s;
    }
};

template<>
struct NormDiffInf_SIMD<schar, số_nguyên> {
    số_nguyên operator() (const schar* src1, const schar* src2, số_nguyên n) const {
        số_nguyên j = 0;
        số_nguyên s = 0;
        v_uint8 r0 = vx_setzero_u8(), r1 = vx_setzero_u8();
        v_uint8 r2 = vx_setzero_u8(), r3 = vx_setzero_u8();
        cho (; j <= n - 4 * VTraits<v_int8>::vlanes(); j += 4 * VTraits<v_int8>::vlanes()) {
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
        s = (số_nguyên)v_reduce_max(v_max(v_max(v_max(r0, r1), r2), r3));
        cho (; j < n; j++) {
            số_nguyên v = (số_nguyên)cv_absdiff(src1[j], src2[j]);
            s = std::max(s, v);
        }
        trả_về s;
    }
};

template<>
struct NormDiffInf_SIMD<ushort, số_nguyên> {
    số_nguyên operator() (const ushort* src1, const ushort* src2, số_nguyên n) const {
        số_nguyên j = 0;
        số_nguyên s = 0;
        v_uint16 r0 = vx_setzero_u16(), r1 = vx_setzero_u16();
        v_uint16 r2 = vx_setzero_u16(), r3 = vx_setzero_u16();
        cho (; j <= n - 4 * VTraits<v_uint16>::vlanes(); j += 4 * VTraits<v_uint16>::vlanes()) {
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
        s = (số_nguyên)v_reduce_max(v_max(v_max(v_max(r0, r1), r2), r3));
        cho (; j < n; j++) {
            số_nguyên v = (số_nguyên)cv_absdiff(src1[j], src2[j]);
            s = std::max(s, v);
        }
        trả_về s;
    }
};

template<>
struct NormDiffInf_SIMD<short, số_nguyên> {
    số_nguyên operator() (const short* src1, const short* src2, số_nguyên n) const {
        số_nguyên j = 0;
        số_nguyên s = 0;
        v_uint16 r0 = vx_setzero_u16(), r1 = vx_setzero_u16();
        v_uint16 r2 = vx_setzero_u16(), r3 = vx_setzero_u16();
        cho (; j <= n - 4 * VTraits<v_int16>::vlanes(); j += 4 * VTraits<v_int16>::vlanes()) {
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
        s = (số_nguyên)v_reduce_max(v_max(v_max(v_max(r0, r1), r2), r3));
        cho (; j < n; j++) {
            số_nguyên v = (số_nguyên)cv_absdiff(src1[j], src2[j]);
            s = std::max(s, v);
        }
        trả_về s;
    }
};

template<>
struct NormDiffInf_SIMD<số_nguyên, unsigned> {
    unsigned operator() (const số_nguyên* src1, const số_nguyên* src2, số_nguyên n) const {
        số_nguyên j = 0;
        unsigned s = 0;
        v_uint32 r0 = vx_setzero_u32(), r1 = vx_setzero_u32();
        v_uint32 r2 = vx_setzero_u32(), r3 = vx_setzero_u32();
        cho (; j <= n - 4 * VTraits<v_int32>::vlanes(); j += 4 * VTraits<v_int32>::vlanes()) {
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
        s = (số_nguyên)v_reduce_max(v_max(v_max(v_max(r0, r1), r2), r3));
        cho (; j < n; j++) {
            unsigned v = (unsigned)cv_absdiff(src1[j], src2[j]);
            s = std::max(s, v);
        }
        trả_về s;
    }
};

template<>
struct NormDiffInf_SIMD<float, float> {
    float operator() (const float* src1, const float* src2, số_nguyên n) const {
        số_nguyên j = 0;
        float s = 0;
        v_float32 r0 = vx_setzero_f32(), r1 = vx_setzero_f32();
        cho (; j <= n - 2 * VTraits<v_float32>::vlanes(); j += 2 * VTraits<v_float32>::vlanes()) {
            v_float32 v01 = vx_load(src1 + j), v02 = vx_load(src2 + j);
            r0 = v_max(r0, v_absdiff(v01, v02));

            v_float32 v11 = vx_load(src1 + j + VTraits<v_float32>::vlanes()),
                      v12 = vx_load(src2 + j + VTraits<v_float32>::vlanes());
            r1 = v_max(r1, v_absdiff(v11, v12));
        }
        s = v_reduce_max(v_max(r0, r1));
        cho (; j < n; j++) {
            float v = (float)cv_absdiff(src1[j], src2[j]);
            s = std::max(s, v);
        }
        trả_về s;
    }
};

template<>
struct NormDiffL1_SIMD<uchar, số_nguyên> {
    số_nguyên operator() (const uchar* src1, const uchar* src2, số_nguyên n) const {
        số_nguyên j = 0;
        số_nguyên s = 0;
        v_uint32 r0 = vx_setzero_u32(), r1 = vx_setzero_u32();
        v_uint8  one = vx_setall_u8(1);
        cho (; j<= n - 2 * VTraits<v_uint8>::vlanes(); j += 2 * VTraits<v_uint8>::vlanes()) {
            v_uint8 v01 = vx_load(src1 + j), v02 = vx_load(src2 + j);
            r0 = v_dotprod_expand_fast(v_absdiff(v01, v02), one, r0);

            v_uint8 v11 = vx_load(src1 + j + VTraits<v_uint8>::vlanes()),
                    v12 = vx_load(src2 + j + VTraits<v_uint8>::vlanes());
            r1 = v_dotprod_expand_fast(v_absdiff(v11, v12), one, r1);
        }
        s += v_reduce_sum(v_add(r0, r1));
        cho (; j < n; j++) {
            số_nguyên v = (số_nguyên)cv_absdiff(src1[j], src2[j]);
            s += v;
        }
        trả_về s;
    }
};

template<>
struct NormDiffL1_SIMD<schar, số_nguyên> {
    số_nguyên operator() (const schar* src1, const schar* src2, số_nguyên n) const {
        số_nguyên j = 0;
        số_nguyên s = 0;
        v_uint32 r0 = vx_setzero_u32(), r1 = vx_setzero_u32();
        v_uint8  one = vx_setall_u8(1);
        cho (; j<= n - 2 * VTraits<v_int8>::vlanes(); j += 2 * VTraits<v_int8>::vlanes()) {
            v_int8 v01 = vx_load(src1 + j), v02 = vx_load(src2 + j);
            r0 = v_dotprod_expand_fast(v_absdiff(v01, v02), one, r0);

            v_int8 v11 = vx_load(src1 + j + VTraits<v_int8>::vlanes()),
                   v12 = vx_load(src2 + j + VTraits<v_int8>::vlanes());
            r1 = v_dotprod_expand_fast(v_absdiff(v11, v12), one, r1);
        }
        s += v_reduce_sum(v_add(r0, r1));
        cho (; j < n; j++) {
            số_nguyên v = (số_nguyên)cv_absdiff(src1[j], src2[j]);
            s += v;
        }
        trả_về s;
    }
};

template<>
struct NormDiffL1_SIMD<ushort, số_nguyên> {
    số_nguyên operator() (const ushort* src1, const ushort* src2, số_nguyên n) const {
        số_nguyên j = 0;
        số_nguyên s = 0;
        v_uint32 r0 = vx_setzero_u32(), r1 = vx_setzero_u32();
        v_uint32 r2 = vx_setzero_u32(), r3 = vx_setzero_u32();
        cho (; j<= n - 4 * VTraits<v_uint16>::vlanes(); j += 4 * VTraits<v_uint16>::vlanes()) {
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
        s += (số_nguyên)v_reduce_sum(v_add(v_add(v_add(r0, r1), r2), r3));
        cho (; j < n; j++) {
            số_nguyên v = (số_nguyên)cv_absdiff(src1[j], src2[j]);
            s += v;
        }
        trả_về s;
    }
};

template<>
struct NormDiffL1_SIMD<short, số_nguyên> {
    số_nguyên operator() (const short* src1, const short* src2, số_nguyên n) const {
        số_nguyên j = 0;
        số_nguyên s = 0;
        v_uint32 r0 = vx_setzero_u32(), r1 = vx_setzero_u32();
        v_uint32 r2 = vx_setzero_u32(), r3 = vx_setzero_u32();
        cho (; j<= n - 4 * VTraits<v_int16>::vlanes(); j += 4 * VTraits<v_int16>::vlanes()) {
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
        s += (số_nguyên)v_reduce_sum(v_add(v_add(v_add(r0, r1), r2), r3));
        cho (; j < n; j++) {
            số_nguyên v = (số_nguyên)cv_absdiff(src1[j], src2[j]);
            s += v;
        }
        trả_về s;
    }
};

template<>
struct NormDiffL2_SIMD<uchar, số_nguyên> {
    số_nguyên operator() (const uchar* src1, const uchar* src2, số_nguyên n) const {
        số_nguyên j = 0;
        số_nguyên s = 0;
        v_uint32 r0 = vx_setzero_u32(), r1 = vx_setzero_u32();
        cho (; j <= n - 2 * VTraits<v_uint8>::vlanes(); j += 2 * VTraits<v_uint8>::vlanes()) {
            v_uint8 v01 = vx_load(src1 + j), v02 = vx_load(src2 + j);
            v_uint8 v0 = v_absdiff(v01, v02);
            r0 = v_dotprod_expand_fast(v0, v0, r0);

            v_uint8 v11 = vx_load(src1 + j + VTraits<v_uint8>::vlanes()),
                    v12 = vx_load(src2 + j + VTraits<v_uint8>::vlanes());
            v_uint8 v1 = v_absdiff(v11, v12);
            r1 = v_dotprod_expand_fast(v1, v1, r1);
        }
        s += v_reduce_sum(v_add(r0, r1));
        cho (; j < n; j++) {
            số_nguyên v = (số_nguyên)src1[j] - (số_nguyên)src2[j];
            s += v * v;
        }
        trả_về s;
    }
};

template<>
struct NormDiffL2_SIMD<schar, số_nguyên> {
    số_nguyên operator() (const schar* src1, const schar* src2, số_nguyên n) const {
        số_nguyên j = 0;
        số_nguyên s = 0;
        v_uint32 r0 = vx_setzero_u32(), r1 = vx_setzero_u32();
        cho (; j <= n - 2 * VTraits<v_int8>::vlanes(); j += 2 * VTraits<v_int8>::vlanes()) {
            v_int8 v01 = vx_load(src1 + j), v02 = vx_load(src2 + j);
            v_uint8 v0 = v_absdiff(v01, v02);
            r0 = v_dotprod_expand_fast(v0, v0, r0);

            v_int8 v11 = vx_load(src1 + j + VTraits<v_int8>::vlanes()),
                   v12 = vx_load(src2 + j + VTraits<v_int8>::vlanes());
            v_uint8 v1 = v_absdiff(v11, v12);
            r1 = v_dotprod_expand_fast(v1, v1, r1);
        }
        s += v_reduce_sum(v_add(r0, r1));
        cho (; j < n; j++) {
            số_nguyên v = (số_nguyên)src1[j] - (số_nguyên)src2[j];
            s += v * v;
        }
        trả_về s;
    }
};

#endif

#nếu (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)

template<>
struct NormInf_SIMD<double, double> {
    double operator() (const double* src, số_nguyên n) const {
        số_nguyên j = 0;
        double s = 0.f;
        v_float64 r0 = vx_setzero_f64(), r1 = vx_setzero_f64();
        v_float64 r2 = vx_setzero_f64(), r3 = vx_setzero_f64();
        cho (; j <= n - 4 * VTraits<v_float64>::vlanes(); j += 4 * VTraits<v_float64>::vlanes()) {
            r0 = v_max(r0, v_abs(vx_load(src + j                                   )));
            r1 = v_max(r1, v_abs(vx_load(src + j +     VTraits<v_float64>::vlanes())));
            r2 = v_max(r2, v_abs(vx_load(src + j + 2 * VTraits<v_float64>::vlanes())));
            r3 = v_max(r3, v_abs(vx_load(src + j + 3 * VTraits<v_float64>::vlanes())));
        }
        r0 = v_max(r0, v_max(r1, v_max(r2, r3)));
        cho (; j < n; j++) {
            s = std::max(s, cv_abs(src[j]));
        }
        // [TODO]: use v_reduce_max when it supports float64
        double t[VTraits<v_float64>::max_nlanes];
        vx_store(t, r0);
        cho (số_nguyên i = 0; i < VTraits<v_float64>::vlanes(); i++) {
            s = std::max(s, cv_abs(t[i]));
        }
        trả_về s;
    }
};

template<>
struct NormL1_SIMD<số_nguyên, double> {
    double operator() (const số_nguyên* src, số_nguyên n) const {
        số_nguyên j = 0;
        double s = 0.f;
        v_float64 r00 = vx_setzero_f64(), r01 = vx_setzero_f64();
        v_float64 r10 = vx_setzero_f64(), r11 = vx_setzero_f64();
        cho (; j <= n - 2 * VTraits<v_int32>::vlanes(); j += 2 * VTraits<v_int32>::vlanes()) {
            // Widen to double before taking the magnitude: exact over the whole int32 range,
            // INT_MIN included, which a float intermediate would not be above 2^24.
            v_int32 i0 = vx_load(src + j), i1 = vx_load(src + j + VTraits<v_int32>::vlanes());
            r00 = v_add(r00, v_abs(v_cvt_f64(i0))); r01 = v_add(r01, v_abs(v_cvt_f64_high(i0)));
            r10 = v_add(r10, v_abs(v_cvt_f64(i1))); r11 = v_add(r11, v_abs(v_cvt_f64_high(i1)));
        }
        s += v_reduce_sum(v_add(v_add(v_add(r00, r01), r10), r11));
        cho (; j < n; j++) {
            s += cv_abs(src[j]);
        }
        trả_về s;
    }
};

template<>
struct NormL1_SIMD<float, double> {
    double operator() (const float* src, số_nguyên n) const {
        số_nguyên j = 0;
        double s = 0.f;
        v_float64 r00 = vx_setzero_f64(), r01 = vx_setzero_f64();
        v_float64 r10 = vx_setzero_f64(), r11 = vx_setzero_f64();
        v_float64 r20 = vx_setzero_f64(), r21 = vx_setzero_f64();
        v_float64 r30 = vx_setzero_f64(), r31 = vx_setzero_f64();
        cho (; j <= n - 4 * VTraits<v_float32>::vlanes(); j += 4 * VTraits<v_float32>::vlanes()) {
            v_float32 v0 = v_abs(vx_load(src + j)), v1 = v_abs(vx_load(src + j + VTraits<v_float32>::vlanes()));
            r00 = v_add(r00, v_cvt_f64(v0)); r01 = v_add(r01, v_cvt_f64_high(v0));
            r10 = v_add(r10, v_cvt_f64(v1)); r11 = v_add(r11, v_cvt_f64_high(v1));

            v_float32 v2 = v_abs(vx_load(src + j + 2 * VTraits<v_float32>::vlanes())), v3 = v_abs(vx_load(src + j + 3 * VTraits<v_float32>::vlanes()));
            r20 = v_add(r20, v_cvt_f64(v2)); r21 = v_add(r21, v_cvt_f64_high(v2));
            r30 = v_add(r30, v_cvt_f64(v3)); r31 = v_add(r31, v_cvt_f64_high(v3));
        }
        s += v_reduce_sum(v_add(v_add(v_add(r00, r01), r10), r11));
        s += v_reduce_sum(v_add(v_add(v_add(r20, r21), r30), r31));
        cho (; j < n; j++) {
            s += cv_abs(src[j]);
        }
        trả_về s;
    }
};

template<>
struct NormL2_SIMD<ushort, double> {
    double operator() (const ushort* src, số_nguyên n) const {
        số_nguyên j = 0;
        double s = 0.f;
        v_float64 r0 = vx_setzero_f64(), r1 = vx_setzero_f64();
        cho (; j <= n - 2 * VTraits<v_uint16>::vlanes(); j += 2 * VTraits<v_uint16>::vlanes()) {
            v_uint16 v0 = vx_load(src + j);
            v_uint64 u0 = v_dotprod_expand_fast(v0, v0);
            r0 = v_add(r0, v_cvt_f64(v_reinterpret_as_s64(u0)));

            v_uint16 v1 = vx_load(src + j + VTraits<v_uint16>::vlanes());
            v_uint64 u1 = v_dotprod_expand_fast(v1, v1);
            r1 = v_add(r1, v_cvt_f64(v_reinterpret_as_s64(u1)));
        }
        s += v_reduce_sum(v_add(r0, r1));
        cho (; j < n; j++) {
            double v = saturate_cast<double>(src[j]);
            s += v * v;
        }
        trả_về s;
    }
};

template<>
struct NormL2_SIMD<short, double> {
    double operator() (const short* src, số_nguyên n) const {
        số_nguyên j = 0;
        double s = 0.f;
        v_float64 r0 = vx_setzero_f64(), r1 = vx_setzero_f64();
        cho (; j <= n - 2 * VTraits<v_int16>::vlanes(); j += 2 * VTraits<v_int16>::vlanes()) {
            v_int16 v0 = vx_load(src + j);
            r0 = v_add(r0, v_cvt_f64(v_dotprod_expand_fast(v0, v0)));

            v_int16 v1 = vx_load(src + j + VTraits<v_int16>::vlanes());
            r1 = v_add(r1, v_cvt_f64(v_dotprod_expand_fast(v1, v1)));
        }
        s += v_reduce_sum(v_add(r0, r1));
        cho (; j < n; j++) {
            double v = saturate_cast<double>(src[j]);
            s += v * v;
        }
        trả_về s;
    }
};

template<>
struct NormL2_SIMD<số_nguyên, double> {
    double operator() (const số_nguyên* src, số_nguyên n) const {
        số_nguyên j = 0;
        double s = 0.f;
        v_float64 r0 = vx_setzero_f64(), r1 = vx_setzero_f64();
        cho (; j <= n - 2 * VTraits<v_int32>::vlanes(); j += 2 * VTraits<v_int32>::vlanes()) {
            v_int32 v0 = vx_load(src + j);
            r0 = v_dotprod_expand_fast(v0, v0, r0);

            v_int32 v1 = vx_load(src + j + VTraits<v_int32>::vlanes());
            r1 = v_dotprod_expand_fast(v1, v1, r1);
        }
        s += v_reduce_sum(v_add(r0, r1));
        cho (; j < n; j++) {
            double v = src[j];
            s += v * v;
        }
        trả_về s;
    }
};

template<>
struct NormL2_SIMD<float, double> {
    double operator() (const float* src, số_nguyên n) const {
        số_nguyên j = 0;
        double s = 0.f;
        v_float64 r00 = vx_setzero_f64(), r01 = vx_setzero_f64();
        v_float64 r10 = vx_setzero_f64(), r11 = vx_setzero_f64();
        cho (; j <= n - 2 * VTraits<v_float32>::vlanes(); j += 2 * VTraits<v_float32>::vlanes()) {
            v_float32 v0 = vx_load(src + j), v1 = vx_load(src + j + VTraits<v_float32>::vlanes());
            v_float64 v00 = v_cvt_f64(v0), v01 = v_cvt_f64_high(v0);
            v_float64 v10 = v_cvt_f64(v1), v11 = v_cvt_f64_high(v1);
            r00 = v_fma(v00, v00, r00); r01 = v_fma(v01, v01, r01);
            r10 = v_fma(v10, v10, r10); r11 = v_fma(v11, v11, r11);
        }
        s += v_reduce_sum(v_add(v_add(v_add(r00, r01), r10), r11));
        cho (; j < n; j++) {
            double v = src[j];
            s += v * v;
        }
        trả_về s;
    }
};

#endif

#nếu CV_SIMD_64F // CV_SIMD_SCALABLE_64F has accuracy problem with the following kernels on ci

template<>
struct NormL1_SIMD<double, double> {
    double operator() (const double* src, số_nguyên n) const {
        số_nguyên j = 0;
        double s = 0.f;
        v_float64 r00 = vx_setzero_f64(), r01 = vx_setzero_f64();
        v_float64 r10 = vx_setzero_f64(), r11 = vx_setzero_f64();
        cho (; j <= n - 4 * VTraits<v_float64>::vlanes(); j += 4 * VTraits<v_float64>::vlanes()) {
            r00 = v_add(r00, v_abs(vx_load(src + j                                   )));
            r01 = v_add(r01, v_abs(vx_load(src + j +     VTraits<v_float64>::vlanes())));
            r10 = v_add(r10, v_abs(vx_load(src + j + 2 * VTraits<v_float64>::vlanes())));
            r11 = v_add(r11, v_abs(vx_load(src + j + 3 * VTraits<v_float64>::vlanes())));
        }
        s += v_reduce_sum(v_add(v_add(v_add(r00, r01), r10), r11));
        cho (; j < n; j++) {
            s += cv_abs(src[j]);
        }
        trả_về s;
    }
};

template<>
struct NormL2_SIMD<double, double> {
    double operator() (const double* src, số_nguyên n) const {
        số_nguyên j = 0;
        double s = 0.f;
        v_float64 r00 = vx_setzero_f64(), r01 = vx_setzero_f64();
        v_float64 r10 = vx_setzero_f64(), r11 = vx_setzero_f64();
        cho (; j <= n - 4 * VTraits<v_float64>::vlanes(); j += 4 * VTraits<v_float64>::vlanes()) {
            v_float64 v00 = vx_load(src + j                                   );
            v_float64 v01 = vx_load(src + j +     VTraits<v_float64>::vlanes());
            v_float64 v10 = vx_load(src + j + 2 * VTraits<v_float64>::vlanes());
            v_float64 v11 = vx_load(src + j + 3 * VTraits<v_float64>::vlanes());
            r00 = v_fma(v00, v00, r00); r01 = v_fma(v01, v01, r01);
            r10 = v_fma(v10, v10, r10); r11 = v_fma(v11, v11, r11);
        }
        s += v_reduce_sum(v_add(v_add(v_add(r00, r01), r10), r11));
        cho (; j < n; j++) {
            double v = src[j];
            s += v * v;
        }
        trả_về s;
    }
};

template<>
struct NormDiffInf_SIMD<double, double> {
    double operator() (const double* src1, const double* src2, số_nguyên n) const {
        số_nguyên j = 0;
        double s = 0;
        v_float64 r0 = vx_setzero_f64(), r1 = vx_setzero_f64();
        cho (; j <= n - 2 * VTraits<v_float64>::vlanes(); j += 2 * VTraits<v_float64>::vlanes()) {
            v_float64 v01 = vx_load(src1 + j), v02 = vx_load(src2 + j);
            r0 = v_max(r0, v_absdiff(v01, v02));

            v_float64 v11 = vx_load(src1 + j + VTraits<v_float64>::vlanes()),
                      v12 = vx_load(src2 + j + VTraits<v_float64>::vlanes());
            r1 = v_max(r1, v_absdiff(v11, v12));
        }
        // [TODO]: use v_reduce_max when it supports float64
        double t[VTraits<v_float64>::max_nlanes];
        vx_store(t, v_max(r0, r1));
        cho (số_nguyên i = 0; i < VTraits<v_float64>::vlanes(); i++) {
            s = std::max(s, t[i]);
        }
        cho (; j < n; j++) {
            double v = (double)cv_absdiff(src1[j], src2[j]);
            s = std::max(s, v);
        }
        trả_về s;
    }
};

template<>
struct NormDiffL1_SIMD<float, double> {
    double operator() (const float* src1, const float* src2, số_nguyên n) const {
        số_nguyên j = 0;
        double s = 0.f;
        v_float64 r0 = vx_setzero_f64(), r1 = vx_setzero_f64();
        v_float64 r2 = vx_setzero_f64(), r3 = vx_setzero_f64();
        cho (; j <= n - 2 * VTraits<v_float32>::vlanes(); j += 2 * VTraits<v_float32>::vlanes()) {
            v_float32 v01 = vx_load(src1 + j), v02 = vx_load(src2 + j);
            v_float32 v0 = v_absdiff(v01, v02);
            r0 = v_add(r0, v_cvt_f64(v0)); r1 = v_add(r1, v_cvt_f64_high(v0));

            v_float32 v11 = vx_load(src1 + j + VTraits<v_float32>::vlanes()),
                      v12 = vx_load(src2 + j + VTraits<v_float32>::vlanes());
            v_float32 v1 = v_absdiff(v11, v12);
            r2 = v_add(r2, v_cvt_f64(v1)); r3 = v_add(r3, v_cvt_f64_high(v1));
        }
        s += v_reduce_sum(v_add(v_add(v_add(r0, r1), r2), r3));
        cho (; j < n; j++) {
            double v = (double)cv_absdiff(src1[j], src2[j]);
            s += v;
        }
        trả_về s;
    }
};

template<>
struct NormDiffL1_SIMD<double, double> {
    double operator() (const double* src1, const double* src2, số_nguyên n) const {
        số_nguyên j = 0;
        double s = 0.f;
        v_float64 r0 = vx_setzero_f64(), r1 = vx_setzero_f64();
        cho (; j <= n - 2 * VTraits<v_float64>::vlanes(); j += 2 * VTraits<v_float64>::vlanes()) {
            v_float64 v01 = vx_load(src1 + j), v02 = vx_load(src2 + j);
            r0 = v_add(r0, v_absdiff(v01, v02));

            v_float64 v11 = vx_load(src1 + j + VTraits<v_float64>::vlanes()),
                      v12 = vx_load(src2 + j + VTraits<v_float64>::vlanes());
            r1 = v_add(r1, v_absdiff(v11, v12));
        }
        s += v_reduce_sum(v_add(r0, r1));
        cho (; j < n; j++) {
            double v = (double)cv_absdiff(src1[j], src2[j]);
            s += v;
        }
        trả_về s;
    }
};

template<>
struct NormDiffL2_SIMD<ushort, double> {
    double operator() (const ushort* src1, const ushort* src2, số_nguyên n) const {
        số_nguyên j = 0;
        double s = 0.f;
        v_float64 r0 = vx_setzero_f64(), r1 = vx_setzero_f64();
        cho (; j <= n - 2 * VTraits<v_uint16>::vlanes(); j += 2 * VTraits<v_uint16>::vlanes()) {
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
        cho (; j < n; j++) {
            double v = (double)src1[j] - (double)src2[j];
            s += v * v;
        }
        trả_về s;
    }
};

template<>
struct NormDiffL2_SIMD<short, double> {
    double operator() (const short* src1, const short* src2, số_nguyên n) const {
        số_nguyên j = 0;
        double s = 0.f;
        v_float64 r0 = vx_setzero_f64(), r1 = vx_setzero_f64();
        cho (; j <= n - 2 * VTraits<v_int16>::vlanes(); j += 2 * VTraits<v_int16>::vlanes()) {
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
        cho (; j < n; j++) {
            double v = (double)src1[j] - (double)src2[j];
            s += v * v;
        }
        trả_về s;
    }
};

template<>
struct NormDiffL2_SIMD<số_nguyên, double> {
    double operator() (const số_nguyên* src1, const số_nguyên* src2, số_nguyên n) const {
        số_nguyên j = 0;
        double s = 0.f;
        v_float64 r0 = vx_setzero_f64(), r1 = vx_setzero_f64();
        cho (; j <= n - VTraits<v_int32>::vlanes(); j += VTraits<v_int32>::vlanes()) {
            v_int32 v01 = vx_load(src1 + j), v02 = vx_load(src2 + j);
            v_uint32 v0 = v_absdiff(v01, v02);
            v_uint64 ev0, ev1;
            v_expand(v0, ev0, ev1);
            v_float64 f0 = v_cvt_f64(v_reinterpret_as_s64(ev0)), f1 = v_cvt_f64(v_reinterpret_as_s64(ev1));
            r0 = v_fma(f0, f0, r0); r1 = v_fma(f1, f1, r1);
        }
        s += v_reduce_sum(v_add(r0, r1));
        cho (; j < n; j++) {
            double v = (double)src1[j] - (double)src2[j];
            s += v * v;
        }
        trả_về s;
    }
};

template<>
struct NormDiffL2_SIMD<float, double> {
    double operator() (const float* src1, const float* src2, số_nguyên n) const {
        số_nguyên j = 0;
        double s = 0.f;
        v_float64 r0 = vx_setzero_f64(), r1 = vx_setzero_f64();
        v_float64 r2 = vx_setzero_f64(), r3 = vx_setzero_f64();
        cho (; j <= n - 2 * VTraits<v_float32>::vlanes(); j += 2 * VTraits<v_float32>::vlanes()) {
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
        cho (; j < n; j++) {
            double v = (double)src1[j] - (double)src2[j];
            s += v * v;
        }
        trả_về s;
    }
};

template<>
struct NormDiffL2_SIMD<double, double> {
    double operator() (const double* src1, const double* src2, số_nguyên n) const {
        số_nguyên j = 0;
        double s = 0.f;
        v_float64 r0 = vx_setzero_f64(), r1 = vx_setzero_f64();
        v_float64 r2 = vx_setzero_f64(), r3 = vx_setzero_f64();
        cho (; j <= n - 4 * VTraits<v_float64>::vlanes(); j += 4 * VTraits<v_float64>::vlanes()) {
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
        cho (; j < n; j++) {
            double v = (double)src1[j] - (double)src2[j];
            s += v * v;
        }
        trả_về s;
    }
};

#endif

template <typename T, typename ST>
struct MaskedNormInf_SIMD {
    inline ST operator() (const T* src, const uchar* mask, số_nguyên len, số_nguyên cn) const {
        ST s = 0;
        nếu (cn == 1) {
            cho (số_nguyên i = 0; i < len; i++) {
                nếu (mask[i]) {
                    s = std::max(s, (ST)cv_abs(src[i]));
                }
            }
        }
        không_thì {
            cho (số_nguyên i = 0; i < len; i++) {
                nếu (mask[i]) {
                    const T* elem = src + i * cn;
                    số_nguyên k = 0;
                #nếu CV_ENABLE_UNROLLED && !CV_SIMD_SCALABLE
                    cho (; k <= cn - 4; k += 4) {
                        s = std::max(s, (ST)cv_abs(elem[k]));
                        s = std::max(s, (ST)cv_abs(elem[k + 1]));
                        s = std::max(s, (ST)cv_abs(elem[k + 2]));
                        s = std::max(s, (ST)cv_abs(elem[k + 3]));
                    }
                #endif
                    cho (; k < cn; k++) {
                        s = std::max(s, (ST)cv_abs(elem[k]));
                    }
                }
            }
        }
        trả_về s;
    }
};

template <typename T, typename ST>
struct MaskedNormL1_SIMD {
    inline ST operator() (const T* src, const uchar* mask, số_nguyên len, số_nguyên cn) const {
        ST s = 0;
        nếu (cn == 1) {
            cho (số_nguyên i = 0; i < len; i++) {
                nếu (mask[i]) {
                    s += (ST)cv_abs(src[i]);
                }
            }
        }
        không_thì {
            cho (số_nguyên i = 0; i < len; i++) {
                nếu (mask[i]) {
                    const T* elem = src + i * cn;
                    số_nguyên k = 0;
                #nếu CV_ENABLE_UNROLLED && !CV_SIMD_SCALABLE
                    cho (; k <= cn - 4; k += 4) {
                        s += (ST)cv_abs(elem[k]);
                        s += (ST)cv_abs(elem[k + 1]);
                        s += (ST)cv_abs(elem[k + 2]);
                        s += (ST)cv_abs(elem[k + 3]);
                    }
                #endif
                    cho (; k < cn; k++) {
                        s += (ST)cv_abs(elem[k]);
                    }
                }
            }
        }
        trả_về s;
    }
};

template <typename T, typename ST>
struct MaskedNormL2_SIMD {
    inline ST operator() (const T* src, const uchar* mask, số_nguyên len, số_nguyên cn) const {
        ST s = 0;
        nếu (cn == 1) {
            số_nguyên i = 0;
        #nếu CV_ENABLE_UNROLLED && !CV_SIMD_SCALABLE
            cho (; i <= len - 4; i += 4) {
                nếu (mask[i])     { T v0 = src[i];     s += (ST)v0 * v0; }
                nếu (mask[i + 1]) { T v1 = src[i + 1]; s += (ST)v1 * v1; }
                nếu (mask[i + 2]) { T v2 = src[i + 2]; s += (ST)v2 * v2; }
                nếu (mask[i + 3]) { T v3 = src[i + 3]; s += (ST)v3 * v3; }
            }
        #endif
            cho (; i < len; i++) {
                nếu (mask[i]) {
                    T v = src[i];
                    s += (ST)v * v;
                }
            }
        }
        không_thì {
            cho (số_nguyên i = 0; i < len; i++) {
                nếu (mask[i]) {
                    const T* elem = src + i * cn;
                    số_nguyên k = 0;
                #nếu CV_ENABLE_UNROLLED && !CV_SIMD_SCALABLE
                    cho (; k <= cn - 4; k += 4) {
                        T v0 = elem[k];     s += (ST)v0 * v0;
                        T v1 = elem[k + 1]; s += (ST)v1 * v1;
                        T v2 = elem[k + 2]; s += (ST)v2 * v2;
                        T v3 = elem[k + 3]; s += (ST)v3 * v3;
                    }
                #endif
                    cho (; k < cn; k++) {
                        T v = elem[k];
                        s += (ST)v * v;
                    }
                }
            }
        }

        trả_về s;
    }
};

#nếu (CV_SIMD || CV_SIMD_SCALABLE)
static inline v_uint8  v_normmask_u8 (const uchar* m) { trả_về v_gt(vx_load(m),          vx_setzero_u8());  }
static inline v_uint16 v_normmask_u16(const uchar* m) { trả_về v_gt(vx_load_expand(m),   vx_setzero_u16()); }
static inline v_uint32 v_normmask_u32(const uchar* m) { trả_về v_gt(vx_load_expand_q(m), vx_setzero_u32()); }
static inline void v_normmask_u64(const uchar* m, v_uint64& m0, v_uint64& m1) {
    v_uint64 e0, e1;
    v_expand(v_normmask_u32(m), e0, e1);
    m0 = v_or(e0, v_shl<32>(e0));
    m1 = v_or(e1, v_shl<32>(e1));
}

template <>
struct MaskedNormInf_SIMD<float, float> {
    inline float operator()(const float* src, const uchar* mask, số_nguyên len, số_nguyên cn) const {
        float result = 0.0f;
        nếu (cn == 1) {
            số_nguyên i = 0;
            const số_nguyên vstep = VTraits<v_float32>::vlanes();
            v_float32 acc = vx_setzero_f32();

            cho (; i <= len - vstep; i += vstep) {
                v_uint32 m = vx_load_expand_q(mask + i);
                v_uint32 cmp = v_gt(m, vx_setzero_u32());
                v_float32 s = vx_load(src + i);
                s = v_abs(s);
                s = v_reinterpret_as_f32(v_and(v_reinterpret_as_u32(s), cmp));
                acc = v_max(acc, s);
            }
            result = v_reduce_max(acc);

            cho (; i < len; i++) {
                nếu (mask[i])
                    result = std::max(result, std::abs(src[i]));
            }
        }
        không_thì {
            cho (số_nguyên i = 0; i < len; i++) {
                nếu (mask[i]) {
                    const float* elem = src + i * cn;
                    số_nguyên k = 0;
                    const số_nguyên vstep = VTraits<v_float32>::vlanes();
                    v_float32 acc = vx_setzero_f32();

                    cho (; k <= cn - vstep; k += vstep) {
                        v_float32 s = vx_load(elem + k);
                        acc = v_max(acc, v_abs(s));
                    }

                    result = std::max(result, v_reduce_max(acc));

                    cho (; k < cn; k++)
                        result = std::max(result, std::abs(elem[k]));
                }
            }
        }
        trả_về result;
    }
};

#nếu CV_SIMD_64F
template <>
struct MaskedNormL1_SIMD<float, double> {
    inline double operator()(const float* src, const uchar* mask, số_nguyên len, số_nguyên cn) const {
        double result = 0.0;
        nếu (cn == 1) {
            số_nguyên i = 0;
            const số_nguyên vstep = VTraits<v_float32>::vlanes();
            v_float64 acc0 = vx_setzero_f64(), acc1 = vx_setzero_f64();
            v_float64 acc2 = vx_setzero_f64(), acc3 = vx_setzero_f64();

            cho (; i <= len - 2*vstep; i += 2*vstep) {
                v_float32 s0 = v_and(v_abs(vx_load(src + i)),
                                     v_reinterpret_as_f32(v_normmask_u32(mask + i)));
                v_float32 s1 = v_and(v_abs(vx_load(src + i + vstep)),
                                     v_reinterpret_as_f32(v_normmask_u32(mask + i + vstep)));
                acc0 = v_add(acc0, v_cvt_f64(s0));
                acc1 = v_add(acc1, v_cvt_f64_high(s0));
                acc2 = v_add(acc2, v_cvt_f64(s1));
                acc3 = v_add(acc3, v_cvt_f64_high(s1));
            }
            cho (; i <= len - vstep; i += vstep) {
                v_float32 s = v_and(v_abs(vx_load(src + i)),
                                    v_reinterpret_as_f32(v_normmask_u32(mask + i)));
                acc0 = v_add(acc0, v_cvt_f64(s));
                acc1 = v_add(acc1, v_cvt_f64_high(s));
            }
            result = v_reduce_sum(v_add(v_add(acc0, acc1), v_add(acc2, acc3)));

            cho (; i < len; i++) {
                nếu (mask[i])
                    result += std::abs(src[i]);
            }
        }
        không_thì {
            cho (số_nguyên i = 0; i < len; i++) {
                nếu (mask[i]) {
                    const float* elem = src + i * cn;
                    số_nguyên k = 0;
                    const số_nguyên vstep = VTraits<v_float32>::vlanes();
                    v_float64 acc = vx_setzero_f64();

                    cho (; k <= cn - vstep; k += vstep) {
                        v_float32 s = v_abs(vx_load(elem + k));
                        acc = v_add(acc, v_cvt_f64(s));
                        acc = v_add(acc, v_cvt_f64_high(s));
                    }

                    result += v_reduce_sum(acc);

                    cho (; k < cn; k++)
                        result += std::abs(elem[k]);
                }
            }
        }
        trả_về result;
    }
};

template <>
struct MaskedNormL2_SIMD<float, double> {
    inline double operator()(const float* src, const uchar* mask, số_nguyên len, số_nguyên cn) const {
        double result = 0.0;
        nếu (cn == 1) {
            số_nguyên i = 0;
            const số_nguyên vstep = VTraits<v_float32>::vlanes();
            v_float32 facc = vx_setzero_f32();
            v_float64 dacc = vx_setzero_f64();
            số_nguyên flush = 0;

            cho (; i <= len - vstep; i += vstep, flush += vstep) {
                nếu (flush >= 64) {
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

            cho (; i < len; i++) {
                nếu (mask[i]) {
                    double v = src[i];
                    result += v * v;
                }
            }
        }
        không_thì {
            cho (số_nguyên i = 0; i < len; i++) {
                nếu (mask[i]) {
                    const float* elem = src + i * cn;
                    số_nguyên k = 0;
                    const số_nguyên vstep = VTraits<v_float32>::vlanes();
                    v_float32 facc = vx_setzero_f32();

                    cho (; k <= cn - vstep; k += vstep) {
                        v_float32 s = vx_load(elem + k);
                        facc = v_add(facc, v_mul(s, s));
                    }

                    v_float64 dacc = v_add(v_cvt_f64(facc), v_cvt_f64_high(facc));
                    result += v_reduce_sum(dacc);

                    cho (; k < cn; k++) {
                        double v = elem[k];
                        result += v * v;
                    }
                }
            }
        }
        trả_về result;
    }
};

#endif

template <>
struct MaskedNormInf_SIMD<uchar, số_nguyên> {
    inline số_nguyên operator()(const uchar* src, const uchar* mask, số_nguyên len, số_nguyên cn) const {
        số_nguyên result = 0;
        nếu (cn == 1) {
            số_nguyên i = 0;
            const số_nguyên vstep = VTraits<v_uint8>::vlanes();
            v_uint8 acc = vx_setzero_u8();

            cho (; i <= len - vstep; i += vstep) {
                v_uint8 m   = vx_load(mask + i);
                v_uint8 s   = vx_load(src + i);
                v_uint8 sel = v_and(s, v_gt(m, vx_setzero_u8()));
                acc = v_max(acc, sel);
            }

            result = (số_nguyên)v_reduce_max(acc);

            cho (; i < len; i++) {
                nếu (mask[i])
                    result = std::max(result, (số_nguyên)src[i]);
            }
        }
        không_thì nếu (cn == 4 && len >= VTraits<v_uint8>::vlanes()) {
            const số_nguyên vstep = VTraits<v_uint8>::vlanes();
            v_uint8 acc = vx_setzero_u8();
            số_nguyên i = 0;
            cho (;;) {
                nếu (i > len - vstep)
                    i = len - vstep; // back-step (max is idempotent)
                v_uint8 c0, c1, c2, c3;
                v_load_deinterleave(src + i * 4, c0, c1, c2, c3);
                v_uint8 pmax = v_max(v_max(c0, c1), v_max(c2, c3));
                v_uint8 m = v_gt(vx_load(mask + i), vx_setzero_u8());
                acc = v_max(acc, v_and(pmax, m));
                nếu (i >= len - vstep)
                    break;
                i += vstep;
            }
            result = (số_nguyên)v_reduce_max(acc);
        }
        không_thì nếu (cn == 4) {
            cho (số_nguyên i = 0; i < len; i++) {
                nếu (mask[i]) {
                    const uchar* elem = src + i * 4;
                    result = std::max(result, (số_nguyên)std::max(std::max(elem[0], elem[1]),
                                                            std::max(elem[2], elem[3])));
                }
            }
        }
        không_thì {
            cho (số_nguyên i = 0; i < len; i++) {
                nếu (mask[i]) {
                    const uchar* elem = src + i * cn;
                    cho (số_nguyên k = 0; k < cn; k++)
                        result = std::max(result, (số_nguyên)elem[k]);
                }
            }
        }
        trả_về result;
    }
};

template <>
struct MaskedNormL1_SIMD<uchar, số_nguyên> {
    inline số_nguyên operator()(const uchar* src, const uchar* mask, số_nguyên len, số_nguyên cn) const {
        số_nguyên result = 0;
        const số_nguyên vstep = VTraits<v_uint8>::vlanes();
        const v_uint8 one = vx_setall_u8(1);
        nếu (cn == 1) {
            số_nguyên i = 0;
            v_uint32 acc = vx_setzero_u32();
            cho (; i <= len - vstep; i += vstep) {
                v_uint8 s = v_and(vx_load(src + i), v_normmask_u8(mask + i));
                acc = v_dotprod_expand_fast(s, one, acc);
            }
            result = (số_nguyên)v_reduce_sum(acc);
            cho (; i < len; i++) {
                nếu (mask[i])
                    result += src[i];
            }
        }
        không_thì nếu (cn == 4) {
            số_nguyên i = 0;
            v_uint32 acc = vx_setzero_u32();
            cho (; i <= len - vstep; i += vstep) {
                v_uint8 c0, c1, c2, c3;
                v_load_deinterleave(src + i * 4, c0, c1, c2, c3);
                v_uint8 m = v_normmask_u8(mask + i);
                acc = v_dotprod_expand_fast(v_and(c0, m), one, acc);
                acc = v_dotprod_expand_fast(v_and(c1, m), one, acc);
                acc = v_dotprod_expand_fast(v_and(c2, m), one, acc);
                acc = v_dotprod_expand_fast(v_and(c3, m), one, acc);
            }
            result = (số_nguyên)v_reduce_sum(acc);
            cho (; i < len; i++) {
                nếu (mask[i]) {
                    const uchar* elem = src + i * 4;
                    result += elem[0] + elem[1] + elem[2] + elem[3];
                }
            }
        }
        không_thì {
            cho (số_nguyên i = 0; i < len; i++) {
                nếu (mask[i]) {
                    const uchar* elem = src + i * cn;
                    cho (số_nguyên k = 0; k < cn; k++)
                        result += elem[k];
                }
            }
        }
        trả_về result;
    }
};

template <>
struct MaskedNormInf_SIMD<ushort, số_nguyên> {
    inline số_nguyên operator()(const ushort* src, const uchar* mask, số_nguyên len, số_nguyên cn) const {
        số_nguyên result = 0;
        nếu (cn == 1) {
            số_nguyên i = 0;
            const số_nguyên vstep = VTraits<v_uint16>::vlanes();
            v_uint16 acc = vx_setzero_u16();

            cho (; i <= len - vstep; i += vstep) {
                v_uint16 m   = vx_load_expand(mask + i);
                v_uint16 cmp = v_gt(m, vx_setzero_u16());
                v_uint16 s   = vx_load(src + i);
                v_uint16 sel = v_and(s, cmp);
                acc = v_max(acc, sel);
            }

            result = (số_nguyên)v_reduce_max(acc);

            cho (; i < len; i++) {
                nếu (mask[i])
                    result = std::max(result, (số_nguyên)src[i]);
            }
        }
        không_thì {
            cho (số_nguyên i = 0; i < len; i++) {
                nếu (mask[i]) {
                    const ushort* elem = src + i * cn;
                    số_nguyên k = 0;
                    const số_nguyên vstep = VTraits<v_uint16>::vlanes();
                    v_uint16 acc = vx_setzero_u16();

                    cho (; k <= cn - vstep; k += vstep) {
                        acc = v_max(acc, vx_load(elem + k));
                    }

                    result = std::max(result, (số_nguyên)v_reduce_max(acc));

                    cho (; k < cn; k++)
                        result = std::max(result, (số_nguyên)elem[k]);
                }
            }
        }
        trả_về result;
    }
};

template <>
struct MaskedNormL1_SIMD<ushort, số_nguyên> {
    inline số_nguyên operator()(const ushort* src, const uchar* mask, số_nguyên len, số_nguyên cn) const {
        số_nguyên result = 0;
        nếu (cn == 1) {
            số_nguyên i = 0;
            const số_nguyên vstep = VTraits<v_uint16>::vlanes();
            v_uint32 acc32 = vx_setzero_u32();
            v_uint64 acc64 = vx_setzero_u64();
            số_nguyên acc32_elems = 0;

            cho (; i <= len - vstep; i += vstep, acc32_elems += vstep) {
                nếu (acc32_elems >= 512) {
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
            result = (số_nguyên)v_reduce_sum(acc64);

            cho (; i < len; i++) {
                nếu (mask[i])
                    result += src[i];
            }
        }
        không_thì {
            cho (số_nguyên i = 0; i < len; i++) {
                nếu (mask[i]) {
                    const ushort* elem = src + i * cn;
                    số_nguyên k = 0;
                    const số_nguyên vstep = VTraits<v_uint16>::vlanes();
                    v_uint32 acc = vx_setzero_u32();

                    cho (; k <= cn - vstep; k += vstep) {
                        v_uint32 lo32, hi32;
                        v_expand(vx_load(elem + k), lo32, hi32);
                        acc = v_add(acc, v_add(lo32, hi32));
                    }

                    result += (số_nguyên)v_reduce_sum(acc);

                    cho (; k < cn; k++)
                        result += elem[k];
                }
            }
        }
        trả_về result;
    }
};

template <>
struct MaskedNormL2_SIMD<ushort, double> {
    inline double operator()(const ushort* src, const uchar* mask, số_nguyên len, số_nguyên cn) const {
        double result = 0.0;
        nếu (cn == 1) {
            số_nguyên i = 0;
            const số_nguyên vstep = VTraits<v_uint16>::vlanes();
            v_uint64 acc = vx_setzero_u64();
            cho (; i <= len - vstep; i += vstep) {
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
            result = (double)v_reduce_sum(acc);
            cho (; i < len; i++) {
                nếu (mask[i]) {
                    double v = src[i];
                    result += v * v;
                }
            }
        }
        không_thì {
            cho (số_nguyên i = 0; i < len; i++) {
                nếu (mask[i]) {
                    const ushort* elem = src + i * cn;
                    số_nguyên k = 0;
                    const số_nguyên vstep = VTraits<v_uint16>::vlanes();
                    v_uint64 acc = vx_setzero_u64();
                    cho (; k <= cn - vstep; k += vstep) {
                        v_uint32 lo32, hi32;
                        v_expand(vx_load(elem + k), lo32, hi32);
                        v_uint64 lo64a, lo64b, hi64a, hi64b;
                        v_expand(v_mul(lo32, lo32), lo64a, lo64b);
                        v_expand(v_mul(hi32, hi32), hi64a, hi64b);
                        acc = v_add(acc, v_add(v_add(lo64a, lo64b), v_add(hi64a, hi64b)));
                    }

                    result += (double)v_reduce_sum(acc);

                    cho (; k < cn; k++) {
                        double v = elem[k];
                        result += v * v;
                    }
                }
            }
        }
        trả_về result;
    }
};

template <>
struct MaskedNormInf_SIMD<schar, số_nguyên> {
    inline số_nguyên operator()(const schar* src, const uchar* mask, số_nguyên len, số_nguyên cn) const {
        số_nguyên result = 0;
        nếu (cn == 1) {
            const số_nguyên vstep = VTraits<v_int8>::vlanes();
            số_nguyên i = 0;
            v_uint8 acc = vx_setzero_u8();
            cho (; i <= len - vstep; i += vstep)
                acc = v_max(acc, v_and(v_abs(vx_load(src + i)), v_normmask_u8(mask + i)));
            result = (số_nguyên)v_reduce_max(acc);
            cho (; i < len; i++)
                nếu (mask[i]) result = std::max(result, cv_abs(src[i]));
        }
        không_thì {
            cho (số_nguyên i = 0; i < len; i++)
                nếu (mask[i]) {
                    const schar* elem = src + i * cn;
                    cho (số_nguyên k = 0; k < cn; k++)
                        result = std::max(result, cv_abs(elem[k]));
                }
        }
        trả_về result;
    }
};

template <>
struct MaskedNormL1_SIMD<schar, số_nguyên> {
    inline số_nguyên operator()(const schar* src, const uchar* mask, số_nguyên len, số_nguyên cn) const {
        số_nguyên result = 0;
        nếu (cn == 1) {
            const số_nguyên vstep = VTraits<v_int8>::vlanes();
            const v_uint8 one = vx_setall_u8(1);
            số_nguyên i = 0;
            v_uint32 acc = vx_setzero_u32();
            cho (; i <= len - vstep; i += vstep)
                acc = v_dotprod_expand_fast(v_and(v_abs(vx_load(src + i)), v_normmask_u8(mask + i)), one, acc);
            result = (số_nguyên)v_reduce_sum(acc);
            cho (; i < len; i++)
                nếu (mask[i]) result += cv_abs(src[i]);
        }
        không_thì {
            cho (số_nguyên i = 0; i < len; i++)
                nếu (mask[i]) {
                    const schar* elem = src + i * cn;
                    cho (số_nguyên k = 0; k < cn; k++)
                        result += cv_abs(elem[k]);
                }
        }
        trả_về result;
    }
};

template <>
struct MaskedNormL2_SIMD<schar, số_nguyên> {
    inline số_nguyên operator()(const schar* src, const uchar* mask, số_nguyên len, số_nguyên cn) const {
        số_nguyên result = 0;
        nếu (cn == 1) {
            const số_nguyên vstep = VTraits<v_int8>::vlanes();
            số_nguyên i = 0;
            v_int32 acc = vx_setzero_s32();
            cho (; i <= len - vstep; i += vstep) {
                v_int8 s = v_and(vx_load(src + i), v_reinterpret_as_s8(v_normmask_u8(mask + i)));
                acc = v_dotprod_expand_fast(s, s, acc);
            }
            result = (số_nguyên)v_reduce_sum(acc);
            cho (; i < len; i++)
                nếu (mask[i]) { số_nguyên v = src[i]; result += v * v; }
        }
        không_thì {
            cho (số_nguyên i = 0; i < len; i++)
                nếu (mask[i]) {
                    const schar* elem = src + i * cn;
                    cho (số_nguyên k = 0; k < cn; k++) { số_nguyên v = elem[k]; result += v * v; }
                }
        }
        trả_về result;
    }
};

template <>
struct MaskedNormL2_SIMD<uchar, số_nguyên> {
    inline số_nguyên operator()(const uchar* src, const uchar* mask, số_nguyên len, số_nguyên cn) const {
        số_nguyên result = 0;
        const số_nguyên vstep = VTraits<v_uint8>::vlanes();
        nếu (cn == 1) {
            số_nguyên i = 0;
            v_uint32 acc = vx_setzero_u32();
            cho (; i <= len - vstep; i += vstep) {
                v_uint8 s = v_and(vx_load(src + i), v_normmask_u8(mask + i));
                acc = v_dotprod_expand_fast(s, s, acc);
            }
            result = (số_nguyên)v_reduce_sum(acc);
            cho (; i < len; i++)
                nếu (mask[i]) { số_nguyên v = src[i]; result += v * v; }
        }
        không_thì nếu (cn == 4) {
            số_nguyên i = 0;
            v_uint32 acc = vx_setzero_u32();
            cho (; i <= len - vstep; i += vstep) {
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
            result = (số_nguyên)v_reduce_sum(acc);
            cho (; i < len; i++)
                nếu (mask[i]) {
                    const uchar* elem = src + i * 4;
                    cho (số_nguyên k = 0; k < 4; k++) { số_nguyên v = elem[k]; result += v * v; }
                }
        }
        không_thì {
            cho (số_nguyên i = 0; i < len; i++)
                nếu (mask[i]) {
                    const uchar* elem = src + i * cn;
                    cho (số_nguyên k = 0; k < cn; k++) { số_nguyên v = elem[k]; result += v * v; }
                }
        }
        trả_về result;
    }
};

template <>
struct MaskedNormInf_SIMD<short, số_nguyên> {
    inline số_nguyên operator()(const short* src, const uchar* mask, số_nguyên len, số_nguyên cn) const {
        số_nguyên result = 0;
        nếu (cn == 1) {
            const số_nguyên vstep = VTraits<v_int16>::vlanes();
            số_nguyên i = 0;
            v_uint16 acc = vx_setzero_u16();
            cho (; i <= len - vstep; i += vstep)
                acc = v_max(acc, v_and(v_abs(vx_load(src + i)), v_normmask_u16(mask + i)));
            result = (số_nguyên)v_reduce_max(acc);
            cho (; i < len; i++)
                nếu (mask[i]) result = std::max(result, cv_abs(src[i]));
        }
        không_thì {
            cho (số_nguyên i = 0; i < len; i++)
                nếu (mask[i]) {
                    const short* elem = src + i * cn;
                    cho (số_nguyên k = 0; k < cn; k++)
                        result = std::max(result, cv_abs(elem[k]));
                }
        }
        trả_về result;
    }
};

template <>
struct MaskedNormL1_SIMD<short, số_nguyên> {
    inline số_nguyên operator()(const short* src, const uchar* mask, số_nguyên len, số_nguyên cn) const {
        số_nguyên result = 0;
        nếu (cn == 1) {
            const số_nguyên vstep = VTraits<v_int16>::vlanes();
            số_nguyên i = 0;
            v_uint32 acc0 = vx_setzero_u32(), acc1 = vx_setzero_u32();
            cho (; i <= len - vstep; i += vstep) {
                v_uint16 s = v_and(v_abs(vx_load(src + i)), v_normmask_u16(mask + i));
                v_uint32 lo, hi;
                v_expand(s, lo, hi);
                acc0 = v_add(acc0, lo);
                acc1 = v_add(acc1, hi);
            }
            result = (số_nguyên)v_reduce_sum(v_add(acc0, acc1));
            cho (; i < len; i++)
                nếu (mask[i]) result += cv_abs(src[i]);
        }
        không_thì {
            cho (số_nguyên i = 0; i < len; i++)
                nếu (mask[i]) {
                    const short* elem = src + i * cn;
                    cho (số_nguyên k = 0; k < cn; k++)
                        result += cv_abs(elem[k]);
                }
        }
        trả_về result;
    }
};

template <>
struct MaskedNormL2_SIMD<short, double> {
    inline double operator()(const short* src, const uchar* mask, số_nguyên len, số_nguyên cn) const {
        double result = 0.0;
        nếu (cn == 1) {
            const số_nguyên vstep = VTraits<v_int16>::vlanes();
            số_nguyên i = 0;
            v_uint64 acc = vx_setzero_u64();
            cho (; i <= len - vstep; i += vstep) {
                v_uint16 s = v_and(v_abs(vx_load(src + i)), v_normmask_u16(mask + i));
                acc = v_add(acc, v_dotprod_expand_fast(s, s));
            }
            result = (double)v_reduce_sum(acc);
            cho (; i < len; i++)
                nếu (mask[i]) { double v = src[i]; result += v * v; }
        }
        không_thì {
            cho (số_nguyên i = 0; i < len; i++)
                nếu (mask[i]) {
                    const short* elem = src + i * cn;
                    cho (số_nguyên k = 0; k < cn; k++) { double v = elem[k]; result += v * v; }
                }
        }
        trả_về result;
    }
};

template <>
struct MaskedNormInf_SIMD<số_nguyên, unsigned> {
    inline unsigned operator()(const số_nguyên* src, const uchar* mask, số_nguyên len, số_nguyên cn) const {
        unsigned result = 0;
        nếu (cn == 1) {
            const số_nguyên vstep = VTraits<v_int32>::vlanes();
            số_nguyên i = 0;
            v_uint32 acc = vx_setzero_u32();
            cho (; i <= len - vstep; i += vstep)
                acc = v_max(acc, v_and(v_abs(vx_load(src + i)), v_normmask_u32(mask + i)));
            result = v_reduce_max(acc);
            cho (; i < len; i++)
                nếu (mask[i]) result = std::max(result, cv_abs(src[i]));
        }
        không_thì {
            cho (số_nguyên i = 0; i < len; i++)
                nếu (mask[i]) {
                    const số_nguyên* elem = src + i * cn;
                    cho (số_nguyên k = 0; k < cn; k++)
                        result = std::max(result, cv_abs(elem[k]));
                }
        }
        trả_về result;
    }
};

#nếu CV_SIMD_64F || CV_SIMD_SCALABLE_64F

template <>
struct MaskedNormL1_SIMD<số_nguyên, double> {
    inline double operator()(const số_nguyên* src, const uchar* mask, số_nguyên len, số_nguyên cn) const {
        double result = 0.0;
        nếu (cn == 1) {
            const số_nguyên vstep = VTraits<v_int32>::vlanes();
            số_nguyên i = 0;
            v_float64 acc0 = vx_setzero_f64(), acc1 = vx_setzero_f64();
            cho (; i <= len - vstep; i += vstep) {
                // Mask the value, then widen and take the magnitude in double. v_abs() does give
                // |INT_MIN| as an unsigned 2147483648, but reinterpreting that back as int32 turns
                // it negative again, and the widening then carries the sign into the accumulator.
                v_int32 s = v_reinterpret_as_s32(v_and(v_reinterpret_as_u32(vx_load(src + i)),
                                                       v_normmask_u32(mask + i)));
                acc0 = v_add(acc0, v_abs(v_cvt_f64(s)));
                acc1 = v_add(acc1, v_abs(v_cvt_f64_high(s)));
            }
            result = v_reduce_sum(v_add(acc0, acc1));
            cho (; i < len; i++)
                nếu (mask[i]) result += cv_abs(src[i]);
        }
        không_thì {
            cho (số_nguyên i = 0; i < len; i++)
                nếu (mask[i]) {
                    const số_nguyên* elem = src + i * cn;
                    cho (số_nguyên k = 0; k < cn; k++)
                        result += cv_abs(elem[k]);
                }
        }
        trả_về result;
    }
};

template <>
struct MaskedNormL2_SIMD<số_nguyên, double> {
    inline double operator()(const số_nguyên* src, const uchar* mask, số_nguyên len, số_nguyên cn) const {
        double result = 0.0;
        nếu (cn == 1) {
            const số_nguyên vstep = VTraits<v_int32>::vlanes();
            số_nguyên i = 0;
            v_float64 acc0 = vx_setzero_f64(), acc1 = vx_setzero_f64();
            cho (; i <= len - vstep; i += vstep) {
                v_uint32 s = v_and(v_abs(vx_load(src + i)), v_normmask_u32(mask + i));
                v_uint64 e0, e1;
                v_expand(s, e0, e1);
                v_float64 f0 = v_cvt_f64(v_reinterpret_as_s64(e0));
                v_float64 f1 = v_cvt_f64(v_reinterpret_as_s64(e1));
                acc0 = v_fma(f0, f0, acc0);
                acc1 = v_fma(f1, f1, acc1);
            }
            result = v_reduce_sum(v_add(acc0, acc1));
            cho (; i < len; i++)
                nếu (mask[i]) { double v = src[i]; result += v * v; }
        }
        không_thì {
            cho (số_nguyên i = 0; i < len; i++)
                nếu (mask[i]) {
                    const số_nguyên* elem = src + i * cn;
                    cho (số_nguyên k = 0; k < cn; k++) { double v = elem[k]; result += v * v; }
                }
        }
        trả_về result;
    }
};

template <>
struct MaskedNormInf_SIMD<double, double> {
    inline double operator()(const double* src, const uchar* mask, số_nguyên len, số_nguyên cn) const {
        double result = 0.0;
        nếu (cn == 1) {
            const số_nguyên vstep = VTraits<v_float64>::vlanes();
            số_nguyên i = 0;
            v_float64 acc0 = vx_setzero_f64(), acc1 = vx_setzero_f64();
            cho (; i <= len - 2*vstep; i += 2*vstep) {
                v_uint64 m0, m1;
                v_normmask_u64(mask + i, m0, m1);
                acc0 = v_max(acc0, v_reinterpret_as_f64(v_and(v_reinterpret_as_u64(v_abs(vx_load(src + i))), m0)));
                acc1 = v_max(acc1, v_reinterpret_as_f64(v_and(v_reinterpret_as_u64(v_abs(vx_load(src + i + vstep))), m1)));
            }
            double t[VTraits<v_float64>::max_nlanes];
            vx_store(t, v_max(acc0, acc1));
            cho (số_nguyên k = 0; k < vstep; k++)
                result = std::max(result, t[k]);
            cho (; i < len; i++)
                nếu (mask[i]) result = std::max(result, cv_abs(src[i]));
        }
        không_thì {
            cho (số_nguyên i = 0; i < len; i++)
                nếu (mask[i]) {
                    const double* elem = src + i * cn;
                    cho (số_nguyên k = 0; k < cn; k++)
                        result = std::max(result, cv_abs(elem[k]));
                }
        }
        trả_về result;
    }
};

template <>
struct MaskedNormL1_SIMD<double, double> {
    inline double operator()(const double* src, const uchar* mask, số_nguyên len, số_nguyên cn) const {
        double result = 0.0;
        nếu (cn == 1) {
            const số_nguyên vstep = VTraits<v_float64>::vlanes();
            số_nguyên i = 0;
            v_float64 acc0 = vx_setzero_f64(), acc1 = vx_setzero_f64();
            cho (; i <= len - 2*vstep; i += 2*vstep) {
                v_uint64 m0, m1;
                v_normmask_u64(mask + i, m0, m1);
                acc0 = v_add(acc0, v_reinterpret_as_f64(v_and(v_reinterpret_as_u64(v_abs(vx_load(src + i))), m0)));
                acc1 = v_add(acc1, v_reinterpret_as_f64(v_and(v_reinterpret_as_u64(v_abs(vx_load(src + i + vstep))), m1)));
            }
            result = v_reduce_sum(v_add(acc0, acc1));
            cho (; i < len; i++)
                nếu (mask[i]) result += cv_abs(src[i]);
        }
        không_thì {
            cho (số_nguyên i = 0; i < len; i++)
                nếu (mask[i]) {
                    const double* elem = src + i * cn;
                    cho (số_nguyên k = 0; k < cn; k++)
                        result += cv_abs(elem[k]);
                }
        }
        trả_về result;
    }
};

template <>
struct MaskedNormL2_SIMD<double, double> {
    inline double operator()(const double* src, const uchar* mask, số_nguyên len, số_nguyên cn) const {
        double result = 0.0;
        nếu (cn == 1) {
            const số_nguyên vstep = VTraits<v_float64>::vlanes();
            số_nguyên i = 0;
            v_float64 acc0 = vx_setzero_f64(), acc1 = vx_setzero_f64();
            cho (; i <= len - 2*vstep; i += 2*vstep) {
                v_uint64 m0, m1;
                v_normmask_u64(mask + i, m0, m1);
                v_float64 s0 = v_reinterpret_as_f64(v_and(v_reinterpret_as_u64(vx_load(src + i)), m0));
                v_float64 s1 = v_reinterpret_as_f64(v_and(v_reinterpret_as_u64(vx_load(src + i + vstep)), m1));
                acc0 = v_fma(s0, s0, acc0);
                acc1 = v_fma(s1, s1, acc1);
            }
            result = v_reduce_sum(v_add(acc0, acc1));
            cho (; i < len; i++)
                nếu (mask[i]) { double v = src[i]; result += v * v; }
        }
        không_thì {
            cho (số_nguyên i = 0; i < len; i++)
                nếu (mask[i]) {
                    const double* elem = src + i * cn;
                    cho (số_nguyên k = 0; k < cn; k++) { double v = elem[k]; result += v * v; }
                }
        }
        trả_về result;
    }
};

#endif // CV_SIMD_64F || CV_SIMD_SCALABLE_64F
#endif // CV_SIMD || CV_SIMD_SCALABLE

template<typename T, typename ST> số_nguyên
normInf_(const T* src, const uchar* mask, ST* _result, số_nguyên len, số_nguyên cn)
{
    ST result = *_result;
    nếu( !mask )
    {
        NormInf_SIMD<T, ST> op;
        result = std::max(result, op(src, len*cn));
    } không_thì {
        MaskedNormInf_SIMD<T, ST> op;
        result = std::max(result, op(src, mask, len, cn));
    }
    *_result = result;
    trả_về 0;
}

template<typename T, typename ST> số_nguyên
normL1_(const T* src, const uchar* mask, ST* _result, số_nguyên len, số_nguyên cn)
{
    ST result = *_result;
    nếu( !mask )
    {
        NormL1_SIMD<T, ST> op;
        result += op(src, len*cn);
    } không_thì {
        MaskedNormL1_SIMD<T, ST> op;
        result += op(src, mask, len, cn);
    }
    *_result = result;
    trả_về 0;
}

template<typename T, typename ST> số_nguyên
normL2_(const T* src, const uchar* mask, ST* _result, số_nguyên len, số_nguyên cn)
{
    ST result = *_result;
    nếu( !mask )
    {
        NormL2_SIMD<T, ST> op;
        result += op(src, len*cn);
    }
    không_thì
    {
        MaskedNormL2_SIMD<T, ST> op;
        result += op(src, mask, len, cn);
    }
    *_result = result;
    trả_về 0;
}

static số_nguyên
normInf_Bool(const uchar* src, const uchar* mask, số_nguyên* _result, số_nguyên len, số_nguyên cn)
{
    số_nguyên result = *_result;
    nếu( !mask )
    {
        cho ( số_nguyên i = 0; i < len*cn; i++ ) {
            result = std::max(result, (số_nguyên)(src[i] != 0));
            nếu (result != 0)
                break;
        }
    }
    không_thì
    {
        cho( số_nguyên i = 0; i < len; i++, src += cn )
            nếu( mask[i] )
            {
                cho( số_nguyên k = 0; k < cn; k++ )
                    result = std::max(result, (số_nguyên)(src[k] != 0));
                nếu (result != 0)
                    break;
            }
    }
    *_result = result;
    trả_về 0;
}

static số_nguyên
normL1_Bool(const uchar* src, const uchar* mask, số_nguyên* _result, số_nguyên len, số_nguyên cn)
{
    số_nguyên result = *_result;
    nếu( !mask )
    {
        cho ( số_nguyên i = 0; i < len*cn; i++ )
            result += (số_nguyên)(src[i] != 0);
    }
    không_thì
    {
        cho( số_nguyên i = 0; i < len; i++, src += cn )
            nếu( mask[i] )
            {
                cho( số_nguyên k = 0; k < cn; k++ )
                    result += (số_nguyên)(src[k] != 0);
            }
    }
    *_result = result;
    trả_về 0;
}

static số_nguyên
normL2_Bool(const uchar* src, const uchar* mask, số_nguyên* _result, số_nguyên len, số_nguyên cn)
{
    trả_về normL1_Bool(src, mask, _result, len, cn);
}

// ====================================================================
// Masked norm-diff kernels.
// Scalar base templates (safe for every depth) plus SIMD cn==1
// specializations for the concrete (T, ST) pairs dispatched by
// CV_DEF_NORM_ALL. The masked normDiff*_ functions below select them.
// ====================================================================
template <typename T, typename ST>
struct MaskedNormDiffInf_SIMD {
    inline ST operator()(const T* s1, const T* s2, const uchar* mask, số_nguyên len, số_nguyên cn) const {
        ST result = 0;
        cho (số_nguyên i = 0; i < len; i++) nếu (mask[i]) {
            const T* e1 = s1 + i*cn; const T* e2 = s2 + i*cn;
            cho (số_nguyên k = 0; k < cn; k++)
                result = std::max(result, (ST)cv_absdiff(e1[k], e2[k]));
        }
        trả_về result;
    }
};
template <typename T, typename ST>
struct MaskedNormDiffL1_SIMD {
    inline ST operator()(const T* s1, const T* s2, const uchar* mask, số_nguyên len, số_nguyên cn) const {
        ST result = 0;
        cho (số_nguyên i = 0; i < len; i++) nếu (mask[i]) {
            const T* e1 = s1 + i*cn; const T* e2 = s2 + i*cn;
            cho (số_nguyên k = 0; k < cn; k++)
                result += (ST)cv_absdiff(e1[k], e2[k]);
        }
        trả_về result;
    }
};
template <typename T, typename ST>
struct MaskedNormDiffL2_SIMD {
    inline ST operator()(const T* s1, const T* s2, const uchar* mask, số_nguyên len, số_nguyên cn) const {
        ST result = 0;
        cho (số_nguyên i = 0; i < len; i++) nếu (mask[i]) {
            const T* e1 = s1 + i*cn; const T* e2 = s2 + i*cn;
            cho (số_nguyên k = 0; k < cn; k++) { ST v = (ST)e1[k] - (ST)e2[k]; result += v*v; }
        }
        trả_về result;
    }
};

#nếu (CV_SIMD || CV_SIMD_SCALABLE)
// AND a per-lane diff vector with the (expanded) mask>0 predicate.
inline v_uint8  v_diffmask(const v_uint8&  d, const uchar* m) { trả_về v_and(d, v_gt(vx_load(m),          vx_setzero_u8())); }
inline v_uint16 v_diffmask(const v_uint16& d, const uchar* m) { trả_về v_and(d, v_gt(vx_load_expand(m),   vx_setzero_u16())); }
inline v_uint32 v_diffmask(const v_uint32& d, const uchar* m) { trả_về v_and(d, v_gt(vx_load_expand_q(m), vx_setzero_u32())); }
inline v_float32 v_diffmask(const v_float32& d, const uchar* m) {
    v_uint32 cm = v_gt(vx_load_expand_q(m), vx_setzero_u32());
    trả_về v_reinterpret_as_f32(v_and(v_reinterpret_as_u32(d), cm));
}

// Shared scalar cn>1 fallbacks (match the base templates' cv_absdiff semantics).
template<typename T, typename RT>
static inline RT maskedNormDiffInfTail(const T* s1, const T* s2, const uchar* mask, số_nguyên len, số_nguyên cn, RT result) {
    cho (số_nguyên i = 0; i < len; i++) nếu (mask[i]) {
        const T* e1 = s1 + i*cn; const T* e2 = s2 + i*cn;
        cho (số_nguyên k = 0; k < cn; k++) result = std::max(result, (RT)cv_absdiff(e1[k], e2[k]));
    }
    trả_về result;
}
template<typename T, typename RT>
static inline RT maskedNormDiffL1Tail(const T* s1, const T* s2, const uchar* mask, số_nguyên len, số_nguyên cn, RT result) {
    cho (số_nguyên i = 0; i < len; i++) nếu (mask[i]) {
        const T* e1 = s1 + i*cn; const T* e2 = s2 + i*cn;
        cho (số_nguyên k = 0; k < cn; k++) result += (RT)cv_absdiff(e1[k], e2[k]);
    }
    trả_về result;
}
template<typename T, typename RT>
static inline RT maskedNormDiffL2Tail(const T* s1, const T* s2, const uchar* mask, số_nguyên len, số_nguyên cn, RT result) {
    cho (số_nguyên i = 0; i < len; i++) nếu (mask[i]) {
        const T* e1 = s1 + i*cn; const T* e2 = s2 + i*cn;
        cho (số_nguyên k = 0; k < cn; k++) { RT v = (RT)e1[k] - (RT)e2[k]; result += v*v; }
    }
    trả_về result;
}

// Inf (cn==1): reduce_max of masked |a-b|. Shared across SIMD-capable depths.
template<typename T, typename ST>
static inline ST maskedNormDiffInfCn1(const T* s1, const T* s2, const uchar* mask, số_nguyên len) {
    const số_nguyên vstep = VTraits<decltype(vx_load(s1))>::vlanes();
    ST result = 0;
    số_nguyên i = 0;
    nếu (len >= vstep) {
        auto acc = v_diffmask(v_absdiff(vx_load(s1), vx_load(s2)), mask);
        cho (i = vstep; i <= len - vstep; i += vstep)
            acc = v_max(acc, v_diffmask(v_absdiff(vx_load(s1 + i), vx_load(s2 + i)), mask + i));
        result = (ST)v_reduce_max(acc);
    }
    cho (; i < len; i++) nếu (mask[i]) result = std::max(result, (ST)cv_absdiff(s1[i], s2[i]));
    trả_về result;
}

// Shared 8-bit cn==1 masked L1/L2 kernels (uchar/schar): v_absdiff yields v_uint8 for both.
template<typename T>
static inline số_nguyên maskedNormDiffL1_8(const T* s1, const T* s2, const uchar* mask, số_nguyên len) {
    số_nguyên i = 0; const số_nguyên vstep = VTraits<v_uint8>::vlanes();
    const v_uint8 one = vx_setall_u8(1);
    v_uint32 acc = vx_setzero_u32();
    cho (; i <= len - vstep; i += vstep) {
        v_uint8 m  = v_gt(vx_load(mask + i), vx_setzero_u8());
        v_uint8 ad = v_and(v_absdiff(vx_load(s1 + i), vx_load(s2 + i)), m);
        acc = v_dotprod_expand_fast(ad, one, acc);
    }
    số_nguyên result = (số_nguyên)v_reduce_sum(acc);
    cho (; i < len; i++) nếu (mask[i]) result += (số_nguyên)cv_absdiff(s1[i], s2[i]);
    trả_về result;
}
template<typename T>
static inline số_nguyên maskedNormDiffL2_8(const T* s1, const T* s2, const uchar* mask, số_nguyên len) {
    số_nguyên i = 0; const số_nguyên vstep = VTraits<v_uint8>::vlanes();
    v_uint32 acc = vx_setzero_u32();
    cho (; i <= len - vstep; i += vstep) {
        v_uint8 m  = v_gt(vx_load(mask + i), vx_setzero_u8());
        v_uint8 ad = v_and(v_absdiff(vx_load(s1 + i), vx_load(s2 + i)), m);
        acc = v_dotprod_expand_fast(ad, ad, acc);
    }
    số_nguyên result = (số_nguyên)v_reduce_sum(acc);
    cho (; i < len; i++) nếu (mask[i]) { số_nguyên v = (số_nguyên)s1[i] - (số_nguyên)s2[i]; result += v*v; }
    trả_về result;
}

// Shared 16-bit cn==1 masked L1 kernel (ushort/short): v_absdiff yields v_uint16 for both.
template<typename T>
static inline số_nguyên maskedNormDiffL1_16(const T* s1, const T* s2, const uchar* mask, số_nguyên len) {
    số_nguyên i = 0; const số_nguyên vstep = VTraits<v_uint16>::vlanes();
    v_uint32 acc = vx_setzero_u32();
    cho (; i <= len - vstep; i += vstep) {
        v_uint16 m  = v_gt(vx_load_expand(mask + i), vx_setzero_u16());
        v_uint16 ad = v_and(v_absdiff(vx_load(s1 + i), vx_load(s2 + i)), m);
        v_uint32 lo, hi; v_expand(ad, lo, hi);
        acc = v_add(acc, v_add(lo, hi));
    }
    số_nguyên result = (số_nguyên)v_reduce_sum(acc);
    cho (; i < len; i++) nếu (mask[i]) result += (số_nguyên)cv_absdiff(s1[i], s2[i]);
    trả_về result;
}

template<> struct MaskedNormDiffInf_SIMD<uchar, số_nguyên> {
    số_nguyên operator()(const uchar* s1, const uchar* s2, const uchar* mask, số_nguyên len, số_nguyên cn) const {
        nếu (cn == 1) trả_về maskedNormDiffInfCn1<uchar, số_nguyên>(s1, s2, mask, len);
        nếu (cn == 4 && len >= VTraits<v_uint8>::vlanes()) {
            const số_nguyên vstep = VTraits<v_uint8>::vlanes();
            v_uint8 acc = vx_setzero_u8();
            số_nguyên i = 0;
            cho (;;) {
                nếu (i > len - vstep) i = len - vstep; // back-step (max is idempotent)
                v_uint8 a0,a1,a2,a3,b0,b1,b2,b3;
                v_load_deinterleave(s1 + i*4, a0,a1,a2,a3);
                v_load_deinterleave(s2 + i*4, b0,b1,b2,b3);
                v_uint8 ad = v_max(v_max(v_absdiff(a0,b0), v_absdiff(a1,b1)),
                                   v_max(v_absdiff(a2,b2), v_absdiff(a3,b3)));
                v_uint8 m = v_gt(vx_load(mask + i), vx_setzero_u8());
                acc = v_max(acc, v_and(ad, m));
                nếu (i >= len - vstep) break;
                i += vstep;
            }
            trả_về (số_nguyên)v_reduce_max(acc);
        }
        trả_về maskedNormDiffInfTail<uchar, số_nguyên>(s1, s2, mask, len, cn, 0);
    }
};
template<> struct MaskedNormDiffInf_SIMD<schar, số_nguyên> {
    số_nguyên operator()(const schar* s1, const schar* s2, const uchar* mask, số_nguyên len, số_nguyên cn) const {
        nếu (cn == 1) trả_về maskedNormDiffInfCn1<schar, số_nguyên>(s1, s2, mask, len);
        trả_về maskedNormDiffInfTail<schar, số_nguyên>(s1, s2, mask, len, cn, 0);
    }
};
template<> struct MaskedNormDiffInf_SIMD<ushort, số_nguyên> {
    số_nguyên operator()(const ushort* s1, const ushort* s2, const uchar* mask, số_nguyên len, số_nguyên cn) const {
        nếu (cn == 1) trả_về maskedNormDiffInfCn1<ushort, số_nguyên>(s1, s2, mask, len);
        trả_về maskedNormDiffInfTail<ushort, số_nguyên>(s1, s2, mask, len, cn, 0);
    }
};
template<> struct MaskedNormDiffInf_SIMD<short, số_nguyên> {
    số_nguyên operator()(const short* s1, const short* s2, const uchar* mask, số_nguyên len, số_nguyên cn) const {
        nếu (cn == 1) trả_về maskedNormDiffInfCn1<short, số_nguyên>(s1, s2, mask, len);
        trả_về maskedNormDiffInfTail<short, số_nguyên>(s1, s2, mask, len, cn, 0);
    }
};
template<> struct MaskedNormDiffInf_SIMD<số_nguyên, unsigned> {
    unsigned operator()(const số_nguyên* s1, const số_nguyên* s2, const uchar* mask, số_nguyên len, số_nguyên cn) const {
        nếu (cn == 1) trả_về maskedNormDiffInfCn1<số_nguyên, unsigned>(s1, s2, mask, len);
        trả_về maskedNormDiffInfTail<số_nguyên, unsigned>(s1, s2, mask, len, cn, 0u);
    }
};
template<> struct MaskedNormDiffInf_SIMD<float, float> {
    float operator()(const float* s1, const float* s2, const uchar* mask, số_nguyên len, số_nguyên cn) const {
        nếu (cn == 1) trả_về maskedNormDiffInfCn1<float, float>(s1, s2, mask, len);
        trả_về maskedNormDiffInfTail<float, float>(s1, s2, mask, len, cn, 0.0f);
    }
};

template<> struct MaskedNormDiffL1_SIMD<uchar, số_nguyên> {
    số_nguyên operator()(const uchar* s1, const uchar* s2, const uchar* mask, số_nguyên len, số_nguyên cn) const {
        nếu (cn == 1) trả_về maskedNormDiffL1_8(s1, s2, mask, len);
        nếu (cn == 4) {
            số_nguyên i = 0; const số_nguyên vstep = VTraits<v_uint8>::vlanes();
            const v_uint8 one = vx_setall_u8(1);
            v_uint32 acc = vx_setzero_u32();
            cho (; i <= len - vstep; i += vstep) {
                v_uint8 a0,a1,a2,a3,b0,b1,b2,b3;
                v_load_deinterleave(s1 + i*4, a0,a1,a2,a3);
                v_load_deinterleave(s2 + i*4, b0,b1,b2,b3);
                v_uint8 m = v_gt(vx_load(mask + i), vx_setzero_u8());
                acc = v_dotprod_expand_fast(v_and(v_absdiff(a0,b0), m), one, acc);
                acc = v_dotprod_expand_fast(v_and(v_absdiff(a1,b1), m), one, acc);
                acc = v_dotprod_expand_fast(v_and(v_absdiff(a2,b2), m), one, acc);
                acc = v_dotprod_expand_fast(v_and(v_absdiff(a3,b3), m), one, acc);
            }
            số_nguyên result = (số_nguyên)v_reduce_sum(acc);
            cho (; i < len; i++) nếu (mask[i]) {
                const uchar* e1 = s1 + i*4; const uchar* e2 = s2 + i*4;
                cho (số_nguyên k = 0; k < 4; k++) result += (số_nguyên)cv_absdiff(e1[k], e2[k]);
            }
            trả_về result;
        }
        trả_về maskedNormDiffL1Tail<uchar, số_nguyên>(s1, s2, mask, len, cn, 0);
    }
};
template<> struct MaskedNormDiffL1_SIMD<schar, số_nguyên> {
    số_nguyên operator()(const schar* s1, const schar* s2, const uchar* mask, số_nguyên len, số_nguyên cn) const {
        nếu (cn == 1) trả_về maskedNormDiffL1_8(s1, s2, mask, len);
        trả_về maskedNormDiffL1Tail<schar, số_nguyên>(s1, s2, mask, len, cn, 0);
    }
};
template<> struct MaskedNormDiffL1_SIMD<ushort, số_nguyên> {
    số_nguyên operator()(const ushort* s1, const ushort* s2, const uchar* mask, số_nguyên len, số_nguyên cn) const {
        nếu (cn == 1) trả_về maskedNormDiffL1_16(s1, s2, mask, len);
        trả_về maskedNormDiffL1Tail<ushort, số_nguyên>(s1, s2, mask, len, cn, 0);
    }
};
template<> struct MaskedNormDiffL1_SIMD<short, số_nguyên> {
    số_nguyên operator()(const short* s1, const short* s2, const uchar* mask, số_nguyên len, số_nguyên cn) const {
        nếu (cn == 1) trả_về maskedNormDiffL1_16(s1, s2, mask, len);
        trả_về maskedNormDiffL1Tail<short, số_nguyên>(s1, s2, mask, len, cn, 0);
    }
};

template<> struct MaskedNormDiffL2_SIMD<uchar, số_nguyên> {
    số_nguyên operator()(const uchar* s1, const uchar* s2, const uchar* mask, số_nguyên len, số_nguyên cn) const {
        nếu (cn == 1) trả_về maskedNormDiffL2_8(s1, s2, mask, len);
        nếu (cn == 4) {
            số_nguyên i = 0; const số_nguyên vstep = VTraits<v_uint8>::vlanes();
            v_uint32 acc = vx_setzero_u32();
            cho (; i <= len - vstep; i += vstep) {
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
            số_nguyên result = (số_nguyên)v_reduce_sum(acc);
            cho (; i < len; i++) nếu (mask[i]) {
                const uchar* e1 = s1 + i*4; const uchar* e2 = s2 + i*4;
                cho (số_nguyên k = 0; k < 4; k++) { số_nguyên v = (số_nguyên)e1[k] - (số_nguyên)e2[k]; result += v*v; }
            }
            trả_về result;
        }
        trả_về maskedNormDiffL2Tail<uchar, số_nguyên>(s1, s2, mask, len, cn, 0);
    }
};
template<> struct MaskedNormDiffL2_SIMD<schar, số_nguyên> {
    số_nguyên operator()(const schar* s1, const schar* s2, const uchar* mask, số_nguyên len, số_nguyên cn) const {
        nếu (cn == 1) trả_về maskedNormDiffL2_8(s1, s2, mask, len);
        trả_về maskedNormDiffL2Tail<schar, số_nguyên>(s1, s2, mask, len, cn, 0);
    }
};

#nếu CV_SIMD_64F
// Depths whose L1/L2 accumulate in double.
template<> struct MaskedNormDiffL1_SIMD<số_nguyên, double> {
    double operator()(const số_nguyên* s1, const số_nguyên* s2, const uchar* mask, số_nguyên len, số_nguyên cn) const {
        nếu (cn != 1) trả_về maskedNormDiffL1Tail<số_nguyên, double>(s1, s2, mask, len, cn, 0.0);
        số_nguyên i = 0; const số_nguyên vstep = VTraits<v_int32>::vlanes();
        v_float64 acc = vx_setzero_f64();
        cho (; i <= len - vstep; i += vstep) {
            v_uint32 m  = v_gt(vx_load_expand_q(mask + i), vx_setzero_u32());
            v_uint32 ad = v_and(v_absdiff(vx_load(s1 + i), vx_load(s2 + i)), m);
            v_uint64 e0, e1; v_expand(ad, e0, e1);
            acc = v_add(acc, v_cvt_f64(v_reinterpret_as_s64(e0)));
            acc = v_add(acc, v_cvt_f64(v_reinterpret_as_s64(e1)));
        }
        double result = v_reduce_sum(acc);
        cho (; i < len; i++) nếu (mask[i]) result += (double)cv_absdiff(s1[i], s2[i]);
        trả_về result;
    }
};
template<> struct MaskedNormDiffL1_SIMD<float, double> {
    double operator()(const float* s1, const float* s2, const uchar* mask, số_nguyên len, số_nguyên cn) const {
        nếu (cn != 1) trả_về maskedNormDiffL1Tail<float, double>(s1, s2, mask, len, cn, 0.0);
        số_nguyên i = 0; const số_nguyên vstep = VTraits<v_float32>::vlanes();
        v_float64 acc0 = vx_setzero_f64(), acc1 = vx_setzero_f64();
        cho (; i <= len - vstep; i += vstep) {
            v_float32 m  = v_reinterpret_as_f32(v_gt(vx_load_expand_q(mask + i), vx_setzero_u32()));
            v_float32 ad = v_and(v_absdiff(vx_load(s1 + i), vx_load(s2 + i)), m);
            acc0 = v_add(acc0, v_cvt_f64(ad));
            acc1 = v_add(acc1, v_cvt_f64_high(ad));
        }
        double result = v_reduce_sum(v_add(acc0, acc1));
        cho (; i < len; i++) nếu (mask[i]) result += (double)cv_absdiff(s1[i], s2[i]);
        trả_về result;
    }
};

template<typename T>
static inline double maskedNormDiffL2_16(const T* s1, const T* s2, const uchar* mask, số_nguyên len, số_nguyên cn) {
    nếu (cn != 1) trả_về maskedNormDiffL2Tail<T, double>(s1, s2, mask, len, cn, 0.0);
    số_nguyên i = 0; const số_nguyên vstep = VTraits<v_uint16>::vlanes();
    v_float64 acc = vx_setzero_f64();
    cho (; i <= len - vstep; i += vstep) {
        v_uint16 m  = v_gt(vx_load_expand(mask + i), vx_setzero_u16());
        v_uint16 ad = v_and(v_absdiff(vx_load(s1 + i), vx_load(s2 + i)), m);
        v_uint64 u = v_dotprod_expand_fast(ad, ad);
        acc = v_add(acc, v_cvt_f64(v_reinterpret_as_s64(u)));
    }
    double result = v_reduce_sum(acc);
    cho (; i < len; i++) nếu (mask[i]) { double v = (double)s1[i] - (double)s2[i]; result += v*v; }
    trả_về result;
}
template<> struct MaskedNormDiffL2_SIMD<ushort, double> {
    double operator()(const ushort* s1, const ushort* s2, const uchar* mask, số_nguyên len, số_nguyên cn) const
    { trả_về maskedNormDiffL2_16(s1, s2, mask, len, cn); } };
template<> struct MaskedNormDiffL2_SIMD<short, double> {
    double operator()(const short* s1, const short* s2, const uchar* mask, số_nguyên len, số_nguyên cn) const
    { trả_về maskedNormDiffL2_16(s1, s2, mask, len, cn); } };

template<> struct MaskedNormDiffL2_SIMD<số_nguyên, double> {
    double operator()(const số_nguyên* s1, const số_nguyên* s2, const uchar* mask, số_nguyên len, số_nguyên cn) const {
        nếu (cn != 1) trả_về maskedNormDiffL2Tail<số_nguyên, double>(s1, s2, mask, len, cn, 0.0);
        số_nguyên i = 0; const số_nguyên vstep = VTraits<v_int32>::vlanes();
        v_float64 r0 = vx_setzero_f64(), r1 = vx_setzero_f64();
        cho (; i <= len - vstep; i += vstep) {
            v_uint32 m  = v_gt(vx_load_expand_q(mask + i), vx_setzero_u32());
            v_uint32 ad = v_and(v_absdiff(vx_load(s1 + i), vx_load(s2 + i)), m);
            v_uint64 e0, e1; v_expand(ad, e0, e1);
            v_float64 f0 = v_cvt_f64(v_reinterpret_as_s64(e0)), f1 = v_cvt_f64(v_reinterpret_as_s64(e1));
            r0 = v_fma(f0, f0, r0); r1 = v_fma(f1, f1, r1);
        }
        double result = v_reduce_sum(v_add(r0, r1));
        cho (; i < len; i++) nếu (mask[i]) { double v = (double)s1[i] - (double)s2[i]; result += v*v; }
        trả_về result;
    }
};
template<> struct MaskedNormDiffL2_SIMD<float, double> {
    double operator()(const float* s1, const float* s2, const uchar* mask, số_nguyên len, số_nguyên cn) const {
        nếu (cn != 1) trả_về maskedNormDiffL2Tail<float, double>(s1, s2, mask, len, cn, 0.0);
        số_nguyên i = 0; const số_nguyên vstep = VTraits<v_float32>::vlanes();
        v_float64 r0 = vx_setzero_f64(), r1 = vx_setzero_f64();
        cho (; i <= len - vstep; i += vstep) {
            v_float32 m  = v_reinterpret_as_f32(v_gt(vx_load_expand_q(mask + i), vx_setzero_u32()));
            v_float32 ad = v_and(v_absdiff(vx_load(s1 + i), vx_load(s2 + i)), m);
            v_float64 f0 = v_cvt_f64(ad), f1 = v_cvt_f64_high(ad);
            r0 = v_fma(f0, f0, r0); r1 = v_fma(f1, f1, r1);
        }
        double result = v_reduce_sum(v_add(r0, r1));
        cho (; i < len; i++) nếu (mask[i]) { double v = (double)s1[i] - (double)s2[i]; result += v*v; }
        trả_về result;
    }
};
template<>
struct MaskedNormDiffInf_SIMD<double, double> {
    inline double operator()(const double* s1, const double* s2, const uchar* mask, số_nguyên len, số_nguyên cn) const {
        double result = 0.0;
        số_nguyên i = 0;
#nếu CV_SIMD_64F || CV_SIMD_SCALABLE_64F
        nếu (cn == 1) {
            const số_nguyên vstep = VTraits<v_float64>::vlanes();
            v_float64 acc0 = vx_setzero_f64(), acc1 = vx_setzero_f64();
            cho (; i <= len - 2*vstep; i += 2*vstep) {
                v_uint64 m0, m1;
                v_normmask_u64(mask + i, m0, m1);
                v_float64 d0 = v_absdiff(vx_load(s1 + i), vx_load(s2 + i));
                v_float64 d1 = v_absdiff(vx_load(s1 + i + vstep), vx_load(s2 + i + vstep));
                acc0 = v_max(acc0, v_reinterpret_as_f64(v_and(v_reinterpret_as_u64(d0), m0)));
                acc1 = v_max(acc1, v_reinterpret_as_f64(v_and(v_reinterpret_as_u64(d1), m1)));
            }
            double t[VTraits<v_float64>::max_nlanes];
            vx_store(t, v_max(acc0, acc1));
            cho (số_nguyên k = 0; k < vstep; k++)
                result = std::max(result, t[k]);
        }
#endif
        cho (; i < len; i++) nếu (mask[i]) {
            const double* e1 = s1 + i*cn; const double* e2 = s2 + i*cn;
            cho (số_nguyên k = 0; k < cn; k++) result = std::max(result, std::abs(e1[k] - e2[k]));
        }
        trả_về result;
    }
};

template<>
struct MaskedNormDiffL1_SIMD<double, double> {
    double operator()(const double* s1, const double* s2, const uchar* mask, số_nguyên len, số_nguyên cn) const {
        double result = 0.0;
        nếu (cn == 1) {
            const số_nguyên vstep = VTraits<v_float64>::vlanes();
            số_nguyên i = 0;
            v_float64 acc0 = vx_setzero_f64(), acc1 = vx_setzero_f64();
            cho (; i <= len - 2*vstep; i += 2*vstep) {
                v_uint64 m0, m1;
                v_normmask_u64(mask + i, m0, m1);
                v_float64 d0 = v_absdiff(vx_load(s1 + i), vx_load(s2 + i));
                v_float64 d1 = v_absdiff(vx_load(s1 + i + vstep), vx_load(s2 + i + vstep));
                acc0 = v_add(acc0, v_reinterpret_as_f64(v_and(v_reinterpret_as_u64(d0), m0)));
                acc1 = v_add(acc1, v_reinterpret_as_f64(v_and(v_reinterpret_as_u64(d1), m1)));
            }
            result = v_reduce_sum(v_add(acc0, acc1));
            cho (; i < len; i++) nếu (mask[i]) result += std::abs(s1[i] - s2[i]);
        } không_thì {
            result = maskedNormDiffL1Tail<double, double>(s1, s2, mask, len, cn, 0.0);
        }
        trả_về result;
    }
};

template<>
struct MaskedNormDiffL2_SIMD<double, double> {
    double operator()(const double* s1, const double* s2, const uchar* mask, số_nguyên len, số_nguyên cn) const {
        double result = 0.0;
        nếu (cn == 1) {
            const số_nguyên vstep = VTraits<v_float64>::vlanes();
            số_nguyên i = 0;
            v_float64 acc0 = vx_setzero_f64(), acc1 = vx_setzero_f64();
            cho (; i <= len - 2*vstep; i += 2*vstep) {
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
            cho (; i < len; i++) nếu (mask[i]) { double v = s1[i] - s2[i]; result += v*v; }
        } không_thì {
            result = maskedNormDiffL2Tail<double, double>(s1, s2, mask, len, cn, 0.0);
        }
        trả_về result;
    }
};
#endif // CV_SIMD_64F
#endif // CV_SIMD

template<typename T, typename ST> số_nguyên
normDiffInf_(const T* src1, const T* src2, const uchar* mask, ST* _result, số_nguyên len, số_nguyên cn) {
    ST result = *_result;
    nếu( !mask ) {
        NormDiffInf_SIMD<T, ST> op;
        result = std::max(result, op(src1, src2, len*cn));
    } không_thì {
        MaskedNormDiffInf_SIMD<T, ST> op;
        result = std::max(result, op(src1, src2, mask, len, cn));
    }
    *_result = result;
    trả_về 0;
}

template<typename T, typename ST> số_nguyên
normDiffL1_(const T* src1, const T* src2, const uchar* mask, ST* _result, số_nguyên len, số_nguyên cn) {
    ST result = *_result;
    nếu( !mask ) {
        NormDiffL1_SIMD<T, ST> op;
        result += op(src1, src2, len*cn);
    } không_thì {
        MaskedNormDiffL1_SIMD<T, ST> op;
        result += op(src1, src2, mask, len, cn);
    }
    *_result = result;
    trả_về 0;
}

template<typename T, typename ST> số_nguyên
normDiffL2_(const T* src1, const T* src2, const uchar* mask, ST* _result, số_nguyên len, số_nguyên cn) {
    ST result = *_result;
    nếu( !mask ) {
        NormDiffL2_SIMD<T, ST> op;
        result += op(src1, src2, len*cn);
    } không_thì {
        MaskedNormDiffL2_SIMD<T, ST> op;
        result += op(src1, src2, mask, len, cn);
    }
    *_result = result;
    trả_về 0;
}

static số_nguyên
normDiffInf_Bool(const uchar* src1, const uchar* src2, const uchar* mask, số_nguyên* _result, số_nguyên len, số_nguyên cn)
{
    số_nguyên result = *_result;
    nếu( !mask )
    {
        cho( số_nguyên i = 0; i < len*cn; i++ ) {
            result = std::max(result, (số_nguyên)((src1[i] != 0) != (src2[i] != 0)));
            nếu (result != 0)
                break;
        }
    }
    không_thì
    {
        cho( số_nguyên i = 0; i < len; i++, src1 += cn, src2 += cn )
            nếu( mask[i] )
            {
                cho( số_nguyên k = 0; k < cn; k++ )
                    result = std::max(result, (số_nguyên)((src1[k] != 0) != (src2[k] != 0)));
                nếu (result != 0)
                    break;
            }
    }
    *_result = result;
    trả_về 0;
}

static số_nguyên
normDiffL1_Bool(const uchar* src1, const uchar* src2, const uchar* mask, số_nguyên* _result, số_nguyên len, số_nguyên cn)
{
    số_nguyên result = *_result;
    nếu( !mask )
    {
        cho( số_nguyên i = 0; i < len*cn; i++ )
            result += (số_nguyên)((src1[i] != 0) != (src2[i] != 0));
    }
    không_thì
    {
        cho( số_nguyên i = 0; i < len; i++, src1 += cn, src2 += cn )
            nếu( mask[i] )
            {
                cho( số_nguyên k = 0; k < cn; k++ )
                    result += (số_nguyên)((src1[k] != 0) != (src2[k] != 0));
            }
    }
    *_result = result;
    trả_về 0;
}

static số_nguyên
normDiffL2_Bool(const uchar* src1, const uchar* src2, const uchar* mask, số_nguyên* _result, số_nguyên len, số_nguyên cn)
{
    trả_về normDiffL1_Bool(src1, src2, mask, _result, len, cn);
}

#define CV_DEF_NORM_FUNC(L, suffix, type, ntype) \
    static số_nguyên norm##L##_##suffix(const type* src, const uchar* mask, ntype* r, số_nguyên len, số_nguyên cn) \
{ CV_INSTRUMENT_REGION(); trả_về norm##L##_(src, mask, r, len, cn); } \
    static số_nguyên normDiff##L##_##suffix(const type* src1, const type* src2, \
    const uchar* mask, ntype* r, số_nguyên len, số_nguyên cn) \
{ trả_về normDiff##L##_(src1, src2, mask, r, (số_nguyên)len, cn); }

#define CV_DEF_NORM_ALL(suffix, type, inftype, l1type, l2type) \
    CV_DEF_NORM_FUNC(Inf, suffix, type, inftype) \
    CV_DEF_NORM_FUNC(L1, suffix, type, l1type) \
    CV_DEF_NORM_FUNC(L2, suffix, type, l2type)

CV_DEF_NORM_ALL(8u, uchar, số_nguyên, số_nguyên, số_nguyên)
CV_DEF_NORM_ALL(8s, schar, số_nguyên, số_nguyên, số_nguyên)
CV_DEF_NORM_ALL(16u, ushort, số_nguyên, số_nguyên, double)
CV_DEF_NORM_ALL(16s, short, số_nguyên, số_nguyên, double)
CV_DEF_NORM_ALL(32u, unsigned, unsigned, double, double)
CV_DEF_NORM_ALL(32s, số_nguyên, unsigned, double, double)
CV_DEF_NORM_ALL(32f, float, float, double, double)
CV_DEF_NORM_ALL(64f, double, double, double, double)
CV_DEF_NORM_ALL(64u, uint64, uint64, double, double)
CV_DEF_NORM_ALL(64s, int64, uint64, double, double)
CV_DEF_NORM_ALL(16f, hfloat, float, float, float)
CV_DEF_NORM_ALL(16bf, bfloat, float, float, float)

NormFunc getNormFunc(số_nguyên normType, số_nguyên depth)
{
    CV_INSTRUMENT_REGION();

    static NormFunc normTab[3][CV_DEPTH_MAX] =
    {
        {
            normWrap<uchar, số_nguyên, normInf_8u>,
            normWrap<schar, số_nguyên, normInf_8s>,
            normWrap<ushort, số_nguyên, normInf_16u>,
            normWrap<short, số_nguyên, normInf_16s>,
            normWrap<số_nguyên, unsigned, normInf_32s>,
            normWrap<float, float, normInf_32f>,
            normWrap<double, double, normInf_64f>,
            normWrap<hfloat, float, normInf_16f>,
            normWrap<bfloat, float, normInf_16bf>,
            normWrap<uchar, số_nguyên, normInf_Bool>,
            normWrap<uint64, uint64, normInf_64u>,
            normWrap<int64, uint64, normInf_64s>,
            normWrap<unsigned, unsigned, normInf_32u>,
            0
        },
        {
            normWrap<uchar, số_nguyên, normL1_8u>,
            normWrap<schar, số_nguyên, normL1_8s>,
            normWrap<ushort, số_nguyên, normL1_16u>,
            normWrap<short, số_nguyên, normL1_16s>,
            normWrap<số_nguyên, double, normL1_32s>,
            normWrap<float, double, normL1_32f>,
            normWrap<double, double, normL1_64f>,
            normWrap<hfloat, float, normL1_16f>,
            normWrap<bfloat, float, normL1_16bf>,
            normWrap<uchar, số_nguyên, normL1_Bool>,
            normWrap<uint64, double, normL1_64u>,
            normWrap<int64, double, normL1_64s>,
            normWrap<unsigned, double, normL1_32u>,
            0
        },
        {
            normWrap<uchar, số_nguyên, normL2_8u>,
            normWrap<schar, số_nguyên, normL2_8s>,
            normWrap<ushort, double, normL2_16u>,
            normWrap<short, double, normL2_16s>,
            normWrap<số_nguyên, double, normL2_32s>,
            normWrap<float, double, normL2_32f>,
            normWrap<double, double, normL2_64f>,
            normWrap<hfloat, float, normL2_16f>,
            normWrap<bfloat, float, normL2_16bf>,
            normWrap<uchar, số_nguyên, normL2_Bool>,
            normWrap<uint64, double, normL2_64u>,
            normWrap<int64, double, normL2_64s>,
            normWrap<unsigned, double, normL2_32u>,
            0
        }
    };

    nếu (normType >= 3 || normType < 0) trả_về nullptr;

    trả_về normTab[normType][depth];
}

NormDiffFunc getNormDiffFunc(số_nguyên normType, số_nguyên depth)
{
    static NormDiffFunc normDiffTab[3][CV_DEPTH_MAX] =
    {
        {
            normDiffWrap<uchar, số_nguyên, normDiffInf_8u>,
            normDiffWrap<schar, số_nguyên, normDiffInf_8s>,
            normDiffWrap<ushort, số_nguyên, normDiffInf_16u>,
            normDiffWrap<short, số_nguyên, normDiffInf_16s>,
            normDiffWrap<số_nguyên, unsigned, normDiffInf_32s>,
            normDiffWrap<float, float, normDiffInf_32f>,
            normDiffWrap<double, double, normDiffInf_64f>,
            normDiffWrap<hfloat, float, normDiffInf_16f>,
            normDiffWrap<bfloat, float, normDiffInf_16bf>,
            normDiffWrap<uchar, số_nguyên, normDiffInf_Bool>,
            normDiffWrap<uint64, uint64, normDiffInf_64u>,
            normDiffWrap<int64, uint64, normDiffInf_64s>,
            normDiffWrap<unsigned, unsigned, normDiffInf_32u>,
            0
        },
        {
            normDiffWrap<uchar, số_nguyên, normDiffL1_8u>,
            normDiffWrap<schar, số_nguyên, normDiffL1_8s>,
            normDiffWrap<ushort, số_nguyên, normDiffL1_16u>,
            normDiffWrap<short, số_nguyên, normDiffL1_16s>,
            normDiffWrap<số_nguyên, double, normDiffL1_32s>,
            normDiffWrap<float, double, normDiffL1_32f>,
            normDiffWrap<double, double, normDiffL1_64f>,
            normDiffWrap<hfloat, float, normDiffL1_16f>,
            normDiffWrap<bfloat, float, normDiffL1_16bf>,
            normDiffWrap<uchar, số_nguyên, normDiffL1_Bool>,
            normDiffWrap<uint64, double, normDiffL1_64u>,
            normDiffWrap<int64, double, normDiffL1_64s>,
            normDiffWrap<unsigned, double, normDiffL1_32u>,
            0
        },
        {
            normDiffWrap<uchar, số_nguyên, normDiffL2_8u>,
            normDiffWrap<schar, số_nguyên, normDiffL2_8s>,
            normDiffWrap<ushort, double, normDiffL2_16u>,
            normDiffWrap<short, double, normDiffL2_16s>,
            normDiffWrap<số_nguyên, double, normDiffL2_32s>,
            normDiffWrap<float, double, normDiffL2_32f>,
            normDiffWrap<double, double, normDiffL2_64f>,
            normDiffWrap<hfloat, float, normDiffL2_16f>,
            normDiffWrap<bfloat, float, normDiffL2_16bf>,
            normDiffWrap<uchar, số_nguyên, normDiffL2_Bool>,
            normDiffWrap<uint64, double, normDiffL2_64u>,
            normDiffWrap<int64, double, normDiffL2_64s>,
            normDiffWrap<unsigned, double, normDiffL2_32u>,
            0
        },
    };
    nếu (normType >= 3 || normType < 0) trả_về nullptr;

    trả_về normDiffTab[normType][depth];
}

#endif // CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

CV_CPU_OPTIMIZATION_NAMESPACE_END

} // cv::
