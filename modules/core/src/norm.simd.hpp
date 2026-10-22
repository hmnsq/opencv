// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html
//
// Copyright (C) 2025, SpaceMIT Inc., all rights reserved.
// Copyright (C) 2026, Advanced Micro Devices, Inc., all rights reserved.
// Third party copyrights are property of their respective owners.

#ضمن "precomp.hpp"

نطاق cv {

using NormFunc = صحيح (*)(ثابت uchar*, ثابت uchar*, uchar*, صحيح, صحيح);
using NormDiffFunc = صحيح (*)(ثابت uchar*, ثابت uchar*, ثابت uchar*, uchar*, صحيح, صحيح);

CV_CPU_OPTIMIZATION_NAMESPACE_BEGIN

template<typename T, typename RT, صحيح (*fn)(ثابت T*, ثابت uchar*, RT*, صحيح, صحيح)>
ساكن صحيح normWrap(ثابت uchar* src1, ثابت uchar* src2, uchar* res, صحيح len, صحيح cn)
{
    أرجع fn((ثابت T*)src1, src2, (RT*)res, len, cn);
}

template<typename T, typename RT, صحيح (*fn)(ثابت T*, ثابت T*, ثابت uchar*, RT*, صحيح, صحيح)>
ساكن صحيح normDiffWrap(ثابت uchar* src1, ثابت uchar* src2, ثابت uchar* src3, uchar* res, صحيح len, صحيح cn)
{
    أرجع fn((ثابت T*)src1, (ثابت T*)src2, src3, (RT*)res, len, cn);
}

NormFunc getNormFunc(صحيح normType, صحيح depth);
NormDiffFunc getNormDiffFunc(صحيح normType, صحيح depth);

#ifndef CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

template <typename T, typename ST>
بنية NormInf_SIMD {
    مضمن ST operator() (ثابت T* src, صحيح n) ثابت {
        ST s = 0;
        لكل (صحيح i = 0; i < n; i++) {
            s = std::max(s, (ST)cv_abs(src[i]));
        }
        أرجع s;
    }
};

template <typename T, typename ST>
بنية NormL1_SIMD {
    مضمن ST operator() (ثابت T* src, صحيح n) ثابت {
        ST s = 0;
        لكل (صحيح i = 0; i < n; i++) {
            s += cv_abs(src[i]);
        }
        أرجع s;
    }
};

template <typename T, typename ST>
بنية NormL2_SIMD {
    مضمن ST operator() (ثابت T* src, صحيح n) ثابت {
        ST s = 0;
        لكل (صحيح i = 0; i < n; i++) {
            ST v = (ST)src[i];
            s += v * v;
        }
        أرجع s;
    }
};

template <typename T, typename ST>
بنية NormDiffInf_SIMD {
    مضمن ST operator() (ثابت T* src1, ثابت T* src2, صحيح n) ثابت {
        ST s = 0;
        لكل (صحيح i = 0; i < n; i++) {
            ST v = (ST)cv_absdiff(src1[i], src2[i]);
            s = std::max(s, v);
        }
        أرجع s;
    }
};

template <typename T, typename ST>
بنية NormDiffL1_SIMD {
    مضمن ST operator() (ثابت T* src1, ثابت T* src2, صحيح n) ثابت {
        ST s = 0;
        لكل (صحيح i = 0; i < n; i++) {
            ST v = (ST)cv_absdiff(src1[i], src2[i]);
            s += v;
        }
        أرجع s;
    }
};

template <typename T, typename ST>
بنية NormDiffL2_SIMD {
    مضمن ST operator() (ثابت T* src1, ثابت T* src2, صحيح n) ثابت {
        ST s = 0;
        لكل (صحيح i = 0; i < n; i++) {
            ST v = (ST)src1[i] - (ST)src2[i];
            s += v * v;
        }
        أرجع s;
    }
};

#إذا (CV_SIMD || CV_SIMD_SCALABLE)

template<>
بنية NormInf_SIMD<uchar, صحيح> {
    صحيح operator() (ثابت uchar* src, صحيح n) ثابت {
        صحيح j = 0;
        صحيح s = 0;
        v_uint8 r0 = vx_setzero_u8(), r1 = vx_setzero_u8();
        v_uint8 r2 = vx_setzero_u8(), r3 = vx_setzero_u8();
        لكل (; j <= n - 4 * VTraits<v_uint8>::vlanes(); j += 4 * VTraits<v_uint8>::vlanes()) {
            r0 = v_max(r0, vx_load(src + j                                 ));
            r1 = v_max(r1, vx_load(src + j +     VTraits<v_uint8>::vlanes()));
            r2 = v_max(r2, vx_load(src + j + 2 * VTraits<v_uint8>::vlanes()));
            r3 = v_max(r3, vx_load(src + j + 3 * VTraits<v_uint8>::vlanes()));
        }
        r0 = v_max(r0, v_max(r1, v_max(r2, r3)));
        لكل (; j < n; j++) {
            s = std::max(s, (صحيح)src[j]);
        }
        أرجع std::max(s, (صحيح)v_reduce_max(r0));
    }
};

template<>
بنية NormInf_SIMD<schar, صحيح> {
    صحيح operator() (ثابت schar* src, صحيح n) ثابت {
        صحيح j = 0;
        صحيح s = 0;
        v_uint8 r0 = vx_setzero_u8(), r1 = vx_setzero_u8();
        v_uint8 r2 = vx_setzero_u8(), r3 = vx_setzero_u8();
        لكل (; j <= n - 4 * VTraits<v_int8>::vlanes(); j += 4 * VTraits<v_int8>::vlanes()) {
            r0 = v_max(r0, v_abs(vx_load(src + j                                )));
            r1 = v_max(r1, v_abs(vx_load(src + j +     VTraits<v_int8>::vlanes())));
            r2 = v_max(r2, v_abs(vx_load(src + j + 2 * VTraits<v_int8>::vlanes())));
            r3 = v_max(r3, v_abs(vx_load(src + j + 3 * VTraits<v_int8>::vlanes())));
        }
        r0 = v_max(r0, v_max(r1, v_max(r2, r3)));
        لكل (; j < n; j++) {
            s = std::max(s, cv_abs(src[j]));
        }
        أرجع std::max(s, saturate_cast<صحيح>(v_reduce_max(r0)));
    }
};

template<>
بنية NormInf_SIMD<ushort, صحيح> {
    صحيح operator() (ثابت ushort* src, صحيح n) ثابت {
        صحيح j = 0;
        صحيح s = 0;
        v_uint16 d0 = vx_setzero_u16(), d1 = vx_setzero_u16();
        v_uint16 d2 = vx_setzero_u16(), d3 = vx_setzero_u16();
        لكل (; j <= n - 4 * VTraits<v_uint16>::vlanes(); j += 4 * VTraits<v_uint16>::vlanes()) {
            d0 = v_max(d0, vx_load(src + j                                  ));
            d1 = v_max(d1, vx_load(src + j +     VTraits<v_uint16>::vlanes()));
            d2 = v_max(d2, vx_load(src + j + 2 * VTraits<v_uint16>::vlanes()));
            d3 = v_max(d3, vx_load(src + j + 3 * VTraits<v_uint16>::vlanes()));
        }
        d0 = v_max(d0, v_max(d1, v_max(d2, d3)));
        لكل (; j < n; j++) {
            s = std::max(s, (صحيح)src[j]);
        }
        أرجع std::max(s, (صحيح)v_reduce_max(d0));
    }
};

template<>
بنية NormInf_SIMD<short, صحيح> {
    صحيح operator() (ثابت short* src, صحيح n) ثابت {
        صحيح j = 0;
        صحيح s = 0;
        v_uint16 d0 = vx_setzero_u16(), d1 = vx_setzero_u16();
        v_uint16 d2 = vx_setzero_u16(), d3 = vx_setzero_u16();
        لكل (; j <= n - 4 * VTraits<v_int16>::vlanes(); j += 4 * VTraits<v_int16>::vlanes()) {
            d0 = v_max(d0, v_abs(vx_load(src + j                                  )));
            d1 = v_max(d1, v_abs(vx_load(src + j +     VTraits<v_int16>::vlanes())));
            d2 = v_max(d2, v_abs(vx_load(src + j + 2 * VTraits<v_int16>::vlanes())));
            d3 = v_max(d3, v_abs(vx_load(src + j + 3 * VTraits<v_int16>::vlanes())));
        }
        d0 = v_max(d0, v_max(d1, v_max(d2, d3)));
        لكل (; j < n; j++) {
            s = std::max(s, saturate_cast<صحيح>(cv_abs(src[j])));
        }
        أرجع std::max(s, saturate_cast<صحيح>(v_reduce_max(d0)));
    }
};

template<>
بنية NormInf_SIMD<صحيح, صحيح> {
    صحيح operator() (ثابت صحيح* src, صحيح n) ثابت {
        صحيح j = 0;
        صحيح s = 0;
        v_uint32 r0 = vx_setzero_u32(), r1 = vx_setzero_u32();
        v_uint32 r2 = vx_setzero_u32(), r3 = vx_setzero_u32();
        لكل (; j <= n - 4 * VTraits<v_int32>::vlanes(); j += 4 * VTraits<v_int32>::vlanes()) {
            r0 = v_max(r0, v_abs(vx_load(src + j                                 )));
            r1 = v_max(r1, v_abs(vx_load(src + j +     VTraits<v_int32>::vlanes())));
            r2 = v_max(r2, v_abs(vx_load(src + j + 2 * VTraits<v_int32>::vlanes())));
            r3 = v_max(r3, v_abs(vx_load(src + j + 3 * VTraits<v_int32>::vlanes())));
        }
        r0 = v_max(r0, v_max(r1, v_max(r2, r3)));
        s = std::max(s, saturate_cast<صحيح>(v_reduce_max(r0)));
        لكل (; j < n; j++) {
            s = std::max(s, std::abs(src[j]));
        }
        أرجع s;
    }
};

template<>
بنية NormInf_SIMD<عائم, عائم> {
    عائم operator() (ثابت عائم* src, صحيح n) ثابت {
        صحيح j = 0;
        عائم s = 0.f;
        v_float32 r0 = vx_setzero_f32(), r1 = vx_setzero_f32();
        v_float32 r2 = vx_setzero_f32(), r3 = vx_setzero_f32();
        لكل (; j <= n - 4 * VTraits<v_float32>::vlanes(); j += 4 * VTraits<v_float32>::vlanes()) {
            r0 = v_max(r0, v_abs(vx_load(src + j                                   )));
            r1 = v_max(r1, v_abs(vx_load(src + j +     VTraits<v_float32>::vlanes())));
            r2 = v_max(r2, v_abs(vx_load(src + j + 2 * VTraits<v_float32>::vlanes())));
            r3 = v_max(r3, v_abs(vx_load(src + j + 3 * VTraits<v_float32>::vlanes())));
        }
        r0 = v_max(r0, v_max(r1, v_max(r2, r3)));
        لكل (; j < n; j++) {
            s = std::max(s, cv_abs(src[j]));
        }
        أرجع std::max(s, v_reduce_max(r0));
    }
};

template<>
بنية NormL1_SIMD<uchar, صحيح> {
    صحيح operator() (ثابت uchar* src, صحيح n) ثابت {
        صحيح j = 0;
        صحيح s = 0;
        v_uint32 r0 = vx_setzero_u32(), r1 = vx_setzero_u32();
        v_uint8  one = vx_setall_u8(1);
        لكل (; j<= n - 2 * VTraits<v_uint8>::vlanes(); j += 2 * VTraits<v_uint8>::vlanes()) {
            v_uint8 v0 = vx_load(src + j);
            r0 = v_dotprod_expand_fast(v0, one, r0);

            v_uint8 v1 = vx_load(src + j + VTraits<v_uint8>::vlanes());
            r1 = v_dotprod_expand_fast(v1, one, r1);
        }
        s += v_reduce_sum(v_add(r0, r1));
        لكل (; j < n; j++) {
            s += src[j];
        }
        أرجع s;
    }
};

template<>
بنية NormL1_SIMD<schar, صحيح> {
    صحيح operator() (ثابت schar* src, صحيح n) ثابت {
        صحيح j = 0;
        صحيح s = 0;
        v_uint32 r0 = vx_setzero_u32(), r1 = vx_setzero_u32();
        v_uint8  one = vx_setall_u8(1);
        لكل (; j<= n - 2 * VTraits<v_int8>::vlanes(); j += 2 * VTraits<v_int8>::vlanes()) {
            v_uint8 v0 = v_abs(vx_load(src + j));
            r0 = v_dotprod_expand_fast(v0, one, r0);

            v_uint8 v1 = v_abs(vx_load(src + j + VTraits<v_int8>::vlanes()));
            r1 = v_dotprod_expand_fast(v1, one, r1);
        }
        s += v_reduce_sum(v_add(r0, r1));
        لكل (; j < n; j++) {
            s += saturate_cast<صحيح>(cv_abs(src[j]));
        }
        أرجع s;
    }
};

template<>
بنية NormL1_SIMD<ushort, صحيح> {
    صحيح operator() (ثابت ushort* src, صحيح n) ثابت {
        صحيح j = 0;
        صحيح s = 0;
        v_uint32 r00 = vx_setzero_u32(), r01 = vx_setzero_u32();
        v_uint32 r10 = vx_setzero_u32(), r11 = vx_setzero_u32();
        لكل (; j<= n - 2 * VTraits<v_uint16>::vlanes(); j += 2 * VTraits<v_uint16>::vlanes()) {
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
        s += (صحيح)v_reduce_sum(v_add(v_add(v_add(r00, r01), r10), r11));
        لكل (; j < n; j++) {
            s += src[j];
        }
        أرجع s;
    }
};

template<>
بنية NormL1_SIMD<short, صحيح> {
    صحيح operator() (ثابت short* src, صحيح n) ثابت {
        صحيح j = 0;
        صحيح s = 0;
        v_uint32 r00 = vx_setzero_u32(), r01 = vx_setzero_u32();
        v_uint32 r10 = vx_setzero_u32(), r11 = vx_setzero_u32();
        لكل (; j<= n - 2 * VTraits<v_int16>::vlanes(); j += 2 * VTraits<v_int16>::vlanes()) {
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
        s += (صحيح)v_reduce_sum(v_add(v_add(v_add(r00, r01), r10), r11));
        لكل (; j < n; j++) {
            s += saturate_cast<صحيح>(cv_abs(src[j]));
        }
        أرجع s;
    }
};

template<>
بنية NormL2_SIMD<uchar, صحيح> {
    صحيح operator() (ثابت uchar* src, صحيح n) ثابت {
        صحيح j = 0;
        صحيح s = 0;
        v_uint32 r0 = vx_setzero_u32(), r1 = vx_setzero_u32();
        لكل (; j <= n - 2 * VTraits<v_uint8>::vlanes(); j += 2 * VTraits<v_uint8>::vlanes()) {
            v_uint8 v0 = vx_load(src + j);
            r0 = v_dotprod_expand_fast(v0, v0, r0);

            v_uint8 v1 = vx_load(src + j + VTraits<v_uint8>::vlanes());
            r1 = v_dotprod_expand_fast(v1, v1, r1);
        }
        s += v_reduce_sum(v_add(r0, r1));
        لكل (; j < n; j++) {
            صحيح v = saturate_cast<صحيح>(src[j]);
            s += v * v;
        }
        أرجع s;
    }
};

template<>
بنية NormL2_SIMD<schar, صحيح> {
    صحيح operator() (ثابت schar* src, صحيح n) ثابت {
        صحيح j = 0;
        صحيح s = 0;
        v_int32 r0 = vx_setzero_s32(), r1 = vx_setzero_s32();
        لكل (; j <= n - 2 * VTraits<v_int8>::vlanes(); j += 2 * VTraits<v_int8>::vlanes()) {
            v_int8 v0 = vx_load(src + j);
            r0 = v_dotprod_expand_fast(v0, v0, r0);
            v_int8 v1 = vx_load(src + j + VTraits<v_int8>::vlanes());
            r1 = v_dotprod_expand_fast(v1, v1, r1);
        }
        s += v_reduce_sum(v_add(r0, r1));
        لكل (; j < n; j++) {
            صحيح v = saturate_cast<صحيح>(src[j]);
            s += v * v;
        }
        أرجع s;
    }
};

template<>
بنية NormDiffInf_SIMD<uchar, صحيح> {
    صحيح operator() (ثابت uchar* src1, ثابت uchar* src2, صحيح n) ثابت {
        صحيح j = 0;
        صحيح s = 0;
        v_uint8 r0 = vx_setzero_u8(), r1 = vx_setzero_u8();
        v_uint8 r2 = vx_setzero_u8(), r3 = vx_setzero_u8();
        لكل (; j <= n - 4 * VTraits<v_uint8>::vlanes(); j += 4 * VTraits<v_uint8>::vlanes()) {
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
        s = (صحيح)v_reduce_max(v_max(v_max(v_max(r0, r1), r2), r3));
        لكل (; j < n; j++) {
            صحيح v = (صحيح)cv_absdiff(src1[j], src2[j]);
            s = std::max(s, v);
        }
        أرجع s;
    }
};

template<>
بنية NormDiffInf_SIMD<schar, صحيح> {
    صحيح operator() (ثابت schar* src1, ثابت schar* src2, صحيح n) ثابت {
        صحيح j = 0;
        صحيح s = 0;
        v_uint8 r0 = vx_setzero_u8(), r1 = vx_setzero_u8();
        v_uint8 r2 = vx_setzero_u8(), r3 = vx_setzero_u8();
        لكل (; j <= n - 4 * VTraits<v_int8>::vlanes(); j += 4 * VTraits<v_int8>::vlanes()) {
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
        s = (صحيح)v_reduce_max(v_max(v_max(v_max(r0, r1), r2), r3));
        لكل (; j < n; j++) {
            صحيح v = (صحيح)cv_absdiff(src1[j], src2[j]);
            s = std::max(s, v);
        }
        أرجع s;
    }
};

template<>
بنية NormDiffInf_SIMD<ushort, صحيح> {
    صحيح operator() (ثابت ushort* src1, ثابت ushort* src2, صحيح n) ثابت {
        صحيح j = 0;
        صحيح s = 0;
        v_uint16 r0 = vx_setzero_u16(), r1 = vx_setzero_u16();
        v_uint16 r2 = vx_setzero_u16(), r3 = vx_setzero_u16();
        لكل (; j <= n - 4 * VTraits<v_uint16>::vlanes(); j += 4 * VTraits<v_uint16>::vlanes()) {
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
        s = (صحيح)v_reduce_max(v_max(v_max(v_max(r0, r1), r2), r3));
        لكل (; j < n; j++) {
            صحيح v = (صحيح)cv_absdiff(src1[j], src2[j]);
            s = std::max(s, v);
        }
        أرجع s;
    }
};

template<>
بنية NormDiffInf_SIMD<short, صحيح> {
    صحيح operator() (ثابت short* src1, ثابت short* src2, صحيح n) ثابت {
        صحيح j = 0;
        صحيح s = 0;
        v_uint16 r0 = vx_setzero_u16(), r1 = vx_setzero_u16();
        v_uint16 r2 = vx_setzero_u16(), r3 = vx_setzero_u16();
        لكل (; j <= n - 4 * VTraits<v_int16>::vlanes(); j += 4 * VTraits<v_int16>::vlanes()) {
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
        s = (صحيح)v_reduce_max(v_max(v_max(v_max(r0, r1), r2), r3));
        لكل (; j < n; j++) {
            صحيح v = (صحيح)cv_absdiff(src1[j], src2[j]);
            s = std::max(s, v);
        }
        أرجع s;
    }
};

template<>
بنية NormDiffInf_SIMD<صحيح, unsigned> {
    unsigned operator() (ثابت صحيح* src1, ثابت صحيح* src2, صحيح n) ثابت {
        صحيح j = 0;
        unsigned s = 0;
        v_uint32 r0 = vx_setzero_u32(), r1 = vx_setzero_u32();
        v_uint32 r2 = vx_setzero_u32(), r3 = vx_setzero_u32();
        لكل (; j <= n - 4 * VTraits<v_int32>::vlanes(); j += 4 * VTraits<v_int32>::vlanes()) {
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
        s = (صحيح)v_reduce_max(v_max(v_max(v_max(r0, r1), r2), r3));
        لكل (; j < n; j++) {
            unsigned v = (unsigned)cv_absdiff(src1[j], src2[j]);
            s = std::max(s, v);
        }
        أرجع s;
    }
};

template<>
بنية NormDiffInf_SIMD<عائم, عائم> {
    عائم operator() (ثابت عائم* src1, ثابت عائم* src2, صحيح n) ثابت {
        صحيح j = 0;
        عائم s = 0;
        v_float32 r0 = vx_setzero_f32(), r1 = vx_setzero_f32();
        لكل (; j <= n - 2 * VTraits<v_float32>::vlanes(); j += 2 * VTraits<v_float32>::vlanes()) {
            v_float32 v01 = vx_load(src1 + j), v02 = vx_load(src2 + j);
            r0 = v_max(r0, v_absdiff(v01, v02));

            v_float32 v11 = vx_load(src1 + j + VTraits<v_float32>::vlanes()),
                      v12 = vx_load(src2 + j + VTraits<v_float32>::vlanes());
            r1 = v_max(r1, v_absdiff(v11, v12));
        }
        s = v_reduce_max(v_max(r0, r1));
        لكل (; j < n; j++) {
            عائم v = (عائم)cv_absdiff(src1[j], src2[j]);
            s = std::max(s, v);
        }
        أرجع s;
    }
};

template<>
بنية NormDiffL1_SIMD<uchar, صحيح> {
    صحيح operator() (ثابت uchar* src1, ثابت uchar* src2, صحيح n) ثابت {
        صحيح j = 0;
        صحيح s = 0;
        v_uint32 r0 = vx_setzero_u32(), r1 = vx_setzero_u32();
        v_uint8  one = vx_setall_u8(1);
        لكل (; j<= n - 2 * VTraits<v_uint8>::vlanes(); j += 2 * VTraits<v_uint8>::vlanes()) {
            v_uint8 v01 = vx_load(src1 + j), v02 = vx_load(src2 + j);
            r0 = v_dotprod_expand_fast(v_absdiff(v01, v02), one, r0);

            v_uint8 v11 = vx_load(src1 + j + VTraits<v_uint8>::vlanes()),
                    v12 = vx_load(src2 + j + VTraits<v_uint8>::vlanes());
            r1 = v_dotprod_expand_fast(v_absdiff(v11, v12), one, r1);
        }
        s += v_reduce_sum(v_add(r0, r1));
        لكل (; j < n; j++) {
            صحيح v = (صحيح)cv_absdiff(src1[j], src2[j]);
            s += v;
        }
        أرجع s;
    }
};

template<>
بنية NormDiffL1_SIMD<schar, صحيح> {
    صحيح operator() (ثابت schar* src1, ثابت schar* src2, صحيح n) ثابت {
        صحيح j = 0;
        صحيح s = 0;
        v_uint32 r0 = vx_setzero_u32(), r1 = vx_setzero_u32();
        v_uint8  one = vx_setall_u8(1);
        لكل (; j<= n - 2 * VTraits<v_int8>::vlanes(); j += 2 * VTraits<v_int8>::vlanes()) {
            v_int8 v01 = vx_load(src1 + j), v02 = vx_load(src2 + j);
            r0 = v_dotprod_expand_fast(v_absdiff(v01, v02), one, r0);

            v_int8 v11 = vx_load(src1 + j + VTraits<v_int8>::vlanes()),
                   v12 = vx_load(src2 + j + VTraits<v_int8>::vlanes());
            r1 = v_dotprod_expand_fast(v_absdiff(v11, v12), one, r1);
        }
        s += v_reduce_sum(v_add(r0, r1));
        لكل (; j < n; j++) {
            صحيح v = (صحيح)cv_absdiff(src1[j], src2[j]);
            s += v;
        }
        أرجع s;
    }
};

template<>
بنية NormDiffL1_SIMD<ushort, صحيح> {
    صحيح operator() (ثابت ushort* src1, ثابت ushort* src2, صحيح n) ثابت {
        صحيح j = 0;
        صحيح s = 0;
        v_uint32 r0 = vx_setzero_u32(), r1 = vx_setzero_u32();
        v_uint32 r2 = vx_setzero_u32(), r3 = vx_setzero_u32();
        لكل (; j<= n - 4 * VTraits<v_uint16>::vlanes(); j += 4 * VTraits<v_uint16>::vlanes()) {
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
        s += (صحيح)v_reduce_sum(v_add(v_add(v_add(r0, r1), r2), r3));
        لكل (; j < n; j++) {
            صحيح v = (صحيح)cv_absdiff(src1[j], src2[j]);
            s += v;
        }
        أرجع s;
    }
};

template<>
بنية NormDiffL1_SIMD<short, صحيح> {
    صحيح operator() (ثابت short* src1, ثابت short* src2, صحيح n) ثابت {
        صحيح j = 0;
        صحيح s = 0;
        v_uint32 r0 = vx_setzero_u32(), r1 = vx_setzero_u32();
        v_uint32 r2 = vx_setzero_u32(), r3 = vx_setzero_u32();
        لكل (; j<= n - 4 * VTraits<v_int16>::vlanes(); j += 4 * VTraits<v_int16>::vlanes()) {
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
        s += (صحيح)v_reduce_sum(v_add(v_add(v_add(r0, r1), r2), r3));
        لكل (; j < n; j++) {
            صحيح v = (صحيح)cv_absdiff(src1[j], src2[j]);
            s += v;
        }
        أرجع s;
    }
};

template<>
بنية NormDiffL2_SIMD<uchar, صحيح> {
    صحيح operator() (ثابت uchar* src1, ثابت uchar* src2, صحيح n) ثابت {
        صحيح j = 0;
        صحيح s = 0;
        v_uint32 r0 = vx_setzero_u32(), r1 = vx_setzero_u32();
        لكل (; j <= n - 2 * VTraits<v_uint8>::vlanes(); j += 2 * VTraits<v_uint8>::vlanes()) {
            v_uint8 v01 = vx_load(src1 + j), v02 = vx_load(src2 + j);
            v_uint8 v0 = v_absdiff(v01, v02);
            r0 = v_dotprod_expand_fast(v0, v0, r0);

            v_uint8 v11 = vx_load(src1 + j + VTraits<v_uint8>::vlanes()),
                    v12 = vx_load(src2 + j + VTraits<v_uint8>::vlanes());
            v_uint8 v1 = v_absdiff(v11, v12);
            r1 = v_dotprod_expand_fast(v1, v1, r1);
        }
        s += v_reduce_sum(v_add(r0, r1));
        لكل (; j < n; j++) {
            صحيح v = (صحيح)src1[j] - (صحيح)src2[j];
            s += v * v;
        }
        أرجع s;
    }
};

template<>
بنية NormDiffL2_SIMD<schar, صحيح> {
    صحيح operator() (ثابت schar* src1, ثابت schar* src2, صحيح n) ثابت {
        صحيح j = 0;
        صحيح s = 0;
        v_uint32 r0 = vx_setzero_u32(), r1 = vx_setzero_u32();
        لكل (; j <= n - 2 * VTraits<v_int8>::vlanes(); j += 2 * VTraits<v_int8>::vlanes()) {
            v_int8 v01 = vx_load(src1 + j), v02 = vx_load(src2 + j);
            v_uint8 v0 = v_absdiff(v01, v02);
            r0 = v_dotprod_expand_fast(v0, v0, r0);

            v_int8 v11 = vx_load(src1 + j + VTraits<v_int8>::vlanes()),
                   v12 = vx_load(src2 + j + VTraits<v_int8>::vlanes());
            v_uint8 v1 = v_absdiff(v11, v12);
            r1 = v_dotprod_expand_fast(v1, v1, r1);
        }
        s += v_reduce_sum(v_add(r0, r1));
        لكل (; j < n; j++) {
            صحيح v = (صحيح)src1[j] - (صحيح)src2[j];
            s += v * v;
        }
        أرجع s;
    }
};

#endif

#إذا (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)

template<>
بنية NormInf_SIMD<مزدوج, مزدوج> {
    مزدوج operator() (ثابت مزدوج* src, صحيح n) ثابت {
        صحيح j = 0;
        مزدوج s = 0.f;
        v_float64 r0 = vx_setzero_f64(), r1 = vx_setzero_f64();
        v_float64 r2 = vx_setzero_f64(), r3 = vx_setzero_f64();
        لكل (; j <= n - 4 * VTraits<v_float64>::vlanes(); j += 4 * VTraits<v_float64>::vlanes()) {
            r0 = v_max(r0, v_abs(vx_load(src + j                                   )));
            r1 = v_max(r1, v_abs(vx_load(src + j +     VTraits<v_float64>::vlanes())));
            r2 = v_max(r2, v_abs(vx_load(src + j + 2 * VTraits<v_float64>::vlanes())));
            r3 = v_max(r3, v_abs(vx_load(src + j + 3 * VTraits<v_float64>::vlanes())));
        }
        r0 = v_max(r0, v_max(r1, v_max(r2, r3)));
        لكل (; j < n; j++) {
            s = std::max(s, cv_abs(src[j]));
        }
        // [TODO]: use v_reduce_max when it supports float64
        مزدوج t[VTraits<v_float64>::max_nlanes];
        vx_store(t, r0);
        لكل (صحيح i = 0; i < VTraits<v_float64>::vlanes(); i++) {
            s = std::max(s, cv_abs(t[i]));
        }
        أرجع s;
    }
};

template<>
بنية NormL1_SIMD<صحيح, مزدوج> {
    مزدوج operator() (ثابت صحيح* src, صحيح n) ثابت {
        صحيح j = 0;
        مزدوج s = 0.f;
        v_float64 r00 = vx_setzero_f64(), r01 = vx_setzero_f64();
        v_float64 r10 = vx_setzero_f64(), r11 = vx_setzero_f64();
        لكل (; j <= n - 2 * VTraits<v_int32>::vlanes(); j += 2 * VTraits<v_int32>::vlanes()) {
            // Widen to double before taking the magnitude: exact over the whole int32 range,
            // INT_MIN included, which a float intermediate would not be above 2^24.
            v_int32 i0 = vx_load(src + j), i1 = vx_load(src + j + VTraits<v_int32>::vlanes());
            r00 = v_add(r00, v_abs(v_cvt_f64(i0))); r01 = v_add(r01, v_abs(v_cvt_f64_بلند(i0)));
            r10 = v_add(r10, v_abs(v_cvt_f64(i1))); r11 = v_add(r11, v_abs(v_cvt_f64_بلند(i1)));
        }
        s += v_reduce_sum(v_add(v_add(v_add(r00, r01), r10), r11));
        لكل (; j < n; j++) {
            s += cv_abs(src[j]);
        }
        أرجع s;
    }
};

template<>
بنية NormL1_SIMD<عائم, مزدوج> {
    مزدوج operator() (ثابت عائم* src, صحيح n) ثابت {
        صحيح j = 0;
        مزدوج s = 0.f;
        v_float64 r00 = vx_setzero_f64(), r01 = vx_setzero_f64();
        v_float64 r10 = vx_setzero_f64(), r11 = vx_setzero_f64();
        v_float64 r20 = vx_setzero_f64(), r21 = vx_setzero_f64();
        v_float64 r30 = vx_setzero_f64(), r31 = vx_setzero_f64();
        لكل (; j <= n - 4 * VTraits<v_float32>::vlanes(); j += 4 * VTraits<v_float32>::vlanes()) {
            v_float32 v0 = v_abs(vx_load(src + j)), v1 = v_abs(vx_load(src + j + VTraits<v_float32>::vlanes()));
            r00 = v_add(r00, v_cvt_f64(v0)); r01 = v_add(r01, v_cvt_f64_بلند(v0));
            r10 = v_add(r10, v_cvt_f64(v1)); r11 = v_add(r11, v_cvt_f64_بلند(v1));

            v_float32 v2 = v_abs(vx_load(src + j + 2 * VTraits<v_float32>::vlanes())), v3 = v_abs(vx_load(src + j + 3 * VTraits<v_float32>::vlanes()));
            r20 = v_add(r20, v_cvt_f64(v2)); r21 = v_add(r21, v_cvt_f64_بلند(v2));
            r30 = v_add(r30, v_cvt_f64(v3)); r31 = v_add(r31, v_cvt_f64_بلند(v3));
        }
        s += v_reduce_sum(v_add(v_add(v_add(r00, r01), r10), r11));
        s += v_reduce_sum(v_add(v_add(v_add(r20, r21), r30), r31));
        لكل (; j < n; j++) {
            s += cv_abs(src[j]);
        }
        أرجع s;
    }
};

template<>
بنية NormL2_SIMD<ushort, مزدوج> {
    مزدوج operator() (ثابت ushort* src, صحيح n) ثابت {
        صحيح j = 0;
        مزدوج s = 0.f;
        v_float64 r0 = vx_setzero_f64(), r1 = vx_setzero_f64();
        لكل (; j <= n - 2 * VTraits<v_uint16>::vlanes(); j += 2 * VTraits<v_uint16>::vlanes()) {
            v_uint16 v0 = vx_load(src + j);
            v_uint64 u0 = v_dotprod_expand_fast(v0, v0);
            r0 = v_add(r0, v_cvt_f64(v_reinterpret_as_s64(u0)));

            v_uint16 v1 = vx_load(src + j + VTraits<v_uint16>::vlanes());
            v_uint64 u1 = v_dotprod_expand_fast(v1, v1);
            r1 = v_add(r1, v_cvt_f64(v_reinterpret_as_s64(u1)));
        }
        s += v_reduce_sum(v_add(r0, r1));
        لكل (; j < n; j++) {
            مزدوج v = saturate_cast<مزدوج>(src[j]);
            s += v * v;
        }
        أرجع s;
    }
};

template<>
بنية NormL2_SIMD<short, مزدوج> {
    مزدوج operator() (ثابت short* src, صحيح n) ثابت {
        صحيح j = 0;
        مزدوج s = 0.f;
        v_float64 r0 = vx_setzero_f64(), r1 = vx_setzero_f64();
        لكل (; j <= n - 2 * VTraits<v_int16>::vlanes(); j += 2 * VTraits<v_int16>::vlanes()) {
            v_int16 v0 = vx_load(src + j);
            r0 = v_add(r0, v_cvt_f64(v_dotprod_expand_fast(v0, v0)));

            v_int16 v1 = vx_load(src + j + VTraits<v_int16>::vlanes());
            r1 = v_add(r1, v_cvt_f64(v_dotprod_expand_fast(v1, v1)));
        }
        s += v_reduce_sum(v_add(r0, r1));
        لكل (; j < n; j++) {
            مزدوج v = saturate_cast<مزدوج>(src[j]);
            s += v * v;
        }
        أرجع s;
    }
};

template<>
بنية NormL2_SIMD<صحيح, مزدوج> {
    مزدوج operator() (ثابت صحيح* src, صحيح n) ثابت {
        صحيح j = 0;
        مزدوج s = 0.f;
        v_float64 r0 = vx_setzero_f64(), r1 = vx_setzero_f64();
        لكل (; j <= n - 2 * VTraits<v_int32>::vlanes(); j += 2 * VTraits<v_int32>::vlanes()) {
            v_int32 v0 = vx_load(src + j);
            r0 = v_dotprod_expand_fast(v0, v0, r0);

            v_int32 v1 = vx_load(src + j + VTraits<v_int32>::vlanes());
            r1 = v_dotprod_expand_fast(v1, v1, r1);
        }
        s += v_reduce_sum(v_add(r0, r1));
        لكل (; j < n; j++) {
            مزدوج v = src[j];
            s += v * v;
        }
        أرجع s;
    }
};

template<>
بنية NormL2_SIMD<عائم, مزدوج> {
    مزدوج operator() (ثابت عائم* src, صحيح n) ثابت {
        صحيح j = 0;
        مزدوج s = 0.f;
        v_float64 r00 = vx_setzero_f64(), r01 = vx_setzero_f64();
        v_float64 r10 = vx_setzero_f64(), r11 = vx_setzero_f64();
        لكل (; j <= n - 2 * VTraits<v_float32>::vlanes(); j += 2 * VTraits<v_float32>::vlanes()) {
            v_float32 v0 = vx_load(src + j), v1 = vx_load(src + j + VTraits<v_float32>::vlanes());
            v_float64 v00 = v_cvt_f64(v0), v01 = v_cvt_f64_بلند(v0);
            v_float64 v10 = v_cvt_f64(v1), v11 = v_cvt_f64_بلند(v1);
            r00 = v_fma(v00, v00, r00); r01 = v_fma(v01, v01, r01);
            r10 = v_fma(v10, v10, r10); r11 = v_fma(v11, v11, r11);
        }
        s += v_reduce_sum(v_add(v_add(v_add(r00, r01), r10), r11));
        لكل (; j < n; j++) {
            مزدوج v = src[j];
            s += v * v;
        }
        أرجع s;
    }
};

#endif

#إذا CV_SIMD_64F // CV_SIMD_SCALABLE_64F has accuracy problem with the following kernels on ci

template<>
بنية NormL1_SIMD<مزدوج, مزدوج> {
    مزدوج operator() (ثابت مزدوج* src, صحيح n) ثابت {
        صحيح j = 0;
        مزدوج s = 0.f;
        v_float64 r00 = vx_setzero_f64(), r01 = vx_setzero_f64();
        v_float64 r10 = vx_setzero_f64(), r11 = vx_setzero_f64();
        لكل (; j <= n - 4 * VTraits<v_float64>::vlanes(); j += 4 * VTraits<v_float64>::vlanes()) {
            r00 = v_add(r00, v_abs(vx_load(src + j                                   )));
            r01 = v_add(r01, v_abs(vx_load(src + j +     VTraits<v_float64>::vlanes())));
            r10 = v_add(r10, v_abs(vx_load(src + j + 2 * VTraits<v_float64>::vlanes())));
            r11 = v_add(r11, v_abs(vx_load(src + j + 3 * VTraits<v_float64>::vlanes())));
        }
        s += v_reduce_sum(v_add(v_add(v_add(r00, r01), r10), r11));
        لكل (; j < n; j++) {
            s += cv_abs(src[j]);
        }
        أرجع s;
    }
};

template<>
بنية NormL2_SIMD<مزدوج, مزدوج> {
    مزدوج operator() (ثابت مزدوج* src, صحيح n) ثابت {
        صحيح j = 0;
        مزدوج s = 0.f;
        v_float64 r00 = vx_setzero_f64(), r01 = vx_setzero_f64();
        v_float64 r10 = vx_setzero_f64(), r11 = vx_setzero_f64();
        لكل (; j <= n - 4 * VTraits<v_float64>::vlanes(); j += 4 * VTraits<v_float64>::vlanes()) {
            v_float64 v00 = vx_load(src + j                                   );
            v_float64 v01 = vx_load(src + j +     VTraits<v_float64>::vlanes());
            v_float64 v10 = vx_load(src + j + 2 * VTraits<v_float64>::vlanes());
            v_float64 v11 = vx_load(src + j + 3 * VTraits<v_float64>::vlanes());
            r00 = v_fma(v00, v00, r00); r01 = v_fma(v01, v01, r01);
            r10 = v_fma(v10, v10, r10); r11 = v_fma(v11, v11, r11);
        }
        s += v_reduce_sum(v_add(v_add(v_add(r00, r01), r10), r11));
        لكل (; j < n; j++) {
            مزدوج v = src[j];
            s += v * v;
        }
        أرجع s;
    }
};

template<>
بنية NormDiffInf_SIMD<مزدوج, مزدوج> {
    مزدوج operator() (ثابت مزدوج* src1, ثابت مزدوج* src2, صحيح n) ثابت {
        صحيح j = 0;
        مزدوج s = 0;
        v_float64 r0 = vx_setzero_f64(), r1 = vx_setzero_f64();
        لكل (; j <= n - 2 * VTraits<v_float64>::vlanes(); j += 2 * VTraits<v_float64>::vlanes()) {
            v_float64 v01 = vx_load(src1 + j), v02 = vx_load(src2 + j);
            r0 = v_max(r0, v_absdiff(v01, v02));

            v_float64 v11 = vx_load(src1 + j + VTraits<v_float64>::vlanes()),
                      v12 = vx_load(src2 + j + VTraits<v_float64>::vlanes());
            r1 = v_max(r1, v_absdiff(v11, v12));
        }
        // [TODO]: use v_reduce_max when it supports float64
        مزدوج t[VTraits<v_float64>::max_nlanes];
        vx_store(t, v_max(r0, r1));
        لكل (صحيح i = 0; i < VTraits<v_float64>::vlanes(); i++) {
            s = std::max(s, t[i]);
        }
        لكل (; j < n; j++) {
            مزدوج v = (مزدوج)cv_absdiff(src1[j], src2[j]);
            s = std::max(s, v);
        }
        أرجع s;
    }
};

template<>
بنية NormDiffL1_SIMD<عائم, مزدوج> {
    مزدوج operator() (ثابت عائم* src1, ثابت عائم* src2, صحيح n) ثابت {
        صحيح j = 0;
        مزدوج s = 0.f;
        v_float64 r0 = vx_setzero_f64(), r1 = vx_setzero_f64();
        v_float64 r2 = vx_setzero_f64(), r3 = vx_setzero_f64();
        لكل (; j <= n - 2 * VTraits<v_float32>::vlanes(); j += 2 * VTraits<v_float32>::vlanes()) {
            v_float32 v01 = vx_load(src1 + j), v02 = vx_load(src2 + j);
            v_float32 v0 = v_absdiff(v01, v02);
            r0 = v_add(r0, v_cvt_f64(v0)); r1 = v_add(r1, v_cvt_f64_بلند(v0));

            v_float32 v11 = vx_load(src1 + j + VTraits<v_float32>::vlanes()),
                      v12 = vx_load(src2 + j + VTraits<v_float32>::vlanes());
            v_float32 v1 = v_absdiff(v11, v12);
            r2 = v_add(r2, v_cvt_f64(v1)); r3 = v_add(r3, v_cvt_f64_بلند(v1));
        }
        s += v_reduce_sum(v_add(v_add(v_add(r0, r1), r2), r3));
        لكل (; j < n; j++) {
            مزدوج v = (مزدوج)cv_absdiff(src1[j], src2[j]);
            s += v;
        }
        أرجع s;
    }
};

template<>
بنية NormDiffL1_SIMD<مزدوج, مزدوج> {
    مزدوج operator() (ثابت مزدوج* src1, ثابت مزدوج* src2, صحيح n) ثابت {
        صحيح j = 0;
        مزدوج s = 0.f;
        v_float64 r0 = vx_setzero_f64(), r1 = vx_setzero_f64();
        لكل (; j <= n - 2 * VTraits<v_float64>::vlanes(); j += 2 * VTraits<v_float64>::vlanes()) {
            v_float64 v01 = vx_load(src1 + j), v02 = vx_load(src2 + j);
            r0 = v_add(r0, v_absdiff(v01, v02));

            v_float64 v11 = vx_load(src1 + j + VTraits<v_float64>::vlanes()),
                      v12 = vx_load(src2 + j + VTraits<v_float64>::vlanes());
            r1 = v_add(r1, v_absdiff(v11, v12));
        }
        s += v_reduce_sum(v_add(r0, r1));
        لكل (; j < n; j++) {
            مزدوج v = (مزدوج)cv_absdiff(src1[j], src2[j]);
            s += v;
        }
        أرجع s;
    }
};

template<>
بنية NormDiffL2_SIMD<ushort, مزدوج> {
    مزدوج operator() (ثابت ushort* src1, ثابت ushort* src2, صحيح n) ثابت {
        صحيح j = 0;
        مزدوج s = 0.f;
        v_float64 r0 = vx_setzero_f64(), r1 = vx_setzero_f64();
        لكل (; j <= n - 2 * VTraits<v_uint16>::vlanes(); j += 2 * VTraits<v_uint16>::vlanes()) {
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
        لكل (; j < n; j++) {
            مزدوج v = (مزدوج)src1[j] - (مزدوج)src2[j];
            s += v * v;
        }
        أرجع s;
    }
};

template<>
بنية NormDiffL2_SIMD<short, مزدوج> {
    مزدوج operator() (ثابت short* src1, ثابت short* src2, صحيح n) ثابت {
        صحيح j = 0;
        مزدوج s = 0.f;
        v_float64 r0 = vx_setzero_f64(), r1 = vx_setzero_f64();
        لكل (; j <= n - 2 * VTraits<v_int16>::vlanes(); j += 2 * VTraits<v_int16>::vlanes()) {
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
        لكل (; j < n; j++) {
            مزدوج v = (مزدوج)src1[j] - (مزدوج)src2[j];
            s += v * v;
        }
        أرجع s;
    }
};

template<>
بنية NormDiffL2_SIMD<صحيح, مزدوج> {
    مزدوج operator() (ثابت صحيح* src1, ثابت صحيح* src2, صحيح n) ثابت {
        صحيح j = 0;
        مزدوج s = 0.f;
        v_float64 r0 = vx_setzero_f64(), r1 = vx_setzero_f64();
        لكل (; j <= n - VTraits<v_int32>::vlanes(); j += VTraits<v_int32>::vlanes()) {
            v_int32 v01 = vx_load(src1 + j), v02 = vx_load(src2 + j);
            v_uint32 v0 = v_absdiff(v01, v02);
            v_uint64 ev0, ev1;
            v_expand(v0, ev0, ev1);
            v_float64 f0 = v_cvt_f64(v_reinterpret_as_s64(ev0)), f1 = v_cvt_f64(v_reinterpret_as_s64(ev1));
            r0 = v_fma(f0, f0, r0); r1 = v_fma(f1, f1, r1);
        }
        s += v_reduce_sum(v_add(r0, r1));
        لكل (; j < n; j++) {
            مزدوج v = (مزدوج)src1[j] - (مزدوج)src2[j];
            s += v * v;
        }
        أرجع s;
    }
};

template<>
بنية NormDiffL2_SIMD<عائم, مزدوج> {
    مزدوج operator() (ثابت عائم* src1, ثابت عائم* src2, صحيح n) ثابت {
        صحيح j = 0;
        مزدوج s = 0.f;
        v_float64 r0 = vx_setzero_f64(), r1 = vx_setzero_f64();
        v_float64 r2 = vx_setzero_f64(), r3 = vx_setzero_f64();
        لكل (; j <= n - 2 * VTraits<v_float32>::vlanes(); j += 2 * VTraits<v_float32>::vlanes()) {
            v_float32 v01 = vx_load(src1 + j), v02 = vx_load(src2 + j);
            v_float32 v0 = v_absdiff(v01, v02);
            v_float64 f01 = v_cvt_f64(v0), f02 = v_cvt_f64_بلند(v0);
            r0 = v_fma(f01, f01, r0); r1 = v_fma(f02, f02, r1);

            v_float32 v11 = vx_load(src1 + j + VTraits<v_float32>::vlanes()),
                      v12 = vx_load(src2 + j + VTraits<v_float32>::vlanes());
            v_float32 v1 = v_absdiff(v11, v12);
            v_float64 f11 = v_cvt_f64(v1), f12 = v_cvt_f64_بلند(v1);
            r2 = v_fma(f11, f11, r2); r3 = v_fma(f12, f12, r3);
        }
        s += v_reduce_sum(v_add(v_add(v_add(r0, r1), r2), r3));
        لكل (; j < n; j++) {
            مزدوج v = (مزدوج)src1[j] - (مزدوج)src2[j];
            s += v * v;
        }
        أرجع s;
    }
};

template<>
بنية NormDiffL2_SIMD<مزدوج, مزدوج> {
    مزدوج operator() (ثابت مزدوج* src1, ثابت مزدوج* src2, صحيح n) ثابت {
        صحيح j = 0;
        مزدوج s = 0.f;
        v_float64 r0 = vx_setzero_f64(), r1 = vx_setzero_f64();
        v_float64 r2 = vx_setzero_f64(), r3 = vx_setzero_f64();
        لكل (; j <= n - 4 * VTraits<v_float64>::vlanes(); j += 4 * VTraits<v_float64>::vlanes()) {
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
        لكل (; j < n; j++) {
            مزدوج v = (مزدوج)src1[j] - (مزدوج)src2[j];
            s += v * v;
        }
        أرجع s;
    }
};

#endif

template <typename T, typename ST>
بنية MaskedNormInf_SIMD {
    مضمن ST operator() (ثابت T* src, ثابت uchar* mask, صحيح len, صحيح cn) ثابت {
        ST s = 0;
        إذا (cn == 1) {
            لكل (صحيح i = 0; i < len; i++) {
                إذا (mask[i]) {
                    s = std::max(s, (ST)cv_abs(src[i]));
                }
            }
        }
        وإلا {
            لكل (صحيح i = 0; i < len; i++) {
                إذا (mask[i]) {
                    ثابت T* elem = src + i * cn;
                    صحيح k = 0;
                #إذا CV_ENABLE_UNROLLED && !CV_SIMD_SCALABLE
                    لكل (; k <= cn - 4; k += 4) {
                        s = std::max(s, (ST)cv_abs(elem[k]));
                        s = std::max(s, (ST)cv_abs(elem[k + 1]));
                        s = std::max(s, (ST)cv_abs(elem[k + 2]));
                        s = std::max(s, (ST)cv_abs(elem[k + 3]));
                    }
                #endif
                    لكل (; k < cn; k++) {
                        s = std::max(s, (ST)cv_abs(elem[k]));
                    }
                }
            }
        }
        أرجع s;
    }
};

template <typename T, typename ST>
بنية MaskedNormL1_SIMD {
    مضمن ST operator() (ثابت T* src, ثابت uchar* mask, صحيح len, صحيح cn) ثابت {
        ST s = 0;
        إذا (cn == 1) {
            لكل (صحيح i = 0; i < len; i++) {
                إذا (mask[i]) {
                    s += (ST)cv_abs(src[i]);
                }
            }
        }
        وإلا {
            لكل (صحيح i = 0; i < len; i++) {
                إذا (mask[i]) {
                    ثابت T* elem = src + i * cn;
                    صحيح k = 0;
                #إذا CV_ENABLE_UNROLLED && !CV_SIMD_SCALABLE
                    لكل (; k <= cn - 4; k += 4) {
                        s += (ST)cv_abs(elem[k]);
                        s += (ST)cv_abs(elem[k + 1]);
                        s += (ST)cv_abs(elem[k + 2]);
                        s += (ST)cv_abs(elem[k + 3]);
                    }
                #endif
                    لكل (; k < cn; k++) {
                        s += (ST)cv_abs(elem[k]);
                    }
                }
            }
        }
        أرجع s;
    }
};

template <typename T, typename ST>
بنية MaskedNormL2_SIMD {
    مضمن ST operator() (ثابت T* src, ثابت uchar* mask, صحيح len, صحيح cn) ثابت {
        ST s = 0;
        إذا (cn == 1) {
            صحيح i = 0;
        #إذا CV_ENABLE_UNROLLED && !CV_SIMD_SCALABLE
            لكل (; i <= len - 4; i += 4) {
                إذا (mask[i])     { T v0 = src[i];     s += (ST)v0 * v0; }
                إذا (mask[i + 1]) { T v1 = src[i + 1]; s += (ST)v1 * v1; }
                إذا (mask[i + 2]) { T v2 = src[i + 2]; s += (ST)v2 * v2; }
                إذا (mask[i + 3]) { T v3 = src[i + 3]; s += (ST)v3 * v3; }
            }
        #endif
            لكل (; i < len; i++) {
                إذا (mask[i]) {
                    T v = src[i];
                    s += (ST)v * v;
                }
            }
        }
        وإلا {
            لكل (صحيح i = 0; i < len; i++) {
                إذا (mask[i]) {
                    ثابت T* elem = src + i * cn;
                    صحيح k = 0;
                #إذا CV_ENABLE_UNROLLED && !CV_SIMD_SCALABLE
                    لكل (; k <= cn - 4; k += 4) {
                        T v0 = elem[k];     s += (ST)v0 * v0;
                        T v1 = elem[k + 1]; s += (ST)v1 * v1;
                        T v2 = elem[k + 2]; s += (ST)v2 * v2;
                        T v3 = elem[k + 3]; s += (ST)v3 * v3;
                    }
                #endif
                    لكل (; k < cn; k++) {
                        T v = elem[k];
                        s += (ST)v * v;
                    }
                }
            }
        }

        أرجع s;
    }
};

#إذا (CV_SIMD || CV_SIMD_SCALABLE)
ساكن مضمن v_uint8  v_normmask_u8 (ثابت uchar* m) { أرجع v_gt(vx_load(m),          vx_setzero_u8());  }
ساكن مضمن v_uint16 v_normmask_u16(ثابت uchar* m) { أرجع v_gt(vx_load_expand(m),   vx_setzero_u16()); }
ساكن مضمن v_uint32 v_normmask_u32(ثابت uchar* m) { أرجع v_gt(vx_load_expand_q(m), vx_setzero_u32()); }
ساكن مضمن فراغ v_normmask_u64(ثابت uchar* m, v_uint64& m0, v_uint64& m1) {
    v_uint64 e0, e1;
    v_expand(v_normmask_u32(m), e0, e1);
    m0 = v_or(e0, v_shl<32>(e0));
    m1 = v_or(e1, v_shl<32>(e1));
}

template <>
بنية MaskedNormInf_SIMD<عائم, عائم> {
    مضمن عائم operator()(ثابت عائم* src, ثابت uchar* mask, صحيح len, صحيح cn) ثابت {
        عائم result = 0.0f;
        إذا (cn == 1) {
            صحيح i = 0;
            ثابت صحيح vstep = VTraits<v_float32>::vlanes();
            v_float32 acc = vx_setzero_f32();

            لكل (; i <= len - vstep; i += vstep) {
                v_uint32 m = vx_load_expand_q(mask + i);
                v_uint32 cmp = v_gt(m, vx_setzero_u32());
                v_float32 s = vx_load(src + i);
                s = v_abs(s);
                s = v_reinterpret_as_f32(v_and(v_reinterpret_as_u32(s), cmp));
                acc = v_max(acc, s);
            }
            result = v_reduce_max(acc);

            لكل (; i < len; i++) {
                إذا (mask[i])
                    result = std::max(result, std::abs(src[i]));
            }
        }
        وإلا {
            لكل (صحيح i = 0; i < len; i++) {
                إذا (mask[i]) {
                    ثابت عائم* elem = src + i * cn;
                    صحيح k = 0;
                    ثابت صحيح vstep = VTraits<v_float32>::vlanes();
                    v_float32 acc = vx_setzero_f32();

                    لكل (; k <= cn - vstep; k += vstep) {
                        v_float32 s = vx_load(elem + k);
                        acc = v_max(acc, v_abs(s));
                    }

                    result = std::max(result, v_reduce_max(acc));

                    لكل (; k < cn; k++)
                        result = std::max(result, std::abs(elem[k]));
                }
            }
        }
        أرجع result;
    }
};

#إذا CV_SIMD_64F
template <>
بنية MaskedNormL1_SIMD<عائم, مزدوج> {
    مضمن مزدوج operator()(ثابت عائم* src, ثابت uchar* mask, صحيح len, صحيح cn) ثابت {
        مزدوج result = 0.0;
        إذا (cn == 1) {
            صحيح i = 0;
            ثابت صحيح vstep = VTraits<v_float32>::vlanes();
            v_float64 acc0 = vx_setzero_f64(), acc1 = vx_setzero_f64();
            v_float64 acc2 = vx_setzero_f64(), acc3 = vx_setzero_f64();

            لكل (; i <= len - 2*vstep; i += 2*vstep) {
                v_float32 s0 = v_and(v_abs(vx_load(src + i)),
                                     v_reinterpret_as_f32(v_normmask_u32(mask + i)));
                v_float32 s1 = v_and(v_abs(vx_load(src + i + vstep)),
                                     v_reinterpret_as_f32(v_normmask_u32(mask + i + vstep)));
                acc0 = v_add(acc0, v_cvt_f64(s0));
                acc1 = v_add(acc1, v_cvt_f64_بلند(s0));
                acc2 = v_add(acc2, v_cvt_f64(s1));
                acc3 = v_add(acc3, v_cvt_f64_بلند(s1));
            }
            لكل (; i <= len - vstep; i += vstep) {
                v_float32 s = v_and(v_abs(vx_load(src + i)),
                                    v_reinterpret_as_f32(v_normmask_u32(mask + i)));
                acc0 = v_add(acc0, v_cvt_f64(s));
                acc1 = v_add(acc1, v_cvt_f64_بلند(s));
            }
            result = v_reduce_sum(v_add(v_add(acc0, acc1), v_add(acc2, acc3)));

            لكل (; i < len; i++) {
                إذا (mask[i])
                    result += std::abs(src[i]);
            }
        }
        وإلا {
            لكل (صحيح i = 0; i < len; i++) {
                إذا (mask[i]) {
                    ثابت عائم* elem = src + i * cn;
                    صحيح k = 0;
                    ثابت صحيح vstep = VTraits<v_float32>::vlanes();
                    v_float64 acc = vx_setzero_f64();

                    لكل (; k <= cn - vstep; k += vstep) {
                        v_float32 s = v_abs(vx_load(elem + k));
                        acc = v_add(acc, v_cvt_f64(s));
                        acc = v_add(acc, v_cvt_f64_بلند(s));
                    }

                    result += v_reduce_sum(acc);

                    لكل (; k < cn; k++)
                        result += std::abs(elem[k]);
                }
            }
        }
        أرجع result;
    }
};

template <>
بنية MaskedNormL2_SIMD<عائم, مزدوج> {
    مضمن مزدوج operator()(ثابت عائم* src, ثابت uchar* mask, صحيح len, صحيح cn) ثابت {
        مزدوج result = 0.0;
        إذا (cn == 1) {
            صحيح i = 0;
            ثابت صحيح vstep = VTraits<v_float32>::vlanes();
            v_float32 facc = vx_setzero_f32();
            v_float64 dacc = vx_setzero_f64();
            صحيح flush = 0;

            لكل (; i <= len - vstep; i += vstep, flush += vstep) {
                إذا (flush >= 64) {
                    dacc = v_add(dacc, v_cvt_f64(facc));
                    dacc = v_add(dacc, v_cvt_f64_بلند(facc));
                    facc = vx_setzero_f32();
                    flush = 0;
                }
                v_uint32 cmp = v_gt(vx_load_expand_q(mask + i), vx_setzero_u32());
                v_float32 s  = v_reinterpret_as_f32(v_and(v_reinterpret_as_u32(vx_load(src + i)), cmp));
                facc = v_add(facc, v_mul(s, s));
            }

            dacc = v_add(dacc, v_cvt_f64(facc));
            dacc = v_add(dacc, v_cvt_f64_بلند(facc));
            result = v_reduce_sum(dacc);

            لكل (; i < len; i++) {
                إذا (mask[i]) {
                    مزدوج v = src[i];
                    result += v * v;
                }
            }
        }
        وإلا {
            لكل (صحيح i = 0; i < len; i++) {
                إذا (mask[i]) {
                    ثابت عائم* elem = src + i * cn;
                    صحيح k = 0;
                    ثابت صحيح vstep = VTraits<v_float32>::vlanes();
                    v_float32 facc = vx_setzero_f32();

                    لكل (; k <= cn - vstep; k += vstep) {
                        v_float32 s = vx_load(elem + k);
                        facc = v_add(facc, v_mul(s, s));
                    }

                    v_float64 dacc = v_add(v_cvt_f64(facc), v_cvt_f64_بلند(facc));
                    result += v_reduce_sum(dacc);

                    لكل (; k < cn; k++) {
                        مزدوج v = elem[k];
                        result += v * v;
                    }
                }
            }
        }
        أرجع result;
    }
};

#endif

template <>
بنية MaskedNormInf_SIMD<uchar, صحيح> {
    مضمن صحيح operator()(ثابت uchar* src, ثابت uchar* mask, صحيح len, صحيح cn) ثابت {
        صحيح result = 0;
        إذا (cn == 1) {
            صحيح i = 0;
            ثابت صحيح vstep = VTraits<v_uint8>::vlanes();
            v_uint8 acc = vx_setzero_u8();

            لكل (; i <= len - vstep; i += vstep) {
                v_uint8 m   = vx_load(mask + i);
                v_uint8 s   = vx_load(src + i);
                v_uint8 sel = v_and(s, v_gt(m, vx_setzero_u8()));
                acc = v_max(acc, sel);
            }

            result = (صحيح)v_reduce_max(acc);

            لكل (; i < len; i++) {
                إذا (mask[i])
                    result = std::max(result, (صحيح)src[i]);
            }
        }
        وإلا إذا (cn == 4 && len >= VTraits<v_uint8>::vlanes()) {
            ثابت صحيح vstep = VTraits<v_uint8>::vlanes();
            v_uint8 acc = vx_setzero_u8();
            صحيح i = 0;
            لكل (;;) {
                إذا (i > len - vstep)
                    i = len - vstep; // back-step (max is idempotent)
                v_uint8 c0, c1, c2, c3;
                v_load_deinterleave(src + i * 4, c0, c1, c2, c3);
                v_uint8 pmax = v_max(v_max(c0, c1), v_max(c2, c3));
                v_uint8 m = v_gt(vx_load(mask + i), vx_setzero_u8());
                acc = v_max(acc, v_and(pmax, m));
                إذا (i >= len - vstep)
                    اكسر;
                i += vstep;
            }
            result = (صحيح)v_reduce_max(acc);
        }
        وإلا إذا (cn == 4) {
            لكل (صحيح i = 0; i < len; i++) {
                إذا (mask[i]) {
                    ثابت uchar* elem = src + i * 4;
                    result = std::max(result, (صحيح)std::max(std::max(elem[0], elem[1]),
                                                            std::max(elem[2], elem[3])));
                }
            }
        }
        وإلا {
            لكل (صحيح i = 0; i < len; i++) {
                إذا (mask[i]) {
                    ثابت uchar* elem = src + i * cn;
                    لكل (صحيح k = 0; k < cn; k++)
                        result = std::max(result, (صحيح)elem[k]);
                }
            }
        }
        أرجع result;
    }
};

template <>
بنية MaskedNormL1_SIMD<uchar, صحيح> {
    مضمن صحيح operator()(ثابت uchar* src, ثابت uchar* mask, صحيح len, صحيح cn) ثابت {
        صحيح result = 0;
        ثابت صحيح vstep = VTraits<v_uint8>::vlanes();
        ثابت v_uint8 one = vx_setall_u8(1);
        إذا (cn == 1) {
            صحيح i = 0;
            v_uint32 acc = vx_setzero_u32();
            لكل (; i <= len - vstep; i += vstep) {
                v_uint8 s = v_and(vx_load(src + i), v_normmask_u8(mask + i));
                acc = v_dotprod_expand_fast(s, one, acc);
            }
            result = (صحيح)v_reduce_sum(acc);
            لكل (; i < len; i++) {
                إذا (mask[i])
                    result += src[i];
            }
        }
        وإلا إذا (cn == 4) {
            صحيح i = 0;
            v_uint32 acc = vx_setzero_u32();
            لكل (; i <= len - vstep; i += vstep) {
                v_uint8 c0, c1, c2, c3;
                v_load_deinterleave(src + i * 4, c0, c1, c2, c3);
                v_uint8 m = v_normmask_u8(mask + i);
                acc = v_dotprod_expand_fast(v_and(c0, m), one, acc);
                acc = v_dotprod_expand_fast(v_and(c1, m), one, acc);
                acc = v_dotprod_expand_fast(v_and(c2, m), one, acc);
                acc = v_dotprod_expand_fast(v_and(c3, m), one, acc);
            }
            result = (صحيح)v_reduce_sum(acc);
            لكل (; i < len; i++) {
                إذا (mask[i]) {
                    ثابت uchar* elem = src + i * 4;
                    result += elem[0] + elem[1] + elem[2] + elem[3];
                }
            }
        }
        وإلا {
            لكل (صحيح i = 0; i < len; i++) {
                إذا (mask[i]) {
                    ثابت uchar* elem = src + i * cn;
                    لكل (صحيح k = 0; k < cn; k++)
                        result += elem[k];
                }
            }
        }
        أرجع result;
    }
};

template <>
بنية MaskedNormInf_SIMD<ushort, صحيح> {
    مضمن صحيح operator()(ثابت ushort* src, ثابت uchar* mask, صحيح len, صحيح cn) ثابت {
        صحيح result = 0;
        إذا (cn == 1) {
            صحيح i = 0;
            ثابت صحيح vstep = VTraits<v_uint16>::vlanes();
            v_uint16 acc = vx_setzero_u16();

            لكل (; i <= len - vstep; i += vstep) {
                v_uint16 m   = vx_load_expand(mask + i);
                v_uint16 cmp = v_gt(m, vx_setzero_u16());
                v_uint16 s   = vx_load(src + i);
                v_uint16 sel = v_and(s, cmp);
                acc = v_max(acc, sel);
            }

            result = (صحيح)v_reduce_max(acc);

            لكل (; i < len; i++) {
                إذا (mask[i])
                    result = std::max(result, (صحيح)src[i]);
            }
        }
        وإلا {
            لكل (صحيح i = 0; i < len; i++) {
                إذا (mask[i]) {
                    ثابت ushort* elem = src + i * cn;
                    صحيح k = 0;
                    ثابت صحيح vstep = VTraits<v_uint16>::vlanes();
                    v_uint16 acc = vx_setzero_u16();

                    لكل (; k <= cn - vstep; k += vstep) {
                        acc = v_max(acc, vx_load(elem + k));
                    }

                    result = std::max(result, (صحيح)v_reduce_max(acc));

                    لكل (; k < cn; k++)
                        result = std::max(result, (صحيح)elem[k]);
                }
            }
        }
        أرجع result;
    }
};

template <>
بنية MaskedNormL1_SIMD<ushort, صحيح> {
    مضمن صحيح operator()(ثابت ushort* src, ثابت uchar* mask, صحيح len, صحيح cn) ثابت {
        صحيح result = 0;
        إذا (cn == 1) {
            صحيح i = 0;
            ثابت صحيح vstep = VTraits<v_uint16>::vlanes();
            v_uint32 acc32 = vx_setzero_u32();
            v_uint64 acc64 = vx_setzero_u64();
            صحيح acc32_elems = 0;

            لكل (; i <= len - vstep; i += vstep, acc32_elems += vstep) {
                إذا (acc32_elems >= 512) {
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
            result = (صحيح)v_reduce_sum(acc64);

            لكل (; i < len; i++) {
                إذا (mask[i])
                    result += src[i];
            }
        }
        وإلا {
            لكل (صحيح i = 0; i < len; i++) {
                إذا (mask[i]) {
                    ثابت ushort* elem = src + i * cn;
                    صحيح k = 0;
                    ثابت صحيح vstep = VTraits<v_uint16>::vlanes();
                    v_uint32 acc = vx_setzero_u32();

                    لكل (; k <= cn - vstep; k += vstep) {
                        v_uint32 lo32, hi32;
                        v_expand(vx_load(elem + k), lo32, hi32);
                        acc = v_add(acc, v_add(lo32, hi32));
                    }

                    result += (صحيح)v_reduce_sum(acc);

                    لكل (; k < cn; k++)
                        result += elem[k];
                }
            }
        }
        أرجع result;
    }
};

template <>
بنية MaskedNormL2_SIMD<ushort, مزدوج> {
    مضمن مزدوج operator()(ثابت ushort* src, ثابت uchar* mask, صحيح len, صحيح cn) ثابت {
        مزدوج result = 0.0;
        إذا (cn == 1) {
            صحيح i = 0;
            ثابت صحيح vstep = VTraits<v_uint16>::vlanes();
            v_uint64 acc = vx_setzero_u64();
            لكل (; i <= len - vstep; i += vstep) {
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
            result = (مزدوج)v_reduce_sum(acc);
            لكل (; i < len; i++) {
                إذا (mask[i]) {
                    مزدوج v = src[i];
                    result += v * v;
                }
            }
        }
        وإلا {
            لكل (صحيح i = 0; i < len; i++) {
                إذا (mask[i]) {
                    ثابت ushort* elem = src + i * cn;
                    صحيح k = 0;
                    ثابت صحيح vstep = VTraits<v_uint16>::vlanes();
                    v_uint64 acc = vx_setzero_u64();
                    لكل (; k <= cn - vstep; k += vstep) {
                        v_uint32 lo32, hi32;
                        v_expand(vx_load(elem + k), lo32, hi32);
                        v_uint64 lo64a, lo64b, hi64a, hi64b;
                        v_expand(v_mul(lo32, lo32), lo64a, lo64b);
                        v_expand(v_mul(hi32, hi32), hi64a, hi64b);
                        acc = v_add(acc, v_add(v_add(lo64a, lo64b), v_add(hi64a, hi64b)));
                    }

                    result += (مزدوج)v_reduce_sum(acc);

                    لكل (; k < cn; k++) {
                        مزدوج v = elem[k];
                        result += v * v;
                    }
                }
            }
        }
        أرجع result;
    }
};

template <>
بنية MaskedNormInf_SIMD<schar, صحيح> {
    مضمن صحيح operator()(ثابت schar* src, ثابت uchar* mask, صحيح len, صحيح cn) ثابت {
        صحيح result = 0;
        إذا (cn == 1) {
            ثابت صحيح vstep = VTraits<v_int8>::vlanes();
            صحيح i = 0;
            v_uint8 acc = vx_setzero_u8();
            لكل (; i <= len - vstep; i += vstep)
                acc = v_max(acc, v_and(v_abs(vx_load(src + i)), v_normmask_u8(mask + i)));
            result = (صحيح)v_reduce_max(acc);
            لكل (; i < len; i++)
                إذا (mask[i]) result = std::max(result, cv_abs(src[i]));
        }
        وإلا {
            لكل (صحيح i = 0; i < len; i++)
                إذا (mask[i]) {
                    ثابت schar* elem = src + i * cn;
                    لكل (صحيح k = 0; k < cn; k++)
                        result = std::max(result, cv_abs(elem[k]));
                }
        }
        أرجع result;
    }
};

template <>
بنية MaskedNormL1_SIMD<schar, صحيح> {
    مضمن صحيح operator()(ثابت schar* src, ثابت uchar* mask, صحيح len, صحيح cn) ثابت {
        صحيح result = 0;
        إذا (cn == 1) {
            ثابت صحيح vstep = VTraits<v_int8>::vlanes();
            ثابت v_uint8 one = vx_setall_u8(1);
            صحيح i = 0;
            v_uint32 acc = vx_setzero_u32();
            لكل (; i <= len - vstep; i += vstep)
                acc = v_dotprod_expand_fast(v_and(v_abs(vx_load(src + i)), v_normmask_u8(mask + i)), one, acc);
            result = (صحيح)v_reduce_sum(acc);
            لكل (; i < len; i++)
                إذا (mask[i]) result += cv_abs(src[i]);
        }
        وإلا {
            لكل (صحيح i = 0; i < len; i++)
                إذا (mask[i]) {
                    ثابت schar* elem = src + i * cn;
                    لكل (صحيح k = 0; k < cn; k++)
                        result += cv_abs(elem[k]);
                }
        }
        أرجع result;
    }
};

template <>
بنية MaskedNormL2_SIMD<schar, صحيح> {
    مضمن صحيح operator()(ثابت schar* src, ثابت uchar* mask, صحيح len, صحيح cn) ثابت {
        صحيح result = 0;
        إذا (cn == 1) {
            ثابت صحيح vstep = VTraits<v_int8>::vlanes();
            صحيح i = 0;
            v_int32 acc = vx_setzero_s32();
            لكل (; i <= len - vstep; i += vstep) {
                v_int8 s = v_and(vx_load(src + i), v_reinterpret_as_s8(v_normmask_u8(mask + i)));
                acc = v_dotprod_expand_fast(s, s, acc);
            }
            result = (صحيح)v_reduce_sum(acc);
            لكل (; i < len; i++)
                إذا (mask[i]) { صحيح v = src[i]; result += v * v; }
        }
        وإلا {
            لكل (صحيح i = 0; i < len; i++)
                إذا (mask[i]) {
                    ثابت schar* elem = src + i * cn;
                    لكل (صحيح k = 0; k < cn; k++) { صحيح v = elem[k]; result += v * v; }
                }
        }
        أرجع result;
    }
};

template <>
بنية MaskedNormL2_SIMD<uchar, صحيح> {
    مضمن صحيح operator()(ثابت uchar* src, ثابت uchar* mask, صحيح len, صحيح cn) ثابت {
        صحيح result = 0;
        ثابت صحيح vstep = VTraits<v_uint8>::vlanes();
        إذا (cn == 1) {
            صحيح i = 0;
            v_uint32 acc = vx_setzero_u32();
            لكل (; i <= len - vstep; i += vstep) {
                v_uint8 s = v_and(vx_load(src + i), v_normmask_u8(mask + i));
                acc = v_dotprod_expand_fast(s, s, acc);
            }
            result = (صحيح)v_reduce_sum(acc);
            لكل (; i < len; i++)
                إذا (mask[i]) { صحيح v = src[i]; result += v * v; }
        }
        وإلا إذا (cn == 4) {
            صحيح i = 0;
            v_uint32 acc = vx_setzero_u32();
            لكل (; i <= len - vstep; i += vstep) {
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
            result = (صحيح)v_reduce_sum(acc);
            لكل (; i < len; i++)
                إذا (mask[i]) {
                    ثابت uchar* elem = src + i * 4;
                    لكل (صحيح k = 0; k < 4; k++) { صحيح v = elem[k]; result += v * v; }
                }
        }
        وإلا {
            لكل (صحيح i = 0; i < len; i++)
                إذا (mask[i]) {
                    ثابت uchar* elem = src + i * cn;
                    لكل (صحيح k = 0; k < cn; k++) { صحيح v = elem[k]; result += v * v; }
                }
        }
        أرجع result;
    }
};

template <>
بنية MaskedNormInf_SIMD<short, صحيح> {
    مضمن صحيح operator()(ثابت short* src, ثابت uchar* mask, صحيح len, صحيح cn) ثابت {
        صحيح result = 0;
        إذا (cn == 1) {
            ثابت صحيح vstep = VTraits<v_int16>::vlanes();
            صحيح i = 0;
            v_uint16 acc = vx_setzero_u16();
            لكل (; i <= len - vstep; i += vstep)
                acc = v_max(acc, v_and(v_abs(vx_load(src + i)), v_normmask_u16(mask + i)));
            result = (صحيح)v_reduce_max(acc);
            لكل (; i < len; i++)
                إذا (mask[i]) result = std::max(result, cv_abs(src[i]));
        }
        وإلا {
            لكل (صحيح i = 0; i < len; i++)
                إذا (mask[i]) {
                    ثابت short* elem = src + i * cn;
                    لكل (صحيح k = 0; k < cn; k++)
                        result = std::max(result, cv_abs(elem[k]));
                }
        }
        أرجع result;
    }
};

template <>
بنية MaskedNormL1_SIMD<short, صحيح> {
    مضمن صحيح operator()(ثابت short* src, ثابت uchar* mask, صحيح len, صحيح cn) ثابت {
        صحيح result = 0;
        إذا (cn == 1) {
            ثابت صحيح vstep = VTraits<v_int16>::vlanes();
            صحيح i = 0;
            v_uint32 acc0 = vx_setzero_u32(), acc1 = vx_setzero_u32();
            لكل (; i <= len - vstep; i += vstep) {
                v_uint16 s = v_and(v_abs(vx_load(src + i)), v_normmask_u16(mask + i));
                v_uint32 lo, hi;
                v_expand(s, lo, hi);
                acc0 = v_add(acc0, lo);
                acc1 = v_add(acc1, hi);
            }
            result = (صحيح)v_reduce_sum(v_add(acc0, acc1));
            لكل (; i < len; i++)
                إذا (mask[i]) result += cv_abs(src[i]);
        }
        وإلا {
            لكل (صحيح i = 0; i < len; i++)
                إذا (mask[i]) {
                    ثابت short* elem = src + i * cn;
                    لكل (صحيح k = 0; k < cn; k++)
                        result += cv_abs(elem[k]);
                }
        }
        أرجع result;
    }
};

template <>
بنية MaskedNormL2_SIMD<short, مزدوج> {
    مضمن مزدوج operator()(ثابت short* src, ثابت uchar* mask, صحيح len, صحيح cn) ثابت {
        مزدوج result = 0.0;
        إذا (cn == 1) {
            ثابت صحيح vstep = VTraits<v_int16>::vlanes();
            صحيح i = 0;
            v_uint64 acc = vx_setzero_u64();
            لكل (; i <= len - vstep; i += vstep) {
                v_uint16 s = v_and(v_abs(vx_load(src + i)), v_normmask_u16(mask + i));
                acc = v_add(acc, v_dotprod_expand_fast(s, s));
            }
            result = (مزدوج)v_reduce_sum(acc);
            لكل (; i < len; i++)
                إذا (mask[i]) { مزدوج v = src[i]; result += v * v; }
        }
        وإلا {
            لكل (صحيح i = 0; i < len; i++)
                إذا (mask[i]) {
                    ثابت short* elem = src + i * cn;
                    لكل (صحيح k = 0; k < cn; k++) { مزدوج v = elem[k]; result += v * v; }
                }
        }
        أرجع result;
    }
};

template <>
بنية MaskedNormInf_SIMD<صحيح, unsigned> {
    مضمن unsigned operator()(ثابت صحيح* src, ثابت uchar* mask, صحيح len, صحيح cn) ثابت {
        unsigned result = 0;
        إذا (cn == 1) {
            ثابت صحيح vstep = VTraits<v_int32>::vlanes();
            صحيح i = 0;
            v_uint32 acc = vx_setzero_u32();
            لكل (; i <= len - vstep; i += vstep)
                acc = v_max(acc, v_and(v_abs(vx_load(src + i)), v_normmask_u32(mask + i)));
            result = v_reduce_max(acc);
            لكل (; i < len; i++)
                إذا (mask[i]) result = std::max(result, cv_abs(src[i]));
        }
        وإلا {
            لكل (صحيح i = 0; i < len; i++)
                إذا (mask[i]) {
                    ثابت صحيح* elem = src + i * cn;
                    لكل (صحيح k = 0; k < cn; k++)
                        result = std::max(result, cv_abs(elem[k]));
                }
        }
        أرجع result;
    }
};

#إذا CV_SIMD_64F || CV_SIMD_SCALABLE_64F

template <>
بنية MaskedNormL1_SIMD<صحيح, مزدوج> {
    مضمن مزدوج operator()(ثابت صحيح* src, ثابت uchar* mask, صحيح len, صحيح cn) ثابت {
        مزدوج result = 0.0;
        إذا (cn == 1) {
            ثابت صحيح vstep = VTraits<v_int32>::vlanes();
            صحيح i = 0;
            v_float64 acc0 = vx_setzero_f64(), acc1 = vx_setzero_f64();
            لكل (; i <= len - vstep; i += vstep) {
                // Mask the value, then widen and take the magnitude in double. v_abs() does give
                // |INT_MIN| as an unsigned 2147483648, but reinterpreting that back as int32 turns
                // it negative again, and the widening then carries the sign into the accumulator.
                v_int32 s = v_reinterpret_as_s32(v_and(v_reinterpret_as_u32(vx_load(src + i)),
                                                       v_normmask_u32(mask + i)));
                acc0 = v_add(acc0, v_abs(v_cvt_f64(s)));
                acc1 = v_add(acc1, v_abs(v_cvt_f64_بلند(s)));
            }
            result = v_reduce_sum(v_add(acc0, acc1));
            لكل (; i < len; i++)
                إذا (mask[i]) result += cv_abs(src[i]);
        }
        وإلا {
            لكل (صحيح i = 0; i < len; i++)
                إذا (mask[i]) {
                    ثابت صحيح* elem = src + i * cn;
                    لكل (صحيح k = 0; k < cn; k++)
                        result += cv_abs(elem[k]);
                }
        }
        أرجع result;
    }
};

template <>
بنية MaskedNormL2_SIMD<صحيح, مزدوج> {
    مضمن مزدوج operator()(ثابت صحيح* src, ثابت uchar* mask, صحيح len, صحيح cn) ثابت {
        مزدوج result = 0.0;
        إذا (cn == 1) {
            ثابت صحيح vstep = VTraits<v_int32>::vlanes();
            صحيح i = 0;
            v_float64 acc0 = vx_setzero_f64(), acc1 = vx_setzero_f64();
            لكل (; i <= len - vstep; i += vstep) {
                v_uint32 s = v_and(v_abs(vx_load(src + i)), v_normmask_u32(mask + i));
                v_uint64 e0, e1;
                v_expand(s, e0, e1);
                v_float64 f0 = v_cvt_f64(v_reinterpret_as_s64(e0));
                v_float64 f1 = v_cvt_f64(v_reinterpret_as_s64(e1));
                acc0 = v_fma(f0, f0, acc0);
                acc1 = v_fma(f1, f1, acc1);
            }
            result = v_reduce_sum(v_add(acc0, acc1));
            لكل (; i < len; i++)
                إذا (mask[i]) { مزدوج v = src[i]; result += v * v; }
        }
        وإلا {
            لكل (صحيح i = 0; i < len; i++)
                إذا (mask[i]) {
                    ثابت صحيح* elem = src + i * cn;
                    لكل (صحيح k = 0; k < cn; k++) { مزدوج v = elem[k]; result += v * v; }
                }
        }
        أرجع result;
    }
};

template <>
بنية MaskedNormInf_SIMD<مزدوج, مزدوج> {
    مضمن مزدوج operator()(ثابت مزدوج* src, ثابت uchar* mask, صحيح len, صحيح cn) ثابت {
        مزدوج result = 0.0;
        إذا (cn == 1) {
            ثابت صحيح vstep = VTraits<v_float64>::vlanes();
            صحيح i = 0;
            v_float64 acc0 = vx_setzero_f64(), acc1 = vx_setzero_f64();
            لكل (; i <= len - 2*vstep; i += 2*vstep) {
                v_uint64 m0, m1;
                v_normmask_u64(mask + i, m0, m1);
                acc0 = v_max(acc0, v_reinterpret_as_f64(v_and(v_reinterpret_as_u64(v_abs(vx_load(src + i))), m0)));
                acc1 = v_max(acc1, v_reinterpret_as_f64(v_and(v_reinterpret_as_u64(v_abs(vx_load(src + i + vstep))), m1)));
            }
            مزدوج t[VTraits<v_float64>::max_nlanes];
            vx_store(t, v_max(acc0, acc1));
            لكل (صحيح k = 0; k < vstep; k++)
                result = std::max(result, t[k]);
            لكل (; i < len; i++)
                إذا (mask[i]) result = std::max(result, cv_abs(src[i]));
        }
        وإلا {
            لكل (صحيح i = 0; i < len; i++)
                إذا (mask[i]) {
                    ثابت مزدوج* elem = src + i * cn;
                    لكل (صحيح k = 0; k < cn; k++)
                        result = std::max(result, cv_abs(elem[k]));
                }
        }
        أرجع result;
    }
};

template <>
بنية MaskedNormL1_SIMD<مزدوج, مزدوج> {
    مضمن مزدوج operator()(ثابت مزدوج* src, ثابت uchar* mask, صحيح len, صحيح cn) ثابت {
        مزدوج result = 0.0;
        إذا (cn == 1) {
            ثابت صحيح vstep = VTraits<v_float64>::vlanes();
            صحيح i = 0;
            v_float64 acc0 = vx_setzero_f64(), acc1 = vx_setzero_f64();
            لكل (; i <= len - 2*vstep; i += 2*vstep) {
                v_uint64 m0, m1;
                v_normmask_u64(mask + i, m0, m1);
                acc0 = v_add(acc0, v_reinterpret_as_f64(v_and(v_reinterpret_as_u64(v_abs(vx_load(src + i))), m0)));
                acc1 = v_add(acc1, v_reinterpret_as_f64(v_and(v_reinterpret_as_u64(v_abs(vx_load(src + i + vstep))), m1)));
            }
            result = v_reduce_sum(v_add(acc0, acc1));
            لكل (; i < len; i++)
                إذا (mask[i]) result += cv_abs(src[i]);
        }
        وإلا {
            لكل (صحيح i = 0; i < len; i++)
                إذا (mask[i]) {
                    ثابت مزدوج* elem = src + i * cn;
                    لكل (صحيح k = 0; k < cn; k++)
                        result += cv_abs(elem[k]);
                }
        }
        أرجع result;
    }
};

template <>
بنية MaskedNormL2_SIMD<مزدوج, مزدوج> {
    مضمن مزدوج operator()(ثابت مزدوج* src, ثابت uchar* mask, صحيح len, صحيح cn) ثابت {
        مزدوج result = 0.0;
        إذا (cn == 1) {
            ثابت صحيح vstep = VTraits<v_float64>::vlanes();
            صحيح i = 0;
            v_float64 acc0 = vx_setzero_f64(), acc1 = vx_setzero_f64();
            لكل (; i <= len - 2*vstep; i += 2*vstep) {
                v_uint64 m0, m1;
                v_normmask_u64(mask + i, m0, m1);
                v_float64 s0 = v_reinterpret_as_f64(v_and(v_reinterpret_as_u64(vx_load(src + i)), m0));
                v_float64 s1 = v_reinterpret_as_f64(v_and(v_reinterpret_as_u64(vx_load(src + i + vstep)), m1));
                acc0 = v_fma(s0, s0, acc0);
                acc1 = v_fma(s1, s1, acc1);
            }
            result = v_reduce_sum(v_add(acc0, acc1));
            لكل (; i < len; i++)
                إذا (mask[i]) { مزدوج v = src[i]; result += v * v; }
        }
        وإلا {
            لكل (صحيح i = 0; i < len; i++)
                إذا (mask[i]) {
                    ثابت مزدوج* elem = src + i * cn;
                    لكل (صحيح k = 0; k < cn; k++) { مزدوج v = elem[k]; result += v * v; }
                }
        }
        أرجع result;
    }
};

#endif // CV_SIMD_64F || CV_SIMD_SCALABLE_64F
#endif // CV_SIMD || CV_SIMD_SCALABLE

template<typename T, typename ST> صحيح
normInf_(ثابت T* src, ثابت uchar* mask, ST* _result, صحيح len, صحيح cn)
{
    ST result = *_result;
    إذا( !mask )
    {
        NormInf_SIMD<T, ST> op;
        result = std::max(result, op(src, len*cn));
    } وإلا {
        MaskedNormInf_SIMD<T, ST> op;
        result = std::max(result, op(src, mask, len, cn));
    }
    *_result = result;
    أرجع 0;
}

template<typename T, typename ST> صحيح
normL1_(ثابت T* src, ثابت uchar* mask, ST* _result, صحيح len, صحيح cn)
{
    ST result = *_result;
    إذا( !mask )
    {
        NormL1_SIMD<T, ST> op;
        result += op(src, len*cn);
    } وإلا {
        MaskedNormL1_SIMD<T, ST> op;
        result += op(src, mask, len, cn);
    }
    *_result = result;
    أرجع 0;
}

template<typename T, typename ST> صحيح
normL2_(ثابت T* src, ثابت uchar* mask, ST* _result, صحيح len, صحيح cn)
{
    ST result = *_result;
    إذا( !mask )
    {
        NormL2_SIMD<T, ST> op;
        result += op(src, len*cn);
    }
    وإلا
    {
        MaskedNormL2_SIMD<T, ST> op;
        result += op(src, mask, len, cn);
    }
    *_result = result;
    أرجع 0;
}

ساكن صحيح
normInf_Bool(ثابت uchar* src, ثابت uchar* mask, صحيح* _result, صحيح len, صحيح cn)
{
    صحيح result = *_result;
    إذا( !mask )
    {
        لكل ( صحيح i = 0; i < len*cn; i++ ) {
            result = std::max(result, (صحيح)(src[i] != 0));
            إذا (result != 0)
                اكسر;
        }
    }
    وإلا
    {
        لكل( صحيح i = 0; i < len; i++, src += cn )
            إذا( mask[i] )
            {
                لكل( صحيح k = 0; k < cn; k++ )
                    result = std::max(result, (صحيح)(src[k] != 0));
                إذا (result != 0)
                    اكسر;
            }
    }
    *_result = result;
    أرجع 0;
}

ساكن صحيح
normL1_Bool(ثابت uchar* src, ثابت uchar* mask, صحيح* _result, صحيح len, صحيح cn)
{
    صحيح result = *_result;
    إذا( !mask )
    {
        لكل ( صحيح i = 0; i < len*cn; i++ )
            result += (صحيح)(src[i] != 0);
    }
    وإلا
    {
        لكل( صحيح i = 0; i < len; i++, src += cn )
            إذا( mask[i] )
            {
                لكل( صحيح k = 0; k < cn; k++ )
                    result += (صحيح)(src[k] != 0);
            }
    }
    *_result = result;
    أرجع 0;
}

ساكن صحيح
normL2_Bool(ثابت uchar* src, ثابت uchar* mask, صحيح* _result, صحيح len, صحيح cn)
{
    أرجع normL1_Bool(src, mask, _result, len, cn);
}

// ====================================================================
// Masked norm-diff kernels.
// Scalar base templates (safe for every depth) plus SIMD cn==1
// specializations for the concrete (T, ST) pairs dispatched by
// CV_DEF_NORM_ALL. The masked normDiff*_ functions below select them.
// ====================================================================
template <typename T, typename ST>
بنية MaskedNormDiffInf_SIMD {
    مضمن ST operator()(ثابت T* s1, ثابت T* s2, ثابت uchar* mask, صحيح len, صحيح cn) ثابت {
        ST result = 0;
        لكل (صحيح i = 0; i < len; i++) إذا (mask[i]) {
            ثابت T* e1 = s1 + i*cn; ثابت T* e2 = s2 + i*cn;
            لكل (صحيح k = 0; k < cn; k++)
                result = std::max(result, (ST)cv_absdiff(e1[k], e2[k]));
        }
        أرجع result;
    }
};
template <typename T, typename ST>
بنية MaskedNormDiffL1_SIMD {
    مضمن ST operator()(ثابت T* s1, ثابت T* s2, ثابت uchar* mask, صحيح len, صحيح cn) ثابت {
        ST result = 0;
        لكل (صحيح i = 0; i < len; i++) إذا (mask[i]) {
            ثابت T* e1 = s1 + i*cn; ثابت T* e2 = s2 + i*cn;
            لكل (صحيح k = 0; k < cn; k++)
                result += (ST)cv_absdiff(e1[k], e2[k]);
        }
        أرجع result;
    }
};
template <typename T, typename ST>
بنية MaskedNormDiffL2_SIMD {
    مضمن ST operator()(ثابت T* s1, ثابت T* s2, ثابت uchar* mask, صحيح len, صحيح cn) ثابت {
        ST result = 0;
        لكل (صحيح i = 0; i < len; i++) إذا (mask[i]) {
            ثابت T* e1 = s1 + i*cn; ثابت T* e2 = s2 + i*cn;
            لكل (صحيح k = 0; k < cn; k++) { ST v = (ST)e1[k] - (ST)e2[k]; result += v*v; }
        }
        أرجع result;
    }
};

#إذا (CV_SIMD || CV_SIMD_SCALABLE)
// AND a per-lane diff vector with the (expanded) mask>0 predicate.
مضمن v_uint8  v_diffmask(ثابت v_uint8&  d, ثابت uchar* m) { أرجع v_and(d, v_gt(vx_load(m),          vx_setzero_u8())); }
مضمن v_uint16 v_diffmask(ثابت v_uint16& d, ثابت uchar* m) { أرجع v_and(d, v_gt(vx_load_expand(m),   vx_setzero_u16())); }
مضمن v_uint32 v_diffmask(ثابت v_uint32& d, ثابت uchar* m) { أرجع v_and(d, v_gt(vx_load_expand_q(m), vx_setzero_u32())); }
مضمن v_float32 v_diffmask(ثابت v_float32& d, ثابت uchar* m) {
    v_uint32 cm = v_gt(vx_load_expand_q(m), vx_setzero_u32());
    أرجع v_reinterpret_as_f32(v_and(v_reinterpret_as_u32(d), cm));
}

// Shared scalar cn>1 fallbacks (match the base templates' cv_absdiff semantics).
template<typename T, typename RT>
ساكن مضمن RT maskedNormDiffInfTail(ثابت T* s1, ثابت T* s2, ثابت uchar* mask, صحيح len, صحيح cn, RT result) {
    لكل (صحيح i = 0; i < len; i++) إذا (mask[i]) {
        ثابت T* e1 = s1 + i*cn; ثابت T* e2 = s2 + i*cn;
        لكل (صحيح k = 0; k < cn; k++) result = std::max(result, (RT)cv_absdiff(e1[k], e2[k]));
    }
    أرجع result;
}
template<typename T, typename RT>
ساكن مضمن RT maskedNormDiffL1Tail(ثابت T* s1, ثابت T* s2, ثابت uchar* mask, صحيح len, صحيح cn, RT result) {
    لكل (صحيح i = 0; i < len; i++) إذا (mask[i]) {
        ثابت T* e1 = s1 + i*cn; ثابت T* e2 = s2 + i*cn;
        لكل (صحيح k = 0; k < cn; k++) result += (RT)cv_absdiff(e1[k], e2[k]);
    }
    أرجع result;
}
template<typename T, typename RT>
ساكن مضمن RT maskedNormDiffL2Tail(ثابت T* s1, ثابت T* s2, ثابت uchar* mask, صحيح len, صحيح cn, RT result) {
    لكل (صحيح i = 0; i < len; i++) إذا (mask[i]) {
        ثابت T* e1 = s1 + i*cn; ثابت T* e2 = s2 + i*cn;
        لكل (صحيح k = 0; k < cn; k++) { RT v = (RT)e1[k] - (RT)e2[k]; result += v*v; }
    }
    أرجع result;
}

// Inf (cn==1): reduce_max of masked |a-b|. Shared across SIMD-capable depths.
template<typename T, typename ST>
ساكن مضمن ST maskedNormDiffInfCn1(ثابت T* s1, ثابت T* s2, ثابت uchar* mask, صحيح len) {
    ثابت صحيح vstep = VTraits<decltype(vx_load(s1))>::vlanes();
    ST result = 0;
    صحيح i = 0;
    إذا (len >= vstep) {
        auto acc = v_diffmask(v_absdiff(vx_load(s1), vx_load(s2)), mask);
        لكل (i = vstep; i <= len - vstep; i += vstep)
            acc = v_max(acc, v_diffmask(v_absdiff(vx_load(s1 + i), vx_load(s2 + i)), mask + i));
        result = (ST)v_reduce_max(acc);
    }
    لكل (; i < len; i++) إذا (mask[i]) result = std::max(result, (ST)cv_absdiff(s1[i], s2[i]));
    أرجع result;
}

// Shared 8-bit cn==1 masked L1/L2 kernels (uchar/schar): v_absdiff yields v_uint8 for both.
template<typename T>
ساكن مضمن صحيح maskedNormDiffL1_8(ثابت T* s1, ثابت T* s2, ثابت uchar* mask, صحيح len) {
    صحيح i = 0; ثابت صحيح vstep = VTraits<v_uint8>::vlanes();
    ثابت v_uint8 one = vx_setall_u8(1);
    v_uint32 acc = vx_setzero_u32();
    لكل (; i <= len - vstep; i += vstep) {
        v_uint8 m  = v_gt(vx_load(mask + i), vx_setzero_u8());
        v_uint8 ad = v_and(v_absdiff(vx_load(s1 + i), vx_load(s2 + i)), m);
        acc = v_dotprod_expand_fast(ad, one, acc);
    }
    صحيح result = (صحيح)v_reduce_sum(acc);
    لكل (; i < len; i++) إذا (mask[i]) result += (صحيح)cv_absdiff(s1[i], s2[i]);
    أرجع result;
}
template<typename T>
ساكن مضمن صحيح maskedNormDiffL2_8(ثابت T* s1, ثابت T* s2, ثابت uchar* mask, صحيح len) {
    صحيح i = 0; ثابت صحيح vstep = VTraits<v_uint8>::vlanes();
    v_uint32 acc = vx_setzero_u32();
    لكل (; i <= len - vstep; i += vstep) {
        v_uint8 m  = v_gt(vx_load(mask + i), vx_setzero_u8());
        v_uint8 ad = v_and(v_absdiff(vx_load(s1 + i), vx_load(s2 + i)), m);
        acc = v_dotprod_expand_fast(ad, ad, acc);
    }
    صحيح result = (صحيح)v_reduce_sum(acc);
    لكل (; i < len; i++) إذا (mask[i]) { صحيح v = (صحيح)s1[i] - (صحيح)s2[i]; result += v*v; }
    أرجع result;
}

// Shared 16-bit cn==1 masked L1 kernel (ushort/short): v_absdiff yields v_uint16 for both.
template<typename T>
ساكن مضمن صحيح maskedNormDiffL1_16(ثابت T* s1, ثابت T* s2, ثابت uchar* mask, صحيح len) {
    صحيح i = 0; ثابت صحيح vstep = VTraits<v_uint16>::vlanes();
    v_uint32 acc = vx_setzero_u32();
    لكل (; i <= len - vstep; i += vstep) {
        v_uint16 m  = v_gt(vx_load_expand(mask + i), vx_setzero_u16());
        v_uint16 ad = v_and(v_absdiff(vx_load(s1 + i), vx_load(s2 + i)), m);
        v_uint32 lo, hi; v_expand(ad, lo, hi);
        acc = v_add(acc, v_add(lo, hi));
    }
    صحيح result = (صحيح)v_reduce_sum(acc);
    لكل (; i < len; i++) إذا (mask[i]) result += (صحيح)cv_absdiff(s1[i], s2[i]);
    أرجع result;
}

template<> بنية MaskedNormDiffInf_SIMD<uchar, صحيح> {
    صحيح operator()(ثابت uchar* s1, ثابت uchar* s2, ثابت uchar* mask, صحيح len, صحيح cn) ثابت {
        إذا (cn == 1) أرجع maskedNormDiffInfCn1<uchar, صحيح>(s1, s2, mask, len);
        إذا (cn == 4 && len >= VTraits<v_uint8>::vlanes()) {
            ثابت صحيح vstep = VTraits<v_uint8>::vlanes();
            v_uint8 acc = vx_setzero_u8();
            صحيح i = 0;
            لكل (;;) {
                إذا (i > len - vstep) i = len - vstep; // back-step (max is idempotent)
                v_uint8 a0,a1,a2,a3,b0,b1,b2,b3;
                v_load_deinterleave(s1 + i*4, a0,a1,a2,a3);
                v_load_deinterleave(s2 + i*4, b0,b1,b2,b3);
                v_uint8 ad = v_max(v_max(v_absdiff(a0,b0), v_absdiff(a1,b1)),
                                   v_max(v_absdiff(a2,b2), v_absdiff(a3,b3)));
                v_uint8 m = v_gt(vx_load(mask + i), vx_setzero_u8());
                acc = v_max(acc, v_and(ad, m));
                إذا (i >= len - vstep) اكسر;
                i += vstep;
            }
            أرجع (صحيح)v_reduce_max(acc);
        }
        أرجع maskedNormDiffInfTail<uchar, صحيح>(s1, s2, mask, len, cn, 0);
    }
};
template<> بنية MaskedNormDiffInf_SIMD<schar, صحيح> {
    صحيح operator()(ثابت schar* s1, ثابت schar* s2, ثابت uchar* mask, صحيح len, صحيح cn) ثابت {
        إذا (cn == 1) أرجع maskedNormDiffInfCn1<schar, صحيح>(s1, s2, mask, len);
        أرجع maskedNormDiffInfTail<schar, صحيح>(s1, s2, mask, len, cn, 0);
    }
};
template<> بنية MaskedNormDiffInf_SIMD<ushort, صحيح> {
    صحيح operator()(ثابت ushort* s1, ثابت ushort* s2, ثابت uchar* mask, صحيح len, صحيح cn) ثابت {
        إذا (cn == 1) أرجع maskedNormDiffInfCn1<ushort, صحيح>(s1, s2, mask, len);
        أرجع maskedNormDiffInfTail<ushort, صحيح>(s1, s2, mask, len, cn, 0);
    }
};
template<> بنية MaskedNormDiffInf_SIMD<short, صحيح> {
    صحيح operator()(ثابت short* s1, ثابت short* s2, ثابت uchar* mask, صحيح len, صحيح cn) ثابت {
        إذا (cn == 1) أرجع maskedNormDiffInfCn1<short, صحيح>(s1, s2, mask, len);
        أرجع maskedNormDiffInfTail<short, صحيح>(s1, s2, mask, len, cn, 0);
    }
};
template<> بنية MaskedNormDiffInf_SIMD<صحيح, unsigned> {
    unsigned operator()(ثابت صحيح* s1, ثابت صحيح* s2, ثابت uchar* mask, صحيح len, صحيح cn) ثابت {
        إذا (cn == 1) أرجع maskedNormDiffInfCn1<صحيح, unsigned>(s1, s2, mask, len);
        أرجع maskedNormDiffInfTail<صحيح, unsigned>(s1, s2, mask, len, cn, 0u);
    }
};
template<> بنية MaskedNormDiffInf_SIMD<عائم, عائم> {
    عائم operator()(ثابت عائم* s1, ثابت عائم* s2, ثابت uchar* mask, صحيح len, صحيح cn) ثابت {
        إذا (cn == 1) أرجع maskedNormDiffInfCn1<عائم, عائم>(s1, s2, mask, len);
        أرجع maskedNormDiffInfTail<عائم, عائم>(s1, s2, mask, len, cn, 0.0f);
    }
};

template<> بنية MaskedNormDiffL1_SIMD<uchar, صحيح> {
    صحيح operator()(ثابت uchar* s1, ثابت uchar* s2, ثابت uchar* mask, صحيح len, صحيح cn) ثابت {
        إذا (cn == 1) أرجع maskedNormDiffL1_8(s1, s2, mask, len);
        إذا (cn == 4) {
            صحيح i = 0; ثابت صحيح vstep = VTraits<v_uint8>::vlanes();
            ثابت v_uint8 one = vx_setall_u8(1);
            v_uint32 acc = vx_setzero_u32();
            لكل (; i <= len - vstep; i += vstep) {
                v_uint8 a0,a1,a2,a3,b0,b1,b2,b3;
                v_load_deinterleave(s1 + i*4, a0,a1,a2,a3);
                v_load_deinterleave(s2 + i*4, b0,b1,b2,b3);
                v_uint8 m = v_gt(vx_load(mask + i), vx_setzero_u8());
                acc = v_dotprod_expand_fast(v_and(v_absdiff(a0,b0), m), one, acc);
                acc = v_dotprod_expand_fast(v_and(v_absdiff(a1,b1), m), one, acc);
                acc = v_dotprod_expand_fast(v_and(v_absdiff(a2,b2), m), one, acc);
                acc = v_dotprod_expand_fast(v_and(v_absdiff(a3,b3), m), one, acc);
            }
            صحيح result = (صحيح)v_reduce_sum(acc);
            لكل (; i < len; i++) إذا (mask[i]) {
                ثابت uchar* e1 = s1 + i*4; ثابت uchar* e2 = s2 + i*4;
                لكل (صحيح k = 0; k < 4; k++) result += (صحيح)cv_absdiff(e1[k], e2[k]);
            }
            أرجع result;
        }
        أرجع maskedNormDiffL1Tail<uchar, صحيح>(s1, s2, mask, len, cn, 0);
    }
};
template<> بنية MaskedNormDiffL1_SIMD<schar, صحيح> {
    صحيح operator()(ثابت schar* s1, ثابت schar* s2, ثابت uchar* mask, صحيح len, صحيح cn) ثابت {
        إذا (cn == 1) أرجع maskedNormDiffL1_8(s1, s2, mask, len);
        أرجع maskedNormDiffL1Tail<schar, صحيح>(s1, s2, mask, len, cn, 0);
    }
};
template<> بنية MaskedNormDiffL1_SIMD<ushort, صحيح> {
    صحيح operator()(ثابت ushort* s1, ثابت ushort* s2, ثابت uchar* mask, صحيح len, صحيح cn) ثابت {
        إذا (cn == 1) أرجع maskedNormDiffL1_16(s1, s2, mask, len);
        أرجع maskedNormDiffL1Tail<ushort, صحيح>(s1, s2, mask, len, cn, 0);
    }
};
template<> بنية MaskedNormDiffL1_SIMD<short, صحيح> {
    صحيح operator()(ثابت short* s1, ثابت short* s2, ثابت uchar* mask, صحيح len, صحيح cn) ثابت {
        إذا (cn == 1) أرجع maskedNormDiffL1_16(s1, s2, mask, len);
        أرجع maskedNormDiffL1Tail<short, صحيح>(s1, s2, mask, len, cn, 0);
    }
};

template<> بنية MaskedNormDiffL2_SIMD<uchar, صحيح> {
    صحيح operator()(ثابت uchar* s1, ثابت uchar* s2, ثابت uchar* mask, صحيح len, صحيح cn) ثابت {
        إذا (cn == 1) أرجع maskedNormDiffL2_8(s1, s2, mask, len);
        إذا (cn == 4) {
            صحيح i = 0; ثابت صحيح vstep = VTraits<v_uint8>::vlanes();
            v_uint32 acc = vx_setzero_u32();
            لكل (; i <= len - vstep; i += vstep) {
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
            صحيح result = (صحيح)v_reduce_sum(acc);
            لكل (; i < len; i++) إذا (mask[i]) {
                ثابت uchar* e1 = s1 + i*4; ثابت uchar* e2 = s2 + i*4;
                لكل (صحيح k = 0; k < 4; k++) { صحيح v = (صحيح)e1[k] - (صحيح)e2[k]; result += v*v; }
            }
            أرجع result;
        }
        أرجع maskedNormDiffL2Tail<uchar, صحيح>(s1, s2, mask, len, cn, 0);
    }
};
template<> بنية MaskedNormDiffL2_SIMD<schar, صحيح> {
    صحيح operator()(ثابت schar* s1, ثابت schar* s2, ثابت uchar* mask, صحيح len, صحيح cn) ثابت {
        إذا (cn == 1) أرجع maskedNormDiffL2_8(s1, s2, mask, len);
        أرجع maskedNormDiffL2Tail<schar, صحيح>(s1, s2, mask, len, cn, 0);
    }
};

#إذا CV_SIMD_64F
// Depths whose L1/L2 accumulate in double.
template<> بنية MaskedNormDiffL1_SIMD<صحيح, مزدوج> {
    مزدوج operator()(ثابت صحيح* s1, ثابت صحيح* s2, ثابت uchar* mask, صحيح len, صحيح cn) ثابت {
        إذا (cn != 1) أرجع maskedNormDiffL1Tail<صحيح, مزدوج>(s1, s2, mask, len, cn, 0.0);
        صحيح i = 0; ثابت صحيح vstep = VTraits<v_int32>::vlanes();
        v_float64 acc = vx_setzero_f64();
        لكل (; i <= len - vstep; i += vstep) {
            v_uint32 m  = v_gt(vx_load_expand_q(mask + i), vx_setzero_u32());
            v_uint32 ad = v_and(v_absdiff(vx_load(s1 + i), vx_load(s2 + i)), m);
            v_uint64 e0, e1; v_expand(ad, e0, e1);
            acc = v_add(acc, v_cvt_f64(v_reinterpret_as_s64(e0)));
            acc = v_add(acc, v_cvt_f64(v_reinterpret_as_s64(e1)));
        }
        مزدوج result = v_reduce_sum(acc);
        لكل (; i < len; i++) إذا (mask[i]) result += (مزدوج)cv_absdiff(s1[i], s2[i]);
        أرجع result;
    }
};
template<> بنية MaskedNormDiffL1_SIMD<عائم, مزدوج> {
    مزدوج operator()(ثابت عائم* s1, ثابت عائم* s2, ثابت uchar* mask, صحيح len, صحيح cn) ثابت {
        إذا (cn != 1) أرجع maskedNormDiffL1Tail<عائم, مزدوج>(s1, s2, mask, len, cn, 0.0);
        صحيح i = 0; ثابت صحيح vstep = VTraits<v_float32>::vlanes();
        v_float64 acc0 = vx_setzero_f64(), acc1 = vx_setzero_f64();
        لكل (; i <= len - vstep; i += vstep) {
            v_float32 m  = v_reinterpret_as_f32(v_gt(vx_load_expand_q(mask + i), vx_setzero_u32()));
            v_float32 ad = v_and(v_absdiff(vx_load(s1 + i), vx_load(s2 + i)), m);
            acc0 = v_add(acc0, v_cvt_f64(ad));
            acc1 = v_add(acc1, v_cvt_f64_بلند(ad));
        }
        مزدوج result = v_reduce_sum(v_add(acc0, acc1));
        لكل (; i < len; i++) إذا (mask[i]) result += (مزدوج)cv_absdiff(s1[i], s2[i]);
        أرجع result;
    }
};

template<typename T>
ساكن مضمن مزدوج maskedNormDiffL2_16(ثابت T* s1, ثابت T* s2, ثابت uchar* mask, صحيح len, صحيح cn) {
    إذا (cn != 1) أرجع maskedNormDiffL2Tail<T, مزدوج>(s1, s2, mask, len, cn, 0.0);
    صحيح i = 0; ثابت صحيح vstep = VTraits<v_uint16>::vlanes();
    v_float64 acc = vx_setzero_f64();
    لكل (; i <= len - vstep; i += vstep) {
        v_uint16 m  = v_gt(vx_load_expand(mask + i), vx_setzero_u16());
        v_uint16 ad = v_and(v_absdiff(vx_load(s1 + i), vx_load(s2 + i)), m);
        v_uint64 u = v_dotprod_expand_fast(ad, ad);
        acc = v_add(acc, v_cvt_f64(v_reinterpret_as_s64(u)));
    }
    مزدوج result = v_reduce_sum(acc);
    لكل (; i < len; i++) إذا (mask[i]) { مزدوج v = (مزدوج)s1[i] - (مزدوج)s2[i]; result += v*v; }
    أرجع result;
}
template<> بنية MaskedNormDiffL2_SIMD<ushort, مزدوج> {
    مزدوج operator()(ثابت ushort* s1, ثابت ushort* s2, ثابت uchar* mask, صحيح len, صحيح cn) ثابت
    { أرجع maskedNormDiffL2_16(s1, s2, mask, len, cn); } };
template<> بنية MaskedNormDiffL2_SIMD<short, مزدوج> {
    مزدوج operator()(ثابت short* s1, ثابت short* s2, ثابت uchar* mask, صحيح len, صحيح cn) ثابت
    { أرجع maskedNormDiffL2_16(s1, s2, mask, len, cn); } };

template<> بنية MaskedNormDiffL2_SIMD<صحيح, مزدوج> {
    مزدوج operator()(ثابت صحيح* s1, ثابت صحيح* s2, ثابت uchar* mask, صحيح len, صحيح cn) ثابت {
        إذا (cn != 1) أرجع maskedNormDiffL2Tail<صحيح, مزدوج>(s1, s2, mask, len, cn, 0.0);
        صحيح i = 0; ثابت صحيح vstep = VTraits<v_int32>::vlanes();
        v_float64 r0 = vx_setzero_f64(), r1 = vx_setzero_f64();
        لكل (; i <= len - vstep; i += vstep) {
            v_uint32 m  = v_gt(vx_load_expand_q(mask + i), vx_setzero_u32());
            v_uint32 ad = v_and(v_absdiff(vx_load(s1 + i), vx_load(s2 + i)), m);
            v_uint64 e0, e1; v_expand(ad, e0, e1);
            v_float64 f0 = v_cvt_f64(v_reinterpret_as_s64(e0)), f1 = v_cvt_f64(v_reinterpret_as_s64(e1));
            r0 = v_fma(f0, f0, r0); r1 = v_fma(f1, f1, r1);
        }
        مزدوج result = v_reduce_sum(v_add(r0, r1));
        لكل (; i < len; i++) إذا (mask[i]) { مزدوج v = (مزدوج)s1[i] - (مزدوج)s2[i]; result += v*v; }
        أرجع result;
    }
};
template<> بنية MaskedNormDiffL2_SIMD<عائم, مزدوج> {
    مزدوج operator()(ثابت عائم* s1, ثابت عائم* s2, ثابت uchar* mask, صحيح len, صحيح cn) ثابت {
        إذا (cn != 1) أرجع maskedNormDiffL2Tail<عائم, مزدوج>(s1, s2, mask, len, cn, 0.0);
        صحيح i = 0; ثابت صحيح vstep = VTraits<v_float32>::vlanes();
        v_float64 r0 = vx_setzero_f64(), r1 = vx_setzero_f64();
        لكل (; i <= len - vstep; i += vstep) {
            v_float32 m  = v_reinterpret_as_f32(v_gt(vx_load_expand_q(mask + i), vx_setzero_u32()));
            v_float32 ad = v_and(v_absdiff(vx_load(s1 + i), vx_load(s2 + i)), m);
            v_float64 f0 = v_cvt_f64(ad), f1 = v_cvt_f64_بلند(ad);
            r0 = v_fma(f0, f0, r0); r1 = v_fma(f1, f1, r1);
        }
        مزدوج result = v_reduce_sum(v_add(r0, r1));
        لكل (; i < len; i++) إذا (mask[i]) { مزدوج v = (مزدوج)s1[i] - (مزدوج)s2[i]; result += v*v; }
        أرجع result;
    }
};
template<>
بنية MaskedNormDiffInf_SIMD<مزدوج, مزدوج> {
    مضمن مزدوج operator()(ثابت مزدوج* s1, ثابت مزدوج* s2, ثابت uchar* mask, صحيح len, صحيح cn) ثابت {
        مزدوج result = 0.0;
        صحيح i = 0;
#إذا CV_SIMD_64F || CV_SIMD_SCALABLE_64F
        إذا (cn == 1) {
            ثابت صحيح vstep = VTraits<v_float64>::vlanes();
            v_float64 acc0 = vx_setzero_f64(), acc1 = vx_setzero_f64();
            لكل (; i <= len - 2*vstep; i += 2*vstep) {
                v_uint64 m0, m1;
                v_normmask_u64(mask + i, m0, m1);
                v_float64 d0 = v_absdiff(vx_load(s1 + i), vx_load(s2 + i));
                v_float64 d1 = v_absdiff(vx_load(s1 + i + vstep), vx_load(s2 + i + vstep));
                acc0 = v_max(acc0, v_reinterpret_as_f64(v_and(v_reinterpret_as_u64(d0), m0)));
                acc1 = v_max(acc1, v_reinterpret_as_f64(v_and(v_reinterpret_as_u64(d1), m1)));
            }
            مزدوج t[VTraits<v_float64>::max_nlanes];
            vx_store(t, v_max(acc0, acc1));
            لكل (صحيح k = 0; k < vstep; k++)
                result = std::max(result, t[k]);
        }
#endif
        لكل (; i < len; i++) إذا (mask[i]) {
            ثابت مزدوج* e1 = s1 + i*cn; ثابت مزدوج* e2 = s2 + i*cn;
            لكل (صحيح k = 0; k < cn; k++) result = std::max(result, std::abs(e1[k] - e2[k]));
        }
        أرجع result;
    }
};

template<>
بنية MaskedNormDiffL1_SIMD<مزدوج, مزدوج> {
    مزدوج operator()(ثابت مزدوج* s1, ثابت مزدوج* s2, ثابت uchar* mask, صحيح len, صحيح cn) ثابت {
        مزدوج result = 0.0;
        إذا (cn == 1) {
            ثابت صحيح vstep = VTraits<v_float64>::vlanes();
            صحيح i = 0;
            v_float64 acc0 = vx_setzero_f64(), acc1 = vx_setzero_f64();
            لكل (; i <= len - 2*vstep; i += 2*vstep) {
                v_uint64 m0, m1;
                v_normmask_u64(mask + i, m0, m1);
                v_float64 d0 = v_absdiff(vx_load(s1 + i), vx_load(s2 + i));
                v_float64 d1 = v_absdiff(vx_load(s1 + i + vstep), vx_load(s2 + i + vstep));
                acc0 = v_add(acc0, v_reinterpret_as_f64(v_and(v_reinterpret_as_u64(d0), m0)));
                acc1 = v_add(acc1, v_reinterpret_as_f64(v_and(v_reinterpret_as_u64(d1), m1)));
            }
            result = v_reduce_sum(v_add(acc0, acc1));
            لكل (; i < len; i++) إذا (mask[i]) result += std::abs(s1[i] - s2[i]);
        } وإلا {
            result = maskedNormDiffL1Tail<مزدوج, مزدوج>(s1, s2, mask, len, cn, 0.0);
        }
        أرجع result;
    }
};

template<>
بنية MaskedNormDiffL2_SIMD<مزدوج, مزدوج> {
    مزدوج operator()(ثابت مزدوج* s1, ثابت مزدوج* s2, ثابت uchar* mask, صحيح len, صحيح cn) ثابت {
        مزدوج result = 0.0;
        إذا (cn == 1) {
            ثابت صحيح vstep = VTraits<v_float64>::vlanes();
            صحيح i = 0;
            v_float64 acc0 = vx_setzero_f64(), acc1 = vx_setzero_f64();
            لكل (; i <= len - 2*vstep; i += 2*vstep) {
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
            لكل (; i < len; i++) إذا (mask[i]) { مزدوج v = s1[i] - s2[i]; result += v*v; }
        } وإلا {
            result = maskedNormDiffL2Tail<مزدوج, مزدوج>(s1, s2, mask, len, cn, 0.0);
        }
        أرجع result;
    }
};
#endif // CV_SIMD_64F
#endif // CV_SIMD

template<typename T, typename ST> صحيح
normDiffInf_(ثابت T* src1, ثابت T* src2, ثابت uchar* mask, ST* _result, صحيح len, صحيح cn) {
    ST result = *_result;
    إذا( !mask ) {
        NormDiffInf_SIMD<T, ST> op;
        result = std::max(result, op(src1, src2, len*cn));
    } وإلا {
        MaskedNormDiffInf_SIMD<T, ST> op;
        result = std::max(result, op(src1, src2, mask, len, cn));
    }
    *_result = result;
    أرجع 0;
}

template<typename T, typename ST> صحيح
normDiffL1_(ثابت T* src1, ثابت T* src2, ثابت uchar* mask, ST* _result, صحيح len, صحيح cn) {
    ST result = *_result;
    إذا( !mask ) {
        NormDiffL1_SIMD<T, ST> op;
        result += op(src1, src2, len*cn);
    } وإلا {
        MaskedNormDiffL1_SIMD<T, ST> op;
        result += op(src1, src2, mask, len, cn);
    }
    *_result = result;
    أرجع 0;
}

template<typename T, typename ST> صحيح
normDiffL2_(ثابت T* src1, ثابت T* src2, ثابت uchar* mask, ST* _result, صحيح len, صحيح cn) {
    ST result = *_result;
    إذا( !mask ) {
        NormDiffL2_SIMD<T, ST> op;
        result += op(src1, src2, len*cn);
    } وإلا {
        MaskedNormDiffL2_SIMD<T, ST> op;
        result += op(src1, src2, mask, len, cn);
    }
    *_result = result;
    أرجع 0;
}

ساكن صحيح
normDiffInf_Bool(ثابت uchar* src1, ثابت uchar* src2, ثابت uchar* mask, صحيح* _result, صحيح len, صحيح cn)
{
    صحيح result = *_result;
    إذا( !mask )
    {
        لكل( صحيح i = 0; i < len*cn; i++ ) {
            result = std::max(result, (صحيح)((src1[i] != 0) != (src2[i] != 0)));
            إذا (result != 0)
                اكسر;
        }
    }
    وإلا
    {
        لكل( صحيح i = 0; i < len; i++, src1 += cn, src2 += cn )
            إذا( mask[i] )
            {
                لكل( صحيح k = 0; k < cn; k++ )
                    result = std::max(result, (صحيح)((src1[k] != 0) != (src2[k] != 0)));
                إذا (result != 0)
                    اكسر;
            }
    }
    *_result = result;
    أرجع 0;
}

ساكن صحيح
normDiffL1_Bool(ثابت uchar* src1, ثابت uchar* src2, ثابت uchar* mask, صحيح* _result, صحيح len, صحيح cn)
{
    صحيح result = *_result;
    إذا( !mask )
    {
        لكل( صحيح i = 0; i < len*cn; i++ )
            result += (صحيح)((src1[i] != 0) != (src2[i] != 0));
    }
    وإلا
    {
        لكل( صحيح i = 0; i < len; i++, src1 += cn, src2 += cn )
            إذا( mask[i] )
            {
                لكل( صحيح k = 0; k < cn; k++ )
                    result += (صحيح)((src1[k] != 0) != (src2[k] != 0));
            }
    }
    *_result = result;
    أرجع 0;
}

ساكن صحيح
normDiffL2_Bool(ثابت uchar* src1, ثابت uchar* src2, ثابت uchar* mask, صحيح* _result, صحيح len, صحيح cn)
{
    أرجع normDiffL1_Bool(src1, src2, mask, _result, len, cn);
}

#define CV_DEF_NORM_FUNC(L, suffix, type, ntype) \
    ساكن صحيح norm##L##_##suffix(ثابت type* src, ثابت uchar* mask, ntype* r, صحيح len, صحيح cn) \
{ CV_INSTRUMENT_REGION(); أرجع norm##L##_(src, mask, r, len, cn); } \
    ساكن صحيح normDiff##L##_##suffix(ثابت type* src1, ثابت type* src2, \
    ثابت uchar* mask, ntype* r, صحيح len, صحيح cn) \
{ أرجع normDiff##L##_(src1, src2, mask, r, (صحيح)len, cn); }

#define CV_DEF_NORM_ALL(suffix, type, inftype, l1type, l2type) \
    CV_DEF_NORM_FUNC(Inf, suffix, type, inftype) \
    CV_DEF_NORM_FUNC(L1, suffix, type, l1type) \
    CV_DEF_NORM_FUNC(L2, suffix, type, l2type)

CV_DEF_NORM_ALL(8u, uchar, صحيح, صحيح, صحيح)
CV_DEF_NORM_ALL(8s, schar, صحيح, صحيح, صحيح)
CV_DEF_NORM_ALL(16u, ushort, صحيح, صحيح, مزدوج)
CV_DEF_NORM_ALL(16s, short, صحيح, صحيح, مزدوج)
CV_DEF_NORM_ALL(32u, unsigned, unsigned, مزدوج, مزدوج)
CV_DEF_NORM_ALL(32s, صحيح, unsigned, مزدوج, مزدوج)
CV_DEF_NORM_ALL(32f, عائم, عائم, مزدوج, مزدوج)
CV_DEF_NORM_ALL(64f, مزدوج, مزدوج, مزدوج, مزدوج)
CV_DEF_NORM_ALL(64u, uint64, uint64, مزدوج, مزدوج)
CV_DEF_NORM_ALL(64s, int64, uint64, مزدوج, مزدوج)
CV_DEF_NORM_ALL(16f, hfloat, عائم, عائم, عائم)
CV_DEF_NORM_ALL(16bf, bfloat, عائم, عائم, عائم)

NormFunc getNormFunc(صحيح normType, صحيح depth)
{
    CV_INSTRUMENT_REGION();

    ساكن NormFunc normTab[3][CV_DEPTH_MAX] =
    {
        {
            normWrap<uchar, صحيح, normInf_8u>,
            normWrap<schar, صحيح, normInf_8s>,
            normWrap<ushort, صحيح, normInf_16u>,
            normWrap<short, صحيح, normInf_16s>,
            normWrap<صحيح, unsigned, normInf_32s>,
            normWrap<عائم, عائم, normInf_32f>,
            normWrap<مزدوج, مزدوج, normInf_64f>,
            normWrap<hfloat, عائم, normInf_16f>,
            normWrap<bfloat, عائم, normInf_16bf>,
            normWrap<uchar, صحيح, normInf_Bool>,
            normWrap<uint64, uint64, normInf_64u>,
            normWrap<int64, uint64, normInf_64s>,
            normWrap<unsigned, unsigned, normInf_32u>,
            0
        },
        {
            normWrap<uchar, صحيح, normL1_8u>,
            normWrap<schar, صحيح, normL1_8s>,
            normWrap<ushort, صحيح, normL1_16u>,
            normWrap<short, صحيح, normL1_16s>,
            normWrap<صحيح, مزدوج, normL1_32s>,
            normWrap<عائم, مزدوج, normL1_32f>,
            normWrap<مزدوج, مزدوج, normL1_64f>,
            normWrap<hfloat, عائم, normL1_16f>,
            normWrap<bfloat, عائم, normL1_16bf>,
            normWrap<uchar, صحيح, normL1_Bool>,
            normWrap<uint64, مزدوج, normL1_64u>,
            normWrap<int64, مزدوج, normL1_64s>,
            normWrap<unsigned, مزدوج, normL1_32u>,
            0
        },
        {
            normWrap<uchar, صحيح, normL2_8u>,
            normWrap<schar, صحيح, normL2_8s>,
            normWrap<ushort, مزدوج, normL2_16u>,
            normWrap<short, مزدوج, normL2_16s>,
            normWrap<صحيح, مزدوج, normL2_32s>,
            normWrap<عائم, مزدوج, normL2_32f>,
            normWrap<مزدوج, مزدوج, normL2_64f>,
            normWrap<hfloat, عائم, normL2_16f>,
            normWrap<bfloat, عائم, normL2_16bf>,
            normWrap<uchar, صحيح, normL2_Bool>,
            normWrap<uint64, مزدوج, normL2_64u>,
            normWrap<int64, مزدوج, normL2_64s>,
            normWrap<unsigned, مزدوج, normL2_32u>,
            0
        }
    };

    إذا (normType >= 3 || normType < 0) أرجع nullptr;

    أرجع normTab[normType][depth];
}

NormDiffFunc getNormDiffFunc(صحيح normType, صحيح depth)
{
    ساكن NormDiffFunc normDiffTab[3][CV_DEPTH_MAX] =
    {
        {
            normDiffWrap<uchar, صحيح, normDiffInf_8u>,
            normDiffWrap<schar, صحيح, normDiffInf_8s>,
            normDiffWrap<ushort, صحيح, normDiffInf_16u>,
            normDiffWrap<short, صحيح, normDiffInf_16s>,
            normDiffWrap<صحيح, unsigned, normDiffInf_32s>,
            normDiffWrap<عائم, عائم, normDiffInf_32f>,
            normDiffWrap<مزدوج, مزدوج, normDiffInf_64f>,
            normDiffWrap<hfloat, عائم, normDiffInf_16f>,
            normDiffWrap<bfloat, عائم, normDiffInf_16bf>,
            normDiffWrap<uchar, صحيح, normDiffInf_Bool>,
            normDiffWrap<uint64, uint64, normDiffInf_64u>,
            normDiffWrap<int64, uint64, normDiffInf_64s>,
            normDiffWrap<unsigned, unsigned, normDiffInf_32u>,
            0
        },
        {
            normDiffWrap<uchar, صحيح, normDiffL1_8u>,
            normDiffWrap<schar, صحيح, normDiffL1_8s>,
            normDiffWrap<ushort, صحيح, normDiffL1_16u>,
            normDiffWrap<short, صحيح, normDiffL1_16s>,
            normDiffWrap<صحيح, مزدوج, normDiffL1_32s>,
            normDiffWrap<عائم, مزدوج, normDiffL1_32f>,
            normDiffWrap<مزدوج, مزدوج, normDiffL1_64f>,
            normDiffWrap<hfloat, عائم, normDiffL1_16f>,
            normDiffWrap<bfloat, عائم, normDiffL1_16bf>,
            normDiffWrap<uchar, صحيح, normDiffL1_Bool>,
            normDiffWrap<uint64, مزدوج, normDiffL1_64u>,
            normDiffWrap<int64, مزدوج, normDiffL1_64s>,
            normDiffWrap<unsigned, مزدوج, normDiffL1_32u>,
            0
        },
        {
            normDiffWrap<uchar, صحيح, normDiffL2_8u>,
            normDiffWrap<schar, صحيح, normDiffL2_8s>,
            normDiffWrap<ushort, مزدوج, normDiffL2_16u>,
            normDiffWrap<short, مزدوج, normDiffL2_16s>,
            normDiffWrap<صحيح, مزدوج, normDiffL2_32s>,
            normDiffWrap<عائم, مزدوج, normDiffL2_32f>,
            normDiffWrap<مزدوج, مزدوج, normDiffL2_64f>,
            normDiffWrap<hfloat, عائم, normDiffL2_16f>,
            normDiffWrap<bfloat, عائم, normDiffL2_16bf>,
            normDiffWrap<uchar, صحيح, normDiffL2_Bool>,
            normDiffWrap<uint64, مزدوج, normDiffL2_64u>,
            normDiffWrap<int64, مزدوج, normDiffL2_64s>,
            normDiffWrap<unsigned, مزدوج, normDiffL2_32u>,
            0
        },
    };
    إذا (normType >= 3 || normType < 0) أرجع nullptr;

    أرجع normDiffTab[normType][depth];
}

#endif // CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

CV_CPU_OPTIMIZATION_NAMESPACE_END

} // cv::
