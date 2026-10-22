// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html
//
// Copyright (C) 2025, SpaceMIT Inc., all rights reserved.
// Copyright (C) 2026, Advanced Micro Devices, Inc., all rights reserved.
// Third party copyrights are property of their respective owners.

#ਅਨ੍ਤਰ੍ਭਾਵ "precomp.hpp"

ਨਾਮਾਕਾਸ਼ cv {

ਵਰਤੋਂ NormFunc = ਪੂਰਨ (*)(ਅਚਲ uchar*, ਅਚਲ uchar*, uchar*, ਪੂਰਨ, ਪੂਰਨ);
ਵਰਤੋਂ NormDiffFunc = ਪੂਰਨ (*)(ਅਚਲ uchar*, ਅਚਲ uchar*, ਅਚਲ uchar*, uchar*, ਪੂਰਨ, ਪੂਰਨ);

CV_CPU_OPTIMIZATION_NAMESPACE_BEGIN

ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ T, ਕਿਸਮ_ਨਾਂ RT, ਪੂਰਨ (*fn)(ਅਚਲ T*, ਅਚਲ uchar*, RT*, ਪੂਰਨ, ਪੂਰਨ)>
ਸਥਿਰ ਪੂਰਨ normWrap(ਅਚਲ uchar* src1, ਅਚਲ uchar* src2, uchar* res, ਪੂਰਨ len, ਪੂਰਨ cn)
{
    ਵਾਪਸ fn((ਅਚਲ T*)src1, src2, (RT*)res, len, cn);
}

ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ T, ਕਿਸਮ_ਨਾਂ RT, ਪੂਰਨ (*fn)(ਅਚਲ T*, ਅਚਲ T*, ਅਚਲ uchar*, RT*, ਪੂਰਨ, ਪੂਰਨ)>
ਸਥਿਰ ਪੂਰਨ normDiffWrap(ਅਚਲ uchar* src1, ਅਚਲ uchar* src2, ਅਚਲ uchar* src3, uchar* res, ਪੂਰਨ len, ਪੂਰਨ cn)
{
    ਵਾਪਸ fn((ਅਚਲ T*)src1, (ਅਚਲ T*)src2, src3, (RT*)res, len, cn);
}

NormFunc getNormFunc(ਪੂਰਨ normType, ਪੂਰਨ ਡੂੰਘਾਈ);
NormDiffFunc getNormDiffFunc(ਪੂਰਨ normType, ਪੂਰਨ ਡੂੰਘਾਈ);

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T, ਕਿਸਮ_ਨਾਂ ST>
ਬਣਤਰ NormInf_SIMD {
    ਸਤਰਅੰਦਰ ST ਸੰਚਾਲਕ() (ਅਚਲ T* src, ਪੂਰਨ n) ਅਚਲ {
        ST s = 0;
        ਲਈ (ਪੂਰਨ i = 0; i < n; i++) {
            s = std::max(s, (ST)cv_abs(src[i]));
        }
        ਵਾਪਸ s;
    }
};

ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T, ਕਿਸਮ_ਨਾਂ ST>
ਬਣਤਰ NormL1_SIMD {
    ਸਤਰਅੰਦਰ ST ਸੰਚਾਲਕ() (ਅਚਲ T* src, ਪੂਰਨ n) ਅਚਲ {
        ST s = 0;
        ਲਈ (ਪੂਰਨ i = 0; i < n; i++) {
            s += cv_abs(src[i]);
        }
        ਵਾਪਸ s;
    }
};

ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T, ਕਿਸਮ_ਨਾਂ ST>
ਬਣਤਰ NormL2_SIMD {
    ਸਤਰਅੰਦਰ ST ਸੰਚਾਲਕ() (ਅਚਲ T* src, ਪੂਰਨ n) ਅਚਲ {
        ST s = 0;
        ਲਈ (ਪੂਰਨ i = 0; i < n; i++) {
            ST v = (ST)src[i];
            s += v * v;
        }
        ਵਾਪਸ s;
    }
};

ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T, ਕਿਸਮ_ਨਾਂ ST>
ਬਣਤਰ NormDiffInf_SIMD {
    ਸਤਰਅੰਦਰ ST ਸੰਚਾਲਕ() (ਅਚਲ T* src1, ਅਚਲ T* src2, ਪੂਰਨ n) ਅਚਲ {
        ST s = 0;
        ਲਈ (ਪੂਰਨ i = 0; i < n; i++) {
            ST v = (ST)cv_absdiff(src1[i], src2[i]);
            s = std::max(s, v);
        }
        ਵਾਪਸ s;
    }
};

ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T, ਕਿਸਮ_ਨਾਂ ST>
ਬਣਤਰ NormDiffL1_SIMD {
    ਸਤਰਅੰਦਰ ST ਸੰਚਾਲਕ() (ਅਚਲ T* src1, ਅਚਲ T* src2, ਪੂਰਨ n) ਅਚਲ {
        ST s = 0;
        ਲਈ (ਪੂਰਨ i = 0; i < n; i++) {
            ST v = (ST)cv_absdiff(src1[i], src2[i]);
            s += v;
        }
        ਵਾਪਸ s;
    }
};

ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T, ਕਿਸਮ_ਨਾਂ ST>
ਬਣਤਰ NormDiffL2_SIMD {
    ਸਤਰਅੰਦਰ ST ਸੰਚਾਲਕ() (ਅਚਲ T* src1, ਅਚਲ T* src2, ਪੂਰਨ n) ਅਚਲ {
        ST s = 0;
        ਲਈ (ਪੂਰਨ i = 0; i < n; i++) {
            ST v = (ST)src1[i] - (ST)src2[i];
            s += v * v;
        }
        ਵਾਪਸ s;
    }
};

#ਜੇ (CV_SIMD || CV_SIMD_SCALABLE)

ਸਾਂਚਾ<>
ਬਣਤਰ NormInf_SIMD<uchar, ਪੂਰਨ> {
    ਪੂਰਨ ਸੰਚਾਲਕ() (ਅਚਲ uchar* src, ਪੂਰਨ n) ਅਚਲ {
        ਪੂਰਨ j = 0;
        ਪੂਰਨ s = 0;
        v_uint8 r0 = vx_setzero_u8(), r1 = vx_setzero_u8();
        v_uint8 r2 = vx_setzero_u8(), r3 = vx_setzero_u8();
        ਲਈ (; j <= n - 4 * VTraits<v_uint8>::vlanes(); j += 4 * VTraits<v_uint8>::vlanes()) {
            r0 = v_max(r0, vx_ਲੱਦੋ(src + j                                 ));
            r1 = v_max(r1, vx_ਲੱਦੋ(src + j +     VTraits<v_uint8>::vlanes()));
            r2 = v_max(r2, vx_ਲੱਦੋ(src + j + 2 * VTraits<v_uint8>::vlanes()));
            r3 = v_max(r3, vx_ਲੱਦੋ(src + j + 3 * VTraits<v_uint8>::vlanes()));
        }
        r0 = v_max(r0, v_max(r1, v_max(r2, r3)));
        ਲਈ (; j < n; j++) {
            s = std::max(s, (ਪੂਰਨ)src[j]);
        }
        ਵਾਪਸ std::max(s, (ਪੂਰਨ)v_reduce_max(r0));
    }
};

ਸਾਂਚਾ<>
ਬਣਤਰ NormInf_SIMD<schar, ਪੂਰਨ> {
    ਪੂਰਨ ਸੰਚਾਲਕ() (ਅਚਲ schar* src, ਪੂਰਨ n) ਅਚਲ {
        ਪੂਰਨ j = 0;
        ਪੂਰਨ s = 0;
        v_uint8 r0 = vx_setzero_u8(), r1 = vx_setzero_u8();
        v_uint8 r2 = vx_setzero_u8(), r3 = vx_setzero_u8();
        ਲਈ (; j <= n - 4 * VTraits<v_int8>::vlanes(); j += 4 * VTraits<v_int8>::vlanes()) {
            r0 = v_max(r0, v_abs(vx_ਲੱਦੋ(src + j                                )));
            r1 = v_max(r1, v_abs(vx_ਲੱਦੋ(src + j +     VTraits<v_int8>::vlanes())));
            r2 = v_max(r2, v_abs(vx_ਲੱਦੋ(src + j + 2 * VTraits<v_int8>::vlanes())));
            r3 = v_max(r3, v_abs(vx_ਲੱਦੋ(src + j + 3 * VTraits<v_int8>::vlanes())));
        }
        r0 = v_max(r0, v_max(r1, v_max(r2, r3)));
        ਲਈ (; j < n; j++) {
            s = std::max(s, cv_abs(src[j]));
        }
        ਵਾਪਸ std::max(s, saturate_cast<ਪੂਰਨ>(v_reduce_max(r0)));
    }
};

ਸਾਂਚਾ<>
ਬਣਤਰ NormInf_SIMD<ushort, ਪੂਰਨ> {
    ਪੂਰਨ ਸੰਚਾਲਕ() (ਅਚਲ ushort* src, ਪੂਰਨ n) ਅਚਲ {
        ਪੂਰਨ j = 0;
        ਪੂਰਨ s = 0;
        v_uint16 d0 = vx_setzero_u16(), d1 = vx_setzero_u16();
        v_uint16 d2 = vx_setzero_u16(), d3 = vx_setzero_u16();
        ਲਈ (; j <= n - 4 * VTraits<v_uint16>::vlanes(); j += 4 * VTraits<v_uint16>::vlanes()) {
            d0 = v_max(d0, vx_ਲੱਦੋ(src + j                                  ));
            d1 = v_max(d1, vx_ਲੱਦੋ(src + j +     VTraits<v_uint16>::vlanes()));
            d2 = v_max(d2, vx_ਲੱਦੋ(src + j + 2 * VTraits<v_uint16>::vlanes()));
            d3 = v_max(d3, vx_ਲੱਦੋ(src + j + 3 * VTraits<v_uint16>::vlanes()));
        }
        d0 = v_max(d0, v_max(d1, v_max(d2, d3)));
        ਲਈ (; j < n; j++) {
            s = std::max(s, (ਪੂਰਨ)src[j]);
        }
        ਵਾਪਸ std::max(s, (ਪੂਰਨ)v_reduce_max(d0));
    }
};

ਸਾਂਚਾ<>
ਬਣਤਰ NormInf_SIMD<ਛੋਟਾ, ਪੂਰਨ> {
    ਪੂਰਨ ਸੰਚਾਲਕ() (ਅਚਲ ਛੋਟਾ* src, ਪੂਰਨ n) ਅਚਲ {
        ਪੂਰਨ j = 0;
        ਪੂਰਨ s = 0;
        v_uint16 d0 = vx_setzero_u16(), d1 = vx_setzero_u16();
        v_uint16 d2 = vx_setzero_u16(), d3 = vx_setzero_u16();
        ਲਈ (; j <= n - 4 * VTraits<v_int16>::vlanes(); j += 4 * VTraits<v_int16>::vlanes()) {
            d0 = v_max(d0, v_abs(vx_ਲੱਦੋ(src + j                                  )));
            d1 = v_max(d1, v_abs(vx_ਲੱਦੋ(src + j +     VTraits<v_int16>::vlanes())));
            d2 = v_max(d2, v_abs(vx_ਲੱਦੋ(src + j + 2 * VTraits<v_int16>::vlanes())));
            d3 = v_max(d3, v_abs(vx_ਲੱਦੋ(src + j + 3 * VTraits<v_int16>::vlanes())));
        }
        d0 = v_max(d0, v_max(d1, v_max(d2, d3)));
        ਲਈ (; j < n; j++) {
            s = std::max(s, saturate_cast<ਪੂਰਨ>(cv_abs(src[j])));
        }
        ਵਾਪਸ std::max(s, saturate_cast<ਪੂਰਨ>(v_reduce_max(d0)));
    }
};

ਸਾਂਚਾ<>
ਬਣਤਰ NormInf_SIMD<ਪੂਰਨ, ਪੂਰਨ> {
    ਪੂਰਨ ਸੰਚਾਲਕ() (ਅਚਲ ਪੂਰਨ* src, ਪੂਰਨ n) ਅਚਲ {
        ਪੂਰਨ j = 0;
        ਪੂਰਨ s = 0;
        v_uint32 r0 = vx_setzero_u32(), r1 = vx_setzero_u32();
        v_uint32 r2 = vx_setzero_u32(), r3 = vx_setzero_u32();
        ਲਈ (; j <= n - 4 * VTraits<v_int32>::vlanes(); j += 4 * VTraits<v_int32>::vlanes()) {
            r0 = v_max(r0, v_abs(vx_ਲੱਦੋ(src + j                                 )));
            r1 = v_max(r1, v_abs(vx_ਲੱਦੋ(src + j +     VTraits<v_int32>::vlanes())));
            r2 = v_max(r2, v_abs(vx_ਲੱਦੋ(src + j + 2 * VTraits<v_int32>::vlanes())));
            r3 = v_max(r3, v_abs(vx_ਲੱਦੋ(src + j + 3 * VTraits<v_int32>::vlanes())));
        }
        r0 = v_max(r0, v_max(r1, v_max(r2, r3)));
        s = std::max(s, saturate_cast<ਪੂਰਨ>(v_reduce_max(r0)));
        ਲਈ (; j < n; j++) {
            s = std::max(s, std::ਨਿਰਪੇਖ(src[j]));
        }
        ਵਾਪਸ s;
    }
};

ਸਾਂਚਾ<>
ਬਣਤਰ NormInf_SIMD<ਦਸ਼ਮਲਵ, ਦਸ਼ਮਲਵ> {
    ਦਸ਼ਮਲਵ ਸੰਚਾਲਕ() (ਅਚਲ ਦਸ਼ਮਲਵ* src, ਪੂਰਨ n) ਅਚਲ {
        ਪੂਰਨ j = 0;
        ਦਸ਼ਮਲਵ s = 0.f;
        v_float32 r0 = vx_setzero_f32(), r1 = vx_setzero_f32();
        v_float32 r2 = vx_setzero_f32(), r3 = vx_setzero_f32();
        ਲਈ (; j <= n - 4 * VTraits<v_float32>::vlanes(); j += 4 * VTraits<v_float32>::vlanes()) {
            r0 = v_max(r0, v_abs(vx_ਲੱਦੋ(src + j                                   )));
            r1 = v_max(r1, v_abs(vx_ਲੱਦੋ(src + j +     VTraits<v_float32>::vlanes())));
            r2 = v_max(r2, v_abs(vx_ਲੱਦੋ(src + j + 2 * VTraits<v_float32>::vlanes())));
            r3 = v_max(r3, v_abs(vx_ਲੱਦੋ(src + j + 3 * VTraits<v_float32>::vlanes())));
        }
        r0 = v_max(r0, v_max(r1, v_max(r2, r3)));
        ਲਈ (; j < n; j++) {
            s = std::max(s, cv_abs(src[j]));
        }
        ਵਾਪਸ std::max(s, v_reduce_max(r0));
    }
};

ਸਾਂਚਾ<>
ਬਣਤਰ NormL1_SIMD<uchar, ਪੂਰਨ> {
    ਪੂਰਨ ਸੰਚਾਲਕ() (ਅਚਲ uchar* src, ਪੂਰਨ n) ਅਚਲ {
        ਪੂਰਨ j = 0;
        ਪੂਰਨ s = 0;
        v_uint32 r0 = vx_setzero_u32(), r1 = vx_setzero_u32();
        v_uint8  one = vx_setall_u8(1);
        ਲਈ (; j<= n - 2 * VTraits<v_uint8>::vlanes(); j += 2 * VTraits<v_uint8>::vlanes()) {
            v_uint8 v0 = vx_ਲੱਦੋ(src + j);
            r0 = v_dotprod_expand_fast(v0, one, r0);

            v_uint8 v1 = vx_ਲੱਦੋ(src + j + VTraits<v_uint8>::vlanes());
            r1 = v_dotprod_expand_fast(v1, one, r1);
        }
        s += v_reduce_ਜੋੜ(v_ਜੋੜੋ(r0, r1));
        ਲਈ (; j < n; j++) {
            s += src[j];
        }
        ਵਾਪਸ s;
    }
};

ਸਾਂਚਾ<>
ਬਣਤਰ NormL1_SIMD<schar, ਪੂਰਨ> {
    ਪੂਰਨ ਸੰਚਾਲਕ() (ਅਚਲ schar* src, ਪੂਰਨ n) ਅਚਲ {
        ਪੂਰਨ j = 0;
        ਪੂਰਨ s = 0;
        v_uint32 r0 = vx_setzero_u32(), r1 = vx_setzero_u32();
        v_uint8  one = vx_setall_u8(1);
        ਲਈ (; j<= n - 2 * VTraits<v_int8>::vlanes(); j += 2 * VTraits<v_int8>::vlanes()) {
            v_uint8 v0 = v_abs(vx_ਲੱਦੋ(src + j));
            r0 = v_dotprod_expand_fast(v0, one, r0);

            v_uint8 v1 = v_abs(vx_ਲੱਦੋ(src + j + VTraits<v_int8>::vlanes()));
            r1 = v_dotprod_expand_fast(v1, one, r1);
        }
        s += v_reduce_ਜੋੜ(v_ਜੋੜੋ(r0, r1));
        ਲਈ (; j < n; j++) {
            s += saturate_cast<ਪੂਰਨ>(cv_abs(src[j]));
        }
        ਵਾਪਸ s;
    }
};

ਸਾਂਚਾ<>
ਬਣਤਰ NormL1_SIMD<ushort, ਪੂਰਨ> {
    ਪੂਰਨ ਸੰਚਾਲਕ() (ਅਚਲ ushort* src, ਪੂਰਨ n) ਅਚਲ {
        ਪੂਰਨ j = 0;
        ਪੂਰਨ s = 0;
        v_uint32 r00 = vx_setzero_u32(), r01 = vx_setzero_u32();
        v_uint32 r10 = vx_setzero_u32(), r11 = vx_setzero_u32();
        ਲਈ (; j<= n - 2 * VTraits<v_uint16>::vlanes(); j += 2 * VTraits<v_uint16>::vlanes()) {
            v_uint16 v0 = vx_ਲੱਦੋ(src + j);
            v_uint32 v00, v01;
            v_expand(v0, v00, v01);
            r00 = v_ਜੋੜੋ(r00, v00);
            r01 = v_ਜੋੜੋ(r01, v01);

            v_uint16 v1 = vx_ਲੱਦੋ(src + j + VTraits<v_uint16>::vlanes());
            v_uint32 v10, v11;
            v_expand(v1, v10, v11);
            r10 = v_ਜੋੜੋ(r10, v10);
            r11 = v_ਜੋੜੋ(r11, v11);
        }
        s += (ਪੂਰਨ)v_reduce_ਜੋੜ(v_ਜੋੜੋ(v_ਜੋੜੋ(v_ਜੋੜੋ(r00, r01), r10), r11));
        ਲਈ (; j < n; j++) {
            s += src[j];
        }
        ਵਾਪਸ s;
    }
};

ਸਾਂਚਾ<>
ਬਣਤਰ NormL1_SIMD<ਛੋਟਾ, ਪੂਰਨ> {
    ਪੂਰਨ ਸੰਚਾਲਕ() (ਅਚਲ ਛੋਟਾ* src, ਪੂਰਨ n) ਅਚਲ {
        ਪੂਰਨ j = 0;
        ਪੂਰਨ s = 0;
        v_uint32 r00 = vx_setzero_u32(), r01 = vx_setzero_u32();
        v_uint32 r10 = vx_setzero_u32(), r11 = vx_setzero_u32();
        ਲਈ (; j<= n - 2 * VTraits<v_int16>::vlanes(); j += 2 * VTraits<v_int16>::vlanes()) {
            v_uint16 v0 = v_abs(vx_ਲੱਦੋ(src + j));
            v_uint32 v00, v01;
            v_expand(v0, v00, v01);
            r00 = v_ਜੋੜੋ(r00, v00);
            r01 = v_ਜੋੜੋ(r01, v01);

            v_uint16 v1 = v_abs(vx_ਲੱਦੋ(src + j + VTraits<v_int16>::vlanes()));
            v_uint32 v10, v11;
            v_expand(v1, v10, v11);
            r10 = v_ਜੋੜੋ(r10, v10);
            r11 = v_ਜੋੜੋ(r11, v11);
        }
        s += (ਪੂਰਨ)v_reduce_ਜੋੜ(v_ਜੋੜੋ(v_ਜੋੜੋ(v_ਜੋੜੋ(r00, r01), r10), r11));
        ਲਈ (; j < n; j++) {
            s += saturate_cast<ਪੂਰਨ>(cv_abs(src[j]));
        }
        ਵਾਪਸ s;
    }
};

ਸਾਂਚਾ<>
ਬਣਤਰ NormL2_SIMD<uchar, ਪੂਰਨ> {
    ਪੂਰਨ ਸੰਚਾਲਕ() (ਅਚਲ uchar* src, ਪੂਰਨ n) ਅਚਲ {
        ਪੂਰਨ j = 0;
        ਪੂਰਨ s = 0;
        v_uint32 r0 = vx_setzero_u32(), r1 = vx_setzero_u32();
        ਲਈ (; j <= n - 2 * VTraits<v_uint8>::vlanes(); j += 2 * VTraits<v_uint8>::vlanes()) {
            v_uint8 v0 = vx_ਲੱਦੋ(src + j);
            r0 = v_dotprod_expand_fast(v0, v0, r0);

            v_uint8 v1 = vx_ਲੱਦੋ(src + j + VTraits<v_uint8>::vlanes());
            r1 = v_dotprod_expand_fast(v1, v1, r1);
        }
        s += v_reduce_ਜੋੜ(v_ਜੋੜੋ(r0, r1));
        ਲਈ (; j < n; j++) {
            ਪੂਰਨ v = saturate_cast<ਪੂਰਨ>(src[j]);
            s += v * v;
        }
        ਵਾਪਸ s;
    }
};

ਸਾਂਚਾ<>
ਬਣਤਰ NormL2_SIMD<schar, ਪੂਰਨ> {
    ਪੂਰਨ ਸੰਚਾਲਕ() (ਅਚਲ schar* src, ਪੂਰਨ n) ਅਚਲ {
        ਪੂਰਨ j = 0;
        ਪੂਰਨ s = 0;
        v_int32 r0 = vx_setzero_s32(), r1 = vx_setzero_s32();
        ਲਈ (; j <= n - 2 * VTraits<v_int8>::vlanes(); j += 2 * VTraits<v_int8>::vlanes()) {
            v_int8 v0 = vx_ਲੱਦੋ(src + j);
            r0 = v_dotprod_expand_fast(v0, v0, r0);
            v_int8 v1 = vx_ਲੱਦੋ(src + j + VTraits<v_int8>::vlanes());
            r1 = v_dotprod_expand_fast(v1, v1, r1);
        }
        s += v_reduce_ਜੋੜ(v_ਜੋੜੋ(r0, r1));
        ਲਈ (; j < n; j++) {
            ਪੂਰਨ v = saturate_cast<ਪੂਰਨ>(src[j]);
            s += v * v;
        }
        ਵਾਪਸ s;
    }
};

ਸਾਂਚਾ<>
ਬਣਤਰ NormDiffInf_SIMD<uchar, ਪੂਰਨ> {
    ਪੂਰਨ ਸੰਚਾਲਕ() (ਅਚਲ uchar* src1, ਅਚਲ uchar* src2, ਪੂਰਨ n) ਅਚਲ {
        ਪੂਰਨ j = 0;
        ਪੂਰਨ s = 0;
        v_uint8 r0 = vx_setzero_u8(), r1 = vx_setzero_u8();
        v_uint8 r2 = vx_setzero_u8(), r3 = vx_setzero_u8();
        ਲਈ (; j <= n - 4 * VTraits<v_uint8>::vlanes(); j += 4 * VTraits<v_uint8>::vlanes()) {
            v_uint8 v01 = vx_ਲੱਦੋ(src1 + j), v02 = vx_ਲੱਦੋ(src2 + j);
            r0 = v_max(r0, v_absdiff(v01, v02));

            v_uint8 v11 = vx_ਲੱਦੋ(src1 + j + VTraits<v_uint8>::vlanes()),
                    v12 = vx_ਲੱਦੋ(src2 + j + VTraits<v_uint8>::vlanes());
            r1 = v_max(r1, v_absdiff(v11, v12));

            v_uint8 v21 = vx_ਲੱਦੋ(src1 + j + 2 * VTraits<v_uint8>::vlanes()),
                    v22 = vx_ਲੱਦੋ(src2 + j + 2 * VTraits<v_uint8>::vlanes());
            r2 = v_max(r2, v_absdiff(v21, v22));

            v_uint8 v31 = vx_ਲੱਦੋ(src1 + j + 3 * VTraits<v_uint8>::vlanes()),
                    v32 = vx_ਲੱਦੋ(src2 + j + 3 * VTraits<v_uint8>::vlanes());
            r3 = v_max(r3, v_absdiff(v31, v32));
        }
        s = (ਪੂਰਨ)v_reduce_max(v_max(v_max(v_max(r0, r1), r2), r3));
        ਲਈ (; j < n; j++) {
            ਪੂਰਨ v = (ਪੂਰਨ)cv_absdiff(src1[j], src2[j]);
            s = std::max(s, v);
        }
        ਵਾਪਸ s;
    }
};

ਸਾਂਚਾ<>
ਬਣਤਰ NormDiffInf_SIMD<schar, ਪੂਰਨ> {
    ਪੂਰਨ ਸੰਚਾਲਕ() (ਅਚਲ schar* src1, ਅਚਲ schar* src2, ਪੂਰਨ n) ਅਚਲ {
        ਪੂਰਨ j = 0;
        ਪੂਰਨ s = 0;
        v_uint8 r0 = vx_setzero_u8(), r1 = vx_setzero_u8();
        v_uint8 r2 = vx_setzero_u8(), r3 = vx_setzero_u8();
        ਲਈ (; j <= n - 4 * VTraits<v_int8>::vlanes(); j += 4 * VTraits<v_int8>::vlanes()) {
            v_int8 v01 = vx_ਲੱਦੋ(src1 + j), v02 = vx_ਲੱਦੋ(src2 + j);
            r0 = v_max(r0, v_absdiff(v01, v02));

            v_int8 v11 = vx_ਲੱਦੋ(src1 + j + VTraits<v_int8>::vlanes()),
                   v12 = vx_ਲੱਦੋ(src2 + j + VTraits<v_int8>::vlanes());
            r1 = v_max(r1, v_absdiff(v11, v12));

            v_int8 v21 = vx_ਲੱਦੋ(src1 + j + 2 * VTraits<v_int8>::vlanes()),
                   v22 = vx_ਲੱਦੋ(src2 + j + 2 * VTraits<v_int8>::vlanes());
            r2 = v_max(r2, v_absdiff(v21, v22));

            v_int8 v31 = vx_ਲੱਦੋ(src1 + j + 3 * VTraits<v_int8>::vlanes()),
                   v32 = vx_ਲੱਦੋ(src2 + j + 3 * VTraits<v_int8>::vlanes());
            r3 = v_max(r3, v_absdiff(v31, v32));
        }
        s = (ਪੂਰਨ)v_reduce_max(v_max(v_max(v_max(r0, r1), r2), r3));
        ਲਈ (; j < n; j++) {
            ਪੂਰਨ v = (ਪੂਰਨ)cv_absdiff(src1[j], src2[j]);
            s = std::max(s, v);
        }
        ਵਾਪਸ s;
    }
};

ਸਾਂਚਾ<>
ਬਣਤਰ NormDiffInf_SIMD<ushort, ਪੂਰਨ> {
    ਪੂਰਨ ਸੰਚਾਲਕ() (ਅਚਲ ushort* src1, ਅਚਲ ushort* src2, ਪੂਰਨ n) ਅਚਲ {
        ਪੂਰਨ j = 0;
        ਪੂਰਨ s = 0;
        v_uint16 r0 = vx_setzero_u16(), r1 = vx_setzero_u16();
        v_uint16 r2 = vx_setzero_u16(), r3 = vx_setzero_u16();
        ਲਈ (; j <= n - 4 * VTraits<v_uint16>::vlanes(); j += 4 * VTraits<v_uint16>::vlanes()) {
            v_uint16 v01 = vx_ਲੱਦੋ(src1 + j), v02 = vx_ਲੱਦੋ(src2 + j);
            r0 = v_max(r0, v_absdiff(v01, v02));

            v_uint16 v11 = vx_ਲੱਦੋ(src1 + j + VTraits<v_uint16>::vlanes()),
                     v12 = vx_ਲੱਦੋ(src2 + j + VTraits<v_uint16>::vlanes());
            r1 = v_max(r1, v_absdiff(v11, v12));

            v_uint16 v21 = vx_ਲੱਦੋ(src1 + j + 2 * VTraits<v_uint16>::vlanes()),
                     v22 = vx_ਲੱਦੋ(src2 + j + 2 * VTraits<v_uint16>::vlanes());
            r2 = v_max(r2, v_absdiff(v21, v22));

            v_uint16 v31 = vx_ਲੱਦੋ(src1 + j + 3 * VTraits<v_uint16>::vlanes()),
                     v32 = vx_ਲੱਦੋ(src2 + j + 3 * VTraits<v_uint16>::vlanes());
            r3 = v_max(r3, v_absdiff(v31, v32));
        }
        s = (ਪੂਰਨ)v_reduce_max(v_max(v_max(v_max(r0, r1), r2), r3));
        ਲਈ (; j < n; j++) {
            ਪੂਰਨ v = (ਪੂਰਨ)cv_absdiff(src1[j], src2[j]);
            s = std::max(s, v);
        }
        ਵਾਪਸ s;
    }
};

ਸਾਂਚਾ<>
ਬਣਤਰ NormDiffInf_SIMD<ਛੋਟਾ, ਪੂਰਨ> {
    ਪੂਰਨ ਸੰਚਾਲਕ() (ਅਚਲ ਛੋਟਾ* src1, ਅਚਲ ਛੋਟਾ* src2, ਪੂਰਨ n) ਅਚਲ {
        ਪੂਰਨ j = 0;
        ਪੂਰਨ s = 0;
        v_uint16 r0 = vx_setzero_u16(), r1 = vx_setzero_u16();
        v_uint16 r2 = vx_setzero_u16(), r3 = vx_setzero_u16();
        ਲਈ (; j <= n - 4 * VTraits<v_int16>::vlanes(); j += 4 * VTraits<v_int16>::vlanes()) {
            v_int16 v01 = vx_ਲੱਦੋ(src1 + j), v02 = vx_ਲੱਦੋ(src2 + j);
            r0 = v_max(r0, v_absdiff(v01, v02));

            v_int16 v11 = vx_ਲੱਦੋ(src1 + j + VTraits<v_int16>::vlanes()),
                    v12 = vx_ਲੱਦੋ(src2 + j + VTraits<v_int16>::vlanes());
            r1 = v_max(r1, v_absdiff(v11, v12));

            v_int16 v21 = vx_ਲੱਦੋ(src1 + j + 2 * VTraits<v_int16>::vlanes()),
                    v22 = vx_ਲੱਦੋ(src2 + j + 2 * VTraits<v_int16>::vlanes());
            r2 = v_max(r2, v_absdiff(v21, v22));

            v_int16 v31 = vx_ਲੱਦੋ(src1 + j + 3 * VTraits<v_int16>::vlanes()),
                    v32 = vx_ਲੱਦੋ(src2 + j + 3 * VTraits<v_int16>::vlanes());
            r3 = v_max(r3, v_absdiff(v31, v32));
        }
        s = (ਪੂਰਨ)v_reduce_max(v_max(v_max(v_max(r0, r1), r2), r3));
        ਲਈ (; j < n; j++) {
            ਪੂਰਨ v = (ਪੂਰਨ)cv_absdiff(src1[j], src2[j]);
            s = std::max(s, v);
        }
        ਵਾਪਸ s;
    }
};

ਸਾਂਚਾ<>
ਬਣਤਰ NormDiffInf_SIMD<ਪੂਰਨ, ਬਿਨਾਨਿਸ਼ਾਨੀ> {
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਸੰਚਾਲਕ() (ਅਚਲ ਪੂਰਨ* src1, ਅਚਲ ਪੂਰਨ* src2, ਪੂਰਨ n) ਅਚਲ {
        ਪੂਰਨ j = 0;
        ਬਿਨਾਨਿਸ਼ਾਨੀ s = 0;
        v_uint32 r0 = vx_setzero_u32(), r1 = vx_setzero_u32();
        v_uint32 r2 = vx_setzero_u32(), r3 = vx_setzero_u32();
        ਲਈ (; j <= n - 4 * VTraits<v_int32>::vlanes(); j += 4 * VTraits<v_int32>::vlanes()) {
            v_int32 v01 = vx_ਲੱਦੋ(src1 + j), v02 = vx_ਲੱਦੋ(src2 + j);
            r0 = v_max(r0, v_absdiff(v01, v02));

            v_int32 v11 = vx_ਲੱਦੋ(src1 + j + VTraits<v_int32>::vlanes()),
                    v12 = vx_ਲੱਦੋ(src2 + j + VTraits<v_int32>::vlanes());
            r1 = v_max(r1, v_absdiff(v11, v12));

            v_int32 v21 = vx_ਲੱਦੋ(src1 + j + 2 * VTraits<v_int32>::vlanes()),
                    v22 = vx_ਲੱਦੋ(src2 + j + 2 * VTraits<v_int32>::vlanes());
            r2 = v_max(r2, v_absdiff(v21, v22));

            v_int32 v31 = vx_ਲੱਦੋ(src1 + j + 3 * VTraits<v_int32>::vlanes()),
                    v32 = vx_ਲੱਦੋ(src2 + j + 3 * VTraits<v_int32>::vlanes());
            r3 = v_max(r3, v_absdiff(v31, v32));
        }
        s = (ਪੂਰਨ)v_reduce_max(v_max(v_max(v_max(r0, r1), r2), r3));
        ਲਈ (; j < n; j++) {
            ਬਿਨਾਨਿਸ਼ਾਨੀ v = (ਬਿਨਾਨਿਸ਼ਾਨੀ)cv_absdiff(src1[j], src2[j]);
            s = std::max(s, v);
        }
        ਵਾਪਸ s;
    }
};

ਸਾਂਚਾ<>
ਬਣਤਰ NormDiffInf_SIMD<ਦਸ਼ਮਲਵ, ਦਸ਼ਮਲਵ> {
    ਦਸ਼ਮਲਵ ਸੰਚਾਲਕ() (ਅਚਲ ਦਸ਼ਮਲਵ* src1, ਅਚਲ ਦਸ਼ਮਲਵ* src2, ਪੂਰਨ n) ਅਚਲ {
        ਪੂਰਨ j = 0;
        ਦਸ਼ਮਲਵ s = 0;
        v_float32 r0 = vx_setzero_f32(), r1 = vx_setzero_f32();
        ਲਈ (; j <= n - 2 * VTraits<v_float32>::vlanes(); j += 2 * VTraits<v_float32>::vlanes()) {
            v_float32 v01 = vx_ਲੱਦੋ(src1 + j), v02 = vx_ਲੱਦੋ(src2 + j);
            r0 = v_max(r0, v_absdiff(v01, v02));

            v_float32 v11 = vx_ਲੱਦੋ(src1 + j + VTraits<v_float32>::vlanes()),
                      v12 = vx_ਲੱਦੋ(src2 + j + VTraits<v_float32>::vlanes());
            r1 = v_max(r1, v_absdiff(v11, v12));
        }
        s = v_reduce_max(v_max(r0, r1));
        ਲਈ (; j < n; j++) {
            ਦਸ਼ਮਲਵ v = (ਦਸ਼ਮਲਵ)cv_absdiff(src1[j], src2[j]);
            s = std::max(s, v);
        }
        ਵਾਪਸ s;
    }
};

ਸਾਂਚਾ<>
ਬਣਤਰ NormDiffL1_SIMD<uchar, ਪੂਰਨ> {
    ਪੂਰਨ ਸੰਚਾਲਕ() (ਅਚਲ uchar* src1, ਅਚਲ uchar* src2, ਪੂਰਨ n) ਅਚਲ {
        ਪੂਰਨ j = 0;
        ਪੂਰਨ s = 0;
        v_uint32 r0 = vx_setzero_u32(), r1 = vx_setzero_u32();
        v_uint8  one = vx_setall_u8(1);
        ਲਈ (; j<= n - 2 * VTraits<v_uint8>::vlanes(); j += 2 * VTraits<v_uint8>::vlanes()) {
            v_uint8 v01 = vx_ਲੱਦੋ(src1 + j), v02 = vx_ਲੱਦੋ(src2 + j);
            r0 = v_dotprod_expand_fast(v_absdiff(v01, v02), one, r0);

            v_uint8 v11 = vx_ਲੱਦੋ(src1 + j + VTraits<v_uint8>::vlanes()),
                    v12 = vx_ਲੱਦੋ(src2 + j + VTraits<v_uint8>::vlanes());
            r1 = v_dotprod_expand_fast(v_absdiff(v11, v12), one, r1);
        }
        s += v_reduce_ਜੋੜ(v_ਜੋੜੋ(r0, r1));
        ਲਈ (; j < n; j++) {
            ਪੂਰਨ v = (ਪੂਰਨ)cv_absdiff(src1[j], src2[j]);
            s += v;
        }
        ਵਾਪਸ s;
    }
};

ਸਾਂਚਾ<>
ਬਣਤਰ NormDiffL1_SIMD<schar, ਪੂਰਨ> {
    ਪੂਰਨ ਸੰਚਾਲਕ() (ਅਚਲ schar* src1, ਅਚਲ schar* src2, ਪੂਰਨ n) ਅਚਲ {
        ਪੂਰਨ j = 0;
        ਪੂਰਨ s = 0;
        v_uint32 r0 = vx_setzero_u32(), r1 = vx_setzero_u32();
        v_uint8  one = vx_setall_u8(1);
        ਲਈ (; j<= n - 2 * VTraits<v_int8>::vlanes(); j += 2 * VTraits<v_int8>::vlanes()) {
            v_int8 v01 = vx_ਲੱਦੋ(src1 + j), v02 = vx_ਲੱਦੋ(src2 + j);
            r0 = v_dotprod_expand_fast(v_absdiff(v01, v02), one, r0);

            v_int8 v11 = vx_ਲੱਦੋ(src1 + j + VTraits<v_int8>::vlanes()),
                   v12 = vx_ਲੱਦੋ(src2 + j + VTraits<v_int8>::vlanes());
            r1 = v_dotprod_expand_fast(v_absdiff(v11, v12), one, r1);
        }
        s += v_reduce_ਜੋੜ(v_ਜੋੜੋ(r0, r1));
        ਲਈ (; j < n; j++) {
            ਪੂਰਨ v = (ਪੂਰਨ)cv_absdiff(src1[j], src2[j]);
            s += v;
        }
        ਵਾਪਸ s;
    }
};

ਸਾਂਚਾ<>
ਬਣਤਰ NormDiffL1_SIMD<ushort, ਪੂਰਨ> {
    ਪੂਰਨ ਸੰਚਾਲਕ() (ਅਚਲ ushort* src1, ਅਚਲ ushort* src2, ਪੂਰਨ n) ਅਚਲ {
        ਪੂਰਨ j = 0;
        ਪੂਰਨ s = 0;
        v_uint32 r0 = vx_setzero_u32(), r1 = vx_setzero_u32();
        v_uint32 r2 = vx_setzero_u32(), r3 = vx_setzero_u32();
        ਲਈ (; j<= n - 4 * VTraits<v_uint16>::vlanes(); j += 4 * VTraits<v_uint16>::vlanes()) {
            v_uint16 v01 = vx_ਲੱਦੋ(src1 + j), v02 = vx_ਲੱਦੋ(src2 + j);
            v_uint32 u00, u01;
            v_expand(v_absdiff(v01, v02), u00, u01);
            r0 = v_ਜੋੜੋ(r0, v_ਜੋੜੋ(u00, u01));

            v_uint16 v11 = vx_ਲੱਦੋ(src1 + j + VTraits<v_uint16>::vlanes()),
                     v12 = vx_ਲੱਦੋ(src2 + j + VTraits<v_uint16>::vlanes());
            v_uint32 u10, u11;
            v_expand(v_absdiff(v11, v12), u10, u11);
            r1 = v_ਜੋੜੋ(r1, v_ਜੋੜੋ(u10, u11));

            v_uint16 v21 = vx_ਲੱਦੋ(src1 + j + 2 * VTraits<v_uint16>::vlanes()),
                     v22 = vx_ਲੱਦੋ(src2 + j + 2 * VTraits<v_uint16>::vlanes());
            v_uint32 u20, u21;
            v_expand(v_absdiff(v21, v22), u20, u21);
            r2 = v_ਜੋੜੋ(r2, v_ਜੋੜੋ(u20, u21));

            v_uint16 v31 = vx_ਲੱਦੋ(src1 + j + 3 * VTraits<v_uint16>::vlanes()),
                     v32 = vx_ਲੱਦੋ(src2 + j + 3 * VTraits<v_uint16>::vlanes());
            v_uint32 u30, u31;
            v_expand(v_absdiff(v31, v32), u30, u31);
            r3 = v_ਜੋੜੋ(r3, v_ਜੋੜੋ(u30, u31));
        }
        s += (ਪੂਰਨ)v_reduce_ਜੋੜ(v_ਜੋੜੋ(v_ਜੋੜੋ(v_ਜੋੜੋ(r0, r1), r2), r3));
        ਲਈ (; j < n; j++) {
            ਪੂਰਨ v = (ਪੂਰਨ)cv_absdiff(src1[j], src2[j]);
            s += v;
        }
        ਵਾਪਸ s;
    }
};

ਸਾਂਚਾ<>
ਬਣਤਰ NormDiffL1_SIMD<ਛੋਟਾ, ਪੂਰਨ> {
    ਪੂਰਨ ਸੰਚਾਲਕ() (ਅਚਲ ਛੋਟਾ* src1, ਅਚਲ ਛੋਟਾ* src2, ਪੂਰਨ n) ਅਚਲ {
        ਪੂਰਨ j = 0;
        ਪੂਰਨ s = 0;
        v_uint32 r0 = vx_setzero_u32(), r1 = vx_setzero_u32();
        v_uint32 r2 = vx_setzero_u32(), r3 = vx_setzero_u32();
        ਲਈ (; j<= n - 4 * VTraits<v_int16>::vlanes(); j += 4 * VTraits<v_int16>::vlanes()) {
            v_int16 v01 = vx_ਲੱਦੋ(src1 + j), v02 = vx_ਲੱਦੋ(src2 + j);
            v_uint32 u00, u01;
            v_expand(v_absdiff(v01, v02), u00, u01);
            r0 = v_ਜੋੜੋ(r0, v_ਜੋੜੋ(u00, u01));

            v_int16 v11 = vx_ਲੱਦੋ(src1 + j + VTraits<v_int16>::vlanes()),
                    v12 = vx_ਲੱਦੋ(src2 + j + VTraits<v_int16>::vlanes());
            v_uint32 u10, u11;
            v_expand(v_absdiff(v11, v12), u10, u11);
            r1 = v_ਜੋੜੋ(r1, v_ਜੋੜੋ(u10, u11));

            v_int16 v21 = vx_ਲੱਦੋ(src1 + j + 2 * VTraits<v_int16>::vlanes()),
                    v22 = vx_ਲੱਦੋ(src2 + j + 2 * VTraits<v_int16>::vlanes());
            v_uint32 u20, u21;
            v_expand(v_absdiff(v21, v22), u20, u21);
            r2 = v_ਜੋੜੋ(r2, v_ਜੋੜੋ(u20, u21));

            v_int16 v31 = vx_ਲੱਦੋ(src1 + j + 3 * VTraits<v_int16>::vlanes()),
                    v32 = vx_ਲੱਦੋ(src2 + j + 3 * VTraits<v_int16>::vlanes());
            v_uint32 u30, u31;
            v_expand(v_absdiff(v31, v32), u30, u31);
            r3 = v_ਜੋੜੋ(r3, v_ਜੋੜੋ(u30, u31));
        }
        s += (ਪੂਰਨ)v_reduce_ਜੋੜ(v_ਜੋੜੋ(v_ਜੋੜੋ(v_ਜੋੜੋ(r0, r1), r2), r3));
        ਲਈ (; j < n; j++) {
            ਪੂਰਨ v = (ਪੂਰਨ)cv_absdiff(src1[j], src2[j]);
            s += v;
        }
        ਵਾਪਸ s;
    }
};

ਸਾਂਚਾ<>
ਬਣਤਰ NormDiffL2_SIMD<uchar, ਪੂਰਨ> {
    ਪੂਰਨ ਸੰਚਾਲਕ() (ਅਚਲ uchar* src1, ਅਚਲ uchar* src2, ਪੂਰਨ n) ਅਚਲ {
        ਪੂਰਨ j = 0;
        ਪੂਰਨ s = 0;
        v_uint32 r0 = vx_setzero_u32(), r1 = vx_setzero_u32();
        ਲਈ (; j <= n - 2 * VTraits<v_uint8>::vlanes(); j += 2 * VTraits<v_uint8>::vlanes()) {
            v_uint8 v01 = vx_ਲੱਦੋ(src1 + j), v02 = vx_ਲੱਦੋ(src2 + j);
            v_uint8 v0 = v_absdiff(v01, v02);
            r0 = v_dotprod_expand_fast(v0, v0, r0);

            v_uint8 v11 = vx_ਲੱਦੋ(src1 + j + VTraits<v_uint8>::vlanes()),
                    v12 = vx_ਲੱਦੋ(src2 + j + VTraits<v_uint8>::vlanes());
            v_uint8 v1 = v_absdiff(v11, v12);
            r1 = v_dotprod_expand_fast(v1, v1, r1);
        }
        s += v_reduce_ਜੋੜ(v_ਜੋੜੋ(r0, r1));
        ਲਈ (; j < n; j++) {
            ਪੂਰਨ v = (ਪੂਰਨ)src1[j] - (ਪੂਰਨ)src2[j];
            s += v * v;
        }
        ਵਾਪਸ s;
    }
};

ਸਾਂਚਾ<>
ਬਣਤਰ NormDiffL2_SIMD<schar, ਪੂਰਨ> {
    ਪੂਰਨ ਸੰਚਾਲਕ() (ਅਚਲ schar* src1, ਅਚਲ schar* src2, ਪੂਰਨ n) ਅਚਲ {
        ਪੂਰਨ j = 0;
        ਪੂਰਨ s = 0;
        v_uint32 r0 = vx_setzero_u32(), r1 = vx_setzero_u32();
        ਲਈ (; j <= n - 2 * VTraits<v_int8>::vlanes(); j += 2 * VTraits<v_int8>::vlanes()) {
            v_int8 v01 = vx_ਲੱਦੋ(src1 + j), v02 = vx_ਲੱਦੋ(src2 + j);
            v_uint8 v0 = v_absdiff(v01, v02);
            r0 = v_dotprod_expand_fast(v0, v0, r0);

            v_int8 v11 = vx_ਲੱਦੋ(src1 + j + VTraits<v_int8>::vlanes()),
                   v12 = vx_ਲੱਦੋ(src2 + j + VTraits<v_int8>::vlanes());
            v_uint8 v1 = v_absdiff(v11, v12);
            r1 = v_dotprod_expand_fast(v1, v1, r1);
        }
        s += v_reduce_ਜੋੜ(v_ਜੋੜੋ(r0, r1));
        ਲਈ (; j < n; j++) {
            ਪੂਰਨ v = (ਪੂਰਨ)src1[j] - (ਪੂਰਨ)src2[j];
            s += v * v;
        }
        ਵਾਪਸ s;
    }
};

#ਜੇ_ਅੰਤ

#ਜੇ (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)

ਸਾਂਚਾ<>
ਬਣਤਰ NormInf_SIMD<ਦੁੱਗਣਾ, ਦੁੱਗਣਾ> {
    ਦੁੱਗਣਾ ਸੰਚਾਲਕ() (ਅਚਲ ਦੁੱਗਣਾ* src, ਪੂਰਨ n) ਅਚਲ {
        ਪੂਰਨ j = 0;
        ਦੁੱਗਣਾ s = 0.f;
        v_float64 r0 = vx_setzero_f64(), r1 = vx_setzero_f64();
        v_float64 r2 = vx_setzero_f64(), r3 = vx_setzero_f64();
        ਲਈ (; j <= n - 4 * VTraits<v_float64>::vlanes(); j += 4 * VTraits<v_float64>::vlanes()) {
            r0 = v_max(r0, v_abs(vx_ਲੱਦੋ(src + j                                   )));
            r1 = v_max(r1, v_abs(vx_ਲੱਦੋ(src + j +     VTraits<v_float64>::vlanes())));
            r2 = v_max(r2, v_abs(vx_ਲੱਦੋ(src + j + 2 * VTraits<v_float64>::vlanes())));
            r3 = v_max(r3, v_abs(vx_ਲੱਦੋ(src + j + 3 * VTraits<v_float64>::vlanes())));
        }
        r0 = v_max(r0, v_max(r1, v_max(r2, r3)));
        ਲਈ (; j < n; j++) {
            s = std::max(s, cv_abs(src[j]));
        }
        // [TODO]: use v_reduce_max when it supports float64
        ਦੁੱਗਣਾ t[VTraits<v_float64>::max_nlanes];
        vx_store(t, r0);
        ਲਈ (ਪੂਰਨ i = 0; i < VTraits<v_float64>::vlanes(); i++) {
            s = std::max(s, cv_abs(t[i]));
        }
        ਵਾਪਸ s;
    }
};

ਸਾਂਚਾ<>
ਬਣਤਰ NormL1_SIMD<ਪੂਰਨ, ਦੁੱਗਣਾ> {
    ਦੁੱਗਣਾ ਸੰਚਾਲਕ() (ਅਚਲ ਪੂਰਨ* src, ਪੂਰਨ n) ਅਚਲ {
        ਪੂਰਨ j = 0;
        ਦੁੱਗਣਾ s = 0.f;
        v_float64 r00 = vx_setzero_f64(), r01 = vx_setzero_f64();
        v_float64 r10 = vx_setzero_f64(), r11 = vx_setzero_f64();
        ਲਈ (; j <= n - 2 * VTraits<v_int32>::vlanes(); j += 2 * VTraits<v_int32>::vlanes()) {
            // Widen to double before taking the magnitude: exact over the whole int32 range,
            // INT_MIN included, which a float intermediate would not be above 2^24.
            v_int32 i0 = vx_ਲੱਦੋ(src + j), i1 = vx_ਲੱਦੋ(src + j + VTraits<v_int32>::vlanes());
            r00 = v_ਜੋੜੋ(r00, v_abs(v_cvt_f64(i0))); r01 = v_ਜੋੜੋ(r01, v_abs(v_cvt_f64_ਉੱਚਾ(i0)));
            r10 = v_ਜੋੜੋ(r10, v_abs(v_cvt_f64(i1))); r11 = v_ਜੋੜੋ(r11, v_abs(v_cvt_f64_ਉੱਚਾ(i1)));
        }
        s += v_reduce_ਜੋੜ(v_ਜੋੜੋ(v_ਜੋੜੋ(v_ਜੋੜੋ(r00, r01), r10), r11));
        ਲਈ (; j < n; j++) {
            s += cv_abs(src[j]);
        }
        ਵਾਪਸ s;
    }
};

ਸਾਂਚਾ<>
ਬਣਤਰ NormL1_SIMD<ਦਸ਼ਮਲਵ, ਦੁੱਗਣਾ> {
    ਦੁੱਗਣਾ ਸੰਚਾਲਕ() (ਅਚਲ ਦਸ਼ਮਲਵ* src, ਪੂਰਨ n) ਅਚਲ {
        ਪੂਰਨ j = 0;
        ਦੁੱਗਣਾ s = 0.f;
        v_float64 r00 = vx_setzero_f64(), r01 = vx_setzero_f64();
        v_float64 r10 = vx_setzero_f64(), r11 = vx_setzero_f64();
        v_float64 r20 = vx_setzero_f64(), r21 = vx_setzero_f64();
        v_float64 r30 = vx_setzero_f64(), r31 = vx_setzero_f64();
        ਲਈ (; j <= n - 4 * VTraits<v_float32>::vlanes(); j += 4 * VTraits<v_float32>::vlanes()) {
            v_float32 v0 = v_abs(vx_ਲੱਦੋ(src + j)), v1 = v_abs(vx_ਲੱਦੋ(src + j + VTraits<v_float32>::vlanes()));
            r00 = v_ਜੋੜੋ(r00, v_cvt_f64(v0)); r01 = v_ਜੋੜੋ(r01, v_cvt_f64_ਉੱਚਾ(v0));
            r10 = v_ਜੋੜੋ(r10, v_cvt_f64(v1)); r11 = v_ਜੋੜੋ(r11, v_cvt_f64_ਉੱਚਾ(v1));

            v_float32 v2 = v_abs(vx_ਲੱਦੋ(src + j + 2 * VTraits<v_float32>::vlanes())), v3 = v_abs(vx_ਲੱਦੋ(src + j + 3 * VTraits<v_float32>::vlanes()));
            r20 = v_ਜੋੜੋ(r20, v_cvt_f64(v2)); r21 = v_ਜੋੜੋ(r21, v_cvt_f64_ਉੱਚਾ(v2));
            r30 = v_ਜੋੜੋ(r30, v_cvt_f64(v3)); r31 = v_ਜੋੜੋ(r31, v_cvt_f64_ਉੱਚਾ(v3));
        }
        s += v_reduce_ਜੋੜ(v_ਜੋੜੋ(v_ਜੋੜੋ(v_ਜੋੜੋ(r00, r01), r10), r11));
        s += v_reduce_ਜੋੜ(v_ਜੋੜੋ(v_ਜੋੜੋ(v_ਜੋੜੋ(r20, r21), r30), r31));
        ਲਈ (; j < n; j++) {
            s += cv_abs(src[j]);
        }
        ਵਾਪਸ s;
    }
};

ਸਾਂਚਾ<>
ਬਣਤਰ NormL2_SIMD<ushort, ਦੁੱਗਣਾ> {
    ਦੁੱਗਣਾ ਸੰਚਾਲਕ() (ਅਚਲ ushort* src, ਪੂਰਨ n) ਅਚਲ {
        ਪੂਰਨ j = 0;
        ਦੁੱਗਣਾ s = 0.f;
        v_float64 r0 = vx_setzero_f64(), r1 = vx_setzero_f64();
        ਲਈ (; j <= n - 2 * VTraits<v_uint16>::vlanes(); j += 2 * VTraits<v_uint16>::vlanes()) {
            v_uint16 v0 = vx_ਲੱਦੋ(src + j);
            v_uint64 u0 = v_dotprod_expand_fast(v0, v0);
            r0 = v_ਜੋੜੋ(r0, v_cvt_f64(v_reinterpret_as_s64(u0)));

            v_uint16 v1 = vx_ਲੱਦੋ(src + j + VTraits<v_uint16>::vlanes());
            v_uint64 u1 = v_dotprod_expand_fast(v1, v1);
            r1 = v_ਜੋੜੋ(r1, v_cvt_f64(v_reinterpret_as_s64(u1)));
        }
        s += v_reduce_ਜੋੜ(v_ਜੋੜੋ(r0, r1));
        ਲਈ (; j < n; j++) {
            ਦੁੱਗਣਾ v = saturate_cast<ਦੁੱਗਣਾ>(src[j]);
            s += v * v;
        }
        ਵਾਪਸ s;
    }
};

ਸਾਂਚਾ<>
ਬਣਤਰ NormL2_SIMD<ਛੋਟਾ, ਦੁੱਗਣਾ> {
    ਦੁੱਗਣਾ ਸੰਚਾਲਕ() (ਅਚਲ ਛੋਟਾ* src, ਪੂਰਨ n) ਅਚਲ {
        ਪੂਰਨ j = 0;
        ਦੁੱਗਣਾ s = 0.f;
        v_float64 r0 = vx_setzero_f64(), r1 = vx_setzero_f64();
        ਲਈ (; j <= n - 2 * VTraits<v_int16>::vlanes(); j += 2 * VTraits<v_int16>::vlanes()) {
            v_int16 v0 = vx_ਲੱਦੋ(src + j);
            r0 = v_ਜੋੜੋ(r0, v_cvt_f64(v_dotprod_expand_fast(v0, v0)));

            v_int16 v1 = vx_ਲੱਦੋ(src + j + VTraits<v_int16>::vlanes());
            r1 = v_ਜੋੜੋ(r1, v_cvt_f64(v_dotprod_expand_fast(v1, v1)));
        }
        s += v_reduce_ਜੋੜ(v_ਜੋੜੋ(r0, r1));
        ਲਈ (; j < n; j++) {
            ਦੁੱਗਣਾ v = saturate_cast<ਦੁੱਗਣਾ>(src[j]);
            s += v * v;
        }
        ਵਾਪਸ s;
    }
};

ਸਾਂਚਾ<>
ਬਣਤਰ NormL2_SIMD<ਪੂਰਨ, ਦੁੱਗਣਾ> {
    ਦੁੱਗਣਾ ਸੰਚਾਲਕ() (ਅਚਲ ਪੂਰਨ* src, ਪੂਰਨ n) ਅਚਲ {
        ਪੂਰਨ j = 0;
        ਦੁੱਗਣਾ s = 0.f;
        v_float64 r0 = vx_setzero_f64(), r1 = vx_setzero_f64();
        ਲਈ (; j <= n - 2 * VTraits<v_int32>::vlanes(); j += 2 * VTraits<v_int32>::vlanes()) {
            v_int32 v0 = vx_ਲੱਦੋ(src + j);
            r0 = v_dotprod_expand_fast(v0, v0, r0);

            v_int32 v1 = vx_ਲੱਦੋ(src + j + VTraits<v_int32>::vlanes());
            r1 = v_dotprod_expand_fast(v1, v1, r1);
        }
        s += v_reduce_ਜੋੜ(v_ਜੋੜੋ(r0, r1));
        ਲਈ (; j < n; j++) {
            ਦੁੱਗਣਾ v = src[j];
            s += v * v;
        }
        ਵਾਪਸ s;
    }
};

ਸਾਂਚਾ<>
ਬਣਤਰ NormL2_SIMD<ਦਸ਼ਮਲਵ, ਦੁੱਗਣਾ> {
    ਦੁੱਗਣਾ ਸੰਚਾਲਕ() (ਅਚਲ ਦਸ਼ਮਲਵ* src, ਪੂਰਨ n) ਅਚਲ {
        ਪੂਰਨ j = 0;
        ਦੁੱਗਣਾ s = 0.f;
        v_float64 r00 = vx_setzero_f64(), r01 = vx_setzero_f64();
        v_float64 r10 = vx_setzero_f64(), r11 = vx_setzero_f64();
        ਲਈ (; j <= n - 2 * VTraits<v_float32>::vlanes(); j += 2 * VTraits<v_float32>::vlanes()) {
            v_float32 v0 = vx_ਲੱਦੋ(src + j), v1 = vx_ਲੱਦੋ(src + j + VTraits<v_float32>::vlanes());
            v_float64 v00 = v_cvt_f64(v0), v01 = v_cvt_f64_ਉੱਚਾ(v0);
            v_float64 v10 = v_cvt_f64(v1), v11 = v_cvt_f64_ਉੱਚਾ(v1);
            r00 = v_fma(v00, v00, r00); r01 = v_fma(v01, v01, r01);
            r10 = v_fma(v10, v10, r10); r11 = v_fma(v11, v11, r11);
        }
        s += v_reduce_ਜੋੜ(v_ਜੋੜੋ(v_ਜੋੜੋ(v_ਜੋੜੋ(r00, r01), r10), r11));
        ਲਈ (; j < n; j++) {
            ਦੁੱਗਣਾ v = src[j];
            s += v * v;
        }
        ਵਾਪਸ s;
    }
};

#ਜੇ_ਅੰਤ

#ਜੇ CV_SIMD_64F // CV_SIMD_SCALABLE_64F has accuracy problem with the following kernels on ci

ਸਾਂਚਾ<>
ਬਣਤਰ NormL1_SIMD<ਦੁੱਗਣਾ, ਦੁੱਗਣਾ> {
    ਦੁੱਗਣਾ ਸੰਚਾਲਕ() (ਅਚਲ ਦੁੱਗਣਾ* src, ਪੂਰਨ n) ਅਚਲ {
        ਪੂਰਨ j = 0;
        ਦੁੱਗਣਾ s = 0.f;
        v_float64 r00 = vx_setzero_f64(), r01 = vx_setzero_f64();
        v_float64 r10 = vx_setzero_f64(), r11 = vx_setzero_f64();
        ਲਈ (; j <= n - 4 * VTraits<v_float64>::vlanes(); j += 4 * VTraits<v_float64>::vlanes()) {
            r00 = v_ਜੋੜੋ(r00, v_abs(vx_ਲੱਦੋ(src + j                                   )));
            r01 = v_ਜੋੜੋ(r01, v_abs(vx_ਲੱਦੋ(src + j +     VTraits<v_float64>::vlanes())));
            r10 = v_ਜੋੜੋ(r10, v_abs(vx_ਲੱਦੋ(src + j + 2 * VTraits<v_float64>::vlanes())));
            r11 = v_ਜੋੜੋ(r11, v_abs(vx_ਲੱਦੋ(src + j + 3 * VTraits<v_float64>::vlanes())));
        }
        s += v_reduce_ਜੋੜ(v_ਜੋੜੋ(v_ਜੋੜੋ(v_ਜੋੜੋ(r00, r01), r10), r11));
        ਲਈ (; j < n; j++) {
            s += cv_abs(src[j]);
        }
        ਵਾਪਸ s;
    }
};

ਸਾਂਚਾ<>
ਬਣਤਰ NormL2_SIMD<ਦੁੱਗਣਾ, ਦੁੱਗਣਾ> {
    ਦੁੱਗਣਾ ਸੰਚਾਲਕ() (ਅਚਲ ਦੁੱਗਣਾ* src, ਪੂਰਨ n) ਅਚਲ {
        ਪੂਰਨ j = 0;
        ਦੁੱਗਣਾ s = 0.f;
        v_float64 r00 = vx_setzero_f64(), r01 = vx_setzero_f64();
        v_float64 r10 = vx_setzero_f64(), r11 = vx_setzero_f64();
        ਲਈ (; j <= n - 4 * VTraits<v_float64>::vlanes(); j += 4 * VTraits<v_float64>::vlanes()) {
            v_float64 v00 = vx_ਲੱਦੋ(src + j                                   );
            v_float64 v01 = vx_ਲੱਦੋ(src + j +     VTraits<v_float64>::vlanes());
            v_float64 v10 = vx_ਲੱਦੋ(src + j + 2 * VTraits<v_float64>::vlanes());
            v_float64 v11 = vx_ਲੱਦੋ(src + j + 3 * VTraits<v_float64>::vlanes());
            r00 = v_fma(v00, v00, r00); r01 = v_fma(v01, v01, r01);
            r10 = v_fma(v10, v10, r10); r11 = v_fma(v11, v11, r11);
        }
        s += v_reduce_ਜੋੜ(v_ਜੋੜੋ(v_ਜੋੜੋ(v_ਜੋੜੋ(r00, r01), r10), r11));
        ਲਈ (; j < n; j++) {
            ਦੁੱਗਣਾ v = src[j];
            s += v * v;
        }
        ਵਾਪਸ s;
    }
};

ਸਾਂਚਾ<>
ਬਣਤਰ NormDiffInf_SIMD<ਦੁੱਗਣਾ, ਦੁੱਗਣਾ> {
    ਦੁੱਗਣਾ ਸੰਚਾਲਕ() (ਅਚਲ ਦੁੱਗਣਾ* src1, ਅਚਲ ਦੁੱਗਣਾ* src2, ਪੂਰਨ n) ਅਚਲ {
        ਪੂਰਨ j = 0;
        ਦੁੱਗਣਾ s = 0;
        v_float64 r0 = vx_setzero_f64(), r1 = vx_setzero_f64();
        ਲਈ (; j <= n - 2 * VTraits<v_float64>::vlanes(); j += 2 * VTraits<v_float64>::vlanes()) {
            v_float64 v01 = vx_ਲੱਦੋ(src1 + j), v02 = vx_ਲੱਦੋ(src2 + j);
            r0 = v_max(r0, v_absdiff(v01, v02));

            v_float64 v11 = vx_ਲੱਦੋ(src1 + j + VTraits<v_float64>::vlanes()),
                      v12 = vx_ਲੱਦੋ(src2 + j + VTraits<v_float64>::vlanes());
            r1 = v_max(r1, v_absdiff(v11, v12));
        }
        // [TODO]: use v_reduce_max when it supports float64
        ਦੁੱਗਣਾ t[VTraits<v_float64>::max_nlanes];
        vx_store(t, v_max(r0, r1));
        ਲਈ (ਪੂਰਨ i = 0; i < VTraits<v_float64>::vlanes(); i++) {
            s = std::max(s, t[i]);
        }
        ਲਈ (; j < n; j++) {
            ਦੁੱਗਣਾ v = (ਦੁੱਗਣਾ)cv_absdiff(src1[j], src2[j]);
            s = std::max(s, v);
        }
        ਵਾਪਸ s;
    }
};

ਸਾਂਚਾ<>
ਬਣਤਰ NormDiffL1_SIMD<ਦਸ਼ਮਲਵ, ਦੁੱਗਣਾ> {
    ਦੁੱਗਣਾ ਸੰਚਾਲਕ() (ਅਚਲ ਦਸ਼ਮਲਵ* src1, ਅਚਲ ਦਸ਼ਮਲਵ* src2, ਪੂਰਨ n) ਅਚਲ {
        ਪੂਰਨ j = 0;
        ਦੁੱਗਣਾ s = 0.f;
        v_float64 r0 = vx_setzero_f64(), r1 = vx_setzero_f64();
        v_float64 r2 = vx_setzero_f64(), r3 = vx_setzero_f64();
        ਲਈ (; j <= n - 2 * VTraits<v_float32>::vlanes(); j += 2 * VTraits<v_float32>::vlanes()) {
            v_float32 v01 = vx_ਲੱਦੋ(src1 + j), v02 = vx_ਲੱਦੋ(src2 + j);
            v_float32 v0 = v_absdiff(v01, v02);
            r0 = v_ਜੋੜੋ(r0, v_cvt_f64(v0)); r1 = v_ਜੋੜੋ(r1, v_cvt_f64_ਉੱਚਾ(v0));

            v_float32 v11 = vx_ਲੱਦੋ(src1 + j + VTraits<v_float32>::vlanes()),
                      v12 = vx_ਲੱਦੋ(src2 + j + VTraits<v_float32>::vlanes());
            v_float32 v1 = v_absdiff(v11, v12);
            r2 = v_ਜੋੜੋ(r2, v_cvt_f64(v1)); r3 = v_ਜੋੜੋ(r3, v_cvt_f64_ਉੱਚਾ(v1));
        }
        s += v_reduce_ਜੋੜ(v_ਜੋੜੋ(v_ਜੋੜੋ(v_ਜੋੜੋ(r0, r1), r2), r3));
        ਲਈ (; j < n; j++) {
            ਦੁੱਗਣਾ v = (ਦੁੱਗਣਾ)cv_absdiff(src1[j], src2[j]);
            s += v;
        }
        ਵਾਪਸ s;
    }
};

ਸਾਂਚਾ<>
ਬਣਤਰ NormDiffL1_SIMD<ਦੁੱਗਣਾ, ਦੁੱਗਣਾ> {
    ਦੁੱਗਣਾ ਸੰਚਾਲਕ() (ਅਚਲ ਦੁੱਗਣਾ* src1, ਅਚਲ ਦੁੱਗਣਾ* src2, ਪੂਰਨ n) ਅਚਲ {
        ਪੂਰਨ j = 0;
        ਦੁੱਗਣਾ s = 0.f;
        v_float64 r0 = vx_setzero_f64(), r1 = vx_setzero_f64();
        ਲਈ (; j <= n - 2 * VTraits<v_float64>::vlanes(); j += 2 * VTraits<v_float64>::vlanes()) {
            v_float64 v01 = vx_ਲੱਦੋ(src1 + j), v02 = vx_ਲੱਦੋ(src2 + j);
            r0 = v_ਜੋੜੋ(r0, v_absdiff(v01, v02));

            v_float64 v11 = vx_ਲੱਦੋ(src1 + j + VTraits<v_float64>::vlanes()),
                      v12 = vx_ਲੱਦੋ(src2 + j + VTraits<v_float64>::vlanes());
            r1 = v_ਜੋੜੋ(r1, v_absdiff(v11, v12));
        }
        s += v_reduce_ਜੋੜ(v_ਜੋੜੋ(r0, r1));
        ਲਈ (; j < n; j++) {
            ਦੁੱਗਣਾ v = (ਦੁੱਗਣਾ)cv_absdiff(src1[j], src2[j]);
            s += v;
        }
        ਵਾਪਸ s;
    }
};

ਸਾਂਚਾ<>
ਬਣਤਰ NormDiffL2_SIMD<ushort, ਦੁੱਗਣਾ> {
    ਦੁੱਗਣਾ ਸੰਚਾਲਕ() (ਅਚਲ ushort* src1, ਅਚਲ ushort* src2, ਪੂਰਨ n) ਅਚਲ {
        ਪੂਰਨ j = 0;
        ਦੁੱਗਣਾ s = 0.f;
        v_float64 r0 = vx_setzero_f64(), r1 = vx_setzero_f64();
        ਲਈ (; j <= n - 2 * VTraits<v_uint16>::vlanes(); j += 2 * VTraits<v_uint16>::vlanes()) {
            v_uint16 v01 = vx_ਲੱਦੋ(src1 + j), v02 = vx_ਲੱਦੋ(src2 + j);
            v_uint16 v0 = v_absdiff(v01, v02);
            v_uint64 u0 = v_dotprod_expand_fast(v0, v0);
            r0 = v_ਜੋੜੋ(r0, v_cvt_f64(v_reinterpret_as_s64(u0)));

            v_uint16 v11 = vx_ਲੱਦੋ(src1 + j + VTraits<v_uint16>::vlanes()),
                     v12 = vx_ਲੱਦੋ(src2 + j + VTraits<v_uint16>::vlanes());
            v_uint16 v1 = v_absdiff(v11, v12);
            v_uint64 u1 = v_dotprod_expand_fast(v1, v1);
            r1 = v_ਜੋੜੋ(r1, v_cvt_f64(v_reinterpret_as_s64(u1)));
        }
        s += v_reduce_ਜੋੜ(v_ਜੋੜੋ(r0, r1));
        ਲਈ (; j < n; j++) {
            ਦੁੱਗਣਾ v = (ਦੁੱਗਣਾ)src1[j] - (ਦੁੱਗਣਾ)src2[j];
            s += v * v;
        }
        ਵਾਪਸ s;
    }
};

ਸਾਂਚਾ<>
ਬਣਤਰ NormDiffL2_SIMD<ਛੋਟਾ, ਦੁੱਗਣਾ> {
    ਦੁੱਗਣਾ ਸੰਚਾਲਕ() (ਅਚਲ ਛੋਟਾ* src1, ਅਚਲ ਛੋਟਾ* src2, ਪੂਰਨ n) ਅਚਲ {
        ਪੂਰਨ j = 0;
        ਦੁੱਗਣਾ s = 0.f;
        v_float64 r0 = vx_setzero_f64(), r1 = vx_setzero_f64();
        ਲਈ (; j <= n - 2 * VTraits<v_int16>::vlanes(); j += 2 * VTraits<v_int16>::vlanes()) {
            v_int16 v01 = vx_ਲੱਦੋ(src1 + j), v02 = vx_ਲੱਦੋ(src2 + j);
            v_uint16 v0 = v_absdiff(v01, v02);
            v_uint64 u0 = v_dotprod_expand_fast(v0, v0);
            r0 = v_ਜੋੜੋ(r0, v_cvt_f64(v_reinterpret_as_s64(u0)));

            v_int16 v11 = vx_ਲੱਦੋ(src1 + j + VTraits<v_uint16>::vlanes()),
                    v12 = vx_ਲੱਦੋ(src2 + j + VTraits<v_uint16>::vlanes());
            v_uint16 v1 = v_absdiff(v11, v12);
            v_uint64 u1 = v_dotprod_expand_fast(v1, v1);
            r1 = v_ਜੋੜੋ(r1, v_cvt_f64(v_reinterpret_as_s64(u1)));
        }
        s += v_reduce_ਜੋੜ(v_ਜੋੜੋ(r0, r1));
        ਲਈ (; j < n; j++) {
            ਦੁੱਗਣਾ v = (ਦੁੱਗਣਾ)src1[j] - (ਦੁੱਗਣਾ)src2[j];
            s += v * v;
        }
        ਵਾਪਸ s;
    }
};

ਸਾਂਚਾ<>
ਬਣਤਰ NormDiffL2_SIMD<ਪੂਰਨ, ਦੁੱਗਣਾ> {
    ਦੁੱਗਣਾ ਸੰਚਾਲਕ() (ਅਚਲ ਪੂਰਨ* src1, ਅਚਲ ਪੂਰਨ* src2, ਪੂਰਨ n) ਅਚਲ {
        ਪੂਰਨ j = 0;
        ਦੁੱਗਣਾ s = 0.f;
        v_float64 r0 = vx_setzero_f64(), r1 = vx_setzero_f64();
        ਲਈ (; j <= n - VTraits<v_int32>::vlanes(); j += VTraits<v_int32>::vlanes()) {
            v_int32 v01 = vx_ਲੱਦੋ(src1 + j), v02 = vx_ਲੱਦੋ(src2 + j);
            v_uint32 v0 = v_absdiff(v01, v02);
            v_uint64 ev0, ev1;
            v_expand(v0, ev0, ev1);
            v_float64 f0 = v_cvt_f64(v_reinterpret_as_s64(ev0)), f1 = v_cvt_f64(v_reinterpret_as_s64(ev1));
            r0 = v_fma(f0, f0, r0); r1 = v_fma(f1, f1, r1);
        }
        s += v_reduce_ਜੋੜ(v_ਜੋੜੋ(r0, r1));
        ਲਈ (; j < n; j++) {
            ਦੁੱਗਣਾ v = (ਦੁੱਗਣਾ)src1[j] - (ਦੁੱਗਣਾ)src2[j];
            s += v * v;
        }
        ਵਾਪਸ s;
    }
};

ਸਾਂਚਾ<>
ਬਣਤਰ NormDiffL2_SIMD<ਦਸ਼ਮਲਵ, ਦੁੱਗਣਾ> {
    ਦੁੱਗਣਾ ਸੰਚਾਲਕ() (ਅਚਲ ਦਸ਼ਮਲਵ* src1, ਅਚਲ ਦਸ਼ਮਲਵ* src2, ਪੂਰਨ n) ਅਚਲ {
        ਪੂਰਨ j = 0;
        ਦੁੱਗਣਾ s = 0.f;
        v_float64 r0 = vx_setzero_f64(), r1 = vx_setzero_f64();
        v_float64 r2 = vx_setzero_f64(), r3 = vx_setzero_f64();
        ਲਈ (; j <= n - 2 * VTraits<v_float32>::vlanes(); j += 2 * VTraits<v_float32>::vlanes()) {
            v_float32 v01 = vx_ਲੱਦੋ(src1 + j), v02 = vx_ਲੱਦੋ(src2 + j);
            v_float32 v0 = v_absdiff(v01, v02);
            v_float64 f01 = v_cvt_f64(v0), f02 = v_cvt_f64_ਉੱਚਾ(v0);
            r0 = v_fma(f01, f01, r0); r1 = v_fma(f02, f02, r1);

            v_float32 v11 = vx_ਲੱਦੋ(src1 + j + VTraits<v_float32>::vlanes()),
                      v12 = vx_ਲੱਦੋ(src2 + j + VTraits<v_float32>::vlanes());
            v_float32 v1 = v_absdiff(v11, v12);
            v_float64 f11 = v_cvt_f64(v1), f12 = v_cvt_f64_ਉੱਚਾ(v1);
            r2 = v_fma(f11, f11, r2); r3 = v_fma(f12, f12, r3);
        }
        s += v_reduce_ਜੋੜ(v_ਜੋੜੋ(v_ਜੋੜੋ(v_ਜੋੜੋ(r0, r1), r2), r3));
        ਲਈ (; j < n; j++) {
            ਦੁੱਗਣਾ v = (ਦੁੱਗਣਾ)src1[j] - (ਦੁੱਗਣਾ)src2[j];
            s += v * v;
        }
        ਵਾਪਸ s;
    }
};

ਸਾਂਚਾ<>
ਬਣਤਰ NormDiffL2_SIMD<ਦੁੱਗਣਾ, ਦੁੱਗਣਾ> {
    ਦੁੱਗਣਾ ਸੰਚਾਲਕ() (ਅਚਲ ਦੁੱਗਣਾ* src1, ਅਚਲ ਦੁੱਗਣਾ* src2, ਪੂਰਨ n) ਅਚਲ {
        ਪੂਰਨ j = 0;
        ਦੁੱਗਣਾ s = 0.f;
        v_float64 r0 = vx_setzero_f64(), r1 = vx_setzero_f64();
        v_float64 r2 = vx_setzero_f64(), r3 = vx_setzero_f64();
        ਲਈ (; j <= n - 4 * VTraits<v_float64>::vlanes(); j += 4 * VTraits<v_float64>::vlanes()) {
            v_float64 v01 = vx_ਲੱਦੋ(src1 + j), v02 = vx_ਲੱਦੋ(src2 + j);
            v_float64 v0 = v_absdiff(v01, v02);
            r0 = v_fma(v0, v0, r0);

            v_float64 v11 = vx_ਲੱਦੋ(src1 + j + VTraits<v_float64>::vlanes()),
                      v12 = vx_ਲੱਦੋ(src2 + j + VTraits<v_float64>::vlanes());
            v_float64 v1 = v_absdiff(v11, v12);
            r1 = v_fma(v1, v1, r1);

            v_float64 v21 = vx_ਲੱਦੋ(src1 + j + 2 * VTraits<v_float64>::vlanes()),
                      v22 = vx_ਲੱਦੋ(src2 + j + 2 * VTraits<v_float64>::vlanes());
            v_float64 v2 = v_absdiff(v21, v22);
            r2 = v_fma(v2, v2, r2);

            v_float64 v31 = vx_ਲੱਦੋ(src1 + j + 3 * VTraits<v_float64>::vlanes()),
                      v32 = vx_ਲੱਦੋ(src2 + j + 3 * VTraits<v_float64>::vlanes());
            v_float64 v3 = v_absdiff(v31, v32);
            r3 = v_fma(v3, v3, r3);
        }
        s += v_reduce_ਜੋੜ(v_ਜੋੜੋ(v_ਜੋੜੋ(v_ਜੋੜੋ(r0, r1), r2), r3));
        ਲਈ (; j < n; j++) {
            ਦੁੱਗਣਾ v = (ਦੁੱਗਣਾ)src1[j] - (ਦੁੱਗਣਾ)src2[j];
            s += v * v;
        }
        ਵਾਪਸ s;
    }
};

#ਜੇ_ਅੰਤ

ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T, ਕਿਸਮ_ਨਾਂ ST>
ਬਣਤਰ MaskedNormInf_SIMD {
    ਸਤਰਅੰਦਰ ST ਸੰਚਾਲਕ() (ਅਚਲ T* src, ਅਚਲ uchar* ਨਕਾਬ, ਪੂਰਨ len, ਪੂਰਨ cn) ਅਚਲ {
        ST s = 0;
        ਜੇ (cn == 1) {
            ਲਈ (ਪੂਰਨ i = 0; i < len; i++) {
                ਜੇ (ਨਕਾਬ[i]) {
                    s = std::max(s, (ST)cv_abs(src[i]));
                }
            }
        }
        ਨਹੀਂਤਾਂ {
            ਲਈ (ਪੂਰਨ i = 0; i < len; i++) {
                ਜੇ (ਨਕਾਬ[i]) {
                    ਅਚਲ T* elem = src + i * cn;
                    ਪੂਰਨ k = 0;
                #ਜੇ CV_ENABLE_UNROLLED && !CV_SIMD_SCALABLE
                    ਲਈ (; k <= cn - 4; k += 4) {
                        s = std::max(s, (ST)cv_abs(elem[k]));
                        s = std::max(s, (ST)cv_abs(elem[k + 1]));
                        s = std::max(s, (ST)cv_abs(elem[k + 2]));
                        s = std::max(s, (ST)cv_abs(elem[k + 3]));
                    }
                #ਜੇ_ਅੰਤ
                    ਲਈ (; k < cn; k++) {
                        s = std::max(s, (ST)cv_abs(elem[k]));
                    }
                }
            }
        }
        ਵਾਪਸ s;
    }
};

ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T, ਕਿਸਮ_ਨਾਂ ST>
ਬਣਤਰ MaskedNormL1_SIMD {
    ਸਤਰਅੰਦਰ ST ਸੰਚਾਲਕ() (ਅਚਲ T* src, ਅਚਲ uchar* ਨਕਾਬ, ਪੂਰਨ len, ਪੂਰਨ cn) ਅਚਲ {
        ST s = 0;
        ਜੇ (cn == 1) {
            ਲਈ (ਪੂਰਨ i = 0; i < len; i++) {
                ਜੇ (ਨਕਾਬ[i]) {
                    s += (ST)cv_abs(src[i]);
                }
            }
        }
        ਨਹੀਂਤਾਂ {
            ਲਈ (ਪੂਰਨ i = 0; i < len; i++) {
                ਜੇ (ਨਕਾਬ[i]) {
                    ਅਚਲ T* elem = src + i * cn;
                    ਪੂਰਨ k = 0;
                #ਜੇ CV_ENABLE_UNROLLED && !CV_SIMD_SCALABLE
                    ਲਈ (; k <= cn - 4; k += 4) {
                        s += (ST)cv_abs(elem[k]);
                        s += (ST)cv_abs(elem[k + 1]);
                        s += (ST)cv_abs(elem[k + 2]);
                        s += (ST)cv_abs(elem[k + 3]);
                    }
                #ਜੇ_ਅੰਤ
                    ਲਈ (; k < cn; k++) {
                        s += (ST)cv_abs(elem[k]);
                    }
                }
            }
        }
        ਵਾਪਸ s;
    }
};

ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T, ਕਿਸਮ_ਨਾਂ ST>
ਬਣਤਰ MaskedNormL2_SIMD {
    ਸਤਰਅੰਦਰ ST ਸੰਚਾਲਕ() (ਅਚਲ T* src, ਅਚਲ uchar* ਨਕਾਬ, ਪੂਰਨ len, ਪੂਰਨ cn) ਅਚਲ {
        ST s = 0;
        ਜੇ (cn == 1) {
            ਪੂਰਨ i = 0;
        #ਜੇ CV_ENABLE_UNROLLED && !CV_SIMD_SCALABLE
            ਲਈ (; i <= len - 4; i += 4) {
                ਜੇ (ਨਕਾਬ[i])     { T v0 = src[i];     s += (ST)v0 * v0; }
                ਜੇ (ਨਕਾਬ[i + 1]) { T v1 = src[i + 1]; s += (ST)v1 * v1; }
                ਜੇ (ਨਕਾਬ[i + 2]) { T v2 = src[i + 2]; s += (ST)v2 * v2; }
                ਜੇ (ਨਕਾਬ[i + 3]) { T v3 = src[i + 3]; s += (ST)v3 * v3; }
            }
        #ਜੇ_ਅੰਤ
            ਲਈ (; i < len; i++) {
                ਜੇ (ਨਕਾਬ[i]) {
                    T v = src[i];
                    s += (ST)v * v;
                }
            }
        }
        ਨਹੀਂਤਾਂ {
            ਲਈ (ਪੂਰਨ i = 0; i < len; i++) {
                ਜੇ (ਨਕਾਬ[i]) {
                    ਅਚਲ T* elem = src + i * cn;
                    ਪੂਰਨ k = 0;
                #ਜੇ CV_ENABLE_UNROLLED && !CV_SIMD_SCALABLE
                    ਲਈ (; k <= cn - 4; k += 4) {
                        T v0 = elem[k];     s += (ST)v0 * v0;
                        T v1 = elem[k + 1]; s += (ST)v1 * v1;
                        T v2 = elem[k + 2]; s += (ST)v2 * v2;
                        T v3 = elem[k + 3]; s += (ST)v3 * v3;
                    }
                #ਜੇ_ਅੰਤ
                    ਲਈ (; k < cn; k++) {
                        T v = elem[k];
                        s += (ST)v * v;
                    }
                }
            }
        }

        ਵਾਪਸ s;
    }
};

#ਜੇ (CV_SIMD || CV_SIMD_SCALABLE)
ਸਥਿਰ ਸਤਰਅੰਦਰ v_uint8  v_normmask_u8 (ਅਚਲ uchar* m) { ਵਾਪਸ v_gt(vx_ਲੱਦੋ(m),          vx_setzero_u8());  }
ਸਥਿਰ ਸਤਰਅੰਦਰ v_uint16 v_normmask_u16(ਅਚਲ uchar* m) { ਵਾਪਸ v_gt(vx_ਲੱਦੋ_expand(m),   vx_setzero_u16()); }
ਸਥਿਰ ਸਤਰਅੰਦਰ v_uint32 v_normmask_u32(ਅਚਲ uchar* m) { ਵਾਪਸ v_gt(vx_ਲੱਦੋ_expand_q(m), vx_setzero_u32()); }
ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ v_normmask_u64(ਅਚਲ uchar* m, v_uint64& m0, v_uint64& m1) {
    v_uint64 e0, e1;
    v_expand(v_normmask_u32(m), e0, e1);
    m0 = v_or(e0, v_shl<32>(e0));
    m1 = v_or(e1, v_shl<32>(e1));
}

ਸਾਂਚਾ <>
ਬਣਤਰ MaskedNormInf_SIMD<ਦਸ਼ਮਲਵ, ਦਸ਼ਮਲਵ> {
    ਸਤਰਅੰਦਰ ਦਸ਼ਮਲਵ ਸੰਚਾਲਕ()(ਅਚਲ ਦਸ਼ਮਲਵ* src, ਅਚਲ uchar* ਨਕਾਬ, ਪੂਰਨ len, ਪੂਰਨ cn) ਅਚਲ {
        ਦਸ਼ਮਲਵ ਨਤੀਜਾ = 0.0f;
        ਜੇ (cn == 1) {
            ਪੂਰਨ i = 0;
            ਅਚਲ ਪੂਰਨ vstep = VTraits<v_float32>::vlanes();
            v_float32 acc = vx_setzero_f32();

            ਲਈ (; i <= len - vstep; i += vstep) {
                v_uint32 m = vx_ਲੱਦੋ_expand_q(ਨਕਾਬ + i);
                v_uint32 cmp = v_gt(m, vx_setzero_u32());
                v_float32 s = vx_ਲੱਦੋ(src + i);
                s = v_abs(s);
                s = v_reinterpret_as_f32(v_and(v_reinterpret_as_u32(s), cmp));
                acc = v_max(acc, s);
            }
            ਨਤੀਜਾ = v_reduce_max(acc);

            ਲਈ (; i < len; i++) {
                ਜੇ (ਨਕਾਬ[i])
                    ਨਤੀਜਾ = std::max(ਨਤੀਜਾ, std::ਨਿਰਪੇਖ(src[i]));
            }
        }
        ਨਹੀਂਤਾਂ {
            ਲਈ (ਪੂਰਨ i = 0; i < len; i++) {
                ਜੇ (ਨਕਾਬ[i]) {
                    ਅਚਲ ਦਸ਼ਮਲਵ* elem = src + i * cn;
                    ਪੂਰਨ k = 0;
                    ਅਚਲ ਪੂਰਨ vstep = VTraits<v_float32>::vlanes();
                    v_float32 acc = vx_setzero_f32();

                    ਲਈ (; k <= cn - vstep; k += vstep) {
                        v_float32 s = vx_ਲੱਦੋ(elem + k);
                        acc = v_max(acc, v_abs(s));
                    }

                    ਨਤੀਜਾ = std::max(ਨਤੀਜਾ, v_reduce_max(acc));

                    ਲਈ (; k < cn; k++)
                        ਨਤੀਜਾ = std::max(ਨਤੀਜਾ, std::ਨਿਰਪੇਖ(elem[k]));
                }
            }
        }
        ਵਾਪਸ ਨਤੀਜਾ;
    }
};

#ਜੇ CV_SIMD_64F
ਸਾਂਚਾ <>
ਬਣਤਰ MaskedNormL1_SIMD<ਦਸ਼ਮਲਵ, ਦੁੱਗਣਾ> {
    ਸਤਰਅੰਦਰ ਦੁੱਗਣਾ ਸੰਚਾਲਕ()(ਅਚਲ ਦਸ਼ਮਲਵ* src, ਅਚਲ uchar* ਨਕਾਬ, ਪੂਰਨ len, ਪੂਰਨ cn) ਅਚਲ {
        ਦੁੱਗਣਾ ਨਤੀਜਾ = 0.0;
        ਜੇ (cn == 1) {
            ਪੂਰਨ i = 0;
            ਅਚਲ ਪੂਰਨ vstep = VTraits<v_float32>::vlanes();
            v_float64 acc0 = vx_setzero_f64(), acc1 = vx_setzero_f64();
            v_float64 acc2 = vx_setzero_f64(), acc3 = vx_setzero_f64();

            ਲਈ (; i <= len - 2*vstep; i += 2*vstep) {
                v_float32 s0 = v_and(v_abs(vx_ਲੱਦੋ(src + i)),
                                     v_reinterpret_as_f32(v_normmask_u32(ਨਕਾਬ + i)));
                v_float32 s1 = v_and(v_abs(vx_ਲੱਦੋ(src + i + vstep)),
                                     v_reinterpret_as_f32(v_normmask_u32(ਨਕਾਬ + i + vstep)));
                acc0 = v_ਜੋੜੋ(acc0, v_cvt_f64(s0));
                acc1 = v_ਜੋੜੋ(acc1, v_cvt_f64_ਉੱਚਾ(s0));
                acc2 = v_ਜੋੜੋ(acc2, v_cvt_f64(s1));
                acc3 = v_ਜੋੜੋ(acc3, v_cvt_f64_ਉੱਚਾ(s1));
            }
            ਲਈ (; i <= len - vstep; i += vstep) {
                v_float32 s = v_and(v_abs(vx_ਲੱਦੋ(src + i)),
                                    v_reinterpret_as_f32(v_normmask_u32(ਨਕਾਬ + i)));
                acc0 = v_ਜੋੜੋ(acc0, v_cvt_f64(s));
                acc1 = v_ਜੋੜੋ(acc1, v_cvt_f64_ਉੱਚਾ(s));
            }
            ਨਤੀਜਾ = v_reduce_ਜੋੜ(v_ਜੋੜੋ(v_ਜੋੜੋ(acc0, acc1), v_ਜੋੜੋ(acc2, acc3)));

            ਲਈ (; i < len; i++) {
                ਜੇ (ਨਕਾਬ[i])
                    ਨਤੀਜਾ += std::ਨਿਰਪੇਖ(src[i]);
            }
        }
        ਨਹੀਂਤਾਂ {
            ਲਈ (ਪੂਰਨ i = 0; i < len; i++) {
                ਜੇ (ਨਕਾਬ[i]) {
                    ਅਚਲ ਦਸ਼ਮਲਵ* elem = src + i * cn;
                    ਪੂਰਨ k = 0;
                    ਅਚਲ ਪੂਰਨ vstep = VTraits<v_float32>::vlanes();
                    v_float64 acc = vx_setzero_f64();

                    ਲਈ (; k <= cn - vstep; k += vstep) {
                        v_float32 s = v_abs(vx_ਲੱਦੋ(elem + k));
                        acc = v_ਜੋੜੋ(acc, v_cvt_f64(s));
                        acc = v_ਜੋੜੋ(acc, v_cvt_f64_ਉੱਚਾ(s));
                    }

                    ਨਤੀਜਾ += v_reduce_ਜੋੜ(acc);

                    ਲਈ (; k < cn; k++)
                        ਨਤੀਜਾ += std::ਨਿਰਪੇਖ(elem[k]);
                }
            }
        }
        ਵਾਪਸ ਨਤੀਜਾ;
    }
};

ਸਾਂਚਾ <>
ਬਣਤਰ MaskedNormL2_SIMD<ਦਸ਼ਮਲਵ, ਦੁੱਗਣਾ> {
    ਸਤਰਅੰਦਰ ਦੁੱਗਣਾ ਸੰਚਾਲਕ()(ਅਚਲ ਦਸ਼ਮਲਵ* src, ਅਚਲ uchar* ਨਕਾਬ, ਪੂਰਨ len, ਪੂਰਨ cn) ਅਚਲ {
        ਦੁੱਗਣਾ ਨਤੀਜਾ = 0.0;
        ਜੇ (cn == 1) {
            ਪੂਰਨ i = 0;
            ਅਚਲ ਪੂਰਨ vstep = VTraits<v_float32>::vlanes();
            v_float32 facc = vx_setzero_f32();
            v_float64 dacc = vx_setzero_f64();
            ਪੂਰਨ flush = 0;

            ਲਈ (; i <= len - vstep; i += vstep, flush += vstep) {
                ਜੇ (flush >= 64) {
                    dacc = v_ਜੋੜੋ(dacc, v_cvt_f64(facc));
                    dacc = v_ਜੋੜੋ(dacc, v_cvt_f64_ਉੱਚਾ(facc));
                    facc = vx_setzero_f32();
                    flush = 0;
                }
                v_uint32 cmp = v_gt(vx_ਲੱਦੋ_expand_q(ਨਕਾਬ + i), vx_setzero_u32());
                v_float32 s  = v_reinterpret_as_f32(v_and(v_reinterpret_as_u32(vx_ਲੱਦੋ(src + i)), cmp));
                facc = v_ਜੋੜੋ(facc, v_mul(s, s));
            }

            dacc = v_ਜੋੜੋ(dacc, v_cvt_f64(facc));
            dacc = v_ਜੋੜੋ(dacc, v_cvt_f64_ਉੱਚਾ(facc));
            ਨਤੀਜਾ = v_reduce_ਜੋੜ(dacc);

            ਲਈ (; i < len; i++) {
                ਜੇ (ਨਕਾਬ[i]) {
                    ਦੁੱਗਣਾ v = src[i];
                    ਨਤੀਜਾ += v * v;
                }
            }
        }
        ਨਹੀਂਤਾਂ {
            ਲਈ (ਪੂਰਨ i = 0; i < len; i++) {
                ਜੇ (ਨਕਾਬ[i]) {
                    ਅਚਲ ਦਸ਼ਮਲਵ* elem = src + i * cn;
                    ਪੂਰਨ k = 0;
                    ਅਚਲ ਪੂਰਨ vstep = VTraits<v_float32>::vlanes();
                    v_float32 facc = vx_setzero_f32();

                    ਲਈ (; k <= cn - vstep; k += vstep) {
                        v_float32 s = vx_ਲੱਦੋ(elem + k);
                        facc = v_ਜੋੜੋ(facc, v_mul(s, s));
                    }

                    v_float64 dacc = v_ਜੋੜੋ(v_cvt_f64(facc), v_cvt_f64_ਉੱਚਾ(facc));
                    ਨਤੀਜਾ += v_reduce_ਜੋੜ(dacc);

                    ਲਈ (; k < cn; k++) {
                        ਦੁੱਗਣਾ v = elem[k];
                        ਨਤੀਜਾ += v * v;
                    }
                }
            }
        }
        ਵਾਪਸ ਨਤੀਜਾ;
    }
};

#ਜੇ_ਅੰਤ

ਸਾਂਚਾ <>
ਬਣਤਰ MaskedNormInf_SIMD<uchar, ਪੂਰਨ> {
    ਸਤਰਅੰਦਰ ਪੂਰਨ ਸੰਚਾਲਕ()(ਅਚਲ uchar* src, ਅਚਲ uchar* ਨਕਾਬ, ਪੂਰਨ len, ਪੂਰਨ cn) ਅਚਲ {
        ਪੂਰਨ ਨਤੀਜਾ = 0;
        ਜੇ (cn == 1) {
            ਪੂਰਨ i = 0;
            ਅਚਲ ਪੂਰਨ vstep = VTraits<v_uint8>::vlanes();
            v_uint8 acc = vx_setzero_u8();

            ਲਈ (; i <= len - vstep; i += vstep) {
                v_uint8 m   = vx_ਲੱਦੋ(ਨਕਾਬ + i);
                v_uint8 s   = vx_ਲੱਦੋ(src + i);
                v_uint8 sel = v_and(s, v_gt(m, vx_setzero_u8()));
                acc = v_max(acc, sel);
            }

            ਨਤੀਜਾ = (ਪੂਰਨ)v_reduce_max(acc);

            ਲਈ (; i < len; i++) {
                ਜੇ (ਨਕਾਬ[i])
                    ਨਤੀਜਾ = std::max(ਨਤੀਜਾ, (ਪੂਰਨ)src[i]);
            }
        }
        ਨਹੀਂਤਾਂ ਜੇ (cn == 4 && len >= VTraits<v_uint8>::vlanes()) {
            ਅਚਲ ਪੂਰਨ vstep = VTraits<v_uint8>::vlanes();
            v_uint8 acc = vx_setzero_u8();
            ਪੂਰਨ i = 0;
            ਲਈ (;;) {
                ਜੇ (i > len - vstep)
                    i = len - vstep; // back-step (max is idempotent)
                v_uint8 c0, c1, c2, c3;
                v_ਲੱਦੋ_deinterleave(src + i * 4, c0, c1, c2, c3);
                v_uint8 pmax = v_max(v_max(c0, c1), v_max(c2, c3));
                v_uint8 m = v_gt(vx_ਲੱਦੋ(ਨਕਾਬ + i), vx_setzero_u8());
                acc = v_max(acc, v_and(pmax, m));
                ਜੇ (i >= len - vstep)
                    ਰੋਕੋ;
                i += vstep;
            }
            ਨਤੀਜਾ = (ਪੂਰਨ)v_reduce_max(acc);
        }
        ਨਹੀਂਤਾਂ ਜੇ (cn == 4) {
            ਲਈ (ਪੂਰਨ i = 0; i < len; i++) {
                ਜੇ (ਨਕਾਬ[i]) {
                    ਅਚਲ uchar* elem = src + i * 4;
                    ਨਤੀਜਾ = std::max(ਨਤੀਜਾ, (ਪੂਰਨ)std::max(std::max(elem[0], elem[1]),
                                                            std::max(elem[2], elem[3])));
                }
            }
        }
        ਨਹੀਂਤਾਂ {
            ਲਈ (ਪੂਰਨ i = 0; i < len; i++) {
                ਜੇ (ਨਕਾਬ[i]) {
                    ਅਚਲ uchar* elem = src + i * cn;
                    ਲਈ (ਪੂਰਨ k = 0; k < cn; k++)
                        ਨਤੀਜਾ = std::max(ਨਤੀਜਾ, (ਪੂਰਨ)elem[k]);
                }
            }
        }
        ਵਾਪਸ ਨਤੀਜਾ;
    }
};

ਸਾਂਚਾ <>
ਬਣਤਰ MaskedNormL1_SIMD<uchar, ਪੂਰਨ> {
    ਸਤਰਅੰਦਰ ਪੂਰਨ ਸੰਚਾਲਕ()(ਅਚਲ uchar* src, ਅਚਲ uchar* ਨਕਾਬ, ਪੂਰਨ len, ਪੂਰਨ cn) ਅਚਲ {
        ਪੂਰਨ ਨਤੀਜਾ = 0;
        ਅਚਲ ਪੂਰਨ vstep = VTraits<v_uint8>::vlanes();
        ਅਚਲ v_uint8 one = vx_setall_u8(1);
        ਜੇ (cn == 1) {
            ਪੂਰਨ i = 0;
            v_uint32 acc = vx_setzero_u32();
            ਲਈ (; i <= len - vstep; i += vstep) {
                v_uint8 s = v_and(vx_ਲੱਦੋ(src + i), v_normmask_u8(ਨਕਾਬ + i));
                acc = v_dotprod_expand_fast(s, one, acc);
            }
            ਨਤੀਜਾ = (ਪੂਰਨ)v_reduce_ਜੋੜ(acc);
            ਲਈ (; i < len; i++) {
                ਜੇ (ਨਕਾਬ[i])
                    ਨਤੀਜਾ += src[i];
            }
        }
        ਨਹੀਂਤਾਂ ਜੇ (cn == 4) {
            ਪੂਰਨ i = 0;
            v_uint32 acc = vx_setzero_u32();
            ਲਈ (; i <= len - vstep; i += vstep) {
                v_uint8 c0, c1, c2, c3;
                v_ਲੱਦੋ_deinterleave(src + i * 4, c0, c1, c2, c3);
                v_uint8 m = v_normmask_u8(ਨਕਾਬ + i);
                acc = v_dotprod_expand_fast(v_and(c0, m), one, acc);
                acc = v_dotprod_expand_fast(v_and(c1, m), one, acc);
                acc = v_dotprod_expand_fast(v_and(c2, m), one, acc);
                acc = v_dotprod_expand_fast(v_and(c3, m), one, acc);
            }
            ਨਤੀਜਾ = (ਪੂਰਨ)v_reduce_ਜੋੜ(acc);
            ਲਈ (; i < len; i++) {
                ਜੇ (ਨਕਾਬ[i]) {
                    ਅਚਲ uchar* elem = src + i * 4;
                    ਨਤੀਜਾ += elem[0] + elem[1] + elem[2] + elem[3];
                }
            }
        }
        ਨਹੀਂਤਾਂ {
            ਲਈ (ਪੂਰਨ i = 0; i < len; i++) {
                ਜੇ (ਨਕਾਬ[i]) {
                    ਅਚਲ uchar* elem = src + i * cn;
                    ਲਈ (ਪੂਰਨ k = 0; k < cn; k++)
                        ਨਤੀਜਾ += elem[k];
                }
            }
        }
        ਵਾਪਸ ਨਤੀਜਾ;
    }
};

ਸਾਂਚਾ <>
ਬਣਤਰ MaskedNormInf_SIMD<ushort, ਪੂਰਨ> {
    ਸਤਰਅੰਦਰ ਪੂਰਨ ਸੰਚਾਲਕ()(ਅਚਲ ushort* src, ਅਚਲ uchar* ਨਕਾਬ, ਪੂਰਨ len, ਪੂਰਨ cn) ਅਚਲ {
        ਪੂਰਨ ਨਤੀਜਾ = 0;
        ਜੇ (cn == 1) {
            ਪੂਰਨ i = 0;
            ਅਚਲ ਪੂਰਨ vstep = VTraits<v_uint16>::vlanes();
            v_uint16 acc = vx_setzero_u16();

            ਲਈ (; i <= len - vstep; i += vstep) {
                v_uint16 m   = vx_ਲੱਦੋ_expand(ਨਕਾਬ + i);
                v_uint16 cmp = v_gt(m, vx_setzero_u16());
                v_uint16 s   = vx_ਲੱਦੋ(src + i);
                v_uint16 sel = v_and(s, cmp);
                acc = v_max(acc, sel);
            }

            ਨਤੀਜਾ = (ਪੂਰਨ)v_reduce_max(acc);

            ਲਈ (; i < len; i++) {
                ਜੇ (ਨਕਾਬ[i])
                    ਨਤੀਜਾ = std::max(ਨਤੀਜਾ, (ਪੂਰਨ)src[i]);
            }
        }
        ਨਹੀਂਤਾਂ {
            ਲਈ (ਪੂਰਨ i = 0; i < len; i++) {
                ਜੇ (ਨਕਾਬ[i]) {
                    ਅਚਲ ushort* elem = src + i * cn;
                    ਪੂਰਨ k = 0;
                    ਅਚਲ ਪੂਰਨ vstep = VTraits<v_uint16>::vlanes();
                    v_uint16 acc = vx_setzero_u16();

                    ਲਈ (; k <= cn - vstep; k += vstep) {
                        acc = v_max(acc, vx_ਲੱਦੋ(elem + k));
                    }

                    ਨਤੀਜਾ = std::max(ਨਤੀਜਾ, (ਪੂਰਨ)v_reduce_max(acc));

                    ਲਈ (; k < cn; k++)
                        ਨਤੀਜਾ = std::max(ਨਤੀਜਾ, (ਪੂਰਨ)elem[k]);
                }
            }
        }
        ਵਾਪਸ ਨਤੀਜਾ;
    }
};

ਸਾਂਚਾ <>
ਬਣਤਰ MaskedNormL1_SIMD<ushort, ਪੂਰਨ> {
    ਸਤਰਅੰਦਰ ਪੂਰਨ ਸੰਚਾਲਕ()(ਅਚਲ ushort* src, ਅਚਲ uchar* ਨਕਾਬ, ਪੂਰਨ len, ਪੂਰਨ cn) ਅਚਲ {
        ਪੂਰਨ ਨਤੀਜਾ = 0;
        ਜੇ (cn == 1) {
            ਪੂਰਨ i = 0;
            ਅਚਲ ਪੂਰਨ vstep = VTraits<v_uint16>::vlanes();
            v_uint32 acc32 = vx_setzero_u32();
            v_uint64 acc64 = vx_setzero_u64();
            ਪੂਰਨ acc32_elems = 0;

            ਲਈ (; i <= len - vstep; i += vstep, acc32_elems += vstep) {
                ਜੇ (acc32_elems >= 512) {
                    v_uint64 lo64, hi64;
                    v_expand(acc32, lo64, hi64);
                    acc64 = v_ਜੋੜੋ(acc64, v_ਜੋੜੋ(lo64, hi64));
                    acc32 = vx_setzero_u32();
                    acc32_elems = 0;
                }
                v_uint16 m   = vx_ਲੱਦੋ_expand(ਨਕਾਬ + i);
                v_uint16 cmp = v_gt(m, vx_setzero_u16());
                v_uint16 s   = v_and(vx_ਲੱਦੋ(src + i), cmp);
                v_uint32 lo32, hi32;
                v_expand(s, lo32, hi32);
                acc32 = v_ਜੋੜੋ(acc32, v_ਜੋੜੋ(lo32, hi32));
            }

            v_uint64 lo64, hi64;
            v_expand(acc32, lo64, hi64);
            acc64 = v_ਜੋੜੋ(acc64, v_ਜੋੜੋ(lo64, hi64));
            ਨਤੀਜਾ = (ਪੂਰਨ)v_reduce_ਜੋੜ(acc64);

            ਲਈ (; i < len; i++) {
                ਜੇ (ਨਕਾਬ[i])
                    ਨਤੀਜਾ += src[i];
            }
        }
        ਨਹੀਂਤਾਂ {
            ਲਈ (ਪੂਰਨ i = 0; i < len; i++) {
                ਜੇ (ਨਕਾਬ[i]) {
                    ਅਚਲ ushort* elem = src + i * cn;
                    ਪੂਰਨ k = 0;
                    ਅਚਲ ਪੂਰਨ vstep = VTraits<v_uint16>::vlanes();
                    v_uint32 acc = vx_setzero_u32();

                    ਲਈ (; k <= cn - vstep; k += vstep) {
                        v_uint32 lo32, hi32;
                        v_expand(vx_ਲੱਦੋ(elem + k), lo32, hi32);
                        acc = v_ਜੋੜੋ(acc, v_ਜੋੜੋ(lo32, hi32));
                    }

                    ਨਤੀਜਾ += (ਪੂਰਨ)v_reduce_ਜੋੜ(acc);

                    ਲਈ (; k < cn; k++)
                        ਨਤੀਜਾ += elem[k];
                }
            }
        }
        ਵਾਪਸ ਨਤੀਜਾ;
    }
};

ਸਾਂਚਾ <>
ਬਣਤਰ MaskedNormL2_SIMD<ushort, ਦੁੱਗਣਾ> {
    ਸਤਰਅੰਦਰ ਦੁੱਗਣਾ ਸੰਚਾਲਕ()(ਅਚਲ ushort* src, ਅਚਲ uchar* ਨਕਾਬ, ਪੂਰਨ len, ਪੂਰਨ cn) ਅਚਲ {
        ਦੁੱਗਣਾ ਨਤੀਜਾ = 0.0;
        ਜੇ (cn == 1) {
            ਪੂਰਨ i = 0;
            ਅਚਲ ਪੂਰਨ vstep = VTraits<v_uint16>::vlanes();
            v_uint64 acc = vx_setzero_u64();
            ਲਈ (; i <= len - vstep; i += vstep) {
                v_uint16 m   = vx_ਲੱਦੋ_expand(ਨਕਾਬ + i);
                v_uint16 cmp = v_gt(m, vx_setzero_u16());
                v_uint16 s   = v_and(vx_ਲੱਦੋ(src + i), cmp);
                v_uint32 lo32, hi32;
                v_expand(s, lo32, hi32);
                v_uint64 lo64a, lo64b, hi64a, hi64b;
                v_expand(v_mul(lo32, lo32), lo64a, lo64b);
                v_expand(v_mul(hi32, hi32), hi64a, hi64b);
                acc = v_ਜੋੜੋ(acc, v_ਜੋੜੋ(v_ਜੋੜੋ(lo64a, lo64b), v_ਜੋੜੋ(hi64a, hi64b)));
            }
            ਨਤੀਜਾ = (ਦੁੱਗਣਾ)v_reduce_ਜੋੜ(acc);
            ਲਈ (; i < len; i++) {
                ਜੇ (ਨਕਾਬ[i]) {
                    ਦੁੱਗਣਾ v = src[i];
                    ਨਤੀਜਾ += v * v;
                }
            }
        }
        ਨਹੀਂਤਾਂ {
            ਲਈ (ਪੂਰਨ i = 0; i < len; i++) {
                ਜੇ (ਨਕਾਬ[i]) {
                    ਅਚਲ ushort* elem = src + i * cn;
                    ਪੂਰਨ k = 0;
                    ਅਚਲ ਪੂਰਨ vstep = VTraits<v_uint16>::vlanes();
                    v_uint64 acc = vx_setzero_u64();
                    ਲਈ (; k <= cn - vstep; k += vstep) {
                        v_uint32 lo32, hi32;
                        v_expand(vx_ਲੱਦੋ(elem + k), lo32, hi32);
                        v_uint64 lo64a, lo64b, hi64a, hi64b;
                        v_expand(v_mul(lo32, lo32), lo64a, lo64b);
                        v_expand(v_mul(hi32, hi32), hi64a, hi64b);
                        acc = v_ਜੋੜੋ(acc, v_ਜੋੜੋ(v_ਜੋੜੋ(lo64a, lo64b), v_ਜੋੜੋ(hi64a, hi64b)));
                    }

                    ਨਤੀਜਾ += (ਦੁੱਗਣਾ)v_reduce_ਜੋੜ(acc);

                    ਲਈ (; k < cn; k++) {
                        ਦੁੱਗਣਾ v = elem[k];
                        ਨਤੀਜਾ += v * v;
                    }
                }
            }
        }
        ਵਾਪਸ ਨਤੀਜਾ;
    }
};

ਸਾਂਚਾ <>
ਬਣਤਰ MaskedNormInf_SIMD<schar, ਪੂਰਨ> {
    ਸਤਰਅੰਦਰ ਪੂਰਨ ਸੰਚਾਲਕ()(ਅਚਲ schar* src, ਅਚਲ uchar* ਨਕਾਬ, ਪੂਰਨ len, ਪੂਰਨ cn) ਅਚਲ {
        ਪੂਰਨ ਨਤੀਜਾ = 0;
        ਜੇ (cn == 1) {
            ਅਚਲ ਪੂਰਨ vstep = VTraits<v_int8>::vlanes();
            ਪੂਰਨ i = 0;
            v_uint8 acc = vx_setzero_u8();
            ਲਈ (; i <= len - vstep; i += vstep)
                acc = v_max(acc, v_and(v_abs(vx_ਲੱਦੋ(src + i)), v_normmask_u8(ਨਕਾਬ + i)));
            ਨਤੀਜਾ = (ਪੂਰਨ)v_reduce_max(acc);
            ਲਈ (; i < len; i++)
                ਜੇ (ਨਕਾਬ[i]) ਨਤੀਜਾ = std::max(ਨਤੀਜਾ, cv_abs(src[i]));
        }
        ਨਹੀਂਤਾਂ {
            ਲਈ (ਪੂਰਨ i = 0; i < len; i++)
                ਜੇ (ਨਕਾਬ[i]) {
                    ਅਚਲ schar* elem = src + i * cn;
                    ਲਈ (ਪੂਰਨ k = 0; k < cn; k++)
                        ਨਤੀਜਾ = std::max(ਨਤੀਜਾ, cv_abs(elem[k]));
                }
        }
        ਵਾਪਸ ਨਤੀਜਾ;
    }
};

ਸਾਂਚਾ <>
ਬਣਤਰ MaskedNormL1_SIMD<schar, ਪੂਰਨ> {
    ਸਤਰਅੰਦਰ ਪੂਰਨ ਸੰਚਾਲਕ()(ਅਚਲ schar* src, ਅਚਲ uchar* ਨਕਾਬ, ਪੂਰਨ len, ਪੂਰਨ cn) ਅਚਲ {
        ਪੂਰਨ ਨਤੀਜਾ = 0;
        ਜੇ (cn == 1) {
            ਅਚਲ ਪੂਰਨ vstep = VTraits<v_int8>::vlanes();
            ਅਚਲ v_uint8 one = vx_setall_u8(1);
            ਪੂਰਨ i = 0;
            v_uint32 acc = vx_setzero_u32();
            ਲਈ (; i <= len - vstep; i += vstep)
                acc = v_dotprod_expand_fast(v_and(v_abs(vx_ਲੱਦੋ(src + i)), v_normmask_u8(ਨਕਾਬ + i)), one, acc);
            ਨਤੀਜਾ = (ਪੂਰਨ)v_reduce_ਜੋੜ(acc);
            ਲਈ (; i < len; i++)
                ਜੇ (ਨਕਾਬ[i]) ਨਤੀਜਾ += cv_abs(src[i]);
        }
        ਨਹੀਂਤਾਂ {
            ਲਈ (ਪੂਰਨ i = 0; i < len; i++)
                ਜੇ (ਨਕਾਬ[i]) {
                    ਅਚਲ schar* elem = src + i * cn;
                    ਲਈ (ਪੂਰਨ k = 0; k < cn; k++)
                        ਨਤੀਜਾ += cv_abs(elem[k]);
                }
        }
        ਵਾਪਸ ਨਤੀਜਾ;
    }
};

ਸਾਂਚਾ <>
ਬਣਤਰ MaskedNormL2_SIMD<schar, ਪੂਰਨ> {
    ਸਤਰਅੰਦਰ ਪੂਰਨ ਸੰਚਾਲਕ()(ਅਚਲ schar* src, ਅਚਲ uchar* ਨਕਾਬ, ਪੂਰਨ len, ਪੂਰਨ cn) ਅਚਲ {
        ਪੂਰਨ ਨਤੀਜਾ = 0;
        ਜੇ (cn == 1) {
            ਅਚਲ ਪੂਰਨ vstep = VTraits<v_int8>::vlanes();
            ਪੂਰਨ i = 0;
            v_int32 acc = vx_setzero_s32();
            ਲਈ (; i <= len - vstep; i += vstep) {
                v_int8 s = v_and(vx_ਲੱਦੋ(src + i), v_reinterpret_as_s8(v_normmask_u8(ਨਕਾਬ + i)));
                acc = v_dotprod_expand_fast(s, s, acc);
            }
            ਨਤੀਜਾ = (ਪੂਰਨ)v_reduce_ਜੋੜ(acc);
            ਲਈ (; i < len; i++)
                ਜੇ (ਨਕਾਬ[i]) { ਪੂਰਨ v = src[i]; ਨਤੀਜਾ += v * v; }
        }
        ਨਹੀਂਤਾਂ {
            ਲਈ (ਪੂਰਨ i = 0; i < len; i++)
                ਜੇ (ਨਕਾਬ[i]) {
                    ਅਚਲ schar* elem = src + i * cn;
                    ਲਈ (ਪੂਰਨ k = 0; k < cn; k++) { ਪੂਰਨ v = elem[k]; ਨਤੀਜਾ += v * v; }
                }
        }
        ਵਾਪਸ ਨਤੀਜਾ;
    }
};

ਸਾਂਚਾ <>
ਬਣਤਰ MaskedNormL2_SIMD<uchar, ਪੂਰਨ> {
    ਸਤਰਅੰਦਰ ਪੂਰਨ ਸੰਚਾਲਕ()(ਅਚਲ uchar* src, ਅਚਲ uchar* ਨਕਾਬ, ਪੂਰਨ len, ਪੂਰਨ cn) ਅਚਲ {
        ਪੂਰਨ ਨਤੀਜਾ = 0;
        ਅਚਲ ਪੂਰਨ vstep = VTraits<v_uint8>::vlanes();
        ਜੇ (cn == 1) {
            ਪੂਰਨ i = 0;
            v_uint32 acc = vx_setzero_u32();
            ਲਈ (; i <= len - vstep; i += vstep) {
                v_uint8 s = v_and(vx_ਲੱਦੋ(src + i), v_normmask_u8(ਨਕਾਬ + i));
                acc = v_dotprod_expand_fast(s, s, acc);
            }
            ਨਤੀਜਾ = (ਪੂਰਨ)v_reduce_ਜੋੜ(acc);
            ਲਈ (; i < len; i++)
                ਜੇ (ਨਕਾਬ[i]) { ਪੂਰਨ v = src[i]; ਨਤੀਜਾ += v * v; }
        }
        ਨਹੀਂਤਾਂ ਜੇ (cn == 4) {
            ਪੂਰਨ i = 0;
            v_uint32 acc = vx_setzero_u32();
            ਲਈ (; i <= len - vstep; i += vstep) {
                v_uint8 c0, c1, c2, c3;
                v_ਲੱਦੋ_deinterleave(src + i * 4, c0, c1, c2, c3);
                v_uint8 m = v_normmask_u8(ਨਕਾਬ + i);
                c0 = v_and(c0, m); c1 = v_and(c1, m);
                c2 = v_and(c2, m); c3 = v_and(c3, m);
                acc = v_dotprod_expand_fast(c0, c0, acc);
                acc = v_dotprod_expand_fast(c1, c1, acc);
                acc = v_dotprod_expand_fast(c2, c2, acc);
                acc = v_dotprod_expand_fast(c3, c3, acc);
            }
            ਨਤੀਜਾ = (ਪੂਰਨ)v_reduce_ਜੋੜ(acc);
            ਲਈ (; i < len; i++)
                ਜੇ (ਨਕਾਬ[i]) {
                    ਅਚਲ uchar* elem = src + i * 4;
                    ਲਈ (ਪੂਰਨ k = 0; k < 4; k++) { ਪੂਰਨ v = elem[k]; ਨਤੀਜਾ += v * v; }
                }
        }
        ਨਹੀਂਤਾਂ {
            ਲਈ (ਪੂਰਨ i = 0; i < len; i++)
                ਜੇ (ਨਕਾਬ[i]) {
                    ਅਚਲ uchar* elem = src + i * cn;
                    ਲਈ (ਪੂਰਨ k = 0; k < cn; k++) { ਪੂਰਨ v = elem[k]; ਨਤੀਜਾ += v * v; }
                }
        }
        ਵਾਪਸ ਨਤੀਜਾ;
    }
};

ਸਾਂਚਾ <>
ਬਣਤਰ MaskedNormInf_SIMD<ਛੋਟਾ, ਪੂਰਨ> {
    ਸਤਰਅੰਦਰ ਪੂਰਨ ਸੰਚਾਲਕ()(ਅਚਲ ਛੋਟਾ* src, ਅਚਲ uchar* ਨਕਾਬ, ਪੂਰਨ len, ਪੂਰਨ cn) ਅਚਲ {
        ਪੂਰਨ ਨਤੀਜਾ = 0;
        ਜੇ (cn == 1) {
            ਅਚਲ ਪੂਰਨ vstep = VTraits<v_int16>::vlanes();
            ਪੂਰਨ i = 0;
            v_uint16 acc = vx_setzero_u16();
            ਲਈ (; i <= len - vstep; i += vstep)
                acc = v_max(acc, v_and(v_abs(vx_ਲੱਦੋ(src + i)), v_normmask_u16(ਨਕਾਬ + i)));
            ਨਤੀਜਾ = (ਪੂਰਨ)v_reduce_max(acc);
            ਲਈ (; i < len; i++)
                ਜੇ (ਨਕਾਬ[i]) ਨਤੀਜਾ = std::max(ਨਤੀਜਾ, cv_abs(src[i]));
        }
        ਨਹੀਂਤਾਂ {
            ਲਈ (ਪੂਰਨ i = 0; i < len; i++)
                ਜੇ (ਨਕਾਬ[i]) {
                    ਅਚਲ ਛੋਟਾ* elem = src + i * cn;
                    ਲਈ (ਪੂਰਨ k = 0; k < cn; k++)
                        ਨਤੀਜਾ = std::max(ਨਤੀਜਾ, cv_abs(elem[k]));
                }
        }
        ਵਾਪਸ ਨਤੀਜਾ;
    }
};

ਸਾਂਚਾ <>
ਬਣਤਰ MaskedNormL1_SIMD<ਛੋਟਾ, ਪੂਰਨ> {
    ਸਤਰਅੰਦਰ ਪੂਰਨ ਸੰਚਾਲਕ()(ਅਚਲ ਛੋਟਾ* src, ਅਚਲ uchar* ਨਕਾਬ, ਪੂਰਨ len, ਪੂਰਨ cn) ਅਚਲ {
        ਪੂਰਨ ਨਤੀਜਾ = 0;
        ਜੇ (cn == 1) {
            ਅਚਲ ਪੂਰਨ vstep = VTraits<v_int16>::vlanes();
            ਪੂਰਨ i = 0;
            v_uint32 acc0 = vx_setzero_u32(), acc1 = vx_setzero_u32();
            ਲਈ (; i <= len - vstep; i += vstep) {
                v_uint16 s = v_and(v_abs(vx_ਲੱਦੋ(src + i)), v_normmask_u16(ਨਕਾਬ + i));
                v_uint32 lo, hi;
                v_expand(s, lo, hi);
                acc0 = v_ਜੋੜੋ(acc0, lo);
                acc1 = v_ਜੋੜੋ(acc1, hi);
            }
            ਨਤੀਜਾ = (ਪੂਰਨ)v_reduce_ਜੋੜ(v_ਜੋੜੋ(acc0, acc1));
            ਲਈ (; i < len; i++)
                ਜੇ (ਨਕਾਬ[i]) ਨਤੀਜਾ += cv_abs(src[i]);
        }
        ਨਹੀਂਤਾਂ {
            ਲਈ (ਪੂਰਨ i = 0; i < len; i++)
                ਜੇ (ਨਕਾਬ[i]) {
                    ਅਚਲ ਛੋਟਾ* elem = src + i * cn;
                    ਲਈ (ਪੂਰਨ k = 0; k < cn; k++)
                        ਨਤੀਜਾ += cv_abs(elem[k]);
                }
        }
        ਵਾਪਸ ਨਤੀਜਾ;
    }
};

ਸਾਂਚਾ <>
ਬਣਤਰ MaskedNormL2_SIMD<ਛੋਟਾ, ਦੁੱਗਣਾ> {
    ਸਤਰਅੰਦਰ ਦੁੱਗਣਾ ਸੰਚਾਲਕ()(ਅਚਲ ਛੋਟਾ* src, ਅਚਲ uchar* ਨਕਾਬ, ਪੂਰਨ len, ਪੂਰਨ cn) ਅਚਲ {
        ਦੁੱਗਣਾ ਨਤੀਜਾ = 0.0;
        ਜੇ (cn == 1) {
            ਅਚਲ ਪੂਰਨ vstep = VTraits<v_int16>::vlanes();
            ਪੂਰਨ i = 0;
            v_uint64 acc = vx_setzero_u64();
            ਲਈ (; i <= len - vstep; i += vstep) {
                v_uint16 s = v_and(v_abs(vx_ਲੱਦੋ(src + i)), v_normmask_u16(ਨਕਾਬ + i));
                acc = v_ਜੋੜੋ(acc, v_dotprod_expand_fast(s, s));
            }
            ਨਤੀਜਾ = (ਦੁੱਗਣਾ)v_reduce_ਜੋੜ(acc);
            ਲਈ (; i < len; i++)
                ਜੇ (ਨਕਾਬ[i]) { ਦੁੱਗਣਾ v = src[i]; ਨਤੀਜਾ += v * v; }
        }
        ਨਹੀਂਤਾਂ {
            ਲਈ (ਪੂਰਨ i = 0; i < len; i++)
                ਜੇ (ਨਕਾਬ[i]) {
                    ਅਚਲ ਛੋਟਾ* elem = src + i * cn;
                    ਲਈ (ਪੂਰਨ k = 0; k < cn; k++) { ਦੁੱਗਣਾ v = elem[k]; ਨਤੀਜਾ += v * v; }
                }
        }
        ਵਾਪਸ ਨਤੀਜਾ;
    }
};

ਸਾਂਚਾ <>
ਬਣਤਰ MaskedNormInf_SIMD<ਪੂਰਨ, ਬਿਨਾਨਿਸ਼ਾਨੀ> {
    ਸਤਰਅੰਦਰ ਬਿਨਾਨਿਸ਼ਾਨੀ ਸੰਚਾਲਕ()(ਅਚਲ ਪੂਰਨ* src, ਅਚਲ uchar* ਨਕਾਬ, ਪੂਰਨ len, ਪੂਰਨ cn) ਅਚਲ {
        ਬਿਨਾਨਿਸ਼ਾਨੀ ਨਤੀਜਾ = 0;
        ਜੇ (cn == 1) {
            ਅਚਲ ਪੂਰਨ vstep = VTraits<v_int32>::vlanes();
            ਪੂਰਨ i = 0;
            v_uint32 acc = vx_setzero_u32();
            ਲਈ (; i <= len - vstep; i += vstep)
                acc = v_max(acc, v_and(v_abs(vx_ਲੱਦੋ(src + i)), v_normmask_u32(ਨਕਾਬ + i)));
            ਨਤੀਜਾ = v_reduce_max(acc);
            ਲਈ (; i < len; i++)
                ਜੇ (ਨਕਾਬ[i]) ਨਤੀਜਾ = std::max(ਨਤੀਜਾ, cv_abs(src[i]));
        }
        ਨਹੀਂਤਾਂ {
            ਲਈ (ਪੂਰਨ i = 0; i < len; i++)
                ਜੇ (ਨਕਾਬ[i]) {
                    ਅਚਲ ਪੂਰਨ* elem = src + i * cn;
                    ਲਈ (ਪੂਰਨ k = 0; k < cn; k++)
                        ਨਤੀਜਾ = std::max(ਨਤੀਜਾ, cv_abs(elem[k]));
                }
        }
        ਵਾਪਸ ਨਤੀਜਾ;
    }
};

#ਜੇ CV_SIMD_64F || CV_SIMD_SCALABLE_64F

ਸਾਂਚਾ <>
ਬਣਤਰ MaskedNormL1_SIMD<ਪੂਰਨ, ਦੁੱਗਣਾ> {
    ਸਤਰਅੰਦਰ ਦੁੱਗਣਾ ਸੰਚਾਲਕ()(ਅਚਲ ਪੂਰਨ* src, ਅਚਲ uchar* ਨਕਾਬ, ਪੂਰਨ len, ਪੂਰਨ cn) ਅਚਲ {
        ਦੁੱਗਣਾ ਨਤੀਜਾ = 0.0;
        ਜੇ (cn == 1) {
            ਅਚਲ ਪੂਰਨ vstep = VTraits<v_int32>::vlanes();
            ਪੂਰਨ i = 0;
            v_float64 acc0 = vx_setzero_f64(), acc1 = vx_setzero_f64();
            ਲਈ (; i <= len - vstep; i += vstep) {
                // Mask the value, then widen and take the magnitude in double. v_abs() does give
                // |INT_MIN| as an unsigned 2147483648, but reinterpreting that back as int32 turns
                // it negative again, and the widening then carries the sign into the accumulator.
                v_int32 s = v_reinterpret_as_s32(v_and(v_reinterpret_as_u32(vx_ਲੱਦੋ(src + i)),
                                                       v_normmask_u32(ਨਕਾਬ + i)));
                acc0 = v_ਜੋੜੋ(acc0, v_abs(v_cvt_f64(s)));
                acc1 = v_ਜੋੜੋ(acc1, v_abs(v_cvt_f64_ਉੱਚਾ(s)));
            }
            ਨਤੀਜਾ = v_reduce_ਜੋੜ(v_ਜੋੜੋ(acc0, acc1));
            ਲਈ (; i < len; i++)
                ਜੇ (ਨਕਾਬ[i]) ਨਤੀਜਾ += cv_abs(src[i]);
        }
        ਨਹੀਂਤਾਂ {
            ਲਈ (ਪੂਰਨ i = 0; i < len; i++)
                ਜੇ (ਨਕਾਬ[i]) {
                    ਅਚਲ ਪੂਰਨ* elem = src + i * cn;
                    ਲਈ (ਪੂਰਨ k = 0; k < cn; k++)
                        ਨਤੀਜਾ += cv_abs(elem[k]);
                }
        }
        ਵਾਪਸ ਨਤੀਜਾ;
    }
};

ਸਾਂਚਾ <>
ਬਣਤਰ MaskedNormL2_SIMD<ਪੂਰਨ, ਦੁੱਗਣਾ> {
    ਸਤਰਅੰਦਰ ਦੁੱਗਣਾ ਸੰਚਾਲਕ()(ਅਚਲ ਪੂਰਨ* src, ਅਚਲ uchar* ਨਕਾਬ, ਪੂਰਨ len, ਪੂਰਨ cn) ਅਚਲ {
        ਦੁੱਗਣਾ ਨਤੀਜਾ = 0.0;
        ਜੇ (cn == 1) {
            ਅਚਲ ਪੂਰਨ vstep = VTraits<v_int32>::vlanes();
            ਪੂਰਨ i = 0;
            v_float64 acc0 = vx_setzero_f64(), acc1 = vx_setzero_f64();
            ਲਈ (; i <= len - vstep; i += vstep) {
                v_uint32 s = v_and(v_abs(vx_ਲੱਦੋ(src + i)), v_normmask_u32(ਨਕਾਬ + i));
                v_uint64 e0, e1;
                v_expand(s, e0, e1);
                v_float64 f0 = v_cvt_f64(v_reinterpret_as_s64(e0));
                v_float64 f1 = v_cvt_f64(v_reinterpret_as_s64(e1));
                acc0 = v_fma(f0, f0, acc0);
                acc1 = v_fma(f1, f1, acc1);
            }
            ਨਤੀਜਾ = v_reduce_ਜੋੜ(v_ਜੋੜੋ(acc0, acc1));
            ਲਈ (; i < len; i++)
                ਜੇ (ਨਕਾਬ[i]) { ਦੁੱਗਣਾ v = src[i]; ਨਤੀਜਾ += v * v; }
        }
        ਨਹੀਂਤਾਂ {
            ਲਈ (ਪੂਰਨ i = 0; i < len; i++)
                ਜੇ (ਨਕਾਬ[i]) {
                    ਅਚਲ ਪੂਰਨ* elem = src + i * cn;
                    ਲਈ (ਪੂਰਨ k = 0; k < cn; k++) { ਦੁੱਗਣਾ v = elem[k]; ਨਤੀਜਾ += v * v; }
                }
        }
        ਵਾਪਸ ਨਤੀਜਾ;
    }
};

ਸਾਂਚਾ <>
ਬਣਤਰ MaskedNormInf_SIMD<ਦੁੱਗਣਾ, ਦੁੱਗਣਾ> {
    ਸਤਰਅੰਦਰ ਦੁੱਗਣਾ ਸੰਚਾਲਕ()(ਅਚਲ ਦੁੱਗਣਾ* src, ਅਚਲ uchar* ਨਕਾਬ, ਪੂਰਨ len, ਪੂਰਨ cn) ਅਚਲ {
        ਦੁੱਗਣਾ ਨਤੀਜਾ = 0.0;
        ਜੇ (cn == 1) {
            ਅਚਲ ਪੂਰਨ vstep = VTraits<v_float64>::vlanes();
            ਪੂਰਨ i = 0;
            v_float64 acc0 = vx_setzero_f64(), acc1 = vx_setzero_f64();
            ਲਈ (; i <= len - 2*vstep; i += 2*vstep) {
                v_uint64 m0, m1;
                v_normmask_u64(ਨਕਾਬ + i, m0, m1);
                acc0 = v_max(acc0, v_reinterpret_as_f64(v_and(v_reinterpret_as_u64(v_abs(vx_ਲੱਦੋ(src + i))), m0)));
                acc1 = v_max(acc1, v_reinterpret_as_f64(v_and(v_reinterpret_as_u64(v_abs(vx_ਲੱਦੋ(src + i + vstep))), m1)));
            }
            ਦੁੱਗਣਾ t[VTraits<v_float64>::max_nlanes];
            vx_store(t, v_max(acc0, acc1));
            ਲਈ (ਪੂਰਨ k = 0; k < vstep; k++)
                ਨਤੀਜਾ = std::max(ਨਤੀਜਾ, t[k]);
            ਲਈ (; i < len; i++)
                ਜੇ (ਨਕਾਬ[i]) ਨਤੀਜਾ = std::max(ਨਤੀਜਾ, cv_abs(src[i]));
        }
        ਨਹੀਂਤਾਂ {
            ਲਈ (ਪੂਰਨ i = 0; i < len; i++)
                ਜੇ (ਨਕਾਬ[i]) {
                    ਅਚਲ ਦੁੱਗਣਾ* elem = src + i * cn;
                    ਲਈ (ਪੂਰਨ k = 0; k < cn; k++)
                        ਨਤੀਜਾ = std::max(ਨਤੀਜਾ, cv_abs(elem[k]));
                }
        }
        ਵਾਪਸ ਨਤੀਜਾ;
    }
};

ਸਾਂਚਾ <>
ਬਣਤਰ MaskedNormL1_SIMD<ਦੁੱਗਣਾ, ਦੁੱਗਣਾ> {
    ਸਤਰਅੰਦਰ ਦੁੱਗਣਾ ਸੰਚਾਲਕ()(ਅਚਲ ਦੁੱਗਣਾ* src, ਅਚਲ uchar* ਨਕਾਬ, ਪੂਰਨ len, ਪੂਰਨ cn) ਅਚਲ {
        ਦੁੱਗਣਾ ਨਤੀਜਾ = 0.0;
        ਜੇ (cn == 1) {
            ਅਚਲ ਪੂਰਨ vstep = VTraits<v_float64>::vlanes();
            ਪੂਰਨ i = 0;
            v_float64 acc0 = vx_setzero_f64(), acc1 = vx_setzero_f64();
            ਲਈ (; i <= len - 2*vstep; i += 2*vstep) {
                v_uint64 m0, m1;
                v_normmask_u64(ਨਕਾਬ + i, m0, m1);
                acc0 = v_ਜੋੜੋ(acc0, v_reinterpret_as_f64(v_and(v_reinterpret_as_u64(v_abs(vx_ਲੱਦੋ(src + i))), m0)));
                acc1 = v_ਜੋੜੋ(acc1, v_reinterpret_as_f64(v_and(v_reinterpret_as_u64(v_abs(vx_ਲੱਦੋ(src + i + vstep))), m1)));
            }
            ਨਤੀਜਾ = v_reduce_ਜੋੜ(v_ਜੋੜੋ(acc0, acc1));
            ਲਈ (; i < len; i++)
                ਜੇ (ਨਕਾਬ[i]) ਨਤੀਜਾ += cv_abs(src[i]);
        }
        ਨਹੀਂਤਾਂ {
            ਲਈ (ਪੂਰਨ i = 0; i < len; i++)
                ਜੇ (ਨਕਾਬ[i]) {
                    ਅਚਲ ਦੁੱਗਣਾ* elem = src + i * cn;
                    ਲਈ (ਪੂਰਨ k = 0; k < cn; k++)
                        ਨਤੀਜਾ += cv_abs(elem[k]);
                }
        }
        ਵਾਪਸ ਨਤੀਜਾ;
    }
};

ਸਾਂਚਾ <>
ਬਣਤਰ MaskedNormL2_SIMD<ਦੁੱਗਣਾ, ਦੁੱਗਣਾ> {
    ਸਤਰਅੰਦਰ ਦੁੱਗਣਾ ਸੰਚਾਲਕ()(ਅਚਲ ਦੁੱਗਣਾ* src, ਅਚਲ uchar* ਨਕਾਬ, ਪੂਰਨ len, ਪੂਰਨ cn) ਅਚਲ {
        ਦੁੱਗਣਾ ਨਤੀਜਾ = 0.0;
        ਜੇ (cn == 1) {
            ਅਚਲ ਪੂਰਨ vstep = VTraits<v_float64>::vlanes();
            ਪੂਰਨ i = 0;
            v_float64 acc0 = vx_setzero_f64(), acc1 = vx_setzero_f64();
            ਲਈ (; i <= len - 2*vstep; i += 2*vstep) {
                v_uint64 m0, m1;
                v_normmask_u64(ਨਕਾਬ + i, m0, m1);
                v_float64 s0 = v_reinterpret_as_f64(v_and(v_reinterpret_as_u64(vx_ਲੱਦੋ(src + i)), m0));
                v_float64 s1 = v_reinterpret_as_f64(v_and(v_reinterpret_as_u64(vx_ਲੱਦੋ(src + i + vstep)), m1));
                acc0 = v_fma(s0, s0, acc0);
                acc1 = v_fma(s1, s1, acc1);
            }
            ਨਤੀਜਾ = v_reduce_ਜੋੜ(v_ਜੋੜੋ(acc0, acc1));
            ਲਈ (; i < len; i++)
                ਜੇ (ਨਕਾਬ[i]) { ਦੁੱਗਣਾ v = src[i]; ਨਤੀਜਾ += v * v; }
        }
        ਨਹੀਂਤਾਂ {
            ਲਈ (ਪੂਰਨ i = 0; i < len; i++)
                ਜੇ (ਨਕਾਬ[i]) {
                    ਅਚਲ ਦੁੱਗਣਾ* elem = src + i * cn;
                    ਲਈ (ਪੂਰਨ k = 0; k < cn; k++) { ਦੁੱਗਣਾ v = elem[k]; ਨਤੀਜਾ += v * v; }
                }
        }
        ਵਾਪਸ ਨਤੀਜਾ;
    }
};

#ਜੇ_ਅੰਤ // CV_SIMD_64F || CV_SIMD_SCALABLE_64F
#ਜੇ_ਅੰਤ // CV_SIMD || CV_SIMD_SCALABLE

ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ T, ਕਿਸਮ_ਨਾਂ ST> ਪੂਰਨ
normInf_(ਅਚਲ T* src, ਅਚਲ uchar* ਨਕਾਬ, ST* ਨਤੀਜਾ_2, ਪੂਰਨ len, ਪੂਰਨ cn)
{
    ST ਨਤੀਜਾ = *ਨਤੀਜਾ_2;
    ਜੇ( !ਨਕਾਬ )
    {
        NormInf_SIMD<T, ST> op;
        ਨਤੀਜਾ = std::max(ਨਤੀਜਾ, op(src, len*cn));
    } ਨਹੀਂਤਾਂ {
        MaskedNormInf_SIMD<T, ST> op;
        ਨਤੀਜਾ = std::max(ਨਤੀਜਾ, op(src, ਨਕਾਬ, len, cn));
    }
    *ਨਤੀਜਾ_2 = ਨਤੀਜਾ;
    ਵਾਪਸ 0;
}

ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ T, ਕਿਸਮ_ਨਾਂ ST> ਪੂਰਨ
normL1_(ਅਚਲ T* src, ਅਚਲ uchar* ਨਕਾਬ, ST* ਨਤੀਜਾ_2, ਪੂਰਨ len, ਪੂਰਨ cn)
{
    ST ਨਤੀਜਾ = *ਨਤੀਜਾ_2;
    ਜੇ( !ਨਕਾਬ )
    {
        NormL1_SIMD<T, ST> op;
        ਨਤੀਜਾ += op(src, len*cn);
    } ਨਹੀਂਤਾਂ {
        MaskedNormL1_SIMD<T, ST> op;
        ਨਤੀਜਾ += op(src, ਨਕਾਬ, len, cn);
    }
    *ਨਤੀਜਾ_2 = ਨਤੀਜਾ;
    ਵਾਪਸ 0;
}

ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ T, ਕਿਸਮ_ਨਾਂ ST> ਪੂਰਨ
normL2_(ਅਚਲ T* src, ਅਚਲ uchar* ਨਕਾਬ, ST* ਨਤੀਜਾ_2, ਪੂਰਨ len, ਪੂਰਨ cn)
{
    ST ਨਤੀਜਾ = *ਨਤੀਜਾ_2;
    ਜੇ( !ਨਕਾਬ )
    {
        NormL2_SIMD<T, ST> op;
        ਨਤੀਜਾ += op(src, len*cn);
    }
    ਨਹੀਂਤਾਂ
    {
        MaskedNormL2_SIMD<T, ST> op;
        ਨਤੀਜਾ += op(src, ਨਕਾਬ, len, cn);
    }
    *ਨਤੀਜਾ_2 = ਨਤੀਜਾ;
    ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ
normInf_Bool(ਅਚਲ uchar* src, ਅਚਲ uchar* ਨਕਾਬ, ਪੂਰਨ* ਨਤੀਜਾ_2, ਪੂਰਨ len, ਪੂਰਨ cn)
{
    ਪੂਰਨ ਨਤੀਜਾ = *ਨਤੀਜਾ_2;
    ਜੇ( !ਨਕਾਬ )
    {
        ਲਈ ( ਪੂਰਨ i = 0; i < len*cn; i++ ) {
            ਨਤੀਜਾ = std::max(ਨਤੀਜਾ, (ਪੂਰਨ)(src[i] != 0));
            ਜੇ (ਨਤੀਜਾ != 0)
                ਰੋਕੋ;
        }
    }
    ਨਹੀਂਤਾਂ
    {
        ਲਈ( ਪੂਰਨ i = 0; i < len; i++, src += cn )
            ਜੇ( ਨਕਾਬ[i] )
            {
                ਲਈ( ਪੂਰਨ k = 0; k < cn; k++ )
                    ਨਤੀਜਾ = std::max(ਨਤੀਜਾ, (ਪੂਰਨ)(src[k] != 0));
                ਜੇ (ਨਤੀਜਾ != 0)
                    ਰੋਕੋ;
            }
    }
    *ਨਤੀਜਾ_2 = ਨਤੀਜਾ;
    ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ
normL1_Bool(ਅਚਲ uchar* src, ਅਚਲ uchar* ਨਕਾਬ, ਪੂਰਨ* ਨਤੀਜਾ_2, ਪੂਰਨ len, ਪੂਰਨ cn)
{
    ਪੂਰਨ ਨਤੀਜਾ = *ਨਤੀਜਾ_2;
    ਜੇ( !ਨਕਾਬ )
    {
        ਲਈ ( ਪੂਰਨ i = 0; i < len*cn; i++ )
            ਨਤੀਜਾ += (ਪੂਰਨ)(src[i] != 0);
    }
    ਨਹੀਂਤਾਂ
    {
        ਲਈ( ਪੂਰਨ i = 0; i < len; i++, src += cn )
            ਜੇ( ਨਕਾਬ[i] )
            {
                ਲਈ( ਪੂਰਨ k = 0; k < cn; k++ )
                    ਨਤੀਜਾ += (ਪੂਰਨ)(src[k] != 0);
            }
    }
    *ਨਤੀਜਾ_2 = ਨਤੀਜਾ;
    ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ
normL2_Bool(ਅਚਲ uchar* src, ਅਚਲ uchar* ਨਕਾਬ, ਪੂਰਨ* ਨਤੀਜਾ_2, ਪੂਰਨ len, ਪੂਰਨ cn)
{
    ਵਾਪਸ normL1_Bool(src, ਨਕਾਬ, ਨਤੀਜਾ_2, len, cn);
}

// ====================================================================
// Masked norm-diff kernels.
// Scalar base templates (safe for every depth) plus SIMD cn==1
// specializations for the concrete (T, ST) pairs dispatched by
// CV_DEF_NORM_ALL. The masked normDiff*_ functions below select them.
// ====================================================================
ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T, ਕਿਸਮ_ਨਾਂ ST>
ਬਣਤਰ MaskedNormDiffInf_SIMD {
    ਸਤਰਅੰਦਰ ST ਸੰਚਾਲਕ()(ਅਚਲ T* s1, ਅਚਲ T* s2, ਅਚਲ uchar* ਨਕਾਬ, ਪੂਰਨ len, ਪੂਰਨ cn) ਅਚਲ {
        ST ਨਤੀਜਾ = 0;
        ਲਈ (ਪੂਰਨ i = 0; i < len; i++) ਜੇ (ਨਕਾਬ[i]) {
            ਅਚਲ T* e1 = s1 + i*cn; ਅਚਲ T* e2 = s2 + i*cn;
            ਲਈ (ਪੂਰਨ k = 0; k < cn; k++)
                ਨਤੀਜਾ = std::max(ਨਤੀਜਾ, (ST)cv_absdiff(e1[k], e2[k]));
        }
        ਵਾਪਸ ਨਤੀਜਾ;
    }
};
ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T, ਕਿਸਮ_ਨਾਂ ST>
ਬਣਤਰ MaskedNormDiffL1_SIMD {
    ਸਤਰਅੰਦਰ ST ਸੰਚਾਲਕ()(ਅਚਲ T* s1, ਅਚਲ T* s2, ਅਚਲ uchar* ਨਕਾਬ, ਪੂਰਨ len, ਪੂਰਨ cn) ਅਚਲ {
        ST ਨਤੀਜਾ = 0;
        ਲਈ (ਪੂਰਨ i = 0; i < len; i++) ਜੇ (ਨਕਾਬ[i]) {
            ਅਚਲ T* e1 = s1 + i*cn; ਅਚਲ T* e2 = s2 + i*cn;
            ਲਈ (ਪੂਰਨ k = 0; k < cn; k++)
                ਨਤੀਜਾ += (ST)cv_absdiff(e1[k], e2[k]);
        }
        ਵਾਪਸ ਨਤੀਜਾ;
    }
};
ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T, ਕਿਸਮ_ਨਾਂ ST>
ਬਣਤਰ MaskedNormDiffL2_SIMD {
    ਸਤਰਅੰਦਰ ST ਸੰਚਾਲਕ()(ਅਚਲ T* s1, ਅਚਲ T* s2, ਅਚਲ uchar* ਨਕਾਬ, ਪੂਰਨ len, ਪੂਰਨ cn) ਅਚਲ {
        ST ਨਤੀਜਾ = 0;
        ਲਈ (ਪੂਰਨ i = 0; i < len; i++) ਜੇ (ਨਕਾਬ[i]) {
            ਅਚਲ T* e1 = s1 + i*cn; ਅਚਲ T* e2 = s2 + i*cn;
            ਲਈ (ਪੂਰਨ k = 0; k < cn; k++) { ST v = (ST)e1[k] - (ST)e2[k]; ਨਤੀਜਾ += v*v; }
        }
        ਵਾਪਸ ਨਤੀਜਾ;
    }
};

#ਜੇ (CV_SIMD || CV_SIMD_SCALABLE)
// AND a per-lane diff vector with the (expanded) mask>0 predicate.
ਸਤਰਅੰਦਰ v_uint8  v_diffmask(ਅਚਲ v_uint8&  d, ਅਚਲ uchar* m) { ਵਾਪਸ v_and(d, v_gt(vx_ਲੱਦੋ(m),          vx_setzero_u8())); }
ਸਤਰਅੰਦਰ v_uint16 v_diffmask(ਅਚਲ v_uint16& d, ਅਚਲ uchar* m) { ਵਾਪਸ v_and(d, v_gt(vx_ਲੱਦੋ_expand(m),   vx_setzero_u16())); }
ਸਤਰਅੰਦਰ v_uint32 v_diffmask(ਅਚਲ v_uint32& d, ਅਚਲ uchar* m) { ਵਾਪਸ v_and(d, v_gt(vx_ਲੱਦੋ_expand_q(m), vx_setzero_u32())); }
ਸਤਰਅੰਦਰ v_float32 v_diffmask(ਅਚਲ v_float32& d, ਅਚਲ uchar* m) {
    v_uint32 cm = v_gt(vx_ਲੱਦੋ_expand_q(m), vx_setzero_u32());
    ਵਾਪਸ v_reinterpret_as_f32(v_and(v_reinterpret_as_u32(d), cm));
}

// Shared scalar cn>1 fallbacks (match the base templates' cv_absdiff semantics).
ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ T, ਕਿਸਮ_ਨਾਂ RT>
ਸਥਿਰ ਸਤਰਅੰਦਰ RT masked_Norm_Diff_Inf_ਪੂਛ(ਅਚਲ T* s1, ਅਚਲ T* s2, ਅਚਲ uchar* ਨਕਾਬ, ਪੂਰਨ len, ਪੂਰਨ cn, RT ਨਤੀਜਾ) {
    ਲਈ (ਪੂਰਨ i = 0; i < len; i++) ਜੇ (ਨਕਾਬ[i]) {
        ਅਚਲ T* e1 = s1 + i*cn; ਅਚਲ T* e2 = s2 + i*cn;
        ਲਈ (ਪੂਰਨ k = 0; k < cn; k++) ਨਤੀਜਾ = std::max(ਨਤੀਜਾ, (RT)cv_absdiff(e1[k], e2[k]));
    }
    ਵਾਪਸ ਨਤੀਜਾ;
}
ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ T, ਕਿਸਮ_ਨਾਂ RT>
ਸਥਿਰ ਸਤਰਅੰਦਰ RT masked_Norm_Diff_L1_ਪੂਛ(ਅਚਲ T* s1, ਅਚਲ T* s2, ਅਚਲ uchar* ਨਕਾਬ, ਪੂਰਨ len, ਪੂਰਨ cn, RT ਨਤੀਜਾ) {
    ਲਈ (ਪੂਰਨ i = 0; i < len; i++) ਜੇ (ਨਕਾਬ[i]) {
        ਅਚਲ T* e1 = s1 + i*cn; ਅਚਲ T* e2 = s2 + i*cn;
        ਲਈ (ਪੂਰਨ k = 0; k < cn; k++) ਨਤੀਜਾ += (RT)cv_absdiff(e1[k], e2[k]);
    }
    ਵਾਪਸ ਨਤੀਜਾ;
}
ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ T, ਕਿਸਮ_ਨਾਂ RT>
ਸਥਿਰ ਸਤਰਅੰਦਰ RT masked_Norm_Diff_L2_ਪੂਛ(ਅਚਲ T* s1, ਅਚਲ T* s2, ਅਚਲ uchar* ਨਕਾਬ, ਪੂਰਨ len, ਪੂਰਨ cn, RT ਨਤੀਜਾ) {
    ਲਈ (ਪੂਰਨ i = 0; i < len; i++) ਜੇ (ਨਕਾਬ[i]) {
        ਅਚਲ T* e1 = s1 + i*cn; ਅਚਲ T* e2 = s2 + i*cn;
        ਲਈ (ਪੂਰਨ k = 0; k < cn; k++) { RT v = (RT)e1[k] - (RT)e2[k]; ਨਤੀਜਾ += v*v; }
    }
    ਵਾਪਸ ਨਤੀਜਾ;
}

// Inf (cn==1): reduce_max of masked |a-b|. Shared across SIMD-capable depths.
ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ T, ਕਿਸਮ_ਨਾਂ ST>
ਸਥਿਰ ਸਤਰਅੰਦਰ ST maskedNormDiffInfCn1(ਅਚਲ T* s1, ਅਚਲ T* s2, ਅਚਲ uchar* ਨਕਾਬ, ਪੂਰਨ len) {
    ਅਚਲ ਪੂਰਨ vstep = VTraits<decltype(vx_ਲੱਦੋ(s1))>::vlanes();
    ST ਨਤੀਜਾ = 0;
    ਪੂਰਨ i = 0;
    ਜੇ (len >= vstep) {
        ਸਵੈਚਾਲਿਤ acc = v_diffmask(v_absdiff(vx_ਲੱਦੋ(s1), vx_ਲੱਦੋ(s2)), ਨਕਾਬ);
        ਲਈ (i = vstep; i <= len - vstep; i += vstep)
            acc = v_max(acc, v_diffmask(v_absdiff(vx_ਲੱਦੋ(s1 + i), vx_ਲੱਦੋ(s2 + i)), ਨਕਾਬ + i));
        ਨਤੀਜਾ = (ST)v_reduce_max(acc);
    }
    ਲਈ (; i < len; i++) ਜੇ (ਨਕਾਬ[i]) ਨਤੀਜਾ = std::max(ਨਤੀਜਾ, (ST)cv_absdiff(s1[i], s2[i]));
    ਵਾਪਸ ਨਤੀਜਾ;
}

// Shared 8-bit cn==1 masked L1/L2 kernels (uchar/schar): v_absdiff yields v_uint8 for both.
ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ T>
ਸਥਿਰ ਸਤਰਅੰਦਰ ਪੂਰਨ maskedNormDiffL1_8(ਅਚਲ T* s1, ਅਚਲ T* s2, ਅਚਲ uchar* ਨਕਾਬ, ਪੂਰਨ len) {
    ਪੂਰਨ i = 0; ਅਚਲ ਪੂਰਨ vstep = VTraits<v_uint8>::vlanes();
    ਅਚਲ v_uint8 one = vx_setall_u8(1);
    v_uint32 acc = vx_setzero_u32();
    ਲਈ (; i <= len - vstep; i += vstep) {
        v_uint8 m  = v_gt(vx_ਲੱਦੋ(ਨਕਾਬ + i), vx_setzero_u8());
        v_uint8 ad = v_and(v_absdiff(vx_ਲੱਦੋ(s1 + i), vx_ਲੱਦੋ(s2 + i)), m);
        acc = v_dotprod_expand_fast(ad, one, acc);
    }
    ਪੂਰਨ ਨਤੀਜਾ = (ਪੂਰਨ)v_reduce_ਜੋੜ(acc);
    ਲਈ (; i < len; i++) ਜੇ (ਨਕਾਬ[i]) ਨਤੀਜਾ += (ਪੂਰਨ)cv_absdiff(s1[i], s2[i]);
    ਵਾਪਸ ਨਤੀਜਾ;
}
ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ T>
ਸਥਿਰ ਸਤਰਅੰਦਰ ਪੂਰਨ maskedNormDiffL2_8(ਅਚਲ T* s1, ਅਚਲ T* s2, ਅਚਲ uchar* ਨਕਾਬ, ਪੂਰਨ len) {
    ਪੂਰਨ i = 0; ਅਚਲ ਪੂਰਨ vstep = VTraits<v_uint8>::vlanes();
    v_uint32 acc = vx_setzero_u32();
    ਲਈ (; i <= len - vstep; i += vstep) {
        v_uint8 m  = v_gt(vx_ਲੱਦੋ(ਨਕਾਬ + i), vx_setzero_u8());
        v_uint8 ad = v_and(v_absdiff(vx_ਲੱਦੋ(s1 + i), vx_ਲੱਦੋ(s2 + i)), m);
        acc = v_dotprod_expand_fast(ad, ad, acc);
    }
    ਪੂਰਨ ਨਤੀਜਾ = (ਪੂਰਨ)v_reduce_ਜੋੜ(acc);
    ਲਈ (; i < len; i++) ਜੇ (ਨਕਾਬ[i]) { ਪੂਰਨ v = (ਪੂਰਨ)s1[i] - (ਪੂਰਨ)s2[i]; ਨਤੀਜਾ += v*v; }
    ਵਾਪਸ ਨਤੀਜਾ;
}

// Shared 16-bit cn==1 masked L1 kernel (ushort/short): v_absdiff yields v_uint16 for both.
ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ T>
ਸਥਿਰ ਸਤਰਅੰਦਰ ਪੂਰਨ maskedNormDiffL1_16(ਅਚਲ T* s1, ਅਚਲ T* s2, ਅਚਲ uchar* ਨਕਾਬ, ਪੂਰਨ len) {
    ਪੂਰਨ i = 0; ਅਚਲ ਪੂਰਨ vstep = VTraits<v_uint16>::vlanes();
    v_uint32 acc = vx_setzero_u32();
    ਲਈ (; i <= len - vstep; i += vstep) {
        v_uint16 m  = v_gt(vx_ਲੱਦੋ_expand(ਨਕਾਬ + i), vx_setzero_u16());
        v_uint16 ad = v_and(v_absdiff(vx_ਲੱਦੋ(s1 + i), vx_ਲੱਦੋ(s2 + i)), m);
        v_uint32 lo, hi; v_expand(ad, lo, hi);
        acc = v_ਜੋੜੋ(acc, v_ਜੋੜੋ(lo, hi));
    }
    ਪੂਰਨ ਨਤੀਜਾ = (ਪੂਰਨ)v_reduce_ਜੋੜ(acc);
    ਲਈ (; i < len; i++) ਜੇ (ਨਕਾਬ[i]) ਨਤੀਜਾ += (ਪੂਰਨ)cv_absdiff(s1[i], s2[i]);
    ਵਾਪਸ ਨਤੀਜਾ;
}

ਸਾਂਚਾ<> ਬਣਤਰ MaskedNormDiffInf_SIMD<uchar, ਪੂਰਨ> {
    ਪੂਰਨ ਸੰਚਾਲਕ()(ਅਚਲ uchar* s1, ਅਚਲ uchar* s2, ਅਚਲ uchar* ਨਕਾਬ, ਪੂਰਨ len, ਪੂਰਨ cn) ਅਚਲ {
        ਜੇ (cn == 1) ਵਾਪਸ maskedNormDiffInfCn1<uchar, ਪੂਰਨ>(s1, s2, ਨਕਾਬ, len);
        ਜੇ (cn == 4 && len >= VTraits<v_uint8>::vlanes()) {
            ਅਚਲ ਪੂਰਨ vstep = VTraits<v_uint8>::vlanes();
            v_uint8 acc = vx_setzero_u8();
            ਪੂਰਨ i = 0;
            ਲਈ (;;) {
                ਜੇ (i > len - vstep) i = len - vstep; // back-step (max is idempotent)
                v_uint8 a0,a1,a2,a3,b0,b1,b2,b3;
                v_ਲੱਦੋ_deinterleave(s1 + i*4, a0,a1,a2,a3);
                v_ਲੱਦੋ_deinterleave(s2 + i*4, b0,b1,b2,b3);
                v_uint8 ad = v_max(v_max(v_absdiff(a0,b0), v_absdiff(a1,b1)),
                                   v_max(v_absdiff(a2,b2), v_absdiff(a3,b3)));
                v_uint8 m = v_gt(vx_ਲੱਦੋ(ਨਕਾਬ + i), vx_setzero_u8());
                acc = v_max(acc, v_and(ad, m));
                ਜੇ (i >= len - vstep) ਰੋਕੋ;
                i += vstep;
            }
            ਵਾਪਸ (ਪੂਰਨ)v_reduce_max(acc);
        }
        ਵਾਪਸ masked_Norm_Diff_Inf_ਪੂਛ<uchar, ਪੂਰਨ>(s1, s2, ਨਕਾਬ, len, cn, 0);
    }
};
ਸਾਂਚਾ<> ਬਣਤਰ MaskedNormDiffInf_SIMD<schar, ਪੂਰਨ> {
    ਪੂਰਨ ਸੰਚਾਲਕ()(ਅਚਲ schar* s1, ਅਚਲ schar* s2, ਅਚਲ uchar* ਨਕਾਬ, ਪੂਰਨ len, ਪੂਰਨ cn) ਅਚਲ {
        ਜੇ (cn == 1) ਵਾਪਸ maskedNormDiffInfCn1<schar, ਪੂਰਨ>(s1, s2, ਨਕਾਬ, len);
        ਵਾਪਸ masked_Norm_Diff_Inf_ਪੂਛ<schar, ਪੂਰਨ>(s1, s2, ਨਕਾਬ, len, cn, 0);
    }
};
ਸਾਂਚਾ<> ਬਣਤਰ MaskedNormDiffInf_SIMD<ushort, ਪੂਰਨ> {
    ਪੂਰਨ ਸੰਚਾਲਕ()(ਅਚਲ ushort* s1, ਅਚਲ ushort* s2, ਅਚਲ uchar* ਨਕਾਬ, ਪੂਰਨ len, ਪੂਰਨ cn) ਅਚਲ {
        ਜੇ (cn == 1) ਵਾਪਸ maskedNormDiffInfCn1<ushort, ਪੂਰਨ>(s1, s2, ਨਕਾਬ, len);
        ਵਾਪਸ masked_Norm_Diff_Inf_ਪੂਛ<ushort, ਪੂਰਨ>(s1, s2, ਨਕਾਬ, len, cn, 0);
    }
};
ਸਾਂਚਾ<> ਬਣਤਰ MaskedNormDiffInf_SIMD<ਛੋਟਾ, ਪੂਰਨ> {
    ਪੂਰਨ ਸੰਚਾਲਕ()(ਅਚਲ ਛੋਟਾ* s1, ਅਚਲ ਛੋਟਾ* s2, ਅਚਲ uchar* ਨਕਾਬ, ਪੂਰਨ len, ਪੂਰਨ cn) ਅਚਲ {
        ਜੇ (cn == 1) ਵਾਪਸ maskedNormDiffInfCn1<ਛੋਟਾ, ਪੂਰਨ>(s1, s2, ਨਕਾਬ, len);
        ਵਾਪਸ masked_Norm_Diff_Inf_ਪੂਛ<ਛੋਟਾ, ਪੂਰਨ>(s1, s2, ਨਕਾਬ, len, cn, 0);
    }
};
ਸਾਂਚਾ<> ਬਣਤਰ MaskedNormDiffInf_SIMD<ਪੂਰਨ, ਬਿਨਾਨਿਸ਼ਾਨੀ> {
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਸੰਚਾਲਕ()(ਅਚਲ ਪੂਰਨ* s1, ਅਚਲ ਪੂਰਨ* s2, ਅਚਲ uchar* ਨਕਾਬ, ਪੂਰਨ len, ਪੂਰਨ cn) ਅਚਲ {
        ਜੇ (cn == 1) ਵਾਪਸ maskedNormDiffInfCn1<ਪੂਰਨ, ਬਿਨਾਨਿਸ਼ਾਨੀ>(s1, s2, ਨਕਾਬ, len);
        ਵਾਪਸ masked_Norm_Diff_Inf_ਪੂਛ<ਪੂਰਨ, ਬਿਨਾਨਿਸ਼ਾਨੀ>(s1, s2, ਨਕਾਬ, len, cn, 0u);
    }
};
ਸਾਂਚਾ<> ਬਣਤਰ MaskedNormDiffInf_SIMD<ਦਸ਼ਮਲਵ, ਦਸ਼ਮਲਵ> {
    ਦਸ਼ਮਲਵ ਸੰਚਾਲਕ()(ਅਚਲ ਦਸ਼ਮਲਵ* s1, ਅਚਲ ਦਸ਼ਮਲਵ* s2, ਅਚਲ uchar* ਨਕਾਬ, ਪੂਰਨ len, ਪੂਰਨ cn) ਅਚਲ {
        ਜੇ (cn == 1) ਵਾਪਸ maskedNormDiffInfCn1<ਦਸ਼ਮਲਵ, ਦਸ਼ਮਲਵ>(s1, s2, ਨਕਾਬ, len);
        ਵਾਪਸ masked_Norm_Diff_Inf_ਪੂਛ<ਦਸ਼ਮਲਵ, ਦਸ਼ਮਲਵ>(s1, s2, ਨਕਾਬ, len, cn, 0.0f);
    }
};

ਸਾਂਚਾ<> ਬਣਤਰ MaskedNormDiffL1_SIMD<uchar, ਪੂਰਨ> {
    ਪੂਰਨ ਸੰਚਾਲਕ()(ਅਚਲ uchar* s1, ਅਚਲ uchar* s2, ਅਚਲ uchar* ਨਕਾਬ, ਪੂਰਨ len, ਪੂਰਨ cn) ਅਚਲ {
        ਜੇ (cn == 1) ਵਾਪਸ maskedNormDiffL1_8(s1, s2, ਨਕਾਬ, len);
        ਜੇ (cn == 4) {
            ਪੂਰਨ i = 0; ਅਚਲ ਪੂਰਨ vstep = VTraits<v_uint8>::vlanes();
            ਅਚਲ v_uint8 one = vx_setall_u8(1);
            v_uint32 acc = vx_setzero_u32();
            ਲਈ (; i <= len - vstep; i += vstep) {
                v_uint8 a0,a1,a2,a3,b0,b1,b2,b3;
                v_ਲੱਦੋ_deinterleave(s1 + i*4, a0,a1,a2,a3);
                v_ਲੱਦੋ_deinterleave(s2 + i*4, b0,b1,b2,b3);
                v_uint8 m = v_gt(vx_ਲੱਦੋ(ਨਕਾਬ + i), vx_setzero_u8());
                acc = v_dotprod_expand_fast(v_and(v_absdiff(a0,b0), m), one, acc);
                acc = v_dotprod_expand_fast(v_and(v_absdiff(a1,b1), m), one, acc);
                acc = v_dotprod_expand_fast(v_and(v_absdiff(a2,b2), m), one, acc);
                acc = v_dotprod_expand_fast(v_and(v_absdiff(a3,b3), m), one, acc);
            }
            ਪੂਰਨ ਨਤੀਜਾ = (ਪੂਰਨ)v_reduce_ਜੋੜ(acc);
            ਲਈ (; i < len; i++) ਜੇ (ਨਕਾਬ[i]) {
                ਅਚਲ uchar* e1 = s1 + i*4; ਅਚਲ uchar* e2 = s2 + i*4;
                ਲਈ (ਪੂਰਨ k = 0; k < 4; k++) ਨਤੀਜਾ += (ਪੂਰਨ)cv_absdiff(e1[k], e2[k]);
            }
            ਵਾਪਸ ਨਤੀਜਾ;
        }
        ਵਾਪਸ masked_Norm_Diff_L1_ਪੂਛ<uchar, ਪੂਰਨ>(s1, s2, ਨਕਾਬ, len, cn, 0);
    }
};
ਸਾਂਚਾ<> ਬਣਤਰ MaskedNormDiffL1_SIMD<schar, ਪੂਰਨ> {
    ਪੂਰਨ ਸੰਚਾਲਕ()(ਅਚਲ schar* s1, ਅਚਲ schar* s2, ਅਚਲ uchar* ਨਕਾਬ, ਪੂਰਨ len, ਪੂਰਨ cn) ਅਚਲ {
        ਜੇ (cn == 1) ਵਾਪਸ maskedNormDiffL1_8(s1, s2, ਨਕਾਬ, len);
        ਵਾਪਸ masked_Norm_Diff_L1_ਪੂਛ<schar, ਪੂਰਨ>(s1, s2, ਨਕਾਬ, len, cn, 0);
    }
};
ਸਾਂਚਾ<> ਬਣਤਰ MaskedNormDiffL1_SIMD<ushort, ਪੂਰਨ> {
    ਪੂਰਨ ਸੰਚਾਲਕ()(ਅਚਲ ushort* s1, ਅਚਲ ushort* s2, ਅਚਲ uchar* ਨਕਾਬ, ਪੂਰਨ len, ਪੂਰਨ cn) ਅਚਲ {
        ਜੇ (cn == 1) ਵਾਪਸ maskedNormDiffL1_16(s1, s2, ਨਕਾਬ, len);
        ਵਾਪਸ masked_Norm_Diff_L1_ਪੂਛ<ushort, ਪੂਰਨ>(s1, s2, ਨਕਾਬ, len, cn, 0);
    }
};
ਸਾਂਚਾ<> ਬਣਤਰ MaskedNormDiffL1_SIMD<ਛੋਟਾ, ਪੂਰਨ> {
    ਪੂਰਨ ਸੰਚਾਲਕ()(ਅਚਲ ਛੋਟਾ* s1, ਅਚਲ ਛੋਟਾ* s2, ਅਚਲ uchar* ਨਕਾਬ, ਪੂਰਨ len, ਪੂਰਨ cn) ਅਚਲ {
        ਜੇ (cn == 1) ਵਾਪਸ maskedNormDiffL1_16(s1, s2, ਨਕਾਬ, len);
        ਵਾਪਸ masked_Norm_Diff_L1_ਪੂਛ<ਛੋਟਾ, ਪੂਰਨ>(s1, s2, ਨਕਾਬ, len, cn, 0);
    }
};

ਸਾਂਚਾ<> ਬਣਤਰ MaskedNormDiffL2_SIMD<uchar, ਪੂਰਨ> {
    ਪੂਰਨ ਸੰਚਾਲਕ()(ਅਚਲ uchar* s1, ਅਚਲ uchar* s2, ਅਚਲ uchar* ਨਕਾਬ, ਪੂਰਨ len, ਪੂਰਨ cn) ਅਚਲ {
        ਜੇ (cn == 1) ਵਾਪਸ maskedNormDiffL2_8(s1, s2, ਨਕਾਬ, len);
        ਜੇ (cn == 4) {
            ਪੂਰਨ i = 0; ਅਚਲ ਪੂਰਨ vstep = VTraits<v_uint8>::vlanes();
            v_uint32 acc = vx_setzero_u32();
            ਲਈ (; i <= len - vstep; i += vstep) {
                v_uint8 a0,a1,a2,a3,b0,b1,b2,b3;
                v_ਲੱਦੋ_deinterleave(s1 + i*4, a0,a1,a2,a3);
                v_ਲੱਦੋ_deinterleave(s2 + i*4, b0,b1,b2,b3);
                v_uint8 m = v_gt(vx_ਲੱਦੋ(ਨਕਾਬ + i), vx_setzero_u8());
                v_uint8 d0 = v_and(v_absdiff(a0,b0), m), d1 = v_and(v_absdiff(a1,b1), m);
                v_uint8 d2 = v_and(v_absdiff(a2,b2), m), d3 = v_and(v_absdiff(a3,b3), m);
                acc = v_dotprod_expand_fast(d0, d0, acc);
                acc = v_dotprod_expand_fast(d1, d1, acc);
                acc = v_dotprod_expand_fast(d2, d2, acc);
                acc = v_dotprod_expand_fast(d3, d3, acc);
            }
            ਪੂਰਨ ਨਤੀਜਾ = (ਪੂਰਨ)v_reduce_ਜੋੜ(acc);
            ਲਈ (; i < len; i++) ਜੇ (ਨਕਾਬ[i]) {
                ਅਚਲ uchar* e1 = s1 + i*4; ਅਚਲ uchar* e2 = s2 + i*4;
                ਲਈ (ਪੂਰਨ k = 0; k < 4; k++) { ਪੂਰਨ v = (ਪੂਰਨ)e1[k] - (ਪੂਰਨ)e2[k]; ਨਤੀਜਾ += v*v; }
            }
            ਵਾਪਸ ਨਤੀਜਾ;
        }
        ਵਾਪਸ masked_Norm_Diff_L2_ਪੂਛ<uchar, ਪੂਰਨ>(s1, s2, ਨਕਾਬ, len, cn, 0);
    }
};
ਸਾਂਚਾ<> ਬਣਤਰ MaskedNormDiffL2_SIMD<schar, ਪੂਰਨ> {
    ਪੂਰਨ ਸੰਚਾਲਕ()(ਅਚਲ schar* s1, ਅਚਲ schar* s2, ਅਚਲ uchar* ਨਕਾਬ, ਪੂਰਨ len, ਪੂਰਨ cn) ਅਚਲ {
        ਜੇ (cn == 1) ਵਾਪਸ maskedNormDiffL2_8(s1, s2, ਨਕਾਬ, len);
        ਵਾਪਸ masked_Norm_Diff_L2_ਪੂਛ<schar, ਪੂਰਨ>(s1, s2, ਨਕਾਬ, len, cn, 0);
    }
};

#ਜੇ CV_SIMD_64F
// Depths whose L1/L2 accumulate in double.
ਸਾਂਚਾ<> ਬਣਤਰ MaskedNormDiffL1_SIMD<ਪੂਰਨ, ਦੁੱਗਣਾ> {
    ਦੁੱਗਣਾ ਸੰਚਾਲਕ()(ਅਚਲ ਪੂਰਨ* s1, ਅਚਲ ਪੂਰਨ* s2, ਅਚਲ uchar* ਨਕਾਬ, ਪੂਰਨ len, ਪੂਰਨ cn) ਅਚਲ {
        ਜੇ (cn != 1) ਵਾਪਸ masked_Norm_Diff_L1_ਪੂਛ<ਪੂਰਨ, ਦੁੱਗਣਾ>(s1, s2, ਨਕਾਬ, len, cn, 0.0);
        ਪੂਰਨ i = 0; ਅਚਲ ਪੂਰਨ vstep = VTraits<v_int32>::vlanes();
        v_float64 acc = vx_setzero_f64();
        ਲਈ (; i <= len - vstep; i += vstep) {
            v_uint32 m  = v_gt(vx_ਲੱਦੋ_expand_q(ਨਕਾਬ + i), vx_setzero_u32());
            v_uint32 ad = v_and(v_absdiff(vx_ਲੱਦੋ(s1 + i), vx_ਲੱਦੋ(s2 + i)), m);
            v_uint64 e0, e1; v_expand(ad, e0, e1);
            acc = v_ਜੋੜੋ(acc, v_cvt_f64(v_reinterpret_as_s64(e0)));
            acc = v_ਜੋੜੋ(acc, v_cvt_f64(v_reinterpret_as_s64(e1)));
        }
        ਦੁੱਗਣਾ ਨਤੀਜਾ = v_reduce_ਜੋੜ(acc);
        ਲਈ (; i < len; i++) ਜੇ (ਨਕਾਬ[i]) ਨਤੀਜਾ += (ਦੁੱਗਣਾ)cv_absdiff(s1[i], s2[i]);
        ਵਾਪਸ ਨਤੀਜਾ;
    }
};
ਸਾਂਚਾ<> ਬਣਤਰ MaskedNormDiffL1_SIMD<ਦਸ਼ਮਲਵ, ਦੁੱਗਣਾ> {
    ਦੁੱਗਣਾ ਸੰਚਾਲਕ()(ਅਚਲ ਦਸ਼ਮਲਵ* s1, ਅਚਲ ਦਸ਼ਮਲਵ* s2, ਅਚਲ uchar* ਨਕਾਬ, ਪੂਰਨ len, ਪੂਰਨ cn) ਅਚਲ {
        ਜੇ (cn != 1) ਵਾਪਸ masked_Norm_Diff_L1_ਪੂਛ<ਦਸ਼ਮਲਵ, ਦੁੱਗਣਾ>(s1, s2, ਨਕਾਬ, len, cn, 0.0);
        ਪੂਰਨ i = 0; ਅਚਲ ਪੂਰਨ vstep = VTraits<v_float32>::vlanes();
        v_float64 acc0 = vx_setzero_f64(), acc1 = vx_setzero_f64();
        ਲਈ (; i <= len - vstep; i += vstep) {
            v_float32 m  = v_reinterpret_as_f32(v_gt(vx_ਲੱਦੋ_expand_q(ਨਕਾਬ + i), vx_setzero_u32()));
            v_float32 ad = v_and(v_absdiff(vx_ਲੱਦੋ(s1 + i), vx_ਲੱਦੋ(s2 + i)), m);
            acc0 = v_ਜੋੜੋ(acc0, v_cvt_f64(ad));
            acc1 = v_ਜੋੜੋ(acc1, v_cvt_f64_ਉੱਚਾ(ad));
        }
        ਦੁੱਗਣਾ ਨਤੀਜਾ = v_reduce_ਜੋੜ(v_ਜੋੜੋ(acc0, acc1));
        ਲਈ (; i < len; i++) ਜੇ (ਨਕਾਬ[i]) ਨਤੀਜਾ += (ਦੁੱਗਣਾ)cv_absdiff(s1[i], s2[i]);
        ਵਾਪਸ ਨਤੀਜਾ;
    }
};

ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ T>
ਸਥਿਰ ਸਤਰਅੰਦਰ ਦੁੱਗਣਾ maskedNormDiffL2_16(ਅਚਲ T* s1, ਅਚਲ T* s2, ਅਚਲ uchar* ਨਕਾਬ, ਪੂਰਨ len, ਪੂਰਨ cn) {
    ਜੇ (cn != 1) ਵਾਪਸ masked_Norm_Diff_L2_ਪੂਛ<T, ਦੁੱਗਣਾ>(s1, s2, ਨਕਾਬ, len, cn, 0.0);
    ਪੂਰਨ i = 0; ਅਚਲ ਪੂਰਨ vstep = VTraits<v_uint16>::vlanes();
    v_float64 acc = vx_setzero_f64();
    ਲਈ (; i <= len - vstep; i += vstep) {
        v_uint16 m  = v_gt(vx_ਲੱਦੋ_expand(ਨਕਾਬ + i), vx_setzero_u16());
        v_uint16 ad = v_and(v_absdiff(vx_ਲੱਦੋ(s1 + i), vx_ਲੱਦੋ(s2 + i)), m);
        v_uint64 u = v_dotprod_expand_fast(ad, ad);
        acc = v_ਜੋੜੋ(acc, v_cvt_f64(v_reinterpret_as_s64(u)));
    }
    ਦੁੱਗਣਾ ਨਤੀਜਾ = v_reduce_ਜੋੜ(acc);
    ਲਈ (; i < len; i++) ਜੇ (ਨਕਾਬ[i]) { ਦੁੱਗਣਾ v = (ਦੁੱਗਣਾ)s1[i] - (ਦੁੱਗਣਾ)s2[i]; ਨਤੀਜਾ += v*v; }
    ਵਾਪਸ ਨਤੀਜਾ;
}
ਸਾਂਚਾ<> ਬਣਤਰ MaskedNormDiffL2_SIMD<ushort, ਦੁੱਗਣਾ> {
    ਦੁੱਗਣਾ ਸੰਚਾਲਕ()(ਅਚਲ ushort* s1, ਅਚਲ ushort* s2, ਅਚਲ uchar* ਨਕਾਬ, ਪੂਰਨ len, ਪੂਰਨ cn) ਅਚਲ
    { ਵਾਪਸ maskedNormDiffL2_16(s1, s2, ਨਕਾਬ, len, cn); } };
ਸਾਂਚਾ<> ਬਣਤਰ MaskedNormDiffL2_SIMD<ਛੋਟਾ, ਦੁੱਗਣਾ> {
    ਦੁੱਗਣਾ ਸੰਚਾਲਕ()(ਅਚਲ ਛੋਟਾ* s1, ਅਚਲ ਛੋਟਾ* s2, ਅਚਲ uchar* ਨਕਾਬ, ਪੂਰਨ len, ਪੂਰਨ cn) ਅਚਲ
    { ਵਾਪਸ maskedNormDiffL2_16(s1, s2, ਨਕਾਬ, len, cn); } };

ਸਾਂਚਾ<> ਬਣਤਰ MaskedNormDiffL2_SIMD<ਪੂਰਨ, ਦੁੱਗਣਾ> {
    ਦੁੱਗਣਾ ਸੰਚਾਲਕ()(ਅਚਲ ਪੂਰਨ* s1, ਅਚਲ ਪੂਰਨ* s2, ਅਚਲ uchar* ਨਕਾਬ, ਪੂਰਨ len, ਪੂਰਨ cn) ਅਚਲ {
        ਜੇ (cn != 1) ਵਾਪਸ masked_Norm_Diff_L2_ਪੂਛ<ਪੂਰਨ, ਦੁੱਗਣਾ>(s1, s2, ਨਕਾਬ, len, cn, 0.0);
        ਪੂਰਨ i = 0; ਅਚਲ ਪੂਰਨ vstep = VTraits<v_int32>::vlanes();
        v_float64 r0 = vx_setzero_f64(), r1 = vx_setzero_f64();
        ਲਈ (; i <= len - vstep; i += vstep) {
            v_uint32 m  = v_gt(vx_ਲੱਦੋ_expand_q(ਨਕਾਬ + i), vx_setzero_u32());
            v_uint32 ad = v_and(v_absdiff(vx_ਲੱਦੋ(s1 + i), vx_ਲੱਦੋ(s2 + i)), m);
            v_uint64 e0, e1; v_expand(ad, e0, e1);
            v_float64 f0 = v_cvt_f64(v_reinterpret_as_s64(e0)), f1 = v_cvt_f64(v_reinterpret_as_s64(e1));
            r0 = v_fma(f0, f0, r0); r1 = v_fma(f1, f1, r1);
        }
        ਦੁੱਗਣਾ ਨਤੀਜਾ = v_reduce_ਜੋੜ(v_ਜੋੜੋ(r0, r1));
        ਲਈ (; i < len; i++) ਜੇ (ਨਕਾਬ[i]) { ਦੁੱਗਣਾ v = (ਦੁੱਗਣਾ)s1[i] - (ਦੁੱਗਣਾ)s2[i]; ਨਤੀਜਾ += v*v; }
        ਵਾਪਸ ਨਤੀਜਾ;
    }
};
ਸਾਂਚਾ<> ਬਣਤਰ MaskedNormDiffL2_SIMD<ਦਸ਼ਮਲਵ, ਦੁੱਗਣਾ> {
    ਦੁੱਗਣਾ ਸੰਚਾਲਕ()(ਅਚਲ ਦਸ਼ਮਲਵ* s1, ਅਚਲ ਦਸ਼ਮਲਵ* s2, ਅਚਲ uchar* ਨਕਾਬ, ਪੂਰਨ len, ਪੂਰਨ cn) ਅਚਲ {
        ਜੇ (cn != 1) ਵਾਪਸ masked_Norm_Diff_L2_ਪੂਛ<ਦਸ਼ਮਲਵ, ਦੁੱਗਣਾ>(s1, s2, ਨਕਾਬ, len, cn, 0.0);
        ਪੂਰਨ i = 0; ਅਚਲ ਪੂਰਨ vstep = VTraits<v_float32>::vlanes();
        v_float64 r0 = vx_setzero_f64(), r1 = vx_setzero_f64();
        ਲਈ (; i <= len - vstep; i += vstep) {
            v_float32 m  = v_reinterpret_as_f32(v_gt(vx_ਲੱਦੋ_expand_q(ਨਕਾਬ + i), vx_setzero_u32()));
            v_float32 ad = v_and(v_absdiff(vx_ਲੱਦੋ(s1 + i), vx_ਲੱਦੋ(s2 + i)), m);
            v_float64 f0 = v_cvt_f64(ad), f1 = v_cvt_f64_ਉੱਚਾ(ad);
            r0 = v_fma(f0, f0, r0); r1 = v_fma(f1, f1, r1);
        }
        ਦੁੱਗਣਾ ਨਤੀਜਾ = v_reduce_ਜੋੜ(v_ਜੋੜੋ(r0, r1));
        ਲਈ (; i < len; i++) ਜੇ (ਨਕਾਬ[i]) { ਦੁੱਗਣਾ v = (ਦੁੱਗਣਾ)s1[i] - (ਦੁੱਗਣਾ)s2[i]; ਨਤੀਜਾ += v*v; }
        ਵਾਪਸ ਨਤੀਜਾ;
    }
};
ਸਾਂਚਾ<>
ਬਣਤਰ MaskedNormDiffInf_SIMD<ਦੁੱਗਣਾ, ਦੁੱਗਣਾ> {
    ਸਤਰਅੰਦਰ ਦੁੱਗਣਾ ਸੰਚਾਲਕ()(ਅਚਲ ਦੁੱਗਣਾ* s1, ਅਚਲ ਦੁੱਗਣਾ* s2, ਅਚਲ uchar* ਨਕਾਬ, ਪੂਰਨ len, ਪੂਰਨ cn) ਅਚਲ {
        ਦੁੱਗਣਾ ਨਤੀਜਾ = 0.0;
        ਪੂਰਨ i = 0;
#ਜੇ CV_SIMD_64F || CV_SIMD_SCALABLE_64F
        ਜੇ (cn == 1) {
            ਅਚਲ ਪੂਰਨ vstep = VTraits<v_float64>::vlanes();
            v_float64 acc0 = vx_setzero_f64(), acc1 = vx_setzero_f64();
            ਲਈ (; i <= len - 2*vstep; i += 2*vstep) {
                v_uint64 m0, m1;
                v_normmask_u64(ਨਕਾਬ + i, m0, m1);
                v_float64 d0 = v_absdiff(vx_ਲੱਦੋ(s1 + i), vx_ਲੱਦੋ(s2 + i));
                v_float64 d1 = v_absdiff(vx_ਲੱਦੋ(s1 + i + vstep), vx_ਲੱਦੋ(s2 + i + vstep));
                acc0 = v_max(acc0, v_reinterpret_as_f64(v_and(v_reinterpret_as_u64(d0), m0)));
                acc1 = v_max(acc1, v_reinterpret_as_f64(v_and(v_reinterpret_as_u64(d1), m1)));
            }
            ਦੁੱਗਣਾ t[VTraits<v_float64>::max_nlanes];
            vx_store(t, v_max(acc0, acc1));
            ਲਈ (ਪੂਰਨ k = 0; k < vstep; k++)
                ਨਤੀਜਾ = std::max(ਨਤੀਜਾ, t[k]);
        }
#ਜੇ_ਅੰਤ
        ਲਈ (; i < len; i++) ਜੇ (ਨਕਾਬ[i]) {
            ਅਚਲ ਦੁੱਗਣਾ* e1 = s1 + i*cn; ਅਚਲ ਦੁੱਗਣਾ* e2 = s2 + i*cn;
            ਲਈ (ਪੂਰਨ k = 0; k < cn; k++) ਨਤੀਜਾ = std::max(ਨਤੀਜਾ, std::ਨਿਰਪੇਖ(e1[k] - e2[k]));
        }
        ਵਾਪਸ ਨਤੀਜਾ;
    }
};

ਸਾਂਚਾ<>
ਬਣਤਰ MaskedNormDiffL1_SIMD<ਦੁੱਗਣਾ, ਦੁੱਗਣਾ> {
    ਦੁੱਗਣਾ ਸੰਚਾਲਕ()(ਅਚਲ ਦੁੱਗਣਾ* s1, ਅਚਲ ਦੁੱਗਣਾ* s2, ਅਚਲ uchar* ਨਕਾਬ, ਪੂਰਨ len, ਪੂਰਨ cn) ਅਚਲ {
        ਦੁੱਗਣਾ ਨਤੀਜਾ = 0.0;
        ਜੇ (cn == 1) {
            ਅਚਲ ਪੂਰਨ vstep = VTraits<v_float64>::vlanes();
            ਪੂਰਨ i = 0;
            v_float64 acc0 = vx_setzero_f64(), acc1 = vx_setzero_f64();
            ਲਈ (; i <= len - 2*vstep; i += 2*vstep) {
                v_uint64 m0, m1;
                v_normmask_u64(ਨਕਾਬ + i, m0, m1);
                v_float64 d0 = v_absdiff(vx_ਲੱਦੋ(s1 + i), vx_ਲੱਦੋ(s2 + i));
                v_float64 d1 = v_absdiff(vx_ਲੱਦੋ(s1 + i + vstep), vx_ਲੱਦੋ(s2 + i + vstep));
                acc0 = v_ਜੋੜੋ(acc0, v_reinterpret_as_f64(v_and(v_reinterpret_as_u64(d0), m0)));
                acc1 = v_ਜੋੜੋ(acc1, v_reinterpret_as_f64(v_and(v_reinterpret_as_u64(d1), m1)));
            }
            ਨਤੀਜਾ = v_reduce_ਜੋੜ(v_ਜੋੜੋ(acc0, acc1));
            ਲਈ (; i < len; i++) ਜੇ (ਨਕਾਬ[i]) ਨਤੀਜਾ += std::ਨਿਰਪੇਖ(s1[i] - s2[i]);
        } ਨਹੀਂਤਾਂ {
            ਨਤੀਜਾ = masked_Norm_Diff_L1_ਪੂਛ<ਦੁੱਗਣਾ, ਦੁੱਗਣਾ>(s1, s2, ਨਕਾਬ, len, cn, 0.0);
        }
        ਵਾਪਸ ਨਤੀਜਾ;
    }
};

ਸਾਂਚਾ<>
ਬਣਤਰ MaskedNormDiffL2_SIMD<ਦੁੱਗਣਾ, ਦੁੱਗਣਾ> {
    ਦੁੱਗਣਾ ਸੰਚਾਲਕ()(ਅਚਲ ਦੁੱਗਣਾ* s1, ਅਚਲ ਦੁੱਗਣਾ* s2, ਅਚਲ uchar* ਨਕਾਬ, ਪੂਰਨ len, ਪੂਰਨ cn) ਅਚਲ {
        ਦੁੱਗਣਾ ਨਤੀਜਾ = 0.0;
        ਜੇ (cn == 1) {
            ਅਚਲ ਪੂਰਨ vstep = VTraits<v_float64>::vlanes();
            ਪੂਰਨ i = 0;
            v_float64 acc0 = vx_setzero_f64(), acc1 = vx_setzero_f64();
            ਲਈ (; i <= len - 2*vstep; i += 2*vstep) {
                v_uint64 m0, m1;
                v_normmask_u64(ਨਕਾਬ + i, m0, m1);
                v_float64 d0 = v_sub(vx_ਲੱਦੋ(s1 + i), vx_ਲੱਦੋ(s2 + i));
                v_float64 d1 = v_sub(vx_ਲੱਦੋ(s1 + i + vstep), vx_ਲੱਦੋ(s2 + i + vstep));
                d0 = v_reinterpret_as_f64(v_and(v_reinterpret_as_u64(d0), m0));
                d1 = v_reinterpret_as_f64(v_and(v_reinterpret_as_u64(d1), m1));
                acc0 = v_fma(d0, d0, acc0);
                acc1 = v_fma(d1, d1, acc1);
            }
            ਨਤੀਜਾ = v_reduce_ਜੋੜ(v_ਜੋੜੋ(acc0, acc1));
            ਲਈ (; i < len; i++) ਜੇ (ਨਕਾਬ[i]) { ਦੁੱਗਣਾ v = s1[i] - s2[i]; ਨਤੀਜਾ += v*v; }
        } ਨਹੀਂਤਾਂ {
            ਨਤੀਜਾ = masked_Norm_Diff_L2_ਪੂਛ<ਦੁੱਗਣਾ, ਦੁੱਗਣਾ>(s1, s2, ਨਕਾਬ, len, cn, 0.0);
        }
        ਵਾਪਸ ਨਤੀਜਾ;
    }
};
#ਜੇ_ਅੰਤ // CV_SIMD_64F
#ਜੇ_ਅੰਤ // CV_SIMD

ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ T, ਕਿਸਮ_ਨਾਂ ST> ਪੂਰਨ
normDiffInf_(ਅਚਲ T* src1, ਅਚਲ T* src2, ਅਚਲ uchar* ਨਕਾਬ, ST* ਨਤੀਜਾ_2, ਪੂਰਨ len, ਪੂਰਨ cn) {
    ST ਨਤੀਜਾ = *ਨਤੀਜਾ_2;
    ਜੇ( !ਨਕਾਬ ) {
        NormDiffInf_SIMD<T, ST> op;
        ਨਤੀਜਾ = std::max(ਨਤੀਜਾ, op(src1, src2, len*cn));
    } ਨਹੀਂਤਾਂ {
        MaskedNormDiffInf_SIMD<T, ST> op;
        ਨਤੀਜਾ = std::max(ਨਤੀਜਾ, op(src1, src2, ਨਕਾਬ, len, cn));
    }
    *ਨਤੀਜਾ_2 = ਨਤੀਜਾ;
    ਵਾਪਸ 0;
}

ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ T, ਕਿਸਮ_ਨਾਂ ST> ਪੂਰਨ
normDiffL1_(ਅਚਲ T* src1, ਅਚਲ T* src2, ਅਚਲ uchar* ਨਕਾਬ, ST* ਨਤੀਜਾ_2, ਪੂਰਨ len, ਪੂਰਨ cn) {
    ST ਨਤੀਜਾ = *ਨਤੀਜਾ_2;
    ਜੇ( !ਨਕਾਬ ) {
        NormDiffL1_SIMD<T, ST> op;
        ਨਤੀਜਾ += op(src1, src2, len*cn);
    } ਨਹੀਂਤਾਂ {
        MaskedNormDiffL1_SIMD<T, ST> op;
        ਨਤੀਜਾ += op(src1, src2, ਨਕਾਬ, len, cn);
    }
    *ਨਤੀਜਾ_2 = ਨਤੀਜਾ;
    ਵਾਪਸ 0;
}

ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ T, ਕਿਸਮ_ਨਾਂ ST> ਪੂਰਨ
normDiffL2_(ਅਚਲ T* src1, ਅਚਲ T* src2, ਅਚਲ uchar* ਨਕਾਬ, ST* ਨਤੀਜਾ_2, ਪੂਰਨ len, ਪੂਰਨ cn) {
    ST ਨਤੀਜਾ = *ਨਤੀਜਾ_2;
    ਜੇ( !ਨਕਾਬ ) {
        NormDiffL2_SIMD<T, ST> op;
        ਨਤੀਜਾ += op(src1, src2, len*cn);
    } ਨਹੀਂਤਾਂ {
        MaskedNormDiffL2_SIMD<T, ST> op;
        ਨਤੀਜਾ += op(src1, src2, ਨਕਾਬ, len, cn);
    }
    *ਨਤੀਜਾ_2 = ਨਤੀਜਾ;
    ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ
normDiffInf_Bool(ਅਚਲ uchar* src1, ਅਚਲ uchar* src2, ਅਚਲ uchar* ਨਕਾਬ, ਪੂਰਨ* ਨਤੀਜਾ_2, ਪੂਰਨ len, ਪੂਰਨ cn)
{
    ਪੂਰਨ ਨਤੀਜਾ = *ਨਤੀਜਾ_2;
    ਜੇ( !ਨਕਾਬ )
    {
        ਲਈ( ਪੂਰਨ i = 0; i < len*cn; i++ ) {
            ਨਤੀਜਾ = std::max(ਨਤੀਜਾ, (ਪੂਰਨ)((src1[i] != 0) != (src2[i] != 0)));
            ਜੇ (ਨਤੀਜਾ != 0)
                ਰੋਕੋ;
        }
    }
    ਨਹੀਂਤਾਂ
    {
        ਲਈ( ਪੂਰਨ i = 0; i < len; i++, src1 += cn, src2 += cn )
            ਜੇ( ਨਕਾਬ[i] )
            {
                ਲਈ( ਪੂਰਨ k = 0; k < cn; k++ )
                    ਨਤੀਜਾ = std::max(ਨਤੀਜਾ, (ਪੂਰਨ)((src1[k] != 0) != (src2[k] != 0)));
                ਜੇ (ਨਤੀਜਾ != 0)
                    ਰੋਕੋ;
            }
    }
    *ਨਤੀਜਾ_2 = ਨਤੀਜਾ;
    ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ
normDiffL1_Bool(ਅਚਲ uchar* src1, ਅਚਲ uchar* src2, ਅਚਲ uchar* ਨਕਾਬ, ਪੂਰਨ* ਨਤੀਜਾ_2, ਪੂਰਨ len, ਪੂਰਨ cn)
{
    ਪੂਰਨ ਨਤੀਜਾ = *ਨਤੀਜਾ_2;
    ਜੇ( !ਨਕਾਬ )
    {
        ਲਈ( ਪੂਰਨ i = 0; i < len*cn; i++ )
            ਨਤੀਜਾ += (ਪੂਰਨ)((src1[i] != 0) != (src2[i] != 0));
    }
    ਨਹੀਂਤਾਂ
    {
        ਲਈ( ਪੂਰਨ i = 0; i < len; i++, src1 += cn, src2 += cn )
            ਜੇ( ਨਕਾਬ[i] )
            {
                ਲਈ( ਪੂਰਨ k = 0; k < cn; k++ )
                    ਨਤੀਜਾ += (ਪੂਰਨ)((src1[k] != 0) != (src2[k] != 0));
            }
    }
    *ਨਤੀਜਾ_2 = ਨਤੀਜਾ;
    ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ
normDiffL2_Bool(ਅਚਲ uchar* src1, ਅਚਲ uchar* src2, ਅਚਲ uchar* ਨਕਾਬ, ਪੂਰਨ* ਨਤੀਜਾ_2, ਪੂਰਨ len, ਪੂਰਨ cn)
{
    ਵਾਪਸ normDiffL1_Bool(src1, src2, ਨਕਾਬ, ਨਤੀਜਾ_2, len, cn);
}

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ CV_DEF_NORM_FUNC(L, suffix, type, ntype) \
    ਸਥਿਰ ਪੂਰਨ norm##L##_##suffix(ਅਚਲ type* src, ਅਚਲ uchar* ਨਕਾਬ, ntype* r, ਪੂਰਨ len, ਪੂਰਨ cn) \
{ CV_INSTRUMENT_REGION(); ਵਾਪਸ norm##L##_(src, ਨਕਾਬ, r, len, cn); } \
    ਸਥਿਰ ਪੂਰਨ normDiff##L##_##suffix(ਅਚਲ type* src1, ਅਚਲ type* src2, \
    ਅਚਲ uchar* ਨਕਾਬ, ntype* r, ਪੂਰਨ len, ਪੂਰਨ cn) \
{ ਵਾਪਸ normDiff##L##_(src1, src2, ਨਕਾਬ, r, (ਪੂਰਨ)len, cn); }

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ CV_DEF_NORM_ALL(suffix, type, inftype, l1type, l2type) \
    CV_DEF_NORM_FUNC(Inf, suffix, type, inftype) \
    CV_DEF_NORM_FUNC(L1, suffix, type, l1type) \
    CV_DEF_NORM_FUNC(L2, suffix, type, l2type)

CV_DEF_NORM_ALL(8u, uchar, ਪੂਰਨ, ਪੂਰਨ, ਪੂਰਨ)
CV_DEF_NORM_ALL(8s, schar, ਪੂਰਨ, ਪੂਰਨ, ਪੂਰਨ)
CV_DEF_NORM_ALL(16u, ushort, ਪੂਰਨ, ਪੂਰਨ, ਦੁੱਗਣਾ)
CV_DEF_NORM_ALL(16s, ਛੋਟਾ, ਪੂਰਨ, ਪੂਰਨ, ਦੁੱਗਣਾ)
CV_DEF_NORM_ALL(32u, ਬਿਨਾਨਿਸ਼ਾਨੀ, ਬਿਨਾਨਿਸ਼ਾਨੀ, ਦੁੱਗਣਾ, ਦੁੱਗਣਾ)
CV_DEF_NORM_ALL(32s, ਪੂਰਨ, ਬਿਨਾਨਿਸ਼ਾਨੀ, ਦੁੱਗਣਾ, ਦੁੱਗਣਾ)
CV_DEF_NORM_ALL(32f, ਦਸ਼ਮਲਵ, ਦਸ਼ਮਲਵ, ਦੁੱਗਣਾ, ਦੁੱਗਣਾ)
CV_DEF_NORM_ALL(64f, ਦੁੱਗਣਾ, ਦੁੱਗਣਾ, ਦੁੱਗਣਾ, ਦੁੱਗਣਾ)
CV_DEF_NORM_ALL(64u, uint64, uint64, ਦੁੱਗਣਾ, ਦੁੱਗਣਾ)
CV_DEF_NORM_ALL(64s, int64, uint64, ਦੁੱਗਣਾ, ਦੁੱਗਣਾ)
CV_DEF_NORM_ALL(16f, hfloat, ਦਸ਼ਮਲਵ, ਦਸ਼ਮਲਵ, ਦਸ਼ਮਲਵ)
CV_DEF_NORM_ALL(16bf, bfloat, ਦਸ਼ਮਲਵ, ਦਸ਼ਮਲਵ, ਦਸ਼ਮਲਵ)

NormFunc getNormFunc(ਪੂਰਨ normType, ਪੂਰਨ ਡੂੰਘਾਈ)
{
    CV_INSTRUMENT_REGION();

    ਸਥਿਰ NormFunc normTab[3][CV_ਡੂੰਘਾਈ_MAX] =
    {
        {
            normWrap<uchar, ਪੂਰਨ, normInf_8u>,
            normWrap<schar, ਪੂਰਨ, normInf_8s>,
            normWrap<ushort, ਪੂਰਨ, normInf_16u>,
            normWrap<ਛੋਟਾ, ਪੂਰਨ, normInf_16s>,
            normWrap<ਪੂਰਨ, ਬਿਨਾਨਿਸ਼ਾਨੀ, normInf_32s>,
            normWrap<ਦਸ਼ਮਲਵ, ਦਸ਼ਮਲਵ, normInf_32f>,
            normWrap<ਦੁੱਗਣਾ, ਦੁੱਗਣਾ, normInf_64f>,
            normWrap<hfloat, ਦਸ਼ਮਲਵ, normInf_16f>,
            normWrap<bfloat, ਦਸ਼ਮਲਵ, normInf_16bf>,
            normWrap<uchar, ਪੂਰਨ, normInf_Bool>,
            normWrap<uint64, uint64, normInf_64u>,
            normWrap<int64, uint64, normInf_64s>,
            normWrap<ਬਿਨਾਨਿਸ਼ਾਨੀ, ਬਿਨਾਨਿਸ਼ਾਨੀ, normInf_32u>,
            0
        },
        {
            normWrap<uchar, ਪੂਰਨ, normL1_8u>,
            normWrap<schar, ਪੂਰਨ, normL1_8s>,
            normWrap<ushort, ਪੂਰਨ, normL1_16u>,
            normWrap<ਛੋਟਾ, ਪੂਰਨ, normL1_16s>,
            normWrap<ਪੂਰਨ, ਦੁੱਗਣਾ, normL1_32s>,
            normWrap<ਦਸ਼ਮਲਵ, ਦੁੱਗਣਾ, normL1_32f>,
            normWrap<ਦੁੱਗਣਾ, ਦੁੱਗਣਾ, normL1_64f>,
            normWrap<hfloat, ਦਸ਼ਮਲਵ, normL1_16f>,
            normWrap<bfloat, ਦਸ਼ਮਲਵ, normL1_16bf>,
            normWrap<uchar, ਪੂਰਨ, normL1_Bool>,
            normWrap<uint64, ਦੁੱਗਣਾ, normL1_64u>,
            normWrap<int64, ਦੁੱਗਣਾ, normL1_64s>,
            normWrap<ਬਿਨਾਨਿਸ਼ਾਨੀ, ਦੁੱਗਣਾ, normL1_32u>,
            0
        },
        {
            normWrap<uchar, ਪੂਰਨ, normL2_8u>,
            normWrap<schar, ਪੂਰਨ, normL2_8s>,
            normWrap<ushort, ਦੁੱਗਣਾ, normL2_16u>,
            normWrap<ਛੋਟਾ, ਦੁੱਗਣਾ, normL2_16s>,
            normWrap<ਪੂਰਨ, ਦੁੱਗਣਾ, normL2_32s>,
            normWrap<ਦਸ਼ਮਲਵ, ਦੁੱਗਣਾ, normL2_32f>,
            normWrap<ਦੁੱਗਣਾ, ਦੁੱਗਣਾ, normL2_64f>,
            normWrap<hfloat, ਦਸ਼ਮਲਵ, normL2_16f>,
            normWrap<bfloat, ਦਸ਼ਮਲਵ, normL2_16bf>,
            normWrap<uchar, ਪੂਰਨ, normL2_Bool>,
            normWrap<uint64, ਦੁੱਗਣਾ, normL2_64u>,
            normWrap<int64, ਦੁੱਗਣਾ, normL2_64s>,
            normWrap<ਬਿਨਾਨਿਸ਼ਾਨੀ, ਦੁੱਗਣਾ, normL2_32u>,
            0
        }
    };

    ਜੇ (normType >= 3 || normType < 0) ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;

    ਵਾਪਸ normTab[normType][ਡੂੰਘਾਈ];
}

NormDiffFunc getNormDiffFunc(ਪੂਰਨ normType, ਪੂਰਨ ਡੂੰਘਾਈ)
{
    ਸਥਿਰ NormDiffFunc normDiffTab[3][CV_ਡੂੰਘਾਈ_MAX] =
    {
        {
            normDiffWrap<uchar, ਪੂਰਨ, normDiffInf_8u>,
            normDiffWrap<schar, ਪੂਰਨ, normDiffInf_8s>,
            normDiffWrap<ushort, ਪੂਰਨ, normDiffInf_16u>,
            normDiffWrap<ਛੋਟਾ, ਪੂਰਨ, normDiffInf_16s>,
            normDiffWrap<ਪੂਰਨ, ਬਿਨਾਨਿਸ਼ਾਨੀ, normDiffInf_32s>,
            normDiffWrap<ਦਸ਼ਮਲਵ, ਦਸ਼ਮਲਵ, normDiffInf_32f>,
            normDiffWrap<ਦੁੱਗਣਾ, ਦੁੱਗਣਾ, normDiffInf_64f>,
            normDiffWrap<hfloat, ਦਸ਼ਮਲਵ, normDiffInf_16f>,
            normDiffWrap<bfloat, ਦਸ਼ਮਲਵ, normDiffInf_16bf>,
            normDiffWrap<uchar, ਪੂਰਨ, normDiffInf_Bool>,
            normDiffWrap<uint64, uint64, normDiffInf_64u>,
            normDiffWrap<int64, uint64, normDiffInf_64s>,
            normDiffWrap<ਬਿਨਾਨਿਸ਼ਾਨੀ, ਬਿਨਾਨਿਸ਼ਾਨੀ, normDiffInf_32u>,
            0
        },
        {
            normDiffWrap<uchar, ਪੂਰਨ, normDiffL1_8u>,
            normDiffWrap<schar, ਪੂਰਨ, normDiffL1_8s>,
            normDiffWrap<ushort, ਪੂਰਨ, normDiffL1_16u>,
            normDiffWrap<ਛੋਟਾ, ਪੂਰਨ, normDiffL1_16s>,
            normDiffWrap<ਪੂਰਨ, ਦੁੱਗਣਾ, normDiffL1_32s>,
            normDiffWrap<ਦਸ਼ਮਲਵ, ਦੁੱਗਣਾ, normDiffL1_32f>,
            normDiffWrap<ਦੁੱਗਣਾ, ਦੁੱਗਣਾ, normDiffL1_64f>,
            normDiffWrap<hfloat, ਦਸ਼ਮਲਵ, normDiffL1_16f>,
            normDiffWrap<bfloat, ਦਸ਼ਮਲਵ, normDiffL1_16bf>,
            normDiffWrap<uchar, ਪੂਰਨ, normDiffL1_Bool>,
            normDiffWrap<uint64, ਦੁੱਗਣਾ, normDiffL1_64u>,
            normDiffWrap<int64, ਦੁੱਗਣਾ, normDiffL1_64s>,
            normDiffWrap<ਬਿਨਾਨਿਸ਼ਾਨੀ, ਦੁੱਗਣਾ, normDiffL1_32u>,
            0
        },
        {
            normDiffWrap<uchar, ਪੂਰਨ, normDiffL2_8u>,
            normDiffWrap<schar, ਪੂਰਨ, normDiffL2_8s>,
            normDiffWrap<ushort, ਦੁੱਗਣਾ, normDiffL2_16u>,
            normDiffWrap<ਛੋਟਾ, ਦੁੱਗਣਾ, normDiffL2_16s>,
            normDiffWrap<ਪੂਰਨ, ਦੁੱਗਣਾ, normDiffL2_32s>,
            normDiffWrap<ਦਸ਼ਮਲਵ, ਦੁੱਗਣਾ, normDiffL2_32f>,
            normDiffWrap<ਦੁੱਗਣਾ, ਦੁੱਗਣਾ, normDiffL2_64f>,
            normDiffWrap<hfloat, ਦਸ਼ਮਲਵ, normDiffL2_16f>,
            normDiffWrap<bfloat, ਦਸ਼ਮਲਵ, normDiffL2_16bf>,
            normDiffWrap<uchar, ਪੂਰਨ, normDiffL2_Bool>,
            normDiffWrap<uint64, ਦੁੱਗਣਾ, normDiffL2_64u>,
            normDiffWrap<int64, ਦੁੱਗਣਾ, normDiffL2_64s>,
            normDiffWrap<ਬਿਨਾਨਿਸ਼ਾਨੀ, ਦੁੱਗਣਾ, normDiffL2_32u>,
            0
        },
    };
    ਜੇ (normType >= 3 || normType < 0) ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;

    ਵਾਪਸ normDiffTab[normType][ਡੂੰਘਾਈ];
}

#ਜੇ_ਅੰਤ // CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

CV_CPU_OPTIMIZATION_NAMESPACE_ਸਿਰਾ

} // cv::
