// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


#അന്തര്ഭാവ "precomp.hpp"
#അന്തര്ഭാവ "stat.hpp"

നാമാകാശ cv {
CV_CPU_OPTIMIZATION_NAMESPACE_BEGIN

template<typename T, typename ST, പൂർണ്ണസംഖ്യ (*fn)(സ്ഥിരം T*, സ്ഥിരം uchar*, ST*, പൂർണ്ണസംഖ്യ, പൂർണ്ണസംഖ്യ)>
സ്ഥിതം പൂർണ്ണസംഖ്യ sumWrap(സ്ഥിരം uchar* src, സ്ഥിരം uchar* mask, uchar* sum, പൂർണ്ണസംഖ്യ len, പൂർണ്ണസംഖ്യ cn)
{
    തിരികെ fn((സ്ഥിരം T*)src, mask, (ST*)sum, len, cn);
}

SumFunc getSumFunc(പൂർണ്ണസംഖ്യ depth);

#ifndef CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

template <typename T, typename ST>
ഘടന Sum_SIMD
{
    Sum_SIMD(പൂർണ്ണസംഖ്യ) {}
    പൂർണ്ണസംഖ്യ operator () (സ്ഥിരം T*, സ്ഥിരം uchar*, ST*, പൂർണ്ണസംഖ്യ, പൂർണ്ണസംഖ്യ) സ്ഥിരം
    {
        തിരികെ 0;
    }
};

#എങ്കിൽ (CV_SIMD || CV_SIMD_SCALABLE)

#undef REDUCE_PARTIAL_SUMS
#define REDUCE_PARTIAL_SUMS() \
    എങ്കിൽ (cn == 1) \
        dst[0] += v_reduce_sum(v_add(v_add(s0, s1), s2)); \
    അല്ലെങ്കിൽ എങ്കിൽ (cn == 2) { \
        s0 = v_add(v_add(s0, s1), s2); \
        dst[0] += v_reduce_sum(v_and(s0, m0)); \
        dst[1] += v_reduce_sum(v_and(s0, m1)); \
    } അല്ലെങ്കിൽ എങ്കിൽ (cn == 3) { \
        dst[0] += v_reduce_sum(v_add(v_add(v_and(s0, m0), v_and(s1, m1)), v_and(s2, m2))); \
        dst[1] += v_reduce_sum(v_add(v_add(v_and(s0, m3), v_and(s1, m4)), v_and(s2, m5))); \
        dst[2] += v_reduce_sum(v_add(v_add(v_and(s0, m6), v_and(s1, m7)), v_and(s2, m8))); \
    } അല്ലെങ്കിൽ എങ്കിൽ (cn == 4) { \
        s0 = v_add(v_add(s0, s1), s2); \
        dst[0] += v_reduce_sum(v_and(s0, m0)); \
        dst[1] += v_reduce_sum(v_and(s0, m1)); \
        dst[2] += v_reduce_sum(v_and(s0, m2)); \
        dst[3] += v_reduce_sum(v_and(s0, m3)); \
    }

template<typename ST>
സ്ഥിതം ശൂന്യം init_maskbuf(ST* maskbuf, പൂർണ്ണസംഖ്യ cn, പൂർണ്ണസംഖ്യ simd_width)
{
    memset(maskbuf, 0, simd_width*9*വലുപ്പം(maskbuf[0]));
    എങ്കിൽ (cn == 1)
        ;
    അല്ലെങ്കിൽ എങ്കിൽ (cn == 2)
        വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < simd_width; i += 2) {
            maskbuf[i] = (ST)-1;
            maskbuf[i+1+simd_width] = (ST)-1;
        }
    അല്ലെങ്കിൽ എങ്കിൽ (cn == 3)
        വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < simd_width*3; i += 3) {
            maskbuf[i] = (ST)-1;
            maskbuf[i+1+simd_width*3] = (ST)-1;
            maskbuf[i+2+simd_width*6] = (ST)-1;
        }
    അല്ലെങ്കിൽ എങ്കിൽ (cn == 4 && simd_width >= 4) {
        വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < simd_width; i += 4) {
            maskbuf[i] = (ST)-1;
            maskbuf[i+1+simd_width] = (ST)-1;
            maskbuf[i+2+simd_width*2] = (ST)-1;
            maskbuf[i+3+simd_width*3] = (ST)-1;
        }
    }
}

#undef DEFINE_SUM_SIMD_8
#define DEFINE_SUM_SIMD_8(T, ST, iST, VecT, load_op) \
template<> ഘടന Sum_SIMD<T, ST> \
{ \
    Sum_SIMD(പൂർണ്ണസംഖ്യ cn) \
    { \
        init_maskbuf((iST*)maskbuf, cn, VTraits<VecT>::vlanes()); \
    } \
    പൂർണ്ണസംഖ്യ operator ()(സ്ഥിരം T* src, സ്ഥിരം uchar* mask, ST* dst, പൂർണ്ണസംഖ്യ len, പൂർണ്ണസംഖ്യ cn) സ്ഥിരം \
    { \
        എങ്കിൽ (mask || (cn < 1 || cn > 4)) \
            തിരികെ 0; \
        len *= cn; \
        പൂർണ്ണസംഖ്യ x = 0, simd_width = VTraits<VecT>::vlanes(); \
        VecT m0 = vx_load(maskbuf), m1, m2, m3, m4, m5, m6, m7, m8; \
        എങ്കിൽ (cn == 1) { \
            m1 = m2 = m3 = m4 = m5 = m6 = m7 = m8 = m0; \
        } അല്ലെങ്കിൽ { \
            m1 = vx_load(maskbuf + simd_width); \
            m2 = vx_load(maskbuf + simd_width*2); \
            m3 = vx_load(maskbuf + simd_width*3); \
            m4 = vx_load(maskbuf + simd_width*4); \
            m5 = vx_load(maskbuf + simd_width*5); \
            m6 = vx_load(maskbuf + simd_width*6); \
            m7 = vx_load(maskbuf + simd_width*7); \
            m8 = vx_load(maskbuf + simd_width*8); \
        } \
        VecT s0 = v_xor(m0, m0), s1 = s0, s2 = s0; \
        വേണ്ടി (; x <= len - simd_width*6; x += simd_width*6) { \
            auto v0 = load_op(src + x); \
            auto v1 = load_op(src + x + simd_width*2); \
            auto v2 = load_op(src + x + simd_width*4); \
            s0 = v_add(s0, v_expand_താഴെയുള്ള(v0)); \
            s1 = v_add(s1, v_expand_മുകളിലെ(v0)); \
            s2 = v_add(s2, v_expand_താഴെയുള്ള(v1)); \
            s0 = v_add(s0, v_expand_മുകളിലെ(v1)); \
            s1 = v_add(s1, v_expand_താഴെയുള്ള(v2)); \
            s2 = v_add(s2, v_expand_മുകളിലെ(v2)); \
        } \
        REDUCE_PARTIAL_SUMS(); \
        vx_cleanup(); \
        തിരികെ x / cn; \
    } \
    ST maskbuf[VTraits<VecT>::max_nlanes*9]; \
};

#undef DEFINE_SUM_SIMD_16
#define DEFINE_SUM_SIMD_16(T, ST, iST, VecT, load_op) \
template<> ഘടന Sum_SIMD<T, ST> \
{ \
    Sum_SIMD(പൂർണ്ണസംഖ്യ cn) \
    { \
        init_maskbuf((iST*)maskbuf, cn, VTraits<VecT>::vlanes()); \
    } \
    പൂർണ്ണസംഖ്യ operator ()(സ്ഥിരം T* src, സ്ഥിരം uchar* mask, ST* dst, പൂർണ്ണസംഖ്യ len, പൂർണ്ണസംഖ്യ cn) സ്ഥിരം \
    { \
        എങ്കിൽ (mask || (cn < 1 || cn > 4)) \
            തിരികെ 0; \
        len *= cn; \
        പൂർണ്ണസംഖ്യ x = 0, simd_width = VTraits<VecT>::vlanes(); \
        VecT m0 = vx_load(maskbuf), m1, m2, m3, m4, m5, m6, m7, m8; \
        എങ്കിൽ (cn == 1) { \
            m1 = m2 = m3 = m4 = m5 = m6 = m7 = m8 = m0; \
        } അല്ലെങ്കിൽ { \
            m1 = vx_load(maskbuf + simd_width); \
            m2 = vx_load(maskbuf + simd_width*2); \
            m3 = vx_load(maskbuf + simd_width*3); \
            m4 = vx_load(maskbuf + simd_width*4); \
            m5 = vx_load(maskbuf + simd_width*5); \
            m6 = vx_load(maskbuf + simd_width*6); \
            m7 = vx_load(maskbuf + simd_width*7); \
            m8 = vx_load(maskbuf + simd_width*8); \
        } \
        VecT s0 = v_xor(m0, m0), s1 = s0, s2 = s0; \
        വേണ്ടി (; x <= len - simd_width*3; x += simd_width*3) { \
            auto v0 = load_op(src + x); \
            auto v1 = load_op(src + x + simd_width); \
            auto v2 = load_op(src + x + simd_width*2); \
            s0 = v_add(s0, v0); \
            s1 = v_add(s1, v1); \
            s2 = v_add(s2, v2); \
        } \
        REDUCE_PARTIAL_SUMS(); \
        vx_cleanup(); \
        തിരികെ x / cn; \
    } \
    ST maskbuf[VTraits<VecT>::max_nlanes*9]; \
};

#undef load_u8_as_s16
#undef load_u16_as_s32
#define load_u8_as_s16(addr) v_reinterpret_as_s16(vx_load_expand(addr))
#define load_u16_as_s32(addr) v_reinterpret_as_s32(vx_load_expand(addr))

DEFINE_SUM_SIMD_8(uchar, പൂർണ്ണസംഖ്യ, പൂർണ്ണസംഖ്യ, v_int32, load_u8_as_s16)
DEFINE_SUM_SIMD_8(schar, പൂർണ്ണസംഖ്യ, പൂർണ്ണസംഖ്യ, v_int32, vx_load_expand)
DEFINE_SUM_SIMD_16(ushort, പൂർണ്ണസംഖ്യ, പൂർണ്ണസംഖ്യ, v_int32, load_u16_as_s32)
DEFINE_SUM_SIMD_16(short, പൂർണ്ണസംഖ്യ, പൂർണ്ണസംഖ്യ, v_int32, vx_load_expand)
DEFINE_SUM_SIMD_16(hfloat, ദശാംശം, പൂർണ്ണസംഖ്യ, v_float32, vx_load_expand)
DEFINE_SUM_SIMD_16(bfloat, ദശാംശം, പൂർണ്ണസംഖ്യ, v_float32, vx_load_expand)

#എങ്കിൽ (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)

#undef DEFINE_SUM_SIMD_32
#define DEFINE_SUM_SIMD_32(T, ST, iST, VecT) \
template<> ഘടന Sum_SIMD<T, ST> \
{ \
    Sum_SIMD(പൂർണ്ണസംഖ്യ cn) \
    { \
        init_maskbuf((iST*)maskbuf, cn, VTraits<VecT>::vlanes()); \
    } \
    പൂർണ്ണസംഖ്യ operator ()(സ്ഥിരം T* src, സ്ഥിരം uchar* mask, ST* dst, പൂർണ്ണസംഖ്യ len, പൂർണ്ണസംഖ്യ cn) സ്ഥിരം \
    { \
        പൂർണ്ണസംഖ്യ x = 0, simd_width = VTraits<VecT>::vlanes(); \
        എങ്കിൽ (mask || (cn < 1 || cn > 3+(simd_width>=4))) \
            തിരികെ 0; \
        len *= cn; \
        VecT m0 = vx_load(maskbuf), m1, m2, m3, m4, m5, m6, m7, m8; \
        എങ്കിൽ (cn == 1) { \
            m1 = m2 = m3 = m4 = m5 = m6 = m7 = m8 = m0; \
        } അല്ലെങ്കിൽ { \
            m1 = vx_load(maskbuf + simd_width); \
            m2 = vx_load(maskbuf + simd_width*2); \
            m3 = vx_load(maskbuf + simd_width*3); \
            m4 = vx_load(maskbuf + simd_width*4); \
            m5 = vx_load(maskbuf + simd_width*5); \
            m6 = vx_load(maskbuf + simd_width*6); \
            m7 = vx_load(maskbuf + simd_width*7); \
            m8 = vx_load(maskbuf + simd_width*8); \
        } \
        VecT s0 = v_xor(m0, m0), s1 = s0, s2 = s0; \
        വേണ്ടി (; x <= len - simd_width*6; x += simd_width*6) { \
            auto v0 = vx_load(src + x); \
            auto v1 = vx_load(src + x + simd_width*2); \
            auto v2 = vx_load(src + x + simd_width*4); \
            s0 = v_add(s0, v_cvt_f64(v0)); \
            s1 = v_add(s1, v_cvt_f64_മുകളിലെ(v0)); \
            s2 = v_add(s2, v_cvt_f64(v1)); \
            s0 = v_add(s0, v_cvt_f64_മുകളിലെ(v1)); \
            s1 = v_add(s1, v_cvt_f64(v2)); \
            s2 = v_add(s2, v_cvt_f64_മുകളിലെ(v2)); \
        } \
        REDUCE_PARTIAL_SUMS(); \
        vx_cleanup(); \
        തിരികെ x / cn; \
    } \
    ST maskbuf[VTraits<VecT>::max_nlanes*9]; \
};

DEFINE_SUM_SIMD_32(പൂർണ്ണസംഖ്യ, ഇരട്ട, int64, v_float64)
DEFINE_SUM_SIMD_32(ദശാംശം, ഇരട്ട, int64, v_float64)
#endif
#endif

template<typename T, typename ST, typename WT=T>
സ്ഥിതം പൂർണ്ണസംഖ്യ sum_(സ്ഥിരം T* src0, സ്ഥിരം uchar* mask, ST* dst, പൂർണ്ണസംഖ്യ len, പൂർണ്ണസംഖ്യ cn )
{
    സ്ഥിരം T* src = src0;
    എങ്കിൽ( !mask )
    {
        Sum_SIMD<T, ST> vop(cn);
        പൂർണ്ണസംഖ്യ i0 = vop(src0, mask, dst, len, cn), i = i0, k = cn % 4;
        src += i0 * cn;

        എങ്കിൽ( k == 1 )
        {
            ST s0 = dst[0];

            #എങ്കിൽ CV_ENABLE_UNROLLED
            വേണ്ടി(; i <= len - 4; i += 4, src += cn*4 )
                s0 += (WT)src[0] + (WT)src[cn] + (WT)src[cn*2] + (WT)src[cn*3];
            #endif
            വേണ്ടി( ; i < len; i++, src += cn )
                s0 += (WT)src[0];
            dst[0] = s0;
        }
        അല്ലെങ്കിൽ എങ്കിൽ( k == 2 )
        {
            ST s0 = dst[0], s1 = dst[1];
            വേണ്ടി( ; i < len; i++, src += cn )
            {
                s0 += (WT)src[0];
                s1 += (WT)src[1];
            }
            dst[0] = s0;
            dst[1] = s1;
        }
        അല്ലെങ്കിൽ എങ്കിൽ( k == 3 )
        {
            ST s0 = dst[0], s1 = dst[1], s2 = dst[2];
            വേണ്ടി( ; i < len; i++, src += cn )
            {
                s0 += (WT)src[0];
                s1 += (WT)src[1];
                s2 += (WT)src[2];
            }
            dst[0] = s0;
            dst[1] = s1;
            dst[2] = s2;
        }

        വേണ്ടി( ; k < cn; k += 4 )
        {
            src = src0 + i0*cn + k;
            ST s0 = dst[k], s1 = dst[k+1], s2 = dst[k+2], s3 = dst[k+3];
            വേണ്ടി( i = i0; i < len; i++, src += cn )
            {
                s0 += (WT)src[0]; s1 += (WT)src[1];
                s2 += (WT)src[2]; s3 += (WT)src[3];
            }
            dst[k] = s0;
            dst[k+1] = s1;
            dst[k+2] = s2;
            dst[k+3] = s3;
        }
        തിരികെ len;
    }

    പൂർണ്ണസംഖ്യ i, nzm = 0;
    എങ്കിൽ( cn == 1 )
    {
        ST s = dst[0];
        വേണ്ടി( i = 0; i < len; i++ )
            എങ്കിൽ( mask[i] )
            {
                s += (WT)src[i];
                nzm++;
            }
        dst[0] = s;
    }
    അല്ലെങ്കിൽ എങ്കിൽ( cn == 3 )
    {
        ST s0 = dst[0], s1 = dst[1], s2 = dst[2];
        വേണ്ടി( i = 0; i < len; i++, src += 3 )
            എങ്കിൽ( mask[i] )
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
    അല്ലെങ്കിൽ
    {
        വേണ്ടി( i = 0; i < len; i++, src += cn )
            എങ്കിൽ( mask[i] )
            {
                പൂർണ്ണസംഖ്യ k = 0;
                #എങ്കിൽ CV_ENABLE_UNROLLED
                വേണ്ടി( ; k <= cn - 4; k += 4 )
                {
                    ST s0, s1;
                    s0 = dst[k] + (WT)src[k];
                    s1 = dst[k+1] + (WT)src[k+1];
                    dst[k] = s0; dst[k+1] = s1;
                    s0 = dst[k+2] + (WT)src[k+2];
                    s1 = dst[k+3] + (WT)src[k+3];
                    dst[k+2] = s0; dst[k+3] = s1;
                }
                #endif
                വേണ്ടി( ; k < cn; k++ )
                    dst[k] += (WT)src[k];
                nzm++;
            }
    }
    തിരികെ nzm;
}


സ്ഥിതം പൂർണ്ണസംഖ്യ sum8u( സ്ഥിരം uchar* src, സ്ഥിരം uchar* mask, പൂർണ്ണസംഖ്യ* dst, പൂർണ്ണസംഖ്യ len, പൂർണ്ണസംഖ്യ cn )
{ CV_INSTRUMENT_REGION(); തിരികെ sum_(src, mask, dst, len, cn); }

സ്ഥിതം പൂർണ്ണസംഖ്യ sum8s( സ്ഥിരം schar* src, സ്ഥിരം uchar* mask, പൂർണ്ണസംഖ്യ* dst, പൂർണ്ണസംഖ്യ len, പൂർണ്ണസംഖ്യ cn )
{ CV_INSTRUMENT_REGION(); തിരികെ sum_(src, mask, dst, len, cn); }

സ്ഥിതം പൂർണ്ണസംഖ്യ sum16u( സ്ഥിരം ushort* src, സ്ഥിരം uchar* mask, പൂർണ്ണസംഖ്യ* dst, പൂർണ്ണസംഖ്യ len, പൂർണ്ണസംഖ്യ cn )
{ CV_INSTRUMENT_REGION(); തിരികെ sum_(src, mask, dst, len, cn); }

സ്ഥിതം പൂർണ്ണസംഖ്യ sum16s( സ്ഥിരം short* src, സ്ഥിരം uchar* mask, പൂർണ്ണസംഖ്യ* dst, പൂർണ്ണസംഖ്യ len, പൂർണ്ണസംഖ്യ cn )
{ CV_INSTRUMENT_REGION(); തിരികെ sum_(src, mask, dst, len, cn); }

സ്ഥിതം പൂർണ്ണസംഖ്യ sum32u( സ്ഥിരം unsigned* src, സ്ഥിരം uchar* mask, ഇരട്ട* dst, പൂർണ്ണസംഖ്യ len, പൂർണ്ണസംഖ്യ cn )
{ CV_INSTRUMENT_REGION(); തിരികെ sum_<unsigned, ഇരട്ട, uint64>(src, mask, dst, len, cn); }

സ്ഥിതം പൂർണ്ണസംഖ്യ sum32s( സ്ഥിരം പൂർണ്ണസംഖ്യ* src, സ്ഥിരം uchar* mask, ഇരട്ട* dst, പൂർണ്ണസംഖ്യ len, പൂർണ്ണസംഖ്യ cn )
{ CV_INSTRUMENT_REGION(); തിരികെ sum_<പൂർണ്ണസംഖ്യ, ഇരട്ട, int64>(src, mask, dst, len, cn); }

സ്ഥിതം പൂർണ്ണസംഖ്യ sum64u( സ്ഥിരം uint64* src, സ്ഥിരം uchar* mask, ഇരട്ട* dst, പൂർണ്ണസംഖ്യ len, പൂർണ്ണസംഖ്യ cn )
{ CV_INSTRUMENT_REGION(); തിരികെ sum_(src, mask, dst, len, cn); }

സ്ഥിതം പൂർണ്ണസംഖ്യ sum64s( സ്ഥിരം int64* src, സ്ഥിരം uchar* mask, ഇരട്ട* dst, പൂർണ്ണസംഖ്യ len, പൂർണ്ണസംഖ്യ cn )
{ CV_INSTRUMENT_REGION(); തിരികെ sum_(src, mask, dst, len, cn); }

സ്ഥിതം പൂർണ്ണസംഖ്യ sum32f( സ്ഥിരം ദശാംശം* src, സ്ഥിരം uchar* mask, ഇരട്ട* dst, പൂർണ്ണസംഖ്യ len, പൂർണ്ണസംഖ്യ cn )
{ CV_INSTRUMENT_REGION(); തിരികെ sum_(src, mask, dst, len, cn); }

സ്ഥിതം പൂർണ്ണസംഖ്യ sum64f( സ്ഥിരം ഇരട്ട* src, സ്ഥിരം uchar* mask, ഇരട്ട* dst, പൂർണ്ണസംഖ്യ len, പൂർണ്ണസംഖ്യ cn )
{ CV_INSTRUMENT_REGION(); തിരികെ sum_(src, mask, dst, len, cn); }

സ്ഥിതം പൂർണ്ണസംഖ്യ sum16f( സ്ഥിരം hfloat* src, സ്ഥിരം uchar* mask, ദശാംശം* dst, പൂർണ്ണസംഖ്യ len, പൂർണ്ണസംഖ്യ cn )
{ CV_INSTRUMENT_REGION(); തിരികെ sum_<hfloat, ദശാംശം, ദശാംശം>(src, mask, dst, len, cn); }

സ്ഥിതം പൂർണ്ണസംഖ്യ sum16bf( സ്ഥിരം bfloat* src, സ്ഥിരം uchar* mask, ദശാംശം* dst, പൂർണ്ണസംഖ്യ len, പൂർണ്ണസംഖ്യ cn )
{ CV_INSTRUMENT_REGION(); തിരികെ sum_<bfloat, ദശാംശം, ദശാംശം>(src, mask, dst, len, cn); }

SumFunc getSumFunc(പൂർണ്ണസംഖ്യ depth)
{
    സ്ഥിതം SumFunc sumTab[CV_DEPTH_MAX] =
    {
        sumWrap<uchar, പൂർണ്ണസംഖ്യ, sum8u>,
        sumWrap<schar, പൂർണ്ണസംഖ്യ, sum8s>,
        sumWrap<ushort, പൂർണ്ണസംഖ്യ, sum16u>,
        sumWrap<short, പൂർണ്ണസംഖ്യ, sum16s>,
        sumWrap<പൂർണ്ണസംഖ്യ, ഇരട്ട, sum32s>,
        sumWrap<ദശാംശം, ഇരട്ട, sum32f>,
        sumWrap<ഇരട്ട, ഇരട്ട, sum64f>,
        sumWrap<hfloat, ദശാംശം, sum16f>,
        sumWrap<bfloat, ദശാംശം, sum16bf>,
        0,
        sumWrap<uint64, ഇരട്ട, sum64u>,
        sumWrap<int64, ഇരട്ട, sum64s>,
        sumWrap<unsigned, ഇരട്ട, sum32u>,
        0
    };

    തിരികെ sumTab[depth];
}

#endif

CV_CPU_OPTIMIZATION_NAMESPACE_END
} // namespace
