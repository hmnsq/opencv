// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


#समावेश "precomp.hpp"
#समावेश "stat.hpp"

namespace cv {
CV_CPU_OPTIMIZATION_NAMESPACE_BEGIN

ढाँचा<typename T, typename ST, पूर्णांक (*fn)(स्थिर T*, स्थिर uchar*, ST*, पूर्णांक, पूर्णांक)>
अटल पूर्णांक योग_Wrap(स्थिर uchar* src, स्थिर uchar* mask, uchar* योग_2, पूर्णांक len, पूर्णांक cn)
{
    वापस fn((स्थिर T*)src, mask, (ST*)योग_2, len, cn);
}

योग_Func get_योग_Func(पूर्णांक depth);

#अगर_अघोषित CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

ढाँचा <typename T, typename ST>
काष्ठा योग_SIMD
{
    योग_SIMD(पूर्णांक) {}
    पूर्णांक चालक () (स्थिर T*, स्थिर uchar*, ST*, पूर्णांक, पूर्णांक) स्थिर
    {
        वापस 0;
    }
};

#अगर (CV_SIMD || CV_SIMD_SCALABLE)

#अघोषित REDUCE_PARTIAL_SUMS
#घोषणा REDUCE_PARTIAL_SUMS() \
    अगर (cn == 1) \
        dst[0] += v_reduce_योग(v_add(v_add(s0, s1), s2)); \
    अन्यथा अगर (cn == 2) { \
        s0 = v_add(v_add(s0, s1), s2); \
        dst[0] += v_reduce_योग(v_and(s0, m0)); \
        dst[1] += v_reduce_योग(v_and(s0, m1)); \
    } अन्यथा अगर (cn == 3) { \
        dst[0] += v_reduce_योग(v_add(v_add(v_and(s0, m0), v_and(s1, m1)), v_and(s2, m2))); \
        dst[1] += v_reduce_योग(v_add(v_add(v_and(s0, m3), v_and(s1, m4)), v_and(s2, m5))); \
        dst[2] += v_reduce_योग(v_add(v_add(v_and(s0, m6), v_and(s1, m7)), v_and(s2, m8))); \
    } अन्यथा अगर (cn == 4) { \
        s0 = v_add(v_add(s0, s1), s2); \
        dst[0] += v_reduce_योग(v_and(s0, m0)); \
        dst[1] += v_reduce_योग(v_and(s0, m1)); \
        dst[2] += v_reduce_योग(v_and(s0, m2)); \
        dst[3] += v_reduce_योग(v_and(s0, m3)); \
    }

ढाँचा<typename ST>
अटल व्योम init_maskbuf(ST* maskbuf, पूर्णांक cn, पूर्णांक simd_width)
{
    स_रखो(maskbuf, 0, simd_width*9*माप(maskbuf[0]));
    अगर (cn == 1)
        ;
    अन्यथा अगर (cn == 2)
        क्रम (पूर्णांक i = 0; i < simd_width; i += 2) {
            maskbuf[i] = (ST)-1;
            maskbuf[i+1+simd_width] = (ST)-1;
        }
    अन्यथा अगर (cn == 3)
        क्रम (पूर्णांक i = 0; i < simd_width*3; i += 3) {
            maskbuf[i] = (ST)-1;
            maskbuf[i+1+simd_width*3] = (ST)-1;
            maskbuf[i+2+simd_width*6] = (ST)-1;
        }
    अन्यथा अगर (cn == 4 && simd_width >= 4) {
        क्रम (पूर्णांक i = 0; i < simd_width; i += 4) {
            maskbuf[i] = (ST)-1;
            maskbuf[i+1+simd_width] = (ST)-1;
            maskbuf[i+2+simd_width*2] = (ST)-1;
            maskbuf[i+3+simd_width*3] = (ST)-1;
        }
    }
}

#अघोषित DEFINE_योग_SIMD_8
#घोषणा DEFINE_योग_SIMD_8(T, ST, iST, VecT, load_op) \
ढाँचा<> काष्ठा योग_SIMD<T, ST> \
{ \
    योग_SIMD(पूर्णांक cn) \
    { \
        init_maskbuf((iST*)maskbuf, cn, VTraits<VecT>::vlanes()); \
    } \
    पूर्णांक चालक ()(स्थिर T* src, स्थिर uchar* mask, ST* dst, पूर्णांक len, पूर्णांक cn) स्थिर \
    { \
        अगर (mask || (cn < 1 || cn > 4)) \
            वापस 0; \
        len *= cn; \
        पूर्णांक x = 0, simd_width = VTraits<VecT>::vlanes(); \
        VecT m0 = vx_load(maskbuf), m1, m2, m3, m4, m5, m6, m7, m8; \
        अगर (cn == 1) { \
            m1 = m2 = m3 = m4 = m5 = m6 = m7 = m8 = m0; \
        } अन्यथा { \
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
        क्रम (; x <= len - simd_width*6; x += simd_width*6) { \
            स्वतः v0 = load_op(src + x); \
            स्वतः v1 = load_op(src + x + simd_width*2); \
            स्वतः v2 = load_op(src + x + simd_width*4); \
            s0 = v_add(s0, v_expand_निचला(v0)); \
            s1 = v_add(s1, v_expand_ऊपरी(v0)); \
            s2 = v_add(s2, v_expand_निचला(v1)); \
            s0 = v_add(s0, v_expand_ऊपरी(v1)); \
            s1 = v_add(s1, v_expand_निचला(v2)); \
            s2 = v_add(s2, v_expand_ऊपरी(v2)); \
        } \
        REDUCE_PARTIAL_SUMS(); \
        vx_cleanup(); \
        वापस x / cn; \
    } \
    ST maskbuf[VTraits<VecT>::max_nlanes*9]; \
};

#अघोषित DEFINE_योग_SIMD_16
#घोषणा DEFINE_योग_SIMD_16(T, ST, iST, VecT, load_op) \
ढाँचा<> काष्ठा योग_SIMD<T, ST> \
{ \
    योग_SIMD(पूर्णांक cn) \
    { \
        init_maskbuf((iST*)maskbuf, cn, VTraits<VecT>::vlanes()); \
    } \
    पूर्णांक चालक ()(स्थिर T* src, स्थिर uchar* mask, ST* dst, पूर्णांक len, पूर्णांक cn) स्थिर \
    { \
        अगर (mask || (cn < 1 || cn > 4)) \
            वापस 0; \
        len *= cn; \
        पूर्णांक x = 0, simd_width = VTraits<VecT>::vlanes(); \
        VecT m0 = vx_load(maskbuf), m1, m2, m3, m4, m5, m6, m7, m8; \
        अगर (cn == 1) { \
            m1 = m2 = m3 = m4 = m5 = m6 = m7 = m8 = m0; \
        } अन्यथा { \
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
        क्रम (; x <= len - simd_width*3; x += simd_width*3) { \
            स्वतः v0 = load_op(src + x); \
            स्वतः v1 = load_op(src + x + simd_width); \
            स्वतः v2 = load_op(src + x + simd_width*2); \
            s0 = v_add(s0, v0); \
            s1 = v_add(s1, v1); \
            s2 = v_add(s2, v2); \
        } \
        REDUCE_PARTIAL_SUMS(); \
        vx_cleanup(); \
        वापस x / cn; \
    } \
    ST maskbuf[VTraits<VecT>::max_nlanes*9]; \
};

#अघोषित load_u8_as_s16
#अघोषित load_u16_as_s32
#घोषणा load_u8_as_s16(addr) v_reinterpret_as_s16(vx_load_expand(addr))
#घोषणा load_u16_as_s32(addr) v_reinterpret_as_s32(vx_load_expand(addr))

DEFINE_योग_SIMD_8(uchar, पूर्णांक, पूर्णांक, v_int32, load_u8_as_s16)
DEFINE_योग_SIMD_8(schar, पूर्णांक, पूर्णांक, v_int32, vx_load_expand)
DEFINE_योग_SIMD_16(ushort, पूर्णांक, पूर्णांक, v_int32, load_u16_as_s32)
DEFINE_योग_SIMD_16(लघु, पूर्णांक, पूर्णांक, v_int32, vx_load_expand)
DEFINE_योग_SIMD_16(hfloat, भग्न, पूर्णांक, v_float32, vx_load_expand)
DEFINE_योग_SIMD_16(bfloat, भग्न, पूर्णांक, v_float32, vx_load_expand)

#अगर (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)

#अघोषित DEFINE_योग_SIMD_32
#घोषणा DEFINE_योग_SIMD_32(T, ST, iST, VecT) \
ढाँचा<> काष्ठा योग_SIMD<T, ST> \
{ \
    योग_SIMD(पूर्णांक cn) \
    { \
        init_maskbuf((iST*)maskbuf, cn, VTraits<VecT>::vlanes()); \
    } \
    पूर्णांक चालक ()(स्थिर T* src, स्थिर uchar* mask, ST* dst, पूर्णांक len, पूर्णांक cn) स्थिर \
    { \
        पूर्णांक x = 0, simd_width = VTraits<VecT>::vlanes(); \
        अगर (mask || (cn < 1 || cn > 3+(simd_width>=4))) \
            वापस 0; \
        len *= cn; \
        VecT m0 = vx_load(maskbuf), m1, m2, m3, m4, m5, m6, m7, m8; \
        अगर (cn == 1) { \
            m1 = m2 = m3 = m4 = m5 = m6 = m7 = m8 = m0; \
        } अन्यथा { \
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
        क्रम (; x <= len - simd_width*6; x += simd_width*6) { \
            स्वतः v0 = vx_load(src + x); \
            स्वतः v1 = vx_load(src + x + simd_width*2); \
            स्वतः v2 = vx_load(src + x + simd_width*4); \
            s0 = v_add(s0, v_cvt_f64(v0)); \
            s1 = v_add(s1, v_cvt_f64_ऊपरी(v0)); \
            s2 = v_add(s2, v_cvt_f64(v1)); \
            s0 = v_add(s0, v_cvt_f64_ऊपरी(v1)); \
            s1 = v_add(s1, v_cvt_f64(v2)); \
            s2 = v_add(s2, v_cvt_f64_ऊपरी(v2)); \
        } \
        REDUCE_PARTIAL_SUMS(); \
        vx_cleanup(); \
        वापस x / cn; \
    } \
    ST maskbuf[VTraits<VecT>::max_nlanes*9]; \
};

DEFINE_योग_SIMD_32(पूर्णांक, द्विगुन, int64, v_float64)
DEFINE_योग_SIMD_32(भग्न, द्विगुन, int64, v_float64)
#पूर्ण_अगर
#पूर्ण_अगर

ढाँचा<typename T, typename ST, typename WT=T>
अटल पूर्णांक योग_4(स्थिर T* src0, स्थिर uchar* mask, ST* dst, पूर्णांक len, पूर्णांक cn )
{
    स्थिर T* src = src0;
    अगर( !mask )
    {
        योग_SIMD<T, ST> vop(cn);
        पूर्णांक i0 = vop(src0, mask, dst, len, cn), i = i0, k = cn % 4;
        src += i0 * cn;

        अगर( k == 1 )
        {
            ST s0 = dst[0];

            #अगर CV_ENABLE_UNROLLED
            क्रम(; i <= len - 4; i += 4, src += cn*4 )
                s0 += (WT)src[0] + (WT)src[cn] + (WT)src[cn*2] + (WT)src[cn*3];
            #पूर्ण_अगर
            क्रम( ; i < len; i++, src += cn )
                s0 += (WT)src[0];
            dst[0] = s0;
        }
        अन्यथा अगर( k == 2 )
        {
            ST s0 = dst[0], s1 = dst[1];
            क्रम( ; i < len; i++, src += cn )
            {
                s0 += (WT)src[0];
                s1 += (WT)src[1];
            }
            dst[0] = s0;
            dst[1] = s1;
        }
        अन्यथा अगर( k == 3 )
        {
            ST s0 = dst[0], s1 = dst[1], s2 = dst[2];
            क्रम( ; i < len; i++, src += cn )
            {
                s0 += (WT)src[0];
                s1 += (WT)src[1];
                s2 += (WT)src[2];
            }
            dst[0] = s0;
            dst[1] = s1;
            dst[2] = s2;
        }

        क्रम( ; k < cn; k += 4 )
        {
            src = src0 + i0*cn + k;
            ST s0 = dst[k], s1 = dst[k+1], s2 = dst[k+2], s3 = dst[k+3];
            क्रम( i = i0; i < len; i++, src += cn )
            {
                s0 += (WT)src[0]; s1 += (WT)src[1];
                s2 += (WT)src[2]; s3 += (WT)src[3];
            }
            dst[k] = s0;
            dst[k+1] = s1;
            dst[k+2] = s2;
            dst[k+3] = s3;
        }
        वापस len;
    }

    पूर्णांक i, nzm = 0;
    अगर( cn == 1 )
    {
        ST s = dst[0];
        क्रम( i = 0; i < len; i++ )
            अगर( mask[i] )
            {
                s += (WT)src[i];
                nzm++;
            }
        dst[0] = s;
    }
    अन्यथा अगर( cn == 3 )
    {
        ST s0 = dst[0], s1 = dst[1], s2 = dst[2];
        क्रम( i = 0; i < len; i++, src += 3 )
            अगर( mask[i] )
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
        क्रम( i = 0; i < len; i++, src += cn )
            अगर( mask[i] )
            {
                पूर्णांक k = 0;
                #अगर CV_ENABLE_UNROLLED
                क्रम( ; k <= cn - 4; k += 4 )
                {
                    ST s0, s1;
                    s0 = dst[k] + (WT)src[k];
                    s1 = dst[k+1] + (WT)src[k+1];
                    dst[k] = s0; dst[k+1] = s1;
                    s0 = dst[k+2] + (WT)src[k+2];
                    s1 = dst[k+3] + (WT)src[k+3];
                    dst[k+2] = s0; dst[k+3] = s1;
                }
                #पूर्ण_अगर
                क्रम( ; k < cn; k++ )
                    dst[k] += (WT)src[k];
                nzm++;
            }
    }
    वापस nzm;
}


अटल पूर्णांक sum8u( स्थिर uchar* src, स्थिर uchar* mask, पूर्णांक* dst, पूर्णांक len, पूर्णांक cn )
{ CV_INSTRUMENT_REGION(); वापस योग_4(src, mask, dst, len, cn); }

अटल पूर्णांक sum8s( स्थिर schar* src, स्थिर uchar* mask, पूर्णांक* dst, पूर्णांक len, पूर्णांक cn )
{ CV_INSTRUMENT_REGION(); वापस योग_4(src, mask, dst, len, cn); }

अटल पूर्णांक sum16u( स्थिर ushort* src, स्थिर uchar* mask, पूर्णांक* dst, पूर्णांक len, पूर्णांक cn )
{ CV_INSTRUMENT_REGION(); वापस योग_4(src, mask, dst, len, cn); }

अटल पूर्णांक sum16s( स्थिर लघु* src, स्थिर uchar* mask, पूर्णांक* dst, पूर्णांक len, पूर्णांक cn )
{ CV_INSTRUMENT_REGION(); वापस योग_4(src, mask, dst, len, cn); }

अटल पूर्णांक sum32u( स्थिर अचिन्हित* src, स्थिर uchar* mask, द्विगुन* dst, पूर्णांक len, पूर्णांक cn )
{ CV_INSTRUMENT_REGION(); वापस योग_4<अचिन्हित, द्विगुन, uint64>(src, mask, dst, len, cn); }

अटल पूर्णांक sum32s( स्थिर पूर्णांक* src, स्थिर uchar* mask, द्विगुन* dst, पूर्णांक len, पूर्णांक cn )
{ CV_INSTRUMENT_REGION(); वापस योग_4<पूर्णांक, द्विगुन, int64>(src, mask, dst, len, cn); }

अटल पूर्णांक sum64u( स्थिर uint64* src, स्थिर uchar* mask, द्विगुन* dst, पूर्णांक len, पूर्णांक cn )
{ CV_INSTRUMENT_REGION(); वापस योग_4(src, mask, dst, len, cn); }

अटल पूर्णांक sum64s( स्थिर int64* src, स्थिर uchar* mask, द्विगुन* dst, पूर्णांक len, पूर्णांक cn )
{ CV_INSTRUMENT_REGION(); वापस योग_4(src, mask, dst, len, cn); }

अटल पूर्णांक sum32f( स्थिर भग्न* src, स्थिर uchar* mask, द्विगुन* dst, पूर्णांक len, पूर्णांक cn )
{ CV_INSTRUMENT_REGION(); वापस योग_4(src, mask, dst, len, cn); }

अटल पूर्णांक sum64f( स्थिर द्विगुन* src, स्थिर uchar* mask, द्विगुन* dst, पूर्णांक len, पूर्णांक cn )
{ CV_INSTRUMENT_REGION(); वापस योग_4(src, mask, dst, len, cn); }

अटल पूर्णांक sum16f( स्थिर hfloat* src, स्थिर uchar* mask, भग्न* dst, पूर्णांक len, पूर्णांक cn )
{ CV_INSTRUMENT_REGION(); वापस योग_4<hfloat, भग्न, भग्न>(src, mask, dst, len, cn); }

अटल पूर्णांक sum16bf( स्थिर bfloat* src, स्थिर uchar* mask, भग्न* dst, पूर्णांक len, पूर्णांक cn )
{ CV_INSTRUMENT_REGION(); वापस योग_4<bfloat, भग्न, भग्न>(src, mask, dst, len, cn); }

योग_Func get_योग_Func(पूर्णांक depth)
{
    अटल योग_Func योग_Tab[CV_DEPTH_MAX] =
    {
        योग_Wrap<uchar, पूर्णांक, sum8u>,
        योग_Wrap<schar, पूर्णांक, sum8s>,
        योग_Wrap<ushort, पूर्णांक, sum16u>,
        योग_Wrap<लघु, पूर्णांक, sum16s>,
        योग_Wrap<पूर्णांक, द्विगुन, sum32s>,
        योग_Wrap<भग्न, द्विगुन, sum32f>,
        योग_Wrap<द्विगुन, द्विगुन, sum64f>,
        योग_Wrap<hfloat, भग्न, sum16f>,
        योग_Wrap<bfloat, भग्न, sum16bf>,
        0,
        योग_Wrap<uint64, द्विगुन, sum64u>,
        योग_Wrap<int64, द्विगुन, sum64s>,
        योग_Wrap<अचिन्हित, द्विगुन, sum32u>,
        0
    };

    वापस योग_Tab[depth];
}

#पूर्ण_अगर

CV_CPU_OPTIMIZATION_NAMESPACE_END
} // namespace
