// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html
//
// Copyright (C) 2025, SpaceMIT Inc., all rights reserved.
// Copyright (C) 2026, Advanced Micro Devices, Inc., all rights reserved.
// Third party copyrights are property of their respective owners.

#అన్తర్భావ "precomp.hpp"

నామాకాశ cv {

ఉపయోగించి NormFunc = పూర్ణాంకం (*)(స్థిరరాశి uchar*, స్థిరరాశి uchar*, uchar*, పూర్ణాంకం, పూర్ణాంకం);
ఉపయోగించి NormDiffFunc = పూర్ణాంకం (*)(స్థిరరాశి uchar*, స్థిరరాశి uchar*, స్థిరరాశి uchar*, uchar*, పూర్ణాంకం, పూర్ణాంకం);

CV_CPU_OPTIMIZATION_NAMESPACE_BEGIN

మూస<రకం_పేరు T, రకం_పేరు RT, పూర్ణాంకం (*fn)(స్థిరరాశి T*, స్థిరరాశి uchar*, RT*, పూర్ణాంకం, పూర్ణాంకం)>
స్థిర పూర్ణాంకం normWrap(స్థిరరాశి uchar* src1, స్థిరరాశి uchar* src2, uchar* res, పూర్ణాంకం len, పూర్ణాంకం cn)
{
    తిరిగి fn((స్థిరరాశి T*)src1, src2, (RT*)res, len, cn);
}

మూస<రకం_పేరు T, రకం_పేరు RT, పూర్ణాంకం (*fn)(స్థిరరాశి T*, స్థిరరాశి T*, స్థిరరాశి uchar*, RT*, పూర్ణాంకం, పూర్ణాంకం)>
స్థిర పూర్ణాంకం normDiffWrap(స్థిరరాశి uchar* src1, స్థిరరాశి uchar* src2, స్థిరరాశి uchar* src3, uchar* res, పూర్ణాంకం len, పూర్ణాంకం cn)
{
    తిరిగి fn((స్థిరరాశి T*)src1, (స్థిరరాశి T*)src2, src3, (RT*)res, len, cn);
}

NormFunc getNormFunc(పూర్ణాంకం normType, పూర్ణాంకం లోతు);
NormDiffFunc getNormDiffFunc(పూర్ణాంకం normType, పూర్ణాంకం లోతు);

#నిర్వచితం_కాకపోతే CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

మూస <రకం_పేరు T, రకం_పేరు ST>
నిర్మాణం NormInf_SIMD {
    అంతఃవరుస ST క్రియాకారకం() (స్థిరరాశి T* src, పూర్ణాంకం n) స్థిరరాశి {
        ST s = 0;
        కొరకు (పూర్ణాంకం i = 0; i < n; i++) {
            s = std::max(s, (ST)cv_abs(src[i]));
        }
        తిరిగి s;
    }
};

మూస <రకం_పేరు T, రకం_పేరు ST>
నిర్మాణం NormL1_SIMD {
    అంతఃవరుస ST క్రియాకారకం() (స్థిరరాశి T* src, పూర్ణాంకం n) స్థిరరాశి {
        ST s = 0;
        కొరకు (పూర్ణాంకం i = 0; i < n; i++) {
            s += cv_abs(src[i]);
        }
        తిరిగి s;
    }
};

మూస <రకం_పేరు T, రకం_పేరు ST>
నిర్మాణం NormL2_SIMD {
    అంతఃవరుస ST క్రియాకారకం() (స్థిరరాశి T* src, పూర్ణాంకం n) స్థిరరాశి {
        ST s = 0;
        కొరకు (పూర్ణాంకం i = 0; i < n; i++) {
            ST v = (ST)src[i];
            s += v * v;
        }
        తిరిగి s;
    }
};

మూస <రకం_పేరు T, రకం_పేరు ST>
నిర్మాణం NormDiffInf_SIMD {
    అంతఃవరుస ST క్రియాకారకం() (స్థిరరాశి T* src1, స్థిరరాశి T* src2, పూర్ణాంకం n) స్థిరరాశి {
        ST s = 0;
        కొరకు (పూర్ణాంకం i = 0; i < n; i++) {
            ST v = (ST)cv_absdiff(src1[i], src2[i]);
            s = std::max(s, v);
        }
        తిరిగి s;
    }
};

మూస <రకం_పేరు T, రకం_పేరు ST>
నిర్మాణం NormDiffL1_SIMD {
    అంతఃవరుస ST క్రియాకారకం() (స్థిరరాశి T* src1, స్థిరరాశి T* src2, పూర్ణాంకం n) స్థిరరాశి {
        ST s = 0;
        కొరకు (పూర్ణాంకం i = 0; i < n; i++) {
            ST v = (ST)cv_absdiff(src1[i], src2[i]);
            s += v;
        }
        తిరిగి s;
    }
};

మూస <రకం_పేరు T, రకం_పేరు ST>
నిర్మాణం NormDiffL2_SIMD {
    అంతఃవరుస ST క్రియాకారకం() (స్థిరరాశి T* src1, స్థిరరాశి T* src2, పూర్ణాంకం n) స్థిరరాశి {
        ST s = 0;
        కొరకు (పూర్ణాంకం i = 0; i < n; i++) {
            ST v = (ST)src1[i] - (ST)src2[i];
            s += v * v;
        }
        తిరిగి s;
    }
};

#ఒకవేళ (CV_SIMD || CV_SIMD_SCALABLE)

మూస<>
నిర్మాణం NormInf_SIMD<uchar, పూర్ణాంకం> {
    పూర్ణాంకం క్రియాకారకం() (స్థిరరాశి uchar* src, పూర్ణాంకం n) స్థిరరాశి {
        పూర్ణాంకం j = 0;
        పూర్ణాంకం s = 0;
        v_uint8 r0 = vx_setzero_u8(), r1 = vx_setzero_u8();
        v_uint8 r2 = vx_setzero_u8(), r3 = vx_setzero_u8();
        కొరకు (; j <= n - 4 * VTraits<v_uint8>::vlanes(); j += 4 * VTraits<v_uint8>::vlanes()) {
            r0 = v_max(r0, vx_ఎక్కించు(src + j                                 ));
            r1 = v_max(r1, vx_ఎక్కించు(src + j +     VTraits<v_uint8>::vlanes()));
            r2 = v_max(r2, vx_ఎక్కించు(src + j + 2 * VTraits<v_uint8>::vlanes()));
            r3 = v_max(r3, vx_ఎక్కించు(src + j + 3 * VTraits<v_uint8>::vlanes()));
        }
        r0 = v_max(r0, v_max(r1, v_max(r2, r3)));
        కొరకు (; j < n; j++) {
            s = std::max(s, (పూర్ణాంకం)src[j]);
        }
        తిరిగి std::max(s, (పూర్ణాంకం)v_reduce_max(r0));
    }
};

మూస<>
నిర్మాణం NormInf_SIMD<schar, పూర్ణాంకం> {
    పూర్ణాంకం క్రియాకారకం() (స్థిరరాశి schar* src, పూర్ణాంకం n) స్థిరరాశి {
        పూర్ణాంకం j = 0;
        పూర్ణాంకం s = 0;
        v_uint8 r0 = vx_setzero_u8(), r1 = vx_setzero_u8();
        v_uint8 r2 = vx_setzero_u8(), r3 = vx_setzero_u8();
        కొరకు (; j <= n - 4 * VTraits<v_int8>::vlanes(); j += 4 * VTraits<v_int8>::vlanes()) {
            r0 = v_max(r0, v_abs(vx_ఎక్కించు(src + j                                )));
            r1 = v_max(r1, v_abs(vx_ఎక్కించు(src + j +     VTraits<v_int8>::vlanes())));
            r2 = v_max(r2, v_abs(vx_ఎక్కించు(src + j + 2 * VTraits<v_int8>::vlanes())));
            r3 = v_max(r3, v_abs(vx_ఎక్కించు(src + j + 3 * VTraits<v_int8>::vlanes())));
        }
        r0 = v_max(r0, v_max(r1, v_max(r2, r3)));
        కొరకు (; j < n; j++) {
            s = std::max(s, cv_abs(src[j]));
        }
        తిరిగి std::max(s, saturate_cast<పూర్ణాంకం>(v_reduce_max(r0)));
    }
};

మూస<>
నిర్మాణం NormInf_SIMD<ushort, పూర్ణాంకం> {
    పూర్ణాంకం క్రియాకారకం() (స్థిరరాశి ushort* src, పూర్ణాంకం n) స్థిరరాశి {
        పూర్ణాంకం j = 0;
        పూర్ణాంకం s = 0;
        v_uint16 d0 = vx_setzero_u16(), d1 = vx_setzero_u16();
        v_uint16 d2 = vx_setzero_u16(), d3 = vx_setzero_u16();
        కొరకు (; j <= n - 4 * VTraits<v_uint16>::vlanes(); j += 4 * VTraits<v_uint16>::vlanes()) {
            d0 = v_max(d0, vx_ఎక్కించు(src + j                                  ));
            d1 = v_max(d1, vx_ఎక్కించు(src + j +     VTraits<v_uint16>::vlanes()));
            d2 = v_max(d2, vx_ఎక్కించు(src + j + 2 * VTraits<v_uint16>::vlanes()));
            d3 = v_max(d3, vx_ఎక్కించు(src + j + 3 * VTraits<v_uint16>::vlanes()));
        }
        d0 = v_max(d0, v_max(d1, v_max(d2, d3)));
        కొరకు (; j < n; j++) {
            s = std::max(s, (పూర్ణాంకం)src[j]);
        }
        తిరిగి std::max(s, (పూర్ణాంకం)v_reduce_max(d0));
    }
};

మూస<>
నిర్మాణం NormInf_SIMD<పొట్టి, పూర్ణాంకం> {
    పూర్ణాంకం క్రియాకారకం() (స్థిరరాశి పొట్టి* src, పూర్ణాంకం n) స్థిరరాశి {
        పూర్ణాంకం j = 0;
        పూర్ణాంకం s = 0;
        v_uint16 d0 = vx_setzero_u16(), d1 = vx_setzero_u16();
        v_uint16 d2 = vx_setzero_u16(), d3 = vx_setzero_u16();
        కొరకు (; j <= n - 4 * VTraits<v_int16>::vlanes(); j += 4 * VTraits<v_int16>::vlanes()) {
            d0 = v_max(d0, v_abs(vx_ఎక్కించు(src + j                                  )));
            d1 = v_max(d1, v_abs(vx_ఎక్కించు(src + j +     VTraits<v_int16>::vlanes())));
            d2 = v_max(d2, v_abs(vx_ఎక్కించు(src + j + 2 * VTraits<v_int16>::vlanes())));
            d3 = v_max(d3, v_abs(vx_ఎక్కించు(src + j + 3 * VTraits<v_int16>::vlanes())));
        }
        d0 = v_max(d0, v_max(d1, v_max(d2, d3)));
        కొరకు (; j < n; j++) {
            s = std::max(s, saturate_cast<పూర్ణాంకం>(cv_abs(src[j])));
        }
        తిరిగి std::max(s, saturate_cast<పూర్ణాంకం>(v_reduce_max(d0)));
    }
};

మూస<>
నిర్మాణం NormInf_SIMD<పూర్ణాంకం, పూర్ణాంకం> {
    పూర్ణాంకం క్రియాకారకం() (స్థిరరాశి పూర్ణాంకం* src, పూర్ణాంకం n) స్థిరరాశి {
        పూర్ణాంకం j = 0;
        పూర్ణాంకం s = 0;
        v_uint32 r0 = vx_setzero_u32(), r1 = vx_setzero_u32();
        v_uint32 r2 = vx_setzero_u32(), r3 = vx_setzero_u32();
        కొరకు (; j <= n - 4 * VTraits<v_int32>::vlanes(); j += 4 * VTraits<v_int32>::vlanes()) {
            r0 = v_max(r0, v_abs(vx_ఎక్కించు(src + j                                 )));
            r1 = v_max(r1, v_abs(vx_ఎక్కించు(src + j +     VTraits<v_int32>::vlanes())));
            r2 = v_max(r2, v_abs(vx_ఎక్కించు(src + j + 2 * VTraits<v_int32>::vlanes())));
            r3 = v_max(r3, v_abs(vx_ఎక్కించు(src + j + 3 * VTraits<v_int32>::vlanes())));
        }
        r0 = v_max(r0, v_max(r1, v_max(r2, r3)));
        s = std::max(s, saturate_cast<పూర్ణాంకం>(v_reduce_max(r0)));
        కొరకు (; j < n; j++) {
            s = std::max(s, std::నిరపేక్ష(src[j]));
        }
        తిరిగి s;
    }
};

మూస<>
నిర్మాణం NormInf_SIMD<దశాంశం, దశాంశం> {
    దశాంశం క్రియాకారకం() (స్థిరరాశి దశాంశం* src, పూర్ణాంకం n) స్థిరరాశి {
        పూర్ణాంకం j = 0;
        దశాంశం s = 0.f;
        v_float32 r0 = vx_setzero_f32(), r1 = vx_setzero_f32();
        v_float32 r2 = vx_setzero_f32(), r3 = vx_setzero_f32();
        కొరకు (; j <= n - 4 * VTraits<v_float32>::vlanes(); j += 4 * VTraits<v_float32>::vlanes()) {
            r0 = v_max(r0, v_abs(vx_ఎక్కించు(src + j                                   )));
            r1 = v_max(r1, v_abs(vx_ఎక్కించు(src + j +     VTraits<v_float32>::vlanes())));
            r2 = v_max(r2, v_abs(vx_ఎక్కించు(src + j + 2 * VTraits<v_float32>::vlanes())));
            r3 = v_max(r3, v_abs(vx_ఎక్కించు(src + j + 3 * VTraits<v_float32>::vlanes())));
        }
        r0 = v_max(r0, v_max(r1, v_max(r2, r3)));
        కొరకు (; j < n; j++) {
            s = std::max(s, cv_abs(src[j]));
        }
        తిరిగి std::max(s, v_reduce_max(r0));
    }
};

మూస<>
నిర్మాణం NormL1_SIMD<uchar, పూర్ణాంకం> {
    పూర్ణాంకం క్రియాకారకం() (స్థిరరాశి uchar* src, పూర్ణాంకం n) స్థిరరాశి {
        పూర్ణాంకం j = 0;
        పూర్ణాంకం s = 0;
        v_uint32 r0 = vx_setzero_u32(), r1 = vx_setzero_u32();
        v_uint8  one = vx_setall_u8(1);
        కొరకు (; j<= n - 2 * VTraits<v_uint8>::vlanes(); j += 2 * VTraits<v_uint8>::vlanes()) {
            v_uint8 v0 = vx_ఎక్కించు(src + j);
            r0 = v_dotprod_expand_fast(v0, one, r0);

            v_uint8 v1 = vx_ఎక్కించు(src + j + VTraits<v_uint8>::vlanes());
            r1 = v_dotprod_expand_fast(v1, one, r1);
        }
        s += v_reduce_కూడిక(v_కూడు(r0, r1));
        కొరకు (; j < n; j++) {
            s += src[j];
        }
        తిరిగి s;
    }
};

మూస<>
నిర్మాణం NormL1_SIMD<schar, పూర్ణాంకం> {
    పూర్ణాంకం క్రియాకారకం() (స్థిరరాశి schar* src, పూర్ణాంకం n) స్థిరరాశి {
        పూర్ణాంకం j = 0;
        పూర్ణాంకం s = 0;
        v_uint32 r0 = vx_setzero_u32(), r1 = vx_setzero_u32();
        v_uint8  one = vx_setall_u8(1);
        కొరకు (; j<= n - 2 * VTraits<v_int8>::vlanes(); j += 2 * VTraits<v_int8>::vlanes()) {
            v_uint8 v0 = v_abs(vx_ఎక్కించు(src + j));
            r0 = v_dotprod_expand_fast(v0, one, r0);

            v_uint8 v1 = v_abs(vx_ఎక్కించు(src + j + VTraits<v_int8>::vlanes()));
            r1 = v_dotprod_expand_fast(v1, one, r1);
        }
        s += v_reduce_కూడిక(v_కూడు(r0, r1));
        కొరకు (; j < n; j++) {
            s += saturate_cast<పూర్ణాంకం>(cv_abs(src[j]));
        }
        తిరిగి s;
    }
};

మూస<>
నిర్మాణం NormL1_SIMD<ushort, పూర్ణాంకం> {
    పూర్ణాంకం క్రియాకారకం() (స్థిరరాశి ushort* src, పూర్ణాంకం n) స్థిరరాశి {
        పూర్ణాంకం j = 0;
        పూర్ణాంకం s = 0;
        v_uint32 r00 = vx_setzero_u32(), r01 = vx_setzero_u32();
        v_uint32 r10 = vx_setzero_u32(), r11 = vx_setzero_u32();
        కొరకు (; j<= n - 2 * VTraits<v_uint16>::vlanes(); j += 2 * VTraits<v_uint16>::vlanes()) {
            v_uint16 v0 = vx_ఎక్కించు(src + j);
            v_uint32 v00, v01;
            v_expand(v0, v00, v01);
            r00 = v_కూడు(r00, v00);
            r01 = v_కూడు(r01, v01);

            v_uint16 v1 = vx_ఎక్కించు(src + j + VTraits<v_uint16>::vlanes());
            v_uint32 v10, v11;
            v_expand(v1, v10, v11);
            r10 = v_కూడు(r10, v10);
            r11 = v_కూడు(r11, v11);
        }
        s += (పూర్ణాంకం)v_reduce_కూడిక(v_కూడు(v_కూడు(v_కూడు(r00, r01), r10), r11));
        కొరకు (; j < n; j++) {
            s += src[j];
        }
        తిరిగి s;
    }
};

మూస<>
నిర్మాణం NormL1_SIMD<పొట్టి, పూర్ణాంకం> {
    పూర్ణాంకం క్రియాకారకం() (స్థిరరాశి పొట్టి* src, పూర్ణాంకం n) స్థిరరాశి {
        పూర్ణాంకం j = 0;
        పూర్ణాంకం s = 0;
        v_uint32 r00 = vx_setzero_u32(), r01 = vx_setzero_u32();
        v_uint32 r10 = vx_setzero_u32(), r11 = vx_setzero_u32();
        కొరకు (; j<= n - 2 * VTraits<v_int16>::vlanes(); j += 2 * VTraits<v_int16>::vlanes()) {
            v_uint16 v0 = v_abs(vx_ఎక్కించు(src + j));
            v_uint32 v00, v01;
            v_expand(v0, v00, v01);
            r00 = v_కూడు(r00, v00);
            r01 = v_కూడు(r01, v01);

            v_uint16 v1 = v_abs(vx_ఎక్కించు(src + j + VTraits<v_int16>::vlanes()));
            v_uint32 v10, v11;
            v_expand(v1, v10, v11);
            r10 = v_కూడు(r10, v10);
            r11 = v_కూడు(r11, v11);
        }
        s += (పూర్ణాంకం)v_reduce_కూడిక(v_కూడు(v_కూడు(v_కూడు(r00, r01), r10), r11));
        కొరకు (; j < n; j++) {
            s += saturate_cast<పూర్ణాంకం>(cv_abs(src[j]));
        }
        తిరిగి s;
    }
};

మూస<>
నిర్మాణం NormL2_SIMD<uchar, పూర్ణాంకం> {
    పూర్ణాంకం క్రియాకారకం() (స్థిరరాశి uchar* src, పూర్ణాంకం n) స్థిరరాశి {
        పూర్ణాంకం j = 0;
        పూర్ణాంకం s = 0;
        v_uint32 r0 = vx_setzero_u32(), r1 = vx_setzero_u32();
        కొరకు (; j <= n - 2 * VTraits<v_uint8>::vlanes(); j += 2 * VTraits<v_uint8>::vlanes()) {
            v_uint8 v0 = vx_ఎక్కించు(src + j);
            r0 = v_dotprod_expand_fast(v0, v0, r0);

            v_uint8 v1 = vx_ఎక్కించు(src + j + VTraits<v_uint8>::vlanes());
            r1 = v_dotprod_expand_fast(v1, v1, r1);
        }
        s += v_reduce_కూడిక(v_కూడు(r0, r1));
        కొరకు (; j < n; j++) {
            పూర్ణాంకం v = saturate_cast<పూర్ణాంకం>(src[j]);
            s += v * v;
        }
        తిరిగి s;
    }
};

మూస<>
నిర్మాణం NormL2_SIMD<schar, పూర్ణాంకం> {
    పూర్ణాంకం క్రియాకారకం() (స్థిరరాశి schar* src, పూర్ణాంకం n) స్థిరరాశి {
        పూర్ణాంకం j = 0;
        పూర్ణాంకం s = 0;
        v_int32 r0 = vx_setzero_s32(), r1 = vx_setzero_s32();
        కొరకు (; j <= n - 2 * VTraits<v_int8>::vlanes(); j += 2 * VTraits<v_int8>::vlanes()) {
            v_int8 v0 = vx_ఎక్కించు(src + j);
            r0 = v_dotprod_expand_fast(v0, v0, r0);
            v_int8 v1 = vx_ఎక్కించు(src + j + VTraits<v_int8>::vlanes());
            r1 = v_dotprod_expand_fast(v1, v1, r1);
        }
        s += v_reduce_కూడిక(v_కూడు(r0, r1));
        కొరకు (; j < n; j++) {
            పూర్ణాంకం v = saturate_cast<పూర్ణాంకం>(src[j]);
            s += v * v;
        }
        తిరిగి s;
    }
};

మూస<>
నిర్మాణం NormDiffInf_SIMD<uchar, పూర్ణాంకం> {
    పూర్ణాంకం క్రియాకారకం() (స్థిరరాశి uchar* src1, స్థిరరాశి uchar* src2, పూర్ణాంకం n) స్థిరరాశి {
        పూర్ణాంకం j = 0;
        పూర్ణాంకం s = 0;
        v_uint8 r0 = vx_setzero_u8(), r1 = vx_setzero_u8();
        v_uint8 r2 = vx_setzero_u8(), r3 = vx_setzero_u8();
        కొరకు (; j <= n - 4 * VTraits<v_uint8>::vlanes(); j += 4 * VTraits<v_uint8>::vlanes()) {
            v_uint8 v01 = vx_ఎక్కించు(src1 + j), v02 = vx_ఎక్కించు(src2 + j);
            r0 = v_max(r0, v_absdiff(v01, v02));

            v_uint8 v11 = vx_ఎక్కించు(src1 + j + VTraits<v_uint8>::vlanes()),
                    v12 = vx_ఎక్కించు(src2 + j + VTraits<v_uint8>::vlanes());
            r1 = v_max(r1, v_absdiff(v11, v12));

            v_uint8 v21 = vx_ఎక్కించు(src1 + j + 2 * VTraits<v_uint8>::vlanes()),
                    v22 = vx_ఎక్కించు(src2 + j + 2 * VTraits<v_uint8>::vlanes());
            r2 = v_max(r2, v_absdiff(v21, v22));

            v_uint8 v31 = vx_ఎక్కించు(src1 + j + 3 * VTraits<v_uint8>::vlanes()),
                    v32 = vx_ఎక్కించు(src2 + j + 3 * VTraits<v_uint8>::vlanes());
            r3 = v_max(r3, v_absdiff(v31, v32));
        }
        s = (పూర్ణాంకం)v_reduce_max(v_max(v_max(v_max(r0, r1), r2), r3));
        కొరకు (; j < n; j++) {
            పూర్ణాంకం v = (పూర్ణాంకం)cv_absdiff(src1[j], src2[j]);
            s = std::max(s, v);
        }
        తిరిగి s;
    }
};

మూస<>
నిర్మాణం NormDiffInf_SIMD<schar, పూర్ణాంకం> {
    పూర్ణాంకం క్రియాకారకం() (స్థిరరాశి schar* src1, స్థిరరాశి schar* src2, పూర్ణాంకం n) స్థిరరాశి {
        పూర్ణాంకం j = 0;
        పూర్ణాంకం s = 0;
        v_uint8 r0 = vx_setzero_u8(), r1 = vx_setzero_u8();
        v_uint8 r2 = vx_setzero_u8(), r3 = vx_setzero_u8();
        కొరకు (; j <= n - 4 * VTraits<v_int8>::vlanes(); j += 4 * VTraits<v_int8>::vlanes()) {
            v_int8 v01 = vx_ఎక్కించు(src1 + j), v02 = vx_ఎక్కించు(src2 + j);
            r0 = v_max(r0, v_absdiff(v01, v02));

            v_int8 v11 = vx_ఎక్కించు(src1 + j + VTraits<v_int8>::vlanes()),
                   v12 = vx_ఎక్కించు(src2 + j + VTraits<v_int8>::vlanes());
            r1 = v_max(r1, v_absdiff(v11, v12));

            v_int8 v21 = vx_ఎక్కించు(src1 + j + 2 * VTraits<v_int8>::vlanes()),
                   v22 = vx_ఎక్కించు(src2 + j + 2 * VTraits<v_int8>::vlanes());
            r2 = v_max(r2, v_absdiff(v21, v22));

            v_int8 v31 = vx_ఎక్కించు(src1 + j + 3 * VTraits<v_int8>::vlanes()),
                   v32 = vx_ఎక్కించు(src2 + j + 3 * VTraits<v_int8>::vlanes());
            r3 = v_max(r3, v_absdiff(v31, v32));
        }
        s = (పూర్ణాంకం)v_reduce_max(v_max(v_max(v_max(r0, r1), r2), r3));
        కొరకు (; j < n; j++) {
            పూర్ణాంకం v = (పూర్ణాంకం)cv_absdiff(src1[j], src2[j]);
            s = std::max(s, v);
        }
        తిరిగి s;
    }
};

మూస<>
నిర్మాణం NormDiffInf_SIMD<ushort, పూర్ణాంకం> {
    పూర్ణాంకం క్రియాకారకం() (స్థిరరాశి ushort* src1, స్థిరరాశి ushort* src2, పూర్ణాంకం n) స్థిరరాశి {
        పూర్ణాంకం j = 0;
        పూర్ణాంకం s = 0;
        v_uint16 r0 = vx_setzero_u16(), r1 = vx_setzero_u16();
        v_uint16 r2 = vx_setzero_u16(), r3 = vx_setzero_u16();
        కొరకు (; j <= n - 4 * VTraits<v_uint16>::vlanes(); j += 4 * VTraits<v_uint16>::vlanes()) {
            v_uint16 v01 = vx_ఎక్కించు(src1 + j), v02 = vx_ఎక్కించు(src2 + j);
            r0 = v_max(r0, v_absdiff(v01, v02));

            v_uint16 v11 = vx_ఎక్కించు(src1 + j + VTraits<v_uint16>::vlanes()),
                     v12 = vx_ఎక్కించు(src2 + j + VTraits<v_uint16>::vlanes());
            r1 = v_max(r1, v_absdiff(v11, v12));

            v_uint16 v21 = vx_ఎక్కించు(src1 + j + 2 * VTraits<v_uint16>::vlanes()),
                     v22 = vx_ఎక్కించు(src2 + j + 2 * VTraits<v_uint16>::vlanes());
            r2 = v_max(r2, v_absdiff(v21, v22));

            v_uint16 v31 = vx_ఎక్కించు(src1 + j + 3 * VTraits<v_uint16>::vlanes()),
                     v32 = vx_ఎక్కించు(src2 + j + 3 * VTraits<v_uint16>::vlanes());
            r3 = v_max(r3, v_absdiff(v31, v32));
        }
        s = (పూర్ణాంకం)v_reduce_max(v_max(v_max(v_max(r0, r1), r2), r3));
        కొరకు (; j < n; j++) {
            పూర్ణాంకం v = (పూర్ణాంకం)cv_absdiff(src1[j], src2[j]);
            s = std::max(s, v);
        }
        తిరిగి s;
    }
};

మూస<>
నిర్మాణం NormDiffInf_SIMD<పొట్టి, పూర్ణాంకం> {
    పూర్ణాంకం క్రియాకారకం() (స్థిరరాశి పొట్టి* src1, స్థిరరాశి పొట్టి* src2, పూర్ణాంకం n) స్థిరరాశి {
        పూర్ణాంకం j = 0;
        పూర్ణాంకం s = 0;
        v_uint16 r0 = vx_setzero_u16(), r1 = vx_setzero_u16();
        v_uint16 r2 = vx_setzero_u16(), r3 = vx_setzero_u16();
        కొరకు (; j <= n - 4 * VTraits<v_int16>::vlanes(); j += 4 * VTraits<v_int16>::vlanes()) {
            v_int16 v01 = vx_ఎక్కించు(src1 + j), v02 = vx_ఎక్కించు(src2 + j);
            r0 = v_max(r0, v_absdiff(v01, v02));

            v_int16 v11 = vx_ఎక్కించు(src1 + j + VTraits<v_int16>::vlanes()),
                    v12 = vx_ఎక్కించు(src2 + j + VTraits<v_int16>::vlanes());
            r1 = v_max(r1, v_absdiff(v11, v12));

            v_int16 v21 = vx_ఎక్కించు(src1 + j + 2 * VTraits<v_int16>::vlanes()),
                    v22 = vx_ఎక్కించు(src2 + j + 2 * VTraits<v_int16>::vlanes());
            r2 = v_max(r2, v_absdiff(v21, v22));

            v_int16 v31 = vx_ఎక్కించు(src1 + j + 3 * VTraits<v_int16>::vlanes()),
                    v32 = vx_ఎక్కించు(src2 + j + 3 * VTraits<v_int16>::vlanes());
            r3 = v_max(r3, v_absdiff(v31, v32));
        }
        s = (పూర్ణాంకం)v_reduce_max(v_max(v_max(v_max(r0, r1), r2), r3));
        కొరకు (; j < n; j++) {
            పూర్ణాంకం v = (పూర్ణాంకం)cv_absdiff(src1[j], src2[j]);
            s = std::max(s, v);
        }
        తిరిగి s;
    }
};

మూస<>
నిర్మాణం NormDiffInf_SIMD<పూర్ణాంకం, గుర్తులేని> {
    గుర్తులేని క్రియాకారకం() (స్థిరరాశి పూర్ణాంకం* src1, స్థిరరాశి పూర్ణాంకం* src2, పూర్ణాంకం n) స్థిరరాశి {
        పూర్ణాంకం j = 0;
        గుర్తులేని s = 0;
        v_uint32 r0 = vx_setzero_u32(), r1 = vx_setzero_u32();
        v_uint32 r2 = vx_setzero_u32(), r3 = vx_setzero_u32();
        కొరకు (; j <= n - 4 * VTraits<v_int32>::vlanes(); j += 4 * VTraits<v_int32>::vlanes()) {
            v_int32 v01 = vx_ఎక్కించు(src1 + j), v02 = vx_ఎక్కించు(src2 + j);
            r0 = v_max(r0, v_absdiff(v01, v02));

            v_int32 v11 = vx_ఎక్కించు(src1 + j + VTraits<v_int32>::vlanes()),
                    v12 = vx_ఎక్కించు(src2 + j + VTraits<v_int32>::vlanes());
            r1 = v_max(r1, v_absdiff(v11, v12));

            v_int32 v21 = vx_ఎక్కించు(src1 + j + 2 * VTraits<v_int32>::vlanes()),
                    v22 = vx_ఎక్కించు(src2 + j + 2 * VTraits<v_int32>::vlanes());
            r2 = v_max(r2, v_absdiff(v21, v22));

            v_int32 v31 = vx_ఎక్కించు(src1 + j + 3 * VTraits<v_int32>::vlanes()),
                    v32 = vx_ఎక్కించు(src2 + j + 3 * VTraits<v_int32>::vlanes());
            r3 = v_max(r3, v_absdiff(v31, v32));
        }
        s = (పూర్ణాంకం)v_reduce_max(v_max(v_max(v_max(r0, r1), r2), r3));
        కొరకు (; j < n; j++) {
            గుర్తులేని v = (గుర్తులేని)cv_absdiff(src1[j], src2[j]);
            s = std::max(s, v);
        }
        తిరిగి s;
    }
};

మూస<>
నిర్మాణం NormDiffInf_SIMD<దశాంశం, దశాంశం> {
    దశాంశం క్రియాకారకం() (స్థిరరాశి దశాంశం* src1, స్థిరరాశి దశాంశం* src2, పూర్ణాంకం n) స్థిరరాశి {
        పూర్ణాంకం j = 0;
        దశాంశం s = 0;
        v_float32 r0 = vx_setzero_f32(), r1 = vx_setzero_f32();
        కొరకు (; j <= n - 2 * VTraits<v_float32>::vlanes(); j += 2 * VTraits<v_float32>::vlanes()) {
            v_float32 v01 = vx_ఎక్కించు(src1 + j), v02 = vx_ఎక్కించు(src2 + j);
            r0 = v_max(r0, v_absdiff(v01, v02));

            v_float32 v11 = vx_ఎక్కించు(src1 + j + VTraits<v_float32>::vlanes()),
                      v12 = vx_ఎక్కించు(src2 + j + VTraits<v_float32>::vlanes());
            r1 = v_max(r1, v_absdiff(v11, v12));
        }
        s = v_reduce_max(v_max(r0, r1));
        కొరకు (; j < n; j++) {
            దశాంశం v = (దశాంశం)cv_absdiff(src1[j], src2[j]);
            s = std::max(s, v);
        }
        తిరిగి s;
    }
};

మూస<>
నిర్మాణం NormDiffL1_SIMD<uchar, పూర్ణాంకం> {
    పూర్ణాంకం క్రియాకారకం() (స్థిరరాశి uchar* src1, స్థిరరాశి uchar* src2, పూర్ణాంకం n) స్థిరరాశి {
        పూర్ణాంకం j = 0;
        పూర్ణాంకం s = 0;
        v_uint32 r0 = vx_setzero_u32(), r1 = vx_setzero_u32();
        v_uint8  one = vx_setall_u8(1);
        కొరకు (; j<= n - 2 * VTraits<v_uint8>::vlanes(); j += 2 * VTraits<v_uint8>::vlanes()) {
            v_uint8 v01 = vx_ఎక్కించు(src1 + j), v02 = vx_ఎక్కించు(src2 + j);
            r0 = v_dotprod_expand_fast(v_absdiff(v01, v02), one, r0);

            v_uint8 v11 = vx_ఎక్కించు(src1 + j + VTraits<v_uint8>::vlanes()),
                    v12 = vx_ఎక్కించు(src2 + j + VTraits<v_uint8>::vlanes());
            r1 = v_dotprod_expand_fast(v_absdiff(v11, v12), one, r1);
        }
        s += v_reduce_కూడిక(v_కూడు(r0, r1));
        కొరకు (; j < n; j++) {
            పూర్ణాంకం v = (పూర్ణాంకం)cv_absdiff(src1[j], src2[j]);
            s += v;
        }
        తిరిగి s;
    }
};

మూస<>
నిర్మాణం NormDiffL1_SIMD<schar, పూర్ణాంకం> {
    పూర్ణాంకం క్రియాకారకం() (స్థిరరాశి schar* src1, స్థిరరాశి schar* src2, పూర్ణాంకం n) స్థిరరాశి {
        పూర్ణాంకం j = 0;
        పూర్ణాంకం s = 0;
        v_uint32 r0 = vx_setzero_u32(), r1 = vx_setzero_u32();
        v_uint8  one = vx_setall_u8(1);
        కొరకు (; j<= n - 2 * VTraits<v_int8>::vlanes(); j += 2 * VTraits<v_int8>::vlanes()) {
            v_int8 v01 = vx_ఎక్కించు(src1 + j), v02 = vx_ఎక్కించు(src2 + j);
            r0 = v_dotprod_expand_fast(v_absdiff(v01, v02), one, r0);

            v_int8 v11 = vx_ఎక్కించు(src1 + j + VTraits<v_int8>::vlanes()),
                   v12 = vx_ఎక్కించు(src2 + j + VTraits<v_int8>::vlanes());
            r1 = v_dotprod_expand_fast(v_absdiff(v11, v12), one, r1);
        }
        s += v_reduce_కూడిక(v_కూడు(r0, r1));
        కొరకు (; j < n; j++) {
            పూర్ణాంకం v = (పూర్ణాంకం)cv_absdiff(src1[j], src2[j]);
            s += v;
        }
        తిరిగి s;
    }
};

మూస<>
నిర్మాణం NormDiffL1_SIMD<ushort, పూర్ణాంకం> {
    పూర్ణాంకం క్రియాకారకం() (స్థిరరాశి ushort* src1, స్థిరరాశి ushort* src2, పూర్ణాంకం n) స్థిరరాశి {
        పూర్ణాంకం j = 0;
        పూర్ణాంకం s = 0;
        v_uint32 r0 = vx_setzero_u32(), r1 = vx_setzero_u32();
        v_uint32 r2 = vx_setzero_u32(), r3 = vx_setzero_u32();
        కొరకు (; j<= n - 4 * VTraits<v_uint16>::vlanes(); j += 4 * VTraits<v_uint16>::vlanes()) {
            v_uint16 v01 = vx_ఎక్కించు(src1 + j), v02 = vx_ఎక్కించు(src2 + j);
            v_uint32 u00, u01;
            v_expand(v_absdiff(v01, v02), u00, u01);
            r0 = v_కూడు(r0, v_కూడు(u00, u01));

            v_uint16 v11 = vx_ఎక్కించు(src1 + j + VTraits<v_uint16>::vlanes()),
                     v12 = vx_ఎక్కించు(src2 + j + VTraits<v_uint16>::vlanes());
            v_uint32 u10, u11;
            v_expand(v_absdiff(v11, v12), u10, u11);
            r1 = v_కూడు(r1, v_కూడు(u10, u11));

            v_uint16 v21 = vx_ఎక్కించు(src1 + j + 2 * VTraits<v_uint16>::vlanes()),
                     v22 = vx_ఎక్కించు(src2 + j + 2 * VTraits<v_uint16>::vlanes());
            v_uint32 u20, u21;
            v_expand(v_absdiff(v21, v22), u20, u21);
            r2 = v_కూడు(r2, v_కూడు(u20, u21));

            v_uint16 v31 = vx_ఎక్కించు(src1 + j + 3 * VTraits<v_uint16>::vlanes()),
                     v32 = vx_ఎక్కించు(src2 + j + 3 * VTraits<v_uint16>::vlanes());
            v_uint32 u30, u31;
            v_expand(v_absdiff(v31, v32), u30, u31);
            r3 = v_కూడు(r3, v_కూడు(u30, u31));
        }
        s += (పూర్ణాంకం)v_reduce_కూడిక(v_కూడు(v_కూడు(v_కూడు(r0, r1), r2), r3));
        కొరకు (; j < n; j++) {
            పూర్ణాంకం v = (పూర్ణాంకం)cv_absdiff(src1[j], src2[j]);
            s += v;
        }
        తిరిగి s;
    }
};

మూస<>
నిర్మాణం NormDiffL1_SIMD<పొట్టి, పూర్ణాంకం> {
    పూర్ణాంకం క్రియాకారకం() (స్థిరరాశి పొట్టి* src1, స్థిరరాశి పొట్టి* src2, పూర్ణాంకం n) స్థిరరాశి {
        పూర్ణాంకం j = 0;
        పూర్ణాంకం s = 0;
        v_uint32 r0 = vx_setzero_u32(), r1 = vx_setzero_u32();
        v_uint32 r2 = vx_setzero_u32(), r3 = vx_setzero_u32();
        కొరకు (; j<= n - 4 * VTraits<v_int16>::vlanes(); j += 4 * VTraits<v_int16>::vlanes()) {
            v_int16 v01 = vx_ఎక్కించు(src1 + j), v02 = vx_ఎక్కించు(src2 + j);
            v_uint32 u00, u01;
            v_expand(v_absdiff(v01, v02), u00, u01);
            r0 = v_కూడు(r0, v_కూడు(u00, u01));

            v_int16 v11 = vx_ఎక్కించు(src1 + j + VTraits<v_int16>::vlanes()),
                    v12 = vx_ఎక్కించు(src2 + j + VTraits<v_int16>::vlanes());
            v_uint32 u10, u11;
            v_expand(v_absdiff(v11, v12), u10, u11);
            r1 = v_కూడు(r1, v_కూడు(u10, u11));

            v_int16 v21 = vx_ఎక్కించు(src1 + j + 2 * VTraits<v_int16>::vlanes()),
                    v22 = vx_ఎక్కించు(src2 + j + 2 * VTraits<v_int16>::vlanes());
            v_uint32 u20, u21;
            v_expand(v_absdiff(v21, v22), u20, u21);
            r2 = v_కూడు(r2, v_కూడు(u20, u21));

            v_int16 v31 = vx_ఎక్కించు(src1 + j + 3 * VTraits<v_int16>::vlanes()),
                    v32 = vx_ఎక్కించు(src2 + j + 3 * VTraits<v_int16>::vlanes());
            v_uint32 u30, u31;
            v_expand(v_absdiff(v31, v32), u30, u31);
            r3 = v_కూడు(r3, v_కూడు(u30, u31));
        }
        s += (పూర్ణాంకం)v_reduce_కూడిక(v_కూడు(v_కూడు(v_కూడు(r0, r1), r2), r3));
        కొరకు (; j < n; j++) {
            పూర్ణాంకం v = (పూర్ణాంకం)cv_absdiff(src1[j], src2[j]);
            s += v;
        }
        తిరిగి s;
    }
};

మూస<>
నిర్మాణం NormDiffL2_SIMD<uchar, పూర్ణాంకం> {
    పూర్ణాంకం క్రియాకారకం() (స్థిరరాశి uchar* src1, స్థిరరాశి uchar* src2, పూర్ణాంకం n) స్థిరరాశి {
        పూర్ణాంకం j = 0;
        పూర్ణాంకం s = 0;
        v_uint32 r0 = vx_setzero_u32(), r1 = vx_setzero_u32();
        కొరకు (; j <= n - 2 * VTraits<v_uint8>::vlanes(); j += 2 * VTraits<v_uint8>::vlanes()) {
            v_uint8 v01 = vx_ఎక్కించు(src1 + j), v02 = vx_ఎక్కించు(src2 + j);
            v_uint8 v0 = v_absdiff(v01, v02);
            r0 = v_dotprod_expand_fast(v0, v0, r0);

            v_uint8 v11 = vx_ఎక్కించు(src1 + j + VTraits<v_uint8>::vlanes()),
                    v12 = vx_ఎక్కించు(src2 + j + VTraits<v_uint8>::vlanes());
            v_uint8 v1 = v_absdiff(v11, v12);
            r1 = v_dotprod_expand_fast(v1, v1, r1);
        }
        s += v_reduce_కూడిక(v_కూడు(r0, r1));
        కొరకు (; j < n; j++) {
            పూర్ణాంకం v = (పూర్ణాంకం)src1[j] - (పూర్ణాంకం)src2[j];
            s += v * v;
        }
        తిరిగి s;
    }
};

మూస<>
నిర్మాణం NormDiffL2_SIMD<schar, పూర్ణాంకం> {
    పూర్ణాంకం క్రియాకారకం() (స్థిరరాశి schar* src1, స్థిరరాశి schar* src2, పూర్ణాంకం n) స్థిరరాశి {
        పూర్ణాంకం j = 0;
        పూర్ణాంకం s = 0;
        v_uint32 r0 = vx_setzero_u32(), r1 = vx_setzero_u32();
        కొరకు (; j <= n - 2 * VTraits<v_int8>::vlanes(); j += 2 * VTraits<v_int8>::vlanes()) {
            v_int8 v01 = vx_ఎక్కించు(src1 + j), v02 = vx_ఎక్కించు(src2 + j);
            v_uint8 v0 = v_absdiff(v01, v02);
            r0 = v_dotprod_expand_fast(v0, v0, r0);

            v_int8 v11 = vx_ఎక్కించు(src1 + j + VTraits<v_int8>::vlanes()),
                   v12 = vx_ఎక్కించు(src2 + j + VTraits<v_int8>::vlanes());
            v_uint8 v1 = v_absdiff(v11, v12);
            r1 = v_dotprod_expand_fast(v1, v1, r1);
        }
        s += v_reduce_కూడిక(v_కూడు(r0, r1));
        కొరకు (; j < n; j++) {
            పూర్ణాంకం v = (పూర్ణాంకం)src1[j] - (పూర్ణాంకం)src2[j];
            s += v * v;
        }
        తిరిగి s;
    }
};

#అయితే_ముగింపు

#ఒకవేళ (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)

మూస<>
నిర్మాణం NormInf_SIMD<ద్విగుణ, ద్విగుణ> {
    ద్విగుణ క్రియాకారకం() (స్థిరరాశి ద్విగుణ* src, పూర్ణాంకం n) స్థిరరాశి {
        పూర్ణాంకం j = 0;
        ద్విగుణ s = 0.f;
        v_float64 r0 = vx_setzero_f64(), r1 = vx_setzero_f64();
        v_float64 r2 = vx_setzero_f64(), r3 = vx_setzero_f64();
        కొరకు (; j <= n - 4 * VTraits<v_float64>::vlanes(); j += 4 * VTraits<v_float64>::vlanes()) {
            r0 = v_max(r0, v_abs(vx_ఎక్కించు(src + j                                   )));
            r1 = v_max(r1, v_abs(vx_ఎక్కించు(src + j +     VTraits<v_float64>::vlanes())));
            r2 = v_max(r2, v_abs(vx_ఎక్కించు(src + j + 2 * VTraits<v_float64>::vlanes())));
            r3 = v_max(r3, v_abs(vx_ఎక్కించు(src + j + 3 * VTraits<v_float64>::vlanes())));
        }
        r0 = v_max(r0, v_max(r1, v_max(r2, r3)));
        కొరకు (; j < n; j++) {
            s = std::max(s, cv_abs(src[j]));
        }
        // [TODO]: use v_reduce_max when it supports float64
        ద్విగుణ t[VTraits<v_float64>::max_nlanes];
        vx_store(t, r0);
        కొరకు (పూర్ణాంకం i = 0; i < VTraits<v_float64>::vlanes(); i++) {
            s = std::max(s, cv_abs(t[i]));
        }
        తిరిగి s;
    }
};

మూస<>
నిర్మాణం NormL1_SIMD<పూర్ణాంకం, ద్విగుణ> {
    ద్విగుణ క్రియాకారకం() (స్థిరరాశి పూర్ణాంకం* src, పూర్ణాంకం n) స్థిరరాశి {
        పూర్ణాంకం j = 0;
        ద్విగుణ s = 0.f;
        v_float64 r00 = vx_setzero_f64(), r01 = vx_setzero_f64();
        v_float64 r10 = vx_setzero_f64(), r11 = vx_setzero_f64();
        కొరకు (; j <= n - 2 * VTraits<v_int32>::vlanes(); j += 2 * VTraits<v_int32>::vlanes()) {
            // Widen to double before taking the magnitude: exact over the whole int32 range,
            // INT_MIN included, which a float intermediate would not be above 2^24.
            v_int32 i0 = vx_ఎక్కించు(src + j), i1 = vx_ఎక్కించు(src + j + VTraits<v_int32>::vlanes());
            r00 = v_కూడు(r00, v_abs(v_cvt_f64(i0))); r01 = v_కూడు(r01, v_abs(v_cvt_f64_ఎగువ(i0)));
            r10 = v_కూడు(r10, v_abs(v_cvt_f64(i1))); r11 = v_కూడు(r11, v_abs(v_cvt_f64_ఎగువ(i1)));
        }
        s += v_reduce_కూడిక(v_కూడు(v_కూడు(v_కూడు(r00, r01), r10), r11));
        కొరకు (; j < n; j++) {
            s += cv_abs(src[j]);
        }
        తిరిగి s;
    }
};

మూస<>
నిర్మాణం NormL1_SIMD<దశాంశం, ద్విగుణ> {
    ద్విగుణ క్రియాకారకం() (స్థిరరాశి దశాంశం* src, పూర్ణాంకం n) స్థిరరాశి {
        పూర్ణాంకం j = 0;
        ద్విగుణ s = 0.f;
        v_float64 r00 = vx_setzero_f64(), r01 = vx_setzero_f64();
        v_float64 r10 = vx_setzero_f64(), r11 = vx_setzero_f64();
        v_float64 r20 = vx_setzero_f64(), r21 = vx_setzero_f64();
        v_float64 r30 = vx_setzero_f64(), r31 = vx_setzero_f64();
        కొరకు (; j <= n - 4 * VTraits<v_float32>::vlanes(); j += 4 * VTraits<v_float32>::vlanes()) {
            v_float32 v0 = v_abs(vx_ఎక్కించు(src + j)), v1 = v_abs(vx_ఎక్కించు(src + j + VTraits<v_float32>::vlanes()));
            r00 = v_కూడు(r00, v_cvt_f64(v0)); r01 = v_కూడు(r01, v_cvt_f64_ఎగువ(v0));
            r10 = v_కూడు(r10, v_cvt_f64(v1)); r11 = v_కూడు(r11, v_cvt_f64_ఎగువ(v1));

            v_float32 v2 = v_abs(vx_ఎక్కించు(src + j + 2 * VTraits<v_float32>::vlanes())), v3 = v_abs(vx_ఎక్కించు(src + j + 3 * VTraits<v_float32>::vlanes()));
            r20 = v_కూడు(r20, v_cvt_f64(v2)); r21 = v_కూడు(r21, v_cvt_f64_ఎగువ(v2));
            r30 = v_కూడు(r30, v_cvt_f64(v3)); r31 = v_కూడు(r31, v_cvt_f64_ఎగువ(v3));
        }
        s += v_reduce_కూడిక(v_కూడు(v_కూడు(v_కూడు(r00, r01), r10), r11));
        s += v_reduce_కూడిక(v_కూడు(v_కూడు(v_కూడు(r20, r21), r30), r31));
        కొరకు (; j < n; j++) {
            s += cv_abs(src[j]);
        }
        తిరిగి s;
    }
};

మూస<>
నిర్మాణం NormL2_SIMD<ushort, ద్విగుణ> {
    ద్విగుణ క్రియాకారకం() (స్థిరరాశి ushort* src, పూర్ణాంకం n) స్థిరరాశి {
        పూర్ణాంకం j = 0;
        ద్విగుణ s = 0.f;
        v_float64 r0 = vx_setzero_f64(), r1 = vx_setzero_f64();
        కొరకు (; j <= n - 2 * VTraits<v_uint16>::vlanes(); j += 2 * VTraits<v_uint16>::vlanes()) {
            v_uint16 v0 = vx_ఎక్కించు(src + j);
            v_uint64 u0 = v_dotprod_expand_fast(v0, v0);
            r0 = v_కూడు(r0, v_cvt_f64(v_reinterpret_as_s64(u0)));

            v_uint16 v1 = vx_ఎక్కించు(src + j + VTraits<v_uint16>::vlanes());
            v_uint64 u1 = v_dotprod_expand_fast(v1, v1);
            r1 = v_కూడు(r1, v_cvt_f64(v_reinterpret_as_s64(u1)));
        }
        s += v_reduce_కూడిక(v_కూడు(r0, r1));
        కొరకు (; j < n; j++) {
            ద్విగుణ v = saturate_cast<ద్విగుణ>(src[j]);
            s += v * v;
        }
        తిరిగి s;
    }
};

మూస<>
నిర్మాణం NormL2_SIMD<పొట్టి, ద్విగుణ> {
    ద్విగుణ క్రియాకారకం() (స్థిరరాశి పొట్టి* src, పూర్ణాంకం n) స్థిరరాశి {
        పూర్ణాంకం j = 0;
        ద్విగుణ s = 0.f;
        v_float64 r0 = vx_setzero_f64(), r1 = vx_setzero_f64();
        కొరకు (; j <= n - 2 * VTraits<v_int16>::vlanes(); j += 2 * VTraits<v_int16>::vlanes()) {
            v_int16 v0 = vx_ఎక్కించు(src + j);
            r0 = v_కూడు(r0, v_cvt_f64(v_dotprod_expand_fast(v0, v0)));

            v_int16 v1 = vx_ఎక్కించు(src + j + VTraits<v_int16>::vlanes());
            r1 = v_కూడు(r1, v_cvt_f64(v_dotprod_expand_fast(v1, v1)));
        }
        s += v_reduce_కూడిక(v_కూడు(r0, r1));
        కొరకు (; j < n; j++) {
            ద్విగుణ v = saturate_cast<ద్విగుణ>(src[j]);
            s += v * v;
        }
        తిరిగి s;
    }
};

మూస<>
నిర్మాణం NormL2_SIMD<పూర్ణాంకం, ద్విగుణ> {
    ద్విగుణ క్రియాకారకం() (స్థిరరాశి పూర్ణాంకం* src, పూర్ణాంకం n) స్థిరరాశి {
        పూర్ణాంకం j = 0;
        ద్విగుణ s = 0.f;
        v_float64 r0 = vx_setzero_f64(), r1 = vx_setzero_f64();
        కొరకు (; j <= n - 2 * VTraits<v_int32>::vlanes(); j += 2 * VTraits<v_int32>::vlanes()) {
            v_int32 v0 = vx_ఎక్కించు(src + j);
            r0 = v_dotprod_expand_fast(v0, v0, r0);

            v_int32 v1 = vx_ఎక్కించు(src + j + VTraits<v_int32>::vlanes());
            r1 = v_dotprod_expand_fast(v1, v1, r1);
        }
        s += v_reduce_కూడిక(v_కూడు(r0, r1));
        కొరకు (; j < n; j++) {
            ద్విగుణ v = src[j];
            s += v * v;
        }
        తిరిగి s;
    }
};

మూస<>
నిర్మాణం NormL2_SIMD<దశాంశం, ద్విగుణ> {
    ద్విగుణ క్రియాకారకం() (స్థిరరాశి దశాంశం* src, పూర్ణాంకం n) స్థిరరాశి {
        పూర్ణాంకం j = 0;
        ద్విగుణ s = 0.f;
        v_float64 r00 = vx_setzero_f64(), r01 = vx_setzero_f64();
        v_float64 r10 = vx_setzero_f64(), r11 = vx_setzero_f64();
        కొరకు (; j <= n - 2 * VTraits<v_float32>::vlanes(); j += 2 * VTraits<v_float32>::vlanes()) {
            v_float32 v0 = vx_ఎక్కించు(src + j), v1 = vx_ఎక్కించు(src + j + VTraits<v_float32>::vlanes());
            v_float64 v00 = v_cvt_f64(v0), v01 = v_cvt_f64_ఎగువ(v0);
            v_float64 v10 = v_cvt_f64(v1), v11 = v_cvt_f64_ఎగువ(v1);
            r00 = v_fma(v00, v00, r00); r01 = v_fma(v01, v01, r01);
            r10 = v_fma(v10, v10, r10); r11 = v_fma(v11, v11, r11);
        }
        s += v_reduce_కూడిక(v_కూడు(v_కూడు(v_కూడు(r00, r01), r10), r11));
        కొరకు (; j < n; j++) {
            ద్విగుణ v = src[j];
            s += v * v;
        }
        తిరిగి s;
    }
};

#అయితే_ముగింపు

#ఒకవేళ CV_SIMD_64F // CV_SIMD_SCALABLE_64F has accuracy problem with the following kernels on ci

మూస<>
నిర్మాణం NormL1_SIMD<ద్విగుణ, ద్విగుణ> {
    ద్విగుణ క్రియాకారకం() (స్థిరరాశి ద్విగుణ* src, పూర్ణాంకం n) స్థిరరాశి {
        పూర్ణాంకం j = 0;
        ద్విగుణ s = 0.f;
        v_float64 r00 = vx_setzero_f64(), r01 = vx_setzero_f64();
        v_float64 r10 = vx_setzero_f64(), r11 = vx_setzero_f64();
        కొరకు (; j <= n - 4 * VTraits<v_float64>::vlanes(); j += 4 * VTraits<v_float64>::vlanes()) {
            r00 = v_కూడు(r00, v_abs(vx_ఎక్కించు(src + j                                   )));
            r01 = v_కూడు(r01, v_abs(vx_ఎక్కించు(src + j +     VTraits<v_float64>::vlanes())));
            r10 = v_కూడు(r10, v_abs(vx_ఎక్కించు(src + j + 2 * VTraits<v_float64>::vlanes())));
            r11 = v_కూడు(r11, v_abs(vx_ఎక్కించు(src + j + 3 * VTraits<v_float64>::vlanes())));
        }
        s += v_reduce_కూడిక(v_కూడు(v_కూడు(v_కూడు(r00, r01), r10), r11));
        కొరకు (; j < n; j++) {
            s += cv_abs(src[j]);
        }
        తిరిగి s;
    }
};

మూస<>
నిర్మాణం NormL2_SIMD<ద్విగుణ, ద్విగుణ> {
    ద్విగుణ క్రియాకారకం() (స్థిరరాశి ద్విగుణ* src, పూర్ణాంకం n) స్థిరరాశి {
        పూర్ణాంకం j = 0;
        ద్విగుణ s = 0.f;
        v_float64 r00 = vx_setzero_f64(), r01 = vx_setzero_f64();
        v_float64 r10 = vx_setzero_f64(), r11 = vx_setzero_f64();
        కొరకు (; j <= n - 4 * VTraits<v_float64>::vlanes(); j += 4 * VTraits<v_float64>::vlanes()) {
            v_float64 v00 = vx_ఎక్కించు(src + j                                   );
            v_float64 v01 = vx_ఎక్కించు(src + j +     VTraits<v_float64>::vlanes());
            v_float64 v10 = vx_ఎక్కించు(src + j + 2 * VTraits<v_float64>::vlanes());
            v_float64 v11 = vx_ఎక్కించు(src + j + 3 * VTraits<v_float64>::vlanes());
            r00 = v_fma(v00, v00, r00); r01 = v_fma(v01, v01, r01);
            r10 = v_fma(v10, v10, r10); r11 = v_fma(v11, v11, r11);
        }
        s += v_reduce_కూడిక(v_కూడు(v_కూడు(v_కూడు(r00, r01), r10), r11));
        కొరకు (; j < n; j++) {
            ద్విగుణ v = src[j];
            s += v * v;
        }
        తిరిగి s;
    }
};

మూస<>
నిర్మాణం NormDiffInf_SIMD<ద్విగుణ, ద్విగుణ> {
    ద్విగుణ క్రియాకారకం() (స్థిరరాశి ద్విగుణ* src1, స్థిరరాశి ద్విగుణ* src2, పూర్ణాంకం n) స్థిరరాశి {
        పూర్ణాంకం j = 0;
        ద్విగుణ s = 0;
        v_float64 r0 = vx_setzero_f64(), r1 = vx_setzero_f64();
        కొరకు (; j <= n - 2 * VTraits<v_float64>::vlanes(); j += 2 * VTraits<v_float64>::vlanes()) {
            v_float64 v01 = vx_ఎక్కించు(src1 + j), v02 = vx_ఎక్కించు(src2 + j);
            r0 = v_max(r0, v_absdiff(v01, v02));

            v_float64 v11 = vx_ఎక్కించు(src1 + j + VTraits<v_float64>::vlanes()),
                      v12 = vx_ఎక్కించు(src2 + j + VTraits<v_float64>::vlanes());
            r1 = v_max(r1, v_absdiff(v11, v12));
        }
        // [TODO]: use v_reduce_max when it supports float64
        ద్విగుణ t[VTraits<v_float64>::max_nlanes];
        vx_store(t, v_max(r0, r1));
        కొరకు (పూర్ణాంకం i = 0; i < VTraits<v_float64>::vlanes(); i++) {
            s = std::max(s, t[i]);
        }
        కొరకు (; j < n; j++) {
            ద్విగుణ v = (ద్విగుణ)cv_absdiff(src1[j], src2[j]);
            s = std::max(s, v);
        }
        తిరిగి s;
    }
};

మూస<>
నిర్మాణం NormDiffL1_SIMD<దశాంశం, ద్విగుణ> {
    ద్విగుణ క్రియాకారకం() (స్థిరరాశి దశాంశం* src1, స్థిరరాశి దశాంశం* src2, పూర్ణాంకం n) స్థిరరాశి {
        పూర్ణాంకం j = 0;
        ద్విగుణ s = 0.f;
        v_float64 r0 = vx_setzero_f64(), r1 = vx_setzero_f64();
        v_float64 r2 = vx_setzero_f64(), r3 = vx_setzero_f64();
        కొరకు (; j <= n - 2 * VTraits<v_float32>::vlanes(); j += 2 * VTraits<v_float32>::vlanes()) {
            v_float32 v01 = vx_ఎక్కించు(src1 + j), v02 = vx_ఎక్కించు(src2 + j);
            v_float32 v0 = v_absdiff(v01, v02);
            r0 = v_కూడు(r0, v_cvt_f64(v0)); r1 = v_కూడు(r1, v_cvt_f64_ఎగువ(v0));

            v_float32 v11 = vx_ఎక్కించు(src1 + j + VTraits<v_float32>::vlanes()),
                      v12 = vx_ఎక్కించు(src2 + j + VTraits<v_float32>::vlanes());
            v_float32 v1 = v_absdiff(v11, v12);
            r2 = v_కూడు(r2, v_cvt_f64(v1)); r3 = v_కూడు(r3, v_cvt_f64_ఎగువ(v1));
        }
        s += v_reduce_కూడిక(v_కూడు(v_కూడు(v_కూడు(r0, r1), r2), r3));
        కొరకు (; j < n; j++) {
            ద్విగుణ v = (ద్విగుణ)cv_absdiff(src1[j], src2[j]);
            s += v;
        }
        తిరిగి s;
    }
};

మూస<>
నిర్మాణం NormDiffL1_SIMD<ద్విగుణ, ద్విగుణ> {
    ద్విగుణ క్రియాకారకం() (స్థిరరాశి ద్విగుణ* src1, స్థిరరాశి ద్విగుణ* src2, పూర్ణాంకం n) స్థిరరాశి {
        పూర్ణాంకం j = 0;
        ద్విగుణ s = 0.f;
        v_float64 r0 = vx_setzero_f64(), r1 = vx_setzero_f64();
        కొరకు (; j <= n - 2 * VTraits<v_float64>::vlanes(); j += 2 * VTraits<v_float64>::vlanes()) {
            v_float64 v01 = vx_ఎక్కించు(src1 + j), v02 = vx_ఎక్కించు(src2 + j);
            r0 = v_కూడు(r0, v_absdiff(v01, v02));

            v_float64 v11 = vx_ఎక్కించు(src1 + j + VTraits<v_float64>::vlanes()),
                      v12 = vx_ఎక్కించు(src2 + j + VTraits<v_float64>::vlanes());
            r1 = v_కూడు(r1, v_absdiff(v11, v12));
        }
        s += v_reduce_కూడిక(v_కూడు(r0, r1));
        కొరకు (; j < n; j++) {
            ద్విగుణ v = (ద్విగుణ)cv_absdiff(src1[j], src2[j]);
            s += v;
        }
        తిరిగి s;
    }
};

మూస<>
నిర్మాణం NormDiffL2_SIMD<ushort, ద్విగుణ> {
    ద్విగుణ క్రియాకారకం() (స్థిరరాశి ushort* src1, స్థిరరాశి ushort* src2, పూర్ణాంకం n) స్థిరరాశి {
        పూర్ణాంకం j = 0;
        ద్విగుణ s = 0.f;
        v_float64 r0 = vx_setzero_f64(), r1 = vx_setzero_f64();
        కొరకు (; j <= n - 2 * VTraits<v_uint16>::vlanes(); j += 2 * VTraits<v_uint16>::vlanes()) {
            v_uint16 v01 = vx_ఎక్కించు(src1 + j), v02 = vx_ఎక్కించు(src2 + j);
            v_uint16 v0 = v_absdiff(v01, v02);
            v_uint64 u0 = v_dotprod_expand_fast(v0, v0);
            r0 = v_కూడు(r0, v_cvt_f64(v_reinterpret_as_s64(u0)));

            v_uint16 v11 = vx_ఎక్కించు(src1 + j + VTraits<v_uint16>::vlanes()),
                     v12 = vx_ఎక్కించు(src2 + j + VTraits<v_uint16>::vlanes());
            v_uint16 v1 = v_absdiff(v11, v12);
            v_uint64 u1 = v_dotprod_expand_fast(v1, v1);
            r1 = v_కూడు(r1, v_cvt_f64(v_reinterpret_as_s64(u1)));
        }
        s += v_reduce_కూడిక(v_కూడు(r0, r1));
        కొరకు (; j < n; j++) {
            ద్విగుణ v = (ద్విగుణ)src1[j] - (ద్విగుణ)src2[j];
            s += v * v;
        }
        తిరిగి s;
    }
};

మూస<>
నిర్మాణం NormDiffL2_SIMD<పొట్టి, ద్విగుణ> {
    ద్విగుణ క్రియాకారకం() (స్థిరరాశి పొట్టి* src1, స్థిరరాశి పొట్టి* src2, పూర్ణాంకం n) స్థిరరాశి {
        పూర్ణాంకం j = 0;
        ద్విగుణ s = 0.f;
        v_float64 r0 = vx_setzero_f64(), r1 = vx_setzero_f64();
        కొరకు (; j <= n - 2 * VTraits<v_int16>::vlanes(); j += 2 * VTraits<v_int16>::vlanes()) {
            v_int16 v01 = vx_ఎక్కించు(src1 + j), v02 = vx_ఎక్కించు(src2 + j);
            v_uint16 v0 = v_absdiff(v01, v02);
            v_uint64 u0 = v_dotprod_expand_fast(v0, v0);
            r0 = v_కూడు(r0, v_cvt_f64(v_reinterpret_as_s64(u0)));

            v_int16 v11 = vx_ఎక్కించు(src1 + j + VTraits<v_uint16>::vlanes()),
                    v12 = vx_ఎక్కించు(src2 + j + VTraits<v_uint16>::vlanes());
            v_uint16 v1 = v_absdiff(v11, v12);
            v_uint64 u1 = v_dotprod_expand_fast(v1, v1);
            r1 = v_కూడు(r1, v_cvt_f64(v_reinterpret_as_s64(u1)));
        }
        s += v_reduce_కూడిక(v_కూడు(r0, r1));
        కొరకు (; j < n; j++) {
            ద్విగుణ v = (ద్విగుణ)src1[j] - (ద్విగుణ)src2[j];
            s += v * v;
        }
        తిరిగి s;
    }
};

మూస<>
నిర్మాణం NormDiffL2_SIMD<పూర్ణాంకం, ద్విగుణ> {
    ద్విగుణ క్రియాకారకం() (స్థిరరాశి పూర్ణాంకం* src1, స్థిరరాశి పూర్ణాంకం* src2, పూర్ణాంకం n) స్థిరరాశి {
        పూర్ణాంకం j = 0;
        ద్విగుణ s = 0.f;
        v_float64 r0 = vx_setzero_f64(), r1 = vx_setzero_f64();
        కొరకు (; j <= n - VTraits<v_int32>::vlanes(); j += VTraits<v_int32>::vlanes()) {
            v_int32 v01 = vx_ఎక్కించు(src1 + j), v02 = vx_ఎక్కించు(src2 + j);
            v_uint32 v0 = v_absdiff(v01, v02);
            v_uint64 ev0, ev1;
            v_expand(v0, ev0, ev1);
            v_float64 f0 = v_cvt_f64(v_reinterpret_as_s64(ev0)), f1 = v_cvt_f64(v_reinterpret_as_s64(ev1));
            r0 = v_fma(f0, f0, r0); r1 = v_fma(f1, f1, r1);
        }
        s += v_reduce_కూడిక(v_కూడు(r0, r1));
        కొరకు (; j < n; j++) {
            ద్విగుణ v = (ద్విగుణ)src1[j] - (ద్విగుణ)src2[j];
            s += v * v;
        }
        తిరిగి s;
    }
};

మూస<>
నిర్మాణం NormDiffL2_SIMD<దశాంశం, ద్విగుణ> {
    ద్విగుణ క్రియాకారకం() (స్థిరరాశి దశాంశం* src1, స్థిరరాశి దశాంశం* src2, పూర్ణాంకం n) స్థిరరాశి {
        పూర్ణాంకం j = 0;
        ద్విగుణ s = 0.f;
        v_float64 r0 = vx_setzero_f64(), r1 = vx_setzero_f64();
        v_float64 r2 = vx_setzero_f64(), r3 = vx_setzero_f64();
        కొరకు (; j <= n - 2 * VTraits<v_float32>::vlanes(); j += 2 * VTraits<v_float32>::vlanes()) {
            v_float32 v01 = vx_ఎక్కించు(src1 + j), v02 = vx_ఎక్కించు(src2 + j);
            v_float32 v0 = v_absdiff(v01, v02);
            v_float64 f01 = v_cvt_f64(v0), f02 = v_cvt_f64_ఎగువ(v0);
            r0 = v_fma(f01, f01, r0); r1 = v_fma(f02, f02, r1);

            v_float32 v11 = vx_ఎక్కించు(src1 + j + VTraits<v_float32>::vlanes()),
                      v12 = vx_ఎక్కించు(src2 + j + VTraits<v_float32>::vlanes());
            v_float32 v1 = v_absdiff(v11, v12);
            v_float64 f11 = v_cvt_f64(v1), f12 = v_cvt_f64_ఎగువ(v1);
            r2 = v_fma(f11, f11, r2); r3 = v_fma(f12, f12, r3);
        }
        s += v_reduce_కూడిక(v_కూడు(v_కూడు(v_కూడు(r0, r1), r2), r3));
        కొరకు (; j < n; j++) {
            ద్విగుణ v = (ద్విగుణ)src1[j] - (ద్విగుణ)src2[j];
            s += v * v;
        }
        తిరిగి s;
    }
};

మూస<>
నిర్మాణం NormDiffL2_SIMD<ద్విగుణ, ద్విగుణ> {
    ద్విగుణ క్రియాకారకం() (స్థిరరాశి ద్విగుణ* src1, స్థిరరాశి ద్విగుణ* src2, పూర్ణాంకం n) స్థిరరాశి {
        పూర్ణాంకం j = 0;
        ద్విగుణ s = 0.f;
        v_float64 r0 = vx_setzero_f64(), r1 = vx_setzero_f64();
        v_float64 r2 = vx_setzero_f64(), r3 = vx_setzero_f64();
        కొరకు (; j <= n - 4 * VTraits<v_float64>::vlanes(); j += 4 * VTraits<v_float64>::vlanes()) {
            v_float64 v01 = vx_ఎక్కించు(src1 + j), v02 = vx_ఎక్కించు(src2 + j);
            v_float64 v0 = v_absdiff(v01, v02);
            r0 = v_fma(v0, v0, r0);

            v_float64 v11 = vx_ఎక్కించు(src1 + j + VTraits<v_float64>::vlanes()),
                      v12 = vx_ఎక్కించు(src2 + j + VTraits<v_float64>::vlanes());
            v_float64 v1 = v_absdiff(v11, v12);
            r1 = v_fma(v1, v1, r1);

            v_float64 v21 = vx_ఎక్కించు(src1 + j + 2 * VTraits<v_float64>::vlanes()),
                      v22 = vx_ఎక్కించు(src2 + j + 2 * VTraits<v_float64>::vlanes());
            v_float64 v2 = v_absdiff(v21, v22);
            r2 = v_fma(v2, v2, r2);

            v_float64 v31 = vx_ఎక్కించు(src1 + j + 3 * VTraits<v_float64>::vlanes()),
                      v32 = vx_ఎక్కించు(src2 + j + 3 * VTraits<v_float64>::vlanes());
            v_float64 v3 = v_absdiff(v31, v32);
            r3 = v_fma(v3, v3, r3);
        }
        s += v_reduce_కూడిక(v_కూడు(v_కూడు(v_కూడు(r0, r1), r2), r3));
        కొరకు (; j < n; j++) {
            ద్విగుణ v = (ద్విగుణ)src1[j] - (ద్విగుణ)src2[j];
            s += v * v;
        }
        తిరిగి s;
    }
};

#అయితే_ముగింపు

మూస <రకం_పేరు T, రకం_పేరు ST>
నిర్మాణం MaskedNormInf_SIMD {
    అంతఃవరుస ST క్రియాకారకం() (స్థిరరాశి T* src, స్థిరరాశి uchar* ముసుగు, పూర్ణాంకం len, పూర్ణాంకం cn) స్థిరరాశి {
        ST s = 0;
        ఒకవేళ (cn == 1) {
            కొరకు (పూర్ణాంకం i = 0; i < len; i++) {
                ఒకవేళ (ముసుగు[i]) {
                    s = std::max(s, (ST)cv_abs(src[i]));
                }
            }
        }
        లేకపోతే {
            కొరకు (పూర్ణాంకం i = 0; i < len; i++) {
                ఒకవేళ (ముసుగు[i]) {
                    స్థిరరాశి T* elem = src + i * cn;
                    పూర్ణాంకం k = 0;
                #ఒకవేళ CV_ENABLE_UNROLLED && !CV_SIMD_SCALABLE
                    కొరకు (; k <= cn - 4; k += 4) {
                        s = std::max(s, (ST)cv_abs(elem[k]));
                        s = std::max(s, (ST)cv_abs(elem[k + 1]));
                        s = std::max(s, (ST)cv_abs(elem[k + 2]));
                        s = std::max(s, (ST)cv_abs(elem[k + 3]));
                    }
                #అయితే_ముగింపు
                    కొరకు (; k < cn; k++) {
                        s = std::max(s, (ST)cv_abs(elem[k]));
                    }
                }
            }
        }
        తిరిగి s;
    }
};

మూస <రకం_పేరు T, రకం_పేరు ST>
నిర్మాణం MaskedNormL1_SIMD {
    అంతఃవరుస ST క్రియాకారకం() (స్థిరరాశి T* src, స్థిరరాశి uchar* ముసుగు, పూర్ణాంకం len, పూర్ణాంకం cn) స్థిరరాశి {
        ST s = 0;
        ఒకవేళ (cn == 1) {
            కొరకు (పూర్ణాంకం i = 0; i < len; i++) {
                ఒకవేళ (ముసుగు[i]) {
                    s += (ST)cv_abs(src[i]);
                }
            }
        }
        లేకపోతే {
            కొరకు (పూర్ణాంకం i = 0; i < len; i++) {
                ఒకవేళ (ముసుగు[i]) {
                    స్థిరరాశి T* elem = src + i * cn;
                    పూర్ణాంకం k = 0;
                #ఒకవేళ CV_ENABLE_UNROLLED && !CV_SIMD_SCALABLE
                    కొరకు (; k <= cn - 4; k += 4) {
                        s += (ST)cv_abs(elem[k]);
                        s += (ST)cv_abs(elem[k + 1]);
                        s += (ST)cv_abs(elem[k + 2]);
                        s += (ST)cv_abs(elem[k + 3]);
                    }
                #అయితే_ముగింపు
                    కొరకు (; k < cn; k++) {
                        s += (ST)cv_abs(elem[k]);
                    }
                }
            }
        }
        తిరిగి s;
    }
};

మూస <రకం_పేరు T, రకం_పేరు ST>
నిర్మాణం MaskedNormL2_SIMD {
    అంతఃవరుస ST క్రియాకారకం() (స్థిరరాశి T* src, స్థిరరాశి uchar* ముసుగు, పూర్ణాంకం len, పూర్ణాంకం cn) స్థిరరాశి {
        ST s = 0;
        ఒకవేళ (cn == 1) {
            పూర్ణాంకం i = 0;
        #ఒకవేళ CV_ENABLE_UNROLLED && !CV_SIMD_SCALABLE
            కొరకు (; i <= len - 4; i += 4) {
                ఒకవేళ (ముసుగు[i])     { T v0 = src[i];     s += (ST)v0 * v0; }
                ఒకవేళ (ముసుగు[i + 1]) { T v1 = src[i + 1]; s += (ST)v1 * v1; }
                ఒకవేళ (ముసుగు[i + 2]) { T v2 = src[i + 2]; s += (ST)v2 * v2; }
                ఒకవేళ (ముసుగు[i + 3]) { T v3 = src[i + 3]; s += (ST)v3 * v3; }
            }
        #అయితే_ముగింపు
            కొరకు (; i < len; i++) {
                ఒకవేళ (ముసుగు[i]) {
                    T v = src[i];
                    s += (ST)v * v;
                }
            }
        }
        లేకపోతే {
            కొరకు (పూర్ణాంకం i = 0; i < len; i++) {
                ఒకవేళ (ముసుగు[i]) {
                    స్థిరరాశి T* elem = src + i * cn;
                    పూర్ణాంకం k = 0;
                #ఒకవేళ CV_ENABLE_UNROLLED && !CV_SIMD_SCALABLE
                    కొరకు (; k <= cn - 4; k += 4) {
                        T v0 = elem[k];     s += (ST)v0 * v0;
                        T v1 = elem[k + 1]; s += (ST)v1 * v1;
                        T v2 = elem[k + 2]; s += (ST)v2 * v2;
                        T v3 = elem[k + 3]; s += (ST)v3 * v3;
                    }
                #అయితే_ముగింపు
                    కొరకు (; k < cn; k++) {
                        T v = elem[k];
                        s += (ST)v * v;
                    }
                }
            }
        }

        తిరిగి s;
    }
};

#ఒకవేళ (CV_SIMD || CV_SIMD_SCALABLE)
స్థిర అంతఃవరుస v_uint8  v_normmask_u8 (స్థిరరాశి uchar* m) { తిరిగి v_gt(vx_ఎక్కించు(m),          vx_setzero_u8());  }
స్థిర అంతఃవరుస v_uint16 v_normmask_u16(స్థిరరాశి uchar* m) { తిరిగి v_gt(vx_ఎక్కించు_expand(m),   vx_setzero_u16()); }
స్థిర అంతఃవరుస v_uint32 v_normmask_u32(స్థిరరాశి uchar* m) { తిరిగి v_gt(vx_ఎక్కించు_expand_q(m), vx_setzero_u32()); }
స్థిర అంతఃవరుస శూన్య v_normmask_u64(స్థిరరాశి uchar* m, v_uint64& m0, v_uint64& m1) {
    v_uint64 e0, e1;
    v_expand(v_normmask_u32(m), e0, e1);
    m0 = v_or(e0, v_shl<32>(e0));
    m1 = v_or(e1, v_shl<32>(e1));
}

మూస <>
నిర్మాణం MaskedNormInf_SIMD<దశాంశం, దశాంశం> {
    అంతఃవరుస దశాంశం క్రియాకారకం()(స్థిరరాశి దశాంశం* src, స్థిరరాశి uchar* ముసుగు, పూర్ణాంకం len, పూర్ణాంకం cn) స్థిరరాశి {
        దశాంశం ఫలితం = 0.0f;
        ఒకవేళ (cn == 1) {
            పూర్ణాంకం i = 0;
            స్థిరరాశి పూర్ణాంకం vstep = VTraits<v_float32>::vlanes();
            v_float32 acc = vx_setzero_f32();

            కొరకు (; i <= len - vstep; i += vstep) {
                v_uint32 m = vx_ఎక్కించు_expand_q(ముసుగు + i);
                v_uint32 cmp = v_gt(m, vx_setzero_u32());
                v_float32 s = vx_ఎక్కించు(src + i);
                s = v_abs(s);
                s = v_reinterpret_as_f32(v_and(v_reinterpret_as_u32(s), cmp));
                acc = v_max(acc, s);
            }
            ఫలితం = v_reduce_max(acc);

            కొరకు (; i < len; i++) {
                ఒకవేళ (ముసుగు[i])
                    ఫలితం = std::max(ఫలితం, std::నిరపేక్ష(src[i]));
            }
        }
        లేకపోతే {
            కొరకు (పూర్ణాంకం i = 0; i < len; i++) {
                ఒకవేళ (ముసుగు[i]) {
                    స్థిరరాశి దశాంశం* elem = src + i * cn;
                    పూర్ణాంకం k = 0;
                    స్థిరరాశి పూర్ణాంకం vstep = VTraits<v_float32>::vlanes();
                    v_float32 acc = vx_setzero_f32();

                    కొరకు (; k <= cn - vstep; k += vstep) {
                        v_float32 s = vx_ఎక్కించు(elem + k);
                        acc = v_max(acc, v_abs(s));
                    }

                    ఫలితం = std::max(ఫలితం, v_reduce_max(acc));

                    కొరకు (; k < cn; k++)
                        ఫలితం = std::max(ఫలితం, std::నిరపేక్ష(elem[k]));
                }
            }
        }
        తిరిగి ఫలితం;
    }
};

#ఒకవేళ CV_SIMD_64F
మూస <>
నిర్మాణం MaskedNormL1_SIMD<దశాంశం, ద్విగుణ> {
    అంతఃవరుస ద్విగుణ క్రియాకారకం()(స్థిరరాశి దశాంశం* src, స్థిరరాశి uchar* ముసుగు, పూర్ణాంకం len, పూర్ణాంకం cn) స్థిరరాశి {
        ద్విగుణ ఫలితం = 0.0;
        ఒకవేళ (cn == 1) {
            పూర్ణాంకం i = 0;
            స్థిరరాశి పూర్ణాంకం vstep = VTraits<v_float32>::vlanes();
            v_float64 acc0 = vx_setzero_f64(), acc1 = vx_setzero_f64();
            v_float64 acc2 = vx_setzero_f64(), acc3 = vx_setzero_f64();

            కొరకు (; i <= len - 2*vstep; i += 2*vstep) {
                v_float32 s0 = v_and(v_abs(vx_ఎక్కించు(src + i)),
                                     v_reinterpret_as_f32(v_normmask_u32(ముసుగు + i)));
                v_float32 s1 = v_and(v_abs(vx_ఎక్కించు(src + i + vstep)),
                                     v_reinterpret_as_f32(v_normmask_u32(ముసుగు + i + vstep)));
                acc0 = v_కూడు(acc0, v_cvt_f64(s0));
                acc1 = v_కూడు(acc1, v_cvt_f64_ఎగువ(s0));
                acc2 = v_కూడు(acc2, v_cvt_f64(s1));
                acc3 = v_కూడు(acc3, v_cvt_f64_ఎగువ(s1));
            }
            కొరకు (; i <= len - vstep; i += vstep) {
                v_float32 s = v_and(v_abs(vx_ఎక్కించు(src + i)),
                                    v_reinterpret_as_f32(v_normmask_u32(ముసుగు + i)));
                acc0 = v_కూడు(acc0, v_cvt_f64(s));
                acc1 = v_కూడు(acc1, v_cvt_f64_ఎగువ(s));
            }
            ఫలితం = v_reduce_కూడిక(v_కూడు(v_కూడు(acc0, acc1), v_కూడు(acc2, acc3)));

            కొరకు (; i < len; i++) {
                ఒకవేళ (ముసుగు[i])
                    ఫలితం += std::నిరపేక్ష(src[i]);
            }
        }
        లేకపోతే {
            కొరకు (పూర్ణాంకం i = 0; i < len; i++) {
                ఒకవేళ (ముసుగు[i]) {
                    స్థిరరాశి దశాంశం* elem = src + i * cn;
                    పూర్ణాంకం k = 0;
                    స్థిరరాశి పూర్ణాంకం vstep = VTraits<v_float32>::vlanes();
                    v_float64 acc = vx_setzero_f64();

                    కొరకు (; k <= cn - vstep; k += vstep) {
                        v_float32 s = v_abs(vx_ఎక్కించు(elem + k));
                        acc = v_కూడు(acc, v_cvt_f64(s));
                        acc = v_కూడు(acc, v_cvt_f64_ఎగువ(s));
                    }

                    ఫలితం += v_reduce_కూడిక(acc);

                    కొరకు (; k < cn; k++)
                        ఫలితం += std::నిరపేక్ష(elem[k]);
                }
            }
        }
        తిరిగి ఫలితం;
    }
};

మూస <>
నిర్మాణం MaskedNormL2_SIMD<దశాంశం, ద్విగుణ> {
    అంతఃవరుస ద్విగుణ క్రియాకారకం()(స్థిరరాశి దశాంశం* src, స్థిరరాశి uchar* ముసుగు, పూర్ణాంకం len, పూర్ణాంకం cn) స్థిరరాశి {
        ద్విగుణ ఫలితం = 0.0;
        ఒకవేళ (cn == 1) {
            పూర్ణాంకం i = 0;
            స్థిరరాశి పూర్ణాంకం vstep = VTraits<v_float32>::vlanes();
            v_float32 facc = vx_setzero_f32();
            v_float64 dacc = vx_setzero_f64();
            పూర్ణాంకం flush = 0;

            కొరకు (; i <= len - vstep; i += vstep, flush += vstep) {
                ఒకవేళ (flush >= 64) {
                    dacc = v_కూడు(dacc, v_cvt_f64(facc));
                    dacc = v_కూడు(dacc, v_cvt_f64_ఎగువ(facc));
                    facc = vx_setzero_f32();
                    flush = 0;
                }
                v_uint32 cmp = v_gt(vx_ఎక్కించు_expand_q(ముసుగు + i), vx_setzero_u32());
                v_float32 s  = v_reinterpret_as_f32(v_and(v_reinterpret_as_u32(vx_ఎక్కించు(src + i)), cmp));
                facc = v_కూడు(facc, v_mul(s, s));
            }

            dacc = v_కూడు(dacc, v_cvt_f64(facc));
            dacc = v_కూడు(dacc, v_cvt_f64_ఎగువ(facc));
            ఫలితం = v_reduce_కూడిక(dacc);

            కొరకు (; i < len; i++) {
                ఒకవేళ (ముసుగు[i]) {
                    ద్విగుణ v = src[i];
                    ఫలితం += v * v;
                }
            }
        }
        లేకపోతే {
            కొరకు (పూర్ణాంకం i = 0; i < len; i++) {
                ఒకవేళ (ముసుగు[i]) {
                    స్థిరరాశి దశాంశం* elem = src + i * cn;
                    పూర్ణాంకం k = 0;
                    స్థిరరాశి పూర్ణాంకం vstep = VTraits<v_float32>::vlanes();
                    v_float32 facc = vx_setzero_f32();

                    కొరకు (; k <= cn - vstep; k += vstep) {
                        v_float32 s = vx_ఎక్కించు(elem + k);
                        facc = v_కూడు(facc, v_mul(s, s));
                    }

                    v_float64 dacc = v_కూడు(v_cvt_f64(facc), v_cvt_f64_ఎగువ(facc));
                    ఫలితం += v_reduce_కూడిక(dacc);

                    కొరకు (; k < cn; k++) {
                        ద్విగుణ v = elem[k];
                        ఫలితం += v * v;
                    }
                }
            }
        }
        తిరిగి ఫలితం;
    }
};

#అయితే_ముగింపు

మూస <>
నిర్మాణం MaskedNormInf_SIMD<uchar, పూర్ణాంకం> {
    అంతఃవరుస పూర్ణాంకం క్రియాకారకం()(స్థిరరాశి uchar* src, స్థిరరాశి uchar* ముసుగు, పూర్ణాంకం len, పూర్ణాంకం cn) స్థిరరాశి {
        పూర్ణాంకం ఫలితం = 0;
        ఒకవేళ (cn == 1) {
            పూర్ణాంకం i = 0;
            స్థిరరాశి పూర్ణాంకం vstep = VTraits<v_uint8>::vlanes();
            v_uint8 acc = vx_setzero_u8();

            కొరకు (; i <= len - vstep; i += vstep) {
                v_uint8 m   = vx_ఎక్కించు(ముసుగు + i);
                v_uint8 s   = vx_ఎక్కించు(src + i);
                v_uint8 sel = v_and(s, v_gt(m, vx_setzero_u8()));
                acc = v_max(acc, sel);
            }

            ఫలితం = (పూర్ణాంకం)v_reduce_max(acc);

            కొరకు (; i < len; i++) {
                ఒకవేళ (ముసుగు[i])
                    ఫలితం = std::max(ఫలితం, (పూర్ణాంకం)src[i]);
            }
        }
        లేకపోతే ఒకవేళ (cn == 4 && len >= VTraits<v_uint8>::vlanes()) {
            స్థిరరాశి పూర్ణాంకం vstep = VTraits<v_uint8>::vlanes();
            v_uint8 acc = vx_setzero_u8();
            పూర్ణాంకం i = 0;
            కొరకు (;;) {
                ఒకవేళ (i > len - vstep)
                    i = len - vstep; // back-step (max is idempotent)
                v_uint8 c0, c1, c2, c3;
                v_ఎక్కించు_deinterleave(src + i * 4, c0, c1, c2, c3);
                v_uint8 pmax = v_max(v_max(c0, c1), v_max(c2, c3));
                v_uint8 m = v_gt(vx_ఎక్కించు(ముసుగు + i), vx_setzero_u8());
                acc = v_max(acc, v_and(pmax, m));
                ఒకవేళ (i >= len - vstep)
                    ఆపు;
                i += vstep;
            }
            ఫలితం = (పూర్ణాంకం)v_reduce_max(acc);
        }
        లేకపోతే ఒకవేళ (cn == 4) {
            కొరకు (పూర్ణాంకం i = 0; i < len; i++) {
                ఒకవేళ (ముసుగు[i]) {
                    స్థిరరాశి uchar* elem = src + i * 4;
                    ఫలితం = std::max(ఫలితం, (పూర్ణాంకం)std::max(std::max(elem[0], elem[1]),
                                                            std::max(elem[2], elem[3])));
                }
            }
        }
        లేకపోతే {
            కొరకు (పూర్ణాంకం i = 0; i < len; i++) {
                ఒకవేళ (ముసుగు[i]) {
                    స్థిరరాశి uchar* elem = src + i * cn;
                    కొరకు (పూర్ణాంకం k = 0; k < cn; k++)
                        ఫలితం = std::max(ఫలితం, (పూర్ణాంకం)elem[k]);
                }
            }
        }
        తిరిగి ఫలితం;
    }
};

మూస <>
నిర్మాణం MaskedNormL1_SIMD<uchar, పూర్ణాంకం> {
    అంతఃవరుస పూర్ణాంకం క్రియాకారకం()(స్థిరరాశి uchar* src, స్థిరరాశి uchar* ముసుగు, పూర్ణాంకం len, పూర్ణాంకం cn) స్థిరరాశి {
        పూర్ణాంకం ఫలితం = 0;
        స్థిరరాశి పూర్ణాంకం vstep = VTraits<v_uint8>::vlanes();
        స్థిరరాశి v_uint8 one = vx_setall_u8(1);
        ఒకవేళ (cn == 1) {
            పూర్ణాంకం i = 0;
            v_uint32 acc = vx_setzero_u32();
            కొరకు (; i <= len - vstep; i += vstep) {
                v_uint8 s = v_and(vx_ఎక్కించు(src + i), v_normmask_u8(ముసుగు + i));
                acc = v_dotprod_expand_fast(s, one, acc);
            }
            ఫలితం = (పూర్ణాంకం)v_reduce_కూడిక(acc);
            కొరకు (; i < len; i++) {
                ఒకవేళ (ముసుగు[i])
                    ఫలితం += src[i];
            }
        }
        లేకపోతే ఒకవేళ (cn == 4) {
            పూర్ణాంకం i = 0;
            v_uint32 acc = vx_setzero_u32();
            కొరకు (; i <= len - vstep; i += vstep) {
                v_uint8 c0, c1, c2, c3;
                v_ఎక్కించు_deinterleave(src + i * 4, c0, c1, c2, c3);
                v_uint8 m = v_normmask_u8(ముసుగు + i);
                acc = v_dotprod_expand_fast(v_and(c0, m), one, acc);
                acc = v_dotprod_expand_fast(v_and(c1, m), one, acc);
                acc = v_dotprod_expand_fast(v_and(c2, m), one, acc);
                acc = v_dotprod_expand_fast(v_and(c3, m), one, acc);
            }
            ఫలితం = (పూర్ణాంకం)v_reduce_కూడిక(acc);
            కొరకు (; i < len; i++) {
                ఒకవేళ (ముసుగు[i]) {
                    స్థిరరాశి uchar* elem = src + i * 4;
                    ఫలితం += elem[0] + elem[1] + elem[2] + elem[3];
                }
            }
        }
        లేకపోతే {
            కొరకు (పూర్ణాంకం i = 0; i < len; i++) {
                ఒకవేళ (ముసుగు[i]) {
                    స్థిరరాశి uchar* elem = src + i * cn;
                    కొరకు (పూర్ణాంకం k = 0; k < cn; k++)
                        ఫలితం += elem[k];
                }
            }
        }
        తిరిగి ఫలితం;
    }
};

మూస <>
నిర్మాణం MaskedNormInf_SIMD<ushort, పూర్ణాంకం> {
    అంతఃవరుస పూర్ణాంకం క్రియాకారకం()(స్థిరరాశి ushort* src, స్థిరరాశి uchar* ముసుగు, పూర్ణాంకం len, పూర్ణాంకం cn) స్థిరరాశి {
        పూర్ణాంకం ఫలితం = 0;
        ఒకవేళ (cn == 1) {
            పూర్ణాంకం i = 0;
            స్థిరరాశి పూర్ణాంకం vstep = VTraits<v_uint16>::vlanes();
            v_uint16 acc = vx_setzero_u16();

            కొరకు (; i <= len - vstep; i += vstep) {
                v_uint16 m   = vx_ఎక్కించు_expand(ముసుగు + i);
                v_uint16 cmp = v_gt(m, vx_setzero_u16());
                v_uint16 s   = vx_ఎక్కించు(src + i);
                v_uint16 sel = v_and(s, cmp);
                acc = v_max(acc, sel);
            }

            ఫలితం = (పూర్ణాంకం)v_reduce_max(acc);

            కొరకు (; i < len; i++) {
                ఒకవేళ (ముసుగు[i])
                    ఫలితం = std::max(ఫలితం, (పూర్ణాంకం)src[i]);
            }
        }
        లేకపోతే {
            కొరకు (పూర్ణాంకం i = 0; i < len; i++) {
                ఒకవేళ (ముసుగు[i]) {
                    స్థిరరాశి ushort* elem = src + i * cn;
                    పూర్ణాంకం k = 0;
                    స్థిరరాశి పూర్ణాంకం vstep = VTraits<v_uint16>::vlanes();
                    v_uint16 acc = vx_setzero_u16();

                    కొరకు (; k <= cn - vstep; k += vstep) {
                        acc = v_max(acc, vx_ఎక్కించు(elem + k));
                    }

                    ఫలితం = std::max(ఫలితం, (పూర్ణాంకం)v_reduce_max(acc));

                    కొరకు (; k < cn; k++)
                        ఫలితం = std::max(ఫలితం, (పూర్ణాంకం)elem[k]);
                }
            }
        }
        తిరిగి ఫలితం;
    }
};

మూస <>
నిర్మాణం MaskedNormL1_SIMD<ushort, పూర్ణాంకం> {
    అంతఃవరుస పూర్ణాంకం క్రియాకారకం()(స్థిరరాశి ushort* src, స్థిరరాశి uchar* ముసుగు, పూర్ణాంకం len, పూర్ణాంకం cn) స్థిరరాశి {
        పూర్ణాంకం ఫలితం = 0;
        ఒకవేళ (cn == 1) {
            పూర్ణాంకం i = 0;
            స్థిరరాశి పూర్ణాంకం vstep = VTraits<v_uint16>::vlanes();
            v_uint32 acc32 = vx_setzero_u32();
            v_uint64 acc64 = vx_setzero_u64();
            పూర్ణాంకం acc32_elems = 0;

            కొరకు (; i <= len - vstep; i += vstep, acc32_elems += vstep) {
                ఒకవేళ (acc32_elems >= 512) {
                    v_uint64 lo64, hi64;
                    v_expand(acc32, lo64, hi64);
                    acc64 = v_కూడు(acc64, v_కూడు(lo64, hi64));
                    acc32 = vx_setzero_u32();
                    acc32_elems = 0;
                }
                v_uint16 m   = vx_ఎక్కించు_expand(ముసుగు + i);
                v_uint16 cmp = v_gt(m, vx_setzero_u16());
                v_uint16 s   = v_and(vx_ఎక్కించు(src + i), cmp);
                v_uint32 lo32, hi32;
                v_expand(s, lo32, hi32);
                acc32 = v_కూడు(acc32, v_కూడు(lo32, hi32));
            }

            v_uint64 lo64, hi64;
            v_expand(acc32, lo64, hi64);
            acc64 = v_కూడు(acc64, v_కూడు(lo64, hi64));
            ఫలితం = (పూర్ణాంకం)v_reduce_కూడిక(acc64);

            కొరకు (; i < len; i++) {
                ఒకవేళ (ముసుగు[i])
                    ఫలితం += src[i];
            }
        }
        లేకపోతే {
            కొరకు (పూర్ణాంకం i = 0; i < len; i++) {
                ఒకవేళ (ముసుగు[i]) {
                    స్థిరరాశి ushort* elem = src + i * cn;
                    పూర్ణాంకం k = 0;
                    స్థిరరాశి పూర్ణాంకం vstep = VTraits<v_uint16>::vlanes();
                    v_uint32 acc = vx_setzero_u32();

                    కొరకు (; k <= cn - vstep; k += vstep) {
                        v_uint32 lo32, hi32;
                        v_expand(vx_ఎక్కించు(elem + k), lo32, hi32);
                        acc = v_కూడు(acc, v_కూడు(lo32, hi32));
                    }

                    ఫలితం += (పూర్ణాంకం)v_reduce_కూడిక(acc);

                    కొరకు (; k < cn; k++)
                        ఫలితం += elem[k];
                }
            }
        }
        తిరిగి ఫలితం;
    }
};

మూస <>
నిర్మాణం MaskedNormL2_SIMD<ushort, ద్విగుణ> {
    అంతఃవరుస ద్విగుణ క్రియాకారకం()(స్థిరరాశి ushort* src, స్థిరరాశి uchar* ముసుగు, పూర్ణాంకం len, పూర్ణాంకం cn) స్థిరరాశి {
        ద్విగుణ ఫలితం = 0.0;
        ఒకవేళ (cn == 1) {
            పూర్ణాంకం i = 0;
            స్థిరరాశి పూర్ణాంకం vstep = VTraits<v_uint16>::vlanes();
            v_uint64 acc = vx_setzero_u64();
            కొరకు (; i <= len - vstep; i += vstep) {
                v_uint16 m   = vx_ఎక్కించు_expand(ముసుగు + i);
                v_uint16 cmp = v_gt(m, vx_setzero_u16());
                v_uint16 s   = v_and(vx_ఎక్కించు(src + i), cmp);
                v_uint32 lo32, hi32;
                v_expand(s, lo32, hi32);
                v_uint64 lo64a, lo64b, hi64a, hi64b;
                v_expand(v_mul(lo32, lo32), lo64a, lo64b);
                v_expand(v_mul(hi32, hi32), hi64a, hi64b);
                acc = v_కూడు(acc, v_కూడు(v_కూడు(lo64a, lo64b), v_కూడు(hi64a, hi64b)));
            }
            ఫలితం = (ద్విగుణ)v_reduce_కూడిక(acc);
            కొరకు (; i < len; i++) {
                ఒకవేళ (ముసుగు[i]) {
                    ద్విగుణ v = src[i];
                    ఫలితం += v * v;
                }
            }
        }
        లేకపోతే {
            కొరకు (పూర్ణాంకం i = 0; i < len; i++) {
                ఒకవేళ (ముసుగు[i]) {
                    స్థిరరాశి ushort* elem = src + i * cn;
                    పూర్ణాంకం k = 0;
                    స్థిరరాశి పూర్ణాంకం vstep = VTraits<v_uint16>::vlanes();
                    v_uint64 acc = vx_setzero_u64();
                    కొరకు (; k <= cn - vstep; k += vstep) {
                        v_uint32 lo32, hi32;
                        v_expand(vx_ఎక్కించు(elem + k), lo32, hi32);
                        v_uint64 lo64a, lo64b, hi64a, hi64b;
                        v_expand(v_mul(lo32, lo32), lo64a, lo64b);
                        v_expand(v_mul(hi32, hi32), hi64a, hi64b);
                        acc = v_కూడు(acc, v_కూడు(v_కూడు(lo64a, lo64b), v_కూడు(hi64a, hi64b)));
                    }

                    ఫలితం += (ద్విగుణ)v_reduce_కూడిక(acc);

                    కొరకు (; k < cn; k++) {
                        ద్విగుణ v = elem[k];
                        ఫలితం += v * v;
                    }
                }
            }
        }
        తిరిగి ఫలితం;
    }
};

మూస <>
నిర్మాణం MaskedNormInf_SIMD<schar, పూర్ణాంకం> {
    అంతఃవరుస పూర్ణాంకం క్రియాకారకం()(స్థిరరాశి schar* src, స్థిరరాశి uchar* ముసుగు, పూర్ణాంకం len, పూర్ణాంకం cn) స్థిరరాశి {
        పూర్ణాంకం ఫలితం = 0;
        ఒకవేళ (cn == 1) {
            స్థిరరాశి పూర్ణాంకం vstep = VTraits<v_int8>::vlanes();
            పూర్ణాంకం i = 0;
            v_uint8 acc = vx_setzero_u8();
            కొరకు (; i <= len - vstep; i += vstep)
                acc = v_max(acc, v_and(v_abs(vx_ఎక్కించు(src + i)), v_normmask_u8(ముసుగు + i)));
            ఫలితం = (పూర్ణాంకం)v_reduce_max(acc);
            కొరకు (; i < len; i++)
                ఒకవేళ (ముసుగు[i]) ఫలితం = std::max(ఫలితం, cv_abs(src[i]));
        }
        లేకపోతే {
            కొరకు (పూర్ణాంకం i = 0; i < len; i++)
                ఒకవేళ (ముసుగు[i]) {
                    స్థిరరాశి schar* elem = src + i * cn;
                    కొరకు (పూర్ణాంకం k = 0; k < cn; k++)
                        ఫలితం = std::max(ఫలితం, cv_abs(elem[k]));
                }
        }
        తిరిగి ఫలితం;
    }
};

మూస <>
నిర్మాణం MaskedNormL1_SIMD<schar, పూర్ణాంకం> {
    అంతఃవరుస పూర్ణాంకం క్రియాకారకం()(స్థిరరాశి schar* src, స్థిరరాశి uchar* ముసుగు, పూర్ణాంకం len, పూర్ణాంకం cn) స్థిరరాశి {
        పూర్ణాంకం ఫలితం = 0;
        ఒకవేళ (cn == 1) {
            స్థిరరాశి పూర్ణాంకం vstep = VTraits<v_int8>::vlanes();
            స్థిరరాశి v_uint8 one = vx_setall_u8(1);
            పూర్ణాంకం i = 0;
            v_uint32 acc = vx_setzero_u32();
            కొరకు (; i <= len - vstep; i += vstep)
                acc = v_dotprod_expand_fast(v_and(v_abs(vx_ఎక్కించు(src + i)), v_normmask_u8(ముసుగు + i)), one, acc);
            ఫలితం = (పూర్ణాంకం)v_reduce_కూడిక(acc);
            కొరకు (; i < len; i++)
                ఒకవేళ (ముసుగు[i]) ఫలితం += cv_abs(src[i]);
        }
        లేకపోతే {
            కొరకు (పూర్ణాంకం i = 0; i < len; i++)
                ఒకవేళ (ముసుగు[i]) {
                    స్థిరరాశి schar* elem = src + i * cn;
                    కొరకు (పూర్ణాంకం k = 0; k < cn; k++)
                        ఫలితం += cv_abs(elem[k]);
                }
        }
        తిరిగి ఫలితం;
    }
};

మూస <>
నిర్మాణం MaskedNormL2_SIMD<schar, పూర్ణాంకం> {
    అంతఃవరుస పూర్ణాంకం క్రియాకారకం()(స్థిరరాశి schar* src, స్థిరరాశి uchar* ముసుగు, పూర్ణాంకం len, పూర్ణాంకం cn) స్థిరరాశి {
        పూర్ణాంకం ఫలితం = 0;
        ఒకవేళ (cn == 1) {
            స్థిరరాశి పూర్ణాంకం vstep = VTraits<v_int8>::vlanes();
            పూర్ణాంకం i = 0;
            v_int32 acc = vx_setzero_s32();
            కొరకు (; i <= len - vstep; i += vstep) {
                v_int8 s = v_and(vx_ఎక్కించు(src + i), v_reinterpret_as_s8(v_normmask_u8(ముసుగు + i)));
                acc = v_dotprod_expand_fast(s, s, acc);
            }
            ఫలితం = (పూర్ణాంకం)v_reduce_కూడిక(acc);
            కొరకు (; i < len; i++)
                ఒకవేళ (ముసుగు[i]) { పూర్ణాంకం v = src[i]; ఫలితం += v * v; }
        }
        లేకపోతే {
            కొరకు (పూర్ణాంకం i = 0; i < len; i++)
                ఒకవేళ (ముసుగు[i]) {
                    స్థిరరాశి schar* elem = src + i * cn;
                    కొరకు (పూర్ణాంకం k = 0; k < cn; k++) { పూర్ణాంకం v = elem[k]; ఫలితం += v * v; }
                }
        }
        తిరిగి ఫలితం;
    }
};

మూస <>
నిర్మాణం MaskedNormL2_SIMD<uchar, పూర్ణాంకం> {
    అంతఃవరుస పూర్ణాంకం క్రియాకారకం()(స్థిరరాశి uchar* src, స్థిరరాశి uchar* ముసుగు, పూర్ణాంకం len, పూర్ణాంకం cn) స్థిరరాశి {
        పూర్ణాంకం ఫలితం = 0;
        స్థిరరాశి పూర్ణాంకం vstep = VTraits<v_uint8>::vlanes();
        ఒకవేళ (cn == 1) {
            పూర్ణాంకం i = 0;
            v_uint32 acc = vx_setzero_u32();
            కొరకు (; i <= len - vstep; i += vstep) {
                v_uint8 s = v_and(vx_ఎక్కించు(src + i), v_normmask_u8(ముసుగు + i));
                acc = v_dotprod_expand_fast(s, s, acc);
            }
            ఫలితం = (పూర్ణాంకం)v_reduce_కూడిక(acc);
            కొరకు (; i < len; i++)
                ఒకవేళ (ముసుగు[i]) { పూర్ణాంకం v = src[i]; ఫలితం += v * v; }
        }
        లేకపోతే ఒకవేళ (cn == 4) {
            పూర్ణాంకం i = 0;
            v_uint32 acc = vx_setzero_u32();
            కొరకు (; i <= len - vstep; i += vstep) {
                v_uint8 c0, c1, c2, c3;
                v_ఎక్కించు_deinterleave(src + i * 4, c0, c1, c2, c3);
                v_uint8 m = v_normmask_u8(ముసుగు + i);
                c0 = v_and(c0, m); c1 = v_and(c1, m);
                c2 = v_and(c2, m); c3 = v_and(c3, m);
                acc = v_dotprod_expand_fast(c0, c0, acc);
                acc = v_dotprod_expand_fast(c1, c1, acc);
                acc = v_dotprod_expand_fast(c2, c2, acc);
                acc = v_dotprod_expand_fast(c3, c3, acc);
            }
            ఫలితం = (పూర్ణాంకం)v_reduce_కూడిక(acc);
            కొరకు (; i < len; i++)
                ఒకవేళ (ముసుగు[i]) {
                    స్థిరరాశి uchar* elem = src + i * 4;
                    కొరకు (పూర్ణాంకం k = 0; k < 4; k++) { పూర్ణాంకం v = elem[k]; ఫలితం += v * v; }
                }
        }
        లేకపోతే {
            కొరకు (పూర్ణాంకం i = 0; i < len; i++)
                ఒకవేళ (ముసుగు[i]) {
                    స్థిరరాశి uchar* elem = src + i * cn;
                    కొరకు (పూర్ణాంకం k = 0; k < cn; k++) { పూర్ణాంకం v = elem[k]; ఫలితం += v * v; }
                }
        }
        తిరిగి ఫలితం;
    }
};

మూస <>
నిర్మాణం MaskedNormInf_SIMD<పొట్టి, పూర్ణాంకం> {
    అంతఃవరుస పూర్ణాంకం క్రియాకారకం()(స్థిరరాశి పొట్టి* src, స్థిరరాశి uchar* ముసుగు, పూర్ణాంకం len, పూర్ణాంకం cn) స్థిరరాశి {
        పూర్ణాంకం ఫలితం = 0;
        ఒకవేళ (cn == 1) {
            స్థిరరాశి పూర్ణాంకం vstep = VTraits<v_int16>::vlanes();
            పూర్ణాంకం i = 0;
            v_uint16 acc = vx_setzero_u16();
            కొరకు (; i <= len - vstep; i += vstep)
                acc = v_max(acc, v_and(v_abs(vx_ఎక్కించు(src + i)), v_normmask_u16(ముసుగు + i)));
            ఫలితం = (పూర్ణాంకం)v_reduce_max(acc);
            కొరకు (; i < len; i++)
                ఒకవేళ (ముసుగు[i]) ఫలితం = std::max(ఫలితం, cv_abs(src[i]));
        }
        లేకపోతే {
            కొరకు (పూర్ణాంకం i = 0; i < len; i++)
                ఒకవేళ (ముసుగు[i]) {
                    స్థిరరాశి పొట్టి* elem = src + i * cn;
                    కొరకు (పూర్ణాంకం k = 0; k < cn; k++)
                        ఫలితం = std::max(ఫలితం, cv_abs(elem[k]));
                }
        }
        తిరిగి ఫలితం;
    }
};

మూస <>
నిర్మాణం MaskedNormL1_SIMD<పొట్టి, పూర్ణాంకం> {
    అంతఃవరుస పూర్ణాంకం క్రియాకారకం()(స్థిరరాశి పొట్టి* src, స్థిరరాశి uchar* ముసుగు, పూర్ణాంకం len, పూర్ణాంకం cn) స్థిరరాశి {
        పూర్ణాంకం ఫలితం = 0;
        ఒకవేళ (cn == 1) {
            స్థిరరాశి పూర్ణాంకం vstep = VTraits<v_int16>::vlanes();
            పూర్ణాంకం i = 0;
            v_uint32 acc0 = vx_setzero_u32(), acc1 = vx_setzero_u32();
            కొరకు (; i <= len - vstep; i += vstep) {
                v_uint16 s = v_and(v_abs(vx_ఎక్కించు(src + i)), v_normmask_u16(ముసుగు + i));
                v_uint32 lo, hi;
                v_expand(s, lo, hi);
                acc0 = v_కూడు(acc0, lo);
                acc1 = v_కూడు(acc1, hi);
            }
            ఫలితం = (పూర్ణాంకం)v_reduce_కూడిక(v_కూడు(acc0, acc1));
            కొరకు (; i < len; i++)
                ఒకవేళ (ముసుగు[i]) ఫలితం += cv_abs(src[i]);
        }
        లేకపోతే {
            కొరకు (పూర్ణాంకం i = 0; i < len; i++)
                ఒకవేళ (ముసుగు[i]) {
                    స్థిరరాశి పొట్టి* elem = src + i * cn;
                    కొరకు (పూర్ణాంకం k = 0; k < cn; k++)
                        ఫలితం += cv_abs(elem[k]);
                }
        }
        తిరిగి ఫలితం;
    }
};

మూస <>
నిర్మాణం MaskedNormL2_SIMD<పొట్టి, ద్విగుణ> {
    అంతఃవరుస ద్విగుణ క్రియాకారకం()(స్థిరరాశి పొట్టి* src, స్థిరరాశి uchar* ముసుగు, పూర్ణాంకం len, పూర్ణాంకం cn) స్థిరరాశి {
        ద్విగుణ ఫలితం = 0.0;
        ఒకవేళ (cn == 1) {
            స్థిరరాశి పూర్ణాంకం vstep = VTraits<v_int16>::vlanes();
            పూర్ణాంకం i = 0;
            v_uint64 acc = vx_setzero_u64();
            కొరకు (; i <= len - vstep; i += vstep) {
                v_uint16 s = v_and(v_abs(vx_ఎక్కించు(src + i)), v_normmask_u16(ముసుగు + i));
                acc = v_కూడు(acc, v_dotprod_expand_fast(s, s));
            }
            ఫలితం = (ద్విగుణ)v_reduce_కూడిక(acc);
            కొరకు (; i < len; i++)
                ఒకవేళ (ముసుగు[i]) { ద్విగుణ v = src[i]; ఫలితం += v * v; }
        }
        లేకపోతే {
            కొరకు (పూర్ణాంకం i = 0; i < len; i++)
                ఒకవేళ (ముసుగు[i]) {
                    స్థిరరాశి పొట్టి* elem = src + i * cn;
                    కొరకు (పూర్ణాంకం k = 0; k < cn; k++) { ద్విగుణ v = elem[k]; ఫలితం += v * v; }
                }
        }
        తిరిగి ఫలితం;
    }
};

మూస <>
నిర్మాణం MaskedNormInf_SIMD<పూర్ణాంకం, గుర్తులేని> {
    అంతఃవరుస గుర్తులేని క్రియాకారకం()(స్థిరరాశి పూర్ణాంకం* src, స్థిరరాశి uchar* ముసుగు, పూర్ణాంకం len, పూర్ణాంకం cn) స్థిరరాశి {
        గుర్తులేని ఫలితం = 0;
        ఒకవేళ (cn == 1) {
            స్థిరరాశి పూర్ణాంకం vstep = VTraits<v_int32>::vlanes();
            పూర్ణాంకం i = 0;
            v_uint32 acc = vx_setzero_u32();
            కొరకు (; i <= len - vstep; i += vstep)
                acc = v_max(acc, v_and(v_abs(vx_ఎక్కించు(src + i)), v_normmask_u32(ముసుగు + i)));
            ఫలితం = v_reduce_max(acc);
            కొరకు (; i < len; i++)
                ఒకవేళ (ముసుగు[i]) ఫలితం = std::max(ఫలితం, cv_abs(src[i]));
        }
        లేకపోతే {
            కొరకు (పూర్ణాంకం i = 0; i < len; i++)
                ఒకవేళ (ముసుగు[i]) {
                    స్థిరరాశి పూర్ణాంకం* elem = src + i * cn;
                    కొరకు (పూర్ణాంకం k = 0; k < cn; k++)
                        ఫలితం = std::max(ఫలితం, cv_abs(elem[k]));
                }
        }
        తిరిగి ఫలితం;
    }
};

#ఒకవేళ CV_SIMD_64F || CV_SIMD_SCALABLE_64F

మూస <>
నిర్మాణం MaskedNormL1_SIMD<పూర్ణాంకం, ద్విగుణ> {
    అంతఃవరుస ద్విగుణ క్రియాకారకం()(స్థిరరాశి పూర్ణాంకం* src, స్థిరరాశి uchar* ముసుగు, పూర్ణాంకం len, పూర్ణాంకం cn) స్థిరరాశి {
        ద్విగుణ ఫలితం = 0.0;
        ఒకవేళ (cn == 1) {
            స్థిరరాశి పూర్ణాంకం vstep = VTraits<v_int32>::vlanes();
            పూర్ణాంకం i = 0;
            v_float64 acc0 = vx_setzero_f64(), acc1 = vx_setzero_f64();
            కొరకు (; i <= len - vstep; i += vstep) {
                // Mask the value, then widen and take the magnitude in double. v_abs() does give
                // |INT_MIN| as an unsigned 2147483648, but reinterpreting that back as int32 turns
                // it negative again, and the widening then carries the sign into the accumulator.
                v_int32 s = v_reinterpret_as_s32(v_and(v_reinterpret_as_u32(vx_ఎక్కించు(src + i)),
                                                       v_normmask_u32(ముసుగు + i)));
                acc0 = v_కూడు(acc0, v_abs(v_cvt_f64(s)));
                acc1 = v_కూడు(acc1, v_abs(v_cvt_f64_ఎగువ(s)));
            }
            ఫలితం = v_reduce_కూడిక(v_కూడు(acc0, acc1));
            కొరకు (; i < len; i++)
                ఒకవేళ (ముసుగు[i]) ఫలితం += cv_abs(src[i]);
        }
        లేకపోతే {
            కొరకు (పూర్ణాంకం i = 0; i < len; i++)
                ఒకవేళ (ముసుగు[i]) {
                    స్థిరరాశి పూర్ణాంకం* elem = src + i * cn;
                    కొరకు (పూర్ణాంకం k = 0; k < cn; k++)
                        ఫలితం += cv_abs(elem[k]);
                }
        }
        తిరిగి ఫలితం;
    }
};

మూస <>
నిర్మాణం MaskedNormL2_SIMD<పూర్ణాంకం, ద్విగుణ> {
    అంతఃవరుస ద్విగుణ క్రియాకారకం()(స్థిరరాశి పూర్ణాంకం* src, స్థిరరాశి uchar* ముసుగు, పూర్ణాంకం len, పూర్ణాంకం cn) స్థిరరాశి {
        ద్విగుణ ఫలితం = 0.0;
        ఒకవేళ (cn == 1) {
            స్థిరరాశి పూర్ణాంకం vstep = VTraits<v_int32>::vlanes();
            పూర్ణాంకం i = 0;
            v_float64 acc0 = vx_setzero_f64(), acc1 = vx_setzero_f64();
            కొరకు (; i <= len - vstep; i += vstep) {
                v_uint32 s = v_and(v_abs(vx_ఎక్కించు(src + i)), v_normmask_u32(ముసుగు + i));
                v_uint64 e0, e1;
                v_expand(s, e0, e1);
                v_float64 f0 = v_cvt_f64(v_reinterpret_as_s64(e0));
                v_float64 f1 = v_cvt_f64(v_reinterpret_as_s64(e1));
                acc0 = v_fma(f0, f0, acc0);
                acc1 = v_fma(f1, f1, acc1);
            }
            ఫలితం = v_reduce_కూడిక(v_కూడు(acc0, acc1));
            కొరకు (; i < len; i++)
                ఒకవేళ (ముసుగు[i]) { ద్విగుణ v = src[i]; ఫలితం += v * v; }
        }
        లేకపోతే {
            కొరకు (పూర్ణాంకం i = 0; i < len; i++)
                ఒకవేళ (ముసుగు[i]) {
                    స్థిరరాశి పూర్ణాంకం* elem = src + i * cn;
                    కొరకు (పూర్ణాంకం k = 0; k < cn; k++) { ద్విగుణ v = elem[k]; ఫలితం += v * v; }
                }
        }
        తిరిగి ఫలితం;
    }
};

మూస <>
నిర్మాణం MaskedNormInf_SIMD<ద్విగుణ, ద్విగుణ> {
    అంతఃవరుస ద్విగుణ క్రియాకారకం()(స్థిరరాశి ద్విగుణ* src, స్థిరరాశి uchar* ముసుగు, పూర్ణాంకం len, పూర్ణాంకం cn) స్థిరరాశి {
        ద్విగుణ ఫలితం = 0.0;
        ఒకవేళ (cn == 1) {
            స్థిరరాశి పూర్ణాంకం vstep = VTraits<v_float64>::vlanes();
            పూర్ణాంకం i = 0;
            v_float64 acc0 = vx_setzero_f64(), acc1 = vx_setzero_f64();
            కొరకు (; i <= len - 2*vstep; i += 2*vstep) {
                v_uint64 m0, m1;
                v_normmask_u64(ముసుగు + i, m0, m1);
                acc0 = v_max(acc0, v_reinterpret_as_f64(v_and(v_reinterpret_as_u64(v_abs(vx_ఎక్కించు(src + i))), m0)));
                acc1 = v_max(acc1, v_reinterpret_as_f64(v_and(v_reinterpret_as_u64(v_abs(vx_ఎక్కించు(src + i + vstep))), m1)));
            }
            ద్విగుణ t[VTraits<v_float64>::max_nlanes];
            vx_store(t, v_max(acc0, acc1));
            కొరకు (పూర్ణాంకం k = 0; k < vstep; k++)
                ఫలితం = std::max(ఫలితం, t[k]);
            కొరకు (; i < len; i++)
                ఒకవేళ (ముసుగు[i]) ఫలితం = std::max(ఫలితం, cv_abs(src[i]));
        }
        లేకపోతే {
            కొరకు (పూర్ణాంకం i = 0; i < len; i++)
                ఒకవేళ (ముసుగు[i]) {
                    స్థిరరాశి ద్విగుణ* elem = src + i * cn;
                    కొరకు (పూర్ణాంకం k = 0; k < cn; k++)
                        ఫలితం = std::max(ఫలితం, cv_abs(elem[k]));
                }
        }
        తిరిగి ఫలితం;
    }
};

మూస <>
నిర్మాణం MaskedNormL1_SIMD<ద్విగుణ, ద్విగుణ> {
    అంతఃవరుస ద్విగుణ క్రియాకారకం()(స్థిరరాశి ద్విగుణ* src, స్థిరరాశి uchar* ముసుగు, పూర్ణాంకం len, పూర్ణాంకం cn) స్థిరరాశి {
        ద్విగుణ ఫలితం = 0.0;
        ఒకవేళ (cn == 1) {
            స్థిరరాశి పూర్ణాంకం vstep = VTraits<v_float64>::vlanes();
            పూర్ణాంకం i = 0;
            v_float64 acc0 = vx_setzero_f64(), acc1 = vx_setzero_f64();
            కొరకు (; i <= len - 2*vstep; i += 2*vstep) {
                v_uint64 m0, m1;
                v_normmask_u64(ముసుగు + i, m0, m1);
                acc0 = v_కూడు(acc0, v_reinterpret_as_f64(v_and(v_reinterpret_as_u64(v_abs(vx_ఎక్కించు(src + i))), m0)));
                acc1 = v_కూడు(acc1, v_reinterpret_as_f64(v_and(v_reinterpret_as_u64(v_abs(vx_ఎక్కించు(src + i + vstep))), m1)));
            }
            ఫలితం = v_reduce_కూడిక(v_కూడు(acc0, acc1));
            కొరకు (; i < len; i++)
                ఒకవేళ (ముసుగు[i]) ఫలితం += cv_abs(src[i]);
        }
        లేకపోతే {
            కొరకు (పూర్ణాంకం i = 0; i < len; i++)
                ఒకవేళ (ముసుగు[i]) {
                    స్థిరరాశి ద్విగుణ* elem = src + i * cn;
                    కొరకు (పూర్ణాంకం k = 0; k < cn; k++)
                        ఫలితం += cv_abs(elem[k]);
                }
        }
        తిరిగి ఫలితం;
    }
};

మూస <>
నిర్మాణం MaskedNormL2_SIMD<ద్విగుణ, ద్విగుణ> {
    అంతఃవరుస ద్విగుణ క్రియాకారకం()(స్థిరరాశి ద్విగుణ* src, స్థిరరాశి uchar* ముసుగు, పూర్ణాంకం len, పూర్ణాంకం cn) స్థిరరాశి {
        ద్విగుణ ఫలితం = 0.0;
        ఒకవేళ (cn == 1) {
            స్థిరరాశి పూర్ణాంకం vstep = VTraits<v_float64>::vlanes();
            పూర్ణాంకం i = 0;
            v_float64 acc0 = vx_setzero_f64(), acc1 = vx_setzero_f64();
            కొరకు (; i <= len - 2*vstep; i += 2*vstep) {
                v_uint64 m0, m1;
                v_normmask_u64(ముసుగు + i, m0, m1);
                v_float64 s0 = v_reinterpret_as_f64(v_and(v_reinterpret_as_u64(vx_ఎక్కించు(src + i)), m0));
                v_float64 s1 = v_reinterpret_as_f64(v_and(v_reinterpret_as_u64(vx_ఎక్కించు(src + i + vstep)), m1));
                acc0 = v_fma(s0, s0, acc0);
                acc1 = v_fma(s1, s1, acc1);
            }
            ఫలితం = v_reduce_కూడిక(v_కూడు(acc0, acc1));
            కొరకు (; i < len; i++)
                ఒకవేళ (ముసుగు[i]) { ద్విగుణ v = src[i]; ఫలితం += v * v; }
        }
        లేకపోతే {
            కొరకు (పూర్ణాంకం i = 0; i < len; i++)
                ఒకవేళ (ముసుగు[i]) {
                    స్థిరరాశి ద్విగుణ* elem = src + i * cn;
                    కొరకు (పూర్ణాంకం k = 0; k < cn; k++) { ద్విగుణ v = elem[k]; ఫలితం += v * v; }
                }
        }
        తిరిగి ఫలితం;
    }
};

#అయితే_ముగింపు // CV_SIMD_64F || CV_SIMD_SCALABLE_64F
#అయితే_ముగింపు // CV_SIMD || CV_SIMD_SCALABLE

మూస<రకం_పేరు T, రకం_పేరు ST> పూర్ణాంకం
normInf_(స్థిరరాశి T* src, స్థిరరాశి uchar* ముసుగు, ST* ఫలితం_2, పూర్ణాంకం len, పూర్ణాంకం cn)
{
    ST ఫలితం = *ఫలితం_2;
    ఒకవేళ( !ముసుగు )
    {
        NormInf_SIMD<T, ST> op;
        ఫలితం = std::max(ఫలితం, op(src, len*cn));
    } లేకపోతే {
        MaskedNormInf_SIMD<T, ST> op;
        ఫలితం = std::max(ఫలితం, op(src, ముసుగు, len, cn));
    }
    *ఫలితం_2 = ఫలితం;
    తిరిగి 0;
}

మూస<రకం_పేరు T, రకం_పేరు ST> పూర్ణాంకం
normL1_(స్థిరరాశి T* src, స్థిరరాశి uchar* ముసుగు, ST* ఫలితం_2, పూర్ణాంకం len, పూర్ణాంకం cn)
{
    ST ఫలితం = *ఫలితం_2;
    ఒకవేళ( !ముసుగు )
    {
        NormL1_SIMD<T, ST> op;
        ఫలితం += op(src, len*cn);
    } లేకపోతే {
        MaskedNormL1_SIMD<T, ST> op;
        ఫలితం += op(src, ముసుగు, len, cn);
    }
    *ఫలితం_2 = ఫలితం;
    తిరిగి 0;
}

మూస<రకం_పేరు T, రకం_పేరు ST> పూర్ణాంకం
normL2_(స్థిరరాశి T* src, స్థిరరాశి uchar* ముసుగు, ST* ఫలితం_2, పూర్ణాంకం len, పూర్ణాంకం cn)
{
    ST ఫలితం = *ఫలితం_2;
    ఒకవేళ( !ముసుగు )
    {
        NormL2_SIMD<T, ST> op;
        ఫలితం += op(src, len*cn);
    }
    లేకపోతే
    {
        MaskedNormL2_SIMD<T, ST> op;
        ఫలితం += op(src, ముసుగు, len, cn);
    }
    *ఫలితం_2 = ఫలితం;
    తిరిగి 0;
}

స్థిర పూర్ణాంకం
normInf_Bool(స్థిరరాశి uchar* src, స్థిరరాశి uchar* ముసుగు, పూర్ణాంకం* ఫలితం_2, పూర్ణాంకం len, పూర్ణాంకం cn)
{
    పూర్ణాంకం ఫలితం = *ఫలితం_2;
    ఒకవేళ( !ముసుగు )
    {
        కొరకు ( పూర్ణాంకం i = 0; i < len*cn; i++ ) {
            ఫలితం = std::max(ఫలితం, (పూర్ణాంకం)(src[i] != 0));
            ఒకవేళ (ఫలితం != 0)
                ఆపు;
        }
    }
    లేకపోతే
    {
        కొరకు( పూర్ణాంకం i = 0; i < len; i++, src += cn )
            ఒకవేళ( ముసుగు[i] )
            {
                కొరకు( పూర్ణాంకం k = 0; k < cn; k++ )
                    ఫలితం = std::max(ఫలితం, (పూర్ణాంకం)(src[k] != 0));
                ఒకవేళ (ఫలితం != 0)
                    ఆపు;
            }
    }
    *ఫలితం_2 = ఫలితం;
    తిరిగి 0;
}

స్థిర పూర్ణాంకం
normL1_Bool(స్థిరరాశి uchar* src, స్థిరరాశి uchar* ముసుగు, పూర్ణాంకం* ఫలితం_2, పూర్ణాంకం len, పూర్ణాంకం cn)
{
    పూర్ణాంకం ఫలితం = *ఫలితం_2;
    ఒకవేళ( !ముసుగు )
    {
        కొరకు ( పూర్ణాంకం i = 0; i < len*cn; i++ )
            ఫలితం += (పూర్ణాంకం)(src[i] != 0);
    }
    లేకపోతే
    {
        కొరకు( పూర్ణాంకం i = 0; i < len; i++, src += cn )
            ఒకవేళ( ముసుగు[i] )
            {
                కొరకు( పూర్ణాంకం k = 0; k < cn; k++ )
                    ఫలితం += (పూర్ణాంకం)(src[k] != 0);
            }
    }
    *ఫలితం_2 = ఫలితం;
    తిరిగి 0;
}

స్థిర పూర్ణాంకం
normL2_Bool(స్థిరరాశి uchar* src, స్థిరరాశి uchar* ముసుగు, పూర్ణాంకం* ఫలితం_2, పూర్ణాంకం len, పూర్ణాంకం cn)
{
    తిరిగి normL1_Bool(src, ముసుగు, ఫలితం_2, len, cn);
}

// ====================================================================
// Masked norm-diff kernels.
// Scalar base templates (safe for every depth) plus SIMD cn==1
// specializations for the concrete (T, ST) pairs dispatched by
// CV_DEF_NORM_ALL. The masked normDiff*_ functions below select them.
// ====================================================================
మూస <రకం_పేరు T, రకం_పేరు ST>
నిర్మాణం MaskedNormDiffInf_SIMD {
    అంతఃవరుస ST క్రియాకారకం()(స్థిరరాశి T* s1, స్థిరరాశి T* s2, స్థిరరాశి uchar* ముసుగు, పూర్ణాంకం len, పూర్ణాంకం cn) స్థిరరాశి {
        ST ఫలితం = 0;
        కొరకు (పూర్ణాంకం i = 0; i < len; i++) ఒకవేళ (ముసుగు[i]) {
            స్థిరరాశి T* e1 = s1 + i*cn; స్థిరరాశి T* e2 = s2 + i*cn;
            కొరకు (పూర్ణాంకం k = 0; k < cn; k++)
                ఫలితం = std::max(ఫలితం, (ST)cv_absdiff(e1[k], e2[k]));
        }
        తిరిగి ఫలితం;
    }
};
మూస <రకం_పేరు T, రకం_పేరు ST>
నిర్మాణం MaskedNormDiffL1_SIMD {
    అంతఃవరుస ST క్రియాకారకం()(స్థిరరాశి T* s1, స్థిరరాశి T* s2, స్థిరరాశి uchar* ముసుగు, పూర్ణాంకం len, పూర్ణాంకం cn) స్థిరరాశి {
        ST ఫలితం = 0;
        కొరకు (పూర్ణాంకం i = 0; i < len; i++) ఒకవేళ (ముసుగు[i]) {
            స్థిరరాశి T* e1 = s1 + i*cn; స్థిరరాశి T* e2 = s2 + i*cn;
            కొరకు (పూర్ణాంకం k = 0; k < cn; k++)
                ఫలితం += (ST)cv_absdiff(e1[k], e2[k]);
        }
        తిరిగి ఫలితం;
    }
};
మూస <రకం_పేరు T, రకం_పేరు ST>
నిర్మాణం MaskedNormDiffL2_SIMD {
    అంతఃవరుస ST క్రియాకారకం()(స్థిరరాశి T* s1, స్థిరరాశి T* s2, స్థిరరాశి uchar* ముసుగు, పూర్ణాంకం len, పూర్ణాంకం cn) స్థిరరాశి {
        ST ఫలితం = 0;
        కొరకు (పూర్ణాంకం i = 0; i < len; i++) ఒకవేళ (ముసుగు[i]) {
            స్థిరరాశి T* e1 = s1 + i*cn; స్థిరరాశి T* e2 = s2 + i*cn;
            కొరకు (పూర్ణాంకం k = 0; k < cn; k++) { ST v = (ST)e1[k] - (ST)e2[k]; ఫలితం += v*v; }
        }
        తిరిగి ఫలితం;
    }
};

#ఒకవేళ (CV_SIMD || CV_SIMD_SCALABLE)
// AND a per-lane diff vector with the (expanded) mask>0 predicate.
అంతఃవరుస v_uint8  v_diffmask(స్థిరరాశి v_uint8&  d, స్థిరరాశి uchar* m) { తిరిగి v_and(d, v_gt(vx_ఎక్కించు(m),          vx_setzero_u8())); }
అంతఃవరుస v_uint16 v_diffmask(స్థిరరాశి v_uint16& d, స్థిరరాశి uchar* m) { తిరిగి v_and(d, v_gt(vx_ఎక్కించు_expand(m),   vx_setzero_u16())); }
అంతఃవరుస v_uint32 v_diffmask(స్థిరరాశి v_uint32& d, స్థిరరాశి uchar* m) { తిరిగి v_and(d, v_gt(vx_ఎక్కించు_expand_q(m), vx_setzero_u32())); }
అంతఃవరుస v_float32 v_diffmask(స్థిరరాశి v_float32& d, స్థిరరాశి uchar* m) {
    v_uint32 cm = v_gt(vx_ఎక్కించు_expand_q(m), vx_setzero_u32());
    తిరిగి v_reinterpret_as_f32(v_and(v_reinterpret_as_u32(d), cm));
}

// Shared scalar cn>1 fallbacks (match the base templates' cv_absdiff semantics).
మూస<రకం_పేరు T, రకం_పేరు RT>
స్థిర అంతఃవరుస RT masked_Norm_Diff_Inf_తోక(స్థిరరాశి T* s1, స్థిరరాశి T* s2, స్థిరరాశి uchar* ముసుగు, పూర్ణాంకం len, పూర్ణాంకం cn, RT ఫలితం) {
    కొరకు (పూర్ణాంకం i = 0; i < len; i++) ఒకవేళ (ముసుగు[i]) {
        స్థిరరాశి T* e1 = s1 + i*cn; స్థిరరాశి T* e2 = s2 + i*cn;
        కొరకు (పూర్ణాంకం k = 0; k < cn; k++) ఫలితం = std::max(ఫలితం, (RT)cv_absdiff(e1[k], e2[k]));
    }
    తిరిగి ఫలితం;
}
మూస<రకం_పేరు T, రకం_పేరు RT>
స్థిర అంతఃవరుస RT masked_Norm_Diff_L1_తోక(స్థిరరాశి T* s1, స్థిరరాశి T* s2, స్థిరరాశి uchar* ముసుగు, పూర్ణాంకం len, పూర్ణాంకం cn, RT ఫలితం) {
    కొరకు (పూర్ణాంకం i = 0; i < len; i++) ఒకవేళ (ముసుగు[i]) {
        స్థిరరాశి T* e1 = s1 + i*cn; స్థిరరాశి T* e2 = s2 + i*cn;
        కొరకు (పూర్ణాంకం k = 0; k < cn; k++) ఫలితం += (RT)cv_absdiff(e1[k], e2[k]);
    }
    తిరిగి ఫలితం;
}
మూస<రకం_పేరు T, రకం_పేరు RT>
స్థిర అంతఃవరుస RT masked_Norm_Diff_L2_తోక(స్థిరరాశి T* s1, స్థిరరాశి T* s2, స్థిరరాశి uchar* ముసుగు, పూర్ణాంకం len, పూర్ణాంకం cn, RT ఫలితం) {
    కొరకు (పూర్ణాంకం i = 0; i < len; i++) ఒకవేళ (ముసుగు[i]) {
        స్థిరరాశి T* e1 = s1 + i*cn; స్థిరరాశి T* e2 = s2 + i*cn;
        కొరకు (పూర్ణాంకం k = 0; k < cn; k++) { RT v = (RT)e1[k] - (RT)e2[k]; ఫలితం += v*v; }
    }
    తిరిగి ఫలితం;
}

// Inf (cn==1): reduce_max of masked |a-b|. Shared across SIMD-capable depths.
మూస<రకం_పేరు T, రకం_పేరు ST>
స్థిర అంతఃవరుస ST maskedNormDiffInfCn1(స్థిరరాశి T* s1, స్థిరరాశి T* s2, స్థిరరాశి uchar* ముసుగు, పూర్ణాంకం len) {
    స్థిరరాశి పూర్ణాంకం vstep = VTraits<decltype(vx_ఎక్కించు(s1))>::vlanes();
    ST ఫలితం = 0;
    పూర్ణాంకం i = 0;
    ఒకవేళ (len >= vstep) {
        స్వయంచాలక acc = v_diffmask(v_absdiff(vx_ఎక్కించు(s1), vx_ఎక్కించు(s2)), ముసుగు);
        కొరకు (i = vstep; i <= len - vstep; i += vstep)
            acc = v_max(acc, v_diffmask(v_absdiff(vx_ఎక్కించు(s1 + i), vx_ఎక్కించు(s2 + i)), ముసుగు + i));
        ఫలితం = (ST)v_reduce_max(acc);
    }
    కొరకు (; i < len; i++) ఒకవేళ (ముసుగు[i]) ఫలితం = std::max(ఫలితం, (ST)cv_absdiff(s1[i], s2[i]));
    తిరిగి ఫలితం;
}

// Shared 8-bit cn==1 masked L1/L2 kernels (uchar/schar): v_absdiff yields v_uint8 for both.
మూస<రకం_పేరు T>
స్థిర అంతఃవరుస పూర్ణాంకం maskedNormDiffL1_8(స్థిరరాశి T* s1, స్థిరరాశి T* s2, స్థిరరాశి uchar* ముసుగు, పూర్ణాంకం len) {
    పూర్ణాంకం i = 0; స్థిరరాశి పూర్ణాంకం vstep = VTraits<v_uint8>::vlanes();
    స్థిరరాశి v_uint8 one = vx_setall_u8(1);
    v_uint32 acc = vx_setzero_u32();
    కొరకు (; i <= len - vstep; i += vstep) {
        v_uint8 m  = v_gt(vx_ఎక్కించు(ముసుగు + i), vx_setzero_u8());
        v_uint8 ad = v_and(v_absdiff(vx_ఎక్కించు(s1 + i), vx_ఎక్కించు(s2 + i)), m);
        acc = v_dotprod_expand_fast(ad, one, acc);
    }
    పూర్ణాంకం ఫలితం = (పూర్ణాంకం)v_reduce_కూడిక(acc);
    కొరకు (; i < len; i++) ఒకవేళ (ముసుగు[i]) ఫలితం += (పూర్ణాంకం)cv_absdiff(s1[i], s2[i]);
    తిరిగి ఫలితం;
}
మూస<రకం_పేరు T>
స్థిర అంతఃవరుస పూర్ణాంకం maskedNormDiffL2_8(స్థిరరాశి T* s1, స్థిరరాశి T* s2, స్థిరరాశి uchar* ముసుగు, పూర్ణాంకం len) {
    పూర్ణాంకం i = 0; స్థిరరాశి పూర్ణాంకం vstep = VTraits<v_uint8>::vlanes();
    v_uint32 acc = vx_setzero_u32();
    కొరకు (; i <= len - vstep; i += vstep) {
        v_uint8 m  = v_gt(vx_ఎక్కించు(ముసుగు + i), vx_setzero_u8());
        v_uint8 ad = v_and(v_absdiff(vx_ఎక్కించు(s1 + i), vx_ఎక్కించు(s2 + i)), m);
        acc = v_dotprod_expand_fast(ad, ad, acc);
    }
    పూర్ణాంకం ఫలితం = (పూర్ణాంకం)v_reduce_కూడిక(acc);
    కొరకు (; i < len; i++) ఒకవేళ (ముసుగు[i]) { పూర్ణాంకం v = (పూర్ణాంకం)s1[i] - (పూర్ణాంకం)s2[i]; ఫలితం += v*v; }
    తిరిగి ఫలితం;
}

// Shared 16-bit cn==1 masked L1 kernel (ushort/short): v_absdiff yields v_uint16 for both.
మూస<రకం_పేరు T>
స్థిర అంతఃవరుస పూర్ణాంకం maskedNormDiffL1_16(స్థిరరాశి T* s1, స్థిరరాశి T* s2, స్థిరరాశి uchar* ముసుగు, పూర్ణాంకం len) {
    పూర్ణాంకం i = 0; స్థిరరాశి పూర్ణాంకం vstep = VTraits<v_uint16>::vlanes();
    v_uint32 acc = vx_setzero_u32();
    కొరకు (; i <= len - vstep; i += vstep) {
        v_uint16 m  = v_gt(vx_ఎక్కించు_expand(ముసుగు + i), vx_setzero_u16());
        v_uint16 ad = v_and(v_absdiff(vx_ఎక్కించు(s1 + i), vx_ఎక్కించు(s2 + i)), m);
        v_uint32 lo, hi; v_expand(ad, lo, hi);
        acc = v_కూడు(acc, v_కూడు(lo, hi));
    }
    పూర్ణాంకం ఫలితం = (పూర్ణాంకం)v_reduce_కూడిక(acc);
    కొరకు (; i < len; i++) ఒకవేళ (ముసుగు[i]) ఫలితం += (పూర్ణాంకం)cv_absdiff(s1[i], s2[i]);
    తిరిగి ఫలితం;
}

మూస<> నిర్మాణం MaskedNormDiffInf_SIMD<uchar, పూర్ణాంకం> {
    పూర్ణాంకం క్రియాకారకం()(స్థిరరాశి uchar* s1, స్థిరరాశి uchar* s2, స్థిరరాశి uchar* ముసుగు, పూర్ణాంకం len, పూర్ణాంకం cn) స్థిరరాశి {
        ఒకవేళ (cn == 1) తిరిగి maskedNormDiffInfCn1<uchar, పూర్ణాంకం>(s1, s2, ముసుగు, len);
        ఒకవేళ (cn == 4 && len >= VTraits<v_uint8>::vlanes()) {
            స్థిరరాశి పూర్ణాంకం vstep = VTraits<v_uint8>::vlanes();
            v_uint8 acc = vx_setzero_u8();
            పూర్ణాంకం i = 0;
            కొరకు (;;) {
                ఒకవేళ (i > len - vstep) i = len - vstep; // back-step (max is idempotent)
                v_uint8 a0,a1,a2,a3,b0,b1,b2,b3;
                v_ఎక్కించు_deinterleave(s1 + i*4, a0,a1,a2,a3);
                v_ఎక్కించు_deinterleave(s2 + i*4, b0,b1,b2,b3);
                v_uint8 ad = v_max(v_max(v_absdiff(a0,b0), v_absdiff(a1,b1)),
                                   v_max(v_absdiff(a2,b2), v_absdiff(a3,b3)));
                v_uint8 m = v_gt(vx_ఎక్కించు(ముసుగు + i), vx_setzero_u8());
                acc = v_max(acc, v_and(ad, m));
                ఒకవేళ (i >= len - vstep) ఆపు;
                i += vstep;
            }
            తిరిగి (పూర్ణాంకం)v_reduce_max(acc);
        }
        తిరిగి masked_Norm_Diff_Inf_తోక<uchar, పూర్ణాంకం>(s1, s2, ముసుగు, len, cn, 0);
    }
};
మూస<> నిర్మాణం MaskedNormDiffInf_SIMD<schar, పూర్ణాంకం> {
    పూర్ణాంకం క్రియాకారకం()(స్థిరరాశి schar* s1, స్థిరరాశి schar* s2, స్థిరరాశి uchar* ముసుగు, పూర్ణాంకం len, పూర్ణాంకం cn) స్థిరరాశి {
        ఒకవేళ (cn == 1) తిరిగి maskedNormDiffInfCn1<schar, పూర్ణాంకం>(s1, s2, ముసుగు, len);
        తిరిగి masked_Norm_Diff_Inf_తోక<schar, పూర్ణాంకం>(s1, s2, ముసుగు, len, cn, 0);
    }
};
మూస<> నిర్మాణం MaskedNormDiffInf_SIMD<ushort, పూర్ణాంకం> {
    పూర్ణాంకం క్రియాకారకం()(స్థిరరాశి ushort* s1, స్థిరరాశి ushort* s2, స్థిరరాశి uchar* ముసుగు, పూర్ణాంకం len, పూర్ణాంకం cn) స్థిరరాశి {
        ఒకవేళ (cn == 1) తిరిగి maskedNormDiffInfCn1<ushort, పూర్ణాంకం>(s1, s2, ముసుగు, len);
        తిరిగి masked_Norm_Diff_Inf_తోక<ushort, పూర్ణాంకం>(s1, s2, ముసుగు, len, cn, 0);
    }
};
మూస<> నిర్మాణం MaskedNormDiffInf_SIMD<పొట్టి, పూర్ణాంకం> {
    పూర్ణాంకం క్రియాకారకం()(స్థిరరాశి పొట్టి* s1, స్థిరరాశి పొట్టి* s2, స్థిరరాశి uchar* ముసుగు, పూర్ణాంకం len, పూర్ణాంకం cn) స్థిరరాశి {
        ఒకవేళ (cn == 1) తిరిగి maskedNormDiffInfCn1<పొట్టి, పూర్ణాంకం>(s1, s2, ముసుగు, len);
        తిరిగి masked_Norm_Diff_Inf_తోక<పొట్టి, పూర్ణాంకం>(s1, s2, ముసుగు, len, cn, 0);
    }
};
మూస<> నిర్మాణం MaskedNormDiffInf_SIMD<పూర్ణాంకం, గుర్తులేని> {
    గుర్తులేని క్రియాకారకం()(స్థిరరాశి పూర్ణాంకం* s1, స్థిరరాశి పూర్ణాంకం* s2, స్థిరరాశి uchar* ముసుగు, పూర్ణాంకం len, పూర్ణాంకం cn) స్థిరరాశి {
        ఒకవేళ (cn == 1) తిరిగి maskedNormDiffInfCn1<పూర్ణాంకం, గుర్తులేని>(s1, s2, ముసుగు, len);
        తిరిగి masked_Norm_Diff_Inf_తోక<పూర్ణాంకం, గుర్తులేని>(s1, s2, ముసుగు, len, cn, 0u);
    }
};
మూస<> నిర్మాణం MaskedNormDiffInf_SIMD<దశాంశం, దశాంశం> {
    దశాంశం క్రియాకారకం()(స్థిరరాశి దశాంశం* s1, స్థిరరాశి దశాంశం* s2, స్థిరరాశి uchar* ముసుగు, పూర్ణాంకం len, పూర్ణాంకం cn) స్థిరరాశి {
        ఒకవేళ (cn == 1) తిరిగి maskedNormDiffInfCn1<దశాంశం, దశాంశం>(s1, s2, ముసుగు, len);
        తిరిగి masked_Norm_Diff_Inf_తోక<దశాంశం, దశాంశం>(s1, s2, ముసుగు, len, cn, 0.0f);
    }
};

మూస<> నిర్మాణం MaskedNormDiffL1_SIMD<uchar, పూర్ణాంకం> {
    పూర్ణాంకం క్రియాకారకం()(స్థిరరాశి uchar* s1, స్థిరరాశి uchar* s2, స్థిరరాశి uchar* ముసుగు, పూర్ణాంకం len, పూర్ణాంకం cn) స్థిరరాశి {
        ఒకవేళ (cn == 1) తిరిగి maskedNormDiffL1_8(s1, s2, ముసుగు, len);
        ఒకవేళ (cn == 4) {
            పూర్ణాంకం i = 0; స్థిరరాశి పూర్ణాంకం vstep = VTraits<v_uint8>::vlanes();
            స్థిరరాశి v_uint8 one = vx_setall_u8(1);
            v_uint32 acc = vx_setzero_u32();
            కొరకు (; i <= len - vstep; i += vstep) {
                v_uint8 a0,a1,a2,a3,b0,b1,b2,b3;
                v_ఎక్కించు_deinterleave(s1 + i*4, a0,a1,a2,a3);
                v_ఎక్కించు_deinterleave(s2 + i*4, b0,b1,b2,b3);
                v_uint8 m = v_gt(vx_ఎక్కించు(ముసుగు + i), vx_setzero_u8());
                acc = v_dotprod_expand_fast(v_and(v_absdiff(a0,b0), m), one, acc);
                acc = v_dotprod_expand_fast(v_and(v_absdiff(a1,b1), m), one, acc);
                acc = v_dotprod_expand_fast(v_and(v_absdiff(a2,b2), m), one, acc);
                acc = v_dotprod_expand_fast(v_and(v_absdiff(a3,b3), m), one, acc);
            }
            పూర్ణాంకం ఫలితం = (పూర్ణాంకం)v_reduce_కూడిక(acc);
            కొరకు (; i < len; i++) ఒకవేళ (ముసుగు[i]) {
                స్థిరరాశి uchar* e1 = s1 + i*4; స్థిరరాశి uchar* e2 = s2 + i*4;
                కొరకు (పూర్ణాంకం k = 0; k < 4; k++) ఫలితం += (పూర్ణాంకం)cv_absdiff(e1[k], e2[k]);
            }
            తిరిగి ఫలితం;
        }
        తిరిగి masked_Norm_Diff_L1_తోక<uchar, పూర్ణాంకం>(s1, s2, ముసుగు, len, cn, 0);
    }
};
మూస<> నిర్మాణం MaskedNormDiffL1_SIMD<schar, పూర్ణాంకం> {
    పూర్ణాంకం క్రియాకారకం()(స్థిరరాశి schar* s1, స్థిరరాశి schar* s2, స్థిరరాశి uchar* ముసుగు, పూర్ణాంకం len, పూర్ణాంకం cn) స్థిరరాశి {
        ఒకవేళ (cn == 1) తిరిగి maskedNormDiffL1_8(s1, s2, ముసుగు, len);
        తిరిగి masked_Norm_Diff_L1_తోక<schar, పూర్ణాంకం>(s1, s2, ముసుగు, len, cn, 0);
    }
};
మూస<> నిర్మాణం MaskedNormDiffL1_SIMD<ushort, పూర్ణాంకం> {
    పూర్ణాంకం క్రియాకారకం()(స్థిరరాశి ushort* s1, స్థిరరాశి ushort* s2, స్థిరరాశి uchar* ముసుగు, పూర్ణాంకం len, పూర్ణాంకం cn) స్థిరరాశి {
        ఒకవేళ (cn == 1) తిరిగి maskedNormDiffL1_16(s1, s2, ముసుగు, len);
        తిరిగి masked_Norm_Diff_L1_తోక<ushort, పూర్ణాంకం>(s1, s2, ముసుగు, len, cn, 0);
    }
};
మూస<> నిర్మాణం MaskedNormDiffL1_SIMD<పొట్టి, పూర్ణాంకం> {
    పూర్ణాంకం క్రియాకారకం()(స్థిరరాశి పొట్టి* s1, స్థిరరాశి పొట్టి* s2, స్థిరరాశి uchar* ముసుగు, పూర్ణాంకం len, పూర్ణాంకం cn) స్థిరరాశి {
        ఒకవేళ (cn == 1) తిరిగి maskedNormDiffL1_16(s1, s2, ముసుగు, len);
        తిరిగి masked_Norm_Diff_L1_తోక<పొట్టి, పూర్ణాంకం>(s1, s2, ముసుగు, len, cn, 0);
    }
};

మూస<> నిర్మాణం MaskedNormDiffL2_SIMD<uchar, పూర్ణాంకం> {
    పూర్ణాంకం క్రియాకారకం()(స్థిరరాశి uchar* s1, స్థిరరాశి uchar* s2, స్థిరరాశి uchar* ముసుగు, పూర్ణాంకం len, పూర్ణాంకం cn) స్థిరరాశి {
        ఒకవేళ (cn == 1) తిరిగి maskedNormDiffL2_8(s1, s2, ముసుగు, len);
        ఒకవేళ (cn == 4) {
            పూర్ణాంకం i = 0; స్థిరరాశి పూర్ణాంకం vstep = VTraits<v_uint8>::vlanes();
            v_uint32 acc = vx_setzero_u32();
            కొరకు (; i <= len - vstep; i += vstep) {
                v_uint8 a0,a1,a2,a3,b0,b1,b2,b3;
                v_ఎక్కించు_deinterleave(s1 + i*4, a0,a1,a2,a3);
                v_ఎక్కించు_deinterleave(s2 + i*4, b0,b1,b2,b3);
                v_uint8 m = v_gt(vx_ఎక్కించు(ముసుగు + i), vx_setzero_u8());
                v_uint8 d0 = v_and(v_absdiff(a0,b0), m), d1 = v_and(v_absdiff(a1,b1), m);
                v_uint8 d2 = v_and(v_absdiff(a2,b2), m), d3 = v_and(v_absdiff(a3,b3), m);
                acc = v_dotprod_expand_fast(d0, d0, acc);
                acc = v_dotprod_expand_fast(d1, d1, acc);
                acc = v_dotprod_expand_fast(d2, d2, acc);
                acc = v_dotprod_expand_fast(d3, d3, acc);
            }
            పూర్ణాంకం ఫలితం = (పూర్ణాంకం)v_reduce_కూడిక(acc);
            కొరకు (; i < len; i++) ఒకవేళ (ముసుగు[i]) {
                స్థిరరాశి uchar* e1 = s1 + i*4; స్థిరరాశి uchar* e2 = s2 + i*4;
                కొరకు (పూర్ణాంకం k = 0; k < 4; k++) { పూర్ణాంకం v = (పూర్ణాంకం)e1[k] - (పూర్ణాంకం)e2[k]; ఫలితం += v*v; }
            }
            తిరిగి ఫలితం;
        }
        తిరిగి masked_Norm_Diff_L2_తోక<uchar, పూర్ణాంకం>(s1, s2, ముసుగు, len, cn, 0);
    }
};
మూస<> నిర్మాణం MaskedNormDiffL2_SIMD<schar, పూర్ణాంకం> {
    పూర్ణాంకం క్రియాకారకం()(స్థిరరాశి schar* s1, స్థిరరాశి schar* s2, స్థిరరాశి uchar* ముసుగు, పూర్ణాంకం len, పూర్ణాంకం cn) స్థిరరాశి {
        ఒకవేళ (cn == 1) తిరిగి maskedNormDiffL2_8(s1, s2, ముసుగు, len);
        తిరిగి masked_Norm_Diff_L2_తోక<schar, పూర్ణాంకం>(s1, s2, ముసుగు, len, cn, 0);
    }
};

#ఒకవేళ CV_SIMD_64F
// Depths whose L1/L2 accumulate in double.
మూస<> నిర్మాణం MaskedNormDiffL1_SIMD<పూర్ణాంకం, ద్విగుణ> {
    ద్విగుణ క్రియాకారకం()(స్థిరరాశి పూర్ణాంకం* s1, స్థిరరాశి పూర్ణాంకం* s2, స్థిరరాశి uchar* ముసుగు, పూర్ణాంకం len, పూర్ణాంకం cn) స్థిరరాశి {
        ఒకవేళ (cn != 1) తిరిగి masked_Norm_Diff_L1_తోక<పూర్ణాంకం, ద్విగుణ>(s1, s2, ముసుగు, len, cn, 0.0);
        పూర్ణాంకం i = 0; స్థిరరాశి పూర్ణాంకం vstep = VTraits<v_int32>::vlanes();
        v_float64 acc = vx_setzero_f64();
        కొరకు (; i <= len - vstep; i += vstep) {
            v_uint32 m  = v_gt(vx_ఎక్కించు_expand_q(ముసుగు + i), vx_setzero_u32());
            v_uint32 ad = v_and(v_absdiff(vx_ఎక్కించు(s1 + i), vx_ఎక్కించు(s2 + i)), m);
            v_uint64 e0, e1; v_expand(ad, e0, e1);
            acc = v_కూడు(acc, v_cvt_f64(v_reinterpret_as_s64(e0)));
            acc = v_కూడు(acc, v_cvt_f64(v_reinterpret_as_s64(e1)));
        }
        ద్విగుణ ఫలితం = v_reduce_కూడిక(acc);
        కొరకు (; i < len; i++) ఒకవేళ (ముసుగు[i]) ఫలితం += (ద్విగుణ)cv_absdiff(s1[i], s2[i]);
        తిరిగి ఫలితం;
    }
};
మూస<> నిర్మాణం MaskedNormDiffL1_SIMD<దశాంశం, ద్విగుణ> {
    ద్విగుణ క్రియాకారకం()(స్థిరరాశి దశాంశం* s1, స్థిరరాశి దశాంశం* s2, స్థిరరాశి uchar* ముసుగు, పూర్ణాంకం len, పూర్ణాంకం cn) స్థిరరాశి {
        ఒకవేళ (cn != 1) తిరిగి masked_Norm_Diff_L1_తోక<దశాంశం, ద్విగుణ>(s1, s2, ముసుగు, len, cn, 0.0);
        పూర్ణాంకం i = 0; స్థిరరాశి పూర్ణాంకం vstep = VTraits<v_float32>::vlanes();
        v_float64 acc0 = vx_setzero_f64(), acc1 = vx_setzero_f64();
        కొరకు (; i <= len - vstep; i += vstep) {
            v_float32 m  = v_reinterpret_as_f32(v_gt(vx_ఎక్కించు_expand_q(ముసుగు + i), vx_setzero_u32()));
            v_float32 ad = v_and(v_absdiff(vx_ఎక్కించు(s1 + i), vx_ఎక్కించు(s2 + i)), m);
            acc0 = v_కూడు(acc0, v_cvt_f64(ad));
            acc1 = v_కూడు(acc1, v_cvt_f64_ఎగువ(ad));
        }
        ద్విగుణ ఫలితం = v_reduce_కూడిక(v_కూడు(acc0, acc1));
        కొరకు (; i < len; i++) ఒకవేళ (ముసుగు[i]) ఫలితం += (ద్విగుణ)cv_absdiff(s1[i], s2[i]);
        తిరిగి ఫలితం;
    }
};

మూస<రకం_పేరు T>
స్థిర అంతఃవరుస ద్విగుణ maskedNormDiffL2_16(స్థిరరాశి T* s1, స్థిరరాశి T* s2, స్థిరరాశి uchar* ముసుగు, పూర్ణాంకం len, పూర్ణాంకం cn) {
    ఒకవేళ (cn != 1) తిరిగి masked_Norm_Diff_L2_తోక<T, ద్విగుణ>(s1, s2, ముసుగు, len, cn, 0.0);
    పూర్ణాంకం i = 0; స్థిరరాశి పూర్ణాంకం vstep = VTraits<v_uint16>::vlanes();
    v_float64 acc = vx_setzero_f64();
    కొరకు (; i <= len - vstep; i += vstep) {
        v_uint16 m  = v_gt(vx_ఎక్కించు_expand(ముసుగు + i), vx_setzero_u16());
        v_uint16 ad = v_and(v_absdiff(vx_ఎక్కించు(s1 + i), vx_ఎక్కించు(s2 + i)), m);
        v_uint64 u = v_dotprod_expand_fast(ad, ad);
        acc = v_కూడు(acc, v_cvt_f64(v_reinterpret_as_s64(u)));
    }
    ద్విగుణ ఫలితం = v_reduce_కూడిక(acc);
    కొరకు (; i < len; i++) ఒకవేళ (ముసుగు[i]) { ద్విగుణ v = (ద్విగుణ)s1[i] - (ద్విగుణ)s2[i]; ఫలితం += v*v; }
    తిరిగి ఫలితం;
}
మూస<> నిర్మాణం MaskedNormDiffL2_SIMD<ushort, ద్విగుణ> {
    ద్విగుణ క్రియాకారకం()(స్థిరరాశి ushort* s1, స్థిరరాశి ushort* s2, స్థిరరాశి uchar* ముసుగు, పూర్ణాంకం len, పూర్ణాంకం cn) స్థిరరాశి
    { తిరిగి maskedNormDiffL2_16(s1, s2, ముసుగు, len, cn); } };
మూస<> నిర్మాణం MaskedNormDiffL2_SIMD<పొట్టి, ద్విగుణ> {
    ద్విగుణ క్రియాకారకం()(స్థిరరాశి పొట్టి* s1, స్థిరరాశి పొట్టి* s2, స్థిరరాశి uchar* ముసుగు, పూర్ణాంకం len, పూర్ణాంకం cn) స్థిరరాశి
    { తిరిగి maskedNormDiffL2_16(s1, s2, ముసుగు, len, cn); } };

మూస<> నిర్మాణం MaskedNormDiffL2_SIMD<పూర్ణాంకం, ద్విగుణ> {
    ద్విగుణ క్రియాకారకం()(స్థిరరాశి పూర్ణాంకం* s1, స్థిరరాశి పూర్ణాంకం* s2, స్థిరరాశి uchar* ముసుగు, పూర్ణాంకం len, పూర్ణాంకం cn) స్థిరరాశి {
        ఒకవేళ (cn != 1) తిరిగి masked_Norm_Diff_L2_తోక<పూర్ణాంకం, ద్విగుణ>(s1, s2, ముసుగు, len, cn, 0.0);
        పూర్ణాంకం i = 0; స్థిరరాశి పూర్ణాంకం vstep = VTraits<v_int32>::vlanes();
        v_float64 r0 = vx_setzero_f64(), r1 = vx_setzero_f64();
        కొరకు (; i <= len - vstep; i += vstep) {
            v_uint32 m  = v_gt(vx_ఎక్కించు_expand_q(ముసుగు + i), vx_setzero_u32());
            v_uint32 ad = v_and(v_absdiff(vx_ఎక్కించు(s1 + i), vx_ఎక్కించు(s2 + i)), m);
            v_uint64 e0, e1; v_expand(ad, e0, e1);
            v_float64 f0 = v_cvt_f64(v_reinterpret_as_s64(e0)), f1 = v_cvt_f64(v_reinterpret_as_s64(e1));
            r0 = v_fma(f0, f0, r0); r1 = v_fma(f1, f1, r1);
        }
        ద్విగుణ ఫలితం = v_reduce_కూడిక(v_కూడు(r0, r1));
        కొరకు (; i < len; i++) ఒకవేళ (ముసుగు[i]) { ద్విగుణ v = (ద్విగుణ)s1[i] - (ద్విగుణ)s2[i]; ఫలితం += v*v; }
        తిరిగి ఫలితం;
    }
};
మూస<> నిర్మాణం MaskedNormDiffL2_SIMD<దశాంశం, ద్విగుణ> {
    ద్విగుణ క్రియాకారకం()(స్థిరరాశి దశాంశం* s1, స్థిరరాశి దశాంశం* s2, స్థిరరాశి uchar* ముసుగు, పూర్ణాంకం len, పూర్ణాంకం cn) స్థిరరాశి {
        ఒకవేళ (cn != 1) తిరిగి masked_Norm_Diff_L2_తోక<దశాంశం, ద్విగుణ>(s1, s2, ముసుగు, len, cn, 0.0);
        పూర్ణాంకం i = 0; స్థిరరాశి పూర్ణాంకం vstep = VTraits<v_float32>::vlanes();
        v_float64 r0 = vx_setzero_f64(), r1 = vx_setzero_f64();
        కొరకు (; i <= len - vstep; i += vstep) {
            v_float32 m  = v_reinterpret_as_f32(v_gt(vx_ఎక్కించు_expand_q(ముసుగు + i), vx_setzero_u32()));
            v_float32 ad = v_and(v_absdiff(vx_ఎక్కించు(s1 + i), vx_ఎక్కించు(s2 + i)), m);
            v_float64 f0 = v_cvt_f64(ad), f1 = v_cvt_f64_ఎగువ(ad);
            r0 = v_fma(f0, f0, r0); r1 = v_fma(f1, f1, r1);
        }
        ద్విగుణ ఫలితం = v_reduce_కూడిక(v_కూడు(r0, r1));
        కొరకు (; i < len; i++) ఒకవేళ (ముసుగు[i]) { ద్విగుణ v = (ద్విగుణ)s1[i] - (ద్విగుణ)s2[i]; ఫలితం += v*v; }
        తిరిగి ఫలితం;
    }
};
మూస<>
నిర్మాణం MaskedNormDiffInf_SIMD<ద్విగుణ, ద్విగుణ> {
    అంతఃవరుస ద్విగుణ క్రియాకారకం()(స్థిరరాశి ద్విగుణ* s1, స్థిరరాశి ద్విగుణ* s2, స్థిరరాశి uchar* ముసుగు, పూర్ణాంకం len, పూర్ణాంకం cn) స్థిరరాశి {
        ద్విగుణ ఫలితం = 0.0;
        పూర్ణాంకం i = 0;
#ఒకవేళ CV_SIMD_64F || CV_SIMD_SCALABLE_64F
        ఒకవేళ (cn == 1) {
            స్థిరరాశి పూర్ణాంకం vstep = VTraits<v_float64>::vlanes();
            v_float64 acc0 = vx_setzero_f64(), acc1 = vx_setzero_f64();
            కొరకు (; i <= len - 2*vstep; i += 2*vstep) {
                v_uint64 m0, m1;
                v_normmask_u64(ముసుగు + i, m0, m1);
                v_float64 d0 = v_absdiff(vx_ఎక్కించు(s1 + i), vx_ఎక్కించు(s2 + i));
                v_float64 d1 = v_absdiff(vx_ఎక్కించు(s1 + i + vstep), vx_ఎక్కించు(s2 + i + vstep));
                acc0 = v_max(acc0, v_reinterpret_as_f64(v_and(v_reinterpret_as_u64(d0), m0)));
                acc1 = v_max(acc1, v_reinterpret_as_f64(v_and(v_reinterpret_as_u64(d1), m1)));
            }
            ద్విగుణ t[VTraits<v_float64>::max_nlanes];
            vx_store(t, v_max(acc0, acc1));
            కొరకు (పూర్ణాంకం k = 0; k < vstep; k++)
                ఫలితం = std::max(ఫలితం, t[k]);
        }
#అయితే_ముగింపు
        కొరకు (; i < len; i++) ఒకవేళ (ముసుగు[i]) {
            స్థిరరాశి ద్విగుణ* e1 = s1 + i*cn; స్థిరరాశి ద్విగుణ* e2 = s2 + i*cn;
            కొరకు (పూర్ణాంకం k = 0; k < cn; k++) ఫలితం = std::max(ఫలితం, std::నిరపేక్ష(e1[k] - e2[k]));
        }
        తిరిగి ఫలితం;
    }
};

మూస<>
నిర్మాణం MaskedNormDiffL1_SIMD<ద్విగుణ, ద్విగుణ> {
    ద్విగుణ క్రియాకారకం()(స్థిరరాశి ద్విగుణ* s1, స్థిరరాశి ద్విగుణ* s2, స్థిరరాశి uchar* ముసుగు, పూర్ణాంకం len, పూర్ణాంకం cn) స్థిరరాశి {
        ద్విగుణ ఫలితం = 0.0;
        ఒకవేళ (cn == 1) {
            స్థిరరాశి పూర్ణాంకం vstep = VTraits<v_float64>::vlanes();
            పూర్ణాంకం i = 0;
            v_float64 acc0 = vx_setzero_f64(), acc1 = vx_setzero_f64();
            కొరకు (; i <= len - 2*vstep; i += 2*vstep) {
                v_uint64 m0, m1;
                v_normmask_u64(ముసుగు + i, m0, m1);
                v_float64 d0 = v_absdiff(vx_ఎక్కించు(s1 + i), vx_ఎక్కించు(s2 + i));
                v_float64 d1 = v_absdiff(vx_ఎక్కించు(s1 + i + vstep), vx_ఎక్కించు(s2 + i + vstep));
                acc0 = v_కూడు(acc0, v_reinterpret_as_f64(v_and(v_reinterpret_as_u64(d0), m0)));
                acc1 = v_కూడు(acc1, v_reinterpret_as_f64(v_and(v_reinterpret_as_u64(d1), m1)));
            }
            ఫలితం = v_reduce_కూడిక(v_కూడు(acc0, acc1));
            కొరకు (; i < len; i++) ఒకవేళ (ముసుగు[i]) ఫలితం += std::నిరపేక్ష(s1[i] - s2[i]);
        } లేకపోతే {
            ఫలితం = masked_Norm_Diff_L1_తోక<ద్విగుణ, ద్విగుణ>(s1, s2, ముసుగు, len, cn, 0.0);
        }
        తిరిగి ఫలితం;
    }
};

మూస<>
నిర్మాణం MaskedNormDiffL2_SIMD<ద్విగుణ, ద్విగుణ> {
    ద్విగుణ క్రియాకారకం()(స్థిరరాశి ద్విగుణ* s1, స్థిరరాశి ద్విగుణ* s2, స్థిరరాశి uchar* ముసుగు, పూర్ణాంకం len, పూర్ణాంకం cn) స్థిరరాశి {
        ద్విగుణ ఫలితం = 0.0;
        ఒకవేళ (cn == 1) {
            స్థిరరాశి పూర్ణాంకం vstep = VTraits<v_float64>::vlanes();
            పూర్ణాంకం i = 0;
            v_float64 acc0 = vx_setzero_f64(), acc1 = vx_setzero_f64();
            కొరకు (; i <= len - 2*vstep; i += 2*vstep) {
                v_uint64 m0, m1;
                v_normmask_u64(ముసుగు + i, m0, m1);
                v_float64 d0 = v_sub(vx_ఎక్కించు(s1 + i), vx_ఎక్కించు(s2 + i));
                v_float64 d1 = v_sub(vx_ఎక్కించు(s1 + i + vstep), vx_ఎక్కించు(s2 + i + vstep));
                d0 = v_reinterpret_as_f64(v_and(v_reinterpret_as_u64(d0), m0));
                d1 = v_reinterpret_as_f64(v_and(v_reinterpret_as_u64(d1), m1));
                acc0 = v_fma(d0, d0, acc0);
                acc1 = v_fma(d1, d1, acc1);
            }
            ఫలితం = v_reduce_కూడిక(v_కూడు(acc0, acc1));
            కొరకు (; i < len; i++) ఒకవేళ (ముసుగు[i]) { ద్విగుణ v = s1[i] - s2[i]; ఫలితం += v*v; }
        } లేకపోతే {
            ఫలితం = masked_Norm_Diff_L2_తోక<ద్విగుణ, ద్విగుణ>(s1, s2, ముసుగు, len, cn, 0.0);
        }
        తిరిగి ఫలితం;
    }
};
#అయితే_ముగింపు // CV_SIMD_64F
#అయితే_ముగింపు // CV_SIMD

మూస<రకం_పేరు T, రకం_పేరు ST> పూర్ణాంకం
normDiffInf_(స్థిరరాశి T* src1, స్థిరరాశి T* src2, స్థిరరాశి uchar* ముసుగు, ST* ఫలితం_2, పూర్ణాంకం len, పూర్ణాంకం cn) {
    ST ఫలితం = *ఫలితం_2;
    ఒకవేళ( !ముసుగు ) {
        NormDiffInf_SIMD<T, ST> op;
        ఫలితం = std::max(ఫలితం, op(src1, src2, len*cn));
    } లేకపోతే {
        MaskedNormDiffInf_SIMD<T, ST> op;
        ఫలితం = std::max(ఫలితం, op(src1, src2, ముసుగు, len, cn));
    }
    *ఫలితం_2 = ఫలితం;
    తిరిగి 0;
}

మూస<రకం_పేరు T, రకం_పేరు ST> పూర్ణాంకం
normDiffL1_(స్థిరరాశి T* src1, స్థిరరాశి T* src2, స్థిరరాశి uchar* ముసుగు, ST* ఫలితం_2, పూర్ణాంకం len, పూర్ణాంకం cn) {
    ST ఫలితం = *ఫలితం_2;
    ఒకవేళ( !ముసుగు ) {
        NormDiffL1_SIMD<T, ST> op;
        ఫలితం += op(src1, src2, len*cn);
    } లేకపోతే {
        MaskedNormDiffL1_SIMD<T, ST> op;
        ఫలితం += op(src1, src2, ముసుగు, len, cn);
    }
    *ఫలితం_2 = ఫలితం;
    తిరిగి 0;
}

మూస<రకం_పేరు T, రకం_పేరు ST> పూర్ణాంకం
normDiffL2_(స్థిరరాశి T* src1, స్థిరరాశి T* src2, స్థిరరాశి uchar* ముసుగు, ST* ఫలితం_2, పూర్ణాంకం len, పూర్ణాంకం cn) {
    ST ఫలితం = *ఫలితం_2;
    ఒకవేళ( !ముసుగు ) {
        NormDiffL2_SIMD<T, ST> op;
        ఫలితం += op(src1, src2, len*cn);
    } లేకపోతే {
        MaskedNormDiffL2_SIMD<T, ST> op;
        ఫలితం += op(src1, src2, ముసుగు, len, cn);
    }
    *ఫలితం_2 = ఫలితం;
    తిరిగి 0;
}

స్థిర పూర్ణాంకం
normDiffInf_Bool(స్థిరరాశి uchar* src1, స్థిరరాశి uchar* src2, స్థిరరాశి uchar* ముసుగు, పూర్ణాంకం* ఫలితం_2, పూర్ణాంకం len, పూర్ణాంకం cn)
{
    పూర్ణాంకం ఫలితం = *ఫలితం_2;
    ఒకవేళ( !ముసుగు )
    {
        కొరకు( పూర్ణాంకం i = 0; i < len*cn; i++ ) {
            ఫలితం = std::max(ఫలితం, (పూర్ణాంకం)((src1[i] != 0) != (src2[i] != 0)));
            ఒకవేళ (ఫలితం != 0)
                ఆపు;
        }
    }
    లేకపోతే
    {
        కొరకు( పూర్ణాంకం i = 0; i < len; i++, src1 += cn, src2 += cn )
            ఒకవేళ( ముసుగు[i] )
            {
                కొరకు( పూర్ణాంకం k = 0; k < cn; k++ )
                    ఫలితం = std::max(ఫలితం, (పూర్ణాంకం)((src1[k] != 0) != (src2[k] != 0)));
                ఒకవేళ (ఫలితం != 0)
                    ఆపు;
            }
    }
    *ఫలితం_2 = ఫలితం;
    తిరిగి 0;
}

స్థిర పూర్ణాంకం
normDiffL1_Bool(స్థిరరాశి uchar* src1, స్థిరరాశి uchar* src2, స్థిరరాశి uchar* ముసుగు, పూర్ణాంకం* ఫలితం_2, పూర్ణాంకం len, పూర్ణాంకం cn)
{
    పూర్ణాంకం ఫలితం = *ఫలితం_2;
    ఒకవేళ( !ముసుగు )
    {
        కొరకు( పూర్ణాంకం i = 0; i < len*cn; i++ )
            ఫలితం += (పూర్ణాంకం)((src1[i] != 0) != (src2[i] != 0));
    }
    లేకపోతే
    {
        కొరకు( పూర్ణాంకం i = 0; i < len; i++, src1 += cn, src2 += cn )
            ఒకవేళ( ముసుగు[i] )
            {
                కొరకు( పూర్ణాంకం k = 0; k < cn; k++ )
                    ఫలితం += (పూర్ణాంకం)((src1[k] != 0) != (src2[k] != 0));
            }
    }
    *ఫలితం_2 = ఫలితం;
    తిరిగి 0;
}

స్థిర పూర్ణాంకం
normDiffL2_Bool(స్థిరరాశి uchar* src1, స్థిరరాశి uchar* src2, స్థిరరాశి uchar* ముసుగు, పూర్ణాంకం* ఫలితం_2, పూర్ణాంకం len, పూర్ణాంకం cn)
{
    తిరిగి normDiffL1_Bool(src1, src2, ముసుగు, ఫలితం_2, len, cn);
}

#నిర్వచించు CV_DEF_NORM_FUNC(L, suffix, type, ntype) \
    స్థిర పూర్ణాంకం norm##L##_##suffix(స్థిరరాశి type* src, స్థిరరాశి uchar* ముసుగు, ntype* r, పూర్ణాంకం len, పూర్ణాంకం cn) \
{ CV_INSTRUMENT_REGION(); తిరిగి norm##L##_(src, ముసుగు, r, len, cn); } \
    స్థిర పూర్ణాంకం normDiff##L##_##suffix(స్థిరరాశి type* src1, స్థిరరాశి type* src2, \
    స్థిరరాశి uchar* ముసుగు, ntype* r, పూర్ణాంకం len, పూర్ణాంకం cn) \
{ తిరిగి normDiff##L##_(src1, src2, ముసుగు, r, (పూర్ణాంకం)len, cn); }

#నిర్వచించు CV_DEF_NORM_ALL(suffix, type, inftype, l1type, l2type) \
    CV_DEF_NORM_FUNC(Inf, suffix, type, inftype) \
    CV_DEF_NORM_FUNC(L1, suffix, type, l1type) \
    CV_DEF_NORM_FUNC(L2, suffix, type, l2type)

CV_DEF_NORM_ALL(8u, uchar, పూర్ణాంకం, పూర్ణాంకం, పూర్ణాంకం)
CV_DEF_NORM_ALL(8s, schar, పూర్ణాంకం, పూర్ణాంకం, పూర్ణాంకం)
CV_DEF_NORM_ALL(16u, ushort, పూర్ణాంకం, పూర్ణాంకం, ద్విగుణ)
CV_DEF_NORM_ALL(16s, పొట్టి, పూర్ణాంకం, పూర్ణాంకం, ద్విగుణ)
CV_DEF_NORM_ALL(32u, గుర్తులేని, గుర్తులేని, ద్విగుణ, ద్విగుణ)
CV_DEF_NORM_ALL(32s, పూర్ణాంకం, గుర్తులేని, ద్విగుణ, ద్విగుణ)
CV_DEF_NORM_ALL(32f, దశాంశం, దశాంశం, ద్విగుణ, ద్విగుణ)
CV_DEF_NORM_ALL(64f, ద్విగుణ, ద్విగుణ, ద్విగుణ, ద్విగుణ)
CV_DEF_NORM_ALL(64u, uint64, uint64, ద్విగుణ, ద్విగుణ)
CV_DEF_NORM_ALL(64s, int64, uint64, ద్విగుణ, ద్విగుణ)
CV_DEF_NORM_ALL(16f, hfloat, దశాంశం, దశాంశం, దశాంశం)
CV_DEF_NORM_ALL(16bf, bfloat, దశాంశం, దశాంశం, దశాంశం)

NormFunc getNormFunc(పూర్ణాంకం normType, పూర్ణాంకం లోతు)
{
    CV_INSTRUMENT_REGION();

    స్థిర NormFunc normTab[3][CV_లోతు_MAX] =
    {
        {
            normWrap<uchar, పూర్ణాంకం, normInf_8u>,
            normWrap<schar, పూర్ణాంకం, normInf_8s>,
            normWrap<ushort, పూర్ణాంకం, normInf_16u>,
            normWrap<పొట్టి, పూర్ణాంకం, normInf_16s>,
            normWrap<పూర్ణాంకం, గుర్తులేని, normInf_32s>,
            normWrap<దశాంశం, దశాంశం, normInf_32f>,
            normWrap<ద్విగుణ, ద్విగుణ, normInf_64f>,
            normWrap<hfloat, దశాంశం, normInf_16f>,
            normWrap<bfloat, దశాంశం, normInf_16bf>,
            normWrap<uchar, పూర్ణాంకం, normInf_Bool>,
            normWrap<uint64, uint64, normInf_64u>,
            normWrap<int64, uint64, normInf_64s>,
            normWrap<గుర్తులేని, గుర్తులేని, normInf_32u>,
            0
        },
        {
            normWrap<uchar, పూర్ణాంకం, normL1_8u>,
            normWrap<schar, పూర్ణాంకం, normL1_8s>,
            normWrap<ushort, పూర్ణాంకం, normL1_16u>,
            normWrap<పొట్టి, పూర్ణాంకం, normL1_16s>,
            normWrap<పూర్ణాంకం, ద్విగుణ, normL1_32s>,
            normWrap<దశాంశం, ద్విగుణ, normL1_32f>,
            normWrap<ద్విగుణ, ద్విగుణ, normL1_64f>,
            normWrap<hfloat, దశాంశం, normL1_16f>,
            normWrap<bfloat, దశాంశం, normL1_16bf>,
            normWrap<uchar, పూర్ణాంకం, normL1_Bool>,
            normWrap<uint64, ద్విగుణ, normL1_64u>,
            normWrap<int64, ద్విగుణ, normL1_64s>,
            normWrap<గుర్తులేని, ద్విగుణ, normL1_32u>,
            0
        },
        {
            normWrap<uchar, పూర్ణాంకం, normL2_8u>,
            normWrap<schar, పూర్ణాంకం, normL2_8s>,
            normWrap<ushort, ద్విగుణ, normL2_16u>,
            normWrap<పొట్టి, ద్విగుణ, normL2_16s>,
            normWrap<పూర్ణాంకం, ద్విగుణ, normL2_32s>,
            normWrap<దశాంశం, ద్విగుణ, normL2_32f>,
            normWrap<ద్విగుణ, ద్విగుణ, normL2_64f>,
            normWrap<hfloat, దశాంశం, normL2_16f>,
            normWrap<bfloat, దశాంశం, normL2_16bf>,
            normWrap<uchar, పూర్ణాంకం, normL2_Bool>,
            normWrap<uint64, ద్విగుణ, normL2_64u>,
            normWrap<int64, ద్విగుణ, normL2_64s>,
            normWrap<గుర్తులేని, ద్విగుణ, normL2_32u>,
            0
        }
    };

    ఒకవేళ (normType >= 3 || normType < 0) తిరిగి శూన్యసూచిక;

    తిరిగి normTab[normType][లోతు];
}

NormDiffFunc getNormDiffFunc(పూర్ణాంకం normType, పూర్ణాంకం లోతు)
{
    స్థిర NormDiffFunc normDiffTab[3][CV_లోతు_MAX] =
    {
        {
            normDiffWrap<uchar, పూర్ణాంకం, normDiffInf_8u>,
            normDiffWrap<schar, పూర్ణాంకం, normDiffInf_8s>,
            normDiffWrap<ushort, పూర్ణాంకం, normDiffInf_16u>,
            normDiffWrap<పొట్టి, పూర్ణాంకం, normDiffInf_16s>,
            normDiffWrap<పూర్ణాంకం, గుర్తులేని, normDiffInf_32s>,
            normDiffWrap<దశాంశం, దశాంశం, normDiffInf_32f>,
            normDiffWrap<ద్విగుణ, ద్విగుణ, normDiffInf_64f>,
            normDiffWrap<hfloat, దశాంశం, normDiffInf_16f>,
            normDiffWrap<bfloat, దశాంశం, normDiffInf_16bf>,
            normDiffWrap<uchar, పూర్ణాంకం, normDiffInf_Bool>,
            normDiffWrap<uint64, uint64, normDiffInf_64u>,
            normDiffWrap<int64, uint64, normDiffInf_64s>,
            normDiffWrap<గుర్తులేని, గుర్తులేని, normDiffInf_32u>,
            0
        },
        {
            normDiffWrap<uchar, పూర్ణాంకం, normDiffL1_8u>,
            normDiffWrap<schar, పూర్ణాంకం, normDiffL1_8s>,
            normDiffWrap<ushort, పూర్ణాంకం, normDiffL1_16u>,
            normDiffWrap<పొట్టి, పూర్ణాంకం, normDiffL1_16s>,
            normDiffWrap<పూర్ణాంకం, ద్విగుణ, normDiffL1_32s>,
            normDiffWrap<దశాంశం, ద్విగుణ, normDiffL1_32f>,
            normDiffWrap<ద్విగుణ, ద్విగుణ, normDiffL1_64f>,
            normDiffWrap<hfloat, దశాంశం, normDiffL1_16f>,
            normDiffWrap<bfloat, దశాంశం, normDiffL1_16bf>,
            normDiffWrap<uchar, పూర్ణాంకం, normDiffL1_Bool>,
            normDiffWrap<uint64, ద్విగుణ, normDiffL1_64u>,
            normDiffWrap<int64, ద్విగుణ, normDiffL1_64s>,
            normDiffWrap<గుర్తులేని, ద్విగుణ, normDiffL1_32u>,
            0
        },
        {
            normDiffWrap<uchar, పూర్ణాంకం, normDiffL2_8u>,
            normDiffWrap<schar, పూర్ణాంకం, normDiffL2_8s>,
            normDiffWrap<ushort, ద్విగుణ, normDiffL2_16u>,
            normDiffWrap<పొట్టి, ద్విగుణ, normDiffL2_16s>,
            normDiffWrap<పూర్ణాంకం, ద్విగుణ, normDiffL2_32s>,
            normDiffWrap<దశాంశం, ద్విగుణ, normDiffL2_32f>,
            normDiffWrap<ద్విగుణ, ద్విగుణ, normDiffL2_64f>,
            normDiffWrap<hfloat, దశాంశం, normDiffL2_16f>,
            normDiffWrap<bfloat, దశాంశం, normDiffL2_16bf>,
            normDiffWrap<uchar, పూర్ణాంకం, normDiffL2_Bool>,
            normDiffWrap<uint64, ద్విగుణ, normDiffL2_64u>,
            normDiffWrap<int64, ద్విగుణ, normDiffL2_64s>,
            normDiffWrap<గుర్తులేని, ద్విగుణ, normDiffL2_32u>,
            0
        },
    };
    ఒకవేళ (normType >= 3 || normType < 0) తిరిగి శూన్యసూచిక;

    తిరిగి normDiffTab[normType][లోతు];
}

#అయితే_ముగింపు // CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

CV_CPU_OPTIMIZATION_NAMESPACE_చివర

} // cv::
