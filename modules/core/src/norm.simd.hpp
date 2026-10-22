// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html
//
// Copyright (C) 2025, SpaceMIT Inc., all rights reserved.
// Copyright (C) 2026, Advanced Micro Devices, Inc., all rights reserved.
// Third party copyrights are property of their respective owners.

#অন্তর্ভাব "precomp.hpp"

নামাকাশ cv {

using NormFunc = পূর্ণসংখ্যা (*)(ধ্রুবক uchar*, ধ্রুবক uchar*, uchar*, পূর্ণসংখ্যা, পূর্ণসংখ্যা);
using NormDiffFunc = পূর্ণসংখ্যা (*)(ধ্রুবক uchar*, ধ্রুবক uchar*, ধ্রুবক uchar*, uchar*, পূর্ণসংখ্যা, পূর্ণসংখ্যা);

CV_CPU_OPTIMIZATION_NAMESPACE_BEGIN

template<typename T, typename RT, পূর্ণসংখ্যা (*fn)(ধ্রুবক T*, ধ্রুবক uchar*, RT*, পূর্ণসংখ্যা, পূর্ণসংখ্যা)>
স্থির পূর্ণসংখ্যা normWrap(ধ্রুবক uchar* src1, ধ্রুবক uchar* src2, uchar* res, পূর্ণসংখ্যা len, পূর্ণসংখ্যা cn)
{
    ফেরত fn((ধ্রুবক T*)src1, src2, (RT*)res, len, cn);
}

template<typename T, typename RT, পূর্ণসংখ্যা (*fn)(ধ্রুবক T*, ধ্রুবক T*, ধ্রুবক uchar*, RT*, পূর্ণসংখ্যা, পূর্ণসংখ্যা)>
স্থির পূর্ণসংখ্যা normDiffWrap(ধ্রুবক uchar* src1, ধ্রুবক uchar* src2, ধ্রুবক uchar* src3, uchar* res, পূর্ণসংখ্যা len, পূর্ণসংখ্যা cn)
{
    ফেরত fn((ধ্রুবক T*)src1, (ধ্রুবক T*)src2, src3, (RT*)res, len, cn);
}

NormFunc getNormFunc(পূর্ণসংখ্যা normType, পূর্ণসংখ্যা depth);
NormDiffFunc getNormDiffFunc(পূর্ণসংখ্যা normType, পূর্ণসংখ্যা depth);

#ifndef CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

template <typename T, typename ST>
কাঠামো NormInf_SIMD {
    অন্তঃরেখ ST operator() (ধ্রুবক T* src, পূর্ণসংখ্যা n) ধ্রুবক {
        ST s = 0;
        জন্য (পূর্ণসংখ্যা i = 0; i < n; i++) {
            s = std::max(s, (ST)cv_abs(src[i]));
        }
        ফেরত s;
    }
};

template <typename T, typename ST>
কাঠামো NormL1_SIMD {
    অন্তঃরেখ ST operator() (ধ্রুবক T* src, পূর্ণসংখ্যা n) ধ্রুবক {
        ST s = 0;
        জন্য (পূর্ণসংখ্যা i = 0; i < n; i++) {
            s += cv_abs(src[i]);
        }
        ফেরত s;
    }
};

template <typename T, typename ST>
কাঠামো NormL2_SIMD {
    অন্তঃরেখ ST operator() (ধ্রুবক T* src, পূর্ণসংখ্যা n) ধ্রুবক {
        ST s = 0;
        জন্য (পূর্ণসংখ্যা i = 0; i < n; i++) {
            ST v = (ST)src[i];
            s += v * v;
        }
        ফেরত s;
    }
};

template <typename T, typename ST>
কাঠামো NormDiffInf_SIMD {
    অন্তঃরেখ ST operator() (ধ্রুবক T* src1, ধ্রুবক T* src2, পূর্ণসংখ্যা n) ধ্রুবক {
        ST s = 0;
        জন্য (পূর্ণসংখ্যা i = 0; i < n; i++) {
            ST v = (ST)cv_absdiff(src1[i], src2[i]);
            s = std::max(s, v);
        }
        ফেরত s;
    }
};

template <typename T, typename ST>
কাঠামো NormDiffL1_SIMD {
    অন্তঃরেখ ST operator() (ধ্রুবক T* src1, ধ্রুবক T* src2, পূর্ণসংখ্যা n) ধ্রুবক {
        ST s = 0;
        জন্য (পূর্ণসংখ্যা i = 0; i < n; i++) {
            ST v = (ST)cv_absdiff(src1[i], src2[i]);
            s += v;
        }
        ফেরত s;
    }
};

template <typename T, typename ST>
কাঠামো NormDiffL2_SIMD {
    অন্তঃরেখ ST operator() (ধ্রুবক T* src1, ধ্রুবক T* src2, পূর্ণসংখ্যা n) ধ্রুবক {
        ST s = 0;
        জন্য (পূর্ণসংখ্যা i = 0; i < n; i++) {
            ST v = (ST)src1[i] - (ST)src2[i];
            s += v * v;
        }
        ফেরত s;
    }
};

#যদি (CV_SIMD || CV_SIMD_SCALABLE)

template<>
কাঠামো NormInf_SIMD<uchar, পূর্ণসংখ্যা> {
    পূর্ণসংখ্যা operator() (ধ্রুবক uchar* src, পূর্ণসংখ্যা n) ধ্রুবক {
        পূর্ণসংখ্যা j = 0;
        পূর্ণসংখ্যা s = 0;
        v_uint8 r0 = vx_setzero_u8(), r1 = vx_setzero_u8();
        v_uint8 r2 = vx_setzero_u8(), r3 = vx_setzero_u8();
        জন্য (; j <= n - 4 * VTraits<v_uint8>::vlanes(); j += 4 * VTraits<v_uint8>::vlanes()) {
            r0 = v_max(r0, vx_load(src + j                                 ));
            r1 = v_max(r1, vx_load(src + j +     VTraits<v_uint8>::vlanes()));
            r2 = v_max(r2, vx_load(src + j + 2 * VTraits<v_uint8>::vlanes()));
            r3 = v_max(r3, vx_load(src + j + 3 * VTraits<v_uint8>::vlanes()));
        }
        r0 = v_max(r0, v_max(r1, v_max(r2, r3)));
        জন্য (; j < n; j++) {
            s = std::max(s, (পূর্ণসংখ্যা)src[j]);
        }
        ফেরত std::max(s, (পূর্ণসংখ্যা)v_reduce_max(r0));
    }
};

template<>
কাঠামো NormInf_SIMD<schar, পূর্ণসংখ্যা> {
    পূর্ণসংখ্যা operator() (ধ্রুবক schar* src, পূর্ণসংখ্যা n) ধ্রুবক {
        পূর্ণসংখ্যা j = 0;
        পূর্ণসংখ্যা s = 0;
        v_uint8 r0 = vx_setzero_u8(), r1 = vx_setzero_u8();
        v_uint8 r2 = vx_setzero_u8(), r3 = vx_setzero_u8();
        জন্য (; j <= n - 4 * VTraits<v_int8>::vlanes(); j += 4 * VTraits<v_int8>::vlanes()) {
            r0 = v_max(r0, v_abs(vx_load(src + j                                )));
            r1 = v_max(r1, v_abs(vx_load(src + j +     VTraits<v_int8>::vlanes())));
            r2 = v_max(r2, v_abs(vx_load(src + j + 2 * VTraits<v_int8>::vlanes())));
            r3 = v_max(r3, v_abs(vx_load(src + j + 3 * VTraits<v_int8>::vlanes())));
        }
        r0 = v_max(r0, v_max(r1, v_max(r2, r3)));
        জন্য (; j < n; j++) {
            s = std::max(s, cv_abs(src[j]));
        }
        ফেরত std::max(s, saturate_cast<পূর্ণসংখ্যা>(v_reduce_max(r0)));
    }
};

template<>
কাঠামো NormInf_SIMD<ushort, পূর্ণসংখ্যা> {
    পূর্ণসংখ্যা operator() (ধ্রুবক ushort* src, পূর্ণসংখ্যা n) ধ্রুবক {
        পূর্ণসংখ্যা j = 0;
        পূর্ণসংখ্যা s = 0;
        v_uint16 d0 = vx_setzero_u16(), d1 = vx_setzero_u16();
        v_uint16 d2 = vx_setzero_u16(), d3 = vx_setzero_u16();
        জন্য (; j <= n - 4 * VTraits<v_uint16>::vlanes(); j += 4 * VTraits<v_uint16>::vlanes()) {
            d0 = v_max(d0, vx_load(src + j                                  ));
            d1 = v_max(d1, vx_load(src + j +     VTraits<v_uint16>::vlanes()));
            d2 = v_max(d2, vx_load(src + j + 2 * VTraits<v_uint16>::vlanes()));
            d3 = v_max(d3, vx_load(src + j + 3 * VTraits<v_uint16>::vlanes()));
        }
        d0 = v_max(d0, v_max(d1, v_max(d2, d3)));
        জন্য (; j < n; j++) {
            s = std::max(s, (পূর্ণসংখ্যা)src[j]);
        }
        ফেরত std::max(s, (পূর্ণসংখ্যা)v_reduce_max(d0));
    }
};

template<>
কাঠামো NormInf_SIMD<short, পূর্ণসংখ্যা> {
    পূর্ণসংখ্যা operator() (ধ্রুবক short* src, পূর্ণসংখ্যা n) ধ্রুবক {
        পূর্ণসংখ্যা j = 0;
        পূর্ণসংখ্যা s = 0;
        v_uint16 d0 = vx_setzero_u16(), d1 = vx_setzero_u16();
        v_uint16 d2 = vx_setzero_u16(), d3 = vx_setzero_u16();
        জন্য (; j <= n - 4 * VTraits<v_int16>::vlanes(); j += 4 * VTraits<v_int16>::vlanes()) {
            d0 = v_max(d0, v_abs(vx_load(src + j                                  )));
            d1 = v_max(d1, v_abs(vx_load(src + j +     VTraits<v_int16>::vlanes())));
            d2 = v_max(d2, v_abs(vx_load(src + j + 2 * VTraits<v_int16>::vlanes())));
            d3 = v_max(d3, v_abs(vx_load(src + j + 3 * VTraits<v_int16>::vlanes())));
        }
        d0 = v_max(d0, v_max(d1, v_max(d2, d3)));
        জন্য (; j < n; j++) {
            s = std::max(s, saturate_cast<পূর্ণসংখ্যা>(cv_abs(src[j])));
        }
        ফেরত std::max(s, saturate_cast<পূর্ণসংখ্যা>(v_reduce_max(d0)));
    }
};

template<>
কাঠামো NormInf_SIMD<পূর্ণসংখ্যা, পূর্ণসংখ্যা> {
    পূর্ণসংখ্যা operator() (ধ্রুবক পূর্ণসংখ্যা* src, পূর্ণসংখ্যা n) ধ্রুবক {
        পূর্ণসংখ্যা j = 0;
        পূর্ণসংখ্যা s = 0;
        v_uint32 r0 = vx_setzero_u32(), r1 = vx_setzero_u32();
        v_uint32 r2 = vx_setzero_u32(), r3 = vx_setzero_u32();
        জন্য (; j <= n - 4 * VTraits<v_int32>::vlanes(); j += 4 * VTraits<v_int32>::vlanes()) {
            r0 = v_max(r0, v_abs(vx_load(src + j                                 )));
            r1 = v_max(r1, v_abs(vx_load(src + j +     VTraits<v_int32>::vlanes())));
            r2 = v_max(r2, v_abs(vx_load(src + j + 2 * VTraits<v_int32>::vlanes())));
            r3 = v_max(r3, v_abs(vx_load(src + j + 3 * VTraits<v_int32>::vlanes())));
        }
        r0 = v_max(r0, v_max(r1, v_max(r2, r3)));
        s = std::max(s, saturate_cast<পূর্ণসংখ্যা>(v_reduce_max(r0)));
        জন্য (; j < n; j++) {
            s = std::max(s, std::abs(src[j]));
        }
        ফেরত s;
    }
};

template<>
কাঠামো NormInf_SIMD<দশমিক, দশমিক> {
    দশমিক operator() (ধ্রুবক দশমিক* src, পূর্ণসংখ্যা n) ধ্রুবক {
        পূর্ণসংখ্যা j = 0;
        দশমিক s = 0.f;
        v_float32 r0 = vx_setzero_f32(), r1 = vx_setzero_f32();
        v_float32 r2 = vx_setzero_f32(), r3 = vx_setzero_f32();
        জন্য (; j <= n - 4 * VTraits<v_float32>::vlanes(); j += 4 * VTraits<v_float32>::vlanes()) {
            r0 = v_max(r0, v_abs(vx_load(src + j                                   )));
            r1 = v_max(r1, v_abs(vx_load(src + j +     VTraits<v_float32>::vlanes())));
            r2 = v_max(r2, v_abs(vx_load(src + j + 2 * VTraits<v_float32>::vlanes())));
            r3 = v_max(r3, v_abs(vx_load(src + j + 3 * VTraits<v_float32>::vlanes())));
        }
        r0 = v_max(r0, v_max(r1, v_max(r2, r3)));
        জন্য (; j < n; j++) {
            s = std::max(s, cv_abs(src[j]));
        }
        ফেরত std::max(s, v_reduce_max(r0));
    }
};

template<>
কাঠামো NormL1_SIMD<uchar, পূর্ণসংখ্যা> {
    পূর্ণসংখ্যা operator() (ধ্রুবক uchar* src, পূর্ণসংখ্যা n) ধ্রুবক {
        পূর্ণসংখ্যা j = 0;
        পূর্ণসংখ্যা s = 0;
        v_uint32 r0 = vx_setzero_u32(), r1 = vx_setzero_u32();
        v_uint8  one = vx_setall_u8(1);
        জন্য (; j<= n - 2 * VTraits<v_uint8>::vlanes(); j += 2 * VTraits<v_uint8>::vlanes()) {
            v_uint8 v0 = vx_load(src + j);
            r0 = v_dotprod_expand_fast(v0, one, r0);

            v_uint8 v1 = vx_load(src + j + VTraits<v_uint8>::vlanes());
            r1 = v_dotprod_expand_fast(v1, one, r1);
        }
        s += v_reduce_যোগ(v_add(r0, r1));
        জন্য (; j < n; j++) {
            s += src[j];
        }
        ফেরত s;
    }
};

template<>
কাঠামো NormL1_SIMD<schar, পূর্ণসংখ্যা> {
    পূর্ণসংখ্যা operator() (ধ্রুবক schar* src, পূর্ণসংখ্যা n) ধ্রুবক {
        পূর্ণসংখ্যা j = 0;
        পূর্ণসংখ্যা s = 0;
        v_uint32 r0 = vx_setzero_u32(), r1 = vx_setzero_u32();
        v_uint8  one = vx_setall_u8(1);
        জন্য (; j<= n - 2 * VTraits<v_int8>::vlanes(); j += 2 * VTraits<v_int8>::vlanes()) {
            v_uint8 v0 = v_abs(vx_load(src + j));
            r0 = v_dotprod_expand_fast(v0, one, r0);

            v_uint8 v1 = v_abs(vx_load(src + j + VTraits<v_int8>::vlanes()));
            r1 = v_dotprod_expand_fast(v1, one, r1);
        }
        s += v_reduce_যোগ(v_add(r0, r1));
        জন্য (; j < n; j++) {
            s += saturate_cast<পূর্ণসংখ্যা>(cv_abs(src[j]));
        }
        ফেরত s;
    }
};

template<>
কাঠামো NormL1_SIMD<ushort, পূর্ণসংখ্যা> {
    পূর্ণসংখ্যা operator() (ধ্রুবক ushort* src, পূর্ণসংখ্যা n) ধ্রুবক {
        পূর্ণসংখ্যা j = 0;
        পূর্ণসংখ্যা s = 0;
        v_uint32 r00 = vx_setzero_u32(), r01 = vx_setzero_u32();
        v_uint32 r10 = vx_setzero_u32(), r11 = vx_setzero_u32();
        জন্য (; j<= n - 2 * VTraits<v_uint16>::vlanes(); j += 2 * VTraits<v_uint16>::vlanes()) {
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
        s += (পূর্ণসংখ্যা)v_reduce_যোগ(v_add(v_add(v_add(r00, r01), r10), r11));
        জন্য (; j < n; j++) {
            s += src[j];
        }
        ফেরত s;
    }
};

template<>
কাঠামো NormL1_SIMD<short, পূর্ণসংখ্যা> {
    পূর্ণসংখ্যা operator() (ধ্রুবক short* src, পূর্ণসংখ্যা n) ধ্রুবক {
        পূর্ণসংখ্যা j = 0;
        পূর্ণসংখ্যা s = 0;
        v_uint32 r00 = vx_setzero_u32(), r01 = vx_setzero_u32();
        v_uint32 r10 = vx_setzero_u32(), r11 = vx_setzero_u32();
        জন্য (; j<= n - 2 * VTraits<v_int16>::vlanes(); j += 2 * VTraits<v_int16>::vlanes()) {
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
        s += (পূর্ণসংখ্যা)v_reduce_যোগ(v_add(v_add(v_add(r00, r01), r10), r11));
        জন্য (; j < n; j++) {
            s += saturate_cast<পূর্ণসংখ্যা>(cv_abs(src[j]));
        }
        ফেরত s;
    }
};

template<>
কাঠামো NormL2_SIMD<uchar, পূর্ণসংখ্যা> {
    পূর্ণসংখ্যা operator() (ধ্রুবক uchar* src, পূর্ণসংখ্যা n) ধ্রুবক {
        পূর্ণসংখ্যা j = 0;
        পূর্ণসংখ্যা s = 0;
        v_uint32 r0 = vx_setzero_u32(), r1 = vx_setzero_u32();
        জন্য (; j <= n - 2 * VTraits<v_uint8>::vlanes(); j += 2 * VTraits<v_uint8>::vlanes()) {
            v_uint8 v0 = vx_load(src + j);
            r0 = v_dotprod_expand_fast(v0, v0, r0);

            v_uint8 v1 = vx_load(src + j + VTraits<v_uint8>::vlanes());
            r1 = v_dotprod_expand_fast(v1, v1, r1);
        }
        s += v_reduce_যোগ(v_add(r0, r1));
        জন্য (; j < n; j++) {
            পূর্ণসংখ্যা v = saturate_cast<পূর্ণসংখ্যা>(src[j]);
            s += v * v;
        }
        ফেরত s;
    }
};

template<>
কাঠামো NormL2_SIMD<schar, পূর্ণসংখ্যা> {
    পূর্ণসংখ্যা operator() (ধ্রুবক schar* src, পূর্ণসংখ্যা n) ধ্রুবক {
        পূর্ণসংখ্যা j = 0;
        পূর্ণসংখ্যা s = 0;
        v_int32 r0 = vx_setzero_s32(), r1 = vx_setzero_s32();
        জন্য (; j <= n - 2 * VTraits<v_int8>::vlanes(); j += 2 * VTraits<v_int8>::vlanes()) {
            v_int8 v0 = vx_load(src + j);
            r0 = v_dotprod_expand_fast(v0, v0, r0);
            v_int8 v1 = vx_load(src + j + VTraits<v_int8>::vlanes());
            r1 = v_dotprod_expand_fast(v1, v1, r1);
        }
        s += v_reduce_যোগ(v_add(r0, r1));
        জন্য (; j < n; j++) {
            পূর্ণসংখ্যা v = saturate_cast<পূর্ণসংখ্যা>(src[j]);
            s += v * v;
        }
        ফেরত s;
    }
};

template<>
কাঠামো NormDiffInf_SIMD<uchar, পূর্ণসংখ্যা> {
    পূর্ণসংখ্যা operator() (ধ্রুবক uchar* src1, ধ্রুবক uchar* src2, পূর্ণসংখ্যা n) ধ্রুবক {
        পূর্ণসংখ্যা j = 0;
        পূর্ণসংখ্যা s = 0;
        v_uint8 r0 = vx_setzero_u8(), r1 = vx_setzero_u8();
        v_uint8 r2 = vx_setzero_u8(), r3 = vx_setzero_u8();
        জন্য (; j <= n - 4 * VTraits<v_uint8>::vlanes(); j += 4 * VTraits<v_uint8>::vlanes()) {
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
        s = (পূর্ণসংখ্যা)v_reduce_max(v_max(v_max(v_max(r0, r1), r2), r3));
        জন্য (; j < n; j++) {
            পূর্ণসংখ্যা v = (পূর্ণসংখ্যা)cv_absdiff(src1[j], src2[j]);
            s = std::max(s, v);
        }
        ফেরত s;
    }
};

template<>
কাঠামো NormDiffInf_SIMD<schar, পূর্ণসংখ্যা> {
    পূর্ণসংখ্যা operator() (ধ্রুবক schar* src1, ধ্রুবক schar* src2, পূর্ণসংখ্যা n) ধ্রুবক {
        পূর্ণসংখ্যা j = 0;
        পূর্ণসংখ্যা s = 0;
        v_uint8 r0 = vx_setzero_u8(), r1 = vx_setzero_u8();
        v_uint8 r2 = vx_setzero_u8(), r3 = vx_setzero_u8();
        জন্য (; j <= n - 4 * VTraits<v_int8>::vlanes(); j += 4 * VTraits<v_int8>::vlanes()) {
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
        s = (পূর্ণসংখ্যা)v_reduce_max(v_max(v_max(v_max(r0, r1), r2), r3));
        জন্য (; j < n; j++) {
            পূর্ণসংখ্যা v = (পূর্ণসংখ্যা)cv_absdiff(src1[j], src2[j]);
            s = std::max(s, v);
        }
        ফেরত s;
    }
};

template<>
কাঠামো NormDiffInf_SIMD<ushort, পূর্ণসংখ্যা> {
    পূর্ণসংখ্যা operator() (ধ্রুবক ushort* src1, ধ্রুবক ushort* src2, পূর্ণসংখ্যা n) ধ্রুবক {
        পূর্ণসংখ্যা j = 0;
        পূর্ণসংখ্যা s = 0;
        v_uint16 r0 = vx_setzero_u16(), r1 = vx_setzero_u16();
        v_uint16 r2 = vx_setzero_u16(), r3 = vx_setzero_u16();
        জন্য (; j <= n - 4 * VTraits<v_uint16>::vlanes(); j += 4 * VTraits<v_uint16>::vlanes()) {
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
        s = (পূর্ণসংখ্যা)v_reduce_max(v_max(v_max(v_max(r0, r1), r2), r3));
        জন্য (; j < n; j++) {
            পূর্ণসংখ্যা v = (পূর্ণসংখ্যা)cv_absdiff(src1[j], src2[j]);
            s = std::max(s, v);
        }
        ফেরত s;
    }
};

template<>
কাঠামো NormDiffInf_SIMD<short, পূর্ণসংখ্যা> {
    পূর্ণসংখ্যা operator() (ধ্রুবক short* src1, ধ্রুবক short* src2, পূর্ণসংখ্যা n) ধ্রুবক {
        পূর্ণসংখ্যা j = 0;
        পূর্ণসংখ্যা s = 0;
        v_uint16 r0 = vx_setzero_u16(), r1 = vx_setzero_u16();
        v_uint16 r2 = vx_setzero_u16(), r3 = vx_setzero_u16();
        জন্য (; j <= n - 4 * VTraits<v_int16>::vlanes(); j += 4 * VTraits<v_int16>::vlanes()) {
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
        s = (পূর্ণসংখ্যা)v_reduce_max(v_max(v_max(v_max(r0, r1), r2), r3));
        জন্য (; j < n; j++) {
            পূর্ণসংখ্যা v = (পূর্ণসংখ্যা)cv_absdiff(src1[j], src2[j]);
            s = std::max(s, v);
        }
        ফেরত s;
    }
};

template<>
কাঠামো NormDiffInf_SIMD<পূর্ণসংখ্যা, unsigned> {
    unsigned operator() (ধ্রুবক পূর্ণসংখ্যা* src1, ধ্রুবক পূর্ণসংখ্যা* src2, পূর্ণসংখ্যা n) ধ্রুবক {
        পূর্ণসংখ্যা j = 0;
        unsigned s = 0;
        v_uint32 r0 = vx_setzero_u32(), r1 = vx_setzero_u32();
        v_uint32 r2 = vx_setzero_u32(), r3 = vx_setzero_u32();
        জন্য (; j <= n - 4 * VTraits<v_int32>::vlanes(); j += 4 * VTraits<v_int32>::vlanes()) {
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
        s = (পূর্ণসংখ্যা)v_reduce_max(v_max(v_max(v_max(r0, r1), r2), r3));
        জন্য (; j < n; j++) {
            unsigned v = (unsigned)cv_absdiff(src1[j], src2[j]);
            s = std::max(s, v);
        }
        ফেরত s;
    }
};

template<>
কাঠামো NormDiffInf_SIMD<দশমিক, দশমিক> {
    দশমিক operator() (ধ্রুবক দশমিক* src1, ধ্রুবক দশমিক* src2, পূর্ণসংখ্যা n) ধ্রুবক {
        পূর্ণসংখ্যা j = 0;
        দশমিক s = 0;
        v_float32 r0 = vx_setzero_f32(), r1 = vx_setzero_f32();
        জন্য (; j <= n - 2 * VTraits<v_float32>::vlanes(); j += 2 * VTraits<v_float32>::vlanes()) {
            v_float32 v01 = vx_load(src1 + j), v02 = vx_load(src2 + j);
            r0 = v_max(r0, v_absdiff(v01, v02));

            v_float32 v11 = vx_load(src1 + j + VTraits<v_float32>::vlanes()),
                      v12 = vx_load(src2 + j + VTraits<v_float32>::vlanes());
            r1 = v_max(r1, v_absdiff(v11, v12));
        }
        s = v_reduce_max(v_max(r0, r1));
        জন্য (; j < n; j++) {
            দশমিক v = (দশমিক)cv_absdiff(src1[j], src2[j]);
            s = std::max(s, v);
        }
        ফেরত s;
    }
};

template<>
কাঠামো NormDiffL1_SIMD<uchar, পূর্ণসংখ্যা> {
    পূর্ণসংখ্যা operator() (ধ্রুবক uchar* src1, ধ্রুবক uchar* src2, পূর্ণসংখ্যা n) ধ্রুবক {
        পূর্ণসংখ্যা j = 0;
        পূর্ণসংখ্যা s = 0;
        v_uint32 r0 = vx_setzero_u32(), r1 = vx_setzero_u32();
        v_uint8  one = vx_setall_u8(1);
        জন্য (; j<= n - 2 * VTraits<v_uint8>::vlanes(); j += 2 * VTraits<v_uint8>::vlanes()) {
            v_uint8 v01 = vx_load(src1 + j), v02 = vx_load(src2 + j);
            r0 = v_dotprod_expand_fast(v_absdiff(v01, v02), one, r0);

            v_uint8 v11 = vx_load(src1 + j + VTraits<v_uint8>::vlanes()),
                    v12 = vx_load(src2 + j + VTraits<v_uint8>::vlanes());
            r1 = v_dotprod_expand_fast(v_absdiff(v11, v12), one, r1);
        }
        s += v_reduce_যোগ(v_add(r0, r1));
        জন্য (; j < n; j++) {
            পূর্ণসংখ্যা v = (পূর্ণসংখ্যা)cv_absdiff(src1[j], src2[j]);
            s += v;
        }
        ফেরত s;
    }
};

template<>
কাঠামো NormDiffL1_SIMD<schar, পূর্ণসংখ্যা> {
    পূর্ণসংখ্যা operator() (ধ্রুবক schar* src1, ধ্রুবক schar* src2, পূর্ণসংখ্যা n) ধ্রুবক {
        পূর্ণসংখ্যা j = 0;
        পূর্ণসংখ্যা s = 0;
        v_uint32 r0 = vx_setzero_u32(), r1 = vx_setzero_u32();
        v_uint8  one = vx_setall_u8(1);
        জন্য (; j<= n - 2 * VTraits<v_int8>::vlanes(); j += 2 * VTraits<v_int8>::vlanes()) {
            v_int8 v01 = vx_load(src1 + j), v02 = vx_load(src2 + j);
            r0 = v_dotprod_expand_fast(v_absdiff(v01, v02), one, r0);

            v_int8 v11 = vx_load(src1 + j + VTraits<v_int8>::vlanes()),
                   v12 = vx_load(src2 + j + VTraits<v_int8>::vlanes());
            r1 = v_dotprod_expand_fast(v_absdiff(v11, v12), one, r1);
        }
        s += v_reduce_যোগ(v_add(r0, r1));
        জন্য (; j < n; j++) {
            পূর্ণসংখ্যা v = (পূর্ণসংখ্যা)cv_absdiff(src1[j], src2[j]);
            s += v;
        }
        ফেরত s;
    }
};

template<>
কাঠামো NormDiffL1_SIMD<ushort, পূর্ণসংখ্যা> {
    পূর্ণসংখ্যা operator() (ধ্রুবক ushort* src1, ধ্রুবক ushort* src2, পূর্ণসংখ্যা n) ধ্রুবক {
        পূর্ণসংখ্যা j = 0;
        পূর্ণসংখ্যা s = 0;
        v_uint32 r0 = vx_setzero_u32(), r1 = vx_setzero_u32();
        v_uint32 r2 = vx_setzero_u32(), r3 = vx_setzero_u32();
        জন্য (; j<= n - 4 * VTraits<v_uint16>::vlanes(); j += 4 * VTraits<v_uint16>::vlanes()) {
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
        s += (পূর্ণসংখ্যা)v_reduce_যোগ(v_add(v_add(v_add(r0, r1), r2), r3));
        জন্য (; j < n; j++) {
            পূর্ণসংখ্যা v = (পূর্ণসংখ্যা)cv_absdiff(src1[j], src2[j]);
            s += v;
        }
        ফেরত s;
    }
};

template<>
কাঠামো NormDiffL1_SIMD<short, পূর্ণসংখ্যা> {
    পূর্ণসংখ্যা operator() (ধ্রুবক short* src1, ধ্রুবক short* src2, পূর্ণসংখ্যা n) ধ্রুবক {
        পূর্ণসংখ্যা j = 0;
        পূর্ণসংখ্যা s = 0;
        v_uint32 r0 = vx_setzero_u32(), r1 = vx_setzero_u32();
        v_uint32 r2 = vx_setzero_u32(), r3 = vx_setzero_u32();
        জন্য (; j<= n - 4 * VTraits<v_int16>::vlanes(); j += 4 * VTraits<v_int16>::vlanes()) {
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
        s += (পূর্ণসংখ্যা)v_reduce_যোগ(v_add(v_add(v_add(r0, r1), r2), r3));
        জন্য (; j < n; j++) {
            পূর্ণসংখ্যা v = (পূর্ণসংখ্যা)cv_absdiff(src1[j], src2[j]);
            s += v;
        }
        ফেরত s;
    }
};

template<>
কাঠামো NormDiffL2_SIMD<uchar, পূর্ণসংখ্যা> {
    পূর্ণসংখ্যা operator() (ধ্রুবক uchar* src1, ধ্রুবক uchar* src2, পূর্ণসংখ্যা n) ধ্রুবক {
        পূর্ণসংখ্যা j = 0;
        পূর্ণসংখ্যা s = 0;
        v_uint32 r0 = vx_setzero_u32(), r1 = vx_setzero_u32();
        জন্য (; j <= n - 2 * VTraits<v_uint8>::vlanes(); j += 2 * VTraits<v_uint8>::vlanes()) {
            v_uint8 v01 = vx_load(src1 + j), v02 = vx_load(src2 + j);
            v_uint8 v0 = v_absdiff(v01, v02);
            r0 = v_dotprod_expand_fast(v0, v0, r0);

            v_uint8 v11 = vx_load(src1 + j + VTraits<v_uint8>::vlanes()),
                    v12 = vx_load(src2 + j + VTraits<v_uint8>::vlanes());
            v_uint8 v1 = v_absdiff(v11, v12);
            r1 = v_dotprod_expand_fast(v1, v1, r1);
        }
        s += v_reduce_যোগ(v_add(r0, r1));
        জন্য (; j < n; j++) {
            পূর্ণসংখ্যা v = (পূর্ণসংখ্যা)src1[j] - (পূর্ণসংখ্যা)src2[j];
            s += v * v;
        }
        ফেরত s;
    }
};

template<>
কাঠামো NormDiffL2_SIMD<schar, পূর্ণসংখ্যা> {
    পূর্ণসংখ্যা operator() (ধ্রুবক schar* src1, ধ্রুবক schar* src2, পূর্ণসংখ্যা n) ধ্রুবক {
        পূর্ণসংখ্যা j = 0;
        পূর্ণসংখ্যা s = 0;
        v_uint32 r0 = vx_setzero_u32(), r1 = vx_setzero_u32();
        জন্য (; j <= n - 2 * VTraits<v_int8>::vlanes(); j += 2 * VTraits<v_int8>::vlanes()) {
            v_int8 v01 = vx_load(src1 + j), v02 = vx_load(src2 + j);
            v_uint8 v0 = v_absdiff(v01, v02);
            r0 = v_dotprod_expand_fast(v0, v0, r0);

            v_int8 v11 = vx_load(src1 + j + VTraits<v_int8>::vlanes()),
                   v12 = vx_load(src2 + j + VTraits<v_int8>::vlanes());
            v_uint8 v1 = v_absdiff(v11, v12);
            r1 = v_dotprod_expand_fast(v1, v1, r1);
        }
        s += v_reduce_যোগ(v_add(r0, r1));
        জন্য (; j < n; j++) {
            পূর্ণসংখ্যা v = (পূর্ণসংখ্যা)src1[j] - (পূর্ণসংখ্যা)src2[j];
            s += v * v;
        }
        ফেরত s;
    }
};

#endif

#যদি (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)

template<>
কাঠামো NormInf_SIMD<দ্বিগুণ, দ্বিগুণ> {
    দ্বিগুণ operator() (ধ্রুবক দ্বিগুণ* src, পূর্ণসংখ্যা n) ধ্রুবক {
        পূর্ণসংখ্যা j = 0;
        দ্বিগুণ s = 0.f;
        v_float64 r0 = vx_setzero_f64(), r1 = vx_setzero_f64();
        v_float64 r2 = vx_setzero_f64(), r3 = vx_setzero_f64();
        জন্য (; j <= n - 4 * VTraits<v_float64>::vlanes(); j += 4 * VTraits<v_float64>::vlanes()) {
            r0 = v_max(r0, v_abs(vx_load(src + j                                   )));
            r1 = v_max(r1, v_abs(vx_load(src + j +     VTraits<v_float64>::vlanes())));
            r2 = v_max(r2, v_abs(vx_load(src + j + 2 * VTraits<v_float64>::vlanes())));
            r3 = v_max(r3, v_abs(vx_load(src + j + 3 * VTraits<v_float64>::vlanes())));
        }
        r0 = v_max(r0, v_max(r1, v_max(r2, r3)));
        জন্য (; j < n; j++) {
            s = std::max(s, cv_abs(src[j]));
        }
        // [TODO]: use v_reduce_max when it supports float64
        দ্বিগুণ t[VTraits<v_float64>::max_nlanes];
        vx_store(t, r0);
        জন্য (পূর্ণসংখ্যা i = 0; i < VTraits<v_float64>::vlanes(); i++) {
            s = std::max(s, cv_abs(t[i]));
        }
        ফেরত s;
    }
};

template<>
কাঠামো NormL1_SIMD<পূর্ণসংখ্যা, দ্বিগুণ> {
    দ্বিগুণ operator() (ধ্রুবক পূর্ণসংখ্যা* src, পূর্ণসংখ্যা n) ধ্রুবক {
        পূর্ণসংখ্যা j = 0;
        দ্বিগুণ s = 0.f;
        v_float64 r00 = vx_setzero_f64(), r01 = vx_setzero_f64();
        v_float64 r10 = vx_setzero_f64(), r11 = vx_setzero_f64();
        জন্য (; j <= n - 2 * VTraits<v_int32>::vlanes(); j += 2 * VTraits<v_int32>::vlanes()) {
            // Widen to double before taking the magnitude: exact over the whole int32 range,
            // INT_MIN included, which a float intermediate would not be above 2^24.
            v_int32 i0 = vx_load(src + j), i1 = vx_load(src + j + VTraits<v_int32>::vlanes());
            r00 = v_add(r00, v_abs(v_cvt_f64(i0))); r01 = v_add(r01, v_abs(v_cvt_f64_উপরের(i0)));
            r10 = v_add(r10, v_abs(v_cvt_f64(i1))); r11 = v_add(r11, v_abs(v_cvt_f64_উপরের(i1)));
        }
        s += v_reduce_যোগ(v_add(v_add(v_add(r00, r01), r10), r11));
        জন্য (; j < n; j++) {
            s += cv_abs(src[j]);
        }
        ফেরত s;
    }
};

template<>
কাঠামো NormL1_SIMD<দশমিক, দ্বিগুণ> {
    দ্বিগুণ operator() (ধ্রুবক দশমিক* src, পূর্ণসংখ্যা n) ধ্রুবক {
        পূর্ণসংখ্যা j = 0;
        দ্বিগুণ s = 0.f;
        v_float64 r00 = vx_setzero_f64(), r01 = vx_setzero_f64();
        v_float64 r10 = vx_setzero_f64(), r11 = vx_setzero_f64();
        v_float64 r20 = vx_setzero_f64(), r21 = vx_setzero_f64();
        v_float64 r30 = vx_setzero_f64(), r31 = vx_setzero_f64();
        জন্য (; j <= n - 4 * VTraits<v_float32>::vlanes(); j += 4 * VTraits<v_float32>::vlanes()) {
            v_float32 v0 = v_abs(vx_load(src + j)), v1 = v_abs(vx_load(src + j + VTraits<v_float32>::vlanes()));
            r00 = v_add(r00, v_cvt_f64(v0)); r01 = v_add(r01, v_cvt_f64_উপরের(v0));
            r10 = v_add(r10, v_cvt_f64(v1)); r11 = v_add(r11, v_cvt_f64_উপরের(v1));

            v_float32 v2 = v_abs(vx_load(src + j + 2 * VTraits<v_float32>::vlanes())), v3 = v_abs(vx_load(src + j + 3 * VTraits<v_float32>::vlanes()));
            r20 = v_add(r20, v_cvt_f64(v2)); r21 = v_add(r21, v_cvt_f64_উপরের(v2));
            r30 = v_add(r30, v_cvt_f64(v3)); r31 = v_add(r31, v_cvt_f64_উপরের(v3));
        }
        s += v_reduce_যোগ(v_add(v_add(v_add(r00, r01), r10), r11));
        s += v_reduce_যোগ(v_add(v_add(v_add(r20, r21), r30), r31));
        জন্য (; j < n; j++) {
            s += cv_abs(src[j]);
        }
        ফেরত s;
    }
};

template<>
কাঠামো NormL2_SIMD<ushort, দ্বিগুণ> {
    দ্বিগুণ operator() (ধ্রুবক ushort* src, পূর্ণসংখ্যা n) ধ্রুবক {
        পূর্ণসংখ্যা j = 0;
        দ্বিগুণ s = 0.f;
        v_float64 r0 = vx_setzero_f64(), r1 = vx_setzero_f64();
        জন্য (; j <= n - 2 * VTraits<v_uint16>::vlanes(); j += 2 * VTraits<v_uint16>::vlanes()) {
            v_uint16 v0 = vx_load(src + j);
            v_uint64 u0 = v_dotprod_expand_fast(v0, v0);
            r0 = v_add(r0, v_cvt_f64(v_reinterpret_as_s64(u0)));

            v_uint16 v1 = vx_load(src + j + VTraits<v_uint16>::vlanes());
            v_uint64 u1 = v_dotprod_expand_fast(v1, v1);
            r1 = v_add(r1, v_cvt_f64(v_reinterpret_as_s64(u1)));
        }
        s += v_reduce_যোগ(v_add(r0, r1));
        জন্য (; j < n; j++) {
            দ্বিগুণ v = saturate_cast<দ্বিগুণ>(src[j]);
            s += v * v;
        }
        ফেরত s;
    }
};

template<>
কাঠামো NormL2_SIMD<short, দ্বিগুণ> {
    দ্বিগুণ operator() (ধ্রুবক short* src, পূর্ণসংখ্যা n) ধ্রুবক {
        পূর্ণসংখ্যা j = 0;
        দ্বিগুণ s = 0.f;
        v_float64 r0 = vx_setzero_f64(), r1 = vx_setzero_f64();
        জন্য (; j <= n - 2 * VTraits<v_int16>::vlanes(); j += 2 * VTraits<v_int16>::vlanes()) {
            v_int16 v0 = vx_load(src + j);
            r0 = v_add(r0, v_cvt_f64(v_dotprod_expand_fast(v0, v0)));

            v_int16 v1 = vx_load(src + j + VTraits<v_int16>::vlanes());
            r1 = v_add(r1, v_cvt_f64(v_dotprod_expand_fast(v1, v1)));
        }
        s += v_reduce_যোগ(v_add(r0, r1));
        জন্য (; j < n; j++) {
            দ্বিগুণ v = saturate_cast<দ্বিগুণ>(src[j]);
            s += v * v;
        }
        ফেরত s;
    }
};

template<>
কাঠামো NormL2_SIMD<পূর্ণসংখ্যা, দ্বিগুণ> {
    দ্বিগুণ operator() (ধ্রুবক পূর্ণসংখ্যা* src, পূর্ণসংখ্যা n) ধ্রুবক {
        পূর্ণসংখ্যা j = 0;
        দ্বিগুণ s = 0.f;
        v_float64 r0 = vx_setzero_f64(), r1 = vx_setzero_f64();
        জন্য (; j <= n - 2 * VTraits<v_int32>::vlanes(); j += 2 * VTraits<v_int32>::vlanes()) {
            v_int32 v0 = vx_load(src + j);
            r0 = v_dotprod_expand_fast(v0, v0, r0);

            v_int32 v1 = vx_load(src + j + VTraits<v_int32>::vlanes());
            r1 = v_dotprod_expand_fast(v1, v1, r1);
        }
        s += v_reduce_যোগ(v_add(r0, r1));
        জন্য (; j < n; j++) {
            দ্বিগুণ v = src[j];
            s += v * v;
        }
        ফেরত s;
    }
};

template<>
কাঠামো NormL2_SIMD<দশমিক, দ্বিগুণ> {
    দ্বিগুণ operator() (ধ্রুবক দশমিক* src, পূর্ণসংখ্যা n) ধ্রুবক {
        পূর্ণসংখ্যা j = 0;
        দ্বিগুণ s = 0.f;
        v_float64 r00 = vx_setzero_f64(), r01 = vx_setzero_f64();
        v_float64 r10 = vx_setzero_f64(), r11 = vx_setzero_f64();
        জন্য (; j <= n - 2 * VTraits<v_float32>::vlanes(); j += 2 * VTraits<v_float32>::vlanes()) {
            v_float32 v0 = vx_load(src + j), v1 = vx_load(src + j + VTraits<v_float32>::vlanes());
            v_float64 v00 = v_cvt_f64(v0), v01 = v_cvt_f64_উপরের(v0);
            v_float64 v10 = v_cvt_f64(v1), v11 = v_cvt_f64_উপরের(v1);
            r00 = v_fma(v00, v00, r00); r01 = v_fma(v01, v01, r01);
            r10 = v_fma(v10, v10, r10); r11 = v_fma(v11, v11, r11);
        }
        s += v_reduce_যোগ(v_add(v_add(v_add(r00, r01), r10), r11));
        জন্য (; j < n; j++) {
            দ্বিগুণ v = src[j];
            s += v * v;
        }
        ফেরত s;
    }
};

#endif

#যদি CV_SIMD_64F // CV_SIMD_SCALABLE_64F has accuracy problem with the following kernels on ci

template<>
কাঠামো NormL1_SIMD<দ্বিগুণ, দ্বিগুণ> {
    দ্বিগুণ operator() (ধ্রুবক দ্বিগুণ* src, পূর্ণসংখ্যা n) ধ্রুবক {
        পূর্ণসংখ্যা j = 0;
        দ্বিগুণ s = 0.f;
        v_float64 r00 = vx_setzero_f64(), r01 = vx_setzero_f64();
        v_float64 r10 = vx_setzero_f64(), r11 = vx_setzero_f64();
        জন্য (; j <= n - 4 * VTraits<v_float64>::vlanes(); j += 4 * VTraits<v_float64>::vlanes()) {
            r00 = v_add(r00, v_abs(vx_load(src + j                                   )));
            r01 = v_add(r01, v_abs(vx_load(src + j +     VTraits<v_float64>::vlanes())));
            r10 = v_add(r10, v_abs(vx_load(src + j + 2 * VTraits<v_float64>::vlanes())));
            r11 = v_add(r11, v_abs(vx_load(src + j + 3 * VTraits<v_float64>::vlanes())));
        }
        s += v_reduce_যোগ(v_add(v_add(v_add(r00, r01), r10), r11));
        জন্য (; j < n; j++) {
            s += cv_abs(src[j]);
        }
        ফেরত s;
    }
};

template<>
কাঠামো NormL2_SIMD<দ্বিগুণ, দ্বিগুণ> {
    দ্বিগুণ operator() (ধ্রুবক দ্বিগুণ* src, পূর্ণসংখ্যা n) ধ্রুবক {
        পূর্ণসংখ্যা j = 0;
        দ্বিগুণ s = 0.f;
        v_float64 r00 = vx_setzero_f64(), r01 = vx_setzero_f64();
        v_float64 r10 = vx_setzero_f64(), r11 = vx_setzero_f64();
        জন্য (; j <= n - 4 * VTraits<v_float64>::vlanes(); j += 4 * VTraits<v_float64>::vlanes()) {
            v_float64 v00 = vx_load(src + j                                   );
            v_float64 v01 = vx_load(src + j +     VTraits<v_float64>::vlanes());
            v_float64 v10 = vx_load(src + j + 2 * VTraits<v_float64>::vlanes());
            v_float64 v11 = vx_load(src + j + 3 * VTraits<v_float64>::vlanes());
            r00 = v_fma(v00, v00, r00); r01 = v_fma(v01, v01, r01);
            r10 = v_fma(v10, v10, r10); r11 = v_fma(v11, v11, r11);
        }
        s += v_reduce_যোগ(v_add(v_add(v_add(r00, r01), r10), r11));
        জন্য (; j < n; j++) {
            দ্বিগুণ v = src[j];
            s += v * v;
        }
        ফেরত s;
    }
};

template<>
কাঠামো NormDiffInf_SIMD<দ্বিগুণ, দ্বিগুণ> {
    দ্বিগুণ operator() (ধ্রুবক দ্বিগুণ* src1, ধ্রুবক দ্বিগুণ* src2, পূর্ণসংখ্যা n) ধ্রুবক {
        পূর্ণসংখ্যা j = 0;
        দ্বিগুণ s = 0;
        v_float64 r0 = vx_setzero_f64(), r1 = vx_setzero_f64();
        জন্য (; j <= n - 2 * VTraits<v_float64>::vlanes(); j += 2 * VTraits<v_float64>::vlanes()) {
            v_float64 v01 = vx_load(src1 + j), v02 = vx_load(src2 + j);
            r0 = v_max(r0, v_absdiff(v01, v02));

            v_float64 v11 = vx_load(src1 + j + VTraits<v_float64>::vlanes()),
                      v12 = vx_load(src2 + j + VTraits<v_float64>::vlanes());
            r1 = v_max(r1, v_absdiff(v11, v12));
        }
        // [TODO]: use v_reduce_max when it supports float64
        দ্বিগুণ t[VTraits<v_float64>::max_nlanes];
        vx_store(t, v_max(r0, r1));
        জন্য (পূর্ণসংখ্যা i = 0; i < VTraits<v_float64>::vlanes(); i++) {
            s = std::max(s, t[i]);
        }
        জন্য (; j < n; j++) {
            দ্বিগুণ v = (দ্বিগুণ)cv_absdiff(src1[j], src2[j]);
            s = std::max(s, v);
        }
        ফেরত s;
    }
};

template<>
কাঠামো NormDiffL1_SIMD<দশমিক, দ্বিগুণ> {
    দ্বিগুণ operator() (ধ্রুবক দশমিক* src1, ধ্রুবক দশমিক* src2, পূর্ণসংখ্যা n) ধ্রুবক {
        পূর্ণসংখ্যা j = 0;
        দ্বিগুণ s = 0.f;
        v_float64 r0 = vx_setzero_f64(), r1 = vx_setzero_f64();
        v_float64 r2 = vx_setzero_f64(), r3 = vx_setzero_f64();
        জন্য (; j <= n - 2 * VTraits<v_float32>::vlanes(); j += 2 * VTraits<v_float32>::vlanes()) {
            v_float32 v01 = vx_load(src1 + j), v02 = vx_load(src2 + j);
            v_float32 v0 = v_absdiff(v01, v02);
            r0 = v_add(r0, v_cvt_f64(v0)); r1 = v_add(r1, v_cvt_f64_উপরের(v0));

            v_float32 v11 = vx_load(src1 + j + VTraits<v_float32>::vlanes()),
                      v12 = vx_load(src2 + j + VTraits<v_float32>::vlanes());
            v_float32 v1 = v_absdiff(v11, v12);
            r2 = v_add(r2, v_cvt_f64(v1)); r3 = v_add(r3, v_cvt_f64_উপরের(v1));
        }
        s += v_reduce_যোগ(v_add(v_add(v_add(r0, r1), r2), r3));
        জন্য (; j < n; j++) {
            দ্বিগুণ v = (দ্বিগুণ)cv_absdiff(src1[j], src2[j]);
            s += v;
        }
        ফেরত s;
    }
};

template<>
কাঠামো NormDiffL1_SIMD<দ্বিগুণ, দ্বিগুণ> {
    দ্বিগুণ operator() (ধ্রুবক দ্বিগুণ* src1, ধ্রুবক দ্বিগুণ* src2, পূর্ণসংখ্যা n) ধ্রুবক {
        পূর্ণসংখ্যা j = 0;
        দ্বিগুণ s = 0.f;
        v_float64 r0 = vx_setzero_f64(), r1 = vx_setzero_f64();
        জন্য (; j <= n - 2 * VTraits<v_float64>::vlanes(); j += 2 * VTraits<v_float64>::vlanes()) {
            v_float64 v01 = vx_load(src1 + j), v02 = vx_load(src2 + j);
            r0 = v_add(r0, v_absdiff(v01, v02));

            v_float64 v11 = vx_load(src1 + j + VTraits<v_float64>::vlanes()),
                      v12 = vx_load(src2 + j + VTraits<v_float64>::vlanes());
            r1 = v_add(r1, v_absdiff(v11, v12));
        }
        s += v_reduce_যোগ(v_add(r0, r1));
        জন্য (; j < n; j++) {
            দ্বিগুণ v = (দ্বিগুণ)cv_absdiff(src1[j], src2[j]);
            s += v;
        }
        ফেরত s;
    }
};

template<>
কাঠামো NormDiffL2_SIMD<ushort, দ্বিগুণ> {
    দ্বিগুণ operator() (ধ্রুবক ushort* src1, ধ্রুবক ushort* src2, পূর্ণসংখ্যা n) ধ্রুবক {
        পূর্ণসংখ্যা j = 0;
        দ্বিগুণ s = 0.f;
        v_float64 r0 = vx_setzero_f64(), r1 = vx_setzero_f64();
        জন্য (; j <= n - 2 * VTraits<v_uint16>::vlanes(); j += 2 * VTraits<v_uint16>::vlanes()) {
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
        s += v_reduce_যোগ(v_add(r0, r1));
        জন্য (; j < n; j++) {
            দ্বিগুণ v = (দ্বিগুণ)src1[j] - (দ্বিগুণ)src2[j];
            s += v * v;
        }
        ফেরত s;
    }
};

template<>
কাঠামো NormDiffL2_SIMD<short, দ্বিগুণ> {
    দ্বিগুণ operator() (ধ্রুবক short* src1, ধ্রুবক short* src2, পূর্ণসংখ্যা n) ধ্রুবক {
        পূর্ণসংখ্যা j = 0;
        দ্বিগুণ s = 0.f;
        v_float64 r0 = vx_setzero_f64(), r1 = vx_setzero_f64();
        জন্য (; j <= n - 2 * VTraits<v_int16>::vlanes(); j += 2 * VTraits<v_int16>::vlanes()) {
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
        s += v_reduce_যোগ(v_add(r0, r1));
        জন্য (; j < n; j++) {
            দ্বিগুণ v = (দ্বিগুণ)src1[j] - (দ্বিগুণ)src2[j];
            s += v * v;
        }
        ফেরত s;
    }
};

template<>
কাঠামো NormDiffL2_SIMD<পূর্ণসংখ্যা, দ্বিগুণ> {
    দ্বিগুণ operator() (ধ্রুবক পূর্ণসংখ্যা* src1, ধ্রুবক পূর্ণসংখ্যা* src2, পূর্ণসংখ্যা n) ধ্রুবক {
        পূর্ণসংখ্যা j = 0;
        দ্বিগুণ s = 0.f;
        v_float64 r0 = vx_setzero_f64(), r1 = vx_setzero_f64();
        জন্য (; j <= n - VTraits<v_int32>::vlanes(); j += VTraits<v_int32>::vlanes()) {
            v_int32 v01 = vx_load(src1 + j), v02 = vx_load(src2 + j);
            v_uint32 v0 = v_absdiff(v01, v02);
            v_uint64 ev0, ev1;
            v_expand(v0, ev0, ev1);
            v_float64 f0 = v_cvt_f64(v_reinterpret_as_s64(ev0)), f1 = v_cvt_f64(v_reinterpret_as_s64(ev1));
            r0 = v_fma(f0, f0, r0); r1 = v_fma(f1, f1, r1);
        }
        s += v_reduce_যোগ(v_add(r0, r1));
        জন্য (; j < n; j++) {
            দ্বিগুণ v = (দ্বিগুণ)src1[j] - (দ্বিগুণ)src2[j];
            s += v * v;
        }
        ফেরত s;
    }
};

template<>
কাঠামো NormDiffL2_SIMD<দশমিক, দ্বিগুণ> {
    দ্বিগুণ operator() (ধ্রুবক দশমিক* src1, ধ্রুবক দশমিক* src2, পূর্ণসংখ্যা n) ধ্রুবক {
        পূর্ণসংখ্যা j = 0;
        দ্বিগুণ s = 0.f;
        v_float64 r0 = vx_setzero_f64(), r1 = vx_setzero_f64();
        v_float64 r2 = vx_setzero_f64(), r3 = vx_setzero_f64();
        জন্য (; j <= n - 2 * VTraits<v_float32>::vlanes(); j += 2 * VTraits<v_float32>::vlanes()) {
            v_float32 v01 = vx_load(src1 + j), v02 = vx_load(src2 + j);
            v_float32 v0 = v_absdiff(v01, v02);
            v_float64 f01 = v_cvt_f64(v0), f02 = v_cvt_f64_উপরের(v0);
            r0 = v_fma(f01, f01, r0); r1 = v_fma(f02, f02, r1);

            v_float32 v11 = vx_load(src1 + j + VTraits<v_float32>::vlanes()),
                      v12 = vx_load(src2 + j + VTraits<v_float32>::vlanes());
            v_float32 v1 = v_absdiff(v11, v12);
            v_float64 f11 = v_cvt_f64(v1), f12 = v_cvt_f64_উপরের(v1);
            r2 = v_fma(f11, f11, r2); r3 = v_fma(f12, f12, r3);
        }
        s += v_reduce_যোগ(v_add(v_add(v_add(r0, r1), r2), r3));
        জন্য (; j < n; j++) {
            দ্বিগুণ v = (দ্বিগুণ)src1[j] - (দ্বিগুণ)src2[j];
            s += v * v;
        }
        ফেরত s;
    }
};

template<>
কাঠামো NormDiffL2_SIMD<দ্বিগুণ, দ্বিগুণ> {
    দ্বিগুণ operator() (ধ্রুবক দ্বিগুণ* src1, ধ্রুবক দ্বিগুণ* src2, পূর্ণসংখ্যা n) ধ্রুবক {
        পূর্ণসংখ্যা j = 0;
        দ্বিগুণ s = 0.f;
        v_float64 r0 = vx_setzero_f64(), r1 = vx_setzero_f64();
        v_float64 r2 = vx_setzero_f64(), r3 = vx_setzero_f64();
        জন্য (; j <= n - 4 * VTraits<v_float64>::vlanes(); j += 4 * VTraits<v_float64>::vlanes()) {
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
        s += v_reduce_যোগ(v_add(v_add(v_add(r0, r1), r2), r3));
        জন্য (; j < n; j++) {
            দ্বিগুণ v = (দ্বিগুণ)src1[j] - (দ্বিগুণ)src2[j];
            s += v * v;
        }
        ফেরত s;
    }
};

#endif

template <typename T, typename ST>
কাঠামো MaskedNormInf_SIMD {
    অন্তঃরেখ ST operator() (ধ্রুবক T* src, ধ্রুবক uchar* mask, পূর্ণসংখ্যা len, পূর্ণসংখ্যা cn) ধ্রুবক {
        ST s = 0;
        যদি (cn == 1) {
            জন্য (পূর্ণসংখ্যা i = 0; i < len; i++) {
                যদি (mask[i]) {
                    s = std::max(s, (ST)cv_abs(src[i]));
                }
            }
        }
        অন্যথা {
            জন্য (পূর্ণসংখ্যা i = 0; i < len; i++) {
                যদি (mask[i]) {
                    ধ্রুবক T* elem = src + i * cn;
                    পূর্ণসংখ্যা k = 0;
                #যদি CV_ENABLE_UNROLLED && !CV_SIMD_SCALABLE
                    জন্য (; k <= cn - 4; k += 4) {
                        s = std::max(s, (ST)cv_abs(elem[k]));
                        s = std::max(s, (ST)cv_abs(elem[k + 1]));
                        s = std::max(s, (ST)cv_abs(elem[k + 2]));
                        s = std::max(s, (ST)cv_abs(elem[k + 3]));
                    }
                #endif
                    জন্য (; k < cn; k++) {
                        s = std::max(s, (ST)cv_abs(elem[k]));
                    }
                }
            }
        }
        ফেরত s;
    }
};

template <typename T, typename ST>
কাঠামো MaskedNormL1_SIMD {
    অন্তঃরেখ ST operator() (ধ্রুবক T* src, ধ্রুবক uchar* mask, পূর্ণসংখ্যা len, পূর্ণসংখ্যা cn) ধ্রুবক {
        ST s = 0;
        যদি (cn == 1) {
            জন্য (পূর্ণসংখ্যা i = 0; i < len; i++) {
                যদি (mask[i]) {
                    s += (ST)cv_abs(src[i]);
                }
            }
        }
        অন্যথা {
            জন্য (পূর্ণসংখ্যা i = 0; i < len; i++) {
                যদি (mask[i]) {
                    ধ্রুবক T* elem = src + i * cn;
                    পূর্ণসংখ্যা k = 0;
                #যদি CV_ENABLE_UNROLLED && !CV_SIMD_SCALABLE
                    জন্য (; k <= cn - 4; k += 4) {
                        s += (ST)cv_abs(elem[k]);
                        s += (ST)cv_abs(elem[k + 1]);
                        s += (ST)cv_abs(elem[k + 2]);
                        s += (ST)cv_abs(elem[k + 3]);
                    }
                #endif
                    জন্য (; k < cn; k++) {
                        s += (ST)cv_abs(elem[k]);
                    }
                }
            }
        }
        ফেরত s;
    }
};

template <typename T, typename ST>
কাঠামো MaskedNormL2_SIMD {
    অন্তঃরেখ ST operator() (ধ্রুবক T* src, ধ্রুবক uchar* mask, পূর্ণসংখ্যা len, পূর্ণসংখ্যা cn) ধ্রুবক {
        ST s = 0;
        যদি (cn == 1) {
            পূর্ণসংখ্যা i = 0;
        #যদি CV_ENABLE_UNROLLED && !CV_SIMD_SCALABLE
            জন্য (; i <= len - 4; i += 4) {
                যদি (mask[i])     { T v0 = src[i];     s += (ST)v0 * v0; }
                যদি (mask[i + 1]) { T v1 = src[i + 1]; s += (ST)v1 * v1; }
                যদি (mask[i + 2]) { T v2 = src[i + 2]; s += (ST)v2 * v2; }
                যদি (mask[i + 3]) { T v3 = src[i + 3]; s += (ST)v3 * v3; }
            }
        #endif
            জন্য (; i < len; i++) {
                যদি (mask[i]) {
                    T v = src[i];
                    s += (ST)v * v;
                }
            }
        }
        অন্যথা {
            জন্য (পূর্ণসংখ্যা i = 0; i < len; i++) {
                যদি (mask[i]) {
                    ধ্রুবক T* elem = src + i * cn;
                    পূর্ণসংখ্যা k = 0;
                #যদি CV_ENABLE_UNROLLED && !CV_SIMD_SCALABLE
                    জন্য (; k <= cn - 4; k += 4) {
                        T v0 = elem[k];     s += (ST)v0 * v0;
                        T v1 = elem[k + 1]; s += (ST)v1 * v1;
                        T v2 = elem[k + 2]; s += (ST)v2 * v2;
                        T v3 = elem[k + 3]; s += (ST)v3 * v3;
                    }
                #endif
                    জন্য (; k < cn; k++) {
                        T v = elem[k];
                        s += (ST)v * v;
                    }
                }
            }
        }

        ফেরত s;
    }
};

#যদি (CV_SIMD || CV_SIMD_SCALABLE)
স্থির অন্তঃরেখ v_uint8  v_normmask_u8 (ধ্রুবক uchar* m) { ফেরত v_gt(vx_load(m),          vx_setzero_u8());  }
স্থির অন্তঃরেখ v_uint16 v_normmask_u16(ধ্রুবক uchar* m) { ফেরত v_gt(vx_load_expand(m),   vx_setzero_u16()); }
স্থির অন্তঃরেখ v_uint32 v_normmask_u32(ধ্রুবক uchar* m) { ফেরত v_gt(vx_load_expand_q(m), vx_setzero_u32()); }
স্থির অন্তঃরেখ শূন্য v_normmask_u64(ধ্রুবক uchar* m, v_uint64& m0, v_uint64& m1) {
    v_uint64 e0, e1;
    v_expand(v_normmask_u32(m), e0, e1);
    m0 = v_or(e0, v_shl<32>(e0));
    m1 = v_or(e1, v_shl<32>(e1));
}

template <>
কাঠামো MaskedNormInf_SIMD<দশমিক, দশমিক> {
    অন্তঃরেখ দশমিক operator()(ধ্রুবক দশমিক* src, ধ্রুবক uchar* mask, পূর্ণসংখ্যা len, পূর্ণসংখ্যা cn) ধ্রুবক {
        দশমিক ফলাফল = 0.0f;
        যদি (cn == 1) {
            পূর্ণসংখ্যা i = 0;
            ধ্রুবক পূর্ণসংখ্যা vstep = VTraits<v_float32>::vlanes();
            v_float32 acc = vx_setzero_f32();

            জন্য (; i <= len - vstep; i += vstep) {
                v_uint32 m = vx_load_expand_q(mask + i);
                v_uint32 cmp = v_gt(m, vx_setzero_u32());
                v_float32 s = vx_load(src + i);
                s = v_abs(s);
                s = v_reinterpret_as_f32(v_and(v_reinterpret_as_u32(s), cmp));
                acc = v_max(acc, s);
            }
            ফলাফল = v_reduce_max(acc);

            জন্য (; i < len; i++) {
                যদি (mask[i])
                    ফলাফল = std::max(ফলাফল, std::abs(src[i]));
            }
        }
        অন্যথা {
            জন্য (পূর্ণসংখ্যা i = 0; i < len; i++) {
                যদি (mask[i]) {
                    ধ্রুবক দশমিক* elem = src + i * cn;
                    পূর্ণসংখ্যা k = 0;
                    ধ্রুবক পূর্ণসংখ্যা vstep = VTraits<v_float32>::vlanes();
                    v_float32 acc = vx_setzero_f32();

                    জন্য (; k <= cn - vstep; k += vstep) {
                        v_float32 s = vx_load(elem + k);
                        acc = v_max(acc, v_abs(s));
                    }

                    ফলাফল = std::max(ফলাফল, v_reduce_max(acc));

                    জন্য (; k < cn; k++)
                        ফলাফল = std::max(ফলাফল, std::abs(elem[k]));
                }
            }
        }
        ফেরত ফলাফল;
    }
};

#যদি CV_SIMD_64F
template <>
কাঠামো MaskedNormL1_SIMD<দশমিক, দ্বিগুণ> {
    অন্তঃরেখ দ্বিগুণ operator()(ধ্রুবক দশমিক* src, ধ্রুবক uchar* mask, পূর্ণসংখ্যা len, পূর্ণসংখ্যা cn) ধ্রুবক {
        দ্বিগুণ ফলাফল = 0.0;
        যদি (cn == 1) {
            পূর্ণসংখ্যা i = 0;
            ধ্রুবক পূর্ণসংখ্যা vstep = VTraits<v_float32>::vlanes();
            v_float64 acc0 = vx_setzero_f64(), acc1 = vx_setzero_f64();
            v_float64 acc2 = vx_setzero_f64(), acc3 = vx_setzero_f64();

            জন্য (; i <= len - 2*vstep; i += 2*vstep) {
                v_float32 s0 = v_and(v_abs(vx_load(src + i)),
                                     v_reinterpret_as_f32(v_normmask_u32(mask + i)));
                v_float32 s1 = v_and(v_abs(vx_load(src + i + vstep)),
                                     v_reinterpret_as_f32(v_normmask_u32(mask + i + vstep)));
                acc0 = v_add(acc0, v_cvt_f64(s0));
                acc1 = v_add(acc1, v_cvt_f64_উপরের(s0));
                acc2 = v_add(acc2, v_cvt_f64(s1));
                acc3 = v_add(acc3, v_cvt_f64_উপরের(s1));
            }
            জন্য (; i <= len - vstep; i += vstep) {
                v_float32 s = v_and(v_abs(vx_load(src + i)),
                                    v_reinterpret_as_f32(v_normmask_u32(mask + i)));
                acc0 = v_add(acc0, v_cvt_f64(s));
                acc1 = v_add(acc1, v_cvt_f64_উপরের(s));
            }
            ফলাফল = v_reduce_যোগ(v_add(v_add(acc0, acc1), v_add(acc2, acc3)));

            জন্য (; i < len; i++) {
                যদি (mask[i])
                    ফলাফল += std::abs(src[i]);
            }
        }
        অন্যথা {
            জন্য (পূর্ণসংখ্যা i = 0; i < len; i++) {
                যদি (mask[i]) {
                    ধ্রুবক দশমিক* elem = src + i * cn;
                    পূর্ণসংখ্যা k = 0;
                    ধ্রুবক পূর্ণসংখ্যা vstep = VTraits<v_float32>::vlanes();
                    v_float64 acc = vx_setzero_f64();

                    জন্য (; k <= cn - vstep; k += vstep) {
                        v_float32 s = v_abs(vx_load(elem + k));
                        acc = v_add(acc, v_cvt_f64(s));
                        acc = v_add(acc, v_cvt_f64_উপরের(s));
                    }

                    ফলাফল += v_reduce_যোগ(acc);

                    জন্য (; k < cn; k++)
                        ফলাফল += std::abs(elem[k]);
                }
            }
        }
        ফেরত ফলাফল;
    }
};

template <>
কাঠামো MaskedNormL2_SIMD<দশমিক, দ্বিগুণ> {
    অন্তঃরেখ দ্বিগুণ operator()(ধ্রুবক দশমিক* src, ধ্রুবক uchar* mask, পূর্ণসংখ্যা len, পূর্ণসংখ্যা cn) ধ্রুবক {
        দ্বিগুণ ফলাফল = 0.0;
        যদি (cn == 1) {
            পূর্ণসংখ্যা i = 0;
            ধ্রুবক পূর্ণসংখ্যা vstep = VTraits<v_float32>::vlanes();
            v_float32 facc = vx_setzero_f32();
            v_float64 dacc = vx_setzero_f64();
            পূর্ণসংখ্যা flush = 0;

            জন্য (; i <= len - vstep; i += vstep, flush += vstep) {
                যদি (flush >= 64) {
                    dacc = v_add(dacc, v_cvt_f64(facc));
                    dacc = v_add(dacc, v_cvt_f64_উপরের(facc));
                    facc = vx_setzero_f32();
                    flush = 0;
                }
                v_uint32 cmp = v_gt(vx_load_expand_q(mask + i), vx_setzero_u32());
                v_float32 s  = v_reinterpret_as_f32(v_and(v_reinterpret_as_u32(vx_load(src + i)), cmp));
                facc = v_add(facc, v_mul(s, s));
            }

            dacc = v_add(dacc, v_cvt_f64(facc));
            dacc = v_add(dacc, v_cvt_f64_উপরের(facc));
            ফলাফল = v_reduce_যোগ(dacc);

            জন্য (; i < len; i++) {
                যদি (mask[i]) {
                    দ্বিগুণ v = src[i];
                    ফলাফল += v * v;
                }
            }
        }
        অন্যথা {
            জন্য (পূর্ণসংখ্যা i = 0; i < len; i++) {
                যদি (mask[i]) {
                    ধ্রুবক দশমিক* elem = src + i * cn;
                    পূর্ণসংখ্যা k = 0;
                    ধ্রুবক পূর্ণসংখ্যা vstep = VTraits<v_float32>::vlanes();
                    v_float32 facc = vx_setzero_f32();

                    জন্য (; k <= cn - vstep; k += vstep) {
                        v_float32 s = vx_load(elem + k);
                        facc = v_add(facc, v_mul(s, s));
                    }

                    v_float64 dacc = v_add(v_cvt_f64(facc), v_cvt_f64_উপরের(facc));
                    ফলাফল += v_reduce_যোগ(dacc);

                    জন্য (; k < cn; k++) {
                        দ্বিগুণ v = elem[k];
                        ফলাফল += v * v;
                    }
                }
            }
        }
        ফেরত ফলাফল;
    }
};

#endif

template <>
কাঠামো MaskedNormInf_SIMD<uchar, পূর্ণসংখ্যা> {
    অন্তঃরেখ পূর্ণসংখ্যা operator()(ধ্রুবক uchar* src, ধ্রুবক uchar* mask, পূর্ণসংখ্যা len, পূর্ণসংখ্যা cn) ধ্রুবক {
        পূর্ণসংখ্যা ফলাফল = 0;
        যদি (cn == 1) {
            পূর্ণসংখ্যা i = 0;
            ধ্রুবক পূর্ণসংখ্যা vstep = VTraits<v_uint8>::vlanes();
            v_uint8 acc = vx_setzero_u8();

            জন্য (; i <= len - vstep; i += vstep) {
                v_uint8 m   = vx_load(mask + i);
                v_uint8 s   = vx_load(src + i);
                v_uint8 sel = v_and(s, v_gt(m, vx_setzero_u8()));
                acc = v_max(acc, sel);
            }

            ফলাফল = (পূর্ণসংখ্যা)v_reduce_max(acc);

            জন্য (; i < len; i++) {
                যদি (mask[i])
                    ফলাফল = std::max(ফলাফল, (পূর্ণসংখ্যা)src[i]);
            }
        }
        অন্যথা যদি (cn == 4 && len >= VTraits<v_uint8>::vlanes()) {
            ধ্রুবক পূর্ণসংখ্যা vstep = VTraits<v_uint8>::vlanes();
            v_uint8 acc = vx_setzero_u8();
            পূর্ণসংখ্যা i = 0;
            জন্য (;;) {
                যদি (i > len - vstep)
                    i = len - vstep; // back-step (max is idempotent)
                v_uint8 c0, c1, c2, c3;
                v_load_deinterleave(src + i * 4, c0, c1, c2, c3);
                v_uint8 pmax = v_max(v_max(c0, c1), v_max(c2, c3));
                v_uint8 m = v_gt(vx_load(mask + i), vx_setzero_u8());
                acc = v_max(acc, v_and(pmax, m));
                যদি (i >= len - vstep)
                    বিরতি;
                i += vstep;
            }
            ফলাফল = (পূর্ণসংখ্যা)v_reduce_max(acc);
        }
        অন্যথা যদি (cn == 4) {
            জন্য (পূর্ণসংখ্যা i = 0; i < len; i++) {
                যদি (mask[i]) {
                    ধ্রুবক uchar* elem = src + i * 4;
                    ফলাফল = std::max(ফলাফল, (পূর্ণসংখ্যা)std::max(std::max(elem[0], elem[1]),
                                                            std::max(elem[2], elem[3])));
                }
            }
        }
        অন্যথা {
            জন্য (পূর্ণসংখ্যা i = 0; i < len; i++) {
                যদি (mask[i]) {
                    ধ্রুবক uchar* elem = src + i * cn;
                    জন্য (পূর্ণসংখ্যা k = 0; k < cn; k++)
                        ফলাফল = std::max(ফলাফল, (পূর্ণসংখ্যা)elem[k]);
                }
            }
        }
        ফেরত ফলাফল;
    }
};

template <>
কাঠামো MaskedNormL1_SIMD<uchar, পূর্ণসংখ্যা> {
    অন্তঃরেখ পূর্ণসংখ্যা operator()(ধ্রুবক uchar* src, ধ্রুবক uchar* mask, পূর্ণসংখ্যা len, পূর্ণসংখ্যা cn) ধ্রুবক {
        পূর্ণসংখ্যা ফলাফল = 0;
        ধ্রুবক পূর্ণসংখ্যা vstep = VTraits<v_uint8>::vlanes();
        ধ্রুবক v_uint8 one = vx_setall_u8(1);
        যদি (cn == 1) {
            পূর্ণসংখ্যা i = 0;
            v_uint32 acc = vx_setzero_u32();
            জন্য (; i <= len - vstep; i += vstep) {
                v_uint8 s = v_and(vx_load(src + i), v_normmask_u8(mask + i));
                acc = v_dotprod_expand_fast(s, one, acc);
            }
            ফলাফল = (পূর্ণসংখ্যা)v_reduce_যোগ(acc);
            জন্য (; i < len; i++) {
                যদি (mask[i])
                    ফলাফল += src[i];
            }
        }
        অন্যথা যদি (cn == 4) {
            পূর্ণসংখ্যা i = 0;
            v_uint32 acc = vx_setzero_u32();
            জন্য (; i <= len - vstep; i += vstep) {
                v_uint8 c0, c1, c2, c3;
                v_load_deinterleave(src + i * 4, c0, c1, c2, c3);
                v_uint8 m = v_normmask_u8(mask + i);
                acc = v_dotprod_expand_fast(v_and(c0, m), one, acc);
                acc = v_dotprod_expand_fast(v_and(c1, m), one, acc);
                acc = v_dotprod_expand_fast(v_and(c2, m), one, acc);
                acc = v_dotprod_expand_fast(v_and(c3, m), one, acc);
            }
            ফলাফল = (পূর্ণসংখ্যা)v_reduce_যোগ(acc);
            জন্য (; i < len; i++) {
                যদি (mask[i]) {
                    ধ্রুবক uchar* elem = src + i * 4;
                    ফলাফল += elem[0] + elem[1] + elem[2] + elem[3];
                }
            }
        }
        অন্যথা {
            জন্য (পূর্ণসংখ্যা i = 0; i < len; i++) {
                যদি (mask[i]) {
                    ধ্রুবক uchar* elem = src + i * cn;
                    জন্য (পূর্ণসংখ্যা k = 0; k < cn; k++)
                        ফলাফল += elem[k];
                }
            }
        }
        ফেরত ফলাফল;
    }
};

template <>
কাঠামো MaskedNormInf_SIMD<ushort, পূর্ণসংখ্যা> {
    অন্তঃরেখ পূর্ণসংখ্যা operator()(ধ্রুবক ushort* src, ধ্রুবক uchar* mask, পূর্ণসংখ্যা len, পূর্ণসংখ্যা cn) ধ্রুবক {
        পূর্ণসংখ্যা ফলাফল = 0;
        যদি (cn == 1) {
            পূর্ণসংখ্যা i = 0;
            ধ্রুবক পূর্ণসংখ্যা vstep = VTraits<v_uint16>::vlanes();
            v_uint16 acc = vx_setzero_u16();

            জন্য (; i <= len - vstep; i += vstep) {
                v_uint16 m   = vx_load_expand(mask + i);
                v_uint16 cmp = v_gt(m, vx_setzero_u16());
                v_uint16 s   = vx_load(src + i);
                v_uint16 sel = v_and(s, cmp);
                acc = v_max(acc, sel);
            }

            ফলাফল = (পূর্ণসংখ্যা)v_reduce_max(acc);

            জন্য (; i < len; i++) {
                যদি (mask[i])
                    ফলাফল = std::max(ফলাফল, (পূর্ণসংখ্যা)src[i]);
            }
        }
        অন্যথা {
            জন্য (পূর্ণসংখ্যা i = 0; i < len; i++) {
                যদি (mask[i]) {
                    ধ্রুবক ushort* elem = src + i * cn;
                    পূর্ণসংখ্যা k = 0;
                    ধ্রুবক পূর্ণসংখ্যা vstep = VTraits<v_uint16>::vlanes();
                    v_uint16 acc = vx_setzero_u16();

                    জন্য (; k <= cn - vstep; k += vstep) {
                        acc = v_max(acc, vx_load(elem + k));
                    }

                    ফলাফল = std::max(ফলাফল, (পূর্ণসংখ্যা)v_reduce_max(acc));

                    জন্য (; k < cn; k++)
                        ফলাফল = std::max(ফলাফল, (পূর্ণসংখ্যা)elem[k]);
                }
            }
        }
        ফেরত ফলাফল;
    }
};

template <>
কাঠামো MaskedNormL1_SIMD<ushort, পূর্ণসংখ্যা> {
    অন্তঃরেখ পূর্ণসংখ্যা operator()(ধ্রুবক ushort* src, ধ্রুবক uchar* mask, পূর্ণসংখ্যা len, পূর্ণসংখ্যা cn) ধ্রুবক {
        পূর্ণসংখ্যা ফলাফল = 0;
        যদি (cn == 1) {
            পূর্ণসংখ্যা i = 0;
            ধ্রুবক পূর্ণসংখ্যা vstep = VTraits<v_uint16>::vlanes();
            v_uint32 acc32 = vx_setzero_u32();
            v_uint64 acc64 = vx_setzero_u64();
            পূর্ণসংখ্যা acc32_elems = 0;

            জন্য (; i <= len - vstep; i += vstep, acc32_elems += vstep) {
                যদি (acc32_elems >= 512) {
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
            ফলাফল = (পূর্ণসংখ্যা)v_reduce_যোগ(acc64);

            জন্য (; i < len; i++) {
                যদি (mask[i])
                    ফলাফল += src[i];
            }
        }
        অন্যথা {
            জন্য (পূর্ণসংখ্যা i = 0; i < len; i++) {
                যদি (mask[i]) {
                    ধ্রুবক ushort* elem = src + i * cn;
                    পূর্ণসংখ্যা k = 0;
                    ধ্রুবক পূর্ণসংখ্যা vstep = VTraits<v_uint16>::vlanes();
                    v_uint32 acc = vx_setzero_u32();

                    জন্য (; k <= cn - vstep; k += vstep) {
                        v_uint32 lo32, hi32;
                        v_expand(vx_load(elem + k), lo32, hi32);
                        acc = v_add(acc, v_add(lo32, hi32));
                    }

                    ফলাফল += (পূর্ণসংখ্যা)v_reduce_যোগ(acc);

                    জন্য (; k < cn; k++)
                        ফলাফল += elem[k];
                }
            }
        }
        ফেরত ফলাফল;
    }
};

template <>
কাঠামো MaskedNormL2_SIMD<ushort, দ্বিগুণ> {
    অন্তঃরেখ দ্বিগুণ operator()(ধ্রুবক ushort* src, ধ্রুবক uchar* mask, পূর্ণসংখ্যা len, পূর্ণসংখ্যা cn) ধ্রুবক {
        দ্বিগুণ ফলাফল = 0.0;
        যদি (cn == 1) {
            পূর্ণসংখ্যা i = 0;
            ধ্রুবক পূর্ণসংখ্যা vstep = VTraits<v_uint16>::vlanes();
            v_uint64 acc = vx_setzero_u64();
            জন্য (; i <= len - vstep; i += vstep) {
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
            ফলাফল = (দ্বিগুণ)v_reduce_যোগ(acc);
            জন্য (; i < len; i++) {
                যদি (mask[i]) {
                    দ্বিগুণ v = src[i];
                    ফলাফল += v * v;
                }
            }
        }
        অন্যথা {
            জন্য (পূর্ণসংখ্যা i = 0; i < len; i++) {
                যদি (mask[i]) {
                    ধ্রুবক ushort* elem = src + i * cn;
                    পূর্ণসংখ্যা k = 0;
                    ধ্রুবক পূর্ণসংখ্যা vstep = VTraits<v_uint16>::vlanes();
                    v_uint64 acc = vx_setzero_u64();
                    জন্য (; k <= cn - vstep; k += vstep) {
                        v_uint32 lo32, hi32;
                        v_expand(vx_load(elem + k), lo32, hi32);
                        v_uint64 lo64a, lo64b, hi64a, hi64b;
                        v_expand(v_mul(lo32, lo32), lo64a, lo64b);
                        v_expand(v_mul(hi32, hi32), hi64a, hi64b);
                        acc = v_add(acc, v_add(v_add(lo64a, lo64b), v_add(hi64a, hi64b)));
                    }

                    ফলাফল += (দ্বিগুণ)v_reduce_যোগ(acc);

                    জন্য (; k < cn; k++) {
                        দ্বিগুণ v = elem[k];
                        ফলাফল += v * v;
                    }
                }
            }
        }
        ফেরত ফলাফল;
    }
};

template <>
কাঠামো MaskedNormInf_SIMD<schar, পূর্ণসংখ্যা> {
    অন্তঃরেখ পূর্ণসংখ্যা operator()(ধ্রুবক schar* src, ধ্রুবক uchar* mask, পূর্ণসংখ্যা len, পূর্ণসংখ্যা cn) ধ্রুবক {
        পূর্ণসংখ্যা ফলাফল = 0;
        যদি (cn == 1) {
            ধ্রুবক পূর্ণসংখ্যা vstep = VTraits<v_int8>::vlanes();
            পূর্ণসংখ্যা i = 0;
            v_uint8 acc = vx_setzero_u8();
            জন্য (; i <= len - vstep; i += vstep)
                acc = v_max(acc, v_and(v_abs(vx_load(src + i)), v_normmask_u8(mask + i)));
            ফলাফল = (পূর্ণসংখ্যা)v_reduce_max(acc);
            জন্য (; i < len; i++)
                যদি (mask[i]) ফলাফল = std::max(ফলাফল, cv_abs(src[i]));
        }
        অন্যথা {
            জন্য (পূর্ণসংখ্যা i = 0; i < len; i++)
                যদি (mask[i]) {
                    ধ্রুবক schar* elem = src + i * cn;
                    জন্য (পূর্ণসংখ্যা k = 0; k < cn; k++)
                        ফলাফল = std::max(ফলাফল, cv_abs(elem[k]));
                }
        }
        ফেরত ফলাফল;
    }
};

template <>
কাঠামো MaskedNormL1_SIMD<schar, পূর্ণসংখ্যা> {
    অন্তঃরেখ পূর্ণসংখ্যা operator()(ধ্রুবক schar* src, ধ্রুবক uchar* mask, পূর্ণসংখ্যা len, পূর্ণসংখ্যা cn) ধ্রুবক {
        পূর্ণসংখ্যা ফলাফল = 0;
        যদি (cn == 1) {
            ধ্রুবক পূর্ণসংখ্যা vstep = VTraits<v_int8>::vlanes();
            ধ্রুবক v_uint8 one = vx_setall_u8(1);
            পূর্ণসংখ্যা i = 0;
            v_uint32 acc = vx_setzero_u32();
            জন্য (; i <= len - vstep; i += vstep)
                acc = v_dotprod_expand_fast(v_and(v_abs(vx_load(src + i)), v_normmask_u8(mask + i)), one, acc);
            ফলাফল = (পূর্ণসংখ্যা)v_reduce_যোগ(acc);
            জন্য (; i < len; i++)
                যদি (mask[i]) ফলাফল += cv_abs(src[i]);
        }
        অন্যথা {
            জন্য (পূর্ণসংখ্যা i = 0; i < len; i++)
                যদি (mask[i]) {
                    ধ্রুবক schar* elem = src + i * cn;
                    জন্য (পূর্ণসংখ্যা k = 0; k < cn; k++)
                        ফলাফল += cv_abs(elem[k]);
                }
        }
        ফেরত ফলাফল;
    }
};

template <>
কাঠামো MaskedNormL2_SIMD<schar, পূর্ণসংখ্যা> {
    অন্তঃরেখ পূর্ণসংখ্যা operator()(ধ্রুবক schar* src, ধ্রুবক uchar* mask, পূর্ণসংখ্যা len, পূর্ণসংখ্যা cn) ধ্রুবক {
        পূর্ণসংখ্যা ফলাফল = 0;
        যদি (cn == 1) {
            ধ্রুবক পূর্ণসংখ্যা vstep = VTraits<v_int8>::vlanes();
            পূর্ণসংখ্যা i = 0;
            v_int32 acc = vx_setzero_s32();
            জন্য (; i <= len - vstep; i += vstep) {
                v_int8 s = v_and(vx_load(src + i), v_reinterpret_as_s8(v_normmask_u8(mask + i)));
                acc = v_dotprod_expand_fast(s, s, acc);
            }
            ফলাফল = (পূর্ণসংখ্যা)v_reduce_যোগ(acc);
            জন্য (; i < len; i++)
                যদি (mask[i]) { পূর্ণসংখ্যা v = src[i]; ফলাফল += v * v; }
        }
        অন্যথা {
            জন্য (পূর্ণসংখ্যা i = 0; i < len; i++)
                যদি (mask[i]) {
                    ধ্রুবক schar* elem = src + i * cn;
                    জন্য (পূর্ণসংখ্যা k = 0; k < cn; k++) { পূর্ণসংখ্যা v = elem[k]; ফলাফল += v * v; }
                }
        }
        ফেরত ফলাফল;
    }
};

template <>
কাঠামো MaskedNormL2_SIMD<uchar, পূর্ণসংখ্যা> {
    অন্তঃরেখ পূর্ণসংখ্যা operator()(ধ্রুবক uchar* src, ধ্রুবক uchar* mask, পূর্ণসংখ্যা len, পূর্ণসংখ্যা cn) ধ্রুবক {
        পূর্ণসংখ্যা ফলাফল = 0;
        ধ্রুবক পূর্ণসংখ্যা vstep = VTraits<v_uint8>::vlanes();
        যদি (cn == 1) {
            পূর্ণসংখ্যা i = 0;
            v_uint32 acc = vx_setzero_u32();
            জন্য (; i <= len - vstep; i += vstep) {
                v_uint8 s = v_and(vx_load(src + i), v_normmask_u8(mask + i));
                acc = v_dotprod_expand_fast(s, s, acc);
            }
            ফলাফল = (পূর্ণসংখ্যা)v_reduce_যোগ(acc);
            জন্য (; i < len; i++)
                যদি (mask[i]) { পূর্ণসংখ্যা v = src[i]; ফলাফল += v * v; }
        }
        অন্যথা যদি (cn == 4) {
            পূর্ণসংখ্যা i = 0;
            v_uint32 acc = vx_setzero_u32();
            জন্য (; i <= len - vstep; i += vstep) {
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
            ফলাফল = (পূর্ণসংখ্যা)v_reduce_যোগ(acc);
            জন্য (; i < len; i++)
                যদি (mask[i]) {
                    ধ্রুবক uchar* elem = src + i * 4;
                    জন্য (পূর্ণসংখ্যা k = 0; k < 4; k++) { পূর্ণসংখ্যা v = elem[k]; ফলাফল += v * v; }
                }
        }
        অন্যথা {
            জন্য (পূর্ণসংখ্যা i = 0; i < len; i++)
                যদি (mask[i]) {
                    ধ্রুবক uchar* elem = src + i * cn;
                    জন্য (পূর্ণসংখ্যা k = 0; k < cn; k++) { পূর্ণসংখ্যা v = elem[k]; ফলাফল += v * v; }
                }
        }
        ফেরত ফলাফল;
    }
};

template <>
কাঠামো MaskedNormInf_SIMD<short, পূর্ণসংখ্যা> {
    অন্তঃরেখ পূর্ণসংখ্যা operator()(ধ্রুবক short* src, ধ্রুবক uchar* mask, পূর্ণসংখ্যা len, পূর্ণসংখ্যা cn) ধ্রুবক {
        পূর্ণসংখ্যা ফলাফল = 0;
        যদি (cn == 1) {
            ধ্রুবক পূর্ণসংখ্যা vstep = VTraits<v_int16>::vlanes();
            পূর্ণসংখ্যা i = 0;
            v_uint16 acc = vx_setzero_u16();
            জন্য (; i <= len - vstep; i += vstep)
                acc = v_max(acc, v_and(v_abs(vx_load(src + i)), v_normmask_u16(mask + i)));
            ফলাফল = (পূর্ণসংখ্যা)v_reduce_max(acc);
            জন্য (; i < len; i++)
                যদি (mask[i]) ফলাফল = std::max(ফলাফল, cv_abs(src[i]));
        }
        অন্যথা {
            জন্য (পূর্ণসংখ্যা i = 0; i < len; i++)
                যদি (mask[i]) {
                    ধ্রুবক short* elem = src + i * cn;
                    জন্য (পূর্ণসংখ্যা k = 0; k < cn; k++)
                        ফলাফল = std::max(ফলাফল, cv_abs(elem[k]));
                }
        }
        ফেরত ফলাফল;
    }
};

template <>
কাঠামো MaskedNormL1_SIMD<short, পূর্ণসংখ্যা> {
    অন্তঃরেখ পূর্ণসংখ্যা operator()(ধ্রুবক short* src, ধ্রুবক uchar* mask, পূর্ণসংখ্যা len, পূর্ণসংখ্যা cn) ধ্রুবক {
        পূর্ণসংখ্যা ফলাফল = 0;
        যদি (cn == 1) {
            ধ্রুবক পূর্ণসংখ্যা vstep = VTraits<v_int16>::vlanes();
            পূর্ণসংখ্যা i = 0;
            v_uint32 acc0 = vx_setzero_u32(), acc1 = vx_setzero_u32();
            জন্য (; i <= len - vstep; i += vstep) {
                v_uint16 s = v_and(v_abs(vx_load(src + i)), v_normmask_u16(mask + i));
                v_uint32 lo, hi;
                v_expand(s, lo, hi);
                acc0 = v_add(acc0, lo);
                acc1 = v_add(acc1, hi);
            }
            ফলাফল = (পূর্ণসংখ্যা)v_reduce_যোগ(v_add(acc0, acc1));
            জন্য (; i < len; i++)
                যদি (mask[i]) ফলাফল += cv_abs(src[i]);
        }
        অন্যথা {
            জন্য (পূর্ণসংখ্যা i = 0; i < len; i++)
                যদি (mask[i]) {
                    ধ্রুবক short* elem = src + i * cn;
                    জন্য (পূর্ণসংখ্যা k = 0; k < cn; k++)
                        ফলাফল += cv_abs(elem[k]);
                }
        }
        ফেরত ফলাফল;
    }
};

template <>
কাঠামো MaskedNormL2_SIMD<short, দ্বিগুণ> {
    অন্তঃরেখ দ্বিগুণ operator()(ধ্রুবক short* src, ধ্রুবক uchar* mask, পূর্ণসংখ্যা len, পূর্ণসংখ্যা cn) ধ্রুবক {
        দ্বিগুণ ফলাফল = 0.0;
        যদি (cn == 1) {
            ধ্রুবক পূর্ণসংখ্যা vstep = VTraits<v_int16>::vlanes();
            পূর্ণসংখ্যা i = 0;
            v_uint64 acc = vx_setzero_u64();
            জন্য (; i <= len - vstep; i += vstep) {
                v_uint16 s = v_and(v_abs(vx_load(src + i)), v_normmask_u16(mask + i));
                acc = v_add(acc, v_dotprod_expand_fast(s, s));
            }
            ফলাফল = (দ্বিগুণ)v_reduce_যোগ(acc);
            জন্য (; i < len; i++)
                যদি (mask[i]) { দ্বিগুণ v = src[i]; ফলাফল += v * v; }
        }
        অন্যথা {
            জন্য (পূর্ণসংখ্যা i = 0; i < len; i++)
                যদি (mask[i]) {
                    ধ্রুবক short* elem = src + i * cn;
                    জন্য (পূর্ণসংখ্যা k = 0; k < cn; k++) { দ্বিগুণ v = elem[k]; ফলাফল += v * v; }
                }
        }
        ফেরত ফলাফল;
    }
};

template <>
কাঠামো MaskedNormInf_SIMD<পূর্ণসংখ্যা, unsigned> {
    অন্তঃরেখ unsigned operator()(ধ্রুবক পূর্ণসংখ্যা* src, ধ্রুবক uchar* mask, পূর্ণসংখ্যা len, পূর্ণসংখ্যা cn) ধ্রুবক {
        unsigned ফলাফল = 0;
        যদি (cn == 1) {
            ধ্রুবক পূর্ণসংখ্যা vstep = VTraits<v_int32>::vlanes();
            পূর্ণসংখ্যা i = 0;
            v_uint32 acc = vx_setzero_u32();
            জন্য (; i <= len - vstep; i += vstep)
                acc = v_max(acc, v_and(v_abs(vx_load(src + i)), v_normmask_u32(mask + i)));
            ফলাফল = v_reduce_max(acc);
            জন্য (; i < len; i++)
                যদি (mask[i]) ফলাফল = std::max(ফলাফল, cv_abs(src[i]));
        }
        অন্যথা {
            জন্য (পূর্ণসংখ্যা i = 0; i < len; i++)
                যদি (mask[i]) {
                    ধ্রুবক পূর্ণসংখ্যা* elem = src + i * cn;
                    জন্য (পূর্ণসংখ্যা k = 0; k < cn; k++)
                        ফলাফল = std::max(ফলাফল, cv_abs(elem[k]));
                }
        }
        ফেরত ফলাফল;
    }
};

#যদি CV_SIMD_64F || CV_SIMD_SCALABLE_64F

template <>
কাঠামো MaskedNormL1_SIMD<পূর্ণসংখ্যা, দ্বিগুণ> {
    অন্তঃরেখ দ্বিগুণ operator()(ধ্রুবক পূর্ণসংখ্যা* src, ধ্রুবক uchar* mask, পূর্ণসংখ্যা len, পূর্ণসংখ্যা cn) ধ্রুবক {
        দ্বিগুণ ফলাফল = 0.0;
        যদি (cn == 1) {
            ধ্রুবক পূর্ণসংখ্যা vstep = VTraits<v_int32>::vlanes();
            পূর্ণসংখ্যা i = 0;
            v_float64 acc0 = vx_setzero_f64(), acc1 = vx_setzero_f64();
            জন্য (; i <= len - vstep; i += vstep) {
                // Mask the value, then widen and take the magnitude in double. v_abs() does give
                // |INT_MIN| as an unsigned 2147483648, but reinterpreting that back as int32 turns
                // it negative again, and the widening then carries the sign into the accumulator.
                v_int32 s = v_reinterpret_as_s32(v_and(v_reinterpret_as_u32(vx_load(src + i)),
                                                       v_normmask_u32(mask + i)));
                acc0 = v_add(acc0, v_abs(v_cvt_f64(s)));
                acc1 = v_add(acc1, v_abs(v_cvt_f64_উপরের(s)));
            }
            ফলাফল = v_reduce_যোগ(v_add(acc0, acc1));
            জন্য (; i < len; i++)
                যদি (mask[i]) ফলাফল += cv_abs(src[i]);
        }
        অন্যথা {
            জন্য (পূর্ণসংখ্যা i = 0; i < len; i++)
                যদি (mask[i]) {
                    ধ্রুবক পূর্ণসংখ্যা* elem = src + i * cn;
                    জন্য (পূর্ণসংখ্যা k = 0; k < cn; k++)
                        ফলাফল += cv_abs(elem[k]);
                }
        }
        ফেরত ফলাফল;
    }
};

template <>
কাঠামো MaskedNormL2_SIMD<পূর্ণসংখ্যা, দ্বিগুণ> {
    অন্তঃরেখ দ্বিগুণ operator()(ধ্রুবক পূর্ণসংখ্যা* src, ধ্রুবক uchar* mask, পূর্ণসংখ্যা len, পূর্ণসংখ্যা cn) ধ্রুবক {
        দ্বিগুণ ফলাফল = 0.0;
        যদি (cn == 1) {
            ধ্রুবক পূর্ণসংখ্যা vstep = VTraits<v_int32>::vlanes();
            পূর্ণসংখ্যা i = 0;
            v_float64 acc0 = vx_setzero_f64(), acc1 = vx_setzero_f64();
            জন্য (; i <= len - vstep; i += vstep) {
                v_uint32 s = v_and(v_abs(vx_load(src + i)), v_normmask_u32(mask + i));
                v_uint64 e0, e1;
                v_expand(s, e0, e1);
                v_float64 f0 = v_cvt_f64(v_reinterpret_as_s64(e0));
                v_float64 f1 = v_cvt_f64(v_reinterpret_as_s64(e1));
                acc0 = v_fma(f0, f0, acc0);
                acc1 = v_fma(f1, f1, acc1);
            }
            ফলাফল = v_reduce_যোগ(v_add(acc0, acc1));
            জন্য (; i < len; i++)
                যদি (mask[i]) { দ্বিগুণ v = src[i]; ফলাফল += v * v; }
        }
        অন্যথা {
            জন্য (পূর্ণসংখ্যা i = 0; i < len; i++)
                যদি (mask[i]) {
                    ধ্রুবক পূর্ণসংখ্যা* elem = src + i * cn;
                    জন্য (পূর্ণসংখ্যা k = 0; k < cn; k++) { দ্বিগুণ v = elem[k]; ফলাফল += v * v; }
                }
        }
        ফেরত ফলাফল;
    }
};

template <>
কাঠামো MaskedNormInf_SIMD<দ্বিগুণ, দ্বিগুণ> {
    অন্তঃরেখ দ্বিগুণ operator()(ধ্রুবক দ্বিগুণ* src, ধ্রুবক uchar* mask, পূর্ণসংখ্যা len, পূর্ণসংখ্যা cn) ধ্রুবক {
        দ্বিগুণ ফলাফল = 0.0;
        যদি (cn == 1) {
            ধ্রুবক পূর্ণসংখ্যা vstep = VTraits<v_float64>::vlanes();
            পূর্ণসংখ্যা i = 0;
            v_float64 acc0 = vx_setzero_f64(), acc1 = vx_setzero_f64();
            জন্য (; i <= len - 2*vstep; i += 2*vstep) {
                v_uint64 m0, m1;
                v_normmask_u64(mask + i, m0, m1);
                acc0 = v_max(acc0, v_reinterpret_as_f64(v_and(v_reinterpret_as_u64(v_abs(vx_load(src + i))), m0)));
                acc1 = v_max(acc1, v_reinterpret_as_f64(v_and(v_reinterpret_as_u64(v_abs(vx_load(src + i + vstep))), m1)));
            }
            দ্বিগুণ t[VTraits<v_float64>::max_nlanes];
            vx_store(t, v_max(acc0, acc1));
            জন্য (পূর্ণসংখ্যা k = 0; k < vstep; k++)
                ফলাফল = std::max(ফলাফল, t[k]);
            জন্য (; i < len; i++)
                যদি (mask[i]) ফলাফল = std::max(ফলাফল, cv_abs(src[i]));
        }
        অন্যথা {
            জন্য (পূর্ণসংখ্যা i = 0; i < len; i++)
                যদি (mask[i]) {
                    ধ্রুবক দ্বিগুণ* elem = src + i * cn;
                    জন্য (পূর্ণসংখ্যা k = 0; k < cn; k++)
                        ফলাফল = std::max(ফলাফল, cv_abs(elem[k]));
                }
        }
        ফেরত ফলাফল;
    }
};

template <>
কাঠামো MaskedNormL1_SIMD<দ্বিগুণ, দ্বিগুণ> {
    অন্তঃরেখ দ্বিগুণ operator()(ধ্রুবক দ্বিগুণ* src, ধ্রুবক uchar* mask, পূর্ণসংখ্যা len, পূর্ণসংখ্যা cn) ধ্রুবক {
        দ্বিগুণ ফলাফল = 0.0;
        যদি (cn == 1) {
            ধ্রুবক পূর্ণসংখ্যা vstep = VTraits<v_float64>::vlanes();
            পূর্ণসংখ্যা i = 0;
            v_float64 acc0 = vx_setzero_f64(), acc1 = vx_setzero_f64();
            জন্য (; i <= len - 2*vstep; i += 2*vstep) {
                v_uint64 m0, m1;
                v_normmask_u64(mask + i, m0, m1);
                acc0 = v_add(acc0, v_reinterpret_as_f64(v_and(v_reinterpret_as_u64(v_abs(vx_load(src + i))), m0)));
                acc1 = v_add(acc1, v_reinterpret_as_f64(v_and(v_reinterpret_as_u64(v_abs(vx_load(src + i + vstep))), m1)));
            }
            ফলাফল = v_reduce_যোগ(v_add(acc0, acc1));
            জন্য (; i < len; i++)
                যদি (mask[i]) ফলাফল += cv_abs(src[i]);
        }
        অন্যথা {
            জন্য (পূর্ণসংখ্যা i = 0; i < len; i++)
                যদি (mask[i]) {
                    ধ্রুবক দ্বিগুণ* elem = src + i * cn;
                    জন্য (পূর্ণসংখ্যা k = 0; k < cn; k++)
                        ফলাফল += cv_abs(elem[k]);
                }
        }
        ফেরত ফলাফল;
    }
};

template <>
কাঠামো MaskedNormL2_SIMD<দ্বিগুণ, দ্বিগুণ> {
    অন্তঃরেখ দ্বিগুণ operator()(ধ্রুবক দ্বিগুণ* src, ধ্রুবক uchar* mask, পূর্ণসংখ্যা len, পূর্ণসংখ্যা cn) ধ্রুবক {
        দ্বিগুণ ফলাফল = 0.0;
        যদি (cn == 1) {
            ধ্রুবক পূর্ণসংখ্যা vstep = VTraits<v_float64>::vlanes();
            পূর্ণসংখ্যা i = 0;
            v_float64 acc0 = vx_setzero_f64(), acc1 = vx_setzero_f64();
            জন্য (; i <= len - 2*vstep; i += 2*vstep) {
                v_uint64 m0, m1;
                v_normmask_u64(mask + i, m0, m1);
                v_float64 s0 = v_reinterpret_as_f64(v_and(v_reinterpret_as_u64(vx_load(src + i)), m0));
                v_float64 s1 = v_reinterpret_as_f64(v_and(v_reinterpret_as_u64(vx_load(src + i + vstep)), m1));
                acc0 = v_fma(s0, s0, acc0);
                acc1 = v_fma(s1, s1, acc1);
            }
            ফলাফল = v_reduce_যোগ(v_add(acc0, acc1));
            জন্য (; i < len; i++)
                যদি (mask[i]) { দ্বিগুণ v = src[i]; ফলাফল += v * v; }
        }
        অন্যথা {
            জন্য (পূর্ণসংখ্যা i = 0; i < len; i++)
                যদি (mask[i]) {
                    ধ্রুবক দ্বিগুণ* elem = src + i * cn;
                    জন্য (পূর্ণসংখ্যা k = 0; k < cn; k++) { দ্বিগুণ v = elem[k]; ফলাফল += v * v; }
                }
        }
        ফেরত ফলাফল;
    }
};

#endif // CV_SIMD_64F || CV_SIMD_SCALABLE_64F
#endif // CV_SIMD || CV_SIMD_SCALABLE

template<typename T, typename ST> পূর্ণসংখ্যা
normInf_(ধ্রুবক T* src, ধ্রুবক uchar* mask, ST* ফলাফল_2, পূর্ণসংখ্যা len, পূর্ণসংখ্যা cn)
{
    ST ফলাফল = *ফলাফল_2;
    যদি( !mask )
    {
        NormInf_SIMD<T, ST> op;
        ফলাফল = std::max(ফলাফল, op(src, len*cn));
    } অন্যথা {
        MaskedNormInf_SIMD<T, ST> op;
        ফলাফল = std::max(ফলাফল, op(src, mask, len, cn));
    }
    *ফলাফল_2 = ফলাফল;
    ফেরত 0;
}

template<typename T, typename ST> পূর্ণসংখ্যা
normL1_(ধ্রুবক T* src, ধ্রুবক uchar* mask, ST* ফলাফল_2, পূর্ণসংখ্যা len, পূর্ণসংখ্যা cn)
{
    ST ফলাফল = *ফলাফল_2;
    যদি( !mask )
    {
        NormL1_SIMD<T, ST> op;
        ফলাফল += op(src, len*cn);
    } অন্যথা {
        MaskedNormL1_SIMD<T, ST> op;
        ফলাফল += op(src, mask, len, cn);
    }
    *ফলাফল_2 = ফলাফল;
    ফেরত 0;
}

template<typename T, typename ST> পূর্ণসংখ্যা
normL2_(ধ্রুবক T* src, ধ্রুবক uchar* mask, ST* ফলাফল_2, পূর্ণসংখ্যা len, পূর্ণসংখ্যা cn)
{
    ST ফলাফল = *ফলাফল_2;
    যদি( !mask )
    {
        NormL2_SIMD<T, ST> op;
        ফলাফল += op(src, len*cn);
    }
    অন্যথা
    {
        MaskedNormL2_SIMD<T, ST> op;
        ফলাফল += op(src, mask, len, cn);
    }
    *ফলাফল_2 = ফলাফল;
    ফেরত 0;
}

স্থির পূর্ণসংখ্যা
normInf_Bool(ধ্রুবক uchar* src, ধ্রুবক uchar* mask, পূর্ণসংখ্যা* ফলাফল_2, পূর্ণসংখ্যা len, পূর্ণসংখ্যা cn)
{
    পূর্ণসংখ্যা ফলাফল = *ফলাফল_2;
    যদি( !mask )
    {
        জন্য ( পূর্ণসংখ্যা i = 0; i < len*cn; i++ ) {
            ফলাফল = std::max(ফলাফল, (পূর্ণসংখ্যা)(src[i] != 0));
            যদি (ফলাফল != 0)
                বিরতি;
        }
    }
    অন্যথা
    {
        জন্য( পূর্ণসংখ্যা i = 0; i < len; i++, src += cn )
            যদি( mask[i] )
            {
                জন্য( পূর্ণসংখ্যা k = 0; k < cn; k++ )
                    ফলাফল = std::max(ফলাফল, (পূর্ণসংখ্যা)(src[k] != 0));
                যদি (ফলাফল != 0)
                    বিরতি;
            }
    }
    *ফলাফল_2 = ফলাফল;
    ফেরত 0;
}

স্থির পূর্ণসংখ্যা
normL1_Bool(ধ্রুবক uchar* src, ধ্রুবক uchar* mask, পূর্ণসংখ্যা* ফলাফল_2, পূর্ণসংখ্যা len, পূর্ণসংখ্যা cn)
{
    পূর্ণসংখ্যা ফলাফল = *ফলাফল_2;
    যদি( !mask )
    {
        জন্য ( পূর্ণসংখ্যা i = 0; i < len*cn; i++ )
            ফলাফল += (পূর্ণসংখ্যা)(src[i] != 0);
    }
    অন্যথা
    {
        জন্য( পূর্ণসংখ্যা i = 0; i < len; i++, src += cn )
            যদি( mask[i] )
            {
                জন্য( পূর্ণসংখ্যা k = 0; k < cn; k++ )
                    ফলাফল += (পূর্ণসংখ্যা)(src[k] != 0);
            }
    }
    *ফলাফল_2 = ফলাফল;
    ফেরত 0;
}

স্থির পূর্ণসংখ্যা
normL2_Bool(ধ্রুবক uchar* src, ধ্রুবক uchar* mask, পূর্ণসংখ্যা* ফলাফল_2, পূর্ণসংখ্যা len, পূর্ণসংখ্যা cn)
{
    ফেরত normL1_Bool(src, mask, ফলাফল_2, len, cn);
}

// ====================================================================
// Masked norm-diff kernels.
// Scalar base templates (safe for every depth) plus SIMD cn==1
// specializations for the concrete (T, ST) pairs dispatched by
// CV_DEF_NORM_ALL. The masked normDiff*_ functions below select them.
// ====================================================================
template <typename T, typename ST>
কাঠামো MaskedNormDiffInf_SIMD {
    অন্তঃরেখ ST operator()(ধ্রুবক T* s1, ধ্রুবক T* s2, ধ্রুবক uchar* mask, পূর্ণসংখ্যা len, পূর্ণসংখ্যা cn) ধ্রুবক {
        ST ফলাফল = 0;
        জন্য (পূর্ণসংখ্যা i = 0; i < len; i++) যদি (mask[i]) {
            ধ্রুবক T* e1 = s1 + i*cn; ধ্রুবক T* e2 = s2 + i*cn;
            জন্য (পূর্ণসংখ্যা k = 0; k < cn; k++)
                ফলাফল = std::max(ফলাফল, (ST)cv_absdiff(e1[k], e2[k]));
        }
        ফেরত ফলাফল;
    }
};
template <typename T, typename ST>
কাঠামো MaskedNormDiffL1_SIMD {
    অন্তঃরেখ ST operator()(ধ্রুবক T* s1, ধ্রুবক T* s2, ধ্রুবক uchar* mask, পূর্ণসংখ্যা len, পূর্ণসংখ্যা cn) ধ্রুবক {
        ST ফলাফল = 0;
        জন্য (পূর্ণসংখ্যা i = 0; i < len; i++) যদি (mask[i]) {
            ধ্রুবক T* e1 = s1 + i*cn; ধ্রুবক T* e2 = s2 + i*cn;
            জন্য (পূর্ণসংখ্যা k = 0; k < cn; k++)
                ফলাফল += (ST)cv_absdiff(e1[k], e2[k]);
        }
        ফেরত ফলাফল;
    }
};
template <typename T, typename ST>
কাঠামো MaskedNormDiffL2_SIMD {
    অন্তঃরেখ ST operator()(ধ্রুবক T* s1, ধ্রুবক T* s2, ধ্রুবক uchar* mask, পূর্ণসংখ্যা len, পূর্ণসংখ্যা cn) ধ্রুবক {
        ST ফলাফল = 0;
        জন্য (পূর্ণসংখ্যা i = 0; i < len; i++) যদি (mask[i]) {
            ধ্রুবক T* e1 = s1 + i*cn; ধ্রুবক T* e2 = s2 + i*cn;
            জন্য (পূর্ণসংখ্যা k = 0; k < cn; k++) { ST v = (ST)e1[k] - (ST)e2[k]; ফলাফল += v*v; }
        }
        ফেরত ফলাফল;
    }
};

#যদি (CV_SIMD || CV_SIMD_SCALABLE)
// AND a per-lane diff vector with the (expanded) mask>0 predicate.
অন্তঃরেখ v_uint8  v_diffmask(ধ্রুবক v_uint8&  d, ধ্রুবক uchar* m) { ফেরত v_and(d, v_gt(vx_load(m),          vx_setzero_u8())); }
অন্তঃরেখ v_uint16 v_diffmask(ধ্রুবক v_uint16& d, ধ্রুবক uchar* m) { ফেরত v_and(d, v_gt(vx_load_expand(m),   vx_setzero_u16())); }
অন্তঃরেখ v_uint32 v_diffmask(ধ্রুবক v_uint32& d, ধ্রুবক uchar* m) { ফেরত v_and(d, v_gt(vx_load_expand_q(m), vx_setzero_u32())); }
অন্তঃরেখ v_float32 v_diffmask(ধ্রুবক v_float32& d, ধ্রুবক uchar* m) {
    v_uint32 cm = v_gt(vx_load_expand_q(m), vx_setzero_u32());
    ফেরত v_reinterpret_as_f32(v_and(v_reinterpret_as_u32(d), cm));
}

// Shared scalar cn>1 fallbacks (match the base templates' cv_absdiff semantics).
template<typename T, typename RT>
স্থির অন্তঃরেখ RT masked_Norm_Diff_Inf_লেজ(ধ্রুবক T* s1, ধ্রুবক T* s2, ধ্রুবক uchar* mask, পূর্ণসংখ্যা len, পূর্ণসংখ্যা cn, RT ফলাফল) {
    জন্য (পূর্ণসংখ্যা i = 0; i < len; i++) যদি (mask[i]) {
        ধ্রুবক T* e1 = s1 + i*cn; ধ্রুবক T* e2 = s2 + i*cn;
        জন্য (পূর্ণসংখ্যা k = 0; k < cn; k++) ফলাফল = std::max(ফলাফল, (RT)cv_absdiff(e1[k], e2[k]));
    }
    ফেরত ফলাফল;
}
template<typename T, typename RT>
স্থির অন্তঃরেখ RT masked_Norm_Diff_L1_লেজ(ধ্রুবক T* s1, ধ্রুবক T* s2, ধ্রুবক uchar* mask, পূর্ণসংখ্যা len, পূর্ণসংখ্যা cn, RT ফলাফল) {
    জন্য (পূর্ণসংখ্যা i = 0; i < len; i++) যদি (mask[i]) {
        ধ্রুবক T* e1 = s1 + i*cn; ধ্রুবক T* e2 = s2 + i*cn;
        জন্য (পূর্ণসংখ্যা k = 0; k < cn; k++) ফলাফল += (RT)cv_absdiff(e1[k], e2[k]);
    }
    ফেরত ফলাফল;
}
template<typename T, typename RT>
স্থির অন্তঃরেখ RT masked_Norm_Diff_L2_লেজ(ধ্রুবক T* s1, ধ্রুবক T* s2, ধ্রুবক uchar* mask, পূর্ণসংখ্যা len, পূর্ণসংখ্যা cn, RT ফলাফল) {
    জন্য (পূর্ণসংখ্যা i = 0; i < len; i++) যদি (mask[i]) {
        ধ্রুবক T* e1 = s1 + i*cn; ধ্রুবক T* e2 = s2 + i*cn;
        জন্য (পূর্ণসংখ্যা k = 0; k < cn; k++) { RT v = (RT)e1[k] - (RT)e2[k]; ফলাফল += v*v; }
    }
    ফেরত ফলাফল;
}

// Inf (cn==1): reduce_max of masked |a-b|. Shared across SIMD-capable depths.
template<typename T, typename ST>
স্থির অন্তঃরেখ ST maskedNormDiffInfCn1(ধ্রুবক T* s1, ধ্রুবক T* s2, ধ্রুবক uchar* mask, পূর্ণসংখ্যা len) {
    ধ্রুবক পূর্ণসংখ্যা vstep = VTraits<decltype(vx_load(s1))>::vlanes();
    ST ফলাফল = 0;
    পূর্ণসংখ্যা i = 0;
    যদি (len >= vstep) {
        auto acc = v_diffmask(v_absdiff(vx_load(s1), vx_load(s2)), mask);
        জন্য (i = vstep; i <= len - vstep; i += vstep)
            acc = v_max(acc, v_diffmask(v_absdiff(vx_load(s1 + i), vx_load(s2 + i)), mask + i));
        ফলাফল = (ST)v_reduce_max(acc);
    }
    জন্য (; i < len; i++) যদি (mask[i]) ফলাফল = std::max(ফলাফল, (ST)cv_absdiff(s1[i], s2[i]));
    ফেরত ফলাফল;
}

// Shared 8-bit cn==1 masked L1/L2 kernels (uchar/schar): v_absdiff yields v_uint8 for both.
template<typename T>
স্থির অন্তঃরেখ পূর্ণসংখ্যা maskedNormDiffL1_8(ধ্রুবক T* s1, ধ্রুবক T* s2, ধ্রুবক uchar* mask, পূর্ণসংখ্যা len) {
    পূর্ণসংখ্যা i = 0; ধ্রুবক পূর্ণসংখ্যা vstep = VTraits<v_uint8>::vlanes();
    ধ্রুবক v_uint8 one = vx_setall_u8(1);
    v_uint32 acc = vx_setzero_u32();
    জন্য (; i <= len - vstep; i += vstep) {
        v_uint8 m  = v_gt(vx_load(mask + i), vx_setzero_u8());
        v_uint8 ad = v_and(v_absdiff(vx_load(s1 + i), vx_load(s2 + i)), m);
        acc = v_dotprod_expand_fast(ad, one, acc);
    }
    পূর্ণসংখ্যা ফলাফল = (পূর্ণসংখ্যা)v_reduce_যোগ(acc);
    জন্য (; i < len; i++) যদি (mask[i]) ফলাফল += (পূর্ণসংখ্যা)cv_absdiff(s1[i], s2[i]);
    ফেরত ফলাফল;
}
template<typename T>
স্থির অন্তঃরেখ পূর্ণসংখ্যা maskedNormDiffL2_8(ধ্রুবক T* s1, ধ্রুবক T* s2, ধ্রুবক uchar* mask, পূর্ণসংখ্যা len) {
    পূর্ণসংখ্যা i = 0; ধ্রুবক পূর্ণসংখ্যা vstep = VTraits<v_uint8>::vlanes();
    v_uint32 acc = vx_setzero_u32();
    জন্য (; i <= len - vstep; i += vstep) {
        v_uint8 m  = v_gt(vx_load(mask + i), vx_setzero_u8());
        v_uint8 ad = v_and(v_absdiff(vx_load(s1 + i), vx_load(s2 + i)), m);
        acc = v_dotprod_expand_fast(ad, ad, acc);
    }
    পূর্ণসংখ্যা ফলাফল = (পূর্ণসংখ্যা)v_reduce_যোগ(acc);
    জন্য (; i < len; i++) যদি (mask[i]) { পূর্ণসংখ্যা v = (পূর্ণসংখ্যা)s1[i] - (পূর্ণসংখ্যা)s2[i]; ফলাফল += v*v; }
    ফেরত ফলাফল;
}

// Shared 16-bit cn==1 masked L1 kernel (ushort/short): v_absdiff yields v_uint16 for both.
template<typename T>
স্থির অন্তঃরেখ পূর্ণসংখ্যা maskedNormDiffL1_16(ধ্রুবক T* s1, ধ্রুবক T* s2, ধ্রুবক uchar* mask, পূর্ণসংখ্যা len) {
    পূর্ণসংখ্যা i = 0; ধ্রুবক পূর্ণসংখ্যা vstep = VTraits<v_uint16>::vlanes();
    v_uint32 acc = vx_setzero_u32();
    জন্য (; i <= len - vstep; i += vstep) {
        v_uint16 m  = v_gt(vx_load_expand(mask + i), vx_setzero_u16());
        v_uint16 ad = v_and(v_absdiff(vx_load(s1 + i), vx_load(s2 + i)), m);
        v_uint32 lo, hi; v_expand(ad, lo, hi);
        acc = v_add(acc, v_add(lo, hi));
    }
    পূর্ণসংখ্যা ফলাফল = (পূর্ণসংখ্যা)v_reduce_যোগ(acc);
    জন্য (; i < len; i++) যদি (mask[i]) ফলাফল += (পূর্ণসংখ্যা)cv_absdiff(s1[i], s2[i]);
    ফেরত ফলাফল;
}

template<> কাঠামো MaskedNormDiffInf_SIMD<uchar, পূর্ণসংখ্যা> {
    পূর্ণসংখ্যা operator()(ধ্রুবক uchar* s1, ধ্রুবক uchar* s2, ধ্রুবক uchar* mask, পূর্ণসংখ্যা len, পূর্ণসংখ্যা cn) ধ্রুবক {
        যদি (cn == 1) ফেরত maskedNormDiffInfCn1<uchar, পূর্ণসংখ্যা>(s1, s2, mask, len);
        যদি (cn == 4 && len >= VTraits<v_uint8>::vlanes()) {
            ধ্রুবক পূর্ণসংখ্যা vstep = VTraits<v_uint8>::vlanes();
            v_uint8 acc = vx_setzero_u8();
            পূর্ণসংখ্যা i = 0;
            জন্য (;;) {
                যদি (i > len - vstep) i = len - vstep; // back-step (max is idempotent)
                v_uint8 a0,a1,a2,a3,b0,b1,b2,b3;
                v_load_deinterleave(s1 + i*4, a0,a1,a2,a3);
                v_load_deinterleave(s2 + i*4, b0,b1,b2,b3);
                v_uint8 ad = v_max(v_max(v_absdiff(a0,b0), v_absdiff(a1,b1)),
                                   v_max(v_absdiff(a2,b2), v_absdiff(a3,b3)));
                v_uint8 m = v_gt(vx_load(mask + i), vx_setzero_u8());
                acc = v_max(acc, v_and(ad, m));
                যদি (i >= len - vstep) বিরতি;
                i += vstep;
            }
            ফেরত (পূর্ণসংখ্যা)v_reduce_max(acc);
        }
        ফেরত masked_Norm_Diff_Inf_লেজ<uchar, পূর্ণসংখ্যা>(s1, s2, mask, len, cn, 0);
    }
};
template<> কাঠামো MaskedNormDiffInf_SIMD<schar, পূর্ণসংখ্যা> {
    পূর্ণসংখ্যা operator()(ধ্রুবক schar* s1, ধ্রুবক schar* s2, ধ্রুবক uchar* mask, পূর্ণসংখ্যা len, পূর্ণসংখ্যা cn) ধ্রুবক {
        যদি (cn == 1) ফেরত maskedNormDiffInfCn1<schar, পূর্ণসংখ্যা>(s1, s2, mask, len);
        ফেরত masked_Norm_Diff_Inf_লেজ<schar, পূর্ণসংখ্যা>(s1, s2, mask, len, cn, 0);
    }
};
template<> কাঠামো MaskedNormDiffInf_SIMD<ushort, পূর্ণসংখ্যা> {
    পূর্ণসংখ্যা operator()(ধ্রুবক ushort* s1, ধ্রুবক ushort* s2, ধ্রুবক uchar* mask, পূর্ণসংখ্যা len, পূর্ণসংখ্যা cn) ধ্রুবক {
        যদি (cn == 1) ফেরত maskedNormDiffInfCn1<ushort, পূর্ণসংখ্যা>(s1, s2, mask, len);
        ফেরত masked_Norm_Diff_Inf_লেজ<ushort, পূর্ণসংখ্যা>(s1, s2, mask, len, cn, 0);
    }
};
template<> কাঠামো MaskedNormDiffInf_SIMD<short, পূর্ণসংখ্যা> {
    পূর্ণসংখ্যা operator()(ধ্রুবক short* s1, ধ্রুবক short* s2, ধ্রুবক uchar* mask, পূর্ণসংখ্যা len, পূর্ণসংখ্যা cn) ধ্রুবক {
        যদি (cn == 1) ফেরত maskedNormDiffInfCn1<short, পূর্ণসংখ্যা>(s1, s2, mask, len);
        ফেরত masked_Norm_Diff_Inf_লেজ<short, পূর্ণসংখ্যা>(s1, s2, mask, len, cn, 0);
    }
};
template<> কাঠামো MaskedNormDiffInf_SIMD<পূর্ণসংখ্যা, unsigned> {
    unsigned operator()(ধ্রুবক পূর্ণসংখ্যা* s1, ধ্রুবক পূর্ণসংখ্যা* s2, ধ্রুবক uchar* mask, পূর্ণসংখ্যা len, পূর্ণসংখ্যা cn) ধ্রুবক {
        যদি (cn == 1) ফেরত maskedNormDiffInfCn1<পূর্ণসংখ্যা, unsigned>(s1, s2, mask, len);
        ফেরত masked_Norm_Diff_Inf_লেজ<পূর্ণসংখ্যা, unsigned>(s1, s2, mask, len, cn, 0u);
    }
};
template<> কাঠামো MaskedNormDiffInf_SIMD<দশমিক, দশমিক> {
    দশমিক operator()(ধ্রুবক দশমিক* s1, ধ্রুবক দশমিক* s2, ধ্রুবক uchar* mask, পূর্ণসংখ্যা len, পূর্ণসংখ্যা cn) ধ্রুবক {
        যদি (cn == 1) ফেরত maskedNormDiffInfCn1<দশমিক, দশমিক>(s1, s2, mask, len);
        ফেরত masked_Norm_Diff_Inf_লেজ<দশমিক, দশমিক>(s1, s2, mask, len, cn, 0.0f);
    }
};

template<> কাঠামো MaskedNormDiffL1_SIMD<uchar, পূর্ণসংখ্যা> {
    পূর্ণসংখ্যা operator()(ধ্রুবক uchar* s1, ধ্রুবক uchar* s2, ধ্রুবক uchar* mask, পূর্ণসংখ্যা len, পূর্ণসংখ্যা cn) ধ্রুবক {
        যদি (cn == 1) ফেরত maskedNormDiffL1_8(s1, s2, mask, len);
        যদি (cn == 4) {
            পূর্ণসংখ্যা i = 0; ধ্রুবক পূর্ণসংখ্যা vstep = VTraits<v_uint8>::vlanes();
            ধ্রুবক v_uint8 one = vx_setall_u8(1);
            v_uint32 acc = vx_setzero_u32();
            জন্য (; i <= len - vstep; i += vstep) {
                v_uint8 a0,a1,a2,a3,b0,b1,b2,b3;
                v_load_deinterleave(s1 + i*4, a0,a1,a2,a3);
                v_load_deinterleave(s2 + i*4, b0,b1,b2,b3);
                v_uint8 m = v_gt(vx_load(mask + i), vx_setzero_u8());
                acc = v_dotprod_expand_fast(v_and(v_absdiff(a0,b0), m), one, acc);
                acc = v_dotprod_expand_fast(v_and(v_absdiff(a1,b1), m), one, acc);
                acc = v_dotprod_expand_fast(v_and(v_absdiff(a2,b2), m), one, acc);
                acc = v_dotprod_expand_fast(v_and(v_absdiff(a3,b3), m), one, acc);
            }
            পূর্ণসংখ্যা ফলাফল = (পূর্ণসংখ্যা)v_reduce_যোগ(acc);
            জন্য (; i < len; i++) যদি (mask[i]) {
                ধ্রুবক uchar* e1 = s1 + i*4; ধ্রুবক uchar* e2 = s2 + i*4;
                জন্য (পূর্ণসংখ্যা k = 0; k < 4; k++) ফলাফল += (পূর্ণসংখ্যা)cv_absdiff(e1[k], e2[k]);
            }
            ফেরত ফলাফল;
        }
        ফেরত masked_Norm_Diff_L1_লেজ<uchar, পূর্ণসংখ্যা>(s1, s2, mask, len, cn, 0);
    }
};
template<> কাঠামো MaskedNormDiffL1_SIMD<schar, পূর্ণসংখ্যা> {
    পূর্ণসংখ্যা operator()(ধ্রুবক schar* s1, ধ্রুবক schar* s2, ধ্রুবক uchar* mask, পূর্ণসংখ্যা len, পূর্ণসংখ্যা cn) ধ্রুবক {
        যদি (cn == 1) ফেরত maskedNormDiffL1_8(s1, s2, mask, len);
        ফেরত masked_Norm_Diff_L1_লেজ<schar, পূর্ণসংখ্যা>(s1, s2, mask, len, cn, 0);
    }
};
template<> কাঠামো MaskedNormDiffL1_SIMD<ushort, পূর্ণসংখ্যা> {
    পূর্ণসংখ্যা operator()(ধ্রুবক ushort* s1, ধ্রুবক ushort* s2, ধ্রুবক uchar* mask, পূর্ণসংখ্যা len, পূর্ণসংখ্যা cn) ধ্রুবক {
        যদি (cn == 1) ফেরত maskedNormDiffL1_16(s1, s2, mask, len);
        ফেরত masked_Norm_Diff_L1_লেজ<ushort, পূর্ণসংখ্যা>(s1, s2, mask, len, cn, 0);
    }
};
template<> কাঠামো MaskedNormDiffL1_SIMD<short, পূর্ণসংখ্যা> {
    পূর্ণসংখ্যা operator()(ধ্রুবক short* s1, ধ্রুবক short* s2, ধ্রুবক uchar* mask, পূর্ণসংখ্যা len, পূর্ণসংখ্যা cn) ধ্রুবক {
        যদি (cn == 1) ফেরত maskedNormDiffL1_16(s1, s2, mask, len);
        ফেরত masked_Norm_Diff_L1_লেজ<short, পূর্ণসংখ্যা>(s1, s2, mask, len, cn, 0);
    }
};

template<> কাঠামো MaskedNormDiffL2_SIMD<uchar, পূর্ণসংখ্যা> {
    পূর্ণসংখ্যা operator()(ধ্রুবক uchar* s1, ধ্রুবক uchar* s2, ধ্রুবক uchar* mask, পূর্ণসংখ্যা len, পূর্ণসংখ্যা cn) ধ্রুবক {
        যদি (cn == 1) ফেরত maskedNormDiffL2_8(s1, s2, mask, len);
        যদি (cn == 4) {
            পূর্ণসংখ্যা i = 0; ধ্রুবক পূর্ণসংখ্যা vstep = VTraits<v_uint8>::vlanes();
            v_uint32 acc = vx_setzero_u32();
            জন্য (; i <= len - vstep; i += vstep) {
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
            পূর্ণসংখ্যা ফলাফল = (পূর্ণসংখ্যা)v_reduce_যোগ(acc);
            জন্য (; i < len; i++) যদি (mask[i]) {
                ধ্রুবক uchar* e1 = s1 + i*4; ধ্রুবক uchar* e2 = s2 + i*4;
                জন্য (পূর্ণসংখ্যা k = 0; k < 4; k++) { পূর্ণসংখ্যা v = (পূর্ণসংখ্যা)e1[k] - (পূর্ণসংখ্যা)e2[k]; ফলাফল += v*v; }
            }
            ফেরত ফলাফল;
        }
        ফেরত masked_Norm_Diff_L2_লেজ<uchar, পূর্ণসংখ্যা>(s1, s2, mask, len, cn, 0);
    }
};
template<> কাঠামো MaskedNormDiffL2_SIMD<schar, পূর্ণসংখ্যা> {
    পূর্ণসংখ্যা operator()(ধ্রুবক schar* s1, ধ্রুবক schar* s2, ধ্রুবক uchar* mask, পূর্ণসংখ্যা len, পূর্ণসংখ্যা cn) ধ্রুবক {
        যদি (cn == 1) ফেরত maskedNormDiffL2_8(s1, s2, mask, len);
        ফেরত masked_Norm_Diff_L2_লেজ<schar, পূর্ণসংখ্যা>(s1, s2, mask, len, cn, 0);
    }
};

#যদি CV_SIMD_64F
// Depths whose L1/L2 accumulate in double.
template<> কাঠামো MaskedNormDiffL1_SIMD<পূর্ণসংখ্যা, দ্বিগুণ> {
    দ্বিগুণ operator()(ধ্রুবক পূর্ণসংখ্যা* s1, ধ্রুবক পূর্ণসংখ্যা* s2, ধ্রুবক uchar* mask, পূর্ণসংখ্যা len, পূর্ণসংখ্যা cn) ধ্রুবক {
        যদি (cn != 1) ফেরত masked_Norm_Diff_L1_লেজ<পূর্ণসংখ্যা, দ্বিগুণ>(s1, s2, mask, len, cn, 0.0);
        পূর্ণসংখ্যা i = 0; ধ্রুবক পূর্ণসংখ্যা vstep = VTraits<v_int32>::vlanes();
        v_float64 acc = vx_setzero_f64();
        জন্য (; i <= len - vstep; i += vstep) {
            v_uint32 m  = v_gt(vx_load_expand_q(mask + i), vx_setzero_u32());
            v_uint32 ad = v_and(v_absdiff(vx_load(s1 + i), vx_load(s2 + i)), m);
            v_uint64 e0, e1; v_expand(ad, e0, e1);
            acc = v_add(acc, v_cvt_f64(v_reinterpret_as_s64(e0)));
            acc = v_add(acc, v_cvt_f64(v_reinterpret_as_s64(e1)));
        }
        দ্বিগুণ ফলাফল = v_reduce_যোগ(acc);
        জন্য (; i < len; i++) যদি (mask[i]) ফলাফল += (দ্বিগুণ)cv_absdiff(s1[i], s2[i]);
        ফেরত ফলাফল;
    }
};
template<> কাঠামো MaskedNormDiffL1_SIMD<দশমিক, দ্বিগুণ> {
    দ্বিগুণ operator()(ধ্রুবক দশমিক* s1, ধ্রুবক দশমিক* s2, ধ্রুবক uchar* mask, পূর্ণসংখ্যা len, পূর্ণসংখ্যা cn) ধ্রুবক {
        যদি (cn != 1) ফেরত masked_Norm_Diff_L1_লেজ<দশমিক, দ্বিগুণ>(s1, s2, mask, len, cn, 0.0);
        পূর্ণসংখ্যা i = 0; ধ্রুবক পূর্ণসংখ্যা vstep = VTraits<v_float32>::vlanes();
        v_float64 acc0 = vx_setzero_f64(), acc1 = vx_setzero_f64();
        জন্য (; i <= len - vstep; i += vstep) {
            v_float32 m  = v_reinterpret_as_f32(v_gt(vx_load_expand_q(mask + i), vx_setzero_u32()));
            v_float32 ad = v_and(v_absdiff(vx_load(s1 + i), vx_load(s2 + i)), m);
            acc0 = v_add(acc0, v_cvt_f64(ad));
            acc1 = v_add(acc1, v_cvt_f64_উপরের(ad));
        }
        দ্বিগুণ ফলাফল = v_reduce_যোগ(v_add(acc0, acc1));
        জন্য (; i < len; i++) যদি (mask[i]) ফলাফল += (দ্বিগুণ)cv_absdiff(s1[i], s2[i]);
        ফেরত ফলাফল;
    }
};

template<typename T>
স্থির অন্তঃরেখ দ্বিগুণ maskedNormDiffL2_16(ধ্রুবক T* s1, ধ্রুবক T* s2, ধ্রুবক uchar* mask, পূর্ণসংখ্যা len, পূর্ণসংখ্যা cn) {
    যদি (cn != 1) ফেরত masked_Norm_Diff_L2_লেজ<T, দ্বিগুণ>(s1, s2, mask, len, cn, 0.0);
    পূর্ণসংখ্যা i = 0; ধ্রুবক পূর্ণসংখ্যা vstep = VTraits<v_uint16>::vlanes();
    v_float64 acc = vx_setzero_f64();
    জন্য (; i <= len - vstep; i += vstep) {
        v_uint16 m  = v_gt(vx_load_expand(mask + i), vx_setzero_u16());
        v_uint16 ad = v_and(v_absdiff(vx_load(s1 + i), vx_load(s2 + i)), m);
        v_uint64 u = v_dotprod_expand_fast(ad, ad);
        acc = v_add(acc, v_cvt_f64(v_reinterpret_as_s64(u)));
    }
    দ্বিগুণ ফলাফল = v_reduce_যোগ(acc);
    জন্য (; i < len; i++) যদি (mask[i]) { দ্বিগুণ v = (দ্বিগুণ)s1[i] - (দ্বিগুণ)s2[i]; ফলাফল += v*v; }
    ফেরত ফলাফল;
}
template<> কাঠামো MaskedNormDiffL2_SIMD<ushort, দ্বিগুণ> {
    দ্বিগুণ operator()(ধ্রুবক ushort* s1, ধ্রুবক ushort* s2, ধ্রুবক uchar* mask, পূর্ণসংখ্যা len, পূর্ণসংখ্যা cn) ধ্রুবক
    { ফেরত maskedNormDiffL2_16(s1, s2, mask, len, cn); } };
template<> কাঠামো MaskedNormDiffL2_SIMD<short, দ্বিগুণ> {
    দ্বিগুণ operator()(ধ্রুবক short* s1, ধ্রুবক short* s2, ধ্রুবক uchar* mask, পূর্ণসংখ্যা len, পূর্ণসংখ্যা cn) ধ্রুবক
    { ফেরত maskedNormDiffL2_16(s1, s2, mask, len, cn); } };

template<> কাঠামো MaskedNormDiffL2_SIMD<পূর্ণসংখ্যা, দ্বিগুণ> {
    দ্বিগুণ operator()(ধ্রুবক পূর্ণসংখ্যা* s1, ধ্রুবক পূর্ণসংখ্যা* s2, ধ্রুবক uchar* mask, পূর্ণসংখ্যা len, পূর্ণসংখ্যা cn) ধ্রুবক {
        যদি (cn != 1) ফেরত masked_Norm_Diff_L2_লেজ<পূর্ণসংখ্যা, দ্বিগুণ>(s1, s2, mask, len, cn, 0.0);
        পূর্ণসংখ্যা i = 0; ধ্রুবক পূর্ণসংখ্যা vstep = VTraits<v_int32>::vlanes();
        v_float64 r0 = vx_setzero_f64(), r1 = vx_setzero_f64();
        জন্য (; i <= len - vstep; i += vstep) {
            v_uint32 m  = v_gt(vx_load_expand_q(mask + i), vx_setzero_u32());
            v_uint32 ad = v_and(v_absdiff(vx_load(s1 + i), vx_load(s2 + i)), m);
            v_uint64 e0, e1; v_expand(ad, e0, e1);
            v_float64 f0 = v_cvt_f64(v_reinterpret_as_s64(e0)), f1 = v_cvt_f64(v_reinterpret_as_s64(e1));
            r0 = v_fma(f0, f0, r0); r1 = v_fma(f1, f1, r1);
        }
        দ্বিগুণ ফলাফল = v_reduce_যোগ(v_add(r0, r1));
        জন্য (; i < len; i++) যদি (mask[i]) { দ্বিগুণ v = (দ্বিগুণ)s1[i] - (দ্বিগুণ)s2[i]; ফলাফল += v*v; }
        ফেরত ফলাফল;
    }
};
template<> কাঠামো MaskedNormDiffL2_SIMD<দশমিক, দ্বিগুণ> {
    দ্বিগুণ operator()(ধ্রুবক দশমিক* s1, ধ্রুবক দশমিক* s2, ধ্রুবক uchar* mask, পূর্ণসংখ্যা len, পূর্ণসংখ্যা cn) ধ্রুবক {
        যদি (cn != 1) ফেরত masked_Norm_Diff_L2_লেজ<দশমিক, দ্বিগুণ>(s1, s2, mask, len, cn, 0.0);
        পূর্ণসংখ্যা i = 0; ধ্রুবক পূর্ণসংখ্যা vstep = VTraits<v_float32>::vlanes();
        v_float64 r0 = vx_setzero_f64(), r1 = vx_setzero_f64();
        জন্য (; i <= len - vstep; i += vstep) {
            v_float32 m  = v_reinterpret_as_f32(v_gt(vx_load_expand_q(mask + i), vx_setzero_u32()));
            v_float32 ad = v_and(v_absdiff(vx_load(s1 + i), vx_load(s2 + i)), m);
            v_float64 f0 = v_cvt_f64(ad), f1 = v_cvt_f64_উপরের(ad);
            r0 = v_fma(f0, f0, r0); r1 = v_fma(f1, f1, r1);
        }
        দ্বিগুণ ফলাফল = v_reduce_যোগ(v_add(r0, r1));
        জন্য (; i < len; i++) যদি (mask[i]) { দ্বিগুণ v = (দ্বিগুণ)s1[i] - (দ্বিগুণ)s2[i]; ফলাফল += v*v; }
        ফেরত ফলাফল;
    }
};
template<>
কাঠামো MaskedNormDiffInf_SIMD<দ্বিগুণ, দ্বিগুণ> {
    অন্তঃরেখ দ্বিগুণ operator()(ধ্রুবক দ্বিগুণ* s1, ধ্রুবক দ্বিগুণ* s2, ধ্রুবক uchar* mask, পূর্ণসংখ্যা len, পূর্ণসংখ্যা cn) ধ্রুবক {
        দ্বিগুণ ফলাফল = 0.0;
        পূর্ণসংখ্যা i = 0;
#যদি CV_SIMD_64F || CV_SIMD_SCALABLE_64F
        যদি (cn == 1) {
            ধ্রুবক পূর্ণসংখ্যা vstep = VTraits<v_float64>::vlanes();
            v_float64 acc0 = vx_setzero_f64(), acc1 = vx_setzero_f64();
            জন্য (; i <= len - 2*vstep; i += 2*vstep) {
                v_uint64 m0, m1;
                v_normmask_u64(mask + i, m0, m1);
                v_float64 d0 = v_absdiff(vx_load(s1 + i), vx_load(s2 + i));
                v_float64 d1 = v_absdiff(vx_load(s1 + i + vstep), vx_load(s2 + i + vstep));
                acc0 = v_max(acc0, v_reinterpret_as_f64(v_and(v_reinterpret_as_u64(d0), m0)));
                acc1 = v_max(acc1, v_reinterpret_as_f64(v_and(v_reinterpret_as_u64(d1), m1)));
            }
            দ্বিগুণ t[VTraits<v_float64>::max_nlanes];
            vx_store(t, v_max(acc0, acc1));
            জন্য (পূর্ণসংখ্যা k = 0; k < vstep; k++)
                ফলাফল = std::max(ফলাফল, t[k]);
        }
#endif
        জন্য (; i < len; i++) যদি (mask[i]) {
            ধ্রুবক দ্বিগুণ* e1 = s1 + i*cn; ধ্রুবক দ্বিগুণ* e2 = s2 + i*cn;
            জন্য (পূর্ণসংখ্যা k = 0; k < cn; k++) ফলাফল = std::max(ফলাফল, std::abs(e1[k] - e2[k]));
        }
        ফেরত ফলাফল;
    }
};

template<>
কাঠামো MaskedNormDiffL1_SIMD<দ্বিগুণ, দ্বিগুণ> {
    দ্বিগুণ operator()(ধ্রুবক দ্বিগুণ* s1, ধ্রুবক দ্বিগুণ* s2, ধ্রুবক uchar* mask, পূর্ণসংখ্যা len, পূর্ণসংখ্যা cn) ধ্রুবক {
        দ্বিগুণ ফলাফল = 0.0;
        যদি (cn == 1) {
            ধ্রুবক পূর্ণসংখ্যা vstep = VTraits<v_float64>::vlanes();
            পূর্ণসংখ্যা i = 0;
            v_float64 acc0 = vx_setzero_f64(), acc1 = vx_setzero_f64();
            জন্য (; i <= len - 2*vstep; i += 2*vstep) {
                v_uint64 m0, m1;
                v_normmask_u64(mask + i, m0, m1);
                v_float64 d0 = v_absdiff(vx_load(s1 + i), vx_load(s2 + i));
                v_float64 d1 = v_absdiff(vx_load(s1 + i + vstep), vx_load(s2 + i + vstep));
                acc0 = v_add(acc0, v_reinterpret_as_f64(v_and(v_reinterpret_as_u64(d0), m0)));
                acc1 = v_add(acc1, v_reinterpret_as_f64(v_and(v_reinterpret_as_u64(d1), m1)));
            }
            ফলাফল = v_reduce_যোগ(v_add(acc0, acc1));
            জন্য (; i < len; i++) যদি (mask[i]) ফলাফল += std::abs(s1[i] - s2[i]);
        } অন্যথা {
            ফলাফল = masked_Norm_Diff_L1_লেজ<দ্বিগুণ, দ্বিগুণ>(s1, s2, mask, len, cn, 0.0);
        }
        ফেরত ফলাফল;
    }
};

template<>
কাঠামো MaskedNormDiffL2_SIMD<দ্বিগুণ, দ্বিগুণ> {
    দ্বিগুণ operator()(ধ্রুবক দ্বিগুণ* s1, ধ্রুবক দ্বিগুণ* s2, ধ্রুবক uchar* mask, পূর্ণসংখ্যা len, পূর্ণসংখ্যা cn) ধ্রুবক {
        দ্বিগুণ ফলাফল = 0.0;
        যদি (cn == 1) {
            ধ্রুবক পূর্ণসংখ্যা vstep = VTraits<v_float64>::vlanes();
            পূর্ণসংখ্যা i = 0;
            v_float64 acc0 = vx_setzero_f64(), acc1 = vx_setzero_f64();
            জন্য (; i <= len - 2*vstep; i += 2*vstep) {
                v_uint64 m0, m1;
                v_normmask_u64(mask + i, m0, m1);
                v_float64 d0 = v_sub(vx_load(s1 + i), vx_load(s2 + i));
                v_float64 d1 = v_sub(vx_load(s1 + i + vstep), vx_load(s2 + i + vstep));
                d0 = v_reinterpret_as_f64(v_and(v_reinterpret_as_u64(d0), m0));
                d1 = v_reinterpret_as_f64(v_and(v_reinterpret_as_u64(d1), m1));
                acc0 = v_fma(d0, d0, acc0);
                acc1 = v_fma(d1, d1, acc1);
            }
            ফলাফল = v_reduce_যোগ(v_add(acc0, acc1));
            জন্য (; i < len; i++) যদি (mask[i]) { দ্বিগুণ v = s1[i] - s2[i]; ফলাফল += v*v; }
        } অন্যথা {
            ফলাফল = masked_Norm_Diff_L2_লেজ<দ্বিগুণ, দ্বিগুণ>(s1, s2, mask, len, cn, 0.0);
        }
        ফেরত ফলাফল;
    }
};
#endif // CV_SIMD_64F
#endif // CV_SIMD

template<typename T, typename ST> পূর্ণসংখ্যা
normDiffInf_(ধ্রুবক T* src1, ধ্রুবক T* src2, ধ্রুবক uchar* mask, ST* ফলাফল_2, পূর্ণসংখ্যা len, পূর্ণসংখ্যা cn) {
    ST ফলাফল = *ফলাফল_2;
    যদি( !mask ) {
        NormDiffInf_SIMD<T, ST> op;
        ফলাফল = std::max(ফলাফল, op(src1, src2, len*cn));
    } অন্যথা {
        MaskedNormDiffInf_SIMD<T, ST> op;
        ফলাফল = std::max(ফলাফল, op(src1, src2, mask, len, cn));
    }
    *ফলাফল_2 = ফলাফল;
    ফেরত 0;
}

template<typename T, typename ST> পূর্ণসংখ্যা
normDiffL1_(ধ্রুবক T* src1, ধ্রুবক T* src2, ধ্রুবক uchar* mask, ST* ফলাফল_2, পূর্ণসংখ্যা len, পূর্ণসংখ্যা cn) {
    ST ফলাফল = *ফলাফল_2;
    যদি( !mask ) {
        NormDiffL1_SIMD<T, ST> op;
        ফলাফল += op(src1, src2, len*cn);
    } অন্যথা {
        MaskedNormDiffL1_SIMD<T, ST> op;
        ফলাফল += op(src1, src2, mask, len, cn);
    }
    *ফলাফল_2 = ফলাফল;
    ফেরত 0;
}

template<typename T, typename ST> পূর্ণসংখ্যা
normDiffL2_(ধ্রুবক T* src1, ধ্রুবক T* src2, ধ্রুবক uchar* mask, ST* ফলাফল_2, পূর্ণসংখ্যা len, পূর্ণসংখ্যা cn) {
    ST ফলাফল = *ফলাফল_2;
    যদি( !mask ) {
        NormDiffL2_SIMD<T, ST> op;
        ফলাফল += op(src1, src2, len*cn);
    } অন্যথা {
        MaskedNormDiffL2_SIMD<T, ST> op;
        ফলাফল += op(src1, src2, mask, len, cn);
    }
    *ফলাফল_2 = ফলাফল;
    ফেরত 0;
}

স্থির পূর্ণসংখ্যা
normDiffInf_Bool(ধ্রুবক uchar* src1, ধ্রুবক uchar* src2, ধ্রুবক uchar* mask, পূর্ণসংখ্যা* ফলাফল_2, পূর্ণসংখ্যা len, পূর্ণসংখ্যা cn)
{
    পূর্ণসংখ্যা ফলাফল = *ফলাফল_2;
    যদি( !mask )
    {
        জন্য( পূর্ণসংখ্যা i = 0; i < len*cn; i++ ) {
            ফলাফল = std::max(ফলাফল, (পূর্ণসংখ্যা)((src1[i] != 0) != (src2[i] != 0)));
            যদি (ফলাফল != 0)
                বিরতি;
        }
    }
    অন্যথা
    {
        জন্য( পূর্ণসংখ্যা i = 0; i < len; i++, src1 += cn, src2 += cn )
            যদি( mask[i] )
            {
                জন্য( পূর্ণসংখ্যা k = 0; k < cn; k++ )
                    ফলাফল = std::max(ফলাফল, (পূর্ণসংখ্যা)((src1[k] != 0) != (src2[k] != 0)));
                যদি (ফলাফল != 0)
                    বিরতি;
            }
    }
    *ফলাফল_2 = ফলাফল;
    ফেরত 0;
}

স্থির পূর্ণসংখ্যা
normDiffL1_Bool(ধ্রুবক uchar* src1, ধ্রুবক uchar* src2, ধ্রুবক uchar* mask, পূর্ণসংখ্যা* ফলাফল_2, পূর্ণসংখ্যা len, পূর্ণসংখ্যা cn)
{
    পূর্ণসংখ্যা ফলাফল = *ফলাফল_2;
    যদি( !mask )
    {
        জন্য( পূর্ণসংখ্যা i = 0; i < len*cn; i++ )
            ফলাফল += (পূর্ণসংখ্যা)((src1[i] != 0) != (src2[i] != 0));
    }
    অন্যথা
    {
        জন্য( পূর্ণসংখ্যা i = 0; i < len; i++, src1 += cn, src2 += cn )
            যদি( mask[i] )
            {
                জন্য( পূর্ণসংখ্যা k = 0; k < cn; k++ )
                    ফলাফল += (পূর্ণসংখ্যা)((src1[k] != 0) != (src2[k] != 0));
            }
    }
    *ফলাফল_2 = ফলাফল;
    ফেরত 0;
}

স্থির পূর্ণসংখ্যা
normDiffL2_Bool(ধ্রুবক uchar* src1, ধ্রুবক uchar* src2, ধ্রুবক uchar* mask, পূর্ণসংখ্যা* ফলাফল_2, পূর্ণসংখ্যা len, পূর্ণসংখ্যা cn)
{
    ফেরত normDiffL1_Bool(src1, src2, mask, ফলাফল_2, len, cn);
}

#define CV_DEF_NORM_FUNC(L, suffix, type, ntype) \
    স্থির পূর্ণসংখ্যা norm##L##_##suffix(ধ্রুবক type* src, ধ্রুবক uchar* mask, ntype* r, পূর্ণসংখ্যা len, পূর্ণসংখ্যা cn) \
{ CV_INSTRUMENT_REGION(); ফেরত norm##L##_(src, mask, r, len, cn); } \
    স্থির পূর্ণসংখ্যা normDiff##L##_##suffix(ধ্রুবক type* src1, ধ্রুবক type* src2, \
    ধ্রুবক uchar* mask, ntype* r, পূর্ণসংখ্যা len, পূর্ণসংখ্যা cn) \
{ ফেরত normDiff##L##_(src1, src2, mask, r, (পূর্ণসংখ্যা)len, cn); }

#define CV_DEF_NORM_ALL(suffix, type, inftype, l1type, l2type) \
    CV_DEF_NORM_FUNC(Inf, suffix, type, inftype) \
    CV_DEF_NORM_FUNC(L1, suffix, type, l1type) \
    CV_DEF_NORM_FUNC(L2, suffix, type, l2type)

CV_DEF_NORM_ALL(8u, uchar, পূর্ণসংখ্যা, পূর্ণসংখ্যা, পূর্ণসংখ্যা)
CV_DEF_NORM_ALL(8s, schar, পূর্ণসংখ্যা, পূর্ণসংখ্যা, পূর্ণসংখ্যা)
CV_DEF_NORM_ALL(16u, ushort, পূর্ণসংখ্যা, পূর্ণসংখ্যা, দ্বিগুণ)
CV_DEF_NORM_ALL(16s, short, পূর্ণসংখ্যা, পূর্ণসংখ্যা, দ্বিগুণ)
CV_DEF_NORM_ALL(32u, unsigned, unsigned, দ্বিগুণ, দ্বিগুণ)
CV_DEF_NORM_ALL(32s, পূর্ণসংখ্যা, unsigned, দ্বিগুণ, দ্বিগুণ)
CV_DEF_NORM_ALL(32f, দশমিক, দশমিক, দ্বিগুণ, দ্বিগুণ)
CV_DEF_NORM_ALL(64f, দ্বিগুণ, দ্বিগুণ, দ্বিগুণ, দ্বিগুণ)
CV_DEF_NORM_ALL(64u, uint64, uint64, দ্বিগুণ, দ্বিগুণ)
CV_DEF_NORM_ALL(64s, int64, uint64, দ্বিগুণ, দ্বিগুণ)
CV_DEF_NORM_ALL(16f, hfloat, দশমিক, দশমিক, দশমিক)
CV_DEF_NORM_ALL(16bf, bfloat, দশমিক, দশমিক, দশমিক)

NormFunc getNormFunc(পূর্ণসংখ্যা normType, পূর্ণসংখ্যা depth)
{
    CV_INSTRUMENT_REGION();

    স্থির NormFunc normTab[3][CV_DEPTH_MAX] =
    {
        {
            normWrap<uchar, পূর্ণসংখ্যা, normInf_8u>,
            normWrap<schar, পূর্ণসংখ্যা, normInf_8s>,
            normWrap<ushort, পূর্ণসংখ্যা, normInf_16u>,
            normWrap<short, পূর্ণসংখ্যা, normInf_16s>,
            normWrap<পূর্ণসংখ্যা, unsigned, normInf_32s>,
            normWrap<দশমিক, দশমিক, normInf_32f>,
            normWrap<দ্বিগুণ, দ্বিগুণ, normInf_64f>,
            normWrap<hfloat, দশমিক, normInf_16f>,
            normWrap<bfloat, দশমিক, normInf_16bf>,
            normWrap<uchar, পূর্ণসংখ্যা, normInf_Bool>,
            normWrap<uint64, uint64, normInf_64u>,
            normWrap<int64, uint64, normInf_64s>,
            normWrap<unsigned, unsigned, normInf_32u>,
            0
        },
        {
            normWrap<uchar, পূর্ণসংখ্যা, normL1_8u>,
            normWrap<schar, পূর্ণসংখ্যা, normL1_8s>,
            normWrap<ushort, পূর্ণসংখ্যা, normL1_16u>,
            normWrap<short, পূর্ণসংখ্যা, normL1_16s>,
            normWrap<পূর্ণসংখ্যা, দ্বিগুণ, normL1_32s>,
            normWrap<দশমিক, দ্বিগুণ, normL1_32f>,
            normWrap<দ্বিগুণ, দ্বিগুণ, normL1_64f>,
            normWrap<hfloat, দশমিক, normL1_16f>,
            normWrap<bfloat, দশমিক, normL1_16bf>,
            normWrap<uchar, পূর্ণসংখ্যা, normL1_Bool>,
            normWrap<uint64, দ্বিগুণ, normL1_64u>,
            normWrap<int64, দ্বিগুণ, normL1_64s>,
            normWrap<unsigned, দ্বিগুণ, normL1_32u>,
            0
        },
        {
            normWrap<uchar, পূর্ণসংখ্যা, normL2_8u>,
            normWrap<schar, পূর্ণসংখ্যা, normL2_8s>,
            normWrap<ushort, দ্বিগুণ, normL2_16u>,
            normWrap<short, দ্বিগুণ, normL2_16s>,
            normWrap<পূর্ণসংখ্যা, দ্বিগুণ, normL2_32s>,
            normWrap<দশমিক, দ্বিগুণ, normL2_32f>,
            normWrap<দ্বিগুণ, দ্বিগুণ, normL2_64f>,
            normWrap<hfloat, দশমিক, normL2_16f>,
            normWrap<bfloat, দশমিক, normL2_16bf>,
            normWrap<uchar, পূর্ণসংখ্যা, normL2_Bool>,
            normWrap<uint64, দ্বিগুণ, normL2_64u>,
            normWrap<int64, দ্বিগুণ, normL2_64s>,
            normWrap<unsigned, দ্বিগুণ, normL2_32u>,
            0
        }
    };

    যদি (normType >= 3 || normType < 0) ফেরত nullptr;

    ফেরত normTab[normType][depth];
}

NormDiffFunc getNormDiffFunc(পূর্ণসংখ্যা normType, পূর্ণসংখ্যা depth)
{
    স্থির NormDiffFunc normDiffTab[3][CV_DEPTH_MAX] =
    {
        {
            normDiffWrap<uchar, পূর্ণসংখ্যা, normDiffInf_8u>,
            normDiffWrap<schar, পূর্ণসংখ্যা, normDiffInf_8s>,
            normDiffWrap<ushort, পূর্ণসংখ্যা, normDiffInf_16u>,
            normDiffWrap<short, পূর্ণসংখ্যা, normDiffInf_16s>,
            normDiffWrap<পূর্ণসংখ্যা, unsigned, normDiffInf_32s>,
            normDiffWrap<দশমিক, দশমিক, normDiffInf_32f>,
            normDiffWrap<দ্বিগুণ, দ্বিগুণ, normDiffInf_64f>,
            normDiffWrap<hfloat, দশমিক, normDiffInf_16f>,
            normDiffWrap<bfloat, দশমিক, normDiffInf_16bf>,
            normDiffWrap<uchar, পূর্ণসংখ্যা, normDiffInf_Bool>,
            normDiffWrap<uint64, uint64, normDiffInf_64u>,
            normDiffWrap<int64, uint64, normDiffInf_64s>,
            normDiffWrap<unsigned, unsigned, normDiffInf_32u>,
            0
        },
        {
            normDiffWrap<uchar, পূর্ণসংখ্যা, normDiffL1_8u>,
            normDiffWrap<schar, পূর্ণসংখ্যা, normDiffL1_8s>,
            normDiffWrap<ushort, পূর্ণসংখ্যা, normDiffL1_16u>,
            normDiffWrap<short, পূর্ণসংখ্যা, normDiffL1_16s>,
            normDiffWrap<পূর্ণসংখ্যা, দ্বিগুণ, normDiffL1_32s>,
            normDiffWrap<দশমিক, দ্বিগুণ, normDiffL1_32f>,
            normDiffWrap<দ্বিগুণ, দ্বিগুণ, normDiffL1_64f>,
            normDiffWrap<hfloat, দশমিক, normDiffL1_16f>,
            normDiffWrap<bfloat, দশমিক, normDiffL1_16bf>,
            normDiffWrap<uchar, পূর্ণসংখ্যা, normDiffL1_Bool>,
            normDiffWrap<uint64, দ্বিগুণ, normDiffL1_64u>,
            normDiffWrap<int64, দ্বিগুণ, normDiffL1_64s>,
            normDiffWrap<unsigned, দ্বিগুণ, normDiffL1_32u>,
            0
        },
        {
            normDiffWrap<uchar, পূর্ণসংখ্যা, normDiffL2_8u>,
            normDiffWrap<schar, পূর্ণসংখ্যা, normDiffL2_8s>,
            normDiffWrap<ushort, দ্বিগুণ, normDiffL2_16u>,
            normDiffWrap<short, দ্বিগুণ, normDiffL2_16s>,
            normDiffWrap<পূর্ণসংখ্যা, দ্বিগুণ, normDiffL2_32s>,
            normDiffWrap<দশমিক, দ্বিগুণ, normDiffL2_32f>,
            normDiffWrap<দ্বিগুণ, দ্বিগুণ, normDiffL2_64f>,
            normDiffWrap<hfloat, দশমিক, normDiffL2_16f>,
            normDiffWrap<bfloat, দশমিক, normDiffL2_16bf>,
            normDiffWrap<uchar, পূর্ণসংখ্যা, normDiffL2_Bool>,
            normDiffWrap<uint64, দ্বিগুণ, normDiffL2_64u>,
            normDiffWrap<int64, দ্বিগুণ, normDiffL2_64s>,
            normDiffWrap<unsigned, দ্বিগুণ, normDiffL2_32u>,
            0
        },
    };
    যদি (normType >= 3 || normType < 0) ফেরত nullptr;

    ফেরত normDiffTab[normType][depth];
}

#endif // CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

CV_CPU_OPTIMIZATION_NAMESPACE_END

} // cv::
