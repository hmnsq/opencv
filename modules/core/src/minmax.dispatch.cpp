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

static MinMaxIdxFunc getMinMaxIdxFunc(целое depth)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getMinMaxIdxFunc, (depth),
                    CV_CPU_DISPATCH_MODES_ALL);
}

// The function expects 1-based indexing for ofs
// Zero is treated as invalid offset (not found)
static void ofs2idx(const Mat& a, size_t ofs, целое* idx)
{
    целое i, d = a.dims;
    если( ofs > 0 )
    {
        ofs--;
        для( i = d-1; i >= 0; i-- )
        {
            целое sz = a.size[i];
            idx[i] = (целое)(ofs % sz);
            ofs /= sz;
        }
    }
    иначе
    {
        для( i = d-1; i >= 0; i-- )
            idx[i] = -1;
    }
}

#ifdef HAVE_OPENCL

#define MINMAX_STRUCT_ALIGNMENT 8 // sizeof double

template <typename T>
void getMinMaxRes(const Mat & db, double * minVal, double * maxVal,
                  целое* minLoc, целое* maxLoc,
                  целое groupnum, целое cols, double * maxVal2)
{
    uint index_max = std::numeric_limits<uint>::max();
    T minval = std::numeric_limits<T>::max();
    T maxval = std::numeric_limits<T>::min() > 0 ? (T)-std::numeric_limits<T>::max() : std::numeric_limits<T>::min(), maxval2 = maxval;
    uint minloc = index_max, maxloc = index_max;

    size_t index = 0;
    const T * minptr = NULL, * maxptr = NULL, * maxptr2 = NULL;
    const uint * minlocptr = NULL, * maxlocptr = NULL;
    если (minVal || minLoc)
    {
        minptr = db.ptr<T>();
        index += sizeof(T) * groupnum;
        index = alignSize(index, MINMAX_STRUCT_ALIGNMENT);
    }
    если (maxVal || maxLoc)
    {
        maxptr = (const T *)(db.ptr() + index);
        index += sizeof(T) * groupnum;
        index = alignSize(index, MINMAX_STRUCT_ALIGNMENT);
    }
    если (minLoc)
    {
        minlocptr = (const uint *)(db.ptr() + index);
        index += sizeof(uint) * groupnum;
        index = alignSize(index, MINMAX_STRUCT_ALIGNMENT);
    }
    если (maxLoc)
    {
        maxlocptr = (const uint *)(db.ptr() + index);
        index += sizeof(uint) * groupnum;
        index = alignSize(index, MINMAX_STRUCT_ALIGNMENT);
    }
    если (maxVal2)
        maxptr2 = (const T *)(db.ptr() + index);

    для (целое i = 0; i < groupnum; i++)
    {
        если (minptr && minptr[i] <= minval)
        {
            если (minptr[i] == minval)
            {
                если (minlocptr)
                    minloc = std::min(minlocptr[i], minloc);
            }
            иначе
            {
                если (minlocptr)
                    minloc = minlocptr[i];
                minval = minptr[i];
            }
        }
        если (maxptr && maxptr[i] >= maxval)
        {
            если (maxptr[i] == maxval)
            {
                если (maxlocptr)
                    maxloc = std::min(maxlocptr[i], maxloc);
            }
            иначе
            {
                если (maxlocptr)
                    maxloc = maxlocptr[i];
                maxval = maxptr[i];
            }
        }
        если (maxptr2 && maxptr2[i] > maxval2)
            maxval2 = maxptr2[i];
    }
    bool zero_mask = (minLoc && minloc == index_max) ||
    (maxLoc && maxloc == index_max);

    если (minVal)
        *minVal = zero_mask ? 0 : (double)minval;
    если (maxVal)
        *maxVal = zero_mask ? 0 : (double)maxval;
    если (maxVal2)
        *maxVal2 = zero_mask ? 0 : (double)maxval2;

    если (minLoc)
    {
        minLoc[0] = zero_mask ? -1 : minloc / cols;
        minLoc[1] = zero_mask ? -1 : minloc % cols;
    }
    если (maxLoc)
    {
        maxLoc[0] = zero_mask ? -1 : maxloc / cols;
        maxLoc[1] = zero_mask ? -1 : maxloc % cols;
    }
}

typedef void (*getMinMaxResFunc)(const Mat & db, double * minVal, double * maxVal,
                                 целое * minLoc, целое *maxLoc, целое gropunum, целое cols, double * maxVal2);

bool ocl_minMaxIdx( InputArray _src, double* minVal, double* maxVal, целое* minLoc, целое* maxLoc, InputArray _mask,
                   целое ddepth, bool absValues, InputArray _src2, double * maxVal2)
{
    const ocl::Device & dev = ocl::Device::getDefault();

#ifdef __ANDROID__
    если (dev.isNVidia())
        вернуть false;
#endif

    если (dev.deviceVersionMajor() == 1 && dev.deviceVersionMinor() < 2)
    {
        // 'static' storage class specifier used by "minmaxloc" is available from OpenCL 1.2+ only
        вернуть false;
    }

    bool doubleSupport = dev.doubleFPConfig() > 0, haveMask = !_mask.empty(),
    haveSrc2 = _src2.kind() != _InputArray::NONE;
    целое type = _src.type(), depth = CV_MAT_DEPTH(type), cn = CV_MAT_CN(type),
    kercn = haveMask ? cn : std::min(4, ocl::predictOptimalVectorWidth(_src, _src2));

    если (depth >= CV_16F)
        вернуть false;

    // disabled following modes since it occasionally fails on AMD devices (e.g. A10-6800K, sep. 2014)
    если ((haveMask || type == CV_32FC1) && dev.isAMD())
        вернуть false;

    CV_Assert( (cn == 1 && (!haveMask || _mask.type() == CV_8U || _mask.type() == CV_8S || _mask.type() == CV_Bool)) ||
              (cn >= 1 && !minLoc && !maxLoc) );

    если (ddepth < 0)
        ddepth = depth;

    CV_Assert(!haveSrc2 || _src2.type() == type);

    если (depth == CV_32S || depth == CV_8S || depth == CV_32U || depth == CV_64U ||
        depth == CV_64S || depth == CV_16F || depth == CV_16BF)
        вернуть false;

    если ((depth == CV_64F || ddepth == CV_64F) && !doubleSupport)
        вернуть false;

    целое groupnum = dev.maxComputeUnits();
    size_t wgs = dev.maxWorkGroupSize();

    целое wgs2_aligned = 1;
    пока (wgs2_aligned < (целое)wgs)
        wgs2_aligned <<= 1;
    wgs2_aligned >>= 1;

    bool needMinVal = minVal || minLoc, needMinLoc = minLoc != NULL,
    needMaxVal = maxVal || maxLoc, needMaxLoc = maxLoc != NULL;

    // in case of mask we must know whether mask is filled with zeros or not
    // so let's calculate min or max location, if it's undefined, so mask is zeros
    если (!(needMaxLoc || needMinLoc) && haveMask)
    {
        если (needMinVal)
            needMinLoc = true;
        иначе
            needMaxLoc = true;
    }

    char cvt[2][50];
    String opts = format("-D DEPTH_%d -D srcT1=%s%s -D WGS=%d -D srcT=%s"
                         " -D WGS2_ALIGNED=%d%s%s%s -D kercn=%d%s%s%s%s"
                         " -D dstT1=%s -D dstT=%s -D convertToDT=%s%s%s%s%s -D wdepth=%d -D convertFromU=%s"
                         " -D MINMAX_STRUCT_ALIGNMENT=%d",
                         depth, ocl::typeToStr(depth), haveMask ? " -D HAVE_MASK" : "", (целое)wgs,
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
    если (k.empty())
        вернуть false;

    целое esz = CV_ELEM_SIZE(ddepth), esz32s = CV_ELEM_SIZE1(CV_32S),
    dbsize = groupnum * ((needMinVal ? esz : 0) + (needMaxVal ? esz : 0) +
                         (needMinLoc ? esz32s : 0) + (needMaxLoc ? esz32s : 0) +
                         (maxVal2 ? esz : 0))
    + 5 * MINMAX_STRUCT_ALIGNMENT;
    UMat src = _src.getUMat(), src2 = _src2.getUMat(), db(1, dbsize, CV_8UC1), mask = _mask.getUMat();

    если (cn > 1 && !haveMask)
    {
        src = src.reshape(1);
        src2 = src2.reshape(1);
    }

    если (haveSrc2)
    {
        если (!haveMask)
            k.args(ocl::KernelArg::ReadOnlyNoSize(src), src.cols, (целое)src.total(),
                   groupnum, ocl::KernelArg::PtrWriteOnly(db), ocl::KernelArg::ReadOnlyNoSize(src2));
        иначе
            k.args(ocl::KernelArg::ReadOnlyNoSize(src), src.cols, (целое)src.total(),
                   groupnum, ocl::KernelArg::PtrWriteOnly(db), ocl::KernelArg::ReadOnlyNoSize(mask),
                   ocl::KernelArg::ReadOnlyNoSize(src2));
    }
    иначе
    {
        если (!haveMask)
            k.args(ocl::KernelArg::ReadOnlyNoSize(src), src.cols, (целое)src.total(),
                   groupnum, ocl::KernelArg::PtrWriteOnly(db));
        иначе
            k.args(ocl::KernelArg::ReadOnlyNoSize(src), src.cols, (целое)src.total(),
                   groupnum, ocl::KernelArg::PtrWriteOnly(db), ocl::KernelArg::ReadOnlyNoSize(mask));
    }

    size_t globalsize = groupnum * wgs;
    если (!k.run(1, &globalsize, &wgs, true))
        вернуть false;

    static const getMinMaxResFunc functab[7] =
    {
        getMinMaxRes<uchar>,
        getMinMaxRes<char>,
        getMinMaxRes<ushort>,
        getMinMaxRes<short>,
        getMinMaxRes<целое>,
        getMinMaxRes<float>,
        getMinMaxRes<double>
    };

    CV_Assert(ddepth <= CV_64F);
    getMinMaxResFunc func = functab[ddepth];

    целое locTemp[2];
    func(db.getMat(ACCESS_READ), minVal, maxVal,
         needMinLoc ? minLoc ? minLoc : locTemp : minLoc,
         needMaxLoc ? maxLoc ? maxLoc : locTemp : maxLoc,
         groupnum, src.cols, maxVal2);

    вернуть true;
}

#endif

}

// serial implementation, also used for the pieces of the parallel version
static void minMaxIdx_(cv::InputArray _src, double* minVal,
                       double* maxVal, целое* minIdx, целое* maxIdx,
                       cv::InputArray _mask)
{
    using namespace cv;
    целое type = _src.type(), depth = CV_MAT_DEPTH(type), cn = CV_MAT_CN(type);
    CV_Assert( (cn == 1 && (_mask.empty() || _mask.type() == CV_8U || _mask.type() == CV_8S || _mask.type() == CV_Bool)) ||
               (cn > 1 && _mask.empty() && !minIdx && !maxIdx) );

    CV_OCL_RUN(OCL_PERFORMANCE_CHECK(_src.isUMat()) && _src.dims() <= 2  && (_mask.empty() || _src.size() == _mask.size()),
               ocl_minMaxIdx(_src, minVal, maxVal, minIdx, maxIdx, _mask))

    Mat src = _src.getMat(), mask = _mask.getMat();

    если (src.dims <= 2)
    {
        если ((size_t)src.step == (size_t)mask.step || mask.empty())
        {
            CALL_HAL(minMaxIdx, cv_hal_minMaxIdx, src.data, src.step, src.cols*cn, src.rows,
                     src.depth(), minVal, maxVal, minIdx, maxIdx, mask.data);
        }
        CALL_HAL(minMaxIdxMaskStep, cv_hal_minMaxIdxMaskStep, src.data, src.step, src.cols*cn, src.rows,
                 src.depth(), minVal, maxVal, minIdx, maxIdx, mask.data, mask.step);
    }
    иначе если (src.isContinuous() && (mask.isContinuous() || mask.empty()))
    {
        целое res = cv_hal_minMaxIdx(src.data, 0, (целое)src.total()*cn, 1, src.depth(),
                                   minVal, maxVal, minIdx, maxIdx, mask.data);

        если (res == CV_HAL_ERROR_OK)
        {
            // minIdx[0] and minIdx[0] are always 0 for "flatten" version
            если (minIdx)
                ofs2idx(src, minIdx[1]+1, minIdx);
            если (maxIdx)
                ofs2idx(src, maxIdx[1]+1, maxIdx);
            вернуть;
        }
        иначе если (res != CV_HAL_ERROR_NOT_IMPLEMENTED)
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
        целое i;
        float f;
        double d;
        int64 L;
        uint64 UL;
    } minval, maxval;
    целое planeSize = (целое)it.size*cn;
    minval.L = maxval.L = 0;

    для( size_t i = 0; i < it.nplanes; i++, ++it, startidx += planeSize )
        func( ptrs[0], ptrs[1], &minval.L, &maxval.L, &minidx, &maxidx, planeSize, startidx );

    double dminval, dmaxval;
    если( depth <= CV_32S || depth == CV_Bool )
        dminval = minval.i, dmaxval = maxval.i;
    иначе если( depth == CV_32F || depth == CV_16F || depth == CV_16BF )
        dminval = minval.f, dmaxval = maxval.f;
    иначе если( depth == CV_64F )
        dminval = minval.d, dmaxval = maxval.d;
    иначе если( depth == CV_64S || depth == CV_32U )
        dminval = (double)minval.L, dmaxval = (double)maxval.L;
    иначе {
        CV_Assert(depth == CV_64U);
        dminval = (double)minval.UL, dmaxval = (double)maxval.UL;
    }

    если( minVal )
        *minVal = dminval;
    если( maxVal )
        *maxVal = dmaxval;

    если( minIdx )
        ofs2idx(src, minidx, minIdx);
    если( maxIdx )
        ofs2idx(src, maxidx, maxIdx);
}

void cv::minMaxIdx(InputArray _src, double* minVal,
                   double* maxVal, целое* minIdx, целое* maxIdx,
                   InputArray _mask)
{
    CV_INSTRUMENT_REGION();

    std::vector<StatChunk> chunks;
    если (!_src.isUMat())
    {
        Mat src = _src.getMat(), mask = _mask.getMat();
        const целое cn = src.channels();
        const bool argsOk = (cn == 1 && (mask.empty() || ((mask.type() == CV_8U || mask.type() == CV_8S ||
                             mask.type() == CV_Bool) && src.size == mask.size))) ||
                            (cn > 1 && mask.empty() && !minIdx && !maxIdx);
        если (argsOk && splitForParallelStat(src, mask, chunks))
        {
            const целое n = (целое)chunks.size();
            std::vector<double> mn(n), mx(n);
            std::vector<size_t> imn(n, 0), imx(n, 0);   // 1-based row-major indices, 0 = no element
            parallel_for_(Range(0, n), [&](const Range& r)
            {
                для (целое i = r.start; i < r.end; i++)
                {
                    целое a[2] = {-1, -1}, b[2] = {-1, -1};
                    minMaxIdx_(chunks[i].src, &mn[i], &mx[i], cn == 1 ? a : nullptr, cn == 1 ? b : nullptr,
                               chunks[i].mask);
                    если (cn > 1)
                        imn[i] = imx[i] = 1;
                    иначе если (a[0] >= 0)
                    {
                        imn[i] = chunkIndexToElem(chunks[i], a) + 1;
                        imx[i] = chunkIndexToElem(chunks[i], b) + 1;
                    }
                }
            });
            // pieces are in memory order, so taking the first strict improvement keeps the
            // first occurrence, as in the serial version
            целое bmin = -1, bmax = -1;
            для (целое i = 0; i < n; i++)
            {
                если (imn[i] == 0)
                    continue;
                если (bmin < 0 || mn[i] < mn[bmin]) bmin = i;
                если (bmax < 0 || mx[i] > mx[bmax]) bmax = i;
            }
            если (minVal) *minVal = bmin >= 0 ? mn[bmin] : 0;
            если (maxVal) *maxVal = bmax >= 0 ? mx[bmax] : 0;
            если (minIdx) ofs2idx(src, bmin >= 0 ? imn[bmin] : 0, minIdx);
            если (maxIdx) ofs2idx(src, bmax >= 0 ? imx[bmax] : 0, maxIdx);
            вернуть;
        }
    }
    minMaxIdx_(_src, minVal, maxVal, minIdx, maxIdx, _mask);
}

void cv::minMaxLoc( InputArray _img, double* minVal, double* maxVal,
                    Point* minLoc, Point* maxLoc, InputArray mask )
{
    CV_INSTRUMENT_REGION();

    целое dims = _img.dims();
    CV_CheckLE(dims, 2, "");

    minMaxIdx(_img, minVal, maxVal, (целое*)minLoc, (целое*)maxLoc, mask);
    если( minLoc) {
        если (dims == 2)
            std::swap(minLoc->x, minLoc->y);
        иначе {
            minLoc->y = 0;
        }
    }
    если( maxLoc) {
        если (dims == 2)
            std::swap(maxLoc->x, maxLoc->y);
        иначе {
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
    void operator()(const cv::Mat& src, cv::Mat& dst, ReduceMode mode, const целое axis) const
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
    static void reduceMinMaxApply(const cv::Mat& src, cv::Mat& dst, const целое axis)
    {
        Cmp<T> cmp;

        const auto *src_ptr = src.ptr<T>();
        auto *dst_ptr = dst.ptr<int32_t>();

        const size_t outer_size = src.total(0, axis);
        const auto mid_size = static_cast<size_t>(src.size[axis]);

        const size_t outer_step = src.total(axis);
        const size_t dst_step = dst.total(axis);

        const size_t mid_step = src.total(axis + 1);

        для (size_t outer = 0; outer < outer_size; ++outer)
        {
            const size_t outer_offset = outer * outer_step;
            const size_t dst_offset = outer * dst_step;
            для (size_t mid = 0; mid != mid_size; ++mid)
            {
                const size_t src_offset = outer_offset + mid * mid_step;
                для (size_t inner = 0; inner < mid_step; inner++)
                {
                    int32_t& index = dst_ptr[dst_offset + inner];

                    const size_t prev = outer_offset + index * mid_step + inner;
                    const size_t curr = src_offset + inner;

                    если (cmp(src_ptr[curr], src_ptr[prev]))
                    {
                        index = static_cast<int32_t>(mid);
                    }
                }
            }
        }
    }
};

static void reduceMinMax(cv::InputArray src, cv::OutputArray dst, ReduceMode mode, целое axis)
{
    CV_INSTRUMENT_REGION();

    cv::Mat srcMat = src.getMat();
    целое dims = std::max(1, srcMat.dims);
    axis = (axis + dims) % dims;
    CV_Assert(srcMat.channels() == 1 && axis >= 0 && axis <= srcMat.dims);

    std::vector<целое> sizes(srcMat.dims);
    std::copy(srcMat.size.p, srcMat.size.p + srcMat.dims, sizes.begin());
    если(!sizes.empty())
        sizes[axis] = 1;

    dst.create(srcMat.dims, sizes.data(), CV_32SC1); // indices
    cv::Mat dstMat = dst.getMat();
    dstMat.setTo(cv::Scalar::all(0));

    если (!srcMat.isContinuous())
    {
        srcMat = srcMat.clone();
    }

    bool needs_copy = !dstMat.isContinuous();
    если (needs_copy)
    {
        dstMat = dstMat.clone();
    }

    cv::detail::depthDispatch<reduceMinMaxImpl>(srcMat.depth(), srcMat, dstMat, mode, axis);

    если (needs_copy)
    {
        dstMat.copyTo(dst);
    }
}

void cv::reduceArgMin(InputArray src, OutputArray dst, целое axis, bool lastIndex)
{
    reduceMinMax(src, dst, lastIndex ? ReduceMode::LAST_MIN : ReduceMode::FIRST_MIN, axis);
}

void cv::reduceArgMax(InputArray src, OutputArray dst, целое axis, bool lastIndex)
{
    reduceMinMax(src, dst, lastIndex ? ReduceMode::LAST_MAX : ReduceMode::FIRST_MAX, axis);
}
