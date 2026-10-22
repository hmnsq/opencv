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

स्थिर MinMaxIdxFunc getMinMaxIdxFunc(पूर्णांक depth)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getMinMaxIdxFunc, (depth),
                    CV_CPU_DISPATCH_MODES_ALL);
}

// The function expects 1-based indexing for ofs
// Zero is treated as invalid offset (not found)
स्थिर शून्य ofs2idx(स्थिरांक Mat& a, माप_प्रकार ofs, पूर्णांक* idx)
{
    पूर्णांक i, d = a.dims;
    जर( ofs > 0 )
    {
        ofs--;
        साठी( i = d-1; i >= 0; i-- )
        {
            पूर्णांक sz = a.size[i];
            idx[i] = (पूर्णांक)(ofs % sz);
            ofs /= sz;
        }
    }
    अन्यथा
    {
        साठी( i = d-1; i >= 0; i-- )
            idx[i] = -1;
    }
}

#ifdef HAVE_OPENCL

#define MINMAX_STRUCT_ALIGNMENT 8 // sizeof double

ढाँचा <typename T>
शून्य getMinMaxRes(स्थिरांक Mat & db, द्विगुण * minVal, द्विगुण * maxVal,
                  पूर्णांक* minLoc, पूर्णांक* maxLoc,
                  पूर्णांक groupnum, पूर्णांक cols, द्विगुण * maxVal2)
{
    uint index_max = std::numeric_limits<uint>::max();
    T minval = std::numeric_limits<T>::max();
    T maxval = std::numeric_limits<T>::min() > 0 ? (T)-std::numeric_limits<T>::max() : std::numeric_limits<T>::min(), maxval2 = maxval;
    uint minloc = index_max, maxloc = index_max;

    माप_प्रकार index = 0;
    स्थिरांक T * minptr = रिक्त, * maxptr = रिक्त, * maxptr2 = रिक्त;
    स्थिरांक uint * minlocptr = रिक्त, * maxlocptr = रिक्त;
    जर (minVal || minLoc)
    {
        minptr = db.ptr<T>();
        index += आकार(T) * groupnum;
        index = alignSize(index, MINMAX_STRUCT_ALIGNMENT);
    }
    जर (maxVal || maxLoc)
    {
        maxptr = (स्थिरांक T *)(db.ptr() + index);
        index += आकार(T) * groupnum;
        index = alignSize(index, MINMAX_STRUCT_ALIGNMENT);
    }
    जर (minLoc)
    {
        minlocptr = (स्थिरांक uint *)(db.ptr() + index);
        index += आकार(uint) * groupnum;
        index = alignSize(index, MINMAX_STRUCT_ALIGNMENT);
    }
    जर (maxLoc)
    {
        maxlocptr = (स्थिरांक uint *)(db.ptr() + index);
        index += आकार(uint) * groupnum;
        index = alignSize(index, MINMAX_STRUCT_ALIGNMENT);
    }
    जर (maxVal2)
        maxptr2 = (स्थिरांक T *)(db.ptr() + index);

    साठी (पूर्णांक i = 0; i < groupnum; i++)
    {
        जर (minptr && minptr[i] <= minval)
        {
            जर (minptr[i] == minval)
            {
                जर (minlocptr)
                    minloc = std::min(minlocptr[i], minloc);
            }
            अन्यथा
            {
                जर (minlocptr)
                    minloc = minlocptr[i];
                minval = minptr[i];
            }
        }
        जर (maxptr && maxptr[i] >= maxval)
        {
            जर (maxptr[i] == maxval)
            {
                जर (maxlocptr)
                    maxloc = std::min(maxlocptr[i], maxloc);
            }
            अन्यथा
            {
                जर (maxlocptr)
                    maxloc = maxlocptr[i];
                maxval = maxptr[i];
            }
        }
        जर (maxptr2 && maxptr2[i] > maxval2)
            maxval2 = maxptr2[i];
    }
    तार्किक zero_mask = (minLoc && minloc == index_max) ||
    (maxLoc && maxloc == index_max);

    जर (minVal)
        *minVal = zero_mask ? 0 : (द्विगुण)minval;
    जर (maxVal)
        *maxVal = zero_mask ? 0 : (द्विगुण)maxval;
    जर (maxVal2)
        *maxVal2 = zero_mask ? 0 : (द्विगुण)maxval2;

    जर (minLoc)
    {
        minLoc[0] = zero_mask ? -1 : minloc / cols;
        minLoc[1] = zero_mask ? -1 : minloc % cols;
    }
    जर (maxLoc)
    {
        maxLoc[0] = zero_mask ? -1 : maxloc / cols;
        maxLoc[1] = zero_mask ? -1 : maxloc % cols;
    }
}

प्रकारव्याख्या शून्य (*getMinMaxResFunc)(स्थिरांक Mat & db, द्विगुण * minVal, द्विगुण * maxVal,
                                 पूर्णांक * minLoc, पूर्णांक *maxLoc, पूर्णांक gropunum, पूर्णांक cols, द्विगुण * maxVal2);

तार्किक ocl_minMaxIdx( आदान_Array _src, द्विगुण* minVal, द्विगुण* maxVal, पूर्णांक* minLoc, पूर्णांक* maxLoc, आदान_Array _mask,
                   पूर्णांक ddepth, तार्किक abs_मूल्ये, आदान_Array _src2, द्विगुण * maxVal2)
{
    स्थिरांक ocl::Device & dev = ocl::Device::getDefault();

#ifdef __ANDROID__
    जर (dev.isNVidia())
        परत असत्य;
#endif

    जर (dev.deviceVersionMajor() == 1 && dev.deviceVersionMinor() < 2)
    {
        // 'static' storage class specifier used by "minmaxloc" is available from OpenCL 1.2+ only
        परत असत्य;
    }

    तार्किक doubleSupport = dev.doubleFPConfig() > 0, haveMask = !_mask.empty(),
    haveSrc2 = _src2.kind() != आदान_Array_2::NONE;
    पूर्णांक type = _src.type(), depth = CV_MAT_DEPTH(type), cn = CV_MAT_CN(type),
    kercn = haveMask ? cn : std::min(4, ocl::predictOptimalVectorWidth(_src, _src2));

    जर (depth >= CV_16F)
        परत असत्य;

    // disabled following modes since it occasionally fails on AMD devices (e.g. A10-6800K, sep. 2014)
    जर ((haveMask || type == CV_32FC1) && dev.isAMD())
        परत असत्य;

    CV_Assert( (cn == 1 && (!haveMask || _mask.type() == CV_8U || _mask.type() == CV_8S || _mask.type() == CV_Bool)) ||
              (cn >= 1 && !minLoc && !maxLoc) );

    जर (ddepth < 0)
        ddepth = depth;

    CV_Assert(!haveSrc2 || _src2.type() == type);

    जर (depth == CV_32S || depth == CV_8S || depth == CV_32U || depth == CV_64U ||
        depth == CV_64S || depth == CV_16F || depth == CV_16BF)
        परत असत्य;

    जर ((depth == CV_64F || ddepth == CV_64F) && !doubleSupport)
        परत असत्य;

    पूर्णांक groupnum = dev.maxComputeUnits();
    माप_प्रकार wgs = dev.maxWorkGroupSize();

    पूर्णांक wgs2_aligned = 1;
    पर्यंत (wgs2_aligned < (पूर्णांक)wgs)
        wgs2_aligned <<= 1;
    wgs2_aligned >>= 1;

    तार्किक needMinVal = minVal || minLoc, needMinLoc = minLoc != रिक्त,
    needMaxVal = maxVal || maxLoc, needMaxLoc = maxLoc != रिक्त;

    // in case of mask we must know whether mask is filled with zeros or not
    // so let's calculate min or max location, if it's undefined, so mask is zeros
    जर (!(needMaxLoc || needMinLoc) && haveMask)
    {
        जर (needMinVal)
            needMinLoc = सत्य;
        अन्यथा
            needMaxLoc = सत्य;
    }

    अक्षर cvt[2][50];
    String opts = format("-D DEPTH_%d -D srcT1=%s%s -D WGS=%d -D srcT=%s"
                         " -D WGS2_ALIGNED=%d%s%s%s -D kercn=%d%s%s%s%s"
                         " -D dstT1=%s -D dstT=%s -D convertToDT=%s%s%s%s%s -D wdepth=%d -D convertFromU=%s"
                         " -D MINMAX_STRUCT_ALIGNMENT=%d",
                         depth, ocl::typeToStr(depth), haveMask ? " -D HAVE_MASK" : "", (पूर्णांक)wgs,
                         ocl::typeToStr(CV_MAKE_TYPE(depth, kercn)), wgs2_aligned,
                         doubleSupport ? " -D DOUBLE_SUPPORT" : "",
                         _src.isContinuous() ? " -D HAVE_SRC_CONT" : "",
                         _mask.isContinuous() ? " -D HAVE_MASK_CONT" : "", kercn,
                         needMinVal ? " -D NEED_MINVAL" : "", needMaxVal ? " -D NEED_MAXVAL" : "",
                         needMinLoc ? " -D NEED_MINLOC" : "", needMaxLoc ? " -D NEED_MAXLOC" : "",
                         ocl::typeToStr(ddepth), ocl::typeToStr(CV_MAKE_TYPE(ddepth, kercn)),
                         ocl::convertTypeStr(depth, ddepth, kercn, cvt[0], आकार(cvt[0])),
                         abs_मूल्ये ? " -D OP_ABS" : "",
                         haveSrc2 ? " -D HAVE_SRC2" : "", maxVal2 ? " -D OP_CALC2" : "",
                         haveSrc2 && _src2.isContinuous() ? " -D HAVE_SRC2_CONT" : "", ddepth,
                         depth <= CV_32S && ddepth == CV_32S ? ocl::convertTypeStr(CV_8U, ddepth, kercn, cvt[1], आकार(cvt[1])) : "noconvert",
                         MINMAX_STRUCT_ALIGNMENT);

    ocl::Kernel k("minmaxloc", ocl::core::minmaxloc_oclsrc, opts);
    जर (k.empty())
        परत असत्य;

    पूर्णांक esz = CV_ELEM_SIZE(ddepth), esz32s = CV_ELEM_SIZE1(CV_32S),
    dbsize = groupnum * ((needMinVal ? esz : 0) + (needMaxVal ? esz : 0) +
                         (needMinLoc ? esz32s : 0) + (needMaxLoc ? esz32s : 0) +
                         (maxVal2 ? esz : 0))
    + 5 * MINMAX_STRUCT_ALIGNMENT;
    UMat src = _src.getUMat(), src2 = _src2.getUMat(), db(1, dbsize, CV_8UC1), mask = _mask.getUMat();

    जर (cn > 1 && !haveMask)
    {
        src = src.reshape(1);
        src2 = src2.reshape(1);
    }

    जर (haveSrc2)
    {
        जर (!haveMask)
            k.args(ocl::KernelArg::ReadOnlyNoSize(src), src.cols, (पूर्णांक)src.total(),
                   groupnum, ocl::KernelArg::PtrWriteOnly(db), ocl::KernelArg::ReadOnlyNoSize(src2));
        अन्यथा
            k.args(ocl::KernelArg::ReadOnlyNoSize(src), src.cols, (पूर्णांक)src.total(),
                   groupnum, ocl::KernelArg::PtrWriteOnly(db), ocl::KernelArg::ReadOnlyNoSize(mask),
                   ocl::KernelArg::ReadOnlyNoSize(src2));
    }
    अन्यथा
    {
        जर (!haveMask)
            k.args(ocl::KernelArg::ReadOnlyNoSize(src), src.cols, (पूर्णांक)src.total(),
                   groupnum, ocl::KernelArg::PtrWriteOnly(db));
        अन्यथा
            k.args(ocl::KernelArg::ReadOnlyNoSize(src), src.cols, (पूर्णांक)src.total(),
                   groupnum, ocl::KernelArg::PtrWriteOnly(db), ocl::KernelArg::ReadOnlyNoSize(mask));
    }

    माप_प्रकार globalsize = groupnum * wgs;
    जर (!k.run(1, &globalsize, &wgs, सत्य))
        परत असत्य;

    स्थिर स्थिरांक getMinMaxResFunc functab[7] =
    {
        getMinMaxRes<uchar>,
        getMinMaxRes<अक्षर>,
        getMinMaxRes<ushort>,
        getMinMaxRes<लघु>,
        getMinMaxRes<पूर्णांक>,
        getMinMaxRes<दशांश>,
        getMinMaxRes<द्विगुण>
    };

    CV_Assert(ddepth <= CV_64F);
    getMinMaxResFunc func = functab[ddepth];

    पूर्णांक locTemp[2];
    func(db.getMat(ACCESS_READ), minVal, maxVal,
         needMinLoc ? minLoc ? minLoc : locTemp : minLoc,
         needMaxLoc ? maxLoc ? maxLoc : locTemp : maxLoc,
         groupnum, src.cols, maxVal2);

    परत सत्य;
}

#endif

}

// serial implementation, also used for the pieces of the parallel version
स्थिर शून्य minMaxIdx_(cv::आदान_Array _src, द्विगुण* minVal,
                       द्विगुण* maxVal, पूर्णांक* minIdx, पूर्णांक* maxIdx,
                       cv::आदान_Array _mask)
{
    using नामाकाश cv;
    पूर्णांक type = _src.type(), depth = CV_MAT_DEPTH(type), cn = CV_MAT_CN(type);
    CV_Assert( (cn == 1 && (_mask.empty() || _mask.type() == CV_8U || _mask.type() == CV_8S || _mask.type() == CV_Bool)) ||
               (cn > 1 && _mask.empty() && !minIdx && !maxIdx) );

    CV_OCL_RUN(OCL_PERFORMANCE_CHECK(_src.isUMat()) && _src.dims() <= 2  && (_mask.empty() || _src.size() == _mask.size()),
               ocl_minMaxIdx(_src, minVal, maxVal, minIdx, maxIdx, _mask))

    Mat src = _src.getMat(), mask = _mask.getMat();

    जर (src.dims <= 2)
    {
        जर ((माप_प्रकार)src.step == (माप_प्रकार)mask.step || mask.empty())
        {
            CALL_HAL(minMaxIdx, cv_hal_minMaxIdx, src.data, src.step, src.cols*cn, src.rows,
                     src.depth(), minVal, maxVal, minIdx, maxIdx, mask.data);
        }
        CALL_HAL(minMaxIdxMaskStep, cv_hal_minMaxIdxMaskStep, src.data, src.step, src.cols*cn, src.rows,
                 src.depth(), minVal, maxVal, minIdx, maxIdx, mask.data, mask.step);
    }
    अन्यथा जर (src.isContinuous() && (mask.isContinuous() || mask.empty()))
    {
        पूर्णांक res = cv_hal_minMaxIdx(src.data, 0, (पूर्णांक)src.total()*cn, 1, src.depth(),
                                   minVal, maxVal, minIdx, maxIdx, mask.data);

        जर (res == CV_HAL_ERROR_OK)
        {
            // minIdx[0] and minIdx[0] are always 0 for "flatten" version
            जर (minIdx)
                ofs2idx(src, minIdx[1]+1, minIdx);
            जर (maxIdx)
                ofs2idx(src, maxIdx[1]+1, maxIdx);
            परत;
        }
        अन्यथा जर (res != CV_HAL_ERROR_NOT_IMPLEMENTED)
        {
            CV_Error_(cv::Error::StsInternal,
            ("HAL implementation minMaxIdx ==> " CVAUX_STR(cv_hal_minMaxIdx) " returned %d (0x%08x)", res, res));
        }
    }

    MinMaxIdxFunc func = getMinMaxIdxFunc(depth);
    CV_Assert( func != 0 );

    स्थिरांक Mat* arrays[] = {&src, &mask, 0};
    uchar* ptrs[2] = {};
    NAryMatIterator it(arrays, ptrs);

    माप_प्रकार minidx = 0, maxidx = 0;
    माप_प्रकार startidx = 1;
    संघ {
        पूर्णांक i;
        दशांश f;
        द्विगुण d;
        int64 L;
        uint64 UL;
    } minval, maxval;
    पूर्णांक planeSize = (पूर्णांक)it.size*cn;
    minval.L = maxval.L = 0;

    साठी( माप_प्रकार i = 0; i < it.nplanes; i++, ++it, startidx += planeSize )
        func( ptrs[0], ptrs[1], &minval.L, &maxval.L, &minidx, &maxidx, planeSize, startidx );

    द्विगुण dminval, dmaxval;
    जर( depth <= CV_32S || depth == CV_Bool )
        dminval = minval.i, dmaxval = maxval.i;
    अन्यथा जर( depth == CV_32F || depth == CV_16F || depth == CV_16BF )
        dminval = minval.f, dmaxval = maxval.f;
    अन्यथा जर( depth == CV_64F )
        dminval = minval.d, dmaxval = maxval.d;
    अन्यथा जर( depth == CV_64S || depth == CV_32U )
        dminval = (द्विगुण)minval.L, dmaxval = (द्विगुण)maxval.L;
    अन्यथा {
        CV_Assert(depth == CV_64U);
        dminval = (द्विगुण)minval.UL, dmaxval = (द्विगुण)maxval.UL;
    }

    जर( minVal )
        *minVal = dminval;
    जर( maxVal )
        *maxVal = dmaxval;

    जर( minIdx )
        ofs2idx(src, minidx, minIdx);
    जर( maxIdx )
        ofs2idx(src, maxidx, maxIdx);
}

शून्य cv::minMaxIdx(आदान_Array _src, द्विगुण* minVal,
                   द्विगुण* maxVal, पूर्णांक* minIdx, पूर्णांक* maxIdx,
                   आदान_Array _mask)
{
    CV_INSTRUMENT_REGION();

    std::vector<StatChunk> chunks;
    जर (!_src.isUMat())
    {
        Mat src = _src.getMat(), mask = _mask.getMat();
        स्थिरांक पूर्णांक cn = src.channels();
        स्थिरांक तार्किक argsOk = (cn == 1 && (mask.empty() || ((mask.type() == CV_8U || mask.type() == CV_8S ||
                             mask.type() == CV_Bool) && src.size == mask.size))) ||
                            (cn > 1 && mask.empty() && !minIdx && !maxIdx);
        जर (argsOk && splitForParallelStat(src, mask, chunks))
        {
            स्थिरांक पूर्णांक n = (पूर्णांक)chunks.size();
            std::vector<द्विगुण> mn(n), mx(n);
            std::vector<माप_प्रकार> imn(n, 0), imx(n, 0);   // 1-based row-major indices, 0 = no element
            parallel_for_(Range(0, n), [&](स्थिरांक Range& r)
            {
                साठी (पूर्णांक i = r.start; i < r.end; i++)
                {
                    पूर्णांक a[2] = {-1, -1}, b[2] = {-1, -1};
                    minMaxIdx_(chunks[i].src, &mn[i], &mx[i], cn == 1 ? a : nullptr, cn == 1 ? b : nullptr,
                               chunks[i].mask);
                    जर (cn > 1)
                        imn[i] = imx[i] = 1;
                    अन्यथा जर (a[0] >= 0)
                    {
                        imn[i] = chunkIndexToElem(chunks[i], a) + 1;
                        imx[i] = chunkIndexToElem(chunks[i], b) + 1;
                    }
                }
            });
            // pieces are in memory order, so taking the first strict improvement keeps the
            // first occurrence, as in the serial version
            पूर्णांक bmin = -1, bmax = -1;
            साठी (पूर्णांक i = 0; i < n; i++)
            {
                जर (imn[i] == 0)
                    सुरूठेवा;
                जर (bmin < 0 || mn[i] < mn[bmin]) bmin = i;
                जर (bmax < 0 || mx[i] > mx[bmax]) bmax = i;
            }
            जर (minVal) *minVal = bmin >= 0 ? mn[bmin] : 0;
            जर (maxVal) *maxVal = bmax >= 0 ? mx[bmax] : 0;
            जर (minIdx) ofs2idx(src, bmin >= 0 ? imn[bmin] : 0, minIdx);
            जर (maxIdx) ofs2idx(src, bmax >= 0 ? imx[bmax] : 0, maxIdx);
            परत;
        }
    }
    minMaxIdx_(_src, minVal, maxVal, minIdx, maxIdx, _mask);
}

शून्य cv::minMaxLoc( आदान_Array _img, द्विगुण* minVal, द्विगुण* maxVal,
                    Point* minLoc, Point* maxLoc, आदान_Array mask )
{
    CV_INSTRUMENT_REGION();

    पूर्णांक dims = _img.dims();
    CV_CheckLE(dims, 2, "");

    minMaxIdx(_img, minVal, maxVal, (पूर्णांक*)minLoc, (पूर्णांक*)maxLoc, mask);
    जर( minLoc) {
        जर (dims == 2)
            std::swap(minLoc->x, minLoc->y);
        अन्यथा {
            minLoc->y = 0;
        }
    }
    जर( maxLoc) {
        जर (dims == 2)
            std::swap(maxLoc->x, maxLoc->y);
        अन्यथा {
            maxLoc->y = 0;
        }
    }
}

गणना वर्ग ReduceMode
{
    FIRST_MIN = 0, //!< get index of first min occurrence
    LAST_MIN  = 1, //!< get index of last min occurrence
    FIRST_MAX = 2, //!< get index of first max occurrence
    LAST_MAX  = 3, //!< get index of last max occurrence
};

ढाँचा <typename T>
संरचना reduceMinMaxImpl
{
    शून्य चालक()(स्थिरांक cv::Mat& src, cv::Mat& dst, ReduceMode mode, स्थिरांक पूर्णांक axis) स्थिरांक
    {
        निवड(mode)
        {
        स्थिती ReduceMode::FIRST_MIN:
            reduceMinMaxApply<std::less>(src, dst, axis);
            थांबा;
        स्थिती ReduceMode::LAST_MIN:
            reduceMinMaxApply<std::less_equal>(src, dst, axis);
            थांबा;
        स्थिती ReduceMode::FIRST_MAX:
            reduceMinMaxApply<std::greater>(src, dst, axis);
            थांबा;
        स्थिती ReduceMode::LAST_MAX:
            reduceMinMaxApply<std::greater_equal>(src, dst, axis);
            थांबा;
        }
    }

    ढाँचा <ढाँचा<वर्ग> वर्ग Cmp>
    स्थिर शून्य reduceMinMaxApply(स्थिरांक cv::Mat& src, cv::Mat& dst, स्थिरांक पूर्णांक axis)
    {
        Cmp<T> cmp;

        स्थिरांक स्वतः *src_ptr = src.ptr<T>();
        स्वतः *dst_ptr = dst.ptr<int32_t>();

        स्थिरांक माप_प्रकार outer_size = src.total(0, axis);
        स्थिरांक स्वतः mid_size = static_cast<माप_प्रकार>(src.size[axis]);

        स्थिरांक माप_प्रकार outer_step = src.total(axis);
        स्थिरांक माप_प्रकार dst_step = dst.total(axis);

        स्थिरांक माप_प्रकार mid_step = src.total(axis + 1);

        साठी (माप_प्रकार outer = 0; outer < outer_size; ++outer)
        {
            स्थिरांक माप_प्रकार outer_offset = outer * outer_step;
            स्थिरांक माप_प्रकार dst_offset = outer * dst_step;
            साठी (माप_प्रकार mid = 0; mid != mid_size; ++mid)
            {
                स्थिरांक माप_प्रकार src_offset = outer_offset + mid * mid_step;
                साठी (माप_प्रकार inner = 0; inner < mid_step; inner++)
                {
                    int32_t& index = dst_ptr[dst_offset + inner];

                    स्थिरांक माप_प्रकार prev = outer_offset + index * mid_step + inner;
                    स्थिरांक माप_प्रकार curr = src_offset + inner;

                    जर (cmp(src_ptr[curr], src_ptr[prev]))
                    {
                        index = static_cast<int32_t>(mid);
                    }
                }
            }
        }
    }
};

स्थिर शून्य reduceMinMax(cv::आदान_Array src, cv::OutputArray dst, ReduceMode mode, पूर्णांक axis)
{
    CV_INSTRUMENT_REGION();

    cv::Mat srcMat = src.getMat();
    पूर्णांक dims = std::max(1, srcMat.dims);
    axis = (axis + dims) % dims;
    CV_Assert(srcMat.channels() == 1 && axis >= 0 && axis <= srcMat.dims);

    std::vector<पूर्णांक> sizes(srcMat.dims);
    std::copy(srcMat.size.p, srcMat.size.p + srcMat.dims, sizes.begin());
    जर(!sizes.empty())
        sizes[axis] = 1;

    dst.create(srcMat.dims, sizes.data(), CV_32SC1); // indices
    cv::Mat dstMat = dst.getMat();
    dstMat.setTo(cv::Scalar::all(0));

    जर (!srcMat.isContinuous())
    {
        srcMat = srcMat.clone();
    }

    तार्किक needs_copy = !dstMat.isContinuous();
    जर (needs_copy)
    {
        dstMat = dstMat.clone();
    }

    cv::detail::depthDispatch<reduceMinMaxImpl>(srcMat.depth(), srcMat, dstMat, mode, axis);

    जर (needs_copy)
    {
        dstMat.copyTo(dst);
    }
}

शून्य cv::reduceArgMin(आदान_Array src, OutputArray dst, पूर्णांक axis, तार्किक lastIndex)
{
    reduceMinMax(src, dst, lastIndex ? ReduceMode::LAST_MIN : ReduceMode::FIRST_MIN, axis);
}

शून्य cv::reduceArgMax(आदान_Array src, OutputArray dst, पूर्णांक axis, तार्किक lastIndex)
{
    reduceMinMax(src, dst, lastIndex ? ReduceMode::LAST_MAX : ReduceMode::FIRST_MAX, axis);
}
