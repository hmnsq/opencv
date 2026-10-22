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

static MinMaxIdxFunc getMinMaxIdxFunc(පූර්ණ_සංඛ්‍යා depth)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getMinMaxIdxFunc, (depth),
                    CV_CPU_DISPATCH_MODES_ALL);
}

// The function expects 1-based indexing for ofs
// Zero is treated as invalid offset (not found)
static void ofs2idx(const Mat& a, size_t ofs, පූර්ණ_සංඛ්‍යා* idx)
{
    පූර්ණ_සංඛ්‍යා i, d = a.dims;
    නම්( ofs > 0 )
    {
        ofs--;
        සඳහා( i = d-1; i >= 0; i-- )
        {
            පූර්ණ_සංඛ්‍යා sz = a.size[i];
            idx[i] = (පූර්ණ_සංඛ්‍යා)(ofs % sz);
            ofs /= sz;
        }
    }
    නැතහොත්
    {
        සඳහා( i = d-1; i >= 0; i-- )
            idx[i] = -1;
    }
}

#ifdef HAVE_OPENCL

#define MINMAX_STRUCT_ALIGNMENT 8 // sizeof double

template <typename T>
void getMinMaxRes(const Mat & db, dviguu_na * minVal, dviguu_na * maxVal,
                  පූර්ණ_සංඛ්‍යා* minLoc, පූර්ණ_සංඛ්‍යා* maxLoc,
                  පූර්ණ_සංඛ්‍යා groupnum, පූර්ණ_සංඛ්‍යා cols, dviguu_na * maxVal2)
{
    uint index_max = std::numeric_limits<uint>::max();
    T minval = std::numeric_limits<T>::max();
    T maxval = std::numeric_limits<T>::min() > 0 ? (T)-std::numeric_limits<T>::max() : std::numeric_limits<T>::min(), maxval2 = maxval;
    uint minloc = index_max, maxloc = index_max;

    size_t index = 0;
    const T * minptr = NULL, * maxptr = NULL, * maxptr2 = NULL;
    const uint * minlocptr = NULL, * maxlocptr = NULL;
    නම් (minVal || minLoc)
    {
        minptr = db.ptr<T>();
        index += parimaa_na(T) * groupnum;
        index = alignSize(index, MINMAX_STRUCT_ALIGNMENT);
    }
    නම් (maxVal || maxLoc)
    {
        maxptr = (const T *)(db.ptr() + index);
        index += parimaa_na(T) * groupnum;
        index = alignSize(index, MINMAX_STRUCT_ALIGNMENT);
    }
    නම් (minLoc)
    {
        minlocptr = (const uint *)(db.ptr() + index);
        index += parimaa_na(uint) * groupnum;
        index = alignSize(index, MINMAX_STRUCT_ALIGNMENT);
    }
    නම් (maxLoc)
    {
        maxlocptr = (const uint *)(db.ptr() + index);
        index += parimaa_na(uint) * groupnum;
        index = alignSize(index, MINMAX_STRUCT_ALIGNMENT);
    }
    නම් (maxVal2)
        maxptr2 = (const T *)(db.ptr() + index);

    සඳහා (පූර්ණ_සංඛ්‍යා i = 0; i < groupnum; i++)
    {
        නම් (minptr && minptr[i] <= minval)
        {
            නම් (minptr[i] == minval)
            {
                නම් (minlocptr)
                    minloc = std::min(minlocptr[i], minloc);
            }
            නැතහොත්
            {
                නම් (minlocptr)
                    minloc = minlocptr[i];
                minval = minptr[i];
            }
        }
        නම් (maxptr && maxptr[i] >= maxval)
        {
            නම් (maxptr[i] == maxval)
            {
                නම් (maxlocptr)
                    maxloc = std::min(maxlocptr[i], maxloc);
            }
            නැතහොත්
            {
                නම් (maxlocptr)
                    maxloc = maxlocptr[i];
                maxval = maxptr[i];
            }
        }
        නම් (maxptr2 && maxptr2[i] > maxval2)
            maxval2 = maxptr2[i];
    }
    bool zero_mask = (minLoc && minloc == index_max) ||
    (maxLoc && maxloc == index_max);

    නම් (minVal)
        *minVal = zero_mask ? 0 : (dviguu_na)minval;
    නම් (maxVal)
        *maxVal = zero_mask ? 0 : (dviguu_na)maxval;
    නම් (maxVal2)
        *maxVal2 = zero_mask ? 0 : (dviguu_na)maxval2;

    නම් (minLoc)
    {
        minLoc[0] = zero_mask ? -1 : minloc / cols;
        minLoc[1] = zero_mask ? -1 : minloc % cols;
    }
    නම් (maxLoc)
    {
        maxLoc[0] = zero_mask ? -1 : maxloc / cols;
        maxLoc[1] = zero_mask ? -1 : maxloc % cols;
    }
}

typedef void (*getMinMaxResFunc)(const Mat & db, dviguu_na * minVal, dviguu_na * maxVal,
                                 පූර්ණ_සංඛ්‍යා * minLoc, පූර්ණ_සංඛ්‍යා *maxLoc, පූර්ණ_සංඛ්‍යා gropunum, පූර්ණ_සංඛ්‍යා cols, dviguu_na * maxVal2);

bool ocl_minMaxIdx( InputArray _src, dviguu_na* minVal, dviguu_na* maxVal, පූර්ණ_සංඛ්‍යා* minLoc, පූර්ණ_සංඛ්‍යා* maxLoc, InputArray _mask,
                   පූර්ණ_සංඛ්‍යා ddepth, bool absValues, InputArray _src2, dviguu_na * maxVal2)
{
    const ocl::Device & dev = ocl::Device::getDefault();

#ifdef __ANDROID__
    නම් (dev.isNVidia())
        ආපසු false;
#endif

    නම් (dev.deviceVersionMajor() == 1 && dev.deviceVersionMinor() < 2)
    {
        // 'static' storage class specifier used by "minmaxloc" is available from OpenCL 1.2+ only
        ආපසු false;
    }

    bool doubleSupport = dev.doubleFPConfig() > 0, haveMask = !_mask.empty(),
    haveSrc2 = _src2.kind() != _InputArray::NONE;
    පූර්ණ_සංඛ්‍යා type = _src.type(), depth = CV_MAT_DEPTH(type), cn = CV_MAT_CN(type),
    kercn = haveMask ? cn : std::min(4, ocl::predictOptimalVectorWidth(_src, _src2));

    නම් (depth >= CV_16F)
        ආපසු false;

    // disabled following modes since it occasionally fails on AMD devices (e.g. A10-6800K, sep. 2014)
    නම් ((haveMask || type == CV_32FC1) && dev.isAMD())
        ආපසු false;

    CV_Assert( (cn == 1 && (!haveMask || _mask.type() == CV_8U || _mask.type() == CV_8S || _mask.type() == CV_Bool)) ||
              (cn >= 1 && !minLoc && !maxLoc) );

    නම් (ddepth < 0)
        ddepth = depth;

    CV_Assert(!haveSrc2 || _src2.type() == type);

    නම් (depth == CV_32S || depth == CV_8S || depth == CV_32U || depth == CV_64U ||
        depth == CV_64S || depth == CV_16F || depth == CV_16BF)
        ආපසු false;

    නම් ((depth == CV_64F || ddepth == CV_64F) && !doubleSupport)
        ආපසු false;

    පූර්ණ_සංඛ්‍යා groupnum = dev.maxComputeUnits();
    size_t wgs = dev.maxWorkGroupSize();

    පූර්ණ_සංඛ්‍යා wgs2_aligned = 1;
    අතරතුර (wgs2_aligned < (පූර්ණ_සංඛ්‍යා)wgs)
        wgs2_aligned <<= 1;
    wgs2_aligned >>= 1;

    bool needMinVal = minVal || minLoc, needMinLoc = minLoc != NULL,
    needMaxVal = maxVal || maxLoc, needMaxLoc = maxLoc != NULL;

    // in case of mask we must know whether mask is filled with zeros or not
    // so let's calculate min or max location, if it's undefined, so mask is zeros
    නම් (!(needMaxLoc || needMinLoc) && haveMask)
    {
        නම් (needMinVal)
            needMinLoc = true;
        නැතහොත්
            needMaxLoc = true;
    }

    var_na cvt[2][50];
    String opts = format("-D DEPTH_%d -D srcT1=%s%s -D WGS=%d -D srcT=%s"
                         " -D WGS2_ALIGNED=%d%s%s%s -D kercn=%d%s%s%s%s"
                         " -D dstT1=%s -D dstT=%s -D convertToDT=%s%s%s%s%s -D wdepth=%d -D convertFromU=%s"
                         " -D MINMAX_STRUCT_ALIGNMENT=%d",
                         depth, ocl::typeToStr(depth), haveMask ? " -D HAVE_MASK" : "", (පූර්ණ_සංඛ්‍යා)wgs,
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
    නම් (k.empty())
        ආපසු false;

    පූර්ණ_සංඛ්‍යා esz = CV_ELEM_SIZE(ddepth), esz32s = CV_ELEM_SIZE1(CV_32S),
    dbsize = groupnum * ((needMinVal ? esz : 0) + (needMaxVal ? esz : 0) +
                         (needMinLoc ? esz32s : 0) + (needMaxLoc ? esz32s : 0) +
                         (maxVal2 ? esz : 0))
    + 5 * MINMAX_STRUCT_ALIGNMENT;
    UMat src = _src.getUMat(), src2 = _src2.getUMat(), db(1, dbsize, CV_8UC1), mask = _mask.getUMat();

    නම් (cn > 1 && !haveMask)
    {
        src = src.reshape(1);
        src2 = src2.reshape(1);
    }

    නම් (haveSrc2)
    {
        නම් (!haveMask)
            k.args(ocl::KernelArg::ReadOnlyNoSize(src), src.cols, (පූර්ණ_සංඛ්‍යා)src.total(),
                   groupnum, ocl::KernelArg::PtrWriteOnly(db), ocl::KernelArg::ReadOnlyNoSize(src2));
        නැතහොත්
            k.args(ocl::KernelArg::ReadOnlyNoSize(src), src.cols, (පූර්ණ_සංඛ්‍යා)src.total(),
                   groupnum, ocl::KernelArg::PtrWriteOnly(db), ocl::KernelArg::ReadOnlyNoSize(mask),
                   ocl::KernelArg::ReadOnlyNoSize(src2));
    }
    නැතහොත්
    {
        නම් (!haveMask)
            k.args(ocl::KernelArg::ReadOnlyNoSize(src), src.cols, (පූර්ණ_සංඛ්‍යා)src.total(),
                   groupnum, ocl::KernelArg::PtrWriteOnly(db));
        නැතහොත්
            k.args(ocl::KernelArg::ReadOnlyNoSize(src), src.cols, (පූර්ණ_සංඛ්‍යා)src.total(),
                   groupnum, ocl::KernelArg::PtrWriteOnly(db), ocl::KernelArg::ReadOnlyNoSize(mask));
    }

    size_t globalsize = groupnum * wgs;
    නම් (!k.run(1, &globalsize, &wgs, true))
        ආපසු false;

    static const getMinMaxResFunc functab[7] =
    {
        getMinMaxRes<uchar>,
        getMinMaxRes<var_na>,
        getMinMaxRes<ushort>,
        getMinMaxRes<short>,
        getMinMaxRes<පූර්ණ_සංඛ්‍යා>,
        getMinMaxRes<float>,
        getMinMaxRes<dviguu_na>
    };

    CV_Assert(ddepth <= CV_64F);
    getMinMaxResFunc func = functab[ddepth];

    පූර්ණ_සංඛ්‍යා locTemp[2];
    func(db.getMat(ACCESS_READ), minVal, maxVal,
         needMinLoc ? minLoc ? minLoc : locTemp : minLoc,
         needMaxLoc ? maxLoc ? maxLoc : locTemp : maxLoc,
         groupnum, src.cols, maxVal2);

    ආපසු true;
}

#endif

}

// serial implementation, also used for the pieces of the parallel version
static void minMaxIdx_(cv::InputArray _src, dviguu_na* minVal,
                       dviguu_na* maxVal, පූර්ණ_සංඛ්‍යා* minIdx, පූර්ණ_සංඛ්‍යා* maxIdx,
                       cv::InputArray _mask)
{
    using namespace cv;
    පූර්ණ_සංඛ්‍යා type = _src.type(), depth = CV_MAT_DEPTH(type), cn = CV_MAT_CN(type);
    CV_Assert( (cn == 1 && (_mask.empty() || _mask.type() == CV_8U || _mask.type() == CV_8S || _mask.type() == CV_Bool)) ||
               (cn > 1 && _mask.empty() && !minIdx && !maxIdx) );

    CV_OCL_RUN(OCL_PERFORMANCE_CHECK(_src.isUMat()) && _src.dims() <= 2  && (_mask.empty() || _src.size() == _mask.size()),
               ocl_minMaxIdx(_src, minVal, maxVal, minIdx, maxIdx, _mask))

    Mat src = _src.getMat(), mask = _mask.getMat();

    නම් (src.dims <= 2)
    {
        නම් ((size_t)src.step == (size_t)mask.step || mask.empty())
        {
            CALL_HAL(minMaxIdx, cv_hal_minMaxIdx, src.data, src.step, src.cols*cn, src.rows,
                     src.depth(), minVal, maxVal, minIdx, maxIdx, mask.data);
        }
        CALL_HAL(minMaxIdxMaskStep, cv_hal_minMaxIdxMaskStep, src.data, src.step, src.cols*cn, src.rows,
                 src.depth(), minVal, maxVal, minIdx, maxIdx, mask.data, mask.step);
    }
    නැතහොත් නම් (src.isContinuous() && (mask.isContinuous() || mask.empty()))
    {
        පූර්ණ_සංඛ්‍යා res = cv_hal_minMaxIdx(src.data, 0, (පූර්ණ_සංඛ්‍යා)src.total()*cn, 1, src.depth(),
                                   minVal, maxVal, minIdx, maxIdx, mask.data);

        නම් (res == CV_HAL_ERROR_OK)
        {
            // minIdx[0] and minIdx[0] are always 0 for "flatten" version
            නම් (minIdx)
                ofs2idx(src, minIdx[1]+1, minIdx);
            නම් (maxIdx)
                ofs2idx(src, maxIdx[1]+1, maxIdx);
            ආපසු;
        }
        නැතහොත් නම් (res != CV_HAL_ERROR_NOT_IMPLEMENTED)
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
        පූර්ණ_සංඛ්‍යා i;
        float f;
        dviguu_na d;
        int64 L;
        uint64 UL;
    } minval, maxval;
    පූර්ණ_සංඛ්‍යා planeSize = (පූර්ණ_සංඛ්‍යා)it.size*cn;
    minval.L = maxval.L = 0;

    සඳහා( size_t i = 0; i < it.nplanes; i++, ++it, startidx += planeSize )
        func( ptrs[0], ptrs[1], &minval.L, &maxval.L, &minidx, &maxidx, planeSize, startidx );

    dviguu_na dminval, dmaxval;
    නම්( depth <= CV_32S || depth == CV_Bool )
        dminval = minval.i, dmaxval = maxval.i;
    නැතහොත් නම්( depth == CV_32F || depth == CV_16F || depth == CV_16BF )
        dminval = minval.f, dmaxval = maxval.f;
    නැතහොත් නම්( depth == CV_64F )
        dminval = minval.d, dmaxval = maxval.d;
    නැතහොත් නම්( depth == CV_64S || depth == CV_32U )
        dminval = (dviguu_na)minval.L, dmaxval = (dviguu_na)maxval.L;
    නැතහොත් {
        CV_Assert(depth == CV_64U);
        dminval = (dviguu_na)minval.UL, dmaxval = (dviguu_na)maxval.UL;
    }

    නම්( minVal )
        *minVal = dminval;
    නම්( maxVal )
        *maxVal = dmaxval;

    නම්( minIdx )
        ofs2idx(src, minidx, minIdx);
    නම්( maxIdx )
        ofs2idx(src, maxidx, maxIdx);
}

void cv::minMaxIdx(InputArray _src, dviguu_na* minVal,
                   dviguu_na* maxVal, පූර්ණ_සංඛ්‍යා* minIdx, පූර්ණ_සංඛ්‍යා* maxIdx,
                   InputArray _mask)
{
    CV_INSTRUMENT_REGION();

    std::vector<StatChunk> chunks;
    නම් (!_src.isUMat())
    {
        Mat src = _src.getMat(), mask = _mask.getMat();
        const පූර්ණ_සංඛ්‍යා cn = src.channels();
        const bool argsOk = (cn == 1 && (mask.empty() || ((mask.type() == CV_8U || mask.type() == CV_8S ||
                             mask.type() == CV_Bool) && src.size == mask.size))) ||
                            (cn > 1 && mask.empty() && !minIdx && !maxIdx);
        නම් (argsOk && splitForParallelStat(src, mask, chunks))
        {
            const පූර්ණ_සංඛ්‍යා n = (පූර්ණ_සංඛ්‍යා)chunks.size();
            std::vector<dviguu_na> mn(n), mx(n);
            std::vector<size_t> imn(n, 0), imx(n, 0);   // 1-based row-major indices, 0 = no element
            parallel_for_(Range(0, n), [&](const Range& r)
            {
                සඳහා (පූර්ණ_සංඛ්‍යා i = r.start; i < r.end; i++)
                {
                    පූර්ණ_සංඛ්‍යා a[2] = {-1, -1}, b[2] = {-1, -1};
                    minMaxIdx_(chunks[i].src, &mn[i], &mx[i], cn == 1 ? a : nullptr, cn == 1 ? b : nullptr,
                               chunks[i].mask);
                    නම් (cn > 1)
                        imn[i] = imx[i] = 1;
                    නැතහොත් නම් (a[0] >= 0)
                    {
                        imn[i] = chunkIndexToElem(chunks[i], a) + 1;
                        imx[i] = chunkIndexToElem(chunks[i], b) + 1;
                    }
                }
            });
            // pieces are in memory order, so taking the first strict improvement keeps the
            // first occurrence, as in the serial version
            පූර්ණ_සංඛ්‍යා bmin = -1, bmax = -1;
            සඳහා (පූර්ණ_සංඛ්‍යා i = 0; i < n; i++)
            {
                නම් (imn[i] == 0)
                    continue;
                නම් (bmin < 0 || mn[i] < mn[bmin]) bmin = i;
                නම් (bmax < 0 || mx[i] > mx[bmax]) bmax = i;
            }
            නම් (minVal) *minVal = bmin >= 0 ? mn[bmin] : 0;
            නම් (maxVal) *maxVal = bmax >= 0 ? mx[bmax] : 0;
            නම් (minIdx) ofs2idx(src, bmin >= 0 ? imn[bmin] : 0, minIdx);
            නම් (maxIdx) ofs2idx(src, bmax >= 0 ? imx[bmax] : 0, maxIdx);
            ආපසු;
        }
    }
    minMaxIdx_(_src, minVal, maxVal, minIdx, maxIdx, _mask);
}

void cv::minMaxLoc( InputArray _img, dviguu_na* minVal, dviguu_na* maxVal,
                    Point* minLoc, Point* maxLoc, InputArray mask )
{
    CV_INSTRUMENT_REGION();

    පූර්ණ_සංඛ්‍යා dims = _img.dims();
    CV_CheckLE(dims, 2, "");

    minMaxIdx(_img, minVal, maxVal, (පූර්ණ_සංඛ්‍යා*)minLoc, (පූර්ණ_සංඛ්‍යා*)maxLoc, mask);
    නම්( minLoc) {
        නම් (dims == 2)
            std::swap(minLoc->x, minLoc->y);
        නැතහොත් {
            minLoc->y = 0;
        }
    }
    නම්( maxLoc) {
        නම් (dims == 2)
            std::swap(maxLoc->x, maxLoc->y);
        නැතහොත් {
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
    void operator()(const cv::Mat& src, cv::Mat& dst, ReduceMode mode, const පූර්ණ_සංඛ්‍යා axis) const
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
    static void reduceMinMaxApply(const cv::Mat& src, cv::Mat& dst, const පූර්ණ_සංඛ්‍යා axis)
    {
        Cmp<T> cmp;

        const auto *src_ptr = src.ptr<T>();
        auto *dst_ptr = dst.ptr<int32_t>();

        const size_t outer_size = src.total(0, axis);
        const auto mid_size = static_cast<size_t>(src.size[axis]);

        const size_t outer_step = src.total(axis);
        const size_t dst_step = dst.total(axis);

        const size_t mid_step = src.total(axis + 1);

        සඳහා (size_t outer = 0; outer < outer_size; ++outer)
        {
            const size_t outer_offset = outer * outer_step;
            const size_t dst_offset = outer * dst_step;
            සඳහා (size_t mid = 0; mid != mid_size; ++mid)
            {
                const size_t src_offset = outer_offset + mid * mid_step;
                සඳහා (size_t inner = 0; inner < mid_step; inner++)
                {
                    int32_t& index = dst_ptr[dst_offset + inner];

                    const size_t prev = outer_offset + index * mid_step + inner;
                    const size_t curr = src_offset + inner;

                    නම් (cmp(src_ptr[curr], src_ptr[prev]))
                    {
                        index = static_cast<int32_t>(mid);
                    }
                }
            }
        }
    }
};

static void reduceMinMax(cv::InputArray src, cv::OutputArray dst, ReduceMode mode, පූර්ණ_සංඛ්‍යා axis)
{
    CV_INSTRUMENT_REGION();

    cv::Mat srcMat = src.getMat();
    පූර්ණ_සංඛ්‍යා dims = std::max(1, srcMat.dims);
    axis = (axis + dims) % dims;
    CV_Assert(srcMat.channels() == 1 && axis >= 0 && axis <= srcMat.dims);

    std::vector<පූර්ණ_සංඛ්‍යා> sizes(srcMat.dims);
    std::copy(srcMat.size.p, srcMat.size.p + srcMat.dims, sizes.begin());
    නම්(!sizes.empty())
        sizes[axis] = 1;

    dst.create(srcMat.dims, sizes.data(), CV_32SC1); // indices
    cv::Mat dstMat = dst.getMat();
    dstMat.setTo(cv::Scalar::all(0));

    නම් (!srcMat.isContinuous())
    {
        srcMat = srcMat.clone();
    }

    bool needs_copy = !dstMat.isContinuous();
    නම් (needs_copy)
    {
        dstMat = dstMat.clone();
    }

    cv::detail::depthDispatch<reduceMinMaxImpl>(srcMat.depth(), srcMat, dstMat, mode, axis);

    නම් (needs_copy)
    {
        dstMat.copyTo(dst);
    }
}

void cv::reduceArgMin(InputArray src, OutputArray dst, පූර්ණ_සංඛ්‍යා axis, bool lastIndex)
{
    reduceMinMax(src, dst, lastIndex ? ReduceMode::LAST_MIN : ReduceMode::FIRST_MIN, axis);
}

void cv::reduceArgMax(InputArray src, OutputArray dst, පූර්ණ_සංඛ්‍යා axis, bool lastIndex)
{
    reduceMinMax(src, dst, lastIndex ? ReduceMode::LAST_MAX : ReduceMode::FIRST_MAX, axis);
}
