// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


#ଅନ୍ତର୍ଭାଵ "precomp.hpp"

ନାମାକାଶ cv { ନାମାକାଶ hal {
CV_CPU_OPTIMIZATION_NAMESPACE_BEGIN

ଶୂନ୍ୟ split8u(ଅଚଳ uchar* src, uchar** dst, ପୂର୍ଣ୍ଣସଂଖ୍ୟା len, ପୂର୍ଣ୍ଣସଂଖ୍ୟା cn);
ଶୂନ୍ୟ split16u(ଅଚଳ ushort* src, ushort** dst, ପୂର୍ଣ୍ଣସଂଖ୍ୟା len, ପୂର୍ଣ୍ଣସଂଖ୍ୟା cn);
ଶୂନ୍ୟ split32s(ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା* src, ପୂର୍ଣ୍ଣସଂଖ୍ୟା** dst, ପୂର୍ଣ୍ଣସଂଖ୍ୟା len, ପୂର୍ଣ୍ଣସଂଖ୍ୟା cn);
ଶୂନ୍ୟ split64s(ଅଚଳ int64* src, int64** dst, ପୂର୍ଣ୍ଣସଂଖ୍ୟା len, ପୂର୍ଣ୍ଣସଂଖ୍ୟା cn);

#ifndef CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

#ଯଦି (CV_SIMD || CV_SIMD_SCALABLE)
// see the comments for vecmerge_ in merge.cpp
template<typename T, typename VecT> ସ୍ଥିର ଶୂନ୍ୟ
vecsplit_( ଅଚଳ T* src, T** dst, ପୂର୍ଣ୍ଣସଂଖ୍ୟା len, ପୂର୍ଣ୍ଣସଂଖ୍ୟା cn )
{
    ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା VECSZ = VTraits<VecT>::vlanes();
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i, i0 = 0;
    T* dst0 = dst[0];
    T* dst1 = dst[1];

    ପୂର୍ଣ୍ଣସଂଖ୍ୟା r0 = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)((size_t)(ଶୂନ୍ୟ*)dst0 % (VECSZ*ଆକାର(T)));
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା r1 = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)((size_t)(ଶୂନ୍ୟ*)dst1 % (VECSZ*ଆକାର(T)));
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା r2 = cn > 2 ? (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)((size_t)(ଶୂନ୍ୟ*)dst[2] % (VECSZ*ଆକାର(T))) : r0;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା r3 = cn > 3 ? (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)((size_t)(ଶୂନ୍ୟ*)dst[3] % (VECSZ*ଆକାର(T))) : r0;

    hal::StoreMode mode = hal::STORE_ALIGNED_NOCACHE;
    ଯଦି( (r0|r1|r2|r3) != 0 )
    {
        mode = hal::STORE_UNALIGNED;
        ଯଦି( r0 == r1 && r0 == r2 && r0 == r3 && r0 % ଆକାର(T) == 0 && len > VECSZ*2 )
            i0 = VECSZ - (r0 / ଆକାର(T));
    }

    ଯଦି( cn == 2 )
    {
        ପାଇଁ( i = 0; i < len; i += VECSZ )
        {
            ଯଦି( i > len - VECSZ )
            {
                i = len - VECSZ;
                mode = hal::STORE_UNALIGNED;
            }
            VecT a, b;
            v_load_deinterleave(src + i*cn, a, b);
            v_store(dst0 + i, a, mode);
            v_store(dst1 + i, b, mode);
            ଯଦି( i < i0 )
            {
                i = i0 - VECSZ;
                mode = hal::STORE_ALIGNED_NOCACHE;
            }
        }
    }
    ଅନ୍ୟଥା ଯଦି( cn == 3 )
    {
        T* dst2 = dst[2];
        ପାଇଁ( i = 0; i < len; i += VECSZ )
        {
            ଯଦି( i > len - VECSZ )
            {
                i = len - VECSZ;
                mode = hal::STORE_UNALIGNED;
            }
            VecT a, b, c;
            v_load_deinterleave(src + i*cn, a, b, c);
            v_store(dst0 + i, a, mode);
            v_store(dst1 + i, b, mode);
            v_store(dst2 + i, c, mode);
            ଯଦି( i < i0 )
            {
                i = i0 - VECSZ;
                mode = hal::STORE_ALIGNED_NOCACHE;
            }
        }
    }
    ଅନ୍ୟଥା
    {
        CV_Assert( cn == 4 );
        T* dst2 = dst[2];
        T* dst3 = dst[3];
        ପାଇଁ( i = 0; i < len; i += VECSZ )
        {
            ଯଦି( i > len - VECSZ )
            {
                i = len - VECSZ;
                mode = hal::STORE_UNALIGNED;
            }
            VecT a, b, c, d;
            v_load_deinterleave(src + i*cn, a, b, c, d);
            v_store(dst0 + i, a, mode);
            v_store(dst1 + i, b, mode);
            v_store(dst2 + i, c, mode);
            v_store(dst3 + i, d, mode);
            ଯଦି( i < i0 )
            {
                i = i0 - VECSZ;
                mode = hal::STORE_ALIGNED_NOCACHE;
            }
        }
    }
    vx_cleanup();
}
#endif

template<typename T> ସ୍ଥିର ଶୂନ୍ୟ
split_( ଅଚଳ T* src, T** dst, ପୂର୍ଣ୍ଣସଂଖ୍ୟା len, ପୂର୍ଣ୍ଣସଂଖ୍ୟା cn )
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା k = cn % 4 ? cn % 4 : 4;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i, j;
    ଯଦି( k == 1 )
    {
        T* dst0 = dst[0];

        ଯଦି(cn == 1)
        {
            memcpy(dst0, src, len * ଆକାର(T));
        }
        ଅନ୍ୟଥା
        {
            ପାଇଁ( i = 0, j = 0 ; i < len; i++, j += cn )
                dst0[i] = src[j];
        }
    }
    ଅନ୍ୟଥା ଯଦି( k == 2 )
    {
        T *dst0 = dst[0], *dst1 = dst[1];
        i = j = 0;

        ପାଇଁ( ; i < len; i++, j += cn )
        {
            dst0[i] = src[j];
            dst1[i] = src[j+1];
        }
    }
    ଅନ୍ୟଥା ଯଦି( k == 3 )
    {
        T *dst0 = dst[0], *dst1 = dst[1], *dst2 = dst[2];
        i = j = 0;

        ପାଇଁ( ; i < len; i++, j += cn )
        {
            dst0[i] = src[j];
            dst1[i] = src[j+1];
            dst2[i] = src[j+2];
        }
    }
    ଅନ୍ୟଥା
    {
        T *dst0 = dst[0], *dst1 = dst[1], *dst2 = dst[2], *dst3 = dst[3];
        i = j = 0;

        ପାଇଁ( ; i < len; i++, j += cn )
        {
            dst0[i] = src[j]; dst1[i] = src[j+1];
            dst2[i] = src[j+2]; dst3[i] = src[j+3];
        }
    }

    ପାଇଁ( ; k < cn; k += 4 )
    {
        T *dst0 = dst[k], *dst1 = dst[k+1], *dst2 = dst[k+2], *dst3 = dst[k+3];
        ପାଇଁ( i = 0, j = k; i < len; i++, j += cn )
        {
            dst0[i] = src[j]; dst1[i] = src[j+1];
            dst2[i] = src[j+2]; dst3[i] = src[j+3];
        }
    }
}

ଶୂନ୍ୟ split8u(ଅଚଳ uchar* src, uchar** dst, ପୂର୍ଣ୍ଣସଂଖ୍ୟା len, ପୂର୍ଣ୍ଣସଂଖ୍ୟା cn )
{
    CV_INSTRUMENT_REGION();
#ଯଦି (CV_SIMD || CV_SIMD_SCALABLE)
    ଯଦି( len >= VTraits<v_uint8>::vlanes() && 2 <= cn && cn <= 4 )
        vecsplit_<uchar, v_uint8>(src, dst, len, cn);
    ଅନ୍ୟଥା
#endif
        split_(src, dst, len, cn);
}

ଶୂନ୍ୟ split16u(ଅଚଳ ushort* src, ushort** dst, ପୂର୍ଣ୍ଣସଂଖ୍ୟା len, ପୂର୍ଣ୍ଣସଂଖ୍ୟା cn )
{
    CV_INSTRUMENT_REGION();
#ଯଦି (CV_SIMD || CV_SIMD_SCALABLE)
    ଯଦି( len >= VTraits<v_uint16>::vlanes() && 2 <= cn && cn <= 4 )
        vecsplit_<ushort, v_uint16>(src, dst, len, cn);
    ଅନ୍ୟଥା
#endif
        split_(src, dst, len, cn);
}

ଶୂନ୍ୟ split32s(ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା* src, ପୂର୍ଣ୍ଣସଂଖ୍ୟା** dst, ପୂର୍ଣ୍ଣସଂଖ୍ୟା len, ପୂର୍ଣ୍ଣସଂଖ୍ୟା cn )
{
    CV_INSTRUMENT_REGION();
#ଯଦି (CV_SIMD || CV_SIMD_SCALABLE)
    ଯଦି( len >= VTraits<v_uint32>::vlanes() && 2 <= cn && cn <= 4 )
        vecsplit_<ପୂର୍ଣ୍ଣସଂଖ୍ୟା, v_int32>(src, dst, len, cn);
    ଅନ୍ୟଥା
#endif
        split_(src, dst, len, cn);
}

ଶୂନ୍ୟ split64s(ଅଚଳ int64* src, int64** dst, ପୂର୍ଣ୍ଣସଂଖ୍ୟା len, ପୂର୍ଣ୍ଣସଂଖ୍ୟା cn )
{
    CV_INSTRUMENT_REGION();
#ଯଦି (CV_SIMD || CV_SIMD_SCALABLE)
    ଯଦି( len >= VTraits<v_int64>::vlanes() && 2 <= cn && cn <= 4 )
        vecsplit_<int64, v_int64>(src, dst, len, cn);
    ଅନ୍ୟଥା
#endif
        split_(src, dst, len, cn);
}

#endif
CV_CPU_OPTIMIZATION_NAMESPACE_END
}} // namespace
