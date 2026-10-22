// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html
//
// Copyright (C) 2025, SpaceMIT Inc., all rights reserved.
// Copyright (C) 2026, Advanced Micro Devices, Inc., all rights reserved.
// Third party copyrights are property of their respective owners.

#अन्तर्भाव "precomp.hpp"

नामाकाश cv {

using NormFunc = पूर्णांक (*)(स्थिरांक uchar*, स्थिरांक uchar*, uchar*, पूर्णांक, पूर्णांक);
using NormDiffFunc = पूर्णांक (*)(स्थिरांक uchar*, स्थिरांक uchar*, स्थिरांक uchar*, uchar*, पूर्णांक, पूर्णांक);

CV_CPU_OPTIMIZATION_NAMESPACE_BEGIN

ढाँचा<typename T, typename RT, पूर्णांक (*fn)(स्थिरांक T*, स्थिरांक uchar*, RT*, पूर्णांक, पूर्णांक)>
स्थिर पूर्णांक normWrap(स्थिरांक uchar* src1, स्थिरांक uchar* src2, uchar* res, पूर्णांक len, पूर्णांक cn)
{
    परत fn((स्थिरांक T*)src1, src2, (RT*)res, len, cn);
}

ढाँचा<typename T, typename RT, पूर्णांक (*fn)(स्थिरांक T*, स्थिरांक T*, स्थिरांक uchar*, RT*, पूर्णांक, पूर्णांक)>
स्थिर पूर्णांक normDiffWrap(स्थिरांक uchar* src1, स्थिरांक uchar* src2, स्थिरांक uchar* src3, uchar* res, पूर्णांक len, पूर्णांक cn)
{
    परत fn((स्थिरांक T*)src1, (स्थिरांक T*)src2, src3, (RT*)res, len, cn);
}

NormFunc getNormFunc(पूर्णांक normType, पूर्णांक depth);
NormDiffFunc getNormDiffFunc(पूर्णांक normType, पूर्णांक depth);

#ifndef CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

ढाँचा <typename T, typename ST>
संरचना NormInf_SIMD {
    ओळीत ST चालक() (स्थिरांक T* src, पूर्णांक n) स्थिरांक {
        ST s = 0;
        साठी (पूर्णांक i = 0; i < n; i++) {
            s = std::max(s, (ST)cv_abs(src[i]));
        }
        परत s;
    }
};

ढाँचा <typename T, typename ST>
संरचना NormL1_SIMD {
    ओळीत ST चालक() (स्थिरांक T* src, पूर्णांक n) स्थिरांक {
        ST s = 0;
        साठी (पूर्णांक i = 0; i < n; i++) {
            s += cv_abs(src[i]);
        }
        परत s;
    }
};

ढाँचा <typename T, typename ST>
संरचना NormL2_SIMD {
    ओळीत ST चालक() (स्थिरांक T* src, पूर्णांक n) स्थिरांक {
        ST s = 0;
        साठी (पूर्णांक i = 0; i < n; i++) {
            ST v = (ST)src[i];
            s += v * v;
        }
        परत s;
    }
};

ढाँचा <typename T, typename ST>
संरचना NormDiffInf_SIMD {
    ओळीत ST चालक() (स्थिरांक T* src1, स्थिरांक T* src2, पूर्णांक n) स्थिरांक {
        ST s = 0;
        साठी (पूर्णांक i = 0; i < n; i++) {
            ST v = (ST)cv_absdiff(src1[i], src2[i]);
            s = std::max(s, v);
        }
        परत s;
    }
};

ढाँचा <typename T, typename ST>
संरचना NormDiffL1_SIMD {
    ओळीत ST चालक() (स्थिरांक T* src1, स्थिरांक T* src2, पूर्णांक n) स्थिरांक {
        ST s = 0;
        साठी (पूर्णांक i = 0; i < n; i++) {
            ST v = (ST)cv_absdiff(src1[i], src2[i]);
            s += v;
        }
        परत s;
    }
};

ढाँचा <typename T, typename ST>
संरचना NormDiffL2_SIMD {
    ओळीत ST चालक() (स्थिरांक T* src1, स्थिरांक T* src2, पूर्णांक n) स्थिरांक {
        ST s = 0;
        साठी (पूर्णांक i = 0; i < n; i++) {
            ST v = (ST)src1[i] - (ST)src2[i];
            s += v * v;
        }
        परत s;
    }
};

#जर (CV_SIMD || CV_SIMD_SCALABLE)

ढाँचा<>
संरचना NormInf_SIMD<uchar, पूर्णांक> {
    पूर्णांक चालक() (स्थिरांक uchar* src, पूर्णांक n) स्थिरांक {
        पूर्णांक j = 0;
        पूर्णांक s = 0;
        v_uint8 r0 = vx_setzero_u8(), r1 = vx_setzero_u8();
        v_uint8 r2 = vx_setzero_u8(), r3 = vx_setzero_u8();
        साठी (; j <= n - 4 * VTraits<v_uint8>::vlanes(); j += 4 * VTraits<v_uint8>::vlanes()) {
            r0 = v_max(r0, vx_load(src + j                                 ));
            r1 = v_max(r1, vx_load(src + j +     VTraits<v_uint8>::vlanes()));
            r2 = v_max(r2, vx_load(src + j + 2 * VTraits<v_uint8>::vlanes()));
            r3 = v_max(r3, vx_load(src + j + 3 * VTraits<v_uint8>::vlanes()));
        }
        r0 = v_max(r0, v_max(r1, v_max(r2, r3)));
        साठी (; j < n; j++) {
            s = std::max(s, (पूर्णांक)src[j]);
        }
        परत std::max(s, (पूर्णांक)v_reduce_max(r0));
    }
};

ढाँचा<>
संरचना NormInf_SIMD<schar, पूर्णांक> {
    पूर्णांक चालक() (स्थिरांक schar* src, पूर्णांक n) स्थिरांक {
        पूर्णांक j = 0;
        पूर्णांक s = 0;
        v_uint8 r0 = vx_setzero_u8(), r1 = vx_setzero_u8();
        v_uint8 r2 = vx_setzero_u8(), r3 = vx_setzero_u8();
        साठी (; j <= n - 4 * VTraits<v_int8>::vlanes(); j += 4 * VTraits<v_int8>::vlanes()) {
            r0 = v_max(r0, v_abs(vx_load(src + j                                )));
            r1 = v_max(r1, v_abs(vx_load(src + j +     VTraits<v_int8>::vlanes())));
            r2 = v_max(r2, v_abs(vx_load(src + j + 2 * VTraits<v_int8>::vlanes())));
            r3 = v_max(r3, v_abs(vx_load(src + j + 3 * VTraits<v_int8>::vlanes())));
        }
        r0 = v_max(r0, v_max(r1, v_max(r2, r3)));
        साठी (; j < n; j++) {
            s = std::max(s, cv_abs(src[j]));
        }
        परत std::max(s, saturate_cast<पूर्णांक>(v_reduce_max(r0)));
    }
};

ढाँचा<>
संरचना NormInf_SIMD<ushort, पूर्णांक> {
    पूर्णांक चालक() (स्थिरांक ushort* src, पूर्णांक n) स्थिरांक {
        पूर्णांक j = 0;
        पूर्णांक s = 0;
        v_uint16 d0 = vx_setzero_u16(), d1 = vx_setzero_u16();
        v_uint16 d2 = vx_setzero_u16(), d3 = vx_setzero_u16();
        साठी (; j <= n - 4 * VTraits<v_uint16>::vlanes(); j += 4 * VTraits<v_uint16>::vlanes()) {
            d0 = v_max(d0, vx_load(src + j                                  ));
            d1 = v_max(d1, vx_load(src + j +     VTraits<v_uint16>::vlanes()));
            d2 = v_max(d2, vx_load(src + j + 2 * VTraits<v_uint16>::vlanes()));
            d3 = v_max(d3, vx_load(src + j + 3 * VTraits<v_uint16>::vlanes()));
        }
        d0 = v_max(d0, v_max(d1, v_max(d2, d3)));
        साठी (; j < n; j++) {
            s = std::max(s, (पूर्णांक)src[j]);
        }
        परत std::max(s, (पूर्णांक)v_reduce_max(d0));
    }
};

ढाँचा<>
संरचना NormInf_SIMD<लघु, पूर्णांक> {
    पूर्णांक चालक() (स्थिरांक लघु* src, पूर्णांक n) स्थिरांक {
        पूर्णांक j = 0;
        पूर्णांक s = 0;
        v_uint16 d0 = vx_setzero_u16(), d1 = vx_setzero_u16();
        v_uint16 d2 = vx_setzero_u16(), d3 = vx_setzero_u16();
        साठी (; j <= n - 4 * VTraits<v_int16>::vlanes(); j += 4 * VTraits<v_int16>::vlanes()) {
            d0 = v_max(d0, v_abs(vx_load(src + j                                  )));
            d1 = v_max(d1, v_abs(vx_load(src + j +     VTraits<v_int16>::vlanes())));
            d2 = v_max(d2, v_abs(vx_load(src + j + 2 * VTraits<v_int16>::vlanes())));
            d3 = v_max(d3, v_abs(vx_load(src + j + 3 * VTraits<v_int16>::vlanes())));
        }
        d0 = v_max(d0, v_max(d1, v_max(d2, d3)));
        साठी (; j < n; j++) {
            s = std::max(s, saturate_cast<पूर्णांक>(cv_abs(src[j])));
        }
        परत std::max(s, saturate_cast<पूर्णांक>(v_reduce_max(d0)));
    }
};

ढाँचा<>
संरचना NormInf_SIMD<पूर्णांक, पूर्णांक> {
    पूर्णांक चालक() (स्थिरांक पूर्णांक* src, पूर्णांक n) स्थिरांक {
        पूर्णांक j = 0;
        पूर्णांक s = 0;
        v_uint32 r0 = vx_setzero_u32(), r1 = vx_setzero_u32();
        v_uint32 r2 = vx_setzero_u32(), r3 = vx_setzero_u32();
        साठी (; j <= n - 4 * VTraits<v_int32>::vlanes(); j += 4 * VTraits<v_int32>::vlanes()) {
            r0 = v_max(r0, v_abs(vx_load(src + j                                 )));
            r1 = v_max(r1, v_abs(vx_load(src + j +     VTraits<v_int32>::vlanes())));
            r2 = v_max(r2, v_abs(vx_load(src + j + 2 * VTraits<v_int32>::vlanes())));
            r3 = v_max(r3, v_abs(vx_load(src + j + 3 * VTraits<v_int32>::vlanes())));
        }
        r0 = v_max(r0, v_max(r1, v_max(r2, r3)));
        s = std::max(s, saturate_cast<पूर्णांक>(v_reduce_max(r0)));
        साठी (; j < n; j++) {
            s = std::max(s, std::असल(src[j]));
        }
        परत s;
    }
};

ढाँचा<>
संरचना NormInf_SIMD<दशांश, दशांश> {
    दशांश चालक() (स्थिरांक दशांश* src, पूर्णांक n) स्थिरांक {
        पूर्णांक j = 0;
        दशांश s = 0.f;
        v_float32 r0 = vx_setzero_f32(), r1 = vx_setzero_f32();
        v_float32 r2 = vx_setzero_f32(), r3 = vx_setzero_f32();
        साठी (; j <= n - 4 * VTraits<v_float32>::vlanes(); j += 4 * VTraits<v_float32>::vlanes()) {
            r0 = v_max(r0, v_abs(vx_load(src + j                                   )));
            r1 = v_max(r1, v_abs(vx_load(src + j +     VTraits<v_float32>::vlanes())));
            r2 = v_max(r2, v_abs(vx_load(src + j + 2 * VTraits<v_float32>::vlanes())));
            r3 = v_max(r3, v_abs(vx_load(src + j + 3 * VTraits<v_float32>::vlanes())));
        }
        r0 = v_max(r0, v_max(r1, v_max(r2, r3)));
        साठी (; j < n; j++) {
            s = std::max(s, cv_abs(src[j]));
        }
        परत std::max(s, v_reduce_max(r0));
    }
};

ढाँचा<>
संरचना NormL1_SIMD<uchar, पूर्णांक> {
    पूर्णांक चालक() (स्थिरांक uchar* src, पूर्णांक n) स्थिरांक {
        पूर्णांक j = 0;
        पूर्णांक s = 0;
        v_uint32 r0 = vx_setzero_u32(), r1 = vx_setzero_u32();
        v_uint8  one = vx_setall_u8(1);
        साठी (; j<= n - 2 * VTraits<v_uint8>::vlanes(); j += 2 * VTraits<v_uint8>::vlanes()) {
            v_uint8 v0 = vx_load(src + j);
            r0 = v_dotprod_expand_fast(v0, one, r0);

            v_uint8 v1 = vx_load(src + j + VTraits<v_uint8>::vlanes());
            r1 = v_dotprod_expand_fast(v1, one, r1);
        }
        s += v_reduce_sum(v_add(r0, r1));
        साठी (; j < n; j++) {
            s += src[j];
        }
        परत s;
    }
};

ढाँचा<>
संरचना NormL1_SIMD<schar, पूर्णांक> {
    पूर्णांक चालक() (स्थिरांक schar* src, पूर्णांक n) स्थिरांक {
        पूर्णांक j = 0;
        पूर्णांक s = 0;
        v_uint32 r0 = vx_setzero_u32(), r1 = vx_setzero_u32();
        v_uint8  one = vx_setall_u8(1);
        साठी (; j<= n - 2 * VTraits<v_int8>::vlanes(); j += 2 * VTraits<v_int8>::vlanes()) {
            v_uint8 v0 = v_abs(vx_load(src + j));
            r0 = v_dotprod_expand_fast(v0, one, r0);

            v_uint8 v1 = v_abs(vx_load(src + j + VTraits<v_int8>::vlanes()));
            r1 = v_dotprod_expand_fast(v1, one, r1);
        }
        s += v_reduce_sum(v_add(r0, r1));
        साठी (; j < n; j++) {
            s += saturate_cast<पूर्णांक>(cv_abs(src[j]));
        }
        परत s;
    }
};

ढाँचा<>
संरचना NormL1_SIMD<ushort, पूर्णांक> {
    पूर्णांक चालक() (स्थिरांक ushort* src, पूर्णांक n) स्थिरांक {
        पूर्णांक j = 0;
        पूर्णांक s = 0;
        v_uint32 r00 = vx_setzero_u32(), r01 = vx_setzero_u32();
        v_uint32 r10 = vx_setzero_u32(), r11 = vx_setzero_u32();
        साठी (; j<= n - 2 * VTraits<v_uint16>::vlanes(); j += 2 * VTraits<v_uint16>::vlanes()) {
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
        s += (पूर्णांक)v_reduce_sum(v_add(v_add(v_add(r00, r01), r10), r11));
        साठी (; j < n; j++) {
            s += src[j];
        }
        परत s;
    }
};

ढाँचा<>
संरचना NormL1_SIMD<लघु, पूर्णांक> {
    पूर्णांक चालक() (स्थिरांक लघु* src, पूर्णांक n) स्थिरांक {
        पूर्णांक j = 0;
        पूर्णांक s = 0;
        v_uint32 r00 = vx_setzero_u32(), r01 = vx_setzero_u32();
        v_uint32 r10 = vx_setzero_u32(), r11 = vx_setzero_u32();
        साठी (; j<= n - 2 * VTraits<v_int16>::vlanes(); j += 2 * VTraits<v_int16>::vlanes()) {
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
        s += (पूर्णांक)v_reduce_sum(v_add(v_add(v_add(r00, r01), r10), r11));
        साठी (; j < n; j++) {
            s += saturate_cast<पूर्णांक>(cv_abs(src[j]));
        }
        परत s;
    }
};

ढाँचा<>
संरचना NormL2_SIMD<uchar, पूर्णांक> {
    पूर्णांक चालक() (स्थिरांक uchar* src, पूर्णांक n) स्थिरांक {
        पूर्णांक j = 0;
        पूर्णांक s = 0;
        v_uint32 r0 = vx_setzero_u32(), r1 = vx_setzero_u32();
        साठी (; j <= n - 2 * VTraits<v_uint8>::vlanes(); j += 2 * VTraits<v_uint8>::vlanes()) {
            v_uint8 v0 = vx_load(src + j);
            r0 = v_dotprod_expand_fast(v0, v0, r0);

            v_uint8 v1 = vx_load(src + j + VTraits<v_uint8>::vlanes());
            r1 = v_dotprod_expand_fast(v1, v1, r1);
        }
        s += v_reduce_sum(v_add(r0, r1));
        साठी (; j < n; j++) {
            पूर्णांक v = saturate_cast<पूर्णांक>(src[j]);
            s += v * v;
        }
        परत s;
    }
};

ढाँचा<>
संरचना NormL2_SIMD<schar, पूर्णांक> {
    पूर्णांक चालक() (स्थिरांक schar* src, पूर्णांक n) स्थिरांक {
        पूर्णांक j = 0;
        पूर्णांक s = 0;
        v_int32 r0 = vx_setzero_s32(), r1 = vx_setzero_s32();
        साठी (; j <= n - 2 * VTraits<v_int8>::vlanes(); j += 2 * VTraits<v_int8>::vlanes()) {
            v_int8 v0 = vx_load(src + j);
            r0 = v_dotprod_expand_fast(v0, v0, r0);
            v_int8 v1 = vx_load(src + j + VTraits<v_int8>::vlanes());
            r1 = v_dotprod_expand_fast(v1, v1, r1);
        }
        s += v_reduce_sum(v_add(r0, r1));
        साठी (; j < n; j++) {
            पूर्णांक v = saturate_cast<पूर्णांक>(src[j]);
            s += v * v;
        }
        परत s;
    }
};

ढाँचा<>
संरचना NormDiffInf_SIMD<uchar, पूर्णांक> {
    पूर्णांक चालक() (स्थिरांक uchar* src1, स्थिरांक uchar* src2, पूर्णांक n) स्थिरांक {
        पूर्णांक j = 0;
        पूर्णांक s = 0;
        v_uint8 r0 = vx_setzero_u8(), r1 = vx_setzero_u8();
        v_uint8 r2 = vx_setzero_u8(), r3 = vx_setzero_u8();
        साठी (; j <= n - 4 * VTraits<v_uint8>::vlanes(); j += 4 * VTraits<v_uint8>::vlanes()) {
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
        s = (पूर्णांक)v_reduce_max(v_max(v_max(v_max(r0, r1), r2), r3));
        साठी (; j < n; j++) {
            पूर्णांक v = (पूर्णांक)cv_absdiff(src1[j], src2[j]);
            s = std::max(s, v);
        }
        परत s;
    }
};

ढाँचा<>
संरचना NormDiffInf_SIMD<schar, पूर्णांक> {
    पूर्णांक चालक() (स्थिरांक schar* src1, स्थिरांक schar* src2, पूर्णांक n) स्थिरांक {
        पूर्णांक j = 0;
        पूर्णांक s = 0;
        v_uint8 r0 = vx_setzero_u8(), r1 = vx_setzero_u8();
        v_uint8 r2 = vx_setzero_u8(), r3 = vx_setzero_u8();
        साठी (; j <= n - 4 * VTraits<v_int8>::vlanes(); j += 4 * VTraits<v_int8>::vlanes()) {
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
        s = (पूर्णांक)v_reduce_max(v_max(v_max(v_max(r0, r1), r2), r3));
        साठी (; j < n; j++) {
            पूर्णांक v = (पूर्णांक)cv_absdiff(src1[j], src2[j]);
            s = std::max(s, v);
        }
        परत s;
    }
};

ढाँचा<>
संरचना NormDiffInf_SIMD<ushort, पूर्णांक> {
    पूर्णांक चालक() (स्थिरांक ushort* src1, स्थिरांक ushort* src2, पूर्णांक n) स्थिरांक {
        पूर्णांक j = 0;
        पूर्णांक s = 0;
        v_uint16 r0 = vx_setzero_u16(), r1 = vx_setzero_u16();
        v_uint16 r2 = vx_setzero_u16(), r3 = vx_setzero_u16();
        साठी (; j <= n - 4 * VTraits<v_uint16>::vlanes(); j += 4 * VTraits<v_uint16>::vlanes()) {
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
        s = (पूर्णांक)v_reduce_max(v_max(v_max(v_max(r0, r1), r2), r3));
        साठी (; j < n; j++) {
            पूर्णांक v = (पूर्णांक)cv_absdiff(src1[j], src2[j]);
            s = std::max(s, v);
        }
        परत s;
    }
};

ढाँचा<>
संरचना NormDiffInf_SIMD<लघु, पूर्णांक> {
    पूर्णांक चालक() (स्थिरांक लघु* src1, स्थिरांक लघु* src2, पूर्णांक n) स्थिरांक {
        पूर्णांक j = 0;
        पूर्णांक s = 0;
        v_uint16 r0 = vx_setzero_u16(), r1 = vx_setzero_u16();
        v_uint16 r2 = vx_setzero_u16(), r3 = vx_setzero_u16();
        साठी (; j <= n - 4 * VTraits<v_int16>::vlanes(); j += 4 * VTraits<v_int16>::vlanes()) {
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
        s = (पूर्णांक)v_reduce_max(v_max(v_max(v_max(r0, r1), r2), r3));
        साठी (; j < n; j++) {
            पूर्णांक v = (पूर्णांक)cv_absdiff(src1[j], src2[j]);
            s = std::max(s, v);
        }
        परत s;
    }
};

ढाँचा<>
संरचना NormDiffInf_SIMD<पूर्णांक, अचिन्हित> {
    अचिन्हित चालक() (स्थिरांक पूर्णांक* src1, स्थिरांक पूर्णांक* src2, पूर्णांक n) स्थिरांक {
        पूर्णांक j = 0;
        अचिन्हित s = 0;
        v_uint32 r0 = vx_setzero_u32(), r1 = vx_setzero_u32();
        v_uint32 r2 = vx_setzero_u32(), r3 = vx_setzero_u32();
        साठी (; j <= n - 4 * VTraits<v_int32>::vlanes(); j += 4 * VTraits<v_int32>::vlanes()) {
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
        s = (पूर्णांक)v_reduce_max(v_max(v_max(v_max(r0, r1), r2), r3));
        साठी (; j < n; j++) {
            अचिन्हित v = (अचिन्हित)cv_absdiff(src1[j], src2[j]);
            s = std::max(s, v);
        }
        परत s;
    }
};

ढाँचा<>
संरचना NormDiffInf_SIMD<दशांश, दशांश> {
    दशांश चालक() (स्थिरांक दशांश* src1, स्थिरांक दशांश* src2, पूर्णांक n) स्थिरांक {
        पूर्णांक j = 0;
        दशांश s = 0;
        v_float32 r0 = vx_setzero_f32(), r1 = vx_setzero_f32();
        साठी (; j <= n - 2 * VTraits<v_float32>::vlanes(); j += 2 * VTraits<v_float32>::vlanes()) {
            v_float32 v01 = vx_load(src1 + j), v02 = vx_load(src2 + j);
            r0 = v_max(r0, v_absdiff(v01, v02));

            v_float32 v11 = vx_load(src1 + j + VTraits<v_float32>::vlanes()),
                      v12 = vx_load(src2 + j + VTraits<v_float32>::vlanes());
            r1 = v_max(r1, v_absdiff(v11, v12));
        }
        s = v_reduce_max(v_max(r0, r1));
        साठी (; j < n; j++) {
            दशांश v = (दशांश)cv_absdiff(src1[j], src2[j]);
            s = std::max(s, v);
        }
        परत s;
    }
};

ढाँचा<>
संरचना NormDiffL1_SIMD<uchar, पूर्णांक> {
    पूर्णांक चालक() (स्थिरांक uchar* src1, स्थिरांक uchar* src2, पूर्णांक n) स्थिरांक {
        पूर्णांक j = 0;
        पूर्णांक s = 0;
        v_uint32 r0 = vx_setzero_u32(), r1 = vx_setzero_u32();
        v_uint8  one = vx_setall_u8(1);
        साठी (; j<= n - 2 * VTraits<v_uint8>::vlanes(); j += 2 * VTraits<v_uint8>::vlanes()) {
            v_uint8 v01 = vx_load(src1 + j), v02 = vx_load(src2 + j);
            r0 = v_dotprod_expand_fast(v_absdiff(v01, v02), one, r0);

            v_uint8 v11 = vx_load(src1 + j + VTraits<v_uint8>::vlanes()),
                    v12 = vx_load(src2 + j + VTraits<v_uint8>::vlanes());
            r1 = v_dotprod_expand_fast(v_absdiff(v11, v12), one, r1);
        }
        s += v_reduce_sum(v_add(r0, r1));
        साठी (; j < n; j++) {
            पूर्णांक v = (पूर्णांक)cv_absdiff(src1[j], src2[j]);
            s += v;
        }
        परत s;
    }
};

ढाँचा<>
संरचना NormDiffL1_SIMD<schar, पूर्णांक> {
    पूर्णांक चालक() (स्थिरांक schar* src1, स्थिरांक schar* src2, पूर्णांक n) स्थिरांक {
        पूर्णांक j = 0;
        पूर्णांक s = 0;
        v_uint32 r0 = vx_setzero_u32(), r1 = vx_setzero_u32();
        v_uint8  one = vx_setall_u8(1);
        साठी (; j<= n - 2 * VTraits<v_int8>::vlanes(); j += 2 * VTraits<v_int8>::vlanes()) {
            v_int8 v01 = vx_load(src1 + j), v02 = vx_load(src2 + j);
            r0 = v_dotprod_expand_fast(v_absdiff(v01, v02), one, r0);

            v_int8 v11 = vx_load(src1 + j + VTraits<v_int8>::vlanes()),
                   v12 = vx_load(src2 + j + VTraits<v_int8>::vlanes());
            r1 = v_dotprod_expand_fast(v_absdiff(v11, v12), one, r1);
        }
        s += v_reduce_sum(v_add(r0, r1));
        साठी (; j < n; j++) {
            पूर्णांक v = (पूर्णांक)cv_absdiff(src1[j], src2[j]);
            s += v;
        }
        परत s;
    }
};

ढाँचा<>
संरचना NormDiffL1_SIMD<ushort, पूर्णांक> {
    पूर्णांक चालक() (स्थिरांक ushort* src1, स्थिरांक ushort* src2, पूर्णांक n) स्थिरांक {
        पूर्णांक j = 0;
        पूर्णांक s = 0;
        v_uint32 r0 = vx_setzero_u32(), r1 = vx_setzero_u32();
        v_uint32 r2 = vx_setzero_u32(), r3 = vx_setzero_u32();
        साठी (; j<= n - 4 * VTraits<v_uint16>::vlanes(); j += 4 * VTraits<v_uint16>::vlanes()) {
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
        s += (पूर्णांक)v_reduce_sum(v_add(v_add(v_add(r0, r1), r2), r3));
        साठी (; j < n; j++) {
            पूर्णांक v = (पूर्णांक)cv_absdiff(src1[j], src2[j]);
            s += v;
        }
        परत s;
    }
};

ढाँचा<>
संरचना NormDiffL1_SIMD<लघु, पूर्णांक> {
    पूर्णांक चालक() (स्थिरांक लघु* src1, स्थिरांक लघु* src2, पूर्णांक n) स्थिरांक {
        पूर्णांक j = 0;
        पूर्णांक s = 0;
        v_uint32 r0 = vx_setzero_u32(), r1 = vx_setzero_u32();
        v_uint32 r2 = vx_setzero_u32(), r3 = vx_setzero_u32();
        साठी (; j<= n - 4 * VTraits<v_int16>::vlanes(); j += 4 * VTraits<v_int16>::vlanes()) {
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
        s += (पूर्णांक)v_reduce_sum(v_add(v_add(v_add(r0, r1), r2), r3));
        साठी (; j < n; j++) {
            पूर्णांक v = (पूर्णांक)cv_absdiff(src1[j], src2[j]);
            s += v;
        }
        परत s;
    }
};

ढाँचा<>
संरचना NormDiffL2_SIMD<uchar, पूर्णांक> {
    पूर्णांक चालक() (स्थिरांक uchar* src1, स्थिरांक uchar* src2, पूर्णांक n) स्थिरांक {
        पूर्णांक j = 0;
        पूर्णांक s = 0;
        v_uint32 r0 = vx_setzero_u32(), r1 = vx_setzero_u32();
        साठी (; j <= n - 2 * VTraits<v_uint8>::vlanes(); j += 2 * VTraits<v_uint8>::vlanes()) {
            v_uint8 v01 = vx_load(src1 + j), v02 = vx_load(src2 + j);
            v_uint8 v0 = v_absdiff(v01, v02);
            r0 = v_dotprod_expand_fast(v0, v0, r0);

            v_uint8 v11 = vx_load(src1 + j + VTraits<v_uint8>::vlanes()),
                    v12 = vx_load(src2 + j + VTraits<v_uint8>::vlanes());
            v_uint8 v1 = v_absdiff(v11, v12);
            r1 = v_dotprod_expand_fast(v1, v1, r1);
        }
        s += v_reduce_sum(v_add(r0, r1));
        साठी (; j < n; j++) {
            पूर्णांक v = (पूर्णांक)src1[j] - (पूर्णांक)src2[j];
            s += v * v;
        }
        परत s;
    }
};

ढाँचा<>
संरचना NormDiffL2_SIMD<schar, पूर्णांक> {
    पूर्णांक चालक() (स्थिरांक schar* src1, स्थिरांक schar* src2, पूर्णांक n) स्थिरांक {
        पूर्णांक j = 0;
        पूर्णांक s = 0;
        v_uint32 r0 = vx_setzero_u32(), r1 = vx_setzero_u32();
        साठी (; j <= n - 2 * VTraits<v_int8>::vlanes(); j += 2 * VTraits<v_int8>::vlanes()) {
            v_int8 v01 = vx_load(src1 + j), v02 = vx_load(src2 + j);
            v_uint8 v0 = v_absdiff(v01, v02);
            r0 = v_dotprod_expand_fast(v0, v0, r0);

            v_int8 v11 = vx_load(src1 + j + VTraits<v_int8>::vlanes()),
                   v12 = vx_load(src2 + j + VTraits<v_int8>::vlanes());
            v_uint8 v1 = v_absdiff(v11, v12);
            r1 = v_dotprod_expand_fast(v1, v1, r1);
        }
        s += v_reduce_sum(v_add(r0, r1));
        साठी (; j < n; j++) {
            पूर्णांक v = (पूर्णांक)src1[j] - (पूर्णांक)src2[j];
            s += v * v;
        }
        परत s;
    }
};

#endif

#जर (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)

ढाँचा<>
संरचना NormInf_SIMD<द्विगुण, द्विगुण> {
    द्विगुण चालक() (स्थिरांक द्विगुण* src, पूर्णांक n) स्थिरांक {
        पूर्णांक j = 0;
        द्विगुण s = 0.f;
        v_float64 r0 = vx_setzero_f64(), r1 = vx_setzero_f64();
        v_float64 r2 = vx_setzero_f64(), r3 = vx_setzero_f64();
        साठी (; j <= n - 4 * VTraits<v_float64>::vlanes(); j += 4 * VTraits<v_float64>::vlanes()) {
            r0 = v_max(r0, v_abs(vx_load(src + j                                   )));
            r1 = v_max(r1, v_abs(vx_load(src + j +     VTraits<v_float64>::vlanes())));
            r2 = v_max(r2, v_abs(vx_load(src + j + 2 * VTraits<v_float64>::vlanes())));
            r3 = v_max(r3, v_abs(vx_load(src + j + 3 * VTraits<v_float64>::vlanes())));
        }
        r0 = v_max(r0, v_max(r1, v_max(r2, r3)));
        साठी (; j < n; j++) {
            s = std::max(s, cv_abs(src[j]));
        }
        // [TODO]: use v_reduce_max when it supports float64
        द्विगुण t[VTraits<v_float64>::max_nlanes];
        vx_store(t, r0);
        साठी (पूर्णांक i = 0; i < VTraits<v_float64>::vlanes(); i++) {
            s = std::max(s, cv_abs(t[i]));
        }
        परत s;
    }
};

ढाँचा<>
संरचना NormL1_SIMD<पूर्णांक, द्विगुण> {
    द्विगुण चालक() (स्थिरांक पूर्णांक* src, पूर्णांक n) स्थिरांक {
        पूर्णांक j = 0;
        द्विगुण s = 0.f;
        v_float64 r00 = vx_setzero_f64(), r01 = vx_setzero_f64();
        v_float64 r10 = vx_setzero_f64(), r11 = vx_setzero_f64();
        साठी (; j <= n - 2 * VTraits<v_int32>::vlanes(); j += 2 * VTraits<v_int32>::vlanes()) {
            // Widen to double before taking the magnitude: exact over the whole int32 range,
            // INT_MIN included, which a float intermediate would not be above 2^24.
            v_int32 i0 = vx_load(src + j), i1 = vx_load(src + j + VTraits<v_int32>::vlanes());
            r00 = v_add(r00, v_abs(v_cvt_f64(i0))); r01 = v_add(r01, v_abs(v_cvt_f64_वरचा(i0)));
            r10 = v_add(r10, v_abs(v_cvt_f64(i1))); r11 = v_add(r11, v_abs(v_cvt_f64_वरचा(i1)));
        }
        s += v_reduce_sum(v_add(v_add(v_add(r00, r01), r10), r11));
        साठी (; j < n; j++) {
            s += cv_abs(src[j]);
        }
        परत s;
    }
};

ढाँचा<>
संरचना NormL1_SIMD<दशांश, द्विगुण> {
    द्विगुण चालक() (स्थिरांक दशांश* src, पूर्णांक n) स्थिरांक {
        पूर्णांक j = 0;
        द्विगुण s = 0.f;
        v_float64 r00 = vx_setzero_f64(), r01 = vx_setzero_f64();
        v_float64 r10 = vx_setzero_f64(), r11 = vx_setzero_f64();
        v_float64 r20 = vx_setzero_f64(), r21 = vx_setzero_f64();
        v_float64 r30 = vx_setzero_f64(), r31 = vx_setzero_f64();
        साठी (; j <= n - 4 * VTraits<v_float32>::vlanes(); j += 4 * VTraits<v_float32>::vlanes()) {
            v_float32 v0 = v_abs(vx_load(src + j)), v1 = v_abs(vx_load(src + j + VTraits<v_float32>::vlanes()));
            r00 = v_add(r00, v_cvt_f64(v0)); r01 = v_add(r01, v_cvt_f64_वरचा(v0));
            r10 = v_add(r10, v_cvt_f64(v1)); r11 = v_add(r11, v_cvt_f64_वरचा(v1));

            v_float32 v2 = v_abs(vx_load(src + j + 2 * VTraits<v_float32>::vlanes())), v3 = v_abs(vx_load(src + j + 3 * VTraits<v_float32>::vlanes()));
            r20 = v_add(r20, v_cvt_f64(v2)); r21 = v_add(r21, v_cvt_f64_वरचा(v2));
            r30 = v_add(r30, v_cvt_f64(v3)); r31 = v_add(r31, v_cvt_f64_वरचा(v3));
        }
        s += v_reduce_sum(v_add(v_add(v_add(r00, r01), r10), r11));
        s += v_reduce_sum(v_add(v_add(v_add(r20, r21), r30), r31));
        साठी (; j < n; j++) {
            s += cv_abs(src[j]);
        }
        परत s;
    }
};

ढाँचा<>
संरचना NormL2_SIMD<ushort, द्विगुण> {
    द्विगुण चालक() (स्थिरांक ushort* src, पूर्णांक n) स्थिरांक {
        पूर्णांक j = 0;
        द्विगुण s = 0.f;
        v_float64 r0 = vx_setzero_f64(), r1 = vx_setzero_f64();
        साठी (; j <= n - 2 * VTraits<v_uint16>::vlanes(); j += 2 * VTraits<v_uint16>::vlanes()) {
            v_uint16 v0 = vx_load(src + j);
            v_uint64 u0 = v_dotprod_expand_fast(v0, v0);
            r0 = v_add(r0, v_cvt_f64(v_reinterpret_as_s64(u0)));

            v_uint16 v1 = vx_load(src + j + VTraits<v_uint16>::vlanes());
            v_uint64 u1 = v_dotprod_expand_fast(v1, v1);
            r1 = v_add(r1, v_cvt_f64(v_reinterpret_as_s64(u1)));
        }
        s += v_reduce_sum(v_add(r0, r1));
        साठी (; j < n; j++) {
            द्विगुण v = saturate_cast<द्विगुण>(src[j]);
            s += v * v;
        }
        परत s;
    }
};

ढाँचा<>
संरचना NormL2_SIMD<लघु, द्विगुण> {
    द्विगुण चालक() (स्थिरांक लघु* src, पूर्णांक n) स्थिरांक {
        पूर्णांक j = 0;
        द्विगुण s = 0.f;
        v_float64 r0 = vx_setzero_f64(), r1 = vx_setzero_f64();
        साठी (; j <= n - 2 * VTraits<v_int16>::vlanes(); j += 2 * VTraits<v_int16>::vlanes()) {
            v_int16 v0 = vx_load(src + j);
            r0 = v_add(r0, v_cvt_f64(v_dotprod_expand_fast(v0, v0)));

            v_int16 v1 = vx_load(src + j + VTraits<v_int16>::vlanes());
            r1 = v_add(r1, v_cvt_f64(v_dotprod_expand_fast(v1, v1)));
        }
        s += v_reduce_sum(v_add(r0, r1));
        साठी (; j < n; j++) {
            द्विगुण v = saturate_cast<द्विगुण>(src[j]);
            s += v * v;
        }
        परत s;
    }
};

ढाँचा<>
संरचना NormL2_SIMD<पूर्णांक, द्विगुण> {
    द्विगुण चालक() (स्थिरांक पूर्णांक* src, पूर्णांक n) स्थिरांक {
        पूर्णांक j = 0;
        द्विगुण s = 0.f;
        v_float64 r0 = vx_setzero_f64(), r1 = vx_setzero_f64();
        साठी (; j <= n - 2 * VTraits<v_int32>::vlanes(); j += 2 * VTraits<v_int32>::vlanes()) {
            v_int32 v0 = vx_load(src + j);
            r0 = v_dotprod_expand_fast(v0, v0, r0);

            v_int32 v1 = vx_load(src + j + VTraits<v_int32>::vlanes());
            r1 = v_dotprod_expand_fast(v1, v1, r1);
        }
        s += v_reduce_sum(v_add(r0, r1));
        साठी (; j < n; j++) {
            द्विगुण v = src[j];
            s += v * v;
        }
        परत s;
    }
};

ढाँचा<>
संरचना NormL2_SIMD<दशांश, द्विगुण> {
    द्विगुण चालक() (स्थिरांक दशांश* src, पूर्णांक n) स्थिरांक {
        पूर्णांक j = 0;
        द्विगुण s = 0.f;
        v_float64 r00 = vx_setzero_f64(), r01 = vx_setzero_f64();
        v_float64 r10 = vx_setzero_f64(), r11 = vx_setzero_f64();
        साठी (; j <= n - 2 * VTraits<v_float32>::vlanes(); j += 2 * VTraits<v_float32>::vlanes()) {
            v_float32 v0 = vx_load(src + j), v1 = vx_load(src + j + VTraits<v_float32>::vlanes());
            v_float64 v00 = v_cvt_f64(v0), v01 = v_cvt_f64_वरचा(v0);
            v_float64 v10 = v_cvt_f64(v1), v11 = v_cvt_f64_वरचा(v1);
            r00 = v_fma(v00, v00, r00); r01 = v_fma(v01, v01, r01);
            r10 = v_fma(v10, v10, r10); r11 = v_fma(v11, v11, r11);
        }
        s += v_reduce_sum(v_add(v_add(v_add(r00, r01), r10), r11));
        साठी (; j < n; j++) {
            द्विगुण v = src[j];
            s += v * v;
        }
        परत s;
    }
};

#endif

#जर CV_SIMD_64F // CV_SIMD_SCALABLE_64F has accuracy problem with the following kernels on ci

ढाँचा<>
संरचना NormL1_SIMD<द्विगुण, द्विगुण> {
    द्विगुण चालक() (स्थिरांक द्विगुण* src, पूर्णांक n) स्थिरांक {
        पूर्णांक j = 0;
        द्विगुण s = 0.f;
        v_float64 r00 = vx_setzero_f64(), r01 = vx_setzero_f64();
        v_float64 r10 = vx_setzero_f64(), r11 = vx_setzero_f64();
        साठी (; j <= n - 4 * VTraits<v_float64>::vlanes(); j += 4 * VTraits<v_float64>::vlanes()) {
            r00 = v_add(r00, v_abs(vx_load(src + j                                   )));
            r01 = v_add(r01, v_abs(vx_load(src + j +     VTraits<v_float64>::vlanes())));
            r10 = v_add(r10, v_abs(vx_load(src + j + 2 * VTraits<v_float64>::vlanes())));
            r11 = v_add(r11, v_abs(vx_load(src + j + 3 * VTraits<v_float64>::vlanes())));
        }
        s += v_reduce_sum(v_add(v_add(v_add(r00, r01), r10), r11));
        साठी (; j < n; j++) {
            s += cv_abs(src[j]);
        }
        परत s;
    }
};

ढाँचा<>
संरचना NormL2_SIMD<द्विगुण, द्विगुण> {
    द्विगुण चालक() (स्थिरांक द्विगुण* src, पूर्णांक n) स्थिरांक {
        पूर्णांक j = 0;
        द्विगुण s = 0.f;
        v_float64 r00 = vx_setzero_f64(), r01 = vx_setzero_f64();
        v_float64 r10 = vx_setzero_f64(), r11 = vx_setzero_f64();
        साठी (; j <= n - 4 * VTraits<v_float64>::vlanes(); j += 4 * VTraits<v_float64>::vlanes()) {
            v_float64 v00 = vx_load(src + j                                   );
            v_float64 v01 = vx_load(src + j +     VTraits<v_float64>::vlanes());
            v_float64 v10 = vx_load(src + j + 2 * VTraits<v_float64>::vlanes());
            v_float64 v11 = vx_load(src + j + 3 * VTraits<v_float64>::vlanes());
            r00 = v_fma(v00, v00, r00); r01 = v_fma(v01, v01, r01);
            r10 = v_fma(v10, v10, r10); r11 = v_fma(v11, v11, r11);
        }
        s += v_reduce_sum(v_add(v_add(v_add(r00, r01), r10), r11));
        साठी (; j < n; j++) {
            द्विगुण v = src[j];
            s += v * v;
        }
        परत s;
    }
};

ढाँचा<>
संरचना NormDiffInf_SIMD<द्विगुण, द्विगुण> {
    द्विगुण चालक() (स्थिरांक द्विगुण* src1, स्थिरांक द्विगुण* src2, पूर्णांक n) स्थिरांक {
        पूर्णांक j = 0;
        द्विगुण s = 0;
        v_float64 r0 = vx_setzero_f64(), r1 = vx_setzero_f64();
        साठी (; j <= n - 2 * VTraits<v_float64>::vlanes(); j += 2 * VTraits<v_float64>::vlanes()) {
            v_float64 v01 = vx_load(src1 + j), v02 = vx_load(src2 + j);
            r0 = v_max(r0, v_absdiff(v01, v02));

            v_float64 v11 = vx_load(src1 + j + VTraits<v_float64>::vlanes()),
                      v12 = vx_load(src2 + j + VTraits<v_float64>::vlanes());
            r1 = v_max(r1, v_absdiff(v11, v12));
        }
        // [TODO]: use v_reduce_max when it supports float64
        द्विगुण t[VTraits<v_float64>::max_nlanes];
        vx_store(t, v_max(r0, r1));
        साठी (पूर्णांक i = 0; i < VTraits<v_float64>::vlanes(); i++) {
            s = std::max(s, t[i]);
        }
        साठी (; j < n; j++) {
            द्विगुण v = (द्विगुण)cv_absdiff(src1[j], src2[j]);
            s = std::max(s, v);
        }
        परत s;
    }
};

ढाँचा<>
संरचना NormDiffL1_SIMD<दशांश, द्विगुण> {
    द्विगुण चालक() (स्थिरांक दशांश* src1, स्थिरांक दशांश* src2, पूर्णांक n) स्थिरांक {
        पूर्णांक j = 0;
        द्विगुण s = 0.f;
        v_float64 r0 = vx_setzero_f64(), r1 = vx_setzero_f64();
        v_float64 r2 = vx_setzero_f64(), r3 = vx_setzero_f64();
        साठी (; j <= n - 2 * VTraits<v_float32>::vlanes(); j += 2 * VTraits<v_float32>::vlanes()) {
            v_float32 v01 = vx_load(src1 + j), v02 = vx_load(src2 + j);
            v_float32 v0 = v_absdiff(v01, v02);
            r0 = v_add(r0, v_cvt_f64(v0)); r1 = v_add(r1, v_cvt_f64_वरचा(v0));

            v_float32 v11 = vx_load(src1 + j + VTraits<v_float32>::vlanes()),
                      v12 = vx_load(src2 + j + VTraits<v_float32>::vlanes());
            v_float32 v1 = v_absdiff(v11, v12);
            r2 = v_add(r2, v_cvt_f64(v1)); r3 = v_add(r3, v_cvt_f64_वरचा(v1));
        }
        s += v_reduce_sum(v_add(v_add(v_add(r0, r1), r2), r3));
        साठी (; j < n; j++) {
            द्विगुण v = (द्विगुण)cv_absdiff(src1[j], src2[j]);
            s += v;
        }
        परत s;
    }
};

ढाँचा<>
संरचना NormDiffL1_SIMD<द्विगुण, द्विगुण> {
    द्विगुण चालक() (स्थिरांक द्विगुण* src1, स्थिरांक द्विगुण* src2, पूर्णांक n) स्थिरांक {
        पूर्णांक j = 0;
        द्विगुण s = 0.f;
        v_float64 r0 = vx_setzero_f64(), r1 = vx_setzero_f64();
        साठी (; j <= n - 2 * VTraits<v_float64>::vlanes(); j += 2 * VTraits<v_float64>::vlanes()) {
            v_float64 v01 = vx_load(src1 + j), v02 = vx_load(src2 + j);
            r0 = v_add(r0, v_absdiff(v01, v02));

            v_float64 v11 = vx_load(src1 + j + VTraits<v_float64>::vlanes()),
                      v12 = vx_load(src2 + j + VTraits<v_float64>::vlanes());
            r1 = v_add(r1, v_absdiff(v11, v12));
        }
        s += v_reduce_sum(v_add(r0, r1));
        साठी (; j < n; j++) {
            द्विगुण v = (द्विगुण)cv_absdiff(src1[j], src2[j]);
            s += v;
        }
        परत s;
    }
};

ढाँचा<>
संरचना NormDiffL2_SIMD<ushort, द्विगुण> {
    द्विगुण चालक() (स्थिरांक ushort* src1, स्थिरांक ushort* src2, पूर्णांक n) स्थिरांक {
        पूर्णांक j = 0;
        द्विगुण s = 0.f;
        v_float64 r0 = vx_setzero_f64(), r1 = vx_setzero_f64();
        साठी (; j <= n - 2 * VTraits<v_uint16>::vlanes(); j += 2 * VTraits<v_uint16>::vlanes()) {
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
        साठी (; j < n; j++) {
            द्विगुण v = (द्विगुण)src1[j] - (द्विगुण)src2[j];
            s += v * v;
        }
        परत s;
    }
};

ढाँचा<>
संरचना NormDiffL2_SIMD<लघु, द्विगुण> {
    द्विगुण चालक() (स्थिरांक लघु* src1, स्थिरांक लघु* src2, पूर्णांक n) स्थिरांक {
        पूर्णांक j = 0;
        द्विगुण s = 0.f;
        v_float64 r0 = vx_setzero_f64(), r1 = vx_setzero_f64();
        साठी (; j <= n - 2 * VTraits<v_int16>::vlanes(); j += 2 * VTraits<v_int16>::vlanes()) {
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
        साठी (; j < n; j++) {
            द्विगुण v = (द्विगुण)src1[j] - (द्विगुण)src2[j];
            s += v * v;
        }
        परत s;
    }
};

ढाँचा<>
संरचना NormDiffL2_SIMD<पूर्णांक, द्विगुण> {
    द्विगुण चालक() (स्थिरांक पूर्णांक* src1, स्थिरांक पूर्णांक* src2, पूर्णांक n) स्थिरांक {
        पूर्णांक j = 0;
        द्विगुण s = 0.f;
        v_float64 r0 = vx_setzero_f64(), r1 = vx_setzero_f64();
        साठी (; j <= n - VTraits<v_int32>::vlanes(); j += VTraits<v_int32>::vlanes()) {
            v_int32 v01 = vx_load(src1 + j), v02 = vx_load(src2 + j);
            v_uint32 v0 = v_absdiff(v01, v02);
            v_uint64 ev0, ev1;
            v_expand(v0, ev0, ev1);
            v_float64 f0 = v_cvt_f64(v_reinterpret_as_s64(ev0)), f1 = v_cvt_f64(v_reinterpret_as_s64(ev1));
            r0 = v_fma(f0, f0, r0); r1 = v_fma(f1, f1, r1);
        }
        s += v_reduce_sum(v_add(r0, r1));
        साठी (; j < n; j++) {
            द्विगुण v = (द्विगुण)src1[j] - (द्विगुण)src2[j];
            s += v * v;
        }
        परत s;
    }
};

ढाँचा<>
संरचना NormDiffL2_SIMD<दशांश, द्विगुण> {
    द्विगुण चालक() (स्थिरांक दशांश* src1, स्थिरांक दशांश* src2, पूर्णांक n) स्थिरांक {
        पूर्णांक j = 0;
        द्विगुण s = 0.f;
        v_float64 r0 = vx_setzero_f64(), r1 = vx_setzero_f64();
        v_float64 r2 = vx_setzero_f64(), r3 = vx_setzero_f64();
        साठी (; j <= n - 2 * VTraits<v_float32>::vlanes(); j += 2 * VTraits<v_float32>::vlanes()) {
            v_float32 v01 = vx_load(src1 + j), v02 = vx_load(src2 + j);
            v_float32 v0 = v_absdiff(v01, v02);
            v_float64 f01 = v_cvt_f64(v0), f02 = v_cvt_f64_वरचा(v0);
            r0 = v_fma(f01, f01, r0); r1 = v_fma(f02, f02, r1);

            v_float32 v11 = vx_load(src1 + j + VTraits<v_float32>::vlanes()),
                      v12 = vx_load(src2 + j + VTraits<v_float32>::vlanes());
            v_float32 v1 = v_absdiff(v11, v12);
            v_float64 f11 = v_cvt_f64(v1), f12 = v_cvt_f64_वरचा(v1);
            r2 = v_fma(f11, f11, r2); r3 = v_fma(f12, f12, r3);
        }
        s += v_reduce_sum(v_add(v_add(v_add(r0, r1), r2), r3));
        साठी (; j < n; j++) {
            द्विगुण v = (द्विगुण)src1[j] - (द्विगुण)src2[j];
            s += v * v;
        }
        परत s;
    }
};

ढाँचा<>
संरचना NormDiffL2_SIMD<द्विगुण, द्विगुण> {
    द्विगुण चालक() (स्थिरांक द्विगुण* src1, स्थिरांक द्विगुण* src2, पूर्णांक n) स्थिरांक {
        पूर्णांक j = 0;
        द्विगुण s = 0.f;
        v_float64 r0 = vx_setzero_f64(), r1 = vx_setzero_f64();
        v_float64 r2 = vx_setzero_f64(), r3 = vx_setzero_f64();
        साठी (; j <= n - 4 * VTraits<v_float64>::vlanes(); j += 4 * VTraits<v_float64>::vlanes()) {
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
        साठी (; j < n; j++) {
            द्विगुण v = (द्विगुण)src1[j] - (द्विगुण)src2[j];
            s += v * v;
        }
        परत s;
    }
};

#endif

ढाँचा <typename T, typename ST>
संरचना MaskedNormInf_SIMD {
    ओळीत ST चालक() (स्थिरांक T* src, स्थिरांक uchar* mask, पूर्णांक len, पूर्णांक cn) स्थिरांक {
        ST s = 0;
        जर (cn == 1) {
            साठी (पूर्णांक i = 0; i < len; i++) {
                जर (mask[i]) {
                    s = std::max(s, (ST)cv_abs(src[i]));
                }
            }
        }
        अन्यथा {
            साठी (पूर्णांक i = 0; i < len; i++) {
                जर (mask[i]) {
                    स्थिरांक T* elem = src + i * cn;
                    पूर्णांक k = 0;
                #जर CV_ENABLE_UNROLLED && !CV_SIMD_SCALABLE
                    साठी (; k <= cn - 4; k += 4) {
                        s = std::max(s, (ST)cv_abs(elem[k]));
                        s = std::max(s, (ST)cv_abs(elem[k + 1]));
                        s = std::max(s, (ST)cv_abs(elem[k + 2]));
                        s = std::max(s, (ST)cv_abs(elem[k + 3]));
                    }
                #endif
                    साठी (; k < cn; k++) {
                        s = std::max(s, (ST)cv_abs(elem[k]));
                    }
                }
            }
        }
        परत s;
    }
};

ढाँचा <typename T, typename ST>
संरचना MaskedNormL1_SIMD {
    ओळीत ST चालक() (स्थिरांक T* src, स्थिरांक uchar* mask, पूर्णांक len, पूर्णांक cn) स्थिरांक {
        ST s = 0;
        जर (cn == 1) {
            साठी (पूर्णांक i = 0; i < len; i++) {
                जर (mask[i]) {
                    s += (ST)cv_abs(src[i]);
                }
            }
        }
        अन्यथा {
            साठी (पूर्णांक i = 0; i < len; i++) {
                जर (mask[i]) {
                    स्थिरांक T* elem = src + i * cn;
                    पूर्णांक k = 0;
                #जर CV_ENABLE_UNROLLED && !CV_SIMD_SCALABLE
                    साठी (; k <= cn - 4; k += 4) {
                        s += (ST)cv_abs(elem[k]);
                        s += (ST)cv_abs(elem[k + 1]);
                        s += (ST)cv_abs(elem[k + 2]);
                        s += (ST)cv_abs(elem[k + 3]);
                    }
                #endif
                    साठी (; k < cn; k++) {
                        s += (ST)cv_abs(elem[k]);
                    }
                }
            }
        }
        परत s;
    }
};

ढाँचा <typename T, typename ST>
संरचना MaskedNormL2_SIMD {
    ओळीत ST चालक() (स्थिरांक T* src, स्थिरांक uchar* mask, पूर्णांक len, पूर्णांक cn) स्थिरांक {
        ST s = 0;
        जर (cn == 1) {
            पूर्णांक i = 0;
        #जर CV_ENABLE_UNROLLED && !CV_SIMD_SCALABLE
            साठी (; i <= len - 4; i += 4) {
                जर (mask[i])     { T v0 = src[i];     s += (ST)v0 * v0; }
                जर (mask[i + 1]) { T v1 = src[i + 1]; s += (ST)v1 * v1; }
                जर (mask[i + 2]) { T v2 = src[i + 2]; s += (ST)v2 * v2; }
                जर (mask[i + 3]) { T v3 = src[i + 3]; s += (ST)v3 * v3; }
            }
        #endif
            साठी (; i < len; i++) {
                जर (mask[i]) {
                    T v = src[i];
                    s += (ST)v * v;
                }
            }
        }
        अन्यथा {
            साठी (पूर्णांक i = 0; i < len; i++) {
                जर (mask[i]) {
                    स्थिरांक T* elem = src + i * cn;
                    पूर्णांक k = 0;
                #जर CV_ENABLE_UNROLLED && !CV_SIMD_SCALABLE
                    साठी (; k <= cn - 4; k += 4) {
                        T v0 = elem[k];     s += (ST)v0 * v0;
                        T v1 = elem[k + 1]; s += (ST)v1 * v1;
                        T v2 = elem[k + 2]; s += (ST)v2 * v2;
                        T v3 = elem[k + 3]; s += (ST)v3 * v3;
                    }
                #endif
                    साठी (; k < cn; k++) {
                        T v = elem[k];
                        s += (ST)v * v;
                    }
                }
            }
        }

        परत s;
    }
};

#जर (CV_SIMD || CV_SIMD_SCALABLE)
स्थिर ओळीत v_uint8  v_normmask_u8 (स्थिरांक uchar* m) { परत v_gt(vx_load(m),          vx_setzero_u8());  }
स्थिर ओळीत v_uint16 v_normmask_u16(स्थिरांक uchar* m) { परत v_gt(vx_load_expand(m),   vx_setzero_u16()); }
स्थिर ओळीत v_uint32 v_normmask_u32(स्थिरांक uchar* m) { परत v_gt(vx_load_expand_q(m), vx_setzero_u32()); }
स्थिर ओळीत शून्य v_normmask_u64(स्थिरांक uchar* m, v_uint64& m0, v_uint64& m1) {
    v_uint64 e0, e1;
    v_expand(v_normmask_u32(m), e0, e1);
    m0 = v_or(e0, v_shl<32>(e0));
    m1 = v_or(e1, v_shl<32>(e1));
}

ढाँचा <>
संरचना MaskedNormInf_SIMD<दशांश, दशांश> {
    ओळीत दशांश चालक()(स्थिरांक दशांश* src, स्थिरांक uchar* mask, पूर्णांक len, पूर्णांक cn) स्थिरांक {
        दशांश result = 0.0f;
        जर (cn == 1) {
            पूर्णांक i = 0;
            स्थिरांक पूर्णांक vstep = VTraits<v_float32>::vlanes();
            v_float32 acc = vx_setzero_f32();

            साठी (; i <= len - vstep; i += vstep) {
                v_uint32 m = vx_load_expand_q(mask + i);
                v_uint32 cmp = v_gt(m, vx_setzero_u32());
                v_float32 s = vx_load(src + i);
                s = v_abs(s);
                s = v_reinterpret_as_f32(v_and(v_reinterpret_as_u32(s), cmp));
                acc = v_max(acc, s);
            }
            result = v_reduce_max(acc);

            साठी (; i < len; i++) {
                जर (mask[i])
                    result = std::max(result, std::असल(src[i]));
            }
        }
        अन्यथा {
            साठी (पूर्णांक i = 0; i < len; i++) {
                जर (mask[i]) {
                    स्थिरांक दशांश* elem = src + i * cn;
                    पूर्णांक k = 0;
                    स्थिरांक पूर्णांक vstep = VTraits<v_float32>::vlanes();
                    v_float32 acc = vx_setzero_f32();

                    साठी (; k <= cn - vstep; k += vstep) {
                        v_float32 s = vx_load(elem + k);
                        acc = v_max(acc, v_abs(s));
                    }

                    result = std::max(result, v_reduce_max(acc));

                    साठी (; k < cn; k++)
                        result = std::max(result, std::असल(elem[k]));
                }
            }
        }
        परत result;
    }
};

#जर CV_SIMD_64F
ढाँचा <>
संरचना MaskedNormL1_SIMD<दशांश, द्विगुण> {
    ओळीत द्विगुण चालक()(स्थिरांक दशांश* src, स्थिरांक uchar* mask, पूर्णांक len, पूर्णांक cn) स्थिरांक {
        द्विगुण result = 0.0;
        जर (cn == 1) {
            पूर्णांक i = 0;
            स्थिरांक पूर्णांक vstep = VTraits<v_float32>::vlanes();
            v_float64 acc0 = vx_setzero_f64(), acc1 = vx_setzero_f64();
            v_float64 acc2 = vx_setzero_f64(), acc3 = vx_setzero_f64();

            साठी (; i <= len - 2*vstep; i += 2*vstep) {
                v_float32 s0 = v_and(v_abs(vx_load(src + i)),
                                     v_reinterpret_as_f32(v_normmask_u32(mask + i)));
                v_float32 s1 = v_and(v_abs(vx_load(src + i + vstep)),
                                     v_reinterpret_as_f32(v_normmask_u32(mask + i + vstep)));
                acc0 = v_add(acc0, v_cvt_f64(s0));
                acc1 = v_add(acc1, v_cvt_f64_वरचा(s0));
                acc2 = v_add(acc2, v_cvt_f64(s1));
                acc3 = v_add(acc3, v_cvt_f64_वरचा(s1));
            }
            साठी (; i <= len - vstep; i += vstep) {
                v_float32 s = v_and(v_abs(vx_load(src + i)),
                                    v_reinterpret_as_f32(v_normmask_u32(mask + i)));
                acc0 = v_add(acc0, v_cvt_f64(s));
                acc1 = v_add(acc1, v_cvt_f64_वरचा(s));
            }
            result = v_reduce_sum(v_add(v_add(acc0, acc1), v_add(acc2, acc3)));

            साठी (; i < len; i++) {
                जर (mask[i])
                    result += std::असल(src[i]);
            }
        }
        अन्यथा {
            साठी (पूर्णांक i = 0; i < len; i++) {
                जर (mask[i]) {
                    स्थिरांक दशांश* elem = src + i * cn;
                    पूर्णांक k = 0;
                    स्थिरांक पूर्णांक vstep = VTraits<v_float32>::vlanes();
                    v_float64 acc = vx_setzero_f64();

                    साठी (; k <= cn - vstep; k += vstep) {
                        v_float32 s = v_abs(vx_load(elem + k));
                        acc = v_add(acc, v_cvt_f64(s));
                        acc = v_add(acc, v_cvt_f64_वरचा(s));
                    }

                    result += v_reduce_sum(acc);

                    साठी (; k < cn; k++)
                        result += std::असल(elem[k]);
                }
            }
        }
        परत result;
    }
};

ढाँचा <>
संरचना MaskedNormL2_SIMD<दशांश, द्विगुण> {
    ओळीत द्विगुण चालक()(स्थिरांक दशांश* src, स्थिरांक uchar* mask, पूर्णांक len, पूर्णांक cn) स्थिरांक {
        द्विगुण result = 0.0;
        जर (cn == 1) {
            पूर्णांक i = 0;
            स्थिरांक पूर्णांक vstep = VTraits<v_float32>::vlanes();
            v_float32 facc = vx_setzero_f32();
            v_float64 dacc = vx_setzero_f64();
            पूर्णांक flush = 0;

            साठी (; i <= len - vstep; i += vstep, flush += vstep) {
                जर (flush >= 64) {
                    dacc = v_add(dacc, v_cvt_f64(facc));
                    dacc = v_add(dacc, v_cvt_f64_वरचा(facc));
                    facc = vx_setzero_f32();
                    flush = 0;
                }
                v_uint32 cmp = v_gt(vx_load_expand_q(mask + i), vx_setzero_u32());
                v_float32 s  = v_reinterpret_as_f32(v_and(v_reinterpret_as_u32(vx_load(src + i)), cmp));
                facc = v_add(facc, v_mul(s, s));
            }

            dacc = v_add(dacc, v_cvt_f64(facc));
            dacc = v_add(dacc, v_cvt_f64_वरचा(facc));
            result = v_reduce_sum(dacc);

            साठी (; i < len; i++) {
                जर (mask[i]) {
                    द्विगुण v = src[i];
                    result += v * v;
                }
            }
        }
        अन्यथा {
            साठी (पूर्णांक i = 0; i < len; i++) {
                जर (mask[i]) {
                    स्थिरांक दशांश* elem = src + i * cn;
                    पूर्णांक k = 0;
                    स्थिरांक पूर्णांक vstep = VTraits<v_float32>::vlanes();
                    v_float32 facc = vx_setzero_f32();

                    साठी (; k <= cn - vstep; k += vstep) {
                        v_float32 s = vx_load(elem + k);
                        facc = v_add(facc, v_mul(s, s));
                    }

                    v_float64 dacc = v_add(v_cvt_f64(facc), v_cvt_f64_वरचा(facc));
                    result += v_reduce_sum(dacc);

                    साठी (; k < cn; k++) {
                        द्विगुण v = elem[k];
                        result += v * v;
                    }
                }
            }
        }
        परत result;
    }
};

#endif

ढाँचा <>
संरचना MaskedNormInf_SIMD<uchar, पूर्णांक> {
    ओळीत पूर्णांक चालक()(स्थिरांक uchar* src, स्थिरांक uchar* mask, पूर्णांक len, पूर्णांक cn) स्थिरांक {
        पूर्णांक result = 0;
        जर (cn == 1) {
            पूर्णांक i = 0;
            स्थिरांक पूर्णांक vstep = VTraits<v_uint8>::vlanes();
            v_uint8 acc = vx_setzero_u8();

            साठी (; i <= len - vstep; i += vstep) {
                v_uint8 m   = vx_load(mask + i);
                v_uint8 s   = vx_load(src + i);
                v_uint8 sel = v_and(s, v_gt(m, vx_setzero_u8()));
                acc = v_max(acc, sel);
            }

            result = (पूर्णांक)v_reduce_max(acc);

            साठी (; i < len; i++) {
                जर (mask[i])
                    result = std::max(result, (पूर्णांक)src[i]);
            }
        }
        अन्यथा जर (cn == 4 && len >= VTraits<v_uint8>::vlanes()) {
            स्थिरांक पूर्णांक vstep = VTraits<v_uint8>::vlanes();
            v_uint8 acc = vx_setzero_u8();
            पूर्णांक i = 0;
            साठी (;;) {
                जर (i > len - vstep)
                    i = len - vstep; // back-step (max is idempotent)
                v_uint8 c0, c1, c2, c3;
                v_load_deinterleave(src + i * 4, c0, c1, c2, c3);
                v_uint8 pmax = v_max(v_max(c0, c1), v_max(c2, c3));
                v_uint8 m = v_gt(vx_load(mask + i), vx_setzero_u8());
                acc = v_max(acc, v_and(pmax, m));
                जर (i >= len - vstep)
                    थांबा;
                i += vstep;
            }
            result = (पूर्णांक)v_reduce_max(acc);
        }
        अन्यथा जर (cn == 4) {
            साठी (पूर्णांक i = 0; i < len; i++) {
                जर (mask[i]) {
                    स्थिरांक uchar* elem = src + i * 4;
                    result = std::max(result, (पूर्णांक)std::max(std::max(elem[0], elem[1]),
                                                            std::max(elem[2], elem[3])));
                }
            }
        }
        अन्यथा {
            साठी (पूर्णांक i = 0; i < len; i++) {
                जर (mask[i]) {
                    स्थिरांक uchar* elem = src + i * cn;
                    साठी (पूर्णांक k = 0; k < cn; k++)
                        result = std::max(result, (पूर्णांक)elem[k]);
                }
            }
        }
        परत result;
    }
};

ढाँचा <>
संरचना MaskedNormL1_SIMD<uchar, पूर्णांक> {
    ओळीत पूर्णांक चालक()(स्थिरांक uchar* src, स्थिरांक uchar* mask, पूर्णांक len, पूर्णांक cn) स्थिरांक {
        पूर्णांक result = 0;
        स्थिरांक पूर्णांक vstep = VTraits<v_uint8>::vlanes();
        स्थिरांक v_uint8 one = vx_setall_u8(1);
        जर (cn == 1) {
            पूर्णांक i = 0;
            v_uint32 acc = vx_setzero_u32();
            साठी (; i <= len - vstep; i += vstep) {
                v_uint8 s = v_and(vx_load(src + i), v_normmask_u8(mask + i));
                acc = v_dotprod_expand_fast(s, one, acc);
            }
            result = (पूर्णांक)v_reduce_sum(acc);
            साठी (; i < len; i++) {
                जर (mask[i])
                    result += src[i];
            }
        }
        अन्यथा जर (cn == 4) {
            पूर्णांक i = 0;
            v_uint32 acc = vx_setzero_u32();
            साठी (; i <= len - vstep; i += vstep) {
                v_uint8 c0, c1, c2, c3;
                v_load_deinterleave(src + i * 4, c0, c1, c2, c3);
                v_uint8 m = v_normmask_u8(mask + i);
                acc = v_dotprod_expand_fast(v_and(c0, m), one, acc);
                acc = v_dotprod_expand_fast(v_and(c1, m), one, acc);
                acc = v_dotprod_expand_fast(v_and(c2, m), one, acc);
                acc = v_dotprod_expand_fast(v_and(c3, m), one, acc);
            }
            result = (पूर्णांक)v_reduce_sum(acc);
            साठी (; i < len; i++) {
                जर (mask[i]) {
                    स्थिरांक uchar* elem = src + i * 4;
                    result += elem[0] + elem[1] + elem[2] + elem[3];
                }
            }
        }
        अन्यथा {
            साठी (पूर्णांक i = 0; i < len; i++) {
                जर (mask[i]) {
                    स्थिरांक uchar* elem = src + i * cn;
                    साठी (पूर्णांक k = 0; k < cn; k++)
                        result += elem[k];
                }
            }
        }
        परत result;
    }
};

ढाँचा <>
संरचना MaskedNormInf_SIMD<ushort, पूर्णांक> {
    ओळीत पूर्णांक चालक()(स्थिरांक ushort* src, स्थिरांक uchar* mask, पूर्णांक len, पूर्णांक cn) स्थिरांक {
        पूर्णांक result = 0;
        जर (cn == 1) {
            पूर्णांक i = 0;
            स्थिरांक पूर्णांक vstep = VTraits<v_uint16>::vlanes();
            v_uint16 acc = vx_setzero_u16();

            साठी (; i <= len - vstep; i += vstep) {
                v_uint16 m   = vx_load_expand(mask + i);
                v_uint16 cmp = v_gt(m, vx_setzero_u16());
                v_uint16 s   = vx_load(src + i);
                v_uint16 sel = v_and(s, cmp);
                acc = v_max(acc, sel);
            }

            result = (पूर्णांक)v_reduce_max(acc);

            साठी (; i < len; i++) {
                जर (mask[i])
                    result = std::max(result, (पूर्णांक)src[i]);
            }
        }
        अन्यथा {
            साठी (पूर्णांक i = 0; i < len; i++) {
                जर (mask[i]) {
                    स्थिरांक ushort* elem = src + i * cn;
                    पूर्णांक k = 0;
                    स्थिरांक पूर्णांक vstep = VTraits<v_uint16>::vlanes();
                    v_uint16 acc = vx_setzero_u16();

                    साठी (; k <= cn - vstep; k += vstep) {
                        acc = v_max(acc, vx_load(elem + k));
                    }

                    result = std::max(result, (पूर्णांक)v_reduce_max(acc));

                    साठी (; k < cn; k++)
                        result = std::max(result, (पूर्णांक)elem[k]);
                }
            }
        }
        परत result;
    }
};

ढाँचा <>
संरचना MaskedNormL1_SIMD<ushort, पूर्णांक> {
    ओळीत पूर्णांक चालक()(स्थिरांक ushort* src, स्थिरांक uchar* mask, पूर्णांक len, पूर्णांक cn) स्थिरांक {
        पूर्णांक result = 0;
        जर (cn == 1) {
            पूर्णांक i = 0;
            स्थिरांक पूर्णांक vstep = VTraits<v_uint16>::vlanes();
            v_uint32 acc32 = vx_setzero_u32();
            v_uint64 acc64 = vx_setzero_u64();
            पूर्णांक acc32_elems = 0;

            साठी (; i <= len - vstep; i += vstep, acc32_elems += vstep) {
                जर (acc32_elems >= 512) {
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
            result = (पूर्णांक)v_reduce_sum(acc64);

            साठी (; i < len; i++) {
                जर (mask[i])
                    result += src[i];
            }
        }
        अन्यथा {
            साठी (पूर्णांक i = 0; i < len; i++) {
                जर (mask[i]) {
                    स्थिरांक ushort* elem = src + i * cn;
                    पूर्णांक k = 0;
                    स्थिरांक पूर्णांक vstep = VTraits<v_uint16>::vlanes();
                    v_uint32 acc = vx_setzero_u32();

                    साठी (; k <= cn - vstep; k += vstep) {
                        v_uint32 lo32, hi32;
                        v_expand(vx_load(elem + k), lo32, hi32);
                        acc = v_add(acc, v_add(lo32, hi32));
                    }

                    result += (पूर्णांक)v_reduce_sum(acc);

                    साठी (; k < cn; k++)
                        result += elem[k];
                }
            }
        }
        परत result;
    }
};

ढाँचा <>
संरचना MaskedNormL2_SIMD<ushort, द्विगुण> {
    ओळीत द्विगुण चालक()(स्थिरांक ushort* src, स्थिरांक uchar* mask, पूर्णांक len, पूर्णांक cn) स्थिरांक {
        द्विगुण result = 0.0;
        जर (cn == 1) {
            पूर्णांक i = 0;
            स्थिरांक पूर्णांक vstep = VTraits<v_uint16>::vlanes();
            v_uint64 acc = vx_setzero_u64();
            साठी (; i <= len - vstep; i += vstep) {
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
            result = (द्विगुण)v_reduce_sum(acc);
            साठी (; i < len; i++) {
                जर (mask[i]) {
                    द्विगुण v = src[i];
                    result += v * v;
                }
            }
        }
        अन्यथा {
            साठी (पूर्णांक i = 0; i < len; i++) {
                जर (mask[i]) {
                    स्थिरांक ushort* elem = src + i * cn;
                    पूर्णांक k = 0;
                    स्थिरांक पूर्णांक vstep = VTraits<v_uint16>::vlanes();
                    v_uint64 acc = vx_setzero_u64();
                    साठी (; k <= cn - vstep; k += vstep) {
                        v_uint32 lo32, hi32;
                        v_expand(vx_load(elem + k), lo32, hi32);
                        v_uint64 lo64a, lo64b, hi64a, hi64b;
                        v_expand(v_mul(lo32, lo32), lo64a, lo64b);
                        v_expand(v_mul(hi32, hi32), hi64a, hi64b);
                        acc = v_add(acc, v_add(v_add(lo64a, lo64b), v_add(hi64a, hi64b)));
                    }

                    result += (द्विगुण)v_reduce_sum(acc);

                    साठी (; k < cn; k++) {
                        द्विगुण v = elem[k];
                        result += v * v;
                    }
                }
            }
        }
        परत result;
    }
};

ढाँचा <>
संरचना MaskedNormInf_SIMD<schar, पूर्णांक> {
    ओळीत पूर्णांक चालक()(स्थिरांक schar* src, स्थिरांक uchar* mask, पूर्णांक len, पूर्णांक cn) स्थिरांक {
        पूर्णांक result = 0;
        जर (cn == 1) {
            स्थिरांक पूर्णांक vstep = VTraits<v_int8>::vlanes();
            पूर्णांक i = 0;
            v_uint8 acc = vx_setzero_u8();
            साठी (; i <= len - vstep; i += vstep)
                acc = v_max(acc, v_and(v_abs(vx_load(src + i)), v_normmask_u8(mask + i)));
            result = (पूर्णांक)v_reduce_max(acc);
            साठी (; i < len; i++)
                जर (mask[i]) result = std::max(result, cv_abs(src[i]));
        }
        अन्यथा {
            साठी (पूर्णांक i = 0; i < len; i++)
                जर (mask[i]) {
                    स्थिरांक schar* elem = src + i * cn;
                    साठी (पूर्णांक k = 0; k < cn; k++)
                        result = std::max(result, cv_abs(elem[k]));
                }
        }
        परत result;
    }
};

ढाँचा <>
संरचना MaskedNormL1_SIMD<schar, पूर्णांक> {
    ओळीत पूर्णांक चालक()(स्थिरांक schar* src, स्थिरांक uchar* mask, पूर्णांक len, पूर्णांक cn) स्थिरांक {
        पूर्णांक result = 0;
        जर (cn == 1) {
            स्थिरांक पूर्णांक vstep = VTraits<v_int8>::vlanes();
            स्थिरांक v_uint8 one = vx_setall_u8(1);
            पूर्णांक i = 0;
            v_uint32 acc = vx_setzero_u32();
            साठी (; i <= len - vstep; i += vstep)
                acc = v_dotprod_expand_fast(v_and(v_abs(vx_load(src + i)), v_normmask_u8(mask + i)), one, acc);
            result = (पूर्णांक)v_reduce_sum(acc);
            साठी (; i < len; i++)
                जर (mask[i]) result += cv_abs(src[i]);
        }
        अन्यथा {
            साठी (पूर्णांक i = 0; i < len; i++)
                जर (mask[i]) {
                    स्थिरांक schar* elem = src + i * cn;
                    साठी (पूर्णांक k = 0; k < cn; k++)
                        result += cv_abs(elem[k]);
                }
        }
        परत result;
    }
};

ढाँचा <>
संरचना MaskedNormL2_SIMD<schar, पूर्णांक> {
    ओळीत पूर्णांक चालक()(स्थिरांक schar* src, स्थिरांक uchar* mask, पूर्णांक len, पूर्णांक cn) स्थिरांक {
        पूर्णांक result = 0;
        जर (cn == 1) {
            स्थिरांक पूर्णांक vstep = VTraits<v_int8>::vlanes();
            पूर्णांक i = 0;
            v_int32 acc = vx_setzero_s32();
            साठी (; i <= len - vstep; i += vstep) {
                v_int8 s = v_and(vx_load(src + i), v_reinterpret_as_s8(v_normmask_u8(mask + i)));
                acc = v_dotprod_expand_fast(s, s, acc);
            }
            result = (पूर्णांक)v_reduce_sum(acc);
            साठी (; i < len; i++)
                जर (mask[i]) { पूर्णांक v = src[i]; result += v * v; }
        }
        अन्यथा {
            साठी (पूर्णांक i = 0; i < len; i++)
                जर (mask[i]) {
                    स्थिरांक schar* elem = src + i * cn;
                    साठी (पूर्णांक k = 0; k < cn; k++) { पूर्णांक v = elem[k]; result += v * v; }
                }
        }
        परत result;
    }
};

ढाँचा <>
संरचना MaskedNormL2_SIMD<uchar, पूर्णांक> {
    ओळीत पूर्णांक चालक()(स्थिरांक uchar* src, स्थिरांक uchar* mask, पूर्णांक len, पूर्णांक cn) स्थिरांक {
        पूर्णांक result = 0;
        स्थिरांक पूर्णांक vstep = VTraits<v_uint8>::vlanes();
        जर (cn == 1) {
            पूर्णांक i = 0;
            v_uint32 acc = vx_setzero_u32();
            साठी (; i <= len - vstep; i += vstep) {
                v_uint8 s = v_and(vx_load(src + i), v_normmask_u8(mask + i));
                acc = v_dotprod_expand_fast(s, s, acc);
            }
            result = (पूर्णांक)v_reduce_sum(acc);
            साठी (; i < len; i++)
                जर (mask[i]) { पूर्णांक v = src[i]; result += v * v; }
        }
        अन्यथा जर (cn == 4) {
            पूर्णांक i = 0;
            v_uint32 acc = vx_setzero_u32();
            साठी (; i <= len - vstep; i += vstep) {
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
            result = (पूर्णांक)v_reduce_sum(acc);
            साठी (; i < len; i++)
                जर (mask[i]) {
                    स्थिरांक uchar* elem = src + i * 4;
                    साठी (पूर्णांक k = 0; k < 4; k++) { पूर्णांक v = elem[k]; result += v * v; }
                }
        }
        अन्यथा {
            साठी (पूर्णांक i = 0; i < len; i++)
                जर (mask[i]) {
                    स्थिरांक uchar* elem = src + i * cn;
                    साठी (पूर्णांक k = 0; k < cn; k++) { पूर्णांक v = elem[k]; result += v * v; }
                }
        }
        परत result;
    }
};

ढाँचा <>
संरचना MaskedNormInf_SIMD<लघु, पूर्णांक> {
    ओळीत पूर्णांक चालक()(स्थिरांक लघु* src, स्थिरांक uchar* mask, पूर्णांक len, पूर्णांक cn) स्थिरांक {
        पूर्णांक result = 0;
        जर (cn == 1) {
            स्थिरांक पूर्णांक vstep = VTraits<v_int16>::vlanes();
            पूर्णांक i = 0;
            v_uint16 acc = vx_setzero_u16();
            साठी (; i <= len - vstep; i += vstep)
                acc = v_max(acc, v_and(v_abs(vx_load(src + i)), v_normmask_u16(mask + i)));
            result = (पूर्णांक)v_reduce_max(acc);
            साठी (; i < len; i++)
                जर (mask[i]) result = std::max(result, cv_abs(src[i]));
        }
        अन्यथा {
            साठी (पूर्णांक i = 0; i < len; i++)
                जर (mask[i]) {
                    स्थिरांक लघु* elem = src + i * cn;
                    साठी (पूर्णांक k = 0; k < cn; k++)
                        result = std::max(result, cv_abs(elem[k]));
                }
        }
        परत result;
    }
};

ढाँचा <>
संरचना MaskedNormL1_SIMD<लघु, पूर्णांक> {
    ओळीत पूर्णांक चालक()(स्थिरांक लघु* src, स्थिरांक uchar* mask, पूर्णांक len, पूर्णांक cn) स्थिरांक {
        पूर्णांक result = 0;
        जर (cn == 1) {
            स्थिरांक पूर्णांक vstep = VTraits<v_int16>::vlanes();
            पूर्णांक i = 0;
            v_uint32 acc0 = vx_setzero_u32(), acc1 = vx_setzero_u32();
            साठी (; i <= len - vstep; i += vstep) {
                v_uint16 s = v_and(v_abs(vx_load(src + i)), v_normmask_u16(mask + i));
                v_uint32 lo, hi;
                v_expand(s, lo, hi);
                acc0 = v_add(acc0, lo);
                acc1 = v_add(acc1, hi);
            }
            result = (पूर्णांक)v_reduce_sum(v_add(acc0, acc1));
            साठी (; i < len; i++)
                जर (mask[i]) result += cv_abs(src[i]);
        }
        अन्यथा {
            साठी (पूर्णांक i = 0; i < len; i++)
                जर (mask[i]) {
                    स्थिरांक लघु* elem = src + i * cn;
                    साठी (पूर्णांक k = 0; k < cn; k++)
                        result += cv_abs(elem[k]);
                }
        }
        परत result;
    }
};

ढाँचा <>
संरचना MaskedNormL2_SIMD<लघु, द्विगुण> {
    ओळीत द्विगुण चालक()(स्थिरांक लघु* src, स्थिरांक uchar* mask, पूर्णांक len, पूर्णांक cn) स्थिरांक {
        द्विगुण result = 0.0;
        जर (cn == 1) {
            स्थिरांक पूर्णांक vstep = VTraits<v_int16>::vlanes();
            पूर्णांक i = 0;
            v_uint64 acc = vx_setzero_u64();
            साठी (; i <= len - vstep; i += vstep) {
                v_uint16 s = v_and(v_abs(vx_load(src + i)), v_normmask_u16(mask + i));
                acc = v_add(acc, v_dotprod_expand_fast(s, s));
            }
            result = (द्विगुण)v_reduce_sum(acc);
            साठी (; i < len; i++)
                जर (mask[i]) { द्विगुण v = src[i]; result += v * v; }
        }
        अन्यथा {
            साठी (पूर्णांक i = 0; i < len; i++)
                जर (mask[i]) {
                    स्थिरांक लघु* elem = src + i * cn;
                    साठी (पूर्णांक k = 0; k < cn; k++) { द्विगुण v = elem[k]; result += v * v; }
                }
        }
        परत result;
    }
};

ढाँचा <>
संरचना MaskedNormInf_SIMD<पूर्णांक, अचिन्हित> {
    ओळीत अचिन्हित चालक()(स्थिरांक पूर्णांक* src, स्थिरांक uchar* mask, पूर्णांक len, पूर्णांक cn) स्थिरांक {
        अचिन्हित result = 0;
        जर (cn == 1) {
            स्थिरांक पूर्णांक vstep = VTraits<v_int32>::vlanes();
            पूर्णांक i = 0;
            v_uint32 acc = vx_setzero_u32();
            साठी (; i <= len - vstep; i += vstep)
                acc = v_max(acc, v_and(v_abs(vx_load(src + i)), v_normmask_u32(mask + i)));
            result = v_reduce_max(acc);
            साठी (; i < len; i++)
                जर (mask[i]) result = std::max(result, cv_abs(src[i]));
        }
        अन्यथा {
            साठी (पूर्णांक i = 0; i < len; i++)
                जर (mask[i]) {
                    स्थिरांक पूर्णांक* elem = src + i * cn;
                    साठी (पूर्णांक k = 0; k < cn; k++)
                        result = std::max(result, cv_abs(elem[k]));
                }
        }
        परत result;
    }
};

#जर CV_SIMD_64F || CV_SIMD_SCALABLE_64F

ढाँचा <>
संरचना MaskedNormL1_SIMD<पूर्णांक, द्विगुण> {
    ओळीत द्विगुण चालक()(स्थिरांक पूर्णांक* src, स्थिरांक uchar* mask, पूर्णांक len, पूर्णांक cn) स्थिरांक {
        द्विगुण result = 0.0;
        जर (cn == 1) {
            स्थिरांक पूर्णांक vstep = VTraits<v_int32>::vlanes();
            पूर्णांक i = 0;
            v_float64 acc0 = vx_setzero_f64(), acc1 = vx_setzero_f64();
            साठी (; i <= len - vstep; i += vstep) {
                // Mask the value, then widen and take the magnitude in double. v_abs() does give
                // |INT_MIN| as an unsigned 2147483648, but reinterpreting that back as int32 turns
                // it negative again, and the widening then carries the sign into the accumulator.
                v_int32 s = v_reinterpret_as_s32(v_and(v_reinterpret_as_u32(vx_load(src + i)),
                                                       v_normmask_u32(mask + i)));
                acc0 = v_add(acc0, v_abs(v_cvt_f64(s)));
                acc1 = v_add(acc1, v_abs(v_cvt_f64_वरचा(s)));
            }
            result = v_reduce_sum(v_add(acc0, acc1));
            साठी (; i < len; i++)
                जर (mask[i]) result += cv_abs(src[i]);
        }
        अन्यथा {
            साठी (पूर्णांक i = 0; i < len; i++)
                जर (mask[i]) {
                    स्थिरांक पूर्णांक* elem = src + i * cn;
                    साठी (पूर्णांक k = 0; k < cn; k++)
                        result += cv_abs(elem[k]);
                }
        }
        परत result;
    }
};

ढाँचा <>
संरचना MaskedNormL2_SIMD<पूर्णांक, द्विगुण> {
    ओळीत द्विगुण चालक()(स्थिरांक पूर्णांक* src, स्थिरांक uchar* mask, पूर्णांक len, पूर्णांक cn) स्थिरांक {
        द्विगुण result = 0.0;
        जर (cn == 1) {
            स्थिरांक पूर्णांक vstep = VTraits<v_int32>::vlanes();
            पूर्णांक i = 0;
            v_float64 acc0 = vx_setzero_f64(), acc1 = vx_setzero_f64();
            साठी (; i <= len - vstep; i += vstep) {
                v_uint32 s = v_and(v_abs(vx_load(src + i)), v_normmask_u32(mask + i));
                v_uint64 e0, e1;
                v_expand(s, e0, e1);
                v_float64 f0 = v_cvt_f64(v_reinterpret_as_s64(e0));
                v_float64 f1 = v_cvt_f64(v_reinterpret_as_s64(e1));
                acc0 = v_fma(f0, f0, acc0);
                acc1 = v_fma(f1, f1, acc1);
            }
            result = v_reduce_sum(v_add(acc0, acc1));
            साठी (; i < len; i++)
                जर (mask[i]) { द्विगुण v = src[i]; result += v * v; }
        }
        अन्यथा {
            साठी (पूर्णांक i = 0; i < len; i++)
                जर (mask[i]) {
                    स्थिरांक पूर्णांक* elem = src + i * cn;
                    साठी (पूर्णांक k = 0; k < cn; k++) { द्विगुण v = elem[k]; result += v * v; }
                }
        }
        परत result;
    }
};

ढाँचा <>
संरचना MaskedNormInf_SIMD<द्विगुण, द्विगुण> {
    ओळीत द्विगुण चालक()(स्थिरांक द्विगुण* src, स्थिरांक uchar* mask, पूर्णांक len, पूर्णांक cn) स्थिरांक {
        द्विगुण result = 0.0;
        जर (cn == 1) {
            स्थिरांक पूर्णांक vstep = VTraits<v_float64>::vlanes();
            पूर्णांक i = 0;
            v_float64 acc0 = vx_setzero_f64(), acc1 = vx_setzero_f64();
            साठी (; i <= len - 2*vstep; i += 2*vstep) {
                v_uint64 m0, m1;
                v_normmask_u64(mask + i, m0, m1);
                acc0 = v_max(acc0, v_reinterpret_as_f64(v_and(v_reinterpret_as_u64(v_abs(vx_load(src + i))), m0)));
                acc1 = v_max(acc1, v_reinterpret_as_f64(v_and(v_reinterpret_as_u64(v_abs(vx_load(src + i + vstep))), m1)));
            }
            द्विगुण t[VTraits<v_float64>::max_nlanes];
            vx_store(t, v_max(acc0, acc1));
            साठी (पूर्णांक k = 0; k < vstep; k++)
                result = std::max(result, t[k]);
            साठी (; i < len; i++)
                जर (mask[i]) result = std::max(result, cv_abs(src[i]));
        }
        अन्यथा {
            साठी (पूर्णांक i = 0; i < len; i++)
                जर (mask[i]) {
                    स्थिरांक द्विगुण* elem = src + i * cn;
                    साठी (पूर्णांक k = 0; k < cn; k++)
                        result = std::max(result, cv_abs(elem[k]));
                }
        }
        परत result;
    }
};

ढाँचा <>
संरचना MaskedNormL1_SIMD<द्विगुण, द्विगुण> {
    ओळीत द्विगुण चालक()(स्थिरांक द्विगुण* src, स्थिरांक uchar* mask, पूर्णांक len, पूर्णांक cn) स्थिरांक {
        द्विगुण result = 0.0;
        जर (cn == 1) {
            स्थिरांक पूर्णांक vstep = VTraits<v_float64>::vlanes();
            पूर्णांक i = 0;
            v_float64 acc0 = vx_setzero_f64(), acc1 = vx_setzero_f64();
            साठी (; i <= len - 2*vstep; i += 2*vstep) {
                v_uint64 m0, m1;
                v_normmask_u64(mask + i, m0, m1);
                acc0 = v_add(acc0, v_reinterpret_as_f64(v_and(v_reinterpret_as_u64(v_abs(vx_load(src + i))), m0)));
                acc1 = v_add(acc1, v_reinterpret_as_f64(v_and(v_reinterpret_as_u64(v_abs(vx_load(src + i + vstep))), m1)));
            }
            result = v_reduce_sum(v_add(acc0, acc1));
            साठी (; i < len; i++)
                जर (mask[i]) result += cv_abs(src[i]);
        }
        अन्यथा {
            साठी (पूर्णांक i = 0; i < len; i++)
                जर (mask[i]) {
                    स्थिरांक द्विगुण* elem = src + i * cn;
                    साठी (पूर्णांक k = 0; k < cn; k++)
                        result += cv_abs(elem[k]);
                }
        }
        परत result;
    }
};

ढाँचा <>
संरचना MaskedNormL2_SIMD<द्विगुण, द्विगुण> {
    ओळीत द्विगुण चालक()(स्थिरांक द्विगुण* src, स्थिरांक uchar* mask, पूर्णांक len, पूर्णांक cn) स्थिरांक {
        द्विगुण result = 0.0;
        जर (cn == 1) {
            स्थिरांक पूर्णांक vstep = VTraits<v_float64>::vlanes();
            पूर्णांक i = 0;
            v_float64 acc0 = vx_setzero_f64(), acc1 = vx_setzero_f64();
            साठी (; i <= len - 2*vstep; i += 2*vstep) {
                v_uint64 m0, m1;
                v_normmask_u64(mask + i, m0, m1);
                v_float64 s0 = v_reinterpret_as_f64(v_and(v_reinterpret_as_u64(vx_load(src + i)), m0));
                v_float64 s1 = v_reinterpret_as_f64(v_and(v_reinterpret_as_u64(vx_load(src + i + vstep)), m1));
                acc0 = v_fma(s0, s0, acc0);
                acc1 = v_fma(s1, s1, acc1);
            }
            result = v_reduce_sum(v_add(acc0, acc1));
            साठी (; i < len; i++)
                जर (mask[i]) { द्विगुण v = src[i]; result += v * v; }
        }
        अन्यथा {
            साठी (पूर्णांक i = 0; i < len; i++)
                जर (mask[i]) {
                    स्थिरांक द्विगुण* elem = src + i * cn;
                    साठी (पूर्णांक k = 0; k < cn; k++) { द्विगुण v = elem[k]; result += v * v; }
                }
        }
        परत result;
    }
};

#endif // CV_SIMD_64F || CV_SIMD_SCALABLE_64F
#endif // CV_SIMD || CV_SIMD_SCALABLE

ढाँचा<typename T, typename ST> पूर्णांक
normInf_(स्थिरांक T* src, स्थिरांक uchar* mask, ST* _result, पूर्णांक len, पूर्णांक cn)
{
    ST result = *_result;
    जर( !mask )
    {
        NormInf_SIMD<T, ST> op;
        result = std::max(result, op(src, len*cn));
    } अन्यथा {
        MaskedNormInf_SIMD<T, ST> op;
        result = std::max(result, op(src, mask, len, cn));
    }
    *_result = result;
    परत 0;
}

ढाँचा<typename T, typename ST> पूर्णांक
normL1_(स्थिरांक T* src, स्थिरांक uchar* mask, ST* _result, पूर्णांक len, पूर्णांक cn)
{
    ST result = *_result;
    जर( !mask )
    {
        NormL1_SIMD<T, ST> op;
        result += op(src, len*cn);
    } अन्यथा {
        MaskedNormL1_SIMD<T, ST> op;
        result += op(src, mask, len, cn);
    }
    *_result = result;
    परत 0;
}

ढाँचा<typename T, typename ST> पूर्णांक
normL2_(स्थिरांक T* src, स्थिरांक uchar* mask, ST* _result, पूर्णांक len, पूर्णांक cn)
{
    ST result = *_result;
    जर( !mask )
    {
        NormL2_SIMD<T, ST> op;
        result += op(src, len*cn);
    }
    अन्यथा
    {
        MaskedNormL2_SIMD<T, ST> op;
        result += op(src, mask, len, cn);
    }
    *_result = result;
    परत 0;
}

स्थिर पूर्णांक
normInf_Bool(स्थिरांक uchar* src, स्थिरांक uchar* mask, पूर्णांक* _result, पूर्णांक len, पूर्णांक cn)
{
    पूर्णांक result = *_result;
    जर( !mask )
    {
        साठी ( पूर्णांक i = 0; i < len*cn; i++ ) {
            result = std::max(result, (पूर्णांक)(src[i] != 0));
            जर (result != 0)
                थांबा;
        }
    }
    अन्यथा
    {
        साठी( पूर्णांक i = 0; i < len; i++, src += cn )
            जर( mask[i] )
            {
                साठी( पूर्णांक k = 0; k < cn; k++ )
                    result = std::max(result, (पूर्णांक)(src[k] != 0));
                जर (result != 0)
                    थांबा;
            }
    }
    *_result = result;
    परत 0;
}

स्थिर पूर्णांक
normL1_Bool(स्थिरांक uchar* src, स्थिरांक uchar* mask, पूर्णांक* _result, पूर्णांक len, पूर्णांक cn)
{
    पूर्णांक result = *_result;
    जर( !mask )
    {
        साठी ( पूर्णांक i = 0; i < len*cn; i++ )
            result += (पूर्णांक)(src[i] != 0);
    }
    अन्यथा
    {
        साठी( पूर्णांक i = 0; i < len; i++, src += cn )
            जर( mask[i] )
            {
                साठी( पूर्णांक k = 0; k < cn; k++ )
                    result += (पूर्णांक)(src[k] != 0);
            }
    }
    *_result = result;
    परत 0;
}

स्थिर पूर्णांक
normL2_Bool(स्थिरांक uchar* src, स्थिरांक uchar* mask, पूर्णांक* _result, पूर्णांक len, पूर्णांक cn)
{
    परत normL1_Bool(src, mask, _result, len, cn);
}

// ====================================================================
// Masked norm-diff kernels.
// Scalar base templates (safe for every depth) plus SIMD cn==1
// specializations for the concrete (T, ST) pairs dispatched by
// CV_DEF_NORM_ALL. The masked normDiff*_ functions below select them.
// ====================================================================
ढाँचा <typename T, typename ST>
संरचना MaskedNormDiffInf_SIMD {
    ओळीत ST चालक()(स्थिरांक T* s1, स्थिरांक T* s2, स्थिरांक uchar* mask, पूर्णांक len, पूर्णांक cn) स्थिरांक {
        ST result = 0;
        साठी (पूर्णांक i = 0; i < len; i++) जर (mask[i]) {
            स्थिरांक T* e1 = s1 + i*cn; स्थिरांक T* e2 = s2 + i*cn;
            साठी (पूर्णांक k = 0; k < cn; k++)
                result = std::max(result, (ST)cv_absdiff(e1[k], e2[k]));
        }
        परत result;
    }
};
ढाँचा <typename T, typename ST>
संरचना MaskedNormDiffL1_SIMD {
    ओळीत ST चालक()(स्थिरांक T* s1, स्थिरांक T* s2, स्थिरांक uchar* mask, पूर्णांक len, पूर्णांक cn) स्थिरांक {
        ST result = 0;
        साठी (पूर्णांक i = 0; i < len; i++) जर (mask[i]) {
            स्थिरांक T* e1 = s1 + i*cn; स्थिरांक T* e2 = s2 + i*cn;
            साठी (पूर्णांक k = 0; k < cn; k++)
                result += (ST)cv_absdiff(e1[k], e2[k]);
        }
        परत result;
    }
};
ढाँचा <typename T, typename ST>
संरचना MaskedNormDiffL2_SIMD {
    ओळीत ST चालक()(स्थिरांक T* s1, स्थिरांक T* s2, स्थिरांक uchar* mask, पूर्णांक len, पूर्णांक cn) स्थिरांक {
        ST result = 0;
        साठी (पूर्णांक i = 0; i < len; i++) जर (mask[i]) {
            स्थिरांक T* e1 = s1 + i*cn; स्थिरांक T* e2 = s2 + i*cn;
            साठी (पूर्णांक k = 0; k < cn; k++) { ST v = (ST)e1[k] - (ST)e2[k]; result += v*v; }
        }
        परत result;
    }
};

#जर (CV_SIMD || CV_SIMD_SCALABLE)
// AND a per-lane diff vector with the (expanded) mask>0 predicate.
ओळीत v_uint8  v_diffmask(स्थिरांक v_uint8&  d, स्थिरांक uchar* m) { परत v_and(d, v_gt(vx_load(m),          vx_setzero_u8())); }
ओळीत v_uint16 v_diffmask(स्थिरांक v_uint16& d, स्थिरांक uchar* m) { परत v_and(d, v_gt(vx_load_expand(m),   vx_setzero_u16())); }
ओळीत v_uint32 v_diffmask(स्थिरांक v_uint32& d, स्थिरांक uchar* m) { परत v_and(d, v_gt(vx_load_expand_q(m), vx_setzero_u32())); }
ओळीत v_float32 v_diffmask(स्थिरांक v_float32& d, स्थिरांक uchar* m) {
    v_uint32 cm = v_gt(vx_load_expand_q(m), vx_setzero_u32());
    परत v_reinterpret_as_f32(v_and(v_reinterpret_as_u32(d), cm));
}

// Shared scalar cn>1 fallbacks (match the base templates' cv_absdiff semantics).
ढाँचा<typename T, typename RT>
स्थिर ओळीत RT maskedNormDiffInfTail(स्थिरांक T* s1, स्थिरांक T* s2, स्थिरांक uchar* mask, पूर्णांक len, पूर्णांक cn, RT result) {
    साठी (पूर्णांक i = 0; i < len; i++) जर (mask[i]) {
        स्थिरांक T* e1 = s1 + i*cn; स्थिरांक T* e2 = s2 + i*cn;
        साठी (पूर्णांक k = 0; k < cn; k++) result = std::max(result, (RT)cv_absdiff(e1[k], e2[k]));
    }
    परत result;
}
ढाँचा<typename T, typename RT>
स्थिर ओळीत RT maskedNormDiffL1Tail(स्थिरांक T* s1, स्थिरांक T* s2, स्थिरांक uchar* mask, पूर्णांक len, पूर्णांक cn, RT result) {
    साठी (पूर्णांक i = 0; i < len; i++) जर (mask[i]) {
        स्थिरांक T* e1 = s1 + i*cn; स्थिरांक T* e2 = s2 + i*cn;
        साठी (पूर्णांक k = 0; k < cn; k++) result += (RT)cv_absdiff(e1[k], e2[k]);
    }
    परत result;
}
ढाँचा<typename T, typename RT>
स्थिर ओळीत RT maskedNormDiffL2Tail(स्थिरांक T* s1, स्थिरांक T* s2, स्थिरांक uchar* mask, पूर्णांक len, पूर्णांक cn, RT result) {
    साठी (पूर्णांक i = 0; i < len; i++) जर (mask[i]) {
        स्थिरांक T* e1 = s1 + i*cn; स्थिरांक T* e2 = s2 + i*cn;
        साठी (पूर्णांक k = 0; k < cn; k++) { RT v = (RT)e1[k] - (RT)e2[k]; result += v*v; }
    }
    परत result;
}

// Inf (cn==1): reduce_max of masked |a-b|. Shared across SIMD-capable depths.
ढाँचा<typename T, typename ST>
स्थिर ओळीत ST maskedNormDiffInfCn1(स्थिरांक T* s1, स्थिरांक T* s2, स्थिरांक uchar* mask, पूर्णांक len) {
    स्थिरांक पूर्णांक vstep = VTraits<decltype(vx_load(s1))>::vlanes();
    ST result = 0;
    पूर्णांक i = 0;
    जर (len >= vstep) {
        स्वतः acc = v_diffmask(v_absdiff(vx_load(s1), vx_load(s2)), mask);
        साठी (i = vstep; i <= len - vstep; i += vstep)
            acc = v_max(acc, v_diffmask(v_absdiff(vx_load(s1 + i), vx_load(s2 + i)), mask + i));
        result = (ST)v_reduce_max(acc);
    }
    साठी (; i < len; i++) जर (mask[i]) result = std::max(result, (ST)cv_absdiff(s1[i], s2[i]));
    परत result;
}

// Shared 8-bit cn==1 masked L1/L2 kernels (uchar/schar): v_absdiff yields v_uint8 for both.
ढाँचा<typename T>
स्थिर ओळीत पूर्णांक maskedNormDiffL1_8(स्थिरांक T* s1, स्थिरांक T* s2, स्थिरांक uchar* mask, पूर्णांक len) {
    पूर्णांक i = 0; स्थिरांक पूर्णांक vstep = VTraits<v_uint8>::vlanes();
    स्थिरांक v_uint8 one = vx_setall_u8(1);
    v_uint32 acc = vx_setzero_u32();
    साठी (; i <= len - vstep; i += vstep) {
        v_uint8 m  = v_gt(vx_load(mask + i), vx_setzero_u8());
        v_uint8 ad = v_and(v_absdiff(vx_load(s1 + i), vx_load(s2 + i)), m);
        acc = v_dotprod_expand_fast(ad, one, acc);
    }
    पूर्णांक result = (पूर्णांक)v_reduce_sum(acc);
    साठी (; i < len; i++) जर (mask[i]) result += (पूर्णांक)cv_absdiff(s1[i], s2[i]);
    परत result;
}
ढाँचा<typename T>
स्थिर ओळीत पूर्णांक maskedNormDiffL2_8(स्थिरांक T* s1, स्थिरांक T* s2, स्थिरांक uchar* mask, पूर्णांक len) {
    पूर्णांक i = 0; स्थिरांक पूर्णांक vstep = VTraits<v_uint8>::vlanes();
    v_uint32 acc = vx_setzero_u32();
    साठी (; i <= len - vstep; i += vstep) {
        v_uint8 m  = v_gt(vx_load(mask + i), vx_setzero_u8());
        v_uint8 ad = v_and(v_absdiff(vx_load(s1 + i), vx_load(s2 + i)), m);
        acc = v_dotprod_expand_fast(ad, ad, acc);
    }
    पूर्णांक result = (पूर्णांक)v_reduce_sum(acc);
    साठी (; i < len; i++) जर (mask[i]) { पूर्णांक v = (पूर्णांक)s1[i] - (पूर्णांक)s2[i]; result += v*v; }
    परत result;
}

// Shared 16-bit cn==1 masked L1 kernel (ushort/short): v_absdiff yields v_uint16 for both.
ढाँचा<typename T>
स्थिर ओळीत पूर्णांक maskedNormDiffL1_16(स्थिरांक T* s1, स्थिरांक T* s2, स्थिरांक uchar* mask, पूर्णांक len) {
    पूर्णांक i = 0; स्थिरांक पूर्णांक vstep = VTraits<v_uint16>::vlanes();
    v_uint32 acc = vx_setzero_u32();
    साठी (; i <= len - vstep; i += vstep) {
        v_uint16 m  = v_gt(vx_load_expand(mask + i), vx_setzero_u16());
        v_uint16 ad = v_and(v_absdiff(vx_load(s1 + i), vx_load(s2 + i)), m);
        v_uint32 lo, hi; v_expand(ad, lo, hi);
        acc = v_add(acc, v_add(lo, hi));
    }
    पूर्णांक result = (पूर्णांक)v_reduce_sum(acc);
    साठी (; i < len; i++) जर (mask[i]) result += (पूर्णांक)cv_absdiff(s1[i], s2[i]);
    परत result;
}

ढाँचा<> संरचना MaskedNormDiffInf_SIMD<uchar, पूर्णांक> {
    पूर्णांक चालक()(स्थिरांक uchar* s1, स्थिरांक uchar* s2, स्थिरांक uchar* mask, पूर्णांक len, पूर्णांक cn) स्थिरांक {
        जर (cn == 1) परत maskedNormDiffInfCn1<uchar, पूर्णांक>(s1, s2, mask, len);
        जर (cn == 4 && len >= VTraits<v_uint8>::vlanes()) {
            स्थिरांक पूर्णांक vstep = VTraits<v_uint8>::vlanes();
            v_uint8 acc = vx_setzero_u8();
            पूर्णांक i = 0;
            साठी (;;) {
                जर (i > len - vstep) i = len - vstep; // back-step (max is idempotent)
                v_uint8 a0,a1,a2,a3,b0,b1,b2,b3;
                v_load_deinterleave(s1 + i*4, a0,a1,a2,a3);
                v_load_deinterleave(s2 + i*4, b0,b1,b2,b3);
                v_uint8 ad = v_max(v_max(v_absdiff(a0,b0), v_absdiff(a1,b1)),
                                   v_max(v_absdiff(a2,b2), v_absdiff(a3,b3)));
                v_uint8 m = v_gt(vx_load(mask + i), vx_setzero_u8());
                acc = v_max(acc, v_and(ad, m));
                जर (i >= len - vstep) थांबा;
                i += vstep;
            }
            परत (पूर्णांक)v_reduce_max(acc);
        }
        परत maskedNormDiffInfTail<uchar, पूर्णांक>(s1, s2, mask, len, cn, 0);
    }
};
ढाँचा<> संरचना MaskedNormDiffInf_SIMD<schar, पूर्णांक> {
    पूर्णांक चालक()(स्थिरांक schar* s1, स्थिरांक schar* s2, स्थिरांक uchar* mask, पूर्णांक len, पूर्णांक cn) स्थिरांक {
        जर (cn == 1) परत maskedNormDiffInfCn1<schar, पूर्णांक>(s1, s2, mask, len);
        परत maskedNormDiffInfTail<schar, पूर्णांक>(s1, s2, mask, len, cn, 0);
    }
};
ढाँचा<> संरचना MaskedNormDiffInf_SIMD<ushort, पूर्णांक> {
    पूर्णांक चालक()(स्थिरांक ushort* s1, स्थिरांक ushort* s2, स्थिरांक uchar* mask, पूर्णांक len, पूर्णांक cn) स्थिरांक {
        जर (cn == 1) परत maskedNormDiffInfCn1<ushort, पूर्णांक>(s1, s2, mask, len);
        परत maskedNormDiffInfTail<ushort, पूर्णांक>(s1, s2, mask, len, cn, 0);
    }
};
ढाँचा<> संरचना MaskedNormDiffInf_SIMD<लघु, पूर्णांक> {
    पूर्णांक चालक()(स्थिरांक लघु* s1, स्थिरांक लघु* s2, स्थिरांक uchar* mask, पूर्णांक len, पूर्णांक cn) स्थिरांक {
        जर (cn == 1) परत maskedNormDiffInfCn1<लघु, पूर्णांक>(s1, s2, mask, len);
        परत maskedNormDiffInfTail<लघु, पूर्णांक>(s1, s2, mask, len, cn, 0);
    }
};
ढाँचा<> संरचना MaskedNormDiffInf_SIMD<पूर्णांक, अचिन्हित> {
    अचिन्हित चालक()(स्थिरांक पूर्णांक* s1, स्थिरांक पूर्णांक* s2, स्थिरांक uchar* mask, पूर्णांक len, पूर्णांक cn) स्थिरांक {
        जर (cn == 1) परत maskedNormDiffInfCn1<पूर्णांक, अचिन्हित>(s1, s2, mask, len);
        परत maskedNormDiffInfTail<पूर्णांक, अचिन्हित>(s1, s2, mask, len, cn, 0u);
    }
};
ढाँचा<> संरचना MaskedNormDiffInf_SIMD<दशांश, दशांश> {
    दशांश चालक()(स्थिरांक दशांश* s1, स्थिरांक दशांश* s2, स्थिरांक uchar* mask, पूर्णांक len, पूर्णांक cn) स्थिरांक {
        जर (cn == 1) परत maskedNormDiffInfCn1<दशांश, दशांश>(s1, s2, mask, len);
        परत maskedNormDiffInfTail<दशांश, दशांश>(s1, s2, mask, len, cn, 0.0f);
    }
};

ढाँचा<> संरचना MaskedNormDiffL1_SIMD<uchar, पूर्णांक> {
    पूर्णांक चालक()(स्थिरांक uchar* s1, स्थिरांक uchar* s2, स्थिरांक uchar* mask, पूर्णांक len, पूर्णांक cn) स्थिरांक {
        जर (cn == 1) परत maskedNormDiffL1_8(s1, s2, mask, len);
        जर (cn == 4) {
            पूर्णांक i = 0; स्थिरांक पूर्णांक vstep = VTraits<v_uint8>::vlanes();
            स्थिरांक v_uint8 one = vx_setall_u8(1);
            v_uint32 acc = vx_setzero_u32();
            साठी (; i <= len - vstep; i += vstep) {
                v_uint8 a0,a1,a2,a3,b0,b1,b2,b3;
                v_load_deinterleave(s1 + i*4, a0,a1,a2,a3);
                v_load_deinterleave(s2 + i*4, b0,b1,b2,b3);
                v_uint8 m = v_gt(vx_load(mask + i), vx_setzero_u8());
                acc = v_dotprod_expand_fast(v_and(v_absdiff(a0,b0), m), one, acc);
                acc = v_dotprod_expand_fast(v_and(v_absdiff(a1,b1), m), one, acc);
                acc = v_dotprod_expand_fast(v_and(v_absdiff(a2,b2), m), one, acc);
                acc = v_dotprod_expand_fast(v_and(v_absdiff(a3,b3), m), one, acc);
            }
            पूर्णांक result = (पूर्णांक)v_reduce_sum(acc);
            साठी (; i < len; i++) जर (mask[i]) {
                स्थिरांक uchar* e1 = s1 + i*4; स्थिरांक uchar* e2 = s2 + i*4;
                साठी (पूर्णांक k = 0; k < 4; k++) result += (पूर्णांक)cv_absdiff(e1[k], e2[k]);
            }
            परत result;
        }
        परत maskedNormDiffL1Tail<uchar, पूर्णांक>(s1, s2, mask, len, cn, 0);
    }
};
ढाँचा<> संरचना MaskedNormDiffL1_SIMD<schar, पूर्णांक> {
    पूर्णांक चालक()(स्थिरांक schar* s1, स्थिरांक schar* s2, स्थिरांक uchar* mask, पूर्णांक len, पूर्णांक cn) स्थिरांक {
        जर (cn == 1) परत maskedNormDiffL1_8(s1, s2, mask, len);
        परत maskedNormDiffL1Tail<schar, पूर्णांक>(s1, s2, mask, len, cn, 0);
    }
};
ढाँचा<> संरचना MaskedNormDiffL1_SIMD<ushort, पूर्णांक> {
    पूर्णांक चालक()(स्थिरांक ushort* s1, स्थिरांक ushort* s2, स्थिरांक uchar* mask, पूर्णांक len, पूर्णांक cn) स्थिरांक {
        जर (cn == 1) परत maskedNormDiffL1_16(s1, s2, mask, len);
        परत maskedNormDiffL1Tail<ushort, पूर्णांक>(s1, s2, mask, len, cn, 0);
    }
};
ढाँचा<> संरचना MaskedNormDiffL1_SIMD<लघु, पूर्णांक> {
    पूर्णांक चालक()(स्थिरांक लघु* s1, स्थिरांक लघु* s2, स्थिरांक uchar* mask, पूर्णांक len, पूर्णांक cn) स्थिरांक {
        जर (cn == 1) परत maskedNormDiffL1_16(s1, s2, mask, len);
        परत maskedNormDiffL1Tail<लघु, पूर्णांक>(s1, s2, mask, len, cn, 0);
    }
};

ढाँचा<> संरचना MaskedNormDiffL2_SIMD<uchar, पूर्णांक> {
    पूर्णांक चालक()(स्थिरांक uchar* s1, स्थिरांक uchar* s2, स्थिरांक uchar* mask, पूर्णांक len, पूर्णांक cn) स्थिरांक {
        जर (cn == 1) परत maskedNormDiffL2_8(s1, s2, mask, len);
        जर (cn == 4) {
            पूर्णांक i = 0; स्थिरांक पूर्णांक vstep = VTraits<v_uint8>::vlanes();
            v_uint32 acc = vx_setzero_u32();
            साठी (; i <= len - vstep; i += vstep) {
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
            पूर्णांक result = (पूर्णांक)v_reduce_sum(acc);
            साठी (; i < len; i++) जर (mask[i]) {
                स्थिरांक uchar* e1 = s1 + i*4; स्थिरांक uchar* e2 = s2 + i*4;
                साठी (पूर्णांक k = 0; k < 4; k++) { पूर्णांक v = (पूर्णांक)e1[k] - (पूर्णांक)e2[k]; result += v*v; }
            }
            परत result;
        }
        परत maskedNormDiffL2Tail<uchar, पूर्णांक>(s1, s2, mask, len, cn, 0);
    }
};
ढाँचा<> संरचना MaskedNormDiffL2_SIMD<schar, पूर्णांक> {
    पूर्णांक चालक()(स्थिरांक schar* s1, स्थिरांक schar* s2, स्थिरांक uchar* mask, पूर्णांक len, पूर्णांक cn) स्थिरांक {
        जर (cn == 1) परत maskedNormDiffL2_8(s1, s2, mask, len);
        परत maskedNormDiffL2Tail<schar, पूर्णांक>(s1, s2, mask, len, cn, 0);
    }
};

#जर CV_SIMD_64F
// Depths whose L1/L2 accumulate in double.
ढाँचा<> संरचना MaskedNormDiffL1_SIMD<पूर्णांक, द्विगुण> {
    द्विगुण चालक()(स्थिरांक पूर्णांक* s1, स्थिरांक पूर्णांक* s2, स्थिरांक uchar* mask, पूर्णांक len, पूर्णांक cn) स्थिरांक {
        जर (cn != 1) परत maskedNormDiffL1Tail<पूर्णांक, द्विगुण>(s1, s2, mask, len, cn, 0.0);
        पूर्णांक i = 0; स्थिरांक पूर्णांक vstep = VTraits<v_int32>::vlanes();
        v_float64 acc = vx_setzero_f64();
        साठी (; i <= len - vstep; i += vstep) {
            v_uint32 m  = v_gt(vx_load_expand_q(mask + i), vx_setzero_u32());
            v_uint32 ad = v_and(v_absdiff(vx_load(s1 + i), vx_load(s2 + i)), m);
            v_uint64 e0, e1; v_expand(ad, e0, e1);
            acc = v_add(acc, v_cvt_f64(v_reinterpret_as_s64(e0)));
            acc = v_add(acc, v_cvt_f64(v_reinterpret_as_s64(e1)));
        }
        द्विगुण result = v_reduce_sum(acc);
        साठी (; i < len; i++) जर (mask[i]) result += (द्विगुण)cv_absdiff(s1[i], s2[i]);
        परत result;
    }
};
ढाँचा<> संरचना MaskedNormDiffL1_SIMD<दशांश, द्विगुण> {
    द्विगुण चालक()(स्थिरांक दशांश* s1, स्थिरांक दशांश* s2, स्थिरांक uchar* mask, पूर्णांक len, पूर्णांक cn) स्थिरांक {
        जर (cn != 1) परत maskedNormDiffL1Tail<दशांश, द्विगुण>(s1, s2, mask, len, cn, 0.0);
        पूर्णांक i = 0; स्थिरांक पूर्णांक vstep = VTraits<v_float32>::vlanes();
        v_float64 acc0 = vx_setzero_f64(), acc1 = vx_setzero_f64();
        साठी (; i <= len - vstep; i += vstep) {
            v_float32 m  = v_reinterpret_as_f32(v_gt(vx_load_expand_q(mask + i), vx_setzero_u32()));
            v_float32 ad = v_and(v_absdiff(vx_load(s1 + i), vx_load(s2 + i)), m);
            acc0 = v_add(acc0, v_cvt_f64(ad));
            acc1 = v_add(acc1, v_cvt_f64_वरचा(ad));
        }
        द्विगुण result = v_reduce_sum(v_add(acc0, acc1));
        साठी (; i < len; i++) जर (mask[i]) result += (द्विगुण)cv_absdiff(s1[i], s2[i]);
        परत result;
    }
};

ढाँचा<typename T>
स्थिर ओळीत द्विगुण maskedNormDiffL2_16(स्थिरांक T* s1, स्थिरांक T* s2, स्थिरांक uchar* mask, पूर्णांक len, पूर्णांक cn) {
    जर (cn != 1) परत maskedNormDiffL2Tail<T, द्विगुण>(s1, s2, mask, len, cn, 0.0);
    पूर्णांक i = 0; स्थिरांक पूर्णांक vstep = VTraits<v_uint16>::vlanes();
    v_float64 acc = vx_setzero_f64();
    साठी (; i <= len - vstep; i += vstep) {
        v_uint16 m  = v_gt(vx_load_expand(mask + i), vx_setzero_u16());
        v_uint16 ad = v_and(v_absdiff(vx_load(s1 + i), vx_load(s2 + i)), m);
        v_uint64 u = v_dotprod_expand_fast(ad, ad);
        acc = v_add(acc, v_cvt_f64(v_reinterpret_as_s64(u)));
    }
    द्विगुण result = v_reduce_sum(acc);
    साठी (; i < len; i++) जर (mask[i]) { द्विगुण v = (द्विगुण)s1[i] - (द्विगुण)s2[i]; result += v*v; }
    परत result;
}
ढाँचा<> संरचना MaskedNormDiffL2_SIMD<ushort, द्विगुण> {
    द्विगुण चालक()(स्थिरांक ushort* s1, स्थिरांक ushort* s2, स्थिरांक uchar* mask, पूर्णांक len, पूर्णांक cn) स्थिरांक
    { परत maskedNormDiffL2_16(s1, s2, mask, len, cn); } };
ढाँचा<> संरचना MaskedNormDiffL2_SIMD<लघु, द्विगुण> {
    द्विगुण चालक()(स्थिरांक लघु* s1, स्थिरांक लघु* s2, स्थिरांक uchar* mask, पूर्णांक len, पूर्णांक cn) स्थिरांक
    { परत maskedNormDiffL2_16(s1, s2, mask, len, cn); } };

ढाँचा<> संरचना MaskedNormDiffL2_SIMD<पूर्णांक, द्विगुण> {
    द्विगुण चालक()(स्थिरांक पूर्णांक* s1, स्थिरांक पूर्णांक* s2, स्थिरांक uchar* mask, पूर्णांक len, पूर्णांक cn) स्थिरांक {
        जर (cn != 1) परत maskedNormDiffL2Tail<पूर्णांक, द्विगुण>(s1, s2, mask, len, cn, 0.0);
        पूर्णांक i = 0; स्थिरांक पूर्णांक vstep = VTraits<v_int32>::vlanes();
        v_float64 r0 = vx_setzero_f64(), r1 = vx_setzero_f64();
        साठी (; i <= len - vstep; i += vstep) {
            v_uint32 m  = v_gt(vx_load_expand_q(mask + i), vx_setzero_u32());
            v_uint32 ad = v_and(v_absdiff(vx_load(s1 + i), vx_load(s2 + i)), m);
            v_uint64 e0, e1; v_expand(ad, e0, e1);
            v_float64 f0 = v_cvt_f64(v_reinterpret_as_s64(e0)), f1 = v_cvt_f64(v_reinterpret_as_s64(e1));
            r0 = v_fma(f0, f0, r0); r1 = v_fma(f1, f1, r1);
        }
        द्विगुण result = v_reduce_sum(v_add(r0, r1));
        साठी (; i < len; i++) जर (mask[i]) { द्विगुण v = (द्विगुण)s1[i] - (द्विगुण)s2[i]; result += v*v; }
        परत result;
    }
};
ढाँचा<> संरचना MaskedNormDiffL2_SIMD<दशांश, द्विगुण> {
    द्विगुण चालक()(स्थिरांक दशांश* s1, स्थिरांक दशांश* s2, स्थिरांक uchar* mask, पूर्णांक len, पूर्णांक cn) स्थिरांक {
        जर (cn != 1) परत maskedNormDiffL2Tail<दशांश, द्विगुण>(s1, s2, mask, len, cn, 0.0);
        पूर्णांक i = 0; स्थिरांक पूर्णांक vstep = VTraits<v_float32>::vlanes();
        v_float64 r0 = vx_setzero_f64(), r1 = vx_setzero_f64();
        साठी (; i <= len - vstep; i += vstep) {
            v_float32 m  = v_reinterpret_as_f32(v_gt(vx_load_expand_q(mask + i), vx_setzero_u32()));
            v_float32 ad = v_and(v_absdiff(vx_load(s1 + i), vx_load(s2 + i)), m);
            v_float64 f0 = v_cvt_f64(ad), f1 = v_cvt_f64_वरचा(ad);
            r0 = v_fma(f0, f0, r0); r1 = v_fma(f1, f1, r1);
        }
        द्विगुण result = v_reduce_sum(v_add(r0, r1));
        साठी (; i < len; i++) जर (mask[i]) { द्विगुण v = (द्विगुण)s1[i] - (द्विगुण)s2[i]; result += v*v; }
        परत result;
    }
};
ढाँचा<>
संरचना MaskedNormDiffInf_SIMD<द्विगुण, द्विगुण> {
    ओळीत द्विगुण चालक()(स्थिरांक द्विगुण* s1, स्थिरांक द्विगुण* s2, स्थिरांक uchar* mask, पूर्णांक len, पूर्णांक cn) स्थिरांक {
        द्विगुण result = 0.0;
        पूर्णांक i = 0;
#जर CV_SIMD_64F || CV_SIMD_SCALABLE_64F
        जर (cn == 1) {
            स्थिरांक पूर्णांक vstep = VTraits<v_float64>::vlanes();
            v_float64 acc0 = vx_setzero_f64(), acc1 = vx_setzero_f64();
            साठी (; i <= len - 2*vstep; i += 2*vstep) {
                v_uint64 m0, m1;
                v_normmask_u64(mask + i, m0, m1);
                v_float64 d0 = v_absdiff(vx_load(s1 + i), vx_load(s2 + i));
                v_float64 d1 = v_absdiff(vx_load(s1 + i + vstep), vx_load(s2 + i + vstep));
                acc0 = v_max(acc0, v_reinterpret_as_f64(v_and(v_reinterpret_as_u64(d0), m0)));
                acc1 = v_max(acc1, v_reinterpret_as_f64(v_and(v_reinterpret_as_u64(d1), m1)));
            }
            द्विगुण t[VTraits<v_float64>::max_nlanes];
            vx_store(t, v_max(acc0, acc1));
            साठी (पूर्णांक k = 0; k < vstep; k++)
                result = std::max(result, t[k]);
        }
#endif
        साठी (; i < len; i++) जर (mask[i]) {
            स्थिरांक द्विगुण* e1 = s1 + i*cn; स्थिरांक द्विगुण* e2 = s2 + i*cn;
            साठी (पूर्णांक k = 0; k < cn; k++) result = std::max(result, std::असल(e1[k] - e2[k]));
        }
        परत result;
    }
};

ढाँचा<>
संरचना MaskedNormDiffL1_SIMD<द्विगुण, द्विगुण> {
    द्विगुण चालक()(स्थिरांक द्विगुण* s1, स्थिरांक द्विगुण* s2, स्थिरांक uchar* mask, पूर्णांक len, पूर्णांक cn) स्थिरांक {
        द्विगुण result = 0.0;
        जर (cn == 1) {
            स्थिरांक पूर्णांक vstep = VTraits<v_float64>::vlanes();
            पूर्णांक i = 0;
            v_float64 acc0 = vx_setzero_f64(), acc1 = vx_setzero_f64();
            साठी (; i <= len - 2*vstep; i += 2*vstep) {
                v_uint64 m0, m1;
                v_normmask_u64(mask + i, m0, m1);
                v_float64 d0 = v_absdiff(vx_load(s1 + i), vx_load(s2 + i));
                v_float64 d1 = v_absdiff(vx_load(s1 + i + vstep), vx_load(s2 + i + vstep));
                acc0 = v_add(acc0, v_reinterpret_as_f64(v_and(v_reinterpret_as_u64(d0), m0)));
                acc1 = v_add(acc1, v_reinterpret_as_f64(v_and(v_reinterpret_as_u64(d1), m1)));
            }
            result = v_reduce_sum(v_add(acc0, acc1));
            साठी (; i < len; i++) जर (mask[i]) result += std::असल(s1[i] - s2[i]);
        } अन्यथा {
            result = maskedNormDiffL1Tail<द्विगुण, द्विगुण>(s1, s2, mask, len, cn, 0.0);
        }
        परत result;
    }
};

ढाँचा<>
संरचना MaskedNormDiffL2_SIMD<द्विगुण, द्विगुण> {
    द्विगुण चालक()(स्थिरांक द्विगुण* s1, स्थिरांक द्विगुण* s2, स्थिरांक uchar* mask, पूर्णांक len, पूर्णांक cn) स्थिरांक {
        द्विगुण result = 0.0;
        जर (cn == 1) {
            स्थिरांक पूर्णांक vstep = VTraits<v_float64>::vlanes();
            पूर्णांक i = 0;
            v_float64 acc0 = vx_setzero_f64(), acc1 = vx_setzero_f64();
            साठी (; i <= len - 2*vstep; i += 2*vstep) {
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
            साठी (; i < len; i++) जर (mask[i]) { द्विगुण v = s1[i] - s2[i]; result += v*v; }
        } अन्यथा {
            result = maskedNormDiffL2Tail<द्विगुण, द्विगुण>(s1, s2, mask, len, cn, 0.0);
        }
        परत result;
    }
};
#endif // CV_SIMD_64F
#endif // CV_SIMD

ढाँचा<typename T, typename ST> पूर्णांक
normDiffInf_(स्थिरांक T* src1, स्थिरांक T* src2, स्थिरांक uchar* mask, ST* _result, पूर्णांक len, पूर्णांक cn) {
    ST result = *_result;
    जर( !mask ) {
        NormDiffInf_SIMD<T, ST> op;
        result = std::max(result, op(src1, src2, len*cn));
    } अन्यथा {
        MaskedNormDiffInf_SIMD<T, ST> op;
        result = std::max(result, op(src1, src2, mask, len, cn));
    }
    *_result = result;
    परत 0;
}

ढाँचा<typename T, typename ST> पूर्णांक
normDiffL1_(स्थिरांक T* src1, स्थिरांक T* src2, स्थिरांक uchar* mask, ST* _result, पूर्णांक len, पूर्णांक cn) {
    ST result = *_result;
    जर( !mask ) {
        NormDiffL1_SIMD<T, ST> op;
        result += op(src1, src2, len*cn);
    } अन्यथा {
        MaskedNormDiffL1_SIMD<T, ST> op;
        result += op(src1, src2, mask, len, cn);
    }
    *_result = result;
    परत 0;
}

ढाँचा<typename T, typename ST> पूर्णांक
normDiffL2_(स्थिरांक T* src1, स्थिरांक T* src2, स्थिरांक uchar* mask, ST* _result, पूर्णांक len, पूर्णांक cn) {
    ST result = *_result;
    जर( !mask ) {
        NormDiffL2_SIMD<T, ST> op;
        result += op(src1, src2, len*cn);
    } अन्यथा {
        MaskedNormDiffL2_SIMD<T, ST> op;
        result += op(src1, src2, mask, len, cn);
    }
    *_result = result;
    परत 0;
}

स्थिर पूर्णांक
normDiffInf_Bool(स्थिरांक uchar* src1, स्थिरांक uchar* src2, स्थिरांक uchar* mask, पूर्णांक* _result, पूर्णांक len, पूर्णांक cn)
{
    पूर्णांक result = *_result;
    जर( !mask )
    {
        साठी( पूर्णांक i = 0; i < len*cn; i++ ) {
            result = std::max(result, (पूर्णांक)((src1[i] != 0) != (src2[i] != 0)));
            जर (result != 0)
                थांबा;
        }
    }
    अन्यथा
    {
        साठी( पूर्णांक i = 0; i < len; i++, src1 += cn, src2 += cn )
            जर( mask[i] )
            {
                साठी( पूर्णांक k = 0; k < cn; k++ )
                    result = std::max(result, (पूर्णांक)((src1[k] != 0) != (src2[k] != 0)));
                जर (result != 0)
                    थांबा;
            }
    }
    *_result = result;
    परत 0;
}

स्थिर पूर्णांक
normDiffL1_Bool(स्थिरांक uchar* src1, स्थिरांक uchar* src2, स्थिरांक uchar* mask, पूर्णांक* _result, पूर्णांक len, पूर्णांक cn)
{
    पूर्णांक result = *_result;
    जर( !mask )
    {
        साठी( पूर्णांक i = 0; i < len*cn; i++ )
            result += (पूर्णांक)((src1[i] != 0) != (src2[i] != 0));
    }
    अन्यथा
    {
        साठी( पूर्णांक i = 0; i < len; i++, src1 += cn, src2 += cn )
            जर( mask[i] )
            {
                साठी( पूर्णांक k = 0; k < cn; k++ )
                    result += (पूर्णांक)((src1[k] != 0) != (src2[k] != 0));
            }
    }
    *_result = result;
    परत 0;
}

स्थिर पूर्णांक
normDiffL2_Bool(स्थिरांक uchar* src1, स्थिरांक uchar* src2, स्थिरांक uchar* mask, पूर्णांक* _result, पूर्णांक len, पूर्णांक cn)
{
    परत normDiffL1_Bool(src1, src2, mask, _result, len, cn);
}

#define CV_DEF_NORM_FUNC(L, suffix, type, ntype) \
    स्थिर पूर्णांक norm##L##_##suffix(स्थिरांक type* src, स्थिरांक uchar* mask, ntype* r, पूर्णांक len, पूर्णांक cn) \
{ CV_INSTRUMENT_REGION(); परत norm##L##_(src, mask, r, len, cn); } \
    स्थिर पूर्णांक normDiff##L##_##suffix(स्थिरांक type* src1, स्थिरांक type* src2, \
    स्थिरांक uchar* mask, ntype* r, पूर्णांक len, पूर्णांक cn) \
{ परत normDiff##L##_(src1, src2, mask, r, (पूर्णांक)len, cn); }

#define CV_DEF_NORM_ALL(suffix, type, inftype, l1type, l2type) \
    CV_DEF_NORM_FUNC(Inf, suffix, type, inftype) \
    CV_DEF_NORM_FUNC(L1, suffix, type, l1type) \
    CV_DEF_NORM_FUNC(L2, suffix, type, l2type)

CV_DEF_NORM_ALL(8u, uchar, पूर्णांक, पूर्णांक, पूर्णांक)
CV_DEF_NORM_ALL(8s, schar, पूर्णांक, पूर्णांक, पूर्णांक)
CV_DEF_NORM_ALL(16u, ushort, पूर्णांक, पूर्णांक, द्विगुण)
CV_DEF_NORM_ALL(16s, लघु, पूर्णांक, पूर्णांक, द्विगुण)
CV_DEF_NORM_ALL(32u, अचिन्हित, अचिन्हित, द्विगुण, द्विगुण)
CV_DEF_NORM_ALL(32s, पूर्णांक, अचिन्हित, द्विगुण, द्विगुण)
CV_DEF_NORM_ALL(32f, दशांश, दशांश, द्विगुण, द्विगुण)
CV_DEF_NORM_ALL(64f, द्विगुण, द्विगुण, द्विगुण, द्विगुण)
CV_DEF_NORM_ALL(64u, uint64, uint64, द्विगुण, द्विगुण)
CV_DEF_NORM_ALL(64s, int64, uint64, द्विगुण, द्विगुण)
CV_DEF_NORM_ALL(16f, hfloat, दशांश, दशांश, दशांश)
CV_DEF_NORM_ALL(16bf, bfloat, दशांश, दशांश, दशांश)

NormFunc getNormFunc(पूर्णांक normType, पूर्णांक depth)
{
    CV_INSTRUMENT_REGION();

    स्थिर NormFunc normTab[3][CV_DEPTH_MAX] =
    {
        {
            normWrap<uchar, पूर्णांक, normInf_8u>,
            normWrap<schar, पूर्णांक, normInf_8s>,
            normWrap<ushort, पूर्णांक, normInf_16u>,
            normWrap<लघु, पूर्णांक, normInf_16s>,
            normWrap<पूर्णांक, अचिन्हित, normInf_32s>,
            normWrap<दशांश, दशांश, normInf_32f>,
            normWrap<द्विगुण, द्विगुण, normInf_64f>,
            normWrap<hfloat, दशांश, normInf_16f>,
            normWrap<bfloat, दशांश, normInf_16bf>,
            normWrap<uchar, पूर्णांक, normInf_Bool>,
            normWrap<uint64, uint64, normInf_64u>,
            normWrap<int64, uint64, normInf_64s>,
            normWrap<अचिन्हित, अचिन्हित, normInf_32u>,
            0
        },
        {
            normWrap<uchar, पूर्णांक, normL1_8u>,
            normWrap<schar, पूर्णांक, normL1_8s>,
            normWrap<ushort, पूर्णांक, normL1_16u>,
            normWrap<लघु, पूर्णांक, normL1_16s>,
            normWrap<पूर्णांक, द्विगुण, normL1_32s>,
            normWrap<दशांश, द्विगुण, normL1_32f>,
            normWrap<द्विगुण, द्विगुण, normL1_64f>,
            normWrap<hfloat, दशांश, normL1_16f>,
            normWrap<bfloat, दशांश, normL1_16bf>,
            normWrap<uchar, पूर्णांक, normL1_Bool>,
            normWrap<uint64, द्विगुण, normL1_64u>,
            normWrap<int64, द्विगुण, normL1_64s>,
            normWrap<अचिन्हित, द्विगुण, normL1_32u>,
            0
        },
        {
            normWrap<uchar, पूर्णांक, normL2_8u>,
            normWrap<schar, पूर्णांक, normL2_8s>,
            normWrap<ushort, द्विगुण, normL2_16u>,
            normWrap<लघु, द्विगुण, normL2_16s>,
            normWrap<पूर्णांक, द्विगुण, normL2_32s>,
            normWrap<दशांश, द्विगुण, normL2_32f>,
            normWrap<द्विगुण, द्विगुण, normL2_64f>,
            normWrap<hfloat, दशांश, normL2_16f>,
            normWrap<bfloat, दशांश, normL2_16bf>,
            normWrap<uchar, पूर्णांक, normL2_Bool>,
            normWrap<uint64, द्विगुण, normL2_64u>,
            normWrap<int64, द्विगुण, normL2_64s>,
            normWrap<अचिन्हित, द्विगुण, normL2_32u>,
            0
        }
    };

    जर (normType >= 3 || normType < 0) परत nullptr;

    परत normTab[normType][depth];
}

NormDiffFunc getNormDiffFunc(पूर्णांक normType, पूर्णांक depth)
{
    स्थिर NormDiffFunc normDiffTab[3][CV_DEPTH_MAX] =
    {
        {
            normDiffWrap<uchar, पूर्णांक, normDiffInf_8u>,
            normDiffWrap<schar, पूर्णांक, normDiffInf_8s>,
            normDiffWrap<ushort, पूर्णांक, normDiffInf_16u>,
            normDiffWrap<लघु, पूर्णांक, normDiffInf_16s>,
            normDiffWrap<पूर्णांक, अचिन्हित, normDiffInf_32s>,
            normDiffWrap<दशांश, दशांश, normDiffInf_32f>,
            normDiffWrap<द्विगुण, द्विगुण, normDiffInf_64f>,
            normDiffWrap<hfloat, दशांश, normDiffInf_16f>,
            normDiffWrap<bfloat, दशांश, normDiffInf_16bf>,
            normDiffWrap<uchar, पूर्णांक, normDiffInf_Bool>,
            normDiffWrap<uint64, uint64, normDiffInf_64u>,
            normDiffWrap<int64, uint64, normDiffInf_64s>,
            normDiffWrap<अचिन्हित, अचिन्हित, normDiffInf_32u>,
            0
        },
        {
            normDiffWrap<uchar, पूर्णांक, normDiffL1_8u>,
            normDiffWrap<schar, पूर्णांक, normDiffL1_8s>,
            normDiffWrap<ushort, पूर्णांक, normDiffL1_16u>,
            normDiffWrap<लघु, पूर्णांक, normDiffL1_16s>,
            normDiffWrap<पूर्णांक, द्विगुण, normDiffL1_32s>,
            normDiffWrap<दशांश, द्विगुण, normDiffL1_32f>,
            normDiffWrap<द्विगुण, द्विगुण, normDiffL1_64f>,
            normDiffWrap<hfloat, दशांश, normDiffL1_16f>,
            normDiffWrap<bfloat, दशांश, normDiffL1_16bf>,
            normDiffWrap<uchar, पूर्णांक, normDiffL1_Bool>,
            normDiffWrap<uint64, द्विगुण, normDiffL1_64u>,
            normDiffWrap<int64, द्विगुण, normDiffL1_64s>,
            normDiffWrap<अचिन्हित, द्विगुण, normDiffL1_32u>,
            0
        },
        {
            normDiffWrap<uchar, पूर्णांक, normDiffL2_8u>,
            normDiffWrap<schar, पूर्णांक, normDiffL2_8s>,
            normDiffWrap<ushort, द्विगुण, normDiffL2_16u>,
            normDiffWrap<लघु, द्विगुण, normDiffL2_16s>,
            normDiffWrap<पूर्णांक, द्विगुण, normDiffL2_32s>,
            normDiffWrap<दशांश, द्विगुण, normDiffL2_32f>,
            normDiffWrap<द्विगुण, द्विगुण, normDiffL2_64f>,
            normDiffWrap<hfloat, दशांश, normDiffL2_16f>,
            normDiffWrap<bfloat, दशांश, normDiffL2_16bf>,
            normDiffWrap<uchar, पूर्णांक, normDiffL2_Bool>,
            normDiffWrap<uint64, द्विगुण, normDiffL2_64u>,
            normDiffWrap<int64, द्विगुण, normDiffL2_64s>,
            normDiffWrap<अचिन्हित, द्विगुण, normDiffL2_32u>,
            0
        },
    };
    जर (normType >= 3 || normType < 0) परत nullptr;

    परत normDiffTab[normType][depth];
}

#endif // CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

CV_CPU_OPTIMIZATION_NAMESPACE_END

} // cv::
