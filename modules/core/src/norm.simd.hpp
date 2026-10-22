// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html
//
// Copyright (C) 2025, SpaceMIT Inc., all rights reserved.
// Copyright (C) 2026, Advanced Micro Devices, Inc., all rights reserved.
// Third party copyrights are property of their respective owners.

#شامل "precomp.hpp"

نامخلا cv {

استعمال NormFunc = صحیح (*)(ثابت uchar*, ثابت uchar*, uchar*, صحیح, صحیح);
استعمال NormDiffFunc = صحیح (*)(ثابت uchar*, ثابت uchar*, ثابت uchar*, uchar*, صحیح, صحیح);

CV_CPU_OPTIMIZATION_NAMESPACE_BEGIN

سانچہ<قسم_نام T, قسم_نام RT, صحیح (*fn)(ثابت T*, ثابت uchar*, RT*, صحیح, صحیح)>
ساکن صحیح normWrap(ثابت uchar* src1, ثابت uchar* src2, uchar* res, صحیح len, صحیح cn)
{
    واپس fn((ثابت T*)src1, src2, (RT*)res, len, cn);
}

سانچہ<قسم_نام T, قسم_نام RT, صحیح (*fn)(ثابت T*, ثابت T*, ثابت uchar*, RT*, صحیح, صحیح)>
ساکن صحیح normDiffWrap(ثابت uchar* src1, ثابت uchar* src2, ثابت uchar* src3, uchar* res, صحیح len, صحیح cn)
{
    واپس fn((ثابت T*)src1, (ثابت T*)src2, src3, (RT*)res, len, cn);
}

NormFunc getNormFunc(صحیح normType, صحیح گہرائی);
NormDiffFunc getNormDiffFunc(صحیح normType, صحیح گہرائی);

#اگر_غیر_تعریف_شدہ CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

سانچہ <قسم_نام T, قسم_نام ST>
ساخت NormInf_SIMD {
    اندرونی ST عامل() (ثابت T* src, صحیح n) ثابت {
        ST s = 0;
        برائے (صحیح i = 0; i < n; i++) {
            s = std::max(s, (ST)cv_abs(src[i]));
        }
        واپس s;
    }
};

سانچہ <قسم_نام T, قسم_نام ST>
ساخت NormL1_SIMD {
    اندرونی ST عامل() (ثابت T* src, صحیح n) ثابت {
        ST s = 0;
        برائے (صحیح i = 0; i < n; i++) {
            s += cv_abs(src[i]);
        }
        واپس s;
    }
};

سانچہ <قسم_نام T, قسم_نام ST>
ساخت NormL2_SIMD {
    اندرونی ST عامل() (ثابت T* src, صحیح n) ثابت {
        ST s = 0;
        برائے (صحیح i = 0; i < n; i++) {
            ST v = (ST)src[i];
            s += v * v;
        }
        واپس s;
    }
};

سانچہ <قسم_نام T, قسم_نام ST>
ساخت NormDiffInf_SIMD {
    اندرونی ST عامل() (ثابت T* src1, ثابت T* src2, صحیح n) ثابت {
        ST s = 0;
        برائے (صحیح i = 0; i < n; i++) {
            ST v = (ST)cv_absdiff(src1[i], src2[i]);
            s = std::max(s, v);
        }
        واپس s;
    }
};

سانچہ <قسم_نام T, قسم_نام ST>
ساخت NormDiffL1_SIMD {
    اندرونی ST عامل() (ثابت T* src1, ثابت T* src2, صحیح n) ثابت {
        ST s = 0;
        برائے (صحیح i = 0; i < n; i++) {
            ST v = (ST)cv_absdiff(src1[i], src2[i]);
            s += v;
        }
        واپس s;
    }
};

سانچہ <قسم_نام T, قسم_نام ST>
ساخت NormDiffL2_SIMD {
    اندرونی ST عامل() (ثابت T* src1, ثابت T* src2, صحیح n) ثابت {
        ST s = 0;
        برائے (صحیح i = 0; i < n; i++) {
            ST v = (ST)src1[i] - (ST)src2[i];
            s += v * v;
        }
        واپس s;
    }
};

#اگر (CV_SIMD || CV_SIMD_SCALABLE)

سانچہ<>
ساخت NormInf_SIMD<uchar, صحیح> {
    صحیح عامل() (ثابت uchar* src, صحیح n) ثابت {
        صحیح j = 0;
        صحیح s = 0;
        v_uint8 r0 = vx_setzero_u8(), r1 = vx_setzero_u8();
        v_uint8 r2 = vx_setzero_u8(), r3 = vx_setzero_u8();
        برائے (; j <= n - 4 * VTraits<v_uint8>::vlanes(); j += 4 * VTraits<v_uint8>::vlanes()) {
            r0 = v_max(r0, vx_بھرو(src + j                                 ));
            r1 = v_max(r1, vx_بھرو(src + j +     VTraits<v_uint8>::vlanes()));
            r2 = v_max(r2, vx_بھرو(src + j + 2 * VTraits<v_uint8>::vlanes()));
            r3 = v_max(r3, vx_بھرو(src + j + 3 * VTraits<v_uint8>::vlanes()));
        }
        r0 = v_max(r0, v_max(r1, v_max(r2, r3)));
        برائے (; j < n; j++) {
            s = std::max(s, (صحیح)src[j]);
        }
        واپس std::max(s, (صحیح)v_reduce_max(r0));
    }
};

سانچہ<>
ساخت NormInf_SIMD<schar, صحیح> {
    صحیح عامل() (ثابت schar* src, صحیح n) ثابت {
        صحیح j = 0;
        صحیح s = 0;
        v_uint8 r0 = vx_setzero_u8(), r1 = vx_setzero_u8();
        v_uint8 r2 = vx_setzero_u8(), r3 = vx_setzero_u8();
        برائے (; j <= n - 4 * VTraits<v_int8>::vlanes(); j += 4 * VTraits<v_int8>::vlanes()) {
            r0 = v_max(r0, v_abs(vx_بھرو(src + j                                )));
            r1 = v_max(r1, v_abs(vx_بھرو(src + j +     VTraits<v_int8>::vlanes())));
            r2 = v_max(r2, v_abs(vx_بھرو(src + j + 2 * VTraits<v_int8>::vlanes())));
            r3 = v_max(r3, v_abs(vx_بھرو(src + j + 3 * VTraits<v_int8>::vlanes())));
        }
        r0 = v_max(r0, v_max(r1, v_max(r2, r3)));
        برائے (; j < n; j++) {
            s = std::max(s, cv_abs(src[j]));
        }
        واپس std::max(s, saturate_cast<صحیح>(v_reduce_max(r0)));
    }
};

سانچہ<>
ساخت NormInf_SIMD<ushort, صحیح> {
    صحیح عامل() (ثابت ushort* src, صحیح n) ثابت {
        صحیح j = 0;
        صحیح s = 0;
        v_uint16 d0 = vx_setzero_u16(), d1 = vx_setzero_u16();
        v_uint16 d2 = vx_setzero_u16(), d3 = vx_setzero_u16();
        برائے (; j <= n - 4 * VTraits<v_uint16>::vlanes(); j += 4 * VTraits<v_uint16>::vlanes()) {
            d0 = v_max(d0, vx_بھرو(src + j                                  ));
            d1 = v_max(d1, vx_بھرو(src + j +     VTraits<v_uint16>::vlanes()));
            d2 = v_max(d2, vx_بھرو(src + j + 2 * VTraits<v_uint16>::vlanes()));
            d3 = v_max(d3, vx_بھرو(src + j + 3 * VTraits<v_uint16>::vlanes()));
        }
        d0 = v_max(d0, v_max(d1, v_max(d2, d3)));
        برائے (; j < n; j++) {
            s = std::max(s, (صحیح)src[j]);
        }
        واپس std::max(s, (صحیح)v_reduce_max(d0));
    }
};

سانچہ<>
ساخت NormInf_SIMD<مختصر, صحیح> {
    صحیح عامل() (ثابت مختصر* src, صحیح n) ثابت {
        صحیح j = 0;
        صحیح s = 0;
        v_uint16 d0 = vx_setzero_u16(), d1 = vx_setzero_u16();
        v_uint16 d2 = vx_setzero_u16(), d3 = vx_setzero_u16();
        برائے (; j <= n - 4 * VTraits<v_int16>::vlanes(); j += 4 * VTraits<v_int16>::vlanes()) {
            d0 = v_max(d0, v_abs(vx_بھرو(src + j                                  )));
            d1 = v_max(d1, v_abs(vx_بھرو(src + j +     VTraits<v_int16>::vlanes())));
            d2 = v_max(d2, v_abs(vx_بھرو(src + j + 2 * VTraits<v_int16>::vlanes())));
            d3 = v_max(d3, v_abs(vx_بھرو(src + j + 3 * VTraits<v_int16>::vlanes())));
        }
        d0 = v_max(d0, v_max(d1, v_max(d2, d3)));
        برائے (; j < n; j++) {
            s = std::max(s, saturate_cast<صحیح>(cv_abs(src[j])));
        }
        واپس std::max(s, saturate_cast<صحیح>(v_reduce_max(d0)));
    }
};

سانچہ<>
ساخت NormInf_SIMD<صحیح, صحیح> {
    صحیح عامل() (ثابت صحیح* src, صحیح n) ثابت {
        صحیح j = 0;
        صحیح s = 0;
        v_uint32 r0 = vx_setzero_u32(), r1 = vx_setzero_u32();
        v_uint32 r2 = vx_setzero_u32(), r3 = vx_setzero_u32();
        برائے (; j <= n - 4 * VTraits<v_int32>::vlanes(); j += 4 * VTraits<v_int32>::vlanes()) {
            r0 = v_max(r0, v_abs(vx_بھرو(src + j                                 )));
            r1 = v_max(r1, v_abs(vx_بھرو(src + j +     VTraits<v_int32>::vlanes())));
            r2 = v_max(r2, v_abs(vx_بھرو(src + j + 2 * VTraits<v_int32>::vlanes())));
            r3 = v_max(r3, v_abs(vx_بھرو(src + j + 3 * VTraits<v_int32>::vlanes())));
        }
        r0 = v_max(r0, v_max(r1, v_max(r2, r3)));
        s = std::max(s, saturate_cast<صحیح>(v_reduce_max(r0)));
        برائے (; j < n; j++) {
            s = std::max(s, std::مطلق_قدر(src[j]));
        }
        واپس s;
    }
};

سانچہ<>
ساخت NormInf_SIMD<اعشاری, اعشاری> {
    اعشاری عامل() (ثابت اعشاری* src, صحیح n) ثابت {
        صحیح j = 0;
        اعشاری s = 0.f;
        v_float32 r0 = vx_setzero_f32(), r1 = vx_setzero_f32();
        v_float32 r2 = vx_setzero_f32(), r3 = vx_setzero_f32();
        برائے (; j <= n - 4 * VTraits<v_float32>::vlanes(); j += 4 * VTraits<v_float32>::vlanes()) {
            r0 = v_max(r0, v_abs(vx_بھرو(src + j                                   )));
            r1 = v_max(r1, v_abs(vx_بھرو(src + j +     VTraits<v_float32>::vlanes())));
            r2 = v_max(r2, v_abs(vx_بھرو(src + j + 2 * VTraits<v_float32>::vlanes())));
            r3 = v_max(r3, v_abs(vx_بھرو(src + j + 3 * VTraits<v_float32>::vlanes())));
        }
        r0 = v_max(r0, v_max(r1, v_max(r2, r3)));
        برائے (; j < n; j++) {
            s = std::max(s, cv_abs(src[j]));
        }
        واپس std::max(s, v_reduce_max(r0));
    }
};

سانچہ<>
ساخت NormL1_SIMD<uchar, صحیح> {
    صحیح عامل() (ثابت uchar* src, صحیح n) ثابت {
        صحیح j = 0;
        صحیح s = 0;
        v_uint32 r0 = vx_setzero_u32(), r1 = vx_setzero_u32();
        v_uint8  one = vx_setall_u8(1);
        برائے (; j<= n - 2 * VTraits<v_uint8>::vlanes(); j += 2 * VTraits<v_uint8>::vlanes()) {
            v_uint8 v0 = vx_بھرو(src + j);
            r0 = v_dotprod_expand_fast(v0, one, r0);

            v_uint8 v1 = vx_بھرو(src + j + VTraits<v_uint8>::vlanes());
            r1 = v_dotprod_expand_fast(v1, one, r1);
        }
        s += v_reduce_مجموع(v_جوڑو(r0, r1));
        برائے (; j < n; j++) {
            s += src[j];
        }
        واپس s;
    }
};

سانچہ<>
ساخت NormL1_SIMD<schar, صحیح> {
    صحیح عامل() (ثابت schar* src, صحیح n) ثابت {
        صحیح j = 0;
        صحیح s = 0;
        v_uint32 r0 = vx_setzero_u32(), r1 = vx_setzero_u32();
        v_uint8  one = vx_setall_u8(1);
        برائے (; j<= n - 2 * VTraits<v_int8>::vlanes(); j += 2 * VTraits<v_int8>::vlanes()) {
            v_uint8 v0 = v_abs(vx_بھرو(src + j));
            r0 = v_dotprod_expand_fast(v0, one, r0);

            v_uint8 v1 = v_abs(vx_بھرو(src + j + VTraits<v_int8>::vlanes()));
            r1 = v_dotprod_expand_fast(v1, one, r1);
        }
        s += v_reduce_مجموع(v_جوڑو(r0, r1));
        برائے (; j < n; j++) {
            s += saturate_cast<صحیح>(cv_abs(src[j]));
        }
        واپس s;
    }
};

سانچہ<>
ساخت NormL1_SIMD<ushort, صحیح> {
    صحیح عامل() (ثابت ushort* src, صحیح n) ثابت {
        صحیح j = 0;
        صحیح s = 0;
        v_uint32 r00 = vx_setzero_u32(), r01 = vx_setzero_u32();
        v_uint32 r10 = vx_setzero_u32(), r11 = vx_setzero_u32();
        برائے (; j<= n - 2 * VTraits<v_uint16>::vlanes(); j += 2 * VTraits<v_uint16>::vlanes()) {
            v_uint16 v0 = vx_بھرو(src + j);
            v_uint32 v00, v01;
            v_expand(v0, v00, v01);
            r00 = v_جوڑو(r00, v00);
            r01 = v_جوڑو(r01, v01);

            v_uint16 v1 = vx_بھرو(src + j + VTraits<v_uint16>::vlanes());
            v_uint32 v10, v11;
            v_expand(v1, v10, v11);
            r10 = v_جوڑو(r10, v10);
            r11 = v_جوڑو(r11, v11);
        }
        s += (صحیح)v_reduce_مجموع(v_جوڑو(v_جوڑو(v_جوڑو(r00, r01), r10), r11));
        برائے (; j < n; j++) {
            s += src[j];
        }
        واپس s;
    }
};

سانچہ<>
ساخت NormL1_SIMD<مختصر, صحیح> {
    صحیح عامل() (ثابت مختصر* src, صحیح n) ثابت {
        صحیح j = 0;
        صحیح s = 0;
        v_uint32 r00 = vx_setzero_u32(), r01 = vx_setzero_u32();
        v_uint32 r10 = vx_setzero_u32(), r11 = vx_setzero_u32();
        برائے (; j<= n - 2 * VTraits<v_int16>::vlanes(); j += 2 * VTraits<v_int16>::vlanes()) {
            v_uint16 v0 = v_abs(vx_بھرو(src + j));
            v_uint32 v00, v01;
            v_expand(v0, v00, v01);
            r00 = v_جوڑو(r00, v00);
            r01 = v_جوڑو(r01, v01);

            v_uint16 v1 = v_abs(vx_بھرو(src + j + VTraits<v_int16>::vlanes()));
            v_uint32 v10, v11;
            v_expand(v1, v10, v11);
            r10 = v_جوڑو(r10, v10);
            r11 = v_جوڑو(r11, v11);
        }
        s += (صحیح)v_reduce_مجموع(v_جوڑو(v_جوڑو(v_جوڑو(r00, r01), r10), r11));
        برائے (; j < n; j++) {
            s += saturate_cast<صحیح>(cv_abs(src[j]));
        }
        واپس s;
    }
};

سانچہ<>
ساخت NormL2_SIMD<uchar, صحیح> {
    صحیح عامل() (ثابت uchar* src, صحیح n) ثابت {
        صحیح j = 0;
        صحیح s = 0;
        v_uint32 r0 = vx_setzero_u32(), r1 = vx_setzero_u32();
        برائے (; j <= n - 2 * VTraits<v_uint8>::vlanes(); j += 2 * VTraits<v_uint8>::vlanes()) {
            v_uint8 v0 = vx_بھرو(src + j);
            r0 = v_dotprod_expand_fast(v0, v0, r0);

            v_uint8 v1 = vx_بھرو(src + j + VTraits<v_uint8>::vlanes());
            r1 = v_dotprod_expand_fast(v1, v1, r1);
        }
        s += v_reduce_مجموع(v_جوڑو(r0, r1));
        برائے (; j < n; j++) {
            صحیح v = saturate_cast<صحیح>(src[j]);
            s += v * v;
        }
        واپس s;
    }
};

سانچہ<>
ساخت NormL2_SIMD<schar, صحیح> {
    صحیح عامل() (ثابت schar* src, صحیح n) ثابت {
        صحیح j = 0;
        صحیح s = 0;
        v_int32 r0 = vx_setzero_s32(), r1 = vx_setzero_s32();
        برائے (; j <= n - 2 * VTraits<v_int8>::vlanes(); j += 2 * VTraits<v_int8>::vlanes()) {
            v_int8 v0 = vx_بھرو(src + j);
            r0 = v_dotprod_expand_fast(v0, v0, r0);
            v_int8 v1 = vx_بھرو(src + j + VTraits<v_int8>::vlanes());
            r1 = v_dotprod_expand_fast(v1, v1, r1);
        }
        s += v_reduce_مجموع(v_جوڑو(r0, r1));
        برائے (; j < n; j++) {
            صحیح v = saturate_cast<صحیح>(src[j]);
            s += v * v;
        }
        واپس s;
    }
};

سانچہ<>
ساخت NormDiffInf_SIMD<uchar, صحیح> {
    صحیح عامل() (ثابت uchar* src1, ثابت uchar* src2, صحیح n) ثابت {
        صحیح j = 0;
        صحیح s = 0;
        v_uint8 r0 = vx_setzero_u8(), r1 = vx_setzero_u8();
        v_uint8 r2 = vx_setzero_u8(), r3 = vx_setzero_u8();
        برائے (; j <= n - 4 * VTraits<v_uint8>::vlanes(); j += 4 * VTraits<v_uint8>::vlanes()) {
            v_uint8 v01 = vx_بھرو(src1 + j), v02 = vx_بھرو(src2 + j);
            r0 = v_max(r0, v_absdiff(v01, v02));

            v_uint8 v11 = vx_بھرو(src1 + j + VTraits<v_uint8>::vlanes()),
                    v12 = vx_بھرو(src2 + j + VTraits<v_uint8>::vlanes());
            r1 = v_max(r1, v_absdiff(v11, v12));

            v_uint8 v21 = vx_بھرو(src1 + j + 2 * VTraits<v_uint8>::vlanes()),
                    v22 = vx_بھرو(src2 + j + 2 * VTraits<v_uint8>::vlanes());
            r2 = v_max(r2, v_absdiff(v21, v22));

            v_uint8 v31 = vx_بھرو(src1 + j + 3 * VTraits<v_uint8>::vlanes()),
                    v32 = vx_بھرو(src2 + j + 3 * VTraits<v_uint8>::vlanes());
            r3 = v_max(r3, v_absdiff(v31, v32));
        }
        s = (صحیح)v_reduce_max(v_max(v_max(v_max(r0, r1), r2), r3));
        برائے (; j < n; j++) {
            صحیح v = (صحیح)cv_absdiff(src1[j], src2[j]);
            s = std::max(s, v);
        }
        واپس s;
    }
};

سانچہ<>
ساخت NormDiffInf_SIMD<schar, صحیح> {
    صحیح عامل() (ثابت schar* src1, ثابت schar* src2, صحیح n) ثابت {
        صحیح j = 0;
        صحیح s = 0;
        v_uint8 r0 = vx_setzero_u8(), r1 = vx_setzero_u8();
        v_uint8 r2 = vx_setzero_u8(), r3 = vx_setzero_u8();
        برائے (; j <= n - 4 * VTraits<v_int8>::vlanes(); j += 4 * VTraits<v_int8>::vlanes()) {
            v_int8 v01 = vx_بھرو(src1 + j), v02 = vx_بھرو(src2 + j);
            r0 = v_max(r0, v_absdiff(v01, v02));

            v_int8 v11 = vx_بھرو(src1 + j + VTraits<v_int8>::vlanes()),
                   v12 = vx_بھرو(src2 + j + VTraits<v_int8>::vlanes());
            r1 = v_max(r1, v_absdiff(v11, v12));

            v_int8 v21 = vx_بھرو(src1 + j + 2 * VTraits<v_int8>::vlanes()),
                   v22 = vx_بھرو(src2 + j + 2 * VTraits<v_int8>::vlanes());
            r2 = v_max(r2, v_absdiff(v21, v22));

            v_int8 v31 = vx_بھرو(src1 + j + 3 * VTraits<v_int8>::vlanes()),
                   v32 = vx_بھرو(src2 + j + 3 * VTraits<v_int8>::vlanes());
            r3 = v_max(r3, v_absdiff(v31, v32));
        }
        s = (صحیح)v_reduce_max(v_max(v_max(v_max(r0, r1), r2), r3));
        برائے (; j < n; j++) {
            صحیح v = (صحیح)cv_absdiff(src1[j], src2[j]);
            s = std::max(s, v);
        }
        واپس s;
    }
};

سانچہ<>
ساخت NormDiffInf_SIMD<ushort, صحیح> {
    صحیح عامل() (ثابت ushort* src1, ثابت ushort* src2, صحیح n) ثابت {
        صحیح j = 0;
        صحیح s = 0;
        v_uint16 r0 = vx_setzero_u16(), r1 = vx_setzero_u16();
        v_uint16 r2 = vx_setzero_u16(), r3 = vx_setzero_u16();
        برائے (; j <= n - 4 * VTraits<v_uint16>::vlanes(); j += 4 * VTraits<v_uint16>::vlanes()) {
            v_uint16 v01 = vx_بھرو(src1 + j), v02 = vx_بھرو(src2 + j);
            r0 = v_max(r0, v_absdiff(v01, v02));

            v_uint16 v11 = vx_بھرو(src1 + j + VTraits<v_uint16>::vlanes()),
                     v12 = vx_بھرو(src2 + j + VTraits<v_uint16>::vlanes());
            r1 = v_max(r1, v_absdiff(v11, v12));

            v_uint16 v21 = vx_بھرو(src1 + j + 2 * VTraits<v_uint16>::vlanes()),
                     v22 = vx_بھرو(src2 + j + 2 * VTraits<v_uint16>::vlanes());
            r2 = v_max(r2, v_absdiff(v21, v22));

            v_uint16 v31 = vx_بھرو(src1 + j + 3 * VTraits<v_uint16>::vlanes()),
                     v32 = vx_بھرو(src2 + j + 3 * VTraits<v_uint16>::vlanes());
            r3 = v_max(r3, v_absdiff(v31, v32));
        }
        s = (صحیح)v_reduce_max(v_max(v_max(v_max(r0, r1), r2), r3));
        برائے (; j < n; j++) {
            صحیح v = (صحیح)cv_absdiff(src1[j], src2[j]);
            s = std::max(s, v);
        }
        واپس s;
    }
};

سانچہ<>
ساخت NormDiffInf_SIMD<مختصر, صحیح> {
    صحیح عامل() (ثابت مختصر* src1, ثابت مختصر* src2, صحیح n) ثابت {
        صحیح j = 0;
        صحیح s = 0;
        v_uint16 r0 = vx_setzero_u16(), r1 = vx_setzero_u16();
        v_uint16 r2 = vx_setzero_u16(), r3 = vx_setzero_u16();
        برائے (; j <= n - 4 * VTraits<v_int16>::vlanes(); j += 4 * VTraits<v_int16>::vlanes()) {
            v_int16 v01 = vx_بھرو(src1 + j), v02 = vx_بھرو(src2 + j);
            r0 = v_max(r0, v_absdiff(v01, v02));

            v_int16 v11 = vx_بھرو(src1 + j + VTraits<v_int16>::vlanes()),
                    v12 = vx_بھرو(src2 + j + VTraits<v_int16>::vlanes());
            r1 = v_max(r1, v_absdiff(v11, v12));

            v_int16 v21 = vx_بھرو(src1 + j + 2 * VTraits<v_int16>::vlanes()),
                    v22 = vx_بھرو(src2 + j + 2 * VTraits<v_int16>::vlanes());
            r2 = v_max(r2, v_absdiff(v21, v22));

            v_int16 v31 = vx_بھرو(src1 + j + 3 * VTraits<v_int16>::vlanes()),
                    v32 = vx_بھرو(src2 + j + 3 * VTraits<v_int16>::vlanes());
            r3 = v_max(r3, v_absdiff(v31, v32));
        }
        s = (صحیح)v_reduce_max(v_max(v_max(v_max(r0, r1), r2), r3));
        برائے (; j < n; j++) {
            صحیح v = (صحیح)cv_absdiff(src1[j], src2[j]);
            s = std::max(s, v);
        }
        واپس s;
    }
};

سانچہ<>
ساخت NormDiffInf_SIMD<صحیح, غیرعلامتی> {
    غیرعلامتی عامل() (ثابت صحیح* src1, ثابت صحیح* src2, صحیح n) ثابت {
        صحیح j = 0;
        غیرعلامتی s = 0;
        v_uint32 r0 = vx_setzero_u32(), r1 = vx_setzero_u32();
        v_uint32 r2 = vx_setzero_u32(), r3 = vx_setzero_u32();
        برائے (; j <= n - 4 * VTraits<v_int32>::vlanes(); j += 4 * VTraits<v_int32>::vlanes()) {
            v_int32 v01 = vx_بھرو(src1 + j), v02 = vx_بھرو(src2 + j);
            r0 = v_max(r0, v_absdiff(v01, v02));

            v_int32 v11 = vx_بھرو(src1 + j + VTraits<v_int32>::vlanes()),
                    v12 = vx_بھرو(src2 + j + VTraits<v_int32>::vlanes());
            r1 = v_max(r1, v_absdiff(v11, v12));

            v_int32 v21 = vx_بھرو(src1 + j + 2 * VTraits<v_int32>::vlanes()),
                    v22 = vx_بھرو(src2 + j + 2 * VTraits<v_int32>::vlanes());
            r2 = v_max(r2, v_absdiff(v21, v22));

            v_int32 v31 = vx_بھرو(src1 + j + 3 * VTraits<v_int32>::vlanes()),
                    v32 = vx_بھرو(src2 + j + 3 * VTraits<v_int32>::vlanes());
            r3 = v_max(r3, v_absdiff(v31, v32));
        }
        s = (صحیح)v_reduce_max(v_max(v_max(v_max(r0, r1), r2), r3));
        برائے (; j < n; j++) {
            غیرعلامتی v = (غیرعلامتی)cv_absdiff(src1[j], src2[j]);
            s = std::max(s, v);
        }
        واپس s;
    }
};

سانچہ<>
ساخت NormDiffInf_SIMD<اعشاری, اعشاری> {
    اعشاری عامل() (ثابت اعشاری* src1, ثابت اعشاری* src2, صحیح n) ثابت {
        صحیح j = 0;
        اعشاری s = 0;
        v_float32 r0 = vx_setzero_f32(), r1 = vx_setzero_f32();
        برائے (; j <= n - 2 * VTraits<v_float32>::vlanes(); j += 2 * VTraits<v_float32>::vlanes()) {
            v_float32 v01 = vx_بھرو(src1 + j), v02 = vx_بھرو(src2 + j);
            r0 = v_max(r0, v_absdiff(v01, v02));

            v_float32 v11 = vx_بھرو(src1 + j + VTraits<v_float32>::vlanes()),
                      v12 = vx_بھرو(src2 + j + VTraits<v_float32>::vlanes());
            r1 = v_max(r1, v_absdiff(v11, v12));
        }
        s = v_reduce_max(v_max(r0, r1));
        برائے (; j < n; j++) {
            اعشاری v = (اعشاری)cv_absdiff(src1[j], src2[j]);
            s = std::max(s, v);
        }
        واپس s;
    }
};

سانچہ<>
ساخت NormDiffL1_SIMD<uchar, صحیح> {
    صحیح عامل() (ثابت uchar* src1, ثابت uchar* src2, صحیح n) ثابت {
        صحیح j = 0;
        صحیح s = 0;
        v_uint32 r0 = vx_setzero_u32(), r1 = vx_setzero_u32();
        v_uint8  one = vx_setall_u8(1);
        برائے (; j<= n - 2 * VTraits<v_uint8>::vlanes(); j += 2 * VTraits<v_uint8>::vlanes()) {
            v_uint8 v01 = vx_بھرو(src1 + j), v02 = vx_بھرو(src2 + j);
            r0 = v_dotprod_expand_fast(v_absdiff(v01, v02), one, r0);

            v_uint8 v11 = vx_بھرو(src1 + j + VTraits<v_uint8>::vlanes()),
                    v12 = vx_بھرو(src2 + j + VTraits<v_uint8>::vlanes());
            r1 = v_dotprod_expand_fast(v_absdiff(v11, v12), one, r1);
        }
        s += v_reduce_مجموع(v_جوڑو(r0, r1));
        برائے (; j < n; j++) {
            صحیح v = (صحیح)cv_absdiff(src1[j], src2[j]);
            s += v;
        }
        واپس s;
    }
};

سانچہ<>
ساخت NormDiffL1_SIMD<schar, صحیح> {
    صحیح عامل() (ثابت schar* src1, ثابت schar* src2, صحیح n) ثابت {
        صحیح j = 0;
        صحیح s = 0;
        v_uint32 r0 = vx_setzero_u32(), r1 = vx_setzero_u32();
        v_uint8  one = vx_setall_u8(1);
        برائے (; j<= n - 2 * VTraits<v_int8>::vlanes(); j += 2 * VTraits<v_int8>::vlanes()) {
            v_int8 v01 = vx_بھرو(src1 + j), v02 = vx_بھرو(src2 + j);
            r0 = v_dotprod_expand_fast(v_absdiff(v01, v02), one, r0);

            v_int8 v11 = vx_بھرو(src1 + j + VTraits<v_int8>::vlanes()),
                   v12 = vx_بھرو(src2 + j + VTraits<v_int8>::vlanes());
            r1 = v_dotprod_expand_fast(v_absdiff(v11, v12), one, r1);
        }
        s += v_reduce_مجموع(v_جوڑو(r0, r1));
        برائے (; j < n; j++) {
            صحیح v = (صحیح)cv_absdiff(src1[j], src2[j]);
            s += v;
        }
        واپس s;
    }
};

سانچہ<>
ساخت NormDiffL1_SIMD<ushort, صحیح> {
    صحیح عامل() (ثابت ushort* src1, ثابت ushort* src2, صحیح n) ثابت {
        صحیح j = 0;
        صحیح s = 0;
        v_uint32 r0 = vx_setzero_u32(), r1 = vx_setzero_u32();
        v_uint32 r2 = vx_setzero_u32(), r3 = vx_setzero_u32();
        برائے (; j<= n - 4 * VTraits<v_uint16>::vlanes(); j += 4 * VTraits<v_uint16>::vlanes()) {
            v_uint16 v01 = vx_بھرو(src1 + j), v02 = vx_بھرو(src2 + j);
            v_uint32 u00, u01;
            v_expand(v_absdiff(v01, v02), u00, u01);
            r0 = v_جوڑو(r0, v_جوڑو(u00, u01));

            v_uint16 v11 = vx_بھرو(src1 + j + VTraits<v_uint16>::vlanes()),
                     v12 = vx_بھرو(src2 + j + VTraits<v_uint16>::vlanes());
            v_uint32 u10, u11;
            v_expand(v_absdiff(v11, v12), u10, u11);
            r1 = v_جوڑو(r1, v_جوڑو(u10, u11));

            v_uint16 v21 = vx_بھرو(src1 + j + 2 * VTraits<v_uint16>::vlanes()),
                     v22 = vx_بھرو(src2 + j + 2 * VTraits<v_uint16>::vlanes());
            v_uint32 u20, u21;
            v_expand(v_absdiff(v21, v22), u20, u21);
            r2 = v_جوڑو(r2, v_جوڑو(u20, u21));

            v_uint16 v31 = vx_بھرو(src1 + j + 3 * VTraits<v_uint16>::vlanes()),
                     v32 = vx_بھرو(src2 + j + 3 * VTraits<v_uint16>::vlanes());
            v_uint32 u30, u31;
            v_expand(v_absdiff(v31, v32), u30, u31);
            r3 = v_جوڑو(r3, v_جوڑو(u30, u31));
        }
        s += (صحیح)v_reduce_مجموع(v_جوڑو(v_جوڑو(v_جوڑو(r0, r1), r2), r3));
        برائے (; j < n; j++) {
            صحیح v = (صحیح)cv_absdiff(src1[j], src2[j]);
            s += v;
        }
        واپس s;
    }
};

سانچہ<>
ساخت NormDiffL1_SIMD<مختصر, صحیح> {
    صحیح عامل() (ثابت مختصر* src1, ثابت مختصر* src2, صحیح n) ثابت {
        صحیح j = 0;
        صحیح s = 0;
        v_uint32 r0 = vx_setzero_u32(), r1 = vx_setzero_u32();
        v_uint32 r2 = vx_setzero_u32(), r3 = vx_setzero_u32();
        برائے (; j<= n - 4 * VTraits<v_int16>::vlanes(); j += 4 * VTraits<v_int16>::vlanes()) {
            v_int16 v01 = vx_بھرو(src1 + j), v02 = vx_بھرو(src2 + j);
            v_uint32 u00, u01;
            v_expand(v_absdiff(v01, v02), u00, u01);
            r0 = v_جوڑو(r0, v_جوڑو(u00, u01));

            v_int16 v11 = vx_بھرو(src1 + j + VTraits<v_int16>::vlanes()),
                    v12 = vx_بھرو(src2 + j + VTraits<v_int16>::vlanes());
            v_uint32 u10, u11;
            v_expand(v_absdiff(v11, v12), u10, u11);
            r1 = v_جوڑو(r1, v_جوڑو(u10, u11));

            v_int16 v21 = vx_بھرو(src1 + j + 2 * VTraits<v_int16>::vlanes()),
                    v22 = vx_بھرو(src2 + j + 2 * VTraits<v_int16>::vlanes());
            v_uint32 u20, u21;
            v_expand(v_absdiff(v21, v22), u20, u21);
            r2 = v_جوڑو(r2, v_جوڑو(u20, u21));

            v_int16 v31 = vx_بھرو(src1 + j + 3 * VTraits<v_int16>::vlanes()),
                    v32 = vx_بھرو(src2 + j + 3 * VTraits<v_int16>::vlanes());
            v_uint32 u30, u31;
            v_expand(v_absdiff(v31, v32), u30, u31);
            r3 = v_جوڑو(r3, v_جوڑو(u30, u31));
        }
        s += (صحیح)v_reduce_مجموع(v_جوڑو(v_جوڑو(v_جوڑو(r0, r1), r2), r3));
        برائے (; j < n; j++) {
            صحیح v = (صحیح)cv_absdiff(src1[j], src2[j]);
            s += v;
        }
        واپس s;
    }
};

سانچہ<>
ساخت NormDiffL2_SIMD<uchar, صحیح> {
    صحیح عامل() (ثابت uchar* src1, ثابت uchar* src2, صحیح n) ثابت {
        صحیح j = 0;
        صحیح s = 0;
        v_uint32 r0 = vx_setzero_u32(), r1 = vx_setzero_u32();
        برائے (; j <= n - 2 * VTraits<v_uint8>::vlanes(); j += 2 * VTraits<v_uint8>::vlanes()) {
            v_uint8 v01 = vx_بھرو(src1 + j), v02 = vx_بھرو(src2 + j);
            v_uint8 v0 = v_absdiff(v01, v02);
            r0 = v_dotprod_expand_fast(v0, v0, r0);

            v_uint8 v11 = vx_بھرو(src1 + j + VTraits<v_uint8>::vlanes()),
                    v12 = vx_بھرو(src2 + j + VTraits<v_uint8>::vlanes());
            v_uint8 v1 = v_absdiff(v11, v12);
            r1 = v_dotprod_expand_fast(v1, v1, r1);
        }
        s += v_reduce_مجموع(v_جوڑو(r0, r1));
        برائے (; j < n; j++) {
            صحیح v = (صحیح)src1[j] - (صحیح)src2[j];
            s += v * v;
        }
        واپس s;
    }
};

سانچہ<>
ساخت NormDiffL2_SIMD<schar, صحیح> {
    صحیح عامل() (ثابت schar* src1, ثابت schar* src2, صحیح n) ثابت {
        صحیح j = 0;
        صحیح s = 0;
        v_uint32 r0 = vx_setzero_u32(), r1 = vx_setzero_u32();
        برائے (; j <= n - 2 * VTraits<v_int8>::vlanes(); j += 2 * VTraits<v_int8>::vlanes()) {
            v_int8 v01 = vx_بھرو(src1 + j), v02 = vx_بھرو(src2 + j);
            v_uint8 v0 = v_absdiff(v01, v02);
            r0 = v_dotprod_expand_fast(v0, v0, r0);

            v_int8 v11 = vx_بھرو(src1 + j + VTraits<v_int8>::vlanes()),
                   v12 = vx_بھرو(src2 + j + VTraits<v_int8>::vlanes());
            v_uint8 v1 = v_absdiff(v11, v12);
            r1 = v_dotprod_expand_fast(v1, v1, r1);
        }
        s += v_reduce_مجموع(v_جوڑو(r0, r1));
        برائے (; j < n; j++) {
            صحیح v = (صحیح)src1[j] - (صحیح)src2[j];
            s += v * v;
        }
        واپس s;
    }
};

#اگر_ختم

#اگر (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)

سانچہ<>
ساخت NormInf_SIMD<دوہرا, دوہرا> {
    دوہرا عامل() (ثابت دوہرا* src, صحیح n) ثابت {
        صحیح j = 0;
        دوہرا s = 0.f;
        v_float64 r0 = vx_setzero_f64(), r1 = vx_setzero_f64();
        v_float64 r2 = vx_setzero_f64(), r3 = vx_setzero_f64();
        برائے (; j <= n - 4 * VTraits<v_float64>::vlanes(); j += 4 * VTraits<v_float64>::vlanes()) {
            r0 = v_max(r0, v_abs(vx_بھرو(src + j                                   )));
            r1 = v_max(r1, v_abs(vx_بھرو(src + j +     VTraits<v_float64>::vlanes())));
            r2 = v_max(r2, v_abs(vx_بھرو(src + j + 2 * VTraits<v_float64>::vlanes())));
            r3 = v_max(r3, v_abs(vx_بھرو(src + j + 3 * VTraits<v_float64>::vlanes())));
        }
        r0 = v_max(r0, v_max(r1, v_max(r2, r3)));
        برائے (; j < n; j++) {
            s = std::max(s, cv_abs(src[j]));
        }
        // [TODO]: use v_reduce_max when it supports float64
        دوہرا t[VTraits<v_float64>::max_nlanes];
        vx_store(t, r0);
        برائے (صحیح i = 0; i < VTraits<v_float64>::vlanes(); i++) {
            s = std::max(s, cv_abs(t[i]));
        }
        واپس s;
    }
};

سانچہ<>
ساخت NormL1_SIMD<صحیح, دوہرا> {
    دوہرا عامل() (ثابت صحیح* src, صحیح n) ثابت {
        صحیح j = 0;
        دوہرا s = 0.f;
        v_float64 r00 = vx_setzero_f64(), r01 = vx_setzero_f64();
        v_float64 r10 = vx_setzero_f64(), r11 = vx_setzero_f64();
        برائے (; j <= n - 2 * VTraits<v_int32>::vlanes(); j += 2 * VTraits<v_int32>::vlanes()) {
            // Widen to double before taking the magnitude: exact over the whole int32 range,
            // INT_MIN included, which a float intermediate would not be above 2^24.
            v_int32 i0 = vx_بھرو(src + j), i1 = vx_بھرو(src + j + VTraits<v_int32>::vlanes());
            r00 = v_جوڑو(r00, v_abs(v_cvt_f64(i0))); r01 = v_جوڑو(r01, v_abs(v_cvt_f64_بلند(i0)));
            r10 = v_جوڑو(r10, v_abs(v_cvt_f64(i1))); r11 = v_جوڑو(r11, v_abs(v_cvt_f64_بلند(i1)));
        }
        s += v_reduce_مجموع(v_جوڑو(v_جوڑو(v_جوڑو(r00, r01), r10), r11));
        برائے (; j < n; j++) {
            s += cv_abs(src[j]);
        }
        واپس s;
    }
};

سانچہ<>
ساخت NormL1_SIMD<اعشاری, دوہرا> {
    دوہرا عامل() (ثابت اعشاری* src, صحیح n) ثابت {
        صحیح j = 0;
        دوہرا s = 0.f;
        v_float64 r00 = vx_setzero_f64(), r01 = vx_setzero_f64();
        v_float64 r10 = vx_setzero_f64(), r11 = vx_setzero_f64();
        v_float64 r20 = vx_setzero_f64(), r21 = vx_setzero_f64();
        v_float64 r30 = vx_setzero_f64(), r31 = vx_setzero_f64();
        برائے (; j <= n - 4 * VTraits<v_float32>::vlanes(); j += 4 * VTraits<v_float32>::vlanes()) {
            v_float32 v0 = v_abs(vx_بھرو(src + j)), v1 = v_abs(vx_بھرو(src + j + VTraits<v_float32>::vlanes()));
            r00 = v_جوڑو(r00, v_cvt_f64(v0)); r01 = v_جوڑو(r01, v_cvt_f64_بلند(v0));
            r10 = v_جوڑو(r10, v_cvt_f64(v1)); r11 = v_جوڑو(r11, v_cvt_f64_بلند(v1));

            v_float32 v2 = v_abs(vx_بھرو(src + j + 2 * VTraits<v_float32>::vlanes())), v3 = v_abs(vx_بھرو(src + j + 3 * VTraits<v_float32>::vlanes()));
            r20 = v_جوڑو(r20, v_cvt_f64(v2)); r21 = v_جوڑو(r21, v_cvt_f64_بلند(v2));
            r30 = v_جوڑو(r30, v_cvt_f64(v3)); r31 = v_جوڑو(r31, v_cvt_f64_بلند(v3));
        }
        s += v_reduce_مجموع(v_جوڑو(v_جوڑو(v_جوڑو(r00, r01), r10), r11));
        s += v_reduce_مجموع(v_جوڑو(v_جوڑو(v_جوڑو(r20, r21), r30), r31));
        برائے (; j < n; j++) {
            s += cv_abs(src[j]);
        }
        واپس s;
    }
};

سانچہ<>
ساخت NormL2_SIMD<ushort, دوہرا> {
    دوہرا عامل() (ثابت ushort* src, صحیح n) ثابت {
        صحیح j = 0;
        دوہرا s = 0.f;
        v_float64 r0 = vx_setzero_f64(), r1 = vx_setzero_f64();
        برائے (; j <= n - 2 * VTraits<v_uint16>::vlanes(); j += 2 * VTraits<v_uint16>::vlanes()) {
            v_uint16 v0 = vx_بھرو(src + j);
            v_uint64 u0 = v_dotprod_expand_fast(v0, v0);
            r0 = v_جوڑو(r0, v_cvt_f64(v_reinterpret_as_s64(u0)));

            v_uint16 v1 = vx_بھرو(src + j + VTraits<v_uint16>::vlanes());
            v_uint64 u1 = v_dotprod_expand_fast(v1, v1);
            r1 = v_جوڑو(r1, v_cvt_f64(v_reinterpret_as_s64(u1)));
        }
        s += v_reduce_مجموع(v_جوڑو(r0, r1));
        برائے (; j < n; j++) {
            دوہرا v = saturate_cast<دوہرا>(src[j]);
            s += v * v;
        }
        واپس s;
    }
};

سانچہ<>
ساخت NormL2_SIMD<مختصر, دوہرا> {
    دوہرا عامل() (ثابت مختصر* src, صحیح n) ثابت {
        صحیح j = 0;
        دوہرا s = 0.f;
        v_float64 r0 = vx_setzero_f64(), r1 = vx_setzero_f64();
        برائے (; j <= n - 2 * VTraits<v_int16>::vlanes(); j += 2 * VTraits<v_int16>::vlanes()) {
            v_int16 v0 = vx_بھرو(src + j);
            r0 = v_جوڑو(r0, v_cvt_f64(v_dotprod_expand_fast(v0, v0)));

            v_int16 v1 = vx_بھرو(src + j + VTraits<v_int16>::vlanes());
            r1 = v_جوڑو(r1, v_cvt_f64(v_dotprod_expand_fast(v1, v1)));
        }
        s += v_reduce_مجموع(v_جوڑو(r0, r1));
        برائے (; j < n; j++) {
            دوہرا v = saturate_cast<دوہرا>(src[j]);
            s += v * v;
        }
        واپس s;
    }
};

سانچہ<>
ساخت NormL2_SIMD<صحیح, دوہرا> {
    دوہرا عامل() (ثابت صحیح* src, صحیح n) ثابت {
        صحیح j = 0;
        دوہرا s = 0.f;
        v_float64 r0 = vx_setzero_f64(), r1 = vx_setzero_f64();
        برائے (; j <= n - 2 * VTraits<v_int32>::vlanes(); j += 2 * VTraits<v_int32>::vlanes()) {
            v_int32 v0 = vx_بھرو(src + j);
            r0 = v_dotprod_expand_fast(v0, v0, r0);

            v_int32 v1 = vx_بھرو(src + j + VTraits<v_int32>::vlanes());
            r1 = v_dotprod_expand_fast(v1, v1, r1);
        }
        s += v_reduce_مجموع(v_جوڑو(r0, r1));
        برائے (; j < n; j++) {
            دوہرا v = src[j];
            s += v * v;
        }
        واپس s;
    }
};

سانچہ<>
ساخت NormL2_SIMD<اعشاری, دوہرا> {
    دوہرا عامل() (ثابت اعشاری* src, صحیح n) ثابت {
        صحیح j = 0;
        دوہرا s = 0.f;
        v_float64 r00 = vx_setzero_f64(), r01 = vx_setzero_f64();
        v_float64 r10 = vx_setzero_f64(), r11 = vx_setzero_f64();
        برائے (; j <= n - 2 * VTraits<v_float32>::vlanes(); j += 2 * VTraits<v_float32>::vlanes()) {
            v_float32 v0 = vx_بھرو(src + j), v1 = vx_بھرو(src + j + VTraits<v_float32>::vlanes());
            v_float64 v00 = v_cvt_f64(v0), v01 = v_cvt_f64_بلند(v0);
            v_float64 v10 = v_cvt_f64(v1), v11 = v_cvt_f64_بلند(v1);
            r00 = v_fma(v00, v00, r00); r01 = v_fma(v01, v01, r01);
            r10 = v_fma(v10, v10, r10); r11 = v_fma(v11, v11, r11);
        }
        s += v_reduce_مجموع(v_جوڑو(v_جوڑو(v_جوڑو(r00, r01), r10), r11));
        برائے (; j < n; j++) {
            دوہرا v = src[j];
            s += v * v;
        }
        واپس s;
    }
};

#اگر_ختم

#اگر CV_SIMD_64F // CV_SIMD_SCALABLE_64F has accuracy problem with the following kernels on ci

سانچہ<>
ساخت NormL1_SIMD<دوہرا, دوہرا> {
    دوہرا عامل() (ثابت دوہرا* src, صحیح n) ثابت {
        صحیح j = 0;
        دوہرا s = 0.f;
        v_float64 r00 = vx_setzero_f64(), r01 = vx_setzero_f64();
        v_float64 r10 = vx_setzero_f64(), r11 = vx_setzero_f64();
        برائے (; j <= n - 4 * VTraits<v_float64>::vlanes(); j += 4 * VTraits<v_float64>::vlanes()) {
            r00 = v_جوڑو(r00, v_abs(vx_بھرو(src + j                                   )));
            r01 = v_جوڑو(r01, v_abs(vx_بھرو(src + j +     VTraits<v_float64>::vlanes())));
            r10 = v_جوڑو(r10, v_abs(vx_بھرو(src + j + 2 * VTraits<v_float64>::vlanes())));
            r11 = v_جوڑو(r11, v_abs(vx_بھرو(src + j + 3 * VTraits<v_float64>::vlanes())));
        }
        s += v_reduce_مجموع(v_جوڑو(v_جوڑو(v_جوڑو(r00, r01), r10), r11));
        برائے (; j < n; j++) {
            s += cv_abs(src[j]);
        }
        واپس s;
    }
};

سانچہ<>
ساخت NormL2_SIMD<دوہرا, دوہرا> {
    دوہرا عامل() (ثابت دوہرا* src, صحیح n) ثابت {
        صحیح j = 0;
        دوہرا s = 0.f;
        v_float64 r00 = vx_setzero_f64(), r01 = vx_setzero_f64();
        v_float64 r10 = vx_setzero_f64(), r11 = vx_setzero_f64();
        برائے (; j <= n - 4 * VTraits<v_float64>::vlanes(); j += 4 * VTraits<v_float64>::vlanes()) {
            v_float64 v00 = vx_بھرو(src + j                                   );
            v_float64 v01 = vx_بھرو(src + j +     VTraits<v_float64>::vlanes());
            v_float64 v10 = vx_بھرو(src + j + 2 * VTraits<v_float64>::vlanes());
            v_float64 v11 = vx_بھرو(src + j + 3 * VTraits<v_float64>::vlanes());
            r00 = v_fma(v00, v00, r00); r01 = v_fma(v01, v01, r01);
            r10 = v_fma(v10, v10, r10); r11 = v_fma(v11, v11, r11);
        }
        s += v_reduce_مجموع(v_جوڑو(v_جوڑو(v_جوڑو(r00, r01), r10), r11));
        برائے (; j < n; j++) {
            دوہرا v = src[j];
            s += v * v;
        }
        واپس s;
    }
};

سانچہ<>
ساخت NormDiffInf_SIMD<دوہرا, دوہرا> {
    دوہرا عامل() (ثابت دوہرا* src1, ثابت دوہرا* src2, صحیح n) ثابت {
        صحیح j = 0;
        دوہرا s = 0;
        v_float64 r0 = vx_setzero_f64(), r1 = vx_setzero_f64();
        برائے (; j <= n - 2 * VTraits<v_float64>::vlanes(); j += 2 * VTraits<v_float64>::vlanes()) {
            v_float64 v01 = vx_بھرو(src1 + j), v02 = vx_بھرو(src2 + j);
            r0 = v_max(r0, v_absdiff(v01, v02));

            v_float64 v11 = vx_بھرو(src1 + j + VTraits<v_float64>::vlanes()),
                      v12 = vx_بھرو(src2 + j + VTraits<v_float64>::vlanes());
            r1 = v_max(r1, v_absdiff(v11, v12));
        }
        // [TODO]: use v_reduce_max when it supports float64
        دوہرا t[VTraits<v_float64>::max_nlanes];
        vx_store(t, v_max(r0, r1));
        برائے (صحیح i = 0; i < VTraits<v_float64>::vlanes(); i++) {
            s = std::max(s, t[i]);
        }
        برائے (; j < n; j++) {
            دوہرا v = (دوہرا)cv_absdiff(src1[j], src2[j]);
            s = std::max(s, v);
        }
        واپس s;
    }
};

سانچہ<>
ساخت NormDiffL1_SIMD<اعشاری, دوہرا> {
    دوہرا عامل() (ثابت اعشاری* src1, ثابت اعشاری* src2, صحیح n) ثابت {
        صحیح j = 0;
        دوہرا s = 0.f;
        v_float64 r0 = vx_setzero_f64(), r1 = vx_setzero_f64();
        v_float64 r2 = vx_setzero_f64(), r3 = vx_setzero_f64();
        برائے (; j <= n - 2 * VTraits<v_float32>::vlanes(); j += 2 * VTraits<v_float32>::vlanes()) {
            v_float32 v01 = vx_بھرو(src1 + j), v02 = vx_بھرو(src2 + j);
            v_float32 v0 = v_absdiff(v01, v02);
            r0 = v_جوڑو(r0, v_cvt_f64(v0)); r1 = v_جوڑو(r1, v_cvt_f64_بلند(v0));

            v_float32 v11 = vx_بھرو(src1 + j + VTraits<v_float32>::vlanes()),
                      v12 = vx_بھرو(src2 + j + VTraits<v_float32>::vlanes());
            v_float32 v1 = v_absdiff(v11, v12);
            r2 = v_جوڑو(r2, v_cvt_f64(v1)); r3 = v_جوڑو(r3, v_cvt_f64_بلند(v1));
        }
        s += v_reduce_مجموع(v_جوڑو(v_جوڑو(v_جوڑو(r0, r1), r2), r3));
        برائے (; j < n; j++) {
            دوہرا v = (دوہرا)cv_absdiff(src1[j], src2[j]);
            s += v;
        }
        واپس s;
    }
};

سانچہ<>
ساخت NormDiffL1_SIMD<دوہرا, دوہرا> {
    دوہرا عامل() (ثابت دوہرا* src1, ثابت دوہرا* src2, صحیح n) ثابت {
        صحیح j = 0;
        دوہرا s = 0.f;
        v_float64 r0 = vx_setzero_f64(), r1 = vx_setzero_f64();
        برائے (; j <= n - 2 * VTraits<v_float64>::vlanes(); j += 2 * VTraits<v_float64>::vlanes()) {
            v_float64 v01 = vx_بھرو(src1 + j), v02 = vx_بھرو(src2 + j);
            r0 = v_جوڑو(r0, v_absdiff(v01, v02));

            v_float64 v11 = vx_بھرو(src1 + j + VTraits<v_float64>::vlanes()),
                      v12 = vx_بھرو(src2 + j + VTraits<v_float64>::vlanes());
            r1 = v_جوڑو(r1, v_absdiff(v11, v12));
        }
        s += v_reduce_مجموع(v_جوڑو(r0, r1));
        برائے (; j < n; j++) {
            دوہرا v = (دوہرا)cv_absdiff(src1[j], src2[j]);
            s += v;
        }
        واپس s;
    }
};

سانچہ<>
ساخت NormDiffL2_SIMD<ushort, دوہرا> {
    دوہرا عامل() (ثابت ushort* src1, ثابت ushort* src2, صحیح n) ثابت {
        صحیح j = 0;
        دوہرا s = 0.f;
        v_float64 r0 = vx_setzero_f64(), r1 = vx_setzero_f64();
        برائے (; j <= n - 2 * VTraits<v_uint16>::vlanes(); j += 2 * VTraits<v_uint16>::vlanes()) {
            v_uint16 v01 = vx_بھرو(src1 + j), v02 = vx_بھرو(src2 + j);
            v_uint16 v0 = v_absdiff(v01, v02);
            v_uint64 u0 = v_dotprod_expand_fast(v0, v0);
            r0 = v_جوڑو(r0, v_cvt_f64(v_reinterpret_as_s64(u0)));

            v_uint16 v11 = vx_بھرو(src1 + j + VTraits<v_uint16>::vlanes()),
                     v12 = vx_بھرو(src2 + j + VTraits<v_uint16>::vlanes());
            v_uint16 v1 = v_absdiff(v11, v12);
            v_uint64 u1 = v_dotprod_expand_fast(v1, v1);
            r1 = v_جوڑو(r1, v_cvt_f64(v_reinterpret_as_s64(u1)));
        }
        s += v_reduce_مجموع(v_جوڑو(r0, r1));
        برائے (; j < n; j++) {
            دوہرا v = (دوہرا)src1[j] - (دوہرا)src2[j];
            s += v * v;
        }
        واپس s;
    }
};

سانچہ<>
ساخت NormDiffL2_SIMD<مختصر, دوہرا> {
    دوہرا عامل() (ثابت مختصر* src1, ثابت مختصر* src2, صحیح n) ثابت {
        صحیح j = 0;
        دوہرا s = 0.f;
        v_float64 r0 = vx_setzero_f64(), r1 = vx_setzero_f64();
        برائے (; j <= n - 2 * VTraits<v_int16>::vlanes(); j += 2 * VTraits<v_int16>::vlanes()) {
            v_int16 v01 = vx_بھرو(src1 + j), v02 = vx_بھرو(src2 + j);
            v_uint16 v0 = v_absdiff(v01, v02);
            v_uint64 u0 = v_dotprod_expand_fast(v0, v0);
            r0 = v_جوڑو(r0, v_cvt_f64(v_reinterpret_as_s64(u0)));

            v_int16 v11 = vx_بھرو(src1 + j + VTraits<v_uint16>::vlanes()),
                    v12 = vx_بھرو(src2 + j + VTraits<v_uint16>::vlanes());
            v_uint16 v1 = v_absdiff(v11, v12);
            v_uint64 u1 = v_dotprod_expand_fast(v1, v1);
            r1 = v_جوڑو(r1, v_cvt_f64(v_reinterpret_as_s64(u1)));
        }
        s += v_reduce_مجموع(v_جوڑو(r0, r1));
        برائے (; j < n; j++) {
            دوہرا v = (دوہرا)src1[j] - (دوہرا)src2[j];
            s += v * v;
        }
        واپس s;
    }
};

سانچہ<>
ساخت NormDiffL2_SIMD<صحیح, دوہرا> {
    دوہرا عامل() (ثابت صحیح* src1, ثابت صحیح* src2, صحیح n) ثابت {
        صحیح j = 0;
        دوہرا s = 0.f;
        v_float64 r0 = vx_setzero_f64(), r1 = vx_setzero_f64();
        برائے (; j <= n - VTraits<v_int32>::vlanes(); j += VTraits<v_int32>::vlanes()) {
            v_int32 v01 = vx_بھرو(src1 + j), v02 = vx_بھرو(src2 + j);
            v_uint32 v0 = v_absdiff(v01, v02);
            v_uint64 ev0, ev1;
            v_expand(v0, ev0, ev1);
            v_float64 f0 = v_cvt_f64(v_reinterpret_as_s64(ev0)), f1 = v_cvt_f64(v_reinterpret_as_s64(ev1));
            r0 = v_fma(f0, f0, r0); r1 = v_fma(f1, f1, r1);
        }
        s += v_reduce_مجموع(v_جوڑو(r0, r1));
        برائے (; j < n; j++) {
            دوہرا v = (دوہرا)src1[j] - (دوہرا)src2[j];
            s += v * v;
        }
        واپس s;
    }
};

سانچہ<>
ساخت NormDiffL2_SIMD<اعشاری, دوہرا> {
    دوہرا عامل() (ثابت اعشاری* src1, ثابت اعشاری* src2, صحیح n) ثابت {
        صحیح j = 0;
        دوہرا s = 0.f;
        v_float64 r0 = vx_setzero_f64(), r1 = vx_setzero_f64();
        v_float64 r2 = vx_setzero_f64(), r3 = vx_setzero_f64();
        برائے (; j <= n - 2 * VTraits<v_float32>::vlanes(); j += 2 * VTraits<v_float32>::vlanes()) {
            v_float32 v01 = vx_بھرو(src1 + j), v02 = vx_بھرو(src2 + j);
            v_float32 v0 = v_absdiff(v01, v02);
            v_float64 f01 = v_cvt_f64(v0), f02 = v_cvt_f64_بلند(v0);
            r0 = v_fma(f01, f01, r0); r1 = v_fma(f02, f02, r1);

            v_float32 v11 = vx_بھرو(src1 + j + VTraits<v_float32>::vlanes()),
                      v12 = vx_بھرو(src2 + j + VTraits<v_float32>::vlanes());
            v_float32 v1 = v_absdiff(v11, v12);
            v_float64 f11 = v_cvt_f64(v1), f12 = v_cvt_f64_بلند(v1);
            r2 = v_fma(f11, f11, r2); r3 = v_fma(f12, f12, r3);
        }
        s += v_reduce_مجموع(v_جوڑو(v_جوڑو(v_جوڑو(r0, r1), r2), r3));
        برائے (; j < n; j++) {
            دوہرا v = (دوہرا)src1[j] - (دوہرا)src2[j];
            s += v * v;
        }
        واپس s;
    }
};

سانچہ<>
ساخت NormDiffL2_SIMD<دوہرا, دوہرا> {
    دوہرا عامل() (ثابت دوہرا* src1, ثابت دوہرا* src2, صحیح n) ثابت {
        صحیح j = 0;
        دوہرا s = 0.f;
        v_float64 r0 = vx_setzero_f64(), r1 = vx_setzero_f64();
        v_float64 r2 = vx_setzero_f64(), r3 = vx_setzero_f64();
        برائے (; j <= n - 4 * VTraits<v_float64>::vlanes(); j += 4 * VTraits<v_float64>::vlanes()) {
            v_float64 v01 = vx_بھرو(src1 + j), v02 = vx_بھرو(src2 + j);
            v_float64 v0 = v_absdiff(v01, v02);
            r0 = v_fma(v0, v0, r0);

            v_float64 v11 = vx_بھرو(src1 + j + VTraits<v_float64>::vlanes()),
                      v12 = vx_بھرو(src2 + j + VTraits<v_float64>::vlanes());
            v_float64 v1 = v_absdiff(v11, v12);
            r1 = v_fma(v1, v1, r1);

            v_float64 v21 = vx_بھرو(src1 + j + 2 * VTraits<v_float64>::vlanes()),
                      v22 = vx_بھرو(src2 + j + 2 * VTraits<v_float64>::vlanes());
            v_float64 v2 = v_absdiff(v21, v22);
            r2 = v_fma(v2, v2, r2);

            v_float64 v31 = vx_بھرو(src1 + j + 3 * VTraits<v_float64>::vlanes()),
                      v32 = vx_بھرو(src2 + j + 3 * VTraits<v_float64>::vlanes());
            v_float64 v3 = v_absdiff(v31, v32);
            r3 = v_fma(v3, v3, r3);
        }
        s += v_reduce_مجموع(v_جوڑو(v_جوڑو(v_جوڑو(r0, r1), r2), r3));
        برائے (; j < n; j++) {
            دوہرا v = (دوہرا)src1[j] - (دوہرا)src2[j];
            s += v * v;
        }
        واپس s;
    }
};

#اگر_ختم

سانچہ <قسم_نام T, قسم_نام ST>
ساخت MaskedNormInf_SIMD {
    اندرونی ST عامل() (ثابت T* src, ثابت uchar* نقاب, صحیح len, صحیح cn) ثابت {
        ST s = 0;
        اگر (cn == 1) {
            برائے (صحیح i = 0; i < len; i++) {
                اگر (نقاب[i]) {
                    s = std::max(s, (ST)cv_abs(src[i]));
                }
            }
        }
        ورنہ {
            برائے (صحیح i = 0; i < len; i++) {
                اگر (نقاب[i]) {
                    ثابت T* elem = src + i * cn;
                    صحیح k = 0;
                #اگر CV_ENABLE_UNROLLED && !CV_SIMD_SCALABLE
                    برائے (; k <= cn - 4; k += 4) {
                        s = std::max(s, (ST)cv_abs(elem[k]));
                        s = std::max(s, (ST)cv_abs(elem[k + 1]));
                        s = std::max(s, (ST)cv_abs(elem[k + 2]));
                        s = std::max(s, (ST)cv_abs(elem[k + 3]));
                    }
                #اگر_ختم
                    برائے (; k < cn; k++) {
                        s = std::max(s, (ST)cv_abs(elem[k]));
                    }
                }
            }
        }
        واپس s;
    }
};

سانچہ <قسم_نام T, قسم_نام ST>
ساخت MaskedNormL1_SIMD {
    اندرونی ST عامل() (ثابت T* src, ثابت uchar* نقاب, صحیح len, صحیح cn) ثابت {
        ST s = 0;
        اگر (cn == 1) {
            برائے (صحیح i = 0; i < len; i++) {
                اگر (نقاب[i]) {
                    s += (ST)cv_abs(src[i]);
                }
            }
        }
        ورنہ {
            برائے (صحیح i = 0; i < len; i++) {
                اگر (نقاب[i]) {
                    ثابت T* elem = src + i * cn;
                    صحیح k = 0;
                #اگر CV_ENABLE_UNROLLED && !CV_SIMD_SCALABLE
                    برائے (; k <= cn - 4; k += 4) {
                        s += (ST)cv_abs(elem[k]);
                        s += (ST)cv_abs(elem[k + 1]);
                        s += (ST)cv_abs(elem[k + 2]);
                        s += (ST)cv_abs(elem[k + 3]);
                    }
                #اگر_ختم
                    برائے (; k < cn; k++) {
                        s += (ST)cv_abs(elem[k]);
                    }
                }
            }
        }
        واپس s;
    }
};

سانچہ <قسم_نام T, قسم_نام ST>
ساخت MaskedNormL2_SIMD {
    اندرونی ST عامل() (ثابت T* src, ثابت uchar* نقاب, صحیح len, صحیح cn) ثابت {
        ST s = 0;
        اگر (cn == 1) {
            صحیح i = 0;
        #اگر CV_ENABLE_UNROLLED && !CV_SIMD_SCALABLE
            برائے (; i <= len - 4; i += 4) {
                اگر (نقاب[i])     { T v0 = src[i];     s += (ST)v0 * v0; }
                اگر (نقاب[i + 1]) { T v1 = src[i + 1]; s += (ST)v1 * v1; }
                اگر (نقاب[i + 2]) { T v2 = src[i + 2]; s += (ST)v2 * v2; }
                اگر (نقاب[i + 3]) { T v3 = src[i + 3]; s += (ST)v3 * v3; }
            }
        #اگر_ختم
            برائے (; i < len; i++) {
                اگر (نقاب[i]) {
                    T v = src[i];
                    s += (ST)v * v;
                }
            }
        }
        ورنہ {
            برائے (صحیح i = 0; i < len; i++) {
                اگر (نقاب[i]) {
                    ثابت T* elem = src + i * cn;
                    صحیح k = 0;
                #اگر CV_ENABLE_UNROLLED && !CV_SIMD_SCALABLE
                    برائے (; k <= cn - 4; k += 4) {
                        T v0 = elem[k];     s += (ST)v0 * v0;
                        T v1 = elem[k + 1]; s += (ST)v1 * v1;
                        T v2 = elem[k + 2]; s += (ST)v2 * v2;
                        T v3 = elem[k + 3]; s += (ST)v3 * v3;
                    }
                #اگر_ختم
                    برائے (; k < cn; k++) {
                        T v = elem[k];
                        s += (ST)v * v;
                    }
                }
            }
        }

        واپس s;
    }
};

#اگر (CV_SIMD || CV_SIMD_SCALABLE)
ساکن اندرونی v_uint8  v_normmask_u8 (ثابت uchar* m) { واپس v_gt(vx_بھرو(m),          vx_setzero_u8());  }
ساکن اندرونی v_uint16 v_normmask_u16(ثابت uchar* m) { واپس v_gt(vx_بھرو_expand(m),   vx_setzero_u16()); }
ساکن اندرونی v_uint32 v_normmask_u32(ثابت uchar* m) { واپس v_gt(vx_بھرو_expand_q(m), vx_setzero_u32()); }
ساکن اندرونی خالی v_normmask_u64(ثابت uchar* m, v_uint64& m0, v_uint64& m1) {
    v_uint64 e0, e1;
    v_expand(v_normmask_u32(m), e0, e1);
    m0 = v_or(e0, v_shl<32>(e0));
    m1 = v_or(e1, v_shl<32>(e1));
}

سانچہ <>
ساخت MaskedNormInf_SIMD<اعشاری, اعشاری> {
    اندرونی اعشاری عامل()(ثابت اعشاری* src, ثابت uchar* نقاب, صحیح len, صحیح cn) ثابت {
        اعشاری نتیجہ = 0.0f;
        اگر (cn == 1) {
            صحیح i = 0;
            ثابت صحیح vstep = VTraits<v_float32>::vlanes();
            v_float32 acc = vx_setzero_f32();

            برائے (; i <= len - vstep; i += vstep) {
                v_uint32 m = vx_بھرو_expand_q(نقاب + i);
                v_uint32 cmp = v_gt(m, vx_setzero_u32());
                v_float32 s = vx_بھرو(src + i);
                s = v_abs(s);
                s = v_reinterpret_as_f32(v_and(v_reinterpret_as_u32(s), cmp));
                acc = v_max(acc, s);
            }
            نتیجہ = v_reduce_max(acc);

            برائے (; i < len; i++) {
                اگر (نقاب[i])
                    نتیجہ = std::max(نتیجہ, std::مطلق_قدر(src[i]));
            }
        }
        ورنہ {
            برائے (صحیح i = 0; i < len; i++) {
                اگر (نقاب[i]) {
                    ثابت اعشاری* elem = src + i * cn;
                    صحیح k = 0;
                    ثابت صحیح vstep = VTraits<v_float32>::vlanes();
                    v_float32 acc = vx_setzero_f32();

                    برائے (; k <= cn - vstep; k += vstep) {
                        v_float32 s = vx_بھرو(elem + k);
                        acc = v_max(acc, v_abs(s));
                    }

                    نتیجہ = std::max(نتیجہ, v_reduce_max(acc));

                    برائے (; k < cn; k++)
                        نتیجہ = std::max(نتیجہ, std::مطلق_قدر(elem[k]));
                }
            }
        }
        واپس نتیجہ;
    }
};

#اگر CV_SIMD_64F
سانچہ <>
ساخت MaskedNormL1_SIMD<اعشاری, دوہرا> {
    اندرونی دوہرا عامل()(ثابت اعشاری* src, ثابت uchar* نقاب, صحیح len, صحیح cn) ثابت {
        دوہرا نتیجہ = 0.0;
        اگر (cn == 1) {
            صحیح i = 0;
            ثابت صحیح vstep = VTraits<v_float32>::vlanes();
            v_float64 acc0 = vx_setzero_f64(), acc1 = vx_setzero_f64();
            v_float64 acc2 = vx_setzero_f64(), acc3 = vx_setzero_f64();

            برائے (; i <= len - 2*vstep; i += 2*vstep) {
                v_float32 s0 = v_and(v_abs(vx_بھرو(src + i)),
                                     v_reinterpret_as_f32(v_normmask_u32(نقاب + i)));
                v_float32 s1 = v_and(v_abs(vx_بھرو(src + i + vstep)),
                                     v_reinterpret_as_f32(v_normmask_u32(نقاب + i + vstep)));
                acc0 = v_جوڑو(acc0, v_cvt_f64(s0));
                acc1 = v_جوڑو(acc1, v_cvt_f64_بلند(s0));
                acc2 = v_جوڑو(acc2, v_cvt_f64(s1));
                acc3 = v_جوڑو(acc3, v_cvt_f64_بلند(s1));
            }
            برائے (; i <= len - vstep; i += vstep) {
                v_float32 s = v_and(v_abs(vx_بھرو(src + i)),
                                    v_reinterpret_as_f32(v_normmask_u32(نقاب + i)));
                acc0 = v_جوڑو(acc0, v_cvt_f64(s));
                acc1 = v_جوڑو(acc1, v_cvt_f64_بلند(s));
            }
            نتیجہ = v_reduce_مجموع(v_جوڑو(v_جوڑو(acc0, acc1), v_جوڑو(acc2, acc3)));

            برائے (; i < len; i++) {
                اگر (نقاب[i])
                    نتیجہ += std::مطلق_قدر(src[i]);
            }
        }
        ورنہ {
            برائے (صحیح i = 0; i < len; i++) {
                اگر (نقاب[i]) {
                    ثابت اعشاری* elem = src + i * cn;
                    صحیح k = 0;
                    ثابت صحیح vstep = VTraits<v_float32>::vlanes();
                    v_float64 acc = vx_setzero_f64();

                    برائے (; k <= cn - vstep; k += vstep) {
                        v_float32 s = v_abs(vx_بھرو(elem + k));
                        acc = v_جوڑو(acc, v_cvt_f64(s));
                        acc = v_جوڑو(acc, v_cvt_f64_بلند(s));
                    }

                    نتیجہ += v_reduce_مجموع(acc);

                    برائے (; k < cn; k++)
                        نتیجہ += std::مطلق_قدر(elem[k]);
                }
            }
        }
        واپس نتیجہ;
    }
};

سانچہ <>
ساخت MaskedNormL2_SIMD<اعشاری, دوہرا> {
    اندرونی دوہرا عامل()(ثابت اعشاری* src, ثابت uchar* نقاب, صحیح len, صحیح cn) ثابت {
        دوہرا نتیجہ = 0.0;
        اگر (cn == 1) {
            صحیح i = 0;
            ثابت صحیح vstep = VTraits<v_float32>::vlanes();
            v_float32 facc = vx_setzero_f32();
            v_float64 dacc = vx_setzero_f64();
            صحیح flush = 0;

            برائے (; i <= len - vstep; i += vstep, flush += vstep) {
                اگر (flush >= 64) {
                    dacc = v_جوڑو(dacc, v_cvt_f64(facc));
                    dacc = v_جوڑو(dacc, v_cvt_f64_بلند(facc));
                    facc = vx_setzero_f32();
                    flush = 0;
                }
                v_uint32 cmp = v_gt(vx_بھرو_expand_q(نقاب + i), vx_setzero_u32());
                v_float32 s  = v_reinterpret_as_f32(v_and(v_reinterpret_as_u32(vx_بھرو(src + i)), cmp));
                facc = v_جوڑو(facc, v_mul(s, s));
            }

            dacc = v_جوڑو(dacc, v_cvt_f64(facc));
            dacc = v_جوڑو(dacc, v_cvt_f64_بلند(facc));
            نتیجہ = v_reduce_مجموع(dacc);

            برائے (; i < len; i++) {
                اگر (نقاب[i]) {
                    دوہرا v = src[i];
                    نتیجہ += v * v;
                }
            }
        }
        ورنہ {
            برائے (صحیح i = 0; i < len; i++) {
                اگر (نقاب[i]) {
                    ثابت اعشاری* elem = src + i * cn;
                    صحیح k = 0;
                    ثابت صحیح vstep = VTraits<v_float32>::vlanes();
                    v_float32 facc = vx_setzero_f32();

                    برائے (; k <= cn - vstep; k += vstep) {
                        v_float32 s = vx_بھرو(elem + k);
                        facc = v_جوڑو(facc, v_mul(s, s));
                    }

                    v_float64 dacc = v_جوڑو(v_cvt_f64(facc), v_cvt_f64_بلند(facc));
                    نتیجہ += v_reduce_مجموع(dacc);

                    برائے (; k < cn; k++) {
                        دوہرا v = elem[k];
                        نتیجہ += v * v;
                    }
                }
            }
        }
        واپس نتیجہ;
    }
};

#اگر_ختم

سانچہ <>
ساخت MaskedNormInf_SIMD<uchar, صحیح> {
    اندرونی صحیح عامل()(ثابت uchar* src, ثابت uchar* نقاب, صحیح len, صحیح cn) ثابت {
        صحیح نتیجہ = 0;
        اگر (cn == 1) {
            صحیح i = 0;
            ثابت صحیح vstep = VTraits<v_uint8>::vlanes();
            v_uint8 acc = vx_setzero_u8();

            برائے (; i <= len - vstep; i += vstep) {
                v_uint8 m   = vx_بھرو(نقاب + i);
                v_uint8 s   = vx_بھرو(src + i);
                v_uint8 sel = v_and(s, v_gt(m, vx_setzero_u8()));
                acc = v_max(acc, sel);
            }

            نتیجہ = (صحیح)v_reduce_max(acc);

            برائے (; i < len; i++) {
                اگر (نقاب[i])
                    نتیجہ = std::max(نتیجہ, (صحیح)src[i]);
            }
        }
        ورنہ اگر (cn == 4 && len >= VTraits<v_uint8>::vlanes()) {
            ثابت صحیح vstep = VTraits<v_uint8>::vlanes();
            v_uint8 acc = vx_setzero_u8();
            صحیح i = 0;
            برائے (;;) {
                اگر (i > len - vstep)
                    i = len - vstep; // back-step (max is idempotent)
                v_uint8 c0, c1, c2, c3;
                v_بھرو_deinterleave(src + i * 4, c0, c1, c2, c3);
                v_uint8 pmax = v_max(v_max(c0, c1), v_max(c2, c3));
                v_uint8 m = v_gt(vx_بھرو(نقاب + i), vx_setzero_u8());
                acc = v_max(acc, v_and(pmax, m));
                اگر (i >= len - vstep)
                    توڑو;
                i += vstep;
            }
            نتیجہ = (صحیح)v_reduce_max(acc);
        }
        ورنہ اگر (cn == 4) {
            برائے (صحیح i = 0; i < len; i++) {
                اگر (نقاب[i]) {
                    ثابت uchar* elem = src + i * 4;
                    نتیجہ = std::max(نتیجہ, (صحیح)std::max(std::max(elem[0], elem[1]),
                                                            std::max(elem[2], elem[3])));
                }
            }
        }
        ورنہ {
            برائے (صحیح i = 0; i < len; i++) {
                اگر (نقاب[i]) {
                    ثابت uchar* elem = src + i * cn;
                    برائے (صحیح k = 0; k < cn; k++)
                        نتیجہ = std::max(نتیجہ, (صحیح)elem[k]);
                }
            }
        }
        واپس نتیجہ;
    }
};

سانچہ <>
ساخت MaskedNormL1_SIMD<uchar, صحیح> {
    اندرونی صحیح عامل()(ثابت uchar* src, ثابت uchar* نقاب, صحیح len, صحیح cn) ثابت {
        صحیح نتیجہ = 0;
        ثابت صحیح vstep = VTraits<v_uint8>::vlanes();
        ثابت v_uint8 one = vx_setall_u8(1);
        اگر (cn == 1) {
            صحیح i = 0;
            v_uint32 acc = vx_setzero_u32();
            برائے (; i <= len - vstep; i += vstep) {
                v_uint8 s = v_and(vx_بھرو(src + i), v_normmask_u8(نقاب + i));
                acc = v_dotprod_expand_fast(s, one, acc);
            }
            نتیجہ = (صحیح)v_reduce_مجموع(acc);
            برائے (; i < len; i++) {
                اگر (نقاب[i])
                    نتیجہ += src[i];
            }
        }
        ورنہ اگر (cn == 4) {
            صحیح i = 0;
            v_uint32 acc = vx_setzero_u32();
            برائے (; i <= len - vstep; i += vstep) {
                v_uint8 c0, c1, c2, c3;
                v_بھرو_deinterleave(src + i * 4, c0, c1, c2, c3);
                v_uint8 m = v_normmask_u8(نقاب + i);
                acc = v_dotprod_expand_fast(v_and(c0, m), one, acc);
                acc = v_dotprod_expand_fast(v_and(c1, m), one, acc);
                acc = v_dotprod_expand_fast(v_and(c2, m), one, acc);
                acc = v_dotprod_expand_fast(v_and(c3, m), one, acc);
            }
            نتیجہ = (صحیح)v_reduce_مجموع(acc);
            برائے (; i < len; i++) {
                اگر (نقاب[i]) {
                    ثابت uchar* elem = src + i * 4;
                    نتیجہ += elem[0] + elem[1] + elem[2] + elem[3];
                }
            }
        }
        ورنہ {
            برائے (صحیح i = 0; i < len; i++) {
                اگر (نقاب[i]) {
                    ثابت uchar* elem = src + i * cn;
                    برائے (صحیح k = 0; k < cn; k++)
                        نتیجہ += elem[k];
                }
            }
        }
        واپس نتیجہ;
    }
};

سانچہ <>
ساخت MaskedNormInf_SIMD<ushort, صحیح> {
    اندرونی صحیح عامل()(ثابت ushort* src, ثابت uchar* نقاب, صحیح len, صحیح cn) ثابت {
        صحیح نتیجہ = 0;
        اگر (cn == 1) {
            صحیح i = 0;
            ثابت صحیح vstep = VTraits<v_uint16>::vlanes();
            v_uint16 acc = vx_setzero_u16();

            برائے (; i <= len - vstep; i += vstep) {
                v_uint16 m   = vx_بھرو_expand(نقاب + i);
                v_uint16 cmp = v_gt(m, vx_setzero_u16());
                v_uint16 s   = vx_بھرو(src + i);
                v_uint16 sel = v_and(s, cmp);
                acc = v_max(acc, sel);
            }

            نتیجہ = (صحیح)v_reduce_max(acc);

            برائے (; i < len; i++) {
                اگر (نقاب[i])
                    نتیجہ = std::max(نتیجہ, (صحیح)src[i]);
            }
        }
        ورنہ {
            برائے (صحیح i = 0; i < len; i++) {
                اگر (نقاب[i]) {
                    ثابت ushort* elem = src + i * cn;
                    صحیح k = 0;
                    ثابت صحیح vstep = VTraits<v_uint16>::vlanes();
                    v_uint16 acc = vx_setzero_u16();

                    برائے (; k <= cn - vstep; k += vstep) {
                        acc = v_max(acc, vx_بھرو(elem + k));
                    }

                    نتیجہ = std::max(نتیجہ, (صحیح)v_reduce_max(acc));

                    برائے (; k < cn; k++)
                        نتیجہ = std::max(نتیجہ, (صحیح)elem[k]);
                }
            }
        }
        واپس نتیجہ;
    }
};

سانچہ <>
ساخت MaskedNormL1_SIMD<ushort, صحیح> {
    اندرونی صحیح عامل()(ثابت ushort* src, ثابت uchar* نقاب, صحیح len, صحیح cn) ثابت {
        صحیح نتیجہ = 0;
        اگر (cn == 1) {
            صحیح i = 0;
            ثابت صحیح vstep = VTraits<v_uint16>::vlanes();
            v_uint32 acc32 = vx_setzero_u32();
            v_uint64 acc64 = vx_setzero_u64();
            صحیح acc32_elems = 0;

            برائے (; i <= len - vstep; i += vstep, acc32_elems += vstep) {
                اگر (acc32_elems >= 512) {
                    v_uint64 lo64, hi64;
                    v_expand(acc32, lo64, hi64);
                    acc64 = v_جوڑو(acc64, v_جوڑو(lo64, hi64));
                    acc32 = vx_setzero_u32();
                    acc32_elems = 0;
                }
                v_uint16 m   = vx_بھرو_expand(نقاب + i);
                v_uint16 cmp = v_gt(m, vx_setzero_u16());
                v_uint16 s   = v_and(vx_بھرو(src + i), cmp);
                v_uint32 lo32, hi32;
                v_expand(s, lo32, hi32);
                acc32 = v_جوڑو(acc32, v_جوڑو(lo32, hi32));
            }

            v_uint64 lo64, hi64;
            v_expand(acc32, lo64, hi64);
            acc64 = v_جوڑو(acc64, v_جوڑو(lo64, hi64));
            نتیجہ = (صحیح)v_reduce_مجموع(acc64);

            برائے (; i < len; i++) {
                اگر (نقاب[i])
                    نتیجہ += src[i];
            }
        }
        ورنہ {
            برائے (صحیح i = 0; i < len; i++) {
                اگر (نقاب[i]) {
                    ثابت ushort* elem = src + i * cn;
                    صحیح k = 0;
                    ثابت صحیح vstep = VTraits<v_uint16>::vlanes();
                    v_uint32 acc = vx_setzero_u32();

                    برائے (; k <= cn - vstep; k += vstep) {
                        v_uint32 lo32, hi32;
                        v_expand(vx_بھرو(elem + k), lo32, hi32);
                        acc = v_جوڑو(acc, v_جوڑو(lo32, hi32));
                    }

                    نتیجہ += (صحیح)v_reduce_مجموع(acc);

                    برائے (; k < cn; k++)
                        نتیجہ += elem[k];
                }
            }
        }
        واپس نتیجہ;
    }
};

سانچہ <>
ساخت MaskedNormL2_SIMD<ushort, دوہرا> {
    اندرونی دوہرا عامل()(ثابت ushort* src, ثابت uchar* نقاب, صحیح len, صحیح cn) ثابت {
        دوہرا نتیجہ = 0.0;
        اگر (cn == 1) {
            صحیح i = 0;
            ثابت صحیح vstep = VTraits<v_uint16>::vlanes();
            v_uint64 acc = vx_setzero_u64();
            برائے (; i <= len - vstep; i += vstep) {
                v_uint16 m   = vx_بھرو_expand(نقاب + i);
                v_uint16 cmp = v_gt(m, vx_setzero_u16());
                v_uint16 s   = v_and(vx_بھرو(src + i), cmp);
                v_uint32 lo32, hi32;
                v_expand(s, lo32, hi32);
                v_uint64 lo64a, lo64b, hi64a, hi64b;
                v_expand(v_mul(lo32, lo32), lo64a, lo64b);
                v_expand(v_mul(hi32, hi32), hi64a, hi64b);
                acc = v_جوڑو(acc, v_جوڑو(v_جوڑو(lo64a, lo64b), v_جوڑو(hi64a, hi64b)));
            }
            نتیجہ = (دوہرا)v_reduce_مجموع(acc);
            برائے (; i < len; i++) {
                اگر (نقاب[i]) {
                    دوہرا v = src[i];
                    نتیجہ += v * v;
                }
            }
        }
        ورنہ {
            برائے (صحیح i = 0; i < len; i++) {
                اگر (نقاب[i]) {
                    ثابت ushort* elem = src + i * cn;
                    صحیح k = 0;
                    ثابت صحیح vstep = VTraits<v_uint16>::vlanes();
                    v_uint64 acc = vx_setzero_u64();
                    برائے (; k <= cn - vstep; k += vstep) {
                        v_uint32 lo32, hi32;
                        v_expand(vx_بھرو(elem + k), lo32, hi32);
                        v_uint64 lo64a, lo64b, hi64a, hi64b;
                        v_expand(v_mul(lo32, lo32), lo64a, lo64b);
                        v_expand(v_mul(hi32, hi32), hi64a, hi64b);
                        acc = v_جوڑو(acc, v_جوڑو(v_جوڑو(lo64a, lo64b), v_جوڑو(hi64a, hi64b)));
                    }

                    نتیجہ += (دوہرا)v_reduce_مجموع(acc);

                    برائے (; k < cn; k++) {
                        دوہرا v = elem[k];
                        نتیجہ += v * v;
                    }
                }
            }
        }
        واپس نتیجہ;
    }
};

سانچہ <>
ساخت MaskedNormInf_SIMD<schar, صحیح> {
    اندرونی صحیح عامل()(ثابت schar* src, ثابت uchar* نقاب, صحیح len, صحیح cn) ثابت {
        صحیح نتیجہ = 0;
        اگر (cn == 1) {
            ثابت صحیح vstep = VTraits<v_int8>::vlanes();
            صحیح i = 0;
            v_uint8 acc = vx_setzero_u8();
            برائے (; i <= len - vstep; i += vstep)
                acc = v_max(acc, v_and(v_abs(vx_بھرو(src + i)), v_normmask_u8(نقاب + i)));
            نتیجہ = (صحیح)v_reduce_max(acc);
            برائے (; i < len; i++)
                اگر (نقاب[i]) نتیجہ = std::max(نتیجہ, cv_abs(src[i]));
        }
        ورنہ {
            برائے (صحیح i = 0; i < len; i++)
                اگر (نقاب[i]) {
                    ثابت schar* elem = src + i * cn;
                    برائے (صحیح k = 0; k < cn; k++)
                        نتیجہ = std::max(نتیجہ, cv_abs(elem[k]));
                }
        }
        واپس نتیجہ;
    }
};

سانچہ <>
ساخت MaskedNormL1_SIMD<schar, صحیح> {
    اندرونی صحیح عامل()(ثابت schar* src, ثابت uchar* نقاب, صحیح len, صحیح cn) ثابت {
        صحیح نتیجہ = 0;
        اگر (cn == 1) {
            ثابت صحیح vstep = VTraits<v_int8>::vlanes();
            ثابت v_uint8 one = vx_setall_u8(1);
            صحیح i = 0;
            v_uint32 acc = vx_setzero_u32();
            برائے (; i <= len - vstep; i += vstep)
                acc = v_dotprod_expand_fast(v_and(v_abs(vx_بھرو(src + i)), v_normmask_u8(نقاب + i)), one, acc);
            نتیجہ = (صحیح)v_reduce_مجموع(acc);
            برائے (; i < len; i++)
                اگر (نقاب[i]) نتیجہ += cv_abs(src[i]);
        }
        ورنہ {
            برائے (صحیح i = 0; i < len; i++)
                اگر (نقاب[i]) {
                    ثابت schar* elem = src + i * cn;
                    برائے (صحیح k = 0; k < cn; k++)
                        نتیجہ += cv_abs(elem[k]);
                }
        }
        واپس نتیجہ;
    }
};

سانچہ <>
ساخت MaskedNormL2_SIMD<schar, صحیح> {
    اندرونی صحیح عامل()(ثابت schar* src, ثابت uchar* نقاب, صحیح len, صحیح cn) ثابت {
        صحیح نتیجہ = 0;
        اگر (cn == 1) {
            ثابت صحیح vstep = VTraits<v_int8>::vlanes();
            صحیح i = 0;
            v_int32 acc = vx_setzero_s32();
            برائے (; i <= len - vstep; i += vstep) {
                v_int8 s = v_and(vx_بھرو(src + i), v_reinterpret_as_s8(v_normmask_u8(نقاب + i)));
                acc = v_dotprod_expand_fast(s, s, acc);
            }
            نتیجہ = (صحیح)v_reduce_مجموع(acc);
            برائے (; i < len; i++)
                اگر (نقاب[i]) { صحیح v = src[i]; نتیجہ += v * v; }
        }
        ورنہ {
            برائے (صحیح i = 0; i < len; i++)
                اگر (نقاب[i]) {
                    ثابت schar* elem = src + i * cn;
                    برائے (صحیح k = 0; k < cn; k++) { صحیح v = elem[k]; نتیجہ += v * v; }
                }
        }
        واپس نتیجہ;
    }
};

سانچہ <>
ساخت MaskedNormL2_SIMD<uchar, صحیح> {
    اندرونی صحیح عامل()(ثابت uchar* src, ثابت uchar* نقاب, صحیح len, صحیح cn) ثابت {
        صحیح نتیجہ = 0;
        ثابت صحیح vstep = VTraits<v_uint8>::vlanes();
        اگر (cn == 1) {
            صحیح i = 0;
            v_uint32 acc = vx_setzero_u32();
            برائے (; i <= len - vstep; i += vstep) {
                v_uint8 s = v_and(vx_بھرو(src + i), v_normmask_u8(نقاب + i));
                acc = v_dotprod_expand_fast(s, s, acc);
            }
            نتیجہ = (صحیح)v_reduce_مجموع(acc);
            برائے (; i < len; i++)
                اگر (نقاب[i]) { صحیح v = src[i]; نتیجہ += v * v; }
        }
        ورنہ اگر (cn == 4) {
            صحیح i = 0;
            v_uint32 acc = vx_setzero_u32();
            برائے (; i <= len - vstep; i += vstep) {
                v_uint8 c0, c1, c2, c3;
                v_بھرو_deinterleave(src + i * 4, c0, c1, c2, c3);
                v_uint8 m = v_normmask_u8(نقاب + i);
                c0 = v_and(c0, m); c1 = v_and(c1, m);
                c2 = v_and(c2, m); c3 = v_and(c3, m);
                acc = v_dotprod_expand_fast(c0, c0, acc);
                acc = v_dotprod_expand_fast(c1, c1, acc);
                acc = v_dotprod_expand_fast(c2, c2, acc);
                acc = v_dotprod_expand_fast(c3, c3, acc);
            }
            نتیجہ = (صحیح)v_reduce_مجموع(acc);
            برائے (; i < len; i++)
                اگر (نقاب[i]) {
                    ثابت uchar* elem = src + i * 4;
                    برائے (صحیح k = 0; k < 4; k++) { صحیح v = elem[k]; نتیجہ += v * v; }
                }
        }
        ورنہ {
            برائے (صحیح i = 0; i < len; i++)
                اگر (نقاب[i]) {
                    ثابت uchar* elem = src + i * cn;
                    برائے (صحیح k = 0; k < cn; k++) { صحیح v = elem[k]; نتیجہ += v * v; }
                }
        }
        واپس نتیجہ;
    }
};

سانچہ <>
ساخت MaskedNormInf_SIMD<مختصر, صحیح> {
    اندرونی صحیح عامل()(ثابت مختصر* src, ثابت uchar* نقاب, صحیح len, صحیح cn) ثابت {
        صحیح نتیجہ = 0;
        اگر (cn == 1) {
            ثابت صحیح vstep = VTraits<v_int16>::vlanes();
            صحیح i = 0;
            v_uint16 acc = vx_setzero_u16();
            برائے (; i <= len - vstep; i += vstep)
                acc = v_max(acc, v_and(v_abs(vx_بھرو(src + i)), v_normmask_u16(نقاب + i)));
            نتیجہ = (صحیح)v_reduce_max(acc);
            برائے (; i < len; i++)
                اگر (نقاب[i]) نتیجہ = std::max(نتیجہ, cv_abs(src[i]));
        }
        ورنہ {
            برائے (صحیح i = 0; i < len; i++)
                اگر (نقاب[i]) {
                    ثابت مختصر* elem = src + i * cn;
                    برائے (صحیح k = 0; k < cn; k++)
                        نتیجہ = std::max(نتیجہ, cv_abs(elem[k]));
                }
        }
        واپس نتیجہ;
    }
};

سانچہ <>
ساخت MaskedNormL1_SIMD<مختصر, صحیح> {
    اندرونی صحیح عامل()(ثابت مختصر* src, ثابت uchar* نقاب, صحیح len, صحیح cn) ثابت {
        صحیح نتیجہ = 0;
        اگر (cn == 1) {
            ثابت صحیح vstep = VTraits<v_int16>::vlanes();
            صحیح i = 0;
            v_uint32 acc0 = vx_setzero_u32(), acc1 = vx_setzero_u32();
            برائے (; i <= len - vstep; i += vstep) {
                v_uint16 s = v_and(v_abs(vx_بھرو(src + i)), v_normmask_u16(نقاب + i));
                v_uint32 lo, hi;
                v_expand(s, lo, hi);
                acc0 = v_جوڑو(acc0, lo);
                acc1 = v_جوڑو(acc1, hi);
            }
            نتیجہ = (صحیح)v_reduce_مجموع(v_جوڑو(acc0, acc1));
            برائے (; i < len; i++)
                اگر (نقاب[i]) نتیجہ += cv_abs(src[i]);
        }
        ورنہ {
            برائے (صحیح i = 0; i < len; i++)
                اگر (نقاب[i]) {
                    ثابت مختصر* elem = src + i * cn;
                    برائے (صحیح k = 0; k < cn; k++)
                        نتیجہ += cv_abs(elem[k]);
                }
        }
        واپس نتیجہ;
    }
};

سانچہ <>
ساخت MaskedNormL2_SIMD<مختصر, دوہرا> {
    اندرونی دوہرا عامل()(ثابت مختصر* src, ثابت uchar* نقاب, صحیح len, صحیح cn) ثابت {
        دوہرا نتیجہ = 0.0;
        اگر (cn == 1) {
            ثابت صحیح vstep = VTraits<v_int16>::vlanes();
            صحیح i = 0;
            v_uint64 acc = vx_setzero_u64();
            برائے (; i <= len - vstep; i += vstep) {
                v_uint16 s = v_and(v_abs(vx_بھرو(src + i)), v_normmask_u16(نقاب + i));
                acc = v_جوڑو(acc, v_dotprod_expand_fast(s, s));
            }
            نتیجہ = (دوہرا)v_reduce_مجموع(acc);
            برائے (; i < len; i++)
                اگر (نقاب[i]) { دوہرا v = src[i]; نتیجہ += v * v; }
        }
        ورنہ {
            برائے (صحیح i = 0; i < len; i++)
                اگر (نقاب[i]) {
                    ثابت مختصر* elem = src + i * cn;
                    برائے (صحیح k = 0; k < cn; k++) { دوہرا v = elem[k]; نتیجہ += v * v; }
                }
        }
        واپس نتیجہ;
    }
};

سانچہ <>
ساخت MaskedNormInf_SIMD<صحیح, غیرعلامتی> {
    اندرونی غیرعلامتی عامل()(ثابت صحیح* src, ثابت uchar* نقاب, صحیح len, صحیح cn) ثابت {
        غیرعلامتی نتیجہ = 0;
        اگر (cn == 1) {
            ثابت صحیح vstep = VTraits<v_int32>::vlanes();
            صحیح i = 0;
            v_uint32 acc = vx_setzero_u32();
            برائے (; i <= len - vstep; i += vstep)
                acc = v_max(acc, v_and(v_abs(vx_بھرو(src + i)), v_normmask_u32(نقاب + i)));
            نتیجہ = v_reduce_max(acc);
            برائے (; i < len; i++)
                اگر (نقاب[i]) نتیجہ = std::max(نتیجہ, cv_abs(src[i]));
        }
        ورنہ {
            برائے (صحیح i = 0; i < len; i++)
                اگر (نقاب[i]) {
                    ثابت صحیح* elem = src + i * cn;
                    برائے (صحیح k = 0; k < cn; k++)
                        نتیجہ = std::max(نتیجہ, cv_abs(elem[k]));
                }
        }
        واپس نتیجہ;
    }
};

#اگر CV_SIMD_64F || CV_SIMD_SCALABLE_64F

سانچہ <>
ساخت MaskedNormL1_SIMD<صحیح, دوہرا> {
    اندرونی دوہرا عامل()(ثابت صحیح* src, ثابت uchar* نقاب, صحیح len, صحیح cn) ثابت {
        دوہرا نتیجہ = 0.0;
        اگر (cn == 1) {
            ثابت صحیح vstep = VTraits<v_int32>::vlanes();
            صحیح i = 0;
            v_float64 acc0 = vx_setzero_f64(), acc1 = vx_setzero_f64();
            برائے (; i <= len - vstep; i += vstep) {
                // Mask the value, then widen and take the magnitude in double. v_abs() does give
                // |INT_MIN| as an unsigned 2147483648, but reinterpreting that back as int32 turns
                // it negative again, and the widening then carries the sign into the accumulator.
                v_int32 s = v_reinterpret_as_s32(v_and(v_reinterpret_as_u32(vx_بھرو(src + i)),
                                                       v_normmask_u32(نقاب + i)));
                acc0 = v_جوڑو(acc0, v_abs(v_cvt_f64(s)));
                acc1 = v_جوڑو(acc1, v_abs(v_cvt_f64_بلند(s)));
            }
            نتیجہ = v_reduce_مجموع(v_جوڑو(acc0, acc1));
            برائے (; i < len; i++)
                اگر (نقاب[i]) نتیجہ += cv_abs(src[i]);
        }
        ورنہ {
            برائے (صحیح i = 0; i < len; i++)
                اگر (نقاب[i]) {
                    ثابت صحیح* elem = src + i * cn;
                    برائے (صحیح k = 0; k < cn; k++)
                        نتیجہ += cv_abs(elem[k]);
                }
        }
        واپس نتیجہ;
    }
};

سانچہ <>
ساخت MaskedNormL2_SIMD<صحیح, دوہرا> {
    اندرونی دوہرا عامل()(ثابت صحیح* src, ثابت uchar* نقاب, صحیح len, صحیح cn) ثابت {
        دوہرا نتیجہ = 0.0;
        اگر (cn == 1) {
            ثابت صحیح vstep = VTraits<v_int32>::vlanes();
            صحیح i = 0;
            v_float64 acc0 = vx_setzero_f64(), acc1 = vx_setzero_f64();
            برائے (; i <= len - vstep; i += vstep) {
                v_uint32 s = v_and(v_abs(vx_بھرو(src + i)), v_normmask_u32(نقاب + i));
                v_uint64 e0, e1;
                v_expand(s, e0, e1);
                v_float64 f0 = v_cvt_f64(v_reinterpret_as_s64(e0));
                v_float64 f1 = v_cvt_f64(v_reinterpret_as_s64(e1));
                acc0 = v_fma(f0, f0, acc0);
                acc1 = v_fma(f1, f1, acc1);
            }
            نتیجہ = v_reduce_مجموع(v_جوڑو(acc0, acc1));
            برائے (; i < len; i++)
                اگر (نقاب[i]) { دوہرا v = src[i]; نتیجہ += v * v; }
        }
        ورنہ {
            برائے (صحیح i = 0; i < len; i++)
                اگر (نقاب[i]) {
                    ثابت صحیح* elem = src + i * cn;
                    برائے (صحیح k = 0; k < cn; k++) { دوہرا v = elem[k]; نتیجہ += v * v; }
                }
        }
        واپس نتیجہ;
    }
};

سانچہ <>
ساخت MaskedNormInf_SIMD<دوہرا, دوہرا> {
    اندرونی دوہرا عامل()(ثابت دوہرا* src, ثابت uchar* نقاب, صحیح len, صحیح cn) ثابت {
        دوہرا نتیجہ = 0.0;
        اگر (cn == 1) {
            ثابت صحیح vstep = VTraits<v_float64>::vlanes();
            صحیح i = 0;
            v_float64 acc0 = vx_setzero_f64(), acc1 = vx_setzero_f64();
            برائے (; i <= len - 2*vstep; i += 2*vstep) {
                v_uint64 m0, m1;
                v_normmask_u64(نقاب + i, m0, m1);
                acc0 = v_max(acc0, v_reinterpret_as_f64(v_and(v_reinterpret_as_u64(v_abs(vx_بھرو(src + i))), m0)));
                acc1 = v_max(acc1, v_reinterpret_as_f64(v_and(v_reinterpret_as_u64(v_abs(vx_بھرو(src + i + vstep))), m1)));
            }
            دوہرا t[VTraits<v_float64>::max_nlanes];
            vx_store(t, v_max(acc0, acc1));
            برائے (صحیح k = 0; k < vstep; k++)
                نتیجہ = std::max(نتیجہ, t[k]);
            برائے (; i < len; i++)
                اگر (نقاب[i]) نتیجہ = std::max(نتیجہ, cv_abs(src[i]));
        }
        ورنہ {
            برائے (صحیح i = 0; i < len; i++)
                اگر (نقاب[i]) {
                    ثابت دوہرا* elem = src + i * cn;
                    برائے (صحیح k = 0; k < cn; k++)
                        نتیجہ = std::max(نتیجہ, cv_abs(elem[k]));
                }
        }
        واپس نتیجہ;
    }
};

سانچہ <>
ساخت MaskedNormL1_SIMD<دوہرا, دوہرا> {
    اندرونی دوہرا عامل()(ثابت دوہرا* src, ثابت uchar* نقاب, صحیح len, صحیح cn) ثابت {
        دوہرا نتیجہ = 0.0;
        اگر (cn == 1) {
            ثابت صحیح vstep = VTraits<v_float64>::vlanes();
            صحیح i = 0;
            v_float64 acc0 = vx_setzero_f64(), acc1 = vx_setzero_f64();
            برائے (; i <= len - 2*vstep; i += 2*vstep) {
                v_uint64 m0, m1;
                v_normmask_u64(نقاب + i, m0, m1);
                acc0 = v_جوڑو(acc0, v_reinterpret_as_f64(v_and(v_reinterpret_as_u64(v_abs(vx_بھرو(src + i))), m0)));
                acc1 = v_جوڑو(acc1, v_reinterpret_as_f64(v_and(v_reinterpret_as_u64(v_abs(vx_بھرو(src + i + vstep))), m1)));
            }
            نتیجہ = v_reduce_مجموع(v_جوڑو(acc0, acc1));
            برائے (; i < len; i++)
                اگر (نقاب[i]) نتیجہ += cv_abs(src[i]);
        }
        ورنہ {
            برائے (صحیح i = 0; i < len; i++)
                اگر (نقاب[i]) {
                    ثابت دوہرا* elem = src + i * cn;
                    برائے (صحیح k = 0; k < cn; k++)
                        نتیجہ += cv_abs(elem[k]);
                }
        }
        واپس نتیجہ;
    }
};

سانچہ <>
ساخت MaskedNormL2_SIMD<دوہرا, دوہرا> {
    اندرونی دوہرا عامل()(ثابت دوہرا* src, ثابت uchar* نقاب, صحیح len, صحیح cn) ثابت {
        دوہرا نتیجہ = 0.0;
        اگر (cn == 1) {
            ثابت صحیح vstep = VTraits<v_float64>::vlanes();
            صحیح i = 0;
            v_float64 acc0 = vx_setzero_f64(), acc1 = vx_setzero_f64();
            برائے (; i <= len - 2*vstep; i += 2*vstep) {
                v_uint64 m0, m1;
                v_normmask_u64(نقاب + i, m0, m1);
                v_float64 s0 = v_reinterpret_as_f64(v_and(v_reinterpret_as_u64(vx_بھرو(src + i)), m0));
                v_float64 s1 = v_reinterpret_as_f64(v_and(v_reinterpret_as_u64(vx_بھرو(src + i + vstep)), m1));
                acc0 = v_fma(s0, s0, acc0);
                acc1 = v_fma(s1, s1, acc1);
            }
            نتیجہ = v_reduce_مجموع(v_جوڑو(acc0, acc1));
            برائے (; i < len; i++)
                اگر (نقاب[i]) { دوہرا v = src[i]; نتیجہ += v * v; }
        }
        ورنہ {
            برائے (صحیح i = 0; i < len; i++)
                اگر (نقاب[i]) {
                    ثابت دوہرا* elem = src + i * cn;
                    برائے (صحیح k = 0; k < cn; k++) { دوہرا v = elem[k]; نتیجہ += v * v; }
                }
        }
        واپس نتیجہ;
    }
};

#اگر_ختم // CV_SIMD_64F || CV_SIMD_SCALABLE_64F
#اگر_ختم // CV_SIMD || CV_SIMD_SCALABLE

سانچہ<قسم_نام T, قسم_نام ST> صحیح
normInf_(ثابت T* src, ثابت uchar* نقاب, ST* نتیجہ_2, صحیح len, صحیح cn)
{
    ST نتیجہ = *نتیجہ_2;
    اگر( !نقاب )
    {
        NormInf_SIMD<T, ST> op;
        نتیجہ = std::max(نتیجہ, op(src, len*cn));
    } ورنہ {
        MaskedNormInf_SIMD<T, ST> op;
        نتیجہ = std::max(نتیجہ, op(src, نقاب, len, cn));
    }
    *نتیجہ_2 = نتیجہ;
    واپس 0;
}

سانچہ<قسم_نام T, قسم_نام ST> صحیح
normL1_(ثابت T* src, ثابت uchar* نقاب, ST* نتیجہ_2, صحیح len, صحیح cn)
{
    ST نتیجہ = *نتیجہ_2;
    اگر( !نقاب )
    {
        NormL1_SIMD<T, ST> op;
        نتیجہ += op(src, len*cn);
    } ورنہ {
        MaskedNormL1_SIMD<T, ST> op;
        نتیجہ += op(src, نقاب, len, cn);
    }
    *نتیجہ_2 = نتیجہ;
    واپس 0;
}

سانچہ<قسم_نام T, قسم_نام ST> صحیح
normL2_(ثابت T* src, ثابت uchar* نقاب, ST* نتیجہ_2, صحیح len, صحیح cn)
{
    ST نتیجہ = *نتیجہ_2;
    اگر( !نقاب )
    {
        NormL2_SIMD<T, ST> op;
        نتیجہ += op(src, len*cn);
    }
    ورنہ
    {
        MaskedNormL2_SIMD<T, ST> op;
        نتیجہ += op(src, نقاب, len, cn);
    }
    *نتیجہ_2 = نتیجہ;
    واپس 0;
}

ساکن صحیح
normInf_Bool(ثابت uchar* src, ثابت uchar* نقاب, صحیح* نتیجہ_2, صحیح len, صحیح cn)
{
    صحیح نتیجہ = *نتیجہ_2;
    اگر( !نقاب )
    {
        برائے ( صحیح i = 0; i < len*cn; i++ ) {
            نتیجہ = std::max(نتیجہ, (صحیح)(src[i] != 0));
            اگر (نتیجہ != 0)
                توڑو;
        }
    }
    ورنہ
    {
        برائے( صحیح i = 0; i < len; i++, src += cn )
            اگر( نقاب[i] )
            {
                برائے( صحیح k = 0; k < cn; k++ )
                    نتیجہ = std::max(نتیجہ, (صحیح)(src[k] != 0));
                اگر (نتیجہ != 0)
                    توڑو;
            }
    }
    *نتیجہ_2 = نتیجہ;
    واپس 0;
}

ساکن صحیح
normL1_Bool(ثابت uchar* src, ثابت uchar* نقاب, صحیح* نتیجہ_2, صحیح len, صحیح cn)
{
    صحیح نتیجہ = *نتیجہ_2;
    اگر( !نقاب )
    {
        برائے ( صحیح i = 0; i < len*cn; i++ )
            نتیجہ += (صحیح)(src[i] != 0);
    }
    ورنہ
    {
        برائے( صحیح i = 0; i < len; i++, src += cn )
            اگر( نقاب[i] )
            {
                برائے( صحیح k = 0; k < cn; k++ )
                    نتیجہ += (صحیح)(src[k] != 0);
            }
    }
    *نتیجہ_2 = نتیجہ;
    واپس 0;
}

ساکن صحیح
normL2_Bool(ثابت uchar* src, ثابت uchar* نقاب, صحیح* نتیجہ_2, صحیح len, صحیح cn)
{
    واپس normL1_Bool(src, نقاب, نتیجہ_2, len, cn);
}

// ====================================================================
// Masked norm-diff kernels.
// Scalar base templates (safe for every depth) plus SIMD cn==1
// specializations for the concrete (T, ST) pairs dispatched by
// CV_DEF_NORM_ALL. The masked normDiff*_ functions below select them.
// ====================================================================
سانچہ <قسم_نام T, قسم_نام ST>
ساخت MaskedNormDiffInf_SIMD {
    اندرونی ST عامل()(ثابت T* s1, ثابت T* s2, ثابت uchar* نقاب, صحیح len, صحیح cn) ثابت {
        ST نتیجہ = 0;
        برائے (صحیح i = 0; i < len; i++) اگر (نقاب[i]) {
            ثابت T* e1 = s1 + i*cn; ثابت T* e2 = s2 + i*cn;
            برائے (صحیح k = 0; k < cn; k++)
                نتیجہ = std::max(نتیجہ, (ST)cv_absdiff(e1[k], e2[k]));
        }
        واپس نتیجہ;
    }
};
سانچہ <قسم_نام T, قسم_نام ST>
ساخت MaskedNormDiffL1_SIMD {
    اندرونی ST عامل()(ثابت T* s1, ثابت T* s2, ثابت uchar* نقاب, صحیح len, صحیح cn) ثابت {
        ST نتیجہ = 0;
        برائے (صحیح i = 0; i < len; i++) اگر (نقاب[i]) {
            ثابت T* e1 = s1 + i*cn; ثابت T* e2 = s2 + i*cn;
            برائے (صحیح k = 0; k < cn; k++)
                نتیجہ += (ST)cv_absdiff(e1[k], e2[k]);
        }
        واپس نتیجہ;
    }
};
سانچہ <قسم_نام T, قسم_نام ST>
ساخت MaskedNormDiffL2_SIMD {
    اندرونی ST عامل()(ثابت T* s1, ثابت T* s2, ثابت uchar* نقاب, صحیح len, صحیح cn) ثابت {
        ST نتیجہ = 0;
        برائے (صحیح i = 0; i < len; i++) اگر (نقاب[i]) {
            ثابت T* e1 = s1 + i*cn; ثابت T* e2 = s2 + i*cn;
            برائے (صحیح k = 0; k < cn; k++) { ST v = (ST)e1[k] - (ST)e2[k]; نتیجہ += v*v; }
        }
        واپس نتیجہ;
    }
};

#اگر (CV_SIMD || CV_SIMD_SCALABLE)
// AND a per-lane diff vector with the (expanded) mask>0 predicate.
اندرونی v_uint8  v_diffmask(ثابت v_uint8&  d, ثابت uchar* m) { واپس v_and(d, v_gt(vx_بھرو(m),          vx_setzero_u8())); }
اندرونی v_uint16 v_diffmask(ثابت v_uint16& d, ثابت uchar* m) { واپس v_and(d, v_gt(vx_بھرو_expand(m),   vx_setzero_u16())); }
اندرونی v_uint32 v_diffmask(ثابت v_uint32& d, ثابت uchar* m) { واپس v_and(d, v_gt(vx_بھرو_expand_q(m), vx_setzero_u32())); }
اندرونی v_float32 v_diffmask(ثابت v_float32& d, ثابت uchar* m) {
    v_uint32 cm = v_gt(vx_بھرو_expand_q(m), vx_setzero_u32());
    واپس v_reinterpret_as_f32(v_and(v_reinterpret_as_u32(d), cm));
}

// Shared scalar cn>1 fallbacks (match the base templates' cv_absdiff semantics).
سانچہ<قسم_نام T, قسم_نام RT>
ساکن اندرونی RT masked_Norm_Diff_Inf_دم(ثابت T* s1, ثابت T* s2, ثابت uchar* نقاب, صحیح len, صحیح cn, RT نتیجہ) {
    برائے (صحیح i = 0; i < len; i++) اگر (نقاب[i]) {
        ثابت T* e1 = s1 + i*cn; ثابت T* e2 = s2 + i*cn;
        برائے (صحیح k = 0; k < cn; k++) نتیجہ = std::max(نتیجہ, (RT)cv_absdiff(e1[k], e2[k]));
    }
    واپس نتیجہ;
}
سانچہ<قسم_نام T, قسم_نام RT>
ساکن اندرونی RT masked_Norm_Diff_L1_دم(ثابت T* s1, ثابت T* s2, ثابت uchar* نقاب, صحیح len, صحیح cn, RT نتیجہ) {
    برائے (صحیح i = 0; i < len; i++) اگر (نقاب[i]) {
        ثابت T* e1 = s1 + i*cn; ثابت T* e2 = s2 + i*cn;
        برائے (صحیح k = 0; k < cn; k++) نتیجہ += (RT)cv_absdiff(e1[k], e2[k]);
    }
    واپس نتیجہ;
}
سانچہ<قسم_نام T, قسم_نام RT>
ساکن اندرونی RT masked_Norm_Diff_L2_دم(ثابت T* s1, ثابت T* s2, ثابت uchar* نقاب, صحیح len, صحیح cn, RT نتیجہ) {
    برائے (صحیح i = 0; i < len; i++) اگر (نقاب[i]) {
        ثابت T* e1 = s1 + i*cn; ثابت T* e2 = s2 + i*cn;
        برائے (صحیح k = 0; k < cn; k++) { RT v = (RT)e1[k] - (RT)e2[k]; نتیجہ += v*v; }
    }
    واپس نتیجہ;
}

// Inf (cn==1): reduce_max of masked |a-b|. Shared across SIMD-capable depths.
سانچہ<قسم_نام T, قسم_نام ST>
ساکن اندرونی ST maskedNormDiffInfCn1(ثابت T* s1, ثابت T* s2, ثابت uchar* نقاب, صحیح len) {
    ثابت صحیح vstep = VTraits<decltype(vx_بھرو(s1))>::vlanes();
    ST نتیجہ = 0;
    صحیح i = 0;
    اگر (len >= vstep) {
        خودکار acc = v_diffmask(v_absdiff(vx_بھرو(s1), vx_بھرو(s2)), نقاب);
        برائے (i = vstep; i <= len - vstep; i += vstep)
            acc = v_max(acc, v_diffmask(v_absdiff(vx_بھرو(s1 + i), vx_بھرو(s2 + i)), نقاب + i));
        نتیجہ = (ST)v_reduce_max(acc);
    }
    برائے (; i < len; i++) اگر (نقاب[i]) نتیجہ = std::max(نتیجہ, (ST)cv_absdiff(s1[i], s2[i]));
    واپس نتیجہ;
}

// Shared 8-bit cn==1 masked L1/L2 kernels (uchar/schar): v_absdiff yields v_uint8 for both.
سانچہ<قسم_نام T>
ساکن اندرونی صحیح maskedNormDiffL1_8(ثابت T* s1, ثابت T* s2, ثابت uchar* نقاب, صحیح len) {
    صحیح i = 0; ثابت صحیح vstep = VTraits<v_uint8>::vlanes();
    ثابت v_uint8 one = vx_setall_u8(1);
    v_uint32 acc = vx_setzero_u32();
    برائے (; i <= len - vstep; i += vstep) {
        v_uint8 m  = v_gt(vx_بھرو(نقاب + i), vx_setzero_u8());
        v_uint8 ad = v_and(v_absdiff(vx_بھرو(s1 + i), vx_بھرو(s2 + i)), m);
        acc = v_dotprod_expand_fast(ad, one, acc);
    }
    صحیح نتیجہ = (صحیح)v_reduce_مجموع(acc);
    برائے (; i < len; i++) اگر (نقاب[i]) نتیجہ += (صحیح)cv_absdiff(s1[i], s2[i]);
    واپس نتیجہ;
}
سانچہ<قسم_نام T>
ساکن اندرونی صحیح maskedNormDiffL2_8(ثابت T* s1, ثابت T* s2, ثابت uchar* نقاب, صحیح len) {
    صحیح i = 0; ثابت صحیح vstep = VTraits<v_uint8>::vlanes();
    v_uint32 acc = vx_setzero_u32();
    برائے (; i <= len - vstep; i += vstep) {
        v_uint8 m  = v_gt(vx_بھرو(نقاب + i), vx_setzero_u8());
        v_uint8 ad = v_and(v_absdiff(vx_بھرو(s1 + i), vx_بھرو(s2 + i)), m);
        acc = v_dotprod_expand_fast(ad, ad, acc);
    }
    صحیح نتیجہ = (صحیح)v_reduce_مجموع(acc);
    برائے (; i < len; i++) اگر (نقاب[i]) { صحیح v = (صحیح)s1[i] - (صحیح)s2[i]; نتیجہ += v*v; }
    واپس نتیجہ;
}

// Shared 16-bit cn==1 masked L1 kernel (ushort/short): v_absdiff yields v_uint16 for both.
سانچہ<قسم_نام T>
ساکن اندرونی صحیح maskedNormDiffL1_16(ثابت T* s1, ثابت T* s2, ثابت uchar* نقاب, صحیح len) {
    صحیح i = 0; ثابت صحیح vstep = VTraits<v_uint16>::vlanes();
    v_uint32 acc = vx_setzero_u32();
    برائے (; i <= len - vstep; i += vstep) {
        v_uint16 m  = v_gt(vx_بھرو_expand(نقاب + i), vx_setzero_u16());
        v_uint16 ad = v_and(v_absdiff(vx_بھرو(s1 + i), vx_بھرو(s2 + i)), m);
        v_uint32 lo, hi; v_expand(ad, lo, hi);
        acc = v_جوڑو(acc, v_جوڑو(lo, hi));
    }
    صحیح نتیجہ = (صحیح)v_reduce_مجموع(acc);
    برائے (; i < len; i++) اگر (نقاب[i]) نتیجہ += (صحیح)cv_absdiff(s1[i], s2[i]);
    واپس نتیجہ;
}

سانچہ<> ساخت MaskedNormDiffInf_SIMD<uchar, صحیح> {
    صحیح عامل()(ثابت uchar* s1, ثابت uchar* s2, ثابت uchar* نقاب, صحیح len, صحیح cn) ثابت {
        اگر (cn == 1) واپس maskedNormDiffInfCn1<uchar, صحیح>(s1, s2, نقاب, len);
        اگر (cn == 4 && len >= VTraits<v_uint8>::vlanes()) {
            ثابت صحیح vstep = VTraits<v_uint8>::vlanes();
            v_uint8 acc = vx_setzero_u8();
            صحیح i = 0;
            برائے (;;) {
                اگر (i > len - vstep) i = len - vstep; // back-step (max is idempotent)
                v_uint8 a0,a1,a2,a3,b0,b1,b2,b3;
                v_بھرو_deinterleave(s1 + i*4, a0,a1,a2,a3);
                v_بھرو_deinterleave(s2 + i*4, b0,b1,b2,b3);
                v_uint8 ad = v_max(v_max(v_absdiff(a0,b0), v_absdiff(a1,b1)),
                                   v_max(v_absdiff(a2,b2), v_absdiff(a3,b3)));
                v_uint8 m = v_gt(vx_بھرو(نقاب + i), vx_setzero_u8());
                acc = v_max(acc, v_and(ad, m));
                اگر (i >= len - vstep) توڑو;
                i += vstep;
            }
            واپس (صحیح)v_reduce_max(acc);
        }
        واپس masked_Norm_Diff_Inf_دم<uchar, صحیح>(s1, s2, نقاب, len, cn, 0);
    }
};
سانچہ<> ساخت MaskedNormDiffInf_SIMD<schar, صحیح> {
    صحیح عامل()(ثابت schar* s1, ثابت schar* s2, ثابت uchar* نقاب, صحیح len, صحیح cn) ثابت {
        اگر (cn == 1) واپس maskedNormDiffInfCn1<schar, صحیح>(s1, s2, نقاب, len);
        واپس masked_Norm_Diff_Inf_دم<schar, صحیح>(s1, s2, نقاب, len, cn, 0);
    }
};
سانچہ<> ساخت MaskedNormDiffInf_SIMD<ushort, صحیح> {
    صحیح عامل()(ثابت ushort* s1, ثابت ushort* s2, ثابت uchar* نقاب, صحیح len, صحیح cn) ثابت {
        اگر (cn == 1) واپس maskedNormDiffInfCn1<ushort, صحیح>(s1, s2, نقاب, len);
        واپس masked_Norm_Diff_Inf_دم<ushort, صحیح>(s1, s2, نقاب, len, cn, 0);
    }
};
سانچہ<> ساخت MaskedNormDiffInf_SIMD<مختصر, صحیح> {
    صحیح عامل()(ثابت مختصر* s1, ثابت مختصر* s2, ثابت uchar* نقاب, صحیح len, صحیح cn) ثابت {
        اگر (cn == 1) واپس maskedNormDiffInfCn1<مختصر, صحیح>(s1, s2, نقاب, len);
        واپس masked_Norm_Diff_Inf_دم<مختصر, صحیح>(s1, s2, نقاب, len, cn, 0);
    }
};
سانچہ<> ساخت MaskedNormDiffInf_SIMD<صحیح, غیرعلامتی> {
    غیرعلامتی عامل()(ثابت صحیح* s1, ثابت صحیح* s2, ثابت uchar* نقاب, صحیح len, صحیح cn) ثابت {
        اگر (cn == 1) واپس maskedNormDiffInfCn1<صحیح, غیرعلامتی>(s1, s2, نقاب, len);
        واپس masked_Norm_Diff_Inf_دم<صحیح, غیرعلامتی>(s1, s2, نقاب, len, cn, 0u);
    }
};
سانچہ<> ساخت MaskedNormDiffInf_SIMD<اعشاری, اعشاری> {
    اعشاری عامل()(ثابت اعشاری* s1, ثابت اعشاری* s2, ثابت uchar* نقاب, صحیح len, صحیح cn) ثابت {
        اگر (cn == 1) واپس maskedNormDiffInfCn1<اعشاری, اعشاری>(s1, s2, نقاب, len);
        واپس masked_Norm_Diff_Inf_دم<اعشاری, اعشاری>(s1, s2, نقاب, len, cn, 0.0f);
    }
};

سانچہ<> ساخت MaskedNormDiffL1_SIMD<uchar, صحیح> {
    صحیح عامل()(ثابت uchar* s1, ثابت uchar* s2, ثابت uchar* نقاب, صحیح len, صحیح cn) ثابت {
        اگر (cn == 1) واپس maskedNormDiffL1_8(s1, s2, نقاب, len);
        اگر (cn == 4) {
            صحیح i = 0; ثابت صحیح vstep = VTraits<v_uint8>::vlanes();
            ثابت v_uint8 one = vx_setall_u8(1);
            v_uint32 acc = vx_setzero_u32();
            برائے (; i <= len - vstep; i += vstep) {
                v_uint8 a0,a1,a2,a3,b0,b1,b2,b3;
                v_بھرو_deinterleave(s1 + i*4, a0,a1,a2,a3);
                v_بھرو_deinterleave(s2 + i*4, b0,b1,b2,b3);
                v_uint8 m = v_gt(vx_بھرو(نقاب + i), vx_setzero_u8());
                acc = v_dotprod_expand_fast(v_and(v_absdiff(a0,b0), m), one, acc);
                acc = v_dotprod_expand_fast(v_and(v_absdiff(a1,b1), m), one, acc);
                acc = v_dotprod_expand_fast(v_and(v_absdiff(a2,b2), m), one, acc);
                acc = v_dotprod_expand_fast(v_and(v_absdiff(a3,b3), m), one, acc);
            }
            صحیح نتیجہ = (صحیح)v_reduce_مجموع(acc);
            برائے (; i < len; i++) اگر (نقاب[i]) {
                ثابت uchar* e1 = s1 + i*4; ثابت uchar* e2 = s2 + i*4;
                برائے (صحیح k = 0; k < 4; k++) نتیجہ += (صحیح)cv_absdiff(e1[k], e2[k]);
            }
            واپس نتیجہ;
        }
        واپس masked_Norm_Diff_L1_دم<uchar, صحیح>(s1, s2, نقاب, len, cn, 0);
    }
};
سانچہ<> ساخت MaskedNormDiffL1_SIMD<schar, صحیح> {
    صحیح عامل()(ثابت schar* s1, ثابت schar* s2, ثابت uchar* نقاب, صحیح len, صحیح cn) ثابت {
        اگر (cn == 1) واپس maskedNormDiffL1_8(s1, s2, نقاب, len);
        واپس masked_Norm_Diff_L1_دم<schar, صحیح>(s1, s2, نقاب, len, cn, 0);
    }
};
سانچہ<> ساخت MaskedNormDiffL1_SIMD<ushort, صحیح> {
    صحیح عامل()(ثابت ushort* s1, ثابت ushort* s2, ثابت uchar* نقاب, صحیح len, صحیح cn) ثابت {
        اگر (cn == 1) واپس maskedNormDiffL1_16(s1, s2, نقاب, len);
        واپس masked_Norm_Diff_L1_دم<ushort, صحیح>(s1, s2, نقاب, len, cn, 0);
    }
};
سانچہ<> ساخت MaskedNormDiffL1_SIMD<مختصر, صحیح> {
    صحیح عامل()(ثابت مختصر* s1, ثابت مختصر* s2, ثابت uchar* نقاب, صحیح len, صحیح cn) ثابت {
        اگر (cn == 1) واپس maskedNormDiffL1_16(s1, s2, نقاب, len);
        واپس masked_Norm_Diff_L1_دم<مختصر, صحیح>(s1, s2, نقاب, len, cn, 0);
    }
};

سانچہ<> ساخت MaskedNormDiffL2_SIMD<uchar, صحیح> {
    صحیح عامل()(ثابت uchar* s1, ثابت uchar* s2, ثابت uchar* نقاب, صحیح len, صحیح cn) ثابت {
        اگر (cn == 1) واپس maskedNormDiffL2_8(s1, s2, نقاب, len);
        اگر (cn == 4) {
            صحیح i = 0; ثابت صحیح vstep = VTraits<v_uint8>::vlanes();
            v_uint32 acc = vx_setzero_u32();
            برائے (; i <= len - vstep; i += vstep) {
                v_uint8 a0,a1,a2,a3,b0,b1,b2,b3;
                v_بھرو_deinterleave(s1 + i*4, a0,a1,a2,a3);
                v_بھرو_deinterleave(s2 + i*4, b0,b1,b2,b3);
                v_uint8 m = v_gt(vx_بھرو(نقاب + i), vx_setzero_u8());
                v_uint8 d0 = v_and(v_absdiff(a0,b0), m), d1 = v_and(v_absdiff(a1,b1), m);
                v_uint8 d2 = v_and(v_absdiff(a2,b2), m), d3 = v_and(v_absdiff(a3,b3), m);
                acc = v_dotprod_expand_fast(d0, d0, acc);
                acc = v_dotprod_expand_fast(d1, d1, acc);
                acc = v_dotprod_expand_fast(d2, d2, acc);
                acc = v_dotprod_expand_fast(d3, d3, acc);
            }
            صحیح نتیجہ = (صحیح)v_reduce_مجموع(acc);
            برائے (; i < len; i++) اگر (نقاب[i]) {
                ثابت uchar* e1 = s1 + i*4; ثابت uchar* e2 = s2 + i*4;
                برائے (صحیح k = 0; k < 4; k++) { صحیح v = (صحیح)e1[k] - (صحیح)e2[k]; نتیجہ += v*v; }
            }
            واپس نتیجہ;
        }
        واپس masked_Norm_Diff_L2_دم<uchar, صحیح>(s1, s2, نقاب, len, cn, 0);
    }
};
سانچہ<> ساخت MaskedNormDiffL2_SIMD<schar, صحیح> {
    صحیح عامل()(ثابت schar* s1, ثابت schar* s2, ثابت uchar* نقاب, صحیح len, صحیح cn) ثابت {
        اگر (cn == 1) واپس maskedNormDiffL2_8(s1, s2, نقاب, len);
        واپس masked_Norm_Diff_L2_دم<schar, صحیح>(s1, s2, نقاب, len, cn, 0);
    }
};

#اگر CV_SIMD_64F
// Depths whose L1/L2 accumulate in double.
سانچہ<> ساخت MaskedNormDiffL1_SIMD<صحیح, دوہرا> {
    دوہرا عامل()(ثابت صحیح* s1, ثابت صحیح* s2, ثابت uchar* نقاب, صحیح len, صحیح cn) ثابت {
        اگر (cn != 1) واپس masked_Norm_Diff_L1_دم<صحیح, دوہرا>(s1, s2, نقاب, len, cn, 0.0);
        صحیح i = 0; ثابت صحیح vstep = VTraits<v_int32>::vlanes();
        v_float64 acc = vx_setzero_f64();
        برائے (; i <= len - vstep; i += vstep) {
            v_uint32 m  = v_gt(vx_بھرو_expand_q(نقاب + i), vx_setzero_u32());
            v_uint32 ad = v_and(v_absdiff(vx_بھرو(s1 + i), vx_بھرو(s2 + i)), m);
            v_uint64 e0, e1; v_expand(ad, e0, e1);
            acc = v_جوڑو(acc, v_cvt_f64(v_reinterpret_as_s64(e0)));
            acc = v_جوڑو(acc, v_cvt_f64(v_reinterpret_as_s64(e1)));
        }
        دوہرا نتیجہ = v_reduce_مجموع(acc);
        برائے (; i < len; i++) اگر (نقاب[i]) نتیجہ += (دوہرا)cv_absdiff(s1[i], s2[i]);
        واپس نتیجہ;
    }
};
سانچہ<> ساخت MaskedNormDiffL1_SIMD<اعشاری, دوہرا> {
    دوہرا عامل()(ثابت اعشاری* s1, ثابت اعشاری* s2, ثابت uchar* نقاب, صحیح len, صحیح cn) ثابت {
        اگر (cn != 1) واپس masked_Norm_Diff_L1_دم<اعشاری, دوہرا>(s1, s2, نقاب, len, cn, 0.0);
        صحیح i = 0; ثابت صحیح vstep = VTraits<v_float32>::vlanes();
        v_float64 acc0 = vx_setzero_f64(), acc1 = vx_setzero_f64();
        برائے (; i <= len - vstep; i += vstep) {
            v_float32 m  = v_reinterpret_as_f32(v_gt(vx_بھرو_expand_q(نقاب + i), vx_setzero_u32()));
            v_float32 ad = v_and(v_absdiff(vx_بھرو(s1 + i), vx_بھرو(s2 + i)), m);
            acc0 = v_جوڑو(acc0, v_cvt_f64(ad));
            acc1 = v_جوڑو(acc1, v_cvt_f64_بلند(ad));
        }
        دوہرا نتیجہ = v_reduce_مجموع(v_جوڑو(acc0, acc1));
        برائے (; i < len; i++) اگر (نقاب[i]) نتیجہ += (دوہرا)cv_absdiff(s1[i], s2[i]);
        واپس نتیجہ;
    }
};

سانچہ<قسم_نام T>
ساکن اندرونی دوہرا maskedNormDiffL2_16(ثابت T* s1, ثابت T* s2, ثابت uchar* نقاب, صحیح len, صحیح cn) {
    اگر (cn != 1) واپس masked_Norm_Diff_L2_دم<T, دوہرا>(s1, s2, نقاب, len, cn, 0.0);
    صحیح i = 0; ثابت صحیح vstep = VTraits<v_uint16>::vlanes();
    v_float64 acc = vx_setzero_f64();
    برائے (; i <= len - vstep; i += vstep) {
        v_uint16 m  = v_gt(vx_بھرو_expand(نقاب + i), vx_setzero_u16());
        v_uint16 ad = v_and(v_absdiff(vx_بھرو(s1 + i), vx_بھرو(s2 + i)), m);
        v_uint64 u = v_dotprod_expand_fast(ad, ad);
        acc = v_جوڑو(acc, v_cvt_f64(v_reinterpret_as_s64(u)));
    }
    دوہرا نتیجہ = v_reduce_مجموع(acc);
    برائے (; i < len; i++) اگر (نقاب[i]) { دوہرا v = (دوہرا)s1[i] - (دوہرا)s2[i]; نتیجہ += v*v; }
    واپس نتیجہ;
}
سانچہ<> ساخت MaskedNormDiffL2_SIMD<ushort, دوہرا> {
    دوہرا عامل()(ثابت ushort* s1, ثابت ushort* s2, ثابت uchar* نقاب, صحیح len, صحیح cn) ثابت
    { واپس maskedNormDiffL2_16(s1, s2, نقاب, len, cn); } };
سانچہ<> ساخت MaskedNormDiffL2_SIMD<مختصر, دوہرا> {
    دوہرا عامل()(ثابت مختصر* s1, ثابت مختصر* s2, ثابت uchar* نقاب, صحیح len, صحیح cn) ثابت
    { واپس maskedNormDiffL2_16(s1, s2, نقاب, len, cn); } };

سانچہ<> ساخت MaskedNormDiffL2_SIMD<صحیح, دوہرا> {
    دوہرا عامل()(ثابت صحیح* s1, ثابت صحیح* s2, ثابت uchar* نقاب, صحیح len, صحیح cn) ثابت {
        اگر (cn != 1) واپس masked_Norm_Diff_L2_دم<صحیح, دوہرا>(s1, s2, نقاب, len, cn, 0.0);
        صحیح i = 0; ثابت صحیح vstep = VTraits<v_int32>::vlanes();
        v_float64 r0 = vx_setzero_f64(), r1 = vx_setzero_f64();
        برائے (; i <= len - vstep; i += vstep) {
            v_uint32 m  = v_gt(vx_بھرو_expand_q(نقاب + i), vx_setzero_u32());
            v_uint32 ad = v_and(v_absdiff(vx_بھرو(s1 + i), vx_بھرو(s2 + i)), m);
            v_uint64 e0, e1; v_expand(ad, e0, e1);
            v_float64 f0 = v_cvt_f64(v_reinterpret_as_s64(e0)), f1 = v_cvt_f64(v_reinterpret_as_s64(e1));
            r0 = v_fma(f0, f0, r0); r1 = v_fma(f1, f1, r1);
        }
        دوہرا نتیجہ = v_reduce_مجموع(v_جوڑو(r0, r1));
        برائے (; i < len; i++) اگر (نقاب[i]) { دوہرا v = (دوہرا)s1[i] - (دوہرا)s2[i]; نتیجہ += v*v; }
        واپس نتیجہ;
    }
};
سانچہ<> ساخت MaskedNormDiffL2_SIMD<اعشاری, دوہرا> {
    دوہرا عامل()(ثابت اعشاری* s1, ثابت اعشاری* s2, ثابت uchar* نقاب, صحیح len, صحیح cn) ثابت {
        اگر (cn != 1) واپس masked_Norm_Diff_L2_دم<اعشاری, دوہرا>(s1, s2, نقاب, len, cn, 0.0);
        صحیح i = 0; ثابت صحیح vstep = VTraits<v_float32>::vlanes();
        v_float64 r0 = vx_setzero_f64(), r1 = vx_setzero_f64();
        برائے (; i <= len - vstep; i += vstep) {
            v_float32 m  = v_reinterpret_as_f32(v_gt(vx_بھرو_expand_q(نقاب + i), vx_setzero_u32()));
            v_float32 ad = v_and(v_absdiff(vx_بھرو(s1 + i), vx_بھرو(s2 + i)), m);
            v_float64 f0 = v_cvt_f64(ad), f1 = v_cvt_f64_بلند(ad);
            r0 = v_fma(f0, f0, r0); r1 = v_fma(f1, f1, r1);
        }
        دوہرا نتیجہ = v_reduce_مجموع(v_جوڑو(r0, r1));
        برائے (; i < len; i++) اگر (نقاب[i]) { دوہرا v = (دوہرا)s1[i] - (دوہرا)s2[i]; نتیجہ += v*v; }
        واپس نتیجہ;
    }
};
سانچہ<>
ساخت MaskedNormDiffInf_SIMD<دوہرا, دوہرا> {
    اندرونی دوہرا عامل()(ثابت دوہرا* s1, ثابت دوہرا* s2, ثابت uchar* نقاب, صحیح len, صحیح cn) ثابت {
        دوہرا نتیجہ = 0.0;
        صحیح i = 0;
#اگر CV_SIMD_64F || CV_SIMD_SCALABLE_64F
        اگر (cn == 1) {
            ثابت صحیح vstep = VTraits<v_float64>::vlanes();
            v_float64 acc0 = vx_setzero_f64(), acc1 = vx_setzero_f64();
            برائے (; i <= len - 2*vstep; i += 2*vstep) {
                v_uint64 m0, m1;
                v_normmask_u64(نقاب + i, m0, m1);
                v_float64 d0 = v_absdiff(vx_بھرو(s1 + i), vx_بھرو(s2 + i));
                v_float64 d1 = v_absdiff(vx_بھرو(s1 + i + vstep), vx_بھرو(s2 + i + vstep));
                acc0 = v_max(acc0, v_reinterpret_as_f64(v_and(v_reinterpret_as_u64(d0), m0)));
                acc1 = v_max(acc1, v_reinterpret_as_f64(v_and(v_reinterpret_as_u64(d1), m1)));
            }
            دوہرا t[VTraits<v_float64>::max_nlanes];
            vx_store(t, v_max(acc0, acc1));
            برائے (صحیح k = 0; k < vstep; k++)
                نتیجہ = std::max(نتیجہ, t[k]);
        }
#اگر_ختم
        برائے (; i < len; i++) اگر (نقاب[i]) {
            ثابت دوہرا* e1 = s1 + i*cn; ثابت دوہرا* e2 = s2 + i*cn;
            برائے (صحیح k = 0; k < cn; k++) نتیجہ = std::max(نتیجہ, std::مطلق_قدر(e1[k] - e2[k]));
        }
        واپس نتیجہ;
    }
};

سانچہ<>
ساخت MaskedNormDiffL1_SIMD<دوہرا, دوہرا> {
    دوہرا عامل()(ثابت دوہرا* s1, ثابت دوہرا* s2, ثابت uchar* نقاب, صحیح len, صحیح cn) ثابت {
        دوہرا نتیجہ = 0.0;
        اگر (cn == 1) {
            ثابت صحیح vstep = VTraits<v_float64>::vlanes();
            صحیح i = 0;
            v_float64 acc0 = vx_setzero_f64(), acc1 = vx_setzero_f64();
            برائے (; i <= len - 2*vstep; i += 2*vstep) {
                v_uint64 m0, m1;
                v_normmask_u64(نقاب + i, m0, m1);
                v_float64 d0 = v_absdiff(vx_بھرو(s1 + i), vx_بھرو(s2 + i));
                v_float64 d1 = v_absdiff(vx_بھرو(s1 + i + vstep), vx_بھرو(s2 + i + vstep));
                acc0 = v_جوڑو(acc0, v_reinterpret_as_f64(v_and(v_reinterpret_as_u64(d0), m0)));
                acc1 = v_جوڑو(acc1, v_reinterpret_as_f64(v_and(v_reinterpret_as_u64(d1), m1)));
            }
            نتیجہ = v_reduce_مجموع(v_جوڑو(acc0, acc1));
            برائے (; i < len; i++) اگر (نقاب[i]) نتیجہ += std::مطلق_قدر(s1[i] - s2[i]);
        } ورنہ {
            نتیجہ = masked_Norm_Diff_L1_دم<دوہرا, دوہرا>(s1, s2, نقاب, len, cn, 0.0);
        }
        واپس نتیجہ;
    }
};

سانچہ<>
ساخت MaskedNormDiffL2_SIMD<دوہرا, دوہرا> {
    دوہرا عامل()(ثابت دوہرا* s1, ثابت دوہرا* s2, ثابت uchar* نقاب, صحیح len, صحیح cn) ثابت {
        دوہرا نتیجہ = 0.0;
        اگر (cn == 1) {
            ثابت صحیح vstep = VTraits<v_float64>::vlanes();
            صحیح i = 0;
            v_float64 acc0 = vx_setzero_f64(), acc1 = vx_setzero_f64();
            برائے (; i <= len - 2*vstep; i += 2*vstep) {
                v_uint64 m0, m1;
                v_normmask_u64(نقاب + i, m0, m1);
                v_float64 d0 = v_sub(vx_بھرو(s1 + i), vx_بھرو(s2 + i));
                v_float64 d1 = v_sub(vx_بھرو(s1 + i + vstep), vx_بھرو(s2 + i + vstep));
                d0 = v_reinterpret_as_f64(v_and(v_reinterpret_as_u64(d0), m0));
                d1 = v_reinterpret_as_f64(v_and(v_reinterpret_as_u64(d1), m1));
                acc0 = v_fma(d0, d0, acc0);
                acc1 = v_fma(d1, d1, acc1);
            }
            نتیجہ = v_reduce_مجموع(v_جوڑو(acc0, acc1));
            برائے (; i < len; i++) اگر (نقاب[i]) { دوہرا v = s1[i] - s2[i]; نتیجہ += v*v; }
        } ورنہ {
            نتیجہ = masked_Norm_Diff_L2_دم<دوہرا, دوہرا>(s1, s2, نقاب, len, cn, 0.0);
        }
        واپس نتیجہ;
    }
};
#اگر_ختم // CV_SIMD_64F
#اگر_ختم // CV_SIMD

سانچہ<قسم_نام T, قسم_نام ST> صحیح
normDiffInf_(ثابت T* src1, ثابت T* src2, ثابت uchar* نقاب, ST* نتیجہ_2, صحیح len, صحیح cn) {
    ST نتیجہ = *نتیجہ_2;
    اگر( !نقاب ) {
        NormDiffInf_SIMD<T, ST> op;
        نتیجہ = std::max(نتیجہ, op(src1, src2, len*cn));
    } ورنہ {
        MaskedNormDiffInf_SIMD<T, ST> op;
        نتیجہ = std::max(نتیجہ, op(src1, src2, نقاب, len, cn));
    }
    *نتیجہ_2 = نتیجہ;
    واپس 0;
}

سانچہ<قسم_نام T, قسم_نام ST> صحیح
normDiffL1_(ثابت T* src1, ثابت T* src2, ثابت uchar* نقاب, ST* نتیجہ_2, صحیح len, صحیح cn) {
    ST نتیجہ = *نتیجہ_2;
    اگر( !نقاب ) {
        NormDiffL1_SIMD<T, ST> op;
        نتیجہ += op(src1, src2, len*cn);
    } ورنہ {
        MaskedNormDiffL1_SIMD<T, ST> op;
        نتیجہ += op(src1, src2, نقاب, len, cn);
    }
    *نتیجہ_2 = نتیجہ;
    واپس 0;
}

سانچہ<قسم_نام T, قسم_نام ST> صحیح
normDiffL2_(ثابت T* src1, ثابت T* src2, ثابت uchar* نقاب, ST* نتیجہ_2, صحیح len, صحیح cn) {
    ST نتیجہ = *نتیجہ_2;
    اگر( !نقاب ) {
        NormDiffL2_SIMD<T, ST> op;
        نتیجہ += op(src1, src2, len*cn);
    } ورنہ {
        MaskedNormDiffL2_SIMD<T, ST> op;
        نتیجہ += op(src1, src2, نقاب, len, cn);
    }
    *نتیجہ_2 = نتیجہ;
    واپس 0;
}

ساکن صحیح
normDiffInf_Bool(ثابت uchar* src1, ثابت uchar* src2, ثابت uchar* نقاب, صحیح* نتیجہ_2, صحیح len, صحیح cn)
{
    صحیح نتیجہ = *نتیجہ_2;
    اگر( !نقاب )
    {
        برائے( صحیح i = 0; i < len*cn; i++ ) {
            نتیجہ = std::max(نتیجہ, (صحیح)((src1[i] != 0) != (src2[i] != 0)));
            اگر (نتیجہ != 0)
                توڑو;
        }
    }
    ورنہ
    {
        برائے( صحیح i = 0; i < len; i++, src1 += cn, src2 += cn )
            اگر( نقاب[i] )
            {
                برائے( صحیح k = 0; k < cn; k++ )
                    نتیجہ = std::max(نتیجہ, (صحیح)((src1[k] != 0) != (src2[k] != 0)));
                اگر (نتیجہ != 0)
                    توڑو;
            }
    }
    *نتیجہ_2 = نتیجہ;
    واپس 0;
}

ساکن صحیح
normDiffL1_Bool(ثابت uchar* src1, ثابت uchar* src2, ثابت uchar* نقاب, صحیح* نتیجہ_2, صحیح len, صحیح cn)
{
    صحیح نتیجہ = *نتیجہ_2;
    اگر( !نقاب )
    {
        برائے( صحیح i = 0; i < len*cn; i++ )
            نتیجہ += (صحیح)((src1[i] != 0) != (src2[i] != 0));
    }
    ورنہ
    {
        برائے( صحیح i = 0; i < len; i++, src1 += cn, src2 += cn )
            اگر( نقاب[i] )
            {
                برائے( صحیح k = 0; k < cn; k++ )
                    نتیجہ += (صحیح)((src1[k] != 0) != (src2[k] != 0));
            }
    }
    *نتیجہ_2 = نتیجہ;
    واپس 0;
}

ساکن صحیح
normDiffL2_Bool(ثابت uchar* src1, ثابت uchar* src2, ثابت uchar* نقاب, صحیح* نتیجہ_2, صحیح len, صحیح cn)
{
    واپس normDiffL1_Bool(src1, src2, نقاب, نتیجہ_2, len, cn);
}

#تعریف_کرو CV_DEF_NORM_FUNC(L, suffix, type, ntype) \
    ساکن صحیح norm##L##_##suffix(ثابت type* src, ثابت uchar* نقاب, ntype* r, صحیح len, صحیح cn) \
{ CV_INSTRUMENT_REGION(); واپس norm##L##_(src, نقاب, r, len, cn); } \
    ساکن صحیح normDiff##L##_##suffix(ثابت type* src1, ثابت type* src2, \
    ثابت uchar* نقاب, ntype* r, صحیح len, صحیح cn) \
{ واپس normDiff##L##_(src1, src2, نقاب, r, (صحیح)len, cn); }

#تعریف_کرو CV_DEF_NORM_ALL(suffix, type, inftype, l1type, l2type) \
    CV_DEF_NORM_FUNC(Inf, suffix, type, inftype) \
    CV_DEF_NORM_FUNC(L1, suffix, type, l1type) \
    CV_DEF_NORM_FUNC(L2, suffix, type, l2type)

CV_DEF_NORM_ALL(8u, uchar, صحیح, صحیح, صحیح)
CV_DEF_NORM_ALL(8s, schar, صحیح, صحیح, صحیح)
CV_DEF_NORM_ALL(16u, ushort, صحیح, صحیح, دوہرا)
CV_DEF_NORM_ALL(16s, مختصر, صحیح, صحیح, دوہرا)
CV_DEF_NORM_ALL(32u, غیرعلامتی, غیرعلامتی, دوہرا, دوہرا)
CV_DEF_NORM_ALL(32s, صحیح, غیرعلامتی, دوہرا, دوہرا)
CV_DEF_NORM_ALL(32f, اعشاری, اعشاری, دوہرا, دوہرا)
CV_DEF_NORM_ALL(64f, دوہرا, دوہرا, دوہرا, دوہرا)
CV_DEF_NORM_ALL(64u, uint64, uint64, دوہرا, دوہرا)
CV_DEF_NORM_ALL(64s, int64, uint64, دوہرا, دوہرا)
CV_DEF_NORM_ALL(16f, hfloat, اعشاری, اعشاری, اعشاری)
CV_DEF_NORM_ALL(16bf, bfloat, اعشاری, اعشاری, اعشاری)

NormFunc getNormFunc(صحیح normType, صحیح گہرائی)
{
    CV_INSTRUMENT_REGION();

    ساکن NormFunc normTab[3][CV_گہرائی_MAX] =
    {
        {
            normWrap<uchar, صحیح, normInf_8u>,
            normWrap<schar, صحیح, normInf_8s>,
            normWrap<ushort, صحیح, normInf_16u>,
            normWrap<مختصر, صحیح, normInf_16s>,
            normWrap<صحیح, غیرعلامتی, normInf_32s>,
            normWrap<اعشاری, اعشاری, normInf_32f>,
            normWrap<دوہرا, دوہرا, normInf_64f>,
            normWrap<hfloat, اعشاری, normInf_16f>,
            normWrap<bfloat, اعشاری, normInf_16bf>,
            normWrap<uchar, صحیح, normInf_Bool>,
            normWrap<uint64, uint64, normInf_64u>,
            normWrap<int64, uint64, normInf_64s>,
            normWrap<غیرعلامتی, غیرعلامتی, normInf_32u>,
            0
        },
        {
            normWrap<uchar, صحیح, normL1_8u>,
            normWrap<schar, صحیح, normL1_8s>,
            normWrap<ushort, صحیح, normL1_16u>,
            normWrap<مختصر, صحیح, normL1_16s>,
            normWrap<صحیح, دوہرا, normL1_32s>,
            normWrap<اعشاری, دوہرا, normL1_32f>,
            normWrap<دوہرا, دوہرا, normL1_64f>,
            normWrap<hfloat, اعشاری, normL1_16f>,
            normWrap<bfloat, اعشاری, normL1_16bf>,
            normWrap<uchar, صحیح, normL1_Bool>,
            normWrap<uint64, دوہرا, normL1_64u>,
            normWrap<int64, دوہرا, normL1_64s>,
            normWrap<غیرعلامتی, دوہرا, normL1_32u>,
            0
        },
        {
            normWrap<uchar, صحیح, normL2_8u>,
            normWrap<schar, صحیح, normL2_8s>,
            normWrap<ushort, دوہرا, normL2_16u>,
            normWrap<مختصر, دوہرا, normL2_16s>,
            normWrap<صحیح, دوہرا, normL2_32s>,
            normWrap<اعشاری, دوہرا, normL2_32f>,
            normWrap<دوہرا, دوہرا, normL2_64f>,
            normWrap<hfloat, اعشاری, normL2_16f>,
            normWrap<bfloat, اعشاری, normL2_16bf>,
            normWrap<uchar, صحیح, normL2_Bool>,
            normWrap<uint64, دوہرا, normL2_64u>,
            normWrap<int64, دوہرا, normL2_64s>,
            normWrap<غیرعلامتی, دوہرا, normL2_32u>,
            0
        }
    };

    اگر (normType >= 3 || normType < 0) واپس خالی_اشاریہ;

    واپس normTab[normType][گہرائی];
}

NormDiffFunc getNormDiffFunc(صحیح normType, صحیح گہرائی)
{
    ساکن NormDiffFunc normDiffTab[3][CV_گہرائی_MAX] =
    {
        {
            normDiffWrap<uchar, صحیح, normDiffInf_8u>,
            normDiffWrap<schar, صحیح, normDiffInf_8s>,
            normDiffWrap<ushort, صحیح, normDiffInf_16u>,
            normDiffWrap<مختصر, صحیح, normDiffInf_16s>,
            normDiffWrap<صحیح, غیرعلامتی, normDiffInf_32s>,
            normDiffWrap<اعشاری, اعشاری, normDiffInf_32f>,
            normDiffWrap<دوہرا, دوہرا, normDiffInf_64f>,
            normDiffWrap<hfloat, اعشاری, normDiffInf_16f>,
            normDiffWrap<bfloat, اعشاری, normDiffInf_16bf>,
            normDiffWrap<uchar, صحیح, normDiffInf_Bool>,
            normDiffWrap<uint64, uint64, normDiffInf_64u>,
            normDiffWrap<int64, uint64, normDiffInf_64s>,
            normDiffWrap<غیرعلامتی, غیرعلامتی, normDiffInf_32u>,
            0
        },
        {
            normDiffWrap<uchar, صحیح, normDiffL1_8u>,
            normDiffWrap<schar, صحیح, normDiffL1_8s>,
            normDiffWrap<ushort, صحیح, normDiffL1_16u>,
            normDiffWrap<مختصر, صحیح, normDiffL1_16s>,
            normDiffWrap<صحیح, دوہرا, normDiffL1_32s>,
            normDiffWrap<اعشاری, دوہرا, normDiffL1_32f>,
            normDiffWrap<دوہرا, دوہرا, normDiffL1_64f>,
            normDiffWrap<hfloat, اعشاری, normDiffL1_16f>,
            normDiffWrap<bfloat, اعشاری, normDiffL1_16bf>,
            normDiffWrap<uchar, صحیح, normDiffL1_Bool>,
            normDiffWrap<uint64, دوہرا, normDiffL1_64u>,
            normDiffWrap<int64, دوہرا, normDiffL1_64s>,
            normDiffWrap<غیرعلامتی, دوہرا, normDiffL1_32u>,
            0
        },
        {
            normDiffWrap<uchar, صحیح, normDiffL2_8u>,
            normDiffWrap<schar, صحیح, normDiffL2_8s>,
            normDiffWrap<ushort, دوہرا, normDiffL2_16u>,
            normDiffWrap<مختصر, دوہرا, normDiffL2_16s>,
            normDiffWrap<صحیح, دوہرا, normDiffL2_32s>,
            normDiffWrap<اعشاری, دوہرا, normDiffL2_32f>,
            normDiffWrap<دوہرا, دوہرا, normDiffL2_64f>,
            normDiffWrap<hfloat, اعشاری, normDiffL2_16f>,
            normDiffWrap<bfloat, اعشاری, normDiffL2_16bf>,
            normDiffWrap<uchar, صحیح, normDiffL2_Bool>,
            normDiffWrap<uint64, دوہرا, normDiffL2_64u>,
            normDiffWrap<int64, دوہرا, normDiffL2_64s>,
            normDiffWrap<غیرعلامتی, دوہرا, normDiffL2_32u>,
            0
        },
    };
    اگر (normType >= 3 || normType < 0) واپس خالی_اشاریہ;

    واپس normDiffTab[normType][گہرائی];
}

#اگر_ختم // CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

CV_CPU_OPTIMIZATION_NAMESPACE_انتہا

} // cv::
