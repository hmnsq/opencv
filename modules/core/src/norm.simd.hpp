// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html
//
// Copyright (C) 2025, SpaceMIT Inc., all rights reserved.
// Copyright (C) 2026, Advanced Micro Devices, Inc., all rights reserved.
// Third party copyrights are property of their respective owners.

#شامل "precomp.hpp"

فضاینام cv {

using NormFunc = صحیح (*)(ثابت uchar*, ثابت uchar*, uchar*, صحیح, صحیح);
using NormDiffFunc = صحیح (*)(ثابت uchar*, ثابت uchar*, ثابت uchar*, uchar*, صحیح, صحیح);

CV_CPU_OPTIMIZATION_NAMESPACE_BEGIN

template<typename T, typename RT, صحیح (*fn)(ثابت T*, ثابت uchar*, RT*, صحیح, صحیح)>
ایستا صحیح normWrap(ثابت uchar* src1, ثابت uchar* src2, uchar* res, صحیح len, صحیح cn)
{
    بازگشت fn((ثابت T*)src1, src2, (RT*)res, len, cn);
}

template<typename T, typename RT, صحیح (*fn)(ثابت T*, ثابت T*, ثابت uchar*, RT*, صحیح, صحیح)>
ایستا صحیح normDiffWrap(ثابت uchar* src1, ثابت uchar* src2, ثابت uchar* src3, uchar* res, صحیح len, صحیح cn)
{
    بازگشت fn((ثابت T*)src1, (ثابت T*)src2, src3, (RT*)res, len, cn);
}

NormFunc getNormFunc(صحیح normType, صحیح depth);
NormDiffFunc getNormDiffFunc(صحیح normType, صحیح depth);

#ifndef CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

template <typename T, typename ST>
ساختار NormInf_SIMD {
    درونخطی ST operator() (ثابت T* src, صحیح n) ثابت {
        ST s = 0;
        برای (صحیح i = 0; i < n; i++) {
            s = std::max(s, (ST)cv_abs(src[i]));
        }
        بازگشت s;
    }
};

template <typename T, typename ST>
ساختار NormL1_SIMD {
    درونخطی ST operator() (ثابت T* src, صحیح n) ثابت {
        ST s = 0;
        برای (صحیح i = 0; i < n; i++) {
            s += cv_abs(src[i]);
        }
        بازگشت s;
    }
};

template <typename T, typename ST>
ساختار NormL2_SIMD {
    درونخطی ST operator() (ثابت T* src, صحیح n) ثابت {
        ST s = 0;
        برای (صحیح i = 0; i < n; i++) {
            ST v = (ST)src[i];
            s += v * v;
        }
        بازگشت s;
    }
};

template <typename T, typename ST>
ساختار NormDiffInf_SIMD {
    درونخطی ST operator() (ثابت T* src1, ثابت T* src2, صحیح n) ثابت {
        ST s = 0;
        برای (صحیح i = 0; i < n; i++) {
            ST v = (ST)cv_absdiff(src1[i], src2[i]);
            s = std::max(s, v);
        }
        بازگشت s;
    }
};

template <typename T, typename ST>
ساختار NormDiffL1_SIMD {
    درونخطی ST operator() (ثابت T* src1, ثابت T* src2, صحیح n) ثابت {
        ST s = 0;
        برای (صحیح i = 0; i < n; i++) {
            ST v = (ST)cv_absdiff(src1[i], src2[i]);
            s += v;
        }
        بازگشت s;
    }
};

template <typename T, typename ST>
ساختار NormDiffL2_SIMD {
    درونخطی ST operator() (ثابت T* src1, ثابت T* src2, صحیح n) ثابت {
        ST s = 0;
        برای (صحیح i = 0; i < n; i++) {
            ST v = (ST)src1[i] - (ST)src2[i];
            s += v * v;
        }
        بازگشت s;
    }
};

#اگر (CV_SIMD || CV_SIMD_SCALABLE)

template<>
ساختار NormInf_SIMD<uchar, صحیح> {
    صحیح operator() (ثابت uchar* src, صحیح n) ثابت {
        صحیح j = 0;
        صحیح s = 0;
        v_uint8 r0 = vx_setzero_u8(), r1 = vx_setzero_u8();
        v_uint8 r2 = vx_setzero_u8(), r3 = vx_setzero_u8();
        برای (; j <= n - 4 * VTraits<v_uint8>::vlanes(); j += 4 * VTraits<v_uint8>::vlanes()) {
            r0 = v_max(r0, vx_load(src + j                                 ));
            r1 = v_max(r1, vx_load(src + j +     VTraits<v_uint8>::vlanes()));
            r2 = v_max(r2, vx_load(src + j + 2 * VTraits<v_uint8>::vlanes()));
            r3 = v_max(r3, vx_load(src + j + 3 * VTraits<v_uint8>::vlanes()));
        }
        r0 = v_max(r0, v_max(r1, v_max(r2, r3)));
        برای (; j < n; j++) {
            s = std::max(s, (صحیح)src[j]);
        }
        بازگشت std::max(s, (صحیح)v_reduce_max(r0));
    }
};

template<>
ساختار NormInf_SIMD<schar, صحیح> {
    صحیح operator() (ثابت schar* src, صحیح n) ثابت {
        صحیح j = 0;
        صحیح s = 0;
        v_uint8 r0 = vx_setzero_u8(), r1 = vx_setzero_u8();
        v_uint8 r2 = vx_setzero_u8(), r3 = vx_setzero_u8();
        برای (; j <= n - 4 * VTraits<v_int8>::vlanes(); j += 4 * VTraits<v_int8>::vlanes()) {
            r0 = v_max(r0, v_abs(vx_load(src + j                                )));
            r1 = v_max(r1, v_abs(vx_load(src + j +     VTraits<v_int8>::vlanes())));
            r2 = v_max(r2, v_abs(vx_load(src + j + 2 * VTraits<v_int8>::vlanes())));
            r3 = v_max(r3, v_abs(vx_load(src + j + 3 * VTraits<v_int8>::vlanes())));
        }
        r0 = v_max(r0, v_max(r1, v_max(r2, r3)));
        برای (; j < n; j++) {
            s = std::max(s, cv_abs(src[j]));
        }
        بازگشت std::max(s, saturate_cast<صحیح>(v_reduce_max(r0)));
    }
};

template<>
ساختار NormInf_SIMD<ushort, صحیح> {
    صحیح operator() (ثابت ushort* src, صحیح n) ثابت {
        صحیح j = 0;
        صحیح s = 0;
        v_uint16 d0 = vx_setzero_u16(), d1 = vx_setzero_u16();
        v_uint16 d2 = vx_setzero_u16(), d3 = vx_setzero_u16();
        برای (; j <= n - 4 * VTraits<v_uint16>::vlanes(); j += 4 * VTraits<v_uint16>::vlanes()) {
            d0 = v_max(d0, vx_load(src + j                                  ));
            d1 = v_max(d1, vx_load(src + j +     VTraits<v_uint16>::vlanes()));
            d2 = v_max(d2, vx_load(src + j + 2 * VTraits<v_uint16>::vlanes()));
            d3 = v_max(d3, vx_load(src + j + 3 * VTraits<v_uint16>::vlanes()));
        }
        d0 = v_max(d0, v_max(d1, v_max(d2, d3)));
        برای (; j < n; j++) {
            s = std::max(s, (صحیح)src[j]);
        }
        بازگشت std::max(s, (صحیح)v_reduce_max(d0));
    }
};

template<>
ساختار NormInf_SIMD<short, صحیح> {
    صحیح operator() (ثابت short* src, صحیح n) ثابت {
        صحیح j = 0;
        صحیح s = 0;
        v_uint16 d0 = vx_setzero_u16(), d1 = vx_setzero_u16();
        v_uint16 d2 = vx_setzero_u16(), d3 = vx_setzero_u16();
        برای (; j <= n - 4 * VTraits<v_int16>::vlanes(); j += 4 * VTraits<v_int16>::vlanes()) {
            d0 = v_max(d0, v_abs(vx_load(src + j                                  )));
            d1 = v_max(d1, v_abs(vx_load(src + j +     VTraits<v_int16>::vlanes())));
            d2 = v_max(d2, v_abs(vx_load(src + j + 2 * VTraits<v_int16>::vlanes())));
            d3 = v_max(d3, v_abs(vx_load(src + j + 3 * VTraits<v_int16>::vlanes())));
        }
        d0 = v_max(d0, v_max(d1, v_max(d2, d3)));
        برای (; j < n; j++) {
            s = std::max(s, saturate_cast<صحیح>(cv_abs(src[j])));
        }
        بازگشت std::max(s, saturate_cast<صحیح>(v_reduce_max(d0)));
    }
};

template<>
ساختار NormInf_SIMD<صحیح, صحیح> {
    صحیح operator() (ثابت صحیح* src, صحیح n) ثابت {
        صحیح j = 0;
        صحیح s = 0;
        v_uint32 r0 = vx_setzero_u32(), r1 = vx_setzero_u32();
        v_uint32 r2 = vx_setzero_u32(), r3 = vx_setzero_u32();
        برای (; j <= n - 4 * VTraits<v_int32>::vlanes(); j += 4 * VTraits<v_int32>::vlanes()) {
            r0 = v_max(r0, v_abs(vx_load(src + j                                 )));
            r1 = v_max(r1, v_abs(vx_load(src + j +     VTraits<v_int32>::vlanes())));
            r2 = v_max(r2, v_abs(vx_load(src + j + 2 * VTraits<v_int32>::vlanes())));
            r3 = v_max(r3, v_abs(vx_load(src + j + 3 * VTraits<v_int32>::vlanes())));
        }
        r0 = v_max(r0, v_max(r1, v_max(r2, r3)));
        s = std::max(s, saturate_cast<صحیح>(v_reduce_max(r0)));
        برای (; j < n; j++) {
            s = std::max(s, std::abs(src[j]));
        }
        بازگشت s;
    }
};

template<>
ساختار NormInf_SIMD<اعشاری, اعشاری> {
    اعشاری operator() (ثابت اعشاری* src, صحیح n) ثابت {
        صحیح j = 0;
        اعشاری s = 0.f;
        v_float32 r0 = vx_setzero_f32(), r1 = vx_setzero_f32();
        v_float32 r2 = vx_setzero_f32(), r3 = vx_setzero_f32();
        برای (; j <= n - 4 * VTraits<v_float32>::vlanes(); j += 4 * VTraits<v_float32>::vlanes()) {
            r0 = v_max(r0, v_abs(vx_load(src + j                                   )));
            r1 = v_max(r1, v_abs(vx_load(src + j +     VTraits<v_float32>::vlanes())));
            r2 = v_max(r2, v_abs(vx_load(src + j + 2 * VTraits<v_float32>::vlanes())));
            r3 = v_max(r3, v_abs(vx_load(src + j + 3 * VTraits<v_float32>::vlanes())));
        }
        r0 = v_max(r0, v_max(r1, v_max(r2, r3)));
        برای (; j < n; j++) {
            s = std::max(s, cv_abs(src[j]));
        }
        بازگشت std::max(s, v_reduce_max(r0));
    }
};

template<>
ساختار NormL1_SIMD<uchar, صحیح> {
    صحیح operator() (ثابت uchar* src, صحیح n) ثابت {
        صحیح j = 0;
        صحیح s = 0;
        v_uint32 r0 = vx_setzero_u32(), r1 = vx_setzero_u32();
        v_uint8  one = vx_setall_u8(1);
        برای (; j<= n - 2 * VTraits<v_uint8>::vlanes(); j += 2 * VTraits<v_uint8>::vlanes()) {
            v_uint8 v0 = vx_load(src + j);
            r0 = v_dotprod_expand_fast(v0, one, r0);

            v_uint8 v1 = vx_load(src + j + VTraits<v_uint8>::vlanes());
            r1 = v_dotprod_expand_fast(v1, one, r1);
        }
        s += v_reduce_sum(v_add(r0, r1));
        برای (; j < n; j++) {
            s += src[j];
        }
        بازگشت s;
    }
};

template<>
ساختار NormL1_SIMD<schar, صحیح> {
    صحیح operator() (ثابت schar* src, صحیح n) ثابت {
        صحیح j = 0;
        صحیح s = 0;
        v_uint32 r0 = vx_setzero_u32(), r1 = vx_setzero_u32();
        v_uint8  one = vx_setall_u8(1);
        برای (; j<= n - 2 * VTraits<v_int8>::vlanes(); j += 2 * VTraits<v_int8>::vlanes()) {
            v_uint8 v0 = v_abs(vx_load(src + j));
            r0 = v_dotprod_expand_fast(v0, one, r0);

            v_uint8 v1 = v_abs(vx_load(src + j + VTraits<v_int8>::vlanes()));
            r1 = v_dotprod_expand_fast(v1, one, r1);
        }
        s += v_reduce_sum(v_add(r0, r1));
        برای (; j < n; j++) {
            s += saturate_cast<صحیح>(cv_abs(src[j]));
        }
        بازگشت s;
    }
};

template<>
ساختار NormL1_SIMD<ushort, صحیح> {
    صحیح operator() (ثابت ushort* src, صحیح n) ثابت {
        صحیح j = 0;
        صحیح s = 0;
        v_uint32 r00 = vx_setzero_u32(), r01 = vx_setzero_u32();
        v_uint32 r10 = vx_setzero_u32(), r11 = vx_setzero_u32();
        برای (; j<= n - 2 * VTraits<v_uint16>::vlanes(); j += 2 * VTraits<v_uint16>::vlanes()) {
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
        s += (صحیح)v_reduce_sum(v_add(v_add(v_add(r00, r01), r10), r11));
        برای (; j < n; j++) {
            s += src[j];
        }
        بازگشت s;
    }
};

template<>
ساختار NormL1_SIMD<short, صحیح> {
    صحیح operator() (ثابت short* src, صحیح n) ثابت {
        صحیح j = 0;
        صحیح s = 0;
        v_uint32 r00 = vx_setzero_u32(), r01 = vx_setzero_u32();
        v_uint32 r10 = vx_setzero_u32(), r11 = vx_setzero_u32();
        برای (; j<= n - 2 * VTraits<v_int16>::vlanes(); j += 2 * VTraits<v_int16>::vlanes()) {
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
        s += (صحیح)v_reduce_sum(v_add(v_add(v_add(r00, r01), r10), r11));
        برای (; j < n; j++) {
            s += saturate_cast<صحیح>(cv_abs(src[j]));
        }
        بازگشت s;
    }
};

template<>
ساختار NormL2_SIMD<uchar, صحیح> {
    صحیح operator() (ثابت uchar* src, صحیح n) ثابت {
        صحیح j = 0;
        صحیح s = 0;
        v_uint32 r0 = vx_setzero_u32(), r1 = vx_setzero_u32();
        برای (; j <= n - 2 * VTraits<v_uint8>::vlanes(); j += 2 * VTraits<v_uint8>::vlanes()) {
            v_uint8 v0 = vx_load(src + j);
            r0 = v_dotprod_expand_fast(v0, v0, r0);

            v_uint8 v1 = vx_load(src + j + VTraits<v_uint8>::vlanes());
            r1 = v_dotprod_expand_fast(v1, v1, r1);
        }
        s += v_reduce_sum(v_add(r0, r1));
        برای (; j < n; j++) {
            صحیح v = saturate_cast<صحیح>(src[j]);
            s += v * v;
        }
        بازگشت s;
    }
};

template<>
ساختار NormL2_SIMD<schar, صحیح> {
    صحیح operator() (ثابت schar* src, صحیح n) ثابت {
        صحیح j = 0;
        صحیح s = 0;
        v_int32 r0 = vx_setzero_s32(), r1 = vx_setzero_s32();
        برای (; j <= n - 2 * VTraits<v_int8>::vlanes(); j += 2 * VTraits<v_int8>::vlanes()) {
            v_int8 v0 = vx_load(src + j);
            r0 = v_dotprod_expand_fast(v0, v0, r0);
            v_int8 v1 = vx_load(src + j + VTraits<v_int8>::vlanes());
            r1 = v_dotprod_expand_fast(v1, v1, r1);
        }
        s += v_reduce_sum(v_add(r0, r1));
        برای (; j < n; j++) {
            صحیح v = saturate_cast<صحیح>(src[j]);
            s += v * v;
        }
        بازگشت s;
    }
};

template<>
ساختار NormDiffInf_SIMD<uchar, صحیح> {
    صحیح operator() (ثابت uchar* src1, ثابت uchar* src2, صحیح n) ثابت {
        صحیح j = 0;
        صحیح s = 0;
        v_uint8 r0 = vx_setzero_u8(), r1 = vx_setzero_u8();
        v_uint8 r2 = vx_setzero_u8(), r3 = vx_setzero_u8();
        برای (; j <= n - 4 * VTraits<v_uint8>::vlanes(); j += 4 * VTraits<v_uint8>::vlanes()) {
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
        s = (صحیح)v_reduce_max(v_max(v_max(v_max(r0, r1), r2), r3));
        برای (; j < n; j++) {
            صحیح v = (صحیح)cv_absdiff(src1[j], src2[j]);
            s = std::max(s, v);
        }
        بازگشت s;
    }
};

template<>
ساختار NormDiffInf_SIMD<schar, صحیح> {
    صحیح operator() (ثابت schar* src1, ثابت schar* src2, صحیح n) ثابت {
        صحیح j = 0;
        صحیح s = 0;
        v_uint8 r0 = vx_setzero_u8(), r1 = vx_setzero_u8();
        v_uint8 r2 = vx_setzero_u8(), r3 = vx_setzero_u8();
        برای (; j <= n - 4 * VTraits<v_int8>::vlanes(); j += 4 * VTraits<v_int8>::vlanes()) {
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
        s = (صحیح)v_reduce_max(v_max(v_max(v_max(r0, r1), r2), r3));
        برای (; j < n; j++) {
            صحیح v = (صحیح)cv_absdiff(src1[j], src2[j]);
            s = std::max(s, v);
        }
        بازگشت s;
    }
};

template<>
ساختار NormDiffInf_SIMD<ushort, صحیح> {
    صحیح operator() (ثابت ushort* src1, ثابت ushort* src2, صحیح n) ثابت {
        صحیح j = 0;
        صحیح s = 0;
        v_uint16 r0 = vx_setzero_u16(), r1 = vx_setzero_u16();
        v_uint16 r2 = vx_setzero_u16(), r3 = vx_setzero_u16();
        برای (; j <= n - 4 * VTraits<v_uint16>::vlanes(); j += 4 * VTraits<v_uint16>::vlanes()) {
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
        s = (صحیح)v_reduce_max(v_max(v_max(v_max(r0, r1), r2), r3));
        برای (; j < n; j++) {
            صحیح v = (صحیح)cv_absdiff(src1[j], src2[j]);
            s = std::max(s, v);
        }
        بازگشت s;
    }
};

template<>
ساختار NormDiffInf_SIMD<short, صحیح> {
    صحیح operator() (ثابت short* src1, ثابت short* src2, صحیح n) ثابت {
        صحیح j = 0;
        صحیح s = 0;
        v_uint16 r0 = vx_setzero_u16(), r1 = vx_setzero_u16();
        v_uint16 r2 = vx_setzero_u16(), r3 = vx_setzero_u16();
        برای (; j <= n - 4 * VTraits<v_int16>::vlanes(); j += 4 * VTraits<v_int16>::vlanes()) {
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
        s = (صحیح)v_reduce_max(v_max(v_max(v_max(r0, r1), r2), r3));
        برای (; j < n; j++) {
            صحیح v = (صحیح)cv_absdiff(src1[j], src2[j]);
            s = std::max(s, v);
        }
        بازگشت s;
    }
};

template<>
ساختار NormDiffInf_SIMD<صحیح, unsigned> {
    unsigned operator() (ثابت صحیح* src1, ثابت صحیح* src2, صحیح n) ثابت {
        صحیح j = 0;
        unsigned s = 0;
        v_uint32 r0 = vx_setzero_u32(), r1 = vx_setzero_u32();
        v_uint32 r2 = vx_setzero_u32(), r3 = vx_setzero_u32();
        برای (; j <= n - 4 * VTraits<v_int32>::vlanes(); j += 4 * VTraits<v_int32>::vlanes()) {
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
        s = (صحیح)v_reduce_max(v_max(v_max(v_max(r0, r1), r2), r3));
        برای (; j < n; j++) {
            unsigned v = (unsigned)cv_absdiff(src1[j], src2[j]);
            s = std::max(s, v);
        }
        بازگشت s;
    }
};

template<>
ساختار NormDiffInf_SIMD<اعشاری, اعشاری> {
    اعشاری operator() (ثابت اعشاری* src1, ثابت اعشاری* src2, صحیح n) ثابت {
        صحیح j = 0;
        اعشاری s = 0;
        v_float32 r0 = vx_setzero_f32(), r1 = vx_setzero_f32();
        برای (; j <= n - 2 * VTraits<v_float32>::vlanes(); j += 2 * VTraits<v_float32>::vlanes()) {
            v_float32 v01 = vx_load(src1 + j), v02 = vx_load(src2 + j);
            r0 = v_max(r0, v_absdiff(v01, v02));

            v_float32 v11 = vx_load(src1 + j + VTraits<v_float32>::vlanes()),
                      v12 = vx_load(src2 + j + VTraits<v_float32>::vlanes());
            r1 = v_max(r1, v_absdiff(v11, v12));
        }
        s = v_reduce_max(v_max(r0, r1));
        برای (; j < n; j++) {
            اعشاری v = (اعشاری)cv_absdiff(src1[j], src2[j]);
            s = std::max(s, v);
        }
        بازگشت s;
    }
};

template<>
ساختار NormDiffL1_SIMD<uchar, صحیح> {
    صحیح operator() (ثابت uchar* src1, ثابت uchar* src2, صحیح n) ثابت {
        صحیح j = 0;
        صحیح s = 0;
        v_uint32 r0 = vx_setzero_u32(), r1 = vx_setzero_u32();
        v_uint8  one = vx_setall_u8(1);
        برای (; j<= n - 2 * VTraits<v_uint8>::vlanes(); j += 2 * VTraits<v_uint8>::vlanes()) {
            v_uint8 v01 = vx_load(src1 + j), v02 = vx_load(src2 + j);
            r0 = v_dotprod_expand_fast(v_absdiff(v01, v02), one, r0);

            v_uint8 v11 = vx_load(src1 + j + VTraits<v_uint8>::vlanes()),
                    v12 = vx_load(src2 + j + VTraits<v_uint8>::vlanes());
            r1 = v_dotprod_expand_fast(v_absdiff(v11, v12), one, r1);
        }
        s += v_reduce_sum(v_add(r0, r1));
        برای (; j < n; j++) {
            صحیح v = (صحیح)cv_absdiff(src1[j], src2[j]);
            s += v;
        }
        بازگشت s;
    }
};

template<>
ساختار NormDiffL1_SIMD<schar, صحیح> {
    صحیح operator() (ثابت schar* src1, ثابت schar* src2, صحیح n) ثابت {
        صحیح j = 0;
        صحیح s = 0;
        v_uint32 r0 = vx_setzero_u32(), r1 = vx_setzero_u32();
        v_uint8  one = vx_setall_u8(1);
        برای (; j<= n - 2 * VTraits<v_int8>::vlanes(); j += 2 * VTraits<v_int8>::vlanes()) {
            v_int8 v01 = vx_load(src1 + j), v02 = vx_load(src2 + j);
            r0 = v_dotprod_expand_fast(v_absdiff(v01, v02), one, r0);

            v_int8 v11 = vx_load(src1 + j + VTraits<v_int8>::vlanes()),
                   v12 = vx_load(src2 + j + VTraits<v_int8>::vlanes());
            r1 = v_dotprod_expand_fast(v_absdiff(v11, v12), one, r1);
        }
        s += v_reduce_sum(v_add(r0, r1));
        برای (; j < n; j++) {
            صحیح v = (صحیح)cv_absdiff(src1[j], src2[j]);
            s += v;
        }
        بازگشت s;
    }
};

template<>
ساختار NormDiffL1_SIMD<ushort, صحیح> {
    صحیح operator() (ثابت ushort* src1, ثابت ushort* src2, صحیح n) ثابت {
        صحیح j = 0;
        صحیح s = 0;
        v_uint32 r0 = vx_setzero_u32(), r1 = vx_setzero_u32();
        v_uint32 r2 = vx_setzero_u32(), r3 = vx_setzero_u32();
        برای (; j<= n - 4 * VTraits<v_uint16>::vlanes(); j += 4 * VTraits<v_uint16>::vlanes()) {
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
        s += (صحیح)v_reduce_sum(v_add(v_add(v_add(r0, r1), r2), r3));
        برای (; j < n; j++) {
            صحیح v = (صحیح)cv_absdiff(src1[j], src2[j]);
            s += v;
        }
        بازگشت s;
    }
};

template<>
ساختار NormDiffL1_SIMD<short, صحیح> {
    صحیح operator() (ثابت short* src1, ثابت short* src2, صحیح n) ثابت {
        صحیح j = 0;
        صحیح s = 0;
        v_uint32 r0 = vx_setzero_u32(), r1 = vx_setzero_u32();
        v_uint32 r2 = vx_setzero_u32(), r3 = vx_setzero_u32();
        برای (; j<= n - 4 * VTraits<v_int16>::vlanes(); j += 4 * VTraits<v_int16>::vlanes()) {
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
        s += (صحیح)v_reduce_sum(v_add(v_add(v_add(r0, r1), r2), r3));
        برای (; j < n; j++) {
            صحیح v = (صحیح)cv_absdiff(src1[j], src2[j]);
            s += v;
        }
        بازگشت s;
    }
};

template<>
ساختار NormDiffL2_SIMD<uchar, صحیح> {
    صحیح operator() (ثابت uchar* src1, ثابت uchar* src2, صحیح n) ثابت {
        صحیح j = 0;
        صحیح s = 0;
        v_uint32 r0 = vx_setzero_u32(), r1 = vx_setzero_u32();
        برای (; j <= n - 2 * VTraits<v_uint8>::vlanes(); j += 2 * VTraits<v_uint8>::vlanes()) {
            v_uint8 v01 = vx_load(src1 + j), v02 = vx_load(src2 + j);
            v_uint8 v0 = v_absdiff(v01, v02);
            r0 = v_dotprod_expand_fast(v0, v0, r0);

            v_uint8 v11 = vx_load(src1 + j + VTraits<v_uint8>::vlanes()),
                    v12 = vx_load(src2 + j + VTraits<v_uint8>::vlanes());
            v_uint8 v1 = v_absdiff(v11, v12);
            r1 = v_dotprod_expand_fast(v1, v1, r1);
        }
        s += v_reduce_sum(v_add(r0, r1));
        برای (; j < n; j++) {
            صحیح v = (صحیح)src1[j] - (صحیح)src2[j];
            s += v * v;
        }
        بازگشت s;
    }
};

template<>
ساختار NormDiffL2_SIMD<schar, صحیح> {
    صحیح operator() (ثابت schar* src1, ثابت schar* src2, صحیح n) ثابت {
        صحیح j = 0;
        صحیح s = 0;
        v_uint32 r0 = vx_setzero_u32(), r1 = vx_setzero_u32();
        برای (; j <= n - 2 * VTraits<v_int8>::vlanes(); j += 2 * VTraits<v_int8>::vlanes()) {
            v_int8 v01 = vx_load(src1 + j), v02 = vx_load(src2 + j);
            v_uint8 v0 = v_absdiff(v01, v02);
            r0 = v_dotprod_expand_fast(v0, v0, r0);

            v_int8 v11 = vx_load(src1 + j + VTraits<v_int8>::vlanes()),
                   v12 = vx_load(src2 + j + VTraits<v_int8>::vlanes());
            v_uint8 v1 = v_absdiff(v11, v12);
            r1 = v_dotprod_expand_fast(v1, v1, r1);
        }
        s += v_reduce_sum(v_add(r0, r1));
        برای (; j < n; j++) {
            صحیح v = (صحیح)src1[j] - (صحیح)src2[j];
            s += v * v;
        }
        بازگشت s;
    }
};

#endif

#اگر (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)

template<>
ساختار NormInf_SIMD<دوگانه, دوگانه> {
    دوگانه operator() (ثابت دوگانه* src, صحیح n) ثابت {
        صحیح j = 0;
        دوگانه s = 0.f;
        v_float64 r0 = vx_setzero_f64(), r1 = vx_setzero_f64();
        v_float64 r2 = vx_setzero_f64(), r3 = vx_setzero_f64();
        برای (; j <= n - 4 * VTraits<v_float64>::vlanes(); j += 4 * VTraits<v_float64>::vlanes()) {
            r0 = v_max(r0, v_abs(vx_load(src + j                                   )));
            r1 = v_max(r1, v_abs(vx_load(src + j +     VTraits<v_float64>::vlanes())));
            r2 = v_max(r2, v_abs(vx_load(src + j + 2 * VTraits<v_float64>::vlanes())));
            r3 = v_max(r3, v_abs(vx_load(src + j + 3 * VTraits<v_float64>::vlanes())));
        }
        r0 = v_max(r0, v_max(r1, v_max(r2, r3)));
        برای (; j < n; j++) {
            s = std::max(s, cv_abs(src[j]));
        }
        // [TODO]: use v_reduce_max when it supports float64
        دوگانه t[VTraits<v_float64>::max_nlanes];
        vx_store(t, r0);
        برای (صحیح i = 0; i < VTraits<v_float64>::vlanes(); i++) {
            s = std::max(s, cv_abs(t[i]));
        }
        بازگشت s;
    }
};

template<>
ساختار NormL1_SIMD<صحیح, دوگانه> {
    دوگانه operator() (ثابت صحیح* src, صحیح n) ثابت {
        صحیح j = 0;
        دوگانه s = 0.f;
        v_float64 r00 = vx_setzero_f64(), r01 = vx_setzero_f64();
        v_float64 r10 = vx_setzero_f64(), r11 = vx_setzero_f64();
        برای (; j <= n - 2 * VTraits<v_int32>::vlanes(); j += 2 * VTraits<v_int32>::vlanes()) {
            // Widen to double before taking the magnitude: exact over the whole int32 range,
            // INT_MIN included, which a float intermediate would not be above 2^24.
            v_int32 i0 = vx_load(src + j), i1 = vx_load(src + j + VTraits<v_int32>::vlanes());
            r00 = v_add(r00, v_abs(v_cvt_f64(i0))); r01 = v_add(r01, v_abs(v_cvt_f64_بالا(i0)));
            r10 = v_add(r10, v_abs(v_cvt_f64(i1))); r11 = v_add(r11, v_abs(v_cvt_f64_بالا(i1)));
        }
        s += v_reduce_sum(v_add(v_add(v_add(r00, r01), r10), r11));
        برای (; j < n; j++) {
            s += cv_abs(src[j]);
        }
        بازگشت s;
    }
};

template<>
ساختار NormL1_SIMD<اعشاری, دوگانه> {
    دوگانه operator() (ثابت اعشاری* src, صحیح n) ثابت {
        صحیح j = 0;
        دوگانه s = 0.f;
        v_float64 r00 = vx_setzero_f64(), r01 = vx_setzero_f64();
        v_float64 r10 = vx_setzero_f64(), r11 = vx_setzero_f64();
        v_float64 r20 = vx_setzero_f64(), r21 = vx_setzero_f64();
        v_float64 r30 = vx_setzero_f64(), r31 = vx_setzero_f64();
        برای (; j <= n - 4 * VTraits<v_float32>::vlanes(); j += 4 * VTraits<v_float32>::vlanes()) {
            v_float32 v0 = v_abs(vx_load(src + j)), v1 = v_abs(vx_load(src + j + VTraits<v_float32>::vlanes()));
            r00 = v_add(r00, v_cvt_f64(v0)); r01 = v_add(r01, v_cvt_f64_بالا(v0));
            r10 = v_add(r10, v_cvt_f64(v1)); r11 = v_add(r11, v_cvt_f64_بالا(v1));

            v_float32 v2 = v_abs(vx_load(src + j + 2 * VTraits<v_float32>::vlanes())), v3 = v_abs(vx_load(src + j + 3 * VTraits<v_float32>::vlanes()));
            r20 = v_add(r20, v_cvt_f64(v2)); r21 = v_add(r21, v_cvt_f64_بالا(v2));
            r30 = v_add(r30, v_cvt_f64(v3)); r31 = v_add(r31, v_cvt_f64_بالا(v3));
        }
        s += v_reduce_sum(v_add(v_add(v_add(r00, r01), r10), r11));
        s += v_reduce_sum(v_add(v_add(v_add(r20, r21), r30), r31));
        برای (; j < n; j++) {
            s += cv_abs(src[j]);
        }
        بازگشت s;
    }
};

template<>
ساختار NormL2_SIMD<ushort, دوگانه> {
    دوگانه operator() (ثابت ushort* src, صحیح n) ثابت {
        صحیح j = 0;
        دوگانه s = 0.f;
        v_float64 r0 = vx_setzero_f64(), r1 = vx_setzero_f64();
        برای (; j <= n - 2 * VTraits<v_uint16>::vlanes(); j += 2 * VTraits<v_uint16>::vlanes()) {
            v_uint16 v0 = vx_load(src + j);
            v_uint64 u0 = v_dotprod_expand_fast(v0, v0);
            r0 = v_add(r0, v_cvt_f64(v_reinterpret_as_s64(u0)));

            v_uint16 v1 = vx_load(src + j + VTraits<v_uint16>::vlanes());
            v_uint64 u1 = v_dotprod_expand_fast(v1, v1);
            r1 = v_add(r1, v_cvt_f64(v_reinterpret_as_s64(u1)));
        }
        s += v_reduce_sum(v_add(r0, r1));
        برای (; j < n; j++) {
            دوگانه v = saturate_cast<دوگانه>(src[j]);
            s += v * v;
        }
        بازگشت s;
    }
};

template<>
ساختار NormL2_SIMD<short, دوگانه> {
    دوگانه operator() (ثابت short* src, صحیح n) ثابت {
        صحیح j = 0;
        دوگانه s = 0.f;
        v_float64 r0 = vx_setzero_f64(), r1 = vx_setzero_f64();
        برای (; j <= n - 2 * VTraits<v_int16>::vlanes(); j += 2 * VTraits<v_int16>::vlanes()) {
            v_int16 v0 = vx_load(src + j);
            r0 = v_add(r0, v_cvt_f64(v_dotprod_expand_fast(v0, v0)));

            v_int16 v1 = vx_load(src + j + VTraits<v_int16>::vlanes());
            r1 = v_add(r1, v_cvt_f64(v_dotprod_expand_fast(v1, v1)));
        }
        s += v_reduce_sum(v_add(r0, r1));
        برای (; j < n; j++) {
            دوگانه v = saturate_cast<دوگانه>(src[j]);
            s += v * v;
        }
        بازگشت s;
    }
};

template<>
ساختار NormL2_SIMD<صحیح, دوگانه> {
    دوگانه operator() (ثابت صحیح* src, صحیح n) ثابت {
        صحیح j = 0;
        دوگانه s = 0.f;
        v_float64 r0 = vx_setzero_f64(), r1 = vx_setzero_f64();
        برای (; j <= n - 2 * VTraits<v_int32>::vlanes(); j += 2 * VTraits<v_int32>::vlanes()) {
            v_int32 v0 = vx_load(src + j);
            r0 = v_dotprod_expand_fast(v0, v0, r0);

            v_int32 v1 = vx_load(src + j + VTraits<v_int32>::vlanes());
            r1 = v_dotprod_expand_fast(v1, v1, r1);
        }
        s += v_reduce_sum(v_add(r0, r1));
        برای (; j < n; j++) {
            دوگانه v = src[j];
            s += v * v;
        }
        بازگشت s;
    }
};

template<>
ساختار NormL2_SIMD<اعشاری, دوگانه> {
    دوگانه operator() (ثابت اعشاری* src, صحیح n) ثابت {
        صحیح j = 0;
        دوگانه s = 0.f;
        v_float64 r00 = vx_setzero_f64(), r01 = vx_setzero_f64();
        v_float64 r10 = vx_setzero_f64(), r11 = vx_setzero_f64();
        برای (; j <= n - 2 * VTraits<v_float32>::vlanes(); j += 2 * VTraits<v_float32>::vlanes()) {
            v_float32 v0 = vx_load(src + j), v1 = vx_load(src + j + VTraits<v_float32>::vlanes());
            v_float64 v00 = v_cvt_f64(v0), v01 = v_cvt_f64_بالا(v0);
            v_float64 v10 = v_cvt_f64(v1), v11 = v_cvt_f64_بالا(v1);
            r00 = v_fma(v00, v00, r00); r01 = v_fma(v01, v01, r01);
            r10 = v_fma(v10, v10, r10); r11 = v_fma(v11, v11, r11);
        }
        s += v_reduce_sum(v_add(v_add(v_add(r00, r01), r10), r11));
        برای (; j < n; j++) {
            دوگانه v = src[j];
            s += v * v;
        }
        بازگشت s;
    }
};

#endif

#اگر CV_SIMD_64F // CV_SIMD_SCALABLE_64F has accuracy problem with the following kernels on ci

template<>
ساختار NormL1_SIMD<دوگانه, دوگانه> {
    دوگانه operator() (ثابت دوگانه* src, صحیح n) ثابت {
        صحیح j = 0;
        دوگانه s = 0.f;
        v_float64 r00 = vx_setzero_f64(), r01 = vx_setzero_f64();
        v_float64 r10 = vx_setzero_f64(), r11 = vx_setzero_f64();
        برای (; j <= n - 4 * VTraits<v_float64>::vlanes(); j += 4 * VTraits<v_float64>::vlanes()) {
            r00 = v_add(r00, v_abs(vx_load(src + j                                   )));
            r01 = v_add(r01, v_abs(vx_load(src + j +     VTraits<v_float64>::vlanes())));
            r10 = v_add(r10, v_abs(vx_load(src + j + 2 * VTraits<v_float64>::vlanes())));
            r11 = v_add(r11, v_abs(vx_load(src + j + 3 * VTraits<v_float64>::vlanes())));
        }
        s += v_reduce_sum(v_add(v_add(v_add(r00, r01), r10), r11));
        برای (; j < n; j++) {
            s += cv_abs(src[j]);
        }
        بازگشت s;
    }
};

template<>
ساختار NormL2_SIMD<دوگانه, دوگانه> {
    دوگانه operator() (ثابت دوگانه* src, صحیح n) ثابت {
        صحیح j = 0;
        دوگانه s = 0.f;
        v_float64 r00 = vx_setzero_f64(), r01 = vx_setzero_f64();
        v_float64 r10 = vx_setzero_f64(), r11 = vx_setzero_f64();
        برای (; j <= n - 4 * VTraits<v_float64>::vlanes(); j += 4 * VTraits<v_float64>::vlanes()) {
            v_float64 v00 = vx_load(src + j                                   );
            v_float64 v01 = vx_load(src + j +     VTraits<v_float64>::vlanes());
            v_float64 v10 = vx_load(src + j + 2 * VTraits<v_float64>::vlanes());
            v_float64 v11 = vx_load(src + j + 3 * VTraits<v_float64>::vlanes());
            r00 = v_fma(v00, v00, r00); r01 = v_fma(v01, v01, r01);
            r10 = v_fma(v10, v10, r10); r11 = v_fma(v11, v11, r11);
        }
        s += v_reduce_sum(v_add(v_add(v_add(r00, r01), r10), r11));
        برای (; j < n; j++) {
            دوگانه v = src[j];
            s += v * v;
        }
        بازگشت s;
    }
};

template<>
ساختار NormDiffInf_SIMD<دوگانه, دوگانه> {
    دوگانه operator() (ثابت دوگانه* src1, ثابت دوگانه* src2, صحیح n) ثابت {
        صحیح j = 0;
        دوگانه s = 0;
        v_float64 r0 = vx_setzero_f64(), r1 = vx_setzero_f64();
        برای (; j <= n - 2 * VTraits<v_float64>::vlanes(); j += 2 * VTraits<v_float64>::vlanes()) {
            v_float64 v01 = vx_load(src1 + j), v02 = vx_load(src2 + j);
            r0 = v_max(r0, v_absdiff(v01, v02));

            v_float64 v11 = vx_load(src1 + j + VTraits<v_float64>::vlanes()),
                      v12 = vx_load(src2 + j + VTraits<v_float64>::vlanes());
            r1 = v_max(r1, v_absdiff(v11, v12));
        }
        // [TODO]: use v_reduce_max when it supports float64
        دوگانه t[VTraits<v_float64>::max_nlanes];
        vx_store(t, v_max(r0, r1));
        برای (صحیح i = 0; i < VTraits<v_float64>::vlanes(); i++) {
            s = std::max(s, t[i]);
        }
        برای (; j < n; j++) {
            دوگانه v = (دوگانه)cv_absdiff(src1[j], src2[j]);
            s = std::max(s, v);
        }
        بازگشت s;
    }
};

template<>
ساختار NormDiffL1_SIMD<اعشاری, دوگانه> {
    دوگانه operator() (ثابت اعشاری* src1, ثابت اعشاری* src2, صحیح n) ثابت {
        صحیح j = 0;
        دوگانه s = 0.f;
        v_float64 r0 = vx_setzero_f64(), r1 = vx_setzero_f64();
        v_float64 r2 = vx_setzero_f64(), r3 = vx_setzero_f64();
        برای (; j <= n - 2 * VTraits<v_float32>::vlanes(); j += 2 * VTraits<v_float32>::vlanes()) {
            v_float32 v01 = vx_load(src1 + j), v02 = vx_load(src2 + j);
            v_float32 v0 = v_absdiff(v01, v02);
            r0 = v_add(r0, v_cvt_f64(v0)); r1 = v_add(r1, v_cvt_f64_بالا(v0));

            v_float32 v11 = vx_load(src1 + j + VTraits<v_float32>::vlanes()),
                      v12 = vx_load(src2 + j + VTraits<v_float32>::vlanes());
            v_float32 v1 = v_absdiff(v11, v12);
            r2 = v_add(r2, v_cvt_f64(v1)); r3 = v_add(r3, v_cvt_f64_بالا(v1));
        }
        s += v_reduce_sum(v_add(v_add(v_add(r0, r1), r2), r3));
        برای (; j < n; j++) {
            دوگانه v = (دوگانه)cv_absdiff(src1[j], src2[j]);
            s += v;
        }
        بازگشت s;
    }
};

template<>
ساختار NormDiffL1_SIMD<دوگانه, دوگانه> {
    دوگانه operator() (ثابت دوگانه* src1, ثابت دوگانه* src2, صحیح n) ثابت {
        صحیح j = 0;
        دوگانه s = 0.f;
        v_float64 r0 = vx_setzero_f64(), r1 = vx_setzero_f64();
        برای (; j <= n - 2 * VTraits<v_float64>::vlanes(); j += 2 * VTraits<v_float64>::vlanes()) {
            v_float64 v01 = vx_load(src1 + j), v02 = vx_load(src2 + j);
            r0 = v_add(r0, v_absdiff(v01, v02));

            v_float64 v11 = vx_load(src1 + j + VTraits<v_float64>::vlanes()),
                      v12 = vx_load(src2 + j + VTraits<v_float64>::vlanes());
            r1 = v_add(r1, v_absdiff(v11, v12));
        }
        s += v_reduce_sum(v_add(r0, r1));
        برای (; j < n; j++) {
            دوگانه v = (دوگانه)cv_absdiff(src1[j], src2[j]);
            s += v;
        }
        بازگشت s;
    }
};

template<>
ساختار NormDiffL2_SIMD<ushort, دوگانه> {
    دوگانه operator() (ثابت ushort* src1, ثابت ushort* src2, صحیح n) ثابت {
        صحیح j = 0;
        دوگانه s = 0.f;
        v_float64 r0 = vx_setzero_f64(), r1 = vx_setzero_f64();
        برای (; j <= n - 2 * VTraits<v_uint16>::vlanes(); j += 2 * VTraits<v_uint16>::vlanes()) {
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
        برای (; j < n; j++) {
            دوگانه v = (دوگانه)src1[j] - (دوگانه)src2[j];
            s += v * v;
        }
        بازگشت s;
    }
};

template<>
ساختار NormDiffL2_SIMD<short, دوگانه> {
    دوگانه operator() (ثابت short* src1, ثابت short* src2, صحیح n) ثابت {
        صحیح j = 0;
        دوگانه s = 0.f;
        v_float64 r0 = vx_setzero_f64(), r1 = vx_setzero_f64();
        برای (; j <= n - 2 * VTraits<v_int16>::vlanes(); j += 2 * VTraits<v_int16>::vlanes()) {
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
        برای (; j < n; j++) {
            دوگانه v = (دوگانه)src1[j] - (دوگانه)src2[j];
            s += v * v;
        }
        بازگشت s;
    }
};

template<>
ساختار NormDiffL2_SIMD<صحیح, دوگانه> {
    دوگانه operator() (ثابت صحیح* src1, ثابت صحیح* src2, صحیح n) ثابت {
        صحیح j = 0;
        دوگانه s = 0.f;
        v_float64 r0 = vx_setzero_f64(), r1 = vx_setzero_f64();
        برای (; j <= n - VTraits<v_int32>::vlanes(); j += VTraits<v_int32>::vlanes()) {
            v_int32 v01 = vx_load(src1 + j), v02 = vx_load(src2 + j);
            v_uint32 v0 = v_absdiff(v01, v02);
            v_uint64 ev0, ev1;
            v_expand(v0, ev0, ev1);
            v_float64 f0 = v_cvt_f64(v_reinterpret_as_s64(ev0)), f1 = v_cvt_f64(v_reinterpret_as_s64(ev1));
            r0 = v_fma(f0, f0, r0); r1 = v_fma(f1, f1, r1);
        }
        s += v_reduce_sum(v_add(r0, r1));
        برای (; j < n; j++) {
            دوگانه v = (دوگانه)src1[j] - (دوگانه)src2[j];
            s += v * v;
        }
        بازگشت s;
    }
};

template<>
ساختار NormDiffL2_SIMD<اعشاری, دوگانه> {
    دوگانه operator() (ثابت اعشاری* src1, ثابت اعشاری* src2, صحیح n) ثابت {
        صحیح j = 0;
        دوگانه s = 0.f;
        v_float64 r0 = vx_setzero_f64(), r1 = vx_setzero_f64();
        v_float64 r2 = vx_setzero_f64(), r3 = vx_setzero_f64();
        برای (; j <= n - 2 * VTraits<v_float32>::vlanes(); j += 2 * VTraits<v_float32>::vlanes()) {
            v_float32 v01 = vx_load(src1 + j), v02 = vx_load(src2 + j);
            v_float32 v0 = v_absdiff(v01, v02);
            v_float64 f01 = v_cvt_f64(v0), f02 = v_cvt_f64_بالا(v0);
            r0 = v_fma(f01, f01, r0); r1 = v_fma(f02, f02, r1);

            v_float32 v11 = vx_load(src1 + j + VTraits<v_float32>::vlanes()),
                      v12 = vx_load(src2 + j + VTraits<v_float32>::vlanes());
            v_float32 v1 = v_absdiff(v11, v12);
            v_float64 f11 = v_cvt_f64(v1), f12 = v_cvt_f64_بالا(v1);
            r2 = v_fma(f11, f11, r2); r3 = v_fma(f12, f12, r3);
        }
        s += v_reduce_sum(v_add(v_add(v_add(r0, r1), r2), r3));
        برای (; j < n; j++) {
            دوگانه v = (دوگانه)src1[j] - (دوگانه)src2[j];
            s += v * v;
        }
        بازگشت s;
    }
};

template<>
ساختار NormDiffL2_SIMD<دوگانه, دوگانه> {
    دوگانه operator() (ثابت دوگانه* src1, ثابت دوگانه* src2, صحیح n) ثابت {
        صحیح j = 0;
        دوگانه s = 0.f;
        v_float64 r0 = vx_setzero_f64(), r1 = vx_setzero_f64();
        v_float64 r2 = vx_setzero_f64(), r3 = vx_setzero_f64();
        برای (; j <= n - 4 * VTraits<v_float64>::vlanes(); j += 4 * VTraits<v_float64>::vlanes()) {
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
        برای (; j < n; j++) {
            دوگانه v = (دوگانه)src1[j] - (دوگانه)src2[j];
            s += v * v;
        }
        بازگشت s;
    }
};

#endif

template <typename T, typename ST>
ساختار MaskedNormInf_SIMD {
    درونخطی ST operator() (ثابت T* src, ثابت uchar* mask, صحیح len, صحیح cn) ثابت {
        ST s = 0;
        اگر (cn == 1) {
            برای (صحیح i = 0; i < len; i++) {
                اگر (mask[i]) {
                    s = std::max(s, (ST)cv_abs(src[i]));
                }
            }
        }
        وگرنه {
            برای (صحیح i = 0; i < len; i++) {
                اگر (mask[i]) {
                    ثابت T* elem = src + i * cn;
                    صحیح k = 0;
                #اگر CV_ENABLE_UNROLLED && !CV_SIMD_SCALABLE
                    برای (; k <= cn - 4; k += 4) {
                        s = std::max(s, (ST)cv_abs(elem[k]));
                        s = std::max(s, (ST)cv_abs(elem[k + 1]));
                        s = std::max(s, (ST)cv_abs(elem[k + 2]));
                        s = std::max(s, (ST)cv_abs(elem[k + 3]));
                    }
                #endif
                    برای (; k < cn; k++) {
                        s = std::max(s, (ST)cv_abs(elem[k]));
                    }
                }
            }
        }
        بازگشت s;
    }
};

template <typename T, typename ST>
ساختار MaskedNormL1_SIMD {
    درونخطی ST operator() (ثابت T* src, ثابت uchar* mask, صحیح len, صحیح cn) ثابت {
        ST s = 0;
        اگر (cn == 1) {
            برای (صحیح i = 0; i < len; i++) {
                اگر (mask[i]) {
                    s += (ST)cv_abs(src[i]);
                }
            }
        }
        وگرنه {
            برای (صحیح i = 0; i < len; i++) {
                اگر (mask[i]) {
                    ثابت T* elem = src + i * cn;
                    صحیح k = 0;
                #اگر CV_ENABLE_UNROLLED && !CV_SIMD_SCALABLE
                    برای (; k <= cn - 4; k += 4) {
                        s += (ST)cv_abs(elem[k]);
                        s += (ST)cv_abs(elem[k + 1]);
                        s += (ST)cv_abs(elem[k + 2]);
                        s += (ST)cv_abs(elem[k + 3]);
                    }
                #endif
                    برای (; k < cn; k++) {
                        s += (ST)cv_abs(elem[k]);
                    }
                }
            }
        }
        بازگشت s;
    }
};

template <typename T, typename ST>
ساختار MaskedNormL2_SIMD {
    درونخطی ST operator() (ثابت T* src, ثابت uchar* mask, صحیح len, صحیح cn) ثابت {
        ST s = 0;
        اگر (cn == 1) {
            صحیح i = 0;
        #اگر CV_ENABLE_UNROLLED && !CV_SIMD_SCALABLE
            برای (; i <= len - 4; i += 4) {
                اگر (mask[i])     { T v0 = src[i];     s += (ST)v0 * v0; }
                اگر (mask[i + 1]) { T v1 = src[i + 1]; s += (ST)v1 * v1; }
                اگر (mask[i + 2]) { T v2 = src[i + 2]; s += (ST)v2 * v2; }
                اگر (mask[i + 3]) { T v3 = src[i + 3]; s += (ST)v3 * v3; }
            }
        #endif
            برای (; i < len; i++) {
                اگر (mask[i]) {
                    T v = src[i];
                    s += (ST)v * v;
                }
            }
        }
        وگرنه {
            برای (صحیح i = 0; i < len; i++) {
                اگر (mask[i]) {
                    ثابت T* elem = src + i * cn;
                    صحیح k = 0;
                #اگر CV_ENABLE_UNROLLED && !CV_SIMD_SCALABLE
                    برای (; k <= cn - 4; k += 4) {
                        T v0 = elem[k];     s += (ST)v0 * v0;
                        T v1 = elem[k + 1]; s += (ST)v1 * v1;
                        T v2 = elem[k + 2]; s += (ST)v2 * v2;
                        T v3 = elem[k + 3]; s += (ST)v3 * v3;
                    }
                #endif
                    برای (; k < cn; k++) {
                        T v = elem[k];
                        s += (ST)v * v;
                    }
                }
            }
        }

        بازگشت s;
    }
};

#اگر (CV_SIMD || CV_SIMD_SCALABLE)
ایستا درونخطی v_uint8  v_normmask_u8 (ثابت uchar* m) { بازگشت v_gt(vx_load(m),          vx_setzero_u8());  }
ایستا درونخطی v_uint16 v_normmask_u16(ثابت uchar* m) { بازگشت v_gt(vx_load_expand(m),   vx_setzero_u16()); }
ایستا درونخطی v_uint32 v_normmask_u32(ثابت uchar* m) { بازگشت v_gt(vx_load_expand_q(m), vx_setzero_u32()); }
ایستا درونخطی تهی v_normmask_u64(ثابت uchar* m, v_uint64& m0, v_uint64& m1) {
    v_uint64 e0, e1;
    v_expand(v_normmask_u32(m), e0, e1);
    m0 = v_or(e0, v_shl<32>(e0));
    m1 = v_or(e1, v_shl<32>(e1));
}

template <>
ساختار MaskedNormInf_SIMD<اعشاری, اعشاری> {
    درونخطی اعشاری operator()(ثابت اعشاری* src, ثابت uchar* mask, صحیح len, صحیح cn) ثابت {
        اعشاری result = 0.0f;
        اگر (cn == 1) {
            صحیح i = 0;
            ثابت صحیح vstep = VTraits<v_float32>::vlanes();
            v_float32 acc = vx_setzero_f32();

            برای (; i <= len - vstep; i += vstep) {
                v_uint32 m = vx_load_expand_q(mask + i);
                v_uint32 cmp = v_gt(m, vx_setzero_u32());
                v_float32 s = vx_load(src + i);
                s = v_abs(s);
                s = v_reinterpret_as_f32(v_and(v_reinterpret_as_u32(s), cmp));
                acc = v_max(acc, s);
            }
            result = v_reduce_max(acc);

            برای (; i < len; i++) {
                اگر (mask[i])
                    result = std::max(result, std::abs(src[i]));
            }
        }
        وگرنه {
            برای (صحیح i = 0; i < len; i++) {
                اگر (mask[i]) {
                    ثابت اعشاری* elem = src + i * cn;
                    صحیح k = 0;
                    ثابت صحیح vstep = VTraits<v_float32>::vlanes();
                    v_float32 acc = vx_setzero_f32();

                    برای (; k <= cn - vstep; k += vstep) {
                        v_float32 s = vx_load(elem + k);
                        acc = v_max(acc, v_abs(s));
                    }

                    result = std::max(result, v_reduce_max(acc));

                    برای (; k < cn; k++)
                        result = std::max(result, std::abs(elem[k]));
                }
            }
        }
        بازگشت result;
    }
};

#اگر CV_SIMD_64F
template <>
ساختار MaskedNormL1_SIMD<اعشاری, دوگانه> {
    درونخطی دوگانه operator()(ثابت اعشاری* src, ثابت uchar* mask, صحیح len, صحیح cn) ثابت {
        دوگانه result = 0.0;
        اگر (cn == 1) {
            صحیح i = 0;
            ثابت صحیح vstep = VTraits<v_float32>::vlanes();
            v_float64 acc0 = vx_setzero_f64(), acc1 = vx_setzero_f64();
            v_float64 acc2 = vx_setzero_f64(), acc3 = vx_setzero_f64();

            برای (; i <= len - 2*vstep; i += 2*vstep) {
                v_float32 s0 = v_and(v_abs(vx_load(src + i)),
                                     v_reinterpret_as_f32(v_normmask_u32(mask + i)));
                v_float32 s1 = v_and(v_abs(vx_load(src + i + vstep)),
                                     v_reinterpret_as_f32(v_normmask_u32(mask + i + vstep)));
                acc0 = v_add(acc0, v_cvt_f64(s0));
                acc1 = v_add(acc1, v_cvt_f64_بالا(s0));
                acc2 = v_add(acc2, v_cvt_f64(s1));
                acc3 = v_add(acc3, v_cvt_f64_بالا(s1));
            }
            برای (; i <= len - vstep; i += vstep) {
                v_float32 s = v_and(v_abs(vx_load(src + i)),
                                    v_reinterpret_as_f32(v_normmask_u32(mask + i)));
                acc0 = v_add(acc0, v_cvt_f64(s));
                acc1 = v_add(acc1, v_cvt_f64_بالا(s));
            }
            result = v_reduce_sum(v_add(v_add(acc0, acc1), v_add(acc2, acc3)));

            برای (; i < len; i++) {
                اگر (mask[i])
                    result += std::abs(src[i]);
            }
        }
        وگرنه {
            برای (صحیح i = 0; i < len; i++) {
                اگر (mask[i]) {
                    ثابت اعشاری* elem = src + i * cn;
                    صحیح k = 0;
                    ثابت صحیح vstep = VTraits<v_float32>::vlanes();
                    v_float64 acc = vx_setzero_f64();

                    برای (; k <= cn - vstep; k += vstep) {
                        v_float32 s = v_abs(vx_load(elem + k));
                        acc = v_add(acc, v_cvt_f64(s));
                        acc = v_add(acc, v_cvt_f64_بالا(s));
                    }

                    result += v_reduce_sum(acc);

                    برای (; k < cn; k++)
                        result += std::abs(elem[k]);
                }
            }
        }
        بازگشت result;
    }
};

template <>
ساختار MaskedNormL2_SIMD<اعشاری, دوگانه> {
    درونخطی دوگانه operator()(ثابت اعشاری* src, ثابت uchar* mask, صحیح len, صحیح cn) ثابت {
        دوگانه result = 0.0;
        اگر (cn == 1) {
            صحیح i = 0;
            ثابت صحیح vstep = VTraits<v_float32>::vlanes();
            v_float32 facc = vx_setzero_f32();
            v_float64 dacc = vx_setzero_f64();
            صحیح flush = 0;

            برای (; i <= len - vstep; i += vstep, flush += vstep) {
                اگر (flush >= 64) {
                    dacc = v_add(dacc, v_cvt_f64(facc));
                    dacc = v_add(dacc, v_cvt_f64_بالا(facc));
                    facc = vx_setzero_f32();
                    flush = 0;
                }
                v_uint32 cmp = v_gt(vx_load_expand_q(mask + i), vx_setzero_u32());
                v_float32 s  = v_reinterpret_as_f32(v_and(v_reinterpret_as_u32(vx_load(src + i)), cmp));
                facc = v_add(facc, v_mul(s, s));
            }

            dacc = v_add(dacc, v_cvt_f64(facc));
            dacc = v_add(dacc, v_cvt_f64_بالا(facc));
            result = v_reduce_sum(dacc);

            برای (; i < len; i++) {
                اگر (mask[i]) {
                    دوگانه v = src[i];
                    result += v * v;
                }
            }
        }
        وگرنه {
            برای (صحیح i = 0; i < len; i++) {
                اگر (mask[i]) {
                    ثابت اعشاری* elem = src + i * cn;
                    صحیح k = 0;
                    ثابت صحیح vstep = VTraits<v_float32>::vlanes();
                    v_float32 facc = vx_setzero_f32();

                    برای (; k <= cn - vstep; k += vstep) {
                        v_float32 s = vx_load(elem + k);
                        facc = v_add(facc, v_mul(s, s));
                    }

                    v_float64 dacc = v_add(v_cvt_f64(facc), v_cvt_f64_بالا(facc));
                    result += v_reduce_sum(dacc);

                    برای (; k < cn; k++) {
                        دوگانه v = elem[k];
                        result += v * v;
                    }
                }
            }
        }
        بازگشت result;
    }
};

#endif

template <>
ساختار MaskedNormInf_SIMD<uchar, صحیح> {
    درونخطی صحیح operator()(ثابت uchar* src, ثابت uchar* mask, صحیح len, صحیح cn) ثابت {
        صحیح result = 0;
        اگر (cn == 1) {
            صحیح i = 0;
            ثابت صحیح vstep = VTraits<v_uint8>::vlanes();
            v_uint8 acc = vx_setzero_u8();

            برای (; i <= len - vstep; i += vstep) {
                v_uint8 m   = vx_load(mask + i);
                v_uint8 s   = vx_load(src + i);
                v_uint8 sel = v_and(s, v_gt(m, vx_setzero_u8()));
                acc = v_max(acc, sel);
            }

            result = (صحیح)v_reduce_max(acc);

            برای (; i < len; i++) {
                اگر (mask[i])
                    result = std::max(result, (صحیح)src[i]);
            }
        }
        وگرنه اگر (cn == 4 && len >= VTraits<v_uint8>::vlanes()) {
            ثابت صحیح vstep = VTraits<v_uint8>::vlanes();
            v_uint8 acc = vx_setzero_u8();
            صحیح i = 0;
            برای (;;) {
                اگر (i > len - vstep)
                    i = len - vstep; // back-step (max is idempotent)
                v_uint8 c0, c1, c2, c3;
                v_load_deinterleave(src + i * 4, c0, c1, c2, c3);
                v_uint8 pmax = v_max(v_max(c0, c1), v_max(c2, c3));
                v_uint8 m = v_gt(vx_load(mask + i), vx_setzero_u8());
                acc = v_max(acc, v_and(pmax, m));
                اگر (i >= len - vstep)
                    بشکن;
                i += vstep;
            }
            result = (صحیح)v_reduce_max(acc);
        }
        وگرنه اگر (cn == 4) {
            برای (صحیح i = 0; i < len; i++) {
                اگر (mask[i]) {
                    ثابت uchar* elem = src + i * 4;
                    result = std::max(result, (صحیح)std::max(std::max(elem[0], elem[1]),
                                                            std::max(elem[2], elem[3])));
                }
            }
        }
        وگرنه {
            برای (صحیح i = 0; i < len; i++) {
                اگر (mask[i]) {
                    ثابت uchar* elem = src + i * cn;
                    برای (صحیح k = 0; k < cn; k++)
                        result = std::max(result, (صحیح)elem[k]);
                }
            }
        }
        بازگشت result;
    }
};

template <>
ساختار MaskedNormL1_SIMD<uchar, صحیح> {
    درونخطی صحیح operator()(ثابت uchar* src, ثابت uchar* mask, صحیح len, صحیح cn) ثابت {
        صحیح result = 0;
        ثابت صحیح vstep = VTraits<v_uint8>::vlanes();
        ثابت v_uint8 one = vx_setall_u8(1);
        اگر (cn == 1) {
            صحیح i = 0;
            v_uint32 acc = vx_setzero_u32();
            برای (; i <= len - vstep; i += vstep) {
                v_uint8 s = v_and(vx_load(src + i), v_normmask_u8(mask + i));
                acc = v_dotprod_expand_fast(s, one, acc);
            }
            result = (صحیح)v_reduce_sum(acc);
            برای (; i < len; i++) {
                اگر (mask[i])
                    result += src[i];
            }
        }
        وگرنه اگر (cn == 4) {
            صحیح i = 0;
            v_uint32 acc = vx_setzero_u32();
            برای (; i <= len - vstep; i += vstep) {
                v_uint8 c0, c1, c2, c3;
                v_load_deinterleave(src + i * 4, c0, c1, c2, c3);
                v_uint8 m = v_normmask_u8(mask + i);
                acc = v_dotprod_expand_fast(v_and(c0, m), one, acc);
                acc = v_dotprod_expand_fast(v_and(c1, m), one, acc);
                acc = v_dotprod_expand_fast(v_and(c2, m), one, acc);
                acc = v_dotprod_expand_fast(v_and(c3, m), one, acc);
            }
            result = (صحیح)v_reduce_sum(acc);
            برای (; i < len; i++) {
                اگر (mask[i]) {
                    ثابت uchar* elem = src + i * 4;
                    result += elem[0] + elem[1] + elem[2] + elem[3];
                }
            }
        }
        وگرنه {
            برای (صحیح i = 0; i < len; i++) {
                اگر (mask[i]) {
                    ثابت uchar* elem = src + i * cn;
                    برای (صحیح k = 0; k < cn; k++)
                        result += elem[k];
                }
            }
        }
        بازگشت result;
    }
};

template <>
ساختار MaskedNormInf_SIMD<ushort, صحیح> {
    درونخطی صحیح operator()(ثابت ushort* src, ثابت uchar* mask, صحیح len, صحیح cn) ثابت {
        صحیح result = 0;
        اگر (cn == 1) {
            صحیح i = 0;
            ثابت صحیح vstep = VTraits<v_uint16>::vlanes();
            v_uint16 acc = vx_setzero_u16();

            برای (; i <= len - vstep; i += vstep) {
                v_uint16 m   = vx_load_expand(mask + i);
                v_uint16 cmp = v_gt(m, vx_setzero_u16());
                v_uint16 s   = vx_load(src + i);
                v_uint16 sel = v_and(s, cmp);
                acc = v_max(acc, sel);
            }

            result = (صحیح)v_reduce_max(acc);

            برای (; i < len; i++) {
                اگر (mask[i])
                    result = std::max(result, (صحیح)src[i]);
            }
        }
        وگرنه {
            برای (صحیح i = 0; i < len; i++) {
                اگر (mask[i]) {
                    ثابت ushort* elem = src + i * cn;
                    صحیح k = 0;
                    ثابت صحیح vstep = VTraits<v_uint16>::vlanes();
                    v_uint16 acc = vx_setzero_u16();

                    برای (; k <= cn - vstep; k += vstep) {
                        acc = v_max(acc, vx_load(elem + k));
                    }

                    result = std::max(result, (صحیح)v_reduce_max(acc));

                    برای (; k < cn; k++)
                        result = std::max(result, (صحیح)elem[k]);
                }
            }
        }
        بازگشت result;
    }
};

template <>
ساختار MaskedNormL1_SIMD<ushort, صحیح> {
    درونخطی صحیح operator()(ثابت ushort* src, ثابت uchar* mask, صحیح len, صحیح cn) ثابت {
        صحیح result = 0;
        اگر (cn == 1) {
            صحیح i = 0;
            ثابت صحیح vstep = VTraits<v_uint16>::vlanes();
            v_uint32 acc32 = vx_setzero_u32();
            v_uint64 acc64 = vx_setzero_u64();
            صحیح acc32_elems = 0;

            برای (; i <= len - vstep; i += vstep, acc32_elems += vstep) {
                اگر (acc32_elems >= 512) {
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
            result = (صحیح)v_reduce_sum(acc64);

            برای (; i < len; i++) {
                اگر (mask[i])
                    result += src[i];
            }
        }
        وگرنه {
            برای (صحیح i = 0; i < len; i++) {
                اگر (mask[i]) {
                    ثابت ushort* elem = src + i * cn;
                    صحیح k = 0;
                    ثابت صحیح vstep = VTraits<v_uint16>::vlanes();
                    v_uint32 acc = vx_setzero_u32();

                    برای (; k <= cn - vstep; k += vstep) {
                        v_uint32 lo32, hi32;
                        v_expand(vx_load(elem + k), lo32, hi32);
                        acc = v_add(acc, v_add(lo32, hi32));
                    }

                    result += (صحیح)v_reduce_sum(acc);

                    برای (; k < cn; k++)
                        result += elem[k];
                }
            }
        }
        بازگشت result;
    }
};

template <>
ساختار MaskedNormL2_SIMD<ushort, دوگانه> {
    درونخطی دوگانه operator()(ثابت ushort* src, ثابت uchar* mask, صحیح len, صحیح cn) ثابت {
        دوگانه result = 0.0;
        اگر (cn == 1) {
            صحیح i = 0;
            ثابت صحیح vstep = VTraits<v_uint16>::vlanes();
            v_uint64 acc = vx_setzero_u64();
            برای (; i <= len - vstep; i += vstep) {
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
            result = (دوگانه)v_reduce_sum(acc);
            برای (; i < len; i++) {
                اگر (mask[i]) {
                    دوگانه v = src[i];
                    result += v * v;
                }
            }
        }
        وگرنه {
            برای (صحیح i = 0; i < len; i++) {
                اگر (mask[i]) {
                    ثابت ushort* elem = src + i * cn;
                    صحیح k = 0;
                    ثابت صحیح vstep = VTraits<v_uint16>::vlanes();
                    v_uint64 acc = vx_setzero_u64();
                    برای (; k <= cn - vstep; k += vstep) {
                        v_uint32 lo32, hi32;
                        v_expand(vx_load(elem + k), lo32, hi32);
                        v_uint64 lo64a, lo64b, hi64a, hi64b;
                        v_expand(v_mul(lo32, lo32), lo64a, lo64b);
                        v_expand(v_mul(hi32, hi32), hi64a, hi64b);
                        acc = v_add(acc, v_add(v_add(lo64a, lo64b), v_add(hi64a, hi64b)));
                    }

                    result += (دوگانه)v_reduce_sum(acc);

                    برای (; k < cn; k++) {
                        دوگانه v = elem[k];
                        result += v * v;
                    }
                }
            }
        }
        بازگشت result;
    }
};

template <>
ساختار MaskedNormInf_SIMD<schar, صحیح> {
    درونخطی صحیح operator()(ثابت schar* src, ثابت uchar* mask, صحیح len, صحیح cn) ثابت {
        صحیح result = 0;
        اگر (cn == 1) {
            ثابت صحیح vstep = VTraits<v_int8>::vlanes();
            صحیح i = 0;
            v_uint8 acc = vx_setzero_u8();
            برای (; i <= len - vstep; i += vstep)
                acc = v_max(acc, v_and(v_abs(vx_load(src + i)), v_normmask_u8(mask + i)));
            result = (صحیح)v_reduce_max(acc);
            برای (; i < len; i++)
                اگر (mask[i]) result = std::max(result, cv_abs(src[i]));
        }
        وگرنه {
            برای (صحیح i = 0; i < len; i++)
                اگر (mask[i]) {
                    ثابت schar* elem = src + i * cn;
                    برای (صحیح k = 0; k < cn; k++)
                        result = std::max(result, cv_abs(elem[k]));
                }
        }
        بازگشت result;
    }
};

template <>
ساختار MaskedNormL1_SIMD<schar, صحیح> {
    درونخطی صحیح operator()(ثابت schar* src, ثابت uchar* mask, صحیح len, صحیح cn) ثابت {
        صحیح result = 0;
        اگر (cn == 1) {
            ثابت صحیح vstep = VTraits<v_int8>::vlanes();
            ثابت v_uint8 one = vx_setall_u8(1);
            صحیح i = 0;
            v_uint32 acc = vx_setzero_u32();
            برای (; i <= len - vstep; i += vstep)
                acc = v_dotprod_expand_fast(v_and(v_abs(vx_load(src + i)), v_normmask_u8(mask + i)), one, acc);
            result = (صحیح)v_reduce_sum(acc);
            برای (; i < len; i++)
                اگر (mask[i]) result += cv_abs(src[i]);
        }
        وگرنه {
            برای (صحیح i = 0; i < len; i++)
                اگر (mask[i]) {
                    ثابت schar* elem = src + i * cn;
                    برای (صحیح k = 0; k < cn; k++)
                        result += cv_abs(elem[k]);
                }
        }
        بازگشت result;
    }
};

template <>
ساختار MaskedNormL2_SIMD<schar, صحیح> {
    درونخطی صحیح operator()(ثابت schar* src, ثابت uchar* mask, صحیح len, صحیح cn) ثابت {
        صحیح result = 0;
        اگر (cn == 1) {
            ثابت صحیح vstep = VTraits<v_int8>::vlanes();
            صحیح i = 0;
            v_int32 acc = vx_setzero_s32();
            برای (; i <= len - vstep; i += vstep) {
                v_int8 s = v_and(vx_load(src + i), v_reinterpret_as_s8(v_normmask_u8(mask + i)));
                acc = v_dotprod_expand_fast(s, s, acc);
            }
            result = (صحیح)v_reduce_sum(acc);
            برای (; i < len; i++)
                اگر (mask[i]) { صحیح v = src[i]; result += v * v; }
        }
        وگرنه {
            برای (صحیح i = 0; i < len; i++)
                اگر (mask[i]) {
                    ثابت schar* elem = src + i * cn;
                    برای (صحیح k = 0; k < cn; k++) { صحیح v = elem[k]; result += v * v; }
                }
        }
        بازگشت result;
    }
};

template <>
ساختار MaskedNormL2_SIMD<uchar, صحیح> {
    درونخطی صحیح operator()(ثابت uchar* src, ثابت uchar* mask, صحیح len, صحیح cn) ثابت {
        صحیح result = 0;
        ثابت صحیح vstep = VTraits<v_uint8>::vlanes();
        اگر (cn == 1) {
            صحیح i = 0;
            v_uint32 acc = vx_setzero_u32();
            برای (; i <= len - vstep; i += vstep) {
                v_uint8 s = v_and(vx_load(src + i), v_normmask_u8(mask + i));
                acc = v_dotprod_expand_fast(s, s, acc);
            }
            result = (صحیح)v_reduce_sum(acc);
            برای (; i < len; i++)
                اگر (mask[i]) { صحیح v = src[i]; result += v * v; }
        }
        وگرنه اگر (cn == 4) {
            صحیح i = 0;
            v_uint32 acc = vx_setzero_u32();
            برای (; i <= len - vstep; i += vstep) {
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
            result = (صحیح)v_reduce_sum(acc);
            برای (; i < len; i++)
                اگر (mask[i]) {
                    ثابت uchar* elem = src + i * 4;
                    برای (صحیح k = 0; k < 4; k++) { صحیح v = elem[k]; result += v * v; }
                }
        }
        وگرنه {
            برای (صحیح i = 0; i < len; i++)
                اگر (mask[i]) {
                    ثابت uchar* elem = src + i * cn;
                    برای (صحیح k = 0; k < cn; k++) { صحیح v = elem[k]; result += v * v; }
                }
        }
        بازگشت result;
    }
};

template <>
ساختار MaskedNormInf_SIMD<short, صحیح> {
    درونخطی صحیح operator()(ثابت short* src, ثابت uchar* mask, صحیح len, صحیح cn) ثابت {
        صحیح result = 0;
        اگر (cn == 1) {
            ثابت صحیح vstep = VTraits<v_int16>::vlanes();
            صحیح i = 0;
            v_uint16 acc = vx_setzero_u16();
            برای (; i <= len - vstep; i += vstep)
                acc = v_max(acc, v_and(v_abs(vx_load(src + i)), v_normmask_u16(mask + i)));
            result = (صحیح)v_reduce_max(acc);
            برای (; i < len; i++)
                اگر (mask[i]) result = std::max(result, cv_abs(src[i]));
        }
        وگرنه {
            برای (صحیح i = 0; i < len; i++)
                اگر (mask[i]) {
                    ثابت short* elem = src + i * cn;
                    برای (صحیح k = 0; k < cn; k++)
                        result = std::max(result, cv_abs(elem[k]));
                }
        }
        بازگشت result;
    }
};

template <>
ساختار MaskedNormL1_SIMD<short, صحیح> {
    درونخطی صحیح operator()(ثابت short* src, ثابت uchar* mask, صحیح len, صحیح cn) ثابت {
        صحیح result = 0;
        اگر (cn == 1) {
            ثابت صحیح vstep = VTraits<v_int16>::vlanes();
            صحیح i = 0;
            v_uint32 acc0 = vx_setzero_u32(), acc1 = vx_setzero_u32();
            برای (; i <= len - vstep; i += vstep) {
                v_uint16 s = v_and(v_abs(vx_load(src + i)), v_normmask_u16(mask + i));
                v_uint32 lo, hi;
                v_expand(s, lo, hi);
                acc0 = v_add(acc0, lo);
                acc1 = v_add(acc1, hi);
            }
            result = (صحیح)v_reduce_sum(v_add(acc0, acc1));
            برای (; i < len; i++)
                اگر (mask[i]) result += cv_abs(src[i]);
        }
        وگرنه {
            برای (صحیح i = 0; i < len; i++)
                اگر (mask[i]) {
                    ثابت short* elem = src + i * cn;
                    برای (صحیح k = 0; k < cn; k++)
                        result += cv_abs(elem[k]);
                }
        }
        بازگشت result;
    }
};

template <>
ساختار MaskedNormL2_SIMD<short, دوگانه> {
    درونخطی دوگانه operator()(ثابت short* src, ثابت uchar* mask, صحیح len, صحیح cn) ثابت {
        دوگانه result = 0.0;
        اگر (cn == 1) {
            ثابت صحیح vstep = VTraits<v_int16>::vlanes();
            صحیح i = 0;
            v_uint64 acc = vx_setzero_u64();
            برای (; i <= len - vstep; i += vstep) {
                v_uint16 s = v_and(v_abs(vx_load(src + i)), v_normmask_u16(mask + i));
                acc = v_add(acc, v_dotprod_expand_fast(s, s));
            }
            result = (دوگانه)v_reduce_sum(acc);
            برای (; i < len; i++)
                اگر (mask[i]) { دوگانه v = src[i]; result += v * v; }
        }
        وگرنه {
            برای (صحیح i = 0; i < len; i++)
                اگر (mask[i]) {
                    ثابت short* elem = src + i * cn;
                    برای (صحیح k = 0; k < cn; k++) { دوگانه v = elem[k]; result += v * v; }
                }
        }
        بازگشت result;
    }
};

template <>
ساختار MaskedNormInf_SIMD<صحیح, unsigned> {
    درونخطی unsigned operator()(ثابت صحیح* src, ثابت uchar* mask, صحیح len, صحیح cn) ثابت {
        unsigned result = 0;
        اگر (cn == 1) {
            ثابت صحیح vstep = VTraits<v_int32>::vlanes();
            صحیح i = 0;
            v_uint32 acc = vx_setzero_u32();
            برای (; i <= len - vstep; i += vstep)
                acc = v_max(acc, v_and(v_abs(vx_load(src + i)), v_normmask_u32(mask + i)));
            result = v_reduce_max(acc);
            برای (; i < len; i++)
                اگر (mask[i]) result = std::max(result, cv_abs(src[i]));
        }
        وگرنه {
            برای (صحیح i = 0; i < len; i++)
                اگر (mask[i]) {
                    ثابت صحیح* elem = src + i * cn;
                    برای (صحیح k = 0; k < cn; k++)
                        result = std::max(result, cv_abs(elem[k]));
                }
        }
        بازگشت result;
    }
};

#اگر CV_SIMD_64F || CV_SIMD_SCALABLE_64F

template <>
ساختار MaskedNormL1_SIMD<صحیح, دوگانه> {
    درونخطی دوگانه operator()(ثابت صحیح* src, ثابت uchar* mask, صحیح len, صحیح cn) ثابت {
        دوگانه result = 0.0;
        اگر (cn == 1) {
            ثابت صحیح vstep = VTraits<v_int32>::vlanes();
            صحیح i = 0;
            v_float64 acc0 = vx_setzero_f64(), acc1 = vx_setzero_f64();
            برای (; i <= len - vstep; i += vstep) {
                // Mask the value, then widen and take the magnitude in double. v_abs() does give
                // |INT_MIN| as an unsigned 2147483648, but reinterpreting that back as int32 turns
                // it negative again, and the widening then carries the sign into the accumulator.
                v_int32 s = v_reinterpret_as_s32(v_and(v_reinterpret_as_u32(vx_load(src + i)),
                                                       v_normmask_u32(mask + i)));
                acc0 = v_add(acc0, v_abs(v_cvt_f64(s)));
                acc1 = v_add(acc1, v_abs(v_cvt_f64_بالا(s)));
            }
            result = v_reduce_sum(v_add(acc0, acc1));
            برای (; i < len; i++)
                اگر (mask[i]) result += cv_abs(src[i]);
        }
        وگرنه {
            برای (صحیح i = 0; i < len; i++)
                اگر (mask[i]) {
                    ثابت صحیح* elem = src + i * cn;
                    برای (صحیح k = 0; k < cn; k++)
                        result += cv_abs(elem[k]);
                }
        }
        بازگشت result;
    }
};

template <>
ساختار MaskedNormL2_SIMD<صحیح, دوگانه> {
    درونخطی دوگانه operator()(ثابت صحیح* src, ثابت uchar* mask, صحیح len, صحیح cn) ثابت {
        دوگانه result = 0.0;
        اگر (cn == 1) {
            ثابت صحیح vstep = VTraits<v_int32>::vlanes();
            صحیح i = 0;
            v_float64 acc0 = vx_setzero_f64(), acc1 = vx_setzero_f64();
            برای (; i <= len - vstep; i += vstep) {
                v_uint32 s = v_and(v_abs(vx_load(src + i)), v_normmask_u32(mask + i));
                v_uint64 e0, e1;
                v_expand(s, e0, e1);
                v_float64 f0 = v_cvt_f64(v_reinterpret_as_s64(e0));
                v_float64 f1 = v_cvt_f64(v_reinterpret_as_s64(e1));
                acc0 = v_fma(f0, f0, acc0);
                acc1 = v_fma(f1, f1, acc1);
            }
            result = v_reduce_sum(v_add(acc0, acc1));
            برای (; i < len; i++)
                اگر (mask[i]) { دوگانه v = src[i]; result += v * v; }
        }
        وگرنه {
            برای (صحیح i = 0; i < len; i++)
                اگر (mask[i]) {
                    ثابت صحیح* elem = src + i * cn;
                    برای (صحیح k = 0; k < cn; k++) { دوگانه v = elem[k]; result += v * v; }
                }
        }
        بازگشت result;
    }
};

template <>
ساختار MaskedNormInf_SIMD<دوگانه, دوگانه> {
    درونخطی دوگانه operator()(ثابت دوگانه* src, ثابت uchar* mask, صحیح len, صحیح cn) ثابت {
        دوگانه result = 0.0;
        اگر (cn == 1) {
            ثابت صحیح vstep = VTraits<v_float64>::vlanes();
            صحیح i = 0;
            v_float64 acc0 = vx_setzero_f64(), acc1 = vx_setzero_f64();
            برای (; i <= len - 2*vstep; i += 2*vstep) {
                v_uint64 m0, m1;
                v_normmask_u64(mask + i, m0, m1);
                acc0 = v_max(acc0, v_reinterpret_as_f64(v_and(v_reinterpret_as_u64(v_abs(vx_load(src + i))), m0)));
                acc1 = v_max(acc1, v_reinterpret_as_f64(v_and(v_reinterpret_as_u64(v_abs(vx_load(src + i + vstep))), m1)));
            }
            دوگانه t[VTraits<v_float64>::max_nlanes];
            vx_store(t, v_max(acc0, acc1));
            برای (صحیح k = 0; k < vstep; k++)
                result = std::max(result, t[k]);
            برای (; i < len; i++)
                اگر (mask[i]) result = std::max(result, cv_abs(src[i]));
        }
        وگرنه {
            برای (صحیح i = 0; i < len; i++)
                اگر (mask[i]) {
                    ثابت دوگانه* elem = src + i * cn;
                    برای (صحیح k = 0; k < cn; k++)
                        result = std::max(result, cv_abs(elem[k]));
                }
        }
        بازگشت result;
    }
};

template <>
ساختار MaskedNormL1_SIMD<دوگانه, دوگانه> {
    درونخطی دوگانه operator()(ثابت دوگانه* src, ثابت uchar* mask, صحیح len, صحیح cn) ثابت {
        دوگانه result = 0.0;
        اگر (cn == 1) {
            ثابت صحیح vstep = VTraits<v_float64>::vlanes();
            صحیح i = 0;
            v_float64 acc0 = vx_setzero_f64(), acc1 = vx_setzero_f64();
            برای (; i <= len - 2*vstep; i += 2*vstep) {
                v_uint64 m0, m1;
                v_normmask_u64(mask + i, m0, m1);
                acc0 = v_add(acc0, v_reinterpret_as_f64(v_and(v_reinterpret_as_u64(v_abs(vx_load(src + i))), m0)));
                acc1 = v_add(acc1, v_reinterpret_as_f64(v_and(v_reinterpret_as_u64(v_abs(vx_load(src + i + vstep))), m1)));
            }
            result = v_reduce_sum(v_add(acc0, acc1));
            برای (; i < len; i++)
                اگر (mask[i]) result += cv_abs(src[i]);
        }
        وگرنه {
            برای (صحیح i = 0; i < len; i++)
                اگر (mask[i]) {
                    ثابت دوگانه* elem = src + i * cn;
                    برای (صحیح k = 0; k < cn; k++)
                        result += cv_abs(elem[k]);
                }
        }
        بازگشت result;
    }
};

template <>
ساختار MaskedNormL2_SIMD<دوگانه, دوگانه> {
    درونخطی دوگانه operator()(ثابت دوگانه* src, ثابت uchar* mask, صحیح len, صحیح cn) ثابت {
        دوگانه result = 0.0;
        اگر (cn == 1) {
            ثابت صحیح vstep = VTraits<v_float64>::vlanes();
            صحیح i = 0;
            v_float64 acc0 = vx_setzero_f64(), acc1 = vx_setzero_f64();
            برای (; i <= len - 2*vstep; i += 2*vstep) {
                v_uint64 m0, m1;
                v_normmask_u64(mask + i, m0, m1);
                v_float64 s0 = v_reinterpret_as_f64(v_and(v_reinterpret_as_u64(vx_load(src + i)), m0));
                v_float64 s1 = v_reinterpret_as_f64(v_and(v_reinterpret_as_u64(vx_load(src + i + vstep)), m1));
                acc0 = v_fma(s0, s0, acc0);
                acc1 = v_fma(s1, s1, acc1);
            }
            result = v_reduce_sum(v_add(acc0, acc1));
            برای (; i < len; i++)
                اگر (mask[i]) { دوگانه v = src[i]; result += v * v; }
        }
        وگرنه {
            برای (صحیح i = 0; i < len; i++)
                اگر (mask[i]) {
                    ثابت دوگانه* elem = src + i * cn;
                    برای (صحیح k = 0; k < cn; k++) { دوگانه v = elem[k]; result += v * v; }
                }
        }
        بازگشت result;
    }
};

#endif // CV_SIMD_64F || CV_SIMD_SCALABLE_64F
#endif // CV_SIMD || CV_SIMD_SCALABLE

template<typename T, typename ST> صحیح
normInf_(ثابت T* src, ثابت uchar* mask, ST* _result, صحیح len, صحیح cn)
{
    ST result = *_result;
    اگر( !mask )
    {
        NormInf_SIMD<T, ST> op;
        result = std::max(result, op(src, len*cn));
    } وگرنه {
        MaskedNormInf_SIMD<T, ST> op;
        result = std::max(result, op(src, mask, len, cn));
    }
    *_result = result;
    بازگشت 0;
}

template<typename T, typename ST> صحیح
normL1_(ثابت T* src, ثابت uchar* mask, ST* _result, صحیح len, صحیح cn)
{
    ST result = *_result;
    اگر( !mask )
    {
        NormL1_SIMD<T, ST> op;
        result += op(src, len*cn);
    } وگرنه {
        MaskedNormL1_SIMD<T, ST> op;
        result += op(src, mask, len, cn);
    }
    *_result = result;
    بازگشت 0;
}

template<typename T, typename ST> صحیح
normL2_(ثابت T* src, ثابت uchar* mask, ST* _result, صحیح len, صحیح cn)
{
    ST result = *_result;
    اگر( !mask )
    {
        NormL2_SIMD<T, ST> op;
        result += op(src, len*cn);
    }
    وگرنه
    {
        MaskedNormL2_SIMD<T, ST> op;
        result += op(src, mask, len, cn);
    }
    *_result = result;
    بازگشت 0;
}

ایستا صحیح
normInf_Bool(ثابت uchar* src, ثابت uchar* mask, صحیح* _result, صحیح len, صحیح cn)
{
    صحیح result = *_result;
    اگر( !mask )
    {
        برای ( صحیح i = 0; i < len*cn; i++ ) {
            result = std::max(result, (صحیح)(src[i] != 0));
            اگر (result != 0)
                بشکن;
        }
    }
    وگرنه
    {
        برای( صحیح i = 0; i < len; i++, src += cn )
            اگر( mask[i] )
            {
                برای( صحیح k = 0; k < cn; k++ )
                    result = std::max(result, (صحیح)(src[k] != 0));
                اگر (result != 0)
                    بشکن;
            }
    }
    *_result = result;
    بازگشت 0;
}

ایستا صحیح
normL1_Bool(ثابت uchar* src, ثابت uchar* mask, صحیح* _result, صحیح len, صحیح cn)
{
    صحیح result = *_result;
    اگر( !mask )
    {
        برای ( صحیح i = 0; i < len*cn; i++ )
            result += (صحیح)(src[i] != 0);
    }
    وگرنه
    {
        برای( صحیح i = 0; i < len; i++, src += cn )
            اگر( mask[i] )
            {
                برای( صحیح k = 0; k < cn; k++ )
                    result += (صحیح)(src[k] != 0);
            }
    }
    *_result = result;
    بازگشت 0;
}

ایستا صحیح
normL2_Bool(ثابت uchar* src, ثابت uchar* mask, صحیح* _result, صحیح len, صحیح cn)
{
    بازگشت normL1_Bool(src, mask, _result, len, cn);
}

// ====================================================================
// Masked norm-diff kernels.
// Scalar base templates (safe for every depth) plus SIMD cn==1
// specializations for the concrete (T, ST) pairs dispatched by
// CV_DEF_NORM_ALL. The masked normDiff*_ functions below select them.
// ====================================================================
template <typename T, typename ST>
ساختار MaskedNormDiffInf_SIMD {
    درونخطی ST operator()(ثابت T* s1, ثابت T* s2, ثابت uchar* mask, صحیح len, صحیح cn) ثابت {
        ST result = 0;
        برای (صحیح i = 0; i < len; i++) اگر (mask[i]) {
            ثابت T* e1 = s1 + i*cn; ثابت T* e2 = s2 + i*cn;
            برای (صحیح k = 0; k < cn; k++)
                result = std::max(result, (ST)cv_absdiff(e1[k], e2[k]));
        }
        بازگشت result;
    }
};
template <typename T, typename ST>
ساختار MaskedNormDiffL1_SIMD {
    درونخطی ST operator()(ثابت T* s1, ثابت T* s2, ثابت uchar* mask, صحیح len, صحیح cn) ثابت {
        ST result = 0;
        برای (صحیح i = 0; i < len; i++) اگر (mask[i]) {
            ثابت T* e1 = s1 + i*cn; ثابت T* e2 = s2 + i*cn;
            برای (صحیح k = 0; k < cn; k++)
                result += (ST)cv_absdiff(e1[k], e2[k]);
        }
        بازگشت result;
    }
};
template <typename T, typename ST>
ساختار MaskedNormDiffL2_SIMD {
    درونخطی ST operator()(ثابت T* s1, ثابت T* s2, ثابت uchar* mask, صحیح len, صحیح cn) ثابت {
        ST result = 0;
        برای (صحیح i = 0; i < len; i++) اگر (mask[i]) {
            ثابت T* e1 = s1 + i*cn; ثابت T* e2 = s2 + i*cn;
            برای (صحیح k = 0; k < cn; k++) { ST v = (ST)e1[k] - (ST)e2[k]; result += v*v; }
        }
        بازگشت result;
    }
};

#اگر (CV_SIMD || CV_SIMD_SCALABLE)
// AND a per-lane diff vector with the (expanded) mask>0 predicate.
درونخطی v_uint8  v_diffmask(ثابت v_uint8&  d, ثابت uchar* m) { بازگشت v_and(d, v_gt(vx_load(m),          vx_setzero_u8())); }
درونخطی v_uint16 v_diffmask(ثابت v_uint16& d, ثابت uchar* m) { بازگشت v_and(d, v_gt(vx_load_expand(m),   vx_setzero_u16())); }
درونخطی v_uint32 v_diffmask(ثابت v_uint32& d, ثابت uchar* m) { بازگشت v_and(d, v_gt(vx_load_expand_q(m), vx_setzero_u32())); }
درونخطی v_float32 v_diffmask(ثابت v_float32& d, ثابت uchar* m) {
    v_uint32 cm = v_gt(vx_load_expand_q(m), vx_setzero_u32());
    بازگشت v_reinterpret_as_f32(v_and(v_reinterpret_as_u32(d), cm));
}

// Shared scalar cn>1 fallbacks (match the base templates' cv_absdiff semantics).
template<typename T, typename RT>
ایستا درونخطی RT maskedNormDiffInfTail(ثابت T* s1, ثابت T* s2, ثابت uchar* mask, صحیح len, صحیح cn, RT result) {
    برای (صحیح i = 0; i < len; i++) اگر (mask[i]) {
        ثابت T* e1 = s1 + i*cn; ثابت T* e2 = s2 + i*cn;
        برای (صحیح k = 0; k < cn; k++) result = std::max(result, (RT)cv_absdiff(e1[k], e2[k]));
    }
    بازگشت result;
}
template<typename T, typename RT>
ایستا درونخطی RT maskedNormDiffL1Tail(ثابت T* s1, ثابت T* s2, ثابت uchar* mask, صحیح len, صحیح cn, RT result) {
    برای (صحیح i = 0; i < len; i++) اگر (mask[i]) {
        ثابت T* e1 = s1 + i*cn; ثابت T* e2 = s2 + i*cn;
        برای (صحیح k = 0; k < cn; k++) result += (RT)cv_absdiff(e1[k], e2[k]);
    }
    بازگشت result;
}
template<typename T, typename RT>
ایستا درونخطی RT maskedNormDiffL2Tail(ثابت T* s1, ثابت T* s2, ثابت uchar* mask, صحیح len, صحیح cn, RT result) {
    برای (صحیح i = 0; i < len; i++) اگر (mask[i]) {
        ثابت T* e1 = s1 + i*cn; ثابت T* e2 = s2 + i*cn;
        برای (صحیح k = 0; k < cn; k++) { RT v = (RT)e1[k] - (RT)e2[k]; result += v*v; }
    }
    بازگشت result;
}

// Inf (cn==1): reduce_max of masked |a-b|. Shared across SIMD-capable depths.
template<typename T, typename ST>
ایستا درونخطی ST maskedNormDiffInfCn1(ثابت T* s1, ثابت T* s2, ثابت uchar* mask, صحیح len) {
    ثابت صحیح vstep = VTraits<decltype(vx_load(s1))>::vlanes();
    ST result = 0;
    صحیح i = 0;
    اگر (len >= vstep) {
        auto acc = v_diffmask(v_absdiff(vx_load(s1), vx_load(s2)), mask);
        برای (i = vstep; i <= len - vstep; i += vstep)
            acc = v_max(acc, v_diffmask(v_absdiff(vx_load(s1 + i), vx_load(s2 + i)), mask + i));
        result = (ST)v_reduce_max(acc);
    }
    برای (; i < len; i++) اگر (mask[i]) result = std::max(result, (ST)cv_absdiff(s1[i], s2[i]));
    بازگشت result;
}

// Shared 8-bit cn==1 masked L1/L2 kernels (uchar/schar): v_absdiff yields v_uint8 for both.
template<typename T>
ایستا درونخطی صحیح maskedNormDiffL1_8(ثابت T* s1, ثابت T* s2, ثابت uchar* mask, صحیح len) {
    صحیح i = 0; ثابت صحیح vstep = VTraits<v_uint8>::vlanes();
    ثابت v_uint8 one = vx_setall_u8(1);
    v_uint32 acc = vx_setzero_u32();
    برای (; i <= len - vstep; i += vstep) {
        v_uint8 m  = v_gt(vx_load(mask + i), vx_setzero_u8());
        v_uint8 ad = v_and(v_absdiff(vx_load(s1 + i), vx_load(s2 + i)), m);
        acc = v_dotprod_expand_fast(ad, one, acc);
    }
    صحیح result = (صحیح)v_reduce_sum(acc);
    برای (; i < len; i++) اگر (mask[i]) result += (صحیح)cv_absdiff(s1[i], s2[i]);
    بازگشت result;
}
template<typename T>
ایستا درونخطی صحیح maskedNormDiffL2_8(ثابت T* s1, ثابت T* s2, ثابت uchar* mask, صحیح len) {
    صحیح i = 0; ثابت صحیح vstep = VTraits<v_uint8>::vlanes();
    v_uint32 acc = vx_setzero_u32();
    برای (; i <= len - vstep; i += vstep) {
        v_uint8 m  = v_gt(vx_load(mask + i), vx_setzero_u8());
        v_uint8 ad = v_and(v_absdiff(vx_load(s1 + i), vx_load(s2 + i)), m);
        acc = v_dotprod_expand_fast(ad, ad, acc);
    }
    صحیح result = (صحیح)v_reduce_sum(acc);
    برای (; i < len; i++) اگر (mask[i]) { صحیح v = (صحیح)s1[i] - (صحیح)s2[i]; result += v*v; }
    بازگشت result;
}

// Shared 16-bit cn==1 masked L1 kernel (ushort/short): v_absdiff yields v_uint16 for both.
template<typename T>
ایستا درونخطی صحیح maskedNormDiffL1_16(ثابت T* s1, ثابت T* s2, ثابت uchar* mask, صحیح len) {
    صحیح i = 0; ثابت صحیح vstep = VTraits<v_uint16>::vlanes();
    v_uint32 acc = vx_setzero_u32();
    برای (; i <= len - vstep; i += vstep) {
        v_uint16 m  = v_gt(vx_load_expand(mask + i), vx_setzero_u16());
        v_uint16 ad = v_and(v_absdiff(vx_load(s1 + i), vx_load(s2 + i)), m);
        v_uint32 lo, hi; v_expand(ad, lo, hi);
        acc = v_add(acc, v_add(lo, hi));
    }
    صحیح result = (صحیح)v_reduce_sum(acc);
    برای (; i < len; i++) اگر (mask[i]) result += (صحیح)cv_absdiff(s1[i], s2[i]);
    بازگشت result;
}

template<> ساختار MaskedNormDiffInf_SIMD<uchar, صحیح> {
    صحیح operator()(ثابت uchar* s1, ثابت uchar* s2, ثابت uchar* mask, صحیح len, صحیح cn) ثابت {
        اگر (cn == 1) بازگشت maskedNormDiffInfCn1<uchar, صحیح>(s1, s2, mask, len);
        اگر (cn == 4 && len >= VTraits<v_uint8>::vlanes()) {
            ثابت صحیح vstep = VTraits<v_uint8>::vlanes();
            v_uint8 acc = vx_setzero_u8();
            صحیح i = 0;
            برای (;;) {
                اگر (i > len - vstep) i = len - vstep; // back-step (max is idempotent)
                v_uint8 a0,a1,a2,a3,b0,b1,b2,b3;
                v_load_deinterleave(s1 + i*4, a0,a1,a2,a3);
                v_load_deinterleave(s2 + i*4, b0,b1,b2,b3);
                v_uint8 ad = v_max(v_max(v_absdiff(a0,b0), v_absdiff(a1,b1)),
                                   v_max(v_absdiff(a2,b2), v_absdiff(a3,b3)));
                v_uint8 m = v_gt(vx_load(mask + i), vx_setzero_u8());
                acc = v_max(acc, v_and(ad, m));
                اگر (i >= len - vstep) بشکن;
                i += vstep;
            }
            بازگشت (صحیح)v_reduce_max(acc);
        }
        بازگشت maskedNormDiffInfTail<uchar, صحیح>(s1, s2, mask, len, cn, 0);
    }
};
template<> ساختار MaskedNormDiffInf_SIMD<schar, صحیح> {
    صحیح operator()(ثابت schar* s1, ثابت schar* s2, ثابت uchar* mask, صحیح len, صحیح cn) ثابت {
        اگر (cn == 1) بازگشت maskedNormDiffInfCn1<schar, صحیح>(s1, s2, mask, len);
        بازگشت maskedNormDiffInfTail<schar, صحیح>(s1, s2, mask, len, cn, 0);
    }
};
template<> ساختار MaskedNormDiffInf_SIMD<ushort, صحیح> {
    صحیح operator()(ثابت ushort* s1, ثابت ushort* s2, ثابت uchar* mask, صحیح len, صحیح cn) ثابت {
        اگر (cn == 1) بازگشت maskedNormDiffInfCn1<ushort, صحیح>(s1, s2, mask, len);
        بازگشت maskedNormDiffInfTail<ushort, صحیح>(s1, s2, mask, len, cn, 0);
    }
};
template<> ساختار MaskedNormDiffInf_SIMD<short, صحیح> {
    صحیح operator()(ثابت short* s1, ثابت short* s2, ثابت uchar* mask, صحیح len, صحیح cn) ثابت {
        اگر (cn == 1) بازگشت maskedNormDiffInfCn1<short, صحیح>(s1, s2, mask, len);
        بازگشت maskedNormDiffInfTail<short, صحیح>(s1, s2, mask, len, cn, 0);
    }
};
template<> ساختار MaskedNormDiffInf_SIMD<صحیح, unsigned> {
    unsigned operator()(ثابت صحیح* s1, ثابت صحیح* s2, ثابت uchar* mask, صحیح len, صحیح cn) ثابت {
        اگر (cn == 1) بازگشت maskedNormDiffInfCn1<صحیح, unsigned>(s1, s2, mask, len);
        بازگشت maskedNormDiffInfTail<صحیح, unsigned>(s1, s2, mask, len, cn, 0u);
    }
};
template<> ساختار MaskedNormDiffInf_SIMD<اعشاری, اعشاری> {
    اعشاری operator()(ثابت اعشاری* s1, ثابت اعشاری* s2, ثابت uchar* mask, صحیح len, صحیح cn) ثابت {
        اگر (cn == 1) بازگشت maskedNormDiffInfCn1<اعشاری, اعشاری>(s1, s2, mask, len);
        بازگشت maskedNormDiffInfTail<اعشاری, اعشاری>(s1, s2, mask, len, cn, 0.0f);
    }
};

template<> ساختار MaskedNormDiffL1_SIMD<uchar, صحیح> {
    صحیح operator()(ثابت uchar* s1, ثابت uchar* s2, ثابت uchar* mask, صحیح len, صحیح cn) ثابت {
        اگر (cn == 1) بازگشت maskedNormDiffL1_8(s1, s2, mask, len);
        اگر (cn == 4) {
            صحیح i = 0; ثابت صحیح vstep = VTraits<v_uint8>::vlanes();
            ثابت v_uint8 one = vx_setall_u8(1);
            v_uint32 acc = vx_setzero_u32();
            برای (; i <= len - vstep; i += vstep) {
                v_uint8 a0,a1,a2,a3,b0,b1,b2,b3;
                v_load_deinterleave(s1 + i*4, a0,a1,a2,a3);
                v_load_deinterleave(s2 + i*4, b0,b1,b2,b3);
                v_uint8 m = v_gt(vx_load(mask + i), vx_setzero_u8());
                acc = v_dotprod_expand_fast(v_and(v_absdiff(a0,b0), m), one, acc);
                acc = v_dotprod_expand_fast(v_and(v_absdiff(a1,b1), m), one, acc);
                acc = v_dotprod_expand_fast(v_and(v_absdiff(a2,b2), m), one, acc);
                acc = v_dotprod_expand_fast(v_and(v_absdiff(a3,b3), m), one, acc);
            }
            صحیح result = (صحیح)v_reduce_sum(acc);
            برای (; i < len; i++) اگر (mask[i]) {
                ثابت uchar* e1 = s1 + i*4; ثابت uchar* e2 = s2 + i*4;
                برای (صحیح k = 0; k < 4; k++) result += (صحیح)cv_absdiff(e1[k], e2[k]);
            }
            بازگشت result;
        }
        بازگشت maskedNormDiffL1Tail<uchar, صحیح>(s1, s2, mask, len, cn, 0);
    }
};
template<> ساختار MaskedNormDiffL1_SIMD<schar, صحیح> {
    صحیح operator()(ثابت schar* s1, ثابت schar* s2, ثابت uchar* mask, صحیح len, صحیح cn) ثابت {
        اگر (cn == 1) بازگشت maskedNormDiffL1_8(s1, s2, mask, len);
        بازگشت maskedNormDiffL1Tail<schar, صحیح>(s1, s2, mask, len, cn, 0);
    }
};
template<> ساختار MaskedNormDiffL1_SIMD<ushort, صحیح> {
    صحیح operator()(ثابت ushort* s1, ثابت ushort* s2, ثابت uchar* mask, صحیح len, صحیح cn) ثابت {
        اگر (cn == 1) بازگشت maskedNormDiffL1_16(s1, s2, mask, len);
        بازگشت maskedNormDiffL1Tail<ushort, صحیح>(s1, s2, mask, len, cn, 0);
    }
};
template<> ساختار MaskedNormDiffL1_SIMD<short, صحیح> {
    صحیح operator()(ثابت short* s1, ثابت short* s2, ثابت uchar* mask, صحیح len, صحیح cn) ثابت {
        اگر (cn == 1) بازگشت maskedNormDiffL1_16(s1, s2, mask, len);
        بازگشت maskedNormDiffL1Tail<short, صحیح>(s1, s2, mask, len, cn, 0);
    }
};

template<> ساختار MaskedNormDiffL2_SIMD<uchar, صحیح> {
    صحیح operator()(ثابت uchar* s1, ثابت uchar* s2, ثابت uchar* mask, صحیح len, صحیح cn) ثابت {
        اگر (cn == 1) بازگشت maskedNormDiffL2_8(s1, s2, mask, len);
        اگر (cn == 4) {
            صحیح i = 0; ثابت صحیح vstep = VTraits<v_uint8>::vlanes();
            v_uint32 acc = vx_setzero_u32();
            برای (; i <= len - vstep; i += vstep) {
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
            صحیح result = (صحیح)v_reduce_sum(acc);
            برای (; i < len; i++) اگر (mask[i]) {
                ثابت uchar* e1 = s1 + i*4; ثابت uchar* e2 = s2 + i*4;
                برای (صحیح k = 0; k < 4; k++) { صحیح v = (صحیح)e1[k] - (صحیح)e2[k]; result += v*v; }
            }
            بازگشت result;
        }
        بازگشت maskedNormDiffL2Tail<uchar, صحیح>(s1, s2, mask, len, cn, 0);
    }
};
template<> ساختار MaskedNormDiffL2_SIMD<schar, صحیح> {
    صحیح operator()(ثابت schar* s1, ثابت schar* s2, ثابت uchar* mask, صحیح len, صحیح cn) ثابت {
        اگر (cn == 1) بازگشت maskedNormDiffL2_8(s1, s2, mask, len);
        بازگشت maskedNormDiffL2Tail<schar, صحیح>(s1, s2, mask, len, cn, 0);
    }
};

#اگر CV_SIMD_64F
// Depths whose L1/L2 accumulate in double.
template<> ساختار MaskedNormDiffL1_SIMD<صحیح, دوگانه> {
    دوگانه operator()(ثابت صحیح* s1, ثابت صحیح* s2, ثابت uchar* mask, صحیح len, صحیح cn) ثابت {
        اگر (cn != 1) بازگشت maskedNormDiffL1Tail<صحیح, دوگانه>(s1, s2, mask, len, cn, 0.0);
        صحیح i = 0; ثابت صحیح vstep = VTraits<v_int32>::vlanes();
        v_float64 acc = vx_setzero_f64();
        برای (; i <= len - vstep; i += vstep) {
            v_uint32 m  = v_gt(vx_load_expand_q(mask + i), vx_setzero_u32());
            v_uint32 ad = v_and(v_absdiff(vx_load(s1 + i), vx_load(s2 + i)), m);
            v_uint64 e0, e1; v_expand(ad, e0, e1);
            acc = v_add(acc, v_cvt_f64(v_reinterpret_as_s64(e0)));
            acc = v_add(acc, v_cvt_f64(v_reinterpret_as_s64(e1)));
        }
        دوگانه result = v_reduce_sum(acc);
        برای (; i < len; i++) اگر (mask[i]) result += (دوگانه)cv_absdiff(s1[i], s2[i]);
        بازگشت result;
    }
};
template<> ساختار MaskedNormDiffL1_SIMD<اعشاری, دوگانه> {
    دوگانه operator()(ثابت اعشاری* s1, ثابت اعشاری* s2, ثابت uchar* mask, صحیح len, صحیح cn) ثابت {
        اگر (cn != 1) بازگشت maskedNormDiffL1Tail<اعشاری, دوگانه>(s1, s2, mask, len, cn, 0.0);
        صحیح i = 0; ثابت صحیح vstep = VTraits<v_float32>::vlanes();
        v_float64 acc0 = vx_setzero_f64(), acc1 = vx_setzero_f64();
        برای (; i <= len - vstep; i += vstep) {
            v_float32 m  = v_reinterpret_as_f32(v_gt(vx_load_expand_q(mask + i), vx_setzero_u32()));
            v_float32 ad = v_and(v_absdiff(vx_load(s1 + i), vx_load(s2 + i)), m);
            acc0 = v_add(acc0, v_cvt_f64(ad));
            acc1 = v_add(acc1, v_cvt_f64_بالا(ad));
        }
        دوگانه result = v_reduce_sum(v_add(acc0, acc1));
        برای (; i < len; i++) اگر (mask[i]) result += (دوگانه)cv_absdiff(s1[i], s2[i]);
        بازگشت result;
    }
};

template<typename T>
ایستا درونخطی دوگانه maskedNormDiffL2_16(ثابت T* s1, ثابت T* s2, ثابت uchar* mask, صحیح len, صحیح cn) {
    اگر (cn != 1) بازگشت maskedNormDiffL2Tail<T, دوگانه>(s1, s2, mask, len, cn, 0.0);
    صحیح i = 0; ثابت صحیح vstep = VTraits<v_uint16>::vlanes();
    v_float64 acc = vx_setzero_f64();
    برای (; i <= len - vstep; i += vstep) {
        v_uint16 m  = v_gt(vx_load_expand(mask + i), vx_setzero_u16());
        v_uint16 ad = v_and(v_absdiff(vx_load(s1 + i), vx_load(s2 + i)), m);
        v_uint64 u = v_dotprod_expand_fast(ad, ad);
        acc = v_add(acc, v_cvt_f64(v_reinterpret_as_s64(u)));
    }
    دوگانه result = v_reduce_sum(acc);
    برای (; i < len; i++) اگر (mask[i]) { دوگانه v = (دوگانه)s1[i] - (دوگانه)s2[i]; result += v*v; }
    بازگشت result;
}
template<> ساختار MaskedNormDiffL2_SIMD<ushort, دوگانه> {
    دوگانه operator()(ثابت ushort* s1, ثابت ushort* s2, ثابت uchar* mask, صحیح len, صحیح cn) ثابت
    { بازگشت maskedNormDiffL2_16(s1, s2, mask, len, cn); } };
template<> ساختار MaskedNormDiffL2_SIMD<short, دوگانه> {
    دوگانه operator()(ثابت short* s1, ثابت short* s2, ثابت uchar* mask, صحیح len, صحیح cn) ثابت
    { بازگشت maskedNormDiffL2_16(s1, s2, mask, len, cn); } };

template<> ساختار MaskedNormDiffL2_SIMD<صحیح, دوگانه> {
    دوگانه operator()(ثابت صحیح* s1, ثابت صحیح* s2, ثابت uchar* mask, صحیح len, صحیح cn) ثابت {
        اگر (cn != 1) بازگشت maskedNormDiffL2Tail<صحیح, دوگانه>(s1, s2, mask, len, cn, 0.0);
        صحیح i = 0; ثابت صحیح vstep = VTraits<v_int32>::vlanes();
        v_float64 r0 = vx_setzero_f64(), r1 = vx_setzero_f64();
        برای (; i <= len - vstep; i += vstep) {
            v_uint32 m  = v_gt(vx_load_expand_q(mask + i), vx_setzero_u32());
            v_uint32 ad = v_and(v_absdiff(vx_load(s1 + i), vx_load(s2 + i)), m);
            v_uint64 e0, e1; v_expand(ad, e0, e1);
            v_float64 f0 = v_cvt_f64(v_reinterpret_as_s64(e0)), f1 = v_cvt_f64(v_reinterpret_as_s64(e1));
            r0 = v_fma(f0, f0, r0); r1 = v_fma(f1, f1, r1);
        }
        دوگانه result = v_reduce_sum(v_add(r0, r1));
        برای (; i < len; i++) اگر (mask[i]) { دوگانه v = (دوگانه)s1[i] - (دوگانه)s2[i]; result += v*v; }
        بازگشت result;
    }
};
template<> ساختار MaskedNormDiffL2_SIMD<اعشاری, دوگانه> {
    دوگانه operator()(ثابت اعشاری* s1, ثابت اعشاری* s2, ثابت uchar* mask, صحیح len, صحیح cn) ثابت {
        اگر (cn != 1) بازگشت maskedNormDiffL2Tail<اعشاری, دوگانه>(s1, s2, mask, len, cn, 0.0);
        صحیح i = 0; ثابت صحیح vstep = VTraits<v_float32>::vlanes();
        v_float64 r0 = vx_setzero_f64(), r1 = vx_setzero_f64();
        برای (; i <= len - vstep; i += vstep) {
            v_float32 m  = v_reinterpret_as_f32(v_gt(vx_load_expand_q(mask + i), vx_setzero_u32()));
            v_float32 ad = v_and(v_absdiff(vx_load(s1 + i), vx_load(s2 + i)), m);
            v_float64 f0 = v_cvt_f64(ad), f1 = v_cvt_f64_بالا(ad);
            r0 = v_fma(f0, f0, r0); r1 = v_fma(f1, f1, r1);
        }
        دوگانه result = v_reduce_sum(v_add(r0, r1));
        برای (; i < len; i++) اگر (mask[i]) { دوگانه v = (دوگانه)s1[i] - (دوگانه)s2[i]; result += v*v; }
        بازگشت result;
    }
};
template<>
ساختار MaskedNormDiffInf_SIMD<دوگانه, دوگانه> {
    درونخطی دوگانه operator()(ثابت دوگانه* s1, ثابت دوگانه* s2, ثابت uchar* mask, صحیح len, صحیح cn) ثابت {
        دوگانه result = 0.0;
        صحیح i = 0;
#اگر CV_SIMD_64F || CV_SIMD_SCALABLE_64F
        اگر (cn == 1) {
            ثابت صحیح vstep = VTraits<v_float64>::vlanes();
            v_float64 acc0 = vx_setzero_f64(), acc1 = vx_setzero_f64();
            برای (; i <= len - 2*vstep; i += 2*vstep) {
                v_uint64 m0, m1;
                v_normmask_u64(mask + i, m0, m1);
                v_float64 d0 = v_absdiff(vx_load(s1 + i), vx_load(s2 + i));
                v_float64 d1 = v_absdiff(vx_load(s1 + i + vstep), vx_load(s2 + i + vstep));
                acc0 = v_max(acc0, v_reinterpret_as_f64(v_and(v_reinterpret_as_u64(d0), m0)));
                acc1 = v_max(acc1, v_reinterpret_as_f64(v_and(v_reinterpret_as_u64(d1), m1)));
            }
            دوگانه t[VTraits<v_float64>::max_nlanes];
            vx_store(t, v_max(acc0, acc1));
            برای (صحیح k = 0; k < vstep; k++)
                result = std::max(result, t[k]);
        }
#endif
        برای (; i < len; i++) اگر (mask[i]) {
            ثابت دوگانه* e1 = s1 + i*cn; ثابت دوگانه* e2 = s2 + i*cn;
            برای (صحیح k = 0; k < cn; k++) result = std::max(result, std::abs(e1[k] - e2[k]));
        }
        بازگشت result;
    }
};

template<>
ساختار MaskedNormDiffL1_SIMD<دوگانه, دوگانه> {
    دوگانه operator()(ثابت دوگانه* s1, ثابت دوگانه* s2, ثابت uchar* mask, صحیح len, صحیح cn) ثابت {
        دوگانه result = 0.0;
        اگر (cn == 1) {
            ثابت صحیح vstep = VTraits<v_float64>::vlanes();
            صحیح i = 0;
            v_float64 acc0 = vx_setzero_f64(), acc1 = vx_setzero_f64();
            برای (; i <= len - 2*vstep; i += 2*vstep) {
                v_uint64 m0, m1;
                v_normmask_u64(mask + i, m0, m1);
                v_float64 d0 = v_absdiff(vx_load(s1 + i), vx_load(s2 + i));
                v_float64 d1 = v_absdiff(vx_load(s1 + i + vstep), vx_load(s2 + i + vstep));
                acc0 = v_add(acc0, v_reinterpret_as_f64(v_and(v_reinterpret_as_u64(d0), m0)));
                acc1 = v_add(acc1, v_reinterpret_as_f64(v_and(v_reinterpret_as_u64(d1), m1)));
            }
            result = v_reduce_sum(v_add(acc0, acc1));
            برای (; i < len; i++) اگر (mask[i]) result += std::abs(s1[i] - s2[i]);
        } وگرنه {
            result = maskedNormDiffL1Tail<دوگانه, دوگانه>(s1, s2, mask, len, cn, 0.0);
        }
        بازگشت result;
    }
};

template<>
ساختار MaskedNormDiffL2_SIMD<دوگانه, دوگانه> {
    دوگانه operator()(ثابت دوگانه* s1, ثابت دوگانه* s2, ثابت uchar* mask, صحیح len, صحیح cn) ثابت {
        دوگانه result = 0.0;
        اگر (cn == 1) {
            ثابت صحیح vstep = VTraits<v_float64>::vlanes();
            صحیح i = 0;
            v_float64 acc0 = vx_setzero_f64(), acc1 = vx_setzero_f64();
            برای (; i <= len - 2*vstep; i += 2*vstep) {
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
            برای (; i < len; i++) اگر (mask[i]) { دوگانه v = s1[i] - s2[i]; result += v*v; }
        } وگرنه {
            result = maskedNormDiffL2Tail<دوگانه, دوگانه>(s1, s2, mask, len, cn, 0.0);
        }
        بازگشت result;
    }
};
#endif // CV_SIMD_64F
#endif // CV_SIMD

template<typename T, typename ST> صحیح
normDiffInf_(ثابت T* src1, ثابت T* src2, ثابت uchar* mask, ST* _result, صحیح len, صحیح cn) {
    ST result = *_result;
    اگر( !mask ) {
        NormDiffInf_SIMD<T, ST> op;
        result = std::max(result, op(src1, src2, len*cn));
    } وگرنه {
        MaskedNormDiffInf_SIMD<T, ST> op;
        result = std::max(result, op(src1, src2, mask, len, cn));
    }
    *_result = result;
    بازگشت 0;
}

template<typename T, typename ST> صحیح
normDiffL1_(ثابت T* src1, ثابت T* src2, ثابت uchar* mask, ST* _result, صحیح len, صحیح cn) {
    ST result = *_result;
    اگر( !mask ) {
        NormDiffL1_SIMD<T, ST> op;
        result += op(src1, src2, len*cn);
    } وگرنه {
        MaskedNormDiffL1_SIMD<T, ST> op;
        result += op(src1, src2, mask, len, cn);
    }
    *_result = result;
    بازگشت 0;
}

template<typename T, typename ST> صحیح
normDiffL2_(ثابت T* src1, ثابت T* src2, ثابت uchar* mask, ST* _result, صحیح len, صحیح cn) {
    ST result = *_result;
    اگر( !mask ) {
        NormDiffL2_SIMD<T, ST> op;
        result += op(src1, src2, len*cn);
    } وگرنه {
        MaskedNormDiffL2_SIMD<T, ST> op;
        result += op(src1, src2, mask, len, cn);
    }
    *_result = result;
    بازگشت 0;
}

ایستا صحیح
normDiffInf_Bool(ثابت uchar* src1, ثابت uchar* src2, ثابت uchar* mask, صحیح* _result, صحیح len, صحیح cn)
{
    صحیح result = *_result;
    اگر( !mask )
    {
        برای( صحیح i = 0; i < len*cn; i++ ) {
            result = std::max(result, (صحیح)((src1[i] != 0) != (src2[i] != 0)));
            اگر (result != 0)
                بشکن;
        }
    }
    وگرنه
    {
        برای( صحیح i = 0; i < len; i++, src1 += cn, src2 += cn )
            اگر( mask[i] )
            {
                برای( صحیح k = 0; k < cn; k++ )
                    result = std::max(result, (صحیح)((src1[k] != 0) != (src2[k] != 0)));
                اگر (result != 0)
                    بشکن;
            }
    }
    *_result = result;
    بازگشت 0;
}

ایستا صحیح
normDiffL1_Bool(ثابت uchar* src1, ثابت uchar* src2, ثابت uchar* mask, صحیح* _result, صحیح len, صحیح cn)
{
    صحیح result = *_result;
    اگر( !mask )
    {
        برای( صحیح i = 0; i < len*cn; i++ )
            result += (صحیح)((src1[i] != 0) != (src2[i] != 0));
    }
    وگرنه
    {
        برای( صحیح i = 0; i < len; i++, src1 += cn, src2 += cn )
            اگر( mask[i] )
            {
                برای( صحیح k = 0; k < cn; k++ )
                    result += (صحیح)((src1[k] != 0) != (src2[k] != 0));
            }
    }
    *_result = result;
    بازگشت 0;
}

ایستا صحیح
normDiffL2_Bool(ثابت uchar* src1, ثابت uchar* src2, ثابت uchar* mask, صحیح* _result, صحیح len, صحیح cn)
{
    بازگشت normDiffL1_Bool(src1, src2, mask, _result, len, cn);
}

#define CV_DEF_NORM_FUNC(L, suffix, type, ntype) \
    ایستا صحیح norm##L##_##suffix(ثابت type* src, ثابت uchar* mask, ntype* r, صحیح len, صحیح cn) \
{ CV_INSTRUMENT_REGION(); بازگشت norm##L##_(src, mask, r, len, cn); } \
    ایستا صحیح normDiff##L##_##suffix(ثابت type* src1, ثابت type* src2, \
    ثابت uchar* mask, ntype* r, صحیح len, صحیح cn) \
{ بازگشت normDiff##L##_(src1, src2, mask, r, (صحیح)len, cn); }

#define CV_DEF_NORM_ALL(suffix, type, inftype, l1type, l2type) \
    CV_DEF_NORM_FUNC(Inf, suffix, type, inftype) \
    CV_DEF_NORM_FUNC(L1, suffix, type, l1type) \
    CV_DEF_NORM_FUNC(L2, suffix, type, l2type)

CV_DEF_NORM_ALL(8u, uchar, صحیح, صحیح, صحیح)
CV_DEF_NORM_ALL(8s, schar, صحیح, صحیح, صحیح)
CV_DEF_NORM_ALL(16u, ushort, صحیح, صحیح, دوگانه)
CV_DEF_NORM_ALL(16s, short, صحیح, صحیح, دوگانه)
CV_DEF_NORM_ALL(32u, unsigned, unsigned, دوگانه, دوگانه)
CV_DEF_NORM_ALL(32s, صحیح, unsigned, دوگانه, دوگانه)
CV_DEF_NORM_ALL(32f, اعشاری, اعشاری, دوگانه, دوگانه)
CV_DEF_NORM_ALL(64f, دوگانه, دوگانه, دوگانه, دوگانه)
CV_DEF_NORM_ALL(64u, uint64, uint64, دوگانه, دوگانه)
CV_DEF_NORM_ALL(64s, int64, uint64, دوگانه, دوگانه)
CV_DEF_NORM_ALL(16f, hfloat, اعشاری, اعشاری, اعشاری)
CV_DEF_NORM_ALL(16bf, bfloat, اعشاری, اعشاری, اعشاری)

NormFunc getNormFunc(صحیح normType, صحیح depth)
{
    CV_INSTRUMENT_REGION();

    ایستا NormFunc normTab[3][CV_DEPTH_MAX] =
    {
        {
            normWrap<uchar, صحیح, normInf_8u>,
            normWrap<schar, صحیح, normInf_8s>,
            normWrap<ushort, صحیح, normInf_16u>,
            normWrap<short, صحیح, normInf_16s>,
            normWrap<صحیح, unsigned, normInf_32s>,
            normWrap<اعشاری, اعشاری, normInf_32f>,
            normWrap<دوگانه, دوگانه, normInf_64f>,
            normWrap<hfloat, اعشاری, normInf_16f>,
            normWrap<bfloat, اعشاری, normInf_16bf>,
            normWrap<uchar, صحیح, normInf_Bool>,
            normWrap<uint64, uint64, normInf_64u>,
            normWrap<int64, uint64, normInf_64s>,
            normWrap<unsigned, unsigned, normInf_32u>,
            0
        },
        {
            normWrap<uchar, صحیح, normL1_8u>,
            normWrap<schar, صحیح, normL1_8s>,
            normWrap<ushort, صحیح, normL1_16u>,
            normWrap<short, صحیح, normL1_16s>,
            normWrap<صحیح, دوگانه, normL1_32s>,
            normWrap<اعشاری, دوگانه, normL1_32f>,
            normWrap<دوگانه, دوگانه, normL1_64f>,
            normWrap<hfloat, اعشاری, normL1_16f>,
            normWrap<bfloat, اعشاری, normL1_16bf>,
            normWrap<uchar, صحیح, normL1_Bool>,
            normWrap<uint64, دوگانه, normL1_64u>,
            normWrap<int64, دوگانه, normL1_64s>,
            normWrap<unsigned, دوگانه, normL1_32u>,
            0
        },
        {
            normWrap<uchar, صحیح, normL2_8u>,
            normWrap<schar, صحیح, normL2_8s>,
            normWrap<ushort, دوگانه, normL2_16u>,
            normWrap<short, دوگانه, normL2_16s>,
            normWrap<صحیح, دوگانه, normL2_32s>,
            normWrap<اعشاری, دوگانه, normL2_32f>,
            normWrap<دوگانه, دوگانه, normL2_64f>,
            normWrap<hfloat, اعشاری, normL2_16f>,
            normWrap<bfloat, اعشاری, normL2_16bf>,
            normWrap<uchar, صحیح, normL2_Bool>,
            normWrap<uint64, دوگانه, normL2_64u>,
            normWrap<int64, دوگانه, normL2_64s>,
            normWrap<unsigned, دوگانه, normL2_32u>,
            0
        }
    };

    اگر (normType >= 3 || normType < 0) بازگشت nullptr;

    بازگشت normTab[normType][depth];
}

NormDiffFunc getNormDiffFunc(صحیح normType, صحیح depth)
{
    ایستا NormDiffFunc normDiffTab[3][CV_DEPTH_MAX] =
    {
        {
            normDiffWrap<uchar, صحیح, normDiffInf_8u>,
            normDiffWrap<schar, صحیح, normDiffInf_8s>,
            normDiffWrap<ushort, صحیح, normDiffInf_16u>,
            normDiffWrap<short, صحیح, normDiffInf_16s>,
            normDiffWrap<صحیح, unsigned, normDiffInf_32s>,
            normDiffWrap<اعشاری, اعشاری, normDiffInf_32f>,
            normDiffWrap<دوگانه, دوگانه, normDiffInf_64f>,
            normDiffWrap<hfloat, اعشاری, normDiffInf_16f>,
            normDiffWrap<bfloat, اعشاری, normDiffInf_16bf>,
            normDiffWrap<uchar, صحیح, normDiffInf_Bool>,
            normDiffWrap<uint64, uint64, normDiffInf_64u>,
            normDiffWrap<int64, uint64, normDiffInf_64s>,
            normDiffWrap<unsigned, unsigned, normDiffInf_32u>,
            0
        },
        {
            normDiffWrap<uchar, صحیح, normDiffL1_8u>,
            normDiffWrap<schar, صحیح, normDiffL1_8s>,
            normDiffWrap<ushort, صحیح, normDiffL1_16u>,
            normDiffWrap<short, صحیح, normDiffL1_16s>,
            normDiffWrap<صحیح, دوگانه, normDiffL1_32s>,
            normDiffWrap<اعشاری, دوگانه, normDiffL1_32f>,
            normDiffWrap<دوگانه, دوگانه, normDiffL1_64f>,
            normDiffWrap<hfloat, اعشاری, normDiffL1_16f>,
            normDiffWrap<bfloat, اعشاری, normDiffL1_16bf>,
            normDiffWrap<uchar, صحیح, normDiffL1_Bool>,
            normDiffWrap<uint64, دوگانه, normDiffL1_64u>,
            normDiffWrap<int64, دوگانه, normDiffL1_64s>,
            normDiffWrap<unsigned, دوگانه, normDiffL1_32u>,
            0
        },
        {
            normDiffWrap<uchar, صحیح, normDiffL2_8u>,
            normDiffWrap<schar, صحیح, normDiffL2_8s>,
            normDiffWrap<ushort, دوگانه, normDiffL2_16u>,
            normDiffWrap<short, دوگانه, normDiffL2_16s>,
            normDiffWrap<صحیح, دوگانه, normDiffL2_32s>,
            normDiffWrap<اعشاری, دوگانه, normDiffL2_32f>,
            normDiffWrap<دوگانه, دوگانه, normDiffL2_64f>,
            normDiffWrap<hfloat, اعشاری, normDiffL2_16f>,
            normDiffWrap<bfloat, اعشاری, normDiffL2_16bf>,
            normDiffWrap<uchar, صحیح, normDiffL2_Bool>,
            normDiffWrap<uint64, دوگانه, normDiffL2_64u>,
            normDiffWrap<int64, دوگانه, normDiffL2_64s>,
            normDiffWrap<unsigned, دوگانه, normDiffL2_32u>,
            0
        },
    };
    اگر (normType >= 3 || normType < 0) بازگشت nullptr;

    بازگشت normDiffTab[normType][depth];
}

#endif // CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

CV_CPU_OPTIMIZATION_NAMESPACE_END

} // cv::
