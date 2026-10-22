// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


#شامل "precomp.hpp"
#شامل "opencl_kernels_core.hpp"
#شامل "stat.hpp"
#شامل "stat_parallel.hpp"
#شامل "opencv2/core/detail/dispatch_helper.impl.hpp"
#شامل <algorithm>

#شامل "minmax.simd.hpp"
#شامل "minmax.simd_declarations.hpp" // defines CV_CPU_DISPATCH_MODES_ALL=AVX2,...,BASELINE based on CMakeLists.txt content

فضاینام cv {

ایستا MinMaxIdxFunc getMinMaxIdxFunc(صحیح depth)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getMinMaxIdxFunc, (depth),
                    CV_CPU_DISPATCH_MODES_ALL);
}

// The function expects 1-based indexing for ofs
// Zero is treated as invalid offset (not found)
ایستا تهی ofs2idx(ثابت Mat& a, size_t ofs, صحیح* idx)
{
    صحیح i, d = a.dims;
    اگر( ofs > 0 )
    {
        ofs--;
        برای( i = d-1; i >= 0; i-- )
        {
            صحیح sz = a.size[i];
            idx[i] = (صحیح)(ofs % sz);
            ofs /= sz;
        }
    }
    وگرنه
    {
        برای( i = d-1; i >= 0; i-- )
            idx[i] = -1;
    }
}

#ifdef HAVE_OPENCL

#define MINMAX_STRUCT_ALIGNMENT 8 // sizeof double

template <typename T>
تهی getMinMaxRes(ثابت Mat & db, دوگانه * minVal, دوگانه * maxVal,
                  صحیح* minLoc, صحیح* maxLoc,
                  صحیح groupnum, صحیح cols, دوگانه * maxVal2)
{
    uint index_max = std::numeric_limits<uint>::max();
    T minval = std::numeric_limits<T>::max();
    T maxval = std::numeric_limits<T>::min() > 0 ? (T)-std::numeric_limits<T>::max() : std::numeric_limits<T>::min(), maxval2 = maxval;
    uint minloc = index_max, maxloc = index_max;

    size_t index = 0;
    ثابت T * minptr = پوچ, * maxptr = پوچ, * maxptr2 = پوچ;
    ثابت uint * minlocptr = پوچ, * maxlocptr = پوچ;
    اگر (minVal || minLoc)
    {
        minptr = db.ptr<T>();
        index += اندازه(T) * groupnum;
        index = alignSize(index, MINMAX_STRUCT_ALIGNMENT);
    }
    اگر (maxVal || maxLoc)
    {
        maxptr = (ثابت T *)(db.ptr() + index);
        index += اندازه(T) * groupnum;
        index = alignSize(index, MINMAX_STRUCT_ALIGNMENT);
    }
    اگر (minLoc)
    {
        minlocptr = (ثابت uint *)(db.ptr() + index);
        index += اندازه(uint) * groupnum;
        index = alignSize(index, MINMAX_STRUCT_ALIGNMENT);
    }
    اگر (maxLoc)
    {
        maxlocptr = (ثابت uint *)(db.ptr() + index);
        index += اندازه(uint) * groupnum;
        index = alignSize(index, MINMAX_STRUCT_ALIGNMENT);
    }
    اگر (maxVal2)
        maxptr2 = (ثابت T *)(db.ptr() + index);

    برای (صحیح i = 0; i < groupnum; i++)
    {
        اگر (minptr && minptr[i] <= minval)
        {
            اگر (minptr[i] == minval)
            {
                اگر (minlocptr)
                    minloc = std::min(minlocptr[i], minloc);
            }
            وگرنه
            {
                اگر (minlocptr)
                    minloc = minlocptr[i];
                minval = minptr[i];
            }
        }
        اگر (maxptr && maxptr[i] >= maxval)
        {
            اگر (maxptr[i] == maxval)
            {
                اگر (maxlocptr)
                    maxloc = std::min(maxlocptr[i], maxloc);
            }
            وگرنه
            {
                اگر (maxlocptr)
                    maxloc = maxlocptr[i];
                maxval = maxptr[i];
            }
        }
        اگر (maxptr2 && maxptr2[i] > maxval2)
            maxval2 = maxptr2[i];
    }
    منطقی zero_mask = (minLoc && minloc == index_max) ||
    (maxLoc && maxloc == index_max);

    اگر (minVal)
        *minVal = zero_mask ? 0 : (دوگانه)minval;
    اگر (maxVal)
        *maxVal = zero_mask ? 0 : (دوگانه)maxval;
    اگر (maxVal2)
        *maxVal2 = zero_mask ? 0 : (دوگانه)maxval2;

    اگر (minLoc)
    {
        minLoc[0] = zero_mask ? -1 : minloc / cols;
        minLoc[1] = zero_mask ? -1 : minloc % cols;
    }
    اگر (maxLoc)
    {
        maxLoc[0] = zero_mask ? -1 : maxloc / cols;
        maxLoc[1] = zero_mask ? -1 : maxloc % cols;
    }
}

تعریفنوع تهی (*getMinMaxResFunc)(ثابت Mat & db, دوگانه * minVal, دوگانه * maxVal,
                                 صحیح * minLoc, صحیح *maxLoc, صحیح gropunum, صحیح cols, دوگانه * maxVal2);

منطقی ocl_minMaxIdx( درون_داد_Array _src, دوگانه* minVal, دوگانه* maxVal, صحیح* minLoc, صحیح* maxLoc, درون_داد_Array _mask,
                   صحیح ddepth, منطقی abs_مقادیر, درون_داد_Array _src2, دوگانه * maxVal2)
{
    ثابت ocl::Device & dev = ocl::Device::getDefault();

#ifdef __ANDROID__
    اگر (dev.isNVidia())
        بازگشت نادرست;
#endif

    اگر (dev.deviceVersionMajor() == 1 && dev.deviceVersionMinor() < 2)
    {
        // 'static' storage class specifier used by "minmaxloc" is available from OpenCL 1.2+ only
        بازگشت نادرست;
    }

    منطقی doubleSupport = dev.doubleFPConfig() > 0, haveMask = !_mask.empty(),
    haveSrc2 = _src2.kind() != درون_داد_Array_2::NONE;
    صحیح type = _src.type(), depth = CV_MAT_DEPTH(type), cn = CV_MAT_CN(type),
    kercn = haveMask ? cn : std::min(4, ocl::predictOptimalVectorWidth(_src, _src2));

    اگر (depth >= CV_16F)
        بازگشت نادرست;

    // disabled following modes since it occasionally fails on AMD devices (e.g. A10-6800K, sep. 2014)
    اگر ((haveMask || type == CV_32FC1) && dev.isAMD())
        بازگشت نادرست;

    CV_Assert( (cn == 1 && (!haveMask || _mask.type() == CV_8U || _mask.type() == CV_8S || _mask.type() == CV_Bool)) ||
              (cn >= 1 && !minLoc && !maxLoc) );

    اگر (ddepth < 0)
        ddepth = depth;

    CV_Assert(!haveSrc2 || _src2.type() == type);

    اگر (depth == CV_32S || depth == CV_8S || depth == CV_32U || depth == CV_64U ||
        depth == CV_64S || depth == CV_16F || depth == CV_16BF)
        بازگشت نادرست;

    اگر ((depth == CV_64F || ddepth == CV_64F) && !doubleSupport)
        بازگشت نادرست;

    صحیح groupnum = dev.maxComputeUnits();
    size_t wgs = dev.maxWorkGroupSize();

    صحیح wgs2_aligned = 1;
    وقتی (wgs2_aligned < (صحیح)wgs)
        wgs2_aligned <<= 1;
    wgs2_aligned >>= 1;

    منطقی needMinVal = minVal || minLoc, needMinLoc = minLoc != پوچ,
    needMaxVal = maxVal || maxLoc, needMaxLoc = maxLoc != پوچ;

    // in case of mask we must know whether mask is filled with zeros or not
    // so let's calculate min or max location, if it's undefined, so mask is zeros
    اگر (!(needMaxLoc || needMinLoc) && haveMask)
    {
        اگر (needMinVal)
            needMinLoc = درست;
        وگرنه
            needMaxLoc = درست;
    }

    نویسه cvt[2][50];
    String opts = format("-D DEPTH_%d -D srcT1=%s%s -D WGS=%d -D srcT=%s"
                         " -D WGS2_ALIGNED=%d%s%s%s -D kercn=%d%s%s%s%s"
                         " -D dstT1=%s -D dstT=%s -D convertToDT=%s%s%s%s%s -D wdepth=%d -D convertFromU=%s"
                         " -D MINMAX_STRUCT_ALIGNMENT=%d",
                         depth, ocl::typeToStr(depth), haveMask ? " -D HAVE_MASK" : "", (صحیح)wgs,
                         ocl::typeToStr(CV_MAKE_TYPE(depth, kercn)), wgs2_aligned,
                         doubleSupport ? " -D DOUBLE_SUPPORT" : "",
                         _src.isContinuous() ? " -D HAVE_SRC_CONT" : "",
                         _mask.isContinuous() ? " -D HAVE_MASK_CONT" : "", kercn,
                         needMinVal ? " -D NEED_MINVAL" : "", needMaxVal ? " -D NEED_MAXVAL" : "",
                         needMinLoc ? " -D NEED_MINLOC" : "", needMaxLoc ? " -D NEED_MAXLOC" : "",
                         ocl::typeToStr(ddepth), ocl::typeToStr(CV_MAKE_TYPE(ddepth, kercn)),
                         ocl::convertTypeStr(depth, ddepth, kercn, cvt[0], اندازه(cvt[0])),
                         abs_مقادیر ? " -D OP_ABS" : "",
                         haveSrc2 ? " -D HAVE_SRC2" : "", maxVal2 ? " -D OP_CALC2" : "",
                         haveSrc2 && _src2.isContinuous() ? " -D HAVE_SRC2_CONT" : "", ddepth,
                         depth <= CV_32S && ddepth == CV_32S ? ocl::convertTypeStr(CV_8U, ddepth, kercn, cvt[1], اندازه(cvt[1])) : "noconvert",
                         MINMAX_STRUCT_ALIGNMENT);

    ocl::Kernel k("minmaxloc", ocl::core::minmaxloc_oclsrc, opts);
    اگر (k.empty())
        بازگشت نادرست;

    صحیح esz = CV_ELEM_SIZE(ddepth), esz32s = CV_ELEM_SIZE1(CV_32S),
    dbsize = groupnum * ((needMinVal ? esz : 0) + (needMaxVal ? esz : 0) +
                         (needMinLoc ? esz32s : 0) + (needMaxLoc ? esz32s : 0) +
                         (maxVal2 ? esz : 0))
    + 5 * MINMAX_STRUCT_ALIGNMENT;
    UMat src = _src.getUMat(), src2 = _src2.getUMat(), db(1, dbsize, CV_8UC1), mask = _mask.getUMat();

    اگر (cn > 1 && !haveMask)
    {
        src = src.reshape(1);
        src2 = src2.reshape(1);
    }

    اگر (haveSrc2)
    {
        اگر (!haveMask)
            k.args(ocl::KernelArg::ReadOnlyNoSize(src), src.cols, (صحیح)src.total(),
                   groupnum, ocl::KernelArg::PtrWriteOnly(db), ocl::KernelArg::ReadOnlyNoSize(src2));
        وگرنه
            k.args(ocl::KernelArg::ReadOnlyNoSize(src), src.cols, (صحیح)src.total(),
                   groupnum, ocl::KernelArg::PtrWriteOnly(db), ocl::KernelArg::ReadOnlyNoSize(mask),
                   ocl::KernelArg::ReadOnlyNoSize(src2));
    }
    وگرنه
    {
        اگر (!haveMask)
            k.args(ocl::KernelArg::ReadOnlyNoSize(src), src.cols, (صحیح)src.total(),
                   groupnum, ocl::KernelArg::PtrWriteOnly(db));
        وگرنه
            k.args(ocl::KernelArg::ReadOnlyNoSize(src), src.cols, (صحیح)src.total(),
                   groupnum, ocl::KernelArg::PtrWriteOnly(db), ocl::KernelArg::ReadOnlyNoSize(mask));
    }

    size_t globalsize = groupnum * wgs;
    اگر (!k.run(1, &globalsize, &wgs, درست))
        بازگشت نادرست;

    ایستا ثابت getMinMaxResFunc functab[7] =
    {
        getMinMaxRes<uchar>,
        getMinMaxRes<نویسه>,
        getMinMaxRes<ushort>,
        getMinMaxRes<short>,
        getMinMaxRes<صحیح>,
        getMinMaxRes<اعشاری>,
        getMinMaxRes<دوگانه>
    };

    CV_Assert(ddepth <= CV_64F);
    getMinMaxResFunc func = functab[ddepth];

    صحیح locTemp[2];
    func(db.getMat(ACCESS_READ), minVal, maxVal,
         needMinLoc ? minLoc ? minLoc : locTemp : minLoc,
         needMaxLoc ? maxLoc ? maxLoc : locTemp : maxLoc,
         groupnum, src.cols, maxVal2);

    بازگشت درست;
}

#endif

}

// serial implementation, also used for the pieces of the parallel version
ایستا تهی minMaxIdx_(cv::درون_داد_Array _src, دوگانه* minVal,
                       دوگانه* maxVal, صحیح* minIdx, صحیح* maxIdx,
                       cv::درون_داد_Array _mask)
{
    using فضاینام cv;
    صحیح type = _src.type(), depth = CV_MAT_DEPTH(type), cn = CV_MAT_CN(type);
    CV_Assert( (cn == 1 && (_mask.empty() || _mask.type() == CV_8U || _mask.type() == CV_8S || _mask.type() == CV_Bool)) ||
               (cn > 1 && _mask.empty() && !minIdx && !maxIdx) );

    CV_OCL_RUN(OCL_PERFORMANCE_CHECK(_src.isUMat()) && _src.dims() <= 2  && (_mask.empty() || _src.size() == _mask.size()),
               ocl_minMaxIdx(_src, minVal, maxVal, minIdx, maxIdx, _mask))

    Mat src = _src.getMat(), mask = _mask.getMat();

    اگر (src.dims <= 2)
    {
        اگر ((size_t)src.step == (size_t)mask.step || mask.empty())
        {
            CALL_HAL(minMaxIdx, cv_hal_minMaxIdx, src.data, src.step, src.cols*cn, src.rows,
                     src.depth(), minVal, maxVal, minIdx, maxIdx, mask.data);
        }
        CALL_HAL(minMaxIdxMaskStep, cv_hal_minMaxIdxMaskStep, src.data, src.step, src.cols*cn, src.rows,
                 src.depth(), minVal, maxVal, minIdx, maxIdx, mask.data, mask.step);
    }
    وگرنه اگر (src.isContinuous() && (mask.isContinuous() || mask.empty()))
    {
        صحیح res = cv_hal_minMaxIdx(src.data, 0, (صحیح)src.total()*cn, 1, src.depth(),
                                   minVal, maxVal, minIdx, maxIdx, mask.data);

        اگر (res == CV_HAL_ERROR_OK)
        {
            // minIdx[0] and minIdx[0] are always 0 for "flatten" version
            اگر (minIdx)
                ofs2idx(src, minIdx[1]+1, minIdx);
            اگر (maxIdx)
                ofs2idx(src, maxIdx[1]+1, maxIdx);
            بازگشت;
        }
        وگرنه اگر (res != CV_HAL_ERROR_NOT_IMPLEMENTED)
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
    اجتماع {
        صحیح i;
        اعشاری f;
        دوگانه d;
        int64 L;
        uint64 UL;
    } minval, maxval;
    صحیح planeSize = (صحیح)it.size*cn;
    minval.L = maxval.L = 0;

    برای( size_t i = 0; i < it.nplanes; i++, ++it, startidx += planeSize )
        func( ptrs[0], ptrs[1], &minval.L, &maxval.L, &minidx, &maxidx, planeSize, startidx );

    دوگانه dminval, dmaxval;
    اگر( depth <= CV_32S || depth == CV_Bool )
        dminval = minval.i, dmaxval = maxval.i;
    وگرنه اگر( depth == CV_32F || depth == CV_16F || depth == CV_16BF )
        dminval = minval.f, dmaxval = maxval.f;
    وگرنه اگر( depth == CV_64F )
        dminval = minval.d, dmaxval = maxval.d;
    وگرنه اگر( depth == CV_64S || depth == CV_32U )
        dminval = (دوگانه)minval.L, dmaxval = (دوگانه)maxval.L;
    وگرنه {
        CV_Assert(depth == CV_64U);
        dminval = (دوگانه)minval.UL, dmaxval = (دوگانه)maxval.UL;
    }

    اگر( minVal )
        *minVal = dminval;
    اگر( maxVal )
        *maxVal = dmaxval;

    اگر( minIdx )
        ofs2idx(src, minidx, minIdx);
    اگر( maxIdx )
        ofs2idx(src, maxidx, maxIdx);
}

تهی cv::minMaxIdx(درون_داد_Array _src, دوگانه* minVal,
                   دوگانه* maxVal, صحیح* minIdx, صحیح* maxIdx,
                   درون_داد_Array _mask)
{
    CV_INSTRUMENT_REGION();

    std::vector<StatChunk> chunks;
    اگر (!_src.isUMat())
    {
        Mat src = _src.getMat(), mask = _mask.getMat();
        ثابت صحیح cn = src.channels();
        ثابت منطقی argsOk = (cn == 1 && (mask.empty() || ((mask.type() == CV_8U || mask.type() == CV_8S ||
                             mask.type() == CV_Bool) && src.size == mask.size))) ||
                            (cn > 1 && mask.empty() && !minIdx && !maxIdx);
        اگر (argsOk && splitForParallelStat(src, mask, chunks))
        {
            ثابت صحیح n = (صحیح)chunks.size();
            std::vector<دوگانه> mn(n), mx(n);
            std::vector<size_t> imn(n, 0), imx(n, 0);   // 1-based row-major indices, 0 = no element
            parallel_for_(Range(0, n), [&](ثابت Range& r)
            {
                برای (صحیح i = r.start; i < r.end; i++)
                {
                    صحیح a[2] = {-1, -1}, b[2] = {-1, -1};
                    minMaxIdx_(chunks[i].src, &mn[i], &mx[i], cn == 1 ? a : nullptr, cn == 1 ? b : nullptr,
                               chunks[i].mask);
                    اگر (cn > 1)
                        imn[i] = imx[i] = 1;
                    وگرنه اگر (a[0] >= 0)
                    {
                        imn[i] = chunkIndexToElem(chunks[i], a) + 1;
                        imx[i] = chunkIndexToElem(chunks[i], b) + 1;
                    }
                }
            });
            // pieces are in memory order, so taking the first strict improvement keeps the
            // first occurrence, as in the serial version
            صحیح bmin = -1, bmax = -1;
            برای (صحیح i = 0; i < n; i++)
            {
                اگر (imn[i] == 0)
                    ادامه;
                اگر (bmin < 0 || mn[i] < mn[bmin]) bmin = i;
                اگر (bmax < 0 || mx[i] > mx[bmax]) bmax = i;
            }
            اگر (minVal) *minVal = bmin >= 0 ? mn[bmin] : 0;
            اگر (maxVal) *maxVal = bmax >= 0 ? mx[bmax] : 0;
            اگر (minIdx) ofs2idx(src, bmin >= 0 ? imn[bmin] : 0, minIdx);
            اگر (maxIdx) ofs2idx(src, bmax >= 0 ? imx[bmax] : 0, maxIdx);
            بازگشت;
        }
    }
    minMaxIdx_(_src, minVal, maxVal, minIdx, maxIdx, _mask);
}

تهی cv::minMaxLoc( درون_داد_Array _img, دوگانه* minVal, دوگانه* maxVal,
                    Point* minLoc, Point* maxLoc, درون_داد_Array mask )
{
    CV_INSTRUMENT_REGION();

    صحیح dims = _img.dims();
    CV_CheckLE(dims, 2, "");

    minMaxIdx(_img, minVal, maxVal, (صحیح*)minLoc, (صحیح*)maxLoc, mask);
    اگر( minLoc) {
        اگر (dims == 2)
            std::swap(minLoc->x, minLoc->y);
        وگرنه {
            minLoc->y = 0;
        }
    }
    اگر( maxLoc) {
        اگر (dims == 2)
            std::swap(maxLoc->x, maxLoc->y);
        وگرنه {
            maxLoc->y = 0;
        }
    }
}

شمارش رده ReduceMode
{
    FIRST_MIN = 0, //!< get index of first min occurrence
    LAST_MIN  = 1, //!< get index of last min occurrence
    FIRST_MAX = 2, //!< get index of first max occurrence
    LAST_MAX  = 3, //!< get index of last max occurrence
};

template <typename T>
ساختار reduceMinMaxImpl
{
    تهی operator()(ثابت cv::Mat& src, cv::Mat& dst, ReduceMode mode, ثابت صحیح axis) ثابت
    {
        انتخاب(mode)
        {
        حالت ReduceMode::FIRST_MIN:
            reduceMinMaxApply<std::less>(src, dst, axis);
            بشکن;
        حالت ReduceMode::LAST_MIN:
            reduceMinMaxApply<std::less_equal>(src, dst, axis);
            بشکن;
        حالت ReduceMode::FIRST_MAX:
            reduceMinMaxApply<std::greater>(src, dst, axis);
            بشکن;
        حالت ReduceMode::LAST_MAX:
            reduceMinMaxApply<std::greater_equal>(src, dst, axis);
            بشکن;
        }
    }

    template <template<رده> رده Cmp>
    ایستا تهی reduceMinMaxApply(ثابت cv::Mat& src, cv::Mat& dst, ثابت صحیح axis)
    {
        Cmp<T> cmp;

        ثابت auto *src_ptr = src.ptr<T>();
        auto *dst_ptr = dst.ptr<int32_t>();

        ثابت size_t outer_size = src.total(0, axis);
        ثابت auto mid_size = static_cast<size_t>(src.size[axis]);

        ثابت size_t outer_step = src.total(axis);
        ثابت size_t dst_step = dst.total(axis);

        ثابت size_t mid_step = src.total(axis + 1);

        برای (size_t outer = 0; outer < outer_size; ++outer)
        {
            ثابت size_t outer_offset = outer * outer_step;
            ثابت size_t dst_offset = outer * dst_step;
            برای (size_t mid = 0; mid != mid_size; ++mid)
            {
                ثابت size_t src_offset = outer_offset + mid * mid_step;
                برای (size_t inner = 0; inner < mid_step; inner++)
                {
                    int32_t& index = dst_ptr[dst_offset + inner];

                    ثابت size_t prev = outer_offset + index * mid_step + inner;
                    ثابت size_t curr = src_offset + inner;

                    اگر (cmp(src_ptr[curr], src_ptr[prev]))
                    {
                        index = static_cast<int32_t>(mid);
                    }
                }
            }
        }
    }
};

ایستا تهی reduceMinMax(cv::درون_داد_Array src, cv::OutputArray dst, ReduceMode mode, صحیح axis)
{
    CV_INSTRUMENT_REGION();

    cv::Mat srcMat = src.getMat();
    صحیح dims = std::max(1, srcMat.dims);
    axis = (axis + dims) % dims;
    CV_Assert(srcMat.channels() == 1 && axis >= 0 && axis <= srcMat.dims);

    std::vector<صحیح> sizes(srcMat.dims);
    std::copy(srcMat.size.p, srcMat.size.p + srcMat.dims, sizes.begin());
    اگر(!sizes.empty())
        sizes[axis] = 1;

    dst.create(srcMat.dims, sizes.data(), CV_32SC1); // indices
    cv::Mat dstMat = dst.getMat();
    dstMat.setTo(cv::Scalar::all(0));

    اگر (!srcMat.isContinuous())
    {
        srcMat = srcMat.clone();
    }

    منطقی needs_copy = !dstMat.isContinuous();
    اگر (needs_copy)
    {
        dstMat = dstMat.clone();
    }

    cv::detail::depthDispatch<reduceMinMaxImpl>(srcMat.depth(), srcMat, dstMat, mode, axis);

    اگر (needs_copy)
    {
        dstMat.copyTo(dst);
    }
}

تهی cv::reduceArgMin(درون_داد_Array src, OutputArray dst, صحیح axis, منطقی lastIndex)
{
    reduceMinMax(src, dst, lastIndex ? ReduceMode::LAST_MIN : ReduceMode::FIRST_MIN, axis);
}

تهی cv::reduceArgMax(درون_داد_Array src, OutputArray dst, صحیح axis, منطقی lastIndex)
{
    reduceMinMax(src, dst, lastIndex ? ReduceMode::LAST_MAX : ReduceMode::FIRST_MAX, axis);
}
