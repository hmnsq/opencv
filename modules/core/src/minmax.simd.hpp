// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html

#ਅਨ੍ਤਰ੍ਭਾਵ "precomp.hpp"

ਨਾਮਾਕਾਸ਼ cv {

ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਖਾਲੀ (*MinMaxIdxFunc)(ਅਚਲ uchar* ਅੰਕੜੇ, ਅਚਲ uchar* ਨਕਾਬ,
                              ਖਾਲੀ* minval, ਖਾਲੀ* maxval,
                              size_t* minidx, size_t* maxidx,
                              ਪੂਰਨ len, size_t startidx);

CV_CPU_OPTIMIZATION_NAMESPACE_BEGIN

ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ T, ਕਿਸਮ_ਨਾਂ WT, ਖਾਲੀ (*fn)(ਅਚਲ T*, ਅਚਲ uchar*, WT*, WT*, size_t*, size_t*, ਪੂਰਨ, size_t)>
ਸਥਿਰ ਖਾਲੀ minMaxIdxWrap(ਅਚਲ uchar* ਅੰਕੜੇ, ਅਚਲ uchar* ਨਕਾਬ,
                          ਖਾਲੀ* minval, ਖਾਲੀ* maxval,
                          size_t* minidx, size_t* maxidx,
                          ਪੂਰਨ len, size_t startidx)
{
    fn((ਅਚਲ T*)ਅੰਕੜੇ, ਨਕਾਬ, (WT*)minval, (WT*)maxval, minidx, maxidx, len, startidx);
}

MinMaxIdxFunc getMinMaxIdxFunc(ਪੂਰਨ ਡੂੰਘਾਈ);

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ T, ਕਿਸਮ_ਨਾਂ WT> ਸਥਿਰ ਖਾਲੀ
minMaxIdx_( ਅਚਲ T* src, ਅਚਲ uchar* ਨਕਾਬ, WT* _minVal, WT* _maxVal,
            size_t* _minIdx, size_t* _maxIdx, ਪੂਰਨ len, size_t ਸ਼ੁਰੂ_Idx )
{
    WT minVal = *_minVal, maxVal = *_maxVal;
    size_t minIdx = *_minIdx, maxIdx = *_maxIdx;
    ਪੂਰਨ i = 0;

    ਜੇ (minIdx == 0 || maxIdx == 0) {
        ਜੇ (ਨਕਾਬ) {
            ਲਈ (; i < len; i++) {
                ਜੇ (ਨਕਾਬ[i]) {
                    minVal = maxVal = (WT)src[i];
                    minIdx = maxIdx = ਸ਼ੁਰੂ_Idx + i;
                    i++;
                    ਰੋਕੋ;
                }
            }
        }
        ਨਹੀਂਤਾਂ ਜੇ (len > 0) {
            minVal = maxVal = (WT)src[0];
            minIdx = maxIdx = ਸ਼ੁਰੂ_Idx;
            i++;
        }
    }

    ਜੇ( !ਨਕਾਬ )
    {
        ਲਈ( ; i < len; i++ )
        {
            WT val = (WT)src[i];
            ਜੇ( val < minVal )
            {
                minVal = val;
                minIdx = ਸ਼ੁਰੂ_Idx + i;
            }
            ਜੇ( val > maxVal )
            {
                maxVal = val;
                maxIdx = ਸ਼ੁਰੂ_Idx + i;
            }
        }
    }
    ਨਹੀਂਤਾਂ
    {
        ਲਈ( ; i < len; i++ )
        {
            WT val = (WT)src[i];
            uchar m = ਨਕਾਬ[i];
            ਜੇ( m && val < minVal )
            {
                minVal = val;
                minIdx = ਸ਼ੁਰੂ_Idx + i;
            }
            ਜੇ( m && val > maxVal )
            {
                maxVal = val;
                maxIdx = ਸ਼ੁਰੂ_Idx + i;
            }
        }
    }

    *_minIdx = minIdx;
    *_maxIdx = maxIdx;
    *_minVal = minVal;
    *_maxVal = maxVal;
}

#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ SIMD_ONLY
#ਜੇ (CV_SIMD || CV_SIMD_SCALABLE)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ SIMD_ONLY(ਵਿਅੰਜਕ) ਵਿਅੰਜਕ
#ਨਹੀਂਤਾਂ
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ SIMD_ONLY(ਵਿਅੰਜਕ)
#ਜੇ_ਅੰਤ

ਸਥਿਰ ਪੂਰਨ minMaxInit(ਅਚਲ uchar* ਨਕਾਬ, ਪੂਰਨ len)
{
    ਪੂਰਨ i = 0;
    SIMD_ONLY(
    ਪੂਰਨ vlanes = VTraits<v_uint8>::vlanes();
    v_uint8 v_zero = vx_setzero_u8();
    ਲਈ (; i < len; i += vlanes) {
        ਜੇ (i + vlanes > len) {
            ਜੇ (i == 0)
                ਰੋਕੋ;
            i = len - vlanes;
        }
        v_uint8 ਨਕਾਬ_i = v_ne(vx_ਲੱਦੋ(ਨਕਾਬ + i), v_zero);
        ਜੇ (v_check_any(ਨਕਾਬ_i))
            ਵਾਪਸ i + v_ਜਾਂਚ_forward(ਨਕਾਬ_i);
    })
    ਲਈ (; i < len; i++) {
        ਜੇ (ਨਕਾਬ[i] != 0)
            ਵਾਪਸ i;
    }
    ਵਾਪਸ -1;
}

// vectorized implementation for u8, s8, u16 and s16
// uses blocks to decrease the lane size necessary to store indices
#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ DEFINE_MINMAXIDX_SMALLINT_FUNC
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ DEFINE_MINMAXIDX_SMALLINT_FUNC(funcname, suffix, usuffix, T, UT, VT, UVT, WT, ਖੰਡ_ਮਾਪ, ਲੱਦੋ_ਨਕਾਬ) \
ਸਥਿਰ ਖਾਲੀ funcname(ਅਚਲ T* src, ਅਚਲ uchar* ਨਕਾਬ, WT* _minVal, WT* _maxVal, \
                     size_t* _minIdx, size_t* _maxIdx, ਪੂਰਨ len, size_t ਸ਼ੁਰੂ_Idx) \
{ \
    T minVal = T(*_minVal), maxVal = T(*_maxVal); \
    size_t minIdx = *_minIdx, maxIdx = *_maxIdx; \
    ਪੂਰਨ i = 0; \
    /* initialize minVal/maxVal/minIdx/maxIdx to the proper values in the beginning */ \
    ਜੇ (minIdx == 0) { \
        ਜੇ (ਨਕਾਬ) { \
            i = minMaxInit(ਨਕਾਬ, len); \
            ਜੇ (i < 0) \
                ਵਾਪਸ; \
        } \
        minVal = maxVal = src[i]; \
        minIdx = maxIdx = ਸ਼ੁਰੂ_Idx + i; \
        i++; \
    } \
    SIMD_ONLY( \
    ਅਚਲ ਪੂਰਨ vlanes = VTraits<VT>::vlanes(); \
    ਅਚਲ ਪੂਰਨ ਖੰਡ_size0 = ਖੰਡ_ਮਾਪ - vlanes; \
    ਜੇ (len-i >= vlanes && ਖੰਡ_size0 > 0 && ਖੰਡ_size0 % vlanes == 0) { \
        UT idxbuf[VTraits<UVT>::max_nlanes]; \
        ਲਈ (ਪੂਰਨ j = 0; j < vlanes; j++) \
            idxbuf[j] = (UT)j; \
        UVT v_idx0 = vx_ਲੱਦੋ(idxbuf); \
        UVT v_idx_delta = vx_setall_##usuffix((UT)vlanes); \
        UVT v_invalid_idx = vx_setall_##usuffix((UT)-1); \
        VT v_minval = vx_setall_##suffix(minVal); \
        VT v_maxval = vx_setall_##suffix(maxVal); \
        ਪੂਰਨ ਖੰਡ_ਮਾਪ_3 = ਖੰਡ_size0; \
        /* process data by blocks: */ \
        /* - for u8/s8 data each block contains up to 256-vlanes elements */ \
        /* - for u16/s16 data each block contains up to 65536-vlanes elements */ \
        /* inside each block we can store the relative (local) index (v_locidx) */ \
        /* in a compact way: 8 bits per lane for u8/s8 data, */ \
        /*                  16 bits per lane for u16/s16 data */ \
        /* 0b111...111 is "invalid index", meaning that this */ \
        /* particular lane has not been updated. */ \
        /* after each block we update minVal, maxVal, minIdx and maxIdx */ \
        ਲਈ (; i <= len - vlanes; i += ਖੰਡ_ਮਾਪ_3) { \
            ਖੰਡ_ਮਾਪ_3 = std::min(ਖੰਡ_ਮਾਪ_3, (len - i) & -vlanes); \
            UVT v_locidx = v_idx0; \
            UVT v_minidx = v_invalid_idx; \
            UVT v_maxidx = v_invalid_idx; \
            ਜੇ (!ਨਕਾਬ) { \
                ਲਈ (ਪੂਰਨ j = 0; j < ਖੰਡ_ਮਾਪ_3; j += vlanes) { \
                    VT ਅੰਕੜੇ = vx_ਲੱਦੋ(src + i + j); \
                    UVT lt_min = v_reinterpret_as_##usuffix(v_lt(ਅੰਕੜੇ, v_minval)); \
                    UVT gt_max = v_reinterpret_as_##usuffix(v_gt(ਅੰਕੜੇ, v_maxval)); \
                    v_minidx = v_select(lt_min, v_locidx, v_minidx); \
                    v_maxidx = v_select(gt_max, v_locidx, v_maxidx); \
                    v_minval = v_min(v_minval, ਅੰਕੜੇ); \
                    v_maxval = v_max(v_maxval, ਅੰਕੜੇ); \
                    v_locidx = v_ਜੋੜੋ(v_locidx, v_idx_delta); \
                } \
            } ਨਹੀਂਤਾਂ { \
                UVT v_zero = vx_setzero_##usuffix(); \
                ਲਈ (ਪੂਰਨ j = 0; j < ਖੰਡ_ਮਾਪ_3; j += vlanes) { \
                    VT ਅੰਕੜੇ = vx_ਲੱਦੋ(src + i + j); \
                    UVT msk = v_ne(ਲੱਦੋ_ਨਕਾਬ(ਨਕਾਬ + i + j), v_zero); \
                    UVT lt_min = v_reinterpret_as_##usuffix(v_lt(ਅੰਕੜੇ, v_minval)); \
                    UVT gt_max = v_reinterpret_as_##usuffix(v_gt(ਅੰਕੜੇ, v_maxval)); \
                    lt_min = v_and(lt_min, msk); \
                    gt_max = v_and(gt_max, msk); \
                    v_minidx = v_select(lt_min, v_locidx, v_minidx); \
                    v_maxidx = v_select(gt_max, v_locidx, v_maxidx); \
                    VT lt_min_ਅੰਕੜੇ = v_reinterpret_as_##suffix(lt_min); \
                    VT gt_max_ਅੰਕੜੇ = v_reinterpret_as_##suffix(gt_max); \
                    v_minval = v_select(lt_min_ਅੰਕੜੇ, ਅੰਕੜੇ, v_minval); \
                    v_maxval = v_select(gt_max_ਅੰਕੜੇ, ਅੰਕੜੇ, v_maxval); \
                    v_locidx = v_ਜੋੜੋ(v_locidx, v_idx_delta); \
                } \
            } \
            /* for both minimum and maximum we check whether global extremum */ \
            /* and its index need to be updated. If yes, we compute */ \
            /* the smallest index within the block where the new global */ \
            /* extremum value occurs */ \
            UVT idxmask = v_ne(v_minidx, v_invalid_idx); \
            ਜੇ (v_check_any(idxmask)) { \
                minVal = (T)v_reduce_min(v_minval); \
                VT invmask = v_ne(v_minval, vx_setall_##suffix(minVal)); \
                v_minidx = v_or(v_minidx, v_reinterpret_as_##usuffix(invmask)); \
                minIdx = ਸ਼ੁਰੂ_Idx + i + v_reduce_min(v_minidx); \
                v_minval = vx_setall_##suffix(minVal); \
            } \
            idxmask = v_ne(v_maxidx, v_invalid_idx); \
            ਜੇ (v_check_any(idxmask)) { \
                maxVal = (T)v_reduce_max(v_maxval); \
                VT invmask = v_ne(v_maxval, vx_setall_##suffix(maxVal)); \
                v_maxidx = v_or(v_maxidx, v_reinterpret_as_##usuffix(invmask)); \
                maxIdx = ਸ਼ੁਰੂ_Idx + i + v_reduce_min(v_maxidx); \
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
    ਜੇ (i < len) { \
        src += i; \
        ਜੇ (ਨਕਾਬ) ਨਕਾਬ += i; \
        ਸ਼ੁਰੂ_Idx += i; \
        len -= i; \
        minMaxIdx_(src, ਨਕਾਬ, _minVal, _maxVal, _minIdx, _maxIdx, len, ਸ਼ੁਰੂ_Idx); \
    } \
}

// vectorized implementation for s32, f32, f16 and bf16
// (potentially can be extended for u32)
// no need to use blocks here
#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ DEFINE_MINMAXIDX_FUNC
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ DEFINE_MINMAXIDX_FUNC(funcname, suffix, usuffix, T, UT, VT, UVT, WT, ਲੱਦੋ_op) \
ਸਥਿਰ ਖਾਲੀ funcname(ਅਚਲ T* src, ਅਚਲ uchar* ਨਕਾਬ, WT* _minVal, WT* _maxVal, \
                     size_t* _minIdx, size_t* _maxIdx, ਪੂਰਨ len, size_t ਸ਼ੁਰੂ_Idx) \
{ \
    WT minVal = *_minVal, maxVal = *_maxVal; \
    size_t minIdx = *_minIdx, maxIdx = *_maxIdx; \
    ਪੂਰਨ i = 0; \
    /* initialize minVal/maxVal/minIdx/maxIdx to the proper values in the beginning */ \
    ਜੇ (minIdx == 0) { \
        ਜੇ (ਨਕਾਬ) { \
            i = minMaxInit(ਨਕਾਬ, len); \
            ਜੇ (i < 0) \
                ਵਾਪਸ; \
        } \
        minVal = maxVal = src[i]; \
        minIdx = maxIdx = ਸ਼ੁਰੂ_Idx + i; \
        i++; \
    } \
    SIMD_ONLY( \
    ਅਚਲ ਪੂਰਨ vlanes = VTraits<VT>::vlanes(); \
    UT idxbuf[VTraits<UVT>::max_nlanes]; \
    ਲਈ (ਪੂਰਨ j = 0; j < vlanes; j++) \
        idxbuf[j] = (UT)(i+j); \
    UVT v_locidx = vx_ਲੱਦੋ(idxbuf); \
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
    ਜੇ (!ਨਕਾਬ) { \
        ਲਈ (; i <= len - vlanes; i += vlanes) { \
            VT ਅੰਕੜੇ = ਲੱਦੋ_op(src + i); \
            UVT lt_min = v_reinterpret_as_##usuffix(v_lt(ਅੰਕੜੇ, v_minval)); \
            UVT gt_max = v_reinterpret_as_##usuffix(v_gt(ਅੰਕੜੇ, v_maxval)); \
            v_minidx = v_select(lt_min, v_locidx, v_minidx); \
            v_maxidx = v_select(gt_max, v_locidx, v_maxidx); \
            v_minval = v_min(v_minval, ਅੰਕੜੇ); \
            v_maxval = v_max(v_maxval, ਅੰਕੜੇ); \
            v_locidx = v_ਜੋੜੋ(v_locidx, v_idx_delta); \
        } \
    } ਨਹੀਂਤਾਂ { \
        UVT v_zero = vx_setzero_##usuffix(); \
        ਲਈ (; i <= len - vlanes; i += vlanes) { \
            VT ਅੰਕੜੇ = ਲੱਦੋ_op(src + i); \
            UVT msk = v_ne(vx_ਲੱਦੋ_expand_q(ਨਕਾਬ + i), v_zero); \
            UVT lt_min = v_reinterpret_as_##usuffix(v_lt(ਅੰਕੜੇ, v_minval)); \
            UVT gt_max = v_reinterpret_as_##usuffix(v_gt(ਅੰਕੜੇ, v_maxval)); \
            lt_min = v_and(lt_min, msk); \
            gt_max = v_and(gt_max, msk); \
            v_minidx = v_select(lt_min, v_locidx, v_minidx); \
            v_maxidx = v_select(gt_max, v_locidx, v_maxidx); \
            VT lt_min_ਅੰਕੜੇ = v_reinterpret_as_##suffix(lt_min); \
            VT gt_max_ਅੰਕੜੇ = v_reinterpret_as_##suffix(gt_max); \
            v_minval = v_select(lt_min_ਅੰਕੜੇ, ਅੰਕੜੇ, v_minval); \
            v_maxval = v_select(gt_max_ਅੰਕੜੇ, ਅੰਕੜੇ, v_maxval); \
            v_locidx = v_ਜੋੜੋ(v_locidx, v_idx_delta); \
        } \
    } \
    /* for both minimum and maximum we check whether global extremum */ \
    /* and its index need to be updated. If yes, we compute */ \
    /* the smallest index within the block where the new global */ \
    /* extremum value occurs */ \
    UVT idxmask = v_ne(v_minidx, v_invalid_idx); \
    ਜੇ (v_check_any(idxmask)) { \
        minVal = v_reduce_min(v_minval); \
        VT invmask = v_ne(v_minval, vx_setall_##suffix(minVal)); \
        v_minidx = v_or(v_minidx, v_reinterpret_as_##usuffix(invmask)); \
        minIdx = ਸ਼ੁਰੂ_Idx + v_reduce_min(v_minidx); \
        v_minval = vx_setall_##suffix(minVal); \
    } \
    idxmask = v_ne(v_maxidx, v_invalid_idx); \
    ਜੇ (v_check_any(idxmask)) { \
        maxVal = v_reduce_max(v_maxval); \
        VT invmask = v_ne(v_maxval, vx_setall_##suffix(maxVal)); \
        v_maxidx = v_or(v_maxidx, v_reinterpret_as_##usuffix(invmask)); \
        maxIdx = ਸ਼ੁਰੂ_Idx + v_reduce_min(v_maxidx); \
        v_maxval = vx_setall_##suffix(maxVal); \
    }) \
    *_minVal = minVal; \
    *_maxVal = maxVal; \
    *_minIdx = minIdx; \
    *_maxIdx = maxIdx; \
    /* [TODO]: unlike sum, countNonZero and other reduce operations, */ \
    /* in the case of minMaxIdx we can process the tail using */ \
    /* vector overlapping technique (as in arithmetic operations) */ \
    ਜੇ (i < len) { \
        src += i; \
        ਜੇ (ਨਕਾਬ) ਨਕਾਬ += i; \
        ਸ਼ੁਰੂ_Idx += i; \
        len -= i; \
        minMaxIdx_(src, ਨਕਾਬ, _minVal, _maxVal, _minIdx, _maxIdx, len, ਸ਼ੁਰੂ_Idx); \
    } \
}

#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ DEFINE_MINMAXIDX_FUNC_NOSIMD
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ DEFINE_MINMAXIDX_FUNC_NOSIMD(funcname, T, WT) \
ਸਥਿਰ ਖਾਲੀ funcname(ਅਚਲ T* src, ਅਚਲ uchar* ਨਕਾਬ, WT* _minVal, WT* _maxVal, \
                     size_t* _minIdx, size_t* _maxIdx, ਪੂਰਨ len, size_t ਸ਼ੁਰੂ_Idx) \
{ \
    minMaxIdx_(src, ਨਕਾਬ, _minVal, _maxVal, _minIdx, _maxIdx, len, ਸ਼ੁਰੂ_Idx); \
}

DEFINE_MINMAXIDX_SMALLINT_FUNC(minMaxIdx8u, u8, u8, uchar, uchar, v_uint8, v_uint8, ਪੂਰਨ, 256, vx_ਲੱਦੋ)
DEFINE_MINMAXIDX_SMALLINT_FUNC(minMaxIdx8s, s8, u8, schar, uchar, v_int8, v_uint8, ਪੂਰਨ, 256, vx_ਲੱਦੋ)
DEFINE_MINMAXIDX_SMALLINT_FUNC(minMaxIdx16u, u16, u16, ushort, ushort, v_uint16, v_uint16, ਪੂਰਨ, 65536, vx_ਲੱਦੋ_expand)
DEFINE_MINMAXIDX_SMALLINT_FUNC(minMaxIdx16s, s16, u16, ਛੋਟਾ, ushort, v_int16, v_uint16, ਪੂਰਨ, 65536, vx_ਲੱਦੋ_expand)

DEFINE_MINMAXIDX_FUNC(minMaxIdx32s, s32, u32, ਪੂਰਨ, ਬਿਨਾਨਿਸ਼ਾਨੀ, v_int32, v_uint32, ਪੂਰਨ, vx_ਲੱਦੋ)
DEFINE_MINMAXIDX_FUNC(minMaxIdx32f, f32, u32, ਦਸ਼ਮਲਵ, ਬਿਨਾਨਿਸ਼ਾਨੀ, v_float32, v_uint32, ਦਸ਼ਮਲਵ, vx_ਲੱਦੋ)
DEFINE_MINMAXIDX_FUNC(minMaxIdx16f, f32, u32, hfloat, ਬਿਨਾਨਿਸ਼ਾਨੀ, v_float32, v_uint32, ਦਸ਼ਮਲਵ, vx_ਲੱਦੋ_expand)
DEFINE_MINMAXIDX_FUNC(minMaxIdx16bf, f32, u32, bfloat, ਬਿਨਾਨਿਸ਼ਾਨੀ, v_float32, v_uint32, ਦਸ਼ਮਲਵ, vx_ਲੱਦੋ_expand)

//DEFINE_MINMAXIDX_FUNC_NOSIMD(minMaxIdx32s, int, int)
//DEFINE_MINMAXIDX_FUNC_NOSIMD(minMaxIdx32f, float, float)
DEFINE_MINMAXIDX_FUNC_NOSIMD(minMaxIdx64f, ਦੁੱਗਣਾ, ਦੁੱਗਣਾ)
//DEFINE_MINMAXIDX_FUNC_NOSIMD(minMaxIdx16f, hfloat, float)
//DEFINE_MINMAXIDX_FUNC_NOSIMD(minMaxIdx16bf, bfloat, float)
DEFINE_MINMAXIDX_FUNC_NOSIMD(minMaxIdx64u, uint64, uint64)
DEFINE_MINMAXIDX_FUNC_NOSIMD(minMaxIdx64s, int64, int64)
DEFINE_MINMAXIDX_FUNC_NOSIMD(minMaxIdx32u, ਬਿਨਾਨਿਸ਼ਾਨੀ, int64)

MinMaxIdxFunc getMinMaxIdxFunc(ਪੂਰਨ ਡੂੰਘਾਈ)
{
    ਸਥਿਰ MinMaxIdxFunc minMaxIdxTab[CV_ਡੂੰਘਾਈ_MAX] =
    {
        minMaxIdxWrap<uchar, ਪੂਰਨ, minMaxIdx8u>,
        minMaxIdxWrap<schar, ਪੂਰਨ, minMaxIdx8s>,
        minMaxIdxWrap<ushort, ਪੂਰਨ, minMaxIdx16u>,
        minMaxIdxWrap<ਛੋਟਾ, ਪੂਰਨ, minMaxIdx16s>,
        minMaxIdxWrap<ਪੂਰਨ, ਪੂਰਨ, minMaxIdx32s>,
        minMaxIdxWrap<ਦਸ਼ਮਲਵ, ਦਸ਼ਮਲਵ, minMaxIdx32f>,
        minMaxIdxWrap<ਦੁੱਗਣਾ, ਦੁੱਗਣਾ, minMaxIdx64f>,
        minMaxIdxWrap<hfloat, ਦਸ਼ਮਲਵ, minMaxIdx16f>,
        minMaxIdxWrap<bfloat, ਦਸ਼ਮਲਵ, minMaxIdx16bf>,
        minMaxIdxWrap<uchar, ਪੂਰਨ, minMaxIdx8u>,
        minMaxIdxWrap<uint64, uint64, minMaxIdx64u>,
        minMaxIdxWrap<int64, int64, minMaxIdx64s>,
        minMaxIdxWrap<ਬਿਨਾਨਿਸ਼ਾਨੀ, int64, minMaxIdx32u>,
        0
    };

    ਵਾਪਸ minMaxIdxTab[ਡੂੰਘਾਈ];
}

#ਜੇ_ਅੰਤ

CV_CPU_OPTIMIZATION_NAMESPACE_ਸਿਰਾ
} // namespace
