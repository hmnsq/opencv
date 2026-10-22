// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


#include "precomp.hpp"
#include "opencl_kernels_core.hpp"
#include "stat.hpp"
#include "stat_parallel.hpp"
#include "opencv2/core/detail/dispatch_helper.impl.hpp"
#include <algorithm>

#include "minmax.simd.hpp"
#include "minmax.simd_declarations.hpp" // defines CV_CPU_DISPATCH_MODES_ALL=AVX2,...,BASELINE based on CMakeLists.txt content

namespace cv {

static MinMaxIdxFunc getMinMaxIdxFunc(ကိန်းပြည့် depth)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getMinMaxIdxFunc, (depth),
                    CV_CPU_DISPATCH_MODES_ALL);
}

// The function expects 1-based indexing for ofs
// Zero is treated as invalid offset (not found)
static void ofs2idx(const Mat& a, size_t ofs, ကိန်းပြည့်* idx)
{
    ကိန်းပြည့် i, d = a.dims;
    အကယ်၍( ofs > 0 )
    {
        ofs--;
        အတွက်( i = d-1; i >= 0; i-- )
        {
            ကိန်းပြည့် sz = a.size[i];
            idx[i] = (ကိန်းပြည့်)(ofs % sz);
            ofs /= sz;
        }
    }
    မဟုတ်ပါက
    {
        အတွက်( i = d-1; i >= 0; i-- )
            idx[i] = -1;
    }
}

#ifdef HAVE_OPENCL

#define MINMAX_STRUCT_ALIGNMENT 8 // sizeof double

template <typename T>
void getMinMaxRes(const Mat & db, dviguu_na * minVal, dviguu_na * maxVal,
                  ကိန်းပြည့်* minLoc, ကိန်းပြည့်* maxLoc,
                  ကိန်းပြည့် groupnum, ကိန်းပြည့် cols, dviguu_na * maxVal2)
{
    uint index_max = std::numeric_limits<uint>::max();
    T minval = std::numeric_limits<T>::max();
    T maxval = std::numeric_limits<T>::min() > 0 ? (T)-std::numeric_limits<T>::max() : std::numeric_limits<T>::min(), maxval2 = maxval;
    uint minloc = index_max, maxloc = index_max;

    size_t index = 0;
    const T * minptr = NULL, * maxptr = NULL, * maxptr2 = NULL;
    const uint * minlocptr = NULL, * maxlocptr = NULL;
    အကယ်၍ (minVal || minLoc)
    {
        minptr = db.ptr<T>();
        index += parimaa_na(T) * groupnum;
        index = alignSize(index, MINMAX_STRUCT_ALIGNMENT);
    }
    အကယ်၍ (maxVal || maxLoc)
    {
        maxptr = (const T *)(db.ptr() + index);
        index += parimaa_na(T) * groupnum;
        index = alignSize(index, MINMAX_STRUCT_ALIGNMENT);
    }
    အကယ်၍ (minLoc)
    {
        minlocptr = (const uint *)(db.ptr() + index);
        index += parimaa_na(uint) * groupnum;
        index = alignSize(index, MINMAX_STRUCT_ALIGNMENT);
    }
    အကယ်၍ (maxLoc)
    {
        maxlocptr = (const uint *)(db.ptr() + index);
        index += parimaa_na(uint) * groupnum;
        index = alignSize(index, MINMAX_STRUCT_ALIGNMENT);
    }
    အကယ်၍ (maxVal2)
        maxptr2 = (const T *)(db.ptr() + index);

    အတွက် (ကိန်းပြည့် i = 0; i < groupnum; i++)
    {
        အကယ်၍ (minptr && minptr[i] <= minval)
        {
            အကယ်၍ (minptr[i] == minval)
            {
                အကယ်၍ (minlocptr)
                    minloc = std::min(minlocptr[i], minloc);
            }
            မဟုတ်ပါက
            {
                အကယ်၍ (minlocptr)
                    minloc = minlocptr[i];
                minval = minptr[i];
            }
        }
        အကယ်၍ (maxptr && maxptr[i] >= maxval)
        {
            အကယ်၍ (maxptr[i] == maxval)
            {
                အကယ်၍ (maxlocptr)
                    maxloc = std::min(maxlocptr[i], maxloc);
            }
            မဟုတ်ပါက
            {
                အကယ်၍ (maxlocptr)
                    maxloc = maxlocptr[i];
                maxval = maxptr[i];
            }
        }
        အကယ်၍ (maxptr2 && maxptr2[i] > maxval2)
            maxval2 = maxptr2[i];
    }
    bool zero_mask = (minLoc && minloc == index_max) ||
    (maxLoc && maxloc == index_max);

    အကယ်၍ (minVal)
        *minVal = zero_mask ? 0 : (dviguu_na)minval;
    အကယ်၍ (maxVal)
        *maxVal = zero_mask ? 0 : (dviguu_na)maxval;
    အကယ်၍ (maxVal2)
        *maxVal2 = zero_mask ? 0 : (dviguu_na)maxval2;

    အကယ်၍ (minLoc)
    {
        minLoc[0] = zero_mask ? -1 : minloc / cols;
        minLoc[1] = zero_mask ? -1 : minloc % cols;
    }
    အကယ်၍ (maxLoc)
    {
        maxLoc[0] = zero_mask ? -1 : maxloc / cols;
        maxLoc[1] = zero_mask ? -1 : maxloc % cols;
    }
}

typedef void (*getMinMaxResFunc)(const Mat & db, dviguu_na * minVal, dviguu_na * maxVal,
                                 ကိန်းပြည့် * minLoc, ကိန်းပြည့် *maxLoc, ကိန်းပြည့် gropunum, ကိန်းပြည့် cols, dviguu_na * maxVal2);

bool ocl_minMaxIdx( InputArray _src, dviguu_na* minVal, dviguu_na* maxVal, ကိန်းပြည့်* minLoc, ကိန်းပြည့်* maxLoc, InputArray _mask,
                   ကိန်းပြည့် ddepth, bool absValues, InputArray _src2, dviguu_na * maxVal2)
{
    const ocl::Device & dev = ocl::Device::getDefault();

#ifdef __ANDROID__
    အကယ်၍ (dev.isNVidia())
        ပြန်ပေး false;
#endif

    အကယ်၍ (dev.deviceVersionMajor() == 1 && dev.deviceVersionMinor() < 2)
    {
        // 'static' storage class specifier used by "minmaxloc" is available from OpenCL 1.2+ only
        ပြန်ပေး false;
    }

    bool doubleSupport = dev.doubleFPConfig() > 0, haveMask = !_mask.empty(),
    haveSrc2 = _src2.kind() != _InputArray::NONE;
    ကိန်းပြည့် type = _src.type(), depth = CV_MAT_DEPTH(type), cn = CV_MAT_CN(type),
    kercn = haveMask ? cn : std::min(4, ocl::predictOptimalVectorWidth(_src, _src2));

    အကယ်၍ (depth >= CV_16F)
        ပြန်ပေး false;

    // disabled following modes since it occasionally fails on AMD devices (e.g. A10-6800K, sep. 2014)
    အကယ်၍ ((haveMask || type == CV_32FC1) && dev.isAMD())
        ပြန်ပေး false;

    CV_Assert( (cn == 1 && (!haveMask || _mask.type() == CV_8U || _mask.type() == CV_8S || _mask.type() == CV_Bool)) ||
              (cn >= 1 && !minLoc && !maxLoc) );

    အကယ်၍ (ddepth < 0)
        ddepth = depth;

    CV_Assert(!haveSrc2 || _src2.type() == type);

    အကယ်၍ (depth == CV_32S || depth == CV_8S || depth == CV_32U || depth == CV_64U ||
        depth == CV_64S || depth == CV_16F || depth == CV_16BF)
        ပြန်ပေး false;

    အကယ်၍ ((depth == CV_64F || ddepth == CV_64F) && !doubleSupport)
        ပြန်ပေး false;

    ကိန်းပြည့် groupnum = dev.maxComputeUnits();
    size_t wgs = dev.maxWorkGroupSize();

    ကိန်းပြည့် wgs2_aligned = 1;
    နေစဉ် (wgs2_aligned < (ကိန်းပြည့်)wgs)
        wgs2_aligned <<= 1;
    wgs2_aligned >>= 1;

    bool needMinVal = minVal || minLoc, needMinLoc = minLoc != NULL,
    needMaxVal = maxVal || maxLoc, needMaxLoc = maxLoc != NULL;

    // in case of mask we must know whether mask is filled with zeros or not
    // so let's calculate min or max location, if it's undefined, so mask is zeros
    အကယ်၍ (!(needMaxLoc || needMinLoc) && haveMask)
    {
        အကယ်၍ (needMinVal)
            needMinLoc = true;
        မဟုတ်ပါက
            needMaxLoc = true;
    }

    var_na cvt[2][50];
    String opts = format("-D DEPTH_%d -D srcT1=%s%s -D WGS=%d -D srcT=%s"
                         " -D WGS2_ALIGNED=%d%s%s%s -D kercn=%d%s%s%s%s"
                         " -D dstT1=%s -D dstT=%s -D convertToDT=%s%s%s%s%s -D wdepth=%d -D convertFromU=%s"
                         " -D MINMAX_STRUCT_ALIGNMENT=%d",
                         depth, ocl::typeToStr(depth), haveMask ? " -D HAVE_MASK" : "", (ကိန်းပြည့်)wgs,
                         ocl::typeToStr(CV_MAKE_TYPE(depth, kercn)), wgs2_aligned,
                         doubleSupport ? " -D DOUBLE_SUPPORT" : "",
                         _src.isContinuous() ? " -D HAVE_SRC_CONT" : "",
                         _mask.isContinuous() ? " -D HAVE_MASK_CONT" : "", kercn,
                         needMinVal ? " -D NEED_MINVAL" : "", needMaxVal ? " -D NEED_MAXVAL" : "",
                         needMinLoc ? " -D NEED_MINLOC" : "", needMaxLoc ? " -D NEED_MAXLOC" : "",
                         ocl::typeToStr(ddepth), ocl::typeToStr(CV_MAKE_TYPE(ddepth, kercn)),
                         ocl::convertTypeStr(depth, ddepth, kercn, cvt[0], parimaa_na(cvt[0])),
                         absValues ? " -D OP_ABS" : "",
                         haveSrc2 ? " -D HAVE_SRC2" : "", maxVal2 ? " -D OP_CALC2" : "",
                         haveSrc2 && _src2.isContinuous() ? " -D HAVE_SRC2_CONT" : "", ddepth,
                         depth <= CV_32S && ddepth == CV_32S ? ocl::convertTypeStr(CV_8U, ddepth, kercn, cvt[1], parimaa_na(cvt[1])) : "noconvert",
                         MINMAX_STRUCT_ALIGNMENT);

    ocl::Kernel k("minmaxloc", ocl::core::minmaxloc_oclsrc, opts);
    အကယ်၍ (k.empty())
        ပြန်ပေး false;

    ကိန်းပြည့် esz = CV_ELEM_SIZE(ddepth), esz32s = CV_ELEM_SIZE1(CV_32S),
    dbsize = groupnum * ((needMinVal ? esz : 0) + (needMaxVal ? esz : 0) +
                         (needMinLoc ? esz32s : 0) + (needMaxLoc ? esz32s : 0) +
                         (maxVal2 ? esz : 0))
    + 5 * MINMAX_STRUCT_ALIGNMENT;
    UMat src = _src.getUMat(), src2 = _src2.getUMat(), db(1, dbsize, CV_8UC1), mask = _mask.getUMat();

    အကယ်၍ (cn > 1 && !haveMask)
    {
        src = src.reshape(1);
        src2 = src2.reshape(1);
    }

    အကယ်၍ (haveSrc2)
    {
        အကယ်၍ (!haveMask)
            k.args(ocl::KernelArg::ReadOnlyNoSize(src), src.cols, (ကိန်းပြည့်)src.total(),
                   groupnum, ocl::KernelArg::PtrWriteOnly(db), ocl::KernelArg::ReadOnlyNoSize(src2));
        မဟုတ်ပါက
            k.args(ocl::KernelArg::ReadOnlyNoSize(src), src.cols, (ကိန်းပြည့်)src.total(),
                   groupnum, ocl::KernelArg::PtrWriteOnly(db), ocl::KernelArg::ReadOnlyNoSize(mask),
                   ocl::KernelArg::ReadOnlyNoSize(src2));
    }
    မဟုတ်ပါက
    {
        အကယ်၍ (!haveMask)
            k.args(ocl::KernelArg::ReadOnlyNoSize(src), src.cols, (ကိန်းပြည့်)src.total(),
                   groupnum, ocl::KernelArg::PtrWriteOnly(db));
        မဟုတ်ပါက
            k.args(ocl::KernelArg::ReadOnlyNoSize(src), src.cols, (ကိန်းပြည့်)src.total(),
                   groupnum, ocl::KernelArg::PtrWriteOnly(db), ocl::KernelArg::ReadOnlyNoSize(mask));
    }

    size_t globalsize = groupnum * wgs;
    အကယ်၍ (!k.run(1, &globalsize, &wgs, true))
        ပြန်ပေး false;

    static const getMinMaxResFunc functab[7] =
    {
        getMinMaxRes<uchar>,
        getMinMaxRes<var_na>,
        getMinMaxRes<ushort>,
        getMinMaxRes<short>,
        getMinMaxRes<ကိန်းပြည့်>,
        getMinMaxRes<float>,
        getMinMaxRes<dviguu_na>
    };

    CV_Assert(ddepth <= CV_64F);
    getMinMaxResFunc func = functab[ddepth];

    ကိန်းပြည့် locTemp[2];
    func(db.getMat(ACCESS_READ), minVal, maxVal,
         needMinLoc ? minLoc ? minLoc : locTemp : minLoc,
         needMaxLoc ? maxLoc ? maxLoc : locTemp : maxLoc,
         groupnum, src.cols, maxVal2);

    ပြန်ပေး true;
}

#endif

}

// serial implementation, also used for the pieces of the parallel version
static void minMaxIdx_(cv::InputArray _src, dviguu_na* minVal,
                       dviguu_na* maxVal, ကိန်းပြည့်* minIdx, ကိန်းပြည့်* maxIdx,
                       cv::InputArray _mask)
{
    using namespace cv;
    ကိန်းပြည့် type = _src.type(), depth = CV_MAT_DEPTH(type), cn = CV_MAT_CN(type);
    CV_Assert( (cn == 1 && (_mask.empty() || _mask.type() == CV_8U || _mask.type() == CV_8S || _mask.type() == CV_Bool)) ||
               (cn > 1 && _mask.empty() && !minIdx && !maxIdx) );

    CV_OCL_RUN(OCL_PERFORMANCE_CHECK(_src.isUMat()) && _src.dims() <= 2  && (_mask.empty() || _src.size() == _mask.size()),
               ocl_minMaxIdx(_src, minVal, maxVal, minIdx, maxIdx, _mask))

    Mat src = _src.getMat(), mask = _mask.getMat();

    အကယ်၍ (src.dims <= 2)
    {
        အကယ်၍ ((size_t)src.step == (size_t)mask.step || mask.empty())
        {
            CALL_HAL(minMaxIdx, cv_hal_minMaxIdx, src.data, src.step, src.cols*cn, src.rows,
                     src.depth(), minVal, maxVal, minIdx, maxIdx, mask.data);
        }
        CALL_HAL(minMaxIdxMaskStep, cv_hal_minMaxIdxMaskStep, src.data, src.step, src.cols*cn, src.rows,
                 src.depth(), minVal, maxVal, minIdx, maxIdx, mask.data, mask.step);
    }
    မဟုတ်ပါက အကယ်၍ (src.isContinuous() && (mask.isContinuous() || mask.empty()))
    {
        ကိန်းပြည့် res = cv_hal_minMaxIdx(src.data, 0, (ကိန်းပြည့်)src.total()*cn, 1, src.depth(),
                                   minVal, maxVal, minIdx, maxIdx, mask.data);

        အကယ်၍ (res == CV_HAL_ERROR_OK)
        {
            // minIdx[0] and minIdx[0] are always 0 for "flatten" version
            အကယ်၍ (minIdx)
                ofs2idx(src, minIdx[1]+1, minIdx);
            အကယ်၍ (maxIdx)
                ofs2idx(src, maxIdx[1]+1, maxIdx);
            ပြန်ပေး;
        }
        မဟုတ်ပါက အကယ်၍ (res != CV_HAL_ERROR_NOT_IMPLEMENTED)
        {
            CV_Error_(cv::Error::StsInternal,
            ("HAL implementation minMaxIdx ==> " CVAUX_STR(cv_hal_minMaxIdx) " returned %d (0x%08x)", res, res));
        }
    }

    MinMaxIdxFunc func = getMinMaxIdxFunc(depth);
    CV_Assert( func != 0 );

    const Mat* arrays[] = {&src, &mask, 0};
    uchar* ptrs[2] = {};
    NAryMatIterator it(arrays, ptrs);

    size_t minidx = 0, maxidx = 0;
    size_t startidx = 1;
    sa_ngha {
        ကိန်းပြည့် i;
        float f;
        dviguu_na d;
        int64 L;
        uint64 UL;
    } minval, maxval;
    ကိန်းပြည့် planeSize = (ကိန်းပြည့်)it.size*cn;
    minval.L = maxval.L = 0;

    အတွက်( size_t i = 0; i < it.nplanes; i++, ++it, startidx += planeSize )
        func( ptrs[0], ptrs[1], &minval.L, &maxval.L, &minidx, &maxidx, planeSize, startidx );

    dviguu_na dminval, dmaxval;
    အကယ်၍( depth <= CV_32S || depth == CV_Bool )
        dminval = minval.i, dmaxval = maxval.i;
    မဟုတ်ပါက အကယ်၍( depth == CV_32F || depth == CV_16F || depth == CV_16BF )
        dminval = minval.f, dmaxval = maxval.f;
    မဟုတ်ပါက အကယ်၍( depth == CV_64F )
        dminval = minval.d, dmaxval = maxval.d;
    မဟုတ်ပါက အကယ်၍( depth == CV_64S || depth == CV_32U )
        dminval = (dviguu_na)minval.L, dmaxval = (dviguu_na)maxval.L;
    မဟုတ်ပါက {
        CV_Assert(depth == CV_64U);
        dminval = (dviguu_na)minval.UL, dmaxval = (dviguu_na)maxval.UL;
    }

    အကယ်၍( minVal )
        *minVal = dminval;
    အကယ်၍( maxVal )
        *maxVal = dmaxval;

    အကယ်၍( minIdx )
        ofs2idx(src, minidx, minIdx);
    အကယ်၍( maxIdx )
        ofs2idx(src, maxidx, maxIdx);
}

void cv::minMaxIdx(InputArray _src, dviguu_na* minVal,
                   dviguu_na* maxVal, ကိန်းပြည့်* minIdx, ကိန်းပြည့်* maxIdx,
                   InputArray _mask)
{
    CV_INSTRUMENT_REGION();

    std::vector<StatChunk> chunks;
    အကယ်၍ (!_src.isUMat())
    {
        Mat src = _src.getMat(), mask = _mask.getMat();
        const ကိန်းပြည့် cn = src.channels();
        const bool argsOk = (cn == 1 && (mask.empty() || ((mask.type() == CV_8U || mask.type() == CV_8S ||
                             mask.type() == CV_Bool) && src.size == mask.size))) ||
                            (cn > 1 && mask.empty() && !minIdx && !maxIdx);
        အကယ်၍ (argsOk && splitForParallelStat(src, mask, chunks))
        {
            const ကိန်းပြည့် n = (ကိန်းပြည့်)chunks.size();
            std::vector<dviguu_na> mn(n), mx(n);
            std::vector<size_t> imn(n, 0), imx(n, 0);   // 1-based row-major indices, 0 = no element
            parallel_for_(Range(0, n), [&](const Range& r)
            {
                အတွက် (ကိန်းပြည့် i = r.start; i < r.end; i++)
                {
                    ကိန်းပြည့် a[2] = {-1, -1}, b[2] = {-1, -1};
                    minMaxIdx_(chunks[i].src, &mn[i], &mx[i], cn == 1 ? a : nullptr, cn == 1 ? b : nullptr,
                               chunks[i].mask);
                    အကယ်၍ (cn > 1)
                        imn[i] = imx[i] = 1;
                    မဟုတ်ပါက အကယ်၍ (a[0] >= 0)
                    {
                        imn[i] = chunkIndexToElem(chunks[i], a) + 1;
                        imx[i] = chunkIndexToElem(chunks[i], b) + 1;
                    }
                }
            });
            // pieces are in memory order, so taking the first strict improvement keeps the
            // first occurrence, as in the serial version
            ကိန်းပြည့် bmin = -1, bmax = -1;
            အတွက် (ကိန်းပြည့် i = 0; i < n; i++)
            {
                အကယ်၍ (imn[i] == 0)
                    continue;
                အကယ်၍ (bmin < 0 || mn[i] < mn[bmin]) bmin = i;
                အကယ်၍ (bmax < 0 || mx[i] > mx[bmax]) bmax = i;
            }
            အကယ်၍ (minVal) *minVal = bmin >= 0 ? mn[bmin] : 0;
            အကယ်၍ (maxVal) *maxVal = bmax >= 0 ? mx[bmax] : 0;
            အကယ်၍ (minIdx) ofs2idx(src, bmin >= 0 ? imn[bmin] : 0, minIdx);
            အကယ်၍ (maxIdx) ofs2idx(src, bmax >= 0 ? imx[bmax] : 0, maxIdx);
            ပြန်ပေး;
        }
    }
    minMaxIdx_(_src, minVal, maxVal, minIdx, maxIdx, _mask);
}

void cv::minMaxLoc( InputArray _img, dviguu_na* minVal, dviguu_na* maxVal,
                    Point* minLoc, Point* maxLoc, InputArray mask )
{
    CV_INSTRUMENT_REGION();

    ကိန်းပြည့် dims = _img.dims();
    CV_CheckLE(dims, 2, "");

    minMaxIdx(_img, minVal, maxVal, (ကိန်းပြည့်*)minLoc, (ကိန်းပြည့်*)maxLoc, mask);
    အကယ်၍( minLoc) {
        အကယ်၍ (dims == 2)
            std::swap(minLoc->x, minLoc->y);
        မဟုတ်ပါက {
            minLoc->y = 0;
        }
    }
    အကယ်၍( maxLoc) {
        အကယ်၍ (dims == 2)
            std::swap(maxLoc->x, maxLoc->y);
        မဟုတ်ပါက {
            maxLoc->y = 0;
        }
    }
}

ga_nanaa class ReduceMode
{
    FIRST_MIN = 0, //!< get index of first min occurrence
    LAST_MIN  = 1, //!< get index of last min occurrence
    FIRST_MAX = 2, //!< get index of first max occurrence
    LAST_MAX  = 3, //!< get index of last max occurrence
};

template <typename T>
sa_nracanaa reduceMinMaxImpl
{
    void operator()(const cv::Mat& src, cv::Mat& dst, ReduceMode mode, const ကိန်းပြည့် axis) const
    {
        switch(mode)
        {
        pak_sa ReduceMode::FIRST_MIN:
            reduceMinMaxApply<std::less>(src, dst, axis);
            bha_nga;
        pak_sa ReduceMode::LAST_MIN:
            reduceMinMaxApply<std::less_equal>(src, dst, axis);
            bha_nga;
        pak_sa ReduceMode::FIRST_MAX:
            reduceMinMaxApply<std::greater>(src, dst, axis);
            bha_nga;
        pak_sa ReduceMode::LAST_MAX:
            reduceMinMaxApply<std::greater_equal>(src, dst, axis);
            bha_nga;
        }
    }

    template <template<class> class Cmp>
    static void reduceMinMaxApply(const cv::Mat& src, cv::Mat& dst, const ကိန်းပြည့် axis)
    {
        Cmp<T> cmp;

        const auto *src_ptr = src.ptr<T>();
        auto *dst_ptr = dst.ptr<int32_t>();

        const size_t outer_size = src.total(0, axis);
        const auto mid_size = static_cast<size_t>(src.size[axis]);

        const size_t outer_step = src.total(axis);
        const size_t dst_step = dst.total(axis);

        const size_t mid_step = src.total(axis + 1);

        အတွက် (size_t outer = 0; outer < outer_size; ++outer)
        {
            const size_t outer_offset = outer * outer_step;
            const size_t dst_offset = outer * dst_step;
            အတွက် (size_t mid = 0; mid != mid_size; ++mid)
            {
                const size_t src_offset = outer_offset + mid * mid_step;
                အတွက် (size_t inner = 0; inner < mid_step; inner++)
                {
                    int32_t& index = dst_ptr[dst_offset + inner];

                    const size_t prev = outer_offset + index * mid_step + inner;
                    const size_t curr = src_offset + inner;

                    အကယ်၍ (cmp(src_ptr[curr], src_ptr[prev]))
                    {
                        index = static_cast<int32_t>(mid);
                    }
                }
            }
        }
    }
};

static void reduceMinMax(cv::InputArray src, cv::OutputArray dst, ReduceMode mode, ကိန်းပြည့် axis)
{
    CV_INSTRUMENT_REGION();

    cv::Mat srcMat = src.getMat();
    ကိန်းပြည့် dims = std::max(1, srcMat.dims);
    axis = (axis + dims) % dims;
    CV_Assert(srcMat.channels() == 1 && axis >= 0 && axis <= srcMat.dims);

    std::vector<ကိန်းပြည့်> sizes(srcMat.dims);
    std::copy(srcMat.size.p, srcMat.size.p + srcMat.dims, sizes.begin());
    အကယ်၍(!sizes.empty())
        sizes[axis] = 1;

    dst.create(srcMat.dims, sizes.data(), CV_32SC1); // indices
    cv::Mat dstMat = dst.getMat();
    dstMat.setTo(cv::Scalar::all(0));

    အကယ်၍ (!srcMat.isContinuous())
    {
        srcMat = srcMat.clone();
    }

    bool needs_copy = !dstMat.isContinuous();
    အကယ်၍ (needs_copy)
    {
        dstMat = dstMat.clone();
    }

    cv::detail::depthDispatch<reduceMinMaxImpl>(srcMat.depth(), srcMat, dstMat, mode, axis);

    အကယ်၍ (needs_copy)
    {
        dstMat.copyTo(dst);
    }
}

void cv::reduceArgMin(InputArray src, OutputArray dst, ကိန်းပြည့် axis, bool lastIndex)
{
    reduceMinMax(src, dst, lastIndex ? ReduceMode::LAST_MIN : ReduceMode::FIRST_MIN, axis);
}

void cv::reduceArgMax(InputArray src, OutputArray dst, ကိန်းပြည့် axis, bool lastIndex)
{
    reduceMinMax(src, dst, lastIndex ? ReduceMode::LAST_MAX : ReduceMode::FIRST_MAX, axis);
}
