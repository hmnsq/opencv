// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


#अन्तर्भाव "precomp.hpp"

नामाकाश cv { नामाकाश hal {
CV_CPU_OPTIMIZATION_NAMESPACE_BEGIN

शून्य split8u(अचर uchar* src, uchar** dst, पूर्णाङ्क len, पूर्णाङ्क cn);
शून्य split16u(अचर ushort* src, ushort** dst, पूर्णाङ्क len, पूर्णाङ्क cn);
शून्य split32s(अचर पूर्णाङ्क* src, पूर्णाङ्क** dst, पूर्णाङ्क len, पूर्णाङ्क cn);
शून्य split64s(अचर int64* src, int64** dst, पूर्णाङ्क len, पूर्णाङ्क cn);

#ifndef CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

#यदि (CV_SIMD || CV_SIMD_SCALABLE)
// see the comments for vecmerge_ in merge.cpp
ढाँचा<typename T, typename VecT> स्थिर शून्य
vecsplit_( अचर T* src, T** dst, पूर्णाङ्क len, पूर्णाङ्क cn )
{
    अचर पूर्णाङ्क VECSZ = VTraits<VecT>::vlanes();
    पूर्णाङ्क i, i0 = 0;
    T* dst0 = dst[0];
    T* dst1 = dst[1];

    पूर्णाङ्क r0 = (पूर्णाङ्क)((माप_प्रकार)(शून्य*)dst0 % (VECSZ*परिमाण(T)));
    पूर्णाङ्क r1 = (पूर्णाङ्क)((माप_प्रकार)(शून्य*)dst1 % (VECSZ*परिमाण(T)));
    पूर्णाङ्क r2 = cn > 2 ? (पूर्णाङ्क)((माप_प्रकार)(शून्य*)dst[2] % (VECSZ*परिमाण(T))) : r0;
    पूर्णाङ्क r3 = cn > 3 ? (पूर्णाङ्क)((माप_प्रकार)(शून्य*)dst[3] % (VECSZ*परिमाण(T))) : r0;

    hal::StoreMode mode = hal::STORE_ALIGNED_NOCACHE;
    यदि( (r0|r1|r2|r3) != 0 )
    {
        mode = hal::STORE_UNALIGNED;
        यदि( r0 == r1 && r0 == r2 && r0 == r3 && r0 % परिमाण(T) == 0 && len > VECSZ*2 )
            i0 = VECSZ - (r0 / परिमाण(T));
    }

    यदि( cn == 2 )
    {
        लागि( i = 0; i < len; i += VECSZ )
        {
            यदि( i > len - VECSZ )
            {
                i = len - VECSZ;
                mode = hal::STORE_UNALIGNED;
            }
            VecT a, b;
            v_load_deinterleave(src + i*cn, a, b);
            v_store(dst0 + i, a, mode);
            v_store(dst1 + i, b, mode);
            यदि( i < i0 )
            {
                i = i0 - VECSZ;
                mode = hal::STORE_ALIGNED_NOCACHE;
            }
        }
    }
    अन्यथा यदि( cn == 3 )
    {
        T* dst2 = dst[2];
        लागि( i = 0; i < len; i += VECSZ )
        {
            यदि( i > len - VECSZ )
            {
                i = len - VECSZ;
                mode = hal::STORE_UNALIGNED;
            }
            VecT a, b, c;
            v_load_deinterleave(src + i*cn, a, b, c);
            v_store(dst0 + i, a, mode);
            v_store(dst1 + i, b, mode);
            v_store(dst2 + i, c, mode);
            यदि( i < i0 )
            {
                i = i0 - VECSZ;
                mode = hal::STORE_ALIGNED_NOCACHE;
            }
        }
    }
    अन्यथा
    {
        CV_Assert( cn == 4 );
        T* dst2 = dst[2];
        T* dst3 = dst[3];
        लागि( i = 0; i < len; i += VECSZ )
        {
            यदि( i > len - VECSZ )
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
            यदि( i < i0 )
            {
                i = i0 - VECSZ;
                mode = hal::STORE_ALIGNED_NOCACHE;
            }
        }
    }
    vx_cleanup();
}
#endif

ढाँचा<typename T> स्थिर शून्य
split_( अचर T* src, T** dst, पूर्णाङ्क len, पूर्णाङ्क cn )
{
    पूर्णाङ्क k = cn % 4 ? cn % 4 : 4;
    पूर्णाङ्क i, j;
    यदि( k == 1 )
    {
        T* dst0 = dst[0];

        यदि(cn == 1)
        {
            स_नकल(dst0, src, len * परिमाण(T));
        }
        अन्यथा
        {
            लागि( i = 0, j = 0 ; i < len; i++, j += cn )
                dst0[i] = src[j];
        }
    }
    अन्यथा यदि( k == 2 )
    {
        T *dst0 = dst[0], *dst1 = dst[1];
        i = j = 0;

        लागि( ; i < len; i++, j += cn )
        {
            dst0[i] = src[j];
            dst1[i] = src[j+1];
        }
    }
    अन्यथा यदि( k == 3 )
    {
        T *dst0 = dst[0], *dst1 = dst[1], *dst2 = dst[2];
        i = j = 0;

        लागि( ; i < len; i++, j += cn )
        {
            dst0[i] = src[j];
            dst1[i] = src[j+1];
            dst2[i] = src[j+2];
        }
    }
    अन्यथा
    {
        T *dst0 = dst[0], *dst1 = dst[1], *dst2 = dst[2], *dst3 = dst[3];
        i = j = 0;

        लागि( ; i < len; i++, j += cn )
        {
            dst0[i] = src[j]; dst1[i] = src[j+1];
            dst2[i] = src[j+2]; dst3[i] = src[j+3];
        }
    }

    लागि( ; k < cn; k += 4 )
    {
        T *dst0 = dst[k], *dst1 = dst[k+1], *dst2 = dst[k+2], *dst3 = dst[k+3];
        लागि( i = 0, j = k; i < len; i++, j += cn )
        {
            dst0[i] = src[j]; dst1[i] = src[j+1];
            dst2[i] = src[j+2]; dst3[i] = src[j+3];
        }
    }
}

शून्य split8u(अचर uchar* src, uchar** dst, पूर्णाङ्क len, पूर्णाङ्क cn )
{
    CV_INSTRUMENT_REGION();
#यदि (CV_SIMD || CV_SIMD_SCALABLE)
    यदि( len >= VTraits<v_uint8>::vlanes() && 2 <= cn && cn <= 4 )
        vecsplit_<uchar, v_uint8>(src, dst, len, cn);
    अन्यथा
#endif
        split_(src, dst, len, cn);
}

शून्य split16u(अचर ushort* src, ushort** dst, पूर्णाङ्क len, पूर्णाङ्क cn )
{
    CV_INSTRUMENT_REGION();
#यदि (CV_SIMD || CV_SIMD_SCALABLE)
    यदि( len >= VTraits<v_uint16>::vlanes() && 2 <= cn && cn <= 4 )
        vecsplit_<ushort, v_uint16>(src, dst, len, cn);
    अन्यथा
#endif
        split_(src, dst, len, cn);
}

शून्य split32s(अचर पूर्णाङ्क* src, पूर्णाङ्क** dst, पूर्णाङ्क len, पूर्णाङ्क cn )
{
    CV_INSTRUMENT_REGION();
#यदि (CV_SIMD || CV_SIMD_SCALABLE)
    यदि( len >= VTraits<v_uint32>::vlanes() && 2 <= cn && cn <= 4 )
        vecsplit_<पूर्णाङ्क, v_int32>(src, dst, len, cn);
    अन्यथा
#endif
        split_(src, dst, len, cn);
}

शून्य split64s(अचर int64* src, int64** dst, पूर्णाङ्क len, पूर्णाङ्क cn )
{
    CV_INSTRUMENT_REGION();
#यदि (CV_SIMD || CV_SIMD_SCALABLE)
    यदि( len >= VTraits<v_int64>::vlanes() && 2 <= cn && cn <= 4 )
        vecsplit_<int64, v_int64>(src, dst, len, cn);
    अन्यथा
#endif
        split_(src, dst, len, cn);
}

#endif
CV_CPU_OPTIMIZATION_NAMESPACE_END
}} // namespace
