// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html

#অন্তর্ভাব "precomp.hpp"

নামাকাশ cv {

প্রকারলেখ শূন্য (*MinMaxIdxFunc)(ধ্রুবক uchar* তথ্য, ধ্রুবক uchar* mask,
                              শূন্য* minval, শূন্য* maxval,
                              size_t* minidx, size_t* maxidx,
                              পূর্ণসংখ্যা len, size_t startidx);

CV_CPU_OPTIMIZATION_NAMESPACE_BEGIN

template<typename T, typename WT, শূন্য (*fn)(ধ্রুবক T*, ধ্রুবক uchar*, WT*, WT*, size_t*, size_t*, পূর্ণসংখ্যা, size_t)>
স্থির শূন্য minMaxIdxWrap(ধ্রুবক uchar* তথ্য, ধ্রুবক uchar* mask,
                          শূন্য* minval, শূন্য* maxval,
                          size_t* minidx, size_t* maxidx,
                          পূর্ণসংখ্যা len, size_t startidx)
{
    fn((ধ্রুবক T*)তথ্য, mask, (WT*)minval, (WT*)maxval, minidx, maxidx, len, startidx);
}

MinMaxIdxFunc getMinMaxIdxFunc(পূর্ণসংখ্যা depth);

#ifndef CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

template<typename T, typename WT> স্থির শূন্য
minMaxIdx_( ধ্রুবক T* src, ধ্রুবক uchar* mask, WT* _minVal, WT* _maxVal,
            size_t* _minIdx, size_t* _maxIdx, পূর্ণসংখ্যা len, size_t startIdx )
{
    WT minVal = *_minVal, maxVal = *_maxVal;
    size_t minIdx = *_minIdx, maxIdx = *_maxIdx;
    পূর্ণসংখ্যা i = 0;

    যদি (minIdx == 0 || maxIdx == 0) {
        যদি (mask) {
            জন্য (; i < len; i++) {
                যদি (mask[i]) {
                    minVal = maxVal = (WT)src[i];
                    minIdx = maxIdx = startIdx + i;
                    i++;
                    বিরতি;
                }
            }
        }
        অন্যথা যদি (len > 0) {
            minVal = maxVal = (WT)src[0];
            minIdx = maxIdx = startIdx;
            i++;
        }
    }

    যদি( !mask )
    {
        জন্য( ; i < len; i++ )
        {
            WT val = (WT)src[i];
            যদি( val < minVal )
            {
                minVal = val;
                minIdx = startIdx + i;
            }
            যদি( val > maxVal )
            {
                maxVal = val;
                maxIdx = startIdx + i;
            }
        }
    }
    অন্যথা
    {
        জন্য( ; i < len; i++ )
        {
            WT val = (WT)src[i];
            uchar m = mask[i];
            যদি( m && val < minVal )
            {
                minVal = val;
                minIdx = startIdx + i;
            }
            যদি( m && val > maxVal )
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

#undef SIMD_ONLY
#যদি (CV_SIMD || CV_SIMD_SCALABLE)
#define SIMD_ONLY(রাশিমালা) রাশিমালা
#অন্যথা
#define SIMD_ONLY(রাশিমালা)
#endif

স্থির পূর্ণসংখ্যা minMaxInit(ধ্রুবক uchar* mask, পূর্ণসংখ্যা len)
{
    পূর্ণসংখ্যা i = 0;
    SIMD_ONLY(
    পূর্ণসংখ্যা vlanes = VTraits<v_uint8>::vlanes();
    v_uint8 v_zero = vx_setzero_u8();
    জন্য (; i < len; i += vlanes) {
        যদি (i + vlanes > len) {
            যদি (i == 0)
                বিরতি;
            i = len - vlanes;
        }
        v_uint8 mask_i = v_ne(vx_load(mask + i), v_zero);
        যদি (v_check_any(mask_i))
            ফেরত i + v_scan_forward(mask_i);
    })
    জন্য (; i < len; i++) {
        যদি (mask[i] != 0)
            ফেরত i;
    }
    ফেরত -1;
}

// vectorized implementation for u8, s8, u16 and s16
// uses blocks to decrease the lane size necessary to store indices
#undef DEFINE_MINMAXIDX_SMALLINT_FUNC
#define DEFINE_MINMAXIDX_SMALLINT_FUNC(funcname, suffix, usuffix, T, UT, VT, UVT, WT, BLOCK_আকার, load_mask) \
স্থির শূন্য funcname(ধ্রুবক T* src, ধ্রুবক uchar* mask, WT* _minVal, WT* _maxVal, \
                     size_t* _minIdx, size_t* _maxIdx, পূর্ণসংখ্যা len, size_t startIdx) \
{ \
    T minVal = T(*_minVal), maxVal = T(*_maxVal); \
    size_t minIdx = *_minIdx, maxIdx = *_maxIdx; \
    পূর্ণসংখ্যা i = 0; \
    /* initialize minVal/maxVal/minIdx/maxIdx to the proper values in the beginning */ \
    যদি (minIdx == 0) { \
        যদি (mask) { \
            i = minMaxInit(mask, len); \
            যদি (i < 0) \
                ফেরত; \
        } \
        minVal = maxVal = src[i]; \
        minIdx = maxIdx = startIdx + i; \
        i++; \
    } \
    SIMD_ONLY( \
    ধ্রুবক পূর্ণসংখ্যা vlanes = VTraits<VT>::vlanes(); \
    ধ্রুবক পূর্ণসংখ্যা block_size0 = BLOCK_আকার - vlanes; \
    যদি (len-i >= vlanes && block_size0 > 0 && block_size0 % vlanes == 0) { \
        UT idxbuf[VTraits<UVT>::max_nlanes]; \
        জন্য (পূর্ণসংখ্যা j = 0; j < vlanes; j++) \
            idxbuf[j] = (UT)j; \
        UVT v_idx0 = vx_load(idxbuf); \
        UVT v_idx_delta = vx_setall_##usuffix((UT)vlanes); \
        UVT v_invalid_idx = vx_setall_##usuffix((UT)-1); \
        VT v_minval = vx_setall_##suffix(minVal); \
        VT v_maxval = vx_setall_##suffix(maxVal); \
        পূর্ণসংখ্যা block_আকার_2 = block_size0; \
        /* process data by blocks: */ \
        /* - for u8/s8 data each block contains up to 256-vlanes elements */ \
        /* - for u16/s16 data each block contains up to 65536-vlanes elements */ \
        /* inside each block we can store the relative (local) index (v_locidx) */ \
        /* in a compact way: 8 bits per lane for u8/s8 data, */ \
        /*                  16 bits per lane for u16/s16 data */ \
        /* 0b111...111 is "invalid index", meaning that this */ \
        /* particular lane has not been updated. */ \
        /* after each block we update minVal, maxVal, minIdx and maxIdx */ \
        জন্য (; i <= len - vlanes; i += block_আকার_2) { \
            block_আকার_2 = std::min(block_আকার_2, (len - i) & -vlanes); \
            UVT v_locidx = v_idx0; \
            UVT v_minidx = v_invalid_idx; \
            UVT v_maxidx = v_invalid_idx; \
            যদি (!mask) { \
                জন্য (পূর্ণসংখ্যা j = 0; j < block_আকার_2; j += vlanes) { \
                    VT তথ্য = vx_load(src + i + j); \
                    UVT lt_min = v_reinterpret_as_##usuffix(v_lt(তথ্য, v_minval)); \
                    UVT gt_max = v_reinterpret_as_##usuffix(v_gt(তথ্য, v_maxval)); \
                    v_minidx = v_select(lt_min, v_locidx, v_minidx); \
                    v_maxidx = v_select(gt_max, v_locidx, v_maxidx); \
                    v_minval = v_min(v_minval, তথ্য); \
                    v_maxval = v_max(v_maxval, তথ্য); \
                    v_locidx = v_add(v_locidx, v_idx_delta); \
                } \
            } অন্যথা { \
                UVT v_zero = vx_setzero_##usuffix(); \
                জন্য (পূর্ণসংখ্যা j = 0; j < block_আকার_2; j += vlanes) { \
                    VT তথ্য = vx_load(src + i + j); \
                    UVT msk = v_ne(load_mask(mask + i + j), v_zero); \
                    UVT lt_min = v_reinterpret_as_##usuffix(v_lt(তথ্য, v_minval)); \
                    UVT gt_max = v_reinterpret_as_##usuffix(v_gt(তথ্য, v_maxval)); \
                    lt_min = v_and(lt_min, msk); \
                    gt_max = v_and(gt_max, msk); \
                    v_minidx = v_select(lt_min, v_locidx, v_minidx); \
                    v_maxidx = v_select(gt_max, v_locidx, v_maxidx); \
                    VT lt_min_তথ্য = v_reinterpret_as_##suffix(lt_min); \
                    VT gt_max_তথ্য = v_reinterpret_as_##suffix(gt_max); \
                    v_minval = v_select(lt_min_তথ্য, তথ্য, v_minval); \
                    v_maxval = v_select(gt_max_তথ্য, তথ্য, v_maxval); \
                    v_locidx = v_add(v_locidx, v_idx_delta); \
                } \
            } \
            /* for both minimum and maximum we check whether global extremum */ \
            /* and its index need to be updated. If yes, we compute */ \
            /* the smallest index within the block where the new global */ \
            /* extremum value occurs */ \
            UVT idxmask = v_ne(v_minidx, v_invalid_idx); \
            যদি (v_check_any(idxmask)) { \
                minVal = (T)v_reduce_min(v_minval); \
                VT invmask = v_ne(v_minval, vx_setall_##suffix(minVal)); \
                v_minidx = v_or(v_minidx, v_reinterpret_as_##usuffix(invmask)); \
                minIdx = startIdx + i + v_reduce_min(v_minidx); \
                v_minval = vx_setall_##suffix(minVal); \
            } \
            idxmask = v_ne(v_maxidx, v_invalid_idx); \
            যদি (v_check_any(idxmask)) { \
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
    যদি (i < len) { \
        src += i; \
        যদি (mask) mask += i; \
        startIdx += i; \
        len -= i; \
        minMaxIdx_(src, mask, _minVal, _maxVal, _minIdx, _maxIdx, len, startIdx); \
    } \
}

// vectorized implementation for s32, f32, f16 and bf16
// (potentially can be extended for u32)
// no need to use blocks here
#undef DEFINE_MINMAXIDX_FUNC
#define DEFINE_MINMAXIDX_FUNC(funcname, suffix, usuffix, T, UT, VT, UVT, WT, load_op) \
স্থির শূন্য funcname(ধ্রুবক T* src, ধ্রুবক uchar* mask, WT* _minVal, WT* _maxVal, \
                     size_t* _minIdx, size_t* _maxIdx, পূর্ণসংখ্যা len, size_t startIdx) \
{ \
    WT minVal = *_minVal, maxVal = *_maxVal; \
    size_t minIdx = *_minIdx, maxIdx = *_maxIdx; \
    পূর্ণসংখ্যা i = 0; \
    /* initialize minVal/maxVal/minIdx/maxIdx to the proper values in the beginning */ \
    যদি (minIdx == 0) { \
        যদি (mask) { \
            i = minMaxInit(mask, len); \
            যদি (i < 0) \
                ফেরত; \
        } \
        minVal = maxVal = src[i]; \
        minIdx = maxIdx = startIdx + i; \
        i++; \
    } \
    SIMD_ONLY( \
    ধ্রুবক পূর্ণসংখ্যা vlanes = VTraits<VT>::vlanes(); \
    UT idxbuf[VTraits<UVT>::max_nlanes]; \
    জন্য (পূর্ণসংখ্যা j = 0; j < vlanes; j++) \
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
    যদি (!mask) { \
        জন্য (; i <= len - vlanes; i += vlanes) { \
            VT তথ্য = load_op(src + i); \
            UVT lt_min = v_reinterpret_as_##usuffix(v_lt(তথ্য, v_minval)); \
            UVT gt_max = v_reinterpret_as_##usuffix(v_gt(তথ্য, v_maxval)); \
            v_minidx = v_select(lt_min, v_locidx, v_minidx); \
            v_maxidx = v_select(gt_max, v_locidx, v_maxidx); \
            v_minval = v_min(v_minval, তথ্য); \
            v_maxval = v_max(v_maxval, তথ্য); \
            v_locidx = v_add(v_locidx, v_idx_delta); \
        } \
    } অন্যথা { \
        UVT v_zero = vx_setzero_##usuffix(); \
        জন্য (; i <= len - vlanes; i += vlanes) { \
            VT তথ্য = load_op(src + i); \
            UVT msk = v_ne(vx_load_expand_q(mask + i), v_zero); \
            UVT lt_min = v_reinterpret_as_##usuffix(v_lt(তথ্য, v_minval)); \
            UVT gt_max = v_reinterpret_as_##usuffix(v_gt(তথ্য, v_maxval)); \
            lt_min = v_and(lt_min, msk); \
            gt_max = v_and(gt_max, msk); \
            v_minidx = v_select(lt_min, v_locidx, v_minidx); \
            v_maxidx = v_select(gt_max, v_locidx, v_maxidx); \
            VT lt_min_তথ্য = v_reinterpret_as_##suffix(lt_min); \
            VT gt_max_তথ্য = v_reinterpret_as_##suffix(gt_max); \
            v_minval = v_select(lt_min_তথ্য, তথ্য, v_minval); \
            v_maxval = v_select(gt_max_তথ্য, তথ্য, v_maxval); \
            v_locidx = v_add(v_locidx, v_idx_delta); \
        } \
    } \
    /* for both minimum and maximum we check whether global extremum */ \
    /* and its index need to be updated. If yes, we compute */ \
    /* the smallest index within the block where the new global */ \
    /* extremum value occurs */ \
    UVT idxmask = v_ne(v_minidx, v_invalid_idx); \
    যদি (v_check_any(idxmask)) { \
        minVal = v_reduce_min(v_minval); \
        VT invmask = v_ne(v_minval, vx_setall_##suffix(minVal)); \
        v_minidx = v_or(v_minidx, v_reinterpret_as_##usuffix(invmask)); \
        minIdx = startIdx + v_reduce_min(v_minidx); \
        v_minval = vx_setall_##suffix(minVal); \
    } \
    idxmask = v_ne(v_maxidx, v_invalid_idx); \
    যদি (v_check_any(idxmask)) { \
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
    যদি (i < len) { \
        src += i; \
        যদি (mask) mask += i; \
        startIdx += i; \
        len -= i; \
        minMaxIdx_(src, mask, _minVal, _maxVal, _minIdx, _maxIdx, len, startIdx); \
    } \
}

#undef DEFINE_MINMAXIDX_FUNC_NOSIMD
#define DEFINE_MINMAXIDX_FUNC_NOSIMD(funcname, T, WT) \
স্থির শূন্য funcname(ধ্রুবক T* src, ধ্রুবক uchar* mask, WT* _minVal, WT* _maxVal, \
                     size_t* _minIdx, size_t* _maxIdx, পূর্ণসংখ্যা len, size_t startIdx) \
{ \
    minMaxIdx_(src, mask, _minVal, _maxVal, _minIdx, _maxIdx, len, startIdx); \
}

DEFINE_MINMAXIDX_SMALLINT_FUNC(minMaxIdx8u, u8, u8, uchar, uchar, v_uint8, v_uint8, পূর্ণসংখ্যা, 256, vx_load)
DEFINE_MINMAXIDX_SMALLINT_FUNC(minMaxIdx8s, s8, u8, schar, uchar, v_int8, v_uint8, পূর্ণসংখ্যা, 256, vx_load)
DEFINE_MINMAXIDX_SMALLINT_FUNC(minMaxIdx16u, u16, u16, ushort, ushort, v_uint16, v_uint16, পূর্ণসংখ্যা, 65536, vx_load_expand)
DEFINE_MINMAXIDX_SMALLINT_FUNC(minMaxIdx16s, s16, u16, short, ushort, v_int16, v_uint16, পূর্ণসংখ্যা, 65536, vx_load_expand)

DEFINE_MINMAXIDX_FUNC(minMaxIdx32s, s32, u32, পূর্ণসংখ্যা, unsigned, v_int32, v_uint32, পূর্ণসংখ্যা, vx_load)
DEFINE_MINMAXIDX_FUNC(minMaxIdx32f, f32, u32, দশমিক, unsigned, v_float32, v_uint32, দশমিক, vx_load)
DEFINE_MINMAXIDX_FUNC(minMaxIdx16f, f32, u32, hfloat, unsigned, v_float32, v_uint32, দশমিক, vx_load_expand)
DEFINE_MINMAXIDX_FUNC(minMaxIdx16bf, f32, u32, bfloat, unsigned, v_float32, v_uint32, দশমিক, vx_load_expand)

//DEFINE_MINMAXIDX_FUNC_NOSIMD(minMaxIdx32s, int, int)
//DEFINE_MINMAXIDX_FUNC_NOSIMD(minMaxIdx32f, float, float)
DEFINE_MINMAXIDX_FUNC_NOSIMD(minMaxIdx64f, দ্বিগুণ, দ্বিগুণ)
//DEFINE_MINMAXIDX_FUNC_NOSIMD(minMaxIdx16f, hfloat, float)
//DEFINE_MINMAXIDX_FUNC_NOSIMD(minMaxIdx16bf, bfloat, float)
DEFINE_MINMAXIDX_FUNC_NOSIMD(minMaxIdx64u, uint64, uint64)
DEFINE_MINMAXIDX_FUNC_NOSIMD(minMaxIdx64s, int64, int64)
DEFINE_MINMAXIDX_FUNC_NOSIMD(minMaxIdx32u, unsigned, int64)

MinMaxIdxFunc getMinMaxIdxFunc(পূর্ণসংখ্যা depth)
{
    স্থির MinMaxIdxFunc minMaxIdxTab[CV_DEPTH_MAX] =
    {
        minMaxIdxWrap<uchar, পূর্ণসংখ্যা, minMaxIdx8u>,
        minMaxIdxWrap<schar, পূর্ণসংখ্যা, minMaxIdx8s>,
        minMaxIdxWrap<ushort, পূর্ণসংখ্যা, minMaxIdx16u>,
        minMaxIdxWrap<short, পূর্ণসংখ্যা, minMaxIdx16s>,
        minMaxIdxWrap<পূর্ণসংখ্যা, পূর্ণসংখ্যা, minMaxIdx32s>,
        minMaxIdxWrap<দশমিক, দশমিক, minMaxIdx32f>,
        minMaxIdxWrap<দ্বিগুণ, দ্বিগুণ, minMaxIdx64f>,
        minMaxIdxWrap<hfloat, দশমিক, minMaxIdx16f>,
        minMaxIdxWrap<bfloat, দশমিক, minMaxIdx16bf>,
        minMaxIdxWrap<uchar, পূর্ণসংখ্যা, minMaxIdx8u>,
        minMaxIdxWrap<uint64, uint64, minMaxIdx64u>,
        minMaxIdxWrap<int64, int64, minMaxIdx64s>,
        minMaxIdxWrap<unsigned, int64, minMaxIdx32u>,
        0
    };

    ফেরত minMaxIdxTab[depth];
}

#endif

CV_CPU_OPTIMIZATION_NAMESPACE_END
} // namespace
