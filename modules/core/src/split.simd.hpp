// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


#અન્તર્ભાવ "precomp.hpp"

નામાકાશ cv { નામાકાશ hal {
CV_CPU_OPTIMIZATION_NAMESPACE_BEGIN

શૂન્ય split8u(અચલ uchar* src, uchar** dst, પૂર્ણાંક len, પૂર્ણાંક cn);
શૂન્ય split16u(અચલ ushort* src, ushort** dst, પૂર્ણાંક len, પૂર્ણાંક cn);
શૂન્ય split32s(અચલ પૂર્ણાંક* src, પૂર્ણાંક** dst, પૂર્ણાંક len, પૂર્ણાંક cn);
શૂન્ય split64s(અચલ int64* src, int64** dst, પૂર્ણાંક len, પૂર્ણાંક cn);

#ifndef CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

#જો (CV_SIMD || CV_SIMD_SCALABLE)
// see the comments for vecmerge_ in merge.cpp
template<typename T, typename VecT> સ્થિર શૂન્ય
vecsplit_( અચલ T* src, T** dst, પૂર્ણાંક len, પૂર્ણાંક cn )
{
    અચલ પૂર્ણાંક VECSZ = VTraits<VecT>::vlanes();
    પૂર્ણાંક i, i0 = 0;
    T* dst0 = dst[0];
    T* dst1 = dst[1];

    પૂર્ણાંક r0 = (પૂર્ણાંક)((size_t)(શૂન્ય*)dst0 % (VECSZ*કદ(T)));
    પૂર્ણાંક r1 = (પૂર્ણાંક)((size_t)(શૂન્ય*)dst1 % (VECSZ*કદ(T)));
    પૂર્ણાંક r2 = cn > 2 ? (પૂર્ણાંક)((size_t)(શૂન્ય*)dst[2] % (VECSZ*કદ(T))) : r0;
    પૂર્ણાંક r3 = cn > 3 ? (પૂર્ણાંક)((size_t)(શૂન્ય*)dst[3] % (VECSZ*કદ(T))) : r0;

    hal::StoreMode mode = hal::STORE_ALIGNED_NOCACHE;
    જો( (r0|r1|r2|r3) != 0 )
    {
        mode = hal::STORE_UNALIGNED;
        જો( r0 == r1 && r0 == r2 && r0 == r3 && r0 % કદ(T) == 0 && len > VECSZ*2 )
            i0 = VECSZ - (r0 / કદ(T));
    }

    જો( cn == 2 )
    {
        માટે( i = 0; i < len; i += VECSZ )
        {
            જો( i > len - VECSZ )
            {
                i = len - VECSZ;
                mode = hal::STORE_UNALIGNED;
            }
            VecT a, b;
            v_load_deinterleave(src + i*cn, a, b);
            v_store(dst0 + i, a, mode);
            v_store(dst1 + i, b, mode);
            જો( i < i0 )
            {
                i = i0 - VECSZ;
                mode = hal::STORE_ALIGNED_NOCACHE;
            }
        }
    }
    નહીંતર જો( cn == 3 )
    {
        T* dst2 = dst[2];
        માટે( i = 0; i < len; i += VECSZ )
        {
            જો( i > len - VECSZ )
            {
                i = len - VECSZ;
                mode = hal::STORE_UNALIGNED;
            }
            VecT a, b, c;
            v_load_deinterleave(src + i*cn, a, b, c);
            v_store(dst0 + i, a, mode);
            v_store(dst1 + i, b, mode);
            v_store(dst2 + i, c, mode);
            જો( i < i0 )
            {
                i = i0 - VECSZ;
                mode = hal::STORE_ALIGNED_NOCACHE;
            }
        }
    }
    નહીંતર
    {
        CV_Assert( cn == 4 );
        T* dst2 = dst[2];
        T* dst3 = dst[3];
        માટે( i = 0; i < len; i += VECSZ )
        {
            જો( i > len - VECSZ )
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
            જો( i < i0 )
            {
                i = i0 - VECSZ;
                mode = hal::STORE_ALIGNED_NOCACHE;
            }
        }
    }
    vx_cleanup();
}
#endif

template<typename T> સ્થિર શૂન્ય
split_( અચલ T* src, T** dst, પૂર્ણાંક len, પૂર્ણાંક cn )
{
    પૂર્ણાંક k = cn % 4 ? cn % 4 : 4;
    પૂર્ણાંક i, j;
    જો( k == 1 )
    {
        T* dst0 = dst[0];

        જો(cn == 1)
        {
            memcpy(dst0, src, len * કદ(T));
        }
        નહીંતર
        {
            માટે( i = 0, j = 0 ; i < len; i++, j += cn )
                dst0[i] = src[j];
        }
    }
    નહીંતર જો( k == 2 )
    {
        T *dst0 = dst[0], *dst1 = dst[1];
        i = j = 0;

        માટે( ; i < len; i++, j += cn )
        {
            dst0[i] = src[j];
            dst1[i] = src[j+1];
        }
    }
    નહીંતર જો( k == 3 )
    {
        T *dst0 = dst[0], *dst1 = dst[1], *dst2 = dst[2];
        i = j = 0;

        માટે( ; i < len; i++, j += cn )
        {
            dst0[i] = src[j];
            dst1[i] = src[j+1];
            dst2[i] = src[j+2];
        }
    }
    નહીંતર
    {
        T *dst0 = dst[0], *dst1 = dst[1], *dst2 = dst[2], *dst3 = dst[3];
        i = j = 0;

        માટે( ; i < len; i++, j += cn )
        {
            dst0[i] = src[j]; dst1[i] = src[j+1];
            dst2[i] = src[j+2]; dst3[i] = src[j+3];
        }
    }

    માટે( ; k < cn; k += 4 )
    {
        T *dst0 = dst[k], *dst1 = dst[k+1], *dst2 = dst[k+2], *dst3 = dst[k+3];
        માટે( i = 0, j = k; i < len; i++, j += cn )
        {
            dst0[i] = src[j]; dst1[i] = src[j+1];
            dst2[i] = src[j+2]; dst3[i] = src[j+3];
        }
    }
}

શૂન્ય split8u(અચલ uchar* src, uchar** dst, પૂર્ણાંક len, પૂર્ણાંક cn )
{
    CV_INSTRUMENT_REGION();
#જો (CV_SIMD || CV_SIMD_SCALABLE)
    જો( len >= VTraits<v_uint8>::vlanes() && 2 <= cn && cn <= 4 )
        vecsplit_<uchar, v_uint8>(src, dst, len, cn);
    નહીંતર
#endif
        split_(src, dst, len, cn);
}

શૂન્ય split16u(અચલ ushort* src, ushort** dst, પૂર્ણાંક len, પૂર્ણાંક cn )
{
    CV_INSTRUMENT_REGION();
#જો (CV_SIMD || CV_SIMD_SCALABLE)
    જો( len >= VTraits<v_uint16>::vlanes() && 2 <= cn && cn <= 4 )
        vecsplit_<ushort, v_uint16>(src, dst, len, cn);
    નહીંતર
#endif
        split_(src, dst, len, cn);
}

શૂન્ય split32s(અચલ પૂર્ણાંક* src, પૂર્ણાંક** dst, પૂર્ણાંક len, પૂર્ણાંક cn )
{
    CV_INSTRUMENT_REGION();
#જો (CV_SIMD || CV_SIMD_SCALABLE)
    જો( len >= VTraits<v_uint32>::vlanes() && 2 <= cn && cn <= 4 )
        vecsplit_<પૂર્ણાંક, v_int32>(src, dst, len, cn);
    નહીંતર
#endif
        split_(src, dst, len, cn);
}

શૂન્ય split64s(અચલ int64* src, int64** dst, પૂર્ણાંક len, પૂર્ણાંક cn )
{
    CV_INSTRUMENT_REGION();
#જો (CV_SIMD || CV_SIMD_SCALABLE)
    જો( len >= VTraits<v_int64>::vlanes() && 2 <= cn && cn <= 4 )
        vecsplit_<int64, v_int64>(src, dst, len, cn);
    નહીંતર
#endif
        split_(src, dst, len, cn);
}

#endif
CV_CPU_OPTIMIZATION_NAMESPACE_END
}} // namespace
