// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


#ଅନ୍ତର୍ଭାଵ "precomp.hpp"
#ଅନ୍ତର୍ଭାଵ "opencl_kernels_core.hpp"
#ଅନ୍ତର୍ଭାଵ "stat.hpp"
#ଅନ୍ତର୍ଭାଵ "stat_parallel.hpp"
#ଅନ୍ତର୍ଭାଵ "opencv2/core/detail/dispatch_helper.impl.hpp"
#ଅନ୍ତର୍ଭାଵ <algorithm>

#ଅନ୍ତର୍ଭାଵ "minmax.simd.hpp"
#ଅନ୍ତର୍ଭାଵ "minmax.simd_declarations.hpp" // defines CV_CPU_DISPATCH_MODES_ALL=AVX2,...,BASELINE based on CMakeLists.txt content

ନାମାକାଶ cv {

ସ୍ଥିର MinMaxIdxFunc getMinMaxIdxFunc(ପୂର୍ଣ୍ଣସଂଖ୍ୟା depth)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getMinMaxIdxFunc, (depth),
                    CV_CPU_DISPATCH_MODES_ALL);
}

// The function expects 1-based indexing for ofs
// Zero is treated as invalid offset (not found)
ସ୍ଥିର ଶୂନ୍ୟ ofs2idx(ଅଚଳ Mat& a, size_t ofs, ପୂର୍ଣ୍ଣସଂଖ୍ୟା* idx)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i, d = a.dims;
    ଯଦି( ofs > 0 )
    {
        ofs--;
        ପାଇଁ( i = d-1; i >= 0; i-- )
        {
            ପୂର୍ଣ୍ଣସଂଖ୍ୟା sz = a.size[i];
            idx[i] = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)(ofs % sz);
            ofs /= sz;
        }
    }
    ଅନ୍ୟଥା
    {
        ପାଇଁ( i = d-1; i >= 0; i-- )
            idx[i] = -1;
    }
}

#ifdef HAVE_OPENCL

#define MINMAX_STRUCT_ALIGNMENT 8 // sizeof double

template <typename T>
ଶୂନ୍ୟ getMinMaxRes(ଅଚଳ Mat & db, ଦ୍ୱିଗୁଣ * minVal, ଦ୍ୱିଗୁଣ * maxVal,
                  ପୂର୍ଣ୍ଣସଂଖ୍ୟା* minLoc, ପୂର୍ଣ୍ଣସଂଖ୍ୟା* maxLoc,
                  ପୂର୍ଣ୍ଣସଂଖ୍ୟା groupnum, ପୂର୍ଣ୍ଣସଂଖ୍ୟା cols, ଦ୍ୱିଗୁଣ * maxVal2)
{
    uint index_max = std::numeric_limits<uint>::max();
    T minval = std::numeric_limits<T>::max();
    T maxval = std::numeric_limits<T>::min() > 0 ? (T)-std::numeric_limits<T>::max() : std::numeric_limits<T>::min(), maxval2 = maxval;
    uint minloc = index_max, maxloc = index_max;

    size_t index = 0;
    ଅଚଳ T * minptr = ଖାଲି, * maxptr = ଖାଲି, * maxptr2 = ଖାଲି;
    ଅଚଳ uint * minlocptr = ଖାଲି, * maxlocptr = ଖାଲି;
    ଯଦି (minVal || minLoc)
    {
        minptr = db.ptr<T>();
        index += ଆକାର(T) * groupnum;
        index = alignSize(index, MINMAX_STRUCT_ALIGNMENT);
    }
    ଯଦି (maxVal || maxLoc)
    {
        maxptr = (ଅଚଳ T *)(db.ptr() + index);
        index += ଆକାର(T) * groupnum;
        index = alignSize(index, MINMAX_STRUCT_ALIGNMENT);
    }
    ଯଦି (minLoc)
    {
        minlocptr = (ଅଚଳ uint *)(db.ptr() + index);
        index += ଆକାର(uint) * groupnum;
        index = alignSize(index, MINMAX_STRUCT_ALIGNMENT);
    }
    ଯଦି (maxLoc)
    {
        maxlocptr = (ଅଚଳ uint *)(db.ptr() + index);
        index += ଆକାର(uint) * groupnum;
        index = alignSize(index, MINMAX_STRUCT_ALIGNMENT);
    }
    ଯଦି (maxVal2)
        maxptr2 = (ଅଚଳ T *)(db.ptr() + index);

    ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < groupnum; i++)
    {
        ଯଦି (minptr && minptr[i] <= minval)
        {
            ଯଦି (minptr[i] == minval)
            {
                ଯଦି (minlocptr)
                    minloc = std::min(minlocptr[i], minloc);
            }
            ଅନ୍ୟଥା
            {
                ଯଦି (minlocptr)
                    minloc = minlocptr[i];
                minval = minptr[i];
            }
        }
        ଯଦି (maxptr && maxptr[i] >= maxval)
        {
            ଯଦି (maxptr[i] == maxval)
            {
                ଯଦି (maxlocptr)
                    maxloc = std::min(maxlocptr[i], maxloc);
            }
            ଅନ୍ୟଥା
            {
                ଯଦି (maxlocptr)
                    maxloc = maxlocptr[i];
                maxval = maxptr[i];
            }
        }
        ଯଦି (maxptr2 && maxptr2[i] > maxval2)
            maxval2 = maxptr2[i];
    }
    ତାର୍କିକ zero_mask = (minLoc && minloc == index_max) ||
    (maxLoc && maxloc == index_max);

    ଯଦି (minVal)
        *minVal = zero_mask ? 0 : (ଦ୍ୱିଗୁଣ)minval;
    ଯଦି (maxVal)
        *maxVal = zero_mask ? 0 : (ଦ୍ୱିଗୁଣ)maxval;
    ଯଦି (maxVal2)
        *maxVal2 = zero_mask ? 0 : (ଦ୍ୱିଗୁଣ)maxval2;

    ଯଦି (minLoc)
    {
        minLoc[0] = zero_mask ? -1 : minloc / cols;
        minLoc[1] = zero_mask ? -1 : minloc % cols;
    }
    ଯଦି (maxLoc)
    {
        maxLoc[0] = zero_mask ? -1 : maxloc / cols;
        maxLoc[1] = zero_mask ? -1 : maxloc % cols;
    }
}

ପ୍ରକାରବ୍ୟାଖ୍ୟା ଶୂନ୍ୟ (*getMinMaxResFunc)(ଅଚଳ Mat & db, ଦ୍ୱିଗୁଣ * minVal, ଦ୍ୱିଗୁଣ * maxVal,
                                 ପୂର୍ଣ୍ଣସଂଖ୍ୟା * minLoc, ପୂର୍ଣ୍ଣସଂଖ୍ୟା *maxLoc, ପୂର୍ଣ୍ଣସଂଖ୍ୟା gropunum, ପୂର୍ଣ୍ଣସଂଖ୍ୟା cols, ଦ୍ୱିଗୁଣ * maxVal2);

ତାର୍କିକ ocl_minMaxIdx( ନିବେଶ_Array _src, ଦ୍ୱିଗୁଣ* minVal, ଦ୍ୱିଗୁଣ* maxVal, ପୂର୍ଣ୍ଣସଂଖ୍ୟା* minLoc, ପୂର୍ଣ୍ଣସଂଖ୍ୟା* maxLoc, ନିବେଶ_Array _mask,
                   ପୂର୍ଣ୍ଣସଂଖ୍ୟା ddepth, ତାର୍କିକ abs_ମୂଲ୍ୟଗୁଡ଼ିକ, ନିବେଶ_Array _src2, ଦ୍ୱିଗୁଣ * maxVal2)
{
    ଅଚଳ ocl::Device & dev = ocl::Device::getDefault();

#ifdef __ANDROID__
    ଯଦି (dev.isNVidia())
        ଫେରନ୍ତୁ ମିଥ୍ୟା;
#endif

    ଯଦି (dev.deviceVersionMajor() == 1 && dev.deviceVersionMinor() < 2)
    {
        // 'static' storage class specifier used by "minmaxloc" is available from OpenCL 1.2+ only
        ଫେରନ୍ତୁ ମିଥ୍ୟା;
    }

    ତାର୍କିକ doubleSupport = dev.doubleFPConfig() > 0, haveMask = !_mask.empty(),
    haveSrc2 = _src2.kind() != ନିବେଶ_Array_2::NONE;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା type = _src.type(), depth = CV_MAT_DEPTH(type), cn = CV_MAT_CN(type),
    kercn = haveMask ? cn : std::min(4, ocl::predictOptimalVectorWidth(_src, _src2));

    ଯଦି (depth >= CV_16F)
        ଫେରନ୍ତୁ ମିଥ୍ୟା;

    // disabled following modes since it occasionally fails on AMD devices (e.g. A10-6800K, sep. 2014)
    ଯଦି ((haveMask || type == CV_32FC1) && dev.isAMD())
        ଫେରନ୍ତୁ ମିଥ୍ୟା;

    CV_Assert( (cn == 1 && (!haveMask || _mask.type() == CV_8U || _mask.type() == CV_8S || _mask.type() == CV_Bool)) ||
              (cn >= 1 && !minLoc && !maxLoc) );

    ଯଦି (ddepth < 0)
        ddepth = depth;

    CV_Assert(!haveSrc2 || _src2.type() == type);

    ଯଦି (depth == CV_32S || depth == CV_8S || depth == CV_32U || depth == CV_64U ||
        depth == CV_64S || depth == CV_16F || depth == CV_16BF)
        ଫେରନ୍ତୁ ମିଥ୍ୟା;

    ଯଦି ((depth == CV_64F || ddepth == CV_64F) && !doubleSupport)
        ଫେରନ୍ତୁ ମିଥ୍ୟା;

    ପୂର୍ଣ୍ଣସଂଖ୍ୟା groupnum = dev.maxComputeUnits();
    size_t wgs = dev.maxWorkGroupSize();

    ପୂର୍ଣ୍ଣସଂଖ୍ୟା wgs2_aligned = 1;
    ଯେତେବେଳେ (wgs2_aligned < (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)wgs)
        wgs2_aligned <<= 1;
    wgs2_aligned >>= 1;

    ତାର୍କିକ needMinVal = minVal || minLoc, needMinLoc = minLoc != ଖାଲି,
    needMaxVal = maxVal || maxLoc, needMaxLoc = maxLoc != ଖାଲି;

    // in case of mask we must know whether mask is filled with zeros or not
    // so let's calculate min or max location, if it's undefined, so mask is zeros
    ଯଦି (!(needMaxLoc || needMinLoc) && haveMask)
    {
        ଯଦି (needMinVal)
            needMinLoc = ସତ୍ୟ;
        ଅନ୍ୟଥା
            needMaxLoc = ସତ୍ୟ;
    }

    ଅକ୍ଷର cvt[2][50];
    String opts = format("-D DEPTH_%d -D srcT1=%s%s -D WGS=%d -D srcT=%s"
                         " -D WGS2_ALIGNED=%d%s%s%s -D kercn=%d%s%s%s%s"
                         " -D dstT1=%s -D dstT=%s -D convertToDT=%s%s%s%s%s -D wdepth=%d -D convertFromU=%s"
                         " -D MINMAX_STRUCT_ALIGNMENT=%d",
                         depth, ocl::typeToStr(depth), haveMask ? " -D HAVE_MASK" : "", (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)wgs,
                         ocl::typeToStr(CV_MAKE_TYPE(depth, kercn)), wgs2_aligned,
                         doubleSupport ? " -D DOUBLE_SUPPORT" : "",
                         _src.isContinuous() ? " -D HAVE_SRC_CONT" : "",
                         _mask.isContinuous() ? " -D HAVE_MASK_CONT" : "", kercn,
                         needMinVal ? " -D NEED_MINVAL" : "", needMaxVal ? " -D NEED_MAXVAL" : "",
                         needMinLoc ? " -D NEED_MINLOC" : "", needMaxLoc ? " -D NEED_MAXLOC" : "",
                         ocl::typeToStr(ddepth), ocl::typeToStr(CV_MAKE_TYPE(ddepth, kercn)),
                         ocl::convertTypeStr(depth, ddepth, kercn, cvt[0], ଆକାର(cvt[0])),
                         abs_ମୂଲ୍ୟଗୁଡ଼ିକ ? " -D OP_ABS" : "",
                         haveSrc2 ? " -D HAVE_SRC2" : "", maxVal2 ? " -D OP_CALC2" : "",
                         haveSrc2 && _src2.isContinuous() ? " -D HAVE_SRC2_CONT" : "", ddepth,
                         depth <= CV_32S && ddepth == CV_32S ? ocl::convertTypeStr(CV_8U, ddepth, kercn, cvt[1], ଆକାର(cvt[1])) : "noconvert",
                         MINMAX_STRUCT_ALIGNMENT);

    ocl::Kernel k("minmaxloc", ocl::core::minmaxloc_oclsrc, opts);
    ଯଦି (k.empty())
        ଫେରନ୍ତୁ ମିଥ୍ୟା;

    ପୂର୍ଣ୍ଣସଂଖ୍ୟା esz = CV_ELEM_SIZE(ddepth), esz32s = CV_ELEM_SIZE1(CV_32S),
    dbsize = groupnum * ((needMinVal ? esz : 0) + (needMaxVal ? esz : 0) +
                         (needMinLoc ? esz32s : 0) + (needMaxLoc ? esz32s : 0) +
                         (maxVal2 ? esz : 0))
    + 5 * MINMAX_STRUCT_ALIGNMENT;
    UMat src = _src.getUMat(), src2 = _src2.getUMat(), db(1, dbsize, CV_8UC1), mask = _mask.getUMat();

    ଯଦି (cn > 1 && !haveMask)
    {
        src = src.reshape(1);
        src2 = src2.reshape(1);
    }

    ଯଦି (haveSrc2)
    {
        ଯଦି (!haveMask)
            k.args(ocl::KernelArg::ReadOnlyNoSize(src), src.cols, (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)src.total(),
                   groupnum, ocl::KernelArg::PtrWriteOnly(db), ocl::KernelArg::ReadOnlyNoSize(src2));
        ଅନ୍ୟଥା
            k.args(ocl::KernelArg::ReadOnlyNoSize(src), src.cols, (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)src.total(),
                   groupnum, ocl::KernelArg::PtrWriteOnly(db), ocl::KernelArg::ReadOnlyNoSize(mask),
                   ocl::KernelArg::ReadOnlyNoSize(src2));
    }
    ଅନ୍ୟଥା
    {
        ଯଦି (!haveMask)
            k.args(ocl::KernelArg::ReadOnlyNoSize(src), src.cols, (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)src.total(),
                   groupnum, ocl::KernelArg::PtrWriteOnly(db));
        ଅନ୍ୟଥା
            k.args(ocl::KernelArg::ReadOnlyNoSize(src), src.cols, (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)src.total(),
                   groupnum, ocl::KernelArg::PtrWriteOnly(db), ocl::KernelArg::ReadOnlyNoSize(mask));
    }

    size_t globalsize = groupnum * wgs;
    ଯଦି (!k.run(1, &globalsize, &wgs, ସତ୍ୟ))
        ଫେରନ୍ତୁ ମିଥ୍ୟା;

    ସ୍ଥିର ଅଚଳ getMinMaxResFunc functab[7] =
    {
        getMinMaxRes<uchar>,
        getMinMaxRes<ଅକ୍ଷର>,
        getMinMaxRes<ushort>,
        getMinMaxRes<short>,
        getMinMaxRes<ପୂର୍ଣ୍ଣସଂଖ୍ୟା>,
        getMinMaxRes<ଦଶମିକ>,
        getMinMaxRes<ଦ୍ୱିଗୁଣ>
    };

    CV_Assert(ddepth <= CV_64F);
    getMinMaxResFunc func = functab[ddepth];

    ପୂର୍ଣ୍ଣସଂଖ୍ୟା locTemp[2];
    func(db.getMat(ACCESS_READ), minVal, maxVal,
         needMinLoc ? minLoc ? minLoc : locTemp : minLoc,
         needMaxLoc ? maxLoc ? maxLoc : locTemp : maxLoc,
         groupnum, src.cols, maxVal2);

    ଫେରନ୍ତୁ ସତ୍ୟ;
}

#endif

}

// serial implementation, also used for the pieces of the parallel version
ସ୍ଥିର ଶୂନ୍ୟ minMaxIdx_(cv::ନିବେଶ_Array _src, ଦ୍ୱିଗୁଣ* minVal,
                       ଦ୍ୱିଗୁଣ* maxVal, ପୂର୍ଣ୍ଣସଂଖ୍ୟା* minIdx, ପୂର୍ଣ୍ଣସଂଖ୍ୟା* maxIdx,
                       cv::ନିବେଶ_Array _mask)
{
    using ନାମାକାଶ cv;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା type = _src.type(), depth = CV_MAT_DEPTH(type), cn = CV_MAT_CN(type);
    CV_Assert( (cn == 1 && (_mask.empty() || _mask.type() == CV_8U || _mask.type() == CV_8S || _mask.type() == CV_Bool)) ||
               (cn > 1 && _mask.empty() && !minIdx && !maxIdx) );

    CV_OCL_RUN(OCL_PERFORMANCE_CHECK(_src.isUMat()) && _src.dims() <= 2  && (_mask.empty() || _src.size() == _mask.size()),
               ocl_minMaxIdx(_src, minVal, maxVal, minIdx, maxIdx, _mask))

    Mat src = _src.getMat(), mask = _mask.getMat();

    ଯଦି (src.dims <= 2)
    {
        ଯଦି ((size_t)src.step == (size_t)mask.step || mask.empty())
        {
            CALL_HAL(minMaxIdx, cv_hal_minMaxIdx, src.data, src.step, src.cols*cn, src.rows,
                     src.depth(), minVal, maxVal, minIdx, maxIdx, mask.data);
        }
        CALL_HAL(minMaxIdxMaskStep, cv_hal_minMaxIdxMaskStep, src.data, src.step, src.cols*cn, src.rows,
                 src.depth(), minVal, maxVal, minIdx, maxIdx, mask.data, mask.step);
    }
    ଅନ୍ୟଥା ଯଦି (src.isContinuous() && (mask.isContinuous() || mask.empty()))
    {
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା res = cv_hal_minMaxIdx(src.data, 0, (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)src.total()*cn, 1, src.depth(),
                                   minVal, maxVal, minIdx, maxIdx, mask.data);

        ଯଦି (res == CV_HAL_ERROR_OK)
        {
            // minIdx[0] and minIdx[0] are always 0 for "flatten" version
            ଯଦି (minIdx)
                ofs2idx(src, minIdx[1]+1, minIdx);
            ଯଦି (maxIdx)
                ofs2idx(src, maxIdx[1]+1, maxIdx);
            ଫେରନ୍ତୁ;
        }
        ଅନ୍ୟଥା ଯଦି (res != CV_HAL_ERROR_NOT_IMPLEMENTED)
        {
            CV_Error_(cv::Error::StsInternal,
            ("HAL implementation minMaxIdx ==> " CVAUX_STR(cv_hal_minMaxIdx) " returned %d (0x%08x)", res, res));
        }
    }

    MinMaxIdxFunc func = getMinMaxIdxFunc(depth);
    CV_Assert( func != 0 );

    ଅଚଳ Mat* arrays[] = {&src, &mask, 0};
    uchar* ptrs[2] = {};
    NAryMatIterator it(arrays, ptrs);

    size_t minidx = 0, maxidx = 0;
    size_t startidx = 1;
    ସଂଘ {
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;
        ଦଶମିକ f;
        ଦ୍ୱିଗୁଣ d;
        int64 L;
        uint64 UL;
    } minval, maxval;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା planeSize = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)it.size*cn;
    minval.L = maxval.L = 0;

    ପାଇଁ( size_t i = 0; i < it.nplanes; i++, ++it, startidx += planeSize )
        func( ptrs[0], ptrs[1], &minval.L, &maxval.L, &minidx, &maxidx, planeSize, startidx );

    ଦ୍ୱିଗୁଣ dminval, dmaxval;
    ଯଦି( depth <= CV_32S || depth == CV_Bool )
        dminval = minval.i, dmaxval = maxval.i;
    ଅନ୍ୟଥା ଯଦି( depth == CV_32F || depth == CV_16F || depth == CV_16BF )
        dminval = minval.f, dmaxval = maxval.f;
    ଅନ୍ୟଥା ଯଦି( depth == CV_64F )
        dminval = minval.d, dmaxval = maxval.d;
    ଅନ୍ୟଥା ଯଦି( depth == CV_64S || depth == CV_32U )
        dminval = (ଦ୍ୱିଗୁଣ)minval.L, dmaxval = (ଦ୍ୱିଗୁଣ)maxval.L;
    ଅନ୍ୟଥା {
        CV_Assert(depth == CV_64U);
        dminval = (ଦ୍ୱିଗୁଣ)minval.UL, dmaxval = (ଦ୍ୱିଗୁଣ)maxval.UL;
    }

    ଯଦି( minVal )
        *minVal = dminval;
    ଯଦି( maxVal )
        *maxVal = dmaxval;

    ଯଦି( minIdx )
        ofs2idx(src, minidx, minIdx);
    ଯଦି( maxIdx )
        ofs2idx(src, maxidx, maxIdx);
}

ଶୂନ୍ୟ cv::minMaxIdx(ନିବେଶ_Array _src, ଦ୍ୱିଗୁଣ* minVal,
                   ଦ୍ୱିଗୁଣ* maxVal, ପୂର୍ଣ୍ଣସଂଖ୍ୟା* minIdx, ପୂର୍ଣ୍ଣସଂଖ୍ୟା* maxIdx,
                   ନିବେଶ_Array _mask)
{
    CV_INSTRUMENT_REGION();

    std::vector<StatChunk> chunks;
    ଯଦି (!_src.isUMat())
    {
        Mat src = _src.getMat(), mask = _mask.getMat();
        ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା cn = src.channels();
        ଅଚଳ ତାର୍କିକ argsOk = (cn == 1 && (mask.empty() || ((mask.type() == CV_8U || mask.type() == CV_8S ||
                             mask.type() == CV_Bool) && src.size == mask.size))) ||
                            (cn > 1 && mask.empty() && !minIdx && !maxIdx);
        ଯଦି (argsOk && splitForParallelStat(src, mask, chunks))
        {
            ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା n = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)chunks.size();
            std::vector<ଦ୍ୱିଗୁଣ> mn(n), mx(n);
            std::vector<size_t> imn(n, 0), imx(n, 0);   // 1-based row-major indices, 0 = no element
            parallel_for_(Range(0, n), [&](ଅଚଳ Range& r)
            {
                ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = r.start; i < r.end; i++)
                {
                    ପୂର୍ଣ୍ଣସଂଖ୍ୟା a[2] = {-1, -1}, b[2] = {-1, -1};
                    minMaxIdx_(chunks[i].src, &mn[i], &mx[i], cn == 1 ? a : nullptr, cn == 1 ? b : nullptr,
                               chunks[i].mask);
                    ଯଦି (cn > 1)
                        imn[i] = imx[i] = 1;
                    ଅନ୍ୟଥା ଯଦି (a[0] >= 0)
                    {
                        imn[i] = chunkIndexToElem(chunks[i], a) + 1;
                        imx[i] = chunkIndexToElem(chunks[i], b) + 1;
                    }
                }
            });
            // pieces are in memory order, so taking the first strict improvement keeps the
            // first occurrence, as in the serial version
            ପୂର୍ଣ୍ଣସଂଖ୍ୟା bmin = -1, bmax = -1;
            ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < n; i++)
            {
                ଯଦି (imn[i] == 0)
                    ଜାରି;
                ଯଦି (bmin < 0 || mn[i] < mn[bmin]) bmin = i;
                ଯଦି (bmax < 0 || mx[i] > mx[bmax]) bmax = i;
            }
            ଯଦି (minVal) *minVal = bmin >= 0 ? mn[bmin] : 0;
            ଯଦି (maxVal) *maxVal = bmax >= 0 ? mx[bmax] : 0;
            ଯଦି (minIdx) ofs2idx(src, bmin >= 0 ? imn[bmin] : 0, minIdx);
            ଯଦି (maxIdx) ofs2idx(src, bmax >= 0 ? imx[bmax] : 0, maxIdx);
            ଫେରନ୍ତୁ;
        }
    }
    minMaxIdx_(_src, minVal, maxVal, minIdx, maxIdx, _mask);
}

ଶୂନ୍ୟ cv::minMaxLoc( ନିବେଶ_Array _img, ଦ୍ୱିଗୁଣ* minVal, ଦ୍ୱିଗୁଣ* maxVal,
                    Point* minLoc, Point* maxLoc, ନିବେଶ_Array mask )
{
    CV_INSTRUMENT_REGION();

    ପୂର୍ଣ୍ଣସଂଖ୍ୟା dims = _img.dims();
    CV_CheckLE(dims, 2, "");

    minMaxIdx(_img, minVal, maxVal, (ପୂର୍ଣ୍ଣସଂଖ୍ୟା*)minLoc, (ପୂର୍ଣ୍ଣସଂଖ୍ୟା*)maxLoc, mask);
    ଯଦି( minLoc) {
        ଯଦି (dims == 2)
            std::swap(minLoc->x, minLoc->y);
        ଅନ୍ୟଥା {
            minLoc->y = 0;
        }
    }
    ଯଦି( maxLoc) {
        ଯଦି (dims == 2)
            std::swap(maxLoc->x, maxLoc->y);
        ଅନ୍ୟଥା {
            maxLoc->y = 0;
        }
    }
}

ଗଣନା ଶ୍ରେଣୀ ReduceMode
{
    FIRST_MIN = 0, //!< get index of first min occurrence
    LAST_MIN  = 1, //!< get index of last min occurrence
    FIRST_MAX = 2, //!< get index of first max occurrence
    LAST_MAX  = 3, //!< get index of last max occurrence
};

template <typename T>
ସଂରଚନା reduceMinMaxImpl
{
    ଶୂନ୍ୟ operator()(ଅଚଳ cv::Mat& src, cv::Mat& dst, ReduceMode mode, ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା axis) ଅଚଳ
    {
        ବାଛ(mode)
        {
        ସ୍ଥିତି ReduceMode::FIRST_MIN:
            reduceMinMaxApply<std::less>(src, dst, axis);
            ଭାଙ୍ଗ;
        ସ୍ଥିତି ReduceMode::LAST_MIN:
            reduceMinMaxApply<std::less_equal>(src, dst, axis);
            ଭାଙ୍ଗ;
        ସ୍ଥିତି ReduceMode::FIRST_MAX:
            reduceMinMaxApply<std::greater>(src, dst, axis);
            ଭାଙ୍ଗ;
        ସ୍ଥିତି ReduceMode::LAST_MAX:
            reduceMinMaxApply<std::greater_equal>(src, dst, axis);
            ଭାଙ୍ଗ;
        }
    }

    template <template<ଶ୍ରେଣୀ> ଶ୍ରେଣୀ Cmp>
    ସ୍ଥିର ଶୂନ୍ୟ reduceMinMaxApply(ଅଚଳ cv::Mat& src, cv::Mat& dst, ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା axis)
    {
        Cmp<T> cmp;

        ଅଚଳ auto *src_ptr = src.ptr<T>();
        auto *dst_ptr = dst.ptr<int32_t>();

        ଅଚଳ size_t outer_size = src.total(0, axis);
        ଅଚଳ auto mid_size = static_cast<size_t>(src.size[axis]);

        ଅଚଳ size_t outer_step = src.total(axis);
        ଅଚଳ size_t dst_step = dst.total(axis);

        ଅଚଳ size_t mid_step = src.total(axis + 1);

        ପାଇଁ (size_t outer = 0; outer < outer_size; ++outer)
        {
            ଅଚଳ size_t outer_offset = outer * outer_step;
            ଅଚଳ size_t dst_offset = outer * dst_step;
            ପାଇଁ (size_t mid = 0; mid != mid_size; ++mid)
            {
                ଅଚଳ size_t src_offset = outer_offset + mid * mid_step;
                ପାଇଁ (size_t inner = 0; inner < mid_step; inner++)
                {
                    int32_t& index = dst_ptr[dst_offset + inner];

                    ଅଚଳ size_t prev = outer_offset + index * mid_step + inner;
                    ଅଚଳ size_t curr = src_offset + inner;

                    ଯଦି (cmp(src_ptr[curr], src_ptr[prev]))
                    {
                        index = static_cast<int32_t>(mid);
                    }
                }
            }
        }
    }
};

ସ୍ଥିର ଶୂନ୍ୟ reduceMinMax(cv::ନିବେଶ_Array src, cv::OutputArray dst, ReduceMode mode, ପୂର୍ଣ୍ଣସଂଖ୍ୟା axis)
{
    CV_INSTRUMENT_REGION();

    cv::Mat srcMat = src.getMat();
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା dims = std::max(1, srcMat.dims);
    axis = (axis + dims) % dims;
    CV_Assert(srcMat.channels() == 1 && axis >= 0 && axis <= srcMat.dims);

    std::vector<ପୂର୍ଣ୍ଣସଂଖ୍ୟା> sizes(srcMat.dims);
    std::copy(srcMat.size.p, srcMat.size.p + srcMat.dims, sizes.begin());
    ଯଦି(!sizes.empty())
        sizes[axis] = 1;

    dst.create(srcMat.dims, sizes.data(), CV_32SC1); // indices
    cv::Mat dstMat = dst.getMat();
    dstMat.setTo(cv::Scalar::all(0));

    ଯଦି (!srcMat.isContinuous())
    {
        srcMat = srcMat.clone();
    }

    ତାର୍କିକ needs_copy = !dstMat.isContinuous();
    ଯଦି (needs_copy)
    {
        dstMat = dstMat.clone();
    }

    cv::detail::depthDispatch<reduceMinMaxImpl>(srcMat.depth(), srcMat, dstMat, mode, axis);

    ଯଦି (needs_copy)
    {
        dstMat.copyTo(dst);
    }
}

ଶୂନ୍ୟ cv::reduceArgMin(ନିବେଶ_Array src, OutputArray dst, ପୂର୍ଣ୍ଣସଂଖ୍ୟା axis, ତାର୍କିକ lastIndex)
{
    reduceMinMax(src, dst, lastIndex ? ReduceMode::LAST_MIN : ReduceMode::FIRST_MIN, axis);
}

ଶୂନ୍ୟ cv::reduceArgMax(ନିବେଶ_Array src, OutputArray dst, ପୂର୍ଣ୍ଣସଂଖ୍ୟା axis, ତାର୍କିକ lastIndex)
{
    reduceMinMax(src, dst, lastIndex ? ReduceMode::LAST_MAX : ReduceMode::FIRST_MAX, axis);
}
