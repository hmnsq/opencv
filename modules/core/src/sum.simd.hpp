// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


#अन्तर्भाव "precomp.hpp"
#अन्तर्भाव "stat.hpp"

नामाकाश cv {
CV_CPU_OPTIMIZATION_NAMESPACE_BEGIN

ढाँचा<typename T, typename ST, पूर्णांक (*fn)(स्थिरांक T*, स्थिरांक uchar*, ST*, पूर्णांक, पूर्णांक)>
स्थिर पूर्णांक sumWrap(स्थिरांक uchar* src, स्थिरांक uchar* mask, uchar* sum, पूर्णांक len, पूर्णांक cn)
{
    परत fn((स्थिरांक T*)src, mask, (ST*)sum, len, cn);
}

SumFunc getSumFunc(पूर्णांक depth);

#ifndef CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

ढाँचा <typename T, typename ST>
संरचना Sum_SIMD
{
    Sum_SIMD(पूर्णांक) {}
    पूर्णांक चालक () (स्थिरांक T*, स्थिरांक uchar*, ST*, पूर्णांक, पूर्णांक) स्थिरांक
    {
        परत 0;
    }
};

#जर (CV_SIMD || CV_SIMD_SCALABLE)

#undef REDUCE_PARTIAL_SUMS
#define REDUCE_PARTIAL_SUMS() \
    जर (cn == 1) \
        dst[0] += v_reduce_sum(v_add(v_add(s0, s1), s2)); \
    अन्यथा जर (cn == 2) { \
        s0 = v_add(v_add(s0, s1), s2); \
        dst[0] += v_reduce_sum(v_and(s0, m0)); \
        dst[1] += v_reduce_sum(v_and(s0, m1)); \
    } अन्यथा जर (cn == 3) { \
        dst[0] += v_reduce_sum(v_add(v_add(v_and(s0, m0), v_and(s1, m1)), v_and(s2, m2))); \
        dst[1] += v_reduce_sum(v_add(v_add(v_and(s0, m3), v_and(s1, m4)), v_and(s2, m5))); \
        dst[2] += v_reduce_sum(v_add(v_add(v_and(s0, m6), v_and(s1, m7)), v_and(s2, m8))); \
    } अन्यथा जर (cn == 4) { \
        s0 = v_add(v_add(s0, s1), s2); \
        dst[0] += v_reduce_sum(v_and(s0, m0)); \
        dst[1] += v_reduce_sum(v_and(s0, m1)); \
        dst[2] += v_reduce_sum(v_and(s0, m2)); \
        dst[3] += v_reduce_sum(v_and(s0, m3)); \
    }

ढाँचा<typename ST>
स्थिर शून्य init_maskbuf(ST* maskbuf, पूर्णांक cn, पूर्णांक simd_width)
{
    स_रखो(maskbuf, 0, simd_width*9*आकार(maskbuf[0]));
    जर (cn == 1)
        ;
    अन्यथा जर (cn == 2)
        साठी (पूर्णांक i = 0; i < simd_width; i += 2) {
            maskbuf[i] = (ST)-1;
            maskbuf[i+1+simd_width] = (ST)-1;
        }
    अन्यथा जर (cn == 3)
        साठी (पूर्णांक i = 0; i < simd_width*3; i += 3) {
            maskbuf[i] = (ST)-1;
            maskbuf[i+1+simd_width*3] = (ST)-1;
            maskbuf[i+2+simd_width*6] = (ST)-1;
        }
    अन्यथा जर (cn == 4 && simd_width >= 4) {
        साठी (पूर्णांक i = 0; i < simd_width; i += 4) {
            maskbuf[i] = (ST)-1;
            maskbuf[i+1+simd_width] = (ST)-1;
            maskbuf[i+2+simd_width*2] = (ST)-1;
            maskbuf[i+3+simd_width*3] = (ST)-1;
        }
    }
}

#undef DEFINE_SUM_SIMD_8
#define DEFINE_SUM_SIMD_8(T, ST, iST, VecT, load_op) \
ढाँचा<> संरचना Sum_SIMD<T, ST> \
{ \
    Sum_SIMD(पूर्णांक cn) \
    { \
        init_maskbuf((iST*)maskbuf, cn, VTraits<VecT>::vlanes()); \
    } \
    पूर्णांक चालक ()(स्थिरांक T* src, स्थिरांक uchar* mask, ST* dst, पूर्णांक len, पूर्णांक cn) स्थिरांक \
    { \
        जर (mask || (cn < 1 || cn > 4)) \
            परत 0; \
        len *= cn; \
        पूर्णांक x = 0, simd_width = VTraits<VecT>::vlanes(); \
        VecT m0 = vx_load(maskbuf), m1, m2, m3, m4, m5, m6, m7, m8; \
        जर (cn == 1) { \
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
        साठी (; x <= len - simd_width*6; x += simd_width*6) { \
            स्वतः v0 = load_op(src + x); \
            स्वतः v1 = load_op(src + x + simd_width*2); \
            स्वतः v2 = load_op(src + x + simd_width*4); \
            s0 = v_add(s0, v_expand_खालचा(v0)); \
            s1 = v_add(s1, v_expand_वरचा(v0)); \
            s2 = v_add(s2, v_expand_खालचा(v1)); \
            s0 = v_add(s0, v_expand_वरचा(v1)); \
            s1 = v_add(s1, v_expand_खालचा(v2)); \
            s2 = v_add(s2, v_expand_वरचा(v2)); \
        } \
        REDUCE_PARTIAL_SUMS(); \
        vx_cleanup(); \
        परत x / cn; \
    } \
    ST maskbuf[VTraits<VecT>::max_nlanes*9]; \
};

#undef DEFINE_SUM_SIMD_16
#define DEFINE_SUM_SIMD_16(T, ST, iST, VecT, load_op) \
ढाँचा<> संरचना Sum_SIMD<T, ST> \
{ \
    Sum_SIMD(पूर्णांक cn) \
    { \
        init_maskbuf((iST*)maskbuf, cn, VTraits<VecT>::vlanes()); \
    } \
    पूर्णांक चालक ()(स्थिरांक T* src, स्थिरांक uchar* mask, ST* dst, पूर्णांक len, पूर्णांक cn) स्थिरांक \
    { \
        जर (mask || (cn < 1 || cn > 4)) \
            परत 0; \
        len *= cn; \
        पूर्णांक x = 0, simd_width = VTraits<VecT>::vlanes(); \
        VecT m0 = vx_load(maskbuf), m1, m2, m3, m4, m5, m6, m7, m8; \
        जर (cn == 1) { \
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
        साठी (; x <= len - simd_width*3; x += simd_width*3) { \
            स्वतः v0 = load_op(src + x); \
            स्वतः v1 = load_op(src + x + simd_width); \
            स्वतः v2 = load_op(src + x + simd_width*2); \
            s0 = v_add(s0, v0); \
            s1 = v_add(s1, v1); \
            s2 = v_add(s2, v2); \
        } \
        REDUCE_PARTIAL_SUMS(); \
        vx_cleanup(); \
        परत x / cn; \
    } \
    ST maskbuf[VTraits<VecT>::max_nlanes*9]; \
};

#undef load_u8_as_s16
#undef load_u16_as_s32
#define load_u8_as_s16(addr) v_reinterpret_as_s16(vx_load_expand(addr))
#define load_u16_as_s32(addr) v_reinterpret_as_s32(vx_load_expand(addr))

DEFINE_SUM_SIMD_8(uchar, पूर्णांक, पूर्णांक, v_int32, load_u8_as_s16)
DEFINE_SUM_SIMD_8(schar, पूर्णांक, पूर्णांक, v_int32, vx_load_expand)
DEFINE_SUM_SIMD_16(ushort, पूर्णांक, पूर्णांक, v_int32, load_u16_as_s32)
DEFINE_SUM_SIMD_16(लघु, पूर्णांक, पूर्णांक, v_int32, vx_load_expand)
DEFINE_SUM_SIMD_16(hfloat, दशांश, पूर्णांक, v_float32, vx_load_expand)
DEFINE_SUM_SIMD_16(bfloat, दशांश, पूर्णांक, v_float32, vx_load_expand)

#जर (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)

#undef DEFINE_SUM_SIMD_32
#define DEFINE_SUM_SIMD_32(T, ST, iST, VecT) \
ढाँचा<> संरचना Sum_SIMD<T, ST> \
{ \
    Sum_SIMD(पूर्णांक cn) \
    { \
        init_maskbuf((iST*)maskbuf, cn, VTraits<VecT>::vlanes()); \
    } \
    पूर्णांक चालक ()(स्थिरांक T* src, स्थिरांक uchar* mask, ST* dst, पूर्णांक len, पूर्णांक cn) स्थिरांक \
    { \
        पूर्णांक x = 0, simd_width = VTraits<VecT>::vlanes(); \
        जर (mask || (cn < 1 || cn > 3+(simd_width>=4))) \
            परत 0; \
        len *= cn; \
        VecT m0 = vx_load(maskbuf), m1, m2, m3, m4, m5, m6, m7, m8; \
        जर (cn == 1) { \
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
        साठी (; x <= len - simd_width*6; x += simd_width*6) { \
            स्वतः v0 = vx_load(src + x); \
            स्वतः v1 = vx_load(src + x + simd_width*2); \
            स्वतः v2 = vx_load(src + x + simd_width*4); \
            s0 = v_add(s0, v_cvt_f64(v0)); \
            s1 = v_add(s1, v_cvt_f64_वरचा(v0)); \
            s2 = v_add(s2, v_cvt_f64(v1)); \
            s0 = v_add(s0, v_cvt_f64_वरचा(v1)); \
            s1 = v_add(s1, v_cvt_f64(v2)); \
            s2 = v_add(s2, v_cvt_f64_वरचा(v2)); \
        } \
        REDUCE_PARTIAL_SUMS(); \
        vx_cleanup(); \
        परत x / cn; \
    } \
    ST maskbuf[VTraits<VecT>::max_nlanes*9]; \
};

DEFINE_SUM_SIMD_32(पूर्णांक, द्विगुण, int64, v_float64)
DEFINE_SUM_SIMD_32(दशांश, द्विगुण, int64, v_float64)
#endif
#endif

ढाँचा<typename T, typename ST, typename WT=T>
स्थिर पूर्णांक sum_(स्थिरांक T* src0, स्थिरांक uchar* mask, ST* dst, पूर्णांक len, पूर्णांक cn )
{
    स्थिरांक T* src = src0;
    जर( !mask )
    {
        Sum_SIMD<T, ST> vop(cn);
        पूर्णांक i0 = vop(src0, mask, dst, len, cn), i = i0, k = cn % 4;
        src += i0 * cn;

        जर( k == 1 )
        {
            ST s0 = dst[0];

            #जर CV_ENABLE_UNROLLED
            साठी(; i <= len - 4; i += 4, src += cn*4 )
                s0 += (WT)src[0] + (WT)src[cn] + (WT)src[cn*2] + (WT)src[cn*3];
            #endif
            साठी( ; i < len; i++, src += cn )
                s0 += (WT)src[0];
            dst[0] = s0;
        }
        अन्यथा जर( k == 2 )
        {
            ST s0 = dst[0], s1 = dst[1];
            साठी( ; i < len; i++, src += cn )
            {
                s0 += (WT)src[0];
                s1 += (WT)src[1];
            }
            dst[0] = s0;
            dst[1] = s1;
        }
        अन्यथा जर( k == 3 )
        {
            ST s0 = dst[0], s1 = dst[1], s2 = dst[2];
            साठी( ; i < len; i++, src += cn )
            {
                s0 += (WT)src[0];
                s1 += (WT)src[1];
                s2 += (WT)src[2];
            }
            dst[0] = s0;
            dst[1] = s1;
            dst[2] = s2;
        }

        साठी( ; k < cn; k += 4 )
        {
            src = src0 + i0*cn + k;
            ST s0 = dst[k], s1 = dst[k+1], s2 = dst[k+2], s3 = dst[k+3];
            साठी( i = i0; i < len; i++, src += cn )
            {
                s0 += (WT)src[0]; s1 += (WT)src[1];
                s2 += (WT)src[2]; s3 += (WT)src[3];
            }
            dst[k] = s0;
            dst[k+1] = s1;
            dst[k+2] = s2;
            dst[k+3] = s3;
        }
        परत len;
    }

    पूर्णांक i, nzm = 0;
    जर( cn == 1 )
    {
        ST s = dst[0];
        साठी( i = 0; i < len; i++ )
            जर( mask[i] )
            {
                s += (WT)src[i];
                nzm++;
            }
        dst[0] = s;
    }
    अन्यथा जर( cn == 3 )
    {
        ST s0 = dst[0], s1 = dst[1], s2 = dst[2];
        साठी( i = 0; i < len; i++, src += 3 )
            जर( mask[i] )
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
        साठी( i = 0; i < len; i++, src += cn )
            जर( mask[i] )
            {
                पूर्णांक k = 0;
                #जर CV_ENABLE_UNROLLED
                साठी( ; k <= cn - 4; k += 4 )
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
                साठी( ; k < cn; k++ )
                    dst[k] += (WT)src[k];
                nzm++;
            }
    }
    परत nzm;
}


स्थिर पूर्णांक sum8u( स्थिरांक uchar* src, स्थिरांक uchar* mask, पूर्णांक* dst, पूर्णांक len, पूर्णांक cn )
{ CV_INSTRUMENT_REGION(); परत sum_(src, mask, dst, len, cn); }

स्थिर पूर्णांक sum8s( स्थिरांक schar* src, स्थिरांक uchar* mask, पूर्णांक* dst, पूर्णांक len, पूर्णांक cn )
{ CV_INSTRUMENT_REGION(); परत sum_(src, mask, dst, len, cn); }

स्थिर पूर्णांक sum16u( स्थिरांक ushort* src, स्थिरांक uchar* mask, पूर्णांक* dst, पूर्णांक len, पूर्णांक cn )
{ CV_INSTRUMENT_REGION(); परत sum_(src, mask, dst, len, cn); }

स्थिर पूर्णांक sum16s( स्थिरांक लघु* src, स्थिरांक uchar* mask, पूर्णांक* dst, पूर्णांक len, पूर्णांक cn )
{ CV_INSTRUMENT_REGION(); परत sum_(src, mask, dst, len, cn); }

स्थिर पूर्णांक sum32u( स्थिरांक अचिन्हित* src, स्थिरांक uchar* mask, द्विगुण* dst, पूर्णांक len, पूर्णांक cn )
{ CV_INSTRUMENT_REGION(); परत sum_<अचिन्हित, द्विगुण, uint64>(src, mask, dst, len, cn); }

स्थिर पूर्णांक sum32s( स्थिरांक पूर्णांक* src, स्थिरांक uchar* mask, द्विगुण* dst, पूर्णांक len, पूर्णांक cn )
{ CV_INSTRUMENT_REGION(); परत sum_<पूर्णांक, द्विगुण, int64>(src, mask, dst, len, cn); }

स्थिर पूर्णांक sum64u( स्थिरांक uint64* src, स्थिरांक uchar* mask, द्विगुण* dst, पूर्णांक len, पूर्णांक cn )
{ CV_INSTRUMENT_REGION(); परत sum_(src, mask, dst, len, cn); }

स्थिर पूर्णांक sum64s( स्थिरांक int64* src, स्थिरांक uchar* mask, द्विगुण* dst, पूर्णांक len, पूर्णांक cn )
{ CV_INSTRUMENT_REGION(); परत sum_(src, mask, dst, len, cn); }

स्थिर पूर्णांक sum32f( स्थिरांक दशांश* src, स्थिरांक uchar* mask, द्विगुण* dst, पूर्णांक len, पूर्णांक cn )
{ CV_INSTRUMENT_REGION(); परत sum_(src, mask, dst, len, cn); }

स्थिर पूर्णांक sum64f( स्थिरांक द्विगुण* src, स्थिरांक uchar* mask, द्विगुण* dst, पूर्णांक len, पूर्णांक cn )
{ CV_INSTRUMENT_REGION(); परत sum_(src, mask, dst, len, cn); }

स्थिर पूर्णांक sum16f( स्थिरांक hfloat* src, स्थिरांक uchar* mask, दशांश* dst, पूर्णांक len, पूर्णांक cn )
{ CV_INSTRUMENT_REGION(); परत sum_<hfloat, दशांश, दशांश>(src, mask, dst, len, cn); }

स्थिर पूर्णांक sum16bf( स्थिरांक bfloat* src, स्थिरांक uchar* mask, दशांश* dst, पूर्णांक len, पूर्णांक cn )
{ CV_INSTRUMENT_REGION(); परत sum_<bfloat, दशांश, दशांश>(src, mask, dst, len, cn); }

SumFunc getSumFunc(पूर्णांक depth)
{
    स्थिर SumFunc sumTab[CV_DEPTH_MAX] =
    {
        sumWrap<uchar, पूर्णांक, sum8u>,
        sumWrap<schar, पूर्णांक, sum8s>,
        sumWrap<ushort, पूर्णांक, sum16u>,
        sumWrap<लघु, पूर्णांक, sum16s>,
        sumWrap<पूर्णांक, द्विगुण, sum32s>,
        sumWrap<दशांश, द्विगुण, sum32f>,
        sumWrap<द्विगुण, द्विगुण, sum64f>,
        sumWrap<hfloat, दशांश, sum16f>,
        sumWrap<bfloat, दशांश, sum16bf>,
        0,
        sumWrap<uint64, द्विगुण, sum64u>,
        sumWrap<int64, द्विगुण, sum64s>,
        sumWrap<अचिन्हित, द्विगुण, sum32u>,
        0
    };

    परत sumTab[depth];
}

#endif

CV_CPU_OPTIMIZATION_NAMESPACE_END
} // namespace
