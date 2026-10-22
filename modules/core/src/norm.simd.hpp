// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html
//
// Copyright (C) 2025, SpaceMIT Inc., all rights reserved.
// Copyright (C) 2026, Advanced Micro Devices, Inc., all rights reserved.
// Third party copyrights are property of their respective owners.

#अन्तर्भाव "precomp.hpp"

नामाकाश cv {

उपयुज्य NormFunc = पूर्णाङ्क (*)(ध्रुव uchar*, ध्रुव uchar*, uchar*, पूर्णाङ्क, पूर्णाङ्क);
उपयुज्य NormDiffFunc = पूर्णाङ्क (*)(ध्रुव uchar*, ध्रुव uchar*, ध्रुव uchar*, uchar*, पूर्णाङ्क, पूर्णाङ्क);

CV_CPU_OPTIMIZATION_NAMESPACE_BEGIN

प्रतिरूपम्<प्रकारनाम T, प्रकारनाम RT, पूर्णाङ्क (*fn)(ध्रुव T*, ध्रुव uchar*, RT*, पूर्णाङ्क, पूर्णाङ्क)>
स्थिर पूर्णाङ्क normWrap(ध्रुव uchar* src1, ध्रुव uchar* src2, uchar* res, पूर्णाङ्क len, पूर्णाङ्क cn)
{
    प्रत्यागम fn((ध्रुव T*)src1, src2, (RT*)res, len, cn);
}

प्रतिरूपम्<प्रकारनाम T, प्रकारनाम RT, पूर्णाङ्क (*fn)(ध्रुव T*, ध्रुव T*, ध्रुव uchar*, RT*, पूर्णाङ्क, पूर्णाङ्क)>
स्थिर पूर्णाङ्क normDiffWrap(ध्रुव uchar* src1, ध्रुव uchar* src2, ध्रुव uchar* src3, uchar* res, पूर्णाङ्क len, पूर्णाङ्क cn)
{
    प्रत्यागम fn((ध्रुव T*)src1, (ध्रुव T*)src2, src3, (RT*)res, len, cn);
}

NormFunc getNormFunc(पूर्णाङ्क normType, पूर्णाङ्क गभीरता);
NormDiffFunc getNormDiffFunc(पूर्णाङ्क normType, पूर्णाङ्क गभीरता);

#यदि_अपरिभाषितम् CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

प्रतिरूपम् <प्रकारनाम T, प्रकारनाम ST>
संरचना NormInf_SIMD {
    अन्तःस्थ ST संक्रियकः() (ध्रुव T* src, पूर्णाङ्क n) ध्रुव {
        ST s = 0;
        प्रति (पूर्णाङ्क i = 0; i < n; i++) {
            s = std::max(s, (ST)cv_abs(src[i]));
        }
        प्रत्यागम s;
    }
};

प्रतिरूपम् <प्रकारनाम T, प्रकारनाम ST>
संरचना NormL1_SIMD {
    अन्तःस्थ ST संक्रियकः() (ध्रुव T* src, पूर्णाङ्क n) ध्रुव {
        ST s = 0;
        प्रति (पूर्णाङ्क i = 0; i < n; i++) {
            s += cv_abs(src[i]);
        }
        प्रत्यागम s;
    }
};

प्रतिरूपम् <प्रकारनाम T, प्रकारनाम ST>
संरचना NormL2_SIMD {
    अन्तःस्थ ST संक्रियकः() (ध्रुव T* src, पूर्णाङ्क n) ध्रुव {
        ST s = 0;
        प्रति (पूर्णाङ्क i = 0; i < n; i++) {
            ST v = (ST)src[i];
            s += v * v;
        }
        प्रत्यागम s;
    }
};

प्रतिरूपम् <प्रकारनाम T, प्रकारनाम ST>
संरचना NormDiffInf_SIMD {
    अन्तःस्थ ST संक्रियकः() (ध्रुव T* src1, ध्रुव T* src2, पूर्णाङ्क n) ध्रुव {
        ST s = 0;
        प्रति (पूर्णाङ्क i = 0; i < n; i++) {
            ST v = (ST)cv_absdiff(src1[i], src2[i]);
            s = std::max(s, v);
        }
        प्रत्यागम s;
    }
};

प्रतिरूपम् <प्रकारनाम T, प्रकारनाम ST>
संरचना NormDiffL1_SIMD {
    अन्तःस्थ ST संक्रियकः() (ध्रुव T* src1, ध्रुव T* src2, पूर्णाङ्क n) ध्रुव {
        ST s = 0;
        प्रति (पूर्णाङ्क i = 0; i < n; i++) {
            ST v = (ST)cv_absdiff(src1[i], src2[i]);
            s += v;
        }
        प्रत्यागम s;
    }
};

प्रतिरूपम् <प्रकारनाम T, प्रकारनाम ST>
संरचना NormDiffL2_SIMD {
    अन्तःस्थ ST संक्रियकः() (ध्रुव T* src1, ध्रुव T* src2, पूर्णाङ्क n) ध्रुव {
        ST s = 0;
        प्रति (पूर्णाङ्क i = 0; i < n; i++) {
            ST v = (ST)src1[i] - (ST)src2[i];
            s += v * v;
        }
        प्रत्यागम s;
    }
};

#यदि (CV_SIMD || CV_SIMD_SCALABLE)

प्रतिरूपम्<>
संरचना NormInf_SIMD<uchar, पूर्णाङ्क> {
    पूर्णाङ्क संक्रियकः() (ध्रुव uchar* src, पूर्णाङ्क n) ध्रुव {
        पूर्णाङ्क j = 0;
        पूर्णाङ्क s = 0;
        v_uint8 r0 = vx_setzero_u8(), r1 = vx_setzero_u8();
        v_uint8 r2 = vx_setzero_u8(), r3 = vx_setzero_u8();
        प्रति (; j <= n - 4 * VTraits<v_uint8>::vlanes(); j += 4 * VTraits<v_uint8>::vlanes()) {
            r0 = v_max(r0, vx_आरोपय(src + j                                 ));
            r1 = v_max(r1, vx_आरोपय(src + j +     VTraits<v_uint8>::vlanes()));
            r2 = v_max(r2, vx_आरोपय(src + j + 2 * VTraits<v_uint8>::vlanes()));
            r3 = v_max(r3, vx_आरोपय(src + j + 3 * VTraits<v_uint8>::vlanes()));
        }
        r0 = v_max(r0, v_max(r1, v_max(r2, r3)));
        प्रति (; j < n; j++) {
            s = std::max(s, (पूर्णाङ्क)src[j]);
        }
        प्रत्यागम std::max(s, (पूर्णाङ्क)v_reduce_max(r0));
    }
};

प्रतिरूपम्<>
संरचना NormInf_SIMD<schar, पूर्णाङ्क> {
    पूर्णाङ्क संक्रियकः() (ध्रुव schar* src, पूर्णाङ्क n) ध्रुव {
        पूर्णाङ्क j = 0;
        पूर्णाङ्क s = 0;
        v_uint8 r0 = vx_setzero_u8(), r1 = vx_setzero_u8();
        v_uint8 r2 = vx_setzero_u8(), r3 = vx_setzero_u8();
        प्रति (; j <= n - 4 * VTraits<v_int8>::vlanes(); j += 4 * VTraits<v_int8>::vlanes()) {
            r0 = v_max(r0, v_abs(vx_आरोपय(src + j                                )));
            r1 = v_max(r1, v_abs(vx_आरोपय(src + j +     VTraits<v_int8>::vlanes())));
            r2 = v_max(r2, v_abs(vx_आरोपय(src + j + 2 * VTraits<v_int8>::vlanes())));
            r3 = v_max(r3, v_abs(vx_आरोपय(src + j + 3 * VTraits<v_int8>::vlanes())));
        }
        r0 = v_max(r0, v_max(r1, v_max(r2, r3)));
        प्रति (; j < n; j++) {
            s = std::max(s, cv_abs(src[j]));
        }
        प्रत्यागम std::max(s, saturate_cast<पूर्णाङ्क>(v_reduce_max(r0)));
    }
};

प्रतिरूपम्<>
संरचना NormInf_SIMD<ushort, पूर्णाङ्क> {
    पूर्णाङ्क संक्रियकः() (ध्रुव ushort* src, पूर्णाङ्क n) ध्रुव {
        पूर्णाङ्क j = 0;
        पूर्णाङ्क s = 0;
        v_uint16 d0 = vx_setzero_u16(), d1 = vx_setzero_u16();
        v_uint16 d2 = vx_setzero_u16(), d3 = vx_setzero_u16();
        प्रति (; j <= n - 4 * VTraits<v_uint16>::vlanes(); j += 4 * VTraits<v_uint16>::vlanes()) {
            d0 = v_max(d0, vx_आरोपय(src + j                                  ));
            d1 = v_max(d1, vx_आरोपय(src + j +     VTraits<v_uint16>::vlanes()));
            d2 = v_max(d2, vx_आरोपय(src + j + 2 * VTraits<v_uint16>::vlanes()));
            d3 = v_max(d3, vx_आरोपय(src + j + 3 * VTraits<v_uint16>::vlanes()));
        }
        d0 = v_max(d0, v_max(d1, v_max(d2, d3)));
        प्रति (; j < n; j++) {
            s = std::max(s, (पूर्णाङ्क)src[j]);
        }
        प्रत्यागम std::max(s, (पूर्णाङ्क)v_reduce_max(d0));
    }
};

प्रतिरूपम्<>
संरचना NormInf_SIMD<ह्रस्व, पूर्णाङ्क> {
    पूर्णाङ्क संक्रियकः() (ध्रुव ह्रस्व* src, पूर्णाङ्क n) ध्रुव {
        पूर्णाङ्क j = 0;
        पूर्णाङ्क s = 0;
        v_uint16 d0 = vx_setzero_u16(), d1 = vx_setzero_u16();
        v_uint16 d2 = vx_setzero_u16(), d3 = vx_setzero_u16();
        प्रति (; j <= n - 4 * VTraits<v_int16>::vlanes(); j += 4 * VTraits<v_int16>::vlanes()) {
            d0 = v_max(d0, v_abs(vx_आरोपय(src + j                                  )));
            d1 = v_max(d1, v_abs(vx_आरोपय(src + j +     VTraits<v_int16>::vlanes())));
            d2 = v_max(d2, v_abs(vx_आरोपय(src + j + 2 * VTraits<v_int16>::vlanes())));
            d3 = v_max(d3, v_abs(vx_आरोपय(src + j + 3 * VTraits<v_int16>::vlanes())));
        }
        d0 = v_max(d0, v_max(d1, v_max(d2, d3)));
        प्रति (; j < n; j++) {
            s = std::max(s, saturate_cast<पूर्णाङ्क>(cv_abs(src[j])));
        }
        प्रत्यागम std::max(s, saturate_cast<पूर्णाङ्क>(v_reduce_max(d0)));
    }
};

प्रतिरूपम्<>
संरचना NormInf_SIMD<पूर्णाङ्क, पूर्णाङ्क> {
    पूर्णाङ्क संक्रियकः() (ध्रुव पूर्णाङ्क* src, पूर्णाङ्क n) ध्रुव {
        पूर्णाङ्क j = 0;
        पूर्णाङ्क s = 0;
        v_uint32 r0 = vx_setzero_u32(), r1 = vx_setzero_u32();
        v_uint32 r2 = vx_setzero_u32(), r3 = vx_setzero_u32();
        प्रति (; j <= n - 4 * VTraits<v_int32>::vlanes(); j += 4 * VTraits<v_int32>::vlanes()) {
            r0 = v_max(r0, v_abs(vx_आरोपय(src + j                                 )));
            r1 = v_max(r1, v_abs(vx_आरोपय(src + j +     VTraits<v_int32>::vlanes())));
            r2 = v_max(r2, v_abs(vx_आरोपय(src + j + 2 * VTraits<v_int32>::vlanes())));
            r3 = v_max(r3, v_abs(vx_आरोपय(src + j + 3 * VTraits<v_int32>::vlanes())));
        }
        r0 = v_max(r0, v_max(r1, v_max(r2, r3)));
        s = std::max(s, saturate_cast<पूर्णाङ्क>(v_reduce_max(r0)));
        प्रति (; j < n; j++) {
            s = std::max(s, std::निरपेक्षम्(src[j]));
        }
        प्रत्यागम s;
    }
};

प्रतिरूपम्<>
संरचना NormInf_SIMD<भिन्न, भिन्न> {
    भिन्न संक्रियकः() (ध्रुव भिन्न* src, पूर्णाङ्क n) ध्रुव {
        पूर्णाङ्क j = 0;
        भिन्न s = 0.f;
        v_float32 r0 = vx_setzero_f32(), r1 = vx_setzero_f32();
        v_float32 r2 = vx_setzero_f32(), r3 = vx_setzero_f32();
        प्रति (; j <= n - 4 * VTraits<v_float32>::vlanes(); j += 4 * VTraits<v_float32>::vlanes()) {
            r0 = v_max(r0, v_abs(vx_आरोपय(src + j                                   )));
            r1 = v_max(r1, v_abs(vx_आरोपय(src + j +     VTraits<v_float32>::vlanes())));
            r2 = v_max(r2, v_abs(vx_आरोपय(src + j + 2 * VTraits<v_float32>::vlanes())));
            r3 = v_max(r3, v_abs(vx_आरोपय(src + j + 3 * VTraits<v_float32>::vlanes())));
        }
        r0 = v_max(r0, v_max(r1, v_max(r2, r3)));
        प्रति (; j < n; j++) {
            s = std::max(s, cv_abs(src[j]));
        }
        प्रत्यागम std::max(s, v_reduce_max(r0));
    }
};

प्रतिरूपम्<>
संरचना NormL1_SIMD<uchar, पूर्णाङ्क> {
    पूर्णाङ्क संक्रियकः() (ध्रुव uchar* src, पूर्णाङ्क n) ध्रुव {
        पूर्णाङ्क j = 0;
        पूर्णाङ्क s = 0;
        v_uint32 r0 = vx_setzero_u32(), r1 = vx_setzero_u32();
        v_uint8  one = vx_setall_u8(1);
        प्रति (; j<= n - 2 * VTraits<v_uint8>::vlanes(); j += 2 * VTraits<v_uint8>::vlanes()) {
            v_uint8 v0 = vx_आरोपय(src + j);
            r0 = v_dotprod_expand_fast(v0, one, r0);

            v_uint8 v1 = vx_आरोपय(src + j + VTraits<v_uint8>::vlanes());
            r1 = v_dotprod_expand_fast(v1, one, r1);
        }
        s += v_reduce_योग(v_सङ्कलय(r0, r1));
        प्रति (; j < n; j++) {
            s += src[j];
        }
        प्रत्यागम s;
    }
};

प्रतिरूपम्<>
संरचना NormL1_SIMD<schar, पूर्णाङ्क> {
    पूर्णाङ्क संक्रियकः() (ध्रुव schar* src, पूर्णाङ्क n) ध्रुव {
        पूर्णाङ्क j = 0;
        पूर्णाङ्क s = 0;
        v_uint32 r0 = vx_setzero_u32(), r1 = vx_setzero_u32();
        v_uint8  one = vx_setall_u8(1);
        प्रति (; j<= n - 2 * VTraits<v_int8>::vlanes(); j += 2 * VTraits<v_int8>::vlanes()) {
            v_uint8 v0 = v_abs(vx_आरोपय(src + j));
            r0 = v_dotprod_expand_fast(v0, one, r0);

            v_uint8 v1 = v_abs(vx_आरोपय(src + j + VTraits<v_int8>::vlanes()));
            r1 = v_dotprod_expand_fast(v1, one, r1);
        }
        s += v_reduce_योग(v_सङ्कलय(r0, r1));
        प्रति (; j < n; j++) {
            s += saturate_cast<पूर्णाङ्क>(cv_abs(src[j]));
        }
        प्रत्यागम s;
    }
};

प्रतिरूपम्<>
संरचना NormL1_SIMD<ushort, पूर्णाङ्क> {
    पूर्णाङ्क संक्रियकः() (ध्रुव ushort* src, पूर्णाङ्क n) ध्रुव {
        पूर्णाङ्क j = 0;
        पूर्णाङ्क s = 0;
        v_uint32 r00 = vx_setzero_u32(), r01 = vx_setzero_u32();
        v_uint32 r10 = vx_setzero_u32(), r11 = vx_setzero_u32();
        प्रति (; j<= n - 2 * VTraits<v_uint16>::vlanes(); j += 2 * VTraits<v_uint16>::vlanes()) {
            v_uint16 v0 = vx_आरोपय(src + j);
            v_uint32 v00, v01;
            v_expand(v0, v00, v01);
            r00 = v_सङ्कलय(r00, v00);
            r01 = v_सङ्कलय(r01, v01);

            v_uint16 v1 = vx_आरोपय(src + j + VTraits<v_uint16>::vlanes());
            v_uint32 v10, v11;
            v_expand(v1, v10, v11);
            r10 = v_सङ्कलय(r10, v10);
            r11 = v_सङ्कलय(r11, v11);
        }
        s += (पूर्णाङ्क)v_reduce_योग(v_सङ्कलय(v_सङ्कलय(v_सङ्कलय(r00, r01), r10), r11));
        प्रति (; j < n; j++) {
            s += src[j];
        }
        प्रत्यागम s;
    }
};

प्रतिरूपम्<>
संरचना NormL1_SIMD<ह्रस्व, पूर्णाङ्क> {
    पूर्णाङ्क संक्रियकः() (ध्रुव ह्रस्व* src, पूर्णाङ्क n) ध्रुव {
        पूर्णाङ्क j = 0;
        पूर्णाङ्क s = 0;
        v_uint32 r00 = vx_setzero_u32(), r01 = vx_setzero_u32();
        v_uint32 r10 = vx_setzero_u32(), r11 = vx_setzero_u32();
        प्रति (; j<= n - 2 * VTraits<v_int16>::vlanes(); j += 2 * VTraits<v_int16>::vlanes()) {
            v_uint16 v0 = v_abs(vx_आरोपय(src + j));
            v_uint32 v00, v01;
            v_expand(v0, v00, v01);
            r00 = v_सङ्कलय(r00, v00);
            r01 = v_सङ्कलय(r01, v01);

            v_uint16 v1 = v_abs(vx_आरोपय(src + j + VTraits<v_int16>::vlanes()));
            v_uint32 v10, v11;
            v_expand(v1, v10, v11);
            r10 = v_सङ्कलय(r10, v10);
            r11 = v_सङ्कलय(r11, v11);
        }
        s += (पूर्णाङ्क)v_reduce_योग(v_सङ्कलय(v_सङ्कलय(v_सङ्कलय(r00, r01), r10), r11));
        प्रति (; j < n; j++) {
            s += saturate_cast<पूर्णाङ्क>(cv_abs(src[j]));
        }
        प्रत्यागम s;
    }
};

प्रतिरूपम्<>
संरचना NormL2_SIMD<uchar, पूर्णाङ्क> {
    पूर्णाङ्क संक्रियकः() (ध्रुव uchar* src, पूर्णाङ्क n) ध्रुव {
        पूर्णाङ्क j = 0;
        पूर्णाङ्क s = 0;
        v_uint32 r0 = vx_setzero_u32(), r1 = vx_setzero_u32();
        प्रति (; j <= n - 2 * VTraits<v_uint8>::vlanes(); j += 2 * VTraits<v_uint8>::vlanes()) {
            v_uint8 v0 = vx_आरोपय(src + j);
            r0 = v_dotprod_expand_fast(v0, v0, r0);

            v_uint8 v1 = vx_आरोपय(src + j + VTraits<v_uint8>::vlanes());
            r1 = v_dotprod_expand_fast(v1, v1, r1);
        }
        s += v_reduce_योग(v_सङ्कलय(r0, r1));
        प्रति (; j < n; j++) {
            पूर्णाङ्क v = saturate_cast<पूर्णाङ्क>(src[j]);
            s += v * v;
        }
        प्रत्यागम s;
    }
};

प्रतिरूपम्<>
संरचना NormL2_SIMD<schar, पूर्णाङ्क> {
    पूर्णाङ्क संक्रियकः() (ध्रुव schar* src, पूर्णाङ्क n) ध्रुव {
        पूर्णाङ्क j = 0;
        पूर्णाङ्क s = 0;
        v_int32 r0 = vx_setzero_s32(), r1 = vx_setzero_s32();
        प्रति (; j <= n - 2 * VTraits<v_int8>::vlanes(); j += 2 * VTraits<v_int8>::vlanes()) {
            v_int8 v0 = vx_आरोपय(src + j);
            r0 = v_dotprod_expand_fast(v0, v0, r0);
            v_int8 v1 = vx_आरोपय(src + j + VTraits<v_int8>::vlanes());
            r1 = v_dotprod_expand_fast(v1, v1, r1);
        }
        s += v_reduce_योग(v_सङ्कलय(r0, r1));
        प्रति (; j < n; j++) {
            पूर्णाङ्क v = saturate_cast<पूर्णाङ्क>(src[j]);
            s += v * v;
        }
        प्रत्यागम s;
    }
};

प्रतिरूपम्<>
संरचना NormDiffInf_SIMD<uchar, पूर्णाङ्क> {
    पूर्णाङ्क संक्रियकः() (ध्रुव uchar* src1, ध्रुव uchar* src2, पूर्णाङ्क n) ध्रुव {
        पूर्णाङ्क j = 0;
        पूर्णाङ्क s = 0;
        v_uint8 r0 = vx_setzero_u8(), r1 = vx_setzero_u8();
        v_uint8 r2 = vx_setzero_u8(), r3 = vx_setzero_u8();
        प्रति (; j <= n - 4 * VTraits<v_uint8>::vlanes(); j += 4 * VTraits<v_uint8>::vlanes()) {
            v_uint8 v01 = vx_आरोपय(src1 + j), v02 = vx_आरोपय(src2 + j);
            r0 = v_max(r0, v_absdiff(v01, v02));

            v_uint8 v11 = vx_आरोपय(src1 + j + VTraits<v_uint8>::vlanes()),
                    v12 = vx_आरोपय(src2 + j + VTraits<v_uint8>::vlanes());
            r1 = v_max(r1, v_absdiff(v11, v12));

            v_uint8 v21 = vx_आरोपय(src1 + j + 2 * VTraits<v_uint8>::vlanes()),
                    v22 = vx_आरोपय(src2 + j + 2 * VTraits<v_uint8>::vlanes());
            r2 = v_max(r2, v_absdiff(v21, v22));

            v_uint8 v31 = vx_आरोपय(src1 + j + 3 * VTraits<v_uint8>::vlanes()),
                    v32 = vx_आरोपय(src2 + j + 3 * VTraits<v_uint8>::vlanes());
            r3 = v_max(r3, v_absdiff(v31, v32));
        }
        s = (पूर्णाङ्क)v_reduce_max(v_max(v_max(v_max(r0, r1), r2), r3));
        प्रति (; j < n; j++) {
            पूर्णाङ्क v = (पूर्णाङ्क)cv_absdiff(src1[j], src2[j]);
            s = std::max(s, v);
        }
        प्रत्यागम s;
    }
};

प्रतिरूपम्<>
संरचना NormDiffInf_SIMD<schar, पूर्णाङ्क> {
    पूर्णाङ्क संक्रियकः() (ध्रुव schar* src1, ध्रुव schar* src2, पूर्णाङ्क n) ध्रुव {
        पूर्णाङ्क j = 0;
        पूर्णाङ्क s = 0;
        v_uint8 r0 = vx_setzero_u8(), r1 = vx_setzero_u8();
        v_uint8 r2 = vx_setzero_u8(), r3 = vx_setzero_u8();
        प्रति (; j <= n - 4 * VTraits<v_int8>::vlanes(); j += 4 * VTraits<v_int8>::vlanes()) {
            v_int8 v01 = vx_आरोपय(src1 + j), v02 = vx_आरोपय(src2 + j);
            r0 = v_max(r0, v_absdiff(v01, v02));

            v_int8 v11 = vx_आरोपय(src1 + j + VTraits<v_int8>::vlanes()),
                   v12 = vx_आरोपय(src2 + j + VTraits<v_int8>::vlanes());
            r1 = v_max(r1, v_absdiff(v11, v12));

            v_int8 v21 = vx_आरोपय(src1 + j + 2 * VTraits<v_int8>::vlanes()),
                   v22 = vx_आरोपय(src2 + j + 2 * VTraits<v_int8>::vlanes());
            r2 = v_max(r2, v_absdiff(v21, v22));

            v_int8 v31 = vx_आरोपय(src1 + j + 3 * VTraits<v_int8>::vlanes()),
                   v32 = vx_आरोपय(src2 + j + 3 * VTraits<v_int8>::vlanes());
            r3 = v_max(r3, v_absdiff(v31, v32));
        }
        s = (पूर्णाङ्क)v_reduce_max(v_max(v_max(v_max(r0, r1), r2), r3));
        प्रति (; j < n; j++) {
            पूर्णाङ्क v = (पूर्णाङ्क)cv_absdiff(src1[j], src2[j]);
            s = std::max(s, v);
        }
        प्रत्यागम s;
    }
};

प्रतिरूपम्<>
संरचना NormDiffInf_SIMD<ushort, पूर्णाङ्क> {
    पूर्णाङ्क संक्रियकः() (ध्रुव ushort* src1, ध्रुव ushort* src2, पूर्णाङ्क n) ध्रुव {
        पूर्णाङ्क j = 0;
        पूर्णाङ्क s = 0;
        v_uint16 r0 = vx_setzero_u16(), r1 = vx_setzero_u16();
        v_uint16 r2 = vx_setzero_u16(), r3 = vx_setzero_u16();
        प्रति (; j <= n - 4 * VTraits<v_uint16>::vlanes(); j += 4 * VTraits<v_uint16>::vlanes()) {
            v_uint16 v01 = vx_आरोपय(src1 + j), v02 = vx_आरोपय(src2 + j);
            r0 = v_max(r0, v_absdiff(v01, v02));

            v_uint16 v11 = vx_आरोपय(src1 + j + VTraits<v_uint16>::vlanes()),
                     v12 = vx_आरोपय(src2 + j + VTraits<v_uint16>::vlanes());
            r1 = v_max(r1, v_absdiff(v11, v12));

            v_uint16 v21 = vx_आरोपय(src1 + j + 2 * VTraits<v_uint16>::vlanes()),
                     v22 = vx_आरोपय(src2 + j + 2 * VTraits<v_uint16>::vlanes());
            r2 = v_max(r2, v_absdiff(v21, v22));

            v_uint16 v31 = vx_आरोपय(src1 + j + 3 * VTraits<v_uint16>::vlanes()),
                     v32 = vx_आरोपय(src2 + j + 3 * VTraits<v_uint16>::vlanes());
            r3 = v_max(r3, v_absdiff(v31, v32));
        }
        s = (पूर्णाङ्क)v_reduce_max(v_max(v_max(v_max(r0, r1), r2), r3));
        प्रति (; j < n; j++) {
            पूर्णाङ्क v = (पूर्णाङ्क)cv_absdiff(src1[j], src2[j]);
            s = std::max(s, v);
        }
        प्रत्यागम s;
    }
};

प्रतिरूपम्<>
संरचना NormDiffInf_SIMD<ह्रस्व, पूर्णाङ्क> {
    पूर्णाङ्क संक्रियकः() (ध्रुव ह्रस्व* src1, ध्रुव ह्रस्व* src2, पूर्णाङ्क n) ध्रुव {
        पूर्णाङ्क j = 0;
        पूर्णाङ्क s = 0;
        v_uint16 r0 = vx_setzero_u16(), r1 = vx_setzero_u16();
        v_uint16 r2 = vx_setzero_u16(), r3 = vx_setzero_u16();
        प्रति (; j <= n - 4 * VTraits<v_int16>::vlanes(); j += 4 * VTraits<v_int16>::vlanes()) {
            v_int16 v01 = vx_आरोपय(src1 + j), v02 = vx_आरोपय(src2 + j);
            r0 = v_max(r0, v_absdiff(v01, v02));

            v_int16 v11 = vx_आरोपय(src1 + j + VTraits<v_int16>::vlanes()),
                    v12 = vx_आरोपय(src2 + j + VTraits<v_int16>::vlanes());
            r1 = v_max(r1, v_absdiff(v11, v12));

            v_int16 v21 = vx_आरोपय(src1 + j + 2 * VTraits<v_int16>::vlanes()),
                    v22 = vx_आरोपय(src2 + j + 2 * VTraits<v_int16>::vlanes());
            r2 = v_max(r2, v_absdiff(v21, v22));

            v_int16 v31 = vx_आरोपय(src1 + j + 3 * VTraits<v_int16>::vlanes()),
                    v32 = vx_आरोपय(src2 + j + 3 * VTraits<v_int16>::vlanes());
            r3 = v_max(r3, v_absdiff(v31, v32));
        }
        s = (पूर्णाङ्क)v_reduce_max(v_max(v_max(v_max(r0, r1), r2), r3));
        प्रति (; j < n; j++) {
            पूर्णाङ्क v = (पूर्णाङ्क)cv_absdiff(src1[j], src2[j]);
            s = std::max(s, v);
        }
        प्रत्यागम s;
    }
};

प्रतिरूपम्<>
संरचना NormDiffInf_SIMD<पूर्णाङ्क, अचिह्नित> {
    अचिह्नित संक्रियकः() (ध्रुव पूर्णाङ्क* src1, ध्रुव पूर्णाङ्क* src2, पूर्णाङ्क n) ध्रुव {
        पूर्णाङ्क j = 0;
        अचिह्नित s = 0;
        v_uint32 r0 = vx_setzero_u32(), r1 = vx_setzero_u32();
        v_uint32 r2 = vx_setzero_u32(), r3 = vx_setzero_u32();
        प्रति (; j <= n - 4 * VTraits<v_int32>::vlanes(); j += 4 * VTraits<v_int32>::vlanes()) {
            v_int32 v01 = vx_आरोपय(src1 + j), v02 = vx_आरोपय(src2 + j);
            r0 = v_max(r0, v_absdiff(v01, v02));

            v_int32 v11 = vx_आरोपय(src1 + j + VTraits<v_int32>::vlanes()),
                    v12 = vx_आरोपय(src2 + j + VTraits<v_int32>::vlanes());
            r1 = v_max(r1, v_absdiff(v11, v12));

            v_int32 v21 = vx_आरोपय(src1 + j + 2 * VTraits<v_int32>::vlanes()),
                    v22 = vx_आरोपय(src2 + j + 2 * VTraits<v_int32>::vlanes());
            r2 = v_max(r2, v_absdiff(v21, v22));

            v_int32 v31 = vx_आरोपय(src1 + j + 3 * VTraits<v_int32>::vlanes()),
                    v32 = vx_आरोपय(src2 + j + 3 * VTraits<v_int32>::vlanes());
            r3 = v_max(r3, v_absdiff(v31, v32));
        }
        s = (पूर्णाङ्क)v_reduce_max(v_max(v_max(v_max(r0, r1), r2), r3));
        प्रति (; j < n; j++) {
            अचिह्नित v = (अचिह्नित)cv_absdiff(src1[j], src2[j]);
            s = std::max(s, v);
        }
        प्रत्यागम s;
    }
};

प्रतिरूपम्<>
संरचना NormDiffInf_SIMD<भिन्न, भिन्न> {
    भिन्न संक्रियकः() (ध्रुव भिन्न* src1, ध्रुव भिन्न* src2, पूर्णाङ्क n) ध्रुव {
        पूर्णाङ्क j = 0;
        भिन्न s = 0;
        v_float32 r0 = vx_setzero_f32(), r1 = vx_setzero_f32();
        प्रति (; j <= n - 2 * VTraits<v_float32>::vlanes(); j += 2 * VTraits<v_float32>::vlanes()) {
            v_float32 v01 = vx_आरोपय(src1 + j), v02 = vx_आरोपय(src2 + j);
            r0 = v_max(r0, v_absdiff(v01, v02));

            v_float32 v11 = vx_आरोपय(src1 + j + VTraits<v_float32>::vlanes()),
                      v12 = vx_आरोपय(src2 + j + VTraits<v_float32>::vlanes());
            r1 = v_max(r1, v_absdiff(v11, v12));
        }
        s = v_reduce_max(v_max(r0, r1));
        प्रति (; j < n; j++) {
            भिन्न v = (भिन्न)cv_absdiff(src1[j], src2[j]);
            s = std::max(s, v);
        }
        प्रत्यागम s;
    }
};

प्रतिरूपम्<>
संरचना NormDiffL1_SIMD<uchar, पूर्णाङ्क> {
    पूर्णाङ्क संक्रियकः() (ध्रुव uchar* src1, ध्रुव uchar* src2, पूर्णाङ्क n) ध्रुव {
        पूर्णाङ्क j = 0;
        पूर्णाङ्क s = 0;
        v_uint32 r0 = vx_setzero_u32(), r1 = vx_setzero_u32();
        v_uint8  one = vx_setall_u8(1);
        प्रति (; j<= n - 2 * VTraits<v_uint8>::vlanes(); j += 2 * VTraits<v_uint8>::vlanes()) {
            v_uint8 v01 = vx_आरोपय(src1 + j), v02 = vx_आरोपय(src2 + j);
            r0 = v_dotprod_expand_fast(v_absdiff(v01, v02), one, r0);

            v_uint8 v11 = vx_आरोपय(src1 + j + VTraits<v_uint8>::vlanes()),
                    v12 = vx_आरोपय(src2 + j + VTraits<v_uint8>::vlanes());
            r1 = v_dotprod_expand_fast(v_absdiff(v11, v12), one, r1);
        }
        s += v_reduce_योग(v_सङ्कलय(r0, r1));
        प्रति (; j < n; j++) {
            पूर्णाङ्क v = (पूर्णाङ्क)cv_absdiff(src1[j], src2[j]);
            s += v;
        }
        प्रत्यागम s;
    }
};

प्रतिरूपम्<>
संरचना NormDiffL1_SIMD<schar, पूर्णाङ्क> {
    पूर्णाङ्क संक्रियकः() (ध्रुव schar* src1, ध्रुव schar* src2, पूर्णाङ्क n) ध्रुव {
        पूर्णाङ्क j = 0;
        पूर्णाङ्क s = 0;
        v_uint32 r0 = vx_setzero_u32(), r1 = vx_setzero_u32();
        v_uint8  one = vx_setall_u8(1);
        प्रति (; j<= n - 2 * VTraits<v_int8>::vlanes(); j += 2 * VTraits<v_int8>::vlanes()) {
            v_int8 v01 = vx_आरोपय(src1 + j), v02 = vx_आरोपय(src2 + j);
            r0 = v_dotprod_expand_fast(v_absdiff(v01, v02), one, r0);

            v_int8 v11 = vx_आरोपय(src1 + j + VTraits<v_int8>::vlanes()),
                   v12 = vx_आरोपय(src2 + j + VTraits<v_int8>::vlanes());
            r1 = v_dotprod_expand_fast(v_absdiff(v11, v12), one, r1);
        }
        s += v_reduce_योग(v_सङ्कलय(r0, r1));
        प्रति (; j < n; j++) {
            पूर्णाङ्क v = (पूर्णाङ्क)cv_absdiff(src1[j], src2[j]);
            s += v;
        }
        प्रत्यागम s;
    }
};

प्रतिरूपम्<>
संरचना NormDiffL1_SIMD<ushort, पूर्णाङ्क> {
    पूर्णाङ्क संक्रियकः() (ध्रुव ushort* src1, ध्रुव ushort* src2, पूर्णाङ्क n) ध्रुव {
        पूर्णाङ्क j = 0;
        पूर्णाङ्क s = 0;
        v_uint32 r0 = vx_setzero_u32(), r1 = vx_setzero_u32();
        v_uint32 r2 = vx_setzero_u32(), r3 = vx_setzero_u32();
        प्रति (; j<= n - 4 * VTraits<v_uint16>::vlanes(); j += 4 * VTraits<v_uint16>::vlanes()) {
            v_uint16 v01 = vx_आरोपय(src1 + j), v02 = vx_आरोपय(src2 + j);
            v_uint32 u00, u01;
            v_expand(v_absdiff(v01, v02), u00, u01);
            r0 = v_सङ्कलय(r0, v_सङ्कलय(u00, u01));

            v_uint16 v11 = vx_आरोपय(src1 + j + VTraits<v_uint16>::vlanes()),
                     v12 = vx_आरोपय(src2 + j + VTraits<v_uint16>::vlanes());
            v_uint32 u10, u11;
            v_expand(v_absdiff(v11, v12), u10, u11);
            r1 = v_सङ्कलय(r1, v_सङ्कलय(u10, u11));

            v_uint16 v21 = vx_आरोपय(src1 + j + 2 * VTraits<v_uint16>::vlanes()),
                     v22 = vx_आरोपय(src2 + j + 2 * VTraits<v_uint16>::vlanes());
            v_uint32 u20, u21;
            v_expand(v_absdiff(v21, v22), u20, u21);
            r2 = v_सङ्कलय(r2, v_सङ्कलय(u20, u21));

            v_uint16 v31 = vx_आरोपय(src1 + j + 3 * VTraits<v_uint16>::vlanes()),
                     v32 = vx_आरोपय(src2 + j + 3 * VTraits<v_uint16>::vlanes());
            v_uint32 u30, u31;
            v_expand(v_absdiff(v31, v32), u30, u31);
            r3 = v_सङ्कलय(r3, v_सङ्कलय(u30, u31));
        }
        s += (पूर्णाङ्क)v_reduce_योग(v_सङ्कलय(v_सङ्कलय(v_सङ्कलय(r0, r1), r2), r3));
        प्रति (; j < n; j++) {
            पूर्णाङ्क v = (पूर्णाङ्क)cv_absdiff(src1[j], src2[j]);
            s += v;
        }
        प्रत्यागम s;
    }
};

प्रतिरूपम्<>
संरचना NormDiffL1_SIMD<ह्रस्व, पूर्णाङ्क> {
    पूर्णाङ्क संक्रियकः() (ध्रुव ह्रस्व* src1, ध्रुव ह्रस्व* src2, पूर्णाङ्क n) ध्रुव {
        पूर्णाङ्क j = 0;
        पूर्णाङ्क s = 0;
        v_uint32 r0 = vx_setzero_u32(), r1 = vx_setzero_u32();
        v_uint32 r2 = vx_setzero_u32(), r3 = vx_setzero_u32();
        प्रति (; j<= n - 4 * VTraits<v_int16>::vlanes(); j += 4 * VTraits<v_int16>::vlanes()) {
            v_int16 v01 = vx_आरोपय(src1 + j), v02 = vx_आरोपय(src2 + j);
            v_uint32 u00, u01;
            v_expand(v_absdiff(v01, v02), u00, u01);
            r0 = v_सङ्कलय(r0, v_सङ्कलय(u00, u01));

            v_int16 v11 = vx_आरोपय(src1 + j + VTraits<v_int16>::vlanes()),
                    v12 = vx_आरोपय(src2 + j + VTraits<v_int16>::vlanes());
            v_uint32 u10, u11;
            v_expand(v_absdiff(v11, v12), u10, u11);
            r1 = v_सङ्कलय(r1, v_सङ्कलय(u10, u11));

            v_int16 v21 = vx_आरोपय(src1 + j + 2 * VTraits<v_int16>::vlanes()),
                    v22 = vx_आरोपय(src2 + j + 2 * VTraits<v_int16>::vlanes());
            v_uint32 u20, u21;
            v_expand(v_absdiff(v21, v22), u20, u21);
            r2 = v_सङ्कलय(r2, v_सङ्कलय(u20, u21));

            v_int16 v31 = vx_आरोपय(src1 + j + 3 * VTraits<v_int16>::vlanes()),
                    v32 = vx_आरोपय(src2 + j + 3 * VTraits<v_int16>::vlanes());
            v_uint32 u30, u31;
            v_expand(v_absdiff(v31, v32), u30, u31);
            r3 = v_सङ्कलय(r3, v_सङ्कलय(u30, u31));
        }
        s += (पूर्णाङ्क)v_reduce_योग(v_सङ्कलय(v_सङ्कलय(v_सङ्कलय(r0, r1), r2), r3));
        प्रति (; j < n; j++) {
            पूर्णाङ्क v = (पूर्णाङ्क)cv_absdiff(src1[j], src2[j]);
            s += v;
        }
        प्रत्यागम s;
    }
};

प्रतिरूपम्<>
संरचना NormDiffL2_SIMD<uchar, पूर्णाङ्क> {
    पूर्णाङ्क संक्रियकः() (ध्रुव uchar* src1, ध्रुव uchar* src2, पूर्णाङ्क n) ध्रुव {
        पूर्णाङ्क j = 0;
        पूर्णाङ्क s = 0;
        v_uint32 r0 = vx_setzero_u32(), r1 = vx_setzero_u32();
        प्रति (; j <= n - 2 * VTraits<v_uint8>::vlanes(); j += 2 * VTraits<v_uint8>::vlanes()) {
            v_uint8 v01 = vx_आरोपय(src1 + j), v02 = vx_आरोपय(src2 + j);
            v_uint8 v0 = v_absdiff(v01, v02);
            r0 = v_dotprod_expand_fast(v0, v0, r0);

            v_uint8 v11 = vx_आरोपय(src1 + j + VTraits<v_uint8>::vlanes()),
                    v12 = vx_आरोपय(src2 + j + VTraits<v_uint8>::vlanes());
            v_uint8 v1 = v_absdiff(v11, v12);
            r1 = v_dotprod_expand_fast(v1, v1, r1);
        }
        s += v_reduce_योग(v_सङ्कलय(r0, r1));
        प्रति (; j < n; j++) {
            पूर्णाङ्क v = (पूर्णाङ्क)src1[j] - (पूर्णाङ्क)src2[j];
            s += v * v;
        }
        प्रत्यागम s;
    }
};

प्रतिरूपम्<>
संरचना NormDiffL2_SIMD<schar, पूर्णाङ्क> {
    पूर्णाङ्क संक्रियकः() (ध्रुव schar* src1, ध्रुव schar* src2, पूर्णाङ्क n) ध्रुव {
        पूर्णाङ्क j = 0;
        पूर्णाङ्क s = 0;
        v_uint32 r0 = vx_setzero_u32(), r1 = vx_setzero_u32();
        प्रति (; j <= n - 2 * VTraits<v_int8>::vlanes(); j += 2 * VTraits<v_int8>::vlanes()) {
            v_int8 v01 = vx_आरोपय(src1 + j), v02 = vx_आरोपय(src2 + j);
            v_uint8 v0 = v_absdiff(v01, v02);
            r0 = v_dotprod_expand_fast(v0, v0, r0);

            v_int8 v11 = vx_आरोपय(src1 + j + VTraits<v_int8>::vlanes()),
                   v12 = vx_आरोपय(src2 + j + VTraits<v_int8>::vlanes());
            v_uint8 v1 = v_absdiff(v11, v12);
            r1 = v_dotprod_expand_fast(v1, v1, r1);
        }
        s += v_reduce_योग(v_सङ्कलय(r0, r1));
        प्रति (; j < n; j++) {
            पूर्णाङ्क v = (पूर्णाङ्क)src1[j] - (पूर्णाङ्क)src2[j];
            s += v * v;
        }
        प्रत्यागम s;
    }
};

#यदि_अन्तः

#यदि (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)

प्रतिरूपम्<>
संरचना NormInf_SIMD<द्विगुण, द्विगुण> {
    द्विगुण संक्रियकः() (ध्रुव द्विगुण* src, पूर्णाङ्क n) ध्रुव {
        पूर्णाङ्क j = 0;
        द्विगुण s = 0.f;
        v_float64 r0 = vx_setzero_f64(), r1 = vx_setzero_f64();
        v_float64 r2 = vx_setzero_f64(), r3 = vx_setzero_f64();
        प्रति (; j <= n - 4 * VTraits<v_float64>::vlanes(); j += 4 * VTraits<v_float64>::vlanes()) {
            r0 = v_max(r0, v_abs(vx_आरोपय(src + j                                   )));
            r1 = v_max(r1, v_abs(vx_आरोपय(src + j +     VTraits<v_float64>::vlanes())));
            r2 = v_max(r2, v_abs(vx_आरोपय(src + j + 2 * VTraits<v_float64>::vlanes())));
            r3 = v_max(r3, v_abs(vx_आरोपय(src + j + 3 * VTraits<v_float64>::vlanes())));
        }
        r0 = v_max(r0, v_max(r1, v_max(r2, r3)));
        प्रति (; j < n; j++) {
            s = std::max(s, cv_abs(src[j]));
        }
        // [TODO]: use v_reduce_max when it supports float64
        द्विगुण t[VTraits<v_float64>::max_nlanes];
        vx_store(t, r0);
        प्रति (पूर्णाङ्क i = 0; i < VTraits<v_float64>::vlanes(); i++) {
            s = std::max(s, cv_abs(t[i]));
        }
        प्रत्यागम s;
    }
};

प्रतिरूपम्<>
संरचना NormL1_SIMD<पूर्णाङ्क, द्विगुण> {
    द्विगुण संक्रियकः() (ध्रुव पूर्णाङ्क* src, पूर्णाङ्क n) ध्रुव {
        पूर्णाङ्क j = 0;
        द्विगुण s = 0.f;
        v_float64 r00 = vx_setzero_f64(), r01 = vx_setzero_f64();
        v_float64 r10 = vx_setzero_f64(), r11 = vx_setzero_f64();
        प्रति (; j <= n - 2 * VTraits<v_int32>::vlanes(); j += 2 * VTraits<v_int32>::vlanes()) {
            // Widen to double before taking the magnitude: exact over the whole int32 range,
            // INT_MIN included, which a float intermediate would not be above 2^24.
            v_int32 i0 = vx_आरोपय(src + j), i1 = vx_आरोपय(src + j + VTraits<v_int32>::vlanes());
            r00 = v_सङ्कलय(r00, v_abs(v_cvt_f64(i0))); r01 = v_सङ्कलय(r01, v_abs(v_cvt_f64_उच्चः(i0)));
            r10 = v_सङ्कलय(r10, v_abs(v_cvt_f64(i1))); r11 = v_सङ्कलय(r11, v_abs(v_cvt_f64_उच्चः(i1)));
        }
        s += v_reduce_योग(v_सङ्कलय(v_सङ्कलय(v_सङ्कलय(r00, r01), r10), r11));
        प्रति (; j < n; j++) {
            s += cv_abs(src[j]);
        }
        प्रत्यागम s;
    }
};

प्रतिरूपम्<>
संरचना NormL1_SIMD<भिन्न, द्विगुण> {
    द्विगुण संक्रियकः() (ध्रुव भिन्न* src, पूर्णाङ्क n) ध्रुव {
        पूर्णाङ्क j = 0;
        द्विगुण s = 0.f;
        v_float64 r00 = vx_setzero_f64(), r01 = vx_setzero_f64();
        v_float64 r10 = vx_setzero_f64(), r11 = vx_setzero_f64();
        v_float64 r20 = vx_setzero_f64(), r21 = vx_setzero_f64();
        v_float64 r30 = vx_setzero_f64(), r31 = vx_setzero_f64();
        प्रति (; j <= n - 4 * VTraits<v_float32>::vlanes(); j += 4 * VTraits<v_float32>::vlanes()) {
            v_float32 v0 = v_abs(vx_आरोपय(src + j)), v1 = v_abs(vx_आरोपय(src + j + VTraits<v_float32>::vlanes()));
            r00 = v_सङ्कलय(r00, v_cvt_f64(v0)); r01 = v_सङ्कलय(r01, v_cvt_f64_उच्चः(v0));
            r10 = v_सङ्कलय(r10, v_cvt_f64(v1)); r11 = v_सङ्कलय(r11, v_cvt_f64_उच्चः(v1));

            v_float32 v2 = v_abs(vx_आरोपय(src + j + 2 * VTraits<v_float32>::vlanes())), v3 = v_abs(vx_आरोपय(src + j + 3 * VTraits<v_float32>::vlanes()));
            r20 = v_सङ्कलय(r20, v_cvt_f64(v2)); r21 = v_सङ्कलय(r21, v_cvt_f64_उच्चः(v2));
            r30 = v_सङ्कलय(r30, v_cvt_f64(v3)); r31 = v_सङ्कलय(r31, v_cvt_f64_उच्चः(v3));
        }
        s += v_reduce_योग(v_सङ्कलय(v_सङ्कलय(v_सङ्कलय(r00, r01), r10), r11));
        s += v_reduce_योग(v_सङ्कलय(v_सङ्कलय(v_सङ्कलय(r20, r21), r30), r31));
        प्रति (; j < n; j++) {
            s += cv_abs(src[j]);
        }
        प्रत्यागम s;
    }
};

प्रतिरूपम्<>
संरचना NormL2_SIMD<ushort, द्विगुण> {
    द्विगुण संक्रियकः() (ध्रुव ushort* src, पूर्णाङ्क n) ध्रुव {
        पूर्णाङ्क j = 0;
        द्विगुण s = 0.f;
        v_float64 r0 = vx_setzero_f64(), r1 = vx_setzero_f64();
        प्रति (; j <= n - 2 * VTraits<v_uint16>::vlanes(); j += 2 * VTraits<v_uint16>::vlanes()) {
            v_uint16 v0 = vx_आरोपय(src + j);
            v_uint64 u0 = v_dotprod_expand_fast(v0, v0);
            r0 = v_सङ्कलय(r0, v_cvt_f64(v_reinterpret_as_s64(u0)));

            v_uint16 v1 = vx_आरोपय(src + j + VTraits<v_uint16>::vlanes());
            v_uint64 u1 = v_dotprod_expand_fast(v1, v1);
            r1 = v_सङ्कलय(r1, v_cvt_f64(v_reinterpret_as_s64(u1)));
        }
        s += v_reduce_योग(v_सङ्कलय(r0, r1));
        प्रति (; j < n; j++) {
            द्विगुण v = saturate_cast<द्विगुण>(src[j]);
            s += v * v;
        }
        प्रत्यागम s;
    }
};

प्रतिरूपम्<>
संरचना NormL2_SIMD<ह्रस्व, द्विगुण> {
    द्विगुण संक्रियकः() (ध्रुव ह्रस्व* src, पूर्णाङ्क n) ध्रुव {
        पूर्णाङ्क j = 0;
        द्विगुण s = 0.f;
        v_float64 r0 = vx_setzero_f64(), r1 = vx_setzero_f64();
        प्रति (; j <= n - 2 * VTraits<v_int16>::vlanes(); j += 2 * VTraits<v_int16>::vlanes()) {
            v_int16 v0 = vx_आरोपय(src + j);
            r0 = v_सङ्कलय(r0, v_cvt_f64(v_dotprod_expand_fast(v0, v0)));

            v_int16 v1 = vx_आरोपय(src + j + VTraits<v_int16>::vlanes());
            r1 = v_सङ्कलय(r1, v_cvt_f64(v_dotprod_expand_fast(v1, v1)));
        }
        s += v_reduce_योग(v_सङ्कलय(r0, r1));
        प्रति (; j < n; j++) {
            द्विगुण v = saturate_cast<द्विगुण>(src[j]);
            s += v * v;
        }
        प्रत्यागम s;
    }
};

प्रतिरूपम्<>
संरचना NormL2_SIMD<पूर्णाङ्क, द्विगुण> {
    द्विगुण संक्रियकः() (ध्रुव पूर्णाङ्क* src, पूर्णाङ्क n) ध्रुव {
        पूर्णाङ्क j = 0;
        द्विगुण s = 0.f;
        v_float64 r0 = vx_setzero_f64(), r1 = vx_setzero_f64();
        प्रति (; j <= n - 2 * VTraits<v_int32>::vlanes(); j += 2 * VTraits<v_int32>::vlanes()) {
            v_int32 v0 = vx_आरोपय(src + j);
            r0 = v_dotprod_expand_fast(v0, v0, r0);

            v_int32 v1 = vx_आरोपय(src + j + VTraits<v_int32>::vlanes());
            r1 = v_dotprod_expand_fast(v1, v1, r1);
        }
        s += v_reduce_योग(v_सङ्कलय(r0, r1));
        प्रति (; j < n; j++) {
            द्विगुण v = src[j];
            s += v * v;
        }
        प्रत्यागम s;
    }
};

प्रतिरूपम्<>
संरचना NormL2_SIMD<भिन्न, द्विगुण> {
    द्विगुण संक्रियकः() (ध्रुव भिन्न* src, पूर्णाङ्क n) ध्रुव {
        पूर्णाङ्क j = 0;
        द्विगुण s = 0.f;
        v_float64 r00 = vx_setzero_f64(), r01 = vx_setzero_f64();
        v_float64 r10 = vx_setzero_f64(), r11 = vx_setzero_f64();
        प्रति (; j <= n - 2 * VTraits<v_float32>::vlanes(); j += 2 * VTraits<v_float32>::vlanes()) {
            v_float32 v0 = vx_आरोपय(src + j), v1 = vx_आरोपय(src + j + VTraits<v_float32>::vlanes());
            v_float64 v00 = v_cvt_f64(v0), v01 = v_cvt_f64_उच्चः(v0);
            v_float64 v10 = v_cvt_f64(v1), v11 = v_cvt_f64_उच्चः(v1);
            r00 = v_fma(v00, v00, r00); r01 = v_fma(v01, v01, r01);
            r10 = v_fma(v10, v10, r10); r11 = v_fma(v11, v11, r11);
        }
        s += v_reduce_योग(v_सङ्कलय(v_सङ्कलय(v_सङ्कलय(r00, r01), r10), r11));
        प्रति (; j < n; j++) {
            द्विगुण v = src[j];
            s += v * v;
        }
        प्रत्यागम s;
    }
};

#यदि_अन्तः

#यदि CV_SIMD_64F // CV_SIMD_SCALABLE_64F has accuracy problem with the following kernels on ci

प्रतिरूपम्<>
संरचना NormL1_SIMD<द्विगुण, द्विगुण> {
    द्विगुण संक्रियकः() (ध्रुव द्विगुण* src, पूर्णाङ्क n) ध्रुव {
        पूर्णाङ्क j = 0;
        द्विगुण s = 0.f;
        v_float64 r00 = vx_setzero_f64(), r01 = vx_setzero_f64();
        v_float64 r10 = vx_setzero_f64(), r11 = vx_setzero_f64();
        प्रति (; j <= n - 4 * VTraits<v_float64>::vlanes(); j += 4 * VTraits<v_float64>::vlanes()) {
            r00 = v_सङ्कलय(r00, v_abs(vx_आरोपय(src + j                                   )));
            r01 = v_सङ्कलय(r01, v_abs(vx_आरोपय(src + j +     VTraits<v_float64>::vlanes())));
            r10 = v_सङ्कलय(r10, v_abs(vx_आरोपय(src + j + 2 * VTraits<v_float64>::vlanes())));
            r11 = v_सङ्कलय(r11, v_abs(vx_आरोपय(src + j + 3 * VTraits<v_float64>::vlanes())));
        }
        s += v_reduce_योग(v_सङ्कलय(v_सङ्कलय(v_सङ्कलय(r00, r01), r10), r11));
        प्रति (; j < n; j++) {
            s += cv_abs(src[j]);
        }
        प्रत्यागम s;
    }
};

प्रतिरूपम्<>
संरचना NormL2_SIMD<द्विगुण, द्विगुण> {
    द्विगुण संक्रियकः() (ध्रुव द्विगुण* src, पूर्णाङ्क n) ध्रुव {
        पूर्णाङ्क j = 0;
        द्विगुण s = 0.f;
        v_float64 r00 = vx_setzero_f64(), r01 = vx_setzero_f64();
        v_float64 r10 = vx_setzero_f64(), r11 = vx_setzero_f64();
        प्रति (; j <= n - 4 * VTraits<v_float64>::vlanes(); j += 4 * VTraits<v_float64>::vlanes()) {
            v_float64 v00 = vx_आरोपय(src + j                                   );
            v_float64 v01 = vx_आरोपय(src + j +     VTraits<v_float64>::vlanes());
            v_float64 v10 = vx_आरोपय(src + j + 2 * VTraits<v_float64>::vlanes());
            v_float64 v11 = vx_आरोपय(src + j + 3 * VTraits<v_float64>::vlanes());
            r00 = v_fma(v00, v00, r00); r01 = v_fma(v01, v01, r01);
            r10 = v_fma(v10, v10, r10); r11 = v_fma(v11, v11, r11);
        }
        s += v_reduce_योग(v_सङ्कलय(v_सङ्कलय(v_सङ्कलय(r00, r01), r10), r11));
        प्रति (; j < n; j++) {
            द्विगुण v = src[j];
            s += v * v;
        }
        प्रत्यागम s;
    }
};

प्रतिरूपम्<>
संरचना NormDiffInf_SIMD<द्विगुण, द्विगुण> {
    द्विगुण संक्रियकः() (ध्रुव द्विगुण* src1, ध्रुव द्विगुण* src2, पूर्णाङ्क n) ध्रुव {
        पूर्णाङ्क j = 0;
        द्विगुण s = 0;
        v_float64 r0 = vx_setzero_f64(), r1 = vx_setzero_f64();
        प्रति (; j <= n - 2 * VTraits<v_float64>::vlanes(); j += 2 * VTraits<v_float64>::vlanes()) {
            v_float64 v01 = vx_आरोपय(src1 + j), v02 = vx_आरोपय(src2 + j);
            r0 = v_max(r0, v_absdiff(v01, v02));

            v_float64 v11 = vx_आरोपय(src1 + j + VTraits<v_float64>::vlanes()),
                      v12 = vx_आरोपय(src2 + j + VTraits<v_float64>::vlanes());
            r1 = v_max(r1, v_absdiff(v11, v12));
        }
        // [TODO]: use v_reduce_max when it supports float64
        द्विगुण t[VTraits<v_float64>::max_nlanes];
        vx_store(t, v_max(r0, r1));
        प्रति (पूर्णाङ्क i = 0; i < VTraits<v_float64>::vlanes(); i++) {
            s = std::max(s, t[i]);
        }
        प्रति (; j < n; j++) {
            द्विगुण v = (द्विगुण)cv_absdiff(src1[j], src2[j]);
            s = std::max(s, v);
        }
        प्रत्यागम s;
    }
};

प्रतिरूपम्<>
संरचना NormDiffL1_SIMD<भिन्न, द्विगुण> {
    द्विगुण संक्रियकः() (ध्रुव भिन्न* src1, ध्रुव भिन्न* src2, पूर्णाङ्क n) ध्रुव {
        पूर्णाङ्क j = 0;
        द्विगुण s = 0.f;
        v_float64 r0 = vx_setzero_f64(), r1 = vx_setzero_f64();
        v_float64 r2 = vx_setzero_f64(), r3 = vx_setzero_f64();
        प्रति (; j <= n - 2 * VTraits<v_float32>::vlanes(); j += 2 * VTraits<v_float32>::vlanes()) {
            v_float32 v01 = vx_आरोपय(src1 + j), v02 = vx_आरोपय(src2 + j);
            v_float32 v0 = v_absdiff(v01, v02);
            r0 = v_सङ्कलय(r0, v_cvt_f64(v0)); r1 = v_सङ्कलय(r1, v_cvt_f64_उच्चः(v0));

            v_float32 v11 = vx_आरोपय(src1 + j + VTraits<v_float32>::vlanes()),
                      v12 = vx_आरोपय(src2 + j + VTraits<v_float32>::vlanes());
            v_float32 v1 = v_absdiff(v11, v12);
            r2 = v_सङ्कलय(r2, v_cvt_f64(v1)); r3 = v_सङ्कलय(r3, v_cvt_f64_उच्चः(v1));
        }
        s += v_reduce_योग(v_सङ्कलय(v_सङ्कलय(v_सङ्कलय(r0, r1), r2), r3));
        प्रति (; j < n; j++) {
            द्विगुण v = (द्विगुण)cv_absdiff(src1[j], src2[j]);
            s += v;
        }
        प्रत्यागम s;
    }
};

प्रतिरूपम्<>
संरचना NormDiffL1_SIMD<द्विगुण, द्विगुण> {
    द्विगुण संक्रियकः() (ध्रुव द्विगुण* src1, ध्रुव द्विगुण* src2, पूर्णाङ्क n) ध्रुव {
        पूर्णाङ्क j = 0;
        द्विगुण s = 0.f;
        v_float64 r0 = vx_setzero_f64(), r1 = vx_setzero_f64();
        प्रति (; j <= n - 2 * VTraits<v_float64>::vlanes(); j += 2 * VTraits<v_float64>::vlanes()) {
            v_float64 v01 = vx_आरोपय(src1 + j), v02 = vx_आरोपय(src2 + j);
            r0 = v_सङ्कलय(r0, v_absdiff(v01, v02));

            v_float64 v11 = vx_आरोपय(src1 + j + VTraits<v_float64>::vlanes()),
                      v12 = vx_आरोपय(src2 + j + VTraits<v_float64>::vlanes());
            r1 = v_सङ्कलय(r1, v_absdiff(v11, v12));
        }
        s += v_reduce_योग(v_सङ्कलय(r0, r1));
        प्रति (; j < n; j++) {
            द्विगुण v = (द्विगुण)cv_absdiff(src1[j], src2[j]);
            s += v;
        }
        प्रत्यागम s;
    }
};

प्रतिरूपम्<>
संरचना NormDiffL2_SIMD<ushort, द्विगुण> {
    द्विगुण संक्रियकः() (ध्रुव ushort* src1, ध्रुव ushort* src2, पूर्णाङ्क n) ध्रुव {
        पूर्णाङ्क j = 0;
        द्विगुण s = 0.f;
        v_float64 r0 = vx_setzero_f64(), r1 = vx_setzero_f64();
        प्रति (; j <= n - 2 * VTraits<v_uint16>::vlanes(); j += 2 * VTraits<v_uint16>::vlanes()) {
            v_uint16 v01 = vx_आरोपय(src1 + j), v02 = vx_आरोपय(src2 + j);
            v_uint16 v0 = v_absdiff(v01, v02);
            v_uint64 u0 = v_dotprod_expand_fast(v0, v0);
            r0 = v_सङ्कलय(r0, v_cvt_f64(v_reinterpret_as_s64(u0)));

            v_uint16 v11 = vx_आरोपय(src1 + j + VTraits<v_uint16>::vlanes()),
                     v12 = vx_आरोपय(src2 + j + VTraits<v_uint16>::vlanes());
            v_uint16 v1 = v_absdiff(v11, v12);
            v_uint64 u1 = v_dotprod_expand_fast(v1, v1);
            r1 = v_सङ्कलय(r1, v_cvt_f64(v_reinterpret_as_s64(u1)));
        }
        s += v_reduce_योग(v_सङ्कलय(r0, r1));
        प्रति (; j < n; j++) {
            द्विगुण v = (द्विगुण)src1[j] - (द्विगुण)src2[j];
            s += v * v;
        }
        प्रत्यागम s;
    }
};

प्रतिरूपम्<>
संरचना NormDiffL2_SIMD<ह्रस्व, द्विगुण> {
    द्विगुण संक्रियकः() (ध्रुव ह्रस्व* src1, ध्रुव ह्रस्व* src2, पूर्णाङ्क n) ध्रुव {
        पूर्णाङ्क j = 0;
        द्विगुण s = 0.f;
        v_float64 r0 = vx_setzero_f64(), r1 = vx_setzero_f64();
        प्रति (; j <= n - 2 * VTraits<v_int16>::vlanes(); j += 2 * VTraits<v_int16>::vlanes()) {
            v_int16 v01 = vx_आरोपय(src1 + j), v02 = vx_आरोपय(src2 + j);
            v_uint16 v0 = v_absdiff(v01, v02);
            v_uint64 u0 = v_dotprod_expand_fast(v0, v0);
            r0 = v_सङ्कलय(r0, v_cvt_f64(v_reinterpret_as_s64(u0)));

            v_int16 v11 = vx_आरोपय(src1 + j + VTraits<v_uint16>::vlanes()),
                    v12 = vx_आरोपय(src2 + j + VTraits<v_uint16>::vlanes());
            v_uint16 v1 = v_absdiff(v11, v12);
            v_uint64 u1 = v_dotprod_expand_fast(v1, v1);
            r1 = v_सङ्कलय(r1, v_cvt_f64(v_reinterpret_as_s64(u1)));
        }
        s += v_reduce_योग(v_सङ्कलय(r0, r1));
        प्रति (; j < n; j++) {
            द्विगुण v = (द्विगुण)src1[j] - (द्विगुण)src2[j];
            s += v * v;
        }
        प्रत्यागम s;
    }
};

प्रतिरूपम्<>
संरचना NormDiffL2_SIMD<पूर्णाङ्क, द्विगुण> {
    द्विगुण संक्रियकः() (ध्रुव पूर्णाङ्क* src1, ध्रुव पूर्णाङ्क* src2, पूर्णाङ्क n) ध्रुव {
        पूर्णाङ्क j = 0;
        द्विगुण s = 0.f;
        v_float64 r0 = vx_setzero_f64(), r1 = vx_setzero_f64();
        प्रति (; j <= n - VTraits<v_int32>::vlanes(); j += VTraits<v_int32>::vlanes()) {
            v_int32 v01 = vx_आरोपय(src1 + j), v02 = vx_आरोपय(src2 + j);
            v_uint32 v0 = v_absdiff(v01, v02);
            v_uint64 ev0, ev1;
            v_expand(v0, ev0, ev1);
            v_float64 f0 = v_cvt_f64(v_reinterpret_as_s64(ev0)), f1 = v_cvt_f64(v_reinterpret_as_s64(ev1));
            r0 = v_fma(f0, f0, r0); r1 = v_fma(f1, f1, r1);
        }
        s += v_reduce_योग(v_सङ्कलय(r0, r1));
        प्रति (; j < n; j++) {
            द्विगुण v = (द्विगुण)src1[j] - (द्विगुण)src2[j];
            s += v * v;
        }
        प्रत्यागम s;
    }
};

प्रतिरूपम्<>
संरचना NormDiffL2_SIMD<भिन्न, द्विगुण> {
    द्विगुण संक्रियकः() (ध्रुव भिन्न* src1, ध्रुव भिन्न* src2, पूर्णाङ्क n) ध्रुव {
        पूर्णाङ्क j = 0;
        द्विगुण s = 0.f;
        v_float64 r0 = vx_setzero_f64(), r1 = vx_setzero_f64();
        v_float64 r2 = vx_setzero_f64(), r3 = vx_setzero_f64();
        प्रति (; j <= n - 2 * VTraits<v_float32>::vlanes(); j += 2 * VTraits<v_float32>::vlanes()) {
            v_float32 v01 = vx_आरोपय(src1 + j), v02 = vx_आरोपय(src2 + j);
            v_float32 v0 = v_absdiff(v01, v02);
            v_float64 f01 = v_cvt_f64(v0), f02 = v_cvt_f64_उच्चः(v0);
            r0 = v_fma(f01, f01, r0); r1 = v_fma(f02, f02, r1);

            v_float32 v11 = vx_आरोपय(src1 + j + VTraits<v_float32>::vlanes()),
                      v12 = vx_आरोपय(src2 + j + VTraits<v_float32>::vlanes());
            v_float32 v1 = v_absdiff(v11, v12);
            v_float64 f11 = v_cvt_f64(v1), f12 = v_cvt_f64_उच्चः(v1);
            r2 = v_fma(f11, f11, r2); r3 = v_fma(f12, f12, r3);
        }
        s += v_reduce_योग(v_सङ्कलय(v_सङ्कलय(v_सङ्कलय(r0, r1), r2), r3));
        प्रति (; j < n; j++) {
            द्विगुण v = (द्विगुण)src1[j] - (द्विगुण)src2[j];
            s += v * v;
        }
        प्रत्यागम s;
    }
};

प्रतिरूपम्<>
संरचना NormDiffL2_SIMD<द्विगुण, द्विगुण> {
    द्विगुण संक्रियकः() (ध्रुव द्विगुण* src1, ध्रुव द्विगुण* src2, पूर्णाङ्क n) ध्रुव {
        पूर्णाङ्क j = 0;
        द्विगुण s = 0.f;
        v_float64 r0 = vx_setzero_f64(), r1 = vx_setzero_f64();
        v_float64 r2 = vx_setzero_f64(), r3 = vx_setzero_f64();
        प्रति (; j <= n - 4 * VTraits<v_float64>::vlanes(); j += 4 * VTraits<v_float64>::vlanes()) {
            v_float64 v01 = vx_आरोपय(src1 + j), v02 = vx_आरोपय(src2 + j);
            v_float64 v0 = v_absdiff(v01, v02);
            r0 = v_fma(v0, v0, r0);

            v_float64 v11 = vx_आरोपय(src1 + j + VTraits<v_float64>::vlanes()),
                      v12 = vx_आरोपय(src2 + j + VTraits<v_float64>::vlanes());
            v_float64 v1 = v_absdiff(v11, v12);
            r1 = v_fma(v1, v1, r1);

            v_float64 v21 = vx_आरोपय(src1 + j + 2 * VTraits<v_float64>::vlanes()),
                      v22 = vx_आरोपय(src2 + j + 2 * VTraits<v_float64>::vlanes());
            v_float64 v2 = v_absdiff(v21, v22);
            r2 = v_fma(v2, v2, r2);

            v_float64 v31 = vx_आरोपय(src1 + j + 3 * VTraits<v_float64>::vlanes()),
                      v32 = vx_आरोपय(src2 + j + 3 * VTraits<v_float64>::vlanes());
            v_float64 v3 = v_absdiff(v31, v32);
            r3 = v_fma(v3, v3, r3);
        }
        s += v_reduce_योग(v_सङ्कलय(v_सङ्कलय(v_सङ्कलय(r0, r1), r2), r3));
        प्रति (; j < n; j++) {
            द्विगुण v = (द्विगुण)src1[j] - (द्विगुण)src2[j];
            s += v * v;
        }
        प्रत्यागम s;
    }
};

#यदि_अन्तः

प्रतिरूपम् <प्रकारनाम T, प्रकारनाम ST>
संरचना MaskedNormInf_SIMD {
    अन्तःस्थ ST संक्रियकः() (ध्रुव T* src, ध्रुव uchar* आवरणम्, पूर्णाङ्क len, पूर्णाङ्क cn) ध्रुव {
        ST s = 0;
        यदि (cn == 1) {
            प्रति (पूर्णाङ्क i = 0; i < len; i++) {
                यदि (आवरणम्[i]) {
                    s = std::max(s, (ST)cv_abs(src[i]));
                }
            }
        }
        अन्यथा {
            प्रति (पूर्णाङ्क i = 0; i < len; i++) {
                यदि (आवरणम्[i]) {
                    ध्रुव T* elem = src + i * cn;
                    पूर्णाङ्क k = 0;
                #यदि CV_ENABLE_UNROLLED && !CV_SIMD_SCALABLE
                    प्रति (; k <= cn - 4; k += 4) {
                        s = std::max(s, (ST)cv_abs(elem[k]));
                        s = std::max(s, (ST)cv_abs(elem[k + 1]));
                        s = std::max(s, (ST)cv_abs(elem[k + 2]));
                        s = std::max(s, (ST)cv_abs(elem[k + 3]));
                    }
                #यदि_अन्तः
                    प्रति (; k < cn; k++) {
                        s = std::max(s, (ST)cv_abs(elem[k]));
                    }
                }
            }
        }
        प्रत्यागम s;
    }
};

प्रतिरूपम् <प्रकारनाम T, प्रकारनाम ST>
संरचना MaskedNormL1_SIMD {
    अन्तःस्थ ST संक्रियकः() (ध्रुव T* src, ध्रुव uchar* आवरणम्, पूर्णाङ्क len, पूर्णाङ्क cn) ध्रुव {
        ST s = 0;
        यदि (cn == 1) {
            प्रति (पूर्णाङ्क i = 0; i < len; i++) {
                यदि (आवरणम्[i]) {
                    s += (ST)cv_abs(src[i]);
                }
            }
        }
        अन्यथा {
            प्रति (पूर्णाङ्क i = 0; i < len; i++) {
                यदि (आवरणम्[i]) {
                    ध्रुव T* elem = src + i * cn;
                    पूर्णाङ्क k = 0;
                #यदि CV_ENABLE_UNROLLED && !CV_SIMD_SCALABLE
                    प्रति (; k <= cn - 4; k += 4) {
                        s += (ST)cv_abs(elem[k]);
                        s += (ST)cv_abs(elem[k + 1]);
                        s += (ST)cv_abs(elem[k + 2]);
                        s += (ST)cv_abs(elem[k + 3]);
                    }
                #यदि_अन्तः
                    प्रति (; k < cn; k++) {
                        s += (ST)cv_abs(elem[k]);
                    }
                }
            }
        }
        प्रत्यागम s;
    }
};

प्रतिरूपम् <प्रकारनाम T, प्रकारनाम ST>
संरचना MaskedNormL2_SIMD {
    अन्तःस्थ ST संक्रियकः() (ध्रुव T* src, ध्रुव uchar* आवरणम्, पूर्णाङ्क len, पूर्णाङ्क cn) ध्रुव {
        ST s = 0;
        यदि (cn == 1) {
            पूर्णाङ्क i = 0;
        #यदि CV_ENABLE_UNROLLED && !CV_SIMD_SCALABLE
            प्रति (; i <= len - 4; i += 4) {
                यदि (आवरणम्[i])     { T v0 = src[i];     s += (ST)v0 * v0; }
                यदि (आवरणम्[i + 1]) { T v1 = src[i + 1]; s += (ST)v1 * v1; }
                यदि (आवरणम्[i + 2]) { T v2 = src[i + 2]; s += (ST)v2 * v2; }
                यदि (आवरणम्[i + 3]) { T v3 = src[i + 3]; s += (ST)v3 * v3; }
            }
        #यदि_अन्तः
            प्रति (; i < len; i++) {
                यदि (आवरणम्[i]) {
                    T v = src[i];
                    s += (ST)v * v;
                }
            }
        }
        अन्यथा {
            प्रति (पूर्णाङ्क i = 0; i < len; i++) {
                यदि (आवरणम्[i]) {
                    ध्रुव T* elem = src + i * cn;
                    पूर्णाङ्क k = 0;
                #यदि CV_ENABLE_UNROLLED && !CV_SIMD_SCALABLE
                    प्रति (; k <= cn - 4; k += 4) {
                        T v0 = elem[k];     s += (ST)v0 * v0;
                        T v1 = elem[k + 1]; s += (ST)v1 * v1;
                        T v2 = elem[k + 2]; s += (ST)v2 * v2;
                        T v3 = elem[k + 3]; s += (ST)v3 * v3;
                    }
                #यदि_अन्तः
                    प्रति (; k < cn; k++) {
                        T v = elem[k];
                        s += (ST)v * v;
                    }
                }
            }
        }

        प्रत्यागम s;
    }
};

#यदि (CV_SIMD || CV_SIMD_SCALABLE)
स्थिर अन्तःस्थ v_uint8  v_normmask_u8 (ध्रुव uchar* m) { प्रत्यागम v_gt(vx_आरोपय(m),          vx_setzero_u8());  }
स्थिर अन्तःस्थ v_uint16 v_normmask_u16(ध्रुव uchar* m) { प्रत्यागम v_gt(vx_आरोपय_expand(m),   vx_setzero_u16()); }
स्थिर अन्तःस्थ v_uint32 v_normmask_u32(ध्रुव uchar* m) { प्रत्यागम v_gt(vx_आरोपय_expand_q(m), vx_setzero_u32()); }
स्थिर अन्तःस्थ शून्य v_normmask_u64(ध्रुव uchar* m, v_uint64& m0, v_uint64& m1) {
    v_uint64 e0, e1;
    v_expand(v_normmask_u32(m), e0, e1);
    m0 = v_or(e0, v_shl<32>(e0));
    m1 = v_or(e1, v_shl<32>(e1));
}

प्रतिरूपम् <>
संरचना MaskedNormInf_SIMD<भिन्न, भिन्न> {
    अन्तःस्थ भिन्न संक्रियकः()(ध्रुव भिन्न* src, ध्रुव uchar* आवरणम्, पूर्णाङ्क len, पूर्णाङ्क cn) ध्रुव {
        भिन्न परिणाम = 0.0f;
        यदि (cn == 1) {
            पूर्णाङ्क i = 0;
            ध्रुव पूर्णाङ्क vstep = VTraits<v_float32>::vlanes();
            v_float32 acc = vx_setzero_f32();

            प्रति (; i <= len - vstep; i += vstep) {
                v_uint32 m = vx_आरोपय_expand_q(आवरणम् + i);
                v_uint32 cmp = v_gt(m, vx_setzero_u32());
                v_float32 s = vx_आरोपय(src + i);
                s = v_abs(s);
                s = v_reinterpret_as_f32(v_and(v_reinterpret_as_u32(s), cmp));
                acc = v_max(acc, s);
            }
            परिणाम = v_reduce_max(acc);

            प्रति (; i < len; i++) {
                यदि (आवरणम्[i])
                    परिणाम = std::max(परिणाम, std::निरपेक्षम्(src[i]));
            }
        }
        अन्यथा {
            प्रति (पूर्णाङ्क i = 0; i < len; i++) {
                यदि (आवरणम्[i]) {
                    ध्रुव भिन्न* elem = src + i * cn;
                    पूर्णाङ्क k = 0;
                    ध्रुव पूर्णाङ्क vstep = VTraits<v_float32>::vlanes();
                    v_float32 acc = vx_setzero_f32();

                    प्रति (; k <= cn - vstep; k += vstep) {
                        v_float32 s = vx_आरोपय(elem + k);
                        acc = v_max(acc, v_abs(s));
                    }

                    परिणाम = std::max(परिणाम, v_reduce_max(acc));

                    प्रति (; k < cn; k++)
                        परिणाम = std::max(परिणाम, std::निरपेक्षम्(elem[k]));
                }
            }
        }
        प्रत्यागम परिणाम;
    }
};

#यदि CV_SIMD_64F
प्रतिरूपम् <>
संरचना MaskedNormL1_SIMD<भिन्न, द्विगुण> {
    अन्तःस्थ द्विगुण संक्रियकः()(ध्रुव भिन्न* src, ध्रुव uchar* आवरणम्, पूर्णाङ्क len, पूर्णाङ्क cn) ध्रुव {
        द्विगुण परिणाम = 0.0;
        यदि (cn == 1) {
            पूर्णाङ्क i = 0;
            ध्रुव पूर्णाङ्क vstep = VTraits<v_float32>::vlanes();
            v_float64 acc0 = vx_setzero_f64(), acc1 = vx_setzero_f64();
            v_float64 acc2 = vx_setzero_f64(), acc3 = vx_setzero_f64();

            प्रति (; i <= len - 2*vstep; i += 2*vstep) {
                v_float32 s0 = v_and(v_abs(vx_आरोपय(src + i)),
                                     v_reinterpret_as_f32(v_normmask_u32(आवरणम् + i)));
                v_float32 s1 = v_and(v_abs(vx_आरोपय(src + i + vstep)),
                                     v_reinterpret_as_f32(v_normmask_u32(आवरणम् + i + vstep)));
                acc0 = v_सङ्कलय(acc0, v_cvt_f64(s0));
                acc1 = v_सङ्कलय(acc1, v_cvt_f64_उच्चः(s0));
                acc2 = v_सङ्कलय(acc2, v_cvt_f64(s1));
                acc3 = v_सङ्कलय(acc3, v_cvt_f64_उच्चः(s1));
            }
            प्रति (; i <= len - vstep; i += vstep) {
                v_float32 s = v_and(v_abs(vx_आरोपय(src + i)),
                                    v_reinterpret_as_f32(v_normmask_u32(आवरणम् + i)));
                acc0 = v_सङ्कलय(acc0, v_cvt_f64(s));
                acc1 = v_सङ्कलय(acc1, v_cvt_f64_उच्चः(s));
            }
            परिणाम = v_reduce_योग(v_सङ्कलय(v_सङ्कलय(acc0, acc1), v_सङ्कलय(acc2, acc3)));

            प्रति (; i < len; i++) {
                यदि (आवरणम्[i])
                    परिणाम += std::निरपेक्षम्(src[i]);
            }
        }
        अन्यथा {
            प्रति (पूर्णाङ्क i = 0; i < len; i++) {
                यदि (आवरणम्[i]) {
                    ध्रुव भिन्न* elem = src + i * cn;
                    पूर्णाङ्क k = 0;
                    ध्रुव पूर्णाङ्क vstep = VTraits<v_float32>::vlanes();
                    v_float64 acc = vx_setzero_f64();

                    प्रति (; k <= cn - vstep; k += vstep) {
                        v_float32 s = v_abs(vx_आरोपय(elem + k));
                        acc = v_सङ्कलय(acc, v_cvt_f64(s));
                        acc = v_सङ्कलय(acc, v_cvt_f64_उच्चः(s));
                    }

                    परिणाम += v_reduce_योग(acc);

                    प्रति (; k < cn; k++)
                        परिणाम += std::निरपेक्षम्(elem[k]);
                }
            }
        }
        प्रत्यागम परिणाम;
    }
};

प्रतिरूपम् <>
संरचना MaskedNormL2_SIMD<भिन्न, द्विगुण> {
    अन्तःस्थ द्विगुण संक्रियकः()(ध्रुव भिन्न* src, ध्रुव uchar* आवरणम्, पूर्णाङ्क len, पूर्णाङ्क cn) ध्रुव {
        द्विगुण परिणाम = 0.0;
        यदि (cn == 1) {
            पूर्णाङ्क i = 0;
            ध्रुव पूर्णाङ्क vstep = VTraits<v_float32>::vlanes();
            v_float32 facc = vx_setzero_f32();
            v_float64 dacc = vx_setzero_f64();
            पूर्णाङ्क flush = 0;

            प्रति (; i <= len - vstep; i += vstep, flush += vstep) {
                यदि (flush >= 64) {
                    dacc = v_सङ्कलय(dacc, v_cvt_f64(facc));
                    dacc = v_सङ्कलय(dacc, v_cvt_f64_उच्चः(facc));
                    facc = vx_setzero_f32();
                    flush = 0;
                }
                v_uint32 cmp = v_gt(vx_आरोपय_expand_q(आवरणम् + i), vx_setzero_u32());
                v_float32 s  = v_reinterpret_as_f32(v_and(v_reinterpret_as_u32(vx_आरोपय(src + i)), cmp));
                facc = v_सङ्कलय(facc, v_mul(s, s));
            }

            dacc = v_सङ्कलय(dacc, v_cvt_f64(facc));
            dacc = v_सङ्कलय(dacc, v_cvt_f64_उच्चः(facc));
            परिणाम = v_reduce_योग(dacc);

            प्रति (; i < len; i++) {
                यदि (आवरणम्[i]) {
                    द्विगुण v = src[i];
                    परिणाम += v * v;
                }
            }
        }
        अन्यथा {
            प्रति (पूर्णाङ्क i = 0; i < len; i++) {
                यदि (आवरणम्[i]) {
                    ध्रुव भिन्न* elem = src + i * cn;
                    पूर्णाङ्क k = 0;
                    ध्रुव पूर्णाङ्क vstep = VTraits<v_float32>::vlanes();
                    v_float32 facc = vx_setzero_f32();

                    प्रति (; k <= cn - vstep; k += vstep) {
                        v_float32 s = vx_आरोपय(elem + k);
                        facc = v_सङ्कलय(facc, v_mul(s, s));
                    }

                    v_float64 dacc = v_सङ्कलय(v_cvt_f64(facc), v_cvt_f64_उच्चः(facc));
                    परिणाम += v_reduce_योग(dacc);

                    प्रति (; k < cn; k++) {
                        द्विगुण v = elem[k];
                        परिणाम += v * v;
                    }
                }
            }
        }
        प्रत्यागम परिणाम;
    }
};

#यदि_अन्तः

प्रतिरूपम् <>
संरचना MaskedNormInf_SIMD<uchar, पूर्णाङ्क> {
    अन्तःस्थ पूर्णाङ्क संक्रियकः()(ध्रुव uchar* src, ध्रुव uchar* आवरणम्, पूर्णाङ्क len, पूर्णाङ्क cn) ध्रुव {
        पूर्णाङ्क परिणाम = 0;
        यदि (cn == 1) {
            पूर्णाङ्क i = 0;
            ध्रुव पूर्णाङ्क vstep = VTraits<v_uint8>::vlanes();
            v_uint8 acc = vx_setzero_u8();

            प्रति (; i <= len - vstep; i += vstep) {
                v_uint8 m   = vx_आरोपय(आवरणम् + i);
                v_uint8 s   = vx_आरोपय(src + i);
                v_uint8 sel = v_and(s, v_gt(m, vx_setzero_u8()));
                acc = v_max(acc, sel);
            }

            परिणाम = (पूर्णाङ्क)v_reduce_max(acc);

            प्रति (; i < len; i++) {
                यदि (आवरणम्[i])
                    परिणाम = std::max(परिणाम, (पूर्णाङ्क)src[i]);
            }
        }
        अन्यथा यदि (cn == 4 && len >= VTraits<v_uint8>::vlanes()) {
            ध्रुव पूर्णाङ्क vstep = VTraits<v_uint8>::vlanes();
            v_uint8 acc = vx_setzero_u8();
            पूर्णाङ्क i = 0;
            प्रति (;;) {
                यदि (i > len - vstep)
                    i = len - vstep; // back-step (max is idempotent)
                v_uint8 c0, c1, c2, c3;
                v_आरोपय_deinterleave(src + i * 4, c0, c1, c2, c3);
                v_uint8 pmax = v_max(v_max(c0, c1), v_max(c2, c3));
                v_uint8 m = v_gt(vx_आरोपय(आवरणम् + i), vx_setzero_u8());
                acc = v_max(acc, v_and(pmax, m));
                यदि (i >= len - vstep)
                    भङ्ग;
                i += vstep;
            }
            परिणाम = (पूर्णाङ्क)v_reduce_max(acc);
        }
        अन्यथा यदि (cn == 4) {
            प्रति (पूर्णाङ्क i = 0; i < len; i++) {
                यदि (आवरणम्[i]) {
                    ध्रुव uchar* elem = src + i * 4;
                    परिणाम = std::max(परिणाम, (पूर्णाङ्क)std::max(std::max(elem[0], elem[1]),
                                                            std::max(elem[2], elem[3])));
                }
            }
        }
        अन्यथा {
            प्रति (पूर्णाङ्क i = 0; i < len; i++) {
                यदि (आवरणम्[i]) {
                    ध्रुव uchar* elem = src + i * cn;
                    प्रति (पूर्णाङ्क k = 0; k < cn; k++)
                        परिणाम = std::max(परिणाम, (पूर्णाङ्क)elem[k]);
                }
            }
        }
        प्रत्यागम परिणाम;
    }
};

प्रतिरूपम् <>
संरचना MaskedNormL1_SIMD<uchar, पूर्णाङ्क> {
    अन्तःस्थ पूर्णाङ्क संक्रियकः()(ध्रुव uchar* src, ध्रुव uchar* आवरणम्, पूर्णाङ्क len, पूर्णाङ्क cn) ध्रुव {
        पूर्णाङ्क परिणाम = 0;
        ध्रुव पूर्णाङ्क vstep = VTraits<v_uint8>::vlanes();
        ध्रुव v_uint8 one = vx_setall_u8(1);
        यदि (cn == 1) {
            पूर्णाङ्क i = 0;
            v_uint32 acc = vx_setzero_u32();
            प्रति (; i <= len - vstep; i += vstep) {
                v_uint8 s = v_and(vx_आरोपय(src + i), v_normmask_u8(आवरणम् + i));
                acc = v_dotprod_expand_fast(s, one, acc);
            }
            परिणाम = (पूर्णाङ्क)v_reduce_योग(acc);
            प्रति (; i < len; i++) {
                यदि (आवरणम्[i])
                    परिणाम += src[i];
            }
        }
        अन्यथा यदि (cn == 4) {
            पूर्णाङ्क i = 0;
            v_uint32 acc = vx_setzero_u32();
            प्रति (; i <= len - vstep; i += vstep) {
                v_uint8 c0, c1, c2, c3;
                v_आरोपय_deinterleave(src + i * 4, c0, c1, c2, c3);
                v_uint8 m = v_normmask_u8(आवरणम् + i);
                acc = v_dotprod_expand_fast(v_and(c0, m), one, acc);
                acc = v_dotprod_expand_fast(v_and(c1, m), one, acc);
                acc = v_dotprod_expand_fast(v_and(c2, m), one, acc);
                acc = v_dotprod_expand_fast(v_and(c3, m), one, acc);
            }
            परिणाम = (पूर्णाङ्क)v_reduce_योग(acc);
            प्रति (; i < len; i++) {
                यदि (आवरणम्[i]) {
                    ध्रुव uchar* elem = src + i * 4;
                    परिणाम += elem[0] + elem[1] + elem[2] + elem[3];
                }
            }
        }
        अन्यथा {
            प्रति (पूर्णाङ्क i = 0; i < len; i++) {
                यदि (आवरणम्[i]) {
                    ध्रुव uchar* elem = src + i * cn;
                    प्रति (पूर्णाङ्क k = 0; k < cn; k++)
                        परिणाम += elem[k];
                }
            }
        }
        प्रत्यागम परिणाम;
    }
};

प्रतिरूपम् <>
संरचना MaskedNormInf_SIMD<ushort, पूर्णाङ्क> {
    अन्तःस्थ पूर्णाङ्क संक्रियकः()(ध्रुव ushort* src, ध्रुव uchar* आवरणम्, पूर्णाङ्क len, पूर्णाङ्क cn) ध्रुव {
        पूर्णाङ्क परिणाम = 0;
        यदि (cn == 1) {
            पूर्णाङ्क i = 0;
            ध्रुव पूर्णाङ्क vstep = VTraits<v_uint16>::vlanes();
            v_uint16 acc = vx_setzero_u16();

            प्रति (; i <= len - vstep; i += vstep) {
                v_uint16 m   = vx_आरोपय_expand(आवरणम् + i);
                v_uint16 cmp = v_gt(m, vx_setzero_u16());
                v_uint16 s   = vx_आरोपय(src + i);
                v_uint16 sel = v_and(s, cmp);
                acc = v_max(acc, sel);
            }

            परिणाम = (पूर्णाङ्क)v_reduce_max(acc);

            प्रति (; i < len; i++) {
                यदि (आवरणम्[i])
                    परिणाम = std::max(परिणाम, (पूर्णाङ्क)src[i]);
            }
        }
        अन्यथा {
            प्रति (पूर्णाङ्क i = 0; i < len; i++) {
                यदि (आवरणम्[i]) {
                    ध्रुव ushort* elem = src + i * cn;
                    पूर्णाङ्क k = 0;
                    ध्रुव पूर्णाङ्क vstep = VTraits<v_uint16>::vlanes();
                    v_uint16 acc = vx_setzero_u16();

                    प्रति (; k <= cn - vstep; k += vstep) {
                        acc = v_max(acc, vx_आरोपय(elem + k));
                    }

                    परिणाम = std::max(परिणाम, (पूर्णाङ्क)v_reduce_max(acc));

                    प्रति (; k < cn; k++)
                        परिणाम = std::max(परिणाम, (पूर्णाङ्क)elem[k]);
                }
            }
        }
        प्रत्यागम परिणाम;
    }
};

प्रतिरूपम् <>
संरचना MaskedNormL1_SIMD<ushort, पूर्णाङ्क> {
    अन्तःस्थ पूर्णाङ्क संक्रियकः()(ध्रुव ushort* src, ध्रुव uchar* आवरणम्, पूर्णाङ्क len, पूर्णाङ्क cn) ध्रुव {
        पूर्णाङ्क परिणाम = 0;
        यदि (cn == 1) {
            पूर्णाङ्क i = 0;
            ध्रुव पूर्णाङ्क vstep = VTraits<v_uint16>::vlanes();
            v_uint32 acc32 = vx_setzero_u32();
            v_uint64 acc64 = vx_setzero_u64();
            पूर्णाङ्क acc32_elems = 0;

            प्रति (; i <= len - vstep; i += vstep, acc32_elems += vstep) {
                यदि (acc32_elems >= 512) {
                    v_uint64 lo64, hi64;
                    v_expand(acc32, lo64, hi64);
                    acc64 = v_सङ्कलय(acc64, v_सङ्कलय(lo64, hi64));
                    acc32 = vx_setzero_u32();
                    acc32_elems = 0;
                }
                v_uint16 m   = vx_आरोपय_expand(आवरणम् + i);
                v_uint16 cmp = v_gt(m, vx_setzero_u16());
                v_uint16 s   = v_and(vx_आरोपय(src + i), cmp);
                v_uint32 lo32, hi32;
                v_expand(s, lo32, hi32);
                acc32 = v_सङ्कलय(acc32, v_सङ्कलय(lo32, hi32));
            }

            v_uint64 lo64, hi64;
            v_expand(acc32, lo64, hi64);
            acc64 = v_सङ्कलय(acc64, v_सङ्कलय(lo64, hi64));
            परिणाम = (पूर्णाङ्क)v_reduce_योग(acc64);

            प्रति (; i < len; i++) {
                यदि (आवरणम्[i])
                    परिणाम += src[i];
            }
        }
        अन्यथा {
            प्रति (पूर्णाङ्क i = 0; i < len; i++) {
                यदि (आवरणम्[i]) {
                    ध्रुव ushort* elem = src + i * cn;
                    पूर्णाङ्क k = 0;
                    ध्रुव पूर्णाङ्क vstep = VTraits<v_uint16>::vlanes();
                    v_uint32 acc = vx_setzero_u32();

                    प्रति (; k <= cn - vstep; k += vstep) {
                        v_uint32 lo32, hi32;
                        v_expand(vx_आरोपय(elem + k), lo32, hi32);
                        acc = v_सङ्कलय(acc, v_सङ्कलय(lo32, hi32));
                    }

                    परिणाम += (पूर्णाङ्क)v_reduce_योग(acc);

                    प्रति (; k < cn; k++)
                        परिणाम += elem[k];
                }
            }
        }
        प्रत्यागम परिणाम;
    }
};

प्रतिरूपम् <>
संरचना MaskedNormL2_SIMD<ushort, द्विगुण> {
    अन्तःस्थ द्विगुण संक्रियकः()(ध्रुव ushort* src, ध्रुव uchar* आवरणम्, पूर्णाङ्क len, पूर्णाङ्क cn) ध्रुव {
        द्विगुण परिणाम = 0.0;
        यदि (cn == 1) {
            पूर्णाङ्क i = 0;
            ध्रुव पूर्णाङ्क vstep = VTraits<v_uint16>::vlanes();
            v_uint64 acc = vx_setzero_u64();
            प्रति (; i <= len - vstep; i += vstep) {
                v_uint16 m   = vx_आरोपय_expand(आवरणम् + i);
                v_uint16 cmp = v_gt(m, vx_setzero_u16());
                v_uint16 s   = v_and(vx_आरोपय(src + i), cmp);
                v_uint32 lo32, hi32;
                v_expand(s, lo32, hi32);
                v_uint64 lo64a, lo64b, hi64a, hi64b;
                v_expand(v_mul(lo32, lo32), lo64a, lo64b);
                v_expand(v_mul(hi32, hi32), hi64a, hi64b);
                acc = v_सङ्कलय(acc, v_सङ्कलय(v_सङ्कलय(lo64a, lo64b), v_सङ्कलय(hi64a, hi64b)));
            }
            परिणाम = (द्विगुण)v_reduce_योग(acc);
            प्रति (; i < len; i++) {
                यदि (आवरणम्[i]) {
                    द्विगुण v = src[i];
                    परिणाम += v * v;
                }
            }
        }
        अन्यथा {
            प्रति (पूर्णाङ्क i = 0; i < len; i++) {
                यदि (आवरणम्[i]) {
                    ध्रुव ushort* elem = src + i * cn;
                    पूर्णाङ्क k = 0;
                    ध्रुव पूर्णाङ्क vstep = VTraits<v_uint16>::vlanes();
                    v_uint64 acc = vx_setzero_u64();
                    प्रति (; k <= cn - vstep; k += vstep) {
                        v_uint32 lo32, hi32;
                        v_expand(vx_आरोपय(elem + k), lo32, hi32);
                        v_uint64 lo64a, lo64b, hi64a, hi64b;
                        v_expand(v_mul(lo32, lo32), lo64a, lo64b);
                        v_expand(v_mul(hi32, hi32), hi64a, hi64b);
                        acc = v_सङ्कलय(acc, v_सङ्कलय(v_सङ्कलय(lo64a, lo64b), v_सङ्कलय(hi64a, hi64b)));
                    }

                    परिणाम += (द्विगुण)v_reduce_योग(acc);

                    प्रति (; k < cn; k++) {
                        द्विगुण v = elem[k];
                        परिणाम += v * v;
                    }
                }
            }
        }
        प्रत्यागम परिणाम;
    }
};

प्रतिरूपम् <>
संरचना MaskedNormInf_SIMD<schar, पूर्णाङ्क> {
    अन्तःस्थ पूर्णाङ्क संक्रियकः()(ध्रुव schar* src, ध्रुव uchar* आवरणम्, पूर्णाङ्क len, पूर्णाङ्क cn) ध्रुव {
        पूर्णाङ्क परिणाम = 0;
        यदि (cn == 1) {
            ध्रुव पूर्णाङ्क vstep = VTraits<v_int8>::vlanes();
            पूर्णाङ्क i = 0;
            v_uint8 acc = vx_setzero_u8();
            प्रति (; i <= len - vstep; i += vstep)
                acc = v_max(acc, v_and(v_abs(vx_आरोपय(src + i)), v_normmask_u8(आवरणम् + i)));
            परिणाम = (पूर्णाङ्क)v_reduce_max(acc);
            प्रति (; i < len; i++)
                यदि (आवरणम्[i]) परिणाम = std::max(परिणाम, cv_abs(src[i]));
        }
        अन्यथा {
            प्रति (पूर्णाङ्क i = 0; i < len; i++)
                यदि (आवरणम्[i]) {
                    ध्रुव schar* elem = src + i * cn;
                    प्रति (पूर्णाङ्क k = 0; k < cn; k++)
                        परिणाम = std::max(परिणाम, cv_abs(elem[k]));
                }
        }
        प्रत्यागम परिणाम;
    }
};

प्रतिरूपम् <>
संरचना MaskedNormL1_SIMD<schar, पूर्णाङ्क> {
    अन्तःस्थ पूर्णाङ्क संक्रियकः()(ध्रुव schar* src, ध्रुव uchar* आवरणम्, पूर्णाङ्क len, पूर्णाङ्क cn) ध्रुव {
        पूर्णाङ्क परिणाम = 0;
        यदि (cn == 1) {
            ध्रुव पूर्णाङ्क vstep = VTraits<v_int8>::vlanes();
            ध्रुव v_uint8 one = vx_setall_u8(1);
            पूर्णाङ्क i = 0;
            v_uint32 acc = vx_setzero_u32();
            प्रति (; i <= len - vstep; i += vstep)
                acc = v_dotprod_expand_fast(v_and(v_abs(vx_आरोपय(src + i)), v_normmask_u8(आवरणम् + i)), one, acc);
            परिणाम = (पूर्णाङ्क)v_reduce_योग(acc);
            प्रति (; i < len; i++)
                यदि (आवरणम्[i]) परिणाम += cv_abs(src[i]);
        }
        अन्यथा {
            प्रति (पूर्णाङ्क i = 0; i < len; i++)
                यदि (आवरणम्[i]) {
                    ध्रुव schar* elem = src + i * cn;
                    प्रति (पूर्णाङ्क k = 0; k < cn; k++)
                        परिणाम += cv_abs(elem[k]);
                }
        }
        प्रत्यागम परिणाम;
    }
};

प्रतिरूपम् <>
संरचना MaskedNormL2_SIMD<schar, पूर्णाङ्क> {
    अन्तःस्थ पूर्णाङ्क संक्रियकः()(ध्रुव schar* src, ध्रुव uchar* आवरणम्, पूर्णाङ्क len, पूर्णाङ्क cn) ध्रुव {
        पूर्णाङ्क परिणाम = 0;
        यदि (cn == 1) {
            ध्रुव पूर्णाङ्क vstep = VTraits<v_int8>::vlanes();
            पूर्णाङ्क i = 0;
            v_int32 acc = vx_setzero_s32();
            प्रति (; i <= len - vstep; i += vstep) {
                v_int8 s = v_and(vx_आरोपय(src + i), v_reinterpret_as_s8(v_normmask_u8(आवरणम् + i)));
                acc = v_dotprod_expand_fast(s, s, acc);
            }
            परिणाम = (पूर्णाङ्क)v_reduce_योग(acc);
            प्रति (; i < len; i++)
                यदि (आवरणम्[i]) { पूर्णाङ्क v = src[i]; परिणाम += v * v; }
        }
        अन्यथा {
            प्रति (पूर्णाङ्क i = 0; i < len; i++)
                यदि (आवरणम्[i]) {
                    ध्रुव schar* elem = src + i * cn;
                    प्रति (पूर्णाङ्क k = 0; k < cn; k++) { पूर्णाङ्क v = elem[k]; परिणाम += v * v; }
                }
        }
        प्रत्यागम परिणाम;
    }
};

प्रतिरूपम् <>
संरचना MaskedNormL2_SIMD<uchar, पूर्णाङ्क> {
    अन्तःस्थ पूर्णाङ्क संक्रियकः()(ध्रुव uchar* src, ध्रुव uchar* आवरणम्, पूर्णाङ्क len, पूर्णाङ्क cn) ध्रुव {
        पूर्णाङ्क परिणाम = 0;
        ध्रुव पूर्णाङ्क vstep = VTraits<v_uint8>::vlanes();
        यदि (cn == 1) {
            पूर्णाङ्क i = 0;
            v_uint32 acc = vx_setzero_u32();
            प्रति (; i <= len - vstep; i += vstep) {
                v_uint8 s = v_and(vx_आरोपय(src + i), v_normmask_u8(आवरणम् + i));
                acc = v_dotprod_expand_fast(s, s, acc);
            }
            परिणाम = (पूर्णाङ्क)v_reduce_योग(acc);
            प्रति (; i < len; i++)
                यदि (आवरणम्[i]) { पूर्णाङ्क v = src[i]; परिणाम += v * v; }
        }
        अन्यथा यदि (cn == 4) {
            पूर्णाङ्क i = 0;
            v_uint32 acc = vx_setzero_u32();
            प्रति (; i <= len - vstep; i += vstep) {
                v_uint8 c0, c1, c2, c3;
                v_आरोपय_deinterleave(src + i * 4, c0, c1, c2, c3);
                v_uint8 m = v_normmask_u8(आवरणम् + i);
                c0 = v_and(c0, m); c1 = v_and(c1, m);
                c2 = v_and(c2, m); c3 = v_and(c3, m);
                acc = v_dotprod_expand_fast(c0, c0, acc);
                acc = v_dotprod_expand_fast(c1, c1, acc);
                acc = v_dotprod_expand_fast(c2, c2, acc);
                acc = v_dotprod_expand_fast(c3, c3, acc);
            }
            परिणाम = (पूर्णाङ्क)v_reduce_योग(acc);
            प्रति (; i < len; i++)
                यदि (आवरणम्[i]) {
                    ध्रुव uchar* elem = src + i * 4;
                    प्रति (पूर्णाङ्क k = 0; k < 4; k++) { पूर्णाङ्क v = elem[k]; परिणाम += v * v; }
                }
        }
        अन्यथा {
            प्रति (पूर्णाङ्क i = 0; i < len; i++)
                यदि (आवरणम्[i]) {
                    ध्रुव uchar* elem = src + i * cn;
                    प्रति (पूर्णाङ्क k = 0; k < cn; k++) { पूर्णाङ्क v = elem[k]; परिणाम += v * v; }
                }
        }
        प्रत्यागम परिणाम;
    }
};

प्रतिरूपम् <>
संरचना MaskedNormInf_SIMD<ह्रस्व, पूर्णाङ्क> {
    अन्तःस्थ पूर्णाङ्क संक्रियकः()(ध्रुव ह्रस्व* src, ध्रुव uchar* आवरणम्, पूर्णाङ्क len, पूर्णाङ्क cn) ध्रुव {
        पूर्णाङ्क परिणाम = 0;
        यदि (cn == 1) {
            ध्रुव पूर्णाङ्क vstep = VTraits<v_int16>::vlanes();
            पूर्णाङ्क i = 0;
            v_uint16 acc = vx_setzero_u16();
            प्रति (; i <= len - vstep; i += vstep)
                acc = v_max(acc, v_and(v_abs(vx_आरोपय(src + i)), v_normmask_u16(आवरणम् + i)));
            परिणाम = (पूर्णाङ्क)v_reduce_max(acc);
            प्रति (; i < len; i++)
                यदि (आवरणम्[i]) परिणाम = std::max(परिणाम, cv_abs(src[i]));
        }
        अन्यथा {
            प्रति (पूर्णाङ्क i = 0; i < len; i++)
                यदि (आवरणम्[i]) {
                    ध्रुव ह्रस्व* elem = src + i * cn;
                    प्रति (पूर्णाङ्क k = 0; k < cn; k++)
                        परिणाम = std::max(परिणाम, cv_abs(elem[k]));
                }
        }
        प्रत्यागम परिणाम;
    }
};

प्रतिरूपम् <>
संरचना MaskedNormL1_SIMD<ह्रस्व, पूर्णाङ्क> {
    अन्तःस्थ पूर्णाङ्क संक्रियकः()(ध्रुव ह्रस्व* src, ध्रुव uchar* आवरणम्, पूर्णाङ्क len, पूर्णाङ्क cn) ध्रुव {
        पूर्णाङ्क परिणाम = 0;
        यदि (cn == 1) {
            ध्रुव पूर्णाङ्क vstep = VTraits<v_int16>::vlanes();
            पूर्णाङ्क i = 0;
            v_uint32 acc0 = vx_setzero_u32(), acc1 = vx_setzero_u32();
            प्रति (; i <= len - vstep; i += vstep) {
                v_uint16 s = v_and(v_abs(vx_आरोपय(src + i)), v_normmask_u16(आवरणम् + i));
                v_uint32 lo, hi;
                v_expand(s, lo, hi);
                acc0 = v_सङ्कलय(acc0, lo);
                acc1 = v_सङ्कलय(acc1, hi);
            }
            परिणाम = (पूर्णाङ्क)v_reduce_योग(v_सङ्कलय(acc0, acc1));
            प्रति (; i < len; i++)
                यदि (आवरणम्[i]) परिणाम += cv_abs(src[i]);
        }
        अन्यथा {
            प्रति (पूर्णाङ्क i = 0; i < len; i++)
                यदि (आवरणम्[i]) {
                    ध्रुव ह्रस्व* elem = src + i * cn;
                    प्रति (पूर्णाङ्क k = 0; k < cn; k++)
                        परिणाम += cv_abs(elem[k]);
                }
        }
        प्रत्यागम परिणाम;
    }
};

प्रतिरूपम् <>
संरचना MaskedNormL2_SIMD<ह्रस्व, द्विगुण> {
    अन्तःस्थ द्विगुण संक्रियकः()(ध्रुव ह्रस्व* src, ध्रुव uchar* आवरणम्, पूर्णाङ्क len, पूर्णाङ्क cn) ध्रुव {
        द्विगुण परिणाम = 0.0;
        यदि (cn == 1) {
            ध्रुव पूर्णाङ्क vstep = VTraits<v_int16>::vlanes();
            पूर्णाङ्क i = 0;
            v_uint64 acc = vx_setzero_u64();
            प्रति (; i <= len - vstep; i += vstep) {
                v_uint16 s = v_and(v_abs(vx_आरोपय(src + i)), v_normmask_u16(आवरणम् + i));
                acc = v_सङ्कलय(acc, v_dotprod_expand_fast(s, s));
            }
            परिणाम = (द्विगुण)v_reduce_योग(acc);
            प्रति (; i < len; i++)
                यदि (आवरणम्[i]) { द्विगुण v = src[i]; परिणाम += v * v; }
        }
        अन्यथा {
            प्रति (पूर्णाङ्क i = 0; i < len; i++)
                यदि (आवरणम्[i]) {
                    ध्रुव ह्रस्व* elem = src + i * cn;
                    प्रति (पूर्णाङ्क k = 0; k < cn; k++) { द्विगुण v = elem[k]; परिणाम += v * v; }
                }
        }
        प्रत्यागम परिणाम;
    }
};

प्रतिरूपम् <>
संरचना MaskedNormInf_SIMD<पूर्णाङ्क, अचिह्नित> {
    अन्तःस्थ अचिह्नित संक्रियकः()(ध्रुव पूर्णाङ्क* src, ध्रुव uchar* आवरणम्, पूर्णाङ्क len, पूर्णाङ्क cn) ध्रुव {
        अचिह्नित परिणाम = 0;
        यदि (cn == 1) {
            ध्रुव पूर्णाङ्क vstep = VTraits<v_int32>::vlanes();
            पूर्णाङ्क i = 0;
            v_uint32 acc = vx_setzero_u32();
            प्रति (; i <= len - vstep; i += vstep)
                acc = v_max(acc, v_and(v_abs(vx_आरोपय(src + i)), v_normmask_u32(आवरणम् + i)));
            परिणाम = v_reduce_max(acc);
            प्रति (; i < len; i++)
                यदि (आवरणम्[i]) परिणाम = std::max(परिणाम, cv_abs(src[i]));
        }
        अन्यथा {
            प्रति (पूर्णाङ्क i = 0; i < len; i++)
                यदि (आवरणम्[i]) {
                    ध्रुव पूर्णाङ्क* elem = src + i * cn;
                    प्रति (पूर्णाङ्क k = 0; k < cn; k++)
                        परिणाम = std::max(परिणाम, cv_abs(elem[k]));
                }
        }
        प्रत्यागम परिणाम;
    }
};

#यदि CV_SIMD_64F || CV_SIMD_SCALABLE_64F

प्रतिरूपम् <>
संरचना MaskedNormL1_SIMD<पूर्णाङ्क, द्विगुण> {
    अन्तःस्थ द्विगुण संक्रियकः()(ध्रुव पूर्णाङ्क* src, ध्रुव uchar* आवरणम्, पूर्णाङ्क len, पूर्णाङ्क cn) ध्रुव {
        द्विगुण परिणाम = 0.0;
        यदि (cn == 1) {
            ध्रुव पूर्णाङ्क vstep = VTraits<v_int32>::vlanes();
            पूर्णाङ्क i = 0;
            v_float64 acc0 = vx_setzero_f64(), acc1 = vx_setzero_f64();
            प्रति (; i <= len - vstep; i += vstep) {
                // Mask the value, then widen and take the magnitude in double. v_abs() does give
                // |INT_MIN| as an unsigned 2147483648, but reinterpreting that back as int32 turns
                // it negative again, and the widening then carries the sign into the accumulator.
                v_int32 s = v_reinterpret_as_s32(v_and(v_reinterpret_as_u32(vx_आरोपय(src + i)),
                                                       v_normmask_u32(आवरणम् + i)));
                acc0 = v_सङ्कलय(acc0, v_abs(v_cvt_f64(s)));
                acc1 = v_सङ्कलय(acc1, v_abs(v_cvt_f64_उच्चः(s)));
            }
            परिणाम = v_reduce_योग(v_सङ्कलय(acc0, acc1));
            प्रति (; i < len; i++)
                यदि (आवरणम्[i]) परिणाम += cv_abs(src[i]);
        }
        अन्यथा {
            प्रति (पूर्णाङ्क i = 0; i < len; i++)
                यदि (आवरणम्[i]) {
                    ध्रुव पूर्णाङ्क* elem = src + i * cn;
                    प्रति (पूर्णाङ्क k = 0; k < cn; k++)
                        परिणाम += cv_abs(elem[k]);
                }
        }
        प्रत्यागम परिणाम;
    }
};

प्रतिरूपम् <>
संरचना MaskedNormL2_SIMD<पूर्णाङ्क, द्विगुण> {
    अन्तःस्थ द्विगुण संक्रियकः()(ध्रुव पूर्णाङ्क* src, ध्रुव uchar* आवरणम्, पूर्णाङ्क len, पूर्णाङ्क cn) ध्रुव {
        द्विगुण परिणाम = 0.0;
        यदि (cn == 1) {
            ध्रुव पूर्णाङ्क vstep = VTraits<v_int32>::vlanes();
            पूर्णाङ्क i = 0;
            v_float64 acc0 = vx_setzero_f64(), acc1 = vx_setzero_f64();
            प्रति (; i <= len - vstep; i += vstep) {
                v_uint32 s = v_and(v_abs(vx_आरोपय(src + i)), v_normmask_u32(आवरणम् + i));
                v_uint64 e0, e1;
                v_expand(s, e0, e1);
                v_float64 f0 = v_cvt_f64(v_reinterpret_as_s64(e0));
                v_float64 f1 = v_cvt_f64(v_reinterpret_as_s64(e1));
                acc0 = v_fma(f0, f0, acc0);
                acc1 = v_fma(f1, f1, acc1);
            }
            परिणाम = v_reduce_योग(v_सङ्कलय(acc0, acc1));
            प्रति (; i < len; i++)
                यदि (आवरणम्[i]) { द्विगुण v = src[i]; परिणाम += v * v; }
        }
        अन्यथा {
            प्रति (पूर्णाङ्क i = 0; i < len; i++)
                यदि (आवरणम्[i]) {
                    ध्रुव पूर्णाङ्क* elem = src + i * cn;
                    प्रति (पूर्णाङ्क k = 0; k < cn; k++) { द्विगुण v = elem[k]; परिणाम += v * v; }
                }
        }
        प्रत्यागम परिणाम;
    }
};

प्रतिरूपम् <>
संरचना MaskedNormInf_SIMD<द्विगुण, द्विगुण> {
    अन्तःस्थ द्विगुण संक्रियकः()(ध्रुव द्विगुण* src, ध्रुव uchar* आवरणम्, पूर्णाङ्क len, पूर्णाङ्क cn) ध्रुव {
        द्विगुण परिणाम = 0.0;
        यदि (cn == 1) {
            ध्रुव पूर्णाङ्क vstep = VTraits<v_float64>::vlanes();
            पूर्णाङ्क i = 0;
            v_float64 acc0 = vx_setzero_f64(), acc1 = vx_setzero_f64();
            प्रति (; i <= len - 2*vstep; i += 2*vstep) {
                v_uint64 m0, m1;
                v_normmask_u64(आवरणम् + i, m0, m1);
                acc0 = v_max(acc0, v_reinterpret_as_f64(v_and(v_reinterpret_as_u64(v_abs(vx_आरोपय(src + i))), m0)));
                acc1 = v_max(acc1, v_reinterpret_as_f64(v_and(v_reinterpret_as_u64(v_abs(vx_आरोपय(src + i + vstep))), m1)));
            }
            द्विगुण t[VTraits<v_float64>::max_nlanes];
            vx_store(t, v_max(acc0, acc1));
            प्रति (पूर्णाङ्क k = 0; k < vstep; k++)
                परिणाम = std::max(परिणाम, t[k]);
            प्रति (; i < len; i++)
                यदि (आवरणम्[i]) परिणाम = std::max(परिणाम, cv_abs(src[i]));
        }
        अन्यथा {
            प्रति (पूर्णाङ्क i = 0; i < len; i++)
                यदि (आवरणम्[i]) {
                    ध्रुव द्विगुण* elem = src + i * cn;
                    प्रति (पूर्णाङ्क k = 0; k < cn; k++)
                        परिणाम = std::max(परिणाम, cv_abs(elem[k]));
                }
        }
        प्रत्यागम परिणाम;
    }
};

प्रतिरूपम् <>
संरचना MaskedNormL1_SIMD<द्विगुण, द्विगुण> {
    अन्तःस्थ द्विगुण संक्रियकः()(ध्रुव द्विगुण* src, ध्रुव uchar* आवरणम्, पूर्णाङ्क len, पूर्णाङ्क cn) ध्रुव {
        द्विगुण परिणाम = 0.0;
        यदि (cn == 1) {
            ध्रुव पूर्णाङ्क vstep = VTraits<v_float64>::vlanes();
            पूर्णाङ्क i = 0;
            v_float64 acc0 = vx_setzero_f64(), acc1 = vx_setzero_f64();
            प्रति (; i <= len - 2*vstep; i += 2*vstep) {
                v_uint64 m0, m1;
                v_normmask_u64(आवरणम् + i, m0, m1);
                acc0 = v_सङ्कलय(acc0, v_reinterpret_as_f64(v_and(v_reinterpret_as_u64(v_abs(vx_आरोपय(src + i))), m0)));
                acc1 = v_सङ्कलय(acc1, v_reinterpret_as_f64(v_and(v_reinterpret_as_u64(v_abs(vx_आरोपय(src + i + vstep))), m1)));
            }
            परिणाम = v_reduce_योग(v_सङ्कलय(acc0, acc1));
            प्रति (; i < len; i++)
                यदि (आवरणम्[i]) परिणाम += cv_abs(src[i]);
        }
        अन्यथा {
            प्रति (पूर्णाङ्क i = 0; i < len; i++)
                यदि (आवरणम्[i]) {
                    ध्रुव द्विगुण* elem = src + i * cn;
                    प्रति (पूर्णाङ्क k = 0; k < cn; k++)
                        परिणाम += cv_abs(elem[k]);
                }
        }
        प्रत्यागम परिणाम;
    }
};

प्रतिरूपम् <>
संरचना MaskedNormL2_SIMD<द्विगुण, द्विगुण> {
    अन्तःस्थ द्विगुण संक्रियकः()(ध्रुव द्विगुण* src, ध्रुव uchar* आवरणम्, पूर्णाङ्क len, पूर्णाङ्क cn) ध्रुव {
        द्विगुण परिणाम = 0.0;
        यदि (cn == 1) {
            ध्रुव पूर्णाङ्क vstep = VTraits<v_float64>::vlanes();
            पूर्णाङ्क i = 0;
            v_float64 acc0 = vx_setzero_f64(), acc1 = vx_setzero_f64();
            प्रति (; i <= len - 2*vstep; i += 2*vstep) {
                v_uint64 m0, m1;
                v_normmask_u64(आवरणम् + i, m0, m1);
                v_float64 s0 = v_reinterpret_as_f64(v_and(v_reinterpret_as_u64(vx_आरोपय(src + i)), m0));
                v_float64 s1 = v_reinterpret_as_f64(v_and(v_reinterpret_as_u64(vx_आरोपय(src + i + vstep)), m1));
                acc0 = v_fma(s0, s0, acc0);
                acc1 = v_fma(s1, s1, acc1);
            }
            परिणाम = v_reduce_योग(v_सङ्कलय(acc0, acc1));
            प्रति (; i < len; i++)
                यदि (आवरणम्[i]) { द्विगुण v = src[i]; परिणाम += v * v; }
        }
        अन्यथा {
            प्रति (पूर्णाङ्क i = 0; i < len; i++)
                यदि (आवरणम्[i]) {
                    ध्रुव द्विगुण* elem = src + i * cn;
                    प्रति (पूर्णाङ्क k = 0; k < cn; k++) { द्विगुण v = elem[k]; परिणाम += v * v; }
                }
        }
        प्रत्यागम परिणाम;
    }
};

#यदि_अन्तः // CV_SIMD_64F || CV_SIMD_SCALABLE_64F
#यदि_अन्तः // CV_SIMD || CV_SIMD_SCALABLE

प्रतिरूपम्<प्रकारनाम T, प्रकारनाम ST> पूर्णाङ्क
normInf_(ध्रुव T* src, ध्रुव uchar* आवरणम्, ST* परिणाम_2, पूर्णाङ्क len, पूर्णाङ्क cn)
{
    ST परिणाम = *परिणाम_2;
    यदि( !आवरणम् )
    {
        NormInf_SIMD<T, ST> op;
        परिणाम = std::max(परिणाम, op(src, len*cn));
    } अन्यथा {
        MaskedNormInf_SIMD<T, ST> op;
        परिणाम = std::max(परिणाम, op(src, आवरणम्, len, cn));
    }
    *परिणाम_2 = परिणाम;
    प्रत्यागम 0;
}

प्रतिरूपम्<प्रकारनाम T, प्रकारनाम ST> पूर्णाङ्क
normL1_(ध्रुव T* src, ध्रुव uchar* आवरणम्, ST* परिणाम_2, पूर्णाङ्क len, पूर्णाङ्क cn)
{
    ST परिणाम = *परिणाम_2;
    यदि( !आवरणम् )
    {
        NormL1_SIMD<T, ST> op;
        परिणाम += op(src, len*cn);
    } अन्यथा {
        MaskedNormL1_SIMD<T, ST> op;
        परिणाम += op(src, आवरणम्, len, cn);
    }
    *परिणाम_2 = परिणाम;
    प्रत्यागम 0;
}

प्रतिरूपम्<प्रकारनाम T, प्रकारनाम ST> पूर्णाङ्क
normL2_(ध्रुव T* src, ध्रुव uchar* आवरणम्, ST* परिणाम_2, पूर्णाङ्क len, पूर्णाङ्क cn)
{
    ST परिणाम = *परिणाम_2;
    यदि( !आवरणम् )
    {
        NormL2_SIMD<T, ST> op;
        परिणाम += op(src, len*cn);
    }
    अन्यथा
    {
        MaskedNormL2_SIMD<T, ST> op;
        परिणाम += op(src, आवरणम्, len, cn);
    }
    *परिणाम_2 = परिणाम;
    प्रत्यागम 0;
}

स्थिर पूर्णाङ्क
normInf_Bool(ध्रुव uchar* src, ध्रुव uchar* आवरणम्, पूर्णाङ्क* परिणाम_2, पूर्णाङ्क len, पूर्णाङ्क cn)
{
    पूर्णाङ्क परिणाम = *परिणाम_2;
    यदि( !आवरणम् )
    {
        प्रति ( पूर्णाङ्क i = 0; i < len*cn; i++ ) {
            परिणाम = std::max(परिणाम, (पूर्णाङ्क)(src[i] != 0));
            यदि (परिणाम != 0)
                भङ्ग;
        }
    }
    अन्यथा
    {
        प्रति( पूर्णाङ्क i = 0; i < len; i++, src += cn )
            यदि( आवरणम्[i] )
            {
                प्रति( पूर्णाङ्क k = 0; k < cn; k++ )
                    परिणाम = std::max(परिणाम, (पूर्णाङ्क)(src[k] != 0));
                यदि (परिणाम != 0)
                    भङ्ग;
            }
    }
    *परिणाम_2 = परिणाम;
    प्रत्यागम 0;
}

स्थिर पूर्णाङ्क
normL1_Bool(ध्रुव uchar* src, ध्रुव uchar* आवरणम्, पूर्णाङ्क* परिणाम_2, पूर्णाङ्क len, पूर्णाङ्क cn)
{
    पूर्णाङ्क परिणाम = *परिणाम_2;
    यदि( !आवरणम् )
    {
        प्रति ( पूर्णाङ्क i = 0; i < len*cn; i++ )
            परिणाम += (पूर्णाङ्क)(src[i] != 0);
    }
    अन्यथा
    {
        प्रति( पूर्णाङ्क i = 0; i < len; i++, src += cn )
            यदि( आवरणम्[i] )
            {
                प्रति( पूर्णाङ्क k = 0; k < cn; k++ )
                    परिणाम += (पूर्णाङ्क)(src[k] != 0);
            }
    }
    *परिणाम_2 = परिणाम;
    प्रत्यागम 0;
}

स्थिर पूर्णाङ्क
normL2_Bool(ध्रुव uchar* src, ध्रुव uchar* आवरणम्, पूर्णाङ्क* परिणाम_2, पूर्णाङ्क len, पूर्णाङ्क cn)
{
    प्रत्यागम normL1_Bool(src, आवरणम्, परिणाम_2, len, cn);
}

// ====================================================================
// Masked norm-diff kernels.
// Scalar base templates (safe for every depth) plus SIMD cn==1
// specializations for the concrete (T, ST) pairs dispatched by
// CV_DEF_NORM_ALL. The masked normDiff*_ functions below select them.
// ====================================================================
प्रतिरूपम् <प्रकारनाम T, प्रकारनाम ST>
संरचना MaskedNormDiffInf_SIMD {
    अन्तःस्थ ST संक्रियकः()(ध्रुव T* s1, ध्रुव T* s2, ध्रुव uchar* आवरणम्, पूर्णाङ्क len, पूर्णाङ्क cn) ध्रुव {
        ST परिणाम = 0;
        प्रति (पूर्णाङ्क i = 0; i < len; i++) यदि (आवरणम्[i]) {
            ध्रुव T* e1 = s1 + i*cn; ध्रुव T* e2 = s2 + i*cn;
            प्रति (पूर्णाङ्क k = 0; k < cn; k++)
                परिणाम = std::max(परिणाम, (ST)cv_absdiff(e1[k], e2[k]));
        }
        प्रत्यागम परिणाम;
    }
};
प्रतिरूपम् <प्रकारनाम T, प्रकारनाम ST>
संरचना MaskedNormDiffL1_SIMD {
    अन्तःस्थ ST संक्रियकः()(ध्रुव T* s1, ध्रुव T* s2, ध्रुव uchar* आवरणम्, पूर्णाङ्क len, पूर्णाङ्क cn) ध्रुव {
        ST परिणाम = 0;
        प्रति (पूर्णाङ्क i = 0; i < len; i++) यदि (आवरणम्[i]) {
            ध्रुव T* e1 = s1 + i*cn; ध्रुव T* e2 = s2 + i*cn;
            प्रति (पूर्णाङ्क k = 0; k < cn; k++)
                परिणाम += (ST)cv_absdiff(e1[k], e2[k]);
        }
        प्रत्यागम परिणाम;
    }
};
प्रतिरूपम् <प्रकारनाम T, प्रकारनाम ST>
संरचना MaskedNormDiffL2_SIMD {
    अन्तःस्थ ST संक्रियकः()(ध्रुव T* s1, ध्रुव T* s2, ध्रुव uchar* आवरणम्, पूर्णाङ्क len, पूर्णाङ्क cn) ध्रुव {
        ST परिणाम = 0;
        प्रति (पूर्णाङ्क i = 0; i < len; i++) यदि (आवरणम्[i]) {
            ध्रुव T* e1 = s1 + i*cn; ध्रुव T* e2 = s2 + i*cn;
            प्रति (पूर्णाङ्क k = 0; k < cn; k++) { ST v = (ST)e1[k] - (ST)e2[k]; परिणाम += v*v; }
        }
        प्रत्यागम परिणाम;
    }
};

#यदि (CV_SIMD || CV_SIMD_SCALABLE)
// AND a per-lane diff vector with the (expanded) mask>0 predicate.
अन्तःस्थ v_uint8  v_diffmask(ध्रुव v_uint8&  d, ध्रुव uchar* m) { प्रत्यागम v_and(d, v_gt(vx_आरोपय(m),          vx_setzero_u8())); }
अन्तःस्थ v_uint16 v_diffmask(ध्रुव v_uint16& d, ध्रुव uchar* m) { प्रत्यागम v_and(d, v_gt(vx_आरोपय_expand(m),   vx_setzero_u16())); }
अन्तःस्थ v_uint32 v_diffmask(ध्रुव v_uint32& d, ध्रुव uchar* m) { प्रत्यागम v_and(d, v_gt(vx_आरोपय_expand_q(m), vx_setzero_u32())); }
अन्तःस्थ v_float32 v_diffmask(ध्रुव v_float32& d, ध्रुव uchar* m) {
    v_uint32 cm = v_gt(vx_आरोपय_expand_q(m), vx_setzero_u32());
    प्रत्यागम v_reinterpret_as_f32(v_and(v_reinterpret_as_u32(d), cm));
}

// Shared scalar cn>1 fallbacks (match the base templates' cv_absdiff semantics).
प्रतिरूपम्<प्रकारनाम T, प्रकारनाम RT>
स्थिर अन्तःस्थ RT masked_Norm_Diff_Inf_पुच्छ(ध्रुव T* s1, ध्रुव T* s2, ध्रुव uchar* आवरणम्, पूर्णाङ्क len, पूर्णाङ्क cn, RT परिणाम) {
    प्रति (पूर्णाङ्क i = 0; i < len; i++) यदि (आवरणम्[i]) {
        ध्रुव T* e1 = s1 + i*cn; ध्रुव T* e2 = s2 + i*cn;
        प्रति (पूर्णाङ्क k = 0; k < cn; k++) परिणाम = std::max(परिणाम, (RT)cv_absdiff(e1[k], e2[k]));
    }
    प्रत्यागम परिणाम;
}
प्रतिरूपम्<प्रकारनाम T, प्रकारनाम RT>
स्थिर अन्तःस्थ RT masked_Norm_Diff_L1_पुच्छ(ध्रुव T* s1, ध्रुव T* s2, ध्रुव uchar* आवरणम्, पूर्णाङ्क len, पूर्णाङ्क cn, RT परिणाम) {
    प्रति (पूर्णाङ्क i = 0; i < len; i++) यदि (आवरणम्[i]) {
        ध्रुव T* e1 = s1 + i*cn; ध्रुव T* e2 = s2 + i*cn;
        प्रति (पूर्णाङ्क k = 0; k < cn; k++) परिणाम += (RT)cv_absdiff(e1[k], e2[k]);
    }
    प्रत्यागम परिणाम;
}
प्रतिरूपम्<प्रकारनाम T, प्रकारनाम RT>
स्थिर अन्तःस्थ RT masked_Norm_Diff_L2_पुच्छ(ध्रुव T* s1, ध्रुव T* s2, ध्रुव uchar* आवरणम्, पूर्णाङ्क len, पूर्णाङ्क cn, RT परिणाम) {
    प्रति (पूर्णाङ्क i = 0; i < len; i++) यदि (आवरणम्[i]) {
        ध्रुव T* e1 = s1 + i*cn; ध्रुव T* e2 = s2 + i*cn;
        प्रति (पूर्णाङ्क k = 0; k < cn; k++) { RT v = (RT)e1[k] - (RT)e2[k]; परिणाम += v*v; }
    }
    प्रत्यागम परिणाम;
}

// Inf (cn==1): reduce_max of masked |a-b|. Shared across SIMD-capable depths.
प्रतिरूपम्<प्रकारनाम T, प्रकारनाम ST>
स्थिर अन्तःस्थ ST maskedNormDiffInfCn1(ध्रुव T* s1, ध्रुव T* s2, ध्रुव uchar* आवरणम्, पूर्णाङ्क len) {
    ध्रुव पूर्णाङ्क vstep = VTraits<decltype(vx_आरोपय(s1))>::vlanes();
    ST परिणाम = 0;
    पूर्णाङ्क i = 0;
    यदि (len >= vstep) {
        स्वचालित acc = v_diffmask(v_absdiff(vx_आरोपय(s1), vx_आरोपय(s2)), आवरणम्);
        प्रति (i = vstep; i <= len - vstep; i += vstep)
            acc = v_max(acc, v_diffmask(v_absdiff(vx_आरोपय(s1 + i), vx_आरोपय(s2 + i)), आवरणम् + i));
        परिणाम = (ST)v_reduce_max(acc);
    }
    प्रति (; i < len; i++) यदि (आवरणम्[i]) परिणाम = std::max(परिणाम, (ST)cv_absdiff(s1[i], s2[i]));
    प्रत्यागम परिणाम;
}

// Shared 8-bit cn==1 masked L1/L2 kernels (uchar/schar): v_absdiff yields v_uint8 for both.
प्रतिरूपम्<प्रकारनाम T>
स्थिर अन्तःस्थ पूर्णाङ्क maskedNormDiffL1_8(ध्रुव T* s1, ध्रुव T* s2, ध्रुव uchar* आवरणम्, पूर्णाङ्क len) {
    पूर्णाङ्क i = 0; ध्रुव पूर्णाङ्क vstep = VTraits<v_uint8>::vlanes();
    ध्रुव v_uint8 one = vx_setall_u8(1);
    v_uint32 acc = vx_setzero_u32();
    प्रति (; i <= len - vstep; i += vstep) {
        v_uint8 m  = v_gt(vx_आरोपय(आवरणम् + i), vx_setzero_u8());
        v_uint8 ad = v_and(v_absdiff(vx_आरोपय(s1 + i), vx_आरोपय(s2 + i)), m);
        acc = v_dotprod_expand_fast(ad, one, acc);
    }
    पूर्णाङ्क परिणाम = (पूर्णाङ्क)v_reduce_योग(acc);
    प्रति (; i < len; i++) यदि (आवरणम्[i]) परिणाम += (पूर्णाङ्क)cv_absdiff(s1[i], s2[i]);
    प्रत्यागम परिणाम;
}
प्रतिरूपम्<प्रकारनाम T>
स्थिर अन्तःस्थ पूर्णाङ्क maskedNormDiffL2_8(ध्रुव T* s1, ध्रुव T* s2, ध्रुव uchar* आवरणम्, पूर्णाङ्क len) {
    पूर्णाङ्क i = 0; ध्रुव पूर्णाङ्क vstep = VTraits<v_uint8>::vlanes();
    v_uint32 acc = vx_setzero_u32();
    प्रति (; i <= len - vstep; i += vstep) {
        v_uint8 m  = v_gt(vx_आरोपय(आवरणम् + i), vx_setzero_u8());
        v_uint8 ad = v_and(v_absdiff(vx_आरोपय(s1 + i), vx_आरोपय(s2 + i)), m);
        acc = v_dotprod_expand_fast(ad, ad, acc);
    }
    पूर्णाङ्क परिणाम = (पूर्णाङ्क)v_reduce_योग(acc);
    प्रति (; i < len; i++) यदि (आवरणम्[i]) { पूर्णाङ्क v = (पूर्णाङ्क)s1[i] - (पूर्णाङ्क)s2[i]; परिणाम += v*v; }
    प्रत्यागम परिणाम;
}

// Shared 16-bit cn==1 masked L1 kernel (ushort/short): v_absdiff yields v_uint16 for both.
प्रतिरूपम्<प्रकारनाम T>
स्थिर अन्तःस्थ पूर्णाङ्क maskedNormDiffL1_16(ध्रुव T* s1, ध्रुव T* s2, ध्रुव uchar* आवरणम्, पूर्णाङ्क len) {
    पूर्णाङ्क i = 0; ध्रुव पूर्णाङ्क vstep = VTraits<v_uint16>::vlanes();
    v_uint32 acc = vx_setzero_u32();
    प्रति (; i <= len - vstep; i += vstep) {
        v_uint16 m  = v_gt(vx_आरोपय_expand(आवरणम् + i), vx_setzero_u16());
        v_uint16 ad = v_and(v_absdiff(vx_आरोपय(s1 + i), vx_आरोपय(s2 + i)), m);
        v_uint32 lo, hi; v_expand(ad, lo, hi);
        acc = v_सङ्कलय(acc, v_सङ्कलय(lo, hi));
    }
    पूर्णाङ्क परिणाम = (पूर्णाङ्क)v_reduce_योग(acc);
    प्रति (; i < len; i++) यदि (आवरणम्[i]) परिणाम += (पूर्णाङ्क)cv_absdiff(s1[i], s2[i]);
    प्रत्यागम परिणाम;
}

प्रतिरूपम्<> संरचना MaskedNormDiffInf_SIMD<uchar, पूर्णाङ्क> {
    पूर्णाङ्क संक्रियकः()(ध्रुव uchar* s1, ध्रुव uchar* s2, ध्रुव uchar* आवरणम्, पूर्णाङ्क len, पूर्णाङ्क cn) ध्रुव {
        यदि (cn == 1) प्रत्यागम maskedNormDiffInfCn1<uchar, पूर्णाङ्क>(s1, s2, आवरणम्, len);
        यदि (cn == 4 && len >= VTraits<v_uint8>::vlanes()) {
            ध्रुव पूर्णाङ्क vstep = VTraits<v_uint8>::vlanes();
            v_uint8 acc = vx_setzero_u8();
            पूर्णाङ्क i = 0;
            प्रति (;;) {
                यदि (i > len - vstep) i = len - vstep; // back-step (max is idempotent)
                v_uint8 a0,a1,a2,a3,b0,b1,b2,b3;
                v_आरोपय_deinterleave(s1 + i*4, a0,a1,a2,a3);
                v_आरोपय_deinterleave(s2 + i*4, b0,b1,b2,b3);
                v_uint8 ad = v_max(v_max(v_absdiff(a0,b0), v_absdiff(a1,b1)),
                                   v_max(v_absdiff(a2,b2), v_absdiff(a3,b3)));
                v_uint8 m = v_gt(vx_आरोपय(आवरणम् + i), vx_setzero_u8());
                acc = v_max(acc, v_and(ad, m));
                यदि (i >= len - vstep) भङ्ग;
                i += vstep;
            }
            प्रत्यागम (पूर्णाङ्क)v_reduce_max(acc);
        }
        प्रत्यागम masked_Norm_Diff_Inf_पुच्छ<uchar, पूर्णाङ्क>(s1, s2, आवरणम्, len, cn, 0);
    }
};
प्रतिरूपम्<> संरचना MaskedNormDiffInf_SIMD<schar, पूर्णाङ्क> {
    पूर्णाङ्क संक्रियकः()(ध्रुव schar* s1, ध्रुव schar* s2, ध्रुव uchar* आवरणम्, पूर्णाङ्क len, पूर्णाङ्क cn) ध्रुव {
        यदि (cn == 1) प्रत्यागम maskedNormDiffInfCn1<schar, पूर्णाङ्क>(s1, s2, आवरणम्, len);
        प्रत्यागम masked_Norm_Diff_Inf_पुच्छ<schar, पूर्णाङ्क>(s1, s2, आवरणम्, len, cn, 0);
    }
};
प्रतिरूपम्<> संरचना MaskedNormDiffInf_SIMD<ushort, पूर्णाङ्क> {
    पूर्णाङ्क संक्रियकः()(ध्रुव ushort* s1, ध्रुव ushort* s2, ध्रुव uchar* आवरणम्, पूर्णाङ्क len, पूर्णाङ्क cn) ध्रुव {
        यदि (cn == 1) प्रत्यागम maskedNormDiffInfCn1<ushort, पूर्णाङ्क>(s1, s2, आवरणम्, len);
        प्रत्यागम masked_Norm_Diff_Inf_पुच्छ<ushort, पूर्णाङ्क>(s1, s2, आवरणम्, len, cn, 0);
    }
};
प्रतिरूपम्<> संरचना MaskedNormDiffInf_SIMD<ह्रस्व, पूर्णाङ्क> {
    पूर्णाङ्क संक्रियकः()(ध्रुव ह्रस्व* s1, ध्रुव ह्रस्व* s2, ध्रुव uchar* आवरणम्, पूर्णाङ्क len, पूर्णाङ्क cn) ध्रुव {
        यदि (cn == 1) प्रत्यागम maskedNormDiffInfCn1<ह्रस्व, पूर्णाङ्क>(s1, s2, आवरणम्, len);
        प्रत्यागम masked_Norm_Diff_Inf_पुच्छ<ह्रस्व, पूर्णाङ्क>(s1, s2, आवरणम्, len, cn, 0);
    }
};
प्रतिरूपम्<> संरचना MaskedNormDiffInf_SIMD<पूर्णाङ्क, अचिह्नित> {
    अचिह्नित संक्रियकः()(ध्रुव पूर्णाङ्क* s1, ध्रुव पूर्णाङ्क* s2, ध्रुव uchar* आवरणम्, पूर्णाङ्क len, पूर्णाङ्क cn) ध्रुव {
        यदि (cn == 1) प्रत्यागम maskedNormDiffInfCn1<पूर्णाङ्क, अचिह्नित>(s1, s2, आवरणम्, len);
        प्रत्यागम masked_Norm_Diff_Inf_पुच्छ<पूर्णाङ्क, अचिह्नित>(s1, s2, आवरणम्, len, cn, 0u);
    }
};
प्रतिरूपम्<> संरचना MaskedNormDiffInf_SIMD<भिन्न, भिन्न> {
    भिन्न संक्रियकः()(ध्रुव भिन्न* s1, ध्रुव भिन्न* s2, ध्रुव uchar* आवरणम्, पूर्णाङ्क len, पूर्णाङ्क cn) ध्रुव {
        यदि (cn == 1) प्रत्यागम maskedNormDiffInfCn1<भिन्न, भिन्न>(s1, s2, आवरणम्, len);
        प्रत्यागम masked_Norm_Diff_Inf_पुच्छ<भिन्न, भिन्न>(s1, s2, आवरणम्, len, cn, 0.0f);
    }
};

प्रतिरूपम्<> संरचना MaskedNormDiffL1_SIMD<uchar, पूर्णाङ्क> {
    पूर्णाङ्क संक्रियकः()(ध्रुव uchar* s1, ध्रुव uchar* s2, ध्रुव uchar* आवरणम्, पूर्णाङ्क len, पूर्णाङ्क cn) ध्रुव {
        यदि (cn == 1) प्रत्यागम maskedNormDiffL1_8(s1, s2, आवरणम्, len);
        यदि (cn == 4) {
            पूर्णाङ्क i = 0; ध्रुव पूर्णाङ्क vstep = VTraits<v_uint8>::vlanes();
            ध्रुव v_uint8 one = vx_setall_u8(1);
            v_uint32 acc = vx_setzero_u32();
            प्रति (; i <= len - vstep; i += vstep) {
                v_uint8 a0,a1,a2,a3,b0,b1,b2,b3;
                v_आरोपय_deinterleave(s1 + i*4, a0,a1,a2,a3);
                v_आरोपय_deinterleave(s2 + i*4, b0,b1,b2,b3);
                v_uint8 m = v_gt(vx_आरोपय(आवरणम् + i), vx_setzero_u8());
                acc = v_dotprod_expand_fast(v_and(v_absdiff(a0,b0), m), one, acc);
                acc = v_dotprod_expand_fast(v_and(v_absdiff(a1,b1), m), one, acc);
                acc = v_dotprod_expand_fast(v_and(v_absdiff(a2,b2), m), one, acc);
                acc = v_dotprod_expand_fast(v_and(v_absdiff(a3,b3), m), one, acc);
            }
            पूर्णाङ्क परिणाम = (पूर्णाङ्क)v_reduce_योग(acc);
            प्रति (; i < len; i++) यदि (आवरणम्[i]) {
                ध्रुव uchar* e1 = s1 + i*4; ध्रुव uchar* e2 = s2 + i*4;
                प्रति (पूर्णाङ्क k = 0; k < 4; k++) परिणाम += (पूर्णाङ्क)cv_absdiff(e1[k], e2[k]);
            }
            प्रत्यागम परिणाम;
        }
        प्रत्यागम masked_Norm_Diff_L1_पुच्छ<uchar, पूर्णाङ्क>(s1, s2, आवरणम्, len, cn, 0);
    }
};
प्रतिरूपम्<> संरचना MaskedNormDiffL1_SIMD<schar, पूर्णाङ्क> {
    पूर्णाङ्क संक्रियकः()(ध्रुव schar* s1, ध्रुव schar* s2, ध्रुव uchar* आवरणम्, पूर्णाङ्क len, पूर्णाङ्क cn) ध्रुव {
        यदि (cn == 1) प्रत्यागम maskedNormDiffL1_8(s1, s2, आवरणम्, len);
        प्रत्यागम masked_Norm_Diff_L1_पुच्छ<schar, पूर्णाङ्क>(s1, s2, आवरणम्, len, cn, 0);
    }
};
प्रतिरूपम्<> संरचना MaskedNormDiffL1_SIMD<ushort, पूर्णाङ्क> {
    पूर्णाङ्क संक्रियकः()(ध्रुव ushort* s1, ध्रुव ushort* s2, ध्रुव uchar* आवरणम्, पूर्णाङ्क len, पूर्णाङ्क cn) ध्रुव {
        यदि (cn == 1) प्रत्यागम maskedNormDiffL1_16(s1, s2, आवरणम्, len);
        प्रत्यागम masked_Norm_Diff_L1_पुच्छ<ushort, पूर्णाङ्क>(s1, s2, आवरणम्, len, cn, 0);
    }
};
प्रतिरूपम्<> संरचना MaskedNormDiffL1_SIMD<ह्रस्व, पूर्णाङ्क> {
    पूर्णाङ्क संक्रियकः()(ध्रुव ह्रस्व* s1, ध्रुव ह्रस्व* s2, ध्रुव uchar* आवरणम्, पूर्णाङ्क len, पूर्णाङ्क cn) ध्रुव {
        यदि (cn == 1) प्रत्यागम maskedNormDiffL1_16(s1, s2, आवरणम्, len);
        प्रत्यागम masked_Norm_Diff_L1_पुच्छ<ह्रस्व, पूर्णाङ्क>(s1, s2, आवरणम्, len, cn, 0);
    }
};

प्रतिरूपम्<> संरचना MaskedNormDiffL2_SIMD<uchar, पूर्णाङ्क> {
    पूर्णाङ्क संक्रियकः()(ध्रुव uchar* s1, ध्रुव uchar* s2, ध्रुव uchar* आवरणम्, पूर्णाङ्क len, पूर्णाङ्क cn) ध्रुव {
        यदि (cn == 1) प्रत्यागम maskedNormDiffL2_8(s1, s2, आवरणम्, len);
        यदि (cn == 4) {
            पूर्णाङ्क i = 0; ध्रुव पूर्णाङ्क vstep = VTraits<v_uint8>::vlanes();
            v_uint32 acc = vx_setzero_u32();
            प्रति (; i <= len - vstep; i += vstep) {
                v_uint8 a0,a1,a2,a3,b0,b1,b2,b3;
                v_आरोपय_deinterleave(s1 + i*4, a0,a1,a2,a3);
                v_आरोपय_deinterleave(s2 + i*4, b0,b1,b2,b3);
                v_uint8 m = v_gt(vx_आरोपय(आवरणम् + i), vx_setzero_u8());
                v_uint8 d0 = v_and(v_absdiff(a0,b0), m), d1 = v_and(v_absdiff(a1,b1), m);
                v_uint8 d2 = v_and(v_absdiff(a2,b2), m), d3 = v_and(v_absdiff(a3,b3), m);
                acc = v_dotprod_expand_fast(d0, d0, acc);
                acc = v_dotprod_expand_fast(d1, d1, acc);
                acc = v_dotprod_expand_fast(d2, d2, acc);
                acc = v_dotprod_expand_fast(d3, d3, acc);
            }
            पूर्णाङ्क परिणाम = (पूर्णाङ्क)v_reduce_योग(acc);
            प्रति (; i < len; i++) यदि (आवरणम्[i]) {
                ध्रुव uchar* e1 = s1 + i*4; ध्रुव uchar* e2 = s2 + i*4;
                प्रति (पूर्णाङ्क k = 0; k < 4; k++) { पूर्णाङ्क v = (पूर्णाङ्क)e1[k] - (पूर्णाङ्क)e2[k]; परिणाम += v*v; }
            }
            प्रत्यागम परिणाम;
        }
        प्रत्यागम masked_Norm_Diff_L2_पुच्छ<uchar, पूर्णाङ्क>(s1, s2, आवरणम्, len, cn, 0);
    }
};
प्रतिरूपम्<> संरचना MaskedNormDiffL2_SIMD<schar, पूर्णाङ्क> {
    पूर्णाङ्क संक्रियकः()(ध्रुव schar* s1, ध्रुव schar* s2, ध्रुव uchar* आवरणम्, पूर्णाङ्क len, पूर्णाङ्क cn) ध्रुव {
        यदि (cn == 1) प्रत्यागम maskedNormDiffL2_8(s1, s2, आवरणम्, len);
        प्रत्यागम masked_Norm_Diff_L2_पुच्छ<schar, पूर्णाङ्क>(s1, s2, आवरणम्, len, cn, 0);
    }
};

#यदि CV_SIMD_64F
// Depths whose L1/L2 accumulate in double.
प्रतिरूपम्<> संरचना MaskedNormDiffL1_SIMD<पूर्णाङ्क, द्विगुण> {
    द्विगुण संक्रियकः()(ध्रुव पूर्णाङ्क* s1, ध्रुव पूर्णाङ्क* s2, ध्रुव uchar* आवरणम्, पूर्णाङ्क len, पूर्णाङ्क cn) ध्रुव {
        यदि (cn != 1) प्रत्यागम masked_Norm_Diff_L1_पुच्छ<पूर्णाङ्क, द्विगुण>(s1, s2, आवरणम्, len, cn, 0.0);
        पूर्णाङ्क i = 0; ध्रुव पूर्णाङ्क vstep = VTraits<v_int32>::vlanes();
        v_float64 acc = vx_setzero_f64();
        प्रति (; i <= len - vstep; i += vstep) {
            v_uint32 m  = v_gt(vx_आरोपय_expand_q(आवरणम् + i), vx_setzero_u32());
            v_uint32 ad = v_and(v_absdiff(vx_आरोपय(s1 + i), vx_आरोपय(s2 + i)), m);
            v_uint64 e0, e1; v_expand(ad, e0, e1);
            acc = v_सङ्कलय(acc, v_cvt_f64(v_reinterpret_as_s64(e0)));
            acc = v_सङ्कलय(acc, v_cvt_f64(v_reinterpret_as_s64(e1)));
        }
        द्विगुण परिणाम = v_reduce_योग(acc);
        प्रति (; i < len; i++) यदि (आवरणम्[i]) परिणाम += (द्विगुण)cv_absdiff(s1[i], s2[i]);
        प्रत्यागम परिणाम;
    }
};
प्रतिरूपम्<> संरचना MaskedNormDiffL1_SIMD<भिन्न, द्विगुण> {
    द्विगुण संक्रियकः()(ध्रुव भिन्न* s1, ध्रुव भिन्न* s2, ध्रुव uchar* आवरणम्, पूर्णाङ्क len, पूर्णाङ्क cn) ध्रुव {
        यदि (cn != 1) प्रत्यागम masked_Norm_Diff_L1_पुच्छ<भिन्न, द्विगुण>(s1, s2, आवरणम्, len, cn, 0.0);
        पूर्णाङ्क i = 0; ध्रुव पूर्णाङ्क vstep = VTraits<v_float32>::vlanes();
        v_float64 acc0 = vx_setzero_f64(), acc1 = vx_setzero_f64();
        प्रति (; i <= len - vstep; i += vstep) {
            v_float32 m  = v_reinterpret_as_f32(v_gt(vx_आरोपय_expand_q(आवरणम् + i), vx_setzero_u32()));
            v_float32 ad = v_and(v_absdiff(vx_आरोपय(s1 + i), vx_आरोपय(s2 + i)), m);
            acc0 = v_सङ्कलय(acc0, v_cvt_f64(ad));
            acc1 = v_सङ्कलय(acc1, v_cvt_f64_उच्चः(ad));
        }
        द्विगुण परिणाम = v_reduce_योग(v_सङ्कलय(acc0, acc1));
        प्रति (; i < len; i++) यदि (आवरणम्[i]) परिणाम += (द्विगुण)cv_absdiff(s1[i], s2[i]);
        प्रत्यागम परिणाम;
    }
};

प्रतिरूपम्<प्रकारनाम T>
स्थिर अन्तःस्थ द्विगुण maskedNormDiffL2_16(ध्रुव T* s1, ध्रुव T* s2, ध्रुव uchar* आवरणम्, पूर्णाङ्क len, पूर्णाङ्क cn) {
    यदि (cn != 1) प्रत्यागम masked_Norm_Diff_L2_पुच्छ<T, द्विगुण>(s1, s2, आवरणम्, len, cn, 0.0);
    पूर्णाङ्क i = 0; ध्रुव पूर्णाङ्क vstep = VTraits<v_uint16>::vlanes();
    v_float64 acc = vx_setzero_f64();
    प्रति (; i <= len - vstep; i += vstep) {
        v_uint16 m  = v_gt(vx_आरोपय_expand(आवरणम् + i), vx_setzero_u16());
        v_uint16 ad = v_and(v_absdiff(vx_आरोपय(s1 + i), vx_आरोपय(s2 + i)), m);
        v_uint64 u = v_dotprod_expand_fast(ad, ad);
        acc = v_सङ्कलय(acc, v_cvt_f64(v_reinterpret_as_s64(u)));
    }
    द्विगुण परिणाम = v_reduce_योग(acc);
    प्रति (; i < len; i++) यदि (आवरणम्[i]) { द्विगुण v = (द्विगुण)s1[i] - (द्विगुण)s2[i]; परिणाम += v*v; }
    प्रत्यागम परिणाम;
}
प्रतिरूपम्<> संरचना MaskedNormDiffL2_SIMD<ushort, द्विगुण> {
    द्विगुण संक्रियकः()(ध्रुव ushort* s1, ध्रुव ushort* s2, ध्रुव uchar* आवरणम्, पूर्णाङ्क len, पूर्णाङ्क cn) ध्रुव
    { प्रत्यागम maskedNormDiffL2_16(s1, s2, आवरणम्, len, cn); } };
प्रतिरूपम्<> संरचना MaskedNormDiffL2_SIMD<ह्रस्व, द्विगुण> {
    द्विगुण संक्रियकः()(ध्रुव ह्रस्व* s1, ध्रुव ह्रस्व* s2, ध्रुव uchar* आवरणम्, पूर्णाङ्क len, पूर्णाङ्क cn) ध्रुव
    { प्रत्यागम maskedNormDiffL2_16(s1, s2, आवरणम्, len, cn); } };

प्रतिरूपम्<> संरचना MaskedNormDiffL2_SIMD<पूर्णाङ्क, द्विगुण> {
    द्विगुण संक्रियकः()(ध्रुव पूर्णाङ्क* s1, ध्रुव पूर्णाङ्क* s2, ध्रुव uchar* आवरणम्, पूर्णाङ्क len, पूर्णाङ्क cn) ध्रुव {
        यदि (cn != 1) प्रत्यागम masked_Norm_Diff_L2_पुच्छ<पूर्णाङ्क, द्विगुण>(s1, s2, आवरणम्, len, cn, 0.0);
        पूर्णाङ्क i = 0; ध्रुव पूर्णाङ्क vstep = VTraits<v_int32>::vlanes();
        v_float64 r0 = vx_setzero_f64(), r1 = vx_setzero_f64();
        प्रति (; i <= len - vstep; i += vstep) {
            v_uint32 m  = v_gt(vx_आरोपय_expand_q(आवरणम् + i), vx_setzero_u32());
            v_uint32 ad = v_and(v_absdiff(vx_आरोपय(s1 + i), vx_आरोपय(s2 + i)), m);
            v_uint64 e0, e1; v_expand(ad, e0, e1);
            v_float64 f0 = v_cvt_f64(v_reinterpret_as_s64(e0)), f1 = v_cvt_f64(v_reinterpret_as_s64(e1));
            r0 = v_fma(f0, f0, r0); r1 = v_fma(f1, f1, r1);
        }
        द्विगुण परिणाम = v_reduce_योग(v_सङ्कलय(r0, r1));
        प्रति (; i < len; i++) यदि (आवरणम्[i]) { द्विगुण v = (द्विगुण)s1[i] - (द्विगुण)s2[i]; परिणाम += v*v; }
        प्रत्यागम परिणाम;
    }
};
प्रतिरूपम्<> संरचना MaskedNormDiffL2_SIMD<भिन्न, द्विगुण> {
    द्विगुण संक्रियकः()(ध्रुव भिन्न* s1, ध्रुव भिन्न* s2, ध्रुव uchar* आवरणम्, पूर्णाङ्क len, पूर्णाङ्क cn) ध्रुव {
        यदि (cn != 1) प्रत्यागम masked_Norm_Diff_L2_पुच्छ<भिन्न, द्विगुण>(s1, s2, आवरणम्, len, cn, 0.0);
        पूर्णाङ्क i = 0; ध्रुव पूर्णाङ्क vstep = VTraits<v_float32>::vlanes();
        v_float64 r0 = vx_setzero_f64(), r1 = vx_setzero_f64();
        प्रति (; i <= len - vstep; i += vstep) {
            v_float32 m  = v_reinterpret_as_f32(v_gt(vx_आरोपय_expand_q(आवरणम् + i), vx_setzero_u32()));
            v_float32 ad = v_and(v_absdiff(vx_आरोपय(s1 + i), vx_आरोपय(s2 + i)), m);
            v_float64 f0 = v_cvt_f64(ad), f1 = v_cvt_f64_उच्चः(ad);
            r0 = v_fma(f0, f0, r0); r1 = v_fma(f1, f1, r1);
        }
        द्विगुण परिणाम = v_reduce_योग(v_सङ्कलय(r0, r1));
        प्रति (; i < len; i++) यदि (आवरणम्[i]) { द्विगुण v = (द्विगुण)s1[i] - (द्विगुण)s2[i]; परिणाम += v*v; }
        प्रत्यागम परिणाम;
    }
};
प्रतिरूपम्<>
संरचना MaskedNormDiffInf_SIMD<द्विगुण, द्विगुण> {
    अन्तःस्थ द्विगुण संक्रियकः()(ध्रुव द्विगुण* s1, ध्रुव द्विगुण* s2, ध्रुव uchar* आवरणम्, पूर्णाङ्क len, पूर्णाङ्क cn) ध्रुव {
        द्विगुण परिणाम = 0.0;
        पूर्णाङ्क i = 0;
#यदि CV_SIMD_64F || CV_SIMD_SCALABLE_64F
        यदि (cn == 1) {
            ध्रुव पूर्णाङ्क vstep = VTraits<v_float64>::vlanes();
            v_float64 acc0 = vx_setzero_f64(), acc1 = vx_setzero_f64();
            प्रति (; i <= len - 2*vstep; i += 2*vstep) {
                v_uint64 m0, m1;
                v_normmask_u64(आवरणम् + i, m0, m1);
                v_float64 d0 = v_absdiff(vx_आरोपय(s1 + i), vx_आरोपय(s2 + i));
                v_float64 d1 = v_absdiff(vx_आरोपय(s1 + i + vstep), vx_आरोपय(s2 + i + vstep));
                acc0 = v_max(acc0, v_reinterpret_as_f64(v_and(v_reinterpret_as_u64(d0), m0)));
                acc1 = v_max(acc1, v_reinterpret_as_f64(v_and(v_reinterpret_as_u64(d1), m1)));
            }
            द्विगुण t[VTraits<v_float64>::max_nlanes];
            vx_store(t, v_max(acc0, acc1));
            प्रति (पूर्णाङ्क k = 0; k < vstep; k++)
                परिणाम = std::max(परिणाम, t[k]);
        }
#यदि_अन्तः
        प्रति (; i < len; i++) यदि (आवरणम्[i]) {
            ध्रुव द्विगुण* e1 = s1 + i*cn; ध्रुव द्विगुण* e2 = s2 + i*cn;
            प्रति (पूर्णाङ्क k = 0; k < cn; k++) परिणाम = std::max(परिणाम, std::निरपेक्षम्(e1[k] - e2[k]));
        }
        प्रत्यागम परिणाम;
    }
};

प्रतिरूपम्<>
संरचना MaskedNormDiffL1_SIMD<द्विगुण, द्विगुण> {
    द्विगुण संक्रियकः()(ध्रुव द्विगुण* s1, ध्रुव द्विगुण* s2, ध्रुव uchar* आवरणम्, पूर्णाङ्क len, पूर्णाङ्क cn) ध्रुव {
        द्विगुण परिणाम = 0.0;
        यदि (cn == 1) {
            ध्रुव पूर्णाङ्क vstep = VTraits<v_float64>::vlanes();
            पूर्णाङ्क i = 0;
            v_float64 acc0 = vx_setzero_f64(), acc1 = vx_setzero_f64();
            प्रति (; i <= len - 2*vstep; i += 2*vstep) {
                v_uint64 m0, m1;
                v_normmask_u64(आवरणम् + i, m0, m1);
                v_float64 d0 = v_absdiff(vx_आरोपय(s1 + i), vx_आरोपय(s2 + i));
                v_float64 d1 = v_absdiff(vx_आरोपय(s1 + i + vstep), vx_आरोपय(s2 + i + vstep));
                acc0 = v_सङ्कलय(acc0, v_reinterpret_as_f64(v_and(v_reinterpret_as_u64(d0), m0)));
                acc1 = v_सङ्कलय(acc1, v_reinterpret_as_f64(v_and(v_reinterpret_as_u64(d1), m1)));
            }
            परिणाम = v_reduce_योग(v_सङ्कलय(acc0, acc1));
            प्रति (; i < len; i++) यदि (आवरणम्[i]) परिणाम += std::निरपेक्षम्(s1[i] - s2[i]);
        } अन्यथा {
            परिणाम = masked_Norm_Diff_L1_पुच्छ<द्विगुण, द्विगुण>(s1, s2, आवरणम्, len, cn, 0.0);
        }
        प्रत्यागम परिणाम;
    }
};

प्रतिरूपम्<>
संरचना MaskedNormDiffL2_SIMD<द्विगुण, द्विगुण> {
    द्विगुण संक्रियकः()(ध्रुव द्विगुण* s1, ध्रुव द्विगुण* s2, ध्रुव uchar* आवरणम्, पूर्णाङ्क len, पूर्णाङ्क cn) ध्रुव {
        द्विगुण परिणाम = 0.0;
        यदि (cn == 1) {
            ध्रुव पूर्णाङ्क vstep = VTraits<v_float64>::vlanes();
            पूर्णाङ्क i = 0;
            v_float64 acc0 = vx_setzero_f64(), acc1 = vx_setzero_f64();
            प्रति (; i <= len - 2*vstep; i += 2*vstep) {
                v_uint64 m0, m1;
                v_normmask_u64(आवरणम् + i, m0, m1);
                v_float64 d0 = v_sub(vx_आरोपय(s1 + i), vx_आरोपय(s2 + i));
                v_float64 d1 = v_sub(vx_आरोपय(s1 + i + vstep), vx_आरोपय(s2 + i + vstep));
                d0 = v_reinterpret_as_f64(v_and(v_reinterpret_as_u64(d0), m0));
                d1 = v_reinterpret_as_f64(v_and(v_reinterpret_as_u64(d1), m1));
                acc0 = v_fma(d0, d0, acc0);
                acc1 = v_fma(d1, d1, acc1);
            }
            परिणाम = v_reduce_योग(v_सङ्कलय(acc0, acc1));
            प्रति (; i < len; i++) यदि (आवरणम्[i]) { द्विगुण v = s1[i] - s2[i]; परिणाम += v*v; }
        } अन्यथा {
            परिणाम = masked_Norm_Diff_L2_पुच्छ<द्विगुण, द्विगुण>(s1, s2, आवरणम्, len, cn, 0.0);
        }
        प्रत्यागम परिणाम;
    }
};
#यदि_अन्तः // CV_SIMD_64F
#यदि_अन्तः // CV_SIMD

प्रतिरूपम्<प्रकारनाम T, प्रकारनाम ST> पूर्णाङ्क
normDiffInf_(ध्रुव T* src1, ध्रुव T* src2, ध्रुव uchar* आवरणम्, ST* परिणाम_2, पूर्णाङ्क len, पूर्णाङ्क cn) {
    ST परिणाम = *परिणाम_2;
    यदि( !आवरणम् ) {
        NormDiffInf_SIMD<T, ST> op;
        परिणाम = std::max(परिणाम, op(src1, src2, len*cn));
    } अन्यथा {
        MaskedNormDiffInf_SIMD<T, ST> op;
        परिणाम = std::max(परिणाम, op(src1, src2, आवरणम्, len, cn));
    }
    *परिणाम_2 = परिणाम;
    प्रत्यागम 0;
}

प्रतिरूपम्<प्रकारनाम T, प्रकारनाम ST> पूर्णाङ्क
normDiffL1_(ध्रुव T* src1, ध्रुव T* src2, ध्रुव uchar* आवरणम्, ST* परिणाम_2, पूर्णाङ्क len, पूर्णाङ्क cn) {
    ST परिणाम = *परिणाम_2;
    यदि( !आवरणम् ) {
        NormDiffL1_SIMD<T, ST> op;
        परिणाम += op(src1, src2, len*cn);
    } अन्यथा {
        MaskedNormDiffL1_SIMD<T, ST> op;
        परिणाम += op(src1, src2, आवरणम्, len, cn);
    }
    *परिणाम_2 = परिणाम;
    प्रत्यागम 0;
}

प्रतिरूपम्<प्रकारनाम T, प्रकारनाम ST> पूर्णाङ्क
normDiffL2_(ध्रुव T* src1, ध्रुव T* src2, ध्रुव uchar* आवरणम्, ST* परिणाम_2, पूर्णाङ्क len, पूर्णाङ्क cn) {
    ST परिणाम = *परिणाम_2;
    यदि( !आवरणम् ) {
        NormDiffL2_SIMD<T, ST> op;
        परिणाम += op(src1, src2, len*cn);
    } अन्यथा {
        MaskedNormDiffL2_SIMD<T, ST> op;
        परिणाम += op(src1, src2, आवरणम्, len, cn);
    }
    *परिणाम_2 = परिणाम;
    प्रत्यागम 0;
}

स्थिर पूर्णाङ्क
normDiffInf_Bool(ध्रुव uchar* src1, ध्रुव uchar* src2, ध्रुव uchar* आवरणम्, पूर्णाङ्क* परिणाम_2, पूर्णाङ्क len, पूर्णाङ्क cn)
{
    पूर्णाङ्क परिणाम = *परिणाम_2;
    यदि( !आवरणम् )
    {
        प्रति( पूर्णाङ्क i = 0; i < len*cn; i++ ) {
            परिणाम = std::max(परिणाम, (पूर्णाङ्क)((src1[i] != 0) != (src2[i] != 0)));
            यदि (परिणाम != 0)
                भङ्ग;
        }
    }
    अन्यथा
    {
        प्रति( पूर्णाङ्क i = 0; i < len; i++, src1 += cn, src2 += cn )
            यदि( आवरणम्[i] )
            {
                प्रति( पूर्णाङ्क k = 0; k < cn; k++ )
                    परिणाम = std::max(परिणाम, (पूर्णाङ्क)((src1[k] != 0) != (src2[k] != 0)));
                यदि (परिणाम != 0)
                    भङ्ग;
            }
    }
    *परिणाम_2 = परिणाम;
    प्रत्यागम 0;
}

स्थिर पूर्णाङ्क
normDiffL1_Bool(ध्रुव uchar* src1, ध्रुव uchar* src2, ध्रुव uchar* आवरणम्, पूर्णाङ्क* परिणाम_2, पूर्णाङ्क len, पूर्णाङ्क cn)
{
    पूर्णाङ्क परिणाम = *परिणाम_2;
    यदि( !आवरणम् )
    {
        प्रति( पूर्णाङ्क i = 0; i < len*cn; i++ )
            परिणाम += (पूर्णाङ्क)((src1[i] != 0) != (src2[i] != 0));
    }
    अन्यथा
    {
        प्रति( पूर्णाङ्क i = 0; i < len; i++, src1 += cn, src2 += cn )
            यदि( आवरणम्[i] )
            {
                प्रति( पूर्णाङ्क k = 0; k < cn; k++ )
                    परिणाम += (पूर्णाङ्क)((src1[k] != 0) != (src2[k] != 0));
            }
    }
    *परिणाम_2 = परिणाम;
    प्रत्यागम 0;
}

स्थिर पूर्णाङ्क
normDiffL2_Bool(ध्रुव uchar* src1, ध्रुव uchar* src2, ध्रुव uchar* आवरणम्, पूर्णाङ्क* परिणाम_2, पूर्णाङ्क len, पूर्णाङ्क cn)
{
    प्रत्यागम normDiffL1_Bool(src1, src2, आवरणम्, परिणाम_2, len, cn);
}

#परिभाषय CV_DEF_NORM_FUNC(L, suffix, type, ntype) \
    स्थिर पूर्णाङ्क norm##L##_##suffix(ध्रुव type* src, ध्रुव uchar* आवरणम्, ntype* r, पूर्णाङ्क len, पूर्णाङ्क cn) \
{ CV_INSTRUMENT_REGION(); प्रत्यागम norm##L##_(src, आवरणम्, r, len, cn); } \
    स्थिर पूर्णाङ्क normDiff##L##_##suffix(ध्रुव type* src1, ध्रुव type* src2, \
    ध्रुव uchar* आवरणम्, ntype* r, पूर्णाङ्क len, पूर्णाङ्क cn) \
{ प्रत्यागम normDiff##L##_(src1, src2, आवरणम्, r, (पूर्णाङ्क)len, cn); }

#परिभाषय CV_DEF_NORM_ALL(suffix, type, inftype, l1type, l2type) \
    CV_DEF_NORM_FUNC(Inf, suffix, type, inftype) \
    CV_DEF_NORM_FUNC(L1, suffix, type, l1type) \
    CV_DEF_NORM_FUNC(L2, suffix, type, l2type)

CV_DEF_NORM_ALL(8u, uchar, पूर्णाङ्क, पूर्णाङ्क, पूर्णाङ्क)
CV_DEF_NORM_ALL(8s, schar, पूर्णाङ्क, पूर्णाङ्क, पूर्णाङ्क)
CV_DEF_NORM_ALL(16u, ushort, पूर्णाङ्क, पूर्णाङ्क, द्विगुण)
CV_DEF_NORM_ALL(16s, ह्रस्व, पूर्णाङ्क, पूर्णाङ्क, द्विगुण)
CV_DEF_NORM_ALL(32u, अचिह्नित, अचिह्नित, द्विगुण, द्विगुण)
CV_DEF_NORM_ALL(32s, पूर्णाङ्क, अचिह्नित, द्विगुण, द्विगुण)
CV_DEF_NORM_ALL(32f, भिन्न, भिन्न, द्विगुण, द्विगुण)
CV_DEF_NORM_ALL(64f, द्विगुण, द्विगुण, द्विगुण, द्विगुण)
CV_DEF_NORM_ALL(64u, uint64, uint64, द्विगुण, द्विगुण)
CV_DEF_NORM_ALL(64s, int64, uint64, द्विगुण, द्विगुण)
CV_DEF_NORM_ALL(16f, hfloat, भिन्न, भिन्न, भिन्न)
CV_DEF_NORM_ALL(16bf, bfloat, भिन्न, भिन्न, भिन्न)

NormFunc getNormFunc(पूर्णाङ्क normType, पूर्णाङ्क गभीरता)
{
    CV_INSTRUMENT_REGION();

    स्थिर NormFunc normTab[3][CV_गभीरता_MAX] =
    {
        {
            normWrap<uchar, पूर्णाङ्क, normInf_8u>,
            normWrap<schar, पूर्णाङ्क, normInf_8s>,
            normWrap<ushort, पूर्णाङ्क, normInf_16u>,
            normWrap<ह्रस्व, पूर्णाङ्क, normInf_16s>,
            normWrap<पूर्णाङ्क, अचिह्नित, normInf_32s>,
            normWrap<भिन्न, भिन्न, normInf_32f>,
            normWrap<द्विगुण, द्विगुण, normInf_64f>,
            normWrap<hfloat, भिन्न, normInf_16f>,
            normWrap<bfloat, भिन्न, normInf_16bf>,
            normWrap<uchar, पूर्णाङ्क, normInf_Bool>,
            normWrap<uint64, uint64, normInf_64u>,
            normWrap<int64, uint64, normInf_64s>,
            normWrap<अचिह्नित, अचिह्नित, normInf_32u>,
            0
        },
        {
            normWrap<uchar, पूर्णाङ्क, normL1_8u>,
            normWrap<schar, पूर्णाङ्क, normL1_8s>,
            normWrap<ushort, पूर्णाङ्क, normL1_16u>,
            normWrap<ह्रस्व, पूर्णाङ्क, normL1_16s>,
            normWrap<पूर्णाङ्क, द्विगुण, normL1_32s>,
            normWrap<भिन्न, द्विगुण, normL1_32f>,
            normWrap<द्विगुण, द्विगुण, normL1_64f>,
            normWrap<hfloat, भिन्न, normL1_16f>,
            normWrap<bfloat, भिन्न, normL1_16bf>,
            normWrap<uchar, पूर्णाङ्क, normL1_Bool>,
            normWrap<uint64, द्विगुण, normL1_64u>,
            normWrap<int64, द्विगुण, normL1_64s>,
            normWrap<अचिह्नित, द्विगुण, normL1_32u>,
            0
        },
        {
            normWrap<uchar, पूर्णाङ्क, normL2_8u>,
            normWrap<schar, पूर्णाङ्क, normL2_8s>,
            normWrap<ushort, द्विगुण, normL2_16u>,
            normWrap<ह्रस्व, द्विगुण, normL2_16s>,
            normWrap<पूर्णाङ्क, द्विगुण, normL2_32s>,
            normWrap<भिन्न, द्विगुण, normL2_32f>,
            normWrap<द्विगुण, द्विगुण, normL2_64f>,
            normWrap<hfloat, भिन्न, normL2_16f>,
            normWrap<bfloat, भिन्न, normL2_16bf>,
            normWrap<uchar, पूर्णाङ्क, normL2_Bool>,
            normWrap<uint64, द्विगुण, normL2_64u>,
            normWrap<int64, द्विगुण, normL2_64s>,
            normWrap<अचिह्नित, द्विगुण, normL2_32u>,
            0
        }
    };

    यदि (normType >= 3 || normType < 0) प्रत्यागम शून्यसूचकः;

    प्रत्यागम normTab[normType][गभीरता];
}

NormDiffFunc getNormDiffFunc(पूर्णाङ्क normType, पूर्णाङ्क गभीरता)
{
    स्थिर NormDiffFunc normDiffTab[3][CV_गभीरता_MAX] =
    {
        {
            normDiffWrap<uchar, पूर्णाङ्क, normDiffInf_8u>,
            normDiffWrap<schar, पूर्णाङ्क, normDiffInf_8s>,
            normDiffWrap<ushort, पूर्णाङ्क, normDiffInf_16u>,
            normDiffWrap<ह्रस्व, पूर्णाङ्क, normDiffInf_16s>,
            normDiffWrap<पूर्णाङ्क, अचिह्नित, normDiffInf_32s>,
            normDiffWrap<भिन्न, भिन्न, normDiffInf_32f>,
            normDiffWrap<द्विगुण, द्विगुण, normDiffInf_64f>,
            normDiffWrap<hfloat, भिन्न, normDiffInf_16f>,
            normDiffWrap<bfloat, भिन्न, normDiffInf_16bf>,
            normDiffWrap<uchar, पूर्णाङ्क, normDiffInf_Bool>,
            normDiffWrap<uint64, uint64, normDiffInf_64u>,
            normDiffWrap<int64, uint64, normDiffInf_64s>,
            normDiffWrap<अचिह्नित, अचिह्नित, normDiffInf_32u>,
            0
        },
        {
            normDiffWrap<uchar, पूर्णाङ्क, normDiffL1_8u>,
            normDiffWrap<schar, पूर्णाङ्क, normDiffL1_8s>,
            normDiffWrap<ushort, पूर्णाङ्क, normDiffL1_16u>,
            normDiffWrap<ह्रस्व, पूर्णाङ्क, normDiffL1_16s>,
            normDiffWrap<पूर्णाङ्क, द्विगुण, normDiffL1_32s>,
            normDiffWrap<भिन्न, द्विगुण, normDiffL1_32f>,
            normDiffWrap<द्विगुण, द्विगुण, normDiffL1_64f>,
            normDiffWrap<hfloat, भिन्न, normDiffL1_16f>,
            normDiffWrap<bfloat, भिन्न, normDiffL1_16bf>,
            normDiffWrap<uchar, पूर्णाङ्क, normDiffL1_Bool>,
            normDiffWrap<uint64, द्विगुण, normDiffL1_64u>,
            normDiffWrap<int64, द्विगुण, normDiffL1_64s>,
            normDiffWrap<अचिह्नित, द्विगुण, normDiffL1_32u>,
            0
        },
        {
            normDiffWrap<uchar, पूर्णाङ्क, normDiffL2_8u>,
            normDiffWrap<schar, पूर्णाङ्क, normDiffL2_8s>,
            normDiffWrap<ushort, द्विगुण, normDiffL2_16u>,
            normDiffWrap<ह्रस्व, द्विगुण, normDiffL2_16s>,
            normDiffWrap<पूर्णाङ्क, द्विगुण, normDiffL2_32s>,
            normDiffWrap<भिन्न, द्विगुण, normDiffL2_32f>,
            normDiffWrap<द्विगुण, द्विगुण, normDiffL2_64f>,
            normDiffWrap<hfloat, भिन्न, normDiffL2_16f>,
            normDiffWrap<bfloat, भिन्न, normDiffL2_16bf>,
            normDiffWrap<uchar, पूर्णाङ्क, normDiffL2_Bool>,
            normDiffWrap<uint64, द्विगुण, normDiffL2_64u>,
            normDiffWrap<int64, द्विगुण, normDiffL2_64s>,
            normDiffWrap<अचिह्नित, द्विगुण, normDiffL2_32u>,
            0
        },
    };
    यदि (normType >= 3 || normType < 0) प्रत्यागम शून्यसूचकः;

    प्रत्यागम normDiffTab[normType][गभीरता];
}

#यदि_अन्तः // CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

CV_CPU_OPTIMIZATION_NAMESPACE_अन्तः

} // cv::
