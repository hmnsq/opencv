// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html
//
// Copyright (C) 2025, SpaceMIT Inc., all rights reserved.
// Copyright (C) 2026, Advanced Micro Devices, Inc., all rights reserved.
// Third party copyrights are property of their respective owners.

#அந்தர்பாவ "precomp.hpp"

நாமாகாஶ cv {

using NormFunc = முழு (*)(மாறிலி uchar*, மாறிலி uchar*, uchar*, முழு, முழு);
using NormDiffFunc = முழு (*)(மாறிலி uchar*, மாறிலி uchar*, மாறிலி uchar*, uchar*, முழு, முழு);

CV_CPU_OPTIMIZATION_NAMESPACE_BEGIN

template<typename T, typename RT, முழு (*fn)(மாறிலி T*, மாறிலி uchar*, RT*, முழு, முழு)>
நிலையான முழு normWrap(மாறிலி uchar* src1, மாறிலி uchar* src2, uchar* res, முழு len, முழு cn)
{
    திரும்பு fn((மாறிலி T*)src1, src2, (RT*)res, len, cn);
}

template<typename T, typename RT, முழு (*fn)(மாறிலி T*, மாறிலி T*, மாறிலி uchar*, RT*, முழு, முழு)>
நிலையான முழு normDiffWrap(மாறிலி uchar* src1, மாறிலி uchar* src2, மாறிலி uchar* src3, uchar* res, முழு len, முழு cn)
{
    திரும்பு fn((மாறிலி T*)src1, (மாறிலி T*)src2, src3, (RT*)res, len, cn);
}

NormFunc getNormFunc(முழு normType, முழு depth);
NormDiffFunc getNormDiffFunc(முழு normType, முழு depth);

#ifndef CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

template <typename T, typename ST>
கட்டமைப்பு NormInf_SIMD {
    உள்வரி ST operator() (மாறிலி T* src, முழு n) மாறிலி {
        ST s = 0;
        க்கு (முழு i = 0; i < n; i++) {
            s = std::max(s, (ST)cv_abs(src[i]));
        }
        திரும்பு s;
    }
};

template <typename T, typename ST>
கட்டமைப்பு NormL1_SIMD {
    உள்வரி ST operator() (மாறிலி T* src, முழு n) மாறிலி {
        ST s = 0;
        க்கு (முழு i = 0; i < n; i++) {
            s += cv_abs(src[i]);
        }
        திரும்பு s;
    }
};

template <typename T, typename ST>
கட்டமைப்பு NormL2_SIMD {
    உள்வரி ST operator() (மாறிலி T* src, முழு n) மாறிலி {
        ST s = 0;
        க்கு (முழு i = 0; i < n; i++) {
            ST v = (ST)src[i];
            s += v * v;
        }
        திரும்பு s;
    }
};

template <typename T, typename ST>
கட்டமைப்பு NormDiffInf_SIMD {
    உள்வரி ST operator() (மாறிலி T* src1, மாறிலி T* src2, முழு n) மாறிலி {
        ST s = 0;
        க்கு (முழு i = 0; i < n; i++) {
            ST v = (ST)cv_absdiff(src1[i], src2[i]);
            s = std::max(s, v);
        }
        திரும்பு s;
    }
};

template <typename T, typename ST>
கட்டமைப்பு NormDiffL1_SIMD {
    உள்வரி ST operator() (மாறிலி T* src1, மாறிலி T* src2, முழு n) மாறிலி {
        ST s = 0;
        க்கு (முழு i = 0; i < n; i++) {
            ST v = (ST)cv_absdiff(src1[i], src2[i]);
            s += v;
        }
        திரும்பு s;
    }
};

template <typename T, typename ST>
கட்டமைப்பு NormDiffL2_SIMD {
    உள்வரி ST operator() (மாறிலி T* src1, மாறிலி T* src2, முழு n) மாறிலி {
        ST s = 0;
        க்கு (முழு i = 0; i < n; i++) {
            ST v = (ST)src1[i] - (ST)src2[i];
            s += v * v;
        }
        திரும்பு s;
    }
};

#எனில் (CV_SIMD || CV_SIMD_SCALABLE)

template<>
கட்டமைப்பு NormInf_SIMD<uchar, முழு> {
    முழு operator() (மாறிலி uchar* src, முழு n) மாறிலி {
        முழு j = 0;
        முழு s = 0;
        v_uint8 r0 = vx_setzero_u8(), r1 = vx_setzero_u8();
        v_uint8 r2 = vx_setzero_u8(), r3 = vx_setzero_u8();
        க்கு (; j <= n - 4 * VTraits<v_uint8>::vlanes(); j += 4 * VTraits<v_uint8>::vlanes()) {
            r0 = v_max(r0, vx_load(src + j                                 ));
            r1 = v_max(r1, vx_load(src + j +     VTraits<v_uint8>::vlanes()));
            r2 = v_max(r2, vx_load(src + j + 2 * VTraits<v_uint8>::vlanes()));
            r3 = v_max(r3, vx_load(src + j + 3 * VTraits<v_uint8>::vlanes()));
        }
        r0 = v_max(r0, v_max(r1, v_max(r2, r3)));
        க்கு (; j < n; j++) {
            s = std::max(s, (முழு)src[j]);
        }
        திரும்பு std::max(s, (முழு)v_reduce_max(r0));
    }
};

template<>
கட்டமைப்பு NormInf_SIMD<schar, முழு> {
    முழு operator() (மாறிலி schar* src, முழு n) மாறிலி {
        முழு j = 0;
        முழு s = 0;
        v_uint8 r0 = vx_setzero_u8(), r1 = vx_setzero_u8();
        v_uint8 r2 = vx_setzero_u8(), r3 = vx_setzero_u8();
        க்கு (; j <= n - 4 * VTraits<v_int8>::vlanes(); j += 4 * VTraits<v_int8>::vlanes()) {
            r0 = v_max(r0, v_abs(vx_load(src + j                                )));
            r1 = v_max(r1, v_abs(vx_load(src + j +     VTraits<v_int8>::vlanes())));
            r2 = v_max(r2, v_abs(vx_load(src + j + 2 * VTraits<v_int8>::vlanes())));
            r3 = v_max(r3, v_abs(vx_load(src + j + 3 * VTraits<v_int8>::vlanes())));
        }
        r0 = v_max(r0, v_max(r1, v_max(r2, r3)));
        க்கு (; j < n; j++) {
            s = std::max(s, cv_abs(src[j]));
        }
        திரும்பு std::max(s, saturate_cast<முழு>(v_reduce_max(r0)));
    }
};

template<>
கட்டமைப்பு NormInf_SIMD<ushort, முழு> {
    முழு operator() (மாறிலி ushort* src, முழு n) மாறிலி {
        முழு j = 0;
        முழு s = 0;
        v_uint16 d0 = vx_setzero_u16(), d1 = vx_setzero_u16();
        v_uint16 d2 = vx_setzero_u16(), d3 = vx_setzero_u16();
        க்கு (; j <= n - 4 * VTraits<v_uint16>::vlanes(); j += 4 * VTraits<v_uint16>::vlanes()) {
            d0 = v_max(d0, vx_load(src + j                                  ));
            d1 = v_max(d1, vx_load(src + j +     VTraits<v_uint16>::vlanes()));
            d2 = v_max(d2, vx_load(src + j + 2 * VTraits<v_uint16>::vlanes()));
            d3 = v_max(d3, vx_load(src + j + 3 * VTraits<v_uint16>::vlanes()));
        }
        d0 = v_max(d0, v_max(d1, v_max(d2, d3)));
        க்கு (; j < n; j++) {
            s = std::max(s, (முழு)src[j]);
        }
        திரும்பு std::max(s, (முழு)v_reduce_max(d0));
    }
};

template<>
கட்டமைப்பு NormInf_SIMD<short, முழு> {
    முழு operator() (மாறிலி short* src, முழு n) மாறிலி {
        முழு j = 0;
        முழு s = 0;
        v_uint16 d0 = vx_setzero_u16(), d1 = vx_setzero_u16();
        v_uint16 d2 = vx_setzero_u16(), d3 = vx_setzero_u16();
        க்கு (; j <= n - 4 * VTraits<v_int16>::vlanes(); j += 4 * VTraits<v_int16>::vlanes()) {
            d0 = v_max(d0, v_abs(vx_load(src + j                                  )));
            d1 = v_max(d1, v_abs(vx_load(src + j +     VTraits<v_int16>::vlanes())));
            d2 = v_max(d2, v_abs(vx_load(src + j + 2 * VTraits<v_int16>::vlanes())));
            d3 = v_max(d3, v_abs(vx_load(src + j + 3 * VTraits<v_int16>::vlanes())));
        }
        d0 = v_max(d0, v_max(d1, v_max(d2, d3)));
        க்கு (; j < n; j++) {
            s = std::max(s, saturate_cast<முழு>(cv_abs(src[j])));
        }
        திரும்பு std::max(s, saturate_cast<முழு>(v_reduce_max(d0)));
    }
};

template<>
கட்டமைப்பு NormInf_SIMD<முழு, முழு> {
    முழு operator() (மாறிலி முழு* src, முழு n) மாறிலி {
        முழு j = 0;
        முழு s = 0;
        v_uint32 r0 = vx_setzero_u32(), r1 = vx_setzero_u32();
        v_uint32 r2 = vx_setzero_u32(), r3 = vx_setzero_u32();
        க்கு (; j <= n - 4 * VTraits<v_int32>::vlanes(); j += 4 * VTraits<v_int32>::vlanes()) {
            r0 = v_max(r0, v_abs(vx_load(src + j                                 )));
            r1 = v_max(r1, v_abs(vx_load(src + j +     VTraits<v_int32>::vlanes())));
            r2 = v_max(r2, v_abs(vx_load(src + j + 2 * VTraits<v_int32>::vlanes())));
            r3 = v_max(r3, v_abs(vx_load(src + j + 3 * VTraits<v_int32>::vlanes())));
        }
        r0 = v_max(r0, v_max(r1, v_max(r2, r3)));
        s = std::max(s, saturate_cast<முழு>(v_reduce_max(r0)));
        க்கு (; j < n; j++) {
            s = std::max(s, std::abs(src[j]));
        }
        திரும்பு s;
    }
};

template<>
கட்டமைப்பு NormInf_SIMD<தசம, தசம> {
    தசம operator() (மாறிலி தசம* src, முழு n) மாறிலி {
        முழு j = 0;
        தசம s = 0.f;
        v_float32 r0 = vx_setzero_f32(), r1 = vx_setzero_f32();
        v_float32 r2 = vx_setzero_f32(), r3 = vx_setzero_f32();
        க்கு (; j <= n - 4 * VTraits<v_float32>::vlanes(); j += 4 * VTraits<v_float32>::vlanes()) {
            r0 = v_max(r0, v_abs(vx_load(src + j                                   )));
            r1 = v_max(r1, v_abs(vx_load(src + j +     VTraits<v_float32>::vlanes())));
            r2 = v_max(r2, v_abs(vx_load(src + j + 2 * VTraits<v_float32>::vlanes())));
            r3 = v_max(r3, v_abs(vx_load(src + j + 3 * VTraits<v_float32>::vlanes())));
        }
        r0 = v_max(r0, v_max(r1, v_max(r2, r3)));
        க்கு (; j < n; j++) {
            s = std::max(s, cv_abs(src[j]));
        }
        திரும்பு std::max(s, v_reduce_max(r0));
    }
};

template<>
கட்டமைப்பு NormL1_SIMD<uchar, முழு> {
    முழு operator() (மாறிலி uchar* src, முழு n) மாறிலி {
        முழு j = 0;
        முழு s = 0;
        v_uint32 r0 = vx_setzero_u32(), r1 = vx_setzero_u32();
        v_uint8  one = vx_setall_u8(1);
        க்கு (; j<= n - 2 * VTraits<v_uint8>::vlanes(); j += 2 * VTraits<v_uint8>::vlanes()) {
            v_uint8 v0 = vx_load(src + j);
            r0 = v_dotprod_expand_fast(v0, one, r0);

            v_uint8 v1 = vx_load(src + j + VTraits<v_uint8>::vlanes());
            r1 = v_dotprod_expand_fast(v1, one, r1);
        }
        s += v_reduce_sum(v_add(r0, r1));
        க்கு (; j < n; j++) {
            s += src[j];
        }
        திரும்பு s;
    }
};

template<>
கட்டமைப்பு NormL1_SIMD<schar, முழு> {
    முழு operator() (மாறிலி schar* src, முழு n) மாறிலி {
        முழு j = 0;
        முழு s = 0;
        v_uint32 r0 = vx_setzero_u32(), r1 = vx_setzero_u32();
        v_uint8  one = vx_setall_u8(1);
        க்கு (; j<= n - 2 * VTraits<v_int8>::vlanes(); j += 2 * VTraits<v_int8>::vlanes()) {
            v_uint8 v0 = v_abs(vx_load(src + j));
            r0 = v_dotprod_expand_fast(v0, one, r0);

            v_uint8 v1 = v_abs(vx_load(src + j + VTraits<v_int8>::vlanes()));
            r1 = v_dotprod_expand_fast(v1, one, r1);
        }
        s += v_reduce_sum(v_add(r0, r1));
        க்கு (; j < n; j++) {
            s += saturate_cast<முழு>(cv_abs(src[j]));
        }
        திரும்பு s;
    }
};

template<>
கட்டமைப்பு NormL1_SIMD<ushort, முழு> {
    முழு operator() (மாறிலி ushort* src, முழு n) மாறிலி {
        முழு j = 0;
        முழு s = 0;
        v_uint32 r00 = vx_setzero_u32(), r01 = vx_setzero_u32();
        v_uint32 r10 = vx_setzero_u32(), r11 = vx_setzero_u32();
        க்கு (; j<= n - 2 * VTraits<v_uint16>::vlanes(); j += 2 * VTraits<v_uint16>::vlanes()) {
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
        s += (முழு)v_reduce_sum(v_add(v_add(v_add(r00, r01), r10), r11));
        க்கு (; j < n; j++) {
            s += src[j];
        }
        திரும்பு s;
    }
};

template<>
கட்டமைப்பு NormL1_SIMD<short, முழு> {
    முழு operator() (மாறிலி short* src, முழு n) மாறிலி {
        முழு j = 0;
        முழு s = 0;
        v_uint32 r00 = vx_setzero_u32(), r01 = vx_setzero_u32();
        v_uint32 r10 = vx_setzero_u32(), r11 = vx_setzero_u32();
        க்கு (; j<= n - 2 * VTraits<v_int16>::vlanes(); j += 2 * VTraits<v_int16>::vlanes()) {
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
        s += (முழு)v_reduce_sum(v_add(v_add(v_add(r00, r01), r10), r11));
        க்கு (; j < n; j++) {
            s += saturate_cast<முழு>(cv_abs(src[j]));
        }
        திரும்பு s;
    }
};

template<>
கட்டமைப்பு NormL2_SIMD<uchar, முழு> {
    முழு operator() (மாறிலி uchar* src, முழு n) மாறிலி {
        முழு j = 0;
        முழு s = 0;
        v_uint32 r0 = vx_setzero_u32(), r1 = vx_setzero_u32();
        க்கு (; j <= n - 2 * VTraits<v_uint8>::vlanes(); j += 2 * VTraits<v_uint8>::vlanes()) {
            v_uint8 v0 = vx_load(src + j);
            r0 = v_dotprod_expand_fast(v0, v0, r0);

            v_uint8 v1 = vx_load(src + j + VTraits<v_uint8>::vlanes());
            r1 = v_dotprod_expand_fast(v1, v1, r1);
        }
        s += v_reduce_sum(v_add(r0, r1));
        க்கு (; j < n; j++) {
            முழு v = saturate_cast<முழு>(src[j]);
            s += v * v;
        }
        திரும்பு s;
    }
};

template<>
கட்டமைப்பு NormL2_SIMD<schar, முழு> {
    முழு operator() (மாறிலி schar* src, முழு n) மாறிலி {
        முழு j = 0;
        முழு s = 0;
        v_int32 r0 = vx_setzero_s32(), r1 = vx_setzero_s32();
        க்கு (; j <= n - 2 * VTraits<v_int8>::vlanes(); j += 2 * VTraits<v_int8>::vlanes()) {
            v_int8 v0 = vx_load(src + j);
            r0 = v_dotprod_expand_fast(v0, v0, r0);
            v_int8 v1 = vx_load(src + j + VTraits<v_int8>::vlanes());
            r1 = v_dotprod_expand_fast(v1, v1, r1);
        }
        s += v_reduce_sum(v_add(r0, r1));
        க்கு (; j < n; j++) {
            முழு v = saturate_cast<முழு>(src[j]);
            s += v * v;
        }
        திரும்பு s;
    }
};

template<>
கட்டமைப்பு NormDiffInf_SIMD<uchar, முழு> {
    முழு operator() (மாறிலி uchar* src1, மாறிலி uchar* src2, முழு n) மாறிலி {
        முழு j = 0;
        முழு s = 0;
        v_uint8 r0 = vx_setzero_u8(), r1 = vx_setzero_u8();
        v_uint8 r2 = vx_setzero_u8(), r3 = vx_setzero_u8();
        க்கு (; j <= n - 4 * VTraits<v_uint8>::vlanes(); j += 4 * VTraits<v_uint8>::vlanes()) {
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
        s = (முழு)v_reduce_max(v_max(v_max(v_max(r0, r1), r2), r3));
        க்கு (; j < n; j++) {
            முழு v = (முழு)cv_absdiff(src1[j], src2[j]);
            s = std::max(s, v);
        }
        திரும்பு s;
    }
};

template<>
கட்டமைப்பு NormDiffInf_SIMD<schar, முழு> {
    முழு operator() (மாறிலி schar* src1, மாறிலி schar* src2, முழு n) மாறிலி {
        முழு j = 0;
        முழு s = 0;
        v_uint8 r0 = vx_setzero_u8(), r1 = vx_setzero_u8();
        v_uint8 r2 = vx_setzero_u8(), r3 = vx_setzero_u8();
        க்கு (; j <= n - 4 * VTraits<v_int8>::vlanes(); j += 4 * VTraits<v_int8>::vlanes()) {
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
        s = (முழு)v_reduce_max(v_max(v_max(v_max(r0, r1), r2), r3));
        க்கு (; j < n; j++) {
            முழு v = (முழு)cv_absdiff(src1[j], src2[j]);
            s = std::max(s, v);
        }
        திரும்பு s;
    }
};

template<>
கட்டமைப்பு NormDiffInf_SIMD<ushort, முழு> {
    முழு operator() (மாறிலி ushort* src1, மாறிலி ushort* src2, முழு n) மாறிலி {
        முழு j = 0;
        முழு s = 0;
        v_uint16 r0 = vx_setzero_u16(), r1 = vx_setzero_u16();
        v_uint16 r2 = vx_setzero_u16(), r3 = vx_setzero_u16();
        க்கு (; j <= n - 4 * VTraits<v_uint16>::vlanes(); j += 4 * VTraits<v_uint16>::vlanes()) {
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
        s = (முழு)v_reduce_max(v_max(v_max(v_max(r0, r1), r2), r3));
        க்கு (; j < n; j++) {
            முழு v = (முழு)cv_absdiff(src1[j], src2[j]);
            s = std::max(s, v);
        }
        திரும்பு s;
    }
};

template<>
கட்டமைப்பு NormDiffInf_SIMD<short, முழு> {
    முழு operator() (மாறிலி short* src1, மாறிலி short* src2, முழு n) மாறிலி {
        முழு j = 0;
        முழு s = 0;
        v_uint16 r0 = vx_setzero_u16(), r1 = vx_setzero_u16();
        v_uint16 r2 = vx_setzero_u16(), r3 = vx_setzero_u16();
        க்கு (; j <= n - 4 * VTraits<v_int16>::vlanes(); j += 4 * VTraits<v_int16>::vlanes()) {
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
        s = (முழு)v_reduce_max(v_max(v_max(v_max(r0, r1), r2), r3));
        க்கு (; j < n; j++) {
            முழு v = (முழு)cv_absdiff(src1[j], src2[j]);
            s = std::max(s, v);
        }
        திரும்பு s;
    }
};

template<>
கட்டமைப்பு NormDiffInf_SIMD<முழு, unsigned> {
    unsigned operator() (மாறிலி முழு* src1, மாறிலி முழு* src2, முழு n) மாறிலி {
        முழு j = 0;
        unsigned s = 0;
        v_uint32 r0 = vx_setzero_u32(), r1 = vx_setzero_u32();
        v_uint32 r2 = vx_setzero_u32(), r3 = vx_setzero_u32();
        க்கு (; j <= n - 4 * VTraits<v_int32>::vlanes(); j += 4 * VTraits<v_int32>::vlanes()) {
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
        s = (முழு)v_reduce_max(v_max(v_max(v_max(r0, r1), r2), r3));
        க்கு (; j < n; j++) {
            unsigned v = (unsigned)cv_absdiff(src1[j], src2[j]);
            s = std::max(s, v);
        }
        திரும்பு s;
    }
};

template<>
கட்டமைப்பு NormDiffInf_SIMD<தசம, தசம> {
    தசம operator() (மாறிலி தசம* src1, மாறிலி தசம* src2, முழு n) மாறிலி {
        முழு j = 0;
        தசம s = 0;
        v_float32 r0 = vx_setzero_f32(), r1 = vx_setzero_f32();
        க்கு (; j <= n - 2 * VTraits<v_float32>::vlanes(); j += 2 * VTraits<v_float32>::vlanes()) {
            v_float32 v01 = vx_load(src1 + j), v02 = vx_load(src2 + j);
            r0 = v_max(r0, v_absdiff(v01, v02));

            v_float32 v11 = vx_load(src1 + j + VTraits<v_float32>::vlanes()),
                      v12 = vx_load(src2 + j + VTraits<v_float32>::vlanes());
            r1 = v_max(r1, v_absdiff(v11, v12));
        }
        s = v_reduce_max(v_max(r0, r1));
        க்கு (; j < n; j++) {
            தசம v = (தசம)cv_absdiff(src1[j], src2[j]);
            s = std::max(s, v);
        }
        திரும்பு s;
    }
};

template<>
கட்டமைப்பு NormDiffL1_SIMD<uchar, முழு> {
    முழு operator() (மாறிலி uchar* src1, மாறிலி uchar* src2, முழு n) மாறிலி {
        முழு j = 0;
        முழு s = 0;
        v_uint32 r0 = vx_setzero_u32(), r1 = vx_setzero_u32();
        v_uint8  one = vx_setall_u8(1);
        க்கு (; j<= n - 2 * VTraits<v_uint8>::vlanes(); j += 2 * VTraits<v_uint8>::vlanes()) {
            v_uint8 v01 = vx_load(src1 + j), v02 = vx_load(src2 + j);
            r0 = v_dotprod_expand_fast(v_absdiff(v01, v02), one, r0);

            v_uint8 v11 = vx_load(src1 + j + VTraits<v_uint8>::vlanes()),
                    v12 = vx_load(src2 + j + VTraits<v_uint8>::vlanes());
            r1 = v_dotprod_expand_fast(v_absdiff(v11, v12), one, r1);
        }
        s += v_reduce_sum(v_add(r0, r1));
        க்கு (; j < n; j++) {
            முழு v = (முழு)cv_absdiff(src1[j], src2[j]);
            s += v;
        }
        திரும்பு s;
    }
};

template<>
கட்டமைப்பு NormDiffL1_SIMD<schar, முழு> {
    முழு operator() (மாறிலி schar* src1, மாறிலி schar* src2, முழு n) மாறிலி {
        முழு j = 0;
        முழு s = 0;
        v_uint32 r0 = vx_setzero_u32(), r1 = vx_setzero_u32();
        v_uint8  one = vx_setall_u8(1);
        க்கு (; j<= n - 2 * VTraits<v_int8>::vlanes(); j += 2 * VTraits<v_int8>::vlanes()) {
            v_int8 v01 = vx_load(src1 + j), v02 = vx_load(src2 + j);
            r0 = v_dotprod_expand_fast(v_absdiff(v01, v02), one, r0);

            v_int8 v11 = vx_load(src1 + j + VTraits<v_int8>::vlanes()),
                   v12 = vx_load(src2 + j + VTraits<v_int8>::vlanes());
            r1 = v_dotprod_expand_fast(v_absdiff(v11, v12), one, r1);
        }
        s += v_reduce_sum(v_add(r0, r1));
        க்கு (; j < n; j++) {
            முழு v = (முழு)cv_absdiff(src1[j], src2[j]);
            s += v;
        }
        திரும்பு s;
    }
};

template<>
கட்டமைப்பு NormDiffL1_SIMD<ushort, முழு> {
    முழு operator() (மாறிலி ushort* src1, மாறிலி ushort* src2, முழு n) மாறிலி {
        முழு j = 0;
        முழு s = 0;
        v_uint32 r0 = vx_setzero_u32(), r1 = vx_setzero_u32();
        v_uint32 r2 = vx_setzero_u32(), r3 = vx_setzero_u32();
        க்கு (; j<= n - 4 * VTraits<v_uint16>::vlanes(); j += 4 * VTraits<v_uint16>::vlanes()) {
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
        s += (முழு)v_reduce_sum(v_add(v_add(v_add(r0, r1), r2), r3));
        க்கு (; j < n; j++) {
            முழு v = (முழு)cv_absdiff(src1[j], src2[j]);
            s += v;
        }
        திரும்பு s;
    }
};

template<>
கட்டமைப்பு NormDiffL1_SIMD<short, முழு> {
    முழு operator() (மாறிலி short* src1, மாறிலி short* src2, முழு n) மாறிலி {
        முழு j = 0;
        முழு s = 0;
        v_uint32 r0 = vx_setzero_u32(), r1 = vx_setzero_u32();
        v_uint32 r2 = vx_setzero_u32(), r3 = vx_setzero_u32();
        க்கு (; j<= n - 4 * VTraits<v_int16>::vlanes(); j += 4 * VTraits<v_int16>::vlanes()) {
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
        s += (முழு)v_reduce_sum(v_add(v_add(v_add(r0, r1), r2), r3));
        க்கு (; j < n; j++) {
            முழு v = (முழு)cv_absdiff(src1[j], src2[j]);
            s += v;
        }
        திரும்பு s;
    }
};

template<>
கட்டமைப்பு NormDiffL2_SIMD<uchar, முழு> {
    முழு operator() (மாறிலி uchar* src1, மாறிலி uchar* src2, முழு n) மாறிலி {
        முழு j = 0;
        முழு s = 0;
        v_uint32 r0 = vx_setzero_u32(), r1 = vx_setzero_u32();
        க்கு (; j <= n - 2 * VTraits<v_uint8>::vlanes(); j += 2 * VTraits<v_uint8>::vlanes()) {
            v_uint8 v01 = vx_load(src1 + j), v02 = vx_load(src2 + j);
            v_uint8 v0 = v_absdiff(v01, v02);
            r0 = v_dotprod_expand_fast(v0, v0, r0);

            v_uint8 v11 = vx_load(src1 + j + VTraits<v_uint8>::vlanes()),
                    v12 = vx_load(src2 + j + VTraits<v_uint8>::vlanes());
            v_uint8 v1 = v_absdiff(v11, v12);
            r1 = v_dotprod_expand_fast(v1, v1, r1);
        }
        s += v_reduce_sum(v_add(r0, r1));
        க்கு (; j < n; j++) {
            முழு v = (முழு)src1[j] - (முழு)src2[j];
            s += v * v;
        }
        திரும்பு s;
    }
};

template<>
கட்டமைப்பு NormDiffL2_SIMD<schar, முழு> {
    முழு operator() (மாறிலி schar* src1, மாறிலி schar* src2, முழு n) மாறிலி {
        முழு j = 0;
        முழு s = 0;
        v_uint32 r0 = vx_setzero_u32(), r1 = vx_setzero_u32();
        க்கு (; j <= n - 2 * VTraits<v_int8>::vlanes(); j += 2 * VTraits<v_int8>::vlanes()) {
            v_int8 v01 = vx_load(src1 + j), v02 = vx_load(src2 + j);
            v_uint8 v0 = v_absdiff(v01, v02);
            r0 = v_dotprod_expand_fast(v0, v0, r0);

            v_int8 v11 = vx_load(src1 + j + VTraits<v_int8>::vlanes()),
                   v12 = vx_load(src2 + j + VTraits<v_int8>::vlanes());
            v_uint8 v1 = v_absdiff(v11, v12);
            r1 = v_dotprod_expand_fast(v1, v1, r1);
        }
        s += v_reduce_sum(v_add(r0, r1));
        க்கு (; j < n; j++) {
            முழு v = (முழு)src1[j] - (முழு)src2[j];
            s += v * v;
        }
        திரும்பு s;
    }
};

#endif

#எனில் (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)

template<>
கட்டமைப்பு NormInf_SIMD<இரட்டை, இரட்டை> {
    இரட்டை operator() (மாறிலி இரட்டை* src, முழு n) மாறிலி {
        முழு j = 0;
        இரட்டை s = 0.f;
        v_float64 r0 = vx_setzero_f64(), r1 = vx_setzero_f64();
        v_float64 r2 = vx_setzero_f64(), r3 = vx_setzero_f64();
        க்கு (; j <= n - 4 * VTraits<v_float64>::vlanes(); j += 4 * VTraits<v_float64>::vlanes()) {
            r0 = v_max(r0, v_abs(vx_load(src + j                                   )));
            r1 = v_max(r1, v_abs(vx_load(src + j +     VTraits<v_float64>::vlanes())));
            r2 = v_max(r2, v_abs(vx_load(src + j + 2 * VTraits<v_float64>::vlanes())));
            r3 = v_max(r3, v_abs(vx_load(src + j + 3 * VTraits<v_float64>::vlanes())));
        }
        r0 = v_max(r0, v_max(r1, v_max(r2, r3)));
        க்கு (; j < n; j++) {
            s = std::max(s, cv_abs(src[j]));
        }
        // [TODO]: use v_reduce_max when it supports float64
        இரட்டை t[VTraits<v_float64>::max_nlanes];
        vx_store(t, r0);
        க்கு (முழு i = 0; i < VTraits<v_float64>::vlanes(); i++) {
            s = std::max(s, cv_abs(t[i]));
        }
        திரும்பு s;
    }
};

template<>
கட்டமைப்பு NormL1_SIMD<முழு, இரட்டை> {
    இரட்டை operator() (மாறிலி முழு* src, முழு n) மாறிலி {
        முழு j = 0;
        இரட்டை s = 0.f;
        v_float64 r00 = vx_setzero_f64(), r01 = vx_setzero_f64();
        v_float64 r10 = vx_setzero_f64(), r11 = vx_setzero_f64();
        க்கு (; j <= n - 2 * VTraits<v_int32>::vlanes(); j += 2 * VTraits<v_int32>::vlanes()) {
            // Widen to double before taking the magnitude: exact over the whole int32 range,
            // INT_MIN included, which a float intermediate would not be above 2^24.
            v_int32 i0 = vx_load(src + j), i1 = vx_load(src + j + VTraits<v_int32>::vlanes());
            r00 = v_add(r00, v_abs(v_cvt_f64(i0))); r01 = v_add(r01, v_abs(v_cvt_f64_உயர்வு(i0)));
            r10 = v_add(r10, v_abs(v_cvt_f64(i1))); r11 = v_add(r11, v_abs(v_cvt_f64_உயர்வு(i1)));
        }
        s += v_reduce_sum(v_add(v_add(v_add(r00, r01), r10), r11));
        க்கு (; j < n; j++) {
            s += cv_abs(src[j]);
        }
        திரும்பு s;
    }
};

template<>
கட்டமைப்பு NormL1_SIMD<தசம, இரட்டை> {
    இரட்டை operator() (மாறிலி தசம* src, முழு n) மாறிலி {
        முழு j = 0;
        இரட்டை s = 0.f;
        v_float64 r00 = vx_setzero_f64(), r01 = vx_setzero_f64();
        v_float64 r10 = vx_setzero_f64(), r11 = vx_setzero_f64();
        v_float64 r20 = vx_setzero_f64(), r21 = vx_setzero_f64();
        v_float64 r30 = vx_setzero_f64(), r31 = vx_setzero_f64();
        க்கு (; j <= n - 4 * VTraits<v_float32>::vlanes(); j += 4 * VTraits<v_float32>::vlanes()) {
            v_float32 v0 = v_abs(vx_load(src + j)), v1 = v_abs(vx_load(src + j + VTraits<v_float32>::vlanes()));
            r00 = v_add(r00, v_cvt_f64(v0)); r01 = v_add(r01, v_cvt_f64_உயர்வு(v0));
            r10 = v_add(r10, v_cvt_f64(v1)); r11 = v_add(r11, v_cvt_f64_உயர்வு(v1));

            v_float32 v2 = v_abs(vx_load(src + j + 2 * VTraits<v_float32>::vlanes())), v3 = v_abs(vx_load(src + j + 3 * VTraits<v_float32>::vlanes()));
            r20 = v_add(r20, v_cvt_f64(v2)); r21 = v_add(r21, v_cvt_f64_உயர்வு(v2));
            r30 = v_add(r30, v_cvt_f64(v3)); r31 = v_add(r31, v_cvt_f64_உயர்வு(v3));
        }
        s += v_reduce_sum(v_add(v_add(v_add(r00, r01), r10), r11));
        s += v_reduce_sum(v_add(v_add(v_add(r20, r21), r30), r31));
        க்கு (; j < n; j++) {
            s += cv_abs(src[j]);
        }
        திரும்பு s;
    }
};

template<>
கட்டமைப்பு NormL2_SIMD<ushort, இரட்டை> {
    இரட்டை operator() (மாறிலி ushort* src, முழு n) மாறிலி {
        முழு j = 0;
        இரட்டை s = 0.f;
        v_float64 r0 = vx_setzero_f64(), r1 = vx_setzero_f64();
        க்கு (; j <= n - 2 * VTraits<v_uint16>::vlanes(); j += 2 * VTraits<v_uint16>::vlanes()) {
            v_uint16 v0 = vx_load(src + j);
            v_uint64 u0 = v_dotprod_expand_fast(v0, v0);
            r0 = v_add(r0, v_cvt_f64(v_reinterpret_as_s64(u0)));

            v_uint16 v1 = vx_load(src + j + VTraits<v_uint16>::vlanes());
            v_uint64 u1 = v_dotprod_expand_fast(v1, v1);
            r1 = v_add(r1, v_cvt_f64(v_reinterpret_as_s64(u1)));
        }
        s += v_reduce_sum(v_add(r0, r1));
        க்கு (; j < n; j++) {
            இரட்டை v = saturate_cast<இரட்டை>(src[j]);
            s += v * v;
        }
        திரும்பு s;
    }
};

template<>
கட்டமைப்பு NormL2_SIMD<short, இரட்டை> {
    இரட்டை operator() (மாறிலி short* src, முழு n) மாறிலி {
        முழு j = 0;
        இரட்டை s = 0.f;
        v_float64 r0 = vx_setzero_f64(), r1 = vx_setzero_f64();
        க்கு (; j <= n - 2 * VTraits<v_int16>::vlanes(); j += 2 * VTraits<v_int16>::vlanes()) {
            v_int16 v0 = vx_load(src + j);
            r0 = v_add(r0, v_cvt_f64(v_dotprod_expand_fast(v0, v0)));

            v_int16 v1 = vx_load(src + j + VTraits<v_int16>::vlanes());
            r1 = v_add(r1, v_cvt_f64(v_dotprod_expand_fast(v1, v1)));
        }
        s += v_reduce_sum(v_add(r0, r1));
        க்கு (; j < n; j++) {
            இரட்டை v = saturate_cast<இரட்டை>(src[j]);
            s += v * v;
        }
        திரும்பு s;
    }
};

template<>
கட்டமைப்பு NormL2_SIMD<முழு, இரட்டை> {
    இரட்டை operator() (மாறிலி முழு* src, முழு n) மாறிலி {
        முழு j = 0;
        இரட்டை s = 0.f;
        v_float64 r0 = vx_setzero_f64(), r1 = vx_setzero_f64();
        க்கு (; j <= n - 2 * VTraits<v_int32>::vlanes(); j += 2 * VTraits<v_int32>::vlanes()) {
            v_int32 v0 = vx_load(src + j);
            r0 = v_dotprod_expand_fast(v0, v0, r0);

            v_int32 v1 = vx_load(src + j + VTraits<v_int32>::vlanes());
            r1 = v_dotprod_expand_fast(v1, v1, r1);
        }
        s += v_reduce_sum(v_add(r0, r1));
        க்கு (; j < n; j++) {
            இரட்டை v = src[j];
            s += v * v;
        }
        திரும்பு s;
    }
};

template<>
கட்டமைப்பு NormL2_SIMD<தசம, இரட்டை> {
    இரட்டை operator() (மாறிலி தசம* src, முழு n) மாறிலி {
        முழு j = 0;
        இரட்டை s = 0.f;
        v_float64 r00 = vx_setzero_f64(), r01 = vx_setzero_f64();
        v_float64 r10 = vx_setzero_f64(), r11 = vx_setzero_f64();
        க்கு (; j <= n - 2 * VTraits<v_float32>::vlanes(); j += 2 * VTraits<v_float32>::vlanes()) {
            v_float32 v0 = vx_load(src + j), v1 = vx_load(src + j + VTraits<v_float32>::vlanes());
            v_float64 v00 = v_cvt_f64(v0), v01 = v_cvt_f64_உயர்வு(v0);
            v_float64 v10 = v_cvt_f64(v1), v11 = v_cvt_f64_உயர்வு(v1);
            r00 = v_fma(v00, v00, r00); r01 = v_fma(v01, v01, r01);
            r10 = v_fma(v10, v10, r10); r11 = v_fma(v11, v11, r11);
        }
        s += v_reduce_sum(v_add(v_add(v_add(r00, r01), r10), r11));
        க்கு (; j < n; j++) {
            இரட்டை v = src[j];
            s += v * v;
        }
        திரும்பு s;
    }
};

#endif

#எனில் CV_SIMD_64F // CV_SIMD_SCALABLE_64F has accuracy problem with the following kernels on ci

template<>
கட்டமைப்பு NormL1_SIMD<இரட்டை, இரட்டை> {
    இரட்டை operator() (மாறிலி இரட்டை* src, முழு n) மாறிலி {
        முழு j = 0;
        இரட்டை s = 0.f;
        v_float64 r00 = vx_setzero_f64(), r01 = vx_setzero_f64();
        v_float64 r10 = vx_setzero_f64(), r11 = vx_setzero_f64();
        க்கு (; j <= n - 4 * VTraits<v_float64>::vlanes(); j += 4 * VTraits<v_float64>::vlanes()) {
            r00 = v_add(r00, v_abs(vx_load(src + j                                   )));
            r01 = v_add(r01, v_abs(vx_load(src + j +     VTraits<v_float64>::vlanes())));
            r10 = v_add(r10, v_abs(vx_load(src + j + 2 * VTraits<v_float64>::vlanes())));
            r11 = v_add(r11, v_abs(vx_load(src + j + 3 * VTraits<v_float64>::vlanes())));
        }
        s += v_reduce_sum(v_add(v_add(v_add(r00, r01), r10), r11));
        க்கு (; j < n; j++) {
            s += cv_abs(src[j]);
        }
        திரும்பு s;
    }
};

template<>
கட்டமைப்பு NormL2_SIMD<இரட்டை, இரட்டை> {
    இரட்டை operator() (மாறிலி இரட்டை* src, முழு n) மாறிலி {
        முழு j = 0;
        இரட்டை s = 0.f;
        v_float64 r00 = vx_setzero_f64(), r01 = vx_setzero_f64();
        v_float64 r10 = vx_setzero_f64(), r11 = vx_setzero_f64();
        க்கு (; j <= n - 4 * VTraits<v_float64>::vlanes(); j += 4 * VTraits<v_float64>::vlanes()) {
            v_float64 v00 = vx_load(src + j                                   );
            v_float64 v01 = vx_load(src + j +     VTraits<v_float64>::vlanes());
            v_float64 v10 = vx_load(src + j + 2 * VTraits<v_float64>::vlanes());
            v_float64 v11 = vx_load(src + j + 3 * VTraits<v_float64>::vlanes());
            r00 = v_fma(v00, v00, r00); r01 = v_fma(v01, v01, r01);
            r10 = v_fma(v10, v10, r10); r11 = v_fma(v11, v11, r11);
        }
        s += v_reduce_sum(v_add(v_add(v_add(r00, r01), r10), r11));
        க்கு (; j < n; j++) {
            இரட்டை v = src[j];
            s += v * v;
        }
        திரும்பு s;
    }
};

template<>
கட்டமைப்பு NormDiffInf_SIMD<இரட்டை, இரட்டை> {
    இரட்டை operator() (மாறிலி இரட்டை* src1, மாறிலி இரட்டை* src2, முழு n) மாறிலி {
        முழு j = 0;
        இரட்டை s = 0;
        v_float64 r0 = vx_setzero_f64(), r1 = vx_setzero_f64();
        க்கு (; j <= n - 2 * VTraits<v_float64>::vlanes(); j += 2 * VTraits<v_float64>::vlanes()) {
            v_float64 v01 = vx_load(src1 + j), v02 = vx_load(src2 + j);
            r0 = v_max(r0, v_absdiff(v01, v02));

            v_float64 v11 = vx_load(src1 + j + VTraits<v_float64>::vlanes()),
                      v12 = vx_load(src2 + j + VTraits<v_float64>::vlanes());
            r1 = v_max(r1, v_absdiff(v11, v12));
        }
        // [TODO]: use v_reduce_max when it supports float64
        இரட்டை t[VTraits<v_float64>::max_nlanes];
        vx_store(t, v_max(r0, r1));
        க்கு (முழு i = 0; i < VTraits<v_float64>::vlanes(); i++) {
            s = std::max(s, t[i]);
        }
        க்கு (; j < n; j++) {
            இரட்டை v = (இரட்டை)cv_absdiff(src1[j], src2[j]);
            s = std::max(s, v);
        }
        திரும்பு s;
    }
};

template<>
கட்டமைப்பு NormDiffL1_SIMD<தசம, இரட்டை> {
    இரட்டை operator() (மாறிலி தசம* src1, மாறிலி தசம* src2, முழு n) மாறிலி {
        முழு j = 0;
        இரட்டை s = 0.f;
        v_float64 r0 = vx_setzero_f64(), r1 = vx_setzero_f64();
        v_float64 r2 = vx_setzero_f64(), r3 = vx_setzero_f64();
        க்கு (; j <= n - 2 * VTraits<v_float32>::vlanes(); j += 2 * VTraits<v_float32>::vlanes()) {
            v_float32 v01 = vx_load(src1 + j), v02 = vx_load(src2 + j);
            v_float32 v0 = v_absdiff(v01, v02);
            r0 = v_add(r0, v_cvt_f64(v0)); r1 = v_add(r1, v_cvt_f64_உயர்வு(v0));

            v_float32 v11 = vx_load(src1 + j + VTraits<v_float32>::vlanes()),
                      v12 = vx_load(src2 + j + VTraits<v_float32>::vlanes());
            v_float32 v1 = v_absdiff(v11, v12);
            r2 = v_add(r2, v_cvt_f64(v1)); r3 = v_add(r3, v_cvt_f64_உயர்வு(v1));
        }
        s += v_reduce_sum(v_add(v_add(v_add(r0, r1), r2), r3));
        க்கு (; j < n; j++) {
            இரட்டை v = (இரட்டை)cv_absdiff(src1[j], src2[j]);
            s += v;
        }
        திரும்பு s;
    }
};

template<>
கட்டமைப்பு NormDiffL1_SIMD<இரட்டை, இரட்டை> {
    இரட்டை operator() (மாறிலி இரட்டை* src1, மாறிலி இரட்டை* src2, முழு n) மாறிலி {
        முழு j = 0;
        இரட்டை s = 0.f;
        v_float64 r0 = vx_setzero_f64(), r1 = vx_setzero_f64();
        க்கு (; j <= n - 2 * VTraits<v_float64>::vlanes(); j += 2 * VTraits<v_float64>::vlanes()) {
            v_float64 v01 = vx_load(src1 + j), v02 = vx_load(src2 + j);
            r0 = v_add(r0, v_absdiff(v01, v02));

            v_float64 v11 = vx_load(src1 + j + VTraits<v_float64>::vlanes()),
                      v12 = vx_load(src2 + j + VTraits<v_float64>::vlanes());
            r1 = v_add(r1, v_absdiff(v11, v12));
        }
        s += v_reduce_sum(v_add(r0, r1));
        க்கு (; j < n; j++) {
            இரட்டை v = (இரட்டை)cv_absdiff(src1[j], src2[j]);
            s += v;
        }
        திரும்பு s;
    }
};

template<>
கட்டமைப்பு NormDiffL2_SIMD<ushort, இரட்டை> {
    இரட்டை operator() (மாறிலி ushort* src1, மாறிலி ushort* src2, முழு n) மாறிலி {
        முழு j = 0;
        இரட்டை s = 0.f;
        v_float64 r0 = vx_setzero_f64(), r1 = vx_setzero_f64();
        க்கு (; j <= n - 2 * VTraits<v_uint16>::vlanes(); j += 2 * VTraits<v_uint16>::vlanes()) {
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
        க்கு (; j < n; j++) {
            இரட்டை v = (இரட்டை)src1[j] - (இரட்டை)src2[j];
            s += v * v;
        }
        திரும்பு s;
    }
};

template<>
கட்டமைப்பு NormDiffL2_SIMD<short, இரட்டை> {
    இரட்டை operator() (மாறிலி short* src1, மாறிலி short* src2, முழு n) மாறிலி {
        முழு j = 0;
        இரட்டை s = 0.f;
        v_float64 r0 = vx_setzero_f64(), r1 = vx_setzero_f64();
        க்கு (; j <= n - 2 * VTraits<v_int16>::vlanes(); j += 2 * VTraits<v_int16>::vlanes()) {
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
        க்கு (; j < n; j++) {
            இரட்டை v = (இரட்டை)src1[j] - (இரட்டை)src2[j];
            s += v * v;
        }
        திரும்பு s;
    }
};

template<>
கட்டமைப்பு NormDiffL2_SIMD<முழு, இரட்டை> {
    இரட்டை operator() (மாறிலி முழு* src1, மாறிலி முழு* src2, முழு n) மாறிலி {
        முழு j = 0;
        இரட்டை s = 0.f;
        v_float64 r0 = vx_setzero_f64(), r1 = vx_setzero_f64();
        க்கு (; j <= n - VTraits<v_int32>::vlanes(); j += VTraits<v_int32>::vlanes()) {
            v_int32 v01 = vx_load(src1 + j), v02 = vx_load(src2 + j);
            v_uint32 v0 = v_absdiff(v01, v02);
            v_uint64 ev0, ev1;
            v_expand(v0, ev0, ev1);
            v_float64 f0 = v_cvt_f64(v_reinterpret_as_s64(ev0)), f1 = v_cvt_f64(v_reinterpret_as_s64(ev1));
            r0 = v_fma(f0, f0, r0); r1 = v_fma(f1, f1, r1);
        }
        s += v_reduce_sum(v_add(r0, r1));
        க்கு (; j < n; j++) {
            இரட்டை v = (இரட்டை)src1[j] - (இரட்டை)src2[j];
            s += v * v;
        }
        திரும்பு s;
    }
};

template<>
கட்டமைப்பு NormDiffL2_SIMD<தசம, இரட்டை> {
    இரட்டை operator() (மாறிலி தசம* src1, மாறிலி தசம* src2, முழு n) மாறிலி {
        முழு j = 0;
        இரட்டை s = 0.f;
        v_float64 r0 = vx_setzero_f64(), r1 = vx_setzero_f64();
        v_float64 r2 = vx_setzero_f64(), r3 = vx_setzero_f64();
        க்கு (; j <= n - 2 * VTraits<v_float32>::vlanes(); j += 2 * VTraits<v_float32>::vlanes()) {
            v_float32 v01 = vx_load(src1 + j), v02 = vx_load(src2 + j);
            v_float32 v0 = v_absdiff(v01, v02);
            v_float64 f01 = v_cvt_f64(v0), f02 = v_cvt_f64_உயர்வு(v0);
            r0 = v_fma(f01, f01, r0); r1 = v_fma(f02, f02, r1);

            v_float32 v11 = vx_load(src1 + j + VTraits<v_float32>::vlanes()),
                      v12 = vx_load(src2 + j + VTraits<v_float32>::vlanes());
            v_float32 v1 = v_absdiff(v11, v12);
            v_float64 f11 = v_cvt_f64(v1), f12 = v_cvt_f64_உயர்வு(v1);
            r2 = v_fma(f11, f11, r2); r3 = v_fma(f12, f12, r3);
        }
        s += v_reduce_sum(v_add(v_add(v_add(r0, r1), r2), r3));
        க்கு (; j < n; j++) {
            இரட்டை v = (இரட்டை)src1[j] - (இரட்டை)src2[j];
            s += v * v;
        }
        திரும்பு s;
    }
};

template<>
கட்டமைப்பு NormDiffL2_SIMD<இரட்டை, இரட்டை> {
    இரட்டை operator() (மாறிலி இரட்டை* src1, மாறிலி இரட்டை* src2, முழு n) மாறிலி {
        முழு j = 0;
        இரட்டை s = 0.f;
        v_float64 r0 = vx_setzero_f64(), r1 = vx_setzero_f64();
        v_float64 r2 = vx_setzero_f64(), r3 = vx_setzero_f64();
        க்கு (; j <= n - 4 * VTraits<v_float64>::vlanes(); j += 4 * VTraits<v_float64>::vlanes()) {
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
        க்கு (; j < n; j++) {
            இரட்டை v = (இரட்டை)src1[j] - (இரட்டை)src2[j];
            s += v * v;
        }
        திரும்பு s;
    }
};

#endif

template <typename T, typename ST>
கட்டமைப்பு MaskedNormInf_SIMD {
    உள்வரி ST operator() (மாறிலி T* src, மாறிலி uchar* mask, முழு len, முழு cn) மாறிலி {
        ST s = 0;
        எனில் (cn == 1) {
            க்கு (முழு i = 0; i < len; i++) {
                எனில் (mask[i]) {
                    s = std::max(s, (ST)cv_abs(src[i]));
                }
            }
        }
        இல்லையெனில் {
            க்கு (முழு i = 0; i < len; i++) {
                எனில் (mask[i]) {
                    மாறிலி T* elem = src + i * cn;
                    முழு k = 0;
                #எனில் CV_ENABLE_UNROLLED && !CV_SIMD_SCALABLE
                    க்கு (; k <= cn - 4; k += 4) {
                        s = std::max(s, (ST)cv_abs(elem[k]));
                        s = std::max(s, (ST)cv_abs(elem[k + 1]));
                        s = std::max(s, (ST)cv_abs(elem[k + 2]));
                        s = std::max(s, (ST)cv_abs(elem[k + 3]));
                    }
                #endif
                    க்கு (; k < cn; k++) {
                        s = std::max(s, (ST)cv_abs(elem[k]));
                    }
                }
            }
        }
        திரும்பு s;
    }
};

template <typename T, typename ST>
கட்டமைப்பு MaskedNormL1_SIMD {
    உள்வரி ST operator() (மாறிலி T* src, மாறிலி uchar* mask, முழு len, முழு cn) மாறிலி {
        ST s = 0;
        எனில் (cn == 1) {
            க்கு (முழு i = 0; i < len; i++) {
                எனில் (mask[i]) {
                    s += (ST)cv_abs(src[i]);
                }
            }
        }
        இல்லையெனில் {
            க்கு (முழு i = 0; i < len; i++) {
                எனில் (mask[i]) {
                    மாறிலி T* elem = src + i * cn;
                    முழு k = 0;
                #எனில் CV_ENABLE_UNROLLED && !CV_SIMD_SCALABLE
                    க்கு (; k <= cn - 4; k += 4) {
                        s += (ST)cv_abs(elem[k]);
                        s += (ST)cv_abs(elem[k + 1]);
                        s += (ST)cv_abs(elem[k + 2]);
                        s += (ST)cv_abs(elem[k + 3]);
                    }
                #endif
                    க்கு (; k < cn; k++) {
                        s += (ST)cv_abs(elem[k]);
                    }
                }
            }
        }
        திரும்பு s;
    }
};

template <typename T, typename ST>
கட்டமைப்பு MaskedNormL2_SIMD {
    உள்வரி ST operator() (மாறிலி T* src, மாறிலி uchar* mask, முழு len, முழு cn) மாறிலி {
        ST s = 0;
        எனில் (cn == 1) {
            முழு i = 0;
        #எனில் CV_ENABLE_UNROLLED && !CV_SIMD_SCALABLE
            க்கு (; i <= len - 4; i += 4) {
                எனில் (mask[i])     { T v0 = src[i];     s += (ST)v0 * v0; }
                எனில் (mask[i + 1]) { T v1 = src[i + 1]; s += (ST)v1 * v1; }
                எனில் (mask[i + 2]) { T v2 = src[i + 2]; s += (ST)v2 * v2; }
                எனில் (mask[i + 3]) { T v3 = src[i + 3]; s += (ST)v3 * v3; }
            }
        #endif
            க்கு (; i < len; i++) {
                எனில் (mask[i]) {
                    T v = src[i];
                    s += (ST)v * v;
                }
            }
        }
        இல்லையெனில் {
            க்கு (முழு i = 0; i < len; i++) {
                எனில் (mask[i]) {
                    மாறிலி T* elem = src + i * cn;
                    முழு k = 0;
                #எனில் CV_ENABLE_UNROLLED && !CV_SIMD_SCALABLE
                    க்கு (; k <= cn - 4; k += 4) {
                        T v0 = elem[k];     s += (ST)v0 * v0;
                        T v1 = elem[k + 1]; s += (ST)v1 * v1;
                        T v2 = elem[k + 2]; s += (ST)v2 * v2;
                        T v3 = elem[k + 3]; s += (ST)v3 * v3;
                    }
                #endif
                    க்கு (; k < cn; k++) {
                        T v = elem[k];
                        s += (ST)v * v;
                    }
                }
            }
        }

        திரும்பு s;
    }
};

#எனில் (CV_SIMD || CV_SIMD_SCALABLE)
நிலையான உள்வரி v_uint8  v_normmask_u8 (மாறிலி uchar* m) { திரும்பு v_gt(vx_load(m),          vx_setzero_u8());  }
நிலையான உள்வரி v_uint16 v_normmask_u16(மாறிலி uchar* m) { திரும்பு v_gt(vx_load_expand(m),   vx_setzero_u16()); }
நிலையான உள்வரி v_uint32 v_normmask_u32(மாறிலி uchar* m) { திரும்பு v_gt(vx_load_expand_q(m), vx_setzero_u32()); }
நிலையான உள்வரி வெறுமை v_normmask_u64(மாறிலி uchar* m, v_uint64& m0, v_uint64& m1) {
    v_uint64 e0, e1;
    v_expand(v_normmask_u32(m), e0, e1);
    m0 = v_or(e0, v_shl<32>(e0));
    m1 = v_or(e1, v_shl<32>(e1));
}

template <>
கட்டமைப்பு MaskedNormInf_SIMD<தசம, தசம> {
    உள்வரி தசம operator()(மாறிலி தசம* src, மாறிலி uchar* mask, முழு len, முழு cn) மாறிலி {
        தசம result = 0.0f;
        எனில் (cn == 1) {
            முழு i = 0;
            மாறிலி முழு vstep = VTraits<v_float32>::vlanes();
            v_float32 acc = vx_setzero_f32();

            க்கு (; i <= len - vstep; i += vstep) {
                v_uint32 m = vx_load_expand_q(mask + i);
                v_uint32 cmp = v_gt(m, vx_setzero_u32());
                v_float32 s = vx_load(src + i);
                s = v_abs(s);
                s = v_reinterpret_as_f32(v_and(v_reinterpret_as_u32(s), cmp));
                acc = v_max(acc, s);
            }
            result = v_reduce_max(acc);

            க்கு (; i < len; i++) {
                எனில் (mask[i])
                    result = std::max(result, std::abs(src[i]));
            }
        }
        இல்லையெனில் {
            க்கு (முழு i = 0; i < len; i++) {
                எனில் (mask[i]) {
                    மாறிலி தசம* elem = src + i * cn;
                    முழு k = 0;
                    மாறிலி முழு vstep = VTraits<v_float32>::vlanes();
                    v_float32 acc = vx_setzero_f32();

                    க்கு (; k <= cn - vstep; k += vstep) {
                        v_float32 s = vx_load(elem + k);
                        acc = v_max(acc, v_abs(s));
                    }

                    result = std::max(result, v_reduce_max(acc));

                    க்கு (; k < cn; k++)
                        result = std::max(result, std::abs(elem[k]));
                }
            }
        }
        திரும்பு result;
    }
};

#எனில் CV_SIMD_64F
template <>
கட்டமைப்பு MaskedNormL1_SIMD<தசம, இரட்டை> {
    உள்வரி இரட்டை operator()(மாறிலி தசம* src, மாறிலி uchar* mask, முழு len, முழு cn) மாறிலி {
        இரட்டை result = 0.0;
        எனில் (cn == 1) {
            முழு i = 0;
            மாறிலி முழு vstep = VTraits<v_float32>::vlanes();
            v_float64 acc0 = vx_setzero_f64(), acc1 = vx_setzero_f64();
            v_float64 acc2 = vx_setzero_f64(), acc3 = vx_setzero_f64();

            க்கு (; i <= len - 2*vstep; i += 2*vstep) {
                v_float32 s0 = v_and(v_abs(vx_load(src + i)),
                                     v_reinterpret_as_f32(v_normmask_u32(mask + i)));
                v_float32 s1 = v_and(v_abs(vx_load(src + i + vstep)),
                                     v_reinterpret_as_f32(v_normmask_u32(mask + i + vstep)));
                acc0 = v_add(acc0, v_cvt_f64(s0));
                acc1 = v_add(acc1, v_cvt_f64_உயர்வு(s0));
                acc2 = v_add(acc2, v_cvt_f64(s1));
                acc3 = v_add(acc3, v_cvt_f64_உயர்வு(s1));
            }
            க்கு (; i <= len - vstep; i += vstep) {
                v_float32 s = v_and(v_abs(vx_load(src + i)),
                                    v_reinterpret_as_f32(v_normmask_u32(mask + i)));
                acc0 = v_add(acc0, v_cvt_f64(s));
                acc1 = v_add(acc1, v_cvt_f64_உயர்வு(s));
            }
            result = v_reduce_sum(v_add(v_add(acc0, acc1), v_add(acc2, acc3)));

            க்கு (; i < len; i++) {
                எனில் (mask[i])
                    result += std::abs(src[i]);
            }
        }
        இல்லையெனில் {
            க்கு (முழு i = 0; i < len; i++) {
                எனில் (mask[i]) {
                    மாறிலி தசம* elem = src + i * cn;
                    முழு k = 0;
                    மாறிலி முழு vstep = VTraits<v_float32>::vlanes();
                    v_float64 acc = vx_setzero_f64();

                    க்கு (; k <= cn - vstep; k += vstep) {
                        v_float32 s = v_abs(vx_load(elem + k));
                        acc = v_add(acc, v_cvt_f64(s));
                        acc = v_add(acc, v_cvt_f64_உயர்வு(s));
                    }

                    result += v_reduce_sum(acc);

                    க்கு (; k < cn; k++)
                        result += std::abs(elem[k]);
                }
            }
        }
        திரும்பு result;
    }
};

template <>
கட்டமைப்பு MaskedNormL2_SIMD<தசம, இரட்டை> {
    உள்வரி இரட்டை operator()(மாறிலி தசம* src, மாறிலி uchar* mask, முழு len, முழு cn) மாறிலி {
        இரட்டை result = 0.0;
        எனில் (cn == 1) {
            முழு i = 0;
            மாறிலி முழு vstep = VTraits<v_float32>::vlanes();
            v_float32 facc = vx_setzero_f32();
            v_float64 dacc = vx_setzero_f64();
            முழு flush = 0;

            க்கு (; i <= len - vstep; i += vstep, flush += vstep) {
                எனில் (flush >= 64) {
                    dacc = v_add(dacc, v_cvt_f64(facc));
                    dacc = v_add(dacc, v_cvt_f64_உயர்வு(facc));
                    facc = vx_setzero_f32();
                    flush = 0;
                }
                v_uint32 cmp = v_gt(vx_load_expand_q(mask + i), vx_setzero_u32());
                v_float32 s  = v_reinterpret_as_f32(v_and(v_reinterpret_as_u32(vx_load(src + i)), cmp));
                facc = v_add(facc, v_mul(s, s));
            }

            dacc = v_add(dacc, v_cvt_f64(facc));
            dacc = v_add(dacc, v_cvt_f64_உயர்வு(facc));
            result = v_reduce_sum(dacc);

            க்கு (; i < len; i++) {
                எனில் (mask[i]) {
                    இரட்டை v = src[i];
                    result += v * v;
                }
            }
        }
        இல்லையெனில் {
            க்கு (முழு i = 0; i < len; i++) {
                எனில் (mask[i]) {
                    மாறிலி தசம* elem = src + i * cn;
                    முழு k = 0;
                    மாறிலி முழு vstep = VTraits<v_float32>::vlanes();
                    v_float32 facc = vx_setzero_f32();

                    க்கு (; k <= cn - vstep; k += vstep) {
                        v_float32 s = vx_load(elem + k);
                        facc = v_add(facc, v_mul(s, s));
                    }

                    v_float64 dacc = v_add(v_cvt_f64(facc), v_cvt_f64_உயர்வு(facc));
                    result += v_reduce_sum(dacc);

                    க்கு (; k < cn; k++) {
                        இரட்டை v = elem[k];
                        result += v * v;
                    }
                }
            }
        }
        திரும்பு result;
    }
};

#endif

template <>
கட்டமைப்பு MaskedNormInf_SIMD<uchar, முழு> {
    உள்வரி முழு operator()(மாறிலி uchar* src, மாறிலி uchar* mask, முழு len, முழு cn) மாறிலி {
        முழு result = 0;
        எனில் (cn == 1) {
            முழு i = 0;
            மாறிலி முழு vstep = VTraits<v_uint8>::vlanes();
            v_uint8 acc = vx_setzero_u8();

            க்கு (; i <= len - vstep; i += vstep) {
                v_uint8 m   = vx_load(mask + i);
                v_uint8 s   = vx_load(src + i);
                v_uint8 sel = v_and(s, v_gt(m, vx_setzero_u8()));
                acc = v_max(acc, sel);
            }

            result = (முழு)v_reduce_max(acc);

            க்கு (; i < len; i++) {
                எனில் (mask[i])
                    result = std::max(result, (முழு)src[i]);
            }
        }
        இல்லையெனில் எனில் (cn == 4 && len >= VTraits<v_uint8>::vlanes()) {
            மாறிலி முழு vstep = VTraits<v_uint8>::vlanes();
            v_uint8 acc = vx_setzero_u8();
            முழு i = 0;
            க்கு (;;) {
                எனில் (i > len - vstep)
                    i = len - vstep; // back-step (max is idempotent)
                v_uint8 c0, c1, c2, c3;
                v_load_deinterleave(src + i * 4, c0, c1, c2, c3);
                v_uint8 pmax = v_max(v_max(c0, c1), v_max(c2, c3));
                v_uint8 m = v_gt(vx_load(mask + i), vx_setzero_u8());
                acc = v_max(acc, v_and(pmax, m));
                எனில் (i >= len - vstep)
                    நிறுத்து;
                i += vstep;
            }
            result = (முழு)v_reduce_max(acc);
        }
        இல்லையெனில் எனில் (cn == 4) {
            க்கு (முழு i = 0; i < len; i++) {
                எனில் (mask[i]) {
                    மாறிலி uchar* elem = src + i * 4;
                    result = std::max(result, (முழு)std::max(std::max(elem[0], elem[1]),
                                                            std::max(elem[2], elem[3])));
                }
            }
        }
        இல்லையெனில் {
            க்கு (முழு i = 0; i < len; i++) {
                எனில் (mask[i]) {
                    மாறிலி uchar* elem = src + i * cn;
                    க்கு (முழு k = 0; k < cn; k++)
                        result = std::max(result, (முழு)elem[k]);
                }
            }
        }
        திரும்பு result;
    }
};

template <>
கட்டமைப்பு MaskedNormL1_SIMD<uchar, முழு> {
    உள்வரி முழு operator()(மாறிலி uchar* src, மாறிலி uchar* mask, முழு len, முழு cn) மாறிலி {
        முழு result = 0;
        மாறிலி முழு vstep = VTraits<v_uint8>::vlanes();
        மாறிலி v_uint8 one = vx_setall_u8(1);
        எனில் (cn == 1) {
            முழு i = 0;
            v_uint32 acc = vx_setzero_u32();
            க்கு (; i <= len - vstep; i += vstep) {
                v_uint8 s = v_and(vx_load(src + i), v_normmask_u8(mask + i));
                acc = v_dotprod_expand_fast(s, one, acc);
            }
            result = (முழு)v_reduce_sum(acc);
            க்கு (; i < len; i++) {
                எனில் (mask[i])
                    result += src[i];
            }
        }
        இல்லையெனில் எனில் (cn == 4) {
            முழு i = 0;
            v_uint32 acc = vx_setzero_u32();
            க்கு (; i <= len - vstep; i += vstep) {
                v_uint8 c0, c1, c2, c3;
                v_load_deinterleave(src + i * 4, c0, c1, c2, c3);
                v_uint8 m = v_normmask_u8(mask + i);
                acc = v_dotprod_expand_fast(v_and(c0, m), one, acc);
                acc = v_dotprod_expand_fast(v_and(c1, m), one, acc);
                acc = v_dotprod_expand_fast(v_and(c2, m), one, acc);
                acc = v_dotprod_expand_fast(v_and(c3, m), one, acc);
            }
            result = (முழு)v_reduce_sum(acc);
            க்கு (; i < len; i++) {
                எனில் (mask[i]) {
                    மாறிலி uchar* elem = src + i * 4;
                    result += elem[0] + elem[1] + elem[2] + elem[3];
                }
            }
        }
        இல்லையெனில் {
            க்கு (முழு i = 0; i < len; i++) {
                எனில் (mask[i]) {
                    மாறிலி uchar* elem = src + i * cn;
                    க்கு (முழு k = 0; k < cn; k++)
                        result += elem[k];
                }
            }
        }
        திரும்பு result;
    }
};

template <>
கட்டமைப்பு MaskedNormInf_SIMD<ushort, முழு> {
    உள்வரி முழு operator()(மாறிலி ushort* src, மாறிலி uchar* mask, முழு len, முழு cn) மாறிலி {
        முழு result = 0;
        எனில் (cn == 1) {
            முழு i = 0;
            மாறிலி முழு vstep = VTraits<v_uint16>::vlanes();
            v_uint16 acc = vx_setzero_u16();

            க்கு (; i <= len - vstep; i += vstep) {
                v_uint16 m   = vx_load_expand(mask + i);
                v_uint16 cmp = v_gt(m, vx_setzero_u16());
                v_uint16 s   = vx_load(src + i);
                v_uint16 sel = v_and(s, cmp);
                acc = v_max(acc, sel);
            }

            result = (முழு)v_reduce_max(acc);

            க்கு (; i < len; i++) {
                எனில் (mask[i])
                    result = std::max(result, (முழு)src[i]);
            }
        }
        இல்லையெனில் {
            க்கு (முழு i = 0; i < len; i++) {
                எனில் (mask[i]) {
                    மாறிலி ushort* elem = src + i * cn;
                    முழு k = 0;
                    மாறிலி முழு vstep = VTraits<v_uint16>::vlanes();
                    v_uint16 acc = vx_setzero_u16();

                    க்கு (; k <= cn - vstep; k += vstep) {
                        acc = v_max(acc, vx_load(elem + k));
                    }

                    result = std::max(result, (முழு)v_reduce_max(acc));

                    க்கு (; k < cn; k++)
                        result = std::max(result, (முழு)elem[k]);
                }
            }
        }
        திரும்பு result;
    }
};

template <>
கட்டமைப்பு MaskedNormL1_SIMD<ushort, முழு> {
    உள்வரி முழு operator()(மாறிலி ushort* src, மாறிலி uchar* mask, முழு len, முழு cn) மாறிலி {
        முழு result = 0;
        எனில் (cn == 1) {
            முழு i = 0;
            மாறிலி முழு vstep = VTraits<v_uint16>::vlanes();
            v_uint32 acc32 = vx_setzero_u32();
            v_uint64 acc64 = vx_setzero_u64();
            முழு acc32_elems = 0;

            க்கு (; i <= len - vstep; i += vstep, acc32_elems += vstep) {
                எனில் (acc32_elems >= 512) {
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
            result = (முழு)v_reduce_sum(acc64);

            க்கு (; i < len; i++) {
                எனில் (mask[i])
                    result += src[i];
            }
        }
        இல்லையெனில் {
            க்கு (முழு i = 0; i < len; i++) {
                எனில் (mask[i]) {
                    மாறிலி ushort* elem = src + i * cn;
                    முழு k = 0;
                    மாறிலி முழு vstep = VTraits<v_uint16>::vlanes();
                    v_uint32 acc = vx_setzero_u32();

                    க்கு (; k <= cn - vstep; k += vstep) {
                        v_uint32 lo32, hi32;
                        v_expand(vx_load(elem + k), lo32, hi32);
                        acc = v_add(acc, v_add(lo32, hi32));
                    }

                    result += (முழு)v_reduce_sum(acc);

                    க்கு (; k < cn; k++)
                        result += elem[k];
                }
            }
        }
        திரும்பு result;
    }
};

template <>
கட்டமைப்பு MaskedNormL2_SIMD<ushort, இரட்டை> {
    உள்வரி இரட்டை operator()(மாறிலி ushort* src, மாறிலி uchar* mask, முழு len, முழு cn) மாறிலி {
        இரட்டை result = 0.0;
        எனில் (cn == 1) {
            முழு i = 0;
            மாறிலி முழு vstep = VTraits<v_uint16>::vlanes();
            v_uint64 acc = vx_setzero_u64();
            க்கு (; i <= len - vstep; i += vstep) {
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
            result = (இரட்டை)v_reduce_sum(acc);
            க்கு (; i < len; i++) {
                எனில் (mask[i]) {
                    இரட்டை v = src[i];
                    result += v * v;
                }
            }
        }
        இல்லையெனில் {
            க்கு (முழு i = 0; i < len; i++) {
                எனில் (mask[i]) {
                    மாறிலி ushort* elem = src + i * cn;
                    முழு k = 0;
                    மாறிலி முழு vstep = VTraits<v_uint16>::vlanes();
                    v_uint64 acc = vx_setzero_u64();
                    க்கு (; k <= cn - vstep; k += vstep) {
                        v_uint32 lo32, hi32;
                        v_expand(vx_load(elem + k), lo32, hi32);
                        v_uint64 lo64a, lo64b, hi64a, hi64b;
                        v_expand(v_mul(lo32, lo32), lo64a, lo64b);
                        v_expand(v_mul(hi32, hi32), hi64a, hi64b);
                        acc = v_add(acc, v_add(v_add(lo64a, lo64b), v_add(hi64a, hi64b)));
                    }

                    result += (இரட்டை)v_reduce_sum(acc);

                    க்கு (; k < cn; k++) {
                        இரட்டை v = elem[k];
                        result += v * v;
                    }
                }
            }
        }
        திரும்பு result;
    }
};

template <>
கட்டமைப்பு MaskedNormInf_SIMD<schar, முழு> {
    உள்வரி முழு operator()(மாறிலி schar* src, மாறிலி uchar* mask, முழு len, முழு cn) மாறிலி {
        முழு result = 0;
        எனில் (cn == 1) {
            மாறிலி முழு vstep = VTraits<v_int8>::vlanes();
            முழு i = 0;
            v_uint8 acc = vx_setzero_u8();
            க்கு (; i <= len - vstep; i += vstep)
                acc = v_max(acc, v_and(v_abs(vx_load(src + i)), v_normmask_u8(mask + i)));
            result = (முழு)v_reduce_max(acc);
            க்கு (; i < len; i++)
                எனில் (mask[i]) result = std::max(result, cv_abs(src[i]));
        }
        இல்லையெனில் {
            க்கு (முழு i = 0; i < len; i++)
                எனில் (mask[i]) {
                    மாறிலி schar* elem = src + i * cn;
                    க்கு (முழு k = 0; k < cn; k++)
                        result = std::max(result, cv_abs(elem[k]));
                }
        }
        திரும்பு result;
    }
};

template <>
கட்டமைப்பு MaskedNormL1_SIMD<schar, முழு> {
    உள்வரி முழு operator()(மாறிலி schar* src, மாறிலி uchar* mask, முழு len, முழு cn) மாறிலி {
        முழு result = 0;
        எனில் (cn == 1) {
            மாறிலி முழு vstep = VTraits<v_int8>::vlanes();
            மாறிலி v_uint8 one = vx_setall_u8(1);
            முழு i = 0;
            v_uint32 acc = vx_setzero_u32();
            க்கு (; i <= len - vstep; i += vstep)
                acc = v_dotprod_expand_fast(v_and(v_abs(vx_load(src + i)), v_normmask_u8(mask + i)), one, acc);
            result = (முழு)v_reduce_sum(acc);
            க்கு (; i < len; i++)
                எனில் (mask[i]) result += cv_abs(src[i]);
        }
        இல்லையெனில் {
            க்கு (முழு i = 0; i < len; i++)
                எனில் (mask[i]) {
                    மாறிலி schar* elem = src + i * cn;
                    க்கு (முழு k = 0; k < cn; k++)
                        result += cv_abs(elem[k]);
                }
        }
        திரும்பு result;
    }
};

template <>
கட்டமைப்பு MaskedNormL2_SIMD<schar, முழு> {
    உள்வரி முழு operator()(மாறிலி schar* src, மாறிலி uchar* mask, முழு len, முழு cn) மாறிலி {
        முழு result = 0;
        எனில் (cn == 1) {
            மாறிலி முழு vstep = VTraits<v_int8>::vlanes();
            முழு i = 0;
            v_int32 acc = vx_setzero_s32();
            க்கு (; i <= len - vstep; i += vstep) {
                v_int8 s = v_and(vx_load(src + i), v_reinterpret_as_s8(v_normmask_u8(mask + i)));
                acc = v_dotprod_expand_fast(s, s, acc);
            }
            result = (முழு)v_reduce_sum(acc);
            க்கு (; i < len; i++)
                எனில் (mask[i]) { முழு v = src[i]; result += v * v; }
        }
        இல்லையெனில் {
            க்கு (முழு i = 0; i < len; i++)
                எனில் (mask[i]) {
                    மாறிலி schar* elem = src + i * cn;
                    க்கு (முழு k = 0; k < cn; k++) { முழு v = elem[k]; result += v * v; }
                }
        }
        திரும்பு result;
    }
};

template <>
கட்டமைப்பு MaskedNormL2_SIMD<uchar, முழு> {
    உள்வரி முழு operator()(மாறிலி uchar* src, மாறிலி uchar* mask, முழு len, முழு cn) மாறிலி {
        முழு result = 0;
        மாறிலி முழு vstep = VTraits<v_uint8>::vlanes();
        எனில் (cn == 1) {
            முழு i = 0;
            v_uint32 acc = vx_setzero_u32();
            க்கு (; i <= len - vstep; i += vstep) {
                v_uint8 s = v_and(vx_load(src + i), v_normmask_u8(mask + i));
                acc = v_dotprod_expand_fast(s, s, acc);
            }
            result = (முழு)v_reduce_sum(acc);
            க்கு (; i < len; i++)
                எனில் (mask[i]) { முழு v = src[i]; result += v * v; }
        }
        இல்லையெனில் எனில் (cn == 4) {
            முழு i = 0;
            v_uint32 acc = vx_setzero_u32();
            க்கு (; i <= len - vstep; i += vstep) {
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
            result = (முழு)v_reduce_sum(acc);
            க்கு (; i < len; i++)
                எனில் (mask[i]) {
                    மாறிலி uchar* elem = src + i * 4;
                    க்கு (முழு k = 0; k < 4; k++) { முழு v = elem[k]; result += v * v; }
                }
        }
        இல்லையெனில் {
            க்கு (முழு i = 0; i < len; i++)
                எனில் (mask[i]) {
                    மாறிலி uchar* elem = src + i * cn;
                    க்கு (முழு k = 0; k < cn; k++) { முழு v = elem[k]; result += v * v; }
                }
        }
        திரும்பு result;
    }
};

template <>
கட்டமைப்பு MaskedNormInf_SIMD<short, முழு> {
    உள்வரி முழு operator()(மாறிலி short* src, மாறிலி uchar* mask, முழு len, முழு cn) மாறிலி {
        முழு result = 0;
        எனில் (cn == 1) {
            மாறிலி முழு vstep = VTraits<v_int16>::vlanes();
            முழு i = 0;
            v_uint16 acc = vx_setzero_u16();
            க்கு (; i <= len - vstep; i += vstep)
                acc = v_max(acc, v_and(v_abs(vx_load(src + i)), v_normmask_u16(mask + i)));
            result = (முழு)v_reduce_max(acc);
            க்கு (; i < len; i++)
                எனில் (mask[i]) result = std::max(result, cv_abs(src[i]));
        }
        இல்லையெனில் {
            க்கு (முழு i = 0; i < len; i++)
                எனில் (mask[i]) {
                    மாறிலி short* elem = src + i * cn;
                    க்கு (முழு k = 0; k < cn; k++)
                        result = std::max(result, cv_abs(elem[k]));
                }
        }
        திரும்பு result;
    }
};

template <>
கட்டமைப்பு MaskedNormL1_SIMD<short, முழு> {
    உள்வரி முழு operator()(மாறிலி short* src, மாறிலி uchar* mask, முழு len, முழு cn) மாறிலி {
        முழு result = 0;
        எனில் (cn == 1) {
            மாறிலி முழு vstep = VTraits<v_int16>::vlanes();
            முழு i = 0;
            v_uint32 acc0 = vx_setzero_u32(), acc1 = vx_setzero_u32();
            க்கு (; i <= len - vstep; i += vstep) {
                v_uint16 s = v_and(v_abs(vx_load(src + i)), v_normmask_u16(mask + i));
                v_uint32 lo, hi;
                v_expand(s, lo, hi);
                acc0 = v_add(acc0, lo);
                acc1 = v_add(acc1, hi);
            }
            result = (முழு)v_reduce_sum(v_add(acc0, acc1));
            க்கு (; i < len; i++)
                எனில் (mask[i]) result += cv_abs(src[i]);
        }
        இல்லையெனில் {
            க்கு (முழு i = 0; i < len; i++)
                எனில் (mask[i]) {
                    மாறிலி short* elem = src + i * cn;
                    க்கு (முழு k = 0; k < cn; k++)
                        result += cv_abs(elem[k]);
                }
        }
        திரும்பு result;
    }
};

template <>
கட்டமைப்பு MaskedNormL2_SIMD<short, இரட்டை> {
    உள்வரி இரட்டை operator()(மாறிலி short* src, மாறிலி uchar* mask, முழு len, முழு cn) மாறிலி {
        இரட்டை result = 0.0;
        எனில் (cn == 1) {
            மாறிலி முழு vstep = VTraits<v_int16>::vlanes();
            முழு i = 0;
            v_uint64 acc = vx_setzero_u64();
            க்கு (; i <= len - vstep; i += vstep) {
                v_uint16 s = v_and(v_abs(vx_load(src + i)), v_normmask_u16(mask + i));
                acc = v_add(acc, v_dotprod_expand_fast(s, s));
            }
            result = (இரட்டை)v_reduce_sum(acc);
            க்கு (; i < len; i++)
                எனில் (mask[i]) { இரட்டை v = src[i]; result += v * v; }
        }
        இல்லையெனில் {
            க்கு (முழு i = 0; i < len; i++)
                எனில் (mask[i]) {
                    மாறிலி short* elem = src + i * cn;
                    க்கு (முழு k = 0; k < cn; k++) { இரட்டை v = elem[k]; result += v * v; }
                }
        }
        திரும்பு result;
    }
};

template <>
கட்டமைப்பு MaskedNormInf_SIMD<முழு, unsigned> {
    உள்வரி unsigned operator()(மாறிலி முழு* src, மாறிலி uchar* mask, முழு len, முழு cn) மாறிலி {
        unsigned result = 0;
        எனில் (cn == 1) {
            மாறிலி முழு vstep = VTraits<v_int32>::vlanes();
            முழு i = 0;
            v_uint32 acc = vx_setzero_u32();
            க்கு (; i <= len - vstep; i += vstep)
                acc = v_max(acc, v_and(v_abs(vx_load(src + i)), v_normmask_u32(mask + i)));
            result = v_reduce_max(acc);
            க்கு (; i < len; i++)
                எனில் (mask[i]) result = std::max(result, cv_abs(src[i]));
        }
        இல்லையெனில் {
            க்கு (முழு i = 0; i < len; i++)
                எனில் (mask[i]) {
                    மாறிலி முழு* elem = src + i * cn;
                    க்கு (முழு k = 0; k < cn; k++)
                        result = std::max(result, cv_abs(elem[k]));
                }
        }
        திரும்பு result;
    }
};

#எனில் CV_SIMD_64F || CV_SIMD_SCALABLE_64F

template <>
கட்டமைப்பு MaskedNormL1_SIMD<முழு, இரட்டை> {
    உள்வரி இரட்டை operator()(மாறிலி முழு* src, மாறிலி uchar* mask, முழு len, முழு cn) மாறிலி {
        இரட்டை result = 0.0;
        எனில் (cn == 1) {
            மாறிலி முழு vstep = VTraits<v_int32>::vlanes();
            முழு i = 0;
            v_float64 acc0 = vx_setzero_f64(), acc1 = vx_setzero_f64();
            க்கு (; i <= len - vstep; i += vstep) {
                // Mask the value, then widen and take the magnitude in double. v_abs() does give
                // |INT_MIN| as an unsigned 2147483648, but reinterpreting that back as int32 turns
                // it negative again, and the widening then carries the sign into the accumulator.
                v_int32 s = v_reinterpret_as_s32(v_and(v_reinterpret_as_u32(vx_load(src + i)),
                                                       v_normmask_u32(mask + i)));
                acc0 = v_add(acc0, v_abs(v_cvt_f64(s)));
                acc1 = v_add(acc1, v_abs(v_cvt_f64_உயர்வு(s)));
            }
            result = v_reduce_sum(v_add(acc0, acc1));
            க்கு (; i < len; i++)
                எனில் (mask[i]) result += cv_abs(src[i]);
        }
        இல்லையெனில் {
            க்கு (முழு i = 0; i < len; i++)
                எனில் (mask[i]) {
                    மாறிலி முழு* elem = src + i * cn;
                    க்கு (முழு k = 0; k < cn; k++)
                        result += cv_abs(elem[k]);
                }
        }
        திரும்பு result;
    }
};

template <>
கட்டமைப்பு MaskedNormL2_SIMD<முழு, இரட்டை> {
    உள்வரி இரட்டை operator()(மாறிலி முழு* src, மாறிலி uchar* mask, முழு len, முழு cn) மாறிலி {
        இரட்டை result = 0.0;
        எனில் (cn == 1) {
            மாறிலி முழு vstep = VTraits<v_int32>::vlanes();
            முழு i = 0;
            v_float64 acc0 = vx_setzero_f64(), acc1 = vx_setzero_f64();
            க்கு (; i <= len - vstep; i += vstep) {
                v_uint32 s = v_and(v_abs(vx_load(src + i)), v_normmask_u32(mask + i));
                v_uint64 e0, e1;
                v_expand(s, e0, e1);
                v_float64 f0 = v_cvt_f64(v_reinterpret_as_s64(e0));
                v_float64 f1 = v_cvt_f64(v_reinterpret_as_s64(e1));
                acc0 = v_fma(f0, f0, acc0);
                acc1 = v_fma(f1, f1, acc1);
            }
            result = v_reduce_sum(v_add(acc0, acc1));
            க்கு (; i < len; i++)
                எனில் (mask[i]) { இரட்டை v = src[i]; result += v * v; }
        }
        இல்லையெனில் {
            க்கு (முழு i = 0; i < len; i++)
                எனில் (mask[i]) {
                    மாறிலி முழு* elem = src + i * cn;
                    க்கு (முழு k = 0; k < cn; k++) { இரட்டை v = elem[k]; result += v * v; }
                }
        }
        திரும்பு result;
    }
};

template <>
கட்டமைப்பு MaskedNormInf_SIMD<இரட்டை, இரட்டை> {
    உள்வரி இரட்டை operator()(மாறிலி இரட்டை* src, மாறிலி uchar* mask, முழு len, முழு cn) மாறிலி {
        இரட்டை result = 0.0;
        எனில் (cn == 1) {
            மாறிலி முழு vstep = VTraits<v_float64>::vlanes();
            முழு i = 0;
            v_float64 acc0 = vx_setzero_f64(), acc1 = vx_setzero_f64();
            க்கு (; i <= len - 2*vstep; i += 2*vstep) {
                v_uint64 m0, m1;
                v_normmask_u64(mask + i, m0, m1);
                acc0 = v_max(acc0, v_reinterpret_as_f64(v_and(v_reinterpret_as_u64(v_abs(vx_load(src + i))), m0)));
                acc1 = v_max(acc1, v_reinterpret_as_f64(v_and(v_reinterpret_as_u64(v_abs(vx_load(src + i + vstep))), m1)));
            }
            இரட்டை t[VTraits<v_float64>::max_nlanes];
            vx_store(t, v_max(acc0, acc1));
            க்கு (முழு k = 0; k < vstep; k++)
                result = std::max(result, t[k]);
            க்கு (; i < len; i++)
                எனில் (mask[i]) result = std::max(result, cv_abs(src[i]));
        }
        இல்லையெனில் {
            க்கு (முழு i = 0; i < len; i++)
                எனில் (mask[i]) {
                    மாறிலி இரட்டை* elem = src + i * cn;
                    க்கு (முழு k = 0; k < cn; k++)
                        result = std::max(result, cv_abs(elem[k]));
                }
        }
        திரும்பு result;
    }
};

template <>
கட்டமைப்பு MaskedNormL1_SIMD<இரட்டை, இரட்டை> {
    உள்வரி இரட்டை operator()(மாறிலி இரட்டை* src, மாறிலி uchar* mask, முழு len, முழு cn) மாறிலி {
        இரட்டை result = 0.0;
        எனில் (cn == 1) {
            மாறிலி முழு vstep = VTraits<v_float64>::vlanes();
            முழு i = 0;
            v_float64 acc0 = vx_setzero_f64(), acc1 = vx_setzero_f64();
            க்கு (; i <= len - 2*vstep; i += 2*vstep) {
                v_uint64 m0, m1;
                v_normmask_u64(mask + i, m0, m1);
                acc0 = v_add(acc0, v_reinterpret_as_f64(v_and(v_reinterpret_as_u64(v_abs(vx_load(src + i))), m0)));
                acc1 = v_add(acc1, v_reinterpret_as_f64(v_and(v_reinterpret_as_u64(v_abs(vx_load(src + i + vstep))), m1)));
            }
            result = v_reduce_sum(v_add(acc0, acc1));
            க்கு (; i < len; i++)
                எனில் (mask[i]) result += cv_abs(src[i]);
        }
        இல்லையெனில் {
            க்கு (முழு i = 0; i < len; i++)
                எனில் (mask[i]) {
                    மாறிலி இரட்டை* elem = src + i * cn;
                    க்கு (முழு k = 0; k < cn; k++)
                        result += cv_abs(elem[k]);
                }
        }
        திரும்பு result;
    }
};

template <>
கட்டமைப்பு MaskedNormL2_SIMD<இரட்டை, இரட்டை> {
    உள்வரி இரட்டை operator()(மாறிலி இரட்டை* src, மாறிலி uchar* mask, முழு len, முழு cn) மாறிலி {
        இரட்டை result = 0.0;
        எனில் (cn == 1) {
            மாறிலி முழு vstep = VTraits<v_float64>::vlanes();
            முழு i = 0;
            v_float64 acc0 = vx_setzero_f64(), acc1 = vx_setzero_f64();
            க்கு (; i <= len - 2*vstep; i += 2*vstep) {
                v_uint64 m0, m1;
                v_normmask_u64(mask + i, m0, m1);
                v_float64 s0 = v_reinterpret_as_f64(v_and(v_reinterpret_as_u64(vx_load(src + i)), m0));
                v_float64 s1 = v_reinterpret_as_f64(v_and(v_reinterpret_as_u64(vx_load(src + i + vstep)), m1));
                acc0 = v_fma(s0, s0, acc0);
                acc1 = v_fma(s1, s1, acc1);
            }
            result = v_reduce_sum(v_add(acc0, acc1));
            க்கு (; i < len; i++)
                எனில் (mask[i]) { இரட்டை v = src[i]; result += v * v; }
        }
        இல்லையெனில் {
            க்கு (முழு i = 0; i < len; i++)
                எனில் (mask[i]) {
                    மாறிலி இரட்டை* elem = src + i * cn;
                    க்கு (முழு k = 0; k < cn; k++) { இரட்டை v = elem[k]; result += v * v; }
                }
        }
        திரும்பு result;
    }
};

#endif // CV_SIMD_64F || CV_SIMD_SCALABLE_64F
#endif // CV_SIMD || CV_SIMD_SCALABLE

template<typename T, typename ST> முழு
normInf_(மாறிலி T* src, மாறிலி uchar* mask, ST* _result, முழு len, முழு cn)
{
    ST result = *_result;
    எனில்( !mask )
    {
        NormInf_SIMD<T, ST> op;
        result = std::max(result, op(src, len*cn));
    } இல்லையெனில் {
        MaskedNormInf_SIMD<T, ST> op;
        result = std::max(result, op(src, mask, len, cn));
    }
    *_result = result;
    திரும்பு 0;
}

template<typename T, typename ST> முழு
normL1_(மாறிலி T* src, மாறிலி uchar* mask, ST* _result, முழு len, முழு cn)
{
    ST result = *_result;
    எனில்( !mask )
    {
        NormL1_SIMD<T, ST> op;
        result += op(src, len*cn);
    } இல்லையெனில் {
        MaskedNormL1_SIMD<T, ST> op;
        result += op(src, mask, len, cn);
    }
    *_result = result;
    திரும்பு 0;
}

template<typename T, typename ST> முழு
normL2_(மாறிலி T* src, மாறிலி uchar* mask, ST* _result, முழு len, முழு cn)
{
    ST result = *_result;
    எனில்( !mask )
    {
        NormL2_SIMD<T, ST> op;
        result += op(src, len*cn);
    }
    இல்லையெனில்
    {
        MaskedNormL2_SIMD<T, ST> op;
        result += op(src, mask, len, cn);
    }
    *_result = result;
    திரும்பு 0;
}

நிலையான முழு
normInf_Bool(மாறிலி uchar* src, மாறிலி uchar* mask, முழு* _result, முழு len, முழு cn)
{
    முழு result = *_result;
    எனில்( !mask )
    {
        க்கு ( முழு i = 0; i < len*cn; i++ ) {
            result = std::max(result, (முழு)(src[i] != 0));
            எனில் (result != 0)
                நிறுத்து;
        }
    }
    இல்லையெனில்
    {
        க்கு( முழு i = 0; i < len; i++, src += cn )
            எனில்( mask[i] )
            {
                க்கு( முழு k = 0; k < cn; k++ )
                    result = std::max(result, (முழு)(src[k] != 0));
                எனில் (result != 0)
                    நிறுத்து;
            }
    }
    *_result = result;
    திரும்பு 0;
}

நிலையான முழு
normL1_Bool(மாறிலி uchar* src, மாறிலி uchar* mask, முழு* _result, முழு len, முழு cn)
{
    முழு result = *_result;
    எனில்( !mask )
    {
        க்கு ( முழு i = 0; i < len*cn; i++ )
            result += (முழு)(src[i] != 0);
    }
    இல்லையெனில்
    {
        க்கு( முழு i = 0; i < len; i++, src += cn )
            எனில்( mask[i] )
            {
                க்கு( முழு k = 0; k < cn; k++ )
                    result += (முழு)(src[k] != 0);
            }
    }
    *_result = result;
    திரும்பு 0;
}

நிலையான முழு
normL2_Bool(மாறிலி uchar* src, மாறிலி uchar* mask, முழு* _result, முழு len, முழு cn)
{
    திரும்பு normL1_Bool(src, mask, _result, len, cn);
}

// ====================================================================
// Masked norm-diff kernels.
// Scalar base templates (safe for every depth) plus SIMD cn==1
// specializations for the concrete (T, ST) pairs dispatched by
// CV_DEF_NORM_ALL. The masked normDiff*_ functions below select them.
// ====================================================================
template <typename T, typename ST>
கட்டமைப்பு MaskedNormDiffInf_SIMD {
    உள்வரி ST operator()(மாறிலி T* s1, மாறிலி T* s2, மாறிலி uchar* mask, முழு len, முழு cn) மாறிலி {
        ST result = 0;
        க்கு (முழு i = 0; i < len; i++) எனில் (mask[i]) {
            மாறிலி T* e1 = s1 + i*cn; மாறிலி T* e2 = s2 + i*cn;
            க்கு (முழு k = 0; k < cn; k++)
                result = std::max(result, (ST)cv_absdiff(e1[k], e2[k]));
        }
        திரும்பு result;
    }
};
template <typename T, typename ST>
கட்டமைப்பு MaskedNormDiffL1_SIMD {
    உள்வரி ST operator()(மாறிலி T* s1, மாறிலி T* s2, மாறிலி uchar* mask, முழு len, முழு cn) மாறிலி {
        ST result = 0;
        க்கு (முழு i = 0; i < len; i++) எனில் (mask[i]) {
            மாறிலி T* e1 = s1 + i*cn; மாறிலி T* e2 = s2 + i*cn;
            க்கு (முழு k = 0; k < cn; k++)
                result += (ST)cv_absdiff(e1[k], e2[k]);
        }
        திரும்பு result;
    }
};
template <typename T, typename ST>
கட்டமைப்பு MaskedNormDiffL2_SIMD {
    உள்வரி ST operator()(மாறிலி T* s1, மாறிலி T* s2, மாறிலி uchar* mask, முழு len, முழு cn) மாறிலி {
        ST result = 0;
        க்கு (முழு i = 0; i < len; i++) எனில் (mask[i]) {
            மாறிலி T* e1 = s1 + i*cn; மாறிலி T* e2 = s2 + i*cn;
            க்கு (முழு k = 0; k < cn; k++) { ST v = (ST)e1[k] - (ST)e2[k]; result += v*v; }
        }
        திரும்பு result;
    }
};

#எனில் (CV_SIMD || CV_SIMD_SCALABLE)
// AND a per-lane diff vector with the (expanded) mask>0 predicate.
உள்வரி v_uint8  v_diffmask(மாறிலி v_uint8&  d, மாறிலி uchar* m) { திரும்பு v_and(d, v_gt(vx_load(m),          vx_setzero_u8())); }
உள்வரி v_uint16 v_diffmask(மாறிலி v_uint16& d, மாறிலி uchar* m) { திரும்பு v_and(d, v_gt(vx_load_expand(m),   vx_setzero_u16())); }
உள்வரி v_uint32 v_diffmask(மாறிலி v_uint32& d, மாறிலி uchar* m) { திரும்பு v_and(d, v_gt(vx_load_expand_q(m), vx_setzero_u32())); }
உள்வரி v_float32 v_diffmask(மாறிலி v_float32& d, மாறிலி uchar* m) {
    v_uint32 cm = v_gt(vx_load_expand_q(m), vx_setzero_u32());
    திரும்பு v_reinterpret_as_f32(v_and(v_reinterpret_as_u32(d), cm));
}

// Shared scalar cn>1 fallbacks (match the base templates' cv_absdiff semantics).
template<typename T, typename RT>
நிலையான உள்வரி RT maskedNormDiffInfTail(மாறிலி T* s1, மாறிலி T* s2, மாறிலி uchar* mask, முழு len, முழு cn, RT result) {
    க்கு (முழு i = 0; i < len; i++) எனில் (mask[i]) {
        மாறிலி T* e1 = s1 + i*cn; மாறிலி T* e2 = s2 + i*cn;
        க்கு (முழு k = 0; k < cn; k++) result = std::max(result, (RT)cv_absdiff(e1[k], e2[k]));
    }
    திரும்பு result;
}
template<typename T, typename RT>
நிலையான உள்வரி RT maskedNormDiffL1Tail(மாறிலி T* s1, மாறிலி T* s2, மாறிலி uchar* mask, முழு len, முழு cn, RT result) {
    க்கு (முழு i = 0; i < len; i++) எனில் (mask[i]) {
        மாறிலி T* e1 = s1 + i*cn; மாறிலி T* e2 = s2 + i*cn;
        க்கு (முழு k = 0; k < cn; k++) result += (RT)cv_absdiff(e1[k], e2[k]);
    }
    திரும்பு result;
}
template<typename T, typename RT>
நிலையான உள்வரி RT maskedNormDiffL2Tail(மாறிலி T* s1, மாறிலி T* s2, மாறிலி uchar* mask, முழு len, முழு cn, RT result) {
    க்கு (முழு i = 0; i < len; i++) எனில் (mask[i]) {
        மாறிலி T* e1 = s1 + i*cn; மாறிலி T* e2 = s2 + i*cn;
        க்கு (முழு k = 0; k < cn; k++) { RT v = (RT)e1[k] - (RT)e2[k]; result += v*v; }
    }
    திரும்பு result;
}

// Inf (cn==1): reduce_max of masked |a-b|. Shared across SIMD-capable depths.
template<typename T, typename ST>
நிலையான உள்வரி ST maskedNormDiffInfCn1(மாறிலி T* s1, மாறிலி T* s2, மாறிலி uchar* mask, முழு len) {
    மாறிலி முழு vstep = VTraits<decltype(vx_load(s1))>::vlanes();
    ST result = 0;
    முழு i = 0;
    எனில் (len >= vstep) {
        auto acc = v_diffmask(v_absdiff(vx_load(s1), vx_load(s2)), mask);
        க்கு (i = vstep; i <= len - vstep; i += vstep)
            acc = v_max(acc, v_diffmask(v_absdiff(vx_load(s1 + i), vx_load(s2 + i)), mask + i));
        result = (ST)v_reduce_max(acc);
    }
    க்கு (; i < len; i++) எனில் (mask[i]) result = std::max(result, (ST)cv_absdiff(s1[i], s2[i]));
    திரும்பு result;
}

// Shared 8-bit cn==1 masked L1/L2 kernels (uchar/schar): v_absdiff yields v_uint8 for both.
template<typename T>
நிலையான உள்வரி முழு maskedNormDiffL1_8(மாறிலி T* s1, மாறிலி T* s2, மாறிலி uchar* mask, முழு len) {
    முழு i = 0; மாறிலி முழு vstep = VTraits<v_uint8>::vlanes();
    மாறிலி v_uint8 one = vx_setall_u8(1);
    v_uint32 acc = vx_setzero_u32();
    க்கு (; i <= len - vstep; i += vstep) {
        v_uint8 m  = v_gt(vx_load(mask + i), vx_setzero_u8());
        v_uint8 ad = v_and(v_absdiff(vx_load(s1 + i), vx_load(s2 + i)), m);
        acc = v_dotprod_expand_fast(ad, one, acc);
    }
    முழு result = (முழு)v_reduce_sum(acc);
    க்கு (; i < len; i++) எனில் (mask[i]) result += (முழு)cv_absdiff(s1[i], s2[i]);
    திரும்பு result;
}
template<typename T>
நிலையான உள்வரி முழு maskedNormDiffL2_8(மாறிலி T* s1, மாறிலி T* s2, மாறிலி uchar* mask, முழு len) {
    முழு i = 0; மாறிலி முழு vstep = VTraits<v_uint8>::vlanes();
    v_uint32 acc = vx_setzero_u32();
    க்கு (; i <= len - vstep; i += vstep) {
        v_uint8 m  = v_gt(vx_load(mask + i), vx_setzero_u8());
        v_uint8 ad = v_and(v_absdiff(vx_load(s1 + i), vx_load(s2 + i)), m);
        acc = v_dotprod_expand_fast(ad, ad, acc);
    }
    முழு result = (முழு)v_reduce_sum(acc);
    க்கு (; i < len; i++) எனில் (mask[i]) { முழு v = (முழு)s1[i] - (முழு)s2[i]; result += v*v; }
    திரும்பு result;
}

// Shared 16-bit cn==1 masked L1 kernel (ushort/short): v_absdiff yields v_uint16 for both.
template<typename T>
நிலையான உள்வரி முழு maskedNormDiffL1_16(மாறிலி T* s1, மாறிலி T* s2, மாறிலி uchar* mask, முழு len) {
    முழு i = 0; மாறிலி முழு vstep = VTraits<v_uint16>::vlanes();
    v_uint32 acc = vx_setzero_u32();
    க்கு (; i <= len - vstep; i += vstep) {
        v_uint16 m  = v_gt(vx_load_expand(mask + i), vx_setzero_u16());
        v_uint16 ad = v_and(v_absdiff(vx_load(s1 + i), vx_load(s2 + i)), m);
        v_uint32 lo, hi; v_expand(ad, lo, hi);
        acc = v_add(acc, v_add(lo, hi));
    }
    முழு result = (முழு)v_reduce_sum(acc);
    க்கு (; i < len; i++) எனில் (mask[i]) result += (முழு)cv_absdiff(s1[i], s2[i]);
    திரும்பு result;
}

template<> கட்டமைப்பு MaskedNormDiffInf_SIMD<uchar, முழு> {
    முழு operator()(மாறிலி uchar* s1, மாறிலி uchar* s2, மாறிலி uchar* mask, முழு len, முழு cn) மாறிலி {
        எனில் (cn == 1) திரும்பு maskedNormDiffInfCn1<uchar, முழு>(s1, s2, mask, len);
        எனில் (cn == 4 && len >= VTraits<v_uint8>::vlanes()) {
            மாறிலி முழு vstep = VTraits<v_uint8>::vlanes();
            v_uint8 acc = vx_setzero_u8();
            முழு i = 0;
            க்கு (;;) {
                எனில் (i > len - vstep) i = len - vstep; // back-step (max is idempotent)
                v_uint8 a0,a1,a2,a3,b0,b1,b2,b3;
                v_load_deinterleave(s1 + i*4, a0,a1,a2,a3);
                v_load_deinterleave(s2 + i*4, b0,b1,b2,b3);
                v_uint8 ad = v_max(v_max(v_absdiff(a0,b0), v_absdiff(a1,b1)),
                                   v_max(v_absdiff(a2,b2), v_absdiff(a3,b3)));
                v_uint8 m = v_gt(vx_load(mask + i), vx_setzero_u8());
                acc = v_max(acc, v_and(ad, m));
                எனில் (i >= len - vstep) நிறுத்து;
                i += vstep;
            }
            திரும்பு (முழு)v_reduce_max(acc);
        }
        திரும்பு maskedNormDiffInfTail<uchar, முழு>(s1, s2, mask, len, cn, 0);
    }
};
template<> கட்டமைப்பு MaskedNormDiffInf_SIMD<schar, முழு> {
    முழு operator()(மாறிலி schar* s1, மாறிலி schar* s2, மாறிலி uchar* mask, முழு len, முழு cn) மாறிலி {
        எனில் (cn == 1) திரும்பு maskedNormDiffInfCn1<schar, முழு>(s1, s2, mask, len);
        திரும்பு maskedNormDiffInfTail<schar, முழு>(s1, s2, mask, len, cn, 0);
    }
};
template<> கட்டமைப்பு MaskedNormDiffInf_SIMD<ushort, முழு> {
    முழு operator()(மாறிலி ushort* s1, மாறிலி ushort* s2, மாறிலி uchar* mask, முழு len, முழு cn) மாறிலி {
        எனில் (cn == 1) திரும்பு maskedNormDiffInfCn1<ushort, முழு>(s1, s2, mask, len);
        திரும்பு maskedNormDiffInfTail<ushort, முழு>(s1, s2, mask, len, cn, 0);
    }
};
template<> கட்டமைப்பு MaskedNormDiffInf_SIMD<short, முழு> {
    முழு operator()(மாறிலி short* s1, மாறிலி short* s2, மாறிலி uchar* mask, முழு len, முழு cn) மாறிலி {
        எனில் (cn == 1) திரும்பு maskedNormDiffInfCn1<short, முழு>(s1, s2, mask, len);
        திரும்பு maskedNormDiffInfTail<short, முழு>(s1, s2, mask, len, cn, 0);
    }
};
template<> கட்டமைப்பு MaskedNormDiffInf_SIMD<முழு, unsigned> {
    unsigned operator()(மாறிலி முழு* s1, மாறிலி முழு* s2, மாறிலி uchar* mask, முழு len, முழு cn) மாறிலி {
        எனில் (cn == 1) திரும்பு maskedNormDiffInfCn1<முழு, unsigned>(s1, s2, mask, len);
        திரும்பு maskedNormDiffInfTail<முழு, unsigned>(s1, s2, mask, len, cn, 0u);
    }
};
template<> கட்டமைப்பு MaskedNormDiffInf_SIMD<தசம, தசம> {
    தசம operator()(மாறிலி தசம* s1, மாறிலி தசம* s2, மாறிலி uchar* mask, முழு len, முழு cn) மாறிலி {
        எனில் (cn == 1) திரும்பு maskedNormDiffInfCn1<தசம, தசம>(s1, s2, mask, len);
        திரும்பு maskedNormDiffInfTail<தசம, தசம>(s1, s2, mask, len, cn, 0.0f);
    }
};

template<> கட்டமைப்பு MaskedNormDiffL1_SIMD<uchar, முழு> {
    முழு operator()(மாறிலி uchar* s1, மாறிலி uchar* s2, மாறிலி uchar* mask, முழு len, முழு cn) மாறிலி {
        எனில் (cn == 1) திரும்பு maskedNormDiffL1_8(s1, s2, mask, len);
        எனில் (cn == 4) {
            முழு i = 0; மாறிலி முழு vstep = VTraits<v_uint8>::vlanes();
            மாறிலி v_uint8 one = vx_setall_u8(1);
            v_uint32 acc = vx_setzero_u32();
            க்கு (; i <= len - vstep; i += vstep) {
                v_uint8 a0,a1,a2,a3,b0,b1,b2,b3;
                v_load_deinterleave(s1 + i*4, a0,a1,a2,a3);
                v_load_deinterleave(s2 + i*4, b0,b1,b2,b3);
                v_uint8 m = v_gt(vx_load(mask + i), vx_setzero_u8());
                acc = v_dotprod_expand_fast(v_and(v_absdiff(a0,b0), m), one, acc);
                acc = v_dotprod_expand_fast(v_and(v_absdiff(a1,b1), m), one, acc);
                acc = v_dotprod_expand_fast(v_and(v_absdiff(a2,b2), m), one, acc);
                acc = v_dotprod_expand_fast(v_and(v_absdiff(a3,b3), m), one, acc);
            }
            முழு result = (முழு)v_reduce_sum(acc);
            க்கு (; i < len; i++) எனில் (mask[i]) {
                மாறிலி uchar* e1 = s1 + i*4; மாறிலி uchar* e2 = s2 + i*4;
                க்கு (முழு k = 0; k < 4; k++) result += (முழு)cv_absdiff(e1[k], e2[k]);
            }
            திரும்பு result;
        }
        திரும்பு maskedNormDiffL1Tail<uchar, முழு>(s1, s2, mask, len, cn, 0);
    }
};
template<> கட்டமைப்பு MaskedNormDiffL1_SIMD<schar, முழு> {
    முழு operator()(மாறிலி schar* s1, மாறிலி schar* s2, மாறிலி uchar* mask, முழு len, முழு cn) மாறிலி {
        எனில் (cn == 1) திரும்பு maskedNormDiffL1_8(s1, s2, mask, len);
        திரும்பு maskedNormDiffL1Tail<schar, முழு>(s1, s2, mask, len, cn, 0);
    }
};
template<> கட்டமைப்பு MaskedNormDiffL1_SIMD<ushort, முழு> {
    முழு operator()(மாறிலி ushort* s1, மாறிலி ushort* s2, மாறிலி uchar* mask, முழு len, முழு cn) மாறிலி {
        எனில் (cn == 1) திரும்பு maskedNormDiffL1_16(s1, s2, mask, len);
        திரும்பு maskedNormDiffL1Tail<ushort, முழு>(s1, s2, mask, len, cn, 0);
    }
};
template<> கட்டமைப்பு MaskedNormDiffL1_SIMD<short, முழு> {
    முழு operator()(மாறிலி short* s1, மாறிலி short* s2, மாறிலி uchar* mask, முழு len, முழு cn) மாறிலி {
        எனில் (cn == 1) திரும்பு maskedNormDiffL1_16(s1, s2, mask, len);
        திரும்பு maskedNormDiffL1Tail<short, முழு>(s1, s2, mask, len, cn, 0);
    }
};

template<> கட்டமைப்பு MaskedNormDiffL2_SIMD<uchar, முழு> {
    முழு operator()(மாறிலி uchar* s1, மாறிலி uchar* s2, மாறிலி uchar* mask, முழு len, முழு cn) மாறிலி {
        எனில் (cn == 1) திரும்பு maskedNormDiffL2_8(s1, s2, mask, len);
        எனில் (cn == 4) {
            முழு i = 0; மாறிலி முழு vstep = VTraits<v_uint8>::vlanes();
            v_uint32 acc = vx_setzero_u32();
            க்கு (; i <= len - vstep; i += vstep) {
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
            முழு result = (முழு)v_reduce_sum(acc);
            க்கு (; i < len; i++) எனில் (mask[i]) {
                மாறிலி uchar* e1 = s1 + i*4; மாறிலி uchar* e2 = s2 + i*4;
                க்கு (முழு k = 0; k < 4; k++) { முழு v = (முழு)e1[k] - (முழு)e2[k]; result += v*v; }
            }
            திரும்பு result;
        }
        திரும்பு maskedNormDiffL2Tail<uchar, முழு>(s1, s2, mask, len, cn, 0);
    }
};
template<> கட்டமைப்பு MaskedNormDiffL2_SIMD<schar, முழு> {
    முழு operator()(மாறிலி schar* s1, மாறிலி schar* s2, மாறிலி uchar* mask, முழு len, முழு cn) மாறிலி {
        எனில் (cn == 1) திரும்பு maskedNormDiffL2_8(s1, s2, mask, len);
        திரும்பு maskedNormDiffL2Tail<schar, முழு>(s1, s2, mask, len, cn, 0);
    }
};

#எனில் CV_SIMD_64F
// Depths whose L1/L2 accumulate in double.
template<> கட்டமைப்பு MaskedNormDiffL1_SIMD<முழு, இரட்டை> {
    இரட்டை operator()(மாறிலி முழு* s1, மாறிலி முழு* s2, மாறிலி uchar* mask, முழு len, முழு cn) மாறிலி {
        எனில் (cn != 1) திரும்பு maskedNormDiffL1Tail<முழு, இரட்டை>(s1, s2, mask, len, cn, 0.0);
        முழு i = 0; மாறிலி முழு vstep = VTraits<v_int32>::vlanes();
        v_float64 acc = vx_setzero_f64();
        க்கு (; i <= len - vstep; i += vstep) {
            v_uint32 m  = v_gt(vx_load_expand_q(mask + i), vx_setzero_u32());
            v_uint32 ad = v_and(v_absdiff(vx_load(s1 + i), vx_load(s2 + i)), m);
            v_uint64 e0, e1; v_expand(ad, e0, e1);
            acc = v_add(acc, v_cvt_f64(v_reinterpret_as_s64(e0)));
            acc = v_add(acc, v_cvt_f64(v_reinterpret_as_s64(e1)));
        }
        இரட்டை result = v_reduce_sum(acc);
        க்கு (; i < len; i++) எனில் (mask[i]) result += (இரட்டை)cv_absdiff(s1[i], s2[i]);
        திரும்பு result;
    }
};
template<> கட்டமைப்பு MaskedNormDiffL1_SIMD<தசம, இரட்டை> {
    இரட்டை operator()(மாறிலி தசம* s1, மாறிலி தசம* s2, மாறிலி uchar* mask, முழு len, முழு cn) மாறிலி {
        எனில் (cn != 1) திரும்பு maskedNormDiffL1Tail<தசம, இரட்டை>(s1, s2, mask, len, cn, 0.0);
        முழு i = 0; மாறிலி முழு vstep = VTraits<v_float32>::vlanes();
        v_float64 acc0 = vx_setzero_f64(), acc1 = vx_setzero_f64();
        க்கு (; i <= len - vstep; i += vstep) {
            v_float32 m  = v_reinterpret_as_f32(v_gt(vx_load_expand_q(mask + i), vx_setzero_u32()));
            v_float32 ad = v_and(v_absdiff(vx_load(s1 + i), vx_load(s2 + i)), m);
            acc0 = v_add(acc0, v_cvt_f64(ad));
            acc1 = v_add(acc1, v_cvt_f64_உயர்வு(ad));
        }
        இரட்டை result = v_reduce_sum(v_add(acc0, acc1));
        க்கு (; i < len; i++) எனில் (mask[i]) result += (இரட்டை)cv_absdiff(s1[i], s2[i]);
        திரும்பு result;
    }
};

template<typename T>
நிலையான உள்வரி இரட்டை maskedNormDiffL2_16(மாறிலி T* s1, மாறிலி T* s2, மாறிலி uchar* mask, முழு len, முழு cn) {
    எனில் (cn != 1) திரும்பு maskedNormDiffL2Tail<T, இரட்டை>(s1, s2, mask, len, cn, 0.0);
    முழு i = 0; மாறிலி முழு vstep = VTraits<v_uint16>::vlanes();
    v_float64 acc = vx_setzero_f64();
    க்கு (; i <= len - vstep; i += vstep) {
        v_uint16 m  = v_gt(vx_load_expand(mask + i), vx_setzero_u16());
        v_uint16 ad = v_and(v_absdiff(vx_load(s1 + i), vx_load(s2 + i)), m);
        v_uint64 u = v_dotprod_expand_fast(ad, ad);
        acc = v_add(acc, v_cvt_f64(v_reinterpret_as_s64(u)));
    }
    இரட்டை result = v_reduce_sum(acc);
    க்கு (; i < len; i++) எனில் (mask[i]) { இரட்டை v = (இரட்டை)s1[i] - (இரட்டை)s2[i]; result += v*v; }
    திரும்பு result;
}
template<> கட்டமைப்பு MaskedNormDiffL2_SIMD<ushort, இரட்டை> {
    இரட்டை operator()(மாறிலி ushort* s1, மாறிலி ushort* s2, மாறிலி uchar* mask, முழு len, முழு cn) மாறிலி
    { திரும்பு maskedNormDiffL2_16(s1, s2, mask, len, cn); } };
template<> கட்டமைப்பு MaskedNormDiffL2_SIMD<short, இரட்டை> {
    இரட்டை operator()(மாறிலி short* s1, மாறிலி short* s2, மாறிலி uchar* mask, முழு len, முழு cn) மாறிலி
    { திரும்பு maskedNormDiffL2_16(s1, s2, mask, len, cn); } };

template<> கட்டமைப்பு MaskedNormDiffL2_SIMD<முழு, இரட்டை> {
    இரட்டை operator()(மாறிலி முழு* s1, மாறிலி முழு* s2, மாறிலி uchar* mask, முழு len, முழு cn) மாறிலி {
        எனில் (cn != 1) திரும்பு maskedNormDiffL2Tail<முழு, இரட்டை>(s1, s2, mask, len, cn, 0.0);
        முழு i = 0; மாறிலி முழு vstep = VTraits<v_int32>::vlanes();
        v_float64 r0 = vx_setzero_f64(), r1 = vx_setzero_f64();
        க்கு (; i <= len - vstep; i += vstep) {
            v_uint32 m  = v_gt(vx_load_expand_q(mask + i), vx_setzero_u32());
            v_uint32 ad = v_and(v_absdiff(vx_load(s1 + i), vx_load(s2 + i)), m);
            v_uint64 e0, e1; v_expand(ad, e0, e1);
            v_float64 f0 = v_cvt_f64(v_reinterpret_as_s64(e0)), f1 = v_cvt_f64(v_reinterpret_as_s64(e1));
            r0 = v_fma(f0, f0, r0); r1 = v_fma(f1, f1, r1);
        }
        இரட்டை result = v_reduce_sum(v_add(r0, r1));
        க்கு (; i < len; i++) எனில் (mask[i]) { இரட்டை v = (இரட்டை)s1[i] - (இரட்டை)s2[i]; result += v*v; }
        திரும்பு result;
    }
};
template<> கட்டமைப்பு MaskedNormDiffL2_SIMD<தசம, இரட்டை> {
    இரட்டை operator()(மாறிலி தசம* s1, மாறிலி தசம* s2, மாறிலி uchar* mask, முழு len, முழு cn) மாறிலி {
        எனில் (cn != 1) திரும்பு maskedNormDiffL2Tail<தசம, இரட்டை>(s1, s2, mask, len, cn, 0.0);
        முழு i = 0; மாறிலி முழு vstep = VTraits<v_float32>::vlanes();
        v_float64 r0 = vx_setzero_f64(), r1 = vx_setzero_f64();
        க்கு (; i <= len - vstep; i += vstep) {
            v_float32 m  = v_reinterpret_as_f32(v_gt(vx_load_expand_q(mask + i), vx_setzero_u32()));
            v_float32 ad = v_and(v_absdiff(vx_load(s1 + i), vx_load(s2 + i)), m);
            v_float64 f0 = v_cvt_f64(ad), f1 = v_cvt_f64_உயர்வு(ad);
            r0 = v_fma(f0, f0, r0); r1 = v_fma(f1, f1, r1);
        }
        இரட்டை result = v_reduce_sum(v_add(r0, r1));
        க்கு (; i < len; i++) எனில் (mask[i]) { இரட்டை v = (இரட்டை)s1[i] - (இரட்டை)s2[i]; result += v*v; }
        திரும்பு result;
    }
};
template<>
கட்டமைப்பு MaskedNormDiffInf_SIMD<இரட்டை, இரட்டை> {
    உள்வரி இரட்டை operator()(மாறிலி இரட்டை* s1, மாறிலி இரட்டை* s2, மாறிலி uchar* mask, முழு len, முழு cn) மாறிலி {
        இரட்டை result = 0.0;
        முழு i = 0;
#எனில் CV_SIMD_64F || CV_SIMD_SCALABLE_64F
        எனில் (cn == 1) {
            மாறிலி முழு vstep = VTraits<v_float64>::vlanes();
            v_float64 acc0 = vx_setzero_f64(), acc1 = vx_setzero_f64();
            க்கு (; i <= len - 2*vstep; i += 2*vstep) {
                v_uint64 m0, m1;
                v_normmask_u64(mask + i, m0, m1);
                v_float64 d0 = v_absdiff(vx_load(s1 + i), vx_load(s2 + i));
                v_float64 d1 = v_absdiff(vx_load(s1 + i + vstep), vx_load(s2 + i + vstep));
                acc0 = v_max(acc0, v_reinterpret_as_f64(v_and(v_reinterpret_as_u64(d0), m0)));
                acc1 = v_max(acc1, v_reinterpret_as_f64(v_and(v_reinterpret_as_u64(d1), m1)));
            }
            இரட்டை t[VTraits<v_float64>::max_nlanes];
            vx_store(t, v_max(acc0, acc1));
            க்கு (முழு k = 0; k < vstep; k++)
                result = std::max(result, t[k]);
        }
#endif
        க்கு (; i < len; i++) எனில் (mask[i]) {
            மாறிலி இரட்டை* e1 = s1 + i*cn; மாறிலி இரட்டை* e2 = s2 + i*cn;
            க்கு (முழு k = 0; k < cn; k++) result = std::max(result, std::abs(e1[k] - e2[k]));
        }
        திரும்பு result;
    }
};

template<>
கட்டமைப்பு MaskedNormDiffL1_SIMD<இரட்டை, இரட்டை> {
    இரட்டை operator()(மாறிலி இரட்டை* s1, மாறிலி இரட்டை* s2, மாறிலி uchar* mask, முழு len, முழு cn) மாறிலி {
        இரட்டை result = 0.0;
        எனில் (cn == 1) {
            மாறிலி முழு vstep = VTraits<v_float64>::vlanes();
            முழு i = 0;
            v_float64 acc0 = vx_setzero_f64(), acc1 = vx_setzero_f64();
            க்கு (; i <= len - 2*vstep; i += 2*vstep) {
                v_uint64 m0, m1;
                v_normmask_u64(mask + i, m0, m1);
                v_float64 d0 = v_absdiff(vx_load(s1 + i), vx_load(s2 + i));
                v_float64 d1 = v_absdiff(vx_load(s1 + i + vstep), vx_load(s2 + i + vstep));
                acc0 = v_add(acc0, v_reinterpret_as_f64(v_and(v_reinterpret_as_u64(d0), m0)));
                acc1 = v_add(acc1, v_reinterpret_as_f64(v_and(v_reinterpret_as_u64(d1), m1)));
            }
            result = v_reduce_sum(v_add(acc0, acc1));
            க்கு (; i < len; i++) எனில் (mask[i]) result += std::abs(s1[i] - s2[i]);
        } இல்லையெனில் {
            result = maskedNormDiffL1Tail<இரட்டை, இரட்டை>(s1, s2, mask, len, cn, 0.0);
        }
        திரும்பு result;
    }
};

template<>
கட்டமைப்பு MaskedNormDiffL2_SIMD<இரட்டை, இரட்டை> {
    இரட்டை operator()(மாறிலி இரட்டை* s1, மாறிலி இரட்டை* s2, மாறிலி uchar* mask, முழு len, முழு cn) மாறிலி {
        இரட்டை result = 0.0;
        எனில் (cn == 1) {
            மாறிலி முழு vstep = VTraits<v_float64>::vlanes();
            முழு i = 0;
            v_float64 acc0 = vx_setzero_f64(), acc1 = vx_setzero_f64();
            க்கு (; i <= len - 2*vstep; i += 2*vstep) {
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
            க்கு (; i < len; i++) எனில் (mask[i]) { இரட்டை v = s1[i] - s2[i]; result += v*v; }
        } இல்லையெனில் {
            result = maskedNormDiffL2Tail<இரட்டை, இரட்டை>(s1, s2, mask, len, cn, 0.0);
        }
        திரும்பு result;
    }
};
#endif // CV_SIMD_64F
#endif // CV_SIMD

template<typename T, typename ST> முழு
normDiffInf_(மாறிலி T* src1, மாறிலி T* src2, மாறிலி uchar* mask, ST* _result, முழு len, முழு cn) {
    ST result = *_result;
    எனில்( !mask ) {
        NormDiffInf_SIMD<T, ST> op;
        result = std::max(result, op(src1, src2, len*cn));
    } இல்லையெனில் {
        MaskedNormDiffInf_SIMD<T, ST> op;
        result = std::max(result, op(src1, src2, mask, len, cn));
    }
    *_result = result;
    திரும்பு 0;
}

template<typename T, typename ST> முழு
normDiffL1_(மாறிலி T* src1, மாறிலி T* src2, மாறிலி uchar* mask, ST* _result, முழு len, முழு cn) {
    ST result = *_result;
    எனில்( !mask ) {
        NormDiffL1_SIMD<T, ST> op;
        result += op(src1, src2, len*cn);
    } இல்லையெனில் {
        MaskedNormDiffL1_SIMD<T, ST> op;
        result += op(src1, src2, mask, len, cn);
    }
    *_result = result;
    திரும்பு 0;
}

template<typename T, typename ST> முழு
normDiffL2_(மாறிலி T* src1, மாறிலி T* src2, மாறிலி uchar* mask, ST* _result, முழு len, முழு cn) {
    ST result = *_result;
    எனில்( !mask ) {
        NormDiffL2_SIMD<T, ST> op;
        result += op(src1, src2, len*cn);
    } இல்லையெனில் {
        MaskedNormDiffL2_SIMD<T, ST> op;
        result += op(src1, src2, mask, len, cn);
    }
    *_result = result;
    திரும்பு 0;
}

நிலையான முழு
normDiffInf_Bool(மாறிலி uchar* src1, மாறிலி uchar* src2, மாறிலி uchar* mask, முழு* _result, முழு len, முழு cn)
{
    முழு result = *_result;
    எனில்( !mask )
    {
        க்கு( முழு i = 0; i < len*cn; i++ ) {
            result = std::max(result, (முழு)((src1[i] != 0) != (src2[i] != 0)));
            எனில் (result != 0)
                நிறுத்து;
        }
    }
    இல்லையெனில்
    {
        க்கு( முழு i = 0; i < len; i++, src1 += cn, src2 += cn )
            எனில்( mask[i] )
            {
                க்கு( முழு k = 0; k < cn; k++ )
                    result = std::max(result, (முழு)((src1[k] != 0) != (src2[k] != 0)));
                எனில் (result != 0)
                    நிறுத்து;
            }
    }
    *_result = result;
    திரும்பு 0;
}

நிலையான முழு
normDiffL1_Bool(மாறிலி uchar* src1, மாறிலி uchar* src2, மாறிலி uchar* mask, முழு* _result, முழு len, முழு cn)
{
    முழு result = *_result;
    எனில்( !mask )
    {
        க்கு( முழு i = 0; i < len*cn; i++ )
            result += (முழு)((src1[i] != 0) != (src2[i] != 0));
    }
    இல்லையெனில்
    {
        க்கு( முழு i = 0; i < len; i++, src1 += cn, src2 += cn )
            எனில்( mask[i] )
            {
                க்கு( முழு k = 0; k < cn; k++ )
                    result += (முழு)((src1[k] != 0) != (src2[k] != 0));
            }
    }
    *_result = result;
    திரும்பு 0;
}

நிலையான முழு
normDiffL2_Bool(மாறிலி uchar* src1, மாறிலி uchar* src2, மாறிலி uchar* mask, முழு* _result, முழு len, முழு cn)
{
    திரும்பு normDiffL1_Bool(src1, src2, mask, _result, len, cn);
}

#define CV_DEF_NORM_FUNC(L, suffix, type, ntype) \
    நிலையான முழு norm##L##_##suffix(மாறிலி type* src, மாறிலி uchar* mask, ntype* r, முழு len, முழு cn) \
{ CV_INSTRUMENT_REGION(); திரும்பு norm##L##_(src, mask, r, len, cn); } \
    நிலையான முழு normDiff##L##_##suffix(மாறிலி type* src1, மாறிலி type* src2, \
    மாறிலி uchar* mask, ntype* r, முழு len, முழு cn) \
{ திரும்பு normDiff##L##_(src1, src2, mask, r, (முழு)len, cn); }

#define CV_DEF_NORM_ALL(suffix, type, inftype, l1type, l2type) \
    CV_DEF_NORM_FUNC(Inf, suffix, type, inftype) \
    CV_DEF_NORM_FUNC(L1, suffix, type, l1type) \
    CV_DEF_NORM_FUNC(L2, suffix, type, l2type)

CV_DEF_NORM_ALL(8u, uchar, முழு, முழு, முழு)
CV_DEF_NORM_ALL(8s, schar, முழு, முழு, முழு)
CV_DEF_NORM_ALL(16u, ushort, முழு, முழு, இரட்டை)
CV_DEF_NORM_ALL(16s, short, முழு, முழு, இரட்டை)
CV_DEF_NORM_ALL(32u, unsigned, unsigned, இரட்டை, இரட்டை)
CV_DEF_NORM_ALL(32s, முழு, unsigned, இரட்டை, இரட்டை)
CV_DEF_NORM_ALL(32f, தசம, தசம, இரட்டை, இரட்டை)
CV_DEF_NORM_ALL(64f, இரட்டை, இரட்டை, இரட்டை, இரட்டை)
CV_DEF_NORM_ALL(64u, uint64, uint64, இரட்டை, இரட்டை)
CV_DEF_NORM_ALL(64s, int64, uint64, இரட்டை, இரட்டை)
CV_DEF_NORM_ALL(16f, hfloat, தசம, தசம, தசம)
CV_DEF_NORM_ALL(16bf, bfloat, தசம, தசம, தசம)

NormFunc getNormFunc(முழு normType, முழு depth)
{
    CV_INSTRUMENT_REGION();

    நிலையான NormFunc normTab[3][CV_DEPTH_MAX] =
    {
        {
            normWrap<uchar, முழு, normInf_8u>,
            normWrap<schar, முழு, normInf_8s>,
            normWrap<ushort, முழு, normInf_16u>,
            normWrap<short, முழு, normInf_16s>,
            normWrap<முழு, unsigned, normInf_32s>,
            normWrap<தசம, தசம, normInf_32f>,
            normWrap<இரட்டை, இரட்டை, normInf_64f>,
            normWrap<hfloat, தசம, normInf_16f>,
            normWrap<bfloat, தசம, normInf_16bf>,
            normWrap<uchar, முழு, normInf_Bool>,
            normWrap<uint64, uint64, normInf_64u>,
            normWrap<int64, uint64, normInf_64s>,
            normWrap<unsigned, unsigned, normInf_32u>,
            0
        },
        {
            normWrap<uchar, முழு, normL1_8u>,
            normWrap<schar, முழு, normL1_8s>,
            normWrap<ushort, முழு, normL1_16u>,
            normWrap<short, முழு, normL1_16s>,
            normWrap<முழு, இரட்டை, normL1_32s>,
            normWrap<தசம, இரட்டை, normL1_32f>,
            normWrap<இரட்டை, இரட்டை, normL1_64f>,
            normWrap<hfloat, தசம, normL1_16f>,
            normWrap<bfloat, தசம, normL1_16bf>,
            normWrap<uchar, முழு, normL1_Bool>,
            normWrap<uint64, இரட்டை, normL1_64u>,
            normWrap<int64, இரட்டை, normL1_64s>,
            normWrap<unsigned, இரட்டை, normL1_32u>,
            0
        },
        {
            normWrap<uchar, முழு, normL2_8u>,
            normWrap<schar, முழு, normL2_8s>,
            normWrap<ushort, இரட்டை, normL2_16u>,
            normWrap<short, இரட்டை, normL2_16s>,
            normWrap<முழு, இரட்டை, normL2_32s>,
            normWrap<தசம, இரட்டை, normL2_32f>,
            normWrap<இரட்டை, இரட்டை, normL2_64f>,
            normWrap<hfloat, தசம, normL2_16f>,
            normWrap<bfloat, தசம, normL2_16bf>,
            normWrap<uchar, முழு, normL2_Bool>,
            normWrap<uint64, இரட்டை, normL2_64u>,
            normWrap<int64, இரட்டை, normL2_64s>,
            normWrap<unsigned, இரட்டை, normL2_32u>,
            0
        }
    };

    எனில் (normType >= 3 || normType < 0) திரும்பு nullptr;

    திரும்பு normTab[normType][depth];
}

NormDiffFunc getNormDiffFunc(முழு normType, முழு depth)
{
    நிலையான NormDiffFunc normDiffTab[3][CV_DEPTH_MAX] =
    {
        {
            normDiffWrap<uchar, முழு, normDiffInf_8u>,
            normDiffWrap<schar, முழு, normDiffInf_8s>,
            normDiffWrap<ushort, முழு, normDiffInf_16u>,
            normDiffWrap<short, முழு, normDiffInf_16s>,
            normDiffWrap<முழு, unsigned, normDiffInf_32s>,
            normDiffWrap<தசம, தசம, normDiffInf_32f>,
            normDiffWrap<இரட்டை, இரட்டை, normDiffInf_64f>,
            normDiffWrap<hfloat, தசம, normDiffInf_16f>,
            normDiffWrap<bfloat, தசம, normDiffInf_16bf>,
            normDiffWrap<uchar, முழு, normDiffInf_Bool>,
            normDiffWrap<uint64, uint64, normDiffInf_64u>,
            normDiffWrap<int64, uint64, normDiffInf_64s>,
            normDiffWrap<unsigned, unsigned, normDiffInf_32u>,
            0
        },
        {
            normDiffWrap<uchar, முழு, normDiffL1_8u>,
            normDiffWrap<schar, முழு, normDiffL1_8s>,
            normDiffWrap<ushort, முழு, normDiffL1_16u>,
            normDiffWrap<short, முழு, normDiffL1_16s>,
            normDiffWrap<முழு, இரட்டை, normDiffL1_32s>,
            normDiffWrap<தசம, இரட்டை, normDiffL1_32f>,
            normDiffWrap<இரட்டை, இரட்டை, normDiffL1_64f>,
            normDiffWrap<hfloat, தசம, normDiffL1_16f>,
            normDiffWrap<bfloat, தசம, normDiffL1_16bf>,
            normDiffWrap<uchar, முழு, normDiffL1_Bool>,
            normDiffWrap<uint64, இரட்டை, normDiffL1_64u>,
            normDiffWrap<int64, இரட்டை, normDiffL1_64s>,
            normDiffWrap<unsigned, இரட்டை, normDiffL1_32u>,
            0
        },
        {
            normDiffWrap<uchar, முழு, normDiffL2_8u>,
            normDiffWrap<schar, முழு, normDiffL2_8s>,
            normDiffWrap<ushort, இரட்டை, normDiffL2_16u>,
            normDiffWrap<short, இரட்டை, normDiffL2_16s>,
            normDiffWrap<முழு, இரட்டை, normDiffL2_32s>,
            normDiffWrap<தசம, இரட்டை, normDiffL2_32f>,
            normDiffWrap<இரட்டை, இரட்டை, normDiffL2_64f>,
            normDiffWrap<hfloat, தசம, normDiffL2_16f>,
            normDiffWrap<bfloat, தசம, normDiffL2_16bf>,
            normDiffWrap<uchar, முழு, normDiffL2_Bool>,
            normDiffWrap<uint64, இரட்டை, normDiffL2_64u>,
            normDiffWrap<int64, இரட்டை, normDiffL2_64s>,
            normDiffWrap<unsigned, இரட்டை, normDiffL2_32u>,
            0
        },
    };
    எனில் (normType >= 3 || normType < 0) திரும்பு nullptr;

    திரும்பு normDiffTab[normType][depth];
}

#endif // CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

CV_CPU_OPTIMIZATION_NAMESPACE_END

} // cv::
