// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html

#समावेश "precomp.hpp"

namespace cv {

प्रकार व्योम (*MinMaxIdxFunc)(स्थिर uchar* तथ्य, स्थिर uchar* mask,
                              व्योम* minval, व्योम* maxval,
                              माप_प्रकार* minidx, माप_प्रकार* maxidx,
                              पूर्णांक len, माप_प्रकार startidx);

CV_CPU_OPTIMIZATION_NAMESPACE_BEGIN

ढाँचा<typename T, typename WT, व्योम (*fn)(स्थिर T*, स्थिर uchar*, WT*, WT*, माप_प्रकार*, माप_प्रकार*, पूर्णांक, माप_प्रकार)>
अटल व्योम minMaxIdxWrap(स्थिर uchar* तथ्य, स्थिर uchar* mask,
                          व्योम* minval, व्योम* maxval,
                          माप_प्रकार* minidx, माप_प्रकार* maxidx,
                          पूर्णांक len, माप_प्रकार startidx)
{
    fn((स्थिर T*)तथ्य, mask, (WT*)minval, (WT*)maxval, minidx, maxidx, len, startidx);
}

MinMaxIdxFunc getMinMaxIdxFunc(पूर्णांक depth);

#अगर_अघोषित CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

ढाँचा<typename T, typename WT> अटल व्योम
minMaxIdx_( स्थिर T* src, स्थिर uchar* mask, WT* _minVal, WT* _maxVal,
            माप_प्रकार* _minIdx, माप_प्रकार* _maxIdx, पूर्णांक len, माप_प्रकार startIdx )
{
    WT minVal = *_minVal, maxVal = *_maxVal;
    माप_प्रकार minIdx = *_minIdx, maxIdx = *_maxIdx;
    पूर्णांक i = 0;

    अगर (minIdx == 0 || maxIdx == 0) {
        अगर (mask) {
            क्रम (; i < len; i++) {
                अगर (mask[i]) {
                    minVal = maxVal = (WT)src[i];
                    minIdx = maxIdx = startIdx + i;
                    i++;
                    अवरोध;
                }
            }
        }
        अन्यथा अगर (len > 0) {
            minVal = maxVal = (WT)src[0];
            minIdx = maxIdx = startIdx;
            i++;
        }
    }

    अगर( !mask )
    {
        क्रम( ; i < len; i++ )
        {
            WT val = (WT)src[i];
            अगर( val < minVal )
            {
                minVal = val;
                minIdx = startIdx + i;
            }
            अगर( val > maxVal )
            {
                maxVal = val;
                maxIdx = startIdx + i;
            }
        }
    }
    अन्यथा
    {
        क्रम( ; i < len; i++ )
        {
            WT val = (WT)src[i];
            uchar m = mask[i];
            अगर( m && val < minVal )
            {
                minVal = val;
                minIdx = startIdx + i;
            }
            अगर( m && val > maxVal )
            {
                maxVal = val;
                maxIdx = startIdx + i;
            }
        }
    }

    *_minIdx = minIdx;
    *_maxIdx = maxIdx;
    *_minVal = minVal;
    *_maxVal = maxVal;
}

#अघोषित SIMD_ONLY
#अगर (CV_SIMD || CV_SIMD_SCALABLE)
#घोषणा SIMD_ONLY(व्यंजक) व्यंजक
#अन्यथा
#घोषणा SIMD_ONLY(व्यंजक)
#पूर्ण_अगर

अटल पूर्णांक minMaxInit(स्थिर uchar* mask, पूर्णांक len)
{
    पूर्णांक i = 0;
    SIMD_ONLY(
    पूर्णांक vlanes = VTraits<v_uint8>::vlanes();
    v_uint8 v_zero = vx_setzero_u8();
    क्रम (; i < len; i += vlanes) {
        अगर (i + vlanes > len) {
            अगर (i == 0)
                अवरोध;
            i = len - vlanes;
        }
        v_uint8 mask_i = v_ne(vx_load(mask + i), v_zero);
        अगर (v_check_any(mask_i))
            वापस i + v_scan_forward(mask_i);
    })
    क्रम (; i < len; i++) {
        अगर (mask[i] != 0)
            वापस i;
    }
    वापस -1;
}

// vectorized implementation for u8, s8, u16 and s16
// uses blocks to decrease the lane size necessary to store indices
#अघोषित DEFINE_MINMAXIDX_SMALLINT_FUNC
#घोषणा DEFINE_MINMAXIDX_SMALLINT_FUNC(funcname, suffix, usuffix, T, UT, VT, UVT, WT, BLOCK_आकार, load_mask) \
अटल व्योम funcname(स्थिर T* src, स्थिर uchar* mask, WT* _minVal, WT* _maxVal, \
                     माप_प्रकार* _minIdx, माप_प्रकार* _maxIdx, पूर्णांक len, माप_प्रकार startIdx) \
{ \
    T minVal = T(*_minVal), maxVal = T(*_maxVal); \
    माप_प्रकार minIdx = *_minIdx, maxIdx = *_maxIdx; \
    पूर्णांक i = 0; \
    /* initialize minVal/maxVal/minIdx/maxIdx to the proper values in the beginning */ \
    अगर (minIdx == 0) { \
        अगर (mask) { \
            i = minMaxInit(mask, len); \
            अगर (i < 0) \
                वापस; \
        } \
        minVal = maxVal = src[i]; \
        minIdx = maxIdx = startIdx + i; \
        i++; \
    } \
    SIMD_ONLY( \
    स्थिर पूर्णांक vlanes = VTraits<VT>::vlanes(); \
    स्थिर पूर्णांक block_size0 = BLOCK_आकार - vlanes; \
    अगर (len-i >= vlanes && block_size0 > 0 && block_size0 % vlanes == 0) { \
        UT idxbuf[VTraits<UVT>::max_nlanes]; \
        क्रम (पूर्णांक j = 0; j < vlanes; j++) \
            idxbuf[j] = (UT)j; \
        UVT v_idx0 = vx_load(idxbuf); \
        UVT v_idx_delta = vx_setall_##usuffix((UT)vlanes); \
        UVT v_invalid_idx = vx_setall_##usuffix((UT)-1); \
        VT v_minval = vx_setall_##suffix(minVal); \
        VT v_maxval = vx_setall_##suffix(maxVal); \
        पूर्णांक block_आकार_2 = block_size0; \
        /* process data by blocks: */ \
        /* - for u8/s8 data each block contains up to 256-vlanes elements */ \
        /* - for u16/s16 data each block contains up to 65536-vlanes elements */ \
        /* inside each block we can store the relative (local) index (v_locidx) */ \
        /* in a compact way: 8 bits per lane for u8/s8 data, */ \
        /*                  16 bits per lane for u16/s16 data */ \
        /* 0b111...111 is "invalid index", meaning that this */ \
        /* particular lane has not been updated. */ \
        /* after each block we update minVal, maxVal, minIdx and maxIdx */ \
        क्रम (; i <= len - vlanes; i += block_आकार_2) { \
            block_आकार_2 = std::min(block_आकार_2, (len - i) & -vlanes); \
            UVT v_locidx = v_idx0; \
            UVT v_minidx = v_invalid_idx; \
            UVT v_maxidx = v_invalid_idx; \
            अगर (!mask) { \
                क्रम (पूर्णांक j = 0; j < block_आकार_2; j += vlanes) { \
                    VT तथ्य = vx_load(src + i + j); \
                    UVT lt_min = v_reinterpret_as_##usuffix(v_lt(तथ्य, v_minval)); \
                    UVT gt_max = v_reinterpret_as_##usuffix(v_gt(तथ्य, v_maxval)); \
                    v_minidx = v_select(lt_min, v_locidx, v_minidx); \
                    v_maxidx = v_select(gt_max, v_locidx, v_maxidx); \
                    v_minval = v_min(v_minval, तथ्य); \
                    v_maxval = v_max(v_maxval, तथ्य); \
                    v_locidx = v_add(v_locidx, v_idx_delta); \
                } \
            } अन्यथा { \
                UVT v_zero = vx_setzero_##usuffix(); \
                क्रम (पूर्णांक j = 0; j < block_आकार_2; j += vlanes) { \
                    VT तथ्य = vx_load(src + i + j); \
                    UVT msk = v_ne(load_mask(mask + i + j), v_zero); \
                    UVT lt_min = v_reinterpret_as_##usuffix(v_lt(तथ्य, v_minval)); \
                    UVT gt_max = v_reinterpret_as_##usuffix(v_gt(तथ्य, v_maxval)); \
                    lt_min = v_and(lt_min, msk); \
                    gt_max = v_and(gt_max, msk); \
                    v_minidx = v_select(lt_min, v_locidx, v_minidx); \
                    v_maxidx = v_select(gt_max, v_locidx, v_maxidx); \
                    VT lt_min_तथ्य = v_reinterpret_as_##suffix(lt_min); \
                    VT gt_max_तथ्य = v_reinterpret_as_##suffix(gt_max); \
                    v_minval = v_select(lt_min_तथ्य, तथ्य, v_minval); \
                    v_maxval = v_select(gt_max_तथ्य, तथ्य, v_maxval); \
                    v_locidx = v_add(v_locidx, v_idx_delta); \
                } \
            } \
            /* for both minimum and maximum we check whether global extremum */ \
            /* and its index need to be updated. If yes, we compute */ \
            /* the smallest index within the block where the new global */ \
            /* extremum value occurs */ \
            UVT idxmask = v_ne(v_minidx, v_invalid_idx); \
            अगर (v_check_any(idxmask)) { \
                minVal = (T)v_reduce_min(v_minval); \
                VT invmask = v_ne(v_minval, vx_setall_##suffix(minVal)); \
                v_minidx = v_or(v_minidx, v_reinterpret_as_##usuffix(invmask)); \
                minIdx = startIdx + i + v_reduce_min(v_minidx); \
                v_minval = vx_setall_##suffix(minVal); \
            } \
            idxmask = v_ne(v_maxidx, v_invalid_idx); \
            अगर (v_check_any(idxmask)) { \
                maxVal = (T)v_reduce_max(v_maxval); \
                VT invmask = v_ne(v_maxval, vx_setall_##suffix(maxVal)); \
                v_maxidx = v_or(v_maxidx, v_reinterpret_as_##usuffix(invmask)); \
                maxIdx = startIdx + i + v_reduce_min(v_maxidx); \
                v_maxval = vx_setall_##suffix(maxVal); \
            } \
        } \
    }) \
    *_minVal = (WT)minVal; \
    *_maxVal = (WT)maxVal; \
    *_minIdx = minIdx; \
    *_maxIdx = maxIdx; \
    /* [TODO]: unlike sum, countNonZero and other reduce operations, */ \
    /* in the case of minMaxIdx we can process the tail using */ \
    /* vector overlapping technique (as in arithmetic operations) */ \
    अगर (i < len) { \
        src += i; \
        अगर (mask) mask += i; \
        startIdx += i; \
        len -= i; \
        minMaxIdx_(src, mask, _minVal, _maxVal, _minIdx, _maxIdx, len, startIdx); \
    } \
}

// vectorized implementation for s32, f32, f16 and bf16
// (potentially can be extended for u32)
// no need to use blocks here
#अघोषित DEFINE_MINMAXIDX_FUNC
#घोषणा DEFINE_MINMAXIDX_FUNC(funcname, suffix, usuffix, T, UT, VT, UVT, WT, load_op) \
अटल व्योम funcname(स्थिर T* src, स्थिर uchar* mask, WT* _minVal, WT* _maxVal, \
                     माप_प्रकार* _minIdx, माप_प्रकार* _maxIdx, पूर्णांक len, माप_प्रकार startIdx) \
{ \
    WT minVal = *_minVal, maxVal = *_maxVal; \
    माप_प्रकार minIdx = *_minIdx, maxIdx = *_maxIdx; \
    पूर्णांक i = 0; \
    /* initialize minVal/maxVal/minIdx/maxIdx to the proper values in the beginning */ \
    अगर (minIdx == 0) { \
        अगर (mask) { \
            i = minMaxInit(mask, len); \
            अगर (i < 0) \
                वापस; \
        } \
        minVal = maxVal = src[i]; \
        minIdx = maxIdx = startIdx + i; \
        i++; \
    } \
    SIMD_ONLY( \
    स्थिर पूर्णांक vlanes = VTraits<VT>::vlanes(); \
    UT idxbuf[VTraits<UVT>::max_nlanes]; \
    क्रम (पूर्णांक j = 0; j < vlanes; j++) \
        idxbuf[j] = (UT)(i+j); \
    UVT v_locidx = vx_load(idxbuf); \
    UVT v_idx_delta = vx_setall_##usuffix((UT)vlanes); \
    UVT v_invalid_idx = vx_setall_##usuffix((UT)-1); \
    VT v_minval = vx_setall_##suffix(minVal); \
    VT v_maxval = vx_setall_##suffix(maxVal); \
    UVT v_minidx = v_invalid_idx; \
    UVT v_maxidx = v_invalid_idx; \
    /* process data by blocks: */ \
    /* - for u8/s8 data each block contains up to 256-vlanes elements */ \
    /* - for u16/s16 data each block contains up to 65536-vlanes elements */ \
    /* inside each block we can store the relative (local) index (v_locidx) */ \
    /* in a compact way: 8 bits per lane for u8/s8 data, */ \
    /*                  16 bits per lane for u16/s16 data */ \
    /* 0b111...111 is "invalid index", meaning that this */ \
    /* particular lane has not been updated. */ \
    /* after each block we update minVal, maxVal, minIdx and maxIdx */ \
    अगर (!mask) { \
        क्रम (; i <= len - vlanes; i += vlanes) { \
            VT तथ्य = load_op(src + i); \
            UVT lt_min = v_reinterpret_as_##usuffix(v_lt(तथ्य, v_minval)); \
            UVT gt_max = v_reinterpret_as_##usuffix(v_gt(तथ्य, v_maxval)); \
            v_minidx = v_select(lt_min, v_locidx, v_minidx); \
            v_maxidx = v_select(gt_max, v_locidx, v_maxidx); \
            v_minval = v_min(v_minval, तथ्य); \
            v_maxval = v_max(v_maxval, तथ्य); \
            v_locidx = v_add(v_locidx, v_idx_delta); \
        } \
    } अन्यथा { \
        UVT v_zero = vx_setzero_##usuffix(); \
        क्रम (; i <= len - vlanes; i += vlanes) { \
            VT तथ्य = load_op(src + i); \
            UVT msk = v_ne(vx_load_expand_q(mask + i), v_zero); \
            UVT lt_min = v_reinterpret_as_##usuffix(v_lt(तथ्य, v_minval)); \
            UVT gt_max = v_reinterpret_as_##usuffix(v_gt(तथ्य, v_maxval)); \
            lt_min = v_and(lt_min, msk); \
            gt_max = v_and(gt_max, msk); \
            v_minidx = v_select(lt_min, v_locidx, v_minidx); \
            v_maxidx = v_select(gt_max, v_locidx, v_maxidx); \
            VT lt_min_तथ्य = v_reinterpret_as_##suffix(lt_min); \
            VT gt_max_तथ्य = v_reinterpret_as_##suffix(gt_max); \
            v_minval = v_select(lt_min_तथ्य, तथ्य, v_minval); \
            v_maxval = v_select(gt_max_तथ्य, तथ्य, v_maxval); \
            v_locidx = v_add(v_locidx, v_idx_delta); \
        } \
    } \
    /* for both minimum and maximum we check whether global extremum */ \
    /* and its index need to be updated. If yes, we compute */ \
    /* the smallest index within the block where the new global */ \
    /* extremum value occurs */ \
    UVT idxmask = v_ne(v_minidx, v_invalid_idx); \
    अगर (v_check_any(idxmask)) { \
        minVal = v_reduce_min(v_minval); \
        VT invmask = v_ne(v_minval, vx_setall_##suffix(minVal)); \
        v_minidx = v_or(v_minidx, v_reinterpret_as_##usuffix(invmask)); \
        minIdx = startIdx + v_reduce_min(v_minidx); \
        v_minval = vx_setall_##suffix(minVal); \
    } \
    idxmask = v_ne(v_maxidx, v_invalid_idx); \
    अगर (v_check_any(idxmask)) { \
        maxVal = v_reduce_max(v_maxval); \
        VT invmask = v_ne(v_maxval, vx_setall_##suffix(maxVal)); \
        v_maxidx = v_or(v_maxidx, v_reinterpret_as_##usuffix(invmask)); \
        maxIdx = startIdx + v_reduce_min(v_maxidx); \
        v_maxval = vx_setall_##suffix(maxVal); \
    }) \
    *_minVal = minVal; \
    *_maxVal = maxVal; \
    *_minIdx = minIdx; \
    *_maxIdx = maxIdx; \
    /* [TODO]: unlike sum, countNonZero and other reduce operations, */ \
    /* in the case of minMaxIdx we can process the tail using */ \
    /* vector overlapping technique (as in arithmetic operations) */ \
    अगर (i < len) { \
        src += i; \
        अगर (mask) mask += i; \
        startIdx += i; \
        len -= i; \
        minMaxIdx_(src, mask, _minVal, _maxVal, _minIdx, _maxIdx, len, startIdx); \
    } \
}

#अघोषित DEFINE_MINMAXIDX_FUNC_NOSIMD
#घोषणा DEFINE_MINMAXIDX_FUNC_NOSIMD(funcname, T, WT) \
अटल व्योम funcname(स्थिर T* src, स्थिर uchar* mask, WT* _minVal, WT* _maxVal, \
                     माप_प्रकार* _minIdx, माप_प्रकार* _maxIdx, पूर्णांक len, माप_प्रकार startIdx) \
{ \
    minMaxIdx_(src, mask, _minVal, _maxVal, _minIdx, _maxIdx, len, startIdx); \
}

DEFINE_MINMAXIDX_SMALLINT_FUNC(minMaxIdx8u, u8, u8, uchar, uchar, v_uint8, v_uint8, पूर्णांक, 256, vx_load)
DEFINE_MINMAXIDX_SMALLINT_FUNC(minMaxIdx8s, s8, u8, schar, uchar, v_int8, v_uint8, पूर्णांक, 256, vx_load)
DEFINE_MINMAXIDX_SMALLINT_FUNC(minMaxIdx16u, u16, u16, ushort, ushort, v_uint16, v_uint16, पूर्णांक, 65536, vx_load_expand)
DEFINE_MINMAXIDX_SMALLINT_FUNC(minMaxIdx16s, s16, u16, लघु, ushort, v_int16, v_uint16, पूर्णांक, 65536, vx_load_expand)

DEFINE_MINMAXIDX_FUNC(minMaxIdx32s, s32, u32, पूर्णांक, अचिन्हित, v_int32, v_uint32, पूर्णांक, vx_load)
DEFINE_MINMAXIDX_FUNC(minMaxIdx32f, f32, u32, भग्न, अचिन्हित, v_float32, v_uint32, भग्न, vx_load)
DEFINE_MINMAXIDX_FUNC(minMaxIdx16f, f32, u32, hfloat, अचिन्हित, v_float32, v_uint32, भग्न, vx_load_expand)
DEFINE_MINMAXIDX_FUNC(minMaxIdx16bf, f32, u32, bfloat, अचिन्हित, v_float32, v_uint32, भग्न, vx_load_expand)

//DEFINE_MINMAXIDX_FUNC_NOSIMD(minMaxIdx32s, int, int)
//DEFINE_MINMAXIDX_FUNC_NOSIMD(minMaxIdx32f, float, float)
DEFINE_MINMAXIDX_FUNC_NOSIMD(minMaxIdx64f, द्विगुन, द्विगुन)
//DEFINE_MINMAXIDX_FUNC_NOSIMD(minMaxIdx16f, hfloat, float)
//DEFINE_MINMAXIDX_FUNC_NOSIMD(minMaxIdx16bf, bfloat, float)
DEFINE_MINMAXIDX_FUNC_NOSIMD(minMaxIdx64u, uint64, uint64)
DEFINE_MINMAXIDX_FUNC_NOSIMD(minMaxIdx64s, int64, int64)
DEFINE_MINMAXIDX_FUNC_NOSIMD(minMaxIdx32u, अचिन्हित, int64)

MinMaxIdxFunc getMinMaxIdxFunc(पूर्णांक depth)
{
    अटल MinMaxIdxFunc minMaxIdxTab[CV_DEPTH_MAX] =
    {
        minMaxIdxWrap<uchar, पूर्णांक, minMaxIdx8u>,
        minMaxIdxWrap<schar, पूर्णांक, minMaxIdx8s>,
        minMaxIdxWrap<ushort, पूर्णांक, minMaxIdx16u>,
        minMaxIdxWrap<लघु, पूर्णांक, minMaxIdx16s>,
        minMaxIdxWrap<पूर्णांक, पूर्णांक, minMaxIdx32s>,
        minMaxIdxWrap<भग्न, भग्न, minMaxIdx32f>,
        minMaxIdxWrap<द्विगुन, द्विगुन, minMaxIdx64f>,
        minMaxIdxWrap<hfloat, भग्न, minMaxIdx16f>,
        minMaxIdxWrap<bfloat, भग्न, minMaxIdx16bf>,
        minMaxIdxWrap<uchar, पूर्णांक, minMaxIdx8u>,
        minMaxIdxWrap<uint64, uint64, minMaxIdx64u>,
        minMaxIdxWrap<int64, int64, minMaxIdx64s>,
        minMaxIdxWrap<अचिन्हित, int64, minMaxIdx32u>,
        0
    };

    वापस minMaxIdxTab[depth];
}

#पूर्ण_अगर

CV_CPU_OPTIMIZATION_NAMESPACE_END
} // namespace
