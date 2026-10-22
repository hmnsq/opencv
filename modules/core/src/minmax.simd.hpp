// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html

#شامل "precomp.hpp"

نامخلا cv {

قسمتعریف خالی (*MinMaxIdxFunc)(ثابت uchar* ڈیٹا, ثابت uchar* نقاب,
                              خالی* minval, خالی* maxval,
                              size_t* minidx, size_t* maxidx,
                              صحیح len, size_t startidx);

CV_CPU_OPTIMIZATION_NAMESPACE_BEGIN

سانچہ<قسم_نام T, قسم_نام WT, خالی (*fn)(ثابت T*, ثابت uchar*, WT*, WT*, size_t*, size_t*, صحیح, size_t)>
ساکن خالی minMaxIdxWrap(ثابت uchar* ڈیٹا, ثابت uchar* نقاب,
                          خالی* minval, خالی* maxval,
                          size_t* minidx, size_t* maxidx,
                          صحیح len, size_t startidx)
{
    fn((ثابت T*)ڈیٹا, نقاب, (WT*)minval, (WT*)maxval, minidx, maxidx, len, startidx);
}

MinMaxIdxFunc getMinMaxIdxFunc(صحیح گہرائی);

#اگر_غیر_تعریف_شدہ CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

سانچہ<قسم_نام T, قسم_نام WT> ساکن خالی
minMaxIdx_( ثابت T* src, ثابت uchar* نقاب, WT* _minVal, WT* _maxVal,
            size_t* _minIdx, size_t* _maxIdx, صحیح len, size_t آغاز_Idx )
{
    WT minVal = *_minVal, maxVal = *_maxVal;
    size_t minIdx = *_minIdx, maxIdx = *_maxIdx;
    صحیح i = 0;

    اگر (minIdx == 0 || maxIdx == 0) {
        اگر (نقاب) {
            برائے (; i < len; i++) {
                اگر (نقاب[i]) {
                    minVal = maxVal = (WT)src[i];
                    minIdx = maxIdx = آغاز_Idx + i;
                    i++;
                    توڑو;
                }
            }
        }
        ورنہ اگر (len > 0) {
            minVal = maxVal = (WT)src[0];
            minIdx = maxIdx = آغاز_Idx;
            i++;
        }
    }

    اگر( !نقاب )
    {
        برائے( ; i < len; i++ )
        {
            WT val = (WT)src[i];
            اگر( val < minVal )
            {
                minVal = val;
                minIdx = آغاز_Idx + i;
            }
            اگر( val > maxVal )
            {
                maxVal = val;
                maxIdx = آغاز_Idx + i;
            }
        }
    }
    ورنہ
    {
        برائے( ; i < len; i++ )
        {
            WT val = (WT)src[i];
            uchar m = نقاب[i];
            اگر( m && val < minVal )
            {
                minVal = val;
                minIdx = آغاز_Idx + i;
            }
            اگر( m && val > maxVal )
            {
                maxVal = val;
                maxIdx = آغاز_Idx + i;
            }
        }
    }

    *_minIdx = minIdx;
    *_maxIdx = maxIdx;
    *_minVal = minVal;
    *_maxVal = maxVal;
}

#تعریف_منسوخ SIMD_ONLY
#اگر (CV_SIMD || CV_SIMD_SCALABLE)
#تعریف_کرو SIMD_ONLY(اظہاریہ) اظہاریہ
#ورنہ
#تعریف_کرو SIMD_ONLY(اظہاریہ)
#اگر_ختم

ساکن صحیح minMaxInit(ثابت uchar* نقاب, صحیح len)
{
    صحیح i = 0;
    SIMD_ONLY(
    صحیح vlanes = VTraits<v_uint8>::vlanes();
    v_uint8 v_zero = vx_setzero_u8();
    برائے (; i < len; i += vlanes) {
        اگر (i + vlanes > len) {
            اگر (i == 0)
                توڑو;
            i = len - vlanes;
        }
        v_uint8 نقاب_i = v_ne(vx_بھرو(نقاب + i), v_zero);
        اگر (v_check_any(نقاب_i))
            واپس i + v_جائزہ_forward(نقاب_i);
    })
    برائے (; i < len; i++) {
        اگر (نقاب[i] != 0)
            واپس i;
    }
    واپس -1;
}

// vectorized implementation for u8, s8, u16 and s16
// uses blocks to decrease the lane size necessary to store indices
#تعریف_منسوخ DEFINE_MINMAXIDX_SMALLINT_FUNC
#تعریف_کرو DEFINE_MINMAXIDX_SMALLINT_FUNC(funcname, suffix, usuffix, T, UT, VT, UVT, WT, بلاک_سائز, بھرو_نقاب) \
ساکن خالی funcname(ثابت T* src, ثابت uchar* نقاب, WT* _minVal, WT* _maxVal, \
                     size_t* _minIdx, size_t* _maxIdx, صحیح len, size_t آغاز_Idx) \
{ \
    T minVal = T(*_minVal), maxVal = T(*_maxVal); \
    size_t minIdx = *_minIdx, maxIdx = *_maxIdx; \
    صحیح i = 0; \
    /* initialize minVal/maxVal/minIdx/maxIdx to the proper values in the beginning */ \
    اگر (minIdx == 0) { \
        اگر (نقاب) { \
            i = minMaxInit(نقاب, len); \
            اگر (i < 0) \
                واپس; \
        } \
        minVal = maxVal = src[i]; \
        minIdx = maxIdx = آغاز_Idx + i; \
        i++; \
    } \
    SIMD_ONLY( \
    ثابت صحیح vlanes = VTraits<VT>::vlanes(); \
    ثابت صحیح بلاک_size0 = بلاک_سائز - vlanes; \
    اگر (len-i >= vlanes && بلاک_size0 > 0 && بلاک_size0 % vlanes == 0) { \
        UT idxbuf[VTraits<UVT>::max_nlanes]; \
        برائے (صحیح j = 0; j < vlanes; j++) \
            idxbuf[j] = (UT)j; \
        UVT v_idx0 = vx_بھرو(idxbuf); \
        UVT v_idx_delta = vx_setall_##usuffix((UT)vlanes); \
        UVT v_invalid_idx = vx_setall_##usuffix((UT)-1); \
        VT v_minval = vx_setall_##suffix(minVal); \
        VT v_maxval = vx_setall_##suffix(maxVal); \
        صحیح بلاک_سائز_3 = بلاک_size0; \
        /* process data by blocks: */ \
        /* - for u8/s8 data each block contains up to 256-vlanes elements */ \
        /* - for u16/s16 data each block contains up to 65536-vlanes elements */ \
        /* inside each block we can store the relative (local) index (v_locidx) */ \
        /* in a compact way: 8 bits per lane for u8/s8 data, */ \
        /*                  16 bits per lane for u16/s16 data */ \
        /* 0b111...111 is "invalid index", meaning that this */ \
        /* particular lane has not been updated. */ \
        /* after each block we update minVal, maxVal, minIdx and maxIdx */ \
        برائے (; i <= len - vlanes; i += بلاک_سائز_3) { \
            بلاک_سائز_3 = std::min(بلاک_سائز_3, (len - i) & -vlanes); \
            UVT v_locidx = v_idx0; \
            UVT v_minidx = v_invalid_idx; \
            UVT v_maxidx = v_invalid_idx; \
            اگر (!نقاب) { \
                برائے (صحیح j = 0; j < بلاک_سائز_3; j += vlanes) { \
                    VT ڈیٹا = vx_بھرو(src + i + j); \
                    UVT lt_min = v_reinterpret_as_##usuffix(v_lt(ڈیٹا, v_minval)); \
                    UVT gt_max = v_reinterpret_as_##usuffix(v_gt(ڈیٹا, v_maxval)); \
                    v_minidx = v_select(lt_min, v_locidx, v_minidx); \
                    v_maxidx = v_select(gt_max, v_locidx, v_maxidx); \
                    v_minval = v_min(v_minval, ڈیٹا); \
                    v_maxval = v_max(v_maxval, ڈیٹا); \
                    v_locidx = v_جوڑو(v_locidx, v_idx_delta); \
                } \
            } ورنہ { \
                UVT v_zero = vx_setzero_##usuffix(); \
                برائے (صحیح j = 0; j < بلاک_سائز_3; j += vlanes) { \
                    VT ڈیٹا = vx_بھرو(src + i + j); \
                    UVT msk = v_ne(بھرو_نقاب(نقاب + i + j), v_zero); \
                    UVT lt_min = v_reinterpret_as_##usuffix(v_lt(ڈیٹا, v_minval)); \
                    UVT gt_max = v_reinterpret_as_##usuffix(v_gt(ڈیٹا, v_maxval)); \
                    lt_min = v_and(lt_min, msk); \
                    gt_max = v_and(gt_max, msk); \
                    v_minidx = v_select(lt_min, v_locidx, v_minidx); \
                    v_maxidx = v_select(gt_max, v_locidx, v_maxidx); \
                    VT lt_min_ڈیٹا = v_reinterpret_as_##suffix(lt_min); \
                    VT gt_max_ڈیٹا = v_reinterpret_as_##suffix(gt_max); \
                    v_minval = v_select(lt_min_ڈیٹا, ڈیٹا, v_minval); \
                    v_maxval = v_select(gt_max_ڈیٹا, ڈیٹا, v_maxval); \
                    v_locidx = v_جوڑو(v_locidx, v_idx_delta); \
                } \
            } \
            /* for both minimum and maximum we check whether global extremum */ \
            /* and its index need to be updated. If yes, we compute */ \
            /* the smallest index within the block where the new global */ \
            /* extremum value occurs */ \
            UVT idxmask = v_ne(v_minidx, v_invalid_idx); \
            اگر (v_check_any(idxmask)) { \
                minVal = (T)v_reduce_min(v_minval); \
                VT invmask = v_ne(v_minval, vx_setall_##suffix(minVal)); \
                v_minidx = v_or(v_minidx, v_reinterpret_as_##usuffix(invmask)); \
                minIdx = آغاز_Idx + i + v_reduce_min(v_minidx); \
                v_minval = vx_setall_##suffix(minVal); \
            } \
            idxmask = v_ne(v_maxidx, v_invalid_idx); \
            اگر (v_check_any(idxmask)) { \
                maxVal = (T)v_reduce_max(v_maxval); \
                VT invmask = v_ne(v_maxval, vx_setall_##suffix(maxVal)); \
                v_maxidx = v_or(v_maxidx, v_reinterpret_as_##usuffix(invmask)); \
                maxIdx = آغاز_Idx + i + v_reduce_min(v_maxidx); \
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
    اگر (i < len) { \
        src += i; \
        اگر (نقاب) نقاب += i; \
        آغاز_Idx += i; \
        len -= i; \
        minMaxIdx_(src, نقاب, _minVal, _maxVal, _minIdx, _maxIdx, len, آغاز_Idx); \
    } \
}

// vectorized implementation for s32, f32, f16 and bf16
// (potentially can be extended for u32)
// no need to use blocks here
#تعریف_منسوخ DEFINE_MINMAXIDX_FUNC
#تعریف_کرو DEFINE_MINMAXIDX_FUNC(funcname, suffix, usuffix, T, UT, VT, UVT, WT, بھرو_op) \
ساکن خالی funcname(ثابت T* src, ثابت uchar* نقاب, WT* _minVal, WT* _maxVal, \
                     size_t* _minIdx, size_t* _maxIdx, صحیح len, size_t آغاز_Idx) \
{ \
    WT minVal = *_minVal, maxVal = *_maxVal; \
    size_t minIdx = *_minIdx, maxIdx = *_maxIdx; \
    صحیح i = 0; \
    /* initialize minVal/maxVal/minIdx/maxIdx to the proper values in the beginning */ \
    اگر (minIdx == 0) { \
        اگر (نقاب) { \
            i = minMaxInit(نقاب, len); \
            اگر (i < 0) \
                واپس; \
        } \
        minVal = maxVal = src[i]; \
        minIdx = maxIdx = آغاز_Idx + i; \
        i++; \
    } \
    SIMD_ONLY( \
    ثابت صحیح vlanes = VTraits<VT>::vlanes(); \
    UT idxbuf[VTraits<UVT>::max_nlanes]; \
    برائے (صحیح j = 0; j < vlanes; j++) \
        idxbuf[j] = (UT)(i+j); \
    UVT v_locidx = vx_بھرو(idxbuf); \
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
    اگر (!نقاب) { \
        برائے (; i <= len - vlanes; i += vlanes) { \
            VT ڈیٹا = بھرو_op(src + i); \
            UVT lt_min = v_reinterpret_as_##usuffix(v_lt(ڈیٹا, v_minval)); \
            UVT gt_max = v_reinterpret_as_##usuffix(v_gt(ڈیٹا, v_maxval)); \
            v_minidx = v_select(lt_min, v_locidx, v_minidx); \
            v_maxidx = v_select(gt_max, v_locidx, v_maxidx); \
            v_minval = v_min(v_minval, ڈیٹا); \
            v_maxval = v_max(v_maxval, ڈیٹا); \
            v_locidx = v_جوڑو(v_locidx, v_idx_delta); \
        } \
    } ورنہ { \
        UVT v_zero = vx_setzero_##usuffix(); \
        برائے (; i <= len - vlanes; i += vlanes) { \
            VT ڈیٹا = بھرو_op(src + i); \
            UVT msk = v_ne(vx_بھرو_expand_q(نقاب + i), v_zero); \
            UVT lt_min = v_reinterpret_as_##usuffix(v_lt(ڈیٹا, v_minval)); \
            UVT gt_max = v_reinterpret_as_##usuffix(v_gt(ڈیٹا, v_maxval)); \
            lt_min = v_and(lt_min, msk); \
            gt_max = v_and(gt_max, msk); \
            v_minidx = v_select(lt_min, v_locidx, v_minidx); \
            v_maxidx = v_select(gt_max, v_locidx, v_maxidx); \
            VT lt_min_ڈیٹا = v_reinterpret_as_##suffix(lt_min); \
            VT gt_max_ڈیٹا = v_reinterpret_as_##suffix(gt_max); \
            v_minval = v_select(lt_min_ڈیٹا, ڈیٹا, v_minval); \
            v_maxval = v_select(gt_max_ڈیٹا, ڈیٹا, v_maxval); \
            v_locidx = v_جوڑو(v_locidx, v_idx_delta); \
        } \
    } \
    /* for both minimum and maximum we check whether global extremum */ \
    /* and its index need to be updated. If yes, we compute */ \
    /* the smallest index within the block where the new global */ \
    /* extremum value occurs */ \
    UVT idxmask = v_ne(v_minidx, v_invalid_idx); \
    اگر (v_check_any(idxmask)) { \
        minVal = v_reduce_min(v_minval); \
        VT invmask = v_ne(v_minval, vx_setall_##suffix(minVal)); \
        v_minidx = v_or(v_minidx, v_reinterpret_as_##usuffix(invmask)); \
        minIdx = آغاز_Idx + v_reduce_min(v_minidx); \
        v_minval = vx_setall_##suffix(minVal); \
    } \
    idxmask = v_ne(v_maxidx, v_invalid_idx); \
    اگر (v_check_any(idxmask)) { \
        maxVal = v_reduce_max(v_maxval); \
        VT invmask = v_ne(v_maxval, vx_setall_##suffix(maxVal)); \
        v_maxidx = v_or(v_maxidx, v_reinterpret_as_##usuffix(invmask)); \
        maxIdx = آغاز_Idx + v_reduce_min(v_maxidx); \
        v_maxval = vx_setall_##suffix(maxVal); \
    }) \
    *_minVal = minVal; \
    *_maxVal = maxVal; \
    *_minIdx = minIdx; \
    *_maxIdx = maxIdx; \
    /* [TODO]: unlike sum, countNonZero and other reduce operations, */ \
    /* in the case of minMaxIdx we can process the tail using */ \
    /* vector overlapping technique (as in arithmetic operations) */ \
    اگر (i < len) { \
        src += i; \
        اگر (نقاب) نقاب += i; \
        آغاز_Idx += i; \
        len -= i; \
        minMaxIdx_(src, نقاب, _minVal, _maxVal, _minIdx, _maxIdx, len, آغاز_Idx); \
    } \
}

#تعریف_منسوخ DEFINE_MINMAXIDX_FUNC_NOSIMD
#تعریف_کرو DEFINE_MINMAXIDX_FUNC_NOSIMD(funcname, T, WT) \
ساکن خالی funcname(ثابت T* src, ثابت uchar* نقاب, WT* _minVal, WT* _maxVal, \
                     size_t* _minIdx, size_t* _maxIdx, صحیح len, size_t آغاز_Idx) \
{ \
    minMaxIdx_(src, نقاب, _minVal, _maxVal, _minIdx, _maxIdx, len, آغاز_Idx); \
}

DEFINE_MINMAXIDX_SMALLINT_FUNC(minMaxIdx8u, u8, u8, uchar, uchar, v_uint8, v_uint8, صحیح, 256, vx_بھرو)
DEFINE_MINMAXIDX_SMALLINT_FUNC(minMaxIdx8s, s8, u8, schar, uchar, v_int8, v_uint8, صحیح, 256, vx_بھرو)
DEFINE_MINMAXIDX_SMALLINT_FUNC(minMaxIdx16u, u16, u16, ushort, ushort, v_uint16, v_uint16, صحیح, 65536, vx_بھرو_expand)
DEFINE_MINMAXIDX_SMALLINT_FUNC(minMaxIdx16s, s16, u16, مختصر, ushort, v_int16, v_uint16, صحیح, 65536, vx_بھرو_expand)

DEFINE_MINMAXIDX_FUNC(minMaxIdx32s, s32, u32, صحیح, غیرعلامتی, v_int32, v_uint32, صحیح, vx_بھرو)
DEFINE_MINMAXIDX_FUNC(minMaxIdx32f, f32, u32, اعشاری, غیرعلامتی, v_float32, v_uint32, اعشاری, vx_بھرو)
DEFINE_MINMAXIDX_FUNC(minMaxIdx16f, f32, u32, hfloat, غیرعلامتی, v_float32, v_uint32, اعشاری, vx_بھرو_expand)
DEFINE_MINMAXIDX_FUNC(minMaxIdx16bf, f32, u32, bfloat, غیرعلامتی, v_float32, v_uint32, اعشاری, vx_بھرو_expand)

//DEFINE_MINMAXIDX_FUNC_NOSIMD(minMaxIdx32s, int, int)
//DEFINE_MINMAXIDX_FUNC_NOSIMD(minMaxIdx32f, float, float)
DEFINE_MINMAXIDX_FUNC_NOSIMD(minMaxIdx64f, دوہرا, دوہرا)
//DEFINE_MINMAXIDX_FUNC_NOSIMD(minMaxIdx16f, hfloat, float)
//DEFINE_MINMAXIDX_FUNC_NOSIMD(minMaxIdx16bf, bfloat, float)
DEFINE_MINMAXIDX_FUNC_NOSIMD(minMaxIdx64u, uint64, uint64)
DEFINE_MINMAXIDX_FUNC_NOSIMD(minMaxIdx64s, int64, int64)
DEFINE_MINMAXIDX_FUNC_NOSIMD(minMaxIdx32u, غیرعلامتی, int64)

MinMaxIdxFunc getMinMaxIdxFunc(صحیح گہرائی)
{
    ساکن MinMaxIdxFunc minMaxIdxTab[CV_گہرائی_MAX] =
    {
        minMaxIdxWrap<uchar, صحیح, minMaxIdx8u>,
        minMaxIdxWrap<schar, صحیح, minMaxIdx8s>,
        minMaxIdxWrap<ushort, صحیح, minMaxIdx16u>,
        minMaxIdxWrap<مختصر, صحیح, minMaxIdx16s>,
        minMaxIdxWrap<صحیح, صحیح, minMaxIdx32s>,
        minMaxIdxWrap<اعشاری, اعشاری, minMaxIdx32f>,
        minMaxIdxWrap<دوہرا, دوہرا, minMaxIdx64f>,
        minMaxIdxWrap<hfloat, اعشاری, minMaxIdx16f>,
        minMaxIdxWrap<bfloat, اعشاری, minMaxIdx16bf>,
        minMaxIdxWrap<uchar, صحیح, minMaxIdx8u>,
        minMaxIdxWrap<uint64, uint64, minMaxIdx64u>,
        minMaxIdxWrap<int64, int64, minMaxIdx64s>,
        minMaxIdxWrap<غیرعلامتی, int64, minMaxIdx32u>,
        0
    };

    واپس minMaxIdxTab[گہرائی];
}

#اگر_ختم

CV_CPU_OPTIMIZATION_NAMESPACE_انتہا
} // namespace
