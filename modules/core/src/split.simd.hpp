// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


#അന്തര്ഭാവ "precomp.hpp"

നാമാകാശ cv { നാമാകാശ hal {
CV_CPU_OPTIMIZATION_NAMESPACE_BEGIN

ശൂന്യം split8u(സ്ഥിരം uchar* src, uchar** dst, പൂർണ്ണസംഖ്യ len, പൂർണ്ണസംഖ്യ cn);
ശൂന്യം split16u(സ്ഥിരം ushort* src, ushort** dst, പൂർണ്ണസംഖ്യ len, പൂർണ്ണസംഖ്യ cn);
ശൂന്യം split32s(സ്ഥിരം പൂർണ്ണസംഖ്യ* src, പൂർണ്ണസംഖ്യ** dst, പൂർണ്ണസംഖ്യ len, പൂർണ്ണസംഖ്യ cn);
ശൂന്യം split64s(സ്ഥിരം int64* src, int64** dst, പൂർണ്ണസംഖ്യ len, പൂർണ്ണസംഖ്യ cn);

#ifndef CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

#എങ്കിൽ (CV_SIMD || CV_SIMD_SCALABLE)
// see the comments for vecmerge_ in merge.cpp
template<typename T, typename VecT> സ്ഥിതം ശൂന്യം
vecsplit_( സ്ഥിരം T* src, T** dst, പൂർണ്ണസംഖ്യ len, പൂർണ്ണസംഖ്യ cn )
{
    സ്ഥിരം പൂർണ്ണസംഖ്യ VECSZ = VTraits<VecT>::vlanes();
    പൂർണ്ണസംഖ്യ i, i0 = 0;
    T* dst0 = dst[0];
    T* dst1 = dst[1];

    പൂർണ്ണസംഖ്യ r0 = (പൂർണ്ണസംഖ്യ)((size_t)(ശൂന്യം*)dst0 % (VECSZ*വലുപ്പം(T)));
    പൂർണ്ണസംഖ്യ r1 = (പൂർണ്ണസംഖ്യ)((size_t)(ശൂന്യം*)dst1 % (VECSZ*വലുപ്പം(T)));
    പൂർണ്ണസംഖ്യ r2 = cn > 2 ? (പൂർണ്ണസംഖ്യ)((size_t)(ശൂന്യം*)dst[2] % (VECSZ*വലുപ്പം(T))) : r0;
    പൂർണ്ണസംഖ്യ r3 = cn > 3 ? (പൂർണ്ണസംഖ്യ)((size_t)(ശൂന്യം*)dst[3] % (VECSZ*വലുപ്പം(T))) : r0;

    hal::StoreMode mode = hal::STORE_ALIGNED_NOCACHE;
    എങ്കിൽ( (r0|r1|r2|r3) != 0 )
    {
        mode = hal::STORE_UNALIGNED;
        എങ്കിൽ( r0 == r1 && r0 == r2 && r0 == r3 && r0 % വലുപ്പം(T) == 0 && len > VECSZ*2 )
            i0 = VECSZ - (r0 / വലുപ്പം(T));
    }

    എങ്കിൽ( cn == 2 )
    {
        വേണ്ടി( i = 0; i < len; i += VECSZ )
        {
            എങ്കിൽ( i > len - VECSZ )
            {
                i = len - VECSZ;
                mode = hal::STORE_UNALIGNED;
            }
            VecT a, b;
            v_load_deinterleave(src + i*cn, a, b);
            v_store(dst0 + i, a, mode);
            v_store(dst1 + i, b, mode);
            എങ്കിൽ( i < i0 )
            {
                i = i0 - VECSZ;
                mode = hal::STORE_ALIGNED_NOCACHE;
            }
        }
    }
    അല്ലെങ്കിൽ എങ്കിൽ( cn == 3 )
    {
        T* dst2 = dst[2];
        വേണ്ടി( i = 0; i < len; i += VECSZ )
        {
            എങ്കിൽ( i > len - VECSZ )
            {
                i = len - VECSZ;
                mode = hal::STORE_UNALIGNED;
            }
            VecT a, b, c;
            v_load_deinterleave(src + i*cn, a, b, c);
            v_store(dst0 + i, a, mode);
            v_store(dst1 + i, b, mode);
            v_store(dst2 + i, c, mode);
            എങ്കിൽ( i < i0 )
            {
                i = i0 - VECSZ;
                mode = hal::STORE_ALIGNED_NOCACHE;
            }
        }
    }
    അല്ലെങ്കിൽ
    {
        CV_Assert( cn == 4 );
        T* dst2 = dst[2];
        T* dst3 = dst[3];
        വേണ്ടി( i = 0; i < len; i += VECSZ )
        {
            എങ്കിൽ( i > len - VECSZ )
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
            എങ്കിൽ( i < i0 )
            {
                i = i0 - VECSZ;
                mode = hal::STORE_ALIGNED_NOCACHE;
            }
        }
    }
    vx_cleanup();
}
#endif

template<typename T> സ്ഥിതം ശൂന്യം
split_( സ്ഥിരം T* src, T** dst, പൂർണ്ണസംഖ്യ len, പൂർണ്ണസംഖ്യ cn )
{
    പൂർണ്ണസംഖ്യ k = cn % 4 ? cn % 4 : 4;
    പൂർണ്ണസംഖ്യ i, j;
    എങ്കിൽ( k == 1 )
    {
        T* dst0 = dst[0];

        എങ്കിൽ(cn == 1)
        {
            memcpy(dst0, src, len * വലുപ്പം(T));
        }
        അല്ലെങ്കിൽ
        {
            വേണ്ടി( i = 0, j = 0 ; i < len; i++, j += cn )
                dst0[i] = src[j];
        }
    }
    അല്ലെങ്കിൽ എങ്കിൽ( k == 2 )
    {
        T *dst0 = dst[0], *dst1 = dst[1];
        i = j = 0;

        വേണ്ടി( ; i < len; i++, j += cn )
        {
            dst0[i] = src[j];
            dst1[i] = src[j+1];
        }
    }
    അല്ലെങ്കിൽ എങ്കിൽ( k == 3 )
    {
        T *dst0 = dst[0], *dst1 = dst[1], *dst2 = dst[2];
        i = j = 0;

        വേണ്ടി( ; i < len; i++, j += cn )
        {
            dst0[i] = src[j];
            dst1[i] = src[j+1];
            dst2[i] = src[j+2];
        }
    }
    അല്ലെങ്കിൽ
    {
        T *dst0 = dst[0], *dst1 = dst[1], *dst2 = dst[2], *dst3 = dst[3];
        i = j = 0;

        വേണ്ടി( ; i < len; i++, j += cn )
        {
            dst0[i] = src[j]; dst1[i] = src[j+1];
            dst2[i] = src[j+2]; dst3[i] = src[j+3];
        }
    }

    വേണ്ടി( ; k < cn; k += 4 )
    {
        T *dst0 = dst[k], *dst1 = dst[k+1], *dst2 = dst[k+2], *dst3 = dst[k+3];
        വേണ്ടി( i = 0, j = k; i < len; i++, j += cn )
        {
            dst0[i] = src[j]; dst1[i] = src[j+1];
            dst2[i] = src[j+2]; dst3[i] = src[j+3];
        }
    }
}

ശൂന്യം split8u(സ്ഥിരം uchar* src, uchar** dst, പൂർണ്ണസംഖ്യ len, പൂർണ്ണസംഖ്യ cn )
{
    CV_INSTRUMENT_REGION();
#എങ്കിൽ (CV_SIMD || CV_SIMD_SCALABLE)
    എങ്കിൽ( len >= VTraits<v_uint8>::vlanes() && 2 <= cn && cn <= 4 )
        vecsplit_<uchar, v_uint8>(src, dst, len, cn);
    അല്ലെങ്കിൽ
#endif
        split_(src, dst, len, cn);
}

ശൂന്യം split16u(സ്ഥിരം ushort* src, ushort** dst, പൂർണ്ണസംഖ്യ len, പൂർണ്ണസംഖ്യ cn )
{
    CV_INSTRUMENT_REGION();
#എങ്കിൽ (CV_SIMD || CV_SIMD_SCALABLE)
    എങ്കിൽ( len >= VTraits<v_uint16>::vlanes() && 2 <= cn && cn <= 4 )
        vecsplit_<ushort, v_uint16>(src, dst, len, cn);
    അല്ലെങ്കിൽ
#endif
        split_(src, dst, len, cn);
}

ശൂന്യം split32s(സ്ഥിരം പൂർണ്ണസംഖ്യ* src, പൂർണ്ണസംഖ്യ** dst, പൂർണ്ണസംഖ്യ len, പൂർണ്ണസംഖ്യ cn )
{
    CV_INSTRUMENT_REGION();
#എങ്കിൽ (CV_SIMD || CV_SIMD_SCALABLE)
    എങ്കിൽ( len >= VTraits<v_uint32>::vlanes() && 2 <= cn && cn <= 4 )
        vecsplit_<പൂർണ്ണസംഖ്യ, v_int32>(src, dst, len, cn);
    അല്ലെങ്കിൽ
#endif
        split_(src, dst, len, cn);
}

ശൂന്യം split64s(സ്ഥിരം int64* src, int64** dst, പൂർണ്ണസംഖ്യ len, പൂർണ്ണസംഖ്യ cn )
{
    CV_INSTRUMENT_REGION();
#എങ്കിൽ (CV_SIMD || CV_SIMD_SCALABLE)
    എങ്കിൽ( len >= VTraits<v_int64>::vlanes() && 2 <= cn && cn <= 4 )
        vecsplit_<int64, v_int64>(src, dst, len, cn);
    അല്ലെങ്കിൽ
#endif
        split_(src, dst, len, cn);
}

#endif
CV_CPU_OPTIMIZATION_NAMESPACE_END
}} // namespace
