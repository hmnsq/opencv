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

static MinMaxIdxFunc getMinMaxIdxFunc(ຈຳນວນເຕັມ depth)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getMinMaxIdxFunc, (depth),
                    CV_CPU_DISPATCH_MODES_ALL);
}

// The function expects 1-based indexing for ofs
// Zero is treated as invalid offset (not found)
static void ofs2idx(const Mat& a, size_t ofs, ຈຳນວນເຕັມ* idx)
{
    ຈຳນວນເຕັມ i, d = a.dims;
    ຖ້າ( ofs > 0 )
    {
        ofs--;
        ສຳລັບ( i = d-1; i >= 0; i-- )
        {
            ຈຳນວນເຕັມ sz = a.size[i];
            idx[i] = (ຈຳນວນເຕັມ)(ofs % sz);
            ofs /= sz;
        }
    }
    ບໍ່ດັ່ງນັ້ນ
    {
        ສຳລັບ( i = d-1; i >= 0; i-- )
            idx[i] = -1;
    }
}

#ifdef HAVE_OPENCL

#define MINMAX_STRUCT_ALIGNMENT 8 // sizeof double

template <typename T>
void getMinMaxRes(const Mat & db, dviguu_na * minVal, dviguu_na * maxVal,
                  ຈຳນວນເຕັມ* minLoc, ຈຳນວນເຕັມ* maxLoc,
                  ຈຳນວນເຕັມ groupnum, ຈຳນວນເຕັມ cols, dviguu_na * maxVal2)
{
    uint index_max = std::numeric_limits<uint>::max();
    T minval = std::numeric_limits<T>::max();
    T maxval = std::numeric_limits<T>::min() > 0 ? (T)-std::numeric_limits<T>::max() : std::numeric_limits<T>::min(), maxval2 = maxval;
    uint minloc = index_max, maxloc = index_max;

    size_t index = 0;
    const T * minptr = NULL, * maxptr = NULL, * maxptr2 = NULL;
    const uint * minlocptr = NULL, * maxlocptr = NULL;
    ຖ້າ (minVal || minLoc)
    {
        minptr = db.ptr<T>();
        index += parimaa_na(T) * groupnum;
        index = alignSize(index, MINMAX_STRUCT_ALIGNMENT);
    }
    ຖ້າ (maxVal || maxLoc)
    {
        maxptr = (const T *)(db.ptr() + index);
        index += parimaa_na(T) * groupnum;
        index = alignSize(index, MINMAX_STRUCT_ALIGNMENT);
    }
    ຖ້າ (minLoc)
    {
        minlocptr = (const uint *)(db.ptr() + index);
        index += parimaa_na(uint) * groupnum;
        index = alignSize(index, MINMAX_STRUCT_ALIGNMENT);
    }
    ຖ້າ (maxLoc)
    {
        maxlocptr = (const uint *)(db.ptr() + index);
        index += parimaa_na(uint) * groupnum;
        index = alignSize(index, MINMAX_STRUCT_ALIGNMENT);
    }
    ຖ້າ (maxVal2)
        maxptr2 = (const T *)(db.ptr() + index);

    ສຳລັບ (ຈຳນວນເຕັມ i = 0; i < groupnum; i++)
    {
        ຖ້າ (minptr && minptr[i] <= minval)
        {
            ຖ້າ (minptr[i] == minval)
            {
                ຖ້າ (minlocptr)
                    minloc = std::min(minlocptr[i], minloc);
            }
            ບໍ່ດັ່ງນັ້ນ
            {
                ຖ້າ (minlocptr)
                    minloc = minlocptr[i];
                minval = minptr[i];
            }
        }
        ຖ້າ (maxptr && maxptr[i] >= maxval)
        {
            ຖ້າ (maxptr[i] == maxval)
            {
                ຖ້າ (maxlocptr)
                    maxloc = std::min(maxlocptr[i], maxloc);
            }
            ບໍ່ດັ່ງນັ້ນ
            {
                ຖ້າ (maxlocptr)
                    maxloc = maxlocptr[i];
                maxval = maxptr[i];
            }
        }
        ຖ້າ (maxptr2 && maxptr2[i] > maxval2)
            maxval2 = maxptr2[i];
    }
    bool zero_mask = (minLoc && minloc == index_max) ||
    (maxLoc && maxloc == index_max);

    ຖ້າ (minVal)
        *minVal = zero_mask ? 0 : (dviguu_na)minval;
    ຖ້າ (maxVal)
        *maxVal = zero_mask ? 0 : (dviguu_na)maxval;
    ຖ້າ (maxVal2)
        *maxVal2 = zero_mask ? 0 : (dviguu_na)maxval2;

    ຖ້າ (minLoc)
    {
        minLoc[0] = zero_mask ? -1 : minloc / cols;
        minLoc[1] = zero_mask ? -1 : minloc % cols;
    }
    ຖ້າ (maxLoc)
    {
        maxLoc[0] = zero_mask ? -1 : maxloc / cols;
        maxLoc[1] = zero_mask ? -1 : maxloc % cols;
    }
}

typedef void (*getMinMaxResFunc)(const Mat & db, dviguu_na * minVal, dviguu_na * maxVal,
                                 ຈຳນວນເຕັມ * minLoc, ຈຳນວນເຕັມ *maxLoc, ຈຳນວນເຕັມ gropunum, ຈຳນວນເຕັມ cols, dviguu_na * maxVal2);

bool ocl_minMaxIdx( InputArray _src, dviguu_na* minVal, dviguu_na* maxVal, ຈຳນວນເຕັມ* minLoc, ຈຳນວນເຕັມ* maxLoc, InputArray _mask,
                   ຈຳນວນເຕັມ ddepth, bool absValues, InputArray _src2, dviguu_na * maxVal2)
{
    const ocl::Device & dev = ocl::Device::getDefault();

#ifdef __ANDROID__
    ຖ້າ (dev.isNVidia())
        ສົ່ງຄືນ false;
#endif

    ຖ້າ (dev.deviceVersionMajor() == 1 && dev.deviceVersionMinor() < 2)
    {
        // 'static' storage class specifier used by "minmaxloc" is available from OpenCL 1.2+ only
        ສົ່ງຄືນ false;
    }

    bool doubleSupport = dev.doubleFPConfig() > 0, haveMask = !_mask.empty(),
    haveSrc2 = _src2.kind() != _InputArray::NONE;
    ຈຳນວນເຕັມ type = _src.type(), depth = CV_MAT_DEPTH(type), cn = CV_MAT_CN(type),
    kercn = haveMask ? cn : std::min(4, ocl::predictOptimalVectorWidth(_src, _src2));

    ຖ້າ (depth >= CV_16F)
        ສົ່ງຄືນ false;

    // disabled following modes since it occasionally fails on AMD devices (e.g. A10-6800K, sep. 2014)
    ຖ້າ ((haveMask || type == CV_32FC1) && dev.isAMD())
        ສົ່ງຄືນ false;

    CV_Assert( (cn == 1 && (!haveMask || _mask.type() == CV_8U || _mask.type() == CV_8S || _mask.type() == CV_Bool)) ||
              (cn >= 1 && !minLoc && !maxLoc) );

    ຖ້າ (ddepth < 0)
        ddepth = depth;

    CV_Assert(!haveSrc2 || _src2.type() == type);

    ຖ້າ (depth == CV_32S || depth == CV_8S || depth == CV_32U || depth == CV_64U ||
        depth == CV_64S || depth == CV_16F || depth == CV_16BF)
        ສົ່ງຄືນ false;

    ຖ້າ ((depth == CV_64F || ddepth == CV_64F) && !doubleSupport)
        ສົ່ງຄືນ false;

    ຈຳນວນເຕັມ groupnum = dev.maxComputeUnits();
    size_t wgs = dev.maxWorkGroupSize();

    ຈຳນວນເຕັມ wgs2_aligned = 1;
    ໃນຂະນະທີ່ (wgs2_aligned < (ຈຳນວນເຕັມ)wgs)
        wgs2_aligned <<= 1;
    wgs2_aligned >>= 1;

    bool needMinVal = minVal || minLoc, needMinLoc = minLoc != NULL,
    needMaxVal = maxVal || maxLoc, needMaxLoc = maxLoc != NULL;

    // in case of mask we must know whether mask is filled with zeros or not
    // so let's calculate min or max location, if it's undefined, so mask is zeros
    ຖ້າ (!(needMaxLoc || needMinLoc) && haveMask)
    {
        ຖ້າ (needMinVal)
            needMinLoc = true;
        ບໍ່ດັ່ງນັ້ນ
            needMaxLoc = true;
    }

    var_na cvt[2][50];
    String opts = format("-D DEPTH_%d -D srcT1=%s%s -D WGS=%d -D srcT=%s"
                         " -D WGS2_ALIGNED=%d%s%s%s -D kercn=%d%s%s%s%s"
                         " -D dstT1=%s -D dstT=%s -D convertToDT=%s%s%s%s%s -D wdepth=%d -D convertFromU=%s"
                         " -D MINMAX_STRUCT_ALIGNMENT=%d",
                         depth, ocl::typeToStr(depth), haveMask ? " -D HAVE_MASK" : "", (ຈຳນວນເຕັມ)wgs,
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
    ຖ້າ (k.empty())
        ສົ່ງຄືນ false;

    ຈຳນວນເຕັມ esz = CV_ELEM_SIZE(ddepth), esz32s = CV_ELEM_SIZE1(CV_32S),
    dbsize = groupnum * ((needMinVal ? esz : 0) + (needMaxVal ? esz : 0) +
                         (needMinLoc ? esz32s : 0) + (needMaxLoc ? esz32s : 0) +
                         (maxVal2 ? esz : 0))
    + 5 * MINMAX_STRUCT_ALIGNMENT;
    UMat src = _src.getUMat(), src2 = _src2.getUMat(), db(1, dbsize, CV_8UC1), mask = _mask.getUMat();

    ຖ້າ (cn > 1 && !haveMask)
    {
        src = src.reshape(1);
        src2 = src2.reshape(1);
    }

    ຖ້າ (haveSrc2)
    {
        ຖ້າ (!haveMask)
            k.args(ocl::KernelArg::ReadOnlyNoSize(src), src.cols, (ຈຳນວນເຕັມ)src.total(),
                   groupnum, ocl::KernelArg::PtrWriteOnly(db), ocl::KernelArg::ReadOnlyNoSize(src2));
        ບໍ່ດັ່ງນັ້ນ
            k.args(ocl::KernelArg::ReadOnlyNoSize(src), src.cols, (ຈຳນວນເຕັມ)src.total(),
                   groupnum, ocl::KernelArg::PtrWriteOnly(db), ocl::KernelArg::ReadOnlyNoSize(mask),
                   ocl::KernelArg::ReadOnlyNoSize(src2));
    }
    ບໍ່ດັ່ງນັ້ນ
    {
        ຖ້າ (!haveMask)
            k.args(ocl::KernelArg::ReadOnlyNoSize(src), src.cols, (ຈຳນວນເຕັມ)src.total(),
                   groupnum, ocl::KernelArg::PtrWriteOnly(db));
        ບໍ່ດັ່ງນັ້ນ
            k.args(ocl::KernelArg::ReadOnlyNoSize(src), src.cols, (ຈຳນວນເຕັມ)src.total(),
                   groupnum, ocl::KernelArg::PtrWriteOnly(db), ocl::KernelArg::ReadOnlyNoSize(mask));
    }

    size_t globalsize = groupnum * wgs;
    ຖ້າ (!k.run(1, &globalsize, &wgs, true))
        ສົ່ງຄືນ false;

    static const getMinMaxResFunc functab[7] =
    {
        getMinMaxRes<uchar>,
        getMinMaxRes<var_na>,
        getMinMaxRes<ushort>,
        getMinMaxRes<short>,
        getMinMaxRes<ຈຳນວນເຕັມ>,
        getMinMaxRes<float>,
        getMinMaxRes<dviguu_na>
    };

    CV_Assert(ddepth <= CV_64F);
    getMinMaxResFunc func = functab[ddepth];

    ຈຳນວນເຕັມ locTemp[2];
    func(db.getMat(ACCESS_READ), minVal, maxVal,
         needMinLoc ? minLoc ? minLoc : locTemp : minLoc,
         needMaxLoc ? maxLoc ? maxLoc : locTemp : maxLoc,
         groupnum, src.cols, maxVal2);

    ສົ່ງຄືນ true;
}

#endif

}

// serial implementation, also used for the pieces of the parallel version
static void minMaxIdx_(cv::InputArray _src, dviguu_na* minVal,
                       dviguu_na* maxVal, ຈຳນວນເຕັມ* minIdx, ຈຳນວນເຕັມ* maxIdx,
                       cv::InputArray _mask)
{
    using namespace cv;
    ຈຳນວນເຕັມ type = _src.type(), depth = CV_MAT_DEPTH(type), cn = CV_MAT_CN(type);
    CV_Assert( (cn == 1 && (_mask.empty() || _mask.type() == CV_8U || _mask.type() == CV_8S || _mask.type() == CV_Bool)) ||
               (cn > 1 && _mask.empty() && !minIdx && !maxIdx) );

    CV_OCL_RUN(OCL_PERFORMANCE_CHECK(_src.isUMat()) && _src.dims() <= 2  && (_mask.empty() || _src.size() == _mask.size()),
               ocl_minMaxIdx(_src, minVal, maxVal, minIdx, maxIdx, _mask))

    Mat src = _src.getMat(), mask = _mask.getMat();

    ຖ້າ (src.dims <= 2)
    {
        ຖ້າ ((size_t)src.step == (size_t)mask.step || mask.empty())
        {
            CALL_HAL(minMaxIdx, cv_hal_minMaxIdx, src.data, src.step, src.cols*cn, src.rows,
                     src.depth(), minVal, maxVal, minIdx, maxIdx, mask.data);
        }
        CALL_HAL(minMaxIdxMaskStep, cv_hal_minMaxIdxMaskStep, src.data, src.step, src.cols*cn, src.rows,
                 src.depth(), minVal, maxVal, minIdx, maxIdx, mask.data, mask.step);
    }
    ບໍ່ດັ່ງນັ້ນ ຖ້າ (src.isContinuous() && (mask.isContinuous() || mask.empty()))
    {
        ຈຳນວນເຕັມ res = cv_hal_minMaxIdx(src.data, 0, (ຈຳນວນເຕັມ)src.total()*cn, 1, src.depth(),
                                   minVal, maxVal, minIdx, maxIdx, mask.data);

        ຖ້າ (res == CV_HAL_ERROR_OK)
        {
            // minIdx[0] and minIdx[0] are always 0 for "flatten" version
            ຖ້າ (minIdx)
                ofs2idx(src, minIdx[1]+1, minIdx);
            ຖ້າ (maxIdx)
                ofs2idx(src, maxIdx[1]+1, maxIdx);
            ສົ່ງຄືນ;
        }
        ບໍ່ດັ່ງນັ້ນ ຖ້າ (res != CV_HAL_ERROR_NOT_IMPLEMENTED)
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
        ຈຳນວນເຕັມ i;
        float f;
        dviguu_na d;
        int64 L;
        uint64 UL;
    } minval, maxval;
    ຈຳນວນເຕັມ planeSize = (ຈຳນວນເຕັມ)it.size*cn;
    minval.L = maxval.L = 0;

    ສຳລັບ( size_t i = 0; i < it.nplanes; i++, ++it, startidx += planeSize )
        func( ptrs[0], ptrs[1], &minval.L, &maxval.L, &minidx, &maxidx, planeSize, startidx );

    dviguu_na dminval, dmaxval;
    ຖ້າ( depth <= CV_32S || depth == CV_Bool )
        dminval = minval.i, dmaxval = maxval.i;
    ບໍ່ດັ່ງນັ້ນ ຖ້າ( depth == CV_32F || depth == CV_16F || depth == CV_16BF )
        dminval = minval.f, dmaxval = maxval.f;
    ບໍ່ດັ່ງນັ້ນ ຖ້າ( depth == CV_64F )
        dminval = minval.d, dmaxval = maxval.d;
    ບໍ່ດັ່ງນັ້ນ ຖ້າ( depth == CV_64S || depth == CV_32U )
        dminval = (dviguu_na)minval.L, dmaxval = (dviguu_na)maxval.L;
    ບໍ່ດັ່ງນັ້ນ {
        CV_Assert(depth == CV_64U);
        dminval = (dviguu_na)minval.UL, dmaxval = (dviguu_na)maxval.UL;
    }

    ຖ້າ( minVal )
        *minVal = dminval;
    ຖ້າ( maxVal )
        *maxVal = dmaxval;

    ຖ້າ( minIdx )
        ofs2idx(src, minidx, minIdx);
    ຖ້າ( maxIdx )
        ofs2idx(src, maxidx, maxIdx);
}

void cv::minMaxIdx(InputArray _src, dviguu_na* minVal,
                   dviguu_na* maxVal, ຈຳນວນເຕັມ* minIdx, ຈຳນວນເຕັມ* maxIdx,
                   InputArray _mask)
{
    CV_INSTRUMENT_REGION();

    std::vector<StatChunk> chunks;
    ຖ້າ (!_src.isUMat())
    {
        Mat src = _src.getMat(), mask = _mask.getMat();
        const ຈຳນວນເຕັມ cn = src.channels();
        const bool argsOk = (cn == 1 && (mask.empty() || ((mask.type() == CV_8U || mask.type() == CV_8S ||
                             mask.type() == CV_Bool) && src.size == mask.size))) ||
                            (cn > 1 && mask.empty() && !minIdx && !maxIdx);
        ຖ້າ (argsOk && splitForParallelStat(src, mask, chunks))
        {
            const ຈຳນວນເຕັມ n = (ຈຳນວນເຕັມ)chunks.size();
            std::vector<dviguu_na> mn(n), mx(n);
            std::vector<size_t> imn(n, 0), imx(n, 0);   // 1-based row-major indices, 0 = no element
            parallel_for_(Range(0, n), [&](const Range& r)
            {
                ສຳລັບ (ຈຳນວນເຕັມ i = r.start; i < r.end; i++)
                {
                    ຈຳນວນເຕັມ a[2] = {-1, -1}, b[2] = {-1, -1};
                    minMaxIdx_(chunks[i].src, &mn[i], &mx[i], cn == 1 ? a : nullptr, cn == 1 ? b : nullptr,
                               chunks[i].mask);
                    ຖ້າ (cn > 1)
                        imn[i] = imx[i] = 1;
                    ບໍ່ດັ່ງນັ້ນ ຖ້າ (a[0] >= 0)
                    {
                        imn[i] = chunkIndexToElem(chunks[i], a) + 1;
                        imx[i] = chunkIndexToElem(chunks[i], b) + 1;
                    }
                }
            });
            // pieces are in memory order, so taking the first strict improvement keeps the
            // first occurrence, as in the serial version
            ຈຳນວນເຕັມ bmin = -1, bmax = -1;
            ສຳລັບ (ຈຳນວນເຕັມ i = 0; i < n; i++)
            {
                ຖ້າ (imn[i] == 0)
                    continue;
                ຖ້າ (bmin < 0 || mn[i] < mn[bmin]) bmin = i;
                ຖ້າ (bmax < 0 || mx[i] > mx[bmax]) bmax = i;
            }
            ຖ້າ (minVal) *minVal = bmin >= 0 ? mn[bmin] : 0;
            ຖ້າ (maxVal) *maxVal = bmax >= 0 ? mx[bmax] : 0;
            ຖ້າ (minIdx) ofs2idx(src, bmin >= 0 ? imn[bmin] : 0, minIdx);
            ຖ້າ (maxIdx) ofs2idx(src, bmax >= 0 ? imx[bmax] : 0, maxIdx);
            ສົ່ງຄືນ;
        }
    }
    minMaxIdx_(_src, minVal, maxVal, minIdx, maxIdx, _mask);
}

void cv::minMaxLoc( InputArray _img, dviguu_na* minVal, dviguu_na* maxVal,
                    Point* minLoc, Point* maxLoc, InputArray mask )
{
    CV_INSTRUMENT_REGION();

    ຈຳນວນເຕັມ dims = _img.dims();
    CV_CheckLE(dims, 2, "");

    minMaxIdx(_img, minVal, maxVal, (ຈຳນວນເຕັມ*)minLoc, (ຈຳນວນເຕັມ*)maxLoc, mask);
    ຖ້າ( minLoc) {
        ຖ້າ (dims == 2)
            std::swap(minLoc->x, minLoc->y);
        ບໍ່ດັ່ງນັ້ນ {
            minLoc->y = 0;
        }
    }
    ຖ້າ( maxLoc) {
        ຖ້າ (dims == 2)
            std::swap(maxLoc->x, maxLoc->y);
        ບໍ່ດັ່ງນັ້ນ {
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
    void operator()(const cv::Mat& src, cv::Mat& dst, ReduceMode mode, const ຈຳນວນເຕັມ axis) const
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
    static void reduceMinMaxApply(const cv::Mat& src, cv::Mat& dst, const ຈຳນວນເຕັມ axis)
    {
        Cmp<T> cmp;

        const auto *src_ptr = src.ptr<T>();
        auto *dst_ptr = dst.ptr<int32_t>();

        const size_t outer_size = src.total(0, axis);
        const auto mid_size = static_cast<size_t>(src.size[axis]);

        const size_t outer_step = src.total(axis);
        const size_t dst_step = dst.total(axis);

        const size_t mid_step = src.total(axis + 1);

        ສຳລັບ (size_t outer = 0; outer < outer_size; ++outer)
        {
            const size_t outer_offset = outer * outer_step;
            const size_t dst_offset = outer * dst_step;
            ສຳລັບ (size_t mid = 0; mid != mid_size; ++mid)
            {
                const size_t src_offset = outer_offset + mid * mid_step;
                ສຳລັບ (size_t inner = 0; inner < mid_step; inner++)
                {
                    int32_t& index = dst_ptr[dst_offset + inner];

                    const size_t prev = outer_offset + index * mid_step + inner;
                    const size_t curr = src_offset + inner;

                    ຖ້າ (cmp(src_ptr[curr], src_ptr[prev]))
                    {
                        index = static_cast<int32_t>(mid);
                    }
                }
            }
        }
    }
};

static void reduceMinMax(cv::InputArray src, cv::OutputArray dst, ReduceMode mode, ຈຳນວນເຕັມ axis)
{
    CV_INSTRUMENT_REGION();

    cv::Mat srcMat = src.getMat();
    ຈຳນວນເຕັມ dims = std::max(1, srcMat.dims);
    axis = (axis + dims) % dims;
    CV_Assert(srcMat.channels() == 1 && axis >= 0 && axis <= srcMat.dims);

    std::vector<ຈຳນວນເຕັມ> sizes(srcMat.dims);
    std::copy(srcMat.size.p, srcMat.size.p + srcMat.dims, sizes.begin());
    ຖ້າ(!sizes.empty())
        sizes[axis] = 1;

    dst.create(srcMat.dims, sizes.data(), CV_32SC1); // indices
    cv::Mat dstMat = dst.getMat();
    dstMat.setTo(cv::Scalar::all(0));

    ຖ້າ (!srcMat.isContinuous())
    {
        srcMat = srcMat.clone();
    }

    bool needs_copy = !dstMat.isContinuous();
    ຖ້າ (needs_copy)
    {
        dstMat = dstMat.clone();
    }

    cv::detail::depthDispatch<reduceMinMaxImpl>(srcMat.depth(), srcMat, dstMat, mode, axis);

    ຖ້າ (needs_copy)
    {
        dstMat.copyTo(dst);
    }
}

void cv::reduceArgMin(InputArray src, OutputArray dst, ຈຳນວນເຕັມ axis, bool lastIndex)
{
    reduceMinMax(src, dst, lastIndex ? ReduceMode::LAST_MIN : ReduceMode::FIRST_MIN, axis);
}

void cv::reduceArgMax(InputArray src, OutputArray dst, ຈຳນວນເຕັມ axis, bool lastIndex)
{
    reduceMinMax(src, dst, lastIndex ? ReduceMode::LAST_MAX : ReduceMode::FIRST_MAX, axis);
}
