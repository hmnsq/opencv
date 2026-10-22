// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html
//
// Copyright (C) 2025, SpaceMIT Inc., all rights reserved.
// Copyright (C) 2026, Advanced Micro Devices, Inc., all rights reserved.
// Third party copyrights are property of their respective owners.

#અન્તર્ભાવ "precomp.hpp"

નામાકાશ cv {

using NormFunc = પૂર્ણાંક (*)(અચલ uchar*, અચલ uchar*, uchar*, પૂર્ણાંક, પૂર્ણાંક);
using NormDiffFunc = પૂર્ણાંક (*)(અચલ uchar*, અચલ uchar*, અચલ uchar*, uchar*, પૂર્ણાંક, પૂર્ણાંક);

CV_CPU_OPTIMIZATION_NAMESPACE_BEGIN

template<typename T, typename RT, પૂર્ણાંક (*fn)(અચલ T*, અચલ uchar*, RT*, પૂર્ણાંક, પૂર્ણાંક)>
સ્થિર પૂર્ણાંક normWrap(અચલ uchar* src1, અચલ uchar* src2, uchar* res, પૂર્ણાંક len, પૂર્ણાંક cn)
{
    પાછા fn((અચલ T*)src1, src2, (RT*)res, len, cn);
}

template<typename T, typename RT, પૂર્ણાંક (*fn)(અચલ T*, અચલ T*, અચલ uchar*, RT*, પૂર્ણાંક, પૂર્ણાંક)>
સ્થિર પૂર્ણાંક normDiffWrap(અચલ uchar* src1, અચલ uchar* src2, અચલ uchar* src3, uchar* res, પૂર્ણાંક len, પૂર્ણાંક cn)
{
    પાછા fn((અચલ T*)src1, (અચલ T*)src2, src3, (RT*)res, len, cn);
}

NormFunc getNormFunc(પૂર્ણાંક normType, પૂર્ણાંક depth);
NormDiffFunc getNormDiffFunc(પૂર્ણાંક normType, પૂર્ણાંક depth);

#ifndef CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

template <typename T, typename ST>
સંરચના NormInf_SIMD {
    અંતઃરેખ ST operator() (અચલ T* src, પૂર્ણાંક n) અચલ {
        ST s = 0;
        માટે (પૂર્ણાંક i = 0; i < n; i++) {
            s = std::max(s, (ST)cv_abs(src[i]));
        }
        પાછા s;
    }
};

template <typename T, typename ST>
સંરચના NormL1_SIMD {
    અંતઃરેખ ST operator() (અચલ T* src, પૂર્ણાંક n) અચલ {
        ST s = 0;
        માટે (પૂર્ણાંક i = 0; i < n; i++) {
            s += cv_abs(src[i]);
        }
        પાછા s;
    }
};

template <typename T, typename ST>
સંરચના NormL2_SIMD {
    અંતઃરેખ ST operator() (અચલ T* src, પૂર્ણાંક n) અચલ {
        ST s = 0;
        માટે (પૂર્ણાંક i = 0; i < n; i++) {
            ST v = (ST)src[i];
            s += v * v;
        }
        પાછા s;
    }
};

template <typename T, typename ST>
સંરચના NormDiffInf_SIMD {
    અંતઃરેખ ST operator() (અચલ T* src1, અચલ T* src2, પૂર્ણાંક n) અચલ {
        ST s = 0;
        માટે (પૂર્ણાંક i = 0; i < n; i++) {
            ST v = (ST)cv_absdiff(src1[i], src2[i]);
            s = std::max(s, v);
        }
        પાછા s;
    }
};

template <typename T, typename ST>
સંરચના NormDiffL1_SIMD {
    અંતઃરેખ ST operator() (અચલ T* src1, અચલ T* src2, પૂર્ણાંક n) અચલ {
        ST s = 0;
        માટે (પૂર્ણાંક i = 0; i < n; i++) {
            ST v = (ST)cv_absdiff(src1[i], src2[i]);
            s += v;
        }
        પાછા s;
    }
};

template <typename T, typename ST>
સંરચના NormDiffL2_SIMD {
    અંતઃરેખ ST operator() (અચલ T* src1, અચલ T* src2, પૂર્ણાંક n) અચલ {
        ST s = 0;
        માટે (પૂર્ણાંક i = 0; i < n; i++) {
            ST v = (ST)src1[i] - (ST)src2[i];
            s += v * v;
        }
        પાછા s;
    }
};

#જો (CV_SIMD || CV_SIMD_SCALABLE)

template<>
સંરચના NormInf_SIMD<uchar, પૂર્ણાંક> {
    પૂર્ણાંક operator() (અચલ uchar* src, પૂર્ણાંક n) અચલ {
        પૂર્ણાંક j = 0;
        પૂર્ણાંક s = 0;
        v_uint8 r0 = vx_setzero_u8(), r1 = vx_setzero_u8();
        v_uint8 r2 = vx_setzero_u8(), r3 = vx_setzero_u8();
        માટે (; j <= n - 4 * VTraits<v_uint8>::vlanes(); j += 4 * VTraits<v_uint8>::vlanes()) {
            r0 = v_max(r0, vx_load(src + j                                 ));
            r1 = v_max(r1, vx_load(src + j +     VTraits<v_uint8>::vlanes()));
            r2 = v_max(r2, vx_load(src + j + 2 * VTraits<v_uint8>::vlanes()));
            r3 = v_max(r3, vx_load(src + j + 3 * VTraits<v_uint8>::vlanes()));
        }
        r0 = v_max(r0, v_max(r1, v_max(r2, r3)));
        માટે (; j < n; j++) {
            s = std::max(s, (પૂર્ણાંક)src[j]);
        }
        પાછા std::max(s, (પૂર્ણાંક)v_reduce_max(r0));
    }
};

template<>
સંરચના NormInf_SIMD<schar, પૂર્ણાંક> {
    પૂર્ણાંક operator() (અચલ schar* src, પૂર્ણાંક n) અચલ {
        પૂર્ણાંક j = 0;
        પૂર્ણાંક s = 0;
        v_uint8 r0 = vx_setzero_u8(), r1 = vx_setzero_u8();
        v_uint8 r2 = vx_setzero_u8(), r3 = vx_setzero_u8();
        માટે (; j <= n - 4 * VTraits<v_int8>::vlanes(); j += 4 * VTraits<v_int8>::vlanes()) {
            r0 = v_max(r0, v_abs(vx_load(src + j                                )));
            r1 = v_max(r1, v_abs(vx_load(src + j +     VTraits<v_int8>::vlanes())));
            r2 = v_max(r2, v_abs(vx_load(src + j + 2 * VTraits<v_int8>::vlanes())));
            r3 = v_max(r3, v_abs(vx_load(src + j + 3 * VTraits<v_int8>::vlanes())));
        }
        r0 = v_max(r0, v_max(r1, v_max(r2, r3)));
        માટે (; j < n; j++) {
            s = std::max(s, cv_abs(src[j]));
        }
        પાછા std::max(s, saturate_cast<પૂર્ણાંક>(v_reduce_max(r0)));
    }
};

template<>
સંરચના NormInf_SIMD<ushort, પૂર્ણાંક> {
    પૂર્ણાંક operator() (અચલ ushort* src, પૂર્ણાંક n) અચલ {
        પૂર્ણાંક j = 0;
        પૂર્ણાંક s = 0;
        v_uint16 d0 = vx_setzero_u16(), d1 = vx_setzero_u16();
        v_uint16 d2 = vx_setzero_u16(), d3 = vx_setzero_u16();
        માટે (; j <= n - 4 * VTraits<v_uint16>::vlanes(); j += 4 * VTraits<v_uint16>::vlanes()) {
            d0 = v_max(d0, vx_load(src + j                                  ));
            d1 = v_max(d1, vx_load(src + j +     VTraits<v_uint16>::vlanes()));
            d2 = v_max(d2, vx_load(src + j + 2 * VTraits<v_uint16>::vlanes()));
            d3 = v_max(d3, vx_load(src + j + 3 * VTraits<v_uint16>::vlanes()));
        }
        d0 = v_max(d0, v_max(d1, v_max(d2, d3)));
        માટે (; j < n; j++) {
            s = std::max(s, (પૂર્ણાંક)src[j]);
        }
        પાછા std::max(s, (પૂર્ણાંક)v_reduce_max(d0));
    }
};

template<>
સંરચના NormInf_SIMD<short, પૂર્ણાંક> {
    પૂર્ણાંક operator() (અચલ short* src, પૂર્ણાંક n) અચલ {
        પૂર્ણાંક j = 0;
        પૂર્ણાંક s = 0;
        v_uint16 d0 = vx_setzero_u16(), d1 = vx_setzero_u16();
        v_uint16 d2 = vx_setzero_u16(), d3 = vx_setzero_u16();
        માટે (; j <= n - 4 * VTraits<v_int16>::vlanes(); j += 4 * VTraits<v_int16>::vlanes()) {
            d0 = v_max(d0, v_abs(vx_load(src + j                                  )));
            d1 = v_max(d1, v_abs(vx_load(src + j +     VTraits<v_int16>::vlanes())));
            d2 = v_max(d2, v_abs(vx_load(src + j + 2 * VTraits<v_int16>::vlanes())));
            d3 = v_max(d3, v_abs(vx_load(src + j + 3 * VTraits<v_int16>::vlanes())));
        }
        d0 = v_max(d0, v_max(d1, v_max(d2, d3)));
        માટે (; j < n; j++) {
            s = std::max(s, saturate_cast<પૂર્ણાંક>(cv_abs(src[j])));
        }
        પાછા std::max(s, saturate_cast<પૂર્ણાંક>(v_reduce_max(d0)));
    }
};

template<>
સંરચના NormInf_SIMD<પૂર્ણાંક, પૂર્ણાંક> {
    પૂર્ણાંક operator() (અચલ પૂર્ણાંક* src, પૂર્ણાંક n) અચલ {
        પૂર્ણાંક j = 0;
        પૂર્ણાંક s = 0;
        v_uint32 r0 = vx_setzero_u32(), r1 = vx_setzero_u32();
        v_uint32 r2 = vx_setzero_u32(), r3 = vx_setzero_u32();
        માટે (; j <= n - 4 * VTraits<v_int32>::vlanes(); j += 4 * VTraits<v_int32>::vlanes()) {
            r0 = v_max(r0, v_abs(vx_load(src + j                                 )));
            r1 = v_max(r1, v_abs(vx_load(src + j +     VTraits<v_int32>::vlanes())));
            r2 = v_max(r2, v_abs(vx_load(src + j + 2 * VTraits<v_int32>::vlanes())));
            r3 = v_max(r3, v_abs(vx_load(src + j + 3 * VTraits<v_int32>::vlanes())));
        }
        r0 = v_max(r0, v_max(r1, v_max(r2, r3)));
        s = std::max(s, saturate_cast<પૂર્ણાંક>(v_reduce_max(r0)));
        માટે (; j < n; j++) {
            s = std::max(s, std::abs(src[j]));
        }
        પાછા s;
    }
};

template<>
સંરચના NormInf_SIMD<દશાંશ, દશાંશ> {
    દશાંશ operator() (અચલ દશાંશ* src, પૂર્ણાંક n) અચલ {
        પૂર્ણાંક j = 0;
        દશાંશ s = 0.f;
        v_float32 r0 = vx_setzero_f32(), r1 = vx_setzero_f32();
        v_float32 r2 = vx_setzero_f32(), r3 = vx_setzero_f32();
        માટે (; j <= n - 4 * VTraits<v_float32>::vlanes(); j += 4 * VTraits<v_float32>::vlanes()) {
            r0 = v_max(r0, v_abs(vx_load(src + j                                   )));
            r1 = v_max(r1, v_abs(vx_load(src + j +     VTraits<v_float32>::vlanes())));
            r2 = v_max(r2, v_abs(vx_load(src + j + 2 * VTraits<v_float32>::vlanes())));
            r3 = v_max(r3, v_abs(vx_load(src + j + 3 * VTraits<v_float32>::vlanes())));
        }
        r0 = v_max(r0, v_max(r1, v_max(r2, r3)));
        માટે (; j < n; j++) {
            s = std::max(s, cv_abs(src[j]));
        }
        પાછા std::max(s, v_reduce_max(r0));
    }
};

template<>
સંરચના NormL1_SIMD<uchar, પૂર્ણાંક> {
    પૂર્ણાંક operator() (અચલ uchar* src, પૂર્ણાંક n) અચલ {
        પૂર્ણાંક j = 0;
        પૂર્ણાંક s = 0;
        v_uint32 r0 = vx_setzero_u32(), r1 = vx_setzero_u32();
        v_uint8  one = vx_setall_u8(1);
        માટે (; j<= n - 2 * VTraits<v_uint8>::vlanes(); j += 2 * VTraits<v_uint8>::vlanes()) {
            v_uint8 v0 = vx_load(src + j);
            r0 = v_dotprod_expand_fast(v0, one, r0);

            v_uint8 v1 = vx_load(src + j + VTraits<v_uint8>::vlanes());
            r1 = v_dotprod_expand_fast(v1, one, r1);
        }
        s += v_reduce_sum(v_add(r0, r1));
        માટે (; j < n; j++) {
            s += src[j];
        }
        પાછા s;
    }
};

template<>
સંરચના NormL1_SIMD<schar, પૂર્ણાંક> {
    પૂર્ણાંક operator() (અચલ schar* src, પૂર્ણાંક n) અચલ {
        પૂર્ણાંક j = 0;
        પૂર્ણાંક s = 0;
        v_uint32 r0 = vx_setzero_u32(), r1 = vx_setzero_u32();
        v_uint8  one = vx_setall_u8(1);
        માટે (; j<= n - 2 * VTraits<v_int8>::vlanes(); j += 2 * VTraits<v_int8>::vlanes()) {
            v_uint8 v0 = v_abs(vx_load(src + j));
            r0 = v_dotprod_expand_fast(v0, one, r0);

            v_uint8 v1 = v_abs(vx_load(src + j + VTraits<v_int8>::vlanes()));
            r1 = v_dotprod_expand_fast(v1, one, r1);
        }
        s += v_reduce_sum(v_add(r0, r1));
        માટે (; j < n; j++) {
            s += saturate_cast<પૂર્ણાંક>(cv_abs(src[j]));
        }
        પાછા s;
    }
};

template<>
સંરચના NormL1_SIMD<ushort, પૂર્ણાંક> {
    પૂર્ણાંક operator() (અચલ ushort* src, પૂર્ણાંક n) અચલ {
        પૂર્ણાંક j = 0;
        પૂર્ણાંક s = 0;
        v_uint32 r00 = vx_setzero_u32(), r01 = vx_setzero_u32();
        v_uint32 r10 = vx_setzero_u32(), r11 = vx_setzero_u32();
        માટે (; j<= n - 2 * VTraits<v_uint16>::vlanes(); j += 2 * VTraits<v_uint16>::vlanes()) {
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
        s += (પૂર્ણાંક)v_reduce_sum(v_add(v_add(v_add(r00, r01), r10), r11));
        માટે (; j < n; j++) {
            s += src[j];
        }
        પાછા s;
    }
};

template<>
સંરચના NormL1_SIMD<short, પૂર્ણાંક> {
    પૂર્ણાંક operator() (અચલ short* src, પૂર્ણાંક n) અચલ {
        પૂર્ણાંક j = 0;
        પૂર્ણાંક s = 0;
        v_uint32 r00 = vx_setzero_u32(), r01 = vx_setzero_u32();
        v_uint32 r10 = vx_setzero_u32(), r11 = vx_setzero_u32();
        માટે (; j<= n - 2 * VTraits<v_int16>::vlanes(); j += 2 * VTraits<v_int16>::vlanes()) {
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
        s += (પૂર્ણાંક)v_reduce_sum(v_add(v_add(v_add(r00, r01), r10), r11));
        માટે (; j < n; j++) {
            s += saturate_cast<પૂર્ણાંક>(cv_abs(src[j]));
        }
        પાછા s;
    }
};

template<>
સંરચના NormL2_SIMD<uchar, પૂર્ણાંક> {
    પૂર્ણાંક operator() (અચલ uchar* src, પૂર્ણાંક n) અચલ {
        પૂર્ણાંક j = 0;
        પૂર્ણાંક s = 0;
        v_uint32 r0 = vx_setzero_u32(), r1 = vx_setzero_u32();
        માટે (; j <= n - 2 * VTraits<v_uint8>::vlanes(); j += 2 * VTraits<v_uint8>::vlanes()) {
            v_uint8 v0 = vx_load(src + j);
            r0 = v_dotprod_expand_fast(v0, v0, r0);

            v_uint8 v1 = vx_load(src + j + VTraits<v_uint8>::vlanes());
            r1 = v_dotprod_expand_fast(v1, v1, r1);
        }
        s += v_reduce_sum(v_add(r0, r1));
        માટે (; j < n; j++) {
            પૂર્ણાંક v = saturate_cast<પૂર્ણાંક>(src[j]);
            s += v * v;
        }
        પાછા s;
    }
};

template<>
સંરચના NormL2_SIMD<schar, પૂર્ણાંક> {
    પૂર્ણાંક operator() (અચલ schar* src, પૂર્ણાંક n) અચલ {
        પૂર્ણાંક j = 0;
        પૂર્ણાંક s = 0;
        v_int32 r0 = vx_setzero_s32(), r1 = vx_setzero_s32();
        માટે (; j <= n - 2 * VTraits<v_int8>::vlanes(); j += 2 * VTraits<v_int8>::vlanes()) {
            v_int8 v0 = vx_load(src + j);
            r0 = v_dotprod_expand_fast(v0, v0, r0);
            v_int8 v1 = vx_load(src + j + VTraits<v_int8>::vlanes());
            r1 = v_dotprod_expand_fast(v1, v1, r1);
        }
        s += v_reduce_sum(v_add(r0, r1));
        માટે (; j < n; j++) {
            પૂર્ણાંક v = saturate_cast<પૂર્ણાંક>(src[j]);
            s += v * v;
        }
        પાછા s;
    }
};

template<>
સંરચના NormDiffInf_SIMD<uchar, પૂર્ણાંક> {
    પૂર્ણાંક operator() (અચલ uchar* src1, અચલ uchar* src2, પૂર્ણાંક n) અચલ {
        પૂર્ણાંક j = 0;
        પૂર્ણાંક s = 0;
        v_uint8 r0 = vx_setzero_u8(), r1 = vx_setzero_u8();
        v_uint8 r2 = vx_setzero_u8(), r3 = vx_setzero_u8();
        માટે (; j <= n - 4 * VTraits<v_uint8>::vlanes(); j += 4 * VTraits<v_uint8>::vlanes()) {
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
        s = (પૂર્ણાંક)v_reduce_max(v_max(v_max(v_max(r0, r1), r2), r3));
        માટે (; j < n; j++) {
            પૂર્ણાંક v = (પૂર્ણાંક)cv_absdiff(src1[j], src2[j]);
            s = std::max(s, v);
        }
        પાછા s;
    }
};

template<>
સંરચના NormDiffInf_SIMD<schar, પૂર્ણાંક> {
    પૂર્ણાંક operator() (અચલ schar* src1, અચલ schar* src2, પૂર્ણાંક n) અચલ {
        પૂર્ણાંક j = 0;
        પૂર્ણાંક s = 0;
        v_uint8 r0 = vx_setzero_u8(), r1 = vx_setzero_u8();
        v_uint8 r2 = vx_setzero_u8(), r3 = vx_setzero_u8();
        માટે (; j <= n - 4 * VTraits<v_int8>::vlanes(); j += 4 * VTraits<v_int8>::vlanes()) {
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
        s = (પૂર્ણાંક)v_reduce_max(v_max(v_max(v_max(r0, r1), r2), r3));
        માટે (; j < n; j++) {
            પૂર્ણાંક v = (પૂર્ણાંક)cv_absdiff(src1[j], src2[j]);
            s = std::max(s, v);
        }
        પાછા s;
    }
};

template<>
સંરચના NormDiffInf_SIMD<ushort, પૂર્ણાંક> {
    પૂર્ણાંક operator() (અચલ ushort* src1, અચલ ushort* src2, પૂર્ણાંક n) અચલ {
        પૂર્ણાંક j = 0;
        પૂર્ણાંક s = 0;
        v_uint16 r0 = vx_setzero_u16(), r1 = vx_setzero_u16();
        v_uint16 r2 = vx_setzero_u16(), r3 = vx_setzero_u16();
        માટે (; j <= n - 4 * VTraits<v_uint16>::vlanes(); j += 4 * VTraits<v_uint16>::vlanes()) {
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
        s = (પૂર્ણાંક)v_reduce_max(v_max(v_max(v_max(r0, r1), r2), r3));
        માટે (; j < n; j++) {
            પૂર્ણાંક v = (પૂર્ણાંક)cv_absdiff(src1[j], src2[j]);
            s = std::max(s, v);
        }
        પાછા s;
    }
};

template<>
સંરચના NormDiffInf_SIMD<short, પૂર્ણાંક> {
    પૂર્ણાંક operator() (અચલ short* src1, અચલ short* src2, પૂર્ણાંક n) અચલ {
        પૂર્ણાંક j = 0;
        પૂર્ણાંક s = 0;
        v_uint16 r0 = vx_setzero_u16(), r1 = vx_setzero_u16();
        v_uint16 r2 = vx_setzero_u16(), r3 = vx_setzero_u16();
        માટે (; j <= n - 4 * VTraits<v_int16>::vlanes(); j += 4 * VTraits<v_int16>::vlanes()) {
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
        s = (પૂર્ણાંક)v_reduce_max(v_max(v_max(v_max(r0, r1), r2), r3));
        માટે (; j < n; j++) {
            પૂર્ણાંક v = (પૂર્ણાંક)cv_absdiff(src1[j], src2[j]);
            s = std::max(s, v);
        }
        પાછા s;
    }
};

template<>
સંરચના NormDiffInf_SIMD<પૂર્ણાંક, unsigned> {
    unsigned operator() (અચલ પૂર્ણાંક* src1, અચલ પૂર્ણાંક* src2, પૂર્ણાંક n) અચલ {
        પૂર્ણાંક j = 0;
        unsigned s = 0;
        v_uint32 r0 = vx_setzero_u32(), r1 = vx_setzero_u32();
        v_uint32 r2 = vx_setzero_u32(), r3 = vx_setzero_u32();
        માટે (; j <= n - 4 * VTraits<v_int32>::vlanes(); j += 4 * VTraits<v_int32>::vlanes()) {
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
        s = (પૂર્ણાંક)v_reduce_max(v_max(v_max(v_max(r0, r1), r2), r3));
        માટે (; j < n; j++) {
            unsigned v = (unsigned)cv_absdiff(src1[j], src2[j]);
            s = std::max(s, v);
        }
        પાછા s;
    }
};

template<>
સંરચના NormDiffInf_SIMD<દશાંશ, દશાંશ> {
    દશાંશ operator() (અચલ દશાંશ* src1, અચલ દશાંશ* src2, પૂર્ણાંક n) અચલ {
        પૂર્ણાંક j = 0;
        દશાંશ s = 0;
        v_float32 r0 = vx_setzero_f32(), r1 = vx_setzero_f32();
        માટે (; j <= n - 2 * VTraits<v_float32>::vlanes(); j += 2 * VTraits<v_float32>::vlanes()) {
            v_float32 v01 = vx_load(src1 + j), v02 = vx_load(src2 + j);
            r0 = v_max(r0, v_absdiff(v01, v02));

            v_float32 v11 = vx_load(src1 + j + VTraits<v_float32>::vlanes()),
                      v12 = vx_load(src2 + j + VTraits<v_float32>::vlanes());
            r1 = v_max(r1, v_absdiff(v11, v12));
        }
        s = v_reduce_max(v_max(r0, r1));
        માટે (; j < n; j++) {
            દશાંશ v = (દશાંશ)cv_absdiff(src1[j], src2[j]);
            s = std::max(s, v);
        }
        પાછા s;
    }
};

template<>
સંરચના NormDiffL1_SIMD<uchar, પૂર્ણાંક> {
    પૂર્ણાંક operator() (અચલ uchar* src1, અચલ uchar* src2, પૂર્ણાંક n) અચલ {
        પૂર્ણાંક j = 0;
        પૂર્ણાંક s = 0;
        v_uint32 r0 = vx_setzero_u32(), r1 = vx_setzero_u32();
        v_uint8  one = vx_setall_u8(1);
        માટે (; j<= n - 2 * VTraits<v_uint8>::vlanes(); j += 2 * VTraits<v_uint8>::vlanes()) {
            v_uint8 v01 = vx_load(src1 + j), v02 = vx_load(src2 + j);
            r0 = v_dotprod_expand_fast(v_absdiff(v01, v02), one, r0);

            v_uint8 v11 = vx_load(src1 + j + VTraits<v_uint8>::vlanes()),
                    v12 = vx_load(src2 + j + VTraits<v_uint8>::vlanes());
            r1 = v_dotprod_expand_fast(v_absdiff(v11, v12), one, r1);
        }
        s += v_reduce_sum(v_add(r0, r1));
        માટે (; j < n; j++) {
            પૂર્ણાંક v = (પૂર્ણાંક)cv_absdiff(src1[j], src2[j]);
            s += v;
        }
        પાછા s;
    }
};

template<>
સંરચના NormDiffL1_SIMD<schar, પૂર્ણાંક> {
    પૂર્ણાંક operator() (અચલ schar* src1, અચલ schar* src2, પૂર્ણાંક n) અચલ {
        પૂર્ણાંક j = 0;
        પૂર્ણાંક s = 0;
        v_uint32 r0 = vx_setzero_u32(), r1 = vx_setzero_u32();
        v_uint8  one = vx_setall_u8(1);
        માટે (; j<= n - 2 * VTraits<v_int8>::vlanes(); j += 2 * VTraits<v_int8>::vlanes()) {
            v_int8 v01 = vx_load(src1 + j), v02 = vx_load(src2 + j);
            r0 = v_dotprod_expand_fast(v_absdiff(v01, v02), one, r0);

            v_int8 v11 = vx_load(src1 + j + VTraits<v_int8>::vlanes()),
                   v12 = vx_load(src2 + j + VTraits<v_int8>::vlanes());
            r1 = v_dotprod_expand_fast(v_absdiff(v11, v12), one, r1);
        }
        s += v_reduce_sum(v_add(r0, r1));
        માટે (; j < n; j++) {
            પૂર્ણાંક v = (પૂર્ણાંક)cv_absdiff(src1[j], src2[j]);
            s += v;
        }
        પાછા s;
    }
};

template<>
સંરચના NormDiffL1_SIMD<ushort, પૂર્ણાંક> {
    પૂર્ણાંક operator() (અચલ ushort* src1, અચલ ushort* src2, પૂર્ણાંક n) અચલ {
        પૂર્ણાંક j = 0;
        પૂર્ણાંક s = 0;
        v_uint32 r0 = vx_setzero_u32(), r1 = vx_setzero_u32();
        v_uint32 r2 = vx_setzero_u32(), r3 = vx_setzero_u32();
        માટે (; j<= n - 4 * VTraits<v_uint16>::vlanes(); j += 4 * VTraits<v_uint16>::vlanes()) {
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
        s += (પૂર્ણાંક)v_reduce_sum(v_add(v_add(v_add(r0, r1), r2), r3));
        માટે (; j < n; j++) {
            પૂર્ણાંક v = (પૂર્ણાંક)cv_absdiff(src1[j], src2[j]);
            s += v;
        }
        પાછા s;
    }
};

template<>
સંરચના NormDiffL1_SIMD<short, પૂર્ણાંક> {
    પૂર્ણાંક operator() (અચલ short* src1, અચલ short* src2, પૂર્ણાંક n) અચલ {
        પૂર્ણાંક j = 0;
        પૂર્ણાંક s = 0;
        v_uint32 r0 = vx_setzero_u32(), r1 = vx_setzero_u32();
        v_uint32 r2 = vx_setzero_u32(), r3 = vx_setzero_u32();
        માટે (; j<= n - 4 * VTraits<v_int16>::vlanes(); j += 4 * VTraits<v_int16>::vlanes()) {
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
        s += (પૂર્ણાંક)v_reduce_sum(v_add(v_add(v_add(r0, r1), r2), r3));
        માટે (; j < n; j++) {
            પૂર્ણાંક v = (પૂર્ણાંક)cv_absdiff(src1[j], src2[j]);
            s += v;
        }
        પાછા s;
    }
};

template<>
સંરચના NormDiffL2_SIMD<uchar, પૂર્ણાંક> {
    પૂર્ણાંક operator() (અચલ uchar* src1, અચલ uchar* src2, પૂર્ણાંક n) અચલ {
        પૂર્ણાંક j = 0;
        પૂર્ણાંક s = 0;
        v_uint32 r0 = vx_setzero_u32(), r1 = vx_setzero_u32();
        માટે (; j <= n - 2 * VTraits<v_uint8>::vlanes(); j += 2 * VTraits<v_uint8>::vlanes()) {
            v_uint8 v01 = vx_load(src1 + j), v02 = vx_load(src2 + j);
            v_uint8 v0 = v_absdiff(v01, v02);
            r0 = v_dotprod_expand_fast(v0, v0, r0);

            v_uint8 v11 = vx_load(src1 + j + VTraits<v_uint8>::vlanes()),
                    v12 = vx_load(src2 + j + VTraits<v_uint8>::vlanes());
            v_uint8 v1 = v_absdiff(v11, v12);
            r1 = v_dotprod_expand_fast(v1, v1, r1);
        }
        s += v_reduce_sum(v_add(r0, r1));
        માટે (; j < n; j++) {
            પૂર્ણાંક v = (પૂર્ણાંક)src1[j] - (પૂર્ણાંક)src2[j];
            s += v * v;
        }
        પાછા s;
    }
};

template<>
સંરચના NormDiffL2_SIMD<schar, પૂર્ણાંક> {
    પૂર્ણાંક operator() (અચલ schar* src1, અચલ schar* src2, પૂર્ણાંક n) અચલ {
        પૂર્ણાંક j = 0;
        પૂર્ણાંક s = 0;
        v_uint32 r0 = vx_setzero_u32(), r1 = vx_setzero_u32();
        માટે (; j <= n - 2 * VTraits<v_int8>::vlanes(); j += 2 * VTraits<v_int8>::vlanes()) {
            v_int8 v01 = vx_load(src1 + j), v02 = vx_load(src2 + j);
            v_uint8 v0 = v_absdiff(v01, v02);
            r0 = v_dotprod_expand_fast(v0, v0, r0);

            v_int8 v11 = vx_load(src1 + j + VTraits<v_int8>::vlanes()),
                   v12 = vx_load(src2 + j + VTraits<v_int8>::vlanes());
            v_uint8 v1 = v_absdiff(v11, v12);
            r1 = v_dotprod_expand_fast(v1, v1, r1);
        }
        s += v_reduce_sum(v_add(r0, r1));
        માટે (; j < n; j++) {
            પૂર્ણાંક v = (પૂર્ણાંક)src1[j] - (પૂર્ણાંક)src2[j];
            s += v * v;
        }
        પાછા s;
    }
};

#endif

#જો (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)

template<>
સંરચના NormInf_SIMD<દ્વિગુણ, દ્વિગુણ> {
    દ્વિગુણ operator() (અચલ દ્વિગુણ* src, પૂર્ણાંક n) અચલ {
        પૂર્ણાંક j = 0;
        દ્વિગુણ s = 0.f;
        v_float64 r0 = vx_setzero_f64(), r1 = vx_setzero_f64();
        v_float64 r2 = vx_setzero_f64(), r3 = vx_setzero_f64();
        માટે (; j <= n - 4 * VTraits<v_float64>::vlanes(); j += 4 * VTraits<v_float64>::vlanes()) {
            r0 = v_max(r0, v_abs(vx_load(src + j                                   )));
            r1 = v_max(r1, v_abs(vx_load(src + j +     VTraits<v_float64>::vlanes())));
            r2 = v_max(r2, v_abs(vx_load(src + j + 2 * VTraits<v_float64>::vlanes())));
            r3 = v_max(r3, v_abs(vx_load(src + j + 3 * VTraits<v_float64>::vlanes())));
        }
        r0 = v_max(r0, v_max(r1, v_max(r2, r3)));
        માટે (; j < n; j++) {
            s = std::max(s, cv_abs(src[j]));
        }
        // [TODO]: use v_reduce_max when it supports float64
        દ્વિગુણ t[VTraits<v_float64>::max_nlanes];
        vx_store(t, r0);
        માટે (પૂર્ણાંક i = 0; i < VTraits<v_float64>::vlanes(); i++) {
            s = std::max(s, cv_abs(t[i]));
        }
        પાછા s;
    }
};

template<>
સંરચના NormL1_SIMD<પૂર્ણાંક, દ્વિગુણ> {
    દ્વિગુણ operator() (અચલ પૂર્ણાંક* src, પૂર્ણાંક n) અચલ {
        પૂર્ણાંક j = 0;
        દ્વિગુણ s = 0.f;
        v_float64 r00 = vx_setzero_f64(), r01 = vx_setzero_f64();
        v_float64 r10 = vx_setzero_f64(), r11 = vx_setzero_f64();
        માટે (; j <= n - 2 * VTraits<v_int32>::vlanes(); j += 2 * VTraits<v_int32>::vlanes()) {
            // Widen to double before taking the magnitude: exact over the whole int32 range,
            // INT_MIN included, which a float intermediate would not be above 2^24.
            v_int32 i0 = vx_load(src + j), i1 = vx_load(src + j + VTraits<v_int32>::vlanes());
            r00 = v_add(r00, v_abs(v_cvt_f64(i0))); r01 = v_add(r01, v_abs(v_cvt_f64_ઉપલું(i0)));
            r10 = v_add(r10, v_abs(v_cvt_f64(i1))); r11 = v_add(r11, v_abs(v_cvt_f64_ઉપલું(i1)));
        }
        s += v_reduce_sum(v_add(v_add(v_add(r00, r01), r10), r11));
        માટે (; j < n; j++) {
            s += cv_abs(src[j]);
        }
        પાછા s;
    }
};

template<>
સંરચના NormL1_SIMD<દશાંશ, દ્વિગુણ> {
    દ્વિગુણ operator() (અચલ દશાંશ* src, પૂર્ણાંક n) અચલ {
        પૂર્ણાંક j = 0;
        દ્વિગુણ s = 0.f;
        v_float64 r00 = vx_setzero_f64(), r01 = vx_setzero_f64();
        v_float64 r10 = vx_setzero_f64(), r11 = vx_setzero_f64();
        v_float64 r20 = vx_setzero_f64(), r21 = vx_setzero_f64();
        v_float64 r30 = vx_setzero_f64(), r31 = vx_setzero_f64();
        માટે (; j <= n - 4 * VTraits<v_float32>::vlanes(); j += 4 * VTraits<v_float32>::vlanes()) {
            v_float32 v0 = v_abs(vx_load(src + j)), v1 = v_abs(vx_load(src + j + VTraits<v_float32>::vlanes()));
            r00 = v_add(r00, v_cvt_f64(v0)); r01 = v_add(r01, v_cvt_f64_ઉપલું(v0));
            r10 = v_add(r10, v_cvt_f64(v1)); r11 = v_add(r11, v_cvt_f64_ઉપલું(v1));

            v_float32 v2 = v_abs(vx_load(src + j + 2 * VTraits<v_float32>::vlanes())), v3 = v_abs(vx_load(src + j + 3 * VTraits<v_float32>::vlanes()));
            r20 = v_add(r20, v_cvt_f64(v2)); r21 = v_add(r21, v_cvt_f64_ઉપલું(v2));
            r30 = v_add(r30, v_cvt_f64(v3)); r31 = v_add(r31, v_cvt_f64_ઉપલું(v3));
        }
        s += v_reduce_sum(v_add(v_add(v_add(r00, r01), r10), r11));
        s += v_reduce_sum(v_add(v_add(v_add(r20, r21), r30), r31));
        માટે (; j < n; j++) {
            s += cv_abs(src[j]);
        }
        પાછા s;
    }
};

template<>
સંરચના NormL2_SIMD<ushort, દ્વિગુણ> {
    દ્વિગુણ operator() (અચલ ushort* src, પૂર્ણાંક n) અચલ {
        પૂર્ણાંક j = 0;
        દ્વિગુણ s = 0.f;
        v_float64 r0 = vx_setzero_f64(), r1 = vx_setzero_f64();
        માટે (; j <= n - 2 * VTraits<v_uint16>::vlanes(); j += 2 * VTraits<v_uint16>::vlanes()) {
            v_uint16 v0 = vx_load(src + j);
            v_uint64 u0 = v_dotprod_expand_fast(v0, v0);
            r0 = v_add(r0, v_cvt_f64(v_reinterpret_as_s64(u0)));

            v_uint16 v1 = vx_load(src + j + VTraits<v_uint16>::vlanes());
            v_uint64 u1 = v_dotprod_expand_fast(v1, v1);
            r1 = v_add(r1, v_cvt_f64(v_reinterpret_as_s64(u1)));
        }
        s += v_reduce_sum(v_add(r0, r1));
        માટે (; j < n; j++) {
            દ્વિગુણ v = saturate_cast<દ્વિગુણ>(src[j]);
            s += v * v;
        }
        પાછા s;
    }
};

template<>
સંરચના NormL2_SIMD<short, દ્વિગુણ> {
    દ્વિગુણ operator() (અચલ short* src, પૂર્ણાંક n) અચલ {
        પૂર્ણાંક j = 0;
        દ્વિગુણ s = 0.f;
        v_float64 r0 = vx_setzero_f64(), r1 = vx_setzero_f64();
        માટે (; j <= n - 2 * VTraits<v_int16>::vlanes(); j += 2 * VTraits<v_int16>::vlanes()) {
            v_int16 v0 = vx_load(src + j);
            r0 = v_add(r0, v_cvt_f64(v_dotprod_expand_fast(v0, v0)));

            v_int16 v1 = vx_load(src + j + VTraits<v_int16>::vlanes());
            r1 = v_add(r1, v_cvt_f64(v_dotprod_expand_fast(v1, v1)));
        }
        s += v_reduce_sum(v_add(r0, r1));
        માટે (; j < n; j++) {
            દ્વિગુણ v = saturate_cast<દ્વિગુણ>(src[j]);
            s += v * v;
        }
        પાછા s;
    }
};

template<>
સંરચના NormL2_SIMD<પૂર્ણાંક, દ્વિગુણ> {
    દ્વિગુણ operator() (અચલ પૂર્ણાંક* src, પૂર્ણાંક n) અચલ {
        પૂર્ણાંક j = 0;
        દ્વિગુણ s = 0.f;
        v_float64 r0 = vx_setzero_f64(), r1 = vx_setzero_f64();
        માટે (; j <= n - 2 * VTraits<v_int32>::vlanes(); j += 2 * VTraits<v_int32>::vlanes()) {
            v_int32 v0 = vx_load(src + j);
            r0 = v_dotprod_expand_fast(v0, v0, r0);

            v_int32 v1 = vx_load(src + j + VTraits<v_int32>::vlanes());
            r1 = v_dotprod_expand_fast(v1, v1, r1);
        }
        s += v_reduce_sum(v_add(r0, r1));
        માટે (; j < n; j++) {
            દ્વિગુણ v = src[j];
            s += v * v;
        }
        પાછા s;
    }
};

template<>
સંરચના NormL2_SIMD<દશાંશ, દ્વિગુણ> {
    દ્વિગુણ operator() (અચલ દશાંશ* src, પૂર્ણાંક n) અચલ {
        પૂર્ણાંક j = 0;
        દ્વિગુણ s = 0.f;
        v_float64 r00 = vx_setzero_f64(), r01 = vx_setzero_f64();
        v_float64 r10 = vx_setzero_f64(), r11 = vx_setzero_f64();
        માટે (; j <= n - 2 * VTraits<v_float32>::vlanes(); j += 2 * VTraits<v_float32>::vlanes()) {
            v_float32 v0 = vx_load(src + j), v1 = vx_load(src + j + VTraits<v_float32>::vlanes());
            v_float64 v00 = v_cvt_f64(v0), v01 = v_cvt_f64_ઉપલું(v0);
            v_float64 v10 = v_cvt_f64(v1), v11 = v_cvt_f64_ઉપલું(v1);
            r00 = v_fma(v00, v00, r00); r01 = v_fma(v01, v01, r01);
            r10 = v_fma(v10, v10, r10); r11 = v_fma(v11, v11, r11);
        }
        s += v_reduce_sum(v_add(v_add(v_add(r00, r01), r10), r11));
        માટે (; j < n; j++) {
            દ્વિગુણ v = src[j];
            s += v * v;
        }
        પાછા s;
    }
};

#endif

#જો CV_SIMD_64F // CV_SIMD_SCALABLE_64F has accuracy problem with the following kernels on ci

template<>
સંરચના NormL1_SIMD<દ્વિગુણ, દ્વિગુણ> {
    દ્વિગુણ operator() (અચલ દ્વિગુણ* src, પૂર્ણાંક n) અચલ {
        પૂર્ણાંક j = 0;
        દ્વિગુણ s = 0.f;
        v_float64 r00 = vx_setzero_f64(), r01 = vx_setzero_f64();
        v_float64 r10 = vx_setzero_f64(), r11 = vx_setzero_f64();
        માટે (; j <= n - 4 * VTraits<v_float64>::vlanes(); j += 4 * VTraits<v_float64>::vlanes()) {
            r00 = v_add(r00, v_abs(vx_load(src + j                                   )));
            r01 = v_add(r01, v_abs(vx_load(src + j +     VTraits<v_float64>::vlanes())));
            r10 = v_add(r10, v_abs(vx_load(src + j + 2 * VTraits<v_float64>::vlanes())));
            r11 = v_add(r11, v_abs(vx_load(src + j + 3 * VTraits<v_float64>::vlanes())));
        }
        s += v_reduce_sum(v_add(v_add(v_add(r00, r01), r10), r11));
        માટે (; j < n; j++) {
            s += cv_abs(src[j]);
        }
        પાછા s;
    }
};

template<>
સંરચના NormL2_SIMD<દ્વિગુણ, દ્વિગુણ> {
    દ્વિગુણ operator() (અચલ દ્વિગુણ* src, પૂર્ણાંક n) અચલ {
        પૂર્ણાંક j = 0;
        દ્વિગુણ s = 0.f;
        v_float64 r00 = vx_setzero_f64(), r01 = vx_setzero_f64();
        v_float64 r10 = vx_setzero_f64(), r11 = vx_setzero_f64();
        માટે (; j <= n - 4 * VTraits<v_float64>::vlanes(); j += 4 * VTraits<v_float64>::vlanes()) {
            v_float64 v00 = vx_load(src + j                                   );
            v_float64 v01 = vx_load(src + j +     VTraits<v_float64>::vlanes());
            v_float64 v10 = vx_load(src + j + 2 * VTraits<v_float64>::vlanes());
            v_float64 v11 = vx_load(src + j + 3 * VTraits<v_float64>::vlanes());
            r00 = v_fma(v00, v00, r00); r01 = v_fma(v01, v01, r01);
            r10 = v_fma(v10, v10, r10); r11 = v_fma(v11, v11, r11);
        }
        s += v_reduce_sum(v_add(v_add(v_add(r00, r01), r10), r11));
        માટે (; j < n; j++) {
            દ્વિગુણ v = src[j];
            s += v * v;
        }
        પાછા s;
    }
};

template<>
સંરચના NormDiffInf_SIMD<દ્વિગુણ, દ્વિગુણ> {
    દ્વિગુણ operator() (અચલ દ્વિગુણ* src1, અચલ દ્વિગુણ* src2, પૂર્ણાંક n) અચલ {
        પૂર્ણાંક j = 0;
        દ્વિગુણ s = 0;
        v_float64 r0 = vx_setzero_f64(), r1 = vx_setzero_f64();
        માટે (; j <= n - 2 * VTraits<v_float64>::vlanes(); j += 2 * VTraits<v_float64>::vlanes()) {
            v_float64 v01 = vx_load(src1 + j), v02 = vx_load(src2 + j);
            r0 = v_max(r0, v_absdiff(v01, v02));

            v_float64 v11 = vx_load(src1 + j + VTraits<v_float64>::vlanes()),
                      v12 = vx_load(src2 + j + VTraits<v_float64>::vlanes());
            r1 = v_max(r1, v_absdiff(v11, v12));
        }
        // [TODO]: use v_reduce_max when it supports float64
        દ્વિગુણ t[VTraits<v_float64>::max_nlanes];
        vx_store(t, v_max(r0, r1));
        માટે (પૂર્ણાંક i = 0; i < VTraits<v_float64>::vlanes(); i++) {
            s = std::max(s, t[i]);
        }
        માટે (; j < n; j++) {
            દ્વિગુણ v = (દ્વિગુણ)cv_absdiff(src1[j], src2[j]);
            s = std::max(s, v);
        }
        પાછા s;
    }
};

template<>
સંરચના NormDiffL1_SIMD<દશાંશ, દ્વિગુણ> {
    દ્વિગુણ operator() (અચલ દશાંશ* src1, અચલ દશાંશ* src2, પૂર્ણાંક n) અચલ {
        પૂર્ણાંક j = 0;
        દ્વિગુણ s = 0.f;
        v_float64 r0 = vx_setzero_f64(), r1 = vx_setzero_f64();
        v_float64 r2 = vx_setzero_f64(), r3 = vx_setzero_f64();
        માટે (; j <= n - 2 * VTraits<v_float32>::vlanes(); j += 2 * VTraits<v_float32>::vlanes()) {
            v_float32 v01 = vx_load(src1 + j), v02 = vx_load(src2 + j);
            v_float32 v0 = v_absdiff(v01, v02);
            r0 = v_add(r0, v_cvt_f64(v0)); r1 = v_add(r1, v_cvt_f64_ઉપલું(v0));

            v_float32 v11 = vx_load(src1 + j + VTraits<v_float32>::vlanes()),
                      v12 = vx_load(src2 + j + VTraits<v_float32>::vlanes());
            v_float32 v1 = v_absdiff(v11, v12);
            r2 = v_add(r2, v_cvt_f64(v1)); r3 = v_add(r3, v_cvt_f64_ઉપલું(v1));
        }
        s += v_reduce_sum(v_add(v_add(v_add(r0, r1), r2), r3));
        માટે (; j < n; j++) {
            દ્વિગુણ v = (દ્વિગુણ)cv_absdiff(src1[j], src2[j]);
            s += v;
        }
        પાછા s;
    }
};

template<>
સંરચના NormDiffL1_SIMD<દ્વિગુણ, દ્વિગુણ> {
    દ્વિગુણ operator() (અચલ દ્વિગુણ* src1, અચલ દ્વિગુણ* src2, પૂર્ણાંક n) અચલ {
        પૂર્ણાંક j = 0;
        દ્વિગુણ s = 0.f;
        v_float64 r0 = vx_setzero_f64(), r1 = vx_setzero_f64();
        માટે (; j <= n - 2 * VTraits<v_float64>::vlanes(); j += 2 * VTraits<v_float64>::vlanes()) {
            v_float64 v01 = vx_load(src1 + j), v02 = vx_load(src2 + j);
            r0 = v_add(r0, v_absdiff(v01, v02));

            v_float64 v11 = vx_load(src1 + j + VTraits<v_float64>::vlanes()),
                      v12 = vx_load(src2 + j + VTraits<v_float64>::vlanes());
            r1 = v_add(r1, v_absdiff(v11, v12));
        }
        s += v_reduce_sum(v_add(r0, r1));
        માટે (; j < n; j++) {
            દ્વિગુણ v = (દ્વિગુણ)cv_absdiff(src1[j], src2[j]);
            s += v;
        }
        પાછા s;
    }
};

template<>
સંરચના NormDiffL2_SIMD<ushort, દ્વિગુણ> {
    દ્વિગુણ operator() (અચલ ushort* src1, અચલ ushort* src2, પૂર્ણાંક n) અચલ {
        પૂર્ણાંક j = 0;
        દ્વિગુણ s = 0.f;
        v_float64 r0 = vx_setzero_f64(), r1 = vx_setzero_f64();
        માટે (; j <= n - 2 * VTraits<v_uint16>::vlanes(); j += 2 * VTraits<v_uint16>::vlanes()) {
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
        માટે (; j < n; j++) {
            દ્વિગુણ v = (દ્વિગુણ)src1[j] - (દ્વિગુણ)src2[j];
            s += v * v;
        }
        પાછા s;
    }
};

template<>
સંરચના NormDiffL2_SIMD<short, દ્વિગુણ> {
    દ્વિગુણ operator() (અચલ short* src1, અચલ short* src2, પૂર્ણાંક n) અચલ {
        પૂર્ણાંક j = 0;
        દ્વિગુણ s = 0.f;
        v_float64 r0 = vx_setzero_f64(), r1 = vx_setzero_f64();
        માટે (; j <= n - 2 * VTraits<v_int16>::vlanes(); j += 2 * VTraits<v_int16>::vlanes()) {
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
        માટે (; j < n; j++) {
            દ્વિગુણ v = (દ્વિગુણ)src1[j] - (દ્વિગુણ)src2[j];
            s += v * v;
        }
        પાછા s;
    }
};

template<>
સંરચના NormDiffL2_SIMD<પૂર્ણાંક, દ્વિગુણ> {
    દ્વિગુણ operator() (અચલ પૂર્ણાંક* src1, અચલ પૂર્ણાંક* src2, પૂર્ણાંક n) અચલ {
        પૂર્ણાંક j = 0;
        દ્વિગુણ s = 0.f;
        v_float64 r0 = vx_setzero_f64(), r1 = vx_setzero_f64();
        માટે (; j <= n - VTraits<v_int32>::vlanes(); j += VTraits<v_int32>::vlanes()) {
            v_int32 v01 = vx_load(src1 + j), v02 = vx_load(src2 + j);
            v_uint32 v0 = v_absdiff(v01, v02);
            v_uint64 ev0, ev1;
            v_expand(v0, ev0, ev1);
            v_float64 f0 = v_cvt_f64(v_reinterpret_as_s64(ev0)), f1 = v_cvt_f64(v_reinterpret_as_s64(ev1));
            r0 = v_fma(f0, f0, r0); r1 = v_fma(f1, f1, r1);
        }
        s += v_reduce_sum(v_add(r0, r1));
        માટે (; j < n; j++) {
            દ્વિગુણ v = (દ્વિગુણ)src1[j] - (દ્વિગુણ)src2[j];
            s += v * v;
        }
        પાછા s;
    }
};

template<>
સંરચના NormDiffL2_SIMD<દશાંશ, દ્વિગુણ> {
    દ્વિગુણ operator() (અચલ દશાંશ* src1, અચલ દશાંશ* src2, પૂર્ણાંક n) અચલ {
        પૂર્ણાંક j = 0;
        દ્વિગુણ s = 0.f;
        v_float64 r0 = vx_setzero_f64(), r1 = vx_setzero_f64();
        v_float64 r2 = vx_setzero_f64(), r3 = vx_setzero_f64();
        માટે (; j <= n - 2 * VTraits<v_float32>::vlanes(); j += 2 * VTraits<v_float32>::vlanes()) {
            v_float32 v01 = vx_load(src1 + j), v02 = vx_load(src2 + j);
            v_float32 v0 = v_absdiff(v01, v02);
            v_float64 f01 = v_cvt_f64(v0), f02 = v_cvt_f64_ઉપલું(v0);
            r0 = v_fma(f01, f01, r0); r1 = v_fma(f02, f02, r1);

            v_float32 v11 = vx_load(src1 + j + VTraits<v_float32>::vlanes()),
                      v12 = vx_load(src2 + j + VTraits<v_float32>::vlanes());
            v_float32 v1 = v_absdiff(v11, v12);
            v_float64 f11 = v_cvt_f64(v1), f12 = v_cvt_f64_ઉપલું(v1);
            r2 = v_fma(f11, f11, r2); r3 = v_fma(f12, f12, r3);
        }
        s += v_reduce_sum(v_add(v_add(v_add(r0, r1), r2), r3));
        માટે (; j < n; j++) {
            દ્વિગુણ v = (દ્વિગુણ)src1[j] - (દ્વિગુણ)src2[j];
            s += v * v;
        }
        પાછા s;
    }
};

template<>
સંરચના NormDiffL2_SIMD<દ્વિગુણ, દ્વિગુણ> {
    દ્વિગુણ operator() (અચલ દ્વિગુણ* src1, અચલ દ્વિગુણ* src2, પૂર્ણાંક n) અચલ {
        પૂર્ણાંક j = 0;
        દ્વિગુણ s = 0.f;
        v_float64 r0 = vx_setzero_f64(), r1 = vx_setzero_f64();
        v_float64 r2 = vx_setzero_f64(), r3 = vx_setzero_f64();
        માટે (; j <= n - 4 * VTraits<v_float64>::vlanes(); j += 4 * VTraits<v_float64>::vlanes()) {
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
        માટે (; j < n; j++) {
            દ્વિગુણ v = (દ્વિગુણ)src1[j] - (દ્વિગુણ)src2[j];
            s += v * v;
        }
        પાછા s;
    }
};

#endif

template <typename T, typename ST>
સંરચના MaskedNormInf_SIMD {
    અંતઃરેખ ST operator() (અચલ T* src, અચલ uchar* mask, પૂર્ણાંક len, પૂર્ણાંક cn) અચલ {
        ST s = 0;
        જો (cn == 1) {
            માટે (પૂર્ણાંક i = 0; i < len; i++) {
                જો (mask[i]) {
                    s = std::max(s, (ST)cv_abs(src[i]));
                }
            }
        }
        નહીંતર {
            માટે (પૂર્ણાંક i = 0; i < len; i++) {
                જો (mask[i]) {
                    અચલ T* elem = src + i * cn;
                    પૂર્ણાંક k = 0;
                #જો CV_ENABLE_UNROLLED && !CV_SIMD_SCALABLE
                    માટે (; k <= cn - 4; k += 4) {
                        s = std::max(s, (ST)cv_abs(elem[k]));
                        s = std::max(s, (ST)cv_abs(elem[k + 1]));
                        s = std::max(s, (ST)cv_abs(elem[k + 2]));
                        s = std::max(s, (ST)cv_abs(elem[k + 3]));
                    }
                #endif
                    માટે (; k < cn; k++) {
                        s = std::max(s, (ST)cv_abs(elem[k]));
                    }
                }
            }
        }
        પાછા s;
    }
};

template <typename T, typename ST>
સંરચના MaskedNormL1_SIMD {
    અંતઃરેખ ST operator() (અચલ T* src, અચલ uchar* mask, પૂર્ણાંક len, પૂર્ણાંક cn) અચલ {
        ST s = 0;
        જો (cn == 1) {
            માટે (પૂર્ણાંક i = 0; i < len; i++) {
                જો (mask[i]) {
                    s += (ST)cv_abs(src[i]);
                }
            }
        }
        નહીંતર {
            માટે (પૂર્ણાંક i = 0; i < len; i++) {
                જો (mask[i]) {
                    અચલ T* elem = src + i * cn;
                    પૂર્ણાંક k = 0;
                #જો CV_ENABLE_UNROLLED && !CV_SIMD_SCALABLE
                    માટે (; k <= cn - 4; k += 4) {
                        s += (ST)cv_abs(elem[k]);
                        s += (ST)cv_abs(elem[k + 1]);
                        s += (ST)cv_abs(elem[k + 2]);
                        s += (ST)cv_abs(elem[k + 3]);
                    }
                #endif
                    માટે (; k < cn; k++) {
                        s += (ST)cv_abs(elem[k]);
                    }
                }
            }
        }
        પાછા s;
    }
};

template <typename T, typename ST>
સંરચના MaskedNormL2_SIMD {
    અંતઃરેખ ST operator() (અચલ T* src, અચલ uchar* mask, પૂર્ણાંક len, પૂર્ણાંક cn) અચલ {
        ST s = 0;
        જો (cn == 1) {
            પૂર્ણાંક i = 0;
        #જો CV_ENABLE_UNROLLED && !CV_SIMD_SCALABLE
            માટે (; i <= len - 4; i += 4) {
                જો (mask[i])     { T v0 = src[i];     s += (ST)v0 * v0; }
                જો (mask[i + 1]) { T v1 = src[i + 1]; s += (ST)v1 * v1; }
                જો (mask[i + 2]) { T v2 = src[i + 2]; s += (ST)v2 * v2; }
                જો (mask[i + 3]) { T v3 = src[i + 3]; s += (ST)v3 * v3; }
            }
        #endif
            માટે (; i < len; i++) {
                જો (mask[i]) {
                    T v = src[i];
                    s += (ST)v * v;
                }
            }
        }
        નહીંતર {
            માટે (પૂર્ણાંક i = 0; i < len; i++) {
                જો (mask[i]) {
                    અચલ T* elem = src + i * cn;
                    પૂર્ણાંક k = 0;
                #જો CV_ENABLE_UNROLLED && !CV_SIMD_SCALABLE
                    માટે (; k <= cn - 4; k += 4) {
                        T v0 = elem[k];     s += (ST)v0 * v0;
                        T v1 = elem[k + 1]; s += (ST)v1 * v1;
                        T v2 = elem[k + 2]; s += (ST)v2 * v2;
                        T v3 = elem[k + 3]; s += (ST)v3 * v3;
                    }
                #endif
                    માટે (; k < cn; k++) {
                        T v = elem[k];
                        s += (ST)v * v;
                    }
                }
            }
        }

        પાછા s;
    }
};

#જો (CV_SIMD || CV_SIMD_SCALABLE)
સ્થિર અંતઃરેખ v_uint8  v_normmask_u8 (અચલ uchar* m) { પાછા v_gt(vx_load(m),          vx_setzero_u8());  }
સ્થિર અંતઃરેખ v_uint16 v_normmask_u16(અચલ uchar* m) { પાછા v_gt(vx_load_expand(m),   vx_setzero_u16()); }
સ્થિર અંતઃરેખ v_uint32 v_normmask_u32(અચલ uchar* m) { પાછા v_gt(vx_load_expand_q(m), vx_setzero_u32()); }
સ્થિર અંતઃરેખ શૂન્ય v_normmask_u64(અચલ uchar* m, v_uint64& m0, v_uint64& m1) {
    v_uint64 e0, e1;
    v_expand(v_normmask_u32(m), e0, e1);
    m0 = v_or(e0, v_shl<32>(e0));
    m1 = v_or(e1, v_shl<32>(e1));
}

template <>
સંરચના MaskedNormInf_SIMD<દશાંશ, દશાંશ> {
    અંતઃરેખ દશાંશ operator()(અચલ દશાંશ* src, અચલ uchar* mask, પૂર્ણાંક len, પૂર્ણાંક cn) અચલ {
        દશાંશ result = 0.0f;
        જો (cn == 1) {
            પૂર્ણાંક i = 0;
            અચલ પૂર્ણાંક vstep = VTraits<v_float32>::vlanes();
            v_float32 acc = vx_setzero_f32();

            માટે (; i <= len - vstep; i += vstep) {
                v_uint32 m = vx_load_expand_q(mask + i);
                v_uint32 cmp = v_gt(m, vx_setzero_u32());
                v_float32 s = vx_load(src + i);
                s = v_abs(s);
                s = v_reinterpret_as_f32(v_and(v_reinterpret_as_u32(s), cmp));
                acc = v_max(acc, s);
            }
            result = v_reduce_max(acc);

            માટે (; i < len; i++) {
                જો (mask[i])
                    result = std::max(result, std::abs(src[i]));
            }
        }
        નહીંતર {
            માટે (પૂર્ણાંક i = 0; i < len; i++) {
                જો (mask[i]) {
                    અચલ દશાંશ* elem = src + i * cn;
                    પૂર્ણાંક k = 0;
                    અચલ પૂર્ણાંક vstep = VTraits<v_float32>::vlanes();
                    v_float32 acc = vx_setzero_f32();

                    માટે (; k <= cn - vstep; k += vstep) {
                        v_float32 s = vx_load(elem + k);
                        acc = v_max(acc, v_abs(s));
                    }

                    result = std::max(result, v_reduce_max(acc));

                    માટે (; k < cn; k++)
                        result = std::max(result, std::abs(elem[k]));
                }
            }
        }
        પાછા result;
    }
};

#જો CV_SIMD_64F
template <>
સંરચના MaskedNormL1_SIMD<દશાંશ, દ્વિગુણ> {
    અંતઃરેખ દ્વિગુણ operator()(અચલ દશાંશ* src, અચલ uchar* mask, પૂર્ણાંક len, પૂર્ણાંક cn) અચલ {
        દ્વિગુણ result = 0.0;
        જો (cn == 1) {
            પૂર્ણાંક i = 0;
            અચલ પૂર્ણાંક vstep = VTraits<v_float32>::vlanes();
            v_float64 acc0 = vx_setzero_f64(), acc1 = vx_setzero_f64();
            v_float64 acc2 = vx_setzero_f64(), acc3 = vx_setzero_f64();

            માટે (; i <= len - 2*vstep; i += 2*vstep) {
                v_float32 s0 = v_and(v_abs(vx_load(src + i)),
                                     v_reinterpret_as_f32(v_normmask_u32(mask + i)));
                v_float32 s1 = v_and(v_abs(vx_load(src + i + vstep)),
                                     v_reinterpret_as_f32(v_normmask_u32(mask + i + vstep)));
                acc0 = v_add(acc0, v_cvt_f64(s0));
                acc1 = v_add(acc1, v_cvt_f64_ઉપલું(s0));
                acc2 = v_add(acc2, v_cvt_f64(s1));
                acc3 = v_add(acc3, v_cvt_f64_ઉપલું(s1));
            }
            માટે (; i <= len - vstep; i += vstep) {
                v_float32 s = v_and(v_abs(vx_load(src + i)),
                                    v_reinterpret_as_f32(v_normmask_u32(mask + i)));
                acc0 = v_add(acc0, v_cvt_f64(s));
                acc1 = v_add(acc1, v_cvt_f64_ઉપલું(s));
            }
            result = v_reduce_sum(v_add(v_add(acc0, acc1), v_add(acc2, acc3)));

            માટે (; i < len; i++) {
                જો (mask[i])
                    result += std::abs(src[i]);
            }
        }
        નહીંતર {
            માટે (પૂર્ણાંક i = 0; i < len; i++) {
                જો (mask[i]) {
                    અચલ દશાંશ* elem = src + i * cn;
                    પૂર્ણાંક k = 0;
                    અચલ પૂર્ણાંક vstep = VTraits<v_float32>::vlanes();
                    v_float64 acc = vx_setzero_f64();

                    માટે (; k <= cn - vstep; k += vstep) {
                        v_float32 s = v_abs(vx_load(elem + k));
                        acc = v_add(acc, v_cvt_f64(s));
                        acc = v_add(acc, v_cvt_f64_ઉપલું(s));
                    }

                    result += v_reduce_sum(acc);

                    માટે (; k < cn; k++)
                        result += std::abs(elem[k]);
                }
            }
        }
        પાછા result;
    }
};

template <>
સંરચના MaskedNormL2_SIMD<દશાંશ, દ્વિગુણ> {
    અંતઃરેખ દ્વિગુણ operator()(અચલ દશાંશ* src, અચલ uchar* mask, પૂર્ણાંક len, પૂર્ણાંક cn) અચલ {
        દ્વિગુણ result = 0.0;
        જો (cn == 1) {
            પૂર્ણાંક i = 0;
            અચલ પૂર્ણાંક vstep = VTraits<v_float32>::vlanes();
            v_float32 facc = vx_setzero_f32();
            v_float64 dacc = vx_setzero_f64();
            પૂર્ણાંક flush = 0;

            માટે (; i <= len - vstep; i += vstep, flush += vstep) {
                જો (flush >= 64) {
                    dacc = v_add(dacc, v_cvt_f64(facc));
                    dacc = v_add(dacc, v_cvt_f64_ઉપલું(facc));
                    facc = vx_setzero_f32();
                    flush = 0;
                }
                v_uint32 cmp = v_gt(vx_load_expand_q(mask + i), vx_setzero_u32());
                v_float32 s  = v_reinterpret_as_f32(v_and(v_reinterpret_as_u32(vx_load(src + i)), cmp));
                facc = v_add(facc, v_mul(s, s));
            }

            dacc = v_add(dacc, v_cvt_f64(facc));
            dacc = v_add(dacc, v_cvt_f64_ઉપલું(facc));
            result = v_reduce_sum(dacc);

            માટે (; i < len; i++) {
                જો (mask[i]) {
                    દ્વિગુણ v = src[i];
                    result += v * v;
                }
            }
        }
        નહીંતર {
            માટે (પૂર્ણાંક i = 0; i < len; i++) {
                જો (mask[i]) {
                    અચલ દશાંશ* elem = src + i * cn;
                    પૂર્ણાંક k = 0;
                    અચલ પૂર્ણાંક vstep = VTraits<v_float32>::vlanes();
                    v_float32 facc = vx_setzero_f32();

                    માટે (; k <= cn - vstep; k += vstep) {
                        v_float32 s = vx_load(elem + k);
                        facc = v_add(facc, v_mul(s, s));
                    }

                    v_float64 dacc = v_add(v_cvt_f64(facc), v_cvt_f64_ઉપલું(facc));
                    result += v_reduce_sum(dacc);

                    માટે (; k < cn; k++) {
                        દ્વિગુણ v = elem[k];
                        result += v * v;
                    }
                }
            }
        }
        પાછા result;
    }
};

#endif

template <>
સંરચના MaskedNormInf_SIMD<uchar, પૂર્ણાંક> {
    અંતઃરેખ પૂર્ણાંક operator()(અચલ uchar* src, અચલ uchar* mask, પૂર્ણાંક len, પૂર્ણાંક cn) અચલ {
        પૂર્ણાંક result = 0;
        જો (cn == 1) {
            પૂર્ણાંક i = 0;
            અચલ પૂર્ણાંક vstep = VTraits<v_uint8>::vlanes();
            v_uint8 acc = vx_setzero_u8();

            માટે (; i <= len - vstep; i += vstep) {
                v_uint8 m   = vx_load(mask + i);
                v_uint8 s   = vx_load(src + i);
                v_uint8 sel = v_and(s, v_gt(m, vx_setzero_u8()));
                acc = v_max(acc, sel);
            }

            result = (પૂર્ણાંક)v_reduce_max(acc);

            માટે (; i < len; i++) {
                જો (mask[i])
                    result = std::max(result, (પૂર્ણાંક)src[i]);
            }
        }
        નહીંતર જો (cn == 4 && len >= VTraits<v_uint8>::vlanes()) {
            અચલ પૂર્ણાંક vstep = VTraits<v_uint8>::vlanes();
            v_uint8 acc = vx_setzero_u8();
            પૂર્ણાંક i = 0;
            માટે (;;) {
                જો (i > len - vstep)
                    i = len - vstep; // back-step (max is idempotent)
                v_uint8 c0, c1, c2, c3;
                v_load_deinterleave(src + i * 4, c0, c1, c2, c3);
                v_uint8 pmax = v_max(v_max(c0, c1), v_max(c2, c3));
                v_uint8 m = v_gt(vx_load(mask + i), vx_setzero_u8());
                acc = v_max(acc, v_and(pmax, m));
                જો (i >= len - vstep)
                    તોડો;
                i += vstep;
            }
            result = (પૂર્ણાંક)v_reduce_max(acc);
        }
        નહીંતર જો (cn == 4) {
            માટે (પૂર્ણાંક i = 0; i < len; i++) {
                જો (mask[i]) {
                    અચલ uchar* elem = src + i * 4;
                    result = std::max(result, (પૂર્ણાંક)std::max(std::max(elem[0], elem[1]),
                                                            std::max(elem[2], elem[3])));
                }
            }
        }
        નહીંતર {
            માટે (પૂર્ણાંક i = 0; i < len; i++) {
                જો (mask[i]) {
                    અચલ uchar* elem = src + i * cn;
                    માટે (પૂર્ણાંક k = 0; k < cn; k++)
                        result = std::max(result, (પૂર્ણાંક)elem[k]);
                }
            }
        }
        પાછા result;
    }
};

template <>
સંરચના MaskedNormL1_SIMD<uchar, પૂર્ણાંક> {
    અંતઃરેખ પૂર્ણાંક operator()(અચલ uchar* src, અચલ uchar* mask, પૂર્ણાંક len, પૂર્ણાંક cn) અચલ {
        પૂર્ણાંક result = 0;
        અચલ પૂર્ણાંક vstep = VTraits<v_uint8>::vlanes();
        અચલ v_uint8 one = vx_setall_u8(1);
        જો (cn == 1) {
            પૂર્ણાંક i = 0;
            v_uint32 acc = vx_setzero_u32();
            માટે (; i <= len - vstep; i += vstep) {
                v_uint8 s = v_and(vx_load(src + i), v_normmask_u8(mask + i));
                acc = v_dotprod_expand_fast(s, one, acc);
            }
            result = (પૂર્ણાંક)v_reduce_sum(acc);
            માટે (; i < len; i++) {
                જો (mask[i])
                    result += src[i];
            }
        }
        નહીંતર જો (cn == 4) {
            પૂર્ણાંક i = 0;
            v_uint32 acc = vx_setzero_u32();
            માટે (; i <= len - vstep; i += vstep) {
                v_uint8 c0, c1, c2, c3;
                v_load_deinterleave(src + i * 4, c0, c1, c2, c3);
                v_uint8 m = v_normmask_u8(mask + i);
                acc = v_dotprod_expand_fast(v_and(c0, m), one, acc);
                acc = v_dotprod_expand_fast(v_and(c1, m), one, acc);
                acc = v_dotprod_expand_fast(v_and(c2, m), one, acc);
                acc = v_dotprod_expand_fast(v_and(c3, m), one, acc);
            }
            result = (પૂર્ણાંક)v_reduce_sum(acc);
            માટે (; i < len; i++) {
                જો (mask[i]) {
                    અચલ uchar* elem = src + i * 4;
                    result += elem[0] + elem[1] + elem[2] + elem[3];
                }
            }
        }
        નહીંતર {
            માટે (પૂર્ણાંક i = 0; i < len; i++) {
                જો (mask[i]) {
                    અચલ uchar* elem = src + i * cn;
                    માટે (પૂર્ણાંક k = 0; k < cn; k++)
                        result += elem[k];
                }
            }
        }
        પાછા result;
    }
};

template <>
સંરચના MaskedNormInf_SIMD<ushort, પૂર્ણાંક> {
    અંતઃરેખ પૂર્ણાંક operator()(અચલ ushort* src, અચલ uchar* mask, પૂર્ણાંક len, પૂર્ણાંક cn) અચલ {
        પૂર્ણાંક result = 0;
        જો (cn == 1) {
            પૂર્ણાંક i = 0;
            અચલ પૂર્ણાંક vstep = VTraits<v_uint16>::vlanes();
            v_uint16 acc = vx_setzero_u16();

            માટે (; i <= len - vstep; i += vstep) {
                v_uint16 m   = vx_load_expand(mask + i);
                v_uint16 cmp = v_gt(m, vx_setzero_u16());
                v_uint16 s   = vx_load(src + i);
                v_uint16 sel = v_and(s, cmp);
                acc = v_max(acc, sel);
            }

            result = (પૂર્ણાંક)v_reduce_max(acc);

            માટે (; i < len; i++) {
                જો (mask[i])
                    result = std::max(result, (પૂર્ણાંક)src[i]);
            }
        }
        નહીંતર {
            માટે (પૂર્ણાંક i = 0; i < len; i++) {
                જો (mask[i]) {
                    અચલ ushort* elem = src + i * cn;
                    પૂર્ણાંક k = 0;
                    અચલ પૂર્ણાંક vstep = VTraits<v_uint16>::vlanes();
                    v_uint16 acc = vx_setzero_u16();

                    માટે (; k <= cn - vstep; k += vstep) {
                        acc = v_max(acc, vx_load(elem + k));
                    }

                    result = std::max(result, (પૂર્ણાંક)v_reduce_max(acc));

                    માટે (; k < cn; k++)
                        result = std::max(result, (પૂર્ણાંક)elem[k]);
                }
            }
        }
        પાછા result;
    }
};

template <>
સંરચના MaskedNormL1_SIMD<ushort, પૂર્ણાંક> {
    અંતઃરેખ પૂર્ણાંક operator()(અચલ ushort* src, અચલ uchar* mask, પૂર્ણાંક len, પૂર્ણાંક cn) અચલ {
        પૂર્ણાંક result = 0;
        જો (cn == 1) {
            પૂર્ણાંક i = 0;
            અચલ પૂર્ણાંક vstep = VTraits<v_uint16>::vlanes();
            v_uint32 acc32 = vx_setzero_u32();
            v_uint64 acc64 = vx_setzero_u64();
            પૂર્ણાંક acc32_elems = 0;

            માટે (; i <= len - vstep; i += vstep, acc32_elems += vstep) {
                જો (acc32_elems >= 512) {
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
            result = (પૂર્ણાંક)v_reduce_sum(acc64);

            માટે (; i < len; i++) {
                જો (mask[i])
                    result += src[i];
            }
        }
        નહીંતર {
            માટે (પૂર્ણાંક i = 0; i < len; i++) {
                જો (mask[i]) {
                    અચલ ushort* elem = src + i * cn;
                    પૂર્ણાંક k = 0;
                    અચલ પૂર્ણાંક vstep = VTraits<v_uint16>::vlanes();
                    v_uint32 acc = vx_setzero_u32();

                    માટે (; k <= cn - vstep; k += vstep) {
                        v_uint32 lo32, hi32;
                        v_expand(vx_load(elem + k), lo32, hi32);
                        acc = v_add(acc, v_add(lo32, hi32));
                    }

                    result += (પૂર્ણાંક)v_reduce_sum(acc);

                    માટે (; k < cn; k++)
                        result += elem[k];
                }
            }
        }
        પાછા result;
    }
};

template <>
સંરચના MaskedNormL2_SIMD<ushort, દ્વિગુણ> {
    અંતઃરેખ દ્વિગુણ operator()(અચલ ushort* src, અચલ uchar* mask, પૂર્ણાંક len, પૂર્ણાંક cn) અચલ {
        દ્વિગુણ result = 0.0;
        જો (cn == 1) {
            પૂર્ણાંક i = 0;
            અચલ પૂર્ણાંક vstep = VTraits<v_uint16>::vlanes();
            v_uint64 acc = vx_setzero_u64();
            માટે (; i <= len - vstep; i += vstep) {
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
            result = (દ્વિગુણ)v_reduce_sum(acc);
            માટે (; i < len; i++) {
                જો (mask[i]) {
                    દ્વિગુણ v = src[i];
                    result += v * v;
                }
            }
        }
        નહીંતર {
            માટે (પૂર્ણાંક i = 0; i < len; i++) {
                જો (mask[i]) {
                    અચલ ushort* elem = src + i * cn;
                    પૂર્ણાંક k = 0;
                    અચલ પૂર્ણાંક vstep = VTraits<v_uint16>::vlanes();
                    v_uint64 acc = vx_setzero_u64();
                    માટે (; k <= cn - vstep; k += vstep) {
                        v_uint32 lo32, hi32;
                        v_expand(vx_load(elem + k), lo32, hi32);
                        v_uint64 lo64a, lo64b, hi64a, hi64b;
                        v_expand(v_mul(lo32, lo32), lo64a, lo64b);
                        v_expand(v_mul(hi32, hi32), hi64a, hi64b);
                        acc = v_add(acc, v_add(v_add(lo64a, lo64b), v_add(hi64a, hi64b)));
                    }

                    result += (દ્વિગુણ)v_reduce_sum(acc);

                    માટે (; k < cn; k++) {
                        દ્વિગુણ v = elem[k];
                        result += v * v;
                    }
                }
            }
        }
        પાછા result;
    }
};

template <>
સંરચના MaskedNormInf_SIMD<schar, પૂર્ણાંક> {
    અંતઃરેખ પૂર્ણાંક operator()(અચલ schar* src, અચલ uchar* mask, પૂર્ણાંક len, પૂર્ણાંક cn) અચલ {
        પૂર્ણાંક result = 0;
        જો (cn == 1) {
            અચલ પૂર્ણાંક vstep = VTraits<v_int8>::vlanes();
            પૂર્ણાંક i = 0;
            v_uint8 acc = vx_setzero_u8();
            માટે (; i <= len - vstep; i += vstep)
                acc = v_max(acc, v_and(v_abs(vx_load(src + i)), v_normmask_u8(mask + i)));
            result = (પૂર્ણાંક)v_reduce_max(acc);
            માટે (; i < len; i++)
                જો (mask[i]) result = std::max(result, cv_abs(src[i]));
        }
        નહીંતર {
            માટે (પૂર્ણાંક i = 0; i < len; i++)
                જો (mask[i]) {
                    અચલ schar* elem = src + i * cn;
                    માટે (પૂર્ણાંક k = 0; k < cn; k++)
                        result = std::max(result, cv_abs(elem[k]));
                }
        }
        પાછા result;
    }
};

template <>
સંરચના MaskedNormL1_SIMD<schar, પૂર્ણાંક> {
    અંતઃરેખ પૂર્ણાંક operator()(અચલ schar* src, અચલ uchar* mask, પૂર્ણાંક len, પૂર્ણાંક cn) અચલ {
        પૂર્ણાંક result = 0;
        જો (cn == 1) {
            અચલ પૂર્ણાંક vstep = VTraits<v_int8>::vlanes();
            અચલ v_uint8 one = vx_setall_u8(1);
            પૂર્ણાંક i = 0;
            v_uint32 acc = vx_setzero_u32();
            માટે (; i <= len - vstep; i += vstep)
                acc = v_dotprod_expand_fast(v_and(v_abs(vx_load(src + i)), v_normmask_u8(mask + i)), one, acc);
            result = (પૂર્ણાંક)v_reduce_sum(acc);
            માટે (; i < len; i++)
                જો (mask[i]) result += cv_abs(src[i]);
        }
        નહીંતર {
            માટે (પૂર્ણાંક i = 0; i < len; i++)
                જો (mask[i]) {
                    અચલ schar* elem = src + i * cn;
                    માટે (પૂર્ણાંક k = 0; k < cn; k++)
                        result += cv_abs(elem[k]);
                }
        }
        પાછા result;
    }
};

template <>
સંરચના MaskedNormL2_SIMD<schar, પૂર્ણાંક> {
    અંતઃરેખ પૂર્ણાંક operator()(અચલ schar* src, અચલ uchar* mask, પૂર્ણાંક len, પૂર્ણાંક cn) અચલ {
        પૂર્ણાંક result = 0;
        જો (cn == 1) {
            અચલ પૂર્ણાંક vstep = VTraits<v_int8>::vlanes();
            પૂર્ણાંક i = 0;
            v_int32 acc = vx_setzero_s32();
            માટે (; i <= len - vstep; i += vstep) {
                v_int8 s = v_and(vx_load(src + i), v_reinterpret_as_s8(v_normmask_u8(mask + i)));
                acc = v_dotprod_expand_fast(s, s, acc);
            }
            result = (પૂર્ણાંક)v_reduce_sum(acc);
            માટે (; i < len; i++)
                જો (mask[i]) { પૂર્ણાંક v = src[i]; result += v * v; }
        }
        નહીંતર {
            માટે (પૂર્ણાંક i = 0; i < len; i++)
                જો (mask[i]) {
                    અચલ schar* elem = src + i * cn;
                    માટે (પૂર્ણાંક k = 0; k < cn; k++) { પૂર્ણાંક v = elem[k]; result += v * v; }
                }
        }
        પાછા result;
    }
};

template <>
સંરચના MaskedNormL2_SIMD<uchar, પૂર્ણાંક> {
    અંતઃરેખ પૂર્ણાંક operator()(અચલ uchar* src, અચલ uchar* mask, પૂર્ણાંક len, પૂર્ણાંક cn) અચલ {
        પૂર્ણાંક result = 0;
        અચલ પૂર્ણાંક vstep = VTraits<v_uint8>::vlanes();
        જો (cn == 1) {
            પૂર્ણાંક i = 0;
            v_uint32 acc = vx_setzero_u32();
            માટે (; i <= len - vstep; i += vstep) {
                v_uint8 s = v_and(vx_load(src + i), v_normmask_u8(mask + i));
                acc = v_dotprod_expand_fast(s, s, acc);
            }
            result = (પૂર્ણાંક)v_reduce_sum(acc);
            માટે (; i < len; i++)
                જો (mask[i]) { પૂર્ણાંક v = src[i]; result += v * v; }
        }
        નહીંતર જો (cn == 4) {
            પૂર્ણાંક i = 0;
            v_uint32 acc = vx_setzero_u32();
            માટે (; i <= len - vstep; i += vstep) {
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
            result = (પૂર્ણાંક)v_reduce_sum(acc);
            માટે (; i < len; i++)
                જો (mask[i]) {
                    અચલ uchar* elem = src + i * 4;
                    માટે (પૂર્ણાંક k = 0; k < 4; k++) { પૂર્ણાંક v = elem[k]; result += v * v; }
                }
        }
        નહીંતર {
            માટે (પૂર્ણાંક i = 0; i < len; i++)
                જો (mask[i]) {
                    અચલ uchar* elem = src + i * cn;
                    માટે (પૂર્ણાંક k = 0; k < cn; k++) { પૂર્ણાંક v = elem[k]; result += v * v; }
                }
        }
        પાછા result;
    }
};

template <>
સંરચના MaskedNormInf_SIMD<short, પૂર્ણાંક> {
    અંતઃરેખ પૂર્ણાંક operator()(અચલ short* src, અચલ uchar* mask, પૂર્ણાંક len, પૂર્ણાંક cn) અચલ {
        પૂર્ણાંક result = 0;
        જો (cn == 1) {
            અચલ પૂર્ણાંક vstep = VTraits<v_int16>::vlanes();
            પૂર્ણાંક i = 0;
            v_uint16 acc = vx_setzero_u16();
            માટે (; i <= len - vstep; i += vstep)
                acc = v_max(acc, v_and(v_abs(vx_load(src + i)), v_normmask_u16(mask + i)));
            result = (પૂર્ણાંક)v_reduce_max(acc);
            માટે (; i < len; i++)
                જો (mask[i]) result = std::max(result, cv_abs(src[i]));
        }
        નહીંતર {
            માટે (પૂર્ણાંક i = 0; i < len; i++)
                જો (mask[i]) {
                    અચલ short* elem = src + i * cn;
                    માટે (પૂર્ણાંક k = 0; k < cn; k++)
                        result = std::max(result, cv_abs(elem[k]));
                }
        }
        પાછા result;
    }
};

template <>
સંરચના MaskedNormL1_SIMD<short, પૂર્ણાંક> {
    અંતઃરેખ પૂર્ણાંક operator()(અચલ short* src, અચલ uchar* mask, પૂર્ણાંક len, પૂર્ણાંક cn) અચલ {
        પૂર્ણાંક result = 0;
        જો (cn == 1) {
            અચલ પૂર્ણાંક vstep = VTraits<v_int16>::vlanes();
            પૂર્ણાંક i = 0;
            v_uint32 acc0 = vx_setzero_u32(), acc1 = vx_setzero_u32();
            માટે (; i <= len - vstep; i += vstep) {
                v_uint16 s = v_and(v_abs(vx_load(src + i)), v_normmask_u16(mask + i));
                v_uint32 lo, hi;
                v_expand(s, lo, hi);
                acc0 = v_add(acc0, lo);
                acc1 = v_add(acc1, hi);
            }
            result = (પૂર્ણાંક)v_reduce_sum(v_add(acc0, acc1));
            માટે (; i < len; i++)
                જો (mask[i]) result += cv_abs(src[i]);
        }
        નહીંતર {
            માટે (પૂર્ણાંક i = 0; i < len; i++)
                જો (mask[i]) {
                    અચલ short* elem = src + i * cn;
                    માટે (પૂર્ણાંક k = 0; k < cn; k++)
                        result += cv_abs(elem[k]);
                }
        }
        પાછા result;
    }
};

template <>
સંરચના MaskedNormL2_SIMD<short, દ્વિગુણ> {
    અંતઃરેખ દ્વિગુણ operator()(અચલ short* src, અચલ uchar* mask, પૂર્ણાંક len, પૂર્ણાંક cn) અચલ {
        દ્વિગુણ result = 0.0;
        જો (cn == 1) {
            અચલ પૂર્ણાંક vstep = VTraits<v_int16>::vlanes();
            પૂર્ણાંક i = 0;
            v_uint64 acc = vx_setzero_u64();
            માટે (; i <= len - vstep; i += vstep) {
                v_uint16 s = v_and(v_abs(vx_load(src + i)), v_normmask_u16(mask + i));
                acc = v_add(acc, v_dotprod_expand_fast(s, s));
            }
            result = (દ્વિગુણ)v_reduce_sum(acc);
            માટે (; i < len; i++)
                જો (mask[i]) { દ્વિગુણ v = src[i]; result += v * v; }
        }
        નહીંતર {
            માટે (પૂર્ણાંક i = 0; i < len; i++)
                જો (mask[i]) {
                    અચલ short* elem = src + i * cn;
                    માટે (પૂર્ણાંક k = 0; k < cn; k++) { દ્વિગુણ v = elem[k]; result += v * v; }
                }
        }
        પાછા result;
    }
};

template <>
સંરચના MaskedNormInf_SIMD<પૂર્ણાંક, unsigned> {
    અંતઃરેખ unsigned operator()(અચલ પૂર્ણાંક* src, અચલ uchar* mask, પૂર્ણાંક len, પૂર્ણાંક cn) અચલ {
        unsigned result = 0;
        જો (cn == 1) {
            અચલ પૂર્ણાંક vstep = VTraits<v_int32>::vlanes();
            પૂર્ણાંક i = 0;
            v_uint32 acc = vx_setzero_u32();
            માટે (; i <= len - vstep; i += vstep)
                acc = v_max(acc, v_and(v_abs(vx_load(src + i)), v_normmask_u32(mask + i)));
            result = v_reduce_max(acc);
            માટે (; i < len; i++)
                જો (mask[i]) result = std::max(result, cv_abs(src[i]));
        }
        નહીંતર {
            માટે (પૂર્ણાંક i = 0; i < len; i++)
                જો (mask[i]) {
                    અચલ પૂર્ણાંક* elem = src + i * cn;
                    માટે (પૂર્ણાંક k = 0; k < cn; k++)
                        result = std::max(result, cv_abs(elem[k]));
                }
        }
        પાછા result;
    }
};

#જો CV_SIMD_64F || CV_SIMD_SCALABLE_64F

template <>
સંરચના MaskedNormL1_SIMD<પૂર્ણાંક, દ્વિગુણ> {
    અંતઃરેખ દ્વિગુણ operator()(અચલ પૂર્ણાંક* src, અચલ uchar* mask, પૂર્ણાંક len, પૂર્ણાંક cn) અચલ {
        દ્વિગુણ result = 0.0;
        જો (cn == 1) {
            અચલ પૂર્ણાંક vstep = VTraits<v_int32>::vlanes();
            પૂર્ણાંક i = 0;
            v_float64 acc0 = vx_setzero_f64(), acc1 = vx_setzero_f64();
            માટે (; i <= len - vstep; i += vstep) {
                // Mask the value, then widen and take the magnitude in double. v_abs() does give
                // |INT_MIN| as an unsigned 2147483648, but reinterpreting that back as int32 turns
                // it negative again, and the widening then carries the sign into the accumulator.
                v_int32 s = v_reinterpret_as_s32(v_and(v_reinterpret_as_u32(vx_load(src + i)),
                                                       v_normmask_u32(mask + i)));
                acc0 = v_add(acc0, v_abs(v_cvt_f64(s)));
                acc1 = v_add(acc1, v_abs(v_cvt_f64_ઉપલું(s)));
            }
            result = v_reduce_sum(v_add(acc0, acc1));
            માટે (; i < len; i++)
                જો (mask[i]) result += cv_abs(src[i]);
        }
        નહીંતર {
            માટે (પૂર્ણાંક i = 0; i < len; i++)
                જો (mask[i]) {
                    અચલ પૂર્ણાંક* elem = src + i * cn;
                    માટે (પૂર્ણાંક k = 0; k < cn; k++)
                        result += cv_abs(elem[k]);
                }
        }
        પાછા result;
    }
};

template <>
સંરચના MaskedNormL2_SIMD<પૂર્ણાંક, દ્વિગુણ> {
    અંતઃરેખ દ્વિગુણ operator()(અચલ પૂર્ણાંક* src, અચલ uchar* mask, પૂર્ણાંક len, પૂર્ણાંક cn) અચલ {
        દ્વિગુણ result = 0.0;
        જો (cn == 1) {
            અચલ પૂર્ણાંક vstep = VTraits<v_int32>::vlanes();
            પૂર્ણાંક i = 0;
            v_float64 acc0 = vx_setzero_f64(), acc1 = vx_setzero_f64();
            માટે (; i <= len - vstep; i += vstep) {
                v_uint32 s = v_and(v_abs(vx_load(src + i)), v_normmask_u32(mask + i));
                v_uint64 e0, e1;
                v_expand(s, e0, e1);
                v_float64 f0 = v_cvt_f64(v_reinterpret_as_s64(e0));
                v_float64 f1 = v_cvt_f64(v_reinterpret_as_s64(e1));
                acc0 = v_fma(f0, f0, acc0);
                acc1 = v_fma(f1, f1, acc1);
            }
            result = v_reduce_sum(v_add(acc0, acc1));
            માટે (; i < len; i++)
                જો (mask[i]) { દ્વિગુણ v = src[i]; result += v * v; }
        }
        નહીંતર {
            માટે (પૂર્ણાંક i = 0; i < len; i++)
                જો (mask[i]) {
                    અચલ પૂર્ણાંક* elem = src + i * cn;
                    માટે (પૂર્ણાંક k = 0; k < cn; k++) { દ્વિગુણ v = elem[k]; result += v * v; }
                }
        }
        પાછા result;
    }
};

template <>
સંરચના MaskedNormInf_SIMD<દ્વિગુણ, દ્વિગુણ> {
    અંતઃરેખ દ્વિગુણ operator()(અચલ દ્વિગુણ* src, અચલ uchar* mask, પૂર્ણાંક len, પૂર્ણાંક cn) અચલ {
        દ્વિગુણ result = 0.0;
        જો (cn == 1) {
            અચલ પૂર્ણાંક vstep = VTraits<v_float64>::vlanes();
            પૂર્ણાંક i = 0;
            v_float64 acc0 = vx_setzero_f64(), acc1 = vx_setzero_f64();
            માટે (; i <= len - 2*vstep; i += 2*vstep) {
                v_uint64 m0, m1;
                v_normmask_u64(mask + i, m0, m1);
                acc0 = v_max(acc0, v_reinterpret_as_f64(v_and(v_reinterpret_as_u64(v_abs(vx_load(src + i))), m0)));
                acc1 = v_max(acc1, v_reinterpret_as_f64(v_and(v_reinterpret_as_u64(v_abs(vx_load(src + i + vstep))), m1)));
            }
            દ્વિગુણ t[VTraits<v_float64>::max_nlanes];
            vx_store(t, v_max(acc0, acc1));
            માટે (પૂર્ણાંક k = 0; k < vstep; k++)
                result = std::max(result, t[k]);
            માટે (; i < len; i++)
                જો (mask[i]) result = std::max(result, cv_abs(src[i]));
        }
        નહીંતર {
            માટે (પૂર્ણાંક i = 0; i < len; i++)
                જો (mask[i]) {
                    અચલ દ્વિગુણ* elem = src + i * cn;
                    માટે (પૂર્ણાંક k = 0; k < cn; k++)
                        result = std::max(result, cv_abs(elem[k]));
                }
        }
        પાછા result;
    }
};

template <>
સંરચના MaskedNormL1_SIMD<દ્વિગુણ, દ્વિગુણ> {
    અંતઃરેખ દ્વિગુણ operator()(અચલ દ્વિગુણ* src, અચલ uchar* mask, પૂર્ણાંક len, પૂર્ણાંક cn) અચલ {
        દ્વિગુણ result = 0.0;
        જો (cn == 1) {
            અચલ પૂર્ણાંક vstep = VTraits<v_float64>::vlanes();
            પૂર્ણાંક i = 0;
            v_float64 acc0 = vx_setzero_f64(), acc1 = vx_setzero_f64();
            માટે (; i <= len - 2*vstep; i += 2*vstep) {
                v_uint64 m0, m1;
                v_normmask_u64(mask + i, m0, m1);
                acc0 = v_add(acc0, v_reinterpret_as_f64(v_and(v_reinterpret_as_u64(v_abs(vx_load(src + i))), m0)));
                acc1 = v_add(acc1, v_reinterpret_as_f64(v_and(v_reinterpret_as_u64(v_abs(vx_load(src + i + vstep))), m1)));
            }
            result = v_reduce_sum(v_add(acc0, acc1));
            માટે (; i < len; i++)
                જો (mask[i]) result += cv_abs(src[i]);
        }
        નહીંતર {
            માટે (પૂર્ણાંક i = 0; i < len; i++)
                જો (mask[i]) {
                    અચલ દ્વિગુણ* elem = src + i * cn;
                    માટે (પૂર્ણાંક k = 0; k < cn; k++)
                        result += cv_abs(elem[k]);
                }
        }
        પાછા result;
    }
};

template <>
સંરચના MaskedNormL2_SIMD<દ્વિગુણ, દ્વિગુણ> {
    અંતઃરેખ દ્વિગુણ operator()(અચલ દ્વિગુણ* src, અચલ uchar* mask, પૂર્ણાંક len, પૂર્ણાંક cn) અચલ {
        દ્વિગુણ result = 0.0;
        જો (cn == 1) {
            અચલ પૂર્ણાંક vstep = VTraits<v_float64>::vlanes();
            પૂર્ણાંક i = 0;
            v_float64 acc0 = vx_setzero_f64(), acc1 = vx_setzero_f64();
            માટે (; i <= len - 2*vstep; i += 2*vstep) {
                v_uint64 m0, m1;
                v_normmask_u64(mask + i, m0, m1);
                v_float64 s0 = v_reinterpret_as_f64(v_and(v_reinterpret_as_u64(vx_load(src + i)), m0));
                v_float64 s1 = v_reinterpret_as_f64(v_and(v_reinterpret_as_u64(vx_load(src + i + vstep)), m1));
                acc0 = v_fma(s0, s0, acc0);
                acc1 = v_fma(s1, s1, acc1);
            }
            result = v_reduce_sum(v_add(acc0, acc1));
            માટે (; i < len; i++)
                જો (mask[i]) { દ્વિગુણ v = src[i]; result += v * v; }
        }
        નહીંતર {
            માટે (પૂર્ણાંક i = 0; i < len; i++)
                જો (mask[i]) {
                    અચલ દ્વિગુણ* elem = src + i * cn;
                    માટે (પૂર્ણાંક k = 0; k < cn; k++) { દ્વિગુણ v = elem[k]; result += v * v; }
                }
        }
        પાછા result;
    }
};

#endif // CV_SIMD_64F || CV_SIMD_SCALABLE_64F
#endif // CV_SIMD || CV_SIMD_SCALABLE

template<typename T, typename ST> પૂર્ણાંક
normInf_(અચલ T* src, અચલ uchar* mask, ST* _result, પૂર્ણાંક len, પૂર્ણાંક cn)
{
    ST result = *_result;
    જો( !mask )
    {
        NormInf_SIMD<T, ST> op;
        result = std::max(result, op(src, len*cn));
    } નહીંતર {
        MaskedNormInf_SIMD<T, ST> op;
        result = std::max(result, op(src, mask, len, cn));
    }
    *_result = result;
    પાછા 0;
}

template<typename T, typename ST> પૂર્ણાંક
normL1_(અચલ T* src, અચલ uchar* mask, ST* _result, પૂર્ણાંક len, પૂર્ણાંક cn)
{
    ST result = *_result;
    જો( !mask )
    {
        NormL1_SIMD<T, ST> op;
        result += op(src, len*cn);
    } નહીંતર {
        MaskedNormL1_SIMD<T, ST> op;
        result += op(src, mask, len, cn);
    }
    *_result = result;
    પાછા 0;
}

template<typename T, typename ST> પૂર્ણાંક
normL2_(અચલ T* src, અચલ uchar* mask, ST* _result, પૂર્ણાંક len, પૂર્ણાંક cn)
{
    ST result = *_result;
    જો( !mask )
    {
        NormL2_SIMD<T, ST> op;
        result += op(src, len*cn);
    }
    નહીંતર
    {
        MaskedNormL2_SIMD<T, ST> op;
        result += op(src, mask, len, cn);
    }
    *_result = result;
    પાછા 0;
}

સ્થિર પૂર્ણાંક
normInf_Bool(અચલ uchar* src, અચલ uchar* mask, પૂર્ણાંક* _result, પૂર્ણાંક len, પૂર્ણાંક cn)
{
    પૂર્ણાંક result = *_result;
    જો( !mask )
    {
        માટે ( પૂર્ણાંક i = 0; i < len*cn; i++ ) {
            result = std::max(result, (પૂર્ણાંક)(src[i] != 0));
            જો (result != 0)
                તોડો;
        }
    }
    નહીંતર
    {
        માટે( પૂર્ણાંક i = 0; i < len; i++, src += cn )
            જો( mask[i] )
            {
                માટે( પૂર્ણાંક k = 0; k < cn; k++ )
                    result = std::max(result, (પૂર્ણાંક)(src[k] != 0));
                જો (result != 0)
                    તોડો;
            }
    }
    *_result = result;
    પાછા 0;
}

સ્થિર પૂર્ણાંક
normL1_Bool(અચલ uchar* src, અચલ uchar* mask, પૂર્ણાંક* _result, પૂર્ણાંક len, પૂર્ણાંક cn)
{
    પૂર્ણાંક result = *_result;
    જો( !mask )
    {
        માટે ( પૂર્ણાંક i = 0; i < len*cn; i++ )
            result += (પૂર્ણાંક)(src[i] != 0);
    }
    નહીંતર
    {
        માટે( પૂર્ણાંક i = 0; i < len; i++, src += cn )
            જો( mask[i] )
            {
                માટે( પૂર્ણાંક k = 0; k < cn; k++ )
                    result += (પૂર્ણાંક)(src[k] != 0);
            }
    }
    *_result = result;
    પાછા 0;
}

સ્થિર પૂર્ણાંક
normL2_Bool(અચલ uchar* src, અચલ uchar* mask, પૂર્ણાંક* _result, પૂર્ણાંક len, પૂર્ણાંક cn)
{
    પાછા normL1_Bool(src, mask, _result, len, cn);
}

// ====================================================================
// Masked norm-diff kernels.
// Scalar base templates (safe for every depth) plus SIMD cn==1
// specializations for the concrete (T, ST) pairs dispatched by
// CV_DEF_NORM_ALL. The masked normDiff*_ functions below select them.
// ====================================================================
template <typename T, typename ST>
સંરચના MaskedNormDiffInf_SIMD {
    અંતઃરેખ ST operator()(અચલ T* s1, અચલ T* s2, અચલ uchar* mask, પૂર્ણાંક len, પૂર્ણાંક cn) અચલ {
        ST result = 0;
        માટે (પૂર્ણાંક i = 0; i < len; i++) જો (mask[i]) {
            અચલ T* e1 = s1 + i*cn; અચલ T* e2 = s2 + i*cn;
            માટે (પૂર્ણાંક k = 0; k < cn; k++)
                result = std::max(result, (ST)cv_absdiff(e1[k], e2[k]));
        }
        પાછા result;
    }
};
template <typename T, typename ST>
સંરચના MaskedNormDiffL1_SIMD {
    અંતઃરેખ ST operator()(અચલ T* s1, અચલ T* s2, અચલ uchar* mask, પૂર્ણાંક len, પૂર્ણાંક cn) અચલ {
        ST result = 0;
        માટે (પૂર્ણાંક i = 0; i < len; i++) જો (mask[i]) {
            અચલ T* e1 = s1 + i*cn; અચલ T* e2 = s2 + i*cn;
            માટે (પૂર્ણાંક k = 0; k < cn; k++)
                result += (ST)cv_absdiff(e1[k], e2[k]);
        }
        પાછા result;
    }
};
template <typename T, typename ST>
સંરચના MaskedNormDiffL2_SIMD {
    અંતઃરેખ ST operator()(અચલ T* s1, અચલ T* s2, અચલ uchar* mask, પૂર્ણાંક len, પૂર્ણાંક cn) અચલ {
        ST result = 0;
        માટે (પૂર્ણાંક i = 0; i < len; i++) જો (mask[i]) {
            અચલ T* e1 = s1 + i*cn; અચલ T* e2 = s2 + i*cn;
            માટે (પૂર્ણાંક k = 0; k < cn; k++) { ST v = (ST)e1[k] - (ST)e2[k]; result += v*v; }
        }
        પાછા result;
    }
};

#જો (CV_SIMD || CV_SIMD_SCALABLE)
// AND a per-lane diff vector with the (expanded) mask>0 predicate.
અંતઃરેખ v_uint8  v_diffmask(અચલ v_uint8&  d, અચલ uchar* m) { પાછા v_and(d, v_gt(vx_load(m),          vx_setzero_u8())); }
અંતઃરેખ v_uint16 v_diffmask(અચલ v_uint16& d, અચલ uchar* m) { પાછા v_and(d, v_gt(vx_load_expand(m),   vx_setzero_u16())); }
અંતઃરેખ v_uint32 v_diffmask(અચલ v_uint32& d, અચલ uchar* m) { પાછા v_and(d, v_gt(vx_load_expand_q(m), vx_setzero_u32())); }
અંતઃરેખ v_float32 v_diffmask(અચલ v_float32& d, અચલ uchar* m) {
    v_uint32 cm = v_gt(vx_load_expand_q(m), vx_setzero_u32());
    પાછા v_reinterpret_as_f32(v_and(v_reinterpret_as_u32(d), cm));
}

// Shared scalar cn>1 fallbacks (match the base templates' cv_absdiff semantics).
template<typename T, typename RT>
સ્થિર અંતઃરેખ RT maskedNormDiffInfTail(અચલ T* s1, અચલ T* s2, અચલ uchar* mask, પૂર્ણાંક len, પૂર્ણાંક cn, RT result) {
    માટે (પૂર્ણાંક i = 0; i < len; i++) જો (mask[i]) {
        અચલ T* e1 = s1 + i*cn; અચલ T* e2 = s2 + i*cn;
        માટે (પૂર્ણાંક k = 0; k < cn; k++) result = std::max(result, (RT)cv_absdiff(e1[k], e2[k]));
    }
    પાછા result;
}
template<typename T, typename RT>
સ્થિર અંતઃરેખ RT maskedNormDiffL1Tail(અચલ T* s1, અચલ T* s2, અચલ uchar* mask, પૂર્ણાંક len, પૂર્ણાંક cn, RT result) {
    માટે (પૂર્ણાંક i = 0; i < len; i++) જો (mask[i]) {
        અચલ T* e1 = s1 + i*cn; અચલ T* e2 = s2 + i*cn;
        માટે (પૂર્ણાંક k = 0; k < cn; k++) result += (RT)cv_absdiff(e1[k], e2[k]);
    }
    પાછા result;
}
template<typename T, typename RT>
સ્થિર અંતઃરેખ RT maskedNormDiffL2Tail(અચલ T* s1, અચલ T* s2, અચલ uchar* mask, પૂર્ણાંક len, પૂર્ણાંક cn, RT result) {
    માટે (પૂર્ણાંક i = 0; i < len; i++) જો (mask[i]) {
        અચલ T* e1 = s1 + i*cn; અચલ T* e2 = s2 + i*cn;
        માટે (પૂર્ણાંક k = 0; k < cn; k++) { RT v = (RT)e1[k] - (RT)e2[k]; result += v*v; }
    }
    પાછા result;
}

// Inf (cn==1): reduce_max of masked |a-b|. Shared across SIMD-capable depths.
template<typename T, typename ST>
સ્થિર અંતઃરેખ ST maskedNormDiffInfCn1(અચલ T* s1, અચલ T* s2, અચલ uchar* mask, પૂર્ણાંક len) {
    અચલ પૂર્ણાંક vstep = VTraits<decltype(vx_load(s1))>::vlanes();
    ST result = 0;
    પૂર્ણાંક i = 0;
    જો (len >= vstep) {
        auto acc = v_diffmask(v_absdiff(vx_load(s1), vx_load(s2)), mask);
        માટે (i = vstep; i <= len - vstep; i += vstep)
            acc = v_max(acc, v_diffmask(v_absdiff(vx_load(s1 + i), vx_load(s2 + i)), mask + i));
        result = (ST)v_reduce_max(acc);
    }
    માટે (; i < len; i++) જો (mask[i]) result = std::max(result, (ST)cv_absdiff(s1[i], s2[i]));
    પાછા result;
}

// Shared 8-bit cn==1 masked L1/L2 kernels (uchar/schar): v_absdiff yields v_uint8 for both.
template<typename T>
સ્થિર અંતઃરેખ પૂર્ણાંક maskedNormDiffL1_8(અચલ T* s1, અચલ T* s2, અચલ uchar* mask, પૂર્ણાંક len) {
    પૂર્ણાંક i = 0; અચલ પૂર્ણાંક vstep = VTraits<v_uint8>::vlanes();
    અચલ v_uint8 one = vx_setall_u8(1);
    v_uint32 acc = vx_setzero_u32();
    માટે (; i <= len - vstep; i += vstep) {
        v_uint8 m  = v_gt(vx_load(mask + i), vx_setzero_u8());
        v_uint8 ad = v_and(v_absdiff(vx_load(s1 + i), vx_load(s2 + i)), m);
        acc = v_dotprod_expand_fast(ad, one, acc);
    }
    પૂર્ણાંક result = (પૂર્ણાંક)v_reduce_sum(acc);
    માટે (; i < len; i++) જો (mask[i]) result += (પૂર્ણાંક)cv_absdiff(s1[i], s2[i]);
    પાછા result;
}
template<typename T>
સ્થિર અંતઃરેખ પૂર્ણાંક maskedNormDiffL2_8(અચલ T* s1, અચલ T* s2, અચલ uchar* mask, પૂર્ણાંક len) {
    પૂર્ણાંક i = 0; અચલ પૂર્ણાંક vstep = VTraits<v_uint8>::vlanes();
    v_uint32 acc = vx_setzero_u32();
    માટે (; i <= len - vstep; i += vstep) {
        v_uint8 m  = v_gt(vx_load(mask + i), vx_setzero_u8());
        v_uint8 ad = v_and(v_absdiff(vx_load(s1 + i), vx_load(s2 + i)), m);
        acc = v_dotprod_expand_fast(ad, ad, acc);
    }
    પૂર્ણાંક result = (પૂર્ણાંક)v_reduce_sum(acc);
    માટે (; i < len; i++) જો (mask[i]) { પૂર્ણાંક v = (પૂર્ણાંક)s1[i] - (પૂર્ણાંક)s2[i]; result += v*v; }
    પાછા result;
}

// Shared 16-bit cn==1 masked L1 kernel (ushort/short): v_absdiff yields v_uint16 for both.
template<typename T>
સ્થિર અંતઃરેખ પૂર્ણાંક maskedNormDiffL1_16(અચલ T* s1, અચલ T* s2, અચલ uchar* mask, પૂર્ણાંક len) {
    પૂર્ણાંક i = 0; અચલ પૂર્ણાંક vstep = VTraits<v_uint16>::vlanes();
    v_uint32 acc = vx_setzero_u32();
    માટે (; i <= len - vstep; i += vstep) {
        v_uint16 m  = v_gt(vx_load_expand(mask + i), vx_setzero_u16());
        v_uint16 ad = v_and(v_absdiff(vx_load(s1 + i), vx_load(s2 + i)), m);
        v_uint32 lo, hi; v_expand(ad, lo, hi);
        acc = v_add(acc, v_add(lo, hi));
    }
    પૂર્ણાંક result = (પૂર્ણાંક)v_reduce_sum(acc);
    માટે (; i < len; i++) જો (mask[i]) result += (પૂર્ણાંક)cv_absdiff(s1[i], s2[i]);
    પાછા result;
}

template<> સંરચના MaskedNormDiffInf_SIMD<uchar, પૂર્ણાંક> {
    પૂર્ણાંક operator()(અચલ uchar* s1, અચલ uchar* s2, અચલ uchar* mask, પૂર્ણાંક len, પૂર્ણાંક cn) અચલ {
        જો (cn == 1) પાછા maskedNormDiffInfCn1<uchar, પૂર્ણાંક>(s1, s2, mask, len);
        જો (cn == 4 && len >= VTraits<v_uint8>::vlanes()) {
            અચલ પૂર્ણાંક vstep = VTraits<v_uint8>::vlanes();
            v_uint8 acc = vx_setzero_u8();
            પૂર્ણાંક i = 0;
            માટે (;;) {
                જો (i > len - vstep) i = len - vstep; // back-step (max is idempotent)
                v_uint8 a0,a1,a2,a3,b0,b1,b2,b3;
                v_load_deinterleave(s1 + i*4, a0,a1,a2,a3);
                v_load_deinterleave(s2 + i*4, b0,b1,b2,b3);
                v_uint8 ad = v_max(v_max(v_absdiff(a0,b0), v_absdiff(a1,b1)),
                                   v_max(v_absdiff(a2,b2), v_absdiff(a3,b3)));
                v_uint8 m = v_gt(vx_load(mask + i), vx_setzero_u8());
                acc = v_max(acc, v_and(ad, m));
                જો (i >= len - vstep) તોડો;
                i += vstep;
            }
            પાછા (પૂર્ણાંક)v_reduce_max(acc);
        }
        પાછા maskedNormDiffInfTail<uchar, પૂર્ણાંક>(s1, s2, mask, len, cn, 0);
    }
};
template<> સંરચના MaskedNormDiffInf_SIMD<schar, પૂર્ણાંક> {
    પૂર્ણાંક operator()(અચલ schar* s1, અચલ schar* s2, અચલ uchar* mask, પૂર્ણાંક len, પૂર્ણાંક cn) અચલ {
        જો (cn == 1) પાછા maskedNormDiffInfCn1<schar, પૂર્ણાંક>(s1, s2, mask, len);
        પાછા maskedNormDiffInfTail<schar, પૂર્ણાંક>(s1, s2, mask, len, cn, 0);
    }
};
template<> સંરચના MaskedNormDiffInf_SIMD<ushort, પૂર્ણાંક> {
    પૂર્ણાંક operator()(અચલ ushort* s1, અચલ ushort* s2, અચલ uchar* mask, પૂર્ણાંક len, પૂર્ણાંક cn) અચલ {
        જો (cn == 1) પાછા maskedNormDiffInfCn1<ushort, પૂર્ણાંક>(s1, s2, mask, len);
        પાછા maskedNormDiffInfTail<ushort, પૂર્ણાંક>(s1, s2, mask, len, cn, 0);
    }
};
template<> સંરચના MaskedNormDiffInf_SIMD<short, પૂર્ણાંક> {
    પૂર્ણાંક operator()(અચલ short* s1, અચલ short* s2, અચલ uchar* mask, પૂર્ણાંક len, પૂર્ણાંક cn) અચલ {
        જો (cn == 1) પાછા maskedNormDiffInfCn1<short, પૂર્ણાંક>(s1, s2, mask, len);
        પાછા maskedNormDiffInfTail<short, પૂર્ણાંક>(s1, s2, mask, len, cn, 0);
    }
};
template<> સંરચના MaskedNormDiffInf_SIMD<પૂર્ણાંક, unsigned> {
    unsigned operator()(અચલ પૂર્ણાંક* s1, અચલ પૂર્ણાંક* s2, અચલ uchar* mask, પૂર્ણાંક len, પૂર્ણાંક cn) અચલ {
        જો (cn == 1) પાછા maskedNormDiffInfCn1<પૂર્ણાંક, unsigned>(s1, s2, mask, len);
        પાછા maskedNormDiffInfTail<પૂર્ણાંક, unsigned>(s1, s2, mask, len, cn, 0u);
    }
};
template<> સંરચના MaskedNormDiffInf_SIMD<દશાંશ, દશાંશ> {
    દશાંશ operator()(અચલ દશાંશ* s1, અચલ દશાંશ* s2, અચલ uchar* mask, પૂર્ણાંક len, પૂર્ણાંક cn) અચલ {
        જો (cn == 1) પાછા maskedNormDiffInfCn1<દશાંશ, દશાંશ>(s1, s2, mask, len);
        પાછા maskedNormDiffInfTail<દશાંશ, દશાંશ>(s1, s2, mask, len, cn, 0.0f);
    }
};

template<> સંરચના MaskedNormDiffL1_SIMD<uchar, પૂર્ણાંક> {
    પૂર્ણાંક operator()(અચલ uchar* s1, અચલ uchar* s2, અચલ uchar* mask, પૂર્ણાંક len, પૂર્ણાંક cn) અચલ {
        જો (cn == 1) પાછા maskedNormDiffL1_8(s1, s2, mask, len);
        જો (cn == 4) {
            પૂર્ણાંક i = 0; અચલ પૂર્ણાંક vstep = VTraits<v_uint8>::vlanes();
            અચલ v_uint8 one = vx_setall_u8(1);
            v_uint32 acc = vx_setzero_u32();
            માટે (; i <= len - vstep; i += vstep) {
                v_uint8 a0,a1,a2,a3,b0,b1,b2,b3;
                v_load_deinterleave(s1 + i*4, a0,a1,a2,a3);
                v_load_deinterleave(s2 + i*4, b0,b1,b2,b3);
                v_uint8 m = v_gt(vx_load(mask + i), vx_setzero_u8());
                acc = v_dotprod_expand_fast(v_and(v_absdiff(a0,b0), m), one, acc);
                acc = v_dotprod_expand_fast(v_and(v_absdiff(a1,b1), m), one, acc);
                acc = v_dotprod_expand_fast(v_and(v_absdiff(a2,b2), m), one, acc);
                acc = v_dotprod_expand_fast(v_and(v_absdiff(a3,b3), m), one, acc);
            }
            પૂર્ણાંક result = (પૂર્ણાંક)v_reduce_sum(acc);
            માટે (; i < len; i++) જો (mask[i]) {
                અચલ uchar* e1 = s1 + i*4; અચલ uchar* e2 = s2 + i*4;
                માટે (પૂર્ણાંક k = 0; k < 4; k++) result += (પૂર્ણાંક)cv_absdiff(e1[k], e2[k]);
            }
            પાછા result;
        }
        પાછા maskedNormDiffL1Tail<uchar, પૂર્ણાંક>(s1, s2, mask, len, cn, 0);
    }
};
template<> સંરચના MaskedNormDiffL1_SIMD<schar, પૂર્ણાંક> {
    પૂર્ણાંક operator()(અચલ schar* s1, અચલ schar* s2, અચલ uchar* mask, પૂર્ણાંક len, પૂર્ણાંક cn) અચલ {
        જો (cn == 1) પાછા maskedNormDiffL1_8(s1, s2, mask, len);
        પાછા maskedNormDiffL1Tail<schar, પૂર્ણાંક>(s1, s2, mask, len, cn, 0);
    }
};
template<> સંરચના MaskedNormDiffL1_SIMD<ushort, પૂર્ણાંક> {
    પૂર્ણાંક operator()(અચલ ushort* s1, અચલ ushort* s2, અચલ uchar* mask, પૂર્ણાંક len, પૂર્ણાંક cn) અચલ {
        જો (cn == 1) પાછા maskedNormDiffL1_16(s1, s2, mask, len);
        પાછા maskedNormDiffL1Tail<ushort, પૂર્ણાંક>(s1, s2, mask, len, cn, 0);
    }
};
template<> સંરચના MaskedNormDiffL1_SIMD<short, પૂર્ણાંક> {
    પૂર્ણાંક operator()(અચલ short* s1, અચલ short* s2, અચલ uchar* mask, પૂર્ણાંક len, પૂર્ણાંક cn) અચલ {
        જો (cn == 1) પાછા maskedNormDiffL1_16(s1, s2, mask, len);
        પાછા maskedNormDiffL1Tail<short, પૂર્ણાંક>(s1, s2, mask, len, cn, 0);
    }
};

template<> સંરચના MaskedNormDiffL2_SIMD<uchar, પૂર્ણાંક> {
    પૂર્ણાંક operator()(અચલ uchar* s1, અચલ uchar* s2, અચલ uchar* mask, પૂર્ણાંક len, પૂર્ણાંક cn) અચલ {
        જો (cn == 1) પાછા maskedNormDiffL2_8(s1, s2, mask, len);
        જો (cn == 4) {
            પૂર્ણાંક i = 0; અચલ પૂર્ણાંક vstep = VTraits<v_uint8>::vlanes();
            v_uint32 acc = vx_setzero_u32();
            માટે (; i <= len - vstep; i += vstep) {
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
            પૂર્ણાંક result = (પૂર્ણાંક)v_reduce_sum(acc);
            માટે (; i < len; i++) જો (mask[i]) {
                અચલ uchar* e1 = s1 + i*4; અચલ uchar* e2 = s2 + i*4;
                માટે (પૂર્ણાંક k = 0; k < 4; k++) { પૂર્ણાંક v = (પૂર્ણાંક)e1[k] - (પૂર્ણાંક)e2[k]; result += v*v; }
            }
            પાછા result;
        }
        પાછા maskedNormDiffL2Tail<uchar, પૂર્ણાંક>(s1, s2, mask, len, cn, 0);
    }
};
template<> સંરચના MaskedNormDiffL2_SIMD<schar, પૂર્ણાંક> {
    પૂર્ણાંક operator()(અચલ schar* s1, અચલ schar* s2, અચલ uchar* mask, પૂર્ણાંક len, પૂર્ણાંક cn) અચલ {
        જો (cn == 1) પાછા maskedNormDiffL2_8(s1, s2, mask, len);
        પાછા maskedNormDiffL2Tail<schar, પૂર્ણાંક>(s1, s2, mask, len, cn, 0);
    }
};

#જો CV_SIMD_64F
// Depths whose L1/L2 accumulate in double.
template<> સંરચના MaskedNormDiffL1_SIMD<પૂર્ણાંક, દ્વિગુણ> {
    દ્વિગુણ operator()(અચલ પૂર્ણાંક* s1, અચલ પૂર્ણાંક* s2, અચલ uchar* mask, પૂર્ણાંક len, પૂર્ણાંક cn) અચલ {
        જો (cn != 1) પાછા maskedNormDiffL1Tail<પૂર્ણાંક, દ્વિગુણ>(s1, s2, mask, len, cn, 0.0);
        પૂર્ણાંક i = 0; અચલ પૂર્ણાંક vstep = VTraits<v_int32>::vlanes();
        v_float64 acc = vx_setzero_f64();
        માટે (; i <= len - vstep; i += vstep) {
            v_uint32 m  = v_gt(vx_load_expand_q(mask + i), vx_setzero_u32());
            v_uint32 ad = v_and(v_absdiff(vx_load(s1 + i), vx_load(s2 + i)), m);
            v_uint64 e0, e1; v_expand(ad, e0, e1);
            acc = v_add(acc, v_cvt_f64(v_reinterpret_as_s64(e0)));
            acc = v_add(acc, v_cvt_f64(v_reinterpret_as_s64(e1)));
        }
        દ્વિગુણ result = v_reduce_sum(acc);
        માટે (; i < len; i++) જો (mask[i]) result += (દ્વિગુણ)cv_absdiff(s1[i], s2[i]);
        પાછા result;
    }
};
template<> સંરચના MaskedNormDiffL1_SIMD<દશાંશ, દ્વિગુણ> {
    દ્વિગુણ operator()(અચલ દશાંશ* s1, અચલ દશાંશ* s2, અચલ uchar* mask, પૂર્ણાંક len, પૂર્ણાંક cn) અચલ {
        જો (cn != 1) પાછા maskedNormDiffL1Tail<દશાંશ, દ્વિગુણ>(s1, s2, mask, len, cn, 0.0);
        પૂર્ણાંક i = 0; અચલ પૂર્ણાંક vstep = VTraits<v_float32>::vlanes();
        v_float64 acc0 = vx_setzero_f64(), acc1 = vx_setzero_f64();
        માટે (; i <= len - vstep; i += vstep) {
            v_float32 m  = v_reinterpret_as_f32(v_gt(vx_load_expand_q(mask + i), vx_setzero_u32()));
            v_float32 ad = v_and(v_absdiff(vx_load(s1 + i), vx_load(s2 + i)), m);
            acc0 = v_add(acc0, v_cvt_f64(ad));
            acc1 = v_add(acc1, v_cvt_f64_ઉપલું(ad));
        }
        દ્વિગુણ result = v_reduce_sum(v_add(acc0, acc1));
        માટે (; i < len; i++) જો (mask[i]) result += (દ્વિગુણ)cv_absdiff(s1[i], s2[i]);
        પાછા result;
    }
};

template<typename T>
સ્થિર અંતઃરેખ દ્વિગુણ maskedNormDiffL2_16(અચલ T* s1, અચલ T* s2, અચલ uchar* mask, પૂર્ણાંક len, પૂર્ણાંક cn) {
    જો (cn != 1) પાછા maskedNormDiffL2Tail<T, દ્વિગુણ>(s1, s2, mask, len, cn, 0.0);
    પૂર્ણાંક i = 0; અચલ પૂર્ણાંક vstep = VTraits<v_uint16>::vlanes();
    v_float64 acc = vx_setzero_f64();
    માટે (; i <= len - vstep; i += vstep) {
        v_uint16 m  = v_gt(vx_load_expand(mask + i), vx_setzero_u16());
        v_uint16 ad = v_and(v_absdiff(vx_load(s1 + i), vx_load(s2 + i)), m);
        v_uint64 u = v_dotprod_expand_fast(ad, ad);
        acc = v_add(acc, v_cvt_f64(v_reinterpret_as_s64(u)));
    }
    દ્વિગુણ result = v_reduce_sum(acc);
    માટે (; i < len; i++) જો (mask[i]) { દ્વિગુણ v = (દ્વિગુણ)s1[i] - (દ્વિગુણ)s2[i]; result += v*v; }
    પાછા result;
}
template<> સંરચના MaskedNormDiffL2_SIMD<ushort, દ્વિગુણ> {
    દ્વિગુણ operator()(અચલ ushort* s1, અચલ ushort* s2, અચલ uchar* mask, પૂર્ણાંક len, પૂર્ણાંક cn) અચલ
    { પાછા maskedNormDiffL2_16(s1, s2, mask, len, cn); } };
template<> સંરચના MaskedNormDiffL2_SIMD<short, દ્વિગુણ> {
    દ્વિગુણ operator()(અચલ short* s1, અચલ short* s2, અચલ uchar* mask, પૂર્ણાંક len, પૂર્ણાંક cn) અચલ
    { પાછા maskedNormDiffL2_16(s1, s2, mask, len, cn); } };

template<> સંરચના MaskedNormDiffL2_SIMD<પૂર્ણાંક, દ્વિગુણ> {
    દ્વિગુણ operator()(અચલ પૂર્ણાંક* s1, અચલ પૂર્ણાંક* s2, અચલ uchar* mask, પૂર્ણાંક len, પૂર્ણાંક cn) અચલ {
        જો (cn != 1) પાછા maskedNormDiffL2Tail<પૂર્ણાંક, દ્વિગુણ>(s1, s2, mask, len, cn, 0.0);
        પૂર્ણાંક i = 0; અચલ પૂર્ણાંક vstep = VTraits<v_int32>::vlanes();
        v_float64 r0 = vx_setzero_f64(), r1 = vx_setzero_f64();
        માટે (; i <= len - vstep; i += vstep) {
            v_uint32 m  = v_gt(vx_load_expand_q(mask + i), vx_setzero_u32());
            v_uint32 ad = v_and(v_absdiff(vx_load(s1 + i), vx_load(s2 + i)), m);
            v_uint64 e0, e1; v_expand(ad, e0, e1);
            v_float64 f0 = v_cvt_f64(v_reinterpret_as_s64(e0)), f1 = v_cvt_f64(v_reinterpret_as_s64(e1));
            r0 = v_fma(f0, f0, r0); r1 = v_fma(f1, f1, r1);
        }
        દ્વિગુણ result = v_reduce_sum(v_add(r0, r1));
        માટે (; i < len; i++) જો (mask[i]) { દ્વિગુણ v = (દ્વિગુણ)s1[i] - (દ્વિગુણ)s2[i]; result += v*v; }
        પાછા result;
    }
};
template<> સંરચના MaskedNormDiffL2_SIMD<દશાંશ, દ્વિગુણ> {
    દ્વિગુણ operator()(અચલ દશાંશ* s1, અચલ દશાંશ* s2, અચલ uchar* mask, પૂર્ણાંક len, પૂર્ણાંક cn) અચલ {
        જો (cn != 1) પાછા maskedNormDiffL2Tail<દશાંશ, દ્વિગુણ>(s1, s2, mask, len, cn, 0.0);
        પૂર્ણાંક i = 0; અચલ પૂર્ણાંક vstep = VTraits<v_float32>::vlanes();
        v_float64 r0 = vx_setzero_f64(), r1 = vx_setzero_f64();
        માટે (; i <= len - vstep; i += vstep) {
            v_float32 m  = v_reinterpret_as_f32(v_gt(vx_load_expand_q(mask + i), vx_setzero_u32()));
            v_float32 ad = v_and(v_absdiff(vx_load(s1 + i), vx_load(s2 + i)), m);
            v_float64 f0 = v_cvt_f64(ad), f1 = v_cvt_f64_ઉપલું(ad);
            r0 = v_fma(f0, f0, r0); r1 = v_fma(f1, f1, r1);
        }
        દ્વિગુણ result = v_reduce_sum(v_add(r0, r1));
        માટે (; i < len; i++) જો (mask[i]) { દ્વિગુણ v = (દ્વિગુણ)s1[i] - (દ્વિગુણ)s2[i]; result += v*v; }
        પાછા result;
    }
};
template<>
સંરચના MaskedNormDiffInf_SIMD<દ્વિગુણ, દ્વિગુણ> {
    અંતઃરેખ દ્વિગુણ operator()(અચલ દ્વિગુણ* s1, અચલ દ્વિગુણ* s2, અચલ uchar* mask, પૂર્ણાંક len, પૂર્ણાંક cn) અચલ {
        દ્વિગુણ result = 0.0;
        પૂર્ણાંક i = 0;
#જો CV_SIMD_64F || CV_SIMD_SCALABLE_64F
        જો (cn == 1) {
            અચલ પૂર્ણાંક vstep = VTraits<v_float64>::vlanes();
            v_float64 acc0 = vx_setzero_f64(), acc1 = vx_setzero_f64();
            માટે (; i <= len - 2*vstep; i += 2*vstep) {
                v_uint64 m0, m1;
                v_normmask_u64(mask + i, m0, m1);
                v_float64 d0 = v_absdiff(vx_load(s1 + i), vx_load(s2 + i));
                v_float64 d1 = v_absdiff(vx_load(s1 + i + vstep), vx_load(s2 + i + vstep));
                acc0 = v_max(acc0, v_reinterpret_as_f64(v_and(v_reinterpret_as_u64(d0), m0)));
                acc1 = v_max(acc1, v_reinterpret_as_f64(v_and(v_reinterpret_as_u64(d1), m1)));
            }
            દ્વિગુણ t[VTraits<v_float64>::max_nlanes];
            vx_store(t, v_max(acc0, acc1));
            માટે (પૂર્ણાંક k = 0; k < vstep; k++)
                result = std::max(result, t[k]);
        }
#endif
        માટે (; i < len; i++) જો (mask[i]) {
            અચલ દ્વિગુણ* e1 = s1 + i*cn; અચલ દ્વિગુણ* e2 = s2 + i*cn;
            માટે (પૂર્ણાંક k = 0; k < cn; k++) result = std::max(result, std::abs(e1[k] - e2[k]));
        }
        પાછા result;
    }
};

template<>
સંરચના MaskedNormDiffL1_SIMD<દ્વિગુણ, દ્વિગુણ> {
    દ્વિગુણ operator()(અચલ દ્વિગુણ* s1, અચલ દ્વિગુણ* s2, અચલ uchar* mask, પૂર્ણાંક len, પૂર્ણાંક cn) અચલ {
        દ્વિગુણ result = 0.0;
        જો (cn == 1) {
            અચલ પૂર્ણાંક vstep = VTraits<v_float64>::vlanes();
            પૂર્ણાંક i = 0;
            v_float64 acc0 = vx_setzero_f64(), acc1 = vx_setzero_f64();
            માટે (; i <= len - 2*vstep; i += 2*vstep) {
                v_uint64 m0, m1;
                v_normmask_u64(mask + i, m0, m1);
                v_float64 d0 = v_absdiff(vx_load(s1 + i), vx_load(s2 + i));
                v_float64 d1 = v_absdiff(vx_load(s1 + i + vstep), vx_load(s2 + i + vstep));
                acc0 = v_add(acc0, v_reinterpret_as_f64(v_and(v_reinterpret_as_u64(d0), m0)));
                acc1 = v_add(acc1, v_reinterpret_as_f64(v_and(v_reinterpret_as_u64(d1), m1)));
            }
            result = v_reduce_sum(v_add(acc0, acc1));
            માટે (; i < len; i++) જો (mask[i]) result += std::abs(s1[i] - s2[i]);
        } નહીંતર {
            result = maskedNormDiffL1Tail<દ્વિગુણ, દ્વિગુણ>(s1, s2, mask, len, cn, 0.0);
        }
        પાછા result;
    }
};

template<>
સંરચના MaskedNormDiffL2_SIMD<દ્વિગુણ, દ્વિગુણ> {
    દ્વિગુણ operator()(અચલ દ્વિગુણ* s1, અચલ દ્વિગુણ* s2, અચલ uchar* mask, પૂર્ણાંક len, પૂર્ણાંક cn) અચલ {
        દ્વિગુણ result = 0.0;
        જો (cn == 1) {
            અચલ પૂર્ણાંક vstep = VTraits<v_float64>::vlanes();
            પૂર્ણાંક i = 0;
            v_float64 acc0 = vx_setzero_f64(), acc1 = vx_setzero_f64();
            માટે (; i <= len - 2*vstep; i += 2*vstep) {
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
            માટે (; i < len; i++) જો (mask[i]) { દ્વિગુણ v = s1[i] - s2[i]; result += v*v; }
        } નહીંતર {
            result = maskedNormDiffL2Tail<દ્વિગુણ, દ્વિગુણ>(s1, s2, mask, len, cn, 0.0);
        }
        પાછા result;
    }
};
#endif // CV_SIMD_64F
#endif // CV_SIMD

template<typename T, typename ST> પૂર્ણાંક
normDiffInf_(અચલ T* src1, અચલ T* src2, અચલ uchar* mask, ST* _result, પૂર્ણાંક len, પૂર્ણાંક cn) {
    ST result = *_result;
    જો( !mask ) {
        NormDiffInf_SIMD<T, ST> op;
        result = std::max(result, op(src1, src2, len*cn));
    } નહીંતર {
        MaskedNormDiffInf_SIMD<T, ST> op;
        result = std::max(result, op(src1, src2, mask, len, cn));
    }
    *_result = result;
    પાછા 0;
}

template<typename T, typename ST> પૂર્ણાંક
normDiffL1_(અચલ T* src1, અચલ T* src2, અચલ uchar* mask, ST* _result, પૂર્ણાંક len, પૂર્ણાંક cn) {
    ST result = *_result;
    જો( !mask ) {
        NormDiffL1_SIMD<T, ST> op;
        result += op(src1, src2, len*cn);
    } નહીંતર {
        MaskedNormDiffL1_SIMD<T, ST> op;
        result += op(src1, src2, mask, len, cn);
    }
    *_result = result;
    પાછા 0;
}

template<typename T, typename ST> પૂર્ણાંક
normDiffL2_(અચલ T* src1, અચલ T* src2, અચલ uchar* mask, ST* _result, પૂર્ણાંક len, પૂર્ણાંક cn) {
    ST result = *_result;
    જો( !mask ) {
        NormDiffL2_SIMD<T, ST> op;
        result += op(src1, src2, len*cn);
    } નહીંતર {
        MaskedNormDiffL2_SIMD<T, ST> op;
        result += op(src1, src2, mask, len, cn);
    }
    *_result = result;
    પાછા 0;
}

સ્થિર પૂર્ણાંક
normDiffInf_Bool(અચલ uchar* src1, અચલ uchar* src2, અચલ uchar* mask, પૂર્ણાંક* _result, પૂર્ણાંક len, પૂર્ણાંક cn)
{
    પૂર્ણાંક result = *_result;
    જો( !mask )
    {
        માટે( પૂર્ણાંક i = 0; i < len*cn; i++ ) {
            result = std::max(result, (પૂર્ણાંક)((src1[i] != 0) != (src2[i] != 0)));
            જો (result != 0)
                તોડો;
        }
    }
    નહીંતર
    {
        માટે( પૂર્ણાંક i = 0; i < len; i++, src1 += cn, src2 += cn )
            જો( mask[i] )
            {
                માટે( પૂર્ણાંક k = 0; k < cn; k++ )
                    result = std::max(result, (પૂર્ણાંક)((src1[k] != 0) != (src2[k] != 0)));
                જો (result != 0)
                    તોડો;
            }
    }
    *_result = result;
    પાછા 0;
}

સ્થિર પૂર્ણાંક
normDiffL1_Bool(અચલ uchar* src1, અચલ uchar* src2, અચલ uchar* mask, પૂર્ણાંક* _result, પૂર્ણાંક len, પૂર્ણાંક cn)
{
    પૂર્ણાંક result = *_result;
    જો( !mask )
    {
        માટે( પૂર્ણાંક i = 0; i < len*cn; i++ )
            result += (પૂર્ણાંક)((src1[i] != 0) != (src2[i] != 0));
    }
    નહીંતર
    {
        માટે( પૂર્ણાંક i = 0; i < len; i++, src1 += cn, src2 += cn )
            જો( mask[i] )
            {
                માટે( પૂર્ણાંક k = 0; k < cn; k++ )
                    result += (પૂર્ણાંક)((src1[k] != 0) != (src2[k] != 0));
            }
    }
    *_result = result;
    પાછા 0;
}

સ્થિર પૂર્ણાંક
normDiffL2_Bool(અચલ uchar* src1, અચલ uchar* src2, અચલ uchar* mask, પૂર્ણાંક* _result, પૂર્ણાંક len, પૂર્ણાંક cn)
{
    પાછા normDiffL1_Bool(src1, src2, mask, _result, len, cn);
}

#define CV_DEF_NORM_FUNC(L, suffix, type, ntype) \
    સ્થિર પૂર્ણાંક norm##L##_##suffix(અચલ type* src, અચલ uchar* mask, ntype* r, પૂર્ણાંક len, પૂર્ણાંક cn) \
{ CV_INSTRUMENT_REGION(); પાછા norm##L##_(src, mask, r, len, cn); } \
    સ્થિર પૂર્ણાંક normDiff##L##_##suffix(અચલ type* src1, અચલ type* src2, \
    અચલ uchar* mask, ntype* r, પૂર્ણાંક len, પૂર્ણાંક cn) \
{ પાછા normDiff##L##_(src1, src2, mask, r, (પૂર્ણાંક)len, cn); }

#define CV_DEF_NORM_ALL(suffix, type, inftype, l1type, l2type) \
    CV_DEF_NORM_FUNC(Inf, suffix, type, inftype) \
    CV_DEF_NORM_FUNC(L1, suffix, type, l1type) \
    CV_DEF_NORM_FUNC(L2, suffix, type, l2type)

CV_DEF_NORM_ALL(8u, uchar, પૂર્ણાંક, પૂર્ણાંક, પૂર્ણાંક)
CV_DEF_NORM_ALL(8s, schar, પૂર્ણાંક, પૂર્ણાંક, પૂર્ણાંક)
CV_DEF_NORM_ALL(16u, ushort, પૂર્ણાંક, પૂર્ણાંક, દ્વિગુણ)
CV_DEF_NORM_ALL(16s, short, પૂર્ણાંક, પૂર્ણાંક, દ્વિગુણ)
CV_DEF_NORM_ALL(32u, unsigned, unsigned, દ્વિગુણ, દ્વિગુણ)
CV_DEF_NORM_ALL(32s, પૂર્ણાંક, unsigned, દ્વિગુણ, દ્વિગુણ)
CV_DEF_NORM_ALL(32f, દશાંશ, દશાંશ, દ્વિગુણ, દ્વિગુણ)
CV_DEF_NORM_ALL(64f, દ્વિગુણ, દ્વિગુણ, દ્વિગુણ, દ્વિગુણ)
CV_DEF_NORM_ALL(64u, uint64, uint64, દ્વિગુણ, દ્વિગુણ)
CV_DEF_NORM_ALL(64s, int64, uint64, દ્વિગુણ, દ્વિગુણ)
CV_DEF_NORM_ALL(16f, hfloat, દશાંશ, દશાંશ, દશાંશ)
CV_DEF_NORM_ALL(16bf, bfloat, દશાંશ, દશાંશ, દશાંશ)

NormFunc getNormFunc(પૂર્ણાંક normType, પૂર્ણાંક depth)
{
    CV_INSTRUMENT_REGION();

    સ્થિર NormFunc normTab[3][CV_DEPTH_MAX] =
    {
        {
            normWrap<uchar, પૂર્ણાંક, normInf_8u>,
            normWrap<schar, પૂર્ણાંક, normInf_8s>,
            normWrap<ushort, પૂર્ણાંક, normInf_16u>,
            normWrap<short, પૂર્ણાંક, normInf_16s>,
            normWrap<પૂર્ણાંક, unsigned, normInf_32s>,
            normWrap<દશાંશ, દશાંશ, normInf_32f>,
            normWrap<દ્વિગુણ, દ્વિગુણ, normInf_64f>,
            normWrap<hfloat, દશાંશ, normInf_16f>,
            normWrap<bfloat, દશાંશ, normInf_16bf>,
            normWrap<uchar, પૂર્ણાંક, normInf_Bool>,
            normWrap<uint64, uint64, normInf_64u>,
            normWrap<int64, uint64, normInf_64s>,
            normWrap<unsigned, unsigned, normInf_32u>,
            0
        },
        {
            normWrap<uchar, પૂર્ણાંક, normL1_8u>,
            normWrap<schar, પૂર્ણાંક, normL1_8s>,
            normWrap<ushort, પૂર્ણાંક, normL1_16u>,
            normWrap<short, પૂર્ણાંક, normL1_16s>,
            normWrap<પૂર્ણાંક, દ્વિગુણ, normL1_32s>,
            normWrap<દશાંશ, દ્વિગુણ, normL1_32f>,
            normWrap<દ્વિગુણ, દ્વિગુણ, normL1_64f>,
            normWrap<hfloat, દશાંશ, normL1_16f>,
            normWrap<bfloat, દશાંશ, normL1_16bf>,
            normWrap<uchar, પૂર્ણાંક, normL1_Bool>,
            normWrap<uint64, દ્વિગુણ, normL1_64u>,
            normWrap<int64, દ્વિગુણ, normL1_64s>,
            normWrap<unsigned, દ્વિગુણ, normL1_32u>,
            0
        },
        {
            normWrap<uchar, પૂર્ણાંક, normL2_8u>,
            normWrap<schar, પૂર્ણાંક, normL2_8s>,
            normWrap<ushort, દ્વિગુણ, normL2_16u>,
            normWrap<short, દ્વિગુણ, normL2_16s>,
            normWrap<પૂર્ણાંક, દ્વિગુણ, normL2_32s>,
            normWrap<દશાંશ, દ્વિગુણ, normL2_32f>,
            normWrap<દ્વિગુણ, દ્વિગુણ, normL2_64f>,
            normWrap<hfloat, દશાંશ, normL2_16f>,
            normWrap<bfloat, દશાંશ, normL2_16bf>,
            normWrap<uchar, પૂર્ણાંક, normL2_Bool>,
            normWrap<uint64, દ્વિગુણ, normL2_64u>,
            normWrap<int64, દ્વિગુણ, normL2_64s>,
            normWrap<unsigned, દ્વિગુણ, normL2_32u>,
            0
        }
    };

    જો (normType >= 3 || normType < 0) પાછા nullptr;

    પાછા normTab[normType][depth];
}

NormDiffFunc getNormDiffFunc(પૂર્ણાંક normType, પૂર્ણાંક depth)
{
    સ્થિર NormDiffFunc normDiffTab[3][CV_DEPTH_MAX] =
    {
        {
            normDiffWrap<uchar, પૂર્ણાંક, normDiffInf_8u>,
            normDiffWrap<schar, પૂર્ણાંક, normDiffInf_8s>,
            normDiffWrap<ushort, પૂર્ણાંક, normDiffInf_16u>,
            normDiffWrap<short, પૂર્ણાંક, normDiffInf_16s>,
            normDiffWrap<પૂર્ણાંક, unsigned, normDiffInf_32s>,
            normDiffWrap<દશાંશ, દશાંશ, normDiffInf_32f>,
            normDiffWrap<દ્વિગુણ, દ્વિગુણ, normDiffInf_64f>,
            normDiffWrap<hfloat, દશાંશ, normDiffInf_16f>,
            normDiffWrap<bfloat, દશાંશ, normDiffInf_16bf>,
            normDiffWrap<uchar, પૂર્ણાંક, normDiffInf_Bool>,
            normDiffWrap<uint64, uint64, normDiffInf_64u>,
            normDiffWrap<int64, uint64, normDiffInf_64s>,
            normDiffWrap<unsigned, unsigned, normDiffInf_32u>,
            0
        },
        {
            normDiffWrap<uchar, પૂર્ણાંક, normDiffL1_8u>,
            normDiffWrap<schar, પૂર્ણાંક, normDiffL1_8s>,
            normDiffWrap<ushort, પૂર્ણાંક, normDiffL1_16u>,
            normDiffWrap<short, પૂર્ણાંક, normDiffL1_16s>,
            normDiffWrap<પૂર્ણાંક, દ્વિગુણ, normDiffL1_32s>,
            normDiffWrap<દશાંશ, દ્વિગુણ, normDiffL1_32f>,
            normDiffWrap<દ્વિગુણ, દ્વિગુણ, normDiffL1_64f>,
            normDiffWrap<hfloat, દશાંશ, normDiffL1_16f>,
            normDiffWrap<bfloat, દશાંશ, normDiffL1_16bf>,
            normDiffWrap<uchar, પૂર્ણાંક, normDiffL1_Bool>,
            normDiffWrap<uint64, દ્વિગુણ, normDiffL1_64u>,
            normDiffWrap<int64, દ્વિગુણ, normDiffL1_64s>,
            normDiffWrap<unsigned, દ્વિગુણ, normDiffL1_32u>,
            0
        },
        {
            normDiffWrap<uchar, પૂર્ણાંક, normDiffL2_8u>,
            normDiffWrap<schar, પૂર્ણાંક, normDiffL2_8s>,
            normDiffWrap<ushort, દ્વિગુણ, normDiffL2_16u>,
            normDiffWrap<short, દ્વિગુણ, normDiffL2_16s>,
            normDiffWrap<પૂર્ણાંક, દ્વિગુણ, normDiffL2_32s>,
            normDiffWrap<દશાંશ, દ્વિગુણ, normDiffL2_32f>,
            normDiffWrap<દ્વિગુણ, દ્વિગુણ, normDiffL2_64f>,
            normDiffWrap<hfloat, દશાંશ, normDiffL2_16f>,
            normDiffWrap<bfloat, દશાંશ, normDiffL2_16bf>,
            normDiffWrap<uchar, પૂર્ણાંક, normDiffL2_Bool>,
            normDiffWrap<uint64, દ્વિગુણ, normDiffL2_64u>,
            normDiffWrap<int64, દ્વિગુણ, normDiffL2_64s>,
            normDiffWrap<unsigned, દ્વિગુણ, normDiffL2_32u>,
            0
        },
    };
    જો (normType >= 3 || normType < 0) પાછા nullptr;

    પાછા normDiffTab[normType][depth];
}

#endif // CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

CV_CPU_OPTIMIZATION_NAMESPACE_END

} // cv::
