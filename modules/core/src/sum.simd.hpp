// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


#અન્તર્ભાવ "precomp.hpp"
#અન્તર્ભાવ "stat.hpp"

નામાકાશ cv {
CV_CPU_OPTIMIZATION_NAMESPACE_BEGIN

template<typename T, typename ST, પૂર્ણાંક (*fn)(અચલ T*, અચલ uchar*, ST*, પૂર્ણાંક, પૂર્ણાંક)>
સ્થિર પૂર્ણાંક sumWrap(અચલ uchar* src, અચલ uchar* mask, uchar* sum, પૂર્ણાંક len, પૂર્ણાંક cn)
{
    પાછા fn((અચલ T*)src, mask, (ST*)sum, len, cn);
}

SumFunc getSumFunc(પૂર્ણાંક depth);

#ifndef CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

template <typename T, typename ST>
સંરચના Sum_SIMD
{
    Sum_SIMD(પૂર્ણાંક) {}
    પૂર્ણાંક operator () (અચલ T*, અચલ uchar*, ST*, પૂર્ણાંક, પૂર્ણાંક) અચલ
    {
        પાછા 0;
    }
};

#જો (CV_SIMD || CV_SIMD_SCALABLE)

#undef REDUCE_PARTIAL_SUMS
#define REDUCE_PARTIAL_SUMS() \
    જો (cn == 1) \
        dst[0] += v_reduce_sum(v_add(v_add(s0, s1), s2)); \
    નહીંતર જો (cn == 2) { \
        s0 = v_add(v_add(s0, s1), s2); \
        dst[0] += v_reduce_sum(v_and(s0, m0)); \
        dst[1] += v_reduce_sum(v_and(s0, m1)); \
    } નહીંતર જો (cn == 3) { \
        dst[0] += v_reduce_sum(v_add(v_add(v_and(s0, m0), v_and(s1, m1)), v_and(s2, m2))); \
        dst[1] += v_reduce_sum(v_add(v_add(v_and(s0, m3), v_and(s1, m4)), v_and(s2, m5))); \
        dst[2] += v_reduce_sum(v_add(v_add(v_and(s0, m6), v_and(s1, m7)), v_and(s2, m8))); \
    } નહીંતર જો (cn == 4) { \
        s0 = v_add(v_add(s0, s1), s2); \
        dst[0] += v_reduce_sum(v_and(s0, m0)); \
        dst[1] += v_reduce_sum(v_and(s0, m1)); \
        dst[2] += v_reduce_sum(v_and(s0, m2)); \
        dst[3] += v_reduce_sum(v_and(s0, m3)); \
    }

template<typename ST>
સ્થિર શૂન્ય init_maskbuf(ST* maskbuf, પૂર્ણાંક cn, પૂર્ણાંક simd_width)
{
    memset(maskbuf, 0, simd_width*9*કદ(maskbuf[0]));
    જો (cn == 1)
        ;
    નહીંતર જો (cn == 2)
        માટે (પૂર્ણાંક i = 0; i < simd_width; i += 2) {
            maskbuf[i] = (ST)-1;
            maskbuf[i+1+simd_width] = (ST)-1;
        }
    નહીંતર જો (cn == 3)
        માટે (પૂર્ણાંક i = 0; i < simd_width*3; i += 3) {
            maskbuf[i] = (ST)-1;
            maskbuf[i+1+simd_width*3] = (ST)-1;
            maskbuf[i+2+simd_width*6] = (ST)-1;
        }
    નહીંતર જો (cn == 4 && simd_width >= 4) {
        માટે (પૂર્ણાંક i = 0; i < simd_width; i += 4) {
            maskbuf[i] = (ST)-1;
            maskbuf[i+1+simd_width] = (ST)-1;
            maskbuf[i+2+simd_width*2] = (ST)-1;
            maskbuf[i+3+simd_width*3] = (ST)-1;
        }
    }
}

#undef DEFINE_SUM_SIMD_8
#define DEFINE_SUM_SIMD_8(T, ST, iST, VecT, load_op) \
template<> સંરચના Sum_SIMD<T, ST> \
{ \
    Sum_SIMD(પૂર્ણાંક cn) \
    { \
        init_maskbuf((iST*)maskbuf, cn, VTraits<VecT>::vlanes()); \
    } \
    પૂર્ણાંક operator ()(અચલ T* src, અચલ uchar* mask, ST* dst, પૂર્ણાંક len, પૂર્ણાંક cn) અચલ \
    { \
        જો (mask || (cn < 1 || cn > 4)) \
            પાછા 0; \
        len *= cn; \
        પૂર્ણાંક x = 0, simd_width = VTraits<VecT>::vlanes(); \
        VecT m0 = vx_load(maskbuf), m1, m2, m3, m4, m5, m6, m7, m8; \
        જો (cn == 1) { \
            m1 = m2 = m3 = m4 = m5 = m6 = m7 = m8 = m0; \
        } નહીંતર { \
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
        માટે (; x <= len - simd_width*6; x += simd_width*6) { \
            auto v0 = load_op(src + x); \
            auto v1 = load_op(src + x + simd_width*2); \
            auto v2 = load_op(src + x + simd_width*4); \
            s0 = v_add(s0, v_expand_નીચલું(v0)); \
            s1 = v_add(s1, v_expand_ઉપલું(v0)); \
            s2 = v_add(s2, v_expand_નીચલું(v1)); \
            s0 = v_add(s0, v_expand_ઉપલું(v1)); \
            s1 = v_add(s1, v_expand_નીચલું(v2)); \
            s2 = v_add(s2, v_expand_ઉપલું(v2)); \
        } \
        REDUCE_PARTIAL_SUMS(); \
        vx_cleanup(); \
        પાછા x / cn; \
    } \
    ST maskbuf[VTraits<VecT>::max_nlanes*9]; \
};

#undef DEFINE_SUM_SIMD_16
#define DEFINE_SUM_SIMD_16(T, ST, iST, VecT, load_op) \
template<> સંરચના Sum_SIMD<T, ST> \
{ \
    Sum_SIMD(પૂર્ણાંક cn) \
    { \
        init_maskbuf((iST*)maskbuf, cn, VTraits<VecT>::vlanes()); \
    } \
    પૂર્ણાંક operator ()(અચલ T* src, અચલ uchar* mask, ST* dst, પૂર્ણાંક len, પૂર્ણાંક cn) અચલ \
    { \
        જો (mask || (cn < 1 || cn > 4)) \
            પાછા 0; \
        len *= cn; \
        પૂર્ણાંક x = 0, simd_width = VTraits<VecT>::vlanes(); \
        VecT m0 = vx_load(maskbuf), m1, m2, m3, m4, m5, m6, m7, m8; \
        જો (cn == 1) { \
            m1 = m2 = m3 = m4 = m5 = m6 = m7 = m8 = m0; \
        } નહીંતર { \
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
        માટે (; x <= len - simd_width*3; x += simd_width*3) { \
            auto v0 = load_op(src + x); \
            auto v1 = load_op(src + x + simd_width); \
            auto v2 = load_op(src + x + simd_width*2); \
            s0 = v_add(s0, v0); \
            s1 = v_add(s1, v1); \
            s2 = v_add(s2, v2); \
        } \
        REDUCE_PARTIAL_SUMS(); \
        vx_cleanup(); \
        પાછા x / cn; \
    } \
    ST maskbuf[VTraits<VecT>::max_nlanes*9]; \
};

#undef load_u8_as_s16
#undef load_u16_as_s32
#define load_u8_as_s16(addr) v_reinterpret_as_s16(vx_load_expand(addr))
#define load_u16_as_s32(addr) v_reinterpret_as_s32(vx_load_expand(addr))

DEFINE_SUM_SIMD_8(uchar, પૂર્ણાંક, પૂર્ણાંક, v_int32, load_u8_as_s16)
DEFINE_SUM_SIMD_8(schar, પૂર્ણાંક, પૂર્ણાંક, v_int32, vx_load_expand)
DEFINE_SUM_SIMD_16(ushort, પૂર્ણાંક, પૂર્ણાંક, v_int32, load_u16_as_s32)
DEFINE_SUM_SIMD_16(short, પૂર્ણાંક, પૂર્ણાંક, v_int32, vx_load_expand)
DEFINE_SUM_SIMD_16(hfloat, દશાંશ, પૂર્ણાંક, v_float32, vx_load_expand)
DEFINE_SUM_SIMD_16(bfloat, દશાંશ, પૂર્ણાંક, v_float32, vx_load_expand)

#જો (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)

#undef DEFINE_SUM_SIMD_32
#define DEFINE_SUM_SIMD_32(T, ST, iST, VecT) \
template<> સંરચના Sum_SIMD<T, ST> \
{ \
    Sum_SIMD(પૂર્ણાંક cn) \
    { \
        init_maskbuf((iST*)maskbuf, cn, VTraits<VecT>::vlanes()); \
    } \
    પૂર્ણાંક operator ()(અચલ T* src, અચલ uchar* mask, ST* dst, પૂર્ણાંક len, પૂર્ણાંક cn) અચલ \
    { \
        પૂર્ણાંક x = 0, simd_width = VTraits<VecT>::vlanes(); \
        જો (mask || (cn < 1 || cn > 3+(simd_width>=4))) \
            પાછા 0; \
        len *= cn; \
        VecT m0 = vx_load(maskbuf), m1, m2, m3, m4, m5, m6, m7, m8; \
        જો (cn == 1) { \
            m1 = m2 = m3 = m4 = m5 = m6 = m7 = m8 = m0; \
        } નહીંતર { \
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
        માટે (; x <= len - simd_width*6; x += simd_width*6) { \
            auto v0 = vx_load(src + x); \
            auto v1 = vx_load(src + x + simd_width*2); \
            auto v2 = vx_load(src + x + simd_width*4); \
            s0 = v_add(s0, v_cvt_f64(v0)); \
            s1 = v_add(s1, v_cvt_f64_ઉપલું(v0)); \
            s2 = v_add(s2, v_cvt_f64(v1)); \
            s0 = v_add(s0, v_cvt_f64_ઉપલું(v1)); \
            s1 = v_add(s1, v_cvt_f64(v2)); \
            s2 = v_add(s2, v_cvt_f64_ઉપલું(v2)); \
        } \
        REDUCE_PARTIAL_SUMS(); \
        vx_cleanup(); \
        પાછા x / cn; \
    } \
    ST maskbuf[VTraits<VecT>::max_nlanes*9]; \
};

DEFINE_SUM_SIMD_32(પૂર્ણાંક, દ્વિગુણ, int64, v_float64)
DEFINE_SUM_SIMD_32(દશાંશ, દ્વિગુણ, int64, v_float64)
#endif
#endif

template<typename T, typename ST, typename WT=T>
સ્થિર પૂર્ણાંક sum_(અચલ T* src0, અચલ uchar* mask, ST* dst, પૂર્ણાંક len, પૂર્ણાંક cn )
{
    અચલ T* src = src0;
    જો( !mask )
    {
        Sum_SIMD<T, ST> vop(cn);
        પૂર્ણાંક i0 = vop(src0, mask, dst, len, cn), i = i0, k = cn % 4;
        src += i0 * cn;

        જો( k == 1 )
        {
            ST s0 = dst[0];

            #જો CV_ENABLE_UNROLLED
            માટે(; i <= len - 4; i += 4, src += cn*4 )
                s0 += (WT)src[0] + (WT)src[cn] + (WT)src[cn*2] + (WT)src[cn*3];
            #endif
            માટે( ; i < len; i++, src += cn )
                s0 += (WT)src[0];
            dst[0] = s0;
        }
        નહીંતર જો( k == 2 )
        {
            ST s0 = dst[0], s1 = dst[1];
            માટે( ; i < len; i++, src += cn )
            {
                s0 += (WT)src[0];
                s1 += (WT)src[1];
            }
            dst[0] = s0;
            dst[1] = s1;
        }
        નહીંતર જો( k == 3 )
        {
            ST s0 = dst[0], s1 = dst[1], s2 = dst[2];
            માટે( ; i < len; i++, src += cn )
            {
                s0 += (WT)src[0];
                s1 += (WT)src[1];
                s2 += (WT)src[2];
            }
            dst[0] = s0;
            dst[1] = s1;
            dst[2] = s2;
        }

        માટે( ; k < cn; k += 4 )
        {
            src = src0 + i0*cn + k;
            ST s0 = dst[k], s1 = dst[k+1], s2 = dst[k+2], s3 = dst[k+3];
            માટે( i = i0; i < len; i++, src += cn )
            {
                s0 += (WT)src[0]; s1 += (WT)src[1];
                s2 += (WT)src[2]; s3 += (WT)src[3];
            }
            dst[k] = s0;
            dst[k+1] = s1;
            dst[k+2] = s2;
            dst[k+3] = s3;
        }
        પાછા len;
    }

    પૂર્ણાંક i, nzm = 0;
    જો( cn == 1 )
    {
        ST s = dst[0];
        માટે( i = 0; i < len; i++ )
            જો( mask[i] )
            {
                s += (WT)src[i];
                nzm++;
            }
        dst[0] = s;
    }
    નહીંતર જો( cn == 3 )
    {
        ST s0 = dst[0], s1 = dst[1], s2 = dst[2];
        માટે( i = 0; i < len; i++, src += 3 )
            જો( mask[i] )
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
    નહીંતર
    {
        માટે( i = 0; i < len; i++, src += cn )
            જો( mask[i] )
            {
                પૂર્ણાંક k = 0;
                #જો CV_ENABLE_UNROLLED
                માટે( ; k <= cn - 4; k += 4 )
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
                માટે( ; k < cn; k++ )
                    dst[k] += (WT)src[k];
                nzm++;
            }
    }
    પાછા nzm;
}


સ્થિર પૂર્ણાંક sum8u( અચલ uchar* src, અચલ uchar* mask, પૂર્ણાંક* dst, પૂર્ણાંક len, પૂર્ણાંક cn )
{ CV_INSTRUMENT_REGION(); પાછા sum_(src, mask, dst, len, cn); }

સ્થિર પૂર્ણાંક sum8s( અચલ schar* src, અચલ uchar* mask, પૂર્ણાંક* dst, પૂર્ણાંક len, પૂર્ણાંક cn )
{ CV_INSTRUMENT_REGION(); પાછા sum_(src, mask, dst, len, cn); }

સ્થિર પૂર્ણાંક sum16u( અચલ ushort* src, અચલ uchar* mask, પૂર્ણાંક* dst, પૂર્ણાંક len, પૂર્ણાંક cn )
{ CV_INSTRUMENT_REGION(); પાછા sum_(src, mask, dst, len, cn); }

સ્થિર પૂર્ણાંક sum16s( અચલ short* src, અચલ uchar* mask, પૂર્ણાંક* dst, પૂર્ણાંક len, પૂર્ણાંક cn )
{ CV_INSTRUMENT_REGION(); પાછા sum_(src, mask, dst, len, cn); }

સ્થિર પૂર્ણાંક sum32u( અચલ unsigned* src, અચલ uchar* mask, દ્વિગુણ* dst, પૂર્ણાંક len, પૂર્ણાંક cn )
{ CV_INSTRUMENT_REGION(); પાછા sum_<unsigned, દ્વિગુણ, uint64>(src, mask, dst, len, cn); }

સ્થિર પૂર્ણાંક sum32s( અચલ પૂર્ણાંક* src, અચલ uchar* mask, દ્વિગુણ* dst, પૂર્ણાંક len, પૂર્ણાંક cn )
{ CV_INSTRUMENT_REGION(); પાછા sum_<પૂર્ણાંક, દ્વિગુણ, int64>(src, mask, dst, len, cn); }

સ્થિર પૂર્ણાંક sum64u( અચલ uint64* src, અચલ uchar* mask, દ્વિગુણ* dst, પૂર્ણાંક len, પૂર્ણાંક cn )
{ CV_INSTRUMENT_REGION(); પાછા sum_(src, mask, dst, len, cn); }

સ્થિર પૂર્ણાંક sum64s( અચલ int64* src, અચલ uchar* mask, દ્વિગુણ* dst, પૂર્ણાંક len, પૂર્ણાંક cn )
{ CV_INSTRUMENT_REGION(); પાછા sum_(src, mask, dst, len, cn); }

સ્થિર પૂર્ણાંક sum32f( અચલ દશાંશ* src, અચલ uchar* mask, દ્વિગુણ* dst, પૂર્ણાંક len, પૂર્ણાંક cn )
{ CV_INSTRUMENT_REGION(); પાછા sum_(src, mask, dst, len, cn); }

સ્થિર પૂર્ણાંક sum64f( અચલ દ્વિગુણ* src, અચલ uchar* mask, દ્વિગુણ* dst, પૂર્ણાંક len, પૂર્ણાંક cn )
{ CV_INSTRUMENT_REGION(); પાછા sum_(src, mask, dst, len, cn); }

સ્થિર પૂર્ણાંક sum16f( અચલ hfloat* src, અચલ uchar* mask, દશાંશ* dst, પૂર્ણાંક len, પૂર્ણાંક cn )
{ CV_INSTRUMENT_REGION(); પાછા sum_<hfloat, દશાંશ, દશાંશ>(src, mask, dst, len, cn); }

સ્થિર પૂર્ણાંક sum16bf( અચલ bfloat* src, અચલ uchar* mask, દશાંશ* dst, પૂર્ણાંક len, પૂર્ણાંક cn )
{ CV_INSTRUMENT_REGION(); પાછા sum_<bfloat, દશાંશ, દશાંશ>(src, mask, dst, len, cn); }

SumFunc getSumFunc(પૂર્ણાંક depth)
{
    સ્થિર SumFunc sumTab[CV_DEPTH_MAX] =
    {
        sumWrap<uchar, પૂર્ણાંક, sum8u>,
        sumWrap<schar, પૂર્ણાંક, sum8s>,
        sumWrap<ushort, પૂર્ણાંક, sum16u>,
        sumWrap<short, પૂર્ણાંક, sum16s>,
        sumWrap<પૂર્ણાંક, દ્વિગુણ, sum32s>,
        sumWrap<દશાંશ, દ્વિગુણ, sum32f>,
        sumWrap<દ્વિગુણ, દ્વિગુણ, sum64f>,
        sumWrap<hfloat, દશાંશ, sum16f>,
        sumWrap<bfloat, દશાંશ, sum16bf>,
        0,
        sumWrap<uint64, દ્વિગુણ, sum64u>,
        sumWrap<int64, દ્વિગુણ, sum64s>,
        sumWrap<unsigned, દ્વિગુણ, sum32u>,
        0
    };

    પાછા sumTab[depth];
}

#endif

CV_CPU_OPTIMIZATION_NAMESPACE_END
} // namespace
