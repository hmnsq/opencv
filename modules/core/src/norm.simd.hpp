// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html
//
// Copyright (C) 2025, SpaceMIT Inc., all rights reserved.
// Copyright (C) 2026, Advanced Micro Devices, Inc., all rights reserved.
// Third party copyrights are property of their respective owners.

#അന്തര്ഭാവ "precomp.hpp"

നാമാകാശ cv {

using NormFunc = പൂർണ്ണസംഖ്യ (*)(സ്ഥിരം uchar*, സ്ഥിരം uchar*, uchar*, പൂർണ്ണസംഖ്യ, പൂർണ്ണസംഖ്യ);
using NormDiffFunc = പൂർണ്ണസംഖ്യ (*)(സ്ഥിരം uchar*, സ്ഥിരം uchar*, സ്ഥിരം uchar*, uchar*, പൂർണ്ണസംഖ്യ, പൂർണ്ണസംഖ്യ);

CV_CPU_OPTIMIZATION_NAMESPACE_BEGIN

template<typename T, typename RT, പൂർണ്ണസംഖ്യ (*fn)(സ്ഥിരം T*, സ്ഥിരം uchar*, RT*, പൂർണ്ണസംഖ്യ, പൂർണ്ണസംഖ്യ)>
സ്ഥിതം പൂർണ്ണസംഖ്യ normWrap(സ്ഥിരം uchar* src1, സ്ഥിരം uchar* src2, uchar* res, പൂർണ്ണസംഖ്യ len, പൂർണ്ണസംഖ്യ cn)
{
    തിരികെ fn((സ്ഥിരം T*)src1, src2, (RT*)res, len, cn);
}

template<typename T, typename RT, പൂർണ്ണസംഖ്യ (*fn)(സ്ഥിരം T*, സ്ഥിരം T*, സ്ഥിരം uchar*, RT*, പൂർണ്ണസംഖ്യ, പൂർണ്ണസംഖ്യ)>
സ്ഥിതം പൂർണ്ണസംഖ്യ normDiffWrap(സ്ഥിരം uchar* src1, സ്ഥിരം uchar* src2, സ്ഥിരം uchar* src3, uchar* res, പൂർണ്ണസംഖ്യ len, പൂർണ്ണസംഖ്യ cn)
{
    തിരികെ fn((സ്ഥിരം T*)src1, (സ്ഥിരം T*)src2, src3, (RT*)res, len, cn);
}

NormFunc getNormFunc(പൂർണ്ണസംഖ്യ normType, പൂർണ്ണസംഖ്യ depth);
NormDiffFunc getNormDiffFunc(പൂർണ്ണസംഖ്യ normType, പൂർണ്ണസംഖ്യ depth);

#ifndef CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

template <typename T, typename ST>
ഘടന NormInf_SIMD {
    വരിയിൽ ST operator() (സ്ഥിരം T* src, പൂർണ്ണസംഖ്യ n) സ്ഥിരം {
        ST s = 0;
        വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < n; i++) {
            s = std::max(s, (ST)cv_abs(src[i]));
        }
        തിരികെ s;
    }
};

template <typename T, typename ST>
ഘടന NormL1_SIMD {
    വരിയിൽ ST operator() (സ്ഥിരം T* src, പൂർണ്ണസംഖ്യ n) സ്ഥിരം {
        ST s = 0;
        വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < n; i++) {
            s += cv_abs(src[i]);
        }
        തിരികെ s;
    }
};

template <typename T, typename ST>
ഘടന NormL2_SIMD {
    വരിയിൽ ST operator() (സ്ഥിരം T* src, പൂർണ്ണസംഖ്യ n) സ്ഥിരം {
        ST s = 0;
        വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < n; i++) {
            ST v = (ST)src[i];
            s += v * v;
        }
        തിരികെ s;
    }
};

template <typename T, typename ST>
ഘടന NormDiffInf_SIMD {
    വരിയിൽ ST operator() (സ്ഥിരം T* src1, സ്ഥിരം T* src2, പൂർണ്ണസംഖ്യ n) സ്ഥിരം {
        ST s = 0;
        വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < n; i++) {
            ST v = (ST)cv_absdiff(src1[i], src2[i]);
            s = std::max(s, v);
        }
        തിരികെ s;
    }
};

template <typename T, typename ST>
ഘടന NormDiffL1_SIMD {
    വരിയിൽ ST operator() (സ്ഥിരം T* src1, സ്ഥിരം T* src2, പൂർണ്ണസംഖ്യ n) സ്ഥിരം {
        ST s = 0;
        വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < n; i++) {
            ST v = (ST)cv_absdiff(src1[i], src2[i]);
            s += v;
        }
        തിരികെ s;
    }
};

template <typename T, typename ST>
ഘടന NormDiffL2_SIMD {
    വരിയിൽ ST operator() (സ്ഥിരം T* src1, സ്ഥിരം T* src2, പൂർണ്ണസംഖ്യ n) സ്ഥിരം {
        ST s = 0;
        വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < n; i++) {
            ST v = (ST)src1[i] - (ST)src2[i];
            s += v * v;
        }
        തിരികെ s;
    }
};

#എങ്കിൽ (CV_SIMD || CV_SIMD_SCALABLE)

template<>
ഘടന NormInf_SIMD<uchar, പൂർണ്ണസംഖ്യ> {
    പൂർണ്ണസംഖ്യ operator() (സ്ഥിരം uchar* src, പൂർണ്ണസംഖ്യ n) സ്ഥിരം {
        പൂർണ്ണസംഖ്യ j = 0;
        പൂർണ്ണസംഖ്യ s = 0;
        v_uint8 r0 = vx_setzero_u8(), r1 = vx_setzero_u8();
        v_uint8 r2 = vx_setzero_u8(), r3 = vx_setzero_u8();
        വേണ്ടി (; j <= n - 4 * VTraits<v_uint8>::vlanes(); j += 4 * VTraits<v_uint8>::vlanes()) {
            r0 = v_max(r0, vx_load(src + j                                 ));
            r1 = v_max(r1, vx_load(src + j +     VTraits<v_uint8>::vlanes()));
            r2 = v_max(r2, vx_load(src + j + 2 * VTraits<v_uint8>::vlanes()));
            r3 = v_max(r3, vx_load(src + j + 3 * VTraits<v_uint8>::vlanes()));
        }
        r0 = v_max(r0, v_max(r1, v_max(r2, r3)));
        വേണ്ടി (; j < n; j++) {
            s = std::max(s, (പൂർണ്ണസംഖ്യ)src[j]);
        }
        തിരികെ std::max(s, (പൂർണ്ണസംഖ്യ)v_reduce_max(r0));
    }
};

template<>
ഘടന NormInf_SIMD<schar, പൂർണ്ണസംഖ്യ> {
    പൂർണ്ണസംഖ്യ operator() (സ്ഥിരം schar* src, പൂർണ്ണസംഖ്യ n) സ്ഥിരം {
        പൂർണ്ണസംഖ്യ j = 0;
        പൂർണ്ണസംഖ്യ s = 0;
        v_uint8 r0 = vx_setzero_u8(), r1 = vx_setzero_u8();
        v_uint8 r2 = vx_setzero_u8(), r3 = vx_setzero_u8();
        വേണ്ടി (; j <= n - 4 * VTraits<v_int8>::vlanes(); j += 4 * VTraits<v_int8>::vlanes()) {
            r0 = v_max(r0, v_abs(vx_load(src + j                                )));
            r1 = v_max(r1, v_abs(vx_load(src + j +     VTraits<v_int8>::vlanes())));
            r2 = v_max(r2, v_abs(vx_load(src + j + 2 * VTraits<v_int8>::vlanes())));
            r3 = v_max(r3, v_abs(vx_load(src + j + 3 * VTraits<v_int8>::vlanes())));
        }
        r0 = v_max(r0, v_max(r1, v_max(r2, r3)));
        വേണ്ടി (; j < n; j++) {
            s = std::max(s, cv_abs(src[j]));
        }
        തിരികെ std::max(s, saturate_cast<പൂർണ്ണസംഖ്യ>(v_reduce_max(r0)));
    }
};

template<>
ഘടന NormInf_SIMD<ushort, പൂർണ്ണസംഖ്യ> {
    പൂർണ്ണസംഖ്യ operator() (സ്ഥിരം ushort* src, പൂർണ്ണസംഖ്യ n) സ്ഥിരം {
        പൂർണ്ണസംഖ്യ j = 0;
        പൂർണ്ണസംഖ്യ s = 0;
        v_uint16 d0 = vx_setzero_u16(), d1 = vx_setzero_u16();
        v_uint16 d2 = vx_setzero_u16(), d3 = vx_setzero_u16();
        വേണ്ടി (; j <= n - 4 * VTraits<v_uint16>::vlanes(); j += 4 * VTraits<v_uint16>::vlanes()) {
            d0 = v_max(d0, vx_load(src + j                                  ));
            d1 = v_max(d1, vx_load(src + j +     VTraits<v_uint16>::vlanes()));
            d2 = v_max(d2, vx_load(src + j + 2 * VTraits<v_uint16>::vlanes()));
            d3 = v_max(d3, vx_load(src + j + 3 * VTraits<v_uint16>::vlanes()));
        }
        d0 = v_max(d0, v_max(d1, v_max(d2, d3)));
        വേണ്ടി (; j < n; j++) {
            s = std::max(s, (പൂർണ്ണസംഖ്യ)src[j]);
        }
        തിരികെ std::max(s, (പൂർണ്ണസംഖ്യ)v_reduce_max(d0));
    }
};

template<>
ഘടന NormInf_SIMD<short, പൂർണ്ണസംഖ്യ> {
    പൂർണ്ണസംഖ്യ operator() (സ്ഥിരം short* src, പൂർണ്ണസംഖ്യ n) സ്ഥിരം {
        പൂർണ്ണസംഖ്യ j = 0;
        പൂർണ്ണസംഖ്യ s = 0;
        v_uint16 d0 = vx_setzero_u16(), d1 = vx_setzero_u16();
        v_uint16 d2 = vx_setzero_u16(), d3 = vx_setzero_u16();
        വേണ്ടി (; j <= n - 4 * VTraits<v_int16>::vlanes(); j += 4 * VTraits<v_int16>::vlanes()) {
            d0 = v_max(d0, v_abs(vx_load(src + j                                  )));
            d1 = v_max(d1, v_abs(vx_load(src + j +     VTraits<v_int16>::vlanes())));
            d2 = v_max(d2, v_abs(vx_load(src + j + 2 * VTraits<v_int16>::vlanes())));
            d3 = v_max(d3, v_abs(vx_load(src + j + 3 * VTraits<v_int16>::vlanes())));
        }
        d0 = v_max(d0, v_max(d1, v_max(d2, d3)));
        വേണ്ടി (; j < n; j++) {
            s = std::max(s, saturate_cast<പൂർണ്ണസംഖ്യ>(cv_abs(src[j])));
        }
        തിരികെ std::max(s, saturate_cast<പൂർണ്ണസംഖ്യ>(v_reduce_max(d0)));
    }
};

template<>
ഘടന NormInf_SIMD<പൂർണ്ണസംഖ്യ, പൂർണ്ണസംഖ്യ> {
    പൂർണ്ണസംഖ്യ operator() (സ്ഥിരം പൂർണ്ണസംഖ്യ* src, പൂർണ്ണസംഖ്യ n) സ്ഥിരം {
        പൂർണ്ണസംഖ്യ j = 0;
        പൂർണ്ണസംഖ്യ s = 0;
        v_uint32 r0 = vx_setzero_u32(), r1 = vx_setzero_u32();
        v_uint32 r2 = vx_setzero_u32(), r3 = vx_setzero_u32();
        വേണ്ടി (; j <= n - 4 * VTraits<v_int32>::vlanes(); j += 4 * VTraits<v_int32>::vlanes()) {
            r0 = v_max(r0, v_abs(vx_load(src + j                                 )));
            r1 = v_max(r1, v_abs(vx_load(src + j +     VTraits<v_int32>::vlanes())));
            r2 = v_max(r2, v_abs(vx_load(src + j + 2 * VTraits<v_int32>::vlanes())));
            r3 = v_max(r3, v_abs(vx_load(src + j + 3 * VTraits<v_int32>::vlanes())));
        }
        r0 = v_max(r0, v_max(r1, v_max(r2, r3)));
        s = std::max(s, saturate_cast<പൂർണ്ണസംഖ്യ>(v_reduce_max(r0)));
        വേണ്ടി (; j < n; j++) {
            s = std::max(s, std::abs(src[j]));
        }
        തിരികെ s;
    }
};

template<>
ഘടന NormInf_SIMD<ദശാംശം, ദശാംശം> {
    ദശാംശം operator() (സ്ഥിരം ദശാംശം* src, പൂർണ്ണസംഖ്യ n) സ്ഥിരം {
        പൂർണ്ണസംഖ്യ j = 0;
        ദശാംശം s = 0.f;
        v_float32 r0 = vx_setzero_f32(), r1 = vx_setzero_f32();
        v_float32 r2 = vx_setzero_f32(), r3 = vx_setzero_f32();
        വേണ്ടി (; j <= n - 4 * VTraits<v_float32>::vlanes(); j += 4 * VTraits<v_float32>::vlanes()) {
            r0 = v_max(r0, v_abs(vx_load(src + j                                   )));
            r1 = v_max(r1, v_abs(vx_load(src + j +     VTraits<v_float32>::vlanes())));
            r2 = v_max(r2, v_abs(vx_load(src + j + 2 * VTraits<v_float32>::vlanes())));
            r3 = v_max(r3, v_abs(vx_load(src + j + 3 * VTraits<v_float32>::vlanes())));
        }
        r0 = v_max(r0, v_max(r1, v_max(r2, r3)));
        വേണ്ടി (; j < n; j++) {
            s = std::max(s, cv_abs(src[j]));
        }
        തിരികെ std::max(s, v_reduce_max(r0));
    }
};

template<>
ഘടന NormL1_SIMD<uchar, പൂർണ്ണസംഖ്യ> {
    പൂർണ്ണസംഖ്യ operator() (സ്ഥിരം uchar* src, പൂർണ്ണസംഖ്യ n) സ്ഥിരം {
        പൂർണ്ണസംഖ്യ j = 0;
        പൂർണ്ണസംഖ്യ s = 0;
        v_uint32 r0 = vx_setzero_u32(), r1 = vx_setzero_u32();
        v_uint8  one = vx_setall_u8(1);
        വേണ്ടി (; j<= n - 2 * VTraits<v_uint8>::vlanes(); j += 2 * VTraits<v_uint8>::vlanes()) {
            v_uint8 v0 = vx_load(src + j);
            r0 = v_dotprod_expand_fast(v0, one, r0);

            v_uint8 v1 = vx_load(src + j + VTraits<v_uint8>::vlanes());
            r1 = v_dotprod_expand_fast(v1, one, r1);
        }
        s += v_reduce_sum(v_add(r0, r1));
        വേണ്ടി (; j < n; j++) {
            s += src[j];
        }
        തിരികെ s;
    }
};

template<>
ഘടന NormL1_SIMD<schar, പൂർണ്ണസംഖ്യ> {
    പൂർണ്ണസംഖ്യ operator() (സ്ഥിരം schar* src, പൂർണ്ണസംഖ്യ n) സ്ഥിരം {
        പൂർണ്ണസംഖ്യ j = 0;
        പൂർണ്ണസംഖ്യ s = 0;
        v_uint32 r0 = vx_setzero_u32(), r1 = vx_setzero_u32();
        v_uint8  one = vx_setall_u8(1);
        വേണ്ടി (; j<= n - 2 * VTraits<v_int8>::vlanes(); j += 2 * VTraits<v_int8>::vlanes()) {
            v_uint8 v0 = v_abs(vx_load(src + j));
            r0 = v_dotprod_expand_fast(v0, one, r0);

            v_uint8 v1 = v_abs(vx_load(src + j + VTraits<v_int8>::vlanes()));
            r1 = v_dotprod_expand_fast(v1, one, r1);
        }
        s += v_reduce_sum(v_add(r0, r1));
        വേണ്ടി (; j < n; j++) {
            s += saturate_cast<പൂർണ്ണസംഖ്യ>(cv_abs(src[j]));
        }
        തിരികെ s;
    }
};

template<>
ഘടന NormL1_SIMD<ushort, പൂർണ്ണസംഖ്യ> {
    പൂർണ്ണസംഖ്യ operator() (സ്ഥിരം ushort* src, പൂർണ്ണസംഖ്യ n) സ്ഥിരം {
        പൂർണ്ണസംഖ്യ j = 0;
        പൂർണ്ണസംഖ്യ s = 0;
        v_uint32 r00 = vx_setzero_u32(), r01 = vx_setzero_u32();
        v_uint32 r10 = vx_setzero_u32(), r11 = vx_setzero_u32();
        വേണ്ടി (; j<= n - 2 * VTraits<v_uint16>::vlanes(); j += 2 * VTraits<v_uint16>::vlanes()) {
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
        s += (പൂർണ്ണസംഖ്യ)v_reduce_sum(v_add(v_add(v_add(r00, r01), r10), r11));
        വേണ്ടി (; j < n; j++) {
            s += src[j];
        }
        തിരികെ s;
    }
};

template<>
ഘടന NormL1_SIMD<short, പൂർണ്ണസംഖ്യ> {
    പൂർണ്ണസംഖ്യ operator() (സ്ഥിരം short* src, പൂർണ്ണസംഖ്യ n) സ്ഥിരം {
        പൂർണ്ണസംഖ്യ j = 0;
        പൂർണ്ണസംഖ്യ s = 0;
        v_uint32 r00 = vx_setzero_u32(), r01 = vx_setzero_u32();
        v_uint32 r10 = vx_setzero_u32(), r11 = vx_setzero_u32();
        വേണ്ടി (; j<= n - 2 * VTraits<v_int16>::vlanes(); j += 2 * VTraits<v_int16>::vlanes()) {
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
        s += (പൂർണ്ണസംഖ്യ)v_reduce_sum(v_add(v_add(v_add(r00, r01), r10), r11));
        വേണ്ടി (; j < n; j++) {
            s += saturate_cast<പൂർണ്ണസംഖ്യ>(cv_abs(src[j]));
        }
        തിരികെ s;
    }
};

template<>
ഘടന NormL2_SIMD<uchar, പൂർണ്ണസംഖ്യ> {
    പൂർണ്ണസംഖ്യ operator() (സ്ഥിരം uchar* src, പൂർണ്ണസംഖ്യ n) സ്ഥിരം {
        പൂർണ്ണസംഖ്യ j = 0;
        പൂർണ്ണസംഖ്യ s = 0;
        v_uint32 r0 = vx_setzero_u32(), r1 = vx_setzero_u32();
        വേണ്ടി (; j <= n - 2 * VTraits<v_uint8>::vlanes(); j += 2 * VTraits<v_uint8>::vlanes()) {
            v_uint8 v0 = vx_load(src + j);
            r0 = v_dotprod_expand_fast(v0, v0, r0);

            v_uint8 v1 = vx_load(src + j + VTraits<v_uint8>::vlanes());
            r1 = v_dotprod_expand_fast(v1, v1, r1);
        }
        s += v_reduce_sum(v_add(r0, r1));
        വേണ്ടി (; j < n; j++) {
            പൂർണ്ണസംഖ്യ v = saturate_cast<പൂർണ്ണസംഖ്യ>(src[j]);
            s += v * v;
        }
        തിരികെ s;
    }
};

template<>
ഘടന NormL2_SIMD<schar, പൂർണ്ണസംഖ്യ> {
    പൂർണ്ണസംഖ്യ operator() (സ്ഥിരം schar* src, പൂർണ്ണസംഖ്യ n) സ്ഥിരം {
        പൂർണ്ണസംഖ്യ j = 0;
        പൂർണ്ണസംഖ്യ s = 0;
        v_int32 r0 = vx_setzero_s32(), r1 = vx_setzero_s32();
        വേണ്ടി (; j <= n - 2 * VTraits<v_int8>::vlanes(); j += 2 * VTraits<v_int8>::vlanes()) {
            v_int8 v0 = vx_load(src + j);
            r0 = v_dotprod_expand_fast(v0, v0, r0);
            v_int8 v1 = vx_load(src + j + VTraits<v_int8>::vlanes());
            r1 = v_dotprod_expand_fast(v1, v1, r1);
        }
        s += v_reduce_sum(v_add(r0, r1));
        വേണ്ടി (; j < n; j++) {
            പൂർണ്ണസംഖ്യ v = saturate_cast<പൂർണ്ണസംഖ്യ>(src[j]);
            s += v * v;
        }
        തിരികെ s;
    }
};

template<>
ഘടന NormDiffInf_SIMD<uchar, പൂർണ്ണസംഖ്യ> {
    പൂർണ്ണസംഖ്യ operator() (സ്ഥിരം uchar* src1, സ്ഥിരം uchar* src2, പൂർണ്ണസംഖ്യ n) സ്ഥിരം {
        പൂർണ്ണസംഖ്യ j = 0;
        പൂർണ്ണസംഖ്യ s = 0;
        v_uint8 r0 = vx_setzero_u8(), r1 = vx_setzero_u8();
        v_uint8 r2 = vx_setzero_u8(), r3 = vx_setzero_u8();
        വേണ്ടി (; j <= n - 4 * VTraits<v_uint8>::vlanes(); j += 4 * VTraits<v_uint8>::vlanes()) {
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
        s = (പൂർണ്ണസംഖ്യ)v_reduce_max(v_max(v_max(v_max(r0, r1), r2), r3));
        വേണ്ടി (; j < n; j++) {
            പൂർണ്ണസംഖ്യ v = (പൂർണ്ണസംഖ്യ)cv_absdiff(src1[j], src2[j]);
            s = std::max(s, v);
        }
        തിരികെ s;
    }
};

template<>
ഘടന NormDiffInf_SIMD<schar, പൂർണ്ണസംഖ്യ> {
    പൂർണ്ണസംഖ്യ operator() (സ്ഥിരം schar* src1, സ്ഥിരം schar* src2, പൂർണ്ണസംഖ്യ n) സ്ഥിരം {
        പൂർണ്ണസംഖ്യ j = 0;
        പൂർണ്ണസംഖ്യ s = 0;
        v_uint8 r0 = vx_setzero_u8(), r1 = vx_setzero_u8();
        v_uint8 r2 = vx_setzero_u8(), r3 = vx_setzero_u8();
        വേണ്ടി (; j <= n - 4 * VTraits<v_int8>::vlanes(); j += 4 * VTraits<v_int8>::vlanes()) {
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
        s = (പൂർണ്ണസംഖ്യ)v_reduce_max(v_max(v_max(v_max(r0, r1), r2), r3));
        വേണ്ടി (; j < n; j++) {
            പൂർണ്ണസംഖ്യ v = (പൂർണ്ണസംഖ്യ)cv_absdiff(src1[j], src2[j]);
            s = std::max(s, v);
        }
        തിരികെ s;
    }
};

template<>
ഘടന NormDiffInf_SIMD<ushort, പൂർണ്ണസംഖ്യ> {
    പൂർണ്ണസംഖ്യ operator() (സ്ഥിരം ushort* src1, സ്ഥിരം ushort* src2, പൂർണ്ണസംഖ്യ n) സ്ഥിരം {
        പൂർണ്ണസംഖ്യ j = 0;
        പൂർണ്ണസംഖ്യ s = 0;
        v_uint16 r0 = vx_setzero_u16(), r1 = vx_setzero_u16();
        v_uint16 r2 = vx_setzero_u16(), r3 = vx_setzero_u16();
        വേണ്ടി (; j <= n - 4 * VTraits<v_uint16>::vlanes(); j += 4 * VTraits<v_uint16>::vlanes()) {
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
        s = (പൂർണ്ണസംഖ്യ)v_reduce_max(v_max(v_max(v_max(r0, r1), r2), r3));
        വേണ്ടി (; j < n; j++) {
            പൂർണ്ണസംഖ്യ v = (പൂർണ്ണസംഖ്യ)cv_absdiff(src1[j], src2[j]);
            s = std::max(s, v);
        }
        തിരികെ s;
    }
};

template<>
ഘടന NormDiffInf_SIMD<short, പൂർണ്ണസംഖ്യ> {
    പൂർണ്ണസംഖ്യ operator() (സ്ഥിരം short* src1, സ്ഥിരം short* src2, പൂർണ്ണസംഖ്യ n) സ്ഥിരം {
        പൂർണ്ണസംഖ്യ j = 0;
        പൂർണ്ണസംഖ്യ s = 0;
        v_uint16 r0 = vx_setzero_u16(), r1 = vx_setzero_u16();
        v_uint16 r2 = vx_setzero_u16(), r3 = vx_setzero_u16();
        വേണ്ടി (; j <= n - 4 * VTraits<v_int16>::vlanes(); j += 4 * VTraits<v_int16>::vlanes()) {
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
        s = (പൂർണ്ണസംഖ്യ)v_reduce_max(v_max(v_max(v_max(r0, r1), r2), r3));
        വേണ്ടി (; j < n; j++) {
            പൂർണ്ണസംഖ്യ v = (പൂർണ്ണസംഖ്യ)cv_absdiff(src1[j], src2[j]);
            s = std::max(s, v);
        }
        തിരികെ s;
    }
};

template<>
ഘടന NormDiffInf_SIMD<പൂർണ്ണസംഖ്യ, unsigned> {
    unsigned operator() (സ്ഥിരം പൂർണ്ണസംഖ്യ* src1, സ്ഥിരം പൂർണ്ണസംഖ്യ* src2, പൂർണ്ണസംഖ്യ n) സ്ഥിരം {
        പൂർണ്ണസംഖ്യ j = 0;
        unsigned s = 0;
        v_uint32 r0 = vx_setzero_u32(), r1 = vx_setzero_u32();
        v_uint32 r2 = vx_setzero_u32(), r3 = vx_setzero_u32();
        വേണ്ടി (; j <= n - 4 * VTraits<v_int32>::vlanes(); j += 4 * VTraits<v_int32>::vlanes()) {
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
        s = (പൂർണ്ണസംഖ്യ)v_reduce_max(v_max(v_max(v_max(r0, r1), r2), r3));
        വേണ്ടി (; j < n; j++) {
            unsigned v = (unsigned)cv_absdiff(src1[j], src2[j]);
            s = std::max(s, v);
        }
        തിരികെ s;
    }
};

template<>
ഘടന NormDiffInf_SIMD<ദശാംശം, ദശാംശം> {
    ദശാംശം operator() (സ്ഥിരം ദശാംശം* src1, സ്ഥിരം ദശാംശം* src2, പൂർണ്ണസംഖ്യ n) സ്ഥിരം {
        പൂർണ്ണസംഖ്യ j = 0;
        ദശാംശം s = 0;
        v_float32 r0 = vx_setzero_f32(), r1 = vx_setzero_f32();
        വേണ്ടി (; j <= n - 2 * VTraits<v_float32>::vlanes(); j += 2 * VTraits<v_float32>::vlanes()) {
            v_float32 v01 = vx_load(src1 + j), v02 = vx_load(src2 + j);
            r0 = v_max(r0, v_absdiff(v01, v02));

            v_float32 v11 = vx_load(src1 + j + VTraits<v_float32>::vlanes()),
                      v12 = vx_load(src2 + j + VTraits<v_float32>::vlanes());
            r1 = v_max(r1, v_absdiff(v11, v12));
        }
        s = v_reduce_max(v_max(r0, r1));
        വേണ്ടി (; j < n; j++) {
            ദശാംശം v = (ദശാംശം)cv_absdiff(src1[j], src2[j]);
            s = std::max(s, v);
        }
        തിരികെ s;
    }
};

template<>
ഘടന NormDiffL1_SIMD<uchar, പൂർണ്ണസംഖ്യ> {
    പൂർണ്ണസംഖ്യ operator() (സ്ഥിരം uchar* src1, സ്ഥിരം uchar* src2, പൂർണ്ണസംഖ്യ n) സ്ഥിരം {
        പൂർണ്ണസംഖ്യ j = 0;
        പൂർണ്ണസംഖ്യ s = 0;
        v_uint32 r0 = vx_setzero_u32(), r1 = vx_setzero_u32();
        v_uint8  one = vx_setall_u8(1);
        വേണ്ടി (; j<= n - 2 * VTraits<v_uint8>::vlanes(); j += 2 * VTraits<v_uint8>::vlanes()) {
            v_uint8 v01 = vx_load(src1 + j), v02 = vx_load(src2 + j);
            r0 = v_dotprod_expand_fast(v_absdiff(v01, v02), one, r0);

            v_uint8 v11 = vx_load(src1 + j + VTraits<v_uint8>::vlanes()),
                    v12 = vx_load(src2 + j + VTraits<v_uint8>::vlanes());
            r1 = v_dotprod_expand_fast(v_absdiff(v11, v12), one, r1);
        }
        s += v_reduce_sum(v_add(r0, r1));
        വേണ്ടി (; j < n; j++) {
            പൂർണ്ണസംഖ്യ v = (പൂർണ്ണസംഖ്യ)cv_absdiff(src1[j], src2[j]);
            s += v;
        }
        തിരികെ s;
    }
};

template<>
ഘടന NormDiffL1_SIMD<schar, പൂർണ്ണസംഖ്യ> {
    പൂർണ്ണസംഖ്യ operator() (സ്ഥിരം schar* src1, സ്ഥിരം schar* src2, പൂർണ്ണസംഖ്യ n) സ്ഥിരം {
        പൂർണ്ണസംഖ്യ j = 0;
        പൂർണ്ണസംഖ്യ s = 0;
        v_uint32 r0 = vx_setzero_u32(), r1 = vx_setzero_u32();
        v_uint8  one = vx_setall_u8(1);
        വേണ്ടി (; j<= n - 2 * VTraits<v_int8>::vlanes(); j += 2 * VTraits<v_int8>::vlanes()) {
            v_int8 v01 = vx_load(src1 + j), v02 = vx_load(src2 + j);
            r0 = v_dotprod_expand_fast(v_absdiff(v01, v02), one, r0);

            v_int8 v11 = vx_load(src1 + j + VTraits<v_int8>::vlanes()),
                   v12 = vx_load(src2 + j + VTraits<v_int8>::vlanes());
            r1 = v_dotprod_expand_fast(v_absdiff(v11, v12), one, r1);
        }
        s += v_reduce_sum(v_add(r0, r1));
        വേണ്ടി (; j < n; j++) {
            പൂർണ്ണസംഖ്യ v = (പൂർണ്ണസംഖ്യ)cv_absdiff(src1[j], src2[j]);
            s += v;
        }
        തിരികെ s;
    }
};

template<>
ഘടന NormDiffL1_SIMD<ushort, പൂർണ്ണസംഖ്യ> {
    പൂർണ്ണസംഖ്യ operator() (സ്ഥിരം ushort* src1, സ്ഥിരം ushort* src2, പൂർണ്ണസംഖ്യ n) സ്ഥിരം {
        പൂർണ്ണസംഖ്യ j = 0;
        പൂർണ്ണസംഖ്യ s = 0;
        v_uint32 r0 = vx_setzero_u32(), r1 = vx_setzero_u32();
        v_uint32 r2 = vx_setzero_u32(), r3 = vx_setzero_u32();
        വേണ്ടി (; j<= n - 4 * VTraits<v_uint16>::vlanes(); j += 4 * VTraits<v_uint16>::vlanes()) {
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
        s += (പൂർണ്ണസംഖ്യ)v_reduce_sum(v_add(v_add(v_add(r0, r1), r2), r3));
        വേണ്ടി (; j < n; j++) {
            പൂർണ്ണസംഖ്യ v = (പൂർണ്ണസംഖ്യ)cv_absdiff(src1[j], src2[j]);
            s += v;
        }
        തിരികെ s;
    }
};

template<>
ഘടന NormDiffL1_SIMD<short, പൂർണ്ണസംഖ്യ> {
    പൂർണ്ണസംഖ്യ operator() (സ്ഥിരം short* src1, സ്ഥിരം short* src2, പൂർണ്ണസംഖ്യ n) സ്ഥിരം {
        പൂർണ്ണസംഖ്യ j = 0;
        പൂർണ്ണസംഖ്യ s = 0;
        v_uint32 r0 = vx_setzero_u32(), r1 = vx_setzero_u32();
        v_uint32 r2 = vx_setzero_u32(), r3 = vx_setzero_u32();
        വേണ്ടി (; j<= n - 4 * VTraits<v_int16>::vlanes(); j += 4 * VTraits<v_int16>::vlanes()) {
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
        s += (പൂർണ്ണസംഖ്യ)v_reduce_sum(v_add(v_add(v_add(r0, r1), r2), r3));
        വേണ്ടി (; j < n; j++) {
            പൂർണ്ണസംഖ്യ v = (പൂർണ്ണസംഖ്യ)cv_absdiff(src1[j], src2[j]);
            s += v;
        }
        തിരികെ s;
    }
};

template<>
ഘടന NormDiffL2_SIMD<uchar, പൂർണ്ണസംഖ്യ> {
    പൂർണ്ണസംഖ്യ operator() (സ്ഥിരം uchar* src1, സ്ഥിരം uchar* src2, പൂർണ്ണസംഖ്യ n) സ്ഥിരം {
        പൂർണ്ണസംഖ്യ j = 0;
        പൂർണ്ണസംഖ്യ s = 0;
        v_uint32 r0 = vx_setzero_u32(), r1 = vx_setzero_u32();
        വേണ്ടി (; j <= n - 2 * VTraits<v_uint8>::vlanes(); j += 2 * VTraits<v_uint8>::vlanes()) {
            v_uint8 v01 = vx_load(src1 + j), v02 = vx_load(src2 + j);
            v_uint8 v0 = v_absdiff(v01, v02);
            r0 = v_dotprod_expand_fast(v0, v0, r0);

            v_uint8 v11 = vx_load(src1 + j + VTraits<v_uint8>::vlanes()),
                    v12 = vx_load(src2 + j + VTraits<v_uint8>::vlanes());
            v_uint8 v1 = v_absdiff(v11, v12);
            r1 = v_dotprod_expand_fast(v1, v1, r1);
        }
        s += v_reduce_sum(v_add(r0, r1));
        വേണ്ടി (; j < n; j++) {
            പൂർണ്ണസംഖ്യ v = (പൂർണ്ണസംഖ്യ)src1[j] - (പൂർണ്ണസംഖ്യ)src2[j];
            s += v * v;
        }
        തിരികെ s;
    }
};

template<>
ഘടന NormDiffL2_SIMD<schar, പൂർണ്ണസംഖ്യ> {
    പൂർണ്ണസംഖ്യ operator() (സ്ഥിരം schar* src1, സ്ഥിരം schar* src2, പൂർണ്ണസംഖ്യ n) സ്ഥിരം {
        പൂർണ്ണസംഖ്യ j = 0;
        പൂർണ്ണസംഖ്യ s = 0;
        v_uint32 r0 = vx_setzero_u32(), r1 = vx_setzero_u32();
        വേണ്ടി (; j <= n - 2 * VTraits<v_int8>::vlanes(); j += 2 * VTraits<v_int8>::vlanes()) {
            v_int8 v01 = vx_load(src1 + j), v02 = vx_load(src2 + j);
            v_uint8 v0 = v_absdiff(v01, v02);
            r0 = v_dotprod_expand_fast(v0, v0, r0);

            v_int8 v11 = vx_load(src1 + j + VTraits<v_int8>::vlanes()),
                   v12 = vx_load(src2 + j + VTraits<v_int8>::vlanes());
            v_uint8 v1 = v_absdiff(v11, v12);
            r1 = v_dotprod_expand_fast(v1, v1, r1);
        }
        s += v_reduce_sum(v_add(r0, r1));
        വേണ്ടി (; j < n; j++) {
            പൂർണ്ണസംഖ്യ v = (പൂർണ്ണസംഖ്യ)src1[j] - (പൂർണ്ണസംഖ്യ)src2[j];
            s += v * v;
        }
        തിരികെ s;
    }
};

#endif

#എങ്കിൽ (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)

template<>
ഘടന NormInf_SIMD<ഇരട്ട, ഇരട്ട> {
    ഇരട്ട operator() (സ്ഥിരം ഇരട്ട* src, പൂർണ്ണസംഖ്യ n) സ്ഥിരം {
        പൂർണ്ണസംഖ്യ j = 0;
        ഇരട്ട s = 0.f;
        v_float64 r0 = vx_setzero_f64(), r1 = vx_setzero_f64();
        v_float64 r2 = vx_setzero_f64(), r3 = vx_setzero_f64();
        വേണ്ടി (; j <= n - 4 * VTraits<v_float64>::vlanes(); j += 4 * VTraits<v_float64>::vlanes()) {
            r0 = v_max(r0, v_abs(vx_load(src + j                                   )));
            r1 = v_max(r1, v_abs(vx_load(src + j +     VTraits<v_float64>::vlanes())));
            r2 = v_max(r2, v_abs(vx_load(src + j + 2 * VTraits<v_float64>::vlanes())));
            r3 = v_max(r3, v_abs(vx_load(src + j + 3 * VTraits<v_float64>::vlanes())));
        }
        r0 = v_max(r0, v_max(r1, v_max(r2, r3)));
        വേണ്ടി (; j < n; j++) {
            s = std::max(s, cv_abs(src[j]));
        }
        // [TODO]: use v_reduce_max when it supports float64
        ഇരട്ട t[VTraits<v_float64>::max_nlanes];
        vx_store(t, r0);
        വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < VTraits<v_float64>::vlanes(); i++) {
            s = std::max(s, cv_abs(t[i]));
        }
        തിരികെ s;
    }
};

template<>
ഘടന NormL1_SIMD<പൂർണ്ണസംഖ്യ, ഇരട്ട> {
    ഇരട്ട operator() (സ്ഥിരം പൂർണ്ണസംഖ്യ* src, പൂർണ്ണസംഖ്യ n) സ്ഥിരം {
        പൂർണ്ണസംഖ്യ j = 0;
        ഇരട്ട s = 0.f;
        v_float64 r00 = vx_setzero_f64(), r01 = vx_setzero_f64();
        v_float64 r10 = vx_setzero_f64(), r11 = vx_setzero_f64();
        വേണ്ടി (; j <= n - 2 * VTraits<v_int32>::vlanes(); j += 2 * VTraits<v_int32>::vlanes()) {
            // Widen to double before taking the magnitude: exact over the whole int32 range,
            // INT_MIN included, which a float intermediate would not be above 2^24.
            v_int32 i0 = vx_load(src + j), i1 = vx_load(src + j + VTraits<v_int32>::vlanes());
            r00 = v_add(r00, v_abs(v_cvt_f64(i0))); r01 = v_add(r01, v_abs(v_cvt_f64_മുകളിലെ(i0)));
            r10 = v_add(r10, v_abs(v_cvt_f64(i1))); r11 = v_add(r11, v_abs(v_cvt_f64_മുകളിലെ(i1)));
        }
        s += v_reduce_sum(v_add(v_add(v_add(r00, r01), r10), r11));
        വേണ്ടി (; j < n; j++) {
            s += cv_abs(src[j]);
        }
        തിരികെ s;
    }
};

template<>
ഘടന NormL1_SIMD<ദശാംശം, ഇരട്ട> {
    ഇരട്ട operator() (സ്ഥിരം ദശാംശം* src, പൂർണ്ണസംഖ്യ n) സ്ഥിരം {
        പൂർണ്ണസംഖ്യ j = 0;
        ഇരട്ട s = 0.f;
        v_float64 r00 = vx_setzero_f64(), r01 = vx_setzero_f64();
        v_float64 r10 = vx_setzero_f64(), r11 = vx_setzero_f64();
        v_float64 r20 = vx_setzero_f64(), r21 = vx_setzero_f64();
        v_float64 r30 = vx_setzero_f64(), r31 = vx_setzero_f64();
        വേണ്ടി (; j <= n - 4 * VTraits<v_float32>::vlanes(); j += 4 * VTraits<v_float32>::vlanes()) {
            v_float32 v0 = v_abs(vx_load(src + j)), v1 = v_abs(vx_load(src + j + VTraits<v_float32>::vlanes()));
            r00 = v_add(r00, v_cvt_f64(v0)); r01 = v_add(r01, v_cvt_f64_മുകളിലെ(v0));
            r10 = v_add(r10, v_cvt_f64(v1)); r11 = v_add(r11, v_cvt_f64_മുകളിലെ(v1));

            v_float32 v2 = v_abs(vx_load(src + j + 2 * VTraits<v_float32>::vlanes())), v3 = v_abs(vx_load(src + j + 3 * VTraits<v_float32>::vlanes()));
            r20 = v_add(r20, v_cvt_f64(v2)); r21 = v_add(r21, v_cvt_f64_മുകളിലെ(v2));
            r30 = v_add(r30, v_cvt_f64(v3)); r31 = v_add(r31, v_cvt_f64_മുകളിലെ(v3));
        }
        s += v_reduce_sum(v_add(v_add(v_add(r00, r01), r10), r11));
        s += v_reduce_sum(v_add(v_add(v_add(r20, r21), r30), r31));
        വേണ്ടി (; j < n; j++) {
            s += cv_abs(src[j]);
        }
        തിരികെ s;
    }
};

template<>
ഘടന NormL2_SIMD<ushort, ഇരട്ട> {
    ഇരട്ട operator() (സ്ഥിരം ushort* src, പൂർണ്ണസംഖ്യ n) സ്ഥിരം {
        പൂർണ്ണസംഖ്യ j = 0;
        ഇരട്ട s = 0.f;
        v_float64 r0 = vx_setzero_f64(), r1 = vx_setzero_f64();
        വേണ്ടി (; j <= n - 2 * VTraits<v_uint16>::vlanes(); j += 2 * VTraits<v_uint16>::vlanes()) {
            v_uint16 v0 = vx_load(src + j);
            v_uint64 u0 = v_dotprod_expand_fast(v0, v0);
            r0 = v_add(r0, v_cvt_f64(v_reinterpret_as_s64(u0)));

            v_uint16 v1 = vx_load(src + j + VTraits<v_uint16>::vlanes());
            v_uint64 u1 = v_dotprod_expand_fast(v1, v1);
            r1 = v_add(r1, v_cvt_f64(v_reinterpret_as_s64(u1)));
        }
        s += v_reduce_sum(v_add(r0, r1));
        വേണ്ടി (; j < n; j++) {
            ഇരട്ട v = saturate_cast<ഇരട്ട>(src[j]);
            s += v * v;
        }
        തിരികെ s;
    }
};

template<>
ഘടന NormL2_SIMD<short, ഇരട്ട> {
    ഇരട്ട operator() (സ്ഥിരം short* src, പൂർണ്ണസംഖ്യ n) സ്ഥിരം {
        പൂർണ്ണസംഖ്യ j = 0;
        ഇരട്ട s = 0.f;
        v_float64 r0 = vx_setzero_f64(), r1 = vx_setzero_f64();
        വേണ്ടി (; j <= n - 2 * VTraits<v_int16>::vlanes(); j += 2 * VTraits<v_int16>::vlanes()) {
            v_int16 v0 = vx_load(src + j);
            r0 = v_add(r0, v_cvt_f64(v_dotprod_expand_fast(v0, v0)));

            v_int16 v1 = vx_load(src + j + VTraits<v_int16>::vlanes());
            r1 = v_add(r1, v_cvt_f64(v_dotprod_expand_fast(v1, v1)));
        }
        s += v_reduce_sum(v_add(r0, r1));
        വേണ്ടി (; j < n; j++) {
            ഇരട്ട v = saturate_cast<ഇരട്ട>(src[j]);
            s += v * v;
        }
        തിരികെ s;
    }
};

template<>
ഘടന NormL2_SIMD<പൂർണ്ണസംഖ്യ, ഇരട്ട> {
    ഇരട്ട operator() (സ്ഥിരം പൂർണ്ണസംഖ്യ* src, പൂർണ്ണസംഖ്യ n) സ്ഥിരം {
        പൂർണ്ണസംഖ്യ j = 0;
        ഇരട്ട s = 0.f;
        v_float64 r0 = vx_setzero_f64(), r1 = vx_setzero_f64();
        വേണ്ടി (; j <= n - 2 * VTraits<v_int32>::vlanes(); j += 2 * VTraits<v_int32>::vlanes()) {
            v_int32 v0 = vx_load(src + j);
            r0 = v_dotprod_expand_fast(v0, v0, r0);

            v_int32 v1 = vx_load(src + j + VTraits<v_int32>::vlanes());
            r1 = v_dotprod_expand_fast(v1, v1, r1);
        }
        s += v_reduce_sum(v_add(r0, r1));
        വേണ്ടി (; j < n; j++) {
            ഇരട്ട v = src[j];
            s += v * v;
        }
        തിരികെ s;
    }
};

template<>
ഘടന NormL2_SIMD<ദശാംശം, ഇരട്ട> {
    ഇരട്ട operator() (സ്ഥിരം ദശാംശം* src, പൂർണ്ണസംഖ്യ n) സ്ഥിരം {
        പൂർണ്ണസംഖ്യ j = 0;
        ഇരട്ട s = 0.f;
        v_float64 r00 = vx_setzero_f64(), r01 = vx_setzero_f64();
        v_float64 r10 = vx_setzero_f64(), r11 = vx_setzero_f64();
        വേണ്ടി (; j <= n - 2 * VTraits<v_float32>::vlanes(); j += 2 * VTraits<v_float32>::vlanes()) {
            v_float32 v0 = vx_load(src + j), v1 = vx_load(src + j + VTraits<v_float32>::vlanes());
            v_float64 v00 = v_cvt_f64(v0), v01 = v_cvt_f64_മുകളിലെ(v0);
            v_float64 v10 = v_cvt_f64(v1), v11 = v_cvt_f64_മുകളിലെ(v1);
            r00 = v_fma(v00, v00, r00); r01 = v_fma(v01, v01, r01);
            r10 = v_fma(v10, v10, r10); r11 = v_fma(v11, v11, r11);
        }
        s += v_reduce_sum(v_add(v_add(v_add(r00, r01), r10), r11));
        വേണ്ടി (; j < n; j++) {
            ഇരട്ട v = src[j];
            s += v * v;
        }
        തിരികെ s;
    }
};

#endif

#എങ്കിൽ CV_SIMD_64F // CV_SIMD_SCALABLE_64F has accuracy problem with the following kernels on ci

template<>
ഘടന NormL1_SIMD<ഇരട്ട, ഇരട്ട> {
    ഇരട്ട operator() (സ്ഥിരം ഇരട്ട* src, പൂർണ്ണസംഖ്യ n) സ്ഥിരം {
        പൂർണ്ണസംഖ്യ j = 0;
        ഇരട്ട s = 0.f;
        v_float64 r00 = vx_setzero_f64(), r01 = vx_setzero_f64();
        v_float64 r10 = vx_setzero_f64(), r11 = vx_setzero_f64();
        വേണ്ടി (; j <= n - 4 * VTraits<v_float64>::vlanes(); j += 4 * VTraits<v_float64>::vlanes()) {
            r00 = v_add(r00, v_abs(vx_load(src + j                                   )));
            r01 = v_add(r01, v_abs(vx_load(src + j +     VTraits<v_float64>::vlanes())));
            r10 = v_add(r10, v_abs(vx_load(src + j + 2 * VTraits<v_float64>::vlanes())));
            r11 = v_add(r11, v_abs(vx_load(src + j + 3 * VTraits<v_float64>::vlanes())));
        }
        s += v_reduce_sum(v_add(v_add(v_add(r00, r01), r10), r11));
        വേണ്ടി (; j < n; j++) {
            s += cv_abs(src[j]);
        }
        തിരികെ s;
    }
};

template<>
ഘടന NormL2_SIMD<ഇരട്ട, ഇരട്ട> {
    ഇരട്ട operator() (സ്ഥിരം ഇരട്ട* src, പൂർണ്ണസംഖ്യ n) സ്ഥിരം {
        പൂർണ്ണസംഖ്യ j = 0;
        ഇരട്ട s = 0.f;
        v_float64 r00 = vx_setzero_f64(), r01 = vx_setzero_f64();
        v_float64 r10 = vx_setzero_f64(), r11 = vx_setzero_f64();
        വേണ്ടി (; j <= n - 4 * VTraits<v_float64>::vlanes(); j += 4 * VTraits<v_float64>::vlanes()) {
            v_float64 v00 = vx_load(src + j                                   );
            v_float64 v01 = vx_load(src + j +     VTraits<v_float64>::vlanes());
            v_float64 v10 = vx_load(src + j + 2 * VTraits<v_float64>::vlanes());
            v_float64 v11 = vx_load(src + j + 3 * VTraits<v_float64>::vlanes());
            r00 = v_fma(v00, v00, r00); r01 = v_fma(v01, v01, r01);
            r10 = v_fma(v10, v10, r10); r11 = v_fma(v11, v11, r11);
        }
        s += v_reduce_sum(v_add(v_add(v_add(r00, r01), r10), r11));
        വേണ്ടി (; j < n; j++) {
            ഇരട്ട v = src[j];
            s += v * v;
        }
        തിരികെ s;
    }
};

template<>
ഘടന NormDiffInf_SIMD<ഇരട്ട, ഇരട്ട> {
    ഇരട്ട operator() (സ്ഥിരം ഇരട്ട* src1, സ്ഥിരം ഇരട്ട* src2, പൂർണ്ണസംഖ്യ n) സ്ഥിരം {
        പൂർണ്ണസംഖ്യ j = 0;
        ഇരട്ട s = 0;
        v_float64 r0 = vx_setzero_f64(), r1 = vx_setzero_f64();
        വേണ്ടി (; j <= n - 2 * VTraits<v_float64>::vlanes(); j += 2 * VTraits<v_float64>::vlanes()) {
            v_float64 v01 = vx_load(src1 + j), v02 = vx_load(src2 + j);
            r0 = v_max(r0, v_absdiff(v01, v02));

            v_float64 v11 = vx_load(src1 + j + VTraits<v_float64>::vlanes()),
                      v12 = vx_load(src2 + j + VTraits<v_float64>::vlanes());
            r1 = v_max(r1, v_absdiff(v11, v12));
        }
        // [TODO]: use v_reduce_max when it supports float64
        ഇരട്ട t[VTraits<v_float64>::max_nlanes];
        vx_store(t, v_max(r0, r1));
        വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < VTraits<v_float64>::vlanes(); i++) {
            s = std::max(s, t[i]);
        }
        വേണ്ടി (; j < n; j++) {
            ഇരട്ട v = (ഇരട്ട)cv_absdiff(src1[j], src2[j]);
            s = std::max(s, v);
        }
        തിരികെ s;
    }
};

template<>
ഘടന NormDiffL1_SIMD<ദശാംശം, ഇരട്ട> {
    ഇരട്ട operator() (സ്ഥിരം ദശാംശം* src1, സ്ഥിരം ദശാംശം* src2, പൂർണ്ണസംഖ്യ n) സ്ഥിരം {
        പൂർണ്ണസംഖ്യ j = 0;
        ഇരട്ട s = 0.f;
        v_float64 r0 = vx_setzero_f64(), r1 = vx_setzero_f64();
        v_float64 r2 = vx_setzero_f64(), r3 = vx_setzero_f64();
        വേണ്ടി (; j <= n - 2 * VTraits<v_float32>::vlanes(); j += 2 * VTraits<v_float32>::vlanes()) {
            v_float32 v01 = vx_load(src1 + j), v02 = vx_load(src2 + j);
            v_float32 v0 = v_absdiff(v01, v02);
            r0 = v_add(r0, v_cvt_f64(v0)); r1 = v_add(r1, v_cvt_f64_മുകളിലെ(v0));

            v_float32 v11 = vx_load(src1 + j + VTraits<v_float32>::vlanes()),
                      v12 = vx_load(src2 + j + VTraits<v_float32>::vlanes());
            v_float32 v1 = v_absdiff(v11, v12);
            r2 = v_add(r2, v_cvt_f64(v1)); r3 = v_add(r3, v_cvt_f64_മുകളിലെ(v1));
        }
        s += v_reduce_sum(v_add(v_add(v_add(r0, r1), r2), r3));
        വേണ്ടി (; j < n; j++) {
            ഇരട്ട v = (ഇരട്ട)cv_absdiff(src1[j], src2[j]);
            s += v;
        }
        തിരികെ s;
    }
};

template<>
ഘടന NormDiffL1_SIMD<ഇരട്ട, ഇരട്ട> {
    ഇരട്ട operator() (സ്ഥിരം ഇരട്ട* src1, സ്ഥിരം ഇരട്ട* src2, പൂർണ്ണസംഖ്യ n) സ്ഥിരം {
        പൂർണ്ണസംഖ്യ j = 0;
        ഇരട്ട s = 0.f;
        v_float64 r0 = vx_setzero_f64(), r1 = vx_setzero_f64();
        വേണ്ടി (; j <= n - 2 * VTraits<v_float64>::vlanes(); j += 2 * VTraits<v_float64>::vlanes()) {
            v_float64 v01 = vx_load(src1 + j), v02 = vx_load(src2 + j);
            r0 = v_add(r0, v_absdiff(v01, v02));

            v_float64 v11 = vx_load(src1 + j + VTraits<v_float64>::vlanes()),
                      v12 = vx_load(src2 + j + VTraits<v_float64>::vlanes());
            r1 = v_add(r1, v_absdiff(v11, v12));
        }
        s += v_reduce_sum(v_add(r0, r1));
        വേണ്ടി (; j < n; j++) {
            ഇരട്ട v = (ഇരട്ട)cv_absdiff(src1[j], src2[j]);
            s += v;
        }
        തിരികെ s;
    }
};

template<>
ഘടന NormDiffL2_SIMD<ushort, ഇരട്ട> {
    ഇരട്ട operator() (സ്ഥിരം ushort* src1, സ്ഥിരം ushort* src2, പൂർണ്ണസംഖ്യ n) സ്ഥിരം {
        പൂർണ്ണസംഖ്യ j = 0;
        ഇരട്ട s = 0.f;
        v_float64 r0 = vx_setzero_f64(), r1 = vx_setzero_f64();
        വേണ്ടി (; j <= n - 2 * VTraits<v_uint16>::vlanes(); j += 2 * VTraits<v_uint16>::vlanes()) {
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
        വേണ്ടി (; j < n; j++) {
            ഇരട്ട v = (ഇരട്ട)src1[j] - (ഇരട്ട)src2[j];
            s += v * v;
        }
        തിരികെ s;
    }
};

template<>
ഘടന NormDiffL2_SIMD<short, ഇരട്ട> {
    ഇരട്ട operator() (സ്ഥിരം short* src1, സ്ഥിരം short* src2, പൂർണ്ണസംഖ്യ n) സ്ഥിരം {
        പൂർണ്ണസംഖ്യ j = 0;
        ഇരട്ട s = 0.f;
        v_float64 r0 = vx_setzero_f64(), r1 = vx_setzero_f64();
        വേണ്ടി (; j <= n - 2 * VTraits<v_int16>::vlanes(); j += 2 * VTraits<v_int16>::vlanes()) {
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
        വേണ്ടി (; j < n; j++) {
            ഇരട്ട v = (ഇരട്ട)src1[j] - (ഇരട്ട)src2[j];
            s += v * v;
        }
        തിരികെ s;
    }
};

template<>
ഘടന NormDiffL2_SIMD<പൂർണ്ണസംഖ്യ, ഇരട്ട> {
    ഇരട്ട operator() (സ്ഥിരം പൂർണ്ണസംഖ്യ* src1, സ്ഥിരം പൂർണ്ണസംഖ്യ* src2, പൂർണ്ണസംഖ്യ n) സ്ഥിരം {
        പൂർണ്ണസംഖ്യ j = 0;
        ഇരട്ട s = 0.f;
        v_float64 r0 = vx_setzero_f64(), r1 = vx_setzero_f64();
        വേണ്ടി (; j <= n - VTraits<v_int32>::vlanes(); j += VTraits<v_int32>::vlanes()) {
            v_int32 v01 = vx_load(src1 + j), v02 = vx_load(src2 + j);
            v_uint32 v0 = v_absdiff(v01, v02);
            v_uint64 ev0, ev1;
            v_expand(v0, ev0, ev1);
            v_float64 f0 = v_cvt_f64(v_reinterpret_as_s64(ev0)), f1 = v_cvt_f64(v_reinterpret_as_s64(ev1));
            r0 = v_fma(f0, f0, r0); r1 = v_fma(f1, f1, r1);
        }
        s += v_reduce_sum(v_add(r0, r1));
        വേണ്ടി (; j < n; j++) {
            ഇരട്ട v = (ഇരട്ട)src1[j] - (ഇരട്ട)src2[j];
            s += v * v;
        }
        തിരികെ s;
    }
};

template<>
ഘടന NormDiffL2_SIMD<ദശാംശം, ഇരട്ട> {
    ഇരട്ട operator() (സ്ഥിരം ദശാംശം* src1, സ്ഥിരം ദശാംശം* src2, പൂർണ്ണസംഖ്യ n) സ്ഥിരം {
        പൂർണ്ണസംഖ്യ j = 0;
        ഇരട്ട s = 0.f;
        v_float64 r0 = vx_setzero_f64(), r1 = vx_setzero_f64();
        v_float64 r2 = vx_setzero_f64(), r3 = vx_setzero_f64();
        വേണ്ടി (; j <= n - 2 * VTraits<v_float32>::vlanes(); j += 2 * VTraits<v_float32>::vlanes()) {
            v_float32 v01 = vx_load(src1 + j), v02 = vx_load(src2 + j);
            v_float32 v0 = v_absdiff(v01, v02);
            v_float64 f01 = v_cvt_f64(v0), f02 = v_cvt_f64_മുകളിലെ(v0);
            r0 = v_fma(f01, f01, r0); r1 = v_fma(f02, f02, r1);

            v_float32 v11 = vx_load(src1 + j + VTraits<v_float32>::vlanes()),
                      v12 = vx_load(src2 + j + VTraits<v_float32>::vlanes());
            v_float32 v1 = v_absdiff(v11, v12);
            v_float64 f11 = v_cvt_f64(v1), f12 = v_cvt_f64_മുകളിലെ(v1);
            r2 = v_fma(f11, f11, r2); r3 = v_fma(f12, f12, r3);
        }
        s += v_reduce_sum(v_add(v_add(v_add(r0, r1), r2), r3));
        വേണ്ടി (; j < n; j++) {
            ഇരട്ട v = (ഇരട്ട)src1[j] - (ഇരട്ട)src2[j];
            s += v * v;
        }
        തിരികെ s;
    }
};

template<>
ഘടന NormDiffL2_SIMD<ഇരട്ട, ഇരട്ട> {
    ഇരട്ട operator() (സ്ഥിരം ഇരട്ട* src1, സ്ഥിരം ഇരട്ട* src2, പൂർണ്ണസംഖ്യ n) സ്ഥിരം {
        പൂർണ്ണസംഖ്യ j = 0;
        ഇരട്ട s = 0.f;
        v_float64 r0 = vx_setzero_f64(), r1 = vx_setzero_f64();
        v_float64 r2 = vx_setzero_f64(), r3 = vx_setzero_f64();
        വേണ്ടി (; j <= n - 4 * VTraits<v_float64>::vlanes(); j += 4 * VTraits<v_float64>::vlanes()) {
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
        വേണ്ടി (; j < n; j++) {
            ഇരട്ട v = (ഇരട്ട)src1[j] - (ഇരട്ട)src2[j];
            s += v * v;
        }
        തിരികെ s;
    }
};

#endif

template <typename T, typename ST>
ഘടന MaskedNormInf_SIMD {
    വരിയിൽ ST operator() (സ്ഥിരം T* src, സ്ഥിരം uchar* mask, പൂർണ്ണസംഖ്യ len, പൂർണ്ണസംഖ്യ cn) സ്ഥിരം {
        ST s = 0;
        എങ്കിൽ (cn == 1) {
            വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < len; i++) {
                എങ്കിൽ (mask[i]) {
                    s = std::max(s, (ST)cv_abs(src[i]));
                }
            }
        }
        അല്ലെങ്കിൽ {
            വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < len; i++) {
                എങ്കിൽ (mask[i]) {
                    സ്ഥിരം T* elem = src + i * cn;
                    പൂർണ്ണസംഖ്യ k = 0;
                #എങ്കിൽ CV_ENABLE_UNROLLED && !CV_SIMD_SCALABLE
                    വേണ്ടി (; k <= cn - 4; k += 4) {
                        s = std::max(s, (ST)cv_abs(elem[k]));
                        s = std::max(s, (ST)cv_abs(elem[k + 1]));
                        s = std::max(s, (ST)cv_abs(elem[k + 2]));
                        s = std::max(s, (ST)cv_abs(elem[k + 3]));
                    }
                #endif
                    വേണ്ടി (; k < cn; k++) {
                        s = std::max(s, (ST)cv_abs(elem[k]));
                    }
                }
            }
        }
        തിരികെ s;
    }
};

template <typename T, typename ST>
ഘടന MaskedNormL1_SIMD {
    വരിയിൽ ST operator() (സ്ഥിരം T* src, സ്ഥിരം uchar* mask, പൂർണ്ണസംഖ്യ len, പൂർണ്ണസംഖ്യ cn) സ്ഥിരം {
        ST s = 0;
        എങ്കിൽ (cn == 1) {
            വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < len; i++) {
                എങ്കിൽ (mask[i]) {
                    s += (ST)cv_abs(src[i]);
                }
            }
        }
        അല്ലെങ്കിൽ {
            വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < len; i++) {
                എങ്കിൽ (mask[i]) {
                    സ്ഥിരം T* elem = src + i * cn;
                    പൂർണ്ണസംഖ്യ k = 0;
                #എങ്കിൽ CV_ENABLE_UNROLLED && !CV_SIMD_SCALABLE
                    വേണ്ടി (; k <= cn - 4; k += 4) {
                        s += (ST)cv_abs(elem[k]);
                        s += (ST)cv_abs(elem[k + 1]);
                        s += (ST)cv_abs(elem[k + 2]);
                        s += (ST)cv_abs(elem[k + 3]);
                    }
                #endif
                    വേണ്ടി (; k < cn; k++) {
                        s += (ST)cv_abs(elem[k]);
                    }
                }
            }
        }
        തിരികെ s;
    }
};

template <typename T, typename ST>
ഘടന MaskedNormL2_SIMD {
    വരിയിൽ ST operator() (സ്ഥിരം T* src, സ്ഥിരം uchar* mask, പൂർണ്ണസംഖ്യ len, പൂർണ്ണസംഖ്യ cn) സ്ഥിരം {
        ST s = 0;
        എങ്കിൽ (cn == 1) {
            പൂർണ്ണസംഖ്യ i = 0;
        #എങ്കിൽ CV_ENABLE_UNROLLED && !CV_SIMD_SCALABLE
            വേണ്ടി (; i <= len - 4; i += 4) {
                എങ്കിൽ (mask[i])     { T v0 = src[i];     s += (ST)v0 * v0; }
                എങ്കിൽ (mask[i + 1]) { T v1 = src[i + 1]; s += (ST)v1 * v1; }
                എങ്കിൽ (mask[i + 2]) { T v2 = src[i + 2]; s += (ST)v2 * v2; }
                എങ്കിൽ (mask[i + 3]) { T v3 = src[i + 3]; s += (ST)v3 * v3; }
            }
        #endif
            വേണ്ടി (; i < len; i++) {
                എങ്കിൽ (mask[i]) {
                    T v = src[i];
                    s += (ST)v * v;
                }
            }
        }
        അല്ലെങ്കിൽ {
            വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < len; i++) {
                എങ്കിൽ (mask[i]) {
                    സ്ഥിരം T* elem = src + i * cn;
                    പൂർണ്ണസംഖ്യ k = 0;
                #എങ്കിൽ CV_ENABLE_UNROLLED && !CV_SIMD_SCALABLE
                    വേണ്ടി (; k <= cn - 4; k += 4) {
                        T v0 = elem[k];     s += (ST)v0 * v0;
                        T v1 = elem[k + 1]; s += (ST)v1 * v1;
                        T v2 = elem[k + 2]; s += (ST)v2 * v2;
                        T v3 = elem[k + 3]; s += (ST)v3 * v3;
                    }
                #endif
                    വേണ്ടി (; k < cn; k++) {
                        T v = elem[k];
                        s += (ST)v * v;
                    }
                }
            }
        }

        തിരികെ s;
    }
};

#എങ്കിൽ (CV_SIMD || CV_SIMD_SCALABLE)
സ്ഥിതം വരിയിൽ v_uint8  v_normmask_u8 (സ്ഥിരം uchar* m) { തിരികെ v_gt(vx_load(m),          vx_setzero_u8());  }
സ്ഥിതം വരിയിൽ v_uint16 v_normmask_u16(സ്ഥിരം uchar* m) { തിരികെ v_gt(vx_load_expand(m),   vx_setzero_u16()); }
സ്ഥിതം വരിയിൽ v_uint32 v_normmask_u32(സ്ഥിരം uchar* m) { തിരികെ v_gt(vx_load_expand_q(m), vx_setzero_u32()); }
സ്ഥിതം വരിയിൽ ശൂന്യം v_normmask_u64(സ്ഥിരം uchar* m, v_uint64& m0, v_uint64& m1) {
    v_uint64 e0, e1;
    v_expand(v_normmask_u32(m), e0, e1);
    m0 = v_or(e0, v_shl<32>(e0));
    m1 = v_or(e1, v_shl<32>(e1));
}

template <>
ഘടന MaskedNormInf_SIMD<ദശാംശം, ദശാംശം> {
    വരിയിൽ ദശാംശം operator()(സ്ഥിരം ദശാംശം* src, സ്ഥിരം uchar* mask, പൂർണ്ണസംഖ്യ len, പൂർണ്ണസംഖ്യ cn) സ്ഥിരം {
        ദശാംശം result = 0.0f;
        എങ്കിൽ (cn == 1) {
            പൂർണ്ണസംഖ്യ i = 0;
            സ്ഥിരം പൂർണ്ണസംഖ്യ vstep = VTraits<v_float32>::vlanes();
            v_float32 acc = vx_setzero_f32();

            വേണ്ടി (; i <= len - vstep; i += vstep) {
                v_uint32 m = vx_load_expand_q(mask + i);
                v_uint32 cmp = v_gt(m, vx_setzero_u32());
                v_float32 s = vx_load(src + i);
                s = v_abs(s);
                s = v_reinterpret_as_f32(v_and(v_reinterpret_as_u32(s), cmp));
                acc = v_max(acc, s);
            }
            result = v_reduce_max(acc);

            വേണ്ടി (; i < len; i++) {
                എങ്കിൽ (mask[i])
                    result = std::max(result, std::abs(src[i]));
            }
        }
        അല്ലെങ്കിൽ {
            വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < len; i++) {
                എങ്കിൽ (mask[i]) {
                    സ്ഥിരം ദശാംശം* elem = src + i * cn;
                    പൂർണ്ണസംഖ്യ k = 0;
                    സ്ഥിരം പൂർണ്ണസംഖ്യ vstep = VTraits<v_float32>::vlanes();
                    v_float32 acc = vx_setzero_f32();

                    വേണ്ടി (; k <= cn - vstep; k += vstep) {
                        v_float32 s = vx_load(elem + k);
                        acc = v_max(acc, v_abs(s));
                    }

                    result = std::max(result, v_reduce_max(acc));

                    വേണ്ടി (; k < cn; k++)
                        result = std::max(result, std::abs(elem[k]));
                }
            }
        }
        തിരികെ result;
    }
};

#എങ്കിൽ CV_SIMD_64F
template <>
ഘടന MaskedNormL1_SIMD<ദശാംശം, ഇരട്ട> {
    വരിയിൽ ഇരട്ട operator()(സ്ഥിരം ദശാംശം* src, സ്ഥിരം uchar* mask, പൂർണ്ണസംഖ്യ len, പൂർണ്ണസംഖ്യ cn) സ്ഥിരം {
        ഇരട്ട result = 0.0;
        എങ്കിൽ (cn == 1) {
            പൂർണ്ണസംഖ്യ i = 0;
            സ്ഥിരം പൂർണ്ണസംഖ്യ vstep = VTraits<v_float32>::vlanes();
            v_float64 acc0 = vx_setzero_f64(), acc1 = vx_setzero_f64();
            v_float64 acc2 = vx_setzero_f64(), acc3 = vx_setzero_f64();

            വേണ്ടി (; i <= len - 2*vstep; i += 2*vstep) {
                v_float32 s0 = v_and(v_abs(vx_load(src + i)),
                                     v_reinterpret_as_f32(v_normmask_u32(mask + i)));
                v_float32 s1 = v_and(v_abs(vx_load(src + i + vstep)),
                                     v_reinterpret_as_f32(v_normmask_u32(mask + i + vstep)));
                acc0 = v_add(acc0, v_cvt_f64(s0));
                acc1 = v_add(acc1, v_cvt_f64_മുകളിലെ(s0));
                acc2 = v_add(acc2, v_cvt_f64(s1));
                acc3 = v_add(acc3, v_cvt_f64_മുകളിലെ(s1));
            }
            വേണ്ടി (; i <= len - vstep; i += vstep) {
                v_float32 s = v_and(v_abs(vx_load(src + i)),
                                    v_reinterpret_as_f32(v_normmask_u32(mask + i)));
                acc0 = v_add(acc0, v_cvt_f64(s));
                acc1 = v_add(acc1, v_cvt_f64_മുകളിലെ(s));
            }
            result = v_reduce_sum(v_add(v_add(acc0, acc1), v_add(acc2, acc3)));

            വേണ്ടി (; i < len; i++) {
                എങ്കിൽ (mask[i])
                    result += std::abs(src[i]);
            }
        }
        അല്ലെങ്കിൽ {
            വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < len; i++) {
                എങ്കിൽ (mask[i]) {
                    സ്ഥിരം ദശാംശം* elem = src + i * cn;
                    പൂർണ്ണസംഖ്യ k = 0;
                    സ്ഥിരം പൂർണ്ണസംഖ്യ vstep = VTraits<v_float32>::vlanes();
                    v_float64 acc = vx_setzero_f64();

                    വേണ്ടി (; k <= cn - vstep; k += vstep) {
                        v_float32 s = v_abs(vx_load(elem + k));
                        acc = v_add(acc, v_cvt_f64(s));
                        acc = v_add(acc, v_cvt_f64_മുകളിലെ(s));
                    }

                    result += v_reduce_sum(acc);

                    വേണ്ടി (; k < cn; k++)
                        result += std::abs(elem[k]);
                }
            }
        }
        തിരികെ result;
    }
};

template <>
ഘടന MaskedNormL2_SIMD<ദശാംശം, ഇരട്ട> {
    വരിയിൽ ഇരട്ട operator()(സ്ഥിരം ദശാംശം* src, സ്ഥിരം uchar* mask, പൂർണ്ണസംഖ്യ len, പൂർണ്ണസംഖ്യ cn) സ്ഥിരം {
        ഇരട്ട result = 0.0;
        എങ്കിൽ (cn == 1) {
            പൂർണ്ണസംഖ്യ i = 0;
            സ്ഥിരം പൂർണ്ണസംഖ്യ vstep = VTraits<v_float32>::vlanes();
            v_float32 facc = vx_setzero_f32();
            v_float64 dacc = vx_setzero_f64();
            പൂർണ്ണസംഖ്യ flush = 0;

            വേണ്ടി (; i <= len - vstep; i += vstep, flush += vstep) {
                എങ്കിൽ (flush >= 64) {
                    dacc = v_add(dacc, v_cvt_f64(facc));
                    dacc = v_add(dacc, v_cvt_f64_മുകളിലെ(facc));
                    facc = vx_setzero_f32();
                    flush = 0;
                }
                v_uint32 cmp = v_gt(vx_load_expand_q(mask + i), vx_setzero_u32());
                v_float32 s  = v_reinterpret_as_f32(v_and(v_reinterpret_as_u32(vx_load(src + i)), cmp));
                facc = v_add(facc, v_mul(s, s));
            }

            dacc = v_add(dacc, v_cvt_f64(facc));
            dacc = v_add(dacc, v_cvt_f64_മുകളിലെ(facc));
            result = v_reduce_sum(dacc);

            വേണ്ടി (; i < len; i++) {
                എങ്കിൽ (mask[i]) {
                    ഇരട്ട v = src[i];
                    result += v * v;
                }
            }
        }
        അല്ലെങ്കിൽ {
            വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < len; i++) {
                എങ്കിൽ (mask[i]) {
                    സ്ഥിരം ദശാംശം* elem = src + i * cn;
                    പൂർണ്ണസംഖ്യ k = 0;
                    സ്ഥിരം പൂർണ്ണസംഖ്യ vstep = VTraits<v_float32>::vlanes();
                    v_float32 facc = vx_setzero_f32();

                    വേണ്ടി (; k <= cn - vstep; k += vstep) {
                        v_float32 s = vx_load(elem + k);
                        facc = v_add(facc, v_mul(s, s));
                    }

                    v_float64 dacc = v_add(v_cvt_f64(facc), v_cvt_f64_മുകളിലെ(facc));
                    result += v_reduce_sum(dacc);

                    വേണ്ടി (; k < cn; k++) {
                        ഇരട്ട v = elem[k];
                        result += v * v;
                    }
                }
            }
        }
        തിരികെ result;
    }
};

#endif

template <>
ഘടന MaskedNormInf_SIMD<uchar, പൂർണ്ണസംഖ്യ> {
    വരിയിൽ പൂർണ്ണസംഖ്യ operator()(സ്ഥിരം uchar* src, സ്ഥിരം uchar* mask, പൂർണ്ണസംഖ്യ len, പൂർണ്ണസംഖ്യ cn) സ്ഥിരം {
        പൂർണ്ണസംഖ്യ result = 0;
        എങ്കിൽ (cn == 1) {
            പൂർണ്ണസംഖ്യ i = 0;
            സ്ഥിരം പൂർണ്ണസംഖ്യ vstep = VTraits<v_uint8>::vlanes();
            v_uint8 acc = vx_setzero_u8();

            വേണ്ടി (; i <= len - vstep; i += vstep) {
                v_uint8 m   = vx_load(mask + i);
                v_uint8 s   = vx_load(src + i);
                v_uint8 sel = v_and(s, v_gt(m, vx_setzero_u8()));
                acc = v_max(acc, sel);
            }

            result = (പൂർണ്ണസംഖ്യ)v_reduce_max(acc);

            വേണ്ടി (; i < len; i++) {
                എങ്കിൽ (mask[i])
                    result = std::max(result, (പൂർണ്ണസംഖ്യ)src[i]);
            }
        }
        അല്ലെങ്കിൽ എങ്കിൽ (cn == 4 && len >= VTraits<v_uint8>::vlanes()) {
            സ്ഥിരം പൂർണ്ണസംഖ്യ vstep = VTraits<v_uint8>::vlanes();
            v_uint8 acc = vx_setzero_u8();
            പൂർണ്ണസംഖ്യ i = 0;
            വേണ്ടി (;;) {
                എങ്കിൽ (i > len - vstep)
                    i = len - vstep; // back-step (max is idempotent)
                v_uint8 c0, c1, c2, c3;
                v_load_deinterleave(src + i * 4, c0, c1, c2, c3);
                v_uint8 pmax = v_max(v_max(c0, c1), v_max(c2, c3));
                v_uint8 m = v_gt(vx_load(mask + i), vx_setzero_u8());
                acc = v_max(acc, v_and(pmax, m));
                എങ്കിൽ (i >= len - vstep)
                    നിർത്തുക;
                i += vstep;
            }
            result = (പൂർണ്ണസംഖ്യ)v_reduce_max(acc);
        }
        അല്ലെങ്കിൽ എങ്കിൽ (cn == 4) {
            വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < len; i++) {
                എങ്കിൽ (mask[i]) {
                    സ്ഥിരം uchar* elem = src + i * 4;
                    result = std::max(result, (പൂർണ്ണസംഖ്യ)std::max(std::max(elem[0], elem[1]),
                                                            std::max(elem[2], elem[3])));
                }
            }
        }
        അല്ലെങ്കിൽ {
            വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < len; i++) {
                എങ്കിൽ (mask[i]) {
                    സ്ഥിരം uchar* elem = src + i * cn;
                    വേണ്ടി (പൂർണ്ണസംഖ്യ k = 0; k < cn; k++)
                        result = std::max(result, (പൂർണ്ണസംഖ്യ)elem[k]);
                }
            }
        }
        തിരികെ result;
    }
};

template <>
ഘടന MaskedNormL1_SIMD<uchar, പൂർണ്ണസംഖ്യ> {
    വരിയിൽ പൂർണ്ണസംഖ്യ operator()(സ്ഥിരം uchar* src, സ്ഥിരം uchar* mask, പൂർണ്ണസംഖ്യ len, പൂർണ്ണസംഖ്യ cn) സ്ഥിരം {
        പൂർണ്ണസംഖ്യ result = 0;
        സ്ഥിരം പൂർണ്ണസംഖ്യ vstep = VTraits<v_uint8>::vlanes();
        സ്ഥിരം v_uint8 one = vx_setall_u8(1);
        എങ്കിൽ (cn == 1) {
            പൂർണ്ണസംഖ്യ i = 0;
            v_uint32 acc = vx_setzero_u32();
            വേണ്ടി (; i <= len - vstep; i += vstep) {
                v_uint8 s = v_and(vx_load(src + i), v_normmask_u8(mask + i));
                acc = v_dotprod_expand_fast(s, one, acc);
            }
            result = (പൂർണ്ണസംഖ്യ)v_reduce_sum(acc);
            വേണ്ടി (; i < len; i++) {
                എങ്കിൽ (mask[i])
                    result += src[i];
            }
        }
        അല്ലെങ്കിൽ എങ്കിൽ (cn == 4) {
            പൂർണ്ണസംഖ്യ i = 0;
            v_uint32 acc = vx_setzero_u32();
            വേണ്ടി (; i <= len - vstep; i += vstep) {
                v_uint8 c0, c1, c2, c3;
                v_load_deinterleave(src + i * 4, c0, c1, c2, c3);
                v_uint8 m = v_normmask_u8(mask + i);
                acc = v_dotprod_expand_fast(v_and(c0, m), one, acc);
                acc = v_dotprod_expand_fast(v_and(c1, m), one, acc);
                acc = v_dotprod_expand_fast(v_and(c2, m), one, acc);
                acc = v_dotprod_expand_fast(v_and(c3, m), one, acc);
            }
            result = (പൂർണ്ണസംഖ്യ)v_reduce_sum(acc);
            വേണ്ടി (; i < len; i++) {
                എങ്കിൽ (mask[i]) {
                    സ്ഥിരം uchar* elem = src + i * 4;
                    result += elem[0] + elem[1] + elem[2] + elem[3];
                }
            }
        }
        അല്ലെങ്കിൽ {
            വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < len; i++) {
                എങ്കിൽ (mask[i]) {
                    സ്ഥിരം uchar* elem = src + i * cn;
                    വേണ്ടി (പൂർണ്ണസംഖ്യ k = 0; k < cn; k++)
                        result += elem[k];
                }
            }
        }
        തിരികെ result;
    }
};

template <>
ഘടന MaskedNormInf_SIMD<ushort, പൂർണ്ണസംഖ്യ> {
    വരിയിൽ പൂർണ്ണസംഖ്യ operator()(സ്ഥിരം ushort* src, സ്ഥിരം uchar* mask, പൂർണ്ണസംഖ്യ len, പൂർണ്ണസംഖ്യ cn) സ്ഥിരം {
        പൂർണ്ണസംഖ്യ result = 0;
        എങ്കിൽ (cn == 1) {
            പൂർണ്ണസംഖ്യ i = 0;
            സ്ഥിരം പൂർണ്ണസംഖ്യ vstep = VTraits<v_uint16>::vlanes();
            v_uint16 acc = vx_setzero_u16();

            വേണ്ടി (; i <= len - vstep; i += vstep) {
                v_uint16 m   = vx_load_expand(mask + i);
                v_uint16 cmp = v_gt(m, vx_setzero_u16());
                v_uint16 s   = vx_load(src + i);
                v_uint16 sel = v_and(s, cmp);
                acc = v_max(acc, sel);
            }

            result = (പൂർണ്ണസംഖ്യ)v_reduce_max(acc);

            വേണ്ടി (; i < len; i++) {
                എങ്കിൽ (mask[i])
                    result = std::max(result, (പൂർണ്ണസംഖ്യ)src[i]);
            }
        }
        അല്ലെങ്കിൽ {
            വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < len; i++) {
                എങ്കിൽ (mask[i]) {
                    സ്ഥിരം ushort* elem = src + i * cn;
                    പൂർണ്ണസംഖ്യ k = 0;
                    സ്ഥിരം പൂർണ്ണസംഖ്യ vstep = VTraits<v_uint16>::vlanes();
                    v_uint16 acc = vx_setzero_u16();

                    വേണ്ടി (; k <= cn - vstep; k += vstep) {
                        acc = v_max(acc, vx_load(elem + k));
                    }

                    result = std::max(result, (പൂർണ്ണസംഖ്യ)v_reduce_max(acc));

                    വേണ്ടി (; k < cn; k++)
                        result = std::max(result, (പൂർണ്ണസംഖ്യ)elem[k]);
                }
            }
        }
        തിരികെ result;
    }
};

template <>
ഘടന MaskedNormL1_SIMD<ushort, പൂർണ്ണസംഖ്യ> {
    വരിയിൽ പൂർണ്ണസംഖ്യ operator()(സ്ഥിരം ushort* src, സ്ഥിരം uchar* mask, പൂർണ്ണസംഖ്യ len, പൂർണ്ണസംഖ്യ cn) സ്ഥിരം {
        പൂർണ്ണസംഖ്യ result = 0;
        എങ്കിൽ (cn == 1) {
            പൂർണ്ണസംഖ്യ i = 0;
            സ്ഥിരം പൂർണ്ണസംഖ്യ vstep = VTraits<v_uint16>::vlanes();
            v_uint32 acc32 = vx_setzero_u32();
            v_uint64 acc64 = vx_setzero_u64();
            പൂർണ്ണസംഖ്യ acc32_elems = 0;

            വേണ്ടി (; i <= len - vstep; i += vstep, acc32_elems += vstep) {
                എങ്കിൽ (acc32_elems >= 512) {
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
            result = (പൂർണ്ണസംഖ്യ)v_reduce_sum(acc64);

            വേണ്ടി (; i < len; i++) {
                എങ്കിൽ (mask[i])
                    result += src[i];
            }
        }
        അല്ലെങ്കിൽ {
            വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < len; i++) {
                എങ്കിൽ (mask[i]) {
                    സ്ഥിരം ushort* elem = src + i * cn;
                    പൂർണ്ണസംഖ്യ k = 0;
                    സ്ഥിരം പൂർണ്ണസംഖ്യ vstep = VTraits<v_uint16>::vlanes();
                    v_uint32 acc = vx_setzero_u32();

                    വേണ്ടി (; k <= cn - vstep; k += vstep) {
                        v_uint32 lo32, hi32;
                        v_expand(vx_load(elem + k), lo32, hi32);
                        acc = v_add(acc, v_add(lo32, hi32));
                    }

                    result += (പൂർണ്ണസംഖ്യ)v_reduce_sum(acc);

                    വേണ്ടി (; k < cn; k++)
                        result += elem[k];
                }
            }
        }
        തിരികെ result;
    }
};

template <>
ഘടന MaskedNormL2_SIMD<ushort, ഇരട്ട> {
    വരിയിൽ ഇരട്ട operator()(സ്ഥിരം ushort* src, സ്ഥിരം uchar* mask, പൂർണ്ണസംഖ്യ len, പൂർണ്ണസംഖ്യ cn) സ്ഥിരം {
        ഇരട്ട result = 0.0;
        എങ്കിൽ (cn == 1) {
            പൂർണ്ണസംഖ്യ i = 0;
            സ്ഥിരം പൂർണ്ണസംഖ്യ vstep = VTraits<v_uint16>::vlanes();
            v_uint64 acc = vx_setzero_u64();
            വേണ്ടി (; i <= len - vstep; i += vstep) {
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
            result = (ഇരട്ട)v_reduce_sum(acc);
            വേണ്ടി (; i < len; i++) {
                എങ്കിൽ (mask[i]) {
                    ഇരട്ട v = src[i];
                    result += v * v;
                }
            }
        }
        അല്ലെങ്കിൽ {
            വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < len; i++) {
                എങ്കിൽ (mask[i]) {
                    സ്ഥിരം ushort* elem = src + i * cn;
                    പൂർണ്ണസംഖ്യ k = 0;
                    സ്ഥിരം പൂർണ്ണസംഖ്യ vstep = VTraits<v_uint16>::vlanes();
                    v_uint64 acc = vx_setzero_u64();
                    വേണ്ടി (; k <= cn - vstep; k += vstep) {
                        v_uint32 lo32, hi32;
                        v_expand(vx_load(elem + k), lo32, hi32);
                        v_uint64 lo64a, lo64b, hi64a, hi64b;
                        v_expand(v_mul(lo32, lo32), lo64a, lo64b);
                        v_expand(v_mul(hi32, hi32), hi64a, hi64b);
                        acc = v_add(acc, v_add(v_add(lo64a, lo64b), v_add(hi64a, hi64b)));
                    }

                    result += (ഇരട്ട)v_reduce_sum(acc);

                    വേണ്ടി (; k < cn; k++) {
                        ഇരട്ട v = elem[k];
                        result += v * v;
                    }
                }
            }
        }
        തിരികെ result;
    }
};

template <>
ഘടന MaskedNormInf_SIMD<schar, പൂർണ്ണസംഖ്യ> {
    വരിയിൽ പൂർണ്ണസംഖ്യ operator()(സ്ഥിരം schar* src, സ്ഥിരം uchar* mask, പൂർണ്ണസംഖ്യ len, പൂർണ്ണസംഖ്യ cn) സ്ഥിരം {
        പൂർണ്ണസംഖ്യ result = 0;
        എങ്കിൽ (cn == 1) {
            സ്ഥിരം പൂർണ്ണസംഖ്യ vstep = VTraits<v_int8>::vlanes();
            പൂർണ്ണസംഖ്യ i = 0;
            v_uint8 acc = vx_setzero_u8();
            വേണ്ടി (; i <= len - vstep; i += vstep)
                acc = v_max(acc, v_and(v_abs(vx_load(src + i)), v_normmask_u8(mask + i)));
            result = (പൂർണ്ണസംഖ്യ)v_reduce_max(acc);
            വേണ്ടി (; i < len; i++)
                എങ്കിൽ (mask[i]) result = std::max(result, cv_abs(src[i]));
        }
        അല്ലെങ്കിൽ {
            വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < len; i++)
                എങ്കിൽ (mask[i]) {
                    സ്ഥിരം schar* elem = src + i * cn;
                    വേണ്ടി (പൂർണ്ണസംഖ്യ k = 0; k < cn; k++)
                        result = std::max(result, cv_abs(elem[k]));
                }
        }
        തിരികെ result;
    }
};

template <>
ഘടന MaskedNormL1_SIMD<schar, പൂർണ്ണസംഖ്യ> {
    വരിയിൽ പൂർണ്ണസംഖ്യ operator()(സ്ഥിരം schar* src, സ്ഥിരം uchar* mask, പൂർണ്ണസംഖ്യ len, പൂർണ്ണസംഖ്യ cn) സ്ഥിരം {
        പൂർണ്ണസംഖ്യ result = 0;
        എങ്കിൽ (cn == 1) {
            സ്ഥിരം പൂർണ്ണസംഖ്യ vstep = VTraits<v_int8>::vlanes();
            സ്ഥിരം v_uint8 one = vx_setall_u8(1);
            പൂർണ്ണസംഖ്യ i = 0;
            v_uint32 acc = vx_setzero_u32();
            വേണ്ടി (; i <= len - vstep; i += vstep)
                acc = v_dotprod_expand_fast(v_and(v_abs(vx_load(src + i)), v_normmask_u8(mask + i)), one, acc);
            result = (പൂർണ്ണസംഖ്യ)v_reduce_sum(acc);
            വേണ്ടി (; i < len; i++)
                എങ്കിൽ (mask[i]) result += cv_abs(src[i]);
        }
        അല്ലെങ്കിൽ {
            വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < len; i++)
                എങ്കിൽ (mask[i]) {
                    സ്ഥിരം schar* elem = src + i * cn;
                    വേണ്ടി (പൂർണ്ണസംഖ്യ k = 0; k < cn; k++)
                        result += cv_abs(elem[k]);
                }
        }
        തിരികെ result;
    }
};

template <>
ഘടന MaskedNormL2_SIMD<schar, പൂർണ്ണസംഖ്യ> {
    വരിയിൽ പൂർണ്ണസംഖ്യ operator()(സ്ഥിരം schar* src, സ്ഥിരം uchar* mask, പൂർണ്ണസംഖ്യ len, പൂർണ്ണസംഖ്യ cn) സ്ഥിരം {
        പൂർണ്ണസംഖ്യ result = 0;
        എങ്കിൽ (cn == 1) {
            സ്ഥിരം പൂർണ്ണസംഖ്യ vstep = VTraits<v_int8>::vlanes();
            പൂർണ്ണസംഖ്യ i = 0;
            v_int32 acc = vx_setzero_s32();
            വേണ്ടി (; i <= len - vstep; i += vstep) {
                v_int8 s = v_and(vx_load(src + i), v_reinterpret_as_s8(v_normmask_u8(mask + i)));
                acc = v_dotprod_expand_fast(s, s, acc);
            }
            result = (പൂർണ്ണസംഖ്യ)v_reduce_sum(acc);
            വേണ്ടി (; i < len; i++)
                എങ്കിൽ (mask[i]) { പൂർണ്ണസംഖ്യ v = src[i]; result += v * v; }
        }
        അല്ലെങ്കിൽ {
            വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < len; i++)
                എങ്കിൽ (mask[i]) {
                    സ്ഥിരം schar* elem = src + i * cn;
                    വേണ്ടി (പൂർണ്ണസംഖ്യ k = 0; k < cn; k++) { പൂർണ്ണസംഖ്യ v = elem[k]; result += v * v; }
                }
        }
        തിരികെ result;
    }
};

template <>
ഘടന MaskedNormL2_SIMD<uchar, പൂർണ്ണസംഖ്യ> {
    വരിയിൽ പൂർണ്ണസംഖ്യ operator()(സ്ഥിരം uchar* src, സ്ഥിരം uchar* mask, പൂർണ്ണസംഖ്യ len, പൂർണ്ണസംഖ്യ cn) സ്ഥിരം {
        പൂർണ്ണസംഖ്യ result = 0;
        സ്ഥിരം പൂർണ്ണസംഖ്യ vstep = VTraits<v_uint8>::vlanes();
        എങ്കിൽ (cn == 1) {
            പൂർണ്ണസംഖ്യ i = 0;
            v_uint32 acc = vx_setzero_u32();
            വേണ്ടി (; i <= len - vstep; i += vstep) {
                v_uint8 s = v_and(vx_load(src + i), v_normmask_u8(mask + i));
                acc = v_dotprod_expand_fast(s, s, acc);
            }
            result = (പൂർണ്ണസംഖ്യ)v_reduce_sum(acc);
            വേണ്ടി (; i < len; i++)
                എങ്കിൽ (mask[i]) { പൂർണ്ണസംഖ്യ v = src[i]; result += v * v; }
        }
        അല്ലെങ്കിൽ എങ്കിൽ (cn == 4) {
            പൂർണ്ണസംഖ്യ i = 0;
            v_uint32 acc = vx_setzero_u32();
            വേണ്ടി (; i <= len - vstep; i += vstep) {
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
            result = (പൂർണ്ണസംഖ്യ)v_reduce_sum(acc);
            വേണ്ടി (; i < len; i++)
                എങ്കിൽ (mask[i]) {
                    സ്ഥിരം uchar* elem = src + i * 4;
                    വേണ്ടി (പൂർണ്ണസംഖ്യ k = 0; k < 4; k++) { പൂർണ്ണസംഖ്യ v = elem[k]; result += v * v; }
                }
        }
        അല്ലെങ്കിൽ {
            വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < len; i++)
                എങ്കിൽ (mask[i]) {
                    സ്ഥിരം uchar* elem = src + i * cn;
                    വേണ്ടി (പൂർണ്ണസംഖ്യ k = 0; k < cn; k++) { പൂർണ്ണസംഖ്യ v = elem[k]; result += v * v; }
                }
        }
        തിരികെ result;
    }
};

template <>
ഘടന MaskedNormInf_SIMD<short, പൂർണ്ണസംഖ്യ> {
    വരിയിൽ പൂർണ്ണസംഖ്യ operator()(സ്ഥിരം short* src, സ്ഥിരം uchar* mask, പൂർണ്ണസംഖ്യ len, പൂർണ്ണസംഖ്യ cn) സ്ഥിരം {
        പൂർണ്ണസംഖ്യ result = 0;
        എങ്കിൽ (cn == 1) {
            സ്ഥിരം പൂർണ്ണസംഖ്യ vstep = VTraits<v_int16>::vlanes();
            പൂർണ്ണസംഖ്യ i = 0;
            v_uint16 acc = vx_setzero_u16();
            വേണ്ടി (; i <= len - vstep; i += vstep)
                acc = v_max(acc, v_and(v_abs(vx_load(src + i)), v_normmask_u16(mask + i)));
            result = (പൂർണ്ണസംഖ്യ)v_reduce_max(acc);
            വേണ്ടി (; i < len; i++)
                എങ്കിൽ (mask[i]) result = std::max(result, cv_abs(src[i]));
        }
        അല്ലെങ്കിൽ {
            വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < len; i++)
                എങ്കിൽ (mask[i]) {
                    സ്ഥിരം short* elem = src + i * cn;
                    വേണ്ടി (പൂർണ്ണസംഖ്യ k = 0; k < cn; k++)
                        result = std::max(result, cv_abs(elem[k]));
                }
        }
        തിരികെ result;
    }
};

template <>
ഘടന MaskedNormL1_SIMD<short, പൂർണ്ണസംഖ്യ> {
    വരിയിൽ പൂർണ്ണസംഖ്യ operator()(സ്ഥിരം short* src, സ്ഥിരം uchar* mask, പൂർണ്ണസംഖ്യ len, പൂർണ്ണസംഖ്യ cn) സ്ഥിരം {
        പൂർണ്ണസംഖ്യ result = 0;
        എങ്കിൽ (cn == 1) {
            സ്ഥിരം പൂർണ്ണസംഖ്യ vstep = VTraits<v_int16>::vlanes();
            പൂർണ്ണസംഖ്യ i = 0;
            v_uint32 acc0 = vx_setzero_u32(), acc1 = vx_setzero_u32();
            വേണ്ടി (; i <= len - vstep; i += vstep) {
                v_uint16 s = v_and(v_abs(vx_load(src + i)), v_normmask_u16(mask + i));
                v_uint32 lo, hi;
                v_expand(s, lo, hi);
                acc0 = v_add(acc0, lo);
                acc1 = v_add(acc1, hi);
            }
            result = (പൂർണ്ണസംഖ്യ)v_reduce_sum(v_add(acc0, acc1));
            വേണ്ടി (; i < len; i++)
                എങ്കിൽ (mask[i]) result += cv_abs(src[i]);
        }
        അല്ലെങ്കിൽ {
            വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < len; i++)
                എങ്കിൽ (mask[i]) {
                    സ്ഥിരം short* elem = src + i * cn;
                    വേണ്ടി (പൂർണ്ണസംഖ്യ k = 0; k < cn; k++)
                        result += cv_abs(elem[k]);
                }
        }
        തിരികെ result;
    }
};

template <>
ഘടന MaskedNormL2_SIMD<short, ഇരട്ട> {
    വരിയിൽ ഇരട്ട operator()(സ്ഥിരം short* src, സ്ഥിരം uchar* mask, പൂർണ്ണസംഖ്യ len, പൂർണ്ണസംഖ്യ cn) സ്ഥിരം {
        ഇരട്ട result = 0.0;
        എങ്കിൽ (cn == 1) {
            സ്ഥിരം പൂർണ്ണസംഖ്യ vstep = VTraits<v_int16>::vlanes();
            പൂർണ്ണസംഖ്യ i = 0;
            v_uint64 acc = vx_setzero_u64();
            വേണ്ടി (; i <= len - vstep; i += vstep) {
                v_uint16 s = v_and(v_abs(vx_load(src + i)), v_normmask_u16(mask + i));
                acc = v_add(acc, v_dotprod_expand_fast(s, s));
            }
            result = (ഇരട്ട)v_reduce_sum(acc);
            വേണ്ടി (; i < len; i++)
                എങ്കിൽ (mask[i]) { ഇരട്ട v = src[i]; result += v * v; }
        }
        അല്ലെങ്കിൽ {
            വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < len; i++)
                എങ്കിൽ (mask[i]) {
                    സ്ഥിരം short* elem = src + i * cn;
                    വേണ്ടി (പൂർണ്ണസംഖ്യ k = 0; k < cn; k++) { ഇരട്ട v = elem[k]; result += v * v; }
                }
        }
        തിരികെ result;
    }
};

template <>
ഘടന MaskedNormInf_SIMD<പൂർണ്ണസംഖ്യ, unsigned> {
    വരിയിൽ unsigned operator()(സ്ഥിരം പൂർണ്ണസംഖ്യ* src, സ്ഥിരം uchar* mask, പൂർണ്ണസംഖ്യ len, പൂർണ്ണസംഖ്യ cn) സ്ഥിരം {
        unsigned result = 0;
        എങ്കിൽ (cn == 1) {
            സ്ഥിരം പൂർണ്ണസംഖ്യ vstep = VTraits<v_int32>::vlanes();
            പൂർണ്ണസംഖ്യ i = 0;
            v_uint32 acc = vx_setzero_u32();
            വേണ്ടി (; i <= len - vstep; i += vstep)
                acc = v_max(acc, v_and(v_abs(vx_load(src + i)), v_normmask_u32(mask + i)));
            result = v_reduce_max(acc);
            വേണ്ടി (; i < len; i++)
                എങ്കിൽ (mask[i]) result = std::max(result, cv_abs(src[i]));
        }
        അല്ലെങ്കിൽ {
            വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < len; i++)
                എങ്കിൽ (mask[i]) {
                    സ്ഥിരം പൂർണ്ണസംഖ്യ* elem = src + i * cn;
                    വേണ്ടി (പൂർണ്ണസംഖ്യ k = 0; k < cn; k++)
                        result = std::max(result, cv_abs(elem[k]));
                }
        }
        തിരികെ result;
    }
};

#എങ്കിൽ CV_SIMD_64F || CV_SIMD_SCALABLE_64F

template <>
ഘടന MaskedNormL1_SIMD<പൂർണ്ണസംഖ്യ, ഇരട്ട> {
    വരിയിൽ ഇരട്ട operator()(സ്ഥിരം പൂർണ്ണസംഖ്യ* src, സ്ഥിരം uchar* mask, പൂർണ്ണസംഖ്യ len, പൂർണ്ണസംഖ്യ cn) സ്ഥിരം {
        ഇരട്ട result = 0.0;
        എങ്കിൽ (cn == 1) {
            സ്ഥിരം പൂർണ്ണസംഖ്യ vstep = VTraits<v_int32>::vlanes();
            പൂർണ്ണസംഖ്യ i = 0;
            v_float64 acc0 = vx_setzero_f64(), acc1 = vx_setzero_f64();
            വേണ്ടി (; i <= len - vstep; i += vstep) {
                // Mask the value, then widen and take the magnitude in double. v_abs() does give
                // |INT_MIN| as an unsigned 2147483648, but reinterpreting that back as int32 turns
                // it negative again, and the widening then carries the sign into the accumulator.
                v_int32 s = v_reinterpret_as_s32(v_and(v_reinterpret_as_u32(vx_load(src + i)),
                                                       v_normmask_u32(mask + i)));
                acc0 = v_add(acc0, v_abs(v_cvt_f64(s)));
                acc1 = v_add(acc1, v_abs(v_cvt_f64_മുകളിലെ(s)));
            }
            result = v_reduce_sum(v_add(acc0, acc1));
            വേണ്ടി (; i < len; i++)
                എങ്കിൽ (mask[i]) result += cv_abs(src[i]);
        }
        അല്ലെങ്കിൽ {
            വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < len; i++)
                എങ്കിൽ (mask[i]) {
                    സ്ഥിരം പൂർണ്ണസംഖ്യ* elem = src + i * cn;
                    വേണ്ടി (പൂർണ്ണസംഖ്യ k = 0; k < cn; k++)
                        result += cv_abs(elem[k]);
                }
        }
        തിരികെ result;
    }
};

template <>
ഘടന MaskedNormL2_SIMD<പൂർണ്ണസംഖ്യ, ഇരട്ട> {
    വരിയിൽ ഇരട്ട operator()(സ്ഥിരം പൂർണ്ണസംഖ്യ* src, സ്ഥിരം uchar* mask, പൂർണ്ണസംഖ്യ len, പൂർണ്ണസംഖ്യ cn) സ്ഥിരം {
        ഇരട്ട result = 0.0;
        എങ്കിൽ (cn == 1) {
            സ്ഥിരം പൂർണ്ണസംഖ്യ vstep = VTraits<v_int32>::vlanes();
            പൂർണ്ണസംഖ്യ i = 0;
            v_float64 acc0 = vx_setzero_f64(), acc1 = vx_setzero_f64();
            വേണ്ടി (; i <= len - vstep; i += vstep) {
                v_uint32 s = v_and(v_abs(vx_load(src + i)), v_normmask_u32(mask + i));
                v_uint64 e0, e1;
                v_expand(s, e0, e1);
                v_float64 f0 = v_cvt_f64(v_reinterpret_as_s64(e0));
                v_float64 f1 = v_cvt_f64(v_reinterpret_as_s64(e1));
                acc0 = v_fma(f0, f0, acc0);
                acc1 = v_fma(f1, f1, acc1);
            }
            result = v_reduce_sum(v_add(acc0, acc1));
            വേണ്ടി (; i < len; i++)
                എങ്കിൽ (mask[i]) { ഇരട്ട v = src[i]; result += v * v; }
        }
        അല്ലെങ്കിൽ {
            വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < len; i++)
                എങ്കിൽ (mask[i]) {
                    സ്ഥിരം പൂർണ്ണസംഖ്യ* elem = src + i * cn;
                    വേണ്ടി (പൂർണ്ണസംഖ്യ k = 0; k < cn; k++) { ഇരട്ട v = elem[k]; result += v * v; }
                }
        }
        തിരികെ result;
    }
};

template <>
ഘടന MaskedNormInf_SIMD<ഇരട്ട, ഇരട്ട> {
    വരിയിൽ ഇരട്ട operator()(സ്ഥിരം ഇരട്ട* src, സ്ഥിരം uchar* mask, പൂർണ്ണസംഖ്യ len, പൂർണ്ണസംഖ്യ cn) സ്ഥിരം {
        ഇരട്ട result = 0.0;
        എങ്കിൽ (cn == 1) {
            സ്ഥിരം പൂർണ്ണസംഖ്യ vstep = VTraits<v_float64>::vlanes();
            പൂർണ്ണസംഖ്യ i = 0;
            v_float64 acc0 = vx_setzero_f64(), acc1 = vx_setzero_f64();
            വേണ്ടി (; i <= len - 2*vstep; i += 2*vstep) {
                v_uint64 m0, m1;
                v_normmask_u64(mask + i, m0, m1);
                acc0 = v_max(acc0, v_reinterpret_as_f64(v_and(v_reinterpret_as_u64(v_abs(vx_load(src + i))), m0)));
                acc1 = v_max(acc1, v_reinterpret_as_f64(v_and(v_reinterpret_as_u64(v_abs(vx_load(src + i + vstep))), m1)));
            }
            ഇരട്ട t[VTraits<v_float64>::max_nlanes];
            vx_store(t, v_max(acc0, acc1));
            വേണ്ടി (പൂർണ്ണസംഖ്യ k = 0; k < vstep; k++)
                result = std::max(result, t[k]);
            വേണ്ടി (; i < len; i++)
                എങ്കിൽ (mask[i]) result = std::max(result, cv_abs(src[i]));
        }
        അല്ലെങ്കിൽ {
            വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < len; i++)
                എങ്കിൽ (mask[i]) {
                    സ്ഥിരം ഇരട്ട* elem = src + i * cn;
                    വേണ്ടി (പൂർണ്ണസംഖ്യ k = 0; k < cn; k++)
                        result = std::max(result, cv_abs(elem[k]));
                }
        }
        തിരികെ result;
    }
};

template <>
ഘടന MaskedNormL1_SIMD<ഇരട്ട, ഇരട്ട> {
    വരിയിൽ ഇരട്ട operator()(സ്ഥിരം ഇരട്ട* src, സ്ഥിരം uchar* mask, പൂർണ്ണസംഖ്യ len, പൂർണ്ണസംഖ്യ cn) സ്ഥിരം {
        ഇരട്ട result = 0.0;
        എങ്കിൽ (cn == 1) {
            സ്ഥിരം പൂർണ്ണസംഖ്യ vstep = VTraits<v_float64>::vlanes();
            പൂർണ്ണസംഖ്യ i = 0;
            v_float64 acc0 = vx_setzero_f64(), acc1 = vx_setzero_f64();
            വേണ്ടി (; i <= len - 2*vstep; i += 2*vstep) {
                v_uint64 m0, m1;
                v_normmask_u64(mask + i, m0, m1);
                acc0 = v_add(acc0, v_reinterpret_as_f64(v_and(v_reinterpret_as_u64(v_abs(vx_load(src + i))), m0)));
                acc1 = v_add(acc1, v_reinterpret_as_f64(v_and(v_reinterpret_as_u64(v_abs(vx_load(src + i + vstep))), m1)));
            }
            result = v_reduce_sum(v_add(acc0, acc1));
            വേണ്ടി (; i < len; i++)
                എങ്കിൽ (mask[i]) result += cv_abs(src[i]);
        }
        അല്ലെങ്കിൽ {
            വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < len; i++)
                എങ്കിൽ (mask[i]) {
                    സ്ഥിരം ഇരട്ട* elem = src + i * cn;
                    വേണ്ടി (പൂർണ്ണസംഖ്യ k = 0; k < cn; k++)
                        result += cv_abs(elem[k]);
                }
        }
        തിരികെ result;
    }
};

template <>
ഘടന MaskedNormL2_SIMD<ഇരട്ട, ഇരട്ട> {
    വരിയിൽ ഇരട്ട operator()(സ്ഥിരം ഇരട്ട* src, സ്ഥിരം uchar* mask, പൂർണ്ണസംഖ്യ len, പൂർണ്ണസംഖ്യ cn) സ്ഥിരം {
        ഇരട്ട result = 0.0;
        എങ്കിൽ (cn == 1) {
            സ്ഥിരം പൂർണ്ണസംഖ്യ vstep = VTraits<v_float64>::vlanes();
            പൂർണ്ണസംഖ്യ i = 0;
            v_float64 acc0 = vx_setzero_f64(), acc1 = vx_setzero_f64();
            വേണ്ടി (; i <= len - 2*vstep; i += 2*vstep) {
                v_uint64 m0, m1;
                v_normmask_u64(mask + i, m0, m1);
                v_float64 s0 = v_reinterpret_as_f64(v_and(v_reinterpret_as_u64(vx_load(src + i)), m0));
                v_float64 s1 = v_reinterpret_as_f64(v_and(v_reinterpret_as_u64(vx_load(src + i + vstep)), m1));
                acc0 = v_fma(s0, s0, acc0);
                acc1 = v_fma(s1, s1, acc1);
            }
            result = v_reduce_sum(v_add(acc0, acc1));
            വേണ്ടി (; i < len; i++)
                എങ്കിൽ (mask[i]) { ഇരട്ട v = src[i]; result += v * v; }
        }
        അല്ലെങ്കിൽ {
            വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < len; i++)
                എങ്കിൽ (mask[i]) {
                    സ്ഥിരം ഇരട്ട* elem = src + i * cn;
                    വേണ്ടി (പൂർണ്ണസംഖ്യ k = 0; k < cn; k++) { ഇരട്ട v = elem[k]; result += v * v; }
                }
        }
        തിരികെ result;
    }
};

#endif // CV_SIMD_64F || CV_SIMD_SCALABLE_64F
#endif // CV_SIMD || CV_SIMD_SCALABLE

template<typename T, typename ST> പൂർണ്ണസംഖ്യ
normInf_(സ്ഥിരം T* src, സ്ഥിരം uchar* mask, ST* _result, പൂർണ്ണസംഖ്യ len, പൂർണ്ണസംഖ്യ cn)
{
    ST result = *_result;
    എങ്കിൽ( !mask )
    {
        NormInf_SIMD<T, ST> op;
        result = std::max(result, op(src, len*cn));
    } അല്ലെങ്കിൽ {
        MaskedNormInf_SIMD<T, ST> op;
        result = std::max(result, op(src, mask, len, cn));
    }
    *_result = result;
    തിരികെ 0;
}

template<typename T, typename ST> പൂർണ്ണസംഖ്യ
normL1_(സ്ഥിരം T* src, സ്ഥിരം uchar* mask, ST* _result, പൂർണ്ണസംഖ്യ len, പൂർണ്ണസംഖ്യ cn)
{
    ST result = *_result;
    എങ്കിൽ( !mask )
    {
        NormL1_SIMD<T, ST> op;
        result += op(src, len*cn);
    } അല്ലെങ്കിൽ {
        MaskedNormL1_SIMD<T, ST> op;
        result += op(src, mask, len, cn);
    }
    *_result = result;
    തിരികെ 0;
}

template<typename T, typename ST> പൂർണ്ണസംഖ്യ
normL2_(സ്ഥിരം T* src, സ്ഥിരം uchar* mask, ST* _result, പൂർണ്ണസംഖ്യ len, പൂർണ്ണസംഖ്യ cn)
{
    ST result = *_result;
    എങ്കിൽ( !mask )
    {
        NormL2_SIMD<T, ST> op;
        result += op(src, len*cn);
    }
    അല്ലെങ്കിൽ
    {
        MaskedNormL2_SIMD<T, ST> op;
        result += op(src, mask, len, cn);
    }
    *_result = result;
    തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ
normInf_Bool(സ്ഥിരം uchar* src, സ്ഥിരം uchar* mask, പൂർണ്ണസംഖ്യ* _result, പൂർണ്ണസംഖ്യ len, പൂർണ്ണസംഖ്യ cn)
{
    പൂർണ്ണസംഖ്യ result = *_result;
    എങ്കിൽ( !mask )
    {
        വേണ്ടി ( പൂർണ്ണസംഖ്യ i = 0; i < len*cn; i++ ) {
            result = std::max(result, (പൂർണ്ണസംഖ്യ)(src[i] != 0));
            എങ്കിൽ (result != 0)
                നിർത്തുക;
        }
    }
    അല്ലെങ്കിൽ
    {
        വേണ്ടി( പൂർണ്ണസംഖ്യ i = 0; i < len; i++, src += cn )
            എങ്കിൽ( mask[i] )
            {
                വേണ്ടി( പൂർണ്ണസംഖ്യ k = 0; k < cn; k++ )
                    result = std::max(result, (പൂർണ്ണസംഖ്യ)(src[k] != 0));
                എങ്കിൽ (result != 0)
                    നിർത്തുക;
            }
    }
    *_result = result;
    തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ
normL1_Bool(സ്ഥിരം uchar* src, സ്ഥിരം uchar* mask, പൂർണ്ണസംഖ്യ* _result, പൂർണ്ണസംഖ്യ len, പൂർണ്ണസംഖ്യ cn)
{
    പൂർണ്ണസംഖ്യ result = *_result;
    എങ്കിൽ( !mask )
    {
        വേണ്ടി ( പൂർണ്ണസംഖ്യ i = 0; i < len*cn; i++ )
            result += (പൂർണ്ണസംഖ്യ)(src[i] != 0);
    }
    അല്ലെങ്കിൽ
    {
        വേണ്ടി( പൂർണ്ണസംഖ്യ i = 0; i < len; i++, src += cn )
            എങ്കിൽ( mask[i] )
            {
                വേണ്ടി( പൂർണ്ണസംഖ്യ k = 0; k < cn; k++ )
                    result += (പൂർണ്ണസംഖ്യ)(src[k] != 0);
            }
    }
    *_result = result;
    തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ
normL2_Bool(സ്ഥിരം uchar* src, സ്ഥിരം uchar* mask, പൂർണ്ണസംഖ്യ* _result, പൂർണ്ണസംഖ്യ len, പൂർണ്ണസംഖ്യ cn)
{
    തിരികെ normL1_Bool(src, mask, _result, len, cn);
}

// ====================================================================
// Masked norm-diff kernels.
// Scalar base templates (safe for every depth) plus SIMD cn==1
// specializations for the concrete (T, ST) pairs dispatched by
// CV_DEF_NORM_ALL. The masked normDiff*_ functions below select them.
// ====================================================================
template <typename T, typename ST>
ഘടന MaskedNormDiffInf_SIMD {
    വരിയിൽ ST operator()(സ്ഥിരം T* s1, സ്ഥിരം T* s2, സ്ഥിരം uchar* mask, പൂർണ്ണസംഖ്യ len, പൂർണ്ണസംഖ്യ cn) സ്ഥിരം {
        ST result = 0;
        വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < len; i++) എങ്കിൽ (mask[i]) {
            സ്ഥിരം T* e1 = s1 + i*cn; സ്ഥിരം T* e2 = s2 + i*cn;
            വേണ്ടി (പൂർണ്ണസംഖ്യ k = 0; k < cn; k++)
                result = std::max(result, (ST)cv_absdiff(e1[k], e2[k]));
        }
        തിരികെ result;
    }
};
template <typename T, typename ST>
ഘടന MaskedNormDiffL1_SIMD {
    വരിയിൽ ST operator()(സ്ഥിരം T* s1, സ്ഥിരം T* s2, സ്ഥിരം uchar* mask, പൂർണ്ണസംഖ്യ len, പൂർണ്ണസംഖ്യ cn) സ്ഥിരം {
        ST result = 0;
        വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < len; i++) എങ്കിൽ (mask[i]) {
            സ്ഥിരം T* e1 = s1 + i*cn; സ്ഥിരം T* e2 = s2 + i*cn;
            വേണ്ടി (പൂർണ്ണസംഖ്യ k = 0; k < cn; k++)
                result += (ST)cv_absdiff(e1[k], e2[k]);
        }
        തിരികെ result;
    }
};
template <typename T, typename ST>
ഘടന MaskedNormDiffL2_SIMD {
    വരിയിൽ ST operator()(സ്ഥിരം T* s1, സ്ഥിരം T* s2, സ്ഥിരം uchar* mask, പൂർണ്ണസംഖ്യ len, പൂർണ്ണസംഖ്യ cn) സ്ഥിരം {
        ST result = 0;
        വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < len; i++) എങ്കിൽ (mask[i]) {
            സ്ഥിരം T* e1 = s1 + i*cn; സ്ഥിരം T* e2 = s2 + i*cn;
            വേണ്ടി (പൂർണ്ണസംഖ്യ k = 0; k < cn; k++) { ST v = (ST)e1[k] - (ST)e2[k]; result += v*v; }
        }
        തിരികെ result;
    }
};

#എങ്കിൽ (CV_SIMD || CV_SIMD_SCALABLE)
// AND a per-lane diff vector with the (expanded) mask>0 predicate.
വരിയിൽ v_uint8  v_diffmask(സ്ഥിരം v_uint8&  d, സ്ഥിരം uchar* m) { തിരികെ v_and(d, v_gt(vx_load(m),          vx_setzero_u8())); }
വരിയിൽ v_uint16 v_diffmask(സ്ഥിരം v_uint16& d, സ്ഥിരം uchar* m) { തിരികെ v_and(d, v_gt(vx_load_expand(m),   vx_setzero_u16())); }
വരിയിൽ v_uint32 v_diffmask(സ്ഥിരം v_uint32& d, സ്ഥിരം uchar* m) { തിരികെ v_and(d, v_gt(vx_load_expand_q(m), vx_setzero_u32())); }
വരിയിൽ v_float32 v_diffmask(സ്ഥിരം v_float32& d, സ്ഥിരം uchar* m) {
    v_uint32 cm = v_gt(vx_load_expand_q(m), vx_setzero_u32());
    തിരികെ v_reinterpret_as_f32(v_and(v_reinterpret_as_u32(d), cm));
}

// Shared scalar cn>1 fallbacks (match the base templates' cv_absdiff semantics).
template<typename T, typename RT>
സ്ഥിതം വരിയിൽ RT maskedNormDiffInfTail(സ്ഥിരം T* s1, സ്ഥിരം T* s2, സ്ഥിരം uchar* mask, പൂർണ്ണസംഖ്യ len, പൂർണ്ണസംഖ്യ cn, RT result) {
    വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < len; i++) എങ്കിൽ (mask[i]) {
        സ്ഥിരം T* e1 = s1 + i*cn; സ്ഥിരം T* e2 = s2 + i*cn;
        വേണ്ടി (പൂർണ്ണസംഖ്യ k = 0; k < cn; k++) result = std::max(result, (RT)cv_absdiff(e1[k], e2[k]));
    }
    തിരികെ result;
}
template<typename T, typename RT>
സ്ഥിതം വരിയിൽ RT maskedNormDiffL1Tail(സ്ഥിരം T* s1, സ്ഥിരം T* s2, സ്ഥിരം uchar* mask, പൂർണ്ണസംഖ്യ len, പൂർണ്ണസംഖ്യ cn, RT result) {
    വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < len; i++) എങ്കിൽ (mask[i]) {
        സ്ഥിരം T* e1 = s1 + i*cn; സ്ഥിരം T* e2 = s2 + i*cn;
        വേണ്ടി (പൂർണ്ണസംഖ്യ k = 0; k < cn; k++) result += (RT)cv_absdiff(e1[k], e2[k]);
    }
    തിരികെ result;
}
template<typename T, typename RT>
സ്ഥിതം വരിയിൽ RT maskedNormDiffL2Tail(സ്ഥിരം T* s1, സ്ഥിരം T* s2, സ്ഥിരം uchar* mask, പൂർണ്ണസംഖ്യ len, പൂർണ്ണസംഖ്യ cn, RT result) {
    വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < len; i++) എങ്കിൽ (mask[i]) {
        സ്ഥിരം T* e1 = s1 + i*cn; സ്ഥിരം T* e2 = s2 + i*cn;
        വേണ്ടി (പൂർണ്ണസംഖ്യ k = 0; k < cn; k++) { RT v = (RT)e1[k] - (RT)e2[k]; result += v*v; }
    }
    തിരികെ result;
}

// Inf (cn==1): reduce_max of masked |a-b|. Shared across SIMD-capable depths.
template<typename T, typename ST>
സ്ഥിതം വരിയിൽ ST maskedNormDiffInfCn1(സ്ഥിരം T* s1, സ്ഥിരം T* s2, സ്ഥിരം uchar* mask, പൂർണ്ണസംഖ്യ len) {
    സ്ഥിരം പൂർണ്ണസംഖ്യ vstep = VTraits<decltype(vx_load(s1))>::vlanes();
    ST result = 0;
    പൂർണ്ണസംഖ്യ i = 0;
    എങ്കിൽ (len >= vstep) {
        auto acc = v_diffmask(v_absdiff(vx_load(s1), vx_load(s2)), mask);
        വേണ്ടി (i = vstep; i <= len - vstep; i += vstep)
            acc = v_max(acc, v_diffmask(v_absdiff(vx_load(s1 + i), vx_load(s2 + i)), mask + i));
        result = (ST)v_reduce_max(acc);
    }
    വേണ്ടി (; i < len; i++) എങ്കിൽ (mask[i]) result = std::max(result, (ST)cv_absdiff(s1[i], s2[i]));
    തിരികെ result;
}

// Shared 8-bit cn==1 masked L1/L2 kernels (uchar/schar): v_absdiff yields v_uint8 for both.
template<typename T>
സ്ഥിതം വരിയിൽ പൂർണ്ണസംഖ്യ maskedNormDiffL1_8(സ്ഥിരം T* s1, സ്ഥിരം T* s2, സ്ഥിരം uchar* mask, പൂർണ്ണസംഖ്യ len) {
    പൂർണ്ണസംഖ്യ i = 0; സ്ഥിരം പൂർണ്ണസംഖ്യ vstep = VTraits<v_uint8>::vlanes();
    സ്ഥിരം v_uint8 one = vx_setall_u8(1);
    v_uint32 acc = vx_setzero_u32();
    വേണ്ടി (; i <= len - vstep; i += vstep) {
        v_uint8 m  = v_gt(vx_load(mask + i), vx_setzero_u8());
        v_uint8 ad = v_and(v_absdiff(vx_load(s1 + i), vx_load(s2 + i)), m);
        acc = v_dotprod_expand_fast(ad, one, acc);
    }
    പൂർണ്ണസംഖ്യ result = (പൂർണ്ണസംഖ്യ)v_reduce_sum(acc);
    വേണ്ടി (; i < len; i++) എങ്കിൽ (mask[i]) result += (പൂർണ്ണസംഖ്യ)cv_absdiff(s1[i], s2[i]);
    തിരികെ result;
}
template<typename T>
സ്ഥിതം വരിയിൽ പൂർണ്ണസംഖ്യ maskedNormDiffL2_8(സ്ഥിരം T* s1, സ്ഥിരം T* s2, സ്ഥിരം uchar* mask, പൂർണ്ണസംഖ്യ len) {
    പൂർണ്ണസംഖ്യ i = 0; സ്ഥിരം പൂർണ്ണസംഖ്യ vstep = VTraits<v_uint8>::vlanes();
    v_uint32 acc = vx_setzero_u32();
    വേണ്ടി (; i <= len - vstep; i += vstep) {
        v_uint8 m  = v_gt(vx_load(mask + i), vx_setzero_u8());
        v_uint8 ad = v_and(v_absdiff(vx_load(s1 + i), vx_load(s2 + i)), m);
        acc = v_dotprod_expand_fast(ad, ad, acc);
    }
    പൂർണ്ണസംഖ്യ result = (പൂർണ്ണസംഖ്യ)v_reduce_sum(acc);
    വേണ്ടി (; i < len; i++) എങ്കിൽ (mask[i]) { പൂർണ്ണസംഖ്യ v = (പൂർണ്ണസംഖ്യ)s1[i] - (പൂർണ്ണസംഖ്യ)s2[i]; result += v*v; }
    തിരികെ result;
}

// Shared 16-bit cn==1 masked L1 kernel (ushort/short): v_absdiff yields v_uint16 for both.
template<typename T>
സ്ഥിതം വരിയിൽ പൂർണ്ണസംഖ്യ maskedNormDiffL1_16(സ്ഥിരം T* s1, സ്ഥിരം T* s2, സ്ഥിരം uchar* mask, പൂർണ്ണസംഖ്യ len) {
    പൂർണ്ണസംഖ്യ i = 0; സ്ഥിരം പൂർണ്ണസംഖ്യ vstep = VTraits<v_uint16>::vlanes();
    v_uint32 acc = vx_setzero_u32();
    വേണ്ടി (; i <= len - vstep; i += vstep) {
        v_uint16 m  = v_gt(vx_load_expand(mask + i), vx_setzero_u16());
        v_uint16 ad = v_and(v_absdiff(vx_load(s1 + i), vx_load(s2 + i)), m);
        v_uint32 lo, hi; v_expand(ad, lo, hi);
        acc = v_add(acc, v_add(lo, hi));
    }
    പൂർണ്ണസംഖ്യ result = (പൂർണ്ണസംഖ്യ)v_reduce_sum(acc);
    വേണ്ടി (; i < len; i++) എങ്കിൽ (mask[i]) result += (പൂർണ്ണസംഖ്യ)cv_absdiff(s1[i], s2[i]);
    തിരികെ result;
}

template<> ഘടന MaskedNormDiffInf_SIMD<uchar, പൂർണ്ണസംഖ്യ> {
    പൂർണ്ണസംഖ്യ operator()(സ്ഥിരം uchar* s1, സ്ഥിരം uchar* s2, സ്ഥിരം uchar* mask, പൂർണ്ണസംഖ്യ len, പൂർണ്ണസംഖ്യ cn) സ്ഥിരം {
        എങ്കിൽ (cn == 1) തിരികെ maskedNormDiffInfCn1<uchar, പൂർണ്ണസംഖ്യ>(s1, s2, mask, len);
        എങ്കിൽ (cn == 4 && len >= VTraits<v_uint8>::vlanes()) {
            സ്ഥിരം പൂർണ്ണസംഖ്യ vstep = VTraits<v_uint8>::vlanes();
            v_uint8 acc = vx_setzero_u8();
            പൂർണ്ണസംഖ്യ i = 0;
            വേണ്ടി (;;) {
                എങ്കിൽ (i > len - vstep) i = len - vstep; // back-step (max is idempotent)
                v_uint8 a0,a1,a2,a3,b0,b1,b2,b3;
                v_load_deinterleave(s1 + i*4, a0,a1,a2,a3);
                v_load_deinterleave(s2 + i*4, b0,b1,b2,b3);
                v_uint8 ad = v_max(v_max(v_absdiff(a0,b0), v_absdiff(a1,b1)),
                                   v_max(v_absdiff(a2,b2), v_absdiff(a3,b3)));
                v_uint8 m = v_gt(vx_load(mask + i), vx_setzero_u8());
                acc = v_max(acc, v_and(ad, m));
                എങ്കിൽ (i >= len - vstep) നിർത്തുക;
                i += vstep;
            }
            തിരികെ (പൂർണ്ണസംഖ്യ)v_reduce_max(acc);
        }
        തിരികെ maskedNormDiffInfTail<uchar, പൂർണ്ണസംഖ്യ>(s1, s2, mask, len, cn, 0);
    }
};
template<> ഘടന MaskedNormDiffInf_SIMD<schar, പൂർണ്ണസംഖ്യ> {
    പൂർണ്ണസംഖ്യ operator()(സ്ഥിരം schar* s1, സ്ഥിരം schar* s2, സ്ഥിരം uchar* mask, പൂർണ്ണസംഖ്യ len, പൂർണ്ണസംഖ്യ cn) സ്ഥിരം {
        എങ്കിൽ (cn == 1) തിരികെ maskedNormDiffInfCn1<schar, പൂർണ്ണസംഖ്യ>(s1, s2, mask, len);
        തിരികെ maskedNormDiffInfTail<schar, പൂർണ്ണസംഖ്യ>(s1, s2, mask, len, cn, 0);
    }
};
template<> ഘടന MaskedNormDiffInf_SIMD<ushort, പൂർണ്ണസംഖ്യ> {
    പൂർണ്ണസംഖ്യ operator()(സ്ഥിരം ushort* s1, സ്ഥിരം ushort* s2, സ്ഥിരം uchar* mask, പൂർണ്ണസംഖ്യ len, പൂർണ്ണസംഖ്യ cn) സ്ഥിരം {
        എങ്കിൽ (cn == 1) തിരികെ maskedNormDiffInfCn1<ushort, പൂർണ്ണസംഖ്യ>(s1, s2, mask, len);
        തിരികെ maskedNormDiffInfTail<ushort, പൂർണ്ണസംഖ്യ>(s1, s2, mask, len, cn, 0);
    }
};
template<> ഘടന MaskedNormDiffInf_SIMD<short, പൂർണ്ണസംഖ്യ> {
    പൂർണ്ണസംഖ്യ operator()(സ്ഥിരം short* s1, സ്ഥിരം short* s2, സ്ഥിരം uchar* mask, പൂർണ്ണസംഖ്യ len, പൂർണ്ണസംഖ്യ cn) സ്ഥിരം {
        എങ്കിൽ (cn == 1) തിരികെ maskedNormDiffInfCn1<short, പൂർണ്ണസംഖ്യ>(s1, s2, mask, len);
        തിരികെ maskedNormDiffInfTail<short, പൂർണ്ണസംഖ്യ>(s1, s2, mask, len, cn, 0);
    }
};
template<> ഘടന MaskedNormDiffInf_SIMD<പൂർണ്ണസംഖ്യ, unsigned> {
    unsigned operator()(സ്ഥിരം പൂർണ്ണസംഖ്യ* s1, സ്ഥിരം പൂർണ്ണസംഖ്യ* s2, സ്ഥിരം uchar* mask, പൂർണ്ണസംഖ്യ len, പൂർണ്ണസംഖ്യ cn) സ്ഥിരം {
        എങ്കിൽ (cn == 1) തിരികെ maskedNormDiffInfCn1<പൂർണ്ണസംഖ്യ, unsigned>(s1, s2, mask, len);
        തിരികെ maskedNormDiffInfTail<പൂർണ്ണസംഖ്യ, unsigned>(s1, s2, mask, len, cn, 0u);
    }
};
template<> ഘടന MaskedNormDiffInf_SIMD<ദശാംശം, ദശാംശം> {
    ദശാംശം operator()(സ്ഥിരം ദശാംശം* s1, സ്ഥിരം ദശാംശം* s2, സ്ഥിരം uchar* mask, പൂർണ്ണസംഖ്യ len, പൂർണ്ണസംഖ്യ cn) സ്ഥിരം {
        എങ്കിൽ (cn == 1) തിരികെ maskedNormDiffInfCn1<ദശാംശം, ദശാംശം>(s1, s2, mask, len);
        തിരികെ maskedNormDiffInfTail<ദശാംശം, ദശാംശം>(s1, s2, mask, len, cn, 0.0f);
    }
};

template<> ഘടന MaskedNormDiffL1_SIMD<uchar, പൂർണ്ണസംഖ്യ> {
    പൂർണ്ണസംഖ്യ operator()(സ്ഥിരം uchar* s1, സ്ഥിരം uchar* s2, സ്ഥിരം uchar* mask, പൂർണ്ണസംഖ്യ len, പൂർണ്ണസംഖ്യ cn) സ്ഥിരം {
        എങ്കിൽ (cn == 1) തിരികെ maskedNormDiffL1_8(s1, s2, mask, len);
        എങ്കിൽ (cn == 4) {
            പൂർണ്ണസംഖ്യ i = 0; സ്ഥിരം പൂർണ്ണസംഖ്യ vstep = VTraits<v_uint8>::vlanes();
            സ്ഥിരം v_uint8 one = vx_setall_u8(1);
            v_uint32 acc = vx_setzero_u32();
            വേണ്ടി (; i <= len - vstep; i += vstep) {
                v_uint8 a0,a1,a2,a3,b0,b1,b2,b3;
                v_load_deinterleave(s1 + i*4, a0,a1,a2,a3);
                v_load_deinterleave(s2 + i*4, b0,b1,b2,b3);
                v_uint8 m = v_gt(vx_load(mask + i), vx_setzero_u8());
                acc = v_dotprod_expand_fast(v_and(v_absdiff(a0,b0), m), one, acc);
                acc = v_dotprod_expand_fast(v_and(v_absdiff(a1,b1), m), one, acc);
                acc = v_dotprod_expand_fast(v_and(v_absdiff(a2,b2), m), one, acc);
                acc = v_dotprod_expand_fast(v_and(v_absdiff(a3,b3), m), one, acc);
            }
            പൂർണ്ണസംഖ്യ result = (പൂർണ്ണസംഖ്യ)v_reduce_sum(acc);
            വേണ്ടി (; i < len; i++) എങ്കിൽ (mask[i]) {
                സ്ഥിരം uchar* e1 = s1 + i*4; സ്ഥിരം uchar* e2 = s2 + i*4;
                വേണ്ടി (പൂർണ്ണസംഖ്യ k = 0; k < 4; k++) result += (പൂർണ്ണസംഖ്യ)cv_absdiff(e1[k], e2[k]);
            }
            തിരികെ result;
        }
        തിരികെ maskedNormDiffL1Tail<uchar, പൂർണ്ണസംഖ്യ>(s1, s2, mask, len, cn, 0);
    }
};
template<> ഘടന MaskedNormDiffL1_SIMD<schar, പൂർണ്ണസംഖ്യ> {
    പൂർണ്ണസംഖ്യ operator()(സ്ഥിരം schar* s1, സ്ഥിരം schar* s2, സ്ഥിരം uchar* mask, പൂർണ്ണസംഖ്യ len, പൂർണ്ണസംഖ്യ cn) സ്ഥിരം {
        എങ്കിൽ (cn == 1) തിരികെ maskedNormDiffL1_8(s1, s2, mask, len);
        തിരികെ maskedNormDiffL1Tail<schar, പൂർണ്ണസംഖ്യ>(s1, s2, mask, len, cn, 0);
    }
};
template<> ഘടന MaskedNormDiffL1_SIMD<ushort, പൂർണ്ണസംഖ്യ> {
    പൂർണ്ണസംഖ്യ operator()(സ്ഥിരം ushort* s1, സ്ഥിരം ushort* s2, സ്ഥിരം uchar* mask, പൂർണ്ണസംഖ്യ len, പൂർണ്ണസംഖ്യ cn) സ്ഥിരം {
        എങ്കിൽ (cn == 1) തിരികെ maskedNormDiffL1_16(s1, s2, mask, len);
        തിരികെ maskedNormDiffL1Tail<ushort, പൂർണ്ണസംഖ്യ>(s1, s2, mask, len, cn, 0);
    }
};
template<> ഘടന MaskedNormDiffL1_SIMD<short, പൂർണ്ണസംഖ്യ> {
    പൂർണ്ണസംഖ്യ operator()(സ്ഥിരം short* s1, സ്ഥിരം short* s2, സ്ഥിരം uchar* mask, പൂർണ്ണസംഖ്യ len, പൂർണ്ണസംഖ്യ cn) സ്ഥിരം {
        എങ്കിൽ (cn == 1) തിരികെ maskedNormDiffL1_16(s1, s2, mask, len);
        തിരികെ maskedNormDiffL1Tail<short, പൂർണ്ണസംഖ്യ>(s1, s2, mask, len, cn, 0);
    }
};

template<> ഘടന MaskedNormDiffL2_SIMD<uchar, പൂർണ്ണസംഖ്യ> {
    പൂർണ്ണസംഖ്യ operator()(സ്ഥിരം uchar* s1, സ്ഥിരം uchar* s2, സ്ഥിരം uchar* mask, പൂർണ്ണസംഖ്യ len, പൂർണ്ണസംഖ്യ cn) സ്ഥിരം {
        എങ്കിൽ (cn == 1) തിരികെ maskedNormDiffL2_8(s1, s2, mask, len);
        എങ്കിൽ (cn == 4) {
            പൂർണ്ണസംഖ്യ i = 0; സ്ഥിരം പൂർണ്ണസംഖ്യ vstep = VTraits<v_uint8>::vlanes();
            v_uint32 acc = vx_setzero_u32();
            വേണ്ടി (; i <= len - vstep; i += vstep) {
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
            പൂർണ്ണസംഖ്യ result = (പൂർണ്ണസംഖ്യ)v_reduce_sum(acc);
            വേണ്ടി (; i < len; i++) എങ്കിൽ (mask[i]) {
                സ്ഥിരം uchar* e1 = s1 + i*4; സ്ഥിരം uchar* e2 = s2 + i*4;
                വേണ്ടി (പൂർണ്ണസംഖ്യ k = 0; k < 4; k++) { പൂർണ്ണസംഖ്യ v = (പൂർണ്ണസംഖ്യ)e1[k] - (പൂർണ്ണസംഖ്യ)e2[k]; result += v*v; }
            }
            തിരികെ result;
        }
        തിരികെ maskedNormDiffL2Tail<uchar, പൂർണ്ണസംഖ്യ>(s1, s2, mask, len, cn, 0);
    }
};
template<> ഘടന MaskedNormDiffL2_SIMD<schar, പൂർണ്ണസംഖ്യ> {
    പൂർണ്ണസംഖ്യ operator()(സ്ഥിരം schar* s1, സ്ഥിരം schar* s2, സ്ഥിരം uchar* mask, പൂർണ്ണസംഖ്യ len, പൂർണ്ണസംഖ്യ cn) സ്ഥിരം {
        എങ്കിൽ (cn == 1) തിരികെ maskedNormDiffL2_8(s1, s2, mask, len);
        തിരികെ maskedNormDiffL2Tail<schar, പൂർണ്ണസംഖ്യ>(s1, s2, mask, len, cn, 0);
    }
};

#എങ്കിൽ CV_SIMD_64F
// Depths whose L1/L2 accumulate in double.
template<> ഘടന MaskedNormDiffL1_SIMD<പൂർണ്ണസംഖ്യ, ഇരട്ട> {
    ഇരട്ട operator()(സ്ഥിരം പൂർണ്ണസംഖ്യ* s1, സ്ഥിരം പൂർണ്ണസംഖ്യ* s2, സ്ഥിരം uchar* mask, പൂർണ്ണസംഖ്യ len, പൂർണ്ണസംഖ്യ cn) സ്ഥിരം {
        എങ്കിൽ (cn != 1) തിരികെ maskedNormDiffL1Tail<പൂർണ്ണസംഖ്യ, ഇരട്ട>(s1, s2, mask, len, cn, 0.0);
        പൂർണ്ണസംഖ്യ i = 0; സ്ഥിരം പൂർണ്ണസംഖ്യ vstep = VTraits<v_int32>::vlanes();
        v_float64 acc = vx_setzero_f64();
        വേണ്ടി (; i <= len - vstep; i += vstep) {
            v_uint32 m  = v_gt(vx_load_expand_q(mask + i), vx_setzero_u32());
            v_uint32 ad = v_and(v_absdiff(vx_load(s1 + i), vx_load(s2 + i)), m);
            v_uint64 e0, e1; v_expand(ad, e0, e1);
            acc = v_add(acc, v_cvt_f64(v_reinterpret_as_s64(e0)));
            acc = v_add(acc, v_cvt_f64(v_reinterpret_as_s64(e1)));
        }
        ഇരട്ട result = v_reduce_sum(acc);
        വേണ്ടി (; i < len; i++) എങ്കിൽ (mask[i]) result += (ഇരട്ട)cv_absdiff(s1[i], s2[i]);
        തിരികെ result;
    }
};
template<> ഘടന MaskedNormDiffL1_SIMD<ദശാംശം, ഇരട്ട> {
    ഇരട്ട operator()(സ്ഥിരം ദശാംശം* s1, സ്ഥിരം ദശാംശം* s2, സ്ഥിരം uchar* mask, പൂർണ്ണസംഖ്യ len, പൂർണ്ണസംഖ്യ cn) സ്ഥിരം {
        എങ്കിൽ (cn != 1) തിരികെ maskedNormDiffL1Tail<ദശാംശം, ഇരട്ട>(s1, s2, mask, len, cn, 0.0);
        പൂർണ്ണസംഖ്യ i = 0; സ്ഥിരം പൂർണ്ണസംഖ്യ vstep = VTraits<v_float32>::vlanes();
        v_float64 acc0 = vx_setzero_f64(), acc1 = vx_setzero_f64();
        വേണ്ടി (; i <= len - vstep; i += vstep) {
            v_float32 m  = v_reinterpret_as_f32(v_gt(vx_load_expand_q(mask + i), vx_setzero_u32()));
            v_float32 ad = v_and(v_absdiff(vx_load(s1 + i), vx_load(s2 + i)), m);
            acc0 = v_add(acc0, v_cvt_f64(ad));
            acc1 = v_add(acc1, v_cvt_f64_മുകളിലെ(ad));
        }
        ഇരട്ട result = v_reduce_sum(v_add(acc0, acc1));
        വേണ്ടി (; i < len; i++) എങ്കിൽ (mask[i]) result += (ഇരട്ട)cv_absdiff(s1[i], s2[i]);
        തിരികെ result;
    }
};

template<typename T>
സ്ഥിതം വരിയിൽ ഇരട്ട maskedNormDiffL2_16(സ്ഥിരം T* s1, സ്ഥിരം T* s2, സ്ഥിരം uchar* mask, പൂർണ്ണസംഖ്യ len, പൂർണ്ണസംഖ്യ cn) {
    എങ്കിൽ (cn != 1) തിരികെ maskedNormDiffL2Tail<T, ഇരട്ട>(s1, s2, mask, len, cn, 0.0);
    പൂർണ്ണസംഖ്യ i = 0; സ്ഥിരം പൂർണ്ണസംഖ്യ vstep = VTraits<v_uint16>::vlanes();
    v_float64 acc = vx_setzero_f64();
    വേണ്ടി (; i <= len - vstep; i += vstep) {
        v_uint16 m  = v_gt(vx_load_expand(mask + i), vx_setzero_u16());
        v_uint16 ad = v_and(v_absdiff(vx_load(s1 + i), vx_load(s2 + i)), m);
        v_uint64 u = v_dotprod_expand_fast(ad, ad);
        acc = v_add(acc, v_cvt_f64(v_reinterpret_as_s64(u)));
    }
    ഇരട്ട result = v_reduce_sum(acc);
    വേണ്ടി (; i < len; i++) എങ്കിൽ (mask[i]) { ഇരട്ട v = (ഇരട്ട)s1[i] - (ഇരട്ട)s2[i]; result += v*v; }
    തിരികെ result;
}
template<> ഘടന MaskedNormDiffL2_SIMD<ushort, ഇരട്ട> {
    ഇരട്ട operator()(സ്ഥിരം ushort* s1, സ്ഥിരം ushort* s2, സ്ഥിരം uchar* mask, പൂർണ്ണസംഖ്യ len, പൂർണ്ണസംഖ്യ cn) സ്ഥിരം
    { തിരികെ maskedNormDiffL2_16(s1, s2, mask, len, cn); } };
template<> ഘടന MaskedNormDiffL2_SIMD<short, ഇരട്ട> {
    ഇരട്ട operator()(സ്ഥിരം short* s1, സ്ഥിരം short* s2, സ്ഥിരം uchar* mask, പൂർണ്ണസംഖ്യ len, പൂർണ്ണസംഖ്യ cn) സ്ഥിരം
    { തിരികെ maskedNormDiffL2_16(s1, s2, mask, len, cn); } };

template<> ഘടന MaskedNormDiffL2_SIMD<പൂർണ്ണസംഖ്യ, ഇരട്ട> {
    ഇരട്ട operator()(സ്ഥിരം പൂർണ്ണസംഖ്യ* s1, സ്ഥിരം പൂർണ്ണസംഖ്യ* s2, സ്ഥിരം uchar* mask, പൂർണ്ണസംഖ്യ len, പൂർണ്ണസംഖ്യ cn) സ്ഥിരം {
        എങ്കിൽ (cn != 1) തിരികെ maskedNormDiffL2Tail<പൂർണ്ണസംഖ്യ, ഇരട്ട>(s1, s2, mask, len, cn, 0.0);
        പൂർണ്ണസംഖ്യ i = 0; സ്ഥിരം പൂർണ്ണസംഖ്യ vstep = VTraits<v_int32>::vlanes();
        v_float64 r0 = vx_setzero_f64(), r1 = vx_setzero_f64();
        വേണ്ടി (; i <= len - vstep; i += vstep) {
            v_uint32 m  = v_gt(vx_load_expand_q(mask + i), vx_setzero_u32());
            v_uint32 ad = v_and(v_absdiff(vx_load(s1 + i), vx_load(s2 + i)), m);
            v_uint64 e0, e1; v_expand(ad, e0, e1);
            v_float64 f0 = v_cvt_f64(v_reinterpret_as_s64(e0)), f1 = v_cvt_f64(v_reinterpret_as_s64(e1));
            r0 = v_fma(f0, f0, r0); r1 = v_fma(f1, f1, r1);
        }
        ഇരട്ട result = v_reduce_sum(v_add(r0, r1));
        വേണ്ടി (; i < len; i++) എങ്കിൽ (mask[i]) { ഇരട്ട v = (ഇരട്ട)s1[i] - (ഇരട്ട)s2[i]; result += v*v; }
        തിരികെ result;
    }
};
template<> ഘടന MaskedNormDiffL2_SIMD<ദശാംശം, ഇരട്ട> {
    ഇരട്ട operator()(സ്ഥിരം ദശാംശം* s1, സ്ഥിരം ദശാംശം* s2, സ്ഥിരം uchar* mask, പൂർണ്ണസംഖ്യ len, പൂർണ്ണസംഖ്യ cn) സ്ഥിരം {
        എങ്കിൽ (cn != 1) തിരികെ maskedNormDiffL2Tail<ദശാംശം, ഇരട്ട>(s1, s2, mask, len, cn, 0.0);
        പൂർണ്ണസംഖ്യ i = 0; സ്ഥിരം പൂർണ്ണസംഖ്യ vstep = VTraits<v_float32>::vlanes();
        v_float64 r0 = vx_setzero_f64(), r1 = vx_setzero_f64();
        വേണ്ടി (; i <= len - vstep; i += vstep) {
            v_float32 m  = v_reinterpret_as_f32(v_gt(vx_load_expand_q(mask + i), vx_setzero_u32()));
            v_float32 ad = v_and(v_absdiff(vx_load(s1 + i), vx_load(s2 + i)), m);
            v_float64 f0 = v_cvt_f64(ad), f1 = v_cvt_f64_മുകളിലെ(ad);
            r0 = v_fma(f0, f0, r0); r1 = v_fma(f1, f1, r1);
        }
        ഇരട്ട result = v_reduce_sum(v_add(r0, r1));
        വേണ്ടി (; i < len; i++) എങ്കിൽ (mask[i]) { ഇരട്ട v = (ഇരട്ട)s1[i] - (ഇരട്ട)s2[i]; result += v*v; }
        തിരികെ result;
    }
};
template<>
ഘടന MaskedNormDiffInf_SIMD<ഇരട്ട, ഇരട്ട> {
    വരിയിൽ ഇരട്ട operator()(സ്ഥിരം ഇരട്ട* s1, സ്ഥിരം ഇരട്ട* s2, സ്ഥിരം uchar* mask, പൂർണ്ണസംഖ്യ len, പൂർണ്ണസംഖ്യ cn) സ്ഥിരം {
        ഇരട്ട result = 0.0;
        പൂർണ്ണസംഖ്യ i = 0;
#എങ്കിൽ CV_SIMD_64F || CV_SIMD_SCALABLE_64F
        എങ്കിൽ (cn == 1) {
            സ്ഥിരം പൂർണ്ണസംഖ്യ vstep = VTraits<v_float64>::vlanes();
            v_float64 acc0 = vx_setzero_f64(), acc1 = vx_setzero_f64();
            വേണ്ടി (; i <= len - 2*vstep; i += 2*vstep) {
                v_uint64 m0, m1;
                v_normmask_u64(mask + i, m0, m1);
                v_float64 d0 = v_absdiff(vx_load(s1 + i), vx_load(s2 + i));
                v_float64 d1 = v_absdiff(vx_load(s1 + i + vstep), vx_load(s2 + i + vstep));
                acc0 = v_max(acc0, v_reinterpret_as_f64(v_and(v_reinterpret_as_u64(d0), m0)));
                acc1 = v_max(acc1, v_reinterpret_as_f64(v_and(v_reinterpret_as_u64(d1), m1)));
            }
            ഇരട്ട t[VTraits<v_float64>::max_nlanes];
            vx_store(t, v_max(acc0, acc1));
            വേണ്ടി (പൂർണ്ണസംഖ്യ k = 0; k < vstep; k++)
                result = std::max(result, t[k]);
        }
#endif
        വേണ്ടി (; i < len; i++) എങ്കിൽ (mask[i]) {
            സ്ഥിരം ഇരട്ട* e1 = s1 + i*cn; സ്ഥിരം ഇരട്ട* e2 = s2 + i*cn;
            വേണ്ടി (പൂർണ്ണസംഖ്യ k = 0; k < cn; k++) result = std::max(result, std::abs(e1[k] - e2[k]));
        }
        തിരികെ result;
    }
};

template<>
ഘടന MaskedNormDiffL1_SIMD<ഇരട്ട, ഇരട്ട> {
    ഇരട്ട operator()(സ്ഥിരം ഇരട്ട* s1, സ്ഥിരം ഇരട്ട* s2, സ്ഥിരം uchar* mask, പൂർണ്ണസംഖ്യ len, പൂർണ്ണസംഖ്യ cn) സ്ഥിരം {
        ഇരട്ട result = 0.0;
        എങ്കിൽ (cn == 1) {
            സ്ഥിരം പൂർണ്ണസംഖ്യ vstep = VTraits<v_float64>::vlanes();
            പൂർണ്ണസംഖ്യ i = 0;
            v_float64 acc0 = vx_setzero_f64(), acc1 = vx_setzero_f64();
            വേണ്ടി (; i <= len - 2*vstep; i += 2*vstep) {
                v_uint64 m0, m1;
                v_normmask_u64(mask + i, m0, m1);
                v_float64 d0 = v_absdiff(vx_load(s1 + i), vx_load(s2 + i));
                v_float64 d1 = v_absdiff(vx_load(s1 + i + vstep), vx_load(s2 + i + vstep));
                acc0 = v_add(acc0, v_reinterpret_as_f64(v_and(v_reinterpret_as_u64(d0), m0)));
                acc1 = v_add(acc1, v_reinterpret_as_f64(v_and(v_reinterpret_as_u64(d1), m1)));
            }
            result = v_reduce_sum(v_add(acc0, acc1));
            വേണ്ടി (; i < len; i++) എങ്കിൽ (mask[i]) result += std::abs(s1[i] - s2[i]);
        } അല്ലെങ്കിൽ {
            result = maskedNormDiffL1Tail<ഇരട്ട, ഇരട്ട>(s1, s2, mask, len, cn, 0.0);
        }
        തിരികെ result;
    }
};

template<>
ഘടന MaskedNormDiffL2_SIMD<ഇരട്ട, ഇരട്ട> {
    ഇരട്ട operator()(സ്ഥിരം ഇരട്ട* s1, സ്ഥിരം ഇരട്ട* s2, സ്ഥിരം uchar* mask, പൂർണ്ണസംഖ്യ len, പൂർണ്ണസംഖ്യ cn) സ്ഥിരം {
        ഇരട്ട result = 0.0;
        എങ്കിൽ (cn == 1) {
            സ്ഥിരം പൂർണ്ണസംഖ്യ vstep = VTraits<v_float64>::vlanes();
            പൂർണ്ണസംഖ്യ i = 0;
            v_float64 acc0 = vx_setzero_f64(), acc1 = vx_setzero_f64();
            വേണ്ടി (; i <= len - 2*vstep; i += 2*vstep) {
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
            വേണ്ടി (; i < len; i++) എങ്കിൽ (mask[i]) { ഇരട്ട v = s1[i] - s2[i]; result += v*v; }
        } അല്ലെങ്കിൽ {
            result = maskedNormDiffL2Tail<ഇരട്ട, ഇരട്ട>(s1, s2, mask, len, cn, 0.0);
        }
        തിരികെ result;
    }
};
#endif // CV_SIMD_64F
#endif // CV_SIMD

template<typename T, typename ST> പൂർണ്ണസംഖ്യ
normDiffInf_(സ്ഥിരം T* src1, സ്ഥിരം T* src2, സ്ഥിരം uchar* mask, ST* _result, പൂർണ്ണസംഖ്യ len, പൂർണ്ണസംഖ്യ cn) {
    ST result = *_result;
    എങ്കിൽ( !mask ) {
        NormDiffInf_SIMD<T, ST> op;
        result = std::max(result, op(src1, src2, len*cn));
    } അല്ലെങ്കിൽ {
        MaskedNormDiffInf_SIMD<T, ST> op;
        result = std::max(result, op(src1, src2, mask, len, cn));
    }
    *_result = result;
    തിരികെ 0;
}

template<typename T, typename ST> പൂർണ്ണസംഖ്യ
normDiffL1_(സ്ഥിരം T* src1, സ്ഥിരം T* src2, സ്ഥിരം uchar* mask, ST* _result, പൂർണ്ണസംഖ്യ len, പൂർണ്ണസംഖ്യ cn) {
    ST result = *_result;
    എങ്കിൽ( !mask ) {
        NormDiffL1_SIMD<T, ST> op;
        result += op(src1, src2, len*cn);
    } അല്ലെങ്കിൽ {
        MaskedNormDiffL1_SIMD<T, ST> op;
        result += op(src1, src2, mask, len, cn);
    }
    *_result = result;
    തിരികെ 0;
}

template<typename T, typename ST> പൂർണ്ണസംഖ്യ
normDiffL2_(സ്ഥിരം T* src1, സ്ഥിരം T* src2, സ്ഥിരം uchar* mask, ST* _result, പൂർണ്ണസംഖ്യ len, പൂർണ്ണസംഖ്യ cn) {
    ST result = *_result;
    എങ്കിൽ( !mask ) {
        NormDiffL2_SIMD<T, ST> op;
        result += op(src1, src2, len*cn);
    } അല്ലെങ്കിൽ {
        MaskedNormDiffL2_SIMD<T, ST> op;
        result += op(src1, src2, mask, len, cn);
    }
    *_result = result;
    തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ
normDiffInf_Bool(സ്ഥിരം uchar* src1, സ്ഥിരം uchar* src2, സ്ഥിരം uchar* mask, പൂർണ്ണസംഖ്യ* _result, പൂർണ്ണസംഖ്യ len, പൂർണ്ണസംഖ്യ cn)
{
    പൂർണ്ണസംഖ്യ result = *_result;
    എങ്കിൽ( !mask )
    {
        വേണ്ടി( പൂർണ്ണസംഖ്യ i = 0; i < len*cn; i++ ) {
            result = std::max(result, (പൂർണ്ണസംഖ്യ)((src1[i] != 0) != (src2[i] != 0)));
            എങ്കിൽ (result != 0)
                നിർത്തുക;
        }
    }
    അല്ലെങ്കിൽ
    {
        വേണ്ടി( പൂർണ്ണസംഖ്യ i = 0; i < len; i++, src1 += cn, src2 += cn )
            എങ്കിൽ( mask[i] )
            {
                വേണ്ടി( പൂർണ്ണസംഖ്യ k = 0; k < cn; k++ )
                    result = std::max(result, (പൂർണ്ണസംഖ്യ)((src1[k] != 0) != (src2[k] != 0)));
                എങ്കിൽ (result != 0)
                    നിർത്തുക;
            }
    }
    *_result = result;
    തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ
normDiffL1_Bool(സ്ഥിരം uchar* src1, സ്ഥിരം uchar* src2, സ്ഥിരം uchar* mask, പൂർണ്ണസംഖ്യ* _result, പൂർണ്ണസംഖ്യ len, പൂർണ്ണസംഖ്യ cn)
{
    പൂർണ്ണസംഖ്യ result = *_result;
    എങ്കിൽ( !mask )
    {
        വേണ്ടി( പൂർണ്ണസംഖ്യ i = 0; i < len*cn; i++ )
            result += (പൂർണ്ണസംഖ്യ)((src1[i] != 0) != (src2[i] != 0));
    }
    അല്ലെങ്കിൽ
    {
        വേണ്ടി( പൂർണ്ണസംഖ്യ i = 0; i < len; i++, src1 += cn, src2 += cn )
            എങ്കിൽ( mask[i] )
            {
                വേണ്ടി( പൂർണ്ണസംഖ്യ k = 0; k < cn; k++ )
                    result += (പൂർണ്ണസംഖ്യ)((src1[k] != 0) != (src2[k] != 0));
            }
    }
    *_result = result;
    തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ
normDiffL2_Bool(സ്ഥിരം uchar* src1, സ്ഥിരം uchar* src2, സ്ഥിരം uchar* mask, പൂർണ്ണസംഖ്യ* _result, പൂർണ്ണസംഖ്യ len, പൂർണ്ണസംഖ്യ cn)
{
    തിരികെ normDiffL1_Bool(src1, src2, mask, _result, len, cn);
}

#define CV_DEF_NORM_FUNC(L, suffix, type, ntype) \
    സ്ഥിതം പൂർണ്ണസംഖ്യ norm##L##_##suffix(സ്ഥിരം type* src, സ്ഥിരം uchar* mask, ntype* r, പൂർണ്ണസംഖ്യ len, പൂർണ്ണസംഖ്യ cn) \
{ CV_INSTRUMENT_REGION(); തിരികെ norm##L##_(src, mask, r, len, cn); } \
    സ്ഥിതം പൂർണ്ണസംഖ്യ normDiff##L##_##suffix(സ്ഥിരം type* src1, സ്ഥിരം type* src2, \
    സ്ഥിരം uchar* mask, ntype* r, പൂർണ്ണസംഖ്യ len, പൂർണ്ണസംഖ്യ cn) \
{ തിരികെ normDiff##L##_(src1, src2, mask, r, (പൂർണ്ണസംഖ്യ)len, cn); }

#define CV_DEF_NORM_ALL(suffix, type, inftype, l1type, l2type) \
    CV_DEF_NORM_FUNC(Inf, suffix, type, inftype) \
    CV_DEF_NORM_FUNC(L1, suffix, type, l1type) \
    CV_DEF_NORM_FUNC(L2, suffix, type, l2type)

CV_DEF_NORM_ALL(8u, uchar, പൂർണ്ണസംഖ്യ, പൂർണ്ണസംഖ്യ, പൂർണ്ണസംഖ്യ)
CV_DEF_NORM_ALL(8s, schar, പൂർണ്ണസംഖ്യ, പൂർണ്ണസംഖ്യ, പൂർണ്ണസംഖ്യ)
CV_DEF_NORM_ALL(16u, ushort, പൂർണ്ണസംഖ്യ, പൂർണ്ണസംഖ്യ, ഇരട്ട)
CV_DEF_NORM_ALL(16s, short, പൂർണ്ണസംഖ്യ, പൂർണ്ണസംഖ്യ, ഇരട്ട)
CV_DEF_NORM_ALL(32u, unsigned, unsigned, ഇരട്ട, ഇരട്ട)
CV_DEF_NORM_ALL(32s, പൂർണ്ണസംഖ്യ, unsigned, ഇരട്ട, ഇരട്ട)
CV_DEF_NORM_ALL(32f, ദശാംശം, ദശാംശം, ഇരട്ട, ഇരട്ട)
CV_DEF_NORM_ALL(64f, ഇരട്ട, ഇരട്ട, ഇരട്ട, ഇരട്ട)
CV_DEF_NORM_ALL(64u, uint64, uint64, ഇരട്ട, ഇരട്ട)
CV_DEF_NORM_ALL(64s, int64, uint64, ഇരട്ട, ഇരട്ട)
CV_DEF_NORM_ALL(16f, hfloat, ദശാംശം, ദശാംശം, ദശാംശം)
CV_DEF_NORM_ALL(16bf, bfloat, ദശാംശം, ദശാംശം, ദശാംശം)

NormFunc getNormFunc(പൂർണ്ണസംഖ്യ normType, പൂർണ്ണസംഖ്യ depth)
{
    CV_INSTRUMENT_REGION();

    സ്ഥിതം NormFunc normTab[3][CV_DEPTH_MAX] =
    {
        {
            normWrap<uchar, പൂർണ്ണസംഖ്യ, normInf_8u>,
            normWrap<schar, പൂർണ്ണസംഖ്യ, normInf_8s>,
            normWrap<ushort, പൂർണ്ണസംഖ്യ, normInf_16u>,
            normWrap<short, പൂർണ്ണസംഖ്യ, normInf_16s>,
            normWrap<പൂർണ്ണസംഖ്യ, unsigned, normInf_32s>,
            normWrap<ദശാംശം, ദശാംശം, normInf_32f>,
            normWrap<ഇരട്ട, ഇരട്ട, normInf_64f>,
            normWrap<hfloat, ദശാംശം, normInf_16f>,
            normWrap<bfloat, ദശാംശം, normInf_16bf>,
            normWrap<uchar, പൂർണ്ണസംഖ്യ, normInf_Bool>,
            normWrap<uint64, uint64, normInf_64u>,
            normWrap<int64, uint64, normInf_64s>,
            normWrap<unsigned, unsigned, normInf_32u>,
            0
        },
        {
            normWrap<uchar, പൂർണ്ണസംഖ്യ, normL1_8u>,
            normWrap<schar, പൂർണ്ണസംഖ്യ, normL1_8s>,
            normWrap<ushort, പൂർണ്ണസംഖ്യ, normL1_16u>,
            normWrap<short, പൂർണ്ണസംഖ്യ, normL1_16s>,
            normWrap<പൂർണ്ണസംഖ്യ, ഇരട്ട, normL1_32s>,
            normWrap<ദശാംശം, ഇരട്ട, normL1_32f>,
            normWrap<ഇരട്ട, ഇരട്ട, normL1_64f>,
            normWrap<hfloat, ദശാംശം, normL1_16f>,
            normWrap<bfloat, ദശാംശം, normL1_16bf>,
            normWrap<uchar, പൂർണ്ണസംഖ്യ, normL1_Bool>,
            normWrap<uint64, ഇരട്ട, normL1_64u>,
            normWrap<int64, ഇരട്ട, normL1_64s>,
            normWrap<unsigned, ഇരട്ട, normL1_32u>,
            0
        },
        {
            normWrap<uchar, പൂർണ്ണസംഖ്യ, normL2_8u>,
            normWrap<schar, പൂർണ്ണസംഖ്യ, normL2_8s>,
            normWrap<ushort, ഇരട്ട, normL2_16u>,
            normWrap<short, ഇരട്ട, normL2_16s>,
            normWrap<പൂർണ്ണസംഖ്യ, ഇരട്ട, normL2_32s>,
            normWrap<ദശാംശം, ഇരട്ട, normL2_32f>,
            normWrap<ഇരട്ട, ഇരട്ട, normL2_64f>,
            normWrap<hfloat, ദശാംശം, normL2_16f>,
            normWrap<bfloat, ദശാംശം, normL2_16bf>,
            normWrap<uchar, പൂർണ്ണസംഖ്യ, normL2_Bool>,
            normWrap<uint64, ഇരട്ട, normL2_64u>,
            normWrap<int64, ഇരട്ട, normL2_64s>,
            normWrap<unsigned, ഇരട്ട, normL2_32u>,
            0
        }
    };

    എങ്കിൽ (normType >= 3 || normType < 0) തിരികെ nullptr;

    തിരികെ normTab[normType][depth];
}

NormDiffFunc getNormDiffFunc(പൂർണ്ണസംഖ്യ normType, പൂർണ്ണസംഖ്യ depth)
{
    സ്ഥിതം NormDiffFunc normDiffTab[3][CV_DEPTH_MAX] =
    {
        {
            normDiffWrap<uchar, പൂർണ്ണസംഖ്യ, normDiffInf_8u>,
            normDiffWrap<schar, പൂർണ്ണസംഖ്യ, normDiffInf_8s>,
            normDiffWrap<ushort, പൂർണ്ണസംഖ്യ, normDiffInf_16u>,
            normDiffWrap<short, പൂർണ്ണസംഖ്യ, normDiffInf_16s>,
            normDiffWrap<പൂർണ്ണസംഖ്യ, unsigned, normDiffInf_32s>,
            normDiffWrap<ദശാംശം, ദശാംശം, normDiffInf_32f>,
            normDiffWrap<ഇരട്ട, ഇരട്ട, normDiffInf_64f>,
            normDiffWrap<hfloat, ദശാംശം, normDiffInf_16f>,
            normDiffWrap<bfloat, ദശാംശം, normDiffInf_16bf>,
            normDiffWrap<uchar, പൂർണ്ണസംഖ്യ, normDiffInf_Bool>,
            normDiffWrap<uint64, uint64, normDiffInf_64u>,
            normDiffWrap<int64, uint64, normDiffInf_64s>,
            normDiffWrap<unsigned, unsigned, normDiffInf_32u>,
            0
        },
        {
            normDiffWrap<uchar, പൂർണ്ണസംഖ്യ, normDiffL1_8u>,
            normDiffWrap<schar, പൂർണ്ണസംഖ്യ, normDiffL1_8s>,
            normDiffWrap<ushort, പൂർണ്ണസംഖ്യ, normDiffL1_16u>,
            normDiffWrap<short, പൂർണ്ണസംഖ്യ, normDiffL1_16s>,
            normDiffWrap<പൂർണ്ണസംഖ്യ, ഇരട്ട, normDiffL1_32s>,
            normDiffWrap<ദശാംശം, ഇരട്ട, normDiffL1_32f>,
            normDiffWrap<ഇരട്ട, ഇരട്ട, normDiffL1_64f>,
            normDiffWrap<hfloat, ദശാംശം, normDiffL1_16f>,
            normDiffWrap<bfloat, ദശാംശം, normDiffL1_16bf>,
            normDiffWrap<uchar, പൂർണ്ണസംഖ്യ, normDiffL1_Bool>,
            normDiffWrap<uint64, ഇരട്ട, normDiffL1_64u>,
            normDiffWrap<int64, ഇരട്ട, normDiffL1_64s>,
            normDiffWrap<unsigned, ഇരട്ട, normDiffL1_32u>,
            0
        },
        {
            normDiffWrap<uchar, പൂർണ്ണസംഖ്യ, normDiffL2_8u>,
            normDiffWrap<schar, പൂർണ്ണസംഖ്യ, normDiffL2_8s>,
            normDiffWrap<ushort, ഇരട്ട, normDiffL2_16u>,
            normDiffWrap<short, ഇരട്ട, normDiffL2_16s>,
            normDiffWrap<പൂർണ്ണസംഖ്യ, ഇരട്ട, normDiffL2_32s>,
            normDiffWrap<ദശാംശം, ഇരട്ട, normDiffL2_32f>,
            normDiffWrap<ഇരട്ട, ഇരട്ട, normDiffL2_64f>,
            normDiffWrap<hfloat, ദശാംശം, normDiffL2_16f>,
            normDiffWrap<bfloat, ദശാംശം, normDiffL2_16bf>,
            normDiffWrap<uchar, പൂർണ്ണസംഖ്യ, normDiffL2_Bool>,
            normDiffWrap<uint64, ഇരട്ട, normDiffL2_64u>,
            normDiffWrap<int64, ഇരട്ട, normDiffL2_64s>,
            normDiffWrap<unsigned, ഇരട്ട, normDiffL2_32u>,
            0
        },
    };
    എങ്കിൽ (normType >= 3 || normType < 0) തിരികെ nullptr;

    തിരികെ normDiffTab[normType][depth];
}

#endif // CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

CV_CPU_OPTIMIZATION_NAMESPACE_END

} // cv::
