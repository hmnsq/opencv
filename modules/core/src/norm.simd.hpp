// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html
//
// Copyright (C) 2025, SpaceMIT Inc., all rights reserved.
// Copyright (C) 2026, Advanced Micro Devices, Inc., all rights reserved.
// Third party copyrights are property of their respective owners.

#ಅನ್ತರ್ಭಾವ "precomp.hpp"

ನಾಮಾಕಾಶ cv {

ಬಳಸಿ NormFunc = ಪೂರ್ಣಾಂಕ (*)(ಸ್ಥಿರಾಂಕ uchar*, ಸ್ಥಿರಾಂಕ uchar*, uchar*, ಪೂರ್ಣಾಂಕ, ಪೂರ್ಣಾಂಕ);
ಬಳಸಿ NormDiffFunc = ಪೂರ್ಣಾಂಕ (*)(ಸ್ಥಿರಾಂಕ uchar*, ಸ್ಥಿರಾಂಕ uchar*, ಸ್ಥಿರಾಂಕ uchar*, uchar*, ಪೂರ್ಣಾಂಕ, ಪೂರ್ಣಾಂಕ);

CV_CPU_OPTIMIZATION_NAMESPACE_BEGIN

ಮಾದರಿ<ಬಗೆ_ಹೆಸರು T, ಬಗೆ_ಹೆಸರು RT, ಪೂರ್ಣಾಂಕ (*fn)(ಸ್ಥಿರಾಂಕ T*, ಸ್ಥಿರಾಂಕ uchar*, RT*, ಪೂರ್ಣಾಂಕ, ಪೂರ್ಣಾಂಕ)>
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ normWrap(ಸ್ಥಿರಾಂಕ uchar* src1, ಸ್ಥಿರಾಂಕ uchar* src2, uchar* res, ಪೂರ್ಣಾಂಕ len, ಪೂರ್ಣಾಂಕ cn)
{
    ಹಿಂತಿರುಗಿ fn((ಸ್ಥಿರಾಂಕ T*)src1, src2, (RT*)res, len, cn);
}

ಮಾದರಿ<ಬಗೆ_ಹೆಸರು T, ಬಗೆ_ಹೆಸರು RT, ಪೂರ್ಣಾಂಕ (*fn)(ಸ್ಥಿರಾಂಕ T*, ಸ್ಥಿರಾಂಕ T*, ಸ್ಥಿರಾಂಕ uchar*, RT*, ಪೂರ್ಣಾಂಕ, ಪೂರ್ಣಾಂಕ)>
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ normDiffWrap(ಸ್ಥಿರಾಂಕ uchar* src1, ಸ್ಥಿರಾಂಕ uchar* src2, ಸ್ಥಿರಾಂಕ uchar* src3, uchar* res, ಪೂರ್ಣಾಂಕ len, ಪೂರ್ಣಾಂಕ cn)
{
    ಹಿಂತಿರುಗಿ fn((ಸ್ಥಿರಾಂಕ T*)src1, (ಸ್ಥಿರಾಂಕ T*)src2, src3, (RT*)res, len, cn);
}

NormFunc getNormFunc(ಪೂರ್ಣಾಂಕ normType, ಪೂರ್ಣಾಂಕ ಆಳ);
NormDiffFunc getNormDiffFunc(ಪೂರ್ಣಾಂಕ normType, ಪೂರ್ಣಾಂಕ ಆಳ);

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

ಮಾದರಿ <ಬಗೆ_ಹೆಸರು T, ಬಗೆ_ಹೆಸರು ST>
ರಚನೆ NormInf_SIMD {
    ಸಾಲೊಳಗೆ ST ಕ್ರಿಯಾಕಾರಕ() (ಸ್ಥಿರಾಂಕ T* src, ಪೂರ್ಣಾಂಕ n) ಸ್ಥಿರಾಂಕ {
        ST s = 0;
        ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < n; i++) {
            s = std::max(s, (ST)cv_abs(src[i]));
        }
        ಹಿಂತಿರುಗಿ s;
    }
};

ಮಾದರಿ <ಬಗೆ_ಹೆಸರು T, ಬಗೆ_ಹೆಸರು ST>
ರಚನೆ NormL1_SIMD {
    ಸಾಲೊಳಗೆ ST ಕ್ರಿಯಾಕಾರಕ() (ಸ್ಥಿರಾಂಕ T* src, ಪೂರ್ಣಾಂಕ n) ಸ್ಥಿರಾಂಕ {
        ST s = 0;
        ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < n; i++) {
            s += cv_abs(src[i]);
        }
        ಹಿಂತಿರುಗಿ s;
    }
};

ಮಾದರಿ <ಬಗೆ_ಹೆಸರು T, ಬಗೆ_ಹೆಸರು ST>
ರಚನೆ NormL2_SIMD {
    ಸಾಲೊಳಗೆ ST ಕ್ರಿಯಾಕಾರಕ() (ಸ್ಥಿರಾಂಕ T* src, ಪೂರ್ಣಾಂಕ n) ಸ್ಥಿರಾಂಕ {
        ST s = 0;
        ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < n; i++) {
            ST v = (ST)src[i];
            s += v * v;
        }
        ಹಿಂತಿರುಗಿ s;
    }
};

ಮಾದರಿ <ಬಗೆ_ಹೆಸರು T, ಬಗೆ_ಹೆಸರು ST>
ರಚನೆ NormDiffInf_SIMD {
    ಸಾಲೊಳಗೆ ST ಕ್ರಿಯಾಕಾರಕ() (ಸ್ಥಿರಾಂಕ T* src1, ಸ್ಥಿರಾಂಕ T* src2, ಪೂರ್ಣಾಂಕ n) ಸ್ಥಿರಾಂಕ {
        ST s = 0;
        ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < n; i++) {
            ST v = (ST)cv_absdiff(src1[i], src2[i]);
            s = std::max(s, v);
        }
        ಹಿಂತಿರುಗಿ s;
    }
};

ಮಾದರಿ <ಬಗೆ_ಹೆಸರು T, ಬಗೆ_ಹೆಸರು ST>
ರಚನೆ NormDiffL1_SIMD {
    ಸಾಲೊಳಗೆ ST ಕ್ರಿಯಾಕಾರಕ() (ಸ್ಥಿರಾಂಕ T* src1, ಸ್ಥಿರಾಂಕ T* src2, ಪೂರ್ಣಾಂಕ n) ಸ್ಥಿರಾಂಕ {
        ST s = 0;
        ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < n; i++) {
            ST v = (ST)cv_absdiff(src1[i], src2[i]);
            s += v;
        }
        ಹಿಂತಿರುಗಿ s;
    }
};

ಮಾದರಿ <ಬಗೆ_ಹೆಸರು T, ಬಗೆ_ಹೆಸರು ST>
ರಚನೆ NormDiffL2_SIMD {
    ಸಾಲೊಳಗೆ ST ಕ್ರಿಯಾಕಾರಕ() (ಸ್ಥಿರಾಂಕ T* src1, ಸ್ಥಿರಾಂಕ T* src2, ಪೂರ್ಣಾಂಕ n) ಸ್ಥಿರಾಂಕ {
        ST s = 0;
        ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < n; i++) {
            ST v = (ST)src1[i] - (ST)src2[i];
            s += v * v;
        }
        ಹಿಂತಿರುಗಿ s;
    }
};

#ಆದರೆ (CV_SIMD || CV_SIMD_SCALABLE)

ಮಾದರಿ<>
ರಚನೆ NormInf_SIMD<uchar, ಪೂರ್ಣಾಂಕ> {
    ಪೂರ್ಣಾಂಕ ಕ್ರಿಯಾಕಾರಕ() (ಸ್ಥಿರಾಂಕ uchar* src, ಪೂರ್ಣಾಂಕ n) ಸ್ಥಿರಾಂಕ {
        ಪೂರ್ಣಾಂಕ j = 0;
        ಪೂರ್ಣಾಂಕ s = 0;
        v_uint8 r0 = vx_setzero_u8(), r1 = vx_setzero_u8();
        v_uint8 r2 = vx_setzero_u8(), r3 = vx_setzero_u8();
        ಗಾಗಿ (; j <= n - 4 * VTraits<v_uint8>::vlanes(); j += 4 * VTraits<v_uint8>::vlanes()) {
            r0 = v_max(r0, vx_ತುಂಬು(src + j                                 ));
            r1 = v_max(r1, vx_ತುಂಬು(src + j +     VTraits<v_uint8>::vlanes()));
            r2 = v_max(r2, vx_ತುಂಬು(src + j + 2 * VTraits<v_uint8>::vlanes()));
            r3 = v_max(r3, vx_ತುಂಬು(src + j + 3 * VTraits<v_uint8>::vlanes()));
        }
        r0 = v_max(r0, v_max(r1, v_max(r2, r3)));
        ಗಾಗಿ (; j < n; j++) {
            s = std::max(s, (ಪೂರ್ಣಾಂಕ)src[j]);
        }
        ಹಿಂತಿರುಗಿ std::max(s, (ಪೂರ್ಣಾಂಕ)v_reduce_max(r0));
    }
};

ಮಾದರಿ<>
ರಚನೆ NormInf_SIMD<schar, ಪೂರ್ಣಾಂಕ> {
    ಪೂರ್ಣಾಂಕ ಕ್ರಿಯಾಕಾರಕ() (ಸ್ಥಿರಾಂಕ schar* src, ಪೂರ್ಣಾಂಕ n) ಸ್ಥಿರಾಂಕ {
        ಪೂರ್ಣಾಂಕ j = 0;
        ಪೂರ್ಣಾಂಕ s = 0;
        v_uint8 r0 = vx_setzero_u8(), r1 = vx_setzero_u8();
        v_uint8 r2 = vx_setzero_u8(), r3 = vx_setzero_u8();
        ಗಾಗಿ (; j <= n - 4 * VTraits<v_int8>::vlanes(); j += 4 * VTraits<v_int8>::vlanes()) {
            r0 = v_max(r0, v_abs(vx_ತುಂಬು(src + j                                )));
            r1 = v_max(r1, v_abs(vx_ತುಂಬು(src + j +     VTraits<v_int8>::vlanes())));
            r2 = v_max(r2, v_abs(vx_ತುಂಬು(src + j + 2 * VTraits<v_int8>::vlanes())));
            r3 = v_max(r3, v_abs(vx_ತುಂಬು(src + j + 3 * VTraits<v_int8>::vlanes())));
        }
        r0 = v_max(r0, v_max(r1, v_max(r2, r3)));
        ಗಾಗಿ (; j < n; j++) {
            s = std::max(s, cv_abs(src[j]));
        }
        ಹಿಂತಿರುಗಿ std::max(s, saturate_cast<ಪೂರ್ಣಾಂಕ>(v_reduce_max(r0)));
    }
};

ಮಾದರಿ<>
ರಚನೆ NormInf_SIMD<ushort, ಪೂರ್ಣಾಂಕ> {
    ಪೂರ್ಣಾಂಕ ಕ್ರಿಯಾಕಾರಕ() (ಸ್ಥಿರಾಂಕ ushort* src, ಪೂರ್ಣಾಂಕ n) ಸ್ಥಿರಾಂಕ {
        ಪೂರ್ಣಾಂಕ j = 0;
        ಪೂರ್ಣಾಂಕ s = 0;
        v_uint16 d0 = vx_setzero_u16(), d1 = vx_setzero_u16();
        v_uint16 d2 = vx_setzero_u16(), d3 = vx_setzero_u16();
        ಗಾಗಿ (; j <= n - 4 * VTraits<v_uint16>::vlanes(); j += 4 * VTraits<v_uint16>::vlanes()) {
            d0 = v_max(d0, vx_ತುಂಬು(src + j                                  ));
            d1 = v_max(d1, vx_ತುಂಬು(src + j +     VTraits<v_uint16>::vlanes()));
            d2 = v_max(d2, vx_ತುಂಬು(src + j + 2 * VTraits<v_uint16>::vlanes()));
            d3 = v_max(d3, vx_ತುಂಬು(src + j + 3 * VTraits<v_uint16>::vlanes()));
        }
        d0 = v_max(d0, v_max(d1, v_max(d2, d3)));
        ಗಾಗಿ (; j < n; j++) {
            s = std::max(s, (ಪೂರ್ಣಾಂಕ)src[j]);
        }
        ಹಿಂತಿರುಗಿ std::max(s, (ಪೂರ್ಣಾಂಕ)v_reduce_max(d0));
    }
};

ಮಾದರಿ<>
ರಚನೆ NormInf_SIMD<ಗಿಡ್ಡ, ಪೂರ್ಣಾಂಕ> {
    ಪೂರ್ಣಾಂಕ ಕ್ರಿಯಾಕಾರಕ() (ಸ್ಥಿರಾಂಕ ಗಿಡ್ಡ* src, ಪೂರ್ಣಾಂಕ n) ಸ್ಥಿರಾಂಕ {
        ಪೂರ್ಣಾಂಕ j = 0;
        ಪೂರ್ಣಾಂಕ s = 0;
        v_uint16 d0 = vx_setzero_u16(), d1 = vx_setzero_u16();
        v_uint16 d2 = vx_setzero_u16(), d3 = vx_setzero_u16();
        ಗಾಗಿ (; j <= n - 4 * VTraits<v_int16>::vlanes(); j += 4 * VTraits<v_int16>::vlanes()) {
            d0 = v_max(d0, v_abs(vx_ತುಂಬು(src + j                                  )));
            d1 = v_max(d1, v_abs(vx_ತುಂಬು(src + j +     VTraits<v_int16>::vlanes())));
            d2 = v_max(d2, v_abs(vx_ತುಂಬು(src + j + 2 * VTraits<v_int16>::vlanes())));
            d3 = v_max(d3, v_abs(vx_ತುಂಬು(src + j + 3 * VTraits<v_int16>::vlanes())));
        }
        d0 = v_max(d0, v_max(d1, v_max(d2, d3)));
        ಗಾಗಿ (; j < n; j++) {
            s = std::max(s, saturate_cast<ಪೂರ್ಣಾಂಕ>(cv_abs(src[j])));
        }
        ಹಿಂತಿರುಗಿ std::max(s, saturate_cast<ಪೂರ್ಣಾಂಕ>(v_reduce_max(d0)));
    }
};

ಮಾದರಿ<>
ರಚನೆ NormInf_SIMD<ಪೂರ್ಣಾಂಕ, ಪೂರ್ಣಾಂಕ> {
    ಪೂರ್ಣಾಂಕ ಕ್ರಿಯಾಕಾರಕ() (ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ* src, ಪೂರ್ಣಾಂಕ n) ಸ್ಥಿರಾಂಕ {
        ಪೂರ್ಣಾಂಕ j = 0;
        ಪೂರ್ಣಾಂಕ s = 0;
        v_uint32 r0 = vx_setzero_u32(), r1 = vx_setzero_u32();
        v_uint32 r2 = vx_setzero_u32(), r3 = vx_setzero_u32();
        ಗಾಗಿ (; j <= n - 4 * VTraits<v_int32>::vlanes(); j += 4 * VTraits<v_int32>::vlanes()) {
            r0 = v_max(r0, v_abs(vx_ತುಂಬು(src + j                                 )));
            r1 = v_max(r1, v_abs(vx_ತುಂಬು(src + j +     VTraits<v_int32>::vlanes())));
            r2 = v_max(r2, v_abs(vx_ತುಂಬು(src + j + 2 * VTraits<v_int32>::vlanes())));
            r3 = v_max(r3, v_abs(vx_ತುಂಬು(src + j + 3 * VTraits<v_int32>::vlanes())));
        }
        r0 = v_max(r0, v_max(r1, v_max(r2, r3)));
        s = std::max(s, saturate_cast<ಪೂರ್ಣಾಂಕ>(v_reduce_max(r0)));
        ಗಾಗಿ (; j < n; j++) {
            s = std::max(s, std::ನಿರಪೇಕ್ಷ(src[j]));
        }
        ಹಿಂತಿರುಗಿ s;
    }
};

ಮಾದರಿ<>
ರಚನೆ NormInf_SIMD<ದಶಮಾಂಶ, ದಶಮಾಂಶ> {
    ದಶಮಾಂಶ ಕ್ರಿಯಾಕಾರಕ() (ಸ್ಥಿರಾಂಕ ದಶಮಾಂಶ* src, ಪೂರ್ಣಾಂಕ n) ಸ್ಥಿರಾಂಕ {
        ಪೂರ್ಣಾಂಕ j = 0;
        ದಶಮಾಂಶ s = 0.f;
        v_float32 r0 = vx_setzero_f32(), r1 = vx_setzero_f32();
        v_float32 r2 = vx_setzero_f32(), r3 = vx_setzero_f32();
        ಗಾಗಿ (; j <= n - 4 * VTraits<v_float32>::vlanes(); j += 4 * VTraits<v_float32>::vlanes()) {
            r0 = v_max(r0, v_abs(vx_ತುಂಬು(src + j                                   )));
            r1 = v_max(r1, v_abs(vx_ತುಂಬು(src + j +     VTraits<v_float32>::vlanes())));
            r2 = v_max(r2, v_abs(vx_ತುಂಬು(src + j + 2 * VTraits<v_float32>::vlanes())));
            r3 = v_max(r3, v_abs(vx_ತುಂಬು(src + j + 3 * VTraits<v_float32>::vlanes())));
        }
        r0 = v_max(r0, v_max(r1, v_max(r2, r3)));
        ಗಾಗಿ (; j < n; j++) {
            s = std::max(s, cv_abs(src[j]));
        }
        ಹಿಂತಿರುಗಿ std::max(s, v_reduce_max(r0));
    }
};

ಮಾದರಿ<>
ರಚನೆ NormL1_SIMD<uchar, ಪೂರ್ಣಾಂಕ> {
    ಪೂರ್ಣಾಂಕ ಕ್ರಿಯಾಕಾರಕ() (ಸ್ಥಿರಾಂಕ uchar* src, ಪೂರ್ಣಾಂಕ n) ಸ್ಥಿರಾಂಕ {
        ಪೂರ್ಣಾಂಕ j = 0;
        ಪೂರ್ಣಾಂಕ s = 0;
        v_uint32 r0 = vx_setzero_u32(), r1 = vx_setzero_u32();
        v_uint8  one = vx_setall_u8(1);
        ಗಾಗಿ (; j<= n - 2 * VTraits<v_uint8>::vlanes(); j += 2 * VTraits<v_uint8>::vlanes()) {
            v_uint8 v0 = vx_ತುಂಬು(src + j);
            r0 = v_dotprod_expand_fast(v0, one, r0);

            v_uint8 v1 = vx_ತುಂಬು(src + j + VTraits<v_uint8>::vlanes());
            r1 = v_dotprod_expand_fast(v1, one, r1);
        }
        s += v_reduce_ಮೊತ್ತ(v_ಕೂಡಿಸು(r0, r1));
        ಗಾಗಿ (; j < n; j++) {
            s += src[j];
        }
        ಹಿಂತಿರುಗಿ s;
    }
};

ಮಾದರಿ<>
ರಚನೆ NormL1_SIMD<schar, ಪೂರ್ಣಾಂಕ> {
    ಪೂರ್ಣಾಂಕ ಕ್ರಿಯಾಕಾರಕ() (ಸ್ಥಿರಾಂಕ schar* src, ಪೂರ್ಣಾಂಕ n) ಸ್ಥಿರಾಂಕ {
        ಪೂರ್ಣಾಂಕ j = 0;
        ಪೂರ್ಣಾಂಕ s = 0;
        v_uint32 r0 = vx_setzero_u32(), r1 = vx_setzero_u32();
        v_uint8  one = vx_setall_u8(1);
        ಗಾಗಿ (; j<= n - 2 * VTraits<v_int8>::vlanes(); j += 2 * VTraits<v_int8>::vlanes()) {
            v_uint8 v0 = v_abs(vx_ತುಂಬು(src + j));
            r0 = v_dotprod_expand_fast(v0, one, r0);

            v_uint8 v1 = v_abs(vx_ತುಂಬು(src + j + VTraits<v_int8>::vlanes()));
            r1 = v_dotprod_expand_fast(v1, one, r1);
        }
        s += v_reduce_ಮೊತ್ತ(v_ಕೂಡಿಸು(r0, r1));
        ಗಾಗಿ (; j < n; j++) {
            s += saturate_cast<ಪೂರ್ಣಾಂಕ>(cv_abs(src[j]));
        }
        ಹಿಂತಿರುಗಿ s;
    }
};

ಮಾದರಿ<>
ರಚನೆ NormL1_SIMD<ushort, ಪೂರ್ಣಾಂಕ> {
    ಪೂರ್ಣಾಂಕ ಕ್ರಿಯಾಕಾರಕ() (ಸ್ಥಿರಾಂಕ ushort* src, ಪೂರ್ಣಾಂಕ n) ಸ್ಥಿರಾಂಕ {
        ಪೂರ್ಣಾಂಕ j = 0;
        ಪೂರ್ಣಾಂಕ s = 0;
        v_uint32 r00 = vx_setzero_u32(), r01 = vx_setzero_u32();
        v_uint32 r10 = vx_setzero_u32(), r11 = vx_setzero_u32();
        ಗಾಗಿ (; j<= n - 2 * VTraits<v_uint16>::vlanes(); j += 2 * VTraits<v_uint16>::vlanes()) {
            v_uint16 v0 = vx_ತುಂಬು(src + j);
            v_uint32 v00, v01;
            v_expand(v0, v00, v01);
            r00 = v_ಕೂಡಿಸು(r00, v00);
            r01 = v_ಕೂಡಿಸು(r01, v01);

            v_uint16 v1 = vx_ತುಂಬು(src + j + VTraits<v_uint16>::vlanes());
            v_uint32 v10, v11;
            v_expand(v1, v10, v11);
            r10 = v_ಕೂಡಿಸು(r10, v10);
            r11 = v_ಕೂಡಿಸು(r11, v11);
        }
        s += (ಪೂರ್ಣಾಂಕ)v_reduce_ಮೊತ್ತ(v_ಕೂಡಿಸು(v_ಕೂಡಿಸು(v_ಕೂಡಿಸು(r00, r01), r10), r11));
        ಗಾಗಿ (; j < n; j++) {
            s += src[j];
        }
        ಹಿಂತಿರುಗಿ s;
    }
};

ಮಾದರಿ<>
ರಚನೆ NormL1_SIMD<ಗಿಡ್ಡ, ಪೂರ್ಣಾಂಕ> {
    ಪೂರ್ಣಾಂಕ ಕ್ರಿಯಾಕಾರಕ() (ಸ್ಥಿರಾಂಕ ಗಿಡ್ಡ* src, ಪೂರ್ಣಾಂಕ n) ಸ್ಥಿರಾಂಕ {
        ಪೂರ್ಣಾಂಕ j = 0;
        ಪೂರ್ಣಾಂಕ s = 0;
        v_uint32 r00 = vx_setzero_u32(), r01 = vx_setzero_u32();
        v_uint32 r10 = vx_setzero_u32(), r11 = vx_setzero_u32();
        ಗಾಗಿ (; j<= n - 2 * VTraits<v_int16>::vlanes(); j += 2 * VTraits<v_int16>::vlanes()) {
            v_uint16 v0 = v_abs(vx_ತುಂಬು(src + j));
            v_uint32 v00, v01;
            v_expand(v0, v00, v01);
            r00 = v_ಕೂಡಿಸು(r00, v00);
            r01 = v_ಕೂಡಿಸು(r01, v01);

            v_uint16 v1 = v_abs(vx_ತುಂಬು(src + j + VTraits<v_int16>::vlanes()));
            v_uint32 v10, v11;
            v_expand(v1, v10, v11);
            r10 = v_ಕೂಡಿಸು(r10, v10);
            r11 = v_ಕೂಡಿಸು(r11, v11);
        }
        s += (ಪೂರ್ಣಾಂಕ)v_reduce_ಮೊತ್ತ(v_ಕೂಡಿಸು(v_ಕೂಡಿಸು(v_ಕೂಡಿಸು(r00, r01), r10), r11));
        ಗಾಗಿ (; j < n; j++) {
            s += saturate_cast<ಪೂರ್ಣಾಂಕ>(cv_abs(src[j]));
        }
        ಹಿಂತಿರುಗಿ s;
    }
};

ಮಾದರಿ<>
ರಚನೆ NormL2_SIMD<uchar, ಪೂರ್ಣಾಂಕ> {
    ಪೂರ್ಣಾಂಕ ಕ್ರಿಯಾಕಾರಕ() (ಸ್ಥಿರಾಂಕ uchar* src, ಪೂರ್ಣಾಂಕ n) ಸ್ಥಿರಾಂಕ {
        ಪೂರ್ಣಾಂಕ j = 0;
        ಪೂರ್ಣಾಂಕ s = 0;
        v_uint32 r0 = vx_setzero_u32(), r1 = vx_setzero_u32();
        ಗಾಗಿ (; j <= n - 2 * VTraits<v_uint8>::vlanes(); j += 2 * VTraits<v_uint8>::vlanes()) {
            v_uint8 v0 = vx_ತುಂಬು(src + j);
            r0 = v_dotprod_expand_fast(v0, v0, r0);

            v_uint8 v1 = vx_ತುಂಬು(src + j + VTraits<v_uint8>::vlanes());
            r1 = v_dotprod_expand_fast(v1, v1, r1);
        }
        s += v_reduce_ಮೊತ್ತ(v_ಕೂಡಿಸು(r0, r1));
        ಗಾಗಿ (; j < n; j++) {
            ಪೂರ್ಣಾಂಕ v = saturate_cast<ಪೂರ್ಣಾಂಕ>(src[j]);
            s += v * v;
        }
        ಹಿಂತಿರುಗಿ s;
    }
};

ಮಾದರಿ<>
ರಚನೆ NormL2_SIMD<schar, ಪೂರ್ಣಾಂಕ> {
    ಪೂರ್ಣಾಂಕ ಕ್ರಿಯಾಕಾರಕ() (ಸ್ಥಿರಾಂಕ schar* src, ಪೂರ್ಣಾಂಕ n) ಸ್ಥಿರಾಂಕ {
        ಪೂರ್ಣಾಂಕ j = 0;
        ಪೂರ್ಣಾಂಕ s = 0;
        v_int32 r0 = vx_setzero_s32(), r1 = vx_setzero_s32();
        ಗಾಗಿ (; j <= n - 2 * VTraits<v_int8>::vlanes(); j += 2 * VTraits<v_int8>::vlanes()) {
            v_int8 v0 = vx_ತುಂಬು(src + j);
            r0 = v_dotprod_expand_fast(v0, v0, r0);
            v_int8 v1 = vx_ತುಂಬು(src + j + VTraits<v_int8>::vlanes());
            r1 = v_dotprod_expand_fast(v1, v1, r1);
        }
        s += v_reduce_ಮೊತ್ತ(v_ಕೂಡಿಸು(r0, r1));
        ಗಾಗಿ (; j < n; j++) {
            ಪೂರ್ಣಾಂಕ v = saturate_cast<ಪೂರ್ಣಾಂಕ>(src[j]);
            s += v * v;
        }
        ಹಿಂತಿರುಗಿ s;
    }
};

ಮಾದರಿ<>
ರಚನೆ NormDiffInf_SIMD<uchar, ಪೂರ್ಣಾಂಕ> {
    ಪೂರ್ಣಾಂಕ ಕ್ರಿಯಾಕಾರಕ() (ಸ್ಥಿರಾಂಕ uchar* src1, ಸ್ಥಿರಾಂಕ uchar* src2, ಪೂರ್ಣಾಂಕ n) ಸ್ಥಿರಾಂಕ {
        ಪೂರ್ಣಾಂಕ j = 0;
        ಪೂರ್ಣಾಂಕ s = 0;
        v_uint8 r0 = vx_setzero_u8(), r1 = vx_setzero_u8();
        v_uint8 r2 = vx_setzero_u8(), r3 = vx_setzero_u8();
        ಗಾಗಿ (; j <= n - 4 * VTraits<v_uint8>::vlanes(); j += 4 * VTraits<v_uint8>::vlanes()) {
            v_uint8 v01 = vx_ತುಂಬು(src1 + j), v02 = vx_ತುಂಬು(src2 + j);
            r0 = v_max(r0, v_absdiff(v01, v02));

            v_uint8 v11 = vx_ತುಂಬು(src1 + j + VTraits<v_uint8>::vlanes()),
                    v12 = vx_ತುಂಬು(src2 + j + VTraits<v_uint8>::vlanes());
            r1 = v_max(r1, v_absdiff(v11, v12));

            v_uint8 v21 = vx_ತುಂಬು(src1 + j + 2 * VTraits<v_uint8>::vlanes()),
                    v22 = vx_ತುಂಬು(src2 + j + 2 * VTraits<v_uint8>::vlanes());
            r2 = v_max(r2, v_absdiff(v21, v22));

            v_uint8 v31 = vx_ತುಂಬು(src1 + j + 3 * VTraits<v_uint8>::vlanes()),
                    v32 = vx_ತುಂಬು(src2 + j + 3 * VTraits<v_uint8>::vlanes());
            r3 = v_max(r3, v_absdiff(v31, v32));
        }
        s = (ಪೂರ್ಣಾಂಕ)v_reduce_max(v_max(v_max(v_max(r0, r1), r2), r3));
        ಗಾಗಿ (; j < n; j++) {
            ಪೂರ್ಣಾಂಕ v = (ಪೂರ್ಣಾಂಕ)cv_absdiff(src1[j], src2[j]);
            s = std::max(s, v);
        }
        ಹಿಂತಿರುಗಿ s;
    }
};

ಮಾದರಿ<>
ರಚನೆ NormDiffInf_SIMD<schar, ಪೂರ್ಣಾಂಕ> {
    ಪೂರ್ಣಾಂಕ ಕ್ರಿಯಾಕಾರಕ() (ಸ್ಥಿರಾಂಕ schar* src1, ಸ್ಥಿರಾಂಕ schar* src2, ಪೂರ್ಣಾಂಕ n) ಸ್ಥಿರಾಂಕ {
        ಪೂರ್ಣಾಂಕ j = 0;
        ಪೂರ್ಣಾಂಕ s = 0;
        v_uint8 r0 = vx_setzero_u8(), r1 = vx_setzero_u8();
        v_uint8 r2 = vx_setzero_u8(), r3 = vx_setzero_u8();
        ಗಾಗಿ (; j <= n - 4 * VTraits<v_int8>::vlanes(); j += 4 * VTraits<v_int8>::vlanes()) {
            v_int8 v01 = vx_ತುಂಬು(src1 + j), v02 = vx_ತುಂಬು(src2 + j);
            r0 = v_max(r0, v_absdiff(v01, v02));

            v_int8 v11 = vx_ತುಂಬು(src1 + j + VTraits<v_int8>::vlanes()),
                   v12 = vx_ತುಂಬು(src2 + j + VTraits<v_int8>::vlanes());
            r1 = v_max(r1, v_absdiff(v11, v12));

            v_int8 v21 = vx_ತುಂಬು(src1 + j + 2 * VTraits<v_int8>::vlanes()),
                   v22 = vx_ತುಂಬು(src2 + j + 2 * VTraits<v_int8>::vlanes());
            r2 = v_max(r2, v_absdiff(v21, v22));

            v_int8 v31 = vx_ತುಂಬು(src1 + j + 3 * VTraits<v_int8>::vlanes()),
                   v32 = vx_ತುಂಬು(src2 + j + 3 * VTraits<v_int8>::vlanes());
            r3 = v_max(r3, v_absdiff(v31, v32));
        }
        s = (ಪೂರ್ಣಾಂಕ)v_reduce_max(v_max(v_max(v_max(r0, r1), r2), r3));
        ಗಾಗಿ (; j < n; j++) {
            ಪೂರ್ಣಾಂಕ v = (ಪೂರ್ಣಾಂಕ)cv_absdiff(src1[j], src2[j]);
            s = std::max(s, v);
        }
        ಹಿಂತಿರುಗಿ s;
    }
};

ಮಾದರಿ<>
ರಚನೆ NormDiffInf_SIMD<ushort, ಪೂರ್ಣಾಂಕ> {
    ಪೂರ್ಣಾಂಕ ಕ್ರಿಯಾಕಾರಕ() (ಸ್ಥಿರಾಂಕ ushort* src1, ಸ್ಥಿರಾಂಕ ushort* src2, ಪೂರ್ಣಾಂಕ n) ಸ್ಥಿರಾಂಕ {
        ಪೂರ್ಣಾಂಕ j = 0;
        ಪೂರ್ಣಾಂಕ s = 0;
        v_uint16 r0 = vx_setzero_u16(), r1 = vx_setzero_u16();
        v_uint16 r2 = vx_setzero_u16(), r3 = vx_setzero_u16();
        ಗಾಗಿ (; j <= n - 4 * VTraits<v_uint16>::vlanes(); j += 4 * VTraits<v_uint16>::vlanes()) {
            v_uint16 v01 = vx_ತುಂಬು(src1 + j), v02 = vx_ತುಂಬು(src2 + j);
            r0 = v_max(r0, v_absdiff(v01, v02));

            v_uint16 v11 = vx_ತುಂಬು(src1 + j + VTraits<v_uint16>::vlanes()),
                     v12 = vx_ತುಂಬು(src2 + j + VTraits<v_uint16>::vlanes());
            r1 = v_max(r1, v_absdiff(v11, v12));

            v_uint16 v21 = vx_ತುಂಬು(src1 + j + 2 * VTraits<v_uint16>::vlanes()),
                     v22 = vx_ತುಂಬು(src2 + j + 2 * VTraits<v_uint16>::vlanes());
            r2 = v_max(r2, v_absdiff(v21, v22));

            v_uint16 v31 = vx_ತುಂಬು(src1 + j + 3 * VTraits<v_uint16>::vlanes()),
                     v32 = vx_ತುಂಬು(src2 + j + 3 * VTraits<v_uint16>::vlanes());
            r3 = v_max(r3, v_absdiff(v31, v32));
        }
        s = (ಪೂರ್ಣಾಂಕ)v_reduce_max(v_max(v_max(v_max(r0, r1), r2), r3));
        ಗಾಗಿ (; j < n; j++) {
            ಪೂರ್ಣಾಂಕ v = (ಪೂರ್ಣಾಂಕ)cv_absdiff(src1[j], src2[j]);
            s = std::max(s, v);
        }
        ಹಿಂತಿರುಗಿ s;
    }
};

ಮಾದರಿ<>
ರಚನೆ NormDiffInf_SIMD<ಗಿಡ್ಡ, ಪೂರ್ಣಾಂಕ> {
    ಪೂರ್ಣಾಂಕ ಕ್ರಿಯಾಕಾರಕ() (ಸ್ಥಿರಾಂಕ ಗಿಡ್ಡ* src1, ಸ್ಥಿರಾಂಕ ಗಿಡ್ಡ* src2, ಪೂರ್ಣಾಂಕ n) ಸ್ಥಿರಾಂಕ {
        ಪೂರ್ಣಾಂಕ j = 0;
        ಪೂರ್ಣಾಂಕ s = 0;
        v_uint16 r0 = vx_setzero_u16(), r1 = vx_setzero_u16();
        v_uint16 r2 = vx_setzero_u16(), r3 = vx_setzero_u16();
        ಗಾಗಿ (; j <= n - 4 * VTraits<v_int16>::vlanes(); j += 4 * VTraits<v_int16>::vlanes()) {
            v_int16 v01 = vx_ತುಂಬು(src1 + j), v02 = vx_ತುಂಬು(src2 + j);
            r0 = v_max(r0, v_absdiff(v01, v02));

            v_int16 v11 = vx_ತುಂಬು(src1 + j + VTraits<v_int16>::vlanes()),
                    v12 = vx_ತುಂಬು(src2 + j + VTraits<v_int16>::vlanes());
            r1 = v_max(r1, v_absdiff(v11, v12));

            v_int16 v21 = vx_ತುಂಬು(src1 + j + 2 * VTraits<v_int16>::vlanes()),
                    v22 = vx_ತುಂಬು(src2 + j + 2 * VTraits<v_int16>::vlanes());
            r2 = v_max(r2, v_absdiff(v21, v22));

            v_int16 v31 = vx_ತುಂಬು(src1 + j + 3 * VTraits<v_int16>::vlanes()),
                    v32 = vx_ತುಂಬು(src2 + j + 3 * VTraits<v_int16>::vlanes());
            r3 = v_max(r3, v_absdiff(v31, v32));
        }
        s = (ಪೂರ್ಣಾಂಕ)v_reduce_max(v_max(v_max(v_max(r0, r1), r2), r3));
        ಗಾಗಿ (; j < n; j++) {
            ಪೂರ್ಣಾಂಕ v = (ಪೂರ್ಣಾಂಕ)cv_absdiff(src1[j], src2[j]);
            s = std::max(s, v);
        }
        ಹಿಂತಿರುಗಿ s;
    }
};

ಮಾದರಿ<>
ರಚನೆ NormDiffInf_SIMD<ಪೂರ್ಣಾಂಕ, ಚಿಹ್ನೆರಹಿತ> {
    ಚಿಹ್ನೆರಹಿತ ಕ್ರಿಯಾಕಾರಕ() (ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ* src1, ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ* src2, ಪೂರ್ಣಾಂಕ n) ಸ್ಥಿರಾಂಕ {
        ಪೂರ್ಣಾಂಕ j = 0;
        ಚಿಹ್ನೆರಹಿತ s = 0;
        v_uint32 r0 = vx_setzero_u32(), r1 = vx_setzero_u32();
        v_uint32 r2 = vx_setzero_u32(), r3 = vx_setzero_u32();
        ಗಾಗಿ (; j <= n - 4 * VTraits<v_int32>::vlanes(); j += 4 * VTraits<v_int32>::vlanes()) {
            v_int32 v01 = vx_ತುಂಬು(src1 + j), v02 = vx_ತುಂಬು(src2 + j);
            r0 = v_max(r0, v_absdiff(v01, v02));

            v_int32 v11 = vx_ತುಂಬು(src1 + j + VTraits<v_int32>::vlanes()),
                    v12 = vx_ತುಂಬು(src2 + j + VTraits<v_int32>::vlanes());
            r1 = v_max(r1, v_absdiff(v11, v12));

            v_int32 v21 = vx_ತುಂಬು(src1 + j + 2 * VTraits<v_int32>::vlanes()),
                    v22 = vx_ತುಂಬು(src2 + j + 2 * VTraits<v_int32>::vlanes());
            r2 = v_max(r2, v_absdiff(v21, v22));

            v_int32 v31 = vx_ತುಂಬು(src1 + j + 3 * VTraits<v_int32>::vlanes()),
                    v32 = vx_ತುಂಬು(src2 + j + 3 * VTraits<v_int32>::vlanes());
            r3 = v_max(r3, v_absdiff(v31, v32));
        }
        s = (ಪೂರ್ಣಾಂಕ)v_reduce_max(v_max(v_max(v_max(r0, r1), r2), r3));
        ಗಾಗಿ (; j < n; j++) {
            ಚಿಹ್ನೆರಹಿತ v = (ಚಿಹ್ನೆರಹಿತ)cv_absdiff(src1[j], src2[j]);
            s = std::max(s, v);
        }
        ಹಿಂತಿರುಗಿ s;
    }
};

ಮಾದರಿ<>
ರಚನೆ NormDiffInf_SIMD<ದಶಮಾಂಶ, ದಶಮಾಂಶ> {
    ದಶಮಾಂಶ ಕ್ರಿಯಾಕಾರಕ() (ಸ್ಥಿರಾಂಕ ದಶಮಾಂಶ* src1, ಸ್ಥಿರಾಂಕ ದಶಮಾಂಶ* src2, ಪೂರ್ಣಾಂಕ n) ಸ್ಥಿರಾಂಕ {
        ಪೂರ್ಣಾಂಕ j = 0;
        ದಶಮಾಂಶ s = 0;
        v_float32 r0 = vx_setzero_f32(), r1 = vx_setzero_f32();
        ಗಾಗಿ (; j <= n - 2 * VTraits<v_float32>::vlanes(); j += 2 * VTraits<v_float32>::vlanes()) {
            v_float32 v01 = vx_ತುಂಬು(src1 + j), v02 = vx_ತುಂಬು(src2 + j);
            r0 = v_max(r0, v_absdiff(v01, v02));

            v_float32 v11 = vx_ತುಂಬು(src1 + j + VTraits<v_float32>::vlanes()),
                      v12 = vx_ತುಂಬು(src2 + j + VTraits<v_float32>::vlanes());
            r1 = v_max(r1, v_absdiff(v11, v12));
        }
        s = v_reduce_max(v_max(r0, r1));
        ಗಾಗಿ (; j < n; j++) {
            ದಶಮಾಂಶ v = (ದಶಮಾಂಶ)cv_absdiff(src1[j], src2[j]);
            s = std::max(s, v);
        }
        ಹಿಂತಿರುಗಿ s;
    }
};

ಮಾದರಿ<>
ರಚನೆ NormDiffL1_SIMD<uchar, ಪೂರ್ಣಾಂಕ> {
    ಪೂರ್ಣಾಂಕ ಕ್ರಿಯಾಕಾರಕ() (ಸ್ಥಿರಾಂಕ uchar* src1, ಸ್ಥಿರಾಂಕ uchar* src2, ಪೂರ್ಣಾಂಕ n) ಸ್ಥಿರಾಂಕ {
        ಪೂರ್ಣಾಂಕ j = 0;
        ಪೂರ್ಣಾಂಕ s = 0;
        v_uint32 r0 = vx_setzero_u32(), r1 = vx_setzero_u32();
        v_uint8  one = vx_setall_u8(1);
        ಗಾಗಿ (; j<= n - 2 * VTraits<v_uint8>::vlanes(); j += 2 * VTraits<v_uint8>::vlanes()) {
            v_uint8 v01 = vx_ತುಂಬು(src1 + j), v02 = vx_ತುಂಬು(src2 + j);
            r0 = v_dotprod_expand_fast(v_absdiff(v01, v02), one, r0);

            v_uint8 v11 = vx_ತುಂಬು(src1 + j + VTraits<v_uint8>::vlanes()),
                    v12 = vx_ತುಂಬು(src2 + j + VTraits<v_uint8>::vlanes());
            r1 = v_dotprod_expand_fast(v_absdiff(v11, v12), one, r1);
        }
        s += v_reduce_ಮೊತ್ತ(v_ಕೂಡಿಸು(r0, r1));
        ಗಾಗಿ (; j < n; j++) {
            ಪೂರ್ಣಾಂಕ v = (ಪೂರ್ಣಾಂಕ)cv_absdiff(src1[j], src2[j]);
            s += v;
        }
        ಹಿಂತಿರುಗಿ s;
    }
};

ಮಾದರಿ<>
ರಚನೆ NormDiffL1_SIMD<schar, ಪೂರ್ಣಾಂಕ> {
    ಪೂರ್ಣಾಂಕ ಕ್ರಿಯಾಕಾರಕ() (ಸ್ಥಿರಾಂಕ schar* src1, ಸ್ಥಿರಾಂಕ schar* src2, ಪೂರ್ಣಾಂಕ n) ಸ್ಥಿರಾಂಕ {
        ಪೂರ್ಣಾಂಕ j = 0;
        ಪೂರ್ಣಾಂಕ s = 0;
        v_uint32 r0 = vx_setzero_u32(), r1 = vx_setzero_u32();
        v_uint8  one = vx_setall_u8(1);
        ಗಾಗಿ (; j<= n - 2 * VTraits<v_int8>::vlanes(); j += 2 * VTraits<v_int8>::vlanes()) {
            v_int8 v01 = vx_ತುಂಬು(src1 + j), v02 = vx_ತುಂಬು(src2 + j);
            r0 = v_dotprod_expand_fast(v_absdiff(v01, v02), one, r0);

            v_int8 v11 = vx_ತುಂಬು(src1 + j + VTraits<v_int8>::vlanes()),
                   v12 = vx_ತುಂಬು(src2 + j + VTraits<v_int8>::vlanes());
            r1 = v_dotprod_expand_fast(v_absdiff(v11, v12), one, r1);
        }
        s += v_reduce_ಮೊತ್ತ(v_ಕೂಡಿಸು(r0, r1));
        ಗಾಗಿ (; j < n; j++) {
            ಪೂರ್ಣಾಂಕ v = (ಪೂರ್ಣಾಂಕ)cv_absdiff(src1[j], src2[j]);
            s += v;
        }
        ಹಿಂತಿರುಗಿ s;
    }
};

ಮಾದರಿ<>
ರಚನೆ NormDiffL1_SIMD<ushort, ಪೂರ್ಣಾಂಕ> {
    ಪೂರ್ಣಾಂಕ ಕ್ರಿಯಾಕಾರಕ() (ಸ್ಥಿರಾಂಕ ushort* src1, ಸ್ಥಿರಾಂಕ ushort* src2, ಪೂರ್ಣಾಂಕ n) ಸ್ಥಿರಾಂಕ {
        ಪೂರ್ಣಾಂಕ j = 0;
        ಪೂರ್ಣಾಂಕ s = 0;
        v_uint32 r0 = vx_setzero_u32(), r1 = vx_setzero_u32();
        v_uint32 r2 = vx_setzero_u32(), r3 = vx_setzero_u32();
        ಗಾಗಿ (; j<= n - 4 * VTraits<v_uint16>::vlanes(); j += 4 * VTraits<v_uint16>::vlanes()) {
            v_uint16 v01 = vx_ತುಂಬು(src1 + j), v02 = vx_ತುಂಬು(src2 + j);
            v_uint32 u00, u01;
            v_expand(v_absdiff(v01, v02), u00, u01);
            r0 = v_ಕೂಡಿಸು(r0, v_ಕೂಡಿಸು(u00, u01));

            v_uint16 v11 = vx_ತುಂಬು(src1 + j + VTraits<v_uint16>::vlanes()),
                     v12 = vx_ತುಂಬು(src2 + j + VTraits<v_uint16>::vlanes());
            v_uint32 u10, u11;
            v_expand(v_absdiff(v11, v12), u10, u11);
            r1 = v_ಕೂಡಿಸು(r1, v_ಕೂಡಿಸು(u10, u11));

            v_uint16 v21 = vx_ತುಂಬು(src1 + j + 2 * VTraits<v_uint16>::vlanes()),
                     v22 = vx_ತುಂಬು(src2 + j + 2 * VTraits<v_uint16>::vlanes());
            v_uint32 u20, u21;
            v_expand(v_absdiff(v21, v22), u20, u21);
            r2 = v_ಕೂಡಿಸು(r2, v_ಕೂಡಿಸು(u20, u21));

            v_uint16 v31 = vx_ತುಂಬು(src1 + j + 3 * VTraits<v_uint16>::vlanes()),
                     v32 = vx_ತುಂಬು(src2 + j + 3 * VTraits<v_uint16>::vlanes());
            v_uint32 u30, u31;
            v_expand(v_absdiff(v31, v32), u30, u31);
            r3 = v_ಕೂಡಿಸು(r3, v_ಕೂಡಿಸು(u30, u31));
        }
        s += (ಪೂರ್ಣಾಂಕ)v_reduce_ಮೊತ್ತ(v_ಕೂಡಿಸು(v_ಕೂಡಿಸು(v_ಕೂಡಿಸು(r0, r1), r2), r3));
        ಗಾಗಿ (; j < n; j++) {
            ಪೂರ್ಣಾಂಕ v = (ಪೂರ್ಣಾಂಕ)cv_absdiff(src1[j], src2[j]);
            s += v;
        }
        ಹಿಂತಿರುಗಿ s;
    }
};

ಮಾದರಿ<>
ರಚನೆ NormDiffL1_SIMD<ಗಿಡ್ಡ, ಪೂರ್ಣಾಂಕ> {
    ಪೂರ್ಣಾಂಕ ಕ್ರಿಯಾಕಾರಕ() (ಸ್ಥಿರಾಂಕ ಗಿಡ್ಡ* src1, ಸ್ಥಿರಾಂಕ ಗಿಡ್ಡ* src2, ಪೂರ್ಣಾಂಕ n) ಸ್ಥಿರಾಂಕ {
        ಪೂರ್ಣಾಂಕ j = 0;
        ಪೂರ್ಣಾಂಕ s = 0;
        v_uint32 r0 = vx_setzero_u32(), r1 = vx_setzero_u32();
        v_uint32 r2 = vx_setzero_u32(), r3 = vx_setzero_u32();
        ಗಾಗಿ (; j<= n - 4 * VTraits<v_int16>::vlanes(); j += 4 * VTraits<v_int16>::vlanes()) {
            v_int16 v01 = vx_ತುಂಬು(src1 + j), v02 = vx_ತುಂಬು(src2 + j);
            v_uint32 u00, u01;
            v_expand(v_absdiff(v01, v02), u00, u01);
            r0 = v_ಕೂಡಿಸು(r0, v_ಕೂಡಿಸು(u00, u01));

            v_int16 v11 = vx_ತುಂಬು(src1 + j + VTraits<v_int16>::vlanes()),
                    v12 = vx_ತುಂಬು(src2 + j + VTraits<v_int16>::vlanes());
            v_uint32 u10, u11;
            v_expand(v_absdiff(v11, v12), u10, u11);
            r1 = v_ಕೂಡಿಸು(r1, v_ಕೂಡಿಸು(u10, u11));

            v_int16 v21 = vx_ತುಂಬು(src1 + j + 2 * VTraits<v_int16>::vlanes()),
                    v22 = vx_ತುಂಬು(src2 + j + 2 * VTraits<v_int16>::vlanes());
            v_uint32 u20, u21;
            v_expand(v_absdiff(v21, v22), u20, u21);
            r2 = v_ಕೂಡಿಸು(r2, v_ಕೂಡಿಸು(u20, u21));

            v_int16 v31 = vx_ತುಂಬು(src1 + j + 3 * VTraits<v_int16>::vlanes()),
                    v32 = vx_ತುಂಬು(src2 + j + 3 * VTraits<v_int16>::vlanes());
            v_uint32 u30, u31;
            v_expand(v_absdiff(v31, v32), u30, u31);
            r3 = v_ಕೂಡಿಸು(r3, v_ಕೂಡಿಸು(u30, u31));
        }
        s += (ಪೂರ್ಣಾಂಕ)v_reduce_ಮೊತ್ತ(v_ಕೂಡಿಸು(v_ಕೂಡಿಸು(v_ಕೂಡಿಸು(r0, r1), r2), r3));
        ಗಾಗಿ (; j < n; j++) {
            ಪೂರ್ಣಾಂಕ v = (ಪೂರ್ಣಾಂಕ)cv_absdiff(src1[j], src2[j]);
            s += v;
        }
        ಹಿಂತಿರುಗಿ s;
    }
};

ಮಾದರಿ<>
ರಚನೆ NormDiffL2_SIMD<uchar, ಪೂರ್ಣಾಂಕ> {
    ಪೂರ್ಣಾಂಕ ಕ್ರಿಯಾಕಾರಕ() (ಸ್ಥಿರಾಂಕ uchar* src1, ಸ್ಥಿರಾಂಕ uchar* src2, ಪೂರ್ಣಾಂಕ n) ಸ್ಥಿರಾಂಕ {
        ಪೂರ್ಣಾಂಕ j = 0;
        ಪೂರ್ಣಾಂಕ s = 0;
        v_uint32 r0 = vx_setzero_u32(), r1 = vx_setzero_u32();
        ಗಾಗಿ (; j <= n - 2 * VTraits<v_uint8>::vlanes(); j += 2 * VTraits<v_uint8>::vlanes()) {
            v_uint8 v01 = vx_ತುಂಬು(src1 + j), v02 = vx_ತುಂಬು(src2 + j);
            v_uint8 v0 = v_absdiff(v01, v02);
            r0 = v_dotprod_expand_fast(v0, v0, r0);

            v_uint8 v11 = vx_ತುಂಬು(src1 + j + VTraits<v_uint8>::vlanes()),
                    v12 = vx_ತುಂಬು(src2 + j + VTraits<v_uint8>::vlanes());
            v_uint8 v1 = v_absdiff(v11, v12);
            r1 = v_dotprod_expand_fast(v1, v1, r1);
        }
        s += v_reduce_ಮೊತ್ತ(v_ಕೂಡಿಸು(r0, r1));
        ಗಾಗಿ (; j < n; j++) {
            ಪೂರ್ಣಾಂಕ v = (ಪೂರ್ಣಾಂಕ)src1[j] - (ಪೂರ್ಣಾಂಕ)src2[j];
            s += v * v;
        }
        ಹಿಂತಿರುಗಿ s;
    }
};

ಮಾದರಿ<>
ರಚನೆ NormDiffL2_SIMD<schar, ಪೂರ್ಣಾಂಕ> {
    ಪೂರ್ಣಾಂಕ ಕ್ರಿಯಾಕಾರಕ() (ಸ್ಥಿರಾಂಕ schar* src1, ಸ್ಥಿರಾಂಕ schar* src2, ಪೂರ್ಣಾಂಕ n) ಸ್ಥಿರಾಂಕ {
        ಪೂರ್ಣಾಂಕ j = 0;
        ಪೂರ್ಣಾಂಕ s = 0;
        v_uint32 r0 = vx_setzero_u32(), r1 = vx_setzero_u32();
        ಗಾಗಿ (; j <= n - 2 * VTraits<v_int8>::vlanes(); j += 2 * VTraits<v_int8>::vlanes()) {
            v_int8 v01 = vx_ತುಂಬು(src1 + j), v02 = vx_ತುಂಬು(src2 + j);
            v_uint8 v0 = v_absdiff(v01, v02);
            r0 = v_dotprod_expand_fast(v0, v0, r0);

            v_int8 v11 = vx_ತುಂಬು(src1 + j + VTraits<v_int8>::vlanes()),
                   v12 = vx_ತುಂಬು(src2 + j + VTraits<v_int8>::vlanes());
            v_uint8 v1 = v_absdiff(v11, v12);
            r1 = v_dotprod_expand_fast(v1, v1, r1);
        }
        s += v_reduce_ಮೊತ್ತ(v_ಕೂಡಿಸು(r0, r1));
        ಗಾಗಿ (; j < n; j++) {
            ಪೂರ್ಣಾಂಕ v = (ಪೂರ್ಣಾಂಕ)src1[j] - (ಪೂರ್ಣಾಂಕ)src2[j];
            s += v * v;
        }
        ಹಿಂತಿರುಗಿ s;
    }
};

#ಆದರೆ_ಅಂತ್ಯ

#ಆದರೆ (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)

ಮಾದರಿ<>
ರಚನೆ NormInf_SIMD<ದ್ವಿಗುಣ, ದ್ವಿಗುಣ> {
    ದ್ವಿಗುಣ ಕ್ರಿಯಾಕಾರಕ() (ಸ್ಥಿರಾಂಕ ದ್ವಿಗುಣ* src, ಪೂರ್ಣಾಂಕ n) ಸ್ಥಿರಾಂಕ {
        ಪೂರ್ಣಾಂಕ j = 0;
        ದ್ವಿಗುಣ s = 0.f;
        v_float64 r0 = vx_setzero_f64(), r1 = vx_setzero_f64();
        v_float64 r2 = vx_setzero_f64(), r3 = vx_setzero_f64();
        ಗಾಗಿ (; j <= n - 4 * VTraits<v_float64>::vlanes(); j += 4 * VTraits<v_float64>::vlanes()) {
            r0 = v_max(r0, v_abs(vx_ತುಂಬು(src + j                                   )));
            r1 = v_max(r1, v_abs(vx_ತುಂಬು(src + j +     VTraits<v_float64>::vlanes())));
            r2 = v_max(r2, v_abs(vx_ತುಂಬು(src + j + 2 * VTraits<v_float64>::vlanes())));
            r3 = v_max(r3, v_abs(vx_ತುಂಬು(src + j + 3 * VTraits<v_float64>::vlanes())));
        }
        r0 = v_max(r0, v_max(r1, v_max(r2, r3)));
        ಗಾಗಿ (; j < n; j++) {
            s = std::max(s, cv_abs(src[j]));
        }
        // [TODO]: use v_reduce_max when it supports float64
        ದ್ವಿಗುಣ t[VTraits<v_float64>::max_nlanes];
        vx_store(t, r0);
        ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < VTraits<v_float64>::vlanes(); i++) {
            s = std::max(s, cv_abs(t[i]));
        }
        ಹಿಂತಿರುಗಿ s;
    }
};

ಮಾದರಿ<>
ರಚನೆ NormL1_SIMD<ಪೂರ್ಣಾಂಕ, ದ್ವಿಗುಣ> {
    ದ್ವಿಗುಣ ಕ್ರಿಯಾಕಾರಕ() (ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ* src, ಪೂರ್ಣಾಂಕ n) ಸ್ಥಿರಾಂಕ {
        ಪೂರ್ಣಾಂಕ j = 0;
        ದ್ವಿಗುಣ s = 0.f;
        v_float64 r00 = vx_setzero_f64(), r01 = vx_setzero_f64();
        v_float64 r10 = vx_setzero_f64(), r11 = vx_setzero_f64();
        ಗಾಗಿ (; j <= n - 2 * VTraits<v_int32>::vlanes(); j += 2 * VTraits<v_int32>::vlanes()) {
            // Widen to double before taking the magnitude: exact over the whole int32 range,
            // INT_MIN included, which a float intermediate would not be above 2^24.
            v_int32 i0 = vx_ತುಂಬು(src + j), i1 = vx_ತುಂಬು(src + j + VTraits<v_int32>::vlanes());
            r00 = v_ಕೂಡಿಸು(r00, v_abs(v_cvt_f64(i0))); r01 = v_ಕೂಡಿಸು(r01, v_abs(v_cvt_f64_ಮೇಲಿನ(i0)));
            r10 = v_ಕೂಡಿಸು(r10, v_abs(v_cvt_f64(i1))); r11 = v_ಕೂಡಿಸು(r11, v_abs(v_cvt_f64_ಮೇಲಿನ(i1)));
        }
        s += v_reduce_ಮೊತ್ತ(v_ಕೂಡಿಸು(v_ಕೂಡಿಸು(v_ಕೂಡಿಸು(r00, r01), r10), r11));
        ಗಾಗಿ (; j < n; j++) {
            s += cv_abs(src[j]);
        }
        ಹಿಂತಿರುಗಿ s;
    }
};

ಮಾದರಿ<>
ರಚನೆ NormL1_SIMD<ದಶಮಾಂಶ, ದ್ವಿಗುಣ> {
    ದ್ವಿಗುಣ ಕ್ರಿಯಾಕಾರಕ() (ಸ್ಥಿರಾಂಕ ದಶಮಾಂಶ* src, ಪೂರ್ಣಾಂಕ n) ಸ್ಥಿರಾಂಕ {
        ಪೂರ್ಣಾಂಕ j = 0;
        ದ್ವಿಗುಣ s = 0.f;
        v_float64 r00 = vx_setzero_f64(), r01 = vx_setzero_f64();
        v_float64 r10 = vx_setzero_f64(), r11 = vx_setzero_f64();
        v_float64 r20 = vx_setzero_f64(), r21 = vx_setzero_f64();
        v_float64 r30 = vx_setzero_f64(), r31 = vx_setzero_f64();
        ಗಾಗಿ (; j <= n - 4 * VTraits<v_float32>::vlanes(); j += 4 * VTraits<v_float32>::vlanes()) {
            v_float32 v0 = v_abs(vx_ತುಂಬು(src + j)), v1 = v_abs(vx_ತುಂಬು(src + j + VTraits<v_float32>::vlanes()));
            r00 = v_ಕೂಡಿಸು(r00, v_cvt_f64(v0)); r01 = v_ಕೂಡಿಸು(r01, v_cvt_f64_ಮೇಲಿನ(v0));
            r10 = v_ಕೂಡಿಸು(r10, v_cvt_f64(v1)); r11 = v_ಕೂಡಿಸು(r11, v_cvt_f64_ಮೇಲಿನ(v1));

            v_float32 v2 = v_abs(vx_ತುಂಬು(src + j + 2 * VTraits<v_float32>::vlanes())), v3 = v_abs(vx_ತುಂಬು(src + j + 3 * VTraits<v_float32>::vlanes()));
            r20 = v_ಕೂಡಿಸು(r20, v_cvt_f64(v2)); r21 = v_ಕೂಡಿಸು(r21, v_cvt_f64_ಮೇಲಿನ(v2));
            r30 = v_ಕೂಡಿಸು(r30, v_cvt_f64(v3)); r31 = v_ಕೂಡಿಸು(r31, v_cvt_f64_ಮೇಲಿನ(v3));
        }
        s += v_reduce_ಮೊತ್ತ(v_ಕೂಡಿಸು(v_ಕೂಡಿಸು(v_ಕೂಡಿಸು(r00, r01), r10), r11));
        s += v_reduce_ಮೊತ್ತ(v_ಕೂಡಿಸು(v_ಕೂಡಿಸು(v_ಕೂಡಿಸು(r20, r21), r30), r31));
        ಗಾಗಿ (; j < n; j++) {
            s += cv_abs(src[j]);
        }
        ಹಿಂತಿರುಗಿ s;
    }
};

ಮಾದರಿ<>
ರಚನೆ NormL2_SIMD<ushort, ದ್ವಿಗುಣ> {
    ದ್ವಿಗುಣ ಕ್ರಿಯಾಕಾರಕ() (ಸ್ಥಿರಾಂಕ ushort* src, ಪೂರ್ಣಾಂಕ n) ಸ್ಥಿರಾಂಕ {
        ಪೂರ್ಣಾಂಕ j = 0;
        ದ್ವಿಗುಣ s = 0.f;
        v_float64 r0 = vx_setzero_f64(), r1 = vx_setzero_f64();
        ಗಾಗಿ (; j <= n - 2 * VTraits<v_uint16>::vlanes(); j += 2 * VTraits<v_uint16>::vlanes()) {
            v_uint16 v0 = vx_ತುಂಬು(src + j);
            v_uint64 u0 = v_dotprod_expand_fast(v0, v0);
            r0 = v_ಕೂಡಿಸು(r0, v_cvt_f64(v_reinterpret_as_s64(u0)));

            v_uint16 v1 = vx_ತುಂಬು(src + j + VTraits<v_uint16>::vlanes());
            v_uint64 u1 = v_dotprod_expand_fast(v1, v1);
            r1 = v_ಕೂಡಿಸು(r1, v_cvt_f64(v_reinterpret_as_s64(u1)));
        }
        s += v_reduce_ಮೊತ್ತ(v_ಕೂಡಿಸು(r0, r1));
        ಗಾಗಿ (; j < n; j++) {
            ದ್ವಿಗುಣ v = saturate_cast<ದ್ವಿಗುಣ>(src[j]);
            s += v * v;
        }
        ಹಿಂತಿರುಗಿ s;
    }
};

ಮಾದರಿ<>
ರಚನೆ NormL2_SIMD<ಗಿಡ್ಡ, ದ್ವಿಗುಣ> {
    ದ್ವಿಗುಣ ಕ್ರಿಯಾಕಾರಕ() (ಸ್ಥಿರಾಂಕ ಗಿಡ್ಡ* src, ಪೂರ್ಣಾಂಕ n) ಸ್ಥಿರಾಂಕ {
        ಪೂರ್ಣಾಂಕ j = 0;
        ದ್ವಿಗುಣ s = 0.f;
        v_float64 r0 = vx_setzero_f64(), r1 = vx_setzero_f64();
        ಗಾಗಿ (; j <= n - 2 * VTraits<v_int16>::vlanes(); j += 2 * VTraits<v_int16>::vlanes()) {
            v_int16 v0 = vx_ತುಂಬು(src + j);
            r0 = v_ಕೂಡಿಸು(r0, v_cvt_f64(v_dotprod_expand_fast(v0, v0)));

            v_int16 v1 = vx_ತುಂಬು(src + j + VTraits<v_int16>::vlanes());
            r1 = v_ಕೂಡಿಸು(r1, v_cvt_f64(v_dotprod_expand_fast(v1, v1)));
        }
        s += v_reduce_ಮೊತ್ತ(v_ಕೂಡಿಸು(r0, r1));
        ಗಾಗಿ (; j < n; j++) {
            ದ್ವಿಗುಣ v = saturate_cast<ದ್ವಿಗುಣ>(src[j]);
            s += v * v;
        }
        ಹಿಂತಿರುಗಿ s;
    }
};

ಮಾದರಿ<>
ರಚನೆ NormL2_SIMD<ಪೂರ್ಣಾಂಕ, ದ್ವಿಗುಣ> {
    ದ್ವಿಗುಣ ಕ್ರಿಯಾಕಾರಕ() (ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ* src, ಪೂರ್ಣಾಂಕ n) ಸ್ಥಿರಾಂಕ {
        ಪೂರ್ಣಾಂಕ j = 0;
        ದ್ವಿಗುಣ s = 0.f;
        v_float64 r0 = vx_setzero_f64(), r1 = vx_setzero_f64();
        ಗಾಗಿ (; j <= n - 2 * VTraits<v_int32>::vlanes(); j += 2 * VTraits<v_int32>::vlanes()) {
            v_int32 v0 = vx_ತುಂಬು(src + j);
            r0 = v_dotprod_expand_fast(v0, v0, r0);

            v_int32 v1 = vx_ತುಂಬು(src + j + VTraits<v_int32>::vlanes());
            r1 = v_dotprod_expand_fast(v1, v1, r1);
        }
        s += v_reduce_ಮೊತ್ತ(v_ಕೂಡಿಸು(r0, r1));
        ಗಾಗಿ (; j < n; j++) {
            ದ್ವಿಗುಣ v = src[j];
            s += v * v;
        }
        ಹಿಂತಿರುಗಿ s;
    }
};

ಮಾದರಿ<>
ರಚನೆ NormL2_SIMD<ದಶಮಾಂಶ, ದ್ವಿಗುಣ> {
    ದ್ವಿಗುಣ ಕ್ರಿಯಾಕಾರಕ() (ಸ್ಥಿರಾಂಕ ದಶಮಾಂಶ* src, ಪೂರ್ಣಾಂಕ n) ಸ್ಥಿರಾಂಕ {
        ಪೂರ್ಣಾಂಕ j = 0;
        ದ್ವಿಗುಣ s = 0.f;
        v_float64 r00 = vx_setzero_f64(), r01 = vx_setzero_f64();
        v_float64 r10 = vx_setzero_f64(), r11 = vx_setzero_f64();
        ಗಾಗಿ (; j <= n - 2 * VTraits<v_float32>::vlanes(); j += 2 * VTraits<v_float32>::vlanes()) {
            v_float32 v0 = vx_ತುಂಬು(src + j), v1 = vx_ತುಂಬು(src + j + VTraits<v_float32>::vlanes());
            v_float64 v00 = v_cvt_f64(v0), v01 = v_cvt_f64_ಮೇಲಿನ(v0);
            v_float64 v10 = v_cvt_f64(v1), v11 = v_cvt_f64_ಮೇಲಿನ(v1);
            r00 = v_fma(v00, v00, r00); r01 = v_fma(v01, v01, r01);
            r10 = v_fma(v10, v10, r10); r11 = v_fma(v11, v11, r11);
        }
        s += v_reduce_ಮೊತ್ತ(v_ಕೂಡಿಸು(v_ಕೂಡಿಸು(v_ಕೂಡಿಸು(r00, r01), r10), r11));
        ಗಾಗಿ (; j < n; j++) {
            ದ್ವಿಗುಣ v = src[j];
            s += v * v;
        }
        ಹಿಂತಿರುಗಿ s;
    }
};

#ಆದರೆ_ಅಂತ್ಯ

#ಆದರೆ CV_SIMD_64F // CV_SIMD_SCALABLE_64F has accuracy problem with the following kernels on ci

ಮಾದರಿ<>
ರಚನೆ NormL1_SIMD<ದ್ವಿಗುಣ, ದ್ವಿಗುಣ> {
    ದ್ವಿಗುಣ ಕ್ರಿಯಾಕಾರಕ() (ಸ್ಥಿರಾಂಕ ದ್ವಿಗುಣ* src, ಪೂರ್ಣಾಂಕ n) ಸ್ಥಿರಾಂಕ {
        ಪೂರ್ಣಾಂಕ j = 0;
        ದ್ವಿಗುಣ s = 0.f;
        v_float64 r00 = vx_setzero_f64(), r01 = vx_setzero_f64();
        v_float64 r10 = vx_setzero_f64(), r11 = vx_setzero_f64();
        ಗಾಗಿ (; j <= n - 4 * VTraits<v_float64>::vlanes(); j += 4 * VTraits<v_float64>::vlanes()) {
            r00 = v_ಕೂಡಿಸು(r00, v_abs(vx_ತುಂಬು(src + j                                   )));
            r01 = v_ಕೂಡಿಸು(r01, v_abs(vx_ತುಂಬು(src + j +     VTraits<v_float64>::vlanes())));
            r10 = v_ಕೂಡಿಸು(r10, v_abs(vx_ತುಂಬು(src + j + 2 * VTraits<v_float64>::vlanes())));
            r11 = v_ಕೂಡಿಸು(r11, v_abs(vx_ತುಂಬು(src + j + 3 * VTraits<v_float64>::vlanes())));
        }
        s += v_reduce_ಮೊತ್ತ(v_ಕೂಡಿಸು(v_ಕೂಡಿಸು(v_ಕೂಡಿಸು(r00, r01), r10), r11));
        ಗಾಗಿ (; j < n; j++) {
            s += cv_abs(src[j]);
        }
        ಹಿಂತಿರುಗಿ s;
    }
};

ಮಾದರಿ<>
ರಚನೆ NormL2_SIMD<ದ್ವಿಗುಣ, ದ್ವಿಗುಣ> {
    ದ್ವಿಗುಣ ಕ್ರಿಯಾಕಾರಕ() (ಸ್ಥಿರಾಂಕ ದ್ವಿಗುಣ* src, ಪೂರ್ಣಾಂಕ n) ಸ್ಥಿರಾಂಕ {
        ಪೂರ್ಣಾಂಕ j = 0;
        ದ್ವಿಗುಣ s = 0.f;
        v_float64 r00 = vx_setzero_f64(), r01 = vx_setzero_f64();
        v_float64 r10 = vx_setzero_f64(), r11 = vx_setzero_f64();
        ಗಾಗಿ (; j <= n - 4 * VTraits<v_float64>::vlanes(); j += 4 * VTraits<v_float64>::vlanes()) {
            v_float64 v00 = vx_ತುಂಬು(src + j                                   );
            v_float64 v01 = vx_ತುಂಬು(src + j +     VTraits<v_float64>::vlanes());
            v_float64 v10 = vx_ತುಂಬು(src + j + 2 * VTraits<v_float64>::vlanes());
            v_float64 v11 = vx_ತುಂಬು(src + j + 3 * VTraits<v_float64>::vlanes());
            r00 = v_fma(v00, v00, r00); r01 = v_fma(v01, v01, r01);
            r10 = v_fma(v10, v10, r10); r11 = v_fma(v11, v11, r11);
        }
        s += v_reduce_ಮೊತ್ತ(v_ಕೂಡಿಸು(v_ಕೂಡಿಸು(v_ಕೂಡಿಸು(r00, r01), r10), r11));
        ಗಾಗಿ (; j < n; j++) {
            ದ್ವಿಗುಣ v = src[j];
            s += v * v;
        }
        ಹಿಂತಿರುಗಿ s;
    }
};

ಮಾದರಿ<>
ರಚನೆ NormDiffInf_SIMD<ದ್ವಿಗುಣ, ದ್ವಿಗುಣ> {
    ದ್ವಿಗುಣ ಕ್ರಿಯಾಕಾರಕ() (ಸ್ಥಿರಾಂಕ ದ್ವಿಗುಣ* src1, ಸ್ಥಿರಾಂಕ ದ್ವಿಗುಣ* src2, ಪೂರ್ಣಾಂಕ n) ಸ್ಥಿರಾಂಕ {
        ಪೂರ್ಣಾಂಕ j = 0;
        ದ್ವಿಗುಣ s = 0;
        v_float64 r0 = vx_setzero_f64(), r1 = vx_setzero_f64();
        ಗಾಗಿ (; j <= n - 2 * VTraits<v_float64>::vlanes(); j += 2 * VTraits<v_float64>::vlanes()) {
            v_float64 v01 = vx_ತುಂಬು(src1 + j), v02 = vx_ತುಂಬು(src2 + j);
            r0 = v_max(r0, v_absdiff(v01, v02));

            v_float64 v11 = vx_ತುಂಬು(src1 + j + VTraits<v_float64>::vlanes()),
                      v12 = vx_ತುಂಬು(src2 + j + VTraits<v_float64>::vlanes());
            r1 = v_max(r1, v_absdiff(v11, v12));
        }
        // [TODO]: use v_reduce_max when it supports float64
        ದ್ವಿಗುಣ t[VTraits<v_float64>::max_nlanes];
        vx_store(t, v_max(r0, r1));
        ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < VTraits<v_float64>::vlanes(); i++) {
            s = std::max(s, t[i]);
        }
        ಗಾಗಿ (; j < n; j++) {
            ದ್ವಿಗುಣ v = (ದ್ವಿಗುಣ)cv_absdiff(src1[j], src2[j]);
            s = std::max(s, v);
        }
        ಹಿಂತಿರುಗಿ s;
    }
};

ಮಾದರಿ<>
ರಚನೆ NormDiffL1_SIMD<ದಶಮಾಂಶ, ದ್ವಿಗುಣ> {
    ದ್ವಿಗುಣ ಕ್ರಿಯಾಕಾರಕ() (ಸ್ಥಿರಾಂಕ ದಶಮಾಂಶ* src1, ಸ್ಥಿರಾಂಕ ದಶಮಾಂಶ* src2, ಪೂರ್ಣಾಂಕ n) ಸ್ಥಿರಾಂಕ {
        ಪೂರ್ಣಾಂಕ j = 0;
        ದ್ವಿಗುಣ s = 0.f;
        v_float64 r0 = vx_setzero_f64(), r1 = vx_setzero_f64();
        v_float64 r2 = vx_setzero_f64(), r3 = vx_setzero_f64();
        ಗಾಗಿ (; j <= n - 2 * VTraits<v_float32>::vlanes(); j += 2 * VTraits<v_float32>::vlanes()) {
            v_float32 v01 = vx_ತುಂಬು(src1 + j), v02 = vx_ತುಂಬು(src2 + j);
            v_float32 v0 = v_absdiff(v01, v02);
            r0 = v_ಕೂಡಿಸು(r0, v_cvt_f64(v0)); r1 = v_ಕೂಡಿಸು(r1, v_cvt_f64_ಮೇಲಿನ(v0));

            v_float32 v11 = vx_ತುಂಬು(src1 + j + VTraits<v_float32>::vlanes()),
                      v12 = vx_ತುಂಬು(src2 + j + VTraits<v_float32>::vlanes());
            v_float32 v1 = v_absdiff(v11, v12);
            r2 = v_ಕೂಡಿಸು(r2, v_cvt_f64(v1)); r3 = v_ಕೂಡಿಸು(r3, v_cvt_f64_ಮೇಲಿನ(v1));
        }
        s += v_reduce_ಮೊತ್ತ(v_ಕೂಡಿಸು(v_ಕೂಡಿಸು(v_ಕೂಡಿಸು(r0, r1), r2), r3));
        ಗಾಗಿ (; j < n; j++) {
            ದ್ವಿಗುಣ v = (ದ್ವಿಗುಣ)cv_absdiff(src1[j], src2[j]);
            s += v;
        }
        ಹಿಂತಿರುಗಿ s;
    }
};

ಮಾದರಿ<>
ರಚನೆ NormDiffL1_SIMD<ದ್ವಿಗುಣ, ದ್ವಿಗುಣ> {
    ದ್ವಿಗುಣ ಕ್ರಿಯಾಕಾರಕ() (ಸ್ಥಿರಾಂಕ ದ್ವಿಗುಣ* src1, ಸ್ಥಿರಾಂಕ ದ್ವಿಗುಣ* src2, ಪೂರ್ಣಾಂಕ n) ಸ್ಥಿರಾಂಕ {
        ಪೂರ್ಣಾಂಕ j = 0;
        ದ್ವಿಗುಣ s = 0.f;
        v_float64 r0 = vx_setzero_f64(), r1 = vx_setzero_f64();
        ಗಾಗಿ (; j <= n - 2 * VTraits<v_float64>::vlanes(); j += 2 * VTraits<v_float64>::vlanes()) {
            v_float64 v01 = vx_ತುಂಬು(src1 + j), v02 = vx_ತುಂಬು(src2 + j);
            r0 = v_ಕೂಡಿಸು(r0, v_absdiff(v01, v02));

            v_float64 v11 = vx_ತುಂಬು(src1 + j + VTraits<v_float64>::vlanes()),
                      v12 = vx_ತುಂಬು(src2 + j + VTraits<v_float64>::vlanes());
            r1 = v_ಕೂಡಿಸು(r1, v_absdiff(v11, v12));
        }
        s += v_reduce_ಮೊತ್ತ(v_ಕೂಡಿಸು(r0, r1));
        ಗಾಗಿ (; j < n; j++) {
            ದ್ವಿಗುಣ v = (ದ್ವಿಗುಣ)cv_absdiff(src1[j], src2[j]);
            s += v;
        }
        ಹಿಂತಿರುಗಿ s;
    }
};

ಮಾದರಿ<>
ರಚನೆ NormDiffL2_SIMD<ushort, ದ್ವಿಗುಣ> {
    ದ್ವಿಗುಣ ಕ್ರಿಯಾಕಾರಕ() (ಸ್ಥಿರಾಂಕ ushort* src1, ಸ್ಥಿರಾಂಕ ushort* src2, ಪೂರ್ಣಾಂಕ n) ಸ್ಥಿರಾಂಕ {
        ಪೂರ್ಣಾಂಕ j = 0;
        ದ್ವಿಗುಣ s = 0.f;
        v_float64 r0 = vx_setzero_f64(), r1 = vx_setzero_f64();
        ಗಾಗಿ (; j <= n - 2 * VTraits<v_uint16>::vlanes(); j += 2 * VTraits<v_uint16>::vlanes()) {
            v_uint16 v01 = vx_ತುಂಬು(src1 + j), v02 = vx_ತುಂಬು(src2 + j);
            v_uint16 v0 = v_absdiff(v01, v02);
            v_uint64 u0 = v_dotprod_expand_fast(v0, v0);
            r0 = v_ಕೂಡಿಸು(r0, v_cvt_f64(v_reinterpret_as_s64(u0)));

            v_uint16 v11 = vx_ತುಂಬು(src1 + j + VTraits<v_uint16>::vlanes()),
                     v12 = vx_ತುಂಬು(src2 + j + VTraits<v_uint16>::vlanes());
            v_uint16 v1 = v_absdiff(v11, v12);
            v_uint64 u1 = v_dotprod_expand_fast(v1, v1);
            r1 = v_ಕೂಡಿಸು(r1, v_cvt_f64(v_reinterpret_as_s64(u1)));
        }
        s += v_reduce_ಮೊತ್ತ(v_ಕೂಡಿಸು(r0, r1));
        ಗಾಗಿ (; j < n; j++) {
            ದ್ವಿಗುಣ v = (ದ್ವಿಗುಣ)src1[j] - (ದ್ವಿಗುಣ)src2[j];
            s += v * v;
        }
        ಹಿಂತಿರುಗಿ s;
    }
};

ಮಾದರಿ<>
ರಚನೆ NormDiffL2_SIMD<ಗಿಡ್ಡ, ದ್ವಿಗುಣ> {
    ದ್ವಿಗುಣ ಕ್ರಿಯಾಕಾರಕ() (ಸ್ಥಿರಾಂಕ ಗಿಡ್ಡ* src1, ಸ್ಥಿರಾಂಕ ಗಿಡ್ಡ* src2, ಪೂರ್ಣಾಂಕ n) ಸ್ಥಿರಾಂಕ {
        ಪೂರ್ಣಾಂಕ j = 0;
        ದ್ವಿಗುಣ s = 0.f;
        v_float64 r0 = vx_setzero_f64(), r1 = vx_setzero_f64();
        ಗಾಗಿ (; j <= n - 2 * VTraits<v_int16>::vlanes(); j += 2 * VTraits<v_int16>::vlanes()) {
            v_int16 v01 = vx_ತುಂಬು(src1 + j), v02 = vx_ತುಂಬು(src2 + j);
            v_uint16 v0 = v_absdiff(v01, v02);
            v_uint64 u0 = v_dotprod_expand_fast(v0, v0);
            r0 = v_ಕೂಡಿಸು(r0, v_cvt_f64(v_reinterpret_as_s64(u0)));

            v_int16 v11 = vx_ತುಂಬು(src1 + j + VTraits<v_uint16>::vlanes()),
                    v12 = vx_ತುಂಬು(src2 + j + VTraits<v_uint16>::vlanes());
            v_uint16 v1 = v_absdiff(v11, v12);
            v_uint64 u1 = v_dotprod_expand_fast(v1, v1);
            r1 = v_ಕೂಡಿಸು(r1, v_cvt_f64(v_reinterpret_as_s64(u1)));
        }
        s += v_reduce_ಮೊತ್ತ(v_ಕೂಡಿಸು(r0, r1));
        ಗಾಗಿ (; j < n; j++) {
            ದ್ವಿಗುಣ v = (ದ್ವಿಗುಣ)src1[j] - (ದ್ವಿಗುಣ)src2[j];
            s += v * v;
        }
        ಹಿಂತಿರುಗಿ s;
    }
};

ಮಾದರಿ<>
ರಚನೆ NormDiffL2_SIMD<ಪೂರ್ಣಾಂಕ, ದ್ವಿಗುಣ> {
    ದ್ವಿಗುಣ ಕ್ರಿಯಾಕಾರಕ() (ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ* src1, ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ* src2, ಪೂರ್ಣಾಂಕ n) ಸ್ಥಿರಾಂಕ {
        ಪೂರ್ಣಾಂಕ j = 0;
        ದ್ವಿಗುಣ s = 0.f;
        v_float64 r0 = vx_setzero_f64(), r1 = vx_setzero_f64();
        ಗಾಗಿ (; j <= n - VTraits<v_int32>::vlanes(); j += VTraits<v_int32>::vlanes()) {
            v_int32 v01 = vx_ತುಂಬು(src1 + j), v02 = vx_ತುಂಬು(src2 + j);
            v_uint32 v0 = v_absdiff(v01, v02);
            v_uint64 ev0, ev1;
            v_expand(v0, ev0, ev1);
            v_float64 f0 = v_cvt_f64(v_reinterpret_as_s64(ev0)), f1 = v_cvt_f64(v_reinterpret_as_s64(ev1));
            r0 = v_fma(f0, f0, r0); r1 = v_fma(f1, f1, r1);
        }
        s += v_reduce_ಮೊತ್ತ(v_ಕೂಡಿಸು(r0, r1));
        ಗಾಗಿ (; j < n; j++) {
            ದ್ವಿಗುಣ v = (ದ್ವಿಗುಣ)src1[j] - (ದ್ವಿಗುಣ)src2[j];
            s += v * v;
        }
        ಹಿಂತಿರುಗಿ s;
    }
};

ಮಾದರಿ<>
ರಚನೆ NormDiffL2_SIMD<ದಶಮಾಂಶ, ದ್ವಿಗುಣ> {
    ದ್ವಿಗುಣ ಕ್ರಿಯಾಕಾರಕ() (ಸ್ಥಿರಾಂಕ ದಶಮಾಂಶ* src1, ಸ್ಥಿರಾಂಕ ದಶಮಾಂಶ* src2, ಪೂರ್ಣಾಂಕ n) ಸ್ಥಿರಾಂಕ {
        ಪೂರ್ಣಾಂಕ j = 0;
        ದ್ವಿಗುಣ s = 0.f;
        v_float64 r0 = vx_setzero_f64(), r1 = vx_setzero_f64();
        v_float64 r2 = vx_setzero_f64(), r3 = vx_setzero_f64();
        ಗಾಗಿ (; j <= n - 2 * VTraits<v_float32>::vlanes(); j += 2 * VTraits<v_float32>::vlanes()) {
            v_float32 v01 = vx_ತುಂಬು(src1 + j), v02 = vx_ತುಂಬು(src2 + j);
            v_float32 v0 = v_absdiff(v01, v02);
            v_float64 f01 = v_cvt_f64(v0), f02 = v_cvt_f64_ಮೇಲಿನ(v0);
            r0 = v_fma(f01, f01, r0); r1 = v_fma(f02, f02, r1);

            v_float32 v11 = vx_ತುಂಬು(src1 + j + VTraits<v_float32>::vlanes()),
                      v12 = vx_ತುಂಬು(src2 + j + VTraits<v_float32>::vlanes());
            v_float32 v1 = v_absdiff(v11, v12);
            v_float64 f11 = v_cvt_f64(v1), f12 = v_cvt_f64_ಮೇಲಿನ(v1);
            r2 = v_fma(f11, f11, r2); r3 = v_fma(f12, f12, r3);
        }
        s += v_reduce_ಮೊತ್ತ(v_ಕೂಡಿಸು(v_ಕೂಡಿಸು(v_ಕೂಡಿಸು(r0, r1), r2), r3));
        ಗಾಗಿ (; j < n; j++) {
            ದ್ವಿಗುಣ v = (ದ್ವಿಗುಣ)src1[j] - (ದ್ವಿಗುಣ)src2[j];
            s += v * v;
        }
        ಹಿಂತಿರುಗಿ s;
    }
};

ಮಾದರಿ<>
ರಚನೆ NormDiffL2_SIMD<ದ್ವಿಗುಣ, ದ್ವಿಗುಣ> {
    ದ್ವಿಗುಣ ಕ್ರಿಯಾಕಾರಕ() (ಸ್ಥಿರಾಂಕ ದ್ವಿಗುಣ* src1, ಸ್ಥಿರಾಂಕ ದ್ವಿಗುಣ* src2, ಪೂರ್ಣಾಂಕ n) ಸ್ಥಿರಾಂಕ {
        ಪೂರ್ಣಾಂಕ j = 0;
        ದ್ವಿಗುಣ s = 0.f;
        v_float64 r0 = vx_setzero_f64(), r1 = vx_setzero_f64();
        v_float64 r2 = vx_setzero_f64(), r3 = vx_setzero_f64();
        ಗಾಗಿ (; j <= n - 4 * VTraits<v_float64>::vlanes(); j += 4 * VTraits<v_float64>::vlanes()) {
            v_float64 v01 = vx_ತುಂಬು(src1 + j), v02 = vx_ತುಂಬು(src2 + j);
            v_float64 v0 = v_absdiff(v01, v02);
            r0 = v_fma(v0, v0, r0);

            v_float64 v11 = vx_ತುಂಬು(src1 + j + VTraits<v_float64>::vlanes()),
                      v12 = vx_ತುಂಬು(src2 + j + VTraits<v_float64>::vlanes());
            v_float64 v1 = v_absdiff(v11, v12);
            r1 = v_fma(v1, v1, r1);

            v_float64 v21 = vx_ತುಂಬು(src1 + j + 2 * VTraits<v_float64>::vlanes()),
                      v22 = vx_ತುಂಬು(src2 + j + 2 * VTraits<v_float64>::vlanes());
            v_float64 v2 = v_absdiff(v21, v22);
            r2 = v_fma(v2, v2, r2);

            v_float64 v31 = vx_ತುಂಬು(src1 + j + 3 * VTraits<v_float64>::vlanes()),
                      v32 = vx_ತುಂಬು(src2 + j + 3 * VTraits<v_float64>::vlanes());
            v_float64 v3 = v_absdiff(v31, v32);
            r3 = v_fma(v3, v3, r3);
        }
        s += v_reduce_ಮೊತ್ತ(v_ಕೂಡಿಸು(v_ಕೂಡಿಸು(v_ಕೂಡಿಸು(r0, r1), r2), r3));
        ಗಾಗಿ (; j < n; j++) {
            ದ್ವಿಗುಣ v = (ದ್ವಿಗುಣ)src1[j] - (ದ್ವಿಗುಣ)src2[j];
            s += v * v;
        }
        ಹಿಂತಿರುಗಿ s;
    }
};

#ಆದರೆ_ಅಂತ್ಯ

ಮಾದರಿ <ಬಗೆ_ಹೆಸರು T, ಬಗೆ_ಹೆಸರು ST>
ರಚನೆ MaskedNormInf_SIMD {
    ಸಾಲೊಳಗೆ ST ಕ್ರಿಯಾಕಾರಕ() (ಸ್ಥಿರಾಂಕ T* src, ಸ್ಥಿರಾಂಕ uchar* ಮುಖವಾಡ, ಪೂರ್ಣಾಂಕ len, ಪೂರ್ಣಾಂಕ cn) ಸ್ಥಿರಾಂಕ {
        ST s = 0;
        ಆದರೆ (cn == 1) {
            ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < len; i++) {
                ಆದರೆ (ಮುಖವಾಡ[i]) {
                    s = std::max(s, (ST)cv_abs(src[i]));
                }
            }
        }
        ಇಲ್ಲದಿದ್ದರೆ {
            ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < len; i++) {
                ಆದರೆ (ಮುಖವಾಡ[i]) {
                    ಸ್ಥಿರಾಂಕ T* elem = src + i * cn;
                    ಪೂರ್ಣಾಂಕ k = 0;
                #ಆದರೆ CV_ENABLE_UNROLLED && !CV_SIMD_SCALABLE
                    ಗಾಗಿ (; k <= cn - 4; k += 4) {
                        s = std::max(s, (ST)cv_abs(elem[k]));
                        s = std::max(s, (ST)cv_abs(elem[k + 1]));
                        s = std::max(s, (ST)cv_abs(elem[k + 2]));
                        s = std::max(s, (ST)cv_abs(elem[k + 3]));
                    }
                #ಆದರೆ_ಅಂತ್ಯ
                    ಗಾಗಿ (; k < cn; k++) {
                        s = std::max(s, (ST)cv_abs(elem[k]));
                    }
                }
            }
        }
        ಹಿಂತಿರುಗಿ s;
    }
};

ಮಾದರಿ <ಬಗೆ_ಹೆಸರು T, ಬಗೆ_ಹೆಸರು ST>
ರಚನೆ MaskedNormL1_SIMD {
    ಸಾಲೊಳಗೆ ST ಕ್ರಿಯಾಕಾರಕ() (ಸ್ಥಿರಾಂಕ T* src, ಸ್ಥಿರಾಂಕ uchar* ಮುಖವಾಡ, ಪೂರ್ಣಾಂಕ len, ಪೂರ್ಣಾಂಕ cn) ಸ್ಥಿರಾಂಕ {
        ST s = 0;
        ಆದರೆ (cn == 1) {
            ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < len; i++) {
                ಆದರೆ (ಮುಖವಾಡ[i]) {
                    s += (ST)cv_abs(src[i]);
                }
            }
        }
        ಇಲ್ಲದಿದ್ದರೆ {
            ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < len; i++) {
                ಆದರೆ (ಮುಖವಾಡ[i]) {
                    ಸ್ಥಿರಾಂಕ T* elem = src + i * cn;
                    ಪೂರ್ಣಾಂಕ k = 0;
                #ಆದರೆ CV_ENABLE_UNROLLED && !CV_SIMD_SCALABLE
                    ಗಾಗಿ (; k <= cn - 4; k += 4) {
                        s += (ST)cv_abs(elem[k]);
                        s += (ST)cv_abs(elem[k + 1]);
                        s += (ST)cv_abs(elem[k + 2]);
                        s += (ST)cv_abs(elem[k + 3]);
                    }
                #ಆದರೆ_ಅಂತ್ಯ
                    ಗಾಗಿ (; k < cn; k++) {
                        s += (ST)cv_abs(elem[k]);
                    }
                }
            }
        }
        ಹಿಂತಿರುಗಿ s;
    }
};

ಮಾದರಿ <ಬಗೆ_ಹೆಸರು T, ಬಗೆ_ಹೆಸರು ST>
ರಚನೆ MaskedNormL2_SIMD {
    ಸಾಲೊಳಗೆ ST ಕ್ರಿಯಾಕಾರಕ() (ಸ್ಥಿರಾಂಕ T* src, ಸ್ಥಿರಾಂಕ uchar* ಮುಖವಾಡ, ಪೂರ್ಣಾಂಕ len, ಪೂರ್ಣಾಂಕ cn) ಸ್ಥಿರಾಂಕ {
        ST s = 0;
        ಆದರೆ (cn == 1) {
            ಪೂರ್ಣಾಂಕ i = 0;
        #ಆದರೆ CV_ENABLE_UNROLLED && !CV_SIMD_SCALABLE
            ಗಾಗಿ (; i <= len - 4; i += 4) {
                ಆದರೆ (ಮುಖವಾಡ[i])     { T v0 = src[i];     s += (ST)v0 * v0; }
                ಆದರೆ (ಮುಖವಾಡ[i + 1]) { T v1 = src[i + 1]; s += (ST)v1 * v1; }
                ಆದರೆ (ಮುಖವಾಡ[i + 2]) { T v2 = src[i + 2]; s += (ST)v2 * v2; }
                ಆದರೆ (ಮುಖವಾಡ[i + 3]) { T v3 = src[i + 3]; s += (ST)v3 * v3; }
            }
        #ಆದರೆ_ಅಂತ್ಯ
            ಗಾಗಿ (; i < len; i++) {
                ಆದರೆ (ಮುಖವಾಡ[i]) {
                    T v = src[i];
                    s += (ST)v * v;
                }
            }
        }
        ಇಲ್ಲದಿದ್ದರೆ {
            ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < len; i++) {
                ಆದರೆ (ಮುಖವಾಡ[i]) {
                    ಸ್ಥಿರಾಂಕ T* elem = src + i * cn;
                    ಪೂರ್ಣಾಂಕ k = 0;
                #ಆದರೆ CV_ENABLE_UNROLLED && !CV_SIMD_SCALABLE
                    ಗಾಗಿ (; k <= cn - 4; k += 4) {
                        T v0 = elem[k];     s += (ST)v0 * v0;
                        T v1 = elem[k + 1]; s += (ST)v1 * v1;
                        T v2 = elem[k + 2]; s += (ST)v2 * v2;
                        T v3 = elem[k + 3]; s += (ST)v3 * v3;
                    }
                #ಆದರೆ_ಅಂತ್ಯ
                    ಗಾಗಿ (; k < cn; k++) {
                        T v = elem[k];
                        s += (ST)v * v;
                    }
                }
            }
        }

        ಹಿಂತಿರುಗಿ s;
    }
};

#ಆದರೆ (CV_SIMD || CV_SIMD_SCALABLE)
ಸ್ಥಿರ ಸಾಲೊಳಗೆ v_uint8  v_normmask_u8 (ಸ್ಥಿರಾಂಕ uchar* m) { ಹಿಂತಿರುಗಿ v_gt(vx_ತುಂಬು(m),          vx_setzero_u8());  }
ಸ್ಥಿರ ಸಾಲೊಳಗೆ v_uint16 v_normmask_u16(ಸ್ಥಿರಾಂಕ uchar* m) { ಹಿಂತಿರುಗಿ v_gt(vx_ತುಂಬು_expand(m),   vx_setzero_u16()); }
ಸ್ಥಿರ ಸಾಲೊಳಗೆ v_uint32 v_normmask_u32(ಸ್ಥಿರಾಂಕ uchar* m) { ಹಿಂತಿರುಗಿ v_gt(vx_ತುಂಬು_expand_q(m), vx_setzero_u32()); }
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ v_normmask_u64(ಸ್ಥಿರಾಂಕ uchar* m, v_uint64& m0, v_uint64& m1) {
    v_uint64 e0, e1;
    v_expand(v_normmask_u32(m), e0, e1);
    m0 = v_or(e0, v_shl<32>(e0));
    m1 = v_or(e1, v_shl<32>(e1));
}

ಮಾದರಿ <>
ರಚನೆ MaskedNormInf_SIMD<ದಶಮಾಂಶ, ದಶಮಾಂಶ> {
    ಸಾಲೊಳಗೆ ದಶಮಾಂಶ ಕ್ರಿಯಾಕಾರಕ()(ಸ್ಥಿರಾಂಕ ದಶಮಾಂಶ* src, ಸ್ಥಿರಾಂಕ uchar* ಮುಖವಾಡ, ಪೂರ್ಣಾಂಕ len, ಪೂರ್ಣಾಂಕ cn) ಸ್ಥಿರಾಂಕ {
        ದಶಮಾಂಶ ಫಲಿತಾಂಶ = 0.0f;
        ಆದರೆ (cn == 1) {
            ಪೂರ್ಣಾಂಕ i = 0;
            ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ vstep = VTraits<v_float32>::vlanes();
            v_float32 acc = vx_setzero_f32();

            ಗಾಗಿ (; i <= len - vstep; i += vstep) {
                v_uint32 m = vx_ತುಂಬು_expand_q(ಮುಖವಾಡ + i);
                v_uint32 cmp = v_gt(m, vx_setzero_u32());
                v_float32 s = vx_ತುಂಬು(src + i);
                s = v_abs(s);
                s = v_reinterpret_as_f32(v_and(v_reinterpret_as_u32(s), cmp));
                acc = v_max(acc, s);
            }
            ಫಲಿತಾಂಶ = v_reduce_max(acc);

            ಗಾಗಿ (; i < len; i++) {
                ಆದರೆ (ಮುಖವಾಡ[i])
                    ಫಲಿತಾಂಶ = std::max(ಫಲಿತಾಂಶ, std::ನಿರಪೇಕ್ಷ(src[i]));
            }
        }
        ಇಲ್ಲದಿದ್ದರೆ {
            ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < len; i++) {
                ಆದರೆ (ಮುಖವಾಡ[i]) {
                    ಸ್ಥಿರಾಂಕ ದಶಮಾಂಶ* elem = src + i * cn;
                    ಪೂರ್ಣಾಂಕ k = 0;
                    ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ vstep = VTraits<v_float32>::vlanes();
                    v_float32 acc = vx_setzero_f32();

                    ಗಾಗಿ (; k <= cn - vstep; k += vstep) {
                        v_float32 s = vx_ತುಂಬು(elem + k);
                        acc = v_max(acc, v_abs(s));
                    }

                    ಫಲಿತಾಂಶ = std::max(ಫಲಿತಾಂಶ, v_reduce_max(acc));

                    ಗಾಗಿ (; k < cn; k++)
                        ಫಲಿತಾಂಶ = std::max(ಫಲಿತಾಂಶ, std::ನಿರಪೇಕ್ಷ(elem[k]));
                }
            }
        }
        ಹಿಂತಿರುಗಿ ಫಲಿತಾಂಶ;
    }
};

#ಆದರೆ CV_SIMD_64F
ಮಾದರಿ <>
ರಚನೆ MaskedNormL1_SIMD<ದಶಮಾಂಶ, ದ್ವಿಗುಣ> {
    ಸಾಲೊಳಗೆ ದ್ವಿಗುಣ ಕ್ರಿಯಾಕಾರಕ()(ಸ್ಥಿರಾಂಕ ದಶಮಾಂಶ* src, ಸ್ಥಿರಾಂಕ uchar* ಮುಖವಾಡ, ಪೂರ್ಣಾಂಕ len, ಪೂರ್ಣಾಂಕ cn) ಸ್ಥಿರಾಂಕ {
        ದ್ವಿಗುಣ ಫಲಿತಾಂಶ = 0.0;
        ಆದರೆ (cn == 1) {
            ಪೂರ್ಣಾಂಕ i = 0;
            ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ vstep = VTraits<v_float32>::vlanes();
            v_float64 acc0 = vx_setzero_f64(), acc1 = vx_setzero_f64();
            v_float64 acc2 = vx_setzero_f64(), acc3 = vx_setzero_f64();

            ಗಾಗಿ (; i <= len - 2*vstep; i += 2*vstep) {
                v_float32 s0 = v_and(v_abs(vx_ತುಂಬು(src + i)),
                                     v_reinterpret_as_f32(v_normmask_u32(ಮುಖವಾಡ + i)));
                v_float32 s1 = v_and(v_abs(vx_ತುಂಬು(src + i + vstep)),
                                     v_reinterpret_as_f32(v_normmask_u32(ಮುಖವಾಡ + i + vstep)));
                acc0 = v_ಕೂಡಿಸು(acc0, v_cvt_f64(s0));
                acc1 = v_ಕೂಡಿಸು(acc1, v_cvt_f64_ಮೇಲಿನ(s0));
                acc2 = v_ಕೂಡಿಸು(acc2, v_cvt_f64(s1));
                acc3 = v_ಕೂಡಿಸು(acc3, v_cvt_f64_ಮೇಲಿನ(s1));
            }
            ಗಾಗಿ (; i <= len - vstep; i += vstep) {
                v_float32 s = v_and(v_abs(vx_ತುಂಬು(src + i)),
                                    v_reinterpret_as_f32(v_normmask_u32(ಮುಖವಾಡ + i)));
                acc0 = v_ಕೂಡಿಸು(acc0, v_cvt_f64(s));
                acc1 = v_ಕೂಡಿಸು(acc1, v_cvt_f64_ಮೇಲಿನ(s));
            }
            ಫಲಿತಾಂಶ = v_reduce_ಮೊತ್ತ(v_ಕೂಡಿಸು(v_ಕೂಡಿಸು(acc0, acc1), v_ಕೂಡಿಸು(acc2, acc3)));

            ಗಾಗಿ (; i < len; i++) {
                ಆದರೆ (ಮುಖವಾಡ[i])
                    ಫಲಿತಾಂಶ += std::ನಿರಪೇಕ್ಷ(src[i]);
            }
        }
        ಇಲ್ಲದಿದ್ದರೆ {
            ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < len; i++) {
                ಆದರೆ (ಮುಖವಾಡ[i]) {
                    ಸ್ಥಿರಾಂಕ ದಶಮಾಂಶ* elem = src + i * cn;
                    ಪೂರ್ಣಾಂಕ k = 0;
                    ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ vstep = VTraits<v_float32>::vlanes();
                    v_float64 acc = vx_setzero_f64();

                    ಗಾಗಿ (; k <= cn - vstep; k += vstep) {
                        v_float32 s = v_abs(vx_ತುಂಬು(elem + k));
                        acc = v_ಕೂಡಿಸು(acc, v_cvt_f64(s));
                        acc = v_ಕೂಡಿಸು(acc, v_cvt_f64_ಮೇಲಿನ(s));
                    }

                    ಫಲಿತಾಂಶ += v_reduce_ಮೊತ್ತ(acc);

                    ಗಾಗಿ (; k < cn; k++)
                        ಫಲಿತಾಂಶ += std::ನಿರಪೇಕ್ಷ(elem[k]);
                }
            }
        }
        ಹಿಂತಿರುಗಿ ಫಲಿತಾಂಶ;
    }
};

ಮಾದರಿ <>
ರಚನೆ MaskedNormL2_SIMD<ದಶಮಾಂಶ, ದ್ವಿಗುಣ> {
    ಸಾಲೊಳಗೆ ದ್ವಿಗುಣ ಕ್ರಿಯಾಕಾರಕ()(ಸ್ಥಿರಾಂಕ ದಶಮಾಂಶ* src, ಸ್ಥಿರಾಂಕ uchar* ಮುಖವಾಡ, ಪೂರ್ಣಾಂಕ len, ಪೂರ್ಣಾಂಕ cn) ಸ್ಥಿರಾಂಕ {
        ದ್ವಿಗುಣ ಫಲಿತಾಂಶ = 0.0;
        ಆದರೆ (cn == 1) {
            ಪೂರ್ಣಾಂಕ i = 0;
            ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ vstep = VTraits<v_float32>::vlanes();
            v_float32 facc = vx_setzero_f32();
            v_float64 dacc = vx_setzero_f64();
            ಪೂರ್ಣಾಂಕ flush = 0;

            ಗಾಗಿ (; i <= len - vstep; i += vstep, flush += vstep) {
                ಆದರೆ (flush >= 64) {
                    dacc = v_ಕೂಡಿಸು(dacc, v_cvt_f64(facc));
                    dacc = v_ಕೂಡಿಸು(dacc, v_cvt_f64_ಮೇಲಿನ(facc));
                    facc = vx_setzero_f32();
                    flush = 0;
                }
                v_uint32 cmp = v_gt(vx_ತುಂಬು_expand_q(ಮುಖವಾಡ + i), vx_setzero_u32());
                v_float32 s  = v_reinterpret_as_f32(v_and(v_reinterpret_as_u32(vx_ತುಂಬು(src + i)), cmp));
                facc = v_ಕೂಡಿಸು(facc, v_mul(s, s));
            }

            dacc = v_ಕೂಡಿಸು(dacc, v_cvt_f64(facc));
            dacc = v_ಕೂಡಿಸು(dacc, v_cvt_f64_ಮೇಲಿನ(facc));
            ಫಲಿತಾಂಶ = v_reduce_ಮೊತ್ತ(dacc);

            ಗಾಗಿ (; i < len; i++) {
                ಆದರೆ (ಮುಖವಾಡ[i]) {
                    ದ್ವಿಗುಣ v = src[i];
                    ಫಲಿತಾಂಶ += v * v;
                }
            }
        }
        ಇಲ್ಲದಿದ್ದರೆ {
            ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < len; i++) {
                ಆದರೆ (ಮುಖವಾಡ[i]) {
                    ಸ್ಥಿರಾಂಕ ದಶಮಾಂಶ* elem = src + i * cn;
                    ಪೂರ್ಣಾಂಕ k = 0;
                    ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ vstep = VTraits<v_float32>::vlanes();
                    v_float32 facc = vx_setzero_f32();

                    ಗಾಗಿ (; k <= cn - vstep; k += vstep) {
                        v_float32 s = vx_ತುಂಬು(elem + k);
                        facc = v_ಕೂಡಿಸು(facc, v_mul(s, s));
                    }

                    v_float64 dacc = v_ಕೂಡಿಸು(v_cvt_f64(facc), v_cvt_f64_ಮೇಲಿನ(facc));
                    ಫಲಿತಾಂಶ += v_reduce_ಮೊತ್ತ(dacc);

                    ಗಾಗಿ (; k < cn; k++) {
                        ದ್ವಿಗುಣ v = elem[k];
                        ಫಲಿತಾಂಶ += v * v;
                    }
                }
            }
        }
        ಹಿಂತಿರುಗಿ ಫಲಿತಾಂಶ;
    }
};

#ಆದರೆ_ಅಂತ್ಯ

ಮಾದರಿ <>
ರಚನೆ MaskedNormInf_SIMD<uchar, ಪೂರ್ಣಾಂಕ> {
    ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ ಕ್ರಿಯಾಕಾರಕ()(ಸ್ಥಿರಾಂಕ uchar* src, ಸ್ಥಿರಾಂಕ uchar* ಮುಖವಾಡ, ಪೂರ್ಣಾಂಕ len, ಪೂರ್ಣಾಂಕ cn) ಸ್ಥಿರಾಂಕ {
        ಪೂರ್ಣಾಂಕ ಫಲಿತಾಂಶ = 0;
        ಆದರೆ (cn == 1) {
            ಪೂರ್ಣಾಂಕ i = 0;
            ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ vstep = VTraits<v_uint8>::vlanes();
            v_uint8 acc = vx_setzero_u8();

            ಗಾಗಿ (; i <= len - vstep; i += vstep) {
                v_uint8 m   = vx_ತುಂಬು(ಮುಖವಾಡ + i);
                v_uint8 s   = vx_ತುಂಬು(src + i);
                v_uint8 sel = v_and(s, v_gt(m, vx_setzero_u8()));
                acc = v_max(acc, sel);
            }

            ಫಲಿತಾಂಶ = (ಪೂರ್ಣಾಂಕ)v_reduce_max(acc);

            ಗಾಗಿ (; i < len; i++) {
                ಆದರೆ (ಮುಖವಾಡ[i])
                    ಫಲಿತಾಂಶ = std::max(ಫಲಿತಾಂಶ, (ಪೂರ್ಣಾಂಕ)src[i]);
            }
        }
        ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (cn == 4 && len >= VTraits<v_uint8>::vlanes()) {
            ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ vstep = VTraits<v_uint8>::vlanes();
            v_uint8 acc = vx_setzero_u8();
            ಪೂರ್ಣಾಂಕ i = 0;
            ಗಾಗಿ (;;) {
                ಆದರೆ (i > len - vstep)
                    i = len - vstep; // back-step (max is idempotent)
                v_uint8 c0, c1, c2, c3;
                v_ತುಂಬು_deinterleave(src + i * 4, c0, c1, c2, c3);
                v_uint8 pmax = v_max(v_max(c0, c1), v_max(c2, c3));
                v_uint8 m = v_gt(vx_ತುಂಬು(ಮುಖವಾಡ + i), vx_setzero_u8());
                acc = v_max(acc, v_and(pmax, m));
                ಆದರೆ (i >= len - vstep)
                    ನಿಲ್ಲಿಸು;
                i += vstep;
            }
            ಫಲಿತಾಂಶ = (ಪೂರ್ಣಾಂಕ)v_reduce_max(acc);
        }
        ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (cn == 4) {
            ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < len; i++) {
                ಆದರೆ (ಮುಖವಾಡ[i]) {
                    ಸ್ಥಿರಾಂಕ uchar* elem = src + i * 4;
                    ಫಲಿತಾಂಶ = std::max(ಫಲಿತಾಂಶ, (ಪೂರ್ಣಾಂಕ)std::max(std::max(elem[0], elem[1]),
                                                            std::max(elem[2], elem[3])));
                }
            }
        }
        ಇಲ್ಲದಿದ್ದರೆ {
            ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < len; i++) {
                ಆದರೆ (ಮುಖವಾಡ[i]) {
                    ಸ್ಥಿರಾಂಕ uchar* elem = src + i * cn;
                    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ k = 0; k < cn; k++)
                        ಫಲಿತಾಂಶ = std::max(ಫಲಿತಾಂಶ, (ಪೂರ್ಣಾಂಕ)elem[k]);
                }
            }
        }
        ಹಿಂತಿರುಗಿ ಫಲಿತಾಂಶ;
    }
};

ಮಾದರಿ <>
ರಚನೆ MaskedNormL1_SIMD<uchar, ಪೂರ್ಣಾಂಕ> {
    ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ ಕ್ರಿಯಾಕಾರಕ()(ಸ್ಥಿರಾಂಕ uchar* src, ಸ್ಥಿರಾಂಕ uchar* ಮುಖವಾಡ, ಪೂರ್ಣಾಂಕ len, ಪೂರ್ಣಾಂಕ cn) ಸ್ಥಿರಾಂಕ {
        ಪೂರ್ಣಾಂಕ ಫಲಿತಾಂಶ = 0;
        ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ vstep = VTraits<v_uint8>::vlanes();
        ಸ್ಥಿರಾಂಕ v_uint8 one = vx_setall_u8(1);
        ಆದರೆ (cn == 1) {
            ಪೂರ್ಣಾಂಕ i = 0;
            v_uint32 acc = vx_setzero_u32();
            ಗಾಗಿ (; i <= len - vstep; i += vstep) {
                v_uint8 s = v_and(vx_ತುಂಬು(src + i), v_normmask_u8(ಮುಖವಾಡ + i));
                acc = v_dotprod_expand_fast(s, one, acc);
            }
            ಫಲಿತಾಂಶ = (ಪೂರ್ಣಾಂಕ)v_reduce_ಮೊತ್ತ(acc);
            ಗಾಗಿ (; i < len; i++) {
                ಆದರೆ (ಮುಖವಾಡ[i])
                    ಫಲಿತಾಂಶ += src[i];
            }
        }
        ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (cn == 4) {
            ಪೂರ್ಣಾಂಕ i = 0;
            v_uint32 acc = vx_setzero_u32();
            ಗಾಗಿ (; i <= len - vstep; i += vstep) {
                v_uint8 c0, c1, c2, c3;
                v_ತುಂಬು_deinterleave(src + i * 4, c0, c1, c2, c3);
                v_uint8 m = v_normmask_u8(ಮುಖವಾಡ + i);
                acc = v_dotprod_expand_fast(v_and(c0, m), one, acc);
                acc = v_dotprod_expand_fast(v_and(c1, m), one, acc);
                acc = v_dotprod_expand_fast(v_and(c2, m), one, acc);
                acc = v_dotprod_expand_fast(v_and(c3, m), one, acc);
            }
            ಫಲಿತಾಂಶ = (ಪೂರ್ಣಾಂಕ)v_reduce_ಮೊತ್ತ(acc);
            ಗಾಗಿ (; i < len; i++) {
                ಆದರೆ (ಮುಖವಾಡ[i]) {
                    ಸ್ಥಿರಾಂಕ uchar* elem = src + i * 4;
                    ಫಲಿತಾಂಶ += elem[0] + elem[1] + elem[2] + elem[3];
                }
            }
        }
        ಇಲ್ಲದಿದ್ದರೆ {
            ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < len; i++) {
                ಆದರೆ (ಮುಖವಾಡ[i]) {
                    ಸ್ಥಿರಾಂಕ uchar* elem = src + i * cn;
                    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ k = 0; k < cn; k++)
                        ಫಲಿತಾಂಶ += elem[k];
                }
            }
        }
        ಹಿಂತಿರುಗಿ ಫಲಿತಾಂಶ;
    }
};

ಮಾದರಿ <>
ರಚನೆ MaskedNormInf_SIMD<ushort, ಪೂರ್ಣಾಂಕ> {
    ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ ಕ್ರಿಯಾಕಾರಕ()(ಸ್ಥಿರಾಂಕ ushort* src, ಸ್ಥಿರಾಂಕ uchar* ಮುಖವಾಡ, ಪೂರ್ಣಾಂಕ len, ಪೂರ್ಣಾಂಕ cn) ಸ್ಥಿರಾಂಕ {
        ಪೂರ್ಣಾಂಕ ಫಲಿತಾಂಶ = 0;
        ಆದರೆ (cn == 1) {
            ಪೂರ್ಣಾಂಕ i = 0;
            ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ vstep = VTraits<v_uint16>::vlanes();
            v_uint16 acc = vx_setzero_u16();

            ಗಾಗಿ (; i <= len - vstep; i += vstep) {
                v_uint16 m   = vx_ತುಂಬು_expand(ಮುಖವಾಡ + i);
                v_uint16 cmp = v_gt(m, vx_setzero_u16());
                v_uint16 s   = vx_ತುಂಬು(src + i);
                v_uint16 sel = v_and(s, cmp);
                acc = v_max(acc, sel);
            }

            ಫಲಿತಾಂಶ = (ಪೂರ್ಣಾಂಕ)v_reduce_max(acc);

            ಗಾಗಿ (; i < len; i++) {
                ಆದರೆ (ಮುಖವಾಡ[i])
                    ಫಲಿತಾಂಶ = std::max(ಫಲಿತಾಂಶ, (ಪೂರ್ಣಾಂಕ)src[i]);
            }
        }
        ಇಲ್ಲದಿದ್ದರೆ {
            ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < len; i++) {
                ಆದರೆ (ಮುಖವಾಡ[i]) {
                    ಸ್ಥಿರಾಂಕ ushort* elem = src + i * cn;
                    ಪೂರ್ಣಾಂಕ k = 0;
                    ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ vstep = VTraits<v_uint16>::vlanes();
                    v_uint16 acc = vx_setzero_u16();

                    ಗಾಗಿ (; k <= cn - vstep; k += vstep) {
                        acc = v_max(acc, vx_ತುಂಬು(elem + k));
                    }

                    ಫಲಿತಾಂಶ = std::max(ಫಲಿತಾಂಶ, (ಪೂರ್ಣಾಂಕ)v_reduce_max(acc));

                    ಗಾಗಿ (; k < cn; k++)
                        ಫಲಿತಾಂಶ = std::max(ಫಲಿತಾಂಶ, (ಪೂರ್ಣಾಂಕ)elem[k]);
                }
            }
        }
        ಹಿಂತಿರುಗಿ ಫಲಿತಾಂಶ;
    }
};

ಮಾದರಿ <>
ರಚನೆ MaskedNormL1_SIMD<ushort, ಪೂರ್ಣಾಂಕ> {
    ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ ಕ್ರಿಯಾಕಾರಕ()(ಸ್ಥಿರಾಂಕ ushort* src, ಸ್ಥಿರಾಂಕ uchar* ಮುಖವಾಡ, ಪೂರ್ಣಾಂಕ len, ಪೂರ್ಣಾಂಕ cn) ಸ್ಥಿರಾಂಕ {
        ಪೂರ್ಣಾಂಕ ಫಲಿತಾಂಶ = 0;
        ಆದರೆ (cn == 1) {
            ಪೂರ್ಣಾಂಕ i = 0;
            ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ vstep = VTraits<v_uint16>::vlanes();
            v_uint32 acc32 = vx_setzero_u32();
            v_uint64 acc64 = vx_setzero_u64();
            ಪೂರ್ಣಾಂಕ acc32_elems = 0;

            ಗಾಗಿ (; i <= len - vstep; i += vstep, acc32_elems += vstep) {
                ಆದರೆ (acc32_elems >= 512) {
                    v_uint64 lo64, hi64;
                    v_expand(acc32, lo64, hi64);
                    acc64 = v_ಕೂಡಿಸು(acc64, v_ಕೂಡಿಸು(lo64, hi64));
                    acc32 = vx_setzero_u32();
                    acc32_elems = 0;
                }
                v_uint16 m   = vx_ತುಂಬು_expand(ಮುಖವಾಡ + i);
                v_uint16 cmp = v_gt(m, vx_setzero_u16());
                v_uint16 s   = v_and(vx_ತುಂಬು(src + i), cmp);
                v_uint32 lo32, hi32;
                v_expand(s, lo32, hi32);
                acc32 = v_ಕೂಡಿಸು(acc32, v_ಕೂಡಿಸು(lo32, hi32));
            }

            v_uint64 lo64, hi64;
            v_expand(acc32, lo64, hi64);
            acc64 = v_ಕೂಡಿಸು(acc64, v_ಕೂಡಿಸು(lo64, hi64));
            ಫಲಿತಾಂಶ = (ಪೂರ್ಣಾಂಕ)v_reduce_ಮೊತ್ತ(acc64);

            ಗಾಗಿ (; i < len; i++) {
                ಆದರೆ (ಮುಖವಾಡ[i])
                    ಫಲಿತಾಂಶ += src[i];
            }
        }
        ಇಲ್ಲದಿದ್ದರೆ {
            ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < len; i++) {
                ಆದರೆ (ಮುಖವಾಡ[i]) {
                    ಸ್ಥಿರಾಂಕ ushort* elem = src + i * cn;
                    ಪೂರ್ಣಾಂಕ k = 0;
                    ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ vstep = VTraits<v_uint16>::vlanes();
                    v_uint32 acc = vx_setzero_u32();

                    ಗಾಗಿ (; k <= cn - vstep; k += vstep) {
                        v_uint32 lo32, hi32;
                        v_expand(vx_ತುಂಬು(elem + k), lo32, hi32);
                        acc = v_ಕೂಡಿಸು(acc, v_ಕೂಡಿಸು(lo32, hi32));
                    }

                    ಫಲಿತಾಂಶ += (ಪೂರ್ಣಾಂಕ)v_reduce_ಮೊತ್ತ(acc);

                    ಗಾಗಿ (; k < cn; k++)
                        ಫಲಿತಾಂಶ += elem[k];
                }
            }
        }
        ಹಿಂತಿರುಗಿ ಫಲಿತಾಂಶ;
    }
};

ಮಾದರಿ <>
ರಚನೆ MaskedNormL2_SIMD<ushort, ದ್ವಿಗುಣ> {
    ಸಾಲೊಳಗೆ ದ್ವಿಗುಣ ಕ್ರಿಯಾಕಾರಕ()(ಸ್ಥಿರಾಂಕ ushort* src, ಸ್ಥಿರಾಂಕ uchar* ಮುಖವಾಡ, ಪೂರ್ಣಾಂಕ len, ಪೂರ್ಣಾಂಕ cn) ಸ್ಥಿರಾಂಕ {
        ದ್ವಿಗುಣ ಫಲಿತಾಂಶ = 0.0;
        ಆದರೆ (cn == 1) {
            ಪೂರ್ಣಾಂಕ i = 0;
            ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ vstep = VTraits<v_uint16>::vlanes();
            v_uint64 acc = vx_setzero_u64();
            ಗಾಗಿ (; i <= len - vstep; i += vstep) {
                v_uint16 m   = vx_ತುಂಬು_expand(ಮುಖವಾಡ + i);
                v_uint16 cmp = v_gt(m, vx_setzero_u16());
                v_uint16 s   = v_and(vx_ತುಂಬು(src + i), cmp);
                v_uint32 lo32, hi32;
                v_expand(s, lo32, hi32);
                v_uint64 lo64a, lo64b, hi64a, hi64b;
                v_expand(v_mul(lo32, lo32), lo64a, lo64b);
                v_expand(v_mul(hi32, hi32), hi64a, hi64b);
                acc = v_ಕೂಡಿಸು(acc, v_ಕೂಡಿಸು(v_ಕೂಡಿಸು(lo64a, lo64b), v_ಕೂಡಿಸು(hi64a, hi64b)));
            }
            ಫಲಿತಾಂಶ = (ದ್ವಿಗುಣ)v_reduce_ಮೊತ್ತ(acc);
            ಗಾಗಿ (; i < len; i++) {
                ಆದರೆ (ಮುಖವಾಡ[i]) {
                    ದ್ವಿಗುಣ v = src[i];
                    ಫಲಿತಾಂಶ += v * v;
                }
            }
        }
        ಇಲ್ಲದಿದ್ದರೆ {
            ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < len; i++) {
                ಆದರೆ (ಮುಖವಾಡ[i]) {
                    ಸ್ಥಿರಾಂಕ ushort* elem = src + i * cn;
                    ಪೂರ್ಣಾಂಕ k = 0;
                    ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ vstep = VTraits<v_uint16>::vlanes();
                    v_uint64 acc = vx_setzero_u64();
                    ಗಾಗಿ (; k <= cn - vstep; k += vstep) {
                        v_uint32 lo32, hi32;
                        v_expand(vx_ತುಂಬು(elem + k), lo32, hi32);
                        v_uint64 lo64a, lo64b, hi64a, hi64b;
                        v_expand(v_mul(lo32, lo32), lo64a, lo64b);
                        v_expand(v_mul(hi32, hi32), hi64a, hi64b);
                        acc = v_ಕೂಡಿಸು(acc, v_ಕೂಡಿಸು(v_ಕೂಡಿಸು(lo64a, lo64b), v_ಕೂಡಿಸು(hi64a, hi64b)));
                    }

                    ಫಲಿತಾಂಶ += (ದ್ವಿಗುಣ)v_reduce_ಮೊತ್ತ(acc);

                    ಗಾಗಿ (; k < cn; k++) {
                        ದ್ವಿಗುಣ v = elem[k];
                        ಫಲಿತಾಂಶ += v * v;
                    }
                }
            }
        }
        ಹಿಂತಿರುಗಿ ಫಲಿತಾಂಶ;
    }
};

ಮಾದರಿ <>
ರಚನೆ MaskedNormInf_SIMD<schar, ಪೂರ್ಣಾಂಕ> {
    ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ ಕ್ರಿಯಾಕಾರಕ()(ಸ್ಥಿರಾಂಕ schar* src, ಸ್ಥಿರಾಂಕ uchar* ಮುಖವಾಡ, ಪೂರ್ಣಾಂಕ len, ಪೂರ್ಣಾಂಕ cn) ಸ್ಥಿರಾಂಕ {
        ಪೂರ್ಣಾಂಕ ಫಲಿತಾಂಶ = 0;
        ಆದರೆ (cn == 1) {
            ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ vstep = VTraits<v_int8>::vlanes();
            ಪೂರ್ಣಾಂಕ i = 0;
            v_uint8 acc = vx_setzero_u8();
            ಗಾಗಿ (; i <= len - vstep; i += vstep)
                acc = v_max(acc, v_and(v_abs(vx_ತುಂಬು(src + i)), v_normmask_u8(ಮುಖವಾಡ + i)));
            ಫಲಿತಾಂಶ = (ಪೂರ್ಣಾಂಕ)v_reduce_max(acc);
            ಗಾಗಿ (; i < len; i++)
                ಆದರೆ (ಮುಖವಾಡ[i]) ಫಲಿತಾಂಶ = std::max(ಫಲಿತಾಂಶ, cv_abs(src[i]));
        }
        ಇಲ್ಲದಿದ್ದರೆ {
            ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < len; i++)
                ಆದರೆ (ಮುಖವಾಡ[i]) {
                    ಸ್ಥಿರಾಂಕ schar* elem = src + i * cn;
                    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ k = 0; k < cn; k++)
                        ಫಲಿತಾಂಶ = std::max(ಫಲಿತಾಂಶ, cv_abs(elem[k]));
                }
        }
        ಹಿಂತಿರುಗಿ ಫಲಿತಾಂಶ;
    }
};

ಮಾದರಿ <>
ರಚನೆ MaskedNormL1_SIMD<schar, ಪೂರ್ಣಾಂಕ> {
    ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ ಕ್ರಿಯಾಕಾರಕ()(ಸ್ಥಿರಾಂಕ schar* src, ಸ್ಥಿರಾಂಕ uchar* ಮುಖವಾಡ, ಪೂರ್ಣಾಂಕ len, ಪೂರ್ಣಾಂಕ cn) ಸ್ಥಿರಾಂಕ {
        ಪೂರ್ಣಾಂಕ ಫಲಿತಾಂಶ = 0;
        ಆದರೆ (cn == 1) {
            ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ vstep = VTraits<v_int8>::vlanes();
            ಸ್ಥಿರಾಂಕ v_uint8 one = vx_setall_u8(1);
            ಪೂರ್ಣಾಂಕ i = 0;
            v_uint32 acc = vx_setzero_u32();
            ಗಾಗಿ (; i <= len - vstep; i += vstep)
                acc = v_dotprod_expand_fast(v_and(v_abs(vx_ತುಂಬು(src + i)), v_normmask_u8(ಮುಖವಾಡ + i)), one, acc);
            ಫಲಿತಾಂಶ = (ಪೂರ್ಣಾಂಕ)v_reduce_ಮೊತ್ತ(acc);
            ಗಾಗಿ (; i < len; i++)
                ಆದರೆ (ಮುಖವಾಡ[i]) ಫಲಿತಾಂಶ += cv_abs(src[i]);
        }
        ಇಲ್ಲದಿದ್ದರೆ {
            ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < len; i++)
                ಆದರೆ (ಮುಖವಾಡ[i]) {
                    ಸ್ಥಿರಾಂಕ schar* elem = src + i * cn;
                    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ k = 0; k < cn; k++)
                        ಫಲಿತಾಂಶ += cv_abs(elem[k]);
                }
        }
        ಹಿಂತಿರುಗಿ ಫಲಿತಾಂಶ;
    }
};

ಮಾದರಿ <>
ರಚನೆ MaskedNormL2_SIMD<schar, ಪೂರ್ಣಾಂಕ> {
    ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ ಕ್ರಿಯಾಕಾರಕ()(ಸ್ಥಿರಾಂಕ schar* src, ಸ್ಥಿರಾಂಕ uchar* ಮುಖವಾಡ, ಪೂರ್ಣಾಂಕ len, ಪೂರ್ಣಾಂಕ cn) ಸ್ಥಿರಾಂಕ {
        ಪೂರ್ಣಾಂಕ ಫಲಿತಾಂಶ = 0;
        ಆದರೆ (cn == 1) {
            ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ vstep = VTraits<v_int8>::vlanes();
            ಪೂರ್ಣಾಂಕ i = 0;
            v_int32 acc = vx_setzero_s32();
            ಗಾಗಿ (; i <= len - vstep; i += vstep) {
                v_int8 s = v_and(vx_ತುಂಬು(src + i), v_reinterpret_as_s8(v_normmask_u8(ಮುಖವಾಡ + i)));
                acc = v_dotprod_expand_fast(s, s, acc);
            }
            ಫಲಿತಾಂಶ = (ಪೂರ್ಣಾಂಕ)v_reduce_ಮೊತ್ತ(acc);
            ಗಾಗಿ (; i < len; i++)
                ಆದರೆ (ಮುಖವಾಡ[i]) { ಪೂರ್ಣಾಂಕ v = src[i]; ಫಲಿತಾಂಶ += v * v; }
        }
        ಇಲ್ಲದಿದ್ದರೆ {
            ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < len; i++)
                ಆದರೆ (ಮುಖವಾಡ[i]) {
                    ಸ್ಥಿರಾಂಕ schar* elem = src + i * cn;
                    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ k = 0; k < cn; k++) { ಪೂರ್ಣಾಂಕ v = elem[k]; ಫಲಿತಾಂಶ += v * v; }
                }
        }
        ಹಿಂತಿರುಗಿ ಫಲಿತಾಂಶ;
    }
};

ಮಾದರಿ <>
ರಚನೆ MaskedNormL2_SIMD<uchar, ಪೂರ್ಣಾಂಕ> {
    ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ ಕ್ರಿಯಾಕಾರಕ()(ಸ್ಥಿರಾಂಕ uchar* src, ಸ್ಥಿರಾಂಕ uchar* ಮುಖವಾಡ, ಪೂರ್ಣಾಂಕ len, ಪೂರ್ಣಾಂಕ cn) ಸ್ಥಿರಾಂಕ {
        ಪೂರ್ಣಾಂಕ ಫಲಿತಾಂಶ = 0;
        ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ vstep = VTraits<v_uint8>::vlanes();
        ಆದರೆ (cn == 1) {
            ಪೂರ್ಣಾಂಕ i = 0;
            v_uint32 acc = vx_setzero_u32();
            ಗಾಗಿ (; i <= len - vstep; i += vstep) {
                v_uint8 s = v_and(vx_ತುಂಬು(src + i), v_normmask_u8(ಮುಖವಾಡ + i));
                acc = v_dotprod_expand_fast(s, s, acc);
            }
            ಫಲಿತಾಂಶ = (ಪೂರ್ಣಾಂಕ)v_reduce_ಮೊತ್ತ(acc);
            ಗಾಗಿ (; i < len; i++)
                ಆದರೆ (ಮುಖವಾಡ[i]) { ಪೂರ್ಣಾಂಕ v = src[i]; ಫಲಿತಾಂಶ += v * v; }
        }
        ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (cn == 4) {
            ಪೂರ್ಣಾಂಕ i = 0;
            v_uint32 acc = vx_setzero_u32();
            ಗಾಗಿ (; i <= len - vstep; i += vstep) {
                v_uint8 c0, c1, c2, c3;
                v_ತುಂಬು_deinterleave(src + i * 4, c0, c1, c2, c3);
                v_uint8 m = v_normmask_u8(ಮುಖವಾಡ + i);
                c0 = v_and(c0, m); c1 = v_and(c1, m);
                c2 = v_and(c2, m); c3 = v_and(c3, m);
                acc = v_dotprod_expand_fast(c0, c0, acc);
                acc = v_dotprod_expand_fast(c1, c1, acc);
                acc = v_dotprod_expand_fast(c2, c2, acc);
                acc = v_dotprod_expand_fast(c3, c3, acc);
            }
            ಫಲಿತಾಂಶ = (ಪೂರ್ಣಾಂಕ)v_reduce_ಮೊತ್ತ(acc);
            ಗಾಗಿ (; i < len; i++)
                ಆದರೆ (ಮುಖವಾಡ[i]) {
                    ಸ್ಥಿರಾಂಕ uchar* elem = src + i * 4;
                    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ k = 0; k < 4; k++) { ಪೂರ್ಣಾಂಕ v = elem[k]; ಫಲಿತಾಂಶ += v * v; }
                }
        }
        ಇಲ್ಲದಿದ್ದರೆ {
            ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < len; i++)
                ಆದರೆ (ಮುಖವಾಡ[i]) {
                    ಸ್ಥಿರಾಂಕ uchar* elem = src + i * cn;
                    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ k = 0; k < cn; k++) { ಪೂರ್ಣಾಂಕ v = elem[k]; ಫಲಿತಾಂಶ += v * v; }
                }
        }
        ಹಿಂತಿರುಗಿ ಫಲಿತಾಂಶ;
    }
};

ಮಾದರಿ <>
ರಚನೆ MaskedNormInf_SIMD<ಗಿಡ್ಡ, ಪೂರ್ಣಾಂಕ> {
    ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ ಕ್ರಿಯಾಕಾರಕ()(ಸ್ಥಿರಾಂಕ ಗಿಡ್ಡ* src, ಸ್ಥಿರಾಂಕ uchar* ಮುಖವಾಡ, ಪೂರ್ಣಾಂಕ len, ಪೂರ್ಣಾಂಕ cn) ಸ್ಥಿರಾಂಕ {
        ಪೂರ್ಣಾಂಕ ಫಲಿತಾಂಶ = 0;
        ಆದರೆ (cn == 1) {
            ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ vstep = VTraits<v_int16>::vlanes();
            ಪೂರ್ಣಾಂಕ i = 0;
            v_uint16 acc = vx_setzero_u16();
            ಗಾಗಿ (; i <= len - vstep; i += vstep)
                acc = v_max(acc, v_and(v_abs(vx_ತುಂಬು(src + i)), v_normmask_u16(ಮುಖವಾಡ + i)));
            ಫಲಿತಾಂಶ = (ಪೂರ್ಣಾಂಕ)v_reduce_max(acc);
            ಗಾಗಿ (; i < len; i++)
                ಆದರೆ (ಮುಖವಾಡ[i]) ಫಲಿತಾಂಶ = std::max(ಫಲಿತಾಂಶ, cv_abs(src[i]));
        }
        ಇಲ್ಲದಿದ್ದರೆ {
            ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < len; i++)
                ಆದರೆ (ಮುಖವಾಡ[i]) {
                    ಸ್ಥಿರಾಂಕ ಗಿಡ್ಡ* elem = src + i * cn;
                    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ k = 0; k < cn; k++)
                        ಫಲಿತಾಂಶ = std::max(ಫಲಿತಾಂಶ, cv_abs(elem[k]));
                }
        }
        ಹಿಂತಿರುಗಿ ಫಲಿತಾಂಶ;
    }
};

ಮಾದರಿ <>
ರಚನೆ MaskedNormL1_SIMD<ಗಿಡ್ಡ, ಪೂರ್ಣಾಂಕ> {
    ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ ಕ್ರಿಯಾಕಾರಕ()(ಸ್ಥಿರಾಂಕ ಗಿಡ್ಡ* src, ಸ್ಥಿರಾಂಕ uchar* ಮುಖವಾಡ, ಪೂರ್ಣಾಂಕ len, ಪೂರ್ಣಾಂಕ cn) ಸ್ಥಿರಾಂಕ {
        ಪೂರ್ಣಾಂಕ ಫಲಿತಾಂಶ = 0;
        ಆದರೆ (cn == 1) {
            ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ vstep = VTraits<v_int16>::vlanes();
            ಪೂರ್ಣಾಂಕ i = 0;
            v_uint32 acc0 = vx_setzero_u32(), acc1 = vx_setzero_u32();
            ಗಾಗಿ (; i <= len - vstep; i += vstep) {
                v_uint16 s = v_and(v_abs(vx_ತುಂಬು(src + i)), v_normmask_u16(ಮುಖವಾಡ + i));
                v_uint32 lo, hi;
                v_expand(s, lo, hi);
                acc0 = v_ಕೂಡಿಸು(acc0, lo);
                acc1 = v_ಕೂಡಿಸು(acc1, hi);
            }
            ಫಲಿತಾಂಶ = (ಪೂರ್ಣಾಂಕ)v_reduce_ಮೊತ್ತ(v_ಕೂಡಿಸು(acc0, acc1));
            ಗಾಗಿ (; i < len; i++)
                ಆದರೆ (ಮುಖವಾಡ[i]) ಫಲಿತಾಂಶ += cv_abs(src[i]);
        }
        ಇಲ್ಲದಿದ್ದರೆ {
            ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < len; i++)
                ಆದರೆ (ಮುಖವಾಡ[i]) {
                    ಸ್ಥಿರಾಂಕ ಗಿಡ್ಡ* elem = src + i * cn;
                    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ k = 0; k < cn; k++)
                        ಫಲಿತಾಂಶ += cv_abs(elem[k]);
                }
        }
        ಹಿಂತಿರುಗಿ ಫಲಿತಾಂಶ;
    }
};

ಮಾದರಿ <>
ರಚನೆ MaskedNormL2_SIMD<ಗಿಡ್ಡ, ದ್ವಿಗುಣ> {
    ಸಾಲೊಳಗೆ ದ್ವಿಗುಣ ಕ್ರಿಯಾಕಾರಕ()(ಸ್ಥಿರಾಂಕ ಗಿಡ್ಡ* src, ಸ್ಥಿರಾಂಕ uchar* ಮುಖವಾಡ, ಪೂರ್ಣಾಂಕ len, ಪೂರ್ಣಾಂಕ cn) ಸ್ಥಿರಾಂಕ {
        ದ್ವಿಗುಣ ಫಲಿತಾಂಶ = 0.0;
        ಆದರೆ (cn == 1) {
            ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ vstep = VTraits<v_int16>::vlanes();
            ಪೂರ್ಣಾಂಕ i = 0;
            v_uint64 acc = vx_setzero_u64();
            ಗಾಗಿ (; i <= len - vstep; i += vstep) {
                v_uint16 s = v_and(v_abs(vx_ತುಂಬು(src + i)), v_normmask_u16(ಮುಖವಾಡ + i));
                acc = v_ಕೂಡಿಸು(acc, v_dotprod_expand_fast(s, s));
            }
            ಫಲಿತಾಂಶ = (ದ್ವಿಗುಣ)v_reduce_ಮೊತ್ತ(acc);
            ಗಾಗಿ (; i < len; i++)
                ಆದರೆ (ಮುಖವಾಡ[i]) { ದ್ವಿಗುಣ v = src[i]; ಫಲಿತಾಂಶ += v * v; }
        }
        ಇಲ್ಲದಿದ್ದರೆ {
            ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < len; i++)
                ಆದರೆ (ಮುಖವಾಡ[i]) {
                    ಸ್ಥಿರಾಂಕ ಗಿಡ್ಡ* elem = src + i * cn;
                    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ k = 0; k < cn; k++) { ದ್ವಿಗುಣ v = elem[k]; ಫಲಿತಾಂಶ += v * v; }
                }
        }
        ಹಿಂತಿರುಗಿ ಫಲಿತಾಂಶ;
    }
};

ಮಾದರಿ <>
ರಚನೆ MaskedNormInf_SIMD<ಪೂರ್ಣಾಂಕ, ಚಿಹ್ನೆರಹಿತ> {
    ಸಾಲೊಳಗೆ ಚಿಹ್ನೆರಹಿತ ಕ್ರಿಯಾಕಾರಕ()(ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ* src, ಸ್ಥಿರಾಂಕ uchar* ಮುಖವಾಡ, ಪೂರ್ಣಾಂಕ len, ಪೂರ್ಣಾಂಕ cn) ಸ್ಥಿರಾಂಕ {
        ಚಿಹ್ನೆರಹಿತ ಫಲಿತಾಂಶ = 0;
        ಆದರೆ (cn == 1) {
            ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ vstep = VTraits<v_int32>::vlanes();
            ಪೂರ್ಣಾಂಕ i = 0;
            v_uint32 acc = vx_setzero_u32();
            ಗಾಗಿ (; i <= len - vstep; i += vstep)
                acc = v_max(acc, v_and(v_abs(vx_ತುಂಬು(src + i)), v_normmask_u32(ಮುಖವಾಡ + i)));
            ಫಲಿತಾಂಶ = v_reduce_max(acc);
            ಗಾಗಿ (; i < len; i++)
                ಆದರೆ (ಮುಖವಾಡ[i]) ಫಲಿತಾಂಶ = std::max(ಫಲಿತಾಂಶ, cv_abs(src[i]));
        }
        ಇಲ್ಲದಿದ್ದರೆ {
            ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < len; i++)
                ಆದರೆ (ಮುಖವಾಡ[i]) {
                    ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ* elem = src + i * cn;
                    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ k = 0; k < cn; k++)
                        ಫಲಿತಾಂಶ = std::max(ಫಲಿತಾಂಶ, cv_abs(elem[k]));
                }
        }
        ಹಿಂತಿರುಗಿ ಫಲಿತಾಂಶ;
    }
};

#ಆದರೆ CV_SIMD_64F || CV_SIMD_SCALABLE_64F

ಮಾದರಿ <>
ರಚನೆ MaskedNormL1_SIMD<ಪೂರ್ಣಾಂಕ, ದ್ವಿಗುಣ> {
    ಸಾಲೊಳಗೆ ದ್ವಿಗುಣ ಕ್ರಿಯಾಕಾರಕ()(ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ* src, ಸ್ಥಿರಾಂಕ uchar* ಮುಖವಾಡ, ಪೂರ್ಣಾಂಕ len, ಪೂರ್ಣಾಂಕ cn) ಸ್ಥಿರಾಂಕ {
        ದ್ವಿಗುಣ ಫಲಿತಾಂಶ = 0.0;
        ಆದರೆ (cn == 1) {
            ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ vstep = VTraits<v_int32>::vlanes();
            ಪೂರ್ಣಾಂಕ i = 0;
            v_float64 acc0 = vx_setzero_f64(), acc1 = vx_setzero_f64();
            ಗಾಗಿ (; i <= len - vstep; i += vstep) {
                // Mask the value, then widen and take the magnitude in double. v_abs() does give
                // |INT_MIN| as an unsigned 2147483648, but reinterpreting that back as int32 turns
                // it negative again, and the widening then carries the sign into the accumulator.
                v_int32 s = v_reinterpret_as_s32(v_and(v_reinterpret_as_u32(vx_ತುಂಬು(src + i)),
                                                       v_normmask_u32(ಮುಖವಾಡ + i)));
                acc0 = v_ಕೂಡಿಸು(acc0, v_abs(v_cvt_f64(s)));
                acc1 = v_ಕೂಡಿಸು(acc1, v_abs(v_cvt_f64_ಮೇಲಿನ(s)));
            }
            ಫಲಿತಾಂಶ = v_reduce_ಮೊತ್ತ(v_ಕೂಡಿಸು(acc0, acc1));
            ಗಾಗಿ (; i < len; i++)
                ಆದರೆ (ಮುಖವಾಡ[i]) ಫಲಿತಾಂಶ += cv_abs(src[i]);
        }
        ಇಲ್ಲದಿದ್ದರೆ {
            ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < len; i++)
                ಆದರೆ (ಮುಖವಾಡ[i]) {
                    ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ* elem = src + i * cn;
                    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ k = 0; k < cn; k++)
                        ಫಲಿತಾಂಶ += cv_abs(elem[k]);
                }
        }
        ಹಿಂತಿರುಗಿ ಫಲಿತಾಂಶ;
    }
};

ಮಾದರಿ <>
ರಚನೆ MaskedNormL2_SIMD<ಪೂರ್ಣಾಂಕ, ದ್ವಿಗುಣ> {
    ಸಾಲೊಳಗೆ ದ್ವಿಗುಣ ಕ್ರಿಯಾಕಾರಕ()(ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ* src, ಸ್ಥಿರಾಂಕ uchar* ಮುಖವಾಡ, ಪೂರ್ಣಾಂಕ len, ಪೂರ್ಣಾಂಕ cn) ಸ್ಥಿರಾಂಕ {
        ದ್ವಿಗುಣ ಫಲಿತಾಂಶ = 0.0;
        ಆದರೆ (cn == 1) {
            ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ vstep = VTraits<v_int32>::vlanes();
            ಪೂರ್ಣಾಂಕ i = 0;
            v_float64 acc0 = vx_setzero_f64(), acc1 = vx_setzero_f64();
            ಗಾಗಿ (; i <= len - vstep; i += vstep) {
                v_uint32 s = v_and(v_abs(vx_ತುಂಬು(src + i)), v_normmask_u32(ಮುಖವಾಡ + i));
                v_uint64 e0, e1;
                v_expand(s, e0, e1);
                v_float64 f0 = v_cvt_f64(v_reinterpret_as_s64(e0));
                v_float64 f1 = v_cvt_f64(v_reinterpret_as_s64(e1));
                acc0 = v_fma(f0, f0, acc0);
                acc1 = v_fma(f1, f1, acc1);
            }
            ಫಲಿತಾಂಶ = v_reduce_ಮೊತ್ತ(v_ಕೂಡಿಸು(acc0, acc1));
            ಗಾಗಿ (; i < len; i++)
                ಆದರೆ (ಮುಖವಾಡ[i]) { ದ್ವಿಗುಣ v = src[i]; ಫಲಿತಾಂಶ += v * v; }
        }
        ಇಲ್ಲದಿದ್ದರೆ {
            ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < len; i++)
                ಆದರೆ (ಮುಖವಾಡ[i]) {
                    ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ* elem = src + i * cn;
                    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ k = 0; k < cn; k++) { ದ್ವಿಗುಣ v = elem[k]; ಫಲಿತಾಂಶ += v * v; }
                }
        }
        ಹಿಂತಿರುಗಿ ಫಲಿತಾಂಶ;
    }
};

ಮಾದರಿ <>
ರಚನೆ MaskedNormInf_SIMD<ದ್ವಿಗುಣ, ದ್ವಿಗುಣ> {
    ಸಾಲೊಳಗೆ ದ್ವಿಗುಣ ಕ್ರಿಯಾಕಾರಕ()(ಸ್ಥಿರಾಂಕ ದ್ವಿಗುಣ* src, ಸ್ಥಿರಾಂಕ uchar* ಮುಖವಾಡ, ಪೂರ್ಣಾಂಕ len, ಪೂರ್ಣಾಂಕ cn) ಸ್ಥಿರಾಂಕ {
        ದ್ವಿಗುಣ ಫಲಿತಾಂಶ = 0.0;
        ಆದರೆ (cn == 1) {
            ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ vstep = VTraits<v_float64>::vlanes();
            ಪೂರ್ಣಾಂಕ i = 0;
            v_float64 acc0 = vx_setzero_f64(), acc1 = vx_setzero_f64();
            ಗಾಗಿ (; i <= len - 2*vstep; i += 2*vstep) {
                v_uint64 m0, m1;
                v_normmask_u64(ಮುಖವಾಡ + i, m0, m1);
                acc0 = v_max(acc0, v_reinterpret_as_f64(v_and(v_reinterpret_as_u64(v_abs(vx_ತುಂಬು(src + i))), m0)));
                acc1 = v_max(acc1, v_reinterpret_as_f64(v_and(v_reinterpret_as_u64(v_abs(vx_ತುಂಬು(src + i + vstep))), m1)));
            }
            ದ್ವಿಗುಣ t[VTraits<v_float64>::max_nlanes];
            vx_store(t, v_max(acc0, acc1));
            ಗಾಗಿ (ಪೂರ್ಣಾಂಕ k = 0; k < vstep; k++)
                ಫಲಿತಾಂಶ = std::max(ಫಲಿತಾಂಶ, t[k]);
            ಗಾಗಿ (; i < len; i++)
                ಆದರೆ (ಮುಖವಾಡ[i]) ಫಲಿತಾಂಶ = std::max(ಫಲಿತಾಂಶ, cv_abs(src[i]));
        }
        ಇಲ್ಲದಿದ್ದರೆ {
            ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < len; i++)
                ಆದರೆ (ಮುಖವಾಡ[i]) {
                    ಸ್ಥಿರಾಂಕ ದ್ವಿಗುಣ* elem = src + i * cn;
                    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ k = 0; k < cn; k++)
                        ಫಲಿತಾಂಶ = std::max(ಫಲಿತಾಂಶ, cv_abs(elem[k]));
                }
        }
        ಹಿಂತಿರುಗಿ ಫಲಿತಾಂಶ;
    }
};

ಮಾದರಿ <>
ರಚನೆ MaskedNormL1_SIMD<ದ್ವಿಗುಣ, ದ್ವಿಗುಣ> {
    ಸಾಲೊಳಗೆ ದ್ವಿಗುಣ ಕ್ರಿಯಾಕಾರಕ()(ಸ್ಥಿರಾಂಕ ದ್ವಿಗುಣ* src, ಸ್ಥಿರಾಂಕ uchar* ಮುಖವಾಡ, ಪೂರ್ಣಾಂಕ len, ಪೂರ್ಣಾಂಕ cn) ಸ್ಥಿರಾಂಕ {
        ದ್ವಿಗುಣ ಫಲಿತಾಂಶ = 0.0;
        ಆದರೆ (cn == 1) {
            ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ vstep = VTraits<v_float64>::vlanes();
            ಪೂರ್ಣಾಂಕ i = 0;
            v_float64 acc0 = vx_setzero_f64(), acc1 = vx_setzero_f64();
            ಗಾಗಿ (; i <= len - 2*vstep; i += 2*vstep) {
                v_uint64 m0, m1;
                v_normmask_u64(ಮುಖವಾಡ + i, m0, m1);
                acc0 = v_ಕೂಡಿಸು(acc0, v_reinterpret_as_f64(v_and(v_reinterpret_as_u64(v_abs(vx_ತುಂಬು(src + i))), m0)));
                acc1 = v_ಕೂಡಿಸು(acc1, v_reinterpret_as_f64(v_and(v_reinterpret_as_u64(v_abs(vx_ತುಂಬು(src + i + vstep))), m1)));
            }
            ಫಲಿತಾಂಶ = v_reduce_ಮೊತ್ತ(v_ಕೂಡಿಸು(acc0, acc1));
            ಗಾಗಿ (; i < len; i++)
                ಆದರೆ (ಮುಖವಾಡ[i]) ಫಲಿತಾಂಶ += cv_abs(src[i]);
        }
        ಇಲ್ಲದಿದ್ದರೆ {
            ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < len; i++)
                ಆದರೆ (ಮುಖವಾಡ[i]) {
                    ಸ್ಥಿರಾಂಕ ದ್ವಿಗುಣ* elem = src + i * cn;
                    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ k = 0; k < cn; k++)
                        ಫಲಿತಾಂಶ += cv_abs(elem[k]);
                }
        }
        ಹಿಂತಿರುಗಿ ಫಲಿತಾಂಶ;
    }
};

ಮಾದರಿ <>
ರಚನೆ MaskedNormL2_SIMD<ದ್ವಿಗುಣ, ದ್ವಿಗುಣ> {
    ಸಾಲೊಳಗೆ ದ್ವಿಗುಣ ಕ್ರಿಯಾಕಾರಕ()(ಸ್ಥಿರಾಂಕ ದ್ವಿಗುಣ* src, ಸ್ಥಿರಾಂಕ uchar* ಮುಖವಾಡ, ಪೂರ್ಣಾಂಕ len, ಪೂರ್ಣಾಂಕ cn) ಸ್ಥಿರಾಂಕ {
        ದ್ವಿಗುಣ ಫಲಿತಾಂಶ = 0.0;
        ಆದರೆ (cn == 1) {
            ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ vstep = VTraits<v_float64>::vlanes();
            ಪೂರ್ಣಾಂಕ i = 0;
            v_float64 acc0 = vx_setzero_f64(), acc1 = vx_setzero_f64();
            ಗಾಗಿ (; i <= len - 2*vstep; i += 2*vstep) {
                v_uint64 m0, m1;
                v_normmask_u64(ಮುಖವಾಡ + i, m0, m1);
                v_float64 s0 = v_reinterpret_as_f64(v_and(v_reinterpret_as_u64(vx_ತುಂಬು(src + i)), m0));
                v_float64 s1 = v_reinterpret_as_f64(v_and(v_reinterpret_as_u64(vx_ತುಂಬು(src + i + vstep)), m1));
                acc0 = v_fma(s0, s0, acc0);
                acc1 = v_fma(s1, s1, acc1);
            }
            ಫಲಿತಾಂಶ = v_reduce_ಮೊತ್ತ(v_ಕೂಡಿಸು(acc0, acc1));
            ಗಾಗಿ (; i < len; i++)
                ಆದರೆ (ಮುಖವಾಡ[i]) { ದ್ವಿಗುಣ v = src[i]; ಫಲಿತಾಂಶ += v * v; }
        }
        ಇಲ್ಲದಿದ್ದರೆ {
            ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < len; i++)
                ಆದರೆ (ಮುಖವಾಡ[i]) {
                    ಸ್ಥಿರಾಂಕ ದ್ವಿಗುಣ* elem = src + i * cn;
                    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ k = 0; k < cn; k++) { ದ್ವಿಗುಣ v = elem[k]; ಫಲಿತಾಂಶ += v * v; }
                }
        }
        ಹಿಂತಿರುಗಿ ಫಲಿತಾಂಶ;
    }
};

#ಆದರೆ_ಅಂತ್ಯ // CV_SIMD_64F || CV_SIMD_SCALABLE_64F
#ಆದರೆ_ಅಂತ್ಯ // CV_SIMD || CV_SIMD_SCALABLE

ಮಾದರಿ<ಬಗೆ_ಹೆಸರು T, ಬಗೆ_ಹೆಸರು ST> ಪೂರ್ಣಾಂಕ
normInf_(ಸ್ಥಿರಾಂಕ T* src, ಸ್ಥಿರಾಂಕ uchar* ಮುಖವಾಡ, ST* ಫಲಿತಾಂಶ_2, ಪೂರ್ಣಾಂಕ len, ಪೂರ್ಣಾಂಕ cn)
{
    ST ಫಲಿತಾಂಶ = *ಫಲಿತಾಂಶ_2;
    ಆದರೆ( !ಮುಖವಾಡ )
    {
        NormInf_SIMD<T, ST> op;
        ಫಲಿತಾಂಶ = std::max(ಫಲಿತಾಂಶ, op(src, len*cn));
    } ಇಲ್ಲದಿದ್ದರೆ {
        MaskedNormInf_SIMD<T, ST> op;
        ಫಲಿತಾಂಶ = std::max(ಫಲಿತಾಂಶ, op(src, ಮುಖವಾಡ, len, cn));
    }
    *ಫಲಿತಾಂಶ_2 = ಫಲಿತಾಂಶ;
    ಹಿಂತಿರುಗಿ 0;
}

ಮಾದರಿ<ಬಗೆ_ಹೆಸರು T, ಬಗೆ_ಹೆಸರು ST> ಪೂರ್ಣಾಂಕ
normL1_(ಸ್ಥಿರಾಂಕ T* src, ಸ್ಥಿರಾಂಕ uchar* ಮುಖವಾಡ, ST* ಫಲಿತಾಂಶ_2, ಪೂರ್ಣಾಂಕ len, ಪೂರ್ಣಾಂಕ cn)
{
    ST ಫಲಿತಾಂಶ = *ಫಲಿತಾಂಶ_2;
    ಆದರೆ( !ಮುಖವಾಡ )
    {
        NormL1_SIMD<T, ST> op;
        ಫಲಿತಾಂಶ += op(src, len*cn);
    } ಇಲ್ಲದಿದ್ದರೆ {
        MaskedNormL1_SIMD<T, ST> op;
        ಫಲಿತಾಂಶ += op(src, ಮುಖವಾಡ, len, cn);
    }
    *ಫಲಿತಾಂಶ_2 = ಫಲಿತಾಂಶ;
    ಹಿಂತಿರುಗಿ 0;
}

ಮಾದರಿ<ಬಗೆ_ಹೆಸರು T, ಬಗೆ_ಹೆಸರು ST> ಪೂರ್ಣಾಂಕ
normL2_(ಸ್ಥಿರಾಂಕ T* src, ಸ್ಥಿರಾಂಕ uchar* ಮುಖವಾಡ, ST* ಫಲಿತಾಂಶ_2, ಪೂರ್ಣಾಂಕ len, ಪೂರ್ಣಾಂಕ cn)
{
    ST ಫಲಿತಾಂಶ = *ಫಲಿತಾಂಶ_2;
    ಆದರೆ( !ಮುಖವಾಡ )
    {
        NormL2_SIMD<T, ST> op;
        ಫಲಿತಾಂಶ += op(src, len*cn);
    }
    ಇಲ್ಲದಿದ್ದರೆ
    {
        MaskedNormL2_SIMD<T, ST> op;
        ಫಲಿತಾಂಶ += op(src, ಮುಖವಾಡ, len, cn);
    }
    *ಫಲಿತಾಂಶ_2 = ಫಲಿತಾಂಶ;
    ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
normInf_Bool(ಸ್ಥಿರಾಂಕ uchar* src, ಸ್ಥಿರಾಂಕ uchar* ಮುಖವಾಡ, ಪೂರ್ಣಾಂಕ* ಫಲಿತಾಂಶ_2, ಪೂರ್ಣಾಂಕ len, ಪೂರ್ಣಾಂಕ cn)
{
    ಪೂರ್ಣಾಂಕ ಫಲಿತಾಂಶ = *ಫಲಿತಾಂಶ_2;
    ಆದರೆ( !ಮುಖವಾಡ )
    {
        ಗಾಗಿ ( ಪೂರ್ಣಾಂಕ i = 0; i < len*cn; i++ ) {
            ಫಲಿತಾಂಶ = std::max(ಫಲಿತಾಂಶ, (ಪೂರ್ಣಾಂಕ)(src[i] != 0));
            ಆದರೆ (ಫಲಿತಾಂಶ != 0)
                ನಿಲ್ಲಿಸು;
        }
    }
    ಇಲ್ಲದಿದ್ದರೆ
    {
        ಗಾಗಿ( ಪೂರ್ಣಾಂಕ i = 0; i < len; i++, src += cn )
            ಆದರೆ( ಮುಖವಾಡ[i] )
            {
                ಗಾಗಿ( ಪೂರ್ಣಾಂಕ k = 0; k < cn; k++ )
                    ಫಲಿತಾಂಶ = std::max(ಫಲಿತಾಂಶ, (ಪೂರ್ಣಾಂಕ)(src[k] != 0));
                ಆದರೆ (ಫಲಿತಾಂಶ != 0)
                    ನಿಲ್ಲಿಸು;
            }
    }
    *ಫಲಿತಾಂಶ_2 = ಫಲಿತಾಂಶ;
    ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
normL1_Bool(ಸ್ಥಿರಾಂಕ uchar* src, ಸ್ಥಿರಾಂಕ uchar* ಮುಖವಾಡ, ಪೂರ್ಣಾಂಕ* ಫಲಿತಾಂಶ_2, ಪೂರ್ಣಾಂಕ len, ಪೂರ್ಣಾಂಕ cn)
{
    ಪೂರ್ಣಾಂಕ ಫಲಿತಾಂಶ = *ಫಲಿತಾಂಶ_2;
    ಆದರೆ( !ಮುಖವಾಡ )
    {
        ಗಾಗಿ ( ಪೂರ್ಣಾಂಕ i = 0; i < len*cn; i++ )
            ಫಲಿತಾಂಶ += (ಪೂರ್ಣಾಂಕ)(src[i] != 0);
    }
    ಇಲ್ಲದಿದ್ದರೆ
    {
        ಗಾಗಿ( ಪೂರ್ಣಾಂಕ i = 0; i < len; i++, src += cn )
            ಆದರೆ( ಮುಖವಾಡ[i] )
            {
                ಗಾಗಿ( ಪೂರ್ಣಾಂಕ k = 0; k < cn; k++ )
                    ಫಲಿತಾಂಶ += (ಪೂರ್ಣಾಂಕ)(src[k] != 0);
            }
    }
    *ಫಲಿತಾಂಶ_2 = ಫಲಿತಾಂಶ;
    ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
normL2_Bool(ಸ್ಥಿರಾಂಕ uchar* src, ಸ್ಥಿರಾಂಕ uchar* ಮುಖವಾಡ, ಪೂರ್ಣಾಂಕ* ಫಲಿತಾಂಶ_2, ಪೂರ್ಣಾಂಕ len, ಪೂರ್ಣಾಂಕ cn)
{
    ಹಿಂತಿರುಗಿ normL1_Bool(src, ಮುಖವಾಡ, ಫಲಿತಾಂಶ_2, len, cn);
}

// ====================================================================
// Masked norm-diff kernels.
// Scalar base templates (safe for every depth) plus SIMD cn==1
// specializations for the concrete (T, ST) pairs dispatched by
// CV_DEF_NORM_ALL. The masked normDiff*_ functions below select them.
// ====================================================================
ಮಾದರಿ <ಬಗೆ_ಹೆಸರು T, ಬಗೆ_ಹೆಸರು ST>
ರಚನೆ MaskedNormDiffInf_SIMD {
    ಸಾಲೊಳಗೆ ST ಕ್ರಿಯಾಕಾರಕ()(ಸ್ಥಿರಾಂಕ T* s1, ಸ್ಥಿರಾಂಕ T* s2, ಸ್ಥಿರಾಂಕ uchar* ಮುಖವಾಡ, ಪೂರ್ಣಾಂಕ len, ಪೂರ್ಣಾಂಕ cn) ಸ್ಥಿರಾಂಕ {
        ST ಫಲಿತಾಂಶ = 0;
        ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < len; i++) ಆದರೆ (ಮುಖವಾಡ[i]) {
            ಸ್ಥಿರಾಂಕ T* e1 = s1 + i*cn; ಸ್ಥಿರಾಂಕ T* e2 = s2 + i*cn;
            ಗಾಗಿ (ಪೂರ್ಣಾಂಕ k = 0; k < cn; k++)
                ಫಲಿತಾಂಶ = std::max(ಫಲಿತಾಂಶ, (ST)cv_absdiff(e1[k], e2[k]));
        }
        ಹಿಂತಿರುಗಿ ಫಲಿತಾಂಶ;
    }
};
ಮಾದರಿ <ಬಗೆ_ಹೆಸರು T, ಬಗೆ_ಹೆಸರು ST>
ರಚನೆ MaskedNormDiffL1_SIMD {
    ಸಾಲೊಳಗೆ ST ಕ್ರಿಯಾಕಾರಕ()(ಸ್ಥಿರಾಂಕ T* s1, ಸ್ಥಿರಾಂಕ T* s2, ಸ್ಥಿರಾಂಕ uchar* ಮುಖವಾಡ, ಪೂರ್ಣಾಂಕ len, ಪೂರ್ಣಾಂಕ cn) ಸ್ಥಿರಾಂಕ {
        ST ಫಲಿತಾಂಶ = 0;
        ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < len; i++) ಆದರೆ (ಮುಖವಾಡ[i]) {
            ಸ್ಥಿರಾಂಕ T* e1 = s1 + i*cn; ಸ್ಥಿರಾಂಕ T* e2 = s2 + i*cn;
            ಗಾಗಿ (ಪೂರ್ಣಾಂಕ k = 0; k < cn; k++)
                ಫಲಿತಾಂಶ += (ST)cv_absdiff(e1[k], e2[k]);
        }
        ಹಿಂತಿರುಗಿ ಫಲಿತಾಂಶ;
    }
};
ಮಾದರಿ <ಬಗೆ_ಹೆಸರು T, ಬಗೆ_ಹೆಸರು ST>
ರಚನೆ MaskedNormDiffL2_SIMD {
    ಸಾಲೊಳಗೆ ST ಕ್ರಿಯಾಕಾರಕ()(ಸ್ಥಿರಾಂಕ T* s1, ಸ್ಥಿರಾಂಕ T* s2, ಸ್ಥಿರಾಂಕ uchar* ಮುಖವಾಡ, ಪೂರ್ಣಾಂಕ len, ಪೂರ್ಣಾಂಕ cn) ಸ್ಥಿರಾಂಕ {
        ST ಫಲಿತಾಂಶ = 0;
        ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < len; i++) ಆದರೆ (ಮುಖವಾಡ[i]) {
            ಸ್ಥಿರಾಂಕ T* e1 = s1 + i*cn; ಸ್ಥಿರಾಂಕ T* e2 = s2 + i*cn;
            ಗಾಗಿ (ಪೂರ್ಣಾಂಕ k = 0; k < cn; k++) { ST v = (ST)e1[k] - (ST)e2[k]; ಫಲಿತಾಂಶ += v*v; }
        }
        ಹಿಂತಿರುಗಿ ಫಲಿತಾಂಶ;
    }
};

#ಆದರೆ (CV_SIMD || CV_SIMD_SCALABLE)
// AND a per-lane diff vector with the (expanded) mask>0 predicate.
ಸಾಲೊಳಗೆ v_uint8  v_diffmask(ಸ್ಥಿರಾಂಕ v_uint8&  d, ಸ್ಥಿರಾಂಕ uchar* m) { ಹಿಂತಿರುಗಿ v_and(d, v_gt(vx_ತುಂಬು(m),          vx_setzero_u8())); }
ಸಾಲೊಳಗೆ v_uint16 v_diffmask(ಸ್ಥಿರಾಂಕ v_uint16& d, ಸ್ಥಿರಾಂಕ uchar* m) { ಹಿಂತಿರುಗಿ v_and(d, v_gt(vx_ತುಂಬು_expand(m),   vx_setzero_u16())); }
ಸಾಲೊಳಗೆ v_uint32 v_diffmask(ಸ್ಥಿರಾಂಕ v_uint32& d, ಸ್ಥಿರಾಂಕ uchar* m) { ಹಿಂತಿರುಗಿ v_and(d, v_gt(vx_ತುಂಬು_expand_q(m), vx_setzero_u32())); }
ಸಾಲೊಳಗೆ v_float32 v_diffmask(ಸ್ಥಿರಾಂಕ v_float32& d, ಸ್ಥಿರಾಂಕ uchar* m) {
    v_uint32 cm = v_gt(vx_ತುಂಬು_expand_q(m), vx_setzero_u32());
    ಹಿಂತಿರುಗಿ v_reinterpret_as_f32(v_and(v_reinterpret_as_u32(d), cm));
}

// Shared scalar cn>1 fallbacks (match the base templates' cv_absdiff semantics).
ಮಾದರಿ<ಬಗೆ_ಹೆಸರು T, ಬಗೆ_ಹೆಸರು RT>
ಸ್ಥಿರ ಸಾಲೊಳಗೆ RT masked_Norm_Diff_Inf_ಬಾಲ(ಸ್ಥಿರಾಂಕ T* s1, ಸ್ಥಿರಾಂಕ T* s2, ಸ್ಥಿರಾಂಕ uchar* ಮುಖವಾಡ, ಪೂರ್ಣಾಂಕ len, ಪೂರ್ಣಾಂಕ cn, RT ಫಲಿತಾಂಶ) {
    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < len; i++) ಆದರೆ (ಮುಖವಾಡ[i]) {
        ಸ್ಥಿರಾಂಕ T* e1 = s1 + i*cn; ಸ್ಥಿರಾಂಕ T* e2 = s2 + i*cn;
        ಗಾಗಿ (ಪೂರ್ಣಾಂಕ k = 0; k < cn; k++) ಫಲಿತಾಂಶ = std::max(ಫಲಿತಾಂಶ, (RT)cv_absdiff(e1[k], e2[k]));
    }
    ಹಿಂತಿರುಗಿ ಫಲಿತಾಂಶ;
}
ಮಾದರಿ<ಬಗೆ_ಹೆಸರು T, ಬಗೆ_ಹೆಸರು RT>
ಸ್ಥಿರ ಸಾಲೊಳಗೆ RT masked_Norm_Diff_L1_ಬಾಲ(ಸ್ಥಿರಾಂಕ T* s1, ಸ್ಥಿರಾಂಕ T* s2, ಸ್ಥಿರಾಂಕ uchar* ಮುಖವಾಡ, ಪೂರ್ಣಾಂಕ len, ಪೂರ್ಣಾಂಕ cn, RT ಫಲಿತಾಂಶ) {
    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < len; i++) ಆದರೆ (ಮುಖವಾಡ[i]) {
        ಸ್ಥಿರಾಂಕ T* e1 = s1 + i*cn; ಸ್ಥಿರಾಂಕ T* e2 = s2 + i*cn;
        ಗಾಗಿ (ಪೂರ್ಣಾಂಕ k = 0; k < cn; k++) ಫಲಿತಾಂಶ += (RT)cv_absdiff(e1[k], e2[k]);
    }
    ಹಿಂತಿರುಗಿ ಫಲಿತಾಂಶ;
}
ಮಾದರಿ<ಬಗೆ_ಹೆಸರು T, ಬಗೆ_ಹೆಸರು RT>
ಸ್ಥಿರ ಸಾಲೊಳಗೆ RT masked_Norm_Diff_L2_ಬಾಲ(ಸ್ಥಿರಾಂಕ T* s1, ಸ್ಥಿರಾಂಕ T* s2, ಸ್ಥಿರಾಂಕ uchar* ಮುಖವಾಡ, ಪೂರ್ಣಾಂಕ len, ಪೂರ್ಣಾಂಕ cn, RT ಫಲಿತಾಂಶ) {
    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < len; i++) ಆದರೆ (ಮುಖವಾಡ[i]) {
        ಸ್ಥಿರಾಂಕ T* e1 = s1 + i*cn; ಸ್ಥಿರಾಂಕ T* e2 = s2 + i*cn;
        ಗಾಗಿ (ಪೂರ್ಣಾಂಕ k = 0; k < cn; k++) { RT v = (RT)e1[k] - (RT)e2[k]; ಫಲಿತಾಂಶ += v*v; }
    }
    ಹಿಂತಿರುಗಿ ಫಲಿತಾಂಶ;
}

// Inf (cn==1): reduce_max of masked |a-b|. Shared across SIMD-capable depths.
ಮಾದರಿ<ಬಗೆ_ಹೆಸರು T, ಬಗೆ_ಹೆಸರು ST>
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ST maskedNormDiffInfCn1(ಸ್ಥಿರಾಂಕ T* s1, ಸ್ಥಿರಾಂಕ T* s2, ಸ್ಥಿರಾಂಕ uchar* ಮುಖವಾಡ, ಪೂರ್ಣಾಂಕ len) {
    ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ vstep = VTraits<decltype(vx_ತುಂಬು(s1))>::vlanes();
    ST ಫಲಿತಾಂಶ = 0;
    ಪೂರ್ಣಾಂಕ i = 0;
    ಆದರೆ (len >= vstep) {
        ಸ್ವಯಂಚಾಲಿತ acc = v_diffmask(v_absdiff(vx_ತುಂಬು(s1), vx_ತುಂಬು(s2)), ಮುಖವಾಡ);
        ಗಾಗಿ (i = vstep; i <= len - vstep; i += vstep)
            acc = v_max(acc, v_diffmask(v_absdiff(vx_ತುಂಬು(s1 + i), vx_ತುಂಬು(s2 + i)), ಮುಖವಾಡ + i));
        ಫಲಿತಾಂಶ = (ST)v_reduce_max(acc);
    }
    ಗಾಗಿ (; i < len; i++) ಆದರೆ (ಮುಖವಾಡ[i]) ಫಲಿತಾಂಶ = std::max(ಫಲಿತಾಂಶ, (ST)cv_absdiff(s1[i], s2[i]));
    ಹಿಂತಿರುಗಿ ಫಲಿತಾಂಶ;
}

// Shared 8-bit cn==1 masked L1/L2 kernels (uchar/schar): v_absdiff yields v_uint8 for both.
ಮಾದರಿ<ಬಗೆ_ಹೆಸರು T>
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ maskedNormDiffL1_8(ಸ್ಥಿರಾಂಕ T* s1, ಸ್ಥಿರಾಂಕ T* s2, ಸ್ಥಿರಾಂಕ uchar* ಮುಖವಾಡ, ಪೂರ್ಣಾಂಕ len) {
    ಪೂರ್ಣಾಂಕ i = 0; ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ vstep = VTraits<v_uint8>::vlanes();
    ಸ್ಥಿರಾಂಕ v_uint8 one = vx_setall_u8(1);
    v_uint32 acc = vx_setzero_u32();
    ಗಾಗಿ (; i <= len - vstep; i += vstep) {
        v_uint8 m  = v_gt(vx_ತುಂಬು(ಮುಖವಾಡ + i), vx_setzero_u8());
        v_uint8 ad = v_and(v_absdiff(vx_ತುಂಬು(s1 + i), vx_ತುಂಬು(s2 + i)), m);
        acc = v_dotprod_expand_fast(ad, one, acc);
    }
    ಪೂರ್ಣಾಂಕ ಫಲಿತಾಂಶ = (ಪೂರ್ಣಾಂಕ)v_reduce_ಮೊತ್ತ(acc);
    ಗಾಗಿ (; i < len; i++) ಆದರೆ (ಮುಖವಾಡ[i]) ಫಲಿತಾಂಶ += (ಪೂರ್ಣಾಂಕ)cv_absdiff(s1[i], s2[i]);
    ಹಿಂತಿರುಗಿ ಫಲಿತಾಂಶ;
}
ಮಾದರಿ<ಬಗೆ_ಹೆಸರು T>
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ maskedNormDiffL2_8(ಸ್ಥಿರಾಂಕ T* s1, ಸ್ಥಿರಾಂಕ T* s2, ಸ್ಥಿರಾಂಕ uchar* ಮುಖವಾಡ, ಪೂರ್ಣಾಂಕ len) {
    ಪೂರ್ಣಾಂಕ i = 0; ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ vstep = VTraits<v_uint8>::vlanes();
    v_uint32 acc = vx_setzero_u32();
    ಗಾಗಿ (; i <= len - vstep; i += vstep) {
        v_uint8 m  = v_gt(vx_ತುಂಬು(ಮುಖವಾಡ + i), vx_setzero_u8());
        v_uint8 ad = v_and(v_absdiff(vx_ತುಂಬು(s1 + i), vx_ತುಂಬು(s2 + i)), m);
        acc = v_dotprod_expand_fast(ad, ad, acc);
    }
    ಪೂರ್ಣಾಂಕ ಫಲಿತಾಂಶ = (ಪೂರ್ಣಾಂಕ)v_reduce_ಮೊತ್ತ(acc);
    ಗಾಗಿ (; i < len; i++) ಆದರೆ (ಮುಖವಾಡ[i]) { ಪೂರ್ಣಾಂಕ v = (ಪೂರ್ಣಾಂಕ)s1[i] - (ಪೂರ್ಣಾಂಕ)s2[i]; ಫಲಿತಾಂಶ += v*v; }
    ಹಿಂತಿರುಗಿ ಫಲಿತಾಂಶ;
}

// Shared 16-bit cn==1 masked L1 kernel (ushort/short): v_absdiff yields v_uint16 for both.
ಮಾದರಿ<ಬಗೆ_ಹೆಸರು T>
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ maskedNormDiffL1_16(ಸ್ಥಿರಾಂಕ T* s1, ಸ್ಥಿರಾಂಕ T* s2, ಸ್ಥಿರಾಂಕ uchar* ಮುಖವಾಡ, ಪೂರ್ಣಾಂಕ len) {
    ಪೂರ್ಣಾಂಕ i = 0; ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ vstep = VTraits<v_uint16>::vlanes();
    v_uint32 acc = vx_setzero_u32();
    ಗಾಗಿ (; i <= len - vstep; i += vstep) {
        v_uint16 m  = v_gt(vx_ತುಂಬು_expand(ಮುಖವಾಡ + i), vx_setzero_u16());
        v_uint16 ad = v_and(v_absdiff(vx_ತುಂಬು(s1 + i), vx_ತುಂಬು(s2 + i)), m);
        v_uint32 lo, hi; v_expand(ad, lo, hi);
        acc = v_ಕೂಡಿಸು(acc, v_ಕೂಡಿಸು(lo, hi));
    }
    ಪೂರ್ಣಾಂಕ ಫಲಿತಾಂಶ = (ಪೂರ್ಣಾಂಕ)v_reduce_ಮೊತ್ತ(acc);
    ಗಾಗಿ (; i < len; i++) ಆದರೆ (ಮುಖವಾಡ[i]) ಫಲಿತಾಂಶ += (ಪೂರ್ಣಾಂಕ)cv_absdiff(s1[i], s2[i]);
    ಹಿಂತಿರುಗಿ ಫಲಿತಾಂಶ;
}

ಮಾದರಿ<> ರಚನೆ MaskedNormDiffInf_SIMD<uchar, ಪೂರ್ಣಾಂಕ> {
    ಪೂರ್ಣಾಂಕ ಕ್ರಿಯಾಕಾರಕ()(ಸ್ಥಿರಾಂಕ uchar* s1, ಸ್ಥಿರಾಂಕ uchar* s2, ಸ್ಥಿರಾಂಕ uchar* ಮುಖವಾಡ, ಪೂರ್ಣಾಂಕ len, ಪೂರ್ಣಾಂಕ cn) ಸ್ಥಿರಾಂಕ {
        ಆದರೆ (cn == 1) ಹಿಂತಿರುಗಿ maskedNormDiffInfCn1<uchar, ಪೂರ್ಣಾಂಕ>(s1, s2, ಮುಖವಾಡ, len);
        ಆದರೆ (cn == 4 && len >= VTraits<v_uint8>::vlanes()) {
            ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ vstep = VTraits<v_uint8>::vlanes();
            v_uint8 acc = vx_setzero_u8();
            ಪೂರ್ಣಾಂಕ i = 0;
            ಗಾಗಿ (;;) {
                ಆದರೆ (i > len - vstep) i = len - vstep; // back-step (max is idempotent)
                v_uint8 a0,a1,a2,a3,b0,b1,b2,b3;
                v_ತುಂಬು_deinterleave(s1 + i*4, a0,a1,a2,a3);
                v_ತುಂಬು_deinterleave(s2 + i*4, b0,b1,b2,b3);
                v_uint8 ad = v_max(v_max(v_absdiff(a0,b0), v_absdiff(a1,b1)),
                                   v_max(v_absdiff(a2,b2), v_absdiff(a3,b3)));
                v_uint8 m = v_gt(vx_ತುಂಬು(ಮುಖವಾಡ + i), vx_setzero_u8());
                acc = v_max(acc, v_and(ad, m));
                ಆದರೆ (i >= len - vstep) ನಿಲ್ಲಿಸು;
                i += vstep;
            }
            ಹಿಂತಿರುಗಿ (ಪೂರ್ಣಾಂಕ)v_reduce_max(acc);
        }
        ಹಿಂತಿರುಗಿ masked_Norm_Diff_Inf_ಬಾಲ<uchar, ಪೂರ್ಣಾಂಕ>(s1, s2, ಮುಖವಾಡ, len, cn, 0);
    }
};
ಮಾದರಿ<> ರಚನೆ MaskedNormDiffInf_SIMD<schar, ಪೂರ್ಣಾಂಕ> {
    ಪೂರ್ಣಾಂಕ ಕ್ರಿಯಾಕಾರಕ()(ಸ್ಥಿರಾಂಕ schar* s1, ಸ್ಥಿರಾಂಕ schar* s2, ಸ್ಥಿರಾಂಕ uchar* ಮುಖವಾಡ, ಪೂರ್ಣಾಂಕ len, ಪೂರ್ಣಾಂಕ cn) ಸ್ಥಿರಾಂಕ {
        ಆದರೆ (cn == 1) ಹಿಂತಿರುಗಿ maskedNormDiffInfCn1<schar, ಪೂರ್ಣಾಂಕ>(s1, s2, ಮುಖವಾಡ, len);
        ಹಿಂತಿರುಗಿ masked_Norm_Diff_Inf_ಬಾಲ<schar, ಪೂರ್ಣಾಂಕ>(s1, s2, ಮುಖವಾಡ, len, cn, 0);
    }
};
ಮಾದರಿ<> ರಚನೆ MaskedNormDiffInf_SIMD<ushort, ಪೂರ್ಣಾಂಕ> {
    ಪೂರ್ಣಾಂಕ ಕ್ರಿಯಾಕಾರಕ()(ಸ್ಥಿರಾಂಕ ushort* s1, ಸ್ಥಿರಾಂಕ ushort* s2, ಸ್ಥಿರಾಂಕ uchar* ಮುಖವಾಡ, ಪೂರ್ಣಾಂಕ len, ಪೂರ್ಣಾಂಕ cn) ಸ್ಥಿರಾಂಕ {
        ಆದರೆ (cn == 1) ಹಿಂತಿರುಗಿ maskedNormDiffInfCn1<ushort, ಪೂರ್ಣಾಂಕ>(s1, s2, ಮುಖವಾಡ, len);
        ಹಿಂತಿರುಗಿ masked_Norm_Diff_Inf_ಬಾಲ<ushort, ಪೂರ್ಣಾಂಕ>(s1, s2, ಮುಖವಾಡ, len, cn, 0);
    }
};
ಮಾದರಿ<> ರಚನೆ MaskedNormDiffInf_SIMD<ಗಿಡ್ಡ, ಪೂರ್ಣಾಂಕ> {
    ಪೂರ್ಣಾಂಕ ಕ್ರಿಯಾಕಾರಕ()(ಸ್ಥಿರಾಂಕ ಗಿಡ್ಡ* s1, ಸ್ಥಿರಾಂಕ ಗಿಡ್ಡ* s2, ಸ್ಥಿರಾಂಕ uchar* ಮುಖವಾಡ, ಪೂರ್ಣಾಂಕ len, ಪೂರ್ಣಾಂಕ cn) ಸ್ಥಿರಾಂಕ {
        ಆದರೆ (cn == 1) ಹಿಂತಿರುಗಿ maskedNormDiffInfCn1<ಗಿಡ್ಡ, ಪೂರ್ಣಾಂಕ>(s1, s2, ಮುಖವಾಡ, len);
        ಹಿಂತಿರುಗಿ masked_Norm_Diff_Inf_ಬಾಲ<ಗಿಡ್ಡ, ಪೂರ್ಣಾಂಕ>(s1, s2, ಮುಖವಾಡ, len, cn, 0);
    }
};
ಮಾದರಿ<> ರಚನೆ MaskedNormDiffInf_SIMD<ಪೂರ್ಣಾಂಕ, ಚಿಹ್ನೆರಹಿತ> {
    ಚಿಹ್ನೆರಹಿತ ಕ್ರಿಯಾಕಾರಕ()(ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ* s1, ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ* s2, ಸ್ಥಿರಾಂಕ uchar* ಮುಖವಾಡ, ಪೂರ್ಣಾಂಕ len, ಪೂರ್ಣಾಂಕ cn) ಸ್ಥಿರಾಂಕ {
        ಆದರೆ (cn == 1) ಹಿಂತಿರುಗಿ maskedNormDiffInfCn1<ಪೂರ್ಣಾಂಕ, ಚಿಹ್ನೆರಹಿತ>(s1, s2, ಮುಖವಾಡ, len);
        ಹಿಂತಿರುಗಿ masked_Norm_Diff_Inf_ಬಾಲ<ಪೂರ್ಣಾಂಕ, ಚಿಹ್ನೆರಹಿತ>(s1, s2, ಮುಖವಾಡ, len, cn, 0u);
    }
};
ಮಾದರಿ<> ರಚನೆ MaskedNormDiffInf_SIMD<ದಶಮಾಂಶ, ದಶಮಾಂಶ> {
    ದಶಮಾಂಶ ಕ್ರಿಯಾಕಾರಕ()(ಸ್ಥಿರಾಂಕ ದಶಮಾಂಶ* s1, ಸ್ಥಿರಾಂಕ ದಶಮಾಂಶ* s2, ಸ್ಥಿರಾಂಕ uchar* ಮುಖವಾಡ, ಪೂರ್ಣಾಂಕ len, ಪೂರ್ಣಾಂಕ cn) ಸ್ಥಿರಾಂಕ {
        ಆದರೆ (cn == 1) ಹಿಂತಿರುಗಿ maskedNormDiffInfCn1<ದಶಮಾಂಶ, ದಶಮಾಂಶ>(s1, s2, ಮುಖವಾಡ, len);
        ಹಿಂತಿರುಗಿ masked_Norm_Diff_Inf_ಬಾಲ<ದಶಮಾಂಶ, ದಶಮಾಂಶ>(s1, s2, ಮುಖವಾಡ, len, cn, 0.0f);
    }
};

ಮಾದರಿ<> ರಚನೆ MaskedNormDiffL1_SIMD<uchar, ಪೂರ್ಣಾಂಕ> {
    ಪೂರ್ಣಾಂಕ ಕ್ರಿಯಾಕಾರಕ()(ಸ್ಥಿರಾಂಕ uchar* s1, ಸ್ಥಿರಾಂಕ uchar* s2, ಸ್ಥಿರಾಂಕ uchar* ಮುಖವಾಡ, ಪೂರ್ಣಾಂಕ len, ಪೂರ್ಣಾಂಕ cn) ಸ್ಥಿರಾಂಕ {
        ಆದರೆ (cn == 1) ಹಿಂತಿರುಗಿ maskedNormDiffL1_8(s1, s2, ಮುಖವಾಡ, len);
        ಆದರೆ (cn == 4) {
            ಪೂರ್ಣಾಂಕ i = 0; ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ vstep = VTraits<v_uint8>::vlanes();
            ಸ್ಥಿರಾಂಕ v_uint8 one = vx_setall_u8(1);
            v_uint32 acc = vx_setzero_u32();
            ಗಾಗಿ (; i <= len - vstep; i += vstep) {
                v_uint8 a0,a1,a2,a3,b0,b1,b2,b3;
                v_ತುಂಬು_deinterleave(s1 + i*4, a0,a1,a2,a3);
                v_ತುಂಬು_deinterleave(s2 + i*4, b0,b1,b2,b3);
                v_uint8 m = v_gt(vx_ತುಂಬು(ಮುಖವಾಡ + i), vx_setzero_u8());
                acc = v_dotprod_expand_fast(v_and(v_absdiff(a0,b0), m), one, acc);
                acc = v_dotprod_expand_fast(v_and(v_absdiff(a1,b1), m), one, acc);
                acc = v_dotprod_expand_fast(v_and(v_absdiff(a2,b2), m), one, acc);
                acc = v_dotprod_expand_fast(v_and(v_absdiff(a3,b3), m), one, acc);
            }
            ಪೂರ್ಣಾಂಕ ಫಲಿತಾಂಶ = (ಪೂರ್ಣಾಂಕ)v_reduce_ಮೊತ್ತ(acc);
            ಗಾಗಿ (; i < len; i++) ಆದರೆ (ಮುಖವಾಡ[i]) {
                ಸ್ಥಿರಾಂಕ uchar* e1 = s1 + i*4; ಸ್ಥಿರಾಂಕ uchar* e2 = s2 + i*4;
                ಗಾಗಿ (ಪೂರ್ಣಾಂಕ k = 0; k < 4; k++) ಫಲಿತಾಂಶ += (ಪೂರ್ಣಾಂಕ)cv_absdiff(e1[k], e2[k]);
            }
            ಹಿಂತಿರುಗಿ ಫಲಿತಾಂಶ;
        }
        ಹಿಂತಿರುಗಿ masked_Norm_Diff_L1_ಬಾಲ<uchar, ಪೂರ್ಣಾಂಕ>(s1, s2, ಮುಖವಾಡ, len, cn, 0);
    }
};
ಮಾದರಿ<> ರಚನೆ MaskedNormDiffL1_SIMD<schar, ಪೂರ್ಣಾಂಕ> {
    ಪೂರ್ಣಾಂಕ ಕ್ರಿಯಾಕಾರಕ()(ಸ್ಥಿರಾಂಕ schar* s1, ಸ್ಥಿರಾಂಕ schar* s2, ಸ್ಥಿರಾಂಕ uchar* ಮುಖವಾಡ, ಪೂರ್ಣಾಂಕ len, ಪೂರ್ಣಾಂಕ cn) ಸ್ಥಿರಾಂಕ {
        ಆದರೆ (cn == 1) ಹಿಂತಿರುಗಿ maskedNormDiffL1_8(s1, s2, ಮುಖವಾಡ, len);
        ಹಿಂತಿರುಗಿ masked_Norm_Diff_L1_ಬಾಲ<schar, ಪೂರ್ಣಾಂಕ>(s1, s2, ಮುಖವಾಡ, len, cn, 0);
    }
};
ಮಾದರಿ<> ರಚನೆ MaskedNormDiffL1_SIMD<ushort, ಪೂರ್ಣಾಂಕ> {
    ಪೂರ್ಣಾಂಕ ಕ್ರಿಯಾಕಾರಕ()(ಸ್ಥಿರಾಂಕ ushort* s1, ಸ್ಥಿರಾಂಕ ushort* s2, ಸ್ಥಿರಾಂಕ uchar* ಮುಖವಾಡ, ಪೂರ್ಣಾಂಕ len, ಪೂರ್ಣಾಂಕ cn) ಸ್ಥಿರಾಂಕ {
        ಆದರೆ (cn == 1) ಹಿಂತಿರುಗಿ maskedNormDiffL1_16(s1, s2, ಮುಖವಾಡ, len);
        ಹಿಂತಿರುಗಿ masked_Norm_Diff_L1_ಬಾಲ<ushort, ಪೂರ್ಣಾಂಕ>(s1, s2, ಮುಖವಾಡ, len, cn, 0);
    }
};
ಮಾದರಿ<> ರಚನೆ MaskedNormDiffL1_SIMD<ಗಿಡ್ಡ, ಪೂರ್ಣಾಂಕ> {
    ಪೂರ್ಣಾಂಕ ಕ್ರಿಯಾಕಾರಕ()(ಸ್ಥಿರಾಂಕ ಗಿಡ್ಡ* s1, ಸ್ಥಿರಾಂಕ ಗಿಡ್ಡ* s2, ಸ್ಥಿರಾಂಕ uchar* ಮುಖವಾಡ, ಪೂರ್ಣಾಂಕ len, ಪೂರ್ಣಾಂಕ cn) ಸ್ಥಿರಾಂಕ {
        ಆದರೆ (cn == 1) ಹಿಂತಿರುಗಿ maskedNormDiffL1_16(s1, s2, ಮುಖವಾಡ, len);
        ಹಿಂತಿರುಗಿ masked_Norm_Diff_L1_ಬಾಲ<ಗಿಡ್ಡ, ಪೂರ್ಣಾಂಕ>(s1, s2, ಮುಖವಾಡ, len, cn, 0);
    }
};

ಮಾದರಿ<> ರಚನೆ MaskedNormDiffL2_SIMD<uchar, ಪೂರ್ಣಾಂಕ> {
    ಪೂರ್ಣಾಂಕ ಕ್ರಿಯಾಕಾರಕ()(ಸ್ಥಿರಾಂಕ uchar* s1, ಸ್ಥಿರಾಂಕ uchar* s2, ಸ್ಥಿರಾಂಕ uchar* ಮುಖವಾಡ, ಪೂರ್ಣಾಂಕ len, ಪೂರ್ಣಾಂಕ cn) ಸ್ಥಿರಾಂಕ {
        ಆದರೆ (cn == 1) ಹಿಂತಿರುಗಿ maskedNormDiffL2_8(s1, s2, ಮುಖವಾಡ, len);
        ಆದರೆ (cn == 4) {
            ಪೂರ್ಣಾಂಕ i = 0; ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ vstep = VTraits<v_uint8>::vlanes();
            v_uint32 acc = vx_setzero_u32();
            ಗಾಗಿ (; i <= len - vstep; i += vstep) {
                v_uint8 a0,a1,a2,a3,b0,b1,b2,b3;
                v_ತುಂಬು_deinterleave(s1 + i*4, a0,a1,a2,a3);
                v_ತುಂಬು_deinterleave(s2 + i*4, b0,b1,b2,b3);
                v_uint8 m = v_gt(vx_ತುಂಬು(ಮುಖವಾಡ + i), vx_setzero_u8());
                v_uint8 d0 = v_and(v_absdiff(a0,b0), m), d1 = v_and(v_absdiff(a1,b1), m);
                v_uint8 d2 = v_and(v_absdiff(a2,b2), m), d3 = v_and(v_absdiff(a3,b3), m);
                acc = v_dotprod_expand_fast(d0, d0, acc);
                acc = v_dotprod_expand_fast(d1, d1, acc);
                acc = v_dotprod_expand_fast(d2, d2, acc);
                acc = v_dotprod_expand_fast(d3, d3, acc);
            }
            ಪೂರ್ಣಾಂಕ ಫಲಿತಾಂಶ = (ಪೂರ್ಣಾಂಕ)v_reduce_ಮೊತ್ತ(acc);
            ಗಾಗಿ (; i < len; i++) ಆದರೆ (ಮುಖವಾಡ[i]) {
                ಸ್ಥಿರಾಂಕ uchar* e1 = s1 + i*4; ಸ್ಥಿರಾಂಕ uchar* e2 = s2 + i*4;
                ಗಾಗಿ (ಪೂರ್ಣಾಂಕ k = 0; k < 4; k++) { ಪೂರ್ಣಾಂಕ v = (ಪೂರ್ಣಾಂಕ)e1[k] - (ಪೂರ್ಣಾಂಕ)e2[k]; ಫಲಿತಾಂಶ += v*v; }
            }
            ಹಿಂತಿರುಗಿ ಫಲಿತಾಂಶ;
        }
        ಹಿಂತಿರುಗಿ masked_Norm_Diff_L2_ಬಾಲ<uchar, ಪೂರ್ಣಾಂಕ>(s1, s2, ಮುಖವಾಡ, len, cn, 0);
    }
};
ಮಾದರಿ<> ರಚನೆ MaskedNormDiffL2_SIMD<schar, ಪೂರ್ಣಾಂಕ> {
    ಪೂರ್ಣಾಂಕ ಕ್ರಿಯಾಕಾರಕ()(ಸ್ಥಿರಾಂಕ schar* s1, ಸ್ಥಿರಾಂಕ schar* s2, ಸ್ಥಿರಾಂಕ uchar* ಮುಖವಾಡ, ಪೂರ್ಣಾಂಕ len, ಪೂರ್ಣಾಂಕ cn) ಸ್ಥಿರಾಂಕ {
        ಆದರೆ (cn == 1) ಹಿಂತಿರುಗಿ maskedNormDiffL2_8(s1, s2, ಮುಖವಾಡ, len);
        ಹಿಂತಿರುಗಿ masked_Norm_Diff_L2_ಬಾಲ<schar, ಪೂರ್ಣಾಂಕ>(s1, s2, ಮುಖವಾಡ, len, cn, 0);
    }
};

#ಆದರೆ CV_SIMD_64F
// Depths whose L1/L2 accumulate in double.
ಮಾದರಿ<> ರಚನೆ MaskedNormDiffL1_SIMD<ಪೂರ್ಣಾಂಕ, ದ್ವಿಗುಣ> {
    ದ್ವಿಗುಣ ಕ್ರಿಯಾಕಾರಕ()(ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ* s1, ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ* s2, ಸ್ಥಿರಾಂಕ uchar* ಮುಖವಾಡ, ಪೂರ್ಣಾಂಕ len, ಪೂರ್ಣಾಂಕ cn) ಸ್ಥಿರಾಂಕ {
        ಆದರೆ (cn != 1) ಹಿಂತಿರುಗಿ masked_Norm_Diff_L1_ಬಾಲ<ಪೂರ್ಣಾಂಕ, ದ್ವಿಗುಣ>(s1, s2, ಮುಖವಾಡ, len, cn, 0.0);
        ಪೂರ್ಣಾಂಕ i = 0; ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ vstep = VTraits<v_int32>::vlanes();
        v_float64 acc = vx_setzero_f64();
        ಗಾಗಿ (; i <= len - vstep; i += vstep) {
            v_uint32 m  = v_gt(vx_ತುಂಬು_expand_q(ಮುಖವಾಡ + i), vx_setzero_u32());
            v_uint32 ad = v_and(v_absdiff(vx_ತುಂಬು(s1 + i), vx_ತುಂಬು(s2 + i)), m);
            v_uint64 e0, e1; v_expand(ad, e0, e1);
            acc = v_ಕೂಡಿಸು(acc, v_cvt_f64(v_reinterpret_as_s64(e0)));
            acc = v_ಕೂಡಿಸು(acc, v_cvt_f64(v_reinterpret_as_s64(e1)));
        }
        ದ್ವಿಗುಣ ಫಲಿತಾಂಶ = v_reduce_ಮೊತ್ತ(acc);
        ಗಾಗಿ (; i < len; i++) ಆದರೆ (ಮುಖವಾಡ[i]) ಫಲಿತಾಂಶ += (ದ್ವಿಗುಣ)cv_absdiff(s1[i], s2[i]);
        ಹಿಂತಿರುಗಿ ಫಲಿತಾಂಶ;
    }
};
ಮಾದರಿ<> ರಚನೆ MaskedNormDiffL1_SIMD<ದಶಮಾಂಶ, ದ್ವಿಗುಣ> {
    ದ್ವಿಗುಣ ಕ್ರಿಯಾಕಾರಕ()(ಸ್ಥಿರಾಂಕ ದಶಮಾಂಶ* s1, ಸ್ಥಿರಾಂಕ ದಶಮಾಂಶ* s2, ಸ್ಥಿರಾಂಕ uchar* ಮುಖವಾಡ, ಪೂರ್ಣಾಂಕ len, ಪೂರ್ಣಾಂಕ cn) ಸ್ಥಿರಾಂಕ {
        ಆದರೆ (cn != 1) ಹಿಂತಿರುಗಿ masked_Norm_Diff_L1_ಬಾಲ<ದಶಮಾಂಶ, ದ್ವಿಗುಣ>(s1, s2, ಮುಖವಾಡ, len, cn, 0.0);
        ಪೂರ್ಣಾಂಕ i = 0; ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ vstep = VTraits<v_float32>::vlanes();
        v_float64 acc0 = vx_setzero_f64(), acc1 = vx_setzero_f64();
        ಗಾಗಿ (; i <= len - vstep; i += vstep) {
            v_float32 m  = v_reinterpret_as_f32(v_gt(vx_ತುಂಬು_expand_q(ಮುಖವಾಡ + i), vx_setzero_u32()));
            v_float32 ad = v_and(v_absdiff(vx_ತುಂಬು(s1 + i), vx_ತುಂಬು(s2 + i)), m);
            acc0 = v_ಕೂಡಿಸು(acc0, v_cvt_f64(ad));
            acc1 = v_ಕೂಡಿಸು(acc1, v_cvt_f64_ಮೇಲಿನ(ad));
        }
        ದ್ವಿಗುಣ ಫಲಿತಾಂಶ = v_reduce_ಮೊತ್ತ(v_ಕೂಡಿಸು(acc0, acc1));
        ಗಾಗಿ (; i < len; i++) ಆದರೆ (ಮುಖವಾಡ[i]) ಫಲಿತಾಂಶ += (ದ್ವಿಗುಣ)cv_absdiff(s1[i], s2[i]);
        ಹಿಂತಿರುಗಿ ಫಲಿತಾಂಶ;
    }
};

ಮಾದರಿ<ಬಗೆ_ಹೆಸರು T>
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ದ್ವಿಗುಣ maskedNormDiffL2_16(ಸ್ಥಿರಾಂಕ T* s1, ಸ್ಥಿರಾಂಕ T* s2, ಸ್ಥಿರಾಂಕ uchar* ಮುಖವಾಡ, ಪೂರ್ಣಾಂಕ len, ಪೂರ್ಣಾಂಕ cn) {
    ಆದರೆ (cn != 1) ಹಿಂತಿರುಗಿ masked_Norm_Diff_L2_ಬಾಲ<T, ದ್ವಿಗುಣ>(s1, s2, ಮುಖವಾಡ, len, cn, 0.0);
    ಪೂರ್ಣಾಂಕ i = 0; ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ vstep = VTraits<v_uint16>::vlanes();
    v_float64 acc = vx_setzero_f64();
    ಗಾಗಿ (; i <= len - vstep; i += vstep) {
        v_uint16 m  = v_gt(vx_ತುಂಬು_expand(ಮುಖವಾಡ + i), vx_setzero_u16());
        v_uint16 ad = v_and(v_absdiff(vx_ತುಂಬು(s1 + i), vx_ತುಂಬು(s2 + i)), m);
        v_uint64 u = v_dotprod_expand_fast(ad, ad);
        acc = v_ಕೂಡಿಸು(acc, v_cvt_f64(v_reinterpret_as_s64(u)));
    }
    ದ್ವಿಗುಣ ಫಲಿತಾಂಶ = v_reduce_ಮೊತ್ತ(acc);
    ಗಾಗಿ (; i < len; i++) ಆದರೆ (ಮುಖವಾಡ[i]) { ದ್ವಿಗುಣ v = (ದ್ವಿಗುಣ)s1[i] - (ದ್ವಿಗುಣ)s2[i]; ಫಲಿತಾಂಶ += v*v; }
    ಹಿಂತಿರುಗಿ ಫಲಿತಾಂಶ;
}
ಮಾದರಿ<> ರಚನೆ MaskedNormDiffL2_SIMD<ushort, ದ್ವಿಗುಣ> {
    ದ್ವಿಗುಣ ಕ್ರಿಯಾಕಾರಕ()(ಸ್ಥಿರಾಂಕ ushort* s1, ಸ್ಥಿರಾಂಕ ushort* s2, ಸ್ಥಿರಾಂಕ uchar* ಮುಖವಾಡ, ಪೂರ್ಣಾಂಕ len, ಪೂರ್ಣಾಂಕ cn) ಸ್ಥಿರಾಂಕ
    { ಹಿಂತಿರುಗಿ maskedNormDiffL2_16(s1, s2, ಮುಖವಾಡ, len, cn); } };
ಮಾದರಿ<> ರಚನೆ MaskedNormDiffL2_SIMD<ಗಿಡ್ಡ, ದ್ವಿಗುಣ> {
    ದ್ವಿಗುಣ ಕ್ರಿಯಾಕಾರಕ()(ಸ್ಥಿರಾಂಕ ಗಿಡ್ಡ* s1, ಸ್ಥಿರಾಂಕ ಗಿಡ್ಡ* s2, ಸ್ಥಿರಾಂಕ uchar* ಮುಖವಾಡ, ಪೂರ್ಣಾಂಕ len, ಪೂರ್ಣಾಂಕ cn) ಸ್ಥಿರಾಂಕ
    { ಹಿಂತಿರುಗಿ maskedNormDiffL2_16(s1, s2, ಮುಖವಾಡ, len, cn); } };

ಮಾದರಿ<> ರಚನೆ MaskedNormDiffL2_SIMD<ಪೂರ್ಣಾಂಕ, ದ್ವಿಗುಣ> {
    ದ್ವಿಗುಣ ಕ್ರಿಯಾಕಾರಕ()(ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ* s1, ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ* s2, ಸ್ಥಿರಾಂಕ uchar* ಮುಖವಾಡ, ಪೂರ್ಣಾಂಕ len, ಪೂರ್ಣಾಂಕ cn) ಸ್ಥಿರಾಂಕ {
        ಆದರೆ (cn != 1) ಹಿಂತಿರುಗಿ masked_Norm_Diff_L2_ಬಾಲ<ಪೂರ್ಣಾಂಕ, ದ್ವಿಗುಣ>(s1, s2, ಮುಖವಾಡ, len, cn, 0.0);
        ಪೂರ್ಣಾಂಕ i = 0; ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ vstep = VTraits<v_int32>::vlanes();
        v_float64 r0 = vx_setzero_f64(), r1 = vx_setzero_f64();
        ಗಾಗಿ (; i <= len - vstep; i += vstep) {
            v_uint32 m  = v_gt(vx_ತುಂಬು_expand_q(ಮುಖವಾಡ + i), vx_setzero_u32());
            v_uint32 ad = v_and(v_absdiff(vx_ತುಂಬು(s1 + i), vx_ತುಂಬು(s2 + i)), m);
            v_uint64 e0, e1; v_expand(ad, e0, e1);
            v_float64 f0 = v_cvt_f64(v_reinterpret_as_s64(e0)), f1 = v_cvt_f64(v_reinterpret_as_s64(e1));
            r0 = v_fma(f0, f0, r0); r1 = v_fma(f1, f1, r1);
        }
        ದ್ವಿಗುಣ ಫಲಿತಾಂಶ = v_reduce_ಮೊತ್ತ(v_ಕೂಡಿಸು(r0, r1));
        ಗಾಗಿ (; i < len; i++) ಆದರೆ (ಮುಖವಾಡ[i]) { ದ್ವಿಗುಣ v = (ದ್ವಿಗುಣ)s1[i] - (ದ್ವಿಗುಣ)s2[i]; ಫಲಿತಾಂಶ += v*v; }
        ಹಿಂತಿರುಗಿ ಫಲಿತಾಂಶ;
    }
};
ಮಾದರಿ<> ರಚನೆ MaskedNormDiffL2_SIMD<ದಶಮಾಂಶ, ದ್ವಿಗುಣ> {
    ದ್ವಿಗುಣ ಕ್ರಿಯಾಕಾರಕ()(ಸ್ಥಿರಾಂಕ ದಶಮಾಂಶ* s1, ಸ್ಥಿರಾಂಕ ದಶಮಾಂಶ* s2, ಸ್ಥಿರಾಂಕ uchar* ಮುಖವಾಡ, ಪೂರ್ಣಾಂಕ len, ಪೂರ್ಣಾಂಕ cn) ಸ್ಥಿರಾಂಕ {
        ಆದರೆ (cn != 1) ಹಿಂತಿರುಗಿ masked_Norm_Diff_L2_ಬಾಲ<ದಶಮಾಂಶ, ದ್ವಿಗುಣ>(s1, s2, ಮುಖವಾಡ, len, cn, 0.0);
        ಪೂರ್ಣಾಂಕ i = 0; ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ vstep = VTraits<v_float32>::vlanes();
        v_float64 r0 = vx_setzero_f64(), r1 = vx_setzero_f64();
        ಗಾಗಿ (; i <= len - vstep; i += vstep) {
            v_float32 m  = v_reinterpret_as_f32(v_gt(vx_ತುಂಬು_expand_q(ಮುಖವಾಡ + i), vx_setzero_u32()));
            v_float32 ad = v_and(v_absdiff(vx_ತುಂಬು(s1 + i), vx_ತುಂಬು(s2 + i)), m);
            v_float64 f0 = v_cvt_f64(ad), f1 = v_cvt_f64_ಮೇಲಿನ(ad);
            r0 = v_fma(f0, f0, r0); r1 = v_fma(f1, f1, r1);
        }
        ದ್ವಿಗುಣ ಫಲಿತಾಂಶ = v_reduce_ಮೊತ್ತ(v_ಕೂಡಿಸು(r0, r1));
        ಗಾಗಿ (; i < len; i++) ಆದರೆ (ಮುಖವಾಡ[i]) { ದ್ವಿಗುಣ v = (ದ್ವಿಗುಣ)s1[i] - (ದ್ವಿಗುಣ)s2[i]; ಫಲಿತಾಂಶ += v*v; }
        ಹಿಂತಿರುಗಿ ಫಲಿತಾಂಶ;
    }
};
ಮಾದರಿ<>
ರಚನೆ MaskedNormDiffInf_SIMD<ದ್ವಿಗುಣ, ದ್ವಿಗುಣ> {
    ಸಾಲೊಳಗೆ ದ್ವಿಗುಣ ಕ್ರಿಯಾಕಾರಕ()(ಸ್ಥಿರಾಂಕ ದ್ವಿಗುಣ* s1, ಸ್ಥಿರಾಂಕ ದ್ವಿಗುಣ* s2, ಸ್ಥಿರಾಂಕ uchar* ಮುಖವಾಡ, ಪೂರ್ಣಾಂಕ len, ಪೂರ್ಣಾಂಕ cn) ಸ್ಥಿರಾಂಕ {
        ದ್ವಿಗುಣ ಫಲಿತಾಂಶ = 0.0;
        ಪೂರ್ಣಾಂಕ i = 0;
#ಆದರೆ CV_SIMD_64F || CV_SIMD_SCALABLE_64F
        ಆದರೆ (cn == 1) {
            ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ vstep = VTraits<v_float64>::vlanes();
            v_float64 acc0 = vx_setzero_f64(), acc1 = vx_setzero_f64();
            ಗಾಗಿ (; i <= len - 2*vstep; i += 2*vstep) {
                v_uint64 m0, m1;
                v_normmask_u64(ಮುಖವಾಡ + i, m0, m1);
                v_float64 d0 = v_absdiff(vx_ತುಂಬು(s1 + i), vx_ತುಂಬು(s2 + i));
                v_float64 d1 = v_absdiff(vx_ತುಂಬು(s1 + i + vstep), vx_ತುಂಬು(s2 + i + vstep));
                acc0 = v_max(acc0, v_reinterpret_as_f64(v_and(v_reinterpret_as_u64(d0), m0)));
                acc1 = v_max(acc1, v_reinterpret_as_f64(v_and(v_reinterpret_as_u64(d1), m1)));
            }
            ದ್ವಿಗುಣ t[VTraits<v_float64>::max_nlanes];
            vx_store(t, v_max(acc0, acc1));
            ಗಾಗಿ (ಪೂರ್ಣಾಂಕ k = 0; k < vstep; k++)
                ಫಲಿತಾಂಶ = std::max(ಫಲಿತಾಂಶ, t[k]);
        }
#ಆದರೆ_ಅಂತ್ಯ
        ಗಾಗಿ (; i < len; i++) ಆದರೆ (ಮುಖವಾಡ[i]) {
            ಸ್ಥಿರಾಂಕ ದ್ವಿಗುಣ* e1 = s1 + i*cn; ಸ್ಥಿರಾಂಕ ದ್ವಿಗುಣ* e2 = s2 + i*cn;
            ಗಾಗಿ (ಪೂರ್ಣಾಂಕ k = 0; k < cn; k++) ಫಲಿತಾಂಶ = std::max(ಫಲಿತಾಂಶ, std::ನಿರಪೇಕ್ಷ(e1[k] - e2[k]));
        }
        ಹಿಂತಿರುಗಿ ಫಲಿತಾಂಶ;
    }
};

ಮಾದರಿ<>
ರಚನೆ MaskedNormDiffL1_SIMD<ದ್ವಿಗುಣ, ದ್ವಿಗುಣ> {
    ದ್ವಿಗುಣ ಕ್ರಿಯಾಕಾರಕ()(ಸ್ಥಿರಾಂಕ ದ್ವಿಗುಣ* s1, ಸ್ಥಿರಾಂಕ ದ್ವಿಗುಣ* s2, ಸ್ಥಿರಾಂಕ uchar* ಮುಖವಾಡ, ಪೂರ್ಣಾಂಕ len, ಪೂರ್ಣಾಂಕ cn) ಸ್ಥಿರಾಂಕ {
        ದ್ವಿಗುಣ ಫಲಿತಾಂಶ = 0.0;
        ಆದರೆ (cn == 1) {
            ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ vstep = VTraits<v_float64>::vlanes();
            ಪೂರ್ಣಾಂಕ i = 0;
            v_float64 acc0 = vx_setzero_f64(), acc1 = vx_setzero_f64();
            ಗಾಗಿ (; i <= len - 2*vstep; i += 2*vstep) {
                v_uint64 m0, m1;
                v_normmask_u64(ಮುಖವಾಡ + i, m0, m1);
                v_float64 d0 = v_absdiff(vx_ತುಂಬು(s1 + i), vx_ತುಂಬು(s2 + i));
                v_float64 d1 = v_absdiff(vx_ತುಂಬು(s1 + i + vstep), vx_ತುಂಬು(s2 + i + vstep));
                acc0 = v_ಕೂಡಿಸು(acc0, v_reinterpret_as_f64(v_and(v_reinterpret_as_u64(d0), m0)));
                acc1 = v_ಕೂಡಿಸು(acc1, v_reinterpret_as_f64(v_and(v_reinterpret_as_u64(d1), m1)));
            }
            ಫಲಿತಾಂಶ = v_reduce_ಮೊತ್ತ(v_ಕೂಡಿಸು(acc0, acc1));
            ಗಾಗಿ (; i < len; i++) ಆದರೆ (ಮುಖವಾಡ[i]) ಫಲಿತಾಂಶ += std::ನಿರಪೇಕ್ಷ(s1[i] - s2[i]);
        } ಇಲ್ಲದಿದ್ದರೆ {
            ಫಲಿತಾಂಶ = masked_Norm_Diff_L1_ಬಾಲ<ದ್ವಿಗುಣ, ದ್ವಿಗುಣ>(s1, s2, ಮುಖವಾಡ, len, cn, 0.0);
        }
        ಹಿಂತಿರುಗಿ ಫಲಿತಾಂಶ;
    }
};

ಮಾದರಿ<>
ರಚನೆ MaskedNormDiffL2_SIMD<ದ್ವಿಗುಣ, ದ್ವಿಗುಣ> {
    ದ್ವಿಗುಣ ಕ್ರಿಯಾಕಾರಕ()(ಸ್ಥಿರಾಂಕ ದ್ವಿಗುಣ* s1, ಸ್ಥಿರಾಂಕ ದ್ವಿಗುಣ* s2, ಸ್ಥಿರಾಂಕ uchar* ಮುಖವಾಡ, ಪೂರ್ಣಾಂಕ len, ಪೂರ್ಣಾಂಕ cn) ಸ್ಥಿರಾಂಕ {
        ದ್ವಿಗುಣ ಫಲಿತಾಂಶ = 0.0;
        ಆದರೆ (cn == 1) {
            ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ vstep = VTraits<v_float64>::vlanes();
            ಪೂರ್ಣಾಂಕ i = 0;
            v_float64 acc0 = vx_setzero_f64(), acc1 = vx_setzero_f64();
            ಗಾಗಿ (; i <= len - 2*vstep; i += 2*vstep) {
                v_uint64 m0, m1;
                v_normmask_u64(ಮುಖವಾಡ + i, m0, m1);
                v_float64 d0 = v_sub(vx_ತುಂಬು(s1 + i), vx_ತುಂಬು(s2 + i));
                v_float64 d1 = v_sub(vx_ತುಂಬು(s1 + i + vstep), vx_ತುಂಬು(s2 + i + vstep));
                d0 = v_reinterpret_as_f64(v_and(v_reinterpret_as_u64(d0), m0));
                d1 = v_reinterpret_as_f64(v_and(v_reinterpret_as_u64(d1), m1));
                acc0 = v_fma(d0, d0, acc0);
                acc1 = v_fma(d1, d1, acc1);
            }
            ಫಲಿತಾಂಶ = v_reduce_ಮೊತ್ತ(v_ಕೂಡಿಸು(acc0, acc1));
            ಗಾಗಿ (; i < len; i++) ಆದರೆ (ಮುಖವಾಡ[i]) { ದ್ವಿಗುಣ v = s1[i] - s2[i]; ಫಲಿತಾಂಶ += v*v; }
        } ಇಲ್ಲದಿದ್ದರೆ {
            ಫಲಿತಾಂಶ = masked_Norm_Diff_L2_ಬಾಲ<ದ್ವಿಗುಣ, ದ್ವಿಗುಣ>(s1, s2, ಮುಖವಾಡ, len, cn, 0.0);
        }
        ಹಿಂತಿರುಗಿ ಫಲಿತಾಂಶ;
    }
};
#ಆದರೆ_ಅಂತ್ಯ // CV_SIMD_64F
#ಆದರೆ_ಅಂತ್ಯ // CV_SIMD

ಮಾದರಿ<ಬಗೆ_ಹೆಸರು T, ಬಗೆ_ಹೆಸರು ST> ಪೂರ್ಣಾಂಕ
normDiffInf_(ಸ್ಥಿರಾಂಕ T* src1, ಸ್ಥಿರಾಂಕ T* src2, ಸ್ಥಿರಾಂಕ uchar* ಮುಖವಾಡ, ST* ಫಲಿತಾಂಶ_2, ಪೂರ್ಣಾಂಕ len, ಪೂರ್ಣಾಂಕ cn) {
    ST ಫಲಿತಾಂಶ = *ಫಲಿತಾಂಶ_2;
    ಆದರೆ( !ಮುಖವಾಡ ) {
        NormDiffInf_SIMD<T, ST> op;
        ಫಲಿತಾಂಶ = std::max(ಫಲಿತಾಂಶ, op(src1, src2, len*cn));
    } ಇಲ್ಲದಿದ್ದರೆ {
        MaskedNormDiffInf_SIMD<T, ST> op;
        ಫಲಿತಾಂಶ = std::max(ಫಲಿತಾಂಶ, op(src1, src2, ಮುಖವಾಡ, len, cn));
    }
    *ಫಲಿತಾಂಶ_2 = ಫಲಿತಾಂಶ;
    ಹಿಂತಿರುಗಿ 0;
}

ಮಾದರಿ<ಬಗೆ_ಹೆಸರು T, ಬಗೆ_ಹೆಸರು ST> ಪೂರ್ಣಾಂಕ
normDiffL1_(ಸ್ಥಿರಾಂಕ T* src1, ಸ್ಥಿರಾಂಕ T* src2, ಸ್ಥಿರಾಂಕ uchar* ಮುಖವಾಡ, ST* ಫಲಿತಾಂಶ_2, ಪೂರ್ಣಾಂಕ len, ಪೂರ್ಣಾಂಕ cn) {
    ST ಫಲಿತಾಂಶ = *ಫಲಿತಾಂಶ_2;
    ಆದರೆ( !ಮುಖವಾಡ ) {
        NormDiffL1_SIMD<T, ST> op;
        ಫಲಿತಾಂಶ += op(src1, src2, len*cn);
    } ಇಲ್ಲದಿದ್ದರೆ {
        MaskedNormDiffL1_SIMD<T, ST> op;
        ಫಲಿತಾಂಶ += op(src1, src2, ಮುಖವಾಡ, len, cn);
    }
    *ಫಲಿತಾಂಶ_2 = ಫಲಿತಾಂಶ;
    ಹಿಂತಿರುಗಿ 0;
}

ಮಾದರಿ<ಬಗೆ_ಹೆಸರು T, ಬಗೆ_ಹೆಸರು ST> ಪೂರ್ಣಾಂಕ
normDiffL2_(ಸ್ಥಿರಾಂಕ T* src1, ಸ್ಥಿರಾಂಕ T* src2, ಸ್ಥಿರಾಂಕ uchar* ಮುಖವಾಡ, ST* ಫಲಿತಾಂಶ_2, ಪೂರ್ಣಾಂಕ len, ಪೂರ್ಣಾಂಕ cn) {
    ST ಫಲಿತಾಂಶ = *ಫಲಿತಾಂಶ_2;
    ಆದರೆ( !ಮುಖವಾಡ ) {
        NormDiffL2_SIMD<T, ST> op;
        ಫಲಿತಾಂಶ += op(src1, src2, len*cn);
    } ಇಲ್ಲದಿದ್ದರೆ {
        MaskedNormDiffL2_SIMD<T, ST> op;
        ಫಲಿತಾಂಶ += op(src1, src2, ಮುಖವಾಡ, len, cn);
    }
    *ಫಲಿತಾಂಶ_2 = ಫಲಿತಾಂಶ;
    ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
normDiffInf_Bool(ಸ್ಥಿರಾಂಕ uchar* src1, ಸ್ಥಿರಾಂಕ uchar* src2, ಸ್ಥಿರಾಂಕ uchar* ಮುಖವಾಡ, ಪೂರ್ಣಾಂಕ* ಫಲಿತಾಂಶ_2, ಪೂರ್ಣಾಂಕ len, ಪೂರ್ಣಾಂಕ cn)
{
    ಪೂರ್ಣಾಂಕ ಫಲಿತಾಂಶ = *ಫಲಿತಾಂಶ_2;
    ಆದರೆ( !ಮುಖವಾಡ )
    {
        ಗಾಗಿ( ಪೂರ್ಣಾಂಕ i = 0; i < len*cn; i++ ) {
            ಫಲಿತಾಂಶ = std::max(ಫಲಿತಾಂಶ, (ಪೂರ್ಣಾಂಕ)((src1[i] != 0) != (src2[i] != 0)));
            ಆದರೆ (ಫಲಿತಾಂಶ != 0)
                ನಿಲ್ಲಿಸು;
        }
    }
    ಇಲ್ಲದಿದ್ದರೆ
    {
        ಗಾಗಿ( ಪೂರ್ಣಾಂಕ i = 0; i < len; i++, src1 += cn, src2 += cn )
            ಆದರೆ( ಮುಖವಾಡ[i] )
            {
                ಗಾಗಿ( ಪೂರ್ಣಾಂಕ k = 0; k < cn; k++ )
                    ಫಲಿತಾಂಶ = std::max(ಫಲಿತಾಂಶ, (ಪೂರ್ಣಾಂಕ)((src1[k] != 0) != (src2[k] != 0)));
                ಆದರೆ (ಫಲಿತಾಂಶ != 0)
                    ನಿಲ್ಲಿಸು;
            }
    }
    *ಫಲಿತಾಂಶ_2 = ಫಲಿತಾಂಶ;
    ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
normDiffL1_Bool(ಸ್ಥಿರಾಂಕ uchar* src1, ಸ್ಥಿರಾಂಕ uchar* src2, ಸ್ಥಿರಾಂಕ uchar* ಮುಖವಾಡ, ಪೂರ್ಣಾಂಕ* ಫಲಿತಾಂಶ_2, ಪೂರ್ಣಾಂಕ len, ಪೂರ್ಣಾಂಕ cn)
{
    ಪೂರ್ಣಾಂಕ ಫಲಿತಾಂಶ = *ಫಲಿತಾಂಶ_2;
    ಆದರೆ( !ಮುಖವಾಡ )
    {
        ಗಾಗಿ( ಪೂರ್ಣಾಂಕ i = 0; i < len*cn; i++ )
            ಫಲಿತಾಂಶ += (ಪೂರ್ಣಾಂಕ)((src1[i] != 0) != (src2[i] != 0));
    }
    ಇಲ್ಲದಿದ್ದರೆ
    {
        ಗಾಗಿ( ಪೂರ್ಣಾಂಕ i = 0; i < len; i++, src1 += cn, src2 += cn )
            ಆದರೆ( ಮುಖವಾಡ[i] )
            {
                ಗಾಗಿ( ಪೂರ್ಣಾಂಕ k = 0; k < cn; k++ )
                    ಫಲಿತಾಂಶ += (ಪೂರ್ಣಾಂಕ)((src1[k] != 0) != (src2[k] != 0));
            }
    }
    *ಫಲಿತಾಂಶ_2 = ಫಲಿತಾಂಶ;
    ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
normDiffL2_Bool(ಸ್ಥಿರಾಂಕ uchar* src1, ಸ್ಥಿರಾಂಕ uchar* src2, ಸ್ಥಿರಾಂಕ uchar* ಮುಖವಾಡ, ಪೂರ್ಣಾಂಕ* ಫಲಿತಾಂಶ_2, ಪೂರ್ಣಾಂಕ len, ಪೂರ್ಣಾಂಕ cn)
{
    ಹಿಂತಿರುಗಿ normDiffL1_Bool(src1, src2, ಮುಖವಾಡ, ಫಲಿತಾಂಶ_2, len, cn);
}

#ವ್ಯಾಖ್ಯಾನಿಸು CV_DEF_NORM_FUNC(L, suffix, type, ntype) \
    ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ norm##L##_##suffix(ಸ್ಥಿರಾಂಕ type* src, ಸ್ಥಿರಾಂಕ uchar* ಮುಖವಾಡ, ntype* r, ಪೂರ್ಣಾಂಕ len, ಪೂರ್ಣಾಂಕ cn) \
{ CV_INSTRUMENT_REGION(); ಹಿಂತಿರುಗಿ norm##L##_(src, ಮುಖವಾಡ, r, len, cn); } \
    ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ normDiff##L##_##suffix(ಸ್ಥಿರಾಂಕ type* src1, ಸ್ಥಿರಾಂಕ type* src2, \
    ಸ್ಥಿರಾಂಕ uchar* ಮುಖವಾಡ, ntype* r, ಪೂರ್ಣಾಂಕ len, ಪೂರ್ಣಾಂಕ cn) \
{ ಹಿಂತಿರುಗಿ normDiff##L##_(src1, src2, ಮುಖವಾಡ, r, (ಪೂರ್ಣಾಂಕ)len, cn); }

#ವ್ಯಾಖ್ಯಾನಿಸು CV_DEF_NORM_ALL(suffix, type, inftype, l1type, l2type) \
    CV_DEF_NORM_FUNC(Inf, suffix, type, inftype) \
    CV_DEF_NORM_FUNC(L1, suffix, type, l1type) \
    CV_DEF_NORM_FUNC(L2, suffix, type, l2type)

CV_DEF_NORM_ALL(8u, uchar, ಪೂರ್ಣಾಂಕ, ಪೂರ್ಣಾಂಕ, ಪೂರ್ಣಾಂಕ)
CV_DEF_NORM_ALL(8s, schar, ಪೂರ್ಣಾಂಕ, ಪೂರ್ಣಾಂಕ, ಪೂರ್ಣಾಂಕ)
CV_DEF_NORM_ALL(16u, ushort, ಪೂರ್ಣಾಂಕ, ಪೂರ್ಣಾಂಕ, ದ್ವಿಗುಣ)
CV_DEF_NORM_ALL(16s, ಗಿಡ್ಡ, ಪೂರ್ಣಾಂಕ, ಪೂರ್ಣಾಂಕ, ದ್ವಿಗುಣ)
CV_DEF_NORM_ALL(32u, ಚಿಹ್ನೆರಹಿತ, ಚಿಹ್ನೆರಹಿತ, ದ್ವಿಗುಣ, ದ್ವಿಗುಣ)
CV_DEF_NORM_ALL(32s, ಪೂರ್ಣಾಂಕ, ಚಿಹ್ನೆರಹಿತ, ದ್ವಿಗುಣ, ದ್ವಿಗುಣ)
CV_DEF_NORM_ALL(32f, ದಶಮಾಂಶ, ದಶಮಾಂಶ, ದ್ವಿಗುಣ, ದ್ವಿಗುಣ)
CV_DEF_NORM_ALL(64f, ದ್ವಿಗುಣ, ದ್ವಿಗುಣ, ದ್ವಿಗುಣ, ದ್ವಿಗುಣ)
CV_DEF_NORM_ALL(64u, uint64, uint64, ದ್ವಿಗುಣ, ದ್ವಿಗುಣ)
CV_DEF_NORM_ALL(64s, int64, uint64, ದ್ವಿಗುಣ, ದ್ವಿಗುಣ)
CV_DEF_NORM_ALL(16f, hfloat, ದಶಮಾಂಶ, ದಶಮಾಂಶ, ದಶಮಾಂಶ)
CV_DEF_NORM_ALL(16bf, bfloat, ದಶಮಾಂಶ, ದಶಮಾಂಶ, ದಶಮಾಂಶ)

NormFunc getNormFunc(ಪೂರ್ಣಾಂಕ normType, ಪೂರ್ಣಾಂಕ ಆಳ)
{
    CV_INSTRUMENT_REGION();

    ಸ್ಥಿರ NormFunc normTab[3][CV_ಆಳ_MAX] =
    {
        {
            normWrap<uchar, ಪೂರ್ಣಾಂಕ, normInf_8u>,
            normWrap<schar, ಪೂರ್ಣಾಂಕ, normInf_8s>,
            normWrap<ushort, ಪೂರ್ಣಾಂಕ, normInf_16u>,
            normWrap<ಗಿಡ್ಡ, ಪೂರ್ಣಾಂಕ, normInf_16s>,
            normWrap<ಪೂರ್ಣಾಂಕ, ಚಿಹ್ನೆರಹಿತ, normInf_32s>,
            normWrap<ದಶಮಾಂಶ, ದಶಮಾಂಶ, normInf_32f>,
            normWrap<ದ್ವಿಗುಣ, ದ್ವಿಗುಣ, normInf_64f>,
            normWrap<hfloat, ದಶಮಾಂಶ, normInf_16f>,
            normWrap<bfloat, ದಶಮಾಂಶ, normInf_16bf>,
            normWrap<uchar, ಪೂರ್ಣಾಂಕ, normInf_Bool>,
            normWrap<uint64, uint64, normInf_64u>,
            normWrap<int64, uint64, normInf_64s>,
            normWrap<ಚಿಹ್ನೆರಹಿತ, ಚಿಹ್ನೆರಹಿತ, normInf_32u>,
            0
        },
        {
            normWrap<uchar, ಪೂರ್ಣಾಂಕ, normL1_8u>,
            normWrap<schar, ಪೂರ್ಣಾಂಕ, normL1_8s>,
            normWrap<ushort, ಪೂರ್ಣಾಂಕ, normL1_16u>,
            normWrap<ಗಿಡ್ಡ, ಪೂರ್ಣಾಂಕ, normL1_16s>,
            normWrap<ಪೂರ್ಣಾಂಕ, ದ್ವಿಗುಣ, normL1_32s>,
            normWrap<ದಶಮಾಂಶ, ದ್ವಿಗುಣ, normL1_32f>,
            normWrap<ದ್ವಿಗುಣ, ದ್ವಿಗುಣ, normL1_64f>,
            normWrap<hfloat, ದಶಮಾಂಶ, normL1_16f>,
            normWrap<bfloat, ದಶಮಾಂಶ, normL1_16bf>,
            normWrap<uchar, ಪೂರ್ಣಾಂಕ, normL1_Bool>,
            normWrap<uint64, ದ್ವಿಗುಣ, normL1_64u>,
            normWrap<int64, ದ್ವಿಗುಣ, normL1_64s>,
            normWrap<ಚಿಹ್ನೆರಹಿತ, ದ್ವಿಗುಣ, normL1_32u>,
            0
        },
        {
            normWrap<uchar, ಪೂರ್ಣಾಂಕ, normL2_8u>,
            normWrap<schar, ಪೂರ್ಣಾಂಕ, normL2_8s>,
            normWrap<ushort, ದ್ವಿಗುಣ, normL2_16u>,
            normWrap<ಗಿಡ್ಡ, ದ್ವಿಗುಣ, normL2_16s>,
            normWrap<ಪೂರ್ಣಾಂಕ, ದ್ವಿಗುಣ, normL2_32s>,
            normWrap<ದಶಮಾಂಶ, ದ್ವಿಗುಣ, normL2_32f>,
            normWrap<ದ್ವಿಗುಣ, ದ್ವಿಗುಣ, normL2_64f>,
            normWrap<hfloat, ದಶಮಾಂಶ, normL2_16f>,
            normWrap<bfloat, ದಶಮಾಂಶ, normL2_16bf>,
            normWrap<uchar, ಪೂರ್ಣಾಂಕ, normL2_Bool>,
            normWrap<uint64, ದ್ವಿಗುಣ, normL2_64u>,
            normWrap<int64, ದ್ವಿಗುಣ, normL2_64s>,
            normWrap<ಚಿಹ್ನೆರಹಿತ, ದ್ವಿಗುಣ, normL2_32u>,
            0
        }
    };

    ಆದರೆ (normType >= 3 || normType < 0) ಹಿಂತಿರುಗಿ ಶೂನ್ಯಸೂಚಕ;

    ಹಿಂತಿರುಗಿ normTab[normType][ಆಳ];
}

NormDiffFunc getNormDiffFunc(ಪೂರ್ಣಾಂಕ normType, ಪೂರ್ಣಾಂಕ ಆಳ)
{
    ಸ್ಥಿರ NormDiffFunc normDiffTab[3][CV_ಆಳ_MAX] =
    {
        {
            normDiffWrap<uchar, ಪೂರ್ಣಾಂಕ, normDiffInf_8u>,
            normDiffWrap<schar, ಪೂರ್ಣಾಂಕ, normDiffInf_8s>,
            normDiffWrap<ushort, ಪೂರ್ಣಾಂಕ, normDiffInf_16u>,
            normDiffWrap<ಗಿಡ್ಡ, ಪೂರ್ಣಾಂಕ, normDiffInf_16s>,
            normDiffWrap<ಪೂರ್ಣಾಂಕ, ಚಿಹ್ನೆರಹಿತ, normDiffInf_32s>,
            normDiffWrap<ದಶಮಾಂಶ, ದಶಮಾಂಶ, normDiffInf_32f>,
            normDiffWrap<ದ್ವಿಗುಣ, ದ್ವಿಗುಣ, normDiffInf_64f>,
            normDiffWrap<hfloat, ದಶಮಾಂಶ, normDiffInf_16f>,
            normDiffWrap<bfloat, ದಶಮಾಂಶ, normDiffInf_16bf>,
            normDiffWrap<uchar, ಪೂರ್ಣಾಂಕ, normDiffInf_Bool>,
            normDiffWrap<uint64, uint64, normDiffInf_64u>,
            normDiffWrap<int64, uint64, normDiffInf_64s>,
            normDiffWrap<ಚಿಹ್ನೆರಹಿತ, ಚಿಹ್ನೆರಹಿತ, normDiffInf_32u>,
            0
        },
        {
            normDiffWrap<uchar, ಪೂರ್ಣಾಂಕ, normDiffL1_8u>,
            normDiffWrap<schar, ಪೂರ್ಣಾಂಕ, normDiffL1_8s>,
            normDiffWrap<ushort, ಪೂರ್ಣಾಂಕ, normDiffL1_16u>,
            normDiffWrap<ಗಿಡ್ಡ, ಪೂರ್ಣಾಂಕ, normDiffL1_16s>,
            normDiffWrap<ಪೂರ್ಣಾಂಕ, ದ್ವಿಗುಣ, normDiffL1_32s>,
            normDiffWrap<ದಶಮಾಂಶ, ದ್ವಿಗುಣ, normDiffL1_32f>,
            normDiffWrap<ದ್ವಿಗುಣ, ದ್ವಿಗುಣ, normDiffL1_64f>,
            normDiffWrap<hfloat, ದಶಮಾಂಶ, normDiffL1_16f>,
            normDiffWrap<bfloat, ದಶಮಾಂಶ, normDiffL1_16bf>,
            normDiffWrap<uchar, ಪೂರ್ಣಾಂಕ, normDiffL1_Bool>,
            normDiffWrap<uint64, ದ್ವಿಗುಣ, normDiffL1_64u>,
            normDiffWrap<int64, ದ್ವಿಗುಣ, normDiffL1_64s>,
            normDiffWrap<ಚಿಹ್ನೆರಹಿತ, ದ್ವಿಗುಣ, normDiffL1_32u>,
            0
        },
        {
            normDiffWrap<uchar, ಪೂರ್ಣಾಂಕ, normDiffL2_8u>,
            normDiffWrap<schar, ಪೂರ್ಣಾಂಕ, normDiffL2_8s>,
            normDiffWrap<ushort, ದ್ವಿಗುಣ, normDiffL2_16u>,
            normDiffWrap<ಗಿಡ್ಡ, ದ್ವಿಗುಣ, normDiffL2_16s>,
            normDiffWrap<ಪೂರ್ಣಾಂಕ, ದ್ವಿಗುಣ, normDiffL2_32s>,
            normDiffWrap<ದಶಮಾಂಶ, ದ್ವಿಗುಣ, normDiffL2_32f>,
            normDiffWrap<ದ್ವಿಗುಣ, ದ್ವಿಗುಣ, normDiffL2_64f>,
            normDiffWrap<hfloat, ದಶಮಾಂಶ, normDiffL2_16f>,
            normDiffWrap<bfloat, ದಶಮಾಂಶ, normDiffL2_16bf>,
            normDiffWrap<uchar, ಪೂರ್ಣಾಂಕ, normDiffL2_Bool>,
            normDiffWrap<uint64, ದ್ವಿಗುಣ, normDiffL2_64u>,
            normDiffWrap<int64, ದ್ವಿಗುಣ, normDiffL2_64s>,
            normDiffWrap<ಚಿಹ್ನೆರಹಿತ, ದ್ವಿಗುಣ, normDiffL2_32u>,
            0
        },
    };
    ಆದರೆ (normType >= 3 || normType < 0) ಹಿಂತಿರುಗಿ ಶೂನ್ಯಸೂಚಕ;

    ಹಿಂತಿರುಗಿ normDiffTab[normType][ಆಳ];
}

#ಆದರೆ_ಅಂತ್ಯ // CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

CV_CPU_OPTIMIZATION_NAMESPACE_ಕೊನೆ

} // cv::
