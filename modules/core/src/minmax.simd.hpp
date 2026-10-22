// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html

#ضمن "precomp.hpp"

نطاق cv {

تعريفنوع فراغ (*MinMaxIdxFunc)(ثابت uchar* معلومات, ثابت uchar* ماسک,
                              فراغ* minval, فراغ* maxval,
                              size_t* minidx, size_t* maxidx,
                              صحيح len, size_t startidx);

CV_CPU_OPTIMIZATION_NAMESPACE_BEGIN

کينډۍ<ډول_نوم T, ډول_نوم WT, فراغ (*fn)(ثابت T*, ثابت uchar*, WT*, WT*, size_t*, size_t*, صحيح, size_t)>
ساكن فراغ minMaxIdxWrap(ثابت uchar* معلومات, ثابت uchar* ماسک,
                          فراغ* minval, فراغ* maxval,
                          size_t* minidx, size_t* maxidx,
                          صحيح len, size_t startidx)
{
    fn((ثابت T*)معلومات, ماسک, (WT*)minval, (WT*)maxval, minidx, maxidx, len, startidx);
}

MinMaxIdxFunc getMinMaxIdxFunc(صحيح ژوروالی);

#که_نه_تعریف_شوی CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

کينډۍ<ډول_نوم T, ډول_نوم WT> ساكن فراغ
minMaxIdx_( ثابت T* src, ثابت uchar* ماسک, WT* _minVal, WT* _maxVal,
            size_t* _minIdx, size_t* _maxIdx, صحيح len, size_t پیل_Idx )
{
    WT minVal = *_minVal, maxVal = *_maxVal;
    size_t minIdx = *_minIdx, maxIdx = *_maxIdx;
    صحيح i = 0;

    إذا (minIdx == 0 || maxIdx == 0) {
        إذا (ماسک) {
            لكل (; i < len; i++) {
                إذا (ماسک[i]) {
                    minVal = maxVal = (WT)src[i];
                    minIdx = maxIdx = پیل_Idx + i;
                    i++;
                    اكسر;
                }
            }
        }
        وإلا إذا (len > 0) {
            minVal = maxVal = (WT)src[0];
            minIdx = maxIdx = پیل_Idx;
            i++;
        }
    }

    إذا( !ماسک )
    {
        لكل( ; i < len; i++ )
        {
            WT val = (WT)src[i];
            إذا( val < minVal )
            {
                minVal = val;
                minIdx = پیل_Idx + i;
            }
            إذا( val > maxVal )
            {
                maxVal = val;
                maxIdx = پیل_Idx + i;
            }
        }
    }
    وإلا
    {
        لكل( ; i < len; i++ )
        {
            WT val = (WT)src[i];
            uchar m = ماسک[i];
            إذا( m && val < minVal )
            {
                minVal = val;
                minIdx = پیل_Idx + i;
            }
            إذا( m && val > maxVal )
            {
                maxVal = val;
                maxIdx = پیل_Idx + i;
            }
        }
    }

    *_minIdx = minIdx;
    *_maxIdx = maxIdx;
    *_minVal = minVal;
    *_maxVal = maxVal;
}

#تعریف_لغوه SIMD_ONLY
#إذا (CV_SIMD || CV_SIMD_SCALABLE)
#تعریف_کړه SIMD_ONLY(بیان) بیان
#وإلا
#تعریف_کړه SIMD_ONLY(بیان)
#د_که_پای

ساكن صحيح minMaxInit(ثابت uchar* ماسک, صحيح len)
{
    صحيح i = 0;
    SIMD_ONLY(
    صحيح vlanes = VTraits<v_uint8>::vlanes();
    v_uint8 v_zero = vx_setzero_u8();
    لكل (; i < len; i += vlanes) {
        إذا (i + vlanes > len) {
            إذا (i == 0)
                اكسر;
            i = len - vlanes;
        }
        v_uint8 ماسک_i = v_ne(vx_بارول(ماسک + i), v_zero);
        إذا (v_check_any(ماسک_i))
            أرجع i + v_لټون_forward(ماسک_i);
    })
    لكل (; i < len; i++) {
        إذا (ماسک[i] != 0)
            أرجع i;
    }
    أرجع -1;
}

// vectorized implementation for u8, s8, u16 and s16
// uses blocks to decrease the lane size necessary to store indices
#تعریف_لغوه DEFINE_MINMAXIDX_SMALLINT_FUNC
#تعریف_کړه DEFINE_MINMAXIDX_SMALLINT_FUNC(funcname, suffix, usuffix, T, UT, VT, UVT, WT, بلاک_اندازه, بارول_ماسک) \
ساكن فراغ funcname(ثابت T* src, ثابت uchar* ماسک, WT* _minVal, WT* _maxVal, \
                     size_t* _minIdx, size_t* _maxIdx, صحيح len, size_t پیل_Idx) \
{ \
    T minVal = T(*_minVal), maxVal = T(*_maxVal); \
    size_t minIdx = *_minIdx, maxIdx = *_maxIdx; \
    صحيح i = 0; \
    /* initialize minVal/maxVal/minIdx/maxIdx to the proper values in the beginning */ \
    إذا (minIdx == 0) { \
        إذا (ماسک) { \
            i = minMaxInit(ماسک, len); \
            إذا (i < 0) \
                أرجع; \
        } \
        minVal = maxVal = src[i]; \
        minIdx = maxIdx = پیل_Idx + i; \
        i++; \
    } \
    SIMD_ONLY( \
    ثابت صحيح vlanes = VTraits<VT>::vlanes(); \
    ثابت صحيح بلاک_size0 = بلاک_اندازه - vlanes; \
    إذا (len-i >= vlanes && بلاک_size0 > 0 && بلاک_size0 % vlanes == 0) { \
        UT idxbuf[VTraits<UVT>::max_nlanes]; \
        لكل (صحيح j = 0; j < vlanes; j++) \
            idxbuf[j] = (UT)j; \
        UVT v_idx0 = vx_بارول(idxbuf); \
        UVT v_idx_delta = vx_setall_##usuffix((UT)vlanes); \
        UVT v_invalid_idx = vx_setall_##usuffix((UT)-1); \
        VT v_minval = vx_setall_##suffix(minVal); \
        VT v_maxval = vx_setall_##suffix(maxVal); \
        صحيح بلاک_اندازه_3 = بلاک_size0; \
        /* process data by blocks: */ \
        /* - for u8/s8 data each block contains up to 256-vlanes elements */ \
        /* - for u16/s16 data each block contains up to 65536-vlanes elements */ \
        /* inside each block we can store the relative (local) index (v_locidx) */ \
        /* in a compact way: 8 bits per lane for u8/s8 data, */ \
        /*                  16 bits per lane for u16/s16 data */ \
        /* 0b111...111 is "invalid index", meaning that this */ \
        /* particular lane has not been updated. */ \
        /* after each block we update minVal, maxVal, minIdx and maxIdx */ \
        لكل (; i <= len - vlanes; i += بلاک_اندازه_3) { \
            بلاک_اندازه_3 = std::min(بلاک_اندازه_3, (len - i) & -vlanes); \
            UVT v_locidx = v_idx0; \
            UVT v_minidx = v_invalid_idx; \
            UVT v_maxidx = v_invalid_idx; \
            إذا (!ماسک) { \
                لكل (صحيح j = 0; j < بلاک_اندازه_3; j += vlanes) { \
                    VT معلومات = vx_بارول(src + i + j); \
                    UVT lt_min = v_reinterpret_as_##usuffix(v_lt(معلومات, v_minval)); \
                    UVT gt_max = v_reinterpret_as_##usuffix(v_gt(معلومات, v_maxval)); \
                    v_minidx = v_select(lt_min, v_locidx, v_minidx); \
                    v_maxidx = v_select(gt_max, v_locidx, v_maxidx); \
                    v_minval = v_min(v_minval, معلومات); \
                    v_maxval = v_max(v_maxval, معلومات); \
                    v_locidx = v_جمع_کړه(v_locidx, v_idx_delta); \
                } \
            } وإلا { \
                UVT v_zero = vx_setzero_##usuffix(); \
                لكل (صحيح j = 0; j < بلاک_اندازه_3; j += vlanes) { \
                    VT معلومات = vx_بارول(src + i + j); \
                    UVT msk = v_ne(بارول_ماسک(ماسک + i + j), v_zero); \
                    UVT lt_min = v_reinterpret_as_##usuffix(v_lt(معلومات, v_minval)); \
                    UVT gt_max = v_reinterpret_as_##usuffix(v_gt(معلومات, v_maxval)); \
                    lt_min = v_and(lt_min, msk); \
                    gt_max = v_and(gt_max, msk); \
                    v_minidx = v_select(lt_min, v_locidx, v_minidx); \
                    v_maxidx = v_select(gt_max, v_locidx, v_maxidx); \
                    VT lt_min_معلومات = v_reinterpret_as_##suffix(lt_min); \
                    VT gt_max_معلومات = v_reinterpret_as_##suffix(gt_max); \
                    v_minval = v_select(lt_min_معلومات, معلومات, v_minval); \
                    v_maxval = v_select(gt_max_معلومات, معلومات, v_maxval); \
                    v_locidx = v_جمع_کړه(v_locidx, v_idx_delta); \
                } \
            } \
            /* for both minimum and maximum we check whether global extremum */ \
            /* and its index need to be updated. If yes, we compute */ \
            /* the smallest index within the block where the new global */ \
            /* extremum value occurs */ \
            UVT idxmask = v_ne(v_minidx, v_invalid_idx); \
            إذا (v_check_any(idxmask)) { \
                minVal = (T)v_reduce_min(v_minval); \
                VT invmask = v_ne(v_minval, vx_setall_##suffix(minVal)); \
                v_minidx = v_or(v_minidx, v_reinterpret_as_##usuffix(invmask)); \
                minIdx = پیل_Idx + i + v_reduce_min(v_minidx); \
                v_minval = vx_setall_##suffix(minVal); \
            } \
            idxmask = v_ne(v_maxidx, v_invalid_idx); \
            إذا (v_check_any(idxmask)) { \
                maxVal = (T)v_reduce_max(v_maxval); \
                VT invmask = v_ne(v_maxval, vx_setall_##suffix(maxVal)); \
                v_maxidx = v_or(v_maxidx, v_reinterpret_as_##usuffix(invmask)); \
                maxIdx = پیل_Idx + i + v_reduce_min(v_maxidx); \
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
    إذا (i < len) { \
        src += i; \
        إذا (ماسک) ماسک += i; \
        پیل_Idx += i; \
        len -= i; \
        minMaxIdx_(src, ماسک, _minVal, _maxVal, _minIdx, _maxIdx, len, پیل_Idx); \
    } \
}

// vectorized implementation for s32, f32, f16 and bf16
// (potentially can be extended for u32)
// no need to use blocks here
#تعریف_لغوه DEFINE_MINMAXIDX_FUNC
#تعریف_کړه DEFINE_MINMAXIDX_FUNC(funcname, suffix, usuffix, T, UT, VT, UVT, WT, بارول_op) \
ساكن فراغ funcname(ثابت T* src, ثابت uchar* ماسک, WT* _minVal, WT* _maxVal, \
                     size_t* _minIdx, size_t* _maxIdx, صحيح len, size_t پیل_Idx) \
{ \
    WT minVal = *_minVal, maxVal = *_maxVal; \
    size_t minIdx = *_minIdx, maxIdx = *_maxIdx; \
    صحيح i = 0; \
    /* initialize minVal/maxVal/minIdx/maxIdx to the proper values in the beginning */ \
    إذا (minIdx == 0) { \
        إذا (ماسک) { \
            i = minMaxInit(ماسک, len); \
            إذا (i < 0) \
                أرجع; \
        } \
        minVal = maxVal = src[i]; \
        minIdx = maxIdx = پیل_Idx + i; \
        i++; \
    } \
    SIMD_ONLY( \
    ثابت صحيح vlanes = VTraits<VT>::vlanes(); \
    UT idxbuf[VTraits<UVT>::max_nlanes]; \
    لكل (صحيح j = 0; j < vlanes; j++) \
        idxbuf[j] = (UT)(i+j); \
    UVT v_locidx = vx_بارول(idxbuf); \
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
    إذا (!ماسک) { \
        لكل (; i <= len - vlanes; i += vlanes) { \
            VT معلومات = بارول_op(src + i); \
            UVT lt_min = v_reinterpret_as_##usuffix(v_lt(معلومات, v_minval)); \
            UVT gt_max = v_reinterpret_as_##usuffix(v_gt(معلومات, v_maxval)); \
            v_minidx = v_select(lt_min, v_locidx, v_minidx); \
            v_maxidx = v_select(gt_max, v_locidx, v_maxidx); \
            v_minval = v_min(v_minval, معلومات); \
            v_maxval = v_max(v_maxval, معلومات); \
            v_locidx = v_جمع_کړه(v_locidx, v_idx_delta); \
        } \
    } وإلا { \
        UVT v_zero = vx_setzero_##usuffix(); \
        لكل (; i <= len - vlanes; i += vlanes) { \
            VT معلومات = بارول_op(src + i); \
            UVT msk = v_ne(vx_بارول_expand_q(ماسک + i), v_zero); \
            UVT lt_min = v_reinterpret_as_##usuffix(v_lt(معلومات, v_minval)); \
            UVT gt_max = v_reinterpret_as_##usuffix(v_gt(معلومات, v_maxval)); \
            lt_min = v_and(lt_min, msk); \
            gt_max = v_and(gt_max, msk); \
            v_minidx = v_select(lt_min, v_locidx, v_minidx); \
            v_maxidx = v_select(gt_max, v_locidx, v_maxidx); \
            VT lt_min_معلومات = v_reinterpret_as_##suffix(lt_min); \
            VT gt_max_معلومات = v_reinterpret_as_##suffix(gt_max); \
            v_minval = v_select(lt_min_معلومات, معلومات, v_minval); \
            v_maxval = v_select(gt_max_معلومات, معلومات, v_maxval); \
            v_locidx = v_جمع_کړه(v_locidx, v_idx_delta); \
        } \
    } \
    /* for both minimum and maximum we check whether global extremum */ \
    /* and its index need to be updated. If yes, we compute */ \
    /* the smallest index within the block where the new global */ \
    /* extremum value occurs */ \
    UVT idxmask = v_ne(v_minidx, v_invalid_idx); \
    إذا (v_check_any(idxmask)) { \
        minVal = v_reduce_min(v_minval); \
        VT invmask = v_ne(v_minval, vx_setall_##suffix(minVal)); \
        v_minidx = v_or(v_minidx, v_reinterpret_as_##usuffix(invmask)); \
        minIdx = پیل_Idx + v_reduce_min(v_minidx); \
        v_minval = vx_setall_##suffix(minVal); \
    } \
    idxmask = v_ne(v_maxidx, v_invalid_idx); \
    إذا (v_check_any(idxmask)) { \
        maxVal = v_reduce_max(v_maxval); \
        VT invmask = v_ne(v_maxval, vx_setall_##suffix(maxVal)); \
        v_maxidx = v_or(v_maxidx, v_reinterpret_as_##usuffix(invmask)); \
        maxIdx = پیل_Idx + v_reduce_min(v_maxidx); \
        v_maxval = vx_setall_##suffix(maxVal); \
    }) \
    *_minVal = minVal; \
    *_maxVal = maxVal; \
    *_minIdx = minIdx; \
    *_maxIdx = maxIdx; \
    /* [TODO]: unlike sum, countNonZero and other reduce operations, */ \
    /* in the case of minMaxIdx we can process the tail using */ \
    /* vector overlapping technique (as in arithmetic operations) */ \
    إذا (i < len) { \
        src += i; \
        إذا (ماسک) ماسک += i; \
        پیل_Idx += i; \
        len -= i; \
        minMaxIdx_(src, ماسک, _minVal, _maxVal, _minIdx, _maxIdx, len, پیل_Idx); \
    } \
}

#تعریف_لغوه DEFINE_MINMAXIDX_FUNC_NOSIMD
#تعریف_کړه DEFINE_MINMAXIDX_FUNC_NOSIMD(funcname, T, WT) \
ساكن فراغ funcname(ثابت T* src, ثابت uchar* ماسک, WT* _minVal, WT* _maxVal, \
                     size_t* _minIdx, size_t* _maxIdx, صحيح len, size_t پیل_Idx) \
{ \
    minMaxIdx_(src, ماسک, _minVal, _maxVal, _minIdx, _maxIdx, len, پیل_Idx); \
}

DEFINE_MINMAXIDX_SMALLINT_FUNC(minMaxIdx8u, u8, u8, uchar, uchar, v_uint8, v_uint8, صحيح, 256, vx_بارول)
DEFINE_MINMAXIDX_SMALLINT_FUNC(minMaxIdx8s, s8, u8, schar, uchar, v_int8, v_uint8, صحيح, 256, vx_بارول)
DEFINE_MINMAXIDX_SMALLINT_FUNC(minMaxIdx16u, u16, u16, ushort, ushort, v_uint16, v_uint16, صحيح, 65536, vx_بارول_expand)
DEFINE_MINMAXIDX_SMALLINT_FUNC(minMaxIdx16s, s16, u16, لنډ, ushort, v_int16, v_uint16, صحيح, 65536, vx_بارول_expand)

DEFINE_MINMAXIDX_FUNC(minMaxIdx32s, s32, u32, صحيح, بینښه, v_int32, v_uint32, صحيح, vx_بارول)
DEFINE_MINMAXIDX_FUNC(minMaxIdx32f, f32, u32, عائم, بینښه, v_float32, v_uint32, عائم, vx_بارول)
DEFINE_MINMAXIDX_FUNC(minMaxIdx16f, f32, u32, hfloat, بینښه, v_float32, v_uint32, عائم, vx_بارول_expand)
DEFINE_MINMAXIDX_FUNC(minMaxIdx16bf, f32, u32, bfloat, بینښه, v_float32, v_uint32, عائم, vx_بارول_expand)

//DEFINE_MINMAXIDX_FUNC_NOSIMD(minMaxIdx32s, int, int)
//DEFINE_MINMAXIDX_FUNC_NOSIMD(minMaxIdx32f, float, float)
DEFINE_MINMAXIDX_FUNC_NOSIMD(minMaxIdx64f, مزدوج, مزدوج)
//DEFINE_MINMAXIDX_FUNC_NOSIMD(minMaxIdx16f, hfloat, float)
//DEFINE_MINMAXIDX_FUNC_NOSIMD(minMaxIdx16bf, bfloat, float)
DEFINE_MINMAXIDX_FUNC_NOSIMD(minMaxIdx64u, uint64, uint64)
DEFINE_MINMAXIDX_FUNC_NOSIMD(minMaxIdx64s, int64, int64)
DEFINE_MINMAXIDX_FUNC_NOSIMD(minMaxIdx32u, بینښه, int64)

MinMaxIdxFunc getMinMaxIdxFunc(صحيح ژوروالی)
{
    ساكن MinMaxIdxFunc minMaxIdxTab[CV_ژوروالی_MAX] =
    {
        minMaxIdxWrap<uchar, صحيح, minMaxIdx8u>,
        minMaxIdxWrap<schar, صحيح, minMaxIdx8s>,
        minMaxIdxWrap<ushort, صحيح, minMaxIdx16u>,
        minMaxIdxWrap<لنډ, صحيح, minMaxIdx16s>,
        minMaxIdxWrap<صحيح, صحيح, minMaxIdx32s>,
        minMaxIdxWrap<عائم, عائم, minMaxIdx32f>,
        minMaxIdxWrap<مزدوج, مزدوج, minMaxIdx64f>,
        minMaxIdxWrap<hfloat, عائم, minMaxIdx16f>,
        minMaxIdxWrap<bfloat, عائم, minMaxIdx16bf>,
        minMaxIdxWrap<uchar, صحيح, minMaxIdx8u>,
        minMaxIdxWrap<uint64, uint64, minMaxIdx64u>,
        minMaxIdxWrap<int64, int64, minMaxIdx64s>,
        minMaxIdxWrap<بینښه, int64, minMaxIdx32u>,
        0
    };

    أرجع minMaxIdxTab[ژوروالی];
}

#د_که_پای

CV_CPU_OPTIMIZATION_NAMESPACE_پای_ټکی
} // namespace
