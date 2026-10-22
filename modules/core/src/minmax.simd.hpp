// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html

#ಅನ್ತರ್ಭಾವ "precomp.hpp"

ನಾಮಾಕಾಶ cv {

ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಶೂನ್ಯ (*MinMaxIdxFunc)(ಸ್ಥಿರಾಂಕ uchar* ದತ್ತಾಂಶ, ಸ್ಥಿರಾಂಕ uchar* ಮುಖವಾಡ,
                              ಶೂನ್ಯ* minval, ಶೂನ್ಯ* maxval,
                              size_t* minidx, size_t* maxidx,
                              ಪೂರ್ಣಾಂಕ len, size_t startidx);

CV_CPU_OPTIMIZATION_NAMESPACE_BEGIN

ಮಾದರಿ<ಬಗೆ_ಹೆಸರು T, ಬಗೆ_ಹೆಸರು WT, ಶೂನ್ಯ (*fn)(ಸ್ಥಿರಾಂಕ T*, ಸ್ಥಿರಾಂಕ uchar*, WT*, WT*, size_t*, size_t*, ಪೂರ್ಣಾಂಕ, size_t)>
ಸ್ಥಿರ ಶೂನ್ಯ minMaxIdxWrap(ಸ್ಥಿರಾಂಕ uchar* ದತ್ತಾಂಶ, ಸ್ಥಿರಾಂಕ uchar* ಮುಖವಾಡ,
                          ಶೂನ್ಯ* minval, ಶೂನ್ಯ* maxval,
                          size_t* minidx, size_t* maxidx,
                          ಪೂರ್ಣಾಂಕ len, size_t startidx)
{
    fn((ಸ್ಥಿರಾಂಕ T*)ದತ್ತಾಂಶ, ಮುಖವಾಡ, (WT*)minval, (WT*)maxval, minidx, maxidx, len, startidx);
}

MinMaxIdxFunc getMinMaxIdxFunc(ಪೂರ್ಣಾಂಕ ಆಳ);

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

ಮಾದರಿ<ಬಗೆ_ಹೆಸರು T, ಬಗೆ_ಹೆಸರು WT> ಸ್ಥಿರ ಶೂನ್ಯ
minMaxIdx_( ಸ್ಥಿರಾಂಕ T* src, ಸ್ಥಿರಾಂಕ uchar* ಮುಖವಾಡ, WT* _minVal, WT* _maxVal,
            size_t* _minIdx, size_t* _maxIdx, ಪೂರ್ಣಾಂಕ len, size_t ಆರಂಭ_Idx )
{
    WT minVal = *_minVal, maxVal = *_maxVal;
    size_t minIdx = *_minIdx, maxIdx = *_maxIdx;
    ಪೂರ್ಣಾಂಕ i = 0;

    ಆದರೆ (minIdx == 0 || maxIdx == 0) {
        ಆದರೆ (ಮುಖವಾಡ) {
            ಗಾಗಿ (; i < len; i++) {
                ಆದರೆ (ಮುಖವಾಡ[i]) {
                    minVal = maxVal = (WT)src[i];
                    minIdx = maxIdx = ಆರಂಭ_Idx + i;
                    i++;
                    ನಿಲ್ಲಿಸು;
                }
            }
        }
        ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (len > 0) {
            minVal = maxVal = (WT)src[0];
            minIdx = maxIdx = ಆರಂಭ_Idx;
            i++;
        }
    }

    ಆದರೆ( !ಮುಖವಾಡ )
    {
        ಗಾಗಿ( ; i < len; i++ )
        {
            WT val = (WT)src[i];
            ಆದರೆ( val < minVal )
            {
                minVal = val;
                minIdx = ಆರಂಭ_Idx + i;
            }
            ಆದರೆ( val > maxVal )
            {
                maxVal = val;
                maxIdx = ಆರಂಭ_Idx + i;
            }
        }
    }
    ಇಲ್ಲದಿದ್ದರೆ
    {
        ಗಾಗಿ( ; i < len; i++ )
        {
            WT val = (WT)src[i];
            uchar m = ಮುಖವಾಡ[i];
            ಆದರೆ( m && val < minVal )
            {
                minVal = val;
                minIdx = ಆರಂಭ_Idx + i;
            }
            ಆದರೆ( m && val > maxVal )
            {
                maxVal = val;
                maxIdx = ಆರಂಭ_Idx + i;
            }
        }
    }

    *_minIdx = minIdx;
    *_maxIdx = maxIdx;
    *_minVal = minVal;
    *_maxVal = maxVal;
}

#ವ್ಯಾಖ್ಯಾನ_ರದ್ದು SIMD_ONLY
#ಆದರೆ (CV_SIMD || CV_SIMD_SCALABLE)
#ವ್ಯಾಖ್ಯಾನಿಸು SIMD_ONLY(ಅಭಿವ್ಯಕ್ತಿ) ಅಭಿವ್ಯಕ್ತಿ
#ಇಲ್ಲದಿದ್ದರೆ
#ವ್ಯಾಖ್ಯಾನಿಸು SIMD_ONLY(ಅಭಿವ್ಯಕ್ತಿ)
#ಆದರೆ_ಅಂತ್ಯ

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ minMaxInit(ಸ್ಥಿರಾಂಕ uchar* ಮುಖವಾಡ, ಪೂರ್ಣಾಂಕ len)
{
    ಪೂರ್ಣಾಂಕ i = 0;
    SIMD_ONLY(
    ಪೂರ್ಣಾಂಕ vlanes = VTraits<v_uint8>::vlanes();
    v_uint8 v_zero = vx_setzero_u8();
    ಗಾಗಿ (; i < len; i += vlanes) {
        ಆದರೆ (i + vlanes > len) {
            ಆದರೆ (i == 0)
                ನಿಲ್ಲಿಸು;
            i = len - vlanes;
        }
        v_uint8 ಮುಖವಾಡ_i = v_ne(vx_ತುಂಬು(ಮುಖವಾಡ + i), v_zero);
        ಆದರೆ (v_check_any(ಮುಖವಾಡ_i))
            ಹಿಂತಿರುಗಿ i + v_ಪರಿಶೀಲನೆ_forward(ಮುಖವಾಡ_i);
    })
    ಗಾಗಿ (; i < len; i++) {
        ಆದರೆ (ಮುಖವಾಡ[i] != 0)
            ಹಿಂತಿರುಗಿ i;
    }
    ಹಿಂತಿರುಗಿ -1;
}

// vectorized implementation for u8, s8, u16 and s16
// uses blocks to decrease the lane size necessary to store indices
#ವ್ಯಾಖ್ಯಾನ_ರದ್ದು DEFINE_MINMAXIDX_SMALLINT_FUNC
#ವ್ಯಾಖ್ಯಾನಿಸು DEFINE_MINMAXIDX_SMALLINT_FUNC(funcname, suffix, usuffix, T, UT, VT, UVT, WT, ಖಂಡ_ಗಾತ್ರ, ತುಂಬು_ಮುಖವಾಡ) \
ಸ್ಥಿರ ಶೂನ್ಯ funcname(ಸ್ಥಿರಾಂಕ T* src, ಸ್ಥಿರಾಂಕ uchar* ಮುಖವಾಡ, WT* _minVal, WT* _maxVal, \
                     size_t* _minIdx, size_t* _maxIdx, ಪೂರ್ಣಾಂಕ len, size_t ಆರಂಭ_Idx) \
{ \
    T minVal = T(*_minVal), maxVal = T(*_maxVal); \
    size_t minIdx = *_minIdx, maxIdx = *_maxIdx; \
    ಪೂರ್ಣಾಂಕ i = 0; \
    /* initialize minVal/maxVal/minIdx/maxIdx to the proper values in the beginning */ \
    ಆದರೆ (minIdx == 0) { \
        ಆದರೆ (ಮುಖವಾಡ) { \
            i = minMaxInit(ಮುಖವಾಡ, len); \
            ಆದರೆ (i < 0) \
                ಹಿಂತಿರುಗಿ; \
        } \
        minVal = maxVal = src[i]; \
        minIdx = maxIdx = ಆರಂಭ_Idx + i; \
        i++; \
    } \
    SIMD_ONLY( \
    ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ vlanes = VTraits<VT>::vlanes(); \
    ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ ಖಂಡ_size0 = ಖಂಡ_ಗಾತ್ರ - vlanes; \
    ಆದರೆ (len-i >= vlanes && ಖಂಡ_size0 > 0 && ಖಂಡ_size0 % vlanes == 0) { \
        UT idxbuf[VTraits<UVT>::max_nlanes]; \
        ಗಾಗಿ (ಪೂರ್ಣಾಂಕ j = 0; j < vlanes; j++) \
            idxbuf[j] = (UT)j; \
        UVT v_idx0 = vx_ತುಂಬು(idxbuf); \
        UVT v_idx_delta = vx_setall_##usuffix((UT)vlanes); \
        UVT v_invalid_idx = vx_setall_##usuffix((UT)-1); \
        VT v_minval = vx_setall_##suffix(minVal); \
        VT v_maxval = vx_setall_##suffix(maxVal); \
        ಪೂರ್ಣಾಂಕ ಖಂಡ_ಗಾತ್ರ_3 = ಖಂಡ_size0; \
        /* process data by blocks: */ \
        /* - for u8/s8 data each block contains up to 256-vlanes elements */ \
        /* - for u16/s16 data each block contains up to 65536-vlanes elements */ \
        /* inside each block we can store the relative (local) index (v_locidx) */ \
        /* in a compact way: 8 bits per lane for u8/s8 data, */ \
        /*                  16 bits per lane for u16/s16 data */ \
        /* 0b111...111 is "invalid index", meaning that this */ \
        /* particular lane has not been updated. */ \
        /* after each block we update minVal, maxVal, minIdx and maxIdx */ \
        ಗಾಗಿ (; i <= len - vlanes; i += ಖಂಡ_ಗಾತ್ರ_3) { \
            ಖಂಡ_ಗಾತ್ರ_3 = std::min(ಖಂಡ_ಗಾತ್ರ_3, (len - i) & -vlanes); \
            UVT v_locidx = v_idx0; \
            UVT v_minidx = v_invalid_idx; \
            UVT v_maxidx = v_invalid_idx; \
            ಆದರೆ (!ಮುಖವಾಡ) { \
                ಗಾಗಿ (ಪೂರ್ಣಾಂಕ j = 0; j < ಖಂಡ_ಗಾತ್ರ_3; j += vlanes) { \
                    VT ದತ್ತಾಂಶ = vx_ತುಂಬು(src + i + j); \
                    UVT lt_min = v_reinterpret_as_##usuffix(v_lt(ದತ್ತಾಂಶ, v_minval)); \
                    UVT gt_max = v_reinterpret_as_##usuffix(v_gt(ದತ್ತಾಂಶ, v_maxval)); \
                    v_minidx = v_select(lt_min, v_locidx, v_minidx); \
                    v_maxidx = v_select(gt_max, v_locidx, v_maxidx); \
                    v_minval = v_min(v_minval, ದತ್ತಾಂಶ); \
                    v_maxval = v_max(v_maxval, ದತ್ತಾಂಶ); \
                    v_locidx = v_ಕೂಡಿಸು(v_locidx, v_idx_delta); \
                } \
            } ಇಲ್ಲದಿದ್ದರೆ { \
                UVT v_zero = vx_setzero_##usuffix(); \
                ಗಾಗಿ (ಪೂರ್ಣಾಂಕ j = 0; j < ಖಂಡ_ಗಾತ್ರ_3; j += vlanes) { \
                    VT ದತ್ತಾಂಶ = vx_ತುಂಬು(src + i + j); \
                    UVT msk = v_ne(ತುಂಬು_ಮುಖವಾಡ(ಮುಖವಾಡ + i + j), v_zero); \
                    UVT lt_min = v_reinterpret_as_##usuffix(v_lt(ದತ್ತಾಂಶ, v_minval)); \
                    UVT gt_max = v_reinterpret_as_##usuffix(v_gt(ದತ್ತಾಂಶ, v_maxval)); \
                    lt_min = v_and(lt_min, msk); \
                    gt_max = v_and(gt_max, msk); \
                    v_minidx = v_select(lt_min, v_locidx, v_minidx); \
                    v_maxidx = v_select(gt_max, v_locidx, v_maxidx); \
                    VT lt_min_ದತ್ತಾಂಶ = v_reinterpret_as_##suffix(lt_min); \
                    VT gt_max_ದತ್ತಾಂಶ = v_reinterpret_as_##suffix(gt_max); \
                    v_minval = v_select(lt_min_ದತ್ತಾಂಶ, ದತ್ತಾಂಶ, v_minval); \
                    v_maxval = v_select(gt_max_ದತ್ತಾಂಶ, ದತ್ತಾಂಶ, v_maxval); \
                    v_locidx = v_ಕೂಡಿಸು(v_locidx, v_idx_delta); \
                } \
            } \
            /* for both minimum and maximum we check whether global extremum */ \
            /* and its index need to be updated. If yes, we compute */ \
            /* the smallest index within the block where the new global */ \
            /* extremum value occurs */ \
            UVT idxmask = v_ne(v_minidx, v_invalid_idx); \
            ಆದರೆ (v_check_any(idxmask)) { \
                minVal = (T)v_reduce_min(v_minval); \
                VT invmask = v_ne(v_minval, vx_setall_##suffix(minVal)); \
                v_minidx = v_or(v_minidx, v_reinterpret_as_##usuffix(invmask)); \
                minIdx = ಆರಂಭ_Idx + i + v_reduce_min(v_minidx); \
                v_minval = vx_setall_##suffix(minVal); \
            } \
            idxmask = v_ne(v_maxidx, v_invalid_idx); \
            ಆದರೆ (v_check_any(idxmask)) { \
                maxVal = (T)v_reduce_max(v_maxval); \
                VT invmask = v_ne(v_maxval, vx_setall_##suffix(maxVal)); \
                v_maxidx = v_or(v_maxidx, v_reinterpret_as_##usuffix(invmask)); \
                maxIdx = ಆರಂಭ_Idx + i + v_reduce_min(v_maxidx); \
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
    ಆದರೆ (i < len) { \
        src += i; \
        ಆದರೆ (ಮುಖವಾಡ) ಮುಖವಾಡ += i; \
        ಆರಂಭ_Idx += i; \
        len -= i; \
        minMaxIdx_(src, ಮುಖವಾಡ, _minVal, _maxVal, _minIdx, _maxIdx, len, ಆರಂಭ_Idx); \
    } \
}

// vectorized implementation for s32, f32, f16 and bf16
// (potentially can be extended for u32)
// no need to use blocks here
#ವ್ಯಾಖ್ಯಾನ_ರದ್ದು DEFINE_MINMAXIDX_FUNC
#ವ್ಯಾಖ್ಯಾನಿಸು DEFINE_MINMAXIDX_FUNC(funcname, suffix, usuffix, T, UT, VT, UVT, WT, ತುಂಬು_op) \
ಸ್ಥಿರ ಶೂನ್ಯ funcname(ಸ್ಥಿರಾಂಕ T* src, ಸ್ಥಿರಾಂಕ uchar* ಮುಖವಾಡ, WT* _minVal, WT* _maxVal, \
                     size_t* _minIdx, size_t* _maxIdx, ಪೂರ್ಣಾಂಕ len, size_t ಆರಂಭ_Idx) \
{ \
    WT minVal = *_minVal, maxVal = *_maxVal; \
    size_t minIdx = *_minIdx, maxIdx = *_maxIdx; \
    ಪೂರ್ಣಾಂಕ i = 0; \
    /* initialize minVal/maxVal/minIdx/maxIdx to the proper values in the beginning */ \
    ಆದರೆ (minIdx == 0) { \
        ಆದರೆ (ಮುಖವಾಡ) { \
            i = minMaxInit(ಮುಖವಾಡ, len); \
            ಆದರೆ (i < 0) \
                ಹಿಂತಿರುಗಿ; \
        } \
        minVal = maxVal = src[i]; \
        minIdx = maxIdx = ಆರಂಭ_Idx + i; \
        i++; \
    } \
    SIMD_ONLY( \
    ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ vlanes = VTraits<VT>::vlanes(); \
    UT idxbuf[VTraits<UVT>::max_nlanes]; \
    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ j = 0; j < vlanes; j++) \
        idxbuf[j] = (UT)(i+j); \
    UVT v_locidx = vx_ತುಂಬು(idxbuf); \
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
    ಆದರೆ (!ಮುಖವಾಡ) { \
        ಗಾಗಿ (; i <= len - vlanes; i += vlanes) { \
            VT ದತ್ತಾಂಶ = ತುಂಬು_op(src + i); \
            UVT lt_min = v_reinterpret_as_##usuffix(v_lt(ದತ್ತಾಂಶ, v_minval)); \
            UVT gt_max = v_reinterpret_as_##usuffix(v_gt(ದತ್ತಾಂಶ, v_maxval)); \
            v_minidx = v_select(lt_min, v_locidx, v_minidx); \
            v_maxidx = v_select(gt_max, v_locidx, v_maxidx); \
            v_minval = v_min(v_minval, ದತ್ತಾಂಶ); \
            v_maxval = v_max(v_maxval, ದತ್ತಾಂಶ); \
            v_locidx = v_ಕೂಡಿಸು(v_locidx, v_idx_delta); \
        } \
    } ಇಲ್ಲದಿದ್ದರೆ { \
        UVT v_zero = vx_setzero_##usuffix(); \
        ಗಾಗಿ (; i <= len - vlanes; i += vlanes) { \
            VT ದತ್ತಾಂಶ = ತುಂಬು_op(src + i); \
            UVT msk = v_ne(vx_ತುಂಬು_expand_q(ಮುಖವಾಡ + i), v_zero); \
            UVT lt_min = v_reinterpret_as_##usuffix(v_lt(ದತ್ತಾಂಶ, v_minval)); \
            UVT gt_max = v_reinterpret_as_##usuffix(v_gt(ದತ್ತಾಂಶ, v_maxval)); \
            lt_min = v_and(lt_min, msk); \
            gt_max = v_and(gt_max, msk); \
            v_minidx = v_select(lt_min, v_locidx, v_minidx); \
            v_maxidx = v_select(gt_max, v_locidx, v_maxidx); \
            VT lt_min_ದತ್ತಾಂಶ = v_reinterpret_as_##suffix(lt_min); \
            VT gt_max_ದತ್ತಾಂಶ = v_reinterpret_as_##suffix(gt_max); \
            v_minval = v_select(lt_min_ದತ್ತಾಂಶ, ದತ್ತಾಂಶ, v_minval); \
            v_maxval = v_select(gt_max_ದತ್ತಾಂಶ, ದತ್ತಾಂಶ, v_maxval); \
            v_locidx = v_ಕೂಡಿಸು(v_locidx, v_idx_delta); \
        } \
    } \
    /* for both minimum and maximum we check whether global extremum */ \
    /* and its index need to be updated. If yes, we compute */ \
    /* the smallest index within the block where the new global */ \
    /* extremum value occurs */ \
    UVT idxmask = v_ne(v_minidx, v_invalid_idx); \
    ಆದರೆ (v_check_any(idxmask)) { \
        minVal = v_reduce_min(v_minval); \
        VT invmask = v_ne(v_minval, vx_setall_##suffix(minVal)); \
        v_minidx = v_or(v_minidx, v_reinterpret_as_##usuffix(invmask)); \
        minIdx = ಆರಂಭ_Idx + v_reduce_min(v_minidx); \
        v_minval = vx_setall_##suffix(minVal); \
    } \
    idxmask = v_ne(v_maxidx, v_invalid_idx); \
    ಆದರೆ (v_check_any(idxmask)) { \
        maxVal = v_reduce_max(v_maxval); \
        VT invmask = v_ne(v_maxval, vx_setall_##suffix(maxVal)); \
        v_maxidx = v_or(v_maxidx, v_reinterpret_as_##usuffix(invmask)); \
        maxIdx = ಆರಂಭ_Idx + v_reduce_min(v_maxidx); \
        v_maxval = vx_setall_##suffix(maxVal); \
    }) \
    *_minVal = minVal; \
    *_maxVal = maxVal; \
    *_minIdx = minIdx; \
    *_maxIdx = maxIdx; \
    /* [TODO]: unlike sum, countNonZero and other reduce operations, */ \
    /* in the case of minMaxIdx we can process the tail using */ \
    /* vector overlapping technique (as in arithmetic operations) */ \
    ಆದರೆ (i < len) { \
        src += i; \
        ಆದರೆ (ಮುಖವಾಡ) ಮುಖವಾಡ += i; \
        ಆರಂಭ_Idx += i; \
        len -= i; \
        minMaxIdx_(src, ಮುಖವಾಡ, _minVal, _maxVal, _minIdx, _maxIdx, len, ಆರಂಭ_Idx); \
    } \
}

#ವ್ಯಾಖ್ಯಾನ_ರದ್ದು DEFINE_MINMAXIDX_FUNC_NOSIMD
#ವ್ಯಾಖ್ಯಾನಿಸು DEFINE_MINMAXIDX_FUNC_NOSIMD(funcname, T, WT) \
ಸ್ಥಿರ ಶೂನ್ಯ funcname(ಸ್ಥಿರಾಂಕ T* src, ಸ್ಥಿರಾಂಕ uchar* ಮುಖವಾಡ, WT* _minVal, WT* _maxVal, \
                     size_t* _minIdx, size_t* _maxIdx, ಪೂರ್ಣಾಂಕ len, size_t ಆರಂಭ_Idx) \
{ \
    minMaxIdx_(src, ಮುಖವಾಡ, _minVal, _maxVal, _minIdx, _maxIdx, len, ಆರಂಭ_Idx); \
}

DEFINE_MINMAXIDX_SMALLINT_FUNC(minMaxIdx8u, u8, u8, uchar, uchar, v_uint8, v_uint8, ಪೂರ್ಣಾಂಕ, 256, vx_ತುಂಬು)
DEFINE_MINMAXIDX_SMALLINT_FUNC(minMaxIdx8s, s8, u8, schar, uchar, v_int8, v_uint8, ಪೂರ್ಣಾಂಕ, 256, vx_ತುಂಬು)
DEFINE_MINMAXIDX_SMALLINT_FUNC(minMaxIdx16u, u16, u16, ushort, ushort, v_uint16, v_uint16, ಪೂರ್ಣಾಂಕ, 65536, vx_ತುಂಬು_expand)
DEFINE_MINMAXIDX_SMALLINT_FUNC(minMaxIdx16s, s16, u16, ಗಿಡ್ಡ, ushort, v_int16, v_uint16, ಪೂರ್ಣಾಂಕ, 65536, vx_ತುಂಬು_expand)

DEFINE_MINMAXIDX_FUNC(minMaxIdx32s, s32, u32, ಪೂರ್ಣಾಂಕ, ಚಿಹ್ನೆರಹಿತ, v_int32, v_uint32, ಪೂರ್ಣಾಂಕ, vx_ತುಂಬು)
DEFINE_MINMAXIDX_FUNC(minMaxIdx32f, f32, u32, ದಶಮಾಂಶ, ಚಿಹ್ನೆರಹಿತ, v_float32, v_uint32, ದಶಮಾಂಶ, vx_ತುಂಬು)
DEFINE_MINMAXIDX_FUNC(minMaxIdx16f, f32, u32, hfloat, ಚಿಹ್ನೆರಹಿತ, v_float32, v_uint32, ದಶಮಾಂಶ, vx_ತುಂಬು_expand)
DEFINE_MINMAXIDX_FUNC(minMaxIdx16bf, f32, u32, bfloat, ಚಿಹ್ನೆರಹಿತ, v_float32, v_uint32, ದಶಮಾಂಶ, vx_ತುಂಬು_expand)

//DEFINE_MINMAXIDX_FUNC_NOSIMD(minMaxIdx32s, int, int)
//DEFINE_MINMAXIDX_FUNC_NOSIMD(minMaxIdx32f, float, float)
DEFINE_MINMAXIDX_FUNC_NOSIMD(minMaxIdx64f, ದ್ವಿಗುಣ, ದ್ವಿಗುಣ)
//DEFINE_MINMAXIDX_FUNC_NOSIMD(minMaxIdx16f, hfloat, float)
//DEFINE_MINMAXIDX_FUNC_NOSIMD(minMaxIdx16bf, bfloat, float)
DEFINE_MINMAXIDX_FUNC_NOSIMD(minMaxIdx64u, uint64, uint64)
DEFINE_MINMAXIDX_FUNC_NOSIMD(minMaxIdx64s, int64, int64)
DEFINE_MINMAXIDX_FUNC_NOSIMD(minMaxIdx32u, ಚಿಹ್ನೆರಹಿತ, int64)

MinMaxIdxFunc getMinMaxIdxFunc(ಪೂರ್ಣಾಂಕ ಆಳ)
{
    ಸ್ಥಿರ MinMaxIdxFunc minMaxIdxTab[CV_ಆಳ_MAX] =
    {
        minMaxIdxWrap<uchar, ಪೂರ್ಣಾಂಕ, minMaxIdx8u>,
        minMaxIdxWrap<schar, ಪೂರ್ಣಾಂಕ, minMaxIdx8s>,
        minMaxIdxWrap<ushort, ಪೂರ್ಣಾಂಕ, minMaxIdx16u>,
        minMaxIdxWrap<ಗಿಡ್ಡ, ಪೂರ್ಣಾಂಕ, minMaxIdx16s>,
        minMaxIdxWrap<ಪೂರ್ಣಾಂಕ, ಪೂರ್ಣಾಂಕ, minMaxIdx32s>,
        minMaxIdxWrap<ದಶಮಾಂಶ, ದಶಮಾಂಶ, minMaxIdx32f>,
        minMaxIdxWrap<ದ್ವಿಗುಣ, ದ್ವಿಗುಣ, minMaxIdx64f>,
        minMaxIdxWrap<hfloat, ದಶಮಾಂಶ, minMaxIdx16f>,
        minMaxIdxWrap<bfloat, ದಶಮಾಂಶ, minMaxIdx16bf>,
        minMaxIdxWrap<uchar, ಪೂರ್ಣಾಂಕ, minMaxIdx8u>,
        minMaxIdxWrap<uint64, uint64, minMaxIdx64u>,
        minMaxIdxWrap<int64, int64, minMaxIdx64s>,
        minMaxIdxWrap<ಚಿಹ್ನೆರಹಿತ, int64, minMaxIdx32u>,
        0
    };

    ಹಿಂತಿರುಗಿ minMaxIdxTab[ಆಳ];
}

#ಆದರೆ_ಅಂತ್ಯ

CV_CPU_OPTIMIZATION_NAMESPACE_ಕೊನೆ
} // namespace
