// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


#অন্তর্ভাৱ "precomp.hpp"
#অন্তর্ভাৱ "opencl_kernels_core.hpp"
#অন্তর্ভাৱ "stat.hpp"
#অন্তর্ভাৱ "stat_parallel.hpp"
#অন্তর্ভাৱ "opencv2/core/detail/dispatch_helper.impl.hpp"
#অন্তর্ভাৱ <algorithm>

#অন্তর্ভাৱ "minmax.simd.hpp"
#অন্তর্ভাৱ "minmax.simd_declarations.hpp" // defines CV_CPU_DISPATCH_MODES_ALL=AVX2,...,BASELINE based on CMakeLists.txt content

নামাকাশ cv {

স্থির MinMaxIdxFunc getMinMaxIdxFunc(পূর্ণাঙ্ক depth)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getMinMaxIdxFunc, (depth),
                    CV_CPU_DISPATCH_MODES_ALL);
}

// The function expects 1-based indexing for ofs
// Zero is treated as invalid offset (not found)
স্থির শূন্য ofs2idx(ধ্রুৱ Mat& a, size_t ofs, পূর্ণাঙ্ক* idx)
{
    পূর্ণাঙ্ক i, d = a.dims;
    যদি( ofs > 0 )
    {
        ofs--;
        বাবে( i = d-1; i >= 0; i-- )
        {
            পূর্ণাঙ্ক sz = a.size[i];
            idx[i] = (পূর্ণাঙ্ক)(ofs % sz);
            ofs /= sz;
        }
    }
    অন্যথা
    {
        বাবে( i = d-1; i >= 0; i-- )
            idx[i] = -1;
    }
}

#ifdef HAVE_OPENCL

#define MINMAX_STRUCT_ALIGNMENT 8 // sizeof double

template <typename T>
শূন্য getMinMaxRes(ধ্রুৱ Mat & db, দ্ৱিগুণ * minVal, দ্ৱিগুণ * maxVal,
                  পূর্ণাঙ্ক* minLoc, পূর্ণাঙ্ক* maxLoc,
                  পূর্ণাঙ্ক groupnum, পূর্ণাঙ্ক cols, দ্ৱিগুণ * maxVal2)
{
    uint index_max = std::numeric_limits<uint>::max();
    T minval = std::numeric_limits<T>::max();
    T maxval = std::numeric_limits<T>::min() > 0 ? (T)-std::numeric_limits<T>::max() : std::numeric_limits<T>::min(), maxval2 = maxval;
    uint minloc = index_max, maxloc = index_max;

    size_t index = 0;
    ধ্রুৱ T * minptr = রিক্ত, * maxptr = রিক্ত, * maxptr2 = রিক্ত;
    ধ্রুৱ uint * minlocptr = রিক্ত, * maxlocptr = রিক্ত;
    যদি (minVal || minLoc)
    {
        minptr = db.ptr<T>();
        index += পরিমাণ(T) * groupnum;
        index = alignSize(index, MINMAX_STRUCT_ALIGNMENT);
    }
    যদি (maxVal || maxLoc)
    {
        maxptr = (ধ্রুৱ T *)(db.ptr() + index);
        index += পরিমাণ(T) * groupnum;
        index = alignSize(index, MINMAX_STRUCT_ALIGNMENT);
    }
    যদি (minLoc)
    {
        minlocptr = (ধ্রুৱ uint *)(db.ptr() + index);
        index += পরিমাণ(uint) * groupnum;
        index = alignSize(index, MINMAX_STRUCT_ALIGNMENT);
    }
    যদি (maxLoc)
    {
        maxlocptr = (ধ্রুৱ uint *)(db.ptr() + index);
        index += পরিমাণ(uint) * groupnum;
        index = alignSize(index, MINMAX_STRUCT_ALIGNMENT);
    }
    যদি (maxVal2)
        maxptr2 = (ধ্রুৱ T *)(db.ptr() + index);

    বাবে (পূর্ণাঙ্ক i = 0; i < groupnum; i++)
    {
        যদি (minptr && minptr[i] <= minval)
        {
            যদি (minptr[i] == minval)
            {
                যদি (minlocptr)
                    minloc = std::min(minlocptr[i], minloc);
            }
            অন্যথা
            {
                যদি (minlocptr)
                    minloc = minlocptr[i];
                minval = minptr[i];
            }
        }
        যদি (maxptr && maxptr[i] >= maxval)
        {
            যদি (maxptr[i] == maxval)
            {
                যদি (maxlocptr)
                    maxloc = std::min(maxlocptr[i], maxloc);
            }
            অন্যথা
            {
                যদি (maxlocptr)
                    maxloc = maxlocptr[i];
                maxval = maxptr[i];
            }
        }
        যদি (maxptr2 && maxptr2[i] > maxval2)
            maxval2 = maxptr2[i];
    }
    তার্কিক zero_mask = (minLoc && minloc == index_max) ||
    (maxLoc && maxloc == index_max);

    যদি (minVal)
        *minVal = zero_mask ? 0 : (দ্ৱিগুণ)minval;
    যদি (maxVal)
        *maxVal = zero_mask ? 0 : (দ্ৱিগুণ)maxval;
    যদি (maxVal2)
        *maxVal2 = zero_mask ? 0 : (দ্ৱিগুণ)maxval2;

    যদি (minLoc)
    {
        minLoc[0] = zero_mask ? -1 : minloc / cols;
        minLoc[1] = zero_mask ? -1 : minloc % cols;
    }
    যদি (maxLoc)
    {
        maxLoc[0] = zero_mask ? -1 : maxloc / cols;
        maxLoc[1] = zero_mask ? -1 : maxloc % cols;
    }
}

প্রকারনির্দেশ শূন্য (*getMinMaxResFunc)(ধ্রুৱ Mat & db, দ্ৱিগুণ * minVal, দ্ৱিগুণ * maxVal,
                                 পূর্ণাঙ্ক * minLoc, পূর্ণাঙ্ক *maxLoc, পূর্ণাঙ্ক gropunum, পূর্ণাঙ্ক cols, দ্ৱিগুণ * maxVal2);

তার্কিক ocl_minMaxIdx( প্ৰবেশ_Array _src, দ্ৱিগুণ* minVal, দ্ৱিগুণ* maxVal, পূর্ণাঙ্ক* minLoc, পূর্ণাঙ্ক* maxLoc, প্ৰবেশ_Array _mask,
                   পূর্ণাঙ্ক ddepth, তার্কিক abs_মানসমূহ, প্ৰবেশ_Array _src2, দ্ৱিগুণ * maxVal2)
{
    ধ্রুৱ ocl::Device & dev = ocl::Device::getDefault();

#ifdef __ANDROID__
    যদি (dev.isNVidia())
        ঘূৰাই মিছা;
#endif

    যদি (dev.deviceVersionMajor() == 1 && dev.deviceVersionMinor() < 2)
    {
        // 'static' storage class specifier used by "minmaxloc" is available from OpenCL 1.2+ only
        ঘূৰাই মিছা;
    }

    তার্কিক doubleSupport = dev.doubleFPConfig() > 0, haveMask = !_mask.empty(),
    haveSrc2 = _src2.kind() != প্ৰবেশ_Array_2::NONE;
    পূর্ণাঙ্ক type = _src.type(), depth = CV_MAT_DEPTH(type), cn = CV_MAT_CN(type),
    kercn = haveMask ? cn : std::min(4, ocl::predictOptimalVectorWidth(_src, _src2));

    যদি (depth >= CV_16F)
        ঘূৰাই মিছা;

    // disabled following modes since it occasionally fails on AMD devices (e.g. A10-6800K, sep. 2014)
    যদি ((haveMask || type == CV_32FC1) && dev.isAMD())
        ঘূৰাই মিছা;

    CV_Assert( (cn == 1 && (!haveMask || _mask.type() == CV_8U || _mask.type() == CV_8S || _mask.type() == CV_Bool)) ||
              (cn >= 1 && !minLoc && !maxLoc) );

    যদি (ddepth < 0)
        ddepth = depth;

    CV_Assert(!haveSrc2 || _src2.type() == type);

    যদি (depth == CV_32S || depth == CV_8S || depth == CV_32U || depth == CV_64U ||
        depth == CV_64S || depth == CV_16F || depth == CV_16BF)
        ঘূৰাই মিছা;

    যদি ((depth == CV_64F || ddepth == CV_64F) && !doubleSupport)
        ঘূৰাই মিছা;

    পূর্ণাঙ্ক groupnum = dev.maxComputeUnits();
    size_t wgs = dev.maxWorkGroupSize();

    পূর্ণাঙ্ক wgs2_aligned = 1;
    যেতিয়ালৈ (wgs2_aligned < (পূর্ণাঙ্ক)wgs)
        wgs2_aligned <<= 1;
    wgs2_aligned >>= 1;

    তার্কিক needMinVal = minVal || minLoc, needMinLoc = minLoc != রিক্ত,
    needMaxVal = maxVal || maxLoc, needMaxLoc = maxLoc != রিক্ত;

    // in case of mask we must know whether mask is filled with zeros or not
    // so let's calculate min or max location, if it's undefined, so mask is zeros
    যদি (!(needMaxLoc || needMinLoc) && haveMask)
    {
        যদি (needMinVal)
            needMinLoc = সঁচা;
        অন্যথা
            needMaxLoc = সঁচা;
    }

    ৱর্ণ cvt[2][50];
    String opts = format("-D DEPTH_%d -D srcT1=%s%s -D WGS=%d -D srcT=%s"
                         " -D WGS2_ALIGNED=%d%s%s%s -D kercn=%d%s%s%s%s"
                         " -D dstT1=%s -D dstT=%s -D convertToDT=%s%s%s%s%s -D wdepth=%d -D convertFromU=%s"
                         " -D MINMAX_STRUCT_ALIGNMENT=%d",
                         depth, ocl::typeToStr(depth), haveMask ? " -D HAVE_MASK" : "", (পূর্ণাঙ্ক)wgs,
                         ocl::typeToStr(CV_MAKE_TYPE(depth, kercn)), wgs2_aligned,
                         doubleSupport ? " -D DOUBLE_SUPPORT" : "",
                         _src.isContinuous() ? " -D HAVE_SRC_CONT" : "",
                         _mask.isContinuous() ? " -D HAVE_MASK_CONT" : "", kercn,
                         needMinVal ? " -D NEED_MINVAL" : "", needMaxVal ? " -D NEED_MAXVAL" : "",
                         needMinLoc ? " -D NEED_MINLOC" : "", needMaxLoc ? " -D NEED_MAXLOC" : "",
                         ocl::typeToStr(ddepth), ocl::typeToStr(CV_MAKE_TYPE(ddepth, kercn)),
                         ocl::convertTypeStr(depth, ddepth, kercn, cvt[0], পরিমাণ(cvt[0])),
                         abs_মানসমূহ ? " -D OP_ABS" : "",
                         haveSrc2 ? " -D HAVE_SRC2" : "", maxVal2 ? " -D OP_CALC2" : "",
                         haveSrc2 && _src2.isContinuous() ? " -D HAVE_SRC2_CONT" : "", ddepth,
                         depth <= CV_32S && ddepth == CV_32S ? ocl::convertTypeStr(CV_8U, ddepth, kercn, cvt[1], পরিমাণ(cvt[1])) : "noconvert",
                         MINMAX_STRUCT_ALIGNMENT);

    ocl::Kernel k("minmaxloc", ocl::core::minmaxloc_oclsrc, opts);
    যদি (k.empty())
        ঘূৰাই মিছা;

    পূর্ণাঙ্ক esz = CV_ELEM_SIZE(ddepth), esz32s = CV_ELEM_SIZE1(CV_32S),
    dbsize = groupnum * ((needMinVal ? esz : 0) + (needMaxVal ? esz : 0) +
                         (needMinLoc ? esz32s : 0) + (needMaxLoc ? esz32s : 0) +
                         (maxVal2 ? esz : 0))
    + 5 * MINMAX_STRUCT_ALIGNMENT;
    UMat src = _src.getUMat(), src2 = _src2.getUMat(), db(1, dbsize, CV_8UC1), mask = _mask.getUMat();

    যদি (cn > 1 && !haveMask)
    {
        src = src.reshape(1);
        src2 = src2.reshape(1);
    }

    যদি (haveSrc2)
    {
        যদি (!haveMask)
            k.args(ocl::KernelArg::ReadOnlyNoSize(src), src.cols, (পূর্ণাঙ্ক)src.total(),
                   groupnum, ocl::KernelArg::PtrWriteOnly(db), ocl::KernelArg::ReadOnlyNoSize(src2));
        অন্যথা
            k.args(ocl::KernelArg::ReadOnlyNoSize(src), src.cols, (পূর্ণাঙ্ক)src.total(),
                   groupnum, ocl::KernelArg::PtrWriteOnly(db), ocl::KernelArg::ReadOnlyNoSize(mask),
                   ocl::KernelArg::ReadOnlyNoSize(src2));
    }
    অন্যথা
    {
        যদি (!haveMask)
            k.args(ocl::KernelArg::ReadOnlyNoSize(src), src.cols, (পূর্ণাঙ্ক)src.total(),
                   groupnum, ocl::KernelArg::PtrWriteOnly(db));
        অন্যথা
            k.args(ocl::KernelArg::ReadOnlyNoSize(src), src.cols, (পূর্ণাঙ্ক)src.total(),
                   groupnum, ocl::KernelArg::PtrWriteOnly(db), ocl::KernelArg::ReadOnlyNoSize(mask));
    }

    size_t globalsize = groupnum * wgs;
    যদি (!k.run(1, &globalsize, &wgs, সঁচা))
        ঘূৰাই মিছা;

    স্থির ধ্রুৱ getMinMaxResFunc functab[7] =
    {
        getMinMaxRes<uchar>,
        getMinMaxRes<ৱর্ণ>,
        getMinMaxRes<ushort>,
        getMinMaxRes<short>,
        getMinMaxRes<পূর্ণাঙ্ক>,
        getMinMaxRes<ভিন্ন>,
        getMinMaxRes<দ্ৱিগুণ>
    };

    CV_Assert(ddepth <= CV_64F);
    getMinMaxResFunc func = functab[ddepth];

    পূর্ণাঙ্ক locTemp[2];
    func(db.getMat(ACCESS_READ), minVal, maxVal,
         needMinLoc ? minLoc ? minLoc : locTemp : minLoc,
         needMaxLoc ? maxLoc ? maxLoc : locTemp : maxLoc,
         groupnum, src.cols, maxVal2);

    ঘূৰাই সঁচা;
}

#endif

}

// serial implementation, also used for the pieces of the parallel version
স্থির শূন্য minMaxIdx_(cv::প্ৰবেশ_Array _src, দ্ৱিগুণ* minVal,
                       দ্ৱিগুণ* maxVal, পূর্ণাঙ্ক* minIdx, পূর্ণাঙ্ক* maxIdx,
                       cv::প্ৰবেশ_Array _mask)
{
    using নামাকাশ cv;
    পূর্ণাঙ্ক type = _src.type(), depth = CV_MAT_DEPTH(type), cn = CV_MAT_CN(type);
    CV_Assert( (cn == 1 && (_mask.empty() || _mask.type() == CV_8U || _mask.type() == CV_8S || _mask.type() == CV_Bool)) ||
               (cn > 1 && _mask.empty() && !minIdx && !maxIdx) );

    CV_OCL_RUN(OCL_PERFORMANCE_CHECK(_src.isUMat()) && _src.dims() <= 2  && (_mask.empty() || _src.size() == _mask.size()),
               ocl_minMaxIdx(_src, minVal, maxVal, minIdx, maxIdx, _mask))

    Mat src = _src.getMat(), mask = _mask.getMat();

    যদি (src.dims <= 2)
    {
        যদি ((size_t)src.step == (size_t)mask.step || mask.empty())
        {
            CALL_HAL(minMaxIdx, cv_hal_minMaxIdx, src.data, src.step, src.cols*cn, src.rows,
                     src.depth(), minVal, maxVal, minIdx, maxIdx, mask.data);
        }
        CALL_HAL(minMaxIdxMaskStep, cv_hal_minMaxIdxMaskStep, src.data, src.step, src.cols*cn, src.rows,
                 src.depth(), minVal, maxVal, minIdx, maxIdx, mask.data, mask.step);
    }
    অন্যথা যদি (src.isContinuous() && (mask.isContinuous() || mask.empty()))
    {
        পূর্ণাঙ্ক res = cv_hal_minMaxIdx(src.data, 0, (পূর্ণাঙ্ক)src.total()*cn, 1, src.depth(),
                                   minVal, maxVal, minIdx, maxIdx, mask.data);

        যদি (res == CV_HAL_ERROR_OK)
        {
            // minIdx[0] and minIdx[0] are always 0 for "flatten" version
            যদি (minIdx)
                ofs2idx(src, minIdx[1]+1, minIdx);
            যদি (maxIdx)
                ofs2idx(src, maxIdx[1]+1, maxIdx);
            ঘূৰাই;
        }
        অন্যথা যদি (res != CV_HAL_ERROR_NOT_IMPLEMENTED)
        {
            CV_Error_(cv::Error::StsInternal,
            ("HAL implementation minMaxIdx ==> " CVAUX_STR(cv_hal_minMaxIdx) " returned %d (0x%08x)", res, res));
        }
    }

    MinMaxIdxFunc func = getMinMaxIdxFunc(depth);
    CV_Assert( func != 0 );

    ধ্রুৱ Mat* arrays[] = {&src, &mask, 0};
    uchar* ptrs[2] = {};
    NAryMatIterator it(arrays, ptrs);

    size_t minidx = 0, maxidx = 0;
    size_t startidx = 1;
    সঙ্ঘ {
        পূর্ণাঙ্ক i;
        ভিন্ন f;
        দ্ৱিগুণ d;
        int64 L;
        uint64 UL;
    } minval, maxval;
    পূর্ণাঙ্ক planeSize = (পূর্ণাঙ্ক)it.size*cn;
    minval.L = maxval.L = 0;

    বাবে( size_t i = 0; i < it.nplanes; i++, ++it, startidx += planeSize )
        func( ptrs[0], ptrs[1], &minval.L, &maxval.L, &minidx, &maxidx, planeSize, startidx );

    দ্ৱিগুণ dminval, dmaxval;
    যদি( depth <= CV_32S || depth == CV_Bool )
        dminval = minval.i, dmaxval = maxval.i;
    অন্যথা যদি( depth == CV_32F || depth == CV_16F || depth == CV_16BF )
        dminval = minval.f, dmaxval = maxval.f;
    অন্যথা যদি( depth == CV_64F )
        dminval = minval.d, dmaxval = maxval.d;
    অন্যথা যদি( depth == CV_64S || depth == CV_32U )
        dminval = (দ্ৱিগুণ)minval.L, dmaxval = (দ্ৱিগুণ)maxval.L;
    অন্যথা {
        CV_Assert(depth == CV_64U);
        dminval = (দ্ৱিগুণ)minval.UL, dmaxval = (দ্ৱিগুণ)maxval.UL;
    }

    যদি( minVal )
        *minVal = dminval;
    যদি( maxVal )
        *maxVal = dmaxval;

    যদি( minIdx )
        ofs2idx(src, minidx, minIdx);
    যদি( maxIdx )
        ofs2idx(src, maxidx, maxIdx);
}

শূন্য cv::minMaxIdx(প্ৰবেশ_Array _src, দ্ৱিগুণ* minVal,
                   দ্ৱিগুণ* maxVal, পূর্ণাঙ্ক* minIdx, পূর্ণাঙ্ক* maxIdx,
                   প্ৰবেশ_Array _mask)
{
    CV_INSTRUMENT_REGION();

    std::vector<StatChunk> chunks;
    যদি (!_src.isUMat())
    {
        Mat src = _src.getMat(), mask = _mask.getMat();
        ধ্রুৱ পূর্ণাঙ্ক cn = src.channels();
        ধ্রুৱ তার্কিক argsOk = (cn == 1 && (mask.empty() || ((mask.type() == CV_8U || mask.type() == CV_8S ||
                             mask.type() == CV_Bool) && src.size == mask.size))) ||
                            (cn > 1 && mask.empty() && !minIdx && !maxIdx);
        যদি (argsOk && splitForParallelStat(src, mask, chunks))
        {
            ধ্রুৱ পূর্ণাঙ্ক n = (পূর্ণাঙ্ক)chunks.size();
            std::vector<দ্ৱিগুণ> mn(n), mx(n);
            std::vector<size_t> imn(n, 0), imx(n, 0);   // 1-based row-major indices, 0 = no element
            parallel_for_(Range(0, n), [&](ধ্রুৱ Range& r)
            {
                বাবে (পূর্ণাঙ্ক i = r.start; i < r.end; i++)
                {
                    পূর্ণাঙ্ক a[2] = {-1, -1}, b[2] = {-1, -1};
                    minMaxIdx_(chunks[i].src, &mn[i], &mx[i], cn == 1 ? a : nullptr, cn == 1 ? b : nullptr,
                               chunks[i].mask);
                    যদি (cn > 1)
                        imn[i] = imx[i] = 1;
                    অন্যথা যদি (a[0] >= 0)
                    {
                        imn[i] = chunkIndexToElem(chunks[i], a) + 1;
                        imx[i] = chunkIndexToElem(chunks[i], b) + 1;
                    }
                }
            });
            // pieces are in memory order, so taking the first strict improvement keeps the
            // first occurrence, as in the serial version
            পূর্ণাঙ্ক bmin = -1, bmax = -1;
            বাবে (পূর্ণাঙ্ক i = 0; i < n; i++)
            {
                যদি (imn[i] == 0)
                    অনুৱর্ত;
                যদি (bmin < 0 || mn[i] < mn[bmin]) bmin = i;
                যদি (bmax < 0 || mx[i] > mx[bmax]) bmax = i;
            }
            যদি (minVal) *minVal = bmin >= 0 ? mn[bmin] : 0;
            যদি (maxVal) *maxVal = bmax >= 0 ? mx[bmax] : 0;
            যদি (minIdx) ofs2idx(src, bmin >= 0 ? imn[bmin] : 0, minIdx);
            যদি (maxIdx) ofs2idx(src, bmax >= 0 ? imx[bmax] : 0, maxIdx);
            ঘূৰাই;
        }
    }
    minMaxIdx_(_src, minVal, maxVal, minIdx, maxIdx, _mask);
}

শূন্য cv::minMaxLoc( প্ৰবেশ_Array _img, দ্ৱিগুণ* minVal, দ্ৱিগুণ* maxVal,
                    Point* minLoc, Point* maxLoc, প্ৰবেশ_Array mask )
{
    CV_INSTRUMENT_REGION();

    পূর্ণাঙ্ক dims = _img.dims();
    CV_CheckLE(dims, 2, "");

    minMaxIdx(_img, minVal, maxVal, (পূর্ণাঙ্ক*)minLoc, (পূর্ণাঙ্ক*)maxLoc, mask);
    যদি( minLoc) {
        যদি (dims == 2)
            std::swap(minLoc->x, minLoc->y);
        অন্যথা {
            minLoc->y = 0;
        }
    }
    যদি( maxLoc) {
        যদি (dims == 2)
            std::swap(maxLoc->x, maxLoc->y);
        অন্যথা {
            maxLoc->y = 0;
        }
    }
}

গণনা শ্ৰেণী ReduceMode
{
    FIRST_MIN = 0, //!< get index of first min occurrence
    LAST_MIN  = 1, //!< get index of last min occurrence
    FIRST_MAX = 2, //!< get index of first max occurrence
    LAST_MAX  = 3, //!< get index of last max occurrence
};

template <typename T>
সংরচনা reduceMinMaxImpl
{
    শূন্য operator()(ধ্রুৱ cv::Mat& src, cv::Mat& dst, ReduceMode mode, ধ্রুৱ পূর্ণাঙ্ক axis) ধ্রুৱ
    {
        ৱিকল্প(mode)
        {
        পক্ষ ReduceMode::FIRST_MIN:
            reduceMinMaxApply<std::less>(src, dst, axis);
            ভঙ্গ;
        পক্ষ ReduceMode::LAST_MIN:
            reduceMinMaxApply<std::less_equal>(src, dst, axis);
            ভঙ্গ;
        পক্ষ ReduceMode::FIRST_MAX:
            reduceMinMaxApply<std::greater>(src, dst, axis);
            ভঙ্গ;
        পক্ষ ReduceMode::LAST_MAX:
            reduceMinMaxApply<std::greater_equal>(src, dst, axis);
            ভঙ্গ;
        }
    }

    template <template<শ্ৰেণী> শ্ৰেণী Cmp>
    স্থির শূন্য reduceMinMaxApply(ধ্রুৱ cv::Mat& src, cv::Mat& dst, ধ্রুৱ পূর্ণাঙ্ক axis)
    {
        Cmp<T> cmp;

        ধ্রুৱ auto *src_ptr = src.ptr<T>();
        auto *dst_ptr = dst.ptr<int32_t>();

        ধ্রুৱ size_t outer_size = src.total(0, axis);
        ধ্রুৱ auto mid_size = static_cast<size_t>(src.size[axis]);

        ধ্রুৱ size_t outer_step = src.total(axis);
        ধ্রুৱ size_t dst_step = dst.total(axis);

        ধ্রুৱ size_t mid_step = src.total(axis + 1);

        বাবে (size_t outer = 0; outer < outer_size; ++outer)
        {
            ধ্রুৱ size_t outer_offset = outer * outer_step;
            ধ্রুৱ size_t dst_offset = outer * dst_step;
            বাবে (size_t mid = 0; mid != mid_size; ++mid)
            {
                ধ্রুৱ size_t src_offset = outer_offset + mid * mid_step;
                বাবে (size_t inner = 0; inner < mid_step; inner++)
                {
                    int32_t& index = dst_ptr[dst_offset + inner];

                    ধ্রুৱ size_t prev = outer_offset + index * mid_step + inner;
                    ধ্রুৱ size_t curr = src_offset + inner;

                    যদি (cmp(src_ptr[curr], src_ptr[prev]))
                    {
                        index = static_cast<int32_t>(mid);
                    }
                }
            }
        }
    }
};

স্থির শূন্য reduceMinMax(cv::প্ৰবেশ_Array src, cv::OutputArray dst, ReduceMode mode, পূর্ণাঙ্ক axis)
{
    CV_INSTRUMENT_REGION();

    cv::Mat srcMat = src.getMat();
    পূর্ণাঙ্ক dims = std::max(1, srcMat.dims);
    axis = (axis + dims) % dims;
    CV_Assert(srcMat.channels() == 1 && axis >= 0 && axis <= srcMat.dims);

    std::vector<পূর্ণাঙ্ক> sizes(srcMat.dims);
    std::copy(srcMat.size.p, srcMat.size.p + srcMat.dims, sizes.begin());
    যদি(!sizes.empty())
        sizes[axis] = 1;

    dst.create(srcMat.dims, sizes.data(), CV_32SC1); // indices
    cv::Mat dstMat = dst.getMat();
    dstMat.setTo(cv::Scalar::all(0));

    যদি (!srcMat.isContinuous())
    {
        srcMat = srcMat.clone();
    }

    তার্কিক needs_copy = !dstMat.isContinuous();
    যদি (needs_copy)
    {
        dstMat = dstMat.clone();
    }

    cv::detail::depthDispatch<reduceMinMaxImpl>(srcMat.depth(), srcMat, dstMat, mode, axis);

    যদি (needs_copy)
    {
        dstMat.copyTo(dst);
    }
}

শূন্য cv::reduceArgMin(প্ৰবেশ_Array src, OutputArray dst, পূর্ণাঙ্ক axis, তার্কিক lastIndex)
{
    reduceMinMax(src, dst, lastIndex ? ReduceMode::LAST_MIN : ReduceMode::FIRST_MIN, axis);
}

শূন্য cv::reduceArgMax(প্ৰবেশ_Array src, OutputArray dst, পূর্ণাঙ্ক axis, তার্কিক lastIndex)
{
    reduceMinMax(src, dst, lastIndex ? ReduceMode::LAST_MAX : ReduceMode::FIRST_MAX, axis);
}
