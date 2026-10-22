// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


#अन्तर्भाव "precomp.hpp"
#अन्तर्भाव "stat.hpp"

नामाकाश cv {
CV_CPU_OPTIMIZATION_NAMESPACE_BEGIN

प्रतिरूपम्<प्रकारनाम T, प्रकारनाम ST, पूर्णाङ्क (*fn)(ध्रुव T*, ध्रुव uchar*, ST*, पूर्णाङ्क, पूर्णाङ्क)>
स्थिर पूर्णाङ्क योग_Wrap(ध्रुव uchar* src, ध्रुव uchar* आवरणम्, uchar* योग_2, पूर्णाङ्क len, पूर्णाङ्क cn)
{
    प्रत्यागम fn((ध्रुव T*)src, आवरणम्, (ST*)योग_2, len, cn);
}

योग_Func get_योग_Func(पूर्णाङ्क गभीरता);

#यदि_अपरिभाषितम् CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

प्रतिरूपम् <प्रकारनाम T, प्रकारनाम ST>
संरचना योग_SIMD
{
    योग_SIMD(पूर्णाङ्क) {}
    पूर्णाङ्क संक्रियकः () (ध्रुव T*, ध्रुव uchar*, ST*, पूर्णाङ्क, पूर्णाङ्क) ध्रुव
    {
        प्रत्यागम 0;
    }
};

#यदि (CV_SIMD || CV_SIMD_SCALABLE)

#अपरिभाषय REDUCE_PARTIAL_SUMS
#परिभाषय REDUCE_PARTIAL_SUMS() \
    यदि (cn == 1) \
        dst[0] += v_reduce_योग(v_सङ्कलय(v_सङ्कलय(s0, s1), s2)); \
    अन्यथा यदि (cn == 2) { \
        s0 = v_सङ्कलय(v_सङ्कलय(s0, s1), s2); \
        dst[0] += v_reduce_योग(v_and(s0, m0)); \
        dst[1] += v_reduce_योग(v_and(s0, m1)); \
    } अन्यथा यदि (cn == 3) { \
        dst[0] += v_reduce_योग(v_सङ्कलय(v_सङ्कलय(v_and(s0, m0), v_and(s1, m1)), v_and(s2, m2))); \
        dst[1] += v_reduce_योग(v_सङ्कलय(v_सङ्कलय(v_and(s0, m3), v_and(s1, m4)), v_and(s2, m5))); \
        dst[2] += v_reduce_योग(v_सङ्कलय(v_सङ्कलय(v_and(s0, m6), v_and(s1, m7)), v_and(s2, m8))); \
    } अन्यथा यदि (cn == 4) { \
        s0 = v_सङ्कलय(v_सङ्कलय(s0, s1), s2); \
        dst[0] += v_reduce_योग(v_and(s0, m0)); \
        dst[1] += v_reduce_योग(v_and(s0, m1)); \
        dst[2] += v_reduce_योग(v_and(s0, m2)); \
        dst[3] += v_reduce_योग(v_and(s0, m3)); \
    }

प्रतिरूपम्<प्रकारनाम ST>
स्थिर शून्य init_maskbuf(ST* maskbuf, पूर्णाङ्क cn, पूर्णाङ्क simd_विस्तारः)
{
    स्मृतिं_पूरय(maskbuf, 0, simd_विस्तारः*9*परिमाण(maskbuf[0]));
    यदि (cn == 1)
        ;
    अन्यथा यदि (cn == 2)
        प्रति (पूर्णाङ्क i = 0; i < simd_विस्तारः; i += 2) {
            maskbuf[i] = (ST)-1;
            maskbuf[i+1+simd_विस्तारः] = (ST)-1;
        }
    अन्यथा यदि (cn == 3)
        प्रति (पूर्णाङ्क i = 0; i < simd_विस्तारः*3; i += 3) {
            maskbuf[i] = (ST)-1;
            maskbuf[i+1+simd_विस्तारः*3] = (ST)-1;
            maskbuf[i+2+simd_विस्तारः*6] = (ST)-1;
        }
    अन्यथा यदि (cn == 4 && simd_विस्तारः >= 4) {
        प्रति (पूर्णाङ्क i = 0; i < simd_विस्तारः; i += 4) {
            maskbuf[i] = (ST)-1;
            maskbuf[i+1+simd_विस्तारः] = (ST)-1;
            maskbuf[i+2+simd_विस्तारः*2] = (ST)-1;
            maskbuf[i+3+simd_विस्तारः*3] = (ST)-1;
        }
    }
}

#अपरिभाषय DEFINE_योग_SIMD_8
#परिभाषय DEFINE_योग_SIMD_8(T, ST, iST, VecT, आरोपय_op) \
प्रतिरूपम्<> संरचना योग_SIMD<T, ST> \
{ \
    योग_SIMD(पूर्णाङ्क cn) \
    { \
        init_maskbuf((iST*)maskbuf, cn, VTraits<VecT>::vlanes()); \
    } \
    पूर्णाङ्क संक्रियकः ()(ध्रुव T* src, ध्रुव uchar* आवरणम्, ST* dst, पूर्णाङ्क len, पूर्णाङ्क cn) ध्रुव \
    { \
        यदि (आवरणम् || (cn < 1 || cn > 4)) \
            प्रत्यागम 0; \
        len *= cn; \
        पूर्णाङ्क x = 0, simd_विस्तारः = VTraits<VecT>::vlanes(); \
        VecT m0 = vx_आरोपय(maskbuf), m1, m2, m3, m4, m5, m6, m7, m8; \
        यदि (cn == 1) { \
            m1 = m2 = m3 = m4 = m5 = m6 = m7 = m8 = m0; \
        } अन्यथा { \
            m1 = vx_आरोपय(maskbuf + simd_विस्तारः); \
            m2 = vx_आरोपय(maskbuf + simd_विस्तारः*2); \
            m3 = vx_आरोपय(maskbuf + simd_विस्तारः*3); \
            m4 = vx_आरोपय(maskbuf + simd_विस्तारः*4); \
            m5 = vx_आरोपय(maskbuf + simd_विस्तारः*5); \
            m6 = vx_आरोपय(maskbuf + simd_विस्तारः*6); \
            m7 = vx_आरोपय(maskbuf + simd_विस्तारः*7); \
            m8 = vx_आरोपय(maskbuf + simd_विस्तारः*8); \
        } \
        VecT s0 = v_xor(m0, m0), s1 = s0, s2 = s0; \
        प्रति (; x <= len - simd_विस्तारः*6; x += simd_विस्तारः*6) { \
            स्वचालित v0 = आरोपय_op(src + x); \
            स्वचालित v1 = आरोपय_op(src + x + simd_विस्तारः*2); \
            स्वचालित v2 = आरोपय_op(src + x + simd_विस्तारः*4); \
            s0 = v_सङ्कलय(s0, v_expand_नीचः(v0)); \
            s1 = v_सङ्कलय(s1, v_expand_उच्चः(v0)); \
            s2 = v_सङ्कलय(s2, v_expand_नीचः(v1)); \
            s0 = v_सङ्कलय(s0, v_expand_उच्चः(v1)); \
            s1 = v_सङ्कलय(s1, v_expand_नीचः(v2)); \
            s2 = v_सङ्कलय(s2, v_expand_उच्चः(v2)); \
        } \
        REDUCE_PARTIAL_SUMS(); \
        vx_cleanup(); \
        प्रत्यागम x / cn; \
    } \
    ST maskbuf[VTraits<VecT>::max_nlanes*9]; \
};

#अपरिभाषय DEFINE_योग_SIMD_16
#परिभाषय DEFINE_योग_SIMD_16(T, ST, iST, VecT, आरोपय_op) \
प्रतिरूपम्<> संरचना योग_SIMD<T, ST> \
{ \
    योग_SIMD(पूर्णाङ्क cn) \
    { \
        init_maskbuf((iST*)maskbuf, cn, VTraits<VecT>::vlanes()); \
    } \
    पूर्णाङ्क संक्रियकः ()(ध्रुव T* src, ध्रुव uchar* आवरणम्, ST* dst, पूर्णाङ्क len, पूर्णाङ्क cn) ध्रुव \
    { \
        यदि (आवरणम् || (cn < 1 || cn > 4)) \
            प्रत्यागम 0; \
        len *= cn; \
        पूर्णाङ्क x = 0, simd_विस्तारः = VTraits<VecT>::vlanes(); \
        VecT m0 = vx_आरोपय(maskbuf), m1, m2, m3, m4, m5, m6, m7, m8; \
        यदि (cn == 1) { \
            m1 = m2 = m3 = m4 = m5 = m6 = m7 = m8 = m0; \
        } अन्यथा { \
            m1 = vx_आरोपय(maskbuf + simd_विस्तारः); \
            m2 = vx_आरोपय(maskbuf + simd_विस्तारः*2); \
            m3 = vx_आरोपय(maskbuf + simd_विस्तारः*3); \
            m4 = vx_आरोपय(maskbuf + simd_विस्तारः*4); \
            m5 = vx_आरोपय(maskbuf + simd_विस्तारः*5); \
            m6 = vx_आरोपय(maskbuf + simd_विस्तारः*6); \
            m7 = vx_आरोपय(maskbuf + simd_विस्तारः*7); \
            m8 = vx_आरोपय(maskbuf + simd_विस्तारः*8); \
        } \
        VecT s0 = v_xor(m0, m0), s1 = s0, s2 = s0; \
        प्रति (; x <= len - simd_विस्तारः*3; x += simd_विस्तारः*3) { \
            स्वचालित v0 = आरोपय_op(src + x); \
            स्वचालित v1 = आरोपय_op(src + x + simd_विस्तारः); \
            स्वचालित v2 = आरोपय_op(src + x + simd_विस्तारः*2); \
            s0 = v_सङ्कलय(s0, v0); \
            s1 = v_सङ्कलय(s1, v1); \
            s2 = v_सङ्कलय(s2, v2); \
        } \
        REDUCE_PARTIAL_SUMS(); \
        vx_cleanup(); \
        प्रत्यागम x / cn; \
    } \
    ST maskbuf[VTraits<VecT>::max_nlanes*9]; \
};

#अपरिभाषय आरोपय_u8_as_s16
#अपरिभाषय आरोपय_u16_as_s32
#परिभाषय आरोपय_u8_as_s16(addr) v_reinterpret_as_s16(vx_आरोपय_expand(addr))
#परिभाषय आरोपय_u16_as_s32(addr) v_reinterpret_as_s32(vx_आरोपय_expand(addr))

DEFINE_योग_SIMD_8(uchar, पूर्णाङ्क, पूर्णाङ्क, v_int32, आरोपय_u8_as_s16)
DEFINE_योग_SIMD_8(schar, पूर्णाङ्क, पूर्णाङ्क, v_int32, vx_आरोपय_expand)
DEFINE_योग_SIMD_16(ushort, पूर्णाङ्क, पूर्णाङ्क, v_int32, आरोपय_u16_as_s32)
DEFINE_योग_SIMD_16(ह्रस्व, पूर्णाङ्क, पूर्णाङ्क, v_int32, vx_आरोपय_expand)
DEFINE_योग_SIMD_16(hfloat, भिन्न, पूर्णाङ्क, v_float32, vx_आरोपय_expand)
DEFINE_योग_SIMD_16(bfloat, भिन्न, पूर्णाङ्क, v_float32, vx_आरोपय_expand)

#यदि (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)

#अपरिभाषय DEFINE_योग_SIMD_32
#परिभाषय DEFINE_योग_SIMD_32(T, ST, iST, VecT) \
प्रतिरूपम्<> संरचना योग_SIMD<T, ST> \
{ \
    योग_SIMD(पूर्णाङ्क cn) \
    { \
        init_maskbuf((iST*)maskbuf, cn, VTraits<VecT>::vlanes()); \
    } \
    पूर्णाङ्क संक्रियकः ()(ध्रुव T* src, ध्रुव uchar* आवरणम्, ST* dst, पूर्णाङ्क len, पूर्णाङ्क cn) ध्रुव \
    { \
        पूर्णाङ्क x = 0, simd_विस्तारः = VTraits<VecT>::vlanes(); \
        यदि (आवरणम् || (cn < 1 || cn > 3+(simd_विस्तारः>=4))) \
            प्रत्यागम 0; \
        len *= cn; \
        VecT m0 = vx_आरोपय(maskbuf), m1, m2, m3, m4, m5, m6, m7, m8; \
        यदि (cn == 1) { \
            m1 = m2 = m3 = m4 = m5 = m6 = m7 = m8 = m0; \
        } अन्यथा { \
            m1 = vx_आरोपय(maskbuf + simd_विस्तारः); \
            m2 = vx_आरोपय(maskbuf + simd_विस्तारः*2); \
            m3 = vx_आरोपय(maskbuf + simd_विस्तारः*3); \
            m4 = vx_आरोपय(maskbuf + simd_विस्तारः*4); \
            m5 = vx_आरोपय(maskbuf + simd_विस्तारः*5); \
            m6 = vx_आरोपय(maskbuf + simd_विस्तारः*6); \
            m7 = vx_आरोपय(maskbuf + simd_विस्तारः*7); \
            m8 = vx_आरोपय(maskbuf + simd_विस्तारः*8); \
        } \
        VecT s0 = v_xor(m0, m0), s1 = s0, s2 = s0; \
        प्रति (; x <= len - simd_विस्तारः*6; x += simd_विस्तारः*6) { \
            स्वचालित v0 = vx_आरोपय(src + x); \
            स्वचालित v1 = vx_आरोपय(src + x + simd_विस्तारः*2); \
            स्वचालित v2 = vx_आरोपय(src + x + simd_विस्तारः*4); \
            s0 = v_सङ्कलय(s0, v_cvt_f64(v0)); \
            s1 = v_सङ्कलय(s1, v_cvt_f64_उच्चः(v0)); \
            s2 = v_सङ्कलय(s2, v_cvt_f64(v1)); \
            s0 = v_सङ्कलय(s0, v_cvt_f64_उच्चः(v1)); \
            s1 = v_सङ्कलय(s1, v_cvt_f64(v2)); \
            s2 = v_सङ्कलय(s2, v_cvt_f64_उच्चः(v2)); \
        } \
        REDUCE_PARTIAL_SUMS(); \
        vx_cleanup(); \
        प्रत्यागम x / cn; \
    } \
    ST maskbuf[VTraits<VecT>::max_nlanes*9]; \
};

DEFINE_योग_SIMD_32(पूर्णाङ्क, द्विगुण, int64, v_float64)
DEFINE_योग_SIMD_32(भिन्न, द्विगुण, int64, v_float64)
#यदि_अन्तः
#यदि_अन्तः

प्रतिरूपम्<प्रकारनाम T, प्रकारनाम ST, प्रकारनाम WT=T>
स्थिर पूर्णाङ्क योग_4(ध्रुव T* src0, ध्रुव uchar* आवरणम्, ST* dst, पूर्णाङ्क len, पूर्णाङ्क cn )
{
    ध्रुव T* src = src0;
    यदि( !आवरणम् )
    {
        योग_SIMD<T, ST> vop(cn);
        पूर्णाङ्क i0 = vop(src0, आवरणम्, dst, len, cn), i = i0, k = cn % 4;
        src += i0 * cn;

        यदि( k == 1 )
        {
            ST s0 = dst[0];

            #यदि CV_ENABLE_UNROLLED
            प्रति(; i <= len - 4; i += 4, src += cn*4 )
                s0 += (WT)src[0] + (WT)src[cn] + (WT)src[cn*2] + (WT)src[cn*3];
            #यदि_अन्तः
            प्रति( ; i < len; i++, src += cn )
                s0 += (WT)src[0];
            dst[0] = s0;
        }
        अन्यथा यदि( k == 2 )
        {
            ST s0 = dst[0], s1 = dst[1];
            प्रति( ; i < len; i++, src += cn )
            {
                s0 += (WT)src[0];
                s1 += (WT)src[1];
            }
            dst[0] = s0;
            dst[1] = s1;
        }
        अन्यथा यदि( k == 3 )
        {
            ST s0 = dst[0], s1 = dst[1], s2 = dst[2];
            प्रति( ; i < len; i++, src += cn )
            {
                s0 += (WT)src[0];
                s1 += (WT)src[1];
                s2 += (WT)src[2];
            }
            dst[0] = s0;
            dst[1] = s1;
            dst[2] = s2;
        }

        प्रति( ; k < cn; k += 4 )
        {
            src = src0 + i0*cn + k;
            ST s0 = dst[k], s1 = dst[k+1], s2 = dst[k+2], s3 = dst[k+3];
            प्रति( i = i0; i < len; i++, src += cn )
            {
                s0 += (WT)src[0]; s1 += (WT)src[1];
                s2 += (WT)src[2]; s3 += (WT)src[3];
            }
            dst[k] = s0;
            dst[k+1] = s1;
            dst[k+2] = s2;
            dst[k+3] = s3;
        }
        प्रत्यागम len;
    }

    पूर्णाङ्क i, nzm = 0;
    यदि( cn == 1 )
    {
        ST s = dst[0];
        प्रति( i = 0; i < len; i++ )
            यदि( आवरणम्[i] )
            {
                s += (WT)src[i];
                nzm++;
            }
        dst[0] = s;
    }
    अन्यथा यदि( cn == 3 )
    {
        ST s0 = dst[0], s1 = dst[1], s2 = dst[2];
        प्रति( i = 0; i < len; i++, src += 3 )
            यदि( आवरणम्[i] )
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
    अन्यथा
    {
        प्रति( i = 0; i < len; i++, src += cn )
            यदि( आवरणम्[i] )
            {
                पूर्णाङ्क k = 0;
                #यदि CV_ENABLE_UNROLLED
                प्रति( ; k <= cn - 4; k += 4 )
                {
                    ST s0, s1;
                    s0 = dst[k] + (WT)src[k];
                    s1 = dst[k+1] + (WT)src[k+1];
                    dst[k] = s0; dst[k+1] = s1;
                    s0 = dst[k+2] + (WT)src[k+2];
                    s1 = dst[k+3] + (WT)src[k+3];
                    dst[k+2] = s0; dst[k+3] = s1;
                }
                #यदि_अन्तः
                प्रति( ; k < cn; k++ )
                    dst[k] += (WT)src[k];
                nzm++;
            }
    }
    प्रत्यागम nzm;
}


स्थिर पूर्णाङ्क sum8u( ध्रुव uchar* src, ध्रुव uchar* आवरणम्, पूर्णाङ्क* dst, पूर्णाङ्क len, पूर्णाङ्क cn )
{ CV_INSTRUMENT_REGION(); प्रत्यागम योग_4(src, आवरणम्, dst, len, cn); }

स्थिर पूर्णाङ्क sum8s( ध्रुव schar* src, ध्रुव uchar* आवरणम्, पूर्णाङ्क* dst, पूर्णाङ्क len, पूर्णाङ्क cn )
{ CV_INSTRUMENT_REGION(); प्रत्यागम योग_4(src, आवरणम्, dst, len, cn); }

स्थिर पूर्णाङ्क sum16u( ध्रुव ushort* src, ध्रुव uchar* आवरणम्, पूर्णाङ्क* dst, पूर्णाङ्क len, पूर्णाङ्क cn )
{ CV_INSTRUMENT_REGION(); प्रत्यागम योग_4(src, आवरणम्, dst, len, cn); }

स्थिर पूर्णाङ्क sum16s( ध्रुव ह्रस्व* src, ध्रुव uchar* आवरणम्, पूर्णाङ्क* dst, पूर्णाङ्क len, पूर्णाङ्क cn )
{ CV_INSTRUMENT_REGION(); प्रत्यागम योग_4(src, आवरणम्, dst, len, cn); }

स्थिर पूर्णाङ्क sum32u( ध्रुव अचिह्नित* src, ध्रुव uchar* आवरणम्, द्विगुण* dst, पूर्णाङ्क len, पूर्णाङ्क cn )
{ CV_INSTRUMENT_REGION(); प्रत्यागम योग_4<अचिह्नित, द्विगुण, uint64>(src, आवरणम्, dst, len, cn); }

स्थिर पूर्णाङ्क sum32s( ध्रुव पूर्णाङ्क* src, ध्रुव uchar* आवरणम्, द्विगुण* dst, पूर्णाङ्क len, पूर्णाङ्क cn )
{ CV_INSTRUMENT_REGION(); प्रत्यागम योग_4<पूर्णाङ्क, द्विगुण, int64>(src, आवरणम्, dst, len, cn); }

स्थिर पूर्णाङ्क sum64u( ध्रुव uint64* src, ध्रुव uchar* आवरणम्, द्विगुण* dst, पूर्णाङ्क len, पूर्णाङ्क cn )
{ CV_INSTRUMENT_REGION(); प्रत्यागम योग_4(src, आवरणम्, dst, len, cn); }

स्थिर पूर्णाङ्क sum64s( ध्रुव int64* src, ध्रुव uchar* आवरणम्, द्विगुण* dst, पूर्णाङ्क len, पूर्णाङ्क cn )
{ CV_INSTRUMENT_REGION(); प्रत्यागम योग_4(src, आवरणम्, dst, len, cn); }

स्थिर पूर्णाङ्क sum32f( ध्रुव भिन्न* src, ध्रुव uchar* आवरणम्, द्विगुण* dst, पूर्णाङ्क len, पूर्णाङ्क cn )
{ CV_INSTRUMENT_REGION(); प्रत्यागम योग_4(src, आवरणम्, dst, len, cn); }

स्थिर पूर्णाङ्क sum64f( ध्रुव द्विगुण* src, ध्रुव uchar* आवरणम्, द्विगुण* dst, पूर्णाङ्क len, पूर्णाङ्क cn )
{ CV_INSTRUMENT_REGION(); प्रत्यागम योग_4(src, आवरणम्, dst, len, cn); }

स्थिर पूर्णाङ्क sum16f( ध्रुव hfloat* src, ध्रुव uchar* आवरणम्, भिन्न* dst, पूर्णाङ्क len, पूर्णाङ्क cn )
{ CV_INSTRUMENT_REGION(); प्रत्यागम योग_4<hfloat, भिन्न, भिन्न>(src, आवरणम्, dst, len, cn); }

स्थिर पूर्णाङ्क sum16bf( ध्रुव bfloat* src, ध्रुव uchar* आवरणम्, भिन्न* dst, पूर्णाङ्क len, पूर्णाङ्क cn )
{ CV_INSTRUMENT_REGION(); प्रत्यागम योग_4<bfloat, भिन्न, भिन्न>(src, आवरणम्, dst, len, cn); }

योग_Func get_योग_Func(पूर्णाङ्क गभीरता)
{
    स्थिर योग_Func योग_Tab[CV_गभीरता_MAX] =
    {
        योग_Wrap<uchar, पूर्णाङ्क, sum8u>,
        योग_Wrap<schar, पूर्णाङ्क, sum8s>,
        योग_Wrap<ushort, पूर्णाङ्क, sum16u>,
        योग_Wrap<ह्रस्व, पूर्णाङ्क, sum16s>,
        योग_Wrap<पूर्णाङ्क, द्विगुण, sum32s>,
        योग_Wrap<भिन्न, द्विगुण, sum32f>,
        योग_Wrap<द्विगुण, द्विगुण, sum64f>,
        योग_Wrap<hfloat, भिन्न, sum16f>,
        योग_Wrap<bfloat, भिन्न, sum16bf>,
        0,
        योग_Wrap<uint64, द्विगुण, sum64u>,
        योग_Wrap<int64, द्विगुण, sum64s>,
        योग_Wrap<अचिह्नित, द्विगुण, sum32u>,
        0
    };

    प्रत्यागम योग_Tab[गभीरता];
}

#यदि_अन्तः

CV_CPU_OPTIMIZATION_NAMESPACE_अन्तः
} // namespace
