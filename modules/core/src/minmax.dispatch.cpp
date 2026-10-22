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
    𐤀𐤌( ofs > 0 )
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
    𐤀𐤌 (minVal || minLoc)
    {
        minptr = db.ptr<T>();
        index += حجم(T) * groupnum;
        index = alignSize(index, MINMAX_STRUCT_ALIGNMENT);
    }
    𐤀𐤌 (maxVal || maxLoc)
    {
        maxptr = (ثابت T *)(db.ptr() + index);
        index += حجم(T) * groupnum;
        index = alignSize(index, MINMAX_STRUCT_ALIGNMENT);
    }
    𐤀𐤌 (minLoc)
    {
        minlocptr = (ثابت uint *)(db.ptr() + index);
        index += حجم(uint) * groupnum;
        index = alignSize(index, MINMAX_STRUCT_ALIGNMENT);
    }
    𐤀𐤌 (maxLoc)
    {
        maxlocptr = (ثابت uint *)(db.ptr() + index);
        index += حجم(uint) * groupnum;
        index = alignSize(index, MINMAX_STRUCT_ALIGNMENT);
    }
    𐤀𐤌 (maxVal2)
        maxptr2 = (ثابت T *)(db.ptr() + index);

    لكل (صحيح i = 0; i < groupnum; i++)
    {
        𐤀𐤌 (minptr && minptr[i] <= minval)
        {
            𐤀𐤌 (minptr[i] == minval)
            {
                𐤀𐤌 (minlocptr)
                    minloc = std::min(minlocptr[i], minloc);
            }
            وإلا
            {
                𐤀𐤌 (minlocptr)
                    minloc = minlocptr[i];
                minval = minptr[i];
            }
        }
        𐤀𐤌 (maxptr && maxptr[i] >= maxval)
        {
            𐤀𐤌 (maxptr[i] == maxval)
            {
                𐤀𐤌 (maxlocptr)
                    maxloc = std::min(maxlocptr[i], maxloc);
            }
            وإلا
            {
                𐤀𐤌 (maxlocptr)
                    maxloc = maxlocptr[i];
                maxval = maxptr[i];
            }
        }
        𐤀𐤌 (maxptr2 && maxptr2[i] > maxval2)
            maxval2 = maxptr2[i];
    }
    منطقي zero_mask = (minLoc && minloc == index_max) ||
    (maxLoc && maxloc == index_max);

    𐤀𐤌 (minVal)
        *minVal = zero_mask ? 0 : (مزدوج)minval;
    𐤀𐤌 (maxVal)
        *maxVal = zero_mask ? 0 : (مزدوج)maxval;
    𐤀𐤌 (maxVal2)
        *maxVal2 = zero_mask ? 0 : (مزدوج)maxval2;

    𐤀𐤌 (minLoc)
    {
        minLoc[0] = zero_mask ? -1 : minloc / cols;
        minLoc[1] = zero_mask ? -1 : minloc % cols;
    }
    𐤀𐤌 (maxLoc)
    {
        maxLoc[0] = zero_mask ? -1 : maxloc / cols;
        maxLoc[1] = zero_mask ? -1 : maxloc % cols;
    }
}

تعريفنوع فراغ (*getMinMaxResFunc)(ثابت Mat & db, مزدوج * minVal, مزدوج * maxVal,
                                 صحيح * minLoc, صحيح *maxLoc, صحيح gropunum, صحيح cols, مزدوج * maxVal2);

منطقي ocl_minMaxIdx( InputArray _src, مزدوج* minVal, مزدوج* maxVal, صحيح* minLoc, صحيح* maxLoc, InputArray _mask,
                   صحيح ddepth, منطقي absValues, InputArray _src2, مزدوج * maxVal2)
{
    ثابت ocl::Device & dev = ocl::Device::getDefault();

#ifdef __ANDROID__
    𐤀𐤌 (dev.isNVidia())
        أرجع خطأ;
#endif

    𐤀𐤌 (dev.deviceVersionMajor() == 1 && dev.deviceVersionMinor() < 2)
    {
        // 'static' storage class specifier used by "minmaxloc" is available from OpenCL 1.2+ only
        أرجع خطأ;
    }

    منطقي doubleSupport = dev.doubleFPConfig() > 0, haveMask = !_mask.empty(),
    haveSrc2 = _src2.kind() != _InputArray::NONE;
    صحيح type = _src.type(), depth = CV_MAT_DEPTH(type), cn = CV_MAT_CN(type),
    kercn = haveMask ? cn : std::min(4, ocl::predictOptimalVectorWidth(_src, _src2));

    𐤀𐤌 (depth >= CV_16F)
        أرجع خطأ;

    // disabled following modes since it occasionally fails on AMD devices (e.g. A10-6800K, sep. 2014)
    𐤀𐤌 ((haveMask || type == CV_32FC1) && dev.isAMD())
        أرجع خطأ;

    CV_Assert( (cn == 1 && (!haveMask || _mask.type() == CV_8U || _mask.type() == CV_8S || _mask.type() == CV_Bool)) ||
              (cn >= 1 && !minLoc && !maxLoc) );

    𐤀𐤌 (ddepth < 0)
        ddepth = depth;

    CV_Assert(!haveSrc2 || _src2.type() == type);

    𐤀𐤌 (depth == CV_32S || depth == CV_8S || depth == CV_32U || depth == CV_64U ||
        depth == CV_64S || depth == CV_16F || depth == CV_16BF)
        أرجع خطأ;

    𐤀𐤌 ((depth == CV_64F || ddepth == CV_64F) && !doubleSupport)
        أرجع خطأ;

    صحيح groupnum = dev.maxComputeUnits();
    size_t wgs = dev.maxWorkGroupSize();

    صحيح wgs2_aligned = 1;
    طالما (wgs2_aligned < (صحيح)wgs)
        wgs2_aligned <<= 1;
    wgs2_aligned >>= 1;

    منطقي needMinVal = minVal || minLoc, needMinLoc = minLoc != عدم,
    needMaxVal = maxVal || maxLoc, needMaxLoc = maxLoc != عدم;

    // in case of mask we must know whether mask is filled with zeros or not
    // so let's calculate min or max location, if it's undefined, so mask is zeros
    𐤀𐤌 (!(needMaxLoc || needMinLoc) && haveMask)
    {
        𐤀𐤌 (needMinVal)
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
                         absValues ? " -D OP_ABS" : "",
                         haveSrc2 ? " -D HAVE_SRC2" : "", maxVal2 ? " -D OP_CALC2" : "",
                         haveSrc2 && _src2.isContinuous() ? " -D HAVE_SRC2_CONT" : "", ddepth,
                         depth <= CV_32S && ddepth == CV_32S ? ocl::convertTypeStr(CV_8U, ddepth, kercn, cvt[1], حجم(cvt[1])) : "noconvert",
                         MINMAX_STRUCT_ALIGNMENT);

    ocl::Kernel k("minmaxloc", ocl::core::minmaxloc_oclsrc, opts);
    𐤀𐤌 (k.empty())
        أرجع خطأ;

    صحيح esz = CV_ELEM_SIZE(ddepth), esz32s = CV_ELEM_SIZE1(CV_32S),
    dbsize = groupnum * ((needMinVal ? esz : 0) + (needMaxVal ? esz : 0) +
                         (needMinLoc ? esz32s : 0) + (needMaxLoc ? esz32s : 0) +
                         (maxVal2 ? esz : 0))
    + 5 * MINMAX_STRUCT_ALIGNMENT;
    UMat src = _src.getUMat(), src2 = _src2.getUMat(), db(1, dbsize, CV_8UC1), mask = _mask.getUMat();

    𐤀𐤌 (cn > 1 && !haveMask)
    {
        src = src.reshape(1);
        src2 = src2.reshape(1);
    }

    𐤀𐤌 (haveSrc2)
    {
        𐤀𐤌 (!haveMask)
            k.args(ocl::KernelArg::ReadOnlyNoSize(src), src.cols, (صحيح)src.total(),
                   groupnum, ocl::KernelArg::PtrWriteOnly(db), ocl::KernelArg::ReadOnlyNoSize(src2));
        وإلا
            k.args(ocl::KernelArg::ReadOnlyNoSize(src), src.cols, (صحيح)src.total(),
                   groupnum, ocl::KernelArg::PtrWriteOnly(db), ocl::KernelArg::ReadOnlyNoSize(mask),
                   ocl::KernelArg::ReadOnlyNoSize(src2));
    }
    وإلا
    {
        𐤀𐤌 (!haveMask)
            k.args(ocl::KernelArg::ReadOnlyNoSize(src), src.cols, (صحيح)src.total(),
                   groupnum, ocl::KernelArg::PtrWriteOnly(db));
        وإلا
            k.args(ocl::KernelArg::ReadOnlyNoSize(src), src.cols, (صحيح)src.total(),
                   groupnum, ocl::KernelArg::PtrWriteOnly(db), ocl::KernelArg::ReadOnlyNoSize(mask));
    }

    size_t globalsize = groupnum * wgs;
    𐤀𐤌 (!k.run(1, &globalsize, &wgs, true))
        أرجع خطأ;

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

    أرجع true;
}

#endif

}

// serial implementation, also used for the pieces of the parallel version
ساكن فراغ minMaxIdx_(cv::InputArray _src, مزدوج* minVal,
                       مزدوج* maxVal, صحيح* minIdx, صحيح* maxIdx,
                       cv::InputArray _mask)
{
    using نطاق cv;
    صحيح type = _src.type(), depth = CV_MAT_DEPTH(type), cn = CV_MAT_CN(type);
    CV_Assert( (cn == 1 && (_mask.empty() || _mask.type() == CV_8U || _mask.type() == CV_8S || _mask.type() == CV_Bool)) ||
               (cn > 1 && _mask.empty() && !minIdx && !maxIdx) );

    CV_OCL_RUN(OCL_PERFORMANCE_CHECK(_src.isUMat()) && _src.dims() <= 2  && (_mask.empty() || _src.size() == _mask.size()),
               ocl_minMaxIdx(_src, minVal, maxVal, minIdx, maxIdx, _mask))

    Mat src = _src.getMat(), mask = _mask.getMat();

    𐤀𐤌 (src.dims <= 2)
    {
        𐤀𐤌 ((size_t)src.step == (size_t)mask.step || mask.empty())
        {
            CALL_HAL(minMaxIdx, cv_hal_minMaxIdx, src.data, src.step, src.cols*cn, src.rows,
                     src.depth(), minVal, maxVal, minIdx, maxIdx, mask.data);
        }
        CALL_HAL(minMaxIdxMaskStep, cv_hal_minMaxIdxMaskStep, src.data, src.step, src.cols*cn, src.rows,
                 src.depth(), minVal, maxVal, minIdx, maxIdx, mask.data, mask.step);
    }
    وإلا 𐤀𐤌 (src.isContinuous() && (mask.isContinuous() || mask.empty()))
    {
        صحيح res = cv_hal_minMaxIdx(src.data, 0, (صحيح)src.total()*cn, 1, src.depth(),
                                   minVal, maxVal, minIdx, maxIdx, mask.data);

        𐤀𐤌 (res == CV_HAL_ERROR_OK)
        {
            // minIdx[0] and minIdx[0] are always 0 for "flatten" version
            𐤀𐤌 (minIdx)
                ofs2idx(src, minIdx[1]+1, minIdx);
            𐤀𐤌 (maxIdx)
                ofs2idx(src, maxIdx[1]+1, maxIdx);
            أرجع;
        }
        وإلا 𐤀𐤌 (res != CV_HAL_ERROR_NOT_IMPLEMENTED)
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
    𐤀𐤌( depth <= CV_32S || depth == CV_Bool )
        dminval = minval.i, dmaxval = maxval.i;
    وإلا 𐤀𐤌( depth == CV_32F || depth == CV_16F || depth == CV_16BF )
        dminval = minval.f, dmaxval = maxval.f;
    وإلا 𐤀𐤌( depth == CV_64F )
        dminval = minval.d, dmaxval = maxval.d;
    وإلا 𐤀𐤌( depth == CV_64S || depth == CV_32U )
        dminval = (مزدوج)minval.L, dmaxval = (مزدوج)maxval.L;
    وإلا {
        CV_Assert(depth == CV_64U);
        dminval = (مزدوج)minval.UL, dmaxval = (مزدوج)maxval.UL;
    }

    𐤀𐤌( minVal )
        *minVal = dminval;
    𐤀𐤌( maxVal )
        *maxVal = dmaxval;

    𐤀𐤌( minIdx )
        ofs2idx(src, minidx, minIdx);
    𐤀𐤌( maxIdx )
        ofs2idx(src, maxidx, maxIdx);
}

فراغ cv::minMaxIdx(InputArray _src, مزدوج* minVal,
                   مزدوج* maxVal, صحيح* minIdx, صحيح* maxIdx,
                   InputArray _mask)
{
    CV_INSTRUMENT_REGION();

    std::vector<StatChunk> chunks;
    𐤀𐤌 (!_src.isUMat())
    {
        Mat src = _src.getMat(), mask = _mask.getMat();
        ثابت صحيح cn = src.channels();
        ثابت منطقي argsOk = (cn == 1 && (mask.empty() || ((mask.type() == CV_8U || mask.type() == CV_8S ||
                             mask.type() == CV_Bool) && src.size == mask.size))) ||
                            (cn > 1 && mask.empty() && !minIdx && !maxIdx);
        𐤀𐤌 (argsOk && splitForParallelStat(src, mask, chunks))
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
                    𐤀𐤌 (cn > 1)
                        imn[i] = imx[i] = 1;
                    وإلا 𐤀𐤌 (a[0] >= 0)
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
                𐤀𐤌 (imn[i] == 0)
                    استمر;
                𐤀𐤌 (bmin < 0 || mn[i] < mn[bmin]) bmin = i;
                𐤀𐤌 (bmax < 0 || mx[i] > mx[bmax]) bmax = i;
            }
            𐤀𐤌 (minVal) *minVal = bmin >= 0 ? mn[bmin] : 0;
            𐤀𐤌 (maxVal) *maxVal = bmax >= 0 ? mx[bmax] : 0;
            𐤀𐤌 (minIdx) ofs2idx(src, bmin >= 0 ? imn[bmin] : 0, minIdx);
            𐤀𐤌 (maxIdx) ofs2idx(src, bmax >= 0 ? imx[bmax] : 0, maxIdx);
            أرجع;
        }
    }
    minMaxIdx_(_src, minVal, maxVal, minIdx, maxIdx, _mask);
}

فراغ cv::minMaxLoc( InputArray _img, مزدوج* minVal, مزدوج* maxVal,
                    Point* minLoc, Point* maxLoc, InputArray mask )
{
    CV_INSTRUMENT_REGION();

    صحيح dims = _img.dims();
    CV_CheckLE(dims, 2, "");

    minMaxIdx(_img, minVal, maxVal, (صحيح*)minLoc, (صحيح*)maxLoc, mask);
    𐤀𐤌( minLoc) {
        𐤀𐤌 (dims == 2)
            std::swap(minLoc->x, minLoc->y);
        وإلا {
            minLoc->y = 0;
        }
    }
    𐤀𐤌( maxLoc) {
        𐤀𐤌 (dims == 2)
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

                    𐤀𐤌 (cmp(src_ptr[curr], src_ptr[prev]))
                    {
                        index = static_cast<int32_t>(mid);
                    }
                }
            }
        }
    }
};

ساكن فراغ reduceMinMax(cv::InputArray src, cv::OutputArray dst, ReduceMode mode, صحيح axis)
{
    CV_INSTRUMENT_REGION();

    cv::Mat srcMat = src.getMat();
    صحيح dims = std::max(1, srcMat.dims);
    axis = (axis + dims) % dims;
    CV_Assert(srcMat.channels() == 1 && axis >= 0 && axis <= srcMat.dims);

    std::vector<صحيح> sizes(srcMat.dims);
    std::copy(srcMat.size.p, srcMat.size.p + srcMat.dims, sizes.begin());
    𐤀𐤌(!sizes.empty())
        sizes[axis] = 1;

    dst.create(srcMat.dims, sizes.data(), CV_32SC1); // indices
    cv::Mat dstMat = dst.getMat();
    dstMat.setTo(cv::Scalar::all(0));

    𐤀𐤌 (!srcMat.isContinuous())
    {
        srcMat = srcMat.clone();
    }

    منطقي needs_copy = !dstMat.isContinuous();
    𐤀𐤌 (needs_copy)
    {
        dstMat = dstMat.clone();
    }

    cv::detail::depthDispatch<reduceMinMaxImpl>(srcMat.depth(), srcMat, dstMat, mode, axis);

    𐤀𐤌 (needs_copy)
    {
        dstMat.copyTo(dst);
    }
}

فراغ cv::reduceArgMin(InputArray src, OutputArray dst, صحيح axis, منطقي lastIndex)
{
    reduceMinMax(src, dst, lastIndex ? ReduceMode::LAST_MIN : ReduceMode::FIRST_MIN, axis);
}

فراغ cv::reduceArgMax(InputArray src, OutputArray dst, صحيح axis, منطقي lastIndex)
{
    reduceMinMax(src, dst, lastIndex ? ReduceMode::LAST_MAX : ReduceMode::FIRST_MAX, axis);
}
