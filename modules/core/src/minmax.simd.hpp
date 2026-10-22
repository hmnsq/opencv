// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html

#అన్తర్భావ "precomp.hpp"

నామాకాశ cv {

రకనిర్వచనం శూన్య (*MinMaxIdxFunc)(స్థిరరాశి uchar* దత్తాంశం, స్థిరరాశి uchar* ముసుగు,
                              శూన్య* minval, శూన్య* maxval,
                              size_t* minidx, size_t* maxidx,
                              పూర్ణాంకం len, size_t startidx);

CV_CPU_OPTIMIZATION_NAMESPACE_BEGIN

మూస<రకం_పేరు T, రకం_పేరు WT, శూన్య (*fn)(స్థిరరాశి T*, స్థిరరాశి uchar*, WT*, WT*, size_t*, size_t*, పూర్ణాంకం, size_t)>
స్థిర శూన్య minMaxIdxWrap(స్థిరరాశి uchar* దత్తాంశం, స్థిరరాశి uchar* ముసుగు,
                          శూన్య* minval, శూన్య* maxval,
                          size_t* minidx, size_t* maxidx,
                          పూర్ణాంకం len, size_t startidx)
{
    fn((స్థిరరాశి T*)దత్తాంశం, ముసుగు, (WT*)minval, (WT*)maxval, minidx, maxidx, len, startidx);
}

MinMaxIdxFunc getMinMaxIdxFunc(పూర్ణాంకం లోతు);

#నిర్వచితం_కాకపోతే CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

మూస<రకం_పేరు T, రకం_పేరు WT> స్థిర శూన్య
minMaxIdx_( స్థిరరాశి T* src, స్థిరరాశి uchar* ముసుగు, WT* _minVal, WT* _maxVal,
            size_t* _minIdx, size_t* _maxIdx, పూర్ణాంకం len, size_t ప్రారంభం_Idx )
{
    WT minVal = *_minVal, maxVal = *_maxVal;
    size_t minIdx = *_minIdx, maxIdx = *_maxIdx;
    పూర్ణాంకం i = 0;

    ఒకవేళ (minIdx == 0 || maxIdx == 0) {
        ఒకవేళ (ముసుగు) {
            కొరకు (; i < len; i++) {
                ఒకవేళ (ముసుగు[i]) {
                    minVal = maxVal = (WT)src[i];
                    minIdx = maxIdx = ప్రారంభం_Idx + i;
                    i++;
                    ఆపు;
                }
            }
        }
        లేకపోతే ఒకవేళ (len > 0) {
            minVal = maxVal = (WT)src[0];
            minIdx = maxIdx = ప్రారంభం_Idx;
            i++;
        }
    }

    ఒకవేళ( !ముసుగు )
    {
        కొరకు( ; i < len; i++ )
        {
            WT val = (WT)src[i];
            ఒకవేళ( val < minVal )
            {
                minVal = val;
                minIdx = ప్రారంభం_Idx + i;
            }
            ఒకవేళ( val > maxVal )
            {
                maxVal = val;
                maxIdx = ప్రారంభం_Idx + i;
            }
        }
    }
    లేకపోతే
    {
        కొరకు( ; i < len; i++ )
        {
            WT val = (WT)src[i];
            uchar m = ముసుగు[i];
            ఒకవేళ( m && val < minVal )
            {
                minVal = val;
                minIdx = ప్రారంభం_Idx + i;
            }
            ఒకవేళ( m && val > maxVal )
            {
                maxVal = val;
                maxIdx = ప్రారంభం_Idx + i;
            }
        }
    }

    *_minIdx = minIdx;
    *_maxIdx = maxIdx;
    *_minVal = minVal;
    *_maxVal = maxVal;
}

#నిర్వచనం_రద్దు SIMD_ONLY
#ఒకవేళ (CV_SIMD || CV_SIMD_SCALABLE)
#నిర్వచించు SIMD_ONLY(సమాసం) సమాసం
#లేకపోతే
#నిర్వచించు SIMD_ONLY(సమాసం)
#అయితే_ముగింపు

స్థిర పూర్ణాంకం minMaxInit(స్థిరరాశి uchar* ముసుగు, పూర్ణాంకం len)
{
    పూర్ణాంకం i = 0;
    SIMD_ONLY(
    పూర్ణాంకం vlanes = VTraits<v_uint8>::vlanes();
    v_uint8 v_zero = vx_setzero_u8();
    కొరకు (; i < len; i += vlanes) {
        ఒకవేళ (i + vlanes > len) {
            ఒకవేళ (i == 0)
                ఆపు;
            i = len - vlanes;
        }
        v_uint8 ముసుగు_i = v_ne(vx_ఎక్కించు(ముసుగు + i), v_zero);
        ఒకవేళ (v_check_any(ముసుగు_i))
            తిరిగి i + v_పరిశీలన_forward(ముసుగు_i);
    })
    కొరకు (; i < len; i++) {
        ఒకవేళ (ముసుగు[i] != 0)
            తిరిగి i;
    }
    తిరిగి -1;
}

// vectorized implementation for u8, s8, u16 and s16
// uses blocks to decrease the lane size necessary to store indices
#నిర్వచనం_రద్దు DEFINE_MINMAXIDX_SMALLINT_FUNC
#నిర్వచించు DEFINE_MINMAXIDX_SMALLINT_FUNC(funcname, suffix, usuffix, T, UT, VT, UVT, WT, ఖండం_కొలత, ఎక్కించు_ముసుగు) \
స్థిర శూన్య funcname(స్థిరరాశి T* src, స్థిరరాశి uchar* ముసుగు, WT* _minVal, WT* _maxVal, \
                     size_t* _minIdx, size_t* _maxIdx, పూర్ణాంకం len, size_t ప్రారంభం_Idx) \
{ \
    T minVal = T(*_minVal), maxVal = T(*_maxVal); \
    size_t minIdx = *_minIdx, maxIdx = *_maxIdx; \
    పూర్ణాంకం i = 0; \
    /* initialize minVal/maxVal/minIdx/maxIdx to the proper values in the beginning */ \
    ఒకవేళ (minIdx == 0) { \
        ఒకవేళ (ముసుగు) { \
            i = minMaxInit(ముసుగు, len); \
            ఒకవేళ (i < 0) \
                తిరిగి; \
        } \
        minVal = maxVal = src[i]; \
        minIdx = maxIdx = ప్రారంభం_Idx + i; \
        i++; \
    } \
    SIMD_ONLY( \
    స్థిరరాశి పూర్ణాంకం vlanes = VTraits<VT>::vlanes(); \
    స్థిరరాశి పూర్ణాంకం ఖండం_size0 = ఖండం_కొలత - vlanes; \
    ఒకవేళ (len-i >= vlanes && ఖండం_size0 > 0 && ఖండం_size0 % vlanes == 0) { \
        UT idxbuf[VTraits<UVT>::max_nlanes]; \
        కొరకు (పూర్ణాంకం j = 0; j < vlanes; j++) \
            idxbuf[j] = (UT)j; \
        UVT v_idx0 = vx_ఎక్కించు(idxbuf); \
        UVT v_idx_delta = vx_setall_##usuffix((UT)vlanes); \
        UVT v_invalid_idx = vx_setall_##usuffix((UT)-1); \
        VT v_minval = vx_setall_##suffix(minVal); \
        VT v_maxval = vx_setall_##suffix(maxVal); \
        పూర్ణాంకం ఖండం_కొలత_3 = ఖండం_size0; \
        /* process data by blocks: */ \
        /* - for u8/s8 data each block contains up to 256-vlanes elements */ \
        /* - for u16/s16 data each block contains up to 65536-vlanes elements */ \
        /* inside each block we can store the relative (local) index (v_locidx) */ \
        /* in a compact way: 8 bits per lane for u8/s8 data, */ \
        /*                  16 bits per lane for u16/s16 data */ \
        /* 0b111...111 is "invalid index", meaning that this */ \
        /* particular lane has not been updated. */ \
        /* after each block we update minVal, maxVal, minIdx and maxIdx */ \
        కొరకు (; i <= len - vlanes; i += ఖండం_కొలత_3) { \
            ఖండం_కొలత_3 = std::min(ఖండం_కొలత_3, (len - i) & -vlanes); \
            UVT v_locidx = v_idx0; \
            UVT v_minidx = v_invalid_idx; \
            UVT v_maxidx = v_invalid_idx; \
            ఒకవేళ (!ముసుగు) { \
                కొరకు (పూర్ణాంకం j = 0; j < ఖండం_కొలత_3; j += vlanes) { \
                    VT దత్తాంశం = vx_ఎక్కించు(src + i + j); \
                    UVT lt_min = v_reinterpret_as_##usuffix(v_lt(దత్తాంశం, v_minval)); \
                    UVT gt_max = v_reinterpret_as_##usuffix(v_gt(దత్తాంశం, v_maxval)); \
                    v_minidx = v_select(lt_min, v_locidx, v_minidx); \
                    v_maxidx = v_select(gt_max, v_locidx, v_maxidx); \
                    v_minval = v_min(v_minval, దత్తాంశం); \
                    v_maxval = v_max(v_maxval, దత్తాంశం); \
                    v_locidx = v_కూడు(v_locidx, v_idx_delta); \
                } \
            } లేకపోతే { \
                UVT v_zero = vx_setzero_##usuffix(); \
                కొరకు (పూర్ణాంకం j = 0; j < ఖండం_కొలత_3; j += vlanes) { \
                    VT దత్తాంశం = vx_ఎక్కించు(src + i + j); \
                    UVT msk = v_ne(ఎక్కించు_ముసుగు(ముసుగు + i + j), v_zero); \
                    UVT lt_min = v_reinterpret_as_##usuffix(v_lt(దత్తాంశం, v_minval)); \
                    UVT gt_max = v_reinterpret_as_##usuffix(v_gt(దత్తాంశం, v_maxval)); \
                    lt_min = v_and(lt_min, msk); \
                    gt_max = v_and(gt_max, msk); \
                    v_minidx = v_select(lt_min, v_locidx, v_minidx); \
                    v_maxidx = v_select(gt_max, v_locidx, v_maxidx); \
                    VT lt_min_దత్తాంశం = v_reinterpret_as_##suffix(lt_min); \
                    VT gt_max_దత్తాంశం = v_reinterpret_as_##suffix(gt_max); \
                    v_minval = v_select(lt_min_దత్తాంశం, దత్తాంశం, v_minval); \
                    v_maxval = v_select(gt_max_దత్తాంశం, దత్తాంశం, v_maxval); \
                    v_locidx = v_కూడు(v_locidx, v_idx_delta); \
                } \
            } \
            /* for both minimum and maximum we check whether global extremum */ \
            /* and its index need to be updated. If yes, we compute */ \
            /* the smallest index within the block where the new global */ \
            /* extremum value occurs */ \
            UVT idxmask = v_ne(v_minidx, v_invalid_idx); \
            ఒకవేళ (v_check_any(idxmask)) { \
                minVal = (T)v_reduce_min(v_minval); \
                VT invmask = v_ne(v_minval, vx_setall_##suffix(minVal)); \
                v_minidx = v_or(v_minidx, v_reinterpret_as_##usuffix(invmask)); \
                minIdx = ప్రారంభం_Idx + i + v_reduce_min(v_minidx); \
                v_minval = vx_setall_##suffix(minVal); \
            } \
            idxmask = v_ne(v_maxidx, v_invalid_idx); \
            ఒకవేళ (v_check_any(idxmask)) { \
                maxVal = (T)v_reduce_max(v_maxval); \
                VT invmask = v_ne(v_maxval, vx_setall_##suffix(maxVal)); \
                v_maxidx = v_or(v_maxidx, v_reinterpret_as_##usuffix(invmask)); \
                maxIdx = ప్రారంభం_Idx + i + v_reduce_min(v_maxidx); \
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
    ఒకవేళ (i < len) { \
        src += i; \
        ఒకవేళ (ముసుగు) ముసుగు += i; \
        ప్రారంభం_Idx += i; \
        len -= i; \
        minMaxIdx_(src, ముసుగు, _minVal, _maxVal, _minIdx, _maxIdx, len, ప్రారంభం_Idx); \
    } \
}

// vectorized implementation for s32, f32, f16 and bf16
// (potentially can be extended for u32)
// no need to use blocks here
#నిర్వచనం_రద్దు DEFINE_MINMAXIDX_FUNC
#నిర్వచించు DEFINE_MINMAXIDX_FUNC(funcname, suffix, usuffix, T, UT, VT, UVT, WT, ఎక్కించు_op) \
స్థిర శూన్య funcname(స్థిరరాశి T* src, స్థిరరాశి uchar* ముసుగు, WT* _minVal, WT* _maxVal, \
                     size_t* _minIdx, size_t* _maxIdx, పూర్ణాంకం len, size_t ప్రారంభం_Idx) \
{ \
    WT minVal = *_minVal, maxVal = *_maxVal; \
    size_t minIdx = *_minIdx, maxIdx = *_maxIdx; \
    పూర్ణాంకం i = 0; \
    /* initialize minVal/maxVal/minIdx/maxIdx to the proper values in the beginning */ \
    ఒకవేళ (minIdx == 0) { \
        ఒకవేళ (ముసుగు) { \
            i = minMaxInit(ముసుగు, len); \
            ఒకవేళ (i < 0) \
                తిరిగి; \
        } \
        minVal = maxVal = src[i]; \
        minIdx = maxIdx = ప్రారంభం_Idx + i; \
        i++; \
    } \
    SIMD_ONLY( \
    స్థిరరాశి పూర్ణాంకం vlanes = VTraits<VT>::vlanes(); \
    UT idxbuf[VTraits<UVT>::max_nlanes]; \
    కొరకు (పూర్ణాంకం j = 0; j < vlanes; j++) \
        idxbuf[j] = (UT)(i+j); \
    UVT v_locidx = vx_ఎక్కించు(idxbuf); \
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
    ఒకవేళ (!ముసుగు) { \
        కొరకు (; i <= len - vlanes; i += vlanes) { \
            VT దత్తాంశం = ఎక్కించు_op(src + i); \
            UVT lt_min = v_reinterpret_as_##usuffix(v_lt(దత్తాంశం, v_minval)); \
            UVT gt_max = v_reinterpret_as_##usuffix(v_gt(దత్తాంశం, v_maxval)); \
            v_minidx = v_select(lt_min, v_locidx, v_minidx); \
            v_maxidx = v_select(gt_max, v_locidx, v_maxidx); \
            v_minval = v_min(v_minval, దత్తాంశం); \
            v_maxval = v_max(v_maxval, దత్తాంశం); \
            v_locidx = v_కూడు(v_locidx, v_idx_delta); \
        } \
    } లేకపోతే { \
        UVT v_zero = vx_setzero_##usuffix(); \
        కొరకు (; i <= len - vlanes; i += vlanes) { \
            VT దత్తాంశం = ఎక్కించు_op(src + i); \
            UVT msk = v_ne(vx_ఎక్కించు_expand_q(ముసుగు + i), v_zero); \
            UVT lt_min = v_reinterpret_as_##usuffix(v_lt(దత్తాంశం, v_minval)); \
            UVT gt_max = v_reinterpret_as_##usuffix(v_gt(దత్తాంశం, v_maxval)); \
            lt_min = v_and(lt_min, msk); \
            gt_max = v_and(gt_max, msk); \
            v_minidx = v_select(lt_min, v_locidx, v_minidx); \
            v_maxidx = v_select(gt_max, v_locidx, v_maxidx); \
            VT lt_min_దత్తాంశం = v_reinterpret_as_##suffix(lt_min); \
            VT gt_max_దత్తాంశం = v_reinterpret_as_##suffix(gt_max); \
            v_minval = v_select(lt_min_దత్తాంశం, దత్తాంశం, v_minval); \
            v_maxval = v_select(gt_max_దత్తాంశం, దత్తాంశం, v_maxval); \
            v_locidx = v_కూడు(v_locidx, v_idx_delta); \
        } \
    } \
    /* for both minimum and maximum we check whether global extremum */ \
    /* and its index need to be updated. If yes, we compute */ \
    /* the smallest index within the block where the new global */ \
    /* extremum value occurs */ \
    UVT idxmask = v_ne(v_minidx, v_invalid_idx); \
    ఒకవేళ (v_check_any(idxmask)) { \
        minVal = v_reduce_min(v_minval); \
        VT invmask = v_ne(v_minval, vx_setall_##suffix(minVal)); \
        v_minidx = v_or(v_minidx, v_reinterpret_as_##usuffix(invmask)); \
        minIdx = ప్రారంభం_Idx + v_reduce_min(v_minidx); \
        v_minval = vx_setall_##suffix(minVal); \
    } \
    idxmask = v_ne(v_maxidx, v_invalid_idx); \
    ఒకవేళ (v_check_any(idxmask)) { \
        maxVal = v_reduce_max(v_maxval); \
        VT invmask = v_ne(v_maxval, vx_setall_##suffix(maxVal)); \
        v_maxidx = v_or(v_maxidx, v_reinterpret_as_##usuffix(invmask)); \
        maxIdx = ప్రారంభం_Idx + v_reduce_min(v_maxidx); \
        v_maxval = vx_setall_##suffix(maxVal); \
    }) \
    *_minVal = minVal; \
    *_maxVal = maxVal; \
    *_minIdx = minIdx; \
    *_maxIdx = maxIdx; \
    /* [TODO]: unlike sum, countNonZero and other reduce operations, */ \
    /* in the case of minMaxIdx we can process the tail using */ \
    /* vector overlapping technique (as in arithmetic operations) */ \
    ఒకవేళ (i < len) { \
        src += i; \
        ఒకవేళ (ముసుగు) ముసుగు += i; \
        ప్రారంభం_Idx += i; \
        len -= i; \
        minMaxIdx_(src, ముసుగు, _minVal, _maxVal, _minIdx, _maxIdx, len, ప్రారంభం_Idx); \
    } \
}

#నిర్వచనం_రద్దు DEFINE_MINMAXIDX_FUNC_NOSIMD
#నిర్వచించు DEFINE_MINMAXIDX_FUNC_NOSIMD(funcname, T, WT) \
స్థిర శూన్య funcname(స్థిరరాశి T* src, స్థిరరాశి uchar* ముసుగు, WT* _minVal, WT* _maxVal, \
                     size_t* _minIdx, size_t* _maxIdx, పూర్ణాంకం len, size_t ప్రారంభం_Idx) \
{ \
    minMaxIdx_(src, ముసుగు, _minVal, _maxVal, _minIdx, _maxIdx, len, ప్రారంభం_Idx); \
}

DEFINE_MINMAXIDX_SMALLINT_FUNC(minMaxIdx8u, u8, u8, uchar, uchar, v_uint8, v_uint8, పూర్ణాంకం, 256, vx_ఎక్కించు)
DEFINE_MINMAXIDX_SMALLINT_FUNC(minMaxIdx8s, s8, u8, schar, uchar, v_int8, v_uint8, పూర్ణాంకం, 256, vx_ఎక్కించు)
DEFINE_MINMAXIDX_SMALLINT_FUNC(minMaxIdx16u, u16, u16, ushort, ushort, v_uint16, v_uint16, పూర్ణాంకం, 65536, vx_ఎక్కించు_expand)
DEFINE_MINMAXIDX_SMALLINT_FUNC(minMaxIdx16s, s16, u16, పొట్టి, ushort, v_int16, v_uint16, పూర్ణాంకం, 65536, vx_ఎక్కించు_expand)

DEFINE_MINMAXIDX_FUNC(minMaxIdx32s, s32, u32, పూర్ణాంకం, గుర్తులేని, v_int32, v_uint32, పూర్ణాంకం, vx_ఎక్కించు)
DEFINE_MINMAXIDX_FUNC(minMaxIdx32f, f32, u32, దశాంశం, గుర్తులేని, v_float32, v_uint32, దశాంశం, vx_ఎక్కించు)
DEFINE_MINMAXIDX_FUNC(minMaxIdx16f, f32, u32, hfloat, గుర్తులేని, v_float32, v_uint32, దశాంశం, vx_ఎక్కించు_expand)
DEFINE_MINMAXIDX_FUNC(minMaxIdx16bf, f32, u32, bfloat, గుర్తులేని, v_float32, v_uint32, దశాంశం, vx_ఎక్కించు_expand)

//DEFINE_MINMAXIDX_FUNC_NOSIMD(minMaxIdx32s, int, int)
//DEFINE_MINMAXIDX_FUNC_NOSIMD(minMaxIdx32f, float, float)
DEFINE_MINMAXIDX_FUNC_NOSIMD(minMaxIdx64f, ద్విగుణ, ద్విగుణ)
//DEFINE_MINMAXIDX_FUNC_NOSIMD(minMaxIdx16f, hfloat, float)
//DEFINE_MINMAXIDX_FUNC_NOSIMD(minMaxIdx16bf, bfloat, float)
DEFINE_MINMAXIDX_FUNC_NOSIMD(minMaxIdx64u, uint64, uint64)
DEFINE_MINMAXIDX_FUNC_NOSIMD(minMaxIdx64s, int64, int64)
DEFINE_MINMAXIDX_FUNC_NOSIMD(minMaxIdx32u, గుర్తులేని, int64)

MinMaxIdxFunc getMinMaxIdxFunc(పూర్ణాంకం లోతు)
{
    స్థిర MinMaxIdxFunc minMaxIdxTab[CV_లోతు_MAX] =
    {
        minMaxIdxWrap<uchar, పూర్ణాంకం, minMaxIdx8u>,
        minMaxIdxWrap<schar, పూర్ణాంకం, minMaxIdx8s>,
        minMaxIdxWrap<ushort, పూర్ణాంకం, minMaxIdx16u>,
        minMaxIdxWrap<పొట్టి, పూర్ణాంకం, minMaxIdx16s>,
        minMaxIdxWrap<పూర్ణాంకం, పూర్ణాంకం, minMaxIdx32s>,
        minMaxIdxWrap<దశాంశం, దశాంశం, minMaxIdx32f>,
        minMaxIdxWrap<ద్విగుణ, ద్విగుణ, minMaxIdx64f>,
        minMaxIdxWrap<hfloat, దశాంశం, minMaxIdx16f>,
        minMaxIdxWrap<bfloat, దశాంశం, minMaxIdx16bf>,
        minMaxIdxWrap<uchar, పూర్ణాంకం, minMaxIdx8u>,
        minMaxIdxWrap<uint64, uint64, minMaxIdx64u>,
        minMaxIdxWrap<int64, int64, minMaxIdx64s>,
        minMaxIdxWrap<గుర్తులేని, int64, minMaxIdx32u>,
        0
    };

    తిరిగి minMaxIdxTab[లోతు];
}

#అయితే_ముగింపు

CV_CPU_OPTIMIZATION_NAMESPACE_చివర
} // namespace
