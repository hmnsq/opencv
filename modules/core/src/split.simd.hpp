// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


#অন্তর্ভাব "precomp.hpp"

নামাকাশ cv { নামাকাশ hal {
CV_CPU_OPTIMIZATION_NAMESPACE_BEGIN

শূন্য split8u(ধ্রুবক uchar* src, uchar** dst, পূর্ণসংখ্যা len, পূর্ণসংখ্যা cn);
শূন্য split16u(ধ্রুবক ushort* src, ushort** dst, পূর্ণসংখ্যা len, পূর্ণসংখ্যা cn);
শূন্য split32s(ধ্রুবক পূর্ণসংখ্যা* src, পূর্ণসংখ্যা** dst, পূর্ণসংখ্যা len, পূর্ণসংখ্যা cn);
শূন্য split64s(ধ্রুবক int64* src, int64** dst, পূর্ণসংখ্যা len, পূর্ণসংখ্যা cn);

#ifndef CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

#যদি (CV_SIMD || CV_SIMD_SCALABLE)
// see the comments for vecmerge_ in merge.cpp
template<typename T, typename VecT> স্থির শূন্য
vecsplit_( ধ্রুবক T* src, T** dst, পূর্ণসংখ্যা len, পূর্ণসংখ্যা cn )
{
    ধ্রুবক পূর্ণসংখ্যা VECSZ = VTraits<VecT>::vlanes();
    পূর্ণসংখ্যা i, i0 = 0;
    T* dst0 = dst[0];
    T* dst1 = dst[1];

    পূর্ণসংখ্যা r0 = (পূর্ণসংখ্যা)((size_t)(শূন্য*)dst0 % (VECSZ*আকার(T)));
    পূর্ণসংখ্যা r1 = (পূর্ণসংখ্যা)((size_t)(শূন্য*)dst1 % (VECSZ*আকার(T)));
    পূর্ণসংখ্যা r2 = cn > 2 ? (পূর্ণসংখ্যা)((size_t)(শূন্য*)dst[2] % (VECSZ*আকার(T))) : r0;
    পূর্ণসংখ্যা r3 = cn > 3 ? (পূর্ণসংখ্যা)((size_t)(শূন্য*)dst[3] % (VECSZ*আকার(T))) : r0;

    hal::StoreMode mode = hal::STORE_ALIGNED_NOCACHE;
    যদি( (r0|r1|r2|r3) != 0 )
    {
        mode = hal::STORE_UNALIGNED;
        যদি( r0 == r1 && r0 == r2 && r0 == r3 && r0 % আকার(T) == 0 && len > VECSZ*2 )
            i0 = VECSZ - (r0 / আকার(T));
    }

    যদি( cn == 2 )
    {
        জন্য( i = 0; i < len; i += VECSZ )
        {
            যদি( i > len - VECSZ )
            {
                i = len - VECSZ;
                mode = hal::STORE_UNALIGNED;
            }
            VecT a, b;
            v_load_deinterleave(src + i*cn, a, b);
            v_store(dst0 + i, a, mode);
            v_store(dst1 + i, b, mode);
            যদি( i < i0 )
            {
                i = i0 - VECSZ;
                mode = hal::STORE_ALIGNED_NOCACHE;
            }
        }
    }
    অন্যথা যদি( cn == 3 )
    {
        T* dst2 = dst[2];
        জন্য( i = 0; i < len; i += VECSZ )
        {
            যদি( i > len - VECSZ )
            {
                i = len - VECSZ;
                mode = hal::STORE_UNALIGNED;
            }
            VecT a, b, c;
            v_load_deinterleave(src + i*cn, a, b, c);
            v_store(dst0 + i, a, mode);
            v_store(dst1 + i, b, mode);
            v_store(dst2 + i, c, mode);
            যদি( i < i0 )
            {
                i = i0 - VECSZ;
                mode = hal::STORE_ALIGNED_NOCACHE;
            }
        }
    }
    অন্যথা
    {
        CV_Assert( cn == 4 );
        T* dst2 = dst[2];
        T* dst3 = dst[3];
        জন্য( i = 0; i < len; i += VECSZ )
        {
            যদি( i > len - VECSZ )
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
            যদি( i < i0 )
            {
                i = i0 - VECSZ;
                mode = hal::STORE_ALIGNED_NOCACHE;
            }
        }
    }
    vx_cleanup();
}
#endif

template<typename T> স্থির শূন্য
split_( ধ্রুবক T* src, T** dst, পূর্ণসংখ্যা len, পূর্ণসংখ্যা cn )
{
    পূর্ণসংখ্যা k = cn % 4 ? cn % 4 : 4;
    পূর্ণসংখ্যা i, j;
    যদি( k == 1 )
    {
        T* dst0 = dst[0];

        যদি(cn == 1)
        {
            memcpy(dst0, src, len * আকার(T));
        }
        অন্যথা
        {
            জন্য( i = 0, j = 0 ; i < len; i++, j += cn )
                dst0[i] = src[j];
        }
    }
    অন্যথা যদি( k == 2 )
    {
        T *dst0 = dst[0], *dst1 = dst[1];
        i = j = 0;

        জন্য( ; i < len; i++, j += cn )
        {
            dst0[i] = src[j];
            dst1[i] = src[j+1];
        }
    }
    অন্যথা যদি( k == 3 )
    {
        T *dst0 = dst[0], *dst1 = dst[1], *dst2 = dst[2];
        i = j = 0;

        জন্য( ; i < len; i++, j += cn )
        {
            dst0[i] = src[j];
            dst1[i] = src[j+1];
            dst2[i] = src[j+2];
        }
    }
    অন্যথা
    {
        T *dst0 = dst[0], *dst1 = dst[1], *dst2 = dst[2], *dst3 = dst[3];
        i = j = 0;

        জন্য( ; i < len; i++, j += cn )
        {
            dst0[i] = src[j]; dst1[i] = src[j+1];
            dst2[i] = src[j+2]; dst3[i] = src[j+3];
        }
    }

    জন্য( ; k < cn; k += 4 )
    {
        T *dst0 = dst[k], *dst1 = dst[k+1], *dst2 = dst[k+2], *dst3 = dst[k+3];
        জন্য( i = 0, j = k; i < len; i++, j += cn )
        {
            dst0[i] = src[j]; dst1[i] = src[j+1];
            dst2[i] = src[j+2]; dst3[i] = src[j+3];
        }
    }
}

শূন্য split8u(ধ্রুবক uchar* src, uchar** dst, পূর্ণসংখ্যা len, পূর্ণসংখ্যা cn )
{
    CV_INSTRUMENT_REGION();
#যদি (CV_SIMD || CV_SIMD_SCALABLE)
    যদি( len >= VTraits<v_uint8>::vlanes() && 2 <= cn && cn <= 4 )
        vecsplit_<uchar, v_uint8>(src, dst, len, cn);
    অন্যথা
#endif
        split_(src, dst, len, cn);
}

শূন্য split16u(ধ্রুবক ushort* src, ushort** dst, পূর্ণসংখ্যা len, পূর্ণসংখ্যা cn )
{
    CV_INSTRUMENT_REGION();
#যদি (CV_SIMD || CV_SIMD_SCALABLE)
    যদি( len >= VTraits<v_uint16>::vlanes() && 2 <= cn && cn <= 4 )
        vecsplit_<ushort, v_uint16>(src, dst, len, cn);
    অন্যথা
#endif
        split_(src, dst, len, cn);
}

শূন্য split32s(ধ্রুবক পূর্ণসংখ্যা* src, পূর্ণসংখ্যা** dst, পূর্ণসংখ্যা len, পূর্ণসংখ্যা cn )
{
    CV_INSTRUMENT_REGION();
#যদি (CV_SIMD || CV_SIMD_SCALABLE)
    যদি( len >= VTraits<v_uint32>::vlanes() && 2 <= cn && cn <= 4 )
        vecsplit_<পূর্ণসংখ্যা, v_int32>(src, dst, len, cn);
    অন্যথা
#endif
        split_(src, dst, len, cn);
}

শূন্য split64s(ধ্রুবক int64* src, int64** dst, পূর্ণসংখ্যা len, পূর্ণসংখ্যা cn )
{
    CV_INSTRUMENT_REGION();
#যদি (CV_SIMD || CV_SIMD_SCALABLE)
    যদি( len >= VTraits<v_int64>::vlanes() && 2 <= cn && cn <= 4 )
        vecsplit_<int64, v_int64>(src, dst, len, cn);
    অন্যথা
#endif
        split_(src, dst, len, cn);
}

#endif
CV_CPU_OPTIMIZATION_NAMESPACE_END
}} // namespace
