// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


#अन्तर्भाव "precomp.hpp"
#अन्तर्भाव "opencl_kernels_core.hpp"
#अन्तर्भाव "stat.hpp"
#अन्तर्भाव "stat_parallel.hpp"
#अन्तर्भाव "opencv2/core/detail/dispatch_helper.impl.hpp"
#अन्तर्भाव <algorithm>

#अन्तर्भाव "minmax.simd.hpp"
#अन्तर्भाव "minmax.simd_declarations.hpp" // defines CV_CPU_DISPATCH_MODES_ALL=AVX2,...,BASELINE based on CMakeLists.txt content

नामाकाश cv {

स्थिर MinMaxIdxFunc getMinMaxIdxFunc(पूर्णाङ्क depth)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getMinMaxIdxFunc, (depth),
                    CV_CPU_DISPATCH_MODES_ALL);
}

// The function expects 1-based indexing for ofs
// Zero is treated as invalid offset (not found)
स्थिर शून्य ofs2idx(ध्रुव Mat& a, size_t ofs, पूर्णाङ्क* idx)
{
    पूर्णाङ्क i, d = a.dims;
    सचे( ofs > 0 )
    {
        ofs--;
        अत्थाय( i = d-1; i >= 0; i-- )
        {
            पूर्णाङ्क sz = a.size[i];
            idx[i] = (पूर्णाङ्क)(ofs % sz);
            ofs /= sz;
        }
    }
    अञ्ञथा
    {
        अत्थाय( i = d-1; i >= 0; i-- )
            idx[i] = -1;
    }
}

#ifdef HAVE_OPENCL

#define MINMAX_STRUCT_ALIGNMENT 8 // sizeof double

template <typename T>
शून्य getMinMaxRes(ध्रुव Mat & db, द्विगुण * minVal, द्विगुण * maxVal,
                  पूर्णाङ्क* minLoc, पूर्णाङ्क* maxLoc,
                  पूर्णाङ्क groupnum, पूर्णाङ्क cols, द्विगुण * maxVal2)
{
    uint index_max = std::numeric_limits<uint>::max();
    T minval = std::numeric_limits<T>::max();
    T maxval = std::numeric_limits<T>::min() > 0 ? (T)-std::numeric_limits<T>::max() : std::numeric_limits<T>::min(), maxval2 = maxval;
    uint minloc = index_max, maxloc = index_max;

    size_t index = 0;
    ध्रुव T * minptr = रिक्त, * maxptr = रिक्त, * maxptr2 = रिक्त;
    ध्रुव uint * minlocptr = रिक्त, * maxlocptr = रिक्त;
    सचे (minVal || minLoc)
    {
        minptr = db.ptr<T>();
        index += परिमाण(T) * groupnum;
        index = alignSize(index, MINMAX_STRUCT_ALIGNMENT);
    }
    सचे (maxVal || maxLoc)
    {
        maxptr = (ध्रुव T *)(db.ptr() + index);
        index += परिमाण(T) * groupnum;
        index = alignSize(index, MINMAX_STRUCT_ALIGNMENT);
    }
    सचे (minLoc)
    {
        minlocptr = (ध्रुव uint *)(db.ptr() + index);
        index += परिमाण(uint) * groupnum;
        index = alignSize(index, MINMAX_STRUCT_ALIGNMENT);
    }
    सचे (maxLoc)
    {
        maxlocptr = (ध्रुव uint *)(db.ptr() + index);
        index += परिमाण(uint) * groupnum;
        index = alignSize(index, MINMAX_STRUCT_ALIGNMENT);
    }
    सचे (maxVal2)
        maxptr2 = (ध्रुव T *)(db.ptr() + index);

    अत्थाय (पूर्णाङ्क i = 0; i < groupnum; i++)
    {
        सचे (minptr && minptr[i] <= minval)
        {
            सचे (minptr[i] == minval)
            {
                सचे (minlocptr)
                    minloc = std::min(minlocptr[i], minloc);
            }
            अञ्ञथा
            {
                सचे (minlocptr)
                    minloc = minlocptr[i];
                minval = minptr[i];
            }
        }
        सचे (maxptr && maxptr[i] >= maxval)
        {
            सचे (maxptr[i] == maxval)
            {
                सचे (maxlocptr)
                    maxloc = std::min(maxlocptr[i], maxloc);
            }
            अञ्ञथा
            {
                सचे (maxlocptr)
                    maxloc = maxlocptr[i];
                maxval = maxptr[i];
            }
        }
        सचे (maxptr2 && maxptr2[i] > maxval2)
            maxval2 = maxptr2[i];
    }
    तार्किक zero_mask = (minLoc && minloc == index_max) ||
    (maxLoc && maxloc == index_max);

    सचे (minVal)
        *minVal = zero_mask ? 0 : (द्विगुण)minval;
    सचे (maxVal)
        *maxVal = zero_mask ? 0 : (द्विगुण)maxval;
    सचे (maxVal2)
        *maxVal2 = zero_mask ? 0 : (द्विगुण)maxval2;

    सचे (minLoc)
    {
        minLoc[0] = zero_mask ? -1 : minloc / cols;
        minLoc[1] = zero_mask ? -1 : minloc % cols;
    }
    सचे (maxLoc)
    {
        maxLoc[0] = zero_mask ? -1 : maxloc / cols;
        maxLoc[1] = zero_mask ? -1 : maxloc % cols;
    }
}

प्रकारनिर्देश शून्य (*getMinMaxResFunc)(ध्रुव Mat & db, द्विगुण * minVal, द्विगुण * maxVal,
                                 पूर्णाङ्क * minLoc, पूर्णाङ्क *maxLoc, पूर्णाङ्क gropunum, पूर्णाङ्क cols, द्विगुण * maxVal2);

तार्किक ocl_minMaxIdx( निवेशः_Array _src, द्विगुण* minVal, द्विगुण* maxVal, पूर्णाङ्क* minLoc, पूर्णाङ्क* maxLoc, निवेशः_Array _mask,
                   पूर्णाङ्क ddepth, तार्किक abs_मानानि, निवेशः_Array _src2, द्विगुण * maxVal2)
{
    ध्रुव ocl::Device & dev = ocl::Device::getDefault();

#ifdef __ANDROID__
    सचे (dev.isNVidia())
        पटिनिवत्त मिच्छा;
#endif

    सचे (dev.deviceVersionMajor() == 1 && dev.deviceVersionMinor() < 2)
    {
        // 'static' storage class specifier used by "minmaxloc" is available from OpenCL 1.2+ only
        पटिनिवत्त मिच्छा;
    }

    तार्किक doubleSupport = dev.doubleFPConfig() > 0, haveMask = !_mask.empty(),
    haveSrc2 = _src2.kind() != निवेशः_Array_2::NONE;
    पूर्णाङ्क type = _src.type(), depth = CV_MAT_DEPTH(type), cn = CV_MAT_CN(type),
    kercn = haveMask ? cn : std::min(4, ocl::predictOptimalVectorWidth(_src, _src2));

    सचे (depth >= CV_16F)
        पटिनिवत्त मिच्छा;

    // disabled following modes since it occasionally fails on AMD devices (e.g. A10-6800K, sep. 2014)
    सचे ((haveMask || type == CV_32FC1) && dev.isAMD())
        पटिनिवत्त मिच्छा;

    CV_Assert( (cn == 1 && (!haveMask || _mask.type() == CV_8U || _mask.type() == CV_8S || _mask.type() == CV_Bool)) ||
              (cn >= 1 && !minLoc && !maxLoc) );

    सचे (ddepth < 0)
        ddepth = depth;

    CV_Assert(!haveSrc2 || _src2.type() == type);

    सचे (depth == CV_32S || depth == CV_8S || depth == CV_32U || depth == CV_64U ||
        depth == CV_64S || depth == CV_16F || depth == CV_16BF)
        पटिनिवत्त मिच्छा;

    सचे ((depth == CV_64F || ddepth == CV_64F) && !doubleSupport)
        पटिनिवत्त मिच्छा;

    पूर्णाङ्क groupnum = dev.maxComputeUnits();
    size_t wgs = dev.maxWorkGroupSize();

    पूर्णाङ्क wgs2_aligned = 1;
    याव (wgs2_aligned < (पूर्णाङ्क)wgs)
        wgs2_aligned <<= 1;
    wgs2_aligned >>= 1;

    तार्किक needMinVal = minVal || minLoc, needMinLoc = minLoc != रिक्त,
    needMaxVal = maxVal || maxLoc, needMaxLoc = maxLoc != रिक्त;

    // in case of mask we must know whether mask is filled with zeros or not
    // so let's calculate min or max location, if it's undefined, so mask is zeros
    सचे (!(needMaxLoc || needMinLoc) && haveMask)
    {
        सचे (needMinVal)
            needMinLoc = सच्च;
        अञ्ञथा
            needMaxLoc = सच्च;
    }

    वर्ण cvt[2][50];
    String opts = format("-D DEPTH_%d -D srcT1=%s%s -D WGS=%d -D srcT=%s"
                         " -D WGS2_ALIGNED=%d%s%s%s -D kercn=%d%s%s%s%s"
                         " -D dstT1=%s -D dstT=%s -D convertToDT=%s%s%s%s%s -D wdepth=%d -D convertFromU=%s"
                         " -D MINMAX_STRUCT_ALIGNMENT=%d",
                         depth, ocl::typeToStr(depth), haveMask ? " -D HAVE_MASK" : "", (पूर्णाङ्क)wgs,
                         ocl::typeToStr(CV_MAKE_TYPE(depth, kercn)), wgs2_aligned,
                         doubleSupport ? " -D DOUBLE_SUPPORT" : "",
                         _src.isContinuous() ? " -D HAVE_SRC_CONT" : "",
                         _mask.isContinuous() ? " -D HAVE_MASK_CONT" : "", kercn,
                         needMinVal ? " -D NEED_MINVAL" : "", needMaxVal ? " -D NEED_MAXVAL" : "",
                         needMinLoc ? " -D NEED_MINLOC" : "", needMaxLoc ? " -D NEED_MAXLOC" : "",
                         ocl::typeToStr(ddepth), ocl::typeToStr(CV_MAKE_TYPE(ddepth, kercn)),
                         ocl::convertTypeStr(depth, ddepth, kercn, cvt[0], परिमाण(cvt[0])),
                         abs_मानानि ? " -D OP_ABS" : "",
                         haveSrc2 ? " -D HAVE_SRC2" : "", maxVal2 ? " -D OP_CALC2" : "",
                         haveSrc2 && _src2.isContinuous() ? " -D HAVE_SRC2_CONT" : "", ddepth,
                         depth <= CV_32S && ddepth == CV_32S ? ocl::convertTypeStr(CV_8U, ddepth, kercn, cvt[1], परिमाण(cvt[1])) : "noconvert",
                         MINMAX_STRUCT_ALIGNMENT);

    ocl::Kernel k("minmaxloc", ocl::core::minmaxloc_oclsrc, opts);
    सचे (k.empty())
        पटिनिवत्त मिच्छा;

    पूर्णाङ्क esz = CV_ELEM_SIZE(ddepth), esz32s = CV_ELEM_SIZE1(CV_32S),
    dbsize = groupnum * ((needMinVal ? esz : 0) + (needMaxVal ? esz : 0) +
                         (needMinLoc ? esz32s : 0) + (needMaxLoc ? esz32s : 0) +
                         (maxVal2 ? esz : 0))
    + 5 * MINMAX_STRUCT_ALIGNMENT;
    UMat src = _src.getUMat(), src2 = _src2.getUMat(), db(1, dbsize, CV_8UC1), mask = _mask.getUMat();

    सचे (cn > 1 && !haveMask)
    {
        src = src.reshape(1);
        src2 = src2.reshape(1);
    }

    सचे (haveSrc2)
    {
        सचे (!haveMask)
            k.args(ocl::KernelArg::ReadOnlyNoSize(src), src.cols, (पूर्णाङ्क)src.total(),
                   groupnum, ocl::KernelArg::PtrWriteOnly(db), ocl::KernelArg::ReadOnlyNoSize(src2));
        अञ्ञथा
            k.args(ocl::KernelArg::ReadOnlyNoSize(src), src.cols, (पूर्णाङ्क)src.total(),
                   groupnum, ocl::KernelArg::PtrWriteOnly(db), ocl::KernelArg::ReadOnlyNoSize(mask),
                   ocl::KernelArg::ReadOnlyNoSize(src2));
    }
    अञ्ञथा
    {
        सचे (!haveMask)
            k.args(ocl::KernelArg::ReadOnlyNoSize(src), src.cols, (पूर्णाङ्क)src.total(),
                   groupnum, ocl::KernelArg::PtrWriteOnly(db));
        अञ्ञथा
            k.args(ocl::KernelArg::ReadOnlyNoSize(src), src.cols, (पूर्णाङ्क)src.total(),
                   groupnum, ocl::KernelArg::PtrWriteOnly(db), ocl::KernelArg::ReadOnlyNoSize(mask));
    }

    size_t globalsize = groupnum * wgs;
    सचे (!k.run(1, &globalsize, &wgs, सच्च))
        पटिनिवत्त मिच्छा;

    स्थिर ध्रुव getMinMaxResFunc functab[7] =
    {
        getMinMaxRes<uchar>,
        getMinMaxRes<वर्ण>,
        getMinMaxRes<ushort>,
        getMinMaxRes<short>,
        getMinMaxRes<पूर्णाङ्क>,
        getMinMaxRes<भिन्न>,
        getMinMaxRes<द्विगुण>
    };

    CV_Assert(ddepth <= CV_64F);
    getMinMaxResFunc func = functab[ddepth];

    पूर्णाङ्क locTemp[2];
    func(db.getMat(ACCESS_READ), minVal, maxVal,
         needMinLoc ? minLoc ? minLoc : locTemp : minLoc,
         needMaxLoc ? maxLoc ? maxLoc : locTemp : maxLoc,
         groupnum, src.cols, maxVal2);

    पटिनिवत्त सच्च;
}

#endif

}

// serial implementation, also used for the pieces of the parallel version
स्थिर शून्य minMaxIdx_(cv::निवेशः_Array _src, द्विगुण* minVal,
                       द्विगुण* maxVal, पूर्णाङ्क* minIdx, पूर्णाङ्क* maxIdx,
                       cv::निवेशः_Array _mask)
{
    using नामाकाश cv;
    पूर्णाङ्क type = _src.type(), depth = CV_MAT_DEPTH(type), cn = CV_MAT_CN(type);
    CV_Assert( (cn == 1 && (_mask.empty() || _mask.type() == CV_8U || _mask.type() == CV_8S || _mask.type() == CV_Bool)) ||
               (cn > 1 && _mask.empty() && !minIdx && !maxIdx) );

    CV_OCL_RUN(OCL_PERFORMANCE_CHECK(_src.isUMat()) && _src.dims() <= 2  && (_mask.empty() || _src.size() == _mask.size()),
               ocl_minMaxIdx(_src, minVal, maxVal, minIdx, maxIdx, _mask))

    Mat src = _src.getMat(), mask = _mask.getMat();

    सचे (src.dims <= 2)
    {
        सचे ((size_t)src.step == (size_t)mask.step || mask.empty())
        {
            CALL_HAL(minMaxIdx, cv_hal_minMaxIdx, src.data, src.step, src.cols*cn, src.rows,
                     src.depth(), minVal, maxVal, minIdx, maxIdx, mask.data);
        }
        CALL_HAL(minMaxIdxMaskStep, cv_hal_minMaxIdxMaskStep, src.data, src.step, src.cols*cn, src.rows,
                 src.depth(), minVal, maxVal, minIdx, maxIdx, mask.data, mask.step);
    }
    अञ्ञथा सचे (src.isContinuous() && (mask.isContinuous() || mask.empty()))
    {
        पूर्णाङ्क res = cv_hal_minMaxIdx(src.data, 0, (पूर्णाङ्क)src.total()*cn, 1, src.depth(),
                                   minVal, maxVal, minIdx, maxIdx, mask.data);

        सचे (res == CV_HAL_ERROR_OK)
        {
            // minIdx[0] and minIdx[0] are always 0 for "flatten" version
            सचे (minIdx)
                ofs2idx(src, minIdx[1]+1, minIdx);
            सचे (maxIdx)
                ofs2idx(src, maxIdx[1]+1, maxIdx);
            पटिनिवत्त;
        }
        अञ्ञथा सचे (res != CV_HAL_ERROR_NOT_IMPLEMENTED)
        {
            CV_Error_(cv::Error::StsInternal,
            ("HAL implementation minMaxIdx ==> " CVAUX_STR(cv_hal_minMaxIdx) " returned %d (0x%08x)", res, res));
        }
    }

    MinMaxIdxFunc func = getMinMaxIdxFunc(depth);
    CV_Assert( func != 0 );

    ध्रुव Mat* arrays[] = {&src, &mask, 0};
    uchar* ptrs[2] = {};
    NAryMatIterator it(arrays, ptrs);

    size_t minidx = 0, maxidx = 0;
    size_t startidx = 1;
    सङ्घ {
        पूर्णाङ्क i;
        भिन्न f;
        द्विगुण d;
        int64 L;
        uint64 UL;
    } minval, maxval;
    पूर्णाङ्क planeSize = (पूर्णाङ्क)it.size*cn;
    minval.L = maxval.L = 0;

    अत्थाय( size_t i = 0; i < it.nplanes; i++, ++it, startidx += planeSize )
        func( ptrs[0], ptrs[1], &minval.L, &maxval.L, &minidx, &maxidx, planeSize, startidx );

    द्विगुण dminval, dmaxval;
    सचे( depth <= CV_32S || depth == CV_Bool )
        dminval = minval.i, dmaxval = maxval.i;
    अञ्ञथा सचे( depth == CV_32F || depth == CV_16F || depth == CV_16BF )
        dminval = minval.f, dmaxval = maxval.f;
    अञ्ञथा सचे( depth == CV_64F )
        dminval = minval.d, dmaxval = maxval.d;
    अञ्ञथा सचे( depth == CV_64S || depth == CV_32U )
        dminval = (द्विगुण)minval.L, dmaxval = (द्विगुण)maxval.L;
    अञ्ञथा {
        CV_Assert(depth == CV_64U);
        dminval = (द्विगुण)minval.UL, dmaxval = (द्विगुण)maxval.UL;
    }

    सचे( minVal )
        *minVal = dminval;
    सचे( maxVal )
        *maxVal = dmaxval;

    सचे( minIdx )
        ofs2idx(src, minidx, minIdx);
    सचे( maxIdx )
        ofs2idx(src, maxidx, maxIdx);
}

शून्य cv::minMaxIdx(निवेशः_Array _src, द्विगुण* minVal,
                   द्विगुण* maxVal, पूर्णाङ्क* minIdx, पूर्णाङ्क* maxIdx,
                   निवेशः_Array _mask)
{
    CV_INSTRUMENT_REGION();

    std::vector<StatChunk> chunks;
    सचे (!_src.isUMat())
    {
        Mat src = _src.getMat(), mask = _mask.getMat();
        ध्रुव पूर्णाङ्क cn = src.channels();
        ध्रुव तार्किक argsOk = (cn == 1 && (mask.empty() || ((mask.type() == CV_8U || mask.type() == CV_8S ||
                             mask.type() == CV_Bool) && src.size == mask.size))) ||
                            (cn > 1 && mask.empty() && !minIdx && !maxIdx);
        सचे (argsOk && splitForParallelStat(src, mask, chunks))
        {
            ध्रुव पूर्णाङ्क n = (पूर्णाङ्क)chunks.size();
            std::vector<द्विगुण> mn(n), mx(n);
            std::vector<size_t> imn(n, 0), imx(n, 0);   // 1-based row-major indices, 0 = no element
            parallel_for_(Range(0, n), [&](ध्रुव Range& r)
            {
                अत्थाय (पूर्णाङ्क i = r.start; i < r.end; i++)
                {
                    पूर्णाङ्क a[2] = {-1, -1}, b[2] = {-1, -1};
                    minMaxIdx_(chunks[i].src, &mn[i], &mx[i], cn == 1 ? a : nullptr, cn == 1 ? b : nullptr,
                               chunks[i].mask);
                    सचे (cn > 1)
                        imn[i] = imx[i] = 1;
                    अञ्ञथा सचे (a[0] >= 0)
                    {
                        imn[i] = chunkIndexToElem(chunks[i], a) + 1;
                        imx[i] = chunkIndexToElem(chunks[i], b) + 1;
                    }
                }
            });
            // pieces are in memory order, so taking the first strict improvement keeps the
            // first occurrence, as in the serial version
            पूर्णाङ्क bmin = -1, bmax = -1;
            अत्थाय (पूर्णाङ्क i = 0; i < n; i++)
            {
                सचे (imn[i] == 0)
                    अनुवर्त;
                सचे (bmin < 0 || mn[i] < mn[bmin]) bmin = i;
                सचे (bmax < 0 || mx[i] > mx[bmax]) bmax = i;
            }
            सचे (minVal) *minVal = bmin >= 0 ? mn[bmin] : 0;
            सचे (maxVal) *maxVal = bmax >= 0 ? mx[bmax] : 0;
            सचे (minIdx) ofs2idx(src, bmin >= 0 ? imn[bmin] : 0, minIdx);
            सचे (maxIdx) ofs2idx(src, bmax >= 0 ? imx[bmax] : 0, maxIdx);
            पटिनिवत्त;
        }
    }
    minMaxIdx_(_src, minVal, maxVal, minIdx, maxIdx, _mask);
}

शून्य cv::minMaxLoc( निवेशः_Array _img, द्विगुण* minVal, द्विगुण* maxVal,
                    Point* minLoc, Point* maxLoc, निवेशः_Array mask )
{
    CV_INSTRUMENT_REGION();

    पूर्णाङ्क dims = _img.dims();
    CV_CheckLE(dims, 2, "");

    minMaxIdx(_img, minVal, maxVal, (पूर्णाङ्क*)minLoc, (पूर्णाङ्क*)maxLoc, mask);
    सचे( minLoc) {
        सचे (dims == 2)
            std::swap(minLoc->x, minLoc->y);
        अञ्ञथा {
            minLoc->y = 0;
        }
    }
    सचे( maxLoc) {
        सचे (dims == 2)
            std::swap(maxLoc->x, maxLoc->y);
        अञ्ञथा {
            maxLoc->y = 0;
        }
    }
}

गणना वग्ग ReduceMode
{
    FIRST_MIN = 0, //!< get index of first min occurrence
    LAST_MIN  = 1, //!< get index of last min occurrence
    FIRST_MAX = 2, //!< get index of first max occurrence
    LAST_MAX  = 3, //!< get index of last max occurrence
};

template <typename T>
संरचना reduceMinMaxImpl
{
    शून्य operator()(ध्रुव cv::Mat& src, cv::Mat& dst, ReduceMode mode, ध्रुव पूर्णाङ्क axis) ध्रुव
    {
        विकल्प(mode)
        {
        पक्ष ReduceMode::FIRST_MIN:
            reduceMinMaxApply<std::less>(src, dst, axis);
            भङ्ग;
        पक्ष ReduceMode::LAST_MIN:
            reduceMinMaxApply<std::less_equal>(src, dst, axis);
            भङ्ग;
        पक्ष ReduceMode::FIRST_MAX:
            reduceMinMaxApply<std::greater>(src, dst, axis);
            भङ्ग;
        पक्ष ReduceMode::LAST_MAX:
            reduceMinMaxApply<std::greater_equal>(src, dst, axis);
            भङ्ग;
        }
    }

    template <template<वग्ग> वग्ग Cmp>
    स्थिर शून्य reduceMinMaxApply(ध्रुव cv::Mat& src, cv::Mat& dst, ध्रुव पूर्णाङ्क axis)
    {
        Cmp<T> cmp;

        ध्रुव auto *src_ptr = src.ptr<T>();
        auto *dst_ptr = dst.ptr<int32_t>();

        ध्रुव size_t outer_size = src.total(0, axis);
        ध्रुव auto mid_size = static_cast<size_t>(src.size[axis]);

        ध्रुव size_t outer_step = src.total(axis);
        ध्रुव size_t dst_step = dst.total(axis);

        ध्रुव size_t mid_step = src.total(axis + 1);

        अत्थाय (size_t outer = 0; outer < outer_size; ++outer)
        {
            ध्रुव size_t outer_offset = outer * outer_step;
            ध्रुव size_t dst_offset = outer * dst_step;
            अत्थाय (size_t mid = 0; mid != mid_size; ++mid)
            {
                ध्रुव size_t src_offset = outer_offset + mid * mid_step;
                अत्थाय (size_t inner = 0; inner < mid_step; inner++)
                {
                    int32_t& index = dst_ptr[dst_offset + inner];

                    ध्रुव size_t prev = outer_offset + index * mid_step + inner;
                    ध्रुव size_t curr = src_offset + inner;

                    सचे (cmp(src_ptr[curr], src_ptr[prev]))
                    {
                        index = static_cast<int32_t>(mid);
                    }
                }
            }
        }
    }
};

स्थिर शून्य reduceMinMax(cv::निवेशः_Array src, cv::OutputArray dst, ReduceMode mode, पूर्णाङ्क axis)
{
    CV_INSTRUMENT_REGION();

    cv::Mat srcMat = src.getMat();
    पूर्णाङ्क dims = std::max(1, srcMat.dims);
    axis = (axis + dims) % dims;
    CV_Assert(srcMat.channels() == 1 && axis >= 0 && axis <= srcMat.dims);

    std::vector<पूर्णाङ्क> sizes(srcMat.dims);
    std::copy(srcMat.size.p, srcMat.size.p + srcMat.dims, sizes.begin());
    सचे(!sizes.empty())
        sizes[axis] = 1;

    dst.create(srcMat.dims, sizes.data(), CV_32SC1); // indices
    cv::Mat dstMat = dst.getMat();
    dstMat.setTo(cv::Scalar::all(0));

    सचे (!srcMat.isContinuous())
    {
        srcMat = srcMat.clone();
    }

    तार्किक needs_copy = !dstMat.isContinuous();
    सचे (needs_copy)
    {
        dstMat = dstMat.clone();
    }

    cv::detail::depthDispatch<reduceMinMaxImpl>(srcMat.depth(), srcMat, dstMat, mode, axis);

    सचे (needs_copy)
    {
        dstMat.copyTo(dst);
    }
}

शून्य cv::reduceArgMin(निवेशः_Array src, OutputArray dst, पूर्णाङ्क axis, तार्किक lastIndex)
{
    reduceMinMax(src, dst, lastIndex ? ReduceMode::LAST_MIN : ReduceMode::FIRST_MIN, axis);
}

शून्य cv::reduceArgMax(निवेशः_Array src, OutputArray dst, पूर्णाङ्क axis, तार्किक lastIndex)
{
    reduceMinMax(src, dst, lastIndex ? ReduceMode::LAST_MAX : ReduceMode::FIRST_MAX, axis);
}
