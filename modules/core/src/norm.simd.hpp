// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html
//
// Copyright (C) 2025, SpaceMIT Inc., all rights reserved.
// Copyright (C) 2026, Advanced Micro Devices, Inc., all rights reserved.
// Third party copyrights are property of their respective owners.

#ଅନ୍ତର୍ଭାଵ "precomp.hpp"

ନାମାକାଶ cv {

using NormFunc = ପୂର୍ଣ୍ଣସଂଖ୍ୟା (*)(ଅଚଳ uchar*, ଅଚଳ uchar*, uchar*, ପୂର୍ଣ୍ଣସଂଖ୍ୟା, ପୂର୍ଣ୍ଣସଂଖ୍ୟା);
using NormDiffFunc = ପୂର୍ଣ୍ଣସଂଖ୍ୟା (*)(ଅଚଳ uchar*, ଅଚଳ uchar*, ଅଚଳ uchar*, uchar*, ପୂର୍ଣ୍ଣସଂଖ୍ୟା, ପୂର୍ଣ୍ଣସଂଖ୍ୟା);

CV_CPU_OPTIMIZATION_NAMESPACE_BEGIN

template<typename T, typename RT, ପୂର୍ଣ୍ଣସଂଖ୍ୟା (*fn)(ଅଚଳ T*, ଅଚଳ uchar*, RT*, ପୂର୍ଣ୍ଣସଂଖ୍ୟା, ପୂର୍ଣ୍ଣସଂଖ୍ୟା)>
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା normWrap(ଅଚଳ uchar* src1, ଅଚଳ uchar* src2, uchar* res, ପୂର୍ଣ୍ଣସଂଖ୍ୟା len, ପୂର୍ଣ୍ଣସଂଖ୍ୟା cn)
{
    ଫେରନ୍ତୁ fn((ଅଚଳ T*)src1, src2, (RT*)res, len, cn);
}

template<typename T, typename RT, ପୂର୍ଣ୍ଣସଂଖ୍ୟା (*fn)(ଅଚଳ T*, ଅଚଳ T*, ଅଚଳ uchar*, RT*, ପୂର୍ଣ୍ଣସଂଖ୍ୟା, ପୂର୍ଣ୍ଣସଂଖ୍ୟା)>
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା normDiffWrap(ଅଚଳ uchar* src1, ଅଚଳ uchar* src2, ଅଚଳ uchar* src3, uchar* res, ପୂର୍ଣ୍ଣସଂଖ୍ୟା len, ପୂର୍ଣ୍ଣସଂଖ୍ୟା cn)
{
    ଫେରନ୍ତୁ fn((ଅଚଳ T*)src1, (ଅଚଳ T*)src2, src3, (RT*)res, len, cn);
}

NormFunc getNormFunc(ପୂର୍ଣ୍ଣସଂଖ୍ୟା normType, ପୂର୍ଣ୍ଣସଂଖ୍ୟା depth);
NormDiffFunc getNormDiffFunc(ପୂର୍ଣ୍ଣସଂଖ୍ୟା normType, ପୂର୍ଣ୍ଣସଂଖ୍ୟା depth);

#ifndef CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

template <typename T, typename ST>
ସଂରଚନା NormInf_SIMD {
    ଧାଡ଼ିଭିତରେ ST operator() (ଅଚଳ T* src, ପୂର୍ଣ୍ଣସଂଖ୍ୟା n) ଅଚଳ {
        ST s = 0;
        ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < n; i++) {
            s = std::max(s, (ST)cv_abs(src[i]));
        }
        ଫେରନ୍ତୁ s;
    }
};

template <typename T, typename ST>
ସଂରଚନା NormL1_SIMD {
    ଧାଡ଼ିଭିତରେ ST operator() (ଅଚଳ T* src, ପୂର୍ଣ୍ଣସଂଖ୍ୟା n) ଅଚଳ {
        ST s = 0;
        ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < n; i++) {
            s += cv_abs(src[i]);
        }
        ଫେରନ୍ତୁ s;
    }
};

template <typename T, typename ST>
ସଂରଚନା NormL2_SIMD {
    ଧାଡ଼ିଭିତରେ ST operator() (ଅଚଳ T* src, ପୂର୍ଣ୍ଣସଂଖ୍ୟା n) ଅଚଳ {
        ST s = 0;
        ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < n; i++) {
            ST v = (ST)src[i];
            s += v * v;
        }
        ଫେରନ୍ତୁ s;
    }
};

template <typename T, typename ST>
ସଂରଚନା NormDiffInf_SIMD {
    ଧାଡ଼ିଭିତରେ ST operator() (ଅଚଳ T* src1, ଅଚଳ T* src2, ପୂର୍ଣ୍ଣସଂଖ୍ୟା n) ଅଚଳ {
        ST s = 0;
        ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < n; i++) {
            ST v = (ST)cv_absdiff(src1[i], src2[i]);
            s = std::max(s, v);
        }
        ଫେରନ୍ତୁ s;
    }
};

template <typename T, typename ST>
ସଂରଚନା NormDiffL1_SIMD {
    ଧାଡ଼ିଭିତରେ ST operator() (ଅଚଳ T* src1, ଅଚଳ T* src2, ପୂର୍ଣ୍ଣସଂଖ୍ୟା n) ଅଚଳ {
        ST s = 0;
        ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < n; i++) {
            ST v = (ST)cv_absdiff(src1[i], src2[i]);
            s += v;
        }
        ଫେରନ୍ତୁ s;
    }
};

template <typename T, typename ST>
ସଂରଚନା NormDiffL2_SIMD {
    ଧାଡ଼ିଭିତରେ ST operator() (ଅଚଳ T* src1, ଅଚଳ T* src2, ପୂର୍ଣ୍ଣସଂଖ୍ୟା n) ଅଚଳ {
        ST s = 0;
        ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < n; i++) {
            ST v = (ST)src1[i] - (ST)src2[i];
            s += v * v;
        }
        ଫେରନ୍ତୁ s;
    }
};

#ଯଦି (CV_SIMD || CV_SIMD_SCALABLE)

template<>
ସଂରଚନା NormInf_SIMD<uchar, ପୂର୍ଣ୍ଣସଂଖ୍ୟା> {
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା operator() (ଅଚଳ uchar* src, ପୂର୍ଣ୍ଣସଂଖ୍ୟା n) ଅଚଳ {
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା j = 0;
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା s = 0;
        v_uint8 r0 = vx_setzero_u8(), r1 = vx_setzero_u8();
        v_uint8 r2 = vx_setzero_u8(), r3 = vx_setzero_u8();
        ପାଇଁ (; j <= n - 4 * VTraits<v_uint8>::vlanes(); j += 4 * VTraits<v_uint8>::vlanes()) {
            r0 = v_max(r0, vx_load(src + j                                 ));
            r1 = v_max(r1, vx_load(src + j +     VTraits<v_uint8>::vlanes()));
            r2 = v_max(r2, vx_load(src + j + 2 * VTraits<v_uint8>::vlanes()));
            r3 = v_max(r3, vx_load(src + j + 3 * VTraits<v_uint8>::vlanes()));
        }
        r0 = v_max(r0, v_max(r1, v_max(r2, r3)));
        ପାଇଁ (; j < n; j++) {
            s = std::max(s, (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)src[j]);
        }
        ଫେରନ୍ତୁ std::max(s, (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)v_reduce_max(r0));
    }
};

template<>
ସଂରଚନା NormInf_SIMD<schar, ପୂର୍ଣ୍ଣସଂଖ୍ୟା> {
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା operator() (ଅଚଳ schar* src, ପୂର୍ଣ୍ଣସଂଖ୍ୟା n) ଅଚଳ {
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା j = 0;
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା s = 0;
        v_uint8 r0 = vx_setzero_u8(), r1 = vx_setzero_u8();
        v_uint8 r2 = vx_setzero_u8(), r3 = vx_setzero_u8();
        ପାଇଁ (; j <= n - 4 * VTraits<v_int8>::vlanes(); j += 4 * VTraits<v_int8>::vlanes()) {
            r0 = v_max(r0, v_abs(vx_load(src + j                                )));
            r1 = v_max(r1, v_abs(vx_load(src + j +     VTraits<v_int8>::vlanes())));
            r2 = v_max(r2, v_abs(vx_load(src + j + 2 * VTraits<v_int8>::vlanes())));
            r3 = v_max(r3, v_abs(vx_load(src + j + 3 * VTraits<v_int8>::vlanes())));
        }
        r0 = v_max(r0, v_max(r1, v_max(r2, r3)));
        ପାଇଁ (; j < n; j++) {
            s = std::max(s, cv_abs(src[j]));
        }
        ଫେରନ୍ତୁ std::max(s, saturate_cast<ପୂର୍ଣ୍ଣସଂଖ୍ୟା>(v_reduce_max(r0)));
    }
};

template<>
ସଂରଚନା NormInf_SIMD<ushort, ପୂର୍ଣ୍ଣସଂଖ୍ୟା> {
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା operator() (ଅଚଳ ushort* src, ପୂର୍ଣ୍ଣସଂଖ୍ୟା n) ଅଚଳ {
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା j = 0;
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା s = 0;
        v_uint16 d0 = vx_setzero_u16(), d1 = vx_setzero_u16();
        v_uint16 d2 = vx_setzero_u16(), d3 = vx_setzero_u16();
        ପାଇଁ (; j <= n - 4 * VTraits<v_uint16>::vlanes(); j += 4 * VTraits<v_uint16>::vlanes()) {
            d0 = v_max(d0, vx_load(src + j                                  ));
            d1 = v_max(d1, vx_load(src + j +     VTraits<v_uint16>::vlanes()));
            d2 = v_max(d2, vx_load(src + j + 2 * VTraits<v_uint16>::vlanes()));
            d3 = v_max(d3, vx_load(src + j + 3 * VTraits<v_uint16>::vlanes()));
        }
        d0 = v_max(d0, v_max(d1, v_max(d2, d3)));
        ପାଇଁ (; j < n; j++) {
            s = std::max(s, (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)src[j]);
        }
        ଫେରନ୍ତୁ std::max(s, (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)v_reduce_max(d0));
    }
};

template<>
ସଂରଚନା NormInf_SIMD<short, ପୂର୍ଣ୍ଣସଂଖ୍ୟା> {
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା operator() (ଅଚଳ short* src, ପୂର୍ଣ୍ଣସଂଖ୍ୟା n) ଅଚଳ {
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା j = 0;
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା s = 0;
        v_uint16 d0 = vx_setzero_u16(), d1 = vx_setzero_u16();
        v_uint16 d2 = vx_setzero_u16(), d3 = vx_setzero_u16();
        ପାଇଁ (; j <= n - 4 * VTraits<v_int16>::vlanes(); j += 4 * VTraits<v_int16>::vlanes()) {
            d0 = v_max(d0, v_abs(vx_load(src + j                                  )));
            d1 = v_max(d1, v_abs(vx_load(src + j +     VTraits<v_int16>::vlanes())));
            d2 = v_max(d2, v_abs(vx_load(src + j + 2 * VTraits<v_int16>::vlanes())));
            d3 = v_max(d3, v_abs(vx_load(src + j + 3 * VTraits<v_int16>::vlanes())));
        }
        d0 = v_max(d0, v_max(d1, v_max(d2, d3)));
        ପାଇଁ (; j < n; j++) {
            s = std::max(s, saturate_cast<ପୂର୍ଣ୍ଣସଂଖ୍ୟା>(cv_abs(src[j])));
        }
        ଫେରନ୍ତୁ std::max(s, saturate_cast<ପୂର୍ଣ୍ଣସଂଖ୍ୟା>(v_reduce_max(d0)));
    }
};

template<>
ସଂରଚନା NormInf_SIMD<ପୂର୍ଣ୍ଣସଂଖ୍ୟା, ପୂର୍ଣ୍ଣସଂଖ୍ୟା> {
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା operator() (ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା* src, ପୂର୍ଣ୍ଣସଂଖ୍ୟା n) ଅଚଳ {
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା j = 0;
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା s = 0;
        v_uint32 r0 = vx_setzero_u32(), r1 = vx_setzero_u32();
        v_uint32 r2 = vx_setzero_u32(), r3 = vx_setzero_u32();
        ପାଇଁ (; j <= n - 4 * VTraits<v_int32>::vlanes(); j += 4 * VTraits<v_int32>::vlanes()) {
            r0 = v_max(r0, v_abs(vx_load(src + j                                 )));
            r1 = v_max(r1, v_abs(vx_load(src + j +     VTraits<v_int32>::vlanes())));
            r2 = v_max(r2, v_abs(vx_load(src + j + 2 * VTraits<v_int32>::vlanes())));
            r3 = v_max(r3, v_abs(vx_load(src + j + 3 * VTraits<v_int32>::vlanes())));
        }
        r0 = v_max(r0, v_max(r1, v_max(r2, r3)));
        s = std::max(s, saturate_cast<ପୂର୍ଣ୍ଣସଂଖ୍ୟା>(v_reduce_max(r0)));
        ପାଇଁ (; j < n; j++) {
            s = std::max(s, std::abs(src[j]));
        }
        ଫେରନ୍ତୁ s;
    }
};

template<>
ସଂରଚନା NormInf_SIMD<ଦଶମିକ, ଦଶମିକ> {
    ଦଶମିକ operator() (ଅଚଳ ଦଶମିକ* src, ପୂର୍ଣ୍ଣସଂଖ୍ୟା n) ଅଚଳ {
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା j = 0;
        ଦଶମିକ s = 0.f;
        v_float32 r0 = vx_setzero_f32(), r1 = vx_setzero_f32();
        v_float32 r2 = vx_setzero_f32(), r3 = vx_setzero_f32();
        ପାଇଁ (; j <= n - 4 * VTraits<v_float32>::vlanes(); j += 4 * VTraits<v_float32>::vlanes()) {
            r0 = v_max(r0, v_abs(vx_load(src + j                                   )));
            r1 = v_max(r1, v_abs(vx_load(src + j +     VTraits<v_float32>::vlanes())));
            r2 = v_max(r2, v_abs(vx_load(src + j + 2 * VTraits<v_float32>::vlanes())));
            r3 = v_max(r3, v_abs(vx_load(src + j + 3 * VTraits<v_float32>::vlanes())));
        }
        r0 = v_max(r0, v_max(r1, v_max(r2, r3)));
        ପାଇଁ (; j < n; j++) {
            s = std::max(s, cv_abs(src[j]));
        }
        ଫେରନ୍ତୁ std::max(s, v_reduce_max(r0));
    }
};

template<>
ସଂରଚନା NormL1_SIMD<uchar, ପୂର୍ଣ୍ଣସଂଖ୍ୟା> {
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା operator() (ଅଚଳ uchar* src, ପୂର୍ଣ୍ଣସଂଖ୍ୟା n) ଅଚଳ {
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା j = 0;
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା s = 0;
        v_uint32 r0 = vx_setzero_u32(), r1 = vx_setzero_u32();
        v_uint8  one = vx_setall_u8(1);
        ପାଇଁ (; j<= n - 2 * VTraits<v_uint8>::vlanes(); j += 2 * VTraits<v_uint8>::vlanes()) {
            v_uint8 v0 = vx_load(src + j);
            r0 = v_dotprod_expand_fast(v0, one, r0);

            v_uint8 v1 = vx_load(src + j + VTraits<v_uint8>::vlanes());
            r1 = v_dotprod_expand_fast(v1, one, r1);
        }
        s += v_reduce_sum(v_add(r0, r1));
        ପାଇଁ (; j < n; j++) {
            s += src[j];
        }
        ଫେରନ୍ତୁ s;
    }
};

template<>
ସଂରଚନା NormL1_SIMD<schar, ପୂର୍ଣ୍ଣସଂଖ୍ୟା> {
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା operator() (ଅଚଳ schar* src, ପୂର୍ଣ୍ଣସଂଖ୍ୟା n) ଅଚଳ {
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା j = 0;
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା s = 0;
        v_uint32 r0 = vx_setzero_u32(), r1 = vx_setzero_u32();
        v_uint8  one = vx_setall_u8(1);
        ପାଇଁ (; j<= n - 2 * VTraits<v_int8>::vlanes(); j += 2 * VTraits<v_int8>::vlanes()) {
            v_uint8 v0 = v_abs(vx_load(src + j));
            r0 = v_dotprod_expand_fast(v0, one, r0);

            v_uint8 v1 = v_abs(vx_load(src + j + VTraits<v_int8>::vlanes()));
            r1 = v_dotprod_expand_fast(v1, one, r1);
        }
        s += v_reduce_sum(v_add(r0, r1));
        ପାଇଁ (; j < n; j++) {
            s += saturate_cast<ପୂର୍ଣ୍ଣସଂଖ୍ୟା>(cv_abs(src[j]));
        }
        ଫେରନ୍ତୁ s;
    }
};

template<>
ସଂରଚନା NormL1_SIMD<ushort, ପୂର୍ଣ୍ଣସଂଖ୍ୟା> {
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା operator() (ଅଚଳ ushort* src, ପୂର୍ଣ୍ଣସଂଖ୍ୟା n) ଅଚଳ {
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା j = 0;
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା s = 0;
        v_uint32 r00 = vx_setzero_u32(), r01 = vx_setzero_u32();
        v_uint32 r10 = vx_setzero_u32(), r11 = vx_setzero_u32();
        ପାଇଁ (; j<= n - 2 * VTraits<v_uint16>::vlanes(); j += 2 * VTraits<v_uint16>::vlanes()) {
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
        s += (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)v_reduce_sum(v_add(v_add(v_add(r00, r01), r10), r11));
        ପାଇଁ (; j < n; j++) {
            s += src[j];
        }
        ଫେରନ୍ତୁ s;
    }
};

template<>
ସଂରଚନା NormL1_SIMD<short, ପୂର୍ଣ୍ଣସଂଖ୍ୟା> {
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା operator() (ଅଚଳ short* src, ପୂର୍ଣ୍ଣସଂଖ୍ୟା n) ଅଚଳ {
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା j = 0;
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା s = 0;
        v_uint32 r00 = vx_setzero_u32(), r01 = vx_setzero_u32();
        v_uint32 r10 = vx_setzero_u32(), r11 = vx_setzero_u32();
        ପାଇଁ (; j<= n - 2 * VTraits<v_int16>::vlanes(); j += 2 * VTraits<v_int16>::vlanes()) {
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
        s += (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)v_reduce_sum(v_add(v_add(v_add(r00, r01), r10), r11));
        ପାଇଁ (; j < n; j++) {
            s += saturate_cast<ପୂର୍ଣ୍ଣସଂଖ୍ୟା>(cv_abs(src[j]));
        }
        ଫେରନ୍ତୁ s;
    }
};

template<>
ସଂରଚନା NormL2_SIMD<uchar, ପୂର୍ଣ୍ଣସଂଖ୍ୟା> {
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା operator() (ଅଚଳ uchar* src, ପୂର୍ଣ୍ଣସଂଖ୍ୟା n) ଅଚଳ {
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା j = 0;
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା s = 0;
        v_uint32 r0 = vx_setzero_u32(), r1 = vx_setzero_u32();
        ପାଇଁ (; j <= n - 2 * VTraits<v_uint8>::vlanes(); j += 2 * VTraits<v_uint8>::vlanes()) {
            v_uint8 v0 = vx_load(src + j);
            r0 = v_dotprod_expand_fast(v0, v0, r0);

            v_uint8 v1 = vx_load(src + j + VTraits<v_uint8>::vlanes());
            r1 = v_dotprod_expand_fast(v1, v1, r1);
        }
        s += v_reduce_sum(v_add(r0, r1));
        ପାଇଁ (; j < n; j++) {
            ପୂର୍ଣ୍ଣସଂଖ୍ୟା v = saturate_cast<ପୂର୍ଣ୍ଣସଂଖ୍ୟା>(src[j]);
            s += v * v;
        }
        ଫେରନ୍ତୁ s;
    }
};

template<>
ସଂରଚନା NormL2_SIMD<schar, ପୂର୍ଣ୍ଣସଂଖ୍ୟା> {
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା operator() (ଅଚଳ schar* src, ପୂର୍ଣ୍ଣସଂଖ୍ୟା n) ଅଚଳ {
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା j = 0;
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା s = 0;
        v_int32 r0 = vx_setzero_s32(), r1 = vx_setzero_s32();
        ପାଇଁ (; j <= n - 2 * VTraits<v_int8>::vlanes(); j += 2 * VTraits<v_int8>::vlanes()) {
            v_int8 v0 = vx_load(src + j);
            r0 = v_dotprod_expand_fast(v0, v0, r0);
            v_int8 v1 = vx_load(src + j + VTraits<v_int8>::vlanes());
            r1 = v_dotprod_expand_fast(v1, v1, r1);
        }
        s += v_reduce_sum(v_add(r0, r1));
        ପାଇଁ (; j < n; j++) {
            ପୂର୍ଣ୍ଣସଂଖ୍ୟା v = saturate_cast<ପୂର୍ଣ୍ଣସଂଖ୍ୟା>(src[j]);
            s += v * v;
        }
        ଫେରନ୍ତୁ s;
    }
};

template<>
ସଂରଚନା NormDiffInf_SIMD<uchar, ପୂର୍ଣ୍ଣସଂଖ୍ୟା> {
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା operator() (ଅଚଳ uchar* src1, ଅଚଳ uchar* src2, ପୂର୍ଣ୍ଣସଂଖ୍ୟା n) ଅଚଳ {
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା j = 0;
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା s = 0;
        v_uint8 r0 = vx_setzero_u8(), r1 = vx_setzero_u8();
        v_uint8 r2 = vx_setzero_u8(), r3 = vx_setzero_u8();
        ପାଇଁ (; j <= n - 4 * VTraits<v_uint8>::vlanes(); j += 4 * VTraits<v_uint8>::vlanes()) {
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
        s = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)v_reduce_max(v_max(v_max(v_max(r0, r1), r2), r3));
        ପାଇଁ (; j < n; j++) {
            ପୂର୍ଣ୍ଣସଂଖ୍ୟା v = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)cv_absdiff(src1[j], src2[j]);
            s = std::max(s, v);
        }
        ଫେରନ୍ତୁ s;
    }
};

template<>
ସଂରଚନା NormDiffInf_SIMD<schar, ପୂର୍ଣ୍ଣସଂଖ୍ୟା> {
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା operator() (ଅଚଳ schar* src1, ଅଚଳ schar* src2, ପୂର୍ଣ୍ଣସଂଖ୍ୟା n) ଅଚଳ {
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା j = 0;
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା s = 0;
        v_uint8 r0 = vx_setzero_u8(), r1 = vx_setzero_u8();
        v_uint8 r2 = vx_setzero_u8(), r3 = vx_setzero_u8();
        ପାଇଁ (; j <= n - 4 * VTraits<v_int8>::vlanes(); j += 4 * VTraits<v_int8>::vlanes()) {
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
        s = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)v_reduce_max(v_max(v_max(v_max(r0, r1), r2), r3));
        ପାଇଁ (; j < n; j++) {
            ପୂର୍ଣ୍ଣସଂଖ୍ୟା v = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)cv_absdiff(src1[j], src2[j]);
            s = std::max(s, v);
        }
        ଫେରନ୍ତୁ s;
    }
};

template<>
ସଂରଚନା NormDiffInf_SIMD<ushort, ପୂର୍ଣ୍ଣସଂଖ୍ୟା> {
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା operator() (ଅଚଳ ushort* src1, ଅଚଳ ushort* src2, ପୂର୍ଣ୍ଣସଂଖ୍ୟା n) ଅଚଳ {
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା j = 0;
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା s = 0;
        v_uint16 r0 = vx_setzero_u16(), r1 = vx_setzero_u16();
        v_uint16 r2 = vx_setzero_u16(), r3 = vx_setzero_u16();
        ପାଇଁ (; j <= n - 4 * VTraits<v_uint16>::vlanes(); j += 4 * VTraits<v_uint16>::vlanes()) {
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
        s = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)v_reduce_max(v_max(v_max(v_max(r0, r1), r2), r3));
        ପାଇଁ (; j < n; j++) {
            ପୂର୍ଣ୍ଣସଂଖ୍ୟା v = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)cv_absdiff(src1[j], src2[j]);
            s = std::max(s, v);
        }
        ଫେରନ୍ତୁ s;
    }
};

template<>
ସଂରଚନା NormDiffInf_SIMD<short, ପୂର୍ଣ୍ଣସଂଖ୍ୟା> {
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା operator() (ଅଚଳ short* src1, ଅଚଳ short* src2, ପୂର୍ଣ୍ଣସଂଖ୍ୟା n) ଅଚଳ {
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା j = 0;
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା s = 0;
        v_uint16 r0 = vx_setzero_u16(), r1 = vx_setzero_u16();
        v_uint16 r2 = vx_setzero_u16(), r3 = vx_setzero_u16();
        ପାଇଁ (; j <= n - 4 * VTraits<v_int16>::vlanes(); j += 4 * VTraits<v_int16>::vlanes()) {
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
        s = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)v_reduce_max(v_max(v_max(v_max(r0, r1), r2), r3));
        ପାଇଁ (; j < n; j++) {
            ପୂର୍ଣ୍ଣସଂଖ୍ୟା v = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)cv_absdiff(src1[j], src2[j]);
            s = std::max(s, v);
        }
        ଫେରନ୍ତୁ s;
    }
};

template<>
ସଂରଚନା NormDiffInf_SIMD<ପୂର୍ଣ୍ଣସଂଖ୍ୟା, unsigned> {
    unsigned operator() (ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା* src1, ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା* src2, ପୂର୍ଣ୍ଣସଂଖ୍ୟା n) ଅଚଳ {
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା j = 0;
        unsigned s = 0;
        v_uint32 r0 = vx_setzero_u32(), r1 = vx_setzero_u32();
        v_uint32 r2 = vx_setzero_u32(), r3 = vx_setzero_u32();
        ପାଇଁ (; j <= n - 4 * VTraits<v_int32>::vlanes(); j += 4 * VTraits<v_int32>::vlanes()) {
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
        s = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)v_reduce_max(v_max(v_max(v_max(r0, r1), r2), r3));
        ପାଇଁ (; j < n; j++) {
            unsigned v = (unsigned)cv_absdiff(src1[j], src2[j]);
            s = std::max(s, v);
        }
        ଫେରନ୍ତୁ s;
    }
};

template<>
ସଂରଚନା NormDiffInf_SIMD<ଦଶମିକ, ଦଶମିକ> {
    ଦଶମିକ operator() (ଅଚଳ ଦଶମିକ* src1, ଅଚଳ ଦଶମିକ* src2, ପୂର୍ଣ୍ଣସଂଖ୍ୟା n) ଅଚଳ {
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା j = 0;
        ଦଶମିକ s = 0;
        v_float32 r0 = vx_setzero_f32(), r1 = vx_setzero_f32();
        ପାଇଁ (; j <= n - 2 * VTraits<v_float32>::vlanes(); j += 2 * VTraits<v_float32>::vlanes()) {
            v_float32 v01 = vx_load(src1 + j), v02 = vx_load(src2 + j);
            r0 = v_max(r0, v_absdiff(v01, v02));

            v_float32 v11 = vx_load(src1 + j + VTraits<v_float32>::vlanes()),
                      v12 = vx_load(src2 + j + VTraits<v_float32>::vlanes());
            r1 = v_max(r1, v_absdiff(v11, v12));
        }
        s = v_reduce_max(v_max(r0, r1));
        ପାଇଁ (; j < n; j++) {
            ଦଶମିକ v = (ଦଶମିକ)cv_absdiff(src1[j], src2[j]);
            s = std::max(s, v);
        }
        ଫେରନ୍ତୁ s;
    }
};

template<>
ସଂରଚନା NormDiffL1_SIMD<uchar, ପୂର୍ଣ୍ଣସଂଖ୍ୟା> {
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା operator() (ଅଚଳ uchar* src1, ଅଚଳ uchar* src2, ପୂର୍ଣ୍ଣସଂଖ୍ୟା n) ଅଚଳ {
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା j = 0;
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା s = 0;
        v_uint32 r0 = vx_setzero_u32(), r1 = vx_setzero_u32();
        v_uint8  one = vx_setall_u8(1);
        ପାଇଁ (; j<= n - 2 * VTraits<v_uint8>::vlanes(); j += 2 * VTraits<v_uint8>::vlanes()) {
            v_uint8 v01 = vx_load(src1 + j), v02 = vx_load(src2 + j);
            r0 = v_dotprod_expand_fast(v_absdiff(v01, v02), one, r0);

            v_uint8 v11 = vx_load(src1 + j + VTraits<v_uint8>::vlanes()),
                    v12 = vx_load(src2 + j + VTraits<v_uint8>::vlanes());
            r1 = v_dotprod_expand_fast(v_absdiff(v11, v12), one, r1);
        }
        s += v_reduce_sum(v_add(r0, r1));
        ପାଇଁ (; j < n; j++) {
            ପୂର୍ଣ୍ଣସଂଖ୍ୟା v = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)cv_absdiff(src1[j], src2[j]);
            s += v;
        }
        ଫେରନ୍ତୁ s;
    }
};

template<>
ସଂରଚନା NormDiffL1_SIMD<schar, ପୂର୍ଣ୍ଣସଂଖ୍ୟା> {
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା operator() (ଅଚଳ schar* src1, ଅଚଳ schar* src2, ପୂର୍ଣ୍ଣସଂଖ୍ୟା n) ଅଚଳ {
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା j = 0;
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା s = 0;
        v_uint32 r0 = vx_setzero_u32(), r1 = vx_setzero_u32();
        v_uint8  one = vx_setall_u8(1);
        ପାଇଁ (; j<= n - 2 * VTraits<v_int8>::vlanes(); j += 2 * VTraits<v_int8>::vlanes()) {
            v_int8 v01 = vx_load(src1 + j), v02 = vx_load(src2 + j);
            r0 = v_dotprod_expand_fast(v_absdiff(v01, v02), one, r0);

            v_int8 v11 = vx_load(src1 + j + VTraits<v_int8>::vlanes()),
                   v12 = vx_load(src2 + j + VTraits<v_int8>::vlanes());
            r1 = v_dotprod_expand_fast(v_absdiff(v11, v12), one, r1);
        }
        s += v_reduce_sum(v_add(r0, r1));
        ପାଇଁ (; j < n; j++) {
            ପୂର୍ଣ୍ଣସଂଖ୍ୟା v = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)cv_absdiff(src1[j], src2[j]);
            s += v;
        }
        ଫେରନ୍ତୁ s;
    }
};

template<>
ସଂରଚନା NormDiffL1_SIMD<ushort, ପୂର୍ଣ୍ଣସଂଖ୍ୟା> {
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା operator() (ଅଚଳ ushort* src1, ଅଚଳ ushort* src2, ପୂର୍ଣ୍ଣସଂଖ୍ୟା n) ଅଚଳ {
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା j = 0;
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା s = 0;
        v_uint32 r0 = vx_setzero_u32(), r1 = vx_setzero_u32();
        v_uint32 r2 = vx_setzero_u32(), r3 = vx_setzero_u32();
        ପାଇଁ (; j<= n - 4 * VTraits<v_uint16>::vlanes(); j += 4 * VTraits<v_uint16>::vlanes()) {
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
        s += (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)v_reduce_sum(v_add(v_add(v_add(r0, r1), r2), r3));
        ପାଇଁ (; j < n; j++) {
            ପୂର୍ଣ୍ଣସଂଖ୍ୟା v = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)cv_absdiff(src1[j], src2[j]);
            s += v;
        }
        ଫେରନ୍ତୁ s;
    }
};

template<>
ସଂରଚନା NormDiffL1_SIMD<short, ପୂର୍ଣ୍ଣସଂଖ୍ୟା> {
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା operator() (ଅଚଳ short* src1, ଅଚଳ short* src2, ପୂର୍ଣ୍ଣସଂଖ୍ୟା n) ଅଚଳ {
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା j = 0;
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା s = 0;
        v_uint32 r0 = vx_setzero_u32(), r1 = vx_setzero_u32();
        v_uint32 r2 = vx_setzero_u32(), r3 = vx_setzero_u32();
        ପାଇଁ (; j<= n - 4 * VTraits<v_int16>::vlanes(); j += 4 * VTraits<v_int16>::vlanes()) {
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
        s += (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)v_reduce_sum(v_add(v_add(v_add(r0, r1), r2), r3));
        ପାଇଁ (; j < n; j++) {
            ପୂର୍ଣ୍ଣସଂଖ୍ୟା v = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)cv_absdiff(src1[j], src2[j]);
            s += v;
        }
        ଫେରନ୍ତୁ s;
    }
};

template<>
ସଂରଚନା NormDiffL2_SIMD<uchar, ପୂର୍ଣ୍ଣସଂଖ୍ୟା> {
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା operator() (ଅଚଳ uchar* src1, ଅଚଳ uchar* src2, ପୂର୍ଣ୍ଣସଂଖ୍ୟା n) ଅଚଳ {
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା j = 0;
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା s = 0;
        v_uint32 r0 = vx_setzero_u32(), r1 = vx_setzero_u32();
        ପାଇଁ (; j <= n - 2 * VTraits<v_uint8>::vlanes(); j += 2 * VTraits<v_uint8>::vlanes()) {
            v_uint8 v01 = vx_load(src1 + j), v02 = vx_load(src2 + j);
            v_uint8 v0 = v_absdiff(v01, v02);
            r0 = v_dotprod_expand_fast(v0, v0, r0);

            v_uint8 v11 = vx_load(src1 + j + VTraits<v_uint8>::vlanes()),
                    v12 = vx_load(src2 + j + VTraits<v_uint8>::vlanes());
            v_uint8 v1 = v_absdiff(v11, v12);
            r1 = v_dotprod_expand_fast(v1, v1, r1);
        }
        s += v_reduce_sum(v_add(r0, r1));
        ପାଇଁ (; j < n; j++) {
            ପୂର୍ଣ୍ଣସଂଖ୍ୟା v = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)src1[j] - (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)src2[j];
            s += v * v;
        }
        ଫେରନ୍ତୁ s;
    }
};

template<>
ସଂରଚନା NormDiffL2_SIMD<schar, ପୂର୍ଣ୍ଣସଂଖ୍ୟା> {
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା operator() (ଅଚଳ schar* src1, ଅଚଳ schar* src2, ପୂର୍ଣ୍ଣସଂଖ୍ୟା n) ଅଚଳ {
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା j = 0;
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା s = 0;
        v_uint32 r0 = vx_setzero_u32(), r1 = vx_setzero_u32();
        ପାଇଁ (; j <= n - 2 * VTraits<v_int8>::vlanes(); j += 2 * VTraits<v_int8>::vlanes()) {
            v_int8 v01 = vx_load(src1 + j), v02 = vx_load(src2 + j);
            v_uint8 v0 = v_absdiff(v01, v02);
            r0 = v_dotprod_expand_fast(v0, v0, r0);

            v_int8 v11 = vx_load(src1 + j + VTraits<v_int8>::vlanes()),
                   v12 = vx_load(src2 + j + VTraits<v_int8>::vlanes());
            v_uint8 v1 = v_absdiff(v11, v12);
            r1 = v_dotprod_expand_fast(v1, v1, r1);
        }
        s += v_reduce_sum(v_add(r0, r1));
        ପାଇଁ (; j < n; j++) {
            ପୂର୍ଣ୍ଣସଂଖ୍ୟା v = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)src1[j] - (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)src2[j];
            s += v * v;
        }
        ଫେରନ୍ତୁ s;
    }
};

#endif

#ଯଦି (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)

template<>
ସଂରଚନା NormInf_SIMD<ଦ୍ୱିଗୁଣ, ଦ୍ୱିଗୁଣ> {
    ଦ୍ୱିଗୁଣ operator() (ଅଚଳ ଦ୍ୱିଗୁଣ* src, ପୂର୍ଣ୍ଣସଂଖ୍ୟା n) ଅଚଳ {
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା j = 0;
        ଦ୍ୱିଗୁଣ s = 0.f;
        v_float64 r0 = vx_setzero_f64(), r1 = vx_setzero_f64();
        v_float64 r2 = vx_setzero_f64(), r3 = vx_setzero_f64();
        ପାଇଁ (; j <= n - 4 * VTraits<v_float64>::vlanes(); j += 4 * VTraits<v_float64>::vlanes()) {
            r0 = v_max(r0, v_abs(vx_load(src + j                                   )));
            r1 = v_max(r1, v_abs(vx_load(src + j +     VTraits<v_float64>::vlanes())));
            r2 = v_max(r2, v_abs(vx_load(src + j + 2 * VTraits<v_float64>::vlanes())));
            r3 = v_max(r3, v_abs(vx_load(src + j + 3 * VTraits<v_float64>::vlanes())));
        }
        r0 = v_max(r0, v_max(r1, v_max(r2, r3)));
        ପାଇଁ (; j < n; j++) {
            s = std::max(s, cv_abs(src[j]));
        }
        // [TODO]: use v_reduce_max when it supports float64
        ଦ୍ୱିଗୁଣ t[VTraits<v_float64>::max_nlanes];
        vx_store(t, r0);
        ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < VTraits<v_float64>::vlanes(); i++) {
            s = std::max(s, cv_abs(t[i]));
        }
        ଫେରନ୍ତୁ s;
    }
};

template<>
ସଂରଚନା NormL1_SIMD<ପୂର୍ଣ୍ଣସଂଖ୍ୟା, ଦ୍ୱିଗୁଣ> {
    ଦ୍ୱିଗୁଣ operator() (ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା* src, ପୂର୍ଣ୍ଣସଂଖ୍ୟା n) ଅଚଳ {
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା j = 0;
        ଦ୍ୱିଗୁଣ s = 0.f;
        v_float64 r00 = vx_setzero_f64(), r01 = vx_setzero_f64();
        v_float64 r10 = vx_setzero_f64(), r11 = vx_setzero_f64();
        ପାଇଁ (; j <= n - 2 * VTraits<v_int32>::vlanes(); j += 2 * VTraits<v_int32>::vlanes()) {
            // Widen to double before taking the magnitude: exact over the whole int32 range,
            // INT_MIN included, which a float intermediate would not be above 2^24.
            v_int32 i0 = vx_load(src + j), i1 = vx_load(src + j + VTraits<v_int32>::vlanes());
            r00 = v_add(r00, v_abs(v_cvt_f64(i0))); r01 = v_add(r01, v_abs(v_cvt_f64_ଉପର(i0)));
            r10 = v_add(r10, v_abs(v_cvt_f64(i1))); r11 = v_add(r11, v_abs(v_cvt_f64_ଉପର(i1)));
        }
        s += v_reduce_sum(v_add(v_add(v_add(r00, r01), r10), r11));
        ପାଇଁ (; j < n; j++) {
            s += cv_abs(src[j]);
        }
        ଫେରନ୍ତୁ s;
    }
};

template<>
ସଂରଚନା NormL1_SIMD<ଦଶମିକ, ଦ୍ୱିଗୁଣ> {
    ଦ୍ୱିଗୁଣ operator() (ଅଚଳ ଦଶମିକ* src, ପୂର୍ଣ୍ଣସଂଖ୍ୟା n) ଅଚଳ {
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା j = 0;
        ଦ୍ୱିଗୁଣ s = 0.f;
        v_float64 r00 = vx_setzero_f64(), r01 = vx_setzero_f64();
        v_float64 r10 = vx_setzero_f64(), r11 = vx_setzero_f64();
        v_float64 r20 = vx_setzero_f64(), r21 = vx_setzero_f64();
        v_float64 r30 = vx_setzero_f64(), r31 = vx_setzero_f64();
        ପାଇଁ (; j <= n - 4 * VTraits<v_float32>::vlanes(); j += 4 * VTraits<v_float32>::vlanes()) {
            v_float32 v0 = v_abs(vx_load(src + j)), v1 = v_abs(vx_load(src + j + VTraits<v_float32>::vlanes()));
            r00 = v_add(r00, v_cvt_f64(v0)); r01 = v_add(r01, v_cvt_f64_ଉପର(v0));
            r10 = v_add(r10, v_cvt_f64(v1)); r11 = v_add(r11, v_cvt_f64_ଉପର(v1));

            v_float32 v2 = v_abs(vx_load(src + j + 2 * VTraits<v_float32>::vlanes())), v3 = v_abs(vx_load(src + j + 3 * VTraits<v_float32>::vlanes()));
            r20 = v_add(r20, v_cvt_f64(v2)); r21 = v_add(r21, v_cvt_f64_ଉପର(v2));
            r30 = v_add(r30, v_cvt_f64(v3)); r31 = v_add(r31, v_cvt_f64_ଉପର(v3));
        }
        s += v_reduce_sum(v_add(v_add(v_add(r00, r01), r10), r11));
        s += v_reduce_sum(v_add(v_add(v_add(r20, r21), r30), r31));
        ପାଇଁ (; j < n; j++) {
            s += cv_abs(src[j]);
        }
        ଫେରନ୍ତୁ s;
    }
};

template<>
ସଂରଚନା NormL2_SIMD<ushort, ଦ୍ୱିଗୁଣ> {
    ଦ୍ୱିଗୁଣ operator() (ଅଚଳ ushort* src, ପୂର୍ଣ୍ଣସଂଖ୍ୟା n) ଅଚଳ {
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା j = 0;
        ଦ୍ୱିଗୁଣ s = 0.f;
        v_float64 r0 = vx_setzero_f64(), r1 = vx_setzero_f64();
        ପାଇଁ (; j <= n - 2 * VTraits<v_uint16>::vlanes(); j += 2 * VTraits<v_uint16>::vlanes()) {
            v_uint16 v0 = vx_load(src + j);
            v_uint64 u0 = v_dotprod_expand_fast(v0, v0);
            r0 = v_add(r0, v_cvt_f64(v_reinterpret_as_s64(u0)));

            v_uint16 v1 = vx_load(src + j + VTraits<v_uint16>::vlanes());
            v_uint64 u1 = v_dotprod_expand_fast(v1, v1);
            r1 = v_add(r1, v_cvt_f64(v_reinterpret_as_s64(u1)));
        }
        s += v_reduce_sum(v_add(r0, r1));
        ପାଇଁ (; j < n; j++) {
            ଦ୍ୱିଗୁଣ v = saturate_cast<ଦ୍ୱିଗୁଣ>(src[j]);
            s += v * v;
        }
        ଫେରନ୍ତୁ s;
    }
};

template<>
ସଂରଚନା NormL2_SIMD<short, ଦ୍ୱିଗୁଣ> {
    ଦ୍ୱିଗୁଣ operator() (ଅଚଳ short* src, ପୂର୍ଣ୍ଣସଂଖ୍ୟା n) ଅଚଳ {
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା j = 0;
        ଦ୍ୱିଗୁଣ s = 0.f;
        v_float64 r0 = vx_setzero_f64(), r1 = vx_setzero_f64();
        ପାଇଁ (; j <= n - 2 * VTraits<v_int16>::vlanes(); j += 2 * VTraits<v_int16>::vlanes()) {
            v_int16 v0 = vx_load(src + j);
            r0 = v_add(r0, v_cvt_f64(v_dotprod_expand_fast(v0, v0)));

            v_int16 v1 = vx_load(src + j + VTraits<v_int16>::vlanes());
            r1 = v_add(r1, v_cvt_f64(v_dotprod_expand_fast(v1, v1)));
        }
        s += v_reduce_sum(v_add(r0, r1));
        ପାଇଁ (; j < n; j++) {
            ଦ୍ୱିଗୁଣ v = saturate_cast<ଦ୍ୱିଗୁଣ>(src[j]);
            s += v * v;
        }
        ଫେରନ୍ତୁ s;
    }
};

template<>
ସଂରଚନା NormL2_SIMD<ପୂର୍ଣ୍ଣସଂଖ୍ୟା, ଦ୍ୱିଗୁଣ> {
    ଦ୍ୱିଗୁଣ operator() (ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା* src, ପୂର୍ଣ୍ଣସଂଖ୍ୟା n) ଅଚଳ {
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା j = 0;
        ଦ୍ୱିଗୁଣ s = 0.f;
        v_float64 r0 = vx_setzero_f64(), r1 = vx_setzero_f64();
        ପାଇଁ (; j <= n - 2 * VTraits<v_int32>::vlanes(); j += 2 * VTraits<v_int32>::vlanes()) {
            v_int32 v0 = vx_load(src + j);
            r0 = v_dotprod_expand_fast(v0, v0, r0);

            v_int32 v1 = vx_load(src + j + VTraits<v_int32>::vlanes());
            r1 = v_dotprod_expand_fast(v1, v1, r1);
        }
        s += v_reduce_sum(v_add(r0, r1));
        ପାଇଁ (; j < n; j++) {
            ଦ୍ୱିଗୁଣ v = src[j];
            s += v * v;
        }
        ଫେରନ୍ତୁ s;
    }
};

template<>
ସଂରଚନା NormL2_SIMD<ଦଶମିକ, ଦ୍ୱିଗୁଣ> {
    ଦ୍ୱିଗୁଣ operator() (ଅଚଳ ଦଶମିକ* src, ପୂର୍ଣ୍ଣସଂଖ୍ୟା n) ଅଚଳ {
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା j = 0;
        ଦ୍ୱିଗୁଣ s = 0.f;
        v_float64 r00 = vx_setzero_f64(), r01 = vx_setzero_f64();
        v_float64 r10 = vx_setzero_f64(), r11 = vx_setzero_f64();
        ପାଇଁ (; j <= n - 2 * VTraits<v_float32>::vlanes(); j += 2 * VTraits<v_float32>::vlanes()) {
            v_float32 v0 = vx_load(src + j), v1 = vx_load(src + j + VTraits<v_float32>::vlanes());
            v_float64 v00 = v_cvt_f64(v0), v01 = v_cvt_f64_ଉପର(v0);
            v_float64 v10 = v_cvt_f64(v1), v11 = v_cvt_f64_ଉପର(v1);
            r00 = v_fma(v00, v00, r00); r01 = v_fma(v01, v01, r01);
            r10 = v_fma(v10, v10, r10); r11 = v_fma(v11, v11, r11);
        }
        s += v_reduce_sum(v_add(v_add(v_add(r00, r01), r10), r11));
        ପାଇଁ (; j < n; j++) {
            ଦ୍ୱିଗୁଣ v = src[j];
            s += v * v;
        }
        ଫେରନ୍ତୁ s;
    }
};

#endif

#ଯଦି CV_SIMD_64F // CV_SIMD_SCALABLE_64F has accuracy problem with the following kernels on ci

template<>
ସଂରଚନା NormL1_SIMD<ଦ୍ୱିଗୁଣ, ଦ୍ୱିଗୁଣ> {
    ଦ୍ୱିଗୁଣ operator() (ଅଚଳ ଦ୍ୱିଗୁଣ* src, ପୂର୍ଣ୍ଣସଂଖ୍ୟା n) ଅଚଳ {
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା j = 0;
        ଦ୍ୱିଗୁଣ s = 0.f;
        v_float64 r00 = vx_setzero_f64(), r01 = vx_setzero_f64();
        v_float64 r10 = vx_setzero_f64(), r11 = vx_setzero_f64();
        ପାଇଁ (; j <= n - 4 * VTraits<v_float64>::vlanes(); j += 4 * VTraits<v_float64>::vlanes()) {
            r00 = v_add(r00, v_abs(vx_load(src + j                                   )));
            r01 = v_add(r01, v_abs(vx_load(src + j +     VTraits<v_float64>::vlanes())));
            r10 = v_add(r10, v_abs(vx_load(src + j + 2 * VTraits<v_float64>::vlanes())));
            r11 = v_add(r11, v_abs(vx_load(src + j + 3 * VTraits<v_float64>::vlanes())));
        }
        s += v_reduce_sum(v_add(v_add(v_add(r00, r01), r10), r11));
        ପାଇଁ (; j < n; j++) {
            s += cv_abs(src[j]);
        }
        ଫେରନ୍ତୁ s;
    }
};

template<>
ସଂରଚନା NormL2_SIMD<ଦ୍ୱିଗୁଣ, ଦ୍ୱିଗୁଣ> {
    ଦ୍ୱିଗୁଣ operator() (ଅଚଳ ଦ୍ୱିଗୁଣ* src, ପୂର୍ଣ୍ଣସଂଖ୍ୟା n) ଅଚଳ {
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା j = 0;
        ଦ୍ୱିଗୁଣ s = 0.f;
        v_float64 r00 = vx_setzero_f64(), r01 = vx_setzero_f64();
        v_float64 r10 = vx_setzero_f64(), r11 = vx_setzero_f64();
        ପାଇଁ (; j <= n - 4 * VTraits<v_float64>::vlanes(); j += 4 * VTraits<v_float64>::vlanes()) {
            v_float64 v00 = vx_load(src + j                                   );
            v_float64 v01 = vx_load(src + j +     VTraits<v_float64>::vlanes());
            v_float64 v10 = vx_load(src + j + 2 * VTraits<v_float64>::vlanes());
            v_float64 v11 = vx_load(src + j + 3 * VTraits<v_float64>::vlanes());
            r00 = v_fma(v00, v00, r00); r01 = v_fma(v01, v01, r01);
            r10 = v_fma(v10, v10, r10); r11 = v_fma(v11, v11, r11);
        }
        s += v_reduce_sum(v_add(v_add(v_add(r00, r01), r10), r11));
        ପାଇଁ (; j < n; j++) {
            ଦ୍ୱିଗୁଣ v = src[j];
            s += v * v;
        }
        ଫେରନ୍ତୁ s;
    }
};

template<>
ସଂରଚନା NormDiffInf_SIMD<ଦ୍ୱିଗୁଣ, ଦ୍ୱିଗୁଣ> {
    ଦ୍ୱିଗୁଣ operator() (ଅଚଳ ଦ୍ୱିଗୁଣ* src1, ଅଚଳ ଦ୍ୱିଗୁଣ* src2, ପୂର୍ଣ୍ଣସଂଖ୍ୟା n) ଅଚଳ {
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା j = 0;
        ଦ୍ୱିଗୁଣ s = 0;
        v_float64 r0 = vx_setzero_f64(), r1 = vx_setzero_f64();
        ପାଇଁ (; j <= n - 2 * VTraits<v_float64>::vlanes(); j += 2 * VTraits<v_float64>::vlanes()) {
            v_float64 v01 = vx_load(src1 + j), v02 = vx_load(src2 + j);
            r0 = v_max(r0, v_absdiff(v01, v02));

            v_float64 v11 = vx_load(src1 + j + VTraits<v_float64>::vlanes()),
                      v12 = vx_load(src2 + j + VTraits<v_float64>::vlanes());
            r1 = v_max(r1, v_absdiff(v11, v12));
        }
        // [TODO]: use v_reduce_max when it supports float64
        ଦ୍ୱିଗୁଣ t[VTraits<v_float64>::max_nlanes];
        vx_store(t, v_max(r0, r1));
        ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < VTraits<v_float64>::vlanes(); i++) {
            s = std::max(s, t[i]);
        }
        ପାଇଁ (; j < n; j++) {
            ଦ୍ୱିଗୁଣ v = (ଦ୍ୱିଗୁଣ)cv_absdiff(src1[j], src2[j]);
            s = std::max(s, v);
        }
        ଫେରନ୍ତୁ s;
    }
};

template<>
ସଂରଚନା NormDiffL1_SIMD<ଦଶମିକ, ଦ୍ୱିଗୁଣ> {
    ଦ୍ୱିଗୁଣ operator() (ଅଚଳ ଦଶମିକ* src1, ଅଚଳ ଦଶମିକ* src2, ପୂର୍ଣ୍ଣସଂଖ୍ୟା n) ଅଚଳ {
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା j = 0;
        ଦ୍ୱିଗୁଣ s = 0.f;
        v_float64 r0 = vx_setzero_f64(), r1 = vx_setzero_f64();
        v_float64 r2 = vx_setzero_f64(), r3 = vx_setzero_f64();
        ପାଇଁ (; j <= n - 2 * VTraits<v_float32>::vlanes(); j += 2 * VTraits<v_float32>::vlanes()) {
            v_float32 v01 = vx_load(src1 + j), v02 = vx_load(src2 + j);
            v_float32 v0 = v_absdiff(v01, v02);
            r0 = v_add(r0, v_cvt_f64(v0)); r1 = v_add(r1, v_cvt_f64_ଉପର(v0));

            v_float32 v11 = vx_load(src1 + j + VTraits<v_float32>::vlanes()),
                      v12 = vx_load(src2 + j + VTraits<v_float32>::vlanes());
            v_float32 v1 = v_absdiff(v11, v12);
            r2 = v_add(r2, v_cvt_f64(v1)); r3 = v_add(r3, v_cvt_f64_ଉପର(v1));
        }
        s += v_reduce_sum(v_add(v_add(v_add(r0, r1), r2), r3));
        ପାଇଁ (; j < n; j++) {
            ଦ୍ୱିଗୁଣ v = (ଦ୍ୱିଗୁଣ)cv_absdiff(src1[j], src2[j]);
            s += v;
        }
        ଫେରନ୍ତୁ s;
    }
};

template<>
ସଂରଚନା NormDiffL1_SIMD<ଦ୍ୱିଗୁଣ, ଦ୍ୱିଗୁଣ> {
    ଦ୍ୱିଗୁଣ operator() (ଅଚଳ ଦ୍ୱିଗୁଣ* src1, ଅଚଳ ଦ୍ୱିଗୁଣ* src2, ପୂର୍ଣ୍ଣସଂଖ୍ୟା n) ଅଚଳ {
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା j = 0;
        ଦ୍ୱିଗୁଣ s = 0.f;
        v_float64 r0 = vx_setzero_f64(), r1 = vx_setzero_f64();
        ପାଇଁ (; j <= n - 2 * VTraits<v_float64>::vlanes(); j += 2 * VTraits<v_float64>::vlanes()) {
            v_float64 v01 = vx_load(src1 + j), v02 = vx_load(src2 + j);
            r0 = v_add(r0, v_absdiff(v01, v02));

            v_float64 v11 = vx_load(src1 + j + VTraits<v_float64>::vlanes()),
                      v12 = vx_load(src2 + j + VTraits<v_float64>::vlanes());
            r1 = v_add(r1, v_absdiff(v11, v12));
        }
        s += v_reduce_sum(v_add(r0, r1));
        ପାଇଁ (; j < n; j++) {
            ଦ୍ୱିଗୁଣ v = (ଦ୍ୱିଗୁଣ)cv_absdiff(src1[j], src2[j]);
            s += v;
        }
        ଫେରନ୍ତୁ s;
    }
};

template<>
ସଂରଚନା NormDiffL2_SIMD<ushort, ଦ୍ୱିଗୁଣ> {
    ଦ୍ୱିଗୁଣ operator() (ଅଚଳ ushort* src1, ଅଚଳ ushort* src2, ପୂର୍ଣ୍ଣସଂଖ୍ୟା n) ଅଚଳ {
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା j = 0;
        ଦ୍ୱିଗୁଣ s = 0.f;
        v_float64 r0 = vx_setzero_f64(), r1 = vx_setzero_f64();
        ପାଇଁ (; j <= n - 2 * VTraits<v_uint16>::vlanes(); j += 2 * VTraits<v_uint16>::vlanes()) {
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
        ପାଇଁ (; j < n; j++) {
            ଦ୍ୱିଗୁଣ v = (ଦ୍ୱିଗୁଣ)src1[j] - (ଦ୍ୱିଗୁଣ)src2[j];
            s += v * v;
        }
        ଫେରନ୍ତୁ s;
    }
};

template<>
ସଂରଚନା NormDiffL2_SIMD<short, ଦ୍ୱିଗୁଣ> {
    ଦ୍ୱିଗୁଣ operator() (ଅଚଳ short* src1, ଅଚଳ short* src2, ପୂର୍ଣ୍ଣସଂଖ୍ୟା n) ଅଚଳ {
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା j = 0;
        ଦ୍ୱିଗୁଣ s = 0.f;
        v_float64 r0 = vx_setzero_f64(), r1 = vx_setzero_f64();
        ପାଇଁ (; j <= n - 2 * VTraits<v_int16>::vlanes(); j += 2 * VTraits<v_int16>::vlanes()) {
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
        ପାଇଁ (; j < n; j++) {
            ଦ୍ୱିଗୁଣ v = (ଦ୍ୱିଗୁଣ)src1[j] - (ଦ୍ୱିଗୁଣ)src2[j];
            s += v * v;
        }
        ଫେରନ୍ତୁ s;
    }
};

template<>
ସଂରଚନା NormDiffL2_SIMD<ପୂର୍ଣ୍ଣସଂଖ୍ୟା, ଦ୍ୱିଗୁଣ> {
    ଦ୍ୱିଗୁଣ operator() (ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା* src1, ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା* src2, ପୂର୍ଣ୍ଣସଂଖ୍ୟା n) ଅଚଳ {
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା j = 0;
        ଦ୍ୱିଗୁଣ s = 0.f;
        v_float64 r0 = vx_setzero_f64(), r1 = vx_setzero_f64();
        ପାଇଁ (; j <= n - VTraits<v_int32>::vlanes(); j += VTraits<v_int32>::vlanes()) {
            v_int32 v01 = vx_load(src1 + j), v02 = vx_load(src2 + j);
            v_uint32 v0 = v_absdiff(v01, v02);
            v_uint64 ev0, ev1;
            v_expand(v0, ev0, ev1);
            v_float64 f0 = v_cvt_f64(v_reinterpret_as_s64(ev0)), f1 = v_cvt_f64(v_reinterpret_as_s64(ev1));
            r0 = v_fma(f0, f0, r0); r1 = v_fma(f1, f1, r1);
        }
        s += v_reduce_sum(v_add(r0, r1));
        ପାଇଁ (; j < n; j++) {
            ଦ୍ୱିଗୁଣ v = (ଦ୍ୱିଗୁଣ)src1[j] - (ଦ୍ୱିଗୁଣ)src2[j];
            s += v * v;
        }
        ଫେରନ୍ତୁ s;
    }
};

template<>
ସଂରଚନା NormDiffL2_SIMD<ଦଶମିକ, ଦ୍ୱିଗୁଣ> {
    ଦ୍ୱିଗୁଣ operator() (ଅଚଳ ଦଶମିକ* src1, ଅଚଳ ଦଶମିକ* src2, ପୂର୍ଣ୍ଣସଂଖ୍ୟା n) ଅଚଳ {
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା j = 0;
        ଦ୍ୱିଗୁଣ s = 0.f;
        v_float64 r0 = vx_setzero_f64(), r1 = vx_setzero_f64();
        v_float64 r2 = vx_setzero_f64(), r3 = vx_setzero_f64();
        ପାଇଁ (; j <= n - 2 * VTraits<v_float32>::vlanes(); j += 2 * VTraits<v_float32>::vlanes()) {
            v_float32 v01 = vx_load(src1 + j), v02 = vx_load(src2 + j);
            v_float32 v0 = v_absdiff(v01, v02);
            v_float64 f01 = v_cvt_f64(v0), f02 = v_cvt_f64_ଉପର(v0);
            r0 = v_fma(f01, f01, r0); r1 = v_fma(f02, f02, r1);

            v_float32 v11 = vx_load(src1 + j + VTraits<v_float32>::vlanes()),
                      v12 = vx_load(src2 + j + VTraits<v_float32>::vlanes());
            v_float32 v1 = v_absdiff(v11, v12);
            v_float64 f11 = v_cvt_f64(v1), f12 = v_cvt_f64_ଉପର(v1);
            r2 = v_fma(f11, f11, r2); r3 = v_fma(f12, f12, r3);
        }
        s += v_reduce_sum(v_add(v_add(v_add(r0, r1), r2), r3));
        ପାଇଁ (; j < n; j++) {
            ଦ୍ୱିଗୁଣ v = (ଦ୍ୱିଗୁଣ)src1[j] - (ଦ୍ୱିଗୁଣ)src2[j];
            s += v * v;
        }
        ଫେରନ୍ତୁ s;
    }
};

template<>
ସଂରଚନା NormDiffL2_SIMD<ଦ୍ୱିଗୁଣ, ଦ୍ୱିଗୁଣ> {
    ଦ୍ୱିଗୁଣ operator() (ଅଚଳ ଦ୍ୱିଗୁଣ* src1, ଅଚଳ ଦ୍ୱିଗୁଣ* src2, ପୂର୍ଣ୍ଣସଂଖ୍ୟା n) ଅଚଳ {
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା j = 0;
        ଦ୍ୱିଗୁଣ s = 0.f;
        v_float64 r0 = vx_setzero_f64(), r1 = vx_setzero_f64();
        v_float64 r2 = vx_setzero_f64(), r3 = vx_setzero_f64();
        ପାଇଁ (; j <= n - 4 * VTraits<v_float64>::vlanes(); j += 4 * VTraits<v_float64>::vlanes()) {
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
        ପାଇଁ (; j < n; j++) {
            ଦ୍ୱିଗୁଣ v = (ଦ୍ୱିଗୁଣ)src1[j] - (ଦ୍ୱିଗୁଣ)src2[j];
            s += v * v;
        }
        ଫେରନ୍ତୁ s;
    }
};

#endif

template <typename T, typename ST>
ସଂରଚନା MaskedNormInf_SIMD {
    ଧାଡ଼ିଭିତରେ ST operator() (ଅଚଳ T* src, ଅଚଳ uchar* mask, ପୂର୍ଣ୍ଣସଂଖ୍ୟା len, ପୂର୍ଣ୍ଣସଂଖ୍ୟା cn) ଅଚଳ {
        ST s = 0;
        ଯଦି (cn == 1) {
            ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < len; i++) {
                ଯଦି (mask[i]) {
                    s = std::max(s, (ST)cv_abs(src[i]));
                }
            }
        }
        ଅନ୍ୟଥା {
            ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < len; i++) {
                ଯଦି (mask[i]) {
                    ଅଚଳ T* elem = src + i * cn;
                    ପୂର୍ଣ୍ଣସଂଖ୍ୟା k = 0;
                #ଯଦି CV_ENABLE_UNROLLED && !CV_SIMD_SCALABLE
                    ପାଇଁ (; k <= cn - 4; k += 4) {
                        s = std::max(s, (ST)cv_abs(elem[k]));
                        s = std::max(s, (ST)cv_abs(elem[k + 1]));
                        s = std::max(s, (ST)cv_abs(elem[k + 2]));
                        s = std::max(s, (ST)cv_abs(elem[k + 3]));
                    }
                #endif
                    ପାଇଁ (; k < cn; k++) {
                        s = std::max(s, (ST)cv_abs(elem[k]));
                    }
                }
            }
        }
        ଫେରନ୍ତୁ s;
    }
};

template <typename T, typename ST>
ସଂରଚନା MaskedNormL1_SIMD {
    ଧାଡ଼ିଭିତରେ ST operator() (ଅଚଳ T* src, ଅଚଳ uchar* mask, ପୂର୍ଣ୍ଣସଂଖ୍ୟା len, ପୂର୍ଣ୍ଣସଂଖ୍ୟା cn) ଅଚଳ {
        ST s = 0;
        ଯଦି (cn == 1) {
            ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < len; i++) {
                ଯଦି (mask[i]) {
                    s += (ST)cv_abs(src[i]);
                }
            }
        }
        ଅନ୍ୟଥା {
            ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < len; i++) {
                ଯଦି (mask[i]) {
                    ଅଚଳ T* elem = src + i * cn;
                    ପୂର୍ଣ୍ଣସଂଖ୍ୟା k = 0;
                #ଯଦି CV_ENABLE_UNROLLED && !CV_SIMD_SCALABLE
                    ପାଇଁ (; k <= cn - 4; k += 4) {
                        s += (ST)cv_abs(elem[k]);
                        s += (ST)cv_abs(elem[k + 1]);
                        s += (ST)cv_abs(elem[k + 2]);
                        s += (ST)cv_abs(elem[k + 3]);
                    }
                #endif
                    ପାଇଁ (; k < cn; k++) {
                        s += (ST)cv_abs(elem[k]);
                    }
                }
            }
        }
        ଫେରନ୍ତୁ s;
    }
};

template <typename T, typename ST>
ସଂରଚନା MaskedNormL2_SIMD {
    ଧାଡ଼ିଭିତରେ ST operator() (ଅଚଳ T* src, ଅଚଳ uchar* mask, ପୂର୍ଣ୍ଣସଂଖ୍ୟା len, ପୂର୍ଣ୍ଣସଂଖ୍ୟା cn) ଅଚଳ {
        ST s = 0;
        ଯଦି (cn == 1) {
            ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0;
        #ଯଦି CV_ENABLE_UNROLLED && !CV_SIMD_SCALABLE
            ପାଇଁ (; i <= len - 4; i += 4) {
                ଯଦି (mask[i])     { T v0 = src[i];     s += (ST)v0 * v0; }
                ଯଦି (mask[i + 1]) { T v1 = src[i + 1]; s += (ST)v1 * v1; }
                ଯଦି (mask[i + 2]) { T v2 = src[i + 2]; s += (ST)v2 * v2; }
                ଯଦି (mask[i + 3]) { T v3 = src[i + 3]; s += (ST)v3 * v3; }
            }
        #endif
            ପାଇଁ (; i < len; i++) {
                ଯଦି (mask[i]) {
                    T v = src[i];
                    s += (ST)v * v;
                }
            }
        }
        ଅନ୍ୟଥା {
            ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < len; i++) {
                ଯଦି (mask[i]) {
                    ଅଚଳ T* elem = src + i * cn;
                    ପୂର୍ଣ୍ଣସଂଖ୍ୟା k = 0;
                #ଯଦି CV_ENABLE_UNROLLED && !CV_SIMD_SCALABLE
                    ପାଇଁ (; k <= cn - 4; k += 4) {
                        T v0 = elem[k];     s += (ST)v0 * v0;
                        T v1 = elem[k + 1]; s += (ST)v1 * v1;
                        T v2 = elem[k + 2]; s += (ST)v2 * v2;
                        T v3 = elem[k + 3]; s += (ST)v3 * v3;
                    }
                #endif
                    ପାଇଁ (; k < cn; k++) {
                        T v = elem[k];
                        s += (ST)v * v;
                    }
                }
            }
        }

        ଫେରନ୍ତୁ s;
    }
};

#ଯଦି (CV_SIMD || CV_SIMD_SCALABLE)
ସ୍ଥିର ଧାଡ଼ିଭିତରେ v_uint8  v_normmask_u8 (ଅଚଳ uchar* m) { ଫେରନ୍ତୁ v_gt(vx_load(m),          vx_setzero_u8());  }
ସ୍ଥିର ଧାଡ଼ିଭିତରେ v_uint16 v_normmask_u16(ଅଚଳ uchar* m) { ଫେରନ୍ତୁ v_gt(vx_load_expand(m),   vx_setzero_u16()); }
ସ୍ଥିର ଧାଡ଼ିଭିତରେ v_uint32 v_normmask_u32(ଅଚଳ uchar* m) { ଫେରନ୍ତୁ v_gt(vx_load_expand_q(m), vx_setzero_u32()); }
ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ v_normmask_u64(ଅଚଳ uchar* m, v_uint64& m0, v_uint64& m1) {
    v_uint64 e0, e1;
    v_expand(v_normmask_u32(m), e0, e1);
    m0 = v_or(e0, v_shl<32>(e0));
    m1 = v_or(e1, v_shl<32>(e1));
}

template <>
ସଂରଚନା MaskedNormInf_SIMD<ଦଶମିକ, ଦଶମିକ> {
    ଧାଡ଼ିଭିତରେ ଦଶମିକ operator()(ଅଚଳ ଦଶମିକ* src, ଅଚଳ uchar* mask, ପୂର୍ଣ୍ଣସଂଖ୍ୟା len, ପୂର୍ଣ୍ଣସଂଖ୍ୟା cn) ଅଚଳ {
        ଦଶମିକ result = 0.0f;
        ଯଦି (cn == 1) {
            ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0;
            ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା vstep = VTraits<v_float32>::vlanes();
            v_float32 acc = vx_setzero_f32();

            ପାଇଁ (; i <= len - vstep; i += vstep) {
                v_uint32 m = vx_load_expand_q(mask + i);
                v_uint32 cmp = v_gt(m, vx_setzero_u32());
                v_float32 s = vx_load(src + i);
                s = v_abs(s);
                s = v_reinterpret_as_f32(v_and(v_reinterpret_as_u32(s), cmp));
                acc = v_max(acc, s);
            }
            result = v_reduce_max(acc);

            ପାଇଁ (; i < len; i++) {
                ଯଦି (mask[i])
                    result = std::max(result, std::abs(src[i]));
            }
        }
        ଅନ୍ୟଥା {
            ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < len; i++) {
                ଯଦି (mask[i]) {
                    ଅଚଳ ଦଶମିକ* elem = src + i * cn;
                    ପୂର୍ଣ୍ଣସଂଖ୍ୟା k = 0;
                    ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା vstep = VTraits<v_float32>::vlanes();
                    v_float32 acc = vx_setzero_f32();

                    ପାଇଁ (; k <= cn - vstep; k += vstep) {
                        v_float32 s = vx_load(elem + k);
                        acc = v_max(acc, v_abs(s));
                    }

                    result = std::max(result, v_reduce_max(acc));

                    ପାଇଁ (; k < cn; k++)
                        result = std::max(result, std::abs(elem[k]));
                }
            }
        }
        ଫେରନ୍ତୁ result;
    }
};

#ଯଦି CV_SIMD_64F
template <>
ସଂରଚନା MaskedNormL1_SIMD<ଦଶମିକ, ଦ୍ୱିଗୁଣ> {
    ଧାଡ଼ିଭିତରେ ଦ୍ୱିଗୁଣ operator()(ଅଚଳ ଦଶମିକ* src, ଅଚଳ uchar* mask, ପୂର୍ଣ୍ଣସଂଖ୍ୟା len, ପୂର୍ଣ୍ଣସଂଖ୍ୟା cn) ଅଚଳ {
        ଦ୍ୱିଗୁଣ result = 0.0;
        ଯଦି (cn == 1) {
            ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0;
            ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା vstep = VTraits<v_float32>::vlanes();
            v_float64 acc0 = vx_setzero_f64(), acc1 = vx_setzero_f64();
            v_float64 acc2 = vx_setzero_f64(), acc3 = vx_setzero_f64();

            ପାଇଁ (; i <= len - 2*vstep; i += 2*vstep) {
                v_float32 s0 = v_and(v_abs(vx_load(src + i)),
                                     v_reinterpret_as_f32(v_normmask_u32(mask + i)));
                v_float32 s1 = v_and(v_abs(vx_load(src + i + vstep)),
                                     v_reinterpret_as_f32(v_normmask_u32(mask + i + vstep)));
                acc0 = v_add(acc0, v_cvt_f64(s0));
                acc1 = v_add(acc1, v_cvt_f64_ଉପର(s0));
                acc2 = v_add(acc2, v_cvt_f64(s1));
                acc3 = v_add(acc3, v_cvt_f64_ଉପର(s1));
            }
            ପାଇଁ (; i <= len - vstep; i += vstep) {
                v_float32 s = v_and(v_abs(vx_load(src + i)),
                                    v_reinterpret_as_f32(v_normmask_u32(mask + i)));
                acc0 = v_add(acc0, v_cvt_f64(s));
                acc1 = v_add(acc1, v_cvt_f64_ଉପର(s));
            }
            result = v_reduce_sum(v_add(v_add(acc0, acc1), v_add(acc2, acc3)));

            ପାଇଁ (; i < len; i++) {
                ଯଦି (mask[i])
                    result += std::abs(src[i]);
            }
        }
        ଅନ୍ୟଥା {
            ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < len; i++) {
                ଯଦି (mask[i]) {
                    ଅଚଳ ଦଶମିକ* elem = src + i * cn;
                    ପୂର୍ଣ୍ଣସଂଖ୍ୟା k = 0;
                    ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା vstep = VTraits<v_float32>::vlanes();
                    v_float64 acc = vx_setzero_f64();

                    ପାଇଁ (; k <= cn - vstep; k += vstep) {
                        v_float32 s = v_abs(vx_load(elem + k));
                        acc = v_add(acc, v_cvt_f64(s));
                        acc = v_add(acc, v_cvt_f64_ଉପର(s));
                    }

                    result += v_reduce_sum(acc);

                    ପାଇଁ (; k < cn; k++)
                        result += std::abs(elem[k]);
                }
            }
        }
        ଫେରନ୍ତୁ result;
    }
};

template <>
ସଂରଚନା MaskedNormL2_SIMD<ଦଶମିକ, ଦ୍ୱିଗୁଣ> {
    ଧାଡ଼ିଭିତରେ ଦ୍ୱିଗୁଣ operator()(ଅଚଳ ଦଶମିକ* src, ଅଚଳ uchar* mask, ପୂର୍ଣ୍ଣସଂଖ୍ୟା len, ପୂର୍ଣ୍ଣସଂଖ୍ୟା cn) ଅଚଳ {
        ଦ୍ୱିଗୁଣ result = 0.0;
        ଯଦି (cn == 1) {
            ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0;
            ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା vstep = VTraits<v_float32>::vlanes();
            v_float32 facc = vx_setzero_f32();
            v_float64 dacc = vx_setzero_f64();
            ପୂର୍ଣ୍ଣସଂଖ୍ୟା flush = 0;

            ପାଇଁ (; i <= len - vstep; i += vstep, flush += vstep) {
                ଯଦି (flush >= 64) {
                    dacc = v_add(dacc, v_cvt_f64(facc));
                    dacc = v_add(dacc, v_cvt_f64_ଉପର(facc));
                    facc = vx_setzero_f32();
                    flush = 0;
                }
                v_uint32 cmp = v_gt(vx_load_expand_q(mask + i), vx_setzero_u32());
                v_float32 s  = v_reinterpret_as_f32(v_and(v_reinterpret_as_u32(vx_load(src + i)), cmp));
                facc = v_add(facc, v_mul(s, s));
            }

            dacc = v_add(dacc, v_cvt_f64(facc));
            dacc = v_add(dacc, v_cvt_f64_ଉପର(facc));
            result = v_reduce_sum(dacc);

            ପାଇଁ (; i < len; i++) {
                ଯଦି (mask[i]) {
                    ଦ୍ୱିଗୁଣ v = src[i];
                    result += v * v;
                }
            }
        }
        ଅନ୍ୟଥା {
            ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < len; i++) {
                ଯଦି (mask[i]) {
                    ଅଚଳ ଦଶମିକ* elem = src + i * cn;
                    ପୂର୍ଣ୍ଣସଂଖ୍ୟା k = 0;
                    ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା vstep = VTraits<v_float32>::vlanes();
                    v_float32 facc = vx_setzero_f32();

                    ପାଇଁ (; k <= cn - vstep; k += vstep) {
                        v_float32 s = vx_load(elem + k);
                        facc = v_add(facc, v_mul(s, s));
                    }

                    v_float64 dacc = v_add(v_cvt_f64(facc), v_cvt_f64_ଉପର(facc));
                    result += v_reduce_sum(dacc);

                    ପାଇଁ (; k < cn; k++) {
                        ଦ୍ୱିଗୁଣ v = elem[k];
                        result += v * v;
                    }
                }
            }
        }
        ଫେରନ୍ତୁ result;
    }
};

#endif

template <>
ସଂରଚନା MaskedNormInf_SIMD<uchar, ପୂର୍ଣ୍ଣସଂଖ୍ୟା> {
    ଧାଡ଼ିଭିତରେ ପୂର୍ଣ୍ଣସଂଖ୍ୟା operator()(ଅଚଳ uchar* src, ଅଚଳ uchar* mask, ପୂର୍ଣ୍ଣସଂଖ୍ୟା len, ପୂର୍ଣ୍ଣସଂଖ୍ୟା cn) ଅଚଳ {
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା result = 0;
        ଯଦି (cn == 1) {
            ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0;
            ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା vstep = VTraits<v_uint8>::vlanes();
            v_uint8 acc = vx_setzero_u8();

            ପାଇଁ (; i <= len - vstep; i += vstep) {
                v_uint8 m   = vx_load(mask + i);
                v_uint8 s   = vx_load(src + i);
                v_uint8 sel = v_and(s, v_gt(m, vx_setzero_u8()));
                acc = v_max(acc, sel);
            }

            result = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)v_reduce_max(acc);

            ପାଇଁ (; i < len; i++) {
                ଯଦି (mask[i])
                    result = std::max(result, (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)src[i]);
            }
        }
        ଅନ୍ୟଥା ଯଦି (cn == 4 && len >= VTraits<v_uint8>::vlanes()) {
            ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା vstep = VTraits<v_uint8>::vlanes();
            v_uint8 acc = vx_setzero_u8();
            ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0;
            ପାଇଁ (;;) {
                ଯଦି (i > len - vstep)
                    i = len - vstep; // back-step (max is idempotent)
                v_uint8 c0, c1, c2, c3;
                v_load_deinterleave(src + i * 4, c0, c1, c2, c3);
                v_uint8 pmax = v_max(v_max(c0, c1), v_max(c2, c3));
                v_uint8 m = v_gt(vx_load(mask + i), vx_setzero_u8());
                acc = v_max(acc, v_and(pmax, m));
                ଯଦି (i >= len - vstep)
                    ଭାଙ୍ଗ;
                i += vstep;
            }
            result = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)v_reduce_max(acc);
        }
        ଅନ୍ୟଥା ଯଦି (cn == 4) {
            ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < len; i++) {
                ଯଦି (mask[i]) {
                    ଅଚଳ uchar* elem = src + i * 4;
                    result = std::max(result, (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)std::max(std::max(elem[0], elem[1]),
                                                            std::max(elem[2], elem[3])));
                }
            }
        }
        ଅନ୍ୟଥା {
            ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < len; i++) {
                ଯଦି (mask[i]) {
                    ଅଚଳ uchar* elem = src + i * cn;
                    ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା k = 0; k < cn; k++)
                        result = std::max(result, (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)elem[k]);
                }
            }
        }
        ଫେରନ୍ତୁ result;
    }
};

template <>
ସଂରଚନା MaskedNormL1_SIMD<uchar, ପୂର୍ଣ୍ଣସଂଖ୍ୟା> {
    ଧାଡ଼ିଭିତରେ ପୂର୍ଣ୍ଣସଂଖ୍ୟା operator()(ଅଚଳ uchar* src, ଅଚଳ uchar* mask, ପୂର୍ଣ୍ଣସଂଖ୍ୟା len, ପୂର୍ଣ୍ଣସଂଖ୍ୟା cn) ଅଚଳ {
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା result = 0;
        ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା vstep = VTraits<v_uint8>::vlanes();
        ଅଚଳ v_uint8 one = vx_setall_u8(1);
        ଯଦି (cn == 1) {
            ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0;
            v_uint32 acc = vx_setzero_u32();
            ପାଇଁ (; i <= len - vstep; i += vstep) {
                v_uint8 s = v_and(vx_load(src + i), v_normmask_u8(mask + i));
                acc = v_dotprod_expand_fast(s, one, acc);
            }
            result = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)v_reduce_sum(acc);
            ପାଇଁ (; i < len; i++) {
                ଯଦି (mask[i])
                    result += src[i];
            }
        }
        ଅନ୍ୟଥା ଯଦି (cn == 4) {
            ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0;
            v_uint32 acc = vx_setzero_u32();
            ପାଇଁ (; i <= len - vstep; i += vstep) {
                v_uint8 c0, c1, c2, c3;
                v_load_deinterleave(src + i * 4, c0, c1, c2, c3);
                v_uint8 m = v_normmask_u8(mask + i);
                acc = v_dotprod_expand_fast(v_and(c0, m), one, acc);
                acc = v_dotprod_expand_fast(v_and(c1, m), one, acc);
                acc = v_dotprod_expand_fast(v_and(c2, m), one, acc);
                acc = v_dotprod_expand_fast(v_and(c3, m), one, acc);
            }
            result = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)v_reduce_sum(acc);
            ପାଇଁ (; i < len; i++) {
                ଯଦି (mask[i]) {
                    ଅଚଳ uchar* elem = src + i * 4;
                    result += elem[0] + elem[1] + elem[2] + elem[3];
                }
            }
        }
        ଅନ୍ୟଥା {
            ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < len; i++) {
                ଯଦି (mask[i]) {
                    ଅଚଳ uchar* elem = src + i * cn;
                    ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା k = 0; k < cn; k++)
                        result += elem[k];
                }
            }
        }
        ଫେରନ୍ତୁ result;
    }
};

template <>
ସଂରଚନା MaskedNormInf_SIMD<ushort, ପୂର୍ଣ୍ଣସଂଖ୍ୟା> {
    ଧାଡ଼ିଭିତରେ ପୂର୍ଣ୍ଣସଂଖ୍ୟା operator()(ଅଚଳ ushort* src, ଅଚଳ uchar* mask, ପୂର୍ଣ୍ଣସଂଖ୍ୟା len, ପୂର୍ଣ୍ଣସଂଖ୍ୟା cn) ଅଚଳ {
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା result = 0;
        ଯଦି (cn == 1) {
            ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0;
            ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା vstep = VTraits<v_uint16>::vlanes();
            v_uint16 acc = vx_setzero_u16();

            ପାଇଁ (; i <= len - vstep; i += vstep) {
                v_uint16 m   = vx_load_expand(mask + i);
                v_uint16 cmp = v_gt(m, vx_setzero_u16());
                v_uint16 s   = vx_load(src + i);
                v_uint16 sel = v_and(s, cmp);
                acc = v_max(acc, sel);
            }

            result = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)v_reduce_max(acc);

            ପାଇଁ (; i < len; i++) {
                ଯଦି (mask[i])
                    result = std::max(result, (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)src[i]);
            }
        }
        ଅନ୍ୟଥା {
            ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < len; i++) {
                ଯଦି (mask[i]) {
                    ଅଚଳ ushort* elem = src + i * cn;
                    ପୂର୍ଣ୍ଣସଂଖ୍ୟା k = 0;
                    ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା vstep = VTraits<v_uint16>::vlanes();
                    v_uint16 acc = vx_setzero_u16();

                    ପାଇଁ (; k <= cn - vstep; k += vstep) {
                        acc = v_max(acc, vx_load(elem + k));
                    }

                    result = std::max(result, (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)v_reduce_max(acc));

                    ପାଇଁ (; k < cn; k++)
                        result = std::max(result, (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)elem[k]);
                }
            }
        }
        ଫେରନ୍ତୁ result;
    }
};

template <>
ସଂରଚନା MaskedNormL1_SIMD<ushort, ପୂର୍ଣ୍ଣସଂଖ୍ୟା> {
    ଧାଡ଼ିଭିତରେ ପୂର୍ଣ୍ଣସଂଖ୍ୟା operator()(ଅଚଳ ushort* src, ଅଚଳ uchar* mask, ପୂର୍ଣ୍ଣସଂଖ୍ୟା len, ପୂର୍ଣ୍ଣସଂଖ୍ୟା cn) ଅଚଳ {
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା result = 0;
        ଯଦି (cn == 1) {
            ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0;
            ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା vstep = VTraits<v_uint16>::vlanes();
            v_uint32 acc32 = vx_setzero_u32();
            v_uint64 acc64 = vx_setzero_u64();
            ପୂର୍ଣ୍ଣସଂଖ୍ୟା acc32_elems = 0;

            ପାଇଁ (; i <= len - vstep; i += vstep, acc32_elems += vstep) {
                ଯଦି (acc32_elems >= 512) {
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
            result = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)v_reduce_sum(acc64);

            ପାଇଁ (; i < len; i++) {
                ଯଦି (mask[i])
                    result += src[i];
            }
        }
        ଅନ୍ୟଥା {
            ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < len; i++) {
                ଯଦି (mask[i]) {
                    ଅଚଳ ushort* elem = src + i * cn;
                    ପୂର୍ଣ୍ଣସଂଖ୍ୟା k = 0;
                    ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା vstep = VTraits<v_uint16>::vlanes();
                    v_uint32 acc = vx_setzero_u32();

                    ପାଇଁ (; k <= cn - vstep; k += vstep) {
                        v_uint32 lo32, hi32;
                        v_expand(vx_load(elem + k), lo32, hi32);
                        acc = v_add(acc, v_add(lo32, hi32));
                    }

                    result += (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)v_reduce_sum(acc);

                    ପାଇଁ (; k < cn; k++)
                        result += elem[k];
                }
            }
        }
        ଫେରନ୍ତୁ result;
    }
};

template <>
ସଂରଚନା MaskedNormL2_SIMD<ushort, ଦ୍ୱିଗୁଣ> {
    ଧାଡ଼ିଭିତରେ ଦ୍ୱିଗୁଣ operator()(ଅଚଳ ushort* src, ଅଚଳ uchar* mask, ପୂର୍ଣ୍ଣସଂଖ୍ୟା len, ପୂର୍ଣ୍ଣସଂଖ୍ୟା cn) ଅଚଳ {
        ଦ୍ୱିଗୁଣ result = 0.0;
        ଯଦି (cn == 1) {
            ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0;
            ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା vstep = VTraits<v_uint16>::vlanes();
            v_uint64 acc = vx_setzero_u64();
            ପାଇଁ (; i <= len - vstep; i += vstep) {
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
            result = (ଦ୍ୱିଗୁଣ)v_reduce_sum(acc);
            ପାଇଁ (; i < len; i++) {
                ଯଦି (mask[i]) {
                    ଦ୍ୱିଗୁଣ v = src[i];
                    result += v * v;
                }
            }
        }
        ଅନ୍ୟଥା {
            ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < len; i++) {
                ଯଦି (mask[i]) {
                    ଅଚଳ ushort* elem = src + i * cn;
                    ପୂର୍ଣ୍ଣସଂଖ୍ୟା k = 0;
                    ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା vstep = VTraits<v_uint16>::vlanes();
                    v_uint64 acc = vx_setzero_u64();
                    ପାଇଁ (; k <= cn - vstep; k += vstep) {
                        v_uint32 lo32, hi32;
                        v_expand(vx_load(elem + k), lo32, hi32);
                        v_uint64 lo64a, lo64b, hi64a, hi64b;
                        v_expand(v_mul(lo32, lo32), lo64a, lo64b);
                        v_expand(v_mul(hi32, hi32), hi64a, hi64b);
                        acc = v_add(acc, v_add(v_add(lo64a, lo64b), v_add(hi64a, hi64b)));
                    }

                    result += (ଦ୍ୱିଗୁଣ)v_reduce_sum(acc);

                    ପାଇଁ (; k < cn; k++) {
                        ଦ୍ୱିଗୁଣ v = elem[k];
                        result += v * v;
                    }
                }
            }
        }
        ଫେରନ୍ତୁ result;
    }
};

template <>
ସଂରଚନା MaskedNormInf_SIMD<schar, ପୂର୍ଣ୍ଣସଂଖ୍ୟା> {
    ଧାଡ଼ିଭିତରେ ପୂର୍ଣ୍ଣସଂଖ୍ୟା operator()(ଅଚଳ schar* src, ଅଚଳ uchar* mask, ପୂର୍ଣ୍ଣସଂଖ୍ୟା len, ପୂର୍ଣ୍ଣସଂଖ୍ୟା cn) ଅଚଳ {
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା result = 0;
        ଯଦି (cn == 1) {
            ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା vstep = VTraits<v_int8>::vlanes();
            ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0;
            v_uint8 acc = vx_setzero_u8();
            ପାଇଁ (; i <= len - vstep; i += vstep)
                acc = v_max(acc, v_and(v_abs(vx_load(src + i)), v_normmask_u8(mask + i)));
            result = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)v_reduce_max(acc);
            ପାଇଁ (; i < len; i++)
                ଯଦି (mask[i]) result = std::max(result, cv_abs(src[i]));
        }
        ଅନ୍ୟଥା {
            ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < len; i++)
                ଯଦି (mask[i]) {
                    ଅଚଳ schar* elem = src + i * cn;
                    ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା k = 0; k < cn; k++)
                        result = std::max(result, cv_abs(elem[k]));
                }
        }
        ଫେରନ୍ତୁ result;
    }
};

template <>
ସଂରଚନା MaskedNormL1_SIMD<schar, ପୂର୍ଣ୍ଣସଂଖ୍ୟା> {
    ଧାଡ଼ିଭିତରେ ପୂର୍ଣ୍ଣସଂଖ୍ୟା operator()(ଅଚଳ schar* src, ଅଚଳ uchar* mask, ପୂର୍ଣ୍ଣସଂଖ୍ୟା len, ପୂର୍ଣ୍ଣସଂଖ୍ୟା cn) ଅଚଳ {
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା result = 0;
        ଯଦି (cn == 1) {
            ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା vstep = VTraits<v_int8>::vlanes();
            ଅଚଳ v_uint8 one = vx_setall_u8(1);
            ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0;
            v_uint32 acc = vx_setzero_u32();
            ପାଇଁ (; i <= len - vstep; i += vstep)
                acc = v_dotprod_expand_fast(v_and(v_abs(vx_load(src + i)), v_normmask_u8(mask + i)), one, acc);
            result = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)v_reduce_sum(acc);
            ପାଇଁ (; i < len; i++)
                ଯଦି (mask[i]) result += cv_abs(src[i]);
        }
        ଅନ୍ୟଥା {
            ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < len; i++)
                ଯଦି (mask[i]) {
                    ଅଚଳ schar* elem = src + i * cn;
                    ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା k = 0; k < cn; k++)
                        result += cv_abs(elem[k]);
                }
        }
        ଫେରନ୍ତୁ result;
    }
};

template <>
ସଂରଚନା MaskedNormL2_SIMD<schar, ପୂର୍ଣ୍ଣସଂଖ୍ୟା> {
    ଧାଡ଼ିଭିତରେ ପୂର୍ଣ୍ଣସଂଖ୍ୟା operator()(ଅଚଳ schar* src, ଅଚଳ uchar* mask, ପୂର୍ଣ୍ଣସଂଖ୍ୟା len, ପୂର୍ଣ୍ଣସଂଖ୍ୟା cn) ଅଚଳ {
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା result = 0;
        ଯଦି (cn == 1) {
            ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା vstep = VTraits<v_int8>::vlanes();
            ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0;
            v_int32 acc = vx_setzero_s32();
            ପାଇଁ (; i <= len - vstep; i += vstep) {
                v_int8 s = v_and(vx_load(src + i), v_reinterpret_as_s8(v_normmask_u8(mask + i)));
                acc = v_dotprod_expand_fast(s, s, acc);
            }
            result = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)v_reduce_sum(acc);
            ପାଇଁ (; i < len; i++)
                ଯଦି (mask[i]) { ପୂର୍ଣ୍ଣସଂଖ୍ୟା v = src[i]; result += v * v; }
        }
        ଅନ୍ୟଥା {
            ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < len; i++)
                ଯଦି (mask[i]) {
                    ଅଚଳ schar* elem = src + i * cn;
                    ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା k = 0; k < cn; k++) { ପୂର୍ଣ୍ଣସଂଖ୍ୟା v = elem[k]; result += v * v; }
                }
        }
        ଫେରନ୍ତୁ result;
    }
};

template <>
ସଂରଚନା MaskedNormL2_SIMD<uchar, ପୂର୍ଣ୍ଣସଂଖ୍ୟା> {
    ଧାଡ଼ିଭିତରେ ପୂର୍ଣ୍ଣସଂଖ୍ୟା operator()(ଅଚଳ uchar* src, ଅଚଳ uchar* mask, ପୂର୍ଣ୍ଣସଂଖ୍ୟା len, ପୂର୍ଣ୍ଣସଂଖ୍ୟା cn) ଅଚଳ {
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା result = 0;
        ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା vstep = VTraits<v_uint8>::vlanes();
        ଯଦି (cn == 1) {
            ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0;
            v_uint32 acc = vx_setzero_u32();
            ପାଇଁ (; i <= len - vstep; i += vstep) {
                v_uint8 s = v_and(vx_load(src + i), v_normmask_u8(mask + i));
                acc = v_dotprod_expand_fast(s, s, acc);
            }
            result = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)v_reduce_sum(acc);
            ପାଇଁ (; i < len; i++)
                ଯଦି (mask[i]) { ପୂର୍ଣ୍ଣସଂଖ୍ୟା v = src[i]; result += v * v; }
        }
        ଅନ୍ୟଥା ଯଦି (cn == 4) {
            ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0;
            v_uint32 acc = vx_setzero_u32();
            ପାଇଁ (; i <= len - vstep; i += vstep) {
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
            result = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)v_reduce_sum(acc);
            ପାଇଁ (; i < len; i++)
                ଯଦି (mask[i]) {
                    ଅଚଳ uchar* elem = src + i * 4;
                    ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା k = 0; k < 4; k++) { ପୂର୍ଣ୍ଣସଂଖ୍ୟା v = elem[k]; result += v * v; }
                }
        }
        ଅନ୍ୟଥା {
            ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < len; i++)
                ଯଦି (mask[i]) {
                    ଅଚଳ uchar* elem = src + i * cn;
                    ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା k = 0; k < cn; k++) { ପୂର୍ଣ୍ଣସଂଖ୍ୟା v = elem[k]; result += v * v; }
                }
        }
        ଫେରନ୍ତୁ result;
    }
};

template <>
ସଂରଚନା MaskedNormInf_SIMD<short, ପୂର୍ଣ୍ଣସଂଖ୍ୟା> {
    ଧାଡ଼ିଭିତରେ ପୂର୍ଣ୍ଣସଂଖ୍ୟା operator()(ଅଚଳ short* src, ଅଚଳ uchar* mask, ପୂର୍ଣ୍ଣସଂଖ୍ୟା len, ପୂର୍ଣ୍ଣସଂଖ୍ୟା cn) ଅଚଳ {
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା result = 0;
        ଯଦି (cn == 1) {
            ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା vstep = VTraits<v_int16>::vlanes();
            ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0;
            v_uint16 acc = vx_setzero_u16();
            ପାଇଁ (; i <= len - vstep; i += vstep)
                acc = v_max(acc, v_and(v_abs(vx_load(src + i)), v_normmask_u16(mask + i)));
            result = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)v_reduce_max(acc);
            ପାଇଁ (; i < len; i++)
                ଯଦି (mask[i]) result = std::max(result, cv_abs(src[i]));
        }
        ଅନ୍ୟଥା {
            ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < len; i++)
                ଯଦି (mask[i]) {
                    ଅଚଳ short* elem = src + i * cn;
                    ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା k = 0; k < cn; k++)
                        result = std::max(result, cv_abs(elem[k]));
                }
        }
        ଫେରନ୍ତୁ result;
    }
};

template <>
ସଂରଚନା MaskedNormL1_SIMD<short, ପୂର୍ଣ୍ଣସଂଖ୍ୟା> {
    ଧାଡ଼ିଭିତରେ ପୂର୍ଣ୍ଣସଂଖ୍ୟା operator()(ଅଚଳ short* src, ଅଚଳ uchar* mask, ପୂର୍ଣ୍ଣସଂଖ୍ୟା len, ପୂର୍ଣ୍ଣସଂଖ୍ୟା cn) ଅଚଳ {
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା result = 0;
        ଯଦି (cn == 1) {
            ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା vstep = VTraits<v_int16>::vlanes();
            ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0;
            v_uint32 acc0 = vx_setzero_u32(), acc1 = vx_setzero_u32();
            ପାଇଁ (; i <= len - vstep; i += vstep) {
                v_uint16 s = v_and(v_abs(vx_load(src + i)), v_normmask_u16(mask + i));
                v_uint32 lo, hi;
                v_expand(s, lo, hi);
                acc0 = v_add(acc0, lo);
                acc1 = v_add(acc1, hi);
            }
            result = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)v_reduce_sum(v_add(acc0, acc1));
            ପାଇଁ (; i < len; i++)
                ଯଦି (mask[i]) result += cv_abs(src[i]);
        }
        ଅନ୍ୟଥା {
            ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < len; i++)
                ଯଦି (mask[i]) {
                    ଅଚଳ short* elem = src + i * cn;
                    ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା k = 0; k < cn; k++)
                        result += cv_abs(elem[k]);
                }
        }
        ଫେରନ୍ତୁ result;
    }
};

template <>
ସଂରଚନା MaskedNormL2_SIMD<short, ଦ୍ୱିଗୁଣ> {
    ଧାଡ଼ିଭିତରେ ଦ୍ୱିଗୁଣ operator()(ଅଚଳ short* src, ଅଚଳ uchar* mask, ପୂର୍ଣ୍ଣସଂଖ୍ୟା len, ପୂର୍ଣ୍ଣସଂଖ୍ୟା cn) ଅଚଳ {
        ଦ୍ୱିଗୁଣ result = 0.0;
        ଯଦି (cn == 1) {
            ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା vstep = VTraits<v_int16>::vlanes();
            ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0;
            v_uint64 acc = vx_setzero_u64();
            ପାଇଁ (; i <= len - vstep; i += vstep) {
                v_uint16 s = v_and(v_abs(vx_load(src + i)), v_normmask_u16(mask + i));
                acc = v_add(acc, v_dotprod_expand_fast(s, s));
            }
            result = (ଦ୍ୱିଗୁଣ)v_reduce_sum(acc);
            ପାଇଁ (; i < len; i++)
                ଯଦି (mask[i]) { ଦ୍ୱିଗୁଣ v = src[i]; result += v * v; }
        }
        ଅନ୍ୟଥା {
            ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < len; i++)
                ଯଦି (mask[i]) {
                    ଅଚଳ short* elem = src + i * cn;
                    ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା k = 0; k < cn; k++) { ଦ୍ୱିଗୁଣ v = elem[k]; result += v * v; }
                }
        }
        ଫେରନ୍ତୁ result;
    }
};

template <>
ସଂରଚନା MaskedNormInf_SIMD<ପୂର୍ଣ୍ଣସଂଖ୍ୟା, unsigned> {
    ଧାଡ଼ିଭିତରେ unsigned operator()(ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା* src, ଅଚଳ uchar* mask, ପୂର୍ଣ୍ଣସଂଖ୍ୟା len, ପୂର୍ଣ୍ଣସଂଖ୍ୟା cn) ଅଚଳ {
        unsigned result = 0;
        ଯଦି (cn == 1) {
            ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା vstep = VTraits<v_int32>::vlanes();
            ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0;
            v_uint32 acc = vx_setzero_u32();
            ପାଇଁ (; i <= len - vstep; i += vstep)
                acc = v_max(acc, v_and(v_abs(vx_load(src + i)), v_normmask_u32(mask + i)));
            result = v_reduce_max(acc);
            ପାଇଁ (; i < len; i++)
                ଯଦି (mask[i]) result = std::max(result, cv_abs(src[i]));
        }
        ଅନ୍ୟଥା {
            ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < len; i++)
                ଯଦି (mask[i]) {
                    ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା* elem = src + i * cn;
                    ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା k = 0; k < cn; k++)
                        result = std::max(result, cv_abs(elem[k]));
                }
        }
        ଫେରନ୍ତୁ result;
    }
};

#ଯଦି CV_SIMD_64F || CV_SIMD_SCALABLE_64F

template <>
ସଂରଚନା MaskedNormL1_SIMD<ପୂର୍ଣ୍ଣସଂଖ୍ୟା, ଦ୍ୱିଗୁଣ> {
    ଧାଡ଼ିଭିତରେ ଦ୍ୱିଗୁଣ operator()(ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା* src, ଅଚଳ uchar* mask, ପୂର୍ଣ୍ଣସଂଖ୍ୟା len, ପୂର୍ଣ୍ଣସଂଖ୍ୟା cn) ଅଚଳ {
        ଦ୍ୱିଗୁଣ result = 0.0;
        ଯଦି (cn == 1) {
            ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା vstep = VTraits<v_int32>::vlanes();
            ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0;
            v_float64 acc0 = vx_setzero_f64(), acc1 = vx_setzero_f64();
            ପାଇଁ (; i <= len - vstep; i += vstep) {
                // Mask the value, then widen and take the magnitude in double. v_abs() does give
                // |INT_MIN| as an unsigned 2147483648, but reinterpreting that back as int32 turns
                // it negative again, and the widening then carries the sign into the accumulator.
                v_int32 s = v_reinterpret_as_s32(v_and(v_reinterpret_as_u32(vx_load(src + i)),
                                                       v_normmask_u32(mask + i)));
                acc0 = v_add(acc0, v_abs(v_cvt_f64(s)));
                acc1 = v_add(acc1, v_abs(v_cvt_f64_ଉପର(s)));
            }
            result = v_reduce_sum(v_add(acc0, acc1));
            ପାଇଁ (; i < len; i++)
                ଯଦି (mask[i]) result += cv_abs(src[i]);
        }
        ଅନ୍ୟଥା {
            ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < len; i++)
                ଯଦି (mask[i]) {
                    ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା* elem = src + i * cn;
                    ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା k = 0; k < cn; k++)
                        result += cv_abs(elem[k]);
                }
        }
        ଫେରନ୍ତୁ result;
    }
};

template <>
ସଂରଚନା MaskedNormL2_SIMD<ପୂର୍ଣ୍ଣସଂଖ୍ୟା, ଦ୍ୱିଗୁଣ> {
    ଧାଡ଼ିଭିତରେ ଦ୍ୱିଗୁଣ operator()(ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା* src, ଅଚଳ uchar* mask, ପୂର୍ଣ୍ଣସଂଖ୍ୟା len, ପୂର୍ଣ୍ଣସଂଖ୍ୟା cn) ଅଚଳ {
        ଦ୍ୱିଗୁଣ result = 0.0;
        ଯଦି (cn == 1) {
            ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା vstep = VTraits<v_int32>::vlanes();
            ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0;
            v_float64 acc0 = vx_setzero_f64(), acc1 = vx_setzero_f64();
            ପାଇଁ (; i <= len - vstep; i += vstep) {
                v_uint32 s = v_and(v_abs(vx_load(src + i)), v_normmask_u32(mask + i));
                v_uint64 e0, e1;
                v_expand(s, e0, e1);
                v_float64 f0 = v_cvt_f64(v_reinterpret_as_s64(e0));
                v_float64 f1 = v_cvt_f64(v_reinterpret_as_s64(e1));
                acc0 = v_fma(f0, f0, acc0);
                acc1 = v_fma(f1, f1, acc1);
            }
            result = v_reduce_sum(v_add(acc0, acc1));
            ପାଇଁ (; i < len; i++)
                ଯଦି (mask[i]) { ଦ୍ୱିଗୁଣ v = src[i]; result += v * v; }
        }
        ଅନ୍ୟଥା {
            ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < len; i++)
                ଯଦି (mask[i]) {
                    ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା* elem = src + i * cn;
                    ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା k = 0; k < cn; k++) { ଦ୍ୱିଗୁଣ v = elem[k]; result += v * v; }
                }
        }
        ଫେରନ୍ତୁ result;
    }
};

template <>
ସଂରଚନା MaskedNormInf_SIMD<ଦ୍ୱିଗୁଣ, ଦ୍ୱିଗୁଣ> {
    ଧାଡ଼ିଭିତରେ ଦ୍ୱିଗୁଣ operator()(ଅଚଳ ଦ୍ୱିଗୁଣ* src, ଅଚଳ uchar* mask, ପୂର୍ଣ୍ଣସଂଖ୍ୟା len, ପୂର୍ଣ୍ଣସଂଖ୍ୟା cn) ଅଚଳ {
        ଦ୍ୱିଗୁଣ result = 0.0;
        ଯଦି (cn == 1) {
            ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା vstep = VTraits<v_float64>::vlanes();
            ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0;
            v_float64 acc0 = vx_setzero_f64(), acc1 = vx_setzero_f64();
            ପାଇଁ (; i <= len - 2*vstep; i += 2*vstep) {
                v_uint64 m0, m1;
                v_normmask_u64(mask + i, m0, m1);
                acc0 = v_max(acc0, v_reinterpret_as_f64(v_and(v_reinterpret_as_u64(v_abs(vx_load(src + i))), m0)));
                acc1 = v_max(acc1, v_reinterpret_as_f64(v_and(v_reinterpret_as_u64(v_abs(vx_load(src + i + vstep))), m1)));
            }
            ଦ୍ୱିଗୁଣ t[VTraits<v_float64>::max_nlanes];
            vx_store(t, v_max(acc0, acc1));
            ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା k = 0; k < vstep; k++)
                result = std::max(result, t[k]);
            ପାଇଁ (; i < len; i++)
                ଯଦି (mask[i]) result = std::max(result, cv_abs(src[i]));
        }
        ଅନ୍ୟଥା {
            ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < len; i++)
                ଯଦି (mask[i]) {
                    ଅଚଳ ଦ୍ୱିଗୁଣ* elem = src + i * cn;
                    ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା k = 0; k < cn; k++)
                        result = std::max(result, cv_abs(elem[k]));
                }
        }
        ଫେରନ୍ତୁ result;
    }
};

template <>
ସଂରଚନା MaskedNormL1_SIMD<ଦ୍ୱିଗୁଣ, ଦ୍ୱିଗୁଣ> {
    ଧାଡ଼ିଭିତରେ ଦ୍ୱିଗୁଣ operator()(ଅଚଳ ଦ୍ୱିଗୁଣ* src, ଅଚଳ uchar* mask, ପୂର୍ଣ୍ଣସଂଖ୍ୟା len, ପୂର୍ଣ୍ଣସଂଖ୍ୟା cn) ଅଚଳ {
        ଦ୍ୱିଗୁଣ result = 0.0;
        ଯଦି (cn == 1) {
            ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା vstep = VTraits<v_float64>::vlanes();
            ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0;
            v_float64 acc0 = vx_setzero_f64(), acc1 = vx_setzero_f64();
            ପାଇଁ (; i <= len - 2*vstep; i += 2*vstep) {
                v_uint64 m0, m1;
                v_normmask_u64(mask + i, m0, m1);
                acc0 = v_add(acc0, v_reinterpret_as_f64(v_and(v_reinterpret_as_u64(v_abs(vx_load(src + i))), m0)));
                acc1 = v_add(acc1, v_reinterpret_as_f64(v_and(v_reinterpret_as_u64(v_abs(vx_load(src + i + vstep))), m1)));
            }
            result = v_reduce_sum(v_add(acc0, acc1));
            ପାଇଁ (; i < len; i++)
                ଯଦି (mask[i]) result += cv_abs(src[i]);
        }
        ଅନ୍ୟଥା {
            ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < len; i++)
                ଯଦି (mask[i]) {
                    ଅଚଳ ଦ୍ୱିଗୁଣ* elem = src + i * cn;
                    ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା k = 0; k < cn; k++)
                        result += cv_abs(elem[k]);
                }
        }
        ଫେରନ୍ତୁ result;
    }
};

template <>
ସଂରଚନା MaskedNormL2_SIMD<ଦ୍ୱିଗୁଣ, ଦ୍ୱିଗୁଣ> {
    ଧାଡ଼ିଭିତରେ ଦ୍ୱିଗୁଣ operator()(ଅଚଳ ଦ୍ୱିଗୁଣ* src, ଅଚଳ uchar* mask, ପୂର୍ଣ୍ଣସଂଖ୍ୟା len, ପୂର୍ଣ୍ଣସଂଖ୍ୟା cn) ଅଚଳ {
        ଦ୍ୱିଗୁଣ result = 0.0;
        ଯଦି (cn == 1) {
            ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା vstep = VTraits<v_float64>::vlanes();
            ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0;
            v_float64 acc0 = vx_setzero_f64(), acc1 = vx_setzero_f64();
            ପାଇଁ (; i <= len - 2*vstep; i += 2*vstep) {
                v_uint64 m0, m1;
                v_normmask_u64(mask + i, m0, m1);
                v_float64 s0 = v_reinterpret_as_f64(v_and(v_reinterpret_as_u64(vx_load(src + i)), m0));
                v_float64 s1 = v_reinterpret_as_f64(v_and(v_reinterpret_as_u64(vx_load(src + i + vstep)), m1));
                acc0 = v_fma(s0, s0, acc0);
                acc1 = v_fma(s1, s1, acc1);
            }
            result = v_reduce_sum(v_add(acc0, acc1));
            ପାଇଁ (; i < len; i++)
                ଯଦି (mask[i]) { ଦ୍ୱିଗୁଣ v = src[i]; result += v * v; }
        }
        ଅନ୍ୟଥା {
            ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < len; i++)
                ଯଦି (mask[i]) {
                    ଅଚଳ ଦ୍ୱିଗୁଣ* elem = src + i * cn;
                    ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା k = 0; k < cn; k++) { ଦ୍ୱିଗୁଣ v = elem[k]; result += v * v; }
                }
        }
        ଫେରନ୍ତୁ result;
    }
};

#endif // CV_SIMD_64F || CV_SIMD_SCALABLE_64F
#endif // CV_SIMD || CV_SIMD_SCALABLE

template<typename T, typename ST> ପୂର୍ଣ୍ଣସଂଖ୍ୟା
normInf_(ଅଚଳ T* src, ଅଚଳ uchar* mask, ST* _result, ପୂର୍ଣ୍ଣସଂଖ୍ୟା len, ପୂର୍ଣ୍ଣସଂଖ୍ୟା cn)
{
    ST result = *_result;
    ଯଦି( !mask )
    {
        NormInf_SIMD<T, ST> op;
        result = std::max(result, op(src, len*cn));
    } ଅନ୍ୟଥା {
        MaskedNormInf_SIMD<T, ST> op;
        result = std::max(result, op(src, mask, len, cn));
    }
    *_result = result;
    ଫେରନ୍ତୁ 0;
}

template<typename T, typename ST> ପୂର୍ଣ୍ଣସଂଖ୍ୟା
normL1_(ଅଚଳ T* src, ଅଚଳ uchar* mask, ST* _result, ପୂର୍ଣ୍ଣସଂଖ୍ୟା len, ପୂର୍ଣ୍ଣସଂଖ୍ୟା cn)
{
    ST result = *_result;
    ଯଦି( !mask )
    {
        NormL1_SIMD<T, ST> op;
        result += op(src, len*cn);
    } ଅନ୍ୟଥା {
        MaskedNormL1_SIMD<T, ST> op;
        result += op(src, mask, len, cn);
    }
    *_result = result;
    ଫେରନ୍ତୁ 0;
}

template<typename T, typename ST> ପୂର୍ଣ୍ଣସଂଖ୍ୟା
normL2_(ଅଚଳ T* src, ଅଚଳ uchar* mask, ST* _result, ପୂର୍ଣ୍ଣସଂଖ୍ୟା len, ପୂର୍ଣ୍ଣସଂଖ୍ୟା cn)
{
    ST result = *_result;
    ଯଦି( !mask )
    {
        NormL2_SIMD<T, ST> op;
        result += op(src, len*cn);
    }
    ଅନ୍ୟଥା
    {
        MaskedNormL2_SIMD<T, ST> op;
        result += op(src, mask, len, cn);
    }
    *_result = result;
    ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
normInf_Bool(ଅଚଳ uchar* src, ଅଚଳ uchar* mask, ପୂର୍ଣ୍ଣସଂଖ୍ୟା* _result, ପୂର୍ଣ୍ଣସଂଖ୍ୟା len, ପୂର୍ଣ୍ଣସଂଖ୍ୟା cn)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା result = *_result;
    ଯଦି( !mask )
    {
        ପାଇଁ ( ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < len*cn; i++ ) {
            result = std::max(result, (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)(src[i] != 0));
            ଯଦି (result != 0)
                ଭାଙ୍ଗ;
        }
    }
    ଅନ୍ୟଥା
    {
        ପାଇଁ( ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < len; i++, src += cn )
            ଯଦି( mask[i] )
            {
                ପାଇଁ( ପୂର୍ଣ୍ଣସଂଖ୍ୟା k = 0; k < cn; k++ )
                    result = std::max(result, (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)(src[k] != 0));
                ଯଦି (result != 0)
                    ଭାଙ୍ଗ;
            }
    }
    *_result = result;
    ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
normL1_Bool(ଅଚଳ uchar* src, ଅଚଳ uchar* mask, ପୂର୍ଣ୍ଣସଂଖ୍ୟା* _result, ପୂର୍ଣ୍ଣସଂଖ୍ୟା len, ପୂର୍ଣ୍ଣସଂଖ୍ୟା cn)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା result = *_result;
    ଯଦି( !mask )
    {
        ପାଇଁ ( ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < len*cn; i++ )
            result += (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)(src[i] != 0);
    }
    ଅନ୍ୟଥା
    {
        ପାଇଁ( ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < len; i++, src += cn )
            ଯଦି( mask[i] )
            {
                ପାଇଁ( ପୂର୍ଣ୍ଣସଂଖ୍ୟା k = 0; k < cn; k++ )
                    result += (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)(src[k] != 0);
            }
    }
    *_result = result;
    ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
normL2_Bool(ଅଚଳ uchar* src, ଅଚଳ uchar* mask, ପୂର୍ଣ୍ଣସଂଖ୍ୟା* _result, ପୂର୍ଣ୍ଣସଂଖ୍ୟା len, ପୂର୍ଣ୍ଣସଂଖ୍ୟା cn)
{
    ଫେରନ୍ତୁ normL1_Bool(src, mask, _result, len, cn);
}

// ====================================================================
// Masked norm-diff kernels.
// Scalar base templates (safe for every depth) plus SIMD cn==1
// specializations for the concrete (T, ST) pairs dispatched by
// CV_DEF_NORM_ALL. The masked normDiff*_ functions below select them.
// ====================================================================
template <typename T, typename ST>
ସଂରଚନା MaskedNormDiffInf_SIMD {
    ଧାଡ଼ିଭିତରେ ST operator()(ଅଚଳ T* s1, ଅଚଳ T* s2, ଅଚଳ uchar* mask, ପୂର୍ଣ୍ଣସଂଖ୍ୟା len, ପୂର୍ଣ୍ଣସଂଖ୍ୟା cn) ଅଚଳ {
        ST result = 0;
        ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < len; i++) ଯଦି (mask[i]) {
            ଅଚଳ T* e1 = s1 + i*cn; ଅଚଳ T* e2 = s2 + i*cn;
            ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା k = 0; k < cn; k++)
                result = std::max(result, (ST)cv_absdiff(e1[k], e2[k]));
        }
        ଫେରନ୍ତୁ result;
    }
};
template <typename T, typename ST>
ସଂରଚନା MaskedNormDiffL1_SIMD {
    ଧାଡ଼ିଭିତରେ ST operator()(ଅଚଳ T* s1, ଅଚଳ T* s2, ଅଚଳ uchar* mask, ପୂର୍ଣ୍ଣସଂଖ୍ୟା len, ପୂର୍ଣ୍ଣସଂଖ୍ୟା cn) ଅଚଳ {
        ST result = 0;
        ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < len; i++) ଯଦି (mask[i]) {
            ଅଚଳ T* e1 = s1 + i*cn; ଅଚଳ T* e2 = s2 + i*cn;
            ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା k = 0; k < cn; k++)
                result += (ST)cv_absdiff(e1[k], e2[k]);
        }
        ଫେରନ୍ତୁ result;
    }
};
template <typename T, typename ST>
ସଂରଚନା MaskedNormDiffL2_SIMD {
    ଧାଡ଼ିଭିତରେ ST operator()(ଅଚଳ T* s1, ଅଚଳ T* s2, ଅଚଳ uchar* mask, ପୂର୍ଣ୍ଣସଂଖ୍ୟା len, ପୂର୍ଣ୍ଣସଂଖ୍ୟା cn) ଅଚଳ {
        ST result = 0;
        ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < len; i++) ଯଦି (mask[i]) {
            ଅଚଳ T* e1 = s1 + i*cn; ଅଚଳ T* e2 = s2 + i*cn;
            ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା k = 0; k < cn; k++) { ST v = (ST)e1[k] - (ST)e2[k]; result += v*v; }
        }
        ଫେରନ୍ତୁ result;
    }
};

#ଯଦି (CV_SIMD || CV_SIMD_SCALABLE)
// AND a per-lane diff vector with the (expanded) mask>0 predicate.
ଧାଡ଼ିଭିତରେ v_uint8  v_diffmask(ଅଚଳ v_uint8&  d, ଅଚଳ uchar* m) { ଫେରନ୍ତୁ v_and(d, v_gt(vx_load(m),          vx_setzero_u8())); }
ଧାଡ଼ିଭିତରେ v_uint16 v_diffmask(ଅଚଳ v_uint16& d, ଅଚଳ uchar* m) { ଫେରନ୍ତୁ v_and(d, v_gt(vx_load_expand(m),   vx_setzero_u16())); }
ଧାଡ଼ିଭିତରେ v_uint32 v_diffmask(ଅଚଳ v_uint32& d, ଅଚଳ uchar* m) { ଫେରନ୍ତୁ v_and(d, v_gt(vx_load_expand_q(m), vx_setzero_u32())); }
ଧାଡ଼ିଭିତରେ v_float32 v_diffmask(ଅଚଳ v_float32& d, ଅଚଳ uchar* m) {
    v_uint32 cm = v_gt(vx_load_expand_q(m), vx_setzero_u32());
    ଫେରନ୍ତୁ v_reinterpret_as_f32(v_and(v_reinterpret_as_u32(d), cm));
}

// Shared scalar cn>1 fallbacks (match the base templates' cv_absdiff semantics).
template<typename T, typename RT>
ସ୍ଥିର ଧାଡ଼ିଭିତରେ RT maskedNormDiffInfTail(ଅଚଳ T* s1, ଅଚଳ T* s2, ଅଚଳ uchar* mask, ପୂର୍ଣ୍ଣସଂଖ୍ୟା len, ପୂର୍ଣ୍ଣସଂଖ୍ୟା cn, RT result) {
    ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < len; i++) ଯଦି (mask[i]) {
        ଅଚଳ T* e1 = s1 + i*cn; ଅଚଳ T* e2 = s2 + i*cn;
        ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା k = 0; k < cn; k++) result = std::max(result, (RT)cv_absdiff(e1[k], e2[k]));
    }
    ଫେରନ୍ତୁ result;
}
template<typename T, typename RT>
ସ୍ଥିର ଧାଡ଼ିଭିତରେ RT maskedNormDiffL1Tail(ଅଚଳ T* s1, ଅଚଳ T* s2, ଅଚଳ uchar* mask, ପୂର୍ଣ୍ଣସଂଖ୍ୟା len, ପୂର୍ଣ୍ଣସଂଖ୍ୟା cn, RT result) {
    ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < len; i++) ଯଦି (mask[i]) {
        ଅଚଳ T* e1 = s1 + i*cn; ଅଚଳ T* e2 = s2 + i*cn;
        ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା k = 0; k < cn; k++) result += (RT)cv_absdiff(e1[k], e2[k]);
    }
    ଫେରନ୍ତୁ result;
}
template<typename T, typename RT>
ସ୍ଥିର ଧାଡ଼ିଭିତରେ RT maskedNormDiffL2Tail(ଅଚଳ T* s1, ଅଚଳ T* s2, ଅଚଳ uchar* mask, ପୂର୍ଣ୍ଣସଂଖ୍ୟା len, ପୂର୍ଣ୍ଣସଂଖ୍ୟା cn, RT result) {
    ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < len; i++) ଯଦି (mask[i]) {
        ଅଚଳ T* e1 = s1 + i*cn; ଅଚଳ T* e2 = s2 + i*cn;
        ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା k = 0; k < cn; k++) { RT v = (RT)e1[k] - (RT)e2[k]; result += v*v; }
    }
    ଫେରନ୍ତୁ result;
}

// Inf (cn==1): reduce_max of masked |a-b|. Shared across SIMD-capable depths.
template<typename T, typename ST>
ସ୍ଥିର ଧାଡ଼ିଭିତରେ ST maskedNormDiffInfCn1(ଅଚଳ T* s1, ଅଚଳ T* s2, ଅଚଳ uchar* mask, ପୂର୍ଣ୍ଣସଂଖ୍ୟା len) {
    ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା vstep = VTraits<decltype(vx_load(s1))>::vlanes();
    ST result = 0;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0;
    ଯଦି (len >= vstep) {
        auto acc = v_diffmask(v_absdiff(vx_load(s1), vx_load(s2)), mask);
        ପାଇଁ (i = vstep; i <= len - vstep; i += vstep)
            acc = v_max(acc, v_diffmask(v_absdiff(vx_load(s1 + i), vx_load(s2 + i)), mask + i));
        result = (ST)v_reduce_max(acc);
    }
    ପାଇଁ (; i < len; i++) ଯଦି (mask[i]) result = std::max(result, (ST)cv_absdiff(s1[i], s2[i]));
    ଫେରନ୍ତୁ result;
}

// Shared 8-bit cn==1 masked L1/L2 kernels (uchar/schar): v_absdiff yields v_uint8 for both.
template<typename T>
ସ୍ଥିର ଧାଡ଼ିଭିତରେ ପୂର୍ଣ୍ଣସଂଖ୍ୟା maskedNormDiffL1_8(ଅଚଳ T* s1, ଅଚଳ T* s2, ଅଚଳ uchar* mask, ପୂର୍ଣ୍ଣସଂଖ୍ୟା len) {
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା vstep = VTraits<v_uint8>::vlanes();
    ଅଚଳ v_uint8 one = vx_setall_u8(1);
    v_uint32 acc = vx_setzero_u32();
    ପାଇଁ (; i <= len - vstep; i += vstep) {
        v_uint8 m  = v_gt(vx_load(mask + i), vx_setzero_u8());
        v_uint8 ad = v_and(v_absdiff(vx_load(s1 + i), vx_load(s2 + i)), m);
        acc = v_dotprod_expand_fast(ad, one, acc);
    }
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା result = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)v_reduce_sum(acc);
    ପାଇଁ (; i < len; i++) ଯଦି (mask[i]) result += (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)cv_absdiff(s1[i], s2[i]);
    ଫେରନ୍ତୁ result;
}
template<typename T>
ସ୍ଥିର ଧାଡ଼ିଭିତରେ ପୂର୍ଣ୍ଣସଂଖ୍ୟା maskedNormDiffL2_8(ଅଚଳ T* s1, ଅଚଳ T* s2, ଅଚଳ uchar* mask, ପୂର୍ଣ୍ଣସଂଖ୍ୟା len) {
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା vstep = VTraits<v_uint8>::vlanes();
    v_uint32 acc = vx_setzero_u32();
    ପାଇଁ (; i <= len - vstep; i += vstep) {
        v_uint8 m  = v_gt(vx_load(mask + i), vx_setzero_u8());
        v_uint8 ad = v_and(v_absdiff(vx_load(s1 + i), vx_load(s2 + i)), m);
        acc = v_dotprod_expand_fast(ad, ad, acc);
    }
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା result = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)v_reduce_sum(acc);
    ପାଇଁ (; i < len; i++) ଯଦି (mask[i]) { ପୂର୍ଣ୍ଣସଂଖ୍ୟା v = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)s1[i] - (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)s2[i]; result += v*v; }
    ଫେରନ୍ତୁ result;
}

// Shared 16-bit cn==1 masked L1 kernel (ushort/short): v_absdiff yields v_uint16 for both.
template<typename T>
ସ୍ଥିର ଧାଡ଼ିଭିତରେ ପୂର୍ଣ୍ଣସଂଖ୍ୟା maskedNormDiffL1_16(ଅଚଳ T* s1, ଅଚଳ T* s2, ଅଚଳ uchar* mask, ପୂର୍ଣ୍ଣସଂଖ୍ୟା len) {
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା vstep = VTraits<v_uint16>::vlanes();
    v_uint32 acc = vx_setzero_u32();
    ପାଇଁ (; i <= len - vstep; i += vstep) {
        v_uint16 m  = v_gt(vx_load_expand(mask + i), vx_setzero_u16());
        v_uint16 ad = v_and(v_absdiff(vx_load(s1 + i), vx_load(s2 + i)), m);
        v_uint32 lo, hi; v_expand(ad, lo, hi);
        acc = v_add(acc, v_add(lo, hi));
    }
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା result = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)v_reduce_sum(acc);
    ପାଇଁ (; i < len; i++) ଯଦି (mask[i]) result += (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)cv_absdiff(s1[i], s2[i]);
    ଫେରନ୍ତୁ result;
}

template<> ସଂରଚନା MaskedNormDiffInf_SIMD<uchar, ପୂର୍ଣ୍ଣସଂଖ୍ୟା> {
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା operator()(ଅଚଳ uchar* s1, ଅଚଳ uchar* s2, ଅଚଳ uchar* mask, ପୂର୍ଣ୍ଣସଂଖ୍ୟା len, ପୂର୍ଣ୍ଣସଂଖ୍ୟା cn) ଅଚଳ {
        ଯଦି (cn == 1) ଫେରନ୍ତୁ maskedNormDiffInfCn1<uchar, ପୂର୍ଣ୍ଣସଂଖ୍ୟା>(s1, s2, mask, len);
        ଯଦି (cn == 4 && len >= VTraits<v_uint8>::vlanes()) {
            ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା vstep = VTraits<v_uint8>::vlanes();
            v_uint8 acc = vx_setzero_u8();
            ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0;
            ପାଇଁ (;;) {
                ଯଦି (i > len - vstep) i = len - vstep; // back-step (max is idempotent)
                v_uint8 a0,a1,a2,a3,b0,b1,b2,b3;
                v_load_deinterleave(s1 + i*4, a0,a1,a2,a3);
                v_load_deinterleave(s2 + i*4, b0,b1,b2,b3);
                v_uint8 ad = v_max(v_max(v_absdiff(a0,b0), v_absdiff(a1,b1)),
                                   v_max(v_absdiff(a2,b2), v_absdiff(a3,b3)));
                v_uint8 m = v_gt(vx_load(mask + i), vx_setzero_u8());
                acc = v_max(acc, v_and(ad, m));
                ଯଦି (i >= len - vstep) ଭାଙ୍ଗ;
                i += vstep;
            }
            ଫେରନ୍ତୁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)v_reduce_max(acc);
        }
        ଫେରନ୍ତୁ maskedNormDiffInfTail<uchar, ପୂର୍ଣ୍ଣସଂଖ୍ୟା>(s1, s2, mask, len, cn, 0);
    }
};
template<> ସଂରଚନା MaskedNormDiffInf_SIMD<schar, ପୂର୍ଣ୍ଣସଂଖ୍ୟା> {
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା operator()(ଅଚଳ schar* s1, ଅଚଳ schar* s2, ଅଚଳ uchar* mask, ପୂର୍ଣ୍ଣସଂଖ୍ୟା len, ପୂର୍ଣ୍ଣସଂଖ୍ୟା cn) ଅଚଳ {
        ଯଦି (cn == 1) ଫେରନ୍ତୁ maskedNormDiffInfCn1<schar, ପୂର୍ଣ୍ଣସଂଖ୍ୟା>(s1, s2, mask, len);
        ଫେରନ୍ତୁ maskedNormDiffInfTail<schar, ପୂର୍ଣ୍ଣସଂଖ୍ୟା>(s1, s2, mask, len, cn, 0);
    }
};
template<> ସଂରଚନା MaskedNormDiffInf_SIMD<ushort, ପୂର୍ଣ୍ଣସଂଖ୍ୟା> {
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା operator()(ଅଚଳ ushort* s1, ଅଚଳ ushort* s2, ଅଚଳ uchar* mask, ପୂର୍ଣ୍ଣସଂଖ୍ୟା len, ପୂର୍ଣ୍ଣସଂଖ୍ୟା cn) ଅଚଳ {
        ଯଦି (cn == 1) ଫେରନ୍ତୁ maskedNormDiffInfCn1<ushort, ପୂର୍ଣ୍ଣସଂଖ୍ୟା>(s1, s2, mask, len);
        ଫେରନ୍ତୁ maskedNormDiffInfTail<ushort, ପୂର୍ଣ୍ଣସଂଖ୍ୟା>(s1, s2, mask, len, cn, 0);
    }
};
template<> ସଂରଚନା MaskedNormDiffInf_SIMD<short, ପୂର୍ଣ୍ଣସଂଖ୍ୟା> {
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା operator()(ଅଚଳ short* s1, ଅଚଳ short* s2, ଅଚଳ uchar* mask, ପୂର୍ଣ୍ଣସଂଖ୍ୟା len, ପୂର୍ଣ୍ଣସଂଖ୍ୟା cn) ଅଚଳ {
        ଯଦି (cn == 1) ଫେରନ୍ତୁ maskedNormDiffInfCn1<short, ପୂର୍ଣ୍ଣସଂଖ୍ୟା>(s1, s2, mask, len);
        ଫେରନ୍ତୁ maskedNormDiffInfTail<short, ପୂର୍ଣ୍ଣସଂଖ୍ୟା>(s1, s2, mask, len, cn, 0);
    }
};
template<> ସଂରଚନା MaskedNormDiffInf_SIMD<ପୂର୍ଣ୍ଣସଂଖ୍ୟା, unsigned> {
    unsigned operator()(ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା* s1, ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା* s2, ଅଚଳ uchar* mask, ପୂର୍ଣ୍ଣସଂଖ୍ୟା len, ପୂର୍ଣ୍ଣସଂଖ୍ୟା cn) ଅଚଳ {
        ଯଦି (cn == 1) ଫେରନ୍ତୁ maskedNormDiffInfCn1<ପୂର୍ଣ୍ଣସଂଖ୍ୟା, unsigned>(s1, s2, mask, len);
        ଫେରନ୍ତୁ maskedNormDiffInfTail<ପୂର୍ଣ୍ଣସଂଖ୍ୟା, unsigned>(s1, s2, mask, len, cn, 0u);
    }
};
template<> ସଂରଚନା MaskedNormDiffInf_SIMD<ଦଶମିକ, ଦଶମିକ> {
    ଦଶମିକ operator()(ଅଚଳ ଦଶମିକ* s1, ଅଚଳ ଦଶମିକ* s2, ଅଚଳ uchar* mask, ପୂର୍ଣ୍ଣସଂଖ୍ୟା len, ପୂର୍ଣ୍ଣସଂଖ୍ୟା cn) ଅଚଳ {
        ଯଦି (cn == 1) ଫେରନ୍ତୁ maskedNormDiffInfCn1<ଦଶମିକ, ଦଶମିକ>(s1, s2, mask, len);
        ଫେରନ୍ତୁ maskedNormDiffInfTail<ଦଶମିକ, ଦଶମିକ>(s1, s2, mask, len, cn, 0.0f);
    }
};

template<> ସଂରଚନା MaskedNormDiffL1_SIMD<uchar, ପୂର୍ଣ୍ଣସଂଖ୍ୟା> {
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା operator()(ଅଚଳ uchar* s1, ଅଚଳ uchar* s2, ଅଚଳ uchar* mask, ପୂର୍ଣ୍ଣସଂଖ୍ୟା len, ପୂର୍ଣ୍ଣସଂଖ୍ୟା cn) ଅଚଳ {
        ଯଦି (cn == 1) ଫେରନ୍ତୁ maskedNormDiffL1_8(s1, s2, mask, len);
        ଯଦି (cn == 4) {
            ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା vstep = VTraits<v_uint8>::vlanes();
            ଅଚଳ v_uint8 one = vx_setall_u8(1);
            v_uint32 acc = vx_setzero_u32();
            ପାଇଁ (; i <= len - vstep; i += vstep) {
                v_uint8 a0,a1,a2,a3,b0,b1,b2,b3;
                v_load_deinterleave(s1 + i*4, a0,a1,a2,a3);
                v_load_deinterleave(s2 + i*4, b0,b1,b2,b3);
                v_uint8 m = v_gt(vx_load(mask + i), vx_setzero_u8());
                acc = v_dotprod_expand_fast(v_and(v_absdiff(a0,b0), m), one, acc);
                acc = v_dotprod_expand_fast(v_and(v_absdiff(a1,b1), m), one, acc);
                acc = v_dotprod_expand_fast(v_and(v_absdiff(a2,b2), m), one, acc);
                acc = v_dotprod_expand_fast(v_and(v_absdiff(a3,b3), m), one, acc);
            }
            ପୂର୍ଣ୍ଣସଂଖ୍ୟା result = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)v_reduce_sum(acc);
            ପାଇଁ (; i < len; i++) ଯଦି (mask[i]) {
                ଅଚଳ uchar* e1 = s1 + i*4; ଅଚଳ uchar* e2 = s2 + i*4;
                ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା k = 0; k < 4; k++) result += (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)cv_absdiff(e1[k], e2[k]);
            }
            ଫେରନ୍ତୁ result;
        }
        ଫେରନ୍ତୁ maskedNormDiffL1Tail<uchar, ପୂର୍ଣ୍ଣସଂଖ୍ୟା>(s1, s2, mask, len, cn, 0);
    }
};
template<> ସଂରଚନା MaskedNormDiffL1_SIMD<schar, ପୂର୍ଣ୍ଣସଂଖ୍ୟା> {
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା operator()(ଅଚଳ schar* s1, ଅଚଳ schar* s2, ଅଚଳ uchar* mask, ପୂର୍ଣ୍ଣସଂଖ୍ୟା len, ପୂର୍ଣ୍ଣସଂଖ୍ୟା cn) ଅଚଳ {
        ଯଦି (cn == 1) ଫେରନ୍ତୁ maskedNormDiffL1_8(s1, s2, mask, len);
        ଫେରନ୍ତୁ maskedNormDiffL1Tail<schar, ପୂର୍ଣ୍ଣସଂଖ୍ୟା>(s1, s2, mask, len, cn, 0);
    }
};
template<> ସଂରଚନା MaskedNormDiffL1_SIMD<ushort, ପୂର୍ଣ୍ଣସଂଖ୍ୟା> {
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା operator()(ଅଚଳ ushort* s1, ଅଚଳ ushort* s2, ଅଚଳ uchar* mask, ପୂର୍ଣ୍ଣସଂଖ୍ୟା len, ପୂର୍ଣ୍ଣସଂଖ୍ୟା cn) ଅଚଳ {
        ଯଦି (cn == 1) ଫେରନ୍ତୁ maskedNormDiffL1_16(s1, s2, mask, len);
        ଫେରନ୍ତୁ maskedNormDiffL1Tail<ushort, ପୂର୍ଣ୍ଣସଂଖ୍ୟା>(s1, s2, mask, len, cn, 0);
    }
};
template<> ସଂରଚନା MaskedNormDiffL1_SIMD<short, ପୂର୍ଣ୍ଣସଂଖ୍ୟା> {
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା operator()(ଅଚଳ short* s1, ଅଚଳ short* s2, ଅଚଳ uchar* mask, ପୂର୍ଣ୍ଣସଂଖ୍ୟା len, ପୂର୍ଣ୍ଣସଂଖ୍ୟା cn) ଅଚଳ {
        ଯଦି (cn == 1) ଫେରନ୍ତୁ maskedNormDiffL1_16(s1, s2, mask, len);
        ଫେରନ୍ତୁ maskedNormDiffL1Tail<short, ପୂର୍ଣ୍ଣସଂଖ୍ୟା>(s1, s2, mask, len, cn, 0);
    }
};

template<> ସଂରଚନା MaskedNormDiffL2_SIMD<uchar, ପୂର୍ଣ୍ଣସଂଖ୍ୟା> {
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା operator()(ଅଚଳ uchar* s1, ଅଚଳ uchar* s2, ଅଚଳ uchar* mask, ପୂର୍ଣ୍ଣସଂଖ୍ୟା len, ପୂର୍ଣ୍ଣସଂଖ୍ୟା cn) ଅଚଳ {
        ଯଦି (cn == 1) ଫେରନ୍ତୁ maskedNormDiffL2_8(s1, s2, mask, len);
        ଯଦି (cn == 4) {
            ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା vstep = VTraits<v_uint8>::vlanes();
            v_uint32 acc = vx_setzero_u32();
            ପାଇଁ (; i <= len - vstep; i += vstep) {
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
            ପୂର୍ଣ୍ଣସଂଖ୍ୟା result = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)v_reduce_sum(acc);
            ପାଇଁ (; i < len; i++) ଯଦି (mask[i]) {
                ଅଚଳ uchar* e1 = s1 + i*4; ଅଚଳ uchar* e2 = s2 + i*4;
                ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା k = 0; k < 4; k++) { ପୂର୍ଣ୍ଣସଂଖ୍ୟା v = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)e1[k] - (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)e2[k]; result += v*v; }
            }
            ଫେରନ୍ତୁ result;
        }
        ଫେରନ୍ତୁ maskedNormDiffL2Tail<uchar, ପୂର୍ଣ୍ଣସଂଖ୍ୟା>(s1, s2, mask, len, cn, 0);
    }
};
template<> ସଂରଚନା MaskedNormDiffL2_SIMD<schar, ପୂର୍ଣ୍ଣସଂଖ୍ୟା> {
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା operator()(ଅଚଳ schar* s1, ଅଚଳ schar* s2, ଅଚଳ uchar* mask, ପୂର୍ଣ୍ଣସଂଖ୍ୟା len, ପୂର୍ଣ୍ଣସଂଖ୍ୟା cn) ଅଚଳ {
        ଯଦି (cn == 1) ଫେରନ୍ତୁ maskedNormDiffL2_8(s1, s2, mask, len);
        ଫେରନ୍ତୁ maskedNormDiffL2Tail<schar, ପୂର୍ଣ୍ଣସଂଖ୍ୟା>(s1, s2, mask, len, cn, 0);
    }
};

#ଯଦି CV_SIMD_64F
// Depths whose L1/L2 accumulate in double.
template<> ସଂରଚନା MaskedNormDiffL1_SIMD<ପୂର୍ଣ୍ଣସଂଖ୍ୟା, ଦ୍ୱିଗୁଣ> {
    ଦ୍ୱିଗୁଣ operator()(ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା* s1, ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା* s2, ଅଚଳ uchar* mask, ପୂର୍ଣ୍ଣସଂଖ୍ୟା len, ପୂର୍ଣ୍ଣସଂଖ୍ୟା cn) ଅଚଳ {
        ଯଦି (cn != 1) ଫେରନ୍ତୁ maskedNormDiffL1Tail<ପୂର୍ଣ୍ଣସଂଖ୍ୟା, ଦ୍ୱିଗୁଣ>(s1, s2, mask, len, cn, 0.0);
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା vstep = VTraits<v_int32>::vlanes();
        v_float64 acc = vx_setzero_f64();
        ପାଇଁ (; i <= len - vstep; i += vstep) {
            v_uint32 m  = v_gt(vx_load_expand_q(mask + i), vx_setzero_u32());
            v_uint32 ad = v_and(v_absdiff(vx_load(s1 + i), vx_load(s2 + i)), m);
            v_uint64 e0, e1; v_expand(ad, e0, e1);
            acc = v_add(acc, v_cvt_f64(v_reinterpret_as_s64(e0)));
            acc = v_add(acc, v_cvt_f64(v_reinterpret_as_s64(e1)));
        }
        ଦ୍ୱିଗୁଣ result = v_reduce_sum(acc);
        ପାଇଁ (; i < len; i++) ଯଦି (mask[i]) result += (ଦ୍ୱିଗୁଣ)cv_absdiff(s1[i], s2[i]);
        ଫେରନ୍ତୁ result;
    }
};
template<> ସଂରଚନା MaskedNormDiffL1_SIMD<ଦଶମିକ, ଦ୍ୱିଗୁଣ> {
    ଦ୍ୱିଗୁଣ operator()(ଅଚଳ ଦଶମିକ* s1, ଅଚଳ ଦଶମିକ* s2, ଅଚଳ uchar* mask, ପୂର୍ଣ୍ଣସଂଖ୍ୟା len, ପୂର୍ଣ୍ଣସଂଖ୍ୟା cn) ଅଚଳ {
        ଯଦି (cn != 1) ଫେରନ୍ତୁ maskedNormDiffL1Tail<ଦଶମିକ, ଦ୍ୱିଗୁଣ>(s1, s2, mask, len, cn, 0.0);
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା vstep = VTraits<v_float32>::vlanes();
        v_float64 acc0 = vx_setzero_f64(), acc1 = vx_setzero_f64();
        ପାଇଁ (; i <= len - vstep; i += vstep) {
            v_float32 m  = v_reinterpret_as_f32(v_gt(vx_load_expand_q(mask + i), vx_setzero_u32()));
            v_float32 ad = v_and(v_absdiff(vx_load(s1 + i), vx_load(s2 + i)), m);
            acc0 = v_add(acc0, v_cvt_f64(ad));
            acc1 = v_add(acc1, v_cvt_f64_ଉପର(ad));
        }
        ଦ୍ୱିଗୁଣ result = v_reduce_sum(v_add(acc0, acc1));
        ପାଇଁ (; i < len; i++) ଯଦି (mask[i]) result += (ଦ୍ୱିଗୁଣ)cv_absdiff(s1[i], s2[i]);
        ଫେରନ୍ତୁ result;
    }
};

template<typename T>
ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଦ୍ୱିଗୁଣ maskedNormDiffL2_16(ଅଚଳ T* s1, ଅଚଳ T* s2, ଅଚଳ uchar* mask, ପୂର୍ଣ୍ଣସଂଖ୍ୟା len, ପୂର୍ଣ୍ଣସଂଖ୍ୟା cn) {
    ଯଦି (cn != 1) ଫେରନ୍ତୁ maskedNormDiffL2Tail<T, ଦ୍ୱିଗୁଣ>(s1, s2, mask, len, cn, 0.0);
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା vstep = VTraits<v_uint16>::vlanes();
    v_float64 acc = vx_setzero_f64();
    ପାଇଁ (; i <= len - vstep; i += vstep) {
        v_uint16 m  = v_gt(vx_load_expand(mask + i), vx_setzero_u16());
        v_uint16 ad = v_and(v_absdiff(vx_load(s1 + i), vx_load(s2 + i)), m);
        v_uint64 u = v_dotprod_expand_fast(ad, ad);
        acc = v_add(acc, v_cvt_f64(v_reinterpret_as_s64(u)));
    }
    ଦ୍ୱିଗୁଣ result = v_reduce_sum(acc);
    ପାଇଁ (; i < len; i++) ଯଦି (mask[i]) { ଦ୍ୱିଗୁଣ v = (ଦ୍ୱିଗୁଣ)s1[i] - (ଦ୍ୱିଗୁଣ)s2[i]; result += v*v; }
    ଫେରନ୍ତୁ result;
}
template<> ସଂରଚନା MaskedNormDiffL2_SIMD<ushort, ଦ୍ୱିଗୁଣ> {
    ଦ୍ୱିଗୁଣ operator()(ଅଚଳ ushort* s1, ଅଚଳ ushort* s2, ଅଚଳ uchar* mask, ପୂର୍ଣ୍ଣସଂଖ୍ୟା len, ପୂର୍ଣ୍ଣସଂଖ୍ୟା cn) ଅଚଳ
    { ଫେରନ୍ତୁ maskedNormDiffL2_16(s1, s2, mask, len, cn); } };
template<> ସଂରଚନା MaskedNormDiffL2_SIMD<short, ଦ୍ୱିଗୁଣ> {
    ଦ୍ୱିଗୁଣ operator()(ଅଚଳ short* s1, ଅଚଳ short* s2, ଅଚଳ uchar* mask, ପୂର୍ଣ୍ଣସଂଖ୍ୟା len, ପୂର୍ଣ୍ଣସଂଖ୍ୟା cn) ଅଚଳ
    { ଫେରନ୍ତୁ maskedNormDiffL2_16(s1, s2, mask, len, cn); } };

template<> ସଂରଚନା MaskedNormDiffL2_SIMD<ପୂର୍ଣ୍ଣସଂଖ୍ୟା, ଦ୍ୱିଗୁଣ> {
    ଦ୍ୱିଗୁଣ operator()(ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା* s1, ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା* s2, ଅଚଳ uchar* mask, ପୂର୍ଣ୍ଣସଂଖ୍ୟା len, ପୂର୍ଣ୍ଣସଂଖ୍ୟା cn) ଅଚଳ {
        ଯଦି (cn != 1) ଫେରନ୍ତୁ maskedNormDiffL2Tail<ପୂର୍ଣ୍ଣସଂଖ୍ୟା, ଦ୍ୱିଗୁଣ>(s1, s2, mask, len, cn, 0.0);
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା vstep = VTraits<v_int32>::vlanes();
        v_float64 r0 = vx_setzero_f64(), r1 = vx_setzero_f64();
        ପାଇଁ (; i <= len - vstep; i += vstep) {
            v_uint32 m  = v_gt(vx_load_expand_q(mask + i), vx_setzero_u32());
            v_uint32 ad = v_and(v_absdiff(vx_load(s1 + i), vx_load(s2 + i)), m);
            v_uint64 e0, e1; v_expand(ad, e0, e1);
            v_float64 f0 = v_cvt_f64(v_reinterpret_as_s64(e0)), f1 = v_cvt_f64(v_reinterpret_as_s64(e1));
            r0 = v_fma(f0, f0, r0); r1 = v_fma(f1, f1, r1);
        }
        ଦ୍ୱିଗୁଣ result = v_reduce_sum(v_add(r0, r1));
        ପାଇଁ (; i < len; i++) ଯଦି (mask[i]) { ଦ୍ୱିଗୁଣ v = (ଦ୍ୱିଗୁଣ)s1[i] - (ଦ୍ୱିଗୁଣ)s2[i]; result += v*v; }
        ଫେରନ୍ତୁ result;
    }
};
template<> ସଂରଚନା MaskedNormDiffL2_SIMD<ଦଶମିକ, ଦ୍ୱିଗୁଣ> {
    ଦ୍ୱିଗୁଣ operator()(ଅଚଳ ଦଶମିକ* s1, ଅଚଳ ଦଶମିକ* s2, ଅଚଳ uchar* mask, ପୂର୍ଣ୍ଣସଂଖ୍ୟା len, ପୂର୍ଣ୍ଣସଂଖ୍ୟା cn) ଅଚଳ {
        ଯଦି (cn != 1) ଫେରନ୍ତୁ maskedNormDiffL2Tail<ଦଶମିକ, ଦ୍ୱିଗୁଣ>(s1, s2, mask, len, cn, 0.0);
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା vstep = VTraits<v_float32>::vlanes();
        v_float64 r0 = vx_setzero_f64(), r1 = vx_setzero_f64();
        ପାଇଁ (; i <= len - vstep; i += vstep) {
            v_float32 m  = v_reinterpret_as_f32(v_gt(vx_load_expand_q(mask + i), vx_setzero_u32()));
            v_float32 ad = v_and(v_absdiff(vx_load(s1 + i), vx_load(s2 + i)), m);
            v_float64 f0 = v_cvt_f64(ad), f1 = v_cvt_f64_ଉପର(ad);
            r0 = v_fma(f0, f0, r0); r1 = v_fma(f1, f1, r1);
        }
        ଦ୍ୱିଗୁଣ result = v_reduce_sum(v_add(r0, r1));
        ପାଇଁ (; i < len; i++) ଯଦି (mask[i]) { ଦ୍ୱିଗୁଣ v = (ଦ୍ୱିଗୁଣ)s1[i] - (ଦ୍ୱିଗୁଣ)s2[i]; result += v*v; }
        ଫେରନ୍ତୁ result;
    }
};
template<>
ସଂରଚନା MaskedNormDiffInf_SIMD<ଦ୍ୱିଗୁଣ, ଦ୍ୱିଗୁଣ> {
    ଧାଡ଼ିଭିତରେ ଦ୍ୱିଗୁଣ operator()(ଅଚଳ ଦ୍ୱିଗୁଣ* s1, ଅଚଳ ଦ୍ୱିଗୁଣ* s2, ଅଚଳ uchar* mask, ପୂର୍ଣ୍ଣସଂଖ୍ୟା len, ପୂର୍ଣ୍ଣସଂଖ୍ୟା cn) ଅଚଳ {
        ଦ୍ୱିଗୁଣ result = 0.0;
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0;
#ଯଦି CV_SIMD_64F || CV_SIMD_SCALABLE_64F
        ଯଦି (cn == 1) {
            ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା vstep = VTraits<v_float64>::vlanes();
            v_float64 acc0 = vx_setzero_f64(), acc1 = vx_setzero_f64();
            ପାଇଁ (; i <= len - 2*vstep; i += 2*vstep) {
                v_uint64 m0, m1;
                v_normmask_u64(mask + i, m0, m1);
                v_float64 d0 = v_absdiff(vx_load(s1 + i), vx_load(s2 + i));
                v_float64 d1 = v_absdiff(vx_load(s1 + i + vstep), vx_load(s2 + i + vstep));
                acc0 = v_max(acc0, v_reinterpret_as_f64(v_and(v_reinterpret_as_u64(d0), m0)));
                acc1 = v_max(acc1, v_reinterpret_as_f64(v_and(v_reinterpret_as_u64(d1), m1)));
            }
            ଦ୍ୱିଗୁଣ t[VTraits<v_float64>::max_nlanes];
            vx_store(t, v_max(acc0, acc1));
            ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା k = 0; k < vstep; k++)
                result = std::max(result, t[k]);
        }
#endif
        ପାଇଁ (; i < len; i++) ଯଦି (mask[i]) {
            ଅଚଳ ଦ୍ୱିଗୁଣ* e1 = s1 + i*cn; ଅଚଳ ଦ୍ୱିଗୁଣ* e2 = s2 + i*cn;
            ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା k = 0; k < cn; k++) result = std::max(result, std::abs(e1[k] - e2[k]));
        }
        ଫେରନ୍ତୁ result;
    }
};

template<>
ସଂରଚନା MaskedNormDiffL1_SIMD<ଦ୍ୱିଗୁଣ, ଦ୍ୱିଗୁଣ> {
    ଦ୍ୱିଗୁଣ operator()(ଅଚଳ ଦ୍ୱିଗୁଣ* s1, ଅଚଳ ଦ୍ୱିଗୁଣ* s2, ଅଚଳ uchar* mask, ପୂର୍ଣ୍ଣସଂଖ୍ୟା len, ପୂର୍ଣ୍ଣସଂଖ୍ୟା cn) ଅଚଳ {
        ଦ୍ୱିଗୁଣ result = 0.0;
        ଯଦି (cn == 1) {
            ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା vstep = VTraits<v_float64>::vlanes();
            ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0;
            v_float64 acc0 = vx_setzero_f64(), acc1 = vx_setzero_f64();
            ପାଇଁ (; i <= len - 2*vstep; i += 2*vstep) {
                v_uint64 m0, m1;
                v_normmask_u64(mask + i, m0, m1);
                v_float64 d0 = v_absdiff(vx_load(s1 + i), vx_load(s2 + i));
                v_float64 d1 = v_absdiff(vx_load(s1 + i + vstep), vx_load(s2 + i + vstep));
                acc0 = v_add(acc0, v_reinterpret_as_f64(v_and(v_reinterpret_as_u64(d0), m0)));
                acc1 = v_add(acc1, v_reinterpret_as_f64(v_and(v_reinterpret_as_u64(d1), m1)));
            }
            result = v_reduce_sum(v_add(acc0, acc1));
            ପାଇଁ (; i < len; i++) ଯଦି (mask[i]) result += std::abs(s1[i] - s2[i]);
        } ଅନ୍ୟଥା {
            result = maskedNormDiffL1Tail<ଦ୍ୱିଗୁଣ, ଦ୍ୱିଗୁଣ>(s1, s2, mask, len, cn, 0.0);
        }
        ଫେରନ୍ତୁ result;
    }
};

template<>
ସଂରଚନା MaskedNormDiffL2_SIMD<ଦ୍ୱିଗୁଣ, ଦ୍ୱିଗୁଣ> {
    ଦ୍ୱିଗୁଣ operator()(ଅଚଳ ଦ୍ୱିଗୁଣ* s1, ଅଚଳ ଦ୍ୱିଗୁଣ* s2, ଅଚଳ uchar* mask, ପୂର୍ଣ୍ଣସଂଖ୍ୟା len, ପୂର୍ଣ୍ଣସଂଖ୍ୟା cn) ଅଚଳ {
        ଦ୍ୱିଗୁଣ result = 0.0;
        ଯଦି (cn == 1) {
            ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା vstep = VTraits<v_float64>::vlanes();
            ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0;
            v_float64 acc0 = vx_setzero_f64(), acc1 = vx_setzero_f64();
            ପାଇଁ (; i <= len - 2*vstep; i += 2*vstep) {
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
            ପାଇଁ (; i < len; i++) ଯଦି (mask[i]) { ଦ୍ୱିଗୁଣ v = s1[i] - s2[i]; result += v*v; }
        } ଅନ୍ୟଥା {
            result = maskedNormDiffL2Tail<ଦ୍ୱିଗୁଣ, ଦ୍ୱିଗୁଣ>(s1, s2, mask, len, cn, 0.0);
        }
        ଫେରନ୍ତୁ result;
    }
};
#endif // CV_SIMD_64F
#endif // CV_SIMD

template<typename T, typename ST> ପୂର୍ଣ୍ଣସଂଖ୍ୟା
normDiffInf_(ଅଚଳ T* src1, ଅଚଳ T* src2, ଅଚଳ uchar* mask, ST* _result, ପୂର୍ଣ୍ଣସଂଖ୍ୟା len, ପୂର୍ଣ୍ଣସଂଖ୍ୟା cn) {
    ST result = *_result;
    ଯଦି( !mask ) {
        NormDiffInf_SIMD<T, ST> op;
        result = std::max(result, op(src1, src2, len*cn));
    } ଅନ୍ୟଥା {
        MaskedNormDiffInf_SIMD<T, ST> op;
        result = std::max(result, op(src1, src2, mask, len, cn));
    }
    *_result = result;
    ଫେରନ୍ତୁ 0;
}

template<typename T, typename ST> ପୂର୍ଣ୍ଣସଂଖ୍ୟା
normDiffL1_(ଅଚଳ T* src1, ଅଚଳ T* src2, ଅଚଳ uchar* mask, ST* _result, ପୂର୍ଣ୍ଣସଂଖ୍ୟା len, ପୂର୍ଣ୍ଣସଂଖ୍ୟା cn) {
    ST result = *_result;
    ଯଦି( !mask ) {
        NormDiffL1_SIMD<T, ST> op;
        result += op(src1, src2, len*cn);
    } ଅନ୍ୟଥା {
        MaskedNormDiffL1_SIMD<T, ST> op;
        result += op(src1, src2, mask, len, cn);
    }
    *_result = result;
    ଫେରନ୍ତୁ 0;
}

template<typename T, typename ST> ପୂର୍ଣ୍ଣସଂଖ୍ୟା
normDiffL2_(ଅଚଳ T* src1, ଅଚଳ T* src2, ଅଚଳ uchar* mask, ST* _result, ପୂର୍ଣ୍ଣସଂଖ୍ୟା len, ପୂର୍ଣ୍ଣସଂଖ୍ୟା cn) {
    ST result = *_result;
    ଯଦି( !mask ) {
        NormDiffL2_SIMD<T, ST> op;
        result += op(src1, src2, len*cn);
    } ଅନ୍ୟଥା {
        MaskedNormDiffL2_SIMD<T, ST> op;
        result += op(src1, src2, mask, len, cn);
    }
    *_result = result;
    ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
normDiffInf_Bool(ଅଚଳ uchar* src1, ଅଚଳ uchar* src2, ଅଚଳ uchar* mask, ପୂର୍ଣ୍ଣସଂଖ୍ୟା* _result, ପୂର୍ଣ୍ଣସଂଖ୍ୟା len, ପୂର୍ଣ୍ଣସଂଖ୍ୟା cn)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା result = *_result;
    ଯଦି( !mask )
    {
        ପାଇଁ( ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < len*cn; i++ ) {
            result = std::max(result, (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)((src1[i] != 0) != (src2[i] != 0)));
            ଯଦି (result != 0)
                ଭାଙ୍ଗ;
        }
    }
    ଅନ୍ୟଥା
    {
        ପାଇଁ( ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < len; i++, src1 += cn, src2 += cn )
            ଯଦି( mask[i] )
            {
                ପାଇଁ( ପୂର୍ଣ୍ଣସଂଖ୍ୟା k = 0; k < cn; k++ )
                    result = std::max(result, (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)((src1[k] != 0) != (src2[k] != 0)));
                ଯଦି (result != 0)
                    ଭାଙ୍ଗ;
            }
    }
    *_result = result;
    ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
normDiffL1_Bool(ଅଚଳ uchar* src1, ଅଚଳ uchar* src2, ଅଚଳ uchar* mask, ପୂର୍ଣ୍ଣସଂଖ୍ୟା* _result, ପୂର୍ଣ୍ଣସଂଖ୍ୟା len, ପୂର୍ଣ୍ଣସଂଖ୍ୟା cn)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା result = *_result;
    ଯଦି( !mask )
    {
        ପାଇଁ( ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < len*cn; i++ )
            result += (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)((src1[i] != 0) != (src2[i] != 0));
    }
    ଅନ୍ୟଥା
    {
        ପାଇଁ( ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < len; i++, src1 += cn, src2 += cn )
            ଯଦି( mask[i] )
            {
                ପାଇଁ( ପୂର୍ଣ୍ଣସଂଖ୍ୟା k = 0; k < cn; k++ )
                    result += (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)((src1[k] != 0) != (src2[k] != 0));
            }
    }
    *_result = result;
    ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
normDiffL2_Bool(ଅଚଳ uchar* src1, ଅଚଳ uchar* src2, ଅଚଳ uchar* mask, ପୂର୍ଣ୍ଣସଂଖ୍ୟା* _result, ପୂର୍ଣ୍ଣସଂଖ୍ୟା len, ପୂର୍ଣ୍ଣସଂଖ୍ୟା cn)
{
    ଫେରନ୍ତୁ normDiffL1_Bool(src1, src2, mask, _result, len, cn);
}

#define CV_DEF_NORM_FUNC(L, suffix, type, ntype) \
    ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା norm##L##_##suffix(ଅଚଳ type* src, ଅଚଳ uchar* mask, ntype* r, ପୂର୍ଣ୍ଣସଂଖ୍ୟା len, ପୂର୍ଣ୍ଣସଂଖ୍ୟା cn) \
{ CV_INSTRUMENT_REGION(); ଫେରନ୍ତୁ norm##L##_(src, mask, r, len, cn); } \
    ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା normDiff##L##_##suffix(ଅଚଳ type* src1, ଅଚଳ type* src2, \
    ଅଚଳ uchar* mask, ntype* r, ପୂର୍ଣ୍ଣସଂଖ୍ୟା len, ପୂର୍ଣ୍ଣସଂଖ୍ୟା cn) \
{ ଫେରନ୍ତୁ normDiff##L##_(src1, src2, mask, r, (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)len, cn); }

#define CV_DEF_NORM_ALL(suffix, type, inftype, l1type, l2type) \
    CV_DEF_NORM_FUNC(Inf, suffix, type, inftype) \
    CV_DEF_NORM_FUNC(L1, suffix, type, l1type) \
    CV_DEF_NORM_FUNC(L2, suffix, type, l2type)

CV_DEF_NORM_ALL(8u, uchar, ପୂର୍ଣ୍ଣସଂଖ୍ୟା, ପୂର୍ଣ୍ଣସଂଖ୍ୟା, ପୂର୍ଣ୍ଣସଂଖ୍ୟା)
CV_DEF_NORM_ALL(8s, schar, ପୂର୍ଣ୍ଣସଂଖ୍ୟା, ପୂର୍ଣ୍ଣସଂଖ୍ୟା, ପୂର୍ଣ୍ଣସଂଖ୍ୟା)
CV_DEF_NORM_ALL(16u, ushort, ପୂର୍ଣ୍ଣସଂଖ୍ୟା, ପୂର୍ଣ୍ଣସଂଖ୍ୟା, ଦ୍ୱିଗୁଣ)
CV_DEF_NORM_ALL(16s, short, ପୂର୍ଣ୍ଣସଂଖ୍ୟା, ପୂର୍ଣ୍ଣସଂଖ୍ୟା, ଦ୍ୱିଗୁଣ)
CV_DEF_NORM_ALL(32u, unsigned, unsigned, ଦ୍ୱିଗୁଣ, ଦ୍ୱିଗୁଣ)
CV_DEF_NORM_ALL(32s, ପୂର୍ଣ୍ଣସଂଖ୍ୟା, unsigned, ଦ୍ୱିଗୁଣ, ଦ୍ୱିଗୁଣ)
CV_DEF_NORM_ALL(32f, ଦଶମିକ, ଦଶମିକ, ଦ୍ୱିଗୁଣ, ଦ୍ୱିଗୁଣ)
CV_DEF_NORM_ALL(64f, ଦ୍ୱିଗୁଣ, ଦ୍ୱିଗୁଣ, ଦ୍ୱିଗୁଣ, ଦ୍ୱିଗୁଣ)
CV_DEF_NORM_ALL(64u, uint64, uint64, ଦ୍ୱିଗୁଣ, ଦ୍ୱିଗୁଣ)
CV_DEF_NORM_ALL(64s, int64, uint64, ଦ୍ୱିଗୁଣ, ଦ୍ୱିଗୁଣ)
CV_DEF_NORM_ALL(16f, hfloat, ଦଶମିକ, ଦଶମିକ, ଦଶମିକ)
CV_DEF_NORM_ALL(16bf, bfloat, ଦଶମିକ, ଦଶମିକ, ଦଶମିକ)

NormFunc getNormFunc(ପୂର୍ଣ୍ଣସଂଖ୍ୟା normType, ପୂର୍ଣ୍ଣସଂଖ୍ୟା depth)
{
    CV_INSTRUMENT_REGION();

    ସ୍ଥିର NormFunc normTab[3][CV_DEPTH_MAX] =
    {
        {
            normWrap<uchar, ପୂର୍ଣ୍ଣସଂଖ୍ୟା, normInf_8u>,
            normWrap<schar, ପୂର୍ଣ୍ଣସଂଖ୍ୟା, normInf_8s>,
            normWrap<ushort, ପୂର୍ଣ୍ଣସଂଖ୍ୟା, normInf_16u>,
            normWrap<short, ପୂର୍ଣ୍ଣସଂଖ୍ୟା, normInf_16s>,
            normWrap<ପୂର୍ଣ୍ଣସଂଖ୍ୟା, unsigned, normInf_32s>,
            normWrap<ଦଶମିକ, ଦଶମିକ, normInf_32f>,
            normWrap<ଦ୍ୱିଗୁଣ, ଦ୍ୱିଗୁଣ, normInf_64f>,
            normWrap<hfloat, ଦଶମିକ, normInf_16f>,
            normWrap<bfloat, ଦଶମିକ, normInf_16bf>,
            normWrap<uchar, ପୂର୍ଣ୍ଣସଂଖ୍ୟା, normInf_Bool>,
            normWrap<uint64, uint64, normInf_64u>,
            normWrap<int64, uint64, normInf_64s>,
            normWrap<unsigned, unsigned, normInf_32u>,
            0
        },
        {
            normWrap<uchar, ପୂର୍ଣ୍ଣସଂଖ୍ୟା, normL1_8u>,
            normWrap<schar, ପୂର୍ଣ୍ଣସଂଖ୍ୟା, normL1_8s>,
            normWrap<ushort, ପୂର୍ଣ୍ଣସଂଖ୍ୟା, normL1_16u>,
            normWrap<short, ପୂର୍ଣ୍ଣସଂଖ୍ୟା, normL1_16s>,
            normWrap<ପୂର୍ଣ୍ଣସଂଖ୍ୟା, ଦ୍ୱିଗୁଣ, normL1_32s>,
            normWrap<ଦଶମିକ, ଦ୍ୱିଗୁଣ, normL1_32f>,
            normWrap<ଦ୍ୱିଗୁଣ, ଦ୍ୱିଗୁଣ, normL1_64f>,
            normWrap<hfloat, ଦଶମିକ, normL1_16f>,
            normWrap<bfloat, ଦଶମିକ, normL1_16bf>,
            normWrap<uchar, ପୂର୍ଣ୍ଣସଂଖ୍ୟା, normL1_Bool>,
            normWrap<uint64, ଦ୍ୱିଗୁଣ, normL1_64u>,
            normWrap<int64, ଦ୍ୱିଗୁଣ, normL1_64s>,
            normWrap<unsigned, ଦ୍ୱିଗୁଣ, normL1_32u>,
            0
        },
        {
            normWrap<uchar, ପୂର୍ଣ୍ଣସଂଖ୍ୟା, normL2_8u>,
            normWrap<schar, ପୂର୍ଣ୍ଣସଂଖ୍ୟା, normL2_8s>,
            normWrap<ushort, ଦ୍ୱିଗୁଣ, normL2_16u>,
            normWrap<short, ଦ୍ୱିଗୁଣ, normL2_16s>,
            normWrap<ପୂର୍ଣ୍ଣସଂଖ୍ୟା, ଦ୍ୱିଗୁଣ, normL2_32s>,
            normWrap<ଦଶମିକ, ଦ୍ୱିଗୁଣ, normL2_32f>,
            normWrap<ଦ୍ୱିଗୁଣ, ଦ୍ୱିଗୁଣ, normL2_64f>,
            normWrap<hfloat, ଦଶମିକ, normL2_16f>,
            normWrap<bfloat, ଦଶମିକ, normL2_16bf>,
            normWrap<uchar, ପୂର୍ଣ୍ଣସଂଖ୍ୟା, normL2_Bool>,
            normWrap<uint64, ଦ୍ୱିଗୁଣ, normL2_64u>,
            normWrap<int64, ଦ୍ୱିଗୁଣ, normL2_64s>,
            normWrap<unsigned, ଦ୍ୱିଗୁଣ, normL2_32u>,
            0
        }
    };

    ଯଦି (normType >= 3 || normType < 0) ଫେରନ୍ତୁ nullptr;

    ଫେରନ୍ତୁ normTab[normType][depth];
}

NormDiffFunc getNormDiffFunc(ପୂର୍ଣ୍ଣସଂଖ୍ୟା normType, ପୂର୍ଣ୍ଣସଂଖ୍ୟା depth)
{
    ସ୍ଥିର NormDiffFunc normDiffTab[3][CV_DEPTH_MAX] =
    {
        {
            normDiffWrap<uchar, ପୂର୍ଣ୍ଣସଂଖ୍ୟା, normDiffInf_8u>,
            normDiffWrap<schar, ପୂର୍ଣ୍ଣସଂଖ୍ୟା, normDiffInf_8s>,
            normDiffWrap<ushort, ପୂର୍ଣ୍ଣସଂଖ୍ୟା, normDiffInf_16u>,
            normDiffWrap<short, ପୂର୍ଣ୍ଣସଂଖ୍ୟା, normDiffInf_16s>,
            normDiffWrap<ପୂର୍ଣ୍ଣସଂଖ୍ୟା, unsigned, normDiffInf_32s>,
            normDiffWrap<ଦଶମିକ, ଦଶମିକ, normDiffInf_32f>,
            normDiffWrap<ଦ୍ୱିଗୁଣ, ଦ୍ୱିଗୁଣ, normDiffInf_64f>,
            normDiffWrap<hfloat, ଦଶମିକ, normDiffInf_16f>,
            normDiffWrap<bfloat, ଦଶମିକ, normDiffInf_16bf>,
            normDiffWrap<uchar, ପୂର୍ଣ୍ଣସଂଖ୍ୟା, normDiffInf_Bool>,
            normDiffWrap<uint64, uint64, normDiffInf_64u>,
            normDiffWrap<int64, uint64, normDiffInf_64s>,
            normDiffWrap<unsigned, unsigned, normDiffInf_32u>,
            0
        },
        {
            normDiffWrap<uchar, ପୂର୍ଣ୍ଣସଂଖ୍ୟା, normDiffL1_8u>,
            normDiffWrap<schar, ପୂର୍ଣ୍ଣସଂଖ୍ୟା, normDiffL1_8s>,
            normDiffWrap<ushort, ପୂର୍ଣ୍ଣସଂଖ୍ୟା, normDiffL1_16u>,
            normDiffWrap<short, ପୂର୍ଣ୍ଣସଂଖ୍ୟା, normDiffL1_16s>,
            normDiffWrap<ପୂର୍ଣ୍ଣସଂଖ୍ୟା, ଦ୍ୱିଗୁଣ, normDiffL1_32s>,
            normDiffWrap<ଦଶମିକ, ଦ୍ୱିଗୁଣ, normDiffL1_32f>,
            normDiffWrap<ଦ୍ୱିଗୁଣ, ଦ୍ୱିଗୁଣ, normDiffL1_64f>,
            normDiffWrap<hfloat, ଦଶମିକ, normDiffL1_16f>,
            normDiffWrap<bfloat, ଦଶମିକ, normDiffL1_16bf>,
            normDiffWrap<uchar, ପୂର୍ଣ୍ଣସଂଖ୍ୟା, normDiffL1_Bool>,
            normDiffWrap<uint64, ଦ୍ୱିଗୁଣ, normDiffL1_64u>,
            normDiffWrap<int64, ଦ୍ୱିଗୁଣ, normDiffL1_64s>,
            normDiffWrap<unsigned, ଦ୍ୱିଗୁଣ, normDiffL1_32u>,
            0
        },
        {
            normDiffWrap<uchar, ପୂର୍ଣ୍ଣସଂଖ୍ୟା, normDiffL2_8u>,
            normDiffWrap<schar, ପୂର୍ଣ୍ଣସଂଖ୍ୟା, normDiffL2_8s>,
            normDiffWrap<ushort, ଦ୍ୱିଗୁଣ, normDiffL2_16u>,
            normDiffWrap<short, ଦ୍ୱିଗୁଣ, normDiffL2_16s>,
            normDiffWrap<ପୂର୍ଣ୍ଣସଂଖ୍ୟା, ଦ୍ୱିଗୁଣ, normDiffL2_32s>,
            normDiffWrap<ଦଶମିକ, ଦ୍ୱିଗୁଣ, normDiffL2_32f>,
            normDiffWrap<ଦ୍ୱିଗୁଣ, ଦ୍ୱିଗୁଣ, normDiffL2_64f>,
            normDiffWrap<hfloat, ଦଶମିକ, normDiffL2_16f>,
            normDiffWrap<bfloat, ଦଶମିକ, normDiffL2_16bf>,
            normDiffWrap<uchar, ପୂର୍ଣ୍ଣସଂଖ୍ୟା, normDiffL2_Bool>,
            normDiffWrap<uint64, ଦ୍ୱିଗୁଣ, normDiffL2_64u>,
            normDiffWrap<int64, ଦ୍ୱିଗୁଣ, normDiffL2_64s>,
            normDiffWrap<unsigned, ଦ୍ୱିଗୁଣ, normDiffL2_32u>,
            0
        },
    };
    ଯଦି (normType >= 3 || normType < 0) ଫେରନ୍ତୁ nullptr;

    ଫେରନ୍ତୁ normDiffTab[normType][depth];
}

#endif // CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

CV_CPU_OPTIMIZATION_NAMESPACE_END

} // cv::
