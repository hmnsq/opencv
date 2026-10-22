// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


#ضمن "precomp.hpp"

نطاق cv { نطاق hal {
CV_CPU_OPTIMIZATION_NAMESPACE_BEGIN

فراغ split8u(ثابت uchar* src, uchar** dst, صحيح len, صحيح cn);
فراغ split16u(ثابت ushort* src, ushort** dst, صحيح len, صحيح cn);
فراغ split32s(ثابت صحيح* src, صحيح** dst, صحيح len, صحيح cn);
فراغ split64s(ثابت int64* src, int64** dst, صحيح len, صحيح cn);

#ifndef CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

#𐤀𐤌 (CV_SIMD || CV_SIMD_SCALABLE)
// see the comments for vecmerge_ in merge.cpp
template<typename T, typename VecT> ساكن فراغ
vecsplit_( ثابت T* src, T** dst, صحيح len, صحيح cn )
{
    ثابت صحيح VECSZ = VTraits<VecT>::vlanes();
    صحيح i, i0 = 0;
    T* dst0 = dst[0];
    T* dst1 = dst[1];

    صحيح r0 = (صحيح)((size_t)(فراغ*)dst0 % (VECSZ*حجم(T)));
    صحيح r1 = (صحيح)((size_t)(فراغ*)dst1 % (VECSZ*حجم(T)));
    صحيح r2 = cn > 2 ? (صحيح)((size_t)(فراغ*)dst[2] % (VECSZ*حجم(T))) : r0;
    صحيح r3 = cn > 3 ? (صحيح)((size_t)(فراغ*)dst[3] % (VECSZ*حجم(T))) : r0;

    hal::StoreMode mode = hal::STORE_ALIGNED_NOCACHE;
    𐤀𐤌( (r0|r1|r2|r3) != 0 )
    {
        mode = hal::STORE_UNALIGNED;
        𐤀𐤌( r0 == r1 && r0 == r2 && r0 == r3 && r0 % حجم(T) == 0 && len > VECSZ*2 )
            i0 = VECSZ - (r0 / حجم(T));
    }

    𐤀𐤌( cn == 2 )
    {
        لكل( i = 0; i < len; i += VECSZ )
        {
            𐤀𐤌( i > len - VECSZ )
            {
                i = len - VECSZ;
                mode = hal::STORE_UNALIGNED;
            }
            VecT a, b;
            v_load_deinterleave(src + i*cn, a, b);
            v_store(dst0 + i, a, mode);
            v_store(dst1 + i, b, mode);
            𐤀𐤌( i < i0 )
            {
                i = i0 - VECSZ;
                mode = hal::STORE_ALIGNED_NOCACHE;
            }
        }
    }
    وإلا 𐤀𐤌( cn == 3 )
    {
        T* dst2 = dst[2];
        لكل( i = 0; i < len; i += VECSZ )
        {
            𐤀𐤌( i > len - VECSZ )
            {
                i = len - VECSZ;
                mode = hal::STORE_UNALIGNED;
            }
            VecT a, b, c;
            v_load_deinterleave(src + i*cn, a, b, c);
            v_store(dst0 + i, a, mode);
            v_store(dst1 + i, b, mode);
            v_store(dst2 + i, c, mode);
            𐤀𐤌( i < i0 )
            {
                i = i0 - VECSZ;
                mode = hal::STORE_ALIGNED_NOCACHE;
            }
        }
    }
    وإلا
    {
        CV_Assert( cn == 4 );
        T* dst2 = dst[2];
        T* dst3 = dst[3];
        لكل( i = 0; i < len; i += VECSZ )
        {
            𐤀𐤌( i > len - VECSZ )
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
            𐤀𐤌( i < i0 )
            {
                i = i0 - VECSZ;
                mode = hal::STORE_ALIGNED_NOCACHE;
            }
        }
    }
    vx_cleanup();
}
#endif

template<typename T> ساكن فراغ
split_( ثابت T* src, T** dst, صحيح len, صحيح cn )
{
    صحيح k = cn % 4 ? cn % 4 : 4;
    صحيح i, j;
    𐤀𐤌( k == 1 )
    {
        T* dst0 = dst[0];

        𐤀𐤌(cn == 1)
        {
            memcpy(dst0, src, len * حجم(T));
        }
        وإلا
        {
            لكل( i = 0, j = 0 ; i < len; i++, j += cn )
                dst0[i] = src[j];
        }
    }
    وإلا 𐤀𐤌( k == 2 )
    {
        T *dst0 = dst[0], *dst1 = dst[1];
        i = j = 0;

        لكل( ; i < len; i++, j += cn )
        {
            dst0[i] = src[j];
            dst1[i] = src[j+1];
        }
    }
    وإلا 𐤀𐤌( k == 3 )
    {
        T *dst0 = dst[0], *dst1 = dst[1], *dst2 = dst[2];
        i = j = 0;

        لكل( ; i < len; i++, j += cn )
        {
            dst0[i] = src[j];
            dst1[i] = src[j+1];
            dst2[i] = src[j+2];
        }
    }
    وإلا
    {
        T *dst0 = dst[0], *dst1 = dst[1], *dst2 = dst[2], *dst3 = dst[3];
        i = j = 0;

        لكل( ; i < len; i++, j += cn )
        {
            dst0[i] = src[j]; dst1[i] = src[j+1];
            dst2[i] = src[j+2]; dst3[i] = src[j+3];
        }
    }

    لكل( ; k < cn; k += 4 )
    {
        T *dst0 = dst[k], *dst1 = dst[k+1], *dst2 = dst[k+2], *dst3 = dst[k+3];
        لكل( i = 0, j = k; i < len; i++, j += cn )
        {
            dst0[i] = src[j]; dst1[i] = src[j+1];
            dst2[i] = src[j+2]; dst3[i] = src[j+3];
        }
    }
}

فراغ split8u(ثابت uchar* src, uchar** dst, صحيح len, صحيح cn )
{
    CV_INSTRUMENT_REGION();
#𐤀𐤌 (CV_SIMD || CV_SIMD_SCALABLE)
    𐤀𐤌( len >= VTraits<v_uint8>::vlanes() && 2 <= cn && cn <= 4 )
        vecsplit_<uchar, v_uint8>(src, dst, len, cn);
    وإلا
#endif
        split_(src, dst, len, cn);
}

فراغ split16u(ثابت ushort* src, ushort** dst, صحيح len, صحيح cn )
{
    CV_INSTRUMENT_REGION();
#𐤀𐤌 (CV_SIMD || CV_SIMD_SCALABLE)
    𐤀𐤌( len >= VTraits<v_uint16>::vlanes() && 2 <= cn && cn <= 4 )
        vecsplit_<ushort, v_uint16>(src, dst, len, cn);
    وإلا
#endif
        split_(src, dst, len, cn);
}

فراغ split32s(ثابت صحيح* src, صحيح** dst, صحيح len, صحيح cn )
{
    CV_INSTRUMENT_REGION();
#𐤀𐤌 (CV_SIMD || CV_SIMD_SCALABLE)
    𐤀𐤌( len >= VTraits<v_uint32>::vlanes() && 2 <= cn && cn <= 4 )
        vecsplit_<صحيح, v_int32>(src, dst, len, cn);
    وإلا
#endif
        split_(src, dst, len, cn);
}

فراغ split64s(ثابت int64* src, int64** dst, صحيح len, صحيح cn )
{
    CV_INSTRUMENT_REGION();
#𐤀𐤌 (CV_SIMD || CV_SIMD_SCALABLE)
    𐤀𐤌( len >= VTraits<v_int64>::vlanes() && 2 <= cn && cn <= 4 )
        vecsplit_<int64, v_int64>(src, dst, len, cn);
    وإلا
#endif
        split_(src, dst, len, cn);
}

#endif
CV_CPU_OPTIMIZATION_NAMESPACE_END
}} // namespace
