// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


#అన్తర్భావ "precomp.hpp"
#అన్తర్భావ "stat.hpp"

నామాకాశ cv {
CV_CPU_OPTIMIZATION_NAMESPACE_BEGIN

మూస<రకం_పేరు T, రకం_పేరు ST, పూర్ణాంకం (*fn)(స్థిరరాశి T*, స్థిరరాశి uchar*, ST*, పూర్ణాంకం, పూర్ణాంకం)>
స్థిర పూర్ణాంకం కూడిక_Wrap(స్థిరరాశి uchar* src, స్థిరరాశి uchar* ముసుగు, uchar* కూడిక, పూర్ణాంకం len, పూర్ణాంకం cn)
{
    తిరిగి fn((స్థిరరాశి T*)src, ముసుగు, (ST*)కూడిక, len, cn);
}

కూడిక_Func get_కూడిక_Func(పూర్ణాంకం లోతు);

#నిర్వచితం_కాకపోతే CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

మూస <రకం_పేరు T, రకం_పేరు ST>
నిర్మాణం కూడిక_SIMD
{
    కూడిక_SIMD(పూర్ణాంకం) {}
    పూర్ణాంకం క్రియాకారకం () (స్థిరరాశి T*, స్థిరరాశి uchar*, ST*, పూర్ణాంకం, పూర్ణాంకం) స్థిరరాశి
    {
        తిరిగి 0;
    }
};

#ఒకవేళ (CV_SIMD || CV_SIMD_SCALABLE)

#నిర్వచనం_రద్దు REDUCE_PARTIAL_SUMS
#నిర్వచించు REDUCE_PARTIAL_SUMS() \
    ఒకవేళ (cn == 1) \
        dst[0] += v_reduce_కూడిక(v_కూడు(v_కూడు(s0, s1), s2)); \
    లేకపోతే ఒకవేళ (cn == 2) { \
        s0 = v_కూడు(v_కూడు(s0, s1), s2); \
        dst[0] += v_reduce_కూడిక(v_and(s0, m0)); \
        dst[1] += v_reduce_కూడిక(v_and(s0, m1)); \
    } లేకపోతే ఒకవేళ (cn == 3) { \
        dst[0] += v_reduce_కూడిక(v_కూడు(v_కూడు(v_and(s0, m0), v_and(s1, m1)), v_and(s2, m2))); \
        dst[1] += v_reduce_కూడిక(v_కూడు(v_కూడు(v_and(s0, m3), v_and(s1, m4)), v_and(s2, m5))); \
        dst[2] += v_reduce_కూడిక(v_కూడు(v_కూడు(v_and(s0, m6), v_and(s1, m7)), v_and(s2, m8))); \
    } లేకపోతే ఒకవేళ (cn == 4) { \
        s0 = v_కూడు(v_కూడు(s0, s1), s2); \
        dst[0] += v_reduce_కూడిక(v_and(s0, m0)); \
        dst[1] += v_reduce_కూడిక(v_and(s0, m1)); \
        dst[2] += v_reduce_కూడిక(v_and(s0, m2)); \
        dst[3] += v_reduce_కూడిక(v_and(s0, m3)); \
    }

మూస<రకం_పేరు ST>
స్థిర శూన్య init_maskbuf(ST* maskbuf, పూర్ణాంకం cn, పూర్ణాంకం simd_వెడల్పు)
{
    memset(maskbuf, 0, simd_వెడల్పు*9*పరిమాణం(maskbuf[0]));
    ఒకవేళ (cn == 1)
        ;
    లేకపోతే ఒకవేళ (cn == 2)
        కొరకు (పూర్ణాంకం i = 0; i < simd_వెడల్పు; i += 2) {
            maskbuf[i] = (ST)-1;
            maskbuf[i+1+simd_వెడల్పు] = (ST)-1;
        }
    లేకపోతే ఒకవేళ (cn == 3)
        కొరకు (పూర్ణాంకం i = 0; i < simd_వెడల్పు*3; i += 3) {
            maskbuf[i] = (ST)-1;
            maskbuf[i+1+simd_వెడల్పు*3] = (ST)-1;
            maskbuf[i+2+simd_వెడల్పు*6] = (ST)-1;
        }
    లేకపోతే ఒకవేళ (cn == 4 && simd_వెడల్పు >= 4) {
        కొరకు (పూర్ణాంకం i = 0; i < simd_వెడల్పు; i += 4) {
            maskbuf[i] = (ST)-1;
            maskbuf[i+1+simd_వెడల్పు] = (ST)-1;
            maskbuf[i+2+simd_వెడల్పు*2] = (ST)-1;
            maskbuf[i+3+simd_వెడల్పు*3] = (ST)-1;
        }
    }
}

#నిర్వచనం_రద్దు DEFINE_కూడిక_SIMD_8
#నిర్వచించు DEFINE_కూడిక_SIMD_8(T, ST, iST, VecT, ఎక్కించు_op) \
మూస<> నిర్మాణం కూడిక_SIMD<T, ST> \
{ \
    కూడిక_SIMD(పూర్ణాంకం cn) \
    { \
        init_maskbuf((iST*)maskbuf, cn, VTraits<VecT>::vlanes()); \
    } \
    పూర్ణాంకం క్రియాకారకం ()(స్థిరరాశి T* src, స్థిరరాశి uchar* ముసుగు, ST* dst, పూర్ణాంకం len, పూర్ణాంకం cn) స్థిరరాశి \
    { \
        ఒకవేళ (ముసుగు || (cn < 1 || cn > 4)) \
            తిరిగి 0; \
        len *= cn; \
        పూర్ణాంకం x = 0, simd_వెడల్పు = VTraits<VecT>::vlanes(); \
        VecT m0 = vx_ఎక్కించు(maskbuf), m1, m2, m3, m4, m5, m6, m7, m8; \
        ఒకవేళ (cn == 1) { \
            m1 = m2 = m3 = m4 = m5 = m6 = m7 = m8 = m0; \
        } లేకపోతే { \
            m1 = vx_ఎక్కించు(maskbuf + simd_వెడల్పు); \
            m2 = vx_ఎక్కించు(maskbuf + simd_వెడల్పు*2); \
            m3 = vx_ఎక్కించు(maskbuf + simd_వెడల్పు*3); \
            m4 = vx_ఎక్కించు(maskbuf + simd_వెడల్పు*4); \
            m5 = vx_ఎక్కించు(maskbuf + simd_వెడల్పు*5); \
            m6 = vx_ఎక్కించు(maskbuf + simd_వెడల్పు*6); \
            m7 = vx_ఎక్కించు(maskbuf + simd_వెడల్పు*7); \
            m8 = vx_ఎక్కించు(maskbuf + simd_వెడల్పు*8); \
        } \
        VecT s0 = v_xor(m0, m0), s1 = s0, s2 = s0; \
        కొరకు (; x <= len - simd_వెడల్పు*6; x += simd_వెడల్పు*6) { \
            స్వయంచాలక v0 = ఎక్కించు_op(src + x); \
            స్వయంచాలక v1 = ఎక్కించు_op(src + x + simd_వెడల్పు*2); \
            స్వయంచాలక v2 = ఎక్కించు_op(src + x + simd_వెడల్పు*4); \
            s0 = v_కూడు(s0, v_expand_దిగువ(v0)); \
            s1 = v_కూడు(s1, v_expand_ఎగువ(v0)); \
            s2 = v_కూడు(s2, v_expand_దిగువ(v1)); \
            s0 = v_కూడు(s0, v_expand_ఎగువ(v1)); \
            s1 = v_కూడు(s1, v_expand_దిగువ(v2)); \
            s2 = v_కూడు(s2, v_expand_ఎగువ(v2)); \
        } \
        REDUCE_PARTIAL_SUMS(); \
        vx_cleanup(); \
        తిరిగి x / cn; \
    } \
    ST maskbuf[VTraits<VecT>::max_nlanes*9]; \
};

#నిర్వచనం_రద్దు DEFINE_కూడిక_SIMD_16
#నిర్వచించు DEFINE_కూడిక_SIMD_16(T, ST, iST, VecT, ఎక్కించు_op) \
మూస<> నిర్మాణం కూడిక_SIMD<T, ST> \
{ \
    కూడిక_SIMD(పూర్ణాంకం cn) \
    { \
        init_maskbuf((iST*)maskbuf, cn, VTraits<VecT>::vlanes()); \
    } \
    పూర్ణాంకం క్రియాకారకం ()(స్థిరరాశి T* src, స్థిరరాశి uchar* ముసుగు, ST* dst, పూర్ణాంకం len, పూర్ణాంకం cn) స్థిరరాశి \
    { \
        ఒకవేళ (ముసుగు || (cn < 1 || cn > 4)) \
            తిరిగి 0; \
        len *= cn; \
        పూర్ణాంకం x = 0, simd_వెడల్పు = VTraits<VecT>::vlanes(); \
        VecT m0 = vx_ఎక్కించు(maskbuf), m1, m2, m3, m4, m5, m6, m7, m8; \
        ఒకవేళ (cn == 1) { \
            m1 = m2 = m3 = m4 = m5 = m6 = m7 = m8 = m0; \
        } లేకపోతే { \
            m1 = vx_ఎక్కించు(maskbuf + simd_వెడల్పు); \
            m2 = vx_ఎక్కించు(maskbuf + simd_వెడల్పు*2); \
            m3 = vx_ఎక్కించు(maskbuf + simd_వెడల్పు*3); \
            m4 = vx_ఎక్కించు(maskbuf + simd_వెడల్పు*4); \
            m5 = vx_ఎక్కించు(maskbuf + simd_వెడల్పు*5); \
            m6 = vx_ఎక్కించు(maskbuf + simd_వెడల్పు*6); \
            m7 = vx_ఎక్కించు(maskbuf + simd_వెడల్పు*7); \
            m8 = vx_ఎక్కించు(maskbuf + simd_వెడల్పు*8); \
        } \
        VecT s0 = v_xor(m0, m0), s1 = s0, s2 = s0; \
        కొరకు (; x <= len - simd_వెడల్పు*3; x += simd_వెడల్పు*3) { \
            స్వయంచాలక v0 = ఎక్కించు_op(src + x); \
            స్వయంచాలక v1 = ఎక్కించు_op(src + x + simd_వెడల్పు); \
            స్వయంచాలక v2 = ఎక్కించు_op(src + x + simd_వెడల్పు*2); \
            s0 = v_కూడు(s0, v0); \
            s1 = v_కూడు(s1, v1); \
            s2 = v_కూడు(s2, v2); \
        } \
        REDUCE_PARTIAL_SUMS(); \
        vx_cleanup(); \
        తిరిగి x / cn; \
    } \
    ST maskbuf[VTraits<VecT>::max_nlanes*9]; \
};

#నిర్వచనం_రద్దు ఎక్కించు_u8_as_s16
#నిర్వచనం_రద్దు ఎక్కించు_u16_as_s32
#నిర్వచించు ఎక్కించు_u8_as_s16(addr) v_reinterpret_as_s16(vx_ఎక్కించు_expand(addr))
#నిర్వచించు ఎక్కించు_u16_as_s32(addr) v_reinterpret_as_s32(vx_ఎక్కించు_expand(addr))

DEFINE_కూడిక_SIMD_8(uchar, పూర్ణాంకం, పూర్ణాంకం, v_int32, ఎక్కించు_u8_as_s16)
DEFINE_కూడిక_SIMD_8(schar, పూర్ణాంకం, పూర్ణాంకం, v_int32, vx_ఎక్కించు_expand)
DEFINE_కూడిక_SIMD_16(ushort, పూర్ణాంకం, పూర్ణాంకం, v_int32, ఎక్కించు_u16_as_s32)
DEFINE_కూడిక_SIMD_16(పొట్టి, పూర్ణాంకం, పూర్ణాంకం, v_int32, vx_ఎక్కించు_expand)
DEFINE_కూడిక_SIMD_16(hfloat, దశాంశం, పూర్ణాంకం, v_float32, vx_ఎక్కించు_expand)
DEFINE_కూడిక_SIMD_16(bfloat, దశాంశం, పూర్ణాంకం, v_float32, vx_ఎక్కించు_expand)

#ఒకవేళ (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)

#నిర్వచనం_రద్దు DEFINE_కూడిక_SIMD_32
#నిర్వచించు DEFINE_కూడిక_SIMD_32(T, ST, iST, VecT) \
మూస<> నిర్మాణం కూడిక_SIMD<T, ST> \
{ \
    కూడిక_SIMD(పూర్ణాంకం cn) \
    { \
        init_maskbuf((iST*)maskbuf, cn, VTraits<VecT>::vlanes()); \
    } \
    పూర్ణాంకం క్రియాకారకం ()(స్థిరరాశి T* src, స్థిరరాశి uchar* ముసుగు, ST* dst, పూర్ణాంకం len, పూర్ణాంకం cn) స్థిరరాశి \
    { \
        పూర్ణాంకం x = 0, simd_వెడల్పు = VTraits<VecT>::vlanes(); \
        ఒకవేళ (ముసుగు || (cn < 1 || cn > 3+(simd_వెడల్పు>=4))) \
            తిరిగి 0; \
        len *= cn; \
        VecT m0 = vx_ఎక్కించు(maskbuf), m1, m2, m3, m4, m5, m6, m7, m8; \
        ఒకవేళ (cn == 1) { \
            m1 = m2 = m3 = m4 = m5 = m6 = m7 = m8 = m0; \
        } లేకపోతే { \
            m1 = vx_ఎక్కించు(maskbuf + simd_వెడల్పు); \
            m2 = vx_ఎక్కించు(maskbuf + simd_వెడల్పు*2); \
            m3 = vx_ఎక్కించు(maskbuf + simd_వెడల్పు*3); \
            m4 = vx_ఎక్కించు(maskbuf + simd_వెడల్పు*4); \
            m5 = vx_ఎక్కించు(maskbuf + simd_వెడల్పు*5); \
            m6 = vx_ఎక్కించు(maskbuf + simd_వెడల్పు*6); \
            m7 = vx_ఎక్కించు(maskbuf + simd_వెడల్పు*7); \
            m8 = vx_ఎక్కించు(maskbuf + simd_వెడల్పు*8); \
        } \
        VecT s0 = v_xor(m0, m0), s1 = s0, s2 = s0; \
        కొరకు (; x <= len - simd_వెడల్పు*6; x += simd_వెడల్పు*6) { \
            స్వయంచాలక v0 = vx_ఎక్కించు(src + x); \
            స్వయంచాలక v1 = vx_ఎక్కించు(src + x + simd_వెడల్పు*2); \
            స్వయంచాలక v2 = vx_ఎక్కించు(src + x + simd_వెడల్పు*4); \
            s0 = v_కూడు(s0, v_cvt_f64(v0)); \
            s1 = v_కూడు(s1, v_cvt_f64_ఎగువ(v0)); \
            s2 = v_కూడు(s2, v_cvt_f64(v1)); \
            s0 = v_కూడు(s0, v_cvt_f64_ఎగువ(v1)); \
            s1 = v_కూడు(s1, v_cvt_f64(v2)); \
            s2 = v_కూడు(s2, v_cvt_f64_ఎగువ(v2)); \
        } \
        REDUCE_PARTIAL_SUMS(); \
        vx_cleanup(); \
        తిరిగి x / cn; \
    } \
    ST maskbuf[VTraits<VecT>::max_nlanes*9]; \
};

DEFINE_కూడిక_SIMD_32(పూర్ణాంకం, ద్విగుణ, int64, v_float64)
DEFINE_కూడిక_SIMD_32(దశాంశం, ద్విగుణ, int64, v_float64)
#అయితే_ముగింపు
#అయితే_ముగింపు

మూస<రకం_పేరు T, రకం_పేరు ST, రకం_పేరు WT=T>
స్థిర పూర్ణాంకం కూడిక_2(స్థిరరాశి T* src0, స్థిరరాశి uchar* ముసుగు, ST* dst, పూర్ణాంకం len, పూర్ణాంకం cn )
{
    స్థిరరాశి T* src = src0;
    ఒకవేళ( !ముసుగు )
    {
        కూడిక_SIMD<T, ST> vop(cn);
        పూర్ణాంకం i0 = vop(src0, ముసుగు, dst, len, cn), i = i0, k = cn % 4;
        src += i0 * cn;

        ఒకవేళ( k == 1 )
        {
            ST s0 = dst[0];

            #ఒకవేళ CV_ENABLE_UNROLLED
            కొరకు(; i <= len - 4; i += 4, src += cn*4 )
                s0 += (WT)src[0] + (WT)src[cn] + (WT)src[cn*2] + (WT)src[cn*3];
            #అయితే_ముగింపు
            కొరకు( ; i < len; i++, src += cn )
                s0 += (WT)src[0];
            dst[0] = s0;
        }
        లేకపోతే ఒకవేళ( k == 2 )
        {
            ST s0 = dst[0], s1 = dst[1];
            కొరకు( ; i < len; i++, src += cn )
            {
                s0 += (WT)src[0];
                s1 += (WT)src[1];
            }
            dst[0] = s0;
            dst[1] = s1;
        }
        లేకపోతే ఒకవేళ( k == 3 )
        {
            ST s0 = dst[0], s1 = dst[1], s2 = dst[2];
            కొరకు( ; i < len; i++, src += cn )
            {
                s0 += (WT)src[0];
                s1 += (WT)src[1];
                s2 += (WT)src[2];
            }
            dst[0] = s0;
            dst[1] = s1;
            dst[2] = s2;
        }

        కొరకు( ; k < cn; k += 4 )
        {
            src = src0 + i0*cn + k;
            ST s0 = dst[k], s1 = dst[k+1], s2 = dst[k+2], s3 = dst[k+3];
            కొరకు( i = i0; i < len; i++, src += cn )
            {
                s0 += (WT)src[0]; s1 += (WT)src[1];
                s2 += (WT)src[2]; s3 += (WT)src[3];
            }
            dst[k] = s0;
            dst[k+1] = s1;
            dst[k+2] = s2;
            dst[k+3] = s3;
        }
        తిరిగి len;
    }

    పూర్ణాంకం i, nzm = 0;
    ఒకవేళ( cn == 1 )
    {
        ST s = dst[0];
        కొరకు( i = 0; i < len; i++ )
            ఒకవేళ( ముసుగు[i] )
            {
                s += (WT)src[i];
                nzm++;
            }
        dst[0] = s;
    }
    లేకపోతే ఒకవేళ( cn == 3 )
    {
        ST s0 = dst[0], s1 = dst[1], s2 = dst[2];
        కొరకు( i = 0; i < len; i++, src += 3 )
            ఒకవేళ( ముసుగు[i] )
            {
                s0 += (WT)src[0];
                s1 += (WT)src[1];
                s2 += (WT)src[2];
                nzm++;
            }
        dst[0] = s0;
        dst[1] = s1;
        dst[2] = s2;
    }
    లేకపోతే
    {
        కొరకు( i = 0; i < len; i++, src += cn )
            ఒకవేళ( ముసుగు[i] )
            {
                పూర్ణాంకం k = 0;
                #ఒకవేళ CV_ENABLE_UNROLLED
                కొరకు( ; k <= cn - 4; k += 4 )
                {
                    ST s0, s1;
                    s0 = dst[k] + (WT)src[k];
                    s1 = dst[k+1] + (WT)src[k+1];
                    dst[k] = s0; dst[k+1] = s1;
                    s0 = dst[k+2] + (WT)src[k+2];
                    s1 = dst[k+3] + (WT)src[k+3];
                    dst[k+2] = s0; dst[k+3] = s1;
                }
                #అయితే_ముగింపు
                కొరకు( ; k < cn; k++ )
                    dst[k] += (WT)src[k];
                nzm++;
            }
    }
    తిరిగి nzm;
}


స్థిర పూర్ణాంకం sum8u( స్థిరరాశి uchar* src, స్థిరరాశి uchar* ముసుగు, పూర్ణాంకం* dst, పూర్ణాంకం len, పూర్ణాంకం cn )
{ CV_INSTRUMENT_REGION(); తిరిగి కూడిక_2(src, ముసుగు, dst, len, cn); }

స్థిర పూర్ణాంకం sum8s( స్థిరరాశి schar* src, స్థిరరాశి uchar* ముసుగు, పూర్ణాంకం* dst, పూర్ణాంకం len, పూర్ణాంకం cn )
{ CV_INSTRUMENT_REGION(); తిరిగి కూడిక_2(src, ముసుగు, dst, len, cn); }

స్థిర పూర్ణాంకం sum16u( స్థిరరాశి ushort* src, స్థిరరాశి uchar* ముసుగు, పూర్ణాంకం* dst, పూర్ణాంకం len, పూర్ణాంకం cn )
{ CV_INSTRUMENT_REGION(); తిరిగి కూడిక_2(src, ముసుగు, dst, len, cn); }

స్థిర పూర్ణాంకం sum16s( స్థిరరాశి పొట్టి* src, స్థిరరాశి uchar* ముసుగు, పూర్ణాంకం* dst, పూర్ణాంకం len, పూర్ణాంకం cn )
{ CV_INSTRUMENT_REGION(); తిరిగి కూడిక_2(src, ముసుగు, dst, len, cn); }

స్థిర పూర్ణాంకం sum32u( స్థిరరాశి గుర్తులేని* src, స్థిరరాశి uchar* ముసుగు, ద్విగుణ* dst, పూర్ణాంకం len, పూర్ణాంకం cn )
{ CV_INSTRUMENT_REGION(); తిరిగి కూడిక_2<గుర్తులేని, ద్విగుణ, uint64>(src, ముసుగు, dst, len, cn); }

స్థిర పూర్ణాంకం sum32s( స్థిరరాశి పూర్ణాంకం* src, స్థిరరాశి uchar* ముసుగు, ద్విగుణ* dst, పూర్ణాంకం len, పూర్ణాంకం cn )
{ CV_INSTRUMENT_REGION(); తిరిగి కూడిక_2<పూర్ణాంకం, ద్విగుణ, int64>(src, ముసుగు, dst, len, cn); }

స్థిర పూర్ణాంకం sum64u( స్థిరరాశి uint64* src, స్థిరరాశి uchar* ముసుగు, ద్విగుణ* dst, పూర్ణాంకం len, పూర్ణాంకం cn )
{ CV_INSTRUMENT_REGION(); తిరిగి కూడిక_2(src, ముసుగు, dst, len, cn); }

స్థిర పూర్ణాంకం sum64s( స్థిరరాశి int64* src, స్థిరరాశి uchar* ముసుగు, ద్విగుణ* dst, పూర్ణాంకం len, పూర్ణాంకం cn )
{ CV_INSTRUMENT_REGION(); తిరిగి కూడిక_2(src, ముసుగు, dst, len, cn); }

స్థిర పూర్ణాంకం sum32f( స్థిరరాశి దశాంశం* src, స్థిరరాశి uchar* ముసుగు, ద్విగుణ* dst, పూర్ణాంకం len, పూర్ణాంకం cn )
{ CV_INSTRUMENT_REGION(); తిరిగి కూడిక_2(src, ముసుగు, dst, len, cn); }

స్థిర పూర్ణాంకం sum64f( స్థిరరాశి ద్విగుణ* src, స్థిరరాశి uchar* ముసుగు, ద్విగుణ* dst, పూర్ణాంకం len, పూర్ణాంకం cn )
{ CV_INSTRUMENT_REGION(); తిరిగి కూడిక_2(src, ముసుగు, dst, len, cn); }

స్థిర పూర్ణాంకం sum16f( స్థిరరాశి hfloat* src, స్థిరరాశి uchar* ముసుగు, దశాంశం* dst, పూర్ణాంకం len, పూర్ణాంకం cn )
{ CV_INSTRUMENT_REGION(); తిరిగి కూడిక_2<hfloat, దశాంశం, దశాంశం>(src, ముసుగు, dst, len, cn); }

స్థిర పూర్ణాంకం sum16bf( స్థిరరాశి bfloat* src, స్థిరరాశి uchar* ముసుగు, దశాంశం* dst, పూర్ణాంకం len, పూర్ణాంకం cn )
{ CV_INSTRUMENT_REGION(); తిరిగి కూడిక_2<bfloat, దశాంశం, దశాంశం>(src, ముసుగు, dst, len, cn); }

కూడిక_Func get_కూడిక_Func(పూర్ణాంకం లోతు)
{
    స్థిర కూడిక_Func కూడిక_Tab[CV_లోతు_MAX] =
    {
        కూడిక_Wrap<uchar, పూర్ణాంకం, sum8u>,
        కూడిక_Wrap<schar, పూర్ణాంకం, sum8s>,
        కూడిక_Wrap<ushort, పూర్ణాంకం, sum16u>,
        కూడిక_Wrap<పొట్టి, పూర్ణాంకం, sum16s>,
        కూడిక_Wrap<పూర్ణాంకం, ద్విగుణ, sum32s>,
        కూడిక_Wrap<దశాంశం, ద్విగుణ, sum32f>,
        కూడిక_Wrap<ద్విగుణ, ద్విగుణ, sum64f>,
        కూడిక_Wrap<hfloat, దశాంశం, sum16f>,
        కూడిక_Wrap<bfloat, దశాంశం, sum16bf>,
        0,
        కూడిక_Wrap<uint64, ద్విగుణ, sum64u>,
        కూడిక_Wrap<int64, ద్విగుణ, sum64s>,
        కూడిక_Wrap<గుర్తులేని, ద్విగుణ, sum32u>,
        0
    };

    తిరిగి కూడిక_Tab[లోతు];
}

#అయితే_ముగింపు

CV_CPU_OPTIMIZATION_NAMESPACE_చివర
} // namespace
