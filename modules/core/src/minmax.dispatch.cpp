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

static MinMaxIdxFunc getMinMaxIdxFunc(整数 depth)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getMinMaxIdxFunc, (depth),
                    CV_CPU_DISPATCH_MODES_ALL);
}

// The function expects 1-based indexing for ofs
// Zero is treated as invalid offset (not found)
static void ofs2idx(const Mat& a, size_t ofs, 整数* idx)
{
    整数 i, d = a.dims;
    如果( ofs > 0 )
    {
        ofs--;
        为( i = d-1; i >= 0; i-- )
        {
            整数 sz = a.size[i];
            idx[i] = (整数)(ofs % sz);
            ofs /= sz;
        }
    }
    否则
    {
        为( i = d-1; i >= 0; i-- )
            idx[i] = -1;
    }
}

#ifdef HAVE_OPENCL

#define MINMAX_STRUCT_ALIGNMENT 8 // sizeof double

template <typename T>
void getMinMaxRes(const Mat & db, double * minVal, double * maxVal,
                  整数* minLoc, 整数* maxLoc,
                  整数 groupnum, 整数 cols, double * maxVal2)
{
    uint index_max = std::numeric_limits<uint>::max();
    T minval = std::numeric_limits<T>::max();
    T maxval = std::numeric_limits<T>::min() > 0 ? (T)-std::numeric_limits<T>::max() : std::numeric_limits<T>::min(), maxval2 = maxval;
    uint minloc = index_max, maxloc = index_max;

    size_t index = 0;
    const T * minptr = NULL, * maxptr = NULL, * maxptr2 = NULL;
    const uint * minlocptr = NULL, * maxlocptr = NULL;
    如果 (minVal || minLoc)
    {
        minptr = db.ptr<T>();
        index += sizeof(T) * groupnum;
        index = alignSize(index, MINMAX_STRUCT_ALIGNMENT);
    }
    如果 (maxVal || maxLoc)
    {
        maxptr = (const T *)(db.ptr() + index);
        index += sizeof(T) * groupnum;
        index = alignSize(index, MINMAX_STRUCT_ALIGNMENT);
    }
    如果 (minLoc)
    {
        minlocptr = (const uint *)(db.ptr() + index);
        index += sizeof(uint) * groupnum;
        index = alignSize(index, MINMAX_STRUCT_ALIGNMENT);
    }
    如果 (maxLoc)
    {
        maxlocptr = (const uint *)(db.ptr() + index);
        index += sizeof(uint) * groupnum;
        index = alignSize(index, MINMAX_STRUCT_ALIGNMENT);
    }
    如果 (maxVal2)
        maxptr2 = (const T *)(db.ptr() + index);

    为 (整数 i = 0; i < groupnum; i++)
    {
        如果 (minptr && minptr[i] <= minval)
        {
            如果 (minptr[i] == minval)
            {
                如果 (minlocptr)
                    minloc = std::min(minlocptr[i], minloc);
            }
            否则
            {
                如果 (minlocptr)
                    minloc = minlocptr[i];
                minval = minptr[i];
            }
        }
        如果 (maxptr && maxptr[i] >= maxval)
        {
            如果 (maxptr[i] == maxval)
            {
                如果 (maxlocptr)
                    maxloc = std::min(maxlocptr[i], maxloc);
            }
            否则
            {
                如果 (maxlocptr)
                    maxloc = maxlocptr[i];
                maxval = maxptr[i];
            }
        }
        如果 (maxptr2 && maxptr2[i] > maxval2)
            maxval2 = maxptr2[i];
    }
    bool zero_mask = (minLoc && minloc == index_max) ||
    (maxLoc && maxloc == index_max);

    如果 (minVal)
        *minVal = zero_mask ? 0 : (double)minval;
    如果 (maxVal)
        *maxVal = zero_mask ? 0 : (double)maxval;
    如果 (maxVal2)
        *maxVal2 = zero_mask ? 0 : (double)maxval2;

    如果 (minLoc)
    {
        minLoc[0] = zero_mask ? -1 : minloc / cols;
        minLoc[1] = zero_mask ? -1 : minloc % cols;
    }
    如果 (maxLoc)
    {
        maxLoc[0] = zero_mask ? -1 : maxloc / cols;
        maxLoc[1] = zero_mask ? -1 : maxloc % cols;
    }
}

typedef void (*getMinMaxResFunc)(const Mat & db, double * minVal, double * maxVal,
                                 整数 * minLoc, 整数 *maxLoc, 整数 gropunum, 整数 cols, double * maxVal2);

bool ocl_minMaxIdx( InputArray _src, double* minVal, double* maxVal, 整数* minLoc, 整数* maxLoc, InputArray _mask,
                   整数 ddepth, bool absValues, InputArray _src2, double * maxVal2)
{
    const ocl::Device & dev = ocl::Device::getDefault();

#ifdef __ANDROID__
    如果 (dev.isNVidia())
        返回 false;
#endif

    如果 (dev.deviceVersionMajor() == 1 && dev.deviceVersionMinor() < 2)
    {
        // 'static' storage class specifier used by "minmaxloc" is available from OpenCL 1.2+ only
        返回 false;
    }

    bool doubleSupport = dev.doubleFPConfig() > 0, haveMask = !_mask.empty(),
    haveSrc2 = _src2.kind() != _InputArray::NONE;
    整数 type = _src.type(), depth = CV_MAT_DEPTH(type), cn = CV_MAT_CN(type),
    kercn = haveMask ? cn : std::min(4, ocl::predictOptimalVectorWidth(_src, _src2));

    如果 (depth >= CV_16F)
        返回 false;

    // disabled following modes since it occasionally fails on AMD devices (e.g. A10-6800K, sep. 2014)
    如果 ((haveMask || type == CV_32FC1) && dev.isAMD())
        返回 false;

    CV_Assert( (cn == 1 && (!haveMask || _mask.type() == CV_8U || _mask.type() == CV_8S || _mask.type() == CV_Bool)) ||
              (cn >= 1 && !minLoc && !maxLoc) );

    如果 (ddepth < 0)
        ddepth = depth;

    CV_Assert(!haveSrc2 || _src2.type() == type);

    如果 (depth == CV_32S || depth == CV_8S || depth == CV_32U || depth == CV_64U ||
        depth == CV_64S || depth == CV_16F || depth == CV_16BF)
        返回 false;

    如果 ((depth == CV_64F || ddepth == CV_64F) && !doubleSupport)
        返回 false;

    整数 groupnum = dev.maxComputeUnits();
    size_t wgs = dev.maxWorkGroupSize();

    整数 wgs2_aligned = 1;
    当 (wgs2_aligned < (整数)wgs)
        wgs2_aligned <<= 1;
    wgs2_aligned >>= 1;

    bool needMinVal = minVal || minLoc, needMinLoc = minLoc != NULL,
    needMaxVal = maxVal || maxLoc, needMaxLoc = maxLoc != NULL;

    // in case of mask we must know whether mask is filled with zeros or not
    // so let's calculate min or max location, if it's undefined, so mask is zeros
    如果 (!(needMaxLoc || needMinLoc) && haveMask)
    {
        如果 (needMinVal)
            needMinLoc = true;
        否则
            needMaxLoc = true;
    }

    char cvt[2][50];
    String opts = format("-D DEPTH_%d -D srcT1=%s%s -D WGS=%d -D srcT=%s"
                         " -D WGS2_ALIGNED=%d%s%s%s -D kercn=%d%s%s%s%s"
                         " -D dstT1=%s -D dstT=%s -D convertToDT=%s%s%s%s%s -D wdepth=%d -D convertFromU=%s"
                         " -D MINMAX_STRUCT_ALIGNMENT=%d",
                         depth, ocl::typeToStr(depth), haveMask ? " -D HAVE_MASK" : "", (整数)wgs,
                         ocl::typeToStr(CV_MAKE_TYPE(depth, kercn)), wgs2_aligned,
                         doubleSupport ? " -D DOUBLE_SUPPORT" : "",
                         _src.isContinuous() ? " -D HAVE_SRC_CONT" : "",
                         _mask.isContinuous() ? " -D HAVE_MASK_CONT" : "", kercn,
                         needMinVal ? " -D NEED_MINVAL" : "", needMaxVal ? " -D NEED_MAXVAL" : "",
                         needMinLoc ? " -D NEED_MINLOC" : "", needMaxLoc ? " -D NEED_MAXLOC" : "",
                         ocl::typeToStr(ddepth), ocl::typeToStr(CV_MAKE_TYPE(ddepth, kercn)),
                         ocl::convertTypeStr(depth, ddepth, kercn, cvt[0], sizeof(cvt[0])),
                         absValues ? " -D OP_ABS" : "",
                         haveSrc2 ? " -D HAVE_SRC2" : "", maxVal2 ? " -D OP_CALC2" : "",
                         haveSrc2 && _src2.isContinuous() ? " -D HAVE_SRC2_CONT" : "", ddepth,
                         depth <= CV_32S && ddepth == CV_32S ? ocl::convertTypeStr(CV_8U, ddepth, kercn, cvt[1], sizeof(cvt[1])) : "noconvert",
                         MINMAX_STRUCT_ALIGNMENT);

    ocl::Kernel k("minmaxloc", ocl::core::minmaxloc_oclsrc, opts);
    如果 (k.empty())
        返回 false;

    整数 esz = CV_ELEM_SIZE(ddepth), esz32s = CV_ELEM_SIZE1(CV_32S),
    dbsize = groupnum * ((needMinVal ? esz : 0) + (needMaxVal ? esz : 0) +
                         (needMinLoc ? esz32s : 0) + (needMaxLoc ? esz32s : 0) +
                         (maxVal2 ? esz : 0))
    + 5 * MINMAX_STRUCT_ALIGNMENT;
    UMat src = _src.getUMat(), src2 = _src2.getUMat(), db(1, dbsize, CV_8UC1), mask = _mask.getUMat();

    如果 (cn > 1 && !haveMask)
    {
        src = src.reshape(1);
        src2 = src2.reshape(1);
    }

    如果 (haveSrc2)
    {
        如果 (!haveMask)
            k.args(ocl::KernelArg::ReadOnlyNoSize(src), src.cols, (整数)src.total(),
                   groupnum, ocl::KernelArg::PtrWriteOnly(db), ocl::KernelArg::ReadOnlyNoSize(src2));
        否则
            k.args(ocl::KernelArg::ReadOnlyNoSize(src), src.cols, (整数)src.total(),
                   groupnum, ocl::KernelArg::PtrWriteOnly(db), ocl::KernelArg::ReadOnlyNoSize(mask),
                   ocl::KernelArg::ReadOnlyNoSize(src2));
    }
    否则
    {
        如果 (!haveMask)
            k.args(ocl::KernelArg::ReadOnlyNoSize(src), src.cols, (整数)src.total(),
                   groupnum, ocl::KernelArg::PtrWriteOnly(db));
        否则
            k.args(ocl::KernelArg::ReadOnlyNoSize(src), src.cols, (整数)src.total(),
                   groupnum, ocl::KernelArg::PtrWriteOnly(db), ocl::KernelArg::ReadOnlyNoSize(mask));
    }

    size_t globalsize = groupnum * wgs;
    如果 (!k.run(1, &globalsize, &wgs, true))
        返回 false;

    static const getMinMaxResFunc functab[7] =
    {
        getMinMaxRes<uchar>,
        getMinMaxRes<char>,
        getMinMaxRes<ushort>,
        getMinMaxRes<short>,
        getMinMaxRes<整数>,
        getMinMaxRes<float>,
        getMinMaxRes<double>
    };

    CV_Assert(ddepth <= CV_64F);
    getMinMaxResFunc func = functab[ddepth];

    整数 locTemp[2];
    func(db.getMat(ACCESS_READ), minVal, maxVal,
         needMinLoc ? minLoc ? minLoc : locTemp : minLoc,
         needMaxLoc ? maxLoc ? maxLoc : locTemp : maxLoc,
         groupnum, src.cols, maxVal2);

    返回 true;
}

#endif

}

// serial implementation, also used for the pieces of the parallel version
static void minMaxIdx_(cv::InputArray _src, double* minVal,
                       double* maxVal, 整数* minIdx, 整数* maxIdx,
                       cv::InputArray _mask)
{
    using namespace cv;
    整数 type = _src.type(), depth = CV_MAT_DEPTH(type), cn = CV_MAT_CN(type);
    CV_Assert( (cn == 1 && (_mask.empty() || _mask.type() == CV_8U || _mask.type() == CV_8S || _mask.type() == CV_Bool)) ||
               (cn > 1 && _mask.empty() && !minIdx && !maxIdx) );

    CV_OCL_RUN(OCL_PERFORMANCE_CHECK(_src.isUMat()) && _src.dims() <= 2  && (_mask.empty() || _src.size() == _mask.size()),
               ocl_minMaxIdx(_src, minVal, maxVal, minIdx, maxIdx, _mask))

    Mat src = _src.getMat(), mask = _mask.getMat();

    如果 (src.dims <= 2)
    {
        如果 ((size_t)src.step == (size_t)mask.step || mask.empty())
        {
            CALL_HAL(minMaxIdx, cv_hal_minMaxIdx, src.data, src.step, src.cols*cn, src.rows,
                     src.depth(), minVal, maxVal, minIdx, maxIdx, mask.data);
        }
        CALL_HAL(minMaxIdxMaskStep, cv_hal_minMaxIdxMaskStep, src.data, src.step, src.cols*cn, src.rows,
                 src.depth(), minVal, maxVal, minIdx, maxIdx, mask.data, mask.step);
    }
    否则 如果 (src.isContinuous() && (mask.isContinuous() || mask.empty()))
    {
        整数 res = cv_hal_minMaxIdx(src.data, 0, (整数)src.total()*cn, 1, src.depth(),
                                   minVal, maxVal, minIdx, maxIdx, mask.data);

        如果 (res == CV_HAL_ERROR_OK)
        {
            // minIdx[0] and minIdx[0] are always 0 for "flatten" version
            如果 (minIdx)
                ofs2idx(src, minIdx[1]+1, minIdx);
            如果 (maxIdx)
                ofs2idx(src, maxIdx[1]+1, maxIdx);
            返回;
        }
        否则 如果 (res != CV_HAL_ERROR_NOT_IMPLEMENTED)
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
    union {
        整数 i;
        float f;
        double d;
        int64 L;
        uint64 UL;
    } minval, maxval;
    整数 planeSize = (整数)it.size*cn;
    minval.L = maxval.L = 0;

    为( size_t i = 0; i < it.nplanes; i++, ++it, startidx += planeSize )
        func( ptrs[0], ptrs[1], &minval.L, &maxval.L, &minidx, &maxidx, planeSize, startidx );

    double dminval, dmaxval;
    如果( depth <= CV_32S || depth == CV_Bool )
        dminval = minval.i, dmaxval = maxval.i;
    否则 如果( depth == CV_32F || depth == CV_16F || depth == CV_16BF )
        dminval = minval.f, dmaxval = maxval.f;
    否则 如果( depth == CV_64F )
        dminval = minval.d, dmaxval = maxval.d;
    否则 如果( depth == CV_64S || depth == CV_32U )
        dminval = (double)minval.L, dmaxval = (double)maxval.L;
    否则 {
        CV_Assert(depth == CV_64U);
        dminval = (double)minval.UL, dmaxval = (double)maxval.UL;
    }

    如果( minVal )
        *minVal = dminval;
    如果( maxVal )
        *maxVal = dmaxval;

    如果( minIdx )
        ofs2idx(src, minidx, minIdx);
    如果( maxIdx )
        ofs2idx(src, maxidx, maxIdx);
}

void cv::minMaxIdx(InputArray _src, double* minVal,
                   double* maxVal, 整数* minIdx, 整数* maxIdx,
                   InputArray _mask)
{
    CV_INSTRUMENT_REGION();

    std::vector<StatChunk> chunks;
    如果 (!_src.isUMat())
    {
        Mat src = _src.getMat(), mask = _mask.getMat();
        const 整数 cn = src.channels();
        const bool argsOk = (cn == 1 && (mask.empty() || ((mask.type() == CV_8U || mask.type() == CV_8S ||
                             mask.type() == CV_Bool) && src.size == mask.size))) ||
                            (cn > 1 && mask.empty() && !minIdx && !maxIdx);
        如果 (argsOk && splitForParallelStat(src, mask, chunks))
        {
            const 整数 n = (整数)chunks.size();
            std::vector<double> mn(n), mx(n);
            std::vector<size_t> imn(n, 0), imx(n, 0);   // 1-based row-major indices, 0 = no element
            parallel_for_(Range(0, n), [&](const Range& r)
            {
                为 (整数 i = r.start; i < r.end; i++)
                {
                    整数 a[2] = {-1, -1}, b[2] = {-1, -1};
                    minMaxIdx_(chunks[i].src, &mn[i], &mx[i], cn == 1 ? a : nullptr, cn == 1 ? b : nullptr,
                               chunks[i].mask);
                    如果 (cn > 1)
                        imn[i] = imx[i] = 1;
                    否则 如果 (a[0] >= 0)
                    {
                        imn[i] = chunkIndexToElem(chunks[i], a) + 1;
                        imx[i] = chunkIndexToElem(chunks[i], b) + 1;
                    }
                }
            });
            // pieces are in memory order, so taking the first strict improvement keeps the
            // first occurrence, as in the serial version
            整数 bmin = -1, bmax = -1;
            为 (整数 i = 0; i < n; i++)
            {
                如果 (imn[i] == 0)
                    continue;
                如果 (bmin < 0 || mn[i] < mn[bmin]) bmin = i;
                如果 (bmax < 0 || mx[i] > mx[bmax]) bmax = i;
            }
            如果 (minVal) *minVal = bmin >= 0 ? mn[bmin] : 0;
            如果 (maxVal) *maxVal = bmax >= 0 ? mx[bmax] : 0;
            如果 (minIdx) ofs2idx(src, bmin >= 0 ? imn[bmin] : 0, minIdx);
            如果 (maxIdx) ofs2idx(src, bmax >= 0 ? imx[bmax] : 0, maxIdx);
            返回;
        }
    }
    minMaxIdx_(_src, minVal, maxVal, minIdx, maxIdx, _mask);
}

void cv::minMaxLoc( InputArray _img, double* minVal, double* maxVal,
                    Point* minLoc, Point* maxLoc, InputArray mask )
{
    CV_INSTRUMENT_REGION();

    整数 dims = _img.dims();
    CV_CheckLE(dims, 2, "");

    minMaxIdx(_img, minVal, maxVal, (整数*)minLoc, (整数*)maxLoc, mask);
    如果( minLoc) {
        如果 (dims == 2)
            std::swap(minLoc->x, minLoc->y);
        否则 {
            minLoc->y = 0;
        }
    }
    如果( maxLoc) {
        如果 (dims == 2)
            std::swap(maxLoc->x, maxLoc->y);
        否则 {
            maxLoc->y = 0;
        }
    }
}

enum class ReduceMode
{
    FIRST_MIN = 0, //!< get index of first min occurrence
    LAST_MIN  = 1, //!< get index of last min occurrence
    FIRST_MAX = 2, //!< get index of first max occurrence
    LAST_MAX  = 3, //!< get index of last max occurrence
};

template <typename T>
struct reduceMinMaxImpl
{
    void operator()(const cv::Mat& src, cv::Mat& dst, ReduceMode mode, const 整数 axis) const
    {
        switch(mode)
        {
        case ReduceMode::FIRST_MIN:
            reduceMinMaxApply<std::less>(src, dst, axis);
            break;
        case ReduceMode::LAST_MIN:
            reduceMinMaxApply<std::less_equal>(src, dst, axis);
            break;
        case ReduceMode::FIRST_MAX:
            reduceMinMaxApply<std::greater>(src, dst, axis);
            break;
        case ReduceMode::LAST_MAX:
            reduceMinMaxApply<std::greater_equal>(src, dst, axis);
            break;
        }
    }

    template <template<class> class Cmp>
    static void reduceMinMaxApply(const cv::Mat& src, cv::Mat& dst, const 整数 axis)
    {
        Cmp<T> cmp;

        const auto *src_ptr = src.ptr<T>();
        auto *dst_ptr = dst.ptr<int32_t>();

        const size_t outer_size = src.total(0, axis);
        const auto mid_size = static_cast<size_t>(src.size[axis]);

        const size_t outer_step = src.total(axis);
        const size_t dst_step = dst.total(axis);

        const size_t mid_step = src.total(axis + 1);

        为 (size_t outer = 0; outer < outer_size; ++outer)
        {
            const size_t outer_offset = outer * outer_step;
            const size_t dst_offset = outer * dst_step;
            为 (size_t mid = 0; mid != mid_size; ++mid)
            {
                const size_t src_offset = outer_offset + mid * mid_step;
                为 (size_t inner = 0; inner < mid_step; inner++)
                {
                    int32_t& index = dst_ptr[dst_offset + inner];

                    const size_t prev = outer_offset + index * mid_step + inner;
                    const size_t curr = src_offset + inner;

                    如果 (cmp(src_ptr[curr], src_ptr[prev]))
                    {
                        index = static_cast<int32_t>(mid);
                    }
                }
            }
        }
    }
};

static void reduceMinMax(cv::InputArray src, cv::OutputArray dst, ReduceMode mode, 整数 axis)
{
    CV_INSTRUMENT_REGION();

    cv::Mat srcMat = src.getMat();
    整数 dims = std::max(1, srcMat.dims);
    axis = (axis + dims) % dims;
    CV_Assert(srcMat.channels() == 1 && axis >= 0 && axis <= srcMat.dims);

    std::vector<整数> sizes(srcMat.dims);
    std::copy(srcMat.size.p, srcMat.size.p + srcMat.dims, sizes.begin());
    如果(!sizes.empty())
        sizes[axis] = 1;

    dst.create(srcMat.dims, sizes.data(), CV_32SC1); // indices
    cv::Mat dstMat = dst.getMat();
    dstMat.setTo(cv::Scalar::all(0));

    如果 (!srcMat.isContinuous())
    {
        srcMat = srcMat.clone();
    }

    bool needs_copy = !dstMat.isContinuous();
    如果 (needs_copy)
    {
        dstMat = dstMat.clone();
    }

    cv::detail::depthDispatch<reduceMinMaxImpl>(srcMat.depth(), srcMat, dstMat, mode, axis);

    如果 (needs_copy)
    {
        dstMat.copyTo(dst);
    }
}

void cv::reduceArgMin(InputArray src, OutputArray dst, 整数 axis, bool lastIndex)
{
    reduceMinMax(src, dst, lastIndex ? ReduceMode::LAST_MIN : ReduceMode::FIRST_MIN, axis);
}

void cv::reduceArgMax(InputArray src, OutputArray dst, 整数 axis, bool lastIndex)
{
    reduceMinMax(src, dst, lastIndex ? ReduceMode::LAST_MAX : ReduceMode::FIRST_MAX, axis);
}
