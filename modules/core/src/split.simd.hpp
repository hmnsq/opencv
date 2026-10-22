// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


#ਅਨ੍ਤਰ੍ਭਾਵ "precomp.hpp"

ਨਾਮਾਕਾਸ਼ cv { ਨਾਮਾਕਾਸ਼ hal {
CV_CPU_OPTIMIZATION_NAMESPACE_BEGIN

ਖਾਲੀ split8u(ਅਚਲ uchar* src, uchar** dst, ਪੂਰਨ len, ਪੂਰਨ cn);
ਖਾਲੀ split16u(ਅਚਲ ushort* src, ushort** dst, ਪੂਰਨ len, ਪੂਰਨ cn);
ਖਾਲੀ split32s(ਅਚਲ ਪੂਰਨ* src, ਪੂਰਨ** dst, ਪੂਰਨ len, ਪੂਰਨ cn);
ਖਾਲੀ split64s(ਅਚਲ int64* src, int64** dst, ਪੂਰਨ len, ਪੂਰਨ cn);

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

#ਜੇ (CV_SIMD || CV_SIMD_SCALABLE)
// see the comments for vecmerge_ in merge.cpp
ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ T, ਕਿਸਮ_ਨਾਂ VecT> ਸਥਿਰ ਖਾਲੀ
vecsplit_( ਅਚਲ T* src, T** dst, ਪੂਰਨ len, ਪੂਰਨ cn )
{
    ਅਚਲ ਪੂਰਨ VECSZ = VTraits<VecT>::vlanes();
    ਪੂਰਨ i, i0 = 0;
    T* dst0 = dst[0];
    T* dst1 = dst[1];

    ਪੂਰਨ r0 = (ਪੂਰਨ)((size_t)(ਖਾਲੀ*)dst0 % (VECSZ*ਆਕਾਰ(T)));
    ਪੂਰਨ r1 = (ਪੂਰਨ)((size_t)(ਖਾਲੀ*)dst1 % (VECSZ*ਆਕਾਰ(T)));
    ਪੂਰਨ r2 = cn > 2 ? (ਪੂਰਨ)((size_t)(ਖਾਲੀ*)dst[2] % (VECSZ*ਆਕਾਰ(T))) : r0;
    ਪੂਰਨ r3 = cn > 3 ? (ਪੂਰਨ)((size_t)(ਖਾਲੀ*)dst[3] % (VECSZ*ਆਕਾਰ(T))) : r0;

    hal::Store_ਬਹੁਲਕ ਬਹੁਲਕ = hal::STORE_ALIGNED_NOCACHE;
    ਜੇ( (r0|r1|r2|r3) != 0 )
    {
        ਬਹੁਲਕ = hal::STORE_UNALIGNED;
        ਜੇ( r0 == r1 && r0 == r2 && r0 == r3 && r0 % ਆਕਾਰ(T) == 0 && len > VECSZ*2 )
            i0 = VECSZ - (r0 / ਆਕਾਰ(T));
    }

    ਜੇ( cn == 2 )
    {
        ਲਈ( i = 0; i < len; i += VECSZ )
        {
            ਜੇ( i > len - VECSZ )
            {
                i = len - VECSZ;
                ਬਹੁਲਕ = hal::STORE_UNALIGNED;
            }
            VecT a, b;
            v_ਲੱਦੋ_deinterleave(src + i*cn, a, b);
            v_store(dst0 + i, a, ਬਹੁਲਕ);
            v_store(dst1 + i, b, ਬਹੁਲਕ);
            ਜੇ( i < i0 )
            {
                i = i0 - VECSZ;
                ਬਹੁਲਕ = hal::STORE_ALIGNED_NOCACHE;
            }
        }
    }
    ਨਹੀਂਤਾਂ ਜੇ( cn == 3 )
    {
        T* dst2 = dst[2];
        ਲਈ( i = 0; i < len; i += VECSZ )
        {
            ਜੇ( i > len - VECSZ )
            {
                i = len - VECSZ;
                ਬਹੁਲਕ = hal::STORE_UNALIGNED;
            }
            VecT a, b, c;
            v_ਲੱਦੋ_deinterleave(src + i*cn, a, b, c);
            v_store(dst0 + i, a, ਬਹੁਲਕ);
            v_store(dst1 + i, b, ਬਹੁਲਕ);
            v_store(dst2 + i, c, ਬਹੁਲਕ);
            ਜੇ( i < i0 )
            {
                i = i0 - VECSZ;
                ਬਹੁਲਕ = hal::STORE_ALIGNED_NOCACHE;
            }
        }
    }
    ਨਹੀਂਤਾਂ
    {
        CV_Assert( cn == 4 );
        T* dst2 = dst[2];
        T* dst3 = dst[3];
        ਲਈ( i = 0; i < len; i += VECSZ )
        {
            ਜੇ( i > len - VECSZ )
            {
                i = len - VECSZ;
                ਬਹੁਲਕ = hal::STORE_UNALIGNED;
            }
            VecT a, b, c, d;
            v_ਲੱਦੋ_deinterleave(src + i*cn, a, b, c, d);
            v_store(dst0 + i, a, ਬਹੁਲਕ);
            v_store(dst1 + i, b, ਬਹੁਲਕ);
            v_store(dst2 + i, c, ਬਹੁਲਕ);
            v_store(dst3 + i, d, ਬਹੁਲਕ);
            ਜੇ( i < i0 )
            {
                i = i0 - VECSZ;
                ਬਹੁਲਕ = hal::STORE_ALIGNED_NOCACHE;
            }
        }
    }
    vx_cleanup();
}
#ਜੇ_ਅੰਤ

ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ T> ਸਥਿਰ ਖਾਲੀ
ਵੰਡੋ_2( ਅਚਲ T* src, T** dst, ਪੂਰਨ len, ਪੂਰਨ cn )
{
    ਪੂਰਨ k = cn % 4 ? cn % 4 : 4;
    ਪੂਰਨ i, j;
    ਜੇ( k == 1 )
    {
        T* dst0 = dst[0];

        ਜੇ(cn == 1)
        {
            ਸਮ੍ਰਿਤੀ_ਨਕਲ(dst0, src, len * ਆਕਾਰ(T));
        }
        ਨਹੀਂਤਾਂ
        {
            ਲਈ( i = 0, j = 0 ; i < len; i++, j += cn )
                dst0[i] = src[j];
        }
    }
    ਨਹੀਂਤਾਂ ਜੇ( k == 2 )
    {
        T *dst0 = dst[0], *dst1 = dst[1];
        i = j = 0;

        ਲਈ( ; i < len; i++, j += cn )
        {
            dst0[i] = src[j];
            dst1[i] = src[j+1];
        }
    }
    ਨਹੀਂਤਾਂ ਜੇ( k == 3 )
    {
        T *dst0 = dst[0], *dst1 = dst[1], *dst2 = dst[2];
        i = j = 0;

        ਲਈ( ; i < len; i++, j += cn )
        {
            dst0[i] = src[j];
            dst1[i] = src[j+1];
            dst2[i] = src[j+2];
        }
    }
    ਨਹੀਂਤਾਂ
    {
        T *dst0 = dst[0], *dst1 = dst[1], *dst2 = dst[2], *dst3 = dst[3];
        i = j = 0;

        ਲਈ( ; i < len; i++, j += cn )
        {
            dst0[i] = src[j]; dst1[i] = src[j+1];
            dst2[i] = src[j+2]; dst3[i] = src[j+3];
        }
    }

    ਲਈ( ; k < cn; k += 4 )
    {
        T *dst0 = dst[k], *dst1 = dst[k+1], *dst2 = dst[k+2], *dst3 = dst[k+3];
        ਲਈ( i = 0, j = k; i < len; i++, j += cn )
        {
            dst0[i] = src[j]; dst1[i] = src[j+1];
            dst2[i] = src[j+2]; dst3[i] = src[j+3];
        }
    }
}

ਖਾਲੀ split8u(ਅਚਲ uchar* src, uchar** dst, ਪੂਰਨ len, ਪੂਰਨ cn )
{
    CV_INSTRUMENT_REGION();
#ਜੇ (CV_SIMD || CV_SIMD_SCALABLE)
    ਜੇ( len >= VTraits<v_uint8>::vlanes() && 2 <= cn && cn <= 4 )
        vecsplit_<uchar, v_uint8>(src, dst, len, cn);
    ਨਹੀਂਤਾਂ
#ਜੇ_ਅੰਤ
        ਵੰਡੋ_2(src, dst, len, cn);
}

ਖਾਲੀ split16u(ਅਚਲ ushort* src, ushort** dst, ਪੂਰਨ len, ਪੂਰਨ cn )
{
    CV_INSTRUMENT_REGION();
#ਜੇ (CV_SIMD || CV_SIMD_SCALABLE)
    ਜੇ( len >= VTraits<v_uint16>::vlanes() && 2 <= cn && cn <= 4 )
        vecsplit_<ushort, v_uint16>(src, dst, len, cn);
    ਨਹੀਂਤਾਂ
#ਜੇ_ਅੰਤ
        ਵੰਡੋ_2(src, dst, len, cn);
}

ਖਾਲੀ split32s(ਅਚਲ ਪੂਰਨ* src, ਪੂਰਨ** dst, ਪੂਰਨ len, ਪੂਰਨ cn )
{
    CV_INSTRUMENT_REGION();
#ਜੇ (CV_SIMD || CV_SIMD_SCALABLE)
    ਜੇ( len >= VTraits<v_uint32>::vlanes() && 2 <= cn && cn <= 4 )
        vecsplit_<ਪੂਰਨ, v_int32>(src, dst, len, cn);
    ਨਹੀਂਤਾਂ
#ਜੇ_ਅੰਤ
        ਵੰਡੋ_2(src, dst, len, cn);
}

ਖਾਲੀ split64s(ਅਚਲ int64* src, int64** dst, ਪੂਰਨ len, ਪੂਰਨ cn )
{
    CV_INSTRUMENT_REGION();
#ਜੇ (CV_SIMD || CV_SIMD_SCALABLE)
    ਜੇ( len >= VTraits<v_int64>::vlanes() && 2 <= cn && cn <= 4 )
        vecsplit_<int64, v_int64>(src, dst, len, cn);
    ਨਹੀਂਤਾਂ
#ਜੇ_ਅੰਤ
        ਵੰਡੋ_2(src, dst, len, cn);
}

#ਜੇ_ਅੰਤ
CV_CPU_OPTIMIZATION_NAMESPACE_ਸਿਰਾ
}} // namespace
