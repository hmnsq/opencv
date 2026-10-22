// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


#समावेश "precomp.hpp"

namespace cv { namespace hal {
CV_CPU_OPTIMIZATION_NAMESPACE_BEGIN

व्योम split8u(स्थिर uchar* src, uchar** dst, पूर्णांक len, पूर्णांक cn);
व्योम split16u(स्थिर ushort* src, ushort** dst, पूर्णांक len, पूर्णांक cn);
व्योम split32s(स्थिर पूर्णांक* src, पूर्णांक** dst, पूर्णांक len, पूर्णांक cn);
व्योम split64s(स्थिर int64* src, int64** dst, पूर्णांक len, पूर्णांक cn);

#अगर_अघोषित CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

#अगर (CV_SIMD || CV_SIMD_SCALABLE)
// see the comments for vecmerge_ in merge.cpp
ढाँचा<typename T, typename VecT> अटल व्योम
vecsplit_( स्थिर T* src, T** dst, पूर्णांक len, पूर्णांक cn )
{
    स्थिर पूर्णांक VECSZ = VTraits<VecT>::vlanes();
    पूर्णांक i, i0 = 0;
    T* dst0 = dst[0];
    T* dst1 = dst[1];

    पूर्णांक r0 = (पूर्णांक)((माप_प्रकार)(व्योम*)dst0 % (VECSZ*माप(T)));
    पूर्णांक r1 = (पूर्णांक)((माप_प्रकार)(व्योम*)dst1 % (VECSZ*माप(T)));
    पूर्णांक r2 = cn > 2 ? (पूर्णांक)((माप_प्रकार)(व्योम*)dst[2] % (VECSZ*माप(T))) : r0;
    पूर्णांक r3 = cn > 3 ? (पूर्णांक)((माप_प्रकार)(व्योम*)dst[3] % (VECSZ*माप(T))) : r0;

    hal::StoreMode mode = hal::STORE_ALIGNED_NOCACHE;
    अगर( (r0|r1|r2|r3) != 0 )
    {
        mode = hal::STORE_UNALIGNED;
        अगर( r0 == r1 && r0 == r2 && r0 == r3 && r0 % माप(T) == 0 && len > VECSZ*2 )
            i0 = VECSZ - (r0 / माप(T));
    }

    अगर( cn == 2 )
    {
        क्रम( i = 0; i < len; i += VECSZ )
        {
            अगर( i > len - VECSZ )
            {
                i = len - VECSZ;
                mode = hal::STORE_UNALIGNED;
            }
            VecT a, b;
            v_load_deinterleave(src + i*cn, a, b);
            v_store(dst0 + i, a, mode);
            v_store(dst1 + i, b, mode);
            अगर( i < i0 )
            {
                i = i0 - VECSZ;
                mode = hal::STORE_ALIGNED_NOCACHE;
            }
        }
    }
    अन्यथा अगर( cn == 3 )
    {
        T* dst2 = dst[2];
        क्रम( i = 0; i < len; i += VECSZ )
        {
            अगर( i > len - VECSZ )
            {
                i = len - VECSZ;
                mode = hal::STORE_UNALIGNED;
            }
            VecT a, b, c;
            v_load_deinterleave(src + i*cn, a, b, c);
            v_store(dst0 + i, a, mode);
            v_store(dst1 + i, b, mode);
            v_store(dst2 + i, c, mode);
            अगर( i < i0 )
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
        क्रम( i = 0; i < len; i += VECSZ )
        {
            अगर( i > len - VECSZ )
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
            अगर( i < i0 )
            {
                i = i0 - VECSZ;
                mode = hal::STORE_ALIGNED_NOCACHE;
            }
        }
    }
    vx_cleanup();
}
#पूर्ण_अगर

ढाँचा<typename T> अटल व्योम
split_( स्थिर T* src, T** dst, पूर्णांक len, पूर्णांक cn )
{
    पूर्णांक k = cn % 4 ? cn % 4 : 4;
    पूर्णांक i, j;
    अगर( k == 1 )
    {
        T* dst0 = dst[0];

        अगर(cn == 1)
        {
            स_नकल(dst0, src, len * माप(T));
        }
        अन्यथा
        {
            क्रम( i = 0, j = 0 ; i < len; i++, j += cn )
                dst0[i] = src[j];
        }
    }
    अन्यथा अगर( k == 2 )
    {
        T *dst0 = dst[0], *dst1 = dst[1];
        i = j = 0;

        क्रम( ; i < len; i++, j += cn )
        {
            dst0[i] = src[j];
            dst1[i] = src[j+1];
        }
    }
    अन्यथा अगर( k == 3 )
    {
        T *dst0 = dst[0], *dst1 = dst[1], *dst2 = dst[2];
        i = j = 0;

        क्रम( ; i < len; i++, j += cn )
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

        क्रम( ; i < len; i++, j += cn )
        {
            dst0[i] = src[j]; dst1[i] = src[j+1];
            dst2[i] = src[j+2]; dst3[i] = src[j+3];
        }
    }

    क्रम( ; k < cn; k += 4 )
    {
        T *dst0 = dst[k], *dst1 = dst[k+1], *dst2 = dst[k+2], *dst3 = dst[k+3];
        क्रम( i = 0, j = k; i < len; i++, j += cn )
        {
            dst0[i] = src[j]; dst1[i] = src[j+1];
            dst2[i] = src[j+2]; dst3[i] = src[j+3];
        }
    }
}

व्योम split8u(स्थिर uchar* src, uchar** dst, पूर्णांक len, पूर्णांक cn )
{
    CV_INSTRUMENT_REGION();
#अगर (CV_SIMD || CV_SIMD_SCALABLE)
    अगर( len >= VTraits<v_uint8>::vlanes() && 2 <= cn && cn <= 4 )
        vecsplit_<uchar, v_uint8>(src, dst, len, cn);
    अन्यथा
#पूर्ण_अगर
        split_(src, dst, len, cn);
}

व्योम split16u(स्थिर ushort* src, ushort** dst, पूर्णांक len, पूर्णांक cn )
{
    CV_INSTRUMENT_REGION();
#अगर (CV_SIMD || CV_SIMD_SCALABLE)
    अगर( len >= VTraits<v_uint16>::vlanes() && 2 <= cn && cn <= 4 )
        vecsplit_<ushort, v_uint16>(src, dst, len, cn);
    अन्यथा
#पूर्ण_अगर
        split_(src, dst, len, cn);
}

व्योम split32s(स्थिर पूर्णांक* src, पूर्णांक** dst, पूर्णांक len, पूर्णांक cn )
{
    CV_INSTRUMENT_REGION();
#अगर (CV_SIMD || CV_SIMD_SCALABLE)
    अगर( len >= VTraits<v_uint32>::vlanes() && 2 <= cn && cn <= 4 )
        vecsplit_<पूर्णांक, v_int32>(src, dst, len, cn);
    अन्यथा
#पूर्ण_अगर
        split_(src, dst, len, cn);
}

व्योम split64s(स्थिर int64* src, int64** dst, पूर्णांक len, पूर्णांक cn )
{
    CV_INSTRUMENT_REGION();
#अगर (CV_SIMD || CV_SIMD_SCALABLE)
    अगर( len >= VTraits<v_int64>::vlanes() && 2 <= cn && cn <= 4 )
        vecsplit_<int64, v_int64>(src, dst, len, cn);
    अन्यथा
#पूर्ण_अगर
        split_(src, dst, len, cn);
}

#पूर्ण_अगर
CV_CPU_OPTIMIZATION_NAMESPACE_END
}} // namespace
