// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html
//
// Copyright (C) 2025, SpaceMIT Inc., all rights reserved.
// Copyright (C) 2026, Advanced Micro Devices, Inc., all rights reserved.
// Third party copyrights are property of their respective owners.

#ضمن "precomp.hpp"

نطاق cv {

کارول NormFunc = صحيح (*)(ثابت uchar*, ثابت uchar*, uchar*, صحيح, صحيح);
کارول NormDiffFunc = صحيح (*)(ثابت uchar*, ثابت uchar*, ثابت uchar*, uchar*, صحيح, صحيح);

CV_CPU_OPTIMIZATION_NAMESPACE_BEGIN

کينډۍ<ډول_نوم T, ډول_نوم RT, صحيح (*fn)(ثابت T*, ثابت uchar*, RT*, صحيح, صحيح)>
ساكن صحيح normWrap(ثابت uchar* src1, ثابت uchar* src2, uchar* res, صحيح len, صحيح cn)
{
    أرجع fn((ثابت T*)src1, src2, (RT*)res, len, cn);
}

کينډۍ<ډول_نوم T, ډول_نوم RT, صحيح (*fn)(ثابت T*, ثابت T*, ثابت uchar*, RT*, صحيح, صحيح)>
ساكن صحيح normDiffWrap(ثابت uchar* src1, ثابت uchar* src2, ثابت uchar* src3, uchar* res, صحيح len, صحيح cn)
{
    أرجع fn((ثابت T*)src1, (ثابت T*)src2, src3, (RT*)res, len, cn);
}

NormFunc getNormFunc(صحيح normType, صحيح ژوروالی);
NormDiffFunc getNormDiffFunc(صحيح normType, صحيح ژوروالی);

#که_نه_تعریف_شوی CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

کينډۍ <ډول_نوم T, ډول_نوم ST>
بنية NormInf_SIMD {
    مضمن ST چلوونکی() (ثابت T* src, صحيح n) ثابت {
        ST s = 0;
        لكل (صحيح i = 0; i < n; i++) {
            s = std::max(s, (ST)cv_abs(src[i]));
        }
        أرجع s;
    }
};

کينډۍ <ډول_نوم T, ډول_نوم ST>
بنية NormL1_SIMD {
    مضمن ST چلوونکی() (ثابت T* src, صحيح n) ثابت {
        ST s = 0;
        لكل (صحيح i = 0; i < n; i++) {
            s += cv_abs(src[i]);
        }
        أرجع s;
    }
};

کينډۍ <ډول_نوم T, ډول_نوم ST>
بنية NormL2_SIMD {
    مضمن ST چلوونکی() (ثابت T* src, صحيح n) ثابت {
        ST s = 0;
        لكل (صحيح i = 0; i < n; i++) {
            ST v = (ST)src[i];
            s += v * v;
        }
        أرجع s;
    }
};

کينډۍ <ډول_نوم T, ډول_نوم ST>
بنية NormDiffInf_SIMD {
    مضمن ST چلوونکی() (ثابت T* src1, ثابت T* src2, صحيح n) ثابت {
        ST s = 0;
        لكل (صحيح i = 0; i < n; i++) {
            ST v = (ST)cv_absdiff(src1[i], src2[i]);
            s = std::max(s, v);
        }
        أرجع s;
    }
};

کينډۍ <ډول_نوم T, ډول_نوم ST>
بنية NormDiffL1_SIMD {
    مضمن ST چلوونکی() (ثابت T* src1, ثابت T* src2, صحيح n) ثابت {
        ST s = 0;
        لكل (صحيح i = 0; i < n; i++) {
            ST v = (ST)cv_absdiff(src1[i], src2[i]);
            s += v;
        }
        أرجع s;
    }
};

کينډۍ <ډول_نوم T, ډول_نوم ST>
بنية NormDiffL2_SIMD {
    مضمن ST چلوونکی() (ثابت T* src1, ثابت T* src2, صحيح n) ثابت {
        ST s = 0;
        لكل (صحيح i = 0; i < n; i++) {
            ST v = (ST)src1[i] - (ST)src2[i];
            s += v * v;
        }
        أرجع s;
    }
};

#إذا (CV_SIMD || CV_SIMD_SCALABLE)

کينډۍ<>
بنية NormInf_SIMD<uchar, صحيح> {
    صحيح چلوونکی() (ثابت uchar* src, صحيح n) ثابت {
        صحيح j = 0;
        صحيح s = 0;
        v_uint8 r0 = vx_setzero_u8(), r1 = vx_setzero_u8();
        v_uint8 r2 = vx_setzero_u8(), r3 = vx_setzero_u8();
        لكل (; j <= n - 4 * VTraits<v_uint8>::vlanes(); j += 4 * VTraits<v_uint8>::vlanes()) {
            r0 = v_max(r0, vx_بارول(src + j                                 ));
            r1 = v_max(r1, vx_بارول(src + j +     VTraits<v_uint8>::vlanes()));
            r2 = v_max(r2, vx_بارول(src + j + 2 * VTraits<v_uint8>::vlanes()));
            r3 = v_max(r3, vx_بارول(src + j + 3 * VTraits<v_uint8>::vlanes()));
        }
        r0 = v_max(r0, v_max(r1, v_max(r2, r3)));
        لكل (; j < n; j++) {
            s = std::max(s, (صحيح)src[j]);
        }
        أرجع std::max(s, (صحيح)v_reduce_max(r0));
    }
};

کينډۍ<>
بنية NormInf_SIMD<schar, صحيح> {
    صحيح چلوونکی() (ثابت schar* src, صحيح n) ثابت {
        صحيح j = 0;
        صحيح s = 0;
        v_uint8 r0 = vx_setzero_u8(), r1 = vx_setzero_u8();
        v_uint8 r2 = vx_setzero_u8(), r3 = vx_setzero_u8();
        لكل (; j <= n - 4 * VTraits<v_int8>::vlanes(); j += 4 * VTraits<v_int8>::vlanes()) {
            r0 = v_max(r0, v_abs(vx_بارول(src + j                                )));
            r1 = v_max(r1, v_abs(vx_بارول(src + j +     VTraits<v_int8>::vlanes())));
            r2 = v_max(r2, v_abs(vx_بارول(src + j + 2 * VTraits<v_int8>::vlanes())));
            r3 = v_max(r3, v_abs(vx_بارول(src + j + 3 * VTraits<v_int8>::vlanes())));
        }
        r0 = v_max(r0, v_max(r1, v_max(r2, r3)));
        لكل (; j < n; j++) {
            s = std::max(s, cv_abs(src[j]));
        }
        أرجع std::max(s, saturate_cast<صحيح>(v_reduce_max(r0)));
    }
};

کينډۍ<>
بنية NormInf_SIMD<ushort, صحيح> {
    صحيح چلوونکی() (ثابت ushort* src, صحيح n) ثابت {
        صحيح j = 0;
        صحيح s = 0;
        v_uint16 d0 = vx_setzero_u16(), d1 = vx_setzero_u16();
        v_uint16 d2 = vx_setzero_u16(), d3 = vx_setzero_u16();
        لكل (; j <= n - 4 * VTraits<v_uint16>::vlanes(); j += 4 * VTraits<v_uint16>::vlanes()) {
            d0 = v_max(d0, vx_بارول(src + j                                  ));
            d1 = v_max(d1, vx_بارول(src + j +     VTraits<v_uint16>::vlanes()));
            d2 = v_max(d2, vx_بارول(src + j + 2 * VTraits<v_uint16>::vlanes()));
            d3 = v_max(d3, vx_بارول(src + j + 3 * VTraits<v_uint16>::vlanes()));
        }
        d0 = v_max(d0, v_max(d1, v_max(d2, d3)));
        لكل (; j < n; j++) {
            s = std::max(s, (صحيح)src[j]);
        }
        أرجع std::max(s, (صحيح)v_reduce_max(d0));
    }
};

کينډۍ<>
بنية NormInf_SIMD<لنډ, صحيح> {
    صحيح چلوونکی() (ثابت لنډ* src, صحيح n) ثابت {
        صحيح j = 0;
        صحيح s = 0;
        v_uint16 d0 = vx_setzero_u16(), d1 = vx_setzero_u16();
        v_uint16 d2 = vx_setzero_u16(), d3 = vx_setzero_u16();
        لكل (; j <= n - 4 * VTraits<v_int16>::vlanes(); j += 4 * VTraits<v_int16>::vlanes()) {
            d0 = v_max(d0, v_abs(vx_بارول(src + j                                  )));
            d1 = v_max(d1, v_abs(vx_بارول(src + j +     VTraits<v_int16>::vlanes())));
            d2 = v_max(d2, v_abs(vx_بارول(src + j + 2 * VTraits<v_int16>::vlanes())));
            d3 = v_max(d3, v_abs(vx_بارول(src + j + 3 * VTraits<v_int16>::vlanes())));
        }
        d0 = v_max(d0, v_max(d1, v_max(d2, d3)));
        لكل (; j < n; j++) {
            s = std::max(s, saturate_cast<صحيح>(cv_abs(src[j])));
        }
        أرجع std::max(s, saturate_cast<صحيح>(v_reduce_max(d0)));
    }
};

کينډۍ<>
بنية NormInf_SIMD<صحيح, صحيح> {
    صحيح چلوونکی() (ثابت صحيح* src, صحيح n) ثابت {
        صحيح j = 0;
        صحيح s = 0;
        v_uint32 r0 = vx_setzero_u32(), r1 = vx_setzero_u32();
        v_uint32 r2 = vx_setzero_u32(), r3 = vx_setzero_u32();
        لكل (; j <= n - 4 * VTraits<v_int32>::vlanes(); j += 4 * VTraits<v_int32>::vlanes()) {
            r0 = v_max(r0, v_abs(vx_بارول(src + j                                 )));
            r1 = v_max(r1, v_abs(vx_بارول(src + j +     VTraits<v_int32>::vlanes())));
            r2 = v_max(r2, v_abs(vx_بارول(src + j + 2 * VTraits<v_int32>::vlanes())));
            r3 = v_max(r3, v_abs(vx_بارول(src + j + 3 * VTraits<v_int32>::vlanes())));
        }
        r0 = v_max(r0, v_max(r1, v_max(r2, r3)));
        s = std::max(s, saturate_cast<صحيح>(v_reduce_max(r0)));
        لكل (; j < n; j++) {
            s = std::max(s, std::مطلق_قیمت(src[j]));
        }
        أرجع s;
    }
};

کينډۍ<>
بنية NormInf_SIMD<عائم, عائم> {
    عائم چلوونکی() (ثابت عائم* src, صحيح n) ثابت {
        صحيح j = 0;
        عائم s = 0.f;
        v_float32 r0 = vx_setzero_f32(), r1 = vx_setzero_f32();
        v_float32 r2 = vx_setzero_f32(), r3 = vx_setzero_f32();
        لكل (; j <= n - 4 * VTraits<v_float32>::vlanes(); j += 4 * VTraits<v_float32>::vlanes()) {
            r0 = v_max(r0, v_abs(vx_بارول(src + j                                   )));
            r1 = v_max(r1, v_abs(vx_بارول(src + j +     VTraits<v_float32>::vlanes())));
            r2 = v_max(r2, v_abs(vx_بارول(src + j + 2 * VTraits<v_float32>::vlanes())));
            r3 = v_max(r3, v_abs(vx_بارول(src + j + 3 * VTraits<v_float32>::vlanes())));
        }
        r0 = v_max(r0, v_max(r1, v_max(r2, r3)));
        لكل (; j < n; j++) {
            s = std::max(s, cv_abs(src[j]));
        }
        أرجع std::max(s, v_reduce_max(r0));
    }
};

کينډۍ<>
بنية NormL1_SIMD<uchar, صحيح> {
    صحيح چلوونکی() (ثابت uchar* src, صحيح n) ثابت {
        صحيح j = 0;
        صحيح s = 0;
        v_uint32 r0 = vx_setzero_u32(), r1 = vx_setzero_u32();
        v_uint8  one = vx_setall_u8(1);
        لكل (; j<= n - 2 * VTraits<v_uint8>::vlanes(); j += 2 * VTraits<v_uint8>::vlanes()) {
            v_uint8 v0 = vx_بارول(src + j);
            r0 = v_dotprod_expand_fast(v0, one, r0);

            v_uint8 v1 = vx_بارول(src + j + VTraits<v_uint8>::vlanes());
            r1 = v_dotprod_expand_fast(v1, one, r1);
        }
        s += v_reduce_مجموعه(v_جمع_کړه(r0, r1));
        لكل (; j < n; j++) {
            s += src[j];
        }
        أرجع s;
    }
};

کينډۍ<>
بنية NormL1_SIMD<schar, صحيح> {
    صحيح چلوونکی() (ثابت schar* src, صحيح n) ثابت {
        صحيح j = 0;
        صحيح s = 0;
        v_uint32 r0 = vx_setzero_u32(), r1 = vx_setzero_u32();
        v_uint8  one = vx_setall_u8(1);
        لكل (; j<= n - 2 * VTraits<v_int8>::vlanes(); j += 2 * VTraits<v_int8>::vlanes()) {
            v_uint8 v0 = v_abs(vx_بارول(src + j));
            r0 = v_dotprod_expand_fast(v0, one, r0);

            v_uint8 v1 = v_abs(vx_بارول(src + j + VTraits<v_int8>::vlanes()));
            r1 = v_dotprod_expand_fast(v1, one, r1);
        }
        s += v_reduce_مجموعه(v_جمع_کړه(r0, r1));
        لكل (; j < n; j++) {
            s += saturate_cast<صحيح>(cv_abs(src[j]));
        }
        أرجع s;
    }
};

کينډۍ<>
بنية NormL1_SIMD<ushort, صحيح> {
    صحيح چلوونکی() (ثابت ushort* src, صحيح n) ثابت {
        صحيح j = 0;
        صحيح s = 0;
        v_uint32 r00 = vx_setzero_u32(), r01 = vx_setzero_u32();
        v_uint32 r10 = vx_setzero_u32(), r11 = vx_setzero_u32();
        لكل (; j<= n - 2 * VTraits<v_uint16>::vlanes(); j += 2 * VTraits<v_uint16>::vlanes()) {
            v_uint16 v0 = vx_بارول(src + j);
            v_uint32 v00, v01;
            v_expand(v0, v00, v01);
            r00 = v_جمع_کړه(r00, v00);
            r01 = v_جمع_کړه(r01, v01);

            v_uint16 v1 = vx_بارول(src + j + VTraits<v_uint16>::vlanes());
            v_uint32 v10, v11;
            v_expand(v1, v10, v11);
            r10 = v_جمع_کړه(r10, v10);
            r11 = v_جمع_کړه(r11, v11);
        }
        s += (صحيح)v_reduce_مجموعه(v_جمع_کړه(v_جمع_کړه(v_جمع_کړه(r00, r01), r10), r11));
        لكل (; j < n; j++) {
            s += src[j];
        }
        أرجع s;
    }
};

کينډۍ<>
بنية NormL1_SIMD<لنډ, صحيح> {
    صحيح چلوونکی() (ثابت لنډ* src, صحيح n) ثابت {
        صحيح j = 0;
        صحيح s = 0;
        v_uint32 r00 = vx_setzero_u32(), r01 = vx_setzero_u32();
        v_uint32 r10 = vx_setzero_u32(), r11 = vx_setzero_u32();
        لكل (; j<= n - 2 * VTraits<v_int16>::vlanes(); j += 2 * VTraits<v_int16>::vlanes()) {
            v_uint16 v0 = v_abs(vx_بارول(src + j));
            v_uint32 v00, v01;
            v_expand(v0, v00, v01);
            r00 = v_جمع_کړه(r00, v00);
            r01 = v_جمع_کړه(r01, v01);

            v_uint16 v1 = v_abs(vx_بارول(src + j + VTraits<v_int16>::vlanes()));
            v_uint32 v10, v11;
            v_expand(v1, v10, v11);
            r10 = v_جمع_کړه(r10, v10);
            r11 = v_جمع_کړه(r11, v11);
        }
        s += (صحيح)v_reduce_مجموعه(v_جمع_کړه(v_جمع_کړه(v_جمع_کړه(r00, r01), r10), r11));
        لكل (; j < n; j++) {
            s += saturate_cast<صحيح>(cv_abs(src[j]));
        }
        أرجع s;
    }
};

کينډۍ<>
بنية NormL2_SIMD<uchar, صحيح> {
    صحيح چلوونکی() (ثابت uchar* src, صحيح n) ثابت {
        صحيح j = 0;
        صحيح s = 0;
        v_uint32 r0 = vx_setzero_u32(), r1 = vx_setzero_u32();
        لكل (; j <= n - 2 * VTraits<v_uint8>::vlanes(); j += 2 * VTraits<v_uint8>::vlanes()) {
            v_uint8 v0 = vx_بارول(src + j);
            r0 = v_dotprod_expand_fast(v0, v0, r0);

            v_uint8 v1 = vx_بارول(src + j + VTraits<v_uint8>::vlanes());
            r1 = v_dotprod_expand_fast(v1, v1, r1);
        }
        s += v_reduce_مجموعه(v_جمع_کړه(r0, r1));
        لكل (; j < n; j++) {
            صحيح v = saturate_cast<صحيح>(src[j]);
            s += v * v;
        }
        أرجع s;
    }
};

کينډۍ<>
بنية NormL2_SIMD<schar, صحيح> {
    صحيح چلوونکی() (ثابت schar* src, صحيح n) ثابت {
        صحيح j = 0;
        صحيح s = 0;
        v_int32 r0 = vx_setzero_s32(), r1 = vx_setzero_s32();
        لكل (; j <= n - 2 * VTraits<v_int8>::vlanes(); j += 2 * VTraits<v_int8>::vlanes()) {
            v_int8 v0 = vx_بارول(src + j);
            r0 = v_dotprod_expand_fast(v0, v0, r0);
            v_int8 v1 = vx_بارول(src + j + VTraits<v_int8>::vlanes());
            r1 = v_dotprod_expand_fast(v1, v1, r1);
        }
        s += v_reduce_مجموعه(v_جمع_کړه(r0, r1));
        لكل (; j < n; j++) {
            صحيح v = saturate_cast<صحيح>(src[j]);
            s += v * v;
        }
        أرجع s;
    }
};

کينډۍ<>
بنية NormDiffInf_SIMD<uchar, صحيح> {
    صحيح چلوونکی() (ثابت uchar* src1, ثابت uchar* src2, صحيح n) ثابت {
        صحيح j = 0;
        صحيح s = 0;
        v_uint8 r0 = vx_setzero_u8(), r1 = vx_setzero_u8();
        v_uint8 r2 = vx_setzero_u8(), r3 = vx_setzero_u8();
        لكل (; j <= n - 4 * VTraits<v_uint8>::vlanes(); j += 4 * VTraits<v_uint8>::vlanes()) {
            v_uint8 v01 = vx_بارول(src1 + j), v02 = vx_بارول(src2 + j);
            r0 = v_max(r0, v_absdiff(v01, v02));

            v_uint8 v11 = vx_بارول(src1 + j + VTraits<v_uint8>::vlanes()),
                    v12 = vx_بارول(src2 + j + VTraits<v_uint8>::vlanes());
            r1 = v_max(r1, v_absdiff(v11, v12));

            v_uint8 v21 = vx_بارول(src1 + j + 2 * VTraits<v_uint8>::vlanes()),
                    v22 = vx_بارول(src2 + j + 2 * VTraits<v_uint8>::vlanes());
            r2 = v_max(r2, v_absdiff(v21, v22));

            v_uint8 v31 = vx_بارول(src1 + j + 3 * VTraits<v_uint8>::vlanes()),
                    v32 = vx_بارول(src2 + j + 3 * VTraits<v_uint8>::vlanes());
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

کينډۍ<>
بنية NormDiffInf_SIMD<schar, صحيح> {
    صحيح چلوونکی() (ثابت schar* src1, ثابت schar* src2, صحيح n) ثابت {
        صحيح j = 0;
        صحيح s = 0;
        v_uint8 r0 = vx_setzero_u8(), r1 = vx_setzero_u8();
        v_uint8 r2 = vx_setzero_u8(), r3 = vx_setzero_u8();
        لكل (; j <= n - 4 * VTraits<v_int8>::vlanes(); j += 4 * VTraits<v_int8>::vlanes()) {
            v_int8 v01 = vx_بارول(src1 + j), v02 = vx_بارول(src2 + j);
            r0 = v_max(r0, v_absdiff(v01, v02));

            v_int8 v11 = vx_بارول(src1 + j + VTraits<v_int8>::vlanes()),
                   v12 = vx_بارول(src2 + j + VTraits<v_int8>::vlanes());
            r1 = v_max(r1, v_absdiff(v11, v12));

            v_int8 v21 = vx_بارول(src1 + j + 2 * VTraits<v_int8>::vlanes()),
                   v22 = vx_بارول(src2 + j + 2 * VTraits<v_int8>::vlanes());
            r2 = v_max(r2, v_absdiff(v21, v22));

            v_int8 v31 = vx_بارول(src1 + j + 3 * VTraits<v_int8>::vlanes()),
                   v32 = vx_بارول(src2 + j + 3 * VTraits<v_int8>::vlanes());
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

کينډۍ<>
بنية NormDiffInf_SIMD<ushort, صحيح> {
    صحيح چلوونکی() (ثابت ushort* src1, ثابت ushort* src2, صحيح n) ثابت {
        صحيح j = 0;
        صحيح s = 0;
        v_uint16 r0 = vx_setzero_u16(), r1 = vx_setzero_u16();
        v_uint16 r2 = vx_setzero_u16(), r3 = vx_setzero_u16();
        لكل (; j <= n - 4 * VTraits<v_uint16>::vlanes(); j += 4 * VTraits<v_uint16>::vlanes()) {
            v_uint16 v01 = vx_بارول(src1 + j), v02 = vx_بارول(src2 + j);
            r0 = v_max(r0, v_absdiff(v01, v02));

            v_uint16 v11 = vx_بارول(src1 + j + VTraits<v_uint16>::vlanes()),
                     v12 = vx_بارول(src2 + j + VTraits<v_uint16>::vlanes());
            r1 = v_max(r1, v_absdiff(v11, v12));

            v_uint16 v21 = vx_بارول(src1 + j + 2 * VTraits<v_uint16>::vlanes()),
                     v22 = vx_بارول(src2 + j + 2 * VTraits<v_uint16>::vlanes());
            r2 = v_max(r2, v_absdiff(v21, v22));

            v_uint16 v31 = vx_بارول(src1 + j + 3 * VTraits<v_uint16>::vlanes()),
                     v32 = vx_بارول(src2 + j + 3 * VTraits<v_uint16>::vlanes());
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

کينډۍ<>
بنية NormDiffInf_SIMD<لنډ, صحيح> {
    صحيح چلوونکی() (ثابت لنډ* src1, ثابت لنډ* src2, صحيح n) ثابت {
        صحيح j = 0;
        صحيح s = 0;
        v_uint16 r0 = vx_setzero_u16(), r1 = vx_setzero_u16();
        v_uint16 r2 = vx_setzero_u16(), r3 = vx_setzero_u16();
        لكل (; j <= n - 4 * VTraits<v_int16>::vlanes(); j += 4 * VTraits<v_int16>::vlanes()) {
            v_int16 v01 = vx_بارول(src1 + j), v02 = vx_بارول(src2 + j);
            r0 = v_max(r0, v_absdiff(v01, v02));

            v_int16 v11 = vx_بارول(src1 + j + VTraits<v_int16>::vlanes()),
                    v12 = vx_بارول(src2 + j + VTraits<v_int16>::vlanes());
            r1 = v_max(r1, v_absdiff(v11, v12));

            v_int16 v21 = vx_بارول(src1 + j + 2 * VTraits<v_int16>::vlanes()),
                    v22 = vx_بارول(src2 + j + 2 * VTraits<v_int16>::vlanes());
            r2 = v_max(r2, v_absdiff(v21, v22));

            v_int16 v31 = vx_بارول(src1 + j + 3 * VTraits<v_int16>::vlanes()),
                    v32 = vx_بارول(src2 + j + 3 * VTraits<v_int16>::vlanes());
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

کينډۍ<>
بنية NormDiffInf_SIMD<صحيح, بینښه> {
    بینښه چلوونکی() (ثابت صحيح* src1, ثابت صحيح* src2, صحيح n) ثابت {
        صحيح j = 0;
        بینښه s = 0;
        v_uint32 r0 = vx_setzero_u32(), r1 = vx_setzero_u32();
        v_uint32 r2 = vx_setzero_u32(), r3 = vx_setzero_u32();
        لكل (; j <= n - 4 * VTraits<v_int32>::vlanes(); j += 4 * VTraits<v_int32>::vlanes()) {
            v_int32 v01 = vx_بارول(src1 + j), v02 = vx_بارول(src2 + j);
            r0 = v_max(r0, v_absdiff(v01, v02));

            v_int32 v11 = vx_بارول(src1 + j + VTraits<v_int32>::vlanes()),
                    v12 = vx_بارول(src2 + j + VTraits<v_int32>::vlanes());
            r1 = v_max(r1, v_absdiff(v11, v12));

            v_int32 v21 = vx_بارول(src1 + j + 2 * VTraits<v_int32>::vlanes()),
                    v22 = vx_بارول(src2 + j + 2 * VTraits<v_int32>::vlanes());
            r2 = v_max(r2, v_absdiff(v21, v22));

            v_int32 v31 = vx_بارول(src1 + j + 3 * VTraits<v_int32>::vlanes()),
                    v32 = vx_بارول(src2 + j + 3 * VTraits<v_int32>::vlanes());
            r3 = v_max(r3, v_absdiff(v31, v32));
        }
        s = (صحيح)v_reduce_max(v_max(v_max(v_max(r0, r1), r2), r3));
        لكل (; j < n; j++) {
            بینښه v = (بینښه)cv_absdiff(src1[j], src2[j]);
            s = std::max(s, v);
        }
        أرجع s;
    }
};

کينډۍ<>
بنية NormDiffInf_SIMD<عائم, عائم> {
    عائم چلوونکی() (ثابت عائم* src1, ثابت عائم* src2, صحيح n) ثابت {
        صحيح j = 0;
        عائم s = 0;
        v_float32 r0 = vx_setzero_f32(), r1 = vx_setzero_f32();
        لكل (; j <= n - 2 * VTraits<v_float32>::vlanes(); j += 2 * VTraits<v_float32>::vlanes()) {
            v_float32 v01 = vx_بارول(src1 + j), v02 = vx_بارول(src2 + j);
            r0 = v_max(r0, v_absdiff(v01, v02));

            v_float32 v11 = vx_بارول(src1 + j + VTraits<v_float32>::vlanes()),
                      v12 = vx_بارول(src2 + j + VTraits<v_float32>::vlanes());
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

کينډۍ<>
بنية NormDiffL1_SIMD<uchar, صحيح> {
    صحيح چلوونکی() (ثابت uchar* src1, ثابت uchar* src2, صحيح n) ثابت {
        صحيح j = 0;
        صحيح s = 0;
        v_uint32 r0 = vx_setzero_u32(), r1 = vx_setzero_u32();
        v_uint8  one = vx_setall_u8(1);
        لكل (; j<= n - 2 * VTraits<v_uint8>::vlanes(); j += 2 * VTraits<v_uint8>::vlanes()) {
            v_uint8 v01 = vx_بارول(src1 + j), v02 = vx_بارول(src2 + j);
            r0 = v_dotprod_expand_fast(v_absdiff(v01, v02), one, r0);

            v_uint8 v11 = vx_بارول(src1 + j + VTraits<v_uint8>::vlanes()),
                    v12 = vx_بارول(src2 + j + VTraits<v_uint8>::vlanes());
            r1 = v_dotprod_expand_fast(v_absdiff(v11, v12), one, r1);
        }
        s += v_reduce_مجموعه(v_جمع_کړه(r0, r1));
        لكل (; j < n; j++) {
            صحيح v = (صحيح)cv_absdiff(src1[j], src2[j]);
            s += v;
        }
        أرجع s;
    }
};

کينډۍ<>
بنية NormDiffL1_SIMD<schar, صحيح> {
    صحيح چلوونکی() (ثابت schar* src1, ثابت schar* src2, صحيح n) ثابت {
        صحيح j = 0;
        صحيح s = 0;
        v_uint32 r0 = vx_setzero_u32(), r1 = vx_setzero_u32();
        v_uint8  one = vx_setall_u8(1);
        لكل (; j<= n - 2 * VTraits<v_int8>::vlanes(); j += 2 * VTraits<v_int8>::vlanes()) {
            v_int8 v01 = vx_بارول(src1 + j), v02 = vx_بارول(src2 + j);
            r0 = v_dotprod_expand_fast(v_absdiff(v01, v02), one, r0);

            v_int8 v11 = vx_بارول(src1 + j + VTraits<v_int8>::vlanes()),
                   v12 = vx_بارول(src2 + j + VTraits<v_int8>::vlanes());
            r1 = v_dotprod_expand_fast(v_absdiff(v11, v12), one, r1);
        }
        s += v_reduce_مجموعه(v_جمع_کړه(r0, r1));
        لكل (; j < n; j++) {
            صحيح v = (صحيح)cv_absdiff(src1[j], src2[j]);
            s += v;
        }
        أرجع s;
    }
};

کينډۍ<>
بنية NormDiffL1_SIMD<ushort, صحيح> {
    صحيح چلوونکی() (ثابت ushort* src1, ثابت ushort* src2, صحيح n) ثابت {
        صحيح j = 0;
        صحيح s = 0;
        v_uint32 r0 = vx_setzero_u32(), r1 = vx_setzero_u32();
        v_uint32 r2 = vx_setzero_u32(), r3 = vx_setzero_u32();
        لكل (; j<= n - 4 * VTraits<v_uint16>::vlanes(); j += 4 * VTraits<v_uint16>::vlanes()) {
            v_uint16 v01 = vx_بارول(src1 + j), v02 = vx_بارول(src2 + j);
            v_uint32 u00, u01;
            v_expand(v_absdiff(v01, v02), u00, u01);
            r0 = v_جمع_کړه(r0, v_جمع_کړه(u00, u01));

            v_uint16 v11 = vx_بارول(src1 + j + VTraits<v_uint16>::vlanes()),
                     v12 = vx_بارول(src2 + j + VTraits<v_uint16>::vlanes());
            v_uint32 u10, u11;
            v_expand(v_absdiff(v11, v12), u10, u11);
            r1 = v_جمع_کړه(r1, v_جمع_کړه(u10, u11));

            v_uint16 v21 = vx_بارول(src1 + j + 2 * VTraits<v_uint16>::vlanes()),
                     v22 = vx_بارول(src2 + j + 2 * VTraits<v_uint16>::vlanes());
            v_uint32 u20, u21;
            v_expand(v_absdiff(v21, v22), u20, u21);
            r2 = v_جمع_کړه(r2, v_جمع_کړه(u20, u21));

            v_uint16 v31 = vx_بارول(src1 + j + 3 * VTraits<v_uint16>::vlanes()),
                     v32 = vx_بارول(src2 + j + 3 * VTraits<v_uint16>::vlanes());
            v_uint32 u30, u31;
            v_expand(v_absdiff(v31, v32), u30, u31);
            r3 = v_جمع_کړه(r3, v_جمع_کړه(u30, u31));
        }
        s += (صحيح)v_reduce_مجموعه(v_جمع_کړه(v_جمع_کړه(v_جمع_کړه(r0, r1), r2), r3));
        لكل (; j < n; j++) {
            صحيح v = (صحيح)cv_absdiff(src1[j], src2[j]);
            s += v;
        }
        أرجع s;
    }
};

کينډۍ<>
بنية NormDiffL1_SIMD<لنډ, صحيح> {
    صحيح چلوونکی() (ثابت لنډ* src1, ثابت لنډ* src2, صحيح n) ثابت {
        صحيح j = 0;
        صحيح s = 0;
        v_uint32 r0 = vx_setzero_u32(), r1 = vx_setzero_u32();
        v_uint32 r2 = vx_setzero_u32(), r3 = vx_setzero_u32();
        لكل (; j<= n - 4 * VTraits<v_int16>::vlanes(); j += 4 * VTraits<v_int16>::vlanes()) {
            v_int16 v01 = vx_بارول(src1 + j), v02 = vx_بارول(src2 + j);
            v_uint32 u00, u01;
            v_expand(v_absdiff(v01, v02), u00, u01);
            r0 = v_جمع_کړه(r0, v_جمع_کړه(u00, u01));

            v_int16 v11 = vx_بارول(src1 + j + VTraits<v_int16>::vlanes()),
                    v12 = vx_بارول(src2 + j + VTraits<v_int16>::vlanes());
            v_uint32 u10, u11;
            v_expand(v_absdiff(v11, v12), u10, u11);
            r1 = v_جمع_کړه(r1, v_جمع_کړه(u10, u11));

            v_int16 v21 = vx_بارول(src1 + j + 2 * VTraits<v_int16>::vlanes()),
                    v22 = vx_بارول(src2 + j + 2 * VTraits<v_int16>::vlanes());
            v_uint32 u20, u21;
            v_expand(v_absdiff(v21, v22), u20, u21);
            r2 = v_جمع_کړه(r2, v_جمع_کړه(u20, u21));

            v_int16 v31 = vx_بارول(src1 + j + 3 * VTraits<v_int16>::vlanes()),
                    v32 = vx_بارول(src2 + j + 3 * VTraits<v_int16>::vlanes());
            v_uint32 u30, u31;
            v_expand(v_absdiff(v31, v32), u30, u31);
            r3 = v_جمع_کړه(r3, v_جمع_کړه(u30, u31));
        }
        s += (صحيح)v_reduce_مجموعه(v_جمع_کړه(v_جمع_کړه(v_جمع_کړه(r0, r1), r2), r3));
        لكل (; j < n; j++) {
            صحيح v = (صحيح)cv_absdiff(src1[j], src2[j]);
            s += v;
        }
        أرجع s;
    }
};

کينډۍ<>
بنية NormDiffL2_SIMD<uchar, صحيح> {
    صحيح چلوونکی() (ثابت uchar* src1, ثابت uchar* src2, صحيح n) ثابت {
        صحيح j = 0;
        صحيح s = 0;
        v_uint32 r0 = vx_setzero_u32(), r1 = vx_setzero_u32();
        لكل (; j <= n - 2 * VTraits<v_uint8>::vlanes(); j += 2 * VTraits<v_uint8>::vlanes()) {
            v_uint8 v01 = vx_بارول(src1 + j), v02 = vx_بارول(src2 + j);
            v_uint8 v0 = v_absdiff(v01, v02);
            r0 = v_dotprod_expand_fast(v0, v0, r0);

            v_uint8 v11 = vx_بارول(src1 + j + VTraits<v_uint8>::vlanes()),
                    v12 = vx_بارول(src2 + j + VTraits<v_uint8>::vlanes());
            v_uint8 v1 = v_absdiff(v11, v12);
            r1 = v_dotprod_expand_fast(v1, v1, r1);
        }
        s += v_reduce_مجموعه(v_جمع_کړه(r0, r1));
        لكل (; j < n; j++) {
            صحيح v = (صحيح)src1[j] - (صحيح)src2[j];
            s += v * v;
        }
        أرجع s;
    }
};

کينډۍ<>
بنية NormDiffL2_SIMD<schar, صحيح> {
    صحيح چلوونکی() (ثابت schar* src1, ثابت schar* src2, صحيح n) ثابت {
        صحيح j = 0;
        صحيح s = 0;
        v_uint32 r0 = vx_setzero_u32(), r1 = vx_setzero_u32();
        لكل (; j <= n - 2 * VTraits<v_int8>::vlanes(); j += 2 * VTraits<v_int8>::vlanes()) {
            v_int8 v01 = vx_بارول(src1 + j), v02 = vx_بارول(src2 + j);
            v_uint8 v0 = v_absdiff(v01, v02);
            r0 = v_dotprod_expand_fast(v0, v0, r0);

            v_int8 v11 = vx_بارول(src1 + j + VTraits<v_int8>::vlanes()),
                   v12 = vx_بارول(src2 + j + VTraits<v_int8>::vlanes());
            v_uint8 v1 = v_absdiff(v11, v12);
            r1 = v_dotprod_expand_fast(v1, v1, r1);
        }
        s += v_reduce_مجموعه(v_جمع_کړه(r0, r1));
        لكل (; j < n; j++) {
            صحيح v = (صحيح)src1[j] - (صحيح)src2[j];
            s += v * v;
        }
        أرجع s;
    }
};

#د_که_پای

#إذا (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)

کينډۍ<>
بنية NormInf_SIMD<مزدوج, مزدوج> {
    مزدوج چلوونکی() (ثابت مزدوج* src, صحيح n) ثابت {
        صحيح j = 0;
        مزدوج s = 0.f;
        v_float64 r0 = vx_setzero_f64(), r1 = vx_setzero_f64();
        v_float64 r2 = vx_setzero_f64(), r3 = vx_setzero_f64();
        لكل (; j <= n - 4 * VTraits<v_float64>::vlanes(); j += 4 * VTraits<v_float64>::vlanes()) {
            r0 = v_max(r0, v_abs(vx_بارول(src + j                                   )));
            r1 = v_max(r1, v_abs(vx_بارول(src + j +     VTraits<v_float64>::vlanes())));
            r2 = v_max(r2, v_abs(vx_بارول(src + j + 2 * VTraits<v_float64>::vlanes())));
            r3 = v_max(r3, v_abs(vx_بارول(src + j + 3 * VTraits<v_float64>::vlanes())));
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

کينډۍ<>
بنية NormL1_SIMD<صحيح, مزدوج> {
    مزدوج چلوونکی() (ثابت صحيح* src, صحيح n) ثابت {
        صحيح j = 0;
        مزدوج s = 0.f;
        v_float64 r00 = vx_setzero_f64(), r01 = vx_setzero_f64();
        v_float64 r10 = vx_setzero_f64(), r11 = vx_setzero_f64();
        لكل (; j <= n - 2 * VTraits<v_int32>::vlanes(); j += 2 * VTraits<v_int32>::vlanes()) {
            // Widen to double before taking the magnitude: exact over the whole int32 range,
            // INT_MIN included, which a float intermediate would not be above 2^24.
            v_int32 i0 = vx_بارول(src + j), i1 = vx_بارول(src + j + VTraits<v_int32>::vlanes());
            r00 = v_جمع_کړه(r00, v_abs(v_cvt_f64(i0))); r01 = v_جمع_کړه(r01, v_abs(v_cvt_f64_لوړ(i0)));
            r10 = v_جمع_کړه(r10, v_abs(v_cvt_f64(i1))); r11 = v_جمع_کړه(r11, v_abs(v_cvt_f64_لوړ(i1)));
        }
        s += v_reduce_مجموعه(v_جمع_کړه(v_جمع_کړه(v_جمع_کړه(r00, r01), r10), r11));
        لكل (; j < n; j++) {
            s += cv_abs(src[j]);
        }
        أرجع s;
    }
};

کينډۍ<>
بنية NormL1_SIMD<عائم, مزدوج> {
    مزدوج چلوونکی() (ثابت عائم* src, صحيح n) ثابت {
        صحيح j = 0;
        مزدوج s = 0.f;
        v_float64 r00 = vx_setzero_f64(), r01 = vx_setzero_f64();
        v_float64 r10 = vx_setzero_f64(), r11 = vx_setzero_f64();
        v_float64 r20 = vx_setzero_f64(), r21 = vx_setzero_f64();
        v_float64 r30 = vx_setzero_f64(), r31 = vx_setzero_f64();
        لكل (; j <= n - 4 * VTraits<v_float32>::vlanes(); j += 4 * VTraits<v_float32>::vlanes()) {
            v_float32 v0 = v_abs(vx_بارول(src + j)), v1 = v_abs(vx_بارول(src + j + VTraits<v_float32>::vlanes()));
            r00 = v_جمع_کړه(r00, v_cvt_f64(v0)); r01 = v_جمع_کړه(r01, v_cvt_f64_لوړ(v0));
            r10 = v_جمع_کړه(r10, v_cvt_f64(v1)); r11 = v_جمع_کړه(r11, v_cvt_f64_لوړ(v1));

            v_float32 v2 = v_abs(vx_بارول(src + j + 2 * VTraits<v_float32>::vlanes())), v3 = v_abs(vx_بارول(src + j + 3 * VTraits<v_float32>::vlanes()));
            r20 = v_جمع_کړه(r20, v_cvt_f64(v2)); r21 = v_جمع_کړه(r21, v_cvt_f64_لوړ(v2));
            r30 = v_جمع_کړه(r30, v_cvt_f64(v3)); r31 = v_جمع_کړه(r31, v_cvt_f64_لوړ(v3));
        }
        s += v_reduce_مجموعه(v_جمع_کړه(v_جمع_کړه(v_جمع_کړه(r00, r01), r10), r11));
        s += v_reduce_مجموعه(v_جمع_کړه(v_جمع_کړه(v_جمع_کړه(r20, r21), r30), r31));
        لكل (; j < n; j++) {
            s += cv_abs(src[j]);
        }
        أرجع s;
    }
};

کينډۍ<>
بنية NormL2_SIMD<ushort, مزدوج> {
    مزدوج چلوونکی() (ثابت ushort* src, صحيح n) ثابت {
        صحيح j = 0;
        مزدوج s = 0.f;
        v_float64 r0 = vx_setzero_f64(), r1 = vx_setzero_f64();
        لكل (; j <= n - 2 * VTraits<v_uint16>::vlanes(); j += 2 * VTraits<v_uint16>::vlanes()) {
            v_uint16 v0 = vx_بارول(src + j);
            v_uint64 u0 = v_dotprod_expand_fast(v0, v0);
            r0 = v_جمع_کړه(r0, v_cvt_f64(v_reinterpret_as_s64(u0)));

            v_uint16 v1 = vx_بارول(src + j + VTraits<v_uint16>::vlanes());
            v_uint64 u1 = v_dotprod_expand_fast(v1, v1);
            r1 = v_جمع_کړه(r1, v_cvt_f64(v_reinterpret_as_s64(u1)));
        }
        s += v_reduce_مجموعه(v_جمع_کړه(r0, r1));
        لكل (; j < n; j++) {
            مزدوج v = saturate_cast<مزدوج>(src[j]);
            s += v * v;
        }
        أرجع s;
    }
};

کينډۍ<>
بنية NormL2_SIMD<لنډ, مزدوج> {
    مزدوج چلوونکی() (ثابت لنډ* src, صحيح n) ثابت {
        صحيح j = 0;
        مزدوج s = 0.f;
        v_float64 r0 = vx_setzero_f64(), r1 = vx_setzero_f64();
        لكل (; j <= n - 2 * VTraits<v_int16>::vlanes(); j += 2 * VTraits<v_int16>::vlanes()) {
            v_int16 v0 = vx_بارول(src + j);
            r0 = v_جمع_کړه(r0, v_cvt_f64(v_dotprod_expand_fast(v0, v0)));

            v_int16 v1 = vx_بارول(src + j + VTraits<v_int16>::vlanes());
            r1 = v_جمع_کړه(r1, v_cvt_f64(v_dotprod_expand_fast(v1, v1)));
        }
        s += v_reduce_مجموعه(v_جمع_کړه(r0, r1));
        لكل (; j < n; j++) {
            مزدوج v = saturate_cast<مزدوج>(src[j]);
            s += v * v;
        }
        أرجع s;
    }
};

کينډۍ<>
بنية NormL2_SIMD<صحيح, مزدوج> {
    مزدوج چلوونکی() (ثابت صحيح* src, صحيح n) ثابت {
        صحيح j = 0;
        مزدوج s = 0.f;
        v_float64 r0 = vx_setzero_f64(), r1 = vx_setzero_f64();
        لكل (; j <= n - 2 * VTraits<v_int32>::vlanes(); j += 2 * VTraits<v_int32>::vlanes()) {
            v_int32 v0 = vx_بارول(src + j);
            r0 = v_dotprod_expand_fast(v0, v0, r0);

            v_int32 v1 = vx_بارول(src + j + VTraits<v_int32>::vlanes());
            r1 = v_dotprod_expand_fast(v1, v1, r1);
        }
        s += v_reduce_مجموعه(v_جمع_کړه(r0, r1));
        لكل (; j < n; j++) {
            مزدوج v = src[j];
            s += v * v;
        }
        أرجع s;
    }
};

کينډۍ<>
بنية NormL2_SIMD<عائم, مزدوج> {
    مزدوج چلوونکی() (ثابت عائم* src, صحيح n) ثابت {
        صحيح j = 0;
        مزدوج s = 0.f;
        v_float64 r00 = vx_setzero_f64(), r01 = vx_setzero_f64();
        v_float64 r10 = vx_setzero_f64(), r11 = vx_setzero_f64();
        لكل (; j <= n - 2 * VTraits<v_float32>::vlanes(); j += 2 * VTraits<v_float32>::vlanes()) {
            v_float32 v0 = vx_بارول(src + j), v1 = vx_بارول(src + j + VTraits<v_float32>::vlanes());
            v_float64 v00 = v_cvt_f64(v0), v01 = v_cvt_f64_لوړ(v0);
            v_float64 v10 = v_cvt_f64(v1), v11 = v_cvt_f64_لوړ(v1);
            r00 = v_fma(v00, v00, r00); r01 = v_fma(v01, v01, r01);
            r10 = v_fma(v10, v10, r10); r11 = v_fma(v11, v11, r11);
        }
        s += v_reduce_مجموعه(v_جمع_کړه(v_جمع_کړه(v_جمع_کړه(r00, r01), r10), r11));
        لكل (; j < n; j++) {
            مزدوج v = src[j];
            s += v * v;
        }
        أرجع s;
    }
};

#د_که_پای

#إذا CV_SIMD_64F // CV_SIMD_SCALABLE_64F has accuracy problem with the following kernels on ci

کينډۍ<>
بنية NormL1_SIMD<مزدوج, مزدوج> {
    مزدوج چلوونکی() (ثابت مزدوج* src, صحيح n) ثابت {
        صحيح j = 0;
        مزدوج s = 0.f;
        v_float64 r00 = vx_setzero_f64(), r01 = vx_setzero_f64();
        v_float64 r10 = vx_setzero_f64(), r11 = vx_setzero_f64();
        لكل (; j <= n - 4 * VTraits<v_float64>::vlanes(); j += 4 * VTraits<v_float64>::vlanes()) {
            r00 = v_جمع_کړه(r00, v_abs(vx_بارول(src + j                                   )));
            r01 = v_جمع_کړه(r01, v_abs(vx_بارول(src + j +     VTraits<v_float64>::vlanes())));
            r10 = v_جمع_کړه(r10, v_abs(vx_بارول(src + j + 2 * VTraits<v_float64>::vlanes())));
            r11 = v_جمع_کړه(r11, v_abs(vx_بارول(src + j + 3 * VTraits<v_float64>::vlanes())));
        }
        s += v_reduce_مجموعه(v_جمع_کړه(v_جمع_کړه(v_جمع_کړه(r00, r01), r10), r11));
        لكل (; j < n; j++) {
            s += cv_abs(src[j]);
        }
        أرجع s;
    }
};

کينډۍ<>
بنية NormL2_SIMD<مزدوج, مزدوج> {
    مزدوج چلوونکی() (ثابت مزدوج* src, صحيح n) ثابت {
        صحيح j = 0;
        مزدوج s = 0.f;
        v_float64 r00 = vx_setzero_f64(), r01 = vx_setzero_f64();
        v_float64 r10 = vx_setzero_f64(), r11 = vx_setzero_f64();
        لكل (; j <= n - 4 * VTraits<v_float64>::vlanes(); j += 4 * VTraits<v_float64>::vlanes()) {
            v_float64 v00 = vx_بارول(src + j                                   );
            v_float64 v01 = vx_بارول(src + j +     VTraits<v_float64>::vlanes());
            v_float64 v10 = vx_بارول(src + j + 2 * VTraits<v_float64>::vlanes());
            v_float64 v11 = vx_بارول(src + j + 3 * VTraits<v_float64>::vlanes());
            r00 = v_fma(v00, v00, r00); r01 = v_fma(v01, v01, r01);
            r10 = v_fma(v10, v10, r10); r11 = v_fma(v11, v11, r11);
        }
        s += v_reduce_مجموعه(v_جمع_کړه(v_جمع_کړه(v_جمع_کړه(r00, r01), r10), r11));
        لكل (; j < n; j++) {
            مزدوج v = src[j];
            s += v * v;
        }
        أرجع s;
    }
};

کينډۍ<>
بنية NormDiffInf_SIMD<مزدوج, مزدوج> {
    مزدوج چلوونکی() (ثابت مزدوج* src1, ثابت مزدوج* src2, صحيح n) ثابت {
        صحيح j = 0;
        مزدوج s = 0;
        v_float64 r0 = vx_setzero_f64(), r1 = vx_setzero_f64();
        لكل (; j <= n - 2 * VTraits<v_float64>::vlanes(); j += 2 * VTraits<v_float64>::vlanes()) {
            v_float64 v01 = vx_بارول(src1 + j), v02 = vx_بارول(src2 + j);
            r0 = v_max(r0, v_absdiff(v01, v02));

            v_float64 v11 = vx_بارول(src1 + j + VTraits<v_float64>::vlanes()),
                      v12 = vx_بارول(src2 + j + VTraits<v_float64>::vlanes());
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

کينډۍ<>
بنية NormDiffL1_SIMD<عائم, مزدوج> {
    مزدوج چلوونکی() (ثابت عائم* src1, ثابت عائم* src2, صحيح n) ثابت {
        صحيح j = 0;
        مزدوج s = 0.f;
        v_float64 r0 = vx_setzero_f64(), r1 = vx_setzero_f64();
        v_float64 r2 = vx_setzero_f64(), r3 = vx_setzero_f64();
        لكل (; j <= n - 2 * VTraits<v_float32>::vlanes(); j += 2 * VTraits<v_float32>::vlanes()) {
            v_float32 v01 = vx_بارول(src1 + j), v02 = vx_بارول(src2 + j);
            v_float32 v0 = v_absdiff(v01, v02);
            r0 = v_جمع_کړه(r0, v_cvt_f64(v0)); r1 = v_جمع_کړه(r1, v_cvt_f64_لوړ(v0));

            v_float32 v11 = vx_بارول(src1 + j + VTraits<v_float32>::vlanes()),
                      v12 = vx_بارول(src2 + j + VTraits<v_float32>::vlanes());
            v_float32 v1 = v_absdiff(v11, v12);
            r2 = v_جمع_کړه(r2, v_cvt_f64(v1)); r3 = v_جمع_کړه(r3, v_cvt_f64_لوړ(v1));
        }
        s += v_reduce_مجموعه(v_جمع_کړه(v_جمع_کړه(v_جمع_کړه(r0, r1), r2), r3));
        لكل (; j < n; j++) {
            مزدوج v = (مزدوج)cv_absdiff(src1[j], src2[j]);
            s += v;
        }
        أرجع s;
    }
};

کينډۍ<>
بنية NormDiffL1_SIMD<مزدوج, مزدوج> {
    مزدوج چلوونکی() (ثابت مزدوج* src1, ثابت مزدوج* src2, صحيح n) ثابت {
        صحيح j = 0;
        مزدوج s = 0.f;
        v_float64 r0 = vx_setzero_f64(), r1 = vx_setzero_f64();
        لكل (; j <= n - 2 * VTraits<v_float64>::vlanes(); j += 2 * VTraits<v_float64>::vlanes()) {
            v_float64 v01 = vx_بارول(src1 + j), v02 = vx_بارول(src2 + j);
            r0 = v_جمع_کړه(r0, v_absdiff(v01, v02));

            v_float64 v11 = vx_بارول(src1 + j + VTraits<v_float64>::vlanes()),
                      v12 = vx_بارول(src2 + j + VTraits<v_float64>::vlanes());
            r1 = v_جمع_کړه(r1, v_absdiff(v11, v12));
        }
        s += v_reduce_مجموعه(v_جمع_کړه(r0, r1));
        لكل (; j < n; j++) {
            مزدوج v = (مزدوج)cv_absdiff(src1[j], src2[j]);
            s += v;
        }
        أرجع s;
    }
};

کينډۍ<>
بنية NormDiffL2_SIMD<ushort, مزدوج> {
    مزدوج چلوونکی() (ثابت ushort* src1, ثابت ushort* src2, صحيح n) ثابت {
        صحيح j = 0;
        مزدوج s = 0.f;
        v_float64 r0 = vx_setzero_f64(), r1 = vx_setzero_f64();
        لكل (; j <= n - 2 * VTraits<v_uint16>::vlanes(); j += 2 * VTraits<v_uint16>::vlanes()) {
            v_uint16 v01 = vx_بارول(src1 + j), v02 = vx_بارول(src2 + j);
            v_uint16 v0 = v_absdiff(v01, v02);
            v_uint64 u0 = v_dotprod_expand_fast(v0, v0);
            r0 = v_جمع_کړه(r0, v_cvt_f64(v_reinterpret_as_s64(u0)));

            v_uint16 v11 = vx_بارول(src1 + j + VTraits<v_uint16>::vlanes()),
                     v12 = vx_بارول(src2 + j + VTraits<v_uint16>::vlanes());
            v_uint16 v1 = v_absdiff(v11, v12);
            v_uint64 u1 = v_dotprod_expand_fast(v1, v1);
            r1 = v_جمع_کړه(r1, v_cvt_f64(v_reinterpret_as_s64(u1)));
        }
        s += v_reduce_مجموعه(v_جمع_کړه(r0, r1));
        لكل (; j < n; j++) {
            مزدوج v = (مزدوج)src1[j] - (مزدوج)src2[j];
            s += v * v;
        }
        أرجع s;
    }
};

کينډۍ<>
بنية NormDiffL2_SIMD<لنډ, مزدوج> {
    مزدوج چلوونکی() (ثابت لنډ* src1, ثابت لنډ* src2, صحيح n) ثابت {
        صحيح j = 0;
        مزدوج s = 0.f;
        v_float64 r0 = vx_setzero_f64(), r1 = vx_setzero_f64();
        لكل (; j <= n - 2 * VTraits<v_int16>::vlanes(); j += 2 * VTraits<v_int16>::vlanes()) {
            v_int16 v01 = vx_بارول(src1 + j), v02 = vx_بارول(src2 + j);
            v_uint16 v0 = v_absdiff(v01, v02);
            v_uint64 u0 = v_dotprod_expand_fast(v0, v0);
            r0 = v_جمع_کړه(r0, v_cvt_f64(v_reinterpret_as_s64(u0)));

            v_int16 v11 = vx_بارول(src1 + j + VTraits<v_uint16>::vlanes()),
                    v12 = vx_بارول(src2 + j + VTraits<v_uint16>::vlanes());
            v_uint16 v1 = v_absdiff(v11, v12);
            v_uint64 u1 = v_dotprod_expand_fast(v1, v1);
            r1 = v_جمع_کړه(r1, v_cvt_f64(v_reinterpret_as_s64(u1)));
        }
        s += v_reduce_مجموعه(v_جمع_کړه(r0, r1));
        لكل (; j < n; j++) {
            مزدوج v = (مزدوج)src1[j] - (مزدوج)src2[j];
            s += v * v;
        }
        أرجع s;
    }
};

کينډۍ<>
بنية NormDiffL2_SIMD<صحيح, مزدوج> {
    مزدوج چلوونکی() (ثابت صحيح* src1, ثابت صحيح* src2, صحيح n) ثابت {
        صحيح j = 0;
        مزدوج s = 0.f;
        v_float64 r0 = vx_setzero_f64(), r1 = vx_setzero_f64();
        لكل (; j <= n - VTraits<v_int32>::vlanes(); j += VTraits<v_int32>::vlanes()) {
            v_int32 v01 = vx_بارول(src1 + j), v02 = vx_بارول(src2 + j);
            v_uint32 v0 = v_absdiff(v01, v02);
            v_uint64 ev0, ev1;
            v_expand(v0, ev0, ev1);
            v_float64 f0 = v_cvt_f64(v_reinterpret_as_s64(ev0)), f1 = v_cvt_f64(v_reinterpret_as_s64(ev1));
            r0 = v_fma(f0, f0, r0); r1 = v_fma(f1, f1, r1);
        }
        s += v_reduce_مجموعه(v_جمع_کړه(r0, r1));
        لكل (; j < n; j++) {
            مزدوج v = (مزدوج)src1[j] - (مزدوج)src2[j];
            s += v * v;
        }
        أرجع s;
    }
};

کينډۍ<>
بنية NormDiffL2_SIMD<عائم, مزدوج> {
    مزدوج چلوونکی() (ثابت عائم* src1, ثابت عائم* src2, صحيح n) ثابت {
        صحيح j = 0;
        مزدوج s = 0.f;
        v_float64 r0 = vx_setzero_f64(), r1 = vx_setzero_f64();
        v_float64 r2 = vx_setzero_f64(), r3 = vx_setzero_f64();
        لكل (; j <= n - 2 * VTraits<v_float32>::vlanes(); j += 2 * VTraits<v_float32>::vlanes()) {
            v_float32 v01 = vx_بارول(src1 + j), v02 = vx_بارول(src2 + j);
            v_float32 v0 = v_absdiff(v01, v02);
            v_float64 f01 = v_cvt_f64(v0), f02 = v_cvt_f64_لوړ(v0);
            r0 = v_fma(f01, f01, r0); r1 = v_fma(f02, f02, r1);

            v_float32 v11 = vx_بارول(src1 + j + VTraits<v_float32>::vlanes()),
                      v12 = vx_بارول(src2 + j + VTraits<v_float32>::vlanes());
            v_float32 v1 = v_absdiff(v11, v12);
            v_float64 f11 = v_cvt_f64(v1), f12 = v_cvt_f64_لوړ(v1);
            r2 = v_fma(f11, f11, r2); r3 = v_fma(f12, f12, r3);
        }
        s += v_reduce_مجموعه(v_جمع_کړه(v_جمع_کړه(v_جمع_کړه(r0, r1), r2), r3));
        لكل (; j < n; j++) {
            مزدوج v = (مزدوج)src1[j] - (مزدوج)src2[j];
            s += v * v;
        }
        أرجع s;
    }
};

کينډۍ<>
بنية NormDiffL2_SIMD<مزدوج, مزدوج> {
    مزدوج چلوونکی() (ثابت مزدوج* src1, ثابت مزدوج* src2, صحيح n) ثابت {
        صحيح j = 0;
        مزدوج s = 0.f;
        v_float64 r0 = vx_setzero_f64(), r1 = vx_setzero_f64();
        v_float64 r2 = vx_setzero_f64(), r3 = vx_setzero_f64();
        لكل (; j <= n - 4 * VTraits<v_float64>::vlanes(); j += 4 * VTraits<v_float64>::vlanes()) {
            v_float64 v01 = vx_بارول(src1 + j), v02 = vx_بارول(src2 + j);
            v_float64 v0 = v_absdiff(v01, v02);
            r0 = v_fma(v0, v0, r0);

            v_float64 v11 = vx_بارول(src1 + j + VTraits<v_float64>::vlanes()),
                      v12 = vx_بارول(src2 + j + VTraits<v_float64>::vlanes());
            v_float64 v1 = v_absdiff(v11, v12);
            r1 = v_fma(v1, v1, r1);

            v_float64 v21 = vx_بارول(src1 + j + 2 * VTraits<v_float64>::vlanes()),
                      v22 = vx_بارول(src2 + j + 2 * VTraits<v_float64>::vlanes());
            v_float64 v2 = v_absdiff(v21, v22);
            r2 = v_fma(v2, v2, r2);

            v_float64 v31 = vx_بارول(src1 + j + 3 * VTraits<v_float64>::vlanes()),
                      v32 = vx_بارول(src2 + j + 3 * VTraits<v_float64>::vlanes());
            v_float64 v3 = v_absdiff(v31, v32);
            r3 = v_fma(v3, v3, r3);
        }
        s += v_reduce_مجموعه(v_جمع_کړه(v_جمع_کړه(v_جمع_کړه(r0, r1), r2), r3));
        لكل (; j < n; j++) {
            مزدوج v = (مزدوج)src1[j] - (مزدوج)src2[j];
            s += v * v;
        }
        أرجع s;
    }
};

#د_که_پای

کينډۍ <ډول_نوم T, ډول_نوم ST>
بنية MaskedNormInf_SIMD {
    مضمن ST چلوونکی() (ثابت T* src, ثابت uchar* ماسک, صحيح len, صحيح cn) ثابت {
        ST s = 0;
        إذا (cn == 1) {
            لكل (صحيح i = 0; i < len; i++) {
                إذا (ماسک[i]) {
                    s = std::max(s, (ST)cv_abs(src[i]));
                }
            }
        }
        وإلا {
            لكل (صحيح i = 0; i < len; i++) {
                إذا (ماسک[i]) {
                    ثابت T* elem = src + i * cn;
                    صحيح k = 0;
                #إذا CV_ENABLE_UNROLLED && !CV_SIMD_SCALABLE
                    لكل (; k <= cn - 4; k += 4) {
                        s = std::max(s, (ST)cv_abs(elem[k]));
                        s = std::max(s, (ST)cv_abs(elem[k + 1]));
                        s = std::max(s, (ST)cv_abs(elem[k + 2]));
                        s = std::max(s, (ST)cv_abs(elem[k + 3]));
                    }
                #د_که_پای
                    لكل (; k < cn; k++) {
                        s = std::max(s, (ST)cv_abs(elem[k]));
                    }
                }
            }
        }
        أرجع s;
    }
};

کينډۍ <ډول_نوم T, ډول_نوم ST>
بنية MaskedNormL1_SIMD {
    مضمن ST چلوونکی() (ثابت T* src, ثابت uchar* ماسک, صحيح len, صحيح cn) ثابت {
        ST s = 0;
        إذا (cn == 1) {
            لكل (صحيح i = 0; i < len; i++) {
                إذا (ماسک[i]) {
                    s += (ST)cv_abs(src[i]);
                }
            }
        }
        وإلا {
            لكل (صحيح i = 0; i < len; i++) {
                إذا (ماسک[i]) {
                    ثابت T* elem = src + i * cn;
                    صحيح k = 0;
                #إذا CV_ENABLE_UNROLLED && !CV_SIMD_SCALABLE
                    لكل (; k <= cn - 4; k += 4) {
                        s += (ST)cv_abs(elem[k]);
                        s += (ST)cv_abs(elem[k + 1]);
                        s += (ST)cv_abs(elem[k + 2]);
                        s += (ST)cv_abs(elem[k + 3]);
                    }
                #د_که_پای
                    لكل (; k < cn; k++) {
                        s += (ST)cv_abs(elem[k]);
                    }
                }
            }
        }
        أرجع s;
    }
};

کينډۍ <ډول_نوم T, ډول_نوم ST>
بنية MaskedNormL2_SIMD {
    مضمن ST چلوونکی() (ثابت T* src, ثابت uchar* ماسک, صحيح len, صحيح cn) ثابت {
        ST s = 0;
        إذا (cn == 1) {
            صحيح i = 0;
        #إذا CV_ENABLE_UNROLLED && !CV_SIMD_SCALABLE
            لكل (; i <= len - 4; i += 4) {
                إذا (ماسک[i])     { T v0 = src[i];     s += (ST)v0 * v0; }
                إذا (ماسک[i + 1]) { T v1 = src[i + 1]; s += (ST)v1 * v1; }
                إذا (ماسک[i + 2]) { T v2 = src[i + 2]; s += (ST)v2 * v2; }
                إذا (ماسک[i + 3]) { T v3 = src[i + 3]; s += (ST)v3 * v3; }
            }
        #د_که_پای
            لكل (; i < len; i++) {
                إذا (ماسک[i]) {
                    T v = src[i];
                    s += (ST)v * v;
                }
            }
        }
        وإلا {
            لكل (صحيح i = 0; i < len; i++) {
                إذا (ماسک[i]) {
                    ثابت T* elem = src + i * cn;
                    صحيح k = 0;
                #إذا CV_ENABLE_UNROLLED && !CV_SIMD_SCALABLE
                    لكل (; k <= cn - 4; k += 4) {
                        T v0 = elem[k];     s += (ST)v0 * v0;
                        T v1 = elem[k + 1]; s += (ST)v1 * v1;
                        T v2 = elem[k + 2]; s += (ST)v2 * v2;
                        T v3 = elem[k + 3]; s += (ST)v3 * v3;
                    }
                #د_که_پای
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
ساكن مضمن v_uint8  v_normmask_u8 (ثابت uchar* m) { أرجع v_gt(vx_بارول(m),          vx_setzero_u8());  }
ساكن مضمن v_uint16 v_normmask_u16(ثابت uchar* m) { أرجع v_gt(vx_بارول_expand(m),   vx_setzero_u16()); }
ساكن مضمن v_uint32 v_normmask_u32(ثابت uchar* m) { أرجع v_gt(vx_بارول_expand_q(m), vx_setzero_u32()); }
ساكن مضمن فراغ v_normmask_u64(ثابت uchar* m, v_uint64& m0, v_uint64& m1) {
    v_uint64 e0, e1;
    v_expand(v_normmask_u32(m), e0, e1);
    m0 = v_or(e0, v_shl<32>(e0));
    m1 = v_or(e1, v_shl<32>(e1));
}

کينډۍ <>
بنية MaskedNormInf_SIMD<عائم, عائم> {
    مضمن عائم چلوونکی()(ثابت عائم* src, ثابت uchar* ماسک, صحيح len, صحيح cn) ثابت {
        عائم پایله = 0.0f;
        إذا (cn == 1) {
            صحيح i = 0;
            ثابت صحيح vstep = VTraits<v_float32>::vlanes();
            v_float32 acc = vx_setzero_f32();

            لكل (; i <= len - vstep; i += vstep) {
                v_uint32 m = vx_بارول_expand_q(ماسک + i);
                v_uint32 cmp = v_gt(m, vx_setzero_u32());
                v_float32 s = vx_بارول(src + i);
                s = v_abs(s);
                s = v_reinterpret_as_f32(v_and(v_reinterpret_as_u32(s), cmp));
                acc = v_max(acc, s);
            }
            پایله = v_reduce_max(acc);

            لكل (; i < len; i++) {
                إذا (ماسک[i])
                    پایله = std::max(پایله, std::مطلق_قیمت(src[i]));
            }
        }
        وإلا {
            لكل (صحيح i = 0; i < len; i++) {
                إذا (ماسک[i]) {
                    ثابت عائم* elem = src + i * cn;
                    صحيح k = 0;
                    ثابت صحيح vstep = VTraits<v_float32>::vlanes();
                    v_float32 acc = vx_setzero_f32();

                    لكل (; k <= cn - vstep; k += vstep) {
                        v_float32 s = vx_بارول(elem + k);
                        acc = v_max(acc, v_abs(s));
                    }

                    پایله = std::max(پایله, v_reduce_max(acc));

                    لكل (; k < cn; k++)
                        پایله = std::max(پایله, std::مطلق_قیمت(elem[k]));
                }
            }
        }
        أرجع پایله;
    }
};

#إذا CV_SIMD_64F
کينډۍ <>
بنية MaskedNormL1_SIMD<عائم, مزدوج> {
    مضمن مزدوج چلوونکی()(ثابت عائم* src, ثابت uchar* ماسک, صحيح len, صحيح cn) ثابت {
        مزدوج پایله = 0.0;
        إذا (cn == 1) {
            صحيح i = 0;
            ثابت صحيح vstep = VTraits<v_float32>::vlanes();
            v_float64 acc0 = vx_setzero_f64(), acc1 = vx_setzero_f64();
            v_float64 acc2 = vx_setzero_f64(), acc3 = vx_setzero_f64();

            لكل (; i <= len - 2*vstep; i += 2*vstep) {
                v_float32 s0 = v_and(v_abs(vx_بارول(src + i)),
                                     v_reinterpret_as_f32(v_normmask_u32(ماسک + i)));
                v_float32 s1 = v_and(v_abs(vx_بارول(src + i + vstep)),
                                     v_reinterpret_as_f32(v_normmask_u32(ماسک + i + vstep)));
                acc0 = v_جمع_کړه(acc0, v_cvt_f64(s0));
                acc1 = v_جمع_کړه(acc1, v_cvt_f64_لوړ(s0));
                acc2 = v_جمع_کړه(acc2, v_cvt_f64(s1));
                acc3 = v_جمع_کړه(acc3, v_cvt_f64_لوړ(s1));
            }
            لكل (; i <= len - vstep; i += vstep) {
                v_float32 s = v_and(v_abs(vx_بارول(src + i)),
                                    v_reinterpret_as_f32(v_normmask_u32(ماسک + i)));
                acc0 = v_جمع_کړه(acc0, v_cvt_f64(s));
                acc1 = v_جمع_کړه(acc1, v_cvt_f64_لوړ(s));
            }
            پایله = v_reduce_مجموعه(v_جمع_کړه(v_جمع_کړه(acc0, acc1), v_جمع_کړه(acc2, acc3)));

            لكل (; i < len; i++) {
                إذا (ماسک[i])
                    پایله += std::مطلق_قیمت(src[i]);
            }
        }
        وإلا {
            لكل (صحيح i = 0; i < len; i++) {
                إذا (ماسک[i]) {
                    ثابت عائم* elem = src + i * cn;
                    صحيح k = 0;
                    ثابت صحيح vstep = VTraits<v_float32>::vlanes();
                    v_float64 acc = vx_setzero_f64();

                    لكل (; k <= cn - vstep; k += vstep) {
                        v_float32 s = v_abs(vx_بارول(elem + k));
                        acc = v_جمع_کړه(acc, v_cvt_f64(s));
                        acc = v_جمع_کړه(acc, v_cvt_f64_لوړ(s));
                    }

                    پایله += v_reduce_مجموعه(acc);

                    لكل (; k < cn; k++)
                        پایله += std::مطلق_قیمت(elem[k]);
                }
            }
        }
        أرجع پایله;
    }
};

کينډۍ <>
بنية MaskedNormL2_SIMD<عائم, مزدوج> {
    مضمن مزدوج چلوونکی()(ثابت عائم* src, ثابت uchar* ماسک, صحيح len, صحيح cn) ثابت {
        مزدوج پایله = 0.0;
        إذا (cn == 1) {
            صحيح i = 0;
            ثابت صحيح vstep = VTraits<v_float32>::vlanes();
            v_float32 facc = vx_setzero_f32();
            v_float64 dacc = vx_setzero_f64();
            صحيح flush = 0;

            لكل (; i <= len - vstep; i += vstep, flush += vstep) {
                إذا (flush >= 64) {
                    dacc = v_جمع_کړه(dacc, v_cvt_f64(facc));
                    dacc = v_جمع_کړه(dacc, v_cvt_f64_لوړ(facc));
                    facc = vx_setzero_f32();
                    flush = 0;
                }
                v_uint32 cmp = v_gt(vx_بارول_expand_q(ماسک + i), vx_setzero_u32());
                v_float32 s  = v_reinterpret_as_f32(v_and(v_reinterpret_as_u32(vx_بارول(src + i)), cmp));
                facc = v_جمع_کړه(facc, v_mul(s, s));
            }

            dacc = v_جمع_کړه(dacc, v_cvt_f64(facc));
            dacc = v_جمع_کړه(dacc, v_cvt_f64_لوړ(facc));
            پایله = v_reduce_مجموعه(dacc);

            لكل (; i < len; i++) {
                إذا (ماسک[i]) {
                    مزدوج v = src[i];
                    پایله += v * v;
                }
            }
        }
        وإلا {
            لكل (صحيح i = 0; i < len; i++) {
                إذا (ماسک[i]) {
                    ثابت عائم* elem = src + i * cn;
                    صحيح k = 0;
                    ثابت صحيح vstep = VTraits<v_float32>::vlanes();
                    v_float32 facc = vx_setzero_f32();

                    لكل (; k <= cn - vstep; k += vstep) {
                        v_float32 s = vx_بارول(elem + k);
                        facc = v_جمع_کړه(facc, v_mul(s, s));
                    }

                    v_float64 dacc = v_جمع_کړه(v_cvt_f64(facc), v_cvt_f64_لوړ(facc));
                    پایله += v_reduce_مجموعه(dacc);

                    لكل (; k < cn; k++) {
                        مزدوج v = elem[k];
                        پایله += v * v;
                    }
                }
            }
        }
        أرجع پایله;
    }
};

#د_که_پای

کينډۍ <>
بنية MaskedNormInf_SIMD<uchar, صحيح> {
    مضمن صحيح چلوونکی()(ثابت uchar* src, ثابت uchar* ماسک, صحيح len, صحيح cn) ثابت {
        صحيح پایله = 0;
        إذا (cn == 1) {
            صحيح i = 0;
            ثابت صحيح vstep = VTraits<v_uint8>::vlanes();
            v_uint8 acc = vx_setzero_u8();

            لكل (; i <= len - vstep; i += vstep) {
                v_uint8 m   = vx_بارول(ماسک + i);
                v_uint8 s   = vx_بارول(src + i);
                v_uint8 sel = v_and(s, v_gt(m, vx_setzero_u8()));
                acc = v_max(acc, sel);
            }

            پایله = (صحيح)v_reduce_max(acc);

            لكل (; i < len; i++) {
                إذا (ماسک[i])
                    پایله = std::max(پایله, (صحيح)src[i]);
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
                v_بارول_deinterleave(src + i * 4, c0, c1, c2, c3);
                v_uint8 pmax = v_max(v_max(c0, c1), v_max(c2, c3));
                v_uint8 m = v_gt(vx_بارول(ماسک + i), vx_setzero_u8());
                acc = v_max(acc, v_and(pmax, m));
                إذا (i >= len - vstep)
                    اكسر;
                i += vstep;
            }
            پایله = (صحيح)v_reduce_max(acc);
        }
        وإلا إذا (cn == 4) {
            لكل (صحيح i = 0; i < len; i++) {
                إذا (ماسک[i]) {
                    ثابت uchar* elem = src + i * 4;
                    پایله = std::max(پایله, (صحيح)std::max(std::max(elem[0], elem[1]),
                                                            std::max(elem[2], elem[3])));
                }
            }
        }
        وإلا {
            لكل (صحيح i = 0; i < len; i++) {
                إذا (ماسک[i]) {
                    ثابت uchar* elem = src + i * cn;
                    لكل (صحيح k = 0; k < cn; k++)
                        پایله = std::max(پایله, (صحيح)elem[k]);
                }
            }
        }
        أرجع پایله;
    }
};

کينډۍ <>
بنية MaskedNormL1_SIMD<uchar, صحيح> {
    مضمن صحيح چلوونکی()(ثابت uchar* src, ثابت uchar* ماسک, صحيح len, صحيح cn) ثابت {
        صحيح پایله = 0;
        ثابت صحيح vstep = VTraits<v_uint8>::vlanes();
        ثابت v_uint8 one = vx_setall_u8(1);
        إذا (cn == 1) {
            صحيح i = 0;
            v_uint32 acc = vx_setzero_u32();
            لكل (; i <= len - vstep; i += vstep) {
                v_uint8 s = v_and(vx_بارول(src + i), v_normmask_u8(ماسک + i));
                acc = v_dotprod_expand_fast(s, one, acc);
            }
            پایله = (صحيح)v_reduce_مجموعه(acc);
            لكل (; i < len; i++) {
                إذا (ماسک[i])
                    پایله += src[i];
            }
        }
        وإلا إذا (cn == 4) {
            صحيح i = 0;
            v_uint32 acc = vx_setzero_u32();
            لكل (; i <= len - vstep; i += vstep) {
                v_uint8 c0, c1, c2, c3;
                v_بارول_deinterleave(src + i * 4, c0, c1, c2, c3);
                v_uint8 m = v_normmask_u8(ماسک + i);
                acc = v_dotprod_expand_fast(v_and(c0, m), one, acc);
                acc = v_dotprod_expand_fast(v_and(c1, m), one, acc);
                acc = v_dotprod_expand_fast(v_and(c2, m), one, acc);
                acc = v_dotprod_expand_fast(v_and(c3, m), one, acc);
            }
            پایله = (صحيح)v_reduce_مجموعه(acc);
            لكل (; i < len; i++) {
                إذا (ماسک[i]) {
                    ثابت uchar* elem = src + i * 4;
                    پایله += elem[0] + elem[1] + elem[2] + elem[3];
                }
            }
        }
        وإلا {
            لكل (صحيح i = 0; i < len; i++) {
                إذا (ماسک[i]) {
                    ثابت uchar* elem = src + i * cn;
                    لكل (صحيح k = 0; k < cn; k++)
                        پایله += elem[k];
                }
            }
        }
        أرجع پایله;
    }
};

کينډۍ <>
بنية MaskedNormInf_SIMD<ushort, صحيح> {
    مضمن صحيح چلوونکی()(ثابت ushort* src, ثابت uchar* ماسک, صحيح len, صحيح cn) ثابت {
        صحيح پایله = 0;
        إذا (cn == 1) {
            صحيح i = 0;
            ثابت صحيح vstep = VTraits<v_uint16>::vlanes();
            v_uint16 acc = vx_setzero_u16();

            لكل (; i <= len - vstep; i += vstep) {
                v_uint16 m   = vx_بارول_expand(ماسک + i);
                v_uint16 cmp = v_gt(m, vx_setzero_u16());
                v_uint16 s   = vx_بارول(src + i);
                v_uint16 sel = v_and(s, cmp);
                acc = v_max(acc, sel);
            }

            پایله = (صحيح)v_reduce_max(acc);

            لكل (; i < len; i++) {
                إذا (ماسک[i])
                    پایله = std::max(پایله, (صحيح)src[i]);
            }
        }
        وإلا {
            لكل (صحيح i = 0; i < len; i++) {
                إذا (ماسک[i]) {
                    ثابت ushort* elem = src + i * cn;
                    صحيح k = 0;
                    ثابت صحيح vstep = VTraits<v_uint16>::vlanes();
                    v_uint16 acc = vx_setzero_u16();

                    لكل (; k <= cn - vstep; k += vstep) {
                        acc = v_max(acc, vx_بارول(elem + k));
                    }

                    پایله = std::max(پایله, (صحيح)v_reduce_max(acc));

                    لكل (; k < cn; k++)
                        پایله = std::max(پایله, (صحيح)elem[k]);
                }
            }
        }
        أرجع پایله;
    }
};

کينډۍ <>
بنية MaskedNormL1_SIMD<ushort, صحيح> {
    مضمن صحيح چلوونکی()(ثابت ushort* src, ثابت uchar* ماسک, صحيح len, صحيح cn) ثابت {
        صحيح پایله = 0;
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
                    acc64 = v_جمع_کړه(acc64, v_جمع_کړه(lo64, hi64));
                    acc32 = vx_setzero_u32();
                    acc32_elems = 0;
                }
                v_uint16 m   = vx_بارول_expand(ماسک + i);
                v_uint16 cmp = v_gt(m, vx_setzero_u16());
                v_uint16 s   = v_and(vx_بارول(src + i), cmp);
                v_uint32 lo32, hi32;
                v_expand(s, lo32, hi32);
                acc32 = v_جمع_کړه(acc32, v_جمع_کړه(lo32, hi32));
            }

            v_uint64 lo64, hi64;
            v_expand(acc32, lo64, hi64);
            acc64 = v_جمع_کړه(acc64, v_جمع_کړه(lo64, hi64));
            پایله = (صحيح)v_reduce_مجموعه(acc64);

            لكل (; i < len; i++) {
                إذا (ماسک[i])
                    پایله += src[i];
            }
        }
        وإلا {
            لكل (صحيح i = 0; i < len; i++) {
                إذا (ماسک[i]) {
                    ثابت ushort* elem = src + i * cn;
                    صحيح k = 0;
                    ثابت صحيح vstep = VTraits<v_uint16>::vlanes();
                    v_uint32 acc = vx_setzero_u32();

                    لكل (; k <= cn - vstep; k += vstep) {
                        v_uint32 lo32, hi32;
                        v_expand(vx_بارول(elem + k), lo32, hi32);
                        acc = v_جمع_کړه(acc, v_جمع_کړه(lo32, hi32));
                    }

                    پایله += (صحيح)v_reduce_مجموعه(acc);

                    لكل (; k < cn; k++)
                        پایله += elem[k];
                }
            }
        }
        أرجع پایله;
    }
};

کينډۍ <>
بنية MaskedNormL2_SIMD<ushort, مزدوج> {
    مضمن مزدوج چلوونکی()(ثابت ushort* src, ثابت uchar* ماسک, صحيح len, صحيح cn) ثابت {
        مزدوج پایله = 0.0;
        إذا (cn == 1) {
            صحيح i = 0;
            ثابت صحيح vstep = VTraits<v_uint16>::vlanes();
            v_uint64 acc = vx_setzero_u64();
            لكل (; i <= len - vstep; i += vstep) {
                v_uint16 m   = vx_بارول_expand(ماسک + i);
                v_uint16 cmp = v_gt(m, vx_setzero_u16());
                v_uint16 s   = v_and(vx_بارول(src + i), cmp);
                v_uint32 lo32, hi32;
                v_expand(s, lo32, hi32);
                v_uint64 lo64a, lo64b, hi64a, hi64b;
                v_expand(v_mul(lo32, lo32), lo64a, lo64b);
                v_expand(v_mul(hi32, hi32), hi64a, hi64b);
                acc = v_جمع_کړه(acc, v_جمع_کړه(v_جمع_کړه(lo64a, lo64b), v_جمع_کړه(hi64a, hi64b)));
            }
            پایله = (مزدوج)v_reduce_مجموعه(acc);
            لكل (; i < len; i++) {
                إذا (ماسک[i]) {
                    مزدوج v = src[i];
                    پایله += v * v;
                }
            }
        }
        وإلا {
            لكل (صحيح i = 0; i < len; i++) {
                إذا (ماسک[i]) {
                    ثابت ushort* elem = src + i * cn;
                    صحيح k = 0;
                    ثابت صحيح vstep = VTraits<v_uint16>::vlanes();
                    v_uint64 acc = vx_setzero_u64();
                    لكل (; k <= cn - vstep; k += vstep) {
                        v_uint32 lo32, hi32;
                        v_expand(vx_بارول(elem + k), lo32, hi32);
                        v_uint64 lo64a, lo64b, hi64a, hi64b;
                        v_expand(v_mul(lo32, lo32), lo64a, lo64b);
                        v_expand(v_mul(hi32, hi32), hi64a, hi64b);
                        acc = v_جمع_کړه(acc, v_جمع_کړه(v_جمع_کړه(lo64a, lo64b), v_جمع_کړه(hi64a, hi64b)));
                    }

                    پایله += (مزدوج)v_reduce_مجموعه(acc);

                    لكل (; k < cn; k++) {
                        مزدوج v = elem[k];
                        پایله += v * v;
                    }
                }
            }
        }
        أرجع پایله;
    }
};

کينډۍ <>
بنية MaskedNormInf_SIMD<schar, صحيح> {
    مضمن صحيح چلوونکی()(ثابت schar* src, ثابت uchar* ماسک, صحيح len, صحيح cn) ثابت {
        صحيح پایله = 0;
        إذا (cn == 1) {
            ثابت صحيح vstep = VTraits<v_int8>::vlanes();
            صحيح i = 0;
            v_uint8 acc = vx_setzero_u8();
            لكل (; i <= len - vstep; i += vstep)
                acc = v_max(acc, v_and(v_abs(vx_بارول(src + i)), v_normmask_u8(ماسک + i)));
            پایله = (صحيح)v_reduce_max(acc);
            لكل (; i < len; i++)
                إذا (ماسک[i]) پایله = std::max(پایله, cv_abs(src[i]));
        }
        وإلا {
            لكل (صحيح i = 0; i < len; i++)
                إذا (ماسک[i]) {
                    ثابت schar* elem = src + i * cn;
                    لكل (صحيح k = 0; k < cn; k++)
                        پایله = std::max(پایله, cv_abs(elem[k]));
                }
        }
        أرجع پایله;
    }
};

کينډۍ <>
بنية MaskedNormL1_SIMD<schar, صحيح> {
    مضمن صحيح چلوونکی()(ثابت schar* src, ثابت uchar* ماسک, صحيح len, صحيح cn) ثابت {
        صحيح پایله = 0;
        إذا (cn == 1) {
            ثابت صحيح vstep = VTraits<v_int8>::vlanes();
            ثابت v_uint8 one = vx_setall_u8(1);
            صحيح i = 0;
            v_uint32 acc = vx_setzero_u32();
            لكل (; i <= len - vstep; i += vstep)
                acc = v_dotprod_expand_fast(v_and(v_abs(vx_بارول(src + i)), v_normmask_u8(ماسک + i)), one, acc);
            پایله = (صحيح)v_reduce_مجموعه(acc);
            لكل (; i < len; i++)
                إذا (ماسک[i]) پایله += cv_abs(src[i]);
        }
        وإلا {
            لكل (صحيح i = 0; i < len; i++)
                إذا (ماسک[i]) {
                    ثابت schar* elem = src + i * cn;
                    لكل (صحيح k = 0; k < cn; k++)
                        پایله += cv_abs(elem[k]);
                }
        }
        أرجع پایله;
    }
};

کينډۍ <>
بنية MaskedNormL2_SIMD<schar, صحيح> {
    مضمن صحيح چلوونکی()(ثابت schar* src, ثابت uchar* ماسک, صحيح len, صحيح cn) ثابت {
        صحيح پایله = 0;
        إذا (cn == 1) {
            ثابت صحيح vstep = VTraits<v_int8>::vlanes();
            صحيح i = 0;
            v_int32 acc = vx_setzero_s32();
            لكل (; i <= len - vstep; i += vstep) {
                v_int8 s = v_and(vx_بارول(src + i), v_reinterpret_as_s8(v_normmask_u8(ماسک + i)));
                acc = v_dotprod_expand_fast(s, s, acc);
            }
            پایله = (صحيح)v_reduce_مجموعه(acc);
            لكل (; i < len; i++)
                إذا (ماسک[i]) { صحيح v = src[i]; پایله += v * v; }
        }
        وإلا {
            لكل (صحيح i = 0; i < len; i++)
                إذا (ماسک[i]) {
                    ثابت schar* elem = src + i * cn;
                    لكل (صحيح k = 0; k < cn; k++) { صحيح v = elem[k]; پایله += v * v; }
                }
        }
        أرجع پایله;
    }
};

کينډۍ <>
بنية MaskedNormL2_SIMD<uchar, صحيح> {
    مضمن صحيح چلوونکی()(ثابت uchar* src, ثابت uchar* ماسک, صحيح len, صحيح cn) ثابت {
        صحيح پایله = 0;
        ثابت صحيح vstep = VTraits<v_uint8>::vlanes();
        إذا (cn == 1) {
            صحيح i = 0;
            v_uint32 acc = vx_setzero_u32();
            لكل (; i <= len - vstep; i += vstep) {
                v_uint8 s = v_and(vx_بارول(src + i), v_normmask_u8(ماسک + i));
                acc = v_dotprod_expand_fast(s, s, acc);
            }
            پایله = (صحيح)v_reduce_مجموعه(acc);
            لكل (; i < len; i++)
                إذا (ماسک[i]) { صحيح v = src[i]; پایله += v * v; }
        }
        وإلا إذا (cn == 4) {
            صحيح i = 0;
            v_uint32 acc = vx_setzero_u32();
            لكل (; i <= len - vstep; i += vstep) {
                v_uint8 c0, c1, c2, c3;
                v_بارول_deinterleave(src + i * 4, c0, c1, c2, c3);
                v_uint8 m = v_normmask_u8(ماسک + i);
                c0 = v_and(c0, m); c1 = v_and(c1, m);
                c2 = v_and(c2, m); c3 = v_and(c3, m);
                acc = v_dotprod_expand_fast(c0, c0, acc);
                acc = v_dotprod_expand_fast(c1, c1, acc);
                acc = v_dotprod_expand_fast(c2, c2, acc);
                acc = v_dotprod_expand_fast(c3, c3, acc);
            }
            پایله = (صحيح)v_reduce_مجموعه(acc);
            لكل (; i < len; i++)
                إذا (ماسک[i]) {
                    ثابت uchar* elem = src + i * 4;
                    لكل (صحيح k = 0; k < 4; k++) { صحيح v = elem[k]; پایله += v * v; }
                }
        }
        وإلا {
            لكل (صحيح i = 0; i < len; i++)
                إذا (ماسک[i]) {
                    ثابت uchar* elem = src + i * cn;
                    لكل (صحيح k = 0; k < cn; k++) { صحيح v = elem[k]; پایله += v * v; }
                }
        }
        أرجع پایله;
    }
};

کينډۍ <>
بنية MaskedNormInf_SIMD<لنډ, صحيح> {
    مضمن صحيح چلوونکی()(ثابت لنډ* src, ثابت uchar* ماسک, صحيح len, صحيح cn) ثابت {
        صحيح پایله = 0;
        إذا (cn == 1) {
            ثابت صحيح vstep = VTraits<v_int16>::vlanes();
            صحيح i = 0;
            v_uint16 acc = vx_setzero_u16();
            لكل (; i <= len - vstep; i += vstep)
                acc = v_max(acc, v_and(v_abs(vx_بارول(src + i)), v_normmask_u16(ماسک + i)));
            پایله = (صحيح)v_reduce_max(acc);
            لكل (; i < len; i++)
                إذا (ماسک[i]) پایله = std::max(پایله, cv_abs(src[i]));
        }
        وإلا {
            لكل (صحيح i = 0; i < len; i++)
                إذا (ماسک[i]) {
                    ثابت لنډ* elem = src + i * cn;
                    لكل (صحيح k = 0; k < cn; k++)
                        پایله = std::max(پایله, cv_abs(elem[k]));
                }
        }
        أرجع پایله;
    }
};

کينډۍ <>
بنية MaskedNormL1_SIMD<لنډ, صحيح> {
    مضمن صحيح چلوونکی()(ثابت لنډ* src, ثابت uchar* ماسک, صحيح len, صحيح cn) ثابت {
        صحيح پایله = 0;
        إذا (cn == 1) {
            ثابت صحيح vstep = VTraits<v_int16>::vlanes();
            صحيح i = 0;
            v_uint32 acc0 = vx_setzero_u32(), acc1 = vx_setzero_u32();
            لكل (; i <= len - vstep; i += vstep) {
                v_uint16 s = v_and(v_abs(vx_بارول(src + i)), v_normmask_u16(ماسک + i));
                v_uint32 lo, hi;
                v_expand(s, lo, hi);
                acc0 = v_جمع_کړه(acc0, lo);
                acc1 = v_جمع_کړه(acc1, hi);
            }
            پایله = (صحيح)v_reduce_مجموعه(v_جمع_کړه(acc0, acc1));
            لكل (; i < len; i++)
                إذا (ماسک[i]) پایله += cv_abs(src[i]);
        }
        وإلا {
            لكل (صحيح i = 0; i < len; i++)
                إذا (ماسک[i]) {
                    ثابت لنډ* elem = src + i * cn;
                    لكل (صحيح k = 0; k < cn; k++)
                        پایله += cv_abs(elem[k]);
                }
        }
        أرجع پایله;
    }
};

کينډۍ <>
بنية MaskedNormL2_SIMD<لنډ, مزدوج> {
    مضمن مزدوج چلوونکی()(ثابت لنډ* src, ثابت uchar* ماسک, صحيح len, صحيح cn) ثابت {
        مزدوج پایله = 0.0;
        إذا (cn == 1) {
            ثابت صحيح vstep = VTraits<v_int16>::vlanes();
            صحيح i = 0;
            v_uint64 acc = vx_setzero_u64();
            لكل (; i <= len - vstep; i += vstep) {
                v_uint16 s = v_and(v_abs(vx_بارول(src + i)), v_normmask_u16(ماسک + i));
                acc = v_جمع_کړه(acc, v_dotprod_expand_fast(s, s));
            }
            پایله = (مزدوج)v_reduce_مجموعه(acc);
            لكل (; i < len; i++)
                إذا (ماسک[i]) { مزدوج v = src[i]; پایله += v * v; }
        }
        وإلا {
            لكل (صحيح i = 0; i < len; i++)
                إذا (ماسک[i]) {
                    ثابت لنډ* elem = src + i * cn;
                    لكل (صحيح k = 0; k < cn; k++) { مزدوج v = elem[k]; پایله += v * v; }
                }
        }
        أرجع پایله;
    }
};

کينډۍ <>
بنية MaskedNormInf_SIMD<صحيح, بینښه> {
    مضمن بینښه چلوونکی()(ثابت صحيح* src, ثابت uchar* ماسک, صحيح len, صحيح cn) ثابت {
        بینښه پایله = 0;
        إذا (cn == 1) {
            ثابت صحيح vstep = VTraits<v_int32>::vlanes();
            صحيح i = 0;
            v_uint32 acc = vx_setzero_u32();
            لكل (; i <= len - vstep; i += vstep)
                acc = v_max(acc, v_and(v_abs(vx_بارول(src + i)), v_normmask_u32(ماسک + i)));
            پایله = v_reduce_max(acc);
            لكل (; i < len; i++)
                إذا (ماسک[i]) پایله = std::max(پایله, cv_abs(src[i]));
        }
        وإلا {
            لكل (صحيح i = 0; i < len; i++)
                إذا (ماسک[i]) {
                    ثابت صحيح* elem = src + i * cn;
                    لكل (صحيح k = 0; k < cn; k++)
                        پایله = std::max(پایله, cv_abs(elem[k]));
                }
        }
        أرجع پایله;
    }
};

#إذا CV_SIMD_64F || CV_SIMD_SCALABLE_64F

کينډۍ <>
بنية MaskedNormL1_SIMD<صحيح, مزدوج> {
    مضمن مزدوج چلوونکی()(ثابت صحيح* src, ثابت uchar* ماسک, صحيح len, صحيح cn) ثابت {
        مزدوج پایله = 0.0;
        إذا (cn == 1) {
            ثابت صحيح vstep = VTraits<v_int32>::vlanes();
            صحيح i = 0;
            v_float64 acc0 = vx_setzero_f64(), acc1 = vx_setzero_f64();
            لكل (; i <= len - vstep; i += vstep) {
                // Mask the value, then widen and take the magnitude in double. v_abs() does give
                // |INT_MIN| as an unsigned 2147483648, but reinterpreting that back as int32 turns
                // it negative again, and the widening then carries the sign into the accumulator.
                v_int32 s = v_reinterpret_as_s32(v_and(v_reinterpret_as_u32(vx_بارول(src + i)),
                                                       v_normmask_u32(ماسک + i)));
                acc0 = v_جمع_کړه(acc0, v_abs(v_cvt_f64(s)));
                acc1 = v_جمع_کړه(acc1, v_abs(v_cvt_f64_لوړ(s)));
            }
            پایله = v_reduce_مجموعه(v_جمع_کړه(acc0, acc1));
            لكل (; i < len; i++)
                إذا (ماسک[i]) پایله += cv_abs(src[i]);
        }
        وإلا {
            لكل (صحيح i = 0; i < len; i++)
                إذا (ماسک[i]) {
                    ثابت صحيح* elem = src + i * cn;
                    لكل (صحيح k = 0; k < cn; k++)
                        پایله += cv_abs(elem[k]);
                }
        }
        أرجع پایله;
    }
};

کينډۍ <>
بنية MaskedNormL2_SIMD<صحيح, مزدوج> {
    مضمن مزدوج چلوونکی()(ثابت صحيح* src, ثابت uchar* ماسک, صحيح len, صحيح cn) ثابت {
        مزدوج پایله = 0.0;
        إذا (cn == 1) {
            ثابت صحيح vstep = VTraits<v_int32>::vlanes();
            صحيح i = 0;
            v_float64 acc0 = vx_setzero_f64(), acc1 = vx_setzero_f64();
            لكل (; i <= len - vstep; i += vstep) {
                v_uint32 s = v_and(v_abs(vx_بارول(src + i)), v_normmask_u32(ماسک + i));
                v_uint64 e0, e1;
                v_expand(s, e0, e1);
                v_float64 f0 = v_cvt_f64(v_reinterpret_as_s64(e0));
                v_float64 f1 = v_cvt_f64(v_reinterpret_as_s64(e1));
                acc0 = v_fma(f0, f0, acc0);
                acc1 = v_fma(f1, f1, acc1);
            }
            پایله = v_reduce_مجموعه(v_جمع_کړه(acc0, acc1));
            لكل (; i < len; i++)
                إذا (ماسک[i]) { مزدوج v = src[i]; پایله += v * v; }
        }
        وإلا {
            لكل (صحيح i = 0; i < len; i++)
                إذا (ماسک[i]) {
                    ثابت صحيح* elem = src + i * cn;
                    لكل (صحيح k = 0; k < cn; k++) { مزدوج v = elem[k]; پایله += v * v; }
                }
        }
        أرجع پایله;
    }
};

کينډۍ <>
بنية MaskedNormInf_SIMD<مزدوج, مزدوج> {
    مضمن مزدوج چلوونکی()(ثابت مزدوج* src, ثابت uchar* ماسک, صحيح len, صحيح cn) ثابت {
        مزدوج پایله = 0.0;
        إذا (cn == 1) {
            ثابت صحيح vstep = VTraits<v_float64>::vlanes();
            صحيح i = 0;
            v_float64 acc0 = vx_setzero_f64(), acc1 = vx_setzero_f64();
            لكل (; i <= len - 2*vstep; i += 2*vstep) {
                v_uint64 m0, m1;
                v_normmask_u64(ماسک + i, m0, m1);
                acc0 = v_max(acc0, v_reinterpret_as_f64(v_and(v_reinterpret_as_u64(v_abs(vx_بارول(src + i))), m0)));
                acc1 = v_max(acc1, v_reinterpret_as_f64(v_and(v_reinterpret_as_u64(v_abs(vx_بارول(src + i + vstep))), m1)));
            }
            مزدوج t[VTraits<v_float64>::max_nlanes];
            vx_store(t, v_max(acc0, acc1));
            لكل (صحيح k = 0; k < vstep; k++)
                پایله = std::max(پایله, t[k]);
            لكل (; i < len; i++)
                إذا (ماسک[i]) پایله = std::max(پایله, cv_abs(src[i]));
        }
        وإلا {
            لكل (صحيح i = 0; i < len; i++)
                إذا (ماسک[i]) {
                    ثابت مزدوج* elem = src + i * cn;
                    لكل (صحيح k = 0; k < cn; k++)
                        پایله = std::max(پایله, cv_abs(elem[k]));
                }
        }
        أرجع پایله;
    }
};

کينډۍ <>
بنية MaskedNormL1_SIMD<مزدوج, مزدوج> {
    مضمن مزدوج چلوونکی()(ثابت مزدوج* src, ثابت uchar* ماسک, صحيح len, صحيح cn) ثابت {
        مزدوج پایله = 0.0;
        إذا (cn == 1) {
            ثابت صحيح vstep = VTraits<v_float64>::vlanes();
            صحيح i = 0;
            v_float64 acc0 = vx_setzero_f64(), acc1 = vx_setzero_f64();
            لكل (; i <= len - 2*vstep; i += 2*vstep) {
                v_uint64 m0, m1;
                v_normmask_u64(ماسک + i, m0, m1);
                acc0 = v_جمع_کړه(acc0, v_reinterpret_as_f64(v_and(v_reinterpret_as_u64(v_abs(vx_بارول(src + i))), m0)));
                acc1 = v_جمع_کړه(acc1, v_reinterpret_as_f64(v_and(v_reinterpret_as_u64(v_abs(vx_بارول(src + i + vstep))), m1)));
            }
            پایله = v_reduce_مجموعه(v_جمع_کړه(acc0, acc1));
            لكل (; i < len; i++)
                إذا (ماسک[i]) پایله += cv_abs(src[i]);
        }
        وإلا {
            لكل (صحيح i = 0; i < len; i++)
                إذا (ماسک[i]) {
                    ثابت مزدوج* elem = src + i * cn;
                    لكل (صحيح k = 0; k < cn; k++)
                        پایله += cv_abs(elem[k]);
                }
        }
        أرجع پایله;
    }
};

کينډۍ <>
بنية MaskedNormL2_SIMD<مزدوج, مزدوج> {
    مضمن مزدوج چلوونکی()(ثابت مزدوج* src, ثابت uchar* ماسک, صحيح len, صحيح cn) ثابت {
        مزدوج پایله = 0.0;
        إذا (cn == 1) {
            ثابت صحيح vstep = VTraits<v_float64>::vlanes();
            صحيح i = 0;
            v_float64 acc0 = vx_setzero_f64(), acc1 = vx_setzero_f64();
            لكل (; i <= len - 2*vstep; i += 2*vstep) {
                v_uint64 m0, m1;
                v_normmask_u64(ماسک + i, m0, m1);
                v_float64 s0 = v_reinterpret_as_f64(v_and(v_reinterpret_as_u64(vx_بارول(src + i)), m0));
                v_float64 s1 = v_reinterpret_as_f64(v_and(v_reinterpret_as_u64(vx_بارول(src + i + vstep)), m1));
                acc0 = v_fma(s0, s0, acc0);
                acc1 = v_fma(s1, s1, acc1);
            }
            پایله = v_reduce_مجموعه(v_جمع_کړه(acc0, acc1));
            لكل (; i < len; i++)
                إذا (ماسک[i]) { مزدوج v = src[i]; پایله += v * v; }
        }
        وإلا {
            لكل (صحيح i = 0; i < len; i++)
                إذا (ماسک[i]) {
                    ثابت مزدوج* elem = src + i * cn;
                    لكل (صحيح k = 0; k < cn; k++) { مزدوج v = elem[k]; پایله += v * v; }
                }
        }
        أرجع پایله;
    }
};

#د_که_پای // CV_SIMD_64F || CV_SIMD_SCALABLE_64F
#د_که_پای // CV_SIMD || CV_SIMD_SCALABLE

کينډۍ<ډول_نوم T, ډول_نوم ST> صحيح
normInf_(ثابت T* src, ثابت uchar* ماسک, ST* پایله_2, صحيح len, صحيح cn)
{
    ST پایله = *پایله_2;
    إذا( !ماسک )
    {
        NormInf_SIMD<T, ST> op;
        پایله = std::max(پایله, op(src, len*cn));
    } وإلا {
        MaskedNormInf_SIMD<T, ST> op;
        پایله = std::max(پایله, op(src, ماسک, len, cn));
    }
    *پایله_2 = پایله;
    أرجع 0;
}

کينډۍ<ډول_نوم T, ډول_نوم ST> صحيح
normL1_(ثابت T* src, ثابت uchar* ماسک, ST* پایله_2, صحيح len, صحيح cn)
{
    ST پایله = *پایله_2;
    إذا( !ماسک )
    {
        NormL1_SIMD<T, ST> op;
        پایله += op(src, len*cn);
    } وإلا {
        MaskedNormL1_SIMD<T, ST> op;
        پایله += op(src, ماسک, len, cn);
    }
    *پایله_2 = پایله;
    أرجع 0;
}

کينډۍ<ډول_نوم T, ډول_نوم ST> صحيح
normL2_(ثابت T* src, ثابت uchar* ماسک, ST* پایله_2, صحيح len, صحيح cn)
{
    ST پایله = *پایله_2;
    إذا( !ماسک )
    {
        NormL2_SIMD<T, ST> op;
        پایله += op(src, len*cn);
    }
    وإلا
    {
        MaskedNormL2_SIMD<T, ST> op;
        پایله += op(src, ماسک, len, cn);
    }
    *پایله_2 = پایله;
    أرجع 0;
}

ساكن صحيح
normInf_Bool(ثابت uchar* src, ثابت uchar* ماسک, صحيح* پایله_2, صحيح len, صحيح cn)
{
    صحيح پایله = *پایله_2;
    إذا( !ماسک )
    {
        لكل ( صحيح i = 0; i < len*cn; i++ ) {
            پایله = std::max(پایله, (صحيح)(src[i] != 0));
            إذا (پایله != 0)
                اكسر;
        }
    }
    وإلا
    {
        لكل( صحيح i = 0; i < len; i++, src += cn )
            إذا( ماسک[i] )
            {
                لكل( صحيح k = 0; k < cn; k++ )
                    پایله = std::max(پایله, (صحيح)(src[k] != 0));
                إذا (پایله != 0)
                    اكسر;
            }
    }
    *پایله_2 = پایله;
    أرجع 0;
}

ساكن صحيح
normL1_Bool(ثابت uchar* src, ثابت uchar* ماسک, صحيح* پایله_2, صحيح len, صحيح cn)
{
    صحيح پایله = *پایله_2;
    إذا( !ماسک )
    {
        لكل ( صحيح i = 0; i < len*cn; i++ )
            پایله += (صحيح)(src[i] != 0);
    }
    وإلا
    {
        لكل( صحيح i = 0; i < len; i++, src += cn )
            إذا( ماسک[i] )
            {
                لكل( صحيح k = 0; k < cn; k++ )
                    پایله += (صحيح)(src[k] != 0);
            }
    }
    *پایله_2 = پایله;
    أرجع 0;
}

ساكن صحيح
normL2_Bool(ثابت uchar* src, ثابت uchar* ماسک, صحيح* پایله_2, صحيح len, صحيح cn)
{
    أرجع normL1_Bool(src, ماسک, پایله_2, len, cn);
}

// ====================================================================
// Masked norm-diff kernels.
// Scalar base templates (safe for every depth) plus SIMD cn==1
// specializations for the concrete (T, ST) pairs dispatched by
// CV_DEF_NORM_ALL. The masked normDiff*_ functions below select them.
// ====================================================================
کينډۍ <ډول_نوم T, ډول_نوم ST>
بنية MaskedNormDiffInf_SIMD {
    مضمن ST چلوونکی()(ثابت T* s1, ثابت T* s2, ثابت uchar* ماسک, صحيح len, صحيح cn) ثابت {
        ST پایله = 0;
        لكل (صحيح i = 0; i < len; i++) إذا (ماسک[i]) {
            ثابت T* e1 = s1 + i*cn; ثابت T* e2 = s2 + i*cn;
            لكل (صحيح k = 0; k < cn; k++)
                پایله = std::max(پایله, (ST)cv_absdiff(e1[k], e2[k]));
        }
        أرجع پایله;
    }
};
کينډۍ <ډول_نوم T, ډول_نوم ST>
بنية MaskedNormDiffL1_SIMD {
    مضمن ST چلوونکی()(ثابت T* s1, ثابت T* s2, ثابت uchar* ماسک, صحيح len, صحيح cn) ثابت {
        ST پایله = 0;
        لكل (صحيح i = 0; i < len; i++) إذا (ماسک[i]) {
            ثابت T* e1 = s1 + i*cn; ثابت T* e2 = s2 + i*cn;
            لكل (صحيح k = 0; k < cn; k++)
                پایله += (ST)cv_absdiff(e1[k], e2[k]);
        }
        أرجع پایله;
    }
};
کينډۍ <ډول_نوم T, ډول_نوم ST>
بنية MaskedNormDiffL2_SIMD {
    مضمن ST چلوونکی()(ثابت T* s1, ثابت T* s2, ثابت uchar* ماسک, صحيح len, صحيح cn) ثابت {
        ST پایله = 0;
        لكل (صحيح i = 0; i < len; i++) إذا (ماسک[i]) {
            ثابت T* e1 = s1 + i*cn; ثابت T* e2 = s2 + i*cn;
            لكل (صحيح k = 0; k < cn; k++) { ST v = (ST)e1[k] - (ST)e2[k]; پایله += v*v; }
        }
        أرجع پایله;
    }
};

#إذا (CV_SIMD || CV_SIMD_SCALABLE)
// AND a per-lane diff vector with the (expanded) mask>0 predicate.
مضمن v_uint8  v_diffmask(ثابت v_uint8&  d, ثابت uchar* m) { أرجع v_and(d, v_gt(vx_بارول(m),          vx_setzero_u8())); }
مضمن v_uint16 v_diffmask(ثابت v_uint16& d, ثابت uchar* m) { أرجع v_and(d, v_gt(vx_بارول_expand(m),   vx_setzero_u16())); }
مضمن v_uint32 v_diffmask(ثابت v_uint32& d, ثابت uchar* m) { أرجع v_and(d, v_gt(vx_بارول_expand_q(m), vx_setzero_u32())); }
مضمن v_float32 v_diffmask(ثابت v_float32& d, ثابت uchar* m) {
    v_uint32 cm = v_gt(vx_بارول_expand_q(m), vx_setzero_u32());
    أرجع v_reinterpret_as_f32(v_and(v_reinterpret_as_u32(d), cm));
}

// Shared scalar cn>1 fallbacks (match the base templates' cv_absdiff semantics).
کينډۍ<ډول_نوم T, ډول_نوم RT>
ساكن مضمن RT masked_Norm_Diff_Inf_لکۍ(ثابت T* s1, ثابت T* s2, ثابت uchar* ماسک, صحيح len, صحيح cn, RT پایله) {
    لكل (صحيح i = 0; i < len; i++) إذا (ماسک[i]) {
        ثابت T* e1 = s1 + i*cn; ثابت T* e2 = s2 + i*cn;
        لكل (صحيح k = 0; k < cn; k++) پایله = std::max(پایله, (RT)cv_absdiff(e1[k], e2[k]));
    }
    أرجع پایله;
}
کينډۍ<ډول_نوم T, ډول_نوم RT>
ساكن مضمن RT masked_Norm_Diff_L1_لکۍ(ثابت T* s1, ثابت T* s2, ثابت uchar* ماسک, صحيح len, صحيح cn, RT پایله) {
    لكل (صحيح i = 0; i < len; i++) إذا (ماسک[i]) {
        ثابت T* e1 = s1 + i*cn; ثابت T* e2 = s2 + i*cn;
        لكل (صحيح k = 0; k < cn; k++) پایله += (RT)cv_absdiff(e1[k], e2[k]);
    }
    أرجع پایله;
}
کينډۍ<ډول_نوم T, ډول_نوم RT>
ساكن مضمن RT masked_Norm_Diff_L2_لکۍ(ثابت T* s1, ثابت T* s2, ثابت uchar* ماسک, صحيح len, صحيح cn, RT پایله) {
    لكل (صحيح i = 0; i < len; i++) إذا (ماسک[i]) {
        ثابت T* e1 = s1 + i*cn; ثابت T* e2 = s2 + i*cn;
        لكل (صحيح k = 0; k < cn; k++) { RT v = (RT)e1[k] - (RT)e2[k]; پایله += v*v; }
    }
    أرجع پایله;
}

// Inf (cn==1): reduce_max of masked |a-b|. Shared across SIMD-capable depths.
کينډۍ<ډول_نوم T, ډول_نوم ST>
ساكن مضمن ST maskedNormDiffInfCn1(ثابت T* s1, ثابت T* s2, ثابت uchar* ماسک, صحيح len) {
    ثابت صحيح vstep = VTraits<decltype(vx_بارول(s1))>::vlanes();
    ST پایله = 0;
    صحيح i = 0;
    إذا (len >= vstep) {
        خپلکاری acc = v_diffmask(v_absdiff(vx_بارول(s1), vx_بارول(s2)), ماسک);
        لكل (i = vstep; i <= len - vstep; i += vstep)
            acc = v_max(acc, v_diffmask(v_absdiff(vx_بارول(s1 + i), vx_بارول(s2 + i)), ماسک + i));
        پایله = (ST)v_reduce_max(acc);
    }
    لكل (; i < len; i++) إذا (ماسک[i]) پایله = std::max(پایله, (ST)cv_absdiff(s1[i], s2[i]));
    أرجع پایله;
}

// Shared 8-bit cn==1 masked L1/L2 kernels (uchar/schar): v_absdiff yields v_uint8 for both.
کينډۍ<ډول_نوم T>
ساكن مضمن صحيح maskedNormDiffL1_8(ثابت T* s1, ثابت T* s2, ثابت uchar* ماسک, صحيح len) {
    صحيح i = 0; ثابت صحيح vstep = VTraits<v_uint8>::vlanes();
    ثابت v_uint8 one = vx_setall_u8(1);
    v_uint32 acc = vx_setzero_u32();
    لكل (; i <= len - vstep; i += vstep) {
        v_uint8 m  = v_gt(vx_بارول(ماسک + i), vx_setzero_u8());
        v_uint8 ad = v_and(v_absdiff(vx_بارول(s1 + i), vx_بارول(s2 + i)), m);
        acc = v_dotprod_expand_fast(ad, one, acc);
    }
    صحيح پایله = (صحيح)v_reduce_مجموعه(acc);
    لكل (; i < len; i++) إذا (ماسک[i]) پایله += (صحيح)cv_absdiff(s1[i], s2[i]);
    أرجع پایله;
}
کينډۍ<ډول_نوم T>
ساكن مضمن صحيح maskedNormDiffL2_8(ثابت T* s1, ثابت T* s2, ثابت uchar* ماسک, صحيح len) {
    صحيح i = 0; ثابت صحيح vstep = VTraits<v_uint8>::vlanes();
    v_uint32 acc = vx_setzero_u32();
    لكل (; i <= len - vstep; i += vstep) {
        v_uint8 m  = v_gt(vx_بارول(ماسک + i), vx_setzero_u8());
        v_uint8 ad = v_and(v_absdiff(vx_بارول(s1 + i), vx_بارول(s2 + i)), m);
        acc = v_dotprod_expand_fast(ad, ad, acc);
    }
    صحيح پایله = (صحيح)v_reduce_مجموعه(acc);
    لكل (; i < len; i++) إذا (ماسک[i]) { صحيح v = (صحيح)s1[i] - (صحيح)s2[i]; پایله += v*v; }
    أرجع پایله;
}

// Shared 16-bit cn==1 masked L1 kernel (ushort/short): v_absdiff yields v_uint16 for both.
کينډۍ<ډول_نوم T>
ساكن مضمن صحيح maskedNormDiffL1_16(ثابت T* s1, ثابت T* s2, ثابت uchar* ماسک, صحيح len) {
    صحيح i = 0; ثابت صحيح vstep = VTraits<v_uint16>::vlanes();
    v_uint32 acc = vx_setzero_u32();
    لكل (; i <= len - vstep; i += vstep) {
        v_uint16 m  = v_gt(vx_بارول_expand(ماسک + i), vx_setzero_u16());
        v_uint16 ad = v_and(v_absdiff(vx_بارول(s1 + i), vx_بارول(s2 + i)), m);
        v_uint32 lo, hi; v_expand(ad, lo, hi);
        acc = v_جمع_کړه(acc, v_جمع_کړه(lo, hi));
    }
    صحيح پایله = (صحيح)v_reduce_مجموعه(acc);
    لكل (; i < len; i++) إذا (ماسک[i]) پایله += (صحيح)cv_absdiff(s1[i], s2[i]);
    أرجع پایله;
}

کينډۍ<> بنية MaskedNormDiffInf_SIMD<uchar, صحيح> {
    صحيح چلوونکی()(ثابت uchar* s1, ثابت uchar* s2, ثابت uchar* ماسک, صحيح len, صحيح cn) ثابت {
        إذا (cn == 1) أرجع maskedNormDiffInfCn1<uchar, صحيح>(s1, s2, ماسک, len);
        إذا (cn == 4 && len >= VTraits<v_uint8>::vlanes()) {
            ثابت صحيح vstep = VTraits<v_uint8>::vlanes();
            v_uint8 acc = vx_setzero_u8();
            صحيح i = 0;
            لكل (;;) {
                إذا (i > len - vstep) i = len - vstep; // back-step (max is idempotent)
                v_uint8 a0,a1,a2,a3,b0,b1,b2,b3;
                v_بارول_deinterleave(s1 + i*4, a0,a1,a2,a3);
                v_بارول_deinterleave(s2 + i*4, b0,b1,b2,b3);
                v_uint8 ad = v_max(v_max(v_absdiff(a0,b0), v_absdiff(a1,b1)),
                                   v_max(v_absdiff(a2,b2), v_absdiff(a3,b3)));
                v_uint8 m = v_gt(vx_بارول(ماسک + i), vx_setzero_u8());
                acc = v_max(acc, v_and(ad, m));
                إذا (i >= len - vstep) اكسر;
                i += vstep;
            }
            أرجع (صحيح)v_reduce_max(acc);
        }
        أرجع masked_Norm_Diff_Inf_لکۍ<uchar, صحيح>(s1, s2, ماسک, len, cn, 0);
    }
};
کينډۍ<> بنية MaskedNormDiffInf_SIMD<schar, صحيح> {
    صحيح چلوونکی()(ثابت schar* s1, ثابت schar* s2, ثابت uchar* ماسک, صحيح len, صحيح cn) ثابت {
        إذا (cn == 1) أرجع maskedNormDiffInfCn1<schar, صحيح>(s1, s2, ماسک, len);
        أرجع masked_Norm_Diff_Inf_لکۍ<schar, صحيح>(s1, s2, ماسک, len, cn, 0);
    }
};
کينډۍ<> بنية MaskedNormDiffInf_SIMD<ushort, صحيح> {
    صحيح چلوونکی()(ثابت ushort* s1, ثابت ushort* s2, ثابت uchar* ماسک, صحيح len, صحيح cn) ثابت {
        إذا (cn == 1) أرجع maskedNormDiffInfCn1<ushort, صحيح>(s1, s2, ماسک, len);
        أرجع masked_Norm_Diff_Inf_لکۍ<ushort, صحيح>(s1, s2, ماسک, len, cn, 0);
    }
};
کينډۍ<> بنية MaskedNormDiffInf_SIMD<لنډ, صحيح> {
    صحيح چلوونکی()(ثابت لنډ* s1, ثابت لنډ* s2, ثابت uchar* ماسک, صحيح len, صحيح cn) ثابت {
        إذا (cn == 1) أرجع maskedNormDiffInfCn1<لنډ, صحيح>(s1, s2, ماسک, len);
        أرجع masked_Norm_Diff_Inf_لکۍ<لنډ, صحيح>(s1, s2, ماسک, len, cn, 0);
    }
};
کينډۍ<> بنية MaskedNormDiffInf_SIMD<صحيح, بینښه> {
    بینښه چلوونکی()(ثابت صحيح* s1, ثابت صحيح* s2, ثابت uchar* ماسک, صحيح len, صحيح cn) ثابت {
        إذا (cn == 1) أرجع maskedNormDiffInfCn1<صحيح, بینښه>(s1, s2, ماسک, len);
        أرجع masked_Norm_Diff_Inf_لکۍ<صحيح, بینښه>(s1, s2, ماسک, len, cn, 0u);
    }
};
کينډۍ<> بنية MaskedNormDiffInf_SIMD<عائم, عائم> {
    عائم چلوونکی()(ثابت عائم* s1, ثابت عائم* s2, ثابت uchar* ماسک, صحيح len, صحيح cn) ثابت {
        إذا (cn == 1) أرجع maskedNormDiffInfCn1<عائم, عائم>(s1, s2, ماسک, len);
        أرجع masked_Norm_Diff_Inf_لکۍ<عائم, عائم>(s1, s2, ماسک, len, cn, 0.0f);
    }
};

کينډۍ<> بنية MaskedNormDiffL1_SIMD<uchar, صحيح> {
    صحيح چلوونکی()(ثابت uchar* s1, ثابت uchar* s2, ثابت uchar* ماسک, صحيح len, صحيح cn) ثابت {
        إذا (cn == 1) أرجع maskedNormDiffL1_8(s1, s2, ماسک, len);
        إذا (cn == 4) {
            صحيح i = 0; ثابت صحيح vstep = VTraits<v_uint8>::vlanes();
            ثابت v_uint8 one = vx_setall_u8(1);
            v_uint32 acc = vx_setzero_u32();
            لكل (; i <= len - vstep; i += vstep) {
                v_uint8 a0,a1,a2,a3,b0,b1,b2,b3;
                v_بارول_deinterleave(s1 + i*4, a0,a1,a2,a3);
                v_بارول_deinterleave(s2 + i*4, b0,b1,b2,b3);
                v_uint8 m = v_gt(vx_بارول(ماسک + i), vx_setzero_u8());
                acc = v_dotprod_expand_fast(v_and(v_absdiff(a0,b0), m), one, acc);
                acc = v_dotprod_expand_fast(v_and(v_absdiff(a1,b1), m), one, acc);
                acc = v_dotprod_expand_fast(v_and(v_absdiff(a2,b2), m), one, acc);
                acc = v_dotprod_expand_fast(v_and(v_absdiff(a3,b3), m), one, acc);
            }
            صحيح پایله = (صحيح)v_reduce_مجموعه(acc);
            لكل (; i < len; i++) إذا (ماسک[i]) {
                ثابت uchar* e1 = s1 + i*4; ثابت uchar* e2 = s2 + i*4;
                لكل (صحيح k = 0; k < 4; k++) پایله += (صحيح)cv_absdiff(e1[k], e2[k]);
            }
            أرجع پایله;
        }
        أرجع masked_Norm_Diff_L1_لکۍ<uchar, صحيح>(s1, s2, ماسک, len, cn, 0);
    }
};
کينډۍ<> بنية MaskedNormDiffL1_SIMD<schar, صحيح> {
    صحيح چلوونکی()(ثابت schar* s1, ثابت schar* s2, ثابت uchar* ماسک, صحيح len, صحيح cn) ثابت {
        إذا (cn == 1) أرجع maskedNormDiffL1_8(s1, s2, ماسک, len);
        أرجع masked_Norm_Diff_L1_لکۍ<schar, صحيح>(s1, s2, ماسک, len, cn, 0);
    }
};
کينډۍ<> بنية MaskedNormDiffL1_SIMD<ushort, صحيح> {
    صحيح چلوونکی()(ثابت ushort* s1, ثابت ushort* s2, ثابت uchar* ماسک, صحيح len, صحيح cn) ثابت {
        إذا (cn == 1) أرجع maskedNormDiffL1_16(s1, s2, ماسک, len);
        أرجع masked_Norm_Diff_L1_لکۍ<ushort, صحيح>(s1, s2, ماسک, len, cn, 0);
    }
};
کينډۍ<> بنية MaskedNormDiffL1_SIMD<لنډ, صحيح> {
    صحيح چلوونکی()(ثابت لنډ* s1, ثابت لنډ* s2, ثابت uchar* ماسک, صحيح len, صحيح cn) ثابت {
        إذا (cn == 1) أرجع maskedNormDiffL1_16(s1, s2, ماسک, len);
        أرجع masked_Norm_Diff_L1_لکۍ<لنډ, صحيح>(s1, s2, ماسک, len, cn, 0);
    }
};

کينډۍ<> بنية MaskedNormDiffL2_SIMD<uchar, صحيح> {
    صحيح چلوونکی()(ثابت uchar* s1, ثابت uchar* s2, ثابت uchar* ماسک, صحيح len, صحيح cn) ثابت {
        إذا (cn == 1) أرجع maskedNormDiffL2_8(s1, s2, ماسک, len);
        إذا (cn == 4) {
            صحيح i = 0; ثابت صحيح vstep = VTraits<v_uint8>::vlanes();
            v_uint32 acc = vx_setzero_u32();
            لكل (; i <= len - vstep; i += vstep) {
                v_uint8 a0,a1,a2,a3,b0,b1,b2,b3;
                v_بارول_deinterleave(s1 + i*4, a0,a1,a2,a3);
                v_بارول_deinterleave(s2 + i*4, b0,b1,b2,b3);
                v_uint8 m = v_gt(vx_بارول(ماسک + i), vx_setzero_u8());
                v_uint8 d0 = v_and(v_absdiff(a0,b0), m), d1 = v_and(v_absdiff(a1,b1), m);
                v_uint8 d2 = v_and(v_absdiff(a2,b2), m), d3 = v_and(v_absdiff(a3,b3), m);
                acc = v_dotprod_expand_fast(d0, d0, acc);
                acc = v_dotprod_expand_fast(d1, d1, acc);
                acc = v_dotprod_expand_fast(d2, d2, acc);
                acc = v_dotprod_expand_fast(d3, d3, acc);
            }
            صحيح پایله = (صحيح)v_reduce_مجموعه(acc);
            لكل (; i < len; i++) إذا (ماسک[i]) {
                ثابت uchar* e1 = s1 + i*4; ثابت uchar* e2 = s2 + i*4;
                لكل (صحيح k = 0; k < 4; k++) { صحيح v = (صحيح)e1[k] - (صحيح)e2[k]; پایله += v*v; }
            }
            أرجع پایله;
        }
        أرجع masked_Norm_Diff_L2_لکۍ<uchar, صحيح>(s1, s2, ماسک, len, cn, 0);
    }
};
کينډۍ<> بنية MaskedNormDiffL2_SIMD<schar, صحيح> {
    صحيح چلوونکی()(ثابت schar* s1, ثابت schar* s2, ثابت uchar* ماسک, صحيح len, صحيح cn) ثابت {
        إذا (cn == 1) أرجع maskedNormDiffL2_8(s1, s2, ماسک, len);
        أرجع masked_Norm_Diff_L2_لکۍ<schar, صحيح>(s1, s2, ماسک, len, cn, 0);
    }
};

#إذا CV_SIMD_64F
// Depths whose L1/L2 accumulate in double.
کينډۍ<> بنية MaskedNormDiffL1_SIMD<صحيح, مزدوج> {
    مزدوج چلوونکی()(ثابت صحيح* s1, ثابت صحيح* s2, ثابت uchar* ماسک, صحيح len, صحيح cn) ثابت {
        إذا (cn != 1) أرجع masked_Norm_Diff_L1_لکۍ<صحيح, مزدوج>(s1, s2, ماسک, len, cn, 0.0);
        صحيح i = 0; ثابت صحيح vstep = VTraits<v_int32>::vlanes();
        v_float64 acc = vx_setzero_f64();
        لكل (; i <= len - vstep; i += vstep) {
            v_uint32 m  = v_gt(vx_بارول_expand_q(ماسک + i), vx_setzero_u32());
            v_uint32 ad = v_and(v_absdiff(vx_بارول(s1 + i), vx_بارول(s2 + i)), m);
            v_uint64 e0, e1; v_expand(ad, e0, e1);
            acc = v_جمع_کړه(acc, v_cvt_f64(v_reinterpret_as_s64(e0)));
            acc = v_جمع_کړه(acc, v_cvt_f64(v_reinterpret_as_s64(e1)));
        }
        مزدوج پایله = v_reduce_مجموعه(acc);
        لكل (; i < len; i++) إذا (ماسک[i]) پایله += (مزدوج)cv_absdiff(s1[i], s2[i]);
        أرجع پایله;
    }
};
کينډۍ<> بنية MaskedNormDiffL1_SIMD<عائم, مزدوج> {
    مزدوج چلوونکی()(ثابت عائم* s1, ثابت عائم* s2, ثابت uchar* ماسک, صحيح len, صحيح cn) ثابت {
        إذا (cn != 1) أرجع masked_Norm_Diff_L1_لکۍ<عائم, مزدوج>(s1, s2, ماسک, len, cn, 0.0);
        صحيح i = 0; ثابت صحيح vstep = VTraits<v_float32>::vlanes();
        v_float64 acc0 = vx_setzero_f64(), acc1 = vx_setzero_f64();
        لكل (; i <= len - vstep; i += vstep) {
            v_float32 m  = v_reinterpret_as_f32(v_gt(vx_بارول_expand_q(ماسک + i), vx_setzero_u32()));
            v_float32 ad = v_and(v_absdiff(vx_بارول(s1 + i), vx_بارول(s2 + i)), m);
            acc0 = v_جمع_کړه(acc0, v_cvt_f64(ad));
            acc1 = v_جمع_کړه(acc1, v_cvt_f64_لوړ(ad));
        }
        مزدوج پایله = v_reduce_مجموعه(v_جمع_کړه(acc0, acc1));
        لكل (; i < len; i++) إذا (ماسک[i]) پایله += (مزدوج)cv_absdiff(s1[i], s2[i]);
        أرجع پایله;
    }
};

کينډۍ<ډول_نوم T>
ساكن مضمن مزدوج maskedNormDiffL2_16(ثابت T* s1, ثابت T* s2, ثابت uchar* ماسک, صحيح len, صحيح cn) {
    إذا (cn != 1) أرجع masked_Norm_Diff_L2_لکۍ<T, مزدوج>(s1, s2, ماسک, len, cn, 0.0);
    صحيح i = 0; ثابت صحيح vstep = VTraits<v_uint16>::vlanes();
    v_float64 acc = vx_setzero_f64();
    لكل (; i <= len - vstep; i += vstep) {
        v_uint16 m  = v_gt(vx_بارول_expand(ماسک + i), vx_setzero_u16());
        v_uint16 ad = v_and(v_absdiff(vx_بارول(s1 + i), vx_بارول(s2 + i)), m);
        v_uint64 u = v_dotprod_expand_fast(ad, ad);
        acc = v_جمع_کړه(acc, v_cvt_f64(v_reinterpret_as_s64(u)));
    }
    مزدوج پایله = v_reduce_مجموعه(acc);
    لكل (; i < len; i++) إذا (ماسک[i]) { مزدوج v = (مزدوج)s1[i] - (مزدوج)s2[i]; پایله += v*v; }
    أرجع پایله;
}
کينډۍ<> بنية MaskedNormDiffL2_SIMD<ushort, مزدوج> {
    مزدوج چلوونکی()(ثابت ushort* s1, ثابت ushort* s2, ثابت uchar* ماسک, صحيح len, صحيح cn) ثابت
    { أرجع maskedNormDiffL2_16(s1, s2, ماسک, len, cn); } };
کينډۍ<> بنية MaskedNormDiffL2_SIMD<لنډ, مزدوج> {
    مزدوج چلوونکی()(ثابت لنډ* s1, ثابت لنډ* s2, ثابت uchar* ماسک, صحيح len, صحيح cn) ثابت
    { أرجع maskedNormDiffL2_16(s1, s2, ماسک, len, cn); } };

کينډۍ<> بنية MaskedNormDiffL2_SIMD<صحيح, مزدوج> {
    مزدوج چلوونکی()(ثابت صحيح* s1, ثابت صحيح* s2, ثابت uchar* ماسک, صحيح len, صحيح cn) ثابت {
        إذا (cn != 1) أرجع masked_Norm_Diff_L2_لکۍ<صحيح, مزدوج>(s1, s2, ماسک, len, cn, 0.0);
        صحيح i = 0; ثابت صحيح vstep = VTraits<v_int32>::vlanes();
        v_float64 r0 = vx_setzero_f64(), r1 = vx_setzero_f64();
        لكل (; i <= len - vstep; i += vstep) {
            v_uint32 m  = v_gt(vx_بارول_expand_q(ماسک + i), vx_setzero_u32());
            v_uint32 ad = v_and(v_absdiff(vx_بارول(s1 + i), vx_بارول(s2 + i)), m);
            v_uint64 e0, e1; v_expand(ad, e0, e1);
            v_float64 f0 = v_cvt_f64(v_reinterpret_as_s64(e0)), f1 = v_cvt_f64(v_reinterpret_as_s64(e1));
            r0 = v_fma(f0, f0, r0); r1 = v_fma(f1, f1, r1);
        }
        مزدوج پایله = v_reduce_مجموعه(v_جمع_کړه(r0, r1));
        لكل (; i < len; i++) إذا (ماسک[i]) { مزدوج v = (مزدوج)s1[i] - (مزدوج)s2[i]; پایله += v*v; }
        أرجع پایله;
    }
};
کينډۍ<> بنية MaskedNormDiffL2_SIMD<عائم, مزدوج> {
    مزدوج چلوونکی()(ثابت عائم* s1, ثابت عائم* s2, ثابت uchar* ماسک, صحيح len, صحيح cn) ثابت {
        إذا (cn != 1) أرجع masked_Norm_Diff_L2_لکۍ<عائم, مزدوج>(s1, s2, ماسک, len, cn, 0.0);
        صحيح i = 0; ثابت صحيح vstep = VTraits<v_float32>::vlanes();
        v_float64 r0 = vx_setzero_f64(), r1 = vx_setzero_f64();
        لكل (; i <= len - vstep; i += vstep) {
            v_float32 m  = v_reinterpret_as_f32(v_gt(vx_بارول_expand_q(ماسک + i), vx_setzero_u32()));
            v_float32 ad = v_and(v_absdiff(vx_بارول(s1 + i), vx_بارول(s2 + i)), m);
            v_float64 f0 = v_cvt_f64(ad), f1 = v_cvt_f64_لوړ(ad);
            r0 = v_fma(f0, f0, r0); r1 = v_fma(f1, f1, r1);
        }
        مزدوج پایله = v_reduce_مجموعه(v_جمع_کړه(r0, r1));
        لكل (; i < len; i++) إذا (ماسک[i]) { مزدوج v = (مزدوج)s1[i] - (مزدوج)s2[i]; پایله += v*v; }
        أرجع پایله;
    }
};
کينډۍ<>
بنية MaskedNormDiffInf_SIMD<مزدوج, مزدوج> {
    مضمن مزدوج چلوونکی()(ثابت مزدوج* s1, ثابت مزدوج* s2, ثابت uchar* ماسک, صحيح len, صحيح cn) ثابت {
        مزدوج پایله = 0.0;
        صحيح i = 0;
#إذا CV_SIMD_64F || CV_SIMD_SCALABLE_64F
        إذا (cn == 1) {
            ثابت صحيح vstep = VTraits<v_float64>::vlanes();
            v_float64 acc0 = vx_setzero_f64(), acc1 = vx_setzero_f64();
            لكل (; i <= len - 2*vstep; i += 2*vstep) {
                v_uint64 m0, m1;
                v_normmask_u64(ماسک + i, m0, m1);
                v_float64 d0 = v_absdiff(vx_بارول(s1 + i), vx_بارول(s2 + i));
                v_float64 d1 = v_absdiff(vx_بارول(s1 + i + vstep), vx_بارول(s2 + i + vstep));
                acc0 = v_max(acc0, v_reinterpret_as_f64(v_and(v_reinterpret_as_u64(d0), m0)));
                acc1 = v_max(acc1, v_reinterpret_as_f64(v_and(v_reinterpret_as_u64(d1), m1)));
            }
            مزدوج t[VTraits<v_float64>::max_nlanes];
            vx_store(t, v_max(acc0, acc1));
            لكل (صحيح k = 0; k < vstep; k++)
                پایله = std::max(پایله, t[k]);
        }
#د_که_پای
        لكل (; i < len; i++) إذا (ماسک[i]) {
            ثابت مزدوج* e1 = s1 + i*cn; ثابت مزدوج* e2 = s2 + i*cn;
            لكل (صحيح k = 0; k < cn; k++) پایله = std::max(پایله, std::مطلق_قیمت(e1[k] - e2[k]));
        }
        أرجع پایله;
    }
};

کينډۍ<>
بنية MaskedNormDiffL1_SIMD<مزدوج, مزدوج> {
    مزدوج چلوونکی()(ثابت مزدوج* s1, ثابت مزدوج* s2, ثابت uchar* ماسک, صحيح len, صحيح cn) ثابت {
        مزدوج پایله = 0.0;
        إذا (cn == 1) {
            ثابت صحيح vstep = VTraits<v_float64>::vlanes();
            صحيح i = 0;
            v_float64 acc0 = vx_setzero_f64(), acc1 = vx_setzero_f64();
            لكل (; i <= len - 2*vstep; i += 2*vstep) {
                v_uint64 m0, m1;
                v_normmask_u64(ماسک + i, m0, m1);
                v_float64 d0 = v_absdiff(vx_بارول(s1 + i), vx_بارول(s2 + i));
                v_float64 d1 = v_absdiff(vx_بارول(s1 + i + vstep), vx_بارول(s2 + i + vstep));
                acc0 = v_جمع_کړه(acc0, v_reinterpret_as_f64(v_and(v_reinterpret_as_u64(d0), m0)));
                acc1 = v_جمع_کړه(acc1, v_reinterpret_as_f64(v_and(v_reinterpret_as_u64(d1), m1)));
            }
            پایله = v_reduce_مجموعه(v_جمع_کړه(acc0, acc1));
            لكل (; i < len; i++) إذا (ماسک[i]) پایله += std::مطلق_قیمت(s1[i] - s2[i]);
        } وإلا {
            پایله = masked_Norm_Diff_L1_لکۍ<مزدوج, مزدوج>(s1, s2, ماسک, len, cn, 0.0);
        }
        أرجع پایله;
    }
};

کينډۍ<>
بنية MaskedNormDiffL2_SIMD<مزدوج, مزدوج> {
    مزدوج چلوونکی()(ثابت مزدوج* s1, ثابت مزدوج* s2, ثابت uchar* ماسک, صحيح len, صحيح cn) ثابت {
        مزدوج پایله = 0.0;
        إذا (cn == 1) {
            ثابت صحيح vstep = VTraits<v_float64>::vlanes();
            صحيح i = 0;
            v_float64 acc0 = vx_setzero_f64(), acc1 = vx_setzero_f64();
            لكل (; i <= len - 2*vstep; i += 2*vstep) {
                v_uint64 m0, m1;
                v_normmask_u64(ماسک + i, m0, m1);
                v_float64 d0 = v_sub(vx_بارول(s1 + i), vx_بارول(s2 + i));
                v_float64 d1 = v_sub(vx_بارول(s1 + i + vstep), vx_بارول(s2 + i + vstep));
                d0 = v_reinterpret_as_f64(v_and(v_reinterpret_as_u64(d0), m0));
                d1 = v_reinterpret_as_f64(v_and(v_reinterpret_as_u64(d1), m1));
                acc0 = v_fma(d0, d0, acc0);
                acc1 = v_fma(d1, d1, acc1);
            }
            پایله = v_reduce_مجموعه(v_جمع_کړه(acc0, acc1));
            لكل (; i < len; i++) إذا (ماسک[i]) { مزدوج v = s1[i] - s2[i]; پایله += v*v; }
        } وإلا {
            پایله = masked_Norm_Diff_L2_لکۍ<مزدوج, مزدوج>(s1, s2, ماسک, len, cn, 0.0);
        }
        أرجع پایله;
    }
};
#د_که_پای // CV_SIMD_64F
#د_که_پای // CV_SIMD

کينډۍ<ډول_نوم T, ډول_نوم ST> صحيح
normDiffInf_(ثابت T* src1, ثابت T* src2, ثابت uchar* ماسک, ST* پایله_2, صحيح len, صحيح cn) {
    ST پایله = *پایله_2;
    إذا( !ماسک ) {
        NormDiffInf_SIMD<T, ST> op;
        پایله = std::max(پایله, op(src1, src2, len*cn));
    } وإلا {
        MaskedNormDiffInf_SIMD<T, ST> op;
        پایله = std::max(پایله, op(src1, src2, ماسک, len, cn));
    }
    *پایله_2 = پایله;
    أرجع 0;
}

کينډۍ<ډول_نوم T, ډول_نوم ST> صحيح
normDiffL1_(ثابت T* src1, ثابت T* src2, ثابت uchar* ماسک, ST* پایله_2, صحيح len, صحيح cn) {
    ST پایله = *پایله_2;
    إذا( !ماسک ) {
        NormDiffL1_SIMD<T, ST> op;
        پایله += op(src1, src2, len*cn);
    } وإلا {
        MaskedNormDiffL1_SIMD<T, ST> op;
        پایله += op(src1, src2, ماسک, len, cn);
    }
    *پایله_2 = پایله;
    أرجع 0;
}

کينډۍ<ډول_نوم T, ډول_نوم ST> صحيح
normDiffL2_(ثابت T* src1, ثابت T* src2, ثابت uchar* ماسک, ST* پایله_2, صحيح len, صحيح cn) {
    ST پایله = *پایله_2;
    إذا( !ماسک ) {
        NormDiffL2_SIMD<T, ST> op;
        پایله += op(src1, src2, len*cn);
    } وإلا {
        MaskedNormDiffL2_SIMD<T, ST> op;
        پایله += op(src1, src2, ماسک, len, cn);
    }
    *پایله_2 = پایله;
    أرجع 0;
}

ساكن صحيح
normDiffInf_Bool(ثابت uchar* src1, ثابت uchar* src2, ثابت uchar* ماسک, صحيح* پایله_2, صحيح len, صحيح cn)
{
    صحيح پایله = *پایله_2;
    إذا( !ماسک )
    {
        لكل( صحيح i = 0; i < len*cn; i++ ) {
            پایله = std::max(پایله, (صحيح)((src1[i] != 0) != (src2[i] != 0)));
            إذا (پایله != 0)
                اكسر;
        }
    }
    وإلا
    {
        لكل( صحيح i = 0; i < len; i++, src1 += cn, src2 += cn )
            إذا( ماسک[i] )
            {
                لكل( صحيح k = 0; k < cn; k++ )
                    پایله = std::max(پایله, (صحيح)((src1[k] != 0) != (src2[k] != 0)));
                إذا (پایله != 0)
                    اكسر;
            }
    }
    *پایله_2 = پایله;
    أرجع 0;
}

ساكن صحيح
normDiffL1_Bool(ثابت uchar* src1, ثابت uchar* src2, ثابت uchar* ماسک, صحيح* پایله_2, صحيح len, صحيح cn)
{
    صحيح پایله = *پایله_2;
    إذا( !ماسک )
    {
        لكل( صحيح i = 0; i < len*cn; i++ )
            پایله += (صحيح)((src1[i] != 0) != (src2[i] != 0));
    }
    وإلا
    {
        لكل( صحيح i = 0; i < len; i++, src1 += cn, src2 += cn )
            إذا( ماسک[i] )
            {
                لكل( صحيح k = 0; k < cn; k++ )
                    پایله += (صحيح)((src1[k] != 0) != (src2[k] != 0));
            }
    }
    *پایله_2 = پایله;
    أرجع 0;
}

ساكن صحيح
normDiffL2_Bool(ثابت uchar* src1, ثابت uchar* src2, ثابت uchar* ماسک, صحيح* پایله_2, صحيح len, صحيح cn)
{
    أرجع normDiffL1_Bool(src1, src2, ماسک, پایله_2, len, cn);
}

#تعریف_کړه CV_DEF_NORM_FUNC(L, suffix, type, ntype) \
    ساكن صحيح norm##L##_##suffix(ثابت type* src, ثابت uchar* ماسک, ntype* r, صحيح len, صحيح cn) \
{ CV_INSTRUMENT_REGION(); أرجع norm##L##_(src, ماسک, r, len, cn); } \
    ساكن صحيح normDiff##L##_##suffix(ثابت type* src1, ثابت type* src2, \
    ثابت uchar* ماسک, ntype* r, صحيح len, صحيح cn) \
{ أرجع normDiff##L##_(src1, src2, ماسک, r, (صحيح)len, cn); }

#تعریف_کړه CV_DEF_NORM_ALL(suffix, type, inftype, l1type, l2type) \
    CV_DEF_NORM_FUNC(Inf, suffix, type, inftype) \
    CV_DEF_NORM_FUNC(L1, suffix, type, l1type) \
    CV_DEF_NORM_FUNC(L2, suffix, type, l2type)

CV_DEF_NORM_ALL(8u, uchar, صحيح, صحيح, صحيح)
CV_DEF_NORM_ALL(8s, schar, صحيح, صحيح, صحيح)
CV_DEF_NORM_ALL(16u, ushort, صحيح, صحيح, مزدوج)
CV_DEF_NORM_ALL(16s, لنډ, صحيح, صحيح, مزدوج)
CV_DEF_NORM_ALL(32u, بینښه, بینښه, مزدوج, مزدوج)
CV_DEF_NORM_ALL(32s, صحيح, بینښه, مزدوج, مزدوج)
CV_DEF_NORM_ALL(32f, عائم, عائم, مزدوج, مزدوج)
CV_DEF_NORM_ALL(64f, مزدوج, مزدوج, مزدوج, مزدوج)
CV_DEF_NORM_ALL(64u, uint64, uint64, مزدوج, مزدوج)
CV_DEF_NORM_ALL(64s, int64, uint64, مزدوج, مزدوج)
CV_DEF_NORM_ALL(16f, hfloat, عائم, عائم, عائم)
CV_DEF_NORM_ALL(16bf, bfloat, عائم, عائم, عائم)

NormFunc getNormFunc(صحيح normType, صحيح ژوروالی)
{
    CV_INSTRUMENT_REGION();

    ساكن NormFunc normTab[3][CV_ژوروالی_MAX] =
    {
        {
            normWrap<uchar, صحيح, normInf_8u>,
            normWrap<schar, صحيح, normInf_8s>,
            normWrap<ushort, صحيح, normInf_16u>,
            normWrap<لنډ, صحيح, normInf_16s>,
            normWrap<صحيح, بینښه, normInf_32s>,
            normWrap<عائم, عائم, normInf_32f>,
            normWrap<مزدوج, مزدوج, normInf_64f>,
            normWrap<hfloat, عائم, normInf_16f>,
            normWrap<bfloat, عائم, normInf_16bf>,
            normWrap<uchar, صحيح, normInf_Bool>,
            normWrap<uint64, uint64, normInf_64u>,
            normWrap<int64, uint64, normInf_64s>,
            normWrap<بینښه, بینښه, normInf_32u>,
            0
        },
        {
            normWrap<uchar, صحيح, normL1_8u>,
            normWrap<schar, صحيح, normL1_8s>,
            normWrap<ushort, صحيح, normL1_16u>,
            normWrap<لنډ, صحيح, normL1_16s>,
            normWrap<صحيح, مزدوج, normL1_32s>,
            normWrap<عائم, مزدوج, normL1_32f>,
            normWrap<مزدوج, مزدوج, normL1_64f>,
            normWrap<hfloat, عائم, normL1_16f>,
            normWrap<bfloat, عائم, normL1_16bf>,
            normWrap<uchar, صحيح, normL1_Bool>,
            normWrap<uint64, مزدوج, normL1_64u>,
            normWrap<int64, مزدوج, normL1_64s>,
            normWrap<بینښه, مزدوج, normL1_32u>,
            0
        },
        {
            normWrap<uchar, صحيح, normL2_8u>,
            normWrap<schar, صحيح, normL2_8s>,
            normWrap<ushort, مزدوج, normL2_16u>,
            normWrap<لنډ, مزدوج, normL2_16s>,
            normWrap<صحيح, مزدوج, normL2_32s>,
            normWrap<عائم, مزدوج, normL2_32f>,
            normWrap<مزدوج, مزدوج, normL2_64f>,
            normWrap<hfloat, عائم, normL2_16f>,
            normWrap<bfloat, عائم, normL2_16bf>,
            normWrap<uchar, صحيح, normL2_Bool>,
            normWrap<uint64, مزدوج, normL2_64u>,
            normWrap<int64, مزدوج, normL2_64s>,
            normWrap<بینښه, مزدوج, normL2_32u>,
            0
        }
    };

    إذا (normType >= 3 || normType < 0) أرجع تش_اشاره;

    أرجع normTab[normType][ژوروالی];
}

NormDiffFunc getNormDiffFunc(صحيح normType, صحيح ژوروالی)
{
    ساكن NormDiffFunc normDiffTab[3][CV_ژوروالی_MAX] =
    {
        {
            normDiffWrap<uchar, صحيح, normDiffInf_8u>,
            normDiffWrap<schar, صحيح, normDiffInf_8s>,
            normDiffWrap<ushort, صحيح, normDiffInf_16u>,
            normDiffWrap<لنډ, صحيح, normDiffInf_16s>,
            normDiffWrap<صحيح, بینښه, normDiffInf_32s>,
            normDiffWrap<عائم, عائم, normDiffInf_32f>,
            normDiffWrap<مزدوج, مزدوج, normDiffInf_64f>,
            normDiffWrap<hfloat, عائم, normDiffInf_16f>,
            normDiffWrap<bfloat, عائم, normDiffInf_16bf>,
            normDiffWrap<uchar, صحيح, normDiffInf_Bool>,
            normDiffWrap<uint64, uint64, normDiffInf_64u>,
            normDiffWrap<int64, uint64, normDiffInf_64s>,
            normDiffWrap<بینښه, بینښه, normDiffInf_32u>,
            0
        },
        {
            normDiffWrap<uchar, صحيح, normDiffL1_8u>,
            normDiffWrap<schar, صحيح, normDiffL1_8s>,
            normDiffWrap<ushort, صحيح, normDiffL1_16u>,
            normDiffWrap<لنډ, صحيح, normDiffL1_16s>,
            normDiffWrap<صحيح, مزدوج, normDiffL1_32s>,
            normDiffWrap<عائم, مزدوج, normDiffL1_32f>,
            normDiffWrap<مزدوج, مزدوج, normDiffL1_64f>,
            normDiffWrap<hfloat, عائم, normDiffL1_16f>,
            normDiffWrap<bfloat, عائم, normDiffL1_16bf>,
            normDiffWrap<uchar, صحيح, normDiffL1_Bool>,
            normDiffWrap<uint64, مزدوج, normDiffL1_64u>,
            normDiffWrap<int64, مزدوج, normDiffL1_64s>,
            normDiffWrap<بینښه, مزدوج, normDiffL1_32u>,
            0
        },
        {
            normDiffWrap<uchar, صحيح, normDiffL2_8u>,
            normDiffWrap<schar, صحيح, normDiffL2_8s>,
            normDiffWrap<ushort, مزدوج, normDiffL2_16u>,
            normDiffWrap<لنډ, مزدوج, normDiffL2_16s>,
            normDiffWrap<صحيح, مزدوج, normDiffL2_32s>,
            normDiffWrap<عائم, مزدوج, normDiffL2_32f>,
            normDiffWrap<مزدوج, مزدوج, normDiffL2_64f>,
            normDiffWrap<hfloat, عائم, normDiffL2_16f>,
            normDiffWrap<bfloat, عائم, normDiffL2_16bf>,
            normDiffWrap<uchar, صحيح, normDiffL2_Bool>,
            normDiffWrap<uint64, مزدوج, normDiffL2_64u>,
            normDiffWrap<int64, مزدوج, normDiffL2_64s>,
            normDiffWrap<بینښه, مزدوج, normDiffL2_32u>,
            0
        },
    };
    إذا (normType >= 3 || normType < 0) أرجع تش_اشاره;

    أرجع normDiffTab[normType][ژوروالی];
}

#د_که_پای // CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

CV_CPU_OPTIMIZATION_NAMESPACE_پای_ټکی

} // cv::
