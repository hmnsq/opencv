// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html

#अन्तर्भाव "precomp.hpp"

नामाकाश cv {

प्रकारनिर्देश शून्य (*MinMaxIdxFunc)(ध्रुव uchar* दत्त, ध्रुव uchar* आवरणम्,
                              शून्य* minval, शून्य* maxval,
                              size_t* minidx, size_t* maxidx,
                              पूर्णाङ्क len, size_t startidx);

CV_CPU_OPTIMIZATION_NAMESPACE_BEGIN

प्रतिरूपम्<प्रकारनाम T, प्रकारनाम WT, शून्य (*fn)(ध्रुव T*, ध्रुव uchar*, WT*, WT*, size_t*, size_t*, पूर्णाङ्क, size_t)>
स्थिर शून्य minMaxIdxWrap(ध्रुव uchar* दत्त, ध्रुव uchar* आवरणम्,
                          शून्य* minval, शून्य* maxval,
                          size_t* minidx, size_t* maxidx,
                          पूर्णाङ्क len, size_t startidx)
{
    fn((ध्रुव T*)दत्त, आवरणम्, (WT*)minval, (WT*)maxval, minidx, maxidx, len, startidx);
}

MinMaxIdxFunc getMinMaxIdxFunc(पूर्णाङ्क गभीरता);

#यदि_अपरिभाषितम् CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

प्रतिरूपम्<प्रकारनाम T, प्रकारनाम WT> स्थिर शून्य
minMaxIdx_( ध्रुव T* src, ध्रुव uchar* आवरणम्, WT* _minVal, WT* _maxVal,
            size_t* _minIdx, size_t* _maxIdx, पूर्णाङ्क len, size_t startIdx )
{
    WT minVal = *_minVal, maxVal = *_maxVal;
    size_t minIdx = *_minIdx, maxIdx = *_maxIdx;
    पूर्णाङ्क i = 0;

    यदि (minIdx == 0 || maxIdx == 0) {
        यदि (आवरणम्) {
            प्रति (; i < len; i++) {
                यदि (आवरणम्[i]) {
                    minVal = maxVal = (WT)src[i];
                    minIdx = maxIdx = startIdx + i;
                    i++;
                    भङ्ग;
                }
            }
        }
        अन्यथा यदि (len > 0) {
            minVal = maxVal = (WT)src[0];
            minIdx = maxIdx = startIdx;
            i++;
        }
    }

    यदि( !आवरणम् )
    {
        प्रति( ; i < len; i++ )
        {
            WT val = (WT)src[i];
            यदि( val < minVal )
            {
                minVal = val;
                minIdx = startIdx + i;
            }
            यदि( val > maxVal )
            {
                maxVal = val;
                maxIdx = startIdx + i;
            }
        }
    }
    अन्यथा
    {
        प्रति( ; i < len; i++ )
        {
            WT val = (WT)src[i];
            uchar m = आवरणम्[i];
            यदि( m && val < minVal )
            {
                minVal = val;
                minIdx = startIdx + i;
            }
            यदि( m && val > maxVal )
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

#अपरिभाषय SIMD_ONLY
#यदि (CV_SIMD || CV_SIMD_SCALABLE)
#परिभाषय SIMD_ONLY(व्यञ्जकम्) व्यञ्जकम्
#अन्यथा
#परिभाषय SIMD_ONLY(व्यञ्जकम्)
#यदि_अन्तः

स्थिर पूर्णाङ्क minMaxInit(ध्रुव uchar* आवरणम्, पूर्णाङ्क len)
{
    पूर्णाङ्क i = 0;
    SIMD_ONLY(
    पूर्णाङ्क vlanes = VTraits<v_uint8>::vlanes();
    v_uint8 v_zero = vx_setzero_u8();
    प्रति (; i < len; i += vlanes) {
        यदि (i + vlanes > len) {
            यदि (i == 0)
                भङ्ग;
            i = len - vlanes;
        }
        v_uint8 आवरणम्_i = v_ne(vx_आरोपय(आवरणम् + i), v_zero);
        यदि (v_check_any(आवरणम्_i))
            प्रत्यागम i + v_अवलोकनम्_forward(आवरणम्_i);
    })
    प्रति (; i < len; i++) {
        यदि (आवरणम्[i] != 0)
            प्रत्यागम i;
    }
    प्रत्यागम -1;
}

// vectorized implementation for u8, s8, u16 and s16
// uses blocks to decrease the lane size necessary to store indices
#अपरिभाषय DEFINE_MINMAXIDX_SMALLINT_FUNC
#परिभाषय DEFINE_MINMAXIDX_SMALLINT_FUNC(funcname, suffix, usuffix, T, UT, VT, UVT, WT, खण्डम्_आकार, आरोपय_आवरणम्) \
स्थिर शून्य funcname(ध्रुव T* src, ध्रुव uchar* आवरणम्, WT* _minVal, WT* _maxVal, \
                     size_t* _minIdx, size_t* _maxIdx, पूर्णाङ्क len, size_t startIdx) \
{ \
    T minVal = T(*_minVal), maxVal = T(*_maxVal); \
    size_t minIdx = *_minIdx, maxIdx = *_maxIdx; \
    पूर्णाङ्क i = 0; \
    /* initialize minVal/maxVal/minIdx/maxIdx to the proper values in the beginning */ \
    यदि (minIdx == 0) { \
        यदि (आवरणम्) { \
            i = minMaxInit(आवरणम्, len); \
            यदि (i < 0) \
                प्रत्यागम; \
        } \
        minVal = maxVal = src[i]; \
        minIdx = maxIdx = startIdx + i; \
        i++; \
    } \
    SIMD_ONLY( \
    ध्रुव पूर्णाङ्क vlanes = VTraits<VT>::vlanes(); \
    ध्रुव पूर्णाङ्क खण्डम्_size0 = खण्डम्_आकार - vlanes; \
    यदि (len-i >= vlanes && खण्डम्_size0 > 0 && खण्डम्_size0 % vlanes == 0) { \
        UT idxbuf[VTraits<UVT>::max_nlanes]; \
        प्रति (पूर्णाङ्क j = 0; j < vlanes; j++) \
            idxbuf[j] = (UT)j; \
        UVT v_idx0 = vx_आरोपय(idxbuf); \
        UVT v_idx_delta = vx_setall_##usuffix((UT)vlanes); \
        UVT v_invalid_idx = vx_setall_##usuffix((UT)-1); \
        VT v_minval = vx_setall_##suffix(minVal); \
        VT v_maxval = vx_setall_##suffix(maxVal); \
        पूर्णाङ्क खण्डम्_आकार_3 = खण्डम्_size0; \
        /* process data by blocks: */ \
        /* - for u8/s8 data each block contains up to 256-vlanes elements */ \
        /* - for u16/s16 data each block contains up to 65536-vlanes elements */ \
        /* inside each block we can store the relative (local) index (v_locidx) */ \
        /* in a compact way: 8 bits per lane for u8/s8 data, */ \
        /*                  16 bits per lane for u16/s16 data */ \
        /* 0b111...111 is "invalid index", meaning that this */ \
        /* particular lane has not been updated. */ \
        /* after each block we update minVal, maxVal, minIdx and maxIdx */ \
        प्रति (; i <= len - vlanes; i += खण्डम्_आकार_3) { \
            खण्डम्_आकार_3 = std::min(खण्डम्_आकार_3, (len - i) & -vlanes); \
            UVT v_locidx = v_idx0; \
            UVT v_minidx = v_invalid_idx; \
            UVT v_maxidx = v_invalid_idx; \
            यदि (!आवरणम्) { \
                प्रति (पूर्णाङ्क j = 0; j < खण्डम्_आकार_3; j += vlanes) { \
                    VT दत्त = vx_आरोपय(src + i + j); \
                    UVT lt_min = v_reinterpret_as_##usuffix(v_lt(दत्त, v_minval)); \
                    UVT gt_max = v_reinterpret_as_##usuffix(v_gt(दत्त, v_maxval)); \
                    v_minidx = v_select(lt_min, v_locidx, v_minidx); \
                    v_maxidx = v_select(gt_max, v_locidx, v_maxidx); \
                    v_minval = v_min(v_minval, दत्त); \
                    v_maxval = v_max(v_maxval, दत्त); \
                    v_locidx = v_सङ्कलय(v_locidx, v_idx_delta); \
                } \
            } अन्यथा { \
                UVT v_zero = vx_setzero_##usuffix(); \
                प्रति (पूर्णाङ्क j = 0; j < खण्डम्_आकार_3; j += vlanes) { \
                    VT दत्त = vx_आरोपय(src + i + j); \
                    UVT msk = v_ne(आरोपय_आवरणम्(आवरणम् + i + j), v_zero); \
                    UVT lt_min = v_reinterpret_as_##usuffix(v_lt(दत्त, v_minval)); \
                    UVT gt_max = v_reinterpret_as_##usuffix(v_gt(दत्त, v_maxval)); \
                    lt_min = v_and(lt_min, msk); \
                    gt_max = v_and(gt_max, msk); \
                    v_minidx = v_select(lt_min, v_locidx, v_minidx); \
                    v_maxidx = v_select(gt_max, v_locidx, v_maxidx); \
                    VT lt_min_दत्त = v_reinterpret_as_##suffix(lt_min); \
                    VT gt_max_दत्त = v_reinterpret_as_##suffix(gt_max); \
                    v_minval = v_select(lt_min_दत्त, दत्त, v_minval); \
                    v_maxval = v_select(gt_max_दत्त, दत्त, v_maxval); \
                    v_locidx = v_सङ्कलय(v_locidx, v_idx_delta); \
                } \
            } \
            /* for both minimum and maximum we check whether global extremum */ \
            /* and its index need to be updated. If yes, we compute */ \
            /* the smallest index within the block where the new global */ \
            /* extremum value occurs */ \
            UVT idxmask = v_ne(v_minidx, v_invalid_idx); \
            यदि (v_check_any(idxmask)) { \
                minVal = (T)v_reduce_min(v_minval); \
                VT invmask = v_ne(v_minval, vx_setall_##suffix(minVal)); \
                v_minidx = v_or(v_minidx, v_reinterpret_as_##usuffix(invmask)); \
                minIdx = startIdx + i + v_reduce_min(v_minidx); \
                v_minval = vx_setall_##suffix(minVal); \
            } \
            idxmask = v_ne(v_maxidx, v_invalid_idx); \
            यदि (v_check_any(idxmask)) { \
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
    यदि (i < len) { \
        src += i; \
        यदि (आवरणम्) आवरणम् += i; \
        startIdx += i; \
        len -= i; \
        minMaxIdx_(src, आवरणम्, _minVal, _maxVal, _minIdx, _maxIdx, len, startIdx); \
    } \
}

// vectorized implementation for s32, f32, f16 and bf16
// (potentially can be extended for u32)
// no need to use blocks here
#अपरिभाषय DEFINE_MINMAXIDX_FUNC
#परिभाषय DEFINE_MINMAXIDX_FUNC(funcname, suffix, usuffix, T, UT, VT, UVT, WT, आरोपय_op) \
स्थिर शून्य funcname(ध्रुव T* src, ध्रुव uchar* आवरणम्, WT* _minVal, WT* _maxVal, \
                     size_t* _minIdx, size_t* _maxIdx, पूर्णाङ्क len, size_t startIdx) \
{ \
    WT minVal = *_minVal, maxVal = *_maxVal; \
    size_t minIdx = *_minIdx, maxIdx = *_maxIdx; \
    पूर्णाङ्क i = 0; \
    /* initialize minVal/maxVal/minIdx/maxIdx to the proper values in the beginning */ \
    यदि (minIdx == 0) { \
        यदि (आवरणम्) { \
            i = minMaxInit(आवरणम्, len); \
            यदि (i < 0) \
                प्रत्यागम; \
        } \
        minVal = maxVal = src[i]; \
        minIdx = maxIdx = startIdx + i; \
        i++; \
    } \
    SIMD_ONLY( \
    ध्रुव पूर्णाङ्क vlanes = VTraits<VT>::vlanes(); \
    UT idxbuf[VTraits<UVT>::max_nlanes]; \
    प्रति (पूर्णाङ्क j = 0; j < vlanes; j++) \
        idxbuf[j] = (UT)(i+j); \
    UVT v_locidx = vx_आरोपय(idxbuf); \
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
    यदि (!आवरणम्) { \
        प्रति (; i <= len - vlanes; i += vlanes) { \
            VT दत्त = आरोपय_op(src + i); \
            UVT lt_min = v_reinterpret_as_##usuffix(v_lt(दत्त, v_minval)); \
            UVT gt_max = v_reinterpret_as_##usuffix(v_gt(दत्त, v_maxval)); \
            v_minidx = v_select(lt_min, v_locidx, v_minidx); \
            v_maxidx = v_select(gt_max, v_locidx, v_maxidx); \
            v_minval = v_min(v_minval, दत्त); \
            v_maxval = v_max(v_maxval, दत्त); \
            v_locidx = v_सङ्कलय(v_locidx, v_idx_delta); \
        } \
    } अन्यथा { \
        UVT v_zero = vx_setzero_##usuffix(); \
        प्रति (; i <= len - vlanes; i += vlanes) { \
            VT दत्त = आरोपय_op(src + i); \
            UVT msk = v_ne(vx_आरोपय_expand_q(आवरणम् + i), v_zero); \
            UVT lt_min = v_reinterpret_as_##usuffix(v_lt(दत्त, v_minval)); \
            UVT gt_max = v_reinterpret_as_##usuffix(v_gt(दत्त, v_maxval)); \
            lt_min = v_and(lt_min, msk); \
            gt_max = v_and(gt_max, msk); \
            v_minidx = v_select(lt_min, v_locidx, v_minidx); \
            v_maxidx = v_select(gt_max, v_locidx, v_maxidx); \
            VT lt_min_दत्त = v_reinterpret_as_##suffix(lt_min); \
            VT gt_max_दत्त = v_reinterpret_as_##suffix(gt_max); \
            v_minval = v_select(lt_min_दत्त, दत्त, v_minval); \
            v_maxval = v_select(gt_max_दत्त, दत्त, v_maxval); \
            v_locidx = v_सङ्कलय(v_locidx, v_idx_delta); \
        } \
    } \
    /* for both minimum and maximum we check whether global extremum */ \
    /* and its index need to be updated. If yes, we compute */ \
    /* the smallest index within the block where the new global */ \
    /* extremum value occurs */ \
    UVT idxmask = v_ne(v_minidx, v_invalid_idx); \
    यदि (v_check_any(idxmask)) { \
        minVal = v_reduce_min(v_minval); \
        VT invmask = v_ne(v_minval, vx_setall_##suffix(minVal)); \
        v_minidx = v_or(v_minidx, v_reinterpret_as_##usuffix(invmask)); \
        minIdx = startIdx + v_reduce_min(v_minidx); \
        v_minval = vx_setall_##suffix(minVal); \
    } \
    idxmask = v_ne(v_maxidx, v_invalid_idx); \
    यदि (v_check_any(idxmask)) { \
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
    यदि (i < len) { \
        src += i; \
        यदि (आवरणम्) आवरणम् += i; \
        startIdx += i; \
        len -= i; \
        minMaxIdx_(src, आवरणम्, _minVal, _maxVal, _minIdx, _maxIdx, len, startIdx); \
    } \
}

#अपरिभाषय DEFINE_MINMAXIDX_FUNC_NOSIMD
#परिभाषय DEFINE_MINMAXIDX_FUNC_NOSIMD(funcname, T, WT) \
स्थिर शून्य funcname(ध्रुव T* src, ध्रुव uchar* आवरणम्, WT* _minVal, WT* _maxVal, \
                     size_t* _minIdx, size_t* _maxIdx, पूर्णाङ्क len, size_t startIdx) \
{ \
    minMaxIdx_(src, आवरणम्, _minVal, _maxVal, _minIdx, _maxIdx, len, startIdx); \
}

DEFINE_MINMAXIDX_SMALLINT_FUNC(minMaxIdx8u, u8, u8, uchar, uchar, v_uint8, v_uint8, पूर्णाङ्क, 256, vx_आरोपय)
DEFINE_MINMAXIDX_SMALLINT_FUNC(minMaxIdx8s, s8, u8, schar, uchar, v_int8, v_uint8, पूर्णाङ्क, 256, vx_आरोपय)
DEFINE_MINMAXIDX_SMALLINT_FUNC(minMaxIdx16u, u16, u16, ushort, ushort, v_uint16, v_uint16, पूर्णाङ्क, 65536, vx_आरोपय_expand)
DEFINE_MINMAXIDX_SMALLINT_FUNC(minMaxIdx16s, s16, u16, ह्रस्व, ushort, v_int16, v_uint16, पूर्णाङ्क, 65536, vx_आरोपय_expand)

DEFINE_MINMAXIDX_FUNC(minMaxIdx32s, s32, u32, पूर्णाङ्क, अचिह्नित, v_int32, v_uint32, पूर्णाङ्क, vx_आरोपय)
DEFINE_MINMAXIDX_FUNC(minMaxIdx32f, f32, u32, भिन्न, अचिह्नित, v_float32, v_uint32, भिन्न, vx_आरोपय)
DEFINE_MINMAXIDX_FUNC(minMaxIdx16f, f32, u32, hfloat, अचिह्नित, v_float32, v_uint32, भिन्न, vx_आरोपय_expand)
DEFINE_MINMAXIDX_FUNC(minMaxIdx16bf, f32, u32, bfloat, अचिह्नित, v_float32, v_uint32, भिन्न, vx_आरोपय_expand)

//DEFINE_MINMAXIDX_FUNC_NOSIMD(minMaxIdx32s, int, int)
//DEFINE_MINMAXIDX_FUNC_NOSIMD(minMaxIdx32f, float, float)
DEFINE_MINMAXIDX_FUNC_NOSIMD(minMaxIdx64f, द्विगुण, द्विगुण)
//DEFINE_MINMAXIDX_FUNC_NOSIMD(minMaxIdx16f, hfloat, float)
//DEFINE_MINMAXIDX_FUNC_NOSIMD(minMaxIdx16bf, bfloat, float)
DEFINE_MINMAXIDX_FUNC_NOSIMD(minMaxIdx64u, uint64, uint64)
DEFINE_MINMAXIDX_FUNC_NOSIMD(minMaxIdx64s, int64, int64)
DEFINE_MINMAXIDX_FUNC_NOSIMD(minMaxIdx32u, अचिह्नित, int64)

MinMaxIdxFunc getMinMaxIdxFunc(पूर्णाङ्क गभीरता)
{
    स्थिर MinMaxIdxFunc minMaxIdxTab[CV_गभीरता_MAX] =
    {
        minMaxIdxWrap<uchar, पूर्णाङ्क, minMaxIdx8u>,
        minMaxIdxWrap<schar, पूर्णाङ्क, minMaxIdx8s>,
        minMaxIdxWrap<ushort, पूर्णाङ्क, minMaxIdx16u>,
        minMaxIdxWrap<ह्रस्व, पूर्णाङ्क, minMaxIdx16s>,
        minMaxIdxWrap<पूर्णाङ्क, पूर्णाङ्क, minMaxIdx32s>,
        minMaxIdxWrap<भिन्न, भिन्न, minMaxIdx32f>,
        minMaxIdxWrap<द्विगुण, द्विगुण, minMaxIdx64f>,
        minMaxIdxWrap<hfloat, भिन्न, minMaxIdx16f>,
        minMaxIdxWrap<bfloat, भिन्न, minMaxIdx16bf>,
        minMaxIdxWrap<uchar, पूर्णाङ्क, minMaxIdx8u>,
        minMaxIdxWrap<uint64, uint64, minMaxIdx64u>,
        minMaxIdxWrap<int64, int64, minMaxIdx64s>,
        minMaxIdxWrap<अचिह्नित, int64, minMaxIdx32u>,
        0
    };

    प्रत्यागम minMaxIdxTab[गभीरता];
}

#यदि_अन्तः

CV_CPU_OPTIMIZATION_NAMESPACE_अन्तः
} // namespace
