// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


#ضمن "precomp.hpp"
#ضمن "opencl_kernels_core.hpp"
#ضمن "stat.hpp"
#ضمن "stat_parallel.hpp"
#ضمن "opencv2/core/detail/dispatch_helper.impl.hpp"
#ضمن <algorithm>

#ضمن "minmax.simd.hpp"
#ضمن "minmax.simd_declarations.hpp" // defines CV_CPU_DISPATCH_MODES_ALL=AVX2,...,BASELINE based on CMakeLists.txt content

نطاق cv {

ساكن MinMaxIdxFunc getMinMaxIdxFunc(صحيح depth)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getMinMaxIdxFunc, (depth),
                    CV_CPU_DISPATCH_MODES_ALL);
}

// The function expects 1-based indexing for ofs
// Zero is treated as invalid offset (not found)
ساكن فراغ ofs2idx(ثابت Mat& a, size_t ofs, صحيح* idx)
{
    صحيح i, d = a.dims;
    إذا( ofs > 0 )
    {
        ofs--;
        لكل( i = d-1; i >= 0; i-- )
        {
            صحيح sz = a.size[i];
            idx[i] = (صحيح)(ofs % sz);
            ofs /= sz;
        }
    }
    وإلا
    {
        لكل( i = d-1; i >= 0; i-- )
            idx[i] = -1;
    }
}

#ifdef HAVE_OPENCL

#define MINMAX_STRUCT_ALIGNMENT 8 // sizeof double

template <typename T>
فراغ getMinMaxRes(ثابت Mat & db, مزدوج * minVal, مزدوج * maxVal,
                  صحيح* minLoc, صحيح* maxLoc,
                  صحيح groupnum, صحيح cols, مزدوج * maxVal2)
{
    uint index_max = std::numeric_limits<uint>::max();
    T minval = std::numeric_limits<T>::max();
    T maxval = std::numeric_limits<T>::min() > 0 ? (T)-std::numeric_limits<T>::max() : std::numeric_limits<T>::min(), maxval2 = maxval;
    uint minloc = index_max, maxloc = index_max;

    size_t index = 0;
    ثابت T * minptr = عدم, * maxptr = عدم, * maxptr2 = عدم;
    ثابت uint * minlocptr = عدم, * maxlocptr = عدم;
    إذا (minVal || minLoc)
    {
        minptr = db.ptr<T>();
        index += حجم(T) * groupnum;
        index = alignSize(index, MINMAX_STRUCT_ALIGNMENT);
    }
    إذا (maxVal || maxLoc)
    {
        maxptr = (ثابت T *)(db.ptr() + index);
        index += حجم(T) * groupnum;
        index = alignSize(index, MINMAX_STRUCT_ALIGNMENT);
    }
    إذا (minLoc)
    {
        minlocptr = (ثابت uint *)(db.ptr() + index);
        index += حجم(uint) * groupnum;
        index = alignSize(index, MINMAX_STRUCT_ALIGNMENT);
    }
    إذا (maxLoc)
    {
        maxlocptr = (ثابت uint *)(db.ptr() + index);
        index += حجم(uint) * groupnum;
        index = alignSize(index, MINMAX_STRUCT_ALIGNMENT);
    }
    إذا (maxVal2)
        maxptr2 = (ثابت T *)(db.ptr() + index);

    لكل (صحيح i = 0; i < groupnum; i++)
    {
        إذا (minptr && minptr[i] <= minval)
        {
            إذا (minptr[i] == minval)
            {
                إذا (minlocptr)
                    minloc = std::min(minlocptr[i], minloc);
            }
            وإلا
            {
                إذا (minlocptr)
                    minloc = minlocptr[i];
                minval = minptr[i];
            }
        }
        إذا (maxptr && maxptr[i] >= maxval)
        {
            إذا (maxptr[i] == maxval)
            {
                إذا (maxlocptr)
                    maxloc = std::min(maxlocptr[i], maxloc);
            }
            وإلا
            {
                إذا (maxlocptr)
                    maxloc = maxlocptr[i];
                maxval = maxptr[i];
            }
        }
        إذا (maxptr2 && maxptr2[i] > maxval2)
            maxval2 = maxptr2[i];
    }
    منطقي zero_mask = (minLoc && minloc == index_max) ||
    (maxLoc && maxloc == index_max);

    إذا (minVal)
        *minVal = zero_mask ? 0 : (مزدوج)minval;
    إذا (maxVal)
        *maxVal = zero_mask ? 0 : (مزدوج)maxval;
    إذا (maxVal2)
        *maxVal2 = zero_mask ? 0 : (مزدوج)maxval2;

    إذا (minLoc)
    {
        minLoc[0] = zero_mask ? -1 : minloc / cols;
        minLoc[1] = zero_mask ? -1 : minloc % cols;
    }
    إذا (maxLoc)
    {
        maxLoc[0] = zero_mask ? -1 : maxloc / cols;
        maxLoc[1] = zero_mask ? -1 : maxloc % cols;
    }
}

تعريفنوع فراغ (*getMinMaxResFunc)(ثابت Mat & db, مزدوج * minVal, مزدوج * maxVal,
                                 صحيح * minLoc, صحيح *maxLoc, صحيح gropunum, صحيح cols, مزدوج * maxVal2);

منطقي ocl_minMaxIdx( إدخال_Array _src, مزدوج* minVal, مزدوج* maxVal, صحيح* minLoc, صحيح* maxLoc, إدخال_Array _mask,
                   صحيح ddepth, منطقي abs_قيم, إدخال_Array _src2, مزدوج * maxVal2)
{
    ثابت ocl::Device & dev = ocl::Device::getDefault();

#ifdef __ANDROID__
    إذا (dev.isNVidia())
        إرجاع خطأ;
#endif

    إذا (dev.deviceVersionMajor() == 1 && dev.deviceVersionMinor() < 2)
    {
        // 'static' storage class specifier used by "minmaxloc" is available from OpenCL 1.2+ only
        إرجاع خطأ;
    }

    منطقي doubleSupport = dev.doubleFPConfig() > 0, haveMask = !_mask.empty(),
    haveSrc2 = _src2.kind() != إدخال_Array_2::NONE;
    صحيح type = _src.type(), depth = CV_MAT_DEPTH(type), cn = CV_MAT_CN(type),
    kercn = haveMask ? cn : std::min(4, ocl::predictOptimalVectorWidth(_src, _src2));

    إذا (depth >= CV_16F)
        إرجاع خطأ;

    // disabled following modes since it occasionally fails on AMD devices (e.g. A10-6800K, sep. 2014)
    إذا ((haveMask || type == CV_32FC1) && dev.isAMD())
        إرجاع خطأ;

    CV_Assert( (cn == 1 && (!haveMask || _mask.type() == CV_8U || _mask.type() == CV_8S || _mask.type() == CV_Bool)) ||
              (cn >= 1 && !minLoc && !maxLoc) );

    إذا (ddepth < 0)
        ddepth = depth;

    CV_Assert(!haveSrc2 || _src2.type() == type);

    إذا (depth == CV_32S || depth == CV_8S || depth == CV_32U || depth == CV_64U ||
        depth == CV_64S || depth == CV_16F || depth == CV_16BF)
        إرجاع خطأ;

    إذا ((depth == CV_64F || ddepth == CV_64F) && !doubleSupport)
        إرجاع خطأ;

    صحيح groupnum = dev.maxComputeUnits();
    size_t wgs = dev.maxWorkGroupSize();

    صحيح wgs2_aligned = 1;
    بينما (wgs2_aligned < (صحيح)wgs)
        wgs2_aligned <<= 1;
    wgs2_aligned >>= 1;

    منطقي needMinVal = minVal || minLoc, needMinLoc = minLoc != عدم,
    needMaxVal = maxVal || maxLoc, needMaxLoc = maxLoc != عدم;

    // in case of mask we must know whether mask is filled with zeros or not
    // so let's calculate min or max location, if it's undefined, so mask is zeros
    إذا (!(needMaxLoc || needMinLoc) && haveMask)
    {
        إذا (needMinVal)
            needMinLoc = true;
        وإلا
            needMaxLoc = true;
    }

    حرف cvt[2][50];
    String opts = format("-D DEPTH_%d -D srcT1=%s%s -D WGS=%d -D srcT=%s"
                         " -D WGS2_ALIGNED=%d%s%s%s -D kercn=%d%s%s%s%s"
                         " -D dstT1=%s -D dstT=%s -D convertToDT=%s%s%s%s%s -D wdepth=%d -D convertFromU=%s"
                         " -D MINMAX_STRUCT_ALIGNMENT=%d",
                         depth, ocl::typeToStr(depth), haveMask ? " -D HAVE_MASK" : "", (صحيح)wgs,
                         ocl::typeToStr(CV_MAKE_TYPE(depth, kercn)), wgs2_aligned,
                         doubleSupport ? " -D DOUBLE_SUPPORT" : "",
                         _src.isContinuous() ? " -D HAVE_SRC_CONT" : "",
                         _mask.isContinuous() ? " -D HAVE_MASK_CONT" : "", kercn,
                         needMinVal ? " -D NEED_MINVAL" : "", needMaxVal ? " -D NEED_MAXVAL" : "",
                         needMinLoc ? " -D NEED_MINLOC" : "", needMaxLoc ? " -D NEED_MAXLOC" : "",
                         ocl::typeToStr(ddepth), ocl::typeToStr(CV_MAKE_TYPE(ddepth, kercn)),
                         ocl::convertTypeStr(depth, ddepth, kercn, cvt[0], حجم(cvt[0])),
                         abs_قيم ? " -D OP_ABS" : "",
                         haveSrc2 ? " -D HAVE_SRC2" : "", maxVal2 ? " -D OP_CALC2" : "",
                         haveSrc2 && _src2.isContinuous() ? " -D HAVE_SRC2_CONT" : "", ddepth,
                         depth <= CV_32S && ddepth == CV_32S ? ocl::convertTypeStr(CV_8U, ddepth, kercn, cvt[1], حجم(cvt[1])) : "noconvert",
                         MINMAX_STRUCT_ALIGNMENT);

    ocl::Kernel k("minmaxloc", ocl::core::minmaxloc_oclsrc, opts);
    إذا (k.empty())
        إرجاع خطأ;

    صحيح esz = CV_ELEM_SIZE(ddepth), esz32s = CV_ELEM_SIZE1(CV_32S),
    dbsize = groupnum * ((needMinVal ? esz : 0) + (needMaxVal ? esz : 0) +
                         (needMinLoc ? esz32s : 0) + (needMaxLoc ? esz32s : 0) +
                         (maxVal2 ? esz : 0))
    + 5 * MINMAX_STRUCT_ALIGNMENT;
    UMat src = _src.getUMat(), src2 = _src2.getUMat(), db(1, dbsize, CV_8UC1), mask = _mask.getUMat();

    إذا (cn > 1 && !haveMask)
    {
        src = src.reshape(1);
        src2 = src2.reshape(1);
    }

    إذا (haveSrc2)
    {
        إذا (!haveMask)
            k.args(ocl::KernelArg::ReadOnlyNoSize(src), src.cols, (صحيح)src.total(),
                   groupnum, ocl::KernelArg::PtrWriteOnly(db), ocl::KernelArg::ReadOnlyNoSize(src2));
        وإلا
            k.args(ocl::KernelArg::ReadOnlyNoSize(src), src.cols, (صحيح)src.total(),
                   groupnum, ocl::KernelArg::PtrWriteOnly(db), ocl::KernelArg::ReadOnlyNoSize(mask),
                   ocl::KernelArg::ReadOnlyNoSize(src2));
    }
    وإلا
    {
        إذا (!haveMask)
            k.args(ocl::KernelArg::ReadOnlyNoSize(src), src.cols, (صحيح)src.total(),
                   groupnum, ocl::KernelArg::PtrWriteOnly(db));
        وإلا
            k.args(ocl::KernelArg::ReadOnlyNoSize(src), src.cols, (صحيح)src.total(),
                   groupnum, ocl::KernelArg::PtrWriteOnly(db), ocl::KernelArg::ReadOnlyNoSize(mask));
    }

    size_t globalsize = groupnum * wgs;
    إذا (!k.run(1, &globalsize, &wgs, true))
        إرجاع خطأ;

    ساكن ثابت getMinMaxResFunc functab[7] =
    {
        getMinMaxRes<uchar>,
        getMinMaxRes<حرف>,
        getMinMaxRes<ushort>,
        getMinMaxRes<short>,
        getMinMaxRes<صحيح>,
        getMinMaxRes<عائم>,
        getMinMaxRes<مزدوج>
    };

    CV_Assert(ddepth <= CV_64F);
    getMinMaxResFunc func = functab[ddepth];

    صحيح locTemp[2];
    func(db.getMat(ACCESS_READ), minVal, maxVal,
         needMinLoc ? minLoc ? minLoc : locTemp : minLoc,
         needMaxLoc ? maxLoc ? maxLoc : locTemp : maxLoc,
         groupnum, src.cols, maxVal2);

    إرجاع true;
}

#endif

}

// serial implementation, also used for the pieces of the parallel version
ساكن فراغ minMaxIdx_(cv::إدخال_Array _src, مزدوج* minVal,
                       مزدوج* maxVal, صحيح* minIdx, صحيح* maxIdx,
                       cv::إدخال_Array _mask)
{
    using نطاق cv;
    صحيح type = _src.type(), depth = CV_MAT_DEPTH(type), cn = CV_MAT_CN(type);
    CV_Assert( (cn == 1 && (_mask.empty() || _mask.type() == CV_8U || _mask.type() == CV_8S || _mask.type() == CV_Bool)) ||
               (cn > 1 && _mask.empty() && !minIdx && !maxIdx) );

    CV_OCL_RUN(OCL_PERFORMANCE_CHECK(_src.isUMat()) && _src.dims() <= 2  && (_mask.empty() || _src.size() == _mask.size()),
               ocl_minMaxIdx(_src, minVal, maxVal, minIdx, maxIdx, _mask))

    Mat src = _src.getMat(), mask = _mask.getMat();

    إذا (src.dims <= 2)
    {
        إذا ((size_t)src.step == (size_t)mask.step || mask.empty())
        {
            CALL_HAL(minMaxIdx, cv_hal_minMaxIdx, src.data, src.step, src.cols*cn, src.rows,
                     src.depth(), minVal, maxVal, minIdx, maxIdx, mask.data);
        }
        CALL_HAL(minMaxIdxMaskStep, cv_hal_minMaxIdxMaskStep, src.data, src.step, src.cols*cn, src.rows,
                 src.depth(), minVal, maxVal, minIdx, maxIdx, mask.data, mask.step);
    }
    وإلا إذا (src.isContinuous() && (mask.isContinuous() || mask.empty()))
    {
        صحيح res = cv_hal_minMaxIdx(src.data, 0, (صحيح)src.total()*cn, 1, src.depth(),
                                   minVal, maxVal, minIdx, maxIdx, mask.data);

        إذا (res == CV_HAL_ERROR_OK)
        {
            // minIdx[0] and minIdx[0] are always 0 for "flatten" version
            إذا (minIdx)
                ofs2idx(src, minIdx[1]+1, minIdx);
            إذا (maxIdx)
                ofs2idx(src, maxIdx[1]+1, maxIdx);
            إرجاع;
        }
        وإلا إذا (res != CV_HAL_ERROR_NOT_IMPLEMENTED)
        {
            CV_Error_(cv::Error::StsInternal,
            ("HAL implementation minMaxIdx ==> " CVAUX_STR(cv_hal_minMaxIdx) " returned %d (0x%08x)", res, res));
        }
    }

    MinMaxIdxFunc func = getMinMaxIdxFunc(depth);
    CV_Assert( func != 0 );

    ثابت Mat* arrays[] = {&src, &mask, 0};
    uchar* ptrs[2] = {};
    NAryMatIterator it(arrays, ptrs);

    size_t minidx = 0, maxidx = 0;
    size_t startidx = 1;
    اتحاد {
        صحيح i;
        عائم f;
        مزدوج d;
        int64 L;
        uint64 UL;
    } minval, maxval;
    صحيح planeSize = (صحيح)it.size*cn;
    minval.L = maxval.L = 0;

    لكل( size_t i = 0; i < it.nplanes; i++, ++it, startidx += planeSize )
        func( ptrs[0], ptrs[1], &minval.L, &maxval.L, &minidx, &maxidx, planeSize, startidx );

    مزدوج dminval, dmaxval;
    إذا( depth <= CV_32S || depth == CV_Bool )
        dminval = minval.i, dmaxval = maxval.i;
    وإلا إذا( depth == CV_32F || depth == CV_16F || depth == CV_16BF )
        dminval = minval.f, dmaxval = maxval.f;
    وإلا إذا( depth == CV_64F )
        dminval = minval.d, dmaxval = maxval.d;
    وإلا إذا( depth == CV_64S || depth == CV_32U )
        dminval = (مزدوج)minval.L, dmaxval = (مزدوج)maxval.L;
    وإلا {
        CV_Assert(depth == CV_64U);
        dminval = (مزدوج)minval.UL, dmaxval = (مزدوج)maxval.UL;
    }

    إذا( minVal )
        *minVal = dminval;
    إذا( maxVal )
        *maxVal = dmaxval;

    إذا( minIdx )
        ofs2idx(src, minidx, minIdx);
    إذا( maxIdx )
        ofs2idx(src, maxidx, maxIdx);
}

فراغ cv::minMaxIdx(إدخال_Array _src, مزدوج* minVal,
                   مزدوج* maxVal, صحيح* minIdx, صحيح* maxIdx,
                   إدخال_Array _mask)
{
    CV_INSTRUMENT_REGION();

    std::vector<StatChunk> chunks;
    إذا (!_src.isUMat())
    {
        Mat src = _src.getMat(), mask = _mask.getMat();
        ثابت صحيح cn = src.channels();
        ثابت منطقي argsOk = (cn == 1 && (mask.empty() || ((mask.type() == CV_8U || mask.type() == CV_8S ||
                             mask.type() == CV_Bool) && src.size == mask.size))) ||
                            (cn > 1 && mask.empty() && !minIdx && !maxIdx);
        إذا (argsOk && splitForParallelStat(src, mask, chunks))
        {
            ثابت صحيح n = (صحيح)chunks.size();
            std::vector<مزدوج> mn(n), mx(n);
            std::vector<size_t> imn(n, 0), imx(n, 0);   // 1-based row-major indices, 0 = no element
            parallel_for_(Range(0, n), [&](ثابت Range& r)
            {
                لكل (صحيح i = r.start; i < r.end; i++)
                {
                    صحيح a[2] = {-1, -1}, b[2] = {-1, -1};
                    minMaxIdx_(chunks[i].src, &mn[i], &mx[i], cn == 1 ? a : nullptr, cn == 1 ? b : nullptr,
                               chunks[i].mask);
                    إذا (cn > 1)
                        imn[i] = imx[i] = 1;
                    وإلا إذا (a[0] >= 0)
                    {
                        imn[i] = chunkIndexToElem(chunks[i], a) + 1;
                        imx[i] = chunkIndexToElem(chunks[i], b) + 1;
                    }
                }
            });
            // pieces are in memory order, so taking the first strict improvement keeps the
            // first occurrence, as in the serial version
            صحيح bmin = -1, bmax = -1;
            لكل (صحيح i = 0; i < n; i++)
            {
                إذا (imn[i] == 0)
                    استمر;
                إذا (bmin < 0 || mn[i] < mn[bmin]) bmin = i;
                إذا (bmax < 0 || mx[i] > mx[bmax]) bmax = i;
            }
            إذا (minVal) *minVal = bmin >= 0 ? mn[bmin] : 0;
            إذا (maxVal) *maxVal = bmax >= 0 ? mx[bmax] : 0;
            إذا (minIdx) ofs2idx(src, bmin >= 0 ? imn[bmin] : 0, minIdx);
            إذا (maxIdx) ofs2idx(src, bmax >= 0 ? imx[bmax] : 0, maxIdx);
            إرجاع;
        }
    }
    minMaxIdx_(_src, minVal, maxVal, minIdx, maxIdx, _mask);
}

فراغ cv::minMaxLoc( إدخال_Array _img, مزدوج* minVal, مزدوج* maxVal,
                    Point* minLoc, Point* maxLoc, إدخال_Array mask )
{
    CV_INSTRUMENT_REGION();

    صحيح dims = _img.dims();
    CV_CheckLE(dims, 2, "");

    minMaxIdx(_img, minVal, maxVal, (صحيح*)minLoc, (صحيح*)maxLoc, mask);
    إذا( minLoc) {
        إذا (dims == 2)
            std::swap(minLoc->x, minLoc->y);
        وإلا {
            minLoc->y = 0;
        }
    }
    إذا( maxLoc) {
        إذا (dims == 2)
            std::swap(maxLoc->x, maxLoc->y);
        وإلا {
            maxLoc->y = 0;
        }
    }
}

تعداد صنف ReduceMode
{
    FIRST_MIN = 0, //!< get index of first min occurrence
    LAST_MIN  = 1, //!< get index of last min occurrence
    FIRST_MAX = 2, //!< get index of first max occurrence
    LAST_MAX  = 3, //!< get index of last max occurrence
};

template <typename T>
بنية reduceMinMaxImpl
{
    فراغ operator()(ثابت cv::Mat& src, cv::Mat& dst, ReduceMode mode, ثابت صحيح axis) ثابت
    {
        بدل(mode)
        {
        حالة ReduceMode::FIRST_MIN:
            reduceMinMaxApply<std::less>(src, dst, axis);
            اكسر;
        حالة ReduceMode::LAST_MIN:
            reduceMinMaxApply<std::less_equal>(src, dst, axis);
            اكسر;
        حالة ReduceMode::FIRST_MAX:
            reduceMinMaxApply<std::greater>(src, dst, axis);
            اكسر;
        حالة ReduceMode::LAST_MAX:
            reduceMinMaxApply<std::greater_equal>(src, dst, axis);
            اكسر;
        }
    }

    template <template<صنف> صنف Cmp>
    ساكن فراغ reduceMinMaxApply(ثابت cv::Mat& src, cv::Mat& dst, ثابت صحيح axis)
    {
        Cmp<T> cmp;

        ثابت auto *src_ptr = src.ptr<T>();
        auto *dst_ptr = dst.ptr<int32_t>();

        ثابت size_t outer_size = src.total(0, axis);
        ثابت auto mid_size = static_cast<size_t>(src.size[axis]);

        ثابت size_t outer_step = src.total(axis);
        ثابت size_t dst_step = dst.total(axis);

        ثابت size_t mid_step = src.total(axis + 1);

        لكل (size_t outer = 0; outer < outer_size; ++outer)
        {
            ثابت size_t outer_offset = outer * outer_step;
            ثابت size_t dst_offset = outer * dst_step;
            لكل (size_t mid = 0; mid != mid_size; ++mid)
            {
                ثابت size_t src_offset = outer_offset + mid * mid_step;
                لكل (size_t inner = 0; inner < mid_step; inner++)
                {
                    int32_t& index = dst_ptr[dst_offset + inner];

                    ثابت size_t prev = outer_offset + index * mid_step + inner;
                    ثابت size_t curr = src_offset + inner;

                    إذا (cmp(src_ptr[curr], src_ptr[prev]))
                    {
                        index = static_cast<int32_t>(mid);
                    }
                }
            }
        }
    }
};

ساكن فراغ reduceMinMax(cv::إدخال_Array src, cv::OutputArray dst, ReduceMode mode, صحيح axis)
{
    CV_INSTRUMENT_REGION();

    cv::Mat srcMat = src.getMat();
    صحيح dims = std::max(1, srcMat.dims);
    axis = (axis + dims) % dims;
    CV_Assert(srcMat.channels() == 1 && axis >= 0 && axis <= srcMat.dims);

    std::vector<صحيح> sizes(srcMat.dims);
    std::copy(srcMat.size.p, srcMat.size.p + srcMat.dims, sizes.begin());
    إذا(!sizes.empty())
        sizes[axis] = 1;

    dst.create(srcMat.dims, sizes.data(), CV_32SC1); // indices
    cv::Mat dstMat = dst.getMat();
    dstMat.setTo(cv::Scalar::all(0));

    إذا (!srcMat.isContinuous())
    {
        srcMat = srcMat.clone();
    }

    منطقي needs_copy = !dstMat.isContinuous();
    إذا (needs_copy)
    {
        dstMat = dstMat.clone();
    }

    cv::detail::depthDispatch<reduceMinMaxImpl>(srcMat.depth(), srcMat, dstMat, mode, axis);

    إذا (needs_copy)
    {
        dstMat.copyTo(dst);
    }
}

فراغ cv::reduceArgMin(إدخال_Array src, OutputArray dst, صحيح axis, منطقي lastIndex)
{
    reduceMinMax(src, dst, lastIndex ? ReduceMode::LAST_MIN : ReduceMode::FIRST_MIN, axis);
}

فراغ cv::reduceArgMax(إدخال_Array src, OutputArray dst, صحيح axis, منطقي lastIndex)
{
    reduceMinMax(src, dst, lastIndex ? ReduceMode::LAST_MAX : ReduceMode::FIRST_MAX, axis);
}
