// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


#ಅನ್ತರ್ಭಾವ "precomp.hpp"
#ಅನ್ತರ್ಭಾವ "opencl_kernels_core.hpp"
#ಅನ್ತರ್ಭಾವ "stat.hpp"
#ಅನ್ತರ್ಭಾವ "stat_parallel.hpp"
#ಅನ್ತರ್ಭಾವ "opencv2/core/detail/dispatch_helper.impl.hpp"
#ಅನ್ತರ್ಭಾವ <algorithm>

#ಅನ್ತರ್ಭಾವ "minmax.simd.hpp"
#ಅನ್ತರ್ಭಾವ "minmax.simd_declarations.hpp" // defines CV_CPU_DISPATCH_MODES_ALL=AVX2,...,BASELINE based on CMakeLists.txt content

ನಾಮಾಕಾಶ cv {

ಸ್ಥಿರ MinMaxIdxFunc getMinMaxIdxFunc(ಪೂರ್ಣಾಙ್ಕ depth)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getMinMaxIdxFunc, (depth),
                    CV_CPU_DISPATCH_MODES_ALL);
}

// The function expects 1-based indexing for ofs
// Zero is treated as invalid offset (not found)
ಸ್ಥಿರ ಶೂನ್ಯ ofs2idx(ಧ್ರುವ Mat& a, size_t ofs, ಪೂರ್ಣಾಙ್ಕ* idx)
{
    ಪೂರ್ಣಾಙ್ಕ i, d = a.dims;
    ಯದಿ( ofs > 0 )
    {
        ofs--;
        ಪ್ರತಿ( i = d-1; i >= 0; i-- )
        {
            ಪೂರ್ಣಾಙ್ಕ sz = a.size[i];
            idx[i] = (ಪೂರ್ಣಾಙ್ಕ)(ofs % sz);
            ofs /= sz;
        }
    }
    ಅನ್ಯಥಾ
    {
        ಪ್ರತಿ( i = d-1; i >= 0; i-- )
            idx[i] = -1;
    }
}

#ifdef HAVE_OPENCL

#define MINMAX_STRUCT_ALIGNMENT 8 // sizeof double

template <typename T>
ಶೂನ್ಯ getMinMaxRes(ಧ್ರುವ Mat & db, ದ್ವಿಗುಣ * minVal, ದ್ವಿಗುಣ * maxVal,
                  ಪೂರ್ಣಾಙ್ಕ* minLoc, ಪೂರ್ಣಾಙ್ಕ* maxLoc,
                  ಪೂರ್ಣಾಙ್ಕ groupnum, ಪೂರ್ಣಾಙ್ಕ cols, ದ್ವಿಗುಣ * maxVal2)
{
    uint index_max = std::numeric_limits<uint>::max();
    T minval = std::numeric_limits<T>::max();
    T maxval = std::numeric_limits<T>::min() > 0 ? (T)-std::numeric_limits<T>::max() : std::numeric_limits<T>::min(), maxval2 = maxval;
    uint minloc = index_max, maxloc = index_max;

    size_t index = 0;
    ಧ್ರುವ T * minptr = ರಿಕ್ತ, * maxptr = ರಿಕ್ತ, * maxptr2 = ರಿಕ್ತ;
    ಧ್ರುವ uint * minlocptr = ರಿಕ್ತ, * maxlocptr = ರಿಕ್ತ;
    ಯದಿ (minVal || minLoc)
    {
        minptr = db.ptr<T>();
        index += ಪರಿಮಾಣ(T) * groupnum;
        index = alignSize(index, MINMAX_STRUCT_ALIGNMENT);
    }
    ಯದಿ (maxVal || maxLoc)
    {
        maxptr = (ಧ್ರುವ T *)(db.ptr() + index);
        index += ಪರಿಮಾಣ(T) * groupnum;
        index = alignSize(index, MINMAX_STRUCT_ALIGNMENT);
    }
    ಯದಿ (minLoc)
    {
        minlocptr = (ಧ್ರುವ uint *)(db.ptr() + index);
        index += ಪರಿಮಾಣ(uint) * groupnum;
        index = alignSize(index, MINMAX_STRUCT_ALIGNMENT);
    }
    ಯದಿ (maxLoc)
    {
        maxlocptr = (ಧ್ರುವ uint *)(db.ptr() + index);
        index += ಪರಿಮಾಣ(uint) * groupnum;
        index = alignSize(index, MINMAX_STRUCT_ALIGNMENT);
    }
    ಯದಿ (maxVal2)
        maxptr2 = (ಧ್ರುವ T *)(db.ptr() + index);

    ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < groupnum; i++)
    {
        ಯದಿ (minptr && minptr[i] <= minval)
        {
            ಯದಿ (minptr[i] == minval)
            {
                ಯದಿ (minlocptr)
                    minloc = std::min(minlocptr[i], minloc);
            }
            ಅನ್ಯಥಾ
            {
                ಯದಿ (minlocptr)
                    minloc = minlocptr[i];
                minval = minptr[i];
            }
        }
        ಯದಿ (maxptr && maxptr[i] >= maxval)
        {
            ಯದಿ (maxptr[i] == maxval)
            {
                ಯದಿ (maxlocptr)
                    maxloc = std::min(maxlocptr[i], maxloc);
            }
            ಅನ್ಯಥಾ
            {
                ಯದಿ (maxlocptr)
                    maxloc = maxlocptr[i];
                maxval = maxptr[i];
            }
        }
        ಯದಿ (maxptr2 && maxptr2[i] > maxval2)
            maxval2 = maxptr2[i];
    }
    ತಾರ್ಕಿಕ zero_mask = (minLoc && minloc == index_max) ||
    (maxLoc && maxloc == index_max);

    ಯದಿ (minVal)
        *minVal = zero_mask ? 0 : (ದ್ವಿಗುಣ)minval;
    ಯದಿ (maxVal)
        *maxVal = zero_mask ? 0 : (ದ್ವಿಗುಣ)maxval;
    ಯದಿ (maxVal2)
        *maxVal2 = zero_mask ? 0 : (ದ್ವಿಗುಣ)maxval2;

    ಯದಿ (minLoc)
    {
        minLoc[0] = zero_mask ? -1 : minloc / cols;
        minLoc[1] = zero_mask ? -1 : minloc % cols;
    }
    ಯದಿ (maxLoc)
    {
        maxLoc[0] = zero_mask ? -1 : maxloc / cols;
        maxLoc[1] = zero_mask ? -1 : maxloc % cols;
    }
}

ಪ್ರಕಾರನಿರ್ದೇಶ ಶೂನ್ಯ (*getMinMaxResFunc)(ಧ್ರುವ Mat & db, ದ್ವಿಗುಣ * minVal, ದ್ವಿಗುಣ * maxVal,
                                 ಪೂರ್ಣಾಙ್ಕ * minLoc, ಪೂರ್ಣಾಙ್ಕ *maxLoc, ಪೂರ್ಣಾಙ್ಕ gropunum, ಪೂರ್ಣಾಙ್ಕ cols, ದ್ವಿಗುಣ * maxVal2);

ತಾರ್ಕಿಕ ocl_minMaxIdx( ಒಳಹರಿವು_Array _src, ದ್ವಿಗುಣ* minVal, ದ್ವಿಗುಣ* maxVal, ಪೂರ್ಣಾಙ್ಕ* minLoc, ಪೂರ್ಣಾಙ್ಕ* maxLoc, ಒಳಹರಿವು_Array _mask,
                   ಪೂರ್ಣಾಙ್ಕ ddepth, ತಾರ್ಕಿಕ abs_ಮೌಲ್ಯಗಳು, ಒಳಹರಿವು_Array _src2, ದ್ವಿಗುಣ * maxVal2)
{
    ಧ್ರುವ ocl::Device & dev = ocl::Device::getDefault();

#ifdef __ANDROID__
    ಯದಿ (dev.isNVidia())
        ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
#endif

    ಯದಿ (dev.deviceVersionMajor() == 1 && dev.deviceVersionMinor() < 2)
    {
        // 'static' storage class specifier used by "minmaxloc" is available from OpenCL 1.2+ only
        ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
    }

    ತಾರ್ಕಿಕ doubleSupport = dev.doubleFPConfig() > 0, haveMask = !_mask.empty(),
    haveSrc2 = _src2.kind() != ಒಳಹರಿವು_Array_2::NONE;
    ಪೂರ್ಣಾಙ್ಕ type = _src.type(), depth = CV_MAT_DEPTH(type), cn = CV_MAT_CN(type),
    kercn = haveMask ? cn : std::min(4, ocl::predictOptimalVectorWidth(_src, _src2));

    ಯದಿ (depth >= CV_16F)
        ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

    // disabled following modes since it occasionally fails on AMD devices (e.g. A10-6800K, sep. 2014)
    ಯದಿ ((haveMask || type == CV_32FC1) && dev.isAMD())
        ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

    CV_Assert( (cn == 1 && (!haveMask || _mask.type() == CV_8U || _mask.type() == CV_8S || _mask.type() == CV_Bool)) ||
              (cn >= 1 && !minLoc && !maxLoc) );

    ಯದಿ (ddepth < 0)
        ddepth = depth;

    CV_Assert(!haveSrc2 || _src2.type() == type);

    ಯದಿ (depth == CV_32S || depth == CV_8S || depth == CV_32U || depth == CV_64U ||
        depth == CV_64S || depth == CV_16F || depth == CV_16BF)
        ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

    ಯದಿ ((depth == CV_64F || ddepth == CV_64F) && !doubleSupport)
        ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

    ಪೂರ್ಣಾಙ್ಕ groupnum = dev.maxComputeUnits();
    size_t wgs = dev.maxWorkGroupSize();

    ಪೂರ್ಣಾಙ್ಕ wgs2_aligned = 1;
    ಯಾವತ್ (wgs2_aligned < (ಪೂರ್ಣಾಙ್ಕ)wgs)
        wgs2_aligned <<= 1;
    wgs2_aligned >>= 1;

    ತಾರ್ಕಿಕ needMinVal = minVal || minLoc, needMinLoc = minLoc != ರಿಕ್ತ,
    needMaxVal = maxVal || maxLoc, needMaxLoc = maxLoc != ರಿಕ್ತ;

    // in case of mask we must know whether mask is filled with zeros or not
    // so let's calculate min or max location, if it's undefined, so mask is zeros
    ಯದಿ (!(needMaxLoc || needMinLoc) && haveMask)
    {
        ಯದಿ (needMinVal)
            needMinLoc = ಸತ್ಯ;
        ಅನ್ಯಥಾ
            needMaxLoc = ಸತ್ಯ;
    }

    ವರ್ಣ cvt[2][50];
    String opts = format("-D DEPTH_%d -D srcT1=%s%s -D WGS=%d -D srcT=%s"
                         " -D WGS2_ALIGNED=%d%s%s%s -D kercn=%d%s%s%s%s"
                         " -D dstT1=%s -D dstT=%s -D convertToDT=%s%s%s%s%s -D wdepth=%d -D convertFromU=%s"
                         " -D MINMAX_STRUCT_ALIGNMENT=%d",
                         depth, ocl::typeToStr(depth), haveMask ? " -D HAVE_MASK" : "", (ಪೂರ್ಣಾಙ್ಕ)wgs,
                         ocl::typeToStr(CV_MAKE_TYPE(depth, kercn)), wgs2_aligned,
                         doubleSupport ? " -D DOUBLE_SUPPORT" : "",
                         _src.isContinuous() ? " -D HAVE_SRC_CONT" : "",
                         _mask.isContinuous() ? " -D HAVE_MASK_CONT" : "", kercn,
                         needMinVal ? " -D NEED_MINVAL" : "", needMaxVal ? " -D NEED_MAXVAL" : "",
                         needMinLoc ? " -D NEED_MINLOC" : "", needMaxLoc ? " -D NEED_MAXLOC" : "",
                         ocl::typeToStr(ddepth), ocl::typeToStr(CV_MAKE_TYPE(ddepth, kercn)),
                         ocl::convertTypeStr(depth, ddepth, kercn, cvt[0], ಪರಿಮಾಣ(cvt[0])),
                         abs_ಮೌಲ್ಯಗಳು ? " -D OP_ABS" : "",
                         haveSrc2 ? " -D HAVE_SRC2" : "", maxVal2 ? " -D OP_CALC2" : "",
                         haveSrc2 && _src2.isContinuous() ? " -D HAVE_SRC2_CONT" : "", ddepth,
                         depth <= CV_32S && ddepth == CV_32S ? ocl::convertTypeStr(CV_8U, ddepth, kercn, cvt[1], ಪರಿಮಾಣ(cvt[1])) : "noconvert",
                         MINMAX_STRUCT_ALIGNMENT);

    ocl::Kernel k("minmaxloc", ocl::core::minmaxloc_oclsrc, opts);
    ಯದಿ (k.empty())
        ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

    ಪೂರ್ಣಾಙ್ಕ esz = CV_ELEM_SIZE(ddepth), esz32s = CV_ELEM_SIZE1(CV_32S),
    dbsize = groupnum * ((needMinVal ? esz : 0) + (needMaxVal ? esz : 0) +
                         (needMinLoc ? esz32s : 0) + (needMaxLoc ? esz32s : 0) +
                         (maxVal2 ? esz : 0))
    + 5 * MINMAX_STRUCT_ALIGNMENT;
    UMat src = _src.getUMat(), src2 = _src2.getUMat(), db(1, dbsize, CV_8UC1), mask = _mask.getUMat();

    ಯದಿ (cn > 1 && !haveMask)
    {
        src = src.reshape(1);
        src2 = src2.reshape(1);
    }

    ಯದಿ (haveSrc2)
    {
        ಯದಿ (!haveMask)
            k.args(ocl::KernelArg::ReadOnlyNoSize(src), src.cols, (ಪೂರ್ಣಾಙ್ಕ)src.total(),
                   groupnum, ocl::KernelArg::PtrWriteOnly(db), ocl::KernelArg::ReadOnlyNoSize(src2));
        ಅನ್ಯಥಾ
            k.args(ocl::KernelArg::ReadOnlyNoSize(src), src.cols, (ಪೂರ್ಣಾಙ್ಕ)src.total(),
                   groupnum, ocl::KernelArg::PtrWriteOnly(db), ocl::KernelArg::ReadOnlyNoSize(mask),
                   ocl::KernelArg::ReadOnlyNoSize(src2));
    }
    ಅನ್ಯಥಾ
    {
        ಯದಿ (!haveMask)
            k.args(ocl::KernelArg::ReadOnlyNoSize(src), src.cols, (ಪೂರ್ಣಾಙ್ಕ)src.total(),
                   groupnum, ocl::KernelArg::PtrWriteOnly(db));
        ಅನ್ಯಥಾ
            k.args(ocl::KernelArg::ReadOnlyNoSize(src), src.cols, (ಪೂರ್ಣಾಙ್ಕ)src.total(),
                   groupnum, ocl::KernelArg::PtrWriteOnly(db), ocl::KernelArg::ReadOnlyNoSize(mask));
    }

    size_t globalsize = groupnum * wgs;
    ಯದಿ (!k.run(1, &globalsize, &wgs, ಸತ್ಯ))
        ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

    ಸ್ಥಿರ ಧ್ರುವ getMinMaxResFunc functab[7] =
    {
        getMinMaxRes<uchar>,
        getMinMaxRes<ವರ್ಣ>,
        getMinMaxRes<ushort>,
        getMinMaxRes<short>,
        getMinMaxRes<ಪೂರ್ಣಾಙ್ಕ>,
        getMinMaxRes<ಭಿನ್ನ>,
        getMinMaxRes<ದ್ವಿಗುಣ>
    };

    CV_Assert(ddepth <= CV_64F);
    getMinMaxResFunc func = functab[ddepth];

    ಪೂರ್ಣಾಙ್ಕ locTemp[2];
    func(db.getMat(ACCESS_READ), minVal, maxVal,
         needMinLoc ? minLoc ? minLoc : locTemp : minLoc,
         needMaxLoc ? maxLoc ? maxLoc : locTemp : maxLoc,
         groupnum, src.cols, maxVal2);

    ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
}

#endif

}

// serial implementation, also used for the pieces of the parallel version
ಸ್ಥಿರ ಶೂನ್ಯ minMaxIdx_(cv::ಒಳಹರಿವು_Array _src, ದ್ವಿಗುಣ* minVal,
                       ದ್ವಿಗುಣ* maxVal, ಪೂರ್ಣಾಙ್ಕ* minIdx, ಪೂರ್ಣಾಙ್ಕ* maxIdx,
                       cv::ಒಳಹರಿವು_Array _mask)
{
    using ನಾಮಾಕಾಶ cv;
    ಪೂರ್ಣಾಙ್ಕ type = _src.type(), depth = CV_MAT_DEPTH(type), cn = CV_MAT_CN(type);
    CV_Assert( (cn == 1 && (_mask.empty() || _mask.type() == CV_8U || _mask.type() == CV_8S || _mask.type() == CV_Bool)) ||
               (cn > 1 && _mask.empty() && !minIdx && !maxIdx) );

    CV_OCL_RUN(OCL_PERFORMANCE_CHECK(_src.isUMat()) && _src.dims() <= 2  && (_mask.empty() || _src.size() == _mask.size()),
               ocl_minMaxIdx(_src, minVal, maxVal, minIdx, maxIdx, _mask))

    Mat src = _src.getMat(), mask = _mask.getMat();

    ಯದಿ (src.dims <= 2)
    {
        ಯದಿ ((size_t)src.step == (size_t)mask.step || mask.empty())
        {
            CALL_HAL(minMaxIdx, cv_hal_minMaxIdx, src.data, src.step, src.cols*cn, src.rows,
                     src.depth(), minVal, maxVal, minIdx, maxIdx, mask.data);
        }
        CALL_HAL(minMaxIdxMaskStep, cv_hal_minMaxIdxMaskStep, src.data, src.step, src.cols*cn, src.rows,
                 src.depth(), minVal, maxVal, minIdx, maxIdx, mask.data, mask.step);
    }
    ಅನ್ಯಥಾ ಯದಿ (src.isContinuous() && (mask.isContinuous() || mask.empty()))
    {
        ಪೂರ್ಣಾಙ್ಕ res = cv_hal_minMaxIdx(src.data, 0, (ಪೂರ್ಣಾಙ್ಕ)src.total()*cn, 1, src.depth(),
                                   minVal, maxVal, minIdx, maxIdx, mask.data);

        ಯದಿ (res == CV_HAL_ERROR_OK)
        {
            // minIdx[0] and minIdx[0] are always 0 for "flatten" version
            ಯದಿ (minIdx)
                ofs2idx(src, minIdx[1]+1, minIdx);
            ಯದಿ (maxIdx)
                ofs2idx(src, maxIdx[1]+1, maxIdx);
            ಪ್ರತ್ಯಾಗಮ;
        }
        ಅನ್ಯಥಾ ಯದಿ (res != CV_HAL_ERROR_NOT_IMPLEMENTED)
        {
            CV_Error_(cv::Error::StsInternal,
            ("HAL implementation minMaxIdx ==> " CVAUX_STR(cv_hal_minMaxIdx) " returned %d (0x%08x)", res, res));
        }
    }

    MinMaxIdxFunc func = getMinMaxIdxFunc(depth);
    CV_Assert( func != 0 );

    ಧ್ರುವ Mat* arrays[] = {&src, &mask, 0};
    uchar* ptrs[2] = {};
    NAryMatIterator it(arrays, ptrs);

    size_t minidx = 0, maxidx = 0;
    size_t startidx = 1;
    ಸಙ್ಘ {
        ಪೂರ್ಣಾಙ್ಕ i;
        ಭಿನ್ನ f;
        ದ್ವಿಗುಣ d;
        int64 L;
        uint64 UL;
    } minval, maxval;
    ಪೂರ್ಣಾಙ್ಕ planeSize = (ಪೂರ್ಣಾಙ್ಕ)it.size*cn;
    minval.L = maxval.L = 0;

    ಪ್ರತಿ( size_t i = 0; i < it.nplanes; i++, ++it, startidx += planeSize )
        func( ptrs[0], ptrs[1], &minval.L, &maxval.L, &minidx, &maxidx, planeSize, startidx );

    ದ್ವಿಗುಣ dminval, dmaxval;
    ಯದಿ( depth <= CV_32S || depth == CV_Bool )
        dminval = minval.i, dmaxval = maxval.i;
    ಅನ್ಯಥಾ ಯದಿ( depth == CV_32F || depth == CV_16F || depth == CV_16BF )
        dminval = minval.f, dmaxval = maxval.f;
    ಅನ್ಯಥಾ ಯದಿ( depth == CV_64F )
        dminval = minval.d, dmaxval = maxval.d;
    ಅನ್ಯಥಾ ಯದಿ( depth == CV_64S || depth == CV_32U )
        dminval = (ದ್ವಿಗುಣ)minval.L, dmaxval = (ದ್ವಿಗುಣ)maxval.L;
    ಅನ್ಯಥಾ {
        CV_Assert(depth == CV_64U);
        dminval = (ದ್ವಿಗುಣ)minval.UL, dmaxval = (ದ್ವಿಗುಣ)maxval.UL;
    }

    ಯದಿ( minVal )
        *minVal = dminval;
    ಯದಿ( maxVal )
        *maxVal = dmaxval;

    ಯದಿ( minIdx )
        ofs2idx(src, minidx, minIdx);
    ಯದಿ( maxIdx )
        ofs2idx(src, maxidx, maxIdx);
}

ಶೂನ್ಯ cv::minMaxIdx(ಒಳಹರಿವು_Array _src, ದ್ವಿಗುಣ* minVal,
                   ದ್ವಿಗುಣ* maxVal, ಪೂರ್ಣಾಙ್ಕ* minIdx, ಪೂರ್ಣಾಙ್ಕ* maxIdx,
                   ಒಳಹರಿವು_Array _mask)
{
    CV_INSTRUMENT_REGION();

    std::vector<StatChunk> chunks;
    ಯದಿ (!_src.isUMat())
    {
        Mat src = _src.getMat(), mask = _mask.getMat();
        ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ cn = src.channels();
        ಧ್ರುವ ತಾರ್ಕಿಕ argsOk = (cn == 1 && (mask.empty() || ((mask.type() == CV_8U || mask.type() == CV_8S ||
                             mask.type() == CV_Bool) && src.size == mask.size))) ||
                            (cn > 1 && mask.empty() && !minIdx && !maxIdx);
        ಯದಿ (argsOk && splitForParallelStat(src, mask, chunks))
        {
            ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ n = (ಪೂರ್ಣಾಙ್ಕ)chunks.size();
            std::vector<ದ್ವಿಗುಣ> mn(n), mx(n);
            std::vector<size_t> imn(n, 0), imx(n, 0);   // 1-based row-major indices, 0 = no element
            parallel_for_(Range(0, n), [&](ಧ್ರುವ Range& r)
            {
                ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = r.start; i < r.end; i++)
                {
                    ಪೂರ್ಣಾಙ್ಕ a[2] = {-1, -1}, b[2] = {-1, -1};
                    minMaxIdx_(chunks[i].src, &mn[i], &mx[i], cn == 1 ? a : nullptr, cn == 1 ? b : nullptr,
                               chunks[i].mask);
                    ಯದಿ (cn > 1)
                        imn[i] = imx[i] = 1;
                    ಅನ್ಯಥಾ ಯದಿ (a[0] >= 0)
                    {
                        imn[i] = chunkIndexToElem(chunks[i], a) + 1;
                        imx[i] = chunkIndexToElem(chunks[i], b) + 1;
                    }
                }
            });
            // pieces are in memory order, so taking the first strict improvement keeps the
            // first occurrence, as in the serial version
            ಪೂರ್ಣಾಙ್ಕ bmin = -1, bmax = -1;
            ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < n; i++)
            {
                ಯದಿ (imn[i] == 0)
                    ಅನುವರ್ತ;
                ಯದಿ (bmin < 0 || mn[i] < mn[bmin]) bmin = i;
                ಯದಿ (bmax < 0 || mx[i] > mx[bmax]) bmax = i;
            }
            ಯದಿ (minVal) *minVal = bmin >= 0 ? mn[bmin] : 0;
            ಯದಿ (maxVal) *maxVal = bmax >= 0 ? mx[bmax] : 0;
            ಯದಿ (minIdx) ofs2idx(src, bmin >= 0 ? imn[bmin] : 0, minIdx);
            ಯದಿ (maxIdx) ofs2idx(src, bmax >= 0 ? imx[bmax] : 0, maxIdx);
            ಪ್ರತ್ಯಾಗಮ;
        }
    }
    minMaxIdx_(_src, minVal, maxVal, minIdx, maxIdx, _mask);
}

ಶೂನ್ಯ cv::minMaxLoc( ಒಳಹರಿವು_Array _img, ದ್ವಿಗುಣ* minVal, ದ್ವಿಗುಣ* maxVal,
                    Point* minLoc, Point* maxLoc, ಒಳಹರಿವು_Array mask )
{
    CV_INSTRUMENT_REGION();

    ಪೂರ್ಣಾಙ್ಕ dims = _img.dims();
    CV_CheckLE(dims, 2, "");

    minMaxIdx(_img, minVal, maxVal, (ಪೂರ್ಣಾಙ್ಕ*)minLoc, (ಪೂರ್ಣಾಙ್ಕ*)maxLoc, mask);
    ಯದಿ( minLoc) {
        ಯದಿ (dims == 2)
            std::swap(minLoc->x, minLoc->y);
        ಅನ್ಯಥಾ {
            minLoc->y = 0;
        }
    }
    ಯದಿ( maxLoc) {
        ಯದಿ (dims == 2)
            std::swap(maxLoc->x, maxLoc->y);
        ಅನ್ಯಥಾ {
            maxLoc->y = 0;
        }
    }
}

ಗಣನಾ ವರ್ಗ ReduceMode
{
    FIRST_MIN = 0, //!< get index of first min occurrence
    LAST_MIN  = 1, //!< get index of last min occurrence
    FIRST_MAX = 2, //!< get index of first max occurrence
    LAST_MAX  = 3, //!< get index of last max occurrence
};

template <typename T>
ಸಂರಚನಾ reduceMinMaxImpl
{
    ಶೂನ್ಯ operator()(ಧ್ರುವ cv::Mat& src, cv::Mat& dst, ReduceMode mode, ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ axis) ಧ್ರುವ
    {
        ವಿಕಲ್ಪ(mode)
        {
        ಪಕ್ಷ ReduceMode::FIRST_MIN:
            reduceMinMaxApply<std::less>(src, dst, axis);
            ಭಙ್ಗ;
        ಪಕ್ಷ ReduceMode::LAST_MIN:
            reduceMinMaxApply<std::less_equal>(src, dst, axis);
            ಭಙ್ಗ;
        ಪಕ್ಷ ReduceMode::FIRST_MAX:
            reduceMinMaxApply<std::greater>(src, dst, axis);
            ಭಙ್ಗ;
        ಪಕ್ಷ ReduceMode::LAST_MAX:
            reduceMinMaxApply<std::greater_equal>(src, dst, axis);
            ಭಙ್ಗ;
        }
    }

    template <template<ವರ್ಗ> ವರ್ಗ Cmp>
    ಸ್ಥಿರ ಶೂನ್ಯ reduceMinMaxApply(ಧ್ರುವ cv::Mat& src, cv::Mat& dst, ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ axis)
    {
        Cmp<T> cmp;

        ಧ್ರುವ auto *src_ptr = src.ptr<T>();
        auto *dst_ptr = dst.ptr<int32_t>();

        ಧ್ರುವ size_t outer_size = src.total(0, axis);
        ಧ್ರುವ auto mid_size = static_cast<size_t>(src.size[axis]);

        ಧ್ರುವ size_t outer_step = src.total(axis);
        ಧ್ರುವ size_t dst_step = dst.total(axis);

        ಧ್ರುವ size_t mid_step = src.total(axis + 1);

        ಪ್ರತಿ (size_t outer = 0; outer < outer_size; ++outer)
        {
            ಧ್ರುವ size_t outer_offset = outer * outer_step;
            ಧ್ರುವ size_t dst_offset = outer * dst_step;
            ಪ್ರತಿ (size_t mid = 0; mid != mid_size; ++mid)
            {
                ಧ್ರುವ size_t src_offset = outer_offset + mid * mid_step;
                ಪ್ರತಿ (size_t inner = 0; inner < mid_step; inner++)
                {
                    int32_t& index = dst_ptr[dst_offset + inner];

                    ಧ್ರುವ size_t prev = outer_offset + index * mid_step + inner;
                    ಧ್ರುವ size_t curr = src_offset + inner;

                    ಯದಿ (cmp(src_ptr[curr], src_ptr[prev]))
                    {
                        index = static_cast<int32_t>(mid);
                    }
                }
            }
        }
    }
};

ಸ್ಥಿರ ಶೂನ್ಯ reduceMinMax(cv::ಒಳಹರಿವು_Array src, cv::OutputArray dst, ReduceMode mode, ಪೂರ್ಣಾಙ್ಕ axis)
{
    CV_INSTRUMENT_REGION();

    cv::Mat srcMat = src.getMat();
    ಪೂರ್ಣಾಙ್ಕ dims = std::max(1, srcMat.dims);
    axis = (axis + dims) % dims;
    CV_Assert(srcMat.channels() == 1 && axis >= 0 && axis <= srcMat.dims);

    std::vector<ಪೂರ್ಣಾಙ್ಕ> sizes(srcMat.dims);
    std::copy(srcMat.size.p, srcMat.size.p + srcMat.dims, sizes.begin());
    ಯದಿ(!sizes.empty())
        sizes[axis] = 1;

    dst.create(srcMat.dims, sizes.data(), CV_32SC1); // indices
    cv::Mat dstMat = dst.getMat();
    dstMat.setTo(cv::Scalar::all(0));

    ಯದಿ (!srcMat.isContinuous())
    {
        srcMat = srcMat.clone();
    }

    ತಾರ್ಕಿಕ needs_copy = !dstMat.isContinuous();
    ಯದಿ (needs_copy)
    {
        dstMat = dstMat.clone();
    }

    cv::detail::depthDispatch<reduceMinMaxImpl>(srcMat.depth(), srcMat, dstMat, mode, axis);

    ಯದಿ (needs_copy)
    {
        dstMat.copyTo(dst);
    }
}

ಶೂನ್ಯ cv::reduceArgMin(ಒಳಹರಿವು_Array src, OutputArray dst, ಪೂರ್ಣಾಙ್ಕ axis, ತಾರ್ಕಿಕ lastIndex)
{
    reduceMinMax(src, dst, lastIndex ? ReduceMode::LAST_MIN : ReduceMode::FIRST_MIN, axis);
}

ಶೂನ್ಯ cv::reduceArgMax(ಒಳಹರಿವು_Array src, OutputArray dst, ಪೂರ್ಣಾಙ್ಕ axis, ತಾರ್ಕಿಕ lastIndex)
{
    reduceMinMax(src, dst, lastIndex ? ReduceMode::LAST_MAX : ReduceMode::FIRST_MAX, axis);
}
