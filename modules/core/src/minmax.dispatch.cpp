// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


#அந்தர்பாவ "precomp.hpp"
#அந்தர்பாவ "opencl_kernels_core.hpp"
#அந்தர்பாவ "stat.hpp"
#அந்தர்பாவ "stat_parallel.hpp"
#அந்தர்பாவ "opencv2/core/detail/dispatch_helper.impl.hpp"
#அந்தர்பாவ <algorithm>

#அந்தர்பாவ "minmax.simd.hpp"
#அந்தர்பாவ "minmax.simd_declarations.hpp" // defines CV_CPU_DISPATCH_MODES_ALL=AVX2,...,BASELINE based on CMakeLists.txt content

நாமாகாஶ cv {

நிலையான MinMaxIdxFunc getMinMaxIdxFunc(முழு depth)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getMinMaxIdxFunc, (depth),
                    CV_CPU_DISPATCH_MODES_ALL);
}

// The function expects 1-based indexing for ofs
// Zero is treated as invalid offset (not found)
நிலையான வெறுமை ofs2idx(மாறிலி Mat& a, size_t ofs, முழு* idx)
{
    முழு i, d = a.dims;
    எனில்( ofs > 0 )
    {
        ofs--;
        க்கு( i = d-1; i >= 0; i-- )
        {
            முழு sz = a.size[i];
            idx[i] = (முழு)(ofs % sz);
            ofs /= sz;
        }
    }
    இல்லையெனில்
    {
        க்கு( i = d-1; i >= 0; i-- )
            idx[i] = -1;
    }
}

#ifdef HAVE_OPENCL

#define MINMAX_STRUCT_ALIGNMENT 8 // sizeof double

template <typename T>
வெறுமை getMinMaxRes(மாறிலி Mat & db, இரட்டை * minVal, இரட்டை * maxVal,
                  முழு* minLoc, முழு* maxLoc,
                  முழு groupnum, முழு cols, இரட்டை * maxVal2)
{
    uint index_max = std::numeric_limits<uint>::max();
    T minval = std::numeric_limits<T>::max();
    T maxval = std::numeric_limits<T>::min() > 0 ? (T)-std::numeric_limits<T>::max() : std::numeric_limits<T>::min(), maxval2 = maxval;
    uint minloc = index_max, maxloc = index_max;

    size_t index = 0;
    மாறிலி T * minptr = வெற்று, * maxptr = வெற்று, * maxptr2 = வெற்று;
    மாறிலி uint * minlocptr = வெற்று, * maxlocptr = வெற்று;
    எனில் (minVal || minLoc)
    {
        minptr = db.ptr<T>();
        index += அளவு(T) * groupnum;
        index = alignSize(index, MINMAX_STRUCT_ALIGNMENT);
    }
    எனில் (maxVal || maxLoc)
    {
        maxptr = (மாறிலி T *)(db.ptr() + index);
        index += அளவு(T) * groupnum;
        index = alignSize(index, MINMAX_STRUCT_ALIGNMENT);
    }
    எனில் (minLoc)
    {
        minlocptr = (மாறிலி uint *)(db.ptr() + index);
        index += அளவு(uint) * groupnum;
        index = alignSize(index, MINMAX_STRUCT_ALIGNMENT);
    }
    எனில் (maxLoc)
    {
        maxlocptr = (மாறிலி uint *)(db.ptr() + index);
        index += அளவு(uint) * groupnum;
        index = alignSize(index, MINMAX_STRUCT_ALIGNMENT);
    }
    எனில் (maxVal2)
        maxptr2 = (மாறிலி T *)(db.ptr() + index);

    க்கு (முழு i = 0; i < groupnum; i++)
    {
        எனில் (minptr && minptr[i] <= minval)
        {
            எனில் (minptr[i] == minval)
            {
                எனில் (minlocptr)
                    minloc = std::min(minlocptr[i], minloc);
            }
            இல்லையெனில்
            {
                எனில் (minlocptr)
                    minloc = minlocptr[i];
                minval = minptr[i];
            }
        }
        எனில் (maxptr && maxptr[i] >= maxval)
        {
            எனில் (maxptr[i] == maxval)
            {
                எனில் (maxlocptr)
                    maxloc = std::min(maxlocptr[i], maxloc);
            }
            இல்லையெனில்
            {
                எனில் (maxlocptr)
                    maxloc = maxlocptr[i];
                maxval = maxptr[i];
            }
        }
        எனில் (maxptr2 && maxptr2[i] > maxval2)
            maxval2 = maxptr2[i];
    }
    தருக்க zero_mask = (minLoc && minloc == index_max) ||
    (maxLoc && maxloc == index_max);

    எனில் (minVal)
        *minVal = zero_mask ? 0 : (இரட்டை)minval;
    எனில் (maxVal)
        *maxVal = zero_mask ? 0 : (இரட்டை)maxval;
    எனில் (maxVal2)
        *maxVal2 = zero_mask ? 0 : (இரட்டை)maxval2;

    எனில் (minLoc)
    {
        minLoc[0] = zero_mask ? -1 : minloc / cols;
        minLoc[1] = zero_mask ? -1 : minloc % cols;
    }
    எனில் (maxLoc)
    {
        maxLoc[0] = zero_mask ? -1 : maxloc / cols;
        maxLoc[1] = zero_mask ? -1 : maxloc % cols;
    }
}

வகைவரை வெறுமை (*getMinMaxResFunc)(மாறிலி Mat & db, இரட்டை * minVal, இரட்டை * maxVal,
                                 முழு * minLoc, முழு *maxLoc, முழு gropunum, முழு cols, இரட்டை * maxVal2);

தருக்க ocl_minMaxIdx( உள்ளீடு_Array _src, இரட்டை* minVal, இரட்டை* maxVal, முழு* minLoc, முழு* maxLoc, உள்ளீடு_Array _mask,
                   முழு ddepth, தருக்க abs_மதிப்புகள், உள்ளீடு_Array _src2, இரட்டை * maxVal2)
{
    மாறிலி ocl::Device & dev = ocl::Device::getDefault();

#ifdef __ANDROID__
    எனில் (dev.isNVidia())
        திரும்பு பொய்;
#endif

    எனில் (dev.deviceVersionMajor() == 1 && dev.deviceVersionMinor() < 2)
    {
        // 'static' storage class specifier used by "minmaxloc" is available from OpenCL 1.2+ only
        திரும்பு பொய்;
    }

    தருக்க doubleSupport = dev.doubleFPConfig() > 0, haveMask = !_mask.empty(),
    haveSrc2 = _src2.kind() != உள்ளீடு_Array_2::NONE;
    முழு type = _src.type(), depth = CV_MAT_DEPTH(type), cn = CV_MAT_CN(type),
    kercn = haveMask ? cn : std::min(4, ocl::predictOptimalVectorWidth(_src, _src2));

    எனில் (depth >= CV_16F)
        திரும்பு பொய்;

    // disabled following modes since it occasionally fails on AMD devices (e.g. A10-6800K, sep. 2014)
    எனில் ((haveMask || type == CV_32FC1) && dev.isAMD())
        திரும்பு பொய்;

    CV_Assert( (cn == 1 && (!haveMask || _mask.type() == CV_8U || _mask.type() == CV_8S || _mask.type() == CV_Bool)) ||
              (cn >= 1 && !minLoc && !maxLoc) );

    எனில் (ddepth < 0)
        ddepth = depth;

    CV_Assert(!haveSrc2 || _src2.type() == type);

    எனில் (depth == CV_32S || depth == CV_8S || depth == CV_32U || depth == CV_64U ||
        depth == CV_64S || depth == CV_16F || depth == CV_16BF)
        திரும்பு பொய்;

    எனில் ((depth == CV_64F || ddepth == CV_64F) && !doubleSupport)
        திரும்பு பொய்;

    முழு groupnum = dev.maxComputeUnits();
    size_t wgs = dev.maxWorkGroupSize();

    முழு wgs2_aligned = 1;
    போது (wgs2_aligned < (முழு)wgs)
        wgs2_aligned <<= 1;
    wgs2_aligned >>= 1;

    தருக்க needMinVal = minVal || minLoc, needMinLoc = minLoc != வெற்று,
    needMaxVal = maxVal || maxLoc, needMaxLoc = maxLoc != வெற்று;

    // in case of mask we must know whether mask is filled with zeros or not
    // so let's calculate min or max location, if it's undefined, so mask is zeros
    எனில் (!(needMaxLoc || needMinLoc) && haveMask)
    {
        எனில் (needMinVal)
            needMinLoc = உண்மை;
        இல்லையெனில்
            needMaxLoc = உண்மை;
    }

    எழுத்து cvt[2][50];
    String opts = format("-D DEPTH_%d -D srcT1=%s%s -D WGS=%d -D srcT=%s"
                         " -D WGS2_ALIGNED=%d%s%s%s -D kercn=%d%s%s%s%s"
                         " -D dstT1=%s -D dstT=%s -D convertToDT=%s%s%s%s%s -D wdepth=%d -D convertFromU=%s"
                         " -D MINMAX_STRUCT_ALIGNMENT=%d",
                         depth, ocl::typeToStr(depth), haveMask ? " -D HAVE_MASK" : "", (முழு)wgs,
                         ocl::typeToStr(CV_MAKE_TYPE(depth, kercn)), wgs2_aligned,
                         doubleSupport ? " -D DOUBLE_SUPPORT" : "",
                         _src.isContinuous() ? " -D HAVE_SRC_CONT" : "",
                         _mask.isContinuous() ? " -D HAVE_MASK_CONT" : "", kercn,
                         needMinVal ? " -D NEED_MINVAL" : "", needMaxVal ? " -D NEED_MAXVAL" : "",
                         needMinLoc ? " -D NEED_MINLOC" : "", needMaxLoc ? " -D NEED_MAXLOC" : "",
                         ocl::typeToStr(ddepth), ocl::typeToStr(CV_MAKE_TYPE(ddepth, kercn)),
                         ocl::convertTypeStr(depth, ddepth, kercn, cvt[0], அளவு(cvt[0])),
                         abs_மதிப்புகள் ? " -D OP_ABS" : "",
                         haveSrc2 ? " -D HAVE_SRC2" : "", maxVal2 ? " -D OP_CALC2" : "",
                         haveSrc2 && _src2.isContinuous() ? " -D HAVE_SRC2_CONT" : "", ddepth,
                         depth <= CV_32S && ddepth == CV_32S ? ocl::convertTypeStr(CV_8U, ddepth, kercn, cvt[1], அளவு(cvt[1])) : "noconvert",
                         MINMAX_STRUCT_ALIGNMENT);

    ocl::Kernel k("minmaxloc", ocl::core::minmaxloc_oclsrc, opts);
    எனில் (k.empty())
        திரும்பு பொய்;

    முழு esz = CV_ELEM_SIZE(ddepth), esz32s = CV_ELEM_SIZE1(CV_32S),
    dbsize = groupnum * ((needMinVal ? esz : 0) + (needMaxVal ? esz : 0) +
                         (needMinLoc ? esz32s : 0) + (needMaxLoc ? esz32s : 0) +
                         (maxVal2 ? esz : 0))
    + 5 * MINMAX_STRUCT_ALIGNMENT;
    UMat src = _src.getUMat(), src2 = _src2.getUMat(), db(1, dbsize, CV_8UC1), mask = _mask.getUMat();

    எனில் (cn > 1 && !haveMask)
    {
        src = src.reshape(1);
        src2 = src2.reshape(1);
    }

    எனில் (haveSrc2)
    {
        எனில் (!haveMask)
            k.args(ocl::KernelArg::ReadOnlyNoSize(src), src.cols, (முழு)src.total(),
                   groupnum, ocl::KernelArg::PtrWriteOnly(db), ocl::KernelArg::ReadOnlyNoSize(src2));
        இல்லையெனில்
            k.args(ocl::KernelArg::ReadOnlyNoSize(src), src.cols, (முழு)src.total(),
                   groupnum, ocl::KernelArg::PtrWriteOnly(db), ocl::KernelArg::ReadOnlyNoSize(mask),
                   ocl::KernelArg::ReadOnlyNoSize(src2));
    }
    இல்லையெனில்
    {
        எனில் (!haveMask)
            k.args(ocl::KernelArg::ReadOnlyNoSize(src), src.cols, (முழு)src.total(),
                   groupnum, ocl::KernelArg::PtrWriteOnly(db));
        இல்லையெனில்
            k.args(ocl::KernelArg::ReadOnlyNoSize(src), src.cols, (முழு)src.total(),
                   groupnum, ocl::KernelArg::PtrWriteOnly(db), ocl::KernelArg::ReadOnlyNoSize(mask));
    }

    size_t globalsize = groupnum * wgs;
    எனில் (!k.run(1, &globalsize, &wgs, உண்மை))
        திரும்பு பொய்;

    நிலையான மாறிலி getMinMaxResFunc functab[7] =
    {
        getMinMaxRes<uchar>,
        getMinMaxRes<எழுத்து>,
        getMinMaxRes<ushort>,
        getMinMaxRes<short>,
        getMinMaxRes<முழு>,
        getMinMaxRes<தசம>,
        getMinMaxRes<இரட்டை>
    };

    CV_Assert(ddepth <= CV_64F);
    getMinMaxResFunc func = functab[ddepth];

    முழு locTemp[2];
    func(db.getMat(ACCESS_READ), minVal, maxVal,
         needMinLoc ? minLoc ? minLoc : locTemp : minLoc,
         needMaxLoc ? maxLoc ? maxLoc : locTemp : maxLoc,
         groupnum, src.cols, maxVal2);

    திரும்பு உண்மை;
}

#endif

}

// serial implementation, also used for the pieces of the parallel version
நிலையான வெறுமை minMaxIdx_(cv::உள்ளீடு_Array _src, இரட்டை* minVal,
                       இரட்டை* maxVal, முழு* minIdx, முழு* maxIdx,
                       cv::உள்ளீடு_Array _mask)
{
    using நாமாகாஶ cv;
    முழு type = _src.type(), depth = CV_MAT_DEPTH(type), cn = CV_MAT_CN(type);
    CV_Assert( (cn == 1 && (_mask.empty() || _mask.type() == CV_8U || _mask.type() == CV_8S || _mask.type() == CV_Bool)) ||
               (cn > 1 && _mask.empty() && !minIdx && !maxIdx) );

    CV_OCL_RUN(OCL_PERFORMANCE_CHECK(_src.isUMat()) && _src.dims() <= 2  && (_mask.empty() || _src.size() == _mask.size()),
               ocl_minMaxIdx(_src, minVal, maxVal, minIdx, maxIdx, _mask))

    Mat src = _src.getMat(), mask = _mask.getMat();

    எனில் (src.dims <= 2)
    {
        எனில் ((size_t)src.step == (size_t)mask.step || mask.empty())
        {
            CALL_HAL(minMaxIdx, cv_hal_minMaxIdx, src.data, src.step, src.cols*cn, src.rows,
                     src.depth(), minVal, maxVal, minIdx, maxIdx, mask.data);
        }
        CALL_HAL(minMaxIdxMaskStep, cv_hal_minMaxIdxMaskStep, src.data, src.step, src.cols*cn, src.rows,
                 src.depth(), minVal, maxVal, minIdx, maxIdx, mask.data, mask.step);
    }
    இல்லையெனில் எனில் (src.isContinuous() && (mask.isContinuous() || mask.empty()))
    {
        முழு res = cv_hal_minMaxIdx(src.data, 0, (முழு)src.total()*cn, 1, src.depth(),
                                   minVal, maxVal, minIdx, maxIdx, mask.data);

        எனில் (res == CV_HAL_ERROR_OK)
        {
            // minIdx[0] and minIdx[0] are always 0 for "flatten" version
            எனில் (minIdx)
                ofs2idx(src, minIdx[1]+1, minIdx);
            எனில் (maxIdx)
                ofs2idx(src, maxIdx[1]+1, maxIdx);
            திரும்பு;
        }
        இல்லையெனில் எனில் (res != CV_HAL_ERROR_NOT_IMPLEMENTED)
        {
            CV_Error_(cv::Error::StsInternal,
            ("HAL implementation minMaxIdx ==> " CVAUX_STR(cv_hal_minMaxIdx) " returned %d (0x%08x)", res, res));
        }
    }

    MinMaxIdxFunc func = getMinMaxIdxFunc(depth);
    CV_Assert( func != 0 );

    மாறிலி Mat* arrays[] = {&src, &mask, 0};
    uchar* ptrs[2] = {};
    NAryMatIterator it(arrays, ptrs);

    size_t minidx = 0, maxidx = 0;
    size_t startidx = 1;
    ஒன்றிப்பு {
        முழு i;
        தசம f;
        இரட்டை d;
        int64 L;
        uint64 UL;
    } minval, maxval;
    முழு planeSize = (முழு)it.size*cn;
    minval.L = maxval.L = 0;

    க்கு( size_t i = 0; i < it.nplanes; i++, ++it, startidx += planeSize )
        func( ptrs[0], ptrs[1], &minval.L, &maxval.L, &minidx, &maxidx, planeSize, startidx );

    இரட்டை dminval, dmaxval;
    எனில்( depth <= CV_32S || depth == CV_Bool )
        dminval = minval.i, dmaxval = maxval.i;
    இல்லையெனில் எனில்( depth == CV_32F || depth == CV_16F || depth == CV_16BF )
        dminval = minval.f, dmaxval = maxval.f;
    இல்லையெனில் எனில்( depth == CV_64F )
        dminval = minval.d, dmaxval = maxval.d;
    இல்லையெனில் எனில்( depth == CV_64S || depth == CV_32U )
        dminval = (இரட்டை)minval.L, dmaxval = (இரட்டை)maxval.L;
    இல்லையெனில் {
        CV_Assert(depth == CV_64U);
        dminval = (இரட்டை)minval.UL, dmaxval = (இரட்டை)maxval.UL;
    }

    எனில்( minVal )
        *minVal = dminval;
    எனில்( maxVal )
        *maxVal = dmaxval;

    எனில்( minIdx )
        ofs2idx(src, minidx, minIdx);
    எனில்( maxIdx )
        ofs2idx(src, maxidx, maxIdx);
}

வெறுமை cv::minMaxIdx(உள்ளீடு_Array _src, இரட்டை* minVal,
                   இரட்டை* maxVal, முழு* minIdx, முழு* maxIdx,
                   உள்ளீடு_Array _mask)
{
    CV_INSTRUMENT_REGION();

    std::vector<StatChunk> chunks;
    எனில் (!_src.isUMat())
    {
        Mat src = _src.getMat(), mask = _mask.getMat();
        மாறிலி முழு cn = src.channels();
        மாறிலி தருக்க argsOk = (cn == 1 && (mask.empty() || ((mask.type() == CV_8U || mask.type() == CV_8S ||
                             mask.type() == CV_Bool) && src.size == mask.size))) ||
                            (cn > 1 && mask.empty() && !minIdx && !maxIdx);
        எனில் (argsOk && splitForParallelStat(src, mask, chunks))
        {
            மாறிலி முழு n = (முழு)chunks.size();
            std::vector<இரட்டை> mn(n), mx(n);
            std::vector<size_t> imn(n, 0), imx(n, 0);   // 1-based row-major indices, 0 = no element
            parallel_for_(Range(0, n), [&](மாறிலி Range& r)
            {
                க்கு (முழு i = r.start; i < r.end; i++)
                {
                    முழு a[2] = {-1, -1}, b[2] = {-1, -1};
                    minMaxIdx_(chunks[i].src, &mn[i], &mx[i], cn == 1 ? a : nullptr, cn == 1 ? b : nullptr,
                               chunks[i].mask);
                    எனில் (cn > 1)
                        imn[i] = imx[i] = 1;
                    இல்லையெனில் எனில் (a[0] >= 0)
                    {
                        imn[i] = chunkIndexToElem(chunks[i], a) + 1;
                        imx[i] = chunkIndexToElem(chunks[i], b) + 1;
                    }
                }
            });
            // pieces are in memory order, so taking the first strict improvement keeps the
            // first occurrence, as in the serial version
            முழு bmin = -1, bmax = -1;
            க்கு (முழு i = 0; i < n; i++)
            {
                எனில் (imn[i] == 0)
                    தொடர்;
                எனில் (bmin < 0 || mn[i] < mn[bmin]) bmin = i;
                எனில் (bmax < 0 || mx[i] > mx[bmax]) bmax = i;
            }
            எனில் (minVal) *minVal = bmin >= 0 ? mn[bmin] : 0;
            எனில் (maxVal) *maxVal = bmax >= 0 ? mx[bmax] : 0;
            எனில் (minIdx) ofs2idx(src, bmin >= 0 ? imn[bmin] : 0, minIdx);
            எனில் (maxIdx) ofs2idx(src, bmax >= 0 ? imx[bmax] : 0, maxIdx);
            திரும்பு;
        }
    }
    minMaxIdx_(_src, minVal, maxVal, minIdx, maxIdx, _mask);
}

வெறுமை cv::minMaxLoc( உள்ளீடு_Array _img, இரட்டை* minVal, இரட்டை* maxVal,
                    Point* minLoc, Point* maxLoc, உள்ளீடு_Array mask )
{
    CV_INSTRUMENT_REGION();

    முழு dims = _img.dims();
    CV_CheckLE(dims, 2, "");

    minMaxIdx(_img, minVal, maxVal, (முழு*)minLoc, (முழு*)maxLoc, mask);
    எனில்( minLoc) {
        எனில் (dims == 2)
            std::swap(minLoc->x, minLoc->y);
        இல்லையெனில் {
            minLoc->y = 0;
        }
    }
    எனில்( maxLoc) {
        எனில் (dims == 2)
            std::swap(maxLoc->x, maxLoc->y);
        இல்லையெனில் {
            maxLoc->y = 0;
        }
    }
}

எண்ணிக்கை வகுப்பு ReduceMode
{
    FIRST_MIN = 0, //!< get index of first min occurrence
    LAST_MIN  = 1, //!< get index of last min occurrence
    FIRST_MAX = 2, //!< get index of first max occurrence
    LAST_MAX  = 3, //!< get index of last max occurrence
};

template <typename T>
கட்டமைப்பு reduceMinMaxImpl
{
    வெறுமை operator()(மாறிலி cv::Mat& src, cv::Mat& dst, ReduceMode mode, மாறிலி முழு axis) மாறிலி
    {
        தேர்வு(mode)
        {
        நிலை ReduceMode::FIRST_MIN:
            reduceMinMaxApply<std::less>(src, dst, axis);
            நிறுத்து;
        நிலை ReduceMode::LAST_MIN:
            reduceMinMaxApply<std::less_equal>(src, dst, axis);
            நிறுத்து;
        நிலை ReduceMode::FIRST_MAX:
            reduceMinMaxApply<std::greater>(src, dst, axis);
            நிறுத்து;
        நிலை ReduceMode::LAST_MAX:
            reduceMinMaxApply<std::greater_equal>(src, dst, axis);
            நிறுத்து;
        }
    }

    template <template<வகுப்பு> வகுப்பு Cmp>
    நிலையான வெறுமை reduceMinMaxApply(மாறிலி cv::Mat& src, cv::Mat& dst, மாறிலி முழு axis)
    {
        Cmp<T> cmp;

        மாறிலி auto *src_ptr = src.ptr<T>();
        auto *dst_ptr = dst.ptr<int32_t>();

        மாறிலி size_t outer_size = src.total(0, axis);
        மாறிலி auto mid_size = static_cast<size_t>(src.size[axis]);

        மாறிலி size_t outer_step = src.total(axis);
        மாறிலி size_t dst_step = dst.total(axis);

        மாறிலி size_t mid_step = src.total(axis + 1);

        க்கு (size_t outer = 0; outer < outer_size; ++outer)
        {
            மாறிலி size_t outer_offset = outer * outer_step;
            மாறிலி size_t dst_offset = outer * dst_step;
            க்கு (size_t mid = 0; mid != mid_size; ++mid)
            {
                மாறிலி size_t src_offset = outer_offset + mid * mid_step;
                க்கு (size_t inner = 0; inner < mid_step; inner++)
                {
                    int32_t& index = dst_ptr[dst_offset + inner];

                    மாறிலி size_t prev = outer_offset + index * mid_step + inner;
                    மாறிலி size_t curr = src_offset + inner;

                    எனில் (cmp(src_ptr[curr], src_ptr[prev]))
                    {
                        index = static_cast<int32_t>(mid);
                    }
                }
            }
        }
    }
};

நிலையான வெறுமை reduceMinMax(cv::உள்ளீடு_Array src, cv::OutputArray dst, ReduceMode mode, முழு axis)
{
    CV_INSTRUMENT_REGION();

    cv::Mat srcMat = src.getMat();
    முழு dims = std::max(1, srcMat.dims);
    axis = (axis + dims) % dims;
    CV_Assert(srcMat.channels() == 1 && axis >= 0 && axis <= srcMat.dims);

    std::vector<முழு> sizes(srcMat.dims);
    std::copy(srcMat.size.p, srcMat.size.p + srcMat.dims, sizes.begin());
    எனில்(!sizes.empty())
        sizes[axis] = 1;

    dst.create(srcMat.dims, sizes.data(), CV_32SC1); // indices
    cv::Mat dstMat = dst.getMat();
    dstMat.setTo(cv::Scalar::all(0));

    எனில் (!srcMat.isContinuous())
    {
        srcMat = srcMat.clone();
    }

    தருக்க needs_copy = !dstMat.isContinuous();
    எனில் (needs_copy)
    {
        dstMat = dstMat.clone();
    }

    cv::detail::depthDispatch<reduceMinMaxImpl>(srcMat.depth(), srcMat, dstMat, mode, axis);

    எனில் (needs_copy)
    {
        dstMat.copyTo(dst);
    }
}

வெறுமை cv::reduceArgMin(உள்ளீடு_Array src, OutputArray dst, முழு axis, தருக்க lastIndex)
{
    reduceMinMax(src, dst, lastIndex ? ReduceMode::LAST_MIN : ReduceMode::FIRST_MIN, axis);
}

வெறுமை cv::reduceArgMax(உள்ளீடு_Array src, OutputArray dst, முழு axis, தருக்க lastIndex)
{
    reduceMinMax(src, dst, lastIndex ? ReduceMode::LAST_MAX : ReduceMode::FIRST_MAX, axis);
}
