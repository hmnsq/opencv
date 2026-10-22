// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


#অন্তর্ভাব "precomp.hpp"
#অন্তর্ভাব "opencl_kernels_core.hpp"
#অন্তর্ভাব "stat.hpp"
#অন্তর্ভাব "stat_parallel.hpp"
#অন্তর্ভাব "opencv2/core/detail/dispatch_helper.impl.hpp"
#অন্তর্ভাব <algorithm>

#অন্তর্ভাব "minmax.simd.hpp"
#অন্তর্ভাব "minmax.simd_declarations.hpp" // defines CV_CPU_DISPATCH_MODES_ALL=AVX2,...,BASELINE based on CMakeLists.txt content

নামাকাশ cv {

স্থির MinMaxIdxFunc getMinMaxIdxFunc(পূর্ণসংখ্যা depth)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getMinMaxIdxFunc, (depth),
                    CV_CPU_DISPATCH_MODES_ALL);
}

// The function expects 1-based indexing for ofs
// Zero is treated as invalid offset (not found)
স্থির শূন্য ofs2idx(ধ্রুবক Mat& a, size_t ofs, পূর্ণসংখ্যা* idx)
{
    পূর্ণসংখ্যা i, d = a.dims;
    যদি( ofs > 0 )
    {
        ofs--;
        জন্য( i = d-1; i >= 0; i-- )
        {
            পূর্ণসংখ্যা sz = a.size[i];
            idx[i] = (পূর্ণসংখ্যা)(ofs % sz);
            ofs /= sz;
        }
    }
    অন্যথা
    {
        জন্য( i = d-1; i >= 0; i-- )
            idx[i] = -1;
    }
}

#ifdef HAVE_OPENCL

#define MINMAX_STRUCT_ALIGNMENT 8 // sizeof double

template <typename T>
শূন্য getMinMaxRes(ধ্রুবক Mat & db, দ্বিগুণ * minVal, দ্বিগুণ * maxVal,
                  পূর্ণসংখ্যা* minLoc, পূর্ণসংখ্যা* maxLoc,
                  পূর্ণসংখ্যা groupnum, পূর্ণসংখ্যা cols, দ্বিগুণ * maxVal2)
{
    uint সূচক_max = std::numeric_limits<uint>::max();
    T minval = std::numeric_limits<T>::max();
    T maxval = std::numeric_limits<T>::min() > 0 ? (T)-std::numeric_limits<T>::max() : std::numeric_limits<T>::min(), maxval2 = maxval;
    uint minloc = সূচক_max, maxloc = সূচক_max;

    size_t সূচক = 0;
    ধ্রুবক T * minptr = ফাঁকা, * maxptr = ফাঁকা, * maxptr2 = ফাঁকা;
    ধ্রুবক uint * minlocptr = ফাঁকা, * maxlocptr = ফাঁকা;
    যদি (minVal || minLoc)
    {
        minptr = db.ptr<T>();
        সূচক += আকার(T) * groupnum;
        সূচক = align_আকার(সূচক, MINMAX_STRUCT_ALIGNMENT);
    }
    যদি (maxVal || maxLoc)
    {
        maxptr = (ধ্রুবক T *)(db.ptr() + সূচক);
        সূচক += আকার(T) * groupnum;
        সূচক = align_আকার(সূচক, MINMAX_STRUCT_ALIGNMENT);
    }
    যদি (minLoc)
    {
        minlocptr = (ধ্রুবক uint *)(db.ptr() + সূচক);
        সূচক += আকার(uint) * groupnum;
        সূচক = align_আকার(সূচক, MINMAX_STRUCT_ALIGNMENT);
    }
    যদি (maxLoc)
    {
        maxlocptr = (ধ্রুবক uint *)(db.ptr() + সূচক);
        সূচক += আকার(uint) * groupnum;
        সূচক = align_আকার(সূচক, MINMAX_STRUCT_ALIGNMENT);
    }
    যদি (maxVal2)
        maxptr2 = (ধ্রুবক T *)(db.ptr() + সূচক);

    জন্য (পূর্ণসংখ্যা i = 0; i < groupnum; i++)
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
    যৌক্তিক zero_mask = (minLoc && minloc == সূচক_max) ||
    (maxLoc && maxloc == সূচক_max);

    যদি (minVal)
        *minVal = zero_mask ? 0 : (দ্বিগুণ)minval;
    যদি (maxVal)
        *maxVal = zero_mask ? 0 : (দ্বিগুণ)maxval;
    যদি (maxVal2)
        *maxVal2 = zero_mask ? 0 : (দ্বিগুণ)maxval2;

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

প্রকারলেখ শূন্য (*getMinMaxResFunc)(ধ্রুবক Mat & db, দ্বিগুণ * minVal, দ্বিগুণ * maxVal,
                                 পূর্ণসংখ্যা * minLoc, পূর্ণসংখ্যা *maxLoc, পূর্ণসংখ্যা gropunum, পূর্ণসংখ্যা cols, দ্বিগুণ * maxVal2);

যৌক্তিক ocl_minMaxIdx( প্রবেশ_Array _src, দ্বিগুণ* minVal, দ্বিগুণ* maxVal, পূর্ণসংখ্যা* minLoc, পূর্ণসংখ্যা* maxLoc, প্রবেশ_Array _mask,
                   পূর্ণসংখ্যা ddepth, যৌক্তিক abs_মানসমূহ, প্রবেশ_Array _src2, দ্বিগুণ * maxVal2)
{
    ধ্রুবক ocl::Device & dev = ocl::Device::getDefault();

#ifdef __ANDROID__
    যদি (dev.isNVidia())
        ফেরত মিথ্যা;
#endif

    যদি (dev.deviceVersionMajor() == 1 && dev.deviceVersionMinor() < 2)
    {
        // 'static' storage class specifier used by "minmaxloc" is available from OpenCL 1.2+ only
        ফেরত মিথ্যা;
    }

    যৌক্তিক doubleSupport = dev.doubleFPConfig() > 0, haveMask = !_mask.empty(),
    haveSrc2 = _src2.kind() != প্রবেশ_Array_2::NONE;
    পূর্ণসংখ্যা type = _src.type(), depth = CV_MAT_DEPTH(type), cn = CV_MAT_CN(type),
    kercn = haveMask ? cn : std::min(4, ocl::predictOptimalVectorWidth(_src, _src2));

    যদি (depth >= CV_16F)
        ফেরত মিথ্যা;

    // disabled following modes since it occasionally fails on AMD devices (e.g. A10-6800K, sep. 2014)
    যদি ((haveMask || type == CV_32FC1) && dev.isAMD())
        ফেরত মিথ্যা;

    CV_Assert( (cn == 1 && (!haveMask || _mask.type() == CV_8U || _mask.type() == CV_8S || _mask.type() == CV_Bool)) ||
              (cn >= 1 && !minLoc && !maxLoc) );

    যদি (ddepth < 0)
        ddepth = depth;

    CV_Assert(!haveSrc2 || _src2.type() == type);

    যদি (depth == CV_32S || depth == CV_8S || depth == CV_32U || depth == CV_64U ||
        depth == CV_64S || depth == CV_16F || depth == CV_16BF)
        ফেরত মিথ্যা;

    যদি ((depth == CV_64F || ddepth == CV_64F) && !doubleSupport)
        ফেরত মিথ্যা;

    পূর্ণসংখ্যা groupnum = dev.maxComputeUnits();
    size_t wgs = dev.max_Work_Group_আকার();

    পূর্ণসংখ্যা wgs2_aligned = 1;
    যখন (wgs2_aligned < (পূর্ণসংখ্যা)wgs)
        wgs2_aligned <<= 1;
    wgs2_aligned >>= 1;

    যৌক্তিক needMinVal = minVal || minLoc, needMinLoc = minLoc != ফাঁকা,
    needMaxVal = maxVal || maxLoc, needMaxLoc = maxLoc != ফাঁকা;

    // in case of mask we must know whether mask is filled with zeros or not
    // so let's calculate min or max location, if it's undefined, so mask is zeros
    যদি (!(needMaxLoc || needMinLoc) && haveMask)
    {
        যদি (needMinVal)
            needMinLoc = সত্য;
        অন্যথা
            needMaxLoc = সত্য;
    }

    অক্ষর cvt[2][50];
    String opts = format("-D DEPTH_%d -D srcT1=%s%s -D WGS=%d -D srcT=%s"
                         " -D WGS2_ALIGNED=%d%s%s%s -D kercn=%d%s%s%s%s"
                         " -D dstT1=%s -D dstT=%s -D convertToDT=%s%s%s%s%s -D wdepth=%d -D convertFromU=%s"
                         " -D MINMAX_STRUCT_ALIGNMENT=%d",
                         depth, ocl::typeToStr(depth), haveMask ? " -D HAVE_MASK" : "", (পূর্ণসংখ্যা)wgs,
                         ocl::typeToStr(CV_MAKE_TYPE(depth, kercn)), wgs2_aligned,
                         doubleSupport ? " -D DOUBLE_SUPPORT" : "",
                         _src.isContinuous() ? " -D HAVE_SRC_CONT" : "",
                         _mask.isContinuous() ? " -D HAVE_MASK_CONT" : "", kercn,
                         needMinVal ? " -D NEED_MINVAL" : "", needMaxVal ? " -D NEED_MAXVAL" : "",
                         needMinLoc ? " -D NEED_MINLOC" : "", needMaxLoc ? " -D NEED_MAXLOC" : "",
                         ocl::typeToStr(ddepth), ocl::typeToStr(CV_MAKE_TYPE(ddepth, kercn)),
                         ocl::convertTypeStr(depth, ddepth, kercn, cvt[0], আকার(cvt[0])),
                         abs_মানসমূহ ? " -D OP_ABS" : "",
                         haveSrc2 ? " -D HAVE_SRC2" : "", maxVal2 ? " -D OP_CALC2" : "",
                         haveSrc2 && _src2.isContinuous() ? " -D HAVE_SRC2_CONT" : "", ddepth,
                         depth <= CV_32S && ddepth == CV_32S ? ocl::convertTypeStr(CV_8U, ddepth, kercn, cvt[1], আকার(cvt[1])) : "noconvert",
                         MINMAX_STRUCT_ALIGNMENT);

    ocl::Kernel k("minmaxloc", ocl::core::minmaxloc_oclsrc, opts);
    যদি (k.empty())
        ফেরত মিথ্যা;

    পূর্ণসংখ্যা esz = CV_ELEM_আকার(ddepth), esz32s = CV_ELEM_SIZE1(CV_32S),
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
            k.args(ocl::KernelArg::Read_Only_No_আকার(src), src.cols, (পূর্ণসংখ্যা)src.যোগ(),
                   groupnum, ocl::KernelArg::PtrWriteOnly(db), ocl::KernelArg::Read_Only_No_আকার(src2));
        অন্যথা
            k.args(ocl::KernelArg::Read_Only_No_আকার(src), src.cols, (পূর্ণসংখ্যা)src.যোগ(),
                   groupnum, ocl::KernelArg::PtrWriteOnly(db), ocl::KernelArg::Read_Only_No_আকার(mask),
                   ocl::KernelArg::Read_Only_No_আকার(src2));
    }
    অন্যথা
    {
        যদি (!haveMask)
            k.args(ocl::KernelArg::Read_Only_No_আকার(src), src.cols, (পূর্ণসংখ্যা)src.যোগ(),
                   groupnum, ocl::KernelArg::PtrWriteOnly(db));
        অন্যথা
            k.args(ocl::KernelArg::Read_Only_No_আকার(src), src.cols, (পূর্ণসংখ্যা)src.যোগ(),
                   groupnum, ocl::KernelArg::PtrWriteOnly(db), ocl::KernelArg::Read_Only_No_আকার(mask));
    }

    size_t globalsize = groupnum * wgs;
    যদি (!k.run(1, &globalsize, &wgs, সত্য))
        ফেরত মিথ্যা;

    স্থির ধ্রুবক getMinMaxResFunc functab[7] =
    {
        getMinMaxRes<uchar>,
        getMinMaxRes<অক্ষর>,
        getMinMaxRes<ushort>,
        getMinMaxRes<short>,
        getMinMaxRes<পূর্ণসংখ্যা>,
        getMinMaxRes<দশমিক>,
        getMinMaxRes<দ্বিগুণ>
    };

    CV_Assert(ddepth <= CV_64F);
    getMinMaxResFunc func = functab[ddepth];

    পূর্ণসংখ্যা loc_অস্থায়ী[2];
    func(db.getMat(ACCESS_READ), minVal, maxVal,
         needMinLoc ? minLoc ? minLoc : loc_অস্থায়ী : minLoc,
         needMaxLoc ? maxLoc ? maxLoc : loc_অস্থায়ী : maxLoc,
         groupnum, src.cols, maxVal2);

    ফেরত সত্য;
}

#endif

}

// serial implementation, also used for the pieces of the parallel version
স্থির শূন্য minMaxIdx_(cv::প্রবেশ_Array _src, দ্বিগুণ* minVal,
                       দ্বিগুণ* maxVal, পূর্ণসংখ্যা* minIdx, পূর্ণসংখ্যা* maxIdx,
                       cv::প্রবেশ_Array _mask)
{
    using নামাকাশ cv;
    পূর্ণসংখ্যা type = _src.type(), depth = CV_MAT_DEPTH(type), cn = CV_MAT_CN(type);
    CV_Assert( (cn == 1 && (_mask.empty() || _mask.type() == CV_8U || _mask.type() == CV_8S || _mask.type() == CV_Bool)) ||
               (cn > 1 && _mask.empty() && !minIdx && !maxIdx) );

    CV_OCL_RUN(OCL_PERFORMANCE_CHECK(_src.isUMat()) && _src.dims() <= 2  && (_mask.empty() || _src.size() == _mask.size()),
               ocl_minMaxIdx(_src, minVal, maxVal, minIdx, maxIdx, _mask))

    Mat src = _src.getMat(), mask = _mask.getMat();

    যদি (src.dims <= 2)
    {
        যদি ((size_t)src.step == (size_t)mask.step || mask.empty())
        {
            CALL_HAL(minMaxIdx, cv_hal_minMaxIdx, src.তথ্য, src.step, src.cols*cn, src.rows,
                     src.depth(), minVal, maxVal, minIdx, maxIdx, mask.তথ্য);
        }
        CALL_HAL(minMaxIdxMaskStep, cv_hal_minMaxIdxMaskStep, src.তথ্য, src.step, src.cols*cn, src.rows,
                 src.depth(), minVal, maxVal, minIdx, maxIdx, mask.তথ্য, mask.step);
    }
    অন্যথা যদি (src.isContinuous() && (mask.isContinuous() || mask.empty()))
    {
        পূর্ণসংখ্যা res = cv_hal_minMaxIdx(src.তথ্য, 0, (পূর্ণসংখ্যা)src.যোগ()*cn, 1, src.depth(),
                                   minVal, maxVal, minIdx, maxIdx, mask.তথ্য);

        যদি (res == CV_HAL_ERROR_OK)
        {
            // minIdx[0] and minIdx[0] are always 0 for "flatten" version
            যদি (minIdx)
                ofs2idx(src, minIdx[1]+1, minIdx);
            যদি (maxIdx)
                ofs2idx(src, maxIdx[1]+1, maxIdx);
            ফেরত;
        }
        অন্যথা যদি (res != CV_HAL_ERROR_NOT_IMPLEMENTED)
        {
            CV_Error_(cv::Error::StsInternal,
            ("HAL implementation minMaxIdx ==> " CVAUX_STR(cv_hal_minMaxIdx) " returned %d (0x%08x)", res, res));
        }
    }

    MinMaxIdxFunc func = getMinMaxIdxFunc(depth);
    CV_Assert( func != 0 );

    ধ্রুবক Mat* arrays[] = {&src, &mask, 0};
    uchar* ptrs[2] = {};
    NAryMatIterator it(arrays, ptrs);

    size_t minidx = 0, maxidx = 0;
    size_t startidx = 1;
    সংঘ {
        পূর্ণসংখ্যা i;
        দশমিক f;
        দ্বিগুণ d;
        int64 L;
        uint64 UL;
    } minval, maxval;
    পূর্ণসংখ্যা plane_আকার = (পূর্ণসংখ্যা)it.size*cn;
    minval.L = maxval.L = 0;

    জন্য( size_t i = 0; i < it.nplanes; i++, ++it, startidx += plane_আকার )
        func( ptrs[0], ptrs[1], &minval.L, &maxval.L, &minidx, &maxidx, plane_আকার, startidx );

    দ্বিগুণ dminval, dmaxval;
    যদি( depth <= CV_32S || depth == CV_Bool )
        dminval = minval.i, dmaxval = maxval.i;
    অন্যথা যদি( depth == CV_32F || depth == CV_16F || depth == CV_16BF )
        dminval = minval.f, dmaxval = maxval.f;
    অন্যথা যদি( depth == CV_64F )
        dminval = minval.d, dmaxval = maxval.d;
    অন্যথা যদি( depth == CV_64S || depth == CV_32U )
        dminval = (দ্বিগুণ)minval.L, dmaxval = (দ্বিগুণ)maxval.L;
    অন্যথা {
        CV_Assert(depth == CV_64U);
        dminval = (দ্বিগুণ)minval.UL, dmaxval = (দ্বিগুণ)maxval.UL;
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

শূন্য cv::minMaxIdx(প্রবেশ_Array _src, দ্বিগুণ* minVal,
                   দ্বিগুণ* maxVal, পূর্ণসংখ্যা* minIdx, পূর্ণসংখ্যা* maxIdx,
                   প্রবেশ_Array _mask)
{
    CV_INSTRUMENT_REGION();

    std::vector<StatChunk> chunks;
    যদি (!_src.isUMat())
    {
        Mat src = _src.getMat(), mask = _mask.getMat();
        ধ্রুবক পূর্ণসংখ্যা cn = src.channels();
        ধ্রুবক যৌক্তিক argsOk = (cn == 1 && (mask.empty() || ((mask.type() == CV_8U || mask.type() == CV_8S ||
                             mask.type() == CV_Bool) && src.size == mask.size))) ||
                            (cn > 1 && mask.empty() && !minIdx && !maxIdx);
        যদি (argsOk && splitForParallelStat(src, mask, chunks))
        {
            ধ্রুবক পূর্ণসংখ্যা n = (পূর্ণসংখ্যা)chunks.size();
            std::vector<দ্বিগুণ> mn(n), mx(n);
            std::vector<size_t> imn(n, 0), imx(n, 0);   // 1-based row-major indices, 0 = no element
            parallel_for_(Range(0, n), [&](ধ্রুবক Range& r)
            {
                জন্য (পূর্ণসংখ্যা i = r.start; i < r.end; i++)
                {
                    পূর্ণসংখ্যা a[2] = {-1, -1}, b[2] = {-1, -1};
                    minMaxIdx_(chunks[i].src, &mn[i], &mx[i], cn == 1 ? a : nullptr, cn == 1 ? b : nullptr,
                               chunks[i].mask);
                    যদি (cn > 1)
                        imn[i] = imx[i] = 1;
                    অন্যথা যদি (a[0] >= 0)
                    {
                        imn[i] = chunk_সূচক_To_Elem(chunks[i], a) + 1;
                        imx[i] = chunk_সূচক_To_Elem(chunks[i], b) + 1;
                    }
                }
            });
            // pieces are in memory order, so taking the first strict improvement keeps the
            // first occurrence, as in the serial version
            পূর্ণসংখ্যা bmin = -1, bmax = -1;
            জন্য (পূর্ণসংখ্যা i = 0; i < n; i++)
            {
                যদি (imn[i] == 0)
                    চালু;
                যদি (bmin < 0 || mn[i] < mn[bmin]) bmin = i;
                যদি (bmax < 0 || mx[i] > mx[bmax]) bmax = i;
            }
            যদি (minVal) *minVal = bmin >= 0 ? mn[bmin] : 0;
            যদি (maxVal) *maxVal = bmax >= 0 ? mx[bmax] : 0;
            যদি (minIdx) ofs2idx(src, bmin >= 0 ? imn[bmin] : 0, minIdx);
            যদি (maxIdx) ofs2idx(src, bmax >= 0 ? imx[bmax] : 0, maxIdx);
            ফেরত;
        }
    }
    minMaxIdx_(_src, minVal, maxVal, minIdx, maxIdx, _mask);
}

শূন্য cv::minMaxLoc( প্রবেশ_Array _img, দ্বিগুণ* minVal, দ্বিগুণ* maxVal,
                    Point* minLoc, Point* maxLoc, প্রবেশ_Array mask )
{
    CV_INSTRUMENT_REGION();

    পূর্ণসংখ্যা dims = _img.dims();
    CV_CheckLE(dims, 2, "");

    minMaxIdx(_img, minVal, maxVal, (পূর্ণসংখ্যা*)minLoc, (পূর্ণসংখ্যা*)maxLoc, mask);
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

গণনা শ্রেণী ReduceMode
{
    FIRST_MIN = 0, //!< get index of first min occurrence
    LAST_MIN  = 1, //!< get index of last min occurrence
    FIRST_MAX = 2, //!< get index of first max occurrence
    LAST_MAX  = 3, //!< get index of last max occurrence
};

template <typename T>
কাঠামো reduceMinMaxImpl
{
    শূন্য operator()(ধ্রুবক cv::Mat& src, cv::Mat& dst, ReduceMode mode, ধ্রুবক পূর্ণসংখ্যা axis) ধ্রুবক
    {
        নির্বাচন(mode)
        {
        ক্ষেত্র ReduceMode::FIRST_MIN:
            reduceMinMaxApply<std::less>(src, dst, axis);
            বিরতি;
        ক্ষেত্র ReduceMode::LAST_MIN:
            reduceMinMaxApply<std::less_equal>(src, dst, axis);
            বিরতি;
        ক্ষেত্র ReduceMode::FIRST_MAX:
            reduceMinMaxApply<std::greater>(src, dst, axis);
            বিরতি;
        ক্ষেত্র ReduceMode::LAST_MAX:
            reduceMinMaxApply<std::greater_equal>(src, dst, axis);
            বিরতি;
        }
    }

    template <template<শ্রেণী> শ্রেণী Cmp>
    স্থির শূন্য reduceMinMaxApply(ধ্রুবক cv::Mat& src, cv::Mat& dst, ধ্রুবক পূর্ণসংখ্যা axis)
    {
        Cmp<T> cmp;

        ধ্রুবক auto *src_ptr = src.ptr<T>();
        auto *dst_ptr = dst.ptr<int32_t>();

        ধ্রুবক size_t outer_আকার = src.যোগ(0, axis);
        ধ্রুবক auto mid_আকার = static_cast<size_t>(src.size[axis]);

        ধ্রুবক size_t outer_step = src.যোগ(axis);
        ধ্রুবক size_t dst_step = dst.যোগ(axis);

        ধ্রুবক size_t mid_step = src.যোগ(axis + 1);

        জন্য (size_t outer = 0; outer < outer_আকার; ++outer)
        {
            ধ্রুবক size_t outer_offset = outer * outer_step;
            ধ্রুবক size_t dst_offset = outer * dst_step;
            জন্য (size_t mid = 0; mid != mid_আকার; ++mid)
            {
                ধ্রুবক size_t src_offset = outer_offset + mid * mid_step;
                জন্য (size_t inner = 0; inner < mid_step; inner++)
                {
                    int32_t& সূচক = dst_ptr[dst_offset + inner];

                    ধ্রুবক size_t পূর্ব = outer_offset + সূচক * mid_step + inner;
                    ধ্রুবক size_t curr = src_offset + inner;

                    যদি (cmp(src_ptr[curr], src_ptr[পূর্ব]))
                    {
                        সূচক = static_cast<int32_t>(mid);
                    }
                }
            }
        }
    }
};

স্থির শূন্য reduceMinMax(cv::প্রবেশ_Array src, cv::OutputArray dst, ReduceMode mode, পূর্ণসংখ্যা axis)
{
    CV_INSTRUMENT_REGION();

    cv::Mat srcMat = src.getMat();
    পূর্ণসংখ্যা dims = std::max(1, srcMat.dims);
    axis = (axis + dims) % dims;
    CV_Assert(srcMat.channels() == 1 && axis >= 0 && axis <= srcMat.dims);

    std::vector<পূর্ণসংখ্যা> sizes(srcMat.dims);
    std::copy(srcMat.size.p, srcMat.size.p + srcMat.dims, sizes.begin());
    যদি(!sizes.empty())
        sizes[axis] = 1;

    dst.create(srcMat.dims, sizes.তথ্য(), CV_32SC1); // indices
    cv::Mat dstMat = dst.getMat();
    dstMat.setTo(cv::Scalar::all(0));

    যদি (!srcMat.isContinuous())
    {
        srcMat = srcMat.clone();
    }

    যৌক্তিক needs_copy = !dstMat.isContinuous();
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

শূন্য cv::reduceArgMin(প্রবেশ_Array src, OutputArray dst, পূর্ণসংখ্যা axis, যৌক্তিক last_সূচক)
{
    reduceMinMax(src, dst, last_সূচক ? ReduceMode::LAST_MIN : ReduceMode::FIRST_MIN, axis);
}

শূন্য cv::reduceArgMax(প্রবেশ_Array src, OutputArray dst, পূর্ণসংখ্যা axis, যৌক্তিক last_সূচক)
{
    reduceMinMax(src, dst, last_সূচক ? ReduceMode::LAST_MAX : ReduceMode::FIRST_MAX, axis);
}
